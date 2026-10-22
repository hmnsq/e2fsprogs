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
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"
#ضمن "hashmap.h"

فراغ ext2fs_free(ext2_filsys fs)
{
	إذا (!fs || (fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS))
		أرجع;
	إذا (fs->image_io != fs->io) {
		إذا (fs->image_io)
			io_channel_close(fs->image_io);
	}
	إذا (fs->io) {
		io_channel_close(fs->io);
	}
	إذا (fs->device_name)
		ext2fs_free_mem(&fs->device_name);
	إذا (fs->super)
		ext2fs_free_mem(&fs->super);
	إذا (fs->orig_super)
		ext2fs_free_mem(&fs->orig_super);
	إذا (fs->گروه_desc)
		ext2fs_free_mem(&fs->گروه_desc);
	إذا (fs->بلوک_map)
		ext2fs_free_بلوک_bitmap(fs->بلوک_map);
	إذا (fs->inode_map)
		ext2fs_free_inode_bitmap(fs->inode_map);
	إذا (fs->image_سرآیند)
		ext2fs_free_mem(&fs->image_سرآیند);

	إذا (fs->badblocks)
		ext2fs_badblocks_list_free(fs->badblocks);
	fs->badblocks = 0;

	إذا (fs->dblist)
		ext2fs_free_dblist(fs->dblist);

	إذا (fs->icache)
		ext2fs_free_inode_cache(fs->icache);

	إذا (fs->mmp_buf)
		ext2fs_free_mem(&fs->mmp_buf);
	إذا (fs->mmp_cmp)
		ext2fs_free_mem(&fs->mmp_cmp);

	إذا (fs->بلوک_sha_map)
		ext2fs_hashmap_free(fs->بلوک_sha_map);

	fs->magic = 0;

	ext2fs_zero_blocks2(عدم, 0, 0, عدم, عدم);
	ext2fs_free_mem(&fs);
}

/*
 * This procedure frees a badblocks list.
 */
فراغ ext2fs_u32_list_free(ext2_u32_list bb)
{
	إذا (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		أرجع;

	إذا (bb->list)
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
	إذا (!dblist || (dblist->magic != EXT2_ET_MAGIC_DBLIST))
		أرجع;

	إذا (dblist->list)
		ext2fs_free_mem(&dblist->list);
	dblist->list = 0;
	إذا (dblist->fs && dblist->fs->dblist == dblist)
		dblist->fs->dblist = 0;
	dblist->magic = 0;
	ext2fs_free_mem(&dblist);
}

