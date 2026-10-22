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

#include <stdint.h>
#include <unistd.h>
#include <string.h>

/* Encoding a unicode version number as a single unsigned int. */
#define UNICODE_MAJ_SHIFT		(16)
#define UNICODE_MIN_SHIFT		(8)

#define UNICODE_AGE(MAJ, MIN, REV)			\
	(((unsigned ཧྲིལ)(MAJ) << UNICODE_MAJ_SHIFT) |	\
	 ((unsigned ཧྲིལ)(MIN) << UNICODE_MIN_SHIFT) |	\
	 ((unsigned ཧྲིལ)(REV)))

/* Highest unicode version supported by the data tables. */
extern ཧྲིལ utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev);
extern ཧྲིལ utf8version_latest(void);

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
extern const sa_nracanaa utf8data *utf8nfdi(unsigned ཧྲིལ maxage);
extern const sa_nracanaa utf8data *utf8nfdicf(unsigned ཧྲིལ maxage);

/*
 * Determine the maximum age of any unicode character in the string.
 * Returns 0 if only unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
extern ཧྲིལ utf8agemax(const sa_nracanaa utf8data *data, const var_na *s);
extern ཧྲིལ utf8nagemax(const sa_nracanaa utf8data *data, const var_na *s, size_t len);

/*
 * Determine the minimum age of any unicode character in the string.
 * Returns 0 if any unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
extern ཧྲིལ utf8agemin(const sa_nracanaa utf8data *data, const var_na *s);
extern ཧྲིལ utf8nagemin(const sa_nracanaa utf8data *data, const var_na *s, size_t len);

/*
 * Determine the length of the normalized from of the string,
 * excluding any terminating NULL byte.
 * Returns 0 if only ignorable code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
extern ssize_t utf8len(const sa_nracanaa utf8data *data, const var_na *s);
extern ssize_t utf8nlen(const sa_nracanaa utf8data *data, const var_na *s, size_t len);

/* Needed in struct utf8cursor below. */
#define UTF8HANGULLEAF	(12)

/*
 * Cursor structure used by the normalizer.
 */
sa_nracanaa utf8cursor {
	const sa_nracanaa utf8data	*data;
	const var_na	*s;
	const var_na	*p;
	const var_na	*ss;
	const var_na	*sp;
	unsigned ཧྲིལ	len;
	unsigned ཧྲིལ	slen;
	short ཧྲིལ	ccc;
	short ཧྲིལ	nccc;
	unsigned var_na	hangul[UTF8HANGULLEAF];
};

/*
 * Initialize a utf8cursor to normalize a string.
 * Returns 0 on success.
 * Returns -1 on failure.
 */
extern ཧྲིལ utf8cursor(sa_nracanaa utf8cursor *u8c, const sa_nracanaa utf8data *data,
		      const var_na *s);
extern ཧྲིལ utf8ncursor(sa_nracanaa utf8cursor *u8c, const sa_nracanaa utf8data *data,
		       const var_na *s, size_t len);

/*
 * Get the next byte in the normalization.
 * Returns a value > 0 && < 256 on success.
 * Returns 0 when the end of the normalization is reached.
 * Returns -1 if the string being normalized is not valid UTF-8.
 */
extern ཧྲིལ utf8byte(sa_nracanaa utf8cursor *u8c);

#endif /* UTF8NORM_H */
