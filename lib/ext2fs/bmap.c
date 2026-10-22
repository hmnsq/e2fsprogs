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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

#ਜੇ defined(__GNUC__) && !defined(NO_INLINE_FUNCS)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _BMAP_INLINE_	__inline__
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _BMAP_INLINE_
#ਜੇ_ਅੰਤ

ਬਾਹਰੀ errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     ਬਣਤਰ ext2_inode *inode,
			     ਅੱਖਰ *ਖੰਡ_buf, ਪੂਰਨ bmap_flags,
			     blk_t ਖੰਡ, blk_t *phys_blk);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ inode_bmap(inode, nr) ((inode)->i_ਖੰਡ[(nr)])

ਸਥਿਰ _BMAP_INLINE_ errcode_t ਖੰਡ_ind_bmap(ext2_filsys fs, ਪੂਰਨ flags,
					      blk_t ind, ਅੱਖਰ *ਖੰਡ_buf,
					      ਪੂਰਨ *ਖੰਡਾਂ_alloc,
					      blk_t nr, blk_t *ret_blk)
{
	errcode_t	retval;
	blk_t		b;

	ਜੇ (!ind) {
		ਜੇ (flags & BMAP_ਸੈੱਟ)
			ਵਾਪਸ EXT2_ET_ਸੈੱਟ_BMAP_NO_IND;
		*ret_blk = 0;
		ਵਾਪਸ 0;
	}
	retval = io_channel_read_blk(fs->io, ind, 1, ਖੰਡ_buf);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਜੇ (flags & BMAP_ਸੈੱਟ) {
		b = *ret_blk;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
		b = ext2fs_swab32(b);
#ਜੇ_ਅੰਤ
		((blk_t *) ਖੰਡ_buf)[nr] = b;
		ਵਾਪਸ io_channel_write_blk(fs->io, ind, 1, ਖੰਡ_buf);
	}

	b = ((blk_t *) ਖੰਡ_buf)[nr];

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	b = ext2fs_swab32(b);
#ਜੇ_ਅੰਤ

	ਜੇ (!b && (flags & BMAP_ALLOC)) {
		b = nr ? ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(((blk_t *)ਖੰਡ_buf)[nr - 1]) : ind;
		retval = ext2fs_alloc_ਖੰਡ(fs, b,
					    ਖੰਡ_buf + fs->blocksize, &b);
		ਜੇ (retval)
			ਵਾਪਸ retval;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
		((blk_t *) ਖੰਡ_buf)[nr] = ext2fs_swab32(b);
#ਨਹੀਂਤਾਂ
		((blk_t *) ਖੰਡ_buf)[nr] = b;
#ਜੇ_ਅੰਤ

		retval = io_channel_write_blk(fs->io, ind, 1, ਖੰਡ_buf);
		ਜੇ (retval)
			ਵਾਪਸ retval;

		(*ਖੰਡਾਂ_alloc)++;
	}

	*ret_blk = b;
	ਵਾਪਸ 0;
}

ਸਥਿਰ _BMAP_INLINE_ errcode_t ਖੰਡ_dind_bmap(ext2_filsys fs, ਪੂਰਨ flags,
					       blk_t dind, ਅੱਖਰ *ਖੰਡ_buf,
					       ਪੂਰਨ *ਖੰਡਾਂ_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_ਪ੍ਰਤੀ_ਖੰਡ;

	addr_ਪ੍ਰਤੀ_ਖੰਡ = (blk_t) fs->blocksize >> 2;

	retval = ਖੰਡ_ind_bmap(fs, flags & ~BMAP_ਸੈੱਟ, dind, ਖੰਡ_buf,
				ਖੰਡਾਂ_alloc, nr / addr_ਪ੍ਰਤੀ_ਖੰਡ, &b);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	retval = ਖੰਡ_ind_bmap(fs, flags, b, ਖੰਡ_buf, ਖੰਡਾਂ_alloc,
				nr % addr_ਪ੍ਰਤੀ_ਖੰਡ, ret_blk);
	ਵਾਪਸ retval;
}

ਸਥਿਰ _BMAP_INLINE_ errcode_t ਖੰਡ_tind_bmap(ext2_filsys fs, ਪੂਰਨ flags,
					       blk_t tind, ਅੱਖਰ *ਖੰਡ_buf,
					       ਪੂਰਨ *ਖੰਡਾਂ_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_ਪ੍ਰਤੀ_ਖੰਡ;

	addr_ਪ੍ਰਤੀ_ਖੰਡ = (blk_t) fs->blocksize >> 2;

	retval = ਖੰਡ_dind_bmap(fs, flags & ~BMAP_ਸੈੱਟ, tind, ਖੰਡ_buf,
				 ਖੰਡਾਂ_alloc, nr / addr_ਪ੍ਰਤੀ_ਖੰਡ, &b);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	retval = ਖੰਡ_ind_bmap(fs, flags, b, ਖੰਡ_buf, ਖੰਡਾਂ_alloc,
				nr % addr_ਪ੍ਰਤੀ_ਖੰਡ, ret_blk);
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     ਬਣਤਰ ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     ਅੱਖਰ *ਖੰਡ_buf, ਪੂਰਨ bmap_flags, blk64_t ਖੰਡ,
			     ਪੂਰਨ *ret_flags, ਪੂਰਨ *ਖੰਡਾਂ_alloc,
			     blk64_t *phys_blk);

ਸਥਿਰ errcode_t implied_cluster_alloc(ext2_filsys fs, ext2_ino_t ino,
				       ਬਣਤਰ ext2_inode *inode,
				       ext2_extent_handle_t handle,
				       blk64_t lblk, blk64_t *phys_blk)
{
	blk64_t	ਅਧਾਰ_ਖੰਡ, pblock = 0;
	ਪੂਰਨ i;

	ਜੇ (!ext2fs_has_feature_bigalloc(fs->super))
		ਵਾਪਸ 0;

	ਅਧਾਰ_ਖੰਡ = lblk & ~EXT2_FS_CLUSTER_ਨਕਾਬ(fs);
	/*
	 * Except for the logical block (lblk) that was passed in, search all
	 * blocks in this logical cluster for a mapping to a physical cluster.
	 * If any such map exists, calculate the physical block that maps to
	 * the logical block and return that.
	 *
	 * The old code wouldn't even look if (block % cluster_ratio) == 0;
	 * this is incorrect if we're allocating blocks in reverse order.
	 */
	ਲਈ (i = 0; i < EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs); i++) {
		ਜੇ (ਅਧਾਰ_ਖੰਡ + i == lblk)
			ਜਾਰੀ;
		extent_bmap(fs, ino, inode, handle, 0, 0,
			    ਅਧਾਰ_ਖੰਡ + i, 0, 0, &pblock);
		ਜੇ (pblock)
			ਰੋਕੋ;
	}
	ਜੇ (pblock == 0)
		ਵਾਪਸ 0;
	*phys_blk = pblock - i + (lblk - ਅਧਾਰ_ਖੰਡ);
	ਵਾਪਸ 0;
}

/* Try to map a logical block to an already-allocated physical cluster. */
errcode_t ext2fs_map_cluster_ਖੰਡ(ext2_filsys fs, ext2_ino_t ino,
				   ਬਣਤਰ ext2_inode *inode, blk64_t lblk,
				   blk64_t *pblk)
{
	ext2_extent_handle_t handle;
	errcode_t retval;

	/* Need bigalloc and extents to be enabled */
	*pblk = 0;
	ਜੇ (!ext2fs_has_feature_bigalloc(fs->super) ||
	    !(inode->i_flags & EXT4_EXTENTS_FL))
		ਵਾਪਸ 0;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	ਜੇ (retval)
		ਜਾਓ ਬਾਹਰਲਾ;

	retval = implied_cluster_alloc(fs, ino, inode, handle, lblk, pblk);
	ਜੇ (retval)
		ਜਾਓ out2;

out2:
	ext2fs_extent_free(handle);
ਬਾਹਰਲਾ:
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     ਬਣਤਰ ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     ਅੱਖਰ *ਖੰਡ_buf, ਪੂਰਨ bmap_flags, blk64_t ਖੰਡ,
			     ਪੂਰਨ *ret_flags, ਪੂਰਨ *ਖੰਡਾਂ_alloc,
			     blk64_t *phys_blk)
{
	ਬਣਤਰ blk_alloc_ctx	alloc_ctx;
	ਬਣਤਰ ext2fs_extent	extent;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		ਵਿੱਥ;
	errcode_t		retval = 0;
	blk64_t			blk64 = 0;
	ਪੂਰਨ			alloc = 0;
	ਪੂਰਨ			ਸੈੱਟ_flags;

	ਸੈੱਟ_flags = bmap_flags & BMAP_UNINIT ? EXT2_EXTENT_ਸੈੱਟ_BMAP_UNINIT : 0;

	ਜੇ (bmap_flags & BMAP_ਸੈੱਟ) {
		retval = ext2fs_extent_ਸੈੱਟ_bmap(handle, ਖੰਡ,
						*phys_blk, ਸੈੱਟ_flags);
		ਵਾਪਸ retval;
	}
	retval = ext2fs_extent_goto(handle, ਖੰਡ);
	ਜੇ (retval) {
		/* If the extent is not found, return phys_blk = 0 */
		ਜੇ (retval == EXT2_ET_EXTENT_NOT_ਮਿਲਿਆ) {
			extent.e_lblk = ਖੰਡ;
			ਜਾਓ got_ਖੰਡ;
		}
		ਵਾਪਸ retval;
	}
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ਵਿੱਥ = ਖੰਡ - extent.e_lblk;
	ਜੇ (ਖੰਡ >= extent.e_lblk && (ਵਿੱਥ <= extent.e_len)) {
		*phys_blk = extent.e_pblk + ਵਿੱਥ;
		ਜੇ (ret_flags && extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			*ret_flags |= BMAP_RET_UNINIT;
	}
got_ਖੰਡ:
	ਜੇ ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
		implied_cluster_alloc(fs, ino, inode, handle, ਖੰਡ, &blk64);
		ਜੇ (blk64)
			ਜਾਓ ਸੈੱਟ_extent;
		retval = extent_bmap(fs, ino, inode, handle, ਖੰਡ_buf,
				     0, ਖੰਡ-1, 0, ਖੰਡਾਂ_alloc, &blk64);
		ਜੇ (retval)
			blk64 = ext2fs_ਲੱਭੋ_inode_goal(fs, ino, inode, ਖੰਡ);
		alloc_ctx.ino = ino;
		alloc_ctx.inode = inode;
		alloc_ctx.lblk = extent.e_lblk;
		alloc_ctx.flags = ਖੰਡ_ALLOC_ਅੰਕੜੇ;
		retval = ext2fs_alloc_block3(fs, blk64, ਖੰਡ_buf, &blk64,
					     &alloc_ctx);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		blk64 &= ~EXT2_FS_CLUSTER_ਨਕਾਬ(fs);
		blk64 += EXT2_FS_CLUSTER_ਨਕਾਬ(fs) & ਖੰਡ;
		alloc++;
	ਸੈੱਟ_extent:
		retval = ext2fs_extent_ਸੈੱਟ_bmap(handle, ਖੰਡ,
						blk64, ਸੈੱਟ_flags);
		ਜੇ (retval) {
			ext2fs_ਖੰਡ_alloc_stats2(fs, blk64, -1);
			ਵਾਪਸ retval;
		}
		/* Update inode after setting extent */
		retval = ext2fs_read_inode(fs, ino, inode);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		*ਖੰਡਾਂ_alloc += alloc;
		*phys_blk = blk64;
	}
	ਵਾਪਸ 0;
}

ਪੂਰਨ ext2fs_file_ਖੰਡ_ਵਿੱਥ_too_big(ext2_filsys fs,
				     ਬਣਤਰ ext2_inode *inode,
				     blk64_t ਵਿੱਥ)
{
	blk64_t addr_ਪ੍ਰਤੀ_ਖੰਡ, max_map_ਖੰਡ;

	/* Kernel seems to cut us off at 4294967294 blocks */
	ਜੇ (ਵਿੱਥ >= (1ULL << 32) - 1)
		ਵਾਪਸ 1;

	ਜੇ (inode->i_flags & EXT4_EXTENTS_FL)
		ਵਾਪਸ 0;

	addr_ਪ੍ਰਤੀ_ਖੰਡ = fs->blocksize >> 2;
	max_map_ਖੰਡ = addr_ਪ੍ਰਤੀ_ਖੰਡ;
	max_map_ਖੰਡ += addr_ਪ੍ਰਤੀ_ਖੰਡ * addr_ਪ੍ਰਤੀ_ਖੰਡ;
	max_map_ਖੰਡ += addr_ਪ੍ਰਤੀ_ਖੰਡ * addr_ਪ੍ਰਤੀ_ਖੰਡ * addr_ਪ੍ਰਤੀ_ਖੰਡ;
	max_map_ਖੰਡ += 12;

	ਵਾਪਸ ਵਿੱਥ >= max_map_ਖੰਡ;
}

errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino, ਬਣਤਰ ext2_inode *inode,
		       ਅੱਖਰ *ਖੰਡ_buf, ਪੂਰਨ bmap_flags, blk64_t ਖੰਡ,
		       ਪੂਰਨ *ret_flags, blk64_t *phys_blk)
{
	ਬਣਤਰ ext2_inode inode_buf;
	ext2_extent_handle_t handle = 0;
	blk_t addr_ਪ੍ਰਤੀ_ਖੰਡ;
	blk_t	b, blk32;
	blk64_t b64;
	ਅੱਖਰ	*buf = 0;
	errcode_t	retval = 0;
	ਪੂਰਨ		ਖੰਡਾਂ_alloc = 0, inode_dirty = 0;
	ਬਣਤਰ blk_alloc_ctx alloc_ctx = {
		.ino	= ino,
		.inode	= inode,
		.lblk	= 0,
		.flags	= ਖੰਡ_ALLOC_ਅੰਕੜੇ,
	};

	ਜੇ (!(bmap_flags & BMAP_ਸੈੱਟ))
		*phys_blk = 0;

	ਜੇ (ret_flags)
		*ret_flags = 0;

	/* Read inode structure if necessary */
	ਜੇ (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		inode = &inode_buf;
	}
	addr_ਪ੍ਰਤੀ_ਖੰਡ = (blk_t) fs->blocksize >> 2;

	ਜੇ (ext2fs_file_ਖੰਡ_ਵਿੱਥ_too_big(fs, inode, ਖੰਡ))
		ਵਾਪਸ EXT2_ET_FILE_TOO_BIG;

	/*
	 * If an inode has inline data, that means that it doesn't have
	 * any blocks and we shouldn't map any blocks for it.
	 */
	ਜੇ (inode->i_flags & EXT4_INLINE_ਅੰਕੜੇ_FL)
		ਵਾਪਸ EXT2_ET_INLINE_ਅੰਕੜੇ_NO_ਖੰਡ;

	ਜੇ (!ਖੰਡ_buf) {
		retval = ext2fs_get_array(2, fs->blocksize, &buf);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		ਖੰਡ_buf = buf;
	}

	ਜੇ (inode->i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		ਜੇ (retval)
			ਜਾਓ ਪੂਰਾ;
		retval = extent_bmap(fs, ino, inode, handle, ਖੰਡ_buf,
				     bmap_flags, ਖੰਡ, ret_flags,
				     &ਖੰਡਾਂ_alloc, phys_blk);
		ਜਾਓ ਪੂਰਾ;
	}

	ਜੇ (ਖੰਡ < EXT2_NDIR_ਖੰਡਾਂ) {
		ਜੇ (bmap_flags & BMAP_ਸੈੱਟ) {
			b = *phys_blk;
			inode_bmap(inode, ਖੰਡ) = b;
			inode_dirty++;
			ਜਾਓ ਪੂਰਾ;
		}

		*phys_blk = inode_bmap(inode, ਖੰਡ);
		b = ਖੰਡ ? inode_bmap(inode, ਖੰਡ - 1) :
			    ext2fs_ਲੱਭੋ_inode_goal(fs, ino, inode, ਖੰਡ);

		ਜੇ ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, ਖੰਡ_buf, &b64,
						     &alloc_ctx);
			b = b64;
			ਜੇ (retval)
				ਜਾਓ ਪੂਰਾ;
			inode_bmap(inode, ਖੰਡ) = b;
			ਖੰਡਾਂ_alloc++;
			*phys_blk = b;
		}
		ਜਾਓ ਪੂਰਾ;
	}

	/* Indirect block */
	ਖੰਡ -= EXT2_NDIR_ਖੰਡਾਂ;
	blk32 = *phys_blk;
	ਜੇ (ਖੰਡ < addr_ਪ੍ਰਤੀ_ਖੰਡ) {
		b = inode_bmap(inode, EXT2_IND_ਖੰਡ);
		ਜੇ (!b) {
			ਜੇ (!(bmap_flags & BMAP_ALLOC)) {
				ਜੇ (bmap_flags & BMAP_ਸੈੱਟ)
					retval = EXT2_ET_ਸੈੱਟ_BMAP_NO_IND;
				ਜਾਓ ਪੂਰਾ;
			}

			b = inode_bmap(inode, EXT2_IND_ਖੰਡ-1);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, ਖੰਡ_buf, &b64,
						     &alloc_ctx);
			b = b64;
			ਜੇ (retval)
				ਜਾਓ ਪੂਰਾ;
			inode_bmap(inode, EXT2_IND_ਖੰਡ) = b;
			ਖੰਡਾਂ_alloc++;
		}
		retval = ਖੰਡ_ind_bmap(fs, bmap_flags, b, ਖੰਡ_buf,
					&ਖੰਡਾਂ_alloc, ਖੰਡ, &blk32);
		ਜੇ (retval == 0)
			*phys_blk = blk32;
		ਜਾਓ ਪੂਰਾ;
	}

	/* Doubly indirect block  */
	ਖੰਡ -= addr_ਪ੍ਰਤੀ_ਖੰਡ;
	ਜੇ (ਖੰਡ < addr_ਪ੍ਰਤੀ_ਖੰਡ * addr_ਪ੍ਰਤੀ_ਖੰਡ) {
		b = inode_bmap(inode, EXT2_DIND_ਖੰਡ);
		ਜੇ (!b) {
			ਜੇ (!(bmap_flags & BMAP_ALLOC)) {
				ਜੇ (bmap_flags & BMAP_ਸੈੱਟ)
					retval = EXT2_ET_ਸੈੱਟ_BMAP_NO_IND;
				ਜਾਓ ਪੂਰਾ;
			}

			b = inode_bmap(inode, EXT2_IND_ਖੰਡ);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, ਖੰਡ_buf, &b64,
						     &alloc_ctx);
			b = b64;
			ਜੇ (retval)
				ਜਾਓ ਪੂਰਾ;
			inode_bmap(inode, EXT2_DIND_ਖੰਡ) = b;
			ਖੰਡਾਂ_alloc++;
		}
		retval = ਖੰਡ_dind_bmap(fs, bmap_flags, b, ਖੰਡ_buf,
					 &ਖੰਡਾਂ_alloc, ਖੰਡ, &blk32);
		ਜੇ (retval == 0)
			*phys_blk = blk32;
		ਜਾਓ ਪੂਰਾ;
	}

	/* Triply indirect block */
	ਖੰਡ -= addr_ਪ੍ਰਤੀ_ਖੰਡ * addr_ਪ੍ਰਤੀ_ਖੰਡ;
	b = inode_bmap(inode, EXT2_TIND_ਖੰਡ);
	ਜੇ (!b) {
		ਜੇ (!(bmap_flags & BMAP_ALLOC)) {
			ਜੇ (bmap_flags & BMAP_ਸੈੱਟ)
				retval = EXT2_ET_ਸੈੱਟ_BMAP_NO_IND;
			ਜਾਓ ਪੂਰਾ;
		}

		b = inode_bmap(inode, EXT2_DIND_ਖੰਡ);
		b64 = b;
		retval = ext2fs_alloc_block3(fs, b64, ਖੰਡ_buf, &b64,
					     &alloc_ctx);
		b = b64;
		ਜੇ (retval)
			ਜਾਓ ਪੂਰਾ;
		inode_bmap(inode, EXT2_TIND_ਖੰਡ) = b;
		ਖੰਡਾਂ_alloc++;
	}
	retval = ਖੰਡ_tind_bmap(fs, bmap_flags, b, ਖੰਡ_buf,
				 &ਖੰਡਾਂ_alloc, ਖੰਡ, &blk32);
	ਜੇ (retval == 0)
		*phys_blk = blk32;
ਪੂਰਾ:
	ਜੇ (*phys_blk && retval == 0 && (bmap_flags & BMAP_ZERO))
		retval = ext2fs_zero_blocks2(fs, *phys_blk, 1, ਨਿਰਾ, ਨਿਰਾ);
	ਜੇ (buf)
		ext2fs_free_mem(&buf);
	ਜੇ (handle)
		ext2fs_extent_free(handle);
	ਜੇ ((retval == 0) && (ਖੰਡਾਂ_alloc || inode_dirty)) {
		ext2fs_iblk_ਜੋੜੋ_ਖੰਡਾਂ(fs, inode, ਖੰਡਾਂ_alloc);
		retval = ext2fs_write_inode(fs, ino, inode);
	}
	ਵਾਪਸ retval;
}

errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino, ਬਣਤਰ ext2_inode *inode,
		      ਅੱਖਰ *ਖੰਡ_buf, ਪੂਰਨ bmap_flags, blk_t ਖੰਡ,
		      blk_t *phys_blk)
{
	errcode_t ret;
	blk64_t	ret_blk = *phys_blk;

	ret = ext2fs_bmap2(fs, ino, inode, ਖੰਡ_buf, bmap_flags, ਖੰਡ,
			    0, &ret_blk);
	ਜੇ (ret)
		ਵਾਪਸ ret;
	ਜੇ (ret_blk >= ((ਲੰਮਾ ਲੰਮਾ) 1 << 32))
		ਵਾਪਸ EOVERFLOW;
	*phys_blk = ret_blk;
	ਵਾਪਸ 0;
}
