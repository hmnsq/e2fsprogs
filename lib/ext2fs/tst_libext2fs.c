/*
 * tst_libext2fs.c
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#ຖ້າ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ຖ້າ HAVE_ERRNO_H
#include <errno.h>
#endif
#ຖ້າ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ຖ້າ HAVE_SYS_TYPES_H
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

static ຈຳນວນເຕັມ print_blocks_proc(ext2_filsys fs EXT2FS_ATTR((unused)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_block, ຈຳນວນເຕັມ ref_offset,
			     void *private EXT2FS_ATTR((unused)))
{
	printf("%6lld %8llu (%d %llu)\n", (long long) blockcnt,
	       (unsigned long long) *blocknr, ref_offset,
	       (unsigned long long) ref_block);
	ສົ່ງຄືນ 0;
}


void do_block_iterate(ຈຳນວນເຕັມ argc, var_na **argv, ຈຳນວນເຕັມ sci_idx EXT2FS_ATTR((unused)),
		      void *infop EXT2FS_ATTR((unused)))
{
	const var_na	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	ຈຳນວນເຕັມ		err = 0;
	ຈຳນວນເຕັມ		flags = 0;

	ຖ້າ (common_args_process(argc, argv, 2, 3, argv[0], usage, 0))
		ສົ່ງຄືນ;

	ino = string_to_inode(argv[1]);
	ຖ້າ (!ino)
		ສົ່ງຄືນ;

	ຖ້າ (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		ຖ້າ (err)
			ສົ່ງຄືນ;
	}
	flags |= BLOCK_FLAG_READ_ONLY;

	ext2fs_block_iterate3(current_fs, ino, flags, NULL,
			      print_blocks_proc, NULL);
	putc('\n', stdout);
}
