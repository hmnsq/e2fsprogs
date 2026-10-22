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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"
#अन्तर्भाव "hashmap.h"

शून्य ext2fs_free(ext2_filsys fs)
{
	यदि (!fs || (fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS))
		फर्काऊ;
	यदि (fs->image_io != fs->io) {
		यदि (fs->image_io)
			io_channel_close(fs->image_io);
	}
	यदि (fs->io) {
		io_channel_close(fs->io);
	}
	यदि (fs->device_name)
		ext2fs_free_mem(&fs->device_name);
	यदि (fs->super)
		ext2fs_free_mem(&fs->super);
	यदि (fs->orig_super)
		ext2fs_free_mem(&fs->orig_super);
	यदि (fs->group_desc)
		ext2fs_free_mem(&fs->group_desc);
	यदि (fs->block_map)
		ext2fs_free_block_bitmap(fs->block_map);
	यदि (fs->inode_map)
		ext2fs_free_inode_bitmap(fs->inode_map);
	यदि (fs->image_header)
		ext2fs_free_mem(&fs->image_header);

	यदि (fs->badblocks)
		ext2fs_badblocks_list_free(fs->badblocks);
	fs->badblocks = 0;

	यदि (fs->dblist)
		ext2fs_free_dblist(fs->dblist);

	यदि (fs->icache)
		ext2fs_free_inode_cache(fs->icache);

	यदि (fs->mmp_buf)
		ext2fs_free_mem(&fs->mmp_buf);
	यदि (fs->mmp_cmp)
		ext2fs_free_mem(&fs->mmp_cmp);

	यदि (fs->block_sha_map)
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
	यदि (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		फर्काऊ;

	यदि (bb->list)
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
	यदि (!dblist || (dblist->magic != EXT2_ET_MAGIC_DBLIST))
		फर्काऊ;

	यदि (dblist->list)
		ext2fs_free_mem(&dblist->list);
	dblist->list = 0;
	यदि (dblist->fs && dblist->fs->dblist == dblist)
		dblist->fs->dblist = 0;
	dblist->magic = 0;
	ext2fs_free_mem(&dblist);
}

