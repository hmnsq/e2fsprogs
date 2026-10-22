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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"
#ಅನ್ತರ್ಭಾವ "hashmap.h"

ಶೂನ್ಯ ext2fs_free(ext2_filsys fs)
{
	ಯದಿ (!fs || (fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS))
		ಪ್ರತ್ಯಾಗಮ;
	ಯದಿ (fs->image_io != fs->io) {
		ಯದಿ (fs->image_io)
			io_channel_close(fs->image_io);
	}
	ಯದಿ (fs->io) {
		io_channel_close(fs->io);
	}
	ಯದಿ (fs->device_name)
		ext2fs_free_mem(&fs->device_name);
	ಯದಿ (fs->super)
		ext2fs_free_mem(&fs->super);
	ಯದಿ (fs->orig_super)
		ext2fs_free_mem(&fs->orig_super);
	ಯದಿ (fs->group_desc)
		ext2fs_free_mem(&fs->group_desc);
	ಯದಿ (fs->block_map)
		ext2fs_free_block_bitmap(fs->block_map);
	ಯದಿ (fs->inode_map)
		ext2fs_free_inode_bitmap(fs->inode_map);
	ಯದಿ (fs->image_header)
		ext2fs_free_mem(&fs->image_header);

	ಯದಿ (fs->badblocks)
		ext2fs_badblocks_list_free(fs->badblocks);
	fs->badblocks = 0;

	ಯದಿ (fs->dblist)
		ext2fs_free_dblist(fs->dblist);

	ಯದಿ (fs->icache)
		ext2fs_free_inode_cache(fs->icache);

	ಯದಿ (fs->mmp_buf)
		ext2fs_free_mem(&fs->mmp_buf);
	ಯದಿ (fs->mmp_cmp)
		ext2fs_free_mem(&fs->mmp_cmp);

	ಯದಿ (fs->block_sha_map)
		ext2fs_hashmap_free(fs->block_sha_map);

	fs->magic = 0;

	ext2fs_zero_blocks2(ರಿಕ್ತ, 0, 0, ರಿಕ್ತ, ರಿಕ್ತ);
	ext2fs_free_mem(&fs);
}

/*
 * This procedure frees a badblocks list.
 */
ಶೂನ್ಯ ext2fs_u32_list_free(ext2_u32_list bb)
{
	ಯದಿ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (bb->list)
		ext2fs_free_mem(&bb->list);
	bb->list = 0;
	ext2fs_free_mem(&bb);
}

ಶೂನ್ಯ ext2fs_badblocks_list_free(ext2_badblocks_list bb)
{
	ext2fs_u32_list_free((ext2_u32_list) bb);
}


/*
 * Free a directory block list
 */
ಶೂನ್ಯ ext2fs_free_dblist(ext2_dblist dblist)
{
	ಯದಿ (!dblist || (dblist->magic != EXT2_ET_MAGIC_DBLIST))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (dblist->list)
		ext2fs_free_mem(&dblist->list);
	dblist->list = 0;
	ಯದಿ (dblist->fs && dblist->fs->dblist == dblist)
		dblist->fs->dblist = 0;
	dblist->magic = 0;
	ext2fs_free_mem(&dblist);
}

