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
#د_که_پای
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

#که_نه_تعریف_شوی EXT2_FT_DIR
#تعریف_کړه EXT2_FT_DIR		2
#د_که_پای

errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t پلار, ext2_ino_t ino,
			بینښه اوږد flags, صحيح link_flags, ثابت حرف *name,
			ext2_ino_t *ret_ino)
{
	ext2_extent_handle_t	handle;
	errcode_t		retval;
	بنية ext2_inode	پلار_inode, inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	حرف			*بلاک = 0;
	صحيح			inline_معلومات = 0;
	صحيح			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * Create a new dir with inline data iff this feature is enabled
	 * and ino >= EXT2_FIRST_INO.
	 */
	إذا ((!ino || ino >= EXT2_لومړی_INO(fs->super)) &&
	    ext2fs_has_feature_inline_معلومات(fs->super))
		inline_معلومات = 1;

	/*
	 * Allocate an inode, if necessary
	 */
	إذا (!ino) {
		retval = ext2fs_new_inode(fs, پلار, LINUX_S_IFDIR | 0755,
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
	إذا (!inline_معلومات) {
		retval = ext2fs_new_block2(fs, ext2fs_پیدا_کړه_inode_goal(fs, ino,
								      &inode,
								      0),
					   عدم, &blk);
		إذا (retval)
			اذهب cleanup;
	}

	/*
	 * Create a scratch template for the directory
	 */
	إذا (inline_معلومات)
		retval = ext2fs_new_dir_inline_معلومات(fs, ino, پلار,
						    inode.i_بلاک);
	وإلا
		retval = ext2fs_new_dir_بلاک(fs, ino, پلار, &بلاک);
	إذا (retval)
		اذهب cleanup;

	/*
	 * Get the parent's inode, if necessary
	 */
	إذا (پلار != ino) {
		retval = ext2fs_read_inode(fs, پلار, &پلار_inode);
		إذا (retval)
			اذهب cleanup;
	} وإلا
		memset(&پلار_inode, 0, حجم(پلار_inode));

	/*
	 * Create the inode structure....
	 */
	inode.i_ډېرپېښ = LINUX_S_IFDIR | (0777 & ~fs->umask);
	inode.i_uid = inode.i_gid = 0;
	إذا (inline_معلومات) {
		inode.i_flags |= EXT4_INLINE_معلومات_FL;
		inode.i_اندازه = EXT4_MIN_INLINE_معلومات_اندازه;
	} وإلا {
		إذا (ext2fs_has_feature_extents(fs->super))
			inode.i_flags |= EXT4_EXTENTS_FL;
		وإلا
			inode.i_بلاک[0] = blk;
		inode.i_اندازه = fs->blocksize;
		ext2fs_iblk_سېټ(fs, &inode, 1);
	}
	inode.i_flags |= flags & (EXT2_FL_USER_MODIFIABLE & ~EXT4_EXTENTS_FL);
	inode.i_links_شمېر = 2;

	/*
	 * Write out the inode and inode data block.  The inode generation
	 * number is assigned by write_new_inode, which means that the call
	 * to write_dir_block must come after that.
	 */
	retval = ext2fs_write_new_inode(fs, ino, &inode);
	إذا (retval)
		اذهب cleanup;
	إذا (inline_معلومات) {
		/* init "system.data" for new dir */
		retval = ext2fs_inline_معلومات_init(fs, ino);
	} وإلا {
		retval = ext2fs_write_dir_block4(fs, blk, بلاک, 0, ino);
		إذا (retval)
			اذهب cleanup;

		إذا (ext2fs_has_feature_extents(fs->super)) {
			retval = ext2fs_extent_open2(fs, ino, &inode, &handle);
			إذا (retval)
				اذهب cleanup;
			retval = ext2fs_extent_سېټ_bmap(handle, 0, blk, 0);
			ext2fs_extent_free(handle);
			إذا (retval)
				اذهب cleanup;
		}
	}

	/*
	 * Update accounting....
	 */
	إذا (!inline_معلومات)
		ext2fs_بلاک_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 1);
	drop_refcount = 1;

	/*
	 * Link the directory into the filesystem hierarchy
	 */
	إذا (name) {
		retval = ext2fs_lookup(fs, پلار, name, مزي_اوږدوالی(name), 0,
				       &scratch_ino);
		إذا (!retval) {
			retval = EXT2_ET_DIR_EXISTS;
			name = 0;
			اذهب cleanup;
		}
		إذا (retval != EXT2_ET_FILE_NOT_وموندل)
			اذهب cleanup;
		retval = ext2fs_link(fs, پلار, name, ino,
				     EXT2_FT_DIR | link_flags);
		إذا (retval)
			اذهب cleanup;
	}

	/*
	 * Update parent inode's counts
	 */
	إذا (پلار != ino) {
		/* reload parent inode due to inline data */
		retval = ext2fs_read_inode(fs, پلار, &پلار_inode);
		إذا (retval)
			اذهب cleanup;
		ext2fs_inc_nlink(fs, &پلار_inode);
		retval = ext2fs_write_inode(fs, پلار, &پلار_inode);
		إذا (retval)
			اذهب cleanup;
	}
	drop_refcount = 0;

cleanup:
	إذا (بلاک)
		ext2fs_free_mem(&بلاک);
	إذا (drop_refcount) {
		إذا (!inline_معلومات)
			ext2fs_بلاک_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 1);
	}
	أرجع retval;

}

errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t پلار, ext2_ino_t ino,
		       ثابت حرف *name)
{
	أرجع ext2fs_mkdir2(fs, پلار, ino, 0, 0, name, عدم);
}
