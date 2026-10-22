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
#ᱡᱩᱫᱤ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <errno.h>

#include "ext2_fs.h"
#include "ext2fsP.h"

#ᱡᱩᱫᱤ defined(__GNUC__) && !defined(NO_INLINE_FUNCS)
#define _BMAP_INLINE_	__inline__
#ᱵᱟᱝ
#define _BMAP_INLINE_
#endif

extern errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     sa_nracanaa ext2_inode *inode,
			     var_na *block_buf, puur_naa_nka bmap_flags,
			     blk_t block, blk_t *phys_blk);

#define inode_bmap(inode, nr) ((inode)->i_block[(nr)])

static _BMAP_INLINE_ errcode_t block_ind_bmap(ext2_filsys fs, puur_naa_nka flags,
					      blk_t ind, var_na *block_buf,
					      puur_naa_nka *blocks_alloc,
					      blk_t nr, blk_t *ret_blk)
{
	errcode_t	retval;
	blk_t		b;

	ᱡᱩᱫᱤ (!ind) {
		ᱡᱩᱫᱤ (flags & BMAP_SET)
			ᱨᱩᱣᱟᱹᱲ EXT2_ET_SET_BMAP_NO_IND;
		*ret_blk = 0;
		ᱨᱩᱣᱟᱹᱲ 0;
	}
	retval = io_channel_read_blk(fs->io, ind, 1, block_buf);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;

	ᱡᱩᱫᱤ (flags & BMAP_SET) {
		b = *ret_blk;
#ifdef WORDS_BIGENDIAN
		b = ext2fs_swab32(b);
#endif
		((blk_t *) block_buf)[nr] = b;
		ᱨᱩᱣᱟᱹᱲ io_channel_write_blk(fs->io, ind, 1, block_buf);
	}

	b = ((blk_t *) block_buf)[nr];

#ifdef WORDS_BIGENDIAN
	b = ext2fs_swab32(b);
#endif

	ᱡᱩᱫᱤ (!b && (flags & BMAP_ALLOC)) {
		b = nr ? ext2fs_le32_to_cpu(((blk_t *)block_buf)[nr - 1]) : ind;
		retval = ext2fs_alloc_block(fs, b,
					    block_buf + fs->blocksize, &b);
		ᱡᱩᱫᱤ (retval)
			ᱨᱩᱣᱟᱹᱲ retval;

#ifdef WORDS_BIGENDIAN
		((blk_t *) block_buf)[nr] = ext2fs_swab32(b);
#ᱵᱟᱝ
		((blk_t *) block_buf)[nr] = b;
#endif

		retval = io_channel_write_blk(fs->io, ind, 1, block_buf);
		ᱡᱩᱫᱤ (retval)
			ᱨᱩᱣᱟᱹᱲ retval;

		(*blocks_alloc)++;
	}

	*ret_blk = b;
	ᱨᱩᱣᱟᱹᱲ 0;
}

static _BMAP_INLINE_ errcode_t block_dind_bmap(ext2_filsys fs, puur_naa_nka flags,
					       blk_t dind, var_na *block_buf,
					       puur_naa_nka *blocks_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_per_block;

	addr_per_block = (blk_t) fs->blocksize >> 2;

	retval = block_ind_bmap(fs, flags & ~BMAP_SET, dind, block_buf,
				blocks_alloc, nr / addr_per_block, &b);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;
	retval = block_ind_bmap(fs, flags, b, block_buf, blocks_alloc,
				nr % addr_per_block, ret_blk);
	ᱨᱩᱣᱟᱹᱲ retval;
}

static _BMAP_INLINE_ errcode_t block_tind_bmap(ext2_filsys fs, puur_naa_nka flags,
					       blk_t tind, var_na *block_buf,
					       puur_naa_nka *blocks_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_per_block;

	addr_per_block = (blk_t) fs->blocksize >> 2;

	retval = block_dind_bmap(fs, flags & ~BMAP_SET, tind, block_buf,
				 blocks_alloc, nr / addr_per_block, &b);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;
	retval = block_ind_bmap(fs, flags, b, block_buf, blocks_alloc,
				nr % addr_per_block, ret_blk);
	ᱨᱩᱣᱟᱹᱲ retval;
}

static errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     sa_nracanaa ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     var_na *block_buf, puur_naa_nka bmap_flags, blk64_t block,
			     puur_naa_nka *ret_flags, puur_naa_nka *blocks_alloc,
			     blk64_t *phys_blk);

static errcode_t implied_cluster_alloc(ext2_filsys fs, ext2_ino_t ino,
				       sa_nracanaa ext2_inode *inode,
				       ext2_extent_handle_t handle,
				       blk64_t lblk, blk64_t *phys_blk)
{
	blk64_t	base_block, pblock = 0;
	puur_naa_nka i;

	ᱡᱩᱫᱤ (!ext2fs_has_feature_bigalloc(fs->super))
		ᱨᱩᱣᱟᱹᱲ 0;

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
	for (i = 0; i < EXT2FS_CLUSTER_RATIO(fs); i++) {
		ᱡᱩᱫᱤ (base_block + i == lblk)
			continue;
		extent_bmap(fs, ino, inode, handle, 0, 0,
			    base_block + i, 0, 0, &pblock);
		ᱡᱩᱫᱤ (pblock)
			bha_nga;
	}
	ᱡᱩᱫᱤ (pblock == 0)
		ᱨᱩᱣᱟᱹᱲ 0;
	*phys_blk = pblock - i + (lblk - base_block);
	ᱨᱩᱣᱟᱹᱲ 0;
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
	ᱡᱩᱫᱤ (!ext2fs_has_feature_bigalloc(fs->super) ||
	    !(inode->i_flags & EXT4_EXTENTS_FL))
		ᱨᱩᱣᱟᱹᱲ 0;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	ᱡᱩᱫᱤ (retval)
		goto out;

	retval = implied_cluster_alloc(fs, ino, inode, handle, lblk, pblk);
	ᱡᱩᱫᱤ (retval)
		goto out2;

out2:
	ext2fs_extent_free(handle);
out:
	ᱨᱩᱣᱟᱹᱲ retval;
}

static errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     sa_nracanaa ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     var_na *block_buf, puur_naa_nka bmap_flags, blk64_t block,
			     puur_naa_nka *ret_flags, puur_naa_nka *blocks_alloc,
			     blk64_t *phys_blk)
{
	sa_nracanaa blk_alloc_ctx	alloc_ctx;
	sa_nracanaa ext2fs_extent	extent;
	unsigned puur_naa_nka		offset;
	errcode_t		retval = 0;
	blk64_t			blk64 = 0;
	puur_naa_nka			alloc = 0;
	puur_naa_nka			set_flags;

	set_flags = bmap_flags & BMAP_UNINIT ? EXT2_EXTENT_SET_BMAP_UNINIT : 0;

	ᱡᱩᱫᱤ (bmap_flags & BMAP_SET) {
		retval = ext2fs_extent_set_bmap(handle, block,
						*phys_blk, set_flags);
		ᱨᱩᱣᱟᱹᱲ retval;
	}
	retval = ext2fs_extent_goto(handle, block);
	ᱡᱩᱫᱤ (retval) {
		/* If the extent is not found, return phys_blk = 0 */
		ᱡᱩᱫᱤ (retval == EXT2_ET_EXTENT_NOT_FOUND) {
			extent.e_lblk = block;
			goto got_block;
		}
		ᱨᱩᱣᱟᱹᱲ retval;
	}
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ᱡᱩᱫᱤ (retval)
		ᱨᱩᱣᱟᱹᱲ retval;
	offset = block - extent.e_lblk;
	ᱡᱩᱫᱤ (block >= extent.e_lblk && (offset <= extent.e_len)) {
		*phys_blk = extent.e_pblk + offset;
		ᱡᱩᱫᱤ (ret_flags && extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			*ret_flags |= BMAP_RET_UNINIT;
	}
got_block:
	ᱡᱩᱫᱤ ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
		implied_cluster_alloc(fs, ino, inode, handle, block, &blk64);
		ᱡᱩᱫᱤ (blk64)
			goto set_extent;
		retval = extent_bmap(fs, ino, inode, handle, block_buf,
				     0, block-1, 0, blocks_alloc, &blk64);
		ᱡᱩᱫᱤ (retval)
			blk64 = ext2fs_find_inode_goal(fs, ino, inode, block);
		alloc_ctx.ino = ino;
		alloc_ctx.inode = inode;
		alloc_ctx.lblk = extent.e_lblk;
		alloc_ctx.flags = BLOCK_ALLOC_DATA;
		retval = ext2fs_alloc_block3(fs, blk64, block_buf, &blk64,
					     &alloc_ctx);
		ᱡᱩᱫᱤ (retval)
			ᱨᱩᱣᱟᱹᱲ retval;
		blk64 &= ~EXT2FS_CLUSTER_MASK(fs);
		blk64 += EXT2FS_CLUSTER_MASK(fs) & block;
		alloc++;
	set_extent:
		retval = ext2fs_extent_set_bmap(handle, block,
						blk64, set_flags);
		ᱡᱩᱫᱤ (retval) {
			ext2fs_block_alloc_stats2(fs, blk64, -1);
			ᱨᱩᱣᱟᱹᱲ retval;
		}
		/* Update inode after setting extent */
		retval = ext2fs_read_inode(fs, ino, inode);
		ᱡᱩᱫᱤ (retval)
			ᱨᱩᱣᱟᱹᱲ retval;
		*blocks_alloc += alloc;
		*phys_blk = blk64;
	}
	ᱨᱩᱣᱟᱹᱲ 0;
}

puur_naa_nka ext2fs_file_block_offset_too_big(ext2_filsys fs,
				     sa_nracanaa ext2_inode *inode,
				     blk64_t offset)
{
	blk64_t addr_per_block, max_map_block;

	/* Kernel seems to cut us off at 4294967294 blocks */
	ᱡᱩᱫᱤ (offset >= (1ULL << 32) - 1)
		ᱨᱩᱣᱟᱹᱲ 1;

	ᱡᱩᱫᱤ (inode->i_flags & EXT4_EXTENTS_FL)
		ᱨᱩᱣᱟᱹᱲ 0;

	addr_per_block = fs->blocksize >> 2;
	max_map_block = addr_per_block;
	max_map_block += addr_per_block * addr_per_block;
	max_map_block += addr_per_block * addr_per_block * addr_per_block;
	max_map_block += 12;

	ᱨᱩᱣᱟᱹᱲ offset >= max_map_block;
}

errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino, sa_nracanaa ext2_inode *inode,
		       var_na *block_buf, puur_naa_nka bmap_flags, blk64_t block,
		       puur_naa_nka *ret_flags, blk64_t *phys_blk)
{
	sa_nracanaa ext2_inode inode_buf;
	ext2_extent_handle_t handle = 0;
	blk_t addr_per_block;
	blk_t	b, blk32;
	blk64_t b64;
	var_na	*buf = 0;
	errcode_t	retval = 0;
	puur_naa_nka		blocks_alloc = 0, inode_dirty = 0;
	sa_nracanaa blk_alloc_ctx alloc_ctx = {
		.ino	= ino,
		.inode	= inode,
		.lblk	= 0,
		.flags	= BLOCK_ALLOC_DATA,
	};

	ᱡᱩᱫᱤ (!(bmap_flags & BMAP_SET))
		*phys_blk = 0;

	ᱡᱩᱫᱤ (ret_flags)
		*ret_flags = 0;

	/* Read inode structure if necessary */
	ᱡᱩᱫᱤ (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		ᱡᱩᱫᱤ (retval)
			ᱨᱩᱣᱟᱹᱲ retval;
		inode = &inode_buf;
	}
	addr_per_block = (blk_t) fs->blocksize >> 2;

	ᱡᱩᱫᱤ (ext2fs_file_block_offset_too_big(fs, inode, block))
		ᱨᱩᱣᱟᱹᱲ EXT2_ET_FILE_TOO_BIG;

	/*
	 * If an inode has inline data, that means that it doesn't have
	 * any blocks and we shouldn't map any blocks for it.
	 */
	ᱡᱩᱫᱤ (inode->i_flags & EXT4_INLINE_DATA_FL)
		ᱨᱩᱣᱟᱹᱲ EXT2_ET_INLINE_DATA_NO_BLOCK;

	ᱡᱩᱫᱤ (!block_buf) {
		retval = ext2fs_get_array(2, fs->blocksize, &buf);
		ᱡᱩᱫᱤ (retval)
			ᱨᱩᱣᱟᱹᱲ retval;
		block_buf = buf;
	}

	ᱡᱩᱫᱤ (inode->i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		ᱡᱩᱫᱤ (retval)
			goto done;
		retval = extent_bmap(fs, ino, inode, handle, block_buf,
				     bmap_flags, block, ret_flags,
				     &blocks_alloc, phys_blk);
		goto done;
	}

	ᱡᱩᱫᱤ (block < EXT2_NDIR_BLOCKS) {
		ᱡᱩᱫᱤ (bmap_flags & BMAP_SET) {
			b = *phys_blk;
			inode_bmap(inode, block) = b;
			inode_dirty++;
			goto done;
		}

		*phys_blk = inode_bmap(inode, block);
		b = block ? inode_bmap(inode, block - 1) :
			    ext2fs_find_inode_goal(fs, ino, inode, block);

		ᱡᱩᱫᱤ ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
						     &alloc_ctx);
			b = b64;
			ᱡᱩᱫᱤ (retval)
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
	ᱡᱩᱫᱤ (block < addr_per_block) {
		b = inode_bmap(inode, EXT2_IND_BLOCK);
		ᱡᱩᱫᱤ (!b) {
			ᱡᱩᱫᱤ (!(bmap_flags & BMAP_ALLOC)) {
				ᱡᱩᱫᱤ (bmap_flags & BMAP_SET)
					retval = EXT2_ET_SET_BMAP_NO_IND;
				goto done;
			}

			b = inode_bmap(inode, EXT2_IND_BLOCK-1);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
						     &alloc_ctx);
			b = b64;
			ᱡᱩᱫᱤ (retval)
				goto done;
			inode_bmap(inode, EXT2_IND_BLOCK) = b;
			blocks_alloc++;
		}
		retval = block_ind_bmap(fs, bmap_flags, b, block_buf,
					&blocks_alloc, block, &blk32);
		ᱡᱩᱫᱤ (retval == 0)
			*phys_blk = blk32;
		goto done;
	}

	/* Doubly indirect block  */
	block -= addr_per_block;
	ᱡᱩᱫᱤ (block < addr_per_block * addr_per_block) {
		b = inode_bmap(inode, EXT2_DIND_BLOCK);
		ᱡᱩᱫᱤ (!b) {
			ᱡᱩᱫᱤ (!(bmap_flags & BMAP_ALLOC)) {
				ᱡᱩᱫᱤ (bmap_flags & BMAP_SET)
					retval = EXT2_ET_SET_BMAP_NO_IND;
				goto done;
			}

			b = inode_bmap(inode, EXT2_IND_BLOCK);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
						     &alloc_ctx);
			b = b64;
			ᱡᱩᱫᱤ (retval)
				goto done;
			inode_bmap(inode, EXT2_DIND_BLOCK) = b;
			blocks_alloc++;
		}
		retval = block_dind_bmap(fs, bmap_flags, b, block_buf,
					 &blocks_alloc, block, &blk32);
		ᱡᱩᱫᱤ (retval == 0)
			*phys_blk = blk32;
		goto done;
	}

	/* Triply indirect block */
	block -= addr_per_block * addr_per_block;
	b = inode_bmap(inode, EXT2_TIND_BLOCK);
	ᱡᱩᱫᱤ (!b) {
		ᱡᱩᱫᱤ (!(bmap_flags & BMAP_ALLOC)) {
			ᱡᱩᱫᱤ (bmap_flags & BMAP_SET)
				retval = EXT2_ET_SET_BMAP_NO_IND;
			goto done;
		}

		b = inode_bmap(inode, EXT2_DIND_BLOCK);
		b64 = b;
		retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
					     &alloc_ctx);
		b = b64;
		ᱡᱩᱫᱤ (retval)
			goto done;
		inode_bmap(inode, EXT2_TIND_BLOCK) = b;
		blocks_alloc++;
	}
	retval = block_tind_bmap(fs, bmap_flags, b, block_buf,
				 &blocks_alloc, block, &blk32);
	ᱡᱩᱫᱤ (retval == 0)
		*phys_blk = blk32;
done:
	ᱡᱩᱫᱤ (*phys_blk && retval == 0 && (bmap_flags & BMAP_ZERO))
		retval = ext2fs_zero_blocks2(fs, *phys_blk, 1, NULL, NULL);
	ᱡᱩᱫᱤ (buf)
		ext2fs_free_mem(&buf);
	ᱡᱩᱫᱤ (handle)
		ext2fs_extent_free(handle);
	ᱡᱩᱫᱤ ((retval == 0) && (blocks_alloc || inode_dirty)) {
		ext2fs_iblk_add_blocks(fs, inode, blocks_alloc);
		retval = ext2fs_write_inode(fs, ino, inode);
	}
	ᱨᱩᱣᱟᱹᱲ retval;
}

errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino, sa_nracanaa ext2_inode *inode,
		      var_na *block_buf, puur_naa_nka bmap_flags, blk_t block,
		      blk_t *phys_blk)
{
	errcode_t ret;
	blk64_t	ret_blk = *phys_blk;

	ret = ext2fs_bmap2(fs, ino, inode, block_buf, bmap_flags, block,
			    0, &ret_blk);
	ᱡᱩᱫᱤ (ret)
		ᱨᱩᱣᱟᱹᱲ ret;
	ᱡᱩᱫᱤ (ret_blk >= ((long long) 1 << 32))
		ᱨᱩᱣᱟᱹᱲ EOVERFLOW;
	*phys_blk = ret_blk;
	ᱨᱩᱣᱟᱹᱲ 0;
}
