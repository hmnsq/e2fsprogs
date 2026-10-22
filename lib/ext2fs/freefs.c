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
#𐡄𐡍 HAVE_UNISTD_H
#ضمن <unistd.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"
#ضمن "hashmap.h"

فراغ ext2fs_free(ext2_filsys fs)
{
	𐡄𐡍 (!fs || (fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS))
		أرجع;
	𐡄𐡍 (fs->image_io != fs->io) {
		𐡄𐡍 (fs->image_io)
			io_channel_close(fs->image_io);
	}
	𐡄𐡍 (fs->io) {
		io_channel_close(fs->io);
	}
	𐡄𐡍 (fs->device_name)
		ext2fs_free_mem(&fs->device_name);
	𐡄𐡍 (fs->super)
		ext2fs_free_mem(&fs->super);
	𐡄𐡍 (fs->orig_super)
		ext2fs_free_mem(&fs->orig_super);
	𐡄𐡍 (fs->group_desc)
		ext2fs_free_mem(&fs->group_desc);
	𐡄𐡍 (fs->block_map)
		ext2fs_free_block_bitmap(fs->block_map);
	𐡄𐡍 (fs->inode_map)
		ext2fs_free_inode_bitmap(fs->inode_map);
	𐡄𐡍 (fs->image_header)
		ext2fs_free_mem(&fs->image_header);

	𐡄𐡍 (fs->badblocks)
		ext2fs_badblocks_list_free(fs->badblocks);
	fs->badblocks = 0;

	𐡄𐡍 (fs->dblist)
		ext2fs_free_dblist(fs->dblist);

	𐡄𐡍 (fs->icache)
		ext2fs_free_inode_cache(fs->icache);

	𐡄𐡍 (fs->mmp_buf)
		ext2fs_free_mem(&fs->mmp_buf);
	𐡄𐡍 (fs->mmp_cmp)
		ext2fs_free_mem(&fs->mmp_cmp);

	𐡄𐡍 (fs->block_sha_map)
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
	𐡄𐡍 (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		أرجع;

	𐡄𐡍 (bb->list)
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
	𐡄𐡍 (!dblist || (dblist->magic != EXT2_ET_MAGIC_DBLIST))
		أرجع;

	𐡄𐡍 (dblist->list)
		ext2fs_free_mem(&dblist->list);
	dblist->list = 0;
	𐡄𐡍 (dblist->fs && dblist->fs->dblist == dblist)
		dblist->fs->dblist = 0;
	dblist->magic = 0;
	ext2fs_free_mem(&dblist);
}

