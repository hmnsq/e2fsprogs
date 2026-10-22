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
#جيڪڏهن_وصف_ٿيل HAVE_GETOPT_H
#ضمن <getopt.h>
#جيڪڏهن_پورو
#ضمن <string.h>
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#ضمن "ss/ss.h"

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

خارجي ss_request_جدول tst_bitmaps_cmds;

ساكن حرف subsystem_name[] = "tst_bitmaps";
ساكن حرف نسخو[] = "1.0";

ext2_filsys	test_fs;
صحيح		exit_حالت = 0;

ساكن صحيح ذريعو_file(ثابت حرف *cmd_file, صحيح sci_idx)
{
	FILE		*f;
	حرف		buf[256];
	حرف		*cp;
	صحيح		retval;
	صحيح 		noecho;

	إذا (strcmp(cmd_file, "-") == 0)
		f = stdin;
	وإلا {
		f = fopen(cmd_file, "r");
		إذا (!f) {
			perror(cmd_file);
			ٻاهر_نڪرو(1);
		}
	}
	fflush(stdout);
	fflush(stderr);
	setbuf(stdout, عدم);
	setbuf(stderr, عدم);
	طالما (!feof(f)) {
		إذا (fgets(buf, حجم(buf), f) == عدم)
			اكسر;
		إذا (buf[0] == '#')
			استمر;
		noecho = 0;
		إذا (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = strchr(buf, '\n');
		إذا (cp)
			*cp = 0;
		cp = strchr(buf, '\r');
		إذا (cp)
			*cp = 0;
		إذا (!noecho)
			لکو("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_سٽ(sci_idx, buf);
		إذا (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_حالت++;
		}
	}
	أرجع exit_حالت;
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
#إذا defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#وإلا
	optind = 1;
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_OPTRESET
	optreset = 1;		/* Makes BSD getopt happy */
#جيڪڏهن_پورو
}

/*
 * This function will convert a string to an unsigned long, printing
 * an error message if it fails, and returning success or failure in err.
 */
بنانشان ڊگهو parse_ulong(ثابت حرف *str, ثابت حرف *cmd,
			  ثابت حرف *descr, صحيح *err)
{
	حرف		*tmp;
	بنانشان ڊگهو	ret;

	ret = strtoul(str, &tmp, 0);
	إذا (*tmp == 0) {
		إذا (err)
			*err = 0;
		أرجع ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	إذا (err)
		*err = 1;
	وإلا
		ٻاهر_نڪرو(1);
	أرجع 0;
}


صحيح check_fs_open(ثابت حرف *name)
{
	إذا (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		أرجع 1;
	}
	أرجع 0;
}

ساكن فراغ تياري_filesystem(ثابت حرف *name,
			     بنانشان صحيح بلاڪون, بنانشان صحيح inodes,
			     بنانشان صحيح type, صحيح flags)
{
	بنية ext2_super_بلاڪ param;
	errcode_t retval;

	memset(&param, 0, حجم(param));
	ext2fs_بلاڪون_ڳڻپ_سيٽ(&param, بلاڪون);
	param.s_inodes_ڳڻپ = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	إذا (retval) {
		com_err(name, retval, "while initializing filesystem");
		أرجع;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_بلاڪ_bitmap(test_fs->بلاڪ_map);
	test_fs->بلاڪ_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_بلاڪ_bitmap(test_fs, "block bitmap",
					      &test_fs->بلاڪ_map);
	إذا (retval) {
		com_err(name, retval, "while allocating block bitmap");
		اذهب errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	إذا (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		اذهب errout;
	}
	أرجع;

errout:
	ext2fs_close_free(&test_fs);
}

فراغ تياري_cmd(صحيح argc, ss_argv_t argv,
	       صحيح sci_idx EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
	       فراغ *infop EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	صحيح		c, err;
	بنانشان صحيح	بلاڪون = 128;
	بنانشان صحيح	inodes = 0;
	بنانشان صحيح	type = EXT2FS_BMAP64_BITARRAY;
	صحيح		flags = EXT2_FLAG_64_بٽ;

	إذا (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	طالما ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		بدل (c) {
		حالة 'b':
			بلاڪون = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			إذا (err)
				أرجع;
			اكسر;
		حالة 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			إذا (err)
				أرجع;
			اكسر;
		حالة 'l':	/* Legacy bitmaps */
			flags = 0;
			اكسر;
		حالة 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			إذا (err)
				أرجع;
			اكسر;
		افتراضي:
			fprintf(stderr, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			أرجع;
		}
	}
	تياري_filesystem(argv[0], بلاڪون, inodes, type, flags);
}

فراغ close_cmd(صحيح argc, حرف **argv, صحيح sci_idx EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
	       فراغ *infop EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	إذا (check_fs_open(argv[0]))
		أرجع;

	ext2fs_close_free(&test_fs);
}


فراغ dump_bitmap(ext2fs_generic_bitmap bmap, بنانشان صحيح شروعات, بنانشان انگ)
{
	بنانشان حرف	*buf;
	errcode_t	retval;
	صحيح		i, len = (انگ - شروعات + 7) / 8;

	buf = يادگيري_ڏيو(len);
	إذا (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		أرجع;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) شروعات, انگ, buf);
	إذا (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		حرر(buf);
		أرجع;
	}
	لكل (i=0; i < len; i++)
		لکو("%02x", buf[i]);
	لکو("\n");
	لکو("bits set: %u\n", ext2fs_bitcount(buf, len));
	حرر(buf);
}

فراغ dump_inode_bitmap_cmd(صحيح argc, ss_argv_t argv,
			   صحيح sci_idx EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
			   فراغ *infop EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	إذا (check_fs_open(argv[0]))
		أرجع;

	لکو("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_ڳڻپ);
}
	
فراغ dump_بلاڪ_bitmap_cmd(صحيح argc, ss_argv_t argv,
			   صحيح sci_idx EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
			   فراغ *infop EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	إذا (check_fs_open(argv[0]))
		أرجع;

	لکو("block bitmap: ");
	dump_bitmap(test_fs->بلاڪ_map, test_fs->super->s_پهريون_معلومات_بلاڪ,
		    test_fs->super->s_بلاڪون_ڳڻپ);
}
	
فراغ do_setb(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
	     فراغ *infop EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	بنانشان صحيح بلاڪ, انگ;
	صحيح err;
	صحيح test_نتيجو, op_نتيجو;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		أرجع;
	}

	بلاڪ = parse_ulong(argv[1], argv[0], "block", &err);
	إذا (err)
		أرجع;

	إذا (argc == 3) {
		انگ = parse_ulong(argv[2], argv[0], "num", &err);
		إذا (err)
			أرجع;

		ext2fs_mark_بلاڪ_bitmap_range2(test_fs->بلاڪ_map,
						بلاڪ, انگ);
		لکو("Marking blocks %u to %u\n", بلاڪ, بلاڪ + انگ - 1);
		أرجع;
	}

	test_نتيجو = ext2fs_test_بلاڪ_bitmap2(test_fs->بلاڪ_map, بلاڪ);
	op_نتيجو = ext2fs_mark_بلاڪ_bitmap2(test_fs->بلاڪ_map, بلاڪ);
	لکو("Setting block %u, was %s before\n", بلاڪ, op_نتيجو ?
	       "set" : "clear");
	إذا (!test_نتيجو != !op_نتيجو)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_نتيجو, op_نتيجو);
}

فراغ do_clearb(صحيح argc, ss_argv_t argv,
	       صحيح sci_idx EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
	       فراغ *infop EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	بنانشان صحيح بلاڪ, انگ;
	صحيح err;
	صحيح test_نتيجو, op_نتيجو;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		أرجع;
	}

	بلاڪ = parse_ulong(argv[1], argv[0], "block", &err);
	إذا (err)
		أرجع;

	إذا (argc == 3) {
		انگ = parse_ulong(argv[2], argv[0], "num", &err);
		إذا (err)
			أرجع;

		ext2fs_unmark_بلاڪ_bitmap_range2(test_fs->بلاڪ_map,
						بلاڪ, انگ);
		لکو("Clearing blocks %u to %u\n", بلاڪ, بلاڪ + انگ - 1);
		أرجع;
	}

	test_نتيجو = ext2fs_test_بلاڪ_bitmap2(test_fs->بلاڪ_map, بلاڪ);
	op_نتيجو = ext2fs_unmark_بلاڪ_bitmap2(test_fs->بلاڪ_map, بلاڪ);
	لکو("Clearing block %u, was %s before\n", بلاڪ, op_نتيجو ?
	       "set" : "clear");
	إذا (!test_نتيجو != !op_نتيجو)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_نتيجو, op_نتيجو);
}

فراغ do_testb(صحيح argc, ss_argv_t argv,
	      صحيح sci_idx EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
	      فراغ *infop EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	بنانشان صحيح بلاڪ, انگ;
	صحيح err;
	صحيح test_نتيجو;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		أرجع;
	}

	بلاڪ = parse_ulong(argv[1], argv[0], "block", &err);
	إذا (err)
		أرجع;

	إذا (argc == 3) {
		انگ = parse_ulong(argv[2], argv[0], "num", &err);
		إذا (err)
			أرجع;

		test_نتيجو =
			ext2fs_test_بلاڪ_bitmap_range2(test_fs->بلاڪ_map,
							بلاڪ, انگ);
		لکو("Blocks %u to %u are %sall clear.\n",
		       بلاڪ, بلاڪ + انگ - 1, test_نتيجو ? "" : "NOT ");
		أرجع;
	}

	test_نتيجو = ext2fs_test_بلاڪ_bitmap2(test_fs->بلاڪ_map, بلاڪ);
	لکو("Block %u is %s\n", بلاڪ, test_نتيجو ? "set" : "clear");
}

فراغ do_ffzb(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
	     فراغ *infop EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	بنانشان صحيح شروعات, پڇاڙي;
	صحيح err;
	errcode_t retval;
	blk64_t ٻاهريون;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		أرجع;
	}

	شروعات = parse_ulong(argv[1], argv[0], "start", &err);
	إذا (err)
		أرجع;

	پڇاڙي = parse_ulong(argv[2], argv[0], "end", &err);
	إذا (err)
		أرجع;

	retval = ext2fs_ڳوليو_پهريون_zero_بلاڪ_bitmap2(test_fs->بلاڪ_map,
						      شروعات, پڇاڙي, &ٻاهريون);
	إذا (retval) {
		لکو("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_پيغام(retval));
		أرجع;
	}
	لکو("First unmarked block is %llu\n", (بنانشان ڊگهو ڊگهو) ٻاهريون);
}

فراغ do_ffsb(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
	     فراغ *infop EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	بنانشان صحيح شروعات, پڇاڙي;
	صحيح err;
	errcode_t retval;
	blk64_t ٻاهريون;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		أرجع;
	}

	شروعات = parse_ulong(argv[1], argv[0], "start", &err);
	إذا (err)
		أرجع;

	پڇاڙي = parse_ulong(argv[2], argv[0], "end", &err);
	إذا (err)
		أرجع;

	retval = ext2fs_ڳوليو_پهريون_سيٽ_بلاڪ_bitmap2(test_fs->بلاڪ_map,
						      شروعات, پڇاڙي, &ٻاهريون);
	إذا (retval) {
		لکو("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_پيغام(retval));
		أرجع;
	}
	لکو("First marked block is %llu\n", (بنانشان ڊگهو ڊگهو) ٻاهريون);
}


فراغ do_zerob(صحيح argc, ss_argv_t argv,
	      صحيح sci_idx EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
	      فراغ *infop EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	إذا (check_fs_open(argv[0]))
		أرجع;

	لکو("Clearing block bitmap.\n");
	ext2fs_clear_بلاڪ_bitmap(test_fs->بلاڪ_map);
}

فراغ do_seti(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
	     فراغ *infop EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	بنانشان صحيح inode;
	صحيح err;
	صحيح test_نتيجو, op_نتيجو;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		أرجع;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	إذا (err)
		أرجع;

	test_نتيجو = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_نتيجو = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	لکو("Setting inode %u, was %s before\n", inode, op_نتيجو ?
	       "set" : "clear");
	إذا (!test_نتيجو != !op_نتيجو) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_نتيجو, op_نتيجو);
		exit_حالت++;
	}
}

فراغ do_cleari(صحيح argc, ss_argv_t argv,
	       صحيح sci_idx EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
	       فراغ *infop EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	بنانشان صحيح inode;
	صحيح err;
	صحيح test_نتيجو, op_نتيجو;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		أرجع;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	إذا (err)
		أرجع;

	test_نتيجو = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_نتيجو = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	لکو("Clearing inode %u, was %s before\n", inode, op_نتيجو ?
	       "set" : "clear");
	إذا (!test_نتيجو != !op_نتيجو) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_نتيجو, op_نتيجو);
		exit_حالت++;
	}
}

فراغ do_testi(صحيح argc, ss_argv_t argv,
	      صحيح sci_idx EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
	      فراغ *infop EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	بنانشان صحيح inode;
	صحيح err;
	صحيح test_نتيجو;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		أرجع;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	إذا (err)
		أرجع;

	test_نتيجو = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	لکو("Inode %u is %s\n", inode, test_نتيجو ? "set" : "clear");
}

فراغ do_ffzi(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
	     فراغ *infop EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	بنانشان صحيح شروعات, پڇاڙي;
	صحيح err;
	errcode_t retval;
	ext2_ino_t ٻاهريون;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		أرجع;
	}

	شروعات = parse_ulong(argv[1], argv[0], "start", &err);
	إذا (err)
		أرجع;

	پڇاڙي = parse_ulong(argv[2], argv[0], "end", &err);
	إذا (err)
		أرجع;

	retval = ext2fs_ڳوليو_پهريون_zero_inode_bitmap2(test_fs->inode_map,
						      شروعات, پڇاڙي, &ٻاهريون);
	إذا (retval) {
		لکو("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_پيغام(retval));
		أرجع;
	}
	لکو("First unmarked inode is %u\n", ٻاهريون);
}

فراغ do_ffsi(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
	     فراغ *infop EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	بنانشان صحيح شروعات, پڇاڙي;
	صحيح err;
	errcode_t retval;
	ext2_ino_t ٻاهريون;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		أرجع;
	}

	شروعات = parse_ulong(argv[1], argv[0], "start", &err);
	إذا (err)
		أرجع;

	پڇاڙي = parse_ulong(argv[2], argv[0], "end", &err);
	إذا (err)
		أرجع;

	retval = ext2fs_ڳوليو_پهريون_سيٽ_inode_bitmap2(test_fs->inode_map,
						     شروعات, پڇاڙي, &ٻاهريون);
	إذا (retval) {
		لکو("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_پيغام(retval));
		أرجع;
	}
	لکو("First marked inode is %u\n", ٻاهريون);
}

فراغ do_zeroi(صحيح argc, ss_argv_t argv,
	      صحيح sci_idx EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
	      فراغ *infop EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	إذا (check_fs_open(argv[0]))
		أرجع;

	لکو("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

صحيح مکيه(صحيح argc, حرف **argv)
{
	بنانشان صحيح	بلاڪون = 128;
	بنانشان صحيح	inodes = 0;
	بنانشان صحيح	type = EXT2FS_BMAP64_BITARRAY;
	صحيح		c, err, ڪوڊ;
	حرف		*request = (حرف *)عدم;
	حرف		*cmd_file = 0;
	صحيح		sci_idx;
	صحيح		flags = EXT2_FLAG_64_بٽ;

	جوڙيو_error_جدول(&et_ss_error_جدول);
	جوڙيو_error_جدول(&et_ext2_error_جدول);
	طالما ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		بدل (c) {
		حالة 'b':
			بلاڪون = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			إذا (err)
				ٻاهر_نڪرو(1);
			اكسر;
		حالة 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			إذا (err)
				ٻاهر_نڪرو(1);
			اكسر;
		حالة 'l':	/* Legacy bitmaps */
			flags = 0;
			اكسر;
		حالة 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			إذا (err)
				ٻاهر_نڪرو(1);
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
			ٻاهر_نڪرو(1);
		}
	}

	sci_idx = ss_create_invocation(subsystem_name, نسخو,
				       (حرف *)عدم, &tst_bitmaps_cmds, &ڪوڊ);
	إذا (ڪوڊ) {
		ss_perror(sci_idx, ڪوڊ, "creating invocation");
		ٻاهر_نڪرو(1);
	}

	(فراغ) ss_جوڙيو_request_جدول (sci_idx, &ss_std_درخواستون, 1, &ڪوڊ);
	إذا (ڪوڊ) {
		ss_perror(sci_idx, ڪوڊ, "adding standard requests");
		ٻاهر_نڪرو (1);
	}

	لکو("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, نسخو);

	تياري_filesystem(argv[0], بلاڪون, inodes, type, flags);

	إذا (request) {
		ڪوڊ = ss_execute_سٽ(sci_idx, request);
		إذا (ڪوڊ) {
			ss_perror(sci_idx, ڪوڊ, request);
			exit_حالت++;
		}
	} وإلا إذا (cmd_file) {
		exit_حالت = ذريعو_file(cmd_file, sci_idx);
	} وإلا {
		ss_listen(sci_idx);
	}

	ٻاهر_نڪرو(exit_حالت);
}

