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
#ifdef HAVE_GETOPT_H
#अन्तर्भाव <getopt.h>
#endif
#अन्तर्भाव <string.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव "ss/ss.h"

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

बाह्य ss_request_table tst_bitmaps_cmds;

स्थिर वर्ण subsystem_name[] = "tst_bitmaps";
स्थिर वर्ण version[] = "1.0";

ext2_filsys	test_fs;
पूर्णाङ्क		exit_status = 0;

स्थिर पूर्णाङ्क source_file(ध्रुव वर्ण *cmd_file, पूर्णाङ्क sci_idx)
{
	खाता		*f;
	वर्ण		buf[256];
	वर्ण		*cp;
	पूर्णाङ्क		retval;
	पूर्णाङ्क 		noecho;

	यदि (म_भेद(cmd_file, "-") == 0)
		f = मानक_निवेश;
	अन्यथा {
		f = ख_खोलो(cmd_file, "r");
		यदि (!f) {
			लिखो_त्रुटि(cmd_file);
			निकास(1);
		}
	}
	ख_साफ(मानक_निकास);
	ख_साफ(मानक_त्रुटि);
	रखो_बफ(मानक_निकास, रिक्त);
	रखो_बफ(मानक_त्रुटि, रिक्त);
	यावत् (!ख_पूर्ण(f)) {
		यदि (ख_माला_लो(buf, परिमाण(buf), f) == रिक्त)
			भङ्ग;
		यदि (buf[0] == '#')
			अनुवर्त;
		noecho = 0;
		यदि (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = म_अक्षर(buf, '\n');
		यदि (cp)
			*cp = 0;
		cp = म_अक्षर(buf, '\r');
		यदि (cp)
			*cp = 0;
		यदि (!noecho)
			म_लिखो("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_पंक्ति(sci_idx, buf);
		यदि (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_status++;
		}
	}
	प्रत्यागम exit_status;
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
#endif
#ifdef HAVE_OPTRESET
	optreset = 1;		/* Makes BSD getopt happy */
#endif
}

/*
 * This function will convert a string to an unsigned long, printing
 * an error message if it fails, and returning success or failure in err.
 */
अचिन्हित दीर्घ parse_ulong(ध्रुव वर्ण *str, ध्रुव वर्ण *cmd,
			  ध्रुव वर्ण *descr, पूर्णाङ्क *err)
{
	वर्ण		*tmp;
	अचिन्हित दीर्घ	ret;

	ret = म_से_अदीर्घ(str, &tmp, 0);
	यदि (*tmp == 0) {
		यदि (err)
			*err = 0;
		प्रत्यागम ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	यदि (err)
		*err = 1;
	अन्यथा
		निकास(1);
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

स्थिर शून्य setup_filesystem(ध्रुव वर्ण *name,
			     अचिन्हित पूर्णाङ्क blocks, अचिन्हित पूर्णाङ्क inodes,
			     अचिन्हित पूर्णाङ्क type, पूर्णाङ्क flags)
{
	संरचना ext2_super_block param;
	errcode_t retval;

	स_रखो(&param, 0, परिमाण(param));
	ext2fs_blocks_गिनती_set(&param, blocks);
	param.s_inodes_गिनती = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	यदि (retval) {
		com_err(name, retval, "while initializing filesystem");
		प्रत्यागम;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_block_bitmap(test_fs->block_map);
	test_fs->block_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_block_bitmap(test_fs, "block bitmap",
					      &test_fs->block_map);
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

शून्य setup_cmd(पूर्णाङ्क argc, ss_argv_t argv,
	       पूर्णाङ्क sci_idx EXT2FS_ATTR((unused)),
	       शून्य *infop EXT2FS_ATTR((unused)))
{
	पूर्णाङ्क		c, err;
	अचिन्हित पूर्णाङ्क	blocks = 128;
	अचिन्हित पूर्णाङ्क	inodes = 0;
	अचिन्हित पूर्णाङ्क	type = EXT2FS_BMAP64_BITARRAY;
	पूर्णाङ्क		flags = EXT2_FLAG_64BITS;

	यदि (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	यावत् ((c = getopt(argc, argv, "b:i:lt:")) != खातापूर्ण) {
		विकल्प (c) {
		पक्ष 'b':
			blocks = parse_ulong(optarg, argv[0],
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
			ख_लिखो(मानक_त्रुटि, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			प्रत्यागम;
		}
	}
	setup_filesystem(argv[0], blocks, inodes, type, flags);
}

शून्य close_cmd(पूर्णाङ्क argc, वर्ण **argv, पूर्णाङ्क sci_idx EXT2FS_ATTR((unused)),
	       शून्य *infop EXT2FS_ATTR((unused)))
{
	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	ext2fs_close_free(&test_fs);
}


शून्य dump_bitmap(ext2fs_generic_bitmap bmap, अचिन्हित पूर्णाङ्क start, अचिन्हित अंक)
{
	अचिन्हित वर्ण	*buf;
	errcode_t	retval;
	पूर्णाङ्क		i, len = (अंक - start + 7) / 8;

	buf = दो_स्मृति(len);
	यदि (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		प्रत्यागम;
	}
	स_रखो(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) start, अंक, buf);
	यदि (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		मुक्त(buf);
		प्रत्यागम;
	}
	प्रति (i=0; i < len; i++)
		म_लिखो("%02x", buf[i]);
	म_लिखो("\n");
	म_लिखो("bits set: %u\n", ext2fs_bitcount(buf, len));
	मुक्त(buf);
}

शून्य dump_inode_bitmap_cmd(पूर्णाङ्क argc, ss_argv_t argv,
			   पूर्णाङ्क sci_idx EXT2FS_ATTR((unused)),
			   शून्य *infop EXT2FS_ATTR((unused)))
{
	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	म_लिखो("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_गिनती);
}
	
शून्य dump_block_bitmap_cmd(पूर्णाङ्क argc, ss_argv_t argv,
			   पूर्णाङ्क sci_idx EXT2FS_ATTR((unused)),
			   शून्य *infop EXT2FS_ATTR((unused)))
{
	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	म_लिखो("block bitmap: ");
	dump_bitmap(test_fs->block_map, test_fs->super->s_first_data_block,
		    test_fs->super->s_blocks_गिनती);
}
	
शून्य do_setb(पूर्णाङ्क argc, ss_argv_t argv,
	     पूर्णाङ्क sci_idx EXT2FS_ATTR((unused)),
	     शून्य *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णाङ्क block, अंक;
	पूर्णाङ्क err;
	पूर्णाङ्क test_result, op_result;

	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	यदि (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		प्रत्यागम;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	यदि (err)
		प्रत्यागम;

	यदि (argc == 3) {
		अंक = parse_ulong(argv[2], argv[0], "num", &err);
		यदि (err)
			प्रत्यागम;

		ext2fs_mark_block_bitmap_range2(test_fs->block_map,
						block, अंक);
		म_लिखो("Marking blocks %u to %u\n", block, block + अंक - 1);
		प्रत्यागम;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_mark_block_bitmap2(test_fs->block_map, block);
	म_लिखो("Setting block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	यदि (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

शून्य do_clearb(पूर्णाङ्क argc, ss_argv_t argv,
	       पूर्णाङ्क sci_idx EXT2FS_ATTR((unused)),
	       शून्य *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णाङ्क block, अंक;
	पूर्णाङ्क err;
	पूर्णाङ्क test_result, op_result;

	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	यदि (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		प्रत्यागम;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	यदि (err)
		प्रत्यागम;

	यदि (argc == 3) {
		अंक = parse_ulong(argv[2], argv[0], "num", &err);
		यदि (err)
			प्रत्यागम;

		ext2fs_unmark_block_bitmap_range2(test_fs->block_map,
						block, अंक);
		म_लिखो("Clearing blocks %u to %u\n", block, block + अंक - 1);
		प्रत्यागम;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_unmark_block_bitmap2(test_fs->block_map, block);
	म_लिखो("Clearing block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	यदि (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

शून्य do_testb(पूर्णाङ्क argc, ss_argv_t argv,
	      पूर्णाङ्क sci_idx EXT2FS_ATTR((unused)),
	      शून्य *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णाङ्क block, अंक;
	पूर्णाङ्क err;
	पूर्णाङ्क test_result;

	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	यदि (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		प्रत्यागम;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	यदि (err)
		प्रत्यागम;

	यदि (argc == 3) {
		अंक = parse_ulong(argv[2], argv[0], "num", &err);
		यदि (err)
			प्रत्यागम;

		test_result =
			ext2fs_test_block_bitmap_range2(test_fs->block_map,
							block, अंक);
		म_लिखो("Blocks %u to %u are %sall clear.\n",
		       block, block + अंक - 1, test_result ? "" : "NOT ");
		प्रत्यागम;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	म_लिखो("Block %u is %s\n", block, test_result ? "set" : "clear");
}

शून्य do_ffzb(पूर्णाङ्क argc, ss_argv_t argv,
	     पूर्णाङ्क sci_idx EXT2FS_ATTR((unused)),
	     शून्य *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णाङ्क start, end;
	पूर्णाङ्क err;
	errcode_t retval;
	blk64_t out;

	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	यदि (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		प्रत्यागम;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	यदि (err)
		प्रत्यागम;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	यदि (err)
		प्रत्यागम;

	retval = ext2fs_find_first_zero_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	यदि (retval) {
		म_लिखो("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_message(retval));
		प्रत्यागम;
	}
	म_लिखो("First unmarked block is %llu\n", (अचिन्हित दीर्घ दीर्घ) out);
}

शून्य do_ffsb(पूर्णाङ्क argc, ss_argv_t argv,
	     पूर्णाङ्क sci_idx EXT2FS_ATTR((unused)),
	     शून्य *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णाङ्क start, end;
	पूर्णाङ्क err;
	errcode_t retval;
	blk64_t out;

	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	यदि (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		प्रत्यागम;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	यदि (err)
		प्रत्यागम;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	यदि (err)
		प्रत्यागम;

	retval = ext2fs_find_first_set_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	यदि (retval) {
		म_लिखो("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_message(retval));
		प्रत्यागम;
	}
	म_लिखो("First marked block is %llu\n", (अचिन्हित दीर्घ दीर्घ) out);
}


शून्य do_zerob(पूर्णाङ्क argc, ss_argv_t argv,
	      पूर्णाङ्क sci_idx EXT2FS_ATTR((unused)),
	      शून्य *infop EXT2FS_ATTR((unused)))
{
	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	म_लिखो("Clearing block bitmap.\n");
	ext2fs_clear_block_bitmap(test_fs->block_map);
}

शून्य do_seti(पूर्णाङ्क argc, ss_argv_t argv,
	     पूर्णाङ्क sci_idx EXT2FS_ATTR((unused)),
	     शून्य *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णाङ्क inode;
	पूर्णाङ्क err;
	पूर्णाङ्क test_result, op_result;

	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	यदि (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		प्रत्यागम;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	यदि (err)
		प्रत्यागम;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	म_लिखो("Setting inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	यदि (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

शून्य do_cleari(पूर्णाङ्क argc, ss_argv_t argv,
	       पूर्णाङ्क sci_idx EXT2FS_ATTR((unused)),
	       शून्य *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णाङ्क inode;
	पूर्णाङ्क err;
	पूर्णाङ्क test_result, op_result;

	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	यदि (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		प्रत्यागम;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	यदि (err)
		प्रत्यागम;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	म_लिखो("Clearing inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	यदि (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

शून्य do_testi(पूर्णाङ्क argc, ss_argv_t argv,
	      पूर्णाङ्क sci_idx EXT2FS_ATTR((unused)),
	      शून्य *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णाङ्क inode;
	पूर्णाङ्क err;
	पूर्णाङ्क test_result;

	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	यदि (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		प्रत्यागम;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	यदि (err)
		प्रत्यागम;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	म_लिखो("Inode %u is %s\n", inode, test_result ? "set" : "clear");
}

शून्य do_ffzi(पूर्णाङ्क argc, ss_argv_t argv,
	     पूर्णाङ्क sci_idx EXT2FS_ATTR((unused)),
	     शून्य *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णाङ्क start, end;
	पूर्णाङ्क err;
	errcode_t retval;
	ext2_ino_t out;

	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	यदि (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		प्रत्यागम;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	यदि (err)
		प्रत्यागम;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	यदि (err)
		प्रत्यागम;

	retval = ext2fs_find_first_zero_inode_bitmap2(test_fs->inode_map,
						      start, end, &out);
	यदि (retval) {
		म_लिखो("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_message(retval));
		प्रत्यागम;
	}
	म_लिखो("First unmarked inode is %u\n", out);
}

शून्य do_ffsi(पूर्णाङ्क argc, ss_argv_t argv,
	     पूर्णाङ्क sci_idx EXT2FS_ATTR((unused)),
	     शून्य *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णाङ्क start, end;
	पूर्णाङ्क err;
	errcode_t retval;
	ext2_ino_t out;

	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	यदि (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		प्रत्यागम;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	यदि (err)
		प्रत्यागम;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	यदि (err)
		प्रत्यागम;

	retval = ext2fs_find_first_set_inode_bitmap2(test_fs->inode_map,
						     start, end, &out);
	यदि (retval) {
		म_लिखो("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_message(retval));
		प्रत्यागम;
	}
	म_लिखो("First marked inode is %u\n", out);
}

शून्य do_zeroi(पूर्णाङ्क argc, ss_argv_t argv,
	      पूर्णाङ्क sci_idx EXT2FS_ATTR((unused)),
	      शून्य *infop EXT2FS_ATTR((unused)))
{
	यदि (check_fs_open(argv[0]))
		प्रत्यागम;

	म_लिखो("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

पूर्णाङ्क main(पूर्णाङ्क argc, वर्ण **argv)
{
	अचिन्हित पूर्णाङ्क	blocks = 128;
	अचिन्हित पूर्णाङ्क	inodes = 0;
	अचिन्हित पूर्णाङ्क	type = EXT2FS_BMAP64_BITARRAY;
	पूर्णाङ्क		c, err, code;
	वर्ण		*request = (वर्ण *)रिक्त;
	वर्ण		*cmd_file = 0;
	पूर्णाङ्क		sci_idx;
	पूर्णाङ्क		flags = EXT2_FLAG_64BITS;

	add_error_table(&et_ss_error_table);
	add_error_table(&et_ext2_error_table);
	यावत् ((c = getopt (argc, argv, "b:i:lt:R:f:")) != खातापूर्ण) {
		विकल्प (c) {
		पक्ष 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			यदि (err)
				निकास(1);
			भङ्ग;
		पक्ष 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			यदि (err)
				निकास(1);
			भङ्ग;
		पक्ष 'l':	/* Legacy bitmaps */
			flags = 0;
			भङ्ग;
		पक्ष 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			यदि (err)
				निकास(1);
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
			निकास(1);
		}
	}

	sci_idx = ss_create_invocation(subsystem_name, version,
				       (वर्ण *)रिक्त, &tst_bitmaps_cmds, &code);
	यदि (code) {
		ss_perror(sci_idx, code, "creating invocation");
		निकास(1);
	}

	(शून्य) ss_add_request_table (sci_idx, &ss_std_requests, 1, &code);
	यदि (code) {
		ss_perror(sci_idx, code, "adding standard requests");
		निकास (1);
	}

	म_लिखो("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, version);

	setup_filesystem(argv[0], blocks, inodes, type, flags);

	यदि (request) {
		code = ss_execute_पंक्ति(sci_idx, request);
		यदि (code) {
			ss_perror(sci_idx, code, request);
			exit_status++;
		}
	} अन्यथा यदि (cmd_file) {
		exit_status = source_file(cmd_file, sci_idx);
	} अन्यथा {
		ss_listen(sci_idx);
	}

	निकास(exit_status);
}

