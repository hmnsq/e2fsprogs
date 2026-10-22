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

#که_نه_تعریف_شوی offsetof
#تعریف_کړه offsetof(type, member)  __builtin_offsetof(type, member)
#د_که_پای

#تعریف_کړه check_ډګر(x, s) cur_دننه_واټن = do_ډګر(#x, s, حجم(inode.x),	       \
					offsetof(بنية ext2_inode_large, x),  \
					cur_دننه_واټن)

ساكن صحيح do_ډګر(ثابت حرف *ډګر, بینښه اندازه, بینښه cur_اندازه,
		    بینښه دننه_واټن, بینښه cur_دننه_واټن)
{
	إذا (اندازه != cur_اندازه) {
		چاپ_کړه("error: %s size %u should be %u\n",
		       ډګر, cur_اندازه, اندازه);
		وتل(1);
	}
	إذا (دننه_واټن != cur_دننه_واټن) {
		چاپ_کړه("error: %s offset %u should be %u\n",
		       ډګر, cur_دننه_واټن, دننه_واټن);
		وتل(1);
	}
	چاپ_کړه("%8d %-30s %3u\n", دننه_واټن, ډګر, (بینښه) اندازه);
	أرجع دننه_واټن + اندازه;
}

صحيح اصلي(صحيح argc, حرف **argv)
{
#إذا (__GNUC__ >= 4)
	صحيح cur_دننه_واټن = 0;

	چاپ_کړه("%8s %-30s %3s\n", "offset", "field", "size");
	check_ډګر(i_ډېرپېښ, 2);
	check_ډګر(i_uid, 2);
	check_ډګر(i_اندازه, 4);
	check_ډګر(i_atime, 4);
	check_ډګر(i_ctime, 4);
	check_ډګر(i_mtime, 4);
	check_ډګر(i_dtime, 4);
	check_ډګر(i_gid, 2);
	check_ډګر(i_links_شمېر, 2);
	check_ډګر(i_بلاکونه, 4);
	check_ډګر(i_flags, 4);
	check_ډګر(osd1.linux1.l_i_نسخه, 4);
	check_ډګر(i_بلاک, 15 * 4);
	check_ډګر(i_generation, 4);
	check_ډګر(i_file_acl, 4);
	check_ډګر(i_اندازه_لوړ, 4);
	check_ډګر(i_faddr, 4);
	check_ډګر(osd2.linux2.l_i_بلاکونه_hi, 2);
	check_ډګر(osd2.linux2.l_i_file_acl_لوړ, 2);
	check_ډګر(osd2.linux2.l_i_uid_لوړ, 2);
	check_ډګر(osd2.linux2.l_i_gid_لوړ, 2);
	check_ډګر(osd2.linux2.l_i_checksum_lo, 2);
	check_ډګر(osd2.linux2.l_i_reserved, 2);
	do_ډګر("Small inode end", 0, 0, cur_دننه_واټن, 128);
	check_ډګر(i_extra_isize, 2);
	check_ډګر(i_checksum_hi, 2);
	check_ډګر(i_ctime_extra, 4);
	check_ډګر(i_mtime_extra, 4);
	check_ډګر(i_atime_extra, 4);
	check_ډګر(i_crtime, 4);
	check_ډګر(i_crtime_extra, 4);
	check_ډګر(i_نسخه_hi, 4);
	check_ډګر(i_projid, 4);
	/* This size will change as new fields are added */
	do_ډګر("Large inode end", 0, 0, cur_دننه_واټن, حجم(inode));
#د_که_پای
	أرجع 0;
}
