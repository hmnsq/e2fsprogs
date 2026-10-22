/*
 * mkdir.c --- make a directory in the filesystem
 *
 * Copyright (C) 1994, 1995 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ EXT2_FT_DIR
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_FT_DIR		2
#ಆದರೆ_ಅಂತ್ಯ

errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t ಪೋಷಕ, ext2_ino_t ino,
			ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags, ಪೂರ್ಣಾಂಕ link_flags, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
			ext2_ino_t *ret_ino)
{
	ext2_extent_handle_t	handle;
	errcode_t		retval;
	ರಚನೆ ext2_inode	ಪೋಷಕ_inode, inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	ಅಕ್ಷರ			*ಖಂಡ = 0;
	ಪೂರ್ಣಾಂಕ			inline_ದತ್ತಾಂಶ = 0;
	ಪೂರ್ಣಾಂಕ			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * Create a new dir with inline data iff this feature is enabled
	 * and ino >= EXT2_FIRST_INO.
	 */
	ಆದರೆ ((!ino || ino >= EXT2_ಮೊದಲ_INO(fs->super)) &&
	    ext2fs_has_feature_inline_ದತ್ತಾಂಶ(fs->super))
		inline_ದತ್ತಾಂಶ = 1;

	/*
	 * Allocate an inode, if necessary
	 */
	ಆದರೆ (!ino) {
		retval = ext2fs_new_inode(fs, ಪೋಷಕ, LINUX_S_IFDIR | 0755,
					  0, &ino);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
	}
	ಆದರೆ (ret_ino)
		*ret_ino = ino;

	/*
	 * Allocate a data block for the directory
	 */
	memset(&inode, 0, ಗಾತ್ರ(ರಚನೆ ext2_inode));
	ಆದರೆ (!inline_ದತ್ತಾಂಶ) {
		retval = ext2fs_new_block2(fs, ext2fs_ಕಂಡುಹಿಡಿ_inode_goal(fs, ino,
								      &inode,
								      0),
					   ಖಾಲಿ, &blk);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
	}

	/*
	 * Create a scratch template for the directory
	 */
	ಆದರೆ (inline_ದತ್ತಾಂಶ)
		retval = ext2fs_new_dir_inline_ದತ್ತಾಂಶ(fs, ino, ಪೋಷಕ,
						    inode.i_ಖಂಡ);
	ಇಲ್ಲದಿದ್ದರೆ
		retval = ext2fs_new_dir_ಖಂಡ(fs, ino, ಪೋಷಕ, &ಖಂಡ);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;

	/*
	 * Get the parent's inode, if necessary
	 */
	ಆದರೆ (ಪೋಷಕ != ino) {
		retval = ext2fs_read_inode(fs, ಪೋಷಕ, &ಪೋಷಕ_inode);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
	} ಇಲ್ಲದಿದ್ದರೆ
		memset(&ಪೋಷಕ_inode, 0, ಗಾತ್ರ(ಪೋಷಕ_inode));

	/*
	 * Create the inode structure....
	 */
	inode.i_ಬಹುಲಕ = LINUX_S_IFDIR | (0777 & ~fs->umask);
	inode.i_uid = inode.i_gid = 0;
	ಆದರೆ (inline_ದತ್ತಾಂಶ) {
		inode.i_flags |= EXT4_INLINE_ದತ್ತಾಂಶ_FL;
		inode.i_ಗಾತ್ರ = EXT4_MIN_INLINE_ದತ್ತಾಂಶ_ಗಾತ್ರ;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (ext2fs_has_feature_extents(fs->super))
			inode.i_flags |= EXT4_EXTENTS_FL;
		ಇಲ್ಲದಿದ್ದರೆ
			inode.i_ಖಂಡ[0] = blk;
		inode.i_ಗಾತ್ರ = fs->blocksize;
		ext2fs_iblk_ಗಣ(fs, &inode, 1);
	}
	inode.i_flags |= flags & (EXT2_FL_USER_MODIFIABLE & ~EXT4_EXTENTS_FL);
	inode.i_links_ಎಣಿಕೆ = 2;

	/*
	 * Write out the inode and inode data block.  The inode generation
	 * number is assigned by write_new_inode, which means that the call
	 * to write_dir_block must come after that.
	 */
	retval = ext2fs_write_new_inode(fs, ino, &inode);
	ಆದರೆ (retval)
		ಹೋಗು cleanup;
	ಆದರೆ (inline_ದತ್ತಾಂಶ) {
		/* init "system.data" for new dir */
		retval = ext2fs_inline_ದತ್ತಾಂಶ_init(fs, ino);
	} ಇಲ್ಲದಿದ್ದರೆ {
		retval = ext2fs_write_dir_block4(fs, blk, ಖಂಡ, 0, ino);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;

		ಆದರೆ (ext2fs_has_feature_extents(fs->super)) {
			retval = ext2fs_extent_open2(fs, ino, &inode, &handle);
			ಆದರೆ (retval)
				ಹೋಗು cleanup;
			retval = ext2fs_extent_ಗಣ_bmap(handle, 0, blk, 0);
			ext2fs_extent_free(handle);
			ಆದರೆ (retval)
				ಹೋಗು cleanup;
		}
	}

	/*
	 * Update accounting....
	 */
	ಆದರೆ (!inline_ದತ್ತಾಂಶ)
		ext2fs_ಖಂಡ_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 1);
	drop_refcount = 1;

	/*
	 * Link the directory into the filesystem hierarchy
	 */
	ಆದರೆ (name) {
		retval = ext2fs_lookup(fs, ಪೋಷಕ, name, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name), 0,
				       &scratch_ino);
		ಆದರೆ (!retval) {
			retval = EXT2_ET_DIR_EXISTS;
			name = 0;
			ಹೋಗು cleanup;
		}
		ಆದರೆ (retval != EXT2_ET_FILE_NOT_ಸಿಕ್ಕಿತು)
			ಹೋಗು cleanup;
		retval = ext2fs_link(fs, ಪೋಷಕ, name, ino,
				     EXT2_FT_DIR | link_flags);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
	}

	/*
	 * Update parent inode's counts
	 */
	ಆದರೆ (ಪೋಷಕ != ino) {
		/* reload parent inode due to inline data */
		retval = ext2fs_read_inode(fs, ಪೋಷಕ, &ಪೋಷಕ_inode);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
		ext2fs_inc_nlink(fs, &ಪೋಷಕ_inode);
		retval = ext2fs_write_inode(fs, ಪೋಷಕ, &ಪೋಷಕ_inode);
		ಆದರೆ (retval)
			ಹೋಗು cleanup;
	}
	drop_refcount = 0;

cleanup:
	ಆದರೆ (ಖಂಡ)
		ext2fs_free_mem(&ಖಂಡ);
	ಆದರೆ (drop_refcount) {
		ಆದರೆ (!inline_ದತ್ತಾಂಶ)
			ext2fs_ಖಂಡ_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 1);
	}
	ಹಿಂತಿರುಗಿ retval;

}

errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t ಪೋಷಕ, ext2_ino_t ino,
		       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
	ಹಿಂತಿರುಗಿ ext2fs_mkdir2(fs, ಪೋಷಕ, ino, 0, 0, name, ಖಾಲಿ);
}
