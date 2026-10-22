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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <errno.h>

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fsP.h"

#எனில் defined(__GNUC__) && !defined(NO_INLINE_FUNCS)
#define _BMAP_INLINE_	__inline__
#இல்லையெனில்
#define _BMAP_INLINE_
#endif

வெளி errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     கட்டமைப்பு ext2_inode *inode,
			     எழுத்து *block_buf, முழு bmap_flags,
			     blk_t block, blk_t *phys_blk);

#define inode_bmap(inode, nr) ((inode)->i_block[(nr)])

நிலையான _BMAP_INLINE_ errcode_t block_ind_bmap(ext2_filsys fs, முழு flags,
					      blk_t ind, எழுத்து *block_buf,
					      முழு *blocks_alloc,
					      blk_t nr, blk_t *ret_blk)
{
	errcode_t	retval;
	blk_t		b;

	எனில் (!ind) {
		எனில் (flags & BMAP_SET)
			திரும்பு EXT2_ET_SET_BMAP_NO_IND;
		*ret_blk = 0;
		திரும்பு 0;
	}
	retval = io_channel_read_blk(fs->io, ind, 1, block_buf);
	எனில் (retval)
		திரும்பு retval;

	எனில் (flags & BMAP_SET) {
		b = *ret_blk;
#ifdef சொற்கள்_BIGENDIAN
		b = ext2fs_swab32(b);
#endif
		((blk_t *) block_buf)[nr] = b;
		திரும்பு io_channel_write_blk(fs->io, ind, 1, block_buf);
	}

	b = ((blk_t *) block_buf)[nr];

#ifdef சொற்கள்_BIGENDIAN
	b = ext2fs_swab32(b);
#endif

	எனில் (!b && (flags & BMAP_ALLOC)) {
		b = nr ? ext2fs_le32_to_cpu(((blk_t *)block_buf)[nr - 1]) : ind;
		retval = ext2fs_alloc_block(fs, b,
					    block_buf + fs->blocksize, &b);
		எனில் (retval)
			திரும்பு retval;

#ifdef சொற்கள்_BIGENDIAN
		((blk_t *) block_buf)[nr] = ext2fs_swab32(b);
#இல்லையெனில்
		((blk_t *) block_buf)[nr] = b;
#endif

		retval = io_channel_write_blk(fs->io, ind, 1, block_buf);
		எனில் (retval)
			திரும்பு retval;

		(*blocks_alloc)++;
	}

	*ret_blk = b;
	திரும்பு 0;
}

நிலையான _BMAP_INLINE_ errcode_t block_dind_bmap(ext2_filsys fs, முழு flags,
					       blk_t dind, எழுத்து *block_buf,
					       முழு *blocks_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_per_block;

	addr_per_block = (blk_t) fs->blocksize >> 2;

	retval = block_ind_bmap(fs, flags & ~BMAP_SET, dind, block_buf,
				blocks_alloc, nr / addr_per_block, &b);
	எனில் (retval)
		திரும்பு retval;
	retval = block_ind_bmap(fs, flags, b, block_buf, blocks_alloc,
				nr % addr_per_block, ret_blk);
	திரும்பு retval;
}

நிலையான _BMAP_INLINE_ errcode_t block_tind_bmap(ext2_filsys fs, முழு flags,
					       blk_t tind, எழுத்து *block_buf,
					       முழு *blocks_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_per_block;

	addr_per_block = (blk_t) fs->blocksize >> 2;

	retval = block_dind_bmap(fs, flags & ~BMAP_SET, tind, block_buf,
				 blocks_alloc, nr / addr_per_block, &b);
	எனில் (retval)
		திரும்பு retval;
	retval = block_ind_bmap(fs, flags, b, block_buf, blocks_alloc,
				nr % addr_per_block, ret_blk);
	திரும்பு retval;
}

நிலையான errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     கட்டமைப்பு ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     எழுத்து *block_buf, முழு bmap_flags, blk64_t block,
			     முழு *ret_flags, முழு *blocks_alloc,
			     blk64_t *phys_blk);

நிலையான errcode_t implied_cluster_alloc(ext2_filsys fs, ext2_ino_t ino,
				       கட்டமைப்பு ext2_inode *inode,
				       ext2_extent_handle_t handle,
				       blk64_t lblk, blk64_t *phys_blk)
{
	blk64_t	base_block, pblock = 0;
	முழு i;

	எனில் (!ext2fs_has_feature_bigalloc(fs->super))
		திரும்பு 0;

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
	க்கு (i = 0; i < EXT2FS_CLUSTER_RATIO(fs); i++) {
		எனில் (base_block + i == lblk)
			தொடர்;
		extent_bmap(fs, ino, inode, handle, 0, 0,
			    base_block + i, 0, 0, &pblock);
		எனில் (pblock)
			நிறுத்து;
	}
	எனில் (pblock == 0)
		திரும்பு 0;
	*phys_blk = pblock - i + (lblk - base_block);
	திரும்பு 0;
}

/* Try to map a logical block to an already-allocated physical cluster. */
errcode_t ext2fs_map_cluster_block(ext2_filsys fs, ext2_ino_t ino,
				   கட்டமைப்பு ext2_inode *inode, blk64_t lblk,
				   blk64_t *pblk)
{
	ext2_extent_handle_t handle;
	errcode_t retval;

	/* Need bigalloc and extents to be enabled */
	*pblk = 0;
	எனில் (!ext2fs_has_feature_bigalloc(fs->super) ||
	    !(inode->i_flags & EXT4_EXTENTS_FL))
		திரும்பு 0;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	எனில் (retval)
		செல் out;

	retval = implied_cluster_alloc(fs, ino, inode, handle, lblk, pblk);
	எனில் (retval)
		செல் out2;

out2:
	ext2fs_extent_free(handle);
out:
	திரும்பு retval;
}

நிலையான errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     கட்டமைப்பு ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     எழுத்து *block_buf, முழு bmap_flags, blk64_t block,
			     முழு *ret_flags, முழு *blocks_alloc,
			     blk64_t *phys_blk)
{
	கட்டமைப்பு blk_alloc_ctx	alloc_ctx;
	கட்டமைப்பு ext2fs_extent	extent;
	unsigned முழு		offset;
	errcode_t		retval = 0;
	blk64_t			blk64 = 0;
	முழு			alloc = 0;
	முழு			set_flags;

	set_flags = bmap_flags & BMAP_UNINIT ? EXT2_EXTENT_SET_BMAP_UNINIT : 0;

	எனில் (bmap_flags & BMAP_SET) {
		retval = ext2fs_extent_set_bmap(handle, block,
						*phys_blk, set_flags);
		திரும்பு retval;
	}
	retval = ext2fs_extent_goto(handle, block);
	எனில் (retval) {
		/* If the extent is not found, return phys_blk = 0 */
		எனில் (retval == EXT2_ET_EXTENT_NOT_FOUND) {
			extent.e_lblk = block;
			செல் got_block;
		}
		திரும்பு retval;
	}
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	எனில் (retval)
		திரும்பு retval;
	offset = block - extent.e_lblk;
	எனில் (block >= extent.e_lblk && (offset <= extent.e_len)) {
		*phys_blk = extent.e_pblk + offset;
		எனில் (ret_flags && extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			*ret_flags |= BMAP_RET_UNINIT;
	}
got_block:
	எனில் ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
		implied_cluster_alloc(fs, ino, inode, handle, block, &blk64);
		எனில் (blk64)
			செல் set_extent;
		retval = extent_bmap(fs, ino, inode, handle, block_buf,
				     0, block-1, 0, blocks_alloc, &blk64);
		எனில் (retval)
			blk64 = ext2fs_find_inode_goal(fs, ino, inode, block);
		alloc_ctx.ino = ino;
		alloc_ctx.inode = inode;
		alloc_ctx.lblk = extent.e_lblk;
		alloc_ctx.flags = BLOCK_ALLOC_DATA;
		retval = ext2fs_alloc_block3(fs, blk64, block_buf, &blk64,
					     &alloc_ctx);
		எனில் (retval)
			திரும்பு retval;
		blk64 &= ~EXT2FS_CLUSTER_MASK(fs);
		blk64 += EXT2FS_CLUSTER_MASK(fs) & block;
		alloc++;
	set_extent:
		retval = ext2fs_extent_set_bmap(handle, block,
						blk64, set_flags);
		எனில் (retval) {
			ext2fs_block_alloc_stats2(fs, blk64, -1);
			திரும்பு retval;
		}
		/* Update inode after setting extent */
		retval = ext2fs_read_inode(fs, ino, inode);
		எனில் (retval)
			திரும்பு retval;
		*blocks_alloc += alloc;
		*phys_blk = blk64;
	}
	திரும்பு 0;
}

முழு ext2fs_file_block_offset_too_big(ext2_filsys fs,
				     கட்டமைப்பு ext2_inode *inode,
				     blk64_t offset)
{
	blk64_t addr_per_block, max_map_block;

	/* Kernel seems to cut us off at 4294967294 blocks */
	எனில் (offset >= (1ULL << 32) - 1)
		திரும்பு 1;

	எனில் (inode->i_flags & EXT4_EXTENTS_FL)
		திரும்பு 0;

	addr_per_block = fs->blocksize >> 2;
	max_map_block = addr_per_block;
	max_map_block += addr_per_block * addr_per_block;
	max_map_block += addr_per_block * addr_per_block * addr_per_block;
	max_map_block += 12;

	திரும்பு offset >= max_map_block;
}

errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino, கட்டமைப்பு ext2_inode *inode,
		       எழுத்து *block_buf, முழு bmap_flags, blk64_t block,
		       முழு *ret_flags, blk64_t *phys_blk)
{
	கட்டமைப்பு ext2_inode inode_buf;
	ext2_extent_handle_t handle = 0;
	blk_t addr_per_block;
	blk_t	b, blk32;
	blk64_t b64;
	எழுத்து	*buf = 0;
	errcode_t	retval = 0;
	முழு		blocks_alloc = 0, inode_dirty = 0;
	கட்டமைப்பு blk_alloc_ctx alloc_ctx = {
		.ino	= ino,
		.inode	= inode,
		.lblk	= 0,
		.flags	= BLOCK_ALLOC_DATA,
	};

	எனில் (!(bmap_flags & BMAP_SET))
		*phys_blk = 0;

	எனில் (ret_flags)
		*ret_flags = 0;

	/* Read inode structure if necessary */
	எனில் (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		எனில் (retval)
			திரும்பு retval;
		inode = &inode_buf;
	}
	addr_per_block = (blk_t) fs->blocksize >> 2;

	எனில் (ext2fs_file_block_offset_too_big(fs, inode, block))
		திரும்பு EXT2_ET_FILE_TOO_BIG;

	/*
	 * If an inode has inline data, that means that it doesn't have
	 * any blocks and we shouldn't map any blocks for it.
	 */
	எனில் (inode->i_flags & EXT4_INLINE_DATA_FL)
		திரும்பு EXT2_ET_INLINE_DATA_NO_BLOCK;

	எனில் (!block_buf) {
		retval = ext2fs_get_array(2, fs->blocksize, &buf);
		எனில் (retval)
			திரும்பு retval;
		block_buf = buf;
	}

	எனில் (inode->i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		எனில் (retval)
			செல் done;
		retval = extent_bmap(fs, ino, inode, handle, block_buf,
				     bmap_flags, block, ret_flags,
				     &blocks_alloc, phys_blk);
		செல் done;
	}

	எனில் (block < EXT2_NDIR_BLOCKS) {
		எனில் (bmap_flags & BMAP_SET) {
			b = *phys_blk;
			inode_bmap(inode, block) = b;
			inode_dirty++;
			செல் done;
		}

		*phys_blk = inode_bmap(inode, block);
		b = block ? inode_bmap(inode, block - 1) :
			    ext2fs_find_inode_goal(fs, ino, inode, block);

		எனில் ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
						     &alloc_ctx);
			b = b64;
			எனில் (retval)
				செல் done;
			inode_bmap(inode, block) = b;
			blocks_alloc++;
			*phys_blk = b;
		}
		செல் done;
	}

	/* Indirect block */
	block -= EXT2_NDIR_BLOCKS;
	blk32 = *phys_blk;
	எனில் (block < addr_per_block) {
		b = inode_bmap(inode, EXT2_IND_BLOCK);
		எனில் (!b) {
			எனில் (!(bmap_flags & BMAP_ALLOC)) {
				எனில் (bmap_flags & BMAP_SET)
					retval = EXT2_ET_SET_BMAP_NO_IND;
				செல் done;
			}

			b = inode_bmap(inode, EXT2_IND_BLOCK-1);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
						     &alloc_ctx);
			b = b64;
			எனில் (retval)
				செல் done;
			inode_bmap(inode, EXT2_IND_BLOCK) = b;
			blocks_alloc++;
		}
		retval = block_ind_bmap(fs, bmap_flags, b, block_buf,
					&blocks_alloc, block, &blk32);
		எனில் (retval == 0)
			*phys_blk = blk32;
		செல் done;
	}

	/* Doubly indirect block  */
	block -= addr_per_block;
	எனில் (block < addr_per_block * addr_per_block) {
		b = inode_bmap(inode, EXT2_DIND_BLOCK);
		எனில் (!b) {
			எனில் (!(bmap_flags & BMAP_ALLOC)) {
				எனில் (bmap_flags & BMAP_SET)
					retval = EXT2_ET_SET_BMAP_NO_IND;
				செல் done;
			}

			b = inode_bmap(inode, EXT2_IND_BLOCK);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
						     &alloc_ctx);
			b = b64;
			எனில் (retval)
				செல் done;
			inode_bmap(inode, EXT2_DIND_BLOCK) = b;
			blocks_alloc++;
		}
		retval = block_dind_bmap(fs, bmap_flags, b, block_buf,
					 &blocks_alloc, block, &blk32);
		எனில் (retval == 0)
			*phys_blk = blk32;
		செல் done;
	}

	/* Triply indirect block */
	block -= addr_per_block * addr_per_block;
	b = inode_bmap(inode, EXT2_TIND_BLOCK);
	எனில் (!b) {
		எனில் (!(bmap_flags & BMAP_ALLOC)) {
			எனில் (bmap_flags & BMAP_SET)
				retval = EXT2_ET_SET_BMAP_NO_IND;
			செல் done;
		}

		b = inode_bmap(inode, EXT2_DIND_BLOCK);
		b64 = b;
		retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
					     &alloc_ctx);
		b = b64;
		எனில் (retval)
			செல் done;
		inode_bmap(inode, EXT2_TIND_BLOCK) = b;
		blocks_alloc++;
	}
	retval = block_tind_bmap(fs, bmap_flags, b, block_buf,
				 &blocks_alloc, block, &blk32);
	எனில் (retval == 0)
		*phys_blk = blk32;
done:
	எனில் (*phys_blk && retval == 0 && (bmap_flags & BMAP_ZERO))
		retval = ext2fs_zero_blocks2(fs, *phys_blk, 1, வெற்று, வெற்று);
	எனில் (buf)
		ext2fs_free_mem(&buf);
	எனில் (handle)
		ext2fs_extent_free(handle);
	எனில் ((retval == 0) && (blocks_alloc || inode_dirty)) {
		ext2fs_iblk_add_blocks(fs, inode, blocks_alloc);
		retval = ext2fs_write_inode(fs, ino, inode);
	}
	திரும்பு retval;
}

errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino, கட்டமைப்பு ext2_inode *inode,
		      எழுத்து *block_buf, முழு bmap_flags, blk_t block,
		      blk_t *phys_blk)
{
	errcode_t ret;
	blk64_t	ret_blk = *phys_blk;

	ret = ext2fs_bmap2(fs, ino, inode, block_buf, bmap_flags, block,
			    0, &ret_blk);
	எனில் (ret)
		திரும்பு ret;
	எனில் (ret_blk >= ((long long) 1 << 32))
		திரும்பு EOVERFLOW;
	*phys_blk = ret_blk;
	திரும்பு 0;
}
