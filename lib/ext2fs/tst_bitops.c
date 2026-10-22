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
#endif
#شامل <fcntl.h>
#شامل <time.h>
#شامل <sys/stat.h>
#شامل <sys/types.h>
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#endif
#شامل <sys/time.h>
#شامل <sys/resource.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

unsigned نویسه bitarray[] = {
	0x80, 0xF0, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x10, 0x20, 0x00, 0x00
	};

صحیح bits_list[] = {
	7, 12, 13, 14,15, 22, 30, 68, 77, -1,
};

#define BIG_TEST_BIT   (((unsigned) 1 << 31) + 42)


صحیح اصلی(صحیح argc, نویسه **argv)
{
	صحیح	i, j, size;
	unsigned نویسه testarray[12];
	unsigned نویسه *bigarray;

	size = اندازه(bitarray)*8;
#اگر 0
	i = ext2fs_find_first_bit_set(bitarray, size);
	وقتی (i < size) {
		printf("Bit set: %d\n", i);
		i = ext2fs_find_next_bit_set(bitarray, size, i+1);
	}
#endif

	/* Test test_bit */
	برای (i=0,j=0; i < size; i++) {
		اگر (ext2fs_test_bit(i, bitarray)) {
			اگر (bits_list[j] == i) {
				j++;
			} وگرنه {
				printf("Bit %d set, not expected\n", i);
				exit(1);
			}
		} وگرنه {
			اگر (bits_list[j] == i) {
				printf("Expected bit %d to be clear.\n", i);
				exit(1);
			}
		}
	}
	printf("ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, اندازه(testarray));
	برای (i=0; bits_list[i] > 0; i++) {
		ext2fs_set_bit(bits_list[i], testarray);
	}
	اگر (memcmp(testarray, bitarray, اندازه(testarray)) == 0) {
		printf("ext2fs_set_bit test succeeded.\n");
	} وگرنه {
		printf("ext2fs_set_bit test failed.\n");
		برای (i=0; i < اندازه(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	برای (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit(bits_list[i], testarray);
	}
	برای (i=0; i < اندازه(testarray); i++) {
		اگر (testarray[i]) {
			printf("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			exit(1);
		}
	}
	printf("ext2fs_clear_bit test succeed.\n");


	/* Do bigarray test */
	bigarray = malloc(1 << 29);
	اگر (!bigarray) {
		fprintf(stderr, "Failed to allocate scratch memory!\n");
		exit(0);
	}

        bigarray[BIG_TEST_BIT >> 3] = 0;

	ext2fs_set_bit(BIG_TEST_BIT, bigarray);
	printf("big bit number (%u) test: %d, expected %d\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3], (1 << (BIG_TEST_BIT & 7)));
	اگر (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_clear_bit(BIG_TEST_BIT, bigarray);

	printf("big bit number (%u) test: %d, expected 0\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	اگر (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("ext2fs_set_bit big_test successful\n");


	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, اندازه(testarray));
	برای (i=0; bits_list[i] > 0; i++) {
		ext2fs_fast_set_bit(bits_list[i], testarray);
	}
	اگر (memcmp(testarray, bitarray, اندازه(testarray)) == 0) {
		printf("ext2fs_fast_set_bit test succeeded.\n");
	} وگرنه {
		printf("ext2fs_fast_set_bit test failed.\n");
		برای (i=0; i < اندازه(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	برای (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit(bits_list[i], testarray);
	}
	برای (i=0; i < اندازه(testarray); i++) {
		اگر (testarray[i]) {
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
	اگر (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_fast_clear_bit(BIG_TEST_BIT, bigarray);

	printf("big bit number (%u) test: %d, expected 0\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	اگر (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("ext2fs_fast_set_bit big_test successful\n");

	/* Repeat foregoing tests for 64-bit bitops */

	/* Test test_bit */
	برای (i=0,j=0; i < size; i++) {
		اگر (ext2fs_test_bit64(i, bitarray)) {
			اگر (bits_list[j] == i) {
				j++;
			} وگرنه {
				printf("64-bit: Bit %d set, not expected\n",
				       i);
				exit(1);
			}
		} وگرنه {
			اگر (bits_list[j] == i) {
				printf("64-bit: "
				       "Expected bit %d to be clear.\n", i);
				exit(1);
			}
		}
	}
	printf("64-bit: ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, اندازه(testarray));
	برای (i=0; bits_list[i] > 0; i++) {
		ext2fs_set_bit64(bits_list[i], testarray);
	}
	اگر (memcmp(testarray, bitarray, اندازه(testarray)) == 0) {
		printf("64-bit: ext2fs_set_bit test succeeded.\n");
	} وگرنه {
		printf("64-bit: ext2fs_set_bit test failed.\n");
		برای (i=0; i < اندازه(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	برای (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit64(bits_list[i], testarray);
	}
	برای (i=0; i < اندازه(testarray); i++) {
		اگر (testarray[i]) {
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
	اگر (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_clear_bit64(BIG_TEST_BIT, bigarray);

	printf("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	اگر (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("64-bit: ext2fs_set_bit big_test successful\n");

	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, اندازه(testarray));
	برای (i=0; bits_list[i] > 0; i++) {
		ext2fs_fast_set_bit64(bits_list[i], testarray);
	}
	اگر (memcmp(testarray, bitarray, اندازه(testarray)) == 0) {
		printf("64-bit: ext2fs_fast_set_bit test succeeded.\n");
	} وگرنه {
		printf("64-bit: ext2fs_fast_set_bit test failed.\n");
		برای (i=0; i < اندازه(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	برای (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit64(bits_list[i], testarray);
	}
	برای (i=0; i < اندازه(testarray); i++) {
		اگر (testarray[i]) {
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
	اگر (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_fast_clear_bit64(BIG_TEST_BIT, bigarray);

	printf("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_BIT, bigarray[BIG_TEST_BIT >> 3]);
	اگر (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("64-bit: ext2fs_fast_set_bit big_test successful\n");
	آزاد(bigarray);
	exit(0);
}
