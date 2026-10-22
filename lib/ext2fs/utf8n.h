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

#اگر_غیر_تعریف_شدہ UTF8NORM_H
#تعریف_کرو UTF8NORM_H

#شامل <stdint.h>
#شامل <unistd.h>
#شامل <string.h>

/* Encoding a unicode version number as a single unsigned int. */
#تعریف_کرو UNICODE_MAJ_سرکاؤ		(16)
#تعریف_کرو UNICODE_MIN_سرکاؤ		(8)

#تعریف_کرو UNICODE_AGE(MAJ, MIN, REV)			\
	(((غیرعلامتی صحیح)(MAJ) << UNICODE_MAJ_سرکاؤ) |	\
	 ((غیرعلامتی صحیح)(MIN) << UNICODE_MIN_سرکاؤ) |	\
	 ((غیرعلامتی صحیح)(REV)))

/* Highest unicode version supported by the data tables. */
خارجی صحیح utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev);
خارجی صحیح utf8version_latest(خالی);

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
خارجی ثابت ساخت utf8data *utf8nfdi(غیرعلامتی صحیح maxage);
خارجی ثابت ساخت utf8data *utf8nfdicf(غیرعلامتی صحیح maxage);

/*
 * Determine the maximum age of any unicode character in the string.
 * Returns 0 if only unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
خارجی صحیح utf8agemax(ثابت ساخت utf8data *ڈیٹا, ثابت حرف *s);
خارجی صحیح utf8nagemax(ثابت ساخت utf8data *ڈیٹا, ثابت حرف *s, size_t len);

/*
 * Determine the minimum age of any unicode character in the string.
 * Returns 0 if any unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
خارجی صحیح utf8agemin(ثابت ساخت utf8data *ڈیٹا, ثابت حرف *s);
خارجی صحیح utf8nagemin(ثابت ساخت utf8data *ڈیٹا, ثابت حرف *s, size_t len);

/*
 * Determine the length of the normalized from of the string,
 * excluding any terminating NULL byte.
 * Returns 0 if only ignorable code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
خارجی ssize_t utf8len(ثابت ساخت utf8data *ڈیٹا, ثابت حرف *s);
خارجی ssize_t utf8nlen(ثابت ساخت utf8data *ڈیٹا, ثابت حرف *s, size_t len);

/* Needed in struct utf8cursor below. */
#تعریف_کرو UTF8HANGULLEAF	(12)

/*
 * Cursor structure used by the normalizer.
 */
ساخت utf8cursor {
	ثابت ساخت utf8data	*ڈیٹا;
	ثابت حرف	*s;
	ثابت حرف	*p;
	ثابت حرف	*ss;
	ثابت حرف	*sp;
	غیرعلامتی صحیح	len;
	غیرعلامتی صحیح	slen;
	مختصر صحیح	ccc;
	مختصر صحیح	nccc;
	غیرعلامتی حرف	hangul[UTF8HANGULLEAF];
};

/*
 * Initialize a utf8cursor to normalize a string.
 * Returns 0 on success.
 * Returns -1 on failure.
 */
خارجی صحیح utf8cursor(ساخت utf8cursor *u8c, ثابت ساخت utf8data *ڈیٹا,
		      ثابت حرف *s);
خارجی صحیح utf8ncursor(ساخت utf8cursor *u8c, ثابت ساخت utf8data *ڈیٹا,
		       ثابت حرف *s, size_t len);

/*
 * Get the next byte in the normalization.
 * Returns a value > 0 && < 256 on success.
 * Returns 0 when the end of the normalization is reached.
 * Returns -1 if the string being normalized is not valid UTF-8.
 */
خارجی صحیح utf8byte(ساخت utf8cursor *u8c);

#اگر_ختم /* UTF8NORM_H */
