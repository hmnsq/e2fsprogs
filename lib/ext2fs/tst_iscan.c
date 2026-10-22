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
#پایان_اگر
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

blk64_t test_vec[] = { 8, 12, 24, 34, 43, 44, 100, 0 };

ext2_filsys	test_fs;
ext2fs_بلوک_bitmap bad_بلوک_map, touched_map;
ext2fs_inode_bitmap bad_inode_map;
badblocks_list	test_badblocks;

صحيح اولی_no_comma = 1;
صحيح failed = 0;

ساكن فراغ iscan_test_read_blk64(بیعلامت دراز دراز بلوک, صحيح شمار, errcode_t err)
{
	صحيح	i;

	إذا (اولی_no_comma)
		اولی_no_comma = 0;
	وإلا
		چاپ_کن(", ");

	إذا (شمار > 1)
		چاپ_کن("%llu-%llu", بلوک, بلوک+شمار-1);
	وإلا
		چاپ_کن("%llu", بلوک);

	لكل (i=0; i < شمار; i++, بلوک++) {
		إذا (ext2fs_test_بلوک_bitmap2(touched_map, بلوک)) {
			چاپ_کن("\nDuplicate block?!? --- %llu\n", بلوک);
			failed++;
			اولی_no_comma = 1;
		}
		ext2fs_mark_بلوک_bitmap2(touched_map, بلوک);
	}
}

ساكن فراغ iscan_test_read_blk(بیعلامت دراز بلوک, صحيح شمار, errcode_t err)
{
	iscan_test_read_blk64(بلوک, شمار, err);
}

/*
 * Setup the variables for doing the inode scan test.
 */
ساكن فراغ راهاندازی(فراغ)
{
	errcode_t	retval;
	صحيح		i;
	بنية ext2_super_بلوک param;

	initialize_ext2_error_جدول();

	memset(&param, 0, حجم(param));
	ext2fs_بلوکها_شمار_مجموعه(&param, 12000);


	test_io_cb_read_blk = iscan_test_read_blk;
	test_io_cb_read_blk64 = iscan_test_read_blk64;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_بیتها, &param,
				   test_io_manager, &test_fs);
	إذا (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		خروج(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	إذا (retval) {
		com_err("setup", retval,
			"While allocating tables for test filesystem");
		خروج(1);
	}
	retval = ext2fs_allocate_بلوک_bitmap(test_fs, "bad block map",
					      &bad_بلوک_map);
	إذا (retval) {
		com_err("setup", retval,
			"While allocating bad_block bitmap");
		خروج(1);
	}
	retval = ext2fs_allocate_بلوک_bitmap(test_fs, "touched map",
					      &touched_map);
	إذا (retval) {
		com_err("setup", retval,
			"While allocating touched block bitmap");
		خروج(1);
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "bad inode map",
					      &bad_inode_map);
	إذا (retval) {
		com_err("setup", retval,
			"While allocating bad inode bitmap");
		خروج(1);
	}

	retval = ext2fs_badblocks_list_create(&test_badblocks, 5);
	إذا (retval) {
		com_err("setup", retval, "while creating badblocks list");
		خروج(1);
	}
	لكل (i=0; test_vec[i]; i++) {
		retval = ext2fs_badblocks_list_جمع_کن(test_badblocks, test_vec[i]);
		إذا (retval) {
			com_err("setup", retval,
				"while adding test vector %d", i);
			خروج(1);
		}
		ext2fs_mark_بلوک_bitmap2(bad_بلوک_map, test_vec[i]);
	}
	test_fs->badblocks = test_badblocks;
}

/*
 * Iterate using inode_scan
 */
ساكن فراغ iterate(فراغ)
{
	بنية ext2_inode inode;
	ext2_inode_پیمایش	پیمایش;
	errcode_t	retval;
	ext2_ino_t	ino;

	retval = ext2fs_open_inode_پیمایش(test_fs, 8, &پیمایش);
	إذا (retval) {
		com_err("iterate", retval, "While opening inode scan");
		خروج(1);
	}
	چاپ_کن("Reading blocks: ");
	retval = ext2fs_get_بعدی_inode(پیمایش, &ino, &inode);
	إذا (retval) {
		com_err("iterate", retval, "while reading first inode");
		خروج(1);
	}
	طالما (ino) {
		retval = ext2fs_get_بعدی_inode(پیمایش, &ino, &inode);
		إذا (retval == EXT2_ET_BAD_بلوک_درون_INODE_جدول) {
			ext2fs_mark_inode_bitmap2(bad_inode_map, ino);
			استمر;
		}
		إذا (retval) {
			com_err("iterate", retval,
				"while getting next inode");
			خروج(1);
		}
	}
	چاپ_کن("\n");
	ext2fs_close_inode_پیمایش(پیمایش);
}

/*
 * Verify the touched map
 */
ساكن فراغ check_map(فراغ)
{
	صحيح	i, j, اولی=1;
	blk64_t	blk;

	لكل (i=0; test_vec[i]; i++) {
		إذا (ext2fs_test_بلوک_bitmap2(touched_map, test_vec[i])) {
			چاپ_کن("Bad block was touched --- %llu\n",
			       (بیعلامت دراز دراز) test_vec[i]);
			failed++;
			اولی_no_comma = 1;
		}
		ext2fs_mark_بلوک_bitmap2(touched_map, test_vec[i]);
	}
	لكل (i = 0; i < test_fs->گروه_desc_شمار; i++) {
		لكل (j=0, blk = ext2fs_inode_جدول_loc(test_fs, i);
		     j < test_fs->inode_بلوکها_در_هر_گروه;
		     j++, blk++) {
			إذا (!ext2fs_test_بلوک_bitmap2(touched_map, blk) &&
			    !ext2fs_test_بلوک_bitmap2(bad_بلوک_map, blk)) {
				چاپ_کن("Missing block --- %llu\n",
				       (بیعلامت دراز دراز) blk);
				failed++;
			}
		}
	}
	چاپ_کن("Bad inodes: ");
	لكل (i=1; i <= test_fs->super->s_inodes_شمار; i++) {
		إذا (ext2fs_test_inode_bitmap2(bad_inode_map, i)) {
			إذا (اولی)
				اولی = 0;
			وإلا
				چاپ_کن(", ");
			چاپ_کن("%u", i);
		}
	}
	چاپ_کن("\n");
}


صحيح اصلی(صحيح argc, حرف **argv)
{
	راهاندازی();
	iterate();
	check_map();
	إذا (!failed)
		چاپ_کن("Inode scan tested OK!\n");
	أرجع failed;
}

