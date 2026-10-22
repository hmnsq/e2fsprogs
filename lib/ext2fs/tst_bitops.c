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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <sys/types.h>
#ఒకవేళ HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు
#అన్తర్భావ <sys/time.h>
#అన్తర్భావ <sys/resource.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

గుర్తులేని అక్షరం bitarray[] = {
	0x80, 0xF0, 0x40, 0x40, 0x0, 0x0, 0x0, 0x0, 0x10, 0x20, 0x00, 0x00
	};

పూర్ణాంకం బిట్లు_list[] = {
	7, 12, 13, 14,15, 22, 30, 68, 77, -1,
};

#నిర్వచించు BIG_TEST_బిట్టు   (((గుర్తులేని) 1 << 31) + 42)


పూర్ణాంకం ప్రధాన(పూర్ణాంకం argc, అక్షరం **argv)
{
	పూర్ణాంకం	i, j, కొలత;
	గుర్తులేని అక్షరం testarray[12];
	గుర్తులేని అక్షరం *bigarray;

	కొలత = పరిమాణం(bitarray)*8;
#ఒకవేళ 0
	i = ext2fs_కనుగొను_మొదటి_బిట్టు_సమితి(bitarray, కొలత);
	అయితే (i < కొలత) {
		రాయి("Bit set: %d\n", i);
		i = ext2fs_కనుగొను_తదుపరి_బిట్టు_సమితి(bitarray, కొలత, i+1);
	}
#అయితే_ముగింపు

	/* Test test_bit */
	కొరకు (i=0,j=0; i < కొలత; i++) {
		ఒకవేళ (ext2fs_test_బిట్టు(i, bitarray)) {
			ఒకవేళ (బిట్లు_list[j] == i) {
				j++;
			} లేకపోతే {
				రాయి("Bit %d set, not expected\n", i);
				నిష్క్రమించు(1);
			}
		} లేకపోతే {
			ఒకవేళ (బిట్లు_list[j] == i) {
				రాయి("Expected bit %d to be clear.\n", i);
				నిష్క్రమించు(1);
			}
		}
	}
	రాయి("ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, పరిమాణం(testarray));
	కొరకు (i=0; బిట్లు_list[i] > 0; i++) {
		ext2fs_సమితి_బిట్టు(బిట్లు_list[i], testarray);
	}
	ఒకవేళ (memcmp(testarray, bitarray, పరిమాణం(testarray)) == 0) {
		రాయి("ext2fs_set_bit test succeeded.\n");
	} లేకపోతే {
		రాయి("ext2fs_set_bit test failed.\n");
		కొరకు (i=0; i < పరిమాణం(testarray); i++) {
			రాయి("%02x ", testarray[i]);
		}
		రాయి("\n");
		నిష్క్రమించు(1);
	}
	కొరకు (i=0; బిట్లు_list[i] > 0; i++) {
		ext2fs_clear_బిట్టు(బిట్లు_list[i], testarray);
	}
	కొరకు (i=0; i < పరిమాణం(testarray); i++) {
		ఒకవేళ (testarray[i]) {
			రాయి("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			నిష్క్రమించు(1);
		}
	}
	రాయి("ext2fs_clear_bit test succeed.\n");


	/* Do bigarray test */
	bigarray = జ్ఞాపకం_ఇవ్వు(1 << 29);
	ఒకవేళ (!bigarray) {
		fprintf(stderr, "Failed to allocate scratch memory!\n");
		నిష్క్రమించు(0);
	}

        bigarray[BIG_TEST_బిట్టు >> 3] = 0;

	ext2fs_సమితి_బిట్టు(BIG_TEST_బిట్టు, bigarray);
	రాయి("big bit number (%u) test: %d, expected %d\n", BIG_TEST_బిట్టు,
	       bigarray[BIG_TEST_బిట్టు >> 3], (1 << (BIG_TEST_బిట్టు & 7)));
	ఒకవేళ (bigarray[BIG_TEST_బిట్టు >> 3] != (1 << (BIG_TEST_బిట్టు & 7)))
		నిష్క్రమించు(1);

	ext2fs_clear_బిట్టు(BIG_TEST_బిట్టు, bigarray);

	రాయి("big bit number (%u) test: %d, expected 0\n", BIG_TEST_బిట్టు,
	       bigarray[BIG_TEST_బిట్టు >> 3]);
	ఒకవేళ (bigarray[BIG_TEST_బిట్టు >> 3] != 0)
		నిష్క్రమించు(1);

	రాయి("ext2fs_set_bit big_test successful\n");


	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, పరిమాణం(testarray));
	కొరకు (i=0; బిట్లు_list[i] > 0; i++) {
		ext2fs_fast_సమితి_బిట్టు(బిట్లు_list[i], testarray);
	}
	ఒకవేళ (memcmp(testarray, bitarray, పరిమాణం(testarray)) == 0) {
		రాయి("ext2fs_fast_set_bit test succeeded.\n");
	} లేకపోతే {
		రాయి("ext2fs_fast_set_bit test failed.\n");
		కొరకు (i=0; i < పరిమాణం(testarray); i++) {
			రాయి("%02x ", testarray[i]);
		}
		రాయి("\n");
		నిష్క్రమించు(1);
	}
	కొరకు (i=0; బిట్లు_list[i] > 0; i++) {
		ext2fs_clear_బిట్టు(బిట్లు_list[i], testarray);
	}
	కొరకు (i=0; i < పరిమాణం(testarray); i++) {
		ఒకవేళ (testarray[i]) {
			రాయి("ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			నిష్క్రమించు(1);
		}
	}
	రాయి("ext2fs_clear_bit test succeed.\n");


        bigarray[BIG_TEST_బిట్టు >> 3] = 0;

	ext2fs_fast_సమితి_బిట్టు(BIG_TEST_బిట్టు, bigarray);
	రాయి("big bit number (%u) test: %d, expected %d\n", BIG_TEST_బిట్టు,
	       bigarray[BIG_TEST_బిట్టు >> 3], (1 << (BIG_TEST_బిట్టు & 7)));
	ఒకవేళ (bigarray[BIG_TEST_బిట్టు >> 3] != (1 << (BIG_TEST_బిట్టు & 7)))
		నిష్క్రమించు(1);

	ext2fs_fast_clear_బిట్టు(BIG_TEST_బిట్టు, bigarray);

	రాయి("big bit number (%u) test: %d, expected 0\n", BIG_TEST_బిట్టు,
	       bigarray[BIG_TEST_బిట్టు >> 3]);
	ఒకవేళ (bigarray[BIG_TEST_బిట్టు >> 3] != 0)
		నిష్క్రమించు(1);

	రాయి("ext2fs_fast_set_bit big_test successful\n");

	/* Repeat foregoing tests for 64-bit bitops */

	/* Test test_bit */
	కొరకు (i=0,j=0; i < కొలత; i++) {
		ఒకవేళ (ext2fs_test_bit64(i, bitarray)) {
			ఒకవేళ (బిట్లు_list[j] == i) {
				j++;
			} లేకపోతే {
				రాయి("64-bit: Bit %d set, not expected\n",
				       i);
				నిష్క్రమించు(1);
			}
		} లేకపోతే {
			ఒకవేళ (బిట్లు_list[j] == i) {
				రాయి("64-bit: "
				       "Expected bit %d to be clear.\n", i);
				నిష్క్రమించు(1);
			}
		}
	}
	రాయి("64-bit: ext2fs_test_bit appears to be correct\n");

	/* Test ext2fs_set_bit */
	memset(testarray, 0, పరిమాణం(testarray));
	కొరకు (i=0; బిట్లు_list[i] > 0; i++) {
		ext2fs_సమితి_bit64(బిట్లు_list[i], testarray);
	}
	ఒకవేళ (memcmp(testarray, bitarray, పరిమాణం(testarray)) == 0) {
		రాయి("64-bit: ext2fs_set_bit test succeeded.\n");
	} లేకపోతే {
		రాయి("64-bit: ext2fs_set_bit test failed.\n");
		కొరకు (i=0; i < పరిమాణం(testarray); i++) {
			రాయి("%02x ", testarray[i]);
		}
		రాయి("\n");
		నిష్క్రమించు(1);
	}
	కొరకు (i=0; బిట్లు_list[i] > 0; i++) {
		ext2fs_clear_bit64(బిట్లు_list[i], testarray);
	}
	కొరకు (i=0; i < పరిమాణం(testarray); i++) {
		ఒకవేళ (testarray[i]) {
			రాయి("64-bit: ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			నిష్క్రమించు(1);
		}
	}
	రాయి("64-bit: ext2fs_clear_bit test succeed.\n");

	/* Do bigarray test */
        bigarray[BIG_TEST_బిట్టు >> 3] = 0;

	ext2fs_సమితి_bit64(BIG_TEST_బిట్టు, bigarray);
	రాయి("64-bit: big bit number (%u) test: %d, expected %d\n",
	       BIG_TEST_బిట్టు, bigarray[BIG_TEST_బిట్టు >> 3],
	       (1 << (BIG_TEST_బిట్టు & 7)));
	ఒకవేళ (bigarray[BIG_TEST_బిట్టు >> 3] != (1 << (BIG_TEST_బిట్టు & 7)))
		నిష్క్రమించు(1);

	ext2fs_clear_bit64(BIG_TEST_బిట్టు, bigarray);

	రాయి("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_బిట్టు,
	       bigarray[BIG_TEST_బిట్టు >> 3]);
	ఒకవేళ (bigarray[BIG_TEST_బిట్టు >> 3] != 0)
		నిష్క్రమించు(1);

	రాయి("64-bit: ext2fs_set_bit big_test successful\n");

	/* Now test ext2fs_fast_set_bit */
	memset(testarray, 0, పరిమాణం(testarray));
	కొరకు (i=0; బిట్లు_list[i] > 0; i++) {
		ext2fs_fast_సమితి_bit64(బిట్లు_list[i], testarray);
	}
	ఒకవేళ (memcmp(testarray, bitarray, పరిమాణం(testarray)) == 0) {
		రాయి("64-bit: ext2fs_fast_set_bit test succeeded.\n");
	} లేకపోతే {
		రాయి("64-bit: ext2fs_fast_set_bit test failed.\n");
		కొరకు (i=0; i < పరిమాణం(testarray); i++) {
			రాయి("%02x ", testarray[i]);
		}
		రాయి("\n");
		నిష్క్రమించు(1);
	}
	కొరకు (i=0; బిట్లు_list[i] > 0; i++) {
		ext2fs_clear_bit64(బిట్లు_list[i], testarray);
	}
	కొరకు (i=0; i < పరిమాణం(testarray); i++) {
		ఒకవేళ (testarray[i]) {
			రాయి("64-bit: ext2fs_clear_bit failed, "
			       "testarray[%d] is %d\n", i, testarray[i]);
			నిష్క్రమించు(1);
		}
	}
	రాయి("64-bit: ext2fs_clear_bit test succeed.\n");

        bigarray[BIG_TEST_బిట్టు >> 3] = 0;

	ext2fs_fast_సమితి_bit64(BIG_TEST_బిట్టు, bigarray);
	రాయి("64-bit: big bit number (%u) test: %d, expected %d\n",
	       BIG_TEST_బిట్టు, bigarray[BIG_TEST_బిట్టు >> 3],
	       (1 << (BIG_TEST_బిట్టు & 7)));
	ఒకవేళ (bigarray[BIG_TEST_బిట్టు >> 3] != (1 << (BIG_TEST_బిట్టు & 7)))
		నిష్క్రమించు(1);

	ext2fs_fast_clear_bit64(BIG_TEST_బిట్టు, bigarray);

	రాయి("64-bit: big bit number (%u) test: %d, expected 0\n",
	       BIG_TEST_బిట్టు, bigarray[BIG_TEST_బిట్టు >> 3]);
	ఒకవేళ (bigarray[BIG_TEST_బిట్టు >> 3] != 0)
		నిష్క్రమించు(1);

	రాయి("64-bit: ext2fs_fast_set_bit big_test successful\n");
	విడుదల(bigarray);
	నిష్క్రమించు(0);
}
