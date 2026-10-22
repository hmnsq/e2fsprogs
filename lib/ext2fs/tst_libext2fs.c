/*
 * tst_libext2fs.c
 */

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#જો HAVE_ERRNO_H
#અન્તર્ભાવ <errno.h>
#endif
#જો HAVE_SYS_STAT_H
#અન્તર્ભાવ <sys/stat.h>
#endif
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fsP.h"

#અન્તર્ભાવ "ss/ss.h"
#અન્તર્ભાવ "debugfs.h"

/*
 * Hook in new commands into debugfs
 * Override debugfs's prompt
 */
અચલ અક્ષર *debug_prog_name = "tst_libext2fs";
બાહ્ય ss_request_table libext2fs_cmds;
ss_request_table *extra_cmds = &libext2fs_cmds;

સ્થિર પૂર્ણાંક print_blocks_proc(ext2_filsys fs EXT2FS_ATTR((unused)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_block, પૂર્ણાંક ref_offset,
			     શૂન્ય *ખાનગી EXT2FS_ATTR((unused)))
{
	printf("%6lld %8llu (%d %llu)\n", (long long) blockcnt,
	       (unsigned long long) *blocknr, ref_offset,
	       (unsigned long long) ref_block);
	પાછા 0;
}


શૂન્ય do_block_iterate(પૂર્ણાંક argc, અક્ષર **argv, પૂર્ણાંક sci_idx EXT2FS_ATTR((unused)),
		      શૂન્ય *infop EXT2FS_ATTR((unused)))
{
	અચલ અક્ષર	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	પૂર્ણાંક		err = 0;
	પૂર્ણાંક		flags = 0;

	જો (common_args_process(argc, argv, 2, 3, argv[0], usage, 0))
		પાછા;

	ino = string_to_inode(argv[1]);
	જો (!ino)
		પાછા;

	જો (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		જો (err)
			પાછા;
	}
	flags |= BLOCK_FLAG_READ_ONLY;

	ext2fs_block_iterate3(current_fs, ino, flags, ખાલી,
			      print_blocks_proc, ખાલી);
	putc('\n', stdout);
}
