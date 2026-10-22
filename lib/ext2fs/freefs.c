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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"
#अन्तर्भाव "hashmap.h"

शून्य ext2fs_free(ext2_filsys fs)
{
	जर (!fs || (fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS))
		परत;
	जर (fs->image_io != fs->io) {
		जर (fs->image_io)
			io_channel_close(fs->image_io);
	}
	जर (fs->io) {
		io_channel_close(fs->io);
	}
	जर (fs->device_name)
		ext2fs_free_mem(&fs->device_name);
	जर (fs->super)
		ext2fs_free_mem(&fs->super);
	जर (fs->orig_super)
		ext2fs_free_mem(&fs->orig_super);
	जर (fs->group_desc)
		ext2fs_free_mem(&fs->group_desc);
	जर (fs->block_map)
		ext2fs_free_block_bitmap(fs->block_map);
	जर (fs->inode_map)
		ext2fs_free_inode_bitmap(fs->inode_map);
	जर (fs->image_header)
		ext2fs_free_mem(&fs->image_header);

	जर (fs->badblocks)
		ext2fs_badblocks_list_free(fs->badblocks);
	fs->badblocks = 0;

	जर (fs->dblist)
		ext2fs_free_dblist(fs->dblist);

	जर (fs->icache)
		ext2fs_free_inode_cache(fs->icache);

	जर (fs->mmp_buf)
		ext2fs_free_mem(&fs->mmp_buf);
	जर (fs->mmp_cmp)
		ext2fs_free_mem(&fs->mmp_cmp);

	जर (fs->block_sha_map)
		ext2fs_hashmap_free(fs->block_sha_map);

	fs->magic = 0;

	ext2fs_zero_blocks2(रिक्त, 0, 0, रिक्त, रिक्त);
	ext2fs_free_mem(&fs);
}

/*
 * This procedure frees a badblocks list.
 */
शून्य ext2fs_u32_list_free(ext2_u32_list bb)
{
	जर (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		परत;

	जर (bb->list)
		ext2fs_free_mem(&bb->list);
	bb->list = 0;
	ext2fs_free_mem(&bb);
}

शून्य ext2fs_badblocks_list_free(ext2_badblocks_list bb)
{
	ext2fs_u32_list_free((ext2_u32_list) bb);
}


/*
 * Free a directory block list
 */
शून्य ext2fs_free_dblist(ext2_dblist dblist)
{
	जर (!dblist || (dblist->magic != EXT2_ET_MAGIC_DBLIST))
		परत;

	जर (dblist->list)
		ext2fs_free_mem(&dblist->list);
	dblist->list = 0;
	जर (dblist->fs && dblist->fs->dblist == dblist)
		dblist->fs->dblist = 0;
	dblist->magic = 0;
	ext2fs_free_mem(&dblist);
}

