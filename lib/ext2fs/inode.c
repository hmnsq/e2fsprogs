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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <time.h>
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"
#ಅನ್ತರ್ಭಾವ "e2image.h"

#ವ್ಯಾಖ್ಯಾನಿಸು IBLOCK_ಸ್ಥಿತಿ_CSUMS_OK	1
#ವ್ಯಾಖ್ಯಾನಿಸು IBLOCK_ಸ್ಥಿತಿ_INSANE	2
#ವ್ಯಾಖ್ಯಾನಿಸು ಪರಿಶೀಲನೆ_ಖಂಡ_ಸ್ಥಿತಿ(ಪರಿಶೀಲನೆ)	((ಪರಿಶೀಲನೆ)->ತಾತ್ಕಾಲಿಕ_ಬಫರ್ + (ಪರಿಶೀಲನೆ)->inode_ಗಾತ್ರ)

ರಚನೆ ext2_struct_inode_ಪರಿಶೀಲನೆ {
	errcode_t		magic;
	ext2_filsys		fs;
	ext2_ino_t		current_inode;
	blk64_t			current_ಖಂಡ;
	dgrp_t			current_ಗುಂಪು;
	ext2_ino_t		inodes_ಎಡ;
	blk_t			ಖಂಡಗಳು_ಎಡ;
	dgrp_t			groups_ಎಡ;
	blk_t			inode_ಬಫರ್_ಖಂಡಗಳು;
	ಅಕ್ಷರ *			inode_ಬಫರ್;
	ಪೂರ್ಣಾಂಕ			inode_ಗಾತ್ರ;
	ಅಕ್ಷರ *			ptr;
	ಪೂರ್ಣಾಂಕ			ಬೈಟುಗಳು_ಎಡ;
	ಅಕ್ಷರ			*ತಾತ್ಕಾಲಿಕ_ಬಫರ್;
	errcode_t		(*ಮುಗಿದ_ಗುಂಪು)(ext2_filsys fs,
					      ext2_inode_ಪರಿಶೀಲನೆ ಪರಿಶೀಲನೆ,
					      dgrp_t ಗುಂಪು,
					      ಶೂನ್ಯ * priv_ದತ್ತಾಂಶ);
	ಶೂನ್ಯ *			ಮುಗಿದ_ಗುಂಪು_ದತ್ತಾಂಶ;
	ಪೂರ್ಣಾಂಕ			bad_ಖಂಡ_ptr;
	ಪೂರ್ಣಾಂಕ			ಪರಿಶೀಲನೆ_flags;
	ಪೂರ್ಣಾಂಕ			reserved[6];
};

/*
 * This routine flushes the icache, if it exists.
 */
errcode_t ext2fs_flush_icache(ext2_filsys fs)
{
	ಚಿಹ್ನೆರಹಿತ	i;

	ಆದರೆ (!fs->icache)
		ಹಿಂತಿರುಗಿ 0;

	ಗಾಗಿ (i=0; i < fs->icache->cache_ಗಾತ್ರ; i++)
		fs->icache->cache[i].ino = 0;

	fs->icache->ಬಫರ್_blk = 0;
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Free the inode cache structure
 */
ಶೂನ್ಯ ext2fs_free_inode_cache(ರಚನೆ ext2_inode_cache *icache)
{
	ಚಿಹ್ನೆರಹಿತ i;

	ಆದರೆ (--icache->refcount)
		ಹಿಂತಿರುಗಿ;
	ಆದರೆ (icache->ಬಫರ್)
		ext2fs_free_mem(&icache->ಬಫರ್);
	ಗಾಗಿ (i = 0; i < icache->cache_ಗಾತ್ರ; i++)
		ext2fs_free_mem(&icache->cache[i].inode);
	ಆದರೆ (icache->cache)
		ext2fs_free_mem(&icache->cache);
	icache->ಬಫರ್_blk = 0;
	ext2fs_free_mem(&icache);
}

errcode_t ext2fs_create_inode_cache(ext2_filsys fs, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cache_ಗಾತ್ರ)
{
	ಚಿಹ್ನೆರಹಿತ	i;
	errcode_t	retval;

	ಆದರೆ (fs->icache)
		ಹಿಂತಿರುಗಿ 0;
	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ ext2_inode_cache), &fs->icache);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	memset(fs->icache, 0, ಗಾತ್ರ(ರಚನೆ ext2_inode_cache));
	retval = ext2fs_get_mem(fs->blocksize, &fs->icache->ಬಫರ್);
	ಆದರೆ (retval)
		ಹೋಗು errout;

	fs->icache->ಬಫರ್_blk = 0;
	fs->icache->cache_ಕೊನೆಯ = -1;
	fs->icache->cache_ಗಾತ್ರ = cache_ಗಾತ್ರ;
	fs->icache->refcount = 1;
	retval = ext2fs_get_array(fs->icache->cache_ಗಾತ್ರ,
				  ಗಾತ್ರ(ರಚನೆ ext2_inode_cache_ent),
				  &fs->icache->cache);
	ಆದರೆ (retval)
		ಹೋಗು errout;

	ಗಾಗಿ (i = 0; i < fs->icache->cache_ಗಾತ್ರ; i++) {
		retval = ext2fs_get_mem(EXT2_INODE_ಗಾತ್ರ(fs->super),
					&fs->icache->cache[i].inode);
		ಆದರೆ (retval)
			ಹೋಗು errout;
	}

	ext2fs_flush_icache(fs);
	ಹಿಂತಿರುಗಿ 0;
errout:
	ext2fs_free_inode_cache(fs->icache);
	fs->icache = 0;
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_open_inode_ಪರಿಶೀಲನೆ(ext2_filsys fs, ಪೂರ್ಣಾಂಕ ಬಫರ್_ಖಂಡಗಳು,
				 ext2_inode_ಪರಿಶೀಲನೆ *ret_ಪರಿಶೀಲನೆ)
{
	ext2_inode_ಪರಿಶೀಲನೆ	ಪರಿಶೀಲನೆ;
	errcode_t	retval;
	errcode_t (*save_get_ಖಂಡಗಳು)(ext2_filsys f, ext2_ino_t ino, blk_t *ಖಂಡಗಳು);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ (ext2fs_has_feature_journal_dev(fs->super))
		ಹಿಂತಿರುಗಿ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	ಆದರೆ (fs->blocksize < 1024)
		ಹಿಂತಿರುಗಿ EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/*
	 * If fs->badblocks isn't set, then set it --- since the inode
	 * scanning functions require it.
	 */
	ಆದರೆ (fs->badblocks == 0) {
		/*
		 * Temporarily save fs->get_blocks and set it to zero,
		 * for compatibility with old e2fsck's.
		 */
		save_get_ಖಂಡಗಳು = fs->get_ಖಂಡಗಳು;
		fs->get_ಖಂಡಗಳು = 0;
		retval = ext2fs_read_bb_inode(fs, &fs->badblocks);
		ಆದರೆ (retval && fs->badblocks) {
			ext2fs_badblocks_list_free(fs->badblocks);
			fs->badblocks = 0;
		}
		fs->get_ಖಂಡಗಳು = save_get_ಖಂಡಗಳು;
	}

	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ ext2_struct_inode_ಪರಿಶೀಲನೆ), &ಪರಿಶೀಲನೆ);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	memset(ಪರಿಶೀಲನೆ, 0, ಗಾತ್ರ(ರಚನೆ ext2_struct_inode_ಪರಿಶೀಲನೆ));

	ಪರಿಶೀಲನೆ->magic = EXT2_ET_MAGIC_INODE_ಪರಿಶೀಲನೆ;
	ಪರಿಶೀಲನೆ->fs = fs;
	ಪರಿಶೀಲನೆ->inode_ಗಾತ್ರ = EXT2_INODE_ಗಾತ್ರ(fs->super);
	ಪರಿಶೀಲನೆ->ಬೈಟುಗಳು_ಎಡ = 0;
	ಪರಿಶೀಲನೆ->current_ಗುಂಪು = 0;
	ಪರಿಶೀಲನೆ->groups_ಎಡ = fs->ಗುಂಪು_desc_ಎಣಿಕೆ - 1;
	ಪರಿಶೀಲನೆ->inode_ಬಫರ್_ಖಂಡಗಳು = ಬಫರ್_ಖಂಡಗಳು ? ಬಫರ್_ಖಂಡಗಳು :
				    EXT2_INODE_ಪರಿಶೀಲನೆ_DEFAULT_ಬಫರ್_ಖಂಡಗಳು;
	ಪರಿಶೀಲನೆ->current_ಖಂಡ = ext2fs_inode_ಕೋಷ್ಟಕ_loc(ಪರಿಶೀಲನೆ->fs,
						     ಪರಿಶೀಲನೆ->current_ಗುಂಪು);
	ಆದರೆ (ಪರಿಶೀಲನೆ->current_ಖಂಡ &&
	    ((ಪರಿಶೀಲನೆ->current_ಖಂಡ < fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ) ||
	     (ಪರಿಶೀಲನೆ->current_ಖಂಡ + fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು - 1 >=
	      ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super)))) {
		ext2fs_free_mem(&ಪರಿಶೀಲನೆ);
		ಹಿಂತಿರುಗಿ EXT2_ET_GDESC_BAD_INODE_ಕೋಷ್ಟಕ;
	}

	ಪರಿಶೀಲನೆ->inodes_ಎಡ = EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(ಪರಿಶೀಲನೆ->fs->super);
	ಪರಿಶೀಲನೆ->ಖಂಡಗಳು_ಎಡ = ಪರಿಶೀಲನೆ->fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು;
	ಆದರೆ (ext2fs_has_ಗುಂಪು_desc_csum(fs)) {
		__u32 ಬಳಸದ = ext2fs_bg_itable_ಬಳಸದ(fs, ಪರಿಶೀಲನೆ->current_ಗುಂಪು);
		ಆದರೆ (ಪರಿಶೀಲನೆ->inodes_ಎಡ > ಬಳಸದ)
			ಪರಿಶೀಲನೆ->inodes_ಎಡ -= ಬಳಸದ;
		ಇಲ್ಲದಿದ್ದರೆ
			ಪರಿಶೀಲನೆ->inodes_ಎಡ = 0;
		ಪರಿಶೀಲನೆ->ಖಂಡಗಳು_ಎಡ =
			(ಪರಿಶೀಲನೆ->inodes_ಎಡ +
			 (fs->blocksize / ಪರಿಶೀಲನೆ->inode_ಗಾತ್ರ - 1)) *
			ಪರಿಶೀಲನೆ->inode_ಗಾತ್ರ / fs->blocksize;
	}
	retval = io_channel_alloc_buf(fs->io, ಪರಿಶೀಲನೆ->inode_ಬಫರ್_ಖಂಡಗಳು,
				      &ಪರಿಶೀಲನೆ->inode_ಬಫರ್);
	ಪರಿಶೀಲನೆ->ಮುಗಿದ_ಗುಂಪು = 0;
	ಪರಿಶೀಲನೆ->ಮುಗಿದ_ಗುಂಪು_ದತ್ತಾಂಶ = 0;
	ಪರಿಶೀಲನೆ->bad_ಖಂಡ_ptr = 0;
	ಆದರೆ (retval) {
		ext2fs_free_mem(&ಪರಿಶೀಲನೆ);
		ಹಿಂತಿರುಗಿ retval;
	}
	retval = ext2fs_get_mem(ಪರಿಶೀಲನೆ->inode_ಗಾತ್ರ + ಪರಿಶೀಲನೆ->inode_ಬಫರ್_ಖಂಡಗಳು,
				&ಪರಿಶೀಲನೆ->ತಾತ್ಕಾಲಿಕ_ಬಫರ್);
	ಆದರೆ (retval) {
		ext2fs_free_mem(&ಪರಿಶೀಲನೆ->inode_ಬಫರ್);
		ext2fs_free_mem(&ಪರಿಶೀಲನೆ);
		ಹಿಂತಿರುಗಿ retval;
	}
	memset(ಪರಿಶೀಲನೆ_ಖಂಡ_ಸ್ಥಿತಿ(ಪರಿಶೀಲನೆ), 0, ಪರಿಶೀಲನೆ->inode_ಬಫರ್_ಖಂಡಗಳು);
	ಆದರೆ (ಪರಿಶೀಲನೆ->fs->badblocks && ಪರಿಶೀಲನೆ->fs->badblocks->ಅಂಕೆ)
		ಪರಿಶೀಲನೆ->ಪರಿಶೀಲನೆ_flags |= EXT2_SF_CHK_BADBLOCKS;
	ಆದರೆ (ext2fs_has_ಗುಂಪು_desc_csum(fs))
		ಪರಿಶೀಲನೆ->ಪರಿಶೀಲನೆ_flags |= EXT2_SF_DO_LAZY;
	*ret_ಪರಿಶೀಲನೆ = ಪರಿಶೀಲನೆ;
	ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ ext2fs_close_inode_ಪರಿಶೀಲನೆ(ext2_inode_ಪರಿಶೀಲನೆ ಪರಿಶೀಲನೆ)
{
	ಆದರೆ (!ಪರಿಶೀಲನೆ || (ಪರಿಶೀಲನೆ->magic != EXT2_ET_MAGIC_INODE_ಪರಿಶೀಲನೆ))
		ಹಿಂತಿರುಗಿ;

	ext2fs_free_mem(&ಪರಿಶೀಲನೆ->inode_ಬಫರ್);
	ಪರಿಶೀಲನೆ->inode_ಬಫರ್ = ಖಾಲಿ;
	ext2fs_free_mem(&ಪರಿಶೀಲನೆ->ತಾತ್ಕಾಲಿಕ_ಬಫರ್);
	ಪರಿಶೀಲನೆ->ತಾತ್ಕಾಲಿಕ_ಬಫರ್ = ಖಾಲಿ;
	ext2fs_free_mem(&ಪರಿಶೀಲನೆ);
	ಹಿಂತಿರುಗಿ;
}

ಶೂನ್ಯ ext2fs_ಗಣ_inode_callback(ext2_inode_ಪರಿಶೀಲನೆ ಪರಿಶೀಲನೆ,
			       errcode_t (*ಮುಗಿದ_ಗುಂಪು)(ext2_filsys fs,
						       ext2_inode_ಪರಿಶೀಲನೆ ಪರಿಶೀಲನೆ,
						       dgrp_t ಗುಂಪು,
						       ಶೂನ್ಯ * priv_ದತ್ತಾಂಶ),
			       ಶೂನ್ಯ *ಮುಗಿದ_ಗುಂಪು_ದತ್ತಾಂಶ)
{
	ಆದರೆ (!ಪರಿಶೀಲನೆ || (ಪರಿಶೀಲನೆ->magic != EXT2_ET_MAGIC_INODE_ಪರಿಶೀಲನೆ))
		ಹಿಂತಿರುಗಿ;

	ಪರಿಶೀಲನೆ->ಮುಗಿದ_ಗುಂಪು = ಮುಗಿದ_ಗುಂಪು;
	ಪರಿಶೀಲನೆ->ಮುಗಿದ_ಗುಂಪು_ದತ್ತಾಂಶ = ಮುಗಿದ_ಗುಂಪು_ದತ್ತಾಂಶ;
}

ಪೂರ್ಣಾಂಕ ext2fs_inode_ಪರಿಶೀಲನೆ_flags(ext2_inode_ಪರಿಶೀಲನೆ ಪರಿಶೀಲನೆ, ಪೂರ್ಣಾಂಕ ಗಣ_flags,
			    ಪೂರ್ಣಾಂಕ clear_flags)
{
	ಪೂರ್ಣಾಂಕ	old_flags;

	ಆದರೆ (!ಪರಿಶೀಲನೆ || (ಪರಿಶೀಲನೆ->magic != EXT2_ET_MAGIC_INODE_ಪರಿಶೀಲನೆ))
		ಹಿಂತಿರುಗಿ 0;

	old_flags = ಪರಿಶೀಲನೆ->ಪರಿಶೀಲನೆ_flags;
	ಪರಿಶೀಲನೆ->ಪರಿಶೀಲನೆ_flags &= ~clear_flags;
	ಪರಿಶೀಲನೆ->ಪರಿಶೀಲನೆ_flags |= ಗಣ_flags;
	ಹಿಂತಿರುಗಿ old_flags;
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * get ready to read in a new blockgroup.
 */
ಸ್ಥಿರ errcode_t get_ಮುಂದಿನ_blockgroup(ext2_inode_ಪರಿಶೀಲನೆ ಪರಿಶೀಲನೆ)
{
	ext2_filsys fs = ಪರಿಶೀಲನೆ->fs;

	ಪರಿಶೀಲನೆ->current_ಗುಂಪು++;
	ಪರಿಶೀಲನೆ->groups_ಎಡ--;

	ಪರಿಶೀಲನೆ->current_ಖಂಡ = ext2fs_inode_ಕೋಷ್ಟಕ_loc(ಪರಿಶೀಲನೆ->fs,
						     ಪರಿಶೀಲನೆ->current_ಗುಂಪು);
	ಪರಿಶೀಲನೆ->current_inode = ಪರಿಶೀಲನೆ->current_ಗುಂಪು *
		EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(fs->super);

	ಪರಿಶೀಲನೆ->ಬೈಟುಗಳು_ಎಡ = 0;
	ಪರಿಶೀಲನೆ->inodes_ಎಡ = EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(fs->super);
	ಪರಿಶೀಲನೆ->ಖಂಡಗಳು_ಎಡ = fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು;
	ಆದರೆ (ext2fs_has_ಗುಂಪು_desc_csum(fs)) {
		__u32 ಬಳಸದ = ext2fs_bg_itable_ಬಳಸದ(fs, ಪರಿಶೀಲನೆ->current_ಗುಂಪು);
		ಆದರೆ (ಪರಿಶೀಲನೆ->inodes_ಎಡ > ಬಳಸದ)
			ಪರಿಶೀಲನೆ->inodes_ಎಡ -= ಬಳಸದ;
		ಇಲ್ಲದಿದ್ದರೆ
			ಪರಿಶೀಲನೆ->inodes_ಎಡ = 0;
		ಪರಿಶೀಲನೆ->ಖಂಡಗಳು_ಎಡ =
			(ಪರಿಶೀಲನೆ->inodes_ಎಡ +
			 (fs->blocksize / ಪರಿಶೀಲನೆ->inode_ಗಾತ್ರ - 1)) *
			ಪರಿಶೀಲನೆ->inode_ಗಾತ್ರ / fs->blocksize;
	}
	ಆದರೆ (ಪರಿಶೀಲನೆ->current_ಖಂಡ &&
	    ((ಪರಿಶೀಲನೆ->current_ಖಂಡ < fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ) ||
	     (ಪರಿಶೀಲನೆ->current_ಖಂಡ + fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು - 1 >=
	      ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super))))
		ಹಿಂತಿರುಗಿ EXT2_ET_GDESC_BAD_INODE_ಕೋಷ್ಟಕ;
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_inode_ಪರಿಶೀಲನೆ_goto_blockgroup(ext2_inode_ಪರಿಶೀಲನೆ ಪರಿಶೀಲನೆ,
					    ಪೂರ್ಣಾಂಕ	ಗುಂಪು)
{
	ಪರಿಶೀಲನೆ->current_ಗುಂಪು = ಗುಂಪು - 1;
	ಪರಿಶೀಲನೆ->groups_ಎಡ = ಪರಿಶೀಲನೆ->fs->ಗುಂಪು_desc_ಎಣಿಕೆ - ಗುಂಪು;
	ಪರಿಶೀಲನೆ->bad_ಖಂಡ_ptr = 0;
	ಹಿಂತಿರುಗಿ get_ಮುಂದಿನ_blockgroup(ಪರಿಶೀಲನೆ);
}

/*
 * This function is called by get_next_blocks() to check for bad
 * blocks in the inode table.
 *
 * This function assumes that badblocks_list->list is sorted in
 * increasing order.
 */
ಸ್ಥಿರ errcode_t check_for_inode_bad_ಖಂಡಗಳು(ext2_inode_ಪರಿಶೀಲನೆ ಪರಿಶೀಲನೆ,
					    blk64_t *ಅಂಕೆ_ಖಂಡಗಳು)
{
	blk64_t	blk = ಪರಿಶೀಲನೆ->current_ಖಂಡ;
	badblocks_list	bb = ಪರಿಶೀಲನೆ->fs->badblocks;

	/*
	 * If the inode table is missing, then obviously there are no
	 * bad blocks.  :-)
	 */
	ಆದರೆ (blk == 0)
		ಹಿಂತಿರುಗಿ 0;

	/* Make sure bad_block_ptr is still valid */
	ಆದರೆ (ಪರಿಶೀಲನೆ->bad_ಖಂಡ_ptr >= bb->ಅಂಕೆ) {
		ಪರಿಶೀಲನೆ->ಪರಿಶೀಲನೆ_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		ಹಿಂತಿರುಗಿ 0;
	}

	/*
	 * If the current block is greater than the bad block listed
	 * in the bad block list, then advance the pointer until this
	 * is no longer the case.  If we run out of bad blocks, then
	 * we don't need to do any more checking!
	 */
	ಆಗ (blk > bb->list[ಪರಿಶೀಲನೆ->bad_ಖಂಡ_ptr]) {
		ಆದರೆ (++ಪರಿಶೀಲನೆ->bad_ಖಂಡ_ptr >= bb->ಅಂಕೆ) {
			ಪರಿಶೀಲನೆ->ಪರಿಶೀಲನೆ_flags &= ~EXT2_SF_CHK_BADBLOCKS;
			ಹಿಂತಿರುಗಿ 0;
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
	ಆದರೆ (blk == bb->list[ಪರಿಶೀಲನೆ->bad_ಖಂಡ_ptr]) {
		ಪರಿಶೀಲನೆ->ಪರಿಶೀಲನೆ_flags |= EXT2_SF_BAD_INODE_BLK;
		*ಅಂಕೆ_ಖಂಡಗಳು = 1;
		ಆದರೆ (++ಪರಿಶೀಲನೆ->bad_ಖಂಡ_ptr >= bb->ಅಂಕೆ)
			ಪರಿಶೀಲನೆ->ಪರಿಶೀಲನೆ_flags &= ~EXT2_SF_CHK_BADBLOCKS;
		ಹಿಂತಿರುಗಿ 0;
	}

	/*
	 * If there is a bad block in the range that we're about to
	 * read in, adjust the number of blocks to read so that we we
	 * don't read in the bad block.  (Then the next block to read
	 * will be the bad block, which is handled in the above case.)
	 */
	ಆದರೆ ((blk + *ಅಂಕೆ_ಖಂಡಗಳು) > bb->list[ಪರಿಶೀಲನೆ->bad_ಖಂಡ_ptr])
		*ಅಂಕೆ_ಖಂಡಗಳು = (ಪೂರ್ಣಾಂಕ) (bb->list[ಪರಿಶೀಲನೆ->bad_ಖಂಡ_ptr] - blk);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಖಂಡ_map_looks_insane(ext2_filsys fs,
				  ರಚನೆ ext2_inode_large *inode)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i, bad;

	/* We're only interested in block mapped files, dirs, and symlinks */
	ಆದರೆ ((inode->i_flags & EXT4_INLINE_ದತ್ತಾಂಶ_FL) ||
	    (inode->i_flags & EXT4_EXTENTS_FL))
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (!LINUX_S_ISREG(inode->i_ಬಹುಲಕ) &&
	    !LINUX_S_ISLNK(inode->i_ಬಹುಲಕ) &&
	    !LINUX_S_ISDIR(inode->i_ಬಹುಲಕ))
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (LINUX_S_ISLNK(inode->i_ಬಹುಲಕ) &&
	    EXT2_I_ಗಾತ್ರ(inode) <= ಗಾತ್ರ(inode->i_ಖಂಡ))
		ಹಿಂತಿರುಗಿ 0;

	/* Unused inodes probably aren't insane */
	ಆದರೆ (inode->i_links_ಎಣಿಕೆ == 0)
		ಹಿಂತಿರುಗಿ 0;

	/* See if more than half the block maps are insane */
	ಗಾಗಿ (i = 0, bad = 0; i < EXT2_N_ಖಂಡಗಳು; i++)
		ಆದರೆ (inode->i_ಖಂಡ[i] != 0 &&
		    (inode->i_ಖಂಡ[i] < fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ ||
		     inode->i_ಖಂಡ[i] >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super)))
			bad++;
	ಹಿಂತಿರುಗಿ bad > EXT2_N_ಖಂಡಗಳು / 2;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ extent_ತಲೆ_looks_insane(ರಚನೆ ext2_inode_large *inode)
{
	ಆದರೆ (!(inode->i_flags & EXT4_EXTENTS_FL) ||
	    ext2fs_extent_ಶೀರ್ಷಿಕೆ_verify(inode->i_ಖಂಡ,
					ಗಾತ್ರ(inode->i_ಖಂಡ)) == 0)
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ 1;
}

/*
 * Check all the inodes that we just read into the buffer.  Record what we
 * find here -- currently, we can observe that all checksums are ok; more
 * than half the inodes are insane; or no conclusions at all.
 */
ಸ್ಥಿರ ಶೂನ್ಯ check_inode_ಖಂಡ_sanity(ext2_inode_ಪರಿಶೀಲನೆ ಪರಿಶೀಲನೆ, blk64_t ಅಂಕೆ_ಖಂಡಗಳು)
{
	ext2_ino_t	ino, inodes_ಗಮ್ಯ_ಪರಿಶೀಲನೆ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	badness, checksum_failures;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	inodes_ಒಳ_buf, inodes_ಪ್ರತಿ_ಖಂಡ;
	ಅಕ್ಷರ		*p;
	ರಚನೆ ext2_inode_large *inode;
	ಅಕ್ಷರ		*ಖಂಡ_ಸ್ಥಿತಿ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	blk, bad_csum;

	ಆದರೆ (!(ಪರಿಶೀಲನೆ->ಪರಿಶೀಲನೆ_flags & EXT2_SF_WARN_GARBAGE_INODES))
		ಹಿಂತಿರುಗಿ;

	inodes_ಗಮ್ಯ_ಪರಿಶೀಲನೆ = ಪರಿಶೀಲನೆ->inodes_ಎಡ;
	inodes_ಒಳ_buf = ಅಂಕೆ_ಖಂಡಗಳು * ಪರಿಶೀಲನೆ->fs->blocksize / ಪರಿಶೀಲನೆ->inode_ಗಾತ್ರ;
	ಆದರೆ (inodes_ಗಮ್ಯ_ಪರಿಶೀಲನೆ > inodes_ಒಳ_buf)
		inodes_ಗಮ್ಯ_ಪರಿಶೀಲನೆ = inodes_ಒಳ_buf;

	p = (ಅಕ್ಷರ *) ಪರಿಶೀಲನೆ->inode_ಬಫರ್;
	ino = ಪರಿಶೀಲನೆ->current_inode + 1;
	checksum_failures = badness = 0;
	ಖಂಡ_ಸ್ಥಿತಿ = ಪರಿಶೀಲನೆ_ಖಂಡ_ಸ್ಥಿತಿ(ಪರಿಶೀಲನೆ);
	memset(ಖಂಡ_ಸ್ಥಿತಿ, 0, ಪರಿಶೀಲನೆ->inode_ಬಫರ್_ಖಂಡಗಳು);
	inodes_ಪ್ರತಿ_ಖಂಡ = EXT2_INODES_ಪ್ರತಿ_ಖಂಡ(ಪರಿಶೀಲನೆ->fs->super);

	ಆದರೆ (inodes_ಪ್ರತಿ_ಖಂಡ < 2)
		ಹಿಂತಿರುಗಿ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	ಆದರೆ (ext2fs_get_mem(EXT2_INODE_ಗಾತ್ರ(ಪರಿಶೀಲನೆ->fs->super), &inode))
		ಹಿಂತಿರುಗಿ;
#ಆದರೆ_ಅಂತ್ಯ

	ಆಗ (inodes_ಗಮ್ಯ_ಪರಿಶೀಲನೆ > 0) {
		blk = (p - (ಅಕ್ಷರ *)ಪರಿಶೀಲನೆ->inode_ಬಫರ್) / ಪರಿಶೀಲನೆ->fs->blocksize;
		bad_csum = ext2fs_inode_csum_verify(ಪರಿಶೀಲನೆ->fs, ino,
				(ರಚನೆ ext2_inode_large *) p) == 0;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
		ext2fs_ಅದಲುಬದಲು_inode_ತುಂಬಿದ(ಪರಿಶೀಲನೆ->fs,
			       (ರಚನೆ ext2_inode_large *) inode,
			       (ರಚನೆ ext2_inode_large *) p,
			       0, EXT2_INODE_ಗಾತ್ರ(ಪರಿಶೀಲನೆ->fs->super));
#ಇಲ್ಲದಿದ್ದರೆ
		inode = (ರಚನೆ ext2_inode_large *) p;
#ಆದರೆ_ಅಂತ್ಯ

		/* Is this inode insane? */
		ಆದರೆ (bad_csum) {
			checksum_failures++;
			badness++;
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (extent_ತಲೆ_looks_insane(inode) ||
			   ಖಂಡ_map_looks_insane(ಪರಿಶೀಲನೆ->fs, inode))
			badness++;

		/* If more than half are insane, declare the whole block bad */
		ಆದರೆ (badness > inodes_ಪ್ರತಿ_ಖಂಡ / 2) {
			ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ino_adj;

			ಖಂಡ_ಸ್ಥಿತಿ[blk] |= IBLOCK_ಸ್ಥಿತಿ_INSANE;
			ino_adj = inodes_ಪ್ರತಿ_ಖಂಡ -
						((ino - 1) % inodes_ಪ್ರತಿ_ಖಂಡ);
			ಆದರೆ (ino_adj > inodes_ಗಮ್ಯ_ಪರಿಶೀಲನೆ)
				ino_adj = inodes_ಗಮ್ಯ_ಪರಿಶೀಲನೆ;
			inodes_ಗಮ್ಯ_ಪರಿಶೀಲನೆ -= ino_adj;
			p += ಪರಿಶೀಲನೆ->inode_ಗಾತ್ರ * ino_adj;
			ino += ino_adj;
			checksum_failures = badness = 0;
			ಮುಂದುವರಿ;
		}

		ಆದರೆ ((ino % inodes_ಪ್ರತಿ_ಖಂಡ) == 0) {
			ಆದರೆ (checksum_failures == 0)
				ಖಂಡ_ಸ್ಥಿತಿ[blk] |= IBLOCK_ಸ್ಥಿತಿ_CSUMS_OK;
			checksum_failures = badness = 0;
		}
		inodes_ಗಮ್ಯ_ಪರಿಶೀಲನೆ--;
		p += ಪರಿಶೀಲನೆ->inode_ಗಾತ್ರ;
		ino++;
	};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	ext2fs_free_mem(&inode);
#ಆದರೆ_ಅಂತ್ಯ
}

/*
 * This function is called by ext2fs_get_next_inode when it needs to
 * read in more blocks from the current blockgroup's inode table.
 */
ಸ್ಥಿರ errcode_t get_ಮುಂದಿನ_ಖಂಡಗಳು(ext2_inode_ಪರಿಶೀಲನೆ ಪರಿಶೀಲನೆ)
{
	blk64_t		ಅಂಕೆ_ಖಂಡಗಳು;
	errcode_t	retval;

	/*
	 * Figure out how many blocks to read; we read at most
	 * inode_buffer_blocks, and perhaps less if there aren't that
	 * many blocks left to read.
	 */
	ಅಂಕೆ_ಖಂಡಗಳು = ಪರಿಶೀಲನೆ->inode_ಬಫರ್_ಖಂಡಗಳು;
	ಆದರೆ (ಅಂಕೆ_ಖಂಡಗಳು > ಪರಿಶೀಲನೆ->ಖಂಡಗಳು_ಎಡ)
		ಅಂಕೆ_ಖಂಡಗಳು = ಪರಿಶೀಲನೆ->ಖಂಡಗಳು_ಎಡ;

	/*
	 * If the past block "read" was a bad block, then mark the
	 * left-over extra bytes as also being bad.
	 */
	ಆದರೆ (ಪರಿಶೀಲನೆ->ಪರಿಶೀಲನೆ_flags & EXT2_SF_BAD_INODE_BLK) {
		ಆದರೆ (ಪರಿಶೀಲನೆ->ಬೈಟುಗಳು_ಎಡ)
			ಪರಿಶೀಲನೆ->ಪರಿಶೀಲನೆ_flags |= EXT2_SF_BAD_EXTRA_ಬೈಟುಗಳು;
		ಪರಿಶೀಲನೆ->ಪರಿಶೀಲನೆ_flags &= ~EXT2_SF_BAD_INODE_BLK;
	}

	/*
	 * Do inode bad block processing, if necessary.
	 */
	ಆದರೆ (ಪರಿಶೀಲನೆ->ಪರಿಶೀಲನೆ_flags & EXT2_SF_CHK_BADBLOCKS) {
		retval = check_for_inode_bad_ಖಂಡಗಳು(ಪರಿಶೀಲನೆ, &ಅಂಕೆ_ಖಂಡಗಳು);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}

	ಆದರೆ ((ಪರಿಶೀಲನೆ->ಪರಿಶೀಲನೆ_flags & EXT2_SF_BAD_INODE_BLK) ||
	    (ಪರಿಶೀಲನೆ->current_ಖಂಡ == 0)) {
		memset(ಪರಿಶೀಲನೆ->inode_ಬಫರ್, 0,
		       (size_t) ಅಂಕೆ_ಖಂಡಗಳು * ಪರಿಶೀಲನೆ->fs->blocksize);
	} ಇಲ್ಲದಿದ್ದರೆ {
		retval = io_channel_read_blk64(ಪರಿಶೀಲನೆ->fs->io,
					     ಪರಿಶೀಲನೆ->current_ಖಂಡ,
					     (ಪೂರ್ಣಾಂಕ) ಅಂಕೆ_ಖಂಡಗಳು,
					     ಪರಿಶೀಲನೆ->inode_ಬಫರ್);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ EXT2_ET_ಮುಂದಿನ_INODE_READ;
	}
	check_inode_ಖಂಡ_sanity(ಪರಿಶೀಲನೆ, ಅಂಕೆ_ಖಂಡಗಳು);

	ಪರಿಶೀಲನೆ->ptr = ಪರಿಶೀಲನೆ->inode_ಬಫರ್;
	ಪರಿಶೀಲನೆ->ಬೈಟುಗಳು_ಎಡ = ಅಂಕೆ_ಖಂಡಗಳು * ಪರಿಶೀಲನೆ->fs->blocksize;

	ಪರಿಶೀಲನೆ->ಖಂಡಗಳು_ಎಡ -= ಅಂಕೆ_ಖಂಡಗಳು;
	ಆದರೆ (ಪರಿಶೀಲನೆ->current_ಖಂಡ)
		ಪರಿಶೀಲನೆ->current_ಖಂಡ += ಅಂಕೆ_ಖಂಡಗಳು;

	ಹಿಂತಿರುಗಿ 0;
}

#ಆದರೆ 0
/*
 * Returns 1 if the entire inode_buffer has a non-zero size and
 * contains all zeros.  (Not just deleted inodes, since that means
 * that part of the inode table was used at one point; we want all
 * zeros, which means that the inode table is pristine.)
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ is_ಖಾಲಿಗಳು_ಪರಿಶೀಲನೆ(ext2_inode_ಪರಿಶೀಲನೆ ಪರಿಶೀಲನೆ)
{
	ಪೂರ್ಣಾಂಕ	i;

	ಆದರೆ (ಪರಿಶೀಲನೆ->ಬೈಟುಗಳು_ಎಡ == 0)
		ಹಿಂತಿರುಗಿ 0;

	ಗಾಗಿ (i=0; i < ಪರಿಶೀಲನೆ->ಬೈಟುಗಳು_ಎಡ; i++)
		ಆದರೆ (ಪರಿಶೀಲನೆ->ptr[i])
			ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ 1;
}
#ಆದರೆ_ಅಂತ್ಯ

errcode_t ext2fs_get_ಮುಂದಿನ_inode_ತುಂಬಿದ(ext2_inode_ಪರಿಶೀಲನೆ ಪರಿಶೀಲನೆ, ext2_ino_t *ino,
				     ರಚನೆ ext2_inode *inode, ಪೂರ್ಣಾಂಕ bufsize)
{
	errcode_t	retval;
	ಪೂರ್ಣಾಂಕ		extra_ಬೈಟುಗಳು = 0;
	ಪೂರ್ಣಾಂಕ		ಉದ್ದಳತೆ;
	ರಚನೆ ext2_inode_large	*iptr = (ರಚನೆ ext2_inode_large *)inode;
	ಅಕ್ಷರ		*iblock_ಸ್ಥಿತಿ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	iblk;

	EXT2_CHECK_MAGIC(ಪರಿಶೀಲನೆ, EXT2_ET_MAGIC_INODE_ಪರಿಶೀಲನೆ);
	ಉದ್ದಳತೆ = EXT2_INODE_ಗಾತ್ರ(ಪರಿಶೀಲನೆ->fs->super);
	iblock_ಸ್ಥಿತಿ = ಪರಿಶೀಲನೆ_ಖಂಡ_ಸ್ಥಿತಿ(ಪರಿಶೀಲನೆ);

	/*
	 * Do we need to start reading a new block group?
	 */
	ಆದರೆ (ಪರಿಶೀಲನೆ->inodes_ಎಡ <= 0) {
	force_new_ಗುಂಪು:
		ಆದರೆ (ಪರಿಶೀಲನೆ->ಮುಗಿದ_ಗುಂಪು) {
			retval = (ಪರಿಶೀಲನೆ->ಮುಗಿದ_ಗುಂಪು)
				(ಪರಿಶೀಲನೆ->fs, ಪರಿಶೀಲನೆ, ಪರಿಶೀಲನೆ->current_ಗುಂಪು,
				 ಪರಿಶೀಲನೆ->ಮುಗಿದ_ಗುಂಪು_ದತ್ತಾಂಶ);
			ಆದರೆ (retval)
				ಹಿಂತಿರುಗಿ retval;
		}
		ಆದರೆ (ಪರಿಶೀಲನೆ->groups_ಎಡ <= 0) {
			*ino = 0;
			ಹಿಂತಿರುಗಿ 0;
		}
		retval = get_ಮುಂದಿನ_blockgroup(ಪರಿಶೀಲನೆ);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}
	/*
	 * These checks are done outside the above if statement so
	 * they can be done for block group #0.
	 */
	ಆದರೆ ((ಪರಿಶೀಲನೆ->ಪರಿಶೀಲನೆ_flags & EXT2_SF_DO_LAZY) &&
	    (ext2fs_bg_flags_test(ಪರಿಶೀಲನೆ->fs, ಪರಿಶೀಲನೆ->current_ಗುಂಪು, EXT2_BG_INODE_UNINIT)
	     ))
		ಹೋಗು force_new_ಗುಂಪು;
	ಆದರೆ (ಪರಿಶೀಲನೆ->inodes_ಎಡ == 0)
		ಹೋಗು force_new_ಗುಂಪು;
	ಆದರೆ (ಪರಿಶೀಲನೆ->current_ಖಂಡ == 0) {
		ಆದರೆ (ಪರಿಶೀಲನೆ->ಪರಿಶೀಲನೆ_flags & EXT2_SF_SKIP_MISSING_ITABLE) {
			ಹೋಗು force_new_ಗುಂಪು;
		} ಇಲ್ಲದಿದ್ದರೆ
			ಹಿಂತಿರುಗಿ EXT2_ET_MISSING_INODE_ಕೋಷ್ಟಕ;
	}


	/*
	 * Have we run out of space in the inode buffer?  If so, we
	 * need to read in more blocks.
	 */
	ಆದರೆ (ಪರಿಶೀಲನೆ->ಬೈಟುಗಳು_ಎಡ < ಪರಿಶೀಲನೆ->inode_ಗಾತ್ರ) {
		ಆದರೆ (ಪರಿಶೀಲನೆ->ಬೈಟುಗಳು_ಎಡ)
			ಸ್ಮರಣೆ_ನಕಲು(ಪರಿಶೀಲನೆ->ತಾತ್ಕಾಲಿಕ_ಬಫರ್, ಪರಿಶೀಲನೆ->ptr, ಪರಿಶೀಲನೆ->ಬೈಟುಗಳು_ಎಡ);
		extra_ಬೈಟುಗಳು = ಪರಿಶೀಲನೆ->ಬೈಟುಗಳು_ಎಡ;

		retval = get_ಮುಂದಿನ_ಖಂಡಗಳು(ಪರಿಶೀಲನೆ);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
#ಆದರೆ 0
		/*
		 * XXX test  Need check for used inode somehow.
		 * (Note: this is hard.)
		 */
		ಆದರೆ (is_ಖಾಲಿಗಳು_ಪರಿಶೀಲನೆ(ಪರಿಶೀಲನೆ))
			ಹೋಗು force_new_ಗುಂಪು;
#ಆದರೆ_ಅಂತ್ಯ
	}

	ಆದರೆ (bufsize < ಉದ್ದಳತೆ) {
		retval = ext2fs_get_mem(ಉದ್ದಳತೆ, &iptr);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}

	retval = 0;
	iblk = ಪರಿಶೀಲನೆ->current_inode % EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(ಪರಿಶೀಲನೆ->fs->super) /
				EXT2_INODES_ಪ್ರತಿ_ಖಂಡ(ಪರಿಶೀಲನೆ->fs->super) %
				ಪರಿಶೀಲನೆ->inode_ಬಫರ್_ಖಂಡಗಳು;
	ಆದರೆ (extra_ಬೈಟುಗಳು) {
		ಸ್ಮರಣೆ_ನಕಲು(ಪರಿಶೀಲನೆ->ತಾತ್ಕಾಲಿಕ_ಬಫರ್+extra_ಬೈಟುಗಳು, ಪರಿಶೀಲನೆ->ptr,
		       ಪರಿಶೀಲನೆ->inode_ಗಾತ್ರ - extra_ಬೈಟುಗಳು);
		ಪರಿಶೀಲನೆ->ptr += ಪರಿಶೀಲನೆ->inode_ಗಾತ್ರ - extra_ಬೈಟುಗಳು;
		ಪರಿಶೀಲನೆ->ಬೈಟುಗಳು_ಎಡ -= ಪರಿಶೀಲನೆ->inode_ಗಾತ್ರ - extra_ಬೈಟುಗಳು;

		/* Verify the inode checksum. */
		ಆದರೆ (!(iblock_ಸ್ಥಿತಿ[iblk] & IBLOCK_ಸ್ಥಿತಿ_CSUMS_OK) &&
		    !(ಪರಿಶೀಲನೆ->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(ಪರಿಶೀಲನೆ->fs, ಪರಿಶೀಲನೆ->current_inode + 1,
				(ರಚನೆ ext2_inode_large *)ಪರಿಶೀಲನೆ->ತಾತ್ಕಾಲಿಕ_ಬಫರ್))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
		memset(iptr, 0, ಉದ್ದಳತೆ);
		ext2fs_ಅದಲುಬದಲು_inode_ತುಂಬಿದ(ಪರಿಶೀಲನೆ->fs,
			       (ರಚನೆ ext2_inode_large *) iptr,
			       (ರಚನೆ ext2_inode_large *) ಪರಿಶೀಲನೆ->ತಾತ್ಕಾಲಿಕ_ಬಫರ್,
			       0, ಉದ್ದಳತೆ);
#ಇಲ್ಲದಿದ್ದರೆ
		ಸ್ಮರಣೆ_ನಕಲು(iptr, ಪರಿಶೀಲನೆ->ತಾತ್ಕಾಲಿಕ_ಬಫರ್, ಉದ್ದಳತೆ);
#ಆದರೆ_ಅಂತ್ಯ
		ಆದರೆ (ಪರಿಶೀಲನೆ->ಪರಿಶೀಲನೆ_flags & EXT2_SF_BAD_EXTRA_ಬೈಟುಗಳು)
			retval = EXT2_ET_BAD_ಖಂಡ_ಒಳ_INODE_ಕೋಷ್ಟಕ;
		ಪರಿಶೀಲನೆ->ಪರಿಶೀಲನೆ_flags &= ~EXT2_SF_BAD_EXTRA_ಬೈಟುಗಳು;
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* Verify the inode checksum. */
		ಆದರೆ (!(iblock_ಸ್ಥಿತಿ[iblk] & IBLOCK_ಸ್ಥಿತಿ_CSUMS_OK) &&
		    !(ಪರಿಶೀಲನೆ->fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
		    !ext2fs_inode_csum_verify(ಪರಿಶೀಲನೆ->fs, ಪರಿಶೀಲನೆ->current_inode + 1,
				(ರಚನೆ ext2_inode_large *)ಪರಿಶೀಲನೆ->ptr))
			retval = EXT2_ET_INODE_CSUM_INVALID;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
		memset(iptr, 0, ಉದ್ದಳತೆ);
		ext2fs_ಅದಲುಬದಲು_inode_ತುಂಬಿದ(ಪರಿಶೀಲನೆ->fs,
				(ರಚನೆ ext2_inode_large *) iptr,
				(ರಚನೆ ext2_inode_large *) ಪರಿಶೀಲನೆ->ptr,
				0, ಉದ್ದಳತೆ);
#ಇಲ್ಲದಿದ್ದರೆ
		ಸ್ಮರಣೆ_ನಕಲು(iptr, ಪರಿಶೀಲನೆ->ptr, ಉದ್ದಳತೆ);
#ಆದರೆ_ಅಂತ್ಯ
		ಪರಿಶೀಲನೆ->ptr += ಪರಿಶೀಲನೆ->inode_ಗಾತ್ರ;
		ಪರಿಶೀಲನೆ->ಬೈಟುಗಳು_ಎಡ -= ಪರಿಶೀಲನೆ->inode_ಗಾತ್ರ;
		ಆದರೆ (ಪರಿಶೀಲನೆ->ಪರಿಶೀಲನೆ_flags & EXT2_SF_BAD_INODE_BLK)
			retval = EXT2_ET_BAD_ಖಂಡ_ಒಳ_INODE_ಕೋಷ್ಟಕ;
	}
	ಆದರೆ ((iblock_ಸ್ಥಿತಿ[iblk] & IBLOCK_ಸ್ಥಿತಿ_INSANE) &&
	    (retval == 0 || retval == EXT2_ET_INODE_CSUM_INVALID))
		retval = EXT2_ET_INODE_IS_GARBAGE;

	ಪರಿಶೀಲನೆ->inodes_ಎಡ--;
	ಪರಿಶೀಲನೆ->current_inode++;
	*ino = ಪರಿಶೀಲನೆ->current_inode;
	ಆದರೆ (iptr != (ರಚನೆ ext2_inode_large *)inode) {
		ಸ್ಮರಣೆ_ನಕಲು(inode, iptr, bufsize);
		ext2fs_free_mem(&iptr);
	}
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_get_ಮುಂದಿನ_inode(ext2_inode_ಪರಿಶೀಲನೆ ಪರಿಶೀಲನೆ, ext2_ino_t *ino,
				ರಚನೆ ext2_inode *inode)
{
	ಹಿಂತಿರುಗಿ ext2fs_get_ಮುಂದಿನ_inode_ತುಂಬಿದ(ಪರಿಶೀಲನೆ, ino, inode,
						ಗಾತ್ರ(ರಚನೆ ext2_inode));
}

/*
 * Functions to read and write a single inode.
 */
errcode_t ext2fs_read_inode2(ext2_filsys fs, ext2_ino_t ino,
			     ರಚನೆ ext2_inode * inode, ಪೂರ್ಣಾಂಕ bufsize,
			     ಪೂರ್ಣಾಂಕ flags)
{
	blk64_t		ಖಂಡ_nr;
	dgrp_t		ಗುಂಪು;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ 	ಖಂಡ, ಸ್ಥಾನಾಂತರ;
	ಅಕ್ಷರ 		*ptr;
	errcode_t	retval;
	ಚಿಹ್ನೆರಹಿತ	i;
	ಪೂರ್ಣಾಂಕ		clen, inodes_ಪ್ರತಿ_ಖಂಡ;
	io_channel	io;
	ಪೂರ್ಣಾಂಕ		ಉದ್ದಳತೆ = EXT2_INODE_ಗಾತ್ರ(fs->super);
	ರಚನೆ ext2_inode_large	*iptr;
	ಪೂರ್ಣಾಂಕ		cache_ಸ್ಥಾನ, fail_csum;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ (ext2fs_has_feature_journal_dev(fs->super))
		ಹಿಂತಿರುಗಿ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	ಆದರೆ (fs->blocksize < 1024)
		ಹಿಂತಿರುಗಿ EXT2_FILSYS_CORRUPTED; /* Should never happen */

	/* Check to see if user has an override function */
	ಆದರೆ (fs->read_inode &&
	    ((bufsize == ಗಾತ್ರ(ರಚನೆ ext2_inode)) ||
	     (EXT2_INODE_ಗಾತ್ರ(fs->super) == ಗಾತ್ರ(ರಚನೆ ext2_inode)))) {
		retval = (fs->read_inode)(fs, ino, inode);
		ಆದರೆ (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			ಹಿಂತಿರುಗಿ retval;
	}
	ಆದರೆ ((ino == 0) || (ino > fs->super->s_inodes_ಎಣಿಕೆ))
		ಹಿಂತಿರುಗಿ EXT2_ET_BAD_INODE_ಅಂಕೆ;
	/* Create inode cache if not present */
	ಆದರೆ (!fs->icache) {
		retval = ext2fs_create_inode_cache(fs, 4);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}
	/* Check to see if it's in the inode cache */
	ಗಾಗಿ (i = 0; i < fs->icache->cache_ಗಾತ್ರ; i++) {
		ಆದರೆ (fs->icache->cache[i].ino == ino) {
			ಸ್ಮರಣೆ_ನಕಲು(inode, fs->icache->cache[i].inode,
			       (bufsize > ಉದ್ದಳತೆ) ? ಉದ್ದಳತೆ : bufsize);
			ಹಿಂತಿರುಗಿ 0;
		}
	}
	ಆದರೆ (fs->flags & EXT2_FLAG_IMAGE_FILE) {
		inodes_ಪ್ರತಿ_ಖಂಡ = fs->blocksize / EXT2_INODE_ಗಾತ್ರ(fs->super);
		ಖಂಡ_nr = ext2fs_le32_ಗಮ್ಯ_cpu(fs->image_ಶೀರ್ಷಿಕೆ->ಸ್ಥಾನಾಂತರ_inode) / fs->blocksize;
		ಖಂಡ_nr += (ino - 1) / inodes_ಪ್ರತಿ_ಖಂಡ;
		ಸ್ಥಾನಾಂತರ = ((ino - 1) % inodes_ಪ್ರತಿ_ಖಂಡ) *
			EXT2_INODE_ಗಾತ್ರ(fs->super);
		io = fs->image_io;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಗುಂಪು = (ino - 1) / EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(fs->super);
		ಆದರೆ (ಗುಂಪು > fs->ಗುಂಪು_desc_ಎಣಿಕೆ)
			ಹಿಂತಿರುಗಿ EXT2_ET_BAD_INODE_ಅಂಕೆ;
		ಸ್ಥಾನಾಂತರ = ((ino - 1) % EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(fs->super)) *
			EXT2_INODE_ಗಾತ್ರ(fs->super);
		ಖಂಡ = ಸ್ಥಾನಾಂತರ >> EXT2_ಖಂಡ_ಗಾತ್ರ_ಬಿಟ್ಟುಗಳು(fs->super);
		ಖಂಡ_nr = ext2fs_inode_ಕೋಷ್ಟಕ_loc(fs, ಗುಂಪು);
		ಆದರೆ (!ಖಂಡ_nr)
			ಹಿಂತಿರುಗಿ EXT2_ET_MISSING_INODE_ಕೋಷ್ಟಕ;
		ಆದರೆ ((ಖಂಡ_nr < fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ) ||
		    (ಖಂಡ_nr + fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು - 1 >=
		     ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super)))
			ಹಿಂತಿರುಗಿ EXT2_ET_GDESC_BAD_INODE_ಕೋಷ್ಟಕ;
		ಖಂಡ_nr += ಖಂಡ;
		io = fs->io;
	}
	ಸ್ಥಾನಾಂತರ &= (EXT2_ಖಂಡ_ಗಾತ್ರ(fs->super) - 1);

	cache_ಸ್ಥಾನ = (fs->icache->cache_ಕೊನೆಯ + 1) % fs->icache->cache_ಗಾತ್ರ;
	iptr = (ರಚನೆ ext2_inode_large *)fs->icache->cache[cache_ಸ್ಥಾನ].inode;

	ptr = (ಅಕ್ಷರ *) iptr;
	ಆಗ (ಉದ್ದಳತೆ) {
		clen = ಉದ್ದಳತೆ;
		ಆದರೆ ((ಸ್ಥಾನಾಂತರ + ಉದ್ದಳತೆ) > fs->blocksize)
			clen = fs->blocksize - ಸ್ಥಾನಾಂತರ;

		ಆದರೆ (ಖಂಡ_nr != fs->icache->ಬಫರ್_blk) {
			retval = io_channel_read_blk64(io, ಖಂಡ_nr, 1,
						     fs->icache->ಬಫರ್);
			ಆದರೆ (retval)
				ಹಿಂತಿರುಗಿ retval;
			fs->icache->ಬಫರ್_blk = ಖಂಡ_nr;
		}

		ಸ್ಮರಣೆ_ನಕಲು(ptr, ((ಅಕ್ಷರ *) fs->icache->ಬಫರ್) + (ಚಿಹ್ನೆರಹಿತ) ಸ್ಥಾನಾಂತರ,
		       clen);

		ಸ್ಥಾನಾಂತರ = 0;
		ಉದ್ದಳತೆ -= clen;
		ptr += clen;
		ಖಂಡ_nr++;
	}
	ಉದ್ದಳತೆ = EXT2_INODE_ಗಾತ್ರ(fs->super);

	/* Verify the inode checksum. */
	fail_csum = !ext2fs_inode_csum_verify(fs, ino, iptr);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	ext2fs_ಅದಲುಬದಲು_inode_ತುಂಬಿದ(fs, (ರಚನೆ ext2_inode_large *) iptr,
			       (ರಚನೆ ext2_inode_large *) iptr,
			       0, ಉದ್ದಳತೆ);
#ಆದರೆ_ಅಂತ್ಯ

	/* Update the inode cache bookkeeping */
	ಆದರೆ (!fail_csum) {
		fs->icache->cache_ಕೊನೆಯ = cache_ಸ್ಥಾನ;
		fs->icache->cache[cache_ಸ್ಥಾನ].ino = ino;
	}
	ಸ್ಮರಣೆ_ನಕಲು(inode, iptr, (bufsize > ಉದ್ದಳತೆ) ? ಉದ್ದಳತೆ : bufsize);

	ಆದರೆ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !(flags & READ_INODE_NOCSUM) && fail_csum)
		ಹಿಂತಿರುಗಿ EXT2_ET_INODE_CSUM_INVALID;

	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_read_inode_ತುಂಬಿದ(ext2_filsys fs, ext2_ino_t ino,
				 ರಚನೆ ext2_inode * inode, ಪೂರ್ಣಾಂಕ bufsize)
{
	ಹಿಂತಿರುಗಿ ext2fs_read_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_read_inode(ext2_filsys fs, ext2_ino_t ino,
			    ರಚನೆ ext2_inode * inode)
{
	ಹಿಂತಿರುಗಿ ext2fs_read_inode2(fs, ino, inode,
				  ಗಾತ್ರ(ರಚನೆ ext2_inode), 0);
}

errcode_t ext2fs_write_inode2(ext2_filsys fs, ext2_ino_t ino,
			      ರಚನೆ ext2_inode * inode, ಪೂರ್ಣಾಂಕ bufsize,
			      ಪೂರ್ಣಾಂಕ flags)
{
	blk64_t ಖಂಡ_nr;
	dgrp_t ಗುಂಪು;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ, ಸ್ಥಾನಾಂತರ;
	errcode_t retval = 0;
	ರಚನೆ ext2_inode_large *w_inode;
	ಅಕ್ಷರ *ptr;
	ಚಿಹ್ನೆರಹಿತ i;
	ಪೂರ್ಣಾಂಕ clen;
	ಪೂರ್ಣಾಂಕ ಉದ್ದಳತೆ = EXT2_INODE_ಗಾತ್ರ(fs->super);

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ (ext2fs_has_feature_journal_dev(fs->super))
		ಹಿಂತಿರುಗಿ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	/* Check to see if user provided an override function */
	ಆದರೆ (fs->write_inode) {
		retval = (fs->write_inode)(fs, ino, inode);
		ಆದರೆ (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			ಹಿಂತಿರುಗಿ retval;
	}

	ಆದರೆ ((ino == 0) || (ino > fs->super->s_inodes_ಎಣಿಕೆ))
		ಹಿಂತಿರುಗಿ EXT2_ET_BAD_INODE_ಅಂಕೆ;

	/* Prepare our shadow buffer for read/modify/byteswap/write */
	retval = ext2fs_get_mem(ಉದ್ದಳತೆ, &w_inode);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಆದರೆ (bufsize < ಉದ್ದಳತೆ) {
		retval = ext2fs_read_inode2(fs, ino,
					    (ರಚನೆ ext2_inode *)w_inode,
					    ಉದ್ದಳತೆ, READ_INODE_NOCSUM);
		ಆದರೆ (retval)
			ಹೋಗು errout;
	}

	/* Check to see if the inode cache needs to be updated */
	ಆದರೆ (fs->icache) {
		ಗಾಗಿ (i=0; i < fs->icache->cache_ಗಾತ್ರ; i++) {
			ಆದರೆ (fs->icache->cache[i].ino == ino) {
				ಸ್ಮರಣೆ_ನಕಲು(fs->icache->cache[i].inode, inode,
				       (bufsize > ಉದ್ದಳತೆ) ? ಉದ್ದಳತೆ : bufsize);
				ನಿಲ್ಲಿಸು;
			}
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		retval = ext2fs_create_inode_cache(fs, 4);
		ಆದರೆ (retval)
			ಹೋಗು errout;
	}
	ಸ್ಮರಣೆ_ನಕಲು(w_inode, inode, (bufsize > ಉದ್ದಳತೆ) ? ಉದ್ದಳತೆ : bufsize);

	ಆದರೆ (!(fs->flags & EXT2_FLAG_RW)) {
		retval = EXT2_ET_RO_FILSYS;
		ಹೋಗು errout;
	}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	ext2fs_ಅದಲುಬದಲು_inode_ತುಂಬಿದ(fs, w_inode, w_inode, 1, ಉದ್ದಳತೆ);
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ ((flags & WRITE_INODE_NOCSUM) == 0) {
		retval = ext2fs_inode_csum_ಗಣ(fs, ino, w_inode);
		ಆದರೆ (retval)
			ಹೋಗು errout;
	}

	ಗುಂಪು = (ino - 1) / EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(fs->super);
	ಸ್ಥಾನಾಂತರ = ((ino - 1) % EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(fs->super)) *
		EXT2_INODE_ಗಾತ್ರ(fs->super);
	ಖಂಡ = ಸ್ಥಾನಾಂತರ >> EXT2_ಖಂಡ_ಗಾತ್ರ_ಬಿಟ್ಟುಗಳು(fs->super);
	ಖಂಡ_nr = ext2fs_inode_ಕೋಷ್ಟಕ_loc(fs, (ಚಿಹ್ನೆರಹಿತ) ಗುಂಪು);
	ಆದರೆ (!ಖಂಡ_nr) {
		retval = EXT2_ET_MISSING_INODE_ಕೋಷ್ಟಕ;
		ಹೋಗು errout;
	}
	ಆದರೆ ((ಖಂಡ_nr < fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ) ||
	    (ಖಂಡ_nr + fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು - 1 >=
	     ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super))) {
		retval = EXT2_ET_GDESC_BAD_INODE_ಕೋಷ್ಟಕ;
		ಹೋಗು errout;
	}
	ಖಂಡ_nr += ಖಂಡ;

	ಸ್ಥಾನಾಂತರ &= (EXT2_ಖಂಡ_ಗಾತ್ರ(fs->super) - 1);

	ptr = (ಅಕ್ಷರ *) w_inode;

	ಆಗ (ಉದ್ದಳತೆ) {
		clen = ಉದ್ದಳತೆ;
		ಆದರೆ ((ಸ್ಥಾನಾಂತರ + ಉದ್ದಳತೆ) > fs->blocksize)
			clen = fs->blocksize - ಸ್ಥಾನಾಂತರ;

		ಆದರೆ (fs->icache->ಬಫರ್_blk != ಖಂಡ_nr) {
			retval = io_channel_read_blk64(fs->io, ಖಂಡ_nr, 1,
						     fs->icache->ಬಫರ್);
			ಆದರೆ (retval)
				ಹೋಗು errout;
			fs->icache->ಬಫರ್_blk = ಖಂಡ_nr;
		}


		ಸ್ಮರಣೆ_ನಕಲು((ಅಕ್ಷರ *) fs->icache->ಬಫರ್ + (ಚಿಹ್ನೆರಹಿತ) ಸ್ಥಾನಾಂತರ,
		       ptr, clen);

		retval = io_channel_write_blk64(fs->io, ಖಂಡ_nr, 1,
					      fs->icache->ಬಫರ್);
		ಆದರೆ (retval)
			ಹೋಗು errout;

		ಸ್ಥಾನಾಂತರ = 0;
		ptr += clen;
		ಉದ್ದಳತೆ -= clen;
		ಖಂಡ_nr++;
	}

	fs->flags |= EXT2_FLAG_ಬದಲಾಯಿತು;
errout:
	ext2fs_free_mem(&w_inode);
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_write_inode_ತುಂಬಿದ(ext2_filsys fs, ext2_ino_t ino,
				  ರಚನೆ ext2_inode * inode, ಪೂರ್ಣಾಂಕ bufsize)
{
	ಹಿಂತಿರುಗಿ ext2fs_write_inode2(fs, ino, inode, bufsize, 0);
}

errcode_t ext2fs_write_inode(ext2_filsys fs, ext2_ino_t ino,
			     ರಚನೆ ext2_inode *inode)
{
	ಹಿಂತಿರುಗಿ ext2fs_write_inode2(fs, ino, inode,
				   ಗಾತ್ರ(ರಚನೆ ext2_inode), 0);
}

/*
 * This function should be called when writing a new inode.  It makes
 * sure that extra part of large inodes is initialized properly.
 */
errcode_t ext2fs_write_new_inode(ext2_filsys fs, ext2_ino_t ino,
				 ರಚನೆ ext2_inode *inode)
{
	ರಚನೆ ext2_inode	*buf;
	ಪೂರ್ಣಾಂಕ			ಗಾತ್ರ_2 = EXT2_INODE_ಗಾತ್ರ(fs->super);
	ರಚನೆ ext2_inode_large	*large_inode;
	errcode_t		retval;
	time_t			t = ext2fs_P_get_ಸಮಯ(fs);

	ಆದರೆ (!ext2fs_inode_xtime_get(inode, i_atime))
		ext2fs_inode_xtime_ಗಣ(inode, i_atime, t);
	ಆದರೆ (!ext2fs_inode_xtime_get(inode, i_ctime))
		ext2fs_inode_xtime_ಗಣ(inode, i_ctime, t);
	ಆದರೆ (!ext2fs_inode_xtime_get(inode, i_mtime))
		ext2fs_inode_xtime_ಗಣ(inode, i_mtime, t);

	ಆದರೆ (ಗಾತ್ರ_2 == ಗಾತ್ರ(ರಚನೆ ext2_inode))
		ಹಿಂತಿರುಗಿ ext2fs_write_inode_ತುಂಬಿದ(fs, ino, inode,
					       ಗಾತ್ರ(ರಚನೆ ext2_inode));

	buf = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ_2);
	ಆದರೆ (!buf)
		ಹಿಂತಿರುಗಿ ENOMEM;

	memset(buf, 0, ಗಾತ್ರ_2);
	*buf = *inode;

	large_inode = (ರಚನೆ ext2_inode_large *) buf;
	large_inode->i_extra_isize = ಗಾತ್ರ(ರಚನೆ ext2_inode_large) -
		EXT2_GOOD_OLD_INODE_ಗಾತ್ರ;
	ಆದರೆ (!large_inode->i_crtime)
		large_inode->i_crtime = t;

	retval = ext2fs_write_inode_ತುಂಬಿದ(fs, ino, buf, ಗಾತ್ರ_2);
	ಬಿಡುಗಡೆ(buf);
	ಹಿಂತಿರುಗಿ retval;
}


errcode_t ext2fs_get_ಖಂಡಗಳು(ext2_filsys fs, ext2_ino_t ino, blk_t *ಖಂಡಗಳು)
{
	ರಚನೆ ext2_inode	inode;
	ಪೂರ್ಣಾಂಕ			i;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ (ino > fs->super->s_inodes_ಎಣಿಕೆ)
		ಹಿಂತಿರುಗಿ EXT2_ET_BAD_INODE_ಅಂಕೆ;

	ಆದರೆ (fs->get_ಖಂಡಗಳು) {
		ಆದರೆ (!(*fs->get_ಖಂಡಗಳು)(fs, ino, ಖಂಡಗಳು))
			ಹಿಂತಿರುಗಿ 0;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ಗಾಗಿ (i=0; i < EXT2_N_ಖಂಡಗಳು; i++)
		ಖಂಡಗಳು[i] = inode.i_ಖಂಡ[i];
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_check_directory(ext2_filsys fs, ext2_ino_t ino)
{
	ರಚನೆ	ext2_inode	inode;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ (ino > fs->super->s_inodes_ಎಣಿಕೆ)
		ಹಿಂತಿರುಗಿ EXT2_ET_BAD_INODE_ಅಂಕೆ;

	ಆದರೆ (fs->check_directory) {
		retval = (fs->check_directory)(fs, ino);
		ಆದರೆ (retval != EXT2_ET_CALLBACK_NOTHANDLED)
			ಹಿಂತಿರುಗಿ retval;
	}
	retval = ext2fs_read_inode(fs, ino, &inode);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ಆದರೆ (!LINUX_S_ISDIR(inode.i_ಬಹುಲಕ))
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_DIRECTORY;
	ಹಿಂತಿರುಗಿ 0;
}

