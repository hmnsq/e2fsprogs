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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <fcntl.h>
#شامل <time.h>
#شامل <sys/stat.h>
#شامل <sys/types.h>
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#اگر_ختم
#شامل <sys/time.h>
#شامل <sys/resource.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

غیرعلامتی حرف bitarray[] = {
	0x80, 0xF0, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x10, 0x20, 0x00, 0x00
	};

صحیح بٹ_list[] = {
	7, 12, 13, 14,15, 22, 30, 68, 77, -1,
};

#تعریف_کرو BIG_TEST_ثنائی_ہندسہ   (((غیرعلامتی) 1 << 31) + 42)


صحیح مین(صحیح argc, حرف **argv)
{
	صحیح	i, j, سائز;
	غیرعلامتی حرف testarray[12];
	غیرعلامتی حرف *bigarray;

	سائز = حجم(bitarray)*8;
#اگر 0
	i = ext2fs_ڈھونڈو_پہلا_ثنائی_ہندسہ_سیٹ(bitarray, سائز);
	جب (i < سائز) {
		لکھو("Bit set: %d\n", i);
		i = ext2fs_ڈھونڈو_اگلا_ثنائی_ہندسہ_سیٹ(bitarray, سائز, i+1);
	}
#اگر_ختم

	/* Test test_bit */
	برائے (i=0,j=0; i < سائز; i++) {
		اگر (ext2fs_test_ثنائی_ہندسہ(i, bitarray)) {
			اگر (بٹ_list[j] == i) {
				j++;
			} ورنہ {
				لکھو("Bit %d set, not expected\n", i);
				باہر_نکلو(1);
			}
		} ورنہ {
			اگر (بٹ_list[j] == i) {
				لکھو("Expected bit %d to be clear.\n", i);
				باہر_نکلو(1);
			}
		}
	}
	لکھو("ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, حجم(testarray));
	برائے (i=0; بٹ_list[i] > 0; i++) {
		ext2fs_سیٹ_ثنائی_ہندسہ(بٹ_list[i], testarray);
	}
	اگر (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		لکھو("ext2fs_set_bit test succeeded.\n");
	} ورنہ {
		لکھو("ext2fs_set_bit test failed.\n");
		برائے (i=0; i < حجم(testarray); i++) {
			لکھو("%02x ", testarray[i]);
		}
		لکھو("\n");
		باہر_نکلو(1);
	}
	برائے (i=0; بٹ_list[i] > 0; i++) {
		ext2fs_clear_ثنائی_ہندسہ(بٹ_list[i], testarray);
	}
	برائے (i=0; i < حجم(testarray); i++) {
		اگر (testarray[i]) {
			لکھو("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			باہر_نکلو(1);
		}
	}
	لکھو("ext2fs_clear_bit test succeed.\n");


	/* Do bigarray test */
	bigarray = حافظہ_دو(1 << 29);
	اگر (!bigarray) {
		fprintf(stderr, "Failed to allocate scratch memory!\n");
		باہر_نکلو(0);
	}

        bigarray[BIG_TEST_ثنائی_ہندسہ >> 3] = 0;

	ext2fs_سیٹ_ثنائی_ہندسہ(BIG_TEST_ثنائی_ہندسہ, bigarray);
	لکھو("big bit number (%u) test: %d, expected %d\n", BIG_TEST_ثنائی_ہندسہ,
	       bigarray[BIG_TEST_ثنائی_ہندسہ >> 3], (1 << (BIG_TEST_ثنائی_ہندسہ & 7)));
	اگر (bigarray[BIG_TEST_ثنائی_ہندسہ >> 3] != (1 << (BIG_TEST_ثنائی_ہندسہ & 7)))
		باہر_نکلو(1);

	ext2fs_clear_ثنائی_ہندسہ(BIG_TEST_ثنائی_ہندسہ, bigarray);

	لکھو("big bit number (%u) test: %d, expected 0\n", BIG_TEST_ثنائی_ہندسہ,
	       bigarray[BIG_TEST_ثنائی_ہندسہ >> 3]);
	اگر (bigarray[BIG_TEST_ثنائی_ہندسہ >> 3] != 0)
		باہر_نکلو(1);

	لکھو("ext2fs_set_bit big_test successful\n");


	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, حجم(testarray));
	برائے (i=0; بٹ_list[i] > 0; i++) {
		ext2fs_fast_سیٹ_ثنائی_ہندسہ(بٹ_list[i], testarray);
	}
	اگر (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		لکھو("ext2fs_fast_set_bit test succeeded.\n");
	} ورنہ {
		لکھو("ext2fs_fast_set_bit test failed.\n");
		برائے (i=0; i < حجم(testarray); i++) {
			لکھو("%02x ", testarray[i]);
		}
		لکھو("\n");
		باہر_نکلو(1);
	}
	برائے (i=0; بٹ_list[i] > 0; i++) {
		ext2fs_clear_ثنائی_ہندسہ(بٹ_list[i], testarray);
	}
	برائے (i=0; i < حجم(testarray); i++) {
		اگر (testarray[i]) {
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
	اگر (bigarray[BIG_TEST_ثنائی_ہندسہ >> 3] != (1 << (BIG_TEST_ثنائی_ہندسہ & 7)))
		باہر_نکلو(1);

	ext2fs_fast_clear_ثنائی_ہندسہ(BIG_TEST_ثنائی_ہندسہ, bigarray);

	لکھو("big bit number (%u) test: %d, expected 0\n", BIG_TEST_ثنائی_ہندسہ,
	       bigarray[BIG_TEST_ثنائی_ہندسہ >> 3]);
	اگر (bigarray[BIG_TEST_ثنائی_ہندسہ >> 3] != 0)
		باہر_نکلو(1);

	لکھو("ext2fs_fast_set_bit big_test successful\n");

	/* Repeat foregoing tests for 64-bit bitops */

	/* Test test_bit */
	برائے (i=0,j=0; i < سائز; i++) {
		اگر (ext2fs_test_bit64(i, bitarray)) {
			اگر (بٹ_list[j] == i) {
				j++;
			} ورنہ {
				لکھو("64-bit: Bit %d set, not expected\n",
				       i);
				باہر_نکلو(1);
			}
		} ورنہ {
			اگر (بٹ_list[j] == i) {
				لکھو("64-bit: "
				       "Expected bit %d to be clear.\n", i);
				باہر_نکلو(1);
			}
		}
	}
	لکھو("64-bit: ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, حجم(testarray));
	برائے (i=0; بٹ_list[i] > 0; i++) {
		ext2fs_سیٹ_bit64(بٹ_list[i], testarray);
	}
	اگر (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		لکھو("64-bit: ext2fs_set_bit test succeeded.\n");
	} ورنہ {
		لکھو("64-bit: ext2fs_set_bit test failed.\n");
		برائے (i=0; i < حجم(testarray); i++) {
			لکھو("%02x ", testarray[i]);
		}
		لکھو("\n");
		باہر_نکلو(1);
	}
	برائے (i=0; بٹ_list[i] > 0; i++) {
		ext2fs_clear_bit64(بٹ_list[i], testarray);
	}
	برائے (i=0; i < حجم(testarray); i++) {
		اگر (testarray[i]) {
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
	اگر (bigarray[BIG_TEST_ثنائی_ہندسہ >> 3] != (1 << (BIG_TEST_ثنائی_ہندسہ & 7)))
		باہر_نکلو(1);

	ext2fs_clear_bit64(BIG_TEST_ثنائی_ہندسہ, bigarray);

	لکھو("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_ثنائی_ہندسہ,
	       bigarray[BIG_TEST_ثنائی_ہندسہ >> 3]);
	اگر (bigarray[BIG_TEST_ثنائی_ہندسہ >> 3] != 0)
		باہر_نکلو(1);

	لکھو("64-bit: ext2fs_set_bit big_test successful\n");

	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, حجم(testarray));
	برائے (i=0; بٹ_list[i] > 0; i++) {
		ext2fs_fast_سیٹ_bit64(بٹ_list[i], testarray);
	}
	اگر (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		لکھو("64-bit: ext2fs_fast_set_bit test succeeded.\n");
	} ورنہ {
		لکھو("64-bit: ext2fs_fast_set_bit test failed.\n");
		برائے (i=0; i < حجم(testarray); i++) {
			لکھو("%02x ", testarray[i]);
		}
		لکھو("\n");
		باہر_نکلو(1);
	}
	برائے (i=0; بٹ_list[i] > 0; i++) {
		ext2fs_clear_bit64(بٹ_list[i], testarray);
	}
	برائے (i=0; i < حجم(testarray); i++) {
		اگر (testarray[i]) {
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
	اگر (bigarray[BIG_TEST_ثنائی_ہندسہ >> 3] != (1 << (BIG_TEST_ثنائی_ہندسہ & 7)))
		باہر_نکلو(1);

	ext2fs_fast_clear_bit64(BIG_TEST_ثنائی_ہندسہ, bigarray);

	لکھو("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_ثنائی_ہندسہ, bigarray[BIG_TEST_ثنائی_ہندسہ >> 3]);
	اگر (bigarray[BIG_TEST_ثنائی_ہندسہ >> 3] != 0)
		باہر_نکلو(1);

	لکھو("64-bit: ext2fs_fast_set_bit big_test successful\n");
	آزاد(bigarray);
	باہر_نکلو(0);
}
