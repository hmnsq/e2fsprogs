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

#شامل "config.h"
#شامل <stdint.h>
#شامل <unistd.h>
#شامل <string.h>
#شامل <limits.h>
#شامل <errno.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

/* Encoding a unicode version number as a single unsigned int. */
#تعریف_کرو UNICODE_MAJ_سرکاؤ		(16)
#تعریف_کرو UNICODE_MIN_سرکاؤ		(8)

#تعریف_کرو UNICODE_AGE(MAJ, MIN, REV)			\
	(((غیرعلامتی صحیح)(MAJ) << UNICODE_MAJ_سرکاؤ) |	\
	 ((غیرعلامتی صحیح)(MIN) << UNICODE_MIN_سرکاؤ) |	\
	 ((غیرعلامتی صحیح)(REV)))

/* Needed in struct utf8cursor below. */
#تعریف_کرو UTF8HANGULLEAF	(12)

/*
 * Cursor structure used by the normalizer.
 */
ساخت utf8cursor {
	ثابت ساخت utf8data	*ڈیٹا;
	ثابت حرف	*s;
	ثابت حرف	*p;
	ثابت حرف	*ss;
	ثابت حرف	*sp;
	غیرعلامتی صحیح	len;
	غیرعلامتی صحیح	slen;
	مختصر صحیح	ccc;
	مختصر صحیح	nccc;
	غیرعلامتی حرف	hangul[UTF8HANGULLEAF];
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


ساخت utf8data {
	غیرعلامتی صحیح maxage;
	غیرعلامتی صحیح انحراف;
};

#تعریف_کرو __INCLUDED_FROM_UTF8NORM_C__
#شامل "utf8data.h"
#تعریف_منسوخ __INCLUDED_FROM_UTF8NORM_C__

#تعریف_کرو ARRAY_سائز(array)			\
        (حجم(array) / حجم(array[0]))

#اگر 0
/* Highest unicode version supported by the data tables. */
ساکن صحیح utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev)
{
	صحیح i = ARRAY_سائز(utf8agetab) - 1;
	غیرعلامتی صحیح sb_utf8version = UNICODE_AGE(maj, min, rev);

	جب (i >= 0 && utf8agetab[i] != 0) {
		اگر (sb_utf8version == utf8agetab[i])
			واپس 1;
		i--;
	}
	واپس 0;
}
#اگر_ختم

#اگر 0
ساکن صحیح utf8version_latest(خالی)
{
	واپس utf8vers;
}
#اگر_ختم

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
ساکن اندرونی صحیح utf8clen(ثابت حرف *s)
{
	غیرعلامتی حرف c = *s;

	واپس 1 + (c >= 0xC0) + (c >= 0xE0) + (c >= 0xF0);
}

/*
 * Decode a 3-byte UTF-8 sequence.
 */
ساکن غیرعلامتی صحیح
utf8decode3(ثابت حرف *str)
{
	غیرعلامتی صحیح		uc;

	uc = *str++ & 0x0F;
	uc <<= 6;
	uc |= *str++ & 0x3F;
	uc <<= 6;
	uc |= *str++ & 0x3F;

	واپس uc;
}

/*
 * Encode a 3-byte UTF-8 sequence.
 */
ساکن صحیح
utf8encode3(حرف *str, غیرعلامتی صحیح val)
{
	str[2] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[1] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[0] = val | 0xE0;

	واپس 3;
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
قسمتعریف ثابت غیرعلامتی حرف utf8trie_t;
#تعریف_کرو BITNUM		0x07
#تعریف_کرو NEXTBYTE	0x08
#تعریف_کرو OFFLEN		0x30
#تعریف_کرو OFFLEN_سرکاؤ	4
#تعریف_کرو RIGHTPATH	0x40
#تعریف_کرو TRIENODE	0x80
#تعریف_کرو RIGHTNODE	0x40
#تعریف_کرو LEFTNODE	0x80

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
قسمتعریف ثابت غیرعلامتی حرف utf8leaf_t;

#تعریف_کرو برگ_GEN(برگ)	((برگ)[0])
#تعریف_کرو برگ_CCC(برگ)	((برگ)[1])
#تعریف_کرو برگ_STR(برگ)	((ثابت حرف *)((برگ) + 2))

#تعریف_کرو MINCCC		(0)
#تعریف_کرو MAXCCC		(254)
#تعریف_کرو STOPPER		(0)
#تعریف_کرو	DECOMPOSE	(255)

/* Marker for hangul syllable decomposition. */
#تعریف_کرو HANGUL		((حرف)(255))
/* Size of the synthesized leaf used for Hangul syllable decomposition. */
#تعریف_کرو UTF8HANGULLEAF	(12)

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
#تعریف_کرو SB	(0xAC00)
#تعریف_کرو LB	(0x1100)
#تعریف_کرو VB	(0x1161)
#تعریف_کرو TB	(0x11A7)
#تعریف_کرو LC	(19)
#تعریف_کرو VC	(21)
#تعریف_کرو TC	(28)
#تعریف_کرو NC	(VC * TC)
#تعریف_کرو SC	(LC * NC)

/* Algorithmic decomposition of hangul syllable. */
ساکن utf8leaf_t *
utf8hangul(ثابت حرف *str, غیرعلامتی حرف *hangul)
{
	غیرعلامتی صحیح	si;
	غیرعلامتی صحیح	li;
	غیرعلامتی صحیح	vi;
	غیرعلامتی صحیح	ti;
	غیرعلامتی حرف	*h;

	/* Calculate the SI, LI, VI, and TI values. */
	si = utf8decode3(str) - SB;
	li = si / NC;
	vi = (si % NC) / TC;
	ti = si % TC;

	/* Fill in base of leaf. */
	h = hangul;
	برگ_GEN(h) = 2;
	برگ_CCC(h) = DECOMPOSE;
	h += 2;

	/* Add LPart, a 3-byte UTF-8 sequence. */
	h += utf8encode3((حرف *)h, li + LB);

	/* Add VPart, a 3-byte UTF-8 sequence. */
	h += utf8encode3((حرف *)h, vi + VB);

	/* Add TPart if required, also a 3-byte UTF-8 sequence. */
	اگر (ti)
		h += utf8encode3((حرف *)h, ti + TB);

	/* Terminate string. */
	h[0] = '\0';

	واپس hangul;
}

/*
 * Use trie to scan s, touching at most len bytes.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * A non-NULL return guarantees that the UTF-8 sequence starting at s
 * is well-formed and corresponds to a known unicode code point.  The
 * shorthand for this will be "is valid UTF-8 unicode".
 */
ساکن utf8leaf_t *utf8nlookup(ثابت ساخت utf8data *ڈیٹا,
			       غیرعلامتی حرف *hangul, ثابت حرف *s, size_t len)
{
	utf8trie_t	*trie;
	صحیح		offlen;
	صحیح		انحراف;
	صحیح		نقاب;
	صحیح		گرہ;

	اگر (!ڈیٹا)
		واپس تہی;
	اگر (len == 0)
		واپس تہی;

	trie = utf8data + ڈیٹا->انحراف;
	گرہ = 1;
	جب (گرہ) {
		offlen = (*trie & OFFLEN) >> OFFLEN_سرکاؤ;
		اگر (*trie & NEXTBYTE) {
			اگر (--len == 0)
				واپس تہی;
			s++;
		}
		نقاب = 1 << (*trie & BITNUM);
		اگر (*s & نقاب) {
			/* Right leg */
			اگر (offlen) {
				/* Right node at offset of trie */
				گرہ = (*trie & RIGHTNODE);
				انحراف = trie[offlen];
				جب (--offlen) {
					انحراف <<= 8;
					انحراف |= trie[offlen];
				}
				trie += انحراف;
			} ورنہ اگر (*trie & RIGHTPATH) {
				/* Right node after this node */
				گرہ = (*trie & TRIENODE);
				trie++;
			} ورنہ {
				/* No right node. */
				واپس تہی;
			}
		} ورنہ {
			/* Left leg */
			اگر (offlen) {
				/* Left node after this node. */
				گرہ = (*trie & LEFTNODE);
				trie += offlen + 1;
			} ورنہ اگر (*trie & RIGHTPATH) {
				/* No left node. */
				واپس تہی;
			} ورنہ {
				/* Left node after this node */
				گرہ = (*trie & TRIENODE);
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
	اگر (برگ_CCC(trie) == DECOMPOSE && برگ_STR(trie)[0] == HANGUL)
		trie = utf8hangul(s - 2, hangul);
	واپس trie;
}

/*
 * Use trie to scan s.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * Forwards to utf8nlookup().
 */
ساکن utf8leaf_t *utf8lookup(ثابت ساخت utf8data *ڈیٹا,
			      غیرعلامتی حرف *hangul, ثابت حرف *s)
{
	واپس utf8nlookup(ڈیٹا, hangul, s, (size_t)-1);
}

#اگر 0
/*
 * Maximum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if only non-assigned code points are used.
 */
ساکن صحیح utf8agemax(ثابت ساخت utf8data *ڈیٹا, ثابت حرف *s)
{
	utf8leaf_t	*برگ_2;
	صحیح		age = 0;
	صحیح		برگ_age;
	غیرعلامتی حرف	hangul[UTF8HANGULLEAF];

	اگر (!ڈیٹا)
		واپس -1;

	جب (*s) {
		برگ_2 = utf8lookup(ڈیٹا, hangul, s);
		اگر (!برگ_2)
			واپس -1;

		برگ_age = utf8agetab[برگ_GEN(برگ_2)];
		اگر (برگ_age <= ڈیٹا->maxage && برگ_age > age)
			age = برگ_age;
		s += utf8clen(s);
	}
	واپس age;
}
#اگر_ختم

#اگر 0
/*
 * Minimum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if non-assigned code points are used.
 */
ساکن صحیح utf8agemin(ثابت ساخت utf8data *ڈیٹا, ثابت حرف *s)
{
	utf8leaf_t	*برگ_2;
	صحیح		age;
	صحیح		برگ_age;
	غیرعلامتی حرف	hangul[UTF8HANGULLEAF];

	اگر (!ڈیٹا)
		واپس -1;
	age = ڈیٹا->maxage;
	جب (*s) {
		برگ_2 = utf8lookup(ڈیٹا, hangul, s);
		اگر (!برگ_2)
			واپس -1;
		برگ_age = utf8agetab[برگ_GEN(برگ_2)];
		اگر (برگ_age <= ڈیٹا->maxage && برگ_age < age)
			age = برگ_age;
		s += utf8clen(s);
	}
	واپس age;
}
#اگر_ختم

#اگر 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
ساکن صحیح utf8nagemax(ثابت ساخت utf8data *ڈیٹا, ثابت حرف *s, size_t len)
{
	utf8leaf_t	*برگ_2;
	صحیح		age = 0;
	صحیح		برگ_age;
	غیرعلامتی حرف	hangul[UTF8HANGULLEAF];

	اگر (!ڈیٹا)
		واپس -1;

	جب (len && *s) {
		برگ_2 = utf8nlookup(ڈیٹا, hangul, s, len);
		اگر (!برگ_2)
			واپس -1;
		برگ_age = utf8agetab[برگ_GEN(برگ_2)];
		اگر (برگ_age <= ڈیٹا->maxage && برگ_age > age)
			age = برگ_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	واپس age;
}
#اگر_ختم

#اگر 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
ساکن صحیح utf8nagemin(ثابت ساخت utf8data *ڈیٹا, ثابت حرف *s, size_t len)
{
	utf8leaf_t	*برگ_2;
	صحیح		برگ_age;
	صحیح		age;
	غیرعلامتی حرف	hangul[UTF8HANGULLEAF];

	اگر (!ڈیٹا)
		واپس -1;
	age = ڈیٹا->maxage;
	جب (len && *s) {
		برگ_2 = utf8nlookup(ڈیٹا, hangul, s, len);
		اگر (!برگ_2)
			واپس -1;
		برگ_age = utf8agetab[برگ_GEN(برگ_2)];
		اگر (برگ_age <= ڈیٹا->maxage && برگ_age < age)
			age = برگ_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	واپس age;
}
#اگر_ختم

#اگر 0
/*
 * Length of the normalization of s.
 * Return -1 if s is not valid UTF-8 unicode.
 *
 * A string of Default_Ignorable_Code_Point has length 0.
 */
ساکن ssize_t utf8len(ثابت ساخت utf8data *ڈیٹا, ثابت حرف *s)
{
	utf8leaf_t	*برگ_2;
	size_t		ret = 0;
	غیرعلامتی حرف	hangul[UTF8HANGULLEAF];

	اگر (!ڈیٹا)
		واپس -1;
	جب (*s) {
		برگ_2 = utf8lookup(ڈیٹا, hangul, s);
		اگر (!برگ_2)
			واپس -1;
		اگر (utf8agetab[برگ_GEN(برگ_2)] > ڈیٹا->maxage)
			ret += utf8clen(s);
		ورنہ اگر (برگ_CCC(برگ_2) == DECOMPOSE)
			ret += لڑی_لمبائی(برگ_STR(برگ_2));
		ورنہ
			ret += utf8clen(s);
		s += utf8clen(s);
	}
	واپس ret;
}
#اگر_ختم

#اگر 0
/*
 * Length of the normalization of s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
ساکن ssize_t utf8nlen(ثابت ساخت utf8data *ڈیٹا, ثابت حرف *s, size_t len)
{
	utf8leaf_t	*برگ_2;
	size_t		ret = 0;
	غیرعلامتی حرف	hangul[UTF8HANGULLEAF];

	اگر (!ڈیٹا)
		واپس -1;
	جب (len && *s) {
		برگ_2 = utf8nlookup(ڈیٹا, hangul, s, len);
		اگر (!برگ_2)
			واپس -1;
		اگر (utf8agetab[برگ_GEN(برگ_2)] > ڈیٹا->maxage)
			ret += utf8clen(s);
		ورنہ اگر (برگ_CCC(برگ_2) == DECOMPOSE)
			ret += لڑی_لمبائی(برگ_STR(برگ_2));
		ورنہ
			ret += utf8clen(s);
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	واپس ret;
}
#اگر_ختم

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
ساکن صحیح utf8ncursor(ساخت utf8cursor *u8c, ثابت ساخت utf8data *ڈیٹا,
		ثابت حرف *s, size_t len)
{
	اگر (!ڈیٹا)
		واپس -1;
	اگر (!s)
		واپس -1;
	u8c->ڈیٹا = ڈیٹا;
	u8c->s = s;
	u8c->p = تہی;
	u8c->ss = تہی;
	u8c->sp = تہی;
	u8c->len = len;
	u8c->slen = 0;
	u8c->ccc = STOPPER;
	u8c->nccc = STOPPER;
	/* Check we didn't clobber the maximum length. */
	اگر (u8c->len != len)
		واپس -1;
	/* The first byte of s may not be an utf8 continuation. */
	اگر (len > 0 && (*s & 0xC0) == 0x80)
		واپس -1;
	واپس 0;
}

#اگر 0
/*
 * Set up an utf8cursor for use by utf8byte().
 *
 *   u8c    : pointer to cursor.
 *   data   : const struct utf8data to use for normalization.
 *   s      : NUL-terminated string.
 *
 * Returns -1 on error, 0 on success.
 */
ساکن صحیح utf8cursor(ساخت utf8cursor *u8c, ثابت ساخت utf8data *ڈیٹا,
	       ثابت حرف *s)
{
	واپس utf8ncursor(u8c, ڈیٹا, s, (غیرعلامتی صحیح)-1);
}
#اگر_ختم

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
ساکن صحیح utf8byte(ساخت utf8cursor *u8c)
{
	utf8leaf_t *برگ_2;
	صحیح ccc;

	برائے (;;) {
		/* Check for the end of a decomposed character. */
		اگر (u8c->p && *u8c->s == '\0') {
			u8c->s = u8c->p;
			u8c->p = تہی;
		}

		/* Check for end-of-string. */
		اگر (!u8c->p && (u8c->len == 0 || *u8c->s == '\0')) {
			/* There is no next byte. */
			اگر (u8c->ccc == STOPPER)
				واپس 0;
			/* End-of-string during a scan counts as a stopper. */
			ccc = STOPPER;
			جاؤ ccc_mismatch;
		} ورنہ اگر ((*u8c->s & 0xC0) == 0x80) {
			/* This is a continuation of the current character. */
			اگر (!u8c->p)
				u8c->len--;
			واپس (غیرعلامتی حرف)*u8c->s++;
		}

		/* Look up the data for the current character. */
		اگر (u8c->p) {
			برگ_2 = utf8lookup(u8c->ڈیٹا, u8c->hangul, u8c->s);
		} ورنہ {
			برگ_2 = utf8nlookup(u8c->ڈیٹا, u8c->hangul,
					   u8c->s, u8c->len);
		}

		/* No leaf found implies that the input is a binary blob. */
		اگر (!برگ_2)
			واپس -1;

		ccc = برگ_CCC(برگ_2);
		/* Characters that are too new have CCC 0. */
		اگر (utf8agetab[برگ_GEN(برگ_2)] > u8c->ڈیٹا->maxage) {
			ccc = STOPPER;
		} ورنہ اگر (ccc == DECOMPOSE) {
			u8c->len -= utf8clen(u8c->s);
			u8c->p = u8c->s + utf8clen(u8c->s);
			u8c->s = برگ_STR(برگ_2);
			/* Empty decomposition implies CCC 0. */
			اگر (*u8c->s == '\0') {
				اگر (u8c->ccc == STOPPER)
					جاری;
				ccc = STOPPER;
				جاؤ ccc_mismatch;
			}

			برگ_2 = utf8lookup(u8c->ڈیٹا, u8c->hangul, u8c->s);
			اگر (!برگ_2)
				واپس -1;
			ccc = برگ_CCC(برگ_2);
		}

		/*
		 * If this is not a stopper, then see if it updates
		 * the next canonical class to be emitted.
		 */
		اگر (ccc != STOPPER && u8c->ccc < ccc && ccc < u8c->nccc)
			u8c->nccc = ccc;

		/*
		 * Return the current byte if this is the current
		 * combining class.
		 */
		اگر (ccc == u8c->ccc) {
			اگر (!u8c->p)
				u8c->len--;
			واپس (غیرعلامتی حرف)*u8c->s++;
		}

		/* Current combining class mismatch. */
ccc_mismatch:
		اگر (u8c->nccc == STOPPER) {
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
			اگر (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} ورنہ اگر (ccc != STOPPER) {
			/* Not a stopper, and not the ccc we're emitting. */
			اگر (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} ورنہ اگر (u8c->nccc != MAXCCC + 1) {
			/* At a stopper, restart for next ccc. */
			u8c->ccc = u8c->nccc;
			u8c->nccc = MAXCCC + 1;
			u8c->s = u8c->ss;
			u8c->p = u8c->sp;
			u8c->len = u8c->slen;
		} ورنہ {
			/* All done, proceed from here. */
			u8c->ccc = STOPPER;
			u8c->nccc = STOPPER;
			u8c->sp = تہی;
			u8c->ss = تہی;
			u8c->slen = 0;
		}
	}
}

#اگر 0
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
ساکن ثابت ساخت utf8data *utf8nfdi(غیرعلامتی صحیح maxage)
{
	صحیح i = ARRAY_سائز(utf8nfdidata) - 1;

	جب (maxage < utf8nfdidata[i].maxage)
		i--;
	اگر (maxage > utf8nfdidata[i].maxage)
		واپس تہی;
	واپس &utf8nfdidata[i];
}
#اگر_ختم

ساکن ثابت ساخت utf8data *utf8nfdicf(غیرعلامتی صحیح maxage)
{
	صحیح i = ARRAY_سائز(utf8nfdicfdata) - 1;

	جب (maxage < utf8nfdicfdata[i].maxage)
		i--;
	اگر (maxage > utf8nfdicfdata[i].maxage)
		واپس تہی;
	واپس &utf8nfdicfdata[i];
}

ساکن صحیح utf8_casefold(ثابت ساخت ext2fs_nls_جدول *جدول,
			  ثابت غیرعلامتی حرف *str, size_t len,
			  غیرعلامتی حرف *dest, size_t dlen)
{
	ثابت ساخت utf8data *ڈیٹا = utf8nfdicf(جدول->نسخہ);
	ساخت utf8cursor cur;
	size_t nlen = 0;

	اگر (utf8ncursor(&cur, ڈیٹا, (ثابت حرف *) str, len) < 0)
		جاؤ invalid_seq;

	برائے (nlen = 0; nlen < dlen; nlen++) {
		صحیح c = utf8byte(&cur);

		dest[nlen] = c;
		اگر (!c)
			واپس nlen;
		اگر (c == -1)
			توڑو;
	}

	واپس -ENAMETOOLONG;

invalid_seq:
	اگر (dlen < len)
		واپس -ENAMETOOLONG;

	/* Signal invalid sequence */
	واپس -EINVAL;
}

ساکن صحیح utf8_validate(ثابت ساخت ext2fs_nls_جدول *جدول,
			 حرف *s, size_t len, حرف **pos)
{
	ثابت ساخت utf8data *ڈیٹا = utf8nfdicf(جدول->نسخہ);
	utf8leaf_t	*برگ_2;
	غیرعلامتی حرف	hangul[UTF8HANGULLEAF];

	اگر (!ڈیٹا)
		واپس -1;
	جب (len && *s) {
		برگ_2 = utf8nlookup(ڈیٹا, hangul, s, len);
		اگر (!برگ_2) {
			*pos = s;
			واپس 1;
		}
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	واپس 0;
}

ساکن صحیح utf8_casefold_cmp(ثابت ساخت ext2fs_nls_جدول *جدول,
			     ثابت غیرعلامتی حرف *str1, size_t len1,
			     ثابت غیرعلامتی حرف *str2, size_t len2)
{
	ثابت ساخت utf8data *ڈیٹا = utf8nfdicf(جدول->نسخہ);
	صحیح c1, c2;
	ساخت utf8cursor cur1, cur2;

	اگر (utf8ncursor(&cur1, ڈیٹا, (ثابت حرف *) str1, len1) < 0)
		واپس -1;
	اگر (utf8ncursor(&cur2, ڈیٹا, (ثابت حرف *) str2, len2) < 0)
		واپس -1;

	کرو {
		c1 = utf8byte(&cur1);
		c2 = utf8byte(&cur2);

		اگر (c1 < 0 || c2 < 0)
			واپس -1;
		اگر (c1 != c2)
			واپس c1 - c2;
	} جب (c1);

	واپس 0;
}

ساکن ثابت ساخت ext2fs_nls_ops utf8_ops = {
	.casefold = utf8_casefold,
	.validate = utf8_validate,
	.casefold_cmp = utf8_casefold_cmp,
};

ساکن ثابت ساخت ext2fs_nls_جدول nls_utf8 = {
	.ops = &utf8_ops,
	.نسخہ = UNICODE_AGE(12, 1, 0),
};

ثابت ساخت ext2fs_nls_جدول *ext2fs_بھرو_nls_جدول(صحیح encoding)
{
	اگر (encoding == EXT4_ENC_UTF8_12_1)
		واپس &nls_utf8;

	واپس تہی;
}

صحیح ext2fs_check_encoded_name(ثابت ساخت ext2fs_nls_جدول *جدول,
			      حرف *name, size_t len, حرف **pos)
{
	واپس جدول->ops->validate(جدول, name, len, pos);
}

صحیح ext2fs_casefold_cmp(ثابت ساخت ext2fs_nls_جدول *جدول,
			ثابت غیرعلامتی حرف *str1, size_t len1,
			ثابت غیرعلامتی حرف *str2, size_t len2)
{
	واپس جدول->ops->casefold_cmp(جدول, str1, len1, str2, len2);
}
