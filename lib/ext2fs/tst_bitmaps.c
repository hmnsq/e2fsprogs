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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <stdio.h>
#ifdef HAVE_GETOPT_H
#അന്തര്ഭാവ <getopt.h>
#endif
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <time.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ "ss/ss.h"

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

ബാഹ്യ ss_request_table tst_bitmaps_cmds;

സ്ഥിതം അക്ഷരം subsystem_name[] = "tst_bitmaps";
സ്ഥിതം അക്ഷരം version[] = "1.0";

ext2_filsys	test_fs;
പൂർണ്ണസംഖ്യ		exit_status = 0;

സ്ഥിതം പൂർണ്ണസംഖ്യ source_file(സ്ഥിരം അക്ഷരം *cmd_file, പൂർണ്ണസംഖ്യ sci_idx)
{
	FILE		*f;
	അക്ഷരം		buf[256];
	അക്ഷരം		*cp;
	പൂർണ്ണസംഖ്യ		retval;
	പൂർണ്ണസംഖ്യ 		noecho;

	എങ്കിൽ (strcmp(cmd_file, "-") == 0)
		f = stdin;
	അല്ലെങ്കിൽ {
		f = fopen(cmd_file, "r");
		എങ്കിൽ (!f) {
			perror(cmd_file);
			exit(1);
		}
	}
	fflush(stdout);
	fflush(stderr);
	setbuf(stdout, ശൂന്യ);
	setbuf(stderr, ശൂന്യ);
	ആകുമ്പോൾ (!feof(f)) {
		എങ്കിൽ (fgets(buf, വലുപ്പം(buf), f) == ശൂന്യ)
			നിർത്തുക;
		എങ്കിൽ (buf[0] == '#')
			തുടരുക;
		noecho = 0;
		എങ്കിൽ (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = strchr(buf, '\n');
		എങ്കിൽ (cp)
			*cp = 0;
		cp = strchr(buf, '\r');
		എങ്കിൽ (cp)
			*cp = 0;
		എങ്കിൽ (!noecho)
			printf("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_വരി(sci_idx, buf);
		എങ്കിൽ (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_status++;
		}
	}
	തിരികെ exit_status;
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
ശൂന്യം reset_getopt(ശൂന്യം)
{
#എങ്കിൽ defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#അല്ലെങ്കിൽ
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
unsigned long parse_ulong(സ്ഥിരം അക്ഷരം *str, സ്ഥിരം അക്ഷരം *cmd,
			  സ്ഥിരം അക്ഷരം *descr, പൂർണ്ണസംഖ്യ *err)
{
	അക്ഷരം		*tmp;
	unsigned long	ret;

	ret = strtoul(str, &tmp, 0);
	എങ്കിൽ (*tmp == 0) {
		എങ്കിൽ (err)
			*err = 0;
		തിരികെ ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	എങ്കിൽ (err)
		*err = 1;
	അല്ലെങ്കിൽ
		exit(1);
	തിരികെ 0;
}


പൂർണ്ണസംഖ്യ check_fs_open(സ്ഥിരം അക്ഷരം *name)
{
	എങ്കിൽ (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		തിരികെ 1;
	}
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം setup_filesystem(സ്ഥിരം അക്ഷരം *name,
			     unsigned പൂർണ്ണസംഖ്യ blocks, unsigned പൂർണ്ണസംഖ്യ inodes,
			     unsigned പൂർണ്ണസംഖ്യ type, പൂർണ്ണസംഖ്യ flags)
{
	ഘടന ext2_super_block param;
	errcode_t retval;

	memset(&param, 0, വലുപ്പം(param));
	ext2fs_blocks_എണ്ണം_set(&param, blocks);
	param.s_inodes_എണ്ണം = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	എങ്കിൽ (retval) {
		com_err(name, retval, "while initializing filesystem");
		തിരികെ;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_block_bitmap(test_fs->block_map);
	test_fs->block_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_block_bitmap(test_fs, "block bitmap",
					      &test_fs->block_map);
	എങ്കിൽ (retval) {
		com_err(name, retval, "while allocating block bitmap");
		പോകുക errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	എങ്കിൽ (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		പോകുക errout;
	}
	തിരികെ;

errout:
	ext2fs_close_free(&test_fs);
}

ശൂന്യം setup_cmd(പൂർണ്ണസംഖ്യ argc, ss_argv_t argv,
	       പൂർണ്ണസംഖ്യ sci_idx EXT2FS_ATTR((unused)),
	       ശൂന്യം *infop EXT2FS_ATTR((unused)))
{
	പൂർണ്ണസംഖ്യ		c, err;
	unsigned പൂർണ്ണസംഖ്യ	blocks = 128;
	unsigned പൂർണ്ണസംഖ്യ	inodes = 0;
	unsigned പൂർണ്ണസംഖ്യ	type = EXT2FS_BMAP64_BITARRAY;
	പൂർണ്ണസംഖ്യ		flags = EXT2_FLAG_64BITS;

	എങ്കിൽ (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	ആകുമ്പോൾ ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		തിരഞ്ഞെടുപ്പ് (c) {
		അവസ്ഥ 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			എങ്കിൽ (err)
				തിരികെ;
			നിർത്തുക;
		അവസ്ഥ 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			എങ്കിൽ (err)
				തിരികെ;
			നിർത്തുക;
		അവസ്ഥ 'l':	/* Legacy bitmaps */
			flags = 0;
			നിർത്തുക;
		അവസ്ഥ 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			എങ്കിൽ (err)
				തിരികെ;
			നിർത്തുക;
		സ്ഥിരസ്ഥിതി:
			fprintf(stderr, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			തിരികെ;
		}
	}
	setup_filesystem(argv[0], blocks, inodes, type, flags);
}

ശൂന്യം close_cmd(പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv, പൂർണ്ണസംഖ്യ sci_idx EXT2FS_ATTR((unused)),
	       ശൂന്യം *infop EXT2FS_ATTR((unused)))
{
	എങ്കിൽ (check_fs_open(argv[0]))
		തിരികെ;

	ext2fs_close_free(&test_fs);
}


ശൂന്യം dump_bitmap(ext2fs_generic_bitmap bmap, unsigned പൂർണ്ണസംഖ്യ start, unsigned അക്കം)
{
	unsigned അക്ഷരം	*buf;
	errcode_t	retval;
	പൂർണ്ണസംഖ്യ		i, len = (അക്കം - start + 7) / 8;

	buf = malloc(len);
	എങ്കിൽ (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		തിരികെ;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) start, അക്കം, buf);
	എങ്കിൽ (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		സ്വതന്ത്രം(buf);
		തിരികെ;
	}
	വേണ്ടി (i=0; i < len; i++)
		printf("%02x", buf[i]);
	printf("\n");
	printf("bits set: %u\n", ext2fs_bitcount(buf, len));
	സ്വതന്ത്രം(buf);
}

ശൂന്യം dump_inode_bitmap_cmd(പൂർണ്ണസംഖ്യ argc, ss_argv_t argv,
			   പൂർണ്ണസംഖ്യ sci_idx EXT2FS_ATTR((unused)),
			   ശൂന്യം *infop EXT2FS_ATTR((unused)))
{
	എങ്കിൽ (check_fs_open(argv[0]))
		തിരികെ;

	printf("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_എണ്ണം);
}
	
ശൂന്യം dump_block_bitmap_cmd(പൂർണ്ണസംഖ്യ argc, ss_argv_t argv,
			   പൂർണ്ണസംഖ്യ sci_idx EXT2FS_ATTR((unused)),
			   ശൂന്യം *infop EXT2FS_ATTR((unused)))
{
	എങ്കിൽ (check_fs_open(argv[0]))
		തിരികെ;

	printf("block bitmap: ");
	dump_bitmap(test_fs->block_map, test_fs->super->s_first_data_block,
		    test_fs->super->s_blocks_എണ്ണം);
}
	
ശൂന്യം do_setb(പൂർണ്ണസംഖ്യ argc, ss_argv_t argv,
	     പൂർണ്ണസംഖ്യ sci_idx EXT2FS_ATTR((unused)),
	     ശൂന്യം *infop EXT2FS_ATTR((unused)))
{
	unsigned പൂർണ്ണസംഖ്യ block, അക്കം;
	പൂർണ്ണസംഖ്യ err;
	പൂർണ്ണസംഖ്യ test_result, op_result;

	എങ്കിൽ (check_fs_open(argv[0]))
		തിരികെ;

	എങ്കിൽ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		തിരികെ;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	എങ്കിൽ (err)
		തിരികെ;

	എങ്കിൽ (argc == 3) {
		അക്കം = parse_ulong(argv[2], argv[0], "num", &err);
		എങ്കിൽ (err)
			തിരികെ;

		ext2fs_mark_block_bitmap_range2(test_fs->block_map,
						block, അക്കം);
		printf("Marking blocks %u to %u\n", block, block + അക്കം - 1);
		തിരികെ;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_mark_block_bitmap2(test_fs->block_map, block);
	printf("Setting block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	എങ്കിൽ (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

ശൂന്യം do_clearb(പൂർണ്ണസംഖ്യ argc, ss_argv_t argv,
	       പൂർണ്ണസംഖ്യ sci_idx EXT2FS_ATTR((unused)),
	       ശൂന്യം *infop EXT2FS_ATTR((unused)))
{
	unsigned പൂർണ്ണസംഖ്യ block, അക്കം;
	പൂർണ്ണസംഖ്യ err;
	പൂർണ്ണസംഖ്യ test_result, op_result;

	എങ്കിൽ (check_fs_open(argv[0]))
		തിരികെ;

	എങ്കിൽ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		തിരികെ;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	എങ്കിൽ (err)
		തിരികെ;

	എങ്കിൽ (argc == 3) {
		അക്കം = parse_ulong(argv[2], argv[0], "num", &err);
		എങ്കിൽ (err)
			തിരികെ;

		ext2fs_unmark_block_bitmap_range2(test_fs->block_map,
						block, അക്കം);
		printf("Clearing blocks %u to %u\n", block, block + അക്കം - 1);
		തിരികെ;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_unmark_block_bitmap2(test_fs->block_map, block);
	printf("Clearing block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	എങ്കിൽ (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

ശൂന്യം do_testb(പൂർണ്ണസംഖ്യ argc, ss_argv_t argv,
	      പൂർണ്ണസംഖ്യ sci_idx EXT2FS_ATTR((unused)),
	      ശൂന്യം *infop EXT2FS_ATTR((unused)))
{
	unsigned പൂർണ്ണസംഖ്യ block, അക്കം;
	പൂർണ്ണസംഖ്യ err;
	പൂർണ്ണസംഖ്യ test_result;

	എങ്കിൽ (check_fs_open(argv[0]))
		തിരികെ;

	എങ്കിൽ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		തിരികെ;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	എങ്കിൽ (err)
		തിരികെ;

	എങ്കിൽ (argc == 3) {
		അക്കം = parse_ulong(argv[2], argv[0], "num", &err);
		എങ്കിൽ (err)
			തിരികെ;

		test_result =
			ext2fs_test_block_bitmap_range2(test_fs->block_map,
							block, അക്കം);
		printf("Blocks %u to %u are %sall clear.\n",
		       block, block + അക്കം - 1, test_result ? "" : "NOT ");
		തിരികെ;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	printf("Block %u is %s\n", block, test_result ? "set" : "clear");
}

ശൂന്യം do_ffzb(പൂർണ്ണസംഖ്യ argc, ss_argv_t argv,
	     പൂർണ്ണസംഖ്യ sci_idx EXT2FS_ATTR((unused)),
	     ശൂന്യം *infop EXT2FS_ATTR((unused)))
{
	unsigned പൂർണ്ണസംഖ്യ start, end;
	പൂർണ്ണസംഖ്യ err;
	errcode_t retval;
	blk64_t out;

	എങ്കിൽ (check_fs_open(argv[0]))
		തിരികെ;

	എങ്കിൽ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		തിരികെ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	എങ്കിൽ (err)
		തിരികെ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	എങ്കിൽ (err)
		തിരികെ;

	retval = ext2fs_find_first_zero_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	എങ്കിൽ (retval) {
		printf("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_message(retval));
		തിരികെ;
	}
	printf("First unmarked block is %llu\n", (unsigned long long) out);
}

ശൂന്യം do_ffsb(പൂർണ്ണസംഖ്യ argc, ss_argv_t argv,
	     പൂർണ്ണസംഖ്യ sci_idx EXT2FS_ATTR((unused)),
	     ശൂന്യം *infop EXT2FS_ATTR((unused)))
{
	unsigned പൂർണ്ണസംഖ്യ start, end;
	പൂർണ്ണസംഖ്യ err;
	errcode_t retval;
	blk64_t out;

	എങ്കിൽ (check_fs_open(argv[0]))
		തിരികെ;

	എങ്കിൽ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		തിരികെ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	എങ്കിൽ (err)
		തിരികെ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	എങ്കിൽ (err)
		തിരികെ;

	retval = ext2fs_find_first_set_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	എങ്കിൽ (retval) {
		printf("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_message(retval));
		തിരികെ;
	}
	printf("First marked block is %llu\n", (unsigned long long) out);
}


ശൂന്യം do_zerob(പൂർണ്ണസംഖ്യ argc, ss_argv_t argv,
	      പൂർണ്ണസംഖ്യ sci_idx EXT2FS_ATTR((unused)),
	      ശൂന്യം *infop EXT2FS_ATTR((unused)))
{
	എങ്കിൽ (check_fs_open(argv[0]))
		തിരികെ;

	printf("Clearing block bitmap.\n");
	ext2fs_clear_block_bitmap(test_fs->block_map);
}

ശൂന്യം do_seti(പൂർണ്ണസംഖ്യ argc, ss_argv_t argv,
	     പൂർണ്ണസംഖ്യ sci_idx EXT2FS_ATTR((unused)),
	     ശൂന്യം *infop EXT2FS_ATTR((unused)))
{
	unsigned പൂർണ്ണസംഖ്യ inode;
	പൂർണ്ണസംഖ്യ err;
	പൂർണ്ണസംഖ്യ test_result, op_result;

	എങ്കിൽ (check_fs_open(argv[0]))
		തിരികെ;

	എങ്കിൽ (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		തിരികെ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	എങ്കിൽ (err)
		തിരികെ;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Setting inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	എങ്കിൽ (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

ശൂന്യം do_cleari(പൂർണ്ണസംഖ്യ argc, ss_argv_t argv,
	       പൂർണ്ണസംഖ്യ sci_idx EXT2FS_ATTR((unused)),
	       ശൂന്യം *infop EXT2FS_ATTR((unused)))
{
	unsigned പൂർണ്ണസംഖ്യ inode;
	പൂർണ്ണസംഖ്യ err;
	പൂർണ്ണസംഖ്യ test_result, op_result;

	എങ്കിൽ (check_fs_open(argv[0]))
		തിരികെ;

	എങ്കിൽ (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		തിരികെ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	എങ്കിൽ (err)
		തിരികെ;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Clearing inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	എങ്കിൽ (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

ശൂന്യം do_testi(പൂർണ്ണസംഖ്യ argc, ss_argv_t argv,
	      പൂർണ്ണസംഖ്യ sci_idx EXT2FS_ATTR((unused)),
	      ശൂന്യം *infop EXT2FS_ATTR((unused)))
{
	unsigned പൂർണ്ണസംഖ്യ inode;
	പൂർണ്ണസംഖ്യ err;
	പൂർണ്ണസംഖ്യ test_result;

	എങ്കിൽ (check_fs_open(argv[0]))
		തിരികെ;

	എങ്കിൽ (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		തിരികെ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	എങ്കിൽ (err)
		തിരികെ;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	printf("Inode %u is %s\n", inode, test_result ? "set" : "clear");
}

ശൂന്യം do_ffzi(പൂർണ്ണസംഖ്യ argc, ss_argv_t argv,
	     പൂർണ്ണസംഖ്യ sci_idx EXT2FS_ATTR((unused)),
	     ശൂന്യം *infop EXT2FS_ATTR((unused)))
{
	unsigned പൂർണ്ണസംഖ്യ start, end;
	പൂർണ്ണസംഖ്യ err;
	errcode_t retval;
	ext2_ino_t out;

	എങ്കിൽ (check_fs_open(argv[0]))
		തിരികെ;

	എങ്കിൽ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		തിരികെ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	എങ്കിൽ (err)
		തിരികെ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	എങ്കിൽ (err)
		തിരികെ;

	retval = ext2fs_find_first_zero_inode_bitmap2(test_fs->inode_map,
						      start, end, &out);
	എങ്കിൽ (retval) {
		printf("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_message(retval));
		തിരികെ;
	}
	printf("First unmarked inode is %u\n", out);
}

ശൂന്യം do_ffsi(പൂർണ്ണസംഖ്യ argc, ss_argv_t argv,
	     പൂർണ്ണസംഖ്യ sci_idx EXT2FS_ATTR((unused)),
	     ശൂന്യം *infop EXT2FS_ATTR((unused)))
{
	unsigned പൂർണ്ണസംഖ്യ start, end;
	പൂർണ്ണസംഖ്യ err;
	errcode_t retval;
	ext2_ino_t out;

	എങ്കിൽ (check_fs_open(argv[0]))
		തിരികെ;

	എങ്കിൽ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		തിരികെ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	എങ്കിൽ (err)
		തിരികെ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	എങ്കിൽ (err)
		തിരികെ;

	retval = ext2fs_find_first_set_inode_bitmap2(test_fs->inode_map,
						     start, end, &out);
	എങ്കിൽ (retval) {
		printf("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_message(retval));
		തിരികെ;
	}
	printf("First marked inode is %u\n", out);
}

ശൂന്യം do_zeroi(പൂർണ്ണസംഖ്യ argc, ss_argv_t argv,
	      പൂർണ്ണസംഖ്യ sci_idx EXT2FS_ATTR((unused)),
	      ശൂന്യം *infop EXT2FS_ATTR((unused)))
{
	എങ്കിൽ (check_fs_open(argv[0]))
		തിരികെ;

	printf("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

പൂർണ്ണസംഖ്യ പ്രധാന(പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
	unsigned പൂർണ്ണസംഖ്യ	blocks = 128;
	unsigned പൂർണ്ണസംഖ്യ	inodes = 0;
	unsigned പൂർണ്ണസംഖ്യ	type = EXT2FS_BMAP64_BITARRAY;
	പൂർണ്ണസംഖ്യ		c, err, code;
	അക്ഷരം		*request = (അക്ഷരം *)ശൂന്യ;
	അക്ഷരം		*cmd_file = 0;
	പൂർണ്ണസംഖ്യ		sci_idx;
	പൂർണ്ണസംഖ്യ		flags = EXT2_FLAG_64BITS;

	add_error_table(&et_ss_error_table);
	add_error_table(&et_ext2_error_table);
	ആകുമ്പോൾ ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		തിരഞ്ഞെടുപ്പ് (c) {
		അവസ്ഥ 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			എങ്കിൽ (err)
				exit(1);
			നിർത്തുക;
		അവസ്ഥ 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			എങ്കിൽ (err)
				exit(1);
			നിർത്തുക;
		അവസ്ഥ 'l':	/* Legacy bitmaps */
			flags = 0;
			നിർത്തുക;
		അവസ്ഥ 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			എങ്കിൽ (err)
				exit(1);
			നിർത്തുക;
		അവസ്ഥ 'R':
			request = optarg;
			നിർത്തുക;
		അവസ്ഥ 'f':
			cmd_file = optarg;
			നിർത്തുക;
		സ്ഥിരസ്ഥിതി:
			com_err(argv[0], 0, "Usage: %s [-R request] "
				"[-f cmd_file]", subsystem_name);
			exit(1);
		}
	}

	sci_idx = ss_create_invocation(subsystem_name, version,
				       (അക്ഷരം *)ശൂന്യ, &tst_bitmaps_cmds, &code);
	എങ്കിൽ (code) {
		ss_perror(sci_idx, code, "creating invocation");
		exit(1);
	}

	(ശൂന്യം) ss_add_request_table (sci_idx, &ss_std_requests, 1, &code);
	എങ്കിൽ (code) {
		ss_perror(sci_idx, code, "adding standard requests");
		exit (1);
	}

	printf("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, version);

	setup_filesystem(argv[0], blocks, inodes, type, flags);

	എങ്കിൽ (request) {
		code = ss_execute_വരി(sci_idx, request);
		എങ്കിൽ (code) {
			ss_perror(sci_idx, code, request);
			exit_status++;
		}
	} അല്ലെങ്കിൽ എങ്കിൽ (cmd_file) {
		exit_status = source_file(cmd_file, sci_idx);
	} അല്ലെങ്കിൽ {
		ss_listen(sci_idx);
	}

	exit(exit_status);
}

