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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <errno.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

#जर defined(__GNUC__) && !defined(NO_INLINE_FUNCS)
#define _BMAP_INLINE_	__inline__
#अन्यथा
#define _BMAP_INLINE_
#endif

बाह्य errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     संरचना ext2_inode *inode,
			     अक्षर *block_buf, पूर्णांक bmap_flags,
			     blk_t block, blk_t *phys_blk);

#define inode_bmap(inode, nr) ((inode)->i_block[(nr)])

स्थिर _BMAP_INLINE_ errcode_t block_ind_bmap(ext2_filsys fs, पूर्णांक flags,
					      blk_t ind, अक्षर *block_buf,
					      पूर्णांक *blocks_alloc,
					      blk_t nr, blk_t *ret_blk)
{
	errcode_t	retval;
	blk_t		b;

	जर (!ind) {
		जर (flags & BMAP_SET)
			परत EXT2_ET_SET_BMAP_NO_IND;
		*ret_blk = 0;
		परत 0;
	}
	retval = io_channel_read_blk(fs->io, ind, 1, block_buf);
	जर (retval)
		परत retval;

	जर (flags & BMAP_SET) {
		b = *ret_blk;
#ifdef शब्द_BIGENDIAN
		b = ext2fs_swab32(b);
#endif
		((blk_t *) block_buf)[nr] = b;
		परत io_channel_write_blk(fs->io, ind, 1, block_buf);
	}

	b = ((blk_t *) block_buf)[nr];

#ifdef शब्द_BIGENDIAN
	b = ext2fs_swab32(b);
#endif

	जर (!b && (flags & BMAP_ALLOC)) {
		b = nr ? ext2fs_le32_to_cpu(((blk_t *)block_buf)[nr - 1]) : ind;
		retval = ext2fs_alloc_block(fs, b,
					    block_buf + fs->blocksize, &b);
		जर (retval)
			परत retval;

#ifdef शब्द_BIGENDIAN
		((blk_t *) block_buf)[nr] = ext2fs_swab32(b);
#अन्यथा
		((blk_t *) block_buf)[nr] = b;
#endif

		retval = io_channel_write_blk(fs->io, ind, 1, block_buf);
		जर (retval)
			परत retval;

		(*blocks_alloc)++;
	}

	*ret_blk = b;
	परत 0;
}

स्थिर _BMAP_INLINE_ errcode_t block_dind_bmap(ext2_filsys fs, पूर्णांक flags,
					       blk_t dind, अक्षर *block_buf,
					       पूर्णांक *blocks_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_per_block;

	addr_per_block = (blk_t) fs->blocksize >> 2;

	retval = block_ind_bmap(fs, flags & ~BMAP_SET, dind, block_buf,
				blocks_alloc, nr / addr_per_block, &b);
	जर (retval)
		परत retval;
	retval = block_ind_bmap(fs, flags, b, block_buf, blocks_alloc,
				nr % addr_per_block, ret_blk);
	परत retval;
}

स्थिर _BMAP_INLINE_ errcode_t block_tind_bmap(ext2_filsys fs, पूर्णांक flags,
					       blk_t tind, अक्षर *block_buf,
					       पूर्णांक *blocks_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_per_block;

	addr_per_block = (blk_t) fs->blocksize >> 2;

	retval = block_dind_bmap(fs, flags & ~BMAP_SET, tind, block_buf,
				 blocks_alloc, nr / addr_per_block, &b);
	जर (retval)
		परत retval;
	retval = block_ind_bmap(fs, flags, b, block_buf, blocks_alloc,
				nr % addr_per_block, ret_blk);
	परत retval;
}

स्थिर errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     संरचना ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     अक्षर *block_buf, पूर्णांक bmap_flags, blk64_t block,
			     पूर्णांक *ret_flags, पूर्णांक *blocks_alloc,
			     blk64_t *phys_blk);

स्थिर errcode_t implied_cluster_alloc(ext2_filsys fs, ext2_ino_t ino,
				       संरचना ext2_inode *inode,
				       ext2_extent_handle_t handle,
				       blk64_t lblk, blk64_t *phys_blk)
{
	blk64_t	base_block, pblock = 0;
	पूर्णांक i;

	जर (!ext2fs_has_feature_bigalloc(fs->super))
		परत 0;

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
	साठी (i = 0; i < EXT2FS_CLUSTER_RATIO(fs); i++) {
		जर (base_block + i == lblk)
			सुरूठेवा;
		extent_bmap(fs, ino, inode, handle, 0, 0,
			    base_block + i, 0, 0, &pblock);
		जर (pblock)
			थांबा;
	}
	जर (pblock == 0)
		परत 0;
	*phys_blk = pblock - i + (lblk - base_block);
	परत 0;
}

/* Try to map a logical block to an already-allocated physical cluster. */
errcode_t ext2fs_map_cluster_block(ext2_filsys fs, ext2_ino_t ino,
				   संरचना ext2_inode *inode, blk64_t lblk,
				   blk64_t *pblk)
{
	ext2_extent_handle_t handle;
	errcode_t retval;

	/* Need bigalloc and extents to be enabled */
	*pblk = 0;
	जर (!ext2fs_has_feature_bigalloc(fs->super) ||
	    !(inode->i_flags & EXT4_EXTENTS_FL))
		परत 0;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	जर (retval)
		जा out;

	retval = implied_cluster_alloc(fs, ino, inode, handle, lblk, pblk);
	जर (retval)
		जा out2;

out2:
	ext2fs_extent_free(handle);
out:
	परत retval;
}

स्थिर errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     संरचना ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     अक्षर *block_buf, पूर्णांक bmap_flags, blk64_t block,
			     पूर्णांक *ret_flags, पूर्णांक *blocks_alloc,
			     blk64_t *phys_blk)
{
	संरचना blk_alloc_ctx	alloc_ctx;
	संरचना ext2fs_extent	extent;
	अचिन्हित पूर्णांक		offset;
	errcode_t		retval = 0;
	blk64_t			blk64 = 0;
	पूर्णांक			alloc = 0;
	पूर्णांक			set_flags;

	set_flags = bmap_flags & BMAP_UNINIT ? EXT2_EXTENT_SET_BMAP_UNINIT : 0;

	जर (bmap_flags & BMAP_SET) {
		retval = ext2fs_extent_set_bmap(handle, block,
						*phys_blk, set_flags);
		परत retval;
	}
	retval = ext2fs_extent_goto(handle, block);
	जर (retval) {
		/* If the extent is not found, return phys_blk = 0 */
		जर (retval == EXT2_ET_EXTENT_NOT_FOUND) {
			extent.e_lblk = block;
			जा got_block;
		}
		परत retval;
	}
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	जर (retval)
		परत retval;
	offset = block - extent.e_lblk;
	जर (block >= extent.e_lblk && (offset <= extent.e_len)) {
		*phys_blk = extent.e_pblk + offset;
		जर (ret_flags && extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			*ret_flags |= BMAP_RET_UNINIT;
	}
got_block:
	जर ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
		implied_cluster_alloc(fs, ino, inode, handle, block, &blk64);
		जर (blk64)
			जा set_extent;
		retval = extent_bmap(fs, ino, inode, handle, block_buf,
				     0, block-1, 0, blocks_alloc, &blk64);
		जर (retval)
			blk64 = ext2fs_find_inode_goal(fs, ino, inode, block);
		alloc_ctx.ino = ino;
		alloc_ctx.inode = inode;
		alloc_ctx.lblk = extent.e_lblk;
		alloc_ctx.flags = BLOCK_ALLOC_DATA;
		retval = ext2fs_alloc_block3(fs, blk64, block_buf, &blk64,
					     &alloc_ctx);
		जर (retval)
			परत retval;
		blk64 &= ~EXT2FS_CLUSTER_MASK(fs);
		blk64 += EXT2FS_CLUSTER_MASK(fs) & block;
		alloc++;
	set_extent:
		retval = ext2fs_extent_set_bmap(handle, block,
						blk64, set_flags);
		जर (retval) {
			ext2fs_block_alloc_stats2(fs, blk64, -1);
			परत retval;
		}
		/* Update inode after setting extent */
		retval = ext2fs_read_inode(fs, ino, inode);
		जर (retval)
			परत retval;
		*blocks_alloc += alloc;
		*phys_blk = blk64;
	}
	परत 0;
}

पूर्णांक ext2fs_file_block_offset_too_big(ext2_filsys fs,
				     संरचना ext2_inode *inode,
				     blk64_t offset)
{
	blk64_t addr_per_block, max_map_block;

	/* Kernel seems to cut us off at 4294967294 blocks */
	जर (offset >= (1ULL << 32) - 1)
		परत 1;

	जर (inode->i_flags & EXT4_EXTENTS_FL)
		परत 0;

	addr_per_block = fs->blocksize >> 2;
	max_map_block = addr_per_block;
	max_map_block += addr_per_block * addr_per_block;
	max_map_block += addr_per_block * addr_per_block * addr_per_block;
	max_map_block += 12;

	परत offset >= max_map_block;
}

errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino, संरचना ext2_inode *inode,
		       अक्षर *block_buf, पूर्णांक bmap_flags, blk64_t block,
		       पूर्णांक *ret_flags, blk64_t *phys_blk)
{
	संरचना ext2_inode inode_buf;
	ext2_extent_handle_t handle = 0;
	blk_t addr_per_block;
	blk_t	b, blk32;
	blk64_t b64;
	अक्षर	*buf = 0;
	errcode_t	retval = 0;
	पूर्णांक		blocks_alloc = 0, inode_dirty = 0;
	संरचना blk_alloc_ctx alloc_ctx = {
		.ino	= ino,
		.inode	= inode,
		.lblk	= 0,
		.flags	= BLOCK_ALLOC_DATA,
	};

	जर (!(bmap_flags & BMAP_SET))
		*phys_blk = 0;

	जर (ret_flags)
		*ret_flags = 0;

	/* Read inode structure if necessary */
	जर (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		जर (retval)
			परत retval;
		inode = &inode_buf;
	}
	addr_per_block = (blk_t) fs->blocksize >> 2;

	जर (ext2fs_file_block_offset_too_big(fs, inode, block))
		परत EXT2_ET_FILE_TOO_BIG;

	/*
	 * If an inode has inline data, that means that it doesn't have
	 * any blocks and we shouldn't map any blocks for it.
	 */
	जर (inode->i_flags & EXT4_INLINE_DATA_FL)
		परत EXT2_ET_INLINE_DATA_NO_BLOCK;

	जर (!block_buf) {
		retval = ext2fs_get_array(2, fs->blocksize, &buf);
		जर (retval)
			परत retval;
		block_buf = buf;
	}

	जर (inode->i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		जर (retval)
			जा done;
		retval = extent_bmap(fs, ino, inode, handle, block_buf,
				     bmap_flags, block, ret_flags,
				     &blocks_alloc, phys_blk);
		जा done;
	}

	जर (block < EXT2_NDIR_BLOCKS) {
		जर (bmap_flags & BMAP_SET) {
			b = *phys_blk;
			inode_bmap(inode, block) = b;
			inode_dirty++;
			जा done;
		}

		*phys_blk = inode_bmap(inode, block);
		b = block ? inode_bmap(inode, block - 1) :
			    ext2fs_find_inode_goal(fs, ino, inode, block);

		जर ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
						     &alloc_ctx);
			b = b64;
			जर (retval)
				जा done;
			inode_bmap(inode, block) = b;
			blocks_alloc++;
			*phys_blk = b;
		}
		जा done;
	}

	/* Indirect block */
	block -= EXT2_NDIR_BLOCKS;
	blk32 = *phys_blk;
	जर (block < addr_per_block) {
		b = inode_bmap(inode, EXT2_IND_BLOCK);
		जर (!b) {
			जर (!(bmap_flags & BMAP_ALLOC)) {
				जर (bmap_flags & BMAP_SET)
					retval = EXT2_ET_SET_BMAP_NO_IND;
				जा done;
			}

			b = inode_bmap(inode, EXT2_IND_BLOCK-1);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
						     &alloc_ctx);
			b = b64;
			जर (retval)
				जा done;
			inode_bmap(inode, EXT2_IND_BLOCK) = b;
			blocks_alloc++;
		}
		retval = block_ind_bmap(fs, bmap_flags, b, block_buf,
					&blocks_alloc, block, &blk32);
		जर (retval == 0)
			*phys_blk = blk32;
		जा done;
	}

	/* Doubly indirect block  */
	block -= addr_per_block;
	जर (block < addr_per_block * addr_per_block) {
		b = inode_bmap(inode, EXT2_DIND_BLOCK);
		जर (!b) {
			जर (!(bmap_flags & BMAP_ALLOC)) {
				जर (bmap_flags & BMAP_SET)
					retval = EXT2_ET_SET_BMAP_NO_IND;
				जा done;
			}

			b = inode_bmap(inode, EXT2_IND_BLOCK);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
						     &alloc_ctx);
			b = b64;
			जर (retval)
				जा done;
			inode_bmap(inode, EXT2_DIND_BLOCK) = b;
			blocks_alloc++;
		}
		retval = block_dind_bmap(fs, bmap_flags, b, block_buf,
					 &blocks_alloc, block, &blk32);
		जर (retval == 0)
			*phys_blk = blk32;
		जा done;
	}

	/* Triply indirect block */
	block -= addr_per_block * addr_per_block;
	b = inode_bmap(inode, EXT2_TIND_BLOCK);
	जर (!b) {
		जर (!(bmap_flags & BMAP_ALLOC)) {
			जर (bmap_flags & BMAP_SET)
				retval = EXT2_ET_SET_BMAP_NO_IND;
			जा done;
		}

		b = inode_bmap(inode, EXT2_DIND_BLOCK);
		b64 = b;
		retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
					     &alloc_ctx);
		b = b64;
		जर (retval)
			जा done;
		inode_bmap(inode, EXT2_TIND_BLOCK) = b;
		blocks_alloc++;
	}
	retval = block_tind_bmap(fs, bmap_flags, b, block_buf,
				 &blocks_alloc, block, &blk32);
	जर (retval == 0)
		*phys_blk = blk32;
done:
	जर (*phys_blk && retval == 0 && (bmap_flags & BMAP_ZERO))
		retval = ext2fs_zero_blocks2(fs, *phys_blk, 1, रिक्त, रिक्त);
	जर (buf)
		ext2fs_free_mem(&buf);
	जर (handle)
		ext2fs_extent_free(handle);
	जर ((retval == 0) && (blocks_alloc || inode_dirty)) {
		ext2fs_iblk_add_blocks(fs, inode, blocks_alloc);
		retval = ext2fs_write_inode(fs, ino, inode);
	}
	परत retval;
}

errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino, संरचना ext2_inode *inode,
		      अक्षर *block_buf, पूर्णांक bmap_flags, blk_t block,
		      blk_t *phys_blk)
{
	errcode_t ret;
	blk64_t	ret_blk = *phys_blk;

	ret = ext2fs_bmap2(fs, ino, inode, block_buf, bmap_flags, block,
			    0, &ret_blk);
	जर (ret)
		परत ret;
	जर (ret_blk >= ((दीर्घ दीर्घ) 1 << 32))
		परत EOVERFLOW;
	*phys_blk = ret_blk;
	परत 0;
}
