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

#समावेश "config.h"
#समावेश <stdint.h>
#समावेश <unistd.h>
#समावेश <string.h>
#समावेश <limits.h>
#समावेश <errno.h>

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"
#समावेश "ext2fsP.h"

/* Encoding a unicode version number as a single unsigned int. */
#घोषणा UNICODE_MAJ_SHIFT		(16)
#घोषणा UNICODE_MIN_SHIFT		(8)

#घोषणा UNICODE_AGE(MAJ, MIN, REV)			\
	(((अचिन्हित पूर्णांक)(MAJ) << UNICODE_MAJ_SHIFT) |	\
	 ((अचिन्हित पूर्णांक)(MIN) << UNICODE_MIN_SHIFT) |	\
	 ((अचिन्हित पूर्णांक)(REV)))

/* Needed in struct utf8cursor below. */
#घोषणा UTF8HANGULLEAF	(12)

/*
 * Cursor structure used by the normalizer.
 */
काष्ठा utf8cursor {
	स्थिर काष्ठा utf8data	*तथ्य;
	स्थिर अक्षर	*s;
	स्थिर अक्षर	*p;
	स्थिर अक्षर	*ss;
	स्थिर अक्षर	*sp;
	अचिन्हित पूर्णांक	len;
	अचिन्हित पूर्णांक	slen;
	लघु पूर्णांक	ccc;
	लघु पूर्णांक	nccc;
	अचिन्हित अक्षर	hangul[UTF8HANGULLEAF];
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


काष्ठा utf8data {
	अचिन्हित पूर्णांक maxage;
	अचिन्हित पूर्णांक offset;
};

#घोषणा __INCLUDED_FROM_UTF8NORM_C__
#समावेश "utf8data.h"
#अघोषित __INCLUDED_FROM_UTF8NORM_C__

#घोषणा ARRAY_आकार(array)			\
        (माप(array) / माप(array[0]))

#अगर 0
/* Highest unicode version supported by the data tables. */
अटल पूर्णांक utf8version_is_supported(uint8_t maj, uint8_t min, uint8_t rev)
{
	पूर्णांक i = ARRAY_आकार(utf8agetab) - 1;
	अचिन्हित पूर्णांक sb_utf8version = UNICODE_AGE(maj, min, rev);

	जबतक (i >= 0 && utf8agetab[i] != 0) {
		अगर (sb_utf8version == utf8agetab[i])
			वापस 1;
		i--;
	}
	वापस 0;
}
#पूर्ण_अगर

#अगर 0
अटल पूर्णांक utf8version_latest(व्योम)
{
	वापस utf8vers;
}
#पूर्ण_अगर

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
अटल अंतरभूत पूर्णांक utf8clen(स्थिर अक्षर *s)
{
	अचिन्हित अक्षर c = *s;

	वापस 1 + (c >= 0xC0) + (c >= 0xE0) + (c >= 0xF0);
}

/*
 * Decode a 3-byte UTF-8 sequence.
 */
अटल अचिन्हित पूर्णांक
utf8decode3(स्थिर अक्षर *str)
{
	अचिन्हित पूर्णांक		uc;

	uc = *str++ & 0x0F;
	uc <<= 6;
	uc |= *str++ & 0x3F;
	uc <<= 6;
	uc |= *str++ & 0x3F;

	वापस uc;
}

/*
 * Encode a 3-byte UTF-8 sequence.
 */
अटल पूर्णांक
utf8encode3(अक्षर *str, अचिन्हित पूर्णांक val)
{
	str[2] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[1] = (val & 0x3F) | 0x80;
	val >>= 6;
	str[0] = val | 0xE0;

	वापस 3;
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
प्रकार स्थिर अचिन्हित अक्षर utf8trie_t;
#घोषणा BITNUM		0x07
#घोषणा NEXTBYTE	0x08
#घोषणा OFFLEN		0x30
#घोषणा OFFLEN_SHIFT	4
#घोषणा RIGHTPATH	0x40
#घोषणा TRIENODE	0x80
#घोषणा RIGHTNODE	0x40
#घोषणा LEFTNODE	0x80

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
प्रकार स्थिर अचिन्हित अक्षर utf8leaf_t;

#घोषणा LEAF_GEN(LEAF)	((LEAF)[0])
#घोषणा LEAF_CCC(LEAF)	((LEAF)[1])
#घोषणा LEAF_STR(LEAF)	((स्थिर अक्षर *)((LEAF) + 2))

#घोषणा MINCCC		(0)
#घोषणा MAXCCC		(254)
#घोषणा STOPPER		(0)
#घोषणा	DECOMPOSE	(255)

/* Marker for hangul syllable decomposition. */
#घोषणा HANGUL		((अक्षर)(255))
/* Size of the synthesized leaf used for Hangul syllable decomposition. */
#घोषणा UTF8HANGULLEAF	(12)

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
#घोषणा SB	(0xAC00)
#घोषणा LB	(0x1100)
#घोषणा VB	(0x1161)
#घोषणा TB	(0x11A7)
#घोषणा LC	(19)
#घोषणा VC	(21)
#घोषणा TC	(28)
#घोषणा NC	(VC * TC)
#घोषणा SC	(LC * NC)

/* Algorithmic decomposition of hangul syllable. */
अटल utf8leaf_t *
utf8hangul(स्थिर अक्षर *str, अचिन्हित अक्षर *hangul)
{
	अचिन्हित पूर्णांक	si;
	अचिन्हित पूर्णांक	li;
	अचिन्हित पूर्णांक	vi;
	अचिन्हित पूर्णांक	ti;
	अचिन्हित अक्षर	*h;

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
	h += utf8encode3((अक्षर *)h, li + LB);

	/* Add VPart, a 3-byte UTF-8 sequence. */
	h += utf8encode3((अक्षर *)h, vi + VB);

	/* Add TPart if required, also a 3-byte UTF-8 sequence. */
	अगर (ti)
		h += utf8encode3((अक्षर *)h, ti + TB);

	/* Terminate string. */
	h[0] = '\0';

	वापस hangul;
}

/*
 * Use trie to scan s, touching at most len bytes.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * A non-NULL return guarantees that the UTF-8 sequence starting at s
 * is well-formed and corresponds to a known unicode code point.  The
 * shorthand for this will be "is valid UTF-8 unicode".
 */
अटल utf8leaf_t *utf8nlookup(स्थिर काष्ठा utf8data *तथ्य,
			       अचिन्हित अक्षर *hangul, स्थिर अक्षर *s, माप_प्रकार len)
{
	utf8trie_t	*trie;
	पूर्णांक		offlen;
	पूर्णांक		offset;
	पूर्णांक		mask;
	पूर्णांक		गाँठ;

	अगर (!तथ्य)
		वापस शून्य;
	अगर (len == 0)
		वापस शून्य;

	trie = utf8data + तथ्य->offset;
	गाँठ = 1;
	जबतक (गाँठ) {
		offlen = (*trie & OFFLEN) >> OFFLEN_SHIFT;
		अगर (*trie & NEXTBYTE) {
			अगर (--len == 0)
				वापस शून्य;
			s++;
		}
		mask = 1 << (*trie & BITNUM);
		अगर (*s & mask) {
			/* Right leg */
			अगर (offlen) {
				/* Right node at offset of trie */
				गाँठ = (*trie & RIGHTNODE);
				offset = trie[offlen];
				जबतक (--offlen) {
					offset <<= 8;
					offset |= trie[offlen];
				}
				trie += offset;
			} अन्यथा अगर (*trie & RIGHTPATH) {
				/* Right node after this node */
				गाँठ = (*trie & TRIENODE);
				trie++;
			} अन्यथा {
				/* No right node. */
				वापस शून्य;
			}
		} अन्यथा {
			/* Left leg */
			अगर (offlen) {
				/* Left node after this node. */
				गाँठ = (*trie & LEFTNODE);
				trie += offlen + 1;
			} अन्यथा अगर (*trie & RIGHTPATH) {
				/* No left node. */
				वापस शून्य;
			} अन्यथा {
				/* Left node after this node */
				गाँठ = (*trie & TRIENODE);
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
	अगर (LEAF_CCC(trie) == DECOMPOSE && LEAF_STR(trie)[0] == HANGUL)
		trie = utf8hangul(s - 2, hangul);
	वापस trie;
}

/*
 * Use trie to scan s.
 * Returns the leaf if one exists, NULL otherwise.
 *
 * Forwards to utf8nlookup().
 */
अटल utf8leaf_t *utf8lookup(स्थिर काष्ठा utf8data *तथ्य,
			      अचिन्हित अक्षर *hangul, स्थिर अक्षर *s)
{
	वापस utf8nlookup(तथ्य, hangul, s, (माप_प्रकार)-1);
}

#अगर 0
/*
 * Maximum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if only non-assigned code points are used.
 */
अटल पूर्णांक utf8agemax(स्थिर काष्ठा utf8data *तथ्य, स्थिर अक्षर *s)
{
	utf8leaf_t	*leaf;
	पूर्णांक		age = 0;
	पूर्णांक		leaf_age;
	अचिन्हित अक्षर	hangul[UTF8HANGULLEAF];

	अगर (!तथ्य)
		वापस -1;

	जबतक (*s) {
		leaf = utf8lookup(तथ्य, hangul, s);
		अगर (!leaf)
			वापस -1;

		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		अगर (leaf_age <= तथ्य->maxage && leaf_age > age)
			age = leaf_age;
		s += utf8clen(s);
	}
	वापस age;
}
#पूर्ण_अगर

#अगर 0
/*
 * Minimum age of any character in s.
 * Return -1 if s is not valid UTF-8 unicode.
 * Return 0 if non-assigned code points are used.
 */
अटल पूर्णांक utf8agemin(स्थिर काष्ठा utf8data *तथ्य, स्थिर अक्षर *s)
{
	utf8leaf_t	*leaf;
	पूर्णांक		age;
	पूर्णांक		leaf_age;
	अचिन्हित अक्षर	hangul[UTF8HANGULLEAF];

	अगर (!तथ्य)
		वापस -1;
	age = तथ्य->maxage;
	जबतक (*s) {
		leaf = utf8lookup(तथ्य, hangul, s);
		अगर (!leaf)
			वापस -1;
		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		अगर (leaf_age <= तथ्य->maxage && leaf_age < age)
			age = leaf_age;
		s += utf8clen(s);
	}
	वापस age;
}
#पूर्ण_अगर

#अगर 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
अटल पूर्णांक utf8nagemax(स्थिर काष्ठा utf8data *तथ्य, स्थिर अक्षर *s, माप_प्रकार len)
{
	utf8leaf_t	*leaf;
	पूर्णांक		age = 0;
	पूर्णांक		leaf_age;
	अचिन्हित अक्षर	hangul[UTF8HANGULLEAF];

	अगर (!तथ्य)
		वापस -1;

	जबतक (len && *s) {
		leaf = utf8nlookup(तथ्य, hangul, s, len);
		अगर (!leaf)
			वापस -1;
		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		अगर (leaf_age <= तथ्य->maxage && leaf_age > age)
			age = leaf_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	वापस age;
}
#पूर्ण_अगर

#अगर 0
/*
 * Maximum age of any character in s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
अटल पूर्णांक utf8nagemin(स्थिर काष्ठा utf8data *तथ्य, स्थिर अक्षर *s, माप_प्रकार len)
{
	utf8leaf_t	*leaf;
	पूर्णांक		leaf_age;
	पूर्णांक		age;
	अचिन्हित अक्षर	hangul[UTF8HANGULLEAF];

	अगर (!तथ्य)
		वापस -1;
	age = तथ्य->maxage;
	जबतक (len && *s) {
		leaf = utf8nlookup(तथ्य, hangul, s, len);
		अगर (!leaf)
			वापस -1;
		leaf_age = utf8agetab[LEAF_GEN(leaf)];
		अगर (leaf_age <= तथ्य->maxage && leaf_age < age)
			age = leaf_age;
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	वापस age;
}
#पूर्ण_अगर

#अगर 0
/*
 * Length of the normalization of s.
 * Return -1 if s is not valid UTF-8 unicode.
 *
 * A string of Default_Ignorable_Code_Point has length 0.
 */
अटल ssize_t utf8len(स्थिर काष्ठा utf8data *तथ्य, स्थिर अक्षर *s)
{
	utf8leaf_t	*leaf;
	माप_प्रकार		ret = 0;
	अचिन्हित अक्षर	hangul[UTF8HANGULLEAF];

	अगर (!तथ्य)
		वापस -1;
	जबतक (*s) {
		leaf = utf8lookup(तथ्य, hangul, s);
		अगर (!leaf)
			वापस -1;
		अगर (utf8agetab[LEAF_GEN(leaf)] > तथ्य->maxage)
			ret += utf8clen(s);
		अन्यथा अगर (LEAF_CCC(leaf) == DECOMPOSE)
			ret += म_माप(LEAF_STR(leaf));
		अन्यथा
			ret += utf8clen(s);
		s += utf8clen(s);
	}
	वापस ret;
}
#पूर्ण_अगर

#अगर 0
/*
 * Length of the normalization of s, touch at most len bytes.
 * Return -1 if s is not valid UTF-8 unicode.
 */
अटल ssize_t utf8nlen(स्थिर काष्ठा utf8data *तथ्य, स्थिर अक्षर *s, माप_प्रकार len)
{
	utf8leaf_t	*leaf;
	माप_प्रकार		ret = 0;
	अचिन्हित अक्षर	hangul[UTF8HANGULLEAF];

	अगर (!तथ्य)
		वापस -1;
	जबतक (len && *s) {
		leaf = utf8nlookup(तथ्य, hangul, s, len);
		अगर (!leaf)
			वापस -1;
		अगर (utf8agetab[LEAF_GEN(leaf)] > तथ्य->maxage)
			ret += utf8clen(s);
		अन्यथा अगर (LEAF_CCC(leaf) == DECOMPOSE)
			ret += म_माप(LEAF_STR(leaf));
		अन्यथा
			ret += utf8clen(s);
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	वापस ret;
}
#पूर्ण_अगर

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
अटल पूर्णांक utf8ncursor(काष्ठा utf8cursor *u8c, स्थिर काष्ठा utf8data *तथ्य,
		स्थिर अक्षर *s, माप_प्रकार len)
{
	अगर (!तथ्य)
		वापस -1;
	अगर (!s)
		वापस -1;
	u8c->तथ्य = तथ्य;
	u8c->s = s;
	u8c->p = शून्य;
	u8c->ss = शून्य;
	u8c->sp = शून्य;
	u8c->len = len;
	u8c->slen = 0;
	u8c->ccc = STOPPER;
	u8c->nccc = STOPPER;
	/* Check we didn't clobber the maximum length. */
	अगर (u8c->len != len)
		वापस -1;
	/* The first byte of s may not be an utf8 continuation. */
	अगर (len > 0 && (*s & 0xC0) == 0x80)
		वापस -1;
	वापस 0;
}

#अगर 0
/*
 * Set up an utf8cursor for use by utf8byte().
 *
 *   u8c    : pointer to cursor.
 *   data   : const struct utf8data to use for normalization.
 *   s      : NUL-terminated string.
 *
 * Returns -1 on error, 0 on success.
 */
अटल पूर्णांक utf8cursor(काष्ठा utf8cursor *u8c, स्थिर काष्ठा utf8data *तथ्य,
	       स्थिर अक्षर *s)
{
	वापस utf8ncursor(u8c, तथ्य, s, (अचिन्हित पूर्णांक)-1);
}
#पूर्ण_अगर

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
अटल पूर्णांक utf8byte(काष्ठा utf8cursor *u8c)
{
	utf8leaf_t *leaf;
	पूर्णांक ccc;

	क्रम (;;) {
		/* Check for the end of a decomposed character. */
		अगर (u8c->p && *u8c->s == '\0') {
			u8c->s = u8c->p;
			u8c->p = शून्य;
		}

		/* Check for end-of-string. */
		अगर (!u8c->p && (u8c->len == 0 || *u8c->s == '\0')) {
			/* There is no next byte. */
			अगर (u8c->ccc == STOPPER)
				वापस 0;
			/* End-of-string during a scan counts as a stopper. */
			ccc = STOPPER;
			जाओ ccc_mismatch;
		} अन्यथा अगर ((*u8c->s & 0xC0) == 0x80) {
			/* This is a continuation of the current character. */
			अगर (!u8c->p)
				u8c->len--;
			वापस (अचिन्हित अक्षर)*u8c->s++;
		}

		/* Look up the data for the current character. */
		अगर (u8c->p) {
			leaf = utf8lookup(u8c->तथ्य, u8c->hangul, u8c->s);
		} अन्यथा {
			leaf = utf8nlookup(u8c->तथ्य, u8c->hangul,
					   u8c->s, u8c->len);
		}

		/* No leaf found implies that the input is a binary blob. */
		अगर (!leaf)
			वापस -1;

		ccc = LEAF_CCC(leaf);
		/* Characters that are too new have CCC 0. */
		अगर (utf8agetab[LEAF_GEN(leaf)] > u8c->तथ्य->maxage) {
			ccc = STOPPER;
		} अन्यथा अगर (ccc == DECOMPOSE) {
			u8c->len -= utf8clen(u8c->s);
			u8c->p = u8c->s + utf8clen(u8c->s);
			u8c->s = LEAF_STR(leaf);
			/* Empty decomposition implies CCC 0. */
			अगर (*u8c->s == '\0') {
				अगर (u8c->ccc == STOPPER)
					जारी;
				ccc = STOPPER;
				जाओ ccc_mismatch;
			}

			leaf = utf8lookup(u8c->तथ्य, u8c->hangul, u8c->s);
			अगर (!leaf)
				वापस -1;
			ccc = LEAF_CCC(leaf);
		}

		/*
		 * If this is not a stopper, then see if it updates
		 * the next canonical class to be emitted.
		 */
		अगर (ccc != STOPPER && u8c->ccc < ccc && ccc < u8c->nccc)
			u8c->nccc = ccc;

		/*
		 * Return the current byte if this is the current
		 * combining class.
		 */
		अगर (ccc == u8c->ccc) {
			अगर (!u8c->p)
				u8c->len--;
			वापस (अचिन्हित अक्षर)*u8c->s++;
		}

		/* Current combining class mismatch. */
ccc_mismatch:
		अगर (u8c->nccc == STOPPER) {
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
			अगर (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} अन्यथा अगर (ccc != STOPPER) {
			/* Not a stopper, and not the ccc we're emitting. */
			अगर (!u8c->p)
				u8c->len -= utf8clen(u8c->s);
			u8c->s += utf8clen(u8c->s);
		} अन्यथा अगर (u8c->nccc != MAXCCC + 1) {
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
			u8c->sp = शून्य;
			u8c->ss = शून्य;
			u8c->slen = 0;
		}
	}
}

#अगर 0
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
अटल स्थिर काष्ठा utf8data *utf8nfdi(अचिन्हित पूर्णांक maxage)
{
	पूर्णांक i = ARRAY_आकार(utf8nfdidata) - 1;

	जबतक (maxage < utf8nfdidata[i].maxage)
		i--;
	अगर (maxage > utf8nfdidata[i].maxage)
		वापस शून्य;
	वापस &utf8nfdidata[i];
}
#पूर्ण_अगर

अटल स्थिर काष्ठा utf8data *utf8nfdicf(अचिन्हित पूर्णांक maxage)
{
	पूर्णांक i = ARRAY_आकार(utf8nfdicfdata) - 1;

	जबतक (maxage < utf8nfdicfdata[i].maxage)
		i--;
	अगर (maxage > utf8nfdicfdata[i].maxage)
		वापस शून्य;
	वापस &utf8nfdicfdata[i];
}

अटल पूर्णांक utf8_casefold(स्थिर काष्ठा ext2fs_nls_table *table,
			  स्थिर अचिन्हित अक्षर *str, माप_प्रकार len,
			  अचिन्हित अक्षर *dest, माप_प्रकार dlen)
{
	स्थिर काष्ठा utf8data *तथ्य = utf8nfdicf(table->version);
	काष्ठा utf8cursor cur;
	माप_प्रकार nlen = 0;

	अगर (utf8ncursor(&cur, तथ्य, (स्थिर अक्षर *) str, len) < 0)
		जाओ invalid_seq;

	क्रम (nlen = 0; nlen < dlen; nlen++) {
		पूर्णांक c = utf8byte(&cur);

		dest[nlen] = c;
		अगर (!c)
			वापस nlen;
		अगर (c == -1)
			अवरोध;
	}

	वापस -ENAMETOOLONG;

invalid_seq:
	अगर (dlen < len)
		वापस -ENAMETOOLONG;

	/* Signal invalid sequence */
	वापस -EINVAL;
}

अटल पूर्णांक utf8_validate(स्थिर काष्ठा ext2fs_nls_table *table,
			 अक्षर *s, माप_प्रकार len, अक्षर **pos)
{
	स्थिर काष्ठा utf8data *तथ्य = utf8nfdicf(table->version);
	utf8leaf_t	*leaf;
	अचिन्हित अक्षर	hangul[UTF8HANGULLEAF];

	अगर (!तथ्य)
		वापस -1;
	जबतक (len && *s) {
		leaf = utf8nlookup(तथ्य, hangul, s, len);
		अगर (!leaf) {
			*pos = s;
			वापस 1;
		}
		len -= utf8clen(s);
		s += utf8clen(s);
	}
	वापस 0;
}

अटल पूर्णांक utf8_casefold_cmp(स्थिर काष्ठा ext2fs_nls_table *table,
			     स्थिर अचिन्हित अक्षर *str1, माप_प्रकार len1,
			     स्थिर अचिन्हित अक्षर *str2, माप_प्रकार len2)
{
	स्थिर काष्ठा utf8data *तथ्य = utf8nfdicf(table->version);
	पूर्णांक c1, c2;
	काष्ठा utf8cursor cur1, cur2;

	अगर (utf8ncursor(&cur1, तथ्य, (स्थिर अक्षर *) str1, len1) < 0)
		वापस -1;
	अगर (utf8ncursor(&cur2, तथ्य, (स्थिर अक्षर *) str2, len2) < 0)
		वापस -1;

	करो {
		c1 = utf8byte(&cur1);
		c2 = utf8byte(&cur2);

		अगर (c1 < 0 || c2 < 0)
			वापस -1;
		अगर (c1 != c2)
			वापस c1 - c2;
	} जबतक (c1);

	वापस 0;
}

अटल स्थिर काष्ठा ext2fs_nls_ops utf8_ops = {
	.casefold = utf8_casefold,
	.validate = utf8_validate,
	.casefold_cmp = utf8_casefold_cmp,
};

अटल स्थिर काष्ठा ext2fs_nls_table nls_utf8 = {
	.ops = &utf8_ops,
	.version = UNICODE_AGE(12, 1, 0),
};

स्थिर काष्ठा ext2fs_nls_table *ext2fs_load_nls_table(पूर्णांक encoding)
{
	अगर (encoding == EXT4_ENC_UTF8_12_1)
		वापस &nls_utf8;

	वापस शून्य;
}

पूर्णांक ext2fs_check_encoded_name(स्थिर काष्ठा ext2fs_nls_table *table,
			      अक्षर *name, माप_प्रकार len, अक्षर **pos)
{
	वापस table->ops->validate(table, name, len, pos);
}

पूर्णांक ext2fs_casefold_cmp(स्थिर काष्ठा ext2fs_nls_table *table,
			स्थिर अचिन्हित अक्षर *str1, माप_प्रकार len1,
			स्थिर अचिन्हित अक्षर *str2, माप_प्रकार len2)
{
	वापस table->ops->casefold_cmp(table, str1, len1, str2, len2);
}
