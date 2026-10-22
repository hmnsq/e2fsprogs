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

#समावेश "config.h"
#समावेश <unistd.h>
#समावेश <stdlib.h>
#समावेश <stdio.h>
#अगर_घोषित HAVE_GETOPT_H
#समावेश <getopt.h>
#पूर्ण_अगर
#समावेश <string.h>
#समावेश <fcntl.h>
#समावेश <time.h>
#समावेश <sys/stat.h>
#समावेश <sys/types.h>
#समावेश "ss/ss.h"

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"
#समावेश "ext2fsP.h"

बाह्य ss_request_table tst_bitmaps_cmds;

अटल अक्षर subsystem_name[] = "tst_bitmaps";
अटल अक्षर version[] = "1.0";

ext2_filsys	test_fs;
पूर्णांक		exit_status = 0;

अटल पूर्णांक source_file(स्थिर अक्षर *cmd_file, पूर्णांक sci_idx)
{
	खाता		*f;
	अक्षर		buf[256];
	अक्षर		*cp;
	पूर्णांक		retval;
	पूर्णांक 		noecho;

	अगर (म_भेद(cmd_file, "-") == 0)
		f = मानक_निवेश;
	अन्यथा {
		f = ख_खोलो(cmd_file, "r");
		अगर (!f) {
			लिखो_त्रुटि(cmd_file);
			निकास(1);
		}
	}
	ख_साफ(मानक_निकास);
	ख_साफ(मानक_त्रुटि);
	रखो_बफ(मानक_निकास, शून्य);
	रखो_बफ(मानक_त्रुटि, शून्य);
	जबतक (!ख_पूर्ण(f)) {
		अगर (ख_माला_लो(buf, माप(buf), f) == शून्य)
			अवरोध;
		अगर (buf[0] == '#')
			जारी;
		noecho = 0;
		अगर (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = म_अक्षर(buf, '\n');
		अगर (cp)
			*cp = 0;
		cp = म_अक्षर(buf, '\r');
		अगर (cp)
			*cp = 0;
		अगर (!noecho)
			म_लिखो("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_पंक्ति(sci_idx, buf);
		अगर (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_status++;
		}
	}
	वापस exit_status;
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
व्योम reset_getopt(व्योम)
{
#अगर defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#अन्यथा
	optind = 1;
#पूर्ण_अगर
#अगर_घोषित HAVE_OPTRESET
	optreset = 1;		/* Makes BSD getopt happy */
#पूर्ण_अगर
}

/*
 * This function will convert a string to an unsigned long, printing
 * an error message if it fails, and returning success or failure in err.
 */
अचिन्हित दीर्घ parse_ulong(स्थिर अक्षर *str, स्थिर अक्षर *cmd,
			  स्थिर अक्षर *descr, पूर्णांक *err)
{
	अक्षर		*tmp;
	अचिन्हित दीर्घ	ret;

	ret = म_से_अदीर्घ(str, &tmp, 0);
	अगर (*tmp == 0) {
		अगर (err)
			*err = 0;
		वापस ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	अगर (err)
		*err = 1;
	अन्यथा
		निकास(1);
	वापस 0;
}


पूर्णांक check_fs_open(स्थिर अक्षर *name)
{
	अगर (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		वापस 1;
	}
	वापस 0;
}

अटल व्योम setup_filesystem(स्थिर अक्षर *name,
			     अचिन्हित पूर्णांक blocks, अचिन्हित पूर्णांक inodes,
			     अचिन्हित पूर्णांक type, पूर्णांक flags)
{
	काष्ठा ext2_super_block param;
	errcode_t retval;

	स_रखो(&param, 0, माप(param));
	ext2fs_blocks_गिनती_set(&param, blocks);
	param.s_inodes_गिनती = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	अगर (retval) {
		com_err(name, retval, "while initializing filesystem");
		वापस;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_block_bitmap(test_fs->block_map);
	test_fs->block_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_block_bitmap(test_fs, "block bitmap",
					      &test_fs->block_map);
	अगर (retval) {
		com_err(name, retval, "while allocating block bitmap");
		जाओ errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	अगर (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		जाओ errout;
	}
	वापस;

errout:
	ext2fs_close_free(&test_fs);
}

व्योम setup_cmd(पूर्णांक argc, ss_argv_t argv,
	       पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	       व्योम *infop EXT2FS_ATTR((unused)))
{
	पूर्णांक		c, err;
	अचिन्हित पूर्णांक	blocks = 128;
	अचिन्हित पूर्णांक	inodes = 0;
	अचिन्हित पूर्णांक	type = EXT2FS_BMAP64_BITARRAY;
	पूर्णांक		flags = EXT2_FLAG_64BITS;

	अगर (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	जबतक ((c = getopt(argc, argv, "b:i:lt:")) != खातापूर्ण) {
		चयन (c) {
		हाल 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			अगर (err)
				वापस;
			अवरोध;
		हाल 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			अगर (err)
				वापस;
			अवरोध;
		हाल 'l':	/* Legacy bitmaps */
			flags = 0;
			अवरोध;
		हाल 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			अगर (err)
				वापस;
			अवरोध;
		शेष:
			ख_लिखो(मानक_त्रुटि, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			वापस;
		}
	}
	setup_filesystem(argv[0], blocks, inodes, type, flags);
}

व्योम close_cmd(पूर्णांक argc, अक्षर **argv, पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	       व्योम *infop EXT2FS_ATTR((unused)))
{
	अगर (check_fs_open(argv[0]))
		वापस;

	ext2fs_close_free(&test_fs);
}


व्योम dump_bitmap(ext2fs_generic_bitmap bmap, अचिन्हित पूर्णांक start, अचिन्हित अंक)
{
	अचिन्हित अक्षर	*buf;
	errcode_t	retval;
	पूर्णांक		i, len = (अंक - start + 7) / 8;

	buf = दो_स्मृति(len);
	अगर (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		वापस;
	}
	स_रखो(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) start, अंक, buf);
	अगर (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		मुक्त(buf);
		वापस;
	}
	क्रम (i=0; i < len; i++)
		म_लिखो("%02x", buf[i]);
	म_लिखो("\n");
	म_लिखो("bits set: %u\n", ext2fs_bitcount(buf, len));
	मुक्त(buf);
}

व्योम dump_inode_bitmap_cmd(पूर्णांक argc, ss_argv_t argv,
			   पूर्णांक sci_idx EXT2FS_ATTR((unused)),
			   व्योम *infop EXT2FS_ATTR((unused)))
{
	अगर (check_fs_open(argv[0]))
		वापस;

	म_लिखो("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_गिनती);
}
	
व्योम dump_block_bitmap_cmd(पूर्णांक argc, ss_argv_t argv,
			   पूर्णांक sci_idx EXT2FS_ATTR((unused)),
			   व्योम *infop EXT2FS_ATTR((unused)))
{
	अगर (check_fs_open(argv[0]))
		वापस;

	म_लिखो("block bitmap: ");
	dump_bitmap(test_fs->block_map, test_fs->super->s_first_तथ्य_block,
		    test_fs->super->s_blocks_गिनती);
}
	
व्योम do_setb(पूर्णांक argc, ss_argv_t argv,
	     पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	     व्योम *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णांक block, अंक;
	पूर्णांक err;
	पूर्णांक test_परिणाम, op_परिणाम;

	अगर (check_fs_open(argv[0]))
		वापस;

	अगर (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		वापस;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	अगर (err)
		वापस;

	अगर (argc == 3) {
		अंक = parse_ulong(argv[2], argv[0], "num", &err);
		अगर (err)
			वापस;

		ext2fs_mark_block_bitmap_range2(test_fs->block_map,
						block, अंक);
		म_लिखो("Marking blocks %u to %u\n", block, block + अंक - 1);
		वापस;
	}

	test_परिणाम = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_परिणाम = ext2fs_mark_block_bitmap2(test_fs->block_map, block);
	म_लिखो("Setting block %u, was %s before\n", block, op_परिणाम ?
	       "set" : "clear");
	अगर (!test_परिणाम != !op_परिणाम)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_परिणाम, op_परिणाम);
}

व्योम do_clearb(पूर्णांक argc, ss_argv_t argv,
	       पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	       व्योम *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णांक block, अंक;
	पूर्णांक err;
	पूर्णांक test_परिणाम, op_परिणाम;

	अगर (check_fs_open(argv[0]))
		वापस;

	अगर (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		वापस;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	अगर (err)
		वापस;

	अगर (argc == 3) {
		अंक = parse_ulong(argv[2], argv[0], "num", &err);
		अगर (err)
			वापस;

		ext2fs_unmark_block_bitmap_range2(test_fs->block_map,
						block, अंक);
		म_लिखो("Clearing blocks %u to %u\n", block, block + अंक - 1);
		वापस;
	}

	test_परिणाम = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_परिणाम = ext2fs_unmark_block_bitmap2(test_fs->block_map, block);
	म_लिखो("Clearing block %u, was %s before\n", block, op_परिणाम ?
	       "set" : "clear");
	अगर (!test_परिणाम != !op_परिणाम)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_परिणाम, op_परिणाम);
}

व्योम do_testb(पूर्णांक argc, ss_argv_t argv,
	      पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	      व्योम *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णांक block, अंक;
	पूर्णांक err;
	पूर्णांक test_परिणाम;

	अगर (check_fs_open(argv[0]))
		वापस;

	अगर (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		वापस;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	अगर (err)
		वापस;

	अगर (argc == 3) {
		अंक = parse_ulong(argv[2], argv[0], "num", &err);
		अगर (err)
			वापस;

		test_परिणाम =
			ext2fs_test_block_bitmap_range2(test_fs->block_map,
							block, अंक);
		म_लिखो("Blocks %u to %u are %sall clear.\n",
		       block, block + अंक - 1, test_परिणाम ? "" : "NOT ");
		वापस;
	}

	test_परिणाम = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	म_लिखो("Block %u is %s\n", block, test_परिणाम ? "set" : "clear");
}

व्योम do_ffzb(पूर्णांक argc, ss_argv_t argv,
	     पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	     व्योम *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णांक start, end;
	पूर्णांक err;
	errcode_t retval;
	blk64_t out;

	अगर (check_fs_open(argv[0]))
		वापस;

	अगर (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		वापस;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	अगर (err)
		वापस;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	अगर (err)
		वापस;

	retval = ext2fs_find_first_zero_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	अगर (retval) {
		म_लिखो("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_message(retval));
		वापस;
	}
	म_लिखो("First unmarked block is %llu\n", (अचिन्हित दीर्घ दीर्घ) out);
}

व्योम do_ffsb(पूर्णांक argc, ss_argv_t argv,
	     पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	     व्योम *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णांक start, end;
	पूर्णांक err;
	errcode_t retval;
	blk64_t out;

	अगर (check_fs_open(argv[0]))
		वापस;

	अगर (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		वापस;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	अगर (err)
		वापस;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	अगर (err)
		वापस;

	retval = ext2fs_find_first_set_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	अगर (retval) {
		म_लिखो("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_message(retval));
		वापस;
	}
	म_लिखो("First marked block is %llu\n", (अचिन्हित दीर्घ दीर्घ) out);
}


व्योम do_zerob(पूर्णांक argc, ss_argv_t argv,
	      पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	      व्योम *infop EXT2FS_ATTR((unused)))
{
	अगर (check_fs_open(argv[0]))
		वापस;

	म_लिखो("Clearing block bitmap.\n");
	ext2fs_clear_block_bitmap(test_fs->block_map);
}

व्योम do_seti(पूर्णांक argc, ss_argv_t argv,
	     पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	     व्योम *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णांक inode;
	पूर्णांक err;
	पूर्णांक test_परिणाम, op_परिणाम;

	अगर (check_fs_open(argv[0]))
		वापस;

	अगर (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		वापस;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	अगर (err)
		वापस;

	test_परिणाम = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_परिणाम = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	म_लिखो("Setting inode %u, was %s before\n", inode, op_परिणाम ?
	       "set" : "clear");
	अगर (!test_परिणाम != !op_परिणाम) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_परिणाम, op_परिणाम);
		exit_status++;
	}
}

व्योम do_cleari(पूर्णांक argc, ss_argv_t argv,
	       पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	       व्योम *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णांक inode;
	पूर्णांक err;
	पूर्णांक test_परिणाम, op_परिणाम;

	अगर (check_fs_open(argv[0]))
		वापस;

	अगर (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		वापस;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	अगर (err)
		वापस;

	test_परिणाम = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_परिणाम = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	म_लिखो("Clearing inode %u, was %s before\n", inode, op_परिणाम ?
	       "set" : "clear");
	अगर (!test_परिणाम != !op_परिणाम) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_परिणाम, op_परिणाम);
		exit_status++;
	}
}

व्योम do_testi(पूर्णांक argc, ss_argv_t argv,
	      पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	      व्योम *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णांक inode;
	पूर्णांक err;
	पूर्णांक test_परिणाम;

	अगर (check_fs_open(argv[0]))
		वापस;

	अगर (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		वापस;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	अगर (err)
		वापस;

	test_परिणाम = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	म_लिखो("Inode %u is %s\n", inode, test_परिणाम ? "set" : "clear");
}

व्योम do_ffzi(पूर्णांक argc, ss_argv_t argv,
	     पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	     व्योम *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णांक start, end;
	पूर्णांक err;
	errcode_t retval;
	ext2_ino_t out;

	अगर (check_fs_open(argv[0]))
		वापस;

	अगर (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		वापस;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	अगर (err)
		वापस;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	अगर (err)
		वापस;

	retval = ext2fs_find_first_zero_inode_bitmap2(test_fs->inode_map,
						      start, end, &out);
	अगर (retval) {
		म_लिखो("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_message(retval));
		वापस;
	}
	म_लिखो("First unmarked inode is %u\n", out);
}

व्योम do_ffsi(पूर्णांक argc, ss_argv_t argv,
	     पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	     व्योम *infop EXT2FS_ATTR((unused)))
{
	अचिन्हित पूर्णांक start, end;
	पूर्णांक err;
	errcode_t retval;
	ext2_ino_t out;

	अगर (check_fs_open(argv[0]))
		वापस;

	अगर (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		वापस;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	अगर (err)
		वापस;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	अगर (err)
		वापस;

	retval = ext2fs_find_first_set_inode_bitmap2(test_fs->inode_map,
						     start, end, &out);
	अगर (retval) {
		म_लिखो("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_message(retval));
		वापस;
	}
	म_लिखो("First marked inode is %u\n", out);
}

व्योम do_zeroi(पूर्णांक argc, ss_argv_t argv,
	      पूर्णांक sci_idx EXT2FS_ATTR((unused)),
	      व्योम *infop EXT2FS_ATTR((unused)))
{
	अगर (check_fs_open(argv[0]))
		वापस;

	म_लिखो("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

पूर्णांक मुख्य(पूर्णांक argc, अक्षर **argv)
{
	अचिन्हित पूर्णांक	blocks = 128;
	अचिन्हित पूर्णांक	inodes = 0;
	अचिन्हित पूर्णांक	type = EXT2FS_BMAP64_BITARRAY;
	पूर्णांक		c, err, code;
	अक्षर		*request = (अक्षर *)शून्य;
	अक्षर		*cmd_file = 0;
	पूर्णांक		sci_idx;
	पूर्णांक		flags = EXT2_FLAG_64BITS;

	add_error_table(&et_ss_error_table);
	add_error_table(&et_ext2_error_table);
	जबतक ((c = getopt (argc, argv, "b:i:lt:R:f:")) != खातापूर्ण) {
		चयन (c) {
		हाल 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			अगर (err)
				निकास(1);
			अवरोध;
		हाल 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			अगर (err)
				निकास(1);
			अवरोध;
		हाल 'l':	/* Legacy bitmaps */
			flags = 0;
			अवरोध;
		हाल 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			अगर (err)
				निकास(1);
			अवरोध;
		हाल 'R':
			request = optarg;
			अवरोध;
		हाल 'f':
			cmd_file = optarg;
			अवरोध;
		शेष:
			com_err(argv[0], 0, "Usage: %s [-R request] "
				"[-f cmd_file]", subsystem_name);
			निकास(1);
		}
	}

	sci_idx = ss_create_invocation(subsystem_name, version,
				       (अक्षर *)शून्य, &tst_bitmaps_cmds, &code);
	अगर (code) {
		ss_perror(sci_idx, code, "creating invocation");
		निकास(1);
	}

	(व्योम) ss_add_request_table (sci_idx, &ss_std_requests, 1, &code);
	अगर (code) {
		ss_perror(sci_idx, code, "adding standard requests");
		निकास (1);
	}

	म_लिखो("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, version);

	setup_filesystem(argv[0], blocks, inodes, type, flags);

	अगर (request) {
		code = ss_execute_पंक्ति(sci_idx, request);
		अगर (code) {
			ss_perror(sci_idx, code, request);
			exit_status++;
		}
	} अन्यथा अगर (cmd_file) {
		exit_status = source_file(cmd_file, sci_idx);
	} अन्यथा {
		ss_listen(sci_idx);
	}

	निकास(exit_status);
}

