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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

/* Encoding a unicode version number as a single unsigned int. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UNICODE_MAJ_ਖਿਸਕਾਓ		(16)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UNICODE_MIN_ਖਿਸਕਾਓ		(8)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UNICODE_AGE(MAJ, MIN, REV)			\
	(((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)(MAJ) << UNICODE_MAJ_ਖਿਸਕਾਓ) |	\
	 ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)(MIN) << UNICODE_MIN_ਖਿਸਕਾਓ) |	\
	 ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)(REV)))

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


ਬਣਤਰ utf8data {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ maxage;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __INCLUDED_FROM_UTF8NORM_C__
#ਅਨ੍ਤਰ੍ਭਾਵ "utf8data.h"
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ __INCLUDED_FROM_UTF8NORM_C__

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ARRAY_ਮਾਪ(array)			\
        (ਆਕਾਰ(array) / ਆਕਾਰ(array[0]))

#ਜੇ 0
/* Highest unicode version supported by the data tables. */
ਸਥਿਰ ਪੂਰਨ utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev)
{
	ਪੂਰਨ i = ARRAY_ਮਾਪ(utf8agetab) - 1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sb_utf8version = UNICODE_AGE(maj, min, rev);

	ਜਦੋਂ (i >= 0 && utf8agetab[i] != 0) {
		ਜੇ (sb_utf8version == utf8agetab[i])
			ਵਾਪਸ 1;
		i--;
	}
	ਵਾਪਸ 0;
}
#ਜੇ_ਅੰਤ

#ਜੇ 0
ਸਥਿਰ ਪੂਰਨ utf8version_latest(ਖਾਲੀ)
{
	ਵਾਪਸ utf8vers;
}
#ਜੇ_ਅੰਤ

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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ utf8clen(ਅਚਲ ਅੱਖਰ *s)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c = *s;

	ਵਾਪਸ 1 + (c >= 0xC0) + (c >= 0xE0) + (c >= 0xF0);
}

/*
 * Decode a 3-byte UTF-8 sequence.
 */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
utf8decode3(ਅਚਲ ਅੱਖਰ *str)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		uc;

	uc = *str++ & 0x0F;
	uc <<= 6;
	uc |= *str++ & 0x3F;
	uc <<= 6;
	uc |= *str++ & 0x3F;

	ਵਾਪਸ uc;
}

/*
 * Encode a 3-byte UTF-8 sequence.
 */
ਸਥਿਰ ਪੂਰਨ
utf8encode3(ਅੱਖਰ *str, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ val)
{
	str[2] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[1] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[0] = val | 0xE0;

	ਵਾਪਸ 3;
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
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ utf8trie_t;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BITNUM		0x07
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NEXTBYTE	0x08
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OFFLEN		0x30
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OFFLEN_ਖਿਸਕਾਓ	4
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RIGHTPATH	0x40
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TRIENODE	0x80
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RIGHTNODE	0x40
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LEFTNODE	0x80

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
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ utf8leaf_t;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੱਤੀ_GEN(ਪੱਤੀ)	((ਪੱਤੀ)[0])
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੱਤੀ_CCC(ਪੱਤੀ)	((ਪੱਤੀ)[1])
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੱਤੀ_STR(ਪੱਤੀ)	((ਅਚਲ ਅੱਖਰ *)((ਪੱਤੀ) + 2))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MINCCC		(0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAXCCC		(254)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STOPPER		(0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ	DECOMPOSE	(255)

/* Marker for hangul syllable decomposition. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HANGUL		((ਅੱਖਰ)(255))
/* Size of the synthesized leaf used for Hangul syllable decomposition. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UTF8HANGULLEAF	(12)

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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SB	(0xAC00)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LB	(0x1100)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ VB	(0x1161)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TB	(0x11A7)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LC	(19)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ VC	(21)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TC	(28)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NC	(VC * TC)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SC	(LC * NC)

/* Algorithmic decomposition of hangul syllable. */
ਸਥਿਰ utf8leaf_t *
utf8hangul(ਅਚਲ ਅੱਖਰ *str, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *hangul)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	si;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	li;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	vi;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	ti;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	*h;

	/* Calculate the SI, LI, VI, and TI values. */
	si = utf8decode3(str) - SB;
	li = si / NC;
	vi = (si % NC) / TC;
	ti = si % TC;

	/* Fill in base of leaf. */
	h = hangul;
	ਪੱਤੀ_GEN(h) = 2;
	ਪੱਤੀ_CCC(h) = DECOMPOSE;
	h += 2;

	/* Add LPart, a 3-byte UTF-8 sequence. */
	h += utf8encode3((ਅੱਖਰ *)h, li + LB);

	/* Add VPart, a 3-byte UTF-8 sequence. */
	h += utf8encode3((ਅੱਖਰ *)h, vi + VB);

	/* Add TPart if required, also a 3-byte UTF-8 sequence. */
	ਜੇ (ti)
		h += utf8encode3((ਅੱਖਰ *)h, ti + TB);

	/* Terminate string. */
	h[0] = '\0';

	ਵਾਪਸ hangul;
}

/*
 * Use trie to scan s, touching at most len bytes.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * A non-NULL return guarantees that the UTF-8 sequence starting at s
 * is well-formed and corresponds to a known unicode code point.  The
 * shorthand for this will be "is valid UTF-8 unicode".
 */
ਸਥਿਰ utf8leaf_t *utf8nlookup(ਅਚਲ ਬਣਤਰ utf8data *ਅੰਕੜੇ,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *hangul, ਅਚਲ ਅੱਖਰ *s, size_t len)
{
	utf8trie_t	*trie;
	ਪੂਰਨ		offlen;
	ਪੂਰਨ		ਵਿੱਥ;
	ਪੂਰਨ		ਨਕਾਬ;
	ਪੂਰਨ		ਗੰਢ;

	ਜੇ (!ਅੰਕੜੇ)
		ਵਾਪਸ ਨਿਰਾ;
	ਜੇ (len == 0)
		ਵਾਪਸ ਨਿਰਾ;

	trie = utf8data + ਅੰਕੜੇ->ਵਿੱਥ;
	ਗੰਢ = 1;
	ਜਦੋਂ (ਗੰਢ) {
		offlen = (*trie & OFFLEN) >> OFFLEN_ਖਿਸਕਾਓ;
		ਜੇ (*trie & NEXTBYTE) {
			ਜੇ (--len == 0)
				ਵਾਪਸ ਨਿਰਾ;
			s++;
		}
		ਨਕਾਬ = 1 << (*trie & BITNUM);
		ਜੇ (*s & ਨਕਾਬ) {
			/* Right leg */
			ਜੇ (offlen) {
				/* Right node at offset of trie */
				ਗੰਢ = (*trie & RIGHTNODE);
				ਵਿੱਥ = trie[offlen];
				ਜਦੋਂ (--offlen) {
					ਵਿੱਥ <<= 8;
					ਵਿੱਥ |= trie[offlen];
				}
				trie += ਵਿੱਥ;
			} ਨਹੀਂਤਾਂ ਜੇ (*trie & RIGHTPATH) {
				/* Right node after this node */
				ਗੰਢ = (*trie & TRIENODE);
				trie++;
			} ਨਹੀਂਤਾਂ {
				/* No right node. */
				ਵਾਪਸ ਨਿਰਾ;
			}
		} ਨਹੀਂਤਾਂ {
			/* Left leg */
			ਜੇ (offlen) {
				/* Left node after this node. */
				ਗੰਢ = (*trie & LEFTNODE);
				trie += offlen + 1;
			} ਨਹੀਂਤਾਂ ਜੇ (*trie & RIGHTPATH) {
				/* No left node. */
				ਵਾਪਸ ਨਿਰਾ;
			} ਨਹੀਂਤਾਂ {
				/* Left node after this node */
				ਗੰਢ = (*trie & TRIENODE);
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
	ਜੇ (ਪੱਤੀ_CCC(trie) == DECOMPOSE && ਪੱਤੀ_STR(trie)[0] == HANGUL)
		trie = utf8hangul(s - 2, hangul);
	ਵਾਪਸ trie;
}

/*
 * Use trie to scan s.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * Forwards to utf8nlookup().
 */
ਸਥਿਰ utf8leaf_t *utf8lookup(ਅਚਲ ਬਣਤਰ utf8data *ਅੰਕੜੇ,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *hangul, ਅਚਲ ਅੱਖਰ *s)
{
	ਵਾਪਸ utf8nlookup(ਅੰਕੜੇ, hangul, s, (size_t)-1);
}

#ਜੇ 0
/*
 * Maximum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if only non-assigned code points are used.
 */
ਸਥਿਰ ਪੂਰਨ utf8agemax(ਅਚਲ ਬਣਤਰ utf8data *ਅੰਕੜੇ, ਅਚਲ ਅੱਖਰ *s)
{
	utf8leaf_t	*ਪੱਤੀ_2;
	ਪੂਰਨ		age = 0;
	ਪੂਰਨ		ਪੱਤੀ_age;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	hangul[UTF8HANGULLEAF];

	ਜੇ (!ਅੰਕੜੇ)
		ਵਾਪਸ -1;

	ਜਦੋਂ (*s) {
		ਪੱਤੀ_2 = utf8lookup(ਅੰਕੜੇ, hangul, s);
		ਜੇ (!ਪੱਤੀ_2)
			ਵਾਪਸ -1;

		ਪੱਤੀ_age = utf8agetab[ਪੱਤੀ_GEN(ਪੱਤੀ_2)];
		ਜੇ (ਪੱਤੀ_age <= ਅੰਕੜੇ->maxage && ਪੱਤੀ_age > age)
			age = ਪੱਤੀ_age;
		s += utf8clen(s);
	}
	ਵਾਪਸ age;
}
#ਜੇ_ਅੰਤ

#ਜੇ 0
/*
 * Minimum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if non-assigned code points are used.
 */
ਸਥਿਰ ਪੂਰਨ utf8agemin(ਅਚਲ ਬਣਤਰ utf8data *ਅੰਕੜੇ, ਅਚਲ ਅੱਖਰ *s)
{
	utf8leaf_t	*ਪੱਤੀ_2;
	ਪੂਰਨ		age;
	ਪੂਰਨ		ਪੱਤੀ_age;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	hangul[UTF8HANGULLEAF];

	ਜੇ (!ਅੰਕੜੇ)
		ਵਾਪਸ -1;
	age = ਅੰਕੜੇ->maxage;
	ਜਦੋਂ (*s) {
		ਪੱਤੀ_2 = utf8lookup(ਅੰਕੜੇ, hangul, s);
		ਜੇ (!ਪੱਤੀ_2)
			ਵਾਪਸ -1;
		ਪੱਤੀ_age = utf8agetab[ਪੱਤੀ_GEN(ਪੱਤੀ_2)];
		ਜੇ (ਪੱਤੀ_age <= ਅੰਕੜੇ->maxage && ਪੱਤੀ_age < age)
			age = ਪੱਤੀ_age;
		s += utf8clen(s);
	}
	ਵਾਪਸ age;
}
#ਜੇ_ਅੰਤ

#ਜੇ 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
ਸਥਿਰ ਪੂਰਨ utf8nagemax(ਅਚਲ ਬਣਤਰ utf8data *ਅੰਕੜੇ, ਅਚਲ ਅੱਖਰ *s, size_t len)
{
	utf8leaf_t	*ਪੱਤੀ_2;
	ਪੂਰਨ		age = 0;
	ਪੂਰਨ		ਪੱਤੀ_age;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	hangul[UTF8HANGULLEAF];

	ਜੇ (!ਅੰਕੜੇ)
		ਵਾਪਸ -1;

	ਜਦੋਂ (len && *s) {
		ਪੱਤੀ_2 = utf8nlookup(ਅੰਕੜੇ, hangul, s, len);
		ਜੇ (!ਪੱਤੀ_2)
			ਵਾਪਸ -1;
		ਪੱਤੀ_age = utf8agetab[ਪੱਤੀ_GEN(ਪੱਤੀ_2)];
		ਜੇ (ਪੱਤੀ_age <= ਅੰਕੜੇ->maxage && ਪੱਤੀ_age > age)
			age = ਪੱਤੀ_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	ਵਾਪਸ age;
}
#ਜੇ_ਅੰਤ

#ਜੇ 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
ਸਥਿਰ ਪੂਰਨ utf8nagemin(ਅਚਲ ਬਣਤਰ utf8data *ਅੰਕੜੇ, ਅਚਲ ਅੱਖਰ *s, size_t len)
{
	utf8leaf_t	*ਪੱਤੀ_2;
	ਪੂਰਨ		ਪੱਤੀ_age;
	ਪੂਰਨ		age;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	hangul[UTF8HANGULLEAF];

	ਜੇ (!ਅੰਕੜੇ)
		ਵਾਪਸ -1;
	age = ਅੰਕੜੇ->maxage;
	ਜਦੋਂ (len && *s) {
		ਪੱਤੀ_2 = utf8nlookup(ਅੰਕੜੇ, hangul, s, len);
		ਜੇ (!ਪੱਤੀ_2)
			ਵਾਪਸ -1;
		ਪੱਤੀ_age = utf8agetab[ਪੱਤੀ_GEN(ਪੱਤੀ_2)];
		ਜੇ (ਪੱਤੀ_age <= ਅੰਕੜੇ->maxage && ਪੱਤੀ_age < age)
			age = ਪੱਤੀ_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	ਵਾਪਸ age;
}
#ਜੇ_ਅੰਤ

#ਜੇ 0
/*
 * Length of the normalization of s.
 * Return -1 if s is not valid UTF-8 unicode.
 *
 * A string of Default_Ignorable_Code_Point has length 0.
 */
ਸਥਿਰ ssize_t utf8len(ਅਚਲ ਬਣਤਰ utf8data *ਅੰਕੜੇ, ਅਚਲ ਅੱਖਰ *s)
{
	utf8leaf_t	*ਪੱਤੀ_2;
	size_t		ret = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	hangul[UTF8HANGULLEAF];

	ਜੇ (!ਅੰਕੜੇ)
		ਵਾਪਸ -1;
	ਜਦੋਂ (*s) {
		ਪੱਤੀ_2 = utf8lookup(ਅੰਕੜੇ, hangul, s);
		ਜੇ (!ਪੱਤੀ_2)
			ਵਾਪਸ -1;
		ਜੇ (utf8agetab[ਪੱਤੀ_GEN(ਪੱਤੀ_2)] > ਅੰਕੜੇ->maxage)
			ret += utf8clen(s);
		ਨਹੀਂਤਾਂ ਜੇ (ਪੱਤੀ_CCC(ਪੱਤੀ_2) == DECOMPOSE)
			ret += ਲੜੀ_ਲੰਬਾਈ(ਪੱਤੀ_STR(ਪੱਤੀ_2));
		ਨਹੀਂਤਾਂ
			ret += utf8clen(s);
		s += utf8clen(s);
	}
	ਵਾਪਸ ret;
}
#ਜੇ_ਅੰਤ

#ਜੇ 0
/*
 * Length of the normalization of s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
ਸਥਿਰ ssize_t utf8nlen(ਅਚਲ ਬਣਤਰ utf8data *ਅੰਕੜੇ, ਅਚਲ ਅੱਖਰ *s, size_t len)
{
	utf8leaf_t	*ਪੱਤੀ_2;
	size_t		ret = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	hangul[UTF8HANGULLEAF];

	ਜੇ (!ਅੰਕੜੇ)
		ਵਾਪਸ -1;
	ਜਦੋਂ (len && *s) {
		ਪੱਤੀ_2 = utf8nlookup(ਅੰਕੜੇ, hangul, s, len);
		ਜੇ (!ਪੱਤੀ_2)
			ਵਾਪਸ -1;
		ਜੇ (utf8agetab[ਪੱਤੀ_GEN(ਪੱਤੀ_2)] > ਅੰਕੜੇ->maxage)
			ret += utf8clen(s);
		ਨਹੀਂਤਾਂ ਜੇ (ਪੱਤੀ_CCC(ਪੱਤੀ_2) == DECOMPOSE)
			ret += ਲੜੀ_ਲੰਬਾਈ(ਪੱਤੀ_STR(ਪੱਤੀ_2));
		ਨਹੀਂਤਾਂ
			ret += utf8clen(s);
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	ਵਾਪਸ ret;
}
#ਜੇ_ਅੰਤ

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
ਸਥਿਰ ਪੂਰਨ utf8ncursor(ਬਣਤਰ utf8cursor *u8c, ਅਚਲ ਬਣਤਰ utf8data *ਅੰਕੜੇ,
		ਅਚਲ ਅੱਖਰ *s, size_t len)
{
	ਜੇ (!ਅੰਕੜੇ)
		ਵਾਪਸ -1;
	ਜੇ (!s)
		ਵਾਪਸ -1;
	u8c->ਅੰਕੜੇ = ਅੰਕੜੇ;
	u8c->s = s;
	u8c->p = ਨਿਰਾ;
	u8c->ss = ਨਿਰਾ;
	u8c->sp = ਨਿਰਾ;
	u8c->len = len;
	u8c->slen = 0;
	u8c->ccc = STOPPER;
	u8c->nccc = STOPPER;
	/* Check we didn't clobber the maximum length. */
	ਜੇ (u8c->len != len)
		ਵਾਪਸ -1;
	/* The first byte of s may not be an utf8 continuation. */
	ਜੇ (len > 0 && (*s & 0xC0) == 0x80)
		ਵਾਪਸ -1;
	ਵਾਪਸ 0;
}

#ਜੇ 0
/*
 * Set up an utf8cursor for use by utf8byte().
 *
 *   u8c    : pointer to cursor.
 *   data   : const struct utf8data to use for normalization.
 *   s      : NUL-terminated string.
 *
 * Returns -1 on error, 0 on success.
 */
ਸਥਿਰ ਪੂਰਨ utf8cursor(ਬਣਤਰ utf8cursor *u8c, ਅਚਲ ਬਣਤਰ utf8data *ਅੰਕੜੇ,
	       ਅਚਲ ਅੱਖਰ *s)
{
	ਵਾਪਸ utf8ncursor(u8c, ਅੰਕੜੇ, s, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)-1);
}
#ਜੇ_ਅੰਤ

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
ਸਥਿਰ ਪੂਰਨ utf8byte(ਬਣਤਰ utf8cursor *u8c)
{
	utf8leaf_t *ਪੱਤੀ_2;
	ਪੂਰਨ ccc;

	ਲਈ (;;) {
		/* Check for the end of a decomposed character. */
		ਜੇ (u8c->p && *u8c->s == '\0') {
			u8c->s = u8c->p;
			u8c->p = ਨਿਰਾ;
		}

		/* Check for end-of-string. */
		ਜੇ (!u8c->p && (u8c->len == 0 || *u8c->s == '\0')) {
			/* There is no next byte. */
			ਜੇ (u8c->ccc == STOPPER)
				ਵਾਪਸ 0;
			/* End-of-string during a scan counts as a stopper. */
			ccc = STOPPER;
			ਜਾਓ ccc_mismatch;
		} ਨਹੀਂਤਾਂ ਜੇ ((*u8c->s & 0xC0) == 0x80) {
			/* This is a continuation of the current character. */
			ਜੇ (!u8c->p)
				u8c->len--;
			ਵਾਪਸ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)*u8c->s++;
		}

		/* Look up the data for the current character. */
		ਜੇ (u8c->p) {
			ਪੱਤੀ_2 = utf8lookup(u8c->ਅੰਕੜੇ, u8c->hangul, u8c->s);
		} ਨਹੀਂਤਾਂ {
			ਪੱਤੀ_2 = utf8nlookup(u8c->ਅੰਕੜੇ, u8c->hangul,
					   u8c->s, u8c->len);
		}

		/* No leaf found implies that the input is a binary blob. */
		ਜੇ (!ਪੱਤੀ_2)
			ਵਾਪਸ -1;

		ccc = ਪੱਤੀ_CCC(ਪੱਤੀ_2);
		/* Characters that are too new have CCC 0. */
		ਜੇ (utf8agetab[ਪੱਤੀ_GEN(ਪੱਤੀ_2)] > u8c->ਅੰਕੜੇ->maxage) {
			ccc = STOPPER;
		} ਨਹੀਂਤਾਂ ਜੇ (ccc == DECOMPOSE) {
			u8c->len -= utf8clen(u8c->s);
			u8c->p = u8c->s + utf8clen(u8c->s);
			u8c->s = ਪੱਤੀ_STR(ਪੱਤੀ_2);
			/* Empty decomposition implies CCC 0. */
			ਜੇ (*u8c->s == '\0') {
				ਜੇ (u8c->ccc == STOPPER)
					ਜਾਰੀ;
				ccc = STOPPER;
				ਜਾਓ ccc_mismatch;
			}

			ਪੱਤੀ_2 = utf8lookup(u8c->ਅੰਕੜੇ, u8c->hangul, u8c->s);
			ਜੇ (!ਪੱਤੀ_2)
				ਵਾਪਸ -1;
			ccc = ਪੱਤੀ_CCC(ਪੱਤੀ_2);
		}

		/*
		 * If this is not a stopper, then see if it updates
		 * the next canonical class to be emitted.
		 */
		ਜੇ (ccc != STOPPER && u8c->ccc < ccc && ccc < u8c->nccc)
			u8c->nccc = ccc;

		/*
		 * Return the current byte if this is the current
		 * combining class.
		 */
		ਜੇ (ccc == u8c->ccc) {
			ਜੇ (!u8c->p)
				u8c->len--;
			ਵਾਪਸ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)*u8c->s++;
		}

		/* Current combining class mismatch. */
ccc_mismatch:
		ਜੇ (u8c->nccc == STOPPER) {
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
			ਜੇ (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} ਨਹੀਂਤਾਂ ਜੇ (ccc != STOPPER) {
			/* Not a stopper, and not the ccc we're emitting. */
			ਜੇ (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} ਨਹੀਂਤਾਂ ਜੇ (u8c->nccc != MAXCCC + 1) {
			/* At a stopper, restart for next ccc. */
			u8c->ccc = u8c->nccc;
			u8c->nccc = MAXCCC + 1;
			u8c->s = u8c->ss;
			u8c->p = u8c->sp;
			u8c->len = u8c->slen;
		} ਨਹੀਂਤਾਂ {
			/* All done, proceed from here. */
			u8c->ccc = STOPPER;
			u8c->nccc = STOPPER;
			u8c->sp = ਨਿਰਾ;
			u8c->ss = ਨਿਰਾ;
			u8c->slen = 0;
		}
	}
}

#ਜੇ 0
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
ਸਥਿਰ ਅਚਲ ਬਣਤਰ utf8data *utf8nfdi(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ maxage)
{
	ਪੂਰਨ i = ARRAY_ਮਾਪ(utf8nfdidata) - 1;

	ਜਦੋਂ (maxage < utf8nfdidata[i].maxage)
		i--;
	ਜੇ (maxage > utf8nfdidata[i].maxage)
		ਵਾਪਸ ਨਿਰਾ;
	ਵਾਪਸ &utf8nfdidata[i];
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਅਚਲ ਬਣਤਰ utf8data *utf8nfdicf(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ maxage)
{
	ਪੂਰਨ i = ARRAY_ਮਾਪ(utf8nfdicfdata) - 1;

	ਜਦੋਂ (maxage < utf8nfdicfdata[i].maxage)
		i--;
	ਜੇ (maxage > utf8nfdicfdata[i].maxage)
		ਵਾਪਸ ਨਿਰਾ;
	ਵਾਪਸ &utf8nfdicfdata[i];
}

ਸਥਿਰ ਪੂਰਨ utf8_casefold(ਅਚਲ ਬਣਤਰ ext2fs_nls_ਸਾਰਣੀ *ਸਾਰਣੀ,
			  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *str, size_t len,
			  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *dest, size_t dlen)
{
	ਅਚਲ ਬਣਤਰ utf8data *ਅੰਕੜੇ = utf8nfdicf(ਸਾਰਣੀ->ਸੰਸਕਰਣ);
	ਬਣਤਰ utf8cursor cur;
	size_t nlen = 0;

	ਜੇ (utf8ncursor(&cur, ਅੰਕੜੇ, (ਅਚਲ ਅੱਖਰ *) str, len) < 0)
		ਜਾਓ invalid_seq;

	ਲਈ (nlen = 0; nlen < dlen; nlen++) {
		ਪੂਰਨ c = utf8byte(&cur);

		dest[nlen] = c;
		ਜੇ (!c)
			ਵਾਪਸ nlen;
		ਜੇ (c == -1)
			ਰੋਕੋ;
	}

	ਵਾਪਸ -ENAMETOOLONG;

invalid_seq:
	ਜੇ (dlen < len)
		ਵਾਪਸ -ENAMETOOLONG;

	/* Signal invalid sequence */
	ਵਾਪਸ -EINVAL;
}

ਸਥਿਰ ਪੂਰਨ utf8_validate(ਅਚਲ ਬਣਤਰ ext2fs_nls_ਸਾਰਣੀ *ਸਾਰਣੀ,
			 ਅੱਖਰ *s, size_t len, ਅੱਖਰ **pos)
{
	ਅਚਲ ਬਣਤਰ utf8data *ਅੰਕੜੇ = utf8nfdicf(ਸਾਰਣੀ->ਸੰਸਕਰਣ);
	utf8leaf_t	*ਪੱਤੀ_2;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	hangul[UTF8HANGULLEAF];

	ਜੇ (!ਅੰਕੜੇ)
		ਵਾਪਸ -1;
	ਜਦੋਂ (len && *s) {
		ਪੱਤੀ_2 = utf8nlookup(ਅੰਕੜੇ, hangul, s, len);
		ਜੇ (!ਪੱਤੀ_2) {
			*pos = s;
			ਵਾਪਸ 1;
		}
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ utf8_casefold_cmp(ਅਚਲ ਬਣਤਰ ext2fs_nls_ਸਾਰਣੀ *ਸਾਰਣੀ,
			     ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *str1, size_t len1,
			     ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *str2, size_t len2)
{
	ਅਚਲ ਬਣਤਰ utf8data *ਅੰਕੜੇ = utf8nfdicf(ਸਾਰਣੀ->ਸੰਸਕਰਣ);
	ਪੂਰਨ c1, c2;
	ਬਣਤਰ utf8cursor cur1, cur2;

	ਜੇ (utf8ncursor(&cur1, ਅੰਕੜੇ, (ਅਚਲ ਅੱਖਰ *) str1, len1) < 0)
		ਵਾਪਸ -1;
	ਜੇ (utf8ncursor(&cur2, ਅੰਕੜੇ, (ਅਚਲ ਅੱਖਰ *) str2, len2) < 0)
		ਵਾਪਸ -1;

	ਕਰੋ {
		c1 = utf8byte(&cur1);
		c2 = utf8byte(&cur2);

		ਜੇ (c1 < 0 || c2 < 0)
			ਵਾਪਸ -1;
		ਜੇ (c1 != c2)
			ਵਾਪਸ c1 - c2;
	} ਜਦੋਂ (c1);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ ext2fs_nls_ops utf8_ops = {
	.casefold = utf8_casefold,
	.validate = utf8_validate,
	.casefold_cmp = utf8_casefold_cmp,
};

ਸਥਿਰ ਅਚਲ ਬਣਤਰ ext2fs_nls_ਸਾਰਣੀ nls_utf8 = {
	.ops = &utf8_ops,
	.ਸੰਸਕਰਣ = UNICODE_AGE(12, 1, 0),
};

ਅਚਲ ਬਣਤਰ ext2fs_nls_ਸਾਰਣੀ *ext2fs_ਲੱਦੋ_nls_ਸਾਰਣੀ(ਪੂਰਨ encoding)
{
	ਜੇ (encoding == EXT4_ENC_UTF8_12_1)
		ਵਾਪਸ &nls_utf8;

	ਵਾਪਸ ਨਿਰਾ;
}

ਪੂਰਨ ext2fs_check_encoded_name(ਅਚਲ ਬਣਤਰ ext2fs_nls_ਸਾਰਣੀ *ਸਾਰਣੀ,
			      ਅੱਖਰ *name, size_t len, ਅੱਖਰ **pos)
{
	ਵਾਪਸ ਸਾਰਣੀ->ops->validate(ਸਾਰਣੀ, name, len, pos);
}

ਪੂਰਨ ext2fs_casefold_cmp(ਅਚਲ ਬਣਤਰ ext2fs_nls_ਸਾਰਣੀ *ਸਾਰਣੀ,
			ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *str1, size_t len1,
			ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *str2, size_t len2)
{
	ਵਾਪਸ ਸਾਰਣੀ->ops->casefold_cmp(ਸਾਰਣੀ, str1, len1, str2, len2);
}
