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

#ضمن "config.h"
#ضمن <stdint.h>
#ضمن <unistd.h>
#ضمن <string.h>
#ضمن <limits.h>
#ضمن <errno.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

/* Encoding a unicode version number as a single unsigned int. */
#تعریف_کرو UNICODE_MAJ_کھسکاؤ		(16)
#تعریف_کرو UNICODE_MIN_کھسکاؤ		(8)

#تعریف_کرو UNICODE_AGE(MAJ, MIN, REV)			\
	(((بنانشانی صحيح)(MAJ) << UNICODE_MAJ_کھسکاؤ) |	\
	 ((بنانشانی صحيح)(MIN) << UNICODE_MIN_کھسکاؤ) |	\
	 ((بنانشانی صحيح)(REV)))

/* Needed in struct utf8cursor below. */
#تعریف_کرو UTF8HANGULLEAF	(12)

/*
 * Cursor structure used by the normalizer.
 */
بنية utf8cursor {
	ثابت بنية utf8data	*انکڑے;
	ثابت حرف	*s;
	ثابت حرف	*p;
	ثابت حرف	*ss;
	ثابت حرف	*sp;
	بنانشانی صحيح	len;
	بنانشانی صحيح	slen;
	چھوٹا صحيح	ccc;
	چھوٹا صحيح	nccc;
	بنانشانی حرف	hangul[UTF8HANGULLEAF];
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


بنية utf8data {
	بنانشانی صحيح maxage;
	بنانشانی صحيح وتھ;
};

#تعریف_کرو __INCLUDED_FROM_UTF8NORM_C__
#ضمن "utf8data.h"
#تعریف_رد __INCLUDED_FROM_UTF8NORM_C__

#تعریف_کرو ARRAY_ناپ(array)			\
        (حجم(array) / حجم(array[0]))

#إذا 0
/* Highest unicode version supported by the data tables. */
ساكن صحيح utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev)
{
	صحيح i = ARRAY_ناپ(utf8agetab) - 1;
	بنانشانی صحيح sb_utf8version = UNICODE_AGE(maj, min, rev);

	طالما (i >= 0 && utf8agetab[i] != 0) {
		إذا (sb_utf8version == utf8agetab[i])
			أرجع 1;
		i--;
	}
	أرجع 0;
}
#جے_انت

#إذا 0
ساكن صحيح utf8version_latest(فراغ)
{
	أرجع utf8vers;
}
#جے_انت

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
ساكن مضمن صحيح utf8clen(ثابت حرف *s)
{
	بنانشانی حرف c = *s;

	أرجع 1 + (c >= 0xC0) + (c >= 0xE0) + (c >= 0xF0);
}

/*
 * Decode a 3-byte UTF-8 sequence.
 */
ساكن بنانشانی صحيح
utf8decode3(ثابت حرف *str)
{
	بنانشانی صحيح		uc;

	uc = *str++ & 0x0F;
	uc <<= 6;
	uc |= *str++ & 0x3F;
	uc <<= 6;
	uc |= *str++ & 0x3F;

	أرجع uc;
}

/*
 * Encode a 3-byte UTF-8 sequence.
 */
ساكن صحيح
utf8encode3(حرف *str, بنانشانی صحيح val)
{
	str[2] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[1] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[0] = val | 0xE0;

	أرجع 3;
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
تعريفنوع ثابت بنانشانی حرف utf8trie_t;
#تعریف_کرو BITNUM		0x07
#تعریف_کرو NEXTBYTE	0x08
#تعریف_کرو OFFLEN		0x30
#تعریف_کرو OFFLEN_کھسکاؤ	4
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
تعريفنوع ثابت بنانشانی حرف utf8leaf_t;

#تعریف_کرو پتی_GEN(پتی)	((پتی)[0])
#تعریف_کرو پتی_CCC(پتی)	((پتی)[1])
#تعریف_کرو پتی_STR(پتی)	((ثابت حرف *)((پتی) + 2))

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
ساكن utf8leaf_t *
utf8hangul(ثابت حرف *str, بنانشانی حرف *hangul)
{
	بنانشانی صحيح	si;
	بنانشانی صحيح	li;
	بنانشانی صحيح	vi;
	بنانشانی صحيح	ti;
	بنانشانی حرف	*h;

	/* Calculate the SI, LI, VI, and TI values. */
	si = utf8decode3(str) - SB;
	li = si / NC;
	vi = (si % NC) / TC;
	ti = si % TC;

	/* Fill in base of leaf. */
	h = hangul;
	پتی_GEN(h) = 2;
	پتی_CCC(h) = DECOMPOSE;
	h += 2;

	/* Add LPart, a 3-byte UTF-8 sequence. */
	h += utf8encode3((حرف *)h, li + LB);

	/* Add VPart, a 3-byte UTF-8 sequence. */
	h += utf8encode3((حرف *)h, vi + VB);

	/* Add TPart if required, also a 3-byte UTF-8 sequence. */
	إذا (ti)
		h += utf8encode3((حرف *)h, ti + TB);

	/* Terminate string. */
	h[0] = '\0';

	أرجع hangul;
}

/*
 * Use trie to scan s, touching at most len bytes.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * A non-NULL return guarantees that the UTF-8 sequence starting at s
 * is well-formed and corresponds to a known unicode code point.  The
 * shorthand for this will be "is valid UTF-8 unicode".
 */
ساكن utf8leaf_t *utf8nlookup(ثابت بنية utf8data *انکڑے,
			       بنانشانی حرف *hangul, ثابت حرف *s, size_t len)
{
	utf8trie_t	*trie;
	صحيح		offlen;
	صحيح		وتھ;
	صحيح		نقاب;
	صحيح		گرہ;

	إذا (!انکڑے)
		أرجع عدم;
	إذا (len == 0)
		أرجع عدم;

	trie = utf8data + انکڑے->وتھ;
	گرہ = 1;
	طالما (گرہ) {
		offlen = (*trie & OFFLEN) >> OFFLEN_کھسکاؤ;
		إذا (*trie & NEXTBYTE) {
			إذا (--len == 0)
				أرجع عدم;
			s++;
		}
		نقاب = 1 << (*trie & BITNUM);
		إذا (*s & نقاب) {
			/* Right leg */
			إذا (offlen) {
				/* Right node at offset of trie */
				گرہ = (*trie & RIGHTNODE);
				وتھ = trie[offlen];
				طالما (--offlen) {
					وتھ <<= 8;
					وتھ |= trie[offlen];
				}
				trie += وتھ;
			} وإلا إذا (*trie & RIGHTPATH) {
				/* Right node after this node */
				گرہ = (*trie & TRIENODE);
				trie++;
			} وإلا {
				/* No right node. */
				أرجع عدم;
			}
		} وإلا {
			/* Left leg */
			إذا (offlen) {
				/* Left node after this node. */
				گرہ = (*trie & LEFTNODE);
				trie += offlen + 1;
			} وإلا إذا (*trie & RIGHTPATH) {
				/* No left node. */
				أرجع عدم;
			} وإلا {
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
	إذا (پتی_CCC(trie) == DECOMPOSE && پتی_STR(trie)[0] == HANGUL)
		trie = utf8hangul(s - 2, hangul);
	أرجع trie;
}

/*
 * Use trie to scan s.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * Forwards to utf8nlookup().
 */
ساكن utf8leaf_t *utf8lookup(ثابت بنية utf8data *انکڑے,
			      بنانشانی حرف *hangul, ثابت حرف *s)
{
	أرجع utf8nlookup(انکڑے, hangul, s, (size_t)-1);
}

#إذا 0
/*
 * Maximum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if only non-assigned code points are used.
 */
ساكن صحيح utf8agemax(ثابت بنية utf8data *انکڑے, ثابت حرف *s)
{
	utf8leaf_t	*پتی_2;
	صحيح		age = 0;
	صحيح		پتی_age;
	بنانشانی حرف	hangul[UTF8HANGULLEAF];

	إذا (!انکڑے)
		أرجع -1;

	طالما (*s) {
		پتی_2 = utf8lookup(انکڑے, hangul, s);
		إذا (!پتی_2)
			أرجع -1;

		پتی_age = utf8agetab[پتی_GEN(پتی_2)];
		إذا (پتی_age <= انکڑے->maxage && پتی_age > age)
			age = پتی_age;
		s += utf8clen(s);
	}
	أرجع age;
}
#جے_انت

#إذا 0
/*
 * Minimum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if non-assigned code points are used.
 */
ساكن صحيح utf8agemin(ثابت بنية utf8data *انکڑے, ثابت حرف *s)
{
	utf8leaf_t	*پتی_2;
	صحيح		age;
	صحيح		پتی_age;
	بنانشانی حرف	hangul[UTF8HANGULLEAF];

	إذا (!انکڑے)
		أرجع -1;
	age = انکڑے->maxage;
	طالما (*s) {
		پتی_2 = utf8lookup(انکڑے, hangul, s);
		إذا (!پتی_2)
			أرجع -1;
		پتی_age = utf8agetab[پتی_GEN(پتی_2)];
		إذا (پتی_age <= انکڑے->maxage && پتی_age < age)
			age = پتی_age;
		s += utf8clen(s);
	}
	أرجع age;
}
#جے_انت

#إذا 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
ساكن صحيح utf8nagemax(ثابت بنية utf8data *انکڑے, ثابت حرف *s, size_t len)
{
	utf8leaf_t	*پتی_2;
	صحيح		age = 0;
	صحيح		پتی_age;
	بنانشانی حرف	hangul[UTF8HANGULLEAF];

	إذا (!انکڑے)
		أرجع -1;

	طالما (len && *s) {
		پتی_2 = utf8nlookup(انکڑے, hangul, s, len);
		إذا (!پتی_2)
			أرجع -1;
		پتی_age = utf8agetab[پتی_GEN(پتی_2)];
		إذا (پتی_age <= انکڑے->maxage && پتی_age > age)
			age = پتی_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	أرجع age;
}
#جے_انت

#إذا 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
ساكن صحيح utf8nagemin(ثابت بنية utf8data *انکڑے, ثابت حرف *s, size_t len)
{
	utf8leaf_t	*پتی_2;
	صحيح		پتی_age;
	صحيح		age;
	بنانشانی حرف	hangul[UTF8HANGULLEAF];

	إذا (!انکڑے)
		أرجع -1;
	age = انکڑے->maxage;
	طالما (len && *s) {
		پتی_2 = utf8nlookup(انکڑے, hangul, s, len);
		إذا (!پتی_2)
			أرجع -1;
		پتی_age = utf8agetab[پتی_GEN(پتی_2)];
		إذا (پتی_age <= انکڑے->maxage && پتی_age < age)
			age = پتی_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	أرجع age;
}
#جے_انت

#إذا 0
/*
 * Length of the normalization of s.
 * Return -1 if s is not valid UTF-8 unicode.
 *
 * A string of Default_Ignorable_Code_Point has length 0.
 */
ساكن ssize_t utf8len(ثابت بنية utf8data *انکڑے, ثابت حرف *s)
{
	utf8leaf_t	*پتی_2;
	size_t		ret = 0;
	بنانشانی حرف	hangul[UTF8HANGULLEAF];

	إذا (!انکڑے)
		أرجع -1;
	طالما (*s) {
		پتی_2 = utf8lookup(انکڑے, hangul, s);
		إذا (!پتی_2)
			أرجع -1;
		إذا (utf8agetab[پتی_GEN(پتی_2)] > انکڑے->maxage)
			ret += utf8clen(s);
		وإلا إذا (پتی_CCC(پتی_2) == DECOMPOSE)
			ret += لڑی_لمبائی(پتی_STR(پتی_2));
		وإلا
			ret += utf8clen(s);
		s += utf8clen(s);
	}
	أرجع ret;
}
#جے_انت

#إذا 0
/*
 * Length of the normalization of s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
ساكن ssize_t utf8nlen(ثابت بنية utf8data *انکڑے, ثابت حرف *s, size_t len)
{
	utf8leaf_t	*پتی_2;
	size_t		ret = 0;
	بنانشانی حرف	hangul[UTF8HANGULLEAF];

	إذا (!انکڑے)
		أرجع -1;
	طالما (len && *s) {
		پتی_2 = utf8nlookup(انکڑے, hangul, s, len);
		إذا (!پتی_2)
			أرجع -1;
		إذا (utf8agetab[پتی_GEN(پتی_2)] > انکڑے->maxage)
			ret += utf8clen(s);
		وإلا إذا (پتی_CCC(پتی_2) == DECOMPOSE)
			ret += لڑی_لمبائی(پتی_STR(پتی_2));
		وإلا
			ret += utf8clen(s);
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	أرجع ret;
}
#جے_انت

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
ساكن صحيح utf8ncursor(بنية utf8cursor *u8c, ثابت بنية utf8data *انکڑے,
		ثابت حرف *s, size_t len)
{
	إذا (!انکڑے)
		أرجع -1;
	إذا (!s)
		أرجع -1;
	u8c->انکڑے = انکڑے;
	u8c->s = s;
	u8c->p = عدم;
	u8c->ss = عدم;
	u8c->sp = عدم;
	u8c->len = len;
	u8c->slen = 0;
	u8c->ccc = STOPPER;
	u8c->nccc = STOPPER;
	/* Check we didn't clobber the maximum length. */
	إذا (u8c->len != len)
		أرجع -1;
	/* The first byte of s may not be an utf8 continuation. */
	إذا (len > 0 && (*s & 0xC0) == 0x80)
		أرجع -1;
	أرجع 0;
}

#إذا 0
/*
 * Set up an utf8cursor for use by utf8byte().
 *
 *   u8c    : pointer to cursor.
 *   data   : const struct utf8data to use for normalization.
 *   s      : NUL-terminated string.
 *
 * Returns -1 on error, 0 on success.
 */
ساكن صحيح utf8cursor(بنية utf8cursor *u8c, ثابت بنية utf8data *انکڑے,
	       ثابت حرف *s)
{
	أرجع utf8ncursor(u8c, انکڑے, s, (بنانشانی صحيح)-1);
}
#جے_انت

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
ساكن صحيح utf8byte(بنية utf8cursor *u8c)
{
	utf8leaf_t *پتی_2;
	صحيح ccc;

	لكل (;;) {
		/* Check for the end of a decomposed character. */
		إذا (u8c->p && *u8c->s == '\0') {
			u8c->s = u8c->p;
			u8c->p = عدم;
		}

		/* Check for end-of-string. */
		إذا (!u8c->p && (u8c->len == 0 || *u8c->s == '\0')) {
			/* There is no next byte. */
			إذا (u8c->ccc == STOPPER)
				أرجع 0;
			/* End-of-string during a scan counts as a stopper. */
			ccc = STOPPER;
			اذهب ccc_mismatch;
		} وإلا إذا ((*u8c->s & 0xC0) == 0x80) {
			/* This is a continuation of the current character. */
			إذا (!u8c->p)
				u8c->len--;
			أرجع (بنانشانی حرف)*u8c->s++;
		}

		/* Look up the data for the current character. */
		إذا (u8c->p) {
			پتی_2 = utf8lookup(u8c->انکڑے, u8c->hangul, u8c->s);
		} وإلا {
			پتی_2 = utf8nlookup(u8c->انکڑے, u8c->hangul,
					   u8c->s, u8c->len);
		}

		/* No leaf found implies that the input is a binary blob. */
		إذا (!پتی_2)
			أرجع -1;

		ccc = پتی_CCC(پتی_2);
		/* Characters that are too new have CCC 0. */
		إذا (utf8agetab[پتی_GEN(پتی_2)] > u8c->انکڑے->maxage) {
			ccc = STOPPER;
		} وإلا إذا (ccc == DECOMPOSE) {
			u8c->len -= utf8clen(u8c->s);
			u8c->p = u8c->s + utf8clen(u8c->s);
			u8c->s = پتی_STR(پتی_2);
			/* Empty decomposition implies CCC 0. */
			إذا (*u8c->s == '\0') {
				إذا (u8c->ccc == STOPPER)
					استمر;
				ccc = STOPPER;
				اذهب ccc_mismatch;
			}

			پتی_2 = utf8lookup(u8c->انکڑے, u8c->hangul, u8c->s);
			إذا (!پتی_2)
				أرجع -1;
			ccc = پتی_CCC(پتی_2);
		}

		/*
		 * If this is not a stopper, then see if it updates
		 * the next canonical class to be emitted.
		 */
		إذا (ccc != STOPPER && u8c->ccc < ccc && ccc < u8c->nccc)
			u8c->nccc = ccc;

		/*
		 * Return the current byte if this is the current
		 * combining class.
		 */
		إذا (ccc == u8c->ccc) {
			إذا (!u8c->p)
				u8c->len--;
			أرجع (بنانشانی حرف)*u8c->s++;
		}

		/* Current combining class mismatch. */
ccc_mismatch:
		إذا (u8c->nccc == STOPPER) {
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
			إذا (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} وإلا إذا (ccc != STOPPER) {
			/* Not a stopper, and not the ccc we're emitting. */
			إذا (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} وإلا إذا (u8c->nccc != MAXCCC + 1) {
			/* At a stopper, restart for next ccc. */
			u8c->ccc = u8c->nccc;
			u8c->nccc = MAXCCC + 1;
			u8c->s = u8c->ss;
			u8c->p = u8c->sp;
			u8c->len = u8c->slen;
		} وإلا {
			/* All done, proceed from here. */
			u8c->ccc = STOPPER;
			u8c->nccc = STOPPER;
			u8c->sp = عدم;
			u8c->ss = عدم;
			u8c->slen = 0;
		}
	}
}

#إذا 0
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
ساكن ثابت بنية utf8data *utf8nfdi(بنانشانی صحيح maxage)
{
	صحيح i = ARRAY_ناپ(utf8nfdidata) - 1;

	طالما (maxage < utf8nfdidata[i].maxage)
		i--;
	إذا (maxage > utf8nfdidata[i].maxage)
		أرجع عدم;
	أرجع &utf8nfdidata[i];
}
#جے_انت

ساكن ثابت بنية utf8data *utf8nfdicf(بنانشانی صحيح maxage)
{
	صحيح i = ARRAY_ناپ(utf8nfdicfdata) - 1;

	طالما (maxage < utf8nfdicfdata[i].maxage)
		i--;
	إذا (maxage > utf8nfdicfdata[i].maxage)
		أرجع عدم;
	أرجع &utf8nfdicfdata[i];
}

ساكن صحيح utf8_casefold(ثابت بنية ext2fs_nls_جدول *جدول,
			  ثابت بنانشانی حرف *str, size_t len,
			  بنانشانی حرف *dest, size_t dlen)
{
	ثابت بنية utf8data *انکڑے = utf8nfdicf(جدول->نسخہ);
	بنية utf8cursor cur;
	size_t nlen = 0;

	إذا (utf8ncursor(&cur, انکڑے, (ثابت حرف *) str, len) < 0)
		اذهب invalid_seq;

	لكل (nlen = 0; nlen < dlen; nlen++) {
		صحيح c = utf8byte(&cur);

		dest[nlen] = c;
		إذا (!c)
			أرجع nlen;
		إذا (c == -1)
			اكسر;
	}

	أرجع -ENAMETOOLONG;

invalid_seq:
	إذا (dlen < len)
		أرجع -ENAMETOOLONG;

	/* Signal invalid sequence */
	أرجع -EINVAL;
}

ساكن صحيح utf8_validate(ثابت بنية ext2fs_nls_جدول *جدول,
			 حرف *s, size_t len, حرف **pos)
{
	ثابت بنية utf8data *انکڑے = utf8nfdicf(جدول->نسخہ);
	utf8leaf_t	*پتی_2;
	بنانشانی حرف	hangul[UTF8HANGULLEAF];

	إذا (!انکڑے)
		أرجع -1;
	طالما (len && *s) {
		پتی_2 = utf8nlookup(انکڑے, hangul, s, len);
		إذا (!پتی_2) {
			*pos = s;
			أرجع 1;
		}
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	أرجع 0;
}

ساكن صحيح utf8_casefold_cmp(ثابت بنية ext2fs_nls_جدول *جدول,
			     ثابت بنانشانی حرف *str1, size_t len1,
			     ثابت بنانشانی حرف *str2, size_t len2)
{
	ثابت بنية utf8data *انکڑے = utf8nfdicf(جدول->نسخہ);
	صحيح c1, c2;
	بنية utf8cursor cur1, cur2;

	إذا (utf8ncursor(&cur1, انکڑے, (ثابت حرف *) str1, len1) < 0)
		أرجع -1;
	إذا (utf8ncursor(&cur2, انکڑے, (ثابت حرف *) str2, len2) < 0)
		أرجع -1;

	افعل {
		c1 = utf8byte(&cur1);
		c2 = utf8byte(&cur2);

		إذا (c1 < 0 || c2 < 0)
			أرجع -1;
		إذا (c1 != c2)
			أرجع c1 - c2;
	} طالما (c1);

	أرجع 0;
}

ساكن ثابت بنية ext2fs_nls_ops utf8_ops = {
	.casefold = utf8_casefold,
	.validate = utf8_validate,
	.casefold_cmp = utf8_casefold_cmp,
};

ساكن ثابت بنية ext2fs_nls_جدول nls_utf8 = {
	.ops = &utf8_ops,
	.نسخہ = UNICODE_AGE(12, 1, 0),
};

ثابت بنية ext2fs_nls_جدول *ext2fs_لدو_nls_جدول(صحيح encoding)
{
	إذا (encoding == EXT4_ENC_UTF8_12_1)
		أرجع &nls_utf8;

	أرجع عدم;
}

صحيح ext2fs_check_encoded_name(ثابت بنية ext2fs_nls_جدول *جدول,
			      حرف *name, size_t len, حرف **pos)
{
	أرجع جدول->ops->validate(جدول, name, len, pos);
}

صحيح ext2fs_casefold_cmp(ثابت بنية ext2fs_nls_جدول *جدول,
			ثابت بنانشانی حرف *str1, size_t len1,
			ثابت بنانشانی حرف *str2, size_t len2)
{
	أرجع جدول->ops->casefold_cmp(جدول, str1, len1, str2, len2);
}
