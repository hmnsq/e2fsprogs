/*
 * tst_libext2fs.c
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif
#nếu HAVE_ERRNO_H
#include <errno.h>
#endif
#nếu HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#nếu HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"

#include "ss/ss.h"
#include "debugfs.h"

/*
 * Hook in new commands into debugfs
 * Override debugfs's prompt
 */
const char *debug_prog_name = "tst_libext2fs";
extern ss_request_table libext2fs_cmds;
ss_request_table *extra_cmds = &libext2fs_cmds;

static số_nguyên print_blocks_proc(ext2_filsys fs EXT2FS_ATTR((unused)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_block, số_nguyên ref_offset,
			     void *private EXT2FS_ATTR((unused)))
{
	printf("%6lld %8llu (%d %llu)\n", (long long) blockcnt,
	       (unsigned long long) *blocknr, ref_offset,
	       (unsigned long long) ref_block);
	trả_về 0;
}


void do_block_iterate(số_nguyên argc, char **argv, số_nguyên sci_idx EXT2FS_ATTR((unused)),
		      void *infop EXT2FS_ATTR((unused)))
{
	const char	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	số_nguyên		err = 0;
	số_nguyên		flags = 0;

	nếu (common_args_process(argc, argv, 2, 3, argv[0], usage, 0))
		trả_về;

	ino = string_to_inode(argv[1]);
	nếu (!ino)
		trả_về;

	nếu (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		nếu (err)
			trả_về;
	}
	flags |= BLOCK_FLAG_READ_ONLY;

	ext2fs_block_iterate3(current_fs, ino, flags, NULL,
			      print_blocks_proc, NULL);
	putc('\n', stdout);
}
