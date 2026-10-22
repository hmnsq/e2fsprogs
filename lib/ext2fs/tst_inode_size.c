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

#جيڪڏهن_وصف_نه_ٿيل offsetof
#وصف_ڪريو offsetof(type, member)  __builtin_offsetof(type, member)
#جيڪڏهن_پورو

#وصف_ڪريو check_ميدان(x, s) cur_وٿي = do_ميدان(#x, s, حجم(inode.x),	       \
					offsetof(بنية ext2_inode_large, x),  \
					cur_وٿي)

ساكن صحيح do_ميدان(ثابت حرف *ميدان, بنانشان سائيز, بنانشان cur_سائيز,
		    بنانشان وٿي, بنانشان cur_وٿي)
{
	إذا (سائيز != cur_سائيز) {
		لکو("error: %s size %u should be %u\n",
		       ميدان, cur_سائيز, سائيز);
		ٻاهر_نڪرو(1);
	}
	إذا (وٿي != cur_وٿي) {
		لکو("error: %s offset %u should be %u\n",
		       ميدان, cur_وٿي, وٿي);
		ٻاهر_نڪرو(1);
	}
	لکو("%8d %-30s %3u\n", وٿي, ميدان, (بنانشان) سائيز);
	أرجع وٿي + سائيز;
}

صحيح مکيه(صحيح argc, حرف **argv)
{
#إذا (__GNUC__ >= 4)
	صحيح cur_وٿي = 0;

	لکو("%8s %-30s %3s\n", "offset", "field", "size");
	check_ميدان(i_گهڻائي, 2);
	check_ميدان(i_uid, 2);
	check_ميدان(i_سائيز, 4);
	check_ميدان(i_atime, 4);
	check_ميدان(i_ctime, 4);
	check_ميدان(i_mtime, 4);
	check_ميدان(i_dtime, 4);
	check_ميدان(i_gid, 2);
	check_ميدان(i_links_ڳڻپ, 2);
	check_ميدان(i_بلاڪون, 4);
	check_ميدان(i_flags, 4);
	check_ميدان(osd1.linux1.l_i_نسخو, 4);
	check_ميدان(i_بلاڪ, 15 * 4);
	check_ميدان(i_generation, 4);
	check_ميدان(i_file_acl, 4);
	check_ميدان(i_سائيز_مٿاهون, 4);
	check_ميدان(i_faddr, 4);
	check_ميدان(osd2.linux2.l_i_بلاڪون_hi, 2);
	check_ميدان(osd2.linux2.l_i_file_acl_مٿاهون, 2);
	check_ميدان(osd2.linux2.l_i_uid_مٿاهون, 2);
	check_ميدان(osd2.linux2.l_i_gid_مٿاهون, 2);
	check_ميدان(osd2.linux2.l_i_checksum_lo, 2);
	check_ميدان(osd2.linux2.l_i_reserved, 2);
	do_ميدان("Small inode end", 0, 0, cur_وٿي, 128);
	check_ميدان(i_extra_isize, 2);
	check_ميدان(i_checksum_hi, 2);
	check_ميدان(i_ctime_extra, 4);
	check_ميدان(i_mtime_extra, 4);
	check_ميدان(i_atime_extra, 4);
	check_ميدان(i_crtime, 4);
	check_ميدان(i_crtime_extra, 4);
	check_ميدان(i_نسخو_hi, 4);
	check_ميدان(i_projid, 4);
	/* This size will change as new fields are added */
	do_ميدان("Large inode end", 0, 0, cur_وٿي, حجم(inode));
#جيڪڏهن_پورو
	أرجع 0;
}
