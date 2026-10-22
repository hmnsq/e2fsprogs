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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"

ਬਣਤਰ ext2_inode_large inode;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ offsetof
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ offsetof(type, member)  __builtin_offsetof(type, member)
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ check_ਖੇਤਰ(x, s) cur_ਵਿੱਥ = do_ਖੇਤਰ(#x, s, ਆਕਾਰ(inode.x),	       \
					offsetof(ਬਣਤਰ ext2_inode_large, x),  \
					cur_ਵਿੱਥ)

ਸਥਿਰ ਪੂਰਨ do_ਖੇਤਰ(ਅਚਲ ਅੱਖਰ *ਖੇਤਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ, ਬਿਨਾਨਿਸ਼ਾਨੀ cur_ਮਾਪ,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ, ਬਿਨਾਨਿਸ਼ਾਨੀ cur_ਵਿੱਥ)
{
	ਜੇ (ਮਾਪ != cur_ਮਾਪ) {
		ਲਿਖੋ("error: %s size %u should be %u\n",
		       ਖੇਤਰ, cur_ਮਾਪ, ਮਾਪ);
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਜੇ (ਵਿੱਥ != cur_ਵਿੱਥ) {
		ਲਿਖੋ("error: %s offset %u should be %u\n",
		       ਖੇਤਰ, cur_ਵਿੱਥ, ਵਿੱਥ);
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਲਿਖੋ("%8d %-30s %3u\n", ਵਿੱਥ, ਖੇਤਰ, (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਮਾਪ);
	ਵਾਪਸ ਵਿੱਥ + ਮਾਪ;
}

ਪੂਰਨ ਮੁੱਖ(ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
#ਜੇ (__GNUC__ >= 4)
	ਪੂਰਨ cur_ਵਿੱਥ = 0;

	ਲਿਖੋ("%8s %-30s %3s\n", "offset", "field", "size");
	check_ਖੇਤਰ(i_ਬਹੁਲਕ, 2);
	check_ਖੇਤਰ(i_uid, 2);
	check_ਖੇਤਰ(i_ਮਾਪ, 4);
	check_ਖੇਤਰ(i_atime, 4);
	check_ਖੇਤਰ(i_ctime, 4);
	check_ਖੇਤਰ(i_mtime, 4);
	check_ਖੇਤਰ(i_dtime, 4);
	check_ਖੇਤਰ(i_gid, 2);
	check_ਖੇਤਰ(i_links_ਗਿਣਤੀ, 2);
	check_ਖੇਤਰ(i_ਖੰਡਾਂ, 4);
	check_ਖੇਤਰ(i_flags, 4);
	check_ਖੇਤਰ(osd1.linux1.l_i_ਸੰਸਕਰਣ, 4);
	check_ਖੇਤਰ(i_ਖੰਡ, 15 * 4);
	check_ਖੇਤਰ(i_generation, 4);
	check_ਖੇਤਰ(i_file_acl, 4);
	check_ਖੇਤਰ(i_ਮਾਪ_ਉੱਚਾ, 4);
	check_ਖੇਤਰ(i_faddr, 4);
	check_ਖੇਤਰ(osd2.linux2.l_i_ਖੰਡਾਂ_hi, 2);
	check_ਖੇਤਰ(osd2.linux2.l_i_file_acl_ਉੱਚਾ, 2);
	check_ਖੇਤਰ(osd2.linux2.l_i_uid_ਉੱਚਾ, 2);
	check_ਖੇਤਰ(osd2.linux2.l_i_gid_ਉੱਚਾ, 2);
	check_ਖੇਤਰ(osd2.linux2.l_i_checksum_lo, 2);
	check_ਖੇਤਰ(osd2.linux2.l_i_reserved, 2);
	do_ਖੇਤਰ("Small inode end", 0, 0, cur_ਵਿੱਥ, 128);
	check_ਖੇਤਰ(i_extra_isize, 2);
	check_ਖੇਤਰ(i_checksum_hi, 2);
	check_ਖੇਤਰ(i_ctime_extra, 4);
	check_ਖੇਤਰ(i_mtime_extra, 4);
	check_ਖੇਤਰ(i_atime_extra, 4);
	check_ਖੇਤਰ(i_crtime, 4);
	check_ਖੇਤਰ(i_crtime_extra, 4);
	check_ਖੇਤਰ(i_ਸੰਸਕਰਣ_hi, 4);
	check_ਖੇਤਰ(i_projid, 4);
	/* This size will change as new fields are added */
	do_ਖੇਤਰ("Large inode end", 0, 0, cur_ਵਿੱਥ, ਆਕਾਰ(inode));
#ਜੇ_ਅੰਤ
	ਵਾਪਸ 0;
}
