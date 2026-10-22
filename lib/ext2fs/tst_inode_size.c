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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <unistd.h>
#ضمن <stdlib.h>

#ضمن "ext2_fs.h"

بنية ext2_inode_large inode;

#اگر_تعریف_نشده offsetof
#تعریف_کن offsetof(type, member)  __builtin_offsetof(type, member)
#پایان_اگر

#تعریف_کن check_فیلد(x, s) cur_جابه_جایی = do_فیلد(#x, s, حجم(inode.x),	       \
					offsetof(بنية ext2_inode_large, x),  \
					cur_جابه_جایی)

ساكن صحيح do_فیلد(ثابت حرف *فیلد, بیعلامت اندازه, بیعلامت cur_اندازه,
		    بیعلامت جابه_جایی, بیعلامت cur_جابه_جایی)
{
	إذا (اندازه != cur_اندازه) {
		چاپ_کن("error: %s size %u should be %u\n",
		       فیلد, cur_اندازه, اندازه);
		خروج(1);
	}
	إذا (جابه_جایی != cur_جابه_جایی) {
		چاپ_کن("error: %s offset %u should be %u\n",
		       فیلد, cur_جابه_جایی, جابه_جایی);
		خروج(1);
	}
	چاپ_کن("%8d %-30s %3u\n", جابه_جایی, فیلد, (بیعلامت) اندازه);
	أرجع جابه_جایی + اندازه;
}

صحيح اصلی(صحيح argc, حرف **argv)
{
#إذا (__GNUC__ >= 4)
	صحيح cur_جابه_جایی = 0;

	چاپ_کن("%8s %-30s %3s\n", "offset", "field", "size");
	check_فیلد(i_نما, 2);
	check_فیلد(i_uid, 2);
	check_فیلد(i_اندازه, 4);
	check_فیلد(i_atime, 4);
	check_فیلد(i_ctime, 4);
	check_فیلد(i_mtime, 4);
	check_فیلد(i_dtime, 4);
	check_فیلد(i_gid, 2);
	check_فیلد(i_links_شمار, 2);
	check_فیلد(i_بلوکها, 4);
	check_فیلد(i_flags, 4);
	check_فیلد(osd1.linux1.l_i_نسخه, 4);
	check_فیلد(i_بلوک, 15 * 4);
	check_فیلد(i_generation, 4);
	check_فیلد(i_file_acl, 4);
	check_فیلد(i_اندازه_بالا, 4);
	check_فیلد(i_faddr, 4);
	check_فیلد(osd2.linux2.l_i_بلوکها_hi, 2);
	check_فیلد(osd2.linux2.l_i_file_acl_بالا, 2);
	check_فیلد(osd2.linux2.l_i_uid_بالا, 2);
	check_فیلد(osd2.linux2.l_i_gid_بالا, 2);
	check_فیلد(osd2.linux2.l_i_checksum_lo, 2);
	check_فیلد(osd2.linux2.l_i_reserved, 2);
	do_فیلد("Small inode end", 0, 0, cur_جابه_جایی, 128);
	check_فیلد(i_extra_isize, 2);
	check_فیلد(i_checksum_hi, 2);
	check_فیلد(i_ctime_extra, 4);
	check_فیلد(i_mtime_extra, 4);
	check_فیلد(i_atime_extra, 4);
	check_فیلد(i_crtime, 4);
	check_فیلد(i_crtime_extra, 4);
	check_فیلد(i_نسخه_hi, 4);
	check_فیلد(i_projid, 4);
	/* This size will change as new fields are added */
	do_فیلد("Large inode end", 0, 0, cur_جابه_جایی, حجم(inode));
#پایان_اگر
	أرجع 0;
}
