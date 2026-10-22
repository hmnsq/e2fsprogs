/*
 * dirhash.c -- Calculate the hash of a directory entry
 *
 * Copyright (c) 2001  Daniel Phillips
 *
 * Copyright (c) 2002 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <limits.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

#यदि_अपरिभाषितम् PATH_MAX
#परिभाषय PATH_MAX 4096
#यदि_अन्तः

/*
 * Keyed 32-bit hash function using TEA in a Davis-Meyer function
 *   H0 = Key
 *   Hi = E Mi(Hi-1) + Hi-1
 *
 * (see Applied Cryptography, 2nd edition, p448).
 *
 * Jeremy Fitzhardinge <jeremy@zip.com.au> 1998
 *
 * This code is made available under the terms of the GPL
 */
#परिभाषय DELTA 0x9E3779B9

स्थिर शून्य TEA_transform(__u32 buf[4], __u32 ध्रुव अन्तर्[])
{
	__u32	योग = 0;
	__u32	b0 = buf[0], b1 = buf[1];
	__u32	a = अन्तर्[0], b = अन्तर्[1], c = अन्तर्[2], d = अन्तर्[3];
	पूर्णाङ्क	n = 16;

	कुरु {
		योग += DELTA;
		b0 += ((b1 << 4)+a) ^ (b1+योग) ^ ((b1 >> 5)+b);
		b1 += ((b0 << 4)+c) ^ (b0+योग) ^ ((b0 >> 5)+d);
	} यावत्(--n);

	buf[0] += b0;
	buf[1] += b1;
}

/* F, G and H are basic MD4 functions: selection, majority, parity */
#परिभाषय F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#परिभाषय G(x, y, z) (((x) & (y)) + (((x) ^ (y)) & (z)))
#परिभाषय H(x, y, z) ((x) ^ (y) ^ (z))

/*
 * The generic round function.  The application is so specific that
 * we don't bother protecting all the arguments with parens, as is generally
 * good macro practice, in favor of extra legibility.
 * Rotation is separate from addition to prevent recomputation
 */
#परिभाषय आवर्तनम्(f, a, b, c, d, x, s)	\
	(a += f(b, c, d) + x, a = (a << s) | (a >> (32-s)))
#परिभाषय K1 0
#परिभाषय K2 013240474631UL
#परिभाषय K3 015666365641UL

/*
 * Basic cut-down MD4 transform.  Returns only 32 bits of result.
 */
स्थिर शून्य halfMD4Transform (__u32 buf[4], __u32 ध्रुव अन्तर्[])
{
	__u32	a = buf[0], b = buf[1], c = buf[2], d = buf[3];

	/* Round 1 */
	आवर्तनम्(F, a, b, c, d, अन्तर्[0] + K1,  3);
	आवर्तनम्(F, d, a, b, c, अन्तर्[1] + K1,  7);
	आवर्तनम्(F, c, d, a, b, अन्तर्[2] + K1, 11);
	आवर्तनम्(F, b, c, d, a, अन्तर्[3] + K1, 19);
	आवर्तनम्(F, a, b, c, d, अन्तर्[4] + K1,  3);
	आवर्तनम्(F, d, a, b, c, अन्तर्[5] + K1,  7);
	आवर्तनम्(F, c, d, a, b, अन्तर्[6] + K1, 11);
	आवर्तनम्(F, b, c, d, a, अन्तर्[7] + K1, 19);

	/* Round 2 */
	आवर्तनम्(G, a, b, c, d, अन्तर्[1] + K2,  3);
	आवर्तनम्(G, d, a, b, c, अन्तर्[3] + K2,  5);
	आवर्तनम्(G, c, d, a, b, अन्तर्[5] + K2,  9);
	आवर्तनम्(G, b, c, d, a, अन्तर्[7] + K2, 13);
	आवर्तनम्(G, a, b, c, d, अन्तर्[0] + K2,  3);
	आवर्तनम्(G, d, a, b, c, अन्तर्[2] + K2,  5);
	आवर्तनम्(G, c, d, a, b, अन्तर्[4] + K2,  9);
	आवर्तनम्(G, b, c, d, a, अन्तर्[6] + K2, 13);

	/* Round 3 */
	आवर्तनम्(H, a, b, c, d, अन्तर्[3] + K3,  3);
	आवर्तनम्(H, d, a, b, c, अन्तर्[7] + K3,  9);
	आवर्तनम्(H, c, d, a, b, अन्तर्[2] + K3, 11);
	आवर्तनम्(H, b, c, d, a, अन्तर्[6] + K3, 15);
	आवर्तनम्(H, a, b, c, d, अन्तर्[1] + K3,  3);
	आवर्तनम्(H, d, a, b, c, अन्तर्[5] + K3,  9);
	आवर्तनम्(H, c, d, a, b, अन्तर्[0] + K3, 11);
	आवर्तनम्(H, b, c, d, a, अन्तर्[4] + K3, 15);

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
}

#अपरिभाषय आवर्तनम्
#अपरिभाषय F
#अपरिभाषय G
#अपरिभाषय H
#अपरिभाषय K1
#अपरिभाषय K2
#अपरिभाषय K3

/* The old legacy hash */
स्थिर ext2_dirhash_t dx_hack_hash (ध्रुव वर्ण *name, पूर्णाङ्क len,
				    पूर्णाङ्क unsigned_flag)
{
	__u32 hash, hash0 = 0x12a3fe2d, hash1 = 0x37abe8f9;
	ध्रुव अचिह्नित वर्ण *ucp = (ध्रुव अचिह्नित वर्ण *) name;
	ध्रुव चिह्नित वर्ण *scp = (ध्रुव चिह्नित वर्ण *) name;
	पूर्णाङ्क c;

	यावत् (len--) {
		यदि (unsigned_flag)
			c = (पूर्णाङ्क) *ucp++;
		अन्यथा
			c = (पूर्णाङ्क) *scp++;
		hash = hash1 + (hash0 ^ (c * 7152373));

		यदि (hash & 0x80000000) hash -= 0x7fffffff;
		hash1 = hash0;
		hash0 = hash;
	}
	प्रत्यागम (hash0 << 1);
}

स्थिर शून्य str2hashbuf(ध्रुव वर्ण *सन्देशः, पूर्णाङ्क len, __u32 *buf, पूर्णाङ्क अङ्कः,
			पूर्णाङ्क unsigned_flag)
{
	__u32	pad, val;
	पूर्णाङ्क	i, c;
	ध्रुव अचिह्नित वर्ण *ucp = (ध्रुव अचिह्नित वर्ण *) सन्देशः;
	ध्रुव चिह्नित वर्ण *scp = (ध्रुव चिह्नित वर्ण *) सन्देशः;

	pad = (__u32)len | ((__u32)len << 8);
	pad |= pad << 16;

	val = pad;
	यदि (len > अङ्कः*4)
		len = अङ्कः * 4;
	प्रति (i=0; i < len; i++) {
		यदि (unsigned_flag)
			c = (पूर्णाङ्क) ucp[i];
		अन्यथा
			c = (पूर्णाङ्क) scp[i];

		val = c + (val << 8);
		यदि ((i % 4) == 3) {
			*buf++ = val;
			val = pad;
			अङ्कः--;
		}
	}
	यदि (--अङ्कः >= 0)
		*buf++ = val;
	यावत् (--अङ्कः >= 0)
		*buf++ = pad;
}

/*
 * Returns the hash of a filename.  If len is 0 and name is NULL, then
 * this function can be used to test whether or not a hash version is
 * supported.
 *
 * The seed is an 4 longword (32 bits) "secret" which can be used to
 * uniquify a hash.  If the seed is all zero's, then some default seed
 * may be used.
 *
 * A particular hash version specifies whether or not the seed is
 * represented, and whether or not the returned hash is 32 bits or 64
 * bits.  32 bit hashes will return 0 for the minor hash.
 *
 * This function doesn't do any normalization or casefolding of the
 * input string.  To take charset encoding into account, use
 * ext2fs_dirhash2.
 *
 */
errcode_t ext2fs_dirhash(पूर्णाङ्क संस्करणम्, ध्रुव वर्ण *name, पूर्णाङ्क len,
			 ध्रुव __u32 *बीजम्,
			 ext2_dirhash_t *ret_hash,
			 ext2_dirhash_t *ret_minor_hash)
{
	__u32	hash;
	__u32	minor_hash = 0;
	ध्रुव वर्ण	*p;
	पूर्णाङ्क		i;
	__u32 		अन्तर्[8], buf[4];
	पूर्णाङ्क		unsigned_flag = 0;

	/* Initialize the default seed for the hash checksum functions */
	buf[0] = 0x67452301;
	buf[1] = 0xefcdab89;
	buf[2] = 0x98badcfe;
	buf[3] = 0x10325476;

	/* Check to see if the seed is all zero's */
	यदि (बीजम्) {
		प्रति (i=0; i < 4; i++) {
			यदि (बीजम्[i])
				भङ्ग;
		}
		यदि (i < 4)
			स्मृतिं_प्रतिलिख(buf, बीजम्, परिमाण(buf));
	}

	विकल्प (संस्करणम्) {
	पक्ष EXT2_HASH_LEGACY_UNSIGNED:
		unsigned_flag++;
		/* fallthrough */
	पक्ष EXT2_HASH_LEGACY:
		hash = dx_hack_hash(name, len, unsigned_flag);
		भङ्ग;
	पक्ष EXT2_HASH_HALF_MD4_UNSIGNED:
		unsigned_flag++;
		/* fallthrough */
	पक्ष EXT2_HASH_HALF_MD4:
		p = name;
		यावत् (len > 0) {
			str2hashbuf(p, len, अन्तर्, 8, unsigned_flag);
			halfMD4Transform(buf, अन्तर्);
			len -= 32;
			p += 32;
		}
		minor_hash = buf[2];
		hash = buf[1];
		भङ्ग;
	पक्ष EXT2_HASH_TEA_UNSIGNED:
		unsigned_flag++;
		/* fallthrough */
	पक्ष EXT2_HASH_TEA:
		p = name;
		यावत् (len > 0) {
			str2hashbuf(p, len, अन्तर्, 4, unsigned_flag);
			TEA_transform(buf, अन्तर्);
			len -= 16;
			p += 16;
		}
		hash = buf[0];
		minor_hash = buf[1];
		भङ्ग;
	स्वभाव:
		*ret_hash = 0;
		प्रत्यागम EXT2_ET_DIRHASH_UNSUPP;
	}
	*ret_hash = hash & ~1;
	यदि (ret_minor_hash)
		*ret_minor_hash = minor_hash;
	प्रत्यागम 0;
}

/*
 * Returns the hash of a filename considering normalization and
 * casefolding.  This is a wrapper around ext2fs_dirhash with string
 * encoding support based on the nls_table and the flags. Check
 * ext2fs_dirhash for documentation on the input and output parameters.
 */
errcode_t ext2fs_dirhash2(पूर्णाङ्क संस्करणम्, ध्रुव वर्ण *name, पूर्णाङ्क len,
			  ध्रुव संरचना ext2fs_nls_सारणी *charset,
			  पूर्णाङ्क hash_flags, ध्रुव __u32 *बीजम्,
			  ext2_dirhash_t *ret_hash,
			  ext2_dirhash_t *ret_minor_hash)
{
	errcode_t r;
	पूर्णाङ्क dlen;

	यदि (len && charset && (hash_flags & EXT4_CASEFOLD_FL)) {
		वर्ण buff[PATH_MAX];

		dlen = charset->ops->casefold(charset,
			      (ध्रुव अचिह्नित वर्ण *) name, len,
			      (अचिह्नित वर्ण *) buff, परिमाण(buff));
		यदि (dlen < 0) {
			यदि (dlen == -EINVAL)
				गच्छ opaque_seq;

			प्रत्यागम dlen;
		}
		r = ext2fs_dirhash(संस्करणम्, buff, dlen, बीजम्, ret_hash,
				   ret_minor_hash);
		प्रत्यागम r;
	}

opaque_seq:
	प्रत्यागम ext2fs_dirhash(संस्करणम्, name, len, बीजम्, ret_hash,
			      ret_minor_hash);
}
