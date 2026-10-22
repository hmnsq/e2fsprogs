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

#घोषणा ADD_BLK	0x0001
#घोषणा DEL_BLK	0x0002

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


अटल पूर्णांक test_fail = 0;
अटल पूर्णांक test_expected_fail = 0;

अटल errcode_t create_test_list(blk_t *vec, badblocks_list *ret)
{
	errcode_t	retval;
	badblocks_list	bb;
	पूर्णांक		i;

	retval = ext2fs_badblocks_list_create(&bb, 5);
	अगर (retval) {
		com_err("create_test_list", retval, "while creating list");
		वापस retval;
	}
	क्रम (i=0; vec[i]; i++) {
		retval = ext2fs_badblocks_list_add(bb, vec[i]);
		अगर (retval) {
			com_err("create_test_list", retval,
				"while adding test vector %d", i);
			ext2fs_badblocks_list_free(bb);
			वापस retval;
		}
	}
	*ret = bb;
	वापस 0;
}

अटल व्योम print_list(badblocks_list bb, पूर्णांक verify)
{
	errcode_t	retval;
	badblocks_iterate	iter;
	blk_t			blk;
	पूर्णांक			i, ok;

	retval = ext2fs_badblocks_list_iterate_begin(bb, &iter);
	अगर (retval) {
		com_err("print_list", retval, "while setting up iterator");
		वापस;
	}
	ok = i = 1;
	जबतक (ext2fs_badblocks_list_iterate(iter, &blk)) {
		म_लिखो("%u ", blk);
		अगर (i++ != blk)
			ok = 0;
	}
	ext2fs_badblocks_list_iterate_end(iter);
	अगर (verify) {
		अगर (ok)
			म_लिखो("--- OK");
		अन्यथा {
			म_लिखो("--- NOT OK");
			test_fail++;
		}
	}
}

अटल व्योम validate_test_seq(badblocks_list bb, blk_t *vec)
{
	पूर्णांक	i, match, ok;

	क्रम (i = 0; vec[i]; i += 2) {
		match = ext2fs_badblocks_list_test(bb, vec[i]);
		अगर (match == vec[i+1])
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

अटल व्योम do_test_seq(badblocks_list bb, blk_t *vec)
{
	पूर्णांक	i, match;

	क्रम (i = 0; vec[i]; i += 2) {
		चयन (vec[i+1]) {
		हाल ADD_BLK:
			ext2fs_badblocks_list_add(bb, vec[i]);
			match = ext2fs_badblocks_list_test(bb, vec[i]);
			म_लिखो("Adding block %u --- now %s\n", vec[i],
			       match ? "present" : "absent");
			अगर (!match) {
				म_लिखो("FAILURE!\n");
				test_fail++;
			}
			अवरोध;
		हाल DEL_BLK:
			ext2fs_badblocks_list_del(bb, vec[i]);
			match = ext2fs_badblocks_list_test(bb, vec[i]);
			म_लिखो("Removing block %u --- now %s\n", vec[i],
			       ext2fs_badblocks_list_test(bb, vec[i]) ?
			       "present" : "absent");
			अगर (match) {
				म_लिखो("FAILURE!\n");
				test_fail++;
			}
			अवरोध;
		}
	}
}


पूर्णांक file_test(badblocks_list bb)
{
	badblocks_list new_bb = 0;
	errcode_t	retval;
	खाता	*f;

	f = क्षणिक_ख();
	अगर (!f) {
		ख_लिखो(मानक_त्रुटि, "Error opening temp file: %s\n",
			error_message(त्रुटि_सं));
		वापस 1;
	}
	retval = ext2fs_write_bb_FILE(bb, 0, f);
	अगर (retval) {
		com_err("file_test", retval, "while writing bad blocks");
		वापस 1;
	}

	शुरुआत(f);
	retval = ext2fs_read_bb_FILE2(0, f, &new_bb, 0, 0);
	अगर (retval) {
		com_err("file_test", retval, "while reading bad blocks");
		वापस 1;
	}
	ख_बंद(f);

	अगर (ext2fs_badblocks_equal(bb, new_bb)) {
		म_लिखो("Block bitmap matched after reading and writing.\n");
	} अन्यथा {
		म_लिखो("Block bitmap NOT matched.\n");
		test_fail++;
	}
	ext2fs_badblocks_list_free(new_bb);
	वापस 0;
}

अटल व्योम invalid_proc(ext2_filsys fs, blk_t blk)
{
	अगर (blk == 34500) {
		म_लिखो("Expected invalid block\n");
		test_expected_fail++;
	} अन्यथा {
		म_लिखो("Invalid block #: %u\n", blk);
		test_fail++;
	}
}

व्योम file_test_invalid(badblocks_list bb)
{
	badblocks_list new_bb = 0;
	errcode_t	retval;
	ext2_filsys 	fs;
	खाता	*f;

	fs = दो_स्मृति(माप(काष्ठा struct_ext2_filsys));
	स_रखो(fs, 0, माप(काष्ठा struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->super = दो_स्मृति(SUPERBLOCK_आकार);
	स_रखो(fs->super, 0, SUPERBLOCK_आकार);
	fs->super->s_first_तथ्य_block = 1;
	ext2fs_blocks_गिनती_set(fs->super, 100);

	f = क्षणिक_ख();
	अगर (!f) {
		ख_लिखो(मानक_त्रुटि, "Error opening temp file: %s\n",
			error_message(त्रुटि_सं));
		test_fail++;
		जाओ out;
	}
	retval = ext2fs_write_bb_FILE(bb, 0, f);
	अगर (retval) {
		com_err("file_test", retval, "while writing bad blocks");
		test_fail++;
		जाओ out;
	}
	ख_लिखो(f, "34500\n");

	शुरुआत(f);
	test_expected_fail = 0;
	retval = ext2fs_read_bb_FILE(fs, f, &new_bb, invalid_proc);
	अगर (retval) {
		com_err("file_test", retval, "while reading bad blocks");
		test_fail++;
		जाओ out;
	}
	ख_बंद(f);
	अगर (!test_expected_fail) {
		म_लिखो("Expected test failure didn't happen!\n");
		test_fail++;
	}


	अगर (ext2fs_badblocks_equal(bb, new_bb)) {
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

पूर्णांक मुख्य(पूर्णांक argc, अक्षर **argv)
{
	badblocks_list bb1, bb2, bb3, bb4, bb5;
	पूर्णांक	equal;
	errcode_t	retval;

	add_error_table(&et_ext2_error_table);

	bb1 = bb2 = bb3 = bb4 = bb5 = 0;

	म_लिखो("test1: ");
	retval = create_test_list(test1, &bb1);
	अगर (retval == 0)
		print_list(bb1, 1);
	म_लिखो("\n");

	म_लिखो("test2: ");
	retval = create_test_list(test2, &bb2);
	अगर (retval == 0)
		print_list(bb2, 1);
	म_लिखो("\n");

	म_लिखो("test3: ");
	retval = create_test_list(test3, &bb3);
	अगर (retval == 0)
		print_list(bb3, 1);
	म_लिखो("\n");

	म_लिखो("test4: ");
	retval = create_test_list(test4, &bb4);
	अगर (retval == 0) {
		print_list(bb4, 0);
		म_लिखो("\n");
		validate_test_seq(bb4, test4a);
	}
	म_लिखो("\n");

	म_लिखो("test5: ");
	retval = create_test_list(test5, &bb5);
	अगर (retval == 0) {
		print_list(bb5, 0);
		म_लिखो("\n");
		do_test_seq(bb5, test5a);
		म_लिखो("After test5 sequence: ");
		print_list(bb5, 0);
		म_लिखो("\n");
	}
	म_लिखो("\n");

	अगर (bb1 && bb2 && bb3 && bb4 && bb5) {
		म_लिखो("Comparison tests:\n");
		equal = ext2fs_badblocks_equal(bb1, bb2);
		म_लिखो("bb1 and bb2 are %sequal.\n", equal ? "" : "NOT ");
		अगर (equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb1, bb3);
		म_लिखो("bb1 and bb3 are %sequal.\n", equal ? "" : "NOT ");
		अगर (!equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb1, bb4);
		म_लिखो("bb1 and bb4 are %sequal.\n", equal ? "" : "NOT ");
		अगर (equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb4, bb5);
		म_लिखो("bb4 and bb5 are %sequal.\n", equal ? "" : "NOT ");
		अगर (!equal)
			test_fail++;
		म_लिखो("\n");
	}

	file_test(bb4);

	file_test_invalid(bb4);

	अगर (test_fail == 0)
		म_लिखो("ext2fs library badblocks tests checks out OK!\n");

	अगर (bb1)
		ext2fs_badblocks_list_free(bb1);
	अगर (bb2)
		ext2fs_badblocks_list_free(bb2);
	अगर (bb3)
		ext2fs_badblocks_list_free(bb3);
	अगर (bb4)
		ext2fs_badblocks_list_free(bb4);
	अगर (bb5)
		ext2fs_badblocks_list_free(bb5);

	वापस test_fail;

}
