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
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"
#ಅನ್ತರ್ಭಾವ "hashmap.h"

ಶೂನ್ಯ ext2fs_free(ext2_filsys fs)
{
	ಆದರೆ (!fs || (fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS))
		ಹಿಂತಿರುಗಿ;
	ಆದರೆ (fs->image_io != fs->io) {
		ಆದರೆ (fs->image_io)
			io_channel_close(fs->image_io);
	}
	ಆದರೆ (fs->io) {
		io_channel_close(fs->io);
	}
	ಆದರೆ (fs->device_name)
		ext2fs_free_mem(&fs->device_name);
	ಆದರೆ (fs->super)
		ext2fs_free_mem(&fs->super);
	ಆದರೆ (fs->orig_super)
		ext2fs_free_mem(&fs->orig_super);
	ಆದರೆ (fs->ಗುಂಪು_desc)
		ext2fs_free_mem(&fs->ಗುಂಪು_desc);
	ಆದರೆ (fs->ಖಂಡ_map)
		ext2fs_free_ಖಂಡ_bitmap(fs->ಖಂಡ_map);
	ಆದರೆ (fs->inode_map)
		ext2fs_free_inode_bitmap(fs->inode_map);
	ಆದರೆ (fs->image_ಶೀರ್ಷಿಕೆ)
		ext2fs_free_mem(&fs->image_ಶೀರ್ಷಿಕೆ);

	ಆದರೆ (fs->badblocks)
		ext2fs_badblocks_list_free(fs->badblocks);
	fs->badblocks = 0;

	ಆದರೆ (fs->dblist)
		ext2fs_free_dblist(fs->dblist);

	ಆದರೆ (fs->icache)
		ext2fs_free_inode_cache(fs->icache);

	ಆದರೆ (fs->mmp_buf)
		ext2fs_free_mem(&fs->mmp_buf);
	ಆದರೆ (fs->mmp_cmp)
		ext2fs_free_mem(&fs->mmp_cmp);

	ಆದರೆ (fs->ಖಂಡ_sha_map)
		ext2fs_hashmap_free(fs->ಖಂಡ_sha_map);

	fs->magic = 0;

	ext2fs_zero_blocks2(ಖಾಲಿ, 0, 0, ಖಾಲಿ, ಖಾಲಿ);
	ext2fs_free_mem(&fs);
}

/*
 * This procedure frees a badblocks list.
 */
ಶೂನ್ಯ ext2fs_u32_list_free(ext2_u32_list bb)
{
	ಆದರೆ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (bb->list)
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
	ಆದರೆ (!dblist || (dblist->magic != EXT2_ET_MAGIC_DBLIST))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (dblist->list)
		ext2fs_free_mem(&dblist->list);
	dblist->list = 0;
	ಆದರೆ (dblist->fs && dblist->fs->dblist == dblist)
		dblist->fs->dblist = 0;
	dblist->magic = 0;
	ext2fs_free_mem(&dblist);
}

