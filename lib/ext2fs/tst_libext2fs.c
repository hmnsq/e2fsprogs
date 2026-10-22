/*
 * tst_libext2fs.c
 */

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#যদি HAVE_ERRNO_H
#অন্তর্ভাব <errno.h>
#endif
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fsP.h"

#অন্তর্ভাব "ss/ss.h"
#অন্তর্ভাব "debugfs.h"

/*
 * Hook in new commands into debugfs
 * Override debugfs's prompt
 */
ধ্রুবক অক্ষর *debug_prog_name = "tst_libext2fs";
বাহ্যিক ss_request_table libext2fs_cmds;
ss_request_table *extra_cmds = &libext2fs_cmds;

স্থির পূর্ণসংখ্যা print_blocks_proc(ext2_filsys fs EXT2FS_ATTR((unused)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_block, পূর্ণসংখ্যা ref_offset,
			     শূন্য *ব্যক্তিগত EXT2FS_ATTR((unused)))
{
	printf("%6lld %8llu (%d %llu)\n", (long long) blockcnt,
	       (unsigned long long) *blocknr, ref_offset,
	       (unsigned long long) ref_block);
	ফেরত 0;
}


শূন্য do_block_iterate(পূর্ণসংখ্যা argc, অক্ষর **argv, পূর্ণসংখ্যা sci_idx EXT2FS_ATTR((unused)),
		      শূন্য *infop EXT2FS_ATTR((unused)))
{
	ধ্রুবক অক্ষর	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	পূর্ণসংখ্যা		err = 0;
	পূর্ণসংখ্যা		flags = 0;

	যদি (common_args_process(argc, argv, 2, 3, argv[0], usage, 0))
		ফেরত;

	ino = string_to_inode(argv[1]);
	যদি (!ino)
		ফেরত;

	যদি (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		যদি (err)
			ফেরত;
	}
	flags |= BLOCK_FLAG_READ_ONLY;

	ext2fs_block_iterate3(current_fs, ino, flags, ফাঁকা,
			      print_blocks_proc, ফাঁকা);
	putc('\n', stdout);
}
