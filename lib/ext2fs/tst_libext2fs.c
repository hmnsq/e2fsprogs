/*
 * tst_libext2fs.c
 */

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#എങ്കിൽ HAVE_ERRNO_H
#അന്തര്ഭാവ <errno.h>
#endif
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

#അന്തര്ഭാവ "ss/ss.h"
#അന്തര്ഭാവ "debugfs.h"

/*
 * Hook in new commands into debugfs
 * Override debugfs's prompt
 */
സ്ഥിരം അക്ഷരം *debug_prog_name = "tst_libext2fs";
ബാഹ്യ ss_request_table libext2fs_cmds;
ss_request_table *extra_cmds = &libext2fs_cmds;

സ്ഥിതം പൂർണ്ണസംഖ്യ print_blocks_proc(ext2_filsys fs EXT2FS_ATTR((unused)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_block, പൂർണ്ണസംഖ്യ ref_offset,
			     ശൂന്യം *സ്വകാര്യ EXT2FS_ATTR((unused)))
{
	printf("%6lld %8llu (%d %llu)\n", (long long) blockcnt,
	       (unsigned long long) *blocknr, ref_offset,
	       (unsigned long long) ref_block);
	തിരികെ 0;
}


ശൂന്യം do_block_iterate(പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv, പൂർണ്ണസംഖ്യ sci_idx EXT2FS_ATTR((unused)),
		      ശൂന്യം *infop EXT2FS_ATTR((unused)))
{
	സ്ഥിരം അക്ഷരം	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	പൂർണ്ണസംഖ്യ		err = 0;
	പൂർണ്ണസംഖ്യ		flags = 0;

	എങ്കിൽ (common_args_process(argc, argv, 2, 3, argv[0], usage, 0))
		തിരികെ;

	ino = string_to_inode(argv[1]);
	എങ്കിൽ (!ino)
		തിരികെ;

	എങ്കിൽ (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		എങ്കിൽ (err)
			തിരികെ;
	}
	flags |= BLOCK_FLAG_READ_ONLY;

	ext2fs_block_iterate3(current_fs, ino, flags, ശൂന്യ,
			      print_blocks_proc, ശൂന്യ);
	putc('\n', stdout);
}
