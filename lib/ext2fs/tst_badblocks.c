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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

#ವ್ಯಾಖ್ಯಾನಿಸು ಕೂಡಿಸು_BLK	0x0001
#ವ್ಯಾಖ್ಯಾನಿಸು DEL_BLK	0x0002

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
	50, ಕೂಡಿಸು_BLK,
	51, DEL_BLK,
	57, DEL_BLK,
	66, ಕೂಡಿಸು_BLK,
	31, DEL_BLK,
	12, ಕೂಡಿಸು_BLK,
	2, ಕೂಡಿಸು_BLK,
	13, ಕೂಡಿಸು_BLK,
	1, DEL_BLK,
	0
	};


ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ test_fail = 0;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ test_expected_fail = 0;

ಸ್ಥಿರ errcode_t create_test_list(blk_t *vec, badblocks_list *ret)
{
	errcode_t	retval;
	badblocks_list	bb;
	ಪೂರ್ಣಾಂಕ		i;

	retval = ext2fs_badblocks_list_create(&bb, 5);
	ಆದರೆ (retval) {
		com_err("create_test_list", retval, "while creating list");
		ಹಿಂತಿರುಗಿ retval;
	}
	ಗಾಗಿ (i=0; vec[i]; i++) {
		retval = ext2fs_badblocks_list_ಕೂಡಿಸು(bb, vec[i]);
		ಆದರೆ (retval) {
			com_err("create_test_list", retval,
				"while adding test vector %d", i);
			ext2fs_badblocks_list_free(bb);
			ಹಿಂತಿರುಗಿ retval;
		}
	}
	*ret = bb;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ print_list(badblocks_list bb, ಪೂರ್ಣಾಂಕ verify)
{
	errcode_t	retval;
	badblocks_iterate	iter;
	blk_t			blk;
	ಪೂರ್ಣಾಂಕ			i, ok;

	retval = ext2fs_badblocks_list_iterate_begin(bb, &iter);
	ಆದರೆ (retval) {
		com_err("print_list", retval, "while setting up iterator");
		ಹಿಂತಿರುಗಿ;
	}
	ok = i = 1;
	ಆಗ (ext2fs_badblocks_list_iterate(iter, &blk)) {
		ಬರೆ("%u ", blk);
		ಆದರೆ (i++ != blk)
			ok = 0;
	}
	ext2fs_badblocks_list_iterate_ಕೊನೆ(iter);
	ಆದರೆ (verify) {
		ಆದರೆ (ok)
			ಬರೆ("--- OK");
		ಇಲ್ಲದಿದ್ದರೆ {
			ಬರೆ("--- NOT OK");
			test_fail++;
		}
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ validate_test_seq(badblocks_list bb, blk_t *vec)
{
	ಪೂರ್ಣಾಂಕ	i, ಹೊಂದಿಕೆ, ok;

	ಗಾಗಿ (i = 0; vec[i]; i += 2) {
		ಹೊಂದಿಕೆ = ext2fs_badblocks_list_test(bb, vec[i]);
		ಆದರೆ (ಹೊಂದಿಕೆ == vec[i+1])
			ok = 1;
		ಇಲ್ಲದಿದ್ದರೆ {
			ok = 0;
			test_fail++;
		}
		ಬರೆ("\tblock %u is %s --- %s\n", vec[i],
		       ಹೊಂದಿಕೆ ? "present" : "absent",
		       ok ? "OK" : "NOT OK");
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ do_test_seq(badblocks_list bb, blk_t *vec)
{
	ಪೂರ್ಣಾಂಕ	i, ಹೊಂದಿಕೆ;

	ಗಾಗಿ (i = 0; vec[i]; i += 2) {
		ಆಯ್ಕೆ (vec[i+1]) {
		ಸ್ಥಿತಿ ಕೂಡಿಸು_BLK:
			ext2fs_badblocks_list_ಕೂಡಿಸು(bb, vec[i]);
			ಹೊಂದಿಕೆ = ext2fs_badblocks_list_test(bb, vec[i]);
			ಬರೆ("Adding block %u --- now %s\n", vec[i],
			       ಹೊಂದಿಕೆ ? "present" : "absent");
			ಆದರೆ (!ಹೊಂದಿಕೆ) {
				ಬರೆ("FAILURE!\n");
				test_fail++;
			}
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ DEL_BLK:
			ext2fs_badblocks_list_del(bb, vec[i]);
			ಹೊಂದಿಕೆ = ext2fs_badblocks_list_test(bb, vec[i]);
			ಬರೆ("Removing block %u --- now %s\n", vec[i],
			       ext2fs_badblocks_list_test(bb, vec[i]) ?
			       "present" : "absent");
			ಆದರೆ (ಹೊಂದಿಕೆ) {
				ಬರೆ("FAILURE!\n");
				test_fail++;
			}
			ನಿಲ್ಲಿಸು;
		}
	}
}


ಪೂರ್ಣಾಂಕ file_test(badblocks_list bb)
{
	badblocks_list new_bb = 0;
	errcode_t	retval;
	FILE	*f;

	f = tmpfile();
	ಆದರೆ (!f) {
		fprintf(stderr, "Error opening temp file: %s\n",
			error_ಸಂದೇಶ(errno));
		ಹಿಂತಿರುಗಿ 1;
	}
	retval = ext2fs_write_bb_FILE(bb, 0, f);
	ಆದರೆ (retval) {
		com_err("file_test", retval, "while writing bad blocks");
		ಹಿಂತಿರುಗಿ 1;
	}

	rewind(f);
	retval = ext2fs_read_bb_FILE2(0, f, &new_bb, 0, 0);
	ಆದರೆ (retval) {
		com_err("file_test", retval, "while reading bad blocks");
		ಹಿಂತಿರುಗಿ 1;
	}
	fclose(f);

	ಆದರೆ (ext2fs_badblocks_equal(bb, new_bb)) {
		ಬರೆ("Block bitmap matched after reading and writing.\n");
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಬರೆ("Block bitmap NOT matched.\n");
		test_fail++;
	}
	ext2fs_badblocks_list_free(new_bb);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ invalid_proc(ext2_filsys fs, blk_t blk)
{
	ಆದರೆ (blk == 34500) {
		ಬರೆ("Expected invalid block\n");
		test_expected_fail++;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಬರೆ("Invalid block #: %u\n", blk);
		test_fail++;
	}
}

ಶೂನ್ಯ file_test_invalid(badblocks_list bb)
{
	badblocks_list new_bb = 0;
	errcode_t	retval;
	ext2_filsys 	fs;
	FILE	*f;

	fs = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(ರಚನೆ struct_ext2_filsys));
	memset(fs, 0, ಗಾತ್ರ(ರಚನೆ struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->super = ಸ್ಮರಣೆ_ಕೊಡು(SUPERBLOCK_ಗಾತ್ರ);
	memset(fs->super, 0, SUPERBLOCK_ಗಾತ್ರ);
	fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ = 1;
	ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(fs->super, 100);

	f = tmpfile();
	ಆದರೆ (!f) {
		fprintf(stderr, "Error opening temp file: %s\n",
			error_ಸಂದೇಶ(errno));
		test_fail++;
		ಹೋಗು ಹೊರಹೋಗು;
	}
	retval = ext2fs_write_bb_FILE(bb, 0, f);
	ಆದರೆ (retval) {
		com_err("file_test", retval, "while writing bad blocks");
		test_fail++;
		ಹೋಗು ಹೊರಹೋಗು;
	}
	fprintf(f, "34500\n");

	rewind(f);
	test_expected_fail = 0;
	retval = ext2fs_read_bb_FILE(fs, f, &new_bb, invalid_proc);
	ಆದರೆ (retval) {
		com_err("file_test", retval, "while reading bad blocks");
		test_fail++;
		ಹೋಗು ಹೊರಹೋಗು;
	}
	fclose(f);
	ಆದರೆ (!test_expected_fail) {
		ಬರೆ("Expected test failure didn't happen!\n");
		test_fail++;
	}


	ಆದರೆ (ext2fs_badblocks_equal(bb, new_bb)) {
		ಬರೆ("Block bitmap matched after reading and writing.\n");
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಬರೆ("Block bitmap NOT matched.\n");
		test_fail++;
	}
	ext2fs_badblocks_list_free(new_bb);
ಹೊರಹೋಗು:
	ಬಿಡುಗಡೆ(fs->super);
	ಬಿಡುಗಡೆ(fs);
}

ಪೂರ್ಣಾಂಕ ಮುಖ್ಯ(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
	badblocks_list bb1, bb2, bb3, bb4, bb5;
	ಪೂರ್ಣಾಂಕ	equal;
	errcode_t	retval;

	ಕೂಡಿಸು_error_ಕೋಷ್ಟಕ(&et_ext2_error_ಕೋಷ್ಟಕ);

	bb1 = bb2 = bb3 = bb4 = bb5 = 0;

	ಬರೆ("test1: ");
	retval = create_test_list(test1, &bb1);
	ಆದರೆ (retval == 0)
		print_list(bb1, 1);
	ಬರೆ("\n");

	ಬರೆ("test2: ");
	retval = create_test_list(test2, &bb2);
	ಆದರೆ (retval == 0)
		print_list(bb2, 1);
	ಬರೆ("\n");

	ಬರೆ("test3: ");
	retval = create_test_list(test3, &bb3);
	ಆದರೆ (retval == 0)
		print_list(bb3, 1);
	ಬರೆ("\n");

	ಬರೆ("test4: ");
	retval = create_test_list(test4, &bb4);
	ಆದರೆ (retval == 0) {
		print_list(bb4, 0);
		ಬರೆ("\n");
		validate_test_seq(bb4, test4a);
	}
	ಬರೆ("\n");

	ಬರೆ("test5: ");
	retval = create_test_list(test5, &bb5);
	ಆದರೆ (retval == 0) {
		print_list(bb5, 0);
		ಬರೆ("\n");
		do_test_seq(bb5, test5a);
		ಬರೆ("After test5 sequence: ");
		print_list(bb5, 0);
		ಬರೆ("\n");
	}
	ಬರೆ("\n");

	ಆದರೆ (bb1 && bb2 && bb3 && bb4 && bb5) {
		ಬರೆ("Comparison tests:\n");
		equal = ext2fs_badblocks_equal(bb1, bb2);
		ಬರೆ("bb1 and bb2 are %sequal.\n", equal ? "" : "NOT ");
		ಆದರೆ (equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb1, bb3);
		ಬರೆ("bb1 and bb3 are %sequal.\n", equal ? "" : "NOT ");
		ಆದರೆ (!equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb1, bb4);
		ಬರೆ("bb1 and bb4 are %sequal.\n", equal ? "" : "NOT ");
		ಆದರೆ (equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb4, bb5);
		ಬರೆ("bb4 and bb5 are %sequal.\n", equal ? "" : "NOT ");
		ಆದರೆ (!equal)
			test_fail++;
		ಬರೆ("\n");
	}

	file_test(bb4);

	file_test_invalid(bb4);

	ಆದರೆ (test_fail == 0)
		ಬರೆ("ext2fs library badblocks tests checks out OK!\n");

	ಆದರೆ (bb1)
		ext2fs_badblocks_list_free(bb1);
	ಆದರೆ (bb2)
		ext2fs_badblocks_list_free(bb2);
	ಆದರೆ (bb3)
		ext2fs_badblocks_list_free(bb3);
	ಆದರೆ (bb4)
		ext2fs_badblocks_list_free(bb4);
	ಆದರೆ (bb5)
		ext2fs_badblocks_list_free(bb5);

	ಹಿಂತಿರುಗಿ test_fail;

}
