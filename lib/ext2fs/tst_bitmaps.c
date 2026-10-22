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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <unistd.h>
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <stdio.h>
#ifdef HAVE_GETOPT_H
#অন্তর্ভাৱ <getopt.h>
#endif
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <time.h>
#অন্তর্ভাৱ <sys/stat.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ "ss/ss.h"

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"
#অন্তর্ভাৱ "ext2fsP.h"

বাহ্য ss_request_table tst_bitmaps_cmds;

স্থির ৱর্ণ subsystem_name[] = "tst_bitmaps";
স্থির ৱর্ণ version[] = "1.0";

ext2_filsys	test_fs;
পূর্ণাঙ্ক		exit_status = 0;

স্থির পূর্ণাঙ্ক source_file(ধ্রুৱ ৱর্ণ *cmd_file, পূর্ণাঙ্ক sci_idx)
{
	FILE		*f;
	ৱর্ণ		buf[256];
	ৱর্ণ		*cp;
	পূর্ণাঙ্ক		retval;
	পূর্ণাঙ্ক 		noecho;

	যদি (strcmp(cmd_file, "-") == 0)
		f = stdin;
	অন্যথা {
		f = fopen(cmd_file, "r");
		যদি (!f) {
			perror(cmd_file);
			exit(1);
		}
	}
	fflush(stdout);
	fflush(stderr);
	setbuf(stdout, রিক্ত);
	setbuf(stderr, রিক্ত);
	যেতিয়ালৈ (!feof(f)) {
		যদি (fgets(buf, পরিমাণ(buf), f) == রিক্ত)
			ভঙ্গ;
		যদি (buf[0] == '#')
			অনুৱর্ত;
		noecho = 0;
		যদি (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = strchr(buf, '\n');
		যদি (cp)
			*cp = 0;
		cp = strchr(buf, '\r');
		যদি (cp)
			*cp = 0;
		যদি (!noecho)
			printf("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_সাৰি(sci_idx, buf);
		যদি (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_status++;
		}
	}
	ঘূৰাই exit_status;
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
শূন্য reset_getopt(শূন্য)
{
#যদি defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#অন্যথা
	optind = 1;
#endif
#ifdef HAVE_OPTRESET
	optreset = 1;		/* Makes BSD getopt happy */
#endif
}

/*
 * This function will convert a string to an unsigned long, printing
 * an error message if it fails, and returning success or failure in err.
 */
unsigned long parse_ulong(ধ্রুৱ ৱর্ণ *str, ধ্রুৱ ৱর্ণ *cmd,
			  ধ্রুৱ ৱর্ণ *descr, পূর্ণাঙ্ক *err)
{
	ৱর্ণ		*tmp;
	unsigned long	ret;

	ret = strtoul(str, &tmp, 0);
	যদি (*tmp == 0) {
		যদি (err)
			*err = 0;
		ঘূৰাই ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	যদি (err)
		*err = 1;
	অন্যথা
		exit(1);
	ঘূৰাই 0;
}


পূর্ণাঙ্ক check_fs_open(ধ্রুৱ ৱর্ণ *name)
{
	যদি (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		ঘূৰাই 1;
	}
	ঘূৰাই 0;
}

স্থির শূন্য setup_filesystem(ধ্রুৱ ৱর্ণ *name,
			     unsigned পূর্ণাঙ্ক blocks, unsigned পূর্ণাঙ্ক inodes,
			     unsigned পূর্ণাঙ্ক type, পূর্ণাঙ্ক flags)
{
	সংরচনা ext2_super_block param;
	errcode_t retval;

	memset(&param, 0, পরিমাণ(param));
	ext2fs_blocks_গুনতি_set(&param, blocks);
	param.s_inodes_গুনতি = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	যদি (retval) {
		com_err(name, retval, "while initializing filesystem");
		ঘূৰাই;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_block_bitmap(test_fs->block_map);
	test_fs->block_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_block_bitmap(test_fs, "block bitmap",
					      &test_fs->block_map);
	যদি (retval) {
		com_err(name, retval, "while allocating block bitmap");
		গচ্ছ errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	যদি (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		গচ্ছ errout;
	}
	ঘূৰাই;

errout:
	ext2fs_close_free(&test_fs);
}

শূন্য setup_cmd(পূর্ণাঙ্ক argc, ss_argv_t argv,
	       পূর্ণাঙ্ক sci_idx EXT2FS_ATTR((unused)),
	       শূন্য *infop EXT2FS_ATTR((unused)))
{
	পূর্ণাঙ্ক		c, err;
	unsigned পূর্ণাঙ্ক	blocks = 128;
	unsigned পূর্ণাঙ্ক	inodes = 0;
	unsigned পূর্ণাঙ্ক	type = EXT2FS_BMAP64_BITARRAY;
	পূর্ণাঙ্ক		flags = EXT2_FLAG_64BITS;

	যদি (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	যেতিয়ালৈ ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		ৱিকল্প (c) {
		পক্ষ 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			যদি (err)
				ঘূৰাই;
			ভঙ্গ;
		পক্ষ 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			যদি (err)
				ঘূৰাই;
			ভঙ্গ;
		পক্ষ 'l':	/* Legacy bitmaps */
			flags = 0;
			ভঙ্গ;
		পক্ষ 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			যদি (err)
				ঘূৰাই;
			ভঙ্গ;
		স্ৱভাৱ:
			fprintf(stderr, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			ঘূৰাই;
		}
	}
	setup_filesystem(argv[0], blocks, inodes, type, flags);
}

শূন্য close_cmd(পূর্ণাঙ্ক argc, ৱর্ণ **argv, পূর্ণাঙ্ক sci_idx EXT2FS_ATTR((unused)),
	       শূন্য *infop EXT2FS_ATTR((unused)))
{
	যদি (check_fs_open(argv[0]))
		ঘূৰাই;

	ext2fs_close_free(&test_fs);
}


শূন্য dump_bitmap(ext2fs_generic_bitmap bmap, unsigned পূর্ণাঙ্ক start, unsigned অঙ্ক)
{
	unsigned ৱর্ণ	*buf;
	errcode_t	retval;
	পূর্ণাঙ্ক		i, len = (অঙ্ক - start + 7) / 8;

	buf = malloc(len);
	যদি (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		ঘূৰাই;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) start, অঙ্ক, buf);
	যদি (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		মুক্ত(buf);
		ঘূৰাই;
	}
	বাবে (i=0; i < len; i++)
		printf("%02x", buf[i]);
	printf("\n");
	printf("bits set: %u\n", ext2fs_bitcount(buf, len));
	মুক্ত(buf);
}

শূন্য dump_inode_bitmap_cmd(পূর্ণাঙ্ক argc, ss_argv_t argv,
			   পূর্ণাঙ্ক sci_idx EXT2FS_ATTR((unused)),
			   শূন্য *infop EXT2FS_ATTR((unused)))
{
	যদি (check_fs_open(argv[0]))
		ঘূৰাই;

	printf("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_গুনতি);
}
	
শূন্য dump_block_bitmap_cmd(পূর্ণাঙ্ক argc, ss_argv_t argv,
			   পূর্ণাঙ্ক sci_idx EXT2FS_ATTR((unused)),
			   শূন্য *infop EXT2FS_ATTR((unused)))
{
	যদি (check_fs_open(argv[0]))
		ঘূৰাই;

	printf("block bitmap: ");
	dump_bitmap(test_fs->block_map, test_fs->super->s_first_data_block,
		    test_fs->super->s_blocks_গুনতি);
}
	
শূন্য do_setb(পূর্ণাঙ্ক argc, ss_argv_t argv,
	     পূর্ণাঙ্ক sci_idx EXT2FS_ATTR((unused)),
	     শূন্য *infop EXT2FS_ATTR((unused)))
{
	unsigned পূর্ণাঙ্ক block, অঙ্ক;
	পূর্ণাঙ্ক err;
	পূর্ণাঙ্ক test_result, op_result;

	যদি (check_fs_open(argv[0]))
		ঘূৰাই;

	যদি (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		ঘূৰাই;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	যদি (err)
		ঘূৰাই;

	যদি (argc == 3) {
		অঙ্ক = parse_ulong(argv[2], argv[0], "num", &err);
		যদি (err)
			ঘূৰাই;

		ext2fs_mark_block_bitmap_range2(test_fs->block_map,
						block, অঙ্ক);
		printf("Marking blocks %u to %u\n", block, block + অঙ্ক - 1);
		ঘূৰাই;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_mark_block_bitmap2(test_fs->block_map, block);
	printf("Setting block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	যদি (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

শূন্য do_clearb(পূর্ণাঙ্ক argc, ss_argv_t argv,
	       পূর্ণাঙ্ক sci_idx EXT2FS_ATTR((unused)),
	       শূন্য *infop EXT2FS_ATTR((unused)))
{
	unsigned পূর্ণাঙ্ক block, অঙ্ক;
	পূর্ণাঙ্ক err;
	পূর্ণাঙ্ক test_result, op_result;

	যদি (check_fs_open(argv[0]))
		ঘূৰাই;

	যদি (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		ঘূৰাই;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	যদি (err)
		ঘূৰাই;

	যদি (argc == 3) {
		অঙ্ক = parse_ulong(argv[2], argv[0], "num", &err);
		যদি (err)
			ঘূৰাই;

		ext2fs_unmark_block_bitmap_range2(test_fs->block_map,
						block, অঙ্ক);
		printf("Clearing blocks %u to %u\n", block, block + অঙ্ক - 1);
		ঘূৰাই;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_unmark_block_bitmap2(test_fs->block_map, block);
	printf("Clearing block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	যদি (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

শূন্য do_testb(পূর্ণাঙ্ক argc, ss_argv_t argv,
	      পূর্ণাঙ্ক sci_idx EXT2FS_ATTR((unused)),
	      শূন্য *infop EXT2FS_ATTR((unused)))
{
	unsigned পূর্ণাঙ্ক block, অঙ্ক;
	পূর্ণাঙ্ক err;
	পূর্ণাঙ্ক test_result;

	যদি (check_fs_open(argv[0]))
		ঘূৰাই;

	যদি (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		ঘূৰাই;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	যদি (err)
		ঘূৰাই;

	যদি (argc == 3) {
		অঙ্ক = parse_ulong(argv[2], argv[0], "num", &err);
		যদি (err)
			ঘূৰাই;

		test_result =
			ext2fs_test_block_bitmap_range2(test_fs->block_map,
							block, অঙ্ক);
		printf("Blocks %u to %u are %sall clear.\n",
		       block, block + অঙ্ক - 1, test_result ? "" : "NOT ");
		ঘূৰাই;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	printf("Block %u is %s\n", block, test_result ? "set" : "clear");
}

শূন্য do_ffzb(পূর্ণাঙ্ক argc, ss_argv_t argv,
	     পূর্ণাঙ্ক sci_idx EXT2FS_ATTR((unused)),
	     শূন্য *infop EXT2FS_ATTR((unused)))
{
	unsigned পূর্ণাঙ্ক start, end;
	পূর্ণাঙ্ক err;
	errcode_t retval;
	blk64_t out;

	যদি (check_fs_open(argv[0]))
		ঘূৰাই;

	যদি (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		ঘূৰাই;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	যদি (err)
		ঘূৰাই;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	যদি (err)
		ঘূৰাই;

	retval = ext2fs_find_first_zero_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	যদি (retval) {
		printf("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_message(retval));
		ঘূৰাই;
	}
	printf("First unmarked block is %llu\n", (unsigned long long) out);
}

শূন্য do_ffsb(পূর্ণাঙ্ক argc, ss_argv_t argv,
	     পূর্ণাঙ্ক sci_idx EXT2FS_ATTR((unused)),
	     শূন্য *infop EXT2FS_ATTR((unused)))
{
	unsigned পূর্ণাঙ্ক start, end;
	পূর্ণাঙ্ক err;
	errcode_t retval;
	blk64_t out;

	যদি (check_fs_open(argv[0]))
		ঘূৰাই;

	যদি (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		ঘূৰাই;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	যদি (err)
		ঘূৰাই;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	যদি (err)
		ঘূৰাই;

	retval = ext2fs_find_first_set_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	যদি (retval) {
		printf("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_message(retval));
		ঘূৰাই;
	}
	printf("First marked block is %llu\n", (unsigned long long) out);
}


শূন্য do_zerob(পূর্ণাঙ্ক argc, ss_argv_t argv,
	      পূর্ণাঙ্ক sci_idx EXT2FS_ATTR((unused)),
	      শূন্য *infop EXT2FS_ATTR((unused)))
{
	যদি (check_fs_open(argv[0]))
		ঘূৰাই;

	printf("Clearing block bitmap.\n");
	ext2fs_clear_block_bitmap(test_fs->block_map);
}

শূন্য do_seti(পূর্ণাঙ্ক argc, ss_argv_t argv,
	     পূর্ণাঙ্ক sci_idx EXT2FS_ATTR((unused)),
	     শূন্য *infop EXT2FS_ATTR((unused)))
{
	unsigned পূর্ণাঙ্ক inode;
	পূর্ণাঙ্ক err;
	পূর্ণাঙ্ক test_result, op_result;

	যদি (check_fs_open(argv[0]))
		ঘূৰাই;

	যদি (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		ঘূৰাই;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	যদি (err)
		ঘূৰাই;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Setting inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	যদি (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

শূন্য do_cleari(পূর্ণাঙ্ক argc, ss_argv_t argv,
	       পূর্ণাঙ্ক sci_idx EXT2FS_ATTR((unused)),
	       শূন্য *infop EXT2FS_ATTR((unused)))
{
	unsigned পূর্ণাঙ্ক inode;
	পূর্ণাঙ্ক err;
	পূর্ণাঙ্ক test_result, op_result;

	যদি (check_fs_open(argv[0]))
		ঘূৰাই;

	যদি (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		ঘূৰাই;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	যদি (err)
		ঘূৰাই;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Clearing inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	যদি (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

শূন্য do_testi(পূর্ণাঙ্ক argc, ss_argv_t argv,
	      পূর্ণাঙ্ক sci_idx EXT2FS_ATTR((unused)),
	      শূন্য *infop EXT2FS_ATTR((unused)))
{
	unsigned পূর্ণাঙ্ক inode;
	পূর্ণাঙ্ক err;
	পূর্ণাঙ্ক test_result;

	যদি (check_fs_open(argv[0]))
		ঘূৰাই;

	যদি (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		ঘূৰাই;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	যদি (err)
		ঘূৰাই;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	printf("Inode %u is %s\n", inode, test_result ? "set" : "clear");
}

শূন্য do_ffzi(পূর্ণাঙ্ক argc, ss_argv_t argv,
	     পূর্ণাঙ্ক sci_idx EXT2FS_ATTR((unused)),
	     শূন্য *infop EXT2FS_ATTR((unused)))
{
	unsigned পূর্ণাঙ্ক start, end;
	পূর্ণাঙ্ক err;
	errcode_t retval;
	ext2_ino_t out;

	যদি (check_fs_open(argv[0]))
		ঘূৰাই;

	যদি (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		ঘূৰাই;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	যদি (err)
		ঘূৰাই;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	যদি (err)
		ঘূৰাই;

	retval = ext2fs_find_first_zero_inode_bitmap2(test_fs->inode_map,
						      start, end, &out);
	যদি (retval) {
		printf("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_message(retval));
		ঘূৰাই;
	}
	printf("First unmarked inode is %u\n", out);
}

শূন্য do_ffsi(পূর্ণাঙ্ক argc, ss_argv_t argv,
	     পূর্ণাঙ্ক sci_idx EXT2FS_ATTR((unused)),
	     শূন্য *infop EXT2FS_ATTR((unused)))
{
	unsigned পূর্ণাঙ্ক start, end;
	পূর্ণাঙ্ক err;
	errcode_t retval;
	ext2_ino_t out;

	যদি (check_fs_open(argv[0]))
		ঘূৰাই;

	যদি (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		ঘূৰাই;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	যদি (err)
		ঘূৰাই;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	যদি (err)
		ঘূৰাই;

	retval = ext2fs_find_first_set_inode_bitmap2(test_fs->inode_map,
						     start, end, &out);
	যদি (retval) {
		printf("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_message(retval));
		ঘূৰাই;
	}
	printf("First marked inode is %u\n", out);
}

শূন্য do_zeroi(পূর্ণাঙ্ক argc, ss_argv_t argv,
	      পূর্ণাঙ্ক sci_idx EXT2FS_ATTR((unused)),
	      শূন্য *infop EXT2FS_ATTR((unused)))
{
	যদি (check_fs_open(argv[0]))
		ঘূৰাই;

	printf("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

পূর্ণাঙ্ক main(পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
	unsigned পূর্ণাঙ্ক	blocks = 128;
	unsigned পূর্ণাঙ্ক	inodes = 0;
	unsigned পূর্ণাঙ্ক	type = EXT2FS_BMAP64_BITARRAY;
	পূর্ণাঙ্ক		c, err, code;
	ৱর্ণ		*request = (ৱর্ণ *)রিক্ত;
	ৱর্ণ		*cmd_file = 0;
	পূর্ণাঙ্ক		sci_idx;
	পূর্ণাঙ্ক		flags = EXT2_FLAG_64BITS;

	add_error_table(&et_ss_error_table);
	add_error_table(&et_ext2_error_table);
	যেতিয়ালৈ ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		ৱিকল্প (c) {
		পক্ষ 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			যদি (err)
				exit(1);
			ভঙ্গ;
		পক্ষ 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			যদি (err)
				exit(1);
			ভঙ্গ;
		পক্ষ 'l':	/* Legacy bitmaps */
			flags = 0;
			ভঙ্গ;
		পক্ষ 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			যদি (err)
				exit(1);
			ভঙ্গ;
		পক্ষ 'R':
			request = optarg;
			ভঙ্গ;
		পক্ষ 'f':
			cmd_file = optarg;
			ভঙ্গ;
		স্ৱভাৱ:
			com_err(argv[0], 0, "Usage: %s [-R request] "
				"[-f cmd_file]", subsystem_name);
			exit(1);
		}
	}

	sci_idx = ss_create_invocation(subsystem_name, version,
				       (ৱর্ণ *)রিক্ত, &tst_bitmaps_cmds, &code);
	যদি (code) {
		ss_perror(sci_idx, code, "creating invocation");
		exit(1);
	}

	(শূন্য) ss_add_request_table (sci_idx, &ss_std_requests, 1, &code);
	যদি (code) {
		ss_perror(sci_idx, code, "adding standard requests");
		exit (1);
	}

	printf("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, version);

	setup_filesystem(argv[0], blocks, inodes, type, flags);

	যদি (request) {
		code = ss_execute_সাৰি(sci_idx, request);
		যদি (code) {
			ss_perror(sci_idx, code, request);
			exit_status++;
		}
	} অন্যথা যদি (cmd_file) {
		exit_status = source_file(cmd_file, sci_idx);
	} অন্যথা {
		ss_listen(sci_idx);
	}

	exit(exit_status);
}

