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

#که_نه_تعریف_شوی HAVE_STRNLEN
/*
 * Incredibly, libc5 doesn't appear to have strnlen.  So we have to
 * provide our own.
 */
ساكن صحيح my_strnlen(ثابت حرف * s, صحيح شمېر)
{
	ثابت حرف *cp = s;

	طالما (شمېر-- && *cp)
		cp++;
	أرجع cp - s;
}
#تعریف_کړه strnlen(str, x) my_strnlen((str),(x))
#د_که_پای

errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t پلار, ext2_ino_t ino,
			 ثابت حرف *name, ثابت حرف *موخه)
{
	errcode_t		retval;
	بنية ext2_inode	inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	صحيح			fastlink, inlinelink;
	بینښه صحيح		موخه_len;
	حرف			*بلاک_buf = 0;
	صحيح			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * The Linux kernel doesn't allow for links longer than a block
	 * (counting the NUL terminator)
	 */
	موخه_len = strnlen(موخه, fs->blocksize + 1);
	إذا (موخه_len >= fs->blocksize) {
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
	strncpy(بلاک_buf, موخه, fs->blocksize);

	memset(&inode, 0, حجم(بنية ext2_inode));
	fastlink = (موخه_len < حجم(inode.i_بلاک));
	إذا (!fastlink) {
		retval = ext2fs_new_block2(fs, ext2fs_پیدا_کړه_inode_goal(fs, ino,
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
		retval = ext2fs_new_inode(fs, پلار, LINUX_S_IFLNK | 0755,
					  0, &ino);
		إذا (retval)
			اذهب cleanup;
	}

	/*
	 * Create the inode structure....
	 */
	inode.i_ډېرپېښ = LINUX_S_IFLNK | 0777;
	inode.i_uid = inode.i_gid = 0;
	inode.i_links_شمېر = 1;
	ext2fs_inode_اندازه_سېټ(fs, &inode, موخه_len);
	/* The time fields are set by ext2fs_write_new_inode() */

	inlinelink = !fastlink && ext2fs_has_feature_inline_معلومات(fs->super);
	إذا (fastlink) {
		/* Fast symlinks, target stored in inode */
		strcpy((حرف *)&inode.i_بلاک, موخه);
	} وإلا إذا (inlinelink) {
		/* Try inserting an inline data symlink */
		inode.i_flags |= EXT4_INLINE_معلومات_FL;
		retval = ext2fs_write_new_inode(fs, ino, &inode);
		إذا (retval)
			اذهب cleanup;
		retval = ext2fs_inline_معلومات_سېټ(fs, ino, &inode, بلاک_buf,
						موخه_len);
		إذا (retval) {
			inode.i_flags &= ~EXT4_INLINE_معلومات_FL;
			inlinelink = 0;
			اذهب need_بلاک;
		}
		retval = ext2fs_read_inode(fs, ino, &inode);
		إذا (retval)
			اذهب cleanup;
	} وإلا {
need_بلاک:
		/* Slow symlinks, target stored in the first block */
		ext2fs_iblk_سېټ(fs, &inode, 1);
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
		retval = ext2fs_bmap2(fs, ino, &inode, عدم, BMAP_سېټ, 0, عدم,
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
		retval = ext2fs_lookup(fs, پلار, name, مزي_اوږدوالی(name), 0,
				       &scratch_ino);
		إذا (!retval) {
			retval = EXT2_ET_FILE_EXISTS;
			اذهب cleanup;
		}
		إذا (retval != EXT2_ET_FILE_NOT_وموندل)
			اذهب cleanup;
		retval = ext2fs_link(fs, پلار, name, ino, EXT2_FT_SYMLINK);
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
	أرجع LINUX_S_ISLNK(inode->i_ډېرپېښ) && EXT2_I_اندازه(inode) &&
	       EXT2_I_اندازه(inode) < حجم(inode->i_بلاک);
}
