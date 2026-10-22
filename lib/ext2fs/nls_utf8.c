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
	(((unsigned 정수)(MAJ) << UNICODE_MAJ_SHIFT) |	\
	 ((unsigned 정수)(MIN) << UNICODE_MIN_SHIFT) |	\
	 ((unsigned 정수)(REV)))

/* Needed in struct utf8cursor below. */
#define UTF8HANGULLEAF	(12)

/*
 * Cursor structure used by the normalizer.
 */
struct utf8cursor {
	const struct utf8data	*data;
	const char	*s;
	const char	*p;
	const char	*ss;
	const char	*sp;
	unsigned 정수	len;
	unsigned 정수	slen;
	short 정수	ccc;
	short 정수	nccc;
	unsigned char	hangul[UTF8HANGULLEAF];
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


struct utf8data {
	unsigned 정수 maxage;
	unsigned 정수 offset;
};

#define __INCLUDED_FROM_UTF8NORM_C__
#include "utf8data.h"
#undef __INCLUDED_FROM_UTF8NORM_C__

#define ARRAY_SIZE(array)			\
        (sizeof(array) / sizeof(array[0]))

#만약 0
/* Highest unicode version supported by the data tables. */
static 정수 utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev)
{
	정수 i = ARRAY_SIZE(utf8agetab) - 1;
	unsigned 정수 sb_utf8version = UNICODE_AGE(maj, min, rev);

	동안 (i >= 0 && utf8agetab[i] != 0) {
		만약 (sb_utf8version == utf8agetab[i])
			반환 1;
		i--;
	}
	반환 0;
}
#endif

#만약 0
static 정수 utf8version_latest(void)
{
	반환 utf8vers;
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
static inline 정수 utf8clen(const char *s)
{
	unsigned char c = *s;

	반환 1 + (c >= 0xC0) + (c >= 0xE0) + (c >= 0xF0);
}

/*
 * Decode a 3-byte UTF-8 sequence.
 */
static unsigned 정수
utf8decode3(const char *str)
{
	unsigned 정수		uc;

	uc = *str++ & 0x0F;
	uc <<= 6;
	uc |= *str++ & 0x3F;
	uc <<= 6;
	uc |= *str++ & 0x3F;

	반환 uc;
}

/*
 * Encode a 3-byte UTF-8 sequence.
 */
static 정수
utf8encode3(char *str, unsigned 정수 val)
{
	str[2] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[1] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[0] = val | 0xE0;

	반환 3;
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
typedef const unsigned char utf8trie_t;
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
typedef const unsigned char utf8leaf_t;

#define LEAF_GEN(LEAF)	((LEAF)[0])
#define LEAF_CCC(LEAF)	((LEAF)[1])
#define LEAF_STR(LEAF)	((const char *)((LEAF) + 2))

#define MINCCC		(0)
#define MAXCCC		(254)
#define STOPPER		(0)
#define	DECOMPOSE	(255)

/* Marker for hangul syllable decomposition. */
#define HANGUL		((char)(255))
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
utf8hangul(const char *str, unsigned char *hangul)
{
	unsigned 정수	si;
	unsigned 정수	li;
	unsigned 정수	vi;
	unsigned 정수	ti;
	unsigned char	*h;

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
	h += utf8encode3((char *)h, li + LB);

	/* Add VPart, a 3-byte UTF-8 sequence. */
	h += utf8encode3((char *)h, vi + VB);

	/* Add TPart if required, also a 3-byte UTF-8 sequence. */
	만약 (ti)
		h += utf8encode3((char *)h, ti + TB);

	/* Terminate string. */
	h[0] = '\0';

	반환 hangul;
}

/*
 * Use trie to scan s, touching at most len bytes.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * A non-NULL return guarantees that the UTF-8 sequence starting at s
 * is well-formed and corresponds to a known unicode code point.  The
 * shorthand for this will be "is valid UTF-8 unicode".
 */
static utf8leaf_t *utf8nlookup(const struct utf8data *data,
			       unsigned char *hangul, const char *s, size_t len)
{
	utf8trie_t	*trie;
	정수		offlen;
	정수		offset;
	정수		mask;
	정수		node;

	만약 (!data)
		반환 NULL;
	만약 (len == 0)
		반환 NULL;

	trie = utf8data + data->offset;
	node = 1;
	동안 (node) {
		offlen = (*trie & OFFLEN) >> OFFLEN_SHIFT;
		만약 (*trie & NEXTBYTE) {
			만약 (--len == 0)
				반환 NULL;
			s++;
		}
		mask = 1 << (*trie & BITNUM);
		만약 (*s & mask) {
			/* Right leg */
			만약 (offlen) {
				/* Right node at offset of trie */
				node = (*trie & RIGHTNODE);
				offset = trie[offlen];
				동안 (--offlen) {
					offset <<= 8;
					offset |= trie[offlen];
				}
				trie += offset;
			} 아니면 만약 (*trie & RIGHTPATH) {
				/* Right node after this node */
				node = (*trie & TRIENODE);
				trie++;
			} 아니면 {
				/* No right node. */
				반환 NULL;
			}
		} 아니면 {
			/* Left leg */
			만약 (offlen) {
				/* Left node after this node. */
				node = (*trie & LEFTNODE);
				trie += offlen + 1;
			} 아니면 만약 (*trie & RIGHTPATH) {
				/* No left node. */
				반환 NULL;
			} 아니면 {
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
	만약 (LEAF_CCC(trie) == DECOMPOSE && LEAF_STR(trie)[0] == HANGUL)
		trie = utf8hangul(s - 2, hangul);
	반환 trie;
}

/*
 * Use trie to scan s.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * Forwards to utf8nlookup().
 */
static utf8leaf_t *utf8lookup(const struct utf8data *data,
			      unsigned char *hangul, const char *s)
{
	반환 utf8nlookup(data, hangul, s, (size_t)-1);
}

#만약 0
/*
 * Maximum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if only non-assigned code points are used.
 */
static 정수 utf8agemax(const struct utf8data *data, const char *s)
{
	utf8leaf_t	*leaf;
	정수		age = 0;
	정수		leaf_age;
	unsigned char	hangul[UTF8HANGULLEAF];

	만약 (!data)
		반환 -1;

	동안 (*s) {
		leaf = utf8lookup(data, hangul, s);
		만약 (!leaf)
			반환 -1;

		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		만약 (leaf_age <= data->maxage && leaf_age > age)
			age = leaf_age;
		s += utf8clen(s);
	}
	반환 age;
}
#endif

#만약 0
/*
 * Minimum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if non-assigned code points are used.
 */
static 정수 utf8agemin(const struct utf8data *data, const char *s)
{
	utf8leaf_t	*leaf;
	정수		age;
	정수		leaf_age;
	unsigned char	hangul[UTF8HANGULLEAF];

	만약 (!data)
		반환 -1;
	age = data->maxage;
	동안 (*s) {
		leaf = utf8lookup(data, hangul, s);
		만약 (!leaf)
			반환 -1;
		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		만약 (leaf_age <= data->maxage && leaf_age < age)
			age = leaf_age;
		s += utf8clen(s);
	}
	반환 age;
}
#endif

#만약 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
static 정수 utf8nagemax(const struct utf8data *data, const char *s, size_t len)
{
	utf8leaf_t	*leaf;
	정수		age = 0;
	정수		leaf_age;
	unsigned char	hangul[UTF8HANGULLEAF];

	만약 (!data)
		반환 -1;

	동안 (len && *s) {
		leaf = utf8nlookup(data, hangul, s, len);
		만약 (!leaf)
			반환 -1;
		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		만약 (leaf_age <= data->maxage && leaf_age > age)
			age = leaf_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	반환 age;
}
#endif

#만약 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
static 정수 utf8nagemin(const struct utf8data *data, const char *s, size_t len)
{
	utf8leaf_t	*leaf;
	정수		leaf_age;
	정수		age;
	unsigned char	hangul[UTF8HANGULLEAF];

	만약 (!data)
		반환 -1;
	age = data->maxage;
	동안 (len && *s) {
		leaf = utf8nlookup(data, hangul, s, len);
		만약 (!leaf)
			반환 -1;
		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		만약 (leaf_age <= data->maxage && leaf_age < age)
			age = leaf_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	반환 age;
}
#endif

#만약 0
/*
 * Length of the normalization of s.
 * Return -1 if s is not valid UTF-8 unicode.
 *
 * A string of Default_Ignorable_Code_Point has length 0.
 */
static ssize_t utf8len(const struct utf8data *data, const char *s)
{
	utf8leaf_t	*leaf;
	size_t		ret = 0;
	unsigned char	hangul[UTF8HANGULLEAF];

	만약 (!data)
		반환 -1;
	동안 (*s) {
		leaf = utf8lookup(data, hangul, s);
		만약 (!leaf)
			반환 -1;
		만약 (utf8agetab[LEAF_GEN(leaf)] > data->maxage)
			ret += utf8clen(s);
		아니면 만약 (LEAF_CCC(leaf) == DECOMPOSE)
			ret += strlen(LEAF_STR(leaf));
		아니면
			ret += utf8clen(s);
		s += utf8clen(s);
	}
	반환 ret;
}
#endif

#만약 0
/*
 * Length of the normalization of s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
static ssize_t utf8nlen(const struct utf8data *data, const char *s, size_t len)
{
	utf8leaf_t	*leaf;
	size_t		ret = 0;
	unsigned char	hangul[UTF8HANGULLEAF];

	만약 (!data)
		반환 -1;
	동안 (len && *s) {
		leaf = utf8nlookup(data, hangul, s, len);
		만약 (!leaf)
			반환 -1;
		만약 (utf8agetab[LEAF_GEN(leaf)] > data->maxage)
			ret += utf8clen(s);
		아니면 만약 (LEAF_CCC(leaf) == DECOMPOSE)
			ret += strlen(LEAF_STR(leaf));
		아니면
			ret += utf8clen(s);
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	반환 ret;
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
static 정수 utf8ncursor(struct utf8cursor *u8c, const struct utf8data *data,
		const char *s, size_t len)
{
	만약 (!data)
		반환 -1;
	만약 (!s)
		반환 -1;
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
	만약 (u8c->len != len)
		반환 -1;
	/* The first byte of s may not be an utf8 continuation. */
	만약 (len > 0 && (*s & 0xC0) == 0x80)
		반환 -1;
	반환 0;
}

#만약 0
/*
 * Set up an utf8cursor for use by utf8byte().
 *
 *   u8c    : pointer to cursor.
 *   data   : const struct utf8data to use for normalization.
 *   s      : NUL-terminated string.
 *
 * Returns -1 on error, 0 on success.
 */
static 정수 utf8cursor(struct utf8cursor *u8c, const struct utf8data *data,
	       const char *s)
{
	반환 utf8ncursor(u8c, data, s, (unsigned 정수)-1);
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
static 정수 utf8byte(struct utf8cursor *u8c)
{
	utf8leaf_t *leaf;
	정수 ccc;

	위해 (;;) {
		/* Check for the end of a decomposed character. */
		만약 (u8c->p && *u8c->s == '\0') {
			u8c->s = u8c->p;
			u8c->p = NULL;
		}

		/* Check for end-of-string. */
		만약 (!u8c->p && (u8c->len == 0 || *u8c->s == '\0')) {
			/* There is no next byte. */
			만약 (u8c->ccc == STOPPER)
				반환 0;
			/* End-of-string during a scan counts as a stopper. */
			ccc = STOPPER;
			goto ccc_mismatch;
		} 아니면 만약 ((*u8c->s & 0xC0) == 0x80) {
			/* This is a continuation of the current character. */
			만약 (!u8c->p)
				u8c->len--;
			반환 (unsigned char)*u8c->s++;
		}

		/* Look up the data for the current character. */
		만약 (u8c->p) {
			leaf = utf8lookup(u8c->data, u8c->hangul, u8c->s);
		} 아니면 {
			leaf = utf8nlookup(u8c->data, u8c->hangul,
					   u8c->s, u8c->len);
		}

		/* No leaf found implies that the input is a binary blob. */
		만약 (!leaf)
			반환 -1;

		ccc = LEAF_CCC(leaf);
		/* Characters that are too new have CCC 0. */
		만약 (utf8agetab[LEAF_GEN(leaf)] > u8c->data->maxage) {
			ccc = STOPPER;
		} 아니면 만약 (ccc == DECOMPOSE) {
			u8c->len -= utf8clen(u8c->s);
			u8c->p = u8c->s + utf8clen(u8c->s);
			u8c->s = LEAF_STR(leaf);
			/* Empty decomposition implies CCC 0. */
			만약 (*u8c->s == '\0') {
				만약 (u8c->ccc == STOPPER)
					continue;
				ccc = STOPPER;
				goto ccc_mismatch;
			}

			leaf = utf8lookup(u8c->data, u8c->hangul, u8c->s);
			만약 (!leaf)
				반환 -1;
			ccc = LEAF_CCC(leaf);
		}

		/*
		 * If this is not a stopper, then see if it updates
		 * the next canonical class to be emitted.
		 */
		만약 (ccc != STOPPER && u8c->ccc < ccc && ccc < u8c->nccc)
			u8c->nccc = ccc;

		/*
		 * Return the current byte if this is the current
		 * combining class.
		 */
		만약 (ccc == u8c->ccc) {
			만약 (!u8c->p)
				u8c->len--;
			반환 (unsigned char)*u8c->s++;
		}

		/* Current combining class mismatch. */
ccc_mismatch:
		만약 (u8c->nccc == STOPPER) {
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
			만약 (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} 아니면 만약 (ccc != STOPPER) {
			/* Not a stopper, and not the ccc we're emitting. */
			만약 (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} 아니면 만약 (u8c->nccc != MAXCCC + 1) {
			/* At a stopper, restart for next ccc. */
			u8c->ccc = u8c->nccc;
			u8c->nccc = MAXCCC + 1;
			u8c->s = u8c->ss;
			u8c->p = u8c->sp;
			u8c->len = u8c->slen;
		} 아니면 {
			/* All done, proceed from here. */
			u8c->ccc = STOPPER;
			u8c->nccc = STOPPER;
			u8c->sp = NULL;
			u8c->ss = NULL;
			u8c->slen = 0;
		}
	}
}

#만약 0
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
static const struct utf8data *utf8nfdi(unsigned 정수 maxage)
{
	정수 i = ARRAY_SIZE(utf8nfdidata) - 1;

	동안 (maxage < utf8nfdidata[i].maxage)
		i--;
	만약 (maxage > utf8nfdidata[i].maxage)
		반환 NULL;
	반환 &utf8nfdidata[i];
}
#endif

static const struct utf8data *utf8nfdicf(unsigned 정수 maxage)
{
	정수 i = ARRAY_SIZE(utf8nfdicfdata) - 1;

	동안 (maxage < utf8nfdicfdata[i].maxage)
		i--;
	만약 (maxage > utf8nfdicfdata[i].maxage)
		반환 NULL;
	반환 &utf8nfdicfdata[i];
}

static 정수 utf8_casefold(const struct ext2fs_nls_table *table,
			  const unsigned char *str, size_t len,
			  unsigned char *dest, size_t dlen)
{
	const struct utf8data *data = utf8nfdicf(table->version);
	struct utf8cursor cur;
	size_t nlen = 0;

	만약 (utf8ncursor(&cur, data, (const char *) str, len) < 0)
		goto invalid_seq;

	위해 (nlen = 0; nlen < dlen; nlen++) {
		정수 c = utf8byte(&cur);

		dest[nlen] = c;
		만약 (!c)
			반환 nlen;
		만약 (c == -1)
			break;
	}

	반환 -ENAMETOOLONG;

invalid_seq:
	만약 (dlen < len)
		반환 -ENAMETOOLONG;

	/* Signal invalid sequence */
	반환 -EINVAL;
}

static 정수 utf8_validate(const struct ext2fs_nls_table *table,
			 char *s, size_t len, char **pos)
{
	const struct utf8data *data = utf8nfdicf(table->version);
	utf8leaf_t	*leaf;
	unsigned char	hangul[UTF8HANGULLEAF];

	만약 (!data)
		반환 -1;
	동안 (len && *s) {
		leaf = utf8nlookup(data, hangul, s, len);
		만약 (!leaf) {
			*pos = s;
			반환 1;
		}
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	반환 0;
}

static 정수 utf8_casefold_cmp(const struct ext2fs_nls_table *table,
			     const unsigned char *str1, size_t len1,
			     const unsigned char *str2, size_t len2)
{
	const struct utf8data *data = utf8nfdicf(table->version);
	정수 c1, c2;
	struct utf8cursor cur1, cur2;

	만약 (utf8ncursor(&cur1, data, (const char *) str1, len1) < 0)
		반환 -1;
	만약 (utf8ncursor(&cur2, data, (const char *) str2, len2) < 0)
		반환 -1;

	do {
		c1 = utf8byte(&cur1);
		c2 = utf8byte(&cur2);

		만약 (c1 < 0 || c2 < 0)
			반환 -1;
		만약 (c1 != c2)
			반환 c1 - c2;
	} 동안 (c1);

	반환 0;
}

static const struct ext2fs_nls_ops utf8_ops = {
	.casefold = utf8_casefold,
	.validate = utf8_validate,
	.casefold_cmp = utf8_casefold_cmp,
};

static const struct ext2fs_nls_table nls_utf8 = {
	.ops = &utf8_ops,
	.version = UNICODE_AGE(12, 1, 0),
};

const struct ext2fs_nls_table *ext2fs_load_nls_table(정수 encoding)
{
	만약 (encoding == EXT4_ENC_UTF8_12_1)
		반환 &nls_utf8;

	반환 NULL;
}

정수 ext2fs_check_encoded_name(const struct ext2fs_nls_table *table,
			      char *name, size_t len, char **pos)
{
	반환 table->ops->validate(table, name, len, pos);
}

정수 ext2fs_casefold_cmp(const struct ext2fs_nls_table *table,
			const unsigned char *str1, size_t len1,
			const unsigned char *str2, size_t len2)
{
	반환 table->ops->casefold_cmp(table, str1, len1, str2, len2);
}
