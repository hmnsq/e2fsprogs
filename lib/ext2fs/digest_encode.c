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
#जर HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#अन्तर्भाव "ext2fs.h"

स्थिर स्थिरांक अक्षर *lookup_table =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+,";

/**
 * ext2fs_digest_encode() -
 *
 * Encodes the input digest using characters from the set [a-zA-Z0-9_+].
 * The encoded string is roughly 4/3 times the size of the input string.
 */
पूर्णांक ext2fs_digest_encode(स्थिरांक अक्षर *src, पूर्णांक len, अक्षर *dst)
{
	पूर्णांक i = 0, bits = 0, ac = 0;
	अक्षर *cp = dst;

	पर्यंत (i < len) {
		ac += (((अचिन्हित अक्षर) src[i]) << bits);
		bits += 8;
		करा {
			*cp++ = lookup_table[ac & 0x3f];
			ac >>= 6;
			bits -= 6;
		} पर्यंत (bits >= 6);
		i++;
	}
	जर (bits)
		*cp++ = lookup_table[ac & 0x3f];
	परत cp - dst;
}

पूर्णांक ext2fs_digest_decode(स्थिरांक अक्षर *src, पूर्णांक len, अक्षर *dst)
{
	पूर्णांक i = 0, bits = 0, ac = 0;
	स्थिरांक अक्षर *p;
	अक्षर *cp = dst;

	पर्यंत (i < len) {
		p = म_अक्षर(lookup_table, src[i]);
		जर (p == रिक्त || src[i] == 0)
			परत -1;
		ac += (p - lookup_table) << bits;
		bits += 6;
		जर (bits >= 8) {
			*cp++ = ac & 0xff;
			ac >>= 8;
			bits -= 8;
		}
		i++;
	}
	जर (ac)
		परत -1;
	परत cp - dst;
}


#ifdef UNITTEST
स्थिर स्थिरांक संरचना {
	अचिन्हित अक्षर d[32];
	अचिन्हित पूर्णांक len;
	स्थिरांक अक्षर *ed;
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

पूर्णांक main(पूर्णांक argc, अक्षर **argv)
{
	पूर्णांक i, ret, len;
	पूर्णांक errors = 0;
	अक्षर tmp[1024], tmp2[1024];

	जर (argc == 3 && !म_भेद(argv[1], "encode")) {
		स_रखो(tmp, 0, आकार(tmp));
		ext2fs_digest_encode(argv[2], म_माप(argv[2]), tmp);
		माला_दो(tmp);
		निकास(0);
	}
	जर (argc == 3 && !म_भेद(argv[1], "decode")) {
		स_रखो(tmp, 0, आकार(tmp));
		ret = ext2fs_digest_decode(argv[2], म_माप(argv[2]), tmp);
		माला_दो(tmp);
		ख_लिखो(मानक_त्रुटि, "returned %d\n", ret);
		निकास(0);
	}
	साठी (i = 0; i < (पूर्णांक)(आकार(tests) / आकार(tests[0])); i++) {
		स_रखो(tmp, 0, आकार(tmp));
		ret = ext2fs_digest_encode((स्थिरांक अक्षर *) tests[i].d,
					   tests[i].len, tmp);
		len = म_माप(tmp);
		म_लिखो("Test Digest %d (returned %d): ", i, ret);
		जर (ret != len) {
			म_लिखो("FAILED returned %d, string length was %d\n",
			       ret, len);
			errors++;
			सुरूठेवा;
		} अन्यथा जर (म_भेद(tmp, tests[i].ed) != 0) {
			म_लिखो("FAILED: got %s, expected %s\n", tmp,
			       tests[i].ed);
			errors++;
			सुरूठेवा;
		}
		ret = ext2fs_digest_decode(tmp, len, tmp2);
		जर (ret != tests[i].len) {
			म_लिखो("FAILED decode returned %d, expected %d\n",
			       ret, tests[i].len);
			errors++;
			सुरूठेवा;
		}
		जर (स_भेद(tmp2, tests[i].d, ret) != 0) {
			माला_दो("FAILED: decode mismatched");
			errors++;
			सुरूठेवा;
		}
		म_लिखो("OK\n");
	}
	साठी (i = 1; i < argc; i++) {
		स_रखो(tmp, 0, आकार(tmp));
		ret = ext2fs_digest_encode(argv[i], म_माप(argv[i]), tmp);
		len = म_माप(tmp);
		म_लिखो("Digest of '%s' is '%s' (returned %d, length %d)\n",
		       argv[i], tmp, ret, len);
	}
	परत errors;
}

#endif /* UNITTEST */
