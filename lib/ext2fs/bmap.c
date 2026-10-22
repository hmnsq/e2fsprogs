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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <errno.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fsP.h"

#ఒకవేళ defined(__GNUC__) && !defined(NO_INLINE_FUNCS)
#నిర్వచించు _BMAP_INLINE_	__inline__
#లేకపోతే
#నిర్వచించు _BMAP_INLINE_
#అయితే_ముగింపు

బాహ్య errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     నిర్మాణం ext2_inode *inode,
			     అక్షరం *ఖండం_buf, పూర్ణాంకం bmap_flags,
			     blk_t ఖండం, blk_t *phys_blk);

#నిర్వచించు inode_bmap(inode, nr) ((inode)->i_ఖండం[(nr)])

స్థిర _BMAP_INLINE_ errcode_t ఖండం_ind_bmap(ext2_filsys fs, పూర్ణాంకం flags,
					      blk_t ind, అక్షరం *ఖండం_buf,
					      పూర్ణాంకం *ఖండాలు_alloc,
					      blk_t nr, blk_t *ret_blk)
{
	errcode_t	retval;
	blk_t		b;

	ఒకవేళ (!ind) {
		ఒకవేళ (flags & BMAP_సమితి)
			తిరిగి EXT2_ET_సమితి_BMAP_NO_IND;
		*ret_blk = 0;
		తిరిగి 0;
	}
	retval = io_channel_read_blk(fs->io, ind, 1, ఖండం_buf);
	ఒకవేళ (retval)
		తిరిగి retval;

	ఒకవేళ (flags & BMAP_సమితి) {
		b = *ret_blk;
#నిర్వచితమైతే పదాలు_BIGENDIAN
		b = ext2fs_swab32(b);
#అయితే_ముగింపు
		((blk_t *) ఖండం_buf)[nr] = b;
		తిరిగి io_channel_write_blk(fs->io, ind, 1, ఖండం_buf);
	}

	b = ((blk_t *) ఖండం_buf)[nr];

#నిర్వచితమైతే పదాలు_BIGENDIAN
	b = ext2fs_swab32(b);
#అయితే_ముగింపు

	ఒకవేళ (!b && (flags & BMAP_ALLOC)) {
		b = nr ? ext2fs_le32_గమ్యం_cpu(((blk_t *)ఖండం_buf)[nr - 1]) : ind;
		retval = ext2fs_alloc_ఖండం(fs, b,
					    ఖండం_buf + fs->blocksize, &b);
		ఒకవేళ (retval)
			తిరిగి retval;

#నిర్వచితమైతే పదాలు_BIGENDIAN
		((blk_t *) ఖండం_buf)[nr] = ext2fs_swab32(b);
#లేకపోతే
		((blk_t *) ఖండం_buf)[nr] = b;
#అయితే_ముగింపు

		retval = io_channel_write_blk(fs->io, ind, 1, ఖండం_buf);
		ఒకవేళ (retval)
			తిరిగి retval;

		(*ఖండాలు_alloc)++;
	}

	*ret_blk = b;
	తిరిగి 0;
}

స్థిర _BMAP_INLINE_ errcode_t ఖండం_dind_bmap(ext2_filsys fs, పూర్ణాంకం flags,
					       blk_t dind, అక్షరం *ఖండం_buf,
					       పూర్ణాంకం *ఖండాలు_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_చొప్పున_ఖండం;

	addr_చొప్పున_ఖండం = (blk_t) fs->blocksize >> 2;

	retval = ఖండం_ind_bmap(fs, flags & ~BMAP_సమితి, dind, ఖండం_buf,
				ఖండాలు_alloc, nr / addr_చొప్పున_ఖండం, &b);
	ఒకవేళ (retval)
		తిరిగి retval;
	retval = ఖండం_ind_bmap(fs, flags, b, ఖండం_buf, ఖండాలు_alloc,
				nr % addr_చొప్పున_ఖండం, ret_blk);
	తిరిగి retval;
}

స్థిర _BMAP_INLINE_ errcode_t ఖండం_tind_bmap(ext2_filsys fs, పూర్ణాంకం flags,
					       blk_t tind, అక్షరం *ఖండం_buf,
					       పూర్ణాంకం *ఖండాలు_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_చొప్పున_ఖండం;

	addr_చొప్పున_ఖండం = (blk_t) fs->blocksize >> 2;

	retval = ఖండం_dind_bmap(fs, flags & ~BMAP_సమితి, tind, ఖండం_buf,
				 ఖండాలు_alloc, nr / addr_చొప్పున_ఖండం, &b);
	ఒకవేళ (retval)
		తిరిగి retval;
	retval = ఖండం_ind_bmap(fs, flags, b, ఖండం_buf, ఖండాలు_alloc,
				nr % addr_చొప్పున_ఖండం, ret_blk);
	తిరిగి retval;
}

స్థిర errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     నిర్మాణం ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     అక్షరం *ఖండం_buf, పూర్ణాంకం bmap_flags, blk64_t ఖండం,
			     పూర్ణాంకం *ret_flags, పూర్ణాంకం *ఖండాలు_alloc,
			     blk64_t *phys_blk);

స్థిర errcode_t implied_cluster_alloc(ext2_filsys fs, ext2_ino_t ino,
				       నిర్మాణం ext2_inode *inode,
				       ext2_extent_handle_t handle,
				       blk64_t lblk, blk64_t *phys_blk)
{
	blk64_t	పునాది_ఖండం, pblock = 0;
	పూర్ణాంకం i;

	ఒకవేళ (!ext2fs_has_feature_bigalloc(fs->super))
		తిరిగి 0;

	పునాది_ఖండం = lblk & ~EXT2_FS_CLUSTER_ముసుగు(fs);
	/*
	 * Except for the logical block (lblk) that was passed in, search all
	 * blocks in this logical cluster for a mapping to a physical cluster.
	 * If any such map exists, calculate the physical block that maps to
	 * the logical block and return that.
	 *
	 * The old code wouldn't even look if (block % cluster_ratio) == 0;
	 * this is incorrect if we're allocating blocks in reverse order.
	 */
	కొరకు (i = 0; i < EXT2_FS_CLUSTER_నిష్పత్తి(fs); i++) {
		ఒకవేళ (పునాది_ఖండం + i == lblk)
			కొనసాగు;
		extent_bmap(fs, ino, inode, handle, 0, 0,
			    పునాది_ఖండం + i, 0, 0, &pblock);
		ఒకవేళ (pblock)
			ఆపు;
	}
	ఒకవేళ (pblock == 0)
		తిరిగి 0;
	*phys_blk = pblock - i + (lblk - పునాది_ఖండం);
	తిరిగి 0;
}

/* Try to map a logical block to an already-allocated physical cluster. */
errcode_t ext2fs_map_cluster_ఖండం(ext2_filsys fs, ext2_ino_t ino,
				   నిర్మాణం ext2_inode *inode, blk64_t lblk,
				   blk64_t *pblk)
{
	ext2_extent_handle_t handle;
	errcode_t retval;

	/* Need bigalloc and extents to be enabled */
	*pblk = 0;
	ఒకవేళ (!ext2fs_has_feature_bigalloc(fs->super) ||
	    !(inode->i_flags & EXT4_EXTENTS_FL))
		తిరిగి 0;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	ఒకవేళ (retval)
		వెళ్ళు బహిర్గతం;

	retval = implied_cluster_alloc(fs, ino, inode, handle, lblk, pblk);
	ఒకవేళ (retval)
		వెళ్ళు out2;

out2:
	ext2fs_extent_free(handle);
బహిర్గతం:
	తిరిగి retval;
}

స్థిర errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     నిర్మాణం ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     అక్షరం *ఖండం_buf, పూర్ణాంకం bmap_flags, blk64_t ఖండం,
			     పూర్ణాంకం *ret_flags, పూర్ణాంకం *ఖండాలు_alloc,
			     blk64_t *phys_blk)
{
	నిర్మాణం blk_alloc_ctx	alloc_ctx;
	నిర్మాణం ext2fs_extent	extent;
	గుర్తులేని పూర్ణాంకం		స్థానభ్రంశం;
	errcode_t		retval = 0;
	blk64_t			blk64 = 0;
	పూర్ణాంకం			alloc = 0;
	పూర్ణాంకం			సమితి_flags;

	సమితి_flags = bmap_flags & BMAP_UNINIT ? EXT2_EXTENT_సమితి_BMAP_UNINIT : 0;

	ఒకవేళ (bmap_flags & BMAP_సమితి) {
		retval = ext2fs_extent_సమితి_bmap(handle, ఖండం,
						*phys_blk, సమితి_flags);
		తిరిగి retval;
	}
	retval = ext2fs_extent_goto(handle, ఖండం);
	ఒకవేళ (retval) {
		/* If the extent is not found, return phys_blk = 0 */
		ఒకవేళ (retval == EXT2_ET_EXTENT_NOT_దొరికింది) {
			extent.e_lblk = ఖండం;
			వెళ్ళు got_ఖండం;
		}
		తిరిగి retval;
	}
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	ఒకవేళ (retval)
		తిరిగి retval;
	స్థానభ్రంశం = ఖండం - extent.e_lblk;
	ఒకవేళ (ఖండం >= extent.e_lblk && (స్థానభ్రంశం <= extent.e_len)) {
		*phys_blk = extent.e_pblk + స్థానభ్రంశం;
		ఒకవేళ (ret_flags && extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			*ret_flags |= BMAP_RET_UNINIT;
	}
got_ఖండం:
	ఒకవేళ ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
		implied_cluster_alloc(fs, ino, inode, handle, ఖండం, &blk64);
		ఒకవేళ (blk64)
			వెళ్ళు సమితి_extent;
		retval = extent_bmap(fs, ino, inode, handle, ఖండం_buf,
				     0, ఖండం-1, 0, ఖండాలు_alloc, &blk64);
		ఒకవేళ (retval)
			blk64 = ext2fs_కనుగొను_inode_goal(fs, ino, inode, ఖండం);
		alloc_ctx.ino = ino;
		alloc_ctx.inode = inode;
		alloc_ctx.lblk = extent.e_lblk;
		alloc_ctx.flags = ఖండం_ALLOC_దత్తాంశం;
		retval = ext2fs_alloc_block3(fs, blk64, ఖండం_buf, &blk64,
					     &alloc_ctx);
		ఒకవేళ (retval)
			తిరిగి retval;
		blk64 &= ~EXT2_FS_CLUSTER_ముసుగు(fs);
		blk64 += EXT2_FS_CLUSTER_ముసుగు(fs) & ఖండం;
		alloc++;
	సమితి_extent:
		retval = ext2fs_extent_సమితి_bmap(handle, ఖండం,
						blk64, సమితి_flags);
		ఒకవేళ (retval) {
			ext2fs_ఖండం_alloc_stats2(fs, blk64, -1);
			తిరిగి retval;
		}
		/* Update inode after setting extent */
		retval = ext2fs_read_inode(fs, ino, inode);
		ఒకవేళ (retval)
			తిరిగి retval;
		*ఖండాలు_alloc += alloc;
		*phys_blk = blk64;
	}
	తిరిగి 0;
}

పూర్ణాంకం ext2fs_file_ఖండం_స్థానభ్రంశం_too_big(ext2_filsys fs,
				     నిర్మాణం ext2_inode *inode,
				     blk64_t స్థానభ్రంశం)
{
	blk64_t addr_చొప్పున_ఖండం, max_map_ఖండం;

	/* Kernel seems to cut us off at 4294967294 blocks */
	ఒకవేళ (స్థానభ్రంశం >= (1ULL << 32) - 1)
		తిరిగి 1;

	ఒకవేళ (inode->i_flags & EXT4_EXTENTS_FL)
		తిరిగి 0;

	addr_చొప్పున_ఖండం = fs->blocksize >> 2;
	max_map_ఖండం = addr_చొప్పున_ఖండం;
	max_map_ఖండం += addr_చొప్పున_ఖండం * addr_చొప్పున_ఖండం;
	max_map_ఖండం += addr_చొప్పున_ఖండం * addr_చొప్పున_ఖండం * addr_చొప్పున_ఖండం;
	max_map_ఖండం += 12;

	తిరిగి స్థానభ్రంశం >= max_map_ఖండం;
}

errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino, నిర్మాణం ext2_inode *inode,
		       అక్షరం *ఖండం_buf, పూర్ణాంకం bmap_flags, blk64_t ఖండం,
		       పూర్ణాంకం *ret_flags, blk64_t *phys_blk)
{
	నిర్మాణం ext2_inode inode_buf;
	ext2_extent_handle_t handle = 0;
	blk_t addr_చొప్పున_ఖండం;
	blk_t	b, blk32;
	blk64_t b64;
	అక్షరం	*buf = 0;
	errcode_t	retval = 0;
	పూర్ణాంకం		ఖండాలు_alloc = 0, inode_dirty = 0;
	నిర్మాణం blk_alloc_ctx alloc_ctx = {
		.ino	= ino,
		.inode	= inode,
		.lblk	= 0,
		.flags	= ఖండం_ALLOC_దత్తాంశం,
	};

	ఒకవేళ (!(bmap_flags & BMAP_సమితి))
		*phys_blk = 0;

	ఒకవేళ (ret_flags)
		*ret_flags = 0;

	/* Read inode structure if necessary */
	ఒకవేళ (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		ఒకవేళ (retval)
			తిరిగి retval;
		inode = &inode_buf;
	}
	addr_చొప్పున_ఖండం = (blk_t) fs->blocksize >> 2;

	ఒకవేళ (ext2fs_file_ఖండం_స్థానభ్రంశం_too_big(fs, inode, ఖండం))
		తిరిగి EXT2_ET_FILE_TOO_BIG;

	/*
	 * If an inode has inline data, that means that it doesn't have
	 * any blocks and we shouldn't map any blocks for it.
	 */
	ఒకవేళ (inode->i_flags & EXT4_INLINE_దత్తాంశం_FL)
		తిరిగి EXT2_ET_INLINE_దత్తాంశం_NO_ఖండం;

	ఒకవేళ (!ఖండం_buf) {
		retval = ext2fs_get_array(2, fs->blocksize, &buf);
		ఒకవేళ (retval)
			తిరిగి retval;
		ఖండం_buf = buf;
	}

	ఒకవేళ (inode->i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		ఒకవేళ (retval)
			వెళ్ళు పూర్తి;
		retval = extent_bmap(fs, ino, inode, handle, ఖండం_buf,
				     bmap_flags, ఖండం, ret_flags,
				     &ఖండాలు_alloc, phys_blk);
		వెళ్ళు పూర్తి;
	}

	ఒకవేళ (ఖండం < EXT2_NDIR_ఖండాలు) {
		ఒకవేళ (bmap_flags & BMAP_సమితి) {
			b = *phys_blk;
			inode_bmap(inode, ఖండం) = b;
			inode_dirty++;
			వెళ్ళు పూర్తి;
		}

		*phys_blk = inode_bmap(inode, ఖండం);
		b = ఖండం ? inode_bmap(inode, ఖండం - 1) :
			    ext2fs_కనుగొను_inode_goal(fs, ino, inode, ఖండం);

		ఒకవేళ ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, ఖండం_buf, &b64,
						     &alloc_ctx);
			b = b64;
			ఒకవేళ (retval)
				వెళ్ళు పూర్తి;
			inode_bmap(inode, ఖండం) = b;
			ఖండాలు_alloc++;
			*phys_blk = b;
		}
		వెళ్ళు పూర్తి;
	}

	/* Indirect block */
	ఖండం -= EXT2_NDIR_ఖండాలు;
	blk32 = *phys_blk;
	ఒకవేళ (ఖండం < addr_చొప్పున_ఖండం) {
		b = inode_bmap(inode, EXT2_IND_ఖండం);
		ఒకవేళ (!b) {
			ఒకవేళ (!(bmap_flags & BMAP_ALLOC)) {
				ఒకవేళ (bmap_flags & BMAP_సమితి)
					retval = EXT2_ET_సమితి_BMAP_NO_IND;
				వెళ్ళు పూర్తి;
			}

			b = inode_bmap(inode, EXT2_IND_ఖండం-1);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, ఖండం_buf, &b64,
						     &alloc_ctx);
			b = b64;
			ఒకవేళ (retval)
				వెళ్ళు పూర్తి;
			inode_bmap(inode, EXT2_IND_ఖండం) = b;
			ఖండాలు_alloc++;
		}
		retval = ఖండం_ind_bmap(fs, bmap_flags, b, ఖండం_buf,
					&ఖండాలు_alloc, ఖండం, &blk32);
		ఒకవేళ (retval == 0)
			*phys_blk = blk32;
		వెళ్ళు పూర్తి;
	}

	/* Doubly indirect block  */
	ఖండం -= addr_చొప్పున_ఖండం;
	ఒకవేళ (ఖండం < addr_చొప్పున_ఖండం * addr_చొప్పున_ఖండం) {
		b = inode_bmap(inode, EXT2_DIND_ఖండం);
		ఒకవేళ (!b) {
			ఒకవేళ (!(bmap_flags & BMAP_ALLOC)) {
				ఒకవేళ (bmap_flags & BMAP_సమితి)
					retval = EXT2_ET_సమితి_BMAP_NO_IND;
				వెళ్ళు పూర్తి;
			}

			b = inode_bmap(inode, EXT2_IND_ఖండం);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, ఖండం_buf, &b64,
						     &alloc_ctx);
			b = b64;
			ఒకవేళ (retval)
				వెళ్ళు పూర్తి;
			inode_bmap(inode, EXT2_DIND_ఖండం) = b;
			ఖండాలు_alloc++;
		}
		retval = ఖండం_dind_bmap(fs, bmap_flags, b, ఖండం_buf,
					 &ఖండాలు_alloc, ఖండం, &blk32);
		ఒకవేళ (retval == 0)
			*phys_blk = blk32;
		వెళ్ళు పూర్తి;
	}

	/* Triply indirect block */
	ఖండం -= addr_చొప్పున_ఖండం * addr_చొప్పున_ఖండం;
	b = inode_bmap(inode, EXT2_TIND_ఖండం);
	ఒకవేళ (!b) {
		ఒకవేళ (!(bmap_flags & BMAP_ALLOC)) {
			ఒకవేళ (bmap_flags & BMAP_సమితి)
				retval = EXT2_ET_సమితి_BMAP_NO_IND;
			వెళ్ళు పూర్తి;
		}

		b = inode_bmap(inode, EXT2_DIND_ఖండం);
		b64 = b;
		retval = ext2fs_alloc_block3(fs, b64, ఖండం_buf, &b64,
					     &alloc_ctx);
		b = b64;
		ఒకవేళ (retval)
			వెళ్ళు పూర్తి;
		inode_bmap(inode, EXT2_TIND_ఖండం) = b;
		ఖండాలు_alloc++;
	}
	retval = ఖండం_tind_bmap(fs, bmap_flags, b, ఖండం_buf,
				 &ఖండాలు_alloc, ఖండం, &blk32);
	ఒకవేళ (retval == 0)
		*phys_blk = blk32;
పూర్తి:
	ఒకవేళ (*phys_blk && retval == 0 && (bmap_flags & BMAP_ZERO))
		retval = ext2fs_zero_blocks2(fs, *phys_blk, 1, ఖాళీ, ఖాళీ);
	ఒకవేళ (buf)
		ext2fs_free_mem(&buf);
	ఒకవేళ (handle)
		ext2fs_extent_free(handle);
	ఒకవేళ ((retval == 0) && (ఖండాలు_alloc || inode_dirty)) {
		ext2fs_iblk_కూడు_ఖండాలు(fs, inode, ఖండాలు_alloc);
		retval = ext2fs_write_inode(fs, ino, inode);
	}
	తిరిగి retval;
}

errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino, నిర్మాణం ext2_inode *inode,
		      అక్షరం *ఖండం_buf, పూర్ణాంకం bmap_flags, blk_t ఖండం,
		      blk_t *phys_blk)
{
	errcode_t ret;
	blk64_t	ret_blk = *phys_blk;

	ret = ext2fs_bmap2(fs, ino, inode, ఖండం_buf, bmap_flags, ఖండం,
			    0, &ret_blk);
	ఒకవేళ (ret)
		తిరిగి ret;
	ఒకవేళ (ret_blk >= ((దీర్ఘ దీర్ఘ) 1 << 32))
		తిరిగి EOVERFLOW;
	*phys_blk = ret_blk;
	తిరిగి 0;
}
