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

#నిర్వచితం_కాకపోతే UTF8NORM_H
#నిర్వచించు UTF8NORM_H

#అన్తర్భావ <stdint.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <string.h>

/* Encoding a unicode version number as a single unsigned int. */
#నిర్వచించు UNICODE_MAJ_జరుపు		(16)
#నిర్వచించు UNICODE_MIN_జరుపు		(8)

#నిర్వచించు UNICODE_AGE(MAJ, MIN, REV)			\
	(((గుర్తులేని పూర్ణాంకం)(MAJ) << UNICODE_MAJ_జరుపు) |	\
	 ((గుర్తులేని పూర్ణాంకం)(MIN) << UNICODE_MIN_జరుపు) |	\
	 ((గుర్తులేని పూర్ణాంకం)(REV)))

/* Highest unicode version supported by the data tables. */
బాహ్య పూర్ణాంకం utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev);
బాహ్య పూర్ణాంకం utf8version_latest(శూన్య);

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
బాహ్య స్థిరరాశి నిర్మాణం utf8data *utf8nfdi(గుర్తులేని పూర్ణాంకం maxage);
బాహ్య స్థిరరాశి నిర్మాణం utf8data *utf8nfdicf(గుర్తులేని పూర్ణాంకం maxage);

/*
 * Determine the maximum age of any unicode character in the string.
 * Returns 0 if only unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
బాహ్య పూర్ణాంకం utf8agemax(స్థిరరాశి నిర్మాణం utf8data *దత్తాంశం, స్థిరరాశి అక్షరం *s);
బాహ్య పూర్ణాంకం utf8nagemax(స్థిరరాశి నిర్మాణం utf8data *దత్తాంశం, స్థిరరాశి అక్షరం *s, size_t len);

/*
 * Determine the minimum age of any unicode character in the string.
 * Returns 0 if any unassigned code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
బాహ్య పూర్ణాంకం utf8agemin(స్థిరరాశి నిర్మాణం utf8data *దత్తాంశం, స్థిరరాశి అక్షరం *s);
బాహ్య పూర్ణాంకం utf8nagemin(స్థిరరాశి నిర్మాణం utf8data *దత్తాంశం, స్థిరరాశి అక్షరం *s, size_t len);

/*
 * Determine the length of the normalized from of the string,
 * excluding any terminating NULL byte.
 * Returns 0 if only ignorable code points are present.
 * Returns -1 if the input is not valid UTF-8.
 */
బాహ్య ssize_t utf8len(స్థిరరాశి నిర్మాణం utf8data *దత్తాంశం, స్థిరరాశి అక్షరం *s);
బాహ్య ssize_t utf8nlen(స్థిరరాశి నిర్మాణం utf8data *దత్తాంశం, స్థిరరాశి అక్షరం *s, size_t len);

/* Needed in struct utf8cursor below. */
#నిర్వచించు UTF8HANGULLEAF	(12)

/*
 * Cursor structure used by the normalizer.
 */
నిర్మాణం utf8cursor {
	స్థిరరాశి నిర్మాణం utf8data	*దత్తాంశం;
	స్థిరరాశి అక్షరం	*s;
	స్థిరరాశి అక్షరం	*p;
	స్థిరరాశి అక్షరం	*ss;
	స్థిరరాశి అక్షరం	*sp;
	గుర్తులేని పూర్ణాంకం	len;
	గుర్తులేని పూర్ణాంకం	slen;
	పొట్టి పూర్ణాంకం	ccc;
	పొట్టి పూర్ణాంకం	nccc;
	గుర్తులేని అక్షరం	hangul[UTF8HANGULLEAF];
};

/*
 * Initialize a utf8cursor to normalize a string.
 * Returns 0 on success.
 * Returns -1 on failure.
 */
బాహ్య పూర్ణాంకం utf8cursor(నిర్మాణం utf8cursor *u8c, స్థిరరాశి నిర్మాణం utf8data *దత్తాంశం,
		      స్థిరరాశి అక్షరం *s);
బాహ్య పూర్ణాంకం utf8ncursor(నిర్మాణం utf8cursor *u8c, స్థిరరాశి నిర్మాణం utf8data *దత్తాంశం,
		       స్థిరరాశి అక్షరం *s, size_t len);

/*
 * Get the next byte in the normalization.
 * Returns a value > 0 && < 256 on success.
 * Returns 0 when the end of the normalization is reached.
 * Returns -1 if the string being normalized is not valid UTF-8.
 */
బాహ్య పూర్ణాంకం utf8byte(నిర్మాణం utf8cursor *u8c);

#అయితే_ముగింపు /* UTF8NORM_H */
