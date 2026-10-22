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

#ضمن "config.h"
#ضمن <unistd.h>
#ضمن <stdlib.h>
#ضمن <stdio.h>
#ifdef HAVE_GETOPT_H
#ضمن <getopt.h>
#endif
#ضمن <string.h>
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#ضمن "ss/ss.h"

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

خارجي ss_request_table tst_bitmaps_cmds;

ساكن حرف subsystem_name[] = "tst_bitmaps";
ساكن حرف version[] = "1.0";

ext2_filsys	test_fs;
صحيح		exit_status = 0;

ساكن صحيح source_file(ثابت حرف *cmd_file, صحيح sci_idx)
{
	FILE		*f;
	حرف		buf[256];
	حرف		*cp;
	صحيح		retval;
	صحيح 		noecho;

	ܐܢ (strcmp(cmd_file, "-") == 0)
		f = stdin;
	ܐܚܪܢܐ {
		f = fopen(cmd_file, "r");
		ܐܢ (!f) {
			perror(cmd_file);
			exit(1);
		}
	}
	fflush(stdout);
	fflush(stderr);
	setbuf(stdout, عدم);
	setbuf(stderr, عدم);
	ܟܡܐ (!feof(f)) {
		ܐܢ (fgets(buf, حجم(buf), f) == عدم)
			اكسر;
		ܐܢ (buf[0] == '#')
			استمر;
		noecho = 0;
		ܐܢ (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = strchr(buf, '\n');
		ܐܢ (cp)
			*cp = 0;
		cp = strchr(buf, '\r');
		ܐܢ (cp)
			*cp = 0;
		ܐܢ (!noecho)
			printf("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_line(sci_idx, buf);
		ܐܢ (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_status++;
		}
	}
	ܦܢܐ exit_status;
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
فراغ reset_getopt(فراغ)
{
#ܐܢ defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#ܐܚܪܢܐ
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
unsigned long parse_ulong(ثابت حرف *str, ثابت حرف *cmd,
			  ثابت حرف *descr, صحيح *err)
{
	حرف		*tmp;
	unsigned long	ret;

	ret = strtoul(str, &tmp, 0);
	ܐܢ (*tmp == 0) {
		ܐܢ (err)
			*err = 0;
		ܦܢܐ ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	ܐܢ (err)
		*err = 1;
	ܐܚܪܢܐ
		exit(1);
	ܦܢܐ 0;
}


صحيح check_fs_open(ثابت حرف *name)
{
	ܐܢ (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		ܦܢܐ 1;
	}
	ܦܢܐ 0;
}

ساكن فراغ setup_filesystem(ثابت حرف *name,
			     unsigned صحيح blocks, unsigned صحيح inodes,
			     unsigned صحيح type, صحيح flags)
{
	بنية ext2_super_block param;
	errcode_t retval;

	memset(&param, 0, حجم(param));
	ext2fs_blocks_count_set(&param, blocks);
	param.s_inodes_count = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	ܐܢ (retval) {
		com_err(name, retval, "while initializing filesystem");
		ܦܢܐ;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_block_bitmap(test_fs->block_map);
	test_fs->block_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_block_bitmap(test_fs, "block bitmap",
					      &test_fs->block_map);
	ܐܢ (retval) {
		com_err(name, retval, "while allocating block bitmap");
		اذهب errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	ܐܢ (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		اذهب errout;
	}
	ܦܢܐ;

errout:
	ext2fs_close_free(&test_fs);
}

فراغ setup_cmd(صحيح argc, ss_argv_t argv,
	       صحيح sci_idx EXT2FS_ATTR((unused)),
	       فراغ *infop EXT2FS_ATTR((unused)))
{
	صحيح		c, err;
	unsigned صحيح	blocks = 128;
	unsigned صحيح	inodes = 0;
	unsigned صحيح	type = EXT2FS_BMAP64_BITARRAY;
	صحيح		flags = EXT2_FLAG_64BITS;

	ܐܢ (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	ܟܡܐ ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		بدل (c) {
		حالة 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ܐܢ (err)
				ܦܢܐ;
			اكسر;
		حالة 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ܐܢ (err)
				ܦܢܐ;
			اكسر;
		حالة 'l':	/* Legacy bitmaps */
			flags = 0;
			اكسر;
		حالة 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			ܐܢ (err)
				ܦܢܐ;
			اكسر;
		افتراضي:
			fprintf(stderr, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			ܦܢܐ;
		}
	}
	setup_filesystem(argv[0], blocks, inodes, type, flags);
}

فراغ close_cmd(صحيح argc, حرف **argv, صحيح sci_idx EXT2FS_ATTR((unused)),
	       فراغ *infop EXT2FS_ATTR((unused)))
{
	ܐܢ (check_fs_open(argv[0]))
		ܦܢܐ;

	ext2fs_close_free(&test_fs);
}


فراغ dump_bitmap(ext2fs_generic_bitmap bmap, unsigned صحيح start, unsigned num)
{
	unsigned حرف	*buf;
	errcode_t	retval;
	صحيح		i, len = (num - start + 7) / 8;

	buf = malloc(len);
	ܐܢ (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		ܦܢܐ;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) start, num, buf);
	ܐܢ (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		حرر(buf);
		ܦܢܐ;
	}
	لكل (i=0; i < len; i++)
		printf("%02x", buf[i]);
	printf("\n");
	printf("bits set: %u\n", ext2fs_bitcount(buf, len));
	حرر(buf);
}

فراغ dump_inode_bitmap_cmd(صحيح argc, ss_argv_t argv,
			   صحيح sci_idx EXT2FS_ATTR((unused)),
			   فراغ *infop EXT2FS_ATTR((unused)))
{
	ܐܢ (check_fs_open(argv[0]))
		ܦܢܐ;

	printf("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_count);
}
	
فراغ dump_block_bitmap_cmd(صحيح argc, ss_argv_t argv,
			   صحيح sci_idx EXT2FS_ATTR((unused)),
			   فراغ *infop EXT2FS_ATTR((unused)))
{
	ܐܢ (check_fs_open(argv[0]))
		ܦܢܐ;

	printf("block bitmap: ");
	dump_bitmap(test_fs->block_map, test_fs->super->s_first_data_block,
		    test_fs->super->s_blocks_count);
}
	
فراغ do_setb(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((unused)),
	     فراغ *infop EXT2FS_ATTR((unused)))
{
	unsigned صحيح block, num;
	صحيح err;
	صحيح test_result, op_result;

	ܐܢ (check_fs_open(argv[0]))
		ܦܢܐ;

	ܐܢ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		ܦܢܐ;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ܐܢ (err)
		ܦܢܐ;

	ܐܢ (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		ܐܢ (err)
			ܦܢܐ;

		ext2fs_mark_block_bitmap_range2(test_fs->block_map,
						block, num);
		printf("Marking blocks %u to %u\n", block, block + num - 1);
		ܦܢܐ;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_mark_block_bitmap2(test_fs->block_map, block);
	printf("Setting block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	ܐܢ (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

فراغ do_clearb(صحيح argc, ss_argv_t argv,
	       صحيح sci_idx EXT2FS_ATTR((unused)),
	       فراغ *infop EXT2FS_ATTR((unused)))
{
	unsigned صحيح block, num;
	صحيح err;
	صحيح test_result, op_result;

	ܐܢ (check_fs_open(argv[0]))
		ܦܢܐ;

	ܐܢ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		ܦܢܐ;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ܐܢ (err)
		ܦܢܐ;

	ܐܢ (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		ܐܢ (err)
			ܦܢܐ;

		ext2fs_unmark_block_bitmap_range2(test_fs->block_map,
						block, num);
		printf("Clearing blocks %u to %u\n", block, block + num - 1);
		ܦܢܐ;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_unmark_block_bitmap2(test_fs->block_map, block);
	printf("Clearing block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	ܐܢ (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

فراغ do_testb(صحيح argc, ss_argv_t argv,
	      صحيح sci_idx EXT2FS_ATTR((unused)),
	      فراغ *infop EXT2FS_ATTR((unused)))
{
	unsigned صحيح block, num;
	صحيح err;
	صحيح test_result;

	ܐܢ (check_fs_open(argv[0]))
		ܦܢܐ;

	ܐܢ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		ܦܢܐ;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ܐܢ (err)
		ܦܢܐ;

	ܐܢ (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		ܐܢ (err)
			ܦܢܐ;

		test_result =
			ext2fs_test_block_bitmap_range2(test_fs->block_map,
							block, num);
		printf("Blocks %u to %u are %sall clear.\n",
		       block, block + num - 1, test_result ? "" : "NOT ");
		ܦܢܐ;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	printf("Block %u is %s\n", block, test_result ? "set" : "clear");
}

فراغ do_ffzb(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((unused)),
	     فراغ *infop EXT2FS_ATTR((unused)))
{
	unsigned صحيح start, end;
	صحيح err;
	errcode_t retval;
	blk64_t out;

	ܐܢ (check_fs_open(argv[0]))
		ܦܢܐ;

	ܐܢ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		ܦܢܐ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ܐܢ (err)
		ܦܢܐ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ܐܢ (err)
		ܦܢܐ;

	retval = ext2fs_find_first_zero_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	ܐܢ (retval) {
		printf("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_message(retval));
		ܦܢܐ;
	}
	printf("First unmarked block is %llu\n", (unsigned long long) out);
}

فراغ do_ffsb(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((unused)),
	     فراغ *infop EXT2FS_ATTR((unused)))
{
	unsigned صحيح start, end;
	صحيح err;
	errcode_t retval;
	blk64_t out;

	ܐܢ (check_fs_open(argv[0]))
		ܦܢܐ;

	ܐܢ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		ܦܢܐ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ܐܢ (err)
		ܦܢܐ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ܐܢ (err)
		ܦܢܐ;

	retval = ext2fs_find_first_set_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	ܐܢ (retval) {
		printf("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_message(retval));
		ܦܢܐ;
	}
	printf("First marked block is %llu\n", (unsigned long long) out);
}


فراغ do_zerob(صحيح argc, ss_argv_t argv,
	      صحيح sci_idx EXT2FS_ATTR((unused)),
	      فراغ *infop EXT2FS_ATTR((unused)))
{
	ܐܢ (check_fs_open(argv[0]))
		ܦܢܐ;

	printf("Clearing block bitmap.\n");
	ext2fs_clear_block_bitmap(test_fs->block_map);
}

فراغ do_seti(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((unused)),
	     فراغ *infop EXT2FS_ATTR((unused)))
{
	unsigned صحيح inode;
	صحيح err;
	صحيح test_result, op_result;

	ܐܢ (check_fs_open(argv[0]))
		ܦܢܐ;

	ܐܢ (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		ܦܢܐ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ܐܢ (err)
		ܦܢܐ;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Setting inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	ܐܢ (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

فراغ do_cleari(صحيح argc, ss_argv_t argv,
	       صحيح sci_idx EXT2FS_ATTR((unused)),
	       فراغ *infop EXT2FS_ATTR((unused)))
{
	unsigned صحيح inode;
	صحيح err;
	صحيح test_result, op_result;

	ܐܢ (check_fs_open(argv[0]))
		ܦܢܐ;

	ܐܢ (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		ܦܢܐ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ܐܢ (err)
		ܦܢܐ;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Clearing inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	ܐܢ (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

فراغ do_testi(صحيح argc, ss_argv_t argv,
	      صحيح sci_idx EXT2FS_ATTR((unused)),
	      فراغ *infop EXT2FS_ATTR((unused)))
{
	unsigned صحيح inode;
	صحيح err;
	صحيح test_result;

	ܐܢ (check_fs_open(argv[0]))
		ܦܢܐ;

	ܐܢ (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		ܦܢܐ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ܐܢ (err)
		ܦܢܐ;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	printf("Inode %u is %s\n", inode, test_result ? "set" : "clear");
}

فراغ do_ffzi(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((unused)),
	     فراغ *infop EXT2FS_ATTR((unused)))
{
	unsigned صحيح start, end;
	صحيح err;
	errcode_t retval;
	ext2_ino_t out;

	ܐܢ (check_fs_open(argv[0]))
		ܦܢܐ;

	ܐܢ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		ܦܢܐ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ܐܢ (err)
		ܦܢܐ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ܐܢ (err)
		ܦܢܐ;

	retval = ext2fs_find_first_zero_inode_bitmap2(test_fs->inode_map,
						      start, end, &out);
	ܐܢ (retval) {
		printf("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_message(retval));
		ܦܢܐ;
	}
	printf("First unmarked inode is %u\n", out);
}

فراغ do_ffsi(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((unused)),
	     فراغ *infop EXT2FS_ATTR((unused)))
{
	unsigned صحيح start, end;
	صحيح err;
	errcode_t retval;
	ext2_ino_t out;

	ܐܢ (check_fs_open(argv[0]))
		ܦܢܐ;

	ܐܢ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		ܦܢܐ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ܐܢ (err)
		ܦܢܐ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ܐܢ (err)
		ܦܢܐ;

	retval = ext2fs_find_first_set_inode_bitmap2(test_fs->inode_map,
						     start, end, &out);
	ܐܢ (retval) {
		printf("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_message(retval));
		ܦܢܐ;
	}
	printf("First marked inode is %u\n", out);
}

فراغ do_zeroi(صحيح argc, ss_argv_t argv,
	      صحيح sci_idx EXT2FS_ATTR((unused)),
	      فراغ *infop EXT2FS_ATTR((unused)))
{
	ܐܢ (check_fs_open(argv[0]))
		ܦܢܐ;

	printf("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

صحيح main(صحيح argc, حرف **argv)
{
	unsigned صحيح	blocks = 128;
	unsigned صحيح	inodes = 0;
	unsigned صحيح	type = EXT2FS_BMAP64_BITARRAY;
	صحيح		c, err, code;
	حرف		*request = (حرف *)عدم;
	حرف		*cmd_file = 0;
	صحيح		sci_idx;
	صحيح		flags = EXT2_FLAG_64BITS;

	add_error_table(&et_ss_error_table);
	add_error_table(&et_ext2_error_table);
	ܟܡܐ ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		بدل (c) {
		حالة 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ܐܢ (err)
				exit(1);
			اكسر;
		حالة 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ܐܢ (err)
				exit(1);
			اكسر;
		حالة 'l':	/* Legacy bitmaps */
			flags = 0;
			اكسر;
		حالة 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			ܐܢ (err)
				exit(1);
			اكسر;
		حالة 'R':
			request = optarg;
			اكسر;
		حالة 'f':
			cmd_file = optarg;
			اكسر;
		افتراضي:
			com_err(argv[0], 0, "Usage: %s [-R request] "
				"[-f cmd_file]", subsystem_name);
			exit(1);
		}
	}

	sci_idx = ss_create_invocation(subsystem_name, version,
				       (حرف *)عدم, &tst_bitmaps_cmds, &code);
	ܐܢ (code) {
		ss_perror(sci_idx, code, "creating invocation");
		exit(1);
	}

	(فراغ) ss_add_request_table (sci_idx, &ss_std_requests, 1, &code);
	ܐܢ (code) {
		ss_perror(sci_idx, code, "adding standard requests");
		exit (1);
	}

	printf("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, version);

	setup_filesystem(argv[0], blocks, inodes, type, flags);

	ܐܢ (request) {
		code = ss_execute_line(sci_idx, request);
		ܐܢ (code) {
			ss_perror(sci_idx, code, request);
			exit_status++;
		}
	} ܐܚܪܢܐ ܐܢ (cmd_file) {
		exit_status = source_file(cmd_file, sci_idx);
	} ܐܚܪܢܐ {
		ss_listen(sci_idx);
	}

	exit(exit_status);
}

