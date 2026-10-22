/*
 * tst_libext2fs.c
 */

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#endif
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

#شامل "ss/ss.h"
#شامل "debugfs.h"

/*
 * Hook in new commands into debugfs
 * Override debugfs's prompt
 */
ثابت نویسه *debug_prog_name = "tst_libext2fs";
بیرونی ss_request_table libext2fs_cmds;
ss_request_table *extra_cmds = &libext2fs_cmds;

ایستا صحیح print_blocks_proc(ext2_filsys fs EXT2FS_ATTR((unused)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_block, صحیح ref_offset,
			     تهی *خصوصی EXT2FS_ATTR((unused)))
{
	printf("%6lld %8llu (%d %llu)\n", (long long) blockcnt,
	       (unsigned long long) *blocknr, ref_offset,
	       (unsigned long long) ref_block);
	بازگشت 0;
}


تهی do_block_iterate(صحیح argc, نویسه **argv, صحیح sci_idx EXT2FS_ATTR((unused)),
		      تهی *infop EXT2FS_ATTR((unused)))
{
	ثابت نویسه	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	صحیح		err = 0;
	صحیح		flags = 0;

	اگر (common_args_process(argc, argv, 2, 3, argv[0], usage, 0))
		بازگشت;

	ino = string_to_inode(argv[1]);
	اگر (!ino)
		بازگشت;

	اگر (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		اگر (err)
			بازگشت;
	}
	flags |= BLOCK_FLAG_READ_ONLY;

	ext2fs_block_iterate3(current_fs, ino, flags, پوچ,
			      print_blocks_proc, پوچ);
	putc('\n', stdout);
}
