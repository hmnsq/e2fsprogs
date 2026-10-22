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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <time.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <sys/types.h>
#എങ്കിൽ HAVE_ERRNO_H
#അന്തര്ഭാവ <errno.h>
#endif
#അന്തര്ഭാവ <sys/time.h>
#അന്തര്ഭാവ <sys/resource.h>

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

unsigned അക്ഷരം bitarray[] = {
	0x80, 0xF0, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x10, 0x20, 0x00, 0x00
	};

പൂർണ്ണസംഖ്യ bits_list[] = {
	7, 12, 13, 14,15, 22, 30, 68, 77, -1,
};

#define BIG_TEST_BIT   (((unsigned) 1 << 31) + 42)


പൂർണ്ണസംഖ്യ പ്രധാന(പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
	പൂർണ്ണസംഖ്യ	i, j, size;
	unsigned അക്ഷരം testarray[12];
	unsigned അക്ഷരം *bigarray;

	size = വലുപ്പം(bitarray)*8;
#എങ്കിൽ 0
	i = ext2fs_find_first_bit_set(bitarray, size);
	ആകുമ്പോൾ (i < size) {
		printf("Bit set: %d\n", i);
		i = ext2fs_find_next_bit_set(bitarray, size, i+1);
	}
#endif

	/* Test test_bit */
	വേണ്ടി (i=0,j=0; i < size; i++) {
		എങ്കിൽ (ext2fs_test_bit(i, bitarray)) {
			എങ്കിൽ (bits_list[j] == i) {
				j++;
			} അല്ലെങ്കിൽ {
				printf("Bit %d set, not expected\n", i);
				exit(1);
			}
		} അല്ലെങ്കിൽ {
			എങ്കിൽ (bits_list[j] == i) {
				printf("Expected bit %d to be clear.\n", i);
				exit(1);
			}
		}
	}
	printf("ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, വലുപ്പം(testarray));
	വേണ്ടി (i=0; bits_list[i] > 0; i++) {
		ext2fs_set_bit(bits_list[i], testarray);
	}
	എങ്കിൽ (memcmp(testarray, bitarray, വലുപ്പം(testarray)) == 0) {
		printf("ext2fs_set_bit test succeeded.\n");
	} അല്ലെങ്കിൽ {
		printf("ext2fs_set_bit test failed.\n");
		വേണ്ടി (i=0; i < വലുപ്പം(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	വേണ്ടി (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit(bits_list[i], testarray);
	}
	വേണ്ടി (i=0; i < വലുപ്പം(testarray); i++) {
		എങ്കിൽ (testarray[i]) {
			printf("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			exit(1);
		}
	}
	printf("ext2fs_clear_bit test succeed.\n");


	/* Do bigarray test */
	bigarray = malloc(1 << 29);
	എങ്കിൽ (!bigarray) {
		fprintf(stderr, "Failed to allocate scratch memory!\n");
		exit(0);
	}

        bigarray[BIG_TEST_BIT >> 3] = 0;

	ext2fs_set_bit(BIG_TEST_BIT, bigarray);
	printf("big bit number (%u) test: %d, expected %d\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3], (1 << (BIG_TEST_BIT & 7)));
	എങ്കിൽ (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_clear_bit(BIG_TEST_BIT, bigarray);

	printf("big bit number (%u) test: %d, expected 0\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	എങ്കിൽ (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("ext2fs_set_bit big_test successful\n");


	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, വലുപ്പം(testarray));
	വേണ്ടി (i=0; bits_list[i] > 0; i++) {
		ext2fs_fast_set_bit(bits_list[i], testarray);
	}
	എങ്കിൽ (memcmp(testarray, bitarray, വലുപ്പം(testarray)) == 0) {
		printf("ext2fs_fast_set_bit test succeeded.\n");
	} അല്ലെങ്കിൽ {
		printf("ext2fs_fast_set_bit test failed.\n");
		വേണ്ടി (i=0; i < വലുപ്പം(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	വേണ്ടി (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit(bits_list[i], testarray);
	}
	വേണ്ടി (i=0; i < വലുപ്പം(testarray); i++) {
		എങ്കിൽ (testarray[i]) {
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
	എങ്കിൽ (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_fast_clear_bit(BIG_TEST_BIT, bigarray);

	printf("big bit number (%u) test: %d, expected 0\n", BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	എങ്കിൽ (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("ext2fs_fast_set_bit big_test successful\n");

	/* Repeat foregoing tests for 64-bit bitops */

	/* Test test_bit */
	വേണ്ടി (i=0,j=0; i < size; i++) {
		എങ്കിൽ (ext2fs_test_bit64(i, bitarray)) {
			എങ്കിൽ (bits_list[j] == i) {
				j++;
			} അല്ലെങ്കിൽ {
				printf("64-bit: Bit %d set, not expected\n",
				       i);
				exit(1);
			}
		} അല്ലെങ്കിൽ {
			എങ്കിൽ (bits_list[j] == i) {
				printf("64-bit: "
				       "Expected bit %d to be clear.\n", i);
				exit(1);
			}
		}
	}
	printf("64-bit: ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, വലുപ്പം(testarray));
	വേണ്ടി (i=0; bits_list[i] > 0; i++) {
		ext2fs_set_bit64(bits_list[i], testarray);
	}
	എങ്കിൽ (memcmp(testarray, bitarray, വലുപ്പം(testarray)) == 0) {
		printf("64-bit: ext2fs_set_bit test succeeded.\n");
	} അല്ലെങ്കിൽ {
		printf("64-bit: ext2fs_set_bit test failed.\n");
		വേണ്ടി (i=0; i < വലുപ്പം(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	വേണ്ടി (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit64(bits_list[i], testarray);
	}
	വേണ്ടി (i=0; i < വലുപ്പം(testarray); i++) {
		എങ്കിൽ (testarray[i]) {
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
	എങ്കിൽ (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_clear_bit64(BIG_TEST_BIT, bigarray);

	printf("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_BIT,
	       bigarray[BIG_TEST_BIT >> 3]);
	എങ്കിൽ (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("64-bit: ext2fs_set_bit big_test successful\n");

	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, വലുപ്പം(testarray));
	വേണ്ടി (i=0; bits_list[i] > 0; i++) {
		ext2fs_fast_set_bit64(bits_list[i], testarray);
	}
	എങ്കിൽ (memcmp(testarray, bitarray, വലുപ്പം(testarray)) == 0) {
		printf("64-bit: ext2fs_fast_set_bit test succeeded.\n");
	} അല്ലെങ്കിൽ {
		printf("64-bit: ext2fs_fast_set_bit test failed.\n");
		വേണ്ടി (i=0; i < വലുപ്പം(testarray); i++) {
			printf("%02x ", testarray[i]);
		}
		printf("\n");
		exit(1);
	}
	വേണ്ടി (i=0; bits_list[i] > 0; i++) {
		ext2fs_clear_bit64(bits_list[i], testarray);
	}
	വേണ്ടി (i=0; i < വലുപ്പം(testarray); i++) {
		എങ്കിൽ (testarray[i]) {
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
	എങ്കിൽ (bigarray[BIG_TEST_BIT >> 3] != (1 << (BIG_TEST_BIT & 7)))
		exit(1);

	ext2fs_fast_clear_bit64(BIG_TEST_BIT, bigarray);

	printf("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_BIT, bigarray[BIG_TEST_BIT >> 3]);
	എങ്കിൽ (bigarray[BIG_TEST_BIT >> 3] != 0)
		exit(1);

	printf("64-bit: ext2fs_fast_set_bit big_test successful\n");
	സ്വതന്ത്രം(bigarray);
	exit(0);
}
