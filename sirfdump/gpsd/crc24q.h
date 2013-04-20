/* Interface for CRC-24Q cyclic redundancy chercksum code
 *
 * This file is Copyright (c) 2010 by the GPSD project
 * BSD terms apply: see the file COPYING in the distribution root for details.
 */
#ifndef _CRC24Q_H_
#define _CRC24Q_H_

struct crc24_iovec {
    void *iov_base;
    size_t iov_len;
};

extern void crc24q_sign(unsigned char *data, int len);

extern unsigned crc24q_check(unsigned char *data, int len);

extern unsigned crc24q_hash(unsigned char *data, int len);

extern unsigned crc24q_hashv(struct crc24_iovec *iov, int iovcnt);
#endif /* _CRC24Q_H_ */
