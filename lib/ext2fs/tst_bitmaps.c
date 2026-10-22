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
целое		exit_status = 0;

static целое source_file(const char *cmd_file, целое sci_idx)
{
	FILE		*f;
	char		buf[256];
	char		*cp;
	целое		retval;
	целое 		noecho;

	если (strcmp(cmd_file, "-") == 0)
		f = stdin;
	иначе {
		f = fopen(cmd_file, "r");
		если (!f) {
			perror(cmd_file);
			exit(1);
		}
	}
	fflush(stdout);
	fflush(stderr);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
	пока (!feof(f)) {
		если (fgets(buf, sizeof(buf), f) == NULL)
			break;
		если (buf[0] == '#')
			continue;
		noecho = 0;
		если (buf[0] == '-') {
			noecho = 1;
			buf[0] = ' ';
		}
		cp = strchr(buf, '\n');
		если (cp)
			*cp = 0;
		cp = strchr(buf, '\r');
		если (cp)
			*cp = 0;
		если (!noecho)
			printf("%s: %s\n", subsystem_name, buf);
		retval = ss_execute_line(sci_idx, buf);
		если (retval) {
			ss_perror(sci_idx, retval, buf);
			exit_status++;
		}
	}
	вернуть exit_status;
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
#если defined(__GLIBC__) || defined(__linux__)
	optind = 0;
#иначе
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
			  const char *descr, целое *err)
{
	char		*tmp;
	unsigned long	ret;

	ret = strtoul(str, &tmp, 0);
	если (*tmp == 0) {
		если (err)
			*err = 0;
		вернуть ret;
	}
	com_err(cmd, 0, "Bad %s - %s", descr, str);
	если (err)
		*err = 1;
	иначе
		exit(1);
	вернуть 0;
}


целое check_fs_open(const char *name)
{
	если (!test_fs) {
		com_err(name, 0, "Filesystem not open");
		вернуть 1;
	}
	вернуть 0;
}

static void setup_filesystem(const char *name,
			     unsigned целое blocks, unsigned целое inodes,
			     unsigned целое type, целое flags)
{
	struct ext2_super_block param;
	errcode_t retval;

	memset(&param, 0, sizeof(param));
	ext2fs_blocks_count_set(&param, blocks);
	param.s_inodes_count = inodes;

	retval = ext2fs_initialize("test fs", flags, &param,
				   test_io_manager, &test_fs);

	если (retval) {
		com_err(name, retval, "while initializing filesystem");
		вернуть;
	}
	test_fs->default_bitmap_type = type;
	ext2fs_free_block_bitmap(test_fs->block_map);
	test_fs->block_map = 0;
	ext2fs_free_inode_bitmap(test_fs->inode_map);
	test_fs->inode_map = 0;
	retval = ext2fs_allocate_block_bitmap(test_fs, "block bitmap",
					      &test_fs->block_map);
	если (retval) {
		com_err(name, retval, "while allocating block bitmap");
		goto errout;
	}
	retval = ext2fs_allocate_inode_bitmap(test_fs, "inode bitmap",
					      &test_fs->inode_map);
	если (retval) {
		com_err(name, retval, "while allocating inode bitmap");
		goto errout;
	}
	вернуть;

errout:
	ext2fs_close_free(&test_fs);
}

void setup_cmd(целое argc, ss_argv_t argv,
	       целое sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	целое		c, err;
	unsigned целое	blocks = 128;
	unsigned целое	inodes = 0;
	unsigned целое	type = EXT2FS_BMAP64_BITARRAY;
	целое		flags = EXT2_FLAG_64BITS;

	если (test_fs)
		ext2fs_close_free(&test_fs);

	reset_getopt();
	пока ((c = getopt(argc, argv, "b:i:lt:")) != EOF) {
		switch (c) {
		case 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			если (err)
				вернуть;
			break;
		case 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			если (err)
				вернуть;
			break;
		case 'l':	/* Legacy bitmaps */
			flags = 0;
			break;
		case 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			если (err)
				вернуть;
			break;
		default:
			fprintf(stderr, "%s: usage: setup [-b blocks] "
				"[-i inodes] [-t type]\n", argv[0]);
			вернуть;
		}
	}
	setup_filesystem(argv[0], blocks, inodes, type, flags);
}

void close_cmd(целое argc, char **argv, целое sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	если (check_fs_open(argv[0]))
		вернуть;

	ext2fs_close_free(&test_fs);
}


void dump_bitmap(ext2fs_generic_bitmap bmap, unsigned целое start, unsigned num)
{
	unsigned char	*buf;
	errcode_t	retval;
	целое		i, len = (num - start + 7) / 8;

	buf = malloc(len);
	если (!buf) {
		com_err("dump_bitmap", 0, "couldn't allocate buffer");
		вернуть;
	}
	memset(buf, 0, len);
	retval = ext2fs_get_generic_bmap_range(bmap, (__u64) start, num, buf);
	если (retval) {
		com_err("dump_bitmap", retval, 
			"while calling ext2fs_generic_bmap_range");
		free(buf);
		вернуть;
	}
	для (i=0; i < len; i++)
		printf("%02x", buf[i]);
	printf("\n");
	printf("bits set: %u\n", ext2fs_bitcount(buf, len));
	free(buf);
}

void dump_inode_bitmap_cmd(целое argc, ss_argv_t argv,
			   целое sci_idx EXT2FS_ATTR((unused)),
			   void *infop EXT2FS_ATTR((unused)))
{
	если (check_fs_open(argv[0]))
		вернуть;

	printf("inode bitmap: ");
	dump_bitmap(test_fs->inode_map, 1, test_fs->super->s_inodes_count);
}
	
void dump_block_bitmap_cmd(целое argc, ss_argv_t argv,
			   целое sci_idx EXT2FS_ATTR((unused)),
			   void *infop EXT2FS_ATTR((unused)))
{
	если (check_fs_open(argv[0]))
		вернуть;

	printf("block bitmap: ");
	dump_bitmap(test_fs->block_map, test_fs->super->s_first_data_block,
		    test_fs->super->s_blocks_count);
}
	
void do_setb(целое argc, ss_argv_t argv,
	     целое sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned целое block, num;
	целое err;
	целое test_result, op_result;

	если (check_fs_open(argv[0]))
		вернуть;

	если (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: setb <block> [num]");
		вернуть;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	если (err)
		вернуть;

	если (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		если (err)
			вернуть;

		ext2fs_mark_block_bitmap_range2(test_fs->block_map,
						block, num);
		printf("Marking blocks %u to %u\n", block, block + num - 1);
		вернуть;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_mark_block_bitmap2(test_fs->block_map, block);
	printf("Setting block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	если (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

void do_clearb(целое argc, ss_argv_t argv,
	       целое sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	unsigned целое block, num;
	целое err;
	целое test_result, op_result;

	если (check_fs_open(argv[0]))
		вернуть;

	если (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: clearb <block> [num]");
		вернуть;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	если (err)
		вернуть;

	если (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		если (err)
			вернуть;

		ext2fs_unmark_block_bitmap_range2(test_fs->block_map,
						block, num);
		printf("Clearing blocks %u to %u\n", block, block + num - 1);
		вернуть;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	op_result = ext2fs_unmark_block_bitmap2(test_fs->block_map, block);
	printf("Clearing block %u, was %s before\n", block, op_result ?
	       "set" : "clear");
	если (!test_result != !op_result)
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
}

void do_testb(целое argc, ss_argv_t argv,
	      целое sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	unsigned целое block, num;
	целое err;
	целое test_result;

	если (check_fs_open(argv[0]))
		вернуть;

	если (argc != 2 && argc != 3) {
		com_err(argv[0], 0, "Usage: testb <block> [num]");
		вернуть;
	}

	block = parse_ulong(argv[1], argv[0], "block", &err);
	если (err)
		вернуть;

	если (argc == 3) {
		num = parse_ulong(argv[2], argv[0], "num", &err);
		если (err)
			вернуть;

		test_result =
			ext2fs_test_block_bitmap_range2(test_fs->block_map,
							block, num);
		printf("Blocks %u to %u are %sall clear.\n",
		       block, block + num - 1, test_result ? "" : "NOT ");
		вернуть;
	}

	test_result = ext2fs_test_block_bitmap2(test_fs->block_map, block);
	printf("Block %u is %s\n", block, test_result ? "set" : "clear");
}

void do_ffzb(целое argc, ss_argv_t argv,
	     целое sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned целое start, end;
	целое err;
	errcode_t retval;
	blk64_t out;

	если (check_fs_open(argv[0]))
		вернуть;

	если (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzb <start> <end>");
		вернуть;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	если (err)
		вернуть;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	если (err)
		вернуть;

	retval = ext2fs_find_first_zero_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	если (retval) {
		printf("ext2fs_find_first_zero_block_bitmap2() returned %s\n",
		       error_message(retval));
		вернуть;
	}
	printf("First unmarked block is %llu\n", (unsigned long long) out);
}

void do_ffsb(целое argc, ss_argv_t argv,
	     целое sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned целое start, end;
	целое err;
	errcode_t retval;
	blk64_t out;

	если (check_fs_open(argv[0]))
		вернуть;

	если (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsb <start> <end>");
		вернуть;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	если (err)
		вернуть;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	если (err)
		вернуть;

	retval = ext2fs_find_first_set_block_bitmap2(test_fs->block_map,
						      start, end, &out);
	если (retval) {
		printf("ext2fs_find_first_set_block_bitmap2() returned %s\n",
		       error_message(retval));
		вернуть;
	}
	printf("First marked block is %llu\n", (unsigned long long) out);
}


void do_zerob(целое argc, ss_argv_t argv,
	      целое sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	если (check_fs_open(argv[0]))
		вернуть;

	printf("Clearing block bitmap.\n");
	ext2fs_clear_block_bitmap(test_fs->block_map);
}

void do_seti(целое argc, ss_argv_t argv,
	     целое sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned целое inode;
	целое err;
	целое test_result, op_result;

	если (check_fs_open(argv[0]))
		вернуть;

	если (argc != 2) {
		com_err(argv[0], 0, "Usage: seti <inode>");
		вернуть;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	если (err)
		вернуть;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_mark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Setting inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	если (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

void do_cleari(целое argc, ss_argv_t argv,
	       целое sci_idx EXT2FS_ATTR((unused)),
	       void *infop EXT2FS_ATTR((unused)))
{
	unsigned целое inode;
	целое err;
	целое test_result, op_result;

	если (check_fs_open(argv[0]))
		вернуть;

	если (argc != 2) {
		com_err(argv[0], 0, "Usage: clearb <inode>");
		вернуть;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	если (err)
		вернуть;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	op_result = ext2fs_unmark_inode_bitmap2(test_fs->inode_map, inode);
	printf("Clearing inode %u, was %s before\n", inode, op_result ?
	       "set" : "clear");
	если (!test_result != !op_result) {
		com_err(argv[0], 0, "*ERROR* test_result different! (%d, %d)",
			test_result, op_result);
		exit_status++;
	}
}

void do_testi(целое argc, ss_argv_t argv,
	      целое sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	unsigned целое inode;
	целое err;
	целое test_result;

	если (check_fs_open(argv[0]))
		вернуть;

	если (argc != 2) {
		com_err(argv[0], 0, "Usage: testb <inode>");
		вернуть;
	}

	inode = parse_ulong(argv[1], argv[0], "inode", &err);
	если (err)
		вернуть;

	test_result = ext2fs_test_inode_bitmap2(test_fs->inode_map, inode);
	printf("Inode %u is %s\n", inode, test_result ? "set" : "clear");
}

void do_ffzi(целое argc, ss_argv_t argv,
	     целое sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned целое start, end;
	целое err;
	errcode_t retval;
	ext2_ino_t out;

	если (check_fs_open(argv[0]))
		вернуть;

	если (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffzi <start> <end>");
		вернуть;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	если (err)
		вернуть;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	если (err)
		вернуть;

	retval = ext2fs_find_first_zero_inode_bitmap2(test_fs->inode_map,
						      start, end, &out);
	если (retval) {
		printf("ext2fs_find_first_zero_inode_bitmap2() returned %s\n",
		       error_message(retval));
		вернуть;
	}
	printf("First unmarked inode is %u\n", out);
}

void do_ffsi(целое argc, ss_argv_t argv,
	     целое sci_idx EXT2FS_ATTR((unused)),
	     void *infop EXT2FS_ATTR((unused)))
{
	unsigned целое start, end;
	целое err;
	errcode_t retval;
	ext2_ino_t out;

	если (check_fs_open(argv[0]))
		вернуть;

	если (argc != 3 && argc != 3) {
		com_err(argv[0], 0, "Usage: ffsi <start> <end>");
		вернуть;
	}

	start = parse_ulong(argv[1], argv[0], "start", &err);
	если (err)
		вернуть;

	end = parse_ulong(argv[2], argv[0], "end", &err);
	если (err)
		вернуть;

	retval = ext2fs_find_first_set_inode_bitmap2(test_fs->inode_map,
						     start, end, &out);
	если (retval) {
		printf("ext2fs_find_first_set_inode_bitmap2() returned %s\n",
		       error_message(retval));
		вернуть;
	}
	printf("First marked inode is %u\n", out);
}

void do_zeroi(целое argc, ss_argv_t argv,
	      целое sci_idx EXT2FS_ATTR((unused)),
	      void *infop EXT2FS_ATTR((unused)))
{
	если (check_fs_open(argv[0]))
		вернуть;

	printf("Clearing inode bitmap.\n");
	ext2fs_clear_inode_bitmap(test_fs->inode_map);
}

целое основной(целое argc, char **argv)
{
	unsigned целое	blocks = 128;
	unsigned целое	inodes = 0;
	unsigned целое	type = EXT2FS_BMAP64_BITARRAY;
	целое		c, err, code;
	char		*request = (char *)NULL;
	char		*cmd_file = 0;
	целое		sci_idx;
	целое		flags = EXT2_FLAG_64BITS;

	add_error_table(&et_ss_error_table);
	add_error_table(&et_ext2_error_table);
	пока ((c = getopt (argc, argv, "b:i:lt:R:f:")) != EOF) {
		switch (c) {
		case 'b':
			blocks = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			если (err)
				exit(1);
			break;
		case 'i':
			inodes = parse_ulong(optarg, argv[0],
					     "number of blocks", &err);
			если (err)
				exit(1);
			break;
		case 'l':	/* Legacy bitmaps */
			flags = 0;
			break;
		case 't':
			type = parse_ulong(optarg, argv[0],
					   "bitmap backend type", &err);
			если (err)
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
	если (code) {
		ss_perror(sci_idx, code, "creating invocation");
		exit(1);
	}

	(void) ss_add_request_table (sci_idx, &ss_std_requests, 1, &code);
	если (code) {
		ss_perror(sci_idx, code, "adding standard requests");
		exit (1);
	}

	printf("%s %s.  Type '?' for a list of commands.\n\n",
	       subsystem_name, version);

	setup_filesystem(argv[0], blocks, inodes, type, flags);

	если (request) {
		code = ss_execute_line(sci_idx, request);
		если (code) {
			ss_perror(sci_idx, code, request);
			exit_status++;
		}
	} иначе если (cmd_file) {
		exit_status = source_file(cmd_file, sci_idx);
	} иначе {
		ss_listen(sci_idx);
	}

	exit(exit_status);
}

