/*
 * tst_libext2fs.c
 */

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

#ಅನ್ತರ್ಭಾವ "ss/ss.h"
#ಅನ್ತರ್ಭಾವ "debugfs.h"

/*
 * Hook in new commands into debugfs
 * Override debugfs's prompt
 */
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *debug_prog_name = "tst_libext2fs";
ಬಾಹ್ಯ ss_request_ಕೋಷ್ಟಕ libext2fs_cmds;
ss_request_ಕೋಷ್ಟಕ *extra_cmds = &libext2fs_cmds;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ print_ಖಂಡಗಳು_proc(ext2_filsys fs EXT2FS_ATTR((ಬಳಸದ)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_ಖಂಡ, ಪೂರ್ಣಾಂಕ ref_ಸ್ಥಾನಾಂತರ,
			     ಶೂನ್ಯ *ಖಾಸಗಿ EXT2FS_ATTR((ಬಳಸದ)))
{
	ಬರೆ("%6lld %8llu (%d %llu)\n", (ದೀರ್ಘ ದೀರ್ಘ) blockcnt,
	       (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) *blocknr, ref_ಸ್ಥಾನಾಂತರ,
	       (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ref_ಖಂಡ);
	ಹಿಂತಿರುಗಿ 0;
}


ಶೂನ್ಯ do_ಖಂಡ_iterate(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv, ಪೂರ್ಣಾಂಕ sci_idx EXT2FS_ATTR((ಬಳಸದ)),
		      ಶೂನ್ಯ *infop EXT2FS_ATTR((ಬಳಸದ)))
{
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	ಪೂರ್ಣಾಂಕ		err = 0;
	ಪೂರ್ಣಾಂಕ		flags = 0;

	ಆದರೆ (common_ವಾದಗಳು_process(argc, argv, 2, 3, argv[0], usage, 0))
		ಹಿಂತಿರುಗಿ;

	ino = string_ಗಮ್ಯ_inode(argv[1]);
	ಆದರೆ (!ino)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		ಆದರೆ (err)
			ಹಿಂತಿರುಗಿ;
	}
	flags |= ಖಂಡ_FLAG_READ_ONLY;

	ext2fs_ಖಂಡ_iterate3(current_fs, ino, flags, ಖಾಲಿ,
			      print_ಖಂಡಗಳು_proc, ಖಾಲಿ);
	putc('\n', stdout);
}
