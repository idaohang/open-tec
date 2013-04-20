
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "gpsd/gps.h"
#include "sirfdump.h"
#include "nav.h"
#include "sirf_msg.h"
#include "sirf_codec_ssb.h"

void init_nav_data(struct nav_data_t *data)
{
   unsigned i;

   assert(data);

   data->sub4_18.is_active=0;

   for(i=0; i < sizeof(data->prn)/sizeof(data->prn[0]); ++i) {
      data->prn[i].is_sub1_active =
	 data->prn[i].is_sub2_active =
	 data->prn[i].is_sub3_active =
	 data->prn[i].is_printed = 0;
   }
}

struct nav_sat_data_t *get_navdata_p(struct nav_data_t *data, unsigned prn)
{
   assert(data);

   if (prn > MAX_GPS_PRN)
      return NULL;

   return &data->prn[prn-1];
}

int populate_navdata_from_mid8(
      const tSIRF_MSG_SSB_50BPS_DATA *msg,
      struct nav_data_t *data
      )
{
   int data_changed;
   unsigned i;
   struct subframe_t subp;
   struct nav_sat_data_t *dst;
   uint32_t words[10];

   assert(msg);
   assert(data);

   for(i=0; i<10; i++)
      words[i]=msg->word[i];

   if (gpsd_interpret_subframe_raw(&subp, msg->svid, words) <= 0)
      return -1;

   data_changed = 0;
   dst = get_navdata_p(data, subp.tSVID);
   /* XXX  */
   if (dst == NULL)
      return -2;

   switch (subp.subframe_num) {
      case 1:
	 if (dst->is_sub1_active
	       && (dst->sub1.sub1.IODC == dst->sub1.sub1.IODC))
	    /*  skip subframe */
	    break;

	 memcpy(&dst->sub1, &subp, sizeof(subp));
	 dst->is_sub1_active = 1;
	 data_changed = 1;
	 if (dst->is_sub2_active
	       && ((dst->sub1.sub1.IODC & 0xff) != dst->sub2.sub2.IODE))
	    dst->is_sub2_active = 0;
	 if (dst->is_sub3_active
	       && ((dst->sub1.sub1.IODC & 0xff) != dst->sub3.sub3.IODE))
	    dst->is_sub3_active = 0;
	 break;
      case 2:
	 if (dst->is_sub2_active
	       && (dst->sub2.sub2.IODE == subp.sub2.IODE))
	    /*  skip subframe */
	    break;

	 memcpy(&dst->sub2, &subp, sizeof(subp));
	 data_changed = 1;
	 dst->is_sub2_active = 1;
	 if (dst->is_sub1_active
	       && ((dst->sub1.sub1.IODC & 0xff) != dst->sub2.sub2.IODE))
	    dst->is_sub1_active = 0;
	 if (dst->is_sub3_active
	       && (dst->sub3.sub3.IODE != dst->sub2.sub2.IODE))
	    dst->is_sub3_active = 0;
	 break;
      case 3:
	 if (dst->is_sub3_active
	       && (dst->sub3.sub3.IODE == subp.sub3.IODE))
	    /*  skip subframe */
	    break;

	 memcpy(&dst->sub3, &subp, sizeof(subp));
	 data_changed = 1;
	 dst->is_sub3_active = 1;
	 if (dst->is_sub1_active
	       && ((dst->sub1.sub1.IODC & 0xff) != dst->sub3.sub3.IODE))
	    dst->is_sub1_active = 0;
	 if (dst->is_sub2_active
	       && (dst->sub2.sub2.IODE != dst->sub3.sub3.IODE))
	    dst->is_sub2_active = 0;
	 break;
      case 4:
	 if (subp.pageid == 56) {
	    if ( data->sub4_18.is_active
		  && ((data->sub4_18.ion_alpha[0] != subp.sub4_18.d_alpha0)
		     || (data->sub4_18.ion_alpha[1] != subp.sub4_18.d_alpha1)
		     || (data->sub4_18.ion_alpha[2] != subp.sub4_18.d_alpha2)
		     || (data->sub4_18.ion_alpha[3] != subp.sub4_18.d_alpha3)
		     || (data->sub4_18.ion_beta[0] != subp.sub4_18.d_beta0)
		     || (data->sub4_18.ion_beta[1] != subp.sub4_18.d_beta1)
		     || (data->sub4_18.ion_beta[2] != subp.sub4_18.d_beta2)
		     || (data->sub4_18.ion_beta[3] != subp.sub4_18.d_beta3)
		     ))
	       data->sub4_18.is_active = 0;

	    if (data->sub4_18.is_active == 0) {
	       data->sub4_18.ion_alpha[0] = subp.sub4_18.d_alpha0;
	       data->sub4_18.ion_alpha[1] = subp.sub4_18.d_alpha1;
	       data->sub4_18.ion_alpha[2] = subp.sub4_18.d_alpha2;
	       data->sub4_18.ion_alpha[3] = subp.sub4_18.d_alpha3;
	       data->sub4_18.ion_beta[0] = subp.sub4_18.d_beta0;
	       data->sub4_18.ion_beta[1] = subp.sub4_18.d_beta1;
	       data->sub4_18.ion_beta[2] = subp.sub4_18.d_beta2;
	       data->sub4_18.ion_beta[3] = subp.sub4_18.d_beta3;
	       data->sub4_18.a0 = subp.sub4_18.d_A0;
	       data->sub4_18.a1 = subp.sub4_18.d_A1;
	       data->sub4_18.tot = subp.sub4_18.d_tot;
	       data->sub4_18.WNt = subp.sub4_18.WNt;
	       data->sub4_18.leap = subp.sub4_18.leap;
	       data->sub4_18.is_active = 1;
	       data_changed |= 0x02;
	    }
	 }
	 break;
      default:
	 break;
   }

   return data_changed;
}


