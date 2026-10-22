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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <fcntl.h>
#समावेश <time.h>
#समावेश <sys/stat.h>
#समावेश <sys/types.h>
#अगर HAVE_ERRNO_H
#समावेश <errno.h>
#पूर्ण_अगर
#समावेश <sys/time.h>
#समावेश <sys/resource.h>

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

अचिन्हित अक्षर bitarray[] = {
	0x80, 0xF0, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x10, 0x20, 0x00, 0x00
	};

पूर्णांक bits_list[] = {
	7, 12, 13, 14,15, 22, 30, 68, 77, -1,
};

#घोषणा BIG_TEST_BIT   (((अचिन्हित) 1 << 31) + 42)


पूर्णांक मुख्य(पूर्णांक argc, अक्षर **argv)
{
	पूर्णांक	i, j, आकार;
	अचिन्हित अक्षर testarray[12];
	अचिन्हित अक्षर *bigarray;

	आकार = माप(bitarray)*8;
#अगर 0
	i = ext2fs_find_first_bit_set(bitarray, आकार);
	जबतक (i < आकार) {
		म_लिखो("Bit set: %d\n", i);
		i = ext2fs_find_अग्र_bit_set(bitarray, आकार, i+1);
	}
#पूर्ण_अगर

	/* Test test_bit */
	क्रम (i=0,j=0; i < आकार; i++) {
		अगर (ext2fs_test_bit(i, bitarray)) {
			अगर (bits_list[j] == i) {
				j++;
			} अन्यथा {
				म_लिखो("Bit %d set, not expected\n", i);
				निकास(1);
			}
		} अन्यथा {
			अगर (bits_list[j] == i) {
				म_लिखो("Expected bit %d to be clear.\n", i);
				निकास(1);
			}
		}
	}
	म_लिखो("ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	स_रखो(testarray, 0, माप(testarray));
	क्रम (i=0; bits_list[i] > 0; i++) {
		ext2fs_set_bit(bits_list[i], testarray);
	}
	अगर (स_भेद(testarray, bitarray, माप(testarray)) == 0) {
		म_लिखो("ext2fs_set_bit test succeeded.\n");
	} अन्यथा {
		म_लिखो("ext2fs_set_bit test failed.\n");
		क्रम (i=0; i < माप(testarray); i++) {
			म_लिखो("%02x ", testarray[i]);
		}
		म_लिखो("\n");
		निकास(1);
	}
	क्रम (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit(bits_list[i], testarray);
	}
	क्रम (i=0; i < माप(testarray); i++) {
		अगर (testarray[i]) {
			म_लिखो("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			निकास(1);
		}
	}
	म_लिखो("ext2fs_clear_bit test succeed.\n");


	/* Do bigarray test */
	bigarray = दो_स्मृति(1 << 29);
	अगर (!bigarray) {
		ख_लिखो(मानक_त्रुटि, "Failed to allocate scratch memory!\n");
		निकास(0);
	}

        bigarray[BIG_TEST_BIT >> 3] = 0;

	ext2fs_set_bit(BIG_TEST_BIT, bigarray);
	म_लिखो("big bit number (%u) test: %d, expected %d\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3], (1 << (BIG_TEST_BIT & 7)));
	अगर (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		निकास(1);

	ext2fs_clear_bit(BIG_TEST_BIT, bigarray);

	म_लिखो("big bit number (%u) test: %d, expected 0\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	अगर (bigarray[BIG_TEST_BIT >> 3] != 0)
		निकास(1);

	म_लिखो("ext2fs_set_bit big_test successful\n");


	/* Now test ext2fs_fast_set_bit */
	स_रखो(testarray, 0, माप(testarray));
	क्रम (i=0; bits_list[i] > 0; i++) {
		ext2fs_fast_set_bit(bits_list[i], testarray);
	}
	अगर (स_भेद(testarray, bitarray, माप(testarray)) == 0) {
		म_लिखो("ext2fs_fast_set_bit test succeeded.\n");
	} अन्यथा {
		म_लिखो("ext2fs_fast_set_bit test failed.\n");
		क्रम (i=0; i < माप(testarray); i++) {
			म_लिखो("%02x ", testarray[i]);
		}
		म_लिखो("\n");
		निकास(1);
	}
	क्रम (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit(bits_list[i], testarray);
	}
	क्रम (i=0; i < माप(testarray); i++) {
		अगर (testarray[i]) {
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
	अगर (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		निकास(1);

	ext2fs_fast_clear_bit(BIG_TEST_BIT, bigarray);

	म_लिखो("big bit number (%u) test: %d, expected 0\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	अगर (bigarray[BIG_TEST_BIT >> 3] != 0)
		निकास(1);

	म_लिखो("ext2fs_fast_set_bit big_test successful\n");

	/* Repeat foregoing tests for 64-bit bitops */

	/* Test test_bit */
	क्रम (i=0,j=0; i < आकार; i++) {
		अगर (ext2fs_test_bit64(i, bitarray)) {
			अगर (bits_list[j] == i) {
				j++;
			} अन्यथा {
				म_लिखो("64-bit: Bit %d set, not expected\n",
				       i);
				निकास(1);
			}
		} अन्यथा {
			अगर (bits_list[j] == i) {
				म_लिखो("64-bit: "
				       "Expected bit %d to be clear.\n", i);
				निकास(1);
			}
		}
	}
	म_लिखो("64-bit: ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	स_रखो(testarray, 0, माप(testarray));
	क्रम (i=0; bits_list[i] > 0; i++) {
		ext2fs_set_bit64(bits_list[i], testarray);
	}
	अगर (स_भेद(testarray, bitarray, माप(testarray)) == 0) {
		म_लिखो("64-bit: ext2fs_set_bit test succeeded.\n");
	} अन्यथा {
		म_लिखो("64-bit: ext2fs_set_bit test failed.\n");
		क्रम (i=0; i < माप(testarray); i++) {
			म_लिखो("%02x ", testarray[i]);
		}
		म_लिखो("\n");
		निकास(1);
	}
	क्रम (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit64(bits_list[i], testarray);
	}
	क्रम (i=0; i < माप(testarray); i++) {
		अगर (testarray[i]) {
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
	अगर (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		निकास(1);

	ext2fs_clear_bit64(BIG_TEST_BIT, bigarray);

	म_लिखो("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	अगर (bigarray[BIG_TEST_BIT >> 3] != 0)
		निकास(1);

	म_लिखो("64-bit: ext2fs_set_bit big_test successful\n");

	/* Now test ext2fs_fast_set_bit */
	स_रखो(testarray, 0, माप(testarray));
	क्रम (i=0; bits_list[i] > 0; i++) {
		ext2fs_fast_set_bit64(bits_list[i], testarray);
	}
	अगर (स_भेद(testarray, bitarray, माप(testarray)) == 0) {
		म_लिखो("64-bit: ext2fs_fast_set_bit test succeeded.\n");
	} अन्यथा {
		म_लिखो("64-bit: ext2fs_fast_set_bit test failed.\n");
		क्रम (i=0; i < माप(testarray); i++) {
			म_लिखो("%02x ", testarray[i]);
		}
		म_लिखो("\n");
		निकास(1);
	}
	क्रम (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit64(bits_list[i], testarray);
	}
	क्रम (i=0; i < माप(testarray); i++) {
		अगर (testarray[i]) {
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
	अगर (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		निकास(1);

	ext2fs_fast_clear_bit64(BIG_TEST_BIT, bigarray);

	म_लिखो("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_BIT, bigarray[BIG_TEST_BIT >> 3]);
	अगर (bigarray[BIG_TEST_BIT >> 3] != 0)
		निकास(1);

	म_लिखो("64-bit: ext2fs_fast_set_bit big_test successful\n");
	मुक्त(bigarray);
	निकास(0);
}
