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
ຈຳນວນເຕັມ		exit_status = 0;

static ຈຳນວນເຕັມ source_file(const var_na *cmd_file, ຈຳນວນເຕັມ sci_idx)
{
	FILE		*f;
	var_na		buf[256];
	var_na		*cp;
	ຈຳນວນເຕັມ		retval;
	ຈຳນວນເຕັມ 		noecho;

	ຖ້າ (strcmp(cmd_file, "-") == 0)
		f = stdin;
	ບໍ່ດັ່ງນັ້ນ {
		f = fopen(cmd_file, "r");
		ຖ້າ (!f) {
			perror(cmd_file);
			exit(1);
		}
	}
	fflush(stdout);
	fflush(stderr);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
	ໃນຂະນະທີ່ (!feof(f)) {
		ຖ້າ (fgets(buf, parimaa_na(buf), f) == NULL)
			bha_nga;
		ຖ້າ (buf[0] == '#')
			continue;
		noecho = 0;
		ຖ້າ (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = strchr(buf, '\n');
		ຖ້າ (cp)
			*cp = 0;
		cp = strchr(buf, '\r');
		ຖ້າ (cp)
			*cp = 0;
		ຖ້າ (!noecho)
			printf("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_line(sci_idx, buf);
		ຖ້າ (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_status++;
		}
	}
	ສົ່ງຄືນ exit_status;
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
#ຖ້າ defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#ບໍ່ດັ່ງນັ້ນ
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
			  const var_na *descr, ຈຳນວນເຕັມ *err)
{
	var_na		*tmp;
	unsigned long	ret;

	ret = strtoul(str, &tmp, 0);
	ຖ້າ (*tmp == 0) {
		ຖ້າ (err)
			*err = 0;
		ສົ່ງຄືນ ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	ຖ້າ (err)
		*err = 1;
	ບໍ່ດັ່ງນັ້ນ
		exit(1);
	ສົ່ງຄືນ 0;
}


ຈຳນວນເຕັມ check_fs_open(const var_na *name)
{
	ຖ້າ (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		ສົ່ງຄືນ 1;
	}
	ສົ່ງຄືນ 0;
}

static void setup_filesystem(const var_na *name,
			     unsigned ຈຳນວນເຕັມ blocks, unsigned ຈຳນວນເຕັມ inodes,
			     unsigned ຈຳນວນເຕັມ type, ຈຳນວນເຕັມ flags)
{
	sa_nracanaa ext2_super_block param;
	errcode_t retval;

	memset(&param, 0, parimaa_na(param));
	ext2fs_blocks_count_set(&param, blocks);
	param.s_inodes_count = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	ຖ້າ (retval) {
		com_err(name, retval, "while initializing filesystem");
		ສົ່ງຄືນ;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_block_bitmap(test_fs->block_map);
	test_fs->block_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_block_bitmap(test_fs, "block bitmap",
					      &test_fs->block_map);
	ຖ້າ (retval) {
		com_err(name, retval, "while allocating block bitmap");
		goto errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	ຖ້າ (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		goto errout;
	}
	ສົ່ງຄືນ;

errout:
	ext2fs_close_free(&test_fs);
}

void setup_cmd(ຈຳນວນເຕັມ argc, ss_argv_t argv,
	       ຈຳນວນເຕັມ sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	ຈຳນວນເຕັມ		c, err;
	unsigned ຈຳນວນເຕັມ	blocks = 128;
	unsigned ຈຳນວນເຕັມ	inodes = 0;
	unsigned ຈຳນວນເຕັມ	type = EXT2FS_BMAP64_BITARRAY;
	ຈຳນວນເຕັມ		flags = EXT2_FLAG_64BITS;

	ຖ້າ (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	ໃນຂະນະທີ່ ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		switch (c) {
		pak_sa 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ຖ້າ (err)
				ສົ່ງຄືນ;
			bha_nga;
		pak_sa 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ຖ້າ (err)
				ສົ່ງຄືນ;
			bha_nga;
		pak_sa 'l':	/* Legacy bitmaps */
			flags = 0;
			bha_nga;
		pak_sa 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			ຖ້າ (err)
				ສົ່ງຄືນ;
			bha_nga;
		default:
			fprintf(stderr, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			ສົ່ງຄືນ;
		}
	}
	setup_filesystem(argv[0], blocks, inodes, type, flags);
}

void close_cmd(ຈຳນວນເຕັມ argc, var_na **argv, ຈຳນວນເຕັມ sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	ຖ້າ (check_fs_open(argv[0]))
		ສົ່ງຄືນ;

	ext2fs_close_free(&test_fs);
}


void dump_bitmap(ext2fs_generic_bitmap bmap, unsigned ຈຳນວນເຕັມ start, unsigned num)
{
	unsigned var_na	*buf;
	errcode_t	retval;
	ຈຳນວນເຕັມ		i, len = (num - start + 7) / 8;

	buf = malloc(len);
	ຖ້າ (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		ສົ່ງຄືນ;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) start, num, buf);
	ຖ້າ (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		free(buf);
		ສົ່ງຄືນ;
	}
	ສຳລັບ (i=0; i < len; i++)
		printf("%02x", buf[i]);
	printf("\n");
	printf("bits set: %u\n", ext2fs_bitcount(buf, len));
	free(buf);
}

void dump_inode_bitmap_cmd(ຈຳນວນເຕັມ argc, ss_argv_t argv,
			   ຈຳນວນເຕັມ sci_idx EXT2FS_ATTR((unused)),
			   void *infop EXT2FS_ATTR((unused)))
{
	ຖ້າ (check_fs_open(argv[0]))
		ສົ່ງຄືນ;

	printf("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_count);
}
	
void dump_block_bitmap_cmd(ຈຳນວນເຕັມ argc, ss_argv_t argv,
			   ຈຳນວນເຕັມ sci_idx EXT2FS_ATTR((unused)),
			   void *infop EXT2FS_ATTR((unused)))
{
	ຖ້າ (check_fs_open(argv[0]))
		ສົ່ງຄືນ;

	printf("block bitmap: ");
	dump_bitmap(test_fs->block_map, test_fs->super->s_first_data_block,
		    test_fs->super->s_blocks_count);
}
	
void do_setb(ຈຳນວນເຕັມ argc, ss_argv_t argv,
	     ຈຳນວນເຕັມ sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned ຈຳນວນເຕັມ block, num;
	ຈຳນວນເຕັມ err;
	ຈຳນວນເຕັມ test_result, op_result;

	ຖ້າ (check_fs_open(argv[0]))
		ສົ່ງຄືນ;

	ຖ້າ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		ສົ່ງຄືນ;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ຖ້າ (err)
		ສົ່ງຄືນ;

	ຖ້າ (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		ຖ້າ (err)
			ສົ່ງຄືນ;

		ext2fs_mark_block_bitmap_range2(test_fs->block_map,
						block, num);
		printf("Marking blocks %u to %u\n", block, block + num - 1);
		ສົ່ງຄືນ;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_mark_block_bitmap2(test_fs->block_map, block);
	printf("Setting block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	ຖ້າ (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

void do_clearb(ຈຳນວນເຕັມ argc, ss_argv_t argv,
	       ຈຳນວນເຕັມ sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	unsigned ຈຳນວນເຕັມ block, num;
	ຈຳນວນເຕັມ err;
	ຈຳນວນເຕັມ test_result, op_result;

	ຖ້າ (check_fs_open(argv[0]))
		ສົ່ງຄືນ;

	ຖ້າ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		ສົ່ງຄືນ;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ຖ້າ (err)
		ສົ່ງຄືນ;

	ຖ້າ (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		ຖ້າ (err)
			ສົ່ງຄືນ;

		ext2fs_unmark_block_bitmap_range2(test_fs->block_map,
						block, num);
		printf("Clearing blocks %u to %u\n", block, block + num - 1);
		ສົ່ງຄືນ;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_unmark_block_bitmap2(test_fs->block_map, block);
	printf("Clearing block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	ຖ້າ (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

void do_testb(ຈຳນວນເຕັມ argc, ss_argv_t argv,
	      ຈຳນວນເຕັມ sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	unsigned ຈຳນວນເຕັມ block, num;
	ຈຳນວນເຕັມ err;
	ຈຳນວນເຕັມ test_result;

	ຖ້າ (check_fs_open(argv[0]))
		ສົ່ງຄືນ;

	ຖ້າ (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		ສົ່ງຄືນ;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ຖ້າ (err)
		ສົ່ງຄືນ;

	ຖ້າ (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		ຖ້າ (err)
			ສົ່ງຄືນ;

		test_result =
			ext2fs_test_block_bitmap_range2(test_fs->block_map,
							block, num);
		printf("Blocks %u to %u are %sall clear.\n",
		       block, block + num - 1, test_result ? "" : "NOT ");
		ສົ່ງຄືນ;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	printf("Block %u is %s\n", block, test_result ? "set" : "clear");
}

void do_ffzb(ຈຳນວນເຕັມ argc, ss_argv_t argv,
	     ຈຳນວນເຕັມ sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned ຈຳນວນເຕັມ start, end;
	ຈຳນວນເຕັມ err;
	errcode_t retval;
	blk64_t out;

	ຖ້າ (check_fs_open(argv[0]))
		ສົ່ງຄືນ;

	ຖ້າ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		ສົ່ງຄືນ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ຖ້າ (err)
		ສົ່ງຄືນ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ຖ້າ (err)
		ສົ່ງຄືນ;

	retval = ext2fs_find_first_zero_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	ຖ້າ (retval) {
		printf("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_message(retval));
		ສົ່ງຄືນ;
	}
	printf("First unmarked block is %llu\n", (unsigned long long) out);
}

void do_ffsb(ຈຳນວນເຕັມ argc, ss_argv_t argv,
	     ຈຳນວນເຕັມ sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned ຈຳນວນເຕັມ start, end;
	ຈຳນວນເຕັມ err;
	errcode_t retval;
	blk64_t out;

	ຖ້າ (check_fs_open(argv[0]))
		ສົ່ງຄືນ;

	ຖ້າ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		ສົ່ງຄືນ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ຖ້າ (err)
		ສົ່ງຄືນ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ຖ້າ (err)
		ສົ່ງຄືນ;

	retval = ext2fs_find_first_set_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	ຖ້າ (retval) {
		printf("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_message(retval));
		ສົ່ງຄືນ;
	}
	printf("First marked block is %llu\n", (unsigned long long) out);
}


void do_zerob(ຈຳນວນເຕັມ argc, ss_argv_t argv,
	      ຈຳນວນເຕັມ sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	ຖ້າ (check_fs_open(argv[0]))
		ສົ່ງຄືນ;

	printf("Clearing block bitmap.\n");
	ext2fs_clear_block_bitmap(test_fs->block_map);
}

void do_seti(ຈຳນວນເຕັມ argc, ss_argv_t argv,
	     ຈຳນວນເຕັມ sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned ຈຳນວນເຕັມ inode;
	ຈຳນວນເຕັມ err;
	ຈຳນວນເຕັມ test_result, op_result;

	ຖ້າ (check_fs_open(argv[0]))
		ສົ່ງຄືນ;

	ຖ້າ (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		ສົ່ງຄືນ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ຖ້າ (err)
		ສົ່ງຄືນ;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Setting inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	ຖ້າ (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

void do_cleari(ຈຳນວນເຕັມ argc, ss_argv_t argv,
	       ຈຳນວນເຕັມ sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	unsigned ຈຳນວນເຕັມ inode;
	ຈຳນວນເຕັມ err;
	ຈຳນວນເຕັມ test_result, op_result;

	ຖ້າ (check_fs_open(argv[0]))
		ສົ່ງຄືນ;

	ຖ້າ (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		ສົ່ງຄືນ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ຖ້າ (err)
		ສົ່ງຄືນ;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Clearing inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	ຖ້າ (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

void do_testi(ຈຳນວນເຕັມ argc, ss_argv_t argv,
	      ຈຳນວນເຕັມ sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	unsigned ຈຳນວນເຕັມ inode;
	ຈຳນວນເຕັມ err;
	ຈຳນວນເຕັມ test_result;

	ຖ້າ (check_fs_open(argv[0]))
		ສົ່ງຄືນ;

	ຖ້າ (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		ສົ່ງຄືນ;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ຖ້າ (err)
		ສົ່ງຄືນ;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	printf("Inode %u is %s\n", inode, test_result ? "set" : "clear");
}

void do_ffzi(ຈຳນວນເຕັມ argc, ss_argv_t argv,
	     ຈຳນວນເຕັມ sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned ຈຳນວນເຕັມ start, end;
	ຈຳນວນເຕັມ err;
	errcode_t retval;
	ext2_ino_t out;

	ຖ້າ (check_fs_open(argv[0]))
		ສົ່ງຄືນ;

	ຖ້າ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		ສົ່ງຄືນ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ຖ້າ (err)
		ສົ່ງຄືນ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ຖ້າ (err)
		ສົ່ງຄືນ;

	retval = ext2fs_find_first_zero_inode_bitmap2(test_fs->inode_map,
						      start, end, &out);
	ຖ້າ (retval) {
		printf("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_message(retval));
		ສົ່ງຄືນ;
	}
	printf("First unmarked inode is %u\n", out);
}

void do_ffsi(ຈຳນວນເຕັມ argc, ss_argv_t argv,
	     ຈຳນວນເຕັມ sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned ຈຳນວນເຕັມ start, end;
	ຈຳນວນເຕັມ err;
	errcode_t retval;
	ext2_ino_t out;

	ຖ້າ (check_fs_open(argv[0]))
		ສົ່ງຄືນ;

	ຖ້າ (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		ສົ່ງຄືນ;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ຖ້າ (err)
		ສົ່ງຄືນ;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ຖ້າ (err)
		ສົ່ງຄືນ;

	retval = ext2fs_find_first_set_inode_bitmap2(test_fs->inode_map,
						     start, end, &out);
	ຖ້າ (retval) {
		printf("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_message(retval));
		ສົ່ງຄືນ;
	}
	printf("First marked inode is %u\n", out);
}

void do_zeroi(ຈຳນວນເຕັມ argc, ss_argv_t argv,
	      ຈຳນວນເຕັມ sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	ຖ້າ (check_fs_open(argv[0]))
		ສົ່ງຄືນ;

	printf("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

ຈຳນວນເຕັມ main(ຈຳນວນເຕັມ argc, var_na **argv)
{
	unsigned ຈຳນວນເຕັມ	blocks = 128;
	unsigned ຈຳນວນເຕັມ	inodes = 0;
	unsigned ຈຳນວນເຕັມ	type = EXT2FS_BMAP64_BITARRAY;
	ຈຳນວນເຕັມ		c, err, code;
	var_na		*request = (var_na *)NULL;
	var_na		*cmd_file = 0;
	ຈຳນວນເຕັມ		sci_idx;
	ຈຳນວນເຕັມ		flags = EXT2_FLAG_64BITS;

	add_error_table(&et_ss_error_table);
	add_error_table(&et_ext2_error_table);
	ໃນຂະນະທີ່ ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		switch (c) {
		pak_sa 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ຖ້າ (err)
				exit(1);
			bha_nga;
		pak_sa 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ຖ້າ (err)
				exit(1);
			bha_nga;
		pak_sa 'l':	/* Legacy bitmaps */
			flags = 0;
			bha_nga;
		pak_sa 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			ຖ້າ (err)
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
	ຖ້າ (code) {
		ss_perror(sci_idx, code, "creating invocation");
		exit(1);
	}

	(void) ss_add_request_table (sci_idx, &ss_std_requests, 1, &code);
	ຖ້າ (code) {
		ss_perror(sci_idx, code, "adding standard requests");
		exit (1);
	}

	printf("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, version);

	setup_filesystem(argv[0], blocks, inodes, type, flags);

	ຖ້າ (request) {
		code = ss_execute_line(sci_idx, request);
		ຖ້າ (code) {
			ss_perror(sci_idx, code, request);
			exit_status++;
		}
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (cmd_file) {
		exit_status = source_file(cmd_file, sci_idx);
	} ບໍ່ດັ່ງນັ້ນ {
		ss_listen(sci_idx);
	}

	exit(exit_status);
}

