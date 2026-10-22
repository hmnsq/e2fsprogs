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

#include "config.h"
#include <stdio.h>
#include <string.h>
#ប្រសិនបើ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#ប្រសិនបើ HAVE_ERRNO_H
#include <errno.h>
#endif
#include <sys/time.h>
#include <sys/resource.h>

#include "ext2_fs.h"
#include "ext2fs.h"

unsigned var_na bitarray[] = {
	0x80, 0xF0, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x10, 0x20, 0x00, 0x00
	};

ចំនួនគត់ bits_list[] = {
	7, 12, 13, 14,15, 22, 30, 68, 77, -1,
};

#define BIG_TEST_BIT   (((unsigned) 1 << 31) + 42)


ចំនួនគត់ main(ចំនួនគត់ argc, var_na **argv)
{
	ចំនួនគត់	i, j, size;
	unsigned var_na testarray[12];
	unsigned var_na *bigarray;

	size = parimaa_na(bitarray)*8;
#ប្រសិនបើ 0
	i = ext2fs_find_first_bit_set(bitarray, size);
	ខណៈពេល (i < size) {
		printf("Bit set: %d\n", i);
		i = ext2fs_find_next_bit_set(bitarray, size, i+1);
	}
#endif

	/* Test test_bit */
	សម្រាប់ (i=0,j=0; i < size; i++) {
		ប្រសិនបើ (ext2fs_test_bit(i, bitarray)) {
			ប្រសិនបើ (bits_list[j] == i) {
				j++;
			} បើមិនដូច្នេះ {
				printf("Bit %d set, not expected\n", i);
				exit(1);
			}
		} បើមិនដូច្នេះ {
			ប្រសិនបើ (bits_list[j] == i) {
				printf("Expected bit %d to be clear.\n", i);
				exit(1);
			}
		}
	}
	printf("ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, parimaa_na(testarray));
	សម្រាប់ (i=0; bits_list[i] > 0; i++) {
		ext2fs_set_bit(bits_list[i], testarray);
	}
	ប្រសិនបើ (memcmp(testarray, bitarray, parimaa_na(testarray)) == 0) {
		printf("ext2fs_set_bit test succeeded.\n");
	} បើមិនដូច្នេះ {
		printf("ext2fs_set_bit test failed.\n");
		សម្រាប់ (i=0; i < parimaa_na(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	សម្រាប់ (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit(bits_list[i], testarray);
	}
	សម្រាប់ (i=0; i < parimaa_na(testarray); i++) {
		ប្រសិនបើ (testarray[i]) {
			printf("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			exit(1);
		}
	}
	printf("ext2fs_clear_bit test succeed.\n");


	/* Do bigarray test */
	bigarray = malloc(1 << 29);
	ប្រសិនបើ (!bigarray) {
		fprintf(stderr, "Failed to allocate scratch memory!\n");
		exit(0);
	}

        bigarray[BIG_TEST_BIT >> 3] = 0;

	ext2fs_set_bit(BIG_TEST_BIT, bigarray);
	printf("big bit number (%u) test: %d, expected %d\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3], (1 << (BIG_TEST_BIT & 7)));
	ប្រសិនបើ (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_clear_bit(BIG_TEST_BIT, bigarray);

	printf("big bit number (%u) test: %d, expected 0\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	ប្រសិនបើ (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("ext2fs_set_bit big_test successful\n");


	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, parimaa_na(testarray));
	សម្រាប់ (i=0; bits_list[i] > 0; i++) {
		ext2fs_fast_set_bit(bits_list[i], testarray);
	}
	ប្រសិនបើ (memcmp(testarray, bitarray, parimaa_na(testarray)) == 0) {
		printf("ext2fs_fast_set_bit test succeeded.\n");
	} បើមិនដូច្នេះ {
		printf("ext2fs_fast_set_bit test failed.\n");
		សម្រាប់ (i=0; i < parimaa_na(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	សម្រាប់ (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit(bits_list[i], testarray);
	}
	សម្រាប់ (i=0; i < parimaa_na(testarray); i++) {
		ប្រសិនបើ (testarray[i]) {
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
	ប្រសិនបើ (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_fast_clear_bit(BIG_TEST_BIT, bigarray);

	printf("big bit number (%u) test: %d, expected 0\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	ប្រសិនបើ (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("ext2fs_fast_set_bit big_test successful\n");

	/* Repeat foregoing tests for 64-bit bitops */

	/* Test test_bit */
	សម្រាប់ (i=0,j=0; i < size; i++) {
		ប្រសិនបើ (ext2fs_test_bit64(i, bitarray)) {
			ប្រសិនបើ (bits_list[j] == i) {
				j++;
			} បើមិនដូច្នេះ {
				printf("64-bit: Bit %d set, not expected\n",
				       i);
				exit(1);
			}
		} បើមិនដូច្នេះ {
			ប្រសិនបើ (bits_list[j] == i) {
				printf("64-bit: "
				       "Expected bit %d to be clear.\n", i);
				exit(1);
			}
		}
	}
	printf("64-bit: ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, parimaa_na(testarray));
	សម្រាប់ (i=0; bits_list[i] > 0; i++) {
		ext2fs_set_bit64(bits_list[i], testarray);
	}
	ប្រសិនបើ (memcmp(testarray, bitarray, parimaa_na(testarray)) == 0) {
		printf("64-bit: ext2fs_set_bit test succeeded.\n");
	} បើមិនដូច្នេះ {
		printf("64-bit: ext2fs_set_bit test failed.\n");
		សម្រាប់ (i=0; i < parimaa_na(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	សម្រាប់ (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit64(bits_list[i], testarray);
	}
	សម្រាប់ (i=0; i < parimaa_na(testarray); i++) {
		ប្រសិនបើ (testarray[i]) {
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
	ប្រសិនបើ (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_clear_bit64(BIG_TEST_BIT, bigarray);

	printf("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	ប្រសិនបើ (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("64-bit: ext2fs_set_bit big_test successful\n");

	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, parimaa_na(testarray));
	សម្រាប់ (i=0; bits_list[i] > 0; i++) {
		ext2fs_fast_set_bit64(bits_list[i], testarray);
	}
	ប្រសិនបើ (memcmp(testarray, bitarray, parimaa_na(testarray)) == 0) {
		printf("64-bit: ext2fs_fast_set_bit test succeeded.\n");
	} បើមិនដូច្នេះ {
		printf("64-bit: ext2fs_fast_set_bit test failed.\n");
		សម្រាប់ (i=0; i < parimaa_na(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	សម្រាប់ (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit64(bits_list[i], testarray);
	}
	សម្រាប់ (i=0; i < parimaa_na(testarray); i++) {
		ប្រសិនបើ (testarray[i]) {
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
	ប្រសិនបើ (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_fast_clear_bit64(BIG_TEST_BIT, bigarray);

	printf("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_BIT, bigarray[BIG_TEST_BIT >> 3]);
	ប្រសិនបើ (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("64-bit: ext2fs_fast_set_bit big_test successful\n");
	free(bigarray);
	exit(0);
}
