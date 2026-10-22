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

#جے_بنا_تعریف offsetof
#تعریف_کرو offsetof(type, member)  __builtin_offsetof (type, member)
#جے_انت
#تعریف_کرو check_میدان(x) cur_وتھ = do_میدان(#x, حجم(fs.x),		\
					offsetof(بنية struct_ext2_filsys, x), \
					cur_وتھ)

ساكن صحيح do_میدان(ثابت حرف *میدان, size_t ناپ, صحيح وتھ, صحيح cur_وتھ)
{
	إذا (وتھ != cur_وتھ) {
		لکھو("\t(padding %d bytes?)\n", وتھ - cur_وتھ);
	}
	لکھو("%8d %-30s %3u\n", وتھ, میدان, (بنانشانی) ناپ);
	أرجع وتھ + ناپ;
}

صحيح مکھ(صحيح argc, حرف **argv)
{
#إذا (__GNUC__ >= 4)
	صحيح cur_وتھ = 0;

	لکھو("%8s %-30s %3s\n", "offset", "field", "size");
	check_میدان(magic);
	check_میدان(io);
	check_میدان(flags);
	check_میدان(device_name);
	check_میدان(super);
	check_میدان(blocksize);
	check_میدان(fragsize);
	check_میدان(گروہ_desc_گنتی);
	check_میدان(desc_بلاکاں);
	check_میدان(گروہ_desc);
	check_میدان(inode_بلاکاں_فی_گروہ);
	check_میدان(inode_map);
	check_میدان(بلاک_map);
	check_میدان(get_بلاکاں);
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
	check_میدان(priv_انکڑے);
	check_میدان(icache);
	check_میدان(image_io);
	check_میدان(get_alloc_بلاک);
	check_میدان(بلاک_alloc_stats);
	check_میدان(mmp_buf);
	check_میدان(mmp_cmp);
	check_میدان(mmp_fd);
	check_میدان(mmp_آخری_written);
	لکھو("Ending offset is %d\n\n", cur_وتھ);
#جے_انت
	باہر_نکلو(0);
}
