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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <limits.h>
#अन्तर्भाव <errno.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

/* Encoding a unicode version number as a single unsigned int. */
#परिभाषय UNICODE_MAJ_सरणम्		(16)
#परिभाषय UNICODE_MIN_सरणम्		(8)

#परिभाषय UNICODE_AGE(MAJ, MIN, REV)			\
	(((अचिह्नित पूर्णाङ्क)(MAJ) << UNICODE_MAJ_सरणम्) |	\
	 ((अचिह्नित पूर्णाङ्क)(MIN) << UNICODE_MIN_सरणम्) |	\
	 ((अचिह्नित पूर्णाङ्क)(REV)))

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


संरचना utf8data {
	अचिह्नित पूर्णाङ्क maxage;
	अचिह्नित पूर्णाङ्क विस्थापनम्;
};

#परिभाषय __INCLUDED_FROM_UTF8NORM_C__
#अन्तर्भाव "utf8data.h"
#अपरिभाषय __INCLUDED_FROM_UTF8NORM_C__

#परिभाषय ARRAY_आकार(array)			\
        (परिमाण(array) / परिमाण(array[0]))

#यदि 0
/* Highest unicode version supported by the data tables. */
स्थिर पूर्णाङ्क utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev)
{
	पूर्णाङ्क i = ARRAY_आकार(utf8agetab) - 1;
	अचिह्नित पूर्णाङ्क sb_utf8version = UNICODE_AGE(maj, min, rev);

	यावत् (i >= 0 && utf8agetab[i] != 0) {
		यदि (sb_utf8version == utf8agetab[i])
			प्रत्यागम 1;
		i--;
	}
	प्रत्यागम 0;
}
#यदि_अन्तः

#यदि 0
स्थिर पूर्णाङ्क utf8version_latest(शून्य)
{
	प्रत्यागम utf8vers;
}
#यदि_अन्तः

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
स्थिर अन्तःस्थ पूर्णाङ्क utf8clen(ध्रुव वर्ण *s)
{
	अचिह्नित वर्ण c = *s;

	प्रत्यागम 1 + (c >= 0xC0) + (c >= 0xE0) + (c >= 0xF0);
}

/*
 * Decode a 3-byte UTF-8 sequence.
 */
स्थिर अचिह्नित पूर्णाङ्क
utf8decode3(ध्रुव वर्ण *str)
{
	अचिह्नित पूर्णाङ्क		uc;

	uc = *str++ & 0x0F;
	uc <<= 6;
	uc |= *str++ & 0x3F;
	uc <<= 6;
	uc |= *str++ & 0x3F;

	प्रत्यागम uc;
}

/*
 * Encode a 3-byte UTF-8 sequence.
 */
स्थिर पूर्णाङ्क
utf8encode3(वर्ण *str, अचिह्नित पूर्णाङ्क val)
{
	str[2] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[1] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[0] = val | 0xE0;

	प्रत्यागम 3;
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
प्रकारनिर्देश ध्रुव अचिह्नित वर्ण utf8trie_t;
#परिभाषय BITNUM		0x07
#परिभाषय NEXTBYTE	0x08
#परिभाषय OFFLEN		0x30
#परिभाषय OFFLEN_सरणम्	4
#परिभाषय RIGHTPATH	0x40
#परिभाषय TRIENODE	0x80
#परिभाषय RIGHTNODE	0x40
#परिभाषय LEFTNODE	0x80

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
प्रकारनिर्देश ध्रुव अचिह्नित वर्ण utf8leaf_t;

#परिभाषय पर्णम्_GEN(पर्णम्)	((पर्णम्)[0])
#परिभाषय पर्णम्_CCC(पर्णम्)	((पर्णम्)[1])
#परिभाषय पर्णम्_STR(पर्णम्)	((ध्रुव वर्ण *)((पर्णम्) + 2))

#परिभाषय MINCCC		(0)
#परिभाषय MAXCCC		(254)
#परिभाषय STOPPER		(0)
#परिभाषय	DECOMPOSE	(255)

/* Marker for hangul syllable decomposition. */
#परिभाषय HANGUL		((वर्ण)(255))
/* Size of the synthesized leaf used for Hangul syllable decomposition. */
#परिभाषय UTF8HANGULLEAF	(12)

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
#परिभाषय SB	(0xAC00)
#परिभाषय LB	(0x1100)
#परिभाषय VB	(0x1161)
#परिभाषय TB	(0x11A7)
#परिभाषय LC	(19)
#परिभाषय VC	(21)
#परिभाषय TC	(28)
#परिभाषय NC	(VC * TC)
#परिभाषय SC	(LC * NC)

/* Algorithmic decomposition of hangul syllable. */
स्थिर utf8leaf_t *
utf8hangul(ध्रुव वर्ण *str, अचिह्नित वर्ण *hangul)
{
	अचिह्नित पूर्णाङ्क	si;
	अचिह्नित पूर्णाङ्क	li;
	अचिह्नित पूर्णाङ्क	vi;
	अचिह्नित पूर्णाङ्क	ti;
	अचिह्नित वर्ण	*h;

	/* Calculate the SI, LI, VI, and TI values. */
	si = utf8decode3(str) - SB;
	li = si / NC;
	vi = (si % NC) / TC;
	ti = si % TC;

	/* Fill in base of leaf. */
	h = hangul;
	पर्णम्_GEN(h) = 2;
	पर्णम्_CCC(h) = DECOMPOSE;
	h += 2;

	/* Add LPart, a 3-byte UTF-8 sequence. */
	h += utf8encode3((वर्ण *)h, li + LB);

	/* Add VPart, a 3-byte UTF-8 sequence. */
	h += utf8encode3((वर्ण *)h, vi + VB);

	/* Add TPart if required, also a 3-byte UTF-8 sequence. */
	यदि (ti)
		h += utf8encode3((वर्ण *)h, ti + TB);

	/* Terminate string. */
	h[0] = '\0';

	प्रत्यागम hangul;
}

/*
 * Use trie to scan s, touching at most len bytes.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * A non-NULL return guarantees that the UTF-8 sequence starting at s
 * is well-formed and corresponds to a known unicode code point.  The
 * shorthand for this will be "is valid UTF-8 unicode".
 */
स्थिर utf8leaf_t *utf8nlookup(ध्रुव संरचना utf8data *दत्त,
			       अचिह्नित वर्ण *hangul, ध्रुव वर्ण *s, size_t len)
{
	utf8trie_t	*trie;
	पूर्णाङ्क		offlen;
	पूर्णाङ्क		विस्थापनम्;
	पूर्णाङ्क		आवरणम्;
	पूर्णाङ्क		ग्रन्थिः;

	यदि (!दत्त)
		प्रत्यागम रिक्त;
	यदि (len == 0)
		प्रत्यागम रिक्त;

	trie = utf8data + दत्त->विस्थापनम्;
	ग्रन्थिः = 1;
	यावत् (ग्रन्थिः) {
		offlen = (*trie & OFFLEN) >> OFFLEN_सरणम्;
		यदि (*trie & NEXTBYTE) {
			यदि (--len == 0)
				प्रत्यागम रिक्त;
			s++;
		}
		आवरणम् = 1 << (*trie & BITNUM);
		यदि (*s & आवरणम्) {
			/* Right leg */
			यदि (offlen) {
				/* Right node at offset of trie */
				ग्रन्थिः = (*trie & RIGHTNODE);
				विस्थापनम् = trie[offlen];
				यावत् (--offlen) {
					विस्थापनम् <<= 8;
					विस्थापनम् |= trie[offlen];
				}
				trie += विस्थापनम्;
			} अन्यथा यदि (*trie & RIGHTPATH) {
				/* Right node after this node */
				ग्रन्थिः = (*trie & TRIENODE);
				trie++;
			} अन्यथा {
				/* No right node. */
				प्रत्यागम रिक्त;
			}
		} अन्यथा {
			/* Left leg */
			यदि (offlen) {
				/* Left node after this node. */
				ग्रन्थिः = (*trie & LEFTNODE);
				trie += offlen + 1;
			} अन्यथा यदि (*trie & RIGHTPATH) {
				/* No left node. */
				प्रत्यागम रिक्त;
			} अन्यथा {
				/* Left node after this node */
				ग्रन्थिः = (*trie & TRIENODE);
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
	यदि (पर्णम्_CCC(trie) == DECOMPOSE && पर्णम्_STR(trie)[0] == HANGUL)
		trie = utf8hangul(s - 2, hangul);
	प्रत्यागम trie;
}

/*
 * Use trie to scan s.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * Forwards to utf8nlookup().
 */
स्थिर utf8leaf_t *utf8lookup(ध्रुव संरचना utf8data *दत्त,
			      अचिह्नित वर्ण *hangul, ध्रुव वर्ण *s)
{
	प्रत्यागम utf8nlookup(दत्त, hangul, s, (size_t)-1);
}

#यदि 0
/*
 * Maximum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if only non-assigned code points are used.
 */
स्थिर पूर्णाङ्क utf8agemax(ध्रुव संरचना utf8data *दत्त, ध्रुव वर्ण *s)
{
	utf8leaf_t	*पर्णम्_2;
	पूर्णाङ्क		age = 0;
	पूर्णाङ्क		पर्णम्_age;
	अचिह्नित वर्ण	hangul[UTF8HANGULLEAF];

	यदि (!दत्त)
		प्रत्यागम -1;

	यावत् (*s) {
		पर्णम्_2 = utf8lookup(दत्त, hangul, s);
		यदि (!पर्णम्_2)
			प्रत्यागम -1;

		पर्णम्_age = utf8agetab[पर्णम्_GEN(पर्णम्_2)];
		यदि (पर्णम्_age <= दत्त->maxage && पर्णम्_age > age)
			age = पर्णम्_age;
		s += utf8clen(s);
	}
	प्रत्यागम age;
}
#यदि_अन्तः

#यदि 0
/*
 * Minimum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if non-assigned code points are used.
 */
स्थिर पूर्णाङ्क utf8agemin(ध्रुव संरचना utf8data *दत्त, ध्रुव वर्ण *s)
{
	utf8leaf_t	*पर्णम्_2;
	पूर्णाङ्क		age;
	पूर्णाङ्क		पर्णम्_age;
	अचिह्नित वर्ण	hangul[UTF8HANGULLEAF];

	यदि (!दत्त)
		प्रत्यागम -1;
	age = दत्त->maxage;
	यावत् (*s) {
		पर्णम्_2 = utf8lookup(दत्त, hangul, s);
		यदि (!पर्णम्_2)
			प्रत्यागम -1;
		पर्णम्_age = utf8agetab[पर्णम्_GEN(पर्णम्_2)];
		यदि (पर्णम्_age <= दत्त->maxage && पर्णम्_age < age)
			age = पर्णम्_age;
		s += utf8clen(s);
	}
	प्रत्यागम age;
}
#यदि_अन्तः

#यदि 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
स्थिर पूर्णाङ्क utf8nagemax(ध्रुव संरचना utf8data *दत्त, ध्रुव वर्ण *s, size_t len)
{
	utf8leaf_t	*पर्णम्_2;
	पूर्णाङ्क		age = 0;
	पूर्णाङ्क		पर्णम्_age;
	अचिह्नित वर्ण	hangul[UTF8HANGULLEAF];

	यदि (!दत्त)
		प्रत्यागम -1;

	यावत् (len && *s) {
		पर्णम्_2 = utf8nlookup(दत्त, hangul, s, len);
		यदि (!पर्णम्_2)
			प्रत्यागम -1;
		पर्णम्_age = utf8agetab[पर्णम्_GEN(पर्णम्_2)];
		यदि (पर्णम्_age <= दत्त->maxage && पर्णम्_age > age)
			age = पर्णम्_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	प्रत्यागम age;
}
#यदि_अन्तः

#यदि 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
स्थिर पूर्णाङ्क utf8nagemin(ध्रुव संरचना utf8data *दत्त, ध्रुव वर्ण *s, size_t len)
{
	utf8leaf_t	*पर्णम्_2;
	पूर्णाङ्क		पर्णम्_age;
	पूर्णाङ्क		age;
	अचिह्नित वर्ण	hangul[UTF8HANGULLEAF];

	यदि (!दत्त)
		प्रत्यागम -1;
	age = दत्त->maxage;
	यावत् (len && *s) {
		पर्णम्_2 = utf8nlookup(दत्त, hangul, s, len);
		यदि (!पर्णम्_2)
			प्रत्यागम -1;
		पर्णम्_age = utf8agetab[पर्णम्_GEN(पर्णम्_2)];
		यदि (पर्णम्_age <= दत्त->maxage && पर्णम्_age < age)
			age = पर्णम्_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	प्रत्यागम age;
}
#यदि_अन्तः

#यदि 0
/*
 * Length of the normalization of s.
 * Return -1 if s is not valid UTF-8 unicode.
 *
 * A string of Default_Ignorable_Code_Point has length 0.
 */
स्थिर ssize_t utf8len(ध्रुव संरचना utf8data *दत्त, ध्रुव वर्ण *s)
{
	utf8leaf_t	*पर्णम्_2;
	size_t		ret = 0;
	अचिह्नित वर्ण	hangul[UTF8HANGULLEAF];

	यदि (!दत्त)
		प्रत्यागम -1;
	यावत् (*s) {
		पर्णम्_2 = utf8lookup(दत्त, hangul, s);
		यदि (!पर्णम्_2)
			प्रत्यागम -1;
		यदि (utf8agetab[पर्णम्_GEN(पर्णम्_2)] > दत्त->maxage)
			ret += utf8clen(s);
		अन्यथा यदि (पर्णम्_CCC(पर्णम्_2) == DECOMPOSE)
			ret += वाक्यदैर्घ्यम्(पर्णम्_STR(पर्णम्_2));
		अन्यथा
			ret += utf8clen(s);
		s += utf8clen(s);
	}
	प्रत्यागम ret;
}
#यदि_अन्तः

#यदि 0
/*
 * Length of the normalization of s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
स्थिर ssize_t utf8nlen(ध्रुव संरचना utf8data *दत्त, ध्रुव वर्ण *s, size_t len)
{
	utf8leaf_t	*पर्णम्_2;
	size_t		ret = 0;
	अचिह्नित वर्ण	hangul[UTF8HANGULLEAF];

	यदि (!दत्त)
		प्रत्यागम -1;
	यावत् (len && *s) {
		पर्णम्_2 = utf8nlookup(दत्त, hangul, s, len);
		यदि (!पर्णम्_2)
			प्रत्यागम -1;
		यदि (utf8agetab[पर्णम्_GEN(पर्णम्_2)] > दत्त->maxage)
			ret += utf8clen(s);
		अन्यथा यदि (पर्णम्_CCC(पर्णम्_2) == DECOMPOSE)
			ret += वाक्यदैर्घ्यम्(पर्णम्_STR(पर्णम्_2));
		अन्यथा
			ret += utf8clen(s);
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	प्रत्यागम ret;
}
#यदि_अन्तः

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
स्थिर पूर्णाङ्क utf8ncursor(संरचना utf8cursor *u8c, ध्रुव संरचना utf8data *दत्त,
		ध्रुव वर्ण *s, size_t len)
{
	यदि (!दत्त)
		प्रत्यागम -1;
	यदि (!s)
		प्रत्यागम -1;
	u8c->दत्त = दत्त;
	u8c->s = s;
	u8c->p = रिक्त;
	u8c->ss = रिक्त;
	u8c->sp = रिक्त;
	u8c->len = len;
	u8c->slen = 0;
	u8c->ccc = STOPPER;
	u8c->nccc = STOPPER;
	/* Check we didn't clobber the maximum length. */
	यदि (u8c->len != len)
		प्रत्यागम -1;
	/* The first byte of s may not be an utf8 continuation. */
	यदि (len > 0 && (*s & 0xC0) == 0x80)
		प्रत्यागम -1;
	प्रत्यागम 0;
}

#यदि 0
/*
 * Set up an utf8cursor for use by utf8byte().
 *
 *   u8c    : pointer to cursor.
 *   data   : const struct utf8data to use for normalization.
 *   s      : NUL-terminated string.
 *
 * Returns -1 on error, 0 on success.
 */
स्थिर पूर्णाङ्क utf8cursor(संरचना utf8cursor *u8c, ध्रुव संरचना utf8data *दत्त,
	       ध्रुव वर्ण *s)
{
	प्रत्यागम utf8ncursor(u8c, दत्त, s, (अचिह्नित पूर्णाङ्क)-1);
}
#यदि_अन्तः

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
स्थिर पूर्णाङ्क utf8byte(संरचना utf8cursor *u8c)
{
	utf8leaf_t *पर्णम्_2;
	पूर्णाङ्क ccc;

	प्रति (;;) {
		/* Check for the end of a decomposed character. */
		यदि (u8c->p && *u8c->s == '\0') {
			u8c->s = u8c->p;
			u8c->p = रिक्त;
		}

		/* Check for end-of-string. */
		यदि (!u8c->p && (u8c->len == 0 || *u8c->s == '\0')) {
			/* There is no next byte. */
			यदि (u8c->ccc == STOPPER)
				प्रत्यागम 0;
			/* End-of-string during a scan counts as a stopper. */
			ccc = STOPPER;
			गच्छ ccc_mismatch;
		} अन्यथा यदि ((*u8c->s & 0xC0) == 0x80) {
			/* This is a continuation of the current character. */
			यदि (!u8c->p)
				u8c->len--;
			प्रत्यागम (अचिह्नित वर्ण)*u8c->s++;
		}

		/* Look up the data for the current character. */
		यदि (u8c->p) {
			पर्णम्_2 = utf8lookup(u8c->दत्त, u8c->hangul, u8c->s);
		} अन्यथा {
			पर्णम्_2 = utf8nlookup(u8c->दत्त, u8c->hangul,
					   u8c->s, u8c->len);
		}

		/* No leaf found implies that the input is a binary blob. */
		यदि (!पर्णम्_2)
			प्रत्यागम -1;

		ccc = पर्णम्_CCC(पर्णम्_2);
		/* Characters that are too new have CCC 0. */
		यदि (utf8agetab[पर्णम्_GEN(पर्णम्_2)] > u8c->दत्त->maxage) {
			ccc = STOPPER;
		} अन्यथा यदि (ccc == DECOMPOSE) {
			u8c->len -= utf8clen(u8c->s);
			u8c->p = u8c->s + utf8clen(u8c->s);
			u8c->s = पर्णम्_STR(पर्णम्_2);
			/* Empty decomposition implies CCC 0. */
			यदि (*u8c->s == '\0') {
				यदि (u8c->ccc == STOPPER)
					अनुवर्त;
				ccc = STOPPER;
				गच्छ ccc_mismatch;
			}

			पर्णम्_2 = utf8lookup(u8c->दत्त, u8c->hangul, u8c->s);
			यदि (!पर्णम्_2)
				प्रत्यागम -1;
			ccc = पर्णम्_CCC(पर्णम्_2);
		}

		/*
		 * If this is not a stopper, then see if it updates
		 * the next canonical class to be emitted.
		 */
		यदि (ccc != STOPPER && u8c->ccc < ccc && ccc < u8c->nccc)
			u8c->nccc = ccc;

		/*
		 * Return the current byte if this is the current
		 * combining class.
		 */
		यदि (ccc == u8c->ccc) {
			यदि (!u8c->p)
				u8c->len--;
			प्रत्यागम (अचिह्नित वर्ण)*u8c->s++;
		}

		/* Current combining class mismatch. */
ccc_mismatch:
		यदि (u8c->nccc == STOPPER) {
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
			यदि (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} अन्यथा यदि (ccc != STOPPER) {
			/* Not a stopper, and not the ccc we're emitting. */
			यदि (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} अन्यथा यदि (u8c->nccc != MAXCCC + 1) {
			/* At a stopper, restart for next ccc. */
			u8c->ccc = u8c->nccc;
			u8c->nccc = MAXCCC + 1;
			u8c->s = u8c->ss;
			u8c->p = u8c->sp;
			u8c->len = u8c->slen;
		} अन्यथा {
			/* All done, proceed from here. */
			u8c->ccc = STOPPER;
			u8c->nccc = STOPPER;
			u8c->sp = रिक्त;
			u8c->ss = रिक्त;
			u8c->slen = 0;
		}
	}
}

#यदि 0
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
स्थिर ध्रुव संरचना utf8data *utf8nfdi(अचिह्नित पूर्णाङ्क maxage)
{
	पूर्णाङ्क i = ARRAY_आकार(utf8nfdidata) - 1;

	यावत् (maxage < utf8nfdidata[i].maxage)
		i--;
	यदि (maxage > utf8nfdidata[i].maxage)
		प्रत्यागम रिक्त;
	प्रत्यागम &utf8nfdidata[i];
}
#यदि_अन्तः

स्थिर ध्रुव संरचना utf8data *utf8nfdicf(अचिह्नित पूर्णाङ्क maxage)
{
	पूर्णाङ्क i = ARRAY_आकार(utf8nfdicfdata) - 1;

	यावत् (maxage < utf8nfdicfdata[i].maxage)
		i--;
	यदि (maxage > utf8nfdicfdata[i].maxage)
		प्रत्यागम रिक्त;
	प्रत्यागम &utf8nfdicfdata[i];
}

स्थिर पूर्णाङ्क utf8_casefold(ध्रुव संरचना ext2fs_nls_सारणी *सारणी,
			  ध्रुव अचिह्नित वर्ण *str, size_t len,
			  अचिह्नित वर्ण *dest, size_t dlen)
{
	ध्रुव संरचना utf8data *दत्त = utf8nfdicf(सारणी->संस्करणम्);
	संरचना utf8cursor cur;
	size_t nlen = 0;

	यदि (utf8ncursor(&cur, दत्त, (ध्रुव वर्ण *) str, len) < 0)
		गच्छ invalid_seq;

	प्रति (nlen = 0; nlen < dlen; nlen++) {
		पूर्णाङ्क c = utf8byte(&cur);

		dest[nlen] = c;
		यदि (!c)
			प्रत्यागम nlen;
		यदि (c == -1)
			भङ्ग;
	}

	प्रत्यागम -ENAMETOOLONG;

invalid_seq:
	यदि (dlen < len)
		प्रत्यागम -ENAMETOOLONG;

	/* Signal invalid sequence */
	प्रत्यागम -EINVAL;
}

स्थिर पूर्णाङ्क utf8_validate(ध्रुव संरचना ext2fs_nls_सारणी *सारणी,
			 वर्ण *s, size_t len, वर्ण **pos)
{
	ध्रुव संरचना utf8data *दत्त = utf8nfdicf(सारणी->संस्करणम्);
	utf8leaf_t	*पर्णम्_2;
	अचिह्नित वर्ण	hangul[UTF8HANGULLEAF];

	यदि (!दत्त)
		प्रत्यागम -1;
	यावत् (len && *s) {
		पर्णम्_2 = utf8nlookup(दत्त, hangul, s, len);
		यदि (!पर्णम्_2) {
			*pos = s;
			प्रत्यागम 1;
		}
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क utf8_casefold_cmp(ध्रुव संरचना ext2fs_nls_सारणी *सारणी,
			     ध्रुव अचिह्नित वर्ण *str1, size_t len1,
			     ध्रुव अचिह्नित वर्ण *str2, size_t len2)
{
	ध्रुव संरचना utf8data *दत्त = utf8nfdicf(सारणी->संस्करणम्);
	पूर्णाङ्क c1, c2;
	संरचना utf8cursor cur1, cur2;

	यदि (utf8ncursor(&cur1, दत्त, (ध्रुव वर्ण *) str1, len1) < 0)
		प्रत्यागम -1;
	यदि (utf8ncursor(&cur2, दत्त, (ध्रुव वर्ण *) str2, len2) < 0)
		प्रत्यागम -1;

	कुरु {
		c1 = utf8byte(&cur1);
		c2 = utf8byte(&cur2);

		यदि (c1 < 0 || c2 < 0)
			प्रत्यागम -1;
		यदि (c1 != c2)
			प्रत्यागम c1 - c2;
	} यावत् (c1);

	प्रत्यागम 0;
}

स्थिर ध्रुव संरचना ext2fs_nls_ops utf8_ops = {
	.casefold = utf8_casefold,
	.validate = utf8_validate,
	.casefold_cmp = utf8_casefold_cmp,
};

स्थिर ध्रुव संरचना ext2fs_nls_सारणी nls_utf8 = {
	.ops = &utf8_ops,
	.संस्करणम् = UNICODE_AGE(12, 1, 0),
};

ध्रुव संरचना ext2fs_nls_सारणी *ext2fs_आरोपय_nls_सारणी(पूर्णाङ्क encoding)
{
	यदि (encoding == EXT4_ENC_UTF8_12_1)
		प्रत्यागम &nls_utf8;

	प्रत्यागम रिक्त;
}

पूर्णाङ्क ext2fs_check_encoded_name(ध्रुव संरचना ext2fs_nls_सारणी *सारणी,
			      वर्ण *name, size_t len, वर्ण **pos)
{
	प्रत्यागम सारणी->ops->validate(सारणी, name, len, pos);
}

पूर्णाङ्क ext2fs_casefold_cmp(ध्रुव संरचना ext2fs_nls_सारणी *सारणी,
			ध्रुव अचिह्नित वर्ण *str1, size_t len1,
			ध्रुव अचिह्नित वर्ण *str2, size_t len2)
{
	प्रत्यागम सारणी->ops->casefold_cmp(सारणी, str1, len1, str2, len2);
}
