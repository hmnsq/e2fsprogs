/*
 * tst_libext2fs.c
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

#अन्तर्भाव "ss/ss.h"
#अन्तर्भाव "debugfs.h"

/*
 * Hook in new commands into debugfs
 * Override debugfs's prompt
 */
अचर वर्ण *debug_prog_name = "tst_libext2fs";
बाह्य ss_request_table libext2fs_cmds;
ss_request_table *extra_cmds = &libext2fs_cmds;

स्थिर पूर्णाङ्क print_blocks_proc(ext2_filsys fs EXT2FS_ATTR((unused)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_block, पूर्णाङ्क ref_offset,
			     शून्य *गुप्त EXT2FS_ATTR((unused)))
{
	म_लिखो("%6lld %8llu (%d %llu)\n", (दीर्घ दीर्घ) blockcnt,
	       (अचिन्हित दीर्घ दीर्घ) *blocknr, ref_offset,
	       (अचिन्हित दीर्घ दीर्घ) ref_block);
	फर्काऊ 0;
}


शून्य do_block_iterate(पूर्णाङ्क argc, वर्ण **argv, पूर्णाङ्क sci_idx EXT2FS_ATTR((unused)),
		      शून्य *infop EXT2FS_ATTR((unused)))
{
	अचर वर्ण	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	पूर्णाङ्क		err = 0;
	पूर्णाङ्क		flags = 0;

	यदि (common_args_process(argc, argv, 2, 3, argv[0], usage, 0))
		फर्काऊ;

	ino = string_to_inode(argv[1]);
	यदि (!ino)
		फर्काऊ;

	यदि (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		यदि (err)
			फर्काऊ;
	}
	flags |= BLOCK_FLAG_READ_ONLY;

	ext2fs_block_iterate3(current_fs, ino, flags, रिक्त,
			      print_blocks_proc, रिक्त);
	अ_दो('\n', मानक_निकास);
}
