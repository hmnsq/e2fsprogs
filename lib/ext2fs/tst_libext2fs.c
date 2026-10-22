/*
 * tst_libext2fs.c
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_ERRNO_H
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "ss/ss.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "debugfs.h"

/*
 * Hook in new commands into debugfs
 * Override debugfs's prompt
 */
ਅਚਲ ਅੱਖਰ *debug_prog_name = "tst_libext2fs";
ਬਾਹਰੀ ss_request_ਸਾਰਣੀ libext2fs_cmds;
ss_request_ਸਾਰਣੀ *extra_cmds = &libext2fs_cmds;

ਸਥਿਰ ਪੂਰਨ print_ਖੰਡਾਂ_proc(ext2_filsys fs EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_ਖੰਡ, ਪੂਰਨ ref_ਵਿੱਥ,
			     ਖਾਲੀ *ਨਿਜੀ EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਲਿਖੋ("%6lld %8llu (%d %llu)\n", (ਲੰਮਾ ਲੰਮਾ) blockcnt,
	       (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) *blocknr, ref_ਵਿੱਥ,
	       (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ref_ਖੰਡ);
	ਵਾਪਸ 0;
}


ਖਾਲੀ do_ਖੰਡ_iterate(ਪੂਰਨ argc, ਅੱਖਰ **argv, ਪੂਰਨ sci_idx EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
		      ਖਾਲੀ *infop EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਅਚਲ ਅੱਖਰ	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	ਪੂਰਨ		err = 0;
	ਪੂਰਨ		flags = 0;

	ਜੇ (common_ਦਲੀਲਾਂ_process(argc, argv, 2, 3, argv[0], usage, 0))
		ਵਾਪਸ;

	ino = string_ਮੰਜ਼ਿਲ_inode(argv[1]);
	ਜੇ (!ino)
		ਵਾਪਸ;

	ਜੇ (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		ਜੇ (err)
			ਵਾਪਸ;
	}
	flags |= ਖੰਡ_FLAG_READ_ONLY;

	ext2fs_ਖੰਡ_iterate3(current_fs, ino, flags, ਨਿਰਾ,
			      print_ਖੰਡਾਂ_proc, ਨਿਰਾ);
	putc('\n', stdout);
}
