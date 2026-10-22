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
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/types.h>
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#यदि_अन्तः
#अन्तर्भाव <sys/time.h>
#अन्तर्भाव <sys/resource.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

अचिह्नित वर्ण bitarray[] = {
	0x80, 0xF0, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x10, 0x20, 0x00, 0x00
	};

पूर्णाङ्क द्वयङ्काः_list[] = {
	7, 12, 13, 14,15, 22, 30, 68, 77, -1,
};

#परिभाषय BIG_TEST_BIT   (((अचिह्नित) 1 << 31) + 42)


पूर्णाङ्क मुख्यम्(पूर्णाङ्क argc, वर्ण **argv)
{
	पूर्णाङ्क	i, j, आकार;
	अचिह्नित वर्ण testarray[12];
	अचिह्नित वर्ण *bigarray;

	आकार = परिमाण(bitarray)*8;
#यदि 0
	i = ext2fs_find_first_bit_समुच्चयः(bitarray, आकार);
	यावत् (i < आकार) {
		प्रारूप_लिख("Bit set: %d\n", i);
		i = ext2fs_find_अग्र_bit_समुच्चयः(bitarray, आकार, i+1);
	}
#यदि_अन्तः

	/* Test test_bit */
	प्रति (i=0,j=0; i < आकार; i++) {
		यदि (ext2fs_test_bit(i, bitarray)) {
			यदि (द्वयङ्काः_list[j] == i) {
				j++;
			} अन्यथा {
				प्रारूप_लिख("Bit %d set, not expected\n", i);
				निर्गच्छ(1);
			}
		} अन्यथा {
			यदि (द्वयङ्काः_list[j] == i) {
				प्रारूप_लिख("Expected bit %d to be clear.\n", i);
				निर्गच्छ(1);
			}
		}
	}
	प्रारूप_लिख("ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	स्मृतिं_पूरय(testarray, 0, परिमाण(testarray));
	प्रति (i=0; द्वयङ्काः_list[i] > 0; i++) {
		ext2fs_समुच्चयः_bit(द्वयङ्काः_list[i], testarray);
	}
	यदि (memcmp(testarray, bitarray, परिमाण(testarray)) == 0) {
		प्रारूप_लिख("ext2fs_set_bit test succeeded.\n");
	} अन्यथा {
		प्रारूप_लिख("ext2fs_set_bit test failed.\n");
		प्रति (i=0; i < परिमाण(testarray); i++) {
			प्रारूप_लिख("%02x ", testarray[i]);
		}
		प्रारूप_लिख("\n");
		निर्गच्छ(1);
	}
	प्रति (i=0; द्वयङ्काः_list[i] > 0; i++) {
		ext2fs_clear_bit(द्वयङ्काः_list[i], testarray);
	}
	प्रति (i=0; i < परिमाण(testarray); i++) {
		यदि (testarray[i]) {
			प्रारूप_लिख("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			निर्गच्छ(1);
		}
	}
	प्रारूप_लिख("ext2fs_clear_bit test succeed.\n");


	/* Do bigarray test */
	bigarray = स्मृतिं_देहि(1 << 29);
	यदि (!bigarray) {
		fprintf(stderr, "Failed to allocate scratch memory!\n");
		निर्गच्छ(0);
	}

        bigarray[BIG_TEST_BIT >> 3] = 0;

	ext2fs_समुच्चयः_bit(BIG_TEST_BIT, bigarray);
	प्रारूप_लिख("big bit number (%u) test: %d, expected %d\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3], (1 << (BIG_TEST_BIT & 7)));
	यदि (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		निर्गच्छ(1);

	ext2fs_clear_bit(BIG_TEST_BIT, bigarray);

	प्रारूप_लिख("big bit number (%u) test: %d, expected 0\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	यदि (bigarray[BIG_TEST_BIT >> 3] != 0)
		निर्गच्छ(1);

	प्रारूप_लिख("ext2fs_set_bit big_test successful\n");


	/* Now test ext2fs_fast_set_bit */
	स्मृतिं_पूरय(testarray, 0, परिमाण(testarray));
	प्रति (i=0; द्वयङ्काः_list[i] > 0; i++) {
		ext2fs_fast_समुच्चयः_bit(द्वयङ्काः_list[i], testarray);
	}
	यदि (memcmp(testarray, bitarray, परिमाण(testarray)) == 0) {
		प्रारूप_लिख("ext2fs_fast_set_bit test succeeded.\n");
	} अन्यथा {
		प्रारूप_लिख("ext2fs_fast_set_bit test failed.\n");
		प्रति (i=0; i < परिमाण(testarray); i++) {
			प्रारूप_लिख("%02x ", testarray[i]);
		}
		प्रारूप_लिख("\n");
		निर्गच्छ(1);
	}
	प्रति (i=0; द्वयङ्काः_list[i] > 0; i++) {
		ext2fs_clear_bit(द्वयङ्काः_list[i], testarray);
	}
	प्रति (i=0; i < परिमाण(testarray); i++) {
		यदि (testarray[i]) {
			प्रारूप_लिख("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			निर्गच्छ(1);
		}
	}
	प्रारूप_लिख("ext2fs_clear_bit test succeed.\n");


        bigarray[BIG_TEST_BIT >> 3] = 0;

	ext2fs_fast_समुच्चयः_bit(BIG_TEST_BIT, bigarray);
	प्रारूप_लिख("big bit number (%u) test: %d, expected %d\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3], (1 << (BIG_TEST_BIT & 7)));
	यदि (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		निर्गच्छ(1);

	ext2fs_fast_clear_bit(BIG_TEST_BIT, bigarray);

	प्रारूप_लिख("big bit number (%u) test: %d, expected 0\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	यदि (bigarray[BIG_TEST_BIT >> 3] != 0)
		निर्गच्छ(1);

	प्रारूप_लिख("ext2fs_fast_set_bit big_test successful\n");

	/* Repeat foregoing tests for 64-bit bitops */

	/* Test test_bit */
	प्रति (i=0,j=0; i < आकार; i++) {
		यदि (ext2fs_test_bit64(i, bitarray)) {
			यदि (द्वयङ्काः_list[j] == i) {
				j++;
			} अन्यथा {
				प्रारूप_लिख("64-bit: Bit %d set, not expected\n",
				       i);
				निर्गच्छ(1);
			}
		} अन्यथा {
			यदि (द्वयङ्काः_list[j] == i) {
				प्रारूप_लिख("64-bit: "
				       "Expected bit %d to be clear.\n", i);
				निर्गच्छ(1);
			}
		}
	}
	प्रारूप_लिख("64-bit: ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	स्मृतिं_पूरय(testarray, 0, परिमाण(testarray));
	प्रति (i=0; द्वयङ्काः_list[i] > 0; i++) {
		ext2fs_समुच्चयः_bit64(द्वयङ्काः_list[i], testarray);
	}
	यदि (memcmp(testarray, bitarray, परिमाण(testarray)) == 0) {
		प्रारूप_लिख("64-bit: ext2fs_set_bit test succeeded.\n");
	} अन्यथा {
		प्रारूप_लिख("64-bit: ext2fs_set_bit test failed.\n");
		प्रति (i=0; i < परिमाण(testarray); i++) {
			प्रारूप_लिख("%02x ", testarray[i]);
		}
		प्रारूप_लिख("\n");
		निर्गच्छ(1);
	}
	प्रति (i=0; द्वयङ्काः_list[i] > 0; i++) {
		ext2fs_clear_bit64(द्वयङ्काः_list[i], testarray);
	}
	प्रति (i=0; i < परिमाण(testarray); i++) {
		यदि (testarray[i]) {
			प्रारूप_लिख("64-bit: ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			निर्गच्छ(1);
		}
	}
	प्रारूप_लिख("64-bit: ext2fs_clear_bit test succeed.\n");

	/* Do bigarray test */
        bigarray[BIG_TEST_BIT >> 3] = 0;

	ext2fs_समुच्चयः_bit64(BIG_TEST_BIT, bigarray);
	प्रारूप_लिख("64-bit: big bit number (%u) test: %d, expected %d\n",
	       BIG_TEST_BIT, bigarray[BIG_TEST_BIT >> 3],
	       (1 << (BIG_TEST_BIT & 7)));
	यदि (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		निर्गच्छ(1);

	ext2fs_clear_bit64(BIG_TEST_BIT, bigarray);

	प्रारूप_लिख("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	यदि (bigarray[BIG_TEST_BIT >> 3] != 0)
		निर्गच्छ(1);

	प्रारूप_लिख("64-bit: ext2fs_set_bit big_test successful\n");

	/* Now test ext2fs_fast_set_bit */
	स्मृतिं_पूरय(testarray, 0, परिमाण(testarray));
	प्रति (i=0; द्वयङ्काः_list[i] > 0; i++) {
		ext2fs_fast_समुच्चयः_bit64(द्वयङ्काः_list[i], testarray);
	}
	यदि (memcmp(testarray, bitarray, परिमाण(testarray)) == 0) {
		प्रारूप_लिख("64-bit: ext2fs_fast_set_bit test succeeded.\n");
	} अन्यथा {
		प्रारूप_लिख("64-bit: ext2fs_fast_set_bit test failed.\n");
		प्रति (i=0; i < परिमाण(testarray); i++) {
			प्रारूप_लिख("%02x ", testarray[i]);
		}
		प्रारूप_लिख("\n");
		निर्गच्छ(1);
	}
	प्रति (i=0; द्वयङ्काः_list[i] > 0; i++) {
		ext2fs_clear_bit64(द्वयङ्काः_list[i], testarray);
	}
	प्रति (i=0; i < परिमाण(testarray); i++) {
		यदि (testarray[i]) {
			प्रारूप_लिख("64-bit: ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			निर्गच्छ(1);
		}
	}
	प्रारूप_लिख("64-bit: ext2fs_clear_bit test succeed.\n");

        bigarray[BIG_TEST_BIT >> 3] = 0;

	ext2fs_fast_समुच्चयः_bit64(BIG_TEST_BIT, bigarray);
	प्रारूप_लिख("64-bit: big bit number (%u) test: %d, expected %d\n",
	       BIG_TEST_BIT, bigarray[BIG_TEST_BIT >> 3],
	       (1 << (BIG_TEST_BIT & 7)));
	यदि (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		निर्गच्छ(1);

	ext2fs_fast_clear_bit64(BIG_TEST_BIT, bigarray);

	प्रारूप_लिख("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_BIT, bigarray[BIG_TEST_BIT >> 3]);
	यदि (bigarray[BIG_TEST_BIT >> 3] != 0)
		निर्गच्छ(1);

	प्रारूप_लिख("64-bit: ext2fs_fast_set_bit big_test successful\n");
	मुक्त(bigarray);
	निर्गच्छ(0);
}
