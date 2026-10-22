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
#ប្រសិនបើ HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"
#include "hashmap.h"

void ext2fs_free(ext2_filsys fs)
{
	ប្រសិនបើ (!fs || (fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS))
		ត្រឡប់;
	ប្រសិនបើ (fs->image_io != fs->io) {
		ប្រសិនបើ (fs->image_io)
			io_channel_close(fs->image_io);
	}
	ប្រសិនបើ (fs->io) {
		io_channel_close(fs->io);
	}
	ប្រសិនបើ (fs->device_name)
		ext2fs_free_mem(&fs->device_name);
	ប្រសិនបើ (fs->super)
		ext2fs_free_mem(&fs->super);
	ប្រសិនបើ (fs->orig_super)
		ext2fs_free_mem(&fs->orig_super);
	ប្រសិនបើ (fs->group_desc)
		ext2fs_free_mem(&fs->group_desc);
	ប្រសិនបើ (fs->block_map)
		ext2fs_free_block_bitmap(fs->block_map);
	ប្រសិនបើ (fs->inode_map)
		ext2fs_free_inode_bitmap(fs->inode_map);
	ប្រសិនបើ (fs->image_header)
		ext2fs_free_mem(&fs->image_header);

	ប្រសិនបើ (fs->badblocks)
		ext2fs_badblocks_list_free(fs->badblocks);
	fs->badblocks = 0;

	ប្រសិនបើ (fs->dblist)
		ext2fs_free_dblist(fs->dblist);

	ប្រសិនបើ (fs->icache)
		ext2fs_free_inode_cache(fs->icache);

	ប្រសិនបើ (fs->mmp_buf)
		ext2fs_free_mem(&fs->mmp_buf);
	ប្រសិនបើ (fs->mmp_cmp)
		ext2fs_free_mem(&fs->mmp_cmp);

	ប្រសិនបើ (fs->block_sha_map)
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
	ប្រសិនបើ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ត្រឡប់;

	ប្រសិនបើ (bb->list)
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
	ប្រសិនបើ (!dblist || (dblist->magic != EXT2_ET_MAGIC_DBLIST))
		ត្រឡប់;

	ប្រសិនបើ (dblist->list)
		ext2fs_free_mem(&dblist->list);
	dblist->list = 0;
	ប្រសិនបើ (dblist->fs && dblist->fs->dblist == dblist)
		dblist->fs->dblist = 0;
	dblist->magic = 0;
	ext2fs_free_mem(&dblist);
}

