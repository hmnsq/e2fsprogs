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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <sys/time.h>
#ಅನ್ತರ್ಭಾವ <sys/resource.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ bitarray[] = {
	0x80, 0xF0, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x10, 0x20, 0x00, 0x00
	};

ಪೂರ್ಣಾಂಕ ಬಿಟ್ಟುಗಳು_list[] = {
	7, 12, 13, 14,15, 22, 30, 68, 77, -1,
};

#ವ್ಯಾಖ್ಯಾನಿಸು BIG_TEST_ಬಿಟ್   (((ಚಿಹ್ನೆರಹಿತ) 1 << 31) + 42)


ಪೂರ್ಣಾಂಕ ಮುಖ್ಯ(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
	ಪೂರ್ಣಾಂಕ	i, j, ಗಾತ್ರ_2;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ testarray[12];
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *bigarray;

	ಗಾತ್ರ_2 = ಗಾತ್ರ(bitarray)*8;
#ಆದರೆ 0
	i = ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಬಿಟ್_ಗಣ(bitarray, ಗಾತ್ರ_2);
	ಆಗ (i < ಗಾತ್ರ_2) {
		ಬರೆ("Bit set: %d\n", i);
		i = ext2fs_ಕಂಡುಹಿಡಿ_ಮುಂದಿನ_ಬಿಟ್_ಗಣ(bitarray, ಗಾತ್ರ_2, i+1);
	}
#ಆದರೆ_ಅಂತ್ಯ

	/* Test test_bit */
	ಗಾಗಿ (i=0,j=0; i < ಗಾತ್ರ_2; i++) {
		ಆದರೆ (ext2fs_test_ಬಿಟ್(i, bitarray)) {
			ಆದರೆ (ಬಿಟ್ಟುಗಳು_list[j] == i) {
				j++;
			} ಇಲ್ಲದಿದ್ದರೆ {
				ಬರೆ("Bit %d set, not expected\n", i);
				ನಿರ್ಗಮಿಸು(1);
			}
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಆದರೆ (ಬಿಟ್ಟುಗಳು_list[j] == i) {
				ಬರೆ("Expected bit %d to be clear.\n", i);
				ನಿರ್ಗಮಿಸು(1);
			}
		}
	}
	ಬರೆ("ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, ಗಾತ್ರ(testarray));
	ಗಾಗಿ (i=0; ಬಿಟ್ಟುಗಳು_list[i] > 0; i++) {
		ext2fs_ಗಣ_ಬಿಟ್(ಬಿಟ್ಟುಗಳು_list[i], testarray);
	}
	ಆದರೆ (memcmp(testarray, bitarray, ಗಾತ್ರ(testarray)) == 0) {
		ಬರೆ("ext2fs_set_bit test succeeded.\n");
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಬರೆ("ext2fs_set_bit test failed.\n");
		ಗಾಗಿ (i=0; i < ಗಾತ್ರ(testarray); i++) {
			ಬರೆ("%02x ", testarray[i]);
		}
		ಬರೆ("\n");
		ನಿರ್ಗಮಿಸು(1);
	}
	ಗಾಗಿ (i=0; ಬಿಟ್ಟುಗಳು_list[i] > 0; i++) {
		ext2fs_clear_ಬಿಟ್(ಬಿಟ್ಟುಗಳು_list[i], testarray);
	}
	ಗಾಗಿ (i=0; i < ಗಾತ್ರ(testarray); i++) {
		ಆದರೆ (testarray[i]) {
			ಬರೆ("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			ನಿರ್ಗಮಿಸು(1);
		}
	}
	ಬರೆ("ext2fs_clear_bit test succeed.\n");


	/* Do bigarray test */
	bigarray = ಸ್ಮರಣೆ_ಕೊಡು(1 << 29);
	ಆದರೆ (!bigarray) {
		fprintf(stderr, "Failed to allocate scratch memory!\n");
		ನಿರ್ಗಮಿಸು(0);
	}

        bigarray[BIG_TEST_ಬಿಟ್ >> 3] = 0;

	ext2fs_ಗಣ_ಬಿಟ್(BIG_TEST_ಬಿಟ್, bigarray);
	ಬರೆ("big bit number (%u) test: %d, expected %d\n", BIG_TEST_ಬಿಟ್,
	       bigarray[BIG_TEST_ಬಿಟ್ >> 3], (1 << (BIG_TEST_ಬಿಟ್ & 7)));
	ಆದರೆ (bigarray[BIG_TEST_ಬಿಟ್ >> 3] != (1 << (BIG_TEST_ಬಿಟ್ & 7)))
		ನಿರ್ಗಮಿಸು(1);

	ext2fs_clear_ಬಿಟ್(BIG_TEST_ಬಿಟ್, bigarray);

	ಬರೆ("big bit number (%u) test: %d, expected 0\n", BIG_TEST_ಬಿಟ್,
	       bigarray[BIG_TEST_ಬಿಟ್ >> 3]);
	ಆದರೆ (bigarray[BIG_TEST_ಬಿಟ್ >> 3] != 0)
		ನಿರ್ಗಮಿಸು(1);

	ಬರೆ("ext2fs_set_bit big_test successful\n");


	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, ಗಾತ್ರ(testarray));
	ಗಾಗಿ (i=0; ಬಿಟ್ಟುಗಳು_list[i] > 0; i++) {
		ext2fs_fast_ಗಣ_ಬಿಟ್(ಬಿಟ್ಟುಗಳು_list[i], testarray);
	}
	ಆದರೆ (memcmp(testarray, bitarray, ಗಾತ್ರ(testarray)) == 0) {
		ಬರೆ("ext2fs_fast_set_bit test succeeded.\n");
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಬರೆ("ext2fs_fast_set_bit test failed.\n");
		ಗಾಗಿ (i=0; i < ಗಾತ್ರ(testarray); i++) {
			ಬರೆ("%02x ", testarray[i]);
		}
		ಬರೆ("\n");
		ನಿರ್ಗಮಿಸು(1);
	}
	ಗಾಗಿ (i=0; ಬಿಟ್ಟುಗಳು_list[i] > 0; i++) {
		ext2fs_clear_ಬಿಟ್(ಬಿಟ್ಟುಗಳು_list[i], testarray);
	}
	ಗಾಗಿ (i=0; i < ಗಾತ್ರ(testarray); i++) {
		ಆದರೆ (testarray[i]) {
			ಬರೆ("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			ನಿರ್ಗಮಿಸು(1);
		}
	}
	ಬರೆ("ext2fs_clear_bit test succeed.\n");


        bigarray[BIG_TEST_ಬಿಟ್ >> 3] = 0;

	ext2fs_fast_ಗಣ_ಬಿಟ್(BIG_TEST_ಬಿಟ್, bigarray);
	ಬರೆ("big bit number (%u) test: %d, expected %d\n", BIG_TEST_ಬಿಟ್,
	       bigarray[BIG_TEST_ಬಿಟ್ >> 3], (1 << (BIG_TEST_ಬಿಟ್ & 7)));
	ಆದರೆ (bigarray[BIG_TEST_ಬಿಟ್ >> 3] != (1 << (BIG_TEST_ಬಿಟ್ & 7)))
		ನಿರ್ಗಮಿಸು(1);

	ext2fs_fast_clear_ಬಿಟ್(BIG_TEST_ಬಿಟ್, bigarray);

	ಬರೆ("big bit number (%u) test: %d, expected 0\n", BIG_TEST_ಬಿಟ್,
	       bigarray[BIG_TEST_ಬಿಟ್ >> 3]);
	ಆದರೆ (bigarray[BIG_TEST_ಬಿಟ್ >> 3] != 0)
		ನಿರ್ಗಮಿಸು(1);

	ಬರೆ("ext2fs_fast_set_bit big_test successful\n");

	/* Repeat foregoing tests for 64-bit bitops */

	/* Test test_bit */
	ಗಾಗಿ (i=0,j=0; i < ಗಾತ್ರ_2; i++) {
		ಆದರೆ (ext2fs_test_bit64(i, bitarray)) {
			ಆದರೆ (ಬಿಟ್ಟುಗಳು_list[j] == i) {
				j++;
			} ಇಲ್ಲದಿದ್ದರೆ {
				ಬರೆ("64-bit: Bit %d set, not expected\n",
				       i);
				ನಿರ್ಗಮಿಸು(1);
			}
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಆದರೆ (ಬಿಟ್ಟುಗಳು_list[j] == i) {
				ಬರೆ("64-bit: "
				       "Expected bit %d to be clear.\n", i);
				ನಿರ್ಗಮಿಸು(1);
			}
		}
	}
	ಬರೆ("64-bit: ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, ಗಾತ್ರ(testarray));
	ಗಾಗಿ (i=0; ಬಿಟ್ಟುಗಳು_list[i] > 0; i++) {
		ext2fs_ಗಣ_bit64(ಬಿಟ್ಟುಗಳು_list[i], testarray);
	}
	ಆದರೆ (memcmp(testarray, bitarray, ಗಾತ್ರ(testarray)) == 0) {
		ಬರೆ("64-bit: ext2fs_set_bit test succeeded.\n");
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಬರೆ("64-bit: ext2fs_set_bit test failed.\n");
		ಗಾಗಿ (i=0; i < ಗಾತ್ರ(testarray); i++) {
			ಬರೆ("%02x ", testarray[i]);
		}
		ಬರೆ("\n");
		ನಿರ್ಗಮಿಸು(1);
	}
	ಗಾಗಿ (i=0; ಬಿಟ್ಟುಗಳು_list[i] > 0; i++) {
		ext2fs_clear_bit64(ಬಿಟ್ಟುಗಳು_list[i], testarray);
	}
	ಗಾಗಿ (i=0; i < ಗಾತ್ರ(testarray); i++) {
		ಆದರೆ (testarray[i]) {
			ಬರೆ("64-bit: ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			ನಿರ್ಗಮಿಸು(1);
		}
	}
	ಬರೆ("64-bit: ext2fs_clear_bit test succeed.\n");

	/* Do bigarray test */
        bigarray[BIG_TEST_ಬಿಟ್ >> 3] = 0;

	ext2fs_ಗಣ_bit64(BIG_TEST_ಬಿಟ್, bigarray);
	ಬರೆ("64-bit: big bit number (%u) test: %d, expected %d\n",
	       BIG_TEST_ಬಿಟ್, bigarray[BIG_TEST_ಬಿಟ್ >> 3],
	       (1 << (BIG_TEST_ಬಿಟ್ & 7)));
	ಆದರೆ (bigarray[BIG_TEST_ಬಿಟ್ >> 3] != (1 << (BIG_TEST_ಬಿಟ್ & 7)))
		ನಿರ್ಗಮಿಸು(1);

	ext2fs_clear_bit64(BIG_TEST_ಬಿಟ್, bigarray);

	ಬರೆ("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_ಬಿಟ್,
	       bigarray[BIG_TEST_ಬಿಟ್ >> 3]);
	ಆದರೆ (bigarray[BIG_TEST_ಬಿಟ್ >> 3] != 0)
		ನಿರ್ಗಮಿಸು(1);

	ಬರೆ("64-bit: ext2fs_set_bit big_test successful\n");

	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, ಗಾತ್ರ(testarray));
	ಗಾಗಿ (i=0; ಬಿಟ್ಟುಗಳು_list[i] > 0; i++) {
		ext2fs_fast_ಗಣ_bit64(ಬಿಟ್ಟುಗಳು_list[i], testarray);
	}
	ಆದರೆ (memcmp(testarray, bitarray, ಗಾತ್ರ(testarray)) == 0) {
		ಬರೆ("64-bit: ext2fs_fast_set_bit test succeeded.\n");
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಬರೆ("64-bit: ext2fs_fast_set_bit test failed.\n");
		ಗಾಗಿ (i=0; i < ಗಾತ್ರ(testarray); i++) {
			ಬರೆ("%02x ", testarray[i]);
		}
		ಬರೆ("\n");
		ನಿರ್ಗಮಿಸು(1);
	}
	ಗಾಗಿ (i=0; ಬಿಟ್ಟುಗಳು_list[i] > 0; i++) {
		ext2fs_clear_bit64(ಬಿಟ್ಟುಗಳು_list[i], testarray);
	}
	ಗಾಗಿ (i=0; i < ಗಾತ್ರ(testarray); i++) {
		ಆದರೆ (testarray[i]) {
			ಬರೆ("64-bit: ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			ನಿರ್ಗಮಿಸು(1);
		}
	}
	ಬರೆ("64-bit: ext2fs_clear_bit test succeed.\n");

        bigarray[BIG_TEST_ಬಿಟ್ >> 3] = 0;

	ext2fs_fast_ಗಣ_bit64(BIG_TEST_ಬಿಟ್, bigarray);
	ಬರೆ("64-bit: big bit number (%u) test: %d, expected %d\n",
	       BIG_TEST_ಬಿಟ್, bigarray[BIG_TEST_ಬಿಟ್ >> 3],
	       (1 << (BIG_TEST_ಬಿಟ್ & 7)));
	ಆದರೆ (bigarray[BIG_TEST_ಬಿಟ್ >> 3] != (1 << (BIG_TEST_ಬಿಟ್ & 7)))
		ನಿರ್ಗಮಿಸು(1);

	ext2fs_fast_clear_bit64(BIG_TEST_ಬಿಟ್, bigarray);

	ಬರೆ("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_ಬಿಟ್, bigarray[BIG_TEST_ಬಿಟ್ >> 3]);
	ಆದರೆ (bigarray[BIG_TEST_ಬಿಟ್ >> 3] != 0)
		ನಿರ್ಗಮಿಸು(1);

	ಬರೆ("64-bit: ext2fs_fast_set_bit big_test successful\n");
	ಬಿಡುಗಡೆ(bigarray);
	ನಿರ್ಗಮಿಸು(0);
}
