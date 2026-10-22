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

static char subsystem_name[] = "tst_bitmaps";
static char version[] = "1.0";

ext2_filsys	test_fs;
цео		exit_status = 0;

static цео source_file(const char *cmd_file, цео sci_idx)
{
	FILE		*f;
	char		buf[256];
	char		*cp;
	цео		retval;
	цео 		noecho;

	ако (strcmp(cmd_file, "-") == 0)
		f = stdin;
	инако {
		f = fopen(cmd_file, "r");
		ако (!f) {
			perror(cmd_file);
			exit(1);
		}
	}
	fflush(stdout);
	fflush(stderr);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
	док (!feof(f)) {
		ако (fgets(buf, sizeof(buf), f) == NULL)
			break;
		ако (buf[0] == '#')
			continue;
		noecho = 0;
		ако (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = strchr(buf, '\n');
		ако (cp)
			*cp = 0;
		cp = strchr(buf, '\r');
		ако (cp)
			*cp = 0;
		ако (!noecho)
			printf("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_line(sci_idx, buf);
		ако (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_status++;
		}
	}
	врати exit_status;
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
#ако defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#инако
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
unsigned long parse_ulong(const char *str, const char *cmd,
			  const char *descr, цео *err)
{
	char		*tmp;
	unsigned long	ret;

	ret = strtoul(str, &tmp, 0);
	ако (*tmp == 0) {
		ако (err)
			*err = 0;
		врати ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	ако (err)
		*err = 1;
	инако
		exit(1);
	врати 0;
}


цео check_fs_open(const char *name)
{
	ако (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		врати 1;
	}
	врати 0;
}

static void setup_filesystem(const char *name,
			     unsigned цео blocks, unsigned цео inodes,
			     unsigned цео type, цео flags)
{
	struct ext2_super_block param;
	errcode_t retval;

	memset(&param, 0, sizeof(param));
	ext2fs_blocks_count_set(&param, blocks);
	param.s_inodes_count = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	ако (retval) {
		com_err(name, retval, "while initializing filesystem");
		врати;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_block_bitmap(test_fs->block_map);
	test_fs->block_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_block_bitmap(test_fs, "block bitmap",
					      &test_fs->block_map);
	ако (retval) {
		com_err(name, retval, "while allocating block bitmap");
		goto errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	ако (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		goto errout;
	}
	врати;

errout:
	ext2fs_close_free(&test_fs);
}

void setup_cmd(цео argc, ss_argv_t argv,
	       цео sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	цео		c, err;
	unsigned цео	blocks = 128;
	unsigned цео	inodes = 0;
	unsigned цео	type = EXT2FS_BMAP64_BITARRAY;
	цео		flags = EXT2_FLAG_64BITS;

	ако (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	док ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		switch (c) {
		case 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ако (err)
				врати;
			break;
		case 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ако (err)
				врати;
			break;
		case 'l':	/* Legacy bitmaps */
			flags = 0;
			break;
		case 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			ако (err)
				врати;
			break;
		default:
			fprintf(stderr, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			врати;
		}
	}
	setup_filesystem(argv[0], blocks, inodes, type, flags);
}

void close_cmd(цео argc, char **argv, цео sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	ако (check_fs_open(argv[0]))
		врати;

	ext2fs_close_free(&test_fs);
}


void dump_bitmap(ext2fs_generic_bitmap bmap, unsigned цео start, unsigned num)
{
	unsigned char	*buf;
	errcode_t	retval;
	цео		i, len = (num - start + 7) / 8;

	buf = malloc(len);
	ако (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		врати;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) start, num, buf);
	ако (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		free(buf);
		врати;
	}
	за (i=0; i < len; i++)
		printf("%02x", buf[i]);
	printf("\n");
	printf("bits set: %u\n", ext2fs_bitcount(buf, len));
	free(buf);
}

void dump_inode_bitmap_cmd(цео argc, ss_argv_t argv,
			   цео sci_idx EXT2FS_ATTR((unused)),
			   void *infop EXT2FS_ATTR((unused)))
{
	ако (check_fs_open(argv[0]))
		врати;

	printf("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_count);
}
	
void dump_block_bitmap_cmd(цео argc, ss_argv_t argv,
			   цео sci_idx EXT2FS_ATTR((unused)),
			   void *infop EXT2FS_ATTR((unused)))
{
	ако (check_fs_open(argv[0]))
		врати;

	printf("block bitmap: ");
	dump_bitmap(test_fs->block_map, test_fs->super->s_first_data_block,
		    test_fs->super->s_blocks_count);
}
	
void do_setb(цео argc, ss_argv_t argv,
	     цео sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned цео block, num;
	цео err;
	цео test_result, op_result;

	ако (check_fs_open(argv[0]))
		врати;

	ако (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		врати;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ако (err)
		врати;

	ако (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		ако (err)
			врати;

		ext2fs_mark_block_bitmap_range2(test_fs->block_map,
						block, num);
		printf("Marking blocks %u to %u\n", block, block + num - 1);
		врати;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_mark_block_bitmap2(test_fs->block_map, block);
	printf("Setting block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	ако (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

void do_clearb(цео argc, ss_argv_t argv,
	       цео sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	unsigned цео block, num;
	цео err;
	цео test_result, op_result;

	ако (check_fs_open(argv[0]))
		врати;

	ако (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		врати;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ако (err)
		врати;

	ако (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		ако (err)
			врати;

		ext2fs_unmark_block_bitmap_range2(test_fs->block_map,
						block, num);
		printf("Clearing blocks %u to %u\n", block, block + num - 1);
		врати;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_unmark_block_bitmap2(test_fs->block_map, block);
	printf("Clearing block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	ако (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

void do_testb(цео argc, ss_argv_t argv,
	      цео sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	unsigned цео block, num;
	цео err;
	цео test_result;

	ако (check_fs_open(argv[0]))
		врати;

	ако (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		врати;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	ако (err)
		врати;

	ако (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		ако (err)
			врати;

		test_result =
			ext2fs_test_block_bitmap_range2(test_fs->block_map,
							block, num);
		printf("Blocks %u to %u are %sall clear.\n",
		       block, block + num - 1, test_result ? "" : "NOT ");
		врати;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	printf("Block %u is %s\n", block, test_result ? "set" : "clear");
}

void do_ffzb(цео argc, ss_argv_t argv,
	     цео sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned цео start, end;
	цео err;
	errcode_t retval;
	blk64_t out;

	ако (check_fs_open(argv[0]))
		врати;

	ако (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		врати;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ако (err)
		врати;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ако (err)
		врати;

	retval = ext2fs_find_first_zero_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	ако (retval) {
		printf("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_message(retval));
		врати;
	}
	printf("First unmarked block is %llu\n", (unsigned long long) out);
}

void do_ffsb(цео argc, ss_argv_t argv,
	     цео sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned цео start, end;
	цео err;
	errcode_t retval;
	blk64_t out;

	ако (check_fs_open(argv[0]))
		врати;

	ако (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		врати;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ако (err)
		врати;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ако (err)
		врати;

	retval = ext2fs_find_first_set_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	ако (retval) {
		printf("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_message(retval));
		врати;
	}
	printf("First marked block is %llu\n", (unsigned long long) out);
}


void do_zerob(цео argc, ss_argv_t argv,
	      цео sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	ако (check_fs_open(argv[0]))
		врати;

	printf("Clearing block bitmap.\n");
	ext2fs_clear_block_bitmap(test_fs->block_map);
}

void do_seti(цео argc, ss_argv_t argv,
	     цео sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned цео inode;
	цео err;
	цео test_result, op_result;

	ако (check_fs_open(argv[0]))
		врати;

	ако (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		врати;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ако (err)
		врати;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Setting inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	ако (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

void do_cleari(цео argc, ss_argv_t argv,
	       цео sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	unsigned цео inode;
	цео err;
	цео test_result, op_result;

	ако (check_fs_open(argv[0]))
		врати;

	ако (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		врати;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ако (err)
		врати;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Clearing inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	ако (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

void do_testi(цео argc, ss_argv_t argv,
	      цео sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	unsigned цео inode;
	цео err;
	цео test_result;

	ако (check_fs_open(argv[0]))
		врати;

	ако (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		врати;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	ако (err)
		врати;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	printf("Inode %u is %s\n", inode, test_result ? "set" : "clear");
}

void do_ffzi(цео argc, ss_argv_t argv,
	     цео sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned цео start, end;
	цео err;
	errcode_t retval;
	ext2_ino_t out;

	ако (check_fs_open(argv[0]))
		врати;

	ако (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		врати;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ако (err)
		врати;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ако (err)
		врати;

	retval = ext2fs_find_first_zero_inode_bitmap2(test_fs->inode_map,
						      start, end, &out);
	ако (retval) {
		printf("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_message(retval));
		врати;
	}
	printf("First unmarked inode is %u\n", out);
}

void do_ffsi(цео argc, ss_argv_t argv,
	     цео sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned цео start, end;
	цео err;
	errcode_t retval;
	ext2_ino_t out;

	ако (check_fs_open(argv[0]))
		врати;

	ако (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		врати;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	ако (err)
		врати;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	ако (err)
		врати;

	retval = ext2fs_find_first_set_inode_bitmap2(test_fs->inode_map,
						     start, end, &out);
	ако (retval) {
		printf("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_message(retval));
		врати;
	}
	printf("First marked inode is %u\n", out);
}

void do_zeroi(цео argc, ss_argv_t argv,
	      цео sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	ако (check_fs_open(argv[0]))
		врати;

	printf("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

цео main(цео argc, char **argv)
{
	unsigned цео	blocks = 128;
	unsigned цео	inodes = 0;
	unsigned цео	type = EXT2FS_BMAP64_BITARRAY;
	цео		c, err, code;
	char		*request = (char *)NULL;
	char		*cmd_file = 0;
	цео		sci_idx;
	цео		flags = EXT2_FLAG_64BITS;

	add_error_table(&et_ss_error_table);
	add_error_table(&et_ext2_error_table);
	док ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		switch (c) {
		case 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ако (err)
				exit(1);
			break;
		case 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			ако (err)
				exit(1);
			break;
		case 'l':	/* Legacy bitmaps */
			flags = 0;
			break;
		case 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			ако (err)
				exit(1);
			break;
		case 'R':
			request = optarg;
			break;
		case 'f':
			cmd_file = optarg;
			break;
		default:
			com_err(argv[0], 0, "Usage: %s [-R request] "
				"[-f cmd_file]", subsystem_name);
			exit(1);
		}
	}

	sci_idx = ss_create_invocation(subsystem_name, version,
				       (char *)NULL, &tst_bitmaps_cmds, &code);
	ако (code) {
		ss_perror(sci_idx, code, "creating invocation");
		exit(1);
	}

	(void) ss_add_request_table (sci_idx, &ss_std_requests, 1, &code);
	ако (code) {
		ss_perror(sci_idx, code, "adding standard requests");
		exit (1);
	}

	printf("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, version);

	setup_filesystem(argv[0], blocks, inodes, type, flags);

	ако (request) {
		code = ss_execute_line(sci_idx, request);
		ако (code) {
			ss_perror(sci_idx, code, request);
			exit_status++;
		}
	} инако ако (cmd_file) {
		exit_status = source_file(cmd_file, sci_idx);
	} инако {
		ss_listen(sci_idx);
	}

	exit(exit_status);
}

