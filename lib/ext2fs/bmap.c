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

#include "config.h"
#include <stdio.h>
#include <string.h>
#གལ་སྲིད HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <errno.h>

#include "ext2_fs.h"
#include "ext2fsP.h"

#གལ་སྲིད defined(__GNUC__) && !defined(NO_INLINE_FUNCS)
#define _BMAP_INLINE_	__inline__
#དེ་མིན
#define _BMAP_INLINE_
#endif

extern errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     sa_nracanaa ext2_inode *inode,
			     var_na *block_buf, ཧྲིལ bmap_flags,
			     blk_t block, blk_t *phys_blk);

#define inode_bmap(inode, nr) ((inode)->i_block[(nr)])

static _BMAP_INLINE_ errcode_t block_ind_bmap(ext2_filsys fs, ཧྲིལ flags,
					      blk_t ind, var_na *block_buf,
					      ཧྲིལ *blocks_alloc,
					      blk_t nr, blk_t *ret_blk)
{
	errcode_t	retval;
	blk_t		b;

	གལ་སྲིད (!ind) {
		གལ་སྲིད (flags & BMAP_SET)
			ལོག EXT2_ET_SET_BMAP_NO_IND;
		*ret_blk = 0;
		ལོག 0;
	}
	retval = io_channel_read_blk(fs->io, ind, 1, block_buf);
	གལ་སྲིད (retval)
		ལོག retval;

	གལ་སྲིད (flags & BMAP_SET) {
		b = *ret_blk;
#ifdef WORDS_BIGENDIAN
		b = ext2fs_swab32(b);
#endif
		((blk_t *) block_buf)[nr] = b;
		ལོག io_channel_write_blk(fs->io, ind, 1, block_buf);
	}

	b = ((blk_t *) block_buf)[nr];

#ifdef WORDS_BIGENDIAN
	b = ext2fs_swab32(b);
#endif

	གལ་སྲིད (!b && (flags & BMAP_ALLOC)) {
		b = nr ? ext2fs_le32_to_cpu(((blk_t *)block_buf)[nr - 1]) : ind;
		retval = ext2fs_alloc_block(fs, b,
					    block_buf + fs->blocksize, &b);
		གལ་སྲིད (retval)
			ལོག retval;

#ifdef WORDS_BIGENDIAN
		((blk_t *) block_buf)[nr] = ext2fs_swab32(b);
#དེ་མིན
		((blk_t *) block_buf)[nr] = b;
#endif

		retval = io_channel_write_blk(fs->io, ind, 1, block_buf);
		གལ་སྲིད (retval)
			ལོག retval;

		(*blocks_alloc)++;
	}

	*ret_blk = b;
	ལོག 0;
}

static _BMAP_INLINE_ errcode_t block_dind_bmap(ext2_filsys fs, ཧྲིལ flags,
					       blk_t dind, var_na *block_buf,
					       ཧྲིལ *blocks_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_per_block;

	addr_per_block = (blk_t) fs->blocksize >> 2;

	retval = block_ind_bmap(fs, flags & ~BMAP_SET, dind, block_buf,
				blocks_alloc, nr / addr_per_block, &b);
	གལ་སྲིད (retval)
		ལོག retval;
	retval = block_ind_bmap(fs, flags, b, block_buf, blocks_alloc,
				nr % addr_per_block, ret_blk);
	ལོག retval;
}

static _BMAP_INLINE_ errcode_t block_tind_bmap(ext2_filsys fs, ཧྲིལ flags,
					       blk_t tind, var_na *block_buf,
					       ཧྲིལ *blocks_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_per_block;

	addr_per_block = (blk_t) fs->blocksize >> 2;

	retval = block_dind_bmap(fs, flags & ~BMAP_SET, tind, block_buf,
				 blocks_alloc, nr / addr_per_block, &b);
	གལ་སྲིད (retval)
		ལོག retval;
	retval = block_ind_bmap(fs, flags, b, block_buf, blocks_alloc,
				nr % addr_per_block, ret_blk);
	ལོག retval;
}

static errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     sa_nracanaa ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     var_na *block_buf, ཧྲིལ bmap_flags, blk64_t block,
			     ཧྲིལ *ret_flags, ཧྲིལ *blocks_alloc,
			     blk64_t *phys_blk);

static errcode_t implied_cluster_alloc(ext2_filsys fs, ext2_ino_t ino,
				       sa_nracanaa ext2_inode *inode,
				       ext2_extent_handle_t handle,
				       blk64_t lblk, blk64_t *phys_blk)
{
	blk64_t	base_block, pblock = 0;
	ཧྲིལ i;

	གལ་སྲིད (!ext2fs_has_feature_bigalloc(fs->super))
		ལོག 0;

	base_block = lblk & ~EXT2FS_CLUSTER_MASK(fs);
	/*
	 * Except for the logical block (lblk) that was passed in, search all
	 * blocks in this logical cluster for a mapping to a physical cluster.
	 * If any such map exists, calculate the physical block that maps to
	 * the logical block and return that.
	 *
	 * The old code wouldn't even look if (block % cluster_ratio) == 0;
	 * this is incorrect if we're allocating blocks in reverse order.
	 */
	དོན་ལུ (i = 0; i < EXT2FS_CLUSTER_RATIO(fs); i++) {
		གལ་སྲིད (base_block + i == lblk)
			continue;
		extent_bmap(fs, ino, inode, handle, 0, 0,
			    base_block + i, 0, 0, &pblock);
		གལ་སྲིད (pblock)
			bha_nga;
	}
	གལ་སྲིད (pblock == 0)
		ལོག 0;
	*phys_blk = pblock - i + (lblk - base_block);
	ལོག 0;
}

/* Try to map a logical block to an already-allocated physical cluster. */
errcode_t ext2fs_map_cluster_block(ext2_filsys fs, ext2_ino_t ino,
				   sa_nracanaa ext2_inode *inode, blk64_t lblk,
				   blk64_t *pblk)
{
	ext2_extent_handle_t handle;
	errcode_t retval;

	/* Need bigalloc and extents to be enabled */
	*pblk = 0;
	གལ་སྲིད (!ext2fs_has_feature_bigalloc(fs->super) ||
	    !(inode->i_flags & EXT4_EXTENTS_FL))
		ལོག 0;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	གལ་སྲིད (retval)
		goto out;

	retval = implied_cluster_alloc(fs, ino, inode, handle, lblk, pblk);
	གལ་སྲིད (retval)
		goto out2;

out2:
	ext2fs_extent_free(handle);
out:
	ལོག retval;
}

static errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     sa_nracanaa ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     var_na *block_buf, ཧྲིལ bmap_flags, blk64_t block,
			     ཧྲིལ *ret_flags, ཧྲིལ *blocks_alloc,
			     blk64_t *phys_blk)
{
	sa_nracanaa blk_alloc_ctx	alloc_ctx;
	sa_nracanaa ext2fs_extent	extent;
	unsigned ཧྲིལ		offset;
	errcode_t		retval = 0;
	blk64_t			blk64 = 0;
	ཧྲིལ			alloc = 0;
	ཧྲིལ			set_flags;

	set_flags = bmap_flags & BMAP_UNINIT ? EXT2_EXTENT_SET_BMAP_UNINIT : 0;

	གལ་སྲིད (bmap_flags & BMAP_SET) {
		retval = ext2fs_extent_set_bmap(handle, block,
						*phys_blk, set_flags);
		ལོག retval;
	}
	retval = ext2fs_extent_goto(handle, block);
	གལ་སྲིད (retval) {
		/* If the extent is not found, return phys_blk = 0 */
		གལ་སྲིད (retval == EXT2_ET_EXTENT_NOT_FOUND) {
			extent.e_lblk = block;
			goto got_block;
		}
		ལོག retval;
	}
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	གལ་སྲིད (retval)
		ལོག retval;
	offset = block - extent.e_lblk;
	གལ་སྲིད (block >= extent.e_lblk && (offset <= extent.e_len)) {
		*phys_blk = extent.e_pblk + offset;
		གལ་སྲིད (ret_flags && extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			*ret_flags |= BMAP_RET_UNINIT;
	}
got_block:
	གལ་སྲིད ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
		implied_cluster_alloc(fs, ino, inode, handle, block, &blk64);
		གལ་སྲིད (blk64)
			goto set_extent;
		retval = extent_bmap(fs, ino, inode, handle, block_buf,
				     0, block-1, 0, blocks_alloc, &blk64);
		གལ་སྲིད (retval)
			blk64 = ext2fs_find_inode_goal(fs, ino, inode, block);
		alloc_ctx.ino = ino;
		alloc_ctx.inode = inode;
		alloc_ctx.lblk = extent.e_lblk;
		alloc_ctx.flags = BLOCK_ALLOC_DATA;
		retval = ext2fs_alloc_block3(fs, blk64, block_buf, &blk64,
					     &alloc_ctx);
		གལ་སྲིད (retval)
			ལོག retval;
		blk64 &= ~EXT2FS_CLUSTER_MASK(fs);
		blk64 += EXT2FS_CLUSTER_MASK(fs) & block;
		alloc++;
	set_extent:
		retval = ext2fs_extent_set_bmap(handle, block,
						blk64, set_flags);
		གལ་སྲིད (retval) {
			ext2fs_block_alloc_stats2(fs, blk64, -1);
			ལོག retval;
		}
		/* Update inode after setting extent */
		retval = ext2fs_read_inode(fs, ino, inode);
		གལ་སྲིད (retval)
			ལོག retval;
		*blocks_alloc += alloc;
		*phys_blk = blk64;
	}
	ལོག 0;
}

ཧྲིལ ext2fs_file_block_offset_too_big(ext2_filsys fs,
				     sa_nracanaa ext2_inode *inode,
				     blk64_t offset)
{
	blk64_t addr_per_block, max_map_block;

	/* Kernel seems to cut us off at 4294967294 blocks */
	གལ་སྲིད (offset >= (1ULL << 32) - 1)
		ལོག 1;

	གལ་སྲིད (inode->i_flags & EXT4_EXTENTS_FL)
		ལོག 0;

	addr_per_block = fs->blocksize >> 2;
	max_map_block = addr_per_block;
	max_map_block += addr_per_block * addr_per_block;
	max_map_block += addr_per_block * addr_per_block * addr_per_block;
	max_map_block += 12;

	ལོག offset >= max_map_block;
}

errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino, sa_nracanaa ext2_inode *inode,
		       var_na *block_buf, ཧྲིལ bmap_flags, blk64_t block,
		       ཧྲིལ *ret_flags, blk64_t *phys_blk)
{
	sa_nracanaa ext2_inode inode_buf;
	ext2_extent_handle_t handle = 0;
	blk_t addr_per_block;
	blk_t	b, blk32;
	blk64_t b64;
	var_na	*buf = 0;
	errcode_t	retval = 0;
	ཧྲིལ		blocks_alloc = 0, inode_dirty = 0;
	sa_nracanaa blk_alloc_ctx alloc_ctx = {
		.ino	= ino,
		.inode	= inode,
		.lblk	= 0,
		.flags	= BLOCK_ALLOC_DATA,
	};

	གལ་སྲིད (!(bmap_flags & BMAP_SET))
		*phys_blk = 0;

	གལ་སྲིད (ret_flags)
		*ret_flags = 0;

	/* Read inode structure if necessary */
	གལ་སྲིད (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		གལ་སྲིད (retval)
			ལོག retval;
		inode = &inode_buf;
	}
	addr_per_block = (blk_t) fs->blocksize >> 2;

	གལ་སྲིད (ext2fs_file_block_offset_too_big(fs, inode, block))
		ལོག EXT2_ET_FILE_TOO_BIG;

	/*
	 * If an inode has inline data, that means that it doesn't have
	 * any blocks and we shouldn't map any blocks for it.
	 */
	གལ་སྲིད (inode->i_flags & EXT4_INLINE_DATA_FL)
		ལོག EXT2_ET_INLINE_DATA_NO_BLOCK;

	གལ་སྲིད (!block_buf) {
		retval = ext2fs_get_array(2, fs->blocksize, &buf);
		གལ་སྲིད (retval)
			ལོག retval;
		block_buf = buf;
	}

	གལ་སྲིད (inode->i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		གལ་སྲིད (retval)
			goto done;
		retval = extent_bmap(fs, ino, inode, handle, block_buf,
				     bmap_flags, block, ret_flags,
				     &blocks_alloc, phys_blk);
		goto done;
	}

	གལ་སྲིད (block < EXT2_NDIR_BLOCKS) {
		གལ་སྲིད (bmap_flags & BMAP_SET) {
			b = *phys_blk;
			inode_bmap(inode, block) = b;
			inode_dirty++;
			goto done;
		}

		*phys_blk = inode_bmap(inode, block);
		b = block ? inode_bmap(inode, block - 1) :
			    ext2fs_find_inode_goal(fs, ino, inode, block);

		གལ་སྲིད ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
						     &alloc_ctx);
			b = b64;
			གལ་སྲིད (retval)
				goto done;
			inode_bmap(inode, block) = b;
			blocks_alloc++;
			*phys_blk = b;
		}
		goto done;
	}

	/* Indirect block */
	block -= EXT2_NDIR_BLOCKS;
	blk32 = *phys_blk;
	གལ་སྲིད (block < addr_per_block) {
		b = inode_bmap(inode, EXT2_IND_BLOCK);
		གལ་སྲིད (!b) {
			གལ་སྲིད (!(bmap_flags & BMAP_ALLOC)) {
				གལ་སྲིད (bmap_flags & BMAP_SET)
					retval = EXT2_ET_SET_BMAP_NO_IND;
				goto done;
			}

			b = inode_bmap(inode, EXT2_IND_BLOCK-1);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
						     &alloc_ctx);
			b = b64;
			གལ་སྲིད (retval)
				goto done;
			inode_bmap(inode, EXT2_IND_BLOCK) = b;
			blocks_alloc++;
		}
		retval = block_ind_bmap(fs, bmap_flags, b, block_buf,
					&blocks_alloc, block, &blk32);
		གལ་སྲིད (retval == 0)
			*phys_blk = blk32;
		goto done;
	}

	/* Doubly indirect block  */
	block -= addr_per_block;
	གལ་སྲིད (block < addr_per_block * addr_per_block) {
		b = inode_bmap(inode, EXT2_DIND_BLOCK);
		གལ་སྲིད (!b) {
			གལ་སྲིད (!(bmap_flags & BMAP_ALLOC)) {
				གལ་སྲིད (bmap_flags & BMAP_SET)
					retval = EXT2_ET_SET_BMAP_NO_IND;
				goto done;
			}

			b = inode_bmap(inode, EXT2_IND_BLOCK);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
						     &alloc_ctx);
			b = b64;
			གལ་སྲིད (retval)
				goto done;
			inode_bmap(inode, EXT2_DIND_BLOCK) = b;
			blocks_alloc++;
		}
		retval = block_dind_bmap(fs, bmap_flags, b, block_buf,
					 &blocks_alloc, block, &blk32);
		གལ་སྲིད (retval == 0)
			*phys_blk = blk32;
		goto done;
	}

	/* Triply indirect block */
	block -= addr_per_block * addr_per_block;
	b = inode_bmap(inode, EXT2_TIND_BLOCK);
	གལ་སྲིད (!b) {
		གལ་སྲིད (!(bmap_flags & BMAP_ALLOC)) {
			གལ་སྲིད (bmap_flags & BMAP_SET)
				retval = EXT2_ET_SET_BMAP_NO_IND;
			goto done;
		}

		b = inode_bmap(inode, EXT2_DIND_BLOCK);
		b64 = b;
		retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
					     &alloc_ctx);
		b = b64;
		གལ་སྲིད (retval)
			goto done;
		inode_bmap(inode, EXT2_TIND_BLOCK) = b;
		blocks_alloc++;
	}
	retval = block_tind_bmap(fs, bmap_flags, b, block_buf,
				 &blocks_alloc, block, &blk32);
	གལ་སྲིད (retval == 0)
		*phys_blk = blk32;
done:
	གལ་སྲིད (*phys_blk && retval == 0 && (bmap_flags & BMAP_ZERO))
		retval = ext2fs_zero_blocks2(fs, *phys_blk, 1, NULL, NULL);
	གལ་སྲིད (buf)
		ext2fs_free_mem(&buf);
	གལ་སྲིད (handle)
		ext2fs_extent_free(handle);
	གལ་སྲིད ((retval == 0) && (blocks_alloc || inode_dirty)) {
		ext2fs_iblk_add_blocks(fs, inode, blocks_alloc);
		retval = ext2fs_write_inode(fs, ino, inode);
	}
	ལོག retval;
}

errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino, sa_nracanaa ext2_inode *inode,
		      var_na *block_buf, ཧྲིལ bmap_flags, blk_t block,
		      blk_t *phys_blk)
{
	errcode_t ret;
	blk64_t	ret_blk = *phys_blk;

	ret = ext2fs_bmap2(fs, ino, inode, block_buf, bmap_flags, block,
			    0, &ret_blk);
	གལ་སྲིད (ret)
		ལོག ret;
	གལ་སྲིད (ret_blk >= ((long long) 1 << 32))
		ལོག EOVERFLOW;
	*phys_blk = ret_blk;
	ལོག 0;
}
