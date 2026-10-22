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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdint.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <limits.h>
#અન્તર્ભાવ <errno.h>

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"
#અન્તર્ભાવ "ext2fsP.h"

/* Encoding a unicode version number as a single unsigned int. */
#define UNICODE_MAJ_SHIFT		(16)
#define UNICODE_MIN_SHIFT		(8)

#define UNICODE_AGE(MAJ, MIN, REV)			\
	(((unsigned પૂર્ણાંક)(MAJ) << UNICODE_MAJ_SHIFT) |	\
	 ((unsigned પૂર્ણાંક)(MIN) << UNICODE_MIN_SHIFT) |	\
	 ((unsigned પૂર્ણાંક)(REV)))

/* Needed in struct utf8cursor below. */
#define UTF8HANGULLEAF	(12)

/*
 * Cursor structure used by the normalizer.
 */
સંરચના utf8cursor {
	અચલ સંરચના utf8data	*data;
	અચલ અક્ષર	*s;
	અચલ અક્ષર	*p;
	અચલ અક્ષર	*ss;
	અચલ અક્ષર	*sp;
	unsigned પૂર્ણાંક	len;
	unsigned પૂર્ણાંક	slen;
	short પૂર્ણાંક	ccc;
	short પૂર્ણાંક	nccc;
	unsigned અક્ષર	hangul[UTF8HANGULLEAF];
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


સંરચના utf8data {
	unsigned પૂર્ણાંક maxage;
	unsigned પૂર્ણાંક offset;
};

#define __INCLUDED_FROM_UTF8NORM_C__
#અન્તર્ભાવ "utf8data.h"
#undef __INCLUDED_FROM_UTF8NORM_C__

#define ARRAY_SIZE(array)			\
        (કદ(array) / કદ(array[0]))

#જો 0
/* Highest unicode version supported by the data tables. */
સ્થિર પૂર્ણાંક utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev)
{
	પૂર્ણાંક i = ARRAY_SIZE(utf8agetab) - 1;
	unsigned પૂર્ણાંક sb_utf8version = UNICODE_AGE(maj, min, rev);

	જ્યારે (i >= 0 && utf8agetab[i] != 0) {
		જો (sb_utf8version == utf8agetab[i])
			પાછા 1;
		i--;
	}
	પાછા 0;
}
#endif

#જો 0
સ્થિર પૂર્ણાંક utf8version_latest(શૂન્ય)
{
	પાછા utf8vers;
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
સ્થિર અંતઃરેખ પૂર્ણાંક utf8clen(અચલ અક્ષર *s)
{
	unsigned અક્ષર c = *s;

	પાછા 1 + (c >= 0xC0) + (c >= 0xE0) + (c >= 0xF0);
}

/*
 * Decode a 3-byte UTF-8 sequence.
 */
સ્થિર unsigned પૂર્ણાંક
utf8decode3(અચલ અક્ષર *str)
{
	unsigned પૂર્ણાંક		uc;

	uc = *str++ & 0x0F;
	uc <<= 6;
	uc |= *str++ & 0x3F;
	uc <<= 6;
	uc |= *str++ & 0x3F;

	પાછા uc;
}

/*
 * Encode a 3-byte UTF-8 sequence.
 */
સ્થિર પૂર્ણાંક
utf8encode3(અક્ષર *str, unsigned પૂર્ણાંક val)
{
	str[2] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[1] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[0] = val | 0xE0;

	પાછા 3;
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
પ્રકારવ્યાખ્યા અચલ unsigned અક્ષર utf8trie_t;
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
પ્રકારવ્યાખ્યા અચલ unsigned અક્ષર utf8leaf_t;

#define LEAF_GEN(LEAF)	((LEAF)[0])
#define LEAF_CCC(LEAF)	((LEAF)[1])
#define LEAF_STR(LEAF)	((અચલ અક્ષર *)((LEAF) + 2))

#define MINCCC		(0)
#define MAXCCC		(254)
#define STOPPER		(0)
#define	DECOMPOSE	(255)

/* Marker for hangul syllable decomposition. */
#define HANGUL		((અક્ષર)(255))
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
સ્થિર utf8leaf_t *
utf8hangul(અચલ અક્ષર *str, unsigned અક્ષર *hangul)
{
	unsigned પૂર્ણાંક	si;
	unsigned પૂર્ણાંક	li;
	unsigned પૂર્ણાંક	vi;
	unsigned પૂર્ણાંક	ti;
	unsigned અક્ષર	*h;

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
	h += utf8encode3((અક્ષર *)h, li + LB);

	/* Add VPart, a 3-byte UTF-8 sequence. */
	h += utf8encode3((અક્ષર *)h, vi + VB);

	/* Add TPart if required, also a 3-byte UTF-8 sequence. */
	જો (ti)
		h += utf8encode3((અક્ષર *)h, ti + TB);

	/* Terminate string. */
	h[0] = '\0';

	પાછા hangul;
}

/*
 * Use trie to scan s, touching at most len bytes.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * A non-NULL return guarantees that the UTF-8 sequence starting at s
 * is well-formed and corresponds to a known unicode code point.  The
 * shorthand for this will be "is valid UTF-8 unicode".
 */
સ્થિર utf8leaf_t *utf8nlookup(અચલ સંરચના utf8data *data,
			       unsigned અક્ષર *hangul, અચલ અક્ષર *s, size_t len)
{
	utf8trie_t	*trie;
	પૂર્ણાંક		offlen;
	પૂર્ણાંક		offset;
	પૂર્ણાંક		mask;
	પૂર્ણાંક		ગાંઠ;

	જો (!data)
		પાછા ખાલી;
	જો (len == 0)
		પાછા ખાલી;

	trie = utf8data + data->offset;
	ગાંઠ = 1;
	જ્યારે (ગાંઠ) {
		offlen = (*trie & OFFLEN) >> OFFLEN_SHIFT;
		જો (*trie & NEXTBYTE) {
			જો (--len == 0)
				પાછા ખાલી;
			s++;
		}
		mask = 1 << (*trie & BITNUM);
		જો (*s & mask) {
			/* Right leg */
			જો (offlen) {
				/* Right node at offset of trie */
				ગાંઠ = (*trie & RIGHTNODE);
				offset = trie[offlen];
				જ્યારે (--offlen) {
					offset <<= 8;
					offset |= trie[offlen];
				}
				trie += offset;
			} નહીંતર જો (*trie & RIGHTPATH) {
				/* Right node after this node */
				ગાંઠ = (*trie & TRIENODE);
				trie++;
			} નહીંતર {
				/* No right node. */
				પાછા ખાલી;
			}
		} નહીંતર {
			/* Left leg */
			જો (offlen) {
				/* Left node after this node. */
				ગાંઠ = (*trie & LEFTNODE);
				trie += offlen + 1;
			} નહીંતર જો (*trie & RIGHTPATH) {
				/* No left node. */
				પાછા ખાલી;
			} નહીંતર {
				/* Left node after this node */
				ગાંઠ = (*trie & TRIENODE);
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
	જો (LEAF_CCC(trie) == DECOMPOSE && LEAF_STR(trie)[0] == HANGUL)
		trie = utf8hangul(s - 2, hangul);
	પાછા trie;
}

/*
 * Use trie to scan s.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * Forwards to utf8nlookup().
 */
સ્થિર utf8leaf_t *utf8lookup(અચલ સંરચના utf8data *data,
			      unsigned અક્ષર *hangul, અચલ અક્ષર *s)
{
	પાછા utf8nlookup(data, hangul, s, (size_t)-1);
}

#જો 0
/*
 * Maximum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if only non-assigned code points are used.
 */
સ્થિર પૂર્ણાંક utf8agemax(અચલ સંરચના utf8data *data, અચલ અક્ષર *s)
{
	utf8leaf_t	*leaf;
	પૂર્ણાંક		age = 0;
	પૂર્ણાંક		leaf_age;
	unsigned અક્ષર	hangul[UTF8HANGULLEAF];

	જો (!data)
		પાછા -1;

	જ્યારે (*s) {
		leaf = utf8lookup(data, hangul, s);
		જો (!leaf)
			પાછા -1;

		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		જો (leaf_age <= data->maxage && leaf_age > age)
			age = leaf_age;
		s += utf8clen(s);
	}
	પાછા age;
}
#endif

#જો 0
/*
 * Minimum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if non-assigned code points are used.
 */
સ્થિર પૂર્ણાંક utf8agemin(અચલ સંરચના utf8data *data, અચલ અક્ષર *s)
{
	utf8leaf_t	*leaf;
	પૂર્ણાંક		age;
	પૂર્ણાંક		leaf_age;
	unsigned અક્ષર	hangul[UTF8HANGULLEAF];

	જો (!data)
		પાછા -1;
	age = data->maxage;
	જ્યારે (*s) {
		leaf = utf8lookup(data, hangul, s);
		જો (!leaf)
			પાછા -1;
		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		જો (leaf_age <= data->maxage && leaf_age < age)
			age = leaf_age;
		s += utf8clen(s);
	}
	પાછા age;
}
#endif

#જો 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
સ્થિર પૂર્ણાંક utf8nagemax(અચલ સંરચના utf8data *data, અચલ અક્ષર *s, size_t len)
{
	utf8leaf_t	*leaf;
	પૂર્ણાંક		age = 0;
	પૂર્ણાંક		leaf_age;
	unsigned અક્ષર	hangul[UTF8HANGULLEAF];

	જો (!data)
		પાછા -1;

	જ્યારે (len && *s) {
		leaf = utf8nlookup(data, hangul, s, len);
		જો (!leaf)
			પાછા -1;
		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		જો (leaf_age <= data->maxage && leaf_age > age)
			age = leaf_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	પાછા age;
}
#endif

#જો 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
સ્થિર પૂર્ણાંક utf8nagemin(અચલ સંરચના utf8data *data, અચલ અક્ષર *s, size_t len)
{
	utf8leaf_t	*leaf;
	પૂર્ણાંક		leaf_age;
	પૂર્ણાંક		age;
	unsigned અક્ષર	hangul[UTF8HANGULLEAF];

	જો (!data)
		પાછા -1;
	age = data->maxage;
	જ્યારે (len && *s) {
		leaf = utf8nlookup(data, hangul, s, len);
		જો (!leaf)
			પાછા -1;
		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		જો (leaf_age <= data->maxage && leaf_age < age)
			age = leaf_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	પાછા age;
}
#endif

#જો 0
/*
 * Length of the normalization of s.
 * Return -1 if s is not valid UTF-8 unicode.
 *
 * A string of Default_Ignorable_Code_Point has length 0.
 */
સ્થિર ssize_t utf8len(અચલ સંરચના utf8data *data, અચલ અક્ષર *s)
{
	utf8leaf_t	*leaf;
	size_t		ret = 0;
	unsigned અક્ષર	hangul[UTF8HANGULLEAF];

	જો (!data)
		પાછા -1;
	જ્યારે (*s) {
		leaf = utf8lookup(data, hangul, s);
		જો (!leaf)
			પાછા -1;
		જો (utf8agetab[LEAF_GEN(leaf)] > data->maxage)
			ret += utf8clen(s);
		નહીંતર જો (LEAF_CCC(leaf) == DECOMPOSE)
			ret += strlen(LEAF_STR(leaf));
		નહીંતર
			ret += utf8clen(s);
		s += utf8clen(s);
	}
	પાછા ret;
}
#endif

#જો 0
/*
 * Length of the normalization of s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
સ્થિર ssize_t utf8nlen(અચલ સંરચના utf8data *data, અચલ અક્ષર *s, size_t len)
{
	utf8leaf_t	*leaf;
	size_t		ret = 0;
	unsigned અક્ષર	hangul[UTF8HANGULLEAF];

	જો (!data)
		પાછા -1;
	જ્યારે (len && *s) {
		leaf = utf8nlookup(data, hangul, s, len);
		જો (!leaf)
			પાછા -1;
		જો (utf8agetab[LEAF_GEN(leaf)] > data->maxage)
			ret += utf8clen(s);
		નહીંતર જો (LEAF_CCC(leaf) == DECOMPOSE)
			ret += strlen(LEAF_STR(leaf));
		નહીંતર
			ret += utf8clen(s);
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	પાછા ret;
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
સ્થિર પૂર્ણાંક utf8ncursor(સંરચના utf8cursor *u8c, અચલ સંરચના utf8data *data,
		અચલ અક્ષર *s, size_t len)
{
	જો (!data)
		પાછા -1;
	જો (!s)
		પાછા -1;
	u8c->data = data;
	u8c->s = s;
	u8c->p = ખાલી;
	u8c->ss = ખાલી;
	u8c->sp = ખાલી;
	u8c->len = len;
	u8c->slen = 0;
	u8c->ccc = STOPPER;
	u8c->nccc = STOPPER;
	/* Check we didn't clobber the maximum length. */
	જો (u8c->len != len)
		પાછા -1;
	/* The first byte of s may not be an utf8 continuation. */
	જો (len > 0 && (*s & 0xC0) == 0x80)
		પાછા -1;
	પાછા 0;
}

#જો 0
/*
 * Set up an utf8cursor for use by utf8byte().
 *
 *   u8c    : pointer to cursor.
 *   data   : const struct utf8data to use for normalization.
 *   s      : NUL-terminated string.
 *
 * Returns -1 on error, 0 on success.
 */
સ્થિર પૂર્ણાંક utf8cursor(સંરચના utf8cursor *u8c, અચલ સંરચના utf8data *data,
	       અચલ અક્ષર *s)
{
	પાછા utf8ncursor(u8c, data, s, (unsigned પૂર્ણાંક)-1);
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
સ્થિર પૂર્ણાંક utf8byte(સંરચના utf8cursor *u8c)
{
	utf8leaf_t *leaf;
	પૂર્ણાંક ccc;

	માટે (;;) {
		/* Check for the end of a decomposed character. */
		જો (u8c->p && *u8c->s == '\0') {
			u8c->s = u8c->p;
			u8c->p = ખાલી;
		}

		/* Check for end-of-string. */
		જો (!u8c->p && (u8c->len == 0 || *u8c->s == '\0')) {
			/* There is no next byte. */
			જો (u8c->ccc == STOPPER)
				પાછા 0;
			/* End-of-string during a scan counts as a stopper. */
			ccc = STOPPER;
			જાઓ ccc_mismatch;
		} નહીંતર જો ((*u8c->s & 0xC0) == 0x80) {
			/* This is a continuation of the current character. */
			જો (!u8c->p)
				u8c->len--;
			પાછા (unsigned અક્ષર)*u8c->s++;
		}

		/* Look up the data for the current character. */
		જો (u8c->p) {
			leaf = utf8lookup(u8c->data, u8c->hangul, u8c->s);
		} નહીંતર {
			leaf = utf8nlookup(u8c->data, u8c->hangul,
					   u8c->s, u8c->len);
		}

		/* No leaf found implies that the input is a binary blob. */
		જો (!leaf)
			પાછા -1;

		ccc = LEAF_CCC(leaf);
		/* Characters that are too new have CCC 0. */
		જો (utf8agetab[LEAF_GEN(leaf)] > u8c->data->maxage) {
			ccc = STOPPER;
		} નહીંતર જો (ccc == DECOMPOSE) {
			u8c->len -= utf8clen(u8c->s);
			u8c->p = u8c->s + utf8clen(u8c->s);
			u8c->s = LEAF_STR(leaf);
			/* Empty decomposition implies CCC 0. */
			જો (*u8c->s == '\0') {
				જો (u8c->ccc == STOPPER)
					ચાલુ;
				ccc = STOPPER;
				જાઓ ccc_mismatch;
			}

			leaf = utf8lookup(u8c->data, u8c->hangul, u8c->s);
			જો (!leaf)
				પાછા -1;
			ccc = LEAF_CCC(leaf);
		}

		/*
		 * If this is not a stopper, then see if it updates
		 * the next canonical class to be emitted.
		 */
		જો (ccc != STOPPER && u8c->ccc < ccc && ccc < u8c->nccc)
			u8c->nccc = ccc;

		/*
		 * Return the current byte if this is the current
		 * combining class.
		 */
		જો (ccc == u8c->ccc) {
			જો (!u8c->p)
				u8c->len--;
			પાછા (unsigned અક્ષર)*u8c->s++;
		}

		/* Current combining class mismatch. */
ccc_mismatch:
		જો (u8c->nccc == STOPPER) {
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
			જો (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} નહીંતર જો (ccc != STOPPER) {
			/* Not a stopper, and not the ccc we're emitting. */
			જો (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} નહીંતર જો (u8c->nccc != MAXCCC + 1) {
			/* At a stopper, restart for next ccc. */
			u8c->ccc = u8c->nccc;
			u8c->nccc = MAXCCC + 1;
			u8c->s = u8c->ss;
			u8c->p = u8c->sp;
			u8c->len = u8c->slen;
		} નહીંતર {
			/* All done, proceed from here. */
			u8c->ccc = STOPPER;
			u8c->nccc = STOPPER;
			u8c->sp = ખાલી;
			u8c->ss = ખાલી;
			u8c->slen = 0;
		}
	}
}

#જો 0
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
સ્થિર અચલ સંરચના utf8data *utf8nfdi(unsigned પૂર્ણાંક maxage)
{
	પૂર્ણાંક i = ARRAY_SIZE(utf8nfdidata) - 1;

	જ્યારે (maxage < utf8nfdidata[i].maxage)
		i--;
	જો (maxage > utf8nfdidata[i].maxage)
		પાછા ખાલી;
	પાછા &utf8nfdidata[i];
}
#endif

સ્થિર અચલ સંરચના utf8data *utf8nfdicf(unsigned પૂર્ણાંક maxage)
{
	પૂર્ણાંક i = ARRAY_SIZE(utf8nfdicfdata) - 1;

	જ્યારે (maxage < utf8nfdicfdata[i].maxage)
		i--;
	જો (maxage > utf8nfdicfdata[i].maxage)
		પાછા ખાલી;
	પાછા &utf8nfdicfdata[i];
}

સ્થિર પૂર્ણાંક utf8_casefold(અચલ સંરચના ext2fs_nls_table *table,
			  અચલ unsigned અક્ષર *str, size_t len,
			  unsigned અક્ષર *dest, size_t dlen)
{
	અચલ સંરચના utf8data *data = utf8nfdicf(table->version);
	સંરચના utf8cursor cur;
	size_t nlen = 0;

	જો (utf8ncursor(&cur, data, (અચલ અક્ષર *) str, len) < 0)
		જાઓ invalid_seq;

	માટે (nlen = 0; nlen < dlen; nlen++) {
		પૂર્ણાંક c = utf8byte(&cur);

		dest[nlen] = c;
		જો (!c)
			પાછા nlen;
		જો (c == -1)
			તોડો;
	}

	પાછા -ENAMETOOLONG;

invalid_seq:
	જો (dlen < len)
		પાછા -ENAMETOOLONG;

	/* Signal invalid sequence */
	પાછા -EINVAL;
}

સ્થિર પૂર્ણાંક utf8_validate(અચલ સંરચના ext2fs_nls_table *table,
			 અક્ષર *s, size_t len, અક્ષર **pos)
{
	અચલ સંરચના utf8data *data = utf8nfdicf(table->version);
	utf8leaf_t	*leaf;
	unsigned અક્ષર	hangul[UTF8HANGULLEAF];

	જો (!data)
		પાછા -1;
	જ્યારે (len && *s) {
		leaf = utf8nlookup(data, hangul, s, len);
		જો (!leaf) {
			*pos = s;
			પાછા 1;
		}
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	પાછા 0;
}

સ્થિર પૂર્ણાંક utf8_casefold_cmp(અચલ સંરચના ext2fs_nls_table *table,
			     અચલ unsigned અક્ષર *str1, size_t len1,
			     અચલ unsigned અક્ષર *str2, size_t len2)
{
	અચલ સંરચના utf8data *data = utf8nfdicf(table->version);
	પૂર્ણાંક c1, c2;
	સંરચના utf8cursor cur1, cur2;

	જો (utf8ncursor(&cur1, data, (અચલ અક્ષર *) str1, len1) < 0)
		પાછા -1;
	જો (utf8ncursor(&cur2, data, (અચલ અક્ષર *) str2, len2) < 0)
		પાછા -1;

	કરો {
		c1 = utf8byte(&cur1);
		c2 = utf8byte(&cur2);

		જો (c1 < 0 || c2 < 0)
			પાછા -1;
		જો (c1 != c2)
			પાછા c1 - c2;
	} જ્યારે (c1);

	પાછા 0;
}

સ્થિર અચલ સંરચના ext2fs_nls_ops utf8_ops = {
	.casefold = utf8_casefold,
	.validate = utf8_validate,
	.casefold_cmp = utf8_casefold_cmp,
};

સ્થિર અચલ સંરચના ext2fs_nls_table nls_utf8 = {
	.ops = &utf8_ops,
	.version = UNICODE_AGE(12, 1, 0),
};

અચલ સંરચના ext2fs_nls_table *ext2fs_load_nls_table(પૂર્ણાંક encoding)
{
	જો (encoding == EXT4_ENC_UTF8_12_1)
		પાછા &nls_utf8;

	પાછા ખાલી;
}

પૂર્ણાંક ext2fs_check_encoded_name(અચલ સંરચના ext2fs_nls_table *table,
			      અક્ષર *name, size_t len, અક્ષર **pos)
{
	પાછા table->ops->validate(table, name, len, pos);
}

પૂર્ણાંક ext2fs_casefold_cmp(અચલ સંરચના ext2fs_nls_table *table,
			અચલ unsigned અક્ષર *str1, size_t len1,
			અચલ unsigned અક્ષર *str2, size_t len2)
{
	પાછા table->ops->casefold_cmp(table, str1, len1, str2, len2);
}
