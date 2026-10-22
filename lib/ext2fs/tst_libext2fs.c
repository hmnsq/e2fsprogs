/*
 * tst_libext2fs.c
 */

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଯଦି HAVE_ERRNO_H
#ଅନ୍ତର୍ଭାଵ <errno.h>
#endif
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

#ଅନ୍ତର୍ଭାଵ "ss/ss.h"
#ଅନ୍ତର୍ଭାଵ "debugfs.h"

/*
 * Hook in new commands into debugfs
 * Override debugfs's prompt
 */
ଅଚଳ ଅକ୍ଷର *debug_prog_name = "tst_libext2fs";
ବାହ୍ୟ ss_request_table libext2fs_cmds;
ss_request_table *extra_cmds = &libext2fs_cmds;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା print_blocks_proc(ext2_filsys fs EXT2FS_ATTR((unused)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ref_offset,
			     ଶୂନ୍ୟ *ବ୍ୟକ୍ତିଗତ EXT2FS_ATTR((unused)))
{
	printf("%6lld %8llu (%d %llu)\n", (long long) blockcnt,
	       (unsigned long long) *blocknr, ref_offset,
	       (unsigned long long) ref_block);
	ଫେରନ୍ତୁ 0;
}


ଶୂନ୍ୟ do_block_iterate(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sci_idx EXT2FS_ATTR((unused)),
		      ଶୂନ୍ୟ *infop EXT2FS_ATTR((unused)))
{
	ଅଚଳ ଅକ୍ଷର	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		err = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		flags = 0;

	ଯଦି (common_args_process(argc, argv, 2, 3, argv[0], usage, 0))
		ଫେରନ୍ତୁ;

	ino = string_to_inode(argv[1]);
	ଯଦି (!ino)
		ଫେରନ୍ତୁ;

	ଯଦି (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		ଯଦି (err)
			ଫେରନ୍ତୁ;
	}
	flags |= BLOCK_FLAG_READ_ONLY;

	ext2fs_block_iterate3(current_fs, ino, flags, ଖାଲି,
			      print_blocks_proc, ଖାଲି);
	putc('\n', stdout);
}
