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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ UTF8NORM_H
#ವ್ಯಾಖ್ಯಾನಿಸು UTF8NORM_H

#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <string.h>

/* Encoding a unicode version number as a single unsigned int. */
#ವ್ಯಾಖ್ಯಾನಿಸು UNICODE_MAJ_ಸರಿಸು		(16)
#ವ್ಯಾಖ್ಯಾನಿಸು UNICODE_MIN_ಸರಿಸು		(8)

#ವ್ಯಾಖ್ಯಾನಿಸು UNICODE_AGE(MAJ, MIN, REV)			\
	(((ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)(MAJ) << UNICODE_MAJ_ಸರಿಸು) |	\
	 ((ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)(MIN) << UNICODE_MIN_ಸರಿಸು) |	\
	 ((ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)(REV)))

/* Highest unicode version supported by the data tables. */
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ utf8version_latest(ಶೂನ್ಯ);

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
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *utf8nfdi(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ maxage);
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *utf8nfdicf(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ maxage);

/*
 * Determine the maximum age of any unicode character in the string.
 * Returns 0 if only unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ utf8agemax(ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *ದತ್ತಾಂಶ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ utf8nagemax(ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *ದತ್ತಾಂಶ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, size_t len);

/*
 * Determine the minimum age of any unicode character in the string.
 * Returns 0 if any unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ utf8agemin(ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *ದತ್ತಾಂಶ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ utf8nagemin(ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *ದತ್ತಾಂಶ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, size_t len);

/*
 * Determine the length of the normalized from of the string,
 * excluding any terminating NULL byte.
 * Returns 0 if only ignorable code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
ಬಾಹ್ಯ ssize_t utf8len(ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *ದತ್ತಾಂಶ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s);
ಬಾಹ್ಯ ssize_t utf8nlen(ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *ದತ್ತಾಂಶ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, size_t len);

/* Needed in struct utf8cursor below. */
#ವ್ಯಾಖ್ಯಾನಿಸು UTF8HANGULLEAF	(12)

/*
 * Cursor structure used by the normalizer.
 */
ರಚನೆ utf8cursor {
	ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data	*ದತ್ತಾಂಶ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ	*s;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ	*p;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ	*ss;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ	*sp;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	len;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	slen;
	ಗಿಡ್ಡ ಪೂರ್ಣಾಂಕ	ccc;
	ಗಿಡ್ಡ ಪೂರ್ಣಾಂಕ	nccc;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	hangul[UTF8HANGULLEAF];
};

/*
 * Initialize a utf8cursor to normalize a string.
 * Returns 0 on success.
 * Returns -1 on failure.
 */
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ utf8cursor(ರಚನೆ utf8cursor *u8c, ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *ದತ್ತಾಂಶ,
		      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ utf8ncursor(ರಚನೆ utf8cursor *u8c, ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *ದತ್ತಾಂಶ,
		       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, size_t len);

/*
 * Get the next byte in the normalization.
 * Returns a value > 0 && < 256 on success.
 * Returns 0 when the end of the normalization is reached.
 * Returns -1 if the string being normalized is not valid UTF-8.
 */
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ utf8byte(ರಚನೆ utf8cursor *u8c);

#ಆದರೆ_ಅಂತ್ಯ /* UTF8NORM_H */
