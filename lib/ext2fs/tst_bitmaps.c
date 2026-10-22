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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <stdio.h>
#ifdef HAVE_GETOPT_H
#அந்தர்பாவ <getopt.h>
#endif
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ "ss/ss.h"

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"
#அந்தர்பாவ "ext2fsP.h"

வெளி ss_request_table tst_bitmaps_cmds;

நிலையான எழுத்து subsystem_name[] = "tst_bitmaps";
நிலையான எழுத்து version[] = "1.0";

ext2_filsys	test_fs;
முழு		exit_status = 0;

நிலையான முழு source_file(மாறிலி எழுத்து *cmd_file, முழு sci_idx)
{
	FILE		*f;
	எழுத்து		buf[256];
	எழுத்து		*cp;
	முழு		retval;
	முழு 		noecho;

	எனில் (strcmp(cmd_file, "-") == 0)
		f = stdin;
	இல்லையெனில் {
		f = fopen(cmd_file, "r");
		எனில் (!f) {
			perror(cmd_file);
			exit(1);
		}
	}
	fflush(stdout);
	fflush(stderr);
	setbuf(stdout, வெற்று);
	setbuf(stderr, வெற்று);
	போது (!feof(f)) {
		எனில் (fgets(buf, அளவு(buf), f) == வெற்று)
			நிறுத்து;
		எனில் (buf[0] == '#')
			தொடர்;
		noecho = 0;
		எனில் (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = strchr(buf, '\n');
		எனில் (cp)
			*cp = 0;
		cp = strchr(buf, '\r');
		எனில் (cp)
			*cp = 0;
		எனில் (!noecho)
			printf("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_வரி(sci_idx, buf);
		எனில் (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_status++;
		}
	}
	திரும்பு exit_status;
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
வெறுமை reset_getopt(வெறுமை)
{
#எனில் defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#இல்லையெனில்
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
unsigned long parse_ulong(மாறிலி எழுத்து *str, மாறிலி எழுத்து *cmd,
			  மாறிலி எழுத்து *descr, முழு *err)
{
	எழுத்து		*tmp;
	unsigned long	ret;

	ret = strtoul(str, &tmp, 0);
	எனில் (*tmp == 0) {
		எனில் (err)
			*err = 0;
		திரும்பு ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	எனில் (err)
		*err = 1;
	இல்லையெனில்
		exit(1);
	திரும்பு 0;
}


முழு check_fs_open(மாறிலி எழுத்து *name)
{
	எனில் (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		திரும்பு 1;
	}
	திரும்பு 0;
}

நிலையான வெறுமை setup_filesystem(மாறிலி எழுத்து *name,
			     unsigned முழு blocks, unsigned முழு inodes,
			     unsigned முழு type, முழு flags)
{
	கட்டமைப்பு ext2_super_block param;
	errcode_t retval;

	memset(&param, 0, அளவு(param));
	ext2fs_blocks_மொத்தம்_set(&param, blocks);
	param.s_inodes_மொத்தம் = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	எனில் (retval) {
		com_err(name, retval, "while initializing filesystem");
		திரும்பு;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_block_bitmap(test_fs->block_map);
	test_fs->block_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_block_bitmap(test_fs, "block bitmap",
					      &test_fs->block_map);
	எனில் (retval) {
		com_err(name, retval, "while allocating block bitmap");
		செல் errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	எனில் (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		செல் errout;
	}
	திரும்பு;

errout:
	ext2fs_close_free(&test_fs);
}

வெறுமை setup_cmd(முழு argc, ss_argv_t argv,
	       முழு sci_idx EXT2FS_ATTR((unused)),
	       வெறுமை *infop EXT2FS_ATTR((unused)))
{
	முழு		c, err;
	unsigned முழு	blocks = 128;
	unsigned முழு	inodes = 0;
	unsigned முழு	type = EXT2FS_BMAP64_BITARRAY;
	முழு		flags = EXT2_FLAG_64BITS;

	எனில் (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	போது ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		தேர்வு (c) {
		நிலை 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			எனில் (err)
				திரும்பு;
			நிறுத்து;
		நிலை 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			எனில் (err)
				திரும்பு;
			நிறுத்து;
		நிலை 'l':	/* Legacy bitmaps */
			flags = 0;
			நிறுத்து;
		நிலை 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			எனில் (err)
				திரும்பு;
			நிறுத்து;
		இயல்பு:
			fprintf(stderr, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			திரும்பு;
		}
	}
	setup_filesystem(argv[0], blocks, inodes, type, flags);
}

வெறுமை close_cmd(முழு argc, எழுத்து **argv, முழு sci_idx EXT2FS_ATTR((unused)),
	       வெறுமை *infop EXT2FS_ATTR((unused)))
{
	எனில் (check_fs_open(argv[0]))
		திரும்பு;

	ext2fs_close_free(&test_fs);
}


வெறுமை dump_bitmap(ext2fs_generic_bitmap bmap, unsigned முழு start, unsigned இலக்கம்)
{
	unsigned எழுத்து	*buf;
	errcode_t	retval;
	முழு		i, len = (இலக்கம் - start + 7) / 8;

	buf = malloc(len);
	எனில் (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		திரும்பு;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) start, இலக்கம், buf);
	எனில் (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		விடுவி(buf);
		திரும்பு;
	}
	க்கு (i=0; i < len; i++)
		printf("%02x", buf[i]);
	printf("\n");
	printf("bits set: %u\n", ext2fs_bitcount(buf, len));
	விடுவி(buf);
}

வெறுமை dump_inode_bitmap_cmd(முழு argc, ss_argv_t argv,
			   முழு sci_idx EXT2FS_ATTR((unused)),
			   வெறுமை *infop EXT2FS_ATTR((unused)))
{
	எனில் (check_fs_open(argv[0]))
		திரும்பு;

	printf("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_மொத்தம்);
}
	
வெறுமை dump_block_bitmap_cmd(முழு argc, ss_argv_t argv,
			   முழு sci_idx EXT2FS_ATTR((unused)),
			   வெறுமை *infop EXT2FS_ATTR((unused)))
{
	எனில் (check_fs_open(argv[0]))
		திரும்பு;

	printf("block bitmap: ");
	dump_bitmap(test_fs->block_map, test_fs->super->s_first_data_block,
		    test_fs->super->s_blocks_மொத்தம்);
}
	
வெறுமை do_setb(முழு argc, ss_argv_t argv,
	     முழு sci_idx EXT2FS_ATTR((unused)),
	     வெறுமை *infop EXT2FS_ATTR((unused)))
{
	unsigned முழு block, இலக்கம்;
	முழு err;
	முழு test_result, op_result;

	எனில் (check_fs_open(argv[0]))
		திரும்பு;

	எனில் (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		திரும்பு;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	எனில் (err)
		திரும்பு;

	எனில் (argc == 3) {
		இலக்கம் = parse_ulong(argv[2], argv[0], "num", &err);
		எனில் (err)
			திரும்பு;

		ext2fs_mark_block_bitmap_range2(test_fs->block_map,
						block, இலக்கம்);
		printf("Marking blocks %u to %u\n", block, block + இலக்கம் - 1);
		திரும்பு;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_mark_block_bitmap2(test_fs->block_map, block);
	printf("Setting block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	எனில் (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

வெறுமை do_clearb(முழு argc, ss_argv_t argv,
	       முழு sci_idx EXT2FS_ATTR((unused)),
	       வெறுமை *infop EXT2FS_ATTR((unused)))
{
	unsigned முழு block, இலக்கம்;
	முழு err;
	முழு test_result, op_result;

	எனில் (check_fs_open(argv[0]))
		திரும்பு;

	எனில் (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		திரும்பு;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	எனில் (err)
		திரும்பு;

	எனில் (argc == 3) {
		இலக்கம் = parse_ulong(argv[2], argv[0], "num", &err);
		எனில் (err)
			திரும்பு;

		ext2fs_unmark_block_bitmap_range2(test_fs->block_map,
						block, இலக்கம்);
		printf("Clearing blocks %u to %u\n", block, block + இலக்கம் - 1);
		திரும்பு;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_unmark_block_bitmap2(test_fs->block_map, block);
	printf("Clearing block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	எனில் (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

வெறுமை do_testb(முழு argc, ss_argv_t argv,
	      முழு sci_idx EXT2FS_ATTR((unused)),
	      வெறுமை *infop EXT2FS_ATTR((unused)))
{
	unsigned முழு block, இலக்கம்;
	முழு err;
	முழு test_result;

	எனில் (check_fs_open(argv[0]))
		திரும்பு;

	எனில் (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		திரும்பு;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	எனில் (err)
		திரும்பு;

	எனில் (argc == 3) {
		இலக்கம் = parse_ulong(argv[2], argv[0], "num", &err);
		எனில் (err)
			திரும்பு;

		test_result =
			ext2fs_test_block_bitmap_range2(test_fs->block_map,
							block, இலக்கம்);
		printf("Blocks %u to %u are %sall clear.\n",
		       block, block + இலக்கம் - 1, test_result ? "" : "NOT ");
		திரும்பு;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	printf("Block %u is %s\n", block, test_result ? "set" : "clear");
}

வெறுமை do_ffzb(முழு argc, ss_argv_t argv,
	     முழு sci_idx EXT2FS_ATTR((unused)),
	     வெறுமை *infop EXT2FS_ATTR((unused)))
{
	unsigned முழு start, end;
	முழு err;
	errcode_t retval;
	blk64_t out;

	எனில் (check_fs_open(argv[0]))
		திரும்பு;

	எனில் (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		திரும்பு;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	எனில் (err)
		திரும்பு;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	எனில் (err)
		திரும்பு;

	retval = ext2fs_find_first_zero_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	எனில் (retval) {
		printf("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_message(retval));
		திரும்பு;
	}
	printf("First unmarked block is %llu\n", (unsigned long long) out);
}

வெறுமை do_ffsb(முழு argc, ss_argv_t argv,
	     முழு sci_idx EXT2FS_ATTR((unused)),
	     வெறுமை *infop EXT2FS_ATTR((unused)))
{
	unsigned முழு start, end;
	முழு err;
	errcode_t retval;
	blk64_t out;

	எனில் (check_fs_open(argv[0]))
		திரும்பு;

	எனில் (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		திரும்பு;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	எனில் (err)
		திரும்பு;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	எனில் (err)
		திரும்பு;

	retval = ext2fs_find_first_set_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	எனில் (retval) {
		printf("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_message(retval));
		திரும்பு;
	}
	printf("First marked block is %llu\n", (unsigned long long) out);
}


வெறுமை do_zerob(முழு argc, ss_argv_t argv,
	      முழு sci_idx EXT2FS_ATTR((unused)),
	      வெறுமை *infop EXT2FS_ATTR((unused)))
{
	எனில் (check_fs_open(argv[0]))
		திரும்பு;

	printf("Clearing block bitmap.\n");
	ext2fs_clear_block_bitmap(test_fs->block_map);
}

வெறுமை do_seti(முழு argc, ss_argv_t argv,
	     முழு sci_idx EXT2FS_ATTR((unused)),
	     வெறுமை *infop EXT2FS_ATTR((unused)))
{
	unsigned முழு inode;
	முழு err;
	முழு test_result, op_result;

	எனில் (check_fs_open(argv[0]))
		திரும்பு;

	எனில் (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		திரும்பு;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	எனில் (err)
		திரும்பு;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Setting inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	எனில் (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

வெறுமை do_cleari(முழு argc, ss_argv_t argv,
	       முழு sci_idx EXT2FS_ATTR((unused)),
	       வெறுமை *infop EXT2FS_ATTR((unused)))
{
	unsigned முழு inode;
	முழு err;
	முழு test_result, op_result;

	எனில் (check_fs_open(argv[0]))
		திரும்பு;

	எனில் (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		திரும்பு;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	எனில் (err)
		திரும்பு;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Clearing inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	எனில் (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

வெறுமை do_testi(முழு argc, ss_argv_t argv,
	      முழு sci_idx EXT2FS_ATTR((unused)),
	      வெறுமை *infop EXT2FS_ATTR((unused)))
{
	unsigned முழு inode;
	முழு err;
	முழு test_result;

	எனில் (check_fs_open(argv[0]))
		திரும்பு;

	எனில் (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		திரும்பு;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	எனில் (err)
		திரும்பு;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	printf("Inode %u is %s\n", inode, test_result ? "set" : "clear");
}

வெறுமை do_ffzi(முழு argc, ss_argv_t argv,
	     முழு sci_idx EXT2FS_ATTR((unused)),
	     வெறுமை *infop EXT2FS_ATTR((unused)))
{
	unsigned முழு start, end;
	முழு err;
	errcode_t retval;
	ext2_ino_t out;

	எனில் (check_fs_open(argv[0]))
		திரும்பு;

	எனில் (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		திரும்பு;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	எனில் (err)
		திரும்பு;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	எனில் (err)
		திரும்பு;

	retval = ext2fs_find_first_zero_inode_bitmap2(test_fs->inode_map,
						      start, end, &out);
	எனில் (retval) {
		printf("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_message(retval));
		திரும்பு;
	}
	printf("First unmarked inode is %u\n", out);
}

வெறுமை do_ffsi(முழு argc, ss_argv_t argv,
	     முழு sci_idx EXT2FS_ATTR((unused)),
	     வெறுமை *infop EXT2FS_ATTR((unused)))
{
	unsigned முழு start, end;
	முழு err;
	errcode_t retval;
	ext2_ino_t out;

	எனில் (check_fs_open(argv[0]))
		திரும்பு;

	எனில் (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		திரும்பு;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	எனில் (err)
		திரும்பு;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	எனில் (err)
		திரும்பு;

	retval = ext2fs_find_first_set_inode_bitmap2(test_fs->inode_map,
						     start, end, &out);
	எனில் (retval) {
		printf("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_message(retval));
		திரும்பு;
	}
	printf("First marked inode is %u\n", out);
}

வெறுமை do_zeroi(முழு argc, ss_argv_t argv,
	      முழு sci_idx EXT2FS_ATTR((unused)),
	      வெறுமை *infop EXT2FS_ATTR((unused)))
{
	எனில் (check_fs_open(argv[0]))
		திரும்பு;

	printf("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

முழு முதன்மை(முழு argc, எழுத்து **argv)
{
	unsigned முழு	blocks = 128;
	unsigned முழு	inodes = 0;
	unsigned முழு	type = EXT2FS_BMAP64_BITARRAY;
	முழு		c, err, code;
	எழுத்து		*request = (எழுத்து *)வெற்று;
	எழுத்து		*cmd_file = 0;
	முழு		sci_idx;
	முழு		flags = EXT2_FLAG_64BITS;

	add_error_table(&et_ss_error_table);
	add_error_table(&et_ext2_error_table);
	போது ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		தேர்வு (c) {
		நிலை 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			எனில் (err)
				exit(1);
			நிறுத்து;
		நிலை 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			எனில் (err)
				exit(1);
			நிறுத்து;
		நிலை 'l':	/* Legacy bitmaps */
			flags = 0;
			நிறுத்து;
		நிலை 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			எனில் (err)
				exit(1);
			நிறுத்து;
		நிலை 'R':
			request = optarg;
			நிறுத்து;
		நிலை 'f':
			cmd_file = optarg;
			நிறுத்து;
		இயல்பு:
			com_err(argv[0], 0, "Usage: %s [-R request] "
				"[-f cmd_file]", subsystem_name);
			exit(1);
		}
	}

	sci_idx = ss_create_invocation(subsystem_name, version,
				       (எழுத்து *)வெற்று, &tst_bitmaps_cmds, &code);
	எனில் (code) {
		ss_perror(sci_idx, code, "creating invocation");
		exit(1);
	}

	(வெறுமை) ss_add_request_table (sci_idx, &ss_std_requests, 1, &code);
	எனில் (code) {
		ss_perror(sci_idx, code, "adding standard requests");
		exit (1);
	}

	printf("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, version);

	setup_filesystem(argv[0], blocks, inodes, type, flags);

	எனில் (request) {
		code = ss_execute_வரி(sci_idx, request);
		எனில் (code) {
			ss_perror(sci_idx, code, request);
			exit_status++;
		}
	} இல்லையெனில் எனில் (cmd_file) {
		exit_status = source_file(cmd_file, sci_idx);
	} இல்லையெனில் {
		ss_listen(sci_idx);
	}

	exit(exit_status);
}

