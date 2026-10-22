/*
 * This testing program makes sure the bitops functions work
 *
 * Copyright (C) 2001 by Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/types.h>
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#अन्तर्भाव <sys/time.h>
#अन्तर्भाव <sys/resource.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

अचिन्हित वर्ण bitarray[] = {
	0x80, 0xF0, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x10, 0x20, 0x00, 0x00
	};

पूर्णाङ्क bits_list[] = {
	7, 12, 13, 14,15, 22, 30, 68, 77, -1,
};

#define BIG_TEST_BIT   (((अचिन्हित) 1 << 31) + 42)


पूर्णाङ्क main(पूर्णाङ्क argc, वर्ण **argv)
{
	पूर्णाङ्क	i, j, size;
	अचिन्हित वर्ण testarray[12];
	अचिन्हित वर्ण *bigarray;

	size = परिमाण(bitarray)*8;
#यदि 0
	i = ext2fs_find_first_bit_set(bitarray, size);
	यावत् (i < size) {
		म_लिखो("Bit set: %d\n", i);
		i = ext2fs_find_next_bit_set(bitarray, size, i+1);
	}
#endif

	/* Test test_bit */
	प्रति (i=0,j=0; i < size; i++) {
		यदि (ext2fs_test_bit(i, bitarray)) {
			यदि (bits_list[j] == i) {
				j++;
			} अन्यथा {
				म_लिखो("Bit %d set, not expected\n", i);
				निकास(1);
			}
		} अन्यथा {
			यदि (bits_list[j] == i) {
				म_लिखो("Expected bit %d to be clear.\n", i);
				निकास(1);
			}
		}
	}
	म_लिखो("ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	स_रखो(testarray, 0, परिमाण(testarray));
	प्रति (i=0; bits_list[i] > 0; i++) {
		ext2fs_set_bit(bits_list[i], testarray);
	}
	यदि (स_भेद(testarray, bitarray, परिमाण(testarray)) == 0) {
		म_लिखो("ext2fs_set_bit test succeeded.\n");
	} अन्यथा {
		म_लिखो("ext2fs_set_bit test failed.\n");
		प्रति (i=0; i < परिमाण(testarray); i++) {
			म_लिखो("%02x ", testarray[i]);
		}
		म_लिखो("\n");
		निकास(1);
	}
	प्रति (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit(bits_list[i], testarray);
	}
	प्रति (i=0; i < परिमाण(testarray); i++) {
		यदि (testarray[i]) {
			म_लिखो("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			निकास(1);
		}
	}
	म_लिखो("ext2fs_clear_bit test succeed.\n");


	/* Do bigarray test */
	bigarray = दो_स्मृति(1 << 29);
	यदि (!bigarray) {
		ख_लिखो(मानक_त्रुटि, "Failed to allocate scratch memory!\n");
		निकास(0);
	}

        bigarray[BIG_TEST_BIT >> 3] = 0;

	ext2fs_set_bit(BIG_TEST_BIT, bigarray);
	म_लिखो("big bit number (%u) test: %d, expected %d\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3], (1 << (BIG_TEST_BIT & 7)));
	यदि (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		निकास(1);

	ext2fs_clear_bit(BIG_TEST_BIT, bigarray);

	म_लिखो("big bit number (%u) test: %d, expected 0\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	यदि (bigarray[BIG_TEST_BIT >> 3] != 0)
		निकास(1);

	म_लिखो("ext2fs_set_bit big_test successful\n");


	/* Now test ext2fs_fast_set_bit */
	स_रखो(testarray, 0, परिमाण(testarray));
	प्रति (i=0; bits_list[i] > 0; i++) {
		ext2fs_fast_set_bit(bits_list[i], testarray);
	}
	यदि (स_भेद(testarray, bitarray, परिमाण(testarray)) == 0) {
		म_लिखो("ext2fs_fast_set_bit test succeeded.\n");
	} अन्यथा {
		म_लिखो("ext2fs_fast_set_bit test failed.\n");
		प्रति (i=0; i < परिमाण(testarray); i++) {
			म_लिखो("%02x ", testarray[i]);
		}
		म_लिखो("\n");
		निकास(1);
	}
	प्रति (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit(bits_list[i], testarray);
	}
	प्रति (i=0; i < परिमाण(testarray); i++) {
		यदि (testarray[i]) {
			म_लिखो("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			निकास(1);
		}
	}
	म_लिखो("ext2fs_clear_bit test succeed.\n");


        bigarray[BIG_TEST_BIT >> 3] = 0;

	ext2fs_fast_set_bit(BIG_TEST_BIT, bigarray);
	म_लिखो("big bit number (%u) test: %d, expected %d\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3], (1 << (BIG_TEST_BIT & 7)));
	यदि (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		निकास(1);

	ext2fs_fast_clear_bit(BIG_TEST_BIT, bigarray);

	म_लिखो("big bit number (%u) test: %d, expected 0\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	यदि (bigarray[BIG_TEST_BIT >> 3] != 0)
		निकास(1);

	म_लिखो("ext2fs_fast_set_bit big_test successful\n");

	/* Repeat foregoing tests for 64-bit bitops */

	/* Test test_bit */
	प्रति (i=0,j=0; i < size; i++) {
		यदि (ext2fs_test_bit64(i, bitarray)) {
			यदि (bits_list[j] == i) {
				j++;
			} अन्यथा {
				म_लिखो("64-bit: Bit %d set, not expected\n",
				       i);
				निकास(1);
			}
		} अन्यथा {
			यदि (bits_list[j] == i) {
				म_लिखो("64-bit: "
				       "Expected bit %d to be clear.\n", i);
				निकास(1);
			}
		}
	}
	म_लिखो("64-bit: ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	स_रखो(testarray, 0, परिमाण(testarray));
	प्रति (i=0; bits_list[i] > 0; i++) {
		ext2fs_set_bit64(bits_list[i], testarray);
	}
	यदि (स_भेद(testarray, bitarray, परिमाण(testarray)) == 0) {
		म_लिखो("64-bit: ext2fs_set_bit test succeeded.\n");
	} अन्यथा {
		म_लिखो("64-bit: ext2fs_set_bit test failed.\n");
		प्रति (i=0; i < परिमाण(testarray); i++) {
			म_लिखो("%02x ", testarray[i]);
		}
		म_लिखो("\n");
		निकास(1);
	}
	प्रति (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit64(bits_list[i], testarray);
	}
	प्रति (i=0; i < परिमाण(testarray); i++) {
		यदि (testarray[i]) {
			म_लिखो("64-bit: ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			निकास(1);
		}
	}
	म_लिखो("64-bit: ext2fs_clear_bit test succeed.\n");

	/* Do bigarray test */
        bigarray[BIG_TEST_BIT >> 3] = 0;

	ext2fs_set_bit64(BIG_TEST_BIT, bigarray);
	म_लिखो("64-bit: big bit number (%u) test: %d, expected %d\n",
	       BIG_TEST_BIT, bigarray[BIG_TEST_BIT >> 3],
	       (1 << (BIG_TEST_BIT & 7)));
	यदि (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		निकास(1);

	ext2fs_clear_bit64(BIG_TEST_BIT, bigarray);

	म_लिखो("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	यदि (bigarray[BIG_TEST_BIT >> 3] != 0)
		निकास(1);

	म_लिखो("64-bit: ext2fs_set_bit big_test successful\n");

	/* Now test ext2fs_fast_set_bit */
	स_रखो(testarray, 0, परिमाण(testarray));
	प्रति (i=0; bits_list[i] > 0; i++) {
		ext2fs_fast_set_bit64(bits_list[i], testarray);
	}
	यदि (स_भेद(testarray, bitarray, परिमाण(testarray)) == 0) {
		म_लिखो("64-bit: ext2fs_fast_set_bit test succeeded.\n");
	} अन्यथा {
		म_लिखो("64-bit: ext2fs_fast_set_bit test failed.\n");
		प्रति (i=0; i < परिमाण(testarray); i++) {
			म_लिखो("%02x ", testarray[i]);
		}
		म_लिखो("\n");
		निकास(1);
	}
	प्रति (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit64(bits_list[i], testarray);
	}
	प्रति (i=0; i < परिमाण(testarray); i++) {
		यदि (testarray[i]) {
			म_लिखो("64-bit: ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			निकास(1);
		}
	}
	म_लिखो("64-bit: ext2fs_clear_bit test succeed.\n");

        bigarray[BIG_TEST_BIT >> 3] = 0;

	ext2fs_fast_set_bit64(BIG_TEST_BIT, bigarray);
	म_लिखो("64-bit: big bit number (%u) test: %d, expected %d\n",
	       BIG_TEST_BIT, bigarray[BIG_TEST_BIT >> 3],
	       (1 << (BIG_TEST_BIT & 7)));
	यदि (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		निकास(1);

	ext2fs_fast_clear_bit64(BIG_TEST_BIT, bigarray);

	म_लिखो("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_BIT, bigarray[BIG_TEST_BIT >> 3]);
	यदि (bigarray[BIG_TEST_BIT >> 3] != 0)
		निकास(1);

	म_लिखो("64-bit: ext2fs_fast_set_bit big_test successful\n");
	मुक्त(bigarray);
	निकास(0);
}
