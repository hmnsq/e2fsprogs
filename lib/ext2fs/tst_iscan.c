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
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/types.h>
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

blk64_t test_vec[] = { 8, 12, 24, 34, 43, 44, 100, 0 };

ext2_filsys	test_fs;
ext2fs_खण्डम्_bitmap bad_खण्डम्_map, touched_map;
ext2fs_inode_bitmap bad_inode_map;
badblocks_list	test_badblocks;

पूर्णाङ्क first_no_comma = 1;
पूर्णाङ्क failed = 0;

स्थिर शून्य iscan_test_read_blk64(अचिह्नित दीर्घ दीर्घ खण्डम्, पूर्णाङ्क गणनम्, errcode_t err)
{
	पूर्णाङ्क	i;

	यदि (first_no_comma)
		first_no_comma = 0;
	अन्यथा
		प्रारूप_लिख(", ");

	यदि (गणनम् > 1)
		प्रारूप_लिख("%llu-%llu", खण्डम्, खण्डम्+गणनम्-1);
	अन्यथा
		प्रारूप_लिख("%llu", खण्डम्);

	प्रति (i=0; i < गणनम्; i++, खण्डम्++) {
		यदि (ext2fs_test_खण्डम्_bitmap2(touched_map, खण्डम्)) {
			प्रारूप_लिख("\nDuplicate block?!? --- %llu\n", खण्डम्);
			failed++;
			first_no_comma = 1;
		}
		ext2fs_mark_खण्डम्_bitmap2(touched_map, खण्डम्);
	}
}

स्थिर शून्य iscan_test_read_blk(अचिह्नित दीर्घ खण्डम्, पूर्णाङ्क गणनम्, errcode_t err)
{
	iscan_test_read_blk64(खण्डम्, गणनम्, err);
}

/*
 * Setup the variables for doing the inode scan test.
 */
स्थिर शून्य सज्जता(शून्य)
{
	errcode_t	retval;
	पूर्णाङ्क		i;
	संरचना ext2_super_खण्डम् param;

	initialize_ext2_error_सारणी();

	स्मृतिं_पूरय(&param, 0, परिमाण(param));
	ext2fs_खण्डानि_गणनम्_समुच्चयः(&param, 12000);


	test_io_cb_read_blk = iscan_test_read_blk;
	test_io_cb_read_blk64 = iscan_test_read_blk64;

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64_द्वयङ्काः, &param,
				   test_io_manager, &test_fs);
	यदि (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		निर्गच्छ(1);
	}
	retval = ext2fs_allocate_tables(test_fs);
	यदि (retval) {
		com_err("setup", retval,
			"While allocating tables for test filesystem");
		निर्गच्छ(1);
	}
	retval = ext2fs_allocate_खण्डम्_bitmap(test_fs, "bad block map",
					      &bad_खण्डम्_map);
	यदि (retval) {
		com_err("setup", retval,
			"While allocating bad_block bitmap");
		निर्गच्छ(1);
	}
	retval = ext2fs_allocate_खण्डम्_bitmap(test_fs, "touched map",
					      &touched_map);
	यदि (retval) {
		com_err("setup", retval,
			"While allocating touched block bitmap");
		निर्गच्छ(1);
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "bad inode map",
					      &bad_inode_map);
	यदि (retval) {
		com_err("setup", retval,
			"While allocating bad inode bitmap");
		निर्गच्छ(1);
	}

	retval = ext2fs_badblocks_list_create(&test_badblocks, 5);
	यदि (retval) {
		com_err("setup", retval, "while creating badblocks list");
		निर्गच्छ(1);
	}
	प्रति (i=0; test_vec[i]; i++) {
		retval = ext2fs_badblocks_list_सङ्कलय(test_badblocks, test_vec[i]);
		यदि (retval) {
			com_err("setup", retval,
				"while adding test vector %d", i);
			निर्गच्छ(1);
		}
		ext2fs_mark_खण्डम्_bitmap2(bad_खण्डम्_map, test_vec[i]);
	}
	test_fs->badblocks = test_badblocks;
}

/*
 * Iterate using inode_scan
 */
स्थिर शून्य iterate(शून्य)
{
	संरचना ext2_inode inode;
	ext2_inode_अवलोकनम्	अवलोकनम्;
	errcode_t	retval;
	ext2_ino_t	ino;

	retval = ext2fs_open_inode_अवलोकनम्(test_fs, 8, &अवलोकनम्);
	यदि (retval) {
		com_err("iterate", retval, "While opening inode scan");
		निर्गच्छ(1);
	}
	प्रारूप_लिख("Reading blocks: ");
	retval = ext2fs_get_अग्र_inode(अवलोकनम्, &ino, &inode);
	यदि (retval) {
		com_err("iterate", retval, "while reading first inode");
		निर्गच्छ(1);
	}
	यावत् (ino) {
		retval = ext2fs_get_अग्र_inode(अवलोकनम्, &ino, &inode);
		यदि (retval == EXT2_ET_BAD_खण्डम्_अन्तर्_INODE_सारणी) {
			ext2fs_mark_inode_bitmap2(bad_inode_map, ino);
			अनुवर्त;
		}
		यदि (retval) {
			com_err("iterate", retval,
				"while getting next inode");
			निर्गच्छ(1);
		}
	}
	प्रारूप_लिख("\n");
	ext2fs_close_inode_अवलोकनम्(अवलोकनम्);
}

/*
 * Verify the touched map
 */
स्थिर शून्य check_map(शून्य)
{
	पूर्णाङ्क	i, j, first=1;
	blk64_t	blk;

	प्रति (i=0; test_vec[i]; i++) {
		यदि (ext2fs_test_खण्डम्_bitmap2(touched_map, test_vec[i])) {
			प्रारूप_लिख("Bad block was touched --- %llu\n",
			       (अचिह्नित दीर्घ दीर्घ) test_vec[i]);
			failed++;
			first_no_comma = 1;
		}
		ext2fs_mark_खण्डम्_bitmap2(touched_map, test_vec[i]);
	}
	प्रति (i = 0; i < test_fs->गणः_desc_गणनम्; i++) {
		प्रति (j=0, blk = ext2fs_inode_सारणी_loc(test_fs, i);
		     j < test_fs->inode_खण्डानि_प्रत्येकम्_गणः;
		     j++, blk++) {
			यदि (!ext2fs_test_खण्डम्_bitmap2(touched_map, blk) &&
			    !ext2fs_test_खण्डम्_bitmap2(bad_खण्डम्_map, blk)) {
				प्रारूप_लिख("Missing block --- %llu\n",
				       (अचिह्नित दीर्घ दीर्घ) blk);
				failed++;
			}
		}
	}
	प्रारूप_लिख("Bad inodes: ");
	प्रति (i=1; i <= test_fs->super->s_inodes_गणनम्; i++) {
		यदि (ext2fs_test_inode_bitmap2(bad_inode_map, i)) {
			यदि (first)
				first = 0;
			अन्यथा
				प्रारूप_लिख(", ");
			प्रारूप_लिख("%u", i);
		}
	}
	प्रारूप_लिख("\n");
}


पूर्णाङ्क मुख्यम्(पूर्णाङ्क argc, वर्ण **argv)
{
	सज्जता();
	iterate();
	check_map();
	यदि (!failed)
		प्रारूप_लिख("Inode scan tested OK!\n");
	प्रत्यागम failed;
}

