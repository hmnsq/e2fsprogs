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

#समावेश "config.h"
#समावेश <stdio.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fsP.h"
#समावेश "hashmap.h"

व्योम ext2fs_free(ext2_filsys fs)
{
	अगर (!fs || (fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS))
		वापस;
	अगर (fs->image_io != fs->io) {
		अगर (fs->image_io)
			io_channel_close(fs->image_io);
	}
	अगर (fs->io) {
		io_channel_close(fs->io);
	}
	अगर (fs->device_name)
		ext2fs_free_mem(&fs->device_name);
	अगर (fs->super)
		ext2fs_free_mem(&fs->super);
	अगर (fs->orig_super)
		ext2fs_free_mem(&fs->orig_super);
	अगर (fs->group_desc)
		ext2fs_free_mem(&fs->group_desc);
	अगर (fs->block_map)
		ext2fs_free_block_bitmap(fs->block_map);
	अगर (fs->inode_map)
		ext2fs_free_inode_bitmap(fs->inode_map);
	अगर (fs->image_header)
		ext2fs_free_mem(&fs->image_header);

	अगर (fs->badblocks)
		ext2fs_badblocks_list_free(fs->badblocks);
	fs->badblocks = 0;

	अगर (fs->dblist)
		ext2fs_free_dblist(fs->dblist);

	अगर (fs->icache)
		ext2fs_free_inode_cache(fs->icache);

	अगर (fs->mmp_buf)
		ext2fs_free_mem(&fs->mmp_buf);
	अगर (fs->mmp_cmp)
		ext2fs_free_mem(&fs->mmp_cmp);

	अगर (fs->block_sha_map)
		ext2fs_hashmap_free(fs->block_sha_map);

	fs->magic = 0;

	ext2fs_zero_blocks2(शून्य, 0, 0, शून्य, शून्य);
	ext2fs_free_mem(&fs);
}

/*
 * This procedure frees a badblocks list.
 */
व्योम ext2fs_u32_list_free(ext2_u32_list bb)
{
	अगर (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		वापस;

	अगर (bb->list)
		ext2fs_free_mem(&bb->list);
	bb->list = 0;
	ext2fs_free_mem(&bb);
}

व्योम ext2fs_badblocks_list_free(ext2_badblocks_list bb)
{
	ext2fs_u32_list_free((ext2_u32_list) bb);
}


/*
 * Free a directory block list
 */
व्योम ext2fs_free_dblist(ext2_dblist dblist)
{
	अगर (!dblist || (dblist->magic != EXT2_ET_MAGIC_DBLIST))
		वापस;

	अगर (dblist->list)
		ext2fs_free_mem(&dblist->list);
	dblist->list = 0;
	अगर (dblist->fs && dblist->fs->dblist == dblist)
		dblist->fs->dblist = 0;
	dblist->magic = 0;
	ext2fs_free_mem(&dblist);
}

