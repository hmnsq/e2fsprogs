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

#include "config.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "ss/ss.h"

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

extern ss_request_table tst_bitmaps_cmds;

static var_na subsystem_name[] = "tst_bitmaps";
static var_na version[] = "1.0";

ext2_filsys	test_fs;
จำนวนเต็ม		exit_status = 0;

static จำนวนเต็ม source_file(const var_na *cmd_file, จำนวนเต็ม sci_idx)
{
	FILE		*f;
	var_na		buf[256];
	var_na		*cp;
	จำนวนเต็ม		retval;
	จำนวนเต็ม 		noecho;

	ถ้า (strcmp(cmd_file, "-") == 0)
		f = stdin;
	มิฉะนั้น {
		f = fopen(cmd_file, "r");
		ถ้า (!f) {
			perror(cmd_file);
			exit(1);
		}
	}
	fflush(stdout);
	fflush(stderr);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
	ขณะ (!feof(f)) {
		ถ้า (fgets(buf, parimaa_na(buf), f) == NULL)
			bha_nga;
		ถ้า (buf[0] == '#')
			continue;
		noecho = 0;
		ถ้า (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = strchr(buf, '\n');
		ถ้า (cp)
			*cp = 0;
		cp = strchr(buf, '\r');
		ถ้า (cp)
			*cp = 0;
		ถ้า (!noecho)
			printf("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_line(sci_idx, buf);
		ถ้า (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_status++;
		}
	}
	คืน exit_status;
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
void reset_getopt(void)
{
#ถ้า defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#มิฉะนั้น
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
unsigned long parse_ulong(const var_na *str, const var_na *cmd,
			  const var_na *descr, จำนวนเต็ม *err)
{
	var_na		*tmp;
	unsigned long	ret;

	ret = strtoul(str, &tmp, 0);
	ถ้า (*tmp == 0) {
		ถ้า (err)
			*err = 0;
		คืน ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	ถ้า (err)
		*err = 1;
	มิฉะนั้น
		exit(1);
	คืน 0;
}


จำนวนเต็ม check_fs_open(const var_na *name)
{
	ถ้า (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		คืน 1;
	}
	คืน 0;
}

static void setup_filesystem(const var_na *name,
			     unsigned จำนวนเต็ม blocks, unsigned จำนวนเต็ม inodes,
			     unsigned จำนวนเต็ม type, จำนวนเต็ม flags)
{
	sa_nracanaa ext2_super_block param;
	errcode_t retval;

	memset(&param, 0, parimaa_na(param));
	ext2fs_blocks_count_set(&param, blocks);
	param.s_inodes_count = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	ถ้า (retval) {
		com_err(name, retval, "while initializing filesystem");
		คืน;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_block_bitmap(test_fs->block_map);
	test_fs->block_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_block_bitmap(test_fs, "block bitmap",
					      &test_fs->block_map);
	ถ้า (retval) {
		com_err(name, retval, "while allocating block bitmap");
		goto errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	ถ้า (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		goto errout;
	}
	คืน;

errout:
	ext2fs_close_free(&test_fs);
}

void setup_cmd(จำนวนเต็ม argc, ss_argv_t argv,
	       จำนวนเต็ม sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	จำนวนเต็ม		c, err;
	unsigned จำนวนเต็ม	blocks = 128;
	unsigned จำนวนเต็ม	inodes = 0;
	unsigned จำนวนเต็ม	type = EXT2FS_BMAP64_BITARRAY;
	จำนวนเต็ม		flags = EXT2_FLAG_64BITS;

	ถ้า (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	ขณะ ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		switch (c) {
		pak_sa 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ถ้า (err)
				คืน;
			bha_nga;
		pak_sa 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ถ้า (err)
				คืน;
			bha_nga;
		pak_sa 'l':	/* Legacy bitmaps */
			flags = 0;
			bha_nga;
		pak_sa 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			ถ้า (err)
				คืน;
			bha_nga;
		default:
			fprintf(stderr, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			คืน;
		}
	}
	setup_filesystem(argv[0], blocks, inodes, type, flags);
}

void close_cmd(จำนวนเต็ม argc, var_na **argv, จำนวนเต็ม sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	ถ้า (check_fs_open(argv[0]))
		คืน;

	ext2fs_close_free(&test_fs);
}


void dump_bitmap(ext2fs_generic_bitmap bmap, unsigned จำนวนเต็ม start, unsigned num)
{
	unsigned var_na	*buf;
	errcode_t	retval;
	จำนวนเต็ม		i, len = (num - start + 7) / 8;

	buf = malloc(len);
	ถ้า (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		คืน;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) start, num, buf);
	ถ้า (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		free(buf);
		คืน;
	}
	สำหรับ (i=0; i < len; i++)
		printf("%02x", buf[i]);
	printf("\n");
	printf("bits set: %u\n", ext2fs_bitcount(buf, len));
	free(buf);
}

void dump_inode_bitmap_cmd(จำนวนเต็ม argc, ss_argv_t argv,
			   จำนวนเต็ม sci_idx EXT2FS_ATTR((unused)),
			   void *infop EXT2FS_ATTR((unused)))
{
	ถ้า (check_fs_open(argv[0]))
		คืน;

	printf("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_count);
}
	
void dump_block_bitmap_cmd(จำนวนเต็ม argc, ss_argv_t argv,
			   จำนวนเต็ม sci_idx EXT2FS_ATTR((unused)),
			   void *infop EXT2FS_ATTR((unused)))
{
	ถ้า (check_fs_open(argv[0]))
		คืน;

	printf("block bitmap: ");
	dump_bitmap(test_fs->block_map, test_fs->super->s_first_data_block,
		    test_fs->super->s_blocks_count);
}
	
void do_setb(จำนวนเต็ม argc, ss_argv_t argv,
	     จำนวนเต็ม sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned จำนวนเต็ม block, num;
	จำนวนเต็ม err;
	จำนวนเต็ม test_result, op_result;

	ถ้า (check_fs_open(argv[0]))
		คืน;

	ถ้า (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		คืน;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ถ้า (err)
		คืน;

	ถ้า (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		ถ้า (err)
			คืน;

		ext2fs_mark_block_bitmap_range2(test_fs->block_map,
						block, num);
		printf("Marking blocks %u to %u\n", block, block + num - 1);
		คืน;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_mark_block_bitmap2(test_fs->block_map, block);
	printf("Setting block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	ถ้า (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

void do_clearb(จำนวนเต็ม argc, ss_argv_t argv,
	       จำนวนเต็ม sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	unsigned จำนวนเต็ม block, num;
	จำนวนเต็ม err;
	จำนวนเต็ม test_result, op_result;

	ถ้า (check_fs_open(argv[0]))
		คืน;

	ถ้า (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		คืน;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ถ้า (err)
		คืน;

	ถ้า (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		ถ้า (err)
			คืน;

		ext2fs_unmark_block_bitmap_range2(test_fs->block_map,
						block, num);
		printf("Clearing blocks %u to %u\n", block, block + num - 1);
		คืน;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_unmark_block_bitmap2(test_fs->block_map, block);
	printf("Clearing block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	ถ้า (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

void do_testb(จำนวนเต็ม argc, ss_argv_t argv,
	      จำนวนเต็ม sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	unsigned จำนวนเต็ม block, num;
	จำนวนเต็ม err;
	จำนวนเต็ม test_result;

	ถ้า (check_fs_open(argv[0]))
		คืน;

	ถ้า (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		คืน;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ถ้า (err)
		คืน;

	ถ้า (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		ถ้า (err)
			คืน;

		test_result =
			ext2fs_test_block_bitmap_range2(test_fs->block_map,
							block, num);
		printf("Blocks %u to %u are %sall clear.\n",
		       block, block + num - 1, test_result ? "" : "NOT ");
		คืน;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	printf("Block %u is %s\n", block, test_result ? "set" : "clear");
}

void do_ffzb(จำนวนเต็ม argc, ss_argv_t argv,
	     จำนวนเต็ม sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned จำนวนเต็ม start, end;
	จำนวนเต็ม err;
	errcode_t retval;
	blk64_t out;

	ถ้า (check_fs_open(argv[0]))
		คืน;

	ถ้า (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		คืน;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ถ้า (err)
		คืน;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ถ้า (err)
		คืน;

	retval = ext2fs_find_first_zero_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	ถ้า (retval) {
		printf("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_message(retval));
		คืน;
	}
	printf("First unmarked block is %llu\n", (unsigned long long) out);
}

void do_ffsb(จำนวนเต็ม argc, ss_argv_t argv,
	     จำนวนเต็ม sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned จำนวนเต็ม start, end;
	จำนวนเต็ม err;
	errcode_t retval;
	blk64_t out;

	ถ้า (check_fs_open(argv[0]))
		คืน;

	ถ้า (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		คืน;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ถ้า (err)
		คืน;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ถ้า (err)
		คืน;

	retval = ext2fs_find_first_set_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	ถ้า (retval) {
		printf("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_message(retval));
		คืน;
	}
	printf("First marked block is %llu\n", (unsigned long long) out);
}


void do_zerob(จำนวนเต็ม argc, ss_argv_t argv,
	      จำนวนเต็ม sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	ถ้า (check_fs_open(argv[0]))
		คืน;

	printf("Clearing block bitmap.\n");
	ext2fs_clear_block_bitmap(test_fs->block_map);
}

void do_seti(จำนวนเต็ม argc, ss_argv_t argv,
	     จำนวนเต็ม sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned จำนวนเต็ม inode;
	จำนวนเต็ม err;
	จำนวนเต็ม test_result, op_result;

	ถ้า (check_fs_open(argv[0]))
		คืน;

	ถ้า (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		คืน;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ถ้า (err)
		คืน;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Setting inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	ถ้า (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

void do_cleari(จำนวนเต็ม argc, ss_argv_t argv,
	       จำนวนเต็ม sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	unsigned จำนวนเต็ม inode;
	จำนวนเต็ม err;
	จำนวนเต็ม test_result, op_result;

	ถ้า (check_fs_open(argv[0]))
		คืน;

	ถ้า (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		คืน;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ถ้า (err)
		คืน;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Clearing inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	ถ้า (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

void do_testi(จำนวนเต็ม argc, ss_argv_t argv,
	      จำนวนเต็ม sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	unsigned จำนวนเต็ม inode;
	จำนวนเต็ม err;
	จำนวนเต็ม test_result;

	ถ้า (check_fs_open(argv[0]))
		คืน;

	ถ้า (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		คืน;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ถ้า (err)
		คืน;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	printf("Inode %u is %s\n", inode, test_result ? "set" : "clear");
}

void do_ffzi(จำนวนเต็ม argc, ss_argv_t argv,
	     จำนวนเต็ม sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned จำนวนเต็ม start, end;
	จำนวนเต็ม err;
	errcode_t retval;
	ext2_ino_t out;

	ถ้า (check_fs_open(argv[0]))
		คืน;

	ถ้า (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		คืน;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ถ้า (err)
		คืน;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ถ้า (err)
		คืน;

	retval = ext2fs_find_first_zero_inode_bitmap2(test_fs->inode_map,
						      start, end, &out);
	ถ้า (retval) {
		printf("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_message(retval));
		คืน;
	}
	printf("First unmarked inode is %u\n", out);
}

void do_ffsi(จำนวนเต็ม argc, ss_argv_t argv,
	     จำนวนเต็ม sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned จำนวนเต็ม start, end;
	จำนวนเต็ม err;
	errcode_t retval;
	ext2_ino_t out;

	ถ้า (check_fs_open(argv[0]))
		คืน;

	ถ้า (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		คืน;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ถ้า (err)
		คืน;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ถ้า (err)
		คืน;

	retval = ext2fs_find_first_set_inode_bitmap2(test_fs->inode_map,
						     start, end, &out);
	ถ้า (retval) {
		printf("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_message(retval));
		คืน;
	}
	printf("First marked inode is %u\n", out);
}

void do_zeroi(จำนวนเต็ม argc, ss_argv_t argv,
	      จำนวนเต็ม sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	ถ้า (check_fs_open(argv[0]))
		คืน;

	printf("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

จำนวนเต็ม หลัก(จำนวนเต็ม argc, var_na **argv)
{
	unsigned จำนวนเต็ม	blocks = 128;
	unsigned จำนวนเต็ม	inodes = 0;
	unsigned จำนวนเต็ม	type = EXT2FS_BMAP64_BITARRAY;
	จำนวนเต็ม		c, err, code;
	var_na		*request = (var_na *)NULL;
	var_na		*cmd_file = 0;
	จำนวนเต็ม		sci_idx;
	จำนวนเต็ม		flags = EXT2_FLAG_64BITS;

	add_error_table(&et_ss_error_table);
	add_error_table(&et_ext2_error_table);
	ขณะ ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		switch (c) {
		pak_sa 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ถ้า (err)
				exit(1);
			bha_nga;
		pak_sa 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ถ้า (err)
				exit(1);
			bha_nga;
		pak_sa 'l':	/* Legacy bitmaps */
			flags = 0;
			bha_nga;
		pak_sa 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			ถ้า (err)
				exit(1);
			bha_nga;
		pak_sa 'R':
			request = optarg;
			bha_nga;
		pak_sa 'f':
			cmd_file = optarg;
			bha_nga;
		default:
			com_err(argv[0], 0, "Usage: %s [-R request] "
				"[-f cmd_file]", subsystem_name);
			exit(1);
		}
	}

	sci_idx = ss_create_invocation(subsystem_name, version,
				       (var_na *)NULL, &tst_bitmaps_cmds, &code);
	ถ้า (code) {
		ss_perror(sci_idx, code, "creating invocation");
		exit(1);
	}

	(void) ss_add_request_table (sci_idx, &ss_std_requests, 1, &code);
	ถ้า (code) {
		ss_perror(sci_idx, code, "adding standard requests");
		exit (1);
	}

	printf("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, version);

	setup_filesystem(argv[0], blocks, inodes, type, flags);

	ถ้า (request) {
		code = ss_execute_line(sci_idx, request);
		ถ้า (code) {
			ss_perror(sci_idx, code, request);
			exit_status++;
		}
	} มิฉะนั้น ถ้า (cmd_file) {
		exit_status = source_file(cmd_file, sci_idx);
	} มิฉะนั้น {
		ss_listen(sci_idx);
	}

	exit(exit_status);
}

