/*
 * This testing program makes sure the badblocks implementation works.
 *
 * Copyright (C) 1996 by Theodore Ts'o.
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

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"

#define ADD_BLK	0x0001
#define DEL_BLK	0x0002

blk_t test1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0 };
blk_t test2[] = { 11, 10, 9, 8, 7, 6, 5, 4, 3, 3, 2, 1, 0 };
blk_t test3[] = { 3, 1, 4, 5, 9, 2, 7, 10, 5, 6, 10, 8, 0 };
blk_t test4[] = { 20, 50, 12, 17, 13, 2, 66, 23, 56, 0 };
blk_t test4a[] = {
	20, 1,
	50, 1,
	3, 0,
	17, 1,
	18, 0,
	16, 0,
	11, 0,
	12, 1,
	13, 1,
	14, 0,
	80, 0,
	45, 0,
	66, 1,
	0 };
blk_t test5[] = { 31, 20, 17, 51, 23, 1, 56, 57, 0 };
blk_t test5a[] = {
	50, ADD_BLK,
	51, DEL_BLK,
	57, DEL_BLK,
	66, ADD_BLK,
	31, DEL_BLK,
	12, ADD_BLK,
	2, ADD_BLK,
	13, ADD_BLK,
	1, DEL_BLK,
	0
	};


স্থির পূর্ণসংখ্যা test_fail = 0;
স্থির পূর্ণসংখ্যা test_expected_fail = 0;

স্থির errcode_t create_test_list(blk_t *vec, badblocks_list *ret)
{
	errcode_t	retval;
	badblocks_list	bb;
	পূর্ণসংখ্যা		i;

	retval = ext2fs_badblocks_list_create(&bb, 5);
	যদি (retval) {
		com_err("create_test_list", retval, "while creating list");
		ফেরত retval;
	}
	জন্য (i=0; vec[i]; i++) {
		retval = ext2fs_badblocks_list_add(bb, vec[i]);
		যদি (retval) {
			com_err("create_test_list", retval,
				"while adding test vector %d", i);
			ext2fs_badblocks_list_free(bb);
			ফেরত retval;
		}
	}
	*ret = bb;
	ফেরত 0;
}

স্থির শূন্য print_list(badblocks_list bb, পূর্ণসংখ্যা verify)
{
	errcode_t	retval;
	badblocks_iterate	iter;
	blk_t			blk;
	পূর্ণসংখ্যা			i, ok;

	retval = ext2fs_badblocks_list_iterate_begin(bb, &iter);
	যদি (retval) {
		com_err("print_list", retval, "while setting up iterator");
		ফেরত;
	}
	ok = i = 1;
	যখন (ext2fs_badblocks_list_iterate(iter, &blk)) {
		printf("%u ", blk);
		যদি (i++ != blk)
			ok = 0;
	}
	ext2fs_badblocks_list_iterate_end(iter);
	যদি (verify) {
		যদি (ok)
			printf("--- OK");
		অন্যথা {
			printf("--- NOT OK");
			test_fail++;
		}
	}
}

স্থির শূন্য validate_test_seq(badblocks_list bb, blk_t *vec)
{
	পূর্ণসংখ্যা	i, match, ok;

	জন্য (i = 0; vec[i]; i += 2) {
		match = ext2fs_badblocks_list_test(bb, vec[i]);
		যদি (match == vec[i+1])
			ok = 1;
		অন্যথা {
			ok = 0;
			test_fail++;
		}
		printf("\tblock %u is %s --- %s\n", vec[i],
		       match ? "present" : "absent",
		       ok ? "OK" : "NOT OK");
	}
}

স্থির শূন্য do_test_seq(badblocks_list bb, blk_t *vec)
{
	পূর্ণসংখ্যা	i, match;

	জন্য (i = 0; vec[i]; i += 2) {
		নির্বাচন (vec[i+1]) {
		ক্ষেত্র ADD_BLK:
			ext2fs_badblocks_list_add(bb, vec[i]);
			match = ext2fs_badblocks_list_test(bb, vec[i]);
			printf("Adding block %u --- now %s\n", vec[i],
			       match ? "present" : "absent");
			যদি (!match) {
				printf("FAILURE!\n");
				test_fail++;
			}
			বিরতি;
		ক্ষেত্র DEL_BLK:
			ext2fs_badblocks_list_del(bb, vec[i]);
			match = ext2fs_badblocks_list_test(bb, vec[i]);
			printf("Removing block %u --- now %s\n", vec[i],
			       ext2fs_badblocks_list_test(bb, vec[i]) ?
			       "present" : "absent");
			যদি (match) {
				printf("FAILURE!\n");
				test_fail++;
			}
			বিরতি;
		}
	}
}


পূর্ণসংখ্যা file_test(badblocks_list bb)
{
	badblocks_list new_bb = 0;
	errcode_t	retval;
	FILE	*f;

	f = tmpfile();
	যদি (!f) {
		fprintf(stderr, "Error opening temp file: %s\n",
			error_message(errno));
		ফেরত 1;
	}
	retval = ext2fs_write_bb_FILE(bb, 0, f);
	যদি (retval) {
		com_err("file_test", retval, "while writing bad blocks");
		ফেরত 1;
	}

	rewind(f);
	retval = ext2fs_read_bb_FILE2(0, f, &new_bb, 0, 0);
	যদি (retval) {
		com_err("file_test", retval, "while reading bad blocks");
		ফেরত 1;
	}
	fclose(f);

	যদি (ext2fs_badblocks_equal(bb, new_bb)) {
		printf("Block bitmap matched after reading and writing.\n");
	} অন্যথা {
		printf("Block bitmap NOT matched.\n");
		test_fail++;
	}
	ext2fs_badblocks_list_free(new_bb);
	ফেরত 0;
}

স্থির শূন্য invalid_proc(ext2_filsys fs, blk_t blk)
{
	যদি (blk == 34500) {
		printf("Expected invalid block\n");
		test_expected_fail++;
	} অন্যথা {
		printf("Invalid block #: %u\n", blk);
		test_fail++;
	}
}

শূন্য file_test_invalid(badblocks_list bb)
{
	badblocks_list new_bb = 0;
	errcode_t	retval;
	ext2_filsys 	fs;
	FILE	*f;

	fs = malloc(আকার(কাঠামো struct_ext2_filsys));
	memset(fs, 0, আকার(কাঠামো struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->super = malloc(SUPERBLOCK_আকার);
	memset(fs->super, 0, SUPERBLOCK_আকার);
	fs->super->s_first_তথ্য_block = 1;
	ext2fs_blocks_গুনতি_set(fs->super, 100);

	f = tmpfile();
	যদি (!f) {
		fprintf(stderr, "Error opening temp file: %s\n",
			error_message(errno));
		test_fail++;
		যাও out;
	}
	retval = ext2fs_write_bb_FILE(bb, 0, f);
	যদি (retval) {
		com_err("file_test", retval, "while writing bad blocks");
		test_fail++;
		যাও out;
	}
	fprintf(f, "34500\n");

	rewind(f);
	test_expected_fail = 0;
	retval = ext2fs_read_bb_FILE(fs, f, &new_bb, invalid_proc);
	যদি (retval) {
		com_err("file_test", retval, "while reading bad blocks");
		test_fail++;
		যাও out;
	}
	fclose(f);
	যদি (!test_expected_fail) {
		printf("Expected test failure didn't happen!\n");
		test_fail++;
	}


	যদি (ext2fs_badblocks_equal(bb, new_bb)) {
		printf("Block bitmap matched after reading and writing.\n");
	} অন্যথা {
		printf("Block bitmap NOT matched.\n");
		test_fail++;
	}
	ext2fs_badblocks_list_free(new_bb);
out:
	মুক্ত(fs->super);
	মুক্ত(fs);
}

পূর্ণসংখ্যা প্রধান(পূর্ণসংখ্যা argc, অক্ষর **argv)
{
	badblocks_list bb1, bb2, bb3, bb4, bb5;
	পূর্ণসংখ্যা	equal;
	errcode_t	retval;

	add_error_table(&et_ext2_error_table);

	bb1 = bb2 = bb3 = bb4 = bb5 = 0;

	printf("test1: ");
	retval = create_test_list(test1, &bb1);
	যদি (retval == 0)
		print_list(bb1, 1);
	printf("\n");

	printf("test2: ");
	retval = create_test_list(test2, &bb2);
	যদি (retval == 0)
		print_list(bb2, 1);
	printf("\n");

	printf("test3: ");
	retval = create_test_list(test3, &bb3);
	যদি (retval == 0)
		print_list(bb3, 1);
	printf("\n");

	printf("test4: ");
	retval = create_test_list(test4, &bb4);
	যদি (retval == 0) {
		print_list(bb4, 0);
		printf("\n");
		validate_test_seq(bb4, test4a);
	}
	printf("\n");

	printf("test5: ");
	retval = create_test_list(test5, &bb5);
	যদি (retval == 0) {
		print_list(bb5, 0);
		printf("\n");
		do_test_seq(bb5, test5a);
		printf("After test5 sequence: ");
		print_list(bb5, 0);
		printf("\n");
	}
	printf("\n");

	যদি (bb1 && bb2 && bb3 && bb4 && bb5) {
		printf("Comparison tests:\n");
		equal = ext2fs_badblocks_equal(bb1, bb2);
		printf("bb1 and bb2 are %sequal.\n", equal ? "" : "NOT ");
		যদি (equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb1, bb3);
		printf("bb1 and bb3 are %sequal.\n", equal ? "" : "NOT ");
		যদি (!equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb1, bb4);
		printf("bb1 and bb4 are %sequal.\n", equal ? "" : "NOT ");
		যদি (equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb4, bb5);
		printf("bb4 and bb5 are %sequal.\n", equal ? "" : "NOT ");
		যদি (!equal)
			test_fail++;
		printf("\n");
	}

	file_test(bb4);

	file_test_invalid(bb4);

	যদি (test_fail == 0)
		printf("ext2fs library badblocks tests checks out OK!\n");

	যদি (bb1)
		ext2fs_badblocks_list_free(bb1);
	যদি (bb2)
		ext2fs_badblocks_list_free(bb2);
	যদি (bb3)
		ext2fs_badblocks_list_free(bb3);
	যদি (bb4)
		ext2fs_badblocks_list_free(bb4);
	যদি (bb5)
		ext2fs_badblocks_list_free(bb5);

	ফেরত test_fail;

}
