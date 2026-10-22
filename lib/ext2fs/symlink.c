/*
 * symlink.c --- make a symlink in the filesystem, based on mkdir.c
 *
 * Copyright (c) 2012, Intel Corporation.
 * All Rights Reserved.
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

#నిర్వచితం_కాకపోతే HAVE_STRNLEN
/*
 * Incredibly, libc5 doesn't appear to have strnlen.  So we have to
 * provide our own.
 */
స్థిర పూర్ణాంకం my_strnlen(స్థిరరాశి అక్షరం * s, పూర్ణాంకం లెక్క)
{
	స్థిరరాశి అక్షరం *cp = s;

	అయితే (లెక్క-- && *cp)
		cp++;
	తిరిగి cp - s;
}
#నిర్వచించు strnlen(str, x) my_strnlen((str),(x))
#అయితే_ముగింపు

errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t మాతృ, ext2_ino_t ino,
			 స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *లక్ష్యం)
{
	errcode_t		retval;
	నిర్మాణం ext2_inode	inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	పూర్ణాంకం			fastlink, inlinelink;
	గుర్తులేని పూర్ణాంకం		లక్ష్యం_len;
	అక్షరం			*ఖండం_buf = 0;
	పూర్ణాంకం			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * The Linux kernel doesn't allow for links longer than a block
	 * (counting the NUL terminator)
	 */
	లక్ష్యం_len = strnlen(లక్ష్యం, fs->blocksize + 1);
	ఒకవేళ (లక్ష్యం_len >= fs->blocksize) {
		retval = EXT2_ET_INVALID_వాదం;
		వెళ్ళు cleanup;
	}

	/*
	 * Allocate a data block for slow links
	 */
	retval = ext2fs_get_mem(fs->blocksize, &ఖండం_buf);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;
	memset(ఖండం_buf, 0, fs->blocksize);
	strncpy(ఖండం_buf, లక్ష్యం, fs->blocksize);

	memset(&inode, 0, పరిమాణం(నిర్మాణం ext2_inode));
	fastlink = (లక్ష్యం_len < పరిమాణం(inode.i_ఖండం));
	ఒకవేళ (!fastlink) {
		retval = ext2fs_new_block2(fs, ext2fs_కనుగొను_inode_goal(fs, ino,
								      &inode,
								      0),
					   ఖాళీ, &blk);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
	}

	/*
	 * Allocate an inode, if necessary
	 */
	ఒకవేళ (!ino) {
		retval = ext2fs_new_inode(fs, మాతృ, LINUX_S_IFLNK | 0755,
					  0, &ino);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
	}

	/*
	 * Create the inode structure....
	 */
	inode.i_బహుళకం = LINUX_S_IFLNK | 0777;
	inode.i_uid = inode.i_gid = 0;
	inode.i_links_లెక్క = 1;
	ext2fs_inode_కొలత_సమితి(fs, &inode, లక్ష్యం_len);
	/* The time fields are set by ext2fs_write_new_inode() */

	inlinelink = !fastlink && ext2fs_has_feature_inline_దత్తాంశం(fs->super);
	ఒకవేళ (fastlink) {
		/* Fast symlinks, target stored in inode */
		strcpy((అక్షరం *)&inode.i_ఖండం, లక్ష్యం);
	} లేకపోతే ఒకవేళ (inlinelink) {
		/* Try inserting an inline data symlink */
		inode.i_flags |= EXT4_INLINE_దత్తాంశం_FL;
		retval = ext2fs_write_new_inode(fs, ino, &inode);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
		retval = ext2fs_inline_దత్తాంశం_సమితి(fs, ino, &inode, ఖండం_buf,
						లక్ష్యం_len);
		ఒకవేళ (retval) {
			inode.i_flags &= ~EXT4_INLINE_దత్తాంశం_FL;
			inlinelink = 0;
			వెళ్ళు need_ఖండం;
		}
		retval = ext2fs_read_inode(fs, ino, &inode);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
	} లేకపోతే {
need_ఖండం:
		/* Slow symlinks, target stored in the first block */
		ext2fs_iblk_సమితి(fs, &inode, 1);
		ఒకవేళ (ext2fs_has_feature_extents(fs->super)) {
			/*
			 * The extent bmap is setup after the inode and block
			 * have been written out below.
			 */
			inode.i_flags |= EXT4_EXTENTS_FL;
		}
	}

	/*
	 * Write out the inode and inode data block.  The inode generation
	 * number is assigned by write_new_inode, which means that the
	 * operations using ino must come after it.
	 */
	ఒకవేళ (inlinelink)
		retval = ext2fs_write_inode(fs, ino, &inode);
	లేకపోతే
		retval = ext2fs_write_new_inode(fs, ino, &inode);
	ఒకవేళ (retval)
		వెళ్ళు cleanup;

	ఒకవేళ (!fastlink && !inlinelink) {
		retval = ext2fs_bmap2(fs, ino, &inode, ఖాళీ, BMAP_సమితి, 0, ఖాళీ,
				      &blk);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;

		retval = io_channel_write_blk64(fs->io, blk, 1, ఖండం_buf);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
	}

	/*
	 * Update accounting....
	 */
	ఒకవేళ (!fastlink && !inlinelink)
		ext2fs_ఖండం_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	drop_refcount = 1;

	/*
	 * Link the symlink into the filesystem hierarchy
	 */
	ఒకవేళ (name) {
		retval = ext2fs_lookup(fs, మాతృ, name, అక్షరశ్రేణి_పొడవు(name), 0,
				       &scratch_ino);
		ఒకవేళ (!retval) {
			retval = EXT2_ET_FILE_EXISTS;
			వెళ్ళు cleanup;
		}
		ఒకవేళ (retval != EXT2_ET_FILE_NOT_దొరికింది)
			వెళ్ళు cleanup;
		retval = ext2fs_link(fs, మాతృ, name, ino, EXT2_FT_SYMLINK);
		ఒకవేళ (retval)
			వెళ్ళు cleanup;
	}
	drop_refcount = 0;

cleanup:
	ఒకవేళ (ఖండం_buf)
		ext2fs_free_mem(&ఖండం_buf);
	ఒకవేళ (drop_refcount) {
		ఒకవేళ (!fastlink && !inlinelink)
			ext2fs_ఖండం_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 0);
	}
	తిరిగి retval;
}

/*
 * Test whether an inode is a fast symlink.
 *
 * A fast symlink has its symlink data stored in inode->i_block.
 */
పూర్ణాంకం ext2fs_is_fast_symlink(నిర్మాణం ext2_inode *inode)
{
	తిరిగి LINUX_S_ISLNK(inode->i_బహుళకం) && EXT2_I_కొలత(inode) &&
	       EXT2_I_కొలత(inode) < పరిమాణం(inode->i_ఖండం);
}
