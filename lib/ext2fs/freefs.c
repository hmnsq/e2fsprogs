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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"
#ଅନ୍ତର୍ଭାଵ "hashmap.h"

ଶୂନ୍ୟ ext2fs_free(ext2_filsys fs)
{
	ଯଦି (!fs || (fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS))
		ଫେରନ୍ତୁ;
	ଯଦି (fs->image_io != fs->io) {
		ଯଦି (fs->image_io)
			io_channel_close(fs->image_io);
	}
	ଯଦି (fs->io) {
		io_channel_close(fs->io);
	}
	ଯଦି (fs->device_name)
		ext2fs_free_mem(&fs->device_name);
	ଯଦି (fs->super)
		ext2fs_free_mem(&fs->super);
	ଯଦି (fs->orig_super)
		ext2fs_free_mem(&fs->orig_super);
	ଯଦି (fs->group_desc)
		ext2fs_free_mem(&fs->group_desc);
	ଯଦି (fs->block_map)
		ext2fs_free_block_bitmap(fs->block_map);
	ଯଦି (fs->inode_map)
		ext2fs_free_inode_bitmap(fs->inode_map);
	ଯଦି (fs->image_header)
		ext2fs_free_mem(&fs->image_header);

	ଯଦି (fs->badblocks)
		ext2fs_badblocks_list_free(fs->badblocks);
	fs->badblocks = 0;

	ଯଦି (fs->dblist)
		ext2fs_free_dblist(fs->dblist);

	ଯଦି (fs->icache)
		ext2fs_free_inode_cache(fs->icache);

	ଯଦି (fs->mmp_buf)
		ext2fs_free_mem(&fs->mmp_buf);
	ଯଦି (fs->mmp_cmp)
		ext2fs_free_mem(&fs->mmp_cmp);

	ଯଦି (fs->block_sha_map)
		ext2fs_hashmap_free(fs->block_sha_map);

	fs->magic = 0;

	ext2fs_zero_blocks2(ଖାଲି, 0, 0, ଖାଲି, ଖାଲି);
	ext2fs_free_mem(&fs);
}

/*
 * This procedure frees a badblocks list.
 */
ଶୂନ୍ୟ ext2fs_u32_list_free(ext2_u32_list bb)
{
	ଯଦି (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ଫେରନ୍ତୁ;

	ଯଦି (bb->list)
		ext2fs_free_mem(&bb->list);
	bb->list = 0;
	ext2fs_free_mem(&bb);
}

ଶୂନ୍ୟ ext2fs_badblocks_list_free(ext2_badblocks_list bb)
{
	ext2fs_u32_list_free((ext2_u32_list) bb);
}


/*
 * Free a directory block list
 */
ଶୂନ୍ୟ ext2fs_free_dblist(ext2_dblist dblist)
{
	ଯଦି (!dblist || (dblist->magic != EXT2_ET_MAGIC_DBLIST))
		ଫେରନ୍ତୁ;

	ଯଦି (dblist->list)
		ext2fs_free_mem(&dblist->list);
	dblist->list = 0;
	ଯଦି (dblist->fs && dblist->fs->dblist == dblist)
		dblist->fs->dblist = 0;
	dblist->magic = 0;
	ext2fs_free_mem(&dblist);
}

