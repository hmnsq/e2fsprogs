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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#د_که_پای
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#د_که_پای
#ضمن <sys/time.h>
#ضمن <sys/resource.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

بینښه حرف bitarray[] = {
	0x80, 0xF0, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x10, 0x20, 0x00, 0x00
	};

صحيح بټونه_list[] = {
	7, 12, 13, 14,15, 22, 30, 68, 77, -1,
};

#تعریف_کړه BIG_TEST_بټ   (((بینښه) 1 << 31) + 42)


صحيح اصلي(صحيح argc, حرف **argv)
{
	صحيح	i, j, اندازه;
	بینښه حرف testarray[12];
	بینښه حرف *bigarray;

	اندازه = حجم(bitarray)*8;
#إذا 0
	i = ext2fs_پیدا_کړه_لومړی_بټ_سېټ(bitarray, اندازه);
	طالما (i < اندازه) {
		چاپ_کړه("Bit set: %d\n", i);
		i = ext2fs_پیدا_کړه_بل_بټ_سېټ(bitarray, اندازه, i+1);
	}
#د_که_پای

	/* Test test_bit */
	لكل (i=0,j=0; i < اندازه; i++) {
		إذا (ext2fs_test_بټ(i, bitarray)) {
			إذا (بټونه_list[j] == i) {
				j++;
			} وإلا {
				چاپ_کړه("Bit %d set, not expected\n", i);
				وتل(1);
			}
		} وإلا {
			إذا (بټونه_list[j] == i) {
				چاپ_کړه("Expected bit %d to be clear.\n", i);
				وتل(1);
			}
		}
	}
	چاپ_کړه("ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, حجم(testarray));
	لكل (i=0; بټونه_list[i] > 0; i++) {
		ext2fs_سېټ_بټ(بټونه_list[i], testarray);
	}
	إذا (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		چاپ_کړه("ext2fs_set_bit test succeeded.\n");
	} وإلا {
		چاپ_کړه("ext2fs_set_bit test failed.\n");
		لكل (i=0; i < حجم(testarray); i++) {
			چاپ_کړه("%02x ", testarray[i]);
		}
		چاپ_کړه("\n");
		وتل(1);
	}
	لكل (i=0; بټونه_list[i] > 0; i++) {
		ext2fs_clear_بټ(بټونه_list[i], testarray);
	}
	لكل (i=0; i < حجم(testarray); i++) {
		إذا (testarray[i]) {
			چاپ_کړه("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			وتل(1);
		}
	}
	چاپ_کړه("ext2fs_clear_bit test succeed.\n");


	/* Do bigarray test */
	bigarray = حافظه_ورکړه(1 << 29);
	إذا (!bigarray) {
		fprintf(stderr, "Failed to allocate scratch memory!\n");
		وتل(0);
	}

        bigarray[BIG_TEST_بټ >> 3] = 0;

	ext2fs_سېټ_بټ(BIG_TEST_بټ, bigarray);
	چاپ_کړه("big bit number (%u) test: %d, expected %d\n", BIG_TEST_بټ,
	       bigarray[BIG_TEST_بټ >> 3], (1 << (BIG_TEST_بټ & 7)));
	إذا (bigarray[BIG_TEST_بټ >> 3] != (1 << (BIG_TEST_بټ & 7)))
		وتل(1);

	ext2fs_clear_بټ(BIG_TEST_بټ, bigarray);

	چاپ_کړه("big bit number (%u) test: %d, expected 0\n", BIG_TEST_بټ,
	       bigarray[BIG_TEST_بټ >> 3]);
	إذا (bigarray[BIG_TEST_بټ >> 3] != 0)
		وتل(1);

	چاپ_کړه("ext2fs_set_bit big_test successful\n");


	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, حجم(testarray));
	لكل (i=0; بټونه_list[i] > 0; i++) {
		ext2fs_fast_سېټ_بټ(بټونه_list[i], testarray);
	}
	إذا (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		چاپ_کړه("ext2fs_fast_set_bit test succeeded.\n");
	} وإلا {
		چاپ_کړه("ext2fs_fast_set_bit test failed.\n");
		لكل (i=0; i < حجم(testarray); i++) {
			چاپ_کړه("%02x ", testarray[i]);
		}
		چاپ_کړه("\n");
		وتل(1);
	}
	لكل (i=0; بټونه_list[i] > 0; i++) {
		ext2fs_clear_بټ(بټونه_list[i], testarray);
	}
	لكل (i=0; i < حجم(testarray); i++) {
		إذا (testarray[i]) {
			چاپ_کړه("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			وتل(1);
		}
	}
	چاپ_کړه("ext2fs_clear_bit test succeed.\n");


        bigarray[BIG_TEST_بټ >> 3] = 0;

	ext2fs_fast_سېټ_بټ(BIG_TEST_بټ, bigarray);
	چاپ_کړه("big bit number (%u) test: %d, expected %d\n", BIG_TEST_بټ,
	       bigarray[BIG_TEST_بټ >> 3], (1 << (BIG_TEST_بټ & 7)));
	إذا (bigarray[BIG_TEST_بټ >> 3] != (1 << (BIG_TEST_بټ & 7)))
		وتل(1);

	ext2fs_fast_clear_بټ(BIG_TEST_بټ, bigarray);

	چاپ_کړه("big bit number (%u) test: %d, expected 0\n", BIG_TEST_بټ,
	       bigarray[BIG_TEST_بټ >> 3]);
	إذا (bigarray[BIG_TEST_بټ >> 3] != 0)
		وتل(1);

	چاپ_کړه("ext2fs_fast_set_bit big_test successful\n");

	/* Repeat foregoing tests for 64-bit bitops */

	/* Test test_bit */
	لكل (i=0,j=0; i < اندازه; i++) {
		إذا (ext2fs_test_bit64(i, bitarray)) {
			إذا (بټونه_list[j] == i) {
				j++;
			} وإلا {
				چاپ_کړه("64-bit: Bit %d set, not expected\n",
				       i);
				وتل(1);
			}
		} وإلا {
			إذا (بټونه_list[j] == i) {
				چاپ_کړه("64-bit: "
				       "Expected bit %d to be clear.\n", i);
				وتل(1);
			}
		}
	}
	چاپ_کړه("64-bit: ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, حجم(testarray));
	لكل (i=0; بټونه_list[i] > 0; i++) {
		ext2fs_سېټ_bit64(بټونه_list[i], testarray);
	}
	إذا (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		چاپ_کړه("64-bit: ext2fs_set_bit test succeeded.\n");
	} وإلا {
		چاپ_کړه("64-bit: ext2fs_set_bit test failed.\n");
		لكل (i=0; i < حجم(testarray); i++) {
			چاپ_کړه("%02x ", testarray[i]);
		}
		چاپ_کړه("\n");
		وتل(1);
	}
	لكل (i=0; بټونه_list[i] > 0; i++) {
		ext2fs_clear_bit64(بټونه_list[i], testarray);
	}
	لكل (i=0; i < حجم(testarray); i++) {
		إذا (testarray[i]) {
			چاپ_کړه("64-bit: ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			وتل(1);
		}
	}
	چاپ_کړه("64-bit: ext2fs_clear_bit test succeed.\n");

	/* Do bigarray test */
        bigarray[BIG_TEST_بټ >> 3] = 0;

	ext2fs_سېټ_bit64(BIG_TEST_بټ, bigarray);
	چاپ_کړه("64-bit: big bit number (%u) test: %d, expected %d\n",
	       BIG_TEST_بټ, bigarray[BIG_TEST_بټ >> 3],
	       (1 << (BIG_TEST_بټ & 7)));
	إذا (bigarray[BIG_TEST_بټ >> 3] != (1 << (BIG_TEST_بټ & 7)))
		وتل(1);

	ext2fs_clear_bit64(BIG_TEST_بټ, bigarray);

	چاپ_کړه("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_بټ,
	       bigarray[BIG_TEST_بټ >> 3]);
	إذا (bigarray[BIG_TEST_بټ >> 3] != 0)
		وتل(1);

	چاپ_کړه("64-bit: ext2fs_set_bit big_test successful\n");

	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, حجم(testarray));
	لكل (i=0; بټونه_list[i] > 0; i++) {
		ext2fs_fast_سېټ_bit64(بټونه_list[i], testarray);
	}
	إذا (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		چاپ_کړه("64-bit: ext2fs_fast_set_bit test succeeded.\n");
	} وإلا {
		چاپ_کړه("64-bit: ext2fs_fast_set_bit test failed.\n");
		لكل (i=0; i < حجم(testarray); i++) {
			چاپ_کړه("%02x ", testarray[i]);
		}
		چاپ_کړه("\n");
		وتل(1);
	}
	لكل (i=0; بټونه_list[i] > 0; i++) {
		ext2fs_clear_bit64(بټونه_list[i], testarray);
	}
	لكل (i=0; i < حجم(testarray); i++) {
		إذا (testarray[i]) {
			چاپ_کړه("64-bit: ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			وتل(1);
		}
	}
	چاپ_کړه("64-bit: ext2fs_clear_bit test succeed.\n");

        bigarray[BIG_TEST_بټ >> 3] = 0;

	ext2fs_fast_سېټ_bit64(BIG_TEST_بټ, bigarray);
	چاپ_کړه("64-bit: big bit number (%u) test: %d, expected %d\n",
	       BIG_TEST_بټ, bigarray[BIG_TEST_بټ >> 3],
	       (1 << (BIG_TEST_بټ & 7)));
	إذا (bigarray[BIG_TEST_بټ >> 3] != (1 << (BIG_TEST_بټ & 7)))
		وتل(1);

	ext2fs_fast_clear_bit64(BIG_TEST_بټ, bigarray);

	چاپ_کړه("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_بټ, bigarray[BIG_TEST_بټ >> 3]);
	إذا (bigarray[BIG_TEST_بټ >> 3] != 0)
		وتل(1);

	چاپ_کړه("64-bit: ext2fs_fast_set_bit big_test successful\n");
	حرر(bigarray);
	وتل(0);
}
