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

#اگر_تعریف_نشده offsetof
#تعریف_کن offsetof(type, member)  __builtin_offsetof (type, member)
#پایان_اگر
#تعریف_کن check_فیلد(x) cur_جابه_جایی = do_فیلد(#x, حجم(fs.x),		\
					offsetof(بنية struct_ext2_filsys, x), \
					cur_جابه_جایی)

ساكن صحيح do_فیلد(ثابت حرف *فیلد, size_t اندازه, صحيح جابه_جایی, صحيح cur_جابه_جایی)
{
	إذا (جابه_جایی != cur_جابه_جایی) {
		چاپ_کن("\t(padding %d bytes?)\n", جابه_جایی - cur_جابه_جایی);
	}
	چاپ_کن("%8d %-30s %3u\n", جابه_جایی, فیلد, (بیعلامت) اندازه);
	أرجع جابه_جایی + اندازه;
}

صحيح اصلی(صحيح argc, حرف **argv)
{
#إذا (__GNUC__ >= 4)
	صحيح cur_جابه_جایی = 0;

	چاپ_کن("%8s %-30s %3s\n", "offset", "field", "size");
	check_فیلد(magic);
	check_فیلد(io);
	check_فیلد(flags);
	check_فیلد(device_name);
	check_فیلد(super);
	check_فیلد(blocksize);
	check_فیلد(fragsize);
	check_فیلد(گروه_desc_شمار);
	check_فیلد(desc_بلوکها);
	check_فیلد(گروه_desc);
	check_فیلد(inode_بلوکها_در_هر_گروه);
	check_فیلد(inode_map);
	check_فیلد(بلوک_map);
	check_فیلد(get_بلوکها);
	check_فیلد(check_directory);
	check_فیلد(write_bitmaps);
	check_فیلد(read_inode);
	check_فیلد(write_inode);
	check_فیلد(badblocks);
	check_فیلد(dblist);
	check_فیلد(stride);
	check_فیلد(orig_super);
	check_فیلد(image_سرآیند);
	check_فیلد(umask);
	check_فیلد(now);
	check_فیلد(cluster_نسبت_بیتها);
	check_فیلد(reserved);
	check_فیلد(priv_داده);
	check_فیلد(icache);
	check_فیلد(image_io);
	check_فیلد(get_alloc_بلوک);
	check_فیلد(بلوک_alloc_stats);
	check_فیلد(mmp_buf);
	check_فیلد(mmp_cmp);
	check_فیلد(mmp_fd);
	check_فیلد(mmp_آخرین_written);
	چاپ_کن("Ending offset is %d\n\n", cur_جابه_جایی);
#پایان_اگر
	خروج(0);
}
