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
#د_که_پای
#ضمن <errno.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

#إذا defined(__GNUC__) && !defined(NO_INLINE_FUNCS)
#تعریف_کړه _BMAP_INLINE_	__inline__
#وإلا
#تعریف_کړه _BMAP_INLINE_
#د_که_پای

خارجي errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_inode *inode,
			     حرف *بلاک_buf, صحيح bmap_flags,
			     blk_t بلاک, blk_t *phys_blk);

#تعریف_کړه inode_bmap(inode, nr) ((inode)->i_بلاک[(nr)])

ساكن _BMAP_INLINE_ errcode_t بلاک_ind_bmap(ext2_filsys fs, صحيح flags,
					      blk_t ind, حرف *بلاک_buf,
					      صحيح *بلاکونه_alloc,
					      blk_t nr, blk_t *ret_blk)
{
	errcode_t	retval;
	blk_t		b;

	إذا (!ind) {
		إذا (flags & BMAP_سېټ)
			أرجع EXT2_ET_سېټ_BMAP_NO_IND;
		*ret_blk = 0;
		أرجع 0;
	}
	retval = io_channel_read_blk(fs->io, ind, 1, بلاک_buf);
	إذا (retval)
		أرجع retval;

	إذا (flags & BMAP_سېټ) {
		b = *ret_blk;
#که_تعریف_شوی ټکي_BIGENDIAN
		b = ext2fs_swab32(b);
#د_که_پای
		((blk_t *) بلاک_buf)[nr] = b;
		أرجع io_channel_write_blk(fs->io, ind, 1, بلاک_buf);
	}

	b = ((blk_t *) بلاک_buf)[nr];

#که_تعریف_شوی ټکي_BIGENDIAN
	b = ext2fs_swab32(b);
#د_که_پای

	إذا (!b && (flags & BMAP_ALLOC)) {
		b = nr ? ext2fs_le32_منزل_cpu(((blk_t *)بلاک_buf)[nr - 1]) : ind;
		retval = ext2fs_alloc_بلاک(fs, b,
					    بلاک_buf + fs->blocksize, &b);
		إذا (retval)
			أرجع retval;

#که_تعریف_شوی ټکي_BIGENDIAN
		((blk_t *) بلاک_buf)[nr] = ext2fs_swab32(b);
#وإلا
		((blk_t *) بلاک_buf)[nr] = b;
#د_که_پای

		retval = io_channel_write_blk(fs->io, ind, 1, بلاک_buf);
		إذا (retval)
			أرجع retval;

		(*بلاکونه_alloc)++;
	}

	*ret_blk = b;
	أرجع 0;
}

ساكن _BMAP_INLINE_ errcode_t بلاک_dind_bmap(ext2_filsys fs, صحيح flags,
					       blk_t dind, حرف *بلاک_buf,
					       صحيح *بلاکونه_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_فی_بلاک;

	addr_فی_بلاک = (blk_t) fs->blocksize >> 2;

	retval = بلاک_ind_bmap(fs, flags & ~BMAP_سېټ, dind, بلاک_buf,
				بلاکونه_alloc, nr / addr_فی_بلاک, &b);
	إذا (retval)
		أرجع retval;
	retval = بلاک_ind_bmap(fs, flags, b, بلاک_buf, بلاکونه_alloc,
				nr % addr_فی_بلاک, ret_blk);
	أرجع retval;
}

ساكن _BMAP_INLINE_ errcode_t بلاک_tind_bmap(ext2_filsys fs, صحيح flags,
					       blk_t tind, حرف *بلاک_buf,
					       صحيح *بلاکونه_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_فی_بلاک;

	addr_فی_بلاک = (blk_t) fs->blocksize >> 2;

	retval = بلاک_dind_bmap(fs, flags & ~BMAP_سېټ, tind, بلاک_buf,
				 بلاکونه_alloc, nr / addr_فی_بلاک, &b);
	إذا (retval)
		أرجع retval;
	retval = بلاک_ind_bmap(fs, flags, b, بلاک_buf, بلاکونه_alloc,
				nr % addr_فی_بلاک, ret_blk);
	أرجع retval;
}

ساكن errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     حرف *بلاک_buf, صحيح bmap_flags, blk64_t بلاک,
			     صحيح *ret_flags, صحيح *بلاکونه_alloc,
			     blk64_t *phys_blk);

ساكن errcode_t implied_cluster_alloc(ext2_filsys fs, ext2_ino_t ino,
				       بنية ext2_inode *inode,
				       ext2_extent_handle_t handle,
				       blk64_t lblk, blk64_t *phys_blk)
{
	blk64_t	بنسټ_بلاک, pblock = 0;
	صحيح i;

	إذا (!ext2fs_has_feature_bigalloc(fs->super))
		أرجع 0;

	بنسټ_بلاک = lblk & ~EXT2_FS_CLUSTER_ماسک(fs);
	/*
	 * Except for the logical block (lblk) that was passed in, search all
	 * blocks in this logical cluster for a mapping to a physical cluster.
	 * If any such map exists, calculate the physical block that maps to
	 * the logical block and return that.
	 *
	 * The old code wouldn't even look if (block % cluster_ratio) == 0;
	 * this is incorrect if we're allocating blocks in reverse order.
	 */
	لكل (i = 0; i < EXT2_FS_CLUSTER_نسبت(fs); i++) {
		إذا (بنسټ_بلاک + i == lblk)
			استمر;
		extent_bmap(fs, ino, inode, handle, 0, 0,
			    بنسټ_بلاک + i, 0, 0, &pblock);
		إذا (pblock)
			اكسر;
	}
	إذا (pblock == 0)
		أرجع 0;
	*phys_blk = pblock - i + (lblk - بنسټ_بلاک);
	أرجع 0;
}

/* Try to map a logical block to an already-allocated physical cluster. */
errcode_t ext2fs_map_cluster_بلاک(ext2_filsys fs, ext2_ino_t ino,
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
		اذهب بهر;

	retval = implied_cluster_alloc(fs, ino, inode, handle, lblk, pblk);
	إذا (retval)
		اذهب out2;

out2:
	ext2fs_extent_free(handle);
بهر:
	أرجع retval;
}

ساكن errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     حرف *بلاک_buf, صحيح bmap_flags, blk64_t بلاک,
			     صحيح *ret_flags, صحيح *بلاکونه_alloc,
			     blk64_t *phys_blk)
{
	بنية blk_alloc_ctx	alloc_ctx;
	بنية ext2fs_extent	extent;
	بینښه صحيح		دننه_واټن;
	errcode_t		retval = 0;
	blk64_t			blk64 = 0;
	صحيح			alloc = 0;
	صحيح			سېټ_flags;

	سېټ_flags = bmap_flags & BMAP_UNINIT ? EXT2_EXTENT_سېټ_BMAP_UNINIT : 0;

	إذا (bmap_flags & BMAP_سېټ) {
		retval = ext2fs_extent_سېټ_bmap(handle, بلاک,
						*phys_blk, سېټ_flags);
		أرجع retval;
	}
	retval = ext2fs_extent_goto(handle, بلاک);
	إذا (retval) {
		/* If the extent is not found, return phys_blk = 0 */
		إذا (retval == EXT2_ET_EXTENT_NOT_وموندل) {
			extent.e_lblk = بلاک;
			اذهب got_بلاک;
		}
		أرجع retval;
	}
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	إذا (retval)
		أرجع retval;
	دننه_واټن = بلاک - extent.e_lblk;
	إذا (بلاک >= extent.e_lblk && (دننه_واټن <= extent.e_len)) {
		*phys_blk = extent.e_pblk + دننه_واټن;
		إذا (ret_flags && extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			*ret_flags |= BMAP_RET_UNINIT;
	}
got_بلاک:
	إذا ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
		implied_cluster_alloc(fs, ino, inode, handle, بلاک, &blk64);
		إذا (blk64)
			اذهب سېټ_extent;
		retval = extent_bmap(fs, ino, inode, handle, بلاک_buf,
				     0, بلاک-1, 0, بلاکونه_alloc, &blk64);
		إذا (retval)
			blk64 = ext2fs_پیدا_کړه_inode_goal(fs, ino, inode, بلاک);
		alloc_ctx.ino = ino;
		alloc_ctx.inode = inode;
		alloc_ctx.lblk = extent.e_lblk;
		alloc_ctx.flags = بلاک_ALLOC_معلومات;
		retval = ext2fs_alloc_block3(fs, blk64, بلاک_buf, &blk64,
					     &alloc_ctx);
		إذا (retval)
			أرجع retval;
		blk64 &= ~EXT2_FS_CLUSTER_ماسک(fs);
		blk64 += EXT2_FS_CLUSTER_ماسک(fs) & بلاک;
		alloc++;
	سېټ_extent:
		retval = ext2fs_extent_سېټ_bmap(handle, بلاک,
						blk64, سېټ_flags);
		إذا (retval) {
			ext2fs_بلاک_alloc_stats2(fs, blk64, -1);
			أرجع retval;
		}
		/* Update inode after setting extent */
		retval = ext2fs_read_inode(fs, ino, inode);
		إذا (retval)
			أرجع retval;
		*بلاکونه_alloc += alloc;
		*phys_blk = blk64;
	}
	أرجع 0;
}

صحيح ext2fs_file_بلاک_دننه_واټن_too_big(ext2_filsys fs,
				     بنية ext2_inode *inode,
				     blk64_t دننه_واټن)
{
	blk64_t addr_فی_بلاک, max_map_بلاک;

	/* Kernel seems to cut us off at 4294967294 blocks */
	إذا (دننه_واټن >= (1ULL << 32) - 1)
		أرجع 1;

	إذا (inode->i_flags & EXT4_EXTENTS_FL)
		أرجع 0;

	addr_فی_بلاک = fs->blocksize >> 2;
	max_map_بلاک = addr_فی_بلاک;
	max_map_بلاک += addr_فی_بلاک * addr_فی_بلاک;
	max_map_بلاک += addr_فی_بلاک * addr_فی_بلاک * addr_فی_بلاک;
	max_map_بلاک += 12;

	أرجع دننه_واټن >= max_map_بلاک;
}

errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino, بنية ext2_inode *inode,
		       حرف *بلاک_buf, صحيح bmap_flags, blk64_t بلاک,
		       صحيح *ret_flags, blk64_t *phys_blk)
{
	بنية ext2_inode inode_buf;
	ext2_extent_handle_t handle = 0;
	blk_t addr_فی_بلاک;
	blk_t	b, blk32;
	blk64_t b64;
	حرف	*buf = 0;
	errcode_t	retval = 0;
	صحيح		بلاکونه_alloc = 0, inode_dirty = 0;
	بنية blk_alloc_ctx alloc_ctx = {
		.ino	= ino,
		.inode	= inode,
		.lblk	= 0,
		.flags	= بلاک_ALLOC_معلومات,
	};

	إذا (!(bmap_flags & BMAP_سېټ))
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
	addr_فی_بلاک = (blk_t) fs->blocksize >> 2;

	إذا (ext2fs_file_بلاک_دننه_واټن_too_big(fs, inode, بلاک))
		أرجع EXT2_ET_FILE_TOO_BIG;

	/*
	 * If an inode has inline data, that means that it doesn't have
	 * any blocks and we shouldn't map any blocks for it.
	 */
	إذا (inode->i_flags & EXT4_INLINE_معلومات_FL)
		أرجع EXT2_ET_INLINE_معلومات_NO_بلاک;

	إذا (!بلاک_buf) {
		retval = ext2fs_get_array(2, fs->blocksize, &buf);
		إذا (retval)
			أرجع retval;
		بلاک_buf = buf;
	}

	إذا (inode->i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		إذا (retval)
			اذهب بشپړ;
		retval = extent_bmap(fs, ino, inode, handle, بلاک_buf,
				     bmap_flags, بلاک, ret_flags,
				     &بلاکونه_alloc, phys_blk);
		اذهب بشپړ;
	}

	إذا (بلاک < EXT2_NDIR_بلاکونه) {
		إذا (bmap_flags & BMAP_سېټ) {
			b = *phys_blk;
			inode_bmap(inode, بلاک) = b;
			inode_dirty++;
			اذهب بشپړ;
		}

		*phys_blk = inode_bmap(inode, بلاک);
		b = بلاک ? inode_bmap(inode, بلاک - 1) :
			    ext2fs_پیدا_کړه_inode_goal(fs, ino, inode, بلاک);

		إذا ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, بلاک_buf, &b64,
						     &alloc_ctx);
			b = b64;
			إذا (retval)
				اذهب بشپړ;
			inode_bmap(inode, بلاک) = b;
			بلاکونه_alloc++;
			*phys_blk = b;
		}
		اذهب بشپړ;
	}

	/* Indirect block */
	بلاک -= EXT2_NDIR_بلاکونه;
	blk32 = *phys_blk;
	إذا (بلاک < addr_فی_بلاک) {
		b = inode_bmap(inode, EXT2_IND_بلاک);
		إذا (!b) {
			إذا (!(bmap_flags & BMAP_ALLOC)) {
				إذا (bmap_flags & BMAP_سېټ)
					retval = EXT2_ET_سېټ_BMAP_NO_IND;
				اذهب بشپړ;
			}

			b = inode_bmap(inode, EXT2_IND_بلاک-1);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, بلاک_buf, &b64,
						     &alloc_ctx);
			b = b64;
			إذا (retval)
				اذهب بشپړ;
			inode_bmap(inode, EXT2_IND_بلاک) = b;
			بلاکونه_alloc++;
		}
		retval = بلاک_ind_bmap(fs, bmap_flags, b, بلاک_buf,
					&بلاکونه_alloc, بلاک, &blk32);
		إذا (retval == 0)
			*phys_blk = blk32;
		اذهب بشپړ;
	}

	/* Doubly indirect block  */
	بلاک -= addr_فی_بلاک;
	إذا (بلاک < addr_فی_بلاک * addr_فی_بلاک) {
		b = inode_bmap(inode, EXT2_DIND_بلاک);
		إذا (!b) {
			إذا (!(bmap_flags & BMAP_ALLOC)) {
				إذا (bmap_flags & BMAP_سېټ)
					retval = EXT2_ET_سېټ_BMAP_NO_IND;
				اذهب بشپړ;
			}

			b = inode_bmap(inode, EXT2_IND_بلاک);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, بلاک_buf, &b64,
						     &alloc_ctx);
			b = b64;
			إذا (retval)
				اذهب بشپړ;
			inode_bmap(inode, EXT2_DIND_بلاک) = b;
			بلاکونه_alloc++;
		}
		retval = بلاک_dind_bmap(fs, bmap_flags, b, بلاک_buf,
					 &بلاکونه_alloc, بلاک, &blk32);
		إذا (retval == 0)
			*phys_blk = blk32;
		اذهب بشپړ;
	}

	/* Triply indirect block */
	بلاک -= addr_فی_بلاک * addr_فی_بلاک;
	b = inode_bmap(inode, EXT2_TIND_بلاک);
	إذا (!b) {
		إذا (!(bmap_flags & BMAP_ALLOC)) {
			إذا (bmap_flags & BMAP_سېټ)
				retval = EXT2_ET_سېټ_BMAP_NO_IND;
			اذهب بشپړ;
		}

		b = inode_bmap(inode, EXT2_DIND_بلاک);
		b64 = b;
		retval = ext2fs_alloc_block3(fs, b64, بلاک_buf, &b64,
					     &alloc_ctx);
		b = b64;
		إذا (retval)
			اذهب بشپړ;
		inode_bmap(inode, EXT2_TIND_بلاک) = b;
		بلاکونه_alloc++;
	}
	retval = بلاک_tind_bmap(fs, bmap_flags, b, بلاک_buf,
				 &بلاکونه_alloc, بلاک, &blk32);
	إذا (retval == 0)
		*phys_blk = blk32;
بشپړ:
	إذا (*phys_blk && retval == 0 && (bmap_flags & BMAP_ZERO))
		retval = ext2fs_zero_blocks2(fs, *phys_blk, 1, عدم, عدم);
	إذا (buf)
		ext2fs_free_mem(&buf);
	إذا (handle)
		ext2fs_extent_free(handle);
	إذا ((retval == 0) && (بلاکونه_alloc || inode_dirty)) {
		ext2fs_iblk_جمع_کړه_بلاکونه(fs, inode, بلاکونه_alloc);
		retval = ext2fs_write_inode(fs, ino, inode);
	}
	أرجع retval;
}

errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino, بنية ext2_inode *inode,
		      حرف *بلاک_buf, صحيح bmap_flags, blk_t بلاک,
		      blk_t *phys_blk)
{
	errcode_t ret;
	blk64_t	ret_blk = *phys_blk;

	ret = ext2fs_bmap2(fs, ino, inode, بلاک_buf, bmap_flags, بلاک,
			    0, &ret_blk);
	إذا (ret)
		أرجع ret;
	إذا (ret_blk >= ((اوږد اوږد) 1 << 32))
		أرجع EOVERFLOW;
	*phys_blk = ret_blk;
	أرجع 0;
}
