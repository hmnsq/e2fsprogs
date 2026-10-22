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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <stdlib.h>

#अन्तर्भाव "ext2_fs.h"

संरचना ext2_inode_large inode;

#ifndef दुरत्व
#define दुरत्व(type, member)  __builtin_offsetof(type, member)
#endif

#define check_field(x, s) cur_offset = do_field(#x, s, आकार(inode.x),	       \
					दुरत्व(संरचना ext2_inode_large, x),  \
					cur_offset)

स्थिर पूर्णांक do_field(स्थिरांक अक्षर *field, अचिन्हित size, अचिन्हित cur_size,
		    अचिन्हित offset, अचिन्हित cur_offset)
{
	जर (size != cur_size) {
		म_लिखो("error: %s size %u should be %u\n",
		       field, cur_size, size);
		निकास(1);
	}
	जर (offset != cur_offset) {
		म_लिखो("error: %s offset %u should be %u\n",
		       field, cur_offset, offset);
		निकास(1);
	}
	म_लिखो("%8d %-30s %3u\n", offset, field, (अचिन्हित) size);
	परत offset + size;
}

पूर्णांक main(पूर्णांक argc, अक्षर **argv)
{
#जर (__GNUC__ >= 4)
	पूर्णांक cur_offset = 0;

	म_लिखो("%8s %-30s %3s\n", "offset", "field", "size");
	check_field(i_mode, 2);
	check_field(i_uid, 2);
	check_field(i_size, 4);
	check_field(i_atime, 4);
	check_field(i_ctime, 4);
	check_field(i_mtime, 4);
	check_field(i_dtime, 4);
	check_field(i_gid, 2);
	check_field(i_links_मोजणी, 2);
	check_field(i_blocks, 4);
	check_field(i_flags, 4);
	check_field(osd1.linux1.l_i_version, 4);
	check_field(i_block, 15 * 4);
	check_field(i_generation, 4);
	check_field(i_file_acl, 4);
	check_field(i_size_वरचा, 4);
	check_field(i_faddr, 4);
	check_field(osd2.linux2.l_i_blocks_hi, 2);
	check_field(osd2.linux2.l_i_file_acl_वरचा, 2);
	check_field(osd2.linux2.l_i_uid_वरचा, 2);
	check_field(osd2.linux2.l_i_gid_वरचा, 2);
	check_field(osd2.linux2.l_i_checksum_lo, 2);
	check_field(osd2.linux2.l_i_reserved, 2);
	do_field("Small inode end", 0, 0, cur_offset, 128);
	check_field(i_extra_isize, 2);
	check_field(i_checksum_hi, 2);
	check_field(i_ctime_extra, 4);
	check_field(i_mtime_extra, 4);
	check_field(i_atime_extra, 4);
	check_field(i_crtime, 4);
	check_field(i_crtime_extra, 4);
	check_field(i_version_hi, 4);
	check_field(i_projid, 4);
	/* This size will change as new fields are added */
	do_field("Large inode end", 0, 0, cur_offset, आकार(inode));
#endif
	परत 0;
}
