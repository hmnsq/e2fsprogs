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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ EXT2_FT_DIR
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_FT_DIR		2
#ਜੇ_ਅੰਤ

errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t ਮਾਪਾ, ext2_ino_t ino,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags, ਪੂਰਨ link_flags, ਅਚਲ ਅੱਖਰ *name,
			ext2_ino_t *ret_ino)
{
	ext2_extent_handle_t	handle;
	errcode_t		retval;
	ਬਣਤਰ ext2_inode	ਮਾਪਾ_inode, inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	ਅੱਖਰ			*ਖੰਡ = 0;
	ਪੂਰਨ			inline_ਅੰਕੜੇ = 0;
	ਪੂਰਨ			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * Create a new dir with inline data iff this feature is enabled
	 * and ino >= EXT2_FIRST_INO.
	 */
	ਜੇ ((!ino || ino >= EXT2_ਪਹਿਲਾ_INO(fs->super)) &&
	    ext2fs_has_feature_inline_ਅੰਕੜੇ(fs->super))
		inline_ਅੰਕੜੇ = 1;

	/*
	 * Allocate an inode, if necessary
	 */
	ਜੇ (!ino) {
		retval = ext2fs_new_inode(fs, ਮਾਪਾ, LINUX_S_IFDIR | 0755,
					  0, &ino);
		ਜੇ (retval)
			ਜਾਓ cleanup;
	}
	ਜੇ (ret_ino)
		*ret_ino = ino;

	/*
	 * Allocate a data block for the directory
	 */
	memset(&inode, 0, ਆਕਾਰ(ਬਣਤਰ ext2_inode));
	ਜੇ (!inline_ਅੰਕੜੇ) {
		retval = ext2fs_new_block2(fs, ext2fs_ਲੱਭੋ_inode_goal(fs, ino,
								      &inode,
								      0),
					   ਨਿਰਾ, &blk);
		ਜੇ (retval)
			ਜਾਓ cleanup;
	}

	/*
	 * Create a scratch template for the directory
	 */
	ਜੇ (inline_ਅੰਕੜੇ)
		retval = ext2fs_new_dir_inline_ਅੰਕੜੇ(fs, ino, ਮਾਪਾ,
						    inode.i_ਖੰਡ);
	ਨਹੀਂਤਾਂ
		retval = ext2fs_new_dir_ਖੰਡ(fs, ino, ਮਾਪਾ, &ਖੰਡ);
	ਜੇ (retval)
		ਜਾਓ cleanup;

	/*
	 * Get the parent's inode, if necessary
	 */
	ਜੇ (ਮਾਪਾ != ino) {
		retval = ext2fs_read_inode(fs, ਮਾਪਾ, &ਮਾਪਾ_inode);
		ਜੇ (retval)
			ਜਾਓ cleanup;
	} ਨਹੀਂਤਾਂ
		memset(&ਮਾਪਾ_inode, 0, ਆਕਾਰ(ਮਾਪਾ_inode));

	/*
	 * Create the inode structure....
	 */
	inode.i_ਬਹੁਲਕ = LINUX_S_IFDIR | (0777 & ~fs->umask);
	inode.i_uid = inode.i_gid = 0;
	ਜੇ (inline_ਅੰਕੜੇ) {
		inode.i_flags |= EXT4_INLINE_ਅੰਕੜੇ_FL;
		inode.i_ਮਾਪ = EXT4_MIN_INLINE_ਅੰਕੜੇ_ਮਾਪ;
	} ਨਹੀਂਤਾਂ {
		ਜੇ (ext2fs_has_feature_extents(fs->super))
			inode.i_flags |= EXT4_EXTENTS_FL;
		ਨਹੀਂਤਾਂ
			inode.i_ਖੰਡ[0] = blk;
		inode.i_ਮਾਪ = fs->blocksize;
		ext2fs_iblk_ਸੈੱਟ(fs, &inode, 1);
	}
	inode.i_flags |= flags & (EXT2_FL_USER_MODIFIABLE & ~EXT4_EXTENTS_FL);
	inode.i_links_ਗਿਣਤੀ = 2;

	/*
	 * Write out the inode and inode data block.  The inode generation
	 * number is assigned by write_new_inode, which means that the call
	 * to write_dir_block must come after that.
	 */
	retval = ext2fs_write_new_inode(fs, ino, &inode);
	ਜੇ (retval)
		ਜਾਓ cleanup;
	ਜੇ (inline_ਅੰਕੜੇ) {
		/* init "system.data" for new dir */
		retval = ext2fs_inline_ਅੰਕੜੇ_init(fs, ino);
	} ਨਹੀਂਤਾਂ {
		retval = ext2fs_write_dir_block4(fs, blk, ਖੰਡ, 0, ino);
		ਜੇ (retval)
			ਜਾਓ cleanup;

		ਜੇ (ext2fs_has_feature_extents(fs->super)) {
			retval = ext2fs_extent_open2(fs, ino, &inode, &handle);
			ਜੇ (retval)
				ਜਾਓ cleanup;
			retval = ext2fs_extent_ਸੈੱਟ_bmap(handle, 0, blk, 0);
			ext2fs_extent_free(handle);
			ਜੇ (retval)
				ਜਾਓ cleanup;
		}
	}

	/*
	 * Update accounting....
	 */
	ਜੇ (!inline_ਅੰਕੜੇ)
		ext2fs_ਖੰਡ_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 1);
	drop_refcount = 1;

	/*
	 * Link the directory into the filesystem hierarchy
	 */
	ਜੇ (name) {
		retval = ext2fs_lookup(fs, ਮਾਪਾ, name, ਲੜੀ_ਲੰਬਾਈ(name), 0,
				       &scratch_ino);
		ਜੇ (!retval) {
			retval = EXT2_ET_DIR_EXISTS;
			name = 0;
			ਜਾਓ cleanup;
		}
		ਜੇ (retval != EXT2_ET_FILE_NOT_ਮਿਲਿਆ)
			ਜਾਓ cleanup;
		retval = ext2fs_link(fs, ਮਾਪਾ, name, ino,
				     EXT2_FT_DIR | link_flags);
		ਜੇ (retval)
			ਜਾਓ cleanup;
	}

	/*
	 * Update parent inode's counts
	 */
	ਜੇ (ਮਾਪਾ != ino) {
		/* reload parent inode due to inline data */
		retval = ext2fs_read_inode(fs, ਮਾਪਾ, &ਮਾਪਾ_inode);
		ਜੇ (retval)
			ਜਾਓ cleanup;
		ext2fs_inc_nlink(fs, &ਮਾਪਾ_inode);
		retval = ext2fs_write_inode(fs, ਮਾਪਾ, &ਮਾਪਾ_inode);
		ਜੇ (retval)
			ਜਾਓ cleanup;
	}
	drop_refcount = 0;

cleanup:
	ਜੇ (ਖੰਡ)
		ext2fs_free_mem(&ਖੰਡ);
	ਜੇ (drop_refcount) {
		ਜੇ (!inline_ਅੰਕੜੇ)
			ext2fs_ਖੰਡ_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 1);
	}
	ਵਾਪਸ retval;

}

errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t ਮਾਪਾ, ext2_ino_t ino,
		       ਅਚਲ ਅੱਖਰ *name)
{
	ਵਾਪਸ ext2fs_mkdir2(fs, ਮਾਪਾ, ino, 0, 0, name, ਨਿਰਾ);
}
