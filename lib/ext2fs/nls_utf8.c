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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <errno.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

/* Encoding a unicode version number as a single unsigned int. */
#ವ್ಯಾಖ್ಯಾನಿಸು UNICODE_MAJ_ಸರಿಸು		(16)
#ವ್ಯಾಖ್ಯಾನಿಸು UNICODE_MIN_ಸರಿಸು		(8)

#ವ್ಯಾಖ್ಯಾನಿಸು UNICODE_AGE(MAJ, MIN, REV)			\
	(((ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)(MAJ) << UNICODE_MAJ_ಸರಿಸು) |	\
	 ((ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)(MIN) << UNICODE_MIN_ಸರಿಸು) |	\
	 ((ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)(REV)))

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


ರಚನೆ utf8data {
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ maxage;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ;
};

#ವ್ಯಾಖ್ಯಾನಿಸು __INCLUDED_FROM_UTF8NORM_C__
#ಅನ್ತರ್ಭಾವ "utf8data.h"
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು __INCLUDED_FROM_UTF8NORM_C__

#ವ್ಯಾಖ್ಯಾನಿಸು ARRAY_ಗಾತ್ರ(array)			\
        (ಗಾತ್ರ(array) / ಗಾತ್ರ(array[0]))

#ಆದರೆ 0
/* Highest unicode version supported by the data tables. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev)
{
	ಪೂರ್ಣಾಂಕ i = ARRAY_ಗಾತ್ರ(utf8agetab) - 1;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sb_utf8version = UNICODE_AGE(maj, min, rev);

	ಆಗ (i >= 0 && utf8agetab[i] != 0) {
		ಆದರೆ (sb_utf8version == utf8agetab[i])
			ಹಿಂತಿರುಗಿ 1;
		i--;
	}
	ಹಿಂತಿರುಗಿ 0;
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ 0
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ utf8version_latest(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ utf8vers;
}
#ಆದರೆ_ಅಂತ್ಯ

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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ utf8clen(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c = *s;

	ಹಿಂತಿರುಗಿ 1 + (c >= 0xC0) + (c >= 0xE0) + (c >= 0xF0);
}

/*
 * Decode a 3-byte UTF-8 sequence.
 */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ
utf8decode3(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		uc;

	uc = *str++ & 0x0F;
	uc <<= 6;
	uc |= *str++ & 0x3F;
	uc <<= 6;
	uc |= *str++ & 0x3F;

	ಹಿಂತಿರುಗಿ uc;
}

/*
 * Encode a 3-byte UTF-8 sequence.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
utf8encode3(ಅಕ್ಷರ *str, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ val)
{
	str[2] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[1] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[0] = val | 0xE0;

	ಹಿಂತಿರುಗಿ 3;
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
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ utf8trie_t;
#ವ್ಯಾಖ್ಯಾನಿಸು BITNUM		0x07
#ವ್ಯಾಖ್ಯಾನಿಸು NEXTBYTE	0x08
#ವ್ಯಾಖ್ಯಾನಿಸು OFFLEN		0x30
#ವ್ಯಾಖ್ಯಾನಿಸು OFFLEN_ಸರಿಸು	4
#ವ್ಯಾಖ್ಯಾನಿಸು RIGHTPATH	0x40
#ವ್ಯಾಖ್ಯಾನಿಸು TRIENODE	0x80
#ವ್ಯಾಖ್ಯಾನಿಸು RIGHTNODE	0x40
#ವ್ಯಾಖ್ಯಾನಿಸು LEFTNODE	0x80

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
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ utf8leaf_t;

#ವ್ಯಾಖ್ಯಾನಿಸು ಎಲೆ_GEN(ಎಲೆ)	((ಎಲೆ)[0])
#ವ್ಯಾಖ್ಯಾನಿಸು ಎಲೆ_CCC(ಎಲೆ)	((ಎಲೆ)[1])
#ವ್ಯಾಖ್ಯಾನಿಸು ಎಲೆ_STR(ಎಲೆ)	((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *)((ಎಲೆ) + 2))

#ವ್ಯಾಖ್ಯಾನಿಸು MINCCC		(0)
#ವ್ಯಾಖ್ಯಾನಿಸು MAXCCC		(254)
#ವ್ಯಾಖ್ಯಾನಿಸು STOPPER		(0)
#ವ್ಯಾಖ್ಯಾನಿಸು	DECOMPOSE	(255)

/* Marker for hangul syllable decomposition. */
#ವ್ಯಾಖ್ಯಾನಿಸು HANGUL		((ಅಕ್ಷರ)(255))
/* Size of the synthesized leaf used for Hangul syllable decomposition. */
#ವ್ಯಾಖ್ಯಾನಿಸು UTF8HANGULLEAF	(12)

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
#ವ್ಯಾಖ್ಯಾನಿಸು SB	(0xAC00)
#ವ್ಯಾಖ್ಯಾನಿಸು LB	(0x1100)
#ವ್ಯಾಖ್ಯಾನಿಸು VB	(0x1161)
#ವ್ಯಾಖ್ಯಾನಿಸು TB	(0x11A7)
#ವ್ಯಾಖ್ಯಾನಿಸು LC	(19)
#ವ್ಯಾಖ್ಯಾನಿಸು VC	(21)
#ವ್ಯಾಖ್ಯಾನಿಸು TC	(28)
#ವ್ಯಾಖ್ಯಾನಿಸು NC	(VC * TC)
#ವ್ಯಾಖ್ಯಾನಿಸು SC	(LC * NC)

/* Algorithmic decomposition of hangul syllable. */
ಸ್ಥಿರ utf8leaf_t *
utf8hangul(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *hangul)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	si;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	li;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	vi;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	ti;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	*h;

	/* Calculate the SI, LI, VI, and TI values. */
	si = utf8decode3(str) - SB;
	li = si / NC;
	vi = (si % NC) / TC;
	ti = si % TC;

	/* Fill in base of leaf. */
	h = hangul;
	ಎಲೆ_GEN(h) = 2;
	ಎಲೆ_CCC(h) = DECOMPOSE;
	h += 2;

	/* Add LPart, a 3-byte UTF-8 sequence. */
	h += utf8encode3((ಅಕ್ಷರ *)h, li + LB);

	/* Add VPart, a 3-byte UTF-8 sequence. */
	h += utf8encode3((ಅಕ್ಷರ *)h, vi + VB);

	/* Add TPart if required, also a 3-byte UTF-8 sequence. */
	ಆದರೆ (ti)
		h += utf8encode3((ಅಕ್ಷರ *)h, ti + TB);

	/* Terminate string. */
	h[0] = '\0';

	ಹಿಂತಿರುಗಿ hangul;
}

/*
 * Use trie to scan s, touching at most len bytes.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * A non-NULL return guarantees that the UTF-8 sequence starting at s
 * is well-formed and corresponds to a known unicode code point.  The
 * shorthand for this will be "is valid UTF-8 unicode".
 */
ಸ್ಥಿರ utf8leaf_t *utf8nlookup(ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *ದತ್ತಾಂಶ,
			       ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *hangul, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, size_t len)
{
	utf8trie_t	*trie;
	ಪೂರ್ಣಾಂಕ		offlen;
	ಪೂರ್ಣಾಂಕ		ಸ್ಥಾನಾಂತರ;
	ಪೂರ್ಣಾಂಕ		ಮುಖವಾಡ;
	ಪೂರ್ಣಾಂಕ		ಗಂಟು;

	ಆದರೆ (!ದತ್ತಾಂಶ)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	ಆದರೆ (len == 0)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	trie = utf8data + ದತ್ತಾಂಶ->ಸ್ಥಾನಾಂತರ;
	ಗಂಟು = 1;
	ಆಗ (ಗಂಟು) {
		offlen = (*trie & OFFLEN) >> OFFLEN_ಸರಿಸು;
		ಆದರೆ (*trie & NEXTBYTE) {
			ಆದರೆ (--len == 0)
				ಹಿಂತಿರುಗಿ ಖಾಲಿ;
			s++;
		}
		ಮುಖವಾಡ = 1 << (*trie & BITNUM);
		ಆದರೆ (*s & ಮುಖವಾಡ) {
			/* Right leg */
			ಆದರೆ (offlen) {
				/* Right node at offset of trie */
				ಗಂಟು = (*trie & RIGHTNODE);
				ಸ್ಥಾನಾಂತರ = trie[offlen];
				ಆಗ (--offlen) {
					ಸ್ಥಾನಾಂತರ <<= 8;
					ಸ್ಥಾನಾಂತರ |= trie[offlen];
				}
				trie += ಸ್ಥಾನಾಂತರ;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*trie & RIGHTPATH) {
				/* Right node after this node */
				ಗಂಟು = (*trie & TRIENODE);
				trie++;
			} ಇಲ್ಲದಿದ್ದರೆ {
				/* No right node. */
				ಹಿಂತಿರುಗಿ ಖಾಲಿ;
			}
		} ಇಲ್ಲದಿದ್ದರೆ {
			/* Left leg */
			ಆದರೆ (offlen) {
				/* Left node after this node. */
				ಗಂಟು = (*trie & LEFTNODE);
				trie += offlen + 1;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*trie & RIGHTPATH) {
				/* No left node. */
				ಹಿಂತಿರುಗಿ ಖಾಲಿ;
			} ಇಲ್ಲದಿದ್ದರೆ {
				/* Left node after this node */
				ಗಂಟು = (*trie & TRIENODE);
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
	ಆದರೆ (ಎಲೆ_CCC(trie) == DECOMPOSE && ಎಲೆ_STR(trie)[0] == HANGUL)
		trie = utf8hangul(s - 2, hangul);
	ಹಿಂತಿರುಗಿ trie;
}

/*
 * Use trie to scan s.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * Forwards to utf8nlookup().
 */
ಸ್ಥಿರ utf8leaf_t *utf8lookup(ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *ದತ್ತಾಂಶ,
			      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *hangul, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
	ಹಿಂತಿರುಗಿ utf8nlookup(ದತ್ತಾಂಶ, hangul, s, (size_t)-1);
}

#ಆದರೆ 0
/*
 * Maximum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if only non-assigned code points are used.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ utf8agemax(ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *ದತ್ತಾಂಶ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
	utf8leaf_t	*ಎಲೆ_2;
	ಪೂರ್ಣಾಂಕ		age = 0;
	ಪೂರ್ಣಾಂಕ		ಎಲೆ_age;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	hangul[UTF8HANGULLEAF];

	ಆದರೆ (!ದತ್ತಾಂಶ)
		ಹಿಂತಿರುಗಿ -1;

	ಆಗ (*s) {
		ಎಲೆ_2 = utf8lookup(ದತ್ತಾಂಶ, hangul, s);
		ಆದರೆ (!ಎಲೆ_2)
			ಹಿಂತಿರುಗಿ -1;

		ಎಲೆ_age = utf8agetab[ಎಲೆ_GEN(ಎಲೆ_2)];
		ಆದರೆ (ಎಲೆ_age <= ದತ್ತಾಂಶ->maxage && ಎಲೆ_age > age)
			age = ಎಲೆ_age;
		s += utf8clen(s);
	}
	ಹಿಂತಿರುಗಿ age;
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ 0
/*
 * Minimum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if non-assigned code points are used.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ utf8agemin(ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *ದತ್ತಾಂಶ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
	utf8leaf_t	*ಎಲೆ_2;
	ಪೂರ್ಣಾಂಕ		age;
	ಪೂರ್ಣಾಂಕ		ಎಲೆ_age;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	hangul[UTF8HANGULLEAF];

	ಆದರೆ (!ದತ್ತಾಂಶ)
		ಹಿಂತಿರುಗಿ -1;
	age = ದತ್ತಾಂಶ->maxage;
	ಆಗ (*s) {
		ಎಲೆ_2 = utf8lookup(ದತ್ತಾಂಶ, hangul, s);
		ಆದರೆ (!ಎಲೆ_2)
			ಹಿಂತಿರುಗಿ -1;
		ಎಲೆ_age = utf8agetab[ಎಲೆ_GEN(ಎಲೆ_2)];
		ಆದರೆ (ಎಲೆ_age <= ದತ್ತಾಂಶ->maxage && ಎಲೆ_age < age)
			age = ಎಲೆ_age;
		s += utf8clen(s);
	}
	ಹಿಂತಿರುಗಿ age;
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ utf8nagemax(ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *ದತ್ತಾಂಶ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, size_t len)
{
	utf8leaf_t	*ಎಲೆ_2;
	ಪೂರ್ಣಾಂಕ		age = 0;
	ಪೂರ್ಣಾಂಕ		ಎಲೆ_age;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	hangul[UTF8HANGULLEAF];

	ಆದರೆ (!ದತ್ತಾಂಶ)
		ಹಿಂತಿರುಗಿ -1;

	ಆಗ (len && *s) {
		ಎಲೆ_2 = utf8nlookup(ದತ್ತಾಂಶ, hangul, s, len);
		ಆದರೆ (!ಎಲೆ_2)
			ಹಿಂತಿರುಗಿ -1;
		ಎಲೆ_age = utf8agetab[ಎಲೆ_GEN(ಎಲೆ_2)];
		ಆದರೆ (ಎಲೆ_age <= ದತ್ತಾಂಶ->maxage && ಎಲೆ_age > age)
			age = ಎಲೆ_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	ಹಿಂತಿರುಗಿ age;
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ utf8nagemin(ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *ದತ್ತಾಂಶ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, size_t len)
{
	utf8leaf_t	*ಎಲೆ_2;
	ಪೂರ್ಣಾಂಕ		ಎಲೆ_age;
	ಪೂರ್ಣಾಂಕ		age;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	hangul[UTF8HANGULLEAF];

	ಆದರೆ (!ದತ್ತಾಂಶ)
		ಹಿಂತಿರುಗಿ -1;
	age = ದತ್ತಾಂಶ->maxage;
	ಆಗ (len && *s) {
		ಎಲೆ_2 = utf8nlookup(ದತ್ತಾಂಶ, hangul, s, len);
		ಆದರೆ (!ಎಲೆ_2)
			ಹಿಂತಿರುಗಿ -1;
		ಎಲೆ_age = utf8agetab[ಎಲೆ_GEN(ಎಲೆ_2)];
		ಆದರೆ (ಎಲೆ_age <= ದತ್ತಾಂಶ->maxage && ಎಲೆ_age < age)
			age = ಎಲೆ_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	ಹಿಂತಿರುಗಿ age;
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ 0
/*
 * Length of the normalization of s.
 * Return -1 if s is not valid UTF-8 unicode.
 *
 * A string of Default_Ignorable_Code_Point has length 0.
 */
ಸ್ಥಿರ ssize_t utf8len(ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *ದತ್ತಾಂಶ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
	utf8leaf_t	*ಎಲೆ_2;
	size_t		ret = 0;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	hangul[UTF8HANGULLEAF];

	ಆದರೆ (!ದತ್ತಾಂಶ)
		ಹಿಂತಿರುಗಿ -1;
	ಆಗ (*s) {
		ಎಲೆ_2 = utf8lookup(ದತ್ತಾಂಶ, hangul, s);
		ಆದರೆ (!ಎಲೆ_2)
			ಹಿಂತಿರುಗಿ -1;
		ಆದರೆ (utf8agetab[ಎಲೆ_GEN(ಎಲೆ_2)] > ದತ್ತಾಂಶ->maxage)
			ret += utf8clen(s);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಎಲೆ_CCC(ಎಲೆ_2) == DECOMPOSE)
			ret += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಎಲೆ_STR(ಎಲೆ_2));
		ಇಲ್ಲದಿದ್ದರೆ
			ret += utf8clen(s);
		s += utf8clen(s);
	}
	ಹಿಂತಿರುಗಿ ret;
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ 0
/*
 * Length of the normalization of s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
ಸ್ಥಿರ ssize_t utf8nlen(ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *ದತ್ತಾಂಶ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, size_t len)
{
	utf8leaf_t	*ಎಲೆ_2;
	size_t		ret = 0;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	hangul[UTF8HANGULLEAF];

	ಆದರೆ (!ದತ್ತಾಂಶ)
		ಹಿಂತಿರುಗಿ -1;
	ಆಗ (len && *s) {
		ಎಲೆ_2 = utf8nlookup(ದತ್ತಾಂಶ, hangul, s, len);
		ಆದರೆ (!ಎಲೆ_2)
			ಹಿಂತಿರುಗಿ -1;
		ಆದರೆ (utf8agetab[ಎಲೆ_GEN(ಎಲೆ_2)] > ದತ್ತಾಂಶ->maxage)
			ret += utf8clen(s);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಎಲೆ_CCC(ಎಲೆ_2) == DECOMPOSE)
			ret += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಎಲೆ_STR(ಎಲೆ_2));
		ಇಲ್ಲದಿದ್ದರೆ
			ret += utf8clen(s);
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	ಹಿಂತಿರುಗಿ ret;
}
#ಆದರೆ_ಅಂತ್ಯ

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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ utf8ncursor(ರಚನೆ utf8cursor *u8c, ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *ದತ್ತಾಂಶ,
		ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s, size_t len)
{
	ಆದರೆ (!ದತ್ತಾಂಶ)
		ಹಿಂತಿರುಗಿ -1;
	ಆದರೆ (!s)
		ಹಿಂತಿರುಗಿ -1;
	u8c->ದತ್ತಾಂಶ = ದತ್ತಾಂಶ;
	u8c->s = s;
	u8c->p = ಖಾಲಿ;
	u8c->ss = ಖಾಲಿ;
	u8c->sp = ಖಾಲಿ;
	u8c->len = len;
	u8c->slen = 0;
	u8c->ccc = STOPPER;
	u8c->nccc = STOPPER;
	/* Check we didn't clobber the maximum length. */
	ಆದರೆ (u8c->len != len)
		ಹಿಂತಿರುಗಿ -1;
	/* The first byte of s may not be an utf8 continuation. */
	ಆದರೆ (len > 0 && (*s & 0xC0) == 0x80)
		ಹಿಂತಿರುಗಿ -1;
	ಹಿಂತಿರುಗಿ 0;
}

#ಆದರೆ 0
/*
 * Set up an utf8cursor for use by utf8byte().
 *
 *   u8c    : pointer to cursor.
 *   data   : const struct utf8data to use for normalization.
 *   s      : NUL-terminated string.
 *
 * Returns -1 on error, 0 on success.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ utf8cursor(ರಚನೆ utf8cursor *u8c, ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *ದತ್ತಾಂಶ,
	       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
	ಹಿಂತಿರುಗಿ utf8ncursor(u8c, ದತ್ತಾಂಶ, s, (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)-1);
}
#ಆದರೆ_ಅಂತ್ಯ

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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ utf8byte(ರಚನೆ utf8cursor *u8c)
{
	utf8leaf_t *ಎಲೆ_2;
	ಪೂರ್ಣಾಂಕ ccc;

	ಗಾಗಿ (;;) {
		/* Check for the end of a decomposed character. */
		ಆದರೆ (u8c->p && *u8c->s == '\0') {
			u8c->s = u8c->p;
			u8c->p = ಖಾಲಿ;
		}

		/* Check for end-of-string. */
		ಆದರೆ (!u8c->p && (u8c->len == 0 || *u8c->s == '\0')) {
			/* There is no next byte. */
			ಆದರೆ (u8c->ccc == STOPPER)
				ಹಿಂತಿರುಗಿ 0;
			/* End-of-string during a scan counts as a stopper. */
			ccc = STOPPER;
			ಹೋಗು ccc_mismatch;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((*u8c->s & 0xC0) == 0x80) {
			/* This is a continuation of the current character. */
			ಆದರೆ (!u8c->p)
				u8c->len--;
			ಹಿಂತಿರುಗಿ (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)*u8c->s++;
		}

		/* Look up the data for the current character. */
		ಆದರೆ (u8c->p) {
			ಎಲೆ_2 = utf8lookup(u8c->ದತ್ತಾಂಶ, u8c->hangul, u8c->s);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಎಲೆ_2 = utf8nlookup(u8c->ದತ್ತಾಂಶ, u8c->hangul,
					   u8c->s, u8c->len);
		}

		/* No leaf found implies that the input is a binary blob. */
		ಆದರೆ (!ಎಲೆ_2)
			ಹಿಂತಿರುಗಿ -1;

		ccc = ಎಲೆ_CCC(ಎಲೆ_2);
		/* Characters that are too new have CCC 0. */
		ಆದರೆ (utf8agetab[ಎಲೆ_GEN(ಎಲೆ_2)] > u8c->ದತ್ತಾಂಶ->maxage) {
			ccc = STOPPER;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ccc == DECOMPOSE) {
			u8c->len -= utf8clen(u8c->s);
			u8c->p = u8c->s + utf8clen(u8c->s);
			u8c->s = ಎಲೆ_STR(ಎಲೆ_2);
			/* Empty decomposition implies CCC 0. */
			ಆದರೆ (*u8c->s == '\0') {
				ಆದರೆ (u8c->ccc == STOPPER)
					ಮುಂದುವರಿ;
				ccc = STOPPER;
				ಹೋಗು ccc_mismatch;
			}

			ಎಲೆ_2 = utf8lookup(u8c->ದತ್ತಾಂಶ, u8c->hangul, u8c->s);
			ಆದರೆ (!ಎಲೆ_2)
				ಹಿಂತಿರುಗಿ -1;
			ccc = ಎಲೆ_CCC(ಎಲೆ_2);
		}

		/*
		 * If this is not a stopper, then see if it updates
		 * the next canonical class to be emitted.
		 */
		ಆದರೆ (ccc != STOPPER && u8c->ccc < ccc && ccc < u8c->nccc)
			u8c->nccc = ccc;

		/*
		 * Return the current byte if this is the current
		 * combining class.
		 */
		ಆದರೆ (ccc == u8c->ccc) {
			ಆದರೆ (!u8c->p)
				u8c->len--;
			ಹಿಂತಿರುಗಿ (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)*u8c->s++;
		}

		/* Current combining class mismatch. */
ccc_mismatch:
		ಆದರೆ (u8c->nccc == STOPPER) {
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
			ಆದರೆ (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ccc != STOPPER) {
			/* Not a stopper, and not the ccc we're emitting. */
			ಆದರೆ (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (u8c->nccc != MAXCCC + 1) {
			/* At a stopper, restart for next ccc. */
			u8c->ccc = u8c->nccc;
			u8c->nccc = MAXCCC + 1;
			u8c->s = u8c->ss;
			u8c->p = u8c->sp;
			u8c->len = u8c->slen;
		} ಇಲ್ಲದಿದ್ದರೆ {
			/* All done, proceed from here. */
			u8c->ccc = STOPPER;
			u8c->nccc = STOPPER;
			u8c->sp = ಖಾಲಿ;
			u8c->ss = ಖಾಲಿ;
			u8c->slen = 0;
		}
	}
}

#ಆದರೆ 0
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
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *utf8nfdi(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ maxage)
{
	ಪೂರ್ಣಾಂಕ i = ARRAY_ಗಾತ್ರ(utf8nfdidata) - 1;

	ಆಗ (maxage < utf8nfdidata[i].maxage)
		i--;
	ಆದರೆ (maxage > utf8nfdidata[i].maxage)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	ಹಿಂತಿರುಗಿ &utf8nfdidata[i];
}
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *utf8nfdicf(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ maxage)
{
	ಪೂರ್ಣಾಂಕ i = ARRAY_ಗಾತ್ರ(utf8nfdicfdata) - 1;

	ಆಗ (maxage < utf8nfdicfdata[i].maxage)
		i--;
	ಆದರೆ (maxage > utf8nfdicfdata[i].maxage)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	ಹಿಂತಿರುಗಿ &utf8nfdicfdata[i];
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ utf8_casefold(ಸ್ಥಿರಾಂಕ ರಚನೆ ext2fs_nls_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ,
			  ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *str, size_t len,
			  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *dest, size_t dlen)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *ದತ್ತಾಂಶ = utf8nfdicf(ಕೋಷ್ಟಕ->ಆವೃತ್ತಿ);
	ರಚನೆ utf8cursor cur;
	size_t nlen = 0;

	ಆದರೆ (utf8ncursor(&cur, ದತ್ತಾಂಶ, (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *) str, len) < 0)
		ಹೋಗು invalid_seq;

	ಗಾಗಿ (nlen = 0; nlen < dlen; nlen++) {
		ಪೂರ್ಣಾಂಕ c = utf8byte(&cur);

		dest[nlen] = c;
		ಆದರೆ (!c)
			ಹಿಂತಿರುಗಿ nlen;
		ಆದರೆ (c == -1)
			ನಿಲ್ಲಿಸು;
	}

	ಹಿಂತಿರುಗಿ -ENAMETOOLONG;

invalid_seq:
	ಆದರೆ (dlen < len)
		ಹಿಂತಿರುಗಿ -ENAMETOOLONG;

	/* Signal invalid sequence */
	ಹಿಂತಿರುಗಿ -EINVAL;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ utf8_validate(ಸ್ಥಿರಾಂಕ ರಚನೆ ext2fs_nls_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ,
			 ಅಕ್ಷರ *s, size_t len, ಅಕ್ಷರ **pos)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *ದತ್ತಾಂಶ = utf8nfdicf(ಕೋಷ್ಟಕ->ಆವೃತ್ತಿ);
	utf8leaf_t	*ಎಲೆ_2;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	hangul[UTF8HANGULLEAF];

	ಆದರೆ (!ದತ್ತಾಂಶ)
		ಹಿಂತಿರುಗಿ -1;
	ಆಗ (len && *s) {
		ಎಲೆ_2 = utf8nlookup(ದತ್ತಾಂಶ, hangul, s, len);
		ಆದರೆ (!ಎಲೆ_2) {
			*pos = s;
			ಹಿಂತಿರುಗಿ 1;
		}
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ utf8_casefold_cmp(ಸ್ಥಿರಾಂಕ ರಚನೆ ext2fs_nls_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ,
			     ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *str1, size_t len1,
			     ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *str2, size_t len2)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ utf8data *ದತ್ತಾಂಶ = utf8nfdicf(ಕೋಷ್ಟಕ->ಆವೃತ್ತಿ);
	ಪೂರ್ಣಾಂಕ c1, c2;
	ರಚನೆ utf8cursor cur1, cur2;

	ಆದರೆ (utf8ncursor(&cur1, ದತ್ತಾಂಶ, (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *) str1, len1) < 0)
		ಹಿಂತಿರುಗಿ -1;
	ಆದರೆ (utf8ncursor(&cur2, ದತ್ತಾಂಶ, (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *) str2, len2) < 0)
		ಹಿಂತಿರುಗಿ -1;

	ಮಾಡು {
		c1 = utf8byte(&cur1);
		c2 = utf8byte(&cur2);

		ಆದರೆ (c1 < 0 || c2 < 0)
			ಹಿಂತಿರುಗಿ -1;
		ಆದರೆ (c1 != c2)
			ಹಿಂತಿರುಗಿ c1 - c2;
	} ಆಗ (c1);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ ext2fs_nls_ops utf8_ops = {
	.casefold = utf8_casefold,
	.validate = utf8_validate,
	.casefold_cmp = utf8_casefold_cmp,
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ ext2fs_nls_ಕೋಷ್ಟಕ nls_utf8 = {
	.ops = &utf8_ops,
	.ಆವೃತ್ತಿ = UNICODE_AGE(12, 1, 0),
};

ಸ್ಥಿರಾಂಕ ರಚನೆ ext2fs_nls_ಕೋಷ್ಟಕ *ext2fs_ತುಂಬು_nls_ಕೋಷ್ಟಕ(ಪೂರ್ಣಾಂಕ encoding)
{
	ಆದರೆ (encoding == EXT4_ENC_UTF8_12_1)
		ಹಿಂತಿರುಗಿ &nls_utf8;

	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಪೂರ್ಣಾಂಕ ext2fs_check_encoded_name(ಸ್ಥಿರಾಂಕ ರಚನೆ ext2fs_nls_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ,
			      ಅಕ್ಷರ *name, size_t len, ಅಕ್ಷರ **pos)
{
	ಹಿಂತಿರುಗಿ ಕೋಷ್ಟಕ->ops->validate(ಕೋಷ್ಟಕ, name, len, pos);
}

ಪೂರ್ಣಾಂಕ ext2fs_casefold_cmp(ಸ್ಥಿರಾಂಕ ರಚನೆ ext2fs_nls_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ,
			ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *str1, size_t len1,
			ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *str2, size_t len2)
{
	ಹಿಂತಿರುಗಿ ಕೋಷ್ಟಕ->ops->casefold_cmp(ಕೋಷ್ಟಕ, str1, len1, str2, len2);
}
