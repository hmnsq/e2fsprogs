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
整数		exit_status = 0;

static 整数 source_file(const char *cmd_file, 整数 sci_idx)
{
	FILE		*f;
	char		buf[256];
	char		*cp;
	整数		retval;
	整数 		noecho;

	もし (strcmp(cmd_file, "-") == 0)
		f = stdin;
	否则 {
		f = fopen(cmd_file, "r");
		もし (!f) {
			perror(cmd_file);
			exit(1);
		}
	}
	fflush(stdout);
	fflush(stderr);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
	ながら (!feof(f)) {
		もし (fgets(buf, sizeof(buf), f) == NULL)
			break;
		もし (buf[0] == '#')
			continue;
		noecho = 0;
		もし (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = strchr(buf, '\n');
		もし (cp)
			*cp = 0;
		cp = strchr(buf, '\r');
		もし (cp)
			*cp = 0;
		もし (!noecho)
			printf("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_line(sci_idx, buf);
		もし (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_status++;
		}
	}
	戻る exit_status;
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
#もし defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#否则
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
			  const char *descr, 整数 *err)
{
	char		*tmp;
	unsigned long	ret;

	ret = strtoul(str, &tmp, 0);
	もし (*tmp == 0) {
		もし (err)
			*err = 0;
		戻る ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	もし (err)
		*err = 1;
	否则
		exit(1);
	戻る 0;
}


整数 check_fs_open(const char *name)
{
	もし (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		戻る 1;
	}
	戻る 0;
}

static void setup_filesystem(const char *name,
			     unsigned 整数 blocks, unsigned 整数 inodes,
			     unsigned 整数 type, 整数 flags)
{
	struct ext2_super_block param;
	errcode_t retval;

	memset(&param, 0, sizeof(param));
	ext2fs_blocks_count_set(&param, blocks);
	param.s_inodes_count = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	もし (retval) {
		com_err(name, retval, "while initializing filesystem");
		戻る;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_block_bitmap(test_fs->block_map);
	test_fs->block_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_block_bitmap(test_fs, "block bitmap",
					      &test_fs->block_map);
	もし (retval) {
		com_err(name, retval, "while allocating block bitmap");
		goto errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	もし (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		goto errout;
	}
	戻る;

errout:
	ext2fs_close_free(&test_fs);
}

void setup_cmd(整数 argc, ss_argv_t argv,
	       整数 sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	整数		c, err;
	unsigned 整数	blocks = 128;
	unsigned 整数	inodes = 0;
	unsigned 整数	type = EXT2FS_BMAP64_BITARRAY;
	整数		flags = EXT2_FLAG_64BITS;

	もし (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	ながら ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		switch (c) {
		case 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			もし (err)
				戻る;
			break;
		case 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			もし (err)
				戻る;
			break;
		case 'l':	/* Legacy bitmaps */
			flags = 0;
			break;
		case 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			もし (err)
				戻る;
			break;
		default:
			fprintf(stderr, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			戻る;
		}
	}
	setup_filesystem(argv[0], blocks, inodes, type, flags);
}

void close_cmd(整数 argc, char **argv, 整数 sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	もし (check_fs_open(argv[0]))
		戻る;

	ext2fs_close_free(&test_fs);
}


void dump_bitmap(ext2fs_generic_bitmap bmap, unsigned 整数 start, unsigned num)
{
	unsigned char	*buf;
	errcode_t	retval;
	整数		i, len = (num - start + 7) / 8;

	buf = malloc(len);
	もし (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		戻る;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) start, num, buf);
	もし (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		free(buf);
		戻る;
	}
	ために (i=0; i < len; i++)
		printf("%02x", buf[i]);
	printf("\n");
	printf("bits set: %u\n", ext2fs_bitcount(buf, len));
	free(buf);
}

void dump_inode_bitmap_cmd(整数 argc, ss_argv_t argv,
			   整数 sci_idx EXT2FS_ATTR((unused)),
			   void *infop EXT2FS_ATTR((unused)))
{
	もし (check_fs_open(argv[0]))
		戻る;

	printf("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_count);
}
	
void dump_block_bitmap_cmd(整数 argc, ss_argv_t argv,
			   整数 sci_idx EXT2FS_ATTR((unused)),
			   void *infop EXT2FS_ATTR((unused)))
{
	もし (check_fs_open(argv[0]))
		戻る;

	printf("block bitmap: ");
	dump_bitmap(test_fs->block_map, test_fs->super->s_first_data_block,
		    test_fs->super->s_blocks_count);
}
	
void do_setb(整数 argc, ss_argv_t argv,
	     整数 sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned 整数 block, num;
	整数 err;
	整数 test_result, op_result;

	もし (check_fs_open(argv[0]))
		戻る;

	もし (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		戻る;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	もし (err)
		戻る;

	もし (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		もし (err)
			戻る;

		ext2fs_mark_block_bitmap_range2(test_fs->block_map,
						block, num);
		printf("Marking blocks %u to %u\n", block, block + num - 1);
		戻る;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_mark_block_bitmap2(test_fs->block_map, block);
	printf("Setting block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	もし (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

void do_clearb(整数 argc, ss_argv_t argv,
	       整数 sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	unsigned 整数 block, num;
	整数 err;
	整数 test_result, op_result;

	もし (check_fs_open(argv[0]))
		戻る;

	もし (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		戻る;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	もし (err)
		戻る;

	もし (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		もし (err)
			戻る;

		ext2fs_unmark_block_bitmap_range2(test_fs->block_map,
						block, num);
		printf("Clearing blocks %u to %u\n", block, block + num - 1);
		戻る;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_unmark_block_bitmap2(test_fs->block_map, block);
	printf("Clearing block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	もし (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

void do_testb(整数 argc, ss_argv_t argv,
	      整数 sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	unsigned 整数 block, num;
	整数 err;
	整数 test_result;

	もし (check_fs_open(argv[0]))
		戻る;

	もし (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		戻る;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	もし (err)
		戻る;

	もし (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		もし (err)
			戻る;

		test_result =
			ext2fs_test_block_bitmap_range2(test_fs->block_map,
							block, num);
		printf("Blocks %u to %u are %sall clear.\n",
		       block, block + num - 1, test_result ? "" : "NOT ");
		戻る;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	printf("Block %u is %s\n", block, test_result ? "set" : "clear");
}

void do_ffzb(整数 argc, ss_argv_t argv,
	     整数 sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned 整数 start, end;
	整数 err;
	errcode_t retval;
	blk64_t out;

	もし (check_fs_open(argv[0]))
		戻る;

	もし (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		戻る;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	もし (err)
		戻る;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	もし (err)
		戻る;

	retval = ext2fs_find_first_zero_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	もし (retval) {
		printf("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_message(retval));
		戻る;
	}
	printf("First unmarked block is %llu\n", (unsigned long long) out);
}

void do_ffsb(整数 argc, ss_argv_t argv,
	     整数 sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned 整数 start, end;
	整数 err;
	errcode_t retval;
	blk64_t out;

	もし (check_fs_open(argv[0]))
		戻る;

	もし (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		戻る;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	もし (err)
		戻る;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	もし (err)
		戻る;

	retval = ext2fs_find_first_set_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	もし (retval) {
		printf("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_message(retval));
		戻る;
	}
	printf("First marked block is %llu\n", (unsigned long long) out);
}


void do_zerob(整数 argc, ss_argv_t argv,
	      整数 sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	もし (check_fs_open(argv[0]))
		戻る;

	printf("Clearing block bitmap.\n");
	ext2fs_clear_block_bitmap(test_fs->block_map);
}

void do_seti(整数 argc, ss_argv_t argv,
	     整数 sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned 整数 inode;
	整数 err;
	整数 test_result, op_result;

	もし (check_fs_open(argv[0]))
		戻る;

	もし (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		戻る;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	もし (err)
		戻る;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Setting inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	もし (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

void do_cleari(整数 argc, ss_argv_t argv,
	       整数 sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	unsigned 整数 inode;
	整数 err;
	整数 test_result, op_result;

	もし (check_fs_open(argv[0]))
		戻る;

	もし (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		戻る;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	もし (err)
		戻る;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Clearing inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	もし (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

void do_testi(整数 argc, ss_argv_t argv,
	      整数 sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	unsigned 整数 inode;
	整数 err;
	整数 test_result;

	もし (check_fs_open(argv[0]))
		戻る;

	もし (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		戻る;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	もし (err)
		戻る;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	printf("Inode %u is %s\n", inode, test_result ? "set" : "clear");
}

void do_ffzi(整数 argc, ss_argv_t argv,
	     整数 sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned 整数 start, end;
	整数 err;
	errcode_t retval;
	ext2_ino_t out;

	もし (check_fs_open(argv[0]))
		戻る;

	もし (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		戻る;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	もし (err)
		戻る;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	もし (err)
		戻る;

	retval = ext2fs_find_first_zero_inode_bitmap2(test_fs->inode_map,
						      start, end, &out);
	もし (retval) {
		printf("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_message(retval));
		戻る;
	}
	printf("First unmarked inode is %u\n", out);
}

void do_ffsi(整数 argc, ss_argv_t argv,
	     整数 sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned 整数 start, end;
	整数 err;
	errcode_t retval;
	ext2_ino_t out;

	もし (check_fs_open(argv[0]))
		戻る;

	もし (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		戻る;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	もし (err)
		戻る;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	もし (err)
		戻る;

	retval = ext2fs_find_first_set_inode_bitmap2(test_fs->inode_map,
						     start, end, &out);
	もし (retval) {
		printf("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_message(retval));
		戻る;
	}
	printf("First marked inode is %u\n", out);
}

void do_zeroi(整数 argc, ss_argv_t argv,
	      整数 sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	もし (check_fs_open(argv[0]))
		戻る;

	printf("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

整数 メインページ(整数 argc, char **argv)
{
	unsigned 整数	blocks = 128;
	unsigned 整数	inodes = 0;
	unsigned 整数	type = EXT2FS_BMAP64_BITARRAY;
	整数		c, err, code;
	char		*request = (char *)NULL;
	char		*cmd_file = 0;
	整数		sci_idx;
	整数		flags = EXT2_FLAG_64BITS;

	add_error_table(&et_ss_error_table);
	add_error_table(&et_ext2_error_table);
	ながら ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		switch (c) {
		case 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			もし (err)
				exit(1);
			break;
		case 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			もし (err)
				exit(1);
			break;
		case 'l':	/* Legacy bitmaps */
			flags = 0;
			break;
		case 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			もし (err)
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
	もし (code) {
		ss_perror(sci_idx, code, "creating invocation");
		exit(1);
	}

	(void) ss_add_request_table (sci_idx, &ss_std_requests, 1, &code);
	もし (code) {
		ss_perror(sci_idx, code, "adding standard requests");
		exit (1);
	}

	printf("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, version);

	setup_filesystem(argv[0], blocks, inodes, type, flags);

	もし (request) {
		code = ss_execute_line(sci_idx, request);
		もし (code) {
			ss_perror(sci_idx, code, request);
			exit_status++;
		}
	} 否则 もし (cmd_file) {
		exit_status = source_file(cmd_file, sci_idx);
	} 否则 {
		ss_listen(sci_idx);
	}

	exit(exit_status);
}

