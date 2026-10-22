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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <sys/types.h>
#எனில் HAVE_ERRNO_H
#அந்தர்பாவ <errno.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

blk64_t test_vec[] = { 8, 12, 24, 34, 43, 44, 100, 0 };

ext2_filsys	test_fs;
ext2fs_block_bitmap bad_block_map, touched_map;
ext2fs_inode_bitmap bad_inode_map;
badblocks_list	test_badblocks;

முழு first_no_comma = 1;
முழு failed = 0;

நிலையான வெறுமை iscan_test_read_blk64(unsigned long long block, முழு மொத்தம், errcode_t err)
{
	முழு	i;

	எனில் (first_no_comma)
		first_no_comma = 0;
	இல்லையெனில்
		printf(", ");

	எனில் (மொத்தம் > 1)
		printf("%llu-%llu", block, block+மொத்தம்-1);
	இல்லையெனில்
		printf("%llu", block);

	க்கு (i=0; i < மொத்தம்; i++, block++) {
		எனில் (ext2fs_test_block_bitmap2(touched_map, block)) {
			printf("\nDuplicate block?!? --- %llu\n", block);
			failed++;
			first_no_comma = 1;
		}
		ext2fs_mark_block_bitmap2(touched_map, block);
	}
}

நிலையான வெறுமை iscan_test_read_blk(unsigned long block, முழு மொத்தம், errcode_t err)
{
	iscan_test_read_blk64(block, மொத்தம், err);
}

/*
 * Setup the variables for doing the inode scan test.
 */
நிலையான வெறுமை setup(வெறுமை)
{
	errcode_t	retval;
	முழு		i;
	கட்டமைப்பு ext2_super_block param;

	initialize_ext2_error_table();

	memset(&param, 0, அளவு(param));
	ext2fs_blocks_மொத்தம்_set(&param, 12000);


	test_io_cb_read_blk = iscan_test_read_blk;
	test_io_cb_read_blk64 = iscan_test_read_blk64;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &test_fs);
	எனில் (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		exit(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	எனில் (retval) {
		com_err("setup", retval,
			"While allocating tables for test filesystem");
		exit(1);
	}
	retval = ext2fs_allocate_block_bitmap(test_fs, "bad block map",
					      &bad_block_map);
	எனில் (retval) {
		com_err("setup", retval,
			"While allocating bad_block bitmap");
		exit(1);
	}
	retval = ext2fs_allocate_block_bitmap(test_fs, "touched map",
					      &touched_map);
	எனில் (retval) {
		com_err("setup", retval,
			"While allocating touched block bitmap");
		exit(1);
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "bad inode map",
					      &bad_inode_map);
	எனில் (retval) {
		com_err("setup", retval,
			"While allocating bad inode bitmap");
		exit(1);
	}

	retval = ext2fs_badblocks_list_create(&test_badblocks, 5);
	எனில் (retval) {
		com_err("setup", retval, "while creating badblocks list");
		exit(1);
	}
	க்கு (i=0; test_vec[i]; i++) {
		retval = ext2fs_badblocks_list_add(test_badblocks, test_vec[i]);
		எனில் (retval) {
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
நிலையான வெறுமை iterate(வெறுமை)
{
	கட்டமைப்பு ext2_inode inode;
	ext2_inode_scan	scan;
	errcode_t	retval;
	ext2_ino_t	ino;

	retval = ext2fs_open_inode_scan(test_fs, 8, &scan);
	எனில் (retval) {
		com_err("iterate", retval, "While opening inode scan");
		exit(1);
	}
	printf("Reading blocks: ");
	retval = ext2fs_get_next_inode(scan, &ino, &inode);
	எனில் (retval) {
		com_err("iterate", retval, "while reading first inode");
		exit(1);
	}
	போது (ino) {
		retval = ext2fs_get_next_inode(scan, &ino, &inode);
		எனில் (retval == EXT2_ET_BAD_BLOCK_IN_INODE_TABLE) {
			ext2fs_mark_inode_bitmap2(bad_inode_map, ino);
			தொடர்;
		}
		எனில் (retval) {
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
நிலையான வெறுமை check_map(வெறுமை)
{
	முழு	i, j, first=1;
	blk64_t	blk;

	க்கு (i=0; test_vec[i]; i++) {
		எனில் (ext2fs_test_block_bitmap2(touched_map, test_vec[i])) {
			printf("Bad block was touched --- %llu\n",
			       (unsigned long long) test_vec[i]);
			failed++;
			first_no_comma = 1;
		}
		ext2fs_mark_block_bitmap2(touched_map, test_vec[i]);
	}
	க்கு (i = 0; i < test_fs->group_desc_மொத்தம்; i++) {
		க்கு (j=0, blk = ext2fs_inode_table_loc(test_fs, i);
		     j < test_fs->inode_blocks_per_group;
		     j++, blk++) {
			எனில் (!ext2fs_test_block_bitmap2(touched_map, blk) &&
			    !ext2fs_test_block_bitmap2(bad_block_map, blk)) {
				printf("Missing block --- %llu\n",
				       (unsigned long long) blk);
				failed++;
			}
		}
	}
	printf("Bad inodes: ");
	க்கு (i=1; i <= test_fs->super->s_inodes_மொத்தம்; i++) {
		எனில் (ext2fs_test_inode_bitmap2(bad_inode_map, i)) {
			எனில் (first)
				first = 0;
			இல்லையெனில்
				printf(", ");
			printf("%u", i);
		}
	}
	printf("\n");
}


முழு முதன்மை(முழு argc, எழுத்து **argv)
{
	setup();
	iterate();
	check_map();
	எனில் (!failed)
		printf("Inode scan tested OK!\n");
	திரும்பு failed;
}

