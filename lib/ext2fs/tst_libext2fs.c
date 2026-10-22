/*
 * tst_libext2fs.c
 */

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#எனில் HAVE_ERRNO_H
#அந்தர்பாவ <errno.h>
#endif
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fsP.h"

#அந்தர்பாவ "ss/ss.h"
#அந்தர்பாவ "debugfs.h"

/*
 * Hook in new commands into debugfs
 * Override debugfs's prompt
 */
மாறிலி எழுத்து *debug_prog_name = "tst_libext2fs";
வெளி ss_request_table libext2fs_cmds;
ss_request_table *extra_cmds = &libext2fs_cmds;

நிலையான முழு print_blocks_proc(ext2_filsys fs EXT2FS_ATTR((unused)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_block, முழு ref_offset,
			     வெறுமை *தனிப்பட்ட EXT2FS_ATTR((unused)))
{
	printf("%6lld %8llu (%d %llu)\n", (long long) blockcnt,
	       (unsigned long long) *blocknr, ref_offset,
	       (unsigned long long) ref_block);
	திரும்பு 0;
}


வெறுமை do_block_iterate(முழு argc, எழுத்து **argv, முழு sci_idx EXT2FS_ATTR((unused)),
		      வெறுமை *infop EXT2FS_ATTR((unused)))
{
	மாறிலி எழுத்து	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	முழு		err = 0;
	முழு		flags = 0;

	எனில் (common_args_process(argc, argv, 2, 3, argv[0], usage, 0))
		திரும்பு;

	ino = string_to_inode(argv[1]);
	எனில் (!ino)
		திரும்பு;

	எனில் (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		எனில் (err)
			திரும்பு;
	}
	flags |= BLOCK_FLAG_READ_ONLY;

	ext2fs_block_iterate3(current_fs, ino, flags, வெற்று,
			      print_blocks_proc, வெற்று);
	putc('\n', stdout);
}
