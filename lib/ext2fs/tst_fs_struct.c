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

#جيڪڏهن_وصف_نه_ٿيل offsetof
#وصف_ڪريو offsetof(type, member)  __builtin_offsetof (type, member)
#جيڪڏهن_پورو
#وصف_ڪريو check_ميدان(x) cur_وٿي = do_ميدان(#x, حجم(fs.x),		\
					offsetof(بنية struct_ext2_filsys, x), \
					cur_وٿي)

ساكن صحيح do_ميدان(ثابت حرف *ميدان, size_t سائيز, صحيح وٿي, صحيح cur_وٿي)
{
	إذا (وٿي != cur_وٿي) {
		لکو("\t(padding %d bytes?)\n", وٿي - cur_وٿي);
	}
	لکو("%8d %-30s %3u\n", وٿي, ميدان, (بنانشان) سائيز);
	أرجع وٿي + سائيز;
}

صحيح مکيه(صحيح argc, حرف **argv)
{
#إذا (__GNUC__ >= 4)
	صحيح cur_وٿي = 0;

	لکو("%8s %-30s %3s\n", "offset", "field", "size");
	check_ميدان(magic);
	check_ميدان(io);
	check_ميدان(flags);
	check_ميدان(device_name);
	check_ميدان(super);
	check_ميدان(blocksize);
	check_ميدان(fragsize);
	check_ميدان(گروهه_desc_ڳڻپ);
	check_ميدان(desc_بلاڪون);
	check_ميدان(گروهه_desc);
	check_ميدان(inode_بلاڪون_في_گروهه);
	check_ميدان(inode_map);
	check_ميدان(بلاڪ_map);
	check_ميدان(get_بلاڪون);
	check_ميدان(check_directory);
	check_ميدان(write_bitmaps);
	check_ميدان(read_inode);
	check_ميدان(write_inode);
	check_ميدان(badblocks);
	check_ميدان(dblist);
	check_ميدان(stride);
	check_ميدان(orig_super);
	check_ميدان(image_سرو);
	check_ميدان(umask);
	check_ميدان(now);
	check_ميدان(cluster_تناسب_بٽ);
	check_ميدان(reserved);
	check_ميدان(priv_معلومات);
	check_ميدان(icache);
	check_ميدان(image_io);
	check_ميدان(get_alloc_بلاڪ);
	check_ميدان(بلاڪ_alloc_stats);
	check_ميدان(mmp_buf);
	check_ميدان(mmp_cmp);
	check_ميدان(mmp_fd);
	check_ميدان(mmp_آخري_written);
	لکو("Ending offset is %d\n\n", cur_وٿي);
#جيڪڏهن_پورو
	ٻاهر_نڪرو(0);
}
