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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <limits.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ PATH_MAX
#ವ್ಯಾಖ್ಯಾನಿಸು PATH_MAX 4096
#ಆದರೆ_ಅಂತ್ಯ

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
#ವ್ಯಾಖ್ಯಾನಿಸು DELTA 0x9E3779B9

ಸ್ಥಿರ ಶೂನ್ಯ TEA_transform(__u32 buf[4], __u32 ಸ್ಥಿರಾಂಕ ಒಳ[])
{
	__u32	ಮೊತ್ತ = 0;
	__u32	b0 = buf[0], b1 = buf[1];
	__u32	a = ಒಳ[0], b = ಒಳ[1], c = ಒಳ[2], d = ಒಳ[3];
	ಪೂರ್ಣಾಂಕ	n = 16;

	ಮಾಡು {
		ಮೊತ್ತ += DELTA;
		b0 += ((b1 << 4)+a) ^ (b1+ಮೊತ್ತ) ^ ((b1 >> 5)+b);
		b1 += ((b0 << 4)+c) ^ (b0+ಮೊತ್ತ) ^ ((b0 >> 5)+d);
	} ಆಗ(--n);

	buf[0] += b0;
	buf[1] += b1;
}

/* F, G and H are basic MD4 functions: selection, majority, parity */
#ವ್ಯಾಖ್ಯಾನಿಸು F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#ವ್ಯಾಖ್ಯಾನಿಸು G(x, y, z) (((x) & (y)) + (((x) ^ (y)) & (z)))
#ವ್ಯಾಖ್ಯಾನಿಸು H(x, y, z) ((x) ^ (y) ^ (z))

/*
 * The generic round function.  The application is so specific that
 * we don't bother protecting all the arguments with parens, as is generally
 * good macro practice, in favor of extra legibility.
 * Rotation is separate from addition to prevent recomputation
 */
#ವ್ಯಾಖ್ಯಾನಿಸು ಸುತ್ತು(f, a, b, c, d, x, s)	\
	(a += f(b, c, d) + x, a = (a << s) | (a >> (32-s)))
#ವ್ಯಾಖ್ಯಾನಿಸು K1 0
#ವ್ಯಾಖ್ಯಾನಿಸು K2 013240474631UL
#ವ್ಯಾಖ್ಯಾನಿಸು K3 015666365641UL

/*
 * Basic cut-down MD4 transform.  Returns only 32 bits of result.
 */
ಸ್ಥಿರ ಶೂನ್ಯ halfMD4Transform (__u32 buf[4], __u32 ಸ್ಥಿರಾಂಕ ಒಳ[])
{
	__u32	a = buf[0], b = buf[1], c = buf[2], d = buf[3];

	/* Round 1 */
	ಸುತ್ತು(F, a, b, c, d, ಒಳ[0] + K1,  3);
	ಸುತ್ತು(F, d, a, b, c, ಒಳ[1] + K1,  7);
	ಸುತ್ತು(F, c, d, a, b, ಒಳ[2] + K1, 11);
	ಸುತ್ತು(F, b, c, d, a, ಒಳ[3] + K1, 19);
	ಸುತ್ತು(F, a, b, c, d, ಒಳ[4] + K1,  3);
	ಸುತ್ತು(F, d, a, b, c, ಒಳ[5] + K1,  7);
	ಸುತ್ತು(F, c, d, a, b, ಒಳ[6] + K1, 11);
	ಸುತ್ತು(F, b, c, d, a, ಒಳ[7] + K1, 19);

	/* Round 2 */
	ಸುತ್ತು(G, a, b, c, d, ಒಳ[1] + K2,  3);
	ಸುತ್ತು(G, d, a, b, c, ಒಳ[3] + K2,  5);
	ಸುತ್ತು(G, c, d, a, b, ಒಳ[5] + K2,  9);
	ಸುತ್ತು(G, b, c, d, a, ಒಳ[7] + K2, 13);
	ಸುತ್ತು(G, a, b, c, d, ಒಳ[0] + K2,  3);
	ಸುತ್ತು(G, d, a, b, c, ಒಳ[2] + K2,  5);
	ಸುತ್ತು(G, c, d, a, b, ಒಳ[4] + K2,  9);
	ಸುತ್ತು(G, b, c, d, a, ಒಳ[6] + K2, 13);

	/* Round 3 */
	ಸುತ್ತು(H, a, b, c, d, ಒಳ[3] + K3,  3);
	ಸುತ್ತು(H, d, a, b, c, ಒಳ[7] + K3,  9);
	ಸುತ್ತು(H, c, d, a, b, ಒಳ[2] + K3, 11);
	ಸುತ್ತು(H, b, c, d, a, ಒಳ[6] + K3, 15);
	ಸುತ್ತು(H, a, b, c, d, ಒಳ[1] + K3,  3);
	ಸುತ್ತು(H, d, a, b, c, ಒಳ[5] + K3,  9);
	ಸುತ್ತು(H, c, d, a, b, ಒಳ[0] + K3, 11);
	ಸುತ್ತು(H, b, c, d, a, ಒಳ[4] + K3, 15);

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
}

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ಸುತ್ತು
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು F
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು G
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು H
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು K1
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು K2
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು K3

/* The old legacy hash */
ಸ್ಥಿರ ext2_dirhash_t dx_hack_hash (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ len,
				    ಪೂರ್ಣಾಂಕ unsigned_flag)
{
	__u32 hash, hash0 = 0x12a3fe2d, hash1 = 0x37abe8f9;
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ucp = (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) name;
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ *scp = (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ *) name;
	ಪೂರ್ಣಾಂಕ c;

	ಆಗ (len--) {
		ಆದರೆ (unsigned_flag)
			c = (ಪೂರ್ಣಾಂಕ) *ucp++;
		ಇಲ್ಲದಿದ್ದರೆ
			c = (ಪೂರ್ಣಾಂಕ) *scp++;
		hash = hash1 + (hash0 ^ (c * 7152373));

		ಆದರೆ (hash & 0x80000000) hash -= 0x7fffffff;
		hash1 = hash0;
		hash0 = hash;
	}
	ಹಿಂತಿರುಗಿ (hash0 << 1);
}

ಸ್ಥಿರ ಶೂನ್ಯ str2hashbuf(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, ಪೂರ್ಣಾಂಕ len, __u32 *buf, ಪೂರ್ಣಾಂಕ ಅಂಕೆ,
			ಪೂರ್ಣಾಂಕ unsigned_flag)
{
	__u32	pad, val;
	ಪೂರ್ಣಾಂಕ	i, c;
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ucp = (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) ಸಂದೇಶ;
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ *scp = (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ *) ಸಂದೇಶ;

	pad = (__u32)len | ((__u32)len << 8);
	pad |= pad << 16;

	val = pad;
	ಆದರೆ (len > ಅಂಕೆ*4)
		len = ಅಂಕೆ * 4;
	ಗಾಗಿ (i=0; i < len; i++) {
		ಆದರೆ (unsigned_flag)
			c = (ಪೂರ್ಣಾಂಕ) ucp[i];
		ಇಲ್ಲದಿದ್ದರೆ
			c = (ಪೂರ್ಣಾಂಕ) scp[i];

		val = c + (val << 8);
		ಆದರೆ ((i % 4) == 3) {
			*buf++ = val;
			val = pad;
			ಅಂಕೆ--;
		}
	}
	ಆದರೆ (--ಅಂಕೆ >= 0)
		*buf++ = val;
	ಆಗ (--ಅಂಕೆ >= 0)
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
errcode_t ext2fs_dirhash(ಪೂರ್ಣಾಂಕ ಆವೃತ್ತಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ len,
			 ಸ್ಥಿರಾಂಕ __u32 *ಬೀಜ,
			 ext2_dirhash_t *ret_hash,
			 ext2_dirhash_t *ret_minor_hash)
{
	__u32	hash;
	__u32	minor_hash = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ	*p;
	ಪೂರ್ಣಾಂಕ		i;
	__u32 		ಒಳ[8], buf[4];
	ಪೂರ್ಣಾಂಕ		unsigned_flag = 0;

	/* Initialize the default seed for the hash checksum functions */
	buf[0] = 0x67452301;
	buf[1] = 0xefcdab89;
	buf[2] = 0x98badcfe;
	buf[3] = 0x10325476;

	/* Check to see if the seed is all zero's */
	ಆದರೆ (ಬೀಜ) {
		ಗಾಗಿ (i=0; i < 4; i++) {
			ಆದರೆ (ಬೀಜ[i])
				ನಿಲ್ಲಿಸು;
		}
		ಆದರೆ (i < 4)
			ಸ್ಮರಣೆ_ನಕಲು(buf, ಬೀಜ, ಗಾತ್ರ(buf));
	}

	ಆಯ್ಕೆ (ಆವೃತ್ತಿ) {
	ಸ್ಥಿತಿ EXT2_HASH_LEGACY_UNSIGNED:
		unsigned_flag++;
		/* fallthrough */
	ಸ್ಥಿತಿ EXT2_HASH_LEGACY:
		hash = dx_hack_hash(name, len, unsigned_flag);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ EXT2_HASH_HALF_MD4_UNSIGNED:
		unsigned_flag++;
		/* fallthrough */
	ಸ್ಥಿತಿ EXT2_HASH_HALF_MD4:
		p = name;
		ಆಗ (len > 0) {
			str2hashbuf(p, len, ಒಳ, 8, unsigned_flag);
			halfMD4Transform(buf, ಒಳ);
			len -= 32;
			p += 32;
		}
		minor_hash = buf[2];
		hash = buf[1];
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ EXT2_HASH_TEA_UNSIGNED:
		unsigned_flag++;
		/* fallthrough */
	ಸ್ಥಿತಿ EXT2_HASH_TEA:
		p = name;
		ಆಗ (len > 0) {
			str2hashbuf(p, len, ಒಳ, 4, unsigned_flag);
			TEA_transform(buf, ಒಳ);
			len -= 16;
			p += 16;
		}
		hash = buf[0];
		minor_hash = buf[1];
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		*ret_hash = 0;
		ಹಿಂತಿರುಗಿ EXT2_ET_DIRHASH_UNSUPP;
	}
	*ret_hash = hash & ~1;
	ಆದರೆ (ret_minor_hash)
		*ret_minor_hash = minor_hash;
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Returns the hash of a filename considering normalization and
 * casefolding.  This is a wrapper around ext2fs_dirhash with string
 * encoding support based on the nls_table and the flags. Check
 * ext2fs_dirhash for documentation on the input and output parameters.
 */
errcode_t ext2fs_dirhash2(ಪೂರ್ಣಾಂಕ ಆವೃತ್ತಿ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ len,
			  ಸ್ಥಿರಾಂಕ ರಚನೆ ext2fs_nls_ಕೋಷ್ಟಕ *charset,
			  ಪೂರ್ಣಾಂಕ hash_flags, ಸ್ಥಿರಾಂಕ __u32 *ಬೀಜ,
			  ext2_dirhash_t *ret_hash,
			  ext2_dirhash_t *ret_minor_hash)
{
	errcode_t r;
	ಪೂರ್ಣಾಂಕ dlen;

	ಆದರೆ (len && charset && (hash_flags & EXT4_CASEFOLD_FL)) {
		ಅಕ್ಷರ buff[PATH_MAX];

		dlen = charset->ops->casefold(charset,
			      (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) name, len,
			      (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) buff, ಗಾತ್ರ(buff));
		ಆದರೆ (dlen < 0) {
			ಆದರೆ (dlen == -EINVAL)
				ಹೋಗು opaque_seq;

			ಹಿಂತಿರುಗಿ dlen;
		}
		r = ext2fs_dirhash(ಆವೃತ್ತಿ, buff, dlen, ಬೀಜ, ret_hash,
				   ret_minor_hash);
		ಹಿಂತಿರುಗಿ r;
	}

opaque_seq:
	ಹಿಂತಿರುಗಿ ext2fs_dirhash(ಆವೃತ್ತಿ, name, len, ಬೀಜ, ret_hash,
			      ret_minor_hash);
}
