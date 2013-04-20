
#define _GNU_SOURCE
#include <sys/types.h>

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#include <io.h>
#include "ultragetopt.h"
#define STDIN_FILENO 0
#define ssize_t int
#else
#include <getopt.h>
#include <unistd.h>
#endif

#include "sirfdump.h"
#include "sirf_msg.h"

const char *progname = "sirfdump";
const char *revision = "$Revision: 0.3 $";

struct opts_t {
   char *infile;
   char *outfile;
   enum {
      OUTPUT_DUMP,
      OUTPUT_NMEA,
      OUTPUT_RINEX,
      OUTPUT_RINEX_NAV,
      OUTPUT_RTCM,
   } output_type;
   unsigned gsw230_byte_order;
};

struct input_stream_t {
   int fd;
   uint8_t buf[1024];
   unsigned head, tail;
   int last_errno;
};

struct ctx_t {
   struct opts_t opts;
   struct input_stream_t in;
   FILE *outfh;
   dumpf_t *dump_f;
   void *user_ctx;
};


unsigned output_dump_use_gsw230_byte_order = 0;


static void usage(void)
{
 fprintf(stdout, "\nUsage:\n    %s [-h] [options]\n"
       ,progname);
 return;
}

static void version(void)
{
 fprintf(stdout,"%s %s\n",progname,revision);
}

static void help(void)
{

 printf("%s - Sirf binary logger\t\t%s\n",
       progname, revision);
 usage();
 printf(
   "\nOptions:\n"
   "    -f, --infile                Input file, default: - (stdin)\n"
   "    -F, --outfile               Output file, default: - (stdout)\n"
   "    -o, --outtype               Output type: dump / nmea / rinex / rinex-nav / rtcm. default: nmea\n"
   "    -2, --gsw230                Use alternate byte order that is used on GSW 2.3.0 - 2.9.9 firmwares\n"
   "    -h, --help                  Help\n"
   "    -v, --version               Show version\n"
   "\n"
 );
 return;
}

static struct ctx_t *init_ctx()
{
   struct ctx_t *ctx;

   ctx = malloc(sizeof(*ctx));

   if (ctx == NULL) {
      perror(NULL);
      return NULL;
   }
   ctx->opts.infile = ctx->opts.outfile = NULL;
   ctx->opts.output_type = OUTPUT_NMEA;
   ctx->opts.gsw230_byte_order = 0;
   ctx->in.fd = -1;
   ctx->in.head = ctx->in.tail = 0;
   ctx->in.last_errno = 0;
   ctx->outfh = NULL;
   ctx->user_ctx = NULL;

   return ctx;
}

static void free_ctx(struct ctx_t *ctx)
{
   if (ctx == NULL)
      return;
   free(ctx->opts.infile);
   free(ctx->opts.outfile);
   if (ctx->in.fd > 0 && (ctx->in.fd != STDIN_FILENO))
      close(ctx->in.fd);
   if (ctx->outfh && (ctx->outfh != stdout))
      fclose(ctx->outfh);
   free(ctx);
}


static int set_file(char **dst, const char *optarg)
{
   assert(dst);
   assert(optarg);

   free(*dst);

   if (optarg[0] == '-' && (optarg[1] == '\0')) {
      *dst = NULL;
   }else {
      *dst = strdup(optarg);
      if (*dst == NULL) {
	 perror(NULL);
	 return 1;
      }
   }
   return 0;
}

static int read_data(struct input_stream_t *stream)
{
   ssize_t l;
   assert(
      !(stream->tail == sizeof(stream->buf)
	 && (stream->head == sizeof(stream->buf)))
      );

   if (stream->tail == sizeof(stream->buf)) {
      memmove(stream->buf, &stream->buf[stream->head],
	    stream->tail - stream->head);
      stream->tail = stream->tail - stream->head;
      stream->head=0;
   }

   l = read(stream->fd, &stream->buf[stream->tail], sizeof(stream->buf) - stream->tail);
   if (l<0)
      stream->last_errno = errno;
   else
      stream->tail = stream->tail+l;

   return l;
}

void *readpkt(struct input_stream_t *stream, struct transport_msg_t *res_msg)
{
   unsigned start_seq_found;
   unsigned payload_length;
   unsigned checksum;
   unsigned garbage_bytes;
   unsigned p0;
   size_t l;
   uint8_t *res;

   garbage_bytes = 0;
   start_seq_found=0;
   payload_length=0;

   for (;;) {
      while (stream->tail - stream->head < 8) {
	 l = read_data(stream);
	 if (l <= 0)
	    return NULL;
      }

      /* search for start sequence  */
      while (stream->head < stream->tail-1) {
	 if ((stream->buf[stream->head] == 0xa0)
	       && (stream->buf[stream->head+1] == 0xa2)) {
	    start_seq_found=1;
	    break;
	 }else {
	    garbage_bytes++;
	    stream->head++;
	 }
      }
      if (!start_seq_found)
	 continue;

      while (stream->tail - stream->head < 6) {
	 l = read_data(stream);
	 if (l <= 0)
	    return NULL;
      }

      /* get payload length  */
      payload_length = (0xff00 & (stream->buf[stream->head+2] << 8))
	 | (0xff & stream->buf[stream->head+3]);
      if (payload_length >= 1023) {
	 stream->head++;
	 continue;
      }

      /* load payload data */
      p0 = 2+2+payload_length;
      while (stream->tail - stream->head < p0+2+2) {
	 l = read_data(stream);
	 if (l <= 0)
	    return NULL;
      }

      /* checksum  */
      checksum = (0xff00 & (stream->buf[stream->head+p0] << 8))
	 | (0xff & stream->buf[stream->head+p0+1]);

      /* end sequence  */
      if (stream->buf[stream->head+p0+2] != 0xb0
	    || (stream->buf[stream->head+p0+3] != 0xb3)) {
	 stream->head++;
	 continue;
      }
      break;
   } /* for(;;)  */

   res = &stream->buf[stream->head];
   stream->head = stream->head + 2+2+payload_length+2+2;
   if (stream->head == stream->tail) {
      stream->head = stream->tail = 0;
   }

   if (res_msg) {
      res_msg->payload = &res[4];
      res_msg->payload_length = payload_length;
      res_msg->checksum = checksum;
      res_msg->skipped_bytes = garbage_bytes;
   }

   return res;
}

int process(struct ctx_t *ctx)
{
   uint8_t *pkt;
   struct transport_msg_t msg;

   while ( (pkt = readpkt(&ctx->in, &msg)) != NULL ) {
      ctx->dump_f(&msg, ctx->outfh, ctx->user_ctx);
   }

   return ctx->in.last_errno;
}

int main(int argc, char *argv[])
{
   signed char c;
   struct ctx_t *ctx;

   static struct option longopts[] = {
      {"version",     no_argument,       0, 'v'},
      {"help",        no_argument,       0, 'h'},
      {"infile",      required_argument, 0, 'f'},
      {"outfile",     required_argument, 0, 'F'},
      {"outtype",     required_argument, 0, 'o'},
      {"gsw230",      no_argument,       0, '2'},
      {0, 0, 0, 0}
   };

   ctx = init_ctx();
   if (ctx == NULL)
      return 1;

#ifdef WIN32
#ifdef _MSC_VER
   _set_output_format(_TWO_DIGIT_EXPONENT);
#else
   putenv("PRINTF_EXPONENT_DIGITS=2");
#endif
#endif

   while ((c = getopt_long(argc, argv, "vh?f:F:o:2",longopts,NULL)) != -1) {
      switch (c) {
	 case 'f':
	    if (set_file(&ctx->opts.infile, optarg) != 0) {
	       free_ctx(ctx);
	       return 1;
	    }
	    break;
	 case 'F':
	    if (set_file(&ctx->opts.outfile, optarg) != 0) {
	       free_ctx(ctx);
	       return 1;
	    }
	    break;
	 case 'o':
	    if (strcmp(optarg, "nmea") == 0) {
	       ctx->opts.output_type = OUTPUT_NMEA;
	    }else if (strcmp(optarg, "dump") == 0) {
	       ctx->opts.output_type = OUTPUT_DUMP;
	    }else if (strcmp(optarg, "rinex") == 0) {
	       ctx->opts.output_type = OUTPUT_RINEX;
	    }else if (strcmp(optarg, "rinex-nav") == 0) {
	       ctx->opts.output_type = OUTPUT_RINEX_NAV;
	    }else if (strcmp(optarg, "rtcm") == 0) {
	       ctx->opts.output_type = OUTPUT_RTCM;
	    }else {
	       fputs("Wrong output type\n", stderr);
	       return 1;
	    }
	    break;
	 case '2':
	    ctx->opts.gsw230_byte_order = 1;
	    break;
	 case 'v':
	    version();
	    free_ctx(ctx);
	    exit(0);
	    break;
	 default:
	    help();
	    free_ctx(ctx);
	    exit(0);
	    break;
      }
   }
   argc -= optind;
   argv += optind;

   /* infile  */
   if (ctx->opts.infile != NULL) {
      ctx->in.fd = open(ctx->opts.infile, O_RDONLY
#ifdef O_BINARY
	 | O_BINARY
#endif
      );
      if (ctx->in.fd < 0) {
	 perror(NULL);
	 free_ctx(ctx);
	 return 1;
      }
   }else
      ctx->in.fd = STDIN_FILENO;

   /* outfile  */
   if (ctx->opts.outfile != NULL) {
      ctx->outfh = fopen(ctx->opts.outfile,
#ifdef WIN32
	"wb"
#else
	"w"
#endif
	);

      if (ctx->outfh == NULL) {
	 perror(NULL);
	 free_ctx(ctx);
	 return 1;
      }
   }else
      ctx->outfh = stdout;

   /* output_type  */
   switch (ctx->opts.output_type) {
      case OUTPUT_NMEA:
	 ctx->dump_f = &output_nmea;
	 break;
      case OUTPUT_RINEX:
	 ctx->dump_f = &output_rinex;
	 ctx->user_ctx = new_rinex_ctx(argc, argv, ctx->opts.gsw230_byte_order);
	 if (ctx->user_ctx == NULL) {
	    perror(NULL);
	    free_ctx(ctx);
	    return 1;
	 }
	 break;
      case OUTPUT_RINEX_NAV:
	 ctx->dump_f = &output_rinex_nav;
	 ctx->user_ctx = new_rinex_nav_ctx(argc, argv);
	 if (ctx->user_ctx == NULL) {
	    perror(NULL);
	    free_ctx(ctx);
	    return 1;
	 }
	 break;
      case OUTPUT_RTCM:
	 ctx->dump_f = &output_rtcm;
	 ctx->user_ctx = new_rtcm_ctx(argc, argv, ctx->opts.gsw230_byte_order);
	 setvbuf(ctx->outfh, NULL, _IONBF, 0);
	 if (ctx->user_ctx == NULL) {
	    perror(NULL);
	    free_ctx(ctx);
	    return 1;
	 }
	 break;
      case OUTPUT_DUMP:
      default:
	 output_dump_use_gsw230_byte_order = ctx->opts.gsw230_byte_order;
	 ctx->dump_f = &output_dump;
	 break;
   }

   process(ctx);

   switch (ctx->opts.output_type) {
      case OUTPUT_RINEX:
	 free_rinex_ctx(ctx->user_ctx);
	 break;
      case OUTPUT_RINEX_NAV:
	 free_rinex_nav_ctx(ctx->user_ctx);
	 break;
      case OUTPUT_RTCM:
	 free_rtcm_ctx(ctx->user_ctx);
	 break;
      default:
	 break;
   }

   free_ctx(ctx);
   return 0;
}

