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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <fcntl.h>
#شامل <time.h>
#شامل <sys/stat.h>
#شامل <sys/types.h>
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

#تعریف_کرو جوڑو_BLK	0x0001
#تعریف_کرو DEL_BLK	0x0002

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
	50, جوڑو_BLK,
	51, DEL_BLK,
	57, DEL_BLK,
	66, جوڑو_BLK,
	31, DEL_BLK,
	12, جوڑو_BLK,
	2, جوڑو_BLK,
	13, جوڑو_BLK,
	1, DEL_BLK,
	0
	};


ساکن صحیح test_fail = 0;
ساکن صحیح test_expected_fail = 0;

ساکن errcode_t create_test_list(blk_t *vec, badblocks_list *ret)
{
	errcode_t	retval;
	badblocks_list	bb;
	صحیح		i;

	retval = ext2fs_badblocks_list_create(&bb, 5);
	اگر (retval) {
		com_err("create_test_list", retval, "while creating list");
		واپس retval;
	}
	برائے (i=0; vec[i]; i++) {
		retval = ext2fs_badblocks_list_جوڑو(bb, vec[i]);
		اگر (retval) {
			com_err("create_test_list", retval,
				"while adding test vector %d", i);
			ext2fs_badblocks_list_free(bb);
			واپس retval;
		}
	}
	*ret = bb;
	واپس 0;
}

ساکن خالی print_list(badblocks_list bb, صحیح verify)
{
	errcode_t	retval;
	badblocks_iterate	iter;
	blk_t			blk;
	صحیح			i, ok;

	retval = ext2fs_badblocks_list_iterate_begin(bb, &iter);
	اگر (retval) {
		com_err("print_list", retval, "while setting up iterator");
		واپس;
	}
	ok = i = 1;
	جب (ext2fs_badblocks_list_iterate(iter, &blk)) {
		لکھو("%u ", blk);
		اگر (i++ != blk)
			ok = 0;
	}
	ext2fs_badblocks_list_iterate_انتہا(iter);
	اگر (verify) {
		اگر (ok)
			لکھو("--- OK");
		ورنہ {
			لکھو("--- NOT OK");
			test_fail++;
		}
	}
}

ساکن خالی validate_test_seq(badblocks_list bb, blk_t *vec)
{
	صحیح	i, مطابقت, ok;

	برائے (i = 0; vec[i]; i += 2) {
		مطابقت = ext2fs_badblocks_list_test(bb, vec[i]);
		اگر (مطابقت == vec[i+1])
			ok = 1;
		ورنہ {
			ok = 0;
			test_fail++;
		}
		لکھو("\tblock %u is %s --- %s\n", vec[i],
		       مطابقت ? "present" : "absent",
		       ok ? "OK" : "NOT OK");
	}
}

ساکن خالی do_test_seq(badblocks_list bb, blk_t *vec)
{
	صحیح	i, مطابقت;

	برائے (i = 0; vec[i]; i += 2) {
		انتخاب (vec[i+1]) {
		حالت جوڑو_BLK:
			ext2fs_badblocks_list_جوڑو(bb, vec[i]);
			مطابقت = ext2fs_badblocks_list_test(bb, vec[i]);
			لکھو("Adding block %u --- now %s\n", vec[i],
			       مطابقت ? "present" : "absent");
			اگر (!مطابقت) {
				لکھو("FAILURE!\n");
				test_fail++;
			}
			توڑو;
		حالت DEL_BLK:
			ext2fs_badblocks_list_del(bb, vec[i]);
			مطابقت = ext2fs_badblocks_list_test(bb, vec[i]);
			لکھو("Removing block %u --- now %s\n", vec[i],
			       ext2fs_badblocks_list_test(bb, vec[i]) ?
			       "present" : "absent");
			اگر (مطابقت) {
				لکھو("FAILURE!\n");
				test_fail++;
			}
			توڑو;
		}
	}
}


صحیح file_test(badblocks_list bb)
{
	badblocks_list new_bb = 0;
	errcode_t	retval;
	FILE	*f;

	f = tmpfile();
	اگر (!f) {
		fprintf(stderr, "Error opening temp file: %s\n",
			error_پیغام(errno));
		واپس 1;
	}
	retval = ext2fs_write_bb_FILE(bb, 0, f);
	اگر (retval) {
		com_err("file_test", retval, "while writing bad blocks");
		واپس 1;
	}

	rewind(f);
	retval = ext2fs_read_bb_FILE2(0, f, &new_bb, 0, 0);
	اگر (retval) {
		com_err("file_test", retval, "while reading bad blocks");
		واپس 1;
	}
	fclose(f);

	اگر (ext2fs_badblocks_equal(bb, new_bb)) {
		لکھو("Block bitmap matched after reading and writing.\n");
	} ورنہ {
		لکھو("Block bitmap NOT matched.\n");
		test_fail++;
	}
	ext2fs_badblocks_list_free(new_bb);
	واپس 0;
}

ساکن خالی invalid_proc(ext2_filsys fs, blk_t blk)
{
	اگر (blk == 34500) {
		لکھو("Expected invalid block\n");
		test_expected_fail++;
	} ورنہ {
		لکھو("Invalid block #: %u\n", blk);
		test_fail++;
	}
}

خالی file_test_invalid(badblocks_list bb)
{
	badblocks_list new_bb = 0;
	errcode_t	retval;
	ext2_filsys 	fs;
	FILE	*f;

	fs = حافظہ_دو(حجم(ساخت struct_ext2_filsys));
	memset(fs, 0, حجم(ساخت struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->super = حافظہ_دو(SUPERBLOCK_سائز);
	memset(fs->super, 0, SUPERBLOCK_سائز);
	fs->super->s_پہلا_ڈیٹا_بلاک = 1;
	ext2fs_بلاکیں_گنتی_سیٹ(fs->super, 100);

	f = tmpfile();
	اگر (!f) {
		fprintf(stderr, "Error opening temp file: %s\n",
			error_پیغام(errno));
		test_fail++;
		جاؤ برآمد;
	}
	retval = ext2fs_write_bb_FILE(bb, 0, f);
	اگر (retval) {
		com_err("file_test", retval, "while writing bad blocks");
		test_fail++;
		جاؤ برآمد;
	}
	fprintf(f, "34500\n");

	rewind(f);
	test_expected_fail = 0;
	retval = ext2fs_read_bb_FILE(fs, f, &new_bb, invalid_proc);
	اگر (retval) {
		com_err("file_test", retval, "while reading bad blocks");
		test_fail++;
		جاؤ برآمد;
	}
	fclose(f);
	اگر (!test_expected_fail) {
		لکھو("Expected test failure didn't happen!\n");
		test_fail++;
	}


	اگر (ext2fs_badblocks_equal(bb, new_bb)) {
		لکھو("Block bitmap matched after reading and writing.\n");
	} ورنہ {
		لکھو("Block bitmap NOT matched.\n");
		test_fail++;
	}
	ext2fs_badblocks_list_free(new_bb);
برآمد:
	آزاد(fs->super);
	آزاد(fs);
}

صحیح مین(صحیح argc, حرف **argv)
{
	badblocks_list bb1, bb2, bb3, bb4, bb5;
	صحیح	equal;
	errcode_t	retval;

	جوڑو_error_جدول(&et_ext2_error_جدول);

	bb1 = bb2 = bb3 = bb4 = bb5 = 0;

	لکھو("test1: ");
	retval = create_test_list(test1, &bb1);
	اگر (retval == 0)
		print_list(bb1, 1);
	لکھو("\n");

	لکھو("test2: ");
	retval = create_test_list(test2, &bb2);
	اگر (retval == 0)
		print_list(bb2, 1);
	لکھو("\n");

	لکھو("test3: ");
	retval = create_test_list(test3, &bb3);
	اگر (retval == 0)
		print_list(bb3, 1);
	لکھو("\n");

	لکھو("test4: ");
	retval = create_test_list(test4, &bb4);
	اگر (retval == 0) {
		print_list(bb4, 0);
		لکھو("\n");
		validate_test_seq(bb4, test4a);
	}
	لکھو("\n");

	لکھو("test5: ");
	retval = create_test_list(test5, &bb5);
	اگر (retval == 0) {
		print_list(bb5, 0);
		لکھو("\n");
		do_test_seq(bb5, test5a);
		لکھو("After test5 sequence: ");
		print_list(bb5, 0);
		لکھو("\n");
	}
	لکھو("\n");

	اگر (bb1 && bb2 && bb3 && bb4 && bb5) {
		لکھو("Comparison tests:\n");
		equal = ext2fs_badblocks_equal(bb1, bb2);
		لکھو("bb1 and bb2 are %sequal.\n", equal ? "" : "NOT ");
		اگر (equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb1, bb3);
		لکھو("bb1 and bb3 are %sequal.\n", equal ? "" : "NOT ");
		اگر (!equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb1, bb4);
		لکھو("bb1 and bb4 are %sequal.\n", equal ? "" : "NOT ");
		اگر (equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb4, bb5);
		لکھو("bb4 and bb5 are %sequal.\n", equal ? "" : "NOT ");
		اگر (!equal)
			test_fail++;
		لکھو("\n");
	}

	file_test(bb4);

	file_test_invalid(bb4);

	اگر (test_fail == 0)
		لکھو("ext2fs library badblocks tests checks out OK!\n");

	اگر (bb1)
		ext2fs_badblocks_list_free(bb1);
	اگر (bb2)
		ext2fs_badblocks_list_free(bb2);
	اگر (bb3)
		ext2fs_badblocks_list_free(bb3);
	اگر (bb4)
		ext2fs_badblocks_list_free(bb4);
	اگر (bb5)
		ext2fs_badblocks_list_free(bb5);

	واپس test_fail;

}
