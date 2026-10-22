/*
 * tst_libext2fs.c
 */

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#endif
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

#ضمن "ss/ss.h"
#ضمن "debugfs.h"

/*
 * Hook in new commands into debugfs
 * Override debugfs's prompt
 */
ثابت حرف *debug_prog_name = "tst_libext2fs";
خارجي ss_request_table libext2fs_cmds;
ss_request_table *extra_cmds = &libext2fs_cmds;

ساكن صحيح print_blocks_proc(ext2_filsys fs EXT2FS_ATTR((unused)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_block, صحيح ref_offset,
			     فراغ *خاص EXT2FS_ATTR((unused)))
{
	printf("%6lld %8llu (%d %llu)\n", (long long) blockcnt,
	       (unsigned long long) *blocknr, ref_offset,
	       (unsigned long long) ref_block);
	أرجع 0;
}


فراغ do_block_iterate(صحيح argc, حرف **argv, صحيح sci_idx EXT2FS_ATTR((unused)),
		      فراغ *infop EXT2FS_ATTR((unused)))
{
	ثابت حرف	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	صحيح		err = 0;
	صحيح		flags = 0;

	إذا (common_args_process(argc, argv, 2, 3, argv[0], usage, 0))
		أرجع;

	ino = string_to_inode(argv[1]);
	إذا (!ino)
		أرجع;

	إذا (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		إذا (err)
			أرجع;
	}
	flags |= BLOCK_FLAG_READ_ONLY;

	ext2fs_block_iterate3(current_fs, ino, flags, عدم,
			      print_blocks_proc, عدم);
	putc('\n', stdout);
}
