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

#جے_بنا_تعریف UTF8NORM_H
#تعریف_کرو UTF8NORM_H

#ضمن <stdint.h>
#ضمن <unistd.h>
#ضمن <string.h>

/* Encoding a unicode version number as a single unsigned int. */
#تعریف_کرو UNICODE_MAJ_کھسکاؤ		(16)
#تعریف_کرو UNICODE_MIN_کھسکاؤ		(8)

#تعریف_کرو UNICODE_AGE(MAJ, MIN, REV)			\
	(((بنانشانی صحيح)(MAJ) << UNICODE_MAJ_کھسکاؤ) |	\
	 ((بنانشانی صحيح)(MIN) << UNICODE_MIN_کھسکاؤ) |	\
	 ((بنانشانی صحيح)(REV)))

/* Highest unicode version supported by the data tables. */
خارجي صحيح utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev);
خارجي صحيح utf8version_latest(فراغ);

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
خارجي ثابت بنية utf8data *utf8nfdi(بنانشانی صحيح maxage);
خارجي ثابت بنية utf8data *utf8nfdicf(بنانشانی صحيح maxage);

/*
 * Determine the maximum age of any unicode character in the string.
 * Returns 0 if only unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
خارجي صحيح utf8agemax(ثابت بنية utf8data *انکڑے, ثابت حرف *s);
خارجي صحيح utf8nagemax(ثابت بنية utf8data *انکڑے, ثابت حرف *s, size_t len);

/*
 * Determine the minimum age of any unicode character in the string.
 * Returns 0 if any unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
خارجي صحيح utf8agemin(ثابت بنية utf8data *انکڑے, ثابت حرف *s);
خارجي صحيح utf8nagemin(ثابت بنية utf8data *انکڑے, ثابت حرف *s, size_t len);

/*
 * Determine the length of the normalized from of the string,
 * excluding any terminating NULL byte.
 * Returns 0 if only ignorable code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
خارجي ssize_t utf8len(ثابت بنية utf8data *انکڑے, ثابت حرف *s);
خارجي ssize_t utf8nlen(ثابت بنية utf8data *انکڑے, ثابت حرف *s, size_t len);

/* Needed in struct utf8cursor below. */
#تعریف_کرو UTF8HANGULLEAF	(12)

/*
 * Cursor structure used by the normalizer.
 */
بنية utf8cursor {
	ثابت بنية utf8data	*انکڑے;
	ثابت حرف	*s;
	ثابت حرف	*p;
	ثابت حرف	*ss;
	ثابت حرف	*sp;
	بنانشانی صحيح	len;
	بنانشانی صحيح	slen;
	چھوٹا صحيح	ccc;
	چھوٹا صحيح	nccc;
	بنانشانی حرف	hangul[UTF8HANGULLEAF];
};

/*
 * Initialize a utf8cursor to normalize a string.
 * Returns 0 on success.
 * Returns -1 on failure.
 */
خارجي صحيح utf8cursor(بنية utf8cursor *u8c, ثابت بنية utf8data *انکڑے,
		      ثابت حرف *s);
خارجي صحيح utf8ncursor(بنية utf8cursor *u8c, ثابت بنية utf8data *انکڑے,
		       ثابت حرف *s, size_t len);

/*
 * Get the next byte in the normalization.
 * Returns a value > 0 && < 256 on success.
 * Returns 0 when the end of the normalization is reached.
 * Returns -1 if the string being normalized is not valid UTF-8.
 */
خارجي صحيح utf8byte(بنية utf8cursor *u8c);

#جے_انت /* UTF8NORM_H */
