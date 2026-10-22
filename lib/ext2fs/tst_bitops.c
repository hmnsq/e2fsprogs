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
#پایان_اگر
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#پایان_اگر
#ضمن <sys/time.h>
#ضمن <sys/resource.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

بیعلامت حرف bitarray[] = {
	0x80, 0xF0, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x10, 0x20, 0x00, 0x00
	};

صحيح بیتها_list[] = {
	7, 12, 13, 14,15, 22, 30, 68, 77, -1,
};

#تعریف_کن BIG_TEST_بیت   (((بیعلامت) 1 << 31) + 42)


صحيح اصلی(صحيح argc, حرف **argv)
{
	صحيح	i, j, اندازه;
	بیعلامت حرف testarray[12];
	بیعلامت حرف *bigarray;

	اندازه = حجم(bitarray)*8;
#إذا 0
	i = ext2fs_پیدا_کن_اولی_بیت_مجموعه(bitarray, اندازه);
	طالما (i < اندازه) {
		چاپ_کن("Bit set: %d\n", i);
		i = ext2fs_پیدا_کن_بعدی_بیت_مجموعه(bitarray, اندازه, i+1);
	}
#پایان_اگر

	/* Test test_bit */
	لكل (i=0,j=0; i < اندازه; i++) {
		إذا (ext2fs_test_بیت(i, bitarray)) {
			إذا (بیتها_list[j] == i) {
				j++;
			} وإلا {
				چاپ_کن("Bit %d set, not expected\n", i);
				خروج(1);
			}
		} وإلا {
			إذا (بیتها_list[j] == i) {
				چاپ_کن("Expected bit %d to be clear.\n", i);
				خروج(1);
			}
		}
	}
	چاپ_کن("ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, حجم(testarray));
	لكل (i=0; بیتها_list[i] > 0; i++) {
		ext2fs_مجموعه_بیت(بیتها_list[i], testarray);
	}
	إذا (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		چاپ_کن("ext2fs_set_bit test succeeded.\n");
	} وإلا {
		چاپ_کن("ext2fs_set_bit test failed.\n");
		لكل (i=0; i < حجم(testarray); i++) {
			چاپ_کن("%02x ", testarray[i]);
		}
		چاپ_کن("\n");
		خروج(1);
	}
	لكل (i=0; بیتها_list[i] > 0; i++) {
		ext2fs_clear_بیت(بیتها_list[i], testarray);
	}
	لكل (i=0; i < حجم(testarray); i++) {
		إذا (testarray[i]) {
			چاپ_کن("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			خروج(1);
		}
	}
	چاپ_کن("ext2fs_clear_bit test succeed.\n");


	/* Do bigarray test */
	bigarray = حافظه_بده(1 << 29);
	إذا (!bigarray) {
		fprintf(stderr, "Failed to allocate scratch memory!\n");
		خروج(0);
	}

        bigarray[BIG_TEST_بیت >> 3] = 0;

	ext2fs_مجموعه_بیت(BIG_TEST_بیت, bigarray);
	چاپ_کن("big bit number (%u) test: %d, expected %d\n", BIG_TEST_بیت,
	       bigarray[BIG_TEST_بیت >> 3], (1 << (BIG_TEST_بیت & 7)));
	إذا (bigarray[BIG_TEST_بیت >> 3] != (1 << (BIG_TEST_بیت & 7)))
		خروج(1);

	ext2fs_clear_بیت(BIG_TEST_بیت, bigarray);

	چاپ_کن("big bit number (%u) test: %d, expected 0\n", BIG_TEST_بیت,
	       bigarray[BIG_TEST_بیت >> 3]);
	إذا (bigarray[BIG_TEST_بیت >> 3] != 0)
		خروج(1);

	چاپ_کن("ext2fs_set_bit big_test successful\n");


	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, حجم(testarray));
	لكل (i=0; بیتها_list[i] > 0; i++) {
		ext2fs_fast_مجموعه_بیت(بیتها_list[i], testarray);
	}
	إذا (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		چاپ_کن("ext2fs_fast_set_bit test succeeded.\n");
	} وإلا {
		چاپ_کن("ext2fs_fast_set_bit test failed.\n");
		لكل (i=0; i < حجم(testarray); i++) {
			چاپ_کن("%02x ", testarray[i]);
		}
		چاپ_کن("\n");
		خروج(1);
	}
	لكل (i=0; بیتها_list[i] > 0; i++) {
		ext2fs_clear_بیت(بیتها_list[i], testarray);
	}
	لكل (i=0; i < حجم(testarray); i++) {
		إذا (testarray[i]) {
			چاپ_کن("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			خروج(1);
		}
	}
	چاپ_کن("ext2fs_clear_bit test succeed.\n");


        bigarray[BIG_TEST_بیت >> 3] = 0;

	ext2fs_fast_مجموعه_بیت(BIG_TEST_بیت, bigarray);
	چاپ_کن("big bit number (%u) test: %d, expected %d\n", BIG_TEST_بیت,
	       bigarray[BIG_TEST_بیت >> 3], (1 << (BIG_TEST_بیت & 7)));
	إذا (bigarray[BIG_TEST_بیت >> 3] != (1 << (BIG_TEST_بیت & 7)))
		خروج(1);

	ext2fs_fast_clear_بیت(BIG_TEST_بیت, bigarray);

	چاپ_کن("big bit number (%u) test: %d, expected 0\n", BIG_TEST_بیت,
	       bigarray[BIG_TEST_بیت >> 3]);
	إذا (bigarray[BIG_TEST_بیت >> 3] != 0)
		خروج(1);

	چاپ_کن("ext2fs_fast_set_bit big_test successful\n");

	/* Repeat foregoing tests for 64-bit bitops */

	/* Test test_bit */
	لكل (i=0,j=0; i < اندازه; i++) {
		إذا (ext2fs_test_bit64(i, bitarray)) {
			إذا (بیتها_list[j] == i) {
				j++;
			} وإلا {
				چاپ_کن("64-bit: Bit %d set, not expected\n",
				       i);
				خروج(1);
			}
		} وإلا {
			إذا (بیتها_list[j] == i) {
				چاپ_کن("64-bit: "
				       "Expected bit %d to be clear.\n", i);
				خروج(1);
			}
		}
	}
	چاپ_کن("64-bit: ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, حجم(testarray));
	لكل (i=0; بیتها_list[i] > 0; i++) {
		ext2fs_مجموعه_bit64(بیتها_list[i], testarray);
	}
	إذا (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		چاپ_کن("64-bit: ext2fs_set_bit test succeeded.\n");
	} وإلا {
		چاپ_کن("64-bit: ext2fs_set_bit test failed.\n");
		لكل (i=0; i < حجم(testarray); i++) {
			چاپ_کن("%02x ", testarray[i]);
		}
		چاپ_کن("\n");
		خروج(1);
	}
	لكل (i=0; بیتها_list[i] > 0; i++) {
		ext2fs_clear_bit64(بیتها_list[i], testarray);
	}
	لكل (i=0; i < حجم(testarray); i++) {
		إذا (testarray[i]) {
			چاپ_کن("64-bit: ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			خروج(1);
		}
	}
	چاپ_کن("64-bit: ext2fs_clear_bit test succeed.\n");

	/* Do bigarray test */
        bigarray[BIG_TEST_بیت >> 3] = 0;

	ext2fs_مجموعه_bit64(BIG_TEST_بیت, bigarray);
	چاپ_کن("64-bit: big bit number (%u) test: %d, expected %d\n",
	       BIG_TEST_بیت, bigarray[BIG_TEST_بیت >> 3],
	       (1 << (BIG_TEST_بیت & 7)));
	إذا (bigarray[BIG_TEST_بیت >> 3] != (1 << (BIG_TEST_بیت & 7)))
		خروج(1);

	ext2fs_clear_bit64(BIG_TEST_بیت, bigarray);

	چاپ_کن("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_بیت,
	       bigarray[BIG_TEST_بیت >> 3]);
	إذا (bigarray[BIG_TEST_بیت >> 3] != 0)
		خروج(1);

	چاپ_کن("64-bit: ext2fs_set_bit big_test successful\n");

	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, حجم(testarray));
	لكل (i=0; بیتها_list[i] > 0; i++) {
		ext2fs_fast_مجموعه_bit64(بیتها_list[i], testarray);
	}
	إذا (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		چاپ_کن("64-bit: ext2fs_fast_set_bit test succeeded.\n");
	} وإلا {
		چاپ_کن("64-bit: ext2fs_fast_set_bit test failed.\n");
		لكل (i=0; i < حجم(testarray); i++) {
			چاپ_کن("%02x ", testarray[i]);
		}
		چاپ_کن("\n");
		خروج(1);
	}
	لكل (i=0; بیتها_list[i] > 0; i++) {
		ext2fs_clear_bit64(بیتها_list[i], testarray);
	}
	لكل (i=0; i < حجم(testarray); i++) {
		إذا (testarray[i]) {
			چاپ_کن("64-bit: ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			خروج(1);
		}
	}
	چاپ_کن("64-bit: ext2fs_clear_bit test succeed.\n");

        bigarray[BIG_TEST_بیت >> 3] = 0;

	ext2fs_fast_مجموعه_bit64(BIG_TEST_بیت, bigarray);
	چاپ_کن("64-bit: big bit number (%u) test: %d, expected %d\n",
	       BIG_TEST_بیت, bigarray[BIG_TEST_بیت >> 3],
	       (1 << (BIG_TEST_بیت & 7)));
	إذا (bigarray[BIG_TEST_بیت >> 3] != (1 << (BIG_TEST_بیت & 7)))
		خروج(1);

	ext2fs_fast_clear_bit64(BIG_TEST_بیت, bigarray);

	چاپ_کن("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_بیت, bigarray[BIG_TEST_بیت >> 3]);
	إذا (bigarray[BIG_TEST_بیت >> 3] != 0)
		خروج(1);

	چاپ_کن("64-bit: ext2fs_fast_set_bit big_test successful\n");
	حرر(bigarray);
	خروج(0);
}
