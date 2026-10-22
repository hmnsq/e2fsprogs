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

#شامل "config.h"
#شامل <stdio.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"
#شامل "hashmap.h"

خالی ext2fs_free(ext2_filsys fs)
{
	اگر (!fs || (fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS))
		واپس;
	اگر (fs->image_io != fs->io) {
		اگر (fs->image_io)
			io_channel_close(fs->image_io);
	}
	اگر (fs->io) {
		io_channel_close(fs->io);
	}
	اگر (fs->device_name)
		ext2fs_free_mem(&fs->device_name);
	اگر (fs->super)
		ext2fs_free_mem(&fs->super);
	اگر (fs->orig_super)
		ext2fs_free_mem(&fs->orig_super);
	اگر (fs->گروہ_desc)
		ext2fs_free_mem(&fs->گروہ_desc);
	اگر (fs->بلاک_map)
		ext2fs_free_بلاک_bitmap(fs->بلاک_map);
	اگر (fs->inode_map)
		ext2fs_free_inode_bitmap(fs->inode_map);
	اگر (fs->image_سرخی)
		ext2fs_free_mem(&fs->image_سرخی);

	اگر (fs->badblocks)
		ext2fs_badblocks_list_free(fs->badblocks);
	fs->badblocks = 0;

	اگر (fs->dblist)
		ext2fs_free_dblist(fs->dblist);

	اگر (fs->icache)
		ext2fs_free_inode_cache(fs->icache);

	اگر (fs->mmp_buf)
		ext2fs_free_mem(&fs->mmp_buf);
	اگر (fs->mmp_cmp)
		ext2fs_free_mem(&fs->mmp_cmp);

	اگر (fs->بلاک_sha_map)
		ext2fs_hashmap_free(fs->بلاک_sha_map);

	fs->magic = 0;

	ext2fs_zero_blocks2(تہی, 0, 0, تہی, تہی);
	ext2fs_free_mem(&fs);
}

/*
 * This procedure frees a badblocks list.
 */
خالی ext2fs_u32_list_free(ext2_u32_list bb)
{
	اگر (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		واپس;

	اگر (bb->list)
		ext2fs_free_mem(&bb->list);
	bb->list = 0;
	ext2fs_free_mem(&bb);
}

خالی ext2fs_badblocks_list_free(ext2_badblocks_list bb)
{
	ext2fs_u32_list_free((ext2_u32_list) bb);
}


/*
 * Free a directory block list
 */
خالی ext2fs_free_dblist(ext2_dblist dblist)
{
	اگر (!dblist || (dblist->magic != EXT2_ET_MAGIC_DBLIST))
		واپس;

	اگر (dblist->list)
		ext2fs_free_mem(&dblist->list);
	dblist->list = 0;
	اگر (dblist->fs && dblist->fs->dblist == dblist)
		dblist->fs->dblist = 0;
	dblist->magic = 0;
	ext2fs_free_mem(&dblist);
}

