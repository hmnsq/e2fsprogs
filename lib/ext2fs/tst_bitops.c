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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <time.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <sys/types.h>
#যদি HAVE_ERRNO_H
#অন্তর্ভাব <errno.h>
#endif
#অন্তর্ভাব <sys/time.h>
#অন্তর্ভাব <sys/resource.h>

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"

unsigned অক্ষর bitarray[] = {
	0x80, 0xF0, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x10, 0x20, 0x00, 0x00
	};

পূর্ণসংখ্যা bits_list[] = {
	7, 12, 13, 14,15, 22, 30, 68, 77, -1,
};

#define BIG_TEST_BIT   (((unsigned) 1 << 31) + 42)


পূর্ণসংখ্যা প্রধান(পূর্ণসংখ্যা argc, অক্ষর **argv)
{
	পূর্ণসংখ্যা	i, j, আকার_2;
	unsigned অক্ষর testarray[12];
	unsigned অক্ষর *bigarray;

	আকার_2 = আকার(bitarray)*8;
#যদি 0
	i = ext2fs_find_first_bit_set(bitarray, আকার_2);
	যখন (i < আকার_2) {
		printf("Bit set: %d\n", i);
		i = ext2fs_find_পরবর্তী_bit_set(bitarray, আকার_2, i+1);
	}
#endif

	/* Test test_bit */
	জন্য (i=0,j=0; i < আকার_2; i++) {
		যদি (ext2fs_test_bit(i, bitarray)) {
			যদি (bits_list[j] == i) {
				j++;
			} অন্যথা {
				printf("Bit %d set, not expected\n", i);
				exit(1);
			}
		} অন্যথা {
			যদি (bits_list[j] == i) {
				printf("Expected bit %d to be clear.\n", i);
				exit(1);
			}
		}
	}
	printf("ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, আকার(testarray));
	জন্য (i=0; bits_list[i] > 0; i++) {
		ext2fs_set_bit(bits_list[i], testarray);
	}
	যদি (memcmp(testarray, bitarray, আকার(testarray)) == 0) {
		printf("ext2fs_set_bit test succeeded.\n");
	} অন্যথা {
		printf("ext2fs_set_bit test failed.\n");
		জন্য (i=0; i < আকার(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	জন্য (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit(bits_list[i], testarray);
	}
	জন্য (i=0; i < আকার(testarray); i++) {
		যদি (testarray[i]) {
			printf("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			exit(1);
		}
	}
	printf("ext2fs_clear_bit test succeed.\n");


	/* Do bigarray test */
	bigarray = malloc(1 << 29);
	যদি (!bigarray) {
		fprintf(stderr, "Failed to allocate scratch memory!\n");
		exit(0);
	}

        bigarray[BIG_TEST_BIT >> 3] = 0;

	ext2fs_set_bit(BIG_TEST_BIT, bigarray);
	printf("big bit number (%u) test: %d, expected %d\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3], (1 << (BIG_TEST_BIT & 7)));
	যদি (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_clear_bit(BIG_TEST_BIT, bigarray);

	printf("big bit number (%u) test: %d, expected 0\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	যদি (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("ext2fs_set_bit big_test successful\n");


	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, আকার(testarray));
	জন্য (i=0; bits_list[i] > 0; i++) {
		ext2fs_fast_set_bit(bits_list[i], testarray);
	}
	যদি (memcmp(testarray, bitarray, আকার(testarray)) == 0) {
		printf("ext2fs_fast_set_bit test succeeded.\n");
	} অন্যথা {
		printf("ext2fs_fast_set_bit test failed.\n");
		জন্য (i=0; i < আকার(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	জন্য (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit(bits_list[i], testarray);
	}
	জন্য (i=0; i < আকার(testarray); i++) {
		যদি (testarray[i]) {
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
	যদি (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_fast_clear_bit(BIG_TEST_BIT, bigarray);

	printf("big bit number (%u) test: %d, expected 0\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	যদি (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("ext2fs_fast_set_bit big_test successful\n");

	/* Repeat foregoing tests for 64-bit bitops */

	/* Test test_bit */
	জন্য (i=0,j=0; i < আকার_2; i++) {
		যদি (ext2fs_test_bit64(i, bitarray)) {
			যদি (bits_list[j] == i) {
				j++;
			} অন্যথা {
				printf("64-bit: Bit %d set, not expected\n",
				       i);
				exit(1);
			}
		} অন্যথা {
			যদি (bits_list[j] == i) {
				printf("64-bit: "
				       "Expected bit %d to be clear.\n", i);
				exit(1);
			}
		}
	}
	printf("64-bit: ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, আকার(testarray));
	জন্য (i=0; bits_list[i] > 0; i++) {
		ext2fs_set_bit64(bits_list[i], testarray);
	}
	যদি (memcmp(testarray, bitarray, আকার(testarray)) == 0) {
		printf("64-bit: ext2fs_set_bit test succeeded.\n");
	} অন্যথা {
		printf("64-bit: ext2fs_set_bit test failed.\n");
		জন্য (i=0; i < আকার(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	জন্য (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit64(bits_list[i], testarray);
	}
	জন্য (i=0; i < আকার(testarray); i++) {
		যদি (testarray[i]) {
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
	যদি (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_clear_bit64(BIG_TEST_BIT, bigarray);

	printf("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	যদি (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("64-bit: ext2fs_set_bit big_test successful\n");

	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, আকার(testarray));
	জন্য (i=0; bits_list[i] > 0; i++) {
		ext2fs_fast_set_bit64(bits_list[i], testarray);
	}
	যদি (memcmp(testarray, bitarray, আকার(testarray)) == 0) {
		printf("64-bit: ext2fs_fast_set_bit test succeeded.\n");
	} অন্যথা {
		printf("64-bit: ext2fs_fast_set_bit test failed.\n");
		জন্য (i=0; i < আকার(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	জন্য (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit64(bits_list[i], testarray);
	}
	জন্য (i=0; i < আকার(testarray); i++) {
		যদি (testarray[i]) {
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
	যদি (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_fast_clear_bit64(BIG_TEST_BIT, bigarray);

	printf("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_BIT, bigarray[BIG_TEST_BIT >> 3]);
	যদি (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("64-bit: ext2fs_fast_set_bit big_test successful\n");
	মুক্ত(bigarray);
	exit(0);
}
