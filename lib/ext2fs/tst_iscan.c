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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ HAVE_ERRNO_H
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

blk64_t test_vec[] = { 8, 12, 24, 34, 43, 44, 100, 0 };

ext2_filsys	test_fs;
ext2fs_ਖੰਡ_bitmap bad_ਖੰਡ_map, touched_map;
ext2fs_inode_bitmap bad_inode_map;
badblocks_list	test_badblocks;

ਪੂਰਨ ਪਹਿਲਾ_no_comma = 1;
ਪੂਰਨ failed = 0;

ਸਥਿਰ ਖਾਲੀ iscan_test_read_blk64(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ ਖੰਡ, ਪੂਰਨ ਗਿਣਤੀ_2, errcode_t err)
{
	ਪੂਰਨ	i;

	ਜੇ (ਪਹਿਲਾ_no_comma)
		ਪਹਿਲਾ_no_comma = 0;
	ਨਹੀਂਤਾਂ
		ਲਿਖੋ(", ");

	ਜੇ (ਗਿਣਤੀ_2 > 1)
		ਲਿਖੋ("%llu-%llu", ਖੰਡ, ਖੰਡ+ਗਿਣਤੀ_2-1);
	ਨਹੀਂਤਾਂ
		ਲਿਖੋ("%llu", ਖੰਡ);

	ਲਈ (i=0; i < ਗਿਣਤੀ_2; i++, ਖੰਡ++) {
		ਜੇ (ext2fs_test_ਖੰਡ_bitmap2(touched_map, ਖੰਡ)) {
			ਲਿਖੋ("\nDuplicate block?!? --- %llu\n", ਖੰਡ);
			failed++;
			ਪਹਿਲਾ_no_comma = 1;
		}
		ext2fs_mark_ਖੰਡ_bitmap2(touched_map, ਖੰਡ);
	}
}

ਸਥਿਰ ਖਾਲੀ iscan_test_read_blk(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ, ਪੂਰਨ ਗਿਣਤੀ_2, errcode_t err)
{
	iscan_test_read_blk64(ਖੰਡ, ਗਿਣਤੀ_2, err);
}

/*
 * Setup the variables for doing the inode scan test.
 */
ਸਥਿਰ ਖਾਲੀ ਤਿਆਰੀ(ਖਾਲੀ)
{
	errcode_t	retval;
	ਪੂਰਨ		i;
	ਬਣਤਰ ext2_super_ਖੰਡ param;

	initialize_ext2_error_ਸਾਰਣੀ();

	memset(&param, 0, ਆਕਾਰ(param));
	ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(&param, 12000);


	test_io_cb_read_blk = iscan_test_read_blk;
	test_io_cb_read_blk64 = iscan_test_read_blk64;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_ਬਿੱਟ, &param,
				   test_io_manager, &test_fs);
	ਜੇ (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	ਜੇ (retval) {
		com_err("setup", retval,
			"While allocating tables for test filesystem");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	retval = ext2fs_allocate_ਖੰਡ_bitmap(test_fs, "bad block map",
					      &bad_ਖੰਡ_map);
	ਜੇ (retval) {
		com_err("setup", retval,
			"While allocating bad_block bitmap");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	retval = ext2fs_allocate_ਖੰਡ_bitmap(test_fs, "touched map",
					      &touched_map);
	ਜੇ (retval) {
		com_err("setup", retval,
			"While allocating touched block bitmap");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "bad inode map",
					      &bad_inode_map);
	ਜੇ (retval) {
		com_err("setup", retval,
			"While allocating bad inode bitmap");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}

	retval = ext2fs_badblocks_list_create(&test_badblocks, 5);
	ਜੇ (retval) {
		com_err("setup", retval, "while creating badblocks list");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਲਈ (i=0; test_vec[i]; i++) {
		retval = ext2fs_badblocks_list_ਜੋੜੋ(test_badblocks, test_vec[i]);
		ਜੇ (retval) {
			com_err("setup", retval,
				"while adding test vector %d", i);
			ਬਾਹਰ_ਨਿਕਲੋ(1);
		}
		ext2fs_mark_ਖੰਡ_bitmap2(bad_ਖੰਡ_map, test_vec[i]);
	}
	test_fs->badblocks = test_badblocks;
}

/*
 * Iterate using inode_scan
 */
ਸਥਿਰ ਖਾਲੀ iterate(ਖਾਲੀ)
{
	ਬਣਤਰ ext2_inode inode;
	ext2_inode_ਜਾਂਚ	ਜਾਂਚ;
	errcode_t	retval;
	ext2_ino_t	ino;

	retval = ext2fs_open_inode_ਜਾਂਚ(test_fs, 8, &ਜਾਂਚ);
	ਜੇ (retval) {
		com_err("iterate", retval, "While opening inode scan");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਲਿਖੋ("Reading blocks: ");
	retval = ext2fs_get_ਅਗਲਾ_inode(ਜਾਂਚ, &ino, &inode);
	ਜੇ (retval) {
		com_err("iterate", retval, "while reading first inode");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਜਦੋਂ (ino) {
		retval = ext2fs_get_ਅਗਲਾ_inode(ਜਾਂਚ, &ino, &inode);
		ਜੇ (retval == EXT2_ET_BAD_ਖੰਡ_ਵਿੱਚ_INODE_ਸਾਰਣੀ) {
			ext2fs_mark_inode_bitmap2(bad_inode_map, ino);
			ਜਾਰੀ;
		}
		ਜੇ (retval) {
			com_err("iterate", retval,
				"while getting next inode");
			ਬਾਹਰ_ਨਿਕਲੋ(1);
		}
	}
	ਲਿਖੋ("\n");
	ext2fs_close_inode_ਜਾਂਚ(ਜਾਂਚ);
}

/*
 * Verify the touched map
 */
ਸਥਿਰ ਖਾਲੀ check_map(ਖਾਲੀ)
{
	ਪੂਰਨ	i, j, ਪਹਿਲਾ=1;
	blk64_t	blk;

	ਲਈ (i=0; test_vec[i]; i++) {
		ਜੇ (ext2fs_test_ਖੰਡ_bitmap2(touched_map, test_vec[i])) {
			ਲਿਖੋ("Bad block was touched --- %llu\n",
			       (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) test_vec[i]);
			failed++;
			ਪਹਿਲਾ_no_comma = 1;
		}
		ext2fs_mark_ਖੰਡ_bitmap2(touched_map, test_vec[i]);
	}
	ਲਈ (i = 0; i < test_fs->ਸਮੂਹ_desc_ਗਿਣਤੀ; i++) {
		ਲਈ (j=0, blk = ext2fs_inode_ਸਾਰਣੀ_loc(test_fs, i);
		     j < test_fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ;
		     j++, blk++) {
			ਜੇ (!ext2fs_test_ਖੰਡ_bitmap2(touched_map, blk) &&
			    !ext2fs_test_ਖੰਡ_bitmap2(bad_ਖੰਡ_map, blk)) {
				ਲਿਖੋ("Missing block --- %llu\n",
				       (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) blk);
				failed++;
			}
		}
	}
	ਲਿਖੋ("Bad inodes: ");
	ਲਈ (i=1; i <= test_fs->super->s_inodes_ਗਿਣਤੀ; i++) {
		ਜੇ (ext2fs_test_inode_bitmap2(bad_inode_map, i)) {
			ਜੇ (ਪਹਿਲਾ)
				ਪਹਿਲਾ = 0;
			ਨਹੀਂਤਾਂ
				ਲਿਖੋ(", ");
			ਲਿਖੋ("%u", i);
		}
	}
	ਲਿਖੋ("\n");
}


ਪੂਰਨ ਮੁੱਖ(ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
	ਤਿਆਰੀ();
	iterate();
	check_map();
	ਜੇ (!failed)
		ਲਿਖੋ("Inode scan tested OK!\n");
	ਵਾਪਸ failed;
}

