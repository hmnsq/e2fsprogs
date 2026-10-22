/*
 * tst_libext2fs.c
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#གལ་སྲིད HAVE_UNISTD_H
#include <unistd.h>
#endif
#གལ་སྲིད HAVE_ERRNO_H
#include <errno.h>
#endif
#གལ་སྲིད HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#གལ་སྲིད HAVE_SYS_TYPES_H
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
const var_na *debug_prog_name = "tst_libext2fs";
extern ss_request_table libext2fs_cmds;
ss_request_table *extra_cmds = &libext2fs_cmds;

static ཧྲིལ print_blocks_proc(ext2_filsys fs EXT2FS_ATTR((unused)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_block, ཧྲིལ ref_offset,
			     void *private EXT2FS_ATTR((unused)))
{
	printf("%6lld %8llu (%d %llu)\n", (long long) blockcnt,
	       (unsigned long long) *blocknr, ref_offset,
	       (unsigned long long) ref_block);
	ལོག 0;
}


void do_block_iterate(ཧྲིལ argc, var_na **argv, ཧྲིལ sci_idx EXT2FS_ATTR((unused)),
		      void *infop EXT2FS_ATTR((unused)))
{
	const var_na	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	ཧྲིལ		err = 0;
	ཧྲིལ		flags = 0;

	གལ་སྲིད (common_args_process(argc, argv, 2, 3, argv[0], usage, 0))
		ལོག;

	ino = string_to_inode(argv[1]);
	གལ་སྲིད (!ino)
		ལོག;

	གལ་སྲིད (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		གལ་སྲིད (err)
			ལོག;
	}
	flags |= BLOCK_FLAG_READ_ONLY;

	ext2fs_block_iterate3(current_fs, ino, flags, NULL,
			      print_blocks_proc, NULL);
	putc('\n', stdout);
}
