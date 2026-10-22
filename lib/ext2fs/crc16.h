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

#که_نه_تعریف_شوی __CRC16_H
#تعریف_کړه __CRC16_H

/* for an unknown reason, PPC treats __u16 as signed and keeps doing sign
 * extension on the value.  Instead, use only the low 16 bits of an
 * unsigned int for holding the CRC value to avoid this.
 */
تعريفنوع بینښه صحيح crc16_t;

خارجي crc16_t ext2fs_crc16(crc16_t دوراني_ازموینه, ثابت فراغ *بفر, بینښه صحيح len);

#د_که_پای /* __CRC16_H */
