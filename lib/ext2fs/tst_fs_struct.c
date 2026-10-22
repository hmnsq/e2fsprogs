/*
 * This testing program checks the offset of the ext2_filsys structure
 *
 * Copyright (C) 2007 by Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <stdlib.h>

#అన్తర్భావ "ext2fs.h"

నిర్మాణం struct_ext2_filsys fs;

#నిర్వచితం_కాకపోతే offsetof
#నిర్వచించు offsetof(type, member)  __builtin_offsetof (type, member)
#అయితే_ముగింపు
#నిర్వచించు check_క్షేత్రం(x) cur_స్థానభ్రంశం = do_క్షేత్రం(#x, పరిమాణం(fs.x),		\
					offsetof(నిర్మాణం struct_ext2_filsys, x), \
					cur_స్థానభ్రంశం)

స్థిర పూర్ణాంకం do_క్షేత్రం(స్థిరరాశి అక్షరం *క్షేత్రం, size_t కొలత, పూర్ణాంకం స్థానభ్రంశం, పూర్ణాంకం cur_స్థానభ్రంశం)
{
	ఒకవేళ (స్థానభ్రంశం != cur_స్థానభ్రంశం) {
		రాయి("\t(padding %d bytes?)\n", స్థానభ్రంశం - cur_స్థానభ్రంశం);
	}
	రాయి("%8d %-30s %3u\n", స్థానభ్రంశం, క్షేత్రం, (గుర్తులేని) కొలత);
	తిరిగి స్థానభ్రంశం + కొలత;
}

పూర్ణాంకం ప్రధాన(పూర్ణాంకం argc, అక్షరం **argv)
{
#ఒకవేళ (__GNUC__ >= 4)
	పూర్ణాంకం cur_స్థానభ్రంశం = 0;

	రాయి("%8s %-30s %3s\n", "offset", "field", "size");
	check_క్షేత్రం(magic);
	check_క్షేత్రం(io);
	check_క్షేత్రం(flags);
	check_క్షేత్రం(device_name);
	check_క్షేత్రం(super);
	check_క్షేత్రం(blocksize);
	check_క్షేత్రం(fragsize);
	check_క్షేత్రం(సమూహం_desc_లెక్క);
	check_క్షేత్రం(desc_ఖండాలు);
	check_క్షేత్రం(సమూహం_desc);
	check_క్షేత్రం(inode_ఖండాలు_చొప్పున_సమూహం);
	check_క్షేత్రం(inode_map);
	check_క్షేత్రం(ఖండం_map);
	check_క్షేత్రం(get_ఖండాలు);
	check_క్షేత్రం(check_directory);
	check_క్షేత్రం(write_bitmaps);
	check_క్షేత్రం(read_inode);
	check_క్షేత్రం(write_inode);
	check_క్షేత్రం(badblocks);
	check_క్షేత్రం(dblist);
	check_క్షేత్రం(stride);
	check_క్షేత్రం(orig_super);
	check_క్షేత్రం(image_శీర్షిక);
	check_క్షేత్రం(umask);
	check_క్షేత్రం(now);
	check_క్షేత్రం(cluster_నిష్పత్తి_బిట్లు);
	check_క్షేత్రం(reserved);
	check_క్షేత్రం(priv_దత్తాంశం);
	check_క్షేత్రం(icache);
	check_క్షేత్రం(image_io);
	check_క్షేత్రం(get_alloc_ఖండం);
	check_క్షేత్రం(ఖండం_alloc_stats);
	check_క్షేత్రం(mmp_buf);
	check_క్షేత్రం(mmp_cmp);
	check_క్షేత్రం(mmp_fd);
	check_క్షేత్రం(mmp_చివరి_written);
	రాయి("Ending offset is %d\n\n", cur_స్థానభ్రంశం);
#అయితే_ముగింపు
	నిష్క్రమించు(0);
}
