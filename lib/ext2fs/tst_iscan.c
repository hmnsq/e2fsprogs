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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <time.h>
#অন্তর্ভাৱ <sys/stat.h>
#অন্তর্ভাৱ <sys/types.h>
#যদি HAVE_ERRNO_H
#অন্তর্ভাৱ <errno.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"

blk64_t test_vec[] = { 8, 12, 24, 34, 43, 44, 100, 0 };

ext2_filsys	test_fs;
ext2fs_block_bitmap bad_block_map, touched_map;
ext2fs_inode_bitmap bad_inode_map;
badblocks_list	test_badblocks;

পূর্ণাঙ্ক first_no_comma = 1;
পূর্ণাঙ্ক failed = 0;

স্থির শূন্য iscan_test_read_blk64(unsigned long long block, পূর্ণাঙ্ক গুনতি, errcode_t err)
{
	পূর্ণাঙ্ক	i;

	যদি (first_no_comma)
		first_no_comma = 0;
	অন্যথা
		printf(", ");

	যদি (গুনতি > 1)
		printf("%llu-%llu", block, block+গুনতি-1);
	অন্যথা
		printf("%llu", block);

	বাবে (i=0; i < গুনতি; i++, block++) {
		যদি (ext2fs_test_block_bitmap2(touched_map, block)) {
			printf("\nDuplicate block?!? --- %llu\n", block);
			failed++;
			first_no_comma = 1;
		}
		ext2fs_mark_block_bitmap2(touched_map, block);
	}
}

স্থির শূন্য iscan_test_read_blk(unsigned long block, পূর্ণাঙ্ক গুনতি, errcode_t err)
{
	iscan_test_read_blk64(block, গুনতি, err);
}

/*
 * Setup the variables for doing the inode scan test.
 */
স্থির শূন্য setup(শূন্য)
{
	errcode_t	retval;
	পূর্ণাঙ্ক		i;
	সংরচনা ext2_super_block param;

	initialize_ext2_error_table();

	memset(&param, 0, পরিমাণ(param));
	ext2fs_blocks_গুনতি_set(&param, 12000);


	test_io_cb_read_blk = iscan_test_read_blk;
	test_io_cb_read_blk64 = iscan_test_read_blk64;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &test_fs);
	যদি (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		exit(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	যদি (retval) {
		com_err("setup", retval,
			"While allocating tables for test filesystem");
		exit(1);
	}
	retval = ext2fs_allocate_block_bitmap(test_fs, "bad block map",
					      &bad_block_map);
	যদি (retval) {
		com_err("setup", retval,
			"While allocating bad_block bitmap");
		exit(1);
	}
	retval = ext2fs_allocate_block_bitmap(test_fs, "touched map",
					      &touched_map);
	যদি (retval) {
		com_err("setup", retval,
			"While allocating touched block bitmap");
		exit(1);
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "bad inode map",
					      &bad_inode_map);
	যদি (retval) {
		com_err("setup", retval,
			"While allocating bad inode bitmap");
		exit(1);
	}

	retval = ext2fs_badblocks_list_create(&test_badblocks, 5);
	যদি (retval) {
		com_err("setup", retval, "while creating badblocks list");
		exit(1);
	}
	বাবে (i=0; test_vec[i]; i++) {
		retval = ext2fs_badblocks_list_add(test_badblocks, test_vec[i]);
		যদি (retval) {
			com_err("setup", retval,
				"while adding test vector %d", i);
			exit(1);
		}
		ext2fs_mark_block_bitmap2(bad_block_map, test_vec[i]);
	}
	test_fs->badblocks = test_badblocks;
}

/*
 * Iterate using inode_scan
 */
স্থির শূন্য iterate(শূন্য)
{
	সংরচনা ext2_inode inode;
	ext2_inode_scan	scan;
	errcode_t	retval;
	ext2_ino_t	ino;

	retval = ext2fs_open_inode_scan(test_fs, 8, &scan);
	যদি (retval) {
		com_err("iterate", retval, "While opening inode scan");
		exit(1);
	}
	printf("Reading blocks: ");
	retval = ext2fs_get_next_inode(scan, &ino, &inode);
	যদি (retval) {
		com_err("iterate", retval, "while reading first inode");
		exit(1);
	}
	যেতিয়ালৈ (ino) {
		retval = ext2fs_get_next_inode(scan, &ino, &inode);
		যদি (retval == EXT2_ET_BAD_BLOCK_IN_INODE_TABLE) {
			ext2fs_mark_inode_bitmap2(bad_inode_map, ino);
			অনুৱর্ত;
		}
		যদি (retval) {
			com_err("iterate", retval,
				"while getting next inode");
			exit(1);
		}
	}
	printf("\n");
	ext2fs_close_inode_scan(scan);
}

/*
 * Verify the touched map
 */
স্থির শূন্য check_map(শূন্য)
{
	পূর্ণাঙ্ক	i, j, first=1;
	blk64_t	blk;

	বাবে (i=0; test_vec[i]; i++) {
		যদি (ext2fs_test_block_bitmap2(touched_map, test_vec[i])) {
			printf("Bad block was touched --- %llu\n",
			       (unsigned long long) test_vec[i]);
			failed++;
			first_no_comma = 1;
		}
		ext2fs_mark_block_bitmap2(touched_map, test_vec[i]);
	}
	বাবে (i = 0; i < test_fs->group_desc_গুনতি; i++) {
		বাবে (j=0, blk = ext2fs_inode_table_loc(test_fs, i);
		     j < test_fs->inode_blocks_per_group;
		     j++, blk++) {
			যদি (!ext2fs_test_block_bitmap2(touched_map, blk) &&
			    !ext2fs_test_block_bitmap2(bad_block_map, blk)) {
				printf("Missing block --- %llu\n",
				       (unsigned long long) blk);
				failed++;
			}
		}
	}
	printf("Bad inodes: ");
	বাবে (i=1; i <= test_fs->super->s_inodes_গুনতি; i++) {
		যদি (ext2fs_test_inode_bitmap2(bad_inode_map, i)) {
			যদি (first)
				first = 0;
			অন্যথা
				printf(", ");
			printf("%u", i);
		}
	}
	printf("\n");
}


পূর্ণাঙ্ক main(পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
	setup();
	iterate();
	check_map();
	যদি (!failed)
		printf("Inode scan tested OK!\n");
	ঘূৰাই failed;
}

