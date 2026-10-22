/*
 * Copyright (c) 2014 SGI.
 * Copyright (c) 2018 Collabora Ltd.
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

/*
 * This code is adapted from the Linux Kernel.  We have a
 * userspace version here such that the hashes will match that
 * implementation.
 */

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdint.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <limits.h>
#అన్తర్భావ <errno.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "ext2fsP.h"

/* Encoding a unicode version number as a single unsigned int. */
#నిర్వచించు UNICODE_MAJ_జరుపు		(16)
#నిర్వచించు UNICODE_MIN_జరుపు		(8)

#నిర్వచించు UNICODE_AGE(MAJ, MIN, REV)			\
	(((గుర్తులేని పూర్ణాంకం)(MAJ) << UNICODE_MAJ_జరుపు) |	\
	 ((గుర్తులేని పూర్ణాంకం)(MIN) << UNICODE_MIN_జరుపు) |	\
	 ((గుర్తులేని పూర్ణాంకం)(REV)))

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
// extern int utf8cursor(struct utf8cursor *u8c, const struct utf8data *data,
//		      const char *s);
// extern int utf8ncursor(struct utf8cursor *u8c, const struct utf8data *data,
//		       const char *s, size_t len);

/*
 * Get the next byte in the normalization.
 * Returns a value > 0 && < 256 on success.
 * Returns 0 when the end of the normalization is reached.
 * Returns -1 if the string being normalized is not valid UTF-8.
 */
// extern int utf8byte(struct utf8cursor *u8c);


నిర్మాణం utf8data {
	గుర్తులేని పూర్ణాంకం maxage;
	గుర్తులేని పూర్ణాంకం స్థానభ్రంశం;
};

#నిర్వచించు __INCLUDED_FROM_UTF8NORM_C__
#అన్తర్భావ "utf8data.h"
#నిర్వచనం_రద్దు __INCLUDED_FROM_UTF8NORM_C__

#నిర్వచించు ARRAY_కొలత(array)			\
        (పరిమాణం(array) / పరిమాణం(array[0]))

#ఒకవేళ 0
/* Highest unicode version supported by the data tables. */
స్థిర పూర్ణాంకం utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev)
{
	పూర్ణాంకం i = ARRAY_కొలత(utf8agetab) - 1;
	గుర్తులేని పూర్ణాంకం sb_utf8version = UNICODE_AGE(maj, min, rev);

	అయితే (i >= 0 && utf8agetab[i] != 0) {
		ఒకవేళ (sb_utf8version == utf8agetab[i])
			తిరిగి 1;
		i--;
	}
	తిరిగి 0;
}
#అయితే_ముగింపు

#ఒకవేళ 0
స్థిర పూర్ణాంకం utf8version_latest(శూన్య)
{
	తిరిగి utf8vers;
}
#అయితే_ముగింపు

/*
 * UTF-8 valid ranges.
 *
 * The UTF-8 encoding spreads the bits of a 32bit word over several
 * bytes. This table gives the ranges that can be held and how they'd
 * be represented.
 *
 * 0x00000000 0x0000007F: 0xxxxxxx
 * 0x00000000 0x000007FF: 110xxxxx 10xxxxxx
 * 0x00000000 0x0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx
 * 0x00000000 0x001FFFFF: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 * 0x00000000 0x03FFFFFF: 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 * 0x00000000 0x7FFFFFFF: 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 *
 * There is an additional requirement on UTF-8, in that only the
 * shortest representation of a 32bit value is to be used.  A decoder
 * must not decode sequences that do not satisfy this requirement.
 * Thus the allowed ranges have a lower bound.
 *
 * 0x00000000 0x0000007F: 0xxxxxxx
 * 0x00000080 0x000007FF: 110xxxxx 10xxxxxx
 * 0x00000800 0x0000FFFF: 1110xxxx 10xxxxxx 10xxxxxx
 * 0x00010000 0x001FFFFF: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 * 0x00200000 0x03FFFFFF: 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 * 0x04000000 0x7FFFFFFF: 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 *
 * Actual unicode characters are limited to the range 0x0 - 0x10FFFF,
 * 17 planes of 65536 values.  This limits the sequences actually seen
 * even more, to just the following.
 *
 *          0 -     0x7F: 0                   - 0x7F
 *       0x80 -    0x7FF: 0xC2 0x80           - 0xDF 0xBF
 *      0x800 -   0xFFFF: 0xE0 0xA0 0x80      - 0xEF 0xBF 0xBF
 *    0x10000 - 0x10FFFF: 0xF0 0x90 0x80 0x80 - 0xF4 0x8F 0xBF 0xBF
 *
 * Within those ranges the surrogates 0xD800 - 0xDFFF are not allowed.
 *
 * Note that the longest sequence seen with valid usage is 4 bytes,
 * the same a single UTF-32 character.  This makes the UTF-8
 * representation of Unicode strictly smaller than UTF-32.
 *
 * The shortest sequence requirement was introduced by:
 *    Corrigendum #1: UTF-8 Shortest Form
 * It can be found here:
 *    http://www.unicode.org/versions/corrigendum1.html
 *
 */

/*
 * Return the number of bytes used by the current UTF-8 sequence.
 * Assumes the input points to the first byte of a valid UTF-8
 * sequence.
 */
స్థిర అంతఃవరుస పూర్ణాంకం utf8clen(స్థిరరాశి అక్షరం *s)
{
	గుర్తులేని అక్షరం c = *s;

	తిరిగి 1 + (c >= 0xC0) + (c >= 0xE0) + (c >= 0xF0);
}

/*
 * Decode a 3-byte UTF-8 sequence.
 */
స్థిర గుర్తులేని పూర్ణాంకం
utf8decode3(స్థిరరాశి అక్షరం *str)
{
	గుర్తులేని పూర్ణాంకం		uc;

	uc = *str++ & 0x0F;
	uc <<= 6;
	uc |= *str++ & 0x3F;
	uc <<= 6;
	uc |= *str++ & 0x3F;

	తిరిగి uc;
}

/*
 * Encode a 3-byte UTF-8 sequence.
 */
స్థిర పూర్ణాంకం
utf8encode3(అక్షరం *str, గుర్తులేని పూర్ణాంకం val)
{
	str[2] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[1] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[0] = val | 0xE0;

	తిరిగి 3;
}

/*
 * utf8trie_t
 *
 * A compact binary tree, used to decode UTF-8 characters.
 *
 * Internal nodes are one byte for the node itself, and up to three
 * bytes for an offset into the tree.  The first byte contains the
 * following information:
 *  NEXTBYTE  - flag        - advance to next byte if set
 *  BITNUM    - 3 bit field - the bit number to tested
 *  OFFLEN    - 2 bit field - number of bytes in the offset
 * if offlen == 0 (non-branching node)
 *  RIGHTPATH - 1 bit field - set if the following node is for the
 *                            right-hand path (tested bit is set)
 *  TRIENODE  - 1 bit field - set if the following node is an internal
 *                            node, otherwise it is a leaf node
 * if offlen != 0 (branching node)
 *  LEFTNODE  - 1 bit field - set if the left-hand node is internal
 *  RIGHTNODE - 1 bit field - set if the right-hand node is internal
 *
 * Due to the way utf8 works, there cannot be branching nodes with
 * NEXTBYTE set, and moreover those nodes always have a righthand
 * descendant.
 */
రకనిర్వచనం స్థిరరాశి గుర్తులేని అక్షరం utf8trie_t;
#నిర్వచించు BITNUM		0x07
#నిర్వచించు NEXTBYTE	0x08
#నిర్వచించు OFFLEN		0x30
#నిర్వచించు OFFLEN_జరుపు	4
#నిర్వచించు RIGHTPATH	0x40
#నిర్వచించు TRIENODE	0x80
#నిర్వచించు RIGHTNODE	0x40
#నిర్వచించు LEFTNODE	0x80

/*
 * utf8leaf_t
 *
 * The leaves of the trie are embedded in the trie, and so the same
 * underlying datatype: unsigned char.
 *
 * leaf[0]: The unicode version, stored as a generation number that is
 *          an index into utf8agetab[].  With this we can filter code
 *          points based on the unicode version in which they were
 *          defined.  The CCC of a non-defined code point is 0.
 * leaf[1]: Canonical Combining Class. During normalization, we need
 *          to do a stable sort into ascending order of all characters
 *          with a non-zero CCC that occur between two characters with
 *          a CCC of 0, or at the begin or end of a string.
 *          The unicode standard guarantees that all CCC values are
 *          between 0 and 254 inclusive, which leaves 255 available as
 *          a special value.
 *          Code points with CCC 0 are known as stoppers.
 * leaf[2]: Decomposition. If leaf[1] == 255, then leaf[2] is the
 *          start of a NUL-terminated string that is the decomposition
 *          of the character.
 *          The CCC of a decomposable character is the same as the CCC
 *          of the first character of its decomposition.
 *          Some characters decompose as the empty string: these are
 *          characters with the Default_Ignorable_Code_Point property.
 *          These do affect normalization, as they all have CCC 0.
 *
 * The decompositions in the trie have been fully expanded, with the
 * exception of Hangul syllables, which are decomposed algorithmically.
 *
 * Casefolding, if applicable, is also done using decompositions.
 *
 * The trie is constructed in such a way that leaves exist for all
 * UTF-8 sequences that match the criteria from the "UTF-8 valid
 * ranges" comment above, and only for those sequences.  Therefore a
 * lookup in the trie can be used to validate the UTF-8 input.
 */
రకనిర్వచనం స్థిరరాశి గుర్తులేని అక్షరం utf8leaf_t;

#నిర్వచించు ఆకు_GEN(ఆకు)	((ఆకు)[0])
#నిర్వచించు ఆకు_CCC(ఆకు)	((ఆకు)[1])
#నిర్వచించు ఆకు_STR(ఆకు)	((స్థిరరాశి అక్షరం *)((ఆకు) + 2))

#నిర్వచించు MINCCC		(0)
#నిర్వచించు MAXCCC		(254)
#నిర్వచించు STOPPER		(0)
#నిర్వచించు	DECOMPOSE	(255)

/* Marker for hangul syllable decomposition. */
#నిర్వచించు HANGUL		((అక్షరం)(255))
/* Size of the synthesized leaf used for Hangul syllable decomposition. */
#నిర్వచించు UTF8HANGULLEAF	(12)

/*
 * Hangul decomposition (algorithm from Section 3.12 of Unicode 6.3.0)
 *
 * AC00;<Hangul Syllable, First>;Lo;0;L;;;;;N;;;;;
 * D7A3;<Hangul Syllable, Last>;Lo;0;L;;;;;N;;;;;
 *
 * SBase = 0xAC00
 * LBase = 0x1100
 * VBase = 0x1161
 * TBase = 0x11A7
 * LCount = 19
 * VCount = 21
 * TCount = 28
 * NCount = 588 (VCount * TCount)
 * SCount = 11172 (LCount * NCount)
 *
 * Decomposition:
 *   SIndex = s - SBase
 *
 * LV (Canonical/Full)
 *   LIndex = SIndex / NCount
 *   VIndex = (Sindex % NCount) / TCount
 *   LPart = LBase + LIndex
 *   VPart = VBase + VIndex
 *
 * LVT (Canonical)
 *   LVIndex = (SIndex / TCount) * TCount
 *   TIndex = (Sindex % TCount)
 *   LVPart = SBase + LVIndex
 *   TPart = TBase + TIndex
 *
 * LVT (Full)
 *   LIndex = SIndex / NCount
 *   VIndex = (Sindex % NCount) / TCount
 *   TIndex = (Sindex % TCount)
 *   LPart = LBase + LIndex
 *   VPart = VBase + VIndex
 *   if (TIndex == 0) {
 *          d = <LPart, VPart>
 *   } else {
 *          TPart = TBase + TIndex
 *          d = <LPart, TPart, VPart>
 *   }
 */

/* Constants */
#నిర్వచించు SB	(0xAC00)
#నిర్వచించు LB	(0x1100)
#నిర్వచించు VB	(0x1161)
#నిర్వచించు TB	(0x11A7)
#నిర్వచించు LC	(19)
#నిర్వచించు VC	(21)
#నిర్వచించు TC	(28)
#నిర్వచించు NC	(VC * TC)
#నిర్వచించు SC	(LC * NC)

/* Algorithmic decomposition of hangul syllable. */
స్థిర utf8leaf_t *
utf8hangul(స్థిరరాశి అక్షరం *str, గుర్తులేని అక్షరం *hangul)
{
	గుర్తులేని పూర్ణాంకం	si;
	గుర్తులేని పూర్ణాంకం	li;
	గుర్తులేని పూర్ణాంకం	vi;
	గుర్తులేని పూర్ణాంకం	ti;
	గుర్తులేని అక్షరం	*h;

	/* Calculate the SI, LI, VI, and TI values. */
	si = utf8decode3(str) - SB;
	li = si / NC;
	vi = (si % NC) / TC;
	ti = si % TC;

	/* Fill in base of leaf. */
	h = hangul;
	ఆకు_GEN(h) = 2;
	ఆకు_CCC(h) = DECOMPOSE;
	h += 2;

	/* Add LPart, a 3-byte UTF-8 sequence. */
	h += utf8encode3((అక్షరం *)h, li + LB);

	/* Add VPart, a 3-byte UTF-8 sequence. */
	h += utf8encode3((అక్షరం *)h, vi + VB);

	/* Add TPart if required, also a 3-byte UTF-8 sequence. */
	ఒకవేళ (ti)
		h += utf8encode3((అక్షరం *)h, ti + TB);

	/* Terminate string. */
	h[0] = '\0';

	తిరిగి hangul;
}

/*
 * Use trie to scan s, touching at most len bytes.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * A non-NULL return guarantees that the UTF-8 sequence starting at s
 * is well-formed and corresponds to a known unicode code point.  The
 * shorthand for this will be "is valid UTF-8 unicode".
 */
స్థిర utf8leaf_t *utf8nlookup(స్థిరరాశి నిర్మాణం utf8data *దత్తాంశం,
			       గుర్తులేని అక్షరం *hangul, స్థిరరాశి అక్షరం *s, size_t len)
{
	utf8trie_t	*trie;
	పూర్ణాంకం		offlen;
	పూర్ణాంకం		స్థానభ్రంశం;
	పూర్ణాంకం		ముసుగు;
	పూర్ణాంకం		కణుపు;

	ఒకవేళ (!దత్తాంశం)
		తిరిగి ఖాళీ;
	ఒకవేళ (len == 0)
		తిరిగి ఖాళీ;

	trie = utf8data + దత్తాంశం->స్థానభ్రంశం;
	కణుపు = 1;
	అయితే (కణుపు) {
		offlen = (*trie & OFFLEN) >> OFFLEN_జరుపు;
		ఒకవేళ (*trie & NEXTBYTE) {
			ఒకవేళ (--len == 0)
				తిరిగి ఖాళీ;
			s++;
		}
		ముసుగు = 1 << (*trie & BITNUM);
		ఒకవేళ (*s & ముసుగు) {
			/* Right leg */
			ఒకవేళ (offlen) {
				/* Right node at offset of trie */
				కణుపు = (*trie & RIGHTNODE);
				స్థానభ్రంశం = trie[offlen];
				అయితే (--offlen) {
					స్థానభ్రంశం <<= 8;
					స్థానభ్రంశం |= trie[offlen];
				}
				trie += స్థానభ్రంశం;
			} లేకపోతే ఒకవేళ (*trie & RIGHTPATH) {
				/* Right node after this node */
				కణుపు = (*trie & TRIENODE);
				trie++;
			} లేకపోతే {
				/* No right node. */
				తిరిగి ఖాళీ;
			}
		} లేకపోతే {
			/* Left leg */
			ఒకవేళ (offlen) {
				/* Left node after this node. */
				కణుపు = (*trie & LEFTNODE);
				trie += offlen + 1;
			} లేకపోతే ఒకవేళ (*trie & RIGHTPATH) {
				/* No left node. */
				తిరిగి ఖాళీ;
			} లేకపోతే {
				/* Left node after this node */
				కణుపు = (*trie & TRIENODE);
				trie++;
			}
		}
	}
	/*
	 * Hangul decomposition is done algorithmically. These are the
	 * codepoints >= 0xAC00 and <= 0xD7A3. Their UTF-8 encoding is
	 * always 3 bytes long, so s has been advanced twice, and the
	 * start of the sequence is at s-2.
	 */
	ఒకవేళ (ఆకు_CCC(trie) == DECOMPOSE && ఆకు_STR(trie)[0] == HANGUL)
		trie = utf8hangul(s - 2, hangul);
	తిరిగి trie;
}

/*
 * Use trie to scan s.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * Forwards to utf8nlookup().
 */
స్థిర utf8leaf_t *utf8lookup(స్థిరరాశి నిర్మాణం utf8data *దత్తాంశం,
			      గుర్తులేని అక్షరం *hangul, స్థిరరాశి అక్షరం *s)
{
	తిరిగి utf8nlookup(దత్తాంశం, hangul, s, (size_t)-1);
}

#ఒకవేళ 0
/*
 * Maximum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if only non-assigned code points are used.
 */
స్థిర పూర్ణాంకం utf8agemax(స్థిరరాశి నిర్మాణం utf8data *దత్తాంశం, స్థిరరాశి అక్షరం *s)
{
	utf8leaf_t	*ఆకు_2;
	పూర్ణాంకం		age = 0;
	పూర్ణాంకం		ఆకు_age;
	గుర్తులేని అక్షరం	hangul[UTF8HANGULLEAF];

	ఒకవేళ (!దత్తాంశం)
		తిరిగి -1;

	అయితే (*s) {
		ఆకు_2 = utf8lookup(దత్తాంశం, hangul, s);
		ఒకవేళ (!ఆకు_2)
			తిరిగి -1;

		ఆకు_age = utf8agetab[ఆకు_GEN(ఆకు_2)];
		ఒకవేళ (ఆకు_age <= దత్తాంశం->maxage && ఆకు_age > age)
			age = ఆకు_age;
		s += utf8clen(s);
	}
	తిరిగి age;
}
#అయితే_ముగింపు

#ఒకవేళ 0
/*
 * Minimum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if non-assigned code points are used.
 */
స్థిర పూర్ణాంకం utf8agemin(స్థిరరాశి నిర్మాణం utf8data *దత్తాంశం, స్థిరరాశి అక్షరం *s)
{
	utf8leaf_t	*ఆకు_2;
	పూర్ణాంకం		age;
	పూర్ణాంకం		ఆకు_age;
	గుర్తులేని అక్షరం	hangul[UTF8HANGULLEAF];

	ఒకవేళ (!దత్తాంశం)
		తిరిగి -1;
	age = దత్తాంశం->maxage;
	అయితే (*s) {
		ఆకు_2 = utf8lookup(దత్తాంశం, hangul, s);
		ఒకవేళ (!ఆకు_2)
			తిరిగి -1;
		ఆకు_age = utf8agetab[ఆకు_GEN(ఆకు_2)];
		ఒకవేళ (ఆకు_age <= దత్తాంశం->maxage && ఆకు_age < age)
			age = ఆకు_age;
		s += utf8clen(s);
	}
	తిరిగి age;
}
#అయితే_ముగింపు

#ఒకవేళ 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
స్థిర పూర్ణాంకం utf8nagemax(స్థిరరాశి నిర్మాణం utf8data *దత్తాంశం, స్థిరరాశి అక్షరం *s, size_t len)
{
	utf8leaf_t	*ఆకు_2;
	పూర్ణాంకం		age = 0;
	పూర్ణాంకం		ఆకు_age;
	గుర్తులేని అక్షరం	hangul[UTF8HANGULLEAF];

	ఒకవేళ (!దత్తాంశం)
		తిరిగి -1;

	అయితే (len && *s) {
		ఆకు_2 = utf8nlookup(దత్తాంశం, hangul, s, len);
		ఒకవేళ (!ఆకు_2)
			తిరిగి -1;
		ఆకు_age = utf8agetab[ఆకు_GEN(ఆకు_2)];
		ఒకవేళ (ఆకు_age <= దత్తాంశం->maxage && ఆకు_age > age)
			age = ఆకు_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	తిరిగి age;
}
#అయితే_ముగింపు

#ఒకవేళ 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
స్థిర పూర్ణాంకం utf8nagemin(స్థిరరాశి నిర్మాణం utf8data *దత్తాంశం, స్థిరరాశి అక్షరం *s, size_t len)
{
	utf8leaf_t	*ఆకు_2;
	పూర్ణాంకం		ఆకు_age;
	పూర్ణాంకం		age;
	గుర్తులేని అక్షరం	hangul[UTF8HANGULLEAF];

	ఒకవేళ (!దత్తాంశం)
		తిరిగి -1;
	age = దత్తాంశం->maxage;
	అయితే (len && *s) {
		ఆకు_2 = utf8nlookup(దత్తాంశం, hangul, s, len);
		ఒకవేళ (!ఆకు_2)
			తిరిగి -1;
		ఆకు_age = utf8agetab[ఆకు_GEN(ఆకు_2)];
		ఒకవేళ (ఆకు_age <= దత్తాంశం->maxage && ఆకు_age < age)
			age = ఆకు_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	తిరిగి age;
}
#అయితే_ముగింపు

#ఒకవేళ 0
/*
 * Length of the normalization of s.
 * Return -1 if s is not valid UTF-8 unicode.
 *
 * A string of Default_Ignorable_Code_Point has length 0.
 */
స్థిర ssize_t utf8len(స్థిరరాశి నిర్మాణం utf8data *దత్తాంశం, స్థిరరాశి అక్షరం *s)
{
	utf8leaf_t	*ఆకు_2;
	size_t		ret = 0;
	గుర్తులేని అక్షరం	hangul[UTF8HANGULLEAF];

	ఒకవేళ (!దత్తాంశం)
		తిరిగి -1;
	అయితే (*s) {
		ఆకు_2 = utf8lookup(దత్తాంశం, hangul, s);
		ఒకవేళ (!ఆకు_2)
			తిరిగి -1;
		ఒకవేళ (utf8agetab[ఆకు_GEN(ఆకు_2)] > దత్తాంశం->maxage)
			ret += utf8clen(s);
		లేకపోతే ఒకవేళ (ఆకు_CCC(ఆకు_2) == DECOMPOSE)
			ret += అక్షరశ్రేణి_పొడవు(ఆకు_STR(ఆకు_2));
		లేకపోతే
			ret += utf8clen(s);
		s += utf8clen(s);
	}
	తిరిగి ret;
}
#అయితే_ముగింపు

#ఒకవేళ 0
/*
 * Length of the normalization of s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
స్థిర ssize_t utf8nlen(స్థిరరాశి నిర్మాణం utf8data *దత్తాంశం, స్థిరరాశి అక్షరం *s, size_t len)
{
	utf8leaf_t	*ఆకు_2;
	size_t		ret = 0;
	గుర్తులేని అక్షరం	hangul[UTF8HANGULLEAF];

	ఒకవేళ (!దత్తాంశం)
		తిరిగి -1;
	అయితే (len && *s) {
		ఆకు_2 = utf8nlookup(దత్తాంశం, hangul, s, len);
		ఒకవేళ (!ఆకు_2)
			తిరిగి -1;
		ఒకవేళ (utf8agetab[ఆకు_GEN(ఆకు_2)] > దత్తాంశం->maxage)
			ret += utf8clen(s);
		లేకపోతే ఒకవేళ (ఆకు_CCC(ఆకు_2) == DECOMPOSE)
			ret += అక్షరశ్రేణి_పొడవు(ఆకు_STR(ఆకు_2));
		లేకపోతే
			ret += utf8clen(s);
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	తిరిగి ret;
}
#అయితే_ముగింపు

/*
 * Set up an utf8cursor for use by utf8byte().
 *
 *   u8c    : pointer to cursor.
 *   data   : const struct utf8data to use for normalization.
 *   s      : string.
 *   len    : length of s.
 *
 * Returns -1 on error, 0 on success.
 */
స్థిర పూర్ణాంకం utf8ncursor(నిర్మాణం utf8cursor *u8c, స్థిరరాశి నిర్మాణం utf8data *దత్తాంశం,
		స్థిరరాశి అక్షరం *s, size_t len)
{
	ఒకవేళ (!దత్తాంశం)
		తిరిగి -1;
	ఒకవేళ (!s)
		తిరిగి -1;
	u8c->దత్తాంశం = దత్తాంశం;
	u8c->s = s;
	u8c->p = ఖాళీ;
	u8c->ss = ఖాళీ;
	u8c->sp = ఖాళీ;
	u8c->len = len;
	u8c->slen = 0;
	u8c->ccc = STOPPER;
	u8c->nccc = STOPPER;
	/* Check we didn't clobber the maximum length. */
	ఒకవేళ (u8c->len != len)
		తిరిగి -1;
	/* The first byte of s may not be an utf8 continuation. */
	ఒకవేళ (len > 0 && (*s & 0xC0) == 0x80)
		తిరిగి -1;
	తిరిగి 0;
}

#ఒకవేళ 0
/*
 * Set up an utf8cursor for use by utf8byte().
 *
 *   u8c    : pointer to cursor.
 *   data   : const struct utf8data to use for normalization.
 *   s      : NUL-terminated string.
 *
 * Returns -1 on error, 0 on success.
 */
స్థిర పూర్ణాంకం utf8cursor(నిర్మాణం utf8cursor *u8c, స్థిరరాశి నిర్మాణం utf8data *దత్తాంశం,
	       స్థిరరాశి అక్షరం *s)
{
	తిరిగి utf8ncursor(u8c, దత్తాంశం, s, (గుర్తులేని పూర్ణాంకం)-1);
}
#అయితే_ముగింపు

/*
 * Get one byte from the normalized form of the string described by u8c.
 *
 * Returns the byte cast to an unsigned char on success, and -1 on failure.
 *
 * The cursor keeps track of the location in the string in u8c->s.
 * When a character is decomposed, the current location is stored in
 * u8c->p, and u8c->s is set to the start of the decomposition. Note
 * that bytes from a decomposition do not count against u8c->len.
 *
 * Characters are emitted if they match the current CCC in u8c->ccc.
 * Hitting end-of-string while u8c->ccc == STOPPER means we're done,
 * and the function returns 0 in that case.
 *
 * Sorting by CCC is done by repeatedly scanning the string.  The
 * values of u8c->s and u8c->p are stored in u8c->ss and u8c->sp at
 * the start of the scan.  The first pass finds the lowest CCC to be
 * emitted and stores it in u8c->nccc, the second pass emits the
 * characters with this CCC and finds the next lowest CCC. This limits
 * the number of passes to 1 + the number of different CCCs in the
 * sequence being scanned.
 *
 * Therefore:
 *  u8c->p  != NULL -> a decomposition is being scanned.
 *  u8c->ss != NULL -> this is a repeating scan.
 *  u8c->ccc == -1   -> this is the first scan of a repeating scan.
 */
స్థిర పూర్ణాంకం utf8byte(నిర్మాణం utf8cursor *u8c)
{
	utf8leaf_t *ఆకు_2;
	పూర్ణాంకం ccc;

	కొరకు (;;) {
		/* Check for the end of a decomposed character. */
		ఒకవేళ (u8c->p && *u8c->s == '\0') {
			u8c->s = u8c->p;
			u8c->p = ఖాళీ;
		}

		/* Check for end-of-string. */
		ఒకవేళ (!u8c->p && (u8c->len == 0 || *u8c->s == '\0')) {
			/* There is no next byte. */
			ఒకవేళ (u8c->ccc == STOPPER)
				తిరిగి 0;
			/* End-of-string during a scan counts as a stopper. */
			ccc = STOPPER;
			వెళ్ళు ccc_mismatch;
		} లేకపోతే ఒకవేళ ((*u8c->s & 0xC0) == 0x80) {
			/* This is a continuation of the current character. */
			ఒకవేళ (!u8c->p)
				u8c->len--;
			తిరిగి (గుర్తులేని అక్షరం)*u8c->s++;
		}

		/* Look up the data for the current character. */
		ఒకవేళ (u8c->p) {
			ఆకు_2 = utf8lookup(u8c->దత్తాంశం, u8c->hangul, u8c->s);
		} లేకపోతే {
			ఆకు_2 = utf8nlookup(u8c->దత్తాంశం, u8c->hangul,
					   u8c->s, u8c->len);
		}

		/* No leaf found implies that the input is a binary blob. */
		ఒకవేళ (!ఆకు_2)
			తిరిగి -1;

		ccc = ఆకు_CCC(ఆకు_2);
		/* Characters that are too new have CCC 0. */
		ఒకవేళ (utf8agetab[ఆకు_GEN(ఆకు_2)] > u8c->దత్తాంశం->maxage) {
			ccc = STOPPER;
		} లేకపోతే ఒకవేళ (ccc == DECOMPOSE) {
			u8c->len -= utf8clen(u8c->s);
			u8c->p = u8c->s + utf8clen(u8c->s);
			u8c->s = ఆకు_STR(ఆకు_2);
			/* Empty decomposition implies CCC 0. */
			ఒకవేళ (*u8c->s == '\0') {
				ఒకవేళ (u8c->ccc == STOPPER)
					కొనసాగు;
				ccc = STOPPER;
				వెళ్ళు ccc_mismatch;
			}

			ఆకు_2 = utf8lookup(u8c->దత్తాంశం, u8c->hangul, u8c->s);
			ఒకవేళ (!ఆకు_2)
				తిరిగి -1;
			ccc = ఆకు_CCC(ఆకు_2);
		}

		/*
		 * If this is not a stopper, then see if it updates
		 * the next canonical class to be emitted.
		 */
		ఒకవేళ (ccc != STOPPER && u8c->ccc < ccc && ccc < u8c->nccc)
			u8c->nccc = ccc;

		/*
		 * Return the current byte if this is the current
		 * combining class.
		 */
		ఒకవేళ (ccc == u8c->ccc) {
			ఒకవేళ (!u8c->p)
				u8c->len--;
			తిరిగి (గుర్తులేని అక్షరం)*u8c->s++;
		}

		/* Current combining class mismatch. */
ccc_mismatch:
		ఒకవేళ (u8c->nccc == STOPPER) {
			/*
			 * Scan forward for the first canonical class
			 * to be emitted.  Save the position from
			 * which to restart.
			 */
			u8c->ccc = MINCCC - 1;
			u8c->nccc = ccc;
			u8c->sp = u8c->p;
			u8c->ss = u8c->s;
			u8c->slen = u8c->len;
			ఒకవేళ (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} లేకపోతే ఒకవేళ (ccc != STOPPER) {
			/* Not a stopper, and not the ccc we're emitting. */
			ఒకవేళ (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} లేకపోతే ఒకవేళ (u8c->nccc != MAXCCC + 1) {
			/* At a stopper, restart for next ccc. */
			u8c->ccc = u8c->nccc;
			u8c->nccc = MAXCCC + 1;
			u8c->s = u8c->ss;
			u8c->p = u8c->sp;
			u8c->len = u8c->slen;
		} లేకపోతే {
			/* All done, proceed from here. */
			u8c->ccc = STOPPER;
			u8c->nccc = STOPPER;
			u8c->sp = ఖాళీ;
			u8c->ss = ఖాళీ;
			u8c->slen = 0;
		}
	}
}

#ఒకవేళ 0
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
స్థిర స్థిరరాశి నిర్మాణం utf8data *utf8nfdi(గుర్తులేని పూర్ణాంకం maxage)
{
	పూర్ణాంకం i = ARRAY_కొలత(utf8nfdidata) - 1;

	అయితే (maxage < utf8nfdidata[i].maxage)
		i--;
	ఒకవేళ (maxage > utf8nfdidata[i].maxage)
		తిరిగి ఖాళీ;
	తిరిగి &utf8nfdidata[i];
}
#అయితే_ముగింపు

స్థిర స్థిరరాశి నిర్మాణం utf8data *utf8nfdicf(గుర్తులేని పూర్ణాంకం maxage)
{
	పూర్ణాంకం i = ARRAY_కొలత(utf8nfdicfdata) - 1;

	అయితే (maxage < utf8nfdicfdata[i].maxage)
		i--;
	ఒకవేళ (maxage > utf8nfdicfdata[i].maxage)
		తిరిగి ఖాళీ;
	తిరిగి &utf8nfdicfdata[i];
}

స్థిర పూర్ణాంకం utf8_casefold(స్థిరరాశి నిర్మాణం ext2fs_nls_పట్టిక *పట్టిక,
			  స్థిరరాశి గుర్తులేని అక్షరం *str, size_t len,
			  గుర్తులేని అక్షరం *dest, size_t dlen)
{
	స్థిరరాశి నిర్మాణం utf8data *దత్తాంశం = utf8nfdicf(పట్టిక->సంస్కరణ);
	నిర్మాణం utf8cursor cur;
	size_t nlen = 0;

	ఒకవేళ (utf8ncursor(&cur, దత్తాంశం, (స్థిరరాశి అక్షరం *) str, len) < 0)
		వెళ్ళు invalid_seq;

	కొరకు (nlen = 0; nlen < dlen; nlen++) {
		పూర్ణాంకం c = utf8byte(&cur);

		dest[nlen] = c;
		ఒకవేళ (!c)
			తిరిగి nlen;
		ఒకవేళ (c == -1)
			ఆపు;
	}

	తిరిగి -ENAMETOOLONG;

invalid_seq:
	ఒకవేళ (dlen < len)
		తిరిగి -ENAMETOOLONG;

	/* Signal invalid sequence */
	తిరిగి -EINVAL;
}

స్థిర పూర్ణాంకం utf8_validate(స్థిరరాశి నిర్మాణం ext2fs_nls_పట్టిక *పట్టిక,
			 అక్షరం *s, size_t len, అక్షరం **pos)
{
	స్థిరరాశి నిర్మాణం utf8data *దత్తాంశం = utf8nfdicf(పట్టిక->సంస్కరణ);
	utf8leaf_t	*ఆకు_2;
	గుర్తులేని అక్షరం	hangul[UTF8HANGULLEAF];

	ఒకవేళ (!దత్తాంశం)
		తిరిగి -1;
	అయితే (len && *s) {
		ఆకు_2 = utf8nlookup(దత్తాంశం, hangul, s, len);
		ఒకవేళ (!ఆకు_2) {
			*pos = s;
			తిరిగి 1;
		}
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	తిరిగి 0;
}

స్థిర పూర్ణాంకం utf8_casefold_cmp(స్థిరరాశి నిర్మాణం ext2fs_nls_పట్టిక *పట్టిక,
			     స్థిరరాశి గుర్తులేని అక్షరం *str1, size_t len1,
			     స్థిరరాశి గుర్తులేని అక్షరం *str2, size_t len2)
{
	స్థిరరాశి నిర్మాణం utf8data *దత్తాంశం = utf8nfdicf(పట్టిక->సంస్కరణ);
	పూర్ణాంకం c1, c2;
	నిర్మాణం utf8cursor cur1, cur2;

	ఒకవేళ (utf8ncursor(&cur1, దత్తాంశం, (స్థిరరాశి అక్షరం *) str1, len1) < 0)
		తిరిగి -1;
	ఒకవేళ (utf8ncursor(&cur2, దత్తాంశం, (స్థిరరాశి అక్షరం *) str2, len2) < 0)
		తిరిగి -1;

	చేయి {
		c1 = utf8byte(&cur1);
		c2 = utf8byte(&cur2);

		ఒకవేళ (c1 < 0 || c2 < 0)
			తిరిగి -1;
		ఒకవేళ (c1 != c2)
			తిరిగి c1 - c2;
	} అయితే (c1);

	తిరిగి 0;
}

స్థిర స్థిరరాశి నిర్మాణం ext2fs_nls_ops utf8_ops = {
	.casefold = utf8_casefold,
	.validate = utf8_validate,
	.casefold_cmp = utf8_casefold_cmp,
};

స్థిర స్థిరరాశి నిర్మాణం ext2fs_nls_పట్టిక nls_utf8 = {
	.ops = &utf8_ops,
	.సంస్కరణ = UNICODE_AGE(12, 1, 0),
};

స్థిరరాశి నిర్మాణం ext2fs_nls_పట్టిక *ext2fs_ఎక్కించు_nls_పట్టిక(పూర్ణాంకం encoding)
{
	ఒకవేళ (encoding == EXT4_ENC_UTF8_12_1)
		తిరిగి &nls_utf8;

	తిరిగి ఖాళీ;
}

పూర్ణాంకం ext2fs_check_encoded_name(స్థిరరాశి నిర్మాణం ext2fs_nls_పట్టిక *పట్టిక,
			      అక్షరం *name, size_t len, అక్షరం **pos)
{
	తిరిగి పట్టిక->ops->validate(పట్టిక, name, len, pos);
}

పూర్ణాంకం ext2fs_casefold_cmp(స్థిరరాశి నిర్మాణం ext2fs_nls_పట్టిక *పట్టిక,
			స్థిరరాశి గుర్తులేని అక్షరం *str1, size_t len1,
			స్థిరరాశి గుర్తులేని అక్షరం *str2, size_t len2)
{
	తిరిగి పట్టిక->ops->casefold_cmp(పట్టిక, str1, len1, str2, len2);
}
