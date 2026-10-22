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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ifdef HAVE_GETOPT_H
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#endif
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ "ss/ss.h"

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

ବାହ୍ୟ ss_request_table tst_bitmaps_cmds;

ସ୍ଥିର ଅକ୍ଷର subsystem_name[] = "tst_bitmaps";
ସ୍ଥିର ଅକ୍ଷର version[] = "1.0";

ext2_filsys	test_fs;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା		exit_status = 0;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା source_file(ଅଚଳ ଅକ୍ଷର *cmd_file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sci_idx)
{
	FILE		*f;
	ଅକ୍ଷର		buf[256];
	ଅକ୍ଷର		*cp;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		retval;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା 		noecho;

	ଯଦି (strcmp(cmd_file, "-") == 0)
		f = stdin;
	ଅନ୍ୟଥା {
		f = fopen(cmd_file, "r");
		ଯଦି (!f) {
			perror(cmd_file);
			exit(1);
		}
	}
	fflush(stdout);
	fflush(stderr);
	setbuf(stdout, ଖାଲି);
	setbuf(stderr, ଖାଲି);
	ଯେତେବେଳେ (!feof(f)) {
		ଯଦି (fgets(buf, ଆକାର(buf), f) == ଖାଲି)
			ଭାଙ୍ଗ;
		ଯଦି (buf[0] == '#')
			ଜାରି;
		noecho = 0;
		ଯଦି (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = strchr(buf, '\n');
		ଯଦି (cp)
			*cp = 0;
		cp = strchr(buf, '\r');
		ଯଦି (cp)
			*cp = 0;
		ଯଦି (!noecho)
			printf("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_ଧାଡ଼ି(sci_idx, buf);
		ଯଦି (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_status++;
		}
	}
	ଫେରନ୍ତୁ exit_status;
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
ଶୂନ୍ୟ reset_getopt(ଶୂନ୍ୟ)
{
#ଯଦି defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#ଅନ୍ୟଥା
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
unsigned long parse_ulong(ଅଚଳ ଅକ୍ଷର *str, ଅଚଳ ଅକ୍ଷର *cmd,
			  ଅଚଳ ଅକ୍ଷର *descr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *err)
{
	ଅକ୍ଷର		*tmp;
	unsigned long	ret;

	ret = strtoul(str, &tmp, 0);
	ଯଦି (*tmp == 0) {
		ଯଦି (err)
			*err = 0;
		ଫେରନ୍ତୁ ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	ଯଦି (err)
		*err = 1;
	ଅନ୍ୟଥା
		exit(1);
	ଫେରନ୍ତୁ 0;
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_fs_open(ଅଚଳ ଅକ୍ଷର *name)
{
	ଯଦି (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		ଫେରନ୍ତୁ 1;
	}
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ setup_filesystem(ଅଚଳ ଅକ୍ଷର *name,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା blocks, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା inodes,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା ext2_super_block param;
	errcode_t retval;

	memset(&param, 0, ଆକାର(param));
	ext2fs_blocks_ଗଣନା_set(&param, blocks);
	param.s_inodes_ଗଣନା = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	ଯଦି (retval) {
		com_err(name, retval, "while initializing filesystem");
		ଫେରନ୍ତୁ;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_block_bitmap(test_fs->block_map);
	test_fs->block_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_block_bitmap(test_fs, "block bitmap",
					      &test_fs->block_map);
	ଯଦି (retval) {
		com_err(name, retval, "while allocating block bitmap");
		ଯାଅ errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	ଯଦି (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		ଯାଅ errout;
	}
	ଫେରନ୍ତୁ;

errout:
	ext2fs_close_free(&test_fs);
}

ଶୂନ୍ୟ setup_cmd(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ss_argv_t argv,
	       ପୂର୍ଣ୍ଣସଂଖ୍ୟା sci_idx EXT2FS_ATTR((unused)),
	       ଶୂନ୍ୟ *infop EXT2FS_ATTR((unused)))
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		c, err;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	blocks = 128;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	inodes = 0;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	type = EXT2FS_BMAP64_BITARRAY;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		flags = EXT2_FLAG_64BITS;

	ଯଦି (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	ଯେତେବେଳେ ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		ବାଛ (c) {
		ସ୍ଥିତି 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ଯଦି (err)
				ଫେରନ୍ତୁ;
			ଭାଙ୍ଗ;
		ସ୍ଥିତି 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ଯଦି (err)
				ଫେରନ୍ତୁ;
			ଭାଙ୍ଗ;
		ସ୍ଥିତି 'l':	/* Legacy bitmaps */
			flags = 0;
			ଭାଙ୍ଗ;
		ସ୍ଥିତି 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			ଯଦି (err)
				ଫେରନ୍ତୁ;
			ଭାଙ୍ଗ;
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			fprintf(stderr, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			ଫେରନ୍ତୁ;
		}
	}
	setup_filesystem(argv[0], blocks, inodes, type, flags);
}

ଶୂନ୍ୟ close_cmd(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sci_idx EXT2FS_ATTR((unused)),
	       ଶୂନ୍ୟ *infop EXT2FS_ATTR((unused)))
{
	ଯଦି (check_fs_open(argv[0]))
		ଫେରନ୍ତୁ;

	ext2fs_close_free(&test_fs);
}


ଶୂନ୍ୟ dump_bitmap(ext2fs_generic_bitmap bmap, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, unsigned ଅଙ୍କ)
{
	unsigned ଅକ୍ଷର	*buf;
	errcode_t	retval;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		i, len = (ଅଙ୍କ - start + 7) / 8;

	buf = malloc(len);
	ଯଦି (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		ଫେରନ୍ତୁ;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) start, ଅଙ୍କ, buf);
	ଯଦି (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		ମୁକ୍ତ(buf);
		ଫେରନ୍ତୁ;
	}
	ପାଇଁ (i=0; i < len; i++)
		printf("%02x", buf[i]);
	printf("\n");
	printf("bits set: %u\n", ext2fs_bitcount(buf, len));
	ମୁକ୍ତ(buf);
}

ଶୂନ୍ୟ dump_inode_bitmap_cmd(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ss_argv_t argv,
			   ପୂର୍ଣ୍ଣସଂଖ୍ୟା sci_idx EXT2FS_ATTR((unused)),
			   ଶୂନ୍ୟ *infop EXT2FS_ATTR((unused)))
{
	ଯଦି (check_fs_open(argv[0]))
		ଫେରନ୍ତୁ;

	printf("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_ଗଣନା);
}
	
ଶୂନ୍ୟ dump_block_bitmap_cmd(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ss_argv_t argv,
			   ପୂର୍ଣ୍ଣସଂଖ୍ୟା sci_idx EXT2FS_ATTR((unused)),
			   ଶୂନ୍ୟ *infop EXT2FS_ATTR((unused)))
{
	ଯଦି (check_fs_open(argv[0]))
		ଫେରନ୍ତୁ;

	printf("block bitmap: ");
	dump_bitmap(test_fs->block_map, test_fs->super->s_first_data_block,
		    test_fs->super->s_blocks_ଗଣନା);
}
	
ଶୂନ୍ୟ do_setb(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ss_argv_t argv,
	     ପୂର୍ଣ୍ଣସଂଖ୍ୟା sci_idx EXT2FS_ATTR((unused)),
	     ଶୂନ୍ୟ *infop EXT2FS_ATTR((unused)))
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା block, ଅଙ୍କ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା test_result, op_result;

	ଯଦି (check_fs_open(argv[0]))
		ଫେରନ୍ତୁ;

	ଯଦି (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		ଫେରନ୍ତୁ;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ଯଦି (err)
		ଫେରନ୍ତୁ;

	ଯଦି (argc == 3) {
		ଅଙ୍କ = parse_ulong(argv[2], argv[0], "num", &err);
		ଯଦି (err)
			ଫେରନ୍ତୁ;

		ext2fs_mark_block_bitmap_range2(test_fs->block_map,
						block, ଅଙ୍କ);
		printf("Marking blocks %u to %u\n", block, block + ଅଙ୍କ - 1);
		ଫେରନ୍ତୁ;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_mark_block_bitmap2(test_fs->block_map, block);
	printf("Setting block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	ଯଦି (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

ଶୂନ୍ୟ do_clearb(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ss_argv_t argv,
	       ପୂର୍ଣ୍ଣସଂଖ୍ୟା sci_idx EXT2FS_ATTR((unused)),
	       ଶୂନ୍ୟ *infop EXT2FS_ATTR((unused)))
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା block, ଅଙ୍କ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା test_result, op_result;

	ଯଦି (check_fs_open(argv[0]))
		ଫେରନ୍ତୁ;

	ଯଦି (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		ଫେରନ୍ତୁ;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ଯଦି (err)
		ଫେରନ୍ତୁ;

	ଯଦି (argc == 3) {
		ଅଙ୍କ = parse_ulong(argv[2], argv[0], "num", &err);
		ଯଦି (err)
			ଫେରନ୍ତୁ;

		ext2fs_unmark_block_bitmap_range2(test_fs->block_map,
						block, ଅଙ୍କ);
		printf("Clearing blocks %u to %u\n", block, block + ଅଙ୍କ - 1);
		ଫେରନ୍ତୁ;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_unmark_block_bitmap2(test_fs->block_map, block);
	printf("Clearing block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	ଯଦି (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

ଶୂନ୍ୟ do_testb(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ss_argv_t argv,
	      ପୂର୍ଣ୍ଣସଂଖ୍ୟା sci_idx EXT2FS_ATTR((unused)),
	      ଶୂନ୍ୟ *infop EXT2FS_ATTR((unused)))
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା block, ଅଙ୍କ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା test_result;

	ଯଦି (check_fs_open(argv[0]))
		ଫେରନ୍ତୁ;

	ଯଦି (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		ଫେରନ୍ତୁ;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ଯଦି (err)
		ଫେରନ୍ତୁ;

	ଯଦି (argc == 3) {
		ଅଙ୍କ = parse_ulong(argv[2], argv[0], "num", &err);
		ଯଦି (err)
			ଫେରନ୍ତୁ;

		test_result =
			ext2fs_test_block_bitmap_range2(test_fs->block_map,
							block, ଅଙ୍କ);
		printf("Blocks %u to %u are %sall clear.\n",
		       block, block + ଅଙ୍କ - 1, test_result ? "" : "NOT ");
		ଫେରନ୍ତୁ;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	printf("Block %u is %s\n", block, test_result ? "set" : "clear");
}

ଶୂନ୍ୟ do_ffzb(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ss_argv_t argv,
	     ପୂର୍ଣ୍ଣସଂଖ୍ୟା sci_idx EXT2FS_ATTR((unused)),
	     ଶୂନ୍ୟ *infop EXT2FS_ATTR((unused)))
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, end;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;
	errcode_t retval;
	blk64_t out;

	ଯଦି (check_fs_open(argv[0]))
		ଫେରନ୍ତୁ;

	ଯଦି (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		ଫେରନ୍ତୁ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ଯଦି (err)
		ଫେରନ୍ତୁ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ଯଦି (err)
		ଫେରନ୍ତୁ;

	retval = ext2fs_find_first_zero_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	ଯଦି (retval) {
		printf("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_message(retval));
		ଫେରନ୍ତୁ;
	}
	printf("First unmarked block is %llu\n", (unsigned long long) out);
}

ଶୂନ୍ୟ do_ffsb(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ss_argv_t argv,
	     ପୂର୍ଣ୍ଣସଂଖ୍ୟା sci_idx EXT2FS_ATTR((unused)),
	     ଶୂନ୍ୟ *infop EXT2FS_ATTR((unused)))
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, end;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;
	errcode_t retval;
	blk64_t out;

	ଯଦି (check_fs_open(argv[0]))
		ଫେରନ୍ତୁ;

	ଯଦି (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		ଫେରନ୍ତୁ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ଯଦି (err)
		ଫେରନ୍ତୁ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ଯଦି (err)
		ଫେରନ୍ତୁ;

	retval = ext2fs_find_first_set_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	ଯଦି (retval) {
		printf("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_message(retval));
		ଫେରନ୍ତୁ;
	}
	printf("First marked block is %llu\n", (unsigned long long) out);
}


ଶୂନ୍ୟ do_zerob(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ss_argv_t argv,
	      ପୂର୍ଣ୍ଣସଂଖ୍ୟା sci_idx EXT2FS_ATTR((unused)),
	      ଶୂନ୍ୟ *infop EXT2FS_ATTR((unused)))
{
	ଯଦି (check_fs_open(argv[0]))
		ଫେରନ୍ତୁ;

	printf("Clearing block bitmap.\n");
	ext2fs_clear_block_bitmap(test_fs->block_map);
}

ଶୂନ୍ୟ do_seti(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ss_argv_t argv,
	     ପୂର୍ଣ୍ଣସଂଖ୍ୟା sci_idx EXT2FS_ATTR((unused)),
	     ଶୂନ୍ୟ *infop EXT2FS_ATTR((unused)))
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା inode;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା test_result, op_result;

	ଯଦି (check_fs_open(argv[0]))
		ଫେରନ୍ତୁ;

	ଯଦି (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		ଫେରନ୍ତୁ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ଯଦି (err)
		ଫେରନ୍ତୁ;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Setting inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	ଯଦି (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

ଶୂନ୍ୟ do_cleari(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ss_argv_t argv,
	       ପୂର୍ଣ୍ଣସଂଖ୍ୟା sci_idx EXT2FS_ATTR((unused)),
	       ଶୂନ୍ୟ *infop EXT2FS_ATTR((unused)))
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା inode;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା test_result, op_result;

	ଯଦି (check_fs_open(argv[0]))
		ଫେରନ୍ତୁ;

	ଯଦି (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		ଫେରନ୍ତୁ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ଯଦି (err)
		ଫେରନ୍ତୁ;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Clearing inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	ଯଦି (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

ଶୂନ୍ୟ do_testi(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ss_argv_t argv,
	      ପୂର୍ଣ୍ଣସଂଖ୍ୟା sci_idx EXT2FS_ATTR((unused)),
	      ଶୂନ୍ୟ *infop EXT2FS_ATTR((unused)))
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା inode;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା test_result;

	ଯଦି (check_fs_open(argv[0]))
		ଫେରନ୍ତୁ;

	ଯଦି (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		ଫେରନ୍ତୁ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ଯଦି (err)
		ଫେରନ୍ତୁ;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	printf("Inode %u is %s\n", inode, test_result ? "set" : "clear");
}

ଶୂନ୍ୟ do_ffzi(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ss_argv_t argv,
	     ପୂର୍ଣ୍ଣସଂଖ୍ୟା sci_idx EXT2FS_ATTR((unused)),
	     ଶୂନ୍ୟ *infop EXT2FS_ATTR((unused)))
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, end;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;
	errcode_t retval;
	ext2_ino_t out;

	ଯଦି (check_fs_open(argv[0]))
		ଫେରନ୍ତୁ;

	ଯଦି (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		ଫେରନ୍ତୁ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ଯଦି (err)
		ଫେରନ୍ତୁ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ଯଦି (err)
		ଫେରନ୍ତୁ;

	retval = ext2fs_find_first_zero_inode_bitmap2(test_fs->inode_map,
						      start, end, &out);
	ଯଦି (retval) {
		printf("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_message(retval));
		ଫେରନ୍ତୁ;
	}
	printf("First unmarked inode is %u\n", out);
}

ଶୂନ୍ୟ do_ffsi(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ss_argv_t argv,
	     ପୂର୍ଣ୍ଣସଂଖ୍ୟା sci_idx EXT2FS_ATTR((unused)),
	     ଶୂନ୍ୟ *infop EXT2FS_ATTR((unused)))
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, end;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;
	errcode_t retval;
	ext2_ino_t out;

	ଯଦି (check_fs_open(argv[0]))
		ଫେରନ୍ତୁ;

	ଯଦି (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		ଫେରନ୍ତୁ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ଯଦି (err)
		ଫେରନ୍ତୁ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ଯଦି (err)
		ଫେରନ୍ତୁ;

	retval = ext2fs_find_first_set_inode_bitmap2(test_fs->inode_map,
						     start, end, &out);
	ଯଦି (retval) {
		printf("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_message(retval));
		ଫେରନ୍ତୁ;
	}
	printf("First marked inode is %u\n", out);
}

ଶୂନ୍ୟ do_zeroi(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ss_argv_t argv,
	      ପୂର୍ଣ୍ଣସଂଖ୍ୟା sci_idx EXT2FS_ATTR((unused)),
	      ଶୂନ୍ୟ *infop EXT2FS_ATTR((unused)))
{
	ଯଦି (check_fs_open(argv[0]))
		ଫେରନ୍ତୁ;

	printf("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୁଖ୍ୟ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	blocks = 128;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	inodes = 0;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	type = EXT2FS_BMAP64_BITARRAY;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		c, err, code;
	ଅକ୍ଷର		*request = (ଅକ୍ଷର *)ଖାଲି;
	ଅକ୍ଷର		*cmd_file = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		sci_idx;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		flags = EXT2_FLAG_64BITS;

	add_error_table(&et_ss_error_table);
	add_error_table(&et_ext2_error_table);
	ଯେତେବେଳେ ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		ବାଛ (c) {
		ସ୍ଥିତି 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ଯଦି (err)
				exit(1);
			ଭାଙ୍ଗ;
		ସ୍ଥିତି 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ଯଦି (err)
				exit(1);
			ଭାଙ୍ଗ;
		ସ୍ଥିତି 'l':	/* Legacy bitmaps */
			flags = 0;
			ଭାଙ୍ଗ;
		ସ୍ଥିତି 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			ଯଦି (err)
				exit(1);
			ଭାଙ୍ଗ;
		ସ୍ଥିତି 'R':
			request = optarg;
			ଭାଙ୍ଗ;
		ସ୍ଥିତି 'f':
			cmd_file = optarg;
			ଭାଙ୍ଗ;
		ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
			com_err(argv[0], 0, "Usage: %s [-R request] "
				"[-f cmd_file]", subsystem_name);
			exit(1);
		}
	}

	sci_idx = ss_create_invocation(subsystem_name, version,
				       (ଅକ୍ଷର *)ଖାଲି, &tst_bitmaps_cmds, &code);
	ଯଦି (code) {
		ss_perror(sci_idx, code, "creating invocation");
		exit(1);
	}

	(ଶୂନ୍ୟ) ss_add_request_table (sci_idx, &ss_std_requests, 1, &code);
	ଯଦି (code) {
		ss_perror(sci_idx, code, "adding standard requests");
		exit (1);
	}

	printf("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, version);

	setup_filesystem(argv[0], blocks, inodes, type, flags);

	ଯଦି (request) {
		code = ss_execute_ଧାଡ଼ି(sci_idx, request);
		ଯଦି (code) {
			ss_perror(sci_idx, code, request);
			exit_status++;
		}
	} ଅନ୍ୟଥା ଯଦି (cmd_file) {
		exit_status = source_file(cmd_file, sci_idx);
	} ଅନ୍ୟଥା {
		ss_listen(sci_idx);
	}

	exit(exit_status);
}

