/*
 * tst_libext2fs.c
 */

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#اگر_ختم
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

#شامل "ss/ss.h"
#شامل "debugfs.h"

/*
 * Hook in new commands into debugfs
 * Override debugfs's prompt
 */
ثابت حرف *debug_prog_name = "tst_libext2fs";
خارجی ss_request_جدول libext2fs_cmds;
ss_request_جدول *extra_cmds = &libext2fs_cmds;

ساکن صحیح print_بلاکیں_proc(ext2_filsys fs EXT2FS_ATTR((غیر_مستعمل)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_بلاک, صحیح ref_انحراف,
			     خالی *نجی EXT2FS_ATTR((غیر_مستعمل)))
{
	لکھو("%6lld %8llu (%d %llu)\n", (طویل طویل) blockcnt,
	       (غیرعلامتی طویل طویل) *blocknr, ref_انحراف,
	       (غیرعلامتی طویل طویل) ref_بلاک);
	واپس 0;
}


خالی do_بلاک_iterate(صحیح argc, حرف **argv, صحیح sci_idx EXT2FS_ATTR((غیر_مستعمل)),
		      خالی *infop EXT2FS_ATTR((غیر_مستعمل)))
{
	ثابت حرف	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	صحیح		err = 0;
	صحیح		flags = 0;

	اگر (common_دلائل_process(argc, argv, 2, 3, argv[0], usage, 0))
		واپس;

	ino = string_مقصد_inode(argv[1]);
	اگر (!ino)
		واپس;

	اگر (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		اگر (err)
			واپس;
	}
	flags |= بلاک_FLAG_READ_ONLY;

	ext2fs_بلاک_iterate3(current_fs, ino, flags, تہی,
			      print_بلاکیں_proc, تہی);
	putc('\n', stdout);
}
