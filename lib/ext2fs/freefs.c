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
#ຖ້າ HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"
#include "hashmap.h"

void ext2fs_free(ext2_filsys fs)
{
	ຖ້າ (!fs || (fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS))
		ສົ່ງຄືນ;
	ຖ້າ (fs->image_io != fs->io) {
		ຖ້າ (fs->image_io)
			io_channel_close(fs->image_io);
	}
	ຖ້າ (fs->io) {
		io_channel_close(fs->io);
	}
	ຖ້າ (fs->device_name)
		ext2fs_free_mem(&fs->device_name);
	ຖ້າ (fs->super)
		ext2fs_free_mem(&fs->super);
	ຖ້າ (fs->orig_super)
		ext2fs_free_mem(&fs->orig_super);
	ຖ້າ (fs->group_desc)
		ext2fs_free_mem(&fs->group_desc);
	ຖ້າ (fs->block_map)
		ext2fs_free_block_bitmap(fs->block_map);
	ຖ້າ (fs->inode_map)
		ext2fs_free_inode_bitmap(fs->inode_map);
	ຖ້າ (fs->image_header)
		ext2fs_free_mem(&fs->image_header);

	ຖ້າ (fs->badblocks)
		ext2fs_badblocks_list_free(fs->badblocks);
	fs->badblocks = 0;

	ຖ້າ (fs->dblist)
		ext2fs_free_dblist(fs->dblist);

	ຖ້າ (fs->icache)
		ext2fs_free_inode_cache(fs->icache);

	ຖ້າ (fs->mmp_buf)
		ext2fs_free_mem(&fs->mmp_buf);
	ຖ້າ (fs->mmp_cmp)
		ext2fs_free_mem(&fs->mmp_cmp);

	ຖ້າ (fs->block_sha_map)
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
	ຖ້າ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ສົ່ງຄືນ;

	ຖ້າ (bb->list)
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
	ຖ້າ (!dblist || (dblist->magic != EXT2_ET_MAGIC_DBLIST))
		ສົ່ງຄືນ;

	ຖ້າ (dblist->list)
		ext2fs_free_mem(&dblist->list);
	dblist->list = 0;
	ຖ້າ (dblist->fs && dblist->fs->dblist == dblist)
		dblist->fs->dblist = 0;
	dblist->magic = 0;
	ext2fs_free_mem(&dblist);
}

