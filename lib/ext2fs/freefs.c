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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fsP.h"
#অন্তর্ভাৱ "hashmap.h"

শূন্য ext2fs_free(ext2_filsys fs)
{
	যদি (!fs || (fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS))
		ঘূৰাই;
	যদি (fs->image_io != fs->io) {
		যদি (fs->image_io)
			io_channel_close(fs->image_io);
	}
	যদি (fs->io) {
		io_channel_close(fs->io);
	}
	যদি (fs->device_name)
		ext2fs_free_mem(&fs->device_name);
	যদি (fs->super)
		ext2fs_free_mem(&fs->super);
	যদি (fs->orig_super)
		ext2fs_free_mem(&fs->orig_super);
	যদি (fs->group_desc)
		ext2fs_free_mem(&fs->group_desc);
	যদি (fs->block_map)
		ext2fs_free_block_bitmap(fs->block_map);
	যদি (fs->inode_map)
		ext2fs_free_inode_bitmap(fs->inode_map);
	যদি (fs->image_header)
		ext2fs_free_mem(&fs->image_header);

	যদি (fs->badblocks)
		ext2fs_badblocks_list_free(fs->badblocks);
	fs->badblocks = 0;

	যদি (fs->dblist)
		ext2fs_free_dblist(fs->dblist);

	যদি (fs->icache)
		ext2fs_free_inode_cache(fs->icache);

	যদি (fs->mmp_buf)
		ext2fs_free_mem(&fs->mmp_buf);
	যদি (fs->mmp_cmp)
		ext2fs_free_mem(&fs->mmp_cmp);

	যদি (fs->block_sha_map)
		ext2fs_hashmap_free(fs->block_sha_map);

	fs->magic = 0;

	ext2fs_zero_blocks2(রিক্ত, 0, 0, রিক্ত, রিক্ত);
	ext2fs_free_mem(&fs);
}

/*
 * This procedure frees a badblocks list.
 */
শূন্য ext2fs_u32_list_free(ext2_u32_list bb)
{
	যদি (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ঘূৰাই;

	যদি (bb->list)
		ext2fs_free_mem(&bb->list);
	bb->list = 0;
	ext2fs_free_mem(&bb);
}

শূন্য ext2fs_badblocks_list_free(ext2_badblocks_list bb)
{
	ext2fs_u32_list_free((ext2_u32_list) bb);
}


/*
 * Free a directory block list
 */
শূন্য ext2fs_free_dblist(ext2_dblist dblist)
{
	যদি (!dblist || (dblist->magic != EXT2_ET_MAGIC_DBLIST))
		ঘূৰাই;

	যদি (dblist->list)
		ext2fs_free_mem(&dblist->list);
	dblist->list = 0;
	যদি (dblist->fs && dblist->fs->dblist == dblist)
		dblist->fs->dblist = 0;
	dblist->magic = 0;
	ext2fs_free_mem(&dblist);
}

