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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <stdio.h>
#ifdef HAVE_GETOPT_H
#অন্তর্ভাব <getopt.h>
#endif
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <time.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব "ss/ss.h"

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"
#অন্তর্ভাব "ext2fsP.h"

বাহ্যিক ss_request_table tst_bitmaps_cmds;

স্থির অক্ষর subsystem_name[] = "tst_bitmaps";
স্থির অক্ষর version[] = "1.0";

ext2_filsys	test_fs;
পূর্ণসংখ্যা		exit_status = 0;

স্থির পূর্ণসংখ্যা source_file(ধ্রুবক অক্ষর *cmd_file, পূর্ণসংখ্যা sci_idx)
{
	FILE		*f;
	অক্ষর		buf[256];
	অক্ষর		*cp;
	পূর্ণসংখ্যা		retval;
	পূর্ণসংখ্যা 		noecho;

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
	setbuf(stdout, ফাঁকা);
	setbuf(stderr, ফাঁকা);
	যখন (!feof(f)) {
		যদি (fgets(buf, আকার(buf), f) == ফাঁকা)
			বিরতি;
		যদি (buf[0] == '#')
			চালু;
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
		retval = ss_execute_সারি(sci_idx, buf);
		যদি (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_status++;
		}
	}
	ফেরত exit_status;
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
unsigned long parse_ulong(ধ্রুবক অক্ষর *str, ধ্রুবক অক্ষর *cmd,
			  ধ্রুবক অক্ষর *descr, পূর্ণসংখ্যা *err)
{
	অক্ষর		*tmp;
	unsigned long	ret;

	ret = strtoul(str, &tmp, 0);
	যদি (*tmp == 0) {
		যদি (err)
			*err = 0;
		ফেরত ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	যদি (err)
		*err = 1;
	অন্যথা
		exit(1);
	ফেরত 0;
}


পূর্ণসংখ্যা check_fs_open(ধ্রুবক অক্ষর *name)
{
	যদি (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		ফেরত 1;
	}
	ফেরত 0;
}

স্থির শূন্য setup_filesystem(ধ্রুবক অক্ষর *name,
			     unsigned পূর্ণসংখ্যা blocks, unsigned পূর্ণসংখ্যা inodes,
			     unsigned পূর্ণসংখ্যা type, পূর্ণসংখ্যা flags)
{
	কাঠামো ext2_super_block param;
	errcode_t retval;

	memset(&param, 0, আকার(param));
	ext2fs_blocks_গুনতি_set(&param, blocks);
	param.s_inodes_গুনতি = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	যদি (retval) {
		com_err(name, retval, "while initializing filesystem");
		ফেরত;
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
		যাও errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	যদি (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		যাও errout;
	}
	ফেরত;

errout:
	ext2fs_close_free(&test_fs);
}

শূন্য setup_cmd(পূর্ণসংখ্যা argc, ss_argv_t argv,
	       পূর্ণসংখ্যা sci_idx EXT2FS_ATTR((unused)),
	       শূন্য *infop EXT2FS_ATTR((unused)))
{
	পূর্ণসংখ্যা		c, err;
	unsigned পূর্ণসংখ্যা	blocks = 128;
	unsigned পূর্ণসংখ্যা	inodes = 0;
	unsigned পূর্ণসংখ্যা	type = EXT2FS_BMAP64_BITARRAY;
	পূর্ণসংখ্যা		flags = EXT2_FLAG_64BITS;

	যদি (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	যখন ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		নির্বাচন (c) {
		ক্ষেত্র 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			যদি (err)
				ফেরত;
			বিরতি;
		ক্ষেত্র 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			যদি (err)
				ফেরত;
			বিরতি;
		ক্ষেত্র 'l':	/* Legacy bitmaps */
			flags = 0;
			বিরতি;
		ক্ষেত্র 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			যদি (err)
				ফেরত;
			বিরতি;
		পূর্বনির্ধারিত:
			fprintf(stderr, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			ফেরত;
		}
	}
	setup_filesystem(argv[0], blocks, inodes, type, flags);
}

শূন্য close_cmd(পূর্ণসংখ্যা argc, অক্ষর **argv, পূর্ণসংখ্যা sci_idx EXT2FS_ATTR((unused)),
	       শূন্য *infop EXT2FS_ATTR((unused)))
{
	যদি (check_fs_open(argv[0]))
		ফেরত;

	ext2fs_close_free(&test_fs);
}


শূন্য dump_bitmap(ext2fs_generic_bitmap bmap, unsigned পূর্ণসংখ্যা start, unsigned অঙ্ক)
{
	unsigned অক্ষর	*buf;
	errcode_t	retval;
	পূর্ণসংখ্যা		i, len = (অঙ্ক - start + 7) / 8;

	buf = malloc(len);
	যদি (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		ফেরত;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) start, অঙ্ক, buf);
	যদি (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		মুক্ত(buf);
		ফেরত;
	}
	জন্য (i=0; i < len; i++)
		printf("%02x", buf[i]);
	printf("\n");
	printf("bits set: %u\n", ext2fs_bitcount(buf, len));
	মুক্ত(buf);
}

শূন্য dump_inode_bitmap_cmd(পূর্ণসংখ্যা argc, ss_argv_t argv,
			   পূর্ণসংখ্যা sci_idx EXT2FS_ATTR((unused)),
			   শূন্য *infop EXT2FS_ATTR((unused)))
{
	যদি (check_fs_open(argv[0]))
		ফেরত;

	printf("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_গুনতি);
}
	
শূন্য dump_block_bitmap_cmd(পূর্ণসংখ্যা argc, ss_argv_t argv,
			   পূর্ণসংখ্যা sci_idx EXT2FS_ATTR((unused)),
			   শূন্য *infop EXT2FS_ATTR((unused)))
{
	যদি (check_fs_open(argv[0]))
		ফেরত;

	printf("block bitmap: ");
	dump_bitmap(test_fs->block_map, test_fs->super->s_first_তথ্য_block,
		    test_fs->super->s_blocks_গুনতি);
}
	
শূন্য do_setb(পূর্ণসংখ্যা argc, ss_argv_t argv,
	     পূর্ণসংখ্যা sci_idx EXT2FS_ATTR((unused)),
	     শূন্য *infop EXT2FS_ATTR((unused)))
{
	unsigned পূর্ণসংখ্যা block, অঙ্ক;
	পূর্ণসংখ্যা err;
	পূর্ণসংখ্যা test_ফলাফল, op_ফলাফল;

	যদি (check_fs_open(argv[0]))
		ফেরত;

	যদি (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		ফেরত;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	যদি (err)
		ফেরত;

	যদি (argc == 3) {
		অঙ্ক = parse_ulong(argv[2], argv[0], "num", &err);
		যদি (err)
			ফেরত;

		ext2fs_mark_block_bitmap_range2(test_fs->block_map,
						block, অঙ্ক);
		printf("Marking blocks %u to %u\n", block, block + অঙ্ক - 1);
		ফেরত;
	}

	test_ফলাফল = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_ফলাফল = ext2fs_mark_block_bitmap2(test_fs->block_map, block);
	printf("Setting block %u, was %s before\n", block, op_ফলাফল ?
	       "set" : "clear");
	যদি (!test_ফলাফল != !op_ফলাফল)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_ফলাফল, op_ফলাফল);
}

শূন্য do_clearb(পূর্ণসংখ্যা argc, ss_argv_t argv,
	       পূর্ণসংখ্যা sci_idx EXT2FS_ATTR((unused)),
	       শূন্য *infop EXT2FS_ATTR((unused)))
{
	unsigned পূর্ণসংখ্যা block, অঙ্ক;
	পূর্ণসংখ্যা err;
	পূর্ণসংখ্যা test_ফলাফল, op_ফলাফল;

	যদি (check_fs_open(argv[0]))
		ফেরত;

	যদি (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		ফেরত;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	যদি (err)
		ফেরত;

	যদি (argc == 3) {
		অঙ্ক = parse_ulong(argv[2], argv[0], "num", &err);
		যদি (err)
			ফেরত;

		ext2fs_unmark_block_bitmap_range2(test_fs->block_map,
						block, অঙ্ক);
		printf("Clearing blocks %u to %u\n", block, block + অঙ্ক - 1);
		ফেরত;
	}

	test_ফলাফল = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_ফলাফল = ext2fs_unmark_block_bitmap2(test_fs->block_map, block);
	printf("Clearing block %u, was %s before\n", block, op_ফলাফল ?
	       "set" : "clear");
	যদি (!test_ফলাফল != !op_ফলাফল)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_ফলাফল, op_ফলাফল);
}

শূন্য do_testb(পূর্ণসংখ্যা argc, ss_argv_t argv,
	      পূর্ণসংখ্যা sci_idx EXT2FS_ATTR((unused)),
	      শূন্য *infop EXT2FS_ATTR((unused)))
{
	unsigned পূর্ণসংখ্যা block, অঙ্ক;
	পূর্ণসংখ্যা err;
	পূর্ণসংখ্যা test_ফলাফল;

	যদি (check_fs_open(argv[0]))
		ফেরত;

	যদি (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		ফেরত;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	যদি (err)
		ফেরত;

	যদি (argc == 3) {
		অঙ্ক = parse_ulong(argv[2], argv[0], "num", &err);
		যদি (err)
			ফেরত;

		test_ফলাফল =
			ext2fs_test_block_bitmap_range2(test_fs->block_map,
							block, অঙ্ক);
		printf("Blocks %u to %u are %sall clear.\n",
		       block, block + অঙ্ক - 1, test_ফলাফল ? "" : "NOT ");
		ফেরত;
	}

	test_ফলাফল = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	printf("Block %u is %s\n", block, test_ফলাফল ? "set" : "clear");
}

শূন্য do_ffzb(পূর্ণসংখ্যা argc, ss_argv_t argv,
	     পূর্ণসংখ্যা sci_idx EXT2FS_ATTR((unused)),
	     শূন্য *infop EXT2FS_ATTR((unused)))
{
	unsigned পূর্ণসংখ্যা start, end;
	পূর্ণসংখ্যা err;
	errcode_t retval;
	blk64_t out;

	যদি (check_fs_open(argv[0]))
		ফেরত;

	যদি (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		ফেরত;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	যদি (err)
		ফেরত;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	যদি (err)
		ফেরত;

	retval = ext2fs_find_first_zero_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	যদি (retval) {
		printf("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_message(retval));
		ফেরত;
	}
	printf("First unmarked block is %llu\n", (unsigned long long) out);
}

শূন্য do_ffsb(পূর্ণসংখ্যা argc, ss_argv_t argv,
	     পূর্ণসংখ্যা sci_idx EXT2FS_ATTR((unused)),
	     শূন্য *infop EXT2FS_ATTR((unused)))
{
	unsigned পূর্ণসংখ্যা start, end;
	পূর্ণসংখ্যা err;
	errcode_t retval;
	blk64_t out;

	যদি (check_fs_open(argv[0]))
		ফেরত;

	যদি (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		ফেরত;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	যদি (err)
		ফেরত;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	যদি (err)
		ফেরত;

	retval = ext2fs_find_first_set_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	যদি (retval) {
		printf("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_message(retval));
		ফেরত;
	}
	printf("First marked block is %llu\n", (unsigned long long) out);
}


শূন্য do_zerob(পূর্ণসংখ্যা argc, ss_argv_t argv,
	      পূর্ণসংখ্যা sci_idx EXT2FS_ATTR((unused)),
	      শূন্য *infop EXT2FS_ATTR((unused)))
{
	যদি (check_fs_open(argv[0]))
		ফেরত;

	printf("Clearing block bitmap.\n");
	ext2fs_clear_block_bitmap(test_fs->block_map);
}

শূন্য do_seti(পূর্ণসংখ্যা argc, ss_argv_t argv,
	     পূর্ণসংখ্যা sci_idx EXT2FS_ATTR((unused)),
	     শূন্য *infop EXT2FS_ATTR((unused)))
{
	unsigned পূর্ণসংখ্যা inode;
	পূর্ণসংখ্যা err;
	পূর্ণসংখ্যা test_ফলাফল, op_ফলাফল;

	যদি (check_fs_open(argv[0]))
		ফেরত;

	যদি (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		ফেরত;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	যদি (err)
		ফেরত;

	test_ফলাফল = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_ফলাফল = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Setting inode %u, was %s before\n", inode, op_ফলাফল ?
	       "set" : "clear");
	যদি (!test_ফলাফল != !op_ফলাফল) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_ফলাফল, op_ফলাফল);
		exit_status++;
	}
}

শূন্য do_cleari(পূর্ণসংখ্যা argc, ss_argv_t argv,
	       পূর্ণসংখ্যা sci_idx EXT2FS_ATTR((unused)),
	       শূন্য *infop EXT2FS_ATTR((unused)))
{
	unsigned পূর্ণসংখ্যা inode;
	পূর্ণসংখ্যা err;
	পূর্ণসংখ্যা test_ফলাফল, op_ফলাফল;

	যদি (check_fs_open(argv[0]))
		ফেরত;

	যদি (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		ফেরত;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	যদি (err)
		ফেরত;

	test_ফলাফল = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_ফলাফল = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Clearing inode %u, was %s before\n", inode, op_ফলাফল ?
	       "set" : "clear");
	যদি (!test_ফলাফল != !op_ফলাফল) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_ফলাফল, op_ফলাফল);
		exit_status++;
	}
}

শূন্য do_testi(পূর্ণসংখ্যা argc, ss_argv_t argv,
	      পূর্ণসংখ্যা sci_idx EXT2FS_ATTR((unused)),
	      শূন্য *infop EXT2FS_ATTR((unused)))
{
	unsigned পূর্ণসংখ্যা inode;
	পূর্ণসংখ্যা err;
	পূর্ণসংখ্যা test_ফলাফল;

	যদি (check_fs_open(argv[0]))
		ফেরত;

	যদি (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		ফেরত;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	যদি (err)
		ফেরত;

	test_ফলাফল = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	printf("Inode %u is %s\n", inode, test_ফলাফল ? "set" : "clear");
}

শূন্য do_ffzi(পূর্ণসংখ্যা argc, ss_argv_t argv,
	     পূর্ণসংখ্যা sci_idx EXT2FS_ATTR((unused)),
	     শূন্য *infop EXT2FS_ATTR((unused)))
{
	unsigned পূর্ণসংখ্যা start, end;
	পূর্ণসংখ্যা err;
	errcode_t retval;
	ext2_ino_t out;

	যদি (check_fs_open(argv[0]))
		ফেরত;

	যদি (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		ফেরত;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	যদি (err)
		ফেরত;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	যদি (err)
		ফেরত;

	retval = ext2fs_find_first_zero_inode_bitmap2(test_fs->inode_map,
						      start, end, &out);
	যদি (retval) {
		printf("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_message(retval));
		ফেরত;
	}
	printf("First unmarked inode is %u\n", out);
}

শূন্য do_ffsi(পূর্ণসংখ্যা argc, ss_argv_t argv,
	     পূর্ণসংখ্যা sci_idx EXT2FS_ATTR((unused)),
	     শূন্য *infop EXT2FS_ATTR((unused)))
{
	unsigned পূর্ণসংখ্যা start, end;
	পূর্ণসংখ্যা err;
	errcode_t retval;
	ext2_ino_t out;

	যদি (check_fs_open(argv[0]))
		ফেরত;

	যদি (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		ফেরত;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	যদি (err)
		ফেরত;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	যদি (err)
		ফেরত;

	retval = ext2fs_find_first_set_inode_bitmap2(test_fs->inode_map,
						     start, end, &out);
	যদি (retval) {
		printf("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_message(retval));
		ফেরত;
	}
	printf("First marked inode is %u\n", out);
}

শূন্য do_zeroi(পূর্ণসংখ্যা argc, ss_argv_t argv,
	      পূর্ণসংখ্যা sci_idx EXT2FS_ATTR((unused)),
	      শূন্য *infop EXT2FS_ATTR((unused)))
{
	যদি (check_fs_open(argv[0]))
		ফেরত;

	printf("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

পূর্ণসংখ্যা প্রধান(পূর্ণসংখ্যা argc, অক্ষর **argv)
{
	unsigned পূর্ণসংখ্যা	blocks = 128;
	unsigned পূর্ণসংখ্যা	inodes = 0;
	unsigned পূর্ণসংখ্যা	type = EXT2FS_BMAP64_BITARRAY;
	পূর্ণসংখ্যা		c, err, code;
	অক্ষর		*request = (অক্ষর *)ফাঁকা;
	অক্ষর		*cmd_file = 0;
	পূর্ণসংখ্যা		sci_idx;
	পূর্ণসংখ্যা		flags = EXT2_FLAG_64BITS;

	add_error_table(&et_ss_error_table);
	add_error_table(&et_ext2_error_table);
	যখন ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		নির্বাচন (c) {
		ক্ষেত্র 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			যদি (err)
				exit(1);
			বিরতি;
		ক্ষেত্র 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			যদি (err)
				exit(1);
			বিরতি;
		ক্ষেত্র 'l':	/* Legacy bitmaps */
			flags = 0;
			বিরতি;
		ক্ষেত্র 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			যদি (err)
				exit(1);
			বিরতি;
		ক্ষেত্র 'R':
			request = optarg;
			বিরতি;
		ক্ষেত্র 'f':
			cmd_file = optarg;
			বিরতি;
		পূর্বনির্ধারিত:
			com_err(argv[0], 0, "Usage: %s [-R request] "
				"[-f cmd_file]", subsystem_name);
			exit(1);
		}
	}

	sci_idx = ss_create_invocation(subsystem_name, version,
				       (অক্ষর *)ফাঁকা, &tst_bitmaps_cmds, &code);
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
		code = ss_execute_সারি(sci_idx, request);
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

