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
#جيڪڏهن_پورو
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

blk64_t test_vec[] = { 8, 12, 24, 34, 43, 44, 100, 0 };

ext2_filsys	test_fs;
ext2fs_بلاڪ_bitmap bad_بلاڪ_map, touched_map;
ext2fs_inode_bitmap bad_inode_map;
badblocks_list	test_badblocks;

صحيح پهريون_no_comma = 1;
صحيح failed = 0;

ساكن فراغ iscan_test_read_blk64(بنانشان ڊگهو ڊگهو بلاڪ, صحيح ڳڻپ, errcode_t err)
{
	صحيح	i;

	إذا (پهريون_no_comma)
		پهريون_no_comma = 0;
	وإلا
		لکو(", ");

	إذا (ڳڻپ > 1)
		لکو("%llu-%llu", بلاڪ, بلاڪ+ڳڻپ-1);
	وإلا
		لکو("%llu", بلاڪ);

	لكل (i=0; i < ڳڻپ; i++, بلاڪ++) {
		إذا (ext2fs_test_بلاڪ_bitmap2(touched_map, بلاڪ)) {
			لکو("\nDuplicate block?!? --- %llu\n", بلاڪ);
			failed++;
			پهريون_no_comma = 1;
		}
		ext2fs_mark_بلاڪ_bitmap2(touched_map, بلاڪ);
	}
}

ساكن فراغ iscan_test_read_blk(بنانشان ڊگهو بلاڪ, صحيح ڳڻپ, errcode_t err)
{
	iscan_test_read_blk64(بلاڪ, ڳڻپ, err);
}

/*
 * Setup the variables for doing the inode scan test.
 */
ساكن فراغ تياري(فراغ)
{
	errcode_t	retval;
	صحيح		i;
	بنية ext2_super_بلاڪ param;

	initialize_ext2_error_جدول();

	memset(&param, 0, حجم(param));
	ext2fs_بلاڪون_ڳڻپ_سيٽ(&param, 12000);


	test_io_cb_read_blk = iscan_test_read_blk;
	test_io_cb_read_blk64 = iscan_test_read_blk64;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_بٽ, &param,
				   test_io_manager, &test_fs);
	إذا (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		ٻاهر_نڪرو(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	إذا (retval) {
		com_err("setup", retval,
			"While allocating tables for test filesystem");
		ٻاهر_نڪرو(1);
	}
	retval = ext2fs_allocate_بلاڪ_bitmap(test_fs, "bad block map",
					      &bad_بلاڪ_map);
	إذا (retval) {
		com_err("setup", retval,
			"While allocating bad_block bitmap");
		ٻاهر_نڪرو(1);
	}
	retval = ext2fs_allocate_بلاڪ_bitmap(test_fs, "touched map",
					      &touched_map);
	إذا (retval) {
		com_err("setup", retval,
			"While allocating touched block bitmap");
		ٻاهر_نڪرو(1);
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "bad inode map",
					      &bad_inode_map);
	إذا (retval) {
		com_err("setup", retval,
			"While allocating bad inode bitmap");
		ٻاهر_نڪرو(1);
	}

	retval = ext2fs_badblocks_list_create(&test_badblocks, 5);
	إذا (retval) {
		com_err("setup", retval, "while creating badblocks list");
		ٻاهر_نڪرو(1);
	}
	لكل (i=0; test_vec[i]; i++) {
		retval = ext2fs_badblocks_list_جوڙيو(test_badblocks, test_vec[i]);
		إذا (retval) {
			com_err("setup", retval,
				"while adding test vector %d", i);
			ٻاهر_نڪرو(1);
		}
		ext2fs_mark_بلاڪ_bitmap2(bad_بلاڪ_map, test_vec[i]);
	}
	test_fs->badblocks = test_badblocks;
}

/*
 * Iterate using inode_scan
 */
ساكن فراغ iterate(فراغ)
{
	بنية ext2_inode inode;
	ext2_inode_جاچ	جاچ;
	errcode_t	retval;
	ext2_ino_t	ino;

	retval = ext2fs_open_inode_جاچ(test_fs, 8, &جاچ);
	إذا (retval) {
		com_err("iterate", retval, "While opening inode scan");
		ٻاهر_نڪرو(1);
	}
	لکو("Reading blocks: ");
	retval = ext2fs_get_ايندڙ_inode(جاچ, &ino, &inode);
	إذا (retval) {
		com_err("iterate", retval, "while reading first inode");
		ٻاهر_نڪرو(1);
	}
	طالما (ino) {
		retval = ext2fs_get_ايندڙ_inode(جاچ, &ino, &inode);
		إذا (retval == EXT2_ET_BAD_بلاڪ_اندريون_INODE_جدول) {
			ext2fs_mark_inode_bitmap2(bad_inode_map, ino);
			استمر;
		}
		إذا (retval) {
			com_err("iterate", retval,
				"while getting next inode");
			ٻاهر_نڪرو(1);
		}
	}
	لکو("\n");
	ext2fs_close_inode_جاچ(جاچ);
}

/*
 * Verify the touched map
 */
ساكن فراغ check_map(فراغ)
{
	صحيح	i, j, پهريون=1;
	blk64_t	blk;

	لكل (i=0; test_vec[i]; i++) {
		إذا (ext2fs_test_بلاڪ_bitmap2(touched_map, test_vec[i])) {
			لکو("Bad block was touched --- %llu\n",
			       (بنانشان ڊگهو ڊگهو) test_vec[i]);
			failed++;
			پهريون_no_comma = 1;
		}
		ext2fs_mark_بلاڪ_bitmap2(touched_map, test_vec[i]);
	}
	لكل (i = 0; i < test_fs->گروهه_desc_ڳڻپ; i++) {
		لكل (j=0, blk = ext2fs_inode_جدول_loc(test_fs, i);
		     j < test_fs->inode_بلاڪون_في_گروهه;
		     j++, blk++) {
			إذا (!ext2fs_test_بلاڪ_bitmap2(touched_map, blk) &&
			    !ext2fs_test_بلاڪ_bitmap2(bad_بلاڪ_map, blk)) {
				لکو("Missing block --- %llu\n",
				       (بنانشان ڊگهو ڊگهو) blk);
				failed++;
			}
		}
	}
	لکو("Bad inodes: ");
	لكل (i=1; i <= test_fs->super->s_inodes_ڳڻپ; i++) {
		إذا (ext2fs_test_inode_bitmap2(bad_inode_map, i)) {
			إذا (پهريون)
				پهريون = 0;
			وإلا
				لکو(", ");
			لکو("%u", i);
		}
	}
	لکو("\n");
}


صحيح مکيه(صحيح argc, حرف **argv)
{
	تياري();
	iterate();
	check_map();
	إذا (!failed)
		لکو("Inode scan tested OK!\n");
	أرجع failed;
}

