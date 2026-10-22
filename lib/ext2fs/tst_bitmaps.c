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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <stdio.h>
#ifdef HAVE_GETOPT_H
#અન્તર્ભાવ <getopt.h>
#endif
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <time.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ "ss/ss.h"

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"
#અન્તર્ભાવ "ext2fsP.h"

બાહ્ય ss_request_table tst_bitmaps_cmds;

સ્થિર અક્ષર subsystem_name[] = "tst_bitmaps";
સ્થિર અક્ષર version[] = "1.0";

ext2_filsys	test_fs;
પૂર્ણાંક		exit_status = 0;

સ્થિર પૂર્ણાંક source_file(અચલ અક્ષર *cmd_file, પૂર્ણાંક sci_idx)
{
	FILE		*f;
	અક્ષર		buf[256];
	અક્ષર		*cp;
	પૂર્ણાંક		retval;
	પૂર્ણાંક 		noecho;

	જો (strcmp(cmd_file, "-") == 0)
		f = stdin;
	નહીંતર {
		f = fopen(cmd_file, "r");
		જો (!f) {
			perror(cmd_file);
			exit(1);
		}
	}
	fflush(stdout);
	fflush(stderr);
	setbuf(stdout, ખાલી);
	setbuf(stderr, ખાલી);
	જ્યારે (!feof(f)) {
		જો (fgets(buf, કદ(buf), f) == ખાલી)
			તોડો;
		જો (buf[0] == '#')
			ચાલુ;
		noecho = 0;
		જો (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = strchr(buf, '\n');
		જો (cp)
			*cp = 0;
		cp = strchr(buf, '\r');
		જો (cp)
			*cp = 0;
		જો (!noecho)
			printf("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_લીટી(sci_idx, buf);
		જો (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_status++;
		}
	}
	પાછા exit_status;
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
શૂન્ય reset_getopt(શૂન્ય)
{
#જો defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#નહીંતર
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
unsigned long parse_ulong(અચલ અક્ષર *str, અચલ અક્ષર *cmd,
			  અચલ અક્ષર *descr, પૂર્ણાંક *err)
{
	અક્ષર		*tmp;
	unsigned long	ret;

	ret = strtoul(str, &tmp, 0);
	જો (*tmp == 0) {
		જો (err)
			*err = 0;
		પાછા ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	જો (err)
		*err = 1;
	નહીંતર
		exit(1);
	પાછા 0;
}


પૂર્ણાંક check_fs_open(અચલ અક્ષર *name)
{
	જો (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		પાછા 1;
	}
	પાછા 0;
}

સ્થિર શૂન્ય setup_filesystem(અચલ અક્ષર *name,
			     unsigned પૂર્ણાંક blocks, unsigned પૂર્ણાંક inodes,
			     unsigned પૂર્ણાંક type, પૂર્ણાંક flags)
{
	સંરચના ext2_super_block param;
	errcode_t retval;

	memset(&param, 0, કદ(param));
	ext2fs_blocks_ગણતરી_set(&param, blocks);
	param.s_inodes_ગણતરી = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	જો (retval) {
		com_err(name, retval, "while initializing filesystem");
		પાછા;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_block_bitmap(test_fs->block_map);
	test_fs->block_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_block_bitmap(test_fs, "block bitmap",
					      &test_fs->block_map);
	જો (retval) {
		com_err(name, retval, "while allocating block bitmap");
		જાઓ errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	જો (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		જાઓ errout;
	}
	પાછા;

errout:
	ext2fs_close_free(&test_fs);
}

શૂન્ય setup_cmd(પૂર્ણાંક argc, ss_argv_t argv,
	       પૂર્ણાંક sci_idx EXT2FS_ATTR((unused)),
	       શૂન્ય *infop EXT2FS_ATTR((unused)))
{
	પૂર્ણાંક		c, err;
	unsigned પૂર્ણાંક	blocks = 128;
	unsigned પૂર્ણાંક	inodes = 0;
	unsigned પૂર્ણાંક	type = EXT2FS_BMAP64_BITARRAY;
	પૂર્ણાંક		flags = EXT2_FLAG_64BITS;

	જો (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	જ્યારે ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		પસંદગી (c) {
		સ્થિતિ 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			જો (err)
				પાછા;
			તોડો;
		સ્થિતિ 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			જો (err)
				પાછા;
			તોડો;
		સ્થિતિ 'l':	/* Legacy bitmaps */
			flags = 0;
			તોડો;
		સ્થિતિ 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			જો (err)
				પાછા;
			તોડો;
		મૂળભૂત:
			fprintf(stderr, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			પાછા;
		}
	}
	setup_filesystem(argv[0], blocks, inodes, type, flags);
}

શૂન્ય close_cmd(પૂર્ણાંક argc, અક્ષર **argv, પૂર્ણાંક sci_idx EXT2FS_ATTR((unused)),
	       શૂન્ય *infop EXT2FS_ATTR((unused)))
{
	જો (check_fs_open(argv[0]))
		પાછા;

	ext2fs_close_free(&test_fs);
}


શૂન્ય dump_bitmap(ext2fs_generic_bitmap bmap, unsigned પૂર્ણાંક start, unsigned અંક)
{
	unsigned અક્ષર	*buf;
	errcode_t	retval;
	પૂર્ણાંક		i, len = (અંક - start + 7) / 8;

	buf = malloc(len);
	જો (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		પાછા;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) start, અંક, buf);
	જો (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		મુક્ત(buf);
		પાછા;
	}
	માટે (i=0; i < len; i++)
		printf("%02x", buf[i]);
	printf("\n");
	printf("bits set: %u\n", ext2fs_bitcount(buf, len));
	મુક્ત(buf);
}

શૂન્ય dump_inode_bitmap_cmd(પૂર્ણાંક argc, ss_argv_t argv,
			   પૂર્ણાંક sci_idx EXT2FS_ATTR((unused)),
			   શૂન્ય *infop EXT2FS_ATTR((unused)))
{
	જો (check_fs_open(argv[0]))
		પાછા;

	printf("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_ગણતરી);
}
	
શૂન્ય dump_block_bitmap_cmd(પૂર્ણાંક argc, ss_argv_t argv,
			   પૂર્ણાંક sci_idx EXT2FS_ATTR((unused)),
			   શૂન્ય *infop EXT2FS_ATTR((unused)))
{
	જો (check_fs_open(argv[0]))
		પાછા;

	printf("block bitmap: ");
	dump_bitmap(test_fs->block_map, test_fs->super->s_first_data_block,
		    test_fs->super->s_blocks_ગણતરી);
}
	
શૂન્ય do_setb(પૂર્ણાંક argc, ss_argv_t argv,
	     પૂર્ણાંક sci_idx EXT2FS_ATTR((unused)),
	     શૂન્ય *infop EXT2FS_ATTR((unused)))
{
	unsigned પૂર્ણાંક block, અંક;
	પૂર્ણાંક err;
	પૂર્ણાંક test_result, op_result;

	જો (check_fs_open(argv[0]))
		પાછા;

	જો (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		પાછા;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	જો (err)
		પાછા;

	જો (argc == 3) {
		અંક = parse_ulong(argv[2], argv[0], "num", &err);
		જો (err)
			પાછા;

		ext2fs_mark_block_bitmap_range2(test_fs->block_map,
						block, અંક);
		printf("Marking blocks %u to %u\n", block, block + અંક - 1);
		પાછા;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_mark_block_bitmap2(test_fs->block_map, block);
	printf("Setting block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	જો (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

શૂન્ય do_clearb(પૂર્ણાંક argc, ss_argv_t argv,
	       પૂર્ણાંક sci_idx EXT2FS_ATTR((unused)),
	       શૂન્ય *infop EXT2FS_ATTR((unused)))
{
	unsigned પૂર્ણાંક block, અંક;
	પૂર્ણાંક err;
	પૂર્ણાંક test_result, op_result;

	જો (check_fs_open(argv[0]))
		પાછા;

	જો (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		પાછા;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	જો (err)
		પાછા;

	જો (argc == 3) {
		અંક = parse_ulong(argv[2], argv[0], "num", &err);
		જો (err)
			પાછા;

		ext2fs_unmark_block_bitmap_range2(test_fs->block_map,
						block, અંક);
		printf("Clearing blocks %u to %u\n", block, block + અંક - 1);
		પાછા;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_unmark_block_bitmap2(test_fs->block_map, block);
	printf("Clearing block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	જો (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

શૂન્ય do_testb(પૂર્ણાંક argc, ss_argv_t argv,
	      પૂર્ણાંક sci_idx EXT2FS_ATTR((unused)),
	      શૂન્ય *infop EXT2FS_ATTR((unused)))
{
	unsigned પૂર્ણાંક block, અંક;
	પૂર્ણાંક err;
	પૂર્ણાંક test_result;

	જો (check_fs_open(argv[0]))
		પાછા;

	જો (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		પાછા;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	જો (err)
		પાછા;

	જો (argc == 3) {
		અંક = parse_ulong(argv[2], argv[0], "num", &err);
		જો (err)
			પાછા;

		test_result =
			ext2fs_test_block_bitmap_range2(test_fs->block_map,
							block, અંક);
		printf("Blocks %u to %u are %sall clear.\n",
		       block, block + અંક - 1, test_result ? "" : "NOT ");
		પાછા;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	printf("Block %u is %s\n", block, test_result ? "set" : "clear");
}

શૂન્ય do_ffzb(પૂર્ણાંક argc, ss_argv_t argv,
	     પૂર્ણાંક sci_idx EXT2FS_ATTR((unused)),
	     શૂન્ય *infop EXT2FS_ATTR((unused)))
{
	unsigned પૂર્ણાંક start, end;
	પૂર્ણાંક err;
	errcode_t retval;
	blk64_t out;

	જો (check_fs_open(argv[0]))
		પાછા;

	જો (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		પાછા;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	જો (err)
		પાછા;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	જો (err)
		પાછા;

	retval = ext2fs_find_first_zero_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	જો (retval) {
		printf("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_message(retval));
		પાછા;
	}
	printf("First unmarked block is %llu\n", (unsigned long long) out);
}

શૂન્ય do_ffsb(પૂર્ણાંક argc, ss_argv_t argv,
	     પૂર્ણાંક sci_idx EXT2FS_ATTR((unused)),
	     શૂન્ય *infop EXT2FS_ATTR((unused)))
{
	unsigned પૂર્ણાંક start, end;
	પૂર્ણાંક err;
	errcode_t retval;
	blk64_t out;

	જો (check_fs_open(argv[0]))
		પાછા;

	જો (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		પાછા;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	જો (err)
		પાછા;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	જો (err)
		પાછા;

	retval = ext2fs_find_first_set_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	જો (retval) {
		printf("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_message(retval));
		પાછા;
	}
	printf("First marked block is %llu\n", (unsigned long long) out);
}


શૂન્ય do_zerob(પૂર્ણાંક argc, ss_argv_t argv,
	      પૂર્ણાંક sci_idx EXT2FS_ATTR((unused)),
	      શૂન્ય *infop EXT2FS_ATTR((unused)))
{
	જો (check_fs_open(argv[0]))
		પાછા;

	printf("Clearing block bitmap.\n");
	ext2fs_clear_block_bitmap(test_fs->block_map);
}

શૂન્ય do_seti(પૂર્ણાંક argc, ss_argv_t argv,
	     પૂર્ણાંક sci_idx EXT2FS_ATTR((unused)),
	     શૂન્ય *infop EXT2FS_ATTR((unused)))
{
	unsigned પૂર્ણાંક inode;
	પૂર્ણાંક err;
	પૂર્ણાંક test_result, op_result;

	જો (check_fs_open(argv[0]))
		પાછા;

	જો (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		પાછા;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	જો (err)
		પાછા;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Setting inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	જો (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

શૂન્ય do_cleari(પૂર્ણાંક argc, ss_argv_t argv,
	       પૂર્ણાંક sci_idx EXT2FS_ATTR((unused)),
	       શૂન્ય *infop EXT2FS_ATTR((unused)))
{
	unsigned પૂર્ણાંક inode;
	પૂર્ણાંક err;
	પૂર્ણાંક test_result, op_result;

	જો (check_fs_open(argv[0]))
		પાછા;

	જો (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		પાછા;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	જો (err)
		પાછા;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Clearing inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	જો (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

શૂન્ય do_testi(પૂર્ણાંક argc, ss_argv_t argv,
	      પૂર્ણાંક sci_idx EXT2FS_ATTR((unused)),
	      શૂન્ય *infop EXT2FS_ATTR((unused)))
{
	unsigned પૂર્ણાંક inode;
	પૂર્ણાંક err;
	પૂર્ણાંક test_result;

	જો (check_fs_open(argv[0]))
		પાછા;

	જો (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		પાછા;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	જો (err)
		પાછા;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	printf("Inode %u is %s\n", inode, test_result ? "set" : "clear");
}

શૂન્ય do_ffzi(પૂર્ણાંક argc, ss_argv_t argv,
	     પૂર્ણાંક sci_idx EXT2FS_ATTR((unused)),
	     શૂન્ય *infop EXT2FS_ATTR((unused)))
{
	unsigned પૂર્ણાંક start, end;
	પૂર્ણાંક err;
	errcode_t retval;
	ext2_ino_t out;

	જો (check_fs_open(argv[0]))
		પાછા;

	જો (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		પાછા;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	જો (err)
		પાછા;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	જો (err)
		પાછા;

	retval = ext2fs_find_first_zero_inode_bitmap2(test_fs->inode_map,
						      start, end, &out);
	જો (retval) {
		printf("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_message(retval));
		પાછા;
	}
	printf("First unmarked inode is %u\n", out);
}

શૂન્ય do_ffsi(પૂર્ણાંક argc, ss_argv_t argv,
	     પૂર્ણાંક sci_idx EXT2FS_ATTR((unused)),
	     શૂન્ય *infop EXT2FS_ATTR((unused)))
{
	unsigned પૂર્ણાંક start, end;
	પૂર્ણાંક err;
	errcode_t retval;
	ext2_ino_t out;

	જો (check_fs_open(argv[0]))
		પાછા;

	જો (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		પાછા;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	જો (err)
		પાછા;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	જો (err)
		પાછા;

	retval = ext2fs_find_first_set_inode_bitmap2(test_fs->inode_map,
						     start, end, &out);
	જો (retval) {
		printf("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_message(retval));
		પાછા;
	}
	printf("First marked inode is %u\n", out);
}

શૂન્ય do_zeroi(પૂર્ણાંક argc, ss_argv_t argv,
	      પૂર્ણાંક sci_idx EXT2FS_ATTR((unused)),
	      શૂન્ય *infop EXT2FS_ATTR((unused)))
{
	જો (check_fs_open(argv[0]))
		પાછા;

	printf("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

પૂર્ણાંક મુખ્ય(પૂર્ણાંક argc, અક્ષર **argv)
{
	unsigned પૂર્ણાંક	blocks = 128;
	unsigned પૂર્ણાંક	inodes = 0;
	unsigned પૂર્ણાંક	type = EXT2FS_BMAP64_BITARRAY;
	પૂર્ણાંક		c, err, code;
	અક્ષર		*request = (અક્ષર *)ખાલી;
	અક્ષર		*cmd_file = 0;
	પૂર્ણાંક		sci_idx;
	પૂર્ણાંક		flags = EXT2_FLAG_64BITS;

	add_error_table(&et_ss_error_table);
	add_error_table(&et_ext2_error_table);
	જ્યારે ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		પસંદગી (c) {
		સ્થિતિ 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			જો (err)
				exit(1);
			તોડો;
		સ્થિતિ 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			જો (err)
				exit(1);
			તોડો;
		સ્થિતિ 'l':	/* Legacy bitmaps */
			flags = 0;
			તોડો;
		સ્થિતિ 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			જો (err)
				exit(1);
			તોડો;
		સ્થિતિ 'R':
			request = optarg;
			તોડો;
		સ્થિતિ 'f':
			cmd_file = optarg;
			તોડો;
		મૂળભૂત:
			com_err(argv[0], 0, "Usage: %s [-R request] "
				"[-f cmd_file]", subsystem_name);
			exit(1);
		}
	}

	sci_idx = ss_create_invocation(subsystem_name, version,
				       (અક્ષર *)ખાલી, &tst_bitmaps_cmds, &code);
	જો (code) {
		ss_perror(sci_idx, code, "creating invocation");
		exit(1);
	}

	(શૂન્ય) ss_add_request_table (sci_idx, &ss_std_requests, 1, &code);
	જો (code) {
		ss_perror(sci_idx, code, "adding standard requests");
		exit (1);
	}

	printf("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, version);

	setup_filesystem(argv[0], blocks, inodes, type, flags);

	જો (request) {
		code = ss_execute_લીટી(sci_idx, request);
		જો (code) {
			ss_perror(sci_idx, code, request);
			exit_status++;
		}
	} નહીંતર જો (cmd_file) {
		exit_status = source_file(cmd_file, sci_idx);
	} નહીંતર {
		ss_listen(sci_idx);
	}

	exit(exit_status);
}

