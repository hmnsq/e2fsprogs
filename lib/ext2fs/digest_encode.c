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

#అన్తర్భావ "config.h"
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు
#అన్తర్భావ "ext2fs.h"

స్థిర స్థిరరాశి అక్షరం *lookup_పట్టిక =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+,";

/**
 * ext2fs_digest_encode() -
 *
 * Encodes the input digest using characters from the set [a-zA-Z0-9_+].
 * The encoded string is roughly 4/3 times the size of the input string.
 */
పూర్ణాంకం ext2fs_digest_encode(స్థిరరాశి అక్షరం *src, పూర్ణాంకం len, అక్షరం *dst)
{
	పూర్ణాంకం i = 0, బిట్లు = 0, ac = 0;
	అక్షరం *cp = dst;

	అయితే (i < len) {
		ac += (((గుర్తులేని అక్షరం) src[i]) << బిట్లు);
		బిట్లు += 8;
		చేయి {
			*cp++ = lookup_పట్టిక[ac & 0x3f];
			ac >>= 6;
			బిట్లు -= 6;
		} అయితే (బిట్లు >= 6);
		i++;
	}
	ఒకవేళ (బిట్లు)
		*cp++ = lookup_పట్టిక[ac & 0x3f];
	తిరిగి cp - dst;
}

పూర్ణాంకం ext2fs_digest_decode(స్థిరరాశి అక్షరం *src, పూర్ణాంకం len, అక్షరం *dst)
{
	పూర్ణాంకం i = 0, బిట్లు = 0, ac = 0;
	స్థిరరాశి అక్షరం *p;
	అక్షరం *cp = dst;

	అయితే (i < len) {
		p = strchr(lookup_పట్టిక, src[i]);
		ఒకవేళ (p == ఖాళీ || src[i] == 0)
			తిరిగి -1;
		ac += (p - lookup_పట్టిక) << బిట్లు;
		బిట్లు += 6;
		ఒకవేళ (బిట్లు >= 8) {
			*cp++ = ac & 0xff;
			ac >>= 8;
			బిట్లు -= 8;
		}
		i++;
	}
	ఒకవేళ (ac)
		తిరిగి -1;
	తిరిగి cp - dst;
}


#నిర్వచితమైతే UNITTEST
స్థిర స్థిరరాశి నిర్మాణం {
	గుర్తులేని అక్షరం d[32];
	గుర్తులేని పూర్ణాంకం len;
	స్థిరరాశి అక్షరం *ed;
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

పూర్ణాంకం ప్రధాన(పూర్ణాంకం argc, అక్షరం **argv)
{
	పూర్ణాంకం i, ret, len;
	పూర్ణాంకం errors = 0;
	అక్షరం tmp[1024], tmp2[1024];

	ఒకవేళ (argc == 3 && !strcmp(argv[1], "encode")) {
		memset(tmp, 0, పరిమాణం(tmp));
		ext2fs_digest_encode(argv[2], అక్షరశ్రేణి_పొడవు(argv[2]), tmp);
		puts(tmp);
		నిష్క్రమించు(0);
	}
	ఒకవేళ (argc == 3 && !strcmp(argv[1], "decode")) {
		memset(tmp, 0, పరిమాణం(tmp));
		ret = ext2fs_digest_decode(argv[2], అక్షరశ్రేణి_పొడవు(argv[2]), tmp);
		puts(tmp);
		fprintf(stderr, "returned %d\n", ret);
		నిష్క్రమించు(0);
	}
	కొరకు (i = 0; i < (పూర్ణాంకం)(పరిమాణం(tests) / పరిమాణం(tests[0])); i++) {
		memset(tmp, 0, పరిమాణం(tmp));
		ret = ext2fs_digest_encode((స్థిరరాశి అక్షరం *) tests[i].d,
					   tests[i].len, tmp);
		len = అక్షరశ్రేణి_పొడవు(tmp);
		రాయి("Test Digest %d (returned %d): ", i, ret);
		ఒకవేళ (ret != len) {
			రాయి("FAILED returned %d, string length was %d\n",
			       ret, len);
			errors++;
			కొనసాగు;
		} లేకపోతే ఒకవేళ (strcmp(tmp, tests[i].ed) != 0) {
			రాయి("FAILED: got %s, expected %s\n", tmp,
			       tests[i].ed);
			errors++;
			కొనసాగు;
		}
		ret = ext2fs_digest_decode(tmp, len, tmp2);
		ఒకవేళ (ret != tests[i].len) {
			రాయి("FAILED decode returned %d, expected %d\n",
			       ret, tests[i].len);
			errors++;
			కొనసాగు;
		}
		ఒకవేళ (memcmp(tmp2, tests[i].d, ret) != 0) {
			puts("FAILED: decode mismatched");
			errors++;
			కొనసాగు;
		}
		రాయి("OK\n");
	}
	కొరకు (i = 1; i < argc; i++) {
		memset(tmp, 0, పరిమాణం(tmp));
		ret = ext2fs_digest_encode(argv[i], అక్షరశ్రేణి_పొడవు(argv[i]), tmp);
		len = అక్షరశ్రేణి_పొడవు(tmp);
		రాయి("Digest of '%s' is '%s' (returned %d, length %d)\n",
		       argv[i], tmp, ret, len);
	}
	తిరిగి errors;
}

#అయితే_ముగింపు /* UNITTEST */
