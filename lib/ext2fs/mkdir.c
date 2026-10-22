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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "ext2fsP.h"

#నిర్వచితం_కాకపోతే EXT2_FT_DIR
#నిర్వచించు EXT2_FT_DIR		2
#అయితే_ముగింపు

errcode_t ext2fs_mkdir2(ext2_filsys fs, ext2_ino_t మాతృ, ext2_ino_t ino,
			గుర్తులేని దీర్ఘ flags, పూర్ణాంకం link_flags, స్థిరరాశి అక్షరం *name,
			ext2_ino_t *ret_ino)
{
	ext2_extent_handle_t	handle;
	errcode_t		retval;
	నిర్మాణం ext2_inode	మాతృ_inode, inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	అక్షరం			*ఖండం = 0;
	పూర్ణాంకం			inline_దత్తాంశం = 0;
	పూర్ణాంకం			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * Create a new dir with inline data iff this feature is enabled
	 * and ino >= EXT2_FIRST_INO.
	 */
	ఒకవేళ ((!ino || ino >= EXT2_మొదటి_INO(fs->super)) &&
	    ext2fs_has_feature_inline_దత్తాంశం(fs->super))
		inline_దత్తాంశం = 1;

	/*
	 * Allocate an inode, if necessary
	 */
	ఒకవేళ (!ino) {
		retval = ext2fs_new_inode(fs, మాతృ, LINUX_S_IFDIR | 0755,
					  0, &ino);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
	}
	ఒకవేళ (ret_ino)
		*ret_ino = ino;

	/*
	 * Allocate a data block for the directory
	 */
	memset(&inode, 0, పరిమాణం(నిర్మాణం ext2_inode));
	ఒకవేళ (!inline_దత్తాంశం) {
		retval = ext2fs_new_block2(fs, ext2fs_కనుగొను_inode_goal(fs, ino,
								      &inode,
								      0),
					   ఖాళీ, &blk);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
	}

	/*
	 * Create a scratch template for the directory
	 */
	ఒకవేళ (inline_దత్తాంశం)
		retval = ext2fs_new_dir_inline_దత్తాంశం(fs, ino, మాతృ,
						    inode.i_ఖండం);
	లేకపోతే
		retval = ext2fs_new_dir_ఖండం(fs, ino, మాతృ, &ఖండం);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;

	/*
	 * Get the parent's inode, if necessary
	 */
	ఒకవేళ (మాతృ != ino) {
		retval = ext2fs_read_inode(fs, మాతృ, &మాతృ_inode);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
	} లేకపోతే
		memset(&మాతృ_inode, 0, పరిమాణం(మాతృ_inode));

	/*
	 * Create the inode structure....
	 */
	inode.i_బహుళకం = LINUX_S_IFDIR | (0777 & ~fs->umask);
	inode.i_uid = inode.i_gid = 0;
	ఒకవేళ (inline_దత్తాంశం) {
		inode.i_flags |= EXT4_INLINE_దత్తాంశం_FL;
		inode.i_కొలత = EXT4_MIN_INLINE_దత్తాంశం_కొలత;
	} లేకపోతే {
		ఒకవేళ (ext2fs_has_feature_extents(fs->super))
			inode.i_flags |= EXT4_EXTENTS_FL;
		లేకపోతే
			inode.i_ఖండం[0] = blk;
		inode.i_కొలత = fs->blocksize;
		ext2fs_iblk_సమితి(fs, &inode, 1);
	}
	inode.i_flags |= flags & (EXT2_FL_USER_MODIFIABLE & ~EXT4_EXTENTS_FL);
	inode.i_links_లెక్క = 2;

	/*
	 * Write out the inode and inode data block.  The inode generation
	 * number is assigned by write_new_inode, which means that the call
	 * to write_dir_block must come after that.
	 */
	retval = ext2fs_write_new_inode(fs, ino, &inode);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;
	ఒకవేళ (inline_దత్తాంశం) {
		/* init "system.data" for new dir */
		retval = ext2fs_inline_దత్తాంశం_init(fs, ino);
	} లేకపోతే {
		retval = ext2fs_write_dir_block4(fs, blk, ఖండం, 0, ino);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;

		ఒకవేళ (ext2fs_has_feature_extents(fs->super)) {
			retval = ext2fs_extent_open2(fs, ino, &inode, &handle);
			ఒకవేళ (retval)
				వెళ్ళు cleanup;
			retval = ext2fs_extent_సమితి_bmap(handle, 0, blk, 0);
			ext2fs_extent_free(handle);
			ఒకవేళ (retval)
				వెళ్ళు cleanup;
		}
	}

	/*
	 * Update accounting....
	 */
	ఒకవేళ (!inline_దత్తాంశం)
		ext2fs_ఖండం_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 1);
	drop_refcount = 1;

	/*
	 * Link the directory into the filesystem hierarchy
	 */
	ఒకవేళ (name) {
		retval = ext2fs_lookup(fs, మాతృ, name, అక్షరశ్రేణి_పొడవు(name), 0,
				       &scratch_ino);
		ఒకవేళ (!retval) {
			retval = EXT2_ET_DIR_EXISTS;
			name = 0;
			వెళ్ళు cleanup;
		}
		ఒకవేళ (retval != EXT2_ET_FILE_NOT_దొరికింది)
			వెళ్ళు cleanup;
		retval = ext2fs_link(fs, మాతృ, name, ino,
				     EXT2_FT_DIR | link_flags);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
	}

	/*
	 * Update parent inode's counts
	 */
	ఒకవేళ (మాతృ != ino) {
		/* reload parent inode due to inline data */
		retval = ext2fs_read_inode(fs, మాతృ, &మాతృ_inode);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
		ext2fs_inc_nlink(fs, &మాతృ_inode);
		retval = ext2fs_write_inode(fs, మాతృ, &మాతృ_inode);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
	}
	drop_refcount = 0;

cleanup:
	ఒకవేళ (ఖండం)
		ext2fs_free_mem(&ఖండం);
	ఒకవేళ (drop_refcount) {
		ఒకవేళ (!inline_దత్తాంశం)
			ext2fs_ఖండం_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 1);
	}
	తిరిగి retval;

}

errcode_t ext2fs_mkdir(ext2_filsys fs, ext2_ino_t మాతృ, ext2_ino_t ino,
		       స్థిరరాశి అక్షరం *name)
{
	తిరిగి ext2fs_mkdir2(fs, మాతృ, ino, 0, 0, name, ఖాళీ);
}
