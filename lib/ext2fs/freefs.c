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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fsP.h"
#అన్తర్భావ "hashmap.h"

శూన్య ext2fs_free(ext2_filsys fs)
{
	ఒకవేళ (!fs || (fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS))
		తిరిగి;
	ఒకవేళ (fs->image_io != fs->io) {
		ఒకవేళ (fs->image_io)
			io_channel_close(fs->image_io);
	}
	ఒకవేళ (fs->io) {
		io_channel_close(fs->io);
	}
	ఒకవేళ (fs->device_name)
		ext2fs_free_mem(&fs->device_name);
	ఒకవేళ (fs->super)
		ext2fs_free_mem(&fs->super);
	ఒకవేళ (fs->orig_super)
		ext2fs_free_mem(&fs->orig_super);
	ఒకవేళ (fs->సమూహం_desc)
		ext2fs_free_mem(&fs->సమూహం_desc);
	ఒకవేళ (fs->ఖండం_map)
		ext2fs_free_ఖండం_bitmap(fs->ఖండం_map);
	ఒకవేళ (fs->inode_map)
		ext2fs_free_inode_bitmap(fs->inode_map);
	ఒకవేళ (fs->image_శీర్షిక)
		ext2fs_free_mem(&fs->image_శీర్షిక);

	ఒకవేళ (fs->badblocks)
		ext2fs_badblocks_list_free(fs->badblocks);
	fs->badblocks = 0;

	ఒకవేళ (fs->dblist)
		ext2fs_free_dblist(fs->dblist);

	ఒకవేళ (fs->icache)
		ext2fs_free_inode_cache(fs->icache);

	ఒకవేళ (fs->mmp_buf)
		ext2fs_free_mem(&fs->mmp_buf);
	ఒకవేళ (fs->mmp_cmp)
		ext2fs_free_mem(&fs->mmp_cmp);

	ఒకవేళ (fs->ఖండం_sha_map)
		ext2fs_hashmap_free(fs->ఖండం_sha_map);

	fs->magic = 0;

	ext2fs_zero_blocks2(ఖాళీ, 0, 0, ఖాళీ, ఖాళీ);
	ext2fs_free_mem(&fs);
}

/*
 * This procedure frees a badblocks list.
 */
శూన్య ext2fs_u32_list_free(ext2_u32_list bb)
{
	ఒకవేళ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		తిరిగి;

	ఒకవేళ (bb->list)
		ext2fs_free_mem(&bb->list);
	bb->list = 0;
	ext2fs_free_mem(&bb);
}

శూన్య ext2fs_badblocks_list_free(ext2_badblocks_list bb)
{
	ext2fs_u32_list_free((ext2_u32_list) bb);
}


/*
 * Free a directory block list
 */
శూన్య ext2fs_free_dblist(ext2_dblist dblist)
{
	ఒకవేళ (!dblist || (dblist->magic != EXT2_ET_MAGIC_DBLIST))
		తిరిగి;

	ఒకవేళ (dblist->list)
		ext2fs_free_mem(&dblist->list);
	dblist->list = 0;
	ఒకవేళ (dblist->fs && dblist->fs->dblist == dblist)
		dblist->fs->dblist = 0;
	dblist->magic = 0;
	ext2fs_free_mem(&dblist);
}

