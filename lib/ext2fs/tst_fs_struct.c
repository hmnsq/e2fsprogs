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

#ضمن <stdio.h>
#ضمن <unistd.h>
#ضمن <stdlib.h>

#ضمن "ext2fs.h"

بنية struct_ext2_filsys fs;

#که_نه_تعریف_شوی offsetof
#تعریف_کړه offsetof(type, member)  __builtin_offsetof (type, member)
#د_که_پای
#تعریف_کړه check_ډګر(x) cur_دننه_واټن = do_ډګر(#x, حجم(fs.x),		\
					offsetof(بنية struct_ext2_filsys, x), \
					cur_دننه_واټن)

ساكن صحيح do_ډګر(ثابت حرف *ډګر, size_t اندازه, صحيح دننه_واټن, صحيح cur_دننه_واټن)
{
	إذا (دننه_واټن != cur_دننه_واټن) {
		چاپ_کړه("\t(padding %d bytes?)\n", دننه_واټن - cur_دننه_واټن);
	}
	چاپ_کړه("%8d %-30s %3u\n", دننه_واټن, ډګر, (بینښه) اندازه);
	أرجع دننه_واټن + اندازه;
}

صحيح اصلي(صحيح argc, حرف **argv)
{
#إذا (__GNUC__ >= 4)
	صحيح cur_دننه_واټن = 0;

	چاپ_کړه("%8s %-30s %3s\n", "offset", "field", "size");
	check_ډګر(magic);
	check_ډګر(io);
	check_ډګر(flags);
	check_ډګر(device_name);
	check_ډګر(super);
	check_ډګر(blocksize);
	check_ډګر(fragsize);
	check_ډګر(ډله_desc_شمېر);
	check_ډګر(desc_بلاکونه);
	check_ډګر(ډله_desc);
	check_ډګر(inode_بلاکونه_فی_ډله);
	check_ډګر(inode_map);
	check_ډګر(بلاک_map);
	check_ډګر(get_بلاکونه);
	check_ډګر(check_directory);
	check_ډګر(write_bitmaps);
	check_ډګر(read_inode);
	check_ډګر(write_inode);
	check_ډګر(badblocks);
	check_ډګر(dblist);
	check_ډګر(stride);
	check_ډګر(orig_super);
	check_ډګر(image_سرلیک);
	check_ډګر(umask);
	check_ډګر(now);
	check_ډګر(cluster_نسبت_بټونه);
	check_ډګر(reserved);
	check_ډګر(priv_معلومات);
	check_ډګر(icache);
	check_ډګر(image_io);
	check_ډګر(get_alloc_بلاک);
	check_ډګر(بلاک_alloc_stats);
	check_ډګر(mmp_buf);
	check_ډګر(mmp_cmp);
	check_ډګر(mmp_fd);
	check_ډګر(mmp_وروستی_written);
	چاپ_کړه("Ending offset is %d\n\n", cur_دننه_واټن);
#د_که_پای
	وتل(0);
}
