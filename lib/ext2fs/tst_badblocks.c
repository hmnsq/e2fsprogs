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

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

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


സ്ഥിതം പൂർണ്ണസംഖ്യ test_fail = 0;
സ്ഥിതം പൂർണ്ണസംഖ്യ test_expected_fail = 0;

സ്ഥിതം errcode_t create_test_list(blk_t *vec, badblocks_list *ret)
{
	errcode_t	retval;
	badblocks_list	bb;
	പൂർണ്ണസംഖ്യ		i;

	retval = ext2fs_badblocks_list_create(&bb, 5);
	എങ്കിൽ (retval) {
		com_err("create_test_list", retval, "while creating list");
		തിരികെ retval;
	}
	വേണ്ടി (i=0; vec[i]; i++) {
		retval = ext2fs_badblocks_list_add(bb, vec[i]);
		എങ്കിൽ (retval) {
			com_err("create_test_list", retval,
				"while adding test vector %d", i);
			ext2fs_badblocks_list_free(bb);
			തിരികെ retval;
		}
	}
	*ret = bb;
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം print_list(badblocks_list bb, പൂർണ്ണസംഖ്യ verify)
{
	errcode_t	retval;
	badblocks_iterate	iter;
	blk_t			blk;
	പൂർണ്ണസംഖ്യ			i, ok;

	retval = ext2fs_badblocks_list_iterate_begin(bb, &iter);
	എങ്കിൽ (retval) {
		com_err("print_list", retval, "while setting up iterator");
		തിരികെ;
	}
	ok = i = 1;
	ആകുമ്പോൾ (ext2fs_badblocks_list_iterate(iter, &blk)) {
		printf("%u ", blk);
		എങ്കിൽ (i++ != blk)
			ok = 0;
	}
	ext2fs_badblocks_list_iterate_end(iter);
	എങ്കിൽ (verify) {
		എങ്കിൽ (ok)
			printf("--- OK");
		അല്ലെങ്കിൽ {
			printf("--- NOT OK");
			test_fail++;
		}
	}
}

സ്ഥിതം ശൂന്യം validate_test_seq(badblocks_list bb, blk_t *vec)
{
	പൂർണ്ണസംഖ്യ	i, match, ok;

	വേണ്ടി (i = 0; vec[i]; i += 2) {
		match = ext2fs_badblocks_list_test(bb, vec[i]);
		എങ്കിൽ (match == vec[i+1])
			ok = 1;
		അല്ലെങ്കിൽ {
			ok = 0;
			test_fail++;
		}
		printf("\tblock %u is %s --- %s\n", vec[i],
		       match ? "present" : "absent",
		       ok ? "OK" : "NOT OK");
	}
}

സ്ഥിതം ശൂന്യം do_test_seq(badblocks_list bb, blk_t *vec)
{
	പൂർണ്ണസംഖ്യ	i, match;

	വേണ്ടി (i = 0; vec[i]; i += 2) {
		തിരഞ്ഞെടുപ്പ് (vec[i+1]) {
		അവസ്ഥ ADD_BLK:
			ext2fs_badblocks_list_add(bb, vec[i]);
			match = ext2fs_badblocks_list_test(bb, vec[i]);
			printf("Adding block %u --- now %s\n", vec[i],
			       match ? "present" : "absent");
			എങ്കിൽ (!match) {
				printf("FAILURE!\n");
				test_fail++;
			}
			നിർത്തുക;
		അവസ്ഥ DEL_BLK:
			ext2fs_badblocks_list_del(bb, vec[i]);
			match = ext2fs_badblocks_list_test(bb, vec[i]);
			printf("Removing block %u --- now %s\n", vec[i],
			       ext2fs_badblocks_list_test(bb, vec[i]) ?
			       "present" : "absent");
			എങ്കിൽ (match) {
				printf("FAILURE!\n");
				test_fail++;
			}
			നിർത്തുക;
		}
	}
}


പൂർണ്ണസംഖ്യ file_test(badblocks_list bb)
{
	badblocks_list new_bb = 0;
	errcode_t	retval;
	FILE	*f;

	f = tmpfile();
	എങ്കിൽ (!f) {
		fprintf(stderr, "Error opening temp file: %s\n",
			error_message(errno));
		തിരികെ 1;
	}
	retval = ext2fs_write_bb_FILE(bb, 0, f);
	എങ്കിൽ (retval) {
		com_err("file_test", retval, "while writing bad blocks");
		തിരികെ 1;
	}

	rewind(f);
	retval = ext2fs_read_bb_FILE2(0, f, &new_bb, 0, 0);
	എങ്കിൽ (retval) {
		com_err("file_test", retval, "while reading bad blocks");
		തിരികെ 1;
	}
	fclose(f);

	എങ്കിൽ (ext2fs_badblocks_equal(bb, new_bb)) {
		printf("Block bitmap matched after reading and writing.\n");
	} അല്ലെങ്കിൽ {
		printf("Block bitmap NOT matched.\n");
		test_fail++;
	}
	ext2fs_badblocks_list_free(new_bb);
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം invalid_proc(ext2_filsys fs, blk_t blk)
{
	എങ്കിൽ (blk == 34500) {
		printf("Expected invalid block\n");
		test_expected_fail++;
	} അല്ലെങ്കിൽ {
		printf("Invalid block #: %u\n", blk);
		test_fail++;
	}
}

ശൂന്യം file_test_invalid(badblocks_list bb)
{
	badblocks_list new_bb = 0;
	errcode_t	retval;
	ext2_filsys 	fs;
	FILE	*f;

	fs = malloc(വലുപ്പം(ഘടന struct_ext2_filsys));
	memset(fs, 0, വലുപ്പം(ഘടന struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->super = malloc(SUPERBLOCK_SIZE);
	memset(fs->super, 0, SUPERBLOCK_SIZE);
	fs->super->s_first_data_block = 1;
	ext2fs_blocks_എണ്ണം_set(fs->super, 100);

	f = tmpfile();
	എങ്കിൽ (!f) {
		fprintf(stderr, "Error opening temp file: %s\n",
			error_message(errno));
		test_fail++;
		പോകുക out;
	}
	retval = ext2fs_write_bb_FILE(bb, 0, f);
	എങ്കിൽ (retval) {
		com_err("file_test", retval, "while writing bad blocks");
		test_fail++;
		പോകുക out;
	}
	fprintf(f, "34500\n");

	rewind(f);
	test_expected_fail = 0;
	retval = ext2fs_read_bb_FILE(fs, f, &new_bb, invalid_proc);
	എങ്കിൽ (retval) {
		com_err("file_test", retval, "while reading bad blocks");
		test_fail++;
		പോകുക out;
	}
	fclose(f);
	എങ്കിൽ (!test_expected_fail) {
		printf("Expected test failure didn't happen!\n");
		test_fail++;
	}


	എങ്കിൽ (ext2fs_badblocks_equal(bb, new_bb)) {
		printf("Block bitmap matched after reading and writing.\n");
	} അല്ലെങ്കിൽ {
		printf("Block bitmap NOT matched.\n");
		test_fail++;
	}
	ext2fs_badblocks_list_free(new_bb);
out:
	സ്വതന്ത്രം(fs->super);
	സ്വതന്ത്രം(fs);
}

പൂർണ്ണസംഖ്യ പ്രധാന(പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
	badblocks_list bb1, bb2, bb3, bb4, bb5;
	പൂർണ്ണസംഖ്യ	equal;
	errcode_t	retval;

	add_error_table(&et_ext2_error_table);

	bb1 = bb2 = bb3 = bb4 = bb5 = 0;

	printf("test1: ");
	retval = create_test_list(test1, &bb1);
	എങ്കിൽ (retval == 0)
		print_list(bb1, 1);
	printf("\n");

	printf("test2: ");
	retval = create_test_list(test2, &bb2);
	എങ്കിൽ (retval == 0)
		print_list(bb2, 1);
	printf("\n");

	printf("test3: ");
	retval = create_test_list(test3, &bb3);
	എങ്കിൽ (retval == 0)
		print_list(bb3, 1);
	printf("\n");

	printf("test4: ");
	retval = create_test_list(test4, &bb4);
	എങ്കിൽ (retval == 0) {
		print_list(bb4, 0);
		printf("\n");
		validate_test_seq(bb4, test4a);
	}
	printf("\n");

	printf("test5: ");
	retval = create_test_list(test5, &bb5);
	എങ്കിൽ (retval == 0) {
		print_list(bb5, 0);
		printf("\n");
		do_test_seq(bb5, test5a);
		printf("After test5 sequence: ");
		print_list(bb5, 0);
		printf("\n");
	}
	printf("\n");

	എങ്കിൽ (bb1 && bb2 && bb3 && bb4 && bb5) {
		printf("Comparison tests:\n");
		equal = ext2fs_badblocks_equal(bb1, bb2);
		printf("bb1 and bb2 are %sequal.\n", equal ? "" : "NOT ");
		എങ്കിൽ (equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb1, bb3);
		printf("bb1 and bb3 are %sequal.\n", equal ? "" : "NOT ");
		എങ്കിൽ (!equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb1, bb4);
		printf("bb1 and bb4 are %sequal.\n", equal ? "" : "NOT ");
		എങ്കിൽ (equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb4, bb5);
		printf("bb4 and bb5 are %sequal.\n", equal ? "" : "NOT ");
		എങ്കിൽ (!equal)
			test_fail++;
		printf("\n");
	}

	file_test(bb4);

	file_test_invalid(bb4);

	എങ്കിൽ (test_fail == 0)
		printf("ext2fs library badblocks tests checks out OK!\n");

	എങ്കിൽ (bb1)
		ext2fs_badblocks_list_free(bb1);
	എങ്കിൽ (bb2)
		ext2fs_badblocks_list_free(bb2);
	എങ്കിൽ (bb3)
		ext2fs_badblocks_list_free(bb3);
	എങ്കിൽ (bb4)
		ext2fs_badblocks_list_free(bb4);
	എങ്കിൽ (bb5)
		ext2fs_badblocks_list_free(bb5);

	തിരികെ test_fail;

}
