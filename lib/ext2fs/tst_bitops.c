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
#ܐܢ HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#ܐܢ HAVE_ERRNO_H
#ضمن <errno.h>
#endif
#ضمن <sys/time.h>
#ضمن <sys/resource.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

unsigned حرف bitarray[] = {
	0x80, 0xF0, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x10, 0x20, 0x00, 0x00
	};

صحيح bits_list[] = {
	7, 12, 13, 14,15, 22, 30, 68, 77, -1,
};

#define BIG_TEST_BIT   (((unsigned) 1 << 31) + 42)


صحيح main(صحيح argc, حرف **argv)
{
	صحيح	i, j, size;
	unsigned حرف testarray[12];
	unsigned حرف *bigarray;

	size = حجم(bitarray)*8;
#ܐܢ 0
	i = ext2fs_find_first_bit_set(bitarray, size);
	ܟܡܐ (i < size) {
		printf("Bit set: %d\n", i);
		i = ext2fs_find_next_bit_set(bitarray, size, i+1);
	}
#endif

	/* Test test_bit */
	لكل (i=0,j=0; i < size; i++) {
		ܐܢ (ext2fs_test_bit(i, bitarray)) {
			ܐܢ (bits_list[j] == i) {
				j++;
			} ܐܚܪܢܐ {
				printf("Bit %d set, not expected\n", i);
				exit(1);
			}
		} ܐܚܪܢܐ {
			ܐܢ (bits_list[j] == i) {
				printf("Expected bit %d to be clear.\n", i);
				exit(1);
			}
		}
	}
	printf("ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, حجم(testarray));
	لكل (i=0; bits_list[i] > 0; i++) {
		ext2fs_set_bit(bits_list[i], testarray);
	}
	ܐܢ (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		printf("ext2fs_set_bit test succeeded.\n");
	} ܐܚܪܢܐ {
		printf("ext2fs_set_bit test failed.\n");
		لكل (i=0; i < حجم(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	لكل (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit(bits_list[i], testarray);
	}
	لكل (i=0; i < حجم(testarray); i++) {
		ܐܢ (testarray[i]) {
			printf("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			exit(1);
		}
	}
	printf("ext2fs_clear_bit test succeed.\n");


	/* Do bigarray test */
	bigarray = malloc(1 << 29);
	ܐܢ (!bigarray) {
		fprintf(stderr, "Failed to allocate scratch memory!\n");
		exit(0);
	}

        bigarray[BIG_TEST_BIT >> 3] = 0;

	ext2fs_set_bit(BIG_TEST_BIT, bigarray);
	printf("big bit number (%u) test: %d, expected %d\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3], (1 << (BIG_TEST_BIT & 7)));
	ܐܢ (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_clear_bit(BIG_TEST_BIT, bigarray);

	printf("big bit number (%u) test: %d, expected 0\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	ܐܢ (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("ext2fs_set_bit big_test successful\n");


	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, حجم(testarray));
	لكل (i=0; bits_list[i] > 0; i++) {
		ext2fs_fast_set_bit(bits_list[i], testarray);
	}
	ܐܢ (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		printf("ext2fs_fast_set_bit test succeeded.\n");
	} ܐܚܪܢܐ {
		printf("ext2fs_fast_set_bit test failed.\n");
		لكل (i=0; i < حجم(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	لكل (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit(bits_list[i], testarray);
	}
	لكل (i=0; i < حجم(testarray); i++) {
		ܐܢ (testarray[i]) {
			printf("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			exit(1);
		}
	}
	printf("ext2fs_clear_bit test succeed.\n");


        bigarray[BIG_TEST_BIT >> 3] = 0;

	ext2fs_fast_set_bit(BIG_TEST_BIT, bigarray);
	printf("big bit number (%u) test: %d, expected %d\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3], (1 << (BIG_TEST_BIT & 7)));
	ܐܢ (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_fast_clear_bit(BIG_TEST_BIT, bigarray);

	printf("big bit number (%u) test: %d, expected 0\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	ܐܢ (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("ext2fs_fast_set_bit big_test successful\n");

	/* Repeat foregoing tests for 64-bit bitops */

	/* Test test_bit */
	لكل (i=0,j=0; i < size; i++) {
		ܐܢ (ext2fs_test_bit64(i, bitarray)) {
			ܐܢ (bits_list[j] == i) {
				j++;
			} ܐܚܪܢܐ {
				printf("64-bit: Bit %d set, not expected\n",
				       i);
				exit(1);
			}
		} ܐܚܪܢܐ {
			ܐܢ (bits_list[j] == i) {
				printf("64-bit: "
				       "Expected bit %d to be clear.\n", i);
				exit(1);
			}
		}
	}
	printf("64-bit: ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, حجم(testarray));
	لكل (i=0; bits_list[i] > 0; i++) {
		ext2fs_set_bit64(bits_list[i], testarray);
	}
	ܐܢ (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		printf("64-bit: ext2fs_set_bit test succeeded.\n");
	} ܐܚܪܢܐ {
		printf("64-bit: ext2fs_set_bit test failed.\n");
		لكل (i=0; i < حجم(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	لكل (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit64(bits_list[i], testarray);
	}
	لكل (i=0; i < حجم(testarray); i++) {
		ܐܢ (testarray[i]) {
			printf("64-bit: ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			exit(1);
		}
	}
	printf("64-bit: ext2fs_clear_bit test succeed.\n");

	/* Do bigarray test */
        bigarray[BIG_TEST_BIT >> 3] = 0;

	ext2fs_set_bit64(BIG_TEST_BIT, bigarray);
	printf("64-bit: big bit number (%u) test: %d, expected %d\n",
	       BIG_TEST_BIT, bigarray[BIG_TEST_BIT >> 3],
	       (1 << (BIG_TEST_BIT & 7)));
	ܐܢ (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_clear_bit64(BIG_TEST_BIT, bigarray);

	printf("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	ܐܢ (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("64-bit: ext2fs_set_bit big_test successful\n");

	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, حجم(testarray));
	لكل (i=0; bits_list[i] > 0; i++) {
		ext2fs_fast_set_bit64(bits_list[i], testarray);
	}
	ܐܢ (memcmp(testarray, bitarray, حجم(testarray)) == 0) {
		printf("64-bit: ext2fs_fast_set_bit test succeeded.\n");
	} ܐܚܪܢܐ {
		printf("64-bit: ext2fs_fast_set_bit test failed.\n");
		لكل (i=0; i < حجم(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	لكل (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit64(bits_list[i], testarray);
	}
	لكل (i=0; i < حجم(testarray); i++) {
		ܐܢ (testarray[i]) {
			printf("64-bit: ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			exit(1);
		}
	}
	printf("64-bit: ext2fs_clear_bit test succeed.\n");

        bigarray[BIG_TEST_BIT >> 3] = 0;

	ext2fs_fast_set_bit64(BIG_TEST_BIT, bigarray);
	printf("64-bit: big bit number (%u) test: %d, expected %d\n",
	       BIG_TEST_BIT, bigarray[BIG_TEST_BIT >> 3],
	       (1 << (BIG_TEST_BIT & 7)));
	ܐܢ (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_fast_clear_bit64(BIG_TEST_BIT, bigarray);

	printf("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_BIT, bigarray[BIG_TEST_BIT >> 3]);
	ܐܢ (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("64-bit: ext2fs_fast_set_bit big_test successful\n");
	حرر(bigarray);
	exit(0);
}
