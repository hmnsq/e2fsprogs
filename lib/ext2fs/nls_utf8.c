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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <limits.h>
#அந்தர்பாவ <errno.h>

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"
#அந்தர்பாவ "ext2fsP.h"

/* Encoding a unicode version number as a single unsigned int. */
#define UNICODE_MAJ_SHIFT		(16)
#define UNICODE_MIN_SHIFT		(8)

#define UNICODE_AGE(MAJ, MIN, REV)			\
	(((unsigned முழு)(MAJ) << UNICODE_MAJ_SHIFT) |	\
	 ((unsigned முழு)(MIN) << UNICODE_MIN_SHIFT) |	\
	 ((unsigned முழு)(REV)))

/* Needed in struct utf8cursor below. */
#define UTF8HANGULLEAF	(12)

/*
 * Cursor structure used by the normalizer.
 */
கட்டமைப்பு utf8cursor {
	மாறிலி கட்டமைப்பு utf8data	*data;
	மாறிலி எழுத்து	*s;
	மாறிலி எழுத்து	*p;
	மாறிலி எழுத்து	*ss;
	மாறிலி எழுத்து	*sp;
	unsigned முழு	len;
	unsigned முழு	slen;
	short முழு	ccc;
	short முழு	nccc;
	unsigned எழுத்து	hangul[UTF8HANGULLEAF];
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


கட்டமைப்பு utf8data {
	unsigned முழு maxage;
	unsigned முழு offset;
};

#define __INCLUDED_FROM_UTF8NORM_C__
#அந்தர்பாவ "utf8data.h"
#undef __INCLUDED_FROM_UTF8NORM_C__

#define ARRAY_SIZE(array)			\
        (அளவு(array) / அளவு(array[0]))

#எனில் 0
/* Highest unicode version supported by the data tables. */
நிலையான முழு utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev)
{
	முழு i = ARRAY_SIZE(utf8agetab) - 1;
	unsigned முழு sb_utf8version = UNICODE_AGE(maj, min, rev);

	போது (i >= 0 && utf8agetab[i] != 0) {
		எனில் (sb_utf8version == utf8agetab[i])
			திரும்பு 1;
		i--;
	}
	திரும்பு 0;
}
#endif

#எனில் 0
நிலையான முழு utf8version_latest(வெறுமை)
{
	திரும்பு utf8vers;
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
நிலையான உள்வரி முழு utf8clen(மாறிலி எழுத்து *s)
{
	unsigned எழுத்து c = *s;

	திரும்பு 1 + (c >= 0xC0) + (c >= 0xE0) + (c >= 0xF0);
}

/*
 * Decode a 3-byte UTF-8 sequence.
 */
நிலையான unsigned முழு
utf8decode3(மாறிலி எழுத்து *str)
{
	unsigned முழு		uc;

	uc = *str++ & 0x0F;
	uc <<= 6;
	uc |= *str++ & 0x3F;
	uc <<= 6;
	uc |= *str++ & 0x3F;

	திரும்பு uc;
}

/*
 * Encode a 3-byte UTF-8 sequence.
 */
நிலையான முழு
utf8encode3(எழுத்து *str, unsigned முழு val)
{
	str[2] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[1] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[0] = val | 0xE0;

	திரும்பு 3;
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
வகைவரை மாறிலி unsigned எழுத்து utf8trie_t;
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
வகைவரை மாறிலி unsigned எழுத்து utf8leaf_t;

#define LEAF_GEN(LEAF)	((LEAF)[0])
#define LEAF_CCC(LEAF)	((LEAF)[1])
#define LEAF_STR(LEAF)	((மாறிலி எழுத்து *)((LEAF) + 2))

#define MINCCC		(0)
#define MAXCCC		(254)
#define STOPPER		(0)
#define	DECOMPOSE	(255)

/* Marker for hangul syllable decomposition. */
#define HANGUL		((எழுத்து)(255))
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
நிலையான utf8leaf_t *
utf8hangul(மாறிலி எழுத்து *str, unsigned எழுத்து *hangul)
{
	unsigned முழு	si;
	unsigned முழு	li;
	unsigned முழு	vi;
	unsigned முழு	ti;
	unsigned எழுத்து	*h;

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
	h += utf8encode3((எழுத்து *)h, li + LB);

	/* Add VPart, a 3-byte UTF-8 sequence. */
	h += utf8encode3((எழுத்து *)h, vi + VB);

	/* Add TPart if required, also a 3-byte UTF-8 sequence. */
	எனில் (ti)
		h += utf8encode3((எழுத்து *)h, ti + TB);

	/* Terminate string. */
	h[0] = '\0';

	திரும்பு hangul;
}

/*
 * Use trie to scan s, touching at most len bytes.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * A non-NULL return guarantees that the UTF-8 sequence starting at s
 * is well-formed and corresponds to a known unicode code point.  The
 * shorthand for this will be "is valid UTF-8 unicode".
 */
நிலையான utf8leaf_t *utf8nlookup(மாறிலி கட்டமைப்பு utf8data *data,
			       unsigned எழுத்து *hangul, மாறிலி எழுத்து *s, size_t len)
{
	utf8trie_t	*trie;
	முழு		offlen;
	முழு		offset;
	முழு		mask;
	முழு		முனை;

	எனில் (!data)
		திரும்பு வெற்று;
	எனில் (len == 0)
		திரும்பு வெற்று;

	trie = utf8data + data->offset;
	முனை = 1;
	போது (முனை) {
		offlen = (*trie & OFFLEN) >> OFFLEN_SHIFT;
		எனில் (*trie & NEXTBYTE) {
			எனில் (--len == 0)
				திரும்பு வெற்று;
			s++;
		}
		mask = 1 << (*trie & BITNUM);
		எனில் (*s & mask) {
			/* Right leg */
			எனில் (offlen) {
				/* Right node at offset of trie */
				முனை = (*trie & RIGHTNODE);
				offset = trie[offlen];
				போது (--offlen) {
					offset <<= 8;
					offset |= trie[offlen];
				}
				trie += offset;
			} இல்லையெனில் எனில் (*trie & RIGHTPATH) {
				/* Right node after this node */
				முனை = (*trie & TRIENODE);
				trie++;
			} இல்லையெனில் {
				/* No right node. */
				திரும்பு வெற்று;
			}
		} இல்லையெனில் {
			/* Left leg */
			எனில் (offlen) {
				/* Left node after this node. */
				முனை = (*trie & LEFTNODE);
				trie += offlen + 1;
			} இல்லையெனில் எனில் (*trie & RIGHTPATH) {
				/* No left node. */
				திரும்பு வெற்று;
			} இல்லையெனில் {
				/* Left node after this node */
				முனை = (*trie & TRIENODE);
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
	எனில் (LEAF_CCC(trie) == DECOMPOSE && LEAF_STR(trie)[0] == HANGUL)
		trie = utf8hangul(s - 2, hangul);
	திரும்பு trie;
}

/*
 * Use trie to scan s.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * Forwards to utf8nlookup().
 */
நிலையான utf8leaf_t *utf8lookup(மாறிலி கட்டமைப்பு utf8data *data,
			      unsigned எழுத்து *hangul, மாறிலி எழுத்து *s)
{
	திரும்பு utf8nlookup(data, hangul, s, (size_t)-1);
}

#எனில் 0
/*
 * Maximum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if only non-assigned code points are used.
 */
நிலையான முழு utf8agemax(மாறிலி கட்டமைப்பு utf8data *data, மாறிலி எழுத்து *s)
{
	utf8leaf_t	*leaf;
	முழு		age = 0;
	முழு		leaf_age;
	unsigned எழுத்து	hangul[UTF8HANGULLEAF];

	எனில் (!data)
		திரும்பு -1;

	போது (*s) {
		leaf = utf8lookup(data, hangul, s);
		எனில் (!leaf)
			திரும்பு -1;

		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		எனில் (leaf_age <= data->maxage && leaf_age > age)
			age = leaf_age;
		s += utf8clen(s);
	}
	திரும்பு age;
}
#endif

#எனில் 0
/*
 * Minimum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if non-assigned code points are used.
 */
நிலையான முழு utf8agemin(மாறிலி கட்டமைப்பு utf8data *data, மாறிலி எழுத்து *s)
{
	utf8leaf_t	*leaf;
	முழு		age;
	முழு		leaf_age;
	unsigned எழுத்து	hangul[UTF8HANGULLEAF];

	எனில் (!data)
		திரும்பு -1;
	age = data->maxage;
	போது (*s) {
		leaf = utf8lookup(data, hangul, s);
		எனில் (!leaf)
			திரும்பு -1;
		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		எனில் (leaf_age <= data->maxage && leaf_age < age)
			age = leaf_age;
		s += utf8clen(s);
	}
	திரும்பு age;
}
#endif

#எனில் 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
நிலையான முழு utf8nagemax(மாறிலி கட்டமைப்பு utf8data *data, மாறிலி எழுத்து *s, size_t len)
{
	utf8leaf_t	*leaf;
	முழு		age = 0;
	முழு		leaf_age;
	unsigned எழுத்து	hangul[UTF8HANGULLEAF];

	எனில் (!data)
		திரும்பு -1;

	போது (len && *s) {
		leaf = utf8nlookup(data, hangul, s, len);
		எனில் (!leaf)
			திரும்பு -1;
		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		எனில் (leaf_age <= data->maxage && leaf_age > age)
			age = leaf_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	திரும்பு age;
}
#endif

#எனில் 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
நிலையான முழு utf8nagemin(மாறிலி கட்டமைப்பு utf8data *data, மாறிலி எழுத்து *s, size_t len)
{
	utf8leaf_t	*leaf;
	முழு		leaf_age;
	முழு		age;
	unsigned எழுத்து	hangul[UTF8HANGULLEAF];

	எனில் (!data)
		திரும்பு -1;
	age = data->maxage;
	போது (len && *s) {
		leaf = utf8nlookup(data, hangul, s, len);
		எனில் (!leaf)
			திரும்பு -1;
		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		எனில் (leaf_age <= data->maxage && leaf_age < age)
			age = leaf_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	திரும்பு age;
}
#endif

#எனில் 0
/*
 * Length of the normalization of s.
 * Return -1 if s is not valid UTF-8 unicode.
 *
 * A string of Default_Ignorable_Code_Point has length 0.
 */
நிலையான ssize_t utf8len(மாறிலி கட்டமைப்பு utf8data *data, மாறிலி எழுத்து *s)
{
	utf8leaf_t	*leaf;
	size_t		ret = 0;
	unsigned எழுத்து	hangul[UTF8HANGULLEAF];

	எனில் (!data)
		திரும்பு -1;
	போது (*s) {
		leaf = utf8lookup(data, hangul, s);
		எனில் (!leaf)
			திரும்பு -1;
		எனில் (utf8agetab[LEAF_GEN(leaf)] > data->maxage)
			ret += utf8clen(s);
		இல்லையெனில் எனில் (LEAF_CCC(leaf) == DECOMPOSE)
			ret += strlen(LEAF_STR(leaf));
		இல்லையெனில்
			ret += utf8clen(s);
		s += utf8clen(s);
	}
	திரும்பு ret;
}
#endif

#எனில் 0
/*
 * Length of the normalization of s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
நிலையான ssize_t utf8nlen(மாறிலி கட்டமைப்பு utf8data *data, மாறிலி எழுத்து *s, size_t len)
{
	utf8leaf_t	*leaf;
	size_t		ret = 0;
	unsigned எழுத்து	hangul[UTF8HANGULLEAF];

	எனில் (!data)
		திரும்பு -1;
	போது (len && *s) {
		leaf = utf8nlookup(data, hangul, s, len);
		எனில் (!leaf)
			திரும்பு -1;
		எனில் (utf8agetab[LEAF_GEN(leaf)] > data->maxage)
			ret += utf8clen(s);
		இல்லையெனில் எனில் (LEAF_CCC(leaf) == DECOMPOSE)
			ret += strlen(LEAF_STR(leaf));
		இல்லையெனில்
			ret += utf8clen(s);
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	திரும்பு ret;
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
நிலையான முழு utf8ncursor(கட்டமைப்பு utf8cursor *u8c, மாறிலி கட்டமைப்பு utf8data *data,
		மாறிலி எழுத்து *s, size_t len)
{
	எனில் (!data)
		திரும்பு -1;
	எனில் (!s)
		திரும்பு -1;
	u8c->data = data;
	u8c->s = s;
	u8c->p = வெற்று;
	u8c->ss = வெற்று;
	u8c->sp = வெற்று;
	u8c->len = len;
	u8c->slen = 0;
	u8c->ccc = STOPPER;
	u8c->nccc = STOPPER;
	/* Check we didn't clobber the maximum length. */
	எனில் (u8c->len != len)
		திரும்பு -1;
	/* The first byte of s may not be an utf8 continuation. */
	எனில் (len > 0 && (*s & 0xC0) == 0x80)
		திரும்பு -1;
	திரும்பு 0;
}

#எனில் 0
/*
 * Set up an utf8cursor for use by utf8byte().
 *
 *   u8c    : pointer to cursor.
 *   data   : const struct utf8data to use for normalization.
 *   s      : NUL-terminated string.
 *
 * Returns -1 on error, 0 on success.
 */
நிலையான முழு utf8cursor(கட்டமைப்பு utf8cursor *u8c, மாறிலி கட்டமைப்பு utf8data *data,
	       மாறிலி எழுத்து *s)
{
	திரும்பு utf8ncursor(u8c, data, s, (unsigned முழு)-1);
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
நிலையான முழு utf8byte(கட்டமைப்பு utf8cursor *u8c)
{
	utf8leaf_t *leaf;
	முழு ccc;

	க்கு (;;) {
		/* Check for the end of a decomposed character. */
		எனில் (u8c->p && *u8c->s == '\0') {
			u8c->s = u8c->p;
			u8c->p = வெற்று;
		}

		/* Check for end-of-string. */
		எனில் (!u8c->p && (u8c->len == 0 || *u8c->s == '\0')) {
			/* There is no next byte. */
			எனில் (u8c->ccc == STOPPER)
				திரும்பு 0;
			/* End-of-string during a scan counts as a stopper. */
			ccc = STOPPER;
			செல் ccc_mismatch;
		} இல்லையெனில் எனில் ((*u8c->s & 0xC0) == 0x80) {
			/* This is a continuation of the current character. */
			எனில் (!u8c->p)
				u8c->len--;
			திரும்பு (unsigned எழுத்து)*u8c->s++;
		}

		/* Look up the data for the current character. */
		எனில் (u8c->p) {
			leaf = utf8lookup(u8c->data, u8c->hangul, u8c->s);
		} இல்லையெனில் {
			leaf = utf8nlookup(u8c->data, u8c->hangul,
					   u8c->s, u8c->len);
		}

		/* No leaf found implies that the input is a binary blob. */
		எனில் (!leaf)
			திரும்பு -1;

		ccc = LEAF_CCC(leaf);
		/* Characters that are too new have CCC 0. */
		எனில் (utf8agetab[LEAF_GEN(leaf)] > u8c->data->maxage) {
			ccc = STOPPER;
		} இல்லையெனில் எனில் (ccc == DECOMPOSE) {
			u8c->len -= utf8clen(u8c->s);
			u8c->p = u8c->s + utf8clen(u8c->s);
			u8c->s = LEAF_STR(leaf);
			/* Empty decomposition implies CCC 0. */
			எனில் (*u8c->s == '\0') {
				எனில் (u8c->ccc == STOPPER)
					தொடர்;
				ccc = STOPPER;
				செல் ccc_mismatch;
			}

			leaf = utf8lookup(u8c->data, u8c->hangul, u8c->s);
			எனில் (!leaf)
				திரும்பு -1;
			ccc = LEAF_CCC(leaf);
		}

		/*
		 * If this is not a stopper, then see if it updates
		 * the next canonical class to be emitted.
		 */
		எனில் (ccc != STOPPER && u8c->ccc < ccc && ccc < u8c->nccc)
			u8c->nccc = ccc;

		/*
		 * Return the current byte if this is the current
		 * combining class.
		 */
		எனில் (ccc == u8c->ccc) {
			எனில் (!u8c->p)
				u8c->len--;
			திரும்பு (unsigned எழுத்து)*u8c->s++;
		}

		/* Current combining class mismatch. */
ccc_mismatch:
		எனில் (u8c->nccc == STOPPER) {
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
			எனில் (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} இல்லையெனில் எனில் (ccc != STOPPER) {
			/* Not a stopper, and not the ccc we're emitting. */
			எனில் (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} இல்லையெனில் எனில் (u8c->nccc != MAXCCC + 1) {
			/* At a stopper, restart for next ccc. */
			u8c->ccc = u8c->nccc;
			u8c->nccc = MAXCCC + 1;
			u8c->s = u8c->ss;
			u8c->p = u8c->sp;
			u8c->len = u8c->slen;
		} இல்லையெனில் {
			/* All done, proceed from here. */
			u8c->ccc = STOPPER;
			u8c->nccc = STOPPER;
			u8c->sp = வெற்று;
			u8c->ss = வெற்று;
			u8c->slen = 0;
		}
	}
}

#எனில் 0
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
நிலையான மாறிலி கட்டமைப்பு utf8data *utf8nfdi(unsigned முழு maxage)
{
	முழு i = ARRAY_SIZE(utf8nfdidata) - 1;

	போது (maxage < utf8nfdidata[i].maxage)
		i--;
	எனில் (maxage > utf8nfdidata[i].maxage)
		திரும்பு வெற்று;
	திரும்பு &utf8nfdidata[i];
}
#endif

நிலையான மாறிலி கட்டமைப்பு utf8data *utf8nfdicf(unsigned முழு maxage)
{
	முழு i = ARRAY_SIZE(utf8nfdicfdata) - 1;

	போது (maxage < utf8nfdicfdata[i].maxage)
		i--;
	எனில் (maxage > utf8nfdicfdata[i].maxage)
		திரும்பு வெற்று;
	திரும்பு &utf8nfdicfdata[i];
}

நிலையான முழு utf8_casefold(மாறிலி கட்டமைப்பு ext2fs_nls_table *table,
			  மாறிலி unsigned எழுத்து *str, size_t len,
			  unsigned எழுத்து *dest, size_t dlen)
{
	மாறிலி கட்டமைப்பு utf8data *data = utf8nfdicf(table->version);
	கட்டமைப்பு utf8cursor cur;
	size_t nlen = 0;

	எனில் (utf8ncursor(&cur, data, (மாறிலி எழுத்து *) str, len) < 0)
		செல் invalid_seq;

	க்கு (nlen = 0; nlen < dlen; nlen++) {
		முழு c = utf8byte(&cur);

		dest[nlen] = c;
		எனில் (!c)
			திரும்பு nlen;
		எனில் (c == -1)
			நிறுத்து;
	}

	திரும்பு -ENAMETOOLONG;

invalid_seq:
	எனில் (dlen < len)
		திரும்பு -ENAMETOOLONG;

	/* Signal invalid sequence */
	திரும்பு -EINVAL;
}

நிலையான முழு utf8_validate(மாறிலி கட்டமைப்பு ext2fs_nls_table *table,
			 எழுத்து *s, size_t len, எழுத்து **pos)
{
	மாறிலி கட்டமைப்பு utf8data *data = utf8nfdicf(table->version);
	utf8leaf_t	*leaf;
	unsigned எழுத்து	hangul[UTF8HANGULLEAF];

	எனில் (!data)
		திரும்பு -1;
	போது (len && *s) {
		leaf = utf8nlookup(data, hangul, s, len);
		எனில் (!leaf) {
			*pos = s;
			திரும்பு 1;
		}
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	திரும்பு 0;
}

நிலையான முழு utf8_casefold_cmp(மாறிலி கட்டமைப்பு ext2fs_nls_table *table,
			     மாறிலி unsigned எழுத்து *str1, size_t len1,
			     மாறிலி unsigned எழுத்து *str2, size_t len2)
{
	மாறிலி கட்டமைப்பு utf8data *data = utf8nfdicf(table->version);
	முழு c1, c2;
	கட்டமைப்பு utf8cursor cur1, cur2;

	எனில் (utf8ncursor(&cur1, data, (மாறிலி எழுத்து *) str1, len1) < 0)
		திரும்பு -1;
	எனில் (utf8ncursor(&cur2, data, (மாறிலி எழுத்து *) str2, len2) < 0)
		திரும்பு -1;

	செய் {
		c1 = utf8byte(&cur1);
		c2 = utf8byte(&cur2);

		எனில் (c1 < 0 || c2 < 0)
			திரும்பு -1;
		எனில் (c1 != c2)
			திரும்பு c1 - c2;
	} போது (c1);

	திரும்பு 0;
}

நிலையான மாறிலி கட்டமைப்பு ext2fs_nls_ops utf8_ops = {
	.casefold = utf8_casefold,
	.validate = utf8_validate,
	.casefold_cmp = utf8_casefold_cmp,
};

நிலையான மாறிலி கட்டமைப்பு ext2fs_nls_table nls_utf8 = {
	.ops = &utf8_ops,
	.version = UNICODE_AGE(12, 1, 0),
};

மாறிலி கட்டமைப்பு ext2fs_nls_table *ext2fs_load_nls_table(முழு encoding)
{
	எனில் (encoding == EXT4_ENC_UTF8_12_1)
		திரும்பு &nls_utf8;

	திரும்பு வெற்று;
}

முழு ext2fs_check_encoded_name(மாறிலி கட்டமைப்பு ext2fs_nls_table *table,
			      எழுத்து *name, size_t len, எழுத்து **pos)
{
	திரும்பு table->ops->validate(table, name, len, pos);
}

முழு ext2fs_casefold_cmp(மாறிலி கட்டமைப்பு ext2fs_nls_table *table,
			மாறிலி unsigned எழுத்து *str1, size_t len1,
			மாறிலி unsigned எழுத்து *str2, size_t len2)
{
	திரும்பு table->ops->casefold_cmp(table, str1, len1, str2, len2);
}
