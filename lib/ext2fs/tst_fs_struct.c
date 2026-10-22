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

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <stdlib.h>

#अन्तर्भाव "ext2fs.h"

संरचना struct_ext2_filsys fs;

#ifndef दुरत्व
#define दुरत्व(type, member)  __builtin_offsetof (type, member)
#endif
#define check_field(x) cur_offset = do_field(#x, आकार(fs.x),		\
					दुरत्व(संरचना struct_ext2_filsys, x), \
					cur_offset)

स्थिर पूर्णांक do_field(स्थिरांक अक्षर *field, माप_प्रकार size, पूर्णांक offset, पूर्णांक cur_offset)
{
	जर (offset != cur_offset) {
		म_लिखो("\t(padding %d bytes?)\n", offset - cur_offset);
	}
	म_लिखो("%8d %-30s %3u\n", offset, field, (अचिन्हित) size);
	परत offset + size;
}

पूर्णांक main(पूर्णांक argc, अक्षर **argv)
{
#जर (__GNUC__ >= 4)
	पूर्णांक cur_offset = 0;

	म_लिखो("%8s %-30s %3s\n", "offset", "field", "size");
	check_field(magic);
	check_field(io);
	check_field(flags);
	check_field(device_name);
	check_field(super);
	check_field(blocksize);
	check_field(fragsize);
	check_field(group_desc_मोजणी);
	check_field(desc_blocks);
	check_field(group_desc);
	check_field(inode_blocks_per_group);
	check_field(inode_map);
	check_field(block_map);
	check_field(get_blocks);
	check_field(check_directory);
	check_field(write_bitmaps);
	check_field(read_inode);
	check_field(write_inode);
	check_field(badblocks);
	check_field(dblist);
	check_field(stride);
	check_field(orig_super);
	check_field(image_header);
	check_field(umask);
	check_field(now);
	check_field(cluster_ratio_bits);
	check_field(reserved);
	check_field(priv_data);
	check_field(icache);
	check_field(image_io);
	check_field(get_alloc_block);
	check_field(block_alloc_stats);
	check_field(mmp_buf);
	check_field(mmp_cmp);
	check_field(mmp_fd);
	check_field(mmp_last_written);
	म_लिखो("Ending offset is %d\n\n", cur_offset);
#endif
	निकास(0);
}
