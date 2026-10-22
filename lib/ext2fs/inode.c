/*
 * inode.c --- utility routines to read and write inodes
 *
 * Copyright (C) 1993, 1994, 1995, 1996, 1997 Theodore Ts'o.
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
#ਜੇ HAVE_ERRNO_H
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "e2image.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IBLOCK_ਸਥਿਤੀ_CSUMS_OK	1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IBLOCK_ਸਥਿਤੀ_INSANE	2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜਾਂਚ_ਖੰਡ_ਸਥਿਤੀ(ਜਾਂਚ)	((ਜਾਂਚ)->ਆਰਜ਼ੀ_ਬਫਰ + (ਜਾਂਚ)->inode_ਮਾਪ)

ਬਣਤਰ ext2_struct_inode_ਜਾਂਚ {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t		current_inode;
	blk64_t			current_ਖੰਡ;
	dgrp_t			current_ਸਮੂਹ;
	ext2_ino_t		inodes_ਖੱਬਾ;
	blk_t			ਖੰਡਾਂ_ਖੱਬਾ;
	dgrp_t			groups_ਖੱਬਾ;
	blk_t			inode_ਬਫਰ_ਖੰਡਾਂ;
	ਅੱਖਰ *			inode_ਬਫਰ;
	ਪੂਰਨ			inode_ਮਾਪ;
	ਅੱਖਰ *			ptr;
	ਪੂਰਨ			ਬਾਈਟਾਂ_ਖੱਬਾ;
	ਅੱਖਰ			*ਆਰਜ਼ੀ_ਬਫਰ;
	errcode_t		(*ਪੂਰਾ_ਸਮੂਹ)(ext2_filsys fs,
					      ext2_inode_ਜਾਂਚ ਜਾਂਚ,
					      dgrp_t ਸਮੂਹ,
					      ਖਾਲੀ * priv_ਅੰਕੜੇ);
	ਖਾਲੀ *			ਪੂਰਾ_ਸਮੂਹ_ਅੰਕੜੇ;
	ਪੂਰਨ			bad_ਖੰਡ_ptr;
	ਪੂਰਨ			ਜਾਂਚ_flags;
	ਪੂਰਨ			reserved[6];
};

/*
 * This routine flushes the icache, if it exists.
 */
errcode_t ext2fs_flush_icache(ext2_filsys fs)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ	i;

	ਜੇ (!fs->icache)
		ਵਾਪਸ 0;

	ਲਈ (i=0; i < fs->icache->cache_ਮਾਪ; i++)
		fs->icache->cache[i].ino = 0;

	fs->icache->ਬਫਰ_blk = 0;
	ਵਾਪਸ 0;
}

/*
 * Free the inode cache structure
 */
ਖਾਲੀ ext2fs_free_inode_cache(ਬਣਤਰ ext2_inode_cache *icache)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ i;

	ਜੇ (--icache->refcount)
		ਵਾਪਸ;
	ਜੇ (icache->ਬਫਰ)
		ext2fs_free_mem(&icache->ਬਫਰ);
	ਲਈ (i = 0; i < icache->cache_ਮਾਪ; i++)
		ext2fs_free_mem(&icache->cache[i].inode);
	ਜੇ (icache->cache)
		ext2fs_free_mem(&icache->cache);
	icache->ਬਫਰ_blk = 0;
	ext2fs_free_mem(&icache);
}

errcode_t ext2fs_create_inode_cache(ext2_filsys fs, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cache_ਮਾਪ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ	i;
	errcode_t	retval;

	ਜੇ (fs->icache)
		ਵਾਪਸ 0;
	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ ext2_inode_cache), &fs->icache);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	memset(fs->icache, 0, ਆਕਾਰ(ਬਣਤਰ ext2_inode_cache));
	retval = ext2fs_get_mem(fs->blocksize, &fs->icache->ਬਫਰ);
	ਜੇ (retval)
		ਜਾਓ errout;

	fs->icache->ਬਫਰ_blk = 0;
	fs->icache->cache_ਆਖਰੀ = -1;
	fs->icache->cache_ਮਾਪ = cache_ਮਾਪ;
	fs->icache->refcount = 1;
	retval = ext2fs_get_array(fs->icache->cache_ਮਾਪ,
				  ਆਕਾਰ(ਬਣਤਰ ext2_inode_cache_ent),
				  &fs->icache->cache);
	ਜੇ (retval)
		ਜਾਓ errout;

	ਲਈ (i = 0; i < fs->icache->cache_ਮਾਪ; i++) {
		retval = ext2fs_get_mem(EXT2_INODE_ਮਾਪ(fs->super),
					&fs->icache->cache[i].inode);
		ਜੇ (retval)
			ਜਾਓ errout;
	}

	ext2fs_flush_icache(fs);
	ਵਾਪਸ 0;
errout:
	ext2fs_free_inode_cache(fs->icache);
	fs->icache = 0;
	ਵਾਪਸ retval;
}

errcode_t ext2fs_open_inode_ਜਾਂਚ(ext2_filsys fs, ਪੂਰਨ ਬਫਰ_ਖੰਡਾਂ,
				 ext2_inode_ਜਾਂਚ *ret_ਜਾਂਚ)
{
	ext2_inode_ਜਾਂਚ	ਜਾਂਚ;
	errcode_t	retval;
	errcode_t (*save_get_ਖੰਡਾਂ)(ext2_filsys f, ext2_ino_t ino, blk_t *ਖੰਡਾਂ);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ (ext2fs_has_feature_journal_dev(fs->super))
		ਵਾਪਸ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	ਜੇ (fs->blocksize < 1024)
		ਵਾਪਸ EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/*
	 * If fs->badblocks isn't set, then set it --- since the inode
	 * scanning functions require it.
	 */
	ਜੇ (fs->badblocks == 0) {
		/*
		 * Temporarily save fs->get_blocks and set it to zero,
		 * for compatibility with old e2fsck's.
		 */
		save_get_ਖੰਡਾਂ = fs->get_ਖੰਡਾਂ;
		fs->get_ਖੰਡਾਂ = 0;
		retval = ext2fs_read_bb_inode(fs, &fs->badblocks);
		ਜੇ (retval && fs->badblocks) {
			ext2fs_badblocks_list_free(fs->badblocks);
			fs->badblocks = 0;
		}
		fs->get_ਖੰਡਾਂ = save_get_ਖੰਡਾਂ;
	}

	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ ext2_struct_inode_ਜਾਂਚ), &ਜਾਂਚ);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	memset(ਜਾਂਚ, 0, ਆਕਾਰ(ਬਣਤਰ ext2_struct_inode_ਜਾਂਚ));

	ਜਾਂਚ->magic = EXT2_ET_MAGIC_INODE_ਜਾਂਚ;
	ਜਾਂਚ->fs = fs;
	ਜਾਂਚ->inode_ਮਾਪ = EXT2_INODE_ਮਾਪ(fs->super);
	ਜਾਂਚ->ਬਾਈਟਾਂ_ਖੱਬਾ = 0;
	ਜਾਂਚ->current_ਸਮੂਹ = 0;
	ਜਾਂਚ->groups_ਖੱਬਾ = fs->ਸਮੂਹ_desc_ਗਿਣਤੀ - 1;
	ਜਾਂਚ->inode_ਬਫਰ_ਖੰਡਾਂ = ਬਫਰ_ਖੰਡਾਂ ? ਬਫਰ_ਖੰਡਾਂ :
				    EXT2_INODE_ਜਾਂਚ_DEFAULT_ਬਫਰ_ਖੰਡਾਂ;
	ਜਾਂਚ->current_ਖੰਡ = ext2fs_inode_ਸਾਰਣੀ_loc(ਜਾਂਚ->fs,
						     ਜਾਂਚ->current_ਸਮੂਹ);
	ਜੇ (ਜਾਂਚ->current_ਖੰਡ &&
	    ((ਜਾਂਚ->current_ਖੰਡ < fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ) ||
	     (ਜਾਂਚ->current_ਖੰਡ + fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ - 1 >=
	      ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super)))) {
		ext2fs_free_mem(&ਜਾਂਚ);
		ਵਾਪਸ EXT2_ET_GDESC_BAD_INODE_ਸਾਰਣੀ;
	}

	ਜਾਂਚ->inodes_ਖੱਬਾ = EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(ਜਾਂਚ->fs->super);
	ਜਾਂਚ->ਖੰਡਾਂ_ਖੱਬਾ = ਜਾਂਚ->fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ;
	ਜੇ (ext2fs_has_ਸਮੂਹ_desc_csum(fs)) {
		__u32 ਅਣਵਰਤਿਆ = ext2fs_bg_itable_ਅਣਵਰਤਿਆ(fs, ਜਾਂਚ->current_ਸਮੂਹ);
		ਜੇ (ਜਾਂਚ->inodes_ਖੱਬਾ > ਅਣਵਰਤਿਆ)
			ਜਾਂਚ->inodes_ਖੱਬਾ -= ਅਣਵਰਤਿਆ;
		ਨਹੀਂਤਾਂ
			ਜਾਂਚ->inodes_ਖੱਬਾ = 0;
		ਜਾਂਚ->ਖੰਡਾਂ_ਖੱਬਾ =
			(ਜਾਂਚ->inodes_ਖੱਬਾ +
			 (fs->blocksize / ਜਾਂਚ->inode_ਮਾਪ - 1)) *
			ਜਾਂਚ->inode_ਮਾਪ / fs->blocksize;
	}
	retval = io_channel_alloc_buf(fs->io, ਜਾਂਚ->inode_ਬਫਰ_ਖੰਡਾਂ,
				      &ਜਾਂਚ->inode_ਬਫਰ);
	ਜਾਂਚ->ਪੂਰਾ_ਸਮੂਹ = 0;
	ਜਾਂਚ->ਪੂਰਾ_ਸਮੂਹ_ਅੰਕੜੇ = 0;
	ਜਾਂਚ->bad_ਖੰਡ_ptr = 0;
	ਜੇ (retval) {
		ext2fs_free_mem(&ਜਾਂਚ);
		ਵਾਪਸ retval;
	}
	retval = ext2fs_get_mem(ਜਾਂਚ->inode_ਮਾਪ + ਜਾਂਚ->inode_ਬਫਰ_ਖੰਡਾਂ,
				&ਜਾਂਚ->ਆਰਜ਼ੀ_ਬਫਰ);
	ਜੇ (retval) {
		ext2fs_free_mem(&ਜਾਂਚ->inode_ਬਫਰ);
		ext2fs_free_mem(&ਜਾਂਚ);
		ਵਾਪਸ retval;
	}
	memset(ਜਾਂਚ_ਖੰਡ_ਸਥਿਤੀ(ਜਾਂਚ), 0, ਜਾਂਚ->inode_ਬਫਰ_ਖੰਡਾਂ);
	ਜੇ (ਜਾਂਚ->fs->badblocks && ਜਾਂਚ->fs->badblocks->ਅੰਕ)
		ਜਾਂਚ->ਜਾਂਚ_flags |= EXT2_SF_CHK_BADBLOCKS;
	ਜੇ (ext2fs_has_ਸਮੂਹ_desc_csum(fs))
		ਜਾਂਚ->ਜਾਂਚ_flags |= EXT2_SF_DO_LAZY;
	*ret_ਜਾਂਚ = ਜਾਂਚ;
	ਵਾਪਸ 0;
}

ਖਾਲੀ ext2fs_close_inode_ਜਾਂਚ(ext2_inode_ਜਾਂਚ ਜਾਂਚ)
{
	ਜੇ (!ਜਾਂਚ || (ਜਾਂਚ->magic != EXT2_ET_MAGIC_INODE_ਜਾਂਚ))
		ਵਾਪਸ;

	ext2fs_free_mem(&ਜਾਂਚ->inode_ਬਫਰ);
	ਜਾਂਚ->inode_ਬਫਰ = ਨਿਰਾ;
	ext2fs_free_mem(&ਜਾਂਚ->ਆਰਜ਼ੀ_ਬਫਰ);
	ਜਾਂਚ->ਆਰਜ਼ੀ_ਬਫਰ = ਨਿਰਾ;
	ext2fs_free_mem(&ਜਾਂਚ);
	ਵਾਪਸ;
}

ਖਾਲੀ ext2fs_ਸੈੱਟ_inode_callback(ext2_inode_ਜਾਂਚ ਜਾਂਚ,
			       errcode_t (*ਪੂਰਾ_ਸਮੂਹ)(ext2_filsys fs,
						       ext2_inode_ਜਾਂਚ ਜਾਂਚ,
						       dgrp_t ਸਮੂਹ,
						       ਖਾਲੀ * priv_ਅੰਕੜੇ),
			       ਖਾਲੀ *ਪੂਰਾ_ਸਮੂਹ_ਅੰਕੜੇ)
{
	ਜੇ (!ਜਾਂਚ || (ਜਾਂਚ->magic != EXT2_ET_MAGIC_INODE_ਜਾਂਚ))
		ਵਾਪਸ;

	ਜਾਂਚ->ਪੂਰਾ_ਸਮੂਹ = ਪੂਰਾ_ਸਮੂਹ;
	ਜਾਂਚ->ਪੂਰਾ_ਸਮੂਹ_ਅੰਕੜੇ = ਪੂਰਾ_ਸਮੂਹ_ਅੰਕੜੇ;
}

ਪੂਰਨ ext2fs_inode_ਜਾਂਚ_flags(ext2_inode_ਜਾਂਚ ਜਾਂਚ, ਪੂਰਨ ਸੈੱਟ_flags,
			    ਪੂਰਨ clear_flags)
{
	ਪੂਰਨ	old_flags;

	ਜੇ (!ਜਾਂਚ || (ਜਾਂਚ->magic != EXT2_ET_MAGIC_INODE_ਜਾਂਚ))
		ਵਾਪਸ 0;

	old_flags = ਜਾਂਚ->ਜਾਂਚ_flags;
	ਜਾਂਚ->ਜਾਂਚ_flags &= ~clear_flags;
	ਜਾਂਚ->ਜਾਂਚ_flags |= ਸੈੱਟ_flags;
	ਵਾਪਸ old_flags;
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * get ready to read in a new blockgroup.
 */
ਸਥਿਰ errcode_t get_ਅਗਲਾ_blockgroup(ext2_inode_ਜਾਂਚ ਜਾਂਚ)
{
	ext2_filsys fs = ਜਾਂਚ->fs;

	ਜਾਂਚ->current_ਸਮੂਹ++;
	ਜਾਂਚ->groups_ਖੱਬਾ--;

	ਜਾਂਚ->current_ਖੰਡ = ext2fs_inode_ਸਾਰਣੀ_loc(ਜਾਂਚ->fs,
						     ਜਾਂਚ->current_ਸਮੂਹ);
	ਜਾਂਚ->current_inode = ਜਾਂਚ->current_ਸਮੂਹ *
		EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super);

	ਜਾਂਚ->ਬਾਈਟਾਂ_ਖੱਬਾ = 0;
	ਜਾਂਚ->inodes_ਖੱਬਾ = EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super);
	ਜਾਂਚ->ਖੰਡਾਂ_ਖੱਬਾ = fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ;
	ਜੇ (ext2fs_has_ਸਮੂਹ_desc_csum(fs)) {
		__u32 ਅਣਵਰਤਿਆ = ext2fs_bg_itable_ਅਣਵਰਤਿਆ(fs, ਜਾਂਚ->current_ਸਮੂਹ);
		ਜੇ (ਜਾਂਚ->inodes_ਖੱਬਾ > ਅਣਵਰਤਿਆ)
			ਜਾਂਚ->inodes_ਖੱਬਾ -= ਅਣਵਰਤਿਆ;
		ਨਹੀਂਤਾਂ
			ਜਾਂਚ->inodes_ਖੱਬਾ = 0;
		ਜਾਂਚ->ਖੰਡਾਂ_ਖੱਬਾ =
			(ਜਾਂਚ->inodes_ਖੱਬਾ +
			 (fs->blocksize / ਜਾਂਚ->inode_ਮਾਪ - 1)) *
			ਜਾਂਚ->inode_ਮਾਪ / fs->blocksize;
	}
	ਜੇ (ਜਾਂਚ->current_ਖੰਡ &&
	    ((ਜਾਂਚ->current_ਖੰਡ < fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ) ||
	     (ਜਾਂਚ->current_ਖੰਡ + fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ - 1 >=
	      ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super))))
		ਵਾਪਸ EXT2_ET_GDESC_BAD_INODE_ਸਾਰਣੀ;
	ਵਾਪਸ 0;
}

errcode_t ext2fs_inode_ਜਾਂਚ_goto_blockgroup(ext2_inode_ਜਾਂਚ ਜਾਂਚ,
					    ਪੂਰਨ	ਸਮੂਹ)
{
	ਜਾਂਚ->current_ਸਮੂਹ = ਸਮੂਹ - 1;
	ਜਾਂਚ->groups_ਖੱਬਾ = ਜਾਂਚ->fs->ਸਮੂਹ_desc_ਗਿਣਤੀ - ਸਮੂਹ;
	ਜਾਂਚ->bad_ਖੰਡ_ptr = 0;
	ਵਾਪਸ get_ਅਗਲਾ_blockgroup(ਜਾਂਚ);
}

/*
 * This function is called by get_next_blocks() to check for bad
 * blocks in the inode table.
 *
 * This function assumes that badblocks_list->list is sorted in
 * increasing order.
 */
ਸਥਿਰ errcode_t check_for_inode_bad_ਖੰਡਾਂ(ext2_inode_ਜਾਂਚ ਜਾਂਚ,
					    blk64_t *ਅੰਕ_ਖੰਡਾਂ)
{
	blk64_t	blk = ਜਾਂਚ->current_ਖੰਡ;
	badblocks_list	bb = ਜਾਂਚ->fs->badblocks;

	/*
	 * If the inode table is missing, then obviously there are no
	 * bad blocks.  :-)
	 */
	ਜੇ (blk == 0)
		ਵਾਪਸ 0;

	/* Make sure bad_block_ptr is still valid */
	ਜੇ (ਜਾਂਚ->bad_ਖੰਡ_ptr >= bb->ਅੰਕ) {
		ਜਾਂਚ->ਜਾਂਚ_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		ਵਾਪਸ 0;
	}

	/*
	 * If the current block is greater than the bad block listed
	 * in the bad block list, then advance the pointer until this
	 * is no longer the case.  If we run out of bad blocks, then
	 * we don't need to do any more checking!
	 */
	ਜਦੋਂ (blk > bb->list[ਜਾਂਚ->bad_ਖੰਡ_ptr]) {
		ਜੇ (++ਜਾਂਚ->bad_ਖੰਡ_ptr >= bb->ਅੰਕ) {
			ਜਾਂਚ->ਜਾਂਚ_flags &= ~EXT2_SF_CHK_BADBLOCKS;
			ਵਾਪਸ 0;
		}
	}

	/*
	 * If the current block is equal to the bad block listed in
	 * the bad block list, then handle that one block specially.
	 * (We could try to handle runs of bad blocks, but that
	 * only increases CPU efficiency by a small amount, at the
	 * expense of a huge expense of code complexity, and for an
	 * uncommon case at that.)
	 */
	ਜੇ (blk == bb->list[ਜਾਂਚ->bad_ਖੰਡ_ptr]) {
		ਜਾਂਚ->ਜਾਂਚ_flags |= EXT2_SF_BAD_INODE_BLK;
		*ਅੰਕ_ਖੰਡਾਂ = 1;
		ਜੇ (++ਜਾਂਚ->bad_ਖੰਡ_ptr >= bb->ਅੰਕ)
			ਜਾਂਚ->ਜਾਂਚ_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		ਵਾਪਸ 0;
	}

	/*
	 * If there is a bad block in the range that we're about to
	 * read in, adjust the number of blocks to read so that we we
	 * don't read in the bad block.  (Then the next block to read
	 * will be the bad block, which is handled in the above case.)
	 */
	ਜੇ ((blk + *ਅੰਕ_ਖੰਡਾਂ) > bb->list[ਜਾਂਚ->bad_ਖੰਡ_ptr])
		*ਅੰਕ_ਖੰਡਾਂ = (ਪੂਰਨ) (bb->list[ਜਾਂਚ->bad_ਖੰਡ_ptr] - blk);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਖੰਡ_map_looks_insane(ext2_filsys fs,
				  ਬਣਤਰ ext2_inode_large *inode)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i, bad;

	/* We're only interested in block mapped files, dirs, and symlinks */
	ਜੇ ((inode->i_flags & EXT4_INLINE_ਅੰਕੜੇ_FL) ||
	    (inode->i_flags & EXT4_EXTENTS_FL))
		ਵਾਪਸ 0;
	ਜੇ (!LINUX_S_ISREG(inode->i_ਬਹੁਲਕ) &&
	    !LINUX_S_ISLNK(inode->i_ਬਹੁਲਕ) &&
	    !LINUX_S_ISDIR(inode->i_ਬਹੁਲਕ))
		ਵਾਪਸ 0;
	ਜੇ (LINUX_S_ISLNK(inode->i_ਬਹੁਲਕ) &&
	    EXT2_I_ਮਾਪ(inode) <= ਆਕਾਰ(inode->i_ਖੰਡ))
		ਵਾਪਸ 0;

	/* Unused inodes probably aren't insane */
	ਜੇ (inode->i_links_ਗਿਣਤੀ == 0)
		ਵਾਪਸ 0;

	/* See if more than half the block maps are insane */
	ਲਈ (i = 0, bad = 0; i < EXT2_N_ਖੰਡਾਂ; i++)
		ਜੇ (inode->i_ਖੰਡ[i] != 0 &&
		    (inode->i_ਖੰਡ[i] < fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ ||
		     inode->i_ਖੰਡ[i] >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super)))
			bad++;
	ਵਾਪਸ bad > EXT2_N_ਖੰਡਾਂ / 2;
}

ਸਥਿਰ ਪੂਰਨ extent_ਸਿਰ_looks_insane(ਬਣਤਰ ext2_inode_large *inode)
{
	ਜੇ (!(inode->i_flags & EXT4_EXTENTS_FL) ||
	    ext2fs_extent_ਸਿਰਲੇਖ_verify(inode->i_ਖੰਡ,
					ਆਕਾਰ(inode->i_ਖੰਡ)) == 0)
		ਵਾਪਸ 0;
	ਵਾਪਸ 1;
}

/*
 * Check all the inodes that we just read into the buffer.  Record what we
 * find here -- currently, we can observe that all checksums are ok; more
 * than half the inodes are insane; or no conclusions at all.
 */
ਸਥਿਰ ਖਾਲੀ check_inode_ਖੰਡ_sanity(ext2_inode_ਜਾਂਚ ਜਾਂਚ, blk64_t ਅੰਕ_ਖੰਡਾਂ)
{
	ext2_ino_t	ino, inodes_ਮੰਜ਼ਿਲ_ਜਾਂਚ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	badness, checksum_failures;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	inodes_ਵਿੱਚ_buf, inodes_ਪ੍ਰਤੀ_ਖੰਡ;
	ਅੱਖਰ		*p;
	ਬਣਤਰ ext2_inode_large *inode;
	ਅੱਖਰ		*ਖੰਡ_ਸਥਿਤੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	blk, bad_csum;

	ਜੇ (!(ਜਾਂਚ->ਜਾਂਚ_flags & EXT2_SF_WARN_GARBAGE_INODES))
		ਵਾਪਸ;

	inodes_ਮੰਜ਼ਿਲ_ਜਾਂਚ = ਜਾਂਚ->inodes_ਖੱਬਾ;
	inodes_ਵਿੱਚ_buf = ਅੰਕ_ਖੰਡਾਂ * ਜਾਂਚ->fs->blocksize / ਜਾਂਚ->inode_ਮਾਪ;
	ਜੇ (inodes_ਮੰਜ਼ਿਲ_ਜਾਂਚ > inodes_ਵਿੱਚ_buf)
		inodes_ਮੰਜ਼ਿਲ_ਜਾਂਚ = inodes_ਵਿੱਚ_buf;

	p = (ਅੱਖਰ *) ਜਾਂਚ->inode_ਬਫਰ;
	ino = ਜਾਂਚ->current_inode + 1;
	checksum_failures = badness = 0;
	ਖੰਡ_ਸਥਿਤੀ = ਜਾਂਚ_ਖੰਡ_ਸਥਿਤੀ(ਜਾਂਚ);
	memset(ਖੰਡ_ਸਥਿਤੀ, 0, ਜਾਂਚ->inode_ਬਫਰ_ਖੰਡਾਂ);
	inodes_ਪ੍ਰਤੀ_ਖੰਡ = EXT2_INODES_ਪ੍ਰਤੀ_ਖੰਡ(ਜਾਂਚ->fs->super);

	ਜੇ (inodes_ਪ੍ਰਤੀ_ਖੰਡ < 2)
		ਵਾਪਸ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	ਜੇ (ext2fs_get_mem(EXT2_INODE_ਮਾਪ(ਜਾਂਚ->fs->super), &inode))
		ਵਾਪਸ;
#ਜੇ_ਅੰਤ

	ਜਦੋਂ (inodes_ਮੰਜ਼ਿਲ_ਜਾਂਚ > 0) {
		blk = (p - (ਅੱਖਰ *)ਜਾਂਚ->inode_ਬਫਰ) / ਜਾਂਚ->fs->blocksize;
		bad_csum = ext2fs_inode_csum_verify(ਜਾਂਚ->fs, ino,
				(ਬਣਤਰ ext2_inode_large *) p) == 0;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
		ext2fs_ਅਦਲਾ_ਬਦਲੀ_inode_ਭਰਿਆ(ਜਾਂਚ->fs,
			       (ਬਣਤਰ ext2_inode_large *) inode,
			       (ਬਣਤਰ ext2_inode_large *) p,
			       0, EXT2_INODE_ਮਾਪ(ਜਾਂਚ->fs->super));
#ਨਹੀਂਤਾਂ
		inode = (ਬਣਤਰ ext2_inode_large *) p;
#ਜੇ_ਅੰਤ

		/* Is this inode insane? */
		ਜੇ (bad_csum) {
			checksum_failures++;
			badness++;
		} ਨਹੀਂਤਾਂ ਜੇ (extent_ਸਿਰ_looks_insane(inode) ||
			   ਖੰਡ_map_looks_insane(ਜਾਂਚ->fs, inode))
			badness++;

		/* If more than half are insane, declare the whole block bad */
		ਜੇ (badness > inodes_ਪ੍ਰਤੀ_ਖੰਡ / 2) {
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ino_adj;

			ਖੰਡ_ਸਥਿਤੀ[blk] |= IBLOCK_ਸਥਿਤੀ_INSANE;
			ino_adj = inodes_ਪ੍ਰਤੀ_ਖੰਡ -
						((ino - 1) % inodes_ਪ੍ਰਤੀ_ਖੰਡ);
			ਜੇ (ino_adj > inodes_ਮੰਜ਼ਿਲ_ਜਾਂਚ)
				ino_adj = inodes_ਮੰਜ਼ਿਲ_ਜਾਂਚ;
			inodes_ਮੰਜ਼ਿਲ_ਜਾਂਚ -= ino_adj;
			p += ਜਾਂਚ->inode_ਮਾਪ * ino_adj;
			ino += ino_adj;
			checksum_failures = badness = 0;
			ਜਾਰੀ;
		}

		ਜੇ ((ino % inodes_ਪ੍ਰਤੀ_ਖੰਡ) == 0) {
			ਜੇ (checksum_failures == 0)
				ਖੰਡ_ਸਥਿਤੀ[blk] |= IBLOCK_ਸਥਿਤੀ_CSUMS_OK;
			checksum_failures = badness = 0;
		}
		inodes_ਮੰਜ਼ਿਲ_ਜਾਂਚ--;
		p += ਜਾਂਚ->inode_ਮਾਪ;
		ino++;
	};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	ext2fs_free_mem(&inode);
#ਜੇ_ਅੰਤ
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * read in more blocks from the current blockgroup's inode table.
 */
ਸਥਿਰ errcode_t get_ਅਗਲਾ_ਖੰਡਾਂ(ext2_inode_ਜਾਂਚ ਜਾਂਚ)
{
	blk64_t		ਅੰਕ_ਖੰਡਾਂ;
	errcode_t	retval;

	/*
	 * Figure out how many blocks to read; we read at most
	 * inode_buffer_blocks, and perhaps less if there aren't that
	 * many blocks left to read.
	 */
	ਅੰਕ_ਖੰਡਾਂ = ਜਾਂਚ->inode_ਬਫਰ_ਖੰਡਾਂ;
	ਜੇ (ਅੰਕ_ਖੰਡਾਂ > ਜਾਂਚ->ਖੰਡਾਂ_ਖੱਬਾ)
		ਅੰਕ_ਖੰਡਾਂ = ਜਾਂਚ->ਖੰਡਾਂ_ਖੱਬਾ;

	/*
	 * If the past block "read" was a bad block, then mark the
	 * left-over extra bytes as also being bad.
	 */
	ਜੇ (ਜਾਂਚ->ਜਾਂਚ_flags & EXT2_SF_BAD_INODE_BLK) {
		ਜੇ (ਜਾਂਚ->ਬਾਈਟਾਂ_ਖੱਬਾ)
			ਜਾਂਚ->ਜਾਂਚ_flags |= EXT2_SF_BAD_EXTRA_ਬਾਈਟਾਂ;
		ਜਾਂਚ->ਜਾਂਚ_flags &= ~EXT2_SF_BAD_INODE_BLK;
	}

	/*
	 * Do inode bad block processing, if necessary.
	 */
	ਜੇ (ਜਾਂਚ->ਜਾਂਚ_flags & EXT2_SF_CHK_BADBLOCKS) {
		retval = check_for_inode_bad_ਖੰਡਾਂ(ਜਾਂਚ, &ਅੰਕ_ਖੰਡਾਂ);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}

	ਜੇ ((ਜਾਂਚ->ਜਾਂਚ_flags & EXT2_SF_BAD_INODE_BLK) ||
	    (ਜਾਂਚ->current_ਖੰਡ == 0)) {
		memset(ਜਾਂਚ->inode_ਬਫਰ, 0,
		       (size_t) ਅੰਕ_ਖੰਡਾਂ * ਜਾਂਚ->fs->blocksize);
	} ਨਹੀਂਤਾਂ {
		retval = io_channel_read_blk64(ਜਾਂਚ->fs->io,
					     ਜਾਂਚ->current_ਖੰਡ,
					     (ਪੂਰਨ) ਅੰਕ_ਖੰਡਾਂ,
					     ਜਾਂਚ->inode_ਬਫਰ);
		ਜੇ (retval)
			ਵਾਪਸ EXT2_ET_ਅਗਲਾ_INODE_READ;
	}
	check_inode_ਖੰਡ_sanity(ਜਾਂਚ, ਅੰਕ_ਖੰਡਾਂ);

	ਜਾਂਚ->ptr = ਜਾਂਚ->inode_ਬਫਰ;
	ਜਾਂਚ->ਬਾਈਟਾਂ_ਖੱਬਾ = ਅੰਕ_ਖੰਡਾਂ * ਜਾਂਚ->fs->blocksize;

	ਜਾਂਚ->ਖੰਡਾਂ_ਖੱਬਾ -= ਅੰਕ_ਖੰਡਾਂ;
	ਜੇ (ਜਾਂਚ->current_ਖੰਡ)
		ਜਾਂਚ->current_ਖੰਡ += ਅੰਕ_ਖੰਡਾਂ;

	ਵਾਪਸ 0;
}

#ਜੇ 0
/*
 * Returns 1 if the entire inode_buffer has a non-zero size and
 * contains all zeros.  (Not just deleted inodes, since that means
 * that part of the inode table was used at one point; we want all
 * zeros, which means that the inode table is pristine.)
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ is_ਖਾਲੀ_ਥਾਵਾਂ_ਜਾਂਚ(ext2_inode_ਜਾਂਚ ਜਾਂਚ)
{
	ਪੂਰਨ	i;

	ਜੇ (ਜਾਂਚ->ਬਾਈਟਾਂ_ਖੱਬਾ == 0)
		ਵਾਪਸ 0;

	ਲਈ (i=0; i < ਜਾਂਚ->ਬਾਈਟਾਂ_ਖੱਬਾ; i++)
		ਜੇ (ਜਾਂਚ->ptr[i])
			ਵਾਪਸ 0;
	ਵਾਪਸ 1;
}
#ਜੇ_ਅੰਤ

errcode_t ext2fs_get_ਅਗਲਾ_inode_ਭਰਿਆ(ext2_inode_ਜਾਂਚ ਜਾਂਚ, ext2_ino_t *ino,
				     ਬਣਤਰ ext2_inode *inode, ਪੂਰਨ bufsize)
{
	errcode_t	retval;
	ਪੂਰਨ		extra_ਬਾਈਟਾਂ = 0;
	ਪੂਰਨ		ਲੰਮਾਈ;
	ਬਣਤਰ ext2_inode_large	*iptr = (ਬਣਤਰ ext2_inode_large *)inode;
	ਅੱਖਰ		*iblock_ਸਥਿਤੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	iblk;

	EXT2_CHECK_MAGIC(ਜਾਂਚ, EXT2_ET_MAGIC_INODE_ਜਾਂਚ);
	ਲੰਮਾਈ = EXT2_INODE_ਮਾਪ(ਜਾਂਚ->fs->super);
	iblock_ਸਥਿਤੀ = ਜਾਂਚ_ਖੰਡ_ਸਥਿਤੀ(ਜਾਂਚ);

	/*
	 * Do we need to start reading a new block group?
	 */
	ਜੇ (ਜਾਂਚ->inodes_ਖੱਬਾ <= 0) {
	force_new_ਸਮੂਹ:
		ਜੇ (ਜਾਂਚ->ਪੂਰਾ_ਸਮੂਹ) {
			retval = (ਜਾਂਚ->ਪੂਰਾ_ਸਮੂਹ)
				(ਜਾਂਚ->fs, ਜਾਂਚ, ਜਾਂਚ->current_ਸਮੂਹ,
				 ਜਾਂਚ->ਪੂਰਾ_ਸਮੂਹ_ਅੰਕੜੇ);
			ਜੇ (retval)
				ਵਾਪਸ retval;
		}
		ਜੇ (ਜਾਂਚ->groups_ਖੱਬਾ <= 0) {
			*ino = 0;
			ਵਾਪਸ 0;
		}
		retval = get_ਅਗਲਾ_blockgroup(ਜਾਂਚ);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}
	/*
	 * These checks are done outside the above if statement so
	 * they can be done for block group #0.
	 */
	ਜੇ ((ਜਾਂਚ->ਜਾਂਚ_flags & EXT2_SF_DO_LAZY) &&
	    (ext2fs_bg_flags_test(ਜਾਂਚ->fs, ਜਾਂਚ->current_ਸਮੂਹ, EXT2_BG_INODE_UNINIT)
	     ))
		ਜਾਓ force_new_ਸਮੂਹ;
	ਜੇ (ਜਾਂਚ->inodes_ਖੱਬਾ == 0)
		ਜਾਓ force_new_ਸਮੂਹ;
	ਜੇ (ਜਾਂਚ->current_ਖੰਡ == 0) {
		ਜੇ (ਜਾਂਚ->ਜਾਂਚ_flags & EXT2_SF_SKIP_MISSING_ITABLE) {
			ਜਾਓ force_new_ਸਮੂਹ;
		} ਨਹੀਂਤਾਂ
			ਵਾਪਸ EXT2_ET_MISSING_INODE_ਸਾਰਣੀ;
	}


	/*
	 * Have we run out of space in the inode buffer?  If so, we
	 * need to read in more blocks.
	 */
	ਜੇ (ਜਾਂਚ->ਬਾਈਟਾਂ_ਖੱਬਾ < ਜਾਂਚ->inode_ਮਾਪ) {
		ਜੇ (ਜਾਂਚ->ਬਾਈਟਾਂ_ਖੱਬਾ)
			ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਜਾਂਚ->ਆਰਜ਼ੀ_ਬਫਰ, ਜਾਂਚ->ptr, ਜਾਂਚ->ਬਾਈਟਾਂ_ਖੱਬਾ);
		extra_ਬਾਈਟਾਂ = ਜਾਂਚ->ਬਾਈਟਾਂ_ਖੱਬਾ;

		retval = get_ਅਗਲਾ_ਖੰਡਾਂ(ਜਾਂਚ);
		ਜੇ (retval)
			ਵਾਪਸ retval;
#ਜੇ 0
		/*
		 * XXX test  Need check for used inode somehow.
		 * (Note: this is hard.)
		 */
		ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_ਜਾਂਚ(ਜਾਂਚ))
			ਜਾਓ force_new_ਸਮੂਹ;
#ਜੇ_ਅੰਤ
	}

	ਜੇ (bufsize < ਲੰਮਾਈ) {
		retval = ext2fs_get_mem(ਲੰਮਾਈ, &iptr);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}

	retval = 0;
	iblk = ਜਾਂਚ->current_inode % EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(ਜਾਂਚ->fs->super) /
				EXT2_INODES_ਪ੍ਰਤੀ_ਖੰਡ(ਜਾਂਚ->fs->super) %
				ਜਾਂਚ->inode_ਬਫਰ_ਖੰਡਾਂ;
	ਜੇ (extra_ਬਾਈਟਾਂ) {
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਜਾਂਚ->ਆਰਜ਼ੀ_ਬਫਰ+extra_ਬਾਈਟਾਂ, ਜਾਂਚ->ptr,
		       ਜਾਂਚ->inode_ਮਾਪ - extra_ਬਾਈਟਾਂ);
		ਜਾਂਚ->ptr += ਜਾਂਚ->inode_ਮਾਪ - extra_ਬਾਈਟਾਂ;
		ਜਾਂਚ->ਬਾਈਟਾਂ_ਖੱਬਾ -= ਜਾਂਚ->inode_ਮਾਪ - extra_ਬਾਈਟਾਂ;

		/* Verify the inode checksum. */
		ਜੇ (!(iblock_ਸਥਿਤੀ[iblk] & IBLOCK_ਸਥਿਤੀ_CSUMS_OK) &&
		    !(ਜਾਂਚ->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(ਜਾਂਚ->fs, ਜਾਂਚ->current_inode + 1,
				(ਬਣਤਰ ext2_inode_large *)ਜਾਂਚ->ਆਰਜ਼ੀ_ਬਫਰ))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
		memset(iptr, 0, ਲੰਮਾਈ);
		ext2fs_ਅਦਲਾ_ਬਦਲੀ_inode_ਭਰਿਆ(ਜਾਂਚ->fs,
			       (ਬਣਤਰ ext2_inode_large *) iptr,
			       (ਬਣਤਰ ext2_inode_large *) ਜਾਂਚ->ਆਰਜ਼ੀ_ਬਫਰ,
			       0, ਲੰਮਾਈ);
#ਨਹੀਂਤਾਂ
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(iptr, ਜਾਂਚ->ਆਰਜ਼ੀ_ਬਫਰ, ਲੰਮਾਈ);
#ਜੇ_ਅੰਤ
		ਜੇ (ਜਾਂਚ->ਜਾਂਚ_flags & EXT2_SF_BAD_EXTRA_ਬਾਈਟਾਂ)
			retval = EXT2_ET_BAD_ਖੰਡ_ਵਿੱਚ_INODE_ਸਾਰਣੀ;
		ਜਾਂਚ->ਜਾਂਚ_flags &= ~EXT2_SF_BAD_EXTRA_ਬਾਈਟਾਂ;
	} ਨਹੀਂਤਾਂ {
		/* Verify the inode checksum. */
		ਜੇ (!(iblock_ਸਥਿਤੀ[iblk] & IBLOCK_ਸਥਿਤੀ_CSUMS_OK) &&
		    !(ਜਾਂਚ->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(ਜਾਂਚ->fs, ਜਾਂਚ->current_inode + 1,
				(ਬਣਤਰ ext2_inode_large *)ਜਾਂਚ->ptr))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
		memset(iptr, 0, ਲੰਮਾਈ);
		ext2fs_ਅਦਲਾ_ਬਦਲੀ_inode_ਭਰਿਆ(ਜਾਂਚ->fs,
				(ਬਣਤਰ ext2_inode_large *) iptr,
				(ਬਣਤਰ ext2_inode_large *) ਜਾਂਚ->ptr,
				0, ਲੰਮਾਈ);
#ਨਹੀਂਤਾਂ
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(iptr, ਜਾਂਚ->ptr, ਲੰਮਾਈ);
#ਜੇ_ਅੰਤ
		ਜਾਂਚ->ptr += ਜਾਂਚ->inode_ਮਾਪ;
		ਜਾਂਚ->ਬਾਈਟਾਂ_ਖੱਬਾ -= ਜਾਂਚ->inode_ਮਾਪ;
		ਜੇ (ਜਾਂਚ->ਜਾਂਚ_flags & EXT2_SF_BAD_INODE_BLK)
			retval = EXT2_ET_BAD_ਖੰਡ_ਵਿੱਚ_INODE_ਸਾਰਣੀ;
	}
	ਜੇ ((iblock_ਸਥਿਤੀ[iblk] & IBLOCK_ਸਥਿਤੀ_INSANE) &&
	    (retval == 0 || retval == EXT2_ET_INODE_CSUM_INVALID))
		retval = EXT2_ET_INODE_IS_GARBAGE;

	ਜਾਂਚ->inodes_ਖੱਬਾ--;
	ਜਾਂਚ->current_inode++;
	*ino = ਜਾਂਚ->current_inode;
	ਜੇ (iptr != (ਬਣਤਰ ext2_inode_large *)inode) {
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(inode, iptr, bufsize);
		ext2fs_free_mem(&iptr);
	}
	ਵਾਪਸ retval;
}

errcode_t ext2fs_get_ਅਗਲਾ_inode(ext2_inode_ਜਾਂਚ ਜਾਂਚ, ext2_ino_t *ino,
				ਬਣਤਰ ext2_inode *inode)
{
	ਵਾਪਸ ext2fs_get_ਅਗਲਾ_inode_ਭਰਿਆ(ਜਾਂਚ, ino, inode,
						ਆਕਾਰ(ਬਣਤਰ ext2_inode));
}

/*
 * Functions to read and write a single inode.
 */
errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
			     ਬਣਤਰ ext2_inode * inode, ਪੂਰਨ bufsize,
			     ਪੂਰਨ flags)
{
	blk64_t		ਖੰਡ_nr;
	dgrp_t		ਸਮੂਹ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ 	ਖੰਡ, ਵਿੱਥ;
	ਅੱਖਰ 		*ptr;
	errcode_t	retval;
	ਬਿਨਾਨਿਸ਼ਾਨੀ	i;
	ਪੂਰਨ		clen, inodes_ਪ੍ਰਤੀ_ਖੰਡ;
	io_channel	io;
	ਪੂਰਨ		ਲੰਮਾਈ = EXT2_INODE_ਮਾਪ(fs->super);
	ਬਣਤਰ ext2_inode_large	*iptr;
	ਪੂਰਨ		cache_ਥਾਂ, fail_csum;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ (ext2fs_has_feature_journal_dev(fs->super))
		ਵਾਪਸ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	ਜੇ (fs->blocksize < 1024)
		ਵਾਪਸ EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/* Check to see if user has an override function */
	ਜੇ (fs->read_inode &&
	    ((bufsize == ਆਕਾਰ(ਬਣਤਰ ext2_inode)) ||
	     (EXT2_INODE_ਮਾਪ(fs->super) == ਆਕਾਰ(ਬਣਤਰ ext2_inode)))) {
		retval = (fs->read_inode)(fs, ino, inode);
		ਜੇ (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			ਵਾਪਸ retval;
	}
	ਜੇ ((ino == 0) || (ino > fs->super->s_inodes_ਗਿਣਤੀ))
		ਵਾਪਸ EXT2_ET_BAD_INODE_ਅੰਕ;
	/* Create inode cache if not present */
	ਜੇ (!fs->icache) {
		retval = ext2fs_create_inode_cache(fs, 4);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}
	/* Check to see if it's in the inode cache */
	ਲਈ (i = 0; i < fs->icache->cache_ਮਾਪ; i++) {
		ਜੇ (fs->icache->cache[i].ino == ino) {
			ਸਮ੍ਰਿਤੀ_ਨਕਲ(inode, fs->icache->cache[i].inode,
			       (bufsize > ਲੰਮਾਈ) ? ਲੰਮਾਈ : bufsize);
			ਵਾਪਸ 0;
		}
	}
	ਜੇ (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		inodes_ਪ੍ਰਤੀ_ਖੰਡ = fs->blocksize / EXT2_INODE_ਮਾਪ(fs->super);
		ਖੰਡ_nr = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(fs->image_ਸਿਰਲੇਖ->ਵਿੱਥ_inode) / fs->blocksize;
		ਖੰਡ_nr += (ino - 1) / inodes_ਪ੍ਰਤੀ_ਖੰਡ;
		ਵਿੱਥ = ((ino - 1) % inodes_ਪ੍ਰਤੀ_ਖੰਡ) *
			EXT2_INODE_ਮਾਪ(fs->super);
		io = fs->image_io;
	} ਨਹੀਂਤਾਂ {
		ਸਮੂਹ = (ino - 1) / EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super);
		ਜੇ (ਸਮੂਹ > fs->ਸਮੂਹ_desc_ਗਿਣਤੀ)
			ਵਾਪਸ EXT2_ET_BAD_INODE_ਅੰਕ;
		ਵਿੱਥ = ((ino - 1) % EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super)) *
			EXT2_INODE_ਮਾਪ(fs->super);
		ਖੰਡ = ਵਿੱਥ >> EXT2_ਖੰਡ_ਮਾਪ_ਬਿੱਟ(fs->super);
		ਖੰਡ_nr = ext2fs_inode_ਸਾਰਣੀ_loc(fs, ਸਮੂਹ);
		ਜੇ (!ਖੰਡ_nr)
			ਵਾਪਸ EXT2_ET_MISSING_INODE_ਸਾਰਣੀ;
		ਜੇ ((ਖੰਡ_nr < fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ) ||
		    (ਖੰਡ_nr + fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ - 1 >=
		     ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super)))
			ਵਾਪਸ EXT2_ET_GDESC_BAD_INODE_ਸਾਰਣੀ;
		ਖੰਡ_nr += ਖੰਡ;
		io = fs->io;
	}
	ਵਿੱਥ &= (EXT2_ਖੰਡ_ਮਾਪ(fs->super) - 1);

	cache_ਥਾਂ = (fs->icache->cache_ਆਖਰੀ + 1) % fs->icache->cache_ਮਾਪ;
	iptr = (ਬਣਤਰ ext2_inode_large *)fs->icache->cache[cache_ਥਾਂ].inode;

	ptr = (ਅੱਖਰ *) iptr;
	ਜਦੋਂ (ਲੰਮਾਈ) {
		clen = ਲੰਮਾਈ;
		ਜੇ ((ਵਿੱਥ + ਲੰਮਾਈ) > fs->blocksize)
			clen = fs->blocksize - ਵਿੱਥ;

		ਜੇ (ਖੰਡ_nr != fs->icache->ਬਫਰ_blk) {
			retval = io_channel_read_blk64(io, ਖੰਡ_nr, 1,
						     fs->icache->ਬਫਰ);
			ਜੇ (retval)
				ਵਾਪਸ retval;
			fs->icache->ਬਫਰ_blk = ਖੰਡ_nr;
		}

		ਸਮ੍ਰਿਤੀ_ਨਕਲ(ptr, ((ਅੱਖਰ *) fs->icache->ਬਫਰ) + (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਵਿੱਥ,
		       clen);

		ਵਿੱਥ = 0;
		ਲੰਮਾਈ -= clen;
		ptr += clen;
		ਖੰਡ_nr++;
	}
	ਲੰਮਾਈ = EXT2_INODE_ਮਾਪ(fs->super);

	/* Verify the inode checksum. */
	fail_csum = !ext2fs_inode_csum_verify(fs, ino, iptr);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	ext2fs_ਅਦਲਾ_ਬਦਲੀ_inode_ਭਰਿਆ(fs, (ਬਣਤਰ ext2_inode_large *) iptr,
			       (ਬਣਤਰ ext2_inode_large *) iptr,
			       0, ਲੰਮਾਈ);
#ਜੇ_ਅੰਤ

	/* Update the inode cache bookkeeping */
	ਜੇ (!fail_csum) {
		fs->icache->cache_ਆਖਰੀ = cache_ਥਾਂ;
		fs->icache->cache[cache_ਥਾਂ].ino = ino;
	}
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(inode, iptr, (bufsize > ਲੰਮਾਈ) ? ਲੰਮਾਈ : bufsize);

	ਜੇ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !(flags & READ_INODE_NOCSUM) && fail_csum)
		ਵਾਪਸ EXT2_ET_INODE_CSUM_INVALID;

	ਵਾਪਸ 0;
}

errcode_t ext2fs_read_inode_ਭਰਿਆ(ext2_filsys fs, ext2_ino_t ino,
				 ਬਣਤਰ ext2_inode * inode, ਪੂਰਨ bufsize)
{
	ਵਾਪਸ ext2fs_read_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    ਬਣਤਰ ext2_inode * inode)
{
	ਵਾਪਸ ext2fs_read_inode2(fs, ino, inode,
				  ਆਕਾਰ(ਬਣਤਰ ext2_inode), 0);
}

errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
			      ਬਣਤਰ ext2_inode * inode, ਪੂਰਨ bufsize,
			      ਪੂਰਨ flags)
{
	blk64_t ਖੰਡ_nr;
	dgrp_t ਸਮੂਹ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ, ਵਿੱਥ;
	errcode_t retval = 0;
	ਬਣਤਰ ext2_inode_large *w_inode;
	ਅੱਖਰ *ptr;
	ਬਿਨਾਨਿਸ਼ਾਨੀ i;
	ਪੂਰਨ clen;
	ਪੂਰਨ ਲੰਮਾਈ = EXT2_INODE_ਮਾਪ(fs->super);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ (ext2fs_has_feature_journal_dev(fs->super))
		ਵਾਪਸ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	/* Check to see if user provided an override function */
	ਜੇ (fs->write_inode) {
		retval = (fs->write_inode)(fs, ino, inode);
		ਜੇ (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			ਵਾਪਸ retval;
	}

	ਜੇ ((ino == 0) || (ino > fs->super->s_inodes_ਗਿਣਤੀ))
		ਵਾਪਸ EXT2_ET_BAD_INODE_ਅੰਕ;

	/* Prepare our shadow buffer for read/modify/byteswap/write */
	retval = ext2fs_get_mem(ਲੰਮਾਈ, &w_inode);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਜੇ (bufsize < ਲੰਮਾਈ) {
		retval = ext2fs_read_inode2(fs, ino,
					    (ਬਣਤਰ ext2_inode *)w_inode,
					    ਲੰਮਾਈ, READ_INODE_NOCSUM);
		ਜੇ (retval)
			ਜਾਓ errout;
	}

	/* Check to see if the inode cache needs to be updated */
	ਜੇ (fs->icache) {
		ਲਈ (i=0; i < fs->icache->cache_ਮਾਪ; i++) {
			ਜੇ (fs->icache->cache[i].ino == ino) {
				ਸਮ੍ਰਿਤੀ_ਨਕਲ(fs->icache->cache[i].inode, inode,
				       (bufsize > ਲੰਮਾਈ) ? ਲੰਮਾਈ : bufsize);
				ਰੋਕੋ;
			}
		}
	} ਨਹੀਂਤਾਂ {
		retval = ext2fs_create_inode_cache(fs, 4);
		ਜੇ (retval)
			ਜਾਓ errout;
	}
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(w_inode, inode, (bufsize > ਲੰਮਾਈ) ? ਲੰਮਾਈ : bufsize);

	ਜੇ (!(fs->flags & EXT2_FLAG_RW)) {
		retval = EXT2_ET_RO_FILSYS;
		ਜਾਓ errout;
	}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	ext2fs_ਅਦਲਾ_ਬਦਲੀ_inode_ਭਰਿਆ(fs, w_inode, w_inode, 1, ਲੰਮਾਈ);
#ਜੇ_ਅੰਤ

	ਜੇ ((flags & WRITE_INODE_NOCSUM) == 0) {
		retval = ext2fs_inode_csum_ਸੈੱਟ(fs, ino, w_inode);
		ਜੇ (retval)
			ਜਾਓ errout;
	}

	ਸਮੂਹ = (ino - 1) / EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super);
	ਵਿੱਥ = ((ino - 1) % EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super)) *
		EXT2_INODE_ਮਾਪ(fs->super);
	ਖੰਡ = ਵਿੱਥ >> EXT2_ਖੰਡ_ਮਾਪ_ਬਿੱਟ(fs->super);
	ਖੰਡ_nr = ext2fs_inode_ਸਾਰਣੀ_loc(fs, (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਸਮੂਹ);
	ਜੇ (!ਖੰਡ_nr) {
		retval = EXT2_ET_MISSING_INODE_ਸਾਰਣੀ;
		ਜਾਓ errout;
	}
	ਜੇ ((ਖੰਡ_nr < fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ) ||
	    (ਖੰਡ_nr + fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ - 1 >=
	     ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super))) {
		retval = EXT2_ET_GDESC_BAD_INODE_ਸਾਰਣੀ;
		ਜਾਓ errout;
	}
	ਖੰਡ_nr += ਖੰਡ;

	ਵਿੱਥ &= (EXT2_ਖੰਡ_ਮਾਪ(fs->super) - 1);

	ptr = (ਅੱਖਰ *) w_inode;

	ਜਦੋਂ (ਲੰਮਾਈ) {
		clen = ਲੰਮਾਈ;
		ਜੇ ((ਵਿੱਥ + ਲੰਮਾਈ) > fs->blocksize)
			clen = fs->blocksize - ਵਿੱਥ;

		ਜੇ (fs->icache->ਬਫਰ_blk != ਖੰਡ_nr) {
			retval = io_channel_read_blk64(fs->io, ਖੰਡ_nr, 1,
						     fs->icache->ਬਫਰ);
			ਜੇ (retval)
				ਜਾਓ errout;
			fs->icache->ਬਫਰ_blk = ਖੰਡ_nr;
		}


		ਸਮ੍ਰਿਤੀ_ਨਕਲ((ਅੱਖਰ *) fs->icache->ਬਫਰ + (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਵਿੱਥ,
		       ptr, clen);

		retval = io_channel_write_blk64(fs->io, ਖੰਡ_nr, 1,
					      fs->icache->ਬਫਰ);
		ਜੇ (retval)
			ਜਾਓ errout;

		ਵਿੱਥ = 0;
		ptr += clen;
		ਲੰਮਾਈ -= clen;
		ਖੰਡ_nr++;
	}

	fs->flags |= EXT2_FLAG_ਬਦਲਿਆ;
errout:
	ext2fs_free_mem(&w_inode);
	ਵਾਪਸ retval;
}

errcode_t ext2fs_write_inode_ਭਰਿਆ(ext2_filsys fs, ext2_ino_t ino,
				  ਬਣਤਰ ext2_inode * inode, ਪੂਰਨ bufsize)
{
	ਵਾਪਸ ext2fs_write_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			     ਬਣਤਰ ext2_inode *inode)
{
	ਵਾਪਸ ext2fs_write_inode2(fs, ino, inode,
				   ਆਕਾਰ(ਬਣਤਰ ext2_inode), 0);
}

/*
 * This function should be called when writing a new inode.  It makes
 * sure that extra part of large inodes is initialized properly.
 */
errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
				 ਬਣਤਰ ext2_inode *inode)
{
	ਬਣਤਰ ext2_inode	*buf;
	ਪੂਰਨ			ਮਾਪ = EXT2_INODE_ਮਾਪ(fs->super);
	ਬਣਤਰ ext2_inode_large	*large_inode;
	errcode_t		retval;
	time_t			t = ext2fs_P_get_ਸਮਾਂ(fs);

	ਜੇ (!ext2fs_inode_xtime_get(inode, i_atime))
		ext2fs_inode_xtime_ਸੈੱਟ(inode, i_atime, t);
	ਜੇ (!ext2fs_inode_xtime_get(inode, i_ctime))
		ext2fs_inode_xtime_ਸੈੱਟ(inode, i_ctime, t);
	ਜੇ (!ext2fs_inode_xtime_get(inode, i_mtime))
		ext2fs_inode_xtime_ਸੈੱਟ(inode, i_mtime, t);

	ਜੇ (ਮਾਪ == ਆਕਾਰ(ਬਣਤਰ ext2_inode))
		ਵਾਪਸ ext2fs_write_inode_ਭਰਿਆ(fs, ino, inode,
					       ਆਕਾਰ(ਬਣਤਰ ext2_inode));

	buf = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਮਾਪ);
	ਜੇ (!buf)
		ਵਾਪਸ ENOMEM;

	memset(buf, 0, ਮਾਪ);
	*buf = *inode;

	large_inode = (ਬਣਤਰ ext2_inode_large *) buf;
	large_inode->i_extra_isize = ਆਕਾਰ(ਬਣਤਰ ext2_inode_large) -
		EXT2_GOOD_OLD_INODE_ਮਾਪ;
	ਜੇ (!large_inode->i_crtime)
		large_inode->i_crtime = t;

	retval = ext2fs_write_inode_ਭਰਿਆ(fs, ino, buf, ਮਾਪ);
	ਮੁਕਤ(buf);
	ਵਾਪਸ retval;
}


errcode_t ext2fs_get_ਖੰਡਾਂ(ext2_filsys fs, ext2_ino_t ino, blk_t *ਖੰਡਾਂ)
{
	ਬਣਤਰ ext2_inode	inode;
	ਪੂਰਨ			i;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ (ino > fs->super->s_inodes_ਗਿਣਤੀ)
		ਵਾਪਸ EXT2_ET_BAD_INODE_ਅੰਕ;

	ਜੇ (fs->get_ਖੰਡਾਂ) {
		ਜੇ (!(*fs->get_ਖੰਡਾਂ)(fs, ino, ਖੰਡਾਂ))
			ਵਾਪਸ 0;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ਲਈ (i=0; i < EXT2_N_ਖੰਡਾਂ; i++)
		ਖੰਡਾਂ[i] = inode.i_ਖੰਡ[i];
	ਵਾਪਸ 0;
}

errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino)
{
	ਬਣਤਰ	ext2_inode	inode;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ (ino > fs->super->s_inodes_ਗਿਣਤੀ)
		ਵਾਪਸ EXT2_ET_BAD_INODE_ਅੰਕ;

	ਜੇ (fs->check_directory) {
		retval = (fs->check_directory)(fs, ino);
		ਜੇ (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			ਵਾਪਸ retval;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ਜੇ (!LINUX_S_ISDIR(inode.i_ਬਹੁਲਕ))
		ਵਾਪਸ EXT2_ET_NO_DIRECTORY;
	ਵਾਪਸ 0;
}

