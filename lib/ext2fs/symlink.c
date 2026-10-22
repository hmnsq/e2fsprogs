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

#اگر_غیر_تعریف_شدہ HAVE_STRNLEN
/*
 * Incredibly, libc5 doesn't appear to have strnlen.  So we have to
 * provide our own.
 */
ساکن صحیح my_strnlen(ثابت حرف * s, صحیح گنتی)
{
	ثابت حرف *cp = s;

	جب (گنتی-- && *cp)
		cp++;
	واپس cp - s;
}
#تعریف_کرو strnlen(str, x) my_strnlen((str),(x))
#اگر_ختم

errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t والد, ext2_ino_t ino,
			 ثابت حرف *name, ثابت حرف *ہدف)
{
	errcode_t		retval;
	ساخت ext2_inode	inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	صحیح			fastlink, inlinelink;
	غیرعلامتی صحیح		ہدف_len;
	حرف			*بلاک_buf = 0;
	صحیح			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * The Linux kernel doesn't allow for links longer than a block
	 * (counting the NUL terminator)
	 */
	ہدف_len = strnlen(ہدف, fs->blocksize + 1);
	اگر (ہدف_len >= fs->blocksize) {
		retval = EXT2_ET_INVALID_دلیل;
		جاؤ cleanup;
	}

	/*
	 * Allocate a data block for slow links
	 */
	retval = ext2fs_get_mem(fs->blocksize, &بلاک_buf);
	اگر (retval)
		جاؤ cleanup;
	memset(بلاک_buf, 0, fs->blocksize);
	strncpy(بلاک_buf, ہدف, fs->blocksize);

	memset(&inode, 0, حجم(ساخت ext2_inode));
	fastlink = (ہدف_len < حجم(inode.i_بلاک));
	اگر (!fastlink) {
		retval = ext2fs_new_block2(fs, ext2fs_ڈھونڈو_inode_goal(fs, ino,
								      &inode,
								      0),
					   تہی, &blk);
		اگر (retval)
			جاؤ cleanup;
	}

	/*
	 * Allocate an inode, if necessary
	 */
	اگر (!ino) {
		retval = ext2fs_new_inode(fs, والد, LINUX_S_IFLNK | 0755,
					  0, &ino);
		اگر (retval)
			جاؤ cleanup;
	}

	/*
	 * Create the inode structure....
	 */
	inode.i_منوال = LINUX_S_IFLNK | 0777;
	inode.i_uid = inode.i_gid = 0;
	inode.i_links_گنتی = 1;
	ext2fs_inode_سائز_سیٹ(fs, &inode, ہدف_len);
	/* The time fields are set by ext2fs_write_new_inode() */

	inlinelink = !fastlink && ext2fs_has_feature_inline_ڈیٹا(fs->super);
	اگر (fastlink) {
		/* Fast symlinks, target stored in inode */
		strcpy((حرف *)&inode.i_بلاک, ہدف);
	} ورنہ اگر (inlinelink) {
		/* Try inserting an inline data symlink */
		inode.i_flags |= EXT4_INLINE_ڈیٹا_FL;
		retval = ext2fs_write_new_inode(fs, ino, &inode);
		اگر (retval)
			جاؤ cleanup;
		retval = ext2fs_inline_ڈیٹا_سیٹ(fs, ino, &inode, بلاک_buf,
						ہدف_len);
		اگر (retval) {
			inode.i_flags &= ~EXT4_INLINE_ڈیٹا_FL;
			inlinelink = 0;
			جاؤ need_بلاک;
		}
		retval = ext2fs_read_inode(fs, ino, &inode);
		اگر (retval)
			جاؤ cleanup;
	} ورنہ {
need_بلاک:
		/* Slow symlinks, target stored in the first block */
		ext2fs_iblk_سیٹ(fs, &inode, 1);
		اگر (ext2fs_has_feature_extents(fs->super)) {
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
	اگر (inlinelink)
		retval = ext2fs_write_inode(fs, ino, &inode);
	ورنہ
		retval = ext2fs_write_new_inode(fs, ino, &inode);
	اگر (retval)
		جاؤ cleanup;

	اگر (!fastlink && !inlinelink) {
		retval = ext2fs_bmap2(fs, ino, &inode, تہی, BMAP_سیٹ, 0, تہی,
				      &blk);
		اگر (retval)
			جاؤ cleanup;

		retval = io_channel_write_blk64(fs->io, blk, 1, بلاک_buf);
		اگر (retval)
			جاؤ cleanup;
	}

	/*
	 * Update accounting....
	 */
	اگر (!fastlink && !inlinelink)
		ext2fs_بلاک_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	drop_refcount = 1;

	/*
	 * Link the symlink into the filesystem hierarchy
	 */
	اگر (name) {
		retval = ext2fs_lookup(fs, والد, name, لڑی_لمبائی(name), 0,
				       &scratch_ino);
		اگر (!retval) {
			retval = EXT2_ET_FILE_EXISTS;
			جاؤ cleanup;
		}
		اگر (retval != EXT2_ET_FILE_NOT_ملا)
			جاؤ cleanup;
		retval = ext2fs_link(fs, والد, name, ino, EXT2_FT_SYMLINK);
		اگر (retval)
			جاؤ cleanup;
	}
	drop_refcount = 0;

cleanup:
	اگر (بلاک_buf)
		ext2fs_free_mem(&بلاک_buf);
	اگر (drop_refcount) {
		اگر (!fastlink && !inlinelink)
			ext2fs_بلاک_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 0);
	}
	واپس retval;
}

/*
 * Test whether an inode is a fast symlink.
 *
 * A fast symlink has its symlink data stored in inode->i_block.
 */
صحیح ext2fs_is_fast_symlink(ساخت ext2_inode *inode)
{
	واپس LINUX_S_ISLNK(inode->i_منوال) && EXT2_I_سائز(inode) &&
	       EXT2_I_سائز(inode) < حجم(inode->i_بلاک);
}
