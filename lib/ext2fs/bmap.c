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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <errno.h>

#समावेश "ext2_fs.h"
#समावेश "ext2fsP.h"

#अगर defined(__GNUC__) && !defined(NO_INLINE_FUNCS)
#घोषणा _BMAP_INLINE_	__inline__
#अन्यथा
#घोषणा _BMAP_INLINE_
#पूर्ण_अगर

बाह्य errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     काष्ठा ext2_inode *inode,
			     अक्षर *block_buf, पूर्णांक bmap_flags,
			     blk_t block, blk_t *phys_blk);

#घोषणा inode_bmap(inode, nr) ((inode)->i_block[(nr)])

अटल _BMAP_INLINE_ errcode_t block_ind_bmap(ext2_filsys fs, पूर्णांक flags,
					      blk_t ind, अक्षर *block_buf,
					      पूर्णांक *blocks_alloc,
					      blk_t nr, blk_t *ret_blk)
{
	errcode_t	retval;
	blk_t		b;

	अगर (!ind) {
		अगर (flags & BMAP_SET)
			वापस EXT2_ET_SET_BMAP_NO_IND;
		*ret_blk = 0;
		वापस 0;
	}
	retval = io_channel_read_blk(fs->io, ind, 1, block_buf);
	अगर (retval)
		वापस retval;

	अगर (flags & BMAP_SET) {
		b = *ret_blk;
#अगर_घोषित शब्दगण_BIGENDIAN
		b = ext2fs_swab32(b);
#पूर्ण_अगर
		((blk_t *) block_buf)[nr] = b;
		वापस io_channel_write_blk(fs->io, ind, 1, block_buf);
	}

	b = ((blk_t *) block_buf)[nr];

#अगर_घोषित शब्दगण_BIGENDIAN
	b = ext2fs_swab32(b);
#पूर्ण_अगर

	अगर (!b && (flags & BMAP_ALLOC)) {
		b = nr ? ext2fs_le32_to_cpu(((blk_t *)block_buf)[nr - 1]) : ind;
		retval = ext2fs_alloc_block(fs, b,
					    block_buf + fs->blocksize, &b);
		अगर (retval)
			वापस retval;

#अगर_घोषित शब्दगण_BIGENDIAN
		((blk_t *) block_buf)[nr] = ext2fs_swab32(b);
#अन्यथा
		((blk_t *) block_buf)[nr] = b;
#पूर्ण_अगर

		retval = io_channel_write_blk(fs->io, ind, 1, block_buf);
		अगर (retval)
			वापस retval;

		(*blocks_alloc)++;
	}

	*ret_blk = b;
	वापस 0;
}

अटल _BMAP_INLINE_ errcode_t block_dind_bmap(ext2_filsys fs, पूर्णांक flags,
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
	अगर (retval)
		वापस retval;
	retval = block_ind_bmap(fs, flags, b, block_buf, blocks_alloc,
				nr % addr_per_block, ret_blk);
	वापस retval;
}

अटल _BMAP_INLINE_ errcode_t block_tind_bmap(ext2_filsys fs, पूर्णांक flags,
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
	अगर (retval)
		वापस retval;
	retval = block_ind_bmap(fs, flags, b, block_buf, blocks_alloc,
				nr % addr_per_block, ret_blk);
	वापस retval;
}

अटल errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     काष्ठा ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     अक्षर *block_buf, पूर्णांक bmap_flags, blk64_t block,
			     पूर्णांक *ret_flags, पूर्णांक *blocks_alloc,
			     blk64_t *phys_blk);

अटल errcode_t implied_cluster_alloc(ext2_filsys fs, ext2_ino_t ino,
				       काष्ठा ext2_inode *inode,
				       ext2_extent_handle_t handle,
				       blk64_t lblk, blk64_t *phys_blk)
{
	blk64_t	base_block, pblock = 0;
	पूर्णांक i;

	अगर (!ext2fs_has_feature_bigalloc(fs->super))
		वापस 0;

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
	क्रम (i = 0; i < EXT2FS_CLUSTER_RATIO(fs); i++) {
		अगर (base_block + i == lblk)
			जारी;
		extent_bmap(fs, ino, inode, handle, 0, 0,
			    base_block + i, 0, 0, &pblock);
		अगर (pblock)
			अवरोध;
	}
	अगर (pblock == 0)
		वापस 0;
	*phys_blk = pblock - i + (lblk - base_block);
	वापस 0;
}

/* Try to map a logical block to an already-allocated physical cluster. */
errcode_t ext2fs_map_cluster_block(ext2_filsys fs, ext2_ino_t ino,
				   काष्ठा ext2_inode *inode, blk64_t lblk,
				   blk64_t *pblk)
{
	ext2_extent_handle_t handle;
	errcode_t retval;

	/* Need bigalloc and extents to be enabled */
	*pblk = 0;
	अगर (!ext2fs_has_feature_bigalloc(fs->super) ||
	    !(inode->i_flags & EXT4_EXTENTS_FL))
		वापस 0;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	अगर (retval)
		जाओ out;

	retval = implied_cluster_alloc(fs, ino, inode, handle, lblk, pblk);
	अगर (retval)
		जाओ out2;

out2:
	ext2fs_extent_free(handle);
out:
	वापस retval;
}

अटल errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     काष्ठा ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     अक्षर *block_buf, पूर्णांक bmap_flags, blk64_t block,
			     पूर्णांक *ret_flags, पूर्णांक *blocks_alloc,
			     blk64_t *phys_blk)
{
	काष्ठा blk_alloc_ctx	alloc_ctx;
	काष्ठा ext2fs_extent	extent;
	अचिन्हित पूर्णांक		offset;
	errcode_t		retval = 0;
	blk64_t			blk64 = 0;
	पूर्णांक			alloc = 0;
	पूर्णांक			set_flags;

	set_flags = bmap_flags & BMAP_UNINIT ? EXT2_EXTENT_SET_BMAP_UNINIT : 0;

	अगर (bmap_flags & BMAP_SET) {
		retval = ext2fs_extent_set_bmap(handle, block,
						*phys_blk, set_flags);
		वापस retval;
	}
	retval = ext2fs_extent_goto(handle, block);
	अगर (retval) {
		/* If the extent is not found, return phys_blk = 0 */
		अगर (retval == EXT2_ET_EXTENT_NOT_FOUND) {
			extent.e_lblk = block;
			जाओ got_block;
		}
		वापस retval;
	}
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	अगर (retval)
		वापस retval;
	offset = block - extent.e_lblk;
	अगर (block >= extent.e_lblk && (offset <= extent.e_len)) {
		*phys_blk = extent.e_pblk + offset;
		अगर (ret_flags && extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			*ret_flags |= BMAP_RET_UNINIT;
	}
got_block:
	अगर ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
		implied_cluster_alloc(fs, ino, inode, handle, block, &blk64);
		अगर (blk64)
			जाओ set_extent;
		retval = extent_bmap(fs, ino, inode, handle, block_buf,
				     0, block-1, 0, blocks_alloc, &blk64);
		अगर (retval)
			blk64 = ext2fs_find_inode_goal(fs, ino, inode, block);
		alloc_ctx.ino = ino;
		alloc_ctx.inode = inode;
		alloc_ctx.lblk = extent.e_lblk;
		alloc_ctx.flags = BLOCK_ALLOC_तथ्य;
		retval = ext2fs_alloc_block3(fs, blk64, block_buf, &blk64,
					     &alloc_ctx);
		अगर (retval)
			वापस retval;
		blk64 &= ~EXT2FS_CLUSTER_MASK(fs);
		blk64 += EXT2FS_CLUSTER_MASK(fs) & block;
		alloc++;
	set_extent:
		retval = ext2fs_extent_set_bmap(handle, block,
						blk64, set_flags);
		अगर (retval) {
			ext2fs_block_alloc_stats2(fs, blk64, -1);
			वापस retval;
		}
		/* Update inode after setting extent */
		retval = ext2fs_read_inode(fs, ino, inode);
		अगर (retval)
			वापस retval;
		*blocks_alloc += alloc;
		*phys_blk = blk64;
	}
	वापस 0;
}

पूर्णांक ext2fs_file_block_offset_too_big(ext2_filsys fs,
				     काष्ठा ext2_inode *inode,
				     blk64_t offset)
{
	blk64_t addr_per_block, max_map_block;

	/* Kernel seems to cut us off at 4294967294 blocks */
	अगर (offset >= (1ULL << 32) - 1)
		वापस 1;

	अगर (inode->i_flags & EXT4_EXTENTS_FL)
		वापस 0;

	addr_per_block = fs->blocksize >> 2;
	max_map_block = addr_per_block;
	max_map_block += addr_per_block * addr_per_block;
	max_map_block += addr_per_block * addr_per_block * addr_per_block;
	max_map_block += 12;

	वापस offset >= max_map_block;
}

errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino, काष्ठा ext2_inode *inode,
		       अक्षर *block_buf, पूर्णांक bmap_flags, blk64_t block,
		       पूर्णांक *ret_flags, blk64_t *phys_blk)
{
	काष्ठा ext2_inode inode_buf;
	ext2_extent_handle_t handle = 0;
	blk_t addr_per_block;
	blk_t	b, blk32;
	blk64_t b64;
	अक्षर	*buf = 0;
	errcode_t	retval = 0;
	पूर्णांक		blocks_alloc = 0, inode_dirty = 0;
	काष्ठा blk_alloc_ctx alloc_ctx = {
		.ino	= ino,
		.inode	= inode,
		.lblk	= 0,
		.flags	= BLOCK_ALLOC_तथ्य,
	};

	अगर (!(bmap_flags & BMAP_SET))
		*phys_blk = 0;

	अगर (ret_flags)
		*ret_flags = 0;

	/* Read inode structure if necessary */
	अगर (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		अगर (retval)
			वापस retval;
		inode = &inode_buf;
	}
	addr_per_block = (blk_t) fs->blocksize >> 2;

	अगर (ext2fs_file_block_offset_too_big(fs, inode, block))
		वापस EXT2_ET_FILE_TOO_BIG;

	/*
	 * If an inode has inline data, that means that it doesn't have
	 * any blocks and we shouldn't map any blocks for it.
	 */
	अगर (inode->i_flags & EXT4_INLINE_तथ्य_FL)
		वापस EXT2_ET_INLINE_तथ्य_NO_BLOCK;

	अगर (!block_buf) {
		retval = ext2fs_get_array(2, fs->blocksize, &buf);
		अगर (retval)
			वापस retval;
		block_buf = buf;
	}

	अगर (inode->i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		अगर (retval)
			जाओ done;
		retval = extent_bmap(fs, ino, inode, handle, block_buf,
				     bmap_flags, block, ret_flags,
				     &blocks_alloc, phys_blk);
		जाओ done;
	}

	अगर (block < EXT2_NDIR_BLOCKS) {
		अगर (bmap_flags & BMAP_SET) {
			b = *phys_blk;
			inode_bmap(inode, block) = b;
			inode_dirty++;
			जाओ done;
		}

		*phys_blk = inode_bmap(inode, block);
		b = block ? inode_bmap(inode, block - 1) :
			    ext2fs_find_inode_goal(fs, ino, inode, block);

		अगर ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
						     &alloc_ctx);
			b = b64;
			अगर (retval)
				जाओ done;
			inode_bmap(inode, block) = b;
			blocks_alloc++;
			*phys_blk = b;
		}
		जाओ done;
	}

	/* Indirect block */
	block -= EXT2_NDIR_BLOCKS;
	blk32 = *phys_blk;
	अगर (block < addr_per_block) {
		b = inode_bmap(inode, EXT2_IND_BLOCK);
		अगर (!b) {
			अगर (!(bmap_flags & BMAP_ALLOC)) {
				अगर (bmap_flags & BMAP_SET)
					retval = EXT2_ET_SET_BMAP_NO_IND;
				जाओ done;
			}

			b = inode_bmap(inode, EXT2_IND_BLOCK-1);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
						     &alloc_ctx);
			b = b64;
			अगर (retval)
				जाओ done;
			inode_bmap(inode, EXT2_IND_BLOCK) = b;
			blocks_alloc++;
		}
		retval = block_ind_bmap(fs, bmap_flags, b, block_buf,
					&blocks_alloc, block, &blk32);
		अगर (retval == 0)
			*phys_blk = blk32;
		जाओ done;
	}

	/* Doubly indirect block  */
	block -= addr_per_block;
	अगर (block < addr_per_block * addr_per_block) {
		b = inode_bmap(inode, EXT2_DIND_BLOCK);
		अगर (!b) {
			अगर (!(bmap_flags & BMAP_ALLOC)) {
				अगर (bmap_flags & BMAP_SET)
					retval = EXT2_ET_SET_BMAP_NO_IND;
				जाओ done;
			}

			b = inode_bmap(inode, EXT2_IND_BLOCK);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
						     &alloc_ctx);
			b = b64;
			अगर (retval)
				जाओ done;
			inode_bmap(inode, EXT2_DIND_BLOCK) = b;
			blocks_alloc++;
		}
		retval = block_dind_bmap(fs, bmap_flags, b, block_buf,
					 &blocks_alloc, block, &blk32);
		अगर (retval == 0)
			*phys_blk = blk32;
		जाओ done;
	}

	/* Triply indirect block */
	block -= addr_per_block * addr_per_block;
	b = inode_bmap(inode, EXT2_TIND_BLOCK);
	अगर (!b) {
		अगर (!(bmap_flags & BMAP_ALLOC)) {
			अगर (bmap_flags & BMAP_SET)
				retval = EXT2_ET_SET_BMAP_NO_IND;
			जाओ done;
		}

		b = inode_bmap(inode, EXT2_DIND_BLOCK);
		b64 = b;
		retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
					     &alloc_ctx);
		b = b64;
		अगर (retval)
			जाओ done;
		inode_bmap(inode, EXT2_TIND_BLOCK) = b;
		blocks_alloc++;
	}
	retval = block_tind_bmap(fs, bmap_flags, b, block_buf,
				 &blocks_alloc, block, &blk32);
	अगर (retval == 0)
		*phys_blk = blk32;
done:
	अगर (*phys_blk && retval == 0 && (bmap_flags & BMAP_ZERO))
		retval = ext2fs_zero_blocks2(fs, *phys_blk, 1, शून्य, शून्य);
	अगर (buf)
		ext2fs_free_mem(&buf);
	अगर (handle)
		ext2fs_extent_free(handle);
	अगर ((retval == 0) && (blocks_alloc || inode_dirty)) {
		ext2fs_iblk_add_blocks(fs, inode, blocks_alloc);
		retval = ext2fs_write_inode(fs, ino, inode);
	}
	वापस retval;
}

errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino, काष्ठा ext2_inode *inode,
		      अक्षर *block_buf, पूर्णांक bmap_flags, blk_t block,
		      blk_t *phys_blk)
{
	errcode_t ret;
	blk64_t	ret_blk = *phys_blk;

	ret = ext2fs_bmap2(fs, ino, inode, block_buf, bmap_flags, block,
			    0, &ret_blk);
	अगर (ret)
		वापस ret;
	अगर (ret_blk >= ((दीर्घ दीर्घ) 1 << 32))
		वापस EOVERFLOW;
	*phys_blk = ret_blk;
	वापस 0;
}
