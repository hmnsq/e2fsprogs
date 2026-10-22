/*
 * tst_libext2fs.c
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#जर HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#जर HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जर HAVE_SYS_TYPES_H
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
स्थिरांक अक्षर *debug_prog_name = "tst_libext2fs";
बाह्य ss_request_table libext2fs_cmds;
ss_request_table *extra_cmds = &libext2fs_cmds;

स्थिर पूर्णांक print_blocks_proc(ext2_filsys fs EXT2FS_ATTR((unused)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_block, पूर्णांक ref_offset,
			     शून्य *खाजगी EXT2FS_ATTR((unused)))
{
	म_लिखो("%6lld %8llu (%d %llu)\n", (दीर्घ दीर्घ) blockcnt,
	       (अचिन्हित दीर्घ दीर्घ) *blocknr, ref_offset,
	       (अचिन्हित दीर्घ दीर्घ) ref_block);
	परत 0;
}


शून्य do_block_iterate(पूर्णांक argc, अक्षर **argv, पूर्णांक sci_idx EXT2FS_ATTR((unused)),
		      शून्य *infop EXT2FS_ATTR((unused)))
{
	स्थिरांक अक्षर	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	पूर्णांक		err = 0;
	पूर्णांक		flags = 0;

	जर (common_args_process(argc, argv, 2, 3, argv[0], usage, 0))
		परत;

	ino = string_to_inode(argv[1]);
	जर (!ino)
		परत;

	जर (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		जर (err)
			परत;
	}
	flags |= BLOCK_FLAG_READ_ONLY;

	ext2fs_block_iterate3(current_fs, ino, flags, रिक्त,
			      print_blocks_proc, रिक्त);
	अ_दो('\n', मानक_निकास);
}
