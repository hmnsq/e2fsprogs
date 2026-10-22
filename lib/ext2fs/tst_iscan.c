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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <sys/types.h>
#ఒకవేళ HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

blk64_t test_vec[] = { 8, 12, 24, 34, 43, 44, 100, 0 };

ext2_filsys	test_fs;
ext2fs_ఖండం_bitmap bad_ఖండం_map, touched_map;
ext2fs_inode_bitmap bad_inode_map;
badblocks_list	test_badblocks;

పూర్ణాంకం మొదటి_no_comma = 1;
పూర్ణాంకం failed = 0;

స్థిర శూన్య iscan_test_read_blk64(గుర్తులేని దీర్ఘ దీర్ఘ ఖండం, పూర్ణాంకం లెక్క, errcode_t err)
{
	పూర్ణాంకం	i;

	ఒకవేళ (మొదటి_no_comma)
		మొదటి_no_comma = 0;
	లేకపోతే
		రాయి(", ");

	ఒకవేళ (లెక్క > 1)
		రాయి("%llu-%llu", ఖండం, ఖండం+లెక్క-1);
	లేకపోతే
		రాయి("%llu", ఖండం);

	కొరకు (i=0; i < లెక్క; i++, ఖండం++) {
		ఒకవేళ (ext2fs_test_ఖండం_bitmap2(touched_map, ఖండం)) {
			రాయి("\nDuplicate block?!? --- %llu\n", ఖండం);
			failed++;
			మొదటి_no_comma = 1;
		}
		ext2fs_mark_ఖండం_bitmap2(touched_map, ఖండం);
	}
}

స్థిర శూన్య iscan_test_read_blk(గుర్తులేని దీర్ఘ ఖండం, పూర్ణాంకం లెక్క, errcode_t err)
{
	iscan_test_read_blk64(ఖండం, లెక్క, err);
}

/*
 * Setup the variables for doing the inode scan test.
 */
స్థిర శూన్య అమరిక(శూన్య)
{
	errcode_t	retval;
	పూర్ణాంకం		i;
	నిర్మాణం ext2_super_ఖండం param;

	initialize_ext2_error_పట్టిక();

	memset(&param, 0, పరిమాణం(param));
	ext2fs_ఖండాలు_లెక్క_సమితి(&param, 12000);


	test_io_cb_read_blk = iscan_test_read_blk;
	test_io_cb_read_blk64 = iscan_test_read_blk64;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_బిట్లు, &param,
				   test_io_manager, &test_fs);
	ఒకవేళ (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		నిష్క్రమించు(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	ఒకవేళ (retval) {
		com_err("setup", retval,
			"While allocating tables for test filesystem");
		నిష్క్రమించు(1);
	}
	retval = ext2fs_allocate_ఖండం_bitmap(test_fs, "bad block map",
					      &bad_ఖండం_map);
	ఒకవేళ (retval) {
		com_err("setup", retval,
			"While allocating bad_block bitmap");
		నిష్క్రమించు(1);
	}
	retval = ext2fs_allocate_ఖండం_bitmap(test_fs, "touched map",
					      &touched_map);
	ఒకవేళ (retval) {
		com_err("setup", retval,
			"While allocating touched block bitmap");
		నిష్క్రమించు(1);
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "bad inode map",
					      &bad_inode_map);
	ఒకవేళ (retval) {
		com_err("setup", retval,
			"While allocating bad inode bitmap");
		నిష్క్రమించు(1);
	}

	retval = ext2fs_badblocks_list_create(&test_badblocks, 5);
	ఒకవేళ (retval) {
		com_err("setup", retval, "while creating badblocks list");
		నిష్క్రమించు(1);
	}
	కొరకు (i=0; test_vec[i]; i++) {
		retval = ext2fs_badblocks_list_కూడు(test_badblocks, test_vec[i]);
		ఒకవేళ (retval) {
			com_err("setup", retval,
				"while adding test vector %d", i);
			నిష్క్రమించు(1);
		}
		ext2fs_mark_ఖండం_bitmap2(bad_ఖండం_map, test_vec[i]);
	}
	test_fs->badblocks = test_badblocks;
}

/*
 * Iterate using inode_scan
 */
స్థిర శూన్య iterate(శూన్య)
{
	నిర్మాణం ext2_inode inode;
	ext2_inode_పరిశీలన	పరిశీలన;
	errcode_t	retval;
	ext2_ino_t	ino;

	retval = ext2fs_open_inode_పరిశీలన(test_fs, 8, &పరిశీలన);
	ఒకవేళ (retval) {
		com_err("iterate", retval, "While opening inode scan");
		నిష్క్రమించు(1);
	}
	రాయి("Reading blocks: ");
	retval = ext2fs_get_తదుపరి_inode(పరిశీలన, &ino, &inode);
	ఒకవేళ (retval) {
		com_err("iterate", retval, "while reading first inode");
		నిష్క్రమించు(1);
	}
	అయితే (ino) {
		retval = ext2fs_get_తదుపరి_inode(పరిశీలన, &ino, &inode);
		ఒకవేళ (retval == EXT2_ET_BAD_ఖండం_లోపల_INODE_పట్టిక) {
			ext2fs_mark_inode_bitmap2(bad_inode_map, ino);
			కొనసాగు;
		}
		ఒకవేళ (retval) {
			com_err("iterate", retval,
				"while getting next inode");
			నిష్క్రమించు(1);
		}
	}
	రాయి("\n");
	ext2fs_close_inode_పరిశీలన(పరిశీలన);
}

/*
 * Verify the touched map
 */
స్థిర శూన్య check_map(శూన్య)
{
	పూర్ణాంకం	i, j, మొదటి=1;
	blk64_t	blk;

	కొరకు (i=0; test_vec[i]; i++) {
		ఒకవేళ (ext2fs_test_ఖండం_bitmap2(touched_map, test_vec[i])) {
			రాయి("Bad block was touched --- %llu\n",
			       (గుర్తులేని దీర్ఘ దీర్ఘ) test_vec[i]);
			failed++;
			మొదటి_no_comma = 1;
		}
		ext2fs_mark_ఖండం_bitmap2(touched_map, test_vec[i]);
	}
	కొరకు (i = 0; i < test_fs->సమూహం_desc_లెక్క; i++) {
		కొరకు (j=0, blk = ext2fs_inode_పట్టిక_loc(test_fs, i);
		     j < test_fs->inode_ఖండాలు_చొప్పున_సమూహం;
		     j++, blk++) {
			ఒకవేళ (!ext2fs_test_ఖండం_bitmap2(touched_map, blk) &&
			    !ext2fs_test_ఖండం_bitmap2(bad_ఖండం_map, blk)) {
				రాయి("Missing block --- %llu\n",
				       (గుర్తులేని దీర్ఘ దీర్ఘ) blk);
				failed++;
			}
		}
	}
	రాయి("Bad inodes: ");
	కొరకు (i=1; i <= test_fs->super->s_inodes_లెక్క; i++) {
		ఒకవేళ (ext2fs_test_inode_bitmap2(bad_inode_map, i)) {
			ఒకవేళ (మొదటి)
				మొదటి = 0;
			లేకపోతే
				రాయి(", ");
			రాయి("%u", i);
		}
	}
	రాయి("\n");
}


పూర్ణాంకం ప్రధాన(పూర్ణాంకం argc, అక్షరం **argv)
{
	అమరిక();
	iterate();
	check_map();
	ఒకవేళ (!failed)
		రాయి("Inode scan tested OK!\n");
	తిరిగి failed;
}

