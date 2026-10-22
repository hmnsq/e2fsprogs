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

#include "config.h"
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

/* Encoding a unicode version number as a single unsigned int. */
#define UNICODE_MAJ_SHIFT		(16)
#define UNICODE_MIN_SHIFT		(8)

#define UNICODE_AGE(MAJ, MIN, REV)			\
	(((unsigned පූර්ණ_සංඛ්‍යා)(MAJ) << UNICODE_MAJ_SHIFT) |	\
	 ((unsigned පූර්ණ_සංඛ්‍යා)(MIN) << UNICODE_MIN_SHIFT) |	\
	 ((unsigned පූර්ණ_සංඛ්‍යා)(REV)))

/* Needed in struct utf8cursor below. */
#define UTF8HANGULLEAF	(12)

/*
 * Cursor structure used by the normalizer.
 */
sa_nracanaa utf8cursor {
	const sa_nracanaa utf8data	*data;
	const var_na	*s;
	const var_na	*p;
	const var_na	*ss;
	const var_na	*sp;
	unsigned පූර්ණ_සංඛ්‍යා	len;
	unsigned පූර්ණ_සංඛ්‍යා	slen;
	short පූර්ණ_සංඛ්‍යා	ccc;
	short පූර්ණ_සංඛ්‍යා	nccc;
	unsigned var_na	hangul[UTF8HANGULLEAF];
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


sa_nracanaa utf8data {
	unsigned පූර්ණ_සංඛ්‍යා maxage;
	unsigned පූර්ණ_සංඛ්‍යා offset;
};

#define __INCLUDED_FROM_UTF8NORM_C__
#include "utf8data.h"
#undef __INCLUDED_FROM_UTF8NORM_C__

#define ARRAY_SIZE(array)			\
        (parimaa_na(array) / parimaa_na(array[0]))

#නම් 0
/* Highest unicode version supported by the data tables. */
static පූර්ණ_සංඛ්‍යා utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev)
{
	පූර්ණ_සංඛ්‍යා i = ARRAY_SIZE(utf8agetab) - 1;
	unsigned පූර්ණ_සංඛ්‍යා sb_utf8version = UNICODE_AGE(maj, min, rev);

	අතරතුර (i >= 0 && utf8agetab[i] != 0) {
		නම් (sb_utf8version == utf8agetab[i])
			ආපසු 1;
		i--;
	}
	ආපසු 0;
}
#endif

#නම් 0
static පූර්ණ_සංඛ්‍යා utf8version_latest(void)
{
	ආපසු utf8vers;
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
static anta_hstha පූර්ණ_සංඛ්‍යා utf8clen(const var_na *s)
{
	unsigned var_na c = *s;

	ආපසු 1 + (c >= 0xC0) + (c >= 0xE0) + (c >= 0xF0);
}

/*
 * Decode a 3-byte UTF-8 sequence.
 */
static unsigned පූර්ණ_සංඛ්‍යා
utf8decode3(const var_na *str)
{
	unsigned පූර්ණ_සංඛ්‍යා		uc;

	uc = *str++ & 0x0F;
	uc <<= 6;
	uc |= *str++ & 0x3F;
	uc <<= 6;
	uc |= *str++ & 0x3F;

	ආපසු uc;
}

/*
 * Encode a 3-byte UTF-8 sequence.
 */
static පූර්ණ_සංඛ්‍යා
utf8encode3(var_na *str, unsigned පූර්ණ_සංඛ්‍යා val)
{
	str[2] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[1] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[0] = val | 0xE0;

	ආපසු 3;
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
typedef const unsigned var_na utf8trie_t;
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
typedef const unsigned var_na utf8leaf_t;

#define LEAF_GEN(LEAF)	((LEAF)[0])
#define LEAF_CCC(LEAF)	((LEAF)[1])
#define LEAF_STR(LEAF)	((const var_na *)((LEAF) + 2))

#define MINCCC		(0)
#define MAXCCC		(254)
#define STOPPER		(0)
#define	DECOMPOSE	(255)

/* Marker for hangul syllable decomposition. */
#define HANGUL		((var_na)(255))
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
static utf8leaf_t *
utf8hangul(const var_na *str, unsigned var_na *hangul)
{
	unsigned පූර්ණ_සංඛ්‍යා	si;
	unsigned පූර්ණ_සංඛ්‍යා	li;
	unsigned පූර්ණ_සංඛ්‍යා	vi;
	unsigned පූර්ණ_සංඛ්‍යා	ti;
	unsigned var_na	*h;

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
	h += utf8encode3((var_na *)h, li + LB);

	/* Add VPart, a 3-byte UTF-8 sequence. */
	h += utf8encode3((var_na *)h, vi + VB);

	/* Add TPart if required, also a 3-byte UTF-8 sequence. */
	නම් (ti)
		h += utf8encode3((var_na *)h, ti + TB);

	/* Terminate string. */
	h[0] = '\0';

	ආපසු hangul;
}

/*
 * Use trie to scan s, touching at most len bytes.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * A non-NULL return guarantees that the UTF-8 sequence starting at s
 * is well-formed and corresponds to a known unicode code point.  The
 * shorthand for this will be "is valid UTF-8 unicode".
 */
static utf8leaf_t *utf8nlookup(const sa_nracanaa utf8data *data,
			       unsigned var_na *hangul, const var_na *s, size_t len)
{
	utf8trie_t	*trie;
	පූර්ණ_සංඛ්‍යා		offlen;
	පූර්ණ_සංඛ්‍යා		offset;
	පූර්ණ_සංඛ්‍යා		mask;
	පූර්ණ_සංඛ්‍යා		node;

	නම් (!data)
		ආපසු NULL;
	නම් (len == 0)
		ආපසු NULL;

	trie = utf8data + data->offset;
	node = 1;
	අතරතුර (node) {
		offlen = (*trie & OFFLEN) >> OFFLEN_SHIFT;
		නම් (*trie & NEXTBYTE) {
			නම් (--len == 0)
				ආපසු NULL;
			s++;
		}
		mask = 1 << (*trie & BITNUM);
		නම් (*s & mask) {
			/* Right leg */
			නම් (offlen) {
				/* Right node at offset of trie */
				node = (*trie & RIGHTNODE);
				offset = trie[offlen];
				අතරතුර (--offlen) {
					offset <<= 8;
					offset |= trie[offlen];
				}
				trie += offset;
			} නැතහොත් නම් (*trie & RIGHTPATH) {
				/* Right node after this node */
				node = (*trie & TRIENODE);
				trie++;
			} නැතහොත් {
				/* No right node. */
				ආපසු NULL;
			}
		} නැතහොත් {
			/* Left leg */
			නම් (offlen) {
				/* Left node after this node. */
				node = (*trie & LEFTNODE);
				trie += offlen + 1;
			} නැතහොත් නම් (*trie & RIGHTPATH) {
				/* No left node. */
				ආපසු NULL;
			} නැතහොත් {
				/* Left node after this node */
				node = (*trie & TRIENODE);
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
	නම් (LEAF_CCC(trie) == DECOMPOSE && LEAF_STR(trie)[0] == HANGUL)
		trie = utf8hangul(s - 2, hangul);
	ආපසු trie;
}

/*
 * Use trie to scan s.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * Forwards to utf8nlookup().
 */
static utf8leaf_t *utf8lookup(const sa_nracanaa utf8data *data,
			      unsigned var_na *hangul, const var_na *s)
{
	ආපසු utf8nlookup(data, hangul, s, (size_t)-1);
}

#නම් 0
/*
 * Maximum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if only non-assigned code points are used.
 */
static පූර්ණ_සංඛ්‍යා utf8agemax(const sa_nracanaa utf8data *data, const var_na *s)
{
	utf8leaf_t	*leaf;
	පූර්ණ_සංඛ්‍යා		age = 0;
	පූර්ණ_සංඛ්‍යා		leaf_age;
	unsigned var_na	hangul[UTF8HANGULLEAF];

	නම් (!data)
		ආපසු -1;

	අතරතුර (*s) {
		leaf = utf8lookup(data, hangul, s);
		නම් (!leaf)
			ආපසු -1;

		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		නම් (leaf_age <= data->maxage && leaf_age > age)
			age = leaf_age;
		s += utf8clen(s);
	}
	ආපසු age;
}
#endif

#නම් 0
/*
 * Minimum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if non-assigned code points are used.
 */
static පූර්ණ_සංඛ්‍යා utf8agemin(const sa_nracanaa utf8data *data, const var_na *s)
{
	utf8leaf_t	*leaf;
	පූර්ණ_සංඛ්‍යා		age;
	පූර්ණ_සංඛ්‍යා		leaf_age;
	unsigned var_na	hangul[UTF8HANGULLEAF];

	නම් (!data)
		ආපසු -1;
	age = data->maxage;
	අතරතුර (*s) {
		leaf = utf8lookup(data, hangul, s);
		නම් (!leaf)
			ආපසු -1;
		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		නම් (leaf_age <= data->maxage && leaf_age < age)
			age = leaf_age;
		s += utf8clen(s);
	}
	ආපසු age;
}
#endif

#නම් 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
static පූර්ණ_සංඛ්‍යා utf8nagemax(const sa_nracanaa utf8data *data, const var_na *s, size_t len)
{
	utf8leaf_t	*leaf;
	පූර්ණ_සංඛ්‍යා		age = 0;
	පූර්ණ_සංඛ්‍යා		leaf_age;
	unsigned var_na	hangul[UTF8HANGULLEAF];

	නම් (!data)
		ආපසු -1;

	අතරතුර (len && *s) {
		leaf = utf8nlookup(data, hangul, s, len);
		නම් (!leaf)
			ආපසු -1;
		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		නම් (leaf_age <= data->maxage && leaf_age > age)
			age = leaf_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	ආපසු age;
}
#endif

#නම් 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
static පූර්ණ_සංඛ්‍යා utf8nagemin(const sa_nracanaa utf8data *data, const var_na *s, size_t len)
{
	utf8leaf_t	*leaf;
	පූර්ණ_සංඛ්‍යා		leaf_age;
	පූර්ණ_සංඛ්‍යා		age;
	unsigned var_na	hangul[UTF8HANGULLEAF];

	නම් (!data)
		ආපසු -1;
	age = data->maxage;
	අතරතුර (len && *s) {
		leaf = utf8nlookup(data, hangul, s, len);
		නම් (!leaf)
			ආපසු -1;
		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		නම් (leaf_age <= data->maxage && leaf_age < age)
			age = leaf_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	ආපසු age;
}
#endif

#නම් 0
/*
 * Length of the normalization of s.
 * Return -1 if s is not valid UTF-8 unicode.
 *
 * A string of Default_Ignorable_Code_Point has length 0.
 */
static ssize_t utf8len(const sa_nracanaa utf8data *data, const var_na *s)
{
	utf8leaf_t	*leaf;
	size_t		ret = 0;
	unsigned var_na	hangul[UTF8HANGULLEAF];

	නම් (!data)
		ආපසු -1;
	අතරතුර (*s) {
		leaf = utf8lookup(data, hangul, s);
		නම් (!leaf)
			ආපසු -1;
		නම් (utf8agetab[LEAF_GEN(leaf)] > data->maxage)
			ret += utf8clen(s);
		නැතහොත් නම් (LEAF_CCC(leaf) == DECOMPOSE)
			ret += strlen(LEAF_STR(leaf));
		නැතහොත්
			ret += utf8clen(s);
		s += utf8clen(s);
	}
	ආපසු ret;
}
#endif

#නම් 0
/*
 * Length of the normalization of s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
static ssize_t utf8nlen(const sa_nracanaa utf8data *data, const var_na *s, size_t len)
{
	utf8leaf_t	*leaf;
	size_t		ret = 0;
	unsigned var_na	hangul[UTF8HANGULLEAF];

	නම් (!data)
		ආපසු -1;
	අතරතුර (len && *s) {
		leaf = utf8nlookup(data, hangul, s, len);
		නම් (!leaf)
			ආපසු -1;
		නම් (utf8agetab[LEAF_GEN(leaf)] > data->maxage)
			ret += utf8clen(s);
		නැතහොත් නම් (LEAF_CCC(leaf) == DECOMPOSE)
			ret += strlen(LEAF_STR(leaf));
		නැතහොත්
			ret += utf8clen(s);
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	ආපසු ret;
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
static පූර්ණ_සංඛ්‍යා utf8ncursor(sa_nracanaa utf8cursor *u8c, const sa_nracanaa utf8data *data,
		const var_na *s, size_t len)
{
	නම් (!data)
		ආපසු -1;
	නම් (!s)
		ආපසු -1;
	u8c->data = data;
	u8c->s = s;
	u8c->p = NULL;
	u8c->ss = NULL;
	u8c->sp = NULL;
	u8c->len = len;
	u8c->slen = 0;
	u8c->ccc = STOPPER;
	u8c->nccc = STOPPER;
	/* Check we didn't clobber the maximum length. */
	නම් (u8c->len != len)
		ආපසු -1;
	/* The first byte of s may not be an utf8 continuation. */
	නම් (len > 0 && (*s & 0xC0) == 0x80)
		ආපසු -1;
	ආපසු 0;
}

#නම් 0
/*
 * Set up an utf8cursor for use by utf8byte().
 *
 *   u8c    : pointer to cursor.
 *   data   : const struct utf8data to use for normalization.
 *   s      : NUL-terminated string.
 *
 * Returns -1 on error, 0 on success.
 */
static පූර්ණ_සංඛ්‍යා utf8cursor(sa_nracanaa utf8cursor *u8c, const sa_nracanaa utf8data *data,
	       const var_na *s)
{
	ආපසු utf8ncursor(u8c, data, s, (unsigned පූර්ණ_සංඛ්‍යා)-1);
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
static පූර්ණ_සංඛ්‍යා utf8byte(sa_nracanaa utf8cursor *u8c)
{
	utf8leaf_t *leaf;
	පූර්ණ_සංඛ්‍යා ccc;

	සඳහා (;;) {
		/* Check for the end of a decomposed character. */
		නම් (u8c->p && *u8c->s == '\0') {
			u8c->s = u8c->p;
			u8c->p = NULL;
		}

		/* Check for end-of-string. */
		නම් (!u8c->p && (u8c->len == 0 || *u8c->s == '\0')) {
			/* There is no next byte. */
			නම් (u8c->ccc == STOPPER)
				ආපසු 0;
			/* End-of-string during a scan counts as a stopper. */
			ccc = STOPPER;
			goto ccc_mismatch;
		} නැතහොත් නම් ((*u8c->s & 0xC0) == 0x80) {
			/* This is a continuation of the current character. */
			නම් (!u8c->p)
				u8c->len--;
			ආපසු (unsigned var_na)*u8c->s++;
		}

		/* Look up the data for the current character. */
		නම් (u8c->p) {
			leaf = utf8lookup(u8c->data, u8c->hangul, u8c->s);
		} නැතහොත් {
			leaf = utf8nlookup(u8c->data, u8c->hangul,
					   u8c->s, u8c->len);
		}

		/* No leaf found implies that the input is a binary blob. */
		නම් (!leaf)
			ආපසු -1;

		ccc = LEAF_CCC(leaf);
		/* Characters that are too new have CCC 0. */
		නම් (utf8agetab[LEAF_GEN(leaf)] > u8c->data->maxage) {
			ccc = STOPPER;
		} නැතහොත් නම් (ccc == DECOMPOSE) {
			u8c->len -= utf8clen(u8c->s);
			u8c->p = u8c->s + utf8clen(u8c->s);
			u8c->s = LEAF_STR(leaf);
			/* Empty decomposition implies CCC 0. */
			නම් (*u8c->s == '\0') {
				නම් (u8c->ccc == STOPPER)
					continue;
				ccc = STOPPER;
				goto ccc_mismatch;
			}

			leaf = utf8lookup(u8c->data, u8c->hangul, u8c->s);
			නම් (!leaf)
				ආපසු -1;
			ccc = LEAF_CCC(leaf);
		}

		/*
		 * If this is not a stopper, then see if it updates
		 * the next canonical class to be emitted.
		 */
		නම් (ccc != STOPPER && u8c->ccc < ccc && ccc < u8c->nccc)
			u8c->nccc = ccc;

		/*
		 * Return the current byte if this is the current
		 * combining class.
		 */
		නම් (ccc == u8c->ccc) {
			නම් (!u8c->p)
				u8c->len--;
			ආපසු (unsigned var_na)*u8c->s++;
		}

		/* Current combining class mismatch. */
ccc_mismatch:
		නම් (u8c->nccc == STOPPER) {
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
			නම් (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} නැතහොත් නම් (ccc != STOPPER) {
			/* Not a stopper, and not the ccc we're emitting. */
			නම් (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} නැතහොත් නම් (u8c->nccc != MAXCCC + 1) {
			/* At a stopper, restart for next ccc. */
			u8c->ccc = u8c->nccc;
			u8c->nccc = MAXCCC + 1;
			u8c->s = u8c->ss;
			u8c->p = u8c->sp;
			u8c->len = u8c->slen;
		} නැතහොත් {
			/* All done, proceed from here. */
			u8c->ccc = STOPPER;
			u8c->nccc = STOPPER;
			u8c->sp = NULL;
			u8c->ss = NULL;
			u8c->slen = 0;
		}
	}
}

#නම් 0
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
static const sa_nracanaa utf8data *utf8nfdi(unsigned පූර්ණ_සංඛ්‍යා maxage)
{
	පූර්ණ_සංඛ්‍යා i = ARRAY_SIZE(utf8nfdidata) - 1;

	අතරතුර (maxage < utf8nfdidata[i].maxage)
		i--;
	නම් (maxage > utf8nfdidata[i].maxage)
		ආපසු NULL;
	ආපසු &utf8nfdidata[i];
}
#endif

static const sa_nracanaa utf8data *utf8nfdicf(unsigned පූර්ණ_සංඛ්‍යා maxage)
{
	පූර්ණ_සංඛ්‍යා i = ARRAY_SIZE(utf8nfdicfdata) - 1;

	අතරතුර (maxage < utf8nfdicfdata[i].maxage)
		i--;
	නම් (maxage > utf8nfdicfdata[i].maxage)
		ආපසු NULL;
	ආපසු &utf8nfdicfdata[i];
}

static පූර්ණ_සංඛ්‍යා utf8_casefold(const sa_nracanaa ext2fs_nls_table *table,
			  const unsigned var_na *str, size_t len,
			  unsigned var_na *dest, size_t dlen)
{
	const sa_nracanaa utf8data *data = utf8nfdicf(table->version);
	sa_nracanaa utf8cursor cur;
	size_t nlen = 0;

	නම් (utf8ncursor(&cur, data, (const var_na *) str, len) < 0)
		goto invalid_seq;

	සඳහා (nlen = 0; nlen < dlen; nlen++) {
		පූර්ණ_සංඛ්‍යා c = utf8byte(&cur);

		dest[nlen] = c;
		නම් (!c)
			ආපසු nlen;
		නම් (c == -1)
			bha_nga;
	}

	ආපසු -ENAMETOOLONG;

invalid_seq:
	නම් (dlen < len)
		ආපසු -ENAMETOOLONG;

	/* Signal invalid sequence */
	ආපසු -EINVAL;
}

static පූර්ණ_සංඛ්‍යා utf8_validate(const sa_nracanaa ext2fs_nls_table *table,
			 var_na *s, size_t len, var_na **pos)
{
	const sa_nracanaa utf8data *data = utf8nfdicf(table->version);
	utf8leaf_t	*leaf;
	unsigned var_na	hangul[UTF8HANGULLEAF];

	නම් (!data)
		ආපසු -1;
	අතරතුර (len && *s) {
		leaf = utf8nlookup(data, hangul, s, len);
		නම් (!leaf) {
			*pos = s;
			ආපසු 1;
		}
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා utf8_casefold_cmp(const sa_nracanaa ext2fs_nls_table *table,
			     const unsigned var_na *str1, size_t len1,
			     const unsigned var_na *str2, size_t len2)
{
	const sa_nracanaa utf8data *data = utf8nfdicf(table->version);
	පූර්ණ_සංඛ්‍යා c1, c2;
	sa_nracanaa utf8cursor cur1, cur2;

	නම් (utf8ncursor(&cur1, data, (const var_na *) str1, len1) < 0)
		ආපසු -1;
	නම් (utf8ncursor(&cur2, data, (const var_na *) str2, len2) < 0)
		ආපසු -1;

	do {
		c1 = utf8byte(&cur1);
		c2 = utf8byte(&cur2);

		නම් (c1 < 0 || c2 < 0)
			ආපසු -1;
		නම් (c1 != c2)
			ආපසු c1 - c2;
	} අතරතුර (c1);

	ආපසු 0;
}

static const sa_nracanaa ext2fs_nls_ops utf8_ops = {
	.casefold = utf8_casefold,
	.validate = utf8_validate,
	.casefold_cmp = utf8_casefold_cmp,
};

static const sa_nracanaa ext2fs_nls_table nls_utf8 = {
	.ops = &utf8_ops,
	.version = UNICODE_AGE(12, 1, 0),
};

const sa_nracanaa ext2fs_nls_table *ext2fs_load_nls_table(පූර්ණ_සංඛ්‍යා encoding)
{
	නම් (encoding == EXT4_ENC_UTF8_12_1)
		ආපසු &nls_utf8;

	ආපසු NULL;
}

පූර්ණ_සංඛ්‍යා ext2fs_check_encoded_name(const sa_nracanaa ext2fs_nls_table *table,
			      var_na *name, size_t len, var_na **pos)
{
	ආපසු table->ops->validate(table, name, len, pos);
}

පූර්ණ_සංඛ්‍යා ext2fs_casefold_cmp(const sa_nracanaa ext2fs_nls_table *table,
			const unsigned var_na *str1, size_t len1,
			const unsigned var_na *str2, size_t len2)
{
	ආපසු table->ops->casefold_cmp(table, str1, len1, str2, len2);
}
