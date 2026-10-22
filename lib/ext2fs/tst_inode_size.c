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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <unistd.h>
#অন্তর্ভাৱ <stdlib.h>

#অন্তর্ভাৱ "ext2_fs.h"

সংরচনা ext2_inode_large inode;

#ifndef offsetof
#define offsetof(type, member)  __builtin_offsetof(type, member)
#endif

#define check_field(x, s) cur_offset = do_field(#x, s, পরিমাণ(inode.x),	       \
					offsetof(সংরচনা ext2_inode_large, x),  \
					cur_offset)

স্থির পূর্ণাঙ্ক do_field(ধ্রুৱ ৱর্ণ *field, unsigned size, unsigned cur_size,
		    unsigned offset, unsigned cur_offset)
{
	যদি (size != cur_size) {
		printf("error: %s size %u should be %u\n",
		       field, cur_size, size);
		exit(1);
	}
	যদি (offset != cur_offset) {
		printf("error: %s offset %u should be %u\n",
		       field, cur_offset, offset);
		exit(1);
	}
	printf("%8d %-30s %3u\n", offset, field, (unsigned) size);
	ঘূৰাই offset + size;
}

পূর্ণাঙ্ক main(পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
#যদি (__GNUC__ >= 4)
	পূর্ণাঙ্ক cur_offset = 0;

	printf("%8s %-30s %3s\n", "offset", "field", "size");
	check_field(i_mode, 2);
	check_field(i_uid, 2);
	check_field(i_size, 4);
	check_field(i_atime, 4);
	check_field(i_ctime, 4);
	check_field(i_mtime, 4);
	check_field(i_dtime, 4);
	check_field(i_gid, 2);
	check_field(i_links_গুনতি, 2);
	check_field(i_blocks, 4);
	check_field(i_flags, 4);
	check_field(osd1.linux1.l_i_version, 4);
	check_field(i_block, 15 * 4);
	check_field(i_generation, 4);
	check_field(i_file_acl, 4);
	check_field(i_size_উপৰেৰ, 4);
	check_field(i_faddr, 4);
	check_field(osd2.linux2.l_i_blocks_hi, 2);
	check_field(osd2.linux2.l_i_file_acl_উপৰেৰ, 2);
	check_field(osd2.linux2.l_i_uid_উপৰেৰ, 2);
	check_field(osd2.linux2.l_i_gid_উপৰেৰ, 2);
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
	do_field("Large inode end", 0, 0, cur_offset, পরিমাণ(inode));
#endif
	ঘূৰাই 0;
}
