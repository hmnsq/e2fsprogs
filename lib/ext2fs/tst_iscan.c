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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଯଦି HAVE_ERRNO_H
#ଅନ୍ତର୍ଭାଵ <errno.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

blk64_t test_vec[] = { 8, 12, 24, 34, 43, 44, 100, 0 };

ext2_filsys	test_fs;
ext2fs_block_bitmap bad_block_map, touched_map;
ext2fs_inode_bitmap bad_inode_map;
badblocks_list	test_badblocks;

ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_no_comma = 1;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା failed = 0;

ସ୍ଥିର ଶୂନ୍ୟ iscan_test_read_blk64(unsigned long long block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, errcode_t err)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	i;

	ଯଦି (first_no_comma)
		first_no_comma = 0;
	ଅନ୍ୟଥା
		printf(", ");

	ଯଦି (ଗଣନା_2 > 1)
		printf("%llu-%llu", block, block+ଗଣନା_2-1);
	ଅନ୍ୟଥା
		printf("%llu", block);

	ପାଇଁ (i=0; i < ଗଣନା_2; i++, block++) {
		ଯଦି (ext2fs_test_block_bitmap2(touched_map, block)) {
			printf("\nDuplicate block?!? --- %llu\n", block);
			failed++;
			first_no_comma = 1;
		}
		ext2fs_mark_block_bitmap2(touched_map, block);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ iscan_test_read_blk(unsigned long block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, errcode_t err)
{
	iscan_test_read_blk64(block, ଗଣନା_2, err);
}

/*
 * Setup the variables for doing the inode scan test.
 */
ସ୍ଥିର ଶୂନ୍ୟ setup(ଶୂନ୍ୟ)
{
	errcode_t	retval;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		i;
	ସଂରଚନା ext2_super_block param;

	initialize_ext2_error_table();

	memset(&param, 0, ଆକାର(param));
	ext2fs_blocks_ଗଣନା_set(&param, 12000);


	test_io_cb_read_blk = iscan_test_read_blk;
	test_io_cb_read_blk64 = iscan_test_read_blk64;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &test_fs);
	ଯଦି (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		exit(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	ଯଦି (retval) {
		com_err("setup", retval,
			"While allocating tables for test filesystem");
		exit(1);
	}
	retval = ext2fs_allocate_block_bitmap(test_fs, "bad block map",
					      &bad_block_map);
	ଯଦି (retval) {
		com_err("setup", retval,
			"While allocating bad_block bitmap");
		exit(1);
	}
	retval = ext2fs_allocate_block_bitmap(test_fs, "touched map",
					      &touched_map);
	ଯଦି (retval) {
		com_err("setup", retval,
			"While allocating touched block bitmap");
		exit(1);
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "bad inode map",
					      &bad_inode_map);
	ଯଦି (retval) {
		com_err("setup", retval,
			"While allocating bad inode bitmap");
		exit(1);
	}

	retval = ext2fs_badblocks_list_create(&test_badblocks, 5);
	ଯଦି (retval) {
		com_err("setup", retval, "while creating badblocks list");
		exit(1);
	}
	ପାଇଁ (i=0; test_vec[i]; i++) {
		retval = ext2fs_badblocks_list_add(test_badblocks, test_vec[i]);
		ଯଦି (retval) {
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
ସ୍ଥିର ଶୂନ୍ୟ iterate(ଶୂନ୍ୟ)
{
	ସଂରଚନା ext2_inode inode;
	ext2_inode_scan	scan;
	errcode_t	retval;
	ext2_ino_t	ino;

	retval = ext2fs_open_inode_scan(test_fs, 8, &scan);
	ଯଦି (retval) {
		com_err("iterate", retval, "While opening inode scan");
		exit(1);
	}
	printf("Reading blocks: ");
	retval = ext2fs_get_next_inode(scan, &ino, &inode);
	ଯଦି (retval) {
		com_err("iterate", retval, "while reading first inode");
		exit(1);
	}
	ଯେତେବେଳେ (ino) {
		retval = ext2fs_get_next_inode(scan, &ino, &inode);
		ଯଦି (retval == EXT2_ET_BAD_BLOCK_IN_INODE_TABLE) {
			ext2fs_mark_inode_bitmap2(bad_inode_map, ino);
			ଜାରି;
		}
		ଯଦି (retval) {
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
ସ୍ଥିର ଶୂନ୍ୟ check_map(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	i, j, first=1;
	blk64_t	blk;

	ପାଇଁ (i=0; test_vec[i]; i++) {
		ଯଦି (ext2fs_test_block_bitmap2(touched_map, test_vec[i])) {
			printf("Bad block was touched --- %llu\n",
			       (unsigned long long) test_vec[i]);
			failed++;
			first_no_comma = 1;
		}
		ext2fs_mark_block_bitmap2(touched_map, test_vec[i]);
	}
	ପାଇଁ (i = 0; i < test_fs->group_desc_ଗଣନା; i++) {
		ପାଇଁ (j=0, blk = ext2fs_inode_table_loc(test_fs, i);
		     j < test_fs->inode_blocks_per_group;
		     j++, blk++) {
			ଯଦି (!ext2fs_test_block_bitmap2(touched_map, blk) &&
			    !ext2fs_test_block_bitmap2(bad_block_map, blk)) {
				printf("Missing block --- %llu\n",
				       (unsigned long long) blk);
				failed++;
			}
		}
	}
	printf("Bad inodes: ");
	ପାଇଁ (i=1; i <= test_fs->super->s_inodes_ଗଣନା; i++) {
		ଯଦି (ext2fs_test_inode_bitmap2(bad_inode_map, i)) {
			ଯଦି (first)
				first = 0;
			ଅନ୍ୟଥା
				printf(", ");
			printf("%u", i);
		}
	}
	printf("\n");
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୁଖ୍ୟ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
	setup();
	iterate();
	check_map();
	ଯଦି (!failed)
		printf("Inode scan tested OK!\n");
	ଫେରନ୍ତୁ failed;
}

