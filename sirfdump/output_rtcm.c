
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "gpsd/gps.h"
#include "gpsd/crc24q.h"
#include "sirfdump.h"
#include "sirf_codec.h"
#include "sirf_codec_ssb.h"
#include "sirf_msg.h"
#include "nav.h"

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
	 unsigned time_in_track;
         unsigned phase_err_cnt;
         unsigned low_power_cnt;
      } ch[SIRF_NUM_CHANNELS];

      /* Epoch */
      unsigned valid_channels;
      double epoch_time;
};

struct rtcm_ctx_t {
   struct epoch_t epoch;
   struct nav_data_t navdata;
   unsigned sirf_flags;
};

static int handle_nl_meas_data_msg(struct rtcm_ctx_t *ctx,
      tSIRF_MSG_SSB_NL_MEAS_DATA *msg);
static int handle_meas_nav_msg(struct rtcm_ctx_t *ctx,
      tSIRF_MSG_SSB_MEASURED_NAVIGATION *msg);
static int handle_clock_status_msg(struct rtcm_ctx_t *ctx,
      tSIRF_MSG_SSB_CLOCK_STATUS *msg, FILE *out_f);
static int handle_mid8_msg(struct rtcm_ctx_t *ctx,
      const tSIRF_MSG_SSB_50BPS_DATA *msg, FILE *out_f);

static void epoch_clear (struct epoch_t *e);
static void epoch_close(struct epoch_t *e);
static int epoch_printf(FILE *out_f, struct epoch_t *e);
static int rtcm_transport_write(FILE *out_f, void *data, unsigned size);

static unsigned set_ubits(uint8_t *buf, unsigned pos, int len, unsigned val);
static unsigned set_sbits(uint8_t *buf, unsigned pos, unsigned len, int val);

void *new_rtcm_ctx(int argc, char **argv, unsigned gsw230_byte_order)
{
   struct rtcm_ctx_t *ctx;

   if (argc || argv) {};

   ctx = malloc(sizeof(*ctx));

   if (ctx == NULL)
      return NULL;

   epoch_clear(&ctx->epoch);
   init_nav_data(&ctx->navdata);
   ctx->sirf_flags = gsw230_byte_order ? SIRF_CODEC_FLAGS_GSW230_BYTE_ORDER : 0;

   return ctx;
}

void free_rtcm_ctx(void *ctx)
{
   free(ctx);
}

int output_rtcm(struct transport_msg_t *msg, FILE *out_f, void *user_ctx)
{
   int err;
   struct rtcm_ctx_t *ctx;
   tSIRF_UINT32 msg_id, msg_length;
   union {
      tSIRF_MSG_SSB_50BPS_DATA data_50bps;
      tSIRF_MSG_SSB_MEASURED_NAVIGATION  meas_nav;
      tSIRF_MSG_SSB_CLOCK_STATUS clock;
      tSIRF_MSG_SSB_NL_MEAS_DATA nld;
      uint8_t u8[SIRF_MSG_SSB_MAX_MESSAGE_LEN];
   } m;
   char str[1024];

   assert(user_ctx);

   if (!msg || msg->payload_length < 1)
      return 1;

   ctx = (struct rtcm_ctx_t *)user_ctx;

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
	 handle_meas_nav_msg(ctx, &m.meas_nav);
	 break;
      case SIRF_MSG_SSB_50BPS_DATA:
	 handle_mid8_msg(ctx, &m.data_50bps, out_f);
	 break;
      case SIRF_MSG_SSB_CLOCK_STATUS:
	 handle_clock_status_msg(ctx, &m.clock, out_f);
	 break;
      default:
	 break;
   }

   return err;
}

static int handle_nl_meas_data_msg(struct rtcm_ctx_t *ctx,
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
   ctx->epoch.ch[msg->Chnl].time_in_track = msg->time_in_track;
   ctx->epoch.ch[msg->Chnl].min_cno = msg->cton[0];
   for (i=1; i<SIRF_NUM_POINTS; i++) {
      if (ctx->epoch.ch[msg->Chnl].min_cno > msg->cton[i])
	 ctx->epoch.ch[msg->Chnl].min_cno = msg->cton[i];
   }
   ctx->epoch.ch[msg->Chnl].phase_err_cnt = msg->phase_error_count;
   ctx->epoch.ch[msg->Chnl].low_power_cnt = msg->low_power_count;

   return 1;
}

static int handle_meas_nav_msg(struct rtcm_ctx_t *ctx,
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
      /* XXX  */
   }

   ctx->epoch.gps_week = (ctx->epoch.gps_week & 0xfc00) | (msg->gps_week & 0x3ff);
   ctx->epoch.gps_tow = msg->gps_tow / 100.0;
   ctx->epoch.solution_svs = msg->sv_used_cnt;

   return 1;
}

static int handle_clock_status_msg(struct rtcm_ctx_t *ctx,
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

   epoch_close(&ctx->epoch);
   epoch_printf(out_f, &ctx->epoch);
   epoch_clear(&ctx->epoch);

   return 1;
}

static int handle_mid8_msg(struct rtcm_ctx_t *ctx,
      const tSIRF_MSG_SSB_50BPS_DATA *msg,
      FILE *out_f)
{
   unsigned pos;
   int data_changed;
   struct nav_sat_data_t *sat;
   uint8_t msg1019[62];

   assert(ctx);
   assert(msg);
   assert(out_f);

   data_changed = populate_navdata_from_mid8(msg, &ctx->navdata);

   if (data_changed < 0)
      return data_changed;

   sat = get_navdata_p(&ctx->navdata, msg->svid);
   if (!sat)
      return -1;

   if (!sat->is_sub1_active
	 || !sat->is_sub2_active
	 || !sat->is_sub3_active)
      return 0;
   assert((sat->sub1.sub1.IODC & 0xff) == sat->sub2.sub2.IODE);
   assert((sat->sub1.sub1.IODC & 0xff) == sat->sub3.sub3.IODE);

   /* MSG1019 */
   pos = 0;
   pos += set_ubits(msg1019, pos, 12, 1019);
   pos += set_ubits(msg1019, pos, 6, msg->svid);
   pos += set_ubits(msg1019, pos, 10, sat->sub1.sub1.WN);
   pos += set_ubits(msg1019, pos, 4, sat->sub1.sub1.ura);
   pos += set_ubits(msg1019, pos, 2, sat->sub1.sub1.l2);
   pos += set_sbits(msg1019, pos, 14, sat->sub3.sub3.IDOT);
   pos += set_ubits(msg1019, pos, 8, sat->sub3.sub3.IODE);
   pos += set_ubits(msg1019, pos, 16, sat->sub1.sub1.toc);
   pos += set_sbits(msg1019, pos, 8, sat->sub1.sub1.af2);
   pos += set_sbits(msg1019, pos, 16, sat->sub1.sub1.af1);
   pos += set_sbits(msg1019, pos, 22, sat->sub1.sub1.af0);
   pos += set_ubits(msg1019, pos, 10, sat->sub1.sub1.IODC);
   pos += set_sbits(msg1019, pos, 16, sat->sub2.sub2.Crs);
   pos += set_sbits(msg1019, pos, 16, sat->sub2.sub2.deltan);
   pos += set_sbits(msg1019, pos, 32, sat->sub2.sub2.M0);
   pos += set_sbits(msg1019, pos, 16, sat->sub2.sub2.Cuc);
   pos += set_ubits(msg1019, pos, 32, sat->sub2.sub2.e);
   pos += set_sbits(msg1019, pos, 16, sat->sub2.sub2.Cus);
   pos += set_ubits(msg1019, pos, 32, sat->sub2.sub2.sqrtA);
   pos += set_ubits(msg1019, pos, 16, sat->sub1.sub1.toc);
   pos += set_sbits(msg1019, pos, 16, sat->sub3.sub3.Cic);
   pos += set_sbits(msg1019, pos, 32, sat->sub3.sub3.Omega0);
   pos += set_sbits(msg1019, pos, 16, sat->sub3.sub3.Cis);
   pos += set_sbits(msg1019, pos, 32, sat->sub3.sub3.i0);
   pos += set_sbits(msg1019, pos, 16, sat->sub3.sub3.Crc);
   pos += set_sbits(msg1019, pos, 32, sat->sub3.sub3.omega);
   pos += set_sbits(msg1019, pos, 24, sat->sub3.sub3.Omegad);
   pos += set_sbits(msg1019, pos, 8, sat->sub1.sub1.Tgd);
   pos += set_ubits(msg1019, pos, 6, sat->sub1.sub1.hlth);
   pos += set_ubits(msg1019, pos, 1, sat->sub1.sub1.l2p);
   pos += set_ubits(msg1019, pos, 1, sat->sub2.sub2.fit);
   assert(pos==488);
   msg1019[61] = 0;

   return rtcm_transport_write(out_f, msg1019, 62);
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

/* XXX  */
static unsigned set_ubits(uint8_t *buf, unsigned pos, int len, unsigned val)
{
   int i;

   pos = pos + len - 1;

   for (i=len-1; i>=0; --i) {
      if (val & 1)
	 buf[ pos/8 ] |= 1<<(7-(pos%8));
      else
	 buf[ pos/8 ] &= ~(1<<(7-(pos%8)));

      val >>= 1;
      pos -= 1;
   }

   return len;
}

/* XXX: 2's complement integer*/
static unsigned set_sbits(uint8_t *buf, unsigned pos, unsigned len, int val)
{
   unsigned v0;

   if (val >= 0)
      v0 = val;
   else
      v0 = ~((unsigned)-val)+1;

   return set_ubits(buf, pos, len, v0);
}

static int epoch_printf(FILE *out_f, struct epoch_t *e)
{
   unsigned pos;
   unsigned chan_id;
   uint8_t msg1002[8+(int)(9.25*SIRF_NUM_CHANNELS)+1];

   if (e->valid_channels == 0)
      return -1;

   pos = 0;

   /* MSG1002 Header  */

   /* DF002 uint12 message number */
   pos += set_ubits(msg1002, pos, 12, 1002);
   /* DF003 uint12 reference station id  */
   pos += set_ubits(msg1002, pos, 12, 0);
   /* DF004 uint30 TOW   */
   pos += set_ubits(msg1002, pos, 30, (unsigned)(e->epoch_time*1000.0)  & 0x3fffffff);
   /* DF005 bit(1) synchronous GNSS flag  */
   pos += set_ubits(msg1002, pos, 1, 0);
   /* DF006 uint5  no of GPS satellite signals processed */
   pos += set_ubits(msg1002, pos, 5, e->valid_channels);
   /* DF007 bit(1) divergence-free smoothind indicator   */
   pos += set_ubits(msg1002, pos, 1, 0);
   /* DF008 bit(3) GPS smooting interval   */
   pos += set_ubits(msg1002, pos, 3, 0);

   assert(pos == 64);

   /* Data  */
   for (chan_id=0; chan_id < SIRF_NUM_CHANNELS; ++chan_id) {
      unsigned pr, pr_high, phase;
      unsigned lock_time;

      if (!e->ch[chan_id].valid)
	 continue;

      if (e->ch[chan_id].pseudorange != 0) {
	 double d_pr;

	 d_pr = e->ch[chan_id].pseudorange - (SPEED_OF_LIGHT * (e->clock_bias / 1.0e9));

	 pr = (unsigned)(fmod(d_pr, SPEED_OF_LIGHT/1000.0) / 0.02);
	 pr_high = (unsigned)(d_pr / (SPEED_OF_LIGHT/1000.0));

	 if ((e->ch[chan_id].carrier_phase != 0)
	       && (e->ch[chan_id].phase_err_cnt < 50)
	       && (e->ch[chan_id].sync_flags & 0x02)
	    ) {
	    double d_phase;
	    d_phase = e->ch[chan_id].carrier_phase - e->ch[chan_id].pseudorange;
	    phase = (int)(d_phase / 0.0005);
	 }else
	    phase = -0x080000;

      } else {
	 pr = 0x080000;
	 pr_high = 0;
	 phase = -0x080000;
      }

      /* XXX: lock_time  */
      if (e->ch[chan_id].time_in_track >= 937000)
	 lock_time = 127;
      else if (e->ch[chan_id].time_in_track >= 744000)
	 lock_time = (e->ch[chan_id].time_in_track + 3096000) / 32000;
      else if (e->ch[chan_id].time_in_track >= 360000)
	 lock_time = (e->ch[chan_id].time_in_track + 1176000) / 16000;
      else if (e->ch[chan_id].time_in_track >= 168000)
	 lock_time = (e->ch[chan_id].time_in_track + 408000) / 8000;
      else if (e->ch[chan_id].time_in_track >= 72000)
	 lock_time = (e->ch[chan_id].time_in_track + 120000) / 4000;
      else if (e->ch[chan_id].time_in_track >= 24000)
	 lock_time = (e->ch[chan_id].time_in_track + 24000) / 2000;
      else
	 lock_time = e->ch[chan_id].time_in_track / 1000;

      /* DF009 uint6 sattelite id  */
      pos += set_ubits(msg1002, pos, 6, e->ch[chan_id].sat_id);
      /* DF010 bit(1) L1 code indicator  */
      pos += set_ubits(msg1002, pos, 1, 0);
      /* DF011 uint24 pseudorange  */
      pos += set_ubits(msg1002, pos, 24, pr);
      /* DF012 int20 PhaseRange-Pseudorange  */
      pos += set_sbits(msg1002, pos, 20, phase);
      /* DF013 uint7 L1 lock time indicator  */
      pos += set_ubits(msg1002, pos, 7, lock_time);
      /* DF014 uint8 L1 pseudorange modulus ambiguity  */
      pos += set_ubits(msg1002, pos, 8, pr_high);
      /* DF015 uint8 L1 CNR  */
      pos += set_ubits(msg1002, pos, 8, (unsigned)(e->ch[chan_id].min_cno / 0.25));
   }

   /* padding */
   if (pos % 24 != 0)
      pos += set_ubits(msg1002, pos, 24 - pos % 24, 0);
   assert(pos % 24 == 0);

   return rtcm_transport_write(out_f, msg1002, pos/8);
}


static int rtcm_transport_write(FILE *out_f, void *data, unsigned size)
{
   unsigned crc24;
   uint8_t  header[3];
   uint8_t  footer[3];
   struct crc24_iovec crc_iovec[] = {
      {header, sizeof(header)},
      {data, (size_t)size},
   };

   assert(size <= 1023);

   header[0] = 0xd3;
   header[1] = (size >> 8) & 0x03;
   header[2] = size & 0xff;

   crc24 = crc24q_hashv(crc_iovec, sizeof(crc_iovec)/sizeof(crc_iovec[0]));
   footer[0] = (crc24 >> 16) & 0xff;
   footer[1] = (crc24 >> 8) & 0xff;
   footer[2] = crc24 & 0xff;

   if (fwrite(header, sizeof(header), 1, out_f) < 1)
      return -1;
   if (fwrite(data, size, 1, out_f) < 1)
      return -1;
   if (fwrite(footer, sizeof(footer), 1, out_f) < 1)
      return -1;

   return 0;
}

