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
ចំនួនគត់		exit_status = 0;

static ចំនួនគត់ source_file(const var_na *cmd_file, ចំនួនគត់ sci_idx)
{
	FILE		*f;
	var_na		buf[256];
	var_na		*cp;
	ចំនួនគត់		retval;
	ចំនួនគត់ 		noecho;

	ប្រសិនបើ (strcmp(cmd_file, "-") == 0)
		f = stdin;
	បើមិនដូច្នេះ {
		f = fopen(cmd_file, "r");
		ប្រសិនបើ (!f) {
			perror(cmd_file);
			exit(1);
		}
	}
	fflush(stdout);
	fflush(stderr);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
	ខណៈពេល (!feof(f)) {
		ប្រសិនបើ (fgets(buf, parimaa_na(buf), f) == NULL)
			bha_nga;
		ប្រសិនបើ (buf[0] == '#')
			continue;
		noecho = 0;
		ប្រសិនបើ (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = strchr(buf, '\n');
		ប្រសិនបើ (cp)
			*cp = 0;
		cp = strchr(buf, '\r');
		ប្រសិនបើ (cp)
			*cp = 0;
		ប្រសិនបើ (!noecho)
			printf("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_line(sci_idx, buf);
		ប្រសិនបើ (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_status++;
		}
	}
	ត្រឡប់ exit_status;
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
#ប្រសិនបើ defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#បើមិនដូច្នេះ
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
			  const var_na *descr, ចំនួនគត់ *err)
{
	var_na		*tmp;
	unsigned long	ret;

	ret = strtoul(str, &tmp, 0);
	ប្រសិនបើ (*tmp == 0) {
		ប្រសិនបើ (err)
			*err = 0;
		ត្រឡប់ ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	ប្រសិនបើ (err)
		*err = 1;
	បើមិនដូច្នេះ
		exit(1);
	ត្រឡប់ 0;
}


ចំនួនគត់ check_fs_open(const var_na *name)
{
	ប្រសិនបើ (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		ត្រឡប់ 1;
	}
	ត្រឡប់ 0;
}

static void setup_filesystem(const var_na *name,
			     unsigned ចំនួនគត់ blocks, unsigned ចំនួនគត់ inodes,
			     unsigned ចំនួនគត់ type, ចំនួនគត់ flags)
{
	sa_nracanaa ext2_super_block param;
	errcode_t retval;

	memset(&param, 0, parimaa_na(param));
	ext2fs_blocks_count_set(&param, blocks);
	param.s_inodes_count = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	ប្រសិនបើ (retval) {
		com_err(name, retval, "while initializing filesystem");
		ត្រឡប់;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_block_bitmap(test_fs->block_map);
	test_fs->block_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_block_bitmap(test_fs, "block bitmap",
					      &test_fs->block_map);
	ប្រសិនបើ (retval) {
		com_err(name, retval, "while allocating block bitmap");
		goto errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	ប្រសិនបើ (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		goto errout;
	}
	ត្រឡប់;

errout:
	ext2fs_close_free(&test_fs);
}

void setup_cmd(ចំនួនគត់ argc, ss_argv_t argv,
	       ចំនួនគត់ sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	ចំនួនគត់		c, err;
	unsigned ចំនួនគត់	blocks = 128;
	unsigned ចំនួនគត់	inodes = 0;
	unsigned ចំនួនគត់	type = EXT2FS_BMAP64_BITARRAY;
	ចំនួនគត់		flags = EXT2_FLAG_64BITS;

	ប្រសិនបើ (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	ខណៈពេល ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		switch (c) {
		pak_sa 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ប្រសិនបើ (err)
				ត្រឡប់;
			bha_nga;
		pak_sa 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ប្រសិនបើ (err)
				ត្រឡប់;
			bha_nga;
		pak_sa 'l':	/* Legacy bitmaps */
			flags = 0;
			bha_nga;
		pak_sa 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			ប្រសិនបើ (err)
				ត្រឡប់;
			bha_nga;
		default:
			fprintf(stderr, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			ត្រឡប់;
		}
	}
	setup_filesystem(argv[0], blocks, inodes, type, flags);
}

void close_cmd(ចំនួនគត់ argc, var_na **argv, ចំនួនគត់ sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	ប្រសិនបើ (check_fs_open(argv[0]))
		ត្រឡប់;

	ext2fs_close_free(&test_fs);
}


void dump_bitmap(ext2fs_generic_bitmap bmap, unsigned ចំនួនគត់ start, unsigned num)
{
	unsigned var_na	*buf;
	errcode_t	retval;
	ចំនួនគត់		i, len = (num - start + 7) / 8;

	buf = malloc(len);
	ប្រសិនបើ (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		ត្រឡប់;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) start, num, buf);
	ប្រសិនបើ (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		free(buf);
		ត្រឡប់;
	}
	សម្រាប់ (i=0; i < len; i++)
		printf("%02x", buf[i]);
	printf("\n");
	printf("bits set: %u\n", ext2fs_bitcount(buf, len));
	free(buf);
}

void dump_inode_bitmap_cmd(ចំនួនគត់ argc, ss_argv_t argv,
			   ចំនួនគត់ sci_idx EXT2FS_ATTR((unused)),
			   void *infop EXT2FS_ATTR((unused)))
{
	ប្រសិនបើ (check_fs_open(argv[0]))
		ត្រឡប់;

	printf("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_count);
}
	
void dump_block_bitmap_cmd(ចំនួនគត់ argc, ss_argv_t argv,
			   ចំនួនគត់ sci_idx EXT2FS_ATTR((unused)),
			   void *infop EXT2FS_ATTR((unused)))
{
	ប្រសិនបើ (check_fs_open(argv[0]))
		ត្រឡប់;

	printf("block bitmap: ");
	dump_bitmap(test_fs->block_map, test_fs->super->s_first_data_block,
		    test_fs->super->s_blocks_count);
}
	
void do_setb(ចំនួនគត់ argc, ss_argv_t argv,
	     ចំនួនគត់ sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned ចំនួនគត់ block, num;
	ចំនួនគត់ err;
	ចំនួនគត់ test_result, op_result;

	ប្រសិនបើ (check_fs_open(argv[0]))
		ត្រឡប់;

	ប្រសិនបើ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		ត្រឡប់;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ប្រសិនបើ (err)
		ត្រឡប់;

	ប្រសិនបើ (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		ប្រសិនបើ (err)
			ត្រឡប់;

		ext2fs_mark_block_bitmap_range2(test_fs->block_map,
						block, num);
		printf("Marking blocks %u to %u\n", block, block + num - 1);
		ត្រឡប់;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_mark_block_bitmap2(test_fs->block_map, block);
	printf("Setting block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	ប្រសិនបើ (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

void do_clearb(ចំនួនគត់ argc, ss_argv_t argv,
	       ចំនួនគត់ sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	unsigned ចំនួនគត់ block, num;
	ចំនួនគត់ err;
	ចំនួនគត់ test_result, op_result;

	ប្រសិនបើ (check_fs_open(argv[0]))
		ត្រឡប់;

	ប្រសិនបើ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		ត្រឡប់;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ប្រសិនបើ (err)
		ត្រឡប់;

	ប្រសិនបើ (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		ប្រសិនបើ (err)
			ត្រឡប់;

		ext2fs_unmark_block_bitmap_range2(test_fs->block_map,
						block, num);
		printf("Clearing blocks %u to %u\n", block, block + num - 1);
		ត្រឡប់;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_unmark_block_bitmap2(test_fs->block_map, block);
	printf("Clearing block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	ប្រសិនបើ (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

void do_testb(ចំនួនគត់ argc, ss_argv_t argv,
	      ចំនួនគត់ sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	unsigned ចំនួនគត់ block, num;
	ចំនួនគត់ err;
	ចំនួនគត់ test_result;

	ប្រសិនបើ (check_fs_open(argv[0]))
		ត្រឡប់;

	ប្រសិនបើ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		ត្រឡប់;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ប្រសិនបើ (err)
		ត្រឡប់;

	ប្រសិនបើ (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		ប្រសិនបើ (err)
			ត្រឡប់;

		test_result =
			ext2fs_test_block_bitmap_range2(test_fs->block_map,
							block, num);
		printf("Blocks %u to %u are %sall clear.\n",
		       block, block + num - 1, test_result ? "" : "NOT ");
		ត្រឡប់;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	printf("Block %u is %s\n", block, test_result ? "set" : "clear");
}

void do_ffzb(ចំនួនគត់ argc, ss_argv_t argv,
	     ចំនួនគត់ sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned ចំនួនគត់ start, end;
	ចំនួនគត់ err;
	errcode_t retval;
	blk64_t out;

	ប្រសិនបើ (check_fs_open(argv[0]))
		ត្រឡប់;

	ប្រសិនបើ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		ត្រឡប់;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ប្រសិនបើ (err)
		ត្រឡប់;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ប្រសិនបើ (err)
		ត្រឡប់;

	retval = ext2fs_find_first_zero_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	ប្រសិនបើ (retval) {
		printf("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_message(retval));
		ត្រឡប់;
	}
	printf("First unmarked block is %llu\n", (unsigned long long) out);
}

void do_ffsb(ចំនួនគត់ argc, ss_argv_t argv,
	     ចំនួនគត់ sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned ចំនួនគត់ start, end;
	ចំនួនគត់ err;
	errcode_t retval;
	blk64_t out;

	ប្រសិនបើ (check_fs_open(argv[0]))
		ត្រឡប់;

	ប្រសិនបើ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		ត្រឡប់;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ប្រសិនបើ (err)
		ត្រឡប់;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ប្រសិនបើ (err)
		ត្រឡប់;

	retval = ext2fs_find_first_set_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	ប្រសិនបើ (retval) {
		printf("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_message(retval));
		ត្រឡប់;
	}
	printf("First marked block is %llu\n", (unsigned long long) out);
}


void do_zerob(ចំនួនគត់ argc, ss_argv_t argv,
	      ចំនួនគត់ sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	ប្រសិនបើ (check_fs_open(argv[0]))
		ត្រឡប់;

	printf("Clearing block bitmap.\n");
	ext2fs_clear_block_bitmap(test_fs->block_map);
}

void do_seti(ចំនួនគត់ argc, ss_argv_t argv,
	     ចំនួនគត់ sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned ចំនួនគត់ inode;
	ចំនួនគត់ err;
	ចំនួនគត់ test_result, op_result;

	ប្រសិនបើ (check_fs_open(argv[0]))
		ត្រឡប់;

	ប្រសិនបើ (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		ត្រឡប់;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ប្រសិនបើ (err)
		ត្រឡប់;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Setting inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	ប្រសិនបើ (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

void do_cleari(ចំនួនគត់ argc, ss_argv_t argv,
	       ចំនួនគត់ sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	unsigned ចំនួនគត់ inode;
	ចំនួនគត់ err;
	ចំនួនគត់ test_result, op_result;

	ប្រសិនបើ (check_fs_open(argv[0]))
		ត្រឡប់;

	ប្រសិនបើ (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		ត្រឡប់;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ប្រសិនបើ (err)
		ត្រឡប់;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Clearing inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	ប្រសិនបើ (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

void do_testi(ចំនួនគត់ argc, ss_argv_t argv,
	      ចំនួនគត់ sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	unsigned ចំនួនគត់ inode;
	ចំនួនគត់ err;
	ចំនួនគត់ test_result;

	ប្រសិនបើ (check_fs_open(argv[0]))
		ត្រឡប់;

	ប្រសិនបើ (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		ត្រឡប់;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ប្រសិនបើ (err)
		ត្រឡប់;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	printf("Inode %u is %s\n", inode, test_result ? "set" : "clear");
}

void do_ffzi(ចំនួនគត់ argc, ss_argv_t argv,
	     ចំនួនគត់ sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned ចំនួនគត់ start, end;
	ចំនួនគត់ err;
	errcode_t retval;
	ext2_ino_t out;

	ប្រសិនបើ (check_fs_open(argv[0]))
		ត្រឡប់;

	ប្រសិនបើ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		ត្រឡប់;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ប្រសិនបើ (err)
		ត្រឡប់;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ប្រសិនបើ (err)
		ត្រឡប់;

	retval = ext2fs_find_first_zero_inode_bitmap2(test_fs->inode_map,
						      start, end, &out);
	ប្រសិនបើ (retval) {
		printf("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_message(retval));
		ត្រឡប់;
	}
	printf("First unmarked inode is %u\n", out);
}

void do_ffsi(ចំនួនគត់ argc, ss_argv_t argv,
	     ចំនួនគត់ sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned ចំនួនគត់ start, end;
	ចំនួនគត់ err;
	errcode_t retval;
	ext2_ino_t out;

	ប្រសិនបើ (check_fs_open(argv[0]))
		ត្រឡប់;

	ប្រសិនបើ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		ត្រឡប់;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ប្រសិនបើ (err)
		ត្រឡប់;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ប្រសិនបើ (err)
		ត្រឡប់;

	retval = ext2fs_find_first_set_inode_bitmap2(test_fs->inode_map,
						     start, end, &out);
	ប្រសិនបើ (retval) {
		printf("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_message(retval));
		ត្រឡប់;
	}
	printf("First marked inode is %u\n", out);
}

void do_zeroi(ចំនួនគត់ argc, ss_argv_t argv,
	      ចំនួនគត់ sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	ប្រសិនបើ (check_fs_open(argv[0]))
		ត្រឡប់;

	printf("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

ចំនួនគត់ main(ចំនួនគត់ argc, var_na **argv)
{
	unsigned ចំនួនគត់	blocks = 128;
	unsigned ចំនួនគត់	inodes = 0;
	unsigned ចំនួនគត់	type = EXT2FS_BMAP64_BITARRAY;
	ចំនួនគត់		c, err, code;
	var_na		*request = (var_na *)NULL;
	var_na		*cmd_file = 0;
	ចំនួនគត់		sci_idx;
	ចំនួនគត់		flags = EXT2_FLAG_64BITS;

	add_error_table(&et_ss_error_table);
	add_error_table(&et_ext2_error_table);
	ខណៈពេល ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		switch (c) {
		pak_sa 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ប្រសិនបើ (err)
				exit(1);
			bha_nga;
		pak_sa 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ប្រសិនបើ (err)
				exit(1);
			bha_nga;
		pak_sa 'l':	/* Legacy bitmaps */
			flags = 0;
			bha_nga;
		pak_sa 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			ប្រសិនបើ (err)
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
	ប្រសិនបើ (code) {
		ss_perror(sci_idx, code, "creating invocation");
		exit(1);
	}

	(void) ss_add_request_table (sci_idx, &ss_std_requests, 1, &code);
	ប្រសិនបើ (code) {
		ss_perror(sci_idx, code, "adding standard requests");
		exit (1);
	}

	printf("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, version);

	setup_filesystem(argv[0], blocks, inodes, type, flags);

	ប្រសិនបើ (request) {
		code = ss_execute_line(sci_idx, request);
		ប្រសិនបើ (code) {
			ss_perror(sci_idx, code, request);
			exit_status++;
		}
	} បើមិនដូច្នេះ ប្រសិនបើ (cmd_file) {
		exit_status = source_file(cmd_file, sci_idx);
	} បើមិនដូច្នេះ {
		ss_listen(sci_idx);
	}

	exit(exit_status);
}

