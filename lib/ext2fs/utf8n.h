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

#شامل <stdint.h>
#شامل <unistd.h>
#شامل <string.h>

/* Encoding a unicode version number as a single unsigned int. */
#define UNICODE_MAJ_SHIFT		(16)
#define UNICODE_MIN_SHIFT		(8)

#define UNICODE_AGE(MAJ, MIN, REV)			\
	(((unsigned صحیح)(MAJ) << UNICODE_MAJ_SHIFT) |	\
	 ((unsigned صحیح)(MIN) << UNICODE_MIN_SHIFT) |	\
	 ((unsigned صحیح)(REV)))

/* Highest unicode version supported by the data tables. */
بیرونی صحیح utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev);
بیرونی صحیح utf8version_latest(تهی);

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
بیرونی ثابت ساختار utf8data *utf8nfdi(unsigned صحیح maxage);
بیرونی ثابت ساختار utf8data *utf8nfdicf(unsigned صحیح maxage);

/*
 * Determine the maximum age of any unicode character in the string.
 * Returns 0 if only unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
بیرونی صحیح utf8agemax(ثابت ساختار utf8data *data, ثابت نویسه *s);
بیرونی صحیح utf8nagemax(ثابت ساختار utf8data *data, ثابت نویسه *s, size_t len);

/*
 * Determine the minimum age of any unicode character in the string.
 * Returns 0 if any unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
بیرونی صحیح utf8agemin(ثابت ساختار utf8data *data, ثابت نویسه *s);
بیرونی صحیح utf8nagemin(ثابت ساختار utf8data *data, ثابت نویسه *s, size_t len);

/*
 * Determine the length of the normalized from of the string,
 * excluding any terminating NULL byte.
 * Returns 0 if only ignorable code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
بیرونی ssize_t utf8len(ثابت ساختار utf8data *data, ثابت نویسه *s);
بیرونی ssize_t utf8nlen(ثابت ساختار utf8data *data, ثابت نویسه *s, size_t len);

/* Needed in struct utf8cursor below. */
#define UTF8HANGULLEAF	(12)

/*
 * Cursor structure used by the normalizer.
 */
ساختار utf8cursor {
	ثابت ساختار utf8data	*data;
	ثابت نویسه	*s;
	ثابت نویسه	*p;
	ثابت نویسه	*ss;
	ثابت نویسه	*sp;
	unsigned صحیح	len;
	unsigned صحیح	slen;
	short صحیح	ccc;
	short صحیح	nccc;
	unsigned نویسه	hangul[UTF8HANGULLEAF];
};

/*
 * Initialize a utf8cursor to normalize a string.
 * Returns 0 on success.
 * Returns -1 on failure.
 */
بیرونی صحیح utf8cursor(ساختار utf8cursor *u8c, ثابت ساختار utf8data *data,
		      ثابت نویسه *s);
بیرونی صحیح utf8ncursor(ساختار utf8cursor *u8c, ثابت ساختار utf8data *data,
		       ثابت نویسه *s, size_t len);

/*
 * Get the next byte in the normalization.
 * Returns a value > 0 && < 256 on success.
 * Returns 0 when the end of the normalization is reached.
 * Returns -1 if the string being normalized is not valid UTF-8.
 */
بیرونی صحیح utf8byte(ساختار utf8cursor *u8c);

#endif /* UTF8NORM_H */
