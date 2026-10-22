/*
 * freefs.c --- free an ext2 filesystem
 *
 * Copyright (C) 1993, 1994, 1995, 1996 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hashmap.h"

ਖਾਲੀ ext2fs_free(ext2_filsys fs)
{
	ਜੇ (!fs || (fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS))
		ਵਾਪਸ;
	ਜੇ (fs->image_io != fs->io) {
		ਜੇ (fs->image_io)
			io_channel_close(fs->image_io);
	}
	ਜੇ (fs->io) {
		io_channel_close(fs->io);
	}
	ਜੇ (fs->device_name)
		ext2fs_free_mem(&fs->device_name);
	ਜੇ (fs->super)
		ext2fs_free_mem(&fs->super);
	ਜੇ (fs->orig_super)
		ext2fs_free_mem(&fs->orig_super);
	ਜੇ (fs->ਸਮੂਹ_desc)
		ext2fs_free_mem(&fs->ਸਮੂਹ_desc);
	ਜੇ (fs->ਖੰਡ_map)
		ext2fs_free_ਖੰਡ_bitmap(fs->ਖੰਡ_map);
	ਜੇ (fs->inode_map)
		ext2fs_free_inode_bitmap(fs->inode_map);
	ਜੇ (fs->image_ਸਿਰਲੇਖ)
		ext2fs_free_mem(&fs->image_ਸਿਰਲੇਖ);

	ਜੇ (fs->badblocks)
		ext2fs_badblocks_list_free(fs->badblocks);
	fs->badblocks = 0;

	ਜੇ (fs->dblist)
		ext2fs_free_dblist(fs->dblist);

	ਜੇ (fs->icache)
		ext2fs_free_inode_cache(fs->icache);

	ਜੇ (fs->mmp_buf)
		ext2fs_free_mem(&fs->mmp_buf);
	ਜੇ (fs->mmp_cmp)
		ext2fs_free_mem(&fs->mmp_cmp);

	ਜੇ (fs->ਖੰਡ_sha_map)
		ext2fs_hashmap_free(fs->ਖੰਡ_sha_map);

	fs->magic = 0;

	ext2fs_zero_blocks2(ਨਿਰਾ, 0, 0, ਨਿਰਾ, ਨਿਰਾ);
	ext2fs_free_mem(&fs);
}

/*
 * This procedure frees a badblocks list.
 */
ਖਾਲੀ ext2fs_u32_list_free(ext2_u32_list bb)
{
	ਜੇ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ਵਾਪਸ;

	ਜੇ (bb->list)
		ext2fs_free_mem(&bb->list);
	bb->list = 0;
	ext2fs_free_mem(&bb);
}

ਖਾਲੀ ext2fs_badblocks_list_free(ext2_badblocks_list bb)
{
	ext2fs_u32_list_free((ext2_u32_list) bb);
}


/*
 * Free a directory block list
 */
ਖਾਲੀ ext2fs_free_dblist(ext2_dblist dblist)
{
	ਜੇ (!dblist || (dblist->magic != EXT2_ET_MAGIC_DBLIST))
		ਵਾਪਸ;

	ਜੇ (dblist->list)
		ext2fs_free_mem(&dblist->list);
	dblist->list = 0;
	ਜੇ (dblist->fs && dblist->fs->dblist == dblist)
		dblist->fs->dblist = 0;
	dblist->magic = 0;
	ext2fs_free_mem(&dblist);
}

