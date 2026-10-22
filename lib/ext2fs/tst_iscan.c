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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

blk64_t test_vec[] = { 8, 12, 24, 34, 43, 44, 100, 0 };

ext2_filsys	test_fs;
ext2fs_بلاک_bitmap bad_بلاک_map, touched_map;
ext2fs_inode_bitmap bad_inode_map;
badblocks_list	test_badblocks;

صحيح پہلا_no_comma = 1;
صحيح failed = 0;

ساكن فراغ iscan_test_read_blk64(بنانشانی لما لما بلاک, صحيح گنتی, errcode_t err)
{
	صحيح	i;

	إذا (پہلا_no_comma)
		پہلا_no_comma = 0;
	وإلا
		لکھو(", ");

	إذا (گنتی > 1)
		لکھو("%llu-%llu", بلاک, بلاک+گنتی-1);
	وإلا
		لکھو("%llu", بلاک);

	لكل (i=0; i < گنتی; i++, بلاک++) {
		إذا (ext2fs_test_بلاک_bitmap2(touched_map, بلاک)) {
			لکھو("\nDuplicate block?!? --- %llu\n", بلاک);
			failed++;
			پہلا_no_comma = 1;
		}
		ext2fs_mark_بلاک_bitmap2(touched_map, بلاک);
	}
}

ساكن فراغ iscan_test_read_blk(بنانشانی لما بلاک, صحيح گنتی, errcode_t err)
{
	iscan_test_read_blk64(بلاک, گنتی, err);
}

/*
 * Setup the variables for doing the inode scan test.
 */
ساكن فراغ تیاری(فراغ)
{
	errcode_t	retval;
	صحيح		i;
	بنية ext2_super_بلاک param;

	initialize_ext2_error_جدول();

	memset(&param, 0, حجم(param));
	ext2fs_بلاکاں_گنتی_سیٹ(&param, 12000);


	test_io_cb_read_blk = iscan_test_read_blk;
	test_io_cb_read_blk64 = iscan_test_read_blk64;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_بٹ, &param,
				   test_io_manager, &test_fs);
	إذا (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		باہر_نکلو(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	إذا (retval) {
		com_err("setup", retval,
			"While allocating tables for test filesystem");
		باہر_نکلو(1);
	}
	retval = ext2fs_allocate_بلاک_bitmap(test_fs, "bad block map",
					      &bad_بلاک_map);
	إذا (retval) {
		com_err("setup", retval,
			"While allocating bad_block bitmap");
		باہر_نکلو(1);
	}
	retval = ext2fs_allocate_بلاک_bitmap(test_fs, "touched map",
					      &touched_map);
	إذا (retval) {
		com_err("setup", retval,
			"While allocating touched block bitmap");
		باہر_نکلو(1);
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "bad inode map",
					      &bad_inode_map);
	إذا (retval) {
		com_err("setup", retval,
			"While allocating bad inode bitmap");
		باہر_نکلو(1);
	}

	retval = ext2fs_badblocks_list_create(&test_badblocks, 5);
	إذا (retval) {
		com_err("setup", retval, "while creating badblocks list");
		باہر_نکلو(1);
	}
	لكل (i=0; test_vec[i]; i++) {
		retval = ext2fs_badblocks_list_جوڑو(test_badblocks, test_vec[i]);
		إذا (retval) {
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
ساكن فراغ iterate(فراغ)
{
	بنية ext2_inode inode;
	ext2_inode_جانچ	جانچ;
	errcode_t	retval;
	ext2_ino_t	ino;

	retval = ext2fs_open_inode_جانچ(test_fs, 8, &جانچ);
	إذا (retval) {
		com_err("iterate", retval, "While opening inode scan");
		باہر_نکلو(1);
	}
	لکھو("Reading blocks: ");
	retval = ext2fs_get_اگلا_inode(جانچ, &ino, &inode);
	إذا (retval) {
		com_err("iterate", retval, "while reading first inode");
		باہر_نکلو(1);
	}
	طالما (ino) {
		retval = ext2fs_get_اگلا_inode(جانچ, &ino, &inode);
		إذا (retval == EXT2_ET_BAD_بلاک_وچ_INODE_جدول) {
			ext2fs_mark_inode_bitmap2(bad_inode_map, ino);
			استمر;
		}
		إذا (retval) {
			com_err("iterate", retval,
				"while getting next inode");
			باہر_نکلو(1);
		}
	}
	لکھو("\n");
	ext2fs_close_inode_جانچ(جانچ);
}

/*
 * Verify the touched map
 */
ساكن فراغ check_map(فراغ)
{
	صحيح	i, j, پہلا=1;
	blk64_t	blk;

	لكل (i=0; test_vec[i]; i++) {
		إذا (ext2fs_test_بلاک_bitmap2(touched_map, test_vec[i])) {
			لکھو("Bad block was touched --- %llu\n",
			       (بنانشانی لما لما) test_vec[i]);
			failed++;
			پہلا_no_comma = 1;
		}
		ext2fs_mark_بلاک_bitmap2(touched_map, test_vec[i]);
	}
	لكل (i = 0; i < test_fs->گروہ_desc_گنتی; i++) {
		لكل (j=0, blk = ext2fs_inode_جدول_loc(test_fs, i);
		     j < test_fs->inode_بلاکاں_فی_گروہ;
		     j++, blk++) {
			إذا (!ext2fs_test_بلاک_bitmap2(touched_map, blk) &&
			    !ext2fs_test_بلاک_bitmap2(bad_بلاک_map, blk)) {
				لکھو("Missing block --- %llu\n",
				       (بنانشانی لما لما) blk);
				failed++;
			}
		}
	}
	لکھو("Bad inodes: ");
	لكل (i=1; i <= test_fs->super->s_inodes_گنتی; i++) {
		إذا (ext2fs_test_inode_bitmap2(bad_inode_map, i)) {
			إذا (پہلا)
				پہلا = 0;
			وإلا
				لکھو(", ");
			لکھو("%u", i);
		}
	}
	لکھو("\n");
}


صحيح مکھ(صحيح argc, حرف **argv)
{
	تیاری();
	iterate();
	check_map();
	إذا (!failed)
		لکھو("Inode scan tested OK!\n");
	أرجع failed;
}

