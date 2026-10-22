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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <sys/types.h>
#எனில் HAVE_ERRNO_H
#அந்தர்பாவ <errno.h>
#endif
#அந்தர்பாவ <sys/time.h>
#அந்தர்பாவ <sys/resource.h>

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

unsigned எழுத்து bitarray[] = {
	0x80, 0xF0, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x10, 0x20, 0x00, 0x00
	};

முழு bits_list[] = {
	7, 12, 13, 14,15, 22, 30, 68, 77, -1,
};

#define BIG_TEST_BIT   (((unsigned) 1 << 31) + 42)


முழு முதன்மை(முழு argc, எழுத்து **argv)
{
	முழு	i, j, size;
	unsigned எழுத்து testarray[12];
	unsigned எழுத்து *bigarray;

	size = அளவு(bitarray)*8;
#எனில் 0
	i = ext2fs_find_first_bit_set(bitarray, size);
	போது (i < size) {
		printf("Bit set: %d\n", i);
		i = ext2fs_find_next_bit_set(bitarray, size, i+1);
	}
#endif

	/* Test test_bit */
	க்கு (i=0,j=0; i < size; i++) {
		எனில் (ext2fs_test_bit(i, bitarray)) {
			எனில் (bits_list[j] == i) {
				j++;
			} இல்லையெனில் {
				printf("Bit %d set, not expected\n", i);
				exit(1);
			}
		} இல்லையெனில் {
			எனில் (bits_list[j] == i) {
				printf("Expected bit %d to be clear.\n", i);
				exit(1);
			}
		}
	}
	printf("ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, அளவு(testarray));
	க்கு (i=0; bits_list[i] > 0; i++) {
		ext2fs_set_bit(bits_list[i], testarray);
	}
	எனில் (memcmp(testarray, bitarray, அளவு(testarray)) == 0) {
		printf("ext2fs_set_bit test succeeded.\n");
	} இல்லையெனில் {
		printf("ext2fs_set_bit test failed.\n");
		க்கு (i=0; i < அளவு(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	க்கு (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit(bits_list[i], testarray);
	}
	க்கு (i=0; i < அளவு(testarray); i++) {
		எனில் (testarray[i]) {
			printf("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			exit(1);
		}
	}
	printf("ext2fs_clear_bit test succeed.\n");


	/* Do bigarray test */
	bigarray = malloc(1 << 29);
	எனில் (!bigarray) {
		fprintf(stderr, "Failed to allocate scratch memory!\n");
		exit(0);
	}

        bigarray[BIG_TEST_BIT >> 3] = 0;

	ext2fs_set_bit(BIG_TEST_BIT, bigarray);
	printf("big bit number (%u) test: %d, expected %d\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3], (1 << (BIG_TEST_BIT & 7)));
	எனில் (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_clear_bit(BIG_TEST_BIT, bigarray);

	printf("big bit number (%u) test: %d, expected 0\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	எனில் (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("ext2fs_set_bit big_test successful\n");


	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, அளவு(testarray));
	க்கு (i=0; bits_list[i] > 0; i++) {
		ext2fs_fast_set_bit(bits_list[i], testarray);
	}
	எனில் (memcmp(testarray, bitarray, அளவு(testarray)) == 0) {
		printf("ext2fs_fast_set_bit test succeeded.\n");
	} இல்லையெனில் {
		printf("ext2fs_fast_set_bit test failed.\n");
		க்கு (i=0; i < அளவு(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	க்கு (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit(bits_list[i], testarray);
	}
	க்கு (i=0; i < அளவு(testarray); i++) {
		எனில் (testarray[i]) {
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
	எனில் (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_fast_clear_bit(BIG_TEST_BIT, bigarray);

	printf("big bit number (%u) test: %d, expected 0\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	எனில் (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("ext2fs_fast_set_bit big_test successful\n");

	/* Repeat foregoing tests for 64-bit bitops */

	/* Test test_bit */
	க்கு (i=0,j=0; i < size; i++) {
		எனில் (ext2fs_test_bit64(i, bitarray)) {
			எனில் (bits_list[j] == i) {
				j++;
			} இல்லையெனில் {
				printf("64-bit: Bit %d set, not expected\n",
				       i);
				exit(1);
			}
		} இல்லையெனில் {
			எனில் (bits_list[j] == i) {
				printf("64-bit: "
				       "Expected bit %d to be clear.\n", i);
				exit(1);
			}
		}
	}
	printf("64-bit: ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, அளவு(testarray));
	க்கு (i=0; bits_list[i] > 0; i++) {
		ext2fs_set_bit64(bits_list[i], testarray);
	}
	எனில் (memcmp(testarray, bitarray, அளவு(testarray)) == 0) {
		printf("64-bit: ext2fs_set_bit test succeeded.\n");
	} இல்லையெனில் {
		printf("64-bit: ext2fs_set_bit test failed.\n");
		க்கு (i=0; i < அளவு(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	க்கு (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit64(bits_list[i], testarray);
	}
	க்கு (i=0; i < அளவு(testarray); i++) {
		எனில் (testarray[i]) {
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
	எனில் (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_clear_bit64(BIG_TEST_BIT, bigarray);

	printf("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	எனில் (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("64-bit: ext2fs_set_bit big_test successful\n");

	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, அளவு(testarray));
	க்கு (i=0; bits_list[i] > 0; i++) {
		ext2fs_fast_set_bit64(bits_list[i], testarray);
	}
	எனில் (memcmp(testarray, bitarray, அளவு(testarray)) == 0) {
		printf("64-bit: ext2fs_fast_set_bit test succeeded.\n");
	} இல்லையெனில் {
		printf("64-bit: ext2fs_fast_set_bit test failed.\n");
		க்கு (i=0; i < அளவு(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	க்கு (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit64(bits_list[i], testarray);
	}
	க்கு (i=0; i < அளவு(testarray); i++) {
		எனில் (testarray[i]) {
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
	எனில் (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_fast_clear_bit64(BIG_TEST_BIT, bigarray);

	printf("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_BIT, bigarray[BIG_TEST_BIT >> 3]);
	எனில் (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("64-bit: ext2fs_fast_set_bit big_test successful\n");
	விடுவி(bigarray);
	exit(0);
}
