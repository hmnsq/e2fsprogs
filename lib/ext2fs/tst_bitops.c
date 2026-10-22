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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ HAVE_ERRNO_H
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/resource.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ bitarray[] = {
	0x80, 0xF0, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x10, 0x20, 0x00, 0x00
	};

ਪੂਰਨ ਬਿੱਟ_list[] = {
	7, 12, 13, 14,15, 22, 30, 68, 77, -1,
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ   (((ਬਿਨਾਨਿਸ਼ਾਨੀ) 1 << 31) + 42)


ਪੂਰਨ ਮੁੱਖ(ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
	ਪੂਰਨ	i, j, ਮਾਪ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ testarray[12];
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *bigarray;

	ਮਾਪ = ਆਕਾਰ(bitarray)*8;
#ਜੇ 0
	i = ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ(bitarray, ਮਾਪ);
	ਜਦੋਂ (i < ਮਾਪ) {
		ਲਿਖੋ("Bit set: %d\n", i);
		i = ext2fs_ਲੱਭੋ_ਅਗਲਾ_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ(bitarray, ਮਾਪ, i+1);
	}
#ਜੇ_ਅੰਤ

	/* Test test_bit */
	ਲਈ (i=0,j=0; i < ਮਾਪ; i++) {
		ਜੇ (ext2fs_test_ਦੋਆਧਾਰੀ_ਅੰਕ(i, bitarray)) {
			ਜੇ (ਬਿੱਟ_list[j] == i) {
				j++;
			} ਨਹੀਂਤਾਂ {
				ਲਿਖੋ("Bit %d set, not expected\n", i);
				ਬਾਹਰ_ਨਿਕਲੋ(1);
			}
		} ਨਹੀਂਤਾਂ {
			ਜੇ (ਬਿੱਟ_list[j] == i) {
				ਲਿਖੋ("Expected bit %d to be clear.\n", i);
				ਬਾਹਰ_ਨਿਕਲੋ(1);
			}
		}
	}
	ਲਿਖੋ("ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, ਆਕਾਰ(testarray));
	ਲਈ (i=0; ਬਿੱਟ_list[i] > 0; i++) {
		ext2fs_ਸੈੱਟ_ਦੋਆਧਾਰੀ_ਅੰਕ(ਬਿੱਟ_list[i], testarray);
	}
	ਜੇ (memcmp(testarray, bitarray, ਆਕਾਰ(testarray)) == 0) {
		ਲਿਖੋ("ext2fs_set_bit test succeeded.\n");
	} ਨਹੀਂਤਾਂ {
		ਲਿਖੋ("ext2fs_set_bit test failed.\n");
		ਲਈ (i=0; i < ਆਕਾਰ(testarray); i++) {
			ਲਿਖੋ("%02x ", testarray[i]);
		}
		ਲਿਖੋ("\n");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਲਈ (i=0; ਬਿੱਟ_list[i] > 0; i++) {
		ext2fs_clear_ਦੋਆਧਾਰੀ_ਅੰਕ(ਬਿੱਟ_list[i], testarray);
	}
	ਲਈ (i=0; i < ਆਕਾਰ(testarray); i++) {
		ਜੇ (testarray[i]) {
			ਲਿਖੋ("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			ਬਾਹਰ_ਨਿਕਲੋ(1);
		}
	}
	ਲਿਖੋ("ext2fs_clear_bit test succeed.\n");


	/* Do bigarray test */
	bigarray = ਸਮ੍ਰਿਤੀ_ਦਿਓ(1 << 29);
	ਜੇ (!bigarray) {
		fprintf(stderr, "Failed to allocate scratch memory!\n");
		ਬਾਹਰ_ਨਿਕਲੋ(0);
	}

        bigarray[BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ >> 3] = 0;

	ext2fs_ਸੈੱਟ_ਦੋਆਧਾਰੀ_ਅੰਕ(BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ, bigarray);
	ਲਿਖੋ("big bit number (%u) test: %d, expected %d\n", BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ,
	       bigarray[BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ >> 3], (1 << (BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ & 7)));
	ਜੇ (bigarray[BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ >> 3] != (1 << (BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ & 7)))
		ਬਾਹਰ_ਨਿਕਲੋ(1);

	ext2fs_clear_ਦੋਆਧਾਰੀ_ਅੰਕ(BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ, bigarray);

	ਲਿਖੋ("big bit number (%u) test: %d, expected 0\n", BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ,
	       bigarray[BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ >> 3]);
	ਜੇ (bigarray[BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ >> 3] != 0)
		ਬਾਹਰ_ਨਿਕਲੋ(1);

	ਲਿਖੋ("ext2fs_set_bit big_test successful\n");


	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, ਆਕਾਰ(testarray));
	ਲਈ (i=0; ਬਿੱਟ_list[i] > 0; i++) {
		ext2fs_fast_ਸੈੱਟ_ਦੋਆਧਾਰੀ_ਅੰਕ(ਬਿੱਟ_list[i], testarray);
	}
	ਜੇ (memcmp(testarray, bitarray, ਆਕਾਰ(testarray)) == 0) {
		ਲਿਖੋ("ext2fs_fast_set_bit test succeeded.\n");
	} ਨਹੀਂਤਾਂ {
		ਲਿਖੋ("ext2fs_fast_set_bit test failed.\n");
		ਲਈ (i=0; i < ਆਕਾਰ(testarray); i++) {
			ਲਿਖੋ("%02x ", testarray[i]);
		}
		ਲਿਖੋ("\n");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਲਈ (i=0; ਬਿੱਟ_list[i] > 0; i++) {
		ext2fs_clear_ਦੋਆਧਾਰੀ_ਅੰਕ(ਬਿੱਟ_list[i], testarray);
	}
	ਲਈ (i=0; i < ਆਕਾਰ(testarray); i++) {
		ਜੇ (testarray[i]) {
			ਲਿਖੋ("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			ਬਾਹਰ_ਨਿਕਲੋ(1);
		}
	}
	ਲਿਖੋ("ext2fs_clear_bit test succeed.\n");


        bigarray[BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ >> 3] = 0;

	ext2fs_fast_ਸੈੱਟ_ਦੋਆਧਾਰੀ_ਅੰਕ(BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ, bigarray);
	ਲਿਖੋ("big bit number (%u) test: %d, expected %d\n", BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ,
	       bigarray[BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ >> 3], (1 << (BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ & 7)));
	ਜੇ (bigarray[BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ >> 3] != (1 << (BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ & 7)))
		ਬਾਹਰ_ਨਿਕਲੋ(1);

	ext2fs_fast_clear_ਦੋਆਧਾਰੀ_ਅੰਕ(BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ, bigarray);

	ਲਿਖੋ("big bit number (%u) test: %d, expected 0\n", BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ,
	       bigarray[BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ >> 3]);
	ਜੇ (bigarray[BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ >> 3] != 0)
		ਬਾਹਰ_ਨਿਕਲੋ(1);

	ਲਿਖੋ("ext2fs_fast_set_bit big_test successful\n");

	/* Repeat foregoing tests for 64-bit bitops */

	/* Test test_bit */
	ਲਈ (i=0,j=0; i < ਮਾਪ; i++) {
		ਜੇ (ext2fs_test_bit64(i, bitarray)) {
			ਜੇ (ਬਿੱਟ_list[j] == i) {
				j++;
			} ਨਹੀਂਤਾਂ {
				ਲਿਖੋ("64-bit: Bit %d set, not expected\n",
				       i);
				ਬਾਹਰ_ਨਿਕਲੋ(1);
			}
		} ਨਹੀਂਤਾਂ {
			ਜੇ (ਬਿੱਟ_list[j] == i) {
				ਲਿਖੋ("64-bit: "
				       "Expected bit %d to be clear.\n", i);
				ਬਾਹਰ_ਨਿਕਲੋ(1);
			}
		}
	}
	ਲਿਖੋ("64-bit: ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, ਆਕਾਰ(testarray));
	ਲਈ (i=0; ਬਿੱਟ_list[i] > 0; i++) {
		ext2fs_ਸੈੱਟ_bit64(ਬਿੱਟ_list[i], testarray);
	}
	ਜੇ (memcmp(testarray, bitarray, ਆਕਾਰ(testarray)) == 0) {
		ਲਿਖੋ("64-bit: ext2fs_set_bit test succeeded.\n");
	} ਨਹੀਂਤਾਂ {
		ਲਿਖੋ("64-bit: ext2fs_set_bit test failed.\n");
		ਲਈ (i=0; i < ਆਕਾਰ(testarray); i++) {
			ਲਿਖੋ("%02x ", testarray[i]);
		}
		ਲਿਖੋ("\n");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਲਈ (i=0; ਬਿੱਟ_list[i] > 0; i++) {
		ext2fs_clear_bit64(ਬਿੱਟ_list[i], testarray);
	}
	ਲਈ (i=0; i < ਆਕਾਰ(testarray); i++) {
		ਜੇ (testarray[i]) {
			ਲਿਖੋ("64-bit: ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			ਬਾਹਰ_ਨਿਕਲੋ(1);
		}
	}
	ਲਿਖੋ("64-bit: ext2fs_clear_bit test succeed.\n");

	/* Do bigarray test */
        bigarray[BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ >> 3] = 0;

	ext2fs_ਸੈੱਟ_bit64(BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ, bigarray);
	ਲਿਖੋ("64-bit: big bit number (%u) test: %d, expected %d\n",
	       BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ, bigarray[BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ >> 3],
	       (1 << (BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ & 7)));
	ਜੇ (bigarray[BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ >> 3] != (1 << (BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ & 7)))
		ਬਾਹਰ_ਨਿਕਲੋ(1);

	ext2fs_clear_bit64(BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ, bigarray);

	ਲਿਖੋ("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ,
	       bigarray[BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ >> 3]);
	ਜੇ (bigarray[BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ >> 3] != 0)
		ਬਾਹਰ_ਨਿਕਲੋ(1);

	ਲਿਖੋ("64-bit: ext2fs_set_bit big_test successful\n");

	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, ਆਕਾਰ(testarray));
	ਲਈ (i=0; ਬਿੱਟ_list[i] > 0; i++) {
		ext2fs_fast_ਸੈੱਟ_bit64(ਬਿੱਟ_list[i], testarray);
	}
	ਜੇ (memcmp(testarray, bitarray, ਆਕਾਰ(testarray)) == 0) {
		ਲਿਖੋ("64-bit: ext2fs_fast_set_bit test succeeded.\n");
	} ਨਹੀਂਤਾਂ {
		ਲਿਖੋ("64-bit: ext2fs_fast_set_bit test failed.\n");
		ਲਈ (i=0; i < ਆਕਾਰ(testarray); i++) {
			ਲਿਖੋ("%02x ", testarray[i]);
		}
		ਲਿਖੋ("\n");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਲਈ (i=0; ਬਿੱਟ_list[i] > 0; i++) {
		ext2fs_clear_bit64(ਬਿੱਟ_list[i], testarray);
	}
	ਲਈ (i=0; i < ਆਕਾਰ(testarray); i++) {
		ਜੇ (testarray[i]) {
			ਲਿਖੋ("64-bit: ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			ਬਾਹਰ_ਨਿਕਲੋ(1);
		}
	}
	ਲਿਖੋ("64-bit: ext2fs_clear_bit test succeed.\n");

        bigarray[BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ >> 3] = 0;

	ext2fs_fast_ਸੈੱਟ_bit64(BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ, bigarray);
	ਲਿਖੋ("64-bit: big bit number (%u) test: %d, expected %d\n",
	       BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ, bigarray[BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ >> 3],
	       (1 << (BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ & 7)));
	ਜੇ (bigarray[BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ >> 3] != (1 << (BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ & 7)))
		ਬਾਹਰ_ਨਿਕਲੋ(1);

	ext2fs_fast_clear_bit64(BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ, bigarray);

	ਲਿਖੋ("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ, bigarray[BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ >> 3]);
	ਜੇ (bigarray[BIG_TEST_ਦੋਆਧਾਰੀ_ਅੰਕ >> 3] != 0)
		ਬਾਹਰ_ਨਿਕਲੋ(1);

	ਲਿਖੋ("64-bit: ext2fs_fast_set_bit big_test successful\n");
	ਮੁਕਤ(bigarray);
	ਬਾਹਰ_ਨਿਕਲੋ(0);
}
