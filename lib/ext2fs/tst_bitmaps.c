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

#अन्तर्भाव "config.h"
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdio.h>
#यदि_परिभाषितम् HAVE_GETOPT_H
#अन्तर्भाव <getopt.h>
#यदि_अन्तः
#अन्तर्भाव <string.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव "ss/ss.h"

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

बाह्य ss_request_सारणी tst_bitmaps_cmds;

स्थिर वर्ण subsystem_name[] = "tst_bitmaps";
स्थिर वर्ण संस्करणम्[] = "1.0";

ext2_filsys	test_fs;
पूर्णाङ्क		exit_स्थितिः = 0;

स्थिर पूर्णाङ्क source_file(ध्रुव वर्ण *cmd_file, पूर्णाङ्क sci_idx)
{
	FILE		*f;
	वर्ण		buf[256];
	वर्ण		*cp;
	पूर्णाङ्क		retval;
	पूर्णाङ्क 		noecho;

	यदि (वाक्यं_तुलय(cmd_file, "-") == 0)
		f = stdin;
	अन्यथा {
		f = fopen(cmd_file, "r");
		यदि (!f) {
			perror(cmd_file);
			निर्गच्छ(1);
		}
	}
	fflush(stdout);
	fflush(stderr);
	setbuf(stdout, रिक्त);
	setbuf(stderr, रिक्त);
	यावत् (!feof(f)) {
		यदि (fgets(buf, परिमाण(buf), f) == रिक्त)
			भङ्ग;
		यदि (buf[0] == '#')
			अनुवर्त;
		noecho = 0;
		यदि (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = strchr(buf, '\n');
		यदि (cp)
			*cp = 0;
		cp = strchr(buf, '\r');
		यदि (cp)
			*cp = 0;
		यदि (!noecho)
			प्रारूप_लिख("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_पङ्क्तिः(sci_idx, buf);
		यदि (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_स्थितिः++;
		}
	}
	प्रत्यागम exit_स्थितिः;
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
शून्य reset_getopt(शून्य)
{
#यदि defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#अन्यथा
	optind = 1;
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_OPTRESET
	optreset = 1;		/* Makes BSD getopt happy */
#यदि_अन्तः
}

/*
 * This function will convert a string to an unsigned long, printing
 * an error message if it fails, and returning success or failure in err.
 */
अचिह्नित दीर्घ parse_ulong(ध्रुव वर्ण *str, ध्रुव वर्ण *cmd,
			  ध्रुव वर्ण *descr, पूर्णाङ्क *err)
{
	वर्ण		*tmp;
	अचिह्नित दीर्घ	ret;

	ret = strtoul(str, &tmp, 0);
	यदि (*tmp == 0) {
		यदि (err)
			*err = 0;
		प्रत्यागम ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	यदि (err)
		*err = 1;
	अन्यथा
		निर्गच्छ(1);
	प्रत्यागम 0;
}


पूर्णाङ्क check_fs_open(ध्रुव वर्ण *name)
{
	यदि (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		प्रत्यागम 1;
	}
	प्रत्यागम 0;
}

स्थिर शून्य सज्जता_filesystem(ध्रुव वर्ण *name,
			     अचिह्नित पूर्णाङ्क खण्डानि, अचिह्नित पूर्णाङ्क inodes,
			     अचिह्नित पूर्णाङ्क type, पूर्णाङ्क flags)
{
	संरचना ext2_super_खण्डम् param;
	errcode_t retval;

	स्मृतिं_पूरय(&param, 0, परिमाण(param));
	ext2fs_खण्डानि_गणनम्_समुच्चयः(&param, खण्डानि);
	param.s_inodes_गणनम् = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	यदि (retval) {
		com_err(name, retval, "while initializing filesystem");
		प्रत्यागम;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_खण्डम्_bitmap(test_fs->खण्डम्_map);
	test_fs->खण्डम्_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_खण्डम्_bitmap(test_fs, "block bitmap",
					      &test_fs->खण्डम्_map);
	यदि (retval) {
		com_err(name, retval, "while allocating block bitmap");
		गच्छ errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	यदि (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		गच्छ errout;
	}
	प्रत्यागम;

errout:
	ext2fs_close_free(&test_fs);
}

शून्य सज्जता_cmd(पूर्णाङ्क argc, ss_argv_t argv,
	       पूर्णाङ्क sci_idx EXT2FS_ATTR((अप्रयुक्तम्)),
	       शून्य *infop EXT2FS_ATTR((अप्रयुक्तम्)))
{
	पूर्णाङ्क		c, err;
	अचिह्नित पूर्णाङ्क	खण्डानि = 128;
	अचिह्नित पूर्णाङ्क	inodes = 0;
	अचिह्नित पूर्णाङ्क	type = EXT2FS_BMAP64_BITARRAY;
	पूर्णाङ्क		flags = EXT2_FLAG_64_द्वयङ्काः;

	यदि (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	यावत् ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		विकल्प (c) {
		पक्ष 'b':
			खण्डानि = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			यदि (err)
				प्रत्यागम;
			भङ्ग;
		पक्ष 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			यदि (err)
				प्रत्यागम;
			भङ्ग;
		पक्ष 'l':	/* Legacy bitmaps */
			flags = 0;
			भङ्ग;
		पक्ष 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			यदि (err)
				प्रत्यागम;
			भङ्ग;
		स्वभाव:
			fprintf(stderr, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			प्रत्यागम;
		}
	}
	सज्जता_filesystem(argv[0], खण्डानि, inodes, type, flags);
}

शून्य close_cmd(पूर्णाङ्क argc, वर्ण **argv, पूर्णाङ्क sci_idx EXT2FS_ATTR((अप्रयुक्तम्)),
	       शून्य *infop EXT2FS_ATTR((अप्रयुक्तम्)))
{
	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	ext2fs_close_free(&test_fs);
}


शून्य dump_bitmap(ext2fs_generic_bitmap bmap, अचिह्नित पूर्णाङ्क start, अचिह्नित अङ्कः)
{
	अचिह्नित वर्ण	*buf;
	errcode_t	retval;
	पूर्णाङ्क		i, len = (अङ्कः - start + 7) / 8;

	buf = स्मृतिं_देहि(len);
	यदि (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		प्रत्यागम;
	}
	स्मृतिं_पूरय(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) start, अङ्कः, buf);
	यदि (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		मुक्त(buf);
		प्रत्यागम;
	}
	प्रति (i=0; i < len; i++)
		प्रारूप_लिख("%02x", buf[i]);
	प्रारूप_लिख("\n");
	प्रारूप_लिख("bits set: %u\n", ext2fs_bitcount(buf, len));
	मुक्त(buf);
}

शून्य dump_inode_bitmap_cmd(पूर्णाङ्क argc, ss_argv_t argv,
			   पूर्णाङ्क sci_idx EXT2FS_ATTR((अप्रयुक्तम्)),
			   शून्य *infop EXT2FS_ATTR((अप्रयुक्तम्)))
{
	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	प्रारूप_लिख("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_गणनम्);
}
	
शून्य dump_खण्डम्_bitmap_cmd(पूर्णाङ्क argc, ss_argv_t argv,
			   पूर्णाङ्क sci_idx EXT2FS_ATTR((अप्रयुक्तम्)),
			   शून्य *infop EXT2FS_ATTR((अप्रयुक्तम्)))
{
	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	प्रारूप_लिख("block bitmap: ");
	dump_bitmap(test_fs->खण्डम्_map, test_fs->super->s_first_दत्त_खण्डम्,
		    test_fs->super->s_खण्डानि_गणनम्);
}
	
शून्य do_setb(पूर्णाङ्क argc, ss_argv_t argv,
	     पूर्णाङ्क sci_idx EXT2FS_ATTR((अप्रयुक्तम्)),
	     शून्य *infop EXT2FS_ATTR((अप्रयुक्तम्)))
{
	अचिह्नित पूर्णाङ्क खण्डम्, अङ्कः;
	पूर्णाङ्क err;
	पूर्णाङ्क test_परिणाम, op_परिणाम;

	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	यदि (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		प्रत्यागम;
	}

	खण्डम् = parse_ulong(argv[1], argv[0], "block", &err);
	यदि (err)
		प्रत्यागम;

	यदि (argc == 3) {
		अङ्कः = parse_ulong(argv[2], argv[0], "num", &err);
		यदि (err)
			प्रत्यागम;

		ext2fs_mark_खण्डम्_bitmap_range2(test_fs->खण्डम्_map,
						खण्डम्, अङ्कः);
		प्रारूप_लिख("Marking blocks %u to %u\n", खण्डम्, खण्डम् + अङ्कः - 1);
		प्रत्यागम;
	}

	test_परिणाम = ext2fs_test_खण्डम्_bitmap2(test_fs->खण्डम्_map, खण्डम्);
	op_परिणाम = ext2fs_mark_खण्डम्_bitmap2(test_fs->खण्डम्_map, खण्डम्);
	प्रारूप_लिख("Setting block %u, was %s before\n", खण्डम्, op_परिणाम ?
	       "set" : "clear");
	यदि (!test_परिणाम != !op_परिणाम)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_परिणाम, op_परिणाम);
}

शून्य do_clearb(पूर्णाङ्क argc, ss_argv_t argv,
	       पूर्णाङ्क sci_idx EXT2FS_ATTR((अप्रयुक्तम्)),
	       शून्य *infop EXT2FS_ATTR((अप्रयुक्तम्)))
{
	अचिह्नित पूर्णाङ्क खण्डम्, अङ्कः;
	पूर्णाङ्क err;
	पूर्णाङ्क test_परिणाम, op_परिणाम;

	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	यदि (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		प्रत्यागम;
	}

	खण्डम् = parse_ulong(argv[1], argv[0], "block", &err);
	यदि (err)
		प्रत्यागम;

	यदि (argc == 3) {
		अङ्कः = parse_ulong(argv[2], argv[0], "num", &err);
		यदि (err)
			प्रत्यागम;

		ext2fs_unmark_खण्डम्_bitmap_range2(test_fs->खण्डम्_map,
						खण्डम्, अङ्कः);
		प्रारूप_लिख("Clearing blocks %u to %u\n", खण्डम्, खण्डम् + अङ्कः - 1);
		प्रत्यागम;
	}

	test_परिणाम = ext2fs_test_खण्डम्_bitmap2(test_fs->खण्डम्_map, खण्डम्);
	op_परिणाम = ext2fs_unmark_खण्डम्_bitmap2(test_fs->खण्डम्_map, खण्डम्);
	प्रारूप_लिख("Clearing block %u, was %s before\n", खण्डम्, op_परिणाम ?
	       "set" : "clear");
	यदि (!test_परिणाम != !op_परिणाम)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_परिणाम, op_परिणाम);
}

शून्य do_testb(पूर्णाङ्क argc, ss_argv_t argv,
	      पूर्णाङ्क sci_idx EXT2FS_ATTR((अप्रयुक्तम्)),
	      शून्य *infop EXT2FS_ATTR((अप्रयुक्तम्)))
{
	अचिह्नित पूर्णाङ्क खण्डम्, अङ्कः;
	पूर्णाङ्क err;
	पूर्णाङ्क test_परिणाम;

	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	यदि (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		प्रत्यागम;
	}

	खण्डम् = parse_ulong(argv[1], argv[0], "block", &err);
	यदि (err)
		प्रत्यागम;

	यदि (argc == 3) {
		अङ्कः = parse_ulong(argv[2], argv[0], "num", &err);
		यदि (err)
			प्रत्यागम;

		test_परिणाम =
			ext2fs_test_खण्डम्_bitmap_range2(test_fs->खण्डम्_map,
							खण्डम्, अङ्कः);
		प्रारूप_लिख("Blocks %u to %u are %sall clear.\n",
		       खण्डम्, खण्डम् + अङ्कः - 1, test_परिणाम ? "" : "NOT ");
		प्रत्यागम;
	}

	test_परिणाम = ext2fs_test_खण्डम्_bitmap2(test_fs->खण्डम्_map, खण्डम्);
	प्रारूप_लिख("Block %u is %s\n", खण्डम्, test_परिणाम ? "set" : "clear");
}

शून्य do_ffzb(पूर्णाङ्क argc, ss_argv_t argv,
	     पूर्णाङ्क sci_idx EXT2FS_ATTR((अप्रयुक्तम्)),
	     शून्य *infop EXT2FS_ATTR((अप्रयुक्तम्)))
{
	अचिह्नित पूर्णाङ्क start, अन्तः;
	पूर्णाङ्क err;
	errcode_t retval;
	blk64_t बहिर्गमनम्;

	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	यदि (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		प्रत्यागम;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	यदि (err)
		प्रत्यागम;

	अन्तः = parse_ulong(argv[2], argv[0], "end", &err);
	यदि (err)
		प्रत्यागम;

	retval = ext2fs_find_first_zero_खण्डम्_bitmap2(test_fs->खण्डम्_map,
						      start, अन्तः, &बहिर्गमनम्);
	यदि (retval) {
		प्रारूप_लिख("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_सन्देशः(retval));
		प्रत्यागम;
	}
	प्रारूप_लिख("First unmarked block is %llu\n", (अचिह्नित दीर्घ दीर्घ) बहिर्गमनम्);
}

शून्य do_ffsb(पूर्णाङ्क argc, ss_argv_t argv,
	     पूर्णाङ्क sci_idx EXT2FS_ATTR((अप्रयुक्तम्)),
	     शून्य *infop EXT2FS_ATTR((अप्रयुक्तम्)))
{
	अचिह्नित पूर्णाङ्क start, अन्तः;
	पूर्णाङ्क err;
	errcode_t retval;
	blk64_t बहिर्गमनम्;

	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	यदि (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		प्रत्यागम;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	यदि (err)
		प्रत्यागम;

	अन्तः = parse_ulong(argv[2], argv[0], "end", &err);
	यदि (err)
		प्रत्यागम;

	retval = ext2fs_find_first_समुच्चयः_खण्डम्_bitmap2(test_fs->खण्डम्_map,
						      start, अन्तः, &बहिर्गमनम्);
	यदि (retval) {
		प्रारूप_लिख("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_सन्देशः(retval));
		प्रत्यागम;
	}
	प्रारूप_लिख("First marked block is %llu\n", (अचिह्नित दीर्घ दीर्घ) बहिर्गमनम्);
}


शून्य do_zerob(पूर्णाङ्क argc, ss_argv_t argv,
	      पूर्णाङ्क sci_idx EXT2FS_ATTR((अप्रयुक्तम्)),
	      शून्य *infop EXT2FS_ATTR((अप्रयुक्तम्)))
{
	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	प्रारूप_लिख("Clearing block bitmap.\n");
	ext2fs_clear_खण्डम्_bitmap(test_fs->खण्डम्_map);
}

शून्य do_seti(पूर्णाङ्क argc, ss_argv_t argv,
	     पूर्णाङ्क sci_idx EXT2FS_ATTR((अप्रयुक्तम्)),
	     शून्य *infop EXT2FS_ATTR((अप्रयुक्तम्)))
{
	अचिह्नित पूर्णाङ्क inode;
	पूर्णाङ्क err;
	पूर्णाङ्क test_परिणाम, op_परिणाम;

	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	यदि (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		प्रत्यागम;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	यदि (err)
		प्रत्यागम;

	test_परिणाम = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_परिणाम = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	प्रारूप_लिख("Setting inode %u, was %s before\n", inode, op_परिणाम ?
	       "set" : "clear");
	यदि (!test_परिणाम != !op_परिणाम) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_परिणाम, op_परिणाम);
		exit_स्थितिः++;
	}
}

शून्य do_cleari(पूर्णाङ्क argc, ss_argv_t argv,
	       पूर्णाङ्क sci_idx EXT2FS_ATTR((अप्रयुक्तम्)),
	       शून्य *infop EXT2FS_ATTR((अप्रयुक्तम्)))
{
	अचिह्नित पूर्णाङ्क inode;
	पूर्णाङ्क err;
	पूर्णाङ्क test_परिणाम, op_परिणाम;

	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	यदि (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		प्रत्यागम;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	यदि (err)
		प्रत्यागम;

	test_परिणाम = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_परिणाम = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	प्रारूप_लिख("Clearing inode %u, was %s before\n", inode, op_परिणाम ?
	       "set" : "clear");
	यदि (!test_परिणाम != !op_परिणाम) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_परिणाम, op_परिणाम);
		exit_स्थितिः++;
	}
}

शून्य do_testi(पूर्णाङ्क argc, ss_argv_t argv,
	      पूर्णाङ्क sci_idx EXT2FS_ATTR((अप्रयुक्तम्)),
	      शून्य *infop EXT2FS_ATTR((अप्रयुक्तम्)))
{
	अचिह्नित पूर्णाङ्क inode;
	पूर्णाङ्क err;
	पूर्णाङ्क test_परिणाम;

	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	यदि (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		प्रत्यागम;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	यदि (err)
		प्रत्यागम;

	test_परिणाम = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	प्रारूप_लिख("Inode %u is %s\n", inode, test_परिणाम ? "set" : "clear");
}

शून्य do_ffzi(पूर्णाङ्क argc, ss_argv_t argv,
	     पूर्णाङ्क sci_idx EXT2FS_ATTR((अप्रयुक्तम्)),
	     शून्य *infop EXT2FS_ATTR((अप्रयुक्तम्)))
{
	अचिह्नित पूर्णाङ्क start, अन्तः;
	पूर्णाङ्क err;
	errcode_t retval;
	ext2_ino_t बहिर्गमनम्;

	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	यदि (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		प्रत्यागम;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	यदि (err)
		प्रत्यागम;

	अन्तः = parse_ulong(argv[2], argv[0], "end", &err);
	यदि (err)
		प्रत्यागम;

	retval = ext2fs_find_first_zero_inode_bitmap2(test_fs->inode_map,
						      start, अन्तः, &बहिर्गमनम्);
	यदि (retval) {
		प्रारूप_लिख("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_सन्देशः(retval));
		प्रत्यागम;
	}
	प्रारूप_लिख("First unmarked inode is %u\n", बहिर्गमनम्);
}

शून्य do_ffsi(पूर्णाङ्क argc, ss_argv_t argv,
	     पूर्णाङ्क sci_idx EXT2FS_ATTR((अप्रयुक्तम्)),
	     शून्य *infop EXT2FS_ATTR((अप्रयुक्तम्)))
{
	अचिह्नित पूर्णाङ्क start, अन्तः;
	पूर्णाङ्क err;
	errcode_t retval;
	ext2_ino_t बहिर्गमनम्;

	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	यदि (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		प्रत्यागम;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	यदि (err)
		प्रत्यागम;

	अन्तः = parse_ulong(argv[2], argv[0], "end", &err);
	यदि (err)
		प्रत्यागम;

	retval = ext2fs_find_first_समुच्चयः_inode_bitmap2(test_fs->inode_map,
						     start, अन्तः, &बहिर्गमनम्);
	यदि (retval) {
		प्रारूप_लिख("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_सन्देशः(retval));
		प्रत्यागम;
	}
	प्रारूप_लिख("First marked inode is %u\n", बहिर्गमनम्);
}

शून्य do_zeroi(पूर्णाङ्क argc, ss_argv_t argv,
	      पूर्णाङ्क sci_idx EXT2FS_ATTR((अप्रयुक्तम्)),
	      शून्य *infop EXT2FS_ATTR((अप्रयुक्तम्)))
{
	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	प्रारूप_लिख("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

पूर्णाङ्क मुख्यम्(पूर्णाङ्क argc, वर्ण **argv)
{
	अचिह्नित पूर्णाङ्क	खण्डानि = 128;
	अचिह्नित पूर्णाङ्क	inodes = 0;
	अचिह्नित पूर्णाङ्क	type = EXT2FS_BMAP64_BITARRAY;
	पूर्णाङ्क		c, err, संहिता;
	वर्ण		*request = (वर्ण *)रिक्त;
	वर्ण		*cmd_file = 0;
	पूर्णाङ्क		sci_idx;
	पूर्णाङ्क		flags = EXT2_FLAG_64_द्वयङ्काः;

	सङ्कलय_error_सारणी(&et_ss_error_सारणी);
	सङ्कलय_error_सारणी(&et_ext2_error_सारणी);
	यावत् ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		विकल्प (c) {
		पक्ष 'b':
			खण्डानि = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			यदि (err)
				निर्गच्छ(1);
			भङ्ग;
		पक्ष 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			यदि (err)
				निर्गच्छ(1);
			भङ्ग;
		पक्ष 'l':	/* Legacy bitmaps */
			flags = 0;
			भङ्ग;
		पक्ष 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			यदि (err)
				निर्गच्छ(1);
			भङ्ग;
		पक्ष 'R':
			request = optarg;
			भङ्ग;
		पक्ष 'f':
			cmd_file = optarg;
			भङ्ग;
		स्वभाव:
			com_err(argv[0], 0, "Usage: %s [-R request] "
				"[-f cmd_file]", subsystem_name);
			निर्गच्छ(1);
		}
	}

	sci_idx = ss_create_invocation(subsystem_name, संस्करणम्,
				       (वर्ण *)रिक्त, &tst_bitmaps_cmds, &संहिता);
	यदि (संहिता) {
		ss_perror(sci_idx, संहिता, "creating invocation");
		निर्गच्छ(1);
	}

	(शून्य) ss_सङ्कलय_request_सारणी (sci_idx, &ss_std_अनुरोधाः, 1, &संहिता);
	यदि (संहिता) {
		ss_perror(sci_idx, संहिता, "adding standard requests");
		निर्गच्छ (1);
	}

	प्रारूप_लिख("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, संस्करणम्);

	सज्जता_filesystem(argv[0], खण्डानि, inodes, type, flags);

	यदि (request) {
		संहिता = ss_execute_पङ्क्तिः(sci_idx, request);
		यदि (संहिता) {
			ss_perror(sci_idx, संहिता, request);
			exit_स्थितिः++;
		}
	} अन्यथा यदि (cmd_file) {
		exit_स्थितिः = source_file(cmd_file, sci_idx);
	} अन्यथा {
		ss_listen(sci_idx);
	}

	निर्गच्छ(exit_स्थितिः);
}

