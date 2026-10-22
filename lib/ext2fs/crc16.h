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

#నిర్వచితం_కాకపోతే __CRC16_H
#నిర్వచించు __CRC16_H

/* for an unknown reason, PPC treats __u16 as signed and keeps doing sign
 * extension on the value.  Instead, use only the low 16 bits of an
 * unsigned int for holding the CRC value to avoid this.
 */
రకనిర్వచనం గుర్తులేని పూర్ణాంకం crc16_t;

బాహ్య crc16_t ext2fs_crc16(crc16_t చక్రీయ_తనిఖీ, స్థిరరాశి శూన్య *బఫరు, గుర్తులేని పూర్ణాంకం len);

#అయితే_ముగింపు /* __CRC16_H */
