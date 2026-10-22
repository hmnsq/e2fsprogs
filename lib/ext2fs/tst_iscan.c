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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/types.h>
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

blk64_t test_vec[] = { 8, 12, 24, 34, 43, 44, 100, 0 };

ext2_filsys	test_fs;
ext2fs_block_bitmap bad_block_map, touched_map;
ext2fs_inode_bitmap bad_inode_map;
badblocks_list	test_badblocks;

पूर्णाङ्क first_no_comma = 1;
पूर्णाङ्क failed = 0;

स्थिर शून्य iscan_test_read_blk64(अचिन्हित दीर्घ दीर्घ block, पूर्णाङ्क गिनती, errcode_t err)
{
	पूर्णाङ्क	i;

	यदि (first_no_comma)
		first_no_comma = 0;
	अन्यथा
		म_लिखो(", ");

	यदि (गिनती > 1)
		म_लिखो("%llu-%llu", block, block+गिनती-1);
	अन्यथा
		म_लिखो("%llu", block);

	प्रति (i=0; i < गिनती; i++, block++) {
		यदि (ext2fs_test_block_bitmap2(touched_map, block)) {
			म_लिखो("\nDuplicate block?!? --- %llu\n", block);
			failed++;
			first_no_comma = 1;
		}
		ext2fs_mark_block_bitmap2(touched_map, block);
	}
}

स्थिर शून्य iscan_test_read_blk(अचिन्हित दीर्घ block, पूर्णाङ्क गिनती, errcode_t err)
{
	iscan_test_read_blk64(block, गिनती, err);
}

/*
 * Setup the variables for doing the inode scan test.
 */
स्थिर शून्य setup(शून्य)
{
	errcode_t	retval;
	पूर्णाङ्क		i;
	संरचना ext2_super_block param;

	initialize_ext2_error_table();

	स_रखो(&param, 0, परिमाण(param));
	ext2fs_blocks_गिनती_set(&param, 12000);


	test_io_cb_read_blk = iscan_test_read_blk;
	test_io_cb_read_blk64 = iscan_test_read_blk64;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &test_fs);
	यदि (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		निकास(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	यदि (retval) {
		com_err("setup", retval,
			"While allocating tables for test filesystem");
		निकास(1);
	}
	retval = ext2fs_allocate_block_bitmap(test_fs, "bad block map",
					      &bad_block_map);
	यदि (retval) {
		com_err("setup", retval,
			"While allocating bad_block bitmap");
		निकास(1);
	}
	retval = ext2fs_allocate_block_bitmap(test_fs, "touched map",
					      &touched_map);
	यदि (retval) {
		com_err("setup", retval,
			"While allocating touched block bitmap");
		निकास(1);
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "bad inode map",
					      &bad_inode_map);
	यदि (retval) {
		com_err("setup", retval,
			"While allocating bad inode bitmap");
		निकास(1);
	}

	retval = ext2fs_badblocks_list_create(&test_badblocks, 5);
	यदि (retval) {
		com_err("setup", retval, "while creating badblocks list");
		निकास(1);
	}
	प्रति (i=0; test_vec[i]; i++) {
		retval = ext2fs_badblocks_list_add(test_badblocks, test_vec[i]);
		यदि (retval) {
			com_err("setup", retval,
				"while adding test vector %d", i);
			निकास(1);
		}
		ext2fs_mark_block_bitmap2(bad_block_map, test_vec[i]);
	}
	test_fs->badblocks = test_badblocks;
}

/*
 * Iterate using inode_scan
 */
स्थिर शून्य iterate(शून्य)
{
	संरचना ext2_inode inode;
	ext2_inode_scan	scan;
	errcode_t	retval;
	ext2_ino_t	ino;

	retval = ext2fs_open_inode_scan(test_fs, 8, &scan);
	यदि (retval) {
		com_err("iterate", retval, "While opening inode scan");
		निकास(1);
	}
	म_लिखो("Reading blocks: ");
	retval = ext2fs_get_next_inode(scan, &ino, &inode);
	यदि (retval) {
		com_err("iterate", retval, "while reading first inode");
		निकास(1);
	}
	यावत् (ino) {
		retval = ext2fs_get_next_inode(scan, &ino, &inode);
		यदि (retval == EXT2_ET_BAD_BLOCK_IN_INODE_TABLE) {
			ext2fs_mark_inode_bitmap2(bad_inode_map, ino);
			अनुवर्त;
		}
		यदि (retval) {
			com_err("iterate", retval,
				"while getting next inode");
			निकास(1);
		}
	}
	म_लिखो("\n");
	ext2fs_close_inode_scan(scan);
}

/*
 * Verify the touched map
 */
स्थिर शून्य check_map(शून्य)
{
	पूर्णाङ्क	i, j, first=1;
	blk64_t	blk;

	प्रति (i=0; test_vec[i]; i++) {
		यदि (ext2fs_test_block_bitmap2(touched_map, test_vec[i])) {
			म_लिखो("Bad block was touched --- %llu\n",
			       (अचिन्हित दीर्घ दीर्घ) test_vec[i]);
			failed++;
			first_no_comma = 1;
		}
		ext2fs_mark_block_bitmap2(touched_map, test_vec[i]);
	}
	प्रति (i = 0; i < test_fs->group_desc_गिनती; i++) {
		प्रति (j=0, blk = ext2fs_inode_table_loc(test_fs, i);
		     j < test_fs->inode_blocks_per_group;
		     j++, blk++) {
			यदि (!ext2fs_test_block_bitmap2(touched_map, blk) &&
			    !ext2fs_test_block_bitmap2(bad_block_map, blk)) {
				म_लिखो("Missing block --- %llu\n",
				       (अचिन्हित दीर्घ दीर्घ) blk);
				failed++;
			}
		}
	}
	म_लिखो("Bad inodes: ");
	प्रति (i=1; i <= test_fs->super->s_inodes_गिनती; i++) {
		यदि (ext2fs_test_inode_bitmap2(bad_inode_map, i)) {
			यदि (first)
				first = 0;
			अन्यथा
				म_लिखो(", ");
			म_लिखो("%u", i);
		}
	}
	म_लिखो("\n");
}


पूर्णाङ्क main(पूर्णाङ्क argc, वर्ण **argv)
{
	setup();
	iterate();
	check_map();
	यदि (!failed)
		म_लिखो("Inode scan tested OK!\n");
	प्रत्यागम failed;
}

