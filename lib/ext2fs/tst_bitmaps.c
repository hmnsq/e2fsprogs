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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_GETOPT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "ss/ss.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

ਬਾਹਰੀ ss_request_ਸਾਰਣੀ tst_bitmaps_cmds;

ਸਥਿਰ ਅੱਖਰ subsystem_name[] = "tst_bitmaps";
ਸਥਿਰ ਅੱਖਰ ਸੰਸਕਰਣ[] = "1.0";

ext2_filsys	test_fs;
ਪੂਰਨ		exit_ਸਥਿਤੀ = 0;

ਸਥਿਰ ਪੂਰਨ ਸਰੋਤ_file(ਅਚਲ ਅੱਖਰ *cmd_file, ਪੂਰਨ sci_idx)
{
	FILE		*f;
	ਅੱਖਰ		buf[256];
	ਅੱਖਰ		*cp;
	ਪੂਰਨ		retval;
	ਪੂਰਨ 		noecho;

	ਜੇ (strcmp(cmd_file, "-") == 0)
		f = stdin;
	ਨਹੀਂਤਾਂ {
		f = fopen(cmd_file, "r");
		ਜੇ (!f) {
			perror(cmd_file);
			ਬਾਹਰ_ਨਿਕਲੋ(1);
		}
	}
	fflush(stdout);
	fflush(stderr);
	setbuf(stdout, ਨਿਰਾ);
	setbuf(stderr, ਨਿਰਾ);
	ਜਦੋਂ (!feof(f)) {
		ਜੇ (fgets(buf, ਆਕਾਰ(buf), f) == ਨਿਰਾ)
			ਰੋਕੋ;
		ਜੇ (buf[0] == '#')
			ਜਾਰੀ;
		noecho = 0;
		ਜੇ (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = strchr(buf, '\n');
		ਜੇ (cp)
			*cp = 0;
		cp = strchr(buf, '\r');
		ਜੇ (cp)
			*cp = 0;
		ਜੇ (!noecho)
			ਲਿਖੋ("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_ਸਤਰ(sci_idx, buf);
		ਜੇ (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_ਸਥਿਤੀ++;
		}
	}
	ਵਾਪਸ exit_ਸਥਿਤੀ;
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
ਖਾਲੀ reset_getopt(ਖਾਲੀ)
{
#ਜੇ defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#ਨਹੀਂਤਾਂ
	optind = 1;
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPTRESET
	optreset = 1;		/* Makes BSD getopt happy */
#ਜੇ_ਅੰਤ
}

/*
 * This function will convert a string to an unsigned long, printing
 * an error message if it fails, and returning success or failure in err.
 */
ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ parse_ulong(ਅਚਲ ਅੱਖਰ *str, ਅਚਲ ਅੱਖਰ *cmd,
			  ਅਚਲ ਅੱਖਰ *descr, ਪੂਰਨ *err)
{
	ਅੱਖਰ		*tmp;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ	ret;

	ret = strtoul(str, &tmp, 0);
	ਜੇ (*tmp == 0) {
		ਜੇ (err)
			*err = 0;
		ਵਾਪਸ ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	ਜੇ (err)
		*err = 1;
	ਨਹੀਂਤਾਂ
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	ਵਾਪਸ 0;
}


ਪੂਰਨ check_fs_open(ਅਚਲ ਅੱਖਰ *name)
{
	ਜੇ (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		ਵਾਪਸ 1;
	}
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ ਤਿਆਰੀ_filesystem(ਅਚਲ ਅੱਖਰ *name,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖੰਡਾਂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ inodes,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ type, ਪੂਰਨ flags)
{
	ਬਣਤਰ ext2_super_ਖੰਡ param;
	errcode_t retval;

	memset(&param, 0, ਆਕਾਰ(param));
	ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(&param, ਖੰਡਾਂ);
	param.s_inodes_ਗਿਣਤੀ = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	ਜੇ (retval) {
		com_err(name, retval, "while initializing filesystem");
		ਵਾਪਸ;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_ਖੰਡ_bitmap(test_fs->ਖੰਡ_map);
	test_fs->ਖੰਡ_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_ਖੰਡ_bitmap(test_fs, "block bitmap",
					      &test_fs->ਖੰਡ_map);
	ਜੇ (retval) {
		com_err(name, retval, "while allocating block bitmap");
		ਜਾਓ errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	ਜੇ (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		ਜਾਓ errout;
	}
	ਵਾਪਸ;

errout:
	ext2fs_close_free(&test_fs);
}

ਖਾਲੀ ਤਿਆਰੀ_cmd(ਪੂਰਨ argc, ss_argv_t argv,
	       ਪੂਰਨ sci_idx EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
	       ਖਾਲੀ *infop EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਪੂਰਨ		c, err;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	ਖੰਡਾਂ = 128;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	inodes = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	type = EXT2FS_BMAP64_BITARRAY;
	ਪੂਰਨ		flags = EXT2_FLAG_64_ਬਿੱਟ;

	ਜੇ (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	ਜਦੋਂ ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		ਚੋਣ (c) {
		ਹਾਲਤ 'b':
			ਖੰਡਾਂ = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ਜੇ (err)
				ਵਾਪਸ;
			ਰੋਕੋ;
		ਹਾਲਤ 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ਜੇ (err)
				ਵਾਪਸ;
			ਰੋਕੋ;
		ਹਾਲਤ 'l':	/* Legacy bitmaps */
			flags = 0;
			ਰੋਕੋ;
		ਹਾਲਤ 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			ਜੇ (err)
				ਵਾਪਸ;
			ਰੋਕੋ;
		ਮੂਲ:
			fprintf(stderr, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			ਵਾਪਸ;
		}
	}
	ਤਿਆਰੀ_filesystem(argv[0], ਖੰਡਾਂ, inodes, type, flags);
}

ਖਾਲੀ close_cmd(ਪੂਰਨ argc, ਅੱਖਰ **argv, ਪੂਰਨ sci_idx EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
	       ਖਾਲੀ *infop EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਜੇ (check_fs_open(argv[0]))
		ਵਾਪਸ;

	ext2fs_close_free(&test_fs);
}


ਖਾਲੀ dump_bitmap(ext2fs_generic_bitmap bmap, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	*buf;
	errcode_t	retval;
	ਪੂਰਨ		i, len = (ਅੰਕ - ਸ਼ੁਰੂ + 7) / 8;

	buf = ਸਮ੍ਰਿਤੀ_ਦਿਓ(len);
	ਜੇ (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		ਵਾਪਸ;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) ਸ਼ੁਰੂ, ਅੰਕ, buf);
	ਜੇ (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		ਮੁਕਤ(buf);
		ਵਾਪਸ;
	}
	ਲਈ (i=0; i < len; i++)
		ਲਿਖੋ("%02x", buf[i]);
	ਲਿਖੋ("\n");
	ਲਿਖੋ("bits set: %u\n", ext2fs_bitcount(buf, len));
	ਮੁਕਤ(buf);
}

ਖਾਲੀ dump_inode_bitmap_cmd(ਪੂਰਨ argc, ss_argv_t argv,
			   ਪੂਰਨ sci_idx EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			   ਖਾਲੀ *infop EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਜੇ (check_fs_open(argv[0]))
		ਵਾਪਸ;

	ਲਿਖੋ("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_ਗਿਣਤੀ);
}
	
ਖਾਲੀ dump_ਖੰਡ_bitmap_cmd(ਪੂਰਨ argc, ss_argv_t argv,
			   ਪੂਰਨ sci_idx EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			   ਖਾਲੀ *infop EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਜੇ (check_fs_open(argv[0]))
		ਵਾਪਸ;

	ਲਿਖੋ("block bitmap: ");
	dump_bitmap(test_fs->ਖੰਡ_map, test_fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ,
		    test_fs->super->s_ਖੰਡਾਂ_ਗਿਣਤੀ);
}
	
ਖਾਲੀ do_setb(ਪੂਰਨ argc, ss_argv_t argv,
	     ਪੂਰਨ sci_idx EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
	     ਖਾਲੀ *infop EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖੰਡ, ਅੰਕ;
	ਪੂਰਨ err;
	ਪੂਰਨ test_ਨਤੀਜਾ, op_ਨਤੀਜਾ;

	ਜੇ (check_fs_open(argv[0]))
		ਵਾਪਸ;

	ਜੇ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		ਵਾਪਸ;
	}

	ਖੰਡ = parse_ulong(argv[1], argv[0], "block", &err);
	ਜੇ (err)
		ਵਾਪਸ;

	ਜੇ (argc == 3) {
		ਅੰਕ = parse_ulong(argv[2], argv[0], "num", &err);
		ਜੇ (err)
			ਵਾਪਸ;

		ext2fs_mark_ਖੰਡ_bitmap_range2(test_fs->ਖੰਡ_map,
						ਖੰਡ, ਅੰਕ);
		ਲਿਖੋ("Marking blocks %u to %u\n", ਖੰਡ, ਖੰਡ + ਅੰਕ - 1);
		ਵਾਪਸ;
	}

	test_ਨਤੀਜਾ = ext2fs_test_ਖੰਡ_bitmap2(test_fs->ਖੰਡ_map, ਖੰਡ);
	op_ਨਤੀਜਾ = ext2fs_mark_ਖੰਡ_bitmap2(test_fs->ਖੰਡ_map, ਖੰਡ);
	ਲਿਖੋ("Setting block %u, was %s before\n", ਖੰਡ, op_ਨਤੀਜਾ ?
	       "set" : "clear");
	ਜੇ (!test_ਨਤੀਜਾ != !op_ਨਤੀਜਾ)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_ਨਤੀਜਾ, op_ਨਤੀਜਾ);
}

ਖਾਲੀ do_clearb(ਪੂਰਨ argc, ss_argv_t argv,
	       ਪੂਰਨ sci_idx EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
	       ਖਾਲੀ *infop EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖੰਡ, ਅੰਕ;
	ਪੂਰਨ err;
	ਪੂਰਨ test_ਨਤੀਜਾ, op_ਨਤੀਜਾ;

	ਜੇ (check_fs_open(argv[0]))
		ਵਾਪਸ;

	ਜੇ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		ਵਾਪਸ;
	}

	ਖੰਡ = parse_ulong(argv[1], argv[0], "block", &err);
	ਜੇ (err)
		ਵਾਪਸ;

	ਜੇ (argc == 3) {
		ਅੰਕ = parse_ulong(argv[2], argv[0], "num", &err);
		ਜੇ (err)
			ਵਾਪਸ;

		ext2fs_unmark_ਖੰਡ_bitmap_range2(test_fs->ਖੰਡ_map,
						ਖੰਡ, ਅੰਕ);
		ਲਿਖੋ("Clearing blocks %u to %u\n", ਖੰਡ, ਖੰਡ + ਅੰਕ - 1);
		ਵਾਪਸ;
	}

	test_ਨਤੀਜਾ = ext2fs_test_ਖੰਡ_bitmap2(test_fs->ਖੰਡ_map, ਖੰਡ);
	op_ਨਤੀਜਾ = ext2fs_unmark_ਖੰਡ_bitmap2(test_fs->ਖੰਡ_map, ਖੰਡ);
	ਲਿਖੋ("Clearing block %u, was %s before\n", ਖੰਡ, op_ਨਤੀਜਾ ?
	       "set" : "clear");
	ਜੇ (!test_ਨਤੀਜਾ != !op_ਨਤੀਜਾ)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_ਨਤੀਜਾ, op_ਨਤੀਜਾ);
}

ਖਾਲੀ do_testb(ਪੂਰਨ argc, ss_argv_t argv,
	      ਪੂਰਨ sci_idx EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
	      ਖਾਲੀ *infop EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਖੰਡ, ਅੰਕ;
	ਪੂਰਨ err;
	ਪੂਰਨ test_ਨਤੀਜਾ;

	ਜੇ (check_fs_open(argv[0]))
		ਵਾਪਸ;

	ਜੇ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		ਵਾਪਸ;
	}

	ਖੰਡ = parse_ulong(argv[1], argv[0], "block", &err);
	ਜੇ (err)
		ਵਾਪਸ;

	ਜੇ (argc == 3) {
		ਅੰਕ = parse_ulong(argv[2], argv[0], "num", &err);
		ਜੇ (err)
			ਵਾਪਸ;

		test_ਨਤੀਜਾ =
			ext2fs_test_ਖੰਡ_bitmap_range2(test_fs->ਖੰਡ_map,
							ਖੰਡ, ਅੰਕ);
		ਲਿਖੋ("Blocks %u to %u are %sall clear.\n",
		       ਖੰਡ, ਖੰਡ + ਅੰਕ - 1, test_ਨਤੀਜਾ ? "" : "NOT ");
		ਵਾਪਸ;
	}

	test_ਨਤੀਜਾ = ext2fs_test_ਖੰਡ_bitmap2(test_fs->ਖੰਡ_map, ਖੰਡ);
	ਲਿਖੋ("Block %u is %s\n", ਖੰਡ, test_ਨਤੀਜਾ ? "set" : "clear");
}

ਖਾਲੀ do_ffzb(ਪੂਰਨ argc, ss_argv_t argv,
	     ਪੂਰਨ sci_idx EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
	     ਖਾਲੀ *infop EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਸਿਰਾ;
	ਪੂਰਨ err;
	errcode_t retval;
	blk64_t ਬਾਹਰਲਾ;

	ਜੇ (check_fs_open(argv[0]))
		ਵਾਪਸ;

	ਜੇ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		ਵਾਪਸ;
	}

	ਸ਼ੁਰੂ = parse_ulong(argv[1], argv[0], "start", &err);
	ਜੇ (err)
		ਵਾਪਸ;

	ਸਿਰਾ = parse_ulong(argv[2], argv[0], "end", &err);
	ਜੇ (err)
		ਵਾਪਸ;

	retval = ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_zero_ਖੰਡ_bitmap2(test_fs->ਖੰਡ_map,
						      ਸ਼ੁਰੂ, ਸਿਰਾ, &ਬਾਹਰਲਾ);
	ਜੇ (retval) {
		ਲਿਖੋ("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_ਸੁਨੇਹਾ(retval));
		ਵਾਪਸ;
	}
	ਲਿਖੋ("First unmarked block is %llu\n", (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ਬਾਹਰਲਾ);
}

ਖਾਲੀ do_ffsb(ਪੂਰਨ argc, ss_argv_t argv,
	     ਪੂਰਨ sci_idx EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
	     ਖਾਲੀ *infop EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਸਿਰਾ;
	ਪੂਰਨ err;
	errcode_t retval;
	blk64_t ਬਾਹਰਲਾ;

	ਜੇ (check_fs_open(argv[0]))
		ਵਾਪਸ;

	ਜੇ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		ਵਾਪਸ;
	}

	ਸ਼ੁਰੂ = parse_ulong(argv[1], argv[0], "start", &err);
	ਜੇ (err)
		ਵਾਪਸ;

	ਸਿਰਾ = parse_ulong(argv[2], argv[0], "end", &err);
	ਜੇ (err)
		ਵਾਪਸ;

	retval = ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ_ਖੰਡ_bitmap2(test_fs->ਖੰਡ_map,
						      ਸ਼ੁਰੂ, ਸਿਰਾ, &ਬਾਹਰਲਾ);
	ਜੇ (retval) {
		ਲਿਖੋ("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_ਸੁਨੇਹਾ(retval));
		ਵਾਪਸ;
	}
	ਲਿਖੋ("First marked block is %llu\n", (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ਬਾਹਰਲਾ);
}


ਖਾਲੀ do_zerob(ਪੂਰਨ argc, ss_argv_t argv,
	      ਪੂਰਨ sci_idx EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
	      ਖਾਲੀ *infop EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਜੇ (check_fs_open(argv[0]))
		ਵਾਪਸ;

	ਲਿਖੋ("Clearing block bitmap.\n");
	ext2fs_clear_ਖੰਡ_bitmap(test_fs->ਖੰਡ_map);
}

ਖਾਲੀ do_seti(ਪੂਰਨ argc, ss_argv_t argv,
	     ਪੂਰਨ sci_idx EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
	     ਖਾਲੀ *infop EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ inode;
	ਪੂਰਨ err;
	ਪੂਰਨ test_ਨਤੀਜਾ, op_ਨਤੀਜਾ;

	ਜੇ (check_fs_open(argv[0]))
		ਵਾਪਸ;

	ਜੇ (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		ਵਾਪਸ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ਜੇ (err)
		ਵਾਪਸ;

	test_ਨਤੀਜਾ = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_ਨਤੀਜਾ = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	ਲਿਖੋ("Setting inode %u, was %s before\n", inode, op_ਨਤੀਜਾ ?
	       "set" : "clear");
	ਜੇ (!test_ਨਤੀਜਾ != !op_ਨਤੀਜਾ) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_ਨਤੀਜਾ, op_ਨਤੀਜਾ);
		exit_ਸਥਿਤੀ++;
	}
}

ਖਾਲੀ do_cleari(ਪੂਰਨ argc, ss_argv_t argv,
	       ਪੂਰਨ sci_idx EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
	       ਖਾਲੀ *infop EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ inode;
	ਪੂਰਨ err;
	ਪੂਰਨ test_ਨਤੀਜਾ, op_ਨਤੀਜਾ;

	ਜੇ (check_fs_open(argv[0]))
		ਵਾਪਸ;

	ਜੇ (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		ਵਾਪਸ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ਜੇ (err)
		ਵਾਪਸ;

	test_ਨਤੀਜਾ = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_ਨਤੀਜਾ = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	ਲਿਖੋ("Clearing inode %u, was %s before\n", inode, op_ਨਤੀਜਾ ?
	       "set" : "clear");
	ਜੇ (!test_ਨਤੀਜਾ != !op_ਨਤੀਜਾ) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_ਨਤੀਜਾ, op_ਨਤੀਜਾ);
		exit_ਸਥਿਤੀ++;
	}
}

ਖਾਲੀ do_testi(ਪੂਰਨ argc, ss_argv_t argv,
	      ਪੂਰਨ sci_idx EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
	      ਖਾਲੀ *infop EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ inode;
	ਪੂਰਨ err;
	ਪੂਰਨ test_ਨਤੀਜਾ;

	ਜੇ (check_fs_open(argv[0]))
		ਵਾਪਸ;

	ਜੇ (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		ਵਾਪਸ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ਜੇ (err)
		ਵਾਪਸ;

	test_ਨਤੀਜਾ = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	ਲਿਖੋ("Inode %u is %s\n", inode, test_ਨਤੀਜਾ ? "set" : "clear");
}

ਖਾਲੀ do_ffzi(ਪੂਰਨ argc, ss_argv_t argv,
	     ਪੂਰਨ sci_idx EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
	     ਖਾਲੀ *infop EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਸਿਰਾ;
	ਪੂਰਨ err;
	errcode_t retval;
	ext2_ino_t ਬਾਹਰਲਾ;

	ਜੇ (check_fs_open(argv[0]))
		ਵਾਪਸ;

	ਜੇ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		ਵਾਪਸ;
	}

	ਸ਼ੁਰੂ = parse_ulong(argv[1], argv[0], "start", &err);
	ਜੇ (err)
		ਵਾਪਸ;

	ਸਿਰਾ = parse_ulong(argv[2], argv[0], "end", &err);
	ਜੇ (err)
		ਵਾਪਸ;

	retval = ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_zero_inode_bitmap2(test_fs->inode_map,
						      ਸ਼ੁਰੂ, ਸਿਰਾ, &ਬਾਹਰਲਾ);
	ਜੇ (retval) {
		ਲਿਖੋ("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_ਸੁਨੇਹਾ(retval));
		ਵਾਪਸ;
	}
	ਲਿਖੋ("First unmarked inode is %u\n", ਬਾਹਰਲਾ);
}

ਖਾਲੀ do_ffsi(ਪੂਰਨ argc, ss_argv_t argv,
	     ਪੂਰਨ sci_idx EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
	     ਖਾਲੀ *infop EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ, ਸਿਰਾ;
	ਪੂਰਨ err;
	errcode_t retval;
	ext2_ino_t ਬਾਹਰਲਾ;

	ਜੇ (check_fs_open(argv[0]))
		ਵਾਪਸ;

	ਜੇ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		ਵਾਪਸ;
	}

	ਸ਼ੁਰੂ = parse_ulong(argv[1], argv[0], "start", &err);
	ਜੇ (err)
		ਵਾਪਸ;

	ਸਿਰਾ = parse_ulong(argv[2], argv[0], "end", &err);
	ਜੇ (err)
		ਵਾਪਸ;

	retval = ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ_inode_bitmap2(test_fs->inode_map,
						     ਸ਼ੁਰੂ, ਸਿਰਾ, &ਬਾਹਰਲਾ);
	ਜੇ (retval) {
		ਲਿਖੋ("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_ਸੁਨੇਹਾ(retval));
		ਵਾਪਸ;
	}
	ਲਿਖੋ("First marked inode is %u\n", ਬਾਹਰਲਾ);
}

ਖਾਲੀ do_zeroi(ਪੂਰਨ argc, ss_argv_t argv,
	      ਪੂਰਨ sci_idx EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
	      ਖਾਲੀ *infop EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਜੇ (check_fs_open(argv[0]))
		ਵਾਪਸ;

	ਲਿਖੋ("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

ਪੂਰਨ ਮੁੱਖ(ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	ਖੰਡਾਂ = 128;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	inodes = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	type = EXT2FS_BMAP64_BITARRAY;
	ਪੂਰਨ		c, err, ਕੋਡ;
	ਅੱਖਰ		*request = (ਅੱਖਰ *)ਨਿਰਾ;
	ਅੱਖਰ		*cmd_file = 0;
	ਪੂਰਨ		sci_idx;
	ਪੂਰਨ		flags = EXT2_FLAG_64_ਬਿੱਟ;

	ਜੋੜੋ_error_ਸਾਰਣੀ(&et_ss_error_ਸਾਰਣੀ);
	ਜੋੜੋ_error_ਸਾਰਣੀ(&et_ext2_error_ਸਾਰਣੀ);
	ਜਦੋਂ ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		ਚੋਣ (c) {
		ਹਾਲਤ 'b':
			ਖੰਡਾਂ = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ਜੇ (err)
				ਬਾਹਰ_ਨਿਕਲੋ(1);
			ਰੋਕੋ;
		ਹਾਲਤ 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ਜੇ (err)
				ਬਾਹਰ_ਨਿਕਲੋ(1);
			ਰੋਕੋ;
		ਹਾਲਤ 'l':	/* Legacy bitmaps */
			flags = 0;
			ਰੋਕੋ;
		ਹਾਲਤ 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			ਜੇ (err)
				ਬਾਹਰ_ਨਿਕਲੋ(1);
			ਰੋਕੋ;
		ਹਾਲਤ 'R':
			request = optarg;
			ਰੋਕੋ;
		ਹਾਲਤ 'f':
			cmd_file = optarg;
			ਰੋਕੋ;
		ਮੂਲ:
			com_err(argv[0], 0, "Usage: %s [-R request] "
				"[-f cmd_file]", subsystem_name);
			ਬਾਹਰ_ਨਿਕਲੋ(1);
		}
	}

	sci_idx = ss_create_invocation(subsystem_name, ਸੰਸਕਰਣ,
				       (ਅੱਖਰ *)ਨਿਰਾ, &tst_bitmaps_cmds, &ਕੋਡ);
	ਜੇ (ਕੋਡ) {
		ss_perror(sci_idx, ਕੋਡ, "creating invocation");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}

	(ਖਾਲੀ) ss_ਜੋੜੋ_request_ਸਾਰਣੀ (sci_idx, &ss_std_ਬੇਨਤੀਆਂ, 1, &ਕੋਡ);
	ਜੇ (ਕੋਡ) {
		ss_perror(sci_idx, ਕੋਡ, "adding standard requests");
		ਬਾਹਰ_ਨਿਕਲੋ (1);
	}

	ਲਿਖੋ("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, ਸੰਸਕਰਣ);

	ਤਿਆਰੀ_filesystem(argv[0], ਖੰਡਾਂ, inodes, type, flags);

	ਜੇ (request) {
		ਕੋਡ = ss_execute_ਸਤਰ(sci_idx, request);
		ਜੇ (ਕੋਡ) {
			ss_perror(sci_idx, ਕੋਡ, request);
			exit_ਸਥਿਤੀ++;
		}
	} ਨਹੀਂਤਾਂ ਜੇ (cmd_file) {
		exit_ਸਥਿਤੀ = ਸਰੋਤ_file(cmd_file, sci_idx);
	} ਨਹੀਂਤਾਂ {
		ss_listen(sci_idx);
	}

	ਬਾਹਰ_ਨਿਕਲੋ(exit_ਸਥਿਤੀ);
}

