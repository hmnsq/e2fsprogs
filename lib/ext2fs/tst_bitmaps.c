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

#شامل "config.h"
#شامل <unistd.h>
#شامل <stdlib.h>
#شامل <stdio.h>
#ifdef HAVE_GETOPT_H
#شامل <getopt.h>
#endif
#شامل <string.h>
#شامل <fcntl.h>
#شامل <time.h>
#شامل <sys/stat.h>
#شامل <sys/types.h>
#شامل "ss/ss.h"

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

بیرونی ss_request_table tst_bitmaps_cmds;

ایستا نویسه subsystem_name[] = "tst_bitmaps";
ایستا نویسه version[] = "1.0";

ext2_filsys	test_fs;
صحیح		exit_status = 0;

ایستا صحیح source_file(ثابت نویسه *cmd_file, صحیح sci_idx)
{
	FILE		*f;
	نویسه		buf[256];
	نویسه		*cp;
	صحیح		retval;
	صحیح 		noecho;

	اگر (strcmp(cmd_file, "-") == 0)
		f = stdin;
	وگرنه {
		f = fopen(cmd_file, "r");
		اگر (!f) {
			perror(cmd_file);
			exit(1);
		}
	}
	fflush(stdout);
	fflush(stderr);
	setbuf(stdout, پوچ);
	setbuf(stderr, پوچ);
	وقتی (!feof(f)) {
		اگر (fgets(buf, اندازه(buf), f) == پوچ)
			بشکن;
		اگر (buf[0] == '#')
			ادامه;
		noecho = 0;
		اگر (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = strchr(buf, '\n');
		اگر (cp)
			*cp = 0;
		cp = strchr(buf, '\r');
		اگر (cp)
			*cp = 0;
		اگر (!noecho)
			printf("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_سطر(sci_idx, buf);
		اگر (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_status++;
		}
	}
	بازگشت exit_status;
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
تهی reset_getopt(تهی)
{
#اگر defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#وگرنه
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
unsigned long parse_ulong(ثابت نویسه *str, ثابت نویسه *cmd,
			  ثابت نویسه *descr, صحیح *err)
{
	نویسه		*tmp;
	unsigned long	ret;

	ret = strtoul(str, &tmp, 0);
	اگر (*tmp == 0) {
		اگر (err)
			*err = 0;
		بازگشت ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	اگر (err)
		*err = 1;
	وگرنه
		exit(1);
	بازگشت 0;
}


صحیح check_fs_open(ثابت نویسه *name)
{
	اگر (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		بازگشت 1;
	}
	بازگشت 0;
}

ایستا تهی setup_filesystem(ثابت نویسه *name,
			     unsigned صحیح blocks, unsigned صحیح inodes,
			     unsigned صحیح type, صحیح flags)
{
	ساختار ext2_super_block param;
	errcode_t retval;

	memset(&param, 0, اندازه(param));
	ext2fs_blocks_شمار_set(&param, blocks);
	param.s_inodes_شمار = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	اگر (retval) {
		com_err(name, retval, "while initializing filesystem");
		بازگشت;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_block_bitmap(test_fs->block_map);
	test_fs->block_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_block_bitmap(test_fs, "block bitmap",
					      &test_fs->block_map);
	اگر (retval) {
		com_err(name, retval, "while allocating block bitmap");
		برو errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	اگر (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		برو errout;
	}
	بازگشت;

errout:
	ext2fs_close_free(&test_fs);
}

تهی setup_cmd(صحیح argc, ss_argv_t argv,
	       صحیح sci_idx EXT2FS_ATTR((unused)),
	       تهی *infop EXT2FS_ATTR((unused)))
{
	صحیح		c, err;
	unsigned صحیح	blocks = 128;
	unsigned صحیح	inodes = 0;
	unsigned صحیح	type = EXT2FS_BMAP64_BITARRAY;
	صحیح		flags = EXT2_FLAG_64BITS;

	اگر (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	وقتی ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		انتخاب (c) {
		حالت 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			اگر (err)
				بازگشت;
			بشکن;
		حالت 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			اگر (err)
				بازگشت;
			بشکن;
		حالت 'l':	/* Legacy bitmaps */
			flags = 0;
			بشکن;
		حالت 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			اگر (err)
				بازگشت;
			بشکن;
		پیشفرض:
			fprintf(stderr, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			بازگشت;
		}
	}
	setup_filesystem(argv[0], blocks, inodes, type, flags);
}

تهی close_cmd(صحیح argc, نویسه **argv, صحیح sci_idx EXT2FS_ATTR((unused)),
	       تهی *infop EXT2FS_ATTR((unused)))
{
	اگر (check_fs_open(argv[0]))
		بازگشت;

	ext2fs_close_free(&test_fs);
}


تهی dump_bitmap(ext2fs_generic_bitmap bmap, unsigned صحیح start, unsigned رقم)
{
	unsigned نویسه	*buf;
	errcode_t	retval;
	صحیح		i, len = (رقم - start + 7) / 8;

	buf = malloc(len);
	اگر (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		بازگشت;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) start, رقم, buf);
	اگر (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		آزاد(buf);
		بازگشت;
	}
	برای (i=0; i < len; i++)
		printf("%02x", buf[i]);
	printf("\n");
	printf("bits set: %u\n", ext2fs_bitcount(buf, len));
	آزاد(buf);
}

تهی dump_inode_bitmap_cmd(صحیح argc, ss_argv_t argv,
			   صحیح sci_idx EXT2FS_ATTR((unused)),
			   تهی *infop EXT2FS_ATTR((unused)))
{
	اگر (check_fs_open(argv[0]))
		بازگشت;

	printf("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_شمار);
}
	
تهی dump_block_bitmap_cmd(صحیح argc, ss_argv_t argv,
			   صحیح sci_idx EXT2FS_ATTR((unused)),
			   تهی *infop EXT2FS_ATTR((unused)))
{
	اگر (check_fs_open(argv[0]))
		بازگشت;

	printf("block bitmap: ");
	dump_bitmap(test_fs->block_map, test_fs->super->s_first_data_block,
		    test_fs->super->s_blocks_شمار);
}
	
تهی do_setb(صحیح argc, ss_argv_t argv,
	     صحیح sci_idx EXT2FS_ATTR((unused)),
	     تهی *infop EXT2FS_ATTR((unused)))
{
	unsigned صحیح block, رقم;
	صحیح err;
	صحیح test_result, op_result;

	اگر (check_fs_open(argv[0]))
		بازگشت;

	اگر (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		بازگشت;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	اگر (err)
		بازگشت;

	اگر (argc == 3) {
		رقم = parse_ulong(argv[2], argv[0], "num", &err);
		اگر (err)
			بازگشت;

		ext2fs_mark_block_bitmap_range2(test_fs->block_map,
						block, رقم);
		printf("Marking blocks %u to %u\n", block, block + رقم - 1);
		بازگشت;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_mark_block_bitmap2(test_fs->block_map, block);
	printf("Setting block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	اگر (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

تهی do_clearb(صحیح argc, ss_argv_t argv,
	       صحیح sci_idx EXT2FS_ATTR((unused)),
	       تهی *infop EXT2FS_ATTR((unused)))
{
	unsigned صحیح block, رقم;
	صحیح err;
	صحیح test_result, op_result;

	اگر (check_fs_open(argv[0]))
		بازگشت;

	اگر (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		بازگشت;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	اگر (err)
		بازگشت;

	اگر (argc == 3) {
		رقم = parse_ulong(argv[2], argv[0], "num", &err);
		اگر (err)
			بازگشت;

		ext2fs_unmark_block_bitmap_range2(test_fs->block_map,
						block, رقم);
		printf("Clearing blocks %u to %u\n", block, block + رقم - 1);
		بازگشت;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_unmark_block_bitmap2(test_fs->block_map, block);
	printf("Clearing block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	اگر (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

تهی do_testb(صحیح argc, ss_argv_t argv,
	      صحیح sci_idx EXT2FS_ATTR((unused)),
	      تهی *infop EXT2FS_ATTR((unused)))
{
	unsigned صحیح block, رقم;
	صحیح err;
	صحیح test_result;

	اگر (check_fs_open(argv[0]))
		بازگشت;

	اگر (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		بازگشت;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	اگر (err)
		بازگشت;

	اگر (argc == 3) {
		رقم = parse_ulong(argv[2], argv[0], "num", &err);
		اگر (err)
			بازگشت;

		test_result =
			ext2fs_test_block_bitmap_range2(test_fs->block_map,
							block, رقم);
		printf("Blocks %u to %u are %sall clear.\n",
		       block, block + رقم - 1, test_result ? "" : "NOT ");
		بازگشت;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	printf("Block %u is %s\n", block, test_result ? "set" : "clear");
}

تهی do_ffzb(صحیح argc, ss_argv_t argv,
	     صحیح sci_idx EXT2FS_ATTR((unused)),
	     تهی *infop EXT2FS_ATTR((unused)))
{
	unsigned صحیح start, end;
	صحیح err;
	errcode_t retval;
	blk64_t out;

	اگر (check_fs_open(argv[0]))
		بازگشت;

	اگر (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		بازگشت;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	اگر (err)
		بازگشت;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	اگر (err)
		بازگشت;

	retval = ext2fs_find_first_zero_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	اگر (retval) {
		printf("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_message(retval));
		بازگشت;
	}
	printf("First unmarked block is %llu\n", (unsigned long long) out);
}

تهی do_ffsb(صحیح argc, ss_argv_t argv,
	     صحیح sci_idx EXT2FS_ATTR((unused)),
	     تهی *infop EXT2FS_ATTR((unused)))
{
	unsigned صحیح start, end;
	صحیح err;
	errcode_t retval;
	blk64_t out;

	اگر (check_fs_open(argv[0]))
		بازگشت;

	اگر (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		بازگشت;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	اگر (err)
		بازگشت;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	اگر (err)
		بازگشت;

	retval = ext2fs_find_first_set_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	اگر (retval) {
		printf("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_message(retval));
		بازگشت;
	}
	printf("First marked block is %llu\n", (unsigned long long) out);
}


تهی do_zerob(صحیح argc, ss_argv_t argv,
	      صحیح sci_idx EXT2FS_ATTR((unused)),
	      تهی *infop EXT2FS_ATTR((unused)))
{
	اگر (check_fs_open(argv[0]))
		بازگشت;

	printf("Clearing block bitmap.\n");
	ext2fs_clear_block_bitmap(test_fs->block_map);
}

تهی do_seti(صحیح argc, ss_argv_t argv,
	     صحیح sci_idx EXT2FS_ATTR((unused)),
	     تهی *infop EXT2FS_ATTR((unused)))
{
	unsigned صحیح inode;
	صحیح err;
	صحیح test_result, op_result;

	اگر (check_fs_open(argv[0]))
		بازگشت;

	اگر (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		بازگشت;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	اگر (err)
		بازگشت;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Setting inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	اگر (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

تهی do_cleari(صحیح argc, ss_argv_t argv,
	       صحیح sci_idx EXT2FS_ATTR((unused)),
	       تهی *infop EXT2FS_ATTR((unused)))
{
	unsigned صحیح inode;
	صحیح err;
	صحیح test_result, op_result;

	اگر (check_fs_open(argv[0]))
		بازگشت;

	اگر (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		بازگشت;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	اگر (err)
		بازگشت;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Clearing inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	اگر (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

تهی do_testi(صحیح argc, ss_argv_t argv,
	      صحیح sci_idx EXT2FS_ATTR((unused)),
	      تهی *infop EXT2FS_ATTR((unused)))
{
	unsigned صحیح inode;
	صحیح err;
	صحیح test_result;

	اگر (check_fs_open(argv[0]))
		بازگشت;

	اگر (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		بازگشت;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	اگر (err)
		بازگشت;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	printf("Inode %u is %s\n", inode, test_result ? "set" : "clear");
}

تهی do_ffzi(صحیح argc, ss_argv_t argv,
	     صحیح sci_idx EXT2FS_ATTR((unused)),
	     تهی *infop EXT2FS_ATTR((unused)))
{
	unsigned صحیح start, end;
	صحیح err;
	errcode_t retval;
	ext2_ino_t out;

	اگر (check_fs_open(argv[0]))
		بازگشت;

	اگر (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		بازگشت;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	اگر (err)
		بازگشت;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	اگر (err)
		بازگشت;

	retval = ext2fs_find_first_zero_inode_bitmap2(test_fs->inode_map,
						      start, end, &out);
	اگر (retval) {
		printf("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_message(retval));
		بازگشت;
	}
	printf("First unmarked inode is %u\n", out);
}

تهی do_ffsi(صحیح argc, ss_argv_t argv,
	     صحیح sci_idx EXT2FS_ATTR((unused)),
	     تهی *infop EXT2FS_ATTR((unused)))
{
	unsigned صحیح start, end;
	صحیح err;
	errcode_t retval;
	ext2_ino_t out;

	اگر (check_fs_open(argv[0]))
		بازگشت;

	اگر (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		بازگشت;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	اگر (err)
		بازگشت;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	اگر (err)
		بازگشت;

	retval = ext2fs_find_first_set_inode_bitmap2(test_fs->inode_map,
						     start, end, &out);
	اگر (retval) {
		printf("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_message(retval));
		بازگشت;
	}
	printf("First marked inode is %u\n", out);
}

تهی do_zeroi(صحیح argc, ss_argv_t argv,
	      صحیح sci_idx EXT2FS_ATTR((unused)),
	      تهی *infop EXT2FS_ATTR((unused)))
{
	اگر (check_fs_open(argv[0]))
		بازگشت;

	printf("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

صحیح اصلی(صحیح argc, نویسه **argv)
{
	unsigned صحیح	blocks = 128;
	unsigned صحیح	inodes = 0;
	unsigned صحیح	type = EXT2FS_BMAP64_BITARRAY;
	صحیح		c, err, code;
	نویسه		*request = (نویسه *)پوچ;
	نویسه		*cmd_file = 0;
	صحیح		sci_idx;
	صحیح		flags = EXT2_FLAG_64BITS;

	add_error_table(&et_ss_error_table);
	add_error_table(&et_ext2_error_table);
	وقتی ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		انتخاب (c) {
		حالت 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			اگر (err)
				exit(1);
			بشکن;
		حالت 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			اگر (err)
				exit(1);
			بشکن;
		حالت 'l':	/* Legacy bitmaps */
			flags = 0;
			بشکن;
		حالت 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			اگر (err)
				exit(1);
			بشکن;
		حالت 'R':
			request = optarg;
			بشکن;
		حالت 'f':
			cmd_file = optarg;
			بشکن;
		پیشفرض:
			com_err(argv[0], 0, "Usage: %s [-R request] "
				"[-f cmd_file]", subsystem_name);
			exit(1);
		}
	}

	sci_idx = ss_create_invocation(subsystem_name, version,
				       (نویسه *)پوچ, &tst_bitmaps_cmds, &code);
	اگر (code) {
		ss_perror(sci_idx, code, "creating invocation");
		exit(1);
	}

	(تهی) ss_add_request_table (sci_idx, &ss_std_requests, 1, &code);
	اگر (code) {
		ss_perror(sci_idx, code, "adding standard requests");
		exit (1);
	}

	printf("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, version);

	setup_filesystem(argv[0], blocks, inodes, type, flags);

	اگر (request) {
		code = ss_execute_سطر(sci_idx, request);
		اگر (code) {
			ss_perror(sci_idx, code, request);
			exit_status++;
		}
	} وگرنه اگر (cmd_file) {
		exit_status = source_file(cmd_file, sci_idx);
	} وگرنه {
		ss_listen(sci_idx);
	}

	exit(exit_status);
}

