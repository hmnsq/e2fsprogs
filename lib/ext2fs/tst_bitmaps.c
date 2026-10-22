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
#که_تعریف_شوی HAVE_GETOPT_H
#ضمن <getopt.h>
#د_که_پای
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
ساكن حرف نسخه[] = "1.0";

ext2_filsys	test_fs;
صحيح		exit_حالت = 0;

ساكن صحيح سرچینه_file(ثابت حرف *cmd_file, صحيح sci_idx)
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
			وتل(1);
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
			چاپ_کړه("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_کرښه(sci_idx, buf);
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
#د_که_پای
#که_تعریف_شوی HAVE_OPTRESET
	optreset = 1;		/* Makes BSD getopt happy */
#د_که_پای
}

/*
 * This function will convert a string to an unsigned long, printing
 * an error message if it fails, and returning success or failure in err.
 */
بینښه اوږد parse_ulong(ثابت حرف *str, ثابت حرف *cmd,
			  ثابت حرف *descr, صحيح *err)
{
	حرف		*tmp;
	بینښه اوږد	ret;

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
		وتل(1);
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

ساكن فراغ تیاری_filesystem(ثابت حرف *name,
			     بینښه صحيح بلاکونه, بینښه صحيح inodes,
			     بینښه صحيح type, صحيح flags)
{
	بنية ext2_super_بلاک param;
	errcode_t retval;

	memset(&param, 0, حجم(param));
	ext2fs_بلاکونه_شمېر_سېټ(&param, بلاکونه);
	param.s_inodes_شمېر = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	إذا (retval) {
		com_err(name, retval, "while initializing filesystem");
		أرجع;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_بلاک_bitmap(test_fs->بلاک_map);
	test_fs->بلاک_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_بلاک_bitmap(test_fs, "block bitmap",
					      &test_fs->بلاک_map);
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

فراغ تیاری_cmd(صحيح argc, ss_argv_t argv,
	       صحيح sci_idx EXT2FS_ATTR((ناکارول_شوی)),
	       فراغ *infop EXT2FS_ATTR((ناکارول_شوی)))
{
	صحيح		c, err;
	بینښه صحيح	بلاکونه = 128;
	بینښه صحيح	inodes = 0;
	بینښه صحيح	type = EXT2FS_BMAP64_BITARRAY;
	صحيح		flags = EXT2_FLAG_64_بټونه;

	إذا (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	طالما ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		بدل (c) {
		حالة 'b':
			بلاکونه = parse_ulong(optarg, argv[0],
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
	تیاری_filesystem(argv[0], بلاکونه, inodes, type, flags);
}

فراغ close_cmd(صحيح argc, حرف **argv, صحيح sci_idx EXT2FS_ATTR((ناکارول_شوی)),
	       فراغ *infop EXT2FS_ATTR((ناکارول_شوی)))
{
	إذا (check_fs_open(argv[0]))
		أرجع;

	ext2fs_close_free(&test_fs);
}


فراغ dump_bitmap(ext2fs_generic_bitmap bmap, بینښه صحيح پیل, بینښه عدد)
{
	بینښه حرف	*buf;
	errcode_t	retval;
	صحيح		i, len = (عدد - پیل + 7) / 8;

	buf = حافظه_ورکړه(len);
	إذا (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		أرجع;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) پیل, عدد, buf);
	إذا (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		حرر(buf);
		أرجع;
	}
	لكل (i=0; i < len; i++)
		چاپ_کړه("%02x", buf[i]);
	چاپ_کړه("\n");
	چاپ_کړه("bits set: %u\n", ext2fs_bitcount(buf, len));
	حرر(buf);
}

فراغ dump_inode_bitmap_cmd(صحيح argc, ss_argv_t argv,
			   صحيح sci_idx EXT2FS_ATTR((ناکارول_شوی)),
			   فراغ *infop EXT2FS_ATTR((ناکارول_شوی)))
{
	إذا (check_fs_open(argv[0]))
		أرجع;

	چاپ_کړه("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_شمېر);
}
	
فراغ dump_بلاک_bitmap_cmd(صحيح argc, ss_argv_t argv,
			   صحيح sci_idx EXT2FS_ATTR((ناکارول_شوی)),
			   فراغ *infop EXT2FS_ATTR((ناکارول_شوی)))
{
	إذا (check_fs_open(argv[0]))
		أرجع;

	چاپ_کړه("block bitmap: ");
	dump_bitmap(test_fs->بلاک_map, test_fs->super->s_لومړی_معلومات_بلاک,
		    test_fs->super->s_بلاکونه_شمېر);
}
	
فراغ do_setb(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((ناکارول_شوی)),
	     فراغ *infop EXT2FS_ATTR((ناکارول_شوی)))
{
	بینښه صحيح بلاک, عدد;
	صحيح err;
	صحيح test_پایله, op_پایله;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		أرجع;
	}

	بلاک = parse_ulong(argv[1], argv[0], "block", &err);
	إذا (err)
		أرجع;

	إذا (argc == 3) {
		عدد = parse_ulong(argv[2], argv[0], "num", &err);
		إذا (err)
			أرجع;

		ext2fs_mark_بلاک_bitmap_range2(test_fs->بلاک_map,
						بلاک, عدد);
		چاپ_کړه("Marking blocks %u to %u\n", بلاک, بلاک + عدد - 1);
		أرجع;
	}

	test_پایله = ext2fs_test_بلاک_bitmap2(test_fs->بلاک_map, بلاک);
	op_پایله = ext2fs_mark_بلاک_bitmap2(test_fs->بلاک_map, بلاک);
	چاپ_کړه("Setting block %u, was %s before\n", بلاک, op_پایله ?
	       "set" : "clear");
	إذا (!test_پایله != !op_پایله)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_پایله, op_پایله);
}

فراغ do_clearb(صحيح argc, ss_argv_t argv,
	       صحيح sci_idx EXT2FS_ATTR((ناکارول_شوی)),
	       فراغ *infop EXT2FS_ATTR((ناکارول_شوی)))
{
	بینښه صحيح بلاک, عدد;
	صحيح err;
	صحيح test_پایله, op_پایله;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		أرجع;
	}

	بلاک = parse_ulong(argv[1], argv[0], "block", &err);
	إذا (err)
		أرجع;

	إذا (argc == 3) {
		عدد = parse_ulong(argv[2], argv[0], "num", &err);
		إذا (err)
			أرجع;

		ext2fs_unmark_بلاک_bitmap_range2(test_fs->بلاک_map,
						بلاک, عدد);
		چاپ_کړه("Clearing blocks %u to %u\n", بلاک, بلاک + عدد - 1);
		أرجع;
	}

	test_پایله = ext2fs_test_بلاک_bitmap2(test_fs->بلاک_map, بلاک);
	op_پایله = ext2fs_unmark_بلاک_bitmap2(test_fs->بلاک_map, بلاک);
	چاپ_کړه("Clearing block %u, was %s before\n", بلاک, op_پایله ?
	       "set" : "clear");
	إذا (!test_پایله != !op_پایله)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_پایله, op_پایله);
}

فراغ do_testb(صحيح argc, ss_argv_t argv,
	      صحيح sci_idx EXT2FS_ATTR((ناکارول_شوی)),
	      فراغ *infop EXT2FS_ATTR((ناکارول_شوی)))
{
	بینښه صحيح بلاک, عدد;
	صحيح err;
	صحيح test_پایله;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		أرجع;
	}

	بلاک = parse_ulong(argv[1], argv[0], "block", &err);
	إذا (err)
		أرجع;

	إذا (argc == 3) {
		عدد = parse_ulong(argv[2], argv[0], "num", &err);
		إذا (err)
			أرجع;

		test_پایله =
			ext2fs_test_بلاک_bitmap_range2(test_fs->بلاک_map,
							بلاک, عدد);
		چاپ_کړه("Blocks %u to %u are %sall clear.\n",
		       بلاک, بلاک + عدد - 1, test_پایله ? "" : "NOT ");
		أرجع;
	}

	test_پایله = ext2fs_test_بلاک_bitmap2(test_fs->بلاک_map, بلاک);
	چاپ_کړه("Block %u is %s\n", بلاک, test_پایله ? "set" : "clear");
}

فراغ do_ffzb(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((ناکارول_شوی)),
	     فراغ *infop EXT2FS_ATTR((ناکارول_شوی)))
{
	بینښه صحيح پیل, پای_ټکی;
	صحيح err;
	errcode_t retval;
	blk64_t بهر;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		أرجع;
	}

	پیل = parse_ulong(argv[1], argv[0], "start", &err);
	إذا (err)
		أرجع;

	پای_ټکی = parse_ulong(argv[2], argv[0], "end", &err);
	إذا (err)
		أرجع;

	retval = ext2fs_پیدا_کړه_لومړی_zero_بلاک_bitmap2(test_fs->بلاک_map,
						      پیل, پای_ټکی, &بهر);
	إذا (retval) {
		چاپ_کړه("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_پیغام(retval));
		أرجع;
	}
	چاپ_کړه("First unmarked block is %llu\n", (بینښه اوږد اوږد) بهر);
}

فراغ do_ffsb(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((ناکارول_شوی)),
	     فراغ *infop EXT2FS_ATTR((ناکارول_شوی)))
{
	بینښه صحيح پیل, پای_ټکی;
	صحيح err;
	errcode_t retval;
	blk64_t بهر;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		أرجع;
	}

	پیل = parse_ulong(argv[1], argv[0], "start", &err);
	إذا (err)
		أرجع;

	پای_ټکی = parse_ulong(argv[2], argv[0], "end", &err);
	إذا (err)
		أرجع;

	retval = ext2fs_پیدا_کړه_لومړی_سېټ_بلاک_bitmap2(test_fs->بلاک_map,
						      پیل, پای_ټکی, &بهر);
	إذا (retval) {
		چاپ_کړه("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_پیغام(retval));
		أرجع;
	}
	چاپ_کړه("First marked block is %llu\n", (بینښه اوږد اوږد) بهر);
}


فراغ do_zerob(صحيح argc, ss_argv_t argv,
	      صحيح sci_idx EXT2FS_ATTR((ناکارول_شوی)),
	      فراغ *infop EXT2FS_ATTR((ناکارول_شوی)))
{
	إذا (check_fs_open(argv[0]))
		أرجع;

	چاپ_کړه("Clearing block bitmap.\n");
	ext2fs_clear_بلاک_bitmap(test_fs->بلاک_map);
}

فراغ do_seti(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((ناکارول_شوی)),
	     فراغ *infop EXT2FS_ATTR((ناکارول_شوی)))
{
	بینښه صحيح inode;
	صحيح err;
	صحيح test_پایله, op_پایله;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		أرجع;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	إذا (err)
		أرجع;

	test_پایله = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_پایله = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	چاپ_کړه("Setting inode %u, was %s before\n", inode, op_پایله ?
	       "set" : "clear");
	إذا (!test_پایله != !op_پایله) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_پایله, op_پایله);
		exit_حالت++;
	}
}

فراغ do_cleari(صحيح argc, ss_argv_t argv,
	       صحيح sci_idx EXT2FS_ATTR((ناکارول_شوی)),
	       فراغ *infop EXT2FS_ATTR((ناکارول_شوی)))
{
	بینښه صحيح inode;
	صحيح err;
	صحيح test_پایله, op_پایله;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		أرجع;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	إذا (err)
		أرجع;

	test_پایله = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_پایله = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	چاپ_کړه("Clearing inode %u, was %s before\n", inode, op_پایله ?
	       "set" : "clear");
	إذا (!test_پایله != !op_پایله) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_پایله, op_پایله);
		exit_حالت++;
	}
}

فراغ do_testi(صحيح argc, ss_argv_t argv,
	      صحيح sci_idx EXT2FS_ATTR((ناکارول_شوی)),
	      فراغ *infop EXT2FS_ATTR((ناکارول_شوی)))
{
	بینښه صحيح inode;
	صحيح err;
	صحيح test_پایله;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		أرجع;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	إذا (err)
		أرجع;

	test_پایله = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	چاپ_کړه("Inode %u is %s\n", inode, test_پایله ? "set" : "clear");
}

فراغ do_ffzi(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((ناکارول_شوی)),
	     فراغ *infop EXT2FS_ATTR((ناکارول_شوی)))
{
	بینښه صحيح پیل, پای_ټکی;
	صحيح err;
	errcode_t retval;
	ext2_ino_t بهر;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		أرجع;
	}

	پیل = parse_ulong(argv[1], argv[0], "start", &err);
	إذا (err)
		أرجع;

	پای_ټکی = parse_ulong(argv[2], argv[0], "end", &err);
	إذا (err)
		أرجع;

	retval = ext2fs_پیدا_کړه_لومړی_zero_inode_bitmap2(test_fs->inode_map,
						      پیل, پای_ټکی, &بهر);
	إذا (retval) {
		چاپ_کړه("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_پیغام(retval));
		أرجع;
	}
	چاپ_کړه("First unmarked inode is %u\n", بهر);
}

فراغ do_ffsi(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((ناکارول_شوی)),
	     فراغ *infop EXT2FS_ATTR((ناکارول_شوی)))
{
	بینښه صحيح پیل, پای_ټکی;
	صحيح err;
	errcode_t retval;
	ext2_ino_t بهر;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		أرجع;
	}

	پیل = parse_ulong(argv[1], argv[0], "start", &err);
	إذا (err)
		أرجع;

	پای_ټکی = parse_ulong(argv[2], argv[0], "end", &err);
	إذا (err)
		أرجع;

	retval = ext2fs_پیدا_کړه_لومړی_سېټ_inode_bitmap2(test_fs->inode_map,
						     پیل, پای_ټکی, &بهر);
	إذا (retval) {
		چاپ_کړه("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_پیغام(retval));
		أرجع;
	}
	چاپ_کړه("First marked inode is %u\n", بهر);
}

فراغ do_zeroi(صحيح argc, ss_argv_t argv,
	      صحيح sci_idx EXT2FS_ATTR((ناکارول_شوی)),
	      فراغ *infop EXT2FS_ATTR((ناکارول_شوی)))
{
	إذا (check_fs_open(argv[0]))
		أرجع;

	چاپ_کړه("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

صحيح اصلي(صحيح argc, حرف **argv)
{
	بینښه صحيح	بلاکونه = 128;
	بینښه صحيح	inodes = 0;
	بینښه صحيح	type = EXT2FS_BMAP64_BITARRAY;
	صحيح		c, err, کوډ;
	حرف		*request = (حرف *)عدم;
	حرف		*cmd_file = 0;
	صحيح		sci_idx;
	صحيح		flags = EXT2_FLAG_64_بټونه;

	جمع_کړه_error_جدول(&et_ss_error_جدول);
	جمع_کړه_error_جدول(&et_ext2_error_جدول);
	طالما ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		بدل (c) {
		حالة 'b':
			بلاکونه = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			إذا (err)
				وتل(1);
			اكسر;
		حالة 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			إذا (err)
				وتل(1);
			اكسر;
		حالة 'l':	/* Legacy bitmaps */
			flags = 0;
			اكسر;
		حالة 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			إذا (err)
				وتل(1);
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
			وتل(1);
		}
	}

	sci_idx = ss_create_invocation(subsystem_name, نسخه,
				       (حرف *)عدم, &tst_bitmaps_cmds, &کوډ);
	إذا (کوډ) {
		ss_perror(sci_idx, کوډ, "creating invocation");
		وتل(1);
	}

	(فراغ) ss_جمع_کړه_request_جدول (sci_idx, &ss_std_غوښتنې, 1, &کوډ);
	إذا (کوډ) {
		ss_perror(sci_idx, کوډ, "adding standard requests");
		وتل (1);
	}

	چاپ_کړه("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, نسخه);

	تیاری_filesystem(argv[0], بلاکونه, inodes, type, flags);

	إذا (request) {
		کوډ = ss_execute_کرښه(sci_idx, request);
		إذا (کوډ) {
			ss_perror(sci_idx, کوډ, request);
			exit_حالت++;
		}
	} وإلا إذا (cmd_file) {
		exit_حالت = سرچینه_file(cmd_file, sci_idx);
	} وإلا {
		ss_listen(sci_idx);
	}

	وتل(exit_حالت);
}

