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

#شامل "config.h"
#شامل <stdio.h>
#شامل <unistd.h>
#شامل <stdlib.h>

#شامل "ext2_fs.h"

ساخت ext2_inode_large inode;

#اگر_غیر_تعریف_شدہ offsetof
#تعریف_کرو offsetof(type, member)  __builtin_offsetof(type, member)
#اگر_ختم

#تعریف_کرو check_میدان(x, s) cur_انحراف = do_میدان(#x, s, حجم(inode.x),	       \
					offsetof(ساخت ext2_inode_large, x),  \
					cur_انحراف)

ساکن صحیح do_میدان(ثابت حرف *میدان, غیرعلامتی سائز, غیرعلامتی cur_سائز,
		    غیرعلامتی انحراف, غیرعلامتی cur_انحراف)
{
	اگر (سائز != cur_سائز) {
		لکھو("error: %s size %u should be %u\n",
		       میدان, cur_سائز, سائز);
		باہر_نکلو(1);
	}
	اگر (انحراف != cur_انحراف) {
		لکھو("error: %s offset %u should be %u\n",
		       میدان, cur_انحراف, انحراف);
		باہر_نکلو(1);
	}
	لکھو("%8d %-30s %3u\n", انحراف, میدان, (غیرعلامتی) سائز);
	واپس انحراف + سائز;
}

صحیح مین(صحیح argc, حرف **argv)
{
#اگر (__GNUC__ >= 4)
	صحیح cur_انحراف = 0;

	لکھو("%8s %-30s %3s\n", "offset", "field", "size");
	check_میدان(i_منوال, 2);
	check_میدان(i_uid, 2);
	check_میدان(i_سائز, 4);
	check_میدان(i_atime, 4);
	check_میدان(i_ctime, 4);
	check_میدان(i_mtime, 4);
	check_میدان(i_dtime, 4);
	check_میدان(i_gid, 2);
	check_میدان(i_links_گنتی, 2);
	check_میدان(i_بلاکیں, 4);
	check_میدان(i_flags, 4);
	check_میدان(osd1.linux1.l_i_نسخہ, 4);
	check_میدان(i_بلاک, 15 * 4);
	check_میدان(i_generation, 4);
	check_میدان(i_file_acl, 4);
	check_میدان(i_سائز_بلند, 4);
	check_میدان(i_faddr, 4);
	check_میدان(osd2.linux2.l_i_بلاکیں_hi, 2);
	check_میدان(osd2.linux2.l_i_file_acl_بلند, 2);
	check_میدان(osd2.linux2.l_i_uid_بلند, 2);
	check_میدان(osd2.linux2.l_i_gid_بلند, 2);
	check_میدان(osd2.linux2.l_i_checksum_lo, 2);
	check_میدان(osd2.linux2.l_i_reserved, 2);
	do_میدان("Small inode end", 0, 0, cur_انحراف, 128);
	check_میدان(i_extra_isize, 2);
	check_میدان(i_checksum_hi, 2);
	check_میدان(i_ctime_extra, 4);
	check_میدان(i_mtime_extra, 4);
	check_میدان(i_atime_extra, 4);
	check_میدان(i_crtime, 4);
	check_میدان(i_crtime_extra, 4);
	check_میدان(i_نسخہ_hi, 4);
	check_میدان(i_projid, 4);
	/* This size will change as new fields are added */
	do_میدان("Large inode end", 0, 0, cur_انحراف, حجم(inode));
#اگر_ختم
	واپس 0;
}
