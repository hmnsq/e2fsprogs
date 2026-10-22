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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ PATH_MAX
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PATH_MAX 4096
#ਜੇ_ਅੰਤ

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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DELTA 0x9E3779B9

ਸਥਿਰ ਖਾਲੀ TEA_transform(__u32 buf[4], __u32 ਅਚਲ ਵਿੱਚ[])
{
	__u32	ਜੋੜ = 0;
	__u32	b0 = buf[0], b1 = buf[1];
	__u32	a = ਵਿੱਚ[0], b = ਵਿੱਚ[1], c = ਵਿੱਚ[2], d = ਵਿੱਚ[3];
	ਪੂਰਨ	n = 16;

	ਕਰੋ {
		ਜੋੜ += DELTA;
		b0 += ((b1 << 4)+a) ^ (b1+ਜੋੜ) ^ ((b1 >> 5)+b);
		b1 += ((b0 << 4)+c) ^ (b0+ਜੋੜ) ^ ((b0 >> 5)+d);
	} ਜਦੋਂ(--n);

	buf[0] += b0;
	buf[1] += b1;
}

/* F, G and H are basic MD4 functions: selection, majority, parity */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ G(x, y, z) (((x) & (y)) + (((x) ^ (y)) & (z)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ H(x, y, z) ((x) ^ (y) ^ (z))

/*
 * The generic round function.  The application is so specific that
 * we don't bother protecting all the arguments with parens, as is generally
 * good macro practice, in favor of extra legibility.
 * Rotation is separate from addition to prevent recomputation
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗੇੜਾ(f, a, b, c, d, x, s)	\
	(a += f(b, c, d) + x, a = (a << s) | (a >> (32-s)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ K1 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ K2 013240474631UL
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ K3 015666365641UL

/*
 * Basic cut-down MD4 transform.  Returns only 32 bits of result.
 */
ਸਥਿਰ ਖਾਲੀ halfMD4Transform (__u32 buf[4], __u32 ਅਚਲ ਵਿੱਚ[])
{
	__u32	a = buf[0], b = buf[1], c = buf[2], d = buf[3];

	/* Round 1 */
	ਗੇੜਾ(F, a, b, c, d, ਵਿੱਚ[0] + K1,  3);
	ਗੇੜਾ(F, d, a, b, c, ਵਿੱਚ[1] + K1,  7);
	ਗੇੜਾ(F, c, d, a, b, ਵਿੱਚ[2] + K1, 11);
	ਗੇੜਾ(F, b, c, d, a, ਵਿੱਚ[3] + K1, 19);
	ਗੇੜਾ(F, a, b, c, d, ਵਿੱਚ[4] + K1,  3);
	ਗੇੜਾ(F, d, a, b, c, ਵਿੱਚ[5] + K1,  7);
	ਗੇੜਾ(F, c, d, a, b, ਵਿੱਚ[6] + K1, 11);
	ਗੇੜਾ(F, b, c, d, a, ਵਿੱਚ[7] + K1, 19);

	/* Round 2 */
	ਗੇੜਾ(G, a, b, c, d, ਵਿੱਚ[1] + K2,  3);
	ਗੇੜਾ(G, d, a, b, c, ਵਿੱਚ[3] + K2,  5);
	ਗੇੜਾ(G, c, d, a, b, ਵਿੱਚ[5] + K2,  9);
	ਗੇੜਾ(G, b, c, d, a, ਵਿੱਚ[7] + K2, 13);
	ਗੇੜਾ(G, a, b, c, d, ਵਿੱਚ[0] + K2,  3);
	ਗੇੜਾ(G, d, a, b, c, ਵਿੱਚ[2] + K2,  5);
	ਗੇੜਾ(G, c, d, a, b, ਵਿੱਚ[4] + K2,  9);
	ਗੇੜਾ(G, b, c, d, a, ਵਿੱਚ[6] + K2, 13);

	/* Round 3 */
	ਗੇੜਾ(H, a, b, c, d, ਵਿੱਚ[3] + K3,  3);
	ਗੇੜਾ(H, d, a, b, c, ਵਿੱਚ[7] + K3,  9);
	ਗੇੜਾ(H, c, d, a, b, ਵਿੱਚ[2] + K3, 11);
	ਗੇੜਾ(H, b, c, d, a, ਵਿੱਚ[6] + K3, 15);
	ਗੇੜਾ(H, a, b, c, d, ਵਿੱਚ[1] + K3,  3);
	ਗੇੜਾ(H, d, a, b, c, ਵਿੱਚ[5] + K3,  9);
	ਗੇੜਾ(H, c, d, a, b, ਵਿੱਚ[0] + K3, 11);
	ਗੇੜਾ(H, b, c, d, a, ਵਿੱਚ[4] + K3, 15);

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ਗੇੜਾ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ F
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ G
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ H
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ K1
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ K2
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ K3

/* The old legacy hash */
ਸਥਿਰ ext2_dirhash_t dx_hack_hash (ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ len,
				    ਪੂਰਨ unsigned_flag)
{
	__u32 hash, hash0 = 0x12a3fe2d, hash1 = 0x37abe8f9;
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ucp = (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) name;
	ਅਚਲ ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ *scp = (ਅਚਲ ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ *) name;
	ਪੂਰਨ c;

	ਜਦੋਂ (len--) {
		ਜੇ (unsigned_flag)
			c = (ਪੂਰਨ) *ucp++;
		ਨਹੀਂਤਾਂ
			c = (ਪੂਰਨ) *scp++;
		hash = hash1 + (hash0 ^ (c * 7152373));

		ਜੇ (hash & 0x80000000) hash -= 0x7fffffff;
		hash1 = hash0;
		hash0 = hash;
	}
	ਵਾਪਸ (hash0 << 1);
}

ਸਥਿਰ ਖਾਲੀ str2hashbuf(ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ਪੂਰਨ len, __u32 *buf, ਪੂਰਨ ਅੰਕ,
			ਪੂਰਨ unsigned_flag)
{
	__u32	pad, val;
	ਪੂਰਨ	i, c;
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ucp = (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) ਸੁਨੇਹਾ;
	ਅਚਲ ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ *scp = (ਅਚਲ ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ *) ਸੁਨੇਹਾ;

	pad = (__u32)len | ((__u32)len << 8);
	pad |= pad << 16;

	val = pad;
	ਜੇ (len > ਅੰਕ*4)
		len = ਅੰਕ * 4;
	ਲਈ (i=0; i < len; i++) {
		ਜੇ (unsigned_flag)
			c = (ਪੂਰਨ) ucp[i];
		ਨਹੀਂਤਾਂ
			c = (ਪੂਰਨ) scp[i];

		val = c + (val << 8);
		ਜੇ ((i % 4) == 3) {
			*buf++ = val;
			val = pad;
			ਅੰਕ--;
		}
	}
	ਜੇ (--ਅੰਕ >= 0)
		*buf++ = val;
	ਜਦੋਂ (--ਅੰਕ >= 0)
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
errcode_t ext2fs_dirhash(ਪੂਰਨ ਸੰਸਕਰਣ, ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ len,
			 ਅਚਲ __u32 *ਬੀਜ,
			 ext2_dirhash_t *ret_hash,
			 ext2_dirhash_t *ret_minor_hash)
{
	__u32	hash;
	__u32	minor_hash = 0;
	ਅਚਲ ਅੱਖਰ	*p;
	ਪੂਰਨ		i;
	__u32 		ਵਿੱਚ[8], buf[4];
	ਪੂਰਨ		unsigned_flag = 0;

	/* Initialize the default seed for the hash checksum functions */
	buf[0] = 0x67452301;
	buf[1] = 0xefcdab89;
	buf[2] = 0x98badcfe;
	buf[3] = 0x10325476;

	/* Check to see if the seed is all zero's */
	ਜੇ (ਬੀਜ) {
		ਲਈ (i=0; i < 4; i++) {
			ਜੇ (ਬੀਜ[i])
				ਰੋਕੋ;
		}
		ਜੇ (i < 4)
			ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf, ਬੀਜ, ਆਕਾਰ(buf));
	}

	ਚੋਣ (ਸੰਸਕਰਣ) {
	ਹਾਲਤ EXT2_HASH_LEGACY_UNSIGNED:
		unsigned_flag++;
		/* fallthrough */
	ਹਾਲਤ EXT2_HASH_LEGACY:
		hash = dx_hack_hash(name, len, unsigned_flag);
		ਰੋਕੋ;
	ਹਾਲਤ EXT2_HASH_HALF_MD4_UNSIGNED:
		unsigned_flag++;
		/* fallthrough */
	ਹਾਲਤ EXT2_HASH_HALF_MD4:
		p = name;
		ਜਦੋਂ (len > 0) {
			str2hashbuf(p, len, ਵਿੱਚ, 8, unsigned_flag);
			halfMD4Transform(buf, ਵਿੱਚ);
			len -= 32;
			p += 32;
		}
		minor_hash = buf[2];
		hash = buf[1];
		ਰੋਕੋ;
	ਹਾਲਤ EXT2_HASH_TEA_UNSIGNED:
		unsigned_flag++;
		/* fallthrough */
	ਹਾਲਤ EXT2_HASH_TEA:
		p = name;
		ਜਦੋਂ (len > 0) {
			str2hashbuf(p, len, ਵਿੱਚ, 4, unsigned_flag);
			TEA_transform(buf, ਵਿੱਚ);
			len -= 16;
			p += 16;
		}
		hash = buf[0];
		minor_hash = buf[1];
		ਰੋਕੋ;
	ਮੂਲ:
		*ret_hash = 0;
		ਵਾਪਸ EXT2_ET_DIRHASH_UNSUPP;
	}
	*ret_hash = hash & ~1;
	ਜੇ (ret_minor_hash)
		*ret_minor_hash = minor_hash;
	ਵਾਪਸ 0;
}

/*
 * Returns the hash of a filename considering normalization and
 * casefolding.  This is a wrapper around ext2fs_dirhash with string
 * encoding support based on the nls_table and the flags. Check
 * ext2fs_dirhash for documentation on the input and output parameters.
 */
errcode_t ext2fs_dirhash2(ਪੂਰਨ ਸੰਸਕਰਣ, ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ len,
			  ਅਚਲ ਬਣਤਰ ext2fs_nls_ਸਾਰਣੀ *charset,
			  ਪੂਰਨ hash_flags, ਅਚਲ __u32 *ਬੀਜ,
			  ext2_dirhash_t *ret_hash,
			  ext2_dirhash_t *ret_minor_hash)
{
	errcode_t r;
	ਪੂਰਨ dlen;

	ਜੇ (len && charset && (hash_flags & EXT4_CASEFOLD_FL)) {
		ਅੱਖਰ buff[PATH_MAX];

		dlen = charset->ops->casefold(charset,
			      (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) name, len,
			      (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) buff, ਆਕਾਰ(buff));
		ਜੇ (dlen < 0) {
			ਜੇ (dlen == -EINVAL)
				ਜਾਓ opaque_seq;

			ਵਾਪਸ dlen;
		}
		r = ext2fs_dirhash(ਸੰਸਕਰਣ, buff, dlen, ਬੀਜ, ret_hash,
				   ret_minor_hash);
		ਵਾਪਸ r;
	}

opaque_seq:
	ਵਾਪਸ ext2fs_dirhash(ਸੰਸਕਰਣ, name, len, ਬੀਜ, ret_hash,
			      ret_minor_hash);
}
