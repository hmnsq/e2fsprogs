/*
 * This testing program makes sure the ext2_inode structure is 1024 bytes long
 *
 * Copyright (C) 2007 by Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <stdlib.h>

#అన్తర్భావ "ext2_fs.h"

నిర్మాణం ext2_inode_large inode;

#నిర్వచితం_కాకపోతే offsetof
#నిర్వచించు offsetof(type, member)  __builtin_offsetof(type, member)
#అయితే_ముగింపు

#నిర్వచించు check_క్షేత్రం(x, s) cur_స్థానభ్రంశం = do_క్షేత్రం(#x, s, పరిమాణం(inode.x),	       \
					offsetof(నిర్మాణం ext2_inode_large, x),  \
					cur_స్థానభ్రంశం)

స్థిర పూర్ణాంకం do_క్షేత్రం(స్థిరరాశి అక్షరం *క్షేత్రం, గుర్తులేని కొలత, గుర్తులేని cur_కొలత,
		    గుర్తులేని స్థానభ్రంశం, గుర్తులేని cur_స్థానభ్రంశం)
{
	ఒకవేళ (కొలత != cur_కొలత) {
		రాయి("error: %s size %u should be %u\n",
		       క్షేత్రం, cur_కొలత, కొలత);
		నిష్క్రమించు(1);
	}
	ఒకవేళ (స్థానభ్రంశం != cur_స్థానభ్రంశం) {
		రాయి("error: %s offset %u should be %u\n",
		       క్షేత్రం, cur_స్థానభ్రంశం, స్థానభ్రంశం);
		నిష్క్రమించు(1);
	}
	రాయి("%8d %-30s %3u\n", స్థానభ్రంశం, క్షేత్రం, (గుర్తులేని) కొలత);
	తిరిగి స్థానభ్రంశం + కొలత;
}

పూర్ణాంకం ప్రధాన(పూర్ణాంకం argc, అక్షరం **argv)
{
#ఒకవేళ (__GNUC__ >= 4)
	పూర్ణాంకం cur_స్థానభ్రంశం = 0;

	రాయి("%8s %-30s %3s\n", "offset", "field", "size");
	check_క్షేత్రం(i_బహుళకం, 2);
	check_క్షేత్రం(i_uid, 2);
	check_క్షేత్రం(i_కొలత, 4);
	check_క్షేత్రం(i_atime, 4);
	check_క్షేత్రం(i_ctime, 4);
	check_క్షేత్రం(i_mtime, 4);
	check_క్షేత్రం(i_dtime, 4);
	check_క్షేత్రం(i_gid, 2);
	check_క్షేత్రం(i_links_లెక్క, 2);
	check_క్షేత్రం(i_ఖండాలు, 4);
	check_క్షేత్రం(i_flags, 4);
	check_క్షేత్రం(osd1.linux1.l_i_సంస్కరణ, 4);
	check_క్షేత్రం(i_ఖండం, 15 * 4);
	check_క్షేత్రం(i_generation, 4);
	check_క్షేత్రం(i_file_acl, 4);
	check_క్షేత్రం(i_కొలత_ఎగువ, 4);
	check_క్షేత్రం(i_faddr, 4);
	check_క్షేత్రం(osd2.linux2.l_i_ఖండాలు_hi, 2);
	check_క్షేత్రం(osd2.linux2.l_i_file_acl_ఎగువ, 2);
	check_క్షేత్రం(osd2.linux2.l_i_uid_ఎగువ, 2);
	check_క్షేత్రం(osd2.linux2.l_i_gid_ఎగువ, 2);
	check_క్షేత్రం(osd2.linux2.l_i_checksum_lo, 2);
	check_క్షేత్రం(osd2.linux2.l_i_reserved, 2);
	do_క్షేత్రం("Small inode end", 0, 0, cur_స్థానభ్రంశం, 128);
	check_క్షేత్రం(i_extra_isize, 2);
	check_క్షేత్రం(i_checksum_hi, 2);
	check_క్షేత్రం(i_ctime_extra, 4);
	check_క్షేత్రం(i_mtime_extra, 4);
	check_క్షేత్రం(i_atime_extra, 4);
	check_క్షేత్రం(i_crtime, 4);
	check_క్షేత్రం(i_crtime_extra, 4);
	check_క్షేత్రం(i_సంస్కరణ_hi, 4);
	check_క్షేత్రం(i_projid, 4);
	/* This size will change as new fields are added */
	do_క్షేత్రం("Large inode end", 0, 0, cur_స్థానభ్రంశం, పరిమాణం(inode));
#అయితే_ముగింపు
	తిరిగి 0;
}
