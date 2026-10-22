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

#include "config.h"
#include <stdio.h>
#ако HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"
#include "hashmap.h"

void ext2fs_free(ext2_filsys fs)
{
	ако (!fs || (fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS))
		върни;
	ако (fs->image_io != fs->io) {
		ако (fs->image_io)
			io_channel_close(fs->image_io);
	}
	ако (fs->io) {
		io_channel_close(fs->io);
	}
	ако (fs->device_name)
		ext2fs_free_mem(&fs->device_name);
	ако (fs->super)
		ext2fs_free_mem(&fs->super);
	ако (fs->orig_super)
		ext2fs_free_mem(&fs->orig_super);
	ако (fs->group_desc)
		ext2fs_free_mem(&fs->group_desc);
	ако (fs->block_map)
		ext2fs_free_block_bitmap(fs->block_map);
	ако (fs->inode_map)
		ext2fs_free_inode_bitmap(fs->inode_map);
	ако (fs->image_header)
		ext2fs_free_mem(&fs->image_header);

	ако (fs->badblocks)
		ext2fs_badblocks_list_free(fs->badblocks);
	fs->badblocks = 0;

	ако (fs->dblist)
		ext2fs_free_dblist(fs->dblist);

	ако (fs->icache)
		ext2fs_free_inode_cache(fs->icache);

	ако (fs->mmp_buf)
		ext2fs_free_mem(&fs->mmp_buf);
	ако (fs->mmp_cmp)
		ext2fs_free_mem(&fs->mmp_cmp);

	ако (fs->block_sha_map)
		ext2fs_hashmap_free(fs->block_sha_map);

	fs->magic = 0;

	ext2fs_zero_blocks2(NULL, 0, 0, NULL, NULL);
	ext2fs_free_mem(&fs);
}

/*
 * This procedure frees a badblocks list.
 */
void ext2fs_u32_list_free(ext2_u32_list bb)
{
	ако (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		върни;

	ако (bb->list)
		ext2fs_free_mem(&bb->list);
	bb->list = 0;
	ext2fs_free_mem(&bb);
}

void ext2fs_badblocks_list_free(ext2_badblocks_list bb)
{
	ext2fs_u32_list_free((ext2_u32_list) bb);
}


/*
 * Free a directory block list
 */
void ext2fs_free_dblist(ext2_dblist dblist)
{
	ако (!dblist || (dblist->magic != EXT2_ET_MAGIC_DBLIST))
		върни;

	ако (dblist->list)
		ext2fs_free_mem(&dblist->list);
	dblist->list = 0;
	ако (dblist->fs && dblist->fs->dblist == dblist)
		dblist->fs->dblist = 0;
	dblist->magic = 0;
	ext2fs_free_mem(&dblist);
}

