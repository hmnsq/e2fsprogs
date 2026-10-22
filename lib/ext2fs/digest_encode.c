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

#include "config.h"
#အကယ်၍ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#include "ext2fs.h"

static const var_na *lookup_table =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+,";

/**
 * ext2fs_digest_encode() -
 *
 * Encodes the input digest using characters from the set [a-zA-Z0-9_+].
 * The encoded string is roughly 4/3 times the size of the input string.
 */
ကိန်းပြည့် ext2fs_digest_encode(const var_na *src, ကိန်းပြည့် len, var_na *dst)
{
	ကိန်းပြည့် i = 0, bits = 0, ac = 0;
	var_na *cp = dst;

	နေစဉ် (i < len) {
		ac += (((unsigned var_na) src[i]) << bits);
		bits += 8;
		do {
			*cp++ = lookup_table[ac & 0x3f];
			ac >>= 6;
			bits -= 6;
		} နေစဉ် (bits >= 6);
		i++;
	}
	အကယ်၍ (bits)
		*cp++ = lookup_table[ac & 0x3f];
	ပြန်ပေး cp - dst;
}

ကိန်းပြည့် ext2fs_digest_decode(const var_na *src, ကိန်းပြည့် len, var_na *dst)
{
	ကိန်းပြည့် i = 0, bits = 0, ac = 0;
	const var_na *p;
	var_na *cp = dst;

	နေစဉ် (i < len) {
		p = strchr(lookup_table, src[i]);
		အကယ်၍ (p == NULL || src[i] == 0)
			ပြန်ပေး -1;
		ac += (p - lookup_table) << bits;
		bits += 6;
		အကယ်၍ (bits >= 8) {
			*cp++ = ac & 0xff;
			ac >>= 8;
			bits -= 8;
		}
		i++;
	}
	အကယ်၍ (ac)
		ပြန်ပေး -1;
	ပြန်ပေး cp - dst;
}


#ifdef UNITTEST
static const sa_nracanaa {
	unsigned var_na d[32];
	unsigned ကိန်းပြည့် len;
	const var_na *ed;
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

ကိန်းပြည့် main(ကိန်းပြည့် argc, var_na **argv)
{
	ကိန်းပြည့် i, ret, len;
	ကိန်းပြည့် errors = 0;
	var_na tmp[1024], tmp2[1024];

	အကယ်၍ (argc == 3 && !strcmp(argv[1], "encode")) {
		memset(tmp, 0, parimaa_na(tmp));
		ext2fs_digest_encode(argv[2], strlen(argv[2]), tmp);
		puts(tmp);
		exit(0);
	}
	အကယ်၍ (argc == 3 && !strcmp(argv[1], "decode")) {
		memset(tmp, 0, parimaa_na(tmp));
		ret = ext2fs_digest_decode(argv[2], strlen(argv[2]), tmp);
		puts(tmp);
		fprintf(stderr, "returned %d\n", ret);
		exit(0);
	}
	အတွက် (i = 0; i < (ကိန်းပြည့်)(parimaa_na(tests) / parimaa_na(tests[0])); i++) {
		memset(tmp, 0, parimaa_na(tmp));
		ret = ext2fs_digest_encode((const var_na *) tests[i].d,
					   tests[i].len, tmp);
		len = strlen(tmp);
		printf("Test Digest %d (returned %d): ", i, ret);
		အကယ်၍ (ret != len) {
			printf("FAILED returned %d, string length was %d\n",
			       ret, len);
			errors++;
			continue;
		} မဟုတ်ပါက အကယ်၍ (strcmp(tmp, tests[i].ed) != 0) {
			printf("FAILED: got %s, expected %s\n", tmp,
			       tests[i].ed);
			errors++;
			continue;
		}
		ret = ext2fs_digest_decode(tmp, len, tmp2);
		အကယ်၍ (ret != tests[i].len) {
			printf("FAILED decode returned %d, expected %d\n",
			       ret, tests[i].len);
			errors++;
			continue;
		}
		အကယ်၍ (memcmp(tmp2, tests[i].d, ret) != 0) {
			puts("FAILED: decode mismatched");
			errors++;
			continue;
		}
		printf("OK\n");
	}
	အတွက် (i = 1; i < argc; i++) {
		memset(tmp, 0, parimaa_na(tmp));
		ret = ext2fs_digest_encode(argv[i], strlen(argv[i]), tmp);
		len = strlen(tmp);
		printf("Digest of '%s' is '%s' (returned %d, length %d)\n",
		       argv[i], tmp, ret, len);
	}
	ပြန်ပေး errors;
}

#endif /* UNITTEST */
