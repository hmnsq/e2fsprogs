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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <errno.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

#यदि defined(__GNUC__) && !defined(NO_INLINE_FUNCS)
#define _BMAP_INLINE_	__inline__
#अन्यथा
#define _BMAP_INLINE_
#endif

बाह्य errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     संरचना ext2_inode *inode,
			     वर्ण *block_buf, पूर्णाङ्क bmap_flags,
			     blk_t block, blk_t *phys_blk);

#define inode_bmap(inode, nr) ((inode)->i_block[(nr)])

स्थिर _BMAP_INLINE_ errcode_t block_ind_bmap(ext2_filsys fs, पूर्णाङ्क flags,
					      blk_t ind, वर्ण *block_buf,
					      पूर्णाङ्क *blocks_alloc,
					      blk_t nr, blk_t *ret_blk)
{
	errcode_t	retval;
	blk_t		b;

	यदि (!ind) {
		यदि (flags & BMAP_SET)
			प्रत्यागम EXT2_ET_SET_BMAP_NO_IND;
		*ret_blk = 0;
		प्रत्यागम 0;
	}
	retval = io_channel_read_blk(fs->io, ind, 1, block_buf);
	यदि (retval)
		प्रत्यागम retval;

	यदि (flags & BMAP_SET) {
		b = *ret_blk;
#ifdef शब्दगण_BIGENDIAN
		b = ext2fs_swab32(b);
#endif
		((blk_t *) block_buf)[nr] = b;
		प्रत्यागम io_channel_write_blk(fs->io, ind, 1, block_buf);
	}

	b = ((blk_t *) block_buf)[nr];

#ifdef शब्दगण_BIGENDIAN
	b = ext2fs_swab32(b);
#endif

	यदि (!b && (flags & BMAP_ALLOC)) {
		b = nr ? ext2fs_le32_to_cpu(((blk_t *)block_buf)[nr - 1]) : ind;
		retval = ext2fs_alloc_block(fs, b,
					    block_buf + fs->blocksize, &b);
		यदि (retval)
			प्रत्यागम retval;

#ifdef शब्दगण_BIGENDIAN
		((blk_t *) block_buf)[nr] = ext2fs_swab32(b);
#अन्यथा
		((blk_t *) block_buf)[nr] = b;
#endif

		retval = io_channel_write_blk(fs->io, ind, 1, block_buf);
		यदि (retval)
			प्रत्यागम retval;

		(*blocks_alloc)++;
	}

	*ret_blk = b;
	प्रत्यागम 0;
}

स्थिर _BMAP_INLINE_ errcode_t block_dind_bmap(ext2_filsys fs, पूर्णाङ्क flags,
					       blk_t dind, वर्ण *block_buf,
					       पूर्णाङ्क *blocks_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_per_block;

	addr_per_block = (blk_t) fs->blocksize >> 2;

	retval = block_ind_bmap(fs, flags & ~BMAP_SET, dind, block_buf,
				blocks_alloc, nr / addr_per_block, &b);
	यदि (retval)
		प्रत्यागम retval;
	retval = block_ind_bmap(fs, flags, b, block_buf, blocks_alloc,
				nr % addr_per_block, ret_blk);
	प्रत्यागम retval;
}

स्थिर _BMAP_INLINE_ errcode_t block_tind_bmap(ext2_filsys fs, पूर्णाङ्क flags,
					       blk_t tind, वर्ण *block_buf,
					       पूर्णाङ्क *blocks_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_per_block;

	addr_per_block = (blk_t) fs->blocksize >> 2;

	retval = block_dind_bmap(fs, flags & ~BMAP_SET, tind, block_buf,
				 blocks_alloc, nr / addr_per_block, &b);
	यदि (retval)
		प्रत्यागम retval;
	retval = block_ind_bmap(fs, flags, b, block_buf, blocks_alloc,
				nr % addr_per_block, ret_blk);
	प्रत्यागम retval;
}

स्थिर errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     संरचना ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     वर्ण *block_buf, पूर्णाङ्क bmap_flags, blk64_t block,
			     पूर्णाङ्क *ret_flags, पूर्णाङ्क *blocks_alloc,
			     blk64_t *phys_blk);

स्थिर errcode_t implied_cluster_alloc(ext2_filsys fs, ext2_ino_t ino,
				       संरचना ext2_inode *inode,
				       ext2_extent_handle_t handle,
				       blk64_t lblk, blk64_t *phys_blk)
{
	blk64_t	base_block, pblock = 0;
	पूर्णाङ्क i;

	यदि (!ext2fs_has_feature_bigalloc(fs->super))
		प्रत्यागम 0;

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
	प्रति (i = 0; i < EXT2FS_CLUSTER_RATIO(fs); i++) {
		यदि (base_block + i == lblk)
			अनुवर्त;
		extent_bmap(fs, ino, inode, handle, 0, 0,
			    base_block + i, 0, 0, &pblock);
		यदि (pblock)
			भङ्ग;
	}
	यदि (pblock == 0)
		प्रत्यागम 0;
	*phys_blk = pblock - i + (lblk - base_block);
	प्रत्यागम 0;
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
	यदि (!ext2fs_has_feature_bigalloc(fs->super) ||
	    !(inode->i_flags & EXT4_EXTENTS_FL))
		प्रत्यागम 0;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	यदि (retval)
		गच्छ out;

	retval = implied_cluster_alloc(fs, ino, inode, handle, lblk, pblk);
	यदि (retval)
		गच्छ out2;

out2:
	ext2fs_extent_free(handle);
out:
	प्रत्यागम retval;
}

स्थिर errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     संरचना ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     वर्ण *block_buf, पूर्णाङ्क bmap_flags, blk64_t block,
			     पूर्णाङ्क *ret_flags, पूर्णाङ्क *blocks_alloc,
			     blk64_t *phys_blk)
{
	संरचना blk_alloc_ctx	alloc_ctx;
	संरचना ext2fs_extent	extent;
	अचिन्हित पूर्णाङ्क		offset;
	errcode_t		retval = 0;
	blk64_t			blk64 = 0;
	पूर्णाङ्क			alloc = 0;
	पूर्णाङ्क			set_flags;

	set_flags = bmap_flags & BMAP_UNINIT ? EXT2_EXTENT_SET_BMAP_UNINIT : 0;

	यदि (bmap_flags & BMAP_SET) {
		retval = ext2fs_extent_set_bmap(handle, block,
						*phys_blk, set_flags);
		प्रत्यागम retval;
	}
	retval = ext2fs_extent_goto(handle, block);
	यदि (retval) {
		/* If the extent is not found, return phys_blk = 0 */
		यदि (retval == EXT2_ET_EXTENT_NOT_FOUND) {
			extent.e_lblk = block;
			गच्छ got_block;
		}
		प्रत्यागम retval;
	}
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	यदि (retval)
		प्रत्यागम retval;
	offset = block - extent.e_lblk;
	यदि (block >= extent.e_lblk && (offset <= extent.e_len)) {
		*phys_blk = extent.e_pblk + offset;
		यदि (ret_flags && extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			*ret_flags |= BMAP_RET_UNINIT;
	}
got_block:
	यदि ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
		implied_cluster_alloc(fs, ino, inode, handle, block, &blk64);
		यदि (blk64)
			गच्छ set_extent;
		retval = extent_bmap(fs, ino, inode, handle, block_buf,
				     0, block-1, 0, blocks_alloc, &blk64);
		यदि (retval)
			blk64 = ext2fs_find_inode_goal(fs, ino, inode, block);
		alloc_ctx.ino = ino;
		alloc_ctx.inode = inode;
		alloc_ctx.lblk = extent.e_lblk;
		alloc_ctx.flags = BLOCK_ALLOC_DATA;
		retval = ext2fs_alloc_block3(fs, blk64, block_buf, &blk64,
					     &alloc_ctx);
		यदि (retval)
			प्रत्यागम retval;
		blk64 &= ~EXT2FS_CLUSTER_MASK(fs);
		blk64 += EXT2FS_CLUSTER_MASK(fs) & block;
		alloc++;
	set_extent:
		retval = ext2fs_extent_set_bmap(handle, block,
						blk64, set_flags);
		यदि (retval) {
			ext2fs_block_alloc_stats2(fs, blk64, -1);
			प्रत्यागम retval;
		}
		/* Update inode after setting extent */
		retval = ext2fs_read_inode(fs, ino, inode);
		यदि (retval)
			प्रत्यागम retval;
		*blocks_alloc += alloc;
		*phys_blk = blk64;
	}
	प्रत्यागम 0;
}

पूर्णाङ्क ext2fs_file_block_offset_too_big(ext2_filsys fs,
				     संरचना ext2_inode *inode,
				     blk64_t offset)
{
	blk64_t addr_per_block, max_map_block;

	/* Kernel seems to cut us off at 4294967294 blocks */
	यदि (offset >= (1ULL << 32) - 1)
		प्रत्यागम 1;

	यदि (inode->i_flags & EXT4_EXTENTS_FL)
		प्रत्यागम 0;

	addr_per_block = fs->blocksize >> 2;
	max_map_block = addr_per_block;
	max_map_block += addr_per_block * addr_per_block;
	max_map_block += addr_per_block * addr_per_block * addr_per_block;
	max_map_block += 12;

	प्रत्यागम offset >= max_map_block;
}

errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino, संरचना ext2_inode *inode,
		       वर्ण *block_buf, पूर्णाङ्क bmap_flags, blk64_t block,
		       पूर्णाङ्क *ret_flags, blk64_t *phys_blk)
{
	संरचना ext2_inode inode_buf;
	ext2_extent_handle_t handle = 0;
	blk_t addr_per_block;
	blk_t	b, blk32;
	blk64_t b64;
	वर्ण	*buf = 0;
	errcode_t	retval = 0;
	पूर्णाङ्क		blocks_alloc = 0, inode_dirty = 0;
	संरचना blk_alloc_ctx alloc_ctx = {
		.ino	= ino,
		.inode	= inode,
		.lblk	= 0,
		.flags	= BLOCK_ALLOC_DATA,
	};

	यदि (!(bmap_flags & BMAP_SET))
		*phys_blk = 0;

	यदि (ret_flags)
		*ret_flags = 0;

	/* Read inode structure if necessary */
	यदि (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		यदि (retval)
			प्रत्यागम retval;
		inode = &inode_buf;
	}
	addr_per_block = (blk_t) fs->blocksize >> 2;

	यदि (ext2fs_file_block_offset_too_big(fs, inode, block))
		प्रत्यागम EXT2_ET_FILE_TOO_BIG;

	/*
	 * If an inode has inline data, that means that it doesn't have
	 * any blocks and we shouldn't map any blocks for it.
	 */
	यदि (inode->i_flags & EXT4_INLINE_DATA_FL)
		प्रत्यागम EXT2_ET_INLINE_DATA_NO_BLOCK;

	यदि (!block_buf) {
		retval = ext2fs_get_array(2, fs->blocksize, &buf);
		यदि (retval)
			प्रत्यागम retval;
		block_buf = buf;
	}

	यदि (inode->i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		यदि (retval)
			गच्छ done;
		retval = extent_bmap(fs, ino, inode, handle, block_buf,
				     bmap_flags, block, ret_flags,
				     &blocks_alloc, phys_blk);
		गच्छ done;
	}

	यदि (block < EXT2_NDIR_BLOCKS) {
		यदि (bmap_flags & BMAP_SET) {
			b = *phys_blk;
			inode_bmap(inode, block) = b;
			inode_dirty++;
			गच्छ done;
		}

		*phys_blk = inode_bmap(inode, block);
		b = block ? inode_bmap(inode, block - 1) :
			    ext2fs_find_inode_goal(fs, ino, inode, block);

		यदि ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
						     &alloc_ctx);
			b = b64;
			यदि (retval)
				गच्छ done;
			inode_bmap(inode, block) = b;
			blocks_alloc++;
			*phys_blk = b;
		}
		गच्छ done;
	}

	/* Indirect block */
	block -= EXT2_NDIR_BLOCKS;
	blk32 = *phys_blk;
	यदि (block < addr_per_block) {
		b = inode_bmap(inode, EXT2_IND_BLOCK);
		यदि (!b) {
			यदि (!(bmap_flags & BMAP_ALLOC)) {
				यदि (bmap_flags & BMAP_SET)
					retval = EXT2_ET_SET_BMAP_NO_IND;
				गच्छ done;
			}

			b = inode_bmap(inode, EXT2_IND_BLOCK-1);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
						     &alloc_ctx);
			b = b64;
			यदि (retval)
				गच्छ done;
			inode_bmap(inode, EXT2_IND_BLOCK) = b;
			blocks_alloc++;
		}
		retval = block_ind_bmap(fs, bmap_flags, b, block_buf,
					&blocks_alloc, block, &blk32);
		यदि (retval == 0)
			*phys_blk = blk32;
		गच्छ done;
	}

	/* Doubly indirect block  */
	block -= addr_per_block;
	यदि (block < addr_per_block * addr_per_block) {
		b = inode_bmap(inode, EXT2_DIND_BLOCK);
		यदि (!b) {
			यदि (!(bmap_flags & BMAP_ALLOC)) {
				यदि (bmap_flags & BMAP_SET)
					retval = EXT2_ET_SET_BMAP_NO_IND;
				गच्छ done;
			}

			b = inode_bmap(inode, EXT2_IND_BLOCK);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
						     &alloc_ctx);
			b = b64;
			यदि (retval)
				गच्छ done;
			inode_bmap(inode, EXT2_DIND_BLOCK) = b;
			blocks_alloc++;
		}
		retval = block_dind_bmap(fs, bmap_flags, b, block_buf,
					 &blocks_alloc, block, &blk32);
		यदि (retval == 0)
			*phys_blk = blk32;
		गच्छ done;
	}

	/* Triply indirect block */
	block -= addr_per_block * addr_per_block;
	b = inode_bmap(inode, EXT2_TIND_BLOCK);
	यदि (!b) {
		यदि (!(bmap_flags & BMAP_ALLOC)) {
			यदि (bmap_flags & BMAP_SET)
				retval = EXT2_ET_SET_BMAP_NO_IND;
			गच्छ done;
		}

		b = inode_bmap(inode, EXT2_DIND_BLOCK);
		b64 = b;
		retval = ext2fs_alloc_block3(fs, b64, block_buf, &b64,
					     &alloc_ctx);
		b = b64;
		यदि (retval)
			गच्छ done;
		inode_bmap(inode, EXT2_TIND_BLOCK) = b;
		blocks_alloc++;
	}
	retval = block_tind_bmap(fs, bmap_flags, b, block_buf,
				 &blocks_alloc, block, &blk32);
	यदि (retval == 0)
		*phys_blk = blk32;
done:
	यदि (*phys_blk && retval == 0 && (bmap_flags & BMAP_ZERO))
		retval = ext2fs_zero_blocks2(fs, *phys_blk, 1, रिक्त, रिक्त);
	यदि (buf)
		ext2fs_free_mem(&buf);
	यदि (handle)
		ext2fs_extent_free(handle);
	यदि ((retval == 0) && (blocks_alloc || inode_dirty)) {
		ext2fs_iblk_add_blocks(fs, inode, blocks_alloc);
		retval = ext2fs_write_inode(fs, ino, inode);
	}
	प्रत्यागम retval;
}

errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino, संरचना ext2_inode *inode,
		      वर्ण *block_buf, पूर्णाङ्क bmap_flags, blk_t block,
		      blk_t *phys_blk)
{
	errcode_t ret;
	blk64_t	ret_blk = *phys_blk;

	ret = ext2fs_bmap2(fs, ino, inode, block_buf, bmap_flags, block,
			    0, &ret_blk);
	यदि (ret)
		प्रत्यागम ret;
	यदि (ret_blk >= ((दीर्घ दीर्घ) 1 << 32))
		प्रत्यागम EOVERFLOW;
	*phys_blk = ret_blk;
	प्रत्यागम 0;
}
