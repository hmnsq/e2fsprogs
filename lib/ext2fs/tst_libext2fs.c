/*
 * tst_libext2fs.c
 */

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#پایان_اگر
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#پایان_اگر
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر

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

ساكن صحيح print_بلوکها_proc(ext2_filsys fs EXT2FS_ATTR((استفادهنشده)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_بلوک, صحيح ref_جابه_جایی,
			     فراغ *خاص EXT2FS_ATTR((استفادهنشده)))
{
	چاپ_کن("%6lld %8llu (%d %llu)\n", (دراز دراز) blockcnt,
	       (بیعلامت دراز دراز) *blocknr, ref_جابه_جایی,
	       (بیعلامت دراز دراز) ref_بلوک);
	أرجع 0;
}


فراغ do_بلوک_iterate(صحيح argc, حرف **argv, صحيح sci_idx EXT2FS_ATTR((استفادهنشده)),
		      فراغ *infop EXT2FS_ATTR((استفادهنشده)))
{
	ثابت حرف	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	صحيح		err = 0;
	صحيح		flags = 0;

	إذا (common_آرگومانها_process(argc, argv, 2, 3, argv[0], usage, 0))
		أرجع;

	ino = string_مقصد_inode(argv[1]);
	إذا (!ino)
		أرجع;

	إذا (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		إذا (err)
			أرجع;
	}
	flags |= بلوک_FLAG_READ_ONLY;

	ext2fs_بلوک_iterate3(current_fs, ino, flags, عدم,
			      print_بلوکها_proc, عدم);
	putc('\n', stdout);
}
