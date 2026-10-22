/*
 * tst_libext2fs.c
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#ប្រសិនបើ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ប្រសិនបើ HAVE_ERRNO_H
#include <errno.h>
#endif
#ប្រសិនបើ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ប្រសិនបើ HAVE_SYS_TYPES_H
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

static ចំនួនគត់ print_blocks_proc(ext2_filsys fs EXT2FS_ATTR((unused)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_block, ចំនួនគត់ ref_offset,
			     void *private EXT2FS_ATTR((unused)))
{
	printf("%6lld %8llu (%d %llu)\n", (long long) blockcnt,
	       (unsigned long long) *blocknr, ref_offset,
	       (unsigned long long) ref_block);
	ត្រឡប់ 0;
}


void do_block_iterate(ចំនួនគត់ argc, var_na **argv, ចំនួនគត់ sci_idx EXT2FS_ATTR((unused)),
		      void *infop EXT2FS_ATTR((unused)))
{
	const var_na	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	ចំនួនគត់		err = 0;
	ចំនួនគត់		flags = 0;

	ប្រសិនបើ (common_args_process(argc, argv, 2, 3, argv[0], usage, 0))
		ត្រឡប់;

	ino = string_to_inode(argv[1]);
	ប្រសិនបើ (!ino)
		ត្រឡប់;

	ប្រសិនបើ (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		ប្រសិនបើ (err)
			ត្រឡប់;
	}
	flags |= BLOCK_FLAG_READ_ONLY;

	ext2fs_block_iterate3(current_fs, ino, flags, NULL,
			      print_blocks_proc, NULL);
	putc('\n', stdout);
}
