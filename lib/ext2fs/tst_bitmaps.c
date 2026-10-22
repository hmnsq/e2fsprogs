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
정수		exit_status = 0;

static 정수 source_file(const char *cmd_file, 정수 sci_idx)
{
	FILE		*f;
	char		buf[256];
	char		*cp;
	정수		retval;
	정수 		noecho;

	만약 (strcmp(cmd_file, "-") == 0)
		f = stdin;
	아니면 {
		f = fopen(cmd_file, "r");
		만약 (!f) {
			perror(cmd_file);
			exit(1);
		}
	}
	fflush(stdout);
	fflush(stderr);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
	동안 (!feof(f)) {
		만약 (fgets(buf, sizeof(buf), f) == NULL)
			break;
		만약 (buf[0] == '#')
			continue;
		noecho = 0;
		만약 (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = strchr(buf, '\n');
		만약 (cp)
			*cp = 0;
		cp = strchr(buf, '\r');
		만약 (cp)
			*cp = 0;
		만약 (!noecho)
			printf("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_line(sci_idx, buf);
		만약 (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_status++;
		}
	}
	반환 exit_status;
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
#만약 defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#아니면
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
			  const char *descr, 정수 *err)
{
	char		*tmp;
	unsigned long	ret;

	ret = strtoul(str, &tmp, 0);
	만약 (*tmp == 0) {
		만약 (err)
			*err = 0;
		반환 ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	만약 (err)
		*err = 1;
	아니면
		exit(1);
	반환 0;
}


정수 check_fs_open(const char *name)
{
	만약 (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		반환 1;
	}
	반환 0;
}

static void setup_filesystem(const char *name,
			     unsigned 정수 blocks, unsigned 정수 inodes,
			     unsigned 정수 type, 정수 flags)
{
	struct ext2_super_block param;
	errcode_t retval;

	memset(&param, 0, sizeof(param));
	ext2fs_blocks_count_set(&param, blocks);
	param.s_inodes_count = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	만약 (retval) {
		com_err(name, retval, "while initializing filesystem");
		반환;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_block_bitmap(test_fs->block_map);
	test_fs->block_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_block_bitmap(test_fs, "block bitmap",
					      &test_fs->block_map);
	만약 (retval) {
		com_err(name, retval, "while allocating block bitmap");
		goto errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	만약 (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		goto errout;
	}
	반환;

errout:
	ext2fs_close_free(&test_fs);
}

void setup_cmd(정수 argc, ss_argv_t argv,
	       정수 sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	정수		c, err;
	unsigned 정수	blocks = 128;
	unsigned 정수	inodes = 0;
	unsigned 정수	type = EXT2FS_BMAP64_BITARRAY;
	정수		flags = EXT2_FLAG_64BITS;

	만약 (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	동안 ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		switch (c) {
		case 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			만약 (err)
				반환;
			break;
		case 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			만약 (err)
				반환;
			break;
		case 'l':	/* Legacy bitmaps */
			flags = 0;
			break;
		case 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			만약 (err)
				반환;
			break;
		default:
			fprintf(stderr, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			반환;
		}
	}
	setup_filesystem(argv[0], blocks, inodes, type, flags);
}

void close_cmd(정수 argc, char **argv, 정수 sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	만약 (check_fs_open(argv[0]))
		반환;

	ext2fs_close_free(&test_fs);
}


void dump_bitmap(ext2fs_generic_bitmap bmap, unsigned 정수 start, unsigned num)
{
	unsigned char	*buf;
	errcode_t	retval;
	정수		i, len = (num - start + 7) / 8;

	buf = malloc(len);
	만약 (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		반환;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) start, num, buf);
	만약 (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		free(buf);
		반환;
	}
	위해 (i=0; i < len; i++)
		printf("%02x", buf[i]);
	printf("\n");
	printf("bits set: %u\n", ext2fs_bitcount(buf, len));
	free(buf);
}

void dump_inode_bitmap_cmd(정수 argc, ss_argv_t argv,
			   정수 sci_idx EXT2FS_ATTR((unused)),
			   void *infop EXT2FS_ATTR((unused)))
{
	만약 (check_fs_open(argv[0]))
		반환;

	printf("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_count);
}
	
void dump_block_bitmap_cmd(정수 argc, ss_argv_t argv,
			   정수 sci_idx EXT2FS_ATTR((unused)),
			   void *infop EXT2FS_ATTR((unused)))
{
	만약 (check_fs_open(argv[0]))
		반환;

	printf("block bitmap: ");
	dump_bitmap(test_fs->block_map, test_fs->super->s_first_data_block,
		    test_fs->super->s_blocks_count);
}
	
void do_setb(정수 argc, ss_argv_t argv,
	     정수 sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned 정수 block, num;
	정수 err;
	정수 test_result, op_result;

	만약 (check_fs_open(argv[0]))
		반환;

	만약 (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		반환;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	만약 (err)
		반환;

	만약 (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		만약 (err)
			반환;

		ext2fs_mark_block_bitmap_range2(test_fs->block_map,
						block, num);
		printf("Marking blocks %u to %u\n", block, block + num - 1);
		반환;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_mark_block_bitmap2(test_fs->block_map, block);
	printf("Setting block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	만약 (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

void do_clearb(정수 argc, ss_argv_t argv,
	       정수 sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	unsigned 정수 block, num;
	정수 err;
	정수 test_result, op_result;

	만약 (check_fs_open(argv[0]))
		반환;

	만약 (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		반환;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	만약 (err)
		반환;

	만약 (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		만약 (err)
			반환;

		ext2fs_unmark_block_bitmap_range2(test_fs->block_map,
						block, num);
		printf("Clearing blocks %u to %u\n", block, block + num - 1);
		반환;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_unmark_block_bitmap2(test_fs->block_map, block);
	printf("Clearing block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	만약 (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

void do_testb(정수 argc, ss_argv_t argv,
	      정수 sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	unsigned 정수 block, num;
	정수 err;
	정수 test_result;

	만약 (check_fs_open(argv[0]))
		반환;

	만약 (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		반환;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	만약 (err)
		반환;

	만약 (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		만약 (err)
			반환;

		test_result =
			ext2fs_test_block_bitmap_range2(test_fs->block_map,
							block, num);
		printf("Blocks %u to %u are %sall clear.\n",
		       block, block + num - 1, test_result ? "" : "NOT ");
		반환;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	printf("Block %u is %s\n", block, test_result ? "set" : "clear");
}

void do_ffzb(정수 argc, ss_argv_t argv,
	     정수 sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned 정수 start, end;
	정수 err;
	errcode_t retval;
	blk64_t out;

	만약 (check_fs_open(argv[0]))
		반환;

	만약 (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		반환;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	만약 (err)
		반환;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	만약 (err)
		반환;

	retval = ext2fs_find_first_zero_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	만약 (retval) {
		printf("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_message(retval));
		반환;
	}
	printf("First unmarked block is %llu\n", (unsigned long long) out);
}

void do_ffsb(정수 argc, ss_argv_t argv,
	     정수 sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned 정수 start, end;
	정수 err;
	errcode_t retval;
	blk64_t out;

	만약 (check_fs_open(argv[0]))
		반환;

	만약 (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		반환;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	만약 (err)
		반환;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	만약 (err)
		반환;

	retval = ext2fs_find_first_set_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	만약 (retval) {
		printf("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_message(retval));
		반환;
	}
	printf("First marked block is %llu\n", (unsigned long long) out);
}


void do_zerob(정수 argc, ss_argv_t argv,
	      정수 sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	만약 (check_fs_open(argv[0]))
		반환;

	printf("Clearing block bitmap.\n");
	ext2fs_clear_block_bitmap(test_fs->block_map);
}

void do_seti(정수 argc, ss_argv_t argv,
	     정수 sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned 정수 inode;
	정수 err;
	정수 test_result, op_result;

	만약 (check_fs_open(argv[0]))
		반환;

	만약 (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		반환;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	만약 (err)
		반환;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Setting inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	만약 (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

void do_cleari(정수 argc, ss_argv_t argv,
	       정수 sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	unsigned 정수 inode;
	정수 err;
	정수 test_result, op_result;

	만약 (check_fs_open(argv[0]))
		반환;

	만약 (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		반환;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	만약 (err)
		반환;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Clearing inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	만약 (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

void do_testi(정수 argc, ss_argv_t argv,
	      정수 sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	unsigned 정수 inode;
	정수 err;
	정수 test_result;

	만약 (check_fs_open(argv[0]))
		반환;

	만약 (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		반환;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	만약 (err)
		반환;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	printf("Inode %u is %s\n", inode, test_result ? "set" : "clear");
}

void do_ffzi(정수 argc, ss_argv_t argv,
	     정수 sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned 정수 start, end;
	정수 err;
	errcode_t retval;
	ext2_ino_t out;

	만약 (check_fs_open(argv[0]))
		반환;

	만약 (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		반환;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	만약 (err)
		반환;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	만약 (err)
		반환;

	retval = ext2fs_find_first_zero_inode_bitmap2(test_fs->inode_map,
						      start, end, &out);
	만약 (retval) {
		printf("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_message(retval));
		반환;
	}
	printf("First unmarked inode is %u\n", out);
}

void do_ffsi(정수 argc, ss_argv_t argv,
	     정수 sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned 정수 start, end;
	정수 err;
	errcode_t retval;
	ext2_ino_t out;

	만약 (check_fs_open(argv[0]))
		반환;

	만약 (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		반환;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	만약 (err)
		반환;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	만약 (err)
		반환;

	retval = ext2fs_find_first_set_inode_bitmap2(test_fs->inode_map,
						     start, end, &out);
	만약 (retval) {
		printf("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_message(retval));
		반환;
	}
	printf("First marked inode is %u\n", out);
}

void do_zeroi(정수 argc, ss_argv_t argv,
	      정수 sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	만약 (check_fs_open(argv[0]))
		반환;

	printf("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

정수 main(정수 argc, char **argv)
{
	unsigned 정수	blocks = 128;
	unsigned 정수	inodes = 0;
	unsigned 정수	type = EXT2FS_BMAP64_BITARRAY;
	정수		c, err, code;
	char		*request = (char *)NULL;
	char		*cmd_file = 0;
	정수		sci_idx;
	정수		flags = EXT2_FLAG_64BITS;

	add_error_table(&et_ss_error_table);
	add_error_table(&et_ext2_error_table);
	동안 ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		switch (c) {
		case 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			만약 (err)
				exit(1);
			break;
		case 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			만약 (err)
				exit(1);
			break;
		case 'l':	/* Legacy bitmaps */
			flags = 0;
			break;
		case 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			만약 (err)
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
	만약 (code) {
		ss_perror(sci_idx, code, "creating invocation");
		exit(1);
	}

	(void) ss_add_request_table (sci_idx, &ss_std_requests, 1, &code);
	만약 (code) {
		ss_perror(sci_idx, code, "adding standard requests");
		exit (1);
	}

	printf("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, version);

	setup_filesystem(argv[0], blocks, inodes, type, flags);

	만약 (request) {
		code = ss_execute_line(sci_idx, request);
		만약 (code) {
			ss_perror(sci_idx, code, request);
			exit_status++;
		}
	} 아니면 만약 (cmd_file) {
		exit_status = source_file(cmd_file, sci_idx);
	} 아니면 {
		ss_listen(sci_idx);
	}

	exit(exit_status);
}

