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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#ضمن <limits.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

#که_نه_تعریف_شوی PATH_MAX
#تعریف_کړه PATH_MAX 4096
#د_که_پای

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
#تعریف_کړه DELTA 0x9E3779B9

ساكن فراغ TEA_transform(__u32 buf[4], __u32 ثابت دننه[])
{
	__u32	مجموعه = 0;
	__u32	b0 = buf[0], b1 = buf[1];
	__u32	a = دننه[0], b = دننه[1], c = دننه[2], d = دننه[3];
	صحيح	n = 16;

	افعل {
		مجموعه += DELTA;
		b0 += ((b1 << 4)+a) ^ (b1+مجموعه) ^ ((b1 >> 5)+b);
		b1 += ((b0 << 4)+c) ^ (b0+مجموعه) ^ ((b0 >> 5)+d);
	} طالما(--n);

	buf[0] += b0;
	buf[1] += b1;
}

/* F, G and H are basic MD4 functions: selection, majority, parity */
#تعریف_کړه F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#تعریف_کړه G(x, y, z) (((x) & (y)) + (((x) ^ (y)) & (z)))
#تعریف_کړه H(x, y, z) ((x) ^ (y) ^ (z))

/*
 * The generic round function.  The application is so specific that
 * we don't bother protecting all the arguments with parens, as is generally
 * good macro practice, in favor of extra legibility.
 * Rotation is separate from addition to prevent recomputation
 */
#تعریف_کړه پړاو(f, a, b, c, d, x, s)	\
	(a += f(b, c, d) + x, a = (a << s) | (a >> (32-s)))
#تعریف_کړه K1 0
#تعریف_کړه K2 013240474631UL
#تعریف_کړه K3 015666365641UL

/*
 * Basic cut-down MD4 transform.  Returns only 32 bits of result.
 */
ساكن فراغ halfMD4Transform (__u32 buf[4], __u32 ثابت دننه[])
{
	__u32	a = buf[0], b = buf[1], c = buf[2], d = buf[3];

	/* Round 1 */
	پړاو(F, a, b, c, d, دننه[0] + K1,  3);
	پړاو(F, d, a, b, c, دننه[1] + K1,  7);
	پړاو(F, c, d, a, b, دننه[2] + K1, 11);
	پړاو(F, b, c, d, a, دننه[3] + K1, 19);
	پړاو(F, a, b, c, d, دننه[4] + K1,  3);
	پړاو(F, d, a, b, c, دننه[5] + K1,  7);
	پړاو(F, c, d, a, b, دننه[6] + K1, 11);
	پړاو(F, b, c, d, a, دننه[7] + K1, 19);

	/* Round 2 */
	پړاو(G, a, b, c, d, دننه[1] + K2,  3);
	پړاو(G, d, a, b, c, دننه[3] + K2,  5);
	پړاو(G, c, d, a, b, دننه[5] + K2,  9);
	پړاو(G, b, c, d, a, دننه[7] + K2, 13);
	پړاو(G, a, b, c, d, دننه[0] + K2,  3);
	پړاو(G, d, a, b, c, دننه[2] + K2,  5);
	پړاو(G, c, d, a, b, دننه[4] + K2,  9);
	پړاو(G, b, c, d, a, دننه[6] + K2, 13);

	/* Round 3 */
	پړاو(H, a, b, c, d, دننه[3] + K3,  3);
	پړاو(H, d, a, b, c, دننه[7] + K3,  9);
	پړاو(H, c, d, a, b, دننه[2] + K3, 11);
	پړاو(H, b, c, d, a, دننه[6] + K3, 15);
	پړاو(H, a, b, c, d, دننه[1] + K3,  3);
	پړاو(H, d, a, b, c, دننه[5] + K3,  9);
	پړاو(H, c, d, a, b, دننه[0] + K3, 11);
	پړاو(H, b, c, d, a, دننه[4] + K3, 15);

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
}

#تعریف_لغوه پړاو
#تعریف_لغوه F
#تعریف_لغوه G
#تعریف_لغوه H
#تعریف_لغوه K1
#تعریف_لغوه K2
#تعریف_لغوه K3

/* The old legacy hash */
ساكن ext2_dirhash_t dx_hack_hash (ثابت حرف *name, صحيح len,
				    صحيح unsigned_flag)
{
	__u32 hash, hash0 = 0x12a3fe2d, hash1 = 0x37abe8f9;
	ثابت بینښه حرف *ucp = (ثابت بینښه حرف *) name;
	ثابت نښهدار حرف *scp = (ثابت نښهدار حرف *) name;
	صحيح c;

	طالما (len--) {
		إذا (unsigned_flag)
			c = (صحيح) *ucp++;
		وإلا
			c = (صحيح) *scp++;
		hash = hash1 + (hash0 ^ (c * 7152373));

		إذا (hash & 0x80000000) hash -= 0x7fffffff;
		hash1 = hash0;
		hash0 = hash;
	}
	أرجع (hash0 << 1);
}

ساكن فراغ str2hashbuf(ثابت حرف *پیغام, صحيح len, __u32 *buf, صحيح عدد,
			صحيح unsigned_flag)
{
	__u32	pad, val;
	صحيح	i, c;
	ثابت بینښه حرف *ucp = (ثابت بینښه حرف *) پیغام;
	ثابت نښهدار حرف *scp = (ثابت نښهدار حرف *) پیغام;

	pad = (__u32)len | ((__u32)len << 8);
	pad |= pad << 16;

	val = pad;
	إذا (len > عدد*4)
		len = عدد * 4;
	لكل (i=0; i < len; i++) {
		إذا (unsigned_flag)
			c = (صحيح) ucp[i];
		وإلا
			c = (صحيح) scp[i];

		val = c + (val << 8);
		إذا ((i % 4) == 3) {
			*buf++ = val;
			val = pad;
			عدد--;
		}
	}
	إذا (--عدد >= 0)
		*buf++ = val;
	طالما (--عدد >= 0)
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
errcode_t ext2fs_dirhash(صحيح نسخه, ثابت حرف *name, صحيح len,
			 ثابت __u32 *تخم,
			 ext2_dirhash_t *ret_hash,
			 ext2_dirhash_t *ret_minor_hash)
{
	__u32	hash;
	__u32	minor_hash = 0;
	ثابت حرف	*p;
	صحيح		i;
	__u32 		دننه[8], buf[4];
	صحيح		unsigned_flag = 0;

	/* Initialize the default seed for the hash checksum functions */
	buf[0] = 0x67452301;
	buf[1] = 0xefcdab89;
	buf[2] = 0x98badcfe;
	buf[3] = 0x10325476;

	/* Check to see if the seed is all zero's */
	إذا (تخم) {
		لكل (i=0; i < 4; i++) {
			إذا (تخم[i])
				اكسر;
		}
		إذا (i < 4)
			حافظه_نقل(buf, تخم, حجم(buf));
	}

	بدل (نسخه) {
	حالة EXT2_HASH_LEGACY_UNSIGNED:
		unsigned_flag++;
		/* fallthrough */
	حالة EXT2_HASH_LEGACY:
		hash = dx_hack_hash(name, len, unsigned_flag);
		اكسر;
	حالة EXT2_HASH_HALF_MD4_UNSIGNED:
		unsigned_flag++;
		/* fallthrough */
	حالة EXT2_HASH_HALF_MD4:
		p = name;
		طالما (len > 0) {
			str2hashbuf(p, len, دننه, 8, unsigned_flag);
			halfMD4Transform(buf, دننه);
			len -= 32;
			p += 32;
		}
		minor_hash = buf[2];
		hash = buf[1];
		اكسر;
	حالة EXT2_HASH_TEA_UNSIGNED:
		unsigned_flag++;
		/* fallthrough */
	حالة EXT2_HASH_TEA:
		p = name;
		طالما (len > 0) {
			str2hashbuf(p, len, دننه, 4, unsigned_flag);
			TEA_transform(buf, دننه);
			len -= 16;
			p += 16;
		}
		hash = buf[0];
		minor_hash = buf[1];
		اكسر;
	افتراضي:
		*ret_hash = 0;
		أرجع EXT2_ET_DIRHASH_UNSUPP;
	}
	*ret_hash = hash & ~1;
	إذا (ret_minor_hash)
		*ret_minor_hash = minor_hash;
	أرجع 0;
}

/*
 * Returns the hash of a filename considering normalization and
 * casefolding.  This is a wrapper around ext2fs_dirhash with string
 * encoding support based on the nls_table and the flags. Check
 * ext2fs_dirhash for documentation on the input and output parameters.
 */
errcode_t ext2fs_dirhash2(صحيح نسخه, ثابت حرف *name, صحيح len,
			  ثابت بنية ext2fs_nls_جدول *charset,
			  صحيح hash_flags, ثابت __u32 *تخم,
			  ext2_dirhash_t *ret_hash,
			  ext2_dirhash_t *ret_minor_hash)
{
	errcode_t r;
	صحيح dlen;

	إذا (len && charset && (hash_flags & EXT4_CASEFOLD_FL)) {
		حرف buff[PATH_MAX];

		dlen = charset->ops->casefold(charset,
			      (ثابت بینښه حرف *) name, len,
			      (بینښه حرف *) buff, حجم(buff));
		إذا (dlen < 0) {
			إذا (dlen == -EINVAL)
				اذهب opaque_seq;

			أرجع dlen;
		}
		r = ext2fs_dirhash(نسخه, buff, dlen, تخم, ret_hash,
				   ret_minor_hash);
		أرجع r;
	}

opaque_seq:
	أرجع ext2fs_dirhash(نسخه, name, len, تخم, ret_hash,
			      ret_minor_hash);
}
