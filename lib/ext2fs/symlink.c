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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#جے_بنا_تعریف HAVE_STRNLEN
/*
 * Incredibly, libc5 doesn't appear to have strnlen.  So we have to
 * provide our own.
 */
ساكن صحيح my_strnlen(ثابت حرف * s, صحيح گنتی)
{
	ثابت حرف *cp = s;

	طالما (گنتی-- && *cp)
		cp++;
	أرجع cp - s;
}
#تعریف_کرو strnlen(str, x) my_strnlen((str),(x))
#جے_انت

errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t ماپا, ext2_ino_t ino,
			 ثابت حرف *name, ثابت حرف *نشانہ)
{
	errcode_t		retval;
	بنية ext2_inode	inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	صحيح			fastlink, inlinelink;
	بنانشانی صحيح		نشانہ_len;
	حرف			*بلاک_buf = 0;
	صحيح			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * The Linux kernel doesn't allow for links longer than a block
	 * (counting the NUL terminator)
	 */
	نشانہ_len = strnlen(نشانہ, fs->blocksize + 1);
	إذا (نشانہ_len >= fs->blocksize) {
		retval = EXT2_ET_INVALID_دلیل;
		اذهب cleanup;
	}

	/*
	 * Allocate a data block for slow links
	 */
	retval = ext2fs_get_mem(fs->blocksize, &بلاک_buf);
	إذا (retval)
		اذهب cleanup;
	memset(بلاک_buf, 0, fs->blocksize);
	strncpy(بلاک_buf, نشانہ, fs->blocksize);

	memset(&inode, 0, حجم(بنية ext2_inode));
	fastlink = (نشانہ_len < حجم(inode.i_بلاک));
	إذا (!fastlink) {
		retval = ext2fs_new_block2(fs, ext2fs_لبھو_inode_goal(fs, ino,
								      &inode,
								      0),
					   عدم, &blk);
		إذا (retval)
			اذهب cleanup;
	}

	/*
	 * Allocate an inode, if necessary
	 */
	إذا (!ino) {
		retval = ext2fs_new_inode(fs, ماپا, LINUX_S_IFLNK | 0755,
					  0, &ino);
		إذا (retval)
			اذهب cleanup;
	}

	/*
	 * Create the inode structure....
	 */
	inode.i_منوال = LINUX_S_IFLNK | 0777;
	inode.i_uid = inode.i_gid = 0;
	inode.i_links_گنتی = 1;
	ext2fs_inode_ناپ_سیٹ(fs, &inode, نشانہ_len);
	/* The time fields are set by ext2fs_write_new_inode() */

	inlinelink = !fastlink && ext2fs_has_feature_inline_انکڑے(fs->super);
	إذا (fastlink) {
		/* Fast symlinks, target stored in inode */
		strcpy((حرف *)&inode.i_بلاک, نشانہ);
	} وإلا إذا (inlinelink) {
		/* Try inserting an inline data symlink */
		inode.i_flags |= EXT4_INLINE_انکڑے_FL;
		retval = ext2fs_write_new_inode(fs, ino, &inode);
		إذا (retval)
			اذهب cleanup;
		retval = ext2fs_inline_انکڑے_سیٹ(fs, ino, &inode, بلاک_buf,
						نشانہ_len);
		إذا (retval) {
			inode.i_flags &= ~EXT4_INLINE_انکڑے_FL;
			inlinelink = 0;
			اذهب need_بلاک;
		}
		retval = ext2fs_read_inode(fs, ino, &inode);
		إذا (retval)
			اذهب cleanup;
	} وإلا {
need_بلاک:
		/* Slow symlinks, target stored in the first block */
		ext2fs_iblk_سیٹ(fs, &inode, 1);
		إذا (ext2fs_has_feature_extents(fs->super)) {
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
	إذا (inlinelink)
		retval = ext2fs_write_inode(fs, ino, &inode);
	وإلا
		retval = ext2fs_write_new_inode(fs, ino, &inode);
	إذا (retval)
		اذهب cleanup;

	إذا (!fastlink && !inlinelink) {
		retval = ext2fs_bmap2(fs, ino, &inode, عدم, BMAP_سیٹ, 0, عدم,
				      &blk);
		إذا (retval)
			اذهب cleanup;

		retval = io_channel_write_blk64(fs->io, blk, 1, بلاک_buf);
		إذا (retval)
			اذهب cleanup;
	}

	/*
	 * Update accounting....
	 */
	إذا (!fastlink && !inlinelink)
		ext2fs_بلاک_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	drop_refcount = 1;

	/*
	 * Link the symlink into the filesystem hierarchy
	 */
	إذا (name) {
		retval = ext2fs_lookup(fs, ماپا, name, لڑی_لمبائی(name), 0,
				       &scratch_ino);
		إذا (!retval) {
			retval = EXT2_ET_FILE_EXISTS;
			اذهب cleanup;
		}
		إذا (retval != EXT2_ET_FILE_NOT_لبھیا)
			اذهب cleanup;
		retval = ext2fs_link(fs, ماپا, name, ino, EXT2_FT_SYMLINK);
		إذا (retval)
			اذهب cleanup;
	}
	drop_refcount = 0;

cleanup:
	إذا (بلاک_buf)
		ext2fs_free_mem(&بلاک_buf);
	إذا (drop_refcount) {
		إذا (!fastlink && !inlinelink)
			ext2fs_بلاک_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 0);
	}
	أرجع retval;
}

/*
 * Test whether an inode is a fast symlink.
 *
 * A fast symlink has its symlink data stored in inode->i_block.
 */
صحيح ext2fs_is_fast_symlink(بنية ext2_inode *inode)
{
	أرجع LINUX_S_ISLNK(inode->i_منوال) && EXT2_I_ناپ(inode) &&
	       EXT2_I_ناپ(inode) < حجم(inode->i_بلاک);
}
