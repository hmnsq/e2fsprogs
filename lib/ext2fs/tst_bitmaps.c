/*
 * tst_bitmaps.c
 *
 * Copyright (C) 2011 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#అన్తర్భావ "config.h"
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <stdio.h>
#నిర్వచితమైతే HAVE_GETOPT_H
#అన్తర్భావ <getopt.h>
#అయితే_ముగింపు
#అన్తర్భావ <string.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ "ss/ss.h"

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "ext2fsP.h"

బాహ్య ss_request_పట్టిక tst_bitmaps_cmds;

స్థిర అక్షరం subsystem_name[] = "tst_bitmaps";
స్థిర అక్షరం సంస్కరణ[] = "1.0";

ext2_filsys	test_fs;
పూర్ణాంకం		exit_స్థితి = 0;

స్థిర పూర్ణాంకం ఆధారం_file(స్థిరరాశి అక్షరం *cmd_file, పూర్ణాంకం sci_idx)
{
	FILE		*f;
	అక్షరం		buf[256];
	అక్షరం		*cp;
	పూర్ణాంకం		retval;
	పూర్ణాంకం 		noecho;

	ఒకవేళ (strcmp(cmd_file, "-") == 0)
		f = stdin;
	లేకపోతే {
		f = fopen(cmd_file, "r");
		ఒకవేళ (!f) {
			perror(cmd_file);
			నిష్క్రమించు(1);
		}
	}
	fflush(stdout);
	fflush(stderr);
	setbuf(stdout, ఖాళీ);
	setbuf(stderr, ఖాళీ);
	అయితే (!feof(f)) {
		ఒకవేళ (fgets(buf, పరిమాణం(buf), f) == ఖాళీ)
			ఆపు;
		ఒకవేళ (buf[0] == '#')
			కొనసాగు;
		noecho = 0;
		ఒకవేళ (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = strchr(buf, '\n');
		ఒకవేళ (cp)
			*cp = 0;
		cp = strchr(buf, '\r');
		ఒకవేళ (cp)
			*cp = 0;
		ఒకవేళ (!noecho)
			రాయి("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_పంక్తి(sci_idx, buf);
		ఒకవేళ (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_స్థితి++;
		}
	}
	తిరిగి exit_స్థితి;
}


/*
 * This function resets the libc getopt() function, which keeps
 * internal state.  Bad design!  Stupid libc API designers!  No
 * biscuit!
 *
 * BSD-derived getopt() functions require that optind be reset to 1 in
 * order to reset getopt() state.  This used to be generally accepted
 * way of resetting getopt().  However, glibc's getopt()
 * has additional getopt() state beyond optind, and requires that
 * optind be set zero to reset its state.  So the unfortunate state of
 * affairs is that BSD-derived versions of getopt() misbehave if
 * optind is set to 0 in order to reset getopt(), and glibc's getopt()
 * will core dump if optind is set 1 in order to reset getopt().
 *
 * More modern versions of BSD require that optreset be set to 1 in
 * order to reset getopt().   Sigh.  Standards, anyone?
 *
 * We hide the hair here.
 */
శూన్య reset_getopt(శూన్య)
{
#ఒకవేళ defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#లేకపోతే
	optind = 1;
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_OPTRESET
	optreset = 1;		/* Makes BSD getopt happy */
#అయితే_ముగింపు
}

/*
 * This function will convert a string to an unsigned long, printing
 * an error message if it fails, and returning success or failure in err.
 */
గుర్తులేని దీర్ఘ parse_ulong(స్థిరరాశి అక్షరం *str, స్థిరరాశి అక్షరం *cmd,
			  స్థిరరాశి అక్షరం *descr, పూర్ణాంకం *err)
{
	అక్షరం		*tmp;
	గుర్తులేని దీర్ఘ	ret;

	ret = strtoul(str, &tmp, 0);
	ఒకవేళ (*tmp == 0) {
		ఒకవేళ (err)
			*err = 0;
		తిరిగి ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	ఒకవేళ (err)
		*err = 1;
	లేకపోతే
		నిష్క్రమించు(1);
	తిరిగి 0;
}


పూర్ణాంకం check_fs_open(స్థిరరాశి అక్షరం *name)
{
	ఒకవేళ (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		తిరిగి 1;
	}
	తిరిగి 0;
}

స్థిర శూన్య అమరిక_filesystem(స్థిరరాశి అక్షరం *name,
			     గుర్తులేని పూర్ణాంకం ఖండాలు, గుర్తులేని పూర్ణాంకం inodes,
			     గుర్తులేని పూర్ణాంకం type, పూర్ణాంకం flags)
{
	నిర్మాణం ext2_super_ఖండం param;
	errcode_t retval;

	memset(&param, 0, పరిమాణం(param));
	ext2fs_ఖండాలు_లెక్క_సమితి(&param, ఖండాలు);
	param.s_inodes_లెక్క = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	ఒకవేళ (retval) {
		com_err(name, retval, "while initializing filesystem");
		తిరిగి;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_ఖండం_bitmap(test_fs->ఖండం_map);
	test_fs->ఖండం_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_ఖండం_bitmap(test_fs, "block bitmap",
					      &test_fs->ఖండం_map);
	ఒకవేళ (retval) {
		com_err(name, retval, "while allocating block bitmap");
		వెళ్ళు errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	ఒకవేళ (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		వెళ్ళు errout;
	}
	తిరిగి;

errout:
	ext2fs_close_free(&test_fs);
}

శూన్య అమరిక_cmd(పూర్ణాంకం argc, ss_argv_t argv,
	       పూర్ణాంకం sci_idx EXT2FS_ATTR((వాడని)),
	       శూన్య *infop EXT2FS_ATTR((వాడని)))
{
	పూర్ణాంకం		c, err;
	గుర్తులేని పూర్ణాంకం	ఖండాలు = 128;
	గుర్తులేని పూర్ణాంకం	inodes = 0;
	గుర్తులేని పూర్ణాంకం	type = EXT2FS_BMAP64_BITARRAY;
	పూర్ణాంకం		flags = EXT2_FLAG_64_బిట్లు;

	ఒకవేళ (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	అయితే ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		ఎంపిక (c) {
		స్థితి 'b':
			ఖండాలు = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ఒకవేళ (err)
				తిరిగి;
			ఆపు;
		స్థితి 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ఒకవేళ (err)
				తిరిగి;
			ఆపు;
		స్థితి 'l':	/* Legacy bitmaps */
			flags = 0;
			ఆపు;
		స్థితి 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			ఒకవేళ (err)
				తిరిగి;
			ఆపు;
		అప్రమేయ:
			fprintf(stderr, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			తిరిగి;
		}
	}
	అమరిక_filesystem(argv[0], ఖండాలు, inodes, type, flags);
}

శూన్య close_cmd(పూర్ణాంకం argc, అక్షరం **argv, పూర్ణాంకం sci_idx EXT2FS_ATTR((వాడని)),
	       శూన్య *infop EXT2FS_ATTR((వాడని)))
{
	ఒకవేళ (check_fs_open(argv[0]))
		తిరిగి;

	ext2fs_close_free(&test_fs);
}


శూన్య dump_bitmap(ext2fs_generic_bitmap bmap, గుర్తులేని పూర్ణాంకం ప్రారంభం, గుర్తులేని అంకె)
{
	గుర్తులేని అక్షరం	*buf;
	errcode_t	retval;
	పూర్ణాంకం		i, len = (అంకె - ప్రారంభం + 7) / 8;

	buf = జ్ఞాపకం_ఇవ్వు(len);
	ఒకవేళ (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		తిరిగి;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) ప్రారంభం, అంకె, buf);
	ఒకవేళ (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		విడుదల(buf);
		తిరిగి;
	}
	కొరకు (i=0; i < len; i++)
		రాయి("%02x", buf[i]);
	రాయి("\n");
	రాయి("bits set: %u\n", ext2fs_bitcount(buf, len));
	విడుదల(buf);
}

శూన్య dump_inode_bitmap_cmd(పూర్ణాంకం argc, ss_argv_t argv,
			   పూర్ణాంకం sci_idx EXT2FS_ATTR((వాడని)),
			   శూన్య *infop EXT2FS_ATTR((వాడని)))
{
	ఒకవేళ (check_fs_open(argv[0]))
		తిరిగి;

	రాయి("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_లెక్క);
}
	
శూన్య dump_ఖండం_bitmap_cmd(పూర్ణాంకం argc, ss_argv_t argv,
			   పూర్ణాంకం sci_idx EXT2FS_ATTR((వాడని)),
			   శూన్య *infop EXT2FS_ATTR((వాడని)))
{
	ఒకవేళ (check_fs_open(argv[0]))
		తిరిగి;

	రాయి("block bitmap: ");
	dump_bitmap(test_fs->ఖండం_map, test_fs->super->s_మొదటి_దత్తాంశం_ఖండం,
		    test_fs->super->s_ఖండాలు_లెక్క);
}
	
శూన్య do_setb(పూర్ణాంకం argc, ss_argv_t argv,
	     పూర్ణాంకం sci_idx EXT2FS_ATTR((వాడని)),
	     శూన్య *infop EXT2FS_ATTR((వాడని)))
{
	గుర్తులేని పూర్ణాంకం ఖండం, అంకె;
	పూర్ణాంకం err;
	పూర్ణాంకం test_ఫలితం, op_ఫలితం;

	ఒకవేళ (check_fs_open(argv[0]))
		తిరిగి;

	ఒకవేళ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		తిరిగి;
	}

	ఖండం = parse_ulong(argv[1], argv[0], "block", &err);
	ఒకవేళ (err)
		తిరిగి;

	ఒకవేళ (argc == 3) {
		అంకె = parse_ulong(argv[2], argv[0], "num", &err);
		ఒకవేళ (err)
			తిరిగి;

		ext2fs_mark_ఖండం_bitmap_range2(test_fs->ఖండం_map,
						ఖండం, అంకె);
		రాయి("Marking blocks %u to %u\n", ఖండం, ఖండం + అంకె - 1);
		తిరిగి;
	}

	test_ఫలితం = ext2fs_test_ఖండం_bitmap2(test_fs->ఖండం_map, ఖండం);
	op_ఫలితం = ext2fs_mark_ఖండం_bitmap2(test_fs->ఖండం_map, ఖండం);
	రాయి("Setting block %u, was %s before\n", ఖండం, op_ఫలితం ?
	       "set" : "clear");
	ఒకవేళ (!test_ఫలితం != !op_ఫలితం)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_ఫలితం, op_ఫలితం);
}

శూన్య do_clearb(పూర్ణాంకం argc, ss_argv_t argv,
	       పూర్ణాంకం sci_idx EXT2FS_ATTR((వాడని)),
	       శూన్య *infop EXT2FS_ATTR((వాడని)))
{
	గుర్తులేని పూర్ణాంకం ఖండం, అంకె;
	పూర్ణాంకం err;
	పూర్ణాంకం test_ఫలితం, op_ఫలితం;

	ఒకవేళ (check_fs_open(argv[0]))
		తిరిగి;

	ఒకవేళ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		తిరిగి;
	}

	ఖండం = parse_ulong(argv[1], argv[0], "block", &err);
	ఒకవేళ (err)
		తిరిగి;

	ఒకవేళ (argc == 3) {
		అంకె = parse_ulong(argv[2], argv[0], "num", &err);
		ఒకవేళ (err)
			తిరిగి;

		ext2fs_unmark_ఖండం_bitmap_range2(test_fs->ఖండం_map,
						ఖండం, అంకె);
		రాయి("Clearing blocks %u to %u\n", ఖండం, ఖండం + అంకె - 1);
		తిరిగి;
	}

	test_ఫలితం = ext2fs_test_ఖండం_bitmap2(test_fs->ఖండం_map, ఖండం);
	op_ఫలితం = ext2fs_unmark_ఖండం_bitmap2(test_fs->ఖండం_map, ఖండం);
	రాయి("Clearing block %u, was %s before\n", ఖండం, op_ఫలితం ?
	       "set" : "clear");
	ఒకవేళ (!test_ఫలితం != !op_ఫలితం)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_ఫలితం, op_ఫలితం);
}

శూన్య do_testb(పూర్ణాంకం argc, ss_argv_t argv,
	      పూర్ణాంకం sci_idx EXT2FS_ATTR((వాడని)),
	      శూన్య *infop EXT2FS_ATTR((వాడని)))
{
	గుర్తులేని పూర్ణాంకం ఖండం, అంకె;
	పూర్ణాంకం err;
	పూర్ణాంకం test_ఫలితం;

	ఒకవేళ (check_fs_open(argv[0]))
		తిరిగి;

	ఒకవేళ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		తిరిగి;
	}

	ఖండం = parse_ulong(argv[1], argv[0], "block", &err);
	ఒకవేళ (err)
		తిరిగి;

	ఒకవేళ (argc == 3) {
		అంకె = parse_ulong(argv[2], argv[0], "num", &err);
		ఒకవేళ (err)
			తిరిగి;

		test_ఫలితం =
			ext2fs_test_ఖండం_bitmap_range2(test_fs->ఖండం_map,
							ఖండం, అంకె);
		రాయి("Blocks %u to %u are %sall clear.\n",
		       ఖండం, ఖండం + అంకె - 1, test_ఫలితం ? "" : "NOT ");
		తిరిగి;
	}

	test_ఫలితం = ext2fs_test_ఖండం_bitmap2(test_fs->ఖండం_map, ఖండం);
	రాయి("Block %u is %s\n", ఖండం, test_ఫలితం ? "set" : "clear");
}

శూన్య do_ffzb(పూర్ణాంకం argc, ss_argv_t argv,
	     పూర్ణాంకం sci_idx EXT2FS_ATTR((వాడని)),
	     శూన్య *infop EXT2FS_ATTR((వాడని)))
{
	గుర్తులేని పూర్ణాంకం ప్రారంభం, చివర;
	పూర్ణాంకం err;
	errcode_t retval;
	blk64_t బహిర్గతం;

	ఒకవేళ (check_fs_open(argv[0]))
		తిరిగి;

	ఒకవేళ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		తిరిగి;
	}

	ప్రారంభం = parse_ulong(argv[1], argv[0], "start", &err);
	ఒకవేళ (err)
		తిరిగి;

	చివర = parse_ulong(argv[2], argv[0], "end", &err);
	ఒకవేళ (err)
		తిరిగి;

	retval = ext2fs_కనుగొను_మొదటి_zero_ఖండం_bitmap2(test_fs->ఖండం_map,
						      ప్రారంభం, చివర, &బహిర్గతం);
	ఒకవేళ (retval) {
		రాయి("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_సందేశం(retval));
		తిరిగి;
	}
	రాయి("First unmarked block is %llu\n", (గుర్తులేని దీర్ఘ దీర్ఘ) బహిర్గతం);
}

శూన్య do_ffsb(పూర్ణాంకం argc, ss_argv_t argv,
	     పూర్ణాంకం sci_idx EXT2FS_ATTR((వాడని)),
	     శూన్య *infop EXT2FS_ATTR((వాడని)))
{
	గుర్తులేని పూర్ణాంకం ప్రారంభం, చివర;
	పూర్ణాంకం err;
	errcode_t retval;
	blk64_t బహిర్గతం;

	ఒకవేళ (check_fs_open(argv[0]))
		తిరిగి;

	ఒకవేళ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		తిరిగి;
	}

	ప్రారంభం = parse_ulong(argv[1], argv[0], "start", &err);
	ఒకవేళ (err)
		తిరిగి;

	చివర = parse_ulong(argv[2], argv[0], "end", &err);
	ఒకవేళ (err)
		తిరిగి;

	retval = ext2fs_కనుగొను_మొదటి_సమితి_ఖండం_bitmap2(test_fs->ఖండం_map,
						      ప్రారంభం, చివర, &బహిర్గతం);
	ఒకవేళ (retval) {
		రాయి("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_సందేశం(retval));
		తిరిగి;
	}
	రాయి("First marked block is %llu\n", (గుర్తులేని దీర్ఘ దీర్ఘ) బహిర్గతం);
}


శూన్య do_zerob(పూర్ణాంకం argc, ss_argv_t argv,
	      పూర్ణాంకం sci_idx EXT2FS_ATTR((వాడని)),
	      శూన్య *infop EXT2FS_ATTR((వాడని)))
{
	ఒకవేళ (check_fs_open(argv[0]))
		తిరిగి;

	రాయి("Clearing block bitmap.\n");
	ext2fs_clear_ఖండం_bitmap(test_fs->ఖండం_map);
}

శూన్య do_seti(పూర్ణాంకం argc, ss_argv_t argv,
	     పూర్ణాంకం sci_idx EXT2FS_ATTR((వాడని)),
	     శూన్య *infop EXT2FS_ATTR((వాడని)))
{
	గుర్తులేని పూర్ణాంకం inode;
	పూర్ణాంకం err;
	పూర్ణాంకం test_ఫలితం, op_ఫలితం;

	ఒకవేళ (check_fs_open(argv[0]))
		తిరిగి;

	ఒకవేళ (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		తిరిగి;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ఒకవేళ (err)
		తిరిగి;

	test_ఫలితం = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_ఫలితం = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	రాయి("Setting inode %u, was %s before\n", inode, op_ఫలితం ?
	       "set" : "clear");
	ఒకవేళ (!test_ఫలితం != !op_ఫలితం) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_ఫలితం, op_ఫలితం);
		exit_స్థితి++;
	}
}

శూన్య do_cleari(పూర్ణాంకం argc, ss_argv_t argv,
	       పూర్ణాంకం sci_idx EXT2FS_ATTR((వాడని)),
	       శూన్య *infop EXT2FS_ATTR((వాడని)))
{
	గుర్తులేని పూర్ణాంకం inode;
	పూర్ణాంకం err;
	పూర్ణాంకం test_ఫలితం, op_ఫలితం;

	ఒకవేళ (check_fs_open(argv[0]))
		తిరిగి;

	ఒకవేళ (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		తిరిగి;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ఒకవేళ (err)
		తిరిగి;

	test_ఫలితం = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_ఫలితం = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	రాయి("Clearing inode %u, was %s before\n", inode, op_ఫలితం ?
	       "set" : "clear");
	ఒకవేళ (!test_ఫలితం != !op_ఫలితం) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_ఫలితం, op_ఫలితం);
		exit_స్థితి++;
	}
}

శూన్య do_testi(పూర్ణాంకం argc, ss_argv_t argv,
	      పూర్ణాంకం sci_idx EXT2FS_ATTR((వాడని)),
	      శూన్య *infop EXT2FS_ATTR((వాడని)))
{
	గుర్తులేని పూర్ణాంకం inode;
	పూర్ణాంకం err;
	పూర్ణాంకం test_ఫలితం;

	ఒకవేళ (check_fs_open(argv[0]))
		తిరిగి;

	ఒకవేళ (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		తిరిగి;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ఒకవేళ (err)
		తిరిగి;

	test_ఫలితం = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	రాయి("Inode %u is %s\n", inode, test_ఫలితం ? "set" : "clear");
}

శూన్య do_ffzi(పూర్ణాంకం argc, ss_argv_t argv,
	     పూర్ణాంకం sci_idx EXT2FS_ATTR((వాడని)),
	     శూన్య *infop EXT2FS_ATTR((వాడని)))
{
	గుర్తులేని పూర్ణాంకం ప్రారంభం, చివర;
	పూర్ణాంకం err;
	errcode_t retval;
	ext2_ino_t బహిర్గతం;

	ఒకవేళ (check_fs_open(argv[0]))
		తిరిగి;

	ఒకవేళ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		తిరిగి;
	}

	ప్రారంభం = parse_ulong(argv[1], argv[0], "start", &err);
	ఒకవేళ (err)
		తిరిగి;

	చివర = parse_ulong(argv[2], argv[0], "end", &err);
	ఒకవేళ (err)
		తిరిగి;

	retval = ext2fs_కనుగొను_మొదటి_zero_inode_bitmap2(test_fs->inode_map,
						      ప్రారంభం, చివర, &బహిర్గతం);
	ఒకవేళ (retval) {
		రాయి("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_సందేశం(retval));
		తిరిగి;
	}
	రాయి("First unmarked inode is %u\n", బహిర్గతం);
}

శూన్య do_ffsi(పూర్ణాంకం argc, ss_argv_t argv,
	     పూర్ణాంకం sci_idx EXT2FS_ATTR((వాడని)),
	     శూన్య *infop EXT2FS_ATTR((వాడని)))
{
	గుర్తులేని పూర్ణాంకం ప్రారంభం, చివర;
	పూర్ణాంకం err;
	errcode_t retval;
	ext2_ino_t బహిర్గతం;

	ఒకవేళ (check_fs_open(argv[0]))
		తిరిగి;

	ఒకవేళ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		తిరిగి;
	}

	ప్రారంభం = parse_ulong(argv[1], argv[0], "start", &err);
	ఒకవేళ (err)
		తిరిగి;

	చివర = parse_ulong(argv[2], argv[0], "end", &err);
	ఒకవేళ (err)
		తిరిగి;

	retval = ext2fs_కనుగొను_మొదటి_సమితి_inode_bitmap2(test_fs->inode_map,
						     ప్రారంభం, చివర, &బహిర్గతం);
	ఒకవేళ (retval) {
		రాయి("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_సందేశం(retval));
		తిరిగి;
	}
	రాయి("First marked inode is %u\n", బహిర్గతం);
}

శూన్య do_zeroi(పూర్ణాంకం argc, ss_argv_t argv,
	      పూర్ణాంకం sci_idx EXT2FS_ATTR((వాడని)),
	      శూన్య *infop EXT2FS_ATTR((వాడని)))
{
	ఒకవేళ (check_fs_open(argv[0]))
		తిరిగి;

	రాయి("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

పూర్ణాంకం ప్రధాన(పూర్ణాంకం argc, అక్షరం **argv)
{
	గుర్తులేని పూర్ణాంకం	ఖండాలు = 128;
	గుర్తులేని పూర్ణాంకం	inodes = 0;
	గుర్తులేని పూర్ణాంకం	type = EXT2FS_BMAP64_BITARRAY;
	పూర్ణాంకం		c, err, సంకేతం_2;
	అక్షరం		*request = (అక్షరం *)ఖాళీ;
	అక్షరం		*cmd_file = 0;
	పూర్ణాంకం		sci_idx;
	పూర్ణాంకం		flags = EXT2_FLAG_64_బిట్లు;

	కూడు_error_పట్టిక(&et_ss_error_పట్టిక);
	కూడు_error_పట్టిక(&et_ext2_error_పట్టిక);
	అయితే ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		ఎంపిక (c) {
		స్థితి 'b':
			ఖండాలు = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ఒకవేళ (err)
				నిష్క్రమించు(1);
			ఆపు;
		స్థితి 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ఒకవేళ (err)
				నిష్క్రమించు(1);
			ఆపు;
		స్థితి 'l':	/* Legacy bitmaps */
			flags = 0;
			ఆపు;
		స్థితి 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			ఒకవేళ (err)
				నిష్క్రమించు(1);
			ఆపు;
		స్థితి 'R':
			request = optarg;
			ఆపు;
		స్థితి 'f':
			cmd_file = optarg;
			ఆపు;
		అప్రమేయ:
			com_err(argv[0], 0, "Usage: %s [-R request] "
				"[-f cmd_file]", subsystem_name);
			నిష్క్రమించు(1);
		}
	}

	sci_idx = ss_create_invocation(subsystem_name, సంస్కరణ,
				       (అక్షరం *)ఖాళీ, &tst_bitmaps_cmds, &సంకేతం_2);
	ఒకవేళ (సంకేతం_2) {
		ss_perror(sci_idx, సంకేతం_2, "creating invocation");
		నిష్క్రమించు(1);
	}

	(శూన్య) ss_కూడు_request_పట్టిక (sci_idx, &ss_std_అభ్యర్థనలు, 1, &సంకేతం_2);
	ఒకవేళ (సంకేతం_2) {
		ss_perror(sci_idx, సంకేతం_2, "adding standard requests");
		నిష్క్రమించు (1);
	}

	రాయి("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, సంస్కరణ);

	అమరిక_filesystem(argv[0], ఖండాలు, inodes, type, flags);

	ఒకవేళ (request) {
		సంకేతం_2 = ss_execute_పంక్తి(sci_idx, request);
		ఒకవేళ (సంకేతం_2) {
			ss_perror(sci_idx, సంకేతం_2, request);
			exit_స్థితి++;
		}
	} లేకపోతే ఒకవేళ (cmd_file) {
		exit_స్థితి = ఆధారం_file(cmd_file, sci_idx);
	} లేకపోతే {
		ss_listen(sci_idx);
	}

	నిష్క్రమించు(exit_స్థితి);
}

