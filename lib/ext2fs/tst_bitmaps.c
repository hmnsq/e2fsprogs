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
#اگر_تعریف_شده HAVE_GETOPT_H
#ضمن <getopt.h>
#پایان_اگر
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
صحيح		exit_وضعیت = 0;

ساكن صحيح منبع_file(ثابت حرف *cmd_file, صحيح sci_idx)
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
			خروج(1);
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
			چاپ_کن("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_سطر(sci_idx, buf);
		إذا (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_وضعیت++;
		}
	}
	أرجع exit_وضعیت;
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
#پایان_اگر
#اگر_تعریف_شده HAVE_OPTRESET
	optreset = 1;		/* Makes BSD getopt happy */
#پایان_اگر
}

/*
 * This function will convert a string to an unsigned long, printing
 * an error message if it fails, and returning success or failure in err.
 */
بیعلامت دراز parse_ulong(ثابت حرف *str, ثابت حرف *cmd,
			  ثابت حرف *descr, صحيح *err)
{
	حرف		*tmp;
	بیعلامت دراز	ret;

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
		خروج(1);
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

ساكن فراغ راهاندازی_filesystem(ثابت حرف *name,
			     بیعلامت صحيح بلوکها, بیعلامت صحيح inodes,
			     بیعلامت صحيح type, صحيح flags)
{
	بنية ext2_super_بلوک param;
	errcode_t retval;

	memset(&param, 0, حجم(param));
	ext2fs_بلوکها_شمار_مجموعه(&param, بلوکها);
	param.s_inodes_شمار = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	إذا (retval) {
		com_err(name, retval, "while initializing filesystem");
		أرجع;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_بلوک_bitmap(test_fs->بلوک_map);
	test_fs->بلوک_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_بلوک_bitmap(test_fs, "block bitmap",
					      &test_fs->بلوک_map);
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

فراغ راهاندازی_cmd(صحيح argc, ss_argv_t argv,
	       صحيح sci_idx EXT2FS_ATTR((استفادهنشده)),
	       فراغ *infop EXT2FS_ATTR((استفادهنشده)))
{
	صحيح		c, err;
	بیعلامت صحيح	بلوکها = 128;
	بیعلامت صحيح	inodes = 0;
	بیعلامت صحيح	type = EXT2FS_BMAP64_BITARRAY;
	صحيح		flags = EXT2_FLAG_64_بیتها;

	إذا (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	طالما ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		بدل (c) {
		حالة 'b':
			بلوکها = parse_ulong(optarg, argv[0],
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
	راهاندازی_filesystem(argv[0], بلوکها, inodes, type, flags);
}

فراغ close_cmd(صحيح argc, حرف **argv, صحيح sci_idx EXT2FS_ATTR((استفادهنشده)),
	       فراغ *infop EXT2FS_ATTR((استفادهنشده)))
{
	إذا (check_fs_open(argv[0]))
		أرجع;

	ext2fs_close_free(&test_fs);
}


فراغ dump_bitmap(ext2fs_generic_bitmap bmap, بیعلامت صحيح شروع, بیعلامت رقم)
{
	بیعلامت حرف	*buf;
	errcode_t	retval;
	صحيح		i, len = (رقم - شروع + 7) / 8;

	buf = حافظه_بده(len);
	إذا (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		أرجع;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) شروع, رقم, buf);
	إذا (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		حرر(buf);
		أرجع;
	}
	لكل (i=0; i < len; i++)
		چاپ_کن("%02x", buf[i]);
	چاپ_کن("\n");
	چاپ_کن("bits set: %u\n", ext2fs_bitcount(buf, len));
	حرر(buf);
}

فراغ dump_inode_bitmap_cmd(صحيح argc, ss_argv_t argv,
			   صحيح sci_idx EXT2FS_ATTR((استفادهنشده)),
			   فراغ *infop EXT2FS_ATTR((استفادهنشده)))
{
	إذا (check_fs_open(argv[0]))
		أرجع;

	چاپ_کن("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_شمار);
}
	
فراغ dump_بلوک_bitmap_cmd(صحيح argc, ss_argv_t argv,
			   صحيح sci_idx EXT2FS_ATTR((استفادهنشده)),
			   فراغ *infop EXT2FS_ATTR((استفادهنشده)))
{
	إذا (check_fs_open(argv[0]))
		أرجع;

	چاپ_کن("block bitmap: ");
	dump_bitmap(test_fs->بلوک_map, test_fs->super->s_اولی_داده_بلوک,
		    test_fs->super->s_بلوکها_شمار);
}
	
فراغ do_setb(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((استفادهنشده)),
	     فراغ *infop EXT2FS_ATTR((استفادهنشده)))
{
	بیعلامت صحيح بلوک, رقم;
	صحيح err;
	صحيح test_نتیجه, op_نتیجه;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		أرجع;
	}

	بلوک = parse_ulong(argv[1], argv[0], "block", &err);
	إذا (err)
		أرجع;

	إذا (argc == 3) {
		رقم = parse_ulong(argv[2], argv[0], "num", &err);
		إذا (err)
			أرجع;

		ext2fs_mark_بلوک_bitmap_range2(test_fs->بلوک_map,
						بلوک, رقم);
		چاپ_کن("Marking blocks %u to %u\n", بلوک, بلوک + رقم - 1);
		أرجع;
	}

	test_نتیجه = ext2fs_test_بلوک_bitmap2(test_fs->بلوک_map, بلوک);
	op_نتیجه = ext2fs_mark_بلوک_bitmap2(test_fs->بلوک_map, بلوک);
	چاپ_کن("Setting block %u, was %s before\n", بلوک, op_نتیجه ?
	       "set" : "clear");
	إذا (!test_نتیجه != !op_نتیجه)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_نتیجه, op_نتیجه);
}

فراغ do_clearb(صحيح argc, ss_argv_t argv,
	       صحيح sci_idx EXT2FS_ATTR((استفادهنشده)),
	       فراغ *infop EXT2FS_ATTR((استفادهنشده)))
{
	بیعلامت صحيح بلوک, رقم;
	صحيح err;
	صحيح test_نتیجه, op_نتیجه;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		أرجع;
	}

	بلوک = parse_ulong(argv[1], argv[0], "block", &err);
	إذا (err)
		أرجع;

	إذا (argc == 3) {
		رقم = parse_ulong(argv[2], argv[0], "num", &err);
		إذا (err)
			أرجع;

		ext2fs_unmark_بلوک_bitmap_range2(test_fs->بلوک_map,
						بلوک, رقم);
		چاپ_کن("Clearing blocks %u to %u\n", بلوک, بلوک + رقم - 1);
		أرجع;
	}

	test_نتیجه = ext2fs_test_بلوک_bitmap2(test_fs->بلوک_map, بلوک);
	op_نتیجه = ext2fs_unmark_بلوک_bitmap2(test_fs->بلوک_map, بلوک);
	چاپ_کن("Clearing block %u, was %s before\n", بلوک, op_نتیجه ?
	       "set" : "clear");
	إذا (!test_نتیجه != !op_نتیجه)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_نتیجه, op_نتیجه);
}

فراغ do_testb(صحيح argc, ss_argv_t argv,
	      صحيح sci_idx EXT2FS_ATTR((استفادهنشده)),
	      فراغ *infop EXT2FS_ATTR((استفادهنشده)))
{
	بیعلامت صحيح بلوک, رقم;
	صحيح err;
	صحيح test_نتیجه;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		أرجع;
	}

	بلوک = parse_ulong(argv[1], argv[0], "block", &err);
	إذا (err)
		أرجع;

	إذا (argc == 3) {
		رقم = parse_ulong(argv[2], argv[0], "num", &err);
		إذا (err)
			أرجع;

		test_نتیجه =
			ext2fs_test_بلوک_bitmap_range2(test_fs->بلوک_map,
							بلوک, رقم);
		چاپ_کن("Blocks %u to %u are %sall clear.\n",
		       بلوک, بلوک + رقم - 1, test_نتیجه ? "" : "NOT ");
		أرجع;
	}

	test_نتیجه = ext2fs_test_بلوک_bitmap2(test_fs->بلوک_map, بلوک);
	چاپ_کن("Block %u is %s\n", بلوک, test_نتیجه ? "set" : "clear");
}

فراغ do_ffzb(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((استفادهنشده)),
	     فراغ *infop EXT2FS_ATTR((استفادهنشده)))
{
	بیعلامت صحيح شروع, انتها;
	صحيح err;
	errcode_t retval;
	blk64_t بیرون;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		أرجع;
	}

	شروع = parse_ulong(argv[1], argv[0], "start", &err);
	إذا (err)
		أرجع;

	انتها = parse_ulong(argv[2], argv[0], "end", &err);
	إذا (err)
		أرجع;

	retval = ext2fs_پیدا_کن_اولی_zero_بلوک_bitmap2(test_fs->بلوک_map,
						      شروع, انتها, &بیرون);
	إذا (retval) {
		چاپ_کن("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_پیام(retval));
		أرجع;
	}
	چاپ_کن("First unmarked block is %llu\n", (بیعلامت دراز دراز) بیرون);
}

فراغ do_ffsb(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((استفادهنشده)),
	     فراغ *infop EXT2FS_ATTR((استفادهنشده)))
{
	بیعلامت صحيح شروع, انتها;
	صحيح err;
	errcode_t retval;
	blk64_t بیرون;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		أرجع;
	}

	شروع = parse_ulong(argv[1], argv[0], "start", &err);
	إذا (err)
		أرجع;

	انتها = parse_ulong(argv[2], argv[0], "end", &err);
	إذا (err)
		أرجع;

	retval = ext2fs_پیدا_کن_اولی_مجموعه_بلوک_bitmap2(test_fs->بلوک_map,
						      شروع, انتها, &بیرون);
	إذا (retval) {
		چاپ_کن("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_پیام(retval));
		أرجع;
	}
	چاپ_کن("First marked block is %llu\n", (بیعلامت دراز دراز) بیرون);
}


فراغ do_zerob(صحيح argc, ss_argv_t argv,
	      صحيح sci_idx EXT2FS_ATTR((استفادهنشده)),
	      فراغ *infop EXT2FS_ATTR((استفادهنشده)))
{
	إذا (check_fs_open(argv[0]))
		أرجع;

	چاپ_کن("Clearing block bitmap.\n");
	ext2fs_clear_بلوک_bitmap(test_fs->بلوک_map);
}

فراغ do_seti(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((استفادهنشده)),
	     فراغ *infop EXT2FS_ATTR((استفادهنشده)))
{
	بیعلامت صحيح inode;
	صحيح err;
	صحيح test_نتیجه, op_نتیجه;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		أرجع;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	إذا (err)
		أرجع;

	test_نتیجه = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_نتیجه = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	چاپ_کن("Setting inode %u, was %s before\n", inode, op_نتیجه ?
	       "set" : "clear");
	إذا (!test_نتیجه != !op_نتیجه) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_نتیجه, op_نتیجه);
		exit_وضعیت++;
	}
}

فراغ do_cleari(صحيح argc, ss_argv_t argv,
	       صحيح sci_idx EXT2FS_ATTR((استفادهنشده)),
	       فراغ *infop EXT2FS_ATTR((استفادهنشده)))
{
	بیعلامت صحيح inode;
	صحيح err;
	صحيح test_نتیجه, op_نتیجه;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		أرجع;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	إذا (err)
		أرجع;

	test_نتیجه = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_نتیجه = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	چاپ_کن("Clearing inode %u, was %s before\n", inode, op_نتیجه ?
	       "set" : "clear");
	إذا (!test_نتیجه != !op_نتیجه) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_نتیجه, op_نتیجه);
		exit_وضعیت++;
	}
}

فراغ do_testi(صحيح argc, ss_argv_t argv,
	      صحيح sci_idx EXT2FS_ATTR((استفادهنشده)),
	      فراغ *infop EXT2FS_ATTR((استفادهنشده)))
{
	بیعلامت صحيح inode;
	صحيح err;
	صحيح test_نتیجه;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		أرجع;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	إذا (err)
		أرجع;

	test_نتیجه = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	چاپ_کن("Inode %u is %s\n", inode, test_نتیجه ? "set" : "clear");
}

فراغ do_ffzi(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((استفادهنشده)),
	     فراغ *infop EXT2FS_ATTR((استفادهنشده)))
{
	بیعلامت صحيح شروع, انتها;
	صحيح err;
	errcode_t retval;
	ext2_ino_t بیرون;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		أرجع;
	}

	شروع = parse_ulong(argv[1], argv[0], "start", &err);
	إذا (err)
		أرجع;

	انتها = parse_ulong(argv[2], argv[0], "end", &err);
	إذا (err)
		أرجع;

	retval = ext2fs_پیدا_کن_اولی_zero_inode_bitmap2(test_fs->inode_map,
						      شروع, انتها, &بیرون);
	إذا (retval) {
		چاپ_کن("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_پیام(retval));
		أرجع;
	}
	چاپ_کن("First unmarked inode is %u\n", بیرون);
}

فراغ do_ffsi(صحيح argc, ss_argv_t argv,
	     صحيح sci_idx EXT2FS_ATTR((استفادهنشده)),
	     فراغ *infop EXT2FS_ATTR((استفادهنشده)))
{
	بیعلامت صحيح شروع, انتها;
	صحيح err;
	errcode_t retval;
	ext2_ino_t بیرون;

	إذا (check_fs_open(argv[0]))
		أرجع;

	إذا (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		أرجع;
	}

	شروع = parse_ulong(argv[1], argv[0], "start", &err);
	إذا (err)
		أرجع;

	انتها = parse_ulong(argv[2], argv[0], "end", &err);
	إذا (err)
		أرجع;

	retval = ext2fs_پیدا_کن_اولی_مجموعه_inode_bitmap2(test_fs->inode_map,
						     شروع, انتها, &بیرون);
	إذا (retval) {
		چاپ_کن("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_پیام(retval));
		أرجع;
	}
	چاپ_کن("First marked inode is %u\n", بیرون);
}

فراغ do_zeroi(صحيح argc, ss_argv_t argv,
	      صحيح sci_idx EXT2FS_ATTR((استفادهنشده)),
	      فراغ *infop EXT2FS_ATTR((استفادهنشده)))
{
	إذا (check_fs_open(argv[0]))
		أرجع;

	چاپ_کن("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

صحيح اصلی(صحيح argc, حرف **argv)
{
	بیعلامت صحيح	بلوکها = 128;
	بیعلامت صحيح	inodes = 0;
	بیعلامت صحيح	type = EXT2FS_BMAP64_BITARRAY;
	صحيح		c, err, کد;
	حرف		*request = (حرف *)عدم;
	حرف		*cmd_file = 0;
	صحيح		sci_idx;
	صحيح		flags = EXT2_FLAG_64_بیتها;

	جمع_کن_error_جدول(&et_ss_error_جدول);
	جمع_کن_error_جدول(&et_ext2_error_جدول);
	طالما ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		بدل (c) {
		حالة 'b':
			بلوکها = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			إذا (err)
				خروج(1);
			اكسر;
		حالة 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			إذا (err)
				خروج(1);
			اكسر;
		حالة 'l':	/* Legacy bitmaps */
			flags = 0;
			اكسر;
		حالة 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			إذا (err)
				خروج(1);
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
			خروج(1);
		}
	}

	sci_idx = ss_create_invocation(subsystem_name, نسخه,
				       (حرف *)عدم, &tst_bitmaps_cmds, &کد);
	إذا (کد) {
		ss_perror(sci_idx, کد, "creating invocation");
		خروج(1);
	}

	(فراغ) ss_جمع_کن_request_جدول (sci_idx, &ss_std_درخواستها, 1, &کد);
	إذا (کد) {
		ss_perror(sci_idx, کد, "adding standard requests");
		خروج (1);
	}

	چاپ_کن("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, نسخه);

	راهاندازی_filesystem(argv[0], بلوکها, inodes, type, flags);

	إذا (request) {
		کد = ss_execute_سطر(sci_idx, request);
		إذا (کد) {
			ss_perror(sci_idx, کد, request);
			exit_وضعیت++;
		}
	} وإلا إذا (cmd_file) {
		exit_وضعیت = منبع_file(cmd_file, sci_idx);
	} وإلا {
		ss_listen(sci_idx);
	}

	خروج(exit_وضعیت);
}

