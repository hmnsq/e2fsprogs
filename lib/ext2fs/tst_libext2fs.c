/*
 * tst_libext2fs.c
 */

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#अगर HAVE_ERRNO_H
#समावेश <errno.h>
#पूर्ण_अगर
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fsP.h"

#समावेश "ss/ss.h"
#समावेश "debugfs.h"

/*
 * Hook in new commands into debugfs
 * Override debugfs's prompt
 */
स्थिर अक्षर *debug_prog_name = "tst_libext2fs";
बाह्य ss_request_table libext2fs_cmds;
ss_request_table *extra_cmds = &libext2fs_cmds;

अटल पूर्णांक print_blocks_proc(ext2_filsys fs EXT2FS_ATTR((unused)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_block, पूर्णांक ref_offset,
			     व्योम *निजी EXT2FS_ATTR((unused)))
{
	म_लिखो("%6lld %8llu (%d %llu)\n", (दीर्घ दीर्घ) blockcnt,
	       (अचिन्हित दीर्घ दीर्घ) *blocknr, ref_offset,
	       (अचिन्हित दीर्घ दीर्घ) ref_block);
	वापस 0;
}


व्योम do_block_iterate(पूर्णांक argc, अक्षर **argv, पूर्णांक sci_idx EXT2FS_ATTR((unused)),
		      व्योम *infop EXT2FS_ATTR((unused)))
{
	स्थिर अक्षर	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	पूर्णांक		err = 0;
	पूर्णांक		flags = 0;

	अगर (common_args_process(argc, argv, 2, 3, argv[0], usage, 0))
		वापस;

	ino = string_to_inode(argv[1]);
	अगर (!ino)
		वापस;

	अगर (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		अगर (err)
			वापस;
	}
	flags |= BLOCK_FLAG_READ_ONLY;

	ext2fs_block_iterate3(current_fs, ino, flags, शून्य,
			      print_blocks_proc, शून्य);
	अ_दो('\n', मानक_निकास);
}
