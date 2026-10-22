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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#پایان_اگر
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

#اگر_تعریف_نشده EXT2_FT_DIR
#تعریف_کن EXT2_FT_DIR		2
#پایان_اگر

errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t والد, ext2_ino_t ino,
			بیعلامت دراز flags, صحيح link_flags, ثابت حرف *name,
			ext2_ino_t *ret_ino)
{
	ext2_extent_handle_t	handle;
	errcode_t		retval;
	بنية ext2_inode	والد_inode, inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	حرف			*بلوک = 0;
	صحيح			inline_داده = 0;
	صحيح			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * Create a new dir with inline data iff this feature is enabled
	 * and ino >= EXT2_FIRST_INO.
	 */
	إذا ((!ino || ino >= EXT2_اولی_INO(fs->super)) &&
	    ext2fs_has_feature_inline_داده(fs->super))
		inline_داده = 1;

	/*
	 * Allocate an inode, if necessary
	 */
	إذا (!ino) {
		retval = ext2fs_new_inode(fs, والد, LINUX_S_IFDIR | 0755,
					  0, &ino);
		إذا (retval)
			اذهب cleanup;
	}
	إذا (ret_ino)
		*ret_ino = ino;

	/*
	 * Allocate a data block for the directory
	 */
	memset(&inode, 0, حجم(بنية ext2_inode));
	إذا (!inline_داده) {
		retval = ext2fs_new_block2(fs, ext2fs_پیدا_کن_inode_goal(fs, ino,
								      &inode,
								      0),
					   عدم, &blk);
		إذا (retval)
			اذهب cleanup;
	}

	/*
	 * Create a scratch template for the directory
	 */
	إذا (inline_داده)
		retval = ext2fs_new_dir_inline_داده(fs, ino, والد,
						    inode.i_بلوک);
	وإلا
		retval = ext2fs_new_dir_بلوک(fs, ino, والد, &بلوک);
	إذا (retval)
		اذهب cleanup;

	/*
	 * Get the parent's inode, if necessary
	 */
	إذا (والد != ino) {
		retval = ext2fs_read_inode(fs, والد, &والد_inode);
		إذا (retval)
			اذهب cleanup;
	} وإلا
		memset(&والد_inode, 0, حجم(والد_inode));

	/*
	 * Create the inode structure....
	 */
	inode.i_نما = LINUX_S_IFDIR | (0777 & ~fs->umask);
	inode.i_uid = inode.i_gid = 0;
	إذا (inline_داده) {
		inode.i_flags |= EXT4_INLINE_داده_FL;
		inode.i_اندازه = EXT4_MIN_INLINE_داده_اندازه;
	} وإلا {
		إذا (ext2fs_has_feature_extents(fs->super))
			inode.i_flags |= EXT4_EXTENTS_FL;
		وإلا
			inode.i_بلوک[0] = blk;
		inode.i_اندازه = fs->blocksize;
		ext2fs_iblk_مجموعه(fs, &inode, 1);
	}
	inode.i_flags |= flags & (EXT2_FL_USER_MODIFIABLE & ~EXT4_EXTENTS_FL);
	inode.i_links_شمار = 2;

	/*
	 * Write out the inode and inode data block.  The inode generation
	 * number is assigned by write_new_inode, which means that the call
	 * to write_dir_block must come after that.
	 */
	retval = ext2fs_write_new_inode(fs, ino, &inode);
	إذا (retval)
		اذهب cleanup;
	إذا (inline_داده) {
		/* init "system.data" for new dir */
		retval = ext2fs_inline_داده_init(fs, ino);
	} وإلا {
		retval = ext2fs_write_dir_block4(fs, blk, بلوک, 0, ino);
		إذا (retval)
			اذهب cleanup;

		إذا (ext2fs_has_feature_extents(fs->super)) {
			retval = ext2fs_extent_open2(fs, ino, &inode, &handle);
			إذا (retval)
				اذهب cleanup;
			retval = ext2fs_extent_مجموعه_bmap(handle, 0, blk, 0);
			ext2fs_extent_free(handle);
			إذا (retval)
				اذهب cleanup;
		}
	}

	/*
	 * Update accounting....
	 */
	إذا (!inline_داده)
		ext2fs_بلوک_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 1);
	drop_refcount = 1;

	/*
	 * Link the directory into the filesystem hierarchy
	 */
	إذا (name) {
		retval = ext2fs_lookup(fs, والد, name, طول_رشته(name), 0,
				       &scratch_ino);
		إذا (!retval) {
			retval = EXT2_ET_DIR_EXISTS;
			name = 0;
			اذهب cleanup;
		}
		إذا (retval != EXT2_ET_FILE_NOT_یافت)
			اذهب cleanup;
		retval = ext2fs_link(fs, والد, name, ino,
				     EXT2_FT_DIR | link_flags);
		إذا (retval)
			اذهب cleanup;
	}

	/*
	 * Update parent inode's counts
	 */
	إذا (والد != ino) {
		/* reload parent inode due to inline data */
		retval = ext2fs_read_inode(fs, والد, &والد_inode);
		إذا (retval)
			اذهب cleanup;
		ext2fs_inc_nlink(fs, &والد_inode);
		retval = ext2fs_write_inode(fs, والد, &والد_inode);
		إذا (retval)
			اذهب cleanup;
	}
	drop_refcount = 0;

cleanup:
	إذا (بلوک)
		ext2fs_free_mem(&بلوک);
	إذا (drop_refcount) {
		إذا (!inline_داده)
			ext2fs_بلوک_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 1);
	}
	أرجع retval;

}

errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t والد, ext2_ino_t ino,
		       ثابت حرف *name)
{
	أرجع ext2fs_mkdir2(fs, والد, ino, 0, 0, name, عدم);
}
