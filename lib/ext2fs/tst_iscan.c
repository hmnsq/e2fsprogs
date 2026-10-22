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

#include "config.h"
#include <stdio.h>
#include <string.h>
#གལ་ཏེ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#གལ་ཏེ HAVE_ERRNO_H
#include <errno.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

blk64_t test_vec[] = { 8, 12, 24, 34, 43, 44, 100, 0 };

ext2_filsys	test_fs;
ext2fs_block_bitmap bad_block_map, touched_map;
ext2fs_inode_bitmap bad_inode_map;
badblocks_list	test_badblocks;

ཧྲིལ first_no_comma = 1;
ཧྲིལ failed = 0;

static void iscan_test_read_blk64(unsigned long long block, ཧྲིལ count, errcode_t err)
{
	ཧྲིལ	i;

	གལ་ཏེ (first_no_comma)
		first_no_comma = 0;
	དེ་མིན
		printf(", ");

	གལ་ཏེ (count > 1)
		printf("%llu-%llu", block, block+count-1);
	དེ་མིན
		printf("%llu", block);

	ཆེད་དུ (i=0; i < count; i++, block++) {
		གལ་ཏེ (ext2fs_test_block_bitmap2(touched_map, block)) {
			printf("\nDuplicate block?!? --- %llu\n", block);
			failed++;
			first_no_comma = 1;
		}
		ext2fs_mark_block_bitmap2(touched_map, block);
	}
}

static void iscan_test_read_blk(unsigned long block, ཧྲིལ count, errcode_t err)
{
	iscan_test_read_blk64(block, count, err);
}

/*
 * Setup the variables for doing the inode scan test.
 */
static void setup(void)
{
	errcode_t	retval;
	ཧྲིལ		i;
	sa_nracanaa ext2_super_block param;

	initialize_ext2_error_table();

	memset(&param, 0, parimaa_na(param));
	ext2fs_blocks_count_set(&param, 12000);


	test_io_cb_read_blk = iscan_test_read_blk;
	test_io_cb_read_blk64 = iscan_test_read_blk64;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &test_fs);
	གལ་ཏེ (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		exit(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	གལ་ཏེ (retval) {
		com_err("setup", retval,
			"While allocating tables for test filesystem");
		exit(1);
	}
	retval = ext2fs_allocate_block_bitmap(test_fs, "bad block map",
					      &bad_block_map);
	གལ་ཏེ (retval) {
		com_err("setup", retval,
			"While allocating bad_block bitmap");
		exit(1);
	}
	retval = ext2fs_allocate_block_bitmap(test_fs, "touched map",
					      &touched_map);
	གལ་ཏེ (retval) {
		com_err("setup", retval,
			"While allocating touched block bitmap");
		exit(1);
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "bad inode map",
					      &bad_inode_map);
	གལ་ཏེ (retval) {
		com_err("setup", retval,
			"While allocating bad inode bitmap");
		exit(1);
	}

	retval = ext2fs_badblocks_list_create(&test_badblocks, 5);
	གལ་ཏེ (retval) {
		com_err("setup", retval, "while creating badblocks list");
		exit(1);
	}
	ཆེད་དུ (i=0; test_vec[i]; i++) {
		retval = ext2fs_badblocks_list_add(test_badblocks, test_vec[i]);
		གལ་ཏེ (retval) {
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
static void iterate(void)
{
	sa_nracanaa ext2_inode inode;
	ext2_inode_scan	scan;
	errcode_t	retval;
	ext2_ino_t	ino;

	retval = ext2fs_open_inode_scan(test_fs, 8, &scan);
	གལ་ཏེ (retval) {
		com_err("iterate", retval, "While opening inode scan");
		exit(1);
	}
	printf("Reading blocks: ");
	retval = ext2fs_get_next_inode(scan, &ino, &inode);
	གལ་ཏེ (retval) {
		com_err("iterate", retval, "while reading first inode");
		exit(1);
	}
	སྐབས (ino) {
		retval = ext2fs_get_next_inode(scan, &ino, &inode);
		གལ་ཏེ (retval == EXT2_ET_BAD_BLOCK_IN_INODE_TABLE) {
			ext2fs_mark_inode_bitmap2(bad_inode_map, ino);
			continue;
		}
		གལ་ཏེ (retval) {
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
static void check_map(void)
{
	ཧྲིལ	i, j, first=1;
	blk64_t	blk;

	ཆེད་དུ (i=0; test_vec[i]; i++) {
		གལ་ཏེ (ext2fs_test_block_bitmap2(touched_map, test_vec[i])) {
			printf("Bad block was touched --- %llu\n",
			       (unsigned long long) test_vec[i]);
			failed++;
			first_no_comma = 1;
		}
		ext2fs_mark_block_bitmap2(touched_map, test_vec[i]);
	}
	ཆེད་དུ (i = 0; i < test_fs->group_desc_count; i++) {
		ཆེད་དུ (j=0, blk = ext2fs_inode_table_loc(test_fs, i);
		     j < test_fs->inode_blocks_per_group;
		     j++, blk++) {
			གལ་ཏེ (!ext2fs_test_block_bitmap2(touched_map, blk) &&
			    !ext2fs_test_block_bitmap2(bad_block_map, blk)) {
				printf("Missing block --- %llu\n",
				       (unsigned long long) blk);
				failed++;
			}
		}
	}
	printf("Bad inodes: ");
	ཆེད་དུ (i=1; i <= test_fs->super->s_inodes_count; i++) {
		གལ་ཏེ (ext2fs_test_inode_bitmap2(bad_inode_map, i)) {
			གལ་ཏེ (first)
				first = 0;
			དེ་མིན
				printf(", ");
			printf("%u", i);
		}
	}
	printf("\n");
}


ཧྲིལ གཙོ(ཧྲིལ argc, var_na **argv)
{
	setup();
	iterate();
	check_map();
	གལ་ཏེ (!failed)
		printf("Inode scan tested OK!\n");
	ཕྱིར failed;
}

