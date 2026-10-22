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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fsP.h"
#அந்தர்பாவ "hashmap.h"

வெறுமை ext2fs_free(ext2_filsys fs)
{
	எனில் (!fs || (fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS))
		திரும்பு;
	எனில் (fs->image_io != fs->io) {
		எனில் (fs->image_io)
			io_channel_close(fs->image_io);
	}
	எனில் (fs->io) {
		io_channel_close(fs->io);
	}
	எனில் (fs->device_name)
		ext2fs_free_mem(&fs->device_name);
	எனில் (fs->super)
		ext2fs_free_mem(&fs->super);
	எனில் (fs->orig_super)
		ext2fs_free_mem(&fs->orig_super);
	எனில் (fs->group_desc)
		ext2fs_free_mem(&fs->group_desc);
	எனில் (fs->block_map)
		ext2fs_free_block_bitmap(fs->block_map);
	எனில் (fs->inode_map)
		ext2fs_free_inode_bitmap(fs->inode_map);
	எனில் (fs->image_header)
		ext2fs_free_mem(&fs->image_header);

	எனில் (fs->badblocks)
		ext2fs_badblocks_list_free(fs->badblocks);
	fs->badblocks = 0;

	எனில் (fs->dblist)
		ext2fs_free_dblist(fs->dblist);

	எனில் (fs->icache)
		ext2fs_free_inode_cache(fs->icache);

	எனில் (fs->mmp_buf)
		ext2fs_free_mem(&fs->mmp_buf);
	எனில் (fs->mmp_cmp)
		ext2fs_free_mem(&fs->mmp_cmp);

	எனில் (fs->block_sha_map)
		ext2fs_hashmap_free(fs->block_sha_map);

	fs->magic = 0;

	ext2fs_zero_blocks2(வெற்று, 0, 0, வெற்று, வெற்று);
	ext2fs_free_mem(&fs);
}

/*
 * This procedure frees a badblocks list.
 */
வெறுமை ext2fs_u32_list_free(ext2_u32_list bb)
{
	எனில் (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		திரும்பு;

	எனில் (bb->list)
		ext2fs_free_mem(&bb->list);
	bb->list = 0;
	ext2fs_free_mem(&bb);
}

வெறுமை ext2fs_badblocks_list_free(ext2_badblocks_list bb)
{
	ext2fs_u32_list_free((ext2_u32_list) bb);
}


/*
 * Free a directory block list
 */
வெறுமை ext2fs_free_dblist(ext2_dblist dblist)
{
	எனில் (!dblist || (dblist->magic != EXT2_ET_MAGIC_DBLIST))
		திரும்பு;

	எனில் (dblist->list)
		ext2fs_free_mem(&dblist->list);
	dblist->list = 0;
	எனில் (dblist->fs && dblist->fs->dblist == dblist)
		dblist->fs->dblist = 0;
	dblist->magic = 0;
	ext2fs_free_mem(&dblist);
}

