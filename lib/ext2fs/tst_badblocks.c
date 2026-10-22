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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#د_که_پای
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#تعریف_کړه جمع_کړه_BLK	0x0001
#تعریف_کړه DEL_BLK	0x0002

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
	50, جمع_کړه_BLK,
	51, DEL_BLK,
	57, DEL_BLK,
	66, جمع_کړه_BLK,
	31, DEL_BLK,
	12, جمع_کړه_BLK,
	2, جمع_کړه_BLK,
	13, جمع_کړه_BLK,
	1, DEL_BLK,
	0
	};


ساكن صحيح test_fail = 0;
ساكن صحيح test_expected_fail = 0;

ساكن errcode_t create_test_list(blk_t *vec, badblocks_list *ret)
{
	errcode_t	retval;
	badblocks_list	bb;
	صحيح		i;

	retval = ext2fs_badblocks_list_create(&bb, 5);
	إذا (retval) {
		com_err("create_test_list", retval, "while creating list");
		أرجع retval;
	}
	لكل (i=0; vec[i]; i++) {
		retval = ext2fs_badblocks_list_جمع_کړه(bb, vec[i]);
		إذا (retval) {
			com_err("create_test_list", retval,
				"while adding test vector %d", i);
			ext2fs_badblocks_list_free(bb);
			أرجع retval;
		}
	}
	*ret = bb;
	أرجع 0;
}

ساكن فراغ print_list(badblocks_list bb, صحيح verify)
{
	errcode_t	retval;
	badblocks_iterate	iter;
	blk_t			blk;
	صحيح			i, ok;

	retval = ext2fs_badblocks_list_iterate_begin(bb, &iter);
	إذا (retval) {
		com_err("print_list", retval, "while setting up iterator");
		أرجع;
	}
	ok = i = 1;
	طالما (ext2fs_badblocks_list_iterate(iter, &blk)) {
		چاپ_کړه("%u ", blk);
		إذا (i++ != blk)
			ok = 0;
	}
	ext2fs_badblocks_list_iterate_پای_ټکی(iter);
	إذا (verify) {
		إذا (ok)
			چاپ_کړه("--- OK");
		وإلا {
			چاپ_کړه("--- NOT OK");
			test_fail++;
		}
	}
}

ساكن فراغ validate_test_seq(badblocks_list bb, blk_t *vec)
{
	صحيح	i, سمون, ok;

	لكل (i = 0; vec[i]; i += 2) {
		سمون = ext2fs_badblocks_list_test(bb, vec[i]);
		إذا (سمون == vec[i+1])
			ok = 1;
		وإلا {
			ok = 0;
			test_fail++;
		}
		چاپ_کړه("\tblock %u is %s --- %s\n", vec[i],
		       سمون ? "present" : "absent",
		       ok ? "OK" : "NOT OK");
	}
}

ساكن فراغ do_test_seq(badblocks_list bb, blk_t *vec)
{
	صحيح	i, سمون;

	لكل (i = 0; vec[i]; i += 2) {
		بدل (vec[i+1]) {
		حالة جمع_کړه_BLK:
			ext2fs_badblocks_list_جمع_کړه(bb, vec[i]);
			سمون = ext2fs_badblocks_list_test(bb, vec[i]);
			چاپ_کړه("Adding block %u --- now %s\n", vec[i],
			       سمون ? "present" : "absent");
			إذا (!سمون) {
				چاپ_کړه("FAILURE!\n");
				test_fail++;
			}
			اكسر;
		حالة DEL_BLK:
			ext2fs_badblocks_list_del(bb, vec[i]);
			سمون = ext2fs_badblocks_list_test(bb, vec[i]);
			چاپ_کړه("Removing block %u --- now %s\n", vec[i],
			       ext2fs_badblocks_list_test(bb, vec[i]) ?
			       "present" : "absent");
			إذا (سمون) {
				چاپ_کړه("FAILURE!\n");
				test_fail++;
			}
			اكسر;
		}
	}
}


صحيح file_test(badblocks_list bb)
{
	badblocks_list new_bb = 0;
	errcode_t	retval;
	FILE	*f;

	f = tmpfile();
	إذا (!f) {
		fprintf(stderr, "Error opening temp file: %s\n",
			error_پیغام(errno));
		أرجع 1;
	}
	retval = ext2fs_write_bb_FILE(bb, 0, f);
	إذا (retval) {
		com_err("file_test", retval, "while writing bad blocks");
		أرجع 1;
	}

	rewind(f);
	retval = ext2fs_read_bb_FILE2(0, f, &new_bb, 0, 0);
	إذا (retval) {
		com_err("file_test", retval, "while reading bad blocks");
		أرجع 1;
	}
	fclose(f);

	إذا (ext2fs_badblocks_equal(bb, new_bb)) {
		چاپ_کړه("Block bitmap matched after reading and writing.\n");
	} وإلا {
		چاپ_کړه("Block bitmap NOT matched.\n");
		test_fail++;
	}
	ext2fs_badblocks_list_free(new_bb);
	أرجع 0;
}

ساكن فراغ invalid_proc(ext2_filsys fs, blk_t blk)
{
	إذا (blk == 34500) {
		چاپ_کړه("Expected invalid block\n");
		test_expected_fail++;
	} وإلا {
		چاپ_کړه("Invalid block #: %u\n", blk);
		test_fail++;
	}
}

فراغ file_test_invalid(badblocks_list bb)
{
	badblocks_list new_bb = 0;
	errcode_t	retval;
	ext2_filsys 	fs;
	FILE	*f;

	fs = حافظه_ورکړه(حجم(بنية struct_ext2_filsys));
	memset(fs, 0, حجم(بنية struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->super = حافظه_ورکړه(SUPERBLOCK_اندازه);
	memset(fs->super, 0, SUPERBLOCK_اندازه);
	fs->super->s_لومړی_معلومات_بلاک = 1;
	ext2fs_بلاکونه_شمېر_سېټ(fs->super, 100);

	f = tmpfile();
	إذا (!f) {
		fprintf(stderr, "Error opening temp file: %s\n",
			error_پیغام(errno));
		test_fail++;
		اذهب بهر;
	}
	retval = ext2fs_write_bb_FILE(bb, 0, f);
	إذا (retval) {
		com_err("file_test", retval, "while writing bad blocks");
		test_fail++;
		اذهب بهر;
	}
	fprintf(f, "34500\n");

	rewind(f);
	test_expected_fail = 0;
	retval = ext2fs_read_bb_FILE(fs, f, &new_bb, invalid_proc);
	إذا (retval) {
		com_err("file_test", retval, "while reading bad blocks");
		test_fail++;
		اذهب بهر;
	}
	fclose(f);
	إذا (!test_expected_fail) {
		چاپ_کړه("Expected test failure didn't happen!\n");
		test_fail++;
	}


	إذا (ext2fs_badblocks_equal(bb, new_bb)) {
		چاپ_کړه("Block bitmap matched after reading and writing.\n");
	} وإلا {
		چاپ_کړه("Block bitmap NOT matched.\n");
		test_fail++;
	}
	ext2fs_badblocks_list_free(new_bb);
بهر:
	حرر(fs->super);
	حرر(fs);
}

صحيح اصلي(صحيح argc, حرف **argv)
{
	badblocks_list bb1, bb2, bb3, bb4, bb5;
	صحيح	equal;
	errcode_t	retval;

	جمع_کړه_error_جدول(&et_ext2_error_جدول);

	bb1 = bb2 = bb3 = bb4 = bb5 = 0;

	چاپ_کړه("test1: ");
	retval = create_test_list(test1, &bb1);
	إذا (retval == 0)
		print_list(bb1, 1);
	چاپ_کړه("\n");

	چاپ_کړه("test2: ");
	retval = create_test_list(test2, &bb2);
	إذا (retval == 0)
		print_list(bb2, 1);
	چاپ_کړه("\n");

	چاپ_کړه("test3: ");
	retval = create_test_list(test3, &bb3);
	إذا (retval == 0)
		print_list(bb3, 1);
	چاپ_کړه("\n");

	چاپ_کړه("test4: ");
	retval = create_test_list(test4, &bb4);
	إذا (retval == 0) {
		print_list(bb4, 0);
		چاپ_کړه("\n");
		validate_test_seq(bb4, test4a);
	}
	چاپ_کړه("\n");

	چاپ_کړه("test5: ");
	retval = create_test_list(test5, &bb5);
	إذا (retval == 0) {
		print_list(bb5, 0);
		چاپ_کړه("\n");
		do_test_seq(bb5, test5a);
		چاپ_کړه("After test5 sequence: ");
		print_list(bb5, 0);
		چاپ_کړه("\n");
	}
	چاپ_کړه("\n");

	إذا (bb1 && bb2 && bb3 && bb4 && bb5) {
		چاپ_کړه("Comparison tests:\n");
		equal = ext2fs_badblocks_equal(bb1, bb2);
		چاپ_کړه("bb1 and bb2 are %sequal.\n", equal ? "" : "NOT ");
		إذا (equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb1, bb3);
		چاپ_کړه("bb1 and bb3 are %sequal.\n", equal ? "" : "NOT ");
		إذا (!equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb1, bb4);
		چاپ_کړه("bb1 and bb4 are %sequal.\n", equal ? "" : "NOT ");
		إذا (equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb4, bb5);
		چاپ_کړه("bb4 and bb5 are %sequal.\n", equal ? "" : "NOT ");
		إذا (!equal)
			test_fail++;
		چاپ_کړه("\n");
	}

	file_test(bb4);

	file_test_invalid(bb4);

	إذا (test_fail == 0)
		چاپ_کړه("ext2fs library badblocks tests checks out OK!\n");

	إذا (bb1)
		ext2fs_badblocks_list_free(bb1);
	إذا (bb2)
		ext2fs_badblocks_list_free(bb2);
	إذا (bb3)
		ext2fs_badblocks_list_free(bb3);
	إذا (bb4)
		ext2fs_badblocks_list_free(bb4);
	إذا (bb5)
		ext2fs_badblocks_list_free(bb5);

	أرجع test_fail;

}
