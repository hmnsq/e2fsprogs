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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __CRC16_H
#ವ್ಯಾಖ್ಯಾನಿಸು __CRC16_H

/* for an unknown reason, PPC treats __u16 as signed and keeps doing sign
 * extension on the value.  Instead, use only the low 16 bits of an
 * unsigned int for holding the CRC value to avoid this.
 */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ crc16_t;

ಬಾಹ್ಯ crc16_t ext2fs_crc16(crc16_t ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಬಫರ್, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ len);

#ಆದರೆ_ಅಂತ್ಯ /* __CRC16_H */
