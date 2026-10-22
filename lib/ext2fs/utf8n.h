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

#अगर_अघोषित UTF8NORM_H
#घोषणा UTF8NORM_H

#समावेश <stdint.h>
#समावेश <unistd.h>
#समावेश <string.h>

/* Encoding a unicode version number as a single unsigned int. */
#घोषणा UNICODE_MAJ_SHIFT		(16)
#घोषणा UNICODE_MIN_SHIFT		(8)

#घोषणा UNICODE_AGE(MAJ, MIN, REV)			\
	(((अचिन्हित पूर्णांक)(MAJ) << UNICODE_MAJ_SHIFT) |	\
	 ((अचिन्हित पूर्णांक)(MIN) << UNICODE_MIN_SHIFT) |	\
	 ((अचिन्हित पूर्णांक)(REV)))

/* Highest unicode version supported by the data tables. */
बाह्य पूर्णांक utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev);
बाह्य पूर्णांक utf8version_latest(व्योम);

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
बाह्य स्थिर काष्ठा utf8data *utf8nfdi(अचिन्हित पूर्णांक maxage);
बाह्य स्थिर काष्ठा utf8data *utf8nfdicf(अचिन्हित पूर्णांक maxage);

/*
 * Determine the maximum age of any unicode character in the string.
 * Returns 0 if only unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
बाह्य पूर्णांक utf8agemax(स्थिर काष्ठा utf8data *तथ्य, स्थिर अक्षर *s);
बाह्य पूर्णांक utf8nagemax(स्थिर काष्ठा utf8data *तथ्य, स्थिर अक्षर *s, माप_प्रकार len);

/*
 * Determine the minimum age of any unicode character in the string.
 * Returns 0 if any unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
बाह्य पूर्णांक utf8agemin(स्थिर काष्ठा utf8data *तथ्य, स्थिर अक्षर *s);
बाह्य पूर्णांक utf8nagemin(स्थिर काष्ठा utf8data *तथ्य, स्थिर अक्षर *s, माप_प्रकार len);

/*
 * Determine the length of the normalized from of the string,
 * excluding any terminating NULL byte.
 * Returns 0 if only ignorable code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
बाह्य ssize_t utf8len(स्थिर काष्ठा utf8data *तथ्य, स्थिर अक्षर *s);
बाह्य ssize_t utf8nlen(स्थिर काष्ठा utf8data *तथ्य, स्थिर अक्षर *s, माप_प्रकार len);

/* Needed in struct utf8cursor below. */
#घोषणा UTF8HANGULLEAF	(12)

/*
 * Cursor structure used by the normalizer.
 */
काष्ठा utf8cursor {
	स्थिर काष्ठा utf8data	*तथ्य;
	स्थिर अक्षर	*s;
	स्थिर अक्षर	*p;
	स्थिर अक्षर	*ss;
	स्थिर अक्षर	*sp;
	अचिन्हित पूर्णांक	len;
	अचिन्हित पूर्णांक	slen;
	लघु पूर्णांक	ccc;
	लघु पूर्णांक	nccc;
	अचिन्हित अक्षर	hangul[UTF8HANGULLEAF];
};

/*
 * Initialize a utf8cursor to normalize a string.
 * Returns 0 on success.
 * Returns -1 on failure.
 */
बाह्य पूर्णांक utf8cursor(काष्ठा utf8cursor *u8c, स्थिर काष्ठा utf8data *तथ्य,
		      स्थिर अक्षर *s);
बाह्य पूर्णांक utf8ncursor(काष्ठा utf8cursor *u8c, स्थिर काष्ठा utf8data *तथ्य,
		       स्थिर अक्षर *s, माप_प्रकार len);

/*
 * Get the next byte in the normalization.
 * Returns a value > 0 && < 256 on success.
 * Returns 0 when the end of the normalization is reached.
 * Returns -1 if the string being normalized is not valid UTF-8.
 */
बाह्य पूर्णांक utf8byte(काष्ठा utf8cursor *u8c);

#पूर्ण_अगर /* UTF8NORM_H */
