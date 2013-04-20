
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "sirfdump.h"
#include "sirf_msg.h"
#include "sirf_codec.h"
#include "sirf_codec_ssb.h"

const char * const MonthName[] = {"JAN","FEB","MAR","APR", "MAY","JUN","JUL","AUG", 
   "SEP", "OCT", "NOV", "DEC"};

struct epoch_t {
      /* mid-7 */
      unsigned gps_week;
      double gps_tow;
      unsigned solution_svs;
      unsigned clock_drift; /*  Clock bias change rate, Hz */
      unsigned clock_bias; /* ns */

      /* mid-28 */
      struct {
         unsigned valid;
	 unsigned sat_id;
         double gps_soft_time;
	 double pseudorange;
	 double carrier_freq;
	 double carrier_phase;
	 unsigned sync_flags;
	 double min_cno;
         unsigned phase_err_cnt;
         unsigned low_power_cnt;
      } ch[SIRF_NUM_CHANNELS];

      /* Epoch */
      unsigned valid_channels;
      double epoch_time;
};

struct rinex_ctx_t {

   struct {
      char pgm[21];
      char run_by[21];
      char date[21];
   } file;

   char marker_name[21];
   char observer[21];
   char agency[21];

   struct {
      char no[21];
      char type[21];
      char version[21];
   } rec;

   struct {
      float x,y,z;
   } approx_pos;

   struct {
      char no[20];
      char type[20];
      float h, e, n;
   } antenna;

   struct gps_tm time_of_first_obs;

   int first_obs_found;
   int header_printed;

   struct epoch_t epoch;

   unsigned sirf_flags;
};

static int handle_nl_meas_data_msg(struct rinex_ctx_t *ctx,
      tSIRF_MSG_SSB_NL_MEAS_DATA *msg);
static int handle_meas_nav_msg(struct rinex_ctx_t *ctx,
      tSIRF_MSG_SSB_MEASURED_NAVIGATION *msg);
static int handle_clock_status_msg(struct rinex_ctx_t *ctx,
      tSIRF_MSG_SSB_CLOCK_STATUS *msg, FILE *out_f);
static int printf_obs_header(FILE *out_f, struct rinex_ctx_t *ctx);

static void epoch_clear (struct epoch_t *e);
static void epoch_close(struct epoch_t *e);
static int epoch_printf(FILE *out_f, struct epoch_t *e);

void *new_rinex_ctx(int argc, char **argv, unsigned gsw230_byte_order)
{
   struct rinex_ctx_t *ctx;
   struct tm *tm;
   time_t clock;

   if (argc || argv) {};

   ctx = malloc(sizeof(*ctx));

   if (ctx == NULL)
      return NULL;

   snprintf(ctx->file.pgm, sizeof(ctx->file.pgm), "sirfdump");
   ctx->file.run_by[0]='\0';
   time(&clock);
   tm = gmtime(&clock);
   snprintf(ctx->file.date, sizeof(ctx->file.date), "%i-%3s-%02u %02i:%02i",
	 tm->tm_mday, MonthName[tm->tm_mon], tm->tm_year % 100,
	 tm->tm_hour, tm->tm_min);

   ctx->marker_name[0]='\0';
   ctx->observer[0]='\0';
   ctx->agency[0]='\0';

   ctx->rec.no[0]='\0';
   ctx->rec.type[0]='\0';
   ctx->rec.version[0]='\0';

   ctx->approx_pos.x = ctx->approx_pos.y = ctx->approx_pos.z = 0;

   ctx->antenna.no[0]='\0';
   ctx->antenna.type[0]='\0';
   ctx->antenna.h = ctx->antenna.e = ctx->antenna.n = 0;

   ctx->time_of_first_obs.year = 0;
   ctx->time_of_first_obs.month = 0;
   ctx->time_of_first_obs.day = 0;
   ctx->time_of_first_obs.hour = 0;
   ctx->time_of_first_obs.min = 0;
   ctx->time_of_first_obs.sec = 0.0;

   ctx->first_obs_found = 0;
   ctx->header_printed = 0;

   ctx->sirf_flags = gsw230_byte_order ? SIRF_CODEC_FLAGS_GSW230_BYTE_ORDER : 0;

   epoch_clear(&ctx->epoch);

   return ctx;
}

void free_rinex_ctx(void *ctx)
{
   struct rinex_ctx_t *c;
   c = (struct rinex_ctx_t *)ctx;
   free(c);
}


int output_rinex(struct transport_msg_t *msg, FILE *out_f, void *user_ctx)
{
   int err;
   struct rinex_ctx_t *ctx;
   tSIRF_UINT32 msg_id, msg_length;
   union {
      tSIRF_MSG_SSB_NL_MEAS_DATA nld;
      uint8_t u8[SIRF_MSG_SSB_MAX_MESSAGE_LEN];
   } m;
   char str[1024];

   assert(user_ctx);

   if (!msg || msg->payload_length < 1)
      return 1;

   ctx = (struct rinex_ctx_t *)user_ctx;

   err = SIRF_CODEC_SSB_Decode_Ex(msg->payload,
	 msg->payload_length,
	 (tSIRF_UINT32)ctx->sirf_flags,
	 &msg_id,
	 m.u8,
	 &msg_length);

   if (err)
      return err;

   str[0]='\0';

   switch (msg_id) {
      case SIRF_MSG_SSB_NL_MEAS_DATA:
	 handle_nl_meas_data_msg(ctx, &m.nld);
	 break;
      case SIRF_MSG_SSB_MEASURED_NAVIGATION:
	 handle_meas_nav_msg(ctx, (tSIRF_MSG_SSB_MEASURED_NAVIGATION *)&m);
	 break;
      case SIRF_MSG_SSB_CLOCK_STATUS:
	 handle_clock_status_msg(ctx, (tSIRF_MSG_SSB_CLOCK_STATUS *)&m, out_f);
	 break;
      default:
	 break;
   }


   return err;
}

static int handle_nl_meas_data_msg(struct rinex_ctx_t *ctx,
      tSIRF_MSG_SSB_NL_MEAS_DATA *msg)
{
   unsigned i;
   assert(ctx);
   assert(msg);

   /*
   fprintf(stderr, "msg 28. ch: %d svid: %d gps_time: %f pr: %f freq: %f "
	 "phase: %f flags: 0x%x(%s) delta_rang_int: %d phase_err: %d\n", msg->Chnl, msg->svid,
	 msg->gps_sw_time, msg->pseudorange,  msg->carrier_freq,
	 msg->carrier_phase, msg->sync_flags,
	 msg->sync_flags & 0x02 ? "valid" : "invalid",
	 msg->delta_range_interval,
	 msg->phase_error_count);
   */

   if (msg->Chnl >= SIRF_NUM_CHANNELS) {
      fprintf(stderr, "skipped msg 28 record: wrong channel %u\n", msg->Chnl);
      return -1;
   }

   ctx->epoch.ch[msg->Chnl].valid = msg->sync_flags;
   ctx->epoch.ch[msg->Chnl].sat_id = msg->svid;
   ctx->epoch.ch[msg->Chnl].gps_soft_time = msg->gps_sw_time;
   ctx->epoch.ch[msg->Chnl].pseudorange = msg->pseudorange;
   ctx->epoch.ch[msg->Chnl].carrier_freq = msg->carrier_freq;
   ctx->epoch.ch[msg->Chnl].carrier_phase = msg->carrier_phase;
   ctx->epoch.ch[msg->Chnl].sync_flags = msg->sync_flags;
   ctx->epoch.ch[msg->Chnl].min_cno = msg->cton[0];
   for (i=1; i<SIRF_NUM_POINTS; i++) {
      if (ctx->epoch.ch[msg->Chnl].min_cno > msg->cton[i])
	 ctx->epoch.ch[msg->Chnl].min_cno = msg->cton[i];
   }
   ctx->epoch.ch[msg->Chnl].phase_err_cnt = msg->phase_error_count;
   ctx->epoch.ch[msg->Chnl].low_power_cnt = msg->low_power_count;

   return 1;
}

static int handle_meas_nav_msg(struct rinex_ctx_t *ctx,
      tSIRF_MSG_SSB_MEASURED_NAVIGATION *msg)
{
   assert(ctx);
   assert(msg);

   /*
   fprintf(stderr, "msg 2 (nav). week: %d tow: %lu sws: %d pmode: 0x%x\n",
	 msg->gps_week, msg->gps_tow, msg->sv_used_cnt, msg->nav_mode & 7);
	 */

   /* pmode  */
   if (((msg->nav_mode & 7) != 3) /* 3-SV solution (Kalman filter)*/
	 && ((msg->nav_mode & 7) != 4) /* >3-SV solution (Kalman filter) */
	 && ((msg->nav_mode & 7) != 5) /* 2-D point solution (least squares)  */
	 && ((msg->nav_mode & 7) != 6)) { /* 3-D point solution (least squares)  */
      /* no slution */
      return 1;
   }

   ctx->approx_pos.x = msg->ecef_x;
   ctx->approx_pos.y = msg->ecef_y;
   ctx->approx_pos.z = msg->ecef_z;

   ctx->epoch.gps_week = (ctx->epoch.gps_week & 0xfc00) | (msg->gps_week & 0x3ff);
   ctx->epoch.gps_tow = msg->gps_tow / 100.0;
   ctx->epoch.solution_svs = msg->sv_used_cnt;

   return 1;
}

static int handle_clock_status_msg(struct rinex_ctx_t *ctx,
      tSIRF_MSG_SSB_CLOCK_STATUS *msg, FILE *out_f)
{
   assert(ctx);
   assert(msg);

   /*
   fprintf(stderr, "msg 7 (clock). week: %u tow: %f svs: %d drift_hz: %ld bias: %ld\n", msg->gps_week,
	 msg->gps_tow/100.0, msg->sv_used_cnt, msg->clk_offset, msg->clk_bias);
	 */

   /* Extended GPS week  */
   ctx->epoch.gps_week = msg->gps_week;
   ctx->epoch.gps_tow = msg->gps_tow / 100.0;
   ctx->epoch.solution_svs = msg->sv_used_cnt;
   ctx->epoch.clock_bias = msg->clk_bias;
   ctx->epoch.clock_drift = msg->clk_offset;

   if (!ctx->first_obs_found
	 && (ctx->epoch.solution_svs >= 3)
	 ) {
      assert(!ctx->header_printed);
      gpstime2tm0(ctx->epoch.gps_week, ctx->epoch.gps_tow, &ctx->time_of_first_obs);
      ctx->first_obs_found=1;
      printf_obs_header(out_f, ctx);
      ctx->header_printed = 1;
   }

   if (ctx->header_printed) {
      epoch_close(&ctx->epoch);
      epoch_printf(out_f, &ctx->epoch);
      epoch_clear(&ctx->epoch);
   }

   return 1;
}

static int printf_obs_header(FILE *out_f, struct rinex_ctx_t *ctx)
{

   assert(ctx);
   assert(out_f);

   fprintf(out_f, "%9.2f%-11s%-20s%-20s%-20s\n", 2.11, "", "OBSERVATION DATA", "G",
	 "RINEX VERSION / TYPE");
   fprintf(out_f, "%-20s%-20s%-20s%-20s\n", ctx->file.pgm, ctx->file.run_by, ctx->file.date,
	 "PGM / RUN BY / DATE");
   fprintf(out_f, "%-60s%-20s\n", ctx->marker_name,
	 "MARKER NAME");
   fprintf(out_f, "%-20s%-40s%-20s\n", ctx->observer, ctx->agency,
	 "OBSERVER / AGENCY");
   fprintf(out_f, "%-20s%-20s%-20s%-20s\n", ctx->rec.no, ctx->rec.type, ctx->rec.version,
	 "REC # / TYPE / VERS");
   fprintf(out_f, "%-20s%-40s%-20s\n", ctx->antenna.no, ctx->antenna.type,
	 "ANT # / TYPE");
   fprintf(out_f, "%14.4f%14.4f%14.4f%18s%-20s\n", ctx->approx_pos.x, ctx->approx_pos.y, ctx->approx_pos.z, "",
	 "APPROX POSITION XYZ");
   fprintf(out_f, "%14.4f%14.4f%14.4f%18s%-20s\n", ctx->antenna.h, ctx->antenna.e, ctx->antenna.n, "",
	 "ANTENNA: DELTA H/E/N");
   fprintf(out_f, "%6i%6i%-48s%-20s\n", 1, 0, "",
	 "WAVELENGTH FACT L1/2");
   fprintf(out_f, "%6i    %-50s%-20s\n", 4, "L1    C1    D1    S1  ",
	 "# / TYPES OF OBSERV");
   fprintf(out_f, "%6d%6d%6d%6d%6d%13.7f%-5s%12s%-20s\n", ctx->time_of_first_obs.year,
	 ctx->time_of_first_obs.month, ctx->time_of_first_obs.day,
	 ctx->time_of_first_obs.hour, ctx->time_of_first_obs.min,
	 ctx->time_of_first_obs.sec, "", "",
	 "TIME OF FIRST OBS");
   fprintf(out_f, "%-60s%-20s\n", "",
	 "END OF HEADER");

   return 1;
}

int gpstime2tm0(unsigned gps_week, double gps_tow, struct gps_tm *res)
{
   time_t tt;
   struct tm *tm;
   double intpart, fractpart;

   assert(res);

   gps_tow = floor(gps_tow * 1e7 + 0.5)/1e7;
   fractpart = modf(gps_tow, &intpart);
   assert(fractpart<1.0);

   tt = GPS_EPOCH + gps_week * 60 * 60 * 24 * 7 + (int)intpart;

   tm = gmtime(&tt);

   res->sec = tm->tm_sec + fractpart;
   assert(res->sec<60.0);
   res->min = tm->tm_min;
   res->hour = tm->tm_hour;
   res->day = tm->tm_mday;
   res->yday = tm->tm_yday + 1;
   res->month = tm->tm_mon + 1;
   res->year = tm->tm_year + 1900;

   return 1;
}

static void epoch_clear (struct epoch_t *e)
{
   unsigned ch;
   assert(e);

   e->gps_week=0x400;
   e->gps_tow=0;
   e->solution_svs=0;

   for (ch=0; ch < SIRF_NUM_CHANNELS; ch++) {
      e->ch[ch].valid = 0;
   }

   e->valid_channels=0;
   e->epoch_time=0;
}

static int is_sat_in_epoch(const struct epoch_t *e, unsigned chan_id)
{
   return ( fabs(e->gps_tow + ((double)e->clock_bias / 1.0e9) - e->ch[chan_id].gps_soft_time) < 0.1);
}

static int snr_project_to_1x9(double snr)
{
   #define MIN(_a, _b) ( ((_a) < (_b)) ? (_a) : (_b)  )
   #define MAX(_a, _b) ( ((_a) > (_b)) ? (_a) : (_b)  )
   return MIN(MAX((int)(snr/6),1),9);
}

static void epoch_close(struct epoch_t *e)
{
   unsigned chan_id;

   assert(e);

   e->valid_channels = 0;
   e->epoch_time = 0;

   for (chan_id=0; chan_id < SIRF_NUM_CHANNELS; chan_id++) {
      if (!e->ch[chan_id].valid)
	 continue;

      if (!is_sat_in_epoch(e, chan_id)) {
	 fprintf(stderr, "Found mid28 from different epoch. Current: %.5lf mid28: %.5lf\n",
	       (double)(e->gps_tow + (e->clock_bias / 1.0e9)),
	       (double)e->ch[chan_id].gps_soft_time);
	 e->ch[chan_id].valid = 0;
	 continue;
      }

      ++e->valid_channels;
      /* Use first valid channel time as epoch time   */
      if (e->epoch_time == 0)
	 e->epoch_time = (double)e->ch[chan_id].gps_soft_time - (double)e->clock_bias / 1.0e9;
   }

}

static int epoch_printf(FILE *out_f, struct epoch_t *e)
{
   const char itoa[] = {'0','1','2','3','4','5','6','7','8','9'};
   struct gps_tm gps_tm0;
   char tmp[82];
   unsigned satlist_p;
   int written;
   unsigned chan_id;
   unsigned epoch_flag;

   if (e->valid_channels == 0)
      return -1;

   gpstime2tm0(e->gps_week, e->epoch_time, &gps_tm0);

   epoch_flag=0;

   /* Header */
   written = snprintf(tmp, sizeof(tmp), " %2u %2u %2u %2u %2u%11.7f  %1u%3u",
	 gps_tm0.year % 100,
	 gps_tm0.month,
	 gps_tm0.day,
	 gps_tm0.hour,
	 gps_tm0.min,
	 (double)gps_tm0.sec,
	 epoch_flag,
	 e->valid_channels % 1000
	 );

   assert(written == 5*3+11+3+3);

   /* XXX */
   assert(e->valid_channels <= 12);

   satlist_p = 32;
   for (chan_id=0; chan_id < SIRF_NUM_CHANNELS; chan_id++) {
      if (!e->ch[chan_id].valid)
	 continue;

      /* sat_id 'Gxx' */
      if (e->ch[chan_id].sat_id >= 100)
	 tmp[satlist_p++]='S';
      else
	 tmp[satlist_p++]='G';

      if (e->ch[chan_id].sat_id >= 10)
	 tmp[satlist_p++] = itoa[ (e->ch[chan_id].sat_id / 10) % 10  ];
      else
	 tmp[satlist_p++] = ' ';
      tmp[satlist_p++] = itoa[ e->ch[chan_id].sat_id % 10  ];
   } /* for  */

   tmp[satlist_p++] = '\n';
   tmp[satlist_p] = '\0';

   assert(satlist_p < sizeof(tmp));
   if ( fwrite(tmp, 1, satlist_p, out_f) < satlist_p)
      return -1;

   /* observations */
   for (chan_id=0; chan_id < SIRF_NUM_CHANNELS; chan_id++) {
      double c1;
      double l1;
      double d1;
      double s1;
      unsigned char l1_loss_of_lock, loss_of_lock;
      unsigned char sig_strength;

      if (!e->ch[chan_id].valid)
	 continue;

      /* Pseudorange C/A on L1, meters */
      c1 = e->ch[chan_id].pseudorange - (SPEED_OF_LIGHT * (e->clock_bias / 1.0e9));

      /* Phase on L1, cycles */
      if (e->ch[chan_id].carrier_phase)
	 l1 = L1_CARRIER_FREQ * (e->ch[chan_id].carrier_phase / SPEED_OF_LIGHT - (e->clock_bias / 1.0e9));
      else
	 l1 = 0;

      /* Doppler freq on L1, Hz */
      d1 = -1 * (e->ch[chan_id].carrier_freq * L1_CARRIER_FREQ / SPEED_OF_LIGHT - e->clock_drift);

      /* Snr */
      s1 = e->ch[chan_id].min_cno;

      /* 0x01 - acq/re-acq completed  */
      /* 0x02 - integrated carrier phase is valid */
      /* 0x04 - subframe sync completed  */
      /* 0x08 - carrier pullin completed  */
      /* 0x10 - code locked  */
      /* 0x40 - ephemeris data available  */
      if (!(e->ch[chan_id].sync_flags & 0x02))
	 l1_loss_of_lock = '1';
      else
	 l1_loss_of_lock = ' ';

      if ( (e->ch[chan_id].phase_err_cnt > 0) && (l1_loss_of_lock != '1')) {
	    fprintf(stderr, "%.5f sync_flags %x phase_err_cnt: %u\n",
		  e->gps_tow,
		  e->ch[chan_id].sync_flags,
		  e->ch[chan_id].phase_err_cnt);
      }

      sig_strength = itoa [ snr_project_to_1x9(s1) ];

      if ((l1 > 9999999999.999) || (l1 < -999999999.999))
	 l1 = 0;
      else if (e->ch[chan_id].phase_err_cnt == 50)
	 l1 = 0;

      if ((d1 > 9999999999.999) || (d1 < -999999999.999))
	 d1 = 0;

      if ((c1 > 9999999999.999) || (c1 < -999999999.999)) {
	 c1 = 0;
	 sig_strength=' ';
      }

      if (l1 == 0)
	 l1_loss_of_lock = ' ';

      loss_of_lock = ' ';

      written = fprintf(out_f, "%14.3f%c%c%14.3f%c%c%14.3f%c%c%14.3f\n",
	    l1, l1_loss_of_lock, ' ',
	    c1, loss_of_lock, sig_strength,
	    d1, loss_of_lock, ' ',
	    s1);
      if (written < 0)
	 return -1;

   } /* for */

   return 0;
}


