/*
 * bmap.c --- logical to physical block mapping
 *
 * Copyright (C) 1997 Theodore Ts'o.
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
#ಅನ್ತರ್ಭಾವ <errno.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

#ಆದರೆ defined(__GNUC__) && !defined(NO_INLINE_FUNCS)
#ವ್ಯಾಖ್ಯಾನಿಸು _BMAP_INLINE_	__inline__
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು _BMAP_INLINE_
#ಆದರೆ_ಅಂತ್ಯ

ಬಾಹ್ಯ errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     ರಚನೆ ext2_inode *inode,
			     ಅಕ್ಷರ *ಖಂಡ_buf, ಪೂರ್ಣಾಂಕ bmap_flags,
			     blk_t ಖಂಡ, blk_t *phys_blk);

#ವ್ಯಾಖ್ಯಾನಿಸು inode_bmap(inode, nr) ((inode)->i_ಖಂಡ[(nr)])

ಸ್ಥಿರ _BMAP_INLINE_ errcode_t ಖಂಡ_ind_bmap(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags,
					      blk_t ind, ಅಕ್ಷರ *ಖಂಡ_buf,
					      ಪೂರ್ಣಾಂಕ *ಖಂಡಗಳು_alloc,
					      blk_t nr, blk_t *ret_blk)
{
	errcode_t	retval;
	blk_t		b;

	ಆದರೆ (!ind) {
		ಆದರೆ (flags & BMAP_ಗಣ)
			ಹಿಂತಿರುಗಿ EXT2_ET_ಗಣ_BMAP_NO_IND;
		*ret_blk = 0;
		ಹಿಂತಿರುಗಿ 0;
	}
	retval = io_channel_read_blk(fs->io, ind, 1, ಖಂಡ_buf);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಆದರೆ (flags & BMAP_ಗಣ) {
		b = *ret_blk;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
		b = ext2fs_swab32(b);
#ಆದರೆ_ಅಂತ್ಯ
		((blk_t *) ಖಂಡ_buf)[nr] = b;
		ಹಿಂತಿರುಗಿ io_channel_write_blk(fs->io, ind, 1, ಖಂಡ_buf);
	}

	b = ((blk_t *) ಖಂಡ_buf)[nr];

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	b = ext2fs_swab32(b);
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (!b && (flags & BMAP_ALLOC)) {
		b = nr ? ext2fs_le32_ಗಮ್ಯ_cpu(((blk_t *)ಖಂಡ_buf)[nr - 1]) : ind;
		retval = ext2fs_alloc_ಖಂಡ(fs, b,
					    ಖಂಡ_buf + fs->blocksize, &b);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
		((blk_t *) ಖಂಡ_buf)[nr] = ext2fs_swab32(b);
#ಇಲ್ಲದಿದ್ದರೆ
		((blk_t *) ಖಂಡ_buf)[nr] = b;
#ಆದರೆ_ಅಂತ್ಯ

		retval = io_channel_write_blk(fs->io, ind, 1, ಖಂಡ_buf);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

		(*ಖಂಡಗಳು_alloc)++;
	}

	*ret_blk = b;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ _BMAP_INLINE_ errcode_t ಖಂಡ_dind_bmap(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags,
					       blk_t dind, ಅಕ್ಷರ *ಖಂಡ_buf,
					       ಪೂರ್ಣಾಂಕ *ಖಂಡಗಳು_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_ಪ್ರತಿ_ಖಂಡ;

	addr_ಪ್ರತಿ_ಖಂಡ = (blk_t) fs->blocksize >> 2;

	retval = ಖಂಡ_ind_bmap(fs, flags & ~BMAP_ಗಣ, dind, ಖಂಡ_buf,
				ಖಂಡಗಳು_alloc, nr / addr_ಪ್ರತಿ_ಖಂಡ, &b);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	retval = ಖಂಡ_ind_bmap(fs, flags, b, ಖಂಡ_buf, ಖಂಡಗಳು_alloc,
				nr % addr_ಪ್ರತಿ_ಖಂಡ, ret_blk);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ _BMAP_INLINE_ errcode_t ಖಂಡ_tind_bmap(ext2_filsys fs, ಪೂರ್ಣಾಂಕ flags,
					       blk_t tind, ಅಕ್ಷರ *ಖಂಡ_buf,
					       ಪೂರ್ಣಾಂಕ *ಖಂಡಗಳು_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_ಪ್ರತಿ_ಖಂಡ;

	addr_ಪ್ರತಿ_ಖಂಡ = (blk_t) fs->blocksize >> 2;

	retval = ಖಂಡ_dind_bmap(fs, flags & ~BMAP_ಗಣ, tind, ಖಂಡ_buf,
				 ಖಂಡಗಳು_alloc, nr / addr_ಪ್ರತಿ_ಖಂಡ, &b);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	retval = ಖಂಡ_ind_bmap(fs, flags, b, ಖಂಡ_buf, ಖಂಡಗಳು_alloc,
				nr % addr_ಪ್ರತಿ_ಖಂಡ, ret_blk);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     ರಚನೆ ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     ಅಕ್ಷರ *ಖಂಡ_buf, ಪೂರ್ಣಾಂಕ bmap_flags, blk64_t ಖಂಡ,
			     ಪೂರ್ಣಾಂಕ *ret_flags, ಪೂರ್ಣಾಂಕ *ಖಂಡಗಳು_alloc,
			     blk64_t *phys_blk);

ಸ್ಥಿರ errcode_t implied_cluster_alloc(ext2_filsys fs, ext2_ino_t ino,
				       ರಚನೆ ext2_inode *inode,
				       ext2_extent_handle_t handle,
				       blk64_t lblk, blk64_t *phys_blk)
{
	blk64_t	ತಳ_ಖಂಡ, pblock = 0;
	ಪೂರ್ಣಾಂಕ i;

	ಆದರೆ (!ext2fs_has_feature_bigalloc(fs->super))
		ಹಿಂತಿರುಗಿ 0;

	ತಳ_ಖಂಡ = lblk & ~EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs);
	/*
	 * Except for the logical block (lblk) that was passed in, search all
	 * blocks in this logical cluster for a mapping to a physical cluster.
	 * If any such map exists, calculate the physical block that maps to
	 * the logical block and return that.
	 *
	 * The old code wouldn't even look if (block % cluster_ratio) == 0;
	 * this is incorrect if we're allocating blocks in reverse order.
	 */
	ಗಾಗಿ (i = 0; i < EXT2_FS_CLUSTER_ಅನುಪಾತ(fs); i++) {
		ಆದರೆ (ತಳ_ಖಂಡ + i == lblk)
			ಮುಂದುವರಿ;
		extent_bmap(fs, ino, inode, handle, 0, 0,
			    ತಳ_ಖಂಡ + i, 0, 0, &pblock);
		ಆದರೆ (pblock)
			ನಿಲ್ಲಿಸು;
	}
	ಆದರೆ (pblock == 0)
		ಹಿಂತಿರುಗಿ 0;
	*phys_blk = pblock - i + (lblk - ತಳ_ಖಂಡ);
	ಹಿಂತಿರುಗಿ 0;
}

/* Try to map a logical block to an already-allocated physical cluster. */
errcode_t ext2fs_map_cluster_ಖಂಡ(ext2_filsys fs, ext2_ino_t ino,
				   ರಚನೆ ext2_inode *inode, blk64_t lblk,
				   blk64_t *pblk)
{
	ext2_extent_handle_t handle;
	errcode_t retval;

	/* Need bigalloc and extents to be enabled */
	*pblk = 0;
	ಆದರೆ (!ext2fs_has_feature_bigalloc(fs->super) ||
	    !(inode->i_flags & EXT4_EXTENTS_FL))
		ಹಿಂತಿರುಗಿ 0;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	ಆದರೆ (retval)
		ಹೋಗು ಹೊರಹೋಗು;

	retval = implied_cluster_alloc(fs, ino, inode, handle, lblk, pblk);
	ಆದರೆ (retval)
		ಹೋಗು out2;

out2:
	ext2fs_extent_free(handle);
ಹೊರಹೋಗು:
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     ರಚನೆ ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     ಅಕ್ಷರ *ಖಂಡ_buf, ಪೂರ್ಣಾಂಕ bmap_flags, blk64_t ಖಂಡ,
			     ಪೂರ್ಣಾಂಕ *ret_flags, ಪೂರ್ಣಾಂಕ *ಖಂಡಗಳು_alloc,
			     blk64_t *phys_blk)
{
	ರಚನೆ blk_alloc_ctx	alloc_ctx;
	ರಚನೆ ext2fs_extent	extent;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		ಸ್ಥಾನಾಂತರ;
	errcode_t		retval = 0;
	blk64_t			blk64 = 0;
	ಪೂರ್ಣಾಂಕ			alloc = 0;
	ಪೂರ್ಣಾಂಕ			ಗಣ_flags;

	ಗಣ_flags = bmap_flags & BMAP_UNINIT ? EXT2_EXTENT_ಗಣ_BMAP_UNINIT : 0;

	ಆದರೆ (bmap_flags & BMAP_ಗಣ) {
		retval = ext2fs_extent_ಗಣ_bmap(handle, ಖಂಡ,
						*phys_blk, ಗಣ_flags);
		ಹಿಂತಿರುಗಿ retval;
	}
	retval = ext2fs_extent_goto(handle, ಖಂಡ);
	ಆದರೆ (retval) {
		/* If the extent is not found, return phys_blk = 0 */
		ಆದರೆ (retval == EXT2_ET_EXTENT_NOT_ಸಿಕ್ಕಿತು) {
			extent.e_lblk = ಖಂಡ;
			ಹೋಗು got_ಖಂಡ;
		}
		ಹಿಂತಿರುಗಿ retval;
	}
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ಸ್ಥಾನಾಂತರ = ಖಂಡ - extent.e_lblk;
	ಆದರೆ (ಖಂಡ >= extent.e_lblk && (ಸ್ಥಾನಾಂತರ <= extent.e_len)) {
		*phys_blk = extent.e_pblk + ಸ್ಥಾನಾಂತರ;
		ಆದರೆ (ret_flags && extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			*ret_flags |= BMAP_RET_UNINIT;
	}
got_ಖಂಡ:
	ಆದರೆ ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
		implied_cluster_alloc(fs, ino, inode, handle, ಖಂಡ, &blk64);
		ಆದರೆ (blk64)
			ಹೋಗು ಗಣ_extent;
		retval = extent_bmap(fs, ino, inode, handle, ಖಂಡ_buf,
				     0, ಖಂಡ-1, 0, ಖಂಡಗಳು_alloc, &blk64);
		ಆದರೆ (retval)
			blk64 = ext2fs_ಕಂಡುಹಿಡಿ_inode_goal(fs, ino, inode, ಖಂಡ);
		alloc_ctx.ino = ino;
		alloc_ctx.inode = inode;
		alloc_ctx.lblk = extent.e_lblk;
		alloc_ctx.flags = ಖಂಡ_ALLOC_ದತ್ತಾಂಶ;
		retval = ext2fs_alloc_block3(fs, blk64, ಖಂಡ_buf, &blk64,
					     &alloc_ctx);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		blk64 &= ~EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs);
		blk64 += EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs) & ಖಂಡ;
		alloc++;
	ಗಣ_extent:
		retval = ext2fs_extent_ಗಣ_bmap(handle, ಖಂಡ,
						blk64, ಗಣ_flags);
		ಆದರೆ (retval) {
			ext2fs_ಖಂಡ_alloc_stats2(fs, blk64, -1);
			ಹಿಂತಿರುಗಿ retval;
		}
		/* Update inode after setting extent */
		retval = ext2fs_read_inode(fs, ino, inode);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		*ಖಂಡಗಳು_alloc += alloc;
		*phys_blk = blk64;
	}
	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ ext2fs_file_ಖಂಡ_ಸ್ಥಾನಾಂತರ_too_big(ext2_filsys fs,
				     ರಚನೆ ext2_inode *inode,
				     blk64_t ಸ್ಥಾನಾಂತರ)
{
	blk64_t addr_ಪ್ರತಿ_ಖಂಡ, max_map_ಖಂಡ;

	/* Kernel seems to cut us off at 4294967294 blocks */
	ಆದರೆ (ಸ್ಥಾನಾಂತರ >= (1ULL << 32) - 1)
		ಹಿಂತಿರುಗಿ 1;

	ಆದರೆ (inode->i_flags & EXT4_EXTENTS_FL)
		ಹಿಂತಿರುಗಿ 0;

	addr_ಪ್ರತಿ_ಖಂಡ = fs->blocksize >> 2;
	max_map_ಖಂಡ = addr_ಪ್ರತಿ_ಖಂಡ;
	max_map_ಖಂಡ += addr_ಪ್ರತಿ_ಖಂಡ * addr_ಪ್ರತಿ_ಖಂಡ;
	max_map_ಖಂಡ += addr_ಪ್ರತಿ_ಖಂಡ * addr_ಪ್ರತಿ_ಖಂಡ * addr_ಪ್ರತಿ_ಖಂಡ;
	max_map_ಖಂಡ += 12;

	ಹಿಂತಿರುಗಿ ಸ್ಥಾನಾಂತರ >= max_map_ಖಂಡ;
}

errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino, ರಚನೆ ext2_inode *inode,
		       ಅಕ್ಷರ *ಖಂಡ_buf, ಪೂರ್ಣಾಂಕ bmap_flags, blk64_t ಖಂಡ,
		       ಪೂರ್ಣಾಂಕ *ret_flags, blk64_t *phys_blk)
{
	ರಚನೆ ext2_inode inode_buf;
	ext2_extent_handle_t handle = 0;
	blk_t addr_ಪ್ರತಿ_ಖಂಡ;
	blk_t	b, blk32;
	blk64_t b64;
	ಅಕ್ಷರ	*buf = 0;
	errcode_t	retval = 0;
	ಪೂರ್ಣಾಂಕ		ಖಂಡಗಳು_alloc = 0, inode_dirty = 0;
	ರಚನೆ blk_alloc_ctx alloc_ctx = {
		.ino	= ino,
		.inode	= inode,
		.lblk	= 0,
		.flags	= ಖಂಡ_ALLOC_ದತ್ತಾಂಶ,
	};

	ಆದರೆ (!(bmap_flags & BMAP_ಗಣ))
		*phys_blk = 0;

	ಆದರೆ (ret_flags)
		*ret_flags = 0;

	/* Read inode structure if necessary */
	ಆದರೆ (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		inode = &inode_buf;
	}
	addr_ಪ್ರತಿ_ಖಂಡ = (blk_t) fs->blocksize >> 2;

	ಆದರೆ (ext2fs_file_ಖಂಡ_ಸ್ಥಾನಾಂತರ_too_big(fs, inode, ಖಂಡ))
		ಹಿಂತಿರುಗಿ EXT2_ET_FILE_TOO_BIG;

	/*
	 * If an inode has inline data, that means that it doesn't have
	 * any blocks and we shouldn't map any blocks for it.
	 */
	ಆದರೆ (inode->i_flags & EXT4_INLINE_ದತ್ತಾಂಶ_FL)
		ಹಿಂತಿರುಗಿ EXT2_ET_INLINE_ದತ್ತಾಂಶ_NO_ಖಂಡ;

	ಆದರೆ (!ಖಂಡ_buf) {
		retval = ext2fs_get_array(2, fs->blocksize, &buf);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		ಖಂಡ_buf = buf;
	}

	ಆದರೆ (inode->i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		ಆದರೆ (retval)
			ಹೋಗು ಮುಗಿದ;
		retval = extent_bmap(fs, ino, inode, handle, ಖಂಡ_buf,
				     bmap_flags, ಖಂಡ, ret_flags,
				     &ಖಂಡಗಳು_alloc, phys_blk);
		ಹೋಗು ಮುಗಿದ;
	}

	ಆದರೆ (ಖಂಡ < EXT2_NDIR_ಖಂಡಗಳು) {
		ಆದರೆ (bmap_flags & BMAP_ಗಣ) {
			b = *phys_blk;
			inode_bmap(inode, ಖಂಡ) = b;
			inode_dirty++;
			ಹೋಗು ಮುಗಿದ;
		}

		*phys_blk = inode_bmap(inode, ಖಂಡ);
		b = ಖಂಡ ? inode_bmap(inode, ಖಂಡ - 1) :
			    ext2fs_ಕಂಡುಹಿಡಿ_inode_goal(fs, ino, inode, ಖಂಡ);

		ಆದರೆ ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, ಖಂಡ_buf, &b64,
						     &alloc_ctx);
			b = b64;
			ಆದರೆ (retval)
				ಹೋಗು ಮುಗಿದ;
			inode_bmap(inode, ಖಂಡ) = b;
			ಖಂಡಗಳು_alloc++;
			*phys_blk = b;
		}
		ಹೋಗು ಮುಗಿದ;
	}

	/* Indirect block */
	ಖಂಡ -= EXT2_NDIR_ಖಂಡಗಳು;
	blk32 = *phys_blk;
	ಆದರೆ (ಖಂಡ < addr_ಪ್ರತಿ_ಖಂಡ) {
		b = inode_bmap(inode, EXT2_IND_ಖಂಡ);
		ಆದರೆ (!b) {
			ಆದರೆ (!(bmap_flags & BMAP_ALLOC)) {
				ಆದರೆ (bmap_flags & BMAP_ಗಣ)
					retval = EXT2_ET_ಗಣ_BMAP_NO_IND;
				ಹೋಗು ಮುಗಿದ;
			}

			b = inode_bmap(inode, EXT2_IND_ಖಂಡ-1);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, ಖಂಡ_buf, &b64,
						     &alloc_ctx);
			b = b64;
			ಆದರೆ (retval)
				ಹೋಗು ಮುಗಿದ;
			inode_bmap(inode, EXT2_IND_ಖಂಡ) = b;
			ಖಂಡಗಳು_alloc++;
		}
		retval = ಖಂಡ_ind_bmap(fs, bmap_flags, b, ಖಂಡ_buf,
					&ಖಂಡಗಳು_alloc, ಖಂಡ, &blk32);
		ಆದರೆ (retval == 0)
			*phys_blk = blk32;
		ಹೋಗು ಮುಗಿದ;
	}

	/* Doubly indirect block  */
	ಖಂಡ -= addr_ಪ್ರತಿ_ಖಂಡ;
	ಆದರೆ (ಖಂಡ < addr_ಪ್ರತಿ_ಖಂಡ * addr_ಪ್ರತಿ_ಖಂಡ) {
		b = inode_bmap(inode, EXT2_DIND_ಖಂಡ);
		ಆದರೆ (!b) {
			ಆದರೆ (!(bmap_flags & BMAP_ALLOC)) {
				ಆದರೆ (bmap_flags & BMAP_ಗಣ)
					retval = EXT2_ET_ಗಣ_BMAP_NO_IND;
				ಹೋಗು ಮುಗಿದ;
			}

			b = inode_bmap(inode, EXT2_IND_ಖಂಡ);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, ಖಂಡ_buf, &b64,
						     &alloc_ctx);
			b = b64;
			ಆದರೆ (retval)
				ಹೋಗು ಮುಗಿದ;
			inode_bmap(inode, EXT2_DIND_ಖಂಡ) = b;
			ಖಂಡಗಳು_alloc++;
		}
		retval = ಖಂಡ_dind_bmap(fs, bmap_flags, b, ಖಂಡ_buf,
					 &ಖಂಡಗಳು_alloc, ಖಂಡ, &blk32);
		ಆದರೆ (retval == 0)
			*phys_blk = blk32;
		ಹೋಗು ಮುಗಿದ;
	}

	/* Triply indirect block */
	ಖಂಡ -= addr_ಪ್ರತಿ_ಖಂಡ * addr_ಪ್ರತಿ_ಖಂಡ;
	b = inode_bmap(inode, EXT2_TIND_ಖಂಡ);
	ಆದರೆ (!b) {
		ಆದರೆ (!(bmap_flags & BMAP_ALLOC)) {
			ಆದರೆ (bmap_flags & BMAP_ಗಣ)
				retval = EXT2_ET_ಗಣ_BMAP_NO_IND;
			ಹೋಗು ಮುಗಿದ;
		}

		b = inode_bmap(inode, EXT2_DIND_ಖಂಡ);
		b64 = b;
		retval = ext2fs_alloc_block3(fs, b64, ಖಂಡ_buf, &b64,
					     &alloc_ctx);
		b = b64;
		ಆದರೆ (retval)
			ಹೋಗು ಮುಗಿದ;
		inode_bmap(inode, EXT2_TIND_ಖಂಡ) = b;
		ಖಂಡಗಳು_alloc++;
	}
	retval = ಖಂಡ_tind_bmap(fs, bmap_flags, b, ಖಂಡ_buf,
				 &ಖಂಡಗಳು_alloc, ಖಂಡ, &blk32);
	ಆದರೆ (retval == 0)
		*phys_blk = blk32;
ಮುಗಿದ:
	ಆದರೆ (*phys_blk && retval == 0 && (bmap_flags & BMAP_ZERO))
		retval = ext2fs_zero_blocks2(fs, *phys_blk, 1, ಖಾಲಿ, ಖಾಲಿ);
	ಆದರೆ (buf)
		ext2fs_free_mem(&buf);
	ಆದರೆ (handle)
		ext2fs_extent_free(handle);
	ಆದರೆ ((retval == 0) && (ಖಂಡಗಳು_alloc || inode_dirty)) {
		ext2fs_iblk_ಕೂಡಿಸು_ಖಂಡಗಳು(fs, inode, ಖಂಡಗಳು_alloc);
		retval = ext2fs_write_inode(fs, ino, inode);
	}
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino, ರಚನೆ ext2_inode *inode,
		      ಅಕ್ಷರ *ಖಂಡ_buf, ಪೂರ್ಣಾಂಕ bmap_flags, blk_t ಖಂಡ,
		      blk_t *phys_blk)
{
	errcode_t ret;
	blk64_t	ret_blk = *phys_blk;

	ret = ext2fs_bmap2(fs, ino, inode, ಖಂಡ_buf, bmap_flags, ಖಂಡ,
			    0, &ret_blk);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ret;
	ಆದರೆ (ret_blk >= ((ದೀರ್ಘ ದೀರ್ಘ) 1 << 32))
		ಹಿಂತಿರುಗಿ EOVERFLOW;
	*phys_blk = ret_blk;
	ಹಿಂತಿರುಗಿ 0;
}
