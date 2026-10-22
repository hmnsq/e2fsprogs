/*
 * tst_libext2fs.c
 */

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جے_انت
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت

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

ساكن صحيح print_بلاکاں_proc(ext2_filsys fs EXT2FS_ATTR((بن_ورتیا)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_بلاک, صحيح ref_وتھ,
			     فراغ *خاص EXT2FS_ATTR((بن_ورتیا)))
{
	لکھو("%6lld %8llu (%d %llu)\n", (لما لما) blockcnt,
	       (بنانشانی لما لما) *blocknr, ref_وتھ,
	       (بنانشانی لما لما) ref_بلاک);
	أرجع 0;
}


فراغ do_بلاک_iterate(صحيح argc, حرف **argv, صحيح sci_idx EXT2FS_ATTR((بن_ورتیا)),
		      فراغ *infop EXT2FS_ATTR((بن_ورتیا)))
{
	ثابت حرف	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	صحيح		err = 0;
	صحيح		flags = 0;

	إذا (common_دلیلاں_process(argc, argv, 2, 3, argv[0], usage, 0))
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
			      print_بلاکاں_proc, عدم);
	putc('\n', stdout);
}
