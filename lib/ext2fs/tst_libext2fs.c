/*
 * tst_libext2fs.c
 */

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#যদি HAVE_ERRNO_H
#অন্তর্ভাৱ <errno.h>
#endif
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাৱ <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাৱ <sys/types.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fsP.h"

#অন্তর্ভাৱ "ss/ss.h"
#অন্তর্ভাৱ "debugfs.h"

/*
 * Hook in new commands into debugfs
 * Override debugfs's prompt
 */
ধ্রুৱ ৱর্ণ *debug_prog_name = "tst_libext2fs";
বাহ্য ss_request_table libext2fs_cmds;
ss_request_table *extra_cmds = &libext2fs_cmds;

স্থির পূর্ণাঙ্ক print_blocks_proc(ext2_filsys fs EXT2FS_ATTR((unused)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_block, পূর্ণাঙ্ক ref_offset,
			     শূন্য *গুপ্ত EXT2FS_ATTR((unused)))
{
	printf("%6lld %8llu (%d %llu)\n", (long long) blockcnt,
	       (unsigned long long) *blocknr, ref_offset,
	       (unsigned long long) ref_block);
	ঘূৰাই 0;
}


শূন্য do_block_iterate(পূর্ণাঙ্ক argc, ৱর্ণ **argv, পূর্ণাঙ্ক sci_idx EXT2FS_ATTR((unused)),
		      শূন্য *infop EXT2FS_ATTR((unused)))
{
	ধ্রুৱ ৱর্ণ	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	পূর্ণাঙ্ক		err = 0;
	পূর্ণাঙ্ক		flags = 0;

	যদি (common_args_process(argc, argv, 2, 3, argv[0], usage, 0))
		ঘূৰাই;

	ino = string_to_inode(argv[1]);
	যদি (!ino)
		ঘূৰাই;

	যদি (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		যদি (err)
			ঘূৰাই;
	}
	flags |= BLOCK_FLAG_READ_ONLY;

	ext2fs_block_iterate3(current_fs, ino, flags, রিক্ত,
			      print_blocks_proc, রিক্ত);
	putc('\n', stdout);
}
