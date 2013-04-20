/* gps.h -- interface of the libgps library */
/*
 * This file is Copyright (c) 2010 by the GPSD project
 * BSD terms apply: see the file COPYING in the distribution root for details.
 */
#ifndef _GPSD_GPS_H_
#define _GPSD_GPS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Macro for declaring function arguments unused. */
#if defined(__GNUC__)
#  define UNUSED __attribute__((unused)) /* Flag variable as unused */
#else /* not __GNUC__ */
#  define UNUSED
#endif

#ifdef _MSC_VER
#include <stdint.h>
#define bool unsigned char
#else
#include <inttypes.h>	/* stdint.h would be smaller but not all have it */
#include <stdbool.h>
#endif

#include <stdio.h>

#define GPS_PI          3.1415926535897932384626433832795029

/* The almanac is a subset of the clock and ephemeris data, with reduced
 * precision. See IS-GPS-200E, Table 20-VI  */
struct almanac_t
{
    uint8_t sv;  /* The satellite this refers to */
    /* toa, almanac reference time, 8 bits unsigned, seconds */
    uint8_t toa;
    long l_toa;
    /* SV health data, 8 bit unsigned bit map */
    uint8_t svh;
    /* deltai, correction to inclination, 16 bits signed, semi-circles */
    int16_t deltai;
    double d_deltai;
    /* M0, Mean Anomaly at Reference Time, 24 bits signed, semi-circles */
    int32_t M0;
    double d_M0;
    /* Omega0, Longitude of Ascending Node of Orbit Plane at Weekly Epoch,
     * 24 bits signed, semi-circles */
    int32_t Omega0;
    double d_Omega0;
    /* omega, Argument of Perigee, 24 bits signed, semi-circles */
    int32_t omega;
    double d_omega;
    /* af0, SV clock correction constant term
     * 11 bits signed, seconds */
    int16_t af0;
    double d_af0;
    /* af1, SV clock correction first order term
     * 11 bits signed, seconds/second */
    int16_t af1;
    double d_af1;
    /* eccentricity, 16 bits, unsigned, dimensionless */
    uint16_t e;
    double d_eccentricity;
    /* sqrt A, Square Root of the Semi-Major Axis
     * 24 bits unsigned, square_root(meters) */
    uint32_t sqrtA;
    double d_sqrtA;
    /* Omega dot, Rate of Right Ascension, 16 bits signed, semi-circles/sec */
    int16_t Omegad;
    double d_Omegad;
};

struct subframe_t {
    /* subframe number, 3 bits, unsigned, 1 to 5 */
    uint8_t subframe_num;
    /* data_id, denotes the NAV data structure of D(t), 2 bits, in
     * IS-GPS-200E always == 0x1 */
    uint8_t data_id;
    /* SV/page id used for subframes 4 & 5, 6 bits */
    uint8_t pageid;
    /* tSVID, SV ID of the sat that transmitted this frame, 6 bits unsigned */
    uint8_t tSVID;
    /* TOW, Time of Week of NEXT message, 17 bits unsigned, scale 6, seconds */
    uint32_t TOW17;
    long l_TOW17;
    /* integrity, URA bounds flag, 1 bit */
    bool integrity;
    /* alert, alert flag, SV URA and/or the SV User Differential Range
     * Accuracy (UDRA) may be worse than indicated, 1 bit */
    bool alert;
    /* antispoof, A-S mode is ON in that SV, 1 bit */
    bool antispoof;
    int is_almanac;
    union {
        /* subframe 1, part of ephemeris, see IS-GPS-200E, Table 20-II
	 * and Table 20-I */
	struct {
	    /* WN, Week Number, 10 bits unsigned, scale 1, weeks */
	    uint16_t WN;
	    /* IODC, Issue of Data, Clock, 10 bits, unsigned,
	     * issued in 8 data ranges at the same time */
	    uint16_t IODC;
	    /* toc, clock data reference time, 16 bits, unsigned, seconds
	     * scale 2**4, issued in 8 data ranges at the same time */
	    uint16_t toc;
	    long l_toc;
	    /* l2, code on L2, 2 bits, bit map */
	    uint8_t l2;
	    /* l2p, L2 P data flag, 1 bit */
	    uint8_t l2p;
	    /* ura, SV accuracy, 4 bits unsigned index */
	    unsigned int ura;
	    /* hlth, SV health, 6 bits unsigned bitmap */
	    unsigned int hlth;
	    /* af0, SV clock correction constant term
	     * 22 bits signed, scale 2**-31, seconds */
	    int32_t af0;
	    double d_af0;
	    /* af1, SV clock correction first order term
	     * 22 bits signed, scale 2**-43, seconds/second */
	    int16_t af1;
	    double d_af1;
	    /* af2, SV clock correction second order term
	     * 8 bits signed, scale 2**-55, seconds/second**2 */
	    int8_t af2;
	    double d_af2;
	    /* Tgd,  L1-L2 correction term, 8 bits signed,  scale 2**-31,
	     * seconds */
	    int8_t Tgd;
	    double d_Tgd;
	} sub1;
        /* subframe 2, part of ephemeris, see IS-GPS-200E, Table 20-II
	 * and Table 20-III */
	struct {
	    /* Issue of Data (Ephemeris),
	     * equal to the 8 LSBs of the 10 bit IODC of the same data set */
	    uint8_t IODE;
	    /* Age of Data Offset for the NMCT, 6 bits, scale 900,
	     * ignore if all ones, seconds */
	    uint8_t AODO;
	    uint16_t u_AODO;
	    /* fit, FIT interval flag, indicates a fit interval greater than
	     * 4 hour, 1 bit */
	    uint8_t fit;
	    /* toe, Reference Time Ephemeris, 16 bits unsigned, scale 2**4,
	     * seconds */
	    uint16_t toe;
	    long l_toe;
	    /* Crs, Amplitude of the Sine Harmonic Correction Term to the
	     * Orbit Radius, 16 bits, scale 2**-5, signed, meters */
	    int16_t Crs;
	    double d_Crs;
	    /* Cus, Amplitude of the Sine Harmonic Correction Term to the
	     * Argument of Latitude, 16 bits, signed, scale 2**-29, radians */
	    int16_t Cus;
	    double d_Cus;
	    /* Cuc, Amplitude of the Cosine Harmonic Correction Term to the
	     * Argument of Latitude, 16 bits, signed, scale 2**-29, radians */
	    int16_t Cuc;
	    double d_Cuc;
	    /* deltan, Mean Motion Difference From Computed Value
	     * Mean Motion Difference From Computed Value
	     * 16 bits, signed, scale 2**-43, semi-circles/sec */
	    int16_t deltan;
	    double d_deltan;
	    /* M0, Mean Anomaly at Reference Time, 32 bits signed,
	     * scale 2**-31, semi-circles */
	    int32_t M0;
	    double d_M0;
	    /* eccentricity, 32 bits, unsigned, scale 2**-33, dimensionless */
	    uint32_t e;
	    double d_eccentricity;
	    /* sqrt A, Square Root of the Semi-Major Axis
	     * 32 bits unsigned, scale 2**-19, square_root(meters) */
	    uint32_t sqrtA;
	    double d_sqrtA;
	} sub2;
        /* subframe 3, part of ephemeris, see IS-GPS-200E, Table 20-II,
	 * Table 20-III */
	struct {
	    /* Issue of Data (Ephemeris), 8 bits, unsigned
	     * equal to the 8 LSBs of the 10 bit IODC of the same data set */
	    uint8_t IODE;
	    /* Rate of Inclination Angle, 14 bits signed, scale2**-43,
	     * semi-circles/sec */
	    int16_t IDOT;
	    double d_IDOT;
	    /* Cic, Amplitude of the Cosine Harmonic Correction Term to the
	     * Angle of Inclination, 16 bits signed, scale 2**-29, radians*/
	    int16_t Cic;
	    double d_Cic;
	    /* Cis, Amplitude of the Sine Harmonic Correction Term to the
	     * Angle of Inclination, 16 bits, unsigned, scale 2**-29, radians */
	    int16_t Cis;
	    double d_Cis;
            /* Crc, Amplitude of the Cosine Harmonic Correction Term to the
	     * Orbit Radius, 16 bits signed, scale 2**-5, meters */
	    int16_t Crc;
	    double d_Crc;
	    /* i0, Inclination Angle at Reference Time, 32 bits, signed,
	     * scale 2**-31, semi-circles */
	    int32_t i0;
	    double d_i0;
	    /* Omega0, Longitude of Ascending Node of Orbit Plane at Weekly
	     * Epoch, 32 bits signed, semi-circles */
	    int32_t Omega0;
	    double d_Omega0;
	    /* omega, Argument of Perigee, 32 bits signed, scale 2**-31,
	     * semi-circles */
	    int32_t omega;
	    double d_omega;
	    /* Omega dot, Rate of Right Ascension, 24 bits signed,
	     * scale 2**-43, semi-circles/sec */
	    int32_t Omegad;
	    double d_Omegad;
	} sub3;
	struct {
	    struct almanac_t almanac;
	} sub4;
	/* subframe 4, page 13 */
	struct {
	    /* mapping ord ERD# to SV # is non trivial
	     * leave it alone.  See IS-GPS-200E Section 20.3.3.5.1.9 */
	    /* Estimated Range Deviation, 6 bits signed, meters */
	    char ERD[33];
	    /* ai, Availability Indicator, 2bits, bit map */
	    unsigned char ai;
	} sub4_13;
	/* subframe 4, page 17, system message, 23 chars, plus nul */
	struct {
	    char str[24];
	} sub4_17;
	/* subframe 4, page 18 */
	struct {
	    /* ionospheric and UTC data */
	    /* A0, Bias coefficient of GPS time scale relative to UTC time
	     * scale, 32 bits signed, scale 2**-30, seconds */
	    int32_t A0;
	    double d_A0;
	    /* A1, Drift coefficient of GPS time scale relative to UTC time
	     * scale, 24 bits signed, scale 2**-50, seconds/second */
	    int32_t A1;
	    double d_A1;

	    /* alphaX, the four coefficients of a cubic equation representing 
	     * the amplitude of the vertical delay */

	    /* alpha0, 8 bits signed, scale w**-30, seconds */
	    int8_t alpha0;
	    double d_alpha0;
	    /* alpha1, 8 bits signed, scale w**-27, seconds/semi-circle */
	    int8_t alpha1;
	    double d_alpha1;
	    /* alpha2, 8 bits signed, scale w**-24, seconds/semi-circle**2 */
	    int8_t alpha2;
	    double d_alpha2;
	    /* alpha3, 8 bits signed, scale w**-24, seconds/semi-circle**3 */
	    int8_t alpha3;
	    double d_alpha3;

	    /* betaX, the four coefficients of a cubic equation representing 
	     * the period of the model */

	    /* beta0, 8 bits signed, scale w**11, seconds */
	    int8_t beta0;
	    double d_beta0;
	    /* beta1, 8 bits signed, scale w**14, seconds/semi-circle */
	    int8_t beta1;
	    double d_beta1;
	    /* beta2, 8 bits signed, scale w**16, seconds/semi-circle**2 */
	    int8_t beta2;
	    double d_beta2;
	    /* beta3, 8 bits signed, scale w**16, seconds/semi-circle**3 */
	    int8_t beta3;
	    double d_beta3;

	    /* leap (delta t ls), current leap second, 8 bits signed,
	     * scale 1, seconds */
	    int8_t leap;
	    /* lsf (delta t lsf), future leap second, 8 bits signed,
	     * scale 1, seconds */
	    int8_t lsf;

	    /* tot, reference time for UTC data,
	     * 8 bits unsigned, scale 2**12, seconds */
	    uint8_t tot;
	    double d_tot;

	    /* WNt, UTC reference week number, 8 bits unsigned, scale 1,
	     * weeks */
	    uint8_t WNt;
	    /* WNlsf, Leap second reference Week Number,
	     * 8 bits unsigned, scale 1, weeks */
	    uint8_t WNlsf;
	    /* DN, Leap second reference Day Number , 8 bits unsigned,
	     * scale 1, days */
	    uint8_t DN;
	} sub4_18;
	/* subframe 4, page 25 */
	struct {
	    /* svf, A-S status and the configuration code of each SV
	     * 4 bits unsigned, bitmap */
	    unsigned char svf[33];
	    /* svh, SV health data for SV 25 through 32
	     * 6 bits unsigned bitmap */
	    uint8_t svhx[8];
	} sub4_25;
	struct {
	    struct almanac_t almanac;
	} sub5;
	struct {
	    /* toa, Almanac reference Time, 8 bits unsigned, scale 2**12,
	     * seconds */
	    uint8_t toa;
	    long l_toa;
	    /* WNa, Week Number almanac, 8 bits, scale 2, GPS Week
	     * Number % 256 */
	    uint8_t WNa;
	    /* sv, SV health status, 6 bits, bitmap */
	    uint8_t sv[25];
	} sub5_25;
    };
};

typedef uint32_t isgps30bits_t;

/* logging levels */
#define LOG_ERROR 1 /*errors, display always */
#define LOG_SHOUT 0 /* not an error but we should always see it */
#define LOG_WARN  1 /* not errors but may indicate a problem */
#define LOG_INF   2 /* key informative messages */
#define LOG_DATA  3 /* log data management messages */
#define LOG_PROG  4 /* progress messages */
#define LOG_IO    5 /* IO to and from devices */
#define LOG_SPIN  6 /* logging for catching spin bugs */
#define LOG_RAW	  7 /* raw low-level I/O */

#define ISGPS_ERRLEVEL_BASE     LOG_RAW

void gpsd_report(int errlevel, const char *fmt, ... );

unsigned gpsd_interpret_subframe_raw(struct subframe_t *subp,
      unsigned int tSVID, uint32_t words[]);

unsigned int isgps_parity(isgps30bits_t th);

#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif

#endif /* _GPSD_GPS_H_ */
/* gps.h ends here */
