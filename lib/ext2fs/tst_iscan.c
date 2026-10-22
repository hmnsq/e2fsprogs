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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

blk64_t test_vec[] = { 8, 12, 24, 34, 43, 44, 100, 0 };

ext2_filsys	test_fs;
ext2fs_ಖಂಡ_bitmap bad_ಖಂಡ_map, touched_map;
ext2fs_inode_bitmap bad_inode_map;
badblocks_list	test_badblocks;

ಪೂರ್ಣಾಂಕ ಮೊದಲ_no_comma = 1;
ಪೂರ್ಣಾಂಕ failed = 0;

ಸ್ಥಿರ ಶೂನ್ಯ iscan_test_read_blk64(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ ಖಂಡ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, errcode_t err)
{
	ಪೂರ್ಣಾಂಕ	i;

	ಆದರೆ (ಮೊದಲ_no_comma)
		ಮೊದಲ_no_comma = 0;
	ಇಲ್ಲದಿದ್ದರೆ
		ಬರೆ(", ");

	ಆದರೆ (ಎಣಿಕೆ_2 > 1)
		ಬರೆ("%llu-%llu", ಖಂಡ, ಖಂಡ+ಎಣಿಕೆ_2-1);
	ಇಲ್ಲದಿದ್ದರೆ
		ಬರೆ("%llu", ಖಂಡ);

	ಗಾಗಿ (i=0; i < ಎಣಿಕೆ_2; i++, ಖಂಡ++) {
		ಆದರೆ (ext2fs_test_ಖಂಡ_bitmap2(touched_map, ಖಂಡ)) {
			ಬರೆ("\nDuplicate block?!? --- %llu\n", ಖಂಡ);
			failed++;
			ಮೊದಲ_no_comma = 1;
		}
		ext2fs_mark_ಖಂಡ_bitmap2(touched_map, ಖಂಡ);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ iscan_test_read_blk(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, errcode_t err)
{
	iscan_test_read_blk64(ಖಂಡ, ಎಣಿಕೆ_2, err);
}

/*
 * Setup the variables for doing the inode scan test.
 */
ಸ್ಥಿರ ಶೂನ್ಯ ಸಿದ್ಧತೆ(ಶೂನ್ಯ)
{
	errcode_t	retval;
	ಪೂರ್ಣಾಂಕ		i;
	ರಚನೆ ext2_super_ಖಂಡ param;

	initialize_ext2_error_ಕೋಷ್ಟಕ();

	memset(&param, 0, ಗಾತ್ರ(param));
	ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(&param, 12000);


	test_io_cb_read_blk = iscan_test_read_blk;
	test_io_cb_read_blk64 = iscan_test_read_blk64;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_ಬಿಟ್ಟುಗಳು, &param,
				   test_io_manager, &test_fs);
	ಆದರೆ (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		ನಿರ್ಗಮಿಸು(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	ಆದರೆ (retval) {
		com_err("setup", retval,
			"While allocating tables for test filesystem");
		ನಿರ್ಗಮಿಸು(1);
	}
	retval = ext2fs_allocate_ಖಂಡ_bitmap(test_fs, "bad block map",
					      &bad_ಖಂಡ_map);
	ಆದರೆ (retval) {
		com_err("setup", retval,
			"While allocating bad_block bitmap");
		ನಿರ್ಗಮಿಸು(1);
	}
	retval = ext2fs_allocate_ಖಂಡ_bitmap(test_fs, "touched map",
					      &touched_map);
	ಆದರೆ (retval) {
		com_err("setup", retval,
			"While allocating touched block bitmap");
		ನಿರ್ಗಮಿಸು(1);
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "bad inode map",
					      &bad_inode_map);
	ಆದರೆ (retval) {
		com_err("setup", retval,
			"While allocating bad inode bitmap");
		ನಿರ್ಗಮಿಸು(1);
	}

	retval = ext2fs_badblocks_list_create(&test_badblocks, 5);
	ಆದರೆ (retval) {
		com_err("setup", retval, "while creating badblocks list");
		ನಿರ್ಗಮಿಸು(1);
	}
	ಗಾಗಿ (i=0; test_vec[i]; i++) {
		retval = ext2fs_badblocks_list_ಕೂಡಿಸು(test_badblocks, test_vec[i]);
		ಆದರೆ (retval) {
			com_err("setup", retval,
				"while adding test vector %d", i);
			ನಿರ್ಗಮಿಸು(1);
		}
		ext2fs_mark_ಖಂಡ_bitmap2(bad_ಖಂಡ_map, test_vec[i]);
	}
	test_fs->badblocks = test_badblocks;
}

/*
 * Iterate using inode_scan
 */
ಸ್ಥಿರ ಶೂನ್ಯ iterate(ಶೂನ್ಯ)
{
	ರಚನೆ ext2_inode inode;
	ext2_inode_ಪರಿಶೀಲನೆ	ಪರಿಶೀಲನೆ;
	errcode_t	retval;
	ext2_ino_t	ino;

	retval = ext2fs_open_inode_ಪರಿಶೀಲನೆ(test_fs, 8, &ಪರಿಶೀಲನೆ);
	ಆದರೆ (retval) {
		com_err("iterate", retval, "While opening inode scan");
		ನಿರ್ಗಮಿಸು(1);
	}
	ಬರೆ("Reading blocks: ");
	retval = ext2fs_get_ಮುಂದಿನ_inode(ಪರಿಶೀಲನೆ, &ino, &inode);
	ಆದರೆ (retval) {
		com_err("iterate", retval, "while reading first inode");
		ನಿರ್ಗಮಿಸು(1);
	}
	ಆಗ (ino) {
		retval = ext2fs_get_ಮುಂದಿನ_inode(ಪರಿಶೀಲನೆ, &ino, &inode);
		ಆದರೆ (retval == EXT2_ET_BAD_ಖಂಡ_ಒಳ_INODE_ಕೋಷ್ಟಕ) {
			ext2fs_mark_inode_bitmap2(bad_inode_map, ino);
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (retval) {
			com_err("iterate", retval,
				"while getting next inode");
			ನಿರ್ಗಮಿಸು(1);
		}
	}
	ಬರೆ("\n");
	ext2fs_close_inode_ಪರಿಶೀಲನೆ(ಪರಿಶೀಲನೆ);
}

/*
 * Verify the touched map
 */
ಸ್ಥಿರ ಶೂನ್ಯ check_map(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ	i, j, ಮೊದಲ=1;
	blk64_t	blk;

	ಗಾಗಿ (i=0; test_vec[i]; i++) {
		ಆದರೆ (ext2fs_test_ಖಂಡ_bitmap2(touched_map, test_vec[i])) {
			ಬರೆ("Bad block was touched --- %llu\n",
			       (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) test_vec[i]);
			failed++;
			ಮೊದಲ_no_comma = 1;
		}
		ext2fs_mark_ಖಂಡ_bitmap2(touched_map, test_vec[i]);
	}
	ಗಾಗಿ (i = 0; i < test_fs->ಗುಂಪು_desc_ಎಣಿಕೆ; i++) {
		ಗಾಗಿ (j=0, blk = ext2fs_inode_ಕೋಷ್ಟಕ_loc(test_fs, i);
		     j < test_fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು;
		     j++, blk++) {
			ಆದರೆ (!ext2fs_test_ಖಂಡ_bitmap2(touched_map, blk) &&
			    !ext2fs_test_ಖಂಡ_bitmap2(bad_ಖಂಡ_map, blk)) {
				ಬರೆ("Missing block --- %llu\n",
				       (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) blk);
				failed++;
			}
		}
	}
	ಬರೆ("Bad inodes: ");
	ಗಾಗಿ (i=1; i <= test_fs->super->s_inodes_ಎಣಿಕೆ; i++) {
		ಆದರೆ (ext2fs_test_inode_bitmap2(bad_inode_map, i)) {
			ಆದರೆ (ಮೊದಲ)
				ಮೊದಲ = 0;
			ಇಲ್ಲದಿದ್ದರೆ
				ಬರೆ(", ");
			ಬರೆ("%u", i);
		}
	}
	ಬರೆ("\n");
}


ಪೂರ್ಣಾಂಕ ಮುಖ್ಯ(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
	ಸಿದ್ಧತೆ();
	iterate();
	check_map();
	ಆದರೆ (!failed)
		ಬರೆ("Inode scan tested OK!\n");
	ಹಿಂತಿರುಗಿ failed;
}

