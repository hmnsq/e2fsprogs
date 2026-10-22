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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <fcntl.h>
#समावेश <time.h>
#समावेश <sys/stat.h>
#समावेश <sys/types.h>
#अगर HAVE_ERRNO_H
#समावेश <errno.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

blk64_t test_vec[] = { 8, 12, 24, 34, 43, 44, 100, 0 };

ext2_filsys	test_fs;
ext2fs_block_bitmap bad_block_map, touched_map;
ext2fs_inode_bitmap bad_inode_map;
badblocks_list	test_badblocks;

पूर्णांक first_no_comma = 1;
पूर्णांक failed = 0;

अटल व्योम iscan_test_read_blk64(अचिन्हित दीर्घ दीर्घ block, पूर्णांक गिनती, errcode_t err)
{
	पूर्णांक	i;

	अगर (first_no_comma)
		first_no_comma = 0;
	अन्यथा
		म_लिखो(", ");

	अगर (गिनती > 1)
		म_लिखो("%llu-%llu", block, block+गिनती-1);
	अन्यथा
		म_लिखो("%llu", block);

	क्रम (i=0; i < गिनती; i++, block++) {
		अगर (ext2fs_test_block_bitmap2(touched_map, block)) {
			म_लिखो("\nDuplicate block?!? --- %llu\n", block);
			failed++;
			first_no_comma = 1;
		}
		ext2fs_mark_block_bitmap2(touched_map, block);
	}
}

अटल व्योम iscan_test_read_blk(अचिन्हित दीर्घ block, पूर्णांक गिनती, errcode_t err)
{
	iscan_test_read_blk64(block, गिनती, err);
}

/*
 * Setup the variables for doing the inode scan test.
 */
अटल व्योम setup(व्योम)
{
	errcode_t	retval;
	पूर्णांक		i;
	काष्ठा ext2_super_block param;

	initialize_ext2_error_table();

	स_रखो(&param, 0, माप(param));
	ext2fs_blocks_गिनती_set(&param, 12000);


	test_io_cb_read_blk = iscan_test_read_blk;
	test_io_cb_read_blk64 = iscan_test_read_blk64;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &test_fs);
	अगर (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		निकास(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	अगर (retval) {
		com_err("setup", retval,
			"While allocating tables for test filesystem");
		निकास(1);
	}
	retval = ext2fs_allocate_block_bitmap(test_fs, "bad block map",
					      &bad_block_map);
	अगर (retval) {
		com_err("setup", retval,
			"While allocating bad_block bitmap");
		निकास(1);
	}
	retval = ext2fs_allocate_block_bitmap(test_fs, "touched map",
					      &touched_map);
	अगर (retval) {
		com_err("setup", retval,
			"While allocating touched block bitmap");
		निकास(1);
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "bad inode map",
					      &bad_inode_map);
	अगर (retval) {
		com_err("setup", retval,
			"While allocating bad inode bitmap");
		निकास(1);
	}

	retval = ext2fs_badblocks_list_create(&test_badblocks, 5);
	अगर (retval) {
		com_err("setup", retval, "while creating badblocks list");
		निकास(1);
	}
	क्रम (i=0; test_vec[i]; i++) {
		retval = ext2fs_badblocks_list_add(test_badblocks, test_vec[i]);
		अगर (retval) {
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
अटल व्योम iterate(व्योम)
{
	काष्ठा ext2_inode inode;
	ext2_inode_scan	scan;
	errcode_t	retval;
	ext2_ino_t	ino;

	retval = ext2fs_open_inode_scan(test_fs, 8, &scan);
	अगर (retval) {
		com_err("iterate", retval, "While opening inode scan");
		निकास(1);
	}
	म_लिखो("Reading blocks: ");
	retval = ext2fs_get_अग्र_inode(scan, &ino, &inode);
	अगर (retval) {
		com_err("iterate", retval, "while reading first inode");
		निकास(1);
	}
	जबतक (ino) {
		retval = ext2fs_get_अग्र_inode(scan, &ino, &inode);
		अगर (retval == EXT2_ET_BAD_BLOCK_IN_INODE_TABLE) {
			ext2fs_mark_inode_bitmap2(bad_inode_map, ino);
			जारी;
		}
		अगर (retval) {
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
अटल व्योम check_map(व्योम)
{
	पूर्णांक	i, j, first=1;
	blk64_t	blk;

	क्रम (i=0; test_vec[i]; i++) {
		अगर (ext2fs_test_block_bitmap2(touched_map, test_vec[i])) {
			म_लिखो("Bad block was touched --- %llu\n",
			       (अचिन्हित दीर्घ दीर्घ) test_vec[i]);
			failed++;
			first_no_comma = 1;
		}
		ext2fs_mark_block_bitmap2(touched_map, test_vec[i]);
	}
	क्रम (i = 0; i < test_fs->group_desc_गिनती; i++) {
		क्रम (j=0, blk = ext2fs_inode_table_loc(test_fs, i);
		     j < test_fs->inode_blocks_per_group;
		     j++, blk++) {
			अगर (!ext2fs_test_block_bitmap2(touched_map, blk) &&
			    !ext2fs_test_block_bitmap2(bad_block_map, blk)) {
				म_लिखो("Missing block --- %llu\n",
				       (अचिन्हित दीर्घ दीर्घ) blk);
				failed++;
			}
		}
	}
	म_लिखो("Bad inodes: ");
	क्रम (i=1; i <= test_fs->super->s_inodes_गिनती; i++) {
		अगर (ext2fs_test_inode_bitmap2(bad_inode_map, i)) {
			अगर (first)
				first = 0;
			अन्यथा
				म_लिखो(", ");
			म_लिखो("%u", i);
		}
	}
	म_लिखो("\n");
}


पूर्णांक मुख्य(पूर्णांक argc, अक्षर **argv)
{
	setup();
	iterate();
	check_map();
	अगर (!failed)
		म_लिखो("Inode scan tested OK!\n");
	वापस failed;
}

