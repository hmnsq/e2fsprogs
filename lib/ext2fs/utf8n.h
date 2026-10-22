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

#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <string.h>

/* Encoding a unicode version number as a single unsigned int. */
#define UNICODE_MAJ_SHIFT		(16)
#define UNICODE_MIN_SHIFT		(8)

#define UNICODE_AGE(MAJ, MIN, REV)			\
	(((unsigned முழு)(MAJ) << UNICODE_MAJ_SHIFT) |	\
	 ((unsigned முழு)(MIN) << UNICODE_MIN_SHIFT) |	\
	 ((unsigned முழு)(REV)))

/* Highest unicode version supported by the data tables. */
வெளி முழு utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev);
வெளி முழு utf8version_latest(வெறுமை);

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
வெளி மாறிலி கட்டமைப்பு utf8data *utf8nfdi(unsigned முழு maxage);
வெளி மாறிலி கட்டமைப்பு utf8data *utf8nfdicf(unsigned முழு maxage);

/*
 * Determine the maximum age of any unicode character in the string.
 * Returns 0 if only unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
வெளி முழு utf8agemax(மாறிலி கட்டமைப்பு utf8data *data, மாறிலி எழுத்து *s);
வெளி முழு utf8nagemax(மாறிலி கட்டமைப்பு utf8data *data, மாறிலி எழுத்து *s, size_t len);

/*
 * Determine the minimum age of any unicode character in the string.
 * Returns 0 if any unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
வெளி முழு utf8agemin(மாறிலி கட்டமைப்பு utf8data *data, மாறிலி எழுத்து *s);
வெளி முழு utf8nagemin(மாறிலி கட்டமைப்பு utf8data *data, மாறிலி எழுத்து *s, size_t len);

/*
 * Determine the length of the normalized from of the string,
 * excluding any terminating NULL byte.
 * Returns 0 if only ignorable code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
வெளி ssize_t utf8len(மாறிலி கட்டமைப்பு utf8data *data, மாறிலி எழுத்து *s);
வெளி ssize_t utf8nlen(மாறிலி கட்டமைப்பு utf8data *data, மாறிலி எழுத்து *s, size_t len);

/* Needed in struct utf8cursor below. */
#define UTF8HANGULLEAF	(12)

/*
 * Cursor structure used by the normalizer.
 */
கட்டமைப்பு utf8cursor {
	மாறிலி கட்டமைப்பு utf8data	*data;
	மாறிலி எழுத்து	*s;
	மாறிலி எழுத்து	*p;
	மாறிலி எழுத்து	*ss;
	மாறிலி எழுத்து	*sp;
	unsigned முழு	len;
	unsigned முழு	slen;
	short முழு	ccc;
	short முழு	nccc;
	unsigned எழுத்து	hangul[UTF8HANGULLEAF];
};

/*
 * Initialize a utf8cursor to normalize a string.
 * Returns 0 on success.
 * Returns -1 on failure.
 */
வெளி முழு utf8cursor(கட்டமைப்பு utf8cursor *u8c, மாறிலி கட்டமைப்பு utf8data *data,
		      மாறிலி எழுத்து *s);
வெளி முழு utf8ncursor(கட்டமைப்பு utf8cursor *u8c, மாறிலி கட்டமைப்பு utf8data *data,
		       மாறிலி எழுத்து *s, size_t len);

/*
 * Get the next byte in the normalization.
 * Returns a value > 0 && < 256 on success.
 * Returns 0 when the end of the normalization is reached.
 * Returns -1 if the string being normalized is not valid UTF-8.
 */
வெளி முழு utf8byte(கட்டமைப்பு utf8cursor *u8c);

#endif /* UTF8NORM_H */
