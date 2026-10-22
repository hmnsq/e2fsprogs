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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_STRNLEN
/*
 * Incredibly, libc5 doesn't appear to have strnlen.  So we have to
 * provide our own.
 */
ਸਥਿਰ ਪੂਰਨ my_strnlen(ਅਚਲ ਅੱਖਰ * s, ਪੂਰਨ ਗਿਣਤੀ_2)
{
	ਅਚਲ ਅੱਖਰ *cp = s;

	ਜਦੋਂ (ਗਿਣਤੀ_2-- && *cp)
		cp++;
	ਵਾਪਸ cp - s;
}
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ strnlen(str, x) my_strnlen((str),(x))
#ਜੇ_ਅੰਤ

errcode_t ext2fs_symlink(ext2_filsys fs, ext2_ino_t ਮਾਪਾ, ext2_ino_t ino,
			 ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *ਨਿਸ਼ਾਨਾ)
{
	errcode_t		retval;
	ਬਣਤਰ ext2_inode	inode;
	ext2_ino_t		scratch_ino;
	blk64_t			blk;
	ਪੂਰਨ			fastlink, inlinelink;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		ਨਿਸ਼ਾਨਾ_len;
	ਅੱਖਰ			*ਖੰਡ_buf = 0;
	ਪੂਰਨ			drop_refcount = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	/*
	 * The Linux kernel doesn't allow for links longer than a block
	 * (counting the NUL terminator)
	 */
	ਨਿਸ਼ਾਨਾ_len = strnlen(ਨਿਸ਼ਾਨਾ, fs->blocksize + 1);
	ਜੇ (ਨਿਸ਼ਾਨਾ_len >= fs->blocksize) {
		retval = EXT2_ET_INVALID_ਦਲੀਲ;
		ਜਾਓ cleanup;
	}

	/*
	 * Allocate a data block for slow links
	 */
	retval = ext2fs_get_mem(fs->blocksize, &ਖੰਡ_buf);
	ਜੇ (retval)
		ਜਾਓ cleanup;
	memset(ਖੰਡ_buf, 0, fs->blocksize);
	strncpy(ਖੰਡ_buf, ਨਿਸ਼ਾਨਾ, fs->blocksize);

	memset(&inode, 0, ਆਕਾਰ(ਬਣਤਰ ext2_inode));
	fastlink = (ਨਿਸ਼ਾਨਾ_len < ਆਕਾਰ(inode.i_ਖੰਡ));
	ਜੇ (!fastlink) {
		retval = ext2fs_new_block2(fs, ext2fs_ਲੱਭੋ_inode_goal(fs, ino,
								      &inode,
								      0),
					   ਨਿਰਾ, &blk);
		ਜੇ (retval)
			ਜਾਓ cleanup;
	}

	/*
	 * Allocate an inode, if necessary
	 */
	ਜੇ (!ino) {
		retval = ext2fs_new_inode(fs, ਮਾਪਾ, LINUX_S_IFLNK | 0755,
					  0, &ino);
		ਜੇ (retval)
			ਜਾਓ cleanup;
	}

	/*
	 * Create the inode structure....
	 */
	inode.i_ਬਹੁਲਕ = LINUX_S_IFLNK | 0777;
	inode.i_uid = inode.i_gid = 0;
	inode.i_links_ਗਿਣਤੀ = 1;
	ext2fs_inode_ਮਾਪ_ਸੈੱਟ(fs, &inode, ਨਿਸ਼ਾਨਾ_len);
	/* The time fields are set by ext2fs_write_new_inode() */

	inlinelink = !fastlink && ext2fs_has_feature_inline_ਅੰਕੜੇ(fs->super);
	ਜੇ (fastlink) {
		/* Fast symlinks, target stored in inode */
		strcpy((ਅੱਖਰ *)&inode.i_ਖੰਡ, ਨਿਸ਼ਾਨਾ);
	} ਨਹੀਂਤਾਂ ਜੇ (inlinelink) {
		/* Try inserting an inline data symlink */
		inode.i_flags |= EXT4_INLINE_ਅੰਕੜੇ_FL;
		retval = ext2fs_write_new_inode(fs, ino, &inode);
		ਜੇ (retval)
			ਜਾਓ cleanup;
		retval = ext2fs_inline_ਅੰਕੜੇ_ਸੈੱਟ(fs, ino, &inode, ਖੰਡ_buf,
						ਨਿਸ਼ਾਨਾ_len);
		ਜੇ (retval) {
			inode.i_flags &= ~EXT4_INLINE_ਅੰਕੜੇ_FL;
			inlinelink = 0;
			ਜਾਓ need_ਖੰਡ;
		}
		retval = ext2fs_read_inode(fs, ino, &inode);
		ਜੇ (retval)
			ਜਾਓ cleanup;
	} ਨਹੀਂਤਾਂ {
need_ਖੰਡ:
		/* Slow symlinks, target stored in the first block */
		ext2fs_iblk_ਸੈੱਟ(fs, &inode, 1);
		ਜੇ (ext2fs_has_feature_extents(fs->super)) {
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
	ਜੇ (inlinelink)
		retval = ext2fs_write_inode(fs, ino, &inode);
	ਨਹੀਂਤਾਂ
		retval = ext2fs_write_new_inode(fs, ino, &inode);
	ਜੇ (retval)
		ਜਾਓ cleanup;

	ਜੇ (!fastlink && !inlinelink) {
		retval = ext2fs_bmap2(fs, ino, &inode, ਨਿਰਾ, BMAP_ਸੈੱਟ, 0, ਨਿਰਾ,
				      &blk);
		ਜੇ (retval)
			ਜਾਓ cleanup;

		retval = io_channel_write_blk64(fs->io, blk, 1, ਖੰਡ_buf);
		ਜੇ (retval)
			ਜਾਓ cleanup;
	}

	/*
	 * Update accounting....
	 */
	ਜੇ (!fastlink && !inlinelink)
		ext2fs_ਖੰਡ_alloc_stats2(fs, blk, +1);
	ext2fs_inode_alloc_stats2(fs, ino, +1, 0);
	drop_refcount = 1;

	/*
	 * Link the symlink into the filesystem hierarchy
	 */
	ਜੇ (name) {
		retval = ext2fs_lookup(fs, ਮਾਪਾ, name, ਲੜੀ_ਲੰਬਾਈ(name), 0,
				       &scratch_ino);
		ਜੇ (!retval) {
			retval = EXT2_ET_FILE_EXISTS;
			ਜਾਓ cleanup;
		}
		ਜੇ (retval != EXT2_ET_FILE_NOT_ਮਿਲਿਆ)
			ਜਾਓ cleanup;
		retval = ext2fs_link(fs, ਮਾਪਾ, name, ino, EXT2_FT_SYMLINK);
		ਜੇ (retval)
			ਜਾਓ cleanup;
	}
	drop_refcount = 0;

cleanup:
	ਜੇ (ਖੰਡ_buf)
		ext2fs_free_mem(&ਖੰਡ_buf);
	ਜੇ (drop_refcount) {
		ਜੇ (!fastlink && !inlinelink)
			ext2fs_ਖੰਡ_alloc_stats2(fs, blk, -1);
		ext2fs_inode_alloc_stats2(fs, ino, -1, 0);
	}
	ਵਾਪਸ retval;
}

/*
 * Test whether an inode is a fast symlink.
 *
 * A fast symlink has its symlink data stored in inode->i_block.
 */
ਪੂਰਨ ext2fs_is_fast_symlink(ਬਣਤਰ ext2_inode *inode)
{
	ਵਾਪਸ LINUX_S_ISLNK(inode->i_ਬਹੁਲਕ) && EXT2_I_ਮਾਪ(inode) &&
	       EXT2_I_ਮਾਪ(inode) < ਆਕਾਰ(inode->i_ਖੰਡ);
}
