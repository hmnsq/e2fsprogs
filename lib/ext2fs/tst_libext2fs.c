/*
 * tst_libext2fs.c
 */

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#यदि_अन्तः
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

#अन्तर्भाव "ss/ss.h"
#अन्तर्भाव "debugfs.h"

/*
 * Hook in new commands into debugfs
 * Override debugfs's prompt
 */
ध्रुव वर्ण *debug_prog_name = "tst_libext2fs";
बाह्य ss_request_सारणी libext2fs_cmds;
ss_request_सारणी *extra_cmds = &libext2fs_cmds;

स्थिर पूर्णाङ्क print_खण्डानि_proc(ext2_filsys fs EXT2FS_ATTR((अप्रयुक्तम्)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_खण्डम्, पूर्णाङ्क ref_विस्थापनम्,
			     शून्य *गुप्त EXT2FS_ATTR((अप्रयुक्तम्)))
{
	प्रारूप_लिख("%6lld %8llu (%d %llu)\n", (दीर्घ दीर्घ) blockcnt,
	       (अचिह्नित दीर्घ दीर्घ) *blocknr, ref_विस्थापनम्,
	       (अचिह्नित दीर्घ दीर्घ) ref_खण्डम्);
	प्रत्यागम 0;
}


शून्य do_खण्डम्_iterate(पूर्णाङ्क argc, वर्ण **argv, पूर्णाङ्क sci_idx EXT2FS_ATTR((अप्रयुक्तम्)),
		      शून्य *infop EXT2FS_ATTR((अप्रयुक्तम्)))
{
	ध्रुव वर्ण	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	पूर्णाङ्क		err = 0;
	पूर्णाङ्क		flags = 0;

	यदि (common_तर्काः_process(argc, argv, 2, 3, argv[0], usage, 0))
		प्रत्यागम;

	ino = string_गन्तव्यम्_inode(argv[1]);
	यदि (!ino)
		प्रत्यागम;

	यदि (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		यदि (err)
			प्रत्यागम;
	}
	flags |= खण्डम्_FLAG_READ_ONLY;

	ext2fs_खण्डम्_iterate3(current_fs, ino, flags, रिक्त,
			      print_खण्डानि_proc, रिक्त);
	putc('\n', stdout);
}
