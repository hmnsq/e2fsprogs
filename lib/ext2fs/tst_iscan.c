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
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

blk64_t test_vec[] = { 8, 12, 24, 34, 43, 44, 100, 0 };

ext2_filsys	test_fs;
ext2fs_block_bitmap bad_block_map, touched_map;
ext2fs_inode_bitmap bad_inode_map;
badblocks_list	test_badblocks;

صحيح first_no_comma = 1;
صحيح failed = 0;

ساكن فراغ iscan_test_read_blk64(unsigned long long block, صحيح تعداد_2, errcode_t err)
{
	صحيح	i;

	إذا (first_no_comma)
		first_no_comma = 0;
	وإلا
		printf(", ");

	إذا (تعداد_2 > 1)
		printf("%llu-%llu", block, block+تعداد_2-1);
	وإلا
		printf("%llu", block);

	لكل (i=0; i < تعداد_2; i++, block++) {
		إذا (ext2fs_test_block_bitmap2(touched_map, block)) {
			printf("\nDuplicate block?!? --- %llu\n", block);
			failed++;
			first_no_comma = 1;
		}
		ext2fs_mark_block_bitmap2(touched_map, block);
	}
}

ساكن فراغ iscan_test_read_blk(unsigned long block, صحيح تعداد_2, errcode_t err)
{
	iscan_test_read_blk64(block, تعداد_2, err);
}

/*
 * Setup the variables for doing the inode scan test.
 */
ساكن فراغ setup(فراغ)
{
	errcode_t	retval;
	صحيح		i;
	بنية ext2_super_block param;

	initialize_ext2_error_table();

	memset(&param, 0, حجم(param));
	ext2fs_blocks_تعداد_set(&param, 12000);


	test_io_cb_read_blk = iscan_test_read_blk;
	test_io_cb_read_blk64 = iscan_test_read_blk64;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &test_fs);
	إذا (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		exit(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	إذا (retval) {
		com_err("setup", retval,
			"While allocating tables for test filesystem");
		exit(1);
	}
	retval = ext2fs_allocate_block_bitmap(test_fs, "bad block map",
					      &bad_block_map);
	إذا (retval) {
		com_err("setup", retval,
			"While allocating bad_block bitmap");
		exit(1);
	}
	retval = ext2fs_allocate_block_bitmap(test_fs, "touched map",
					      &touched_map);
	إذا (retval) {
		com_err("setup", retval,
			"While allocating touched block bitmap");
		exit(1);
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "bad inode map",
					      &bad_inode_map);
	إذا (retval) {
		com_err("setup", retval,
			"While allocating bad inode bitmap");
		exit(1);
	}

	retval = ext2fs_badblocks_list_create(&test_badblocks, 5);
	إذا (retval) {
		com_err("setup", retval, "while creating badblocks list");
		exit(1);
	}
	لكل (i=0; test_vec[i]; i++) {
		retval = ext2fs_badblocks_list_add(test_badblocks, test_vec[i]);
		إذا (retval) {
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
ساكن فراغ iterate(فراغ)
{
	بنية ext2_inode inode;
	ext2_inode_scan	scan;
	errcode_t	retval;
	ext2_ino_t	ino;

	retval = ext2fs_open_inode_scan(test_fs, 8, &scan);
	إذا (retval) {
		com_err("iterate", retval, "While opening inode scan");
		exit(1);
	}
	printf("Reading blocks: ");
	retval = ext2fs_get_next_inode(scan, &ino, &inode);
	إذا (retval) {
		com_err("iterate", retval, "while reading first inode");
		exit(1);
	}
	بينما (ino) {
		retval = ext2fs_get_next_inode(scan, &ino, &inode);
		إذا (retval == EXT2_ET_BAD_BLOCK_IN_INODE_TABLE) {
			ext2fs_mark_inode_bitmap2(bad_inode_map, ino);
			استمر;
		}
		إذا (retval) {
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
ساكن فراغ check_map(فراغ)
{
	صحيح	i, j, first=1;
	blk64_t	blk;

	لكل (i=0; test_vec[i]; i++) {
		إذا (ext2fs_test_block_bitmap2(touched_map, test_vec[i])) {
			printf("Bad block was touched --- %llu\n",
			       (unsigned long long) test_vec[i]);
			failed++;
			first_no_comma = 1;
		}
		ext2fs_mark_block_bitmap2(touched_map, test_vec[i]);
	}
	لكل (i = 0; i < test_fs->group_desc_تعداد; i++) {
		لكل (j=0, blk = ext2fs_inode_table_loc(test_fs, i);
		     j < test_fs->inode_blocks_per_group;
		     j++, blk++) {
			إذا (!ext2fs_test_block_bitmap2(touched_map, blk) &&
			    !ext2fs_test_block_bitmap2(bad_block_map, blk)) {
				printf("Missing block --- %llu\n",
				       (unsigned long long) blk);
				failed++;
			}
		}
	}
	printf("Bad inodes: ");
	لكل (i=1; i <= test_fs->super->s_inodes_تعداد; i++) {
		إذا (ext2fs_test_inode_bitmap2(bad_inode_map, i)) {
			إذا (first)
				first = 0;
			وإلا
				printf(", ");
			printf("%u", i);
		}
	}
	printf("\n");
}


صحيح الرئيسية(صحيح argc, حرف **argv)
{
	setup();
	iterate();
	check_map();
	إذا (!failed)
		printf("Inode scan tested OK!\n");
	إرجاع failed;
}

