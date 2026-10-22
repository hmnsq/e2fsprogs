/*
 * tst_libext2fs.c
 */

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಯದಿ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#endif
#ಯದಿ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#endif
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

#ಅನ್ತರ್ಭಾವ "ss/ss.h"
#ಅನ್ತರ್ಭಾವ "debugfs.h"

/*
 * Hook in new commands into debugfs
 * Override debugfs's prompt
 */
ಧ್ರುವ ವರ್ಣ *debug_prog_name = "tst_libext2fs";
ಬಾಹ್ಯ ss_request_table libext2fs_cmds;
ss_request_table *extra_cmds = &libext2fs_cmds;

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ print_blocks_proc(ext2_filsys fs EXT2FS_ATTR((unused)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_block, ಪೂರ್ಣಾಙ್ಕ ref_offset,
			     ಶೂನ್ಯ *ಗುಪ್ತ EXT2FS_ATTR((unused)))
{
	printf("%6lld %8llu (%d %llu)\n", (long long) blockcnt,
	       (unsigned long long) *blocknr, ref_offset,
	       (unsigned long long) ref_block);
	ಪ್ರತ್ಯಾಗಮ 0;
}


ಶೂನ್ಯ do_block_iterate(ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv, ಪೂರ್ಣಾಙ್ಕ sci_idx EXT2FS_ATTR((unused)),
		      ಶೂನ್ಯ *infop EXT2FS_ATTR((unused)))
{
	ಧ್ರುವ ವರ್ಣ	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	ಪೂರ್ಣಾಙ್ಕ		err = 0;
	ಪೂರ್ಣಾಙ್ಕ		flags = 0;

	ಯದಿ (common_args_process(argc, argv, 2, 3, argv[0], usage, 0))
		ಪ್ರತ್ಯಾಗಮ;

	ino = string_to_inode(argv[1]);
	ಯದಿ (!ino)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		ಯದಿ (err)
			ಪ್ರತ್ಯಾಗಮ;
	}
	flags |= BLOCK_FLAG_READ_ONLY;

	ext2fs_block_iterate3(current_fs, ino, flags, ರಿಕ್ತ,
			      print_blocks_proc, ರಿಕ್ತ);
	putc('\n', stdout);
}
