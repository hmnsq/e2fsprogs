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
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_GETOPT_H
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ "ss/ss.h"

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ಬಾಹ್ಯ ss_request_ಕೋಷ್ಟಕ tst_bitmaps_cmds;

ಸ್ಥಿರ ಅಕ್ಷರ subsystem_name[] = "tst_bitmaps";
ಸ್ಥಿರ ಅಕ್ಷರ ಆವೃತ್ತಿ[] = "1.0";

ext2_filsys	test_fs;
ಪೂರ್ಣಾಂಕ		exit_ಸ್ಥಿತಿ = 0;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಮೂಲ_file(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cmd_file, ಪೂರ್ಣಾಂಕ sci_idx)
{
	FILE		*f;
	ಅಕ್ಷರ		buf[256];
	ಅಕ್ಷರ		*cp;
	ಪೂರ್ಣಾಂಕ		retval;
	ಪೂರ್ಣಾಂಕ 		noecho;

	ಆದರೆ (strcmp(cmd_file, "-") == 0)
		f = stdin;
	ಇಲ್ಲದಿದ್ದರೆ {
		f = fopen(cmd_file, "r");
		ಆದರೆ (!f) {
			perror(cmd_file);
			ನಿರ್ಗಮಿಸು(1);
		}
	}
	fflush(stdout);
	fflush(stderr);
	setbuf(stdout, ಖಾಲಿ);
	setbuf(stderr, ಖಾಲಿ);
	ಆಗ (!feof(f)) {
		ಆದರೆ (fgets(buf, ಗಾತ್ರ(buf), f) == ಖಾಲಿ)
			ನಿಲ್ಲಿಸು;
		ಆದರೆ (buf[0] == '#')
			ಮುಂದುವರಿ;
		noecho = 0;
		ಆದರೆ (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = strchr(buf, '\n');
		ಆದರೆ (cp)
			*cp = 0;
		cp = strchr(buf, '\r');
		ಆದರೆ (cp)
			*cp = 0;
		ಆದರೆ (!noecho)
			ಬರೆ("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_ಸಾಲು(sci_idx, buf);
		ಆದರೆ (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_ಸ್ಥಿತಿ++;
		}
	}
	ಹಿಂತಿರುಗಿ exit_ಸ್ಥಿತಿ;
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
#ಆದರೆ defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#ಇಲ್ಲದಿದ್ದರೆ
	optind = 1;
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPTRESET
	optreset = 1;		/* Makes BSD getopt happy */
#ಆದರೆ_ಅಂತ್ಯ
}

/*
 * This function will convert a string to an unsigned long, printing
 * an error message if it fails, and returning success or failure in err.
 */
ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ parse_ulong(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cmd,
			  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *descr, ಪೂರ್ಣಾಂಕ *err)
{
	ಅಕ್ಷರ		*tmp;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ	ret;

	ret = strtoul(str, &tmp, 0);
	ಆದರೆ (*tmp == 0) {
		ಆದರೆ (err)
			*err = 0;
		ಹಿಂತಿರುಗಿ ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	ಆದರೆ (err)
		*err = 1;
	ಇಲ್ಲದಿದ್ದರೆ
		ನಿರ್ಗಮಿಸು(1);
	ಹಿಂತಿರುಗಿ 0;
}


ಪೂರ್ಣಾಂಕ check_fs_open(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	ಆದರೆ (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		ಹಿಂತಿರುಗಿ 1;
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸಿದ್ಧತೆ_filesystem(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
			     ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಖಂಡಗಳು, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ inodes,
			     ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ type, ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ ext2_super_ಖಂಡ param;
	errcode_t retval;

	memset(&param, 0, ಗಾತ್ರ(param));
	ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(&param, ಖಂಡಗಳು);
	param.s_inodes_ಎಣಿಕೆ = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	ಆದರೆ (retval) {
		com_err(name, retval, "while initializing filesystem");
		ಹಿಂತಿರುಗಿ;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_ಖಂಡ_bitmap(test_fs->ಖಂಡ_map);
	test_fs->ಖಂಡ_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_ಖಂಡ_bitmap(test_fs, "block bitmap",
					      &test_fs->ಖಂಡ_map);
	ಆದರೆ (retval) {
		com_err(name, retval, "while allocating block bitmap");
		ಹೋಗು errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	ಆದರೆ (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		ಹೋಗು errout;
	}
	ಹಿಂತಿರುಗಿ;

errout:
	ext2fs_close_free(&test_fs);
}

ಶೂನ್ಯ ಸಿದ್ಧತೆ_cmd(ಪೂರ್ಣಾಂಕ argc, ss_argv_t argv,
	       ಪೂರ್ಣಾಂಕ sci_idx EXT2FS_ATTR((ಬಳಸದ)),
	       ಶೂನ್ಯ *infop EXT2FS_ATTR((ಬಳಸದ)))
{
	ಪೂರ್ಣಾಂಕ		c, err;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	ಖಂಡಗಳು = 128;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	inodes = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	type = EXT2FS_BMAP64_BITARRAY;
	ಪೂರ್ಣಾಂಕ		flags = EXT2_FLAG_64_ಬಿಟ್ಟುಗಳು;

	ಆದರೆ (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	ಆಗ ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		ಆಯ್ಕೆ (c) {
		ಸ್ಥಿತಿ 'b':
			ಖಂಡಗಳು = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ಆದರೆ (err)
				ಹಿಂತಿರುಗಿ;
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ಆದರೆ (err)
				ಹಿಂತಿರುಗಿ;
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ 'l':	/* Legacy bitmaps */
			flags = 0;
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			ಆದರೆ (err)
				ಹಿಂತಿರುಗಿ;
			ನಿಲ್ಲಿಸು;
		ಪೂರ್ವನಿಯೋಜಿತ:
			fprintf(stderr, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			ಹಿಂತಿರುಗಿ;
		}
	}
	ಸಿದ್ಧತೆ_filesystem(argv[0], ಖಂಡಗಳು, inodes, type, flags);
}

ಶೂನ್ಯ close_cmd(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv, ಪೂರ್ಣಾಂಕ sci_idx EXT2FS_ATTR((ಬಳಸದ)),
	       ಶೂನ್ಯ *infop EXT2FS_ATTR((ಬಳಸದ)))
{
	ಆದರೆ (check_fs_open(argv[0]))
		ಹಿಂತಿರುಗಿ;

	ext2fs_close_free(&test_fs);
}


ಶೂನ್ಯ dump_bitmap(ext2fs_generic_bitmap bmap, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಆರಂಭ, ಚಿಹ್ನೆರಹಿತ ಅಂಕೆ)
{
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	*buf;
	errcode_t	retval;
	ಪೂರ್ಣಾಂಕ		i, len = (ಅಂಕೆ - ಆರಂಭ + 7) / 8;

	buf = ಸ್ಮರಣೆ_ಕೊಡು(len);
	ಆದರೆ (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		ಹಿಂತಿರುಗಿ;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) ಆರಂಭ, ಅಂಕೆ, buf);
	ಆದರೆ (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		ಬಿಡುಗಡೆ(buf);
		ಹಿಂತಿರುಗಿ;
	}
	ಗಾಗಿ (i=0; i < len; i++)
		ಬರೆ("%02x", buf[i]);
	ಬರೆ("\n");
	ಬರೆ("bits set: %u\n", ext2fs_bitcount(buf, len));
	ಬಿಡುಗಡೆ(buf);
}

ಶೂನ್ಯ dump_inode_bitmap_cmd(ಪೂರ್ಣಾಂಕ argc, ss_argv_t argv,
			   ಪೂರ್ಣಾಂಕ sci_idx EXT2FS_ATTR((ಬಳಸದ)),
			   ಶೂನ್ಯ *infop EXT2FS_ATTR((ಬಳಸದ)))
{
	ಆದರೆ (check_fs_open(argv[0]))
		ಹಿಂತಿರುಗಿ;

	ಬರೆ("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_ಎಣಿಕೆ);
}
	
ಶೂನ್ಯ dump_ಖಂಡ_bitmap_cmd(ಪೂರ್ಣಾಂಕ argc, ss_argv_t argv,
			   ಪೂರ್ಣಾಂಕ sci_idx EXT2FS_ATTR((ಬಳಸದ)),
			   ಶೂನ್ಯ *infop EXT2FS_ATTR((ಬಳಸದ)))
{
	ಆದರೆ (check_fs_open(argv[0]))
		ಹಿಂತಿರುಗಿ;

	ಬರೆ("block bitmap: ");
	dump_bitmap(test_fs->ಖಂಡ_map, test_fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ,
		    test_fs->super->s_ಖಂಡಗಳು_ಎಣಿಕೆ);
}
	
ಶೂನ್ಯ do_setb(ಪೂರ್ಣಾಂಕ argc, ss_argv_t argv,
	     ಪೂರ್ಣಾಂಕ sci_idx EXT2FS_ATTR((ಬಳಸದ)),
	     ಶೂನ್ಯ *infop EXT2FS_ATTR((ಬಳಸದ)))
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಖಂಡ, ಅಂಕೆ;
	ಪೂರ್ಣಾಂಕ err;
	ಪೂರ್ಣಾಂಕ test_ಫಲಿತಾಂಶ, op_ಫಲಿತಾಂಶ;

	ಆದರೆ (check_fs_open(argv[0]))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		ಹಿಂತಿರುಗಿ;
	}

	ಖಂಡ = parse_ulong(argv[1], argv[0], "block", &err);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (argc == 3) {
		ಅಂಕೆ = parse_ulong(argv[2], argv[0], "num", &err);
		ಆದರೆ (err)
			ಹಿಂತಿರುಗಿ;

		ext2fs_mark_ಖಂಡ_bitmap_range2(test_fs->ಖಂಡ_map,
						ಖಂಡ, ಅಂಕೆ);
		ಬರೆ("Marking blocks %u to %u\n", ಖಂಡ, ಖಂಡ + ಅಂಕೆ - 1);
		ಹಿಂತಿರುಗಿ;
	}

	test_ಫಲಿತಾಂಶ = ext2fs_test_ಖಂಡ_bitmap2(test_fs->ಖಂಡ_map, ಖಂಡ);
	op_ಫಲಿತಾಂಶ = ext2fs_mark_ಖಂಡ_bitmap2(test_fs->ಖಂಡ_map, ಖಂಡ);
	ಬರೆ("Setting block %u, was %s before\n", ಖಂಡ, op_ಫಲಿತಾಂಶ ?
	       "set" : "clear");
	ಆದರೆ (!test_ಫಲಿತಾಂಶ != !op_ಫಲಿತಾಂಶ)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_ಫಲಿತಾಂಶ, op_ಫಲಿತಾಂಶ);
}

ಶೂನ್ಯ do_clearb(ಪೂರ್ಣಾಂಕ argc, ss_argv_t argv,
	       ಪೂರ್ಣಾಂಕ sci_idx EXT2FS_ATTR((ಬಳಸದ)),
	       ಶೂನ್ಯ *infop EXT2FS_ATTR((ಬಳಸದ)))
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಖಂಡ, ಅಂಕೆ;
	ಪೂರ್ಣಾಂಕ err;
	ಪೂರ್ಣಾಂಕ test_ಫಲಿತಾಂಶ, op_ಫಲಿತಾಂಶ;

	ಆದರೆ (check_fs_open(argv[0]))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		ಹಿಂತಿರುಗಿ;
	}

	ಖಂಡ = parse_ulong(argv[1], argv[0], "block", &err);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (argc == 3) {
		ಅಂಕೆ = parse_ulong(argv[2], argv[0], "num", &err);
		ಆದರೆ (err)
			ಹಿಂತಿರುಗಿ;

		ext2fs_unmark_ಖಂಡ_bitmap_range2(test_fs->ಖಂಡ_map,
						ಖಂಡ, ಅಂಕೆ);
		ಬರೆ("Clearing blocks %u to %u\n", ಖಂಡ, ಖಂಡ + ಅಂಕೆ - 1);
		ಹಿಂತಿರುಗಿ;
	}

	test_ಫಲಿತಾಂಶ = ext2fs_test_ಖಂಡ_bitmap2(test_fs->ಖಂಡ_map, ಖಂಡ);
	op_ಫಲಿತಾಂಶ = ext2fs_unmark_ಖಂಡ_bitmap2(test_fs->ಖಂಡ_map, ಖಂಡ);
	ಬರೆ("Clearing block %u, was %s before\n", ಖಂಡ, op_ಫಲಿತಾಂಶ ?
	       "set" : "clear");
	ಆದರೆ (!test_ಫಲಿತಾಂಶ != !op_ಫಲಿತಾಂಶ)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_ಫಲಿತಾಂಶ, op_ಫಲಿತಾಂಶ);
}

ಶೂನ್ಯ do_testb(ಪೂರ್ಣಾಂಕ argc, ss_argv_t argv,
	      ಪೂರ್ಣಾಂಕ sci_idx EXT2FS_ATTR((ಬಳಸದ)),
	      ಶೂನ್ಯ *infop EXT2FS_ATTR((ಬಳಸದ)))
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಖಂಡ, ಅಂಕೆ;
	ಪೂರ್ಣಾಂಕ err;
	ಪೂರ್ಣಾಂಕ test_ಫಲಿತಾಂಶ;

	ಆದರೆ (check_fs_open(argv[0]))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		ಹಿಂತಿರುಗಿ;
	}

	ಖಂಡ = parse_ulong(argv[1], argv[0], "block", &err);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (argc == 3) {
		ಅಂಕೆ = parse_ulong(argv[2], argv[0], "num", &err);
		ಆದರೆ (err)
			ಹಿಂತಿರುಗಿ;

		test_ಫಲಿತಾಂಶ =
			ext2fs_test_ಖಂಡ_bitmap_range2(test_fs->ಖಂಡ_map,
							ಖಂಡ, ಅಂಕೆ);
		ಬರೆ("Blocks %u to %u are %sall clear.\n",
		       ಖಂಡ, ಖಂಡ + ಅಂಕೆ - 1, test_ಫಲಿತಾಂಶ ? "" : "NOT ");
		ಹಿಂತಿರುಗಿ;
	}

	test_ಫಲಿತಾಂಶ = ext2fs_test_ಖಂಡ_bitmap2(test_fs->ಖಂಡ_map, ಖಂಡ);
	ಬರೆ("Block %u is %s\n", ಖಂಡ, test_ಫಲಿತಾಂಶ ? "set" : "clear");
}

ಶೂನ್ಯ do_ffzb(ಪೂರ್ಣಾಂಕ argc, ss_argv_t argv,
	     ಪೂರ್ಣಾಂಕ sci_idx EXT2FS_ATTR((ಬಳಸದ)),
	     ಶೂನ್ಯ *infop EXT2FS_ATTR((ಬಳಸದ)))
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಆರಂಭ, ಕೊನೆ;
	ಪೂರ್ಣಾಂಕ err;
	errcode_t retval;
	blk64_t ಹೊರಹೋಗು;

	ಆದರೆ (check_fs_open(argv[0]))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		ಹಿಂತಿರುಗಿ;
	}

	ಆರಂಭ = parse_ulong(argv[1], argv[0], "start", &err);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ;

	ಕೊನೆ = parse_ulong(argv[2], argv[0], "end", &err);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ;

	retval = ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero_ಖಂಡ_bitmap2(test_fs->ಖಂಡ_map,
						      ಆರಂಭ, ಕೊನೆ, &ಹೊರಹೋಗು);
	ಆದರೆ (retval) {
		ಬರೆ("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_ಸಂದೇಶ(retval));
		ಹಿಂತಿರುಗಿ;
	}
	ಬರೆ("First unmarked block is %llu\n", (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ಹೊರಹೋಗು);
}

ಶೂನ್ಯ do_ffsb(ಪೂರ್ಣಾಂಕ argc, ss_argv_t argv,
	     ಪೂರ್ಣಾಂಕ sci_idx EXT2FS_ATTR((ಬಳಸದ)),
	     ಶೂನ್ಯ *infop EXT2FS_ATTR((ಬಳಸದ)))
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಆರಂಭ, ಕೊನೆ;
	ಪೂರ್ಣಾಂಕ err;
	errcode_t retval;
	blk64_t ಹೊರಹೋಗು;

	ಆದರೆ (check_fs_open(argv[0]))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		ಹಿಂತಿರುಗಿ;
	}

	ಆರಂಭ = parse_ulong(argv[1], argv[0], "start", &err);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ;

	ಕೊನೆ = parse_ulong(argv[2], argv[0], "end", &err);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ;

	retval = ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ_ಖಂಡ_bitmap2(test_fs->ಖಂಡ_map,
						      ಆರಂಭ, ಕೊನೆ, &ಹೊರಹೋಗು);
	ಆದರೆ (retval) {
		ಬರೆ("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_ಸಂದೇಶ(retval));
		ಹಿಂತಿರುಗಿ;
	}
	ಬರೆ("First marked block is %llu\n", (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ಹೊರಹೋಗು);
}


ಶೂನ್ಯ do_zerob(ಪೂರ್ಣಾಂಕ argc, ss_argv_t argv,
	      ಪೂರ್ಣಾಂಕ sci_idx EXT2FS_ATTR((ಬಳಸದ)),
	      ಶೂನ್ಯ *infop EXT2FS_ATTR((ಬಳಸದ)))
{
	ಆದರೆ (check_fs_open(argv[0]))
		ಹಿಂತಿರುಗಿ;

	ಬರೆ("Clearing block bitmap.\n");
	ext2fs_clear_ಖಂಡ_bitmap(test_fs->ಖಂಡ_map);
}

ಶೂನ್ಯ do_seti(ಪೂರ್ಣಾಂಕ argc, ss_argv_t argv,
	     ಪೂರ್ಣಾಂಕ sci_idx EXT2FS_ATTR((ಬಳಸದ)),
	     ಶೂನ್ಯ *infop EXT2FS_ATTR((ಬಳಸದ)))
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ inode;
	ಪೂರ್ಣಾಂಕ err;
	ಪೂರ್ಣಾಂಕ test_ಫಲಿತಾಂಶ, op_ಫಲಿತಾಂಶ;

	ಆದರೆ (check_fs_open(argv[0]))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		ಹಿಂತಿರುಗಿ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ;

	test_ಫಲಿತಾಂಶ = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_ಫಲಿತಾಂಶ = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	ಬರೆ("Setting inode %u, was %s before\n", inode, op_ಫಲಿತಾಂಶ ?
	       "set" : "clear");
	ಆದರೆ (!test_ಫಲಿತಾಂಶ != !op_ಫಲಿತಾಂಶ) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_ಫಲಿತಾಂಶ, op_ಫಲಿತಾಂಶ);
		exit_ಸ್ಥಿತಿ++;
	}
}

ಶೂನ್ಯ do_cleari(ಪೂರ್ಣಾಂಕ argc, ss_argv_t argv,
	       ಪೂರ್ಣಾಂಕ sci_idx EXT2FS_ATTR((ಬಳಸದ)),
	       ಶೂನ್ಯ *infop EXT2FS_ATTR((ಬಳಸದ)))
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ inode;
	ಪೂರ್ಣಾಂಕ err;
	ಪೂರ್ಣಾಂಕ test_ಫಲಿತಾಂಶ, op_ಫಲಿತಾಂಶ;

	ಆದರೆ (check_fs_open(argv[0]))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		ಹಿಂತಿರುಗಿ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ;

	test_ಫಲಿತಾಂಶ = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_ಫಲಿತಾಂಶ = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	ಬರೆ("Clearing inode %u, was %s before\n", inode, op_ಫಲಿತಾಂಶ ?
	       "set" : "clear");
	ಆದರೆ (!test_ಫಲಿತಾಂಶ != !op_ಫಲಿತಾಂಶ) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_ಫಲಿತಾಂಶ, op_ಫಲಿತಾಂಶ);
		exit_ಸ್ಥಿತಿ++;
	}
}

ಶೂನ್ಯ do_testi(ಪೂರ್ಣಾಂಕ argc, ss_argv_t argv,
	      ಪೂರ್ಣಾಂಕ sci_idx EXT2FS_ATTR((ಬಳಸದ)),
	      ಶೂನ್ಯ *infop EXT2FS_ATTR((ಬಳಸದ)))
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ inode;
	ಪೂರ್ಣಾಂಕ err;
	ಪೂರ್ಣಾಂಕ test_ಫಲಿತಾಂಶ;

	ಆದರೆ (check_fs_open(argv[0]))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		ಹಿಂತಿರುಗಿ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ;

	test_ಫಲಿತಾಂಶ = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	ಬರೆ("Inode %u is %s\n", inode, test_ಫಲಿತಾಂಶ ? "set" : "clear");
}

ಶೂನ್ಯ do_ffzi(ಪೂರ್ಣಾಂಕ argc, ss_argv_t argv,
	     ಪೂರ್ಣಾಂಕ sci_idx EXT2FS_ATTR((ಬಳಸದ)),
	     ಶೂನ್ಯ *infop EXT2FS_ATTR((ಬಳಸದ)))
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಆರಂಭ, ಕೊನೆ;
	ಪೂರ್ಣಾಂಕ err;
	errcode_t retval;
	ext2_ino_t ಹೊರಹೋಗು;

	ಆದರೆ (check_fs_open(argv[0]))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		ಹಿಂತಿರುಗಿ;
	}

	ಆರಂಭ = parse_ulong(argv[1], argv[0], "start", &err);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ;

	ಕೊನೆ = parse_ulong(argv[2], argv[0], "end", &err);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ;

	retval = ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero_inode_bitmap2(test_fs->inode_map,
						      ಆರಂಭ, ಕೊನೆ, &ಹೊರಹೋಗು);
	ಆದರೆ (retval) {
		ಬರೆ("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_ಸಂದೇಶ(retval));
		ಹಿಂತಿರುಗಿ;
	}
	ಬರೆ("First unmarked inode is %u\n", ಹೊರಹೋಗು);
}

ಶೂನ್ಯ do_ffsi(ಪೂರ್ಣಾಂಕ argc, ss_argv_t argv,
	     ಪೂರ್ಣಾಂಕ sci_idx EXT2FS_ATTR((ಬಳಸದ)),
	     ಶೂನ್ಯ *infop EXT2FS_ATTR((ಬಳಸದ)))
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಆರಂಭ, ಕೊನೆ;
	ಪೂರ್ಣಾಂಕ err;
	errcode_t retval;
	ext2_ino_t ಹೊರಹೋಗು;

	ಆದರೆ (check_fs_open(argv[0]))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		ಹಿಂತಿರುಗಿ;
	}

	ಆರಂಭ = parse_ulong(argv[1], argv[0], "start", &err);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ;

	ಕೊನೆ = parse_ulong(argv[2], argv[0], "end", &err);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ;

	retval = ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ_inode_bitmap2(test_fs->inode_map,
						     ಆರಂಭ, ಕೊನೆ, &ಹೊರಹೋಗು);
	ಆದರೆ (retval) {
		ಬರೆ("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_ಸಂದೇಶ(retval));
		ಹಿಂತಿರುಗಿ;
	}
	ಬರೆ("First marked inode is %u\n", ಹೊರಹೋಗು);
}

ಶೂನ್ಯ do_zeroi(ಪೂರ್ಣಾಂಕ argc, ss_argv_t argv,
	      ಪೂರ್ಣಾಂಕ sci_idx EXT2FS_ATTR((ಬಳಸದ)),
	      ಶೂನ್ಯ *infop EXT2FS_ATTR((ಬಳಸದ)))
{
	ಆದರೆ (check_fs_open(argv[0]))
		ಹಿಂತಿರುಗಿ;

	ಬರೆ("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

ಪೂರ್ಣಾಂಕ ಮುಖ್ಯ(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	ಖಂಡಗಳು = 128;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	inodes = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	type = EXT2FS_BMAP64_BITARRAY;
	ಪೂರ್ಣಾಂಕ		c, err, ಸಂಕೇತ_2;
	ಅಕ್ಷರ		*request = (ಅಕ್ಷರ *)ಖಾಲಿ;
	ಅಕ್ಷರ		*cmd_file = 0;
	ಪೂರ್ಣಾಂಕ		sci_idx;
	ಪೂರ್ಣಾಂಕ		flags = EXT2_FLAG_64_ಬಿಟ್ಟುಗಳು;

	ಕೂಡಿಸು_error_ಕೋಷ್ಟಕ(&et_ss_error_ಕೋಷ್ಟಕ);
	ಕೂಡಿಸು_error_ಕೋಷ್ಟಕ(&et_ext2_error_ಕೋಷ್ಟಕ);
	ಆಗ ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		ಆಯ್ಕೆ (c) {
		ಸ್ಥಿತಿ 'b':
			ಖಂಡಗಳು = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ಆದರೆ (err)
				ನಿರ್ಗಮಿಸು(1);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ಆದರೆ (err)
				ನಿರ್ಗಮಿಸು(1);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ 'l':	/* Legacy bitmaps */
			flags = 0;
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			ಆದರೆ (err)
				ನಿರ್ಗಮಿಸು(1);
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ 'R':
			request = optarg;
			ನಿಲ್ಲಿಸು;
		ಸ್ಥಿತಿ 'f':
			cmd_file = optarg;
			ನಿಲ್ಲಿಸು;
		ಪೂರ್ವನಿಯೋಜಿತ:
			com_err(argv[0], 0, "Usage: %s [-R request] "
				"[-f cmd_file]", subsystem_name);
			ನಿರ್ಗಮಿಸು(1);
		}
	}

	sci_idx = ss_create_invocation(subsystem_name, ಆವೃತ್ತಿ,
				       (ಅಕ್ಷರ *)ಖಾಲಿ, &tst_bitmaps_cmds, &ಸಂಕೇತ_2);
	ಆದರೆ (ಸಂಕೇತ_2) {
		ss_perror(sci_idx, ಸಂಕೇತ_2, "creating invocation");
		ನಿರ್ಗಮಿಸು(1);
	}

	(ಶೂನ್ಯ) ss_ಕೂಡಿಸು_request_ಕೋಷ್ಟಕ (sci_idx, &ss_std_ವಿನಂತಿಗಳು, 1, &ಸಂಕೇತ_2);
	ಆದರೆ (ಸಂಕೇತ_2) {
		ss_perror(sci_idx, ಸಂಕೇತ_2, "adding standard requests");
		ನಿರ್ಗಮಿಸು (1);
	}

	ಬರೆ("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, ಆವೃತ್ತಿ);

	ಸಿದ್ಧತೆ_filesystem(argv[0], ಖಂಡಗಳು, inodes, type, flags);

	ಆದರೆ (request) {
		ಸಂಕೇತ_2 = ss_execute_ಸಾಲು(sci_idx, request);
		ಆದರೆ (ಸಂಕೇತ_2) {
			ss_perror(sci_idx, ಸಂಕೇತ_2, request);
			exit_ಸ್ಥಿತಿ++;
		}
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cmd_file) {
		exit_ಸ್ಥಿತಿ = ಮೂಲ_file(cmd_file, sci_idx);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ss_listen(sci_idx);
	}

	ನಿರ್ಗಮಿಸು(exit_ಸ್ಥಿತಿ);
}

