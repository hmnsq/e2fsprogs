/*
 * tst_inode.c --- this function tests the inode scan function
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

blk64_t test_vec[] = { 8, 12, 24, 34, 43, 44, 100, 0 };

ext2_filsys	test_fs;
ext2fs_بلاک_bitmap bad_بلاک_map, touched_map;
ext2fs_inode_bitmap bad_inode_map;
badblocks_list	test_badblocks;

صحیح پہلا_no_comma = 1;
صحیح failed = 0;

ساکن خالی iscan_test_read_blk64(غیرعلامتی طویل طویل بلاک, صحیح گنتی, errcode_t err)
{
	صحیح	i;

	اگر (پہلا_no_comma)
		پہلا_no_comma = 0;
	ورنہ
		لکھو(", ");

	اگر (گنتی > 1)
		لکھو("%llu-%llu", بلاک, بلاک+گنتی-1);
	ورنہ
		لکھو("%llu", بلاک);

	برائے (i=0; i < گنتی; i++, بلاک++) {
		اگر (ext2fs_test_بلاک_bitmap2(touched_map, بلاک)) {
			لکھو("\nDuplicate block?!? --- %llu\n", بلاک);
			failed++;
			پہلا_no_comma = 1;
		}
		ext2fs_mark_بلاک_bitmap2(touched_map, بلاک);
	}
}

ساکن خالی iscan_test_read_blk(غیرعلامتی طویل بلاک, صحیح گنتی, errcode_t err)
{
	iscan_test_read_blk64(بلاک, گنتی, err);
}

/*
 * Setup the variables for doing the inode scan test.
 */
ساکن خالی تیاری(خالی)
{
	errcode_t	retval;
	صحیح		i;
	ساخت ext2_super_بلاک param;

	initialize_ext2_error_جدول();

	memset(&param, 0, حجم(param));
	ext2fs_بلاکیں_گنتی_سیٹ(&param, 12000);


	test_io_cb_read_blk = iscan_test_read_blk;
	test_io_cb_read_blk64 = iscan_test_read_blk64;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_بٹ, &param,
				   test_io_manager, &test_fs);
	اگر (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		باہر_نکلو(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	اگر (retval) {
		com_err("setup", retval,
			"While allocating tables for test filesystem");
		باہر_نکلو(1);
	}
	retval = ext2fs_allocate_بلاک_bitmap(test_fs, "bad block map",
					      &bad_بلاک_map);
	اگر (retval) {
		com_err("setup", retval,
			"While allocating bad_block bitmap");
		باہر_نکلو(1);
	}
	retval = ext2fs_allocate_بلاک_bitmap(test_fs, "touched map",
					      &touched_map);
	اگر (retval) {
		com_err("setup", retval,
			"While allocating touched block bitmap");
		باہر_نکلو(1);
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "bad inode map",
					      &bad_inode_map);
	اگر (retval) {
		com_err("setup", retval,
			"While allocating bad inode bitmap");
		باہر_نکلو(1);
	}

	retval = ext2fs_badblocks_list_create(&test_badblocks, 5);
	اگر (retval) {
		com_err("setup", retval, "while creating badblocks list");
		باہر_نکلو(1);
	}
	برائے (i=0; test_vec[i]; i++) {
		retval = ext2fs_badblocks_list_جوڑو(test_badblocks, test_vec[i]);
		اگر (retval) {
			com_err("setup", retval,
				"while adding test vector %d", i);
			باہر_نکلو(1);
		}
		ext2fs_mark_بلاک_bitmap2(bad_بلاک_map, test_vec[i]);
	}
	test_fs->badblocks = test_badblocks;
}

/*
 * Iterate using inode_scan
 */
ساکن خالی iterate(خالی)
{
	ساخت ext2_inode inode;
	ext2_inode_جائزہ	جائزہ;
	errcode_t	retval;
	ext2_ino_t	ino;

	retval = ext2fs_open_inode_جائزہ(test_fs, 8, &جائزہ);
	اگر (retval) {
		com_err("iterate", retval, "While opening inode scan");
		باہر_نکلو(1);
	}
	لکھو("Reading blocks: ");
	retval = ext2fs_get_اگلا_inode(جائزہ, &ino, &inode);
	اگر (retval) {
		com_err("iterate", retval, "while reading first inode");
		باہر_نکلو(1);
	}
	جب (ino) {
		retval = ext2fs_get_اگلا_inode(جائزہ, &ino, &inode);
		اگر (retval == EXT2_ET_BAD_بلاک_اندر_INODE_جدول) {
			ext2fs_mark_inode_bitmap2(bad_inode_map, ino);
			جاری;
		}
		اگر (retval) {
			com_err("iterate", retval,
				"while getting next inode");
			باہر_نکلو(1);
		}
	}
	لکھو("\n");
	ext2fs_close_inode_جائزہ(جائزہ);
}

/*
 * Verify the touched map
 */
ساکن خالی check_map(خالی)
{
	صحیح	i, j, پہلا=1;
	blk64_t	blk;

	برائے (i=0; test_vec[i]; i++) {
		اگر (ext2fs_test_بلاک_bitmap2(touched_map, test_vec[i])) {
			لکھو("Bad block was touched --- %llu\n",
			       (غیرعلامتی طویل طویل) test_vec[i]);
			failed++;
			پہلا_no_comma = 1;
		}
		ext2fs_mark_بلاک_bitmap2(touched_map, test_vec[i]);
	}
	برائے (i = 0; i < test_fs->گروہ_desc_گنتی; i++) {
		برائے (j=0, blk = ext2fs_inode_جدول_loc(test_fs, i);
		     j < test_fs->inode_بلاکیں_فی_گروہ;
		     j++, blk++) {
			اگر (!ext2fs_test_بلاک_bitmap2(touched_map, blk) &&
			    !ext2fs_test_بلاک_bitmap2(bad_بلاک_map, blk)) {
				لکھو("Missing block --- %llu\n",
				       (غیرعلامتی طویل طویل) blk);
				failed++;
			}
		}
	}
	لکھو("Bad inodes: ");
	برائے (i=1; i <= test_fs->super->s_inodes_گنتی; i++) {
		اگر (ext2fs_test_inode_bitmap2(bad_inode_map, i)) {
			اگر (پہلا)
				پہلا = 0;
			ورنہ
				لکھو(", ");
			لکھو("%u", i);
		}
	}
	لکھو("\n");
}


صحیح مین(صحیح argc, حرف **argv)
{
	تیاری();
	iterate();
	check_map();
	اگر (!failed)
		لکھو("Inode scan tested OK!\n");
	واپس failed;
}

