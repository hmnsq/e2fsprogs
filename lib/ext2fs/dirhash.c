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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#شامل <limits.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

#اگر_غیر_تعریف_شدہ PATH_MAX
#تعریف_کرو PATH_MAX 4096
#اگر_ختم

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
#تعریف_کرو DELTA 0x9E3779B9

ساکن خالی TEA_transform(__u32 buf[4], __u32 ثابت اندر[])
{
	__u32	مجموع = 0;
	__u32	b0 = buf[0], b1 = buf[1];
	__u32	a = اندر[0], b = اندر[1], c = اندر[2], d = اندر[3];
	صحیح	n = 16;

	کرو {
		مجموع += DELTA;
		b0 += ((b1 << 4)+a) ^ (b1+مجموع) ^ ((b1 >> 5)+b);
		b1 += ((b0 << 4)+c) ^ (b0+مجموع) ^ ((b0 >> 5)+d);
	} جب(--n);

	buf[0] += b0;
	buf[1] += b1;
}

/* F, G and H are basic MD4 functions: selection, majority, parity */
#تعریف_کرو F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#تعریف_کرو G(x, y, z) (((x) & (y)) + (((x) ^ (y)) & (z)))
#تعریف_کرو H(x, y, z) ((x) ^ (y) ^ (z))

/*
 * The generic round function.  The application is so specific that
 * we don't bother protecting all the arguments with parens, as is generally
 * good macro practice, in favor of extra legibility.
 * Rotation is separate from addition to prevent recomputation
 */
#تعریف_کرو باری(f, a, b, c, d, x, s)	\
	(a += f(b, c, d) + x, a = (a << s) | (a >> (32-s)))
#تعریف_کرو K1 0
#تعریف_کرو K2 013240474631UL
#تعریف_کرو K3 015666365641UL

/*
 * Basic cut-down MD4 transform.  Returns only 32 bits of result.
 */
ساکن خالی halfMD4Transform (__u32 buf[4], __u32 ثابت اندر[])
{
	__u32	a = buf[0], b = buf[1], c = buf[2], d = buf[3];

	/* Round 1 */
	باری(F, a, b, c, d, اندر[0] + K1,  3);
	باری(F, d, a, b, c, اندر[1] + K1,  7);
	باری(F, c, d, a, b, اندر[2] + K1, 11);
	باری(F, b, c, d, a, اندر[3] + K1, 19);
	باری(F, a, b, c, d, اندر[4] + K1,  3);
	باری(F, d, a, b, c, اندر[5] + K1,  7);
	باری(F, c, d, a, b, اندر[6] + K1, 11);
	باری(F, b, c, d, a, اندر[7] + K1, 19);

	/* Round 2 */
	باری(G, a, b, c, d, اندر[1] + K2,  3);
	باری(G, d, a, b, c, اندر[3] + K2,  5);
	باری(G, c, d, a, b, اندر[5] + K2,  9);
	باری(G, b, c, d, a, اندر[7] + K2, 13);
	باری(G, a, b, c, d, اندر[0] + K2,  3);
	باری(G, d, a, b, c, اندر[2] + K2,  5);
	باری(G, c, d, a, b, اندر[4] + K2,  9);
	باری(G, b, c, d, a, اندر[6] + K2, 13);

	/* Round 3 */
	باری(H, a, b, c, d, اندر[3] + K3,  3);
	باری(H, d, a, b, c, اندر[7] + K3,  9);
	باری(H, c, d, a, b, اندر[2] + K3, 11);
	باری(H, b, c, d, a, اندر[6] + K3, 15);
	باری(H, a, b, c, d, اندر[1] + K3,  3);
	باری(H, d, a, b, c, اندر[5] + K3,  9);
	باری(H, c, d, a, b, اندر[0] + K3, 11);
	باری(H, b, c, d, a, اندر[4] + K3, 15);

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
}

#تعریف_منسوخ باری
#تعریف_منسوخ F
#تعریف_منسوخ G
#تعریف_منسوخ H
#تعریف_منسوخ K1
#تعریف_منسوخ K2
#تعریف_منسوخ K3

/* The old legacy hash */
ساکن ext2_dirhash_t dx_hack_hash (ثابت حرف *name, صحیح len,
				    صحیح unsigned_flag)
{
	__u32 hash, hash0 = 0x12a3fe2d, hash1 = 0x37abe8f9;
	ثابت غیرعلامتی حرف *ucp = (ثابت غیرعلامتی حرف *) name;
	ثابت علامتی حرف *scp = (ثابت علامتی حرف *) name;
	صحیح c;

	جب (len--) {
		اگر (unsigned_flag)
			c = (صحیح) *ucp++;
		ورنہ
			c = (صحیح) *scp++;
		hash = hash1 + (hash0 ^ (c * 7152373));

		اگر (hash & 0x80000000) hash -= 0x7fffffff;
		hash1 = hash0;
		hash0 = hash;
	}
	واپس (hash0 << 1);
}

ساکن خالی str2hashbuf(ثابت حرف *پیغام, صحیح len, __u32 *buf, صحیح عدد,
			صحیح unsigned_flag)
{
	__u32	pad, val;
	صحیح	i, c;
	ثابت غیرعلامتی حرف *ucp = (ثابت غیرعلامتی حرف *) پیغام;
	ثابت علامتی حرف *scp = (ثابت علامتی حرف *) پیغام;

	pad = (__u32)len | ((__u32)len << 8);
	pad |= pad << 16;

	val = pad;
	اگر (len > عدد*4)
		len = عدد * 4;
	برائے (i=0; i < len; i++) {
		اگر (unsigned_flag)
			c = (صحیح) ucp[i];
		ورنہ
			c = (صحیح) scp[i];

		val = c + (val << 8);
		اگر ((i % 4) == 3) {
			*buf++ = val;
			val = pad;
			عدد--;
		}
	}
	اگر (--عدد >= 0)
		*buf++ = val;
	جب (--عدد >= 0)
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
errcode_t ext2fs_dirhash(صحیح نسخہ, ثابت حرف *name, صحیح len,
			 ثابت __u32 *بیج,
			 ext2_dirhash_t *ret_hash,
			 ext2_dirhash_t *ret_minor_hash)
{
	__u32	hash;
	__u32	minor_hash = 0;
	ثابت حرف	*p;
	صحیح		i;
	__u32 		اندر[8], buf[4];
	صحیح		unsigned_flag = 0;

	/* Initialize the default seed for the hash checksum functions */
	buf[0] = 0x67452301;
	buf[1] = 0xefcdab89;
	buf[2] = 0x98badcfe;
	buf[3] = 0x10325476;

	/* Check to see if the seed is all zero's */
	اگر (بیج) {
		برائے (i=0; i < 4; i++) {
			اگر (بیج[i])
				توڑو;
		}
		اگر (i < 4)
			حافظہ_نقل(buf, بیج, حجم(buf));
	}

	انتخاب (نسخہ) {
	حالت EXT2_HASH_LEGACY_UNSIGNED:
		unsigned_flag++;
		/* fallthrough */
	حالت EXT2_HASH_LEGACY:
		hash = dx_hack_hash(name, len, unsigned_flag);
		توڑو;
	حالت EXT2_HASH_HALF_MD4_UNSIGNED:
		unsigned_flag++;
		/* fallthrough */
	حالت EXT2_HASH_HALF_MD4:
		p = name;
		جب (len > 0) {
			str2hashbuf(p, len, اندر, 8, unsigned_flag);
			halfMD4Transform(buf, اندر);
			len -= 32;
			p += 32;
		}
		minor_hash = buf[2];
		hash = buf[1];
		توڑو;
	حالت EXT2_HASH_TEA_UNSIGNED:
		unsigned_flag++;
		/* fallthrough */
	حالت EXT2_HASH_TEA:
		p = name;
		جب (len > 0) {
			str2hashbuf(p, len, اندر, 4, unsigned_flag);
			TEA_transform(buf, اندر);
			len -= 16;
			p += 16;
		}
		hash = buf[0];
		minor_hash = buf[1];
		توڑو;
	طےشدہ:
		*ret_hash = 0;
		واپس EXT2_ET_DIRHASH_UNSUPP;
	}
	*ret_hash = hash & ~1;
	اگر (ret_minor_hash)
		*ret_minor_hash = minor_hash;
	واپس 0;
}

/*
 * Returns the hash of a filename considering normalization and
 * casefolding.  This is a wrapper around ext2fs_dirhash with string
 * encoding support based on the nls_table and the flags. Check
 * ext2fs_dirhash for documentation on the input and output parameters.
 */
errcode_t ext2fs_dirhash2(صحیح نسخہ, ثابت حرف *name, صحیح len,
			  ثابت ساخت ext2fs_nls_جدول *charset,
			  صحیح hash_flags, ثابت __u32 *بیج,
			  ext2_dirhash_t *ret_hash,
			  ext2_dirhash_t *ret_minor_hash)
{
	errcode_t r;
	صحیح dlen;

	اگر (len && charset && (hash_flags & EXT4_CASEFOLD_FL)) {
		حرف buff[PATH_MAX];

		dlen = charset->ops->casefold(charset,
			      (ثابت غیرعلامتی حرف *) name, len,
			      (غیرعلامتی حرف *) buff, حجم(buff));
		اگر (dlen < 0) {
			اگر (dlen == -EINVAL)
				جاؤ opaque_seq;

			واپس dlen;
		}
		r = ext2fs_dirhash(نسخہ, buff, dlen, بیج, ret_hash,
				   ret_minor_hash);
		واپس r;
	}

opaque_seq:
	واپس ext2fs_dirhash(نسخہ, name, len, بیج, ret_hash,
			      ret_minor_hash);
}
