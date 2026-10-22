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
puur_naa_nka		exit_status = 0;

static puur_naa_nka source_file(const var_na *cmd_file, puur_naa_nka sci_idx)
{
	FILE		*f;
	var_na		buf[256];
	var_na		*cp;
	puur_naa_nka		retval;
	puur_naa_nka 		noecho;

	ꦪꦼꦤ꧀ (strcmp(cmd_file, "-") == 0)
		f = stdin;
	ꦭꦶꦪ {
		f = fopen(cmd_file, "r");
		ꦪꦼꦤ꧀ (!f) {
			perror(cmd_file);
			exit(1);
		}
	}
	fflush(stdout);
	fflush(stderr);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
	ꦱꦼꦢꦼꦭ (!feof(f)) {
		ꦪꦼꦤ꧀ (fgets(buf, parimaa_na(buf), f) == NULL)
			bha_nga;
		ꦪꦼꦤ꧀ (buf[0] == '#')
			continue;
		noecho = 0;
		ꦪꦼꦤ꧀ (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = strchr(buf, '\n');
		ꦪꦼꦤ꧀ (cp)
			*cp = 0;
		cp = strchr(buf, '\r');
		ꦪꦼꦤ꧀ (cp)
			*cp = 0;
		ꦪꦼꦤ꧀ (!noecho)
			printf("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_line(sci_idx, buf);
		ꦪꦼꦤ꧀ (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_status++;
		}
	}
	ꦮꦭꦶ exit_status;
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
#ꦪꦼꦤ꧀ defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#ꦭꦶꦪ
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
			  const var_na *descr, puur_naa_nka *err)
{
	var_na		*tmp;
	unsigned long	ret;

	ret = strtoul(str, &tmp, 0);
	ꦪꦼꦤ꧀ (*tmp == 0) {
		ꦪꦼꦤ꧀ (err)
			*err = 0;
		ꦮꦭꦶ ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	ꦪꦼꦤ꧀ (err)
		*err = 1;
	ꦭꦶꦪ
		exit(1);
	ꦮꦭꦶ 0;
}


puur_naa_nka check_fs_open(const var_na *name)
{
	ꦪꦼꦤ꧀ (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		ꦮꦭꦶ 1;
	}
	ꦮꦭꦶ 0;
}

static void setup_filesystem(const var_na *name,
			     unsigned puur_naa_nka blocks, unsigned puur_naa_nka inodes,
			     unsigned puur_naa_nka type, puur_naa_nka flags)
{
	sa_nracanaa ext2_super_block param;
	errcode_t retval;

	memset(&param, 0, parimaa_na(param));
	ext2fs_blocks_count_set(&param, blocks);
	param.s_inodes_count = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	ꦪꦼꦤ꧀ (retval) {
		com_err(name, retval, "while initializing filesystem");
		ꦮꦭꦶ;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_block_bitmap(test_fs->block_map);
	test_fs->block_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_block_bitmap(test_fs, "block bitmap",
					      &test_fs->block_map);
	ꦪꦼꦤ꧀ (retval) {
		com_err(name, retval, "while allocating block bitmap");
		goto errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	ꦪꦼꦤ꧀ (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		goto errout;
	}
	ꦮꦭꦶ;

errout:
	ext2fs_close_free(&test_fs);
}

void setup_cmd(puur_naa_nka argc, ss_argv_t argv,
	       puur_naa_nka sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	puur_naa_nka		c, err;
	unsigned puur_naa_nka	blocks = 128;
	unsigned puur_naa_nka	inodes = 0;
	unsigned puur_naa_nka	type = EXT2FS_BMAP64_BITARRAY;
	puur_naa_nka		flags = EXT2_FLAG_64BITS;

	ꦪꦼꦤ꧀ (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	ꦱꦼꦢꦼꦭ ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		switch (c) {
		pak_sa 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ꦪꦼꦤ꧀ (err)
				ꦮꦭꦶ;
			bha_nga;
		pak_sa 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ꦪꦼꦤ꧀ (err)
				ꦮꦭꦶ;
			bha_nga;
		pak_sa 'l':	/* Legacy bitmaps */
			flags = 0;
			bha_nga;
		pak_sa 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			ꦪꦼꦤ꧀ (err)
				ꦮꦭꦶ;
			bha_nga;
		default:
			fprintf(stderr, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			ꦮꦭꦶ;
		}
	}
	setup_filesystem(argv[0], blocks, inodes, type, flags);
}

void close_cmd(puur_naa_nka argc, var_na **argv, puur_naa_nka sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	ꦪꦼꦤ꧀ (check_fs_open(argv[0]))
		ꦮꦭꦶ;

	ext2fs_close_free(&test_fs);
}


void dump_bitmap(ext2fs_generic_bitmap bmap, unsigned puur_naa_nka start, unsigned num)
{
	unsigned var_na	*buf;
	errcode_t	retval;
	puur_naa_nka		i, len = (num - start + 7) / 8;

	buf = malloc(len);
	ꦪꦼꦤ꧀ (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		ꦮꦭꦶ;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) start, num, buf);
	ꦪꦼꦤ꧀ (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		free(buf);
		ꦮꦭꦶ;
	}
	ꦏꦁꦒꦺꦴ (i=0; i < len; i++)
		printf("%02x", buf[i]);
	printf("\n");
	printf("bits set: %u\n", ext2fs_bitcount(buf, len));
	free(buf);
}

void dump_inode_bitmap_cmd(puur_naa_nka argc, ss_argv_t argv,
			   puur_naa_nka sci_idx EXT2FS_ATTR((unused)),
			   void *infop EXT2FS_ATTR((unused)))
{
	ꦪꦼꦤ꧀ (check_fs_open(argv[0]))
		ꦮꦭꦶ;

	printf("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_count);
}
	
void dump_block_bitmap_cmd(puur_naa_nka argc, ss_argv_t argv,
			   puur_naa_nka sci_idx EXT2FS_ATTR((unused)),
			   void *infop EXT2FS_ATTR((unused)))
{
	ꦪꦼꦤ꧀ (check_fs_open(argv[0]))
		ꦮꦭꦶ;

	printf("block bitmap: ");
	dump_bitmap(test_fs->block_map, test_fs->super->s_first_data_block,
		    test_fs->super->s_blocks_count);
}
	
void do_setb(puur_naa_nka argc, ss_argv_t argv,
	     puur_naa_nka sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned puur_naa_nka block, num;
	puur_naa_nka err;
	puur_naa_nka test_result, op_result;

	ꦪꦼꦤ꧀ (check_fs_open(argv[0]))
		ꦮꦭꦶ;

	ꦪꦼꦤ꧀ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		ꦮꦭꦶ;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ꦪꦼꦤ꧀ (err)
		ꦮꦭꦶ;

	ꦪꦼꦤ꧀ (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		ꦪꦼꦤ꧀ (err)
			ꦮꦭꦶ;

		ext2fs_mark_block_bitmap_range2(test_fs->block_map,
						block, num);
		printf("Marking blocks %u to %u\n", block, block + num - 1);
		ꦮꦭꦶ;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_mark_block_bitmap2(test_fs->block_map, block);
	printf("Setting block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	ꦪꦼꦤ꧀ (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

void do_clearb(puur_naa_nka argc, ss_argv_t argv,
	       puur_naa_nka sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	unsigned puur_naa_nka block, num;
	puur_naa_nka err;
	puur_naa_nka test_result, op_result;

	ꦪꦼꦤ꧀ (check_fs_open(argv[0]))
		ꦮꦭꦶ;

	ꦪꦼꦤ꧀ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		ꦮꦭꦶ;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ꦪꦼꦤ꧀ (err)
		ꦮꦭꦶ;

	ꦪꦼꦤ꧀ (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		ꦪꦼꦤ꧀ (err)
			ꦮꦭꦶ;

		ext2fs_unmark_block_bitmap_range2(test_fs->block_map,
						block, num);
		printf("Clearing blocks %u to %u\n", block, block + num - 1);
		ꦮꦭꦶ;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_unmark_block_bitmap2(test_fs->block_map, block);
	printf("Clearing block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	ꦪꦼꦤ꧀ (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

void do_testb(puur_naa_nka argc, ss_argv_t argv,
	      puur_naa_nka sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	unsigned puur_naa_nka block, num;
	puur_naa_nka err;
	puur_naa_nka test_result;

	ꦪꦼꦤ꧀ (check_fs_open(argv[0]))
		ꦮꦭꦶ;

	ꦪꦼꦤ꧀ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		ꦮꦭꦶ;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ꦪꦼꦤ꧀ (err)
		ꦮꦭꦶ;

	ꦪꦼꦤ꧀ (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		ꦪꦼꦤ꧀ (err)
			ꦮꦭꦶ;

		test_result =
			ext2fs_test_block_bitmap_range2(test_fs->block_map,
							block, num);
		printf("Blocks %u to %u are %sall clear.\n",
		       block, block + num - 1, test_result ? "" : "NOT ");
		ꦮꦭꦶ;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	printf("Block %u is %s\n", block, test_result ? "set" : "clear");
}

void do_ffzb(puur_naa_nka argc, ss_argv_t argv,
	     puur_naa_nka sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned puur_naa_nka start, end;
	puur_naa_nka err;
	errcode_t retval;
	blk64_t out;

	ꦪꦼꦤ꧀ (check_fs_open(argv[0]))
		ꦮꦭꦶ;

	ꦪꦼꦤ꧀ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		ꦮꦭꦶ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ꦪꦼꦤ꧀ (err)
		ꦮꦭꦶ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ꦪꦼꦤ꧀ (err)
		ꦮꦭꦶ;

	retval = ext2fs_find_first_zero_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	ꦪꦼꦤ꧀ (retval) {
		printf("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_message(retval));
		ꦮꦭꦶ;
	}
	printf("First unmarked block is %llu\n", (unsigned long long) out);
}

void do_ffsb(puur_naa_nka argc, ss_argv_t argv,
	     puur_naa_nka sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned puur_naa_nka start, end;
	puur_naa_nka err;
	errcode_t retval;
	blk64_t out;

	ꦪꦼꦤ꧀ (check_fs_open(argv[0]))
		ꦮꦭꦶ;

	ꦪꦼꦤ꧀ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		ꦮꦭꦶ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ꦪꦼꦤ꧀ (err)
		ꦮꦭꦶ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ꦪꦼꦤ꧀ (err)
		ꦮꦭꦶ;

	retval = ext2fs_find_first_set_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	ꦪꦼꦤ꧀ (retval) {
		printf("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_message(retval));
		ꦮꦭꦶ;
	}
	printf("First marked block is %llu\n", (unsigned long long) out);
}


void do_zerob(puur_naa_nka argc, ss_argv_t argv,
	      puur_naa_nka sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	ꦪꦼꦤ꧀ (check_fs_open(argv[0]))
		ꦮꦭꦶ;

	printf("Clearing block bitmap.\n");
	ext2fs_clear_block_bitmap(test_fs->block_map);
}

void do_seti(puur_naa_nka argc, ss_argv_t argv,
	     puur_naa_nka sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned puur_naa_nka inode;
	puur_naa_nka err;
	puur_naa_nka test_result, op_result;

	ꦪꦼꦤ꧀ (check_fs_open(argv[0]))
		ꦮꦭꦶ;

	ꦪꦼꦤ꧀ (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		ꦮꦭꦶ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ꦪꦼꦤ꧀ (err)
		ꦮꦭꦶ;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Setting inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	ꦪꦼꦤ꧀ (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

void do_cleari(puur_naa_nka argc, ss_argv_t argv,
	       puur_naa_nka sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	unsigned puur_naa_nka inode;
	puur_naa_nka err;
	puur_naa_nka test_result, op_result;

	ꦪꦼꦤ꧀ (check_fs_open(argv[0]))
		ꦮꦭꦶ;

	ꦪꦼꦤ꧀ (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		ꦮꦭꦶ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ꦪꦼꦤ꧀ (err)
		ꦮꦭꦶ;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Clearing inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	ꦪꦼꦤ꧀ (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

void do_testi(puur_naa_nka argc, ss_argv_t argv,
	      puur_naa_nka sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	unsigned puur_naa_nka inode;
	puur_naa_nka err;
	puur_naa_nka test_result;

	ꦪꦼꦤ꧀ (check_fs_open(argv[0]))
		ꦮꦭꦶ;

	ꦪꦼꦤ꧀ (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		ꦮꦭꦶ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ꦪꦼꦤ꧀ (err)
		ꦮꦭꦶ;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	printf("Inode %u is %s\n", inode, test_result ? "set" : "clear");
}

void do_ffzi(puur_naa_nka argc, ss_argv_t argv,
	     puur_naa_nka sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned puur_naa_nka start, end;
	puur_naa_nka err;
	errcode_t retval;
	ext2_ino_t out;

	ꦪꦼꦤ꧀ (check_fs_open(argv[0]))
		ꦮꦭꦶ;

	ꦪꦼꦤ꧀ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		ꦮꦭꦶ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ꦪꦼꦤ꧀ (err)
		ꦮꦭꦶ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ꦪꦼꦤ꧀ (err)
		ꦮꦭꦶ;

	retval = ext2fs_find_first_zero_inode_bitmap2(test_fs->inode_map,
						      start, end, &out);
	ꦪꦼꦤ꧀ (retval) {
		printf("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_message(retval));
		ꦮꦭꦶ;
	}
	printf("First unmarked inode is %u\n", out);
}

void do_ffsi(puur_naa_nka argc, ss_argv_t argv,
	     puur_naa_nka sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned puur_naa_nka start, end;
	puur_naa_nka err;
	errcode_t retval;
	ext2_ino_t out;

	ꦪꦼꦤ꧀ (check_fs_open(argv[0]))
		ꦮꦭꦶ;

	ꦪꦼꦤ꧀ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		ꦮꦭꦶ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ꦪꦼꦤ꧀ (err)
		ꦮꦭꦶ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ꦪꦼꦤ꧀ (err)
		ꦮꦭꦶ;

	retval = ext2fs_find_first_set_inode_bitmap2(test_fs->inode_map,
						     start, end, &out);
	ꦪꦼꦤ꧀ (retval) {
		printf("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_message(retval));
		ꦮꦭꦶ;
	}
	printf("First marked inode is %u\n", out);
}

void do_zeroi(puur_naa_nka argc, ss_argv_t argv,
	      puur_naa_nka sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	ꦪꦼꦤ꧀ (check_fs_open(argv[0]))
		ꦮꦭꦶ;

	printf("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

puur_naa_nka main(puur_naa_nka argc, var_na **argv)
{
	unsigned puur_naa_nka	blocks = 128;
	unsigned puur_naa_nka	inodes = 0;
	unsigned puur_naa_nka	type = EXT2FS_BMAP64_BITARRAY;
	puur_naa_nka		c, err, code;
	var_na		*request = (var_na *)NULL;
	var_na		*cmd_file = 0;
	puur_naa_nka		sci_idx;
	puur_naa_nka		flags = EXT2_FLAG_64BITS;

	add_error_table(&et_ss_error_table);
	add_error_table(&et_ext2_error_table);
	ꦱꦼꦢꦼꦭ ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		switch (c) {
		pak_sa 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ꦪꦼꦤ꧀ (err)
				exit(1);
			bha_nga;
		pak_sa 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ꦪꦼꦤ꧀ (err)
				exit(1);
			bha_nga;
		pak_sa 'l':	/* Legacy bitmaps */
			flags = 0;
			bha_nga;
		pak_sa 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			ꦪꦼꦤ꧀ (err)
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
	ꦪꦼꦤ꧀ (code) {
		ss_perror(sci_idx, code, "creating invocation");
		exit(1);
	}

	(void) ss_add_request_table (sci_idx, &ss_std_requests, 1, &code);
	ꦪꦼꦤ꧀ (code) {
		ss_perror(sci_idx, code, "adding standard requests");
		exit (1);
	}

	printf("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, version);

	setup_filesystem(argv[0], blocks, inodes, type, flags);

	ꦪꦼꦤ꧀ (request) {
		code = ss_execute_line(sci_idx, request);
		ꦪꦼꦤ꧀ (code) {
			ss_perror(sci_idx, code, request);
			exit_status++;
		}
	} ꦭꦶꦪ ꦪꦼꦤ꧀ (cmd_file) {
		exit_status = source_file(cmd_file, sci_idx);
	} ꦭꦶꦪ {
		ss_listen(sci_idx);
	}

	exit(exit_status);
}

