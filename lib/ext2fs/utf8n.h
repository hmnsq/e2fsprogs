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

#यदि_अपरिभाषितम् UTF8NORM_H
#परिभाषय UTF8NORM_H

#अन्तर्भाव <stdint.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <string.h>

/* Encoding a unicode version number as a single unsigned int. */
#परिभाषय UNICODE_MAJ_सरणम्		(16)
#परिभाषय UNICODE_MIN_सरणम्		(8)

#परिभाषय UNICODE_AGE(MAJ, MIN, REV)			\
	(((अचिह्नित पूर्णाङ्क)(MAJ) << UNICODE_MAJ_सरणम्) |	\
	 ((अचिह्नित पूर्णाङ्क)(MIN) << UNICODE_MIN_सरणम्) |	\
	 ((अचिह्नित पूर्णाङ्क)(REV)))

/* Highest unicode version supported by the data tables. */
बाह्य पूर्णाङ्क utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev);
बाह्य पूर्णाङ्क utf8version_latest(शून्य);

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
बाह्य ध्रुव संरचना utf8data *utf8nfdi(अचिह्नित पूर्णाङ्क maxage);
बाह्य ध्रुव संरचना utf8data *utf8nfdicf(अचिह्नित पूर्णाङ्क maxage);

/*
 * Determine the maximum age of any unicode character in the string.
 * Returns 0 if only unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
बाह्य पूर्णाङ्क utf8agemax(ध्रुव संरचना utf8data *दत्त, ध्रुव वर्ण *s);
बाह्य पूर्णाङ्क utf8nagemax(ध्रुव संरचना utf8data *दत्त, ध्रुव वर्ण *s, size_t len);

/*
 * Determine the minimum age of any unicode character in the string.
 * Returns 0 if any unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
बाह्य पूर्णाङ्क utf8agemin(ध्रुव संरचना utf8data *दत्त, ध्रुव वर्ण *s);
बाह्य पूर्णाङ्क utf8nagemin(ध्रुव संरचना utf8data *दत्त, ध्रुव वर्ण *s, size_t len);

/*
 * Determine the length of the normalized from of the string,
 * excluding any terminating NULL byte.
 * Returns 0 if only ignorable code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
बाह्य ssize_t utf8len(ध्रुव संरचना utf8data *दत्त, ध्रुव वर्ण *s);
बाह्य ssize_t utf8nlen(ध्रुव संरचना utf8data *दत्त, ध्रुव वर्ण *s, size_t len);

/* Needed in struct utf8cursor below. */
#परिभाषय UTF8HANGULLEAF	(12)

/*
 * Cursor structure used by the normalizer.
 */
संरचना utf8cursor {
	ध्रुव संरचना utf8data	*दत्त;
	ध्रुव वर्ण	*s;
	ध्रुव वर्ण	*p;
	ध्रुव वर्ण	*ss;
	ध्रुव वर्ण	*sp;
	अचिह्नित पूर्णाङ्क	len;
	अचिह्नित पूर्णाङ्क	slen;
	ह्रस्व पूर्णाङ्क	ccc;
	ह्रस्व पूर्णाङ्क	nccc;
	अचिह्नित वर्ण	hangul[UTF8HANGULLEAF];
};

/*
 * Initialize a utf8cursor to normalize a string.
 * Returns 0 on success.
 * Returns -1 on failure.
 */
बाह्य पूर्णाङ्क utf8cursor(संरचना utf8cursor *u8c, ध्रुव संरचना utf8data *दत्त,
		      ध्रुव वर्ण *s);
बाह्य पूर्णाङ्क utf8ncursor(संरचना utf8cursor *u8c, ध्रुव संरचना utf8data *दत्त,
		       ध्रुव वर्ण *s, size_t len);

/*
 * Get the next byte in the normalization.
 * Returns a value > 0 && < 256 on success.
 * Returns 0 when the end of the normalization is reached.
 * Returns -1 if the string being normalized is not valid UTF-8.
 */
बाह्य पूर्णाङ्क utf8byte(संरचना utf8cursor *u8c);

#यदि_अन्तः /* UTF8NORM_H */
