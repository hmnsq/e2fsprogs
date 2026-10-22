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

स्थिर अक्षर subsystem_name[] = "tst_bitmaps";
स्थिर अक्षर version[] = "1.0";

ext2_filsys	test_fs;
पूर्णांक		exit_status = 0;

स्थिर पूर्णांक source_file(स्थिरांक अक्षर *cmd_file, पूर्णांक sci_idx)
{
	खाता		*f;
	अक्षर		buf[256];
	अक्षर		*cp;
	पूर्णांक		retval;
	पूर्णांक 		noecho;

	जर (म_भेद(cmd_file, "-") == 0)
		f = मानक_निवेश;
	अन्यथा {
		f = ख_खोलो(cmd_file, "r");
		जर (!f) {
			लिखो_त्रुटि(cmd_file);
			निकास(1);
		}
	}
	ख_साफ(मानक_निकास);
	ख_साफ(मानक_त्रुटि);
	रखो_बफ(मानक_निकास, रिक्त);
	रखो_बफ(मानक_त्रुटि, रिक्त);
	पर्यंत (!ख_पूर्ण(f)) {
		जर (ख_माला_लो(buf, आकार(buf), f) == रिक्त)
			थांबा;
		जर (buf[0] == '#')
			सुरूठेवा;
		noecho = 0;
		जर (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = म_अक्षर(buf, '\n');
		जर (cp)
			*cp = 0;
		cp = म_अक्षर(buf, '\r');
		जर (cp)
			*cp = 0;
		जर (!noecho)
			म_लिखो("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_ओळ(sci_idx, buf);
		जर (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_status++;
		}
	}
	परत exit_status;
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
#जर defined(__GLIBC__) || defined(__linux__)
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
अचिन्हित दीर्घ parse_ulong(स्थिरांक अक्षर *str, स्थिरांक अक्षर *cmd,
			  स्थिरांक अक्षर *descr, पूर्णांक *err)
{
	अक्षर		*tmp;
	अचिन्हित दीर्घ	ret;

	ret = म_से_अदीर्घ(str, &tmp, 0);
	जर (*tmp == 0) {
		जर (err)
			*err = 0;
		परत ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	जर (err)
		*err = 1;
	अन्यथा
		निकास(1);
	परत 0;
}


पूर्णांक check_fs_open(स्थिरांक अक्षर *name)
{
	जर (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		परत 1;
	}
	परत 0;
}

स्थिर शून्य setup_filesystem(स्थिरांक अक्षर *name,
			     अचिन्हित पूर्णांक blocks, अचिन्हित पूर्णांक inodes,
			     अचिन्हित पूर्णांक type, पूर्णांक flags)
{
	संरचना ext2_super_block param;
	errcode_t retval;

	स_रखो(&param, 0, आकार(param));
	ext2fs_blocks_मोजणी_set(&param, blocks);
	param.s_inodes_मोजणी = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	जर (retval) {
		com_err(name, retval, "while initializing filesystem");
		परत;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_block_bitmap(test_fs->block_map);
	test_fs->block_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_block_bitmap(test_fs, "block bitmap",
					      &test_fs->block_map);
	जर (retval) {
		com_err(name, retval, "while allocating block bitmap");
		जा errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	जर (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		जा errout;
	}
	परत;

errout:
	ext2fs_close_free(&test_fs);
}

शून्य setup_cmd(पूर्णांक argc, ss_argv_t argv,
	       पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	       शून्य *infop EXT2FS_ATTR((unused)))
{
	पूर्णांक		c, err;
	अचिन्हित पूर्णांक	blocks = 128;
	अचिन्हित पूर्णांक	inodes = 0;
	अचिन्हित पूर्णांक	type = EXT2FS_BMAP64_BITARRAY;
	पूर्णांक		flags = EXT2_FLAG_64BITS;

	जर (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	पर्यंत ((c = getopt(argc, argv, "b:i:lt:")) != खातापूर्ण) {
		निवड (c) {
		स्थिती 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			जर (err)
				परत;
			थांबा;
		स्थिती 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			जर (err)
				परत;
			थांबा;
		स्थिती 'l':	/* Legacy bitmaps */
			flags = 0;
			थांबा;
		स्थिती 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			जर (err)
				परत;
			थांबा;
		पूर्वनिर्धारित:
			ख_लिखो(मानक_त्रुटि, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			परत;
		}
	}
	setup_filesystem(argv[0], blocks, inodes, type, flags);
}

शून्य close_cmd(पूर्णांक argc, अक्षर **argv, पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	       शून्य *infop EXT2FS_ATTR((unused)))
{
	जर (check_fs_open(argv[0]))
		परत;

	ext2fs_close_free(&test_fs);
}


शून्य dump_bitmap(ext2fs_generic_bitmap bmap, अचिन्हित पूर्णांक start, अचिन्हित अंक)
{
	अचिन्हित अक्षर	*buf;
	errcode_t	retval;
	पूर्णांक		i, len = (अंक - start + 7) / 8;

	buf = दो_स्मृति(len);
	जर (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		परत;
	}
	स_रखो(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) start, अंक, buf);
	जर (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		मुक्त(buf);
		परत;
	}
	साठी (i=0; i < len; i++)
		म_लिखो("%02x", buf[i]);
	म_लिखो("\n");
	म_लिखो("bits set: %u\n", ext2fs_bitcount(buf, len));
	मुक्त(buf);
}

शून्य dump_inode_bitmap_cmd(पूर्णांक argc, ss_argv_t argv,
			   पूर्णांक sci_idx EXT2FS_ATTR((unused)),
			   शून्य *infop EXT2FS_ATTR((unused)))
{
	जर (check_fs_open(argv[0]))
		परत;

	म_लिखो("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_मोजणी);
}
	
शून्य dump_block_bitmap_cmd(पूर्णांक argc, ss_argv_t argv,
			   पूर्णांक sci_idx EXT2FS_ATTR((unused)),
			   शून्य *infop EXT2FS_ATTR((unused)))
{
	जर (check_fs_open(argv[0]))
		परत;

	म_लिखो("block bitmap: ");
	dump_bitmap(test_fs->block_map, test_fs->super->s_first_data_block,
		    test_fs->super->s_blocks_मोजणी);
}
	
शून्य do_setb(पूर्णांक argc, ss_argv_t argv,
	     पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	     शून्य *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णांक block, अंक;
	पूर्णांक err;
	पूर्णांक test_result, op_result;

	जर (check_fs_open(argv[0]))
		परत;

	जर (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		परत;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	जर (err)
		परत;

	जर (argc == 3) {
		अंक = parse_ulong(argv[2], argv[0], "num", &err);
		जर (err)
			परत;

		ext2fs_mark_block_bitmap_range2(test_fs->block_map,
						block, अंक);
		म_लिखो("Marking blocks %u to %u\n", block, block + अंक - 1);
		परत;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_mark_block_bitmap2(test_fs->block_map, block);
	म_लिखो("Setting block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	जर (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

शून्य do_clearb(पूर्णांक argc, ss_argv_t argv,
	       पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	       शून्य *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णांक block, अंक;
	पूर्णांक err;
	पूर्णांक test_result, op_result;

	जर (check_fs_open(argv[0]))
		परत;

	जर (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		परत;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	जर (err)
		परत;

	जर (argc == 3) {
		अंक = parse_ulong(argv[2], argv[0], "num", &err);
		जर (err)
			परत;

		ext2fs_unmark_block_bitmap_range2(test_fs->block_map,
						block, अंक);
		म_लिखो("Clearing blocks %u to %u\n", block, block + अंक - 1);
		परत;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_unmark_block_bitmap2(test_fs->block_map, block);
	म_लिखो("Clearing block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	जर (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

शून्य do_testb(पूर्णांक argc, ss_argv_t argv,
	      पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	      शून्य *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णांक block, अंक;
	पूर्णांक err;
	पूर्णांक test_result;

	जर (check_fs_open(argv[0]))
		परत;

	जर (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		परत;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	जर (err)
		परत;

	जर (argc == 3) {
		अंक = parse_ulong(argv[2], argv[0], "num", &err);
		जर (err)
			परत;

		test_result =
			ext2fs_test_block_bitmap_range2(test_fs->block_map,
							block, अंक);
		म_लिखो("Blocks %u to %u are %sall clear.\n",
		       block, block + अंक - 1, test_result ? "" : "NOT ");
		परत;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	म_लिखो("Block %u is %s\n", block, test_result ? "set" : "clear");
}

शून्य do_ffzb(पूर्णांक argc, ss_argv_t argv,
	     पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	     शून्य *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णांक start, end;
	पूर्णांक err;
	errcode_t retval;
	blk64_t out;

	जर (check_fs_open(argv[0]))
		परत;

	जर (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		परत;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	जर (err)
		परत;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	जर (err)
		परत;

	retval = ext2fs_find_first_zero_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	जर (retval) {
		म_लिखो("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_message(retval));
		परत;
	}
	म_लिखो("First unmarked block is %llu\n", (अचिन्हित दीर्घ दीर्घ) out);
}

शून्य do_ffsb(पूर्णांक argc, ss_argv_t argv,
	     पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	     शून्य *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णांक start, end;
	पूर्णांक err;
	errcode_t retval;
	blk64_t out;

	जर (check_fs_open(argv[0]))
		परत;

	जर (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		परत;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	जर (err)
		परत;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	जर (err)
		परत;

	retval = ext2fs_find_first_set_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	जर (retval) {
		म_लिखो("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_message(retval));
		परत;
	}
	म_लिखो("First marked block is %llu\n", (अचिन्हित दीर्घ दीर्घ) out);
}


शून्य do_zerob(पूर्णांक argc, ss_argv_t argv,
	      पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	      शून्य *infop EXT2FS_ATTR((unused)))
{
	जर (check_fs_open(argv[0]))
		परत;

	म_लिखो("Clearing block bitmap.\n");
	ext2fs_clear_block_bitmap(test_fs->block_map);
}

शून्य do_seti(पूर्णांक argc, ss_argv_t argv,
	     पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	     शून्य *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णांक inode;
	पूर्णांक err;
	पूर्णांक test_result, op_result;

	जर (check_fs_open(argv[0]))
		परत;

	जर (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		परत;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	जर (err)
		परत;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	म_लिखो("Setting inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	जर (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

शून्य do_cleari(पूर्णांक argc, ss_argv_t argv,
	       पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	       शून्य *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णांक inode;
	पूर्णांक err;
	पूर्णांक test_result, op_result;

	जर (check_fs_open(argv[0]))
		परत;

	जर (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		परत;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	जर (err)
		परत;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	म_लिखो("Clearing inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	जर (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

शून्य do_testi(पूर्णांक argc, ss_argv_t argv,
	      पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	      शून्य *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णांक inode;
	पूर्णांक err;
	पूर्णांक test_result;

	जर (check_fs_open(argv[0]))
		परत;

	जर (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		परत;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	जर (err)
		परत;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	म_लिखो("Inode %u is %s\n", inode, test_result ? "set" : "clear");
}

शून्य do_ffzi(पूर्णांक argc, ss_argv_t argv,
	     पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	     शून्य *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णांक start, end;
	पूर्णांक err;
	errcode_t retval;
	ext2_ino_t out;

	जर (check_fs_open(argv[0]))
		परत;

	जर (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		परत;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	जर (err)
		परत;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	जर (err)
		परत;

	retval = ext2fs_find_first_zero_inode_bitmap2(test_fs->inode_map,
						      start, end, &out);
	जर (retval) {
		म_लिखो("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_message(retval));
		परत;
	}
	म_लिखो("First unmarked inode is %u\n", out);
}

शून्य do_ffsi(पूर्णांक argc, ss_argv_t argv,
	     पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	     शून्य *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णांक start, end;
	पूर्णांक err;
	errcode_t retval;
	ext2_ino_t out;

	जर (check_fs_open(argv[0]))
		परत;

	जर (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		परत;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	जर (err)
		परत;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	जर (err)
		परत;

	retval = ext2fs_find_first_set_inode_bitmap2(test_fs->inode_map,
						     start, end, &out);
	जर (retval) {
		म_लिखो("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_message(retval));
		परत;
	}
	म_लिखो("First marked inode is %u\n", out);
}

शून्य do_zeroi(पूर्णांक argc, ss_argv_t argv,
	      पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	      शून्य *infop EXT2FS_ATTR((unused)))
{
	जर (check_fs_open(argv[0]))
		परत;

	म_लिखो("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

पूर्णांक main(पूर्णांक argc, अक्षर **argv)
{
	अचिन्हित पूर्णांक	blocks = 128;
	अचिन्हित पूर्णांक	inodes = 0;
	अचिन्हित पूर्णांक	type = EXT2FS_BMAP64_BITARRAY;
	पूर्णांक		c, err, code;
	अक्षर		*request = (अक्षर *)रिक्त;
	अक्षर		*cmd_file = 0;
	पूर्णांक		sci_idx;
	पूर्णांक		flags = EXT2_FLAG_64BITS;

	add_error_table(&et_ss_error_table);
	add_error_table(&et_ext2_error_table);
	पर्यंत ((c = getopt (argc, argv, "b:i:lt:R:f:")) != खातापूर्ण) {
		निवड (c) {
		स्थिती 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			जर (err)
				निकास(1);
			थांबा;
		स्थिती 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			जर (err)
				निकास(1);
			थांबा;
		स्थिती 'l':	/* Legacy bitmaps */
			flags = 0;
			थांबा;
		स्थिती 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			जर (err)
				निकास(1);
			थांबा;
		स्थिती 'R':
			request = optarg;
			थांबा;
		स्थिती 'f':
			cmd_file = optarg;
			थांबा;
		पूर्वनिर्धारित:
			com_err(argv[0], 0, "Usage: %s [-R request] "
				"[-f cmd_file]", subsystem_name);
			निकास(1);
		}
	}

	sci_idx = ss_create_invocation(subsystem_name, version,
				       (अक्षर *)रिक्त, &tst_bitmaps_cmds, &code);
	जर (code) {
		ss_perror(sci_idx, code, "creating invocation");
		निकास(1);
	}

	(शून्य) ss_add_request_table (sci_idx, &ss_std_requests, 1, &code);
	जर (code) {
		ss_perror(sci_idx, code, "adding standard requests");
		निकास (1);
	}

	म_लिखो("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, version);

	setup_filesystem(argv[0], blocks, inodes, type, flags);

	जर (request) {
		code = ss_execute_ओळ(sci_idx, request);
		जर (code) {
			ss_perror(sci_idx, code, request);
			exit_status++;
		}
	} अन्यथा जर (cmd_file) {
		exit_status = source_file(cmd_file, sci_idx);
	} अन्यथा {
		ss_listen(sci_idx);
	}

	निकास(exit_status);
}

