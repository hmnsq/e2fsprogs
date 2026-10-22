/*
 * This testing program makes sure the badblocks implementation works.
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

#define ADD_BLK	0x0001
#define DEL_BLK	0x0002

blk_t test1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0 };
blk_t test2[] = { 11, 10, 9, 8, 7, 6, 5, 4, 3, 3, 2, 1, 0 };
blk_t test3[] = { 3, 1, 4, 5, 9, 2, 7, 10, 5, 6, 10, 8, 0 };
blk_t test4[] = { 20, 50, 12, 17, 13, 2, 66, 23, 56, 0 };
blk_t test4a[] = {
	20, 1,
	50, 1,
	3, 0,
	17, 1,
	18, 0,
	16, 0,
	11, 0,
	12, 1,
	13, 1,
	14, 0,
	80, 0,
	45, 0,
	66, 1,
	0 };
blk_t test5[] = { 31, 20, 17, 51, 23, 1, 56, 57, 0 };
blk_t test5a[] = {
	50, ADD_BLK,
	51, DEL_BLK,
	57, DEL_BLK,
	66, ADD_BLK,
	31, DEL_BLK,
	12, ADD_BLK,
	2, ADD_BLK,
	13, ADD_BLK,
	1, DEL_BLK,
	0
	};


स्थिर पूर्णाङ्क test_fail = 0;
स्थिर पूर्णाङ्क test_expected_fail = 0;

स्थिर errcode_t create_test_list(blk_t *vec, badblocks_list *ret)
{
	errcode_t	retval;
	badblocks_list	bb;
	पूर्णाङ्क		i;

	retval = ext2fs_badblocks_list_create(&bb, 5);
	यदि (retval) {
		com_err("create_test_list", retval, "while creating list");
		फर्काऊ retval;
	}
	लागि (i=0; vec[i]; i++) {
		retval = ext2fs_badblocks_list_add(bb, vec[i]);
		यदि (retval) {
			com_err("create_test_list", retval,
				"while adding test vector %d", i);
			ext2fs_badblocks_list_free(bb);
			फर्काऊ retval;
		}
	}
	*ret = bb;
	फर्काऊ 0;
}

स्थिर शून्य print_list(badblocks_list bb, पूर्णाङ्क verify)
{
	errcode_t	retval;
	badblocks_iterate	iter;
	blk_t			blk;
	पूर्णाङ्क			i, ok;

	retval = ext2fs_badblocks_list_iterate_begin(bb, &iter);
	यदि (retval) {
		com_err("print_list", retval, "while setting up iterator");
		फर्काऊ;
	}
	ok = i = 1;
	जबसम्म (ext2fs_badblocks_list_iterate(iter, &blk)) {
		म_लिखो("%u ", blk);
		यदि (i++ != blk)
			ok = 0;
	}
	ext2fs_badblocks_list_iterate_end(iter);
	यदि (verify) {
		यदि (ok)
			म_लिखो("--- OK");
		अन्यथा {
			म_लिखो("--- NOT OK");
			test_fail++;
		}
	}
}

स्थिर शून्य validate_test_seq(badblocks_list bb, blk_t *vec)
{
	पूर्णाङ्क	i, match, ok;

	लागि (i = 0; vec[i]; i += 2) {
		match = ext2fs_badblocks_list_test(bb, vec[i]);
		यदि (match == vec[i+1])
			ok = 1;
		अन्यथा {
			ok = 0;
			test_fail++;
		}
		म_लिखो("\tblock %u is %s --- %s\n", vec[i],
		       match ? "present" : "absent",
		       ok ? "OK" : "NOT OK");
	}
}

स्थिर शून्य do_test_seq(badblocks_list bb, blk_t *vec)
{
	पूर्णाङ्क	i, match;

	लागि (i = 0; vec[i]; i += 2) {
		विकल्प (vec[i+1]) {
		पक्ष ADD_BLK:
			ext2fs_badblocks_list_add(bb, vec[i]);
			match = ext2fs_badblocks_list_test(bb, vec[i]);
			म_लिखो("Adding block %u --- now %s\n", vec[i],
			       match ? "present" : "absent");
			यदि (!match) {
				म_लिखो("FAILURE!\n");
				test_fail++;
			}
			रोक;
		पक्ष DEL_BLK:
			ext2fs_badblocks_list_del(bb, vec[i]);
			match = ext2fs_badblocks_list_test(bb, vec[i]);
			म_लिखो("Removing block %u --- now %s\n", vec[i],
			       ext2fs_badblocks_list_test(bb, vec[i]) ?
			       "present" : "absent");
			यदि (match) {
				म_लिखो("FAILURE!\n");
				test_fail++;
			}
			रोक;
		}
	}
}


पूर्णाङ्क file_test(badblocks_list bb)
{
	badblocks_list new_bb = 0;
	errcode_t	retval;
	खाता	*f;

	f = क्षणिक_ख();
	यदि (!f) {
		ख_लिखो(मानक_त्रुटि, "Error opening temp file: %s\n",
			error_message(त्रुटि_सं));
		फर्काऊ 1;
	}
	retval = ext2fs_write_bb_FILE(bb, 0, f);
	यदि (retval) {
		com_err("file_test", retval, "while writing bad blocks");
		फर्काऊ 1;
	}

	शुरुआत(f);
	retval = ext2fs_read_bb_FILE2(0, f, &new_bb, 0, 0);
	यदि (retval) {
		com_err("file_test", retval, "while reading bad blocks");
		फर्काऊ 1;
	}
	ख_बंद(f);

	यदि (ext2fs_badblocks_equal(bb, new_bb)) {
		म_लिखो("Block bitmap matched after reading and writing.\n");
	} अन्यथा {
		म_लिखो("Block bitmap NOT matched.\n");
		test_fail++;
	}
	ext2fs_badblocks_list_free(new_bb);
	फर्काऊ 0;
}

स्थिर शून्य invalid_proc(ext2_filsys fs, blk_t blk)
{
	यदि (blk == 34500) {
		म_लिखो("Expected invalid block\n");
		test_expected_fail++;
	} अन्यथा {
		म_लिखो("Invalid block #: %u\n", blk);
		test_fail++;
	}
}

शून्य file_test_invalid(badblocks_list bb)
{
	badblocks_list new_bb = 0;
	errcode_t	retval;
	ext2_filsys 	fs;
	खाता	*f;

	fs = दो_स्मृति(परिमाण(संरचना struct_ext2_filsys));
	स_रखो(fs, 0, परिमाण(संरचना struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->super = दो_स्मृति(SUPERBLOCK_SIZE);
	स_रखो(fs->super, 0, SUPERBLOCK_SIZE);
	fs->super->s_first_data_block = 1;
	ext2fs_blocks_गन्ती_set(fs->super, 100);

	f = क्षणिक_ख();
	यदि (!f) {
		ख_लिखो(मानक_त्रुटि, "Error opening temp file: %s\n",
			error_message(त्रुटि_सं));
		test_fail++;
		गच्छ out;
	}
	retval = ext2fs_write_bb_FILE(bb, 0, f);
	यदि (retval) {
		com_err("file_test", retval, "while writing bad blocks");
		test_fail++;
		गच्छ out;
	}
	ख_लिखो(f, "34500\n");

	शुरुआत(f);
	test_expected_fail = 0;
	retval = ext2fs_read_bb_FILE(fs, f, &new_bb, invalid_proc);
	यदि (retval) {
		com_err("file_test", retval, "while reading bad blocks");
		test_fail++;
		गच्छ out;
	}
	ख_बंद(f);
	यदि (!test_expected_fail) {
		म_लिखो("Expected test failure didn't happen!\n");
		test_fail++;
	}


	यदि (ext2fs_badblocks_equal(bb, new_bb)) {
		म_लिखो("Block bitmap matched after reading and writing.\n");
	} अन्यथा {
		म_लिखो("Block bitmap NOT matched.\n");
		test_fail++;
	}
	ext2fs_badblocks_list_free(new_bb);
out:
	मुक्त(fs->super);
	मुक्त(fs);
}

पूर्णाङ्क main(पूर्णाङ्क argc, वर्ण **argv)
{
	badblocks_list bb1, bb2, bb3, bb4, bb5;
	पूर्णाङ्क	equal;
	errcode_t	retval;

	add_error_table(&et_ext2_error_table);

	bb1 = bb2 = bb3 = bb4 = bb5 = 0;

	म_लिखो("test1: ");
	retval = create_test_list(test1, &bb1);
	यदि (retval == 0)
		print_list(bb1, 1);
	म_लिखो("\n");

	म_लिखो("test2: ");
	retval = create_test_list(test2, &bb2);
	यदि (retval == 0)
		print_list(bb2, 1);
	म_लिखो("\n");

	म_लिखो("test3: ");
	retval = create_test_list(test3, &bb3);
	यदि (retval == 0)
		print_list(bb3, 1);
	म_लिखो("\n");

	म_लिखो("test4: ");
	retval = create_test_list(test4, &bb4);
	यदि (retval == 0) {
		print_list(bb4, 0);
		म_लिखो("\n");
		validate_test_seq(bb4, test4a);
	}
	म_लिखो("\n");

	म_लिखो("test5: ");
	retval = create_test_list(test5, &bb5);
	यदि (retval == 0) {
		print_list(bb5, 0);
		म_लिखो("\n");
		do_test_seq(bb5, test5a);
		म_लिखो("After test5 sequence: ");
		print_list(bb5, 0);
		म_लिखो("\n");
	}
	म_लिखो("\n");

	यदि (bb1 && bb2 && bb3 && bb4 && bb5) {
		म_लिखो("Comparison tests:\n");
		equal = ext2fs_badblocks_equal(bb1, bb2);
		म_लिखो("bb1 and bb2 are %sequal.\n", equal ? "" : "NOT ");
		यदि (equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb1, bb3);
		म_लिखो("bb1 and bb3 are %sequal.\n", equal ? "" : "NOT ");
		यदि (!equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb1, bb4);
		म_लिखो("bb1 and bb4 are %sequal.\n", equal ? "" : "NOT ");
		यदि (equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb4, bb5);
		म_लिखो("bb4 and bb5 are %sequal.\n", equal ? "" : "NOT ");
		यदि (!equal)
			test_fail++;
		म_लिखो("\n");
	}

	file_test(bb4);

	file_test_invalid(bb4);

	यदि (test_fail == 0)
		म_लिखो("ext2fs library badblocks tests checks out OK!\n");

	यदि (bb1)
		ext2fs_badblocks_list_free(bb1);
	यदि (bb2)
		ext2fs_badblocks_list_free(bb2);
	यदि (bb3)
		ext2fs_badblocks_list_free(bb3);
	यदि (bb4)
		ext2fs_badblocks_list_free(bb4);
	यदि (bb5)
		ext2fs_badblocks_list_free(bb5);

	फर्काऊ test_fail;

}
