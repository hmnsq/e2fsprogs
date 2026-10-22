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

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

ਬਣਤਰ struct_ext2_filsys fs;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ offsetof
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ offsetof(type, member)  __builtin_offsetof (type, member)
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ check_ਖੇਤਰ(x) cur_ਵਿੱਥ = do_ਖੇਤਰ(#x, ਆਕਾਰ(fs.x),		\
					offsetof(ਬਣਤਰ struct_ext2_filsys, x), \
					cur_ਵਿੱਥ)

ਸਥਿਰ ਪੂਰਨ do_ਖੇਤਰ(ਅਚਲ ਅੱਖਰ *ਖੇਤਰ, size_t ਮਾਪ, ਪੂਰਨ ਵਿੱਥ, ਪੂਰਨ cur_ਵਿੱਥ)
{
	ਜੇ (ਵਿੱਥ != cur_ਵਿੱਥ) {
		ਲਿਖੋ("\t(padding %d bytes?)\n", ਵਿੱਥ - cur_ਵਿੱਥ);
	}
	ਲਿਖੋ("%8d %-30s %3u\n", ਵਿੱਥ, ਖੇਤਰ, (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਮਾਪ);
	ਵਾਪਸ ਵਿੱਥ + ਮਾਪ;
}

ਪੂਰਨ ਮੁੱਖ(ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
#ਜੇ (__GNUC__ >= 4)
	ਪੂਰਨ cur_ਵਿੱਥ = 0;

	ਲਿਖੋ("%8s %-30s %3s\n", "offset", "field", "size");
	check_ਖੇਤਰ(magic);
	check_ਖੇਤਰ(io);
	check_ਖੇਤਰ(flags);
	check_ਖੇਤਰ(device_name);
	check_ਖੇਤਰ(super);
	check_ਖੇਤਰ(blocksize);
	check_ਖੇਤਰ(fragsize);
	check_ਖੇਤਰ(ਸਮੂਹ_desc_ਗਿਣਤੀ);
	check_ਖੇਤਰ(desc_ਖੰਡਾਂ);
	check_ਖੇਤਰ(ਸਮੂਹ_desc);
	check_ਖੇਤਰ(inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ);
	check_ਖੇਤਰ(inode_map);
	check_ਖੇਤਰ(ਖੰਡ_map);
	check_ਖੇਤਰ(get_ਖੰਡਾਂ);
	check_ਖੇਤਰ(check_directory);
	check_ਖੇਤਰ(write_bitmaps);
	check_ਖੇਤਰ(read_inode);
	check_ਖੇਤਰ(write_inode);
	check_ਖੇਤਰ(badblocks);
	check_ਖੇਤਰ(dblist);
	check_ਖੇਤਰ(stride);
	check_ਖੇਤਰ(orig_super);
	check_ਖੇਤਰ(image_ਸਿਰਲੇਖ);
	check_ਖੇਤਰ(umask);
	check_ਖੇਤਰ(now);
	check_ਖੇਤਰ(cluster_ਅਨੁਪਾਤ_ਬਿੱਟ);
	check_ਖੇਤਰ(reserved);
	check_ਖੇਤਰ(priv_ਅੰਕੜੇ);
	check_ਖੇਤਰ(icache);
	check_ਖੇਤਰ(image_io);
	check_ਖੇਤਰ(get_alloc_ਖੰਡ);
	check_ਖੇਤਰ(ਖੰਡ_alloc_stats);
	check_ਖੇਤਰ(mmp_buf);
	check_ਖੇਤਰ(mmp_cmp);
	check_ਖੇਤਰ(mmp_fd);
	check_ਖੇਤਰ(mmp_ਆਖਰੀ_written);
	ਲਿਖੋ("Ending offset is %d\n\n", cur_ਵਿੱਥ);
#ਜੇ_ਅੰਤ
	ਬਾਹਰ_ਨਿਕਲੋ(0);
}
