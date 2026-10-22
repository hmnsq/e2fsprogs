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

#অন্তর্ভাব <stdint.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <string.h>

/* Encoding a unicode version number as a single unsigned int. */
#define UNICODE_MAJ_SHIFT		(16)
#define UNICODE_MIN_SHIFT		(8)

#define UNICODE_AGE(MAJ, MIN, REV)			\
	(((unsigned পূর্ণাঙ্ক)(MAJ) << UNICODE_MAJ_SHIFT) |	\
	 ((unsigned পূর্ণাঙ্ক)(MIN) << UNICODE_MIN_SHIFT) |	\
	 ((unsigned পূর্ণাঙ্ক)(REV)))

/* Highest unicode version supported by the data tables. */
বাহ্য পূর্ণাঙ্ক utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev);
বাহ্য পূর্ণাঙ্ক utf8version_latest(শূন্য);

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
বাহ্য ধ্রুব সংরচনা utf8data *utf8nfdi(unsigned পূর্ণাঙ্ক maxage);
বাহ্য ধ্রুব সংরচনা utf8data *utf8nfdicf(unsigned পূর্ণাঙ্ক maxage);

/*
 * Determine the maximum age of any unicode character in the string.
 * Returns 0 if only unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
বাহ্য পূর্ণাঙ্ক utf8agemax(ধ্রুব সংরচনা utf8data *data, ধ্রুব বর্ণ *s);
বাহ্য পূর্ণাঙ্ক utf8nagemax(ধ্রুব সংরচনা utf8data *data, ধ্রুব বর্ণ *s, size_t len);

/*
 * Determine the minimum age of any unicode character in the string.
 * Returns 0 if any unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
বাহ্য পূর্ণাঙ্ক utf8agemin(ধ্রুব সংরচনা utf8data *data, ধ্রুব বর্ণ *s);
বাহ্য পূর্ণাঙ্ক utf8nagemin(ধ্রুব সংরচনা utf8data *data, ধ্রুব বর্ণ *s, size_t len);

/*
 * Determine the length of the normalized from of the string,
 * excluding any terminating NULL byte.
 * Returns 0 if only ignorable code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
বাহ্য ssize_t utf8len(ধ্রুব সংরচনা utf8data *data, ধ্রুব বর্ণ *s);
বাহ্য ssize_t utf8nlen(ধ্রুব সংরচনা utf8data *data, ধ্রুব বর্ণ *s, size_t len);

/* Needed in struct utf8cursor below. */
#define UTF8HANGULLEAF	(12)

/*
 * Cursor structure used by the normalizer.
 */
সংরচনা utf8cursor {
	ধ্রুব সংরচনা utf8data	*data;
	ধ্রুব বর্ণ	*s;
	ধ্রুব বর্ণ	*p;
	ধ্রুব বর্ণ	*ss;
	ধ্রুব বর্ণ	*sp;
	unsigned পূর্ণাঙ্ক	len;
	unsigned পূর্ণাঙ্ক	slen;
	short পূর্ণাঙ্ক	ccc;
	short পূর্ণাঙ্ক	nccc;
	unsigned বর্ণ	hangul[UTF8HANGULLEAF];
};

/*
 * Initialize a utf8cursor to normalize a string.
 * Returns 0 on success.
 * Returns -1 on failure.
 */
বাহ্য পূর্ণাঙ্ক utf8cursor(সংরচনা utf8cursor *u8c, ধ্রুব সংরচনা utf8data *data,
		      ধ্রুব বর্ণ *s);
বাহ্য পূর্ণাঙ্ক utf8ncursor(সংরচনা utf8cursor *u8c, ধ্রুব সংরচনা utf8data *data,
		       ধ্রুব বর্ণ *s, size_t len);

/*
 * Get the next byte in the normalization.
 * Returns a value > 0 && < 256 on success.
 * Returns 0 when the end of the normalization is reached.
 * Returns -1 if the string being normalized is not valid UTF-8.
 */
বাহ্য পূর্ণাঙ্ক utf8byte(সংরচনা utf8cursor *u8c);

#endif /* UTF8NORM_H */
