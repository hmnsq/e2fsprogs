/*
 * tst_libext2fs.c
 */

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جيڪڏهن_پورو
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو

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

ساكن صحيح print_بلاڪون_proc(ext2_filsys fs EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_بلاڪ, صحيح ref_وٿي,
			     فراغ *خاص EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	لکو("%6lld %8llu (%d %llu)\n", (ڊگهو ڊگهو) blockcnt,
	       (بنانشان ڊگهو ڊگهو) *blocknr, ref_وٿي,
	       (بنانشان ڊگهو ڊگهو) ref_بلاڪ);
	أرجع 0;
}


فراغ do_بلاڪ_iterate(صحيح argc, حرف **argv, صحيح sci_idx EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
		      فراغ *infop EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	ثابت حرف	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	صحيح		err = 0;
	صحيح		flags = 0;

	إذا (common_دليلون_process(argc, argv, 2, 3, argv[0], usage, 0))
		أرجع;

	ino = string_منزل_inode(argv[1]);
	إذا (!ino)
		أرجع;

	إذا (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		إذا (err)
			أرجع;
	}
	flags |= بلاڪ_FLAG_READ_ONLY;

	ext2fs_بلاڪ_iterate3(current_fs, ino, flags, عدم,
			      print_بلاڪون_proc, عدم);
	putc('\n', stdout);
}
