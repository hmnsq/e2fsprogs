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
#جے_انت
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جے_انت
#ضمن <sys/time.h>
#ضمن <sys/resource.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

بنانشانی حرف bitarray[] = {
	0x80, 0xF0, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x10, 0x20, 0x00, 0x00
	};

صحيح بٹ_list[] = {
	7, 12, 13, 14,15, 22, 30, 68, 77, -1,
};

#تعریف_کرو BIG_TEST_ثنائی_ہندسہ   (((بنانشانی) 1 << 31) + 42)


صحيح مکھ(صحيح argc, حرف **argv)
{
	صحيح	i, j, ناپ;
	بنانشانی حرف testarray[12];
	بنانشانی حرف *bigarray;

	ناپ = حجم(bitarray)*8;
#إذا 0
	i = ext2fs_لبھو_پہلا_ثنائی_ہندسہ_سیٹ(bitarray, ناپ);
	طالما (i < ناپ) {
		لکھو("Bit set: %d\n", i);
		i = ext2fs_لبھو_اگلا_ثنائی_ہندسہ_سیٹ(bitarray, ناپ, i+1);
	}
#جے_انت

	/* Test test_bit */
	لكل (i=0,j=0; i < ناپ; i++) {
		إذا (ext2fs_test_ثنائی_ہندسہ(i, bitarray)) {
			إذا (بٹ_list[j] == i) {
				j++;
			} وإلا {
				لکھو("Bit %d set, not expected\n", i);
				باہر_نکلو(1);
			}
		} وإلا {
			إذا (بٹ_list[j] == i) {
				لکھو("Expected bit %d to be clear.\n", i);
				باہر_نکلو(1);
			}
		}
	}
	لکھو("ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, حجم(testarray));
	لكل (i=0; بٹ_list[i] > 0; i++) {
		ext2fs_سیٹ_ثنائی_ہندسہ(بٹ_list[i], testarray);
	}
	إذا (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		لکھو("ext2fs_set_bit test succeeded.\n");
	} وإلا {
		لکھو("ext2fs_set_bit test failed.\n");
		لكل (i=0; i < حجم(testarray); i++) {
			لکھو("%02x ", testarray[i]);
		}
		لکھو("\n");
		باہر_نکلو(1);
	}
	لكل (i=0; بٹ_list[i] > 0; i++) {
		ext2fs_clear_ثنائی_ہندسہ(بٹ_list[i], testarray);
	}
	لكل (i=0; i < حجم(testarray); i++) {
		إذا (testarray[i]) {
			لکھو("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			باہر_نکلو(1);
		}
	}
	لکھو("ext2fs_clear_bit test succeed.\n");


	/* Do bigarray test */
	bigarray = حافظہ_دیو(1 << 29);
	إذا (!bigarray) {
		fprintf(stderr, "Failed to allocate scratch memory!\n");
		باہر_نکلو(0);
	}

        bigarray[BIG_TEST_ثنائی_ہندسہ >> 3] = 0;

	ext2fs_سیٹ_ثنائی_ہندسہ(BIG_TEST_ثنائی_ہندسہ, bigarray);
	لکھو("big bit number (%u) test: %d, expected %d\n", BIG_TEST_ثنائی_ہندسہ,
	       bigarray[BIG_TEST_ثنائی_ہندسہ >> 3], (1 << (BIG_TEST_ثنائی_ہندسہ & 7)));
	إذا (bigarray[BIG_TEST_ثنائی_ہندسہ >> 3] != (1 << (BIG_TEST_ثنائی_ہندسہ & 7)))
		باہر_نکلو(1);

	ext2fs_clear_ثنائی_ہندسہ(BIG_TEST_ثنائی_ہندسہ, bigarray);

	لکھو("big bit number (%u) test: %d, expected 0\n", BIG_TEST_ثنائی_ہندسہ,
	       bigarray[BIG_TEST_ثنائی_ہندسہ >> 3]);
	إذا (bigarray[BIG_TEST_ثنائی_ہندسہ >> 3] != 0)
		باہر_نکلو(1);

	لکھو("ext2fs_set_bit big_test successful\n");


	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, حجم(testarray));
	لكل (i=0; بٹ_list[i] > 0; i++) {
		ext2fs_fast_سیٹ_ثنائی_ہندسہ(بٹ_list[i], testarray);
	}
	إذا (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		لکھو("ext2fs_fast_set_bit test succeeded.\n");
	} وإلا {
		لکھو("ext2fs_fast_set_bit test failed.\n");
		لكل (i=0; i < حجم(testarray); i++) {
			لکھو("%02x ", testarray[i]);
		}
		لکھو("\n");
		باہر_نکلو(1);
	}
	لكل (i=0; بٹ_list[i] > 0; i++) {
		ext2fs_clear_ثنائی_ہندسہ(بٹ_list[i], testarray);
	}
	لكل (i=0; i < حجم(testarray); i++) {
		إذا (testarray[i]) {
			لکھو("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			باہر_نکلو(1);
		}
	}
	لکھو("ext2fs_clear_bit test succeed.\n");


        bigarray[BIG_TEST_ثنائی_ہندسہ >> 3] = 0;

	ext2fs_fast_سیٹ_ثنائی_ہندسہ(BIG_TEST_ثنائی_ہندسہ, bigarray);
	لکھو("big bit number (%u) test: %d, expected %d\n", BIG_TEST_ثنائی_ہندسہ,
	       bigarray[BIG_TEST_ثنائی_ہندسہ >> 3], (1 << (BIG_TEST_ثنائی_ہندسہ & 7)));
	إذا (bigarray[BIG_TEST_ثنائی_ہندسہ >> 3] != (1 << (BIG_TEST_ثنائی_ہندسہ & 7)))
		باہر_نکلو(1);

	ext2fs_fast_clear_ثنائی_ہندسہ(BIG_TEST_ثنائی_ہندسہ, bigarray);

	لکھو("big bit number (%u) test: %d, expected 0\n", BIG_TEST_ثنائی_ہندسہ,
	       bigarray[BIG_TEST_ثنائی_ہندسہ >> 3]);
	إذا (bigarray[BIG_TEST_ثنائی_ہندسہ >> 3] != 0)
		باہر_نکلو(1);

	لکھو("ext2fs_fast_set_bit big_test successful\n");

	/* Repeat foregoing tests for 64-bit bitops */

	/* Test test_bit */
	لكل (i=0,j=0; i < ناپ; i++) {
		إذا (ext2fs_test_bit64(i, bitarray)) {
			إذا (بٹ_list[j] == i) {
				j++;
			} وإلا {
				لکھو("64-bit: Bit %d set, not expected\n",
				       i);
				باہر_نکلو(1);
			}
		} وإلا {
			إذا (بٹ_list[j] == i) {
				لکھو("64-bit: "
				       "Expected bit %d to be clear.\n", i);
				باہر_نکلو(1);
			}
		}
	}
	لکھو("64-bit: ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, حجم(testarray));
	لكل (i=0; بٹ_list[i] > 0; i++) {
		ext2fs_سیٹ_bit64(بٹ_list[i], testarray);
	}
	إذا (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		لکھو("64-bit: ext2fs_set_bit test succeeded.\n");
	} وإلا {
		لکھو("64-bit: ext2fs_set_bit test failed.\n");
		لكل (i=0; i < حجم(testarray); i++) {
			لکھو("%02x ", testarray[i]);
		}
		لکھو("\n");
		باہر_نکلو(1);
	}
	لكل (i=0; بٹ_list[i] > 0; i++) {
		ext2fs_clear_bit64(بٹ_list[i], testarray);
	}
	لكل (i=0; i < حجم(testarray); i++) {
		إذا (testarray[i]) {
			لکھو("64-bit: ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			باہر_نکلو(1);
		}
	}
	لکھو("64-bit: ext2fs_clear_bit test succeed.\n");

	/* Do bigarray test */
        bigarray[BIG_TEST_ثنائی_ہندسہ >> 3] = 0;

	ext2fs_سیٹ_bit64(BIG_TEST_ثنائی_ہندسہ, bigarray);
	لکھو("64-bit: big bit number (%u) test: %d, expected %d\n",
	       BIG_TEST_ثنائی_ہندسہ, bigarray[BIG_TEST_ثنائی_ہندسہ >> 3],
	       (1 << (BIG_TEST_ثنائی_ہندسہ & 7)));
	إذا (bigarray[BIG_TEST_ثنائی_ہندسہ >> 3] != (1 << (BIG_TEST_ثنائی_ہندسہ & 7)))
		باہر_نکلو(1);

	ext2fs_clear_bit64(BIG_TEST_ثنائی_ہندسہ, bigarray);

	لکھو("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_ثنائی_ہندسہ,
	       bigarray[BIG_TEST_ثنائی_ہندسہ >> 3]);
	إذا (bigarray[BIG_TEST_ثنائی_ہندسہ >> 3] != 0)
		باہر_نکلو(1);

	لکھو("64-bit: ext2fs_set_bit big_test successful\n");

	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, حجم(testarray));
	لكل (i=0; بٹ_list[i] > 0; i++) {
		ext2fs_fast_سیٹ_bit64(بٹ_list[i], testarray);
	}
	إذا (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		لکھو("64-bit: ext2fs_fast_set_bit test succeeded.\n");
	} وإلا {
		لکھو("64-bit: ext2fs_fast_set_bit test failed.\n");
		لكل (i=0; i < حجم(testarray); i++) {
			لکھو("%02x ", testarray[i]);
		}
		لکھو("\n");
		باہر_نکلو(1);
	}
	لكل (i=0; بٹ_list[i] > 0; i++) {
		ext2fs_clear_bit64(بٹ_list[i], testarray);
	}
	لكل (i=0; i < حجم(testarray); i++) {
		إذا (testarray[i]) {
			لکھو("64-bit: ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			باہر_نکلو(1);
		}
	}
	لکھو("64-bit: ext2fs_clear_bit test succeed.\n");

        bigarray[BIG_TEST_ثنائی_ہندسہ >> 3] = 0;

	ext2fs_fast_سیٹ_bit64(BIG_TEST_ثنائی_ہندسہ, bigarray);
	لکھو("64-bit: big bit number (%u) test: %d, expected %d\n",
	       BIG_TEST_ثنائی_ہندسہ, bigarray[BIG_TEST_ثنائی_ہندسہ >> 3],
	       (1 << (BIG_TEST_ثنائی_ہندسہ & 7)));
	إذا (bigarray[BIG_TEST_ثنائی_ہندسہ >> 3] != (1 << (BIG_TEST_ثنائی_ہندسہ & 7)))
		باہر_نکلو(1);

	ext2fs_fast_clear_bit64(BIG_TEST_ثنائی_ہندسہ, bigarray);

	لکھو("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_ثنائی_ہندسہ, bigarray[BIG_TEST_ثنائی_ہندسہ >> 3]);
	إذا (bigarray[BIG_TEST_ثنائی_ہندسہ >> 3] != 0)
		باہر_نکلو(1);

	لکھو("64-bit: ext2fs_fast_set_bit big_test successful\n");
	حرر(bigarray);
	باہر_نکلو(0);
}
