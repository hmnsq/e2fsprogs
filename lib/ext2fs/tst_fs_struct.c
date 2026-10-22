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

#समावेश <stdio.h>
#समावेश <unistd.h>
#समावेश <stdlib.h>

#समावेश "ext2fs.h"

काष्ठा struct_ext2_filsys fs;

#अगर_अघोषित दुरत्व
#घोषणा दुरत्व(type, member)  __builtin_offsetof (type, member)
#पूर्ण_अगर
#घोषणा check_field(x) cur_offset = do_field(#x, माप(fs.x),		\
					दुरत्व(काष्ठा struct_ext2_filsys, x), \
					cur_offset)

अटल पूर्णांक do_field(स्थिर अक्षर *field, माप_प्रकार आकार, पूर्णांक offset, पूर्णांक cur_offset)
{
	अगर (offset != cur_offset) {
		म_लिखो("\t(padding %d bytes?)\n", offset - cur_offset);
	}
	म_लिखो("%8d %-30s %3u\n", offset, field, (अचिन्हित) आकार);
	वापस offset + आकार;
}

पूर्णांक मुख्य(पूर्णांक argc, अक्षर **argv)
{
#अगर (__GNUC__ >= 4)
	पूर्णांक cur_offset = 0;

	म_लिखो("%8s %-30s %3s\n", "offset", "field", "size");
	check_field(magic);
	check_field(io);
	check_field(flags);
	check_field(device_name);
	check_field(super);
	check_field(blocksize);
	check_field(fragsize);
	check_field(group_desc_गिनती);
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
	check_field(priv_तथ्य);
	check_field(icache);
	check_field(image_io);
	check_field(get_alloc_block);
	check_field(block_alloc_stats);
	check_field(mmp_buf);
	check_field(mmp_cmp);
	check_field(mmp_fd);
	check_field(mmp_last_written);
	म_लिखो("Ending offset is %d\n\n", cur_offset);
#पूर्ण_अगर
	निकास(0);
}
