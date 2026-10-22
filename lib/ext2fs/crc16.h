/*
 *	crc16.h - CRC-16 routine
 *
 * Implements the standard CRC-16:
 *   Width 16
 *   Poly  0x8005 (x16 + x15 + x2 + 1)
 *   Init  0
 *
 * Copyright (c) 2005 Ben Gardner <bgardner@wabtec.com>
 *
 * This source code is licensed under the GNU General Public License,
 * Version 2. See the file COPYING for more details.
 */

#यदि_अपरिभाषितम् __CRC16_H
#परिभाषय __CRC16_H

/* for an unknown reason, PPC treats __u16 as signed and keeps doing sign
 * extension on the value.  Instead, use only the low 16 bits of an
 * unsigned int for holding the CRC value to avoid this.
 */
प्रकारनिर्देश अचिह्नित पूर्णाङ्क crc16_t;

बाह्य crc16_t ext2fs_crc16(crc16_t crc, ध्रुव शून्य *बफर, अचिह्नित पूर्णाङ्क len);

#यदि_अन्तः /* __CRC16_H */
