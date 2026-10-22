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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *lookup_ಕೋಷ್ಟಕ =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+,";

/**
 * ext2fs_digest_encode() -
 *
 * Encodes the input digest using characters from the set [a-zA-Z0-9_+].
 * The encoded string is roughly 4/3 times the size of the input string.
 */
ಪೂರ್ಣಾಂಕ ext2fs_digest_encode(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *src, ಪೂರ್ಣಾಂಕ len, ಅಕ್ಷರ *dst)
{
	ಪೂರ್ಣಾಂಕ i = 0, ಬಿಟ್ಟುಗಳು = 0, ac = 0;
	ಅಕ್ಷರ *cp = dst;

	ಆಗ (i < len) {
		ac += (((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) src[i]) << ಬಿಟ್ಟುಗಳು);
		ಬಿಟ್ಟುಗಳು += 8;
		ಮಾಡು {
			*cp++ = lookup_ಕೋಷ್ಟಕ[ac & 0x3f];
			ac >>= 6;
			ಬಿಟ್ಟುಗಳು -= 6;
		} ಆಗ (ಬಿಟ್ಟುಗಳು >= 6);
		i++;
	}
	ಆದರೆ (ಬಿಟ್ಟುಗಳು)
		*cp++ = lookup_ಕೋಷ್ಟಕ[ac & 0x3f];
	ಹಿಂತಿರುಗಿ cp - dst;
}

ಪೂರ್ಣಾಂಕ ext2fs_digest_decode(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *src, ಪೂರ್ಣಾಂಕ len, ಅಕ್ಷರ *dst)
{
	ಪೂರ್ಣಾಂಕ i = 0, ಬಿಟ್ಟುಗಳು = 0, ac = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *p;
	ಅಕ್ಷರ *cp = dst;

	ಆಗ (i < len) {
		p = strchr(lookup_ಕೋಷ್ಟಕ, src[i]);
		ಆದರೆ (p == ಖಾಲಿ || src[i] == 0)
			ಹಿಂತಿರುಗಿ -1;
		ac += (p - lookup_ಕೋಷ್ಟಕ) << ಬಿಟ್ಟುಗಳು;
		ಬಿಟ್ಟುಗಳು += 6;
		ಆದರೆ (ಬಿಟ್ಟುಗಳು >= 8) {
			*cp++ = ac & 0xff;
			ac >>= 8;
			ಬಿಟ್ಟುಗಳು -= 8;
		}
		i++;
	}
	ಆದರೆ (ac)
		ಹಿಂತಿರುಗಿ -1;
	ಹಿಂತಿರುಗಿ cp - dst;
}


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ UNITTEST
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ {
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ d[32];
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ len;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ed;
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

ಪೂರ್ಣಾಂಕ ಮುಖ್ಯ(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
	ಪೂರ್ಣಾಂಕ i, ret, len;
	ಪೂರ್ಣಾಂಕ errors = 0;
	ಅಕ್ಷರ tmp[1024], tmp2[1024];

	ಆದರೆ (argc == 3 && !strcmp(argv[1], "encode")) {
		memset(tmp, 0, ಗಾತ್ರ(tmp));
		ext2fs_digest_encode(argv[2], ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(argv[2]), tmp);
		puts(tmp);
		ನಿರ್ಗಮಿಸು(0);
	}
	ಆದರೆ (argc == 3 && !strcmp(argv[1], "decode")) {
		memset(tmp, 0, ಗಾತ್ರ(tmp));
		ret = ext2fs_digest_decode(argv[2], ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(argv[2]), tmp);
		puts(tmp);
		fprintf(stderr, "returned %d\n", ret);
		ನಿರ್ಗಮಿಸು(0);
	}
	ಗಾಗಿ (i = 0; i < (ಪೂರ್ಣಾಂಕ)(ಗಾತ್ರ(tests) / ಗಾತ್ರ(tests[0])); i++) {
		memset(tmp, 0, ಗಾತ್ರ(tmp));
		ret = ext2fs_digest_encode((ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *) tests[i].d,
					   tests[i].len, tmp);
		len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(tmp);
		ಬರೆ("Test Digest %d (returned %d): ", i, ret);
		ಆದರೆ (ret != len) {
			ಬರೆ("FAILED returned %d, string length was %d\n",
			       ret, len);
			errors++;
			ಮುಂದುವರಿ;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp(tmp, tests[i].ed) != 0) {
			ಬರೆ("FAILED: got %s, expected %s\n", tmp,
			       tests[i].ed);
			errors++;
			ಮುಂದುವರಿ;
		}
		ret = ext2fs_digest_decode(tmp, len, tmp2);
		ಆದರೆ (ret != tests[i].len) {
			ಬರೆ("FAILED decode returned %d, expected %d\n",
			       ret, tests[i].len);
			errors++;
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (memcmp(tmp2, tests[i].d, ret) != 0) {
			puts("FAILED: decode mismatched");
			errors++;
			ಮುಂದುವರಿ;
		}
		ಬರೆ("OK\n");
	}
	ಗಾಗಿ (i = 1; i < argc; i++) {
		memset(tmp, 0, ಗಾತ್ರ(tmp));
		ret = ext2fs_digest_encode(argv[i], ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(argv[i]), tmp);
		len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(tmp);
		ಬರೆ("Digest of '%s' is '%s' (returned %d, length %d)\n",
		       argv[i], tmp, ret, len);
	}
	ಹಿಂತಿರುಗಿ errors;
}

#ಆದರೆ_ಅಂತ್ಯ /* UNITTEST */
