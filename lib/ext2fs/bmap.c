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
#यदि_अन्तः
#अन्तर्भाव <errno.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

#यदि defined(__GNUC__) && !defined(NO_INLINE_FUNCS)
#परिभाषय _BMAP_INLINE_	__inline__
#अन्यथा
#परिभाषय _BMAP_INLINE_
#यदि_अन्तः

बाह्य errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     संरचना ext2_inode *inode,
			     वर्ण *खण्डम्_buf, पूर्णाङ्क bmap_flags,
			     blk_t खण्डम्, blk_t *phys_blk);

#परिभाषय inode_bmap(inode, nr) ((inode)->i_खण्डम्[(nr)])

स्थिर _BMAP_INLINE_ errcode_t खण्डम्_ind_bmap(ext2_filsys fs, पूर्णाङ्क flags,
					      blk_t ind, वर्ण *खण्डम्_buf,
					      पूर्णाङ्क *खण्डानि_alloc,
					      blk_t nr, blk_t *ret_blk)
{
	errcode_t	retval;
	blk_t		b;

	यदि (!ind) {
		यदि (flags & BMAP_समुच्चयः)
			प्रत्यागम EXT2_ET_समुच्चयः_BMAP_NO_IND;
		*ret_blk = 0;
		प्रत्यागम 0;
	}
	retval = io_channel_read_blk(fs->io, ind, 1, खण्डम्_buf);
	यदि (retval)
		प्रत्यागम retval;

	यदि (flags & BMAP_समुच्चयः) {
		b = *ret_blk;
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
		b = ext2fs_swab32(b);
#यदि_अन्तः
		((blk_t *) खण्डम्_buf)[nr] = b;
		प्रत्यागम io_channel_write_blk(fs->io, ind, 1, खण्डम्_buf);
	}

	b = ((blk_t *) खण्डम्_buf)[nr];

#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	b = ext2fs_swab32(b);
#यदि_अन्तः

	यदि (!b && (flags & BMAP_ALLOC)) {
		b = nr ? ext2fs_le32_गन्तव्यम्_cpu(((blk_t *)खण्डम्_buf)[nr - 1]) : ind;
		retval = ext2fs_alloc_खण्डम्(fs, b,
					    खण्डम्_buf + fs->blocksize, &b);
		यदि (retval)
			प्रत्यागम retval;

#यदि_परिभाषितम् शब्दाः_BIGENDIAN
		((blk_t *) खण्डम्_buf)[nr] = ext2fs_swab32(b);
#अन्यथा
		((blk_t *) खण्डम्_buf)[nr] = b;
#यदि_अन्तः

		retval = io_channel_write_blk(fs->io, ind, 1, खण्डम्_buf);
		यदि (retval)
			प्रत्यागम retval;

		(*खण्डानि_alloc)++;
	}

	*ret_blk = b;
	प्रत्यागम 0;
}

स्थिर _BMAP_INLINE_ errcode_t खण्डम्_dind_bmap(ext2_filsys fs, पूर्णाङ्क flags,
					       blk_t dind, वर्ण *खण्डम्_buf,
					       पूर्णाङ्क *खण्डानि_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_प्रत्येकम्_खण्डम्;

	addr_प्रत्येकम्_खण्डम् = (blk_t) fs->blocksize >> 2;

	retval = खण्डम्_ind_bmap(fs, flags & ~BMAP_समुच्चयः, dind, खण्डम्_buf,
				खण्डानि_alloc, nr / addr_प्रत्येकम्_खण्डम्, &b);
	यदि (retval)
		प्रत्यागम retval;
	retval = खण्डम्_ind_bmap(fs, flags, b, खण्डम्_buf, खण्डानि_alloc,
				nr % addr_प्रत्येकम्_खण्डम्, ret_blk);
	प्रत्यागम retval;
}

स्थिर _BMAP_INLINE_ errcode_t खण्डम्_tind_bmap(ext2_filsys fs, पूर्णाङ्क flags,
					       blk_t tind, वर्ण *खण्डम्_buf,
					       पूर्णाङ्क *खण्डानि_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_प्रत्येकम्_खण्डम्;

	addr_प्रत्येकम्_खण्डम् = (blk_t) fs->blocksize >> 2;

	retval = खण्डम्_dind_bmap(fs, flags & ~BMAP_समुच्चयः, tind, खण्डम्_buf,
				 खण्डानि_alloc, nr / addr_प्रत्येकम्_खण्डम्, &b);
	यदि (retval)
		प्रत्यागम retval;
	retval = खण्डम्_ind_bmap(fs, flags, b, खण्डम्_buf, खण्डानि_alloc,
				nr % addr_प्रत्येकम्_खण्डम्, ret_blk);
	प्रत्यागम retval;
}

स्थिर errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     संरचना ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     वर्ण *खण्डम्_buf, पूर्णाङ्क bmap_flags, blk64_t खण्डम्,
			     पूर्णाङ्क *ret_flags, पूर्णाङ्क *खण्डानि_alloc,
			     blk64_t *phys_blk);

स्थिर errcode_t implied_cluster_alloc(ext2_filsys fs, ext2_ino_t ino,
				       संरचना ext2_inode *inode,
				       ext2_extent_handle_t handle,
				       blk64_t lblk, blk64_t *phys_blk)
{
	blk64_t	आधारः_खण्डम्, pblock = 0;
	पूर्णाङ्क i;

	यदि (!ext2fs_has_feature_bigalloc(fs->super))
		प्रत्यागम 0;

	आधारः_खण्डम् = lblk & ~EXT2_FS_CLUSTER_आवरणम्(fs);
	/*
	 * Except for the logical block (lblk) that was passed in, search all
	 * blocks in this logical cluster for a mapping to a physical cluster.
	 * If any such map exists, calculate the physical block that maps to
	 * the logical block and return that.
	 *
	 * The old code wouldn't even look if (block % cluster_ratio) == 0;
	 * this is incorrect if we're allocating blocks in reverse order.
	 */
	प्रति (i = 0; i < EXT2_FS_CLUSTER_अनुपातः(fs); i++) {
		यदि (आधारः_खण्डम् + i == lblk)
			अनुवर्त;
		extent_bmap(fs, ino, inode, handle, 0, 0,
			    आधारः_खण्डम् + i, 0, 0, &pblock);
		यदि (pblock)
			भङ्ग;
	}
	यदि (pblock == 0)
		प्रत्यागम 0;
	*phys_blk = pblock - i + (lblk - आधारः_खण्डम्);
	प्रत्यागम 0;
}

/* Try to map a logical block to an already-allocated physical cluster. */
errcode_t ext2fs_map_cluster_खण्डम्(ext2_filsys fs, ext2_ino_t ino,
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
		गच्छ बहिर्गमनम्;

	retval = implied_cluster_alloc(fs, ino, inode, handle, lblk, pblk);
	यदि (retval)
		गच्छ out2;

out2:
	ext2fs_extent_free(handle);
बहिर्गमनम्:
	प्रत्यागम retval;
}

स्थिर errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     संरचना ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     वर्ण *खण्डम्_buf, पूर्णाङ्क bmap_flags, blk64_t खण्डम्,
			     पूर्णाङ्क *ret_flags, पूर्णाङ्क *खण्डानि_alloc,
			     blk64_t *phys_blk)
{
	संरचना blk_alloc_ctx	alloc_ctx;
	संरचना ext2fs_extent	extent;
	अचिह्नित पूर्णाङ्क		विस्थापनम्;
	errcode_t		retval = 0;
	blk64_t			blk64 = 0;
	पूर्णाङ्क			alloc = 0;
	पूर्णाङ्क			समुच्चयः_flags;

	समुच्चयः_flags = bmap_flags & BMAP_UNINIT ? EXT2_EXTENT_समुच्चयः_BMAP_UNINIT : 0;

	यदि (bmap_flags & BMAP_समुच्चयः) {
		retval = ext2fs_extent_समुच्चयः_bmap(handle, खण्डम्,
						*phys_blk, समुच्चयः_flags);
		प्रत्यागम retval;
	}
	retval = ext2fs_extent_goto(handle, खण्डम्);
	यदि (retval) {
		/* If the extent is not found, return phys_blk = 0 */
		यदि (retval == EXT2_ET_EXTENT_NOT_प्राप्तम्) {
			extent.e_lblk = खण्डम्;
			गच्छ got_खण्डम्;
		}
		प्रत्यागम retval;
	}
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	यदि (retval)
		प्रत्यागम retval;
	विस्थापनम् = खण्डम् - extent.e_lblk;
	यदि (खण्डम् >= extent.e_lblk && (विस्थापनम् <= extent.e_len)) {
		*phys_blk = extent.e_pblk + विस्थापनम्;
		यदि (ret_flags && extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			*ret_flags |= BMAP_RET_UNINIT;
	}
got_खण्डम्:
	यदि ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
		implied_cluster_alloc(fs, ino, inode, handle, खण्डम्, &blk64);
		यदि (blk64)
			गच्छ समुच्चयः_extent;
		retval = extent_bmap(fs, ino, inode, handle, खण्डम्_buf,
				     0, खण्डम्-1, 0, खण्डानि_alloc, &blk64);
		यदि (retval)
			blk64 = ext2fs_find_inode_goal(fs, ino, inode, खण्डम्);
		alloc_ctx.ino = ino;
		alloc_ctx.inode = inode;
		alloc_ctx.lblk = extent.e_lblk;
		alloc_ctx.flags = खण्डम्_ALLOC_दत्त;
		retval = ext2fs_alloc_block3(fs, blk64, खण्डम्_buf, &blk64,
					     &alloc_ctx);
		यदि (retval)
			प्रत्यागम retval;
		blk64 &= ~EXT2_FS_CLUSTER_आवरणम्(fs);
		blk64 += EXT2_FS_CLUSTER_आवरणम्(fs) & खण्डम्;
		alloc++;
	समुच्चयः_extent:
		retval = ext2fs_extent_समुच्चयः_bmap(handle, खण्डम्,
						blk64, समुच्चयः_flags);
		यदि (retval) {
			ext2fs_खण्डम्_alloc_stats2(fs, blk64, -1);
			प्रत्यागम retval;
		}
		/* Update inode after setting extent */
		retval = ext2fs_read_inode(fs, ino, inode);
		यदि (retval)
			प्रत्यागम retval;
		*खण्डानि_alloc += alloc;
		*phys_blk = blk64;
	}
	प्रत्यागम 0;
}

पूर्णाङ्क ext2fs_file_खण्डम्_विस्थापनम्_too_big(ext2_filsys fs,
				     संरचना ext2_inode *inode,
				     blk64_t विस्थापनम्)
{
	blk64_t addr_प्रत्येकम्_खण्डम्, max_map_खण्डम्;

	/* Kernel seems to cut us off at 4294967294 blocks */
	यदि (विस्थापनम् >= (1ULL << 32) - 1)
		प्रत्यागम 1;

	यदि (inode->i_flags & EXT4_EXTENTS_FL)
		प्रत्यागम 0;

	addr_प्रत्येकम्_खण्डम् = fs->blocksize >> 2;
	max_map_खण्डम् = addr_प्रत्येकम्_खण्डम्;
	max_map_खण्डम् += addr_प्रत्येकम्_खण्डम् * addr_प्रत्येकम्_खण्डम्;
	max_map_खण्डम् += addr_प्रत्येकम्_खण्डम् * addr_प्रत्येकम्_खण्डम् * addr_प्रत्येकम्_खण्डम्;
	max_map_खण्डम् += 12;

	प्रत्यागम विस्थापनम् >= max_map_खण्डम्;
}

errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino, संरचना ext2_inode *inode,
		       वर्ण *खण्डम्_buf, पूर्णाङ्क bmap_flags, blk64_t खण्डम्,
		       पूर्णाङ्क *ret_flags, blk64_t *phys_blk)
{
	संरचना ext2_inode inode_buf;
	ext2_extent_handle_t handle = 0;
	blk_t addr_प्रत्येकम्_खण्डम्;
	blk_t	b, blk32;
	blk64_t b64;
	वर्ण	*buf = 0;
	errcode_t	retval = 0;
	पूर्णाङ्क		खण्डानि_alloc = 0, inode_dirty = 0;
	संरचना blk_alloc_ctx alloc_ctx = {
		.ino	= ino,
		.inode	= inode,
		.lblk	= 0,
		.flags	= खण्डम्_ALLOC_दत्त,
	};

	यदि (!(bmap_flags & BMAP_समुच्चयः))
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
	addr_प्रत्येकम्_खण्डम् = (blk_t) fs->blocksize >> 2;

	यदि (ext2fs_file_खण्डम्_विस्थापनम्_too_big(fs, inode, खण्डम्))
		प्रत्यागम EXT2_ET_FILE_TOO_BIG;

	/*
	 * If an inode has inline data, that means that it doesn't have
	 * any blocks and we shouldn't map any blocks for it.
	 */
	यदि (inode->i_flags & EXT4_INLINE_दत्त_FL)
		प्रत्यागम EXT2_ET_INLINE_दत्त_NO_खण्डम्;

	यदि (!खण्डम्_buf) {
		retval = ext2fs_get_array(2, fs->blocksize, &buf);
		यदि (retval)
			प्रत्यागम retval;
		खण्डम्_buf = buf;
	}

	यदि (inode->i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		यदि (retval)
			गच्छ समाप्तम्;
		retval = extent_bmap(fs, ino, inode, handle, खण्डम्_buf,
				     bmap_flags, खण्डम्, ret_flags,
				     &खण्डानि_alloc, phys_blk);
		गच्छ समाप्तम्;
	}

	यदि (खण्डम् < EXT2_NDIR_खण्डानि) {
		यदि (bmap_flags & BMAP_समुच्चयः) {
			b = *phys_blk;
			inode_bmap(inode, खण्डम्) = b;
			inode_dirty++;
			गच्छ समाप्तम्;
		}

		*phys_blk = inode_bmap(inode, खण्डम्);
		b = खण्डम् ? inode_bmap(inode, खण्डम् - 1) :
			    ext2fs_find_inode_goal(fs, ino, inode, खण्डम्);

		यदि ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, खण्डम्_buf, &b64,
						     &alloc_ctx);
			b = b64;
			यदि (retval)
				गच्छ समाप्तम्;
			inode_bmap(inode, खण्डम्) = b;
			खण्डानि_alloc++;
			*phys_blk = b;
		}
		गच्छ समाप्तम्;
	}

	/* Indirect block */
	खण्डम् -= EXT2_NDIR_खण्डानि;
	blk32 = *phys_blk;
	यदि (खण्डम् < addr_प्रत्येकम्_खण्डम्) {
		b = inode_bmap(inode, EXT2_IND_खण्डम्);
		यदि (!b) {
			यदि (!(bmap_flags & BMAP_ALLOC)) {
				यदि (bmap_flags & BMAP_समुच्चयः)
					retval = EXT2_ET_समुच्चयः_BMAP_NO_IND;
				गच्छ समाप्तम्;
			}

			b = inode_bmap(inode, EXT2_IND_खण्डम्-1);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, खण्डम्_buf, &b64,
						     &alloc_ctx);
			b = b64;
			यदि (retval)
				गच्छ समाप्तम्;
			inode_bmap(inode, EXT2_IND_खण्डम्) = b;
			खण्डानि_alloc++;
		}
		retval = खण्डम्_ind_bmap(fs, bmap_flags, b, खण्डम्_buf,
					&खण्डानि_alloc, खण्डम्, &blk32);
		यदि (retval == 0)
			*phys_blk = blk32;
		गच्छ समाप्तम्;
	}

	/* Doubly indirect block  */
	खण्डम् -= addr_प्रत्येकम्_खण्डम्;
	यदि (खण्डम् < addr_प्रत्येकम्_खण्डम् * addr_प्रत्येकम्_खण्डम्) {
		b = inode_bmap(inode, EXT2_DIND_खण्डम्);
		यदि (!b) {
			यदि (!(bmap_flags & BMAP_ALLOC)) {
				यदि (bmap_flags & BMAP_समुच्चयः)
					retval = EXT2_ET_समुच्चयः_BMAP_NO_IND;
				गच्छ समाप्तम्;
			}

			b = inode_bmap(inode, EXT2_IND_खण्डम्);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, खण्डम्_buf, &b64,
						     &alloc_ctx);
			b = b64;
			यदि (retval)
				गच्छ समाप्तम्;
			inode_bmap(inode, EXT2_DIND_खण्डम्) = b;
			खण्डानि_alloc++;
		}
		retval = खण्डम्_dind_bmap(fs, bmap_flags, b, खण्डम्_buf,
					 &खण्डानि_alloc, खण्डम्, &blk32);
		यदि (retval == 0)
			*phys_blk = blk32;
		गच्छ समाप्तम्;
	}

	/* Triply indirect block */
	खण्डम् -= addr_प्रत्येकम्_खण्डम् * addr_प्रत्येकम्_खण्डम्;
	b = inode_bmap(inode, EXT2_TIND_खण्डम्);
	यदि (!b) {
		यदि (!(bmap_flags & BMAP_ALLOC)) {
			यदि (bmap_flags & BMAP_समुच्चयः)
				retval = EXT2_ET_समुच्चयः_BMAP_NO_IND;
			गच्छ समाप्तम्;
		}

		b = inode_bmap(inode, EXT2_DIND_खण्डम्);
		b64 = b;
		retval = ext2fs_alloc_block3(fs, b64, खण्डम्_buf, &b64,
					     &alloc_ctx);
		b = b64;
		यदि (retval)
			गच्छ समाप्तम्;
		inode_bmap(inode, EXT2_TIND_खण्डम्) = b;
		खण्डानि_alloc++;
	}
	retval = खण्डम्_tind_bmap(fs, bmap_flags, b, खण्डम्_buf,
				 &खण्डानि_alloc, खण्डम्, &blk32);
	यदि (retval == 0)
		*phys_blk = blk32;
समाप्तम्:
	यदि (*phys_blk && retval == 0 && (bmap_flags & BMAP_ZERO))
		retval = ext2fs_zero_blocks2(fs, *phys_blk, 1, रिक्त, रिक्त);
	यदि (buf)
		ext2fs_free_mem(&buf);
	यदि (handle)
		ext2fs_extent_free(handle);
	यदि ((retval == 0) && (खण्डानि_alloc || inode_dirty)) {
		ext2fs_iblk_सङ्कलय_खण्डानि(fs, inode, खण्डानि_alloc);
		retval = ext2fs_write_inode(fs, ino, inode);
	}
	प्रत्यागम retval;
}

errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino, संरचना ext2_inode *inode,
		      वर्ण *खण्डम्_buf, पूर्णाङ्क bmap_flags, blk_t खण्डम्,
		      blk_t *phys_blk)
{
	errcode_t ret;
	blk64_t	ret_blk = *phys_blk;

	ret = ext2fs_bmap2(fs, ino, inode, खण्डम्_buf, bmap_flags, खण्डम्,
			    0, &ret_blk);
	यदि (ret)
		प्रत्यागम ret;
	यदि (ret_blk >= ((दीर्घ दीर्घ) 1 << 32))
		प्रत्यागम EOVERFLOW;
	*phys_blk = ret_blk;
	प्रत्यागम 0;
}
