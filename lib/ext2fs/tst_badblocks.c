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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ HAVE_ERRNO_H
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_BLK	0x0001
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEL_BLK	0x0002

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
	50, ਜੋੜੋ_BLK,
	51, DEL_BLK,
	57, DEL_BLK,
	66, ਜੋੜੋ_BLK,
	31, DEL_BLK,
	12, ਜੋੜੋ_BLK,
	2, ਜੋੜੋ_BLK,
	13, ਜੋੜੋ_BLK,
	1, DEL_BLK,
	0
	};


ਸਥਿਰ ਪੂਰਨ test_fail = 0;
ਸਥਿਰ ਪੂਰਨ test_expected_fail = 0;

ਸਥਿਰ errcode_t create_test_list(blk_t *vec, badblocks_list *ret)
{
	errcode_t	retval;
	badblocks_list	bb;
	ਪੂਰਨ		i;

	retval = ext2fs_badblocks_list_create(&bb, 5);
	ਜੇ (retval) {
		com_err("create_test_list", retval, "while creating list");
		ਵਾਪਸ retval;
	}
	ਲਈ (i=0; vec[i]; i++) {
		retval = ext2fs_badblocks_list_ਜੋੜੋ(bb, vec[i]);
		ਜੇ (retval) {
			com_err("create_test_list", retval,
				"while adding test vector %d", i);
			ext2fs_badblocks_list_free(bb);
			ਵਾਪਸ retval;
		}
	}
	*ret = bb;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ print_list(badblocks_list bb, ਪੂਰਨ verify)
{
	errcode_t	retval;
	badblocks_iterate	iter;
	blk_t			blk;
	ਪੂਰਨ			i, ok;

	retval = ext2fs_badblocks_list_iterate_begin(bb, &iter);
	ਜੇ (retval) {
		com_err("print_list", retval, "while setting up iterator");
		ਵਾਪਸ;
	}
	ok = i = 1;
	ਜਦੋਂ (ext2fs_badblocks_list_iterate(iter, &blk)) {
		ਲਿਖੋ("%u ", blk);
		ਜੇ (i++ != blk)
			ok = 0;
	}
	ext2fs_badblocks_list_iterate_ਸਿਰਾ(iter);
	ਜੇ (verify) {
		ਜੇ (ok)
			ਲਿਖੋ("--- OK");
		ਨਹੀਂਤਾਂ {
			ਲਿਖੋ("--- NOT OK");
			test_fail++;
		}
	}
}

ਸਥਿਰ ਖਾਲੀ validate_test_seq(badblocks_list bb, blk_t *vec)
{
	ਪੂਰਨ	i, ਮੇਲ, ok;

	ਲਈ (i = 0; vec[i]; i += 2) {
		ਮੇਲ = ext2fs_badblocks_list_test(bb, vec[i]);
		ਜੇ (ਮੇਲ == vec[i+1])
			ok = 1;
		ਨਹੀਂਤਾਂ {
			ok = 0;
			test_fail++;
		}
		ਲਿਖੋ("\tblock %u is %s --- %s\n", vec[i],
		       ਮੇਲ ? "present" : "absent",
		       ok ? "OK" : "NOT OK");
	}
}

ਸਥਿਰ ਖਾਲੀ do_test_seq(badblocks_list bb, blk_t *vec)
{
	ਪੂਰਨ	i, ਮੇਲ;

	ਲਈ (i = 0; vec[i]; i += 2) {
		ਚੋਣ (vec[i+1]) {
		ਹਾਲਤ ਜੋੜੋ_BLK:
			ext2fs_badblocks_list_ਜੋੜੋ(bb, vec[i]);
			ਮੇਲ = ext2fs_badblocks_list_test(bb, vec[i]);
			ਲਿਖੋ("Adding block %u --- now %s\n", vec[i],
			       ਮੇਲ ? "present" : "absent");
			ਜੇ (!ਮੇਲ) {
				ਲਿਖੋ("FAILURE!\n");
				test_fail++;
			}
			ਰੋਕੋ;
		ਹਾਲਤ DEL_BLK:
			ext2fs_badblocks_list_del(bb, vec[i]);
			ਮੇਲ = ext2fs_badblocks_list_test(bb, vec[i]);
			ਲਿਖੋ("Removing block %u --- now %s\n", vec[i],
			       ext2fs_badblocks_list_test(bb, vec[i]) ?
			       "present" : "absent");
			ਜੇ (ਮੇਲ) {
				ਲਿਖੋ("FAILURE!\n");
				test_fail++;
			}
			ਰੋਕੋ;
		}
	}
}


ਪੂਰਨ file_test(badblocks_list bb)
{
	badblocks_list new_bb = 0;
	errcode_t	retval;
	FILE	*f;

	f = tmpfile();
	ਜੇ (!f) {
		fprintf(stderr, "Error opening temp file: %s\n",
			error_ਸੁਨੇਹਾ(errno));
		ਵਾਪਸ 1;
	}
	retval = ext2fs_write_bb_FILE(bb, 0, f);
	ਜੇ (retval) {
		com_err("file_test", retval, "while writing bad blocks");
		ਵਾਪਸ 1;
	}

	rewind(f);
	retval = ext2fs_read_bb_FILE2(0, f, &new_bb, 0, 0);
	ਜੇ (retval) {
		com_err("file_test", retval, "while reading bad blocks");
		ਵਾਪਸ 1;
	}
	fclose(f);

	ਜੇ (ext2fs_badblocks_equal(bb, new_bb)) {
		ਲਿਖੋ("Block bitmap matched after reading and writing.\n");
	} ਨਹੀਂਤਾਂ {
		ਲਿਖੋ("Block bitmap NOT matched.\n");
		test_fail++;
	}
	ext2fs_badblocks_list_free(new_bb);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ invalid_proc(ext2_filsys fs, blk_t blk)
{
	ਜੇ (blk == 34500) {
		ਲਿਖੋ("Expected invalid block\n");
		test_expected_fail++;
	} ਨਹੀਂਤਾਂ {
		ਲਿਖੋ("Invalid block #: %u\n", blk);
		test_fail++;
	}
}

ਖਾਲੀ file_test_invalid(badblocks_list bb)
{
	badblocks_list new_bb = 0;
	errcode_t	retval;
	ext2_filsys 	fs;
	FILE	*f;

	fs = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(ਬਣਤਰ struct_ext2_filsys));
	memset(fs, 0, ਆਕਾਰ(ਬਣਤਰ struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->super = ਸਮ੍ਰਿਤੀ_ਦਿਓ(SUPERBLOCK_ਮਾਪ);
	memset(fs->super, 0, SUPERBLOCK_ਮਾਪ);
	fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ = 1;
	ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(fs->super, 100);

	f = tmpfile();
	ਜੇ (!f) {
		fprintf(stderr, "Error opening temp file: %s\n",
			error_ਸੁਨੇਹਾ(errno));
		test_fail++;
		ਜਾਓ ਬਾਹਰਲਾ;
	}
	retval = ext2fs_write_bb_FILE(bb, 0, f);
	ਜੇ (retval) {
		com_err("file_test", retval, "while writing bad blocks");
		test_fail++;
		ਜਾਓ ਬਾਹਰਲਾ;
	}
	fprintf(f, "34500\n");

	rewind(f);
	test_expected_fail = 0;
	retval = ext2fs_read_bb_FILE(fs, f, &new_bb, invalid_proc);
	ਜੇ (retval) {
		com_err("file_test", retval, "while reading bad blocks");
		test_fail++;
		ਜਾਓ ਬਾਹਰਲਾ;
	}
	fclose(f);
	ਜੇ (!test_expected_fail) {
		ਲਿਖੋ("Expected test failure didn't happen!\n");
		test_fail++;
	}


	ਜੇ (ext2fs_badblocks_equal(bb, new_bb)) {
		ਲਿਖੋ("Block bitmap matched after reading and writing.\n");
	} ਨਹੀਂਤਾਂ {
		ਲਿਖੋ("Block bitmap NOT matched.\n");
		test_fail++;
	}
	ext2fs_badblocks_list_free(new_bb);
ਬਾਹਰਲਾ:
	ਮੁਕਤ(fs->super);
	ਮੁਕਤ(fs);
}

ਪੂਰਨ ਮੁੱਖ(ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
	badblocks_list bb1, bb2, bb3, bb4, bb5;
	ਪੂਰਨ	equal;
	errcode_t	retval;

	ਜੋੜੋ_error_ਸਾਰਣੀ(&et_ext2_error_ਸਾਰਣੀ);

	bb1 = bb2 = bb3 = bb4 = bb5 = 0;

	ਲਿਖੋ("test1: ");
	retval = create_test_list(test1, &bb1);
	ਜੇ (retval == 0)
		print_list(bb1, 1);
	ਲਿਖੋ("\n");

	ਲਿਖੋ("test2: ");
	retval = create_test_list(test2, &bb2);
	ਜੇ (retval == 0)
		print_list(bb2, 1);
	ਲਿਖੋ("\n");

	ਲਿਖੋ("test3: ");
	retval = create_test_list(test3, &bb3);
	ਜੇ (retval == 0)
		print_list(bb3, 1);
	ਲਿਖੋ("\n");

	ਲਿਖੋ("test4: ");
	retval = create_test_list(test4, &bb4);
	ਜੇ (retval == 0) {
		print_list(bb4, 0);
		ਲਿਖੋ("\n");
		validate_test_seq(bb4, test4a);
	}
	ਲਿਖੋ("\n");

	ਲਿਖੋ("test5: ");
	retval = create_test_list(test5, &bb5);
	ਜੇ (retval == 0) {
		print_list(bb5, 0);
		ਲਿਖੋ("\n");
		do_test_seq(bb5, test5a);
		ਲਿਖੋ("After test5 sequence: ");
		print_list(bb5, 0);
		ਲਿਖੋ("\n");
	}
	ਲਿਖੋ("\n");

	ਜੇ (bb1 && bb2 && bb3 && bb4 && bb5) {
		ਲਿਖੋ("Comparison tests:\n");
		equal = ext2fs_badblocks_equal(bb1, bb2);
		ਲਿਖੋ("bb1 and bb2 are %sequal.\n", equal ? "" : "NOT ");
		ਜੇ (equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb1, bb3);
		ਲਿਖੋ("bb1 and bb3 are %sequal.\n", equal ? "" : "NOT ");
		ਜੇ (!equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb1, bb4);
		ਲਿਖੋ("bb1 and bb4 are %sequal.\n", equal ? "" : "NOT ");
		ਜੇ (equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb4, bb5);
		ਲਿਖੋ("bb4 and bb5 are %sequal.\n", equal ? "" : "NOT ");
		ਜੇ (!equal)
			test_fail++;
		ਲਿਖੋ("\n");
	}

	file_test(bb4);

	file_test_invalid(bb4);

	ਜੇ (test_fail == 0)
		ਲਿਖੋ("ext2fs library badblocks tests checks out OK!\n");

	ਜੇ (bb1)
		ext2fs_badblocks_list_free(bb1);
	ਜੇ (bb2)
		ext2fs_badblocks_list_free(bb2);
	ਜੇ (bb3)
		ext2fs_badblocks_list_free(bb3);
	ਜੇ (bb4)
		ext2fs_badblocks_list_free(bb4);
	ਜੇ (bb5)
		ext2fs_badblocks_list_free(bb5);

	ਵਾਪਸ test_fail;

}
