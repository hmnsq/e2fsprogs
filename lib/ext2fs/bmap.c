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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جيڪڏهن_پورو
#ضمن <errno.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

#إذا defined(__GNUC__) && !defined(NO_INLINE_FUNCS)
#وصف_ڪريو _BMAP_INLINE_	__inline__
#وإلا
#وصف_ڪريو _BMAP_INLINE_
#جيڪڏهن_پورو

خارجي errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_inode *inode,
			     حرف *بلاڪ_buf, صحيح bmap_flags,
			     blk_t بلاڪ, blk_t *phys_blk);

#وصف_ڪريو inode_bmap(inode, nr) ((inode)->i_بلاڪ[(nr)])

ساكن _BMAP_INLINE_ errcode_t بلاڪ_ind_bmap(ext2_filsys fs, صحيح flags,
					      blk_t ind, حرف *بلاڪ_buf,
					      صحيح *بلاڪون_alloc,
					      blk_t nr, blk_t *ret_blk)
{
	errcode_t	retval;
	blk_t		b;

	إذا (!ind) {
		إذا (flags & BMAP_سيٽ)
			أرجع EXT2_ET_سيٽ_BMAP_NO_IND;
		*ret_blk = 0;
		أرجع 0;
	}
	retval = io_channel_read_blk(fs->io, ind, 1, بلاڪ_buf);
	إذا (retval)
		أرجع retval;

	إذا (flags & BMAP_سيٽ) {
		b = *ret_blk;
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
		b = ext2fs_swab32(b);
#جيڪڏهن_پورو
		((blk_t *) بلاڪ_buf)[nr] = b;
		أرجع io_channel_write_blk(fs->io, ind, 1, بلاڪ_buf);
	}

	b = ((blk_t *) بلاڪ_buf)[nr];

#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	b = ext2fs_swab32(b);
#جيڪڏهن_پورو

	إذا (!b && (flags & BMAP_ALLOC)) {
		b = nr ? ext2fs_le32_منزل_cpu(((blk_t *)بلاڪ_buf)[nr - 1]) : ind;
		retval = ext2fs_alloc_بلاڪ(fs, b,
					    بلاڪ_buf + fs->blocksize, &b);
		إذا (retval)
			أرجع retval;

#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
		((blk_t *) بلاڪ_buf)[nr] = ext2fs_swab32(b);
#وإلا
		((blk_t *) بلاڪ_buf)[nr] = b;
#جيڪڏهن_پورو

		retval = io_channel_write_blk(fs->io, ind, 1, بلاڪ_buf);
		إذا (retval)
			أرجع retval;

		(*بلاڪون_alloc)++;
	}

	*ret_blk = b;
	أرجع 0;
}

ساكن _BMAP_INLINE_ errcode_t بلاڪ_dind_bmap(ext2_filsys fs, صحيح flags,
					       blk_t dind, حرف *بلاڪ_buf,
					       صحيح *بلاڪون_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_في_بلاڪ;

	addr_في_بلاڪ = (blk_t) fs->blocksize >> 2;

	retval = بلاڪ_ind_bmap(fs, flags & ~BMAP_سيٽ, dind, بلاڪ_buf,
				بلاڪون_alloc, nr / addr_في_بلاڪ, &b);
	إذا (retval)
		أرجع retval;
	retval = بلاڪ_ind_bmap(fs, flags, b, بلاڪ_buf, بلاڪون_alloc,
				nr % addr_في_بلاڪ, ret_blk);
	أرجع retval;
}

ساكن _BMAP_INLINE_ errcode_t بلاڪ_tind_bmap(ext2_filsys fs, صحيح flags,
					       blk_t tind, حرف *بلاڪ_buf,
					       صحيح *بلاڪون_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_في_بلاڪ;

	addr_في_بلاڪ = (blk_t) fs->blocksize >> 2;

	retval = بلاڪ_dind_bmap(fs, flags & ~BMAP_سيٽ, tind, بلاڪ_buf,
				 بلاڪون_alloc, nr / addr_في_بلاڪ, &b);
	إذا (retval)
		أرجع retval;
	retval = بلاڪ_ind_bmap(fs, flags, b, بلاڪ_buf, بلاڪون_alloc,
				nr % addr_في_بلاڪ, ret_blk);
	أرجع retval;
}

ساكن errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     حرف *بلاڪ_buf, صحيح bmap_flags, blk64_t بلاڪ,
			     صحيح *ret_flags, صحيح *بلاڪون_alloc,
			     blk64_t *phys_blk);

ساكن errcode_t implied_cluster_alloc(ext2_filsys fs, ext2_ino_t ino,
				       بنية ext2_inode *inode,
				       ext2_extent_handle_t handle,
				       blk64_t lblk, blk64_t *phys_blk)
{
	blk64_t	بنياد_بلاڪ, pblock = 0;
	صحيح i;

	إذا (!ext2fs_has_feature_bigalloc(fs->super))
		أرجع 0;

	بنياد_بلاڪ = lblk & ~EXT2_FS_CLUSTER_نقاب(fs);
	/*
	 * Except for the logical block (lblk) that was passed in, search all
	 * blocks in this logical cluster for a mapping to a physical cluster.
	 * If any such map exists, calculate the physical block that maps to
	 * the logical block and return that.
	 *
	 * The old code wouldn't even look if (block % cluster_ratio) == 0;
	 * this is incorrect if we're allocating blocks in reverse order.
	 */
	لكل (i = 0; i < EXT2_FS_CLUSTER_تناسب(fs); i++) {
		إذا (بنياد_بلاڪ + i == lblk)
			استمر;
		extent_bmap(fs, ino, inode, handle, 0, 0,
			    بنياد_بلاڪ + i, 0, 0, &pblock);
		إذا (pblock)
			اكسر;
	}
	إذا (pblock == 0)
		أرجع 0;
	*phys_blk = pblock - i + (lblk - بنياد_بلاڪ);
	أرجع 0;
}

/* Try to map a logical block to an already-allocated physical cluster. */
errcode_t ext2fs_map_cluster_بلاڪ(ext2_filsys fs, ext2_ino_t ino,
				   بنية ext2_inode *inode, blk64_t lblk,
				   blk64_t *pblk)
{
	ext2_extent_handle_t handle;
	errcode_t retval;

	/* Need bigalloc and extents to be enabled */
	*pblk = 0;
	إذا (!ext2fs_has_feature_bigalloc(fs->super) ||
	    !(inode->i_flags & EXT4_EXTENTS_FL))
		أرجع 0;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	إذا (retval)
		اذهب ٻاهريون;

	retval = implied_cluster_alloc(fs, ino, inode, handle, lblk, pblk);
	إذا (retval)
		اذهب out2;

out2:
	ext2fs_extent_free(handle);
ٻاهريون:
	أرجع retval;
}

ساكن errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     حرف *بلاڪ_buf, صحيح bmap_flags, blk64_t بلاڪ,
			     صحيح *ret_flags, صحيح *بلاڪون_alloc,
			     blk64_t *phys_blk)
{
	بنية blk_alloc_ctx	alloc_ctx;
	بنية ext2fs_extent	extent;
	بنانشان صحيح		وٿي;
	errcode_t		retval = 0;
	blk64_t			blk64 = 0;
	صحيح			alloc = 0;
	صحيح			سيٽ_flags;

	سيٽ_flags = bmap_flags & BMAP_UNINIT ? EXT2_EXTENT_سيٽ_BMAP_UNINIT : 0;

	إذا (bmap_flags & BMAP_سيٽ) {
		retval = ext2fs_extent_سيٽ_bmap(handle, بلاڪ,
						*phys_blk, سيٽ_flags);
		أرجع retval;
	}
	retval = ext2fs_extent_goto(handle, بلاڪ);
	إذا (retval) {
		/* If the extent is not found, return phys_blk = 0 */
		إذا (retval == EXT2_ET_EXTENT_NOT_مليو) {
			extent.e_lblk = بلاڪ;
			اذهب got_بلاڪ;
		}
		أرجع retval;
	}
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	إذا (retval)
		أرجع retval;
	وٿي = بلاڪ - extent.e_lblk;
	إذا (بلاڪ >= extent.e_lblk && (وٿي <= extent.e_len)) {
		*phys_blk = extent.e_pblk + وٿي;
		إذا (ret_flags && extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			*ret_flags |= BMAP_RET_UNINIT;
	}
got_بلاڪ:
	إذا ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
		implied_cluster_alloc(fs, ino, inode, handle, بلاڪ, &blk64);
		إذا (blk64)
			اذهب سيٽ_extent;
		retval = extent_bmap(fs, ino, inode, handle, بلاڪ_buf,
				     0, بلاڪ-1, 0, بلاڪون_alloc, &blk64);
		إذا (retval)
			blk64 = ext2fs_ڳوليو_inode_goal(fs, ino, inode, بلاڪ);
		alloc_ctx.ino = ino;
		alloc_ctx.inode = inode;
		alloc_ctx.lblk = extent.e_lblk;
		alloc_ctx.flags = بلاڪ_ALLOC_معلومات;
		retval = ext2fs_alloc_block3(fs, blk64, بلاڪ_buf, &blk64,
					     &alloc_ctx);
		إذا (retval)
			أرجع retval;
		blk64 &= ~EXT2_FS_CLUSTER_نقاب(fs);
		blk64 += EXT2_FS_CLUSTER_نقاب(fs) & بلاڪ;
		alloc++;
	سيٽ_extent:
		retval = ext2fs_extent_سيٽ_bmap(handle, بلاڪ,
						blk64, سيٽ_flags);
		إذا (retval) {
			ext2fs_بلاڪ_alloc_stats2(fs, blk64, -1);
			أرجع retval;
		}
		/* Update inode after setting extent */
		retval = ext2fs_read_inode(fs, ino, inode);
		إذا (retval)
			أرجع retval;
		*بلاڪون_alloc += alloc;
		*phys_blk = blk64;
	}
	أرجع 0;
}

صحيح ext2fs_file_بلاڪ_وٿي_too_big(ext2_filsys fs,
				     بنية ext2_inode *inode,
				     blk64_t وٿي)
{
	blk64_t addr_في_بلاڪ, max_map_بلاڪ;

	/* Kernel seems to cut us off at 4294967294 blocks */
	إذا (وٿي >= (1ULL << 32) - 1)
		أرجع 1;

	إذا (inode->i_flags & EXT4_EXTENTS_FL)
		أرجع 0;

	addr_في_بلاڪ = fs->blocksize >> 2;
	max_map_بلاڪ = addr_في_بلاڪ;
	max_map_بلاڪ += addr_في_بلاڪ * addr_في_بلاڪ;
	max_map_بلاڪ += addr_في_بلاڪ * addr_في_بلاڪ * addr_في_بلاڪ;
	max_map_بلاڪ += 12;

	أرجع وٿي >= max_map_بلاڪ;
}

errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino, بنية ext2_inode *inode,
		       حرف *بلاڪ_buf, صحيح bmap_flags, blk64_t بلاڪ,
		       صحيح *ret_flags, blk64_t *phys_blk)
{
	بنية ext2_inode inode_buf;
	ext2_extent_handle_t handle = 0;
	blk_t addr_في_بلاڪ;
	blk_t	b, blk32;
	blk64_t b64;
	حرف	*buf = 0;
	errcode_t	retval = 0;
	صحيح		بلاڪون_alloc = 0, inode_dirty = 0;
	بنية blk_alloc_ctx alloc_ctx = {
		.ino	= ino,
		.inode	= inode,
		.lblk	= 0,
		.flags	= بلاڪ_ALLOC_معلومات,
	};

	إذا (!(bmap_flags & BMAP_سيٽ))
		*phys_blk = 0;

	إذا (ret_flags)
		*ret_flags = 0;

	/* Read inode structure if necessary */
	إذا (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		إذا (retval)
			أرجع retval;
		inode = &inode_buf;
	}
	addr_في_بلاڪ = (blk_t) fs->blocksize >> 2;

	إذا (ext2fs_file_بلاڪ_وٿي_too_big(fs, inode, بلاڪ))
		أرجع EXT2_ET_FILE_TOO_BIG;

	/*
	 * If an inode has inline data, that means that it doesn't have
	 * any blocks and we shouldn't map any blocks for it.
	 */
	إذا (inode->i_flags & EXT4_INLINE_معلومات_FL)
		أرجع EXT2_ET_INLINE_معلومات_NO_بلاڪ;

	إذا (!بلاڪ_buf) {
		retval = ext2fs_get_array(2, fs->blocksize, &buf);
		إذا (retval)
			أرجع retval;
		بلاڪ_buf = buf;
	}

	إذا (inode->i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		إذا (retval)
			اذهب پورو;
		retval = extent_bmap(fs, ino, inode, handle, بلاڪ_buf,
				     bmap_flags, بلاڪ, ret_flags,
				     &بلاڪون_alloc, phys_blk);
		اذهب پورو;
	}

	إذا (بلاڪ < EXT2_NDIR_بلاڪون) {
		إذا (bmap_flags & BMAP_سيٽ) {
			b = *phys_blk;
			inode_bmap(inode, بلاڪ) = b;
			inode_dirty++;
			اذهب پورو;
		}

		*phys_blk = inode_bmap(inode, بلاڪ);
		b = بلاڪ ? inode_bmap(inode, بلاڪ - 1) :
			    ext2fs_ڳوليو_inode_goal(fs, ino, inode, بلاڪ);

		إذا ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, بلاڪ_buf, &b64,
						     &alloc_ctx);
			b = b64;
			إذا (retval)
				اذهب پورو;
			inode_bmap(inode, بلاڪ) = b;
			بلاڪون_alloc++;
			*phys_blk = b;
		}
		اذهب پورو;
	}

	/* Indirect block */
	بلاڪ -= EXT2_NDIR_بلاڪون;
	blk32 = *phys_blk;
	إذا (بلاڪ < addr_في_بلاڪ) {
		b = inode_bmap(inode, EXT2_IND_بلاڪ);
		إذا (!b) {
			إذا (!(bmap_flags & BMAP_ALLOC)) {
				إذا (bmap_flags & BMAP_سيٽ)
					retval = EXT2_ET_سيٽ_BMAP_NO_IND;
				اذهب پورو;
			}

			b = inode_bmap(inode, EXT2_IND_بلاڪ-1);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, بلاڪ_buf, &b64,
						     &alloc_ctx);
			b = b64;
			إذا (retval)
				اذهب پورو;
			inode_bmap(inode, EXT2_IND_بلاڪ) = b;
			بلاڪون_alloc++;
		}
		retval = بلاڪ_ind_bmap(fs, bmap_flags, b, بلاڪ_buf,
					&بلاڪون_alloc, بلاڪ, &blk32);
		إذا (retval == 0)
			*phys_blk = blk32;
		اذهب پورو;
	}

	/* Doubly indirect block  */
	بلاڪ -= addr_في_بلاڪ;
	إذا (بلاڪ < addr_في_بلاڪ * addr_في_بلاڪ) {
		b = inode_bmap(inode, EXT2_DIND_بلاڪ);
		إذا (!b) {
			إذا (!(bmap_flags & BMAP_ALLOC)) {
				إذا (bmap_flags & BMAP_سيٽ)
					retval = EXT2_ET_سيٽ_BMAP_NO_IND;
				اذهب پورو;
			}

			b = inode_bmap(inode, EXT2_IND_بلاڪ);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, بلاڪ_buf, &b64,
						     &alloc_ctx);
			b = b64;
			إذا (retval)
				اذهب پورو;
			inode_bmap(inode, EXT2_DIND_بلاڪ) = b;
			بلاڪون_alloc++;
		}
		retval = بلاڪ_dind_bmap(fs, bmap_flags, b, بلاڪ_buf,
					 &بلاڪون_alloc, بلاڪ, &blk32);
		إذا (retval == 0)
			*phys_blk = blk32;
		اذهب پورو;
	}

	/* Triply indirect block */
	بلاڪ -= addr_في_بلاڪ * addr_في_بلاڪ;
	b = inode_bmap(inode, EXT2_TIND_بلاڪ);
	إذا (!b) {
		إذا (!(bmap_flags & BMAP_ALLOC)) {
			إذا (bmap_flags & BMAP_سيٽ)
				retval = EXT2_ET_سيٽ_BMAP_NO_IND;
			اذهب پورو;
		}

		b = inode_bmap(inode, EXT2_DIND_بلاڪ);
		b64 = b;
		retval = ext2fs_alloc_block3(fs, b64, بلاڪ_buf, &b64,
					     &alloc_ctx);
		b = b64;
		إذا (retval)
			اذهب پورو;
		inode_bmap(inode, EXT2_TIND_بلاڪ) = b;
		بلاڪون_alloc++;
	}
	retval = بلاڪ_tind_bmap(fs, bmap_flags, b, بلاڪ_buf,
				 &بلاڪون_alloc, بلاڪ, &blk32);
	إذا (retval == 0)
		*phys_blk = blk32;
پورو:
	إذا (*phys_blk && retval == 0 && (bmap_flags & BMAP_ZERO))
		retval = ext2fs_zero_blocks2(fs, *phys_blk, 1, عدم, عدم);
	إذا (buf)
		ext2fs_free_mem(&buf);
	إذا (handle)
		ext2fs_extent_free(handle);
	إذا ((retval == 0) && (بلاڪون_alloc || inode_dirty)) {
		ext2fs_iblk_جوڙيو_بلاڪون(fs, inode, بلاڪون_alloc);
		retval = ext2fs_write_inode(fs, ino, inode);
	}
	أرجع retval;
}

errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino, بنية ext2_inode *inode,
		      حرف *بلاڪ_buf, صحيح bmap_flags, blk_t بلاڪ,
		      blk_t *phys_blk)
{
	errcode_t ret;
	blk64_t	ret_blk = *phys_blk;

	ret = ext2fs_bmap2(fs, ino, inode, بلاڪ_buf, bmap_flags, بلاڪ,
			    0, &ret_blk);
	إذا (ret)
		أرجع ret;
	إذا (ret_blk >= ((ڊگهو ڊگهو) 1 << 32))
		أرجع EOVERFLOW;
	*phys_blk = ret_blk;
	أرجع 0;
}
