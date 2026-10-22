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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ UTF8NORM_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UTF8NORM_H

#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

/* Encoding a unicode version number as a single unsigned int. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UNICODE_MAJ_ਖਿਸਕਾਓ		(16)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UNICODE_MIN_ਖਿਸਕਾਓ		(8)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UNICODE_AGE(MAJ, MIN, REV)			\
	(((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)(MAJ) << UNICODE_MAJ_ਖਿਸਕਾਓ) |	\
	 ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)(MIN) << UNICODE_MIN_ਖਿਸਕਾਓ) |	\
	 ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)(REV)))

/* Highest unicode version supported by the data tables. */
ਬਾਹਰੀ ਪੂਰਨ utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev);
ਬਾਹਰੀ ਪੂਰਨ utf8version_latest(ਖਾਲੀ);

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
ਬਾਹਰੀ ਅਚਲ ਬਣਤਰ utf8data *utf8nfdi(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ maxage);
ਬਾਹਰੀ ਅਚਲ ਬਣਤਰ utf8data *utf8nfdicf(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ maxage);

/*
 * Determine the maximum age of any unicode character in the string.
 * Returns 0 if only unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
ਬਾਹਰੀ ਪੂਰਨ utf8agemax(ਅਚਲ ਬਣਤਰ utf8data *ਅੰਕੜੇ, ਅਚਲ ਅੱਖਰ *s);
ਬਾਹਰੀ ਪੂਰਨ utf8nagemax(ਅਚਲ ਬਣਤਰ utf8data *ਅੰਕੜੇ, ਅਚਲ ਅੱਖਰ *s, size_t len);

/*
 * Determine the minimum age of any unicode character in the string.
 * Returns 0 if any unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
ਬਾਹਰੀ ਪੂਰਨ utf8agemin(ਅਚਲ ਬਣਤਰ utf8data *ਅੰਕੜੇ, ਅਚਲ ਅੱਖਰ *s);
ਬਾਹਰੀ ਪੂਰਨ utf8nagemin(ਅਚਲ ਬਣਤਰ utf8data *ਅੰਕੜੇ, ਅਚਲ ਅੱਖਰ *s, size_t len);

/*
 * Determine the length of the normalized from of the string,
 * excluding any terminating NULL byte.
 * Returns 0 if only ignorable code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
ਬਾਹਰੀ ssize_t utf8len(ਅਚਲ ਬਣਤਰ utf8data *ਅੰਕੜੇ, ਅਚਲ ਅੱਖਰ *s);
ਬਾਹਰੀ ssize_t utf8nlen(ਅਚਲ ਬਣਤਰ utf8data *ਅੰਕੜੇ, ਅਚਲ ਅੱਖਰ *s, size_t len);

/* Needed in struct utf8cursor below. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UTF8HANGULLEAF	(12)

/*
 * Cursor structure used by the normalizer.
 */
ਬਣਤਰ utf8cursor {
	ਅਚਲ ਬਣਤਰ utf8data	*ਅੰਕੜੇ;
	ਅਚਲ ਅੱਖਰ	*s;
	ਅਚਲ ਅੱਖਰ	*p;
	ਅਚਲ ਅੱਖਰ	*ss;
	ਅਚਲ ਅੱਖਰ	*sp;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	len;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	slen;
	ਛੋਟਾ ਪੂਰਨ	ccc;
	ਛੋਟਾ ਪੂਰਨ	nccc;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	hangul[UTF8HANGULLEAF];
};

/*
 * Initialize a utf8cursor to normalize a string.
 * Returns 0 on success.
 * Returns -1 on failure.
 */
ਬਾਹਰੀ ਪੂਰਨ utf8cursor(ਬਣਤਰ utf8cursor *u8c, ਅਚਲ ਬਣਤਰ utf8data *ਅੰਕੜੇ,
		      ਅਚਲ ਅੱਖਰ *s);
ਬਾਹਰੀ ਪੂਰਨ utf8ncursor(ਬਣਤਰ utf8cursor *u8c, ਅਚਲ ਬਣਤਰ utf8data *ਅੰਕੜੇ,
		       ਅਚਲ ਅੱਖਰ *s, size_t len);

/*
 * Get the next byte in the normalization.
 * Returns a value > 0 && < 256 on success.
 * Returns 0 when the end of the normalization is reached.
 * Returns -1 if the string being normalized is not valid UTF-8.
 */
ਬਾਹਰੀ ਪੂਰਨ utf8byte(ਬਣਤਰ utf8cursor *u8c);

#ਜੇ_ਅੰਤ /* UTF8NORM_H */
