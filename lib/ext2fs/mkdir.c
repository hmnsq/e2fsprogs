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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

#اگر_غیر_تعریف_شدہ EXT2_FT_DIR
#تعریف_کرو EXT2_FT_DIR		2
#اگر_ختم

errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t والد, ext2_ino_t ino,
			غیرعلامتی طویل flags, صحیح link_flags, ثابت حرف *name,
			ext2_ino_t *ret_ino)
{
	ext2_extent_handle_t	handle;
	errcode_t		retval;
	ساخت ext2_inode	والد_inode, inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	حرف			*بلاک = 0;
	صحیح			inline_ڈیٹا = 0;
	صحیح			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * Create a new dir with inline data iff this feature is enabled
	 * and ino >= EXT2_FIRST_INO.
	 */
	اگر ((!ino || ino >= EXT2_پہلا_INO(fs->super)) &&
	    ext2fs_has_feature_inline_ڈیٹا(fs->super))
		inline_ڈیٹا = 1;

	/*
	 * Allocate an inode, if necessary
	 */
	اگر (!ino) {
		retval = ext2fs_new_inode(fs, والد, LINUX_S_IFDIR | 0755,
					  0, &ino);
		اگر (retval)
			جاؤ cleanup;
	}
	اگر (ret_ino)
		*ret_ino = ino;

	/*
	 * Allocate a data block for the directory
	 */
	memset(&inode, 0, حجم(ساخت ext2_inode));
	اگر (!inline_ڈیٹا) {
		retval = ext2fs_new_block2(fs, ext2fs_ڈھونڈو_inode_goal(fs, ino,
								      &inode,
								      0),
					   تہی, &blk);
		اگر (retval)
			جاؤ cleanup;
	}

	/*
	 * Create a scratch template for the directory
	 */
	اگر (inline_ڈیٹا)
		retval = ext2fs_new_dir_inline_ڈیٹا(fs, ino, والد,
						    inode.i_بلاک);
	ورنہ
		retval = ext2fs_new_dir_بلاک(fs, ino, والد, &بلاک);
	اگر (retval)
		جاؤ cleanup;

	/*
	 * Get the parent's inode, if necessary
	 */
	اگر (والد != ino) {
		retval = ext2fs_read_inode(fs, والد, &والد_inode);
		اگر (retval)
			جاؤ cleanup;
	} ورنہ
		memset(&والد_inode, 0, حجم(والد_inode));

	/*
	 * Create the inode structure....
	 */
	inode.i_منوال = LINUX_S_IFDIR | (0777 & ~fs->umask);
	inode.i_uid = inode.i_gid = 0;
	اگر (inline_ڈیٹا) {
		inode.i_flags |= EXT4_INLINE_ڈیٹا_FL;
		inode.i_سائز = EXT4_MIN_INLINE_ڈیٹا_سائز;
	} ورنہ {
		اگر (ext2fs_has_feature_extents(fs->super))
			inode.i_flags |= EXT4_EXTENTS_FL;
		ورنہ
			inode.i_بلاک[0] = blk;
		inode.i_سائز = fs->blocksize;
		ext2fs_iblk_سیٹ(fs, &inode, 1);
	}
	inode.i_flags |= flags & (EXT2_FL_USER_MODIFIABLE & ~EXT4_EXTENTS_FL);
	inode.i_links_گنتی = 2;

	/*
	 * Write out the inode and inode data block.  The inode generation
	 * number is assigned by write_new_inode, which means that the call
	 * to write_dir_block must come after that.
	 */
	retval = ext2fs_write_new_inode(fs, ino, &inode);
	اگر (retval)
		جاؤ cleanup;
	اگر (inline_ڈیٹا) {
		/* init "system.data" for new dir */
		retval = ext2fs_inline_ڈیٹا_init(fs, ino);
	} ورنہ {
		retval = ext2fs_write_dir_block4(fs, blk, بلاک, 0, ino);
		اگر (retval)
			جاؤ cleanup;

		اگر (ext2fs_has_feature_extents(fs->super)) {
			retval = ext2fs_extent_open2(fs, ino, &inode, &handle);
			اگر (retval)
				جاؤ cleanup;
			retval = ext2fs_extent_سیٹ_bmap(handle, 0, blk, 0);
			ext2fs_extent_free(handle);
			اگر (retval)
				جاؤ cleanup;
		}
	}

	/*
	 * Update accounting....
	 */
	اگر (!inline_ڈیٹا)
		ext2fs_بلاک_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 1);
	drop_refcount = 1;

	/*
	 * Link the directory into the filesystem hierarchy
	 */
	اگر (name) {
		retval = ext2fs_lookup(fs, والد, name, لڑی_لمبائی(name), 0,
				       &scratch_ino);
		اگر (!retval) {
			retval = EXT2_ET_DIR_EXISTS;
			name = 0;
			جاؤ cleanup;
		}
		اگر (retval != EXT2_ET_FILE_NOT_ملا)
			جاؤ cleanup;
		retval = ext2fs_link(fs, والد, name, ino,
				     EXT2_FT_DIR | link_flags);
		اگر (retval)
			جاؤ cleanup;
	}

	/*
	 * Update parent inode's counts
	 */
	اگر (والد != ino) {
		/* reload parent inode due to inline data */
		retval = ext2fs_read_inode(fs, والد, &والد_inode);
		اگر (retval)
			جاؤ cleanup;
		ext2fs_inc_nlink(fs, &والد_inode);
		retval = ext2fs_write_inode(fs, والد, &والد_inode);
		اگر (retval)
			جاؤ cleanup;
	}
	drop_refcount = 0;

cleanup:
	اگر (بلاک)
		ext2fs_free_mem(&بلاک);
	اگر (drop_refcount) {
		اگر (!inline_ڈیٹا)
			ext2fs_بلاک_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 1);
	}
	واپس retval;

}

errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t والد, ext2_ino_t ino,
		       ثابت حرف *name)
{
	واپس ext2fs_mkdir2(fs, والد, ino, 0, 0, name, تہی);
}
