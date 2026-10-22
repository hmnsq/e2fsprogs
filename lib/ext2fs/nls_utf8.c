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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <limits.h>
#അന്തര്ഭാവ <errno.h>

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

/* Encoding a unicode version number as a single unsigned int. */
#define UNICODE_MAJ_SHIFT		(16)
#define UNICODE_MIN_SHIFT		(8)

#define UNICODE_AGE(MAJ, MIN, REV)			\
	(((unsigned പൂർണ്ണസംഖ്യ)(MAJ) << UNICODE_MAJ_SHIFT) |	\
	 ((unsigned പൂർണ്ണസംഖ്യ)(MIN) << UNICODE_MIN_SHIFT) |	\
	 ((unsigned പൂർണ്ണസംഖ്യ)(REV)))

/* Needed in struct utf8cursor below. */
#define UTF8HANGULLEAF	(12)

/*
 * Cursor structure used by the normalizer.
 */
ഘടന utf8cursor {
	സ്ഥിരം ഘടന utf8data	*data;
	സ്ഥിരം അക്ഷരം	*s;
	സ്ഥിരം അക്ഷരം	*p;
	സ്ഥിരം അക്ഷരം	*ss;
	സ്ഥിരം അക്ഷരം	*sp;
	unsigned പൂർണ്ണസംഖ്യ	len;
	unsigned പൂർണ്ണസംഖ്യ	slen;
	short പൂർണ്ണസംഖ്യ	ccc;
	short പൂർണ്ണസംഖ്യ	nccc;
	unsigned അക്ഷരം	hangul[UTF8HANGULLEAF];
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


ഘടന utf8data {
	unsigned പൂർണ്ണസംഖ്യ maxage;
	unsigned പൂർണ്ണസംഖ്യ offset;
};

#define __INCLUDED_FROM_UTF8NORM_C__
#അന്തര്ഭാവ "utf8data.h"
#undef __INCLUDED_FROM_UTF8NORM_C__

#define ARRAY_SIZE(array)			\
        (വലുപ്പം(array) / വലുപ്പം(array[0]))

#എങ്കിൽ 0
/* Highest unicode version supported by the data tables. */
സ്ഥിതം പൂർണ്ണസംഖ്യ utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev)
{
	പൂർണ്ണസംഖ്യ i = ARRAY_SIZE(utf8agetab) - 1;
	unsigned പൂർണ്ണസംഖ്യ sb_utf8version = UNICODE_AGE(maj, min, rev);

	ആകുമ്പോൾ (i >= 0 && utf8agetab[i] != 0) {
		എങ്കിൽ (sb_utf8version == utf8agetab[i])
			തിരികെ 1;
		i--;
	}
	തിരികെ 0;
}
#endif

#എങ്കിൽ 0
സ്ഥിതം പൂർണ്ണസംഖ്യ utf8version_latest(ശൂന്യം)
{
	തിരികെ utf8vers;
}
#endif

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
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ utf8clen(സ്ഥിരം അക്ഷരം *s)
{
	unsigned അക്ഷരം c = *s;

	തിരികെ 1 + (c >= 0xC0) + (c >= 0xE0) + (c >= 0xF0);
}

/*
 * Decode a 3-byte UTF-8 sequence.
 */
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ
utf8decode3(സ്ഥിരം അക്ഷരം *str)
{
	unsigned പൂർണ്ണസംഖ്യ		uc;

	uc = *str++ & 0x0F;
	uc <<= 6;
	uc |= *str++ & 0x3F;
	uc <<= 6;
	uc |= *str++ & 0x3F;

	തിരികെ uc;
}

/*
 * Encode a 3-byte UTF-8 sequence.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ
utf8encode3(അക്ഷരം *str, unsigned പൂർണ്ണസംഖ്യ val)
{
	str[2] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[1] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[0] = val | 0xE0;

	തിരികെ 3;
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
തരനിർവചനം സ്ഥിരം unsigned അക്ഷരം utf8trie_t;
#define BITNUM		0x07
#define NEXTBYTE	0x08
#define OFFLEN		0x30
#define OFFLEN_SHIFT	4
#define RIGHTPATH	0x40
#define TRIENODE	0x80
#define RIGHTNODE	0x40
#define LEFTNODE	0x80

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
തരനിർവചനം സ്ഥിരം unsigned അക്ഷരം utf8leaf_t;

#define LEAF_GEN(LEAF)	((LEAF)[0])
#define LEAF_CCC(LEAF)	((LEAF)[1])
#define LEAF_STR(LEAF)	((സ്ഥിരം അക്ഷരം *)((LEAF) + 2))

#define MINCCC		(0)
#define MAXCCC		(254)
#define STOPPER		(0)
#define	DECOMPOSE	(255)

/* Marker for hangul syllable decomposition. */
#define HANGUL		((അക്ഷരം)(255))
/* Size of the synthesized leaf used for Hangul syllable decomposition. */
#define UTF8HANGULLEAF	(12)

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
#define SB	(0xAC00)
#define LB	(0x1100)
#define VB	(0x1161)
#define TB	(0x11A7)
#define LC	(19)
#define VC	(21)
#define TC	(28)
#define NC	(VC * TC)
#define SC	(LC * NC)

/* Algorithmic decomposition of hangul syllable. */
സ്ഥിതം utf8leaf_t *
utf8hangul(സ്ഥിരം അക്ഷരം *str, unsigned അക്ഷരം *hangul)
{
	unsigned പൂർണ്ണസംഖ്യ	si;
	unsigned പൂർണ്ണസംഖ്യ	li;
	unsigned പൂർണ്ണസംഖ്യ	vi;
	unsigned പൂർണ്ണസംഖ്യ	ti;
	unsigned അക്ഷരം	*h;

	/* Calculate the SI, LI, VI, and TI values. */
	si = utf8decode3(str) - SB;
	li = si / NC;
	vi = (si % NC) / TC;
	ti = si % TC;

	/* Fill in base of leaf. */
	h = hangul;
	LEAF_GEN(h) = 2;
	LEAF_CCC(h) = DECOMPOSE;
	h += 2;

	/* Add LPart, a 3-byte UTF-8 sequence. */
	h += utf8encode3((അക്ഷരം *)h, li + LB);

	/* Add VPart, a 3-byte UTF-8 sequence. */
	h += utf8encode3((അക്ഷരം *)h, vi + VB);

	/* Add TPart if required, also a 3-byte UTF-8 sequence. */
	എങ്കിൽ (ti)
		h += utf8encode3((അക്ഷരം *)h, ti + TB);

	/* Terminate string. */
	h[0] = '\0';

	തിരികെ hangul;
}

/*
 * Use trie to scan s, touching at most len bytes.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * A non-NULL return guarantees that the UTF-8 sequence starting at s
 * is well-formed and corresponds to a known unicode code point.  The
 * shorthand for this will be "is valid UTF-8 unicode".
 */
സ്ഥിതം utf8leaf_t *utf8nlookup(സ്ഥിരം ഘടന utf8data *data,
			       unsigned അക്ഷരം *hangul, സ്ഥിരം അക്ഷരം *s, size_t len)
{
	utf8trie_t	*trie;
	പൂർണ്ണസംഖ്യ		offlen;
	പൂർണ്ണസംഖ്യ		offset;
	പൂർണ്ണസംഖ്യ		mask;
	പൂർണ്ണസംഖ്യ		കണ്ണി;

	എങ്കിൽ (!data)
		തിരികെ ശൂന്യ;
	എങ്കിൽ (len == 0)
		തിരികെ ശൂന്യ;

	trie = utf8data + data->offset;
	കണ്ണി = 1;
	ആകുമ്പോൾ (കണ്ണി) {
		offlen = (*trie & OFFLEN) >> OFFLEN_SHIFT;
		എങ്കിൽ (*trie & NEXTBYTE) {
			എങ്കിൽ (--len == 0)
				തിരികെ ശൂന്യ;
			s++;
		}
		mask = 1 << (*trie & BITNUM);
		എങ്കിൽ (*s & mask) {
			/* Right leg */
			എങ്കിൽ (offlen) {
				/* Right node at offset of trie */
				കണ്ണി = (*trie & RIGHTNODE);
				offset = trie[offlen];
				ആകുമ്പോൾ (--offlen) {
					offset <<= 8;
					offset |= trie[offlen];
				}
				trie += offset;
			} അല്ലെങ്കിൽ എങ്കിൽ (*trie & RIGHTPATH) {
				/* Right node after this node */
				കണ്ണി = (*trie & TRIENODE);
				trie++;
			} അല്ലെങ്കിൽ {
				/* No right node. */
				തിരികെ ശൂന്യ;
			}
		} അല്ലെങ്കിൽ {
			/* Left leg */
			എങ്കിൽ (offlen) {
				/* Left node after this node. */
				കണ്ണി = (*trie & LEFTNODE);
				trie += offlen + 1;
			} അല്ലെങ്കിൽ എങ്കിൽ (*trie & RIGHTPATH) {
				/* No left node. */
				തിരികെ ശൂന്യ;
			} അല്ലെങ്കിൽ {
				/* Left node after this node */
				കണ്ണി = (*trie & TRIENODE);
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
	എങ്കിൽ (LEAF_CCC(trie) == DECOMPOSE && LEAF_STR(trie)[0] == HANGUL)
		trie = utf8hangul(s - 2, hangul);
	തിരികെ trie;
}

/*
 * Use trie to scan s.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * Forwards to utf8nlookup().
 */
സ്ഥിതം utf8leaf_t *utf8lookup(സ്ഥിരം ഘടന utf8data *data,
			      unsigned അക്ഷരം *hangul, സ്ഥിരം അക്ഷരം *s)
{
	തിരികെ utf8nlookup(data, hangul, s, (size_t)-1);
}

#എങ്കിൽ 0
/*
 * Maximum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if only non-assigned code points are used.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ utf8agemax(സ്ഥിരം ഘടന utf8data *data, സ്ഥിരം അക്ഷരം *s)
{
	utf8leaf_t	*leaf;
	പൂർണ്ണസംഖ്യ		age = 0;
	പൂർണ്ണസംഖ്യ		leaf_age;
	unsigned അക്ഷരം	hangul[UTF8HANGULLEAF];

	എങ്കിൽ (!data)
		തിരികെ -1;

	ആകുമ്പോൾ (*s) {
		leaf = utf8lookup(data, hangul, s);
		എങ്കിൽ (!leaf)
			തിരികെ -1;

		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		എങ്കിൽ (leaf_age <= data->maxage && leaf_age > age)
			age = leaf_age;
		s += utf8clen(s);
	}
	തിരികെ age;
}
#endif

#എങ്കിൽ 0
/*
 * Minimum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if non-assigned code points are used.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ utf8agemin(സ്ഥിരം ഘടന utf8data *data, സ്ഥിരം അക്ഷരം *s)
{
	utf8leaf_t	*leaf;
	പൂർണ്ണസംഖ്യ		age;
	പൂർണ്ണസംഖ്യ		leaf_age;
	unsigned അക്ഷരം	hangul[UTF8HANGULLEAF];

	എങ്കിൽ (!data)
		തിരികെ -1;
	age = data->maxage;
	ആകുമ്പോൾ (*s) {
		leaf = utf8lookup(data, hangul, s);
		എങ്കിൽ (!leaf)
			തിരികെ -1;
		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		എങ്കിൽ (leaf_age <= data->maxage && leaf_age < age)
			age = leaf_age;
		s += utf8clen(s);
	}
	തിരികെ age;
}
#endif

#എങ്കിൽ 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ utf8nagemax(സ്ഥിരം ഘടന utf8data *data, സ്ഥിരം അക്ഷരം *s, size_t len)
{
	utf8leaf_t	*leaf;
	പൂർണ്ണസംഖ്യ		age = 0;
	പൂർണ്ണസംഖ്യ		leaf_age;
	unsigned അക്ഷരം	hangul[UTF8HANGULLEAF];

	എങ്കിൽ (!data)
		തിരികെ -1;

	ആകുമ്പോൾ (len && *s) {
		leaf = utf8nlookup(data, hangul, s, len);
		എങ്കിൽ (!leaf)
			തിരികെ -1;
		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		എങ്കിൽ (leaf_age <= data->maxage && leaf_age > age)
			age = leaf_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	തിരികെ age;
}
#endif

#എങ്കിൽ 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ utf8nagemin(സ്ഥിരം ഘടന utf8data *data, സ്ഥിരം അക്ഷരം *s, size_t len)
{
	utf8leaf_t	*leaf;
	പൂർണ്ണസംഖ്യ		leaf_age;
	പൂർണ്ണസംഖ്യ		age;
	unsigned അക്ഷരം	hangul[UTF8HANGULLEAF];

	എങ്കിൽ (!data)
		തിരികെ -1;
	age = data->maxage;
	ആകുമ്പോൾ (len && *s) {
		leaf = utf8nlookup(data, hangul, s, len);
		എങ്കിൽ (!leaf)
			തിരികെ -1;
		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		എങ്കിൽ (leaf_age <= data->maxage && leaf_age < age)
			age = leaf_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	തിരികെ age;
}
#endif

#എങ്കിൽ 0
/*
 * Length of the normalization of s.
 * Return -1 if s is not valid UTF-8 unicode.
 *
 * A string of Default_Ignorable_Code_Point has length 0.
 */
സ്ഥിതം ssize_t utf8len(സ്ഥിരം ഘടന utf8data *data, സ്ഥിരം അക്ഷരം *s)
{
	utf8leaf_t	*leaf;
	size_t		ret = 0;
	unsigned അക്ഷരം	hangul[UTF8HANGULLEAF];

	എങ്കിൽ (!data)
		തിരികെ -1;
	ആകുമ്പോൾ (*s) {
		leaf = utf8lookup(data, hangul, s);
		എങ്കിൽ (!leaf)
			തിരികെ -1;
		എങ്കിൽ (utf8agetab[LEAF_GEN(leaf)] > data->maxage)
			ret += utf8clen(s);
		അല്ലെങ്കിൽ എങ്കിൽ (LEAF_CCC(leaf) == DECOMPOSE)
			ret += strlen(LEAF_STR(leaf));
		അല്ലെങ്കിൽ
			ret += utf8clen(s);
		s += utf8clen(s);
	}
	തിരികെ ret;
}
#endif

#എങ്കിൽ 0
/*
 * Length of the normalization of s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
സ്ഥിതം ssize_t utf8nlen(സ്ഥിരം ഘടന utf8data *data, സ്ഥിരം അക്ഷരം *s, size_t len)
{
	utf8leaf_t	*leaf;
	size_t		ret = 0;
	unsigned അക്ഷരം	hangul[UTF8HANGULLEAF];

	എങ്കിൽ (!data)
		തിരികെ -1;
	ആകുമ്പോൾ (len && *s) {
		leaf = utf8nlookup(data, hangul, s, len);
		എങ്കിൽ (!leaf)
			തിരികെ -1;
		എങ്കിൽ (utf8agetab[LEAF_GEN(leaf)] > data->maxage)
			ret += utf8clen(s);
		അല്ലെങ്കിൽ എങ്കിൽ (LEAF_CCC(leaf) == DECOMPOSE)
			ret += strlen(LEAF_STR(leaf));
		അല്ലെങ്കിൽ
			ret += utf8clen(s);
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	തിരികെ ret;
}
#endif

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
സ്ഥിതം പൂർണ്ണസംഖ്യ utf8ncursor(ഘടന utf8cursor *u8c, സ്ഥിരം ഘടന utf8data *data,
		സ്ഥിരം അക്ഷരം *s, size_t len)
{
	എങ്കിൽ (!data)
		തിരികെ -1;
	എങ്കിൽ (!s)
		തിരികെ -1;
	u8c->data = data;
	u8c->s = s;
	u8c->p = ശൂന്യ;
	u8c->ss = ശൂന്യ;
	u8c->sp = ശൂന്യ;
	u8c->len = len;
	u8c->slen = 0;
	u8c->ccc = STOPPER;
	u8c->nccc = STOPPER;
	/* Check we didn't clobber the maximum length. */
	എങ്കിൽ (u8c->len != len)
		തിരികെ -1;
	/* The first byte of s may not be an utf8 continuation. */
	എങ്കിൽ (len > 0 && (*s & 0xC0) == 0x80)
		തിരികെ -1;
	തിരികെ 0;
}

#എങ്കിൽ 0
/*
 * Set up an utf8cursor for use by utf8byte().
 *
 *   u8c    : pointer to cursor.
 *   data   : const struct utf8data to use for normalization.
 *   s      : NUL-terminated string.
 *
 * Returns -1 on error, 0 on success.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ utf8cursor(ഘടന utf8cursor *u8c, സ്ഥിരം ഘടന utf8data *data,
	       സ്ഥിരം അക്ഷരം *s)
{
	തിരികെ utf8ncursor(u8c, data, s, (unsigned പൂർണ്ണസംഖ്യ)-1);
}
#endif

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
സ്ഥിതം പൂർണ്ണസംഖ്യ utf8byte(ഘടന utf8cursor *u8c)
{
	utf8leaf_t *leaf;
	പൂർണ്ണസംഖ്യ ccc;

	വേണ്ടി (;;) {
		/* Check for the end of a decomposed character. */
		എങ്കിൽ (u8c->p && *u8c->s == '\0') {
			u8c->s = u8c->p;
			u8c->p = ശൂന്യ;
		}

		/* Check for end-of-string. */
		എങ്കിൽ (!u8c->p && (u8c->len == 0 || *u8c->s == '\0')) {
			/* There is no next byte. */
			എങ്കിൽ (u8c->ccc == STOPPER)
				തിരികെ 0;
			/* End-of-string during a scan counts as a stopper. */
			ccc = STOPPER;
			പോകുക ccc_mismatch;
		} അല്ലെങ്കിൽ എങ്കിൽ ((*u8c->s & 0xC0) == 0x80) {
			/* This is a continuation of the current character. */
			എങ്കിൽ (!u8c->p)
				u8c->len--;
			തിരികെ (unsigned അക്ഷരം)*u8c->s++;
		}

		/* Look up the data for the current character. */
		എങ്കിൽ (u8c->p) {
			leaf = utf8lookup(u8c->data, u8c->hangul, u8c->s);
		} അല്ലെങ്കിൽ {
			leaf = utf8nlookup(u8c->data, u8c->hangul,
					   u8c->s, u8c->len);
		}

		/* No leaf found implies that the input is a binary blob. */
		എങ്കിൽ (!leaf)
			തിരികെ -1;

		ccc = LEAF_CCC(leaf);
		/* Characters that are too new have CCC 0. */
		എങ്കിൽ (utf8agetab[LEAF_GEN(leaf)] > u8c->data->maxage) {
			ccc = STOPPER;
		} അല്ലെങ്കിൽ എങ്കിൽ (ccc == DECOMPOSE) {
			u8c->len -= utf8clen(u8c->s);
			u8c->p = u8c->s + utf8clen(u8c->s);
			u8c->s = LEAF_STR(leaf);
			/* Empty decomposition implies CCC 0. */
			എങ്കിൽ (*u8c->s == '\0') {
				എങ്കിൽ (u8c->ccc == STOPPER)
					തുടരുക;
				ccc = STOPPER;
				പോകുക ccc_mismatch;
			}

			leaf = utf8lookup(u8c->data, u8c->hangul, u8c->s);
			എങ്കിൽ (!leaf)
				തിരികെ -1;
			ccc = LEAF_CCC(leaf);
		}

		/*
		 * If this is not a stopper, then see if it updates
		 * the next canonical class to be emitted.
		 */
		എങ്കിൽ (ccc != STOPPER && u8c->ccc < ccc && ccc < u8c->nccc)
			u8c->nccc = ccc;

		/*
		 * Return the current byte if this is the current
		 * combining class.
		 */
		എങ്കിൽ (ccc == u8c->ccc) {
			എങ്കിൽ (!u8c->p)
				u8c->len--;
			തിരികെ (unsigned അക്ഷരം)*u8c->s++;
		}

		/* Current combining class mismatch. */
ccc_mismatch:
		എങ്കിൽ (u8c->nccc == STOPPER) {
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
			എങ്കിൽ (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} അല്ലെങ്കിൽ എങ്കിൽ (ccc != STOPPER) {
			/* Not a stopper, and not the ccc we're emitting. */
			എങ്കിൽ (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} അല്ലെങ്കിൽ എങ്കിൽ (u8c->nccc != MAXCCC + 1) {
			/* At a stopper, restart for next ccc. */
			u8c->ccc = u8c->nccc;
			u8c->nccc = MAXCCC + 1;
			u8c->s = u8c->ss;
			u8c->p = u8c->sp;
			u8c->len = u8c->slen;
		} അല്ലെങ്കിൽ {
			/* All done, proceed from here. */
			u8c->ccc = STOPPER;
			u8c->nccc = STOPPER;
			u8c->sp = ശൂന്യ;
			u8c->ss = ശൂന്യ;
			u8c->slen = 0;
		}
	}
}

#എങ്കിൽ 0
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
സ്ഥിതം സ്ഥിരം ഘടന utf8data *utf8nfdi(unsigned പൂർണ്ണസംഖ്യ maxage)
{
	പൂർണ്ണസംഖ്യ i = ARRAY_SIZE(utf8nfdidata) - 1;

	ആകുമ്പോൾ (maxage < utf8nfdidata[i].maxage)
		i--;
	എങ്കിൽ (maxage > utf8nfdidata[i].maxage)
		തിരികെ ശൂന്യ;
	തിരികെ &utf8nfdidata[i];
}
#endif

സ്ഥിതം സ്ഥിരം ഘടന utf8data *utf8nfdicf(unsigned പൂർണ്ണസംഖ്യ maxage)
{
	പൂർണ്ണസംഖ്യ i = ARRAY_SIZE(utf8nfdicfdata) - 1;

	ആകുമ്പോൾ (maxage < utf8nfdicfdata[i].maxage)
		i--;
	എങ്കിൽ (maxage > utf8nfdicfdata[i].maxage)
		തിരികെ ശൂന്യ;
	തിരികെ &utf8nfdicfdata[i];
}

സ്ഥിതം പൂർണ്ണസംഖ്യ utf8_casefold(സ്ഥിരം ഘടന ext2fs_nls_table *table,
			  സ്ഥിരം unsigned അക്ഷരം *str, size_t len,
			  unsigned അക്ഷരം *dest, size_t dlen)
{
	സ്ഥിരം ഘടന utf8data *data = utf8nfdicf(table->version);
	ഘടന utf8cursor cur;
	size_t nlen = 0;

	എങ്കിൽ (utf8ncursor(&cur, data, (സ്ഥിരം അക്ഷരം *) str, len) < 0)
		പോകുക invalid_seq;

	വേണ്ടി (nlen = 0; nlen < dlen; nlen++) {
		പൂർണ്ണസംഖ്യ c = utf8byte(&cur);

		dest[nlen] = c;
		എങ്കിൽ (!c)
			തിരികെ nlen;
		എങ്കിൽ (c == -1)
			നിർത്തുക;
	}

	തിരികെ -ENAMETOOLONG;

invalid_seq:
	എങ്കിൽ (dlen < len)
		തിരികെ -ENAMETOOLONG;

	/* Signal invalid sequence */
	തിരികെ -EINVAL;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ utf8_validate(സ്ഥിരം ഘടന ext2fs_nls_table *table,
			 അക്ഷരം *s, size_t len, അക്ഷരം **pos)
{
	സ്ഥിരം ഘടന utf8data *data = utf8nfdicf(table->version);
	utf8leaf_t	*leaf;
	unsigned അക്ഷരം	hangul[UTF8HANGULLEAF];

	എങ്കിൽ (!data)
		തിരികെ -1;
	ആകുമ്പോൾ (len && *s) {
		leaf = utf8nlookup(data, hangul, s, len);
		എങ്കിൽ (!leaf) {
			*pos = s;
			തിരികെ 1;
		}
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ utf8_casefold_cmp(സ്ഥിരം ഘടന ext2fs_nls_table *table,
			     സ്ഥിരം unsigned അക്ഷരം *str1, size_t len1,
			     സ്ഥിരം unsigned അക്ഷരം *str2, size_t len2)
{
	സ്ഥിരം ഘടന utf8data *data = utf8nfdicf(table->version);
	പൂർണ്ണസംഖ്യ c1, c2;
	ഘടന utf8cursor cur1, cur2;

	എങ്കിൽ (utf8ncursor(&cur1, data, (സ്ഥിരം അക്ഷരം *) str1, len1) < 0)
		തിരികെ -1;
	എങ്കിൽ (utf8ncursor(&cur2, data, (സ്ഥിരം അക്ഷരം *) str2, len2) < 0)
		തിരികെ -1;

	ചെയ്യുക {
		c1 = utf8byte(&cur1);
		c2 = utf8byte(&cur2);

		എങ്കിൽ (c1 < 0 || c2 < 0)
			തിരികെ -1;
		എങ്കിൽ (c1 != c2)
			തിരികെ c1 - c2;
	} ആകുമ്പോൾ (c1);

	തിരികെ 0;
}

സ്ഥിതം സ്ഥിരം ഘടന ext2fs_nls_ops utf8_ops = {
	.casefold = utf8_casefold,
	.validate = utf8_validate,
	.casefold_cmp = utf8_casefold_cmp,
};

സ്ഥിതം സ്ഥിരം ഘടന ext2fs_nls_table nls_utf8 = {
	.ops = &utf8_ops,
	.version = UNICODE_AGE(12, 1, 0),
};

സ്ഥിരം ഘടന ext2fs_nls_table *ext2fs_load_nls_table(പൂർണ്ണസംഖ്യ encoding)
{
	എങ്കിൽ (encoding == EXT4_ENC_UTF8_12_1)
		തിരികെ &nls_utf8;

	തിരികെ ശൂന്യ;
}

പൂർണ്ണസംഖ്യ ext2fs_check_encoded_name(സ്ഥിരം ഘടന ext2fs_nls_table *table,
			      അക്ഷരം *name, size_t len, അക്ഷരം **pos)
{
	തിരികെ table->ops->validate(table, name, len, pos);
}

പൂർണ്ണസംഖ്യ ext2fs_casefold_cmp(സ്ഥിരം ഘടന ext2fs_nls_table *table,
			സ്ഥിരം unsigned അക്ഷരം *str1, size_t len1,
			സ്ഥിരം unsigned അക്ഷരം *str2, size_t len2)
{
	തിരികെ table->ops->casefold_cmp(table, str1, len1, str2, len2);
}
