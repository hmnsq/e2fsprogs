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

#شامل "config.h"
#شامل <unistd.h>
#شامل <stdlib.h>
#شامل <stdio.h>
#اگر_تعریف_شدہ HAVE_GETOPT_H
#شامل <getopt.h>
#اگر_ختم
#شامل <string.h>
#شامل <fcntl.h>
#شامل <time.h>
#شامل <sys/stat.h>
#شامل <sys/types.h>
#شامل "ss/ss.h"

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

خارجی ss_request_جدول tst_bitmaps_cmds;

ساکن حرف subsystem_name[] = "tst_bitmaps";
ساکن حرف نسخہ[] = "1.0";

ext2_filsys	test_fs;
صحیح		exit_حالت = 0;

ساکن صحیح ماخذ_file(ثابت حرف *cmd_file, صحیح sci_idx)
{
	FILE		*f;
	حرف		buf[256];
	حرف		*cp;
	صحیح		retval;
	صحیح 		noecho;

	اگر (strcmp(cmd_file, "-") == 0)
		f = stdin;
	ورنہ {
		f = fopen(cmd_file, "r");
		اگر (!f) {
			perror(cmd_file);
			باہر_نکلو(1);
		}
	}
	fflush(stdout);
	fflush(stderr);
	setbuf(stdout, تہی);
	setbuf(stderr, تہی);
	جب (!feof(f)) {
		اگر (fgets(buf, حجم(buf), f) == تہی)
			توڑو;
		اگر (buf[0] == '#')
			جاری;
		noecho = 0;
		اگر (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = strchr(buf, '\n');
		اگر (cp)
			*cp = 0;
		cp = strchr(buf, '\r');
		اگر (cp)
			*cp = 0;
		اگر (!noecho)
			لکھو("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_سطر(sci_idx, buf);
		اگر (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_حالت++;
		}
	}
	واپس exit_حالت;
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
خالی reset_getopt(خالی)
{
#اگر defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#ورنہ
	optind = 1;
#اگر_ختم
#اگر_تعریف_شدہ HAVE_OPTRESET
	optreset = 1;		/* Makes BSD getopt happy */
#اگر_ختم
}

/*
 * This function will convert a string to an unsigned long, printing
 * an error message if it fails, and returning success or failure in err.
 */
غیرعلامتی طویل parse_ulong(ثابت حرف *str, ثابت حرف *cmd,
			  ثابت حرف *descr, صحیح *err)
{
	حرف		*tmp;
	غیرعلامتی طویل	ret;

	ret = strtoul(str, &tmp, 0);
	اگر (*tmp == 0) {
		اگر (err)
			*err = 0;
		واپس ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	اگر (err)
		*err = 1;
	ورنہ
		باہر_نکلو(1);
	واپس 0;
}


صحیح check_fs_open(ثابت حرف *name)
{
	اگر (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		واپس 1;
	}
	واپس 0;
}

ساکن خالی تیاری_filesystem(ثابت حرف *name,
			     غیرعلامتی صحیح بلاکیں, غیرعلامتی صحیح inodes,
			     غیرعلامتی صحیح type, صحیح flags)
{
	ساخت ext2_super_بلاک param;
	errcode_t retval;

	memset(&param, 0, حجم(param));
	ext2fs_بلاکیں_گنتی_سیٹ(&param, بلاکیں);
	param.s_inodes_گنتی = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	اگر (retval) {
		com_err(name, retval, "while initializing filesystem");
		واپس;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_بلاک_bitmap(test_fs->بلاک_map);
	test_fs->بلاک_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_بلاک_bitmap(test_fs, "block bitmap",
					      &test_fs->بلاک_map);
	اگر (retval) {
		com_err(name, retval, "while allocating block bitmap");
		جاؤ errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	اگر (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		جاؤ errout;
	}
	واپس;

errout:
	ext2fs_close_free(&test_fs);
}

خالی تیاری_cmd(صحیح argc, ss_argv_t argv,
	       صحیح sci_idx EXT2FS_ATTR((غیر_مستعمل)),
	       خالی *infop EXT2FS_ATTR((غیر_مستعمل)))
{
	صحیح		c, err;
	غیرعلامتی صحیح	بلاکیں = 128;
	غیرعلامتی صحیح	inodes = 0;
	غیرعلامتی صحیح	type = EXT2FS_BMAP64_BITARRAY;
	صحیح		flags = EXT2_FLAG_64_بٹ;

	اگر (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	جب ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		انتخاب (c) {
		حالت 'b':
			بلاکیں = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			اگر (err)
				واپس;
			توڑو;
		حالت 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			اگر (err)
				واپس;
			توڑو;
		حالت 'l':	/* Legacy bitmaps */
			flags = 0;
			توڑو;
		حالت 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			اگر (err)
				واپس;
			توڑو;
		طےشدہ:
			fprintf(stderr, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			واپس;
		}
	}
	تیاری_filesystem(argv[0], بلاکیں, inodes, type, flags);
}

خالی close_cmd(صحیح argc, حرف **argv, صحیح sci_idx EXT2FS_ATTR((غیر_مستعمل)),
	       خالی *infop EXT2FS_ATTR((غیر_مستعمل)))
{
	اگر (check_fs_open(argv[0]))
		واپس;

	ext2fs_close_free(&test_fs);
}


خالی dump_bitmap(ext2fs_generic_bitmap bmap, غیرعلامتی صحیح آغاز, غیرعلامتی عدد)
{
	غیرعلامتی حرف	*buf;
	errcode_t	retval;
	صحیح		i, len = (عدد - آغاز + 7) / 8;

	buf = حافظہ_دو(len);
	اگر (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		واپس;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) آغاز, عدد, buf);
	اگر (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		آزاد(buf);
		واپس;
	}
	برائے (i=0; i < len; i++)
		لکھو("%02x", buf[i]);
	لکھو("\n");
	لکھو("bits set: %u\n", ext2fs_bitcount(buf, len));
	آزاد(buf);
}

خالی dump_inode_bitmap_cmd(صحیح argc, ss_argv_t argv,
			   صحیح sci_idx EXT2FS_ATTR((غیر_مستعمل)),
			   خالی *infop EXT2FS_ATTR((غیر_مستعمل)))
{
	اگر (check_fs_open(argv[0]))
		واپس;

	لکھو("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_گنتی);
}
	
خالی dump_بلاک_bitmap_cmd(صحیح argc, ss_argv_t argv,
			   صحیح sci_idx EXT2FS_ATTR((غیر_مستعمل)),
			   خالی *infop EXT2FS_ATTR((غیر_مستعمل)))
{
	اگر (check_fs_open(argv[0]))
		واپس;

	لکھو("block bitmap: ");
	dump_bitmap(test_fs->بلاک_map, test_fs->super->s_پہلا_ڈیٹا_بلاک,
		    test_fs->super->s_بلاکیں_گنتی);
}
	
خالی do_setb(صحیح argc, ss_argv_t argv,
	     صحیح sci_idx EXT2FS_ATTR((غیر_مستعمل)),
	     خالی *infop EXT2FS_ATTR((غیر_مستعمل)))
{
	غیرعلامتی صحیح بلاک, عدد;
	صحیح err;
	صحیح test_نتیجہ, op_نتیجہ;

	اگر (check_fs_open(argv[0]))
		واپس;

	اگر (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		واپس;
	}

	بلاک = parse_ulong(argv[1], argv[0], "block", &err);
	اگر (err)
		واپس;

	اگر (argc == 3) {
		عدد = parse_ulong(argv[2], argv[0], "num", &err);
		اگر (err)
			واپس;

		ext2fs_mark_بلاک_bitmap_range2(test_fs->بلاک_map,
						بلاک, عدد);
		لکھو("Marking blocks %u to %u\n", بلاک, بلاک + عدد - 1);
		واپس;
	}

	test_نتیجہ = ext2fs_test_بلاک_bitmap2(test_fs->بلاک_map, بلاک);
	op_نتیجہ = ext2fs_mark_بلاک_bitmap2(test_fs->بلاک_map, بلاک);
	لکھو("Setting block %u, was %s before\n", بلاک, op_نتیجہ ?
	       "set" : "clear");
	اگر (!test_نتیجہ != !op_نتیجہ)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_نتیجہ, op_نتیجہ);
}

خالی do_clearb(صحیح argc, ss_argv_t argv,
	       صحیح sci_idx EXT2FS_ATTR((غیر_مستعمل)),
	       خالی *infop EXT2FS_ATTR((غیر_مستعمل)))
{
	غیرعلامتی صحیح بلاک, عدد;
	صحیح err;
	صحیح test_نتیجہ, op_نتیجہ;

	اگر (check_fs_open(argv[0]))
		واپس;

	اگر (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		واپس;
	}

	بلاک = parse_ulong(argv[1], argv[0], "block", &err);
	اگر (err)
		واپس;

	اگر (argc == 3) {
		عدد = parse_ulong(argv[2], argv[0], "num", &err);
		اگر (err)
			واپس;

		ext2fs_unmark_بلاک_bitmap_range2(test_fs->بلاک_map,
						بلاک, عدد);
		لکھو("Clearing blocks %u to %u\n", بلاک, بلاک + عدد - 1);
		واپس;
	}

	test_نتیجہ = ext2fs_test_بلاک_bitmap2(test_fs->بلاک_map, بلاک);
	op_نتیجہ = ext2fs_unmark_بلاک_bitmap2(test_fs->بلاک_map, بلاک);
	لکھو("Clearing block %u, was %s before\n", بلاک, op_نتیجہ ?
	       "set" : "clear");
	اگر (!test_نتیجہ != !op_نتیجہ)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_نتیجہ, op_نتیجہ);
}

خالی do_testb(صحیح argc, ss_argv_t argv,
	      صحیح sci_idx EXT2FS_ATTR((غیر_مستعمل)),
	      خالی *infop EXT2FS_ATTR((غیر_مستعمل)))
{
	غیرعلامتی صحیح بلاک, عدد;
	صحیح err;
	صحیح test_نتیجہ;

	اگر (check_fs_open(argv[0]))
		واپس;

	اگر (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		واپس;
	}

	بلاک = parse_ulong(argv[1], argv[0], "block", &err);
	اگر (err)
		واپس;

	اگر (argc == 3) {
		عدد = parse_ulong(argv[2], argv[0], "num", &err);
		اگر (err)
			واپس;

		test_نتیجہ =
			ext2fs_test_بلاک_bitmap_range2(test_fs->بلاک_map,
							بلاک, عدد);
		لکھو("Blocks %u to %u are %sall clear.\n",
		       بلاک, بلاک + عدد - 1, test_نتیجہ ? "" : "NOT ");
		واپس;
	}

	test_نتیجہ = ext2fs_test_بلاک_bitmap2(test_fs->بلاک_map, بلاک);
	لکھو("Block %u is %s\n", بلاک, test_نتیجہ ? "set" : "clear");
}

خالی do_ffzb(صحیح argc, ss_argv_t argv,
	     صحیح sci_idx EXT2FS_ATTR((غیر_مستعمل)),
	     خالی *infop EXT2FS_ATTR((غیر_مستعمل)))
{
	غیرعلامتی صحیح آغاز, انتہا;
	صحیح err;
	errcode_t retval;
	blk64_t برآمد;

	اگر (check_fs_open(argv[0]))
		واپس;

	اگر (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		واپس;
	}

	آغاز = parse_ulong(argv[1], argv[0], "start", &err);
	اگر (err)
		واپس;

	انتہا = parse_ulong(argv[2], argv[0], "end", &err);
	اگر (err)
		واپس;

	retval = ext2fs_ڈھونڈو_پہلا_zero_بلاک_bitmap2(test_fs->بلاک_map,
						      آغاز, انتہا, &برآمد);
	اگر (retval) {
		لکھو("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_پیغام(retval));
		واپس;
	}
	لکھو("First unmarked block is %llu\n", (غیرعلامتی طویل طویل) برآمد);
}

خالی do_ffsb(صحیح argc, ss_argv_t argv,
	     صحیح sci_idx EXT2FS_ATTR((غیر_مستعمل)),
	     خالی *infop EXT2FS_ATTR((غیر_مستعمل)))
{
	غیرعلامتی صحیح آغاز, انتہا;
	صحیح err;
	errcode_t retval;
	blk64_t برآمد;

	اگر (check_fs_open(argv[0]))
		واپس;

	اگر (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		واپس;
	}

	آغاز = parse_ulong(argv[1], argv[0], "start", &err);
	اگر (err)
		واپس;

	انتہا = parse_ulong(argv[2], argv[0], "end", &err);
	اگر (err)
		واپس;

	retval = ext2fs_ڈھونڈو_پہلا_سیٹ_بلاک_bitmap2(test_fs->بلاک_map,
						      آغاز, انتہا, &برآمد);
	اگر (retval) {
		لکھو("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_پیغام(retval));
		واپس;
	}
	لکھو("First marked block is %llu\n", (غیرعلامتی طویل طویل) برآمد);
}


خالی do_zerob(صحیح argc, ss_argv_t argv,
	      صحیح sci_idx EXT2FS_ATTR((غیر_مستعمل)),
	      خالی *infop EXT2FS_ATTR((غیر_مستعمل)))
{
	اگر (check_fs_open(argv[0]))
		واپس;

	لکھو("Clearing block bitmap.\n");
	ext2fs_clear_بلاک_bitmap(test_fs->بلاک_map);
}

خالی do_seti(صحیح argc, ss_argv_t argv,
	     صحیح sci_idx EXT2FS_ATTR((غیر_مستعمل)),
	     خالی *infop EXT2FS_ATTR((غیر_مستعمل)))
{
	غیرعلامتی صحیح inode;
	صحیح err;
	صحیح test_نتیجہ, op_نتیجہ;

	اگر (check_fs_open(argv[0]))
		واپس;

	اگر (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		واپس;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	اگر (err)
		واپس;

	test_نتیجہ = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_نتیجہ = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	لکھو("Setting inode %u, was %s before\n", inode, op_نتیجہ ?
	       "set" : "clear");
	اگر (!test_نتیجہ != !op_نتیجہ) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_نتیجہ, op_نتیجہ);
		exit_حالت++;
	}
}

خالی do_cleari(صحیح argc, ss_argv_t argv,
	       صحیح sci_idx EXT2FS_ATTR((غیر_مستعمل)),
	       خالی *infop EXT2FS_ATTR((غیر_مستعمل)))
{
	غیرعلامتی صحیح inode;
	صحیح err;
	صحیح test_نتیجہ, op_نتیجہ;

	اگر (check_fs_open(argv[0]))
		واپس;

	اگر (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		واپس;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	اگر (err)
		واپس;

	test_نتیجہ = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_نتیجہ = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	لکھو("Clearing inode %u, was %s before\n", inode, op_نتیجہ ?
	       "set" : "clear");
	اگر (!test_نتیجہ != !op_نتیجہ) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_نتیجہ, op_نتیجہ);
		exit_حالت++;
	}
}

خالی do_testi(صحیح argc, ss_argv_t argv,
	      صحیح sci_idx EXT2FS_ATTR((غیر_مستعمل)),
	      خالی *infop EXT2FS_ATTR((غیر_مستعمل)))
{
	غیرعلامتی صحیح inode;
	صحیح err;
	صحیح test_نتیجہ;

	اگر (check_fs_open(argv[0]))
		واپس;

	اگر (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		واپس;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	اگر (err)
		واپس;

	test_نتیجہ = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	لکھو("Inode %u is %s\n", inode, test_نتیجہ ? "set" : "clear");
}

خالی do_ffzi(صحیح argc, ss_argv_t argv,
	     صحیح sci_idx EXT2FS_ATTR((غیر_مستعمل)),
	     خالی *infop EXT2FS_ATTR((غیر_مستعمل)))
{
	غیرعلامتی صحیح آغاز, انتہا;
	صحیح err;
	errcode_t retval;
	ext2_ino_t برآمد;

	اگر (check_fs_open(argv[0]))
		واپس;

	اگر (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		واپس;
	}

	آغاز = parse_ulong(argv[1], argv[0], "start", &err);
	اگر (err)
		واپس;

	انتہا = parse_ulong(argv[2], argv[0], "end", &err);
	اگر (err)
		واپس;

	retval = ext2fs_ڈھونڈو_پہلا_zero_inode_bitmap2(test_fs->inode_map,
						      آغاز, انتہا, &برآمد);
	اگر (retval) {
		لکھو("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_پیغام(retval));
		واپس;
	}
	لکھو("First unmarked inode is %u\n", برآمد);
}

خالی do_ffsi(صحیح argc, ss_argv_t argv,
	     صحیح sci_idx EXT2FS_ATTR((غیر_مستعمل)),
	     خالی *infop EXT2FS_ATTR((غیر_مستعمل)))
{
	غیرعلامتی صحیح آغاز, انتہا;
	صحیح err;
	errcode_t retval;
	ext2_ino_t برآمد;

	اگر (check_fs_open(argv[0]))
		واپس;

	اگر (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		واپس;
	}

	آغاز = parse_ulong(argv[1], argv[0], "start", &err);
	اگر (err)
		واپس;

	انتہا = parse_ulong(argv[2], argv[0], "end", &err);
	اگر (err)
		واپس;

	retval = ext2fs_ڈھونڈو_پہلا_سیٹ_inode_bitmap2(test_fs->inode_map,
						     آغاز, انتہا, &برآمد);
	اگر (retval) {
		لکھو("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_پیغام(retval));
		واپس;
	}
	لکھو("First marked inode is %u\n", برآمد);
}

خالی do_zeroi(صحیح argc, ss_argv_t argv,
	      صحیح sci_idx EXT2FS_ATTR((غیر_مستعمل)),
	      خالی *infop EXT2FS_ATTR((غیر_مستعمل)))
{
	اگر (check_fs_open(argv[0]))
		واپس;

	لکھو("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

صحیح مین(صحیح argc, حرف **argv)
{
	غیرعلامتی صحیح	بلاکیں = 128;
	غیرعلامتی صحیح	inodes = 0;
	غیرعلامتی صحیح	type = EXT2FS_BMAP64_BITARRAY;
	صحیح		c, err, رمز;
	حرف		*request = (حرف *)تہی;
	حرف		*cmd_file = 0;
	صحیح		sci_idx;
	صحیح		flags = EXT2_FLAG_64_بٹ;

	جوڑو_error_جدول(&et_ss_error_جدول);
	جوڑو_error_جدول(&et_ext2_error_جدول);
	جب ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		انتخاب (c) {
		حالت 'b':
			بلاکیں = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			اگر (err)
				باہر_نکلو(1);
			توڑو;
		حالت 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			اگر (err)
				باہر_نکلو(1);
			توڑو;
		حالت 'l':	/* Legacy bitmaps */
			flags = 0;
			توڑو;
		حالت 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			اگر (err)
				باہر_نکلو(1);
			توڑو;
		حالت 'R':
			request = optarg;
			توڑو;
		حالت 'f':
			cmd_file = optarg;
			توڑو;
		طےشدہ:
			com_err(argv[0], 0, "Usage: %s [-R request] "
				"[-f cmd_file]", subsystem_name);
			باہر_نکلو(1);
		}
	}

	sci_idx = ss_create_invocation(subsystem_name, نسخہ,
				       (حرف *)تہی, &tst_bitmaps_cmds, &رمز);
	اگر (رمز) {
		ss_perror(sci_idx, رمز, "creating invocation");
		باہر_نکلو(1);
	}

	(خالی) ss_جوڑو_request_جدول (sci_idx, &ss_std_درخواستیں, 1, &رمز);
	اگر (رمز) {
		ss_perror(sci_idx, رمز, "adding standard requests");
		باہر_نکلو (1);
	}

	لکھو("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, نسخہ);

	تیاری_filesystem(argv[0], بلاکیں, inodes, type, flags);

	اگر (request) {
		رمز = ss_execute_سطر(sci_idx, request);
		اگر (رمز) {
			ss_perror(sci_idx, رمز, request);
			exit_حالت++;
		}
	} ورنہ اگر (cmd_file) {
		exit_حالت = ماخذ_file(cmd_file, sci_idx);
	} ورنہ {
		ss_listen(sci_idx);
	}

	باہر_نکلو(exit_حالت);
}

