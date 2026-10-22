/*
 * tst_libext2fs.c
 */

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#د_که_پای
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#د_که_پای
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

#ضمن "ss/ss.h"
#ضمن "debugfs.h"

/*
 * Hook in new commands into debugfs
 * Override debugfs's prompt
 */
ثابت حرف *debug_prog_name = "tst_libext2fs";
خارجي ss_request_جدول libext2fs_cmds;
ss_request_جدول *extra_cmds = &libext2fs_cmds;

ساكن صحيح print_بلاکونه_proc(ext2_filsys fs EXT2FS_ATTR((ناکارول_شوی)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_بلاک, صحيح ref_دننه_واټن,
			     فراغ *خاص EXT2FS_ATTR((ناکارول_شوی)))
{
	چاپ_کړه("%6lld %8llu (%d %llu)\n", (اوږد اوږد) blockcnt,
	       (بینښه اوږد اوږد) *blocknr, ref_دننه_واټن,
	       (بینښه اوږد اوږد) ref_بلاک);
	أرجع 0;
}


فراغ do_بلاک_iterate(صحيح argc, حرف **argv, صحيح sci_idx EXT2FS_ATTR((ناکارول_شوی)),
		      فراغ *infop EXT2FS_ATTR((ناکارول_شوی)))
{
	ثابت حرف	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	صحيح		err = 0;
	صحيح		flags = 0;

	إذا (common_دلیلونه_process(argc, argv, 2, 3, argv[0], usage, 0))
		أرجع;

	ino = string_منزل_inode(argv[1]);
	إذا (!ino)
		أرجع;

	إذا (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		إذا (err)
			أرجع;
	}
	flags |= بلاک_FLAG_READ_ONLY;

	ext2fs_بلاک_iterate3(current_fs, ino, flags, عدم,
			      print_بلاکونه_proc, عدم);
	putc('\n', stdout);
}
