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

#अन्तर्भाव "config.h"
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः
#अन्तर्भाव "ext2fs.h"

स्थिर ध्रुव वर्ण *lookup_सारणी =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+,";

/**
 * ext2fs_digest_encode() -
 *
 * Encodes the input digest using characters from the set [a-zA-Z0-9_+].
 * The encoded string is roughly 4/3 times the size of the input string.
 */
पूर्णाङ्क ext2fs_digest_encode(ध्रुव वर्ण *src, पूर्णाङ्क len, वर्ण *dst)
{
	पूर्णाङ्क i = 0, द्वयङ्काः = 0, ac = 0;
	वर्ण *cp = dst;

	यावत् (i < len) {
		ac += (((अचिह्नित वर्ण) src[i]) << द्वयङ्काः);
		द्वयङ्काः += 8;
		कुरु {
			*cp++ = lookup_सारणी[ac & 0x3f];
			ac >>= 6;
			द्वयङ्काः -= 6;
		} यावत् (द्वयङ्काः >= 6);
		i++;
	}
	यदि (द्वयङ्काः)
		*cp++ = lookup_सारणी[ac & 0x3f];
	प्रत्यागम cp - dst;
}

पूर्णाङ्क ext2fs_digest_decode(ध्रुव वर्ण *src, पूर्णाङ्क len, वर्ण *dst)
{
	पूर्णाङ्क i = 0, द्वयङ्काः = 0, ac = 0;
	ध्रुव वर्ण *p;
	वर्ण *cp = dst;

	यावत् (i < len) {
		p = strchr(lookup_सारणी, src[i]);
		यदि (p == रिक्त || src[i] == 0)
			प्रत्यागम -1;
		ac += (p - lookup_सारणी) << द्वयङ्काः;
		द्वयङ्काः += 6;
		यदि (द्वयङ्काः >= 8) {
			*cp++ = ac & 0xff;
			ac >>= 8;
			द्वयङ्काः -= 8;
		}
		i++;
	}
	यदि (ac)
		प्रत्यागम -1;
	प्रत्यागम cp - dst;
}


#यदि_परिभाषितम् UNITTEST
स्थिर ध्रुव संरचना {
	अचिह्नित वर्ण d[32];
	अचिह्नित पूर्णाङ्क len;
	ध्रुव वर्ण *ed;
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

पूर्णाङ्क मुख्यम्(पूर्णाङ्क argc, वर्ण **argv)
{
	पूर्णाङ्क i, ret, len;
	पूर्णाङ्क errors = 0;
	वर्ण tmp[1024], tmp2[1024];

	यदि (argc == 3 && !वाक्यं_तुलय(argv[1], "encode")) {
		स्मृतिं_पूरय(tmp, 0, परिमाण(tmp));
		ext2fs_digest_encode(argv[2], वाक्यदैर्घ्यम्(argv[2]), tmp);
		पङ्क्तिं_लिख(tmp);
		निर्गच्छ(0);
	}
	यदि (argc == 3 && !वाक्यं_तुलय(argv[1], "decode")) {
		स्मृतिं_पूरय(tmp, 0, परिमाण(tmp));
		ret = ext2fs_digest_decode(argv[2], वाक्यदैर्घ्यम्(argv[2]), tmp);
		पङ्क्तिं_लिख(tmp);
		fprintf(stderr, "returned %d\n", ret);
		निर्गच्छ(0);
	}
	प्रति (i = 0; i < (पूर्णाङ्क)(परिमाण(tests) / परिमाण(tests[0])); i++) {
		स्मृतिं_पूरय(tmp, 0, परिमाण(tmp));
		ret = ext2fs_digest_encode((ध्रुव वर्ण *) tests[i].d,
					   tests[i].len, tmp);
		len = वाक्यदैर्घ्यम्(tmp);
		प्रारूप_लिख("Test Digest %d (returned %d): ", i, ret);
		यदि (ret != len) {
			प्रारूप_लिख("FAILED returned %d, string length was %d\n",
			       ret, len);
			errors++;
			अनुवर्त;
		} अन्यथा यदि (वाक्यं_तुलय(tmp, tests[i].ed) != 0) {
			प्रारूप_लिख("FAILED: got %s, expected %s\n", tmp,
			       tests[i].ed);
			errors++;
			अनुवर्त;
		}
		ret = ext2fs_digest_decode(tmp, len, tmp2);
		यदि (ret != tests[i].len) {
			प्रारूप_लिख("FAILED decode returned %d, expected %d\n",
			       ret, tests[i].len);
			errors++;
			अनुवर्त;
		}
		यदि (memcmp(tmp2, tests[i].d, ret) != 0) {
			पङ्क्तिं_लिख("FAILED: decode mismatched");
			errors++;
			अनुवर्त;
		}
		प्रारूप_लिख("OK\n");
	}
	प्रति (i = 1; i < argc; i++) {
		स्मृतिं_पूरय(tmp, 0, परिमाण(tmp));
		ret = ext2fs_digest_encode(argv[i], वाक्यदैर्घ्यम्(argv[i]), tmp);
		len = वाक्यदैर्घ्यम्(tmp);
		प्रारूप_लिख("Digest of '%s' is '%s' (returned %d, length %d)\n",
		       argv[i], tmp, ret, len);
	}
	प्रत्यागम errors;
}

#यदि_अन्तः /* UNITTEST */
