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

#ضمن "config.h"
#ضمن <stdio.h>
#אם HAVE_UNISTD_H
#ضمن <unistd.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"
#ضمن "hashmap.h"

فراغ ext2fs_free(ext2_filsys fs)
{
	אם (!fs || (fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS))
		החזר;
	אם (fs->image_io != fs->io) {
		אם (fs->image_io)
			io_channel_close(fs->image_io);
	}
	אם (fs->io) {
		io_channel_close(fs->io);
	}
	אם (fs->device_name)
		ext2fs_free_mem(&fs->device_name);
	אם (fs->super)
		ext2fs_free_mem(&fs->super);
	אם (fs->orig_super)
		ext2fs_free_mem(&fs->orig_super);
	אם (fs->group_desc)
		ext2fs_free_mem(&fs->group_desc);
	אם (fs->block_map)
		ext2fs_free_block_bitmap(fs->block_map);
	אם (fs->inode_map)
		ext2fs_free_inode_bitmap(fs->inode_map);
	אם (fs->image_header)
		ext2fs_free_mem(&fs->image_header);

	אם (fs->badblocks)
		ext2fs_badblocks_list_free(fs->badblocks);
	fs->badblocks = 0;

	אם (fs->dblist)
		ext2fs_free_dblist(fs->dblist);

	אם (fs->icache)
		ext2fs_free_inode_cache(fs->icache);

	אם (fs->mmp_buf)
		ext2fs_free_mem(&fs->mmp_buf);
	אם (fs->mmp_cmp)
		ext2fs_free_mem(&fs->mmp_cmp);

	אם (fs->block_sha_map)
		ext2fs_hashmap_free(fs->block_sha_map);

	fs->magic = 0;

	ext2fs_zero_blocks2(عدم, 0, 0, عدم, عدم);
	ext2fs_free_mem(&fs);
}

/*
 * This procedure frees a badblocks list.
 */
فراغ ext2fs_u32_list_free(ext2_u32_list bb)
{
	אם (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		החזר;

	אם (bb->list)
		ext2fs_free_mem(&bb->list);
	bb->list = 0;
	ext2fs_free_mem(&bb);
}

فراغ ext2fs_badblocks_list_free(ext2_badblocks_list bb)
{
	ext2fs_u32_list_free((ext2_u32_list) bb);
}


/*
 * Free a directory block list
 */
فراغ ext2fs_free_dblist(ext2_dblist dblist)
{
	אם (!dblist || (dblist->magic != EXT2_ET_MAGIC_DBLIST))
		החזר;

	אם (dblist->list)
		ext2fs_free_mem(&dblist->list);
	dblist->list = 0;
	אם (dblist->fs && dblist->fs->dblist == dblist)
		dblist->fs->dblist = 0;
	dblist->magic = 0;
	ext2fs_free_mem(&dblist);
}

