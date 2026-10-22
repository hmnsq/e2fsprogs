/*
 * tst_libext2fs.c
 */

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fsP.h"

#అన్తర్భావ "ss/ss.h"
#అన్తర్భావ "debugfs.h"

/*
 * Hook in new commands into debugfs
 * Override debugfs's prompt
 */
స్థిరరాశి అక్షరం *debug_prog_name = "tst_libext2fs";
బాహ్య ss_request_పట్టిక libext2fs_cmds;
ss_request_పట్టిక *extra_cmds = &libext2fs_cmds;

స్థిర పూర్ణాంకం print_ఖండాలు_proc(ext2_filsys fs EXT2FS_ATTR((వాడని)),
			     blk64_t *blocknr, e2_blkcnt_t blockcnt,
			     blk64_t ref_ఖండం, పూర్ణాంకం ref_స్థానభ్రంశం,
			     శూన్య *అంతరంగ EXT2FS_ATTR((వాడని)))
{
	రాయి("%6lld %8llu (%d %llu)\n", (దీర్ఘ దీర్ఘ) blockcnt,
	       (గుర్తులేని దీర్ఘ దీర్ఘ) *blocknr, ref_స్థానభ్రంశం,
	       (గుర్తులేని దీర్ఘ దీర్ఘ) ref_ఖండం);
	తిరిగి 0;
}


శూన్య do_ఖండం_iterate(పూర్ణాంకం argc, అక్షరం **argv, పూర్ణాంకం sci_idx EXT2FS_ATTR((వాడని)),
		      శూన్య *infop EXT2FS_ATTR((వాడని)))
{
	స్థిరరాశి అక్షరం	*usage = "block_iterate <file> <flags";
	ext2_ino_t	ino;
	పూర్ణాంకం		err = 0;
	పూర్ణాంకం		flags = 0;

	ఒకవేళ (common_వాదనలు_process(argc, argv, 2, 3, argv[0], usage, 0))
		తిరిగి;

	ino = string_గమ్యం_inode(argv[1]);
	ఒకవేళ (!ino)
		తిరిగి;

	ఒకవేళ (argc > 2) {
		flags = parse_ulong(argv[2], argv[0], "flags", &err);
		ఒకవేళ (err)
			తిరిగి;
	}
	flags |= ఖండం_FLAG_READ_ONLY;

	ext2fs_ఖండం_iterate3(current_fs, ino, flags, ఖాళీ,
			      print_ఖండాలు_proc, ఖాళీ);
	putc('\n', stdout);
}
