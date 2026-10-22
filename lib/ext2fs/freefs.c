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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fsP.h"
#അന്തര്ഭാവ "hashmap.h"

ശൂന്യം ext2fs_free(ext2_filsys fs)
{
	എങ്കിൽ (!fs || (fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS))
		തിരികെ;
	എങ്കിൽ (fs->image_io != fs->io) {
		എങ്കിൽ (fs->image_io)
			io_channel_close(fs->image_io);
	}
	എങ്കിൽ (fs->io) {
		io_channel_close(fs->io);
	}
	എങ്കിൽ (fs->device_name)
		ext2fs_free_mem(&fs->device_name);
	എങ്കിൽ (fs->super)
		ext2fs_free_mem(&fs->super);
	എങ്കിൽ (fs->orig_super)
		ext2fs_free_mem(&fs->orig_super);
	എങ്കിൽ (fs->group_desc)
		ext2fs_free_mem(&fs->group_desc);
	എങ്കിൽ (fs->block_map)
		ext2fs_free_block_bitmap(fs->block_map);
	എങ്കിൽ (fs->inode_map)
		ext2fs_free_inode_bitmap(fs->inode_map);
	എങ്കിൽ (fs->image_header)
		ext2fs_free_mem(&fs->image_header);

	എങ്കിൽ (fs->badblocks)
		ext2fs_badblocks_list_free(fs->badblocks);
	fs->badblocks = 0;

	എങ്കിൽ (fs->dblist)
		ext2fs_free_dblist(fs->dblist);

	എങ്കിൽ (fs->icache)
		ext2fs_free_inode_cache(fs->icache);

	എങ്കിൽ (fs->mmp_buf)
		ext2fs_free_mem(&fs->mmp_buf);
	എങ്കിൽ (fs->mmp_cmp)
		ext2fs_free_mem(&fs->mmp_cmp);

	എങ്കിൽ (fs->block_sha_map)
		ext2fs_hashmap_free(fs->block_sha_map);

	fs->magic = 0;

	ext2fs_zero_blocks2(ശൂന്യ, 0, 0, ശൂന്യ, ശൂന്യ);
	ext2fs_free_mem(&fs);
}

/*
 * This procedure frees a badblocks list.
 */
ശൂന്യം ext2fs_u32_list_free(ext2_u32_list bb)
{
	എങ്കിൽ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		തിരികെ;

	എങ്കിൽ (bb->list)
		ext2fs_free_mem(&bb->list);
	bb->list = 0;
	ext2fs_free_mem(&bb);
}

ശൂന്യം ext2fs_badblocks_list_free(ext2_badblocks_list bb)
{
	ext2fs_u32_list_free((ext2_u32_list) bb);
}


/*
 * Free a directory block list
 */
ശൂന്യം ext2fs_free_dblist(ext2_dblist dblist)
{
	എങ്കിൽ (!dblist || (dblist->magic != EXT2_ET_MAGIC_DBLIST))
		തിരികെ;

	എങ്കിൽ (dblist->list)
		ext2fs_free_mem(&dblist->list);
	dblist->list = 0;
	എങ്കിൽ (dblist->fs && dblist->fs->dblist == dblist)
		dblist->fs->dblist = 0;
	dblist->magic = 0;
	ext2fs_free_mem(&dblist);
}

