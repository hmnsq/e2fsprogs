/*
 * lib/ext2fs/digest_encode.c
 *
 * A function to encode a digest using 64 characters that are valid in a
 * filename per ext2fs rules.
 *
 * Written by Uday Savagaonkar, 2014.
 *
 * Copyright 2014 Google Inc.  All Rights Reserved.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *lookup_ਸਾਰਣੀ =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+,";

/**
 * ext2fs_digest_encode() -
 *
 * Encodes the input digest using characters from the set [a-zA-Z0-9_+].
 * The encoded string is roughly 4/3 times the size of the input string.
 */
ਪੂਰਨ ext2fs_digest_encode(ਅਚਲ ਅੱਖਰ *src, ਪੂਰਨ len, ਅੱਖਰ *dst)
{
	ਪੂਰਨ i = 0, ਬਿੱਟ = 0, ac = 0;
	ਅੱਖਰ *cp = dst;

	ਜਦੋਂ (i < len) {
		ac += (((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) src[i]) << ਬਿੱਟ);
		ਬਿੱਟ += 8;
		ਕਰੋ {
			*cp++ = lookup_ਸਾਰਣੀ[ac & 0x3f];
			ac >>= 6;
			ਬਿੱਟ -= 6;
		} ਜਦੋਂ (ਬਿੱਟ >= 6);
		i++;
	}
	ਜੇ (ਬਿੱਟ)
		*cp++ = lookup_ਸਾਰਣੀ[ac & 0x3f];
	ਵਾਪਸ cp - dst;
}

ਪੂਰਨ ext2fs_digest_decode(ਅਚਲ ਅੱਖਰ *src, ਪੂਰਨ len, ਅੱਖਰ *dst)
{
	ਪੂਰਨ i = 0, ਬਿੱਟ = 0, ac = 0;
	ਅਚਲ ਅੱਖਰ *p;
	ਅੱਖਰ *cp = dst;

	ਜਦੋਂ (i < len) {
		p = strchr(lookup_ਸਾਰਣੀ, src[i]);
		ਜੇ (p == ਨਿਰਾ || src[i] == 0)
			ਵਾਪਸ -1;
		ac += (p - lookup_ਸਾਰਣੀ) << ਬਿੱਟ;
		ਬਿੱਟ += 6;
		ਜੇ (ਬਿੱਟ >= 8) {
			*cp++ = ac & 0xff;
			ac >>= 8;
			ਬਿੱਟ -= 8;
		}
		i++;
	}
	ਜੇ (ac)
		ਵਾਪਸ -1;
	ਵਾਪਸ cp - dst;
}


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ UNITTEST
ਸਥਿਰ ਅਚਲ ਬਣਤਰ {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ d[32];
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len;
	ਅਚਲ ਅੱਖਰ *ed;
} tests[] = {
	{ { 0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14,
	    0x9a, 0xfb, 0xf4, 0xc8, 0x99, 0x6f, 0xb9, 0x24,
	    0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c,
	    0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55 }, 32,
	"jDLxChJ,cQhm7TPyZ+WukcirBROZbOJTkWZmbgnU4WF"
	},
	{ { 0xba, 0x78, 0x16, 0xbf, 0x8f, 0x01, 0xcf, 0xea,
	    0x41, 0x41, 0x40, 0xde, 0x5d, 0xae, 0x22, 0x23,
	    0xb0, 0x03, 0x61, 0xa3, 0x96, 0x17, 0x7a, 0x9c,
	    0xb4, 0x10, 0xff, 0x61, 0xf2, 0x00, 0x15, 0xad }, 32,
	"6inF,+YAPreQBBk3d5qIjA7AhNqlXoHn0Cx,hJPAV0K"
	},
	{ { 0x24, 0x8d, 0x6a, 0x61, 0xd2, 0x06, 0x38, 0xb8,
	    0xe5, 0xc0, 0x26, 0x93, 0x0c, 0x3e, 0x60, 0x39,
	    0xa3, 0x3c, 0xe4, 0x59, 0x64, 0xff, 0x21, 0x67,
	    0xf6, 0xec, 0xed, 0xd4, 0x19, 0xdb, 0x06, 0xc1 }, 32,
	"k0oahJtB4gb5AbykM4DY5MKPknFZ,HyZ2ze7Unx2GEM"
	},
	{ { 0x00, }, 1,
	"AA"
	},
	{ { 0x01, }, 1,
	"BA"
	},
	{ { 0x01, 0x02 }, 2,
	"BIA"
	},
	{ { 0x01, 0x02, 0x03 }, 3,
	"BIwA"
	},
	{ { 0x01, 0x02, 0x03, 0x04 }, 4,
	"BIwAEA"
	},
	{ { 0x01, 0x02, 0x03, 0x04, 0xff }, 5,
	"BIwAE8P"
	},
	{ { 0x01, 0x02, 0x03, 0x04, 0xff, 0xfe }, 6,
	"BIwAE8v,"
	},
	{ { 0x01, 0x02, 0x03, 0x04, 0xff, 0xfe, 0xfd }, 7,
	"BIwAE8v,9D"
	},
};

ਪੂਰਨ ਮੁੱਖ(ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
	ਪੂਰਨ i, ret, len;
	ਪੂਰਨ errors = 0;
	ਅੱਖਰ tmp[1024], tmp2[1024];

	ਜੇ (argc == 3 && !strcmp(argv[1], "encode")) {
		memset(tmp, 0, ਆਕਾਰ(tmp));
		ext2fs_digest_encode(argv[2], ਲੜੀ_ਲੰਬਾਈ(argv[2]), tmp);
		puts(tmp);
		ਬਾਹਰ_ਨਿਕਲੋ(0);
	}
	ਜੇ (argc == 3 && !strcmp(argv[1], "decode")) {
		memset(tmp, 0, ਆਕਾਰ(tmp));
		ret = ext2fs_digest_decode(argv[2], ਲੜੀ_ਲੰਬਾਈ(argv[2]), tmp);
		puts(tmp);
		fprintf(stderr, "returned %d\n", ret);
		ਬਾਹਰ_ਨਿਕਲੋ(0);
	}
	ਲਈ (i = 0; i < (ਪੂਰਨ)(ਆਕਾਰ(tests) / ਆਕਾਰ(tests[0])); i++) {
		memset(tmp, 0, ਆਕਾਰ(tmp));
		ret = ext2fs_digest_encode((ਅਚਲ ਅੱਖਰ *) tests[i].d,
					   tests[i].len, tmp);
		len = ਲੜੀ_ਲੰਬਾਈ(tmp);
		ਲਿਖੋ("Test Digest %d (returned %d): ", i, ret);
		ਜੇ (ret != len) {
			ਲਿਖੋ("FAILED returned %d, string length was %d\n",
			       ret, len);
			errors++;
			ਜਾਰੀ;
		} ਨਹੀਂਤਾਂ ਜੇ (strcmp(tmp, tests[i].ed) != 0) {
			ਲਿਖੋ("FAILED: got %s, expected %s\n", tmp,
			       tests[i].ed);
			errors++;
			ਜਾਰੀ;
		}
		ret = ext2fs_digest_decode(tmp, len, tmp2);
		ਜੇ (ret != tests[i].len) {
			ਲਿਖੋ("FAILED decode returned %d, expected %d\n",
			       ret, tests[i].len);
			errors++;
			ਜਾਰੀ;
		}
		ਜੇ (memcmp(tmp2, tests[i].d, ret) != 0) {
			puts("FAILED: decode mismatched");
			errors++;
			ਜਾਰੀ;
		}
		ਲਿਖੋ("OK\n");
	}
	ਲਈ (i = 1; i < argc; i++) {
		memset(tmp, 0, ਆਕਾਰ(tmp));
		ret = ext2fs_digest_encode(argv[i], ਲੜੀ_ਲੰਬਾਈ(argv[i]), tmp);
		len = ਲੜੀ_ਲੰਬਾਈ(tmp);
		ਲਿਖੋ("Digest of '%s' is '%s' (returned %d, length %d)\n",
		       argv[i], tmp, ret, len);
	}
	ਵਾਪਸ errors;
}

#ਜੇ_ਅੰਤ /* UNITTEST */
