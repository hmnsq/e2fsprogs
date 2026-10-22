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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <sys/types.h>
#ఒకవేళ HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

#నిర్వచించు కూడు_BLK	0x0001
#నిర్వచించు DEL_BLK	0x0002

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
	50, కూడు_BLK,
	51, DEL_BLK,
	57, DEL_BLK,
	66, కూడు_BLK,
	31, DEL_BLK,
	12, కూడు_BLK,
	2, కూడు_BLK,
	13, కూడు_BLK,
	1, DEL_BLK,
	0
	};


స్థిర పూర్ణాంకం test_fail = 0;
స్థిర పూర్ణాంకం test_expected_fail = 0;

స్థిర errcode_t create_test_list(blk_t *vec, badblocks_list *ret)
{
	errcode_t	retval;
	badblocks_list	bb;
	పూర్ణాంకం		i;

	retval = ext2fs_badblocks_list_create(&bb, 5);
	ఒకవేళ (retval) {
		com_err("create_test_list", retval, "while creating list");
		తిరిగి retval;
	}
	కొరకు (i=0; vec[i]; i++) {
		retval = ext2fs_badblocks_list_కూడు(bb, vec[i]);
		ఒకవేళ (retval) {
			com_err("create_test_list", retval,
				"while adding test vector %d", i);
			ext2fs_badblocks_list_free(bb);
			తిరిగి retval;
		}
	}
	*ret = bb;
	తిరిగి 0;
}

స్థిర శూన్య print_list(badblocks_list bb, పూర్ణాంకం verify)
{
	errcode_t	retval;
	badblocks_iterate	iter;
	blk_t			blk;
	పూర్ణాంకం			i, ok;

	retval = ext2fs_badblocks_list_iterate_begin(bb, &iter);
	ఒకవేళ (retval) {
		com_err("print_list", retval, "while setting up iterator");
		తిరిగి;
	}
	ok = i = 1;
	అయితే (ext2fs_badblocks_list_iterate(iter, &blk)) {
		రాయి("%u ", blk);
		ఒకవేళ (i++ != blk)
			ok = 0;
	}
	ext2fs_badblocks_list_iterate_చివర(iter);
	ఒకవేళ (verify) {
		ఒకవేళ (ok)
			రాయి("--- OK");
		లేకపోతే {
			రాయి("--- NOT OK");
			test_fail++;
		}
	}
}

స్థిర శూన్య validate_test_seq(badblocks_list bb, blk_t *vec)
{
	పూర్ణాంకం	i, సరిపోలిక, ok;

	కొరకు (i = 0; vec[i]; i += 2) {
		సరిపోలిక = ext2fs_badblocks_list_test(bb, vec[i]);
		ఒకవేళ (సరిపోలిక == vec[i+1])
			ok = 1;
		లేకపోతే {
			ok = 0;
			test_fail++;
		}
		రాయి("\tblock %u is %s --- %s\n", vec[i],
		       సరిపోలిక ? "present" : "absent",
		       ok ? "OK" : "NOT OK");
	}
}

స్థిర శూన్య do_test_seq(badblocks_list bb, blk_t *vec)
{
	పూర్ణాంకం	i, సరిపోలిక;

	కొరకు (i = 0; vec[i]; i += 2) {
		ఎంపిక (vec[i+1]) {
		స్థితి కూడు_BLK:
			ext2fs_badblocks_list_కూడు(bb, vec[i]);
			సరిపోలిక = ext2fs_badblocks_list_test(bb, vec[i]);
			రాయి("Adding block %u --- now %s\n", vec[i],
			       సరిపోలిక ? "present" : "absent");
			ఒకవేళ (!సరిపోలిక) {
				రాయి("FAILURE!\n");
				test_fail++;
			}
			ఆపు;
		స్థితి DEL_BLK:
			ext2fs_badblocks_list_del(bb, vec[i]);
			సరిపోలిక = ext2fs_badblocks_list_test(bb, vec[i]);
			రాయి("Removing block %u --- now %s\n", vec[i],
			       ext2fs_badblocks_list_test(bb, vec[i]) ?
			       "present" : "absent");
			ఒకవేళ (సరిపోలిక) {
				రాయి("FAILURE!\n");
				test_fail++;
			}
			ఆపు;
		}
	}
}


పూర్ణాంకం file_test(badblocks_list bb)
{
	badblocks_list new_bb = 0;
	errcode_t	retval;
	FILE	*f;

	f = tmpfile();
	ఒకవేళ (!f) {
		fprintf(stderr, "Error opening temp file: %s\n",
			error_సందేశం(errno));
		తిరిగి 1;
	}
	retval = ext2fs_write_bb_FILE(bb, 0, f);
	ఒకవేళ (retval) {
		com_err("file_test", retval, "while writing bad blocks");
		తిరిగి 1;
	}

	rewind(f);
	retval = ext2fs_read_bb_FILE2(0, f, &new_bb, 0, 0);
	ఒకవేళ (retval) {
		com_err("file_test", retval, "while reading bad blocks");
		తిరిగి 1;
	}
	fclose(f);

	ఒకవేళ (ext2fs_badblocks_equal(bb, new_bb)) {
		రాయి("Block bitmap matched after reading and writing.\n");
	} లేకపోతే {
		రాయి("Block bitmap NOT matched.\n");
		test_fail++;
	}
	ext2fs_badblocks_list_free(new_bb);
	తిరిగి 0;
}

స్థిర శూన్య invalid_proc(ext2_filsys fs, blk_t blk)
{
	ఒకవేళ (blk == 34500) {
		రాయి("Expected invalid block\n");
		test_expected_fail++;
	} లేకపోతే {
		రాయి("Invalid block #: %u\n", blk);
		test_fail++;
	}
}

శూన్య file_test_invalid(badblocks_list bb)
{
	badblocks_list new_bb = 0;
	errcode_t	retval;
	ext2_filsys 	fs;
	FILE	*f;

	fs = జ్ఞాపకం_ఇవ్వు(పరిమాణం(నిర్మాణం struct_ext2_filsys));
	memset(fs, 0, పరిమాణం(నిర్మాణం struct_ext2_filsys));
	fs->magic = EXT2_ET_MAGIC_EXT2FS_FILSYS;
	fs->super = జ్ఞాపకం_ఇవ్వు(SUPERBLOCK_కొలత);
	memset(fs->super, 0, SUPERBLOCK_కొలత);
	fs->super->s_మొదటి_దత్తాంశం_ఖండం = 1;
	ext2fs_ఖండాలు_లెక్క_సమితి(fs->super, 100);

	f = tmpfile();
	ఒకవేళ (!f) {
		fprintf(stderr, "Error opening temp file: %s\n",
			error_సందేశం(errno));
		test_fail++;
		వెళ్ళు బహిర్గతం;
	}
	retval = ext2fs_write_bb_FILE(bb, 0, f);
	ఒకవేళ (retval) {
		com_err("file_test", retval, "while writing bad blocks");
		test_fail++;
		వెళ్ళు బహిర్గతం;
	}
	fprintf(f, "34500\n");

	rewind(f);
	test_expected_fail = 0;
	retval = ext2fs_read_bb_FILE(fs, f, &new_bb, invalid_proc);
	ఒకవేళ (retval) {
		com_err("file_test", retval, "while reading bad blocks");
		test_fail++;
		వెళ్ళు బహిర్గతం;
	}
	fclose(f);
	ఒకవేళ (!test_expected_fail) {
		రాయి("Expected test failure didn't happen!\n");
		test_fail++;
	}


	ఒకవేళ (ext2fs_badblocks_equal(bb, new_bb)) {
		రాయి("Block bitmap matched after reading and writing.\n");
	} లేకపోతే {
		రాయి("Block bitmap NOT matched.\n");
		test_fail++;
	}
	ext2fs_badblocks_list_free(new_bb);
బహిర్గతం:
	విడుదల(fs->super);
	విడుదల(fs);
}

పూర్ణాంకం ప్రధాన(పూర్ణాంకం argc, అక్షరం **argv)
{
	badblocks_list bb1, bb2, bb3, bb4, bb5;
	పూర్ణాంకం	equal;
	errcode_t	retval;

	కూడు_error_పట్టిక(&et_ext2_error_పట్టిక);

	bb1 = bb2 = bb3 = bb4 = bb5 = 0;

	రాయి("test1: ");
	retval = create_test_list(test1, &bb1);
	ఒకవేళ (retval == 0)
		print_list(bb1, 1);
	రాయి("\n");

	రాయి("test2: ");
	retval = create_test_list(test2, &bb2);
	ఒకవేళ (retval == 0)
		print_list(bb2, 1);
	రాయి("\n");

	రాయి("test3: ");
	retval = create_test_list(test3, &bb3);
	ఒకవేళ (retval == 0)
		print_list(bb3, 1);
	రాయి("\n");

	రాయి("test4: ");
	retval = create_test_list(test4, &bb4);
	ఒకవేళ (retval == 0) {
		print_list(bb4, 0);
		రాయి("\n");
		validate_test_seq(bb4, test4a);
	}
	రాయి("\n");

	రాయి("test5: ");
	retval = create_test_list(test5, &bb5);
	ఒకవేళ (retval == 0) {
		print_list(bb5, 0);
		రాయి("\n");
		do_test_seq(bb5, test5a);
		రాయి("After test5 sequence: ");
		print_list(bb5, 0);
		రాయి("\n");
	}
	రాయి("\n");

	ఒకవేళ (bb1 && bb2 && bb3 && bb4 && bb5) {
		రాయి("Comparison tests:\n");
		equal = ext2fs_badblocks_equal(bb1, bb2);
		రాయి("bb1 and bb2 are %sequal.\n", equal ? "" : "NOT ");
		ఒకవేళ (equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb1, bb3);
		రాయి("bb1 and bb3 are %sequal.\n", equal ? "" : "NOT ");
		ఒకవేళ (!equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb1, bb4);
		రాయి("bb1 and bb4 are %sequal.\n", equal ? "" : "NOT ");
		ఒకవేళ (equal)
			test_fail++;

		equal = ext2fs_badblocks_equal(bb4, bb5);
		రాయి("bb4 and bb5 are %sequal.\n", equal ? "" : "NOT ");
		ఒకవేళ (!equal)
			test_fail++;
		రాయి("\n");
	}

	file_test(bb4);

	file_test_invalid(bb4);

	ఒకవేళ (test_fail == 0)
		రాయి("ext2fs library badblocks tests checks out OK!\n");

	ఒకవేళ (bb1)
		ext2fs_badblocks_list_free(bb1);
	ఒకవేళ (bb2)
		ext2fs_badblocks_list_free(bb2);
	ఒకవేళ (bb3)
		ext2fs_badblocks_list_free(bb3);
	ఒకవేళ (bb4)
		ext2fs_badblocks_list_free(bb4);
	ఒకవేళ (bb5)
		ext2fs_badblocks_list_free(bb5);

	తిరిగి test_fail;

}
