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

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

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
#define DELTA 0x9E3779B9

स्थिर शून्य TEA_transform(__u32 buf[4], __u32 स्थिरांक in[])
{
	__u32	sum = 0;
	__u32	b0 = buf[0], b1 = buf[1];
	__u32	a = in[0], b = in[1], c = in[2], d = in[3];
	पूर्णांक	n = 16;

	करा {
		sum += DELTA;
		b0 += ((b1 << 4)+a) ^ (b1+sum) ^ ((b1 >> 5)+b);
		b1 += ((b0 << 4)+c) ^ (b0+sum) ^ ((b0 >> 5)+d);
	} पर्यंत(--n);

	buf[0] += b0;
	buf[1] += b1;
}

/* F, G and H are basic MD4 functions: selection, majority, parity */
#define F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z) (((x) & (y)) + (((x) ^ (y)) & (z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))

/*
 * The generic round function.  The application is so specific that
 * we don't bother protecting all the arguments with parens, as is generally
 * good macro practice, in favor of extra legibility.
 * Rotation is separate from addition to prevent recomputation
 */
#define ROUND(f, a, b, c, d, x, s)	\
	(a += f(b, c, d) + x, a = (a << s) | (a >> (32-s)))
#define K1 0
#define K2 013240474631UL
#define K3 015666365641UL

/*
 * Basic cut-down MD4 transform.  Returns only 32 bits of result.
 */
स्थिर शून्य halfMD4Transform (__u32 buf[4], __u32 स्थिरांक in[])
{
	__u32	a = buf[0], b = buf[1], c = buf[2], d = buf[3];

	/* Round 1 */
	ROUND(F, a, b, c, d, in[0] + K1,  3);
	ROUND(F, d, a, b, c, in[1] + K1,  7);
	ROUND(F, c, d, a, b, in[2] + K1, 11);
	ROUND(F, b, c, d, a, in[3] + K1, 19);
	ROUND(F, a, b, c, d, in[4] + K1,  3);
	ROUND(F, d, a, b, c, in[5] + K1,  7);
	ROUND(F, c, d, a, b, in[6] + K1, 11);
	ROUND(F, b, c, d, a, in[7] + K1, 19);

	/* Round 2 */
	ROUND(G, a, b, c, d, in[1] + K2,  3);
	ROUND(G, d, a, b, c, in[3] + K2,  5);
	ROUND(G, c, d, a, b, in[5] + K2,  9);
	ROUND(G, b, c, d, a, in[7] + K2, 13);
	ROUND(G, a, b, c, d, in[0] + K2,  3);
	ROUND(G, d, a, b, c, in[2] + K2,  5);
	ROUND(G, c, d, a, b, in[4] + K2,  9);
	ROUND(G, b, c, d, a, in[6] + K2, 13);

	/* Round 3 */
	ROUND(H, a, b, c, d, in[3] + K3,  3);
	ROUND(H, d, a, b, c, in[7] + K3,  9);
	ROUND(H, c, d, a, b, in[2] + K3, 11);
	ROUND(H, b, c, d, a, in[6] + K3, 15);
	ROUND(H, a, b, c, d, in[1] + K3,  3);
	ROUND(H, d, a, b, c, in[5] + K3,  9);
	ROUND(H, c, d, a, b, in[0] + K3, 11);
	ROUND(H, b, c, d, a, in[4] + K3, 15);

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
}

#undef ROUND
#undef F
#undef G
#undef H
#undef K1
#undef K2
#undef K3

/* The old legacy hash */
स्थिर ext2_dirhash_t dx_hack_hash (स्थिरांक अक्षर *name, पूर्णांक len,
				    पूर्णांक unsigned_flag)
{
	__u32 hash, hash0 = 0x12a3fe2d, hash1 = 0x37abe8f9;
	स्थिरांक अचिन्हित अक्षर *ucp = (स्थिरांक अचिन्हित अक्षर *) name;
	स्थिरांक चिन्हित अक्षर *scp = (स्थिरांक चिन्हित अक्षर *) name;
	पूर्णांक c;

	पर्यंत (len--) {
		जर (unsigned_flag)
			c = (पूर्णांक) *ucp++;
		अन्यथा
			c = (पूर्णांक) *scp++;
		hash = hash1 + (hash0 ^ (c * 7152373));

		जर (hash & 0x80000000) hash -= 0x7fffffff;
		hash1 = hash0;
		hash0 = hash;
	}
	परत (hash0 << 1);
}

स्थिर शून्य str2hashbuf(स्थिरांक अक्षर *संदेश, पूर्णांक len, __u32 *buf, पूर्णांक अंक,
			पूर्णांक unsigned_flag)
{
	__u32	pad, val;
	पूर्णांक	i, c;
	स्थिरांक अचिन्हित अक्षर *ucp = (स्थिरांक अचिन्हित अक्षर *) संदेश;
	स्थिरांक चिन्हित अक्षर *scp = (स्थिरांक चिन्हित अक्षर *) संदेश;

	pad = (__u32)len | ((__u32)len << 8);
	pad |= pad << 16;

	val = pad;
	जर (len > अंक*4)
		len = अंक * 4;
	साठी (i=0; i < len; i++) {
		जर (unsigned_flag)
			c = (पूर्णांक) ucp[i];
		अन्यथा
			c = (पूर्णांक) scp[i];

		val = c + (val << 8);
		जर ((i % 4) == 3) {
			*buf++ = val;
			val = pad;
			अंक--;
		}
	}
	जर (--अंक >= 0)
		*buf++ = val;
	पर्यंत (--अंक >= 0)
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
errcode_t ext2fs_dirhash(पूर्णांक version, स्थिरांक अक्षर *name, पूर्णांक len,
			 स्थिरांक __u32 *seed,
			 ext2_dirhash_t *ret_hash,
			 ext2_dirhash_t *ret_minor_hash)
{
	__u32	hash;
	__u32	minor_hash = 0;
	स्थिरांक अक्षर	*p;
	पूर्णांक		i;
	__u32 		in[8], buf[4];
	पूर्णांक		unsigned_flag = 0;

	/* Initialize the default seed for the hash checksum functions */
	buf[0] = 0x67452301;
	buf[1] = 0xefcdab89;
	buf[2] = 0x98badcfe;
	buf[3] = 0x10325476;

	/* Check to see if the seed is all zero's */
	जर (seed) {
		साठी (i=0; i < 4; i++) {
			जर (seed[i])
				थांबा;
		}
		जर (i < 4)
			स_नकल(buf, seed, आकार(buf));
	}

	निवड (version) {
	स्थिती EXT2_HASH_LEGACY_UNSIGNED:
		unsigned_flag++;
		/* fallthrough */
	स्थिती EXT2_HASH_LEGACY:
		hash = dx_hack_hash(name, len, unsigned_flag);
		थांबा;
	स्थिती EXT2_HASH_HALF_MD4_UNSIGNED:
		unsigned_flag++;
		/* fallthrough */
	स्थिती EXT2_HASH_HALF_MD4:
		p = name;
		पर्यंत (len > 0) {
			str2hashbuf(p, len, in, 8, unsigned_flag);
			halfMD4Transform(buf, in);
			len -= 32;
			p += 32;
		}
		minor_hash = buf[2];
		hash = buf[1];
		थांबा;
	स्थिती EXT2_HASH_TEA_UNSIGNED:
		unsigned_flag++;
		/* fallthrough */
	स्थिती EXT2_HASH_TEA:
		p = name;
		पर्यंत (len > 0) {
			str2hashbuf(p, len, in, 4, unsigned_flag);
			TEA_transform(buf, in);
			len -= 16;
			p += 16;
		}
		hash = buf[0];
		minor_hash = buf[1];
		थांबा;
	पूर्वनिर्धारित:
		*ret_hash = 0;
		परत EXT2_ET_DIRHASH_UNSUPP;
	}
	*ret_hash = hash & ~1;
	जर (ret_minor_hash)
		*ret_minor_hash = minor_hash;
	परत 0;
}

/*
 * Returns the hash of a filename considering normalization and
 * casefolding.  This is a wrapper around ext2fs_dirhash with string
 * encoding support based on the nls_table and the flags. Check
 * ext2fs_dirhash for documentation on the input and output parameters.
 */
errcode_t ext2fs_dirhash2(पूर्णांक version, स्थिरांक अक्षर *name, पूर्णांक len,
			  स्थिरांक संरचना ext2fs_nls_table *charset,
			  पूर्णांक hash_flags, स्थिरांक __u32 *seed,
			  ext2_dirhash_t *ret_hash,
			  ext2_dirhash_t *ret_minor_hash)
{
	errcode_t r;
	पूर्णांक dlen;

	जर (len && charset && (hash_flags & EXT4_CASEFOLD_FL)) {
		अक्षर buff[PATH_MAX];

		dlen = charset->ops->casefold(charset,
			      (स्थिरांक अचिन्हित अक्षर *) name, len,
			      (अचिन्हित अक्षर *) buff, आकार(buff));
		जर (dlen < 0) {
			जर (dlen == -EINVAL)
				जा opaque_seq;

			परत dlen;
		}
		r = ext2fs_dirhash(version, buff, dlen, seed, ret_hash,
				   ret_minor_hash);
		परत r;
	}

opaque_seq:
	परत ext2fs_dirhash(version, name, len, seed, ret_hash,
			      ret_minor_hash);
}
