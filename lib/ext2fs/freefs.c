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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fsP.h"
#અન્તર્ભાવ "hashmap.h"

શૂન્ય ext2fs_free(ext2_filsys fs)
{
	જો (!fs || (fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS))
		પાછા;
	જો (fs->image_io != fs->io) {
		જો (fs->image_io)
			io_channel_close(fs->image_io);
	}
	જો (fs->io) {
		io_channel_close(fs->io);
	}
	જો (fs->device_name)
		ext2fs_free_mem(&fs->device_name);
	જો (fs->super)
		ext2fs_free_mem(&fs->super);
	જો (fs->orig_super)
		ext2fs_free_mem(&fs->orig_super);
	જો (fs->group_desc)
		ext2fs_free_mem(&fs->group_desc);
	જો (fs->block_map)
		ext2fs_free_block_bitmap(fs->block_map);
	જો (fs->inode_map)
		ext2fs_free_inode_bitmap(fs->inode_map);
	જો (fs->image_header)
		ext2fs_free_mem(&fs->image_header);

	જો (fs->badblocks)
		ext2fs_badblocks_list_free(fs->badblocks);
	fs->badblocks = 0;

	જો (fs->dblist)
		ext2fs_free_dblist(fs->dblist);

	જો (fs->icache)
		ext2fs_free_inode_cache(fs->icache);

	જો (fs->mmp_buf)
		ext2fs_free_mem(&fs->mmp_buf);
	જો (fs->mmp_cmp)
		ext2fs_free_mem(&fs->mmp_cmp);

	જો (fs->block_sha_map)
		ext2fs_hashmap_free(fs->block_sha_map);

	fs->magic = 0;

	ext2fs_zero_blocks2(ખાલી, 0, 0, ખાલી, ખાલી);
	ext2fs_free_mem(&fs);
}

/*
 * This procedure frees a badblocks list.
 */
શૂન્ય ext2fs_u32_list_free(ext2_u32_list bb)
{
	જો (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		પાછા;

	જો (bb->list)
		ext2fs_free_mem(&bb->list);
	bb->list = 0;
	ext2fs_free_mem(&bb);
}

શૂન્ય ext2fs_badblocks_list_free(ext2_badblocks_list bb)
{
	ext2fs_u32_list_free((ext2_u32_list) bb);
}


/*
 * Free a directory block list
 */
શૂન્ય ext2fs_free_dblist(ext2_dblist dblist)
{
	જો (!dblist || (dblist->magic != EXT2_ET_MAGIC_DBLIST))
		પાછા;

	જો (dblist->list)
		ext2fs_free_mem(&dblist->list);
	dblist->list = 0;
	જો (dblist->fs && dblist->fs->dblist == dblist)
		dblist->fs->dblist = 0;
	dblist->magic = 0;
	ext2fs_free_mem(&dblist);
}

