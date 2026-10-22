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
#جيڪڏهن_پورو
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جيڪڏهن_پورو
#ضمن <sys/time.h>
#ضمن <sys/resource.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

بنانشان حرف bitarray[] = {
	0x80, 0xF0, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x10, 0x20, 0x00, 0x00
	};

صحيح بٽ_list[] = {
	7, 12, 13, 14,15, 22, 30, 68, 77, -1,
};

#وصف_ڪريو BIG_TEST_ٻٽو_انگ   (((بنانشان) 1 << 31) + 42)


صحيح مکيه(صحيح argc, حرف **argv)
{
	صحيح	i, j, سائيز;
	بنانشان حرف testarray[12];
	بنانشان حرف *bigarray;

	سائيز = حجم(bitarray)*8;
#إذا 0
	i = ext2fs_ڳوليو_پهريون_ٻٽو_انگ_سيٽ(bitarray, سائيز);
	طالما (i < سائيز) {
		لکو("Bit set: %d\n", i);
		i = ext2fs_ڳوليو_ايندڙ_ٻٽو_انگ_سيٽ(bitarray, سائيز, i+1);
	}
#جيڪڏهن_پورو

	/* Test test_bit */
	لكل (i=0,j=0; i < سائيز; i++) {
		إذا (ext2fs_test_ٻٽو_انگ(i, bitarray)) {
			إذا (بٽ_list[j] == i) {
				j++;
			} وإلا {
				لکو("Bit %d set, not expected\n", i);
				ٻاهر_نڪرو(1);
			}
		} وإلا {
			إذا (بٽ_list[j] == i) {
				لکو("Expected bit %d to be clear.\n", i);
				ٻاهر_نڪرو(1);
			}
		}
	}
	لکو("ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, حجم(testarray));
	لكل (i=0; بٽ_list[i] > 0; i++) {
		ext2fs_سيٽ_ٻٽو_انگ(بٽ_list[i], testarray);
	}
	إذا (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		لکو("ext2fs_set_bit test succeeded.\n");
	} وإلا {
		لکو("ext2fs_set_bit test failed.\n");
		لكل (i=0; i < حجم(testarray); i++) {
			لکو("%02x ", testarray[i]);
		}
		لکو("\n");
		ٻاهر_نڪرو(1);
	}
	لكل (i=0; بٽ_list[i] > 0; i++) {
		ext2fs_clear_ٻٽو_انگ(بٽ_list[i], testarray);
	}
	لكل (i=0; i < حجم(testarray); i++) {
		إذا (testarray[i]) {
			لکو("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			ٻاهر_نڪرو(1);
		}
	}
	لکو("ext2fs_clear_bit test succeed.\n");


	/* Do bigarray test */
	bigarray = يادگيري_ڏيو(1 << 29);
	إذا (!bigarray) {
		fprintf(stderr, "Failed to allocate scratch memory!\n");
		ٻاهر_نڪرو(0);
	}

        bigarray[BIG_TEST_ٻٽو_انگ >> 3] = 0;

	ext2fs_سيٽ_ٻٽو_انگ(BIG_TEST_ٻٽو_انگ, bigarray);
	لکو("big bit number (%u) test: %d, expected %d\n", BIG_TEST_ٻٽو_انگ,
	       bigarray[BIG_TEST_ٻٽو_انگ >> 3], (1 << (BIG_TEST_ٻٽو_انگ & 7)));
	إذا (bigarray[BIG_TEST_ٻٽو_انگ >> 3] != (1 << (BIG_TEST_ٻٽو_انگ & 7)))
		ٻاهر_نڪرو(1);

	ext2fs_clear_ٻٽو_انگ(BIG_TEST_ٻٽو_انگ, bigarray);

	لکو("big bit number (%u) test: %d, expected 0\n", BIG_TEST_ٻٽو_انگ,
	       bigarray[BIG_TEST_ٻٽو_انگ >> 3]);
	إذا (bigarray[BIG_TEST_ٻٽو_انگ >> 3] != 0)
		ٻاهر_نڪرو(1);

	لکو("ext2fs_set_bit big_test successful\n");


	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, حجم(testarray));
	لكل (i=0; بٽ_list[i] > 0; i++) {
		ext2fs_fast_سيٽ_ٻٽو_انگ(بٽ_list[i], testarray);
	}
	إذا (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		لکو("ext2fs_fast_set_bit test succeeded.\n");
	} وإلا {
		لکو("ext2fs_fast_set_bit test failed.\n");
		لكل (i=0; i < حجم(testarray); i++) {
			لکو("%02x ", testarray[i]);
		}
		لکو("\n");
		ٻاهر_نڪرو(1);
	}
	لكل (i=0; بٽ_list[i] > 0; i++) {
		ext2fs_clear_ٻٽو_انگ(بٽ_list[i], testarray);
	}
	لكل (i=0; i < حجم(testarray); i++) {
		إذا (testarray[i]) {
			لکو("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			ٻاهر_نڪرو(1);
		}
	}
	لکو("ext2fs_clear_bit test succeed.\n");


        bigarray[BIG_TEST_ٻٽو_انگ >> 3] = 0;

	ext2fs_fast_سيٽ_ٻٽو_انگ(BIG_TEST_ٻٽو_انگ, bigarray);
	لکو("big bit number (%u) test: %d, expected %d\n", BIG_TEST_ٻٽو_انگ,
	       bigarray[BIG_TEST_ٻٽو_انگ >> 3], (1 << (BIG_TEST_ٻٽو_انگ & 7)));
	إذا (bigarray[BIG_TEST_ٻٽو_انگ >> 3] != (1 << (BIG_TEST_ٻٽو_انگ & 7)))
		ٻاهر_نڪرو(1);

	ext2fs_fast_clear_ٻٽو_انگ(BIG_TEST_ٻٽو_انگ, bigarray);

	لکو("big bit number (%u) test: %d, expected 0\n", BIG_TEST_ٻٽو_انگ,
	       bigarray[BIG_TEST_ٻٽو_انگ >> 3]);
	إذا (bigarray[BIG_TEST_ٻٽو_انگ >> 3] != 0)
		ٻاهر_نڪرو(1);

	لکو("ext2fs_fast_set_bit big_test successful\n");

	/* Repeat foregoing tests for 64-bit bitops */

	/* Test test_bit */
	لكل (i=0,j=0; i < سائيز; i++) {
		إذا (ext2fs_test_bit64(i, bitarray)) {
			إذا (بٽ_list[j] == i) {
				j++;
			} وإلا {
				لکو("64-bit: Bit %d set, not expected\n",
				       i);
				ٻاهر_نڪرو(1);
			}
		} وإلا {
			إذا (بٽ_list[j] == i) {
				لکو("64-bit: "
				       "Expected bit %d to be clear.\n", i);
				ٻاهر_نڪرو(1);
			}
		}
	}
	لکو("64-bit: ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, حجم(testarray));
	لكل (i=0; بٽ_list[i] > 0; i++) {
		ext2fs_سيٽ_bit64(بٽ_list[i], testarray);
	}
	إذا (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		لکو("64-bit: ext2fs_set_bit test succeeded.\n");
	} وإلا {
		لکو("64-bit: ext2fs_set_bit test failed.\n");
		لكل (i=0; i < حجم(testarray); i++) {
			لکو("%02x ", testarray[i]);
		}
		لکو("\n");
		ٻاهر_نڪرو(1);
	}
	لكل (i=0; بٽ_list[i] > 0; i++) {
		ext2fs_clear_bit64(بٽ_list[i], testarray);
	}
	لكل (i=0; i < حجم(testarray); i++) {
		إذا (testarray[i]) {
			لکو("64-bit: ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			ٻاهر_نڪرو(1);
		}
	}
	لکو("64-bit: ext2fs_clear_bit test succeed.\n");

	/* Do bigarray test */
        bigarray[BIG_TEST_ٻٽو_انگ >> 3] = 0;

	ext2fs_سيٽ_bit64(BIG_TEST_ٻٽو_انگ, bigarray);
	لکو("64-bit: big bit number (%u) test: %d, expected %d\n",
	       BIG_TEST_ٻٽو_انگ, bigarray[BIG_TEST_ٻٽو_انگ >> 3],
	       (1 << (BIG_TEST_ٻٽو_انگ & 7)));
	إذا (bigarray[BIG_TEST_ٻٽو_انگ >> 3] != (1 << (BIG_TEST_ٻٽو_انگ & 7)))
		ٻاهر_نڪرو(1);

	ext2fs_clear_bit64(BIG_TEST_ٻٽو_انگ, bigarray);

	لکو("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_ٻٽو_انگ,
	       bigarray[BIG_TEST_ٻٽو_انگ >> 3]);
	إذا (bigarray[BIG_TEST_ٻٽو_انگ >> 3] != 0)
		ٻاهر_نڪرو(1);

	لکو("64-bit: ext2fs_set_bit big_test successful\n");

	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, حجم(testarray));
	لكل (i=0; بٽ_list[i] > 0; i++) {
		ext2fs_fast_سيٽ_bit64(بٽ_list[i], testarray);
	}
	إذا (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		لکو("64-bit: ext2fs_fast_set_bit test succeeded.\n");
	} وإلا {
		لکو("64-bit: ext2fs_fast_set_bit test failed.\n");
		لكل (i=0; i < حجم(testarray); i++) {
			لکو("%02x ", testarray[i]);
		}
		لکو("\n");
		ٻاهر_نڪرو(1);
	}
	لكل (i=0; بٽ_list[i] > 0; i++) {
		ext2fs_clear_bit64(بٽ_list[i], testarray);
	}
	لكل (i=0; i < حجم(testarray); i++) {
		إذا (testarray[i]) {
			لکو("64-bit: ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			ٻاهر_نڪرو(1);
		}
	}
	لکو("64-bit: ext2fs_clear_bit test succeed.\n");

        bigarray[BIG_TEST_ٻٽو_انگ >> 3] = 0;

	ext2fs_fast_سيٽ_bit64(BIG_TEST_ٻٽو_انگ, bigarray);
	لکو("64-bit: big bit number (%u) test: %d, expected %d\n",
	       BIG_TEST_ٻٽو_انگ, bigarray[BIG_TEST_ٻٽو_انگ >> 3],
	       (1 << (BIG_TEST_ٻٽو_انگ & 7)));
	إذا (bigarray[BIG_TEST_ٻٽو_انگ >> 3] != (1 << (BIG_TEST_ٻٽو_انگ & 7)))
		ٻاهر_نڪرو(1);

	ext2fs_fast_clear_bit64(BIG_TEST_ٻٽو_انگ, bigarray);

	لکو("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_ٻٽو_انگ, bigarray[BIG_TEST_ٻٽو_انگ >> 3]);
	إذا (bigarray[BIG_TEST_ٻٽو_انگ >> 3] != 0)
		ٻاهر_نڪرو(1);

	لکو("64-bit: ext2fs_fast_set_bit big_test successful\n");
	حرر(bigarray);
	ٻاهر_نڪرو(0);
}
