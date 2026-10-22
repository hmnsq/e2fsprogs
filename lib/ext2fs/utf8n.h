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

#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <string.h>

/* Encoding a unicode version number as a single unsigned int. */
#define UNICODE_MAJ_SHIFT		(16)
#define UNICODE_MIN_SHIFT		(8)

#define UNICODE_AGE(MAJ, MIN, REV)			\
	(((unsigned ಪೂರ್ಣಾಙ್ಕ)(MAJ) << UNICODE_MAJ_SHIFT) |	\
	 ((unsigned ಪೂರ್ಣಾಙ್ಕ)(MIN) << UNICODE_MIN_SHIFT) |	\
	 ((unsigned ಪೂರ್ಣಾಙ್ಕ)(REV)))

/* Highest unicode version supported by the data tables. */
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ utf8version_latest(ಶೂನ್ಯ);

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
ಬಾಹ್ಯ ಧ್ರುವ ಸಂರಚನಾ utf8data *utf8nfdi(unsigned ಪೂರ್ಣಾಙ್ಕ maxage);
ಬಾಹ್ಯ ಧ್ರುವ ಸಂರಚನಾ utf8data *utf8nfdicf(unsigned ಪೂರ್ಣಾಙ್ಕ maxage);

/*
 * Determine the maximum age of any unicode character in the string.
 * Returns 0 if only unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ utf8agemax(ಧ್ರುವ ಸಂರಚನಾ utf8data *data, ಧ್ರುವ ವರ್ಣ *s);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ utf8nagemax(ಧ್ರುವ ಸಂರಚನಾ utf8data *data, ಧ್ರುವ ವರ್ಣ *s, size_t len);

/*
 * Determine the minimum age of any unicode character in the string.
 * Returns 0 if any unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ utf8agemin(ಧ್ರುವ ಸಂರಚನಾ utf8data *data, ಧ್ರುವ ವರ್ಣ *s);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ utf8nagemin(ಧ್ರುವ ಸಂರಚನಾ utf8data *data, ಧ್ರುವ ವರ್ಣ *s, size_t len);

/*
 * Determine the length of the normalized from of the string,
 * excluding any terminating NULL byte.
 * Returns 0 if only ignorable code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
ಬಾಹ್ಯ ssize_t utf8len(ಧ್ರುವ ಸಂರಚನಾ utf8data *data, ಧ್ರುವ ವರ್ಣ *s);
ಬಾಹ್ಯ ssize_t utf8nlen(ಧ್ರುವ ಸಂರಚನಾ utf8data *data, ಧ್ರುವ ವರ್ಣ *s, size_t len);

/* Needed in struct utf8cursor below. */
#define UTF8HANGULLEAF	(12)

/*
 * Cursor structure used by the normalizer.
 */
ಸಂರಚನಾ utf8cursor {
	ಧ್ರುವ ಸಂರಚನಾ utf8data	*data;
	ಧ್ರುವ ವರ್ಣ	*s;
	ಧ್ರುವ ವರ್ಣ	*p;
	ಧ್ರುವ ವರ್ಣ	*ss;
	ಧ್ರುವ ವರ್ಣ	*sp;
	unsigned ಪೂರ್ಣಾಙ್ಕ	len;
	unsigned ಪೂರ್ಣಾಙ್ಕ	slen;
	short ಪೂರ್ಣಾಙ್ಕ	ccc;
	short ಪೂರ್ಣಾಙ್ಕ	nccc;
	unsigned ವರ್ಣ	hangul[UTF8HANGULLEAF];
};

/*
 * Initialize a utf8cursor to normalize a string.
 * Returns 0 on success.
 * Returns -1 on failure.
 */
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ utf8cursor(ಸಂರಚನಾ utf8cursor *u8c, ಧ್ರುವ ಸಂರಚನಾ utf8data *data,
		      ಧ್ರುವ ವರ್ಣ *s);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ utf8ncursor(ಸಂರಚನಾ utf8cursor *u8c, ಧ್ರುವ ಸಂರಚನಾ utf8data *data,
		       ಧ್ರುವ ವರ್ಣ *s, size_t len);

/*
 * Get the next byte in the normalization.
 * Returns a value > 0 && < 256 on success.
 * Returns 0 when the end of the normalization is reached.
 * Returns -1 if the string being normalized is not valid UTF-8.
 */
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ utf8byte(ಸಂರಚನಾ utf8cursor *u8c);

#endif /* UTF8NORM_H */
