/*****************************************************************************

This is a decoder for the unnamed protocol described in IS-GPS-200,
the Navstar GPS Interface Specification, and used as a transport layer
for both GPS satellite downlink transmissions and the RTCM104 version 2
format for broadcasting differential-GPS corrections.

The purpose of this protocol is to support analyzing a serial bit
stream without byte framing into parity-checked packets.
Interpretation of the packets is left to an upper layer. Note that
RTCM104 version 3 does *not* use this code; it assumes a byte-oriented
underlayer.

The upper layer must supply a preamble_match() hook to tell our
decoder when it has a legitimate start of packet, and a length_check()
hook to tell it when the packet has reached the length it is supposed
to have.

Here are Wolfgang's original rather cryptic notes on this code:

--------------------------------------------------------------------------
1) trim and bitflip the input.

While syncing the msb of the input gets shifted into lsb of the
assembled word.
    word <<= 1, or in input >> 5
    word <<= 1, or in input >> 4
    word <<= 1, or in input >> 3
    word <<= 1, or in input >> 2
    word <<= 1, or in input >> 1
    word <<= 1, or in input

At one point it should sync-lock.

----

Shift 6 bytes of RTCM data in as such:

---> (trim-bits-to-5-bits) ---> (end-for-end-bit-flip) --->

---> shift-into-30-bit-shift-register
              |||||||||||||||||||||||
	      detector-for-preamble
              |||||||||||||||||||||||
              detector-for-parity
              |||||||||||||||||||||||
--------------------------------------------------------------------------

The code was originally by Wolfgang Rupprecht.  ESR severely hacked
it, with Wolfgang's help, in order to separate message analysis from
message dumping and separate this lower layer from the upper layer
handing GPS-subframe and RTCM decoding.

You are not expected to understand any of this.

This file is Copyright (c) 2010 by the GPSD project
BSD terms apply: see the file COPYING in the distribution root for details.

*****************************************************************************/

#include "gps.h"

#define MAG_SHIFT 6u
#define MAG_TAG_DATA (1 << MAG_SHIFT)
#define MAG_TAG_MASK (3 << MAG_SHIFT)

#define W_DATA_MASK	0x3fffffc0u

/*@ +charint @*/
static unsigned char parity_array[] = {
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0
};

static unsigned int reverse_bits[] = {
    0, 32, 16, 48, 8, 40, 24, 56, 4, 36, 20, 52, 12, 44, 28, 60,
    2, 34, 18, 50, 10, 42, 26, 58, 6, 38, 22, 54, 14, 46, 30, 62,
    1, 33, 17, 49, 9, 41, 25, 57, 5, 37, 21, 53, 13, 45, 29, 61,
    3, 35, 19, 51, 11, 43, 27, 59, 7, 39, 23, 55, 15, 47, 31, 63
};

/*@ -charint @*/

unsigned int isgps_parity(isgps30bits_t th)
{
#define P_30_MASK	0x40000000u

#define	PARITY_25	0xbb1f3480u
#define	PARITY_26	0x5d8f9a40u
#define	PARITY_27	0xaec7cd00u
#define	PARITY_28	0x5763e680u
#define	PARITY_29	0x6bb1f340u
#define	PARITY_30	0x8b7a89c0u
    isgps30bits_t t;
    unsigned int p;

    /*
     * if (th & P_30_MASK)
     * th ^= W_DATA_MASK;
     */

    /*@ +charint @*/
    t = th & PARITY_25;
    p = parity_array[t & 0xff] ^ parity_array[(t >> 8) & 0xff] ^
	parity_array[(t >> 16) & 0xff] ^ parity_array[(t >> 24) & 0xff];
    t = th & PARITY_26;
    p = (p << 1) | (parity_array[t & 0xff] ^ parity_array[(t >> 8) & 0xff] ^
		    parity_array[(t >> 16) & 0xff] ^ parity_array[(t >> 24) &
								  0xff]);
    t = th & PARITY_27;
    p = (p << 1) | (parity_array[t & 0xff] ^ parity_array[(t >> 8) & 0xff] ^
		    parity_array[(t >> 16) & 0xff] ^ parity_array[(t >> 24) &
								  0xff]);
    t = th & PARITY_28;
    p = (p << 1) | (parity_array[t & 0xff] ^ parity_array[(t >> 8) & 0xff] ^
		    parity_array[(t >> 16) & 0xff] ^ parity_array[(t >> 24) &
								  0xff]);
    t = th & PARITY_29;
    p = (p << 1) | (parity_array[t & 0xff] ^ parity_array[(t >> 8) & 0xff] ^
		    parity_array[(t >> 16) & 0xff] ^ parity_array[(t >> 24) &
								  0xff]);
    t = th & PARITY_30;
    p = (p << 1) | (parity_array[t & 0xff] ^ parity_array[(t >> 8) & 0xff] ^
		    parity_array[(t >> 16) & 0xff] ^ parity_array[(t >> 24) &
								  0xff]);
    /*@ -charint @*/

    gpsd_report(ISGPS_ERRLEVEL_BASE + 2, "ISGPS parity %u\n", p);
    return (p);
}

/*@ +usereleased +compdef @*/
