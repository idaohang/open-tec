
#ifndef SIRFDUMP_H
#define SIRFDUMP_H

#include <stdint.h>
#include <stdio.h>

#define L1_CARRIER_FREQ 1575420000.0
#define SPEED_OF_LIGHT 299792458.0
#define GPS_EPOCH  315964800 /*  GPS epoch in Unix time */
#define MAX_GPS_PRN  32

struct transport_msg_t {
   uint8_t *payload;
   unsigned payload_length;
   unsigned checksum;
   unsigned skipped_bytes;
};

struct gps_tm {
   unsigned year, month, day, hour, min;
   unsigned yday;
   double sec;
};

typedef int (dumpf_t)(struct transport_msg_t *msg, FILE *out_f, void *user_ctx);

int output_dump(struct transport_msg_t *msg, FILE *out_f, void *user_ctx);
int output_nmea(struct transport_msg_t *msg, FILE *out_f, void *user_ctx);

void *new_rinex_ctx(int argc, char **argv, unsigned gsw230_byte_order);
void free_rinex_ctx(void *ctx);
int output_rinex(struct transport_msg_t *msg, FILE *out_f, void *user_ctx);

void *new_rinex_nav_ctx(int argc, char **argv);
void free_rinex_nav_ctx(void *ctx);
int output_rinex_nav(struct transport_msg_t *msg, FILE *out_f, void *user_ctx);

void *new_rtcm_ctx(int argc, char **argv, unsigned gsw230_byte_order);
void free_rtcm_ctx(void *ctx);
int output_rtcm(struct transport_msg_t *msg, FILE *out_f, void *user_ctx);


int gpstime2tm0(unsigned gps_week, double gps_tow, struct gps_tm *res);

#endif /* SIRFDUMP_H */


