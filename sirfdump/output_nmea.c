
#include <stdio.h>
#include "sirfdump.h"
#include "sirf_msg.h"
#include "sirf_codec_ssb.h"
#include "sirf_codec_nmea.h"

int output_nmea(struct transport_msg_t *msg, FILE *out_f, void *ctx)
{
   int err;
   tSIRF_UINT32 msg_id, msg_length;
   union {
      tSIRF_MSG_SSB_MEASURED_TRACKER mt;
      tSIRF_MSG_SSB_GEODETIC_NAVIGATION gn;
      uint8_t u8[SIRF_MSG_SSB_MAX_MESSAGE_LEN];
   } msg_structure;
   char str[1024];

   if (!msg || msg->payload_length < 1)
      return 1;

   err = SIRF_CODEC_SSB_Decode(msg->payload,
	 msg->payload_length,
	 &msg_id,
	 msg_structure.u8,
	 &msg_length);

   if (err)
      return err;

   str[0]='\0';

   switch (msg_id) {
      case SIRF_MSG_SSB_MEASURED_TRACKER:
	 err = SIRF_CODEC_NMEA_Encode_GSV(&msg_structure.mt, str);
	 if (err == 0) fputs(str, out_f);
	 break;
      case SIRF_MSG_SSB_GEODETIC_NAVIGATION:
	 err = SIRF_CODEC_NMEA_Encode_GGA(&msg_structure.gn, str);
	 if (err == 0) fputs(str, out_f);
	 err = SIRF_CODEC_NMEA_Encode_RMC(&msg_structure.gn, str);
	 if (err == 0) fputs(str, out_f);
	 err = SIRF_CODEC_NMEA_Encode_GSA(&msg_structure.gn, str);
	 if (err == 0) fputs(str, out_f);
	 err = SIRF_CODEC_NMEA_Encode_VTG(&msg_structure.gn, str);
	 if (err == 0) fputs(str, out_f);
/* 	 err = SIRF_CODEC_NMEA_Encode_GLL(&msg_structure.gn, str);
	 if (err == 0) fputs(str, out_f); */
      default:
	 break;
   }

   return err;
}


