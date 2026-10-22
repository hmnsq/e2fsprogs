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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <time.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <sys/types.h>
#എങ്കിൽ HAVE_ERRNO_H
#അന്തര്ഭാവ <errno.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

blk64_t test_vec[] = { 8, 12, 24, 34, 43, 44, 100, 0 };

ext2_filsys	test_fs;
ext2fs_block_bitmap bad_block_map, touched_map;
ext2fs_inode_bitmap bad_inode_map;
badblocks_list	test_badblocks;

പൂർണ്ണസംഖ്യ first_no_comma = 1;
പൂർണ്ണസംഖ്യ failed = 0;

സ്ഥിതം ശൂന്യം iscan_test_read_blk64(unsigned long long block, പൂർണ്ണസംഖ്യ എണ്ണം, errcode_t err)
{
	പൂർണ്ണസംഖ്യ	i;

	എങ്കിൽ (first_no_comma)
		first_no_comma = 0;
	അല്ലെങ്കിൽ
		printf(", ");

	എങ്കിൽ (എണ്ണം > 1)
		printf("%llu-%llu", block, block+എണ്ണം-1);
	അല്ലെങ്കിൽ
		printf("%llu", block);

	വേണ്ടി (i=0; i < എണ്ണം; i++, block++) {
		എങ്കിൽ (ext2fs_test_block_bitmap2(touched_map, block)) {
			printf("\nDuplicate block?!? --- %llu\n", block);
			failed++;
			first_no_comma = 1;
		}
		ext2fs_mark_block_bitmap2(touched_map, block);
	}
}

സ്ഥിതം ശൂന്യം iscan_test_read_blk(unsigned long block, പൂർണ്ണസംഖ്യ എണ്ണം, errcode_t err)
{
	iscan_test_read_blk64(block, എണ്ണം, err);
}

/*
 * Setup the variables for doing the inode scan test.
 */
സ്ഥിതം ശൂന്യം setup(ശൂന്യം)
{
	errcode_t	retval;
	പൂർണ്ണസംഖ്യ		i;
	ഘടന ext2_super_block param;

	initialize_ext2_error_table();

	memset(&param, 0, വലുപ്പം(param));
	ext2fs_blocks_എണ്ണം_set(&param, 12000);


	test_io_cb_read_blk = iscan_test_read_blk;
	test_io_cb_read_blk64 = iscan_test_read_blk64;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &test_fs);
	എങ്കിൽ (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		exit(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	എങ്കിൽ (retval) {
		com_err("setup", retval,
			"While allocating tables for test filesystem");
		exit(1);
	}
	retval = ext2fs_allocate_block_bitmap(test_fs, "bad block map",
					      &bad_block_map);
	എങ്കിൽ (retval) {
		com_err("setup", retval,
			"While allocating bad_block bitmap");
		exit(1);
	}
	retval = ext2fs_allocate_block_bitmap(test_fs, "touched map",
					      &touched_map);
	എങ്കിൽ (retval) {
		com_err("setup", retval,
			"While allocating touched block bitmap");
		exit(1);
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "bad inode map",
					      &bad_inode_map);
	എങ്കിൽ (retval) {
		com_err("setup", retval,
			"While allocating bad inode bitmap");
		exit(1);
	}

	retval = ext2fs_badblocks_list_create(&test_badblocks, 5);
	എങ്കിൽ (retval) {
		com_err("setup", retval, "while creating badblocks list");
		exit(1);
	}
	വേണ്ടി (i=0; test_vec[i]; i++) {
		retval = ext2fs_badblocks_list_add(test_badblocks, test_vec[i]);
		എങ്കിൽ (retval) {
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
സ്ഥിതം ശൂന്യം iterate(ശൂന്യം)
{
	ഘടന ext2_inode inode;
	ext2_inode_scan	scan;
	errcode_t	retval;
	ext2_ino_t	ino;

	retval = ext2fs_open_inode_scan(test_fs, 8, &scan);
	എങ്കിൽ (retval) {
		com_err("iterate", retval, "While opening inode scan");
		exit(1);
	}
	printf("Reading blocks: ");
	retval = ext2fs_get_next_inode(scan, &ino, &inode);
	എങ്കിൽ (retval) {
		com_err("iterate", retval, "while reading first inode");
		exit(1);
	}
	ആകുമ്പോൾ (ino) {
		retval = ext2fs_get_next_inode(scan, &ino, &inode);
		എങ്കിൽ (retval == EXT2_ET_BAD_BLOCK_IN_INODE_TABLE) {
			ext2fs_mark_inode_bitmap2(bad_inode_map, ino);
			തുടരുക;
		}
		എങ്കിൽ (retval) {
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
സ്ഥിതം ശൂന്യം check_map(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ	i, j, first=1;
	blk64_t	blk;

	വേണ്ടി (i=0; test_vec[i]; i++) {
		എങ്കിൽ (ext2fs_test_block_bitmap2(touched_map, test_vec[i])) {
			printf("Bad block was touched --- %llu\n",
			       (unsigned long long) test_vec[i]);
			failed++;
			first_no_comma = 1;
		}
		ext2fs_mark_block_bitmap2(touched_map, test_vec[i]);
	}
	വേണ്ടി (i = 0; i < test_fs->group_desc_എണ്ണം; i++) {
		വേണ്ടി (j=0, blk = ext2fs_inode_table_loc(test_fs, i);
		     j < test_fs->inode_blocks_per_group;
		     j++, blk++) {
			എങ്കിൽ (!ext2fs_test_block_bitmap2(touched_map, blk) &&
			    !ext2fs_test_block_bitmap2(bad_block_map, blk)) {
				printf("Missing block --- %llu\n",
				       (unsigned long long) blk);
				failed++;
			}
		}
	}
	printf("Bad inodes: ");
	വേണ്ടി (i=1; i <= test_fs->super->s_inodes_എണ്ണം; i++) {
		എങ്കിൽ (ext2fs_test_inode_bitmap2(bad_inode_map, i)) {
			എങ്കിൽ (first)
				first = 0;
			അല്ലെങ്കിൽ
				printf(", ");
			printf("%u", i);
		}
	}
	printf("\n");
}


പൂർണ്ണസംഖ്യ പ്രധാന(പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
	setup();
	iterate();
	check_map();
	എങ്കിൽ (!failed)
		printf("Inode scan tested OK!\n");
	തിരികെ failed;
}

