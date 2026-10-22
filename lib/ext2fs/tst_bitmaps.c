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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ifdef HAVE_GETOPT_H
#ಅನ್ತರ್ಭಾವ <getopt.h>
#endif
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ "ss/ss.h"

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ಬಾಹ್ಯ ss_request_table tst_bitmaps_cmds;

ಸ್ಥಿರ ವರ್ಣ subsystem_name[] = "tst_bitmaps";
ಸ್ಥಿರ ವರ್ಣ version[] = "1.0";

ext2_filsys	test_fs;
ಪೂರ್ಣಾಙ್ಕ		exit_status = 0;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ source_file(ಧ್ರುವ ವರ್ಣ *cmd_file, ಪೂರ್ಣಾಙ್ಕ sci_idx)
{
	FILE		*f;
	ವರ್ಣ		buf[256];
	ವರ್ಣ		*cp;
	ಪೂರ್ಣಾಙ್ಕ		retval;
	ಪೂರ್ಣಾಙ್ಕ 		noecho;

	ಯದಿ (strcmp(cmd_file, "-") == 0)
		f = stdin;
	ಅನ್ಯಥಾ {
		f = fopen(cmd_file, "r");
		ಯದಿ (!f) {
			perror(cmd_file);
			exit(1);
		}
	}
	fflush(stdout);
	fflush(stderr);
	setbuf(stdout, ರಿಕ್ತ);
	setbuf(stderr, ರಿಕ್ತ);
	ಯಾವತ್ (!feof(f)) {
		ಯದಿ (fgets(buf, ಪರಿಮಾಣ(buf), f) == ರಿಕ್ತ)
			ಭಙ್ಗ;
		ಯದಿ (buf[0] == '#')
			ಅನುವರ್ತ;
		noecho = 0;
		ಯದಿ (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = strchr(buf, '\n');
		ಯದಿ (cp)
			*cp = 0;
		cp = strchr(buf, '\r');
		ಯದಿ (cp)
			*cp = 0;
		ಯದಿ (!noecho)
			printf("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_ಸಾಲು(sci_idx, buf);
		ಯದಿ (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_status++;
		}
	}
	ಪ್ರತ್ಯಾಗಮ exit_status;
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
ಶೂನ್ಯ reset_getopt(ಶೂನ್ಯ)
{
#ಯದಿ defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#ಅನ್ಯಥಾ
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
unsigned long parse_ulong(ಧ್ರುವ ವರ್ಣ *str, ಧ್ರುವ ವರ್ಣ *cmd,
			  ಧ್ರುವ ವರ್ಣ *descr, ಪೂರ್ಣಾಙ್ಕ *err)
{
	ವರ್ಣ		*tmp;
	unsigned long	ret;

	ret = strtoul(str, &tmp, 0);
	ಯದಿ (*tmp == 0) {
		ಯದಿ (err)
			*err = 0;
		ಪ್ರತ್ಯಾಗಮ ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	ಯದಿ (err)
		*err = 1;
	ಅನ್ಯಥಾ
		exit(1);
	ಪ್ರತ್ಯಾಗಮ 0;
}


ಪೂರ್ಣಾಙ್ಕ check_fs_open(ಧ್ರುವ ವರ್ಣ *name)
{
	ಯದಿ (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		ಪ್ರತ್ಯಾಗಮ 1;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ setup_filesystem(ಧ್ರುವ ವರ್ಣ *name,
			     unsigned ಪೂರ್ಣಾಙ್ಕ blocks, unsigned ಪೂರ್ಣಾಙ್ಕ inodes,
			     unsigned ಪೂರ್ಣಾಙ್ಕ type, ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ ext2_super_block param;
	errcode_t retval;

	memset(&param, 0, ಪರಿಮಾಣ(param));
	ext2fs_blocks_ಎಣಿಕೆ_set(&param, blocks);
	param.s_inodes_ಎಣಿಕೆ = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	ಯದಿ (retval) {
		com_err(name, retval, "while initializing filesystem");
		ಪ್ರತ್ಯಾಗಮ;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_block_bitmap(test_fs->block_map);
	test_fs->block_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_block_bitmap(test_fs, "block bitmap",
					      &test_fs->block_map);
	ಯದಿ (retval) {
		com_err(name, retval, "while allocating block bitmap");
		ಗಚ್ಛ errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	ಯದಿ (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		ಗಚ್ಛ errout;
	}
	ಪ್ರತ್ಯಾಗಮ;

errout:
	ext2fs_close_free(&test_fs);
}

ಶೂನ್ಯ setup_cmd(ಪೂರ್ಣಾಙ್ಕ argc, ss_argv_t argv,
	       ಪೂರ್ಣಾಙ್ಕ sci_idx EXT2FS_ATTR((unused)),
	       ಶೂನ್ಯ *infop EXT2FS_ATTR((unused)))
{
	ಪೂರ್ಣಾಙ್ಕ		c, err;
	unsigned ಪೂರ್ಣಾಙ್ಕ	blocks = 128;
	unsigned ಪೂರ್ಣಾಙ್ಕ	inodes = 0;
	unsigned ಪೂರ್ಣಾಙ್ಕ	type = EXT2FS_BMAP64_BITARRAY;
	ಪೂರ್ಣಾಙ್ಕ		flags = EXT2_FLAG_64BITS;

	ಯದಿ (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	ಯಾವತ್ ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		ವಿಕಲ್ಪ (c) {
		ಪಕ್ಷ 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ಯದಿ (err)
				ಪ್ರತ್ಯಾಗಮ;
			ಭಙ್ಗ;
		ಪಕ್ಷ 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ಯದಿ (err)
				ಪ್ರತ್ಯಾಗಮ;
			ಭಙ್ಗ;
		ಪಕ್ಷ 'l':	/* Legacy bitmaps */
			flags = 0;
			ಭಙ್ಗ;
		ಪಕ್ಷ 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			ಯದಿ (err)
				ಪ್ರತ್ಯಾಗಮ;
			ಭಙ್ಗ;
		ಸ್ವಭಾವ:
			fprintf(stderr, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			ಪ್ರತ್ಯಾಗಮ;
		}
	}
	setup_filesystem(argv[0], blocks, inodes, type, flags);
}

ಶೂನ್ಯ close_cmd(ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv, ಪೂರ್ಣಾಙ್ಕ sci_idx EXT2FS_ATTR((unused)),
	       ಶೂನ್ಯ *infop EXT2FS_ATTR((unused)))
{
	ಯದಿ (check_fs_open(argv[0]))
		ಪ್ರತ್ಯಾಗಮ;

	ext2fs_close_free(&test_fs);
}


ಶೂನ್ಯ dump_bitmap(ext2fs_generic_bitmap bmap, unsigned ಪೂರ್ಣಾಙ್ಕ start, unsigned ಅಂಕೆ)
{
	unsigned ವರ್ಣ	*buf;
	errcode_t	retval;
	ಪೂರ್ಣಾಙ್ಕ		i, len = (ಅಂಕೆ - start + 7) / 8;

	buf = malloc(len);
	ಯದಿ (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		ಪ್ರತ್ಯಾಗಮ;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) start, ಅಂಕೆ, buf);
	ಯದಿ (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		ಮುಕ್ತ(buf);
		ಪ್ರತ್ಯಾಗಮ;
	}
	ಪ್ರತಿ (i=0; i < len; i++)
		printf("%02x", buf[i]);
	printf("\n");
	printf("bits set: %u\n", ext2fs_bitcount(buf, len));
	ಮುಕ್ತ(buf);
}

ಶೂನ್ಯ dump_inode_bitmap_cmd(ಪೂರ್ಣಾಙ್ಕ argc, ss_argv_t argv,
			   ಪೂರ್ಣಾಙ್ಕ sci_idx EXT2FS_ATTR((unused)),
			   ಶೂನ್ಯ *infop EXT2FS_ATTR((unused)))
{
	ಯದಿ (check_fs_open(argv[0]))
		ಪ್ರತ್ಯಾಗಮ;

	printf("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_ಎಣಿಕೆ);
}
	
ಶೂನ್ಯ dump_block_bitmap_cmd(ಪೂರ್ಣಾಙ್ಕ argc, ss_argv_t argv,
			   ಪೂರ್ಣಾಙ್ಕ sci_idx EXT2FS_ATTR((unused)),
			   ಶೂನ್ಯ *infop EXT2FS_ATTR((unused)))
{
	ಯದಿ (check_fs_open(argv[0]))
		ಪ್ರತ್ಯಾಗಮ;

	printf("block bitmap: ");
	dump_bitmap(test_fs->block_map, test_fs->super->s_first_data_block,
		    test_fs->super->s_blocks_ಎಣಿಕೆ);
}
	
ಶೂನ್ಯ do_setb(ಪೂರ್ಣಾಙ್ಕ argc, ss_argv_t argv,
	     ಪೂರ್ಣಾಙ್ಕ sci_idx EXT2FS_ATTR((unused)),
	     ಶೂನ್ಯ *infop EXT2FS_ATTR((unused)))
{
	unsigned ಪೂರ್ಣಾಙ್ಕ block, ಅಂಕೆ;
	ಪೂರ್ಣಾಙ್ಕ err;
	ಪೂರ್ಣಾಙ್ಕ test_result, op_result;

	ಯದಿ (check_fs_open(argv[0]))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		ಪ್ರತ್ಯಾಗಮ;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (argc == 3) {
		ಅಂಕೆ = parse_ulong(argv[2], argv[0], "num", &err);
		ಯದಿ (err)
			ಪ್ರತ್ಯಾಗಮ;

		ext2fs_mark_block_bitmap_range2(test_fs->block_map,
						block, ಅಂಕೆ);
		printf("Marking blocks %u to %u\n", block, block + ಅಂಕೆ - 1);
		ಪ್ರತ್ಯಾಗಮ;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_mark_block_bitmap2(test_fs->block_map, block);
	printf("Setting block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	ಯದಿ (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

ಶೂನ್ಯ do_clearb(ಪೂರ್ಣಾಙ್ಕ argc, ss_argv_t argv,
	       ಪೂರ್ಣಾಙ್ಕ sci_idx EXT2FS_ATTR((unused)),
	       ಶೂನ್ಯ *infop EXT2FS_ATTR((unused)))
{
	unsigned ಪೂರ್ಣಾಙ್ಕ block, ಅಂಕೆ;
	ಪೂರ್ಣಾಙ್ಕ err;
	ಪೂರ್ಣಾಙ್ಕ test_result, op_result;

	ಯದಿ (check_fs_open(argv[0]))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		ಪ್ರತ್ಯಾಗಮ;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (argc == 3) {
		ಅಂಕೆ = parse_ulong(argv[2], argv[0], "num", &err);
		ಯದಿ (err)
			ಪ್ರತ್ಯಾಗಮ;

		ext2fs_unmark_block_bitmap_range2(test_fs->block_map,
						block, ಅಂಕೆ);
		printf("Clearing blocks %u to %u\n", block, block + ಅಂಕೆ - 1);
		ಪ್ರತ್ಯಾಗಮ;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_unmark_block_bitmap2(test_fs->block_map, block);
	printf("Clearing block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	ಯದಿ (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

ಶೂನ್ಯ do_testb(ಪೂರ್ಣಾಙ್ಕ argc, ss_argv_t argv,
	      ಪೂರ್ಣಾಙ್ಕ sci_idx EXT2FS_ATTR((unused)),
	      ಶೂನ್ಯ *infop EXT2FS_ATTR((unused)))
{
	unsigned ಪೂರ್ಣಾಙ್ಕ block, ಅಂಕೆ;
	ಪೂರ್ಣಾಙ್ಕ err;
	ಪೂರ್ಣಾಙ್ಕ test_result;

	ಯದಿ (check_fs_open(argv[0]))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		ಪ್ರತ್ಯಾಗಮ;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (argc == 3) {
		ಅಂಕೆ = parse_ulong(argv[2], argv[0], "num", &err);
		ಯದಿ (err)
			ಪ್ರತ್ಯಾಗಮ;

		test_result =
			ext2fs_test_block_bitmap_range2(test_fs->block_map,
							block, ಅಂಕೆ);
		printf("Blocks %u to %u are %sall clear.\n",
		       block, block + ಅಂಕೆ - 1, test_result ? "" : "NOT ");
		ಪ್ರತ್ಯಾಗಮ;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	printf("Block %u is %s\n", block, test_result ? "set" : "clear");
}

ಶೂನ್ಯ do_ffzb(ಪೂರ್ಣಾಙ್ಕ argc, ss_argv_t argv,
	     ಪೂರ್ಣಾಙ್ಕ sci_idx EXT2FS_ATTR((unused)),
	     ಶೂನ್ಯ *infop EXT2FS_ATTR((unused)))
{
	unsigned ಪೂರ್ಣಾಙ್ಕ start, end;
	ಪೂರ್ಣಾಙ್ಕ err;
	errcode_t retval;
	blk64_t out;

	ಯದಿ (check_fs_open(argv[0]))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		ಪ್ರತ್ಯಾಗಮ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ;

	retval = ext2fs_find_first_zero_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	ಯದಿ (retval) {
		printf("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_message(retval));
		ಪ್ರತ್ಯಾಗಮ;
	}
	printf("First unmarked block is %llu\n", (unsigned long long) out);
}

ಶೂನ್ಯ do_ffsb(ಪೂರ್ಣಾಙ್ಕ argc, ss_argv_t argv,
	     ಪೂರ್ಣಾಙ್ಕ sci_idx EXT2FS_ATTR((unused)),
	     ಶೂನ್ಯ *infop EXT2FS_ATTR((unused)))
{
	unsigned ಪೂರ್ಣಾಙ್ಕ start, end;
	ಪೂರ್ಣಾಙ್ಕ err;
	errcode_t retval;
	blk64_t out;

	ಯದಿ (check_fs_open(argv[0]))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		ಪ್ರತ್ಯಾಗಮ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ;

	retval = ext2fs_find_first_set_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	ಯದಿ (retval) {
		printf("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_message(retval));
		ಪ್ರತ್ಯಾಗಮ;
	}
	printf("First marked block is %llu\n", (unsigned long long) out);
}


ಶೂನ್ಯ do_zerob(ಪೂರ್ಣಾಙ್ಕ argc, ss_argv_t argv,
	      ಪೂರ್ಣಾಙ್ಕ sci_idx EXT2FS_ATTR((unused)),
	      ಶೂನ್ಯ *infop EXT2FS_ATTR((unused)))
{
	ಯದಿ (check_fs_open(argv[0]))
		ಪ್ರತ್ಯಾಗಮ;

	printf("Clearing block bitmap.\n");
	ext2fs_clear_block_bitmap(test_fs->block_map);
}

ಶೂನ್ಯ do_seti(ಪೂರ್ಣಾಙ್ಕ argc, ss_argv_t argv,
	     ಪೂರ್ಣಾಙ್ಕ sci_idx EXT2FS_ATTR((unused)),
	     ಶೂನ್ಯ *infop EXT2FS_ATTR((unused)))
{
	unsigned ಪೂರ್ಣಾಙ್ಕ inode;
	ಪೂರ್ಣಾಙ್ಕ err;
	ಪೂರ್ಣಾಙ್ಕ test_result, op_result;

	ಯದಿ (check_fs_open(argv[0]))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		ಪ್ರತ್ಯಾಗಮ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Setting inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	ಯದಿ (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

ಶೂನ್ಯ do_cleari(ಪೂರ್ಣಾಙ್ಕ argc, ss_argv_t argv,
	       ಪೂರ್ಣಾಙ್ಕ sci_idx EXT2FS_ATTR((unused)),
	       ಶೂನ್ಯ *infop EXT2FS_ATTR((unused)))
{
	unsigned ಪೂರ್ಣಾಙ್ಕ inode;
	ಪೂರ್ಣಾಙ್ಕ err;
	ಪೂರ್ಣಾಙ್ಕ test_result, op_result;

	ಯದಿ (check_fs_open(argv[0]))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		ಪ್ರತ್ಯಾಗಮ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Clearing inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	ಯದಿ (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

ಶೂನ್ಯ do_testi(ಪೂರ್ಣಾಙ್ಕ argc, ss_argv_t argv,
	      ಪೂರ್ಣಾಙ್ಕ sci_idx EXT2FS_ATTR((unused)),
	      ಶೂನ್ಯ *infop EXT2FS_ATTR((unused)))
{
	unsigned ಪೂರ್ಣಾಙ್ಕ inode;
	ಪೂರ್ಣಾಙ್ಕ err;
	ಪೂರ್ಣಾಙ್ಕ test_result;

	ಯದಿ (check_fs_open(argv[0]))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		ಪ್ರತ್ಯಾಗಮ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	printf("Inode %u is %s\n", inode, test_result ? "set" : "clear");
}

ಶೂನ್ಯ do_ffzi(ಪೂರ್ಣಾಙ್ಕ argc, ss_argv_t argv,
	     ಪೂರ್ಣಾಙ್ಕ sci_idx EXT2FS_ATTR((unused)),
	     ಶೂನ್ಯ *infop EXT2FS_ATTR((unused)))
{
	unsigned ಪೂರ್ಣಾಙ್ಕ start, end;
	ಪೂರ್ಣಾಙ್ಕ err;
	errcode_t retval;
	ext2_ino_t out;

	ಯದಿ (check_fs_open(argv[0]))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		ಪ್ರತ್ಯಾಗಮ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ;

	retval = ext2fs_find_first_zero_inode_bitmap2(test_fs->inode_map,
						      start, end, &out);
	ಯದಿ (retval) {
		printf("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_message(retval));
		ಪ್ರತ್ಯಾಗಮ;
	}
	printf("First unmarked inode is %u\n", out);
}

ಶೂನ್ಯ do_ffsi(ಪೂರ್ಣಾಙ್ಕ argc, ss_argv_t argv,
	     ಪೂರ್ಣಾಙ್ಕ sci_idx EXT2FS_ATTR((unused)),
	     ಶೂನ್ಯ *infop EXT2FS_ATTR((unused)))
{
	unsigned ಪೂರ್ಣಾಙ್ಕ start, end;
	ಪೂರ್ಣಾಙ್ಕ err;
	errcode_t retval;
	ext2_ino_t out;

	ಯದಿ (check_fs_open(argv[0]))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		ಪ್ರತ್ಯಾಗಮ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ;

	retval = ext2fs_find_first_set_inode_bitmap2(test_fs->inode_map,
						     start, end, &out);
	ಯದಿ (retval) {
		printf("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_message(retval));
		ಪ್ರತ್ಯಾಗಮ;
	}
	printf("First marked inode is %u\n", out);
}

ಶೂನ್ಯ do_zeroi(ಪೂರ್ಣಾಙ್ಕ argc, ss_argv_t argv,
	      ಪೂರ್ಣಾಙ್ಕ sci_idx EXT2FS_ATTR((unused)),
	      ಶೂನ್ಯ *infop EXT2FS_ATTR((unused)))
{
	ಯದಿ (check_fs_open(argv[0]))
		ಪ್ರತ್ಯಾಗಮ;

	printf("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

ಪೂರ್ಣಾಙ್ಕ main(ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ	blocks = 128;
	unsigned ಪೂರ್ಣಾಙ್ಕ	inodes = 0;
	unsigned ಪೂರ್ಣಾಙ್ಕ	type = EXT2FS_BMAP64_BITARRAY;
	ಪೂರ್ಣಾಙ್ಕ		c, err, code;
	ವರ್ಣ		*request = (ವರ್ಣ *)ರಿಕ್ತ;
	ವರ್ಣ		*cmd_file = 0;
	ಪೂರ್ಣಾಙ್ಕ		sci_idx;
	ಪೂರ್ಣಾಙ್ಕ		flags = EXT2_FLAG_64BITS;

	add_error_table(&et_ss_error_table);
	add_error_table(&et_ext2_error_table);
	ಯಾವತ್ ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		ವಿಕಲ್ಪ (c) {
		ಪಕ್ಷ 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ಯದಿ (err)
				exit(1);
			ಭಙ್ಗ;
		ಪಕ್ಷ 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ಯದಿ (err)
				exit(1);
			ಭಙ್ಗ;
		ಪಕ್ಷ 'l':	/* Legacy bitmaps */
			flags = 0;
			ಭಙ್ಗ;
		ಪಕ್ಷ 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			ಯದಿ (err)
				exit(1);
			ಭಙ್ಗ;
		ಪಕ್ಷ 'R':
			request = optarg;
			ಭಙ್ಗ;
		ಪಕ್ಷ 'f':
			cmd_file = optarg;
			ಭಙ್ಗ;
		ಸ್ವಭಾವ:
			com_err(argv[0], 0, "Usage: %s [-R request] "
				"[-f cmd_file]", subsystem_name);
			exit(1);
		}
	}

	sci_idx = ss_create_invocation(subsystem_name, version,
				       (ವರ್ಣ *)ರಿಕ್ತ, &tst_bitmaps_cmds, &code);
	ಯದಿ (code) {
		ss_perror(sci_idx, code, "creating invocation");
		exit(1);
	}

	(ಶೂನ್ಯ) ss_add_request_table (sci_idx, &ss_std_requests, 1, &code);
	ಯದಿ (code) {
		ss_perror(sci_idx, code, "adding standard requests");
		exit (1);
	}

	printf("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, version);

	setup_filesystem(argv[0], blocks, inodes, type, flags);

	ಯದಿ (request) {
		code = ss_execute_ಸಾಲು(sci_idx, request);
		ಯದಿ (code) {
			ss_perror(sci_idx, code, request);
			exit_status++;
		}
	} ಅನ್ಯಥಾ ಯದಿ (cmd_file) {
		exit_status = source_file(cmd_file, sci_idx);
	} ಅನ್ಯಥಾ {
		ss_listen(sci_idx);
	}

	exit(exit_status);
}

