/*
 * Copyright (c) 2014 SGI.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

/* This code is copied from the linux kernel.  We have a userspace
 * version here to such that hashes will match that implementation.
 */

#ifndef UTF8NORM_H
#define UTF8NORM_H

#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <string.h>

/* Encoding a unicode version number as a single unsigned int. */
#define UNICODE_MAJ_SHIFT		(16)
#define UNICODE_MIN_SHIFT		(8)

#define UNICODE_AGE(MAJ, MIN, REV)			\
	(((unsigned പൂർണ്ണസംഖ്യ)(MAJ) << UNICODE_MAJ_SHIFT) |	\
	 ((unsigned പൂർണ്ണസംഖ്യ)(MIN) << UNICODE_MIN_SHIFT) |	\
	 ((unsigned പൂർണ്ണസംഖ്യ)(REV)))

/* Highest unicode version supported by the data tables. */
ബാഹ്യ പൂർണ്ണസംഖ്യ utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev);
ബാഹ്യ പൂർണ്ണസംഖ്യ utf8version_latest(ശൂന്യം);

/*
 * Look for the correct const struct utf8data for a unicode version.
 * Returns NULL if the version requested is too new.
 *
 * Two normalization forms are supported: nfdi and nfdicf.
 *
 * nfdi:
 *  - Apply unicode normalization form NFD.
 *  - Remove any Default_Ignorable_Code_Point.
 *
 * nfdicf:
 *  - Apply unicode normalization form NFD.
 *  - Remove any Default_Ignorable_Code_Point.
 *  - Apply a full casefold (C + F).
 */
ബാഹ്യ സ്ഥിരം ഘടന utf8data *utf8nfdi(unsigned പൂർണ്ണസംഖ്യ maxage);
ബാഹ്യ സ്ഥിരം ഘടന utf8data *utf8nfdicf(unsigned പൂർണ്ണസംഖ്യ maxage);

/*
 * Determine the maximum age of any unicode character in the string.
 * Returns 0 if only unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
ബാഹ്യ പൂർണ്ണസംഖ്യ utf8agemax(സ്ഥിരം ഘടന utf8data *data, സ്ഥിരം അക്ഷരം *s);
ബാഹ്യ പൂർണ്ണസംഖ്യ utf8nagemax(സ്ഥിരം ഘടന utf8data *data, സ്ഥിരം അക്ഷരം *s, size_t len);

/*
 * Determine the minimum age of any unicode character in the string.
 * Returns 0 if any unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
ബാഹ്യ പൂർണ്ണസംഖ്യ utf8agemin(സ്ഥിരം ഘടന utf8data *data, സ്ഥിരം അക്ഷരം *s);
ബാഹ്യ പൂർണ്ണസംഖ്യ utf8nagemin(സ്ഥിരം ഘടന utf8data *data, സ്ഥിരം അക്ഷരം *s, size_t len);

/*
 * Determine the length of the normalized from of the string,
 * excluding any terminating NULL byte.
 * Returns 0 if only ignorable code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
ബാഹ്യ ssize_t utf8len(സ്ഥിരം ഘടന utf8data *data, സ്ഥിരം അക്ഷരം *s);
ബാഹ്യ ssize_t utf8nlen(സ്ഥിരം ഘടന utf8data *data, സ്ഥിരം അക്ഷരം *s, size_t len);

/* Needed in struct utf8cursor below. */
#define UTF8HANGULLEAF	(12)

/*
 * Cursor structure used by the normalizer.
 */
ഘടന utf8cursor {
	സ്ഥിരം ഘടന utf8data	*data;
	സ്ഥിരം അക്ഷരം	*s;
	സ്ഥിരം അക്ഷരം	*p;
	സ്ഥിരം അക്ഷരം	*ss;
	സ്ഥിരം അക്ഷരം	*sp;
	unsigned പൂർണ്ണസംഖ്യ	len;
	unsigned പൂർണ്ണസംഖ്യ	slen;
	short പൂർണ്ണസംഖ്യ	ccc;
	short പൂർണ്ണസംഖ്യ	nccc;
	unsigned അക്ഷരം	hangul[UTF8HANGULLEAF];
};

/*
 * Initialize a utf8cursor to normalize a string.
 * Returns 0 on success.
 * Returns -1 on failure.
 */
ബാഹ്യ പൂർണ്ണസംഖ്യ utf8cursor(ഘടന utf8cursor *u8c, സ്ഥിരം ഘടന utf8data *data,
		      സ്ഥിരം അക്ഷരം *s);
ബാഹ്യ പൂർണ്ണസംഖ്യ utf8ncursor(ഘടന utf8cursor *u8c, സ്ഥിരം ഘടന utf8data *data,
		       സ്ഥിരം അക്ഷരം *s, size_t len);

/*
 * Get the next byte in the normalization.
 * Returns a value > 0 && < 256 on success.
 * Returns 0 when the end of the normalization is reached.
 * Returns -1 if the string being normalized is not valid UTF-8.
 */
ബാഹ്യ പൂർണ്ണസംഖ്യ utf8byte(ഘടന utf8cursor *u8c);

#endif /* UTF8NORM_H */
