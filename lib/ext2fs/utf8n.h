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

#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

/* Encoding a unicode version number as a single unsigned int. */
#define UNICODE_MAJ_SHIFT		(16)
#define UNICODE_MIN_SHIFT		(8)

#define UNICODE_AGE(MAJ, MIN, REV)			\
	(((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(MAJ) << UNICODE_MAJ_SHIFT) |	\
	 ((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(MIN) << UNICODE_MIN_SHIFT) |	\
	 ((unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(REV)))

/* Highest unicode version supported by the data tables. */
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା utf8version_latest(ଶୂନ୍ୟ);

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
ବାହ୍ୟ ଅଚଳ ସଂରଚନା utf8data *utf8nfdi(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା maxage);
ବାହ୍ୟ ଅଚଳ ସଂରଚନା utf8data *utf8nfdicf(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା maxage);

/*
 * Determine the maximum age of any unicode character in the string.
 * Returns 0 if only unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା utf8agemax(ଅଚଳ ସଂରଚନା utf8data *data, ଅଚଳ ଅକ୍ଷର *s);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା utf8nagemax(ଅଚଳ ସଂରଚନା utf8data *data, ଅଚଳ ଅକ୍ଷର *s, size_t len);

/*
 * Determine the minimum age of any unicode character in the string.
 * Returns 0 if any unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା utf8agemin(ଅଚଳ ସଂରଚନା utf8data *data, ଅଚଳ ଅକ୍ଷର *s);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା utf8nagemin(ଅଚଳ ସଂରଚନା utf8data *data, ଅଚଳ ଅକ୍ଷର *s, size_t len);

/*
 * Determine the length of the normalized from of the string,
 * excluding any terminating NULL byte.
 * Returns 0 if only ignorable code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
ବାହ୍ୟ ssize_t utf8len(ଅଚଳ ସଂରଚନା utf8data *data, ଅଚଳ ଅକ୍ଷର *s);
ବାହ୍ୟ ssize_t utf8nlen(ଅଚଳ ସଂରଚନା utf8data *data, ଅଚଳ ଅକ୍ଷର *s, size_t len);

/* Needed in struct utf8cursor below. */
#define UTF8HANGULLEAF	(12)

/*
 * Cursor structure used by the normalizer.
 */
ସଂରଚନା utf8cursor {
	ଅଚଳ ସଂରଚନା utf8data	*data;
	ଅଚଳ ଅକ୍ଷର	*s;
	ଅଚଳ ଅକ୍ଷର	*p;
	ଅଚଳ ଅକ୍ଷର	*ss;
	ଅଚଳ ଅକ୍ଷର	*sp;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	len;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	slen;
	short ପୂର୍ଣ୍ଣସଂଖ୍ୟା	ccc;
	short ପୂର୍ଣ୍ଣସଂଖ୍ୟା	nccc;
	unsigned ଅକ୍ଷର	hangul[UTF8HANGULLEAF];
};

/*
 * Initialize a utf8cursor to normalize a string.
 * Returns 0 on success.
 * Returns -1 on failure.
 */
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା utf8cursor(ସଂରଚନା utf8cursor *u8c, ଅଚଳ ସଂରଚନା utf8data *data,
		      ଅଚଳ ଅକ୍ଷର *s);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା utf8ncursor(ସଂରଚନା utf8cursor *u8c, ଅଚଳ ସଂରଚନା utf8data *data,
		       ଅଚଳ ଅକ୍ଷର *s, size_t len);

/*
 * Get the next byte in the normalization.
 * Returns a value > 0 && < 256 on success.
 * Returns 0 when the end of the normalization is reached.
 * Returns -1 if the string being normalized is not valid UTF-8.
 */
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା utf8byte(ସଂରଚନା utf8cursor *u8c);

#endif /* UTF8NORM_H */
