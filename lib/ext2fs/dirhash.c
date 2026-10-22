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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <limits.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "ext2fsP.h"

#నిర్వచితం_కాకపోతే PATH_MAX
#నిర్వచించు PATH_MAX 4096
#అయితే_ముగింపు

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
#నిర్వచించు DELTA 0x9E3779B9

స్థిర శూన్య TEA_transform(__u32 buf[4], __u32 స్థిరరాశి లోపల[])
{
	__u32	కూడిక = 0;
	__u32	b0 = buf[0], b1 = buf[1];
	__u32	a = లోపల[0], b = లోపల[1], c = లోపల[2], d = లోపల[3];
	పూర్ణాంకం	n = 16;

	చేయి {
		కూడిక += DELTA;
		b0 += ((b1 << 4)+a) ^ (b1+కూడిక) ^ ((b1 >> 5)+b);
		b1 += ((b0 << 4)+c) ^ (b0+కూడిక) ^ ((b0 >> 5)+d);
	} అయితే(--n);

	buf[0] += b0;
	buf[1] += b1;
}

/* F, G and H are basic MD4 functions: selection, majority, parity */
#నిర్వచించు F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#నిర్వచించు G(x, y, z) (((x) & (y)) + (((x) ^ (y)) & (z)))
#నిర్వచించు H(x, y, z) ((x) ^ (y) ^ (z))

/*
 * The generic round function.  The application is so specific that
 * we don't bother protecting all the arguments with parens, as is generally
 * good macro practice, in favor of extra legibility.
 * Rotation is separate from addition to prevent recomputation
 */
#నిర్వచించు విడత(f, a, b, c, d, x, s)	\
	(a += f(b, c, d) + x, a = (a << s) | (a >> (32-s)))
#నిర్వచించు K1 0
#నిర్వచించు K2 013240474631UL
#నిర్వచించు K3 015666365641UL

/*
 * Basic cut-down MD4 transform.  Returns only 32 bits of result.
 */
స్థిర శూన్య halfMD4Transform (__u32 buf[4], __u32 స్థిరరాశి లోపల[])
{
	__u32	a = buf[0], b = buf[1], c = buf[2], d = buf[3];

	/* Round 1 */
	విడత(F, a, b, c, d, లోపల[0] + K1,  3);
	విడత(F, d, a, b, c, లోపల[1] + K1,  7);
	విడత(F, c, d, a, b, లోపల[2] + K1, 11);
	విడత(F, b, c, d, a, లోపల[3] + K1, 19);
	విడత(F, a, b, c, d, లోపల[4] + K1,  3);
	విడత(F, d, a, b, c, లోపల[5] + K1,  7);
	విడత(F, c, d, a, b, లోపల[6] + K1, 11);
	విడత(F, b, c, d, a, లోపల[7] + K1, 19);

	/* Round 2 */
	విడత(G, a, b, c, d, లోపల[1] + K2,  3);
	విడత(G, d, a, b, c, లోపల[3] + K2,  5);
	విడత(G, c, d, a, b, లోపల[5] + K2,  9);
	విడత(G, b, c, d, a, లోపల[7] + K2, 13);
	విడత(G, a, b, c, d, లోపల[0] + K2,  3);
	విడత(G, d, a, b, c, లోపల[2] + K2,  5);
	విడత(G, c, d, a, b, లోపల[4] + K2,  9);
	విడత(G, b, c, d, a, లోపల[6] + K2, 13);

	/* Round 3 */
	విడత(H, a, b, c, d, లోపల[3] + K3,  3);
	విడత(H, d, a, b, c, లోపల[7] + K3,  9);
	విడత(H, c, d, a, b, లోపల[2] + K3, 11);
	విడత(H, b, c, d, a, లోపల[6] + K3, 15);
	విడత(H, a, b, c, d, లోపల[1] + K3,  3);
	విడత(H, d, a, b, c, లోపల[5] + K3,  9);
	విడత(H, c, d, a, b, లోపల[0] + K3, 11);
	విడత(H, b, c, d, a, లోపల[4] + K3, 15);

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
}

#నిర్వచనం_రద్దు విడత
#నిర్వచనం_రద్దు F
#నిర్వచనం_రద్దు G
#నిర్వచనం_రద్దు H
#నిర్వచనం_రద్దు K1
#నిర్వచనం_రద్దు K2
#నిర్వచనం_రద్దు K3

/* The old legacy hash */
స్థిర ext2_dirhash_t dx_hack_hash (స్థిరరాశి అక్షరం *name, పూర్ణాంకం len,
				    పూర్ణాంకం unsigned_flag)
{
	__u32 hash, hash0 = 0x12a3fe2d, hash1 = 0x37abe8f9;
	స్థిరరాశి గుర్తులేని అక్షరం *ucp = (స్థిరరాశి గుర్తులేని అక్షరం *) name;
	స్థిరరాశి గుర్తుగల అక్షరం *scp = (స్థిరరాశి గుర్తుగల అక్షరం *) name;
	పూర్ణాంకం c;

	అయితే (len--) {
		ఒకవేళ (unsigned_flag)
			c = (పూర్ణాంకం) *ucp++;
		లేకపోతే
			c = (పూర్ణాంకం) *scp++;
		hash = hash1 + (hash0 ^ (c * 7152373));

		ఒకవేళ (hash & 0x80000000) hash -= 0x7fffffff;
		hash1 = hash0;
		hash0 = hash;
	}
	తిరిగి (hash0 << 1);
}

స్థిర శూన్య str2hashbuf(స్థిరరాశి అక్షరం *సందేశం, పూర్ణాంకం len, __u32 *buf, పూర్ణాంకం అంకె,
			పూర్ణాంకం unsigned_flag)
{
	__u32	pad, val;
	పూర్ణాంకం	i, c;
	స్థిరరాశి గుర్తులేని అక్షరం *ucp = (స్థిరరాశి గుర్తులేని అక్షరం *) సందేశం;
	స్థిరరాశి గుర్తుగల అక్షరం *scp = (స్థిరరాశి గుర్తుగల అక్షరం *) సందేశం;

	pad = (__u32)len | ((__u32)len << 8);
	pad |= pad << 16;

	val = pad;
	ఒకవేళ (len > అంకె*4)
		len = అంకె * 4;
	కొరకు (i=0; i < len; i++) {
		ఒకవేళ (unsigned_flag)
			c = (పూర్ణాంకం) ucp[i];
		లేకపోతే
			c = (పూర్ణాంకం) scp[i];

		val = c + (val << 8);
		ఒకవేళ ((i % 4) == 3) {
			*buf++ = val;
			val = pad;
			అంకె--;
		}
	}
	ఒకవేళ (--అంకె >= 0)
		*buf++ = val;
	అయితే (--అంకె >= 0)
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
errcode_t ext2fs_dirhash(పూర్ణాంకం సంస్కరణ, స్థిరరాశి అక్షరం *name, పూర్ణాంకం len,
			 స్థిరరాశి __u32 *విత్తనం,
			 ext2_dirhash_t *ret_hash,
			 ext2_dirhash_t *ret_minor_hash)
{
	__u32	hash;
	__u32	minor_hash = 0;
	స్థిరరాశి అక్షరం	*p;
	పూర్ణాంకం		i;
	__u32 		లోపల[8], buf[4];
	పూర్ణాంకం		unsigned_flag = 0;

	/* Initialize the default seed for the hash checksum functions */
	buf[0] = 0x67452301;
	buf[1] = 0xefcdab89;
	buf[2] = 0x98badcfe;
	buf[3] = 0x10325476;

	/* Check to see if the seed is all zero's */
	ఒకవేళ (విత్తనం) {
		కొరకు (i=0; i < 4; i++) {
			ఒకవేళ (విత్తనం[i])
				ఆపు;
		}
		ఒకవేళ (i < 4)
			జ్ఞాపకం_నకలు(buf, విత్తనం, పరిమాణం(buf));
	}

	ఎంపిక (సంస్కరణ) {
	స్థితి EXT2_HASH_LEGACY_UNSIGNED:
		unsigned_flag++;
		/* fallthrough */
	స్థితి EXT2_HASH_LEGACY:
		hash = dx_hack_hash(name, len, unsigned_flag);
		ఆపు;
	స్థితి EXT2_HASH_HALF_MD4_UNSIGNED:
		unsigned_flag++;
		/* fallthrough */
	స్థితి EXT2_HASH_HALF_MD4:
		p = name;
		అయితే (len > 0) {
			str2hashbuf(p, len, లోపల, 8, unsigned_flag);
			halfMD4Transform(buf, లోపల);
			len -= 32;
			p += 32;
		}
		minor_hash = buf[2];
		hash = buf[1];
		ఆపు;
	స్థితి EXT2_HASH_TEA_UNSIGNED:
		unsigned_flag++;
		/* fallthrough */
	స్థితి EXT2_HASH_TEA:
		p = name;
		అయితే (len > 0) {
			str2hashbuf(p, len, లోపల, 4, unsigned_flag);
			TEA_transform(buf, లోపల);
			len -= 16;
			p += 16;
		}
		hash = buf[0];
		minor_hash = buf[1];
		ఆపు;
	అప్రమేయ:
		*ret_hash = 0;
		తిరిగి EXT2_ET_DIRHASH_UNSUPP;
	}
	*ret_hash = hash & ~1;
	ఒకవేళ (ret_minor_hash)
		*ret_minor_hash = minor_hash;
	తిరిగి 0;
}

/*
 * Returns the hash of a filename considering normalization and
 * casefolding.  This is a wrapper around ext2fs_dirhash with string
 * encoding support based on the nls_table and the flags. Check
 * ext2fs_dirhash for documentation on the input and output parameters.
 */
errcode_t ext2fs_dirhash2(పూర్ణాంకం సంస్కరణ, స్థిరరాశి అక్షరం *name, పూర్ణాంకం len,
			  స్థిరరాశి నిర్మాణం ext2fs_nls_పట్టిక *charset,
			  పూర్ణాంకం hash_flags, స్థిరరాశి __u32 *విత్తనం,
			  ext2_dirhash_t *ret_hash,
			  ext2_dirhash_t *ret_minor_hash)
{
	errcode_t r;
	పూర్ణాంకం dlen;

	ఒకవేళ (len && charset && (hash_flags & EXT4_CASEFOLD_FL)) {
		అక్షరం buff[PATH_MAX];

		dlen = charset->ops->casefold(charset,
			      (స్థిరరాశి గుర్తులేని అక్షరం *) name, len,
			      (గుర్తులేని అక్షరం *) buff, పరిమాణం(buff));
		ఒకవేళ (dlen < 0) {
			ఒకవేళ (dlen == -EINVAL)
				వెళ్ళు opaque_seq;

			తిరిగి dlen;
		}
		r = ext2fs_dirhash(సంస్కరణ, buff, dlen, విత్తనం, ret_hash,
				   ret_minor_hash);
		తిరిగి r;
	}

opaque_seq:
	తిరిగి ext2fs_dirhash(సంస్కరణ, name, len, విత్తనం, ret_hash,
			      ret_minor_hash);
}
