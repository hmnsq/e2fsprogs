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

#شامل <stdio.h>
#شامل <unistd.h>
#شامل <stdlib.h>

#شامل "ext2fs.h"

ساخت struct_ext2_filsys fs;

#اگر_غیر_تعریف_شدہ offsetof
#تعریف_کرو offsetof(type, member)  __builtin_offsetof (type, member)
#اگر_ختم
#تعریف_کرو check_میدان(x) cur_انحراف = do_میدان(#x, حجم(fs.x),		\
					offsetof(ساخت struct_ext2_filsys, x), \
					cur_انحراف)

ساکن صحیح do_میدان(ثابت حرف *میدان, size_t سائز, صحیح انحراف, صحیح cur_انحراف)
{
	اگر (انحراف != cur_انحراف) {
		لکھو("\t(padding %d bytes?)\n", انحراف - cur_انحراف);
	}
	لکھو("%8d %-30s %3u\n", انحراف, میدان, (غیرعلامتی) سائز);
	واپس انحراف + سائز;
}

صحیح مین(صحیح argc, حرف **argv)
{
#اگر (__GNUC__ >= 4)
	صحیح cur_انحراف = 0;

	لکھو("%8s %-30s %3s\n", "offset", "field", "size");
	check_میدان(magic);
	check_میدان(io);
	check_میدان(flags);
	check_میدان(device_name);
	check_میدان(super);
	check_میدان(blocksize);
	check_میدان(fragsize);
	check_میدان(گروہ_desc_گنتی);
	check_میدان(desc_بلاکیں);
	check_میدان(گروہ_desc);
	check_میدان(inode_بلاکیں_فی_گروہ);
	check_میدان(inode_map);
	check_میدان(بلاک_map);
	check_میدان(get_بلاکیں);
	check_میدان(check_directory);
	check_میدان(write_bitmaps);
	check_میدان(read_inode);
	check_میدان(write_inode);
	check_میدان(badblocks);
	check_میدان(dblist);
	check_میدان(stride);
	check_میدان(orig_super);
	check_میدان(image_سرخی);
	check_میدان(umask);
	check_میدان(now);
	check_میدان(cluster_تناسب_بٹ);
	check_میدان(reserved);
	check_میدان(priv_ڈیٹا);
	check_میدان(icache);
	check_میدان(image_io);
	check_میدان(get_alloc_بلاک);
	check_میدان(بلاک_alloc_stats);
	check_میدان(mmp_buf);
	check_میدان(mmp_cmp);
	check_میدان(mmp_fd);
	check_میدان(mmp_آخری_written);
	لکھو("Ending offset is %d\n\n", cur_انحراف);
#اگر_ختم
	باہر_نکلو(0);
}
