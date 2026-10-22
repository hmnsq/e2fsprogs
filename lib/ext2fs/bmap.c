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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <errno.h>

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

#اگر defined(__GNUC__) && !defined(NO_INLINE_FUNCS)
#تعریف_کرو _BMAP_INLINE_	__inline__
#ورنہ
#تعریف_کرو _BMAP_INLINE_
#اگر_ختم

خارجی errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     ساخت ext2_inode *inode,
			     حرف *بلاک_buf, صحیح bmap_flags,
			     blk_t بلاک, blk_t *phys_blk);

#تعریف_کرو inode_bmap(inode, nr) ((inode)->i_بلاک[(nr)])

ساکن _BMAP_INLINE_ errcode_t بلاک_ind_bmap(ext2_filsys fs, صحیح flags,
					      blk_t ind, حرف *بلاک_buf,
					      صحیح *بلاکیں_alloc,
					      blk_t nr, blk_t *ret_blk)
{
	errcode_t	retval;
	blk_t		b;

	اگر (!ind) {
		اگر (flags & BMAP_سیٹ)
			واپس EXT2_ET_سیٹ_BMAP_NO_IND;
		*ret_blk = 0;
		واپس 0;
	}
	retval = io_channel_read_blk(fs->io, ind, 1, بلاک_buf);
	اگر (retval)
		واپس retval;

	اگر (flags & BMAP_سیٹ) {
		b = *ret_blk;
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
		b = ext2fs_swab32(b);
#اگر_ختم
		((blk_t *) بلاک_buf)[nr] = b;
		واپس io_channel_write_blk(fs->io, ind, 1, بلاک_buf);
	}

	b = ((blk_t *) بلاک_buf)[nr];

#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	b = ext2fs_swab32(b);
#اگر_ختم

	اگر (!b && (flags & BMAP_ALLOC)) {
		b = nr ? ext2fs_le32_مقصد_cpu(((blk_t *)بلاک_buf)[nr - 1]) : ind;
		retval = ext2fs_alloc_بلاک(fs, b,
					    بلاک_buf + fs->blocksize, &b);
		اگر (retval)
			واپس retval;

#اگر_تعریف_شدہ الفاظ_BIGENDIAN
		((blk_t *) بلاک_buf)[nr] = ext2fs_swab32(b);
#ورنہ
		((blk_t *) بلاک_buf)[nr] = b;
#اگر_ختم

		retval = io_channel_write_blk(fs->io, ind, 1, بلاک_buf);
		اگر (retval)
			واپس retval;

		(*بلاکیں_alloc)++;
	}

	*ret_blk = b;
	واپس 0;
}

ساکن _BMAP_INLINE_ errcode_t بلاک_dind_bmap(ext2_filsys fs, صحیح flags,
					       blk_t dind, حرف *بلاک_buf,
					       صحیح *بلاکیں_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_فی_بلاک;

	addr_فی_بلاک = (blk_t) fs->blocksize >> 2;

	retval = بلاک_ind_bmap(fs, flags & ~BMAP_سیٹ, dind, بلاک_buf,
				بلاکیں_alloc, nr / addr_فی_بلاک, &b);
	اگر (retval)
		واپس retval;
	retval = بلاک_ind_bmap(fs, flags, b, بلاک_buf, بلاکیں_alloc,
				nr % addr_فی_بلاک, ret_blk);
	واپس retval;
}

ساکن _BMAP_INLINE_ errcode_t بلاک_tind_bmap(ext2_filsys fs, صحیح flags,
					       blk_t tind, حرف *بلاک_buf,
					       صحیح *بلاکیں_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_فی_بلاک;

	addr_فی_بلاک = (blk_t) fs->blocksize >> 2;

	retval = بلاک_dind_bmap(fs, flags & ~BMAP_سیٹ, tind, بلاک_buf,
				 بلاکیں_alloc, nr / addr_فی_بلاک, &b);
	اگر (retval)
		واپس retval;
	retval = بلاک_ind_bmap(fs, flags, b, بلاک_buf, بلاکیں_alloc,
				nr % addr_فی_بلاک, ret_blk);
	واپس retval;
}

ساکن errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     ساخت ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     حرف *بلاک_buf, صحیح bmap_flags, blk64_t بلاک,
			     صحیح *ret_flags, صحیح *بلاکیں_alloc,
			     blk64_t *phys_blk);

ساکن errcode_t implied_cluster_alloc(ext2_filsys fs, ext2_ino_t ino,
				       ساخت ext2_inode *inode,
				       ext2_extent_handle_t handle,
				       blk64_t lblk, blk64_t *phys_blk)
{
	blk64_t	بنیاد_بلاک, pblock = 0;
	صحیح i;

	اگر (!ext2fs_has_feature_bigalloc(fs->super))
		واپس 0;

	بنیاد_بلاک = lblk & ~EXT2_FS_CLUSTER_نقاب(fs);
	/*
	 * Except for the logical block (lblk) that was passed in, search all
	 * blocks in this logical cluster for a mapping to a physical cluster.
	 * If any such map exists, calculate the physical block that maps to
	 * the logical block and return that.
	 *
	 * The old code wouldn't even look if (block % cluster_ratio) == 0;
	 * this is incorrect if we're allocating blocks in reverse order.
	 */
	برائے (i = 0; i < EXT2_FS_CLUSTER_تناسب(fs); i++) {
		اگر (بنیاد_بلاک + i == lblk)
			جاری;
		extent_bmap(fs, ino, inode, handle, 0, 0,
			    بنیاد_بلاک + i, 0, 0, &pblock);
		اگر (pblock)
			توڑو;
	}
	اگر (pblock == 0)
		واپس 0;
	*phys_blk = pblock - i + (lblk - بنیاد_بلاک);
	واپس 0;
}

/* Try to map a logical block to an already-allocated physical cluster. */
errcode_t ext2fs_map_cluster_بلاک(ext2_filsys fs, ext2_ino_t ino,
				   ساخت ext2_inode *inode, blk64_t lblk,
				   blk64_t *pblk)
{
	ext2_extent_handle_t handle;
	errcode_t retval;

	/* Need bigalloc and extents to be enabled */
	*pblk = 0;
	اگر (!ext2fs_has_feature_bigalloc(fs->super) ||
	    !(inode->i_flags & EXT4_EXTENTS_FL))
		واپس 0;

	retval = ext2fs_extent_open2(fs, ino, inode, &handle);
	اگر (retval)
		جاؤ برآمد;

	retval = implied_cluster_alloc(fs, ino, inode, handle, lblk, pblk);
	اگر (retval)
		جاؤ out2;

out2:
	ext2fs_extent_free(handle);
برآمد:
	واپس retval;
}

ساکن errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     ساخت ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     حرف *بلاک_buf, صحیح bmap_flags, blk64_t بلاک,
			     صحیح *ret_flags, صحیح *بلاکیں_alloc,
			     blk64_t *phys_blk)
{
	ساخت blk_alloc_ctx	alloc_ctx;
	ساخت ext2fs_extent	extent;
	غیرعلامتی صحیح		انحراف;
	errcode_t		retval = 0;
	blk64_t			blk64 = 0;
	صحیح			alloc = 0;
	صحیح			سیٹ_flags;

	سیٹ_flags = bmap_flags & BMAP_UNINIT ? EXT2_EXTENT_سیٹ_BMAP_UNINIT : 0;

	اگر (bmap_flags & BMAP_سیٹ) {
		retval = ext2fs_extent_سیٹ_bmap(handle, بلاک,
						*phys_blk, سیٹ_flags);
		واپس retval;
	}
	retval = ext2fs_extent_goto(handle, بلاک);
	اگر (retval) {
		/* If the extent is not found, return phys_blk = 0 */
		اگر (retval == EXT2_ET_EXTENT_NOT_ملا) {
			extent.e_lblk = بلاک;
			جاؤ got_بلاک;
		}
		واپس retval;
	}
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	اگر (retval)
		واپس retval;
	انحراف = بلاک - extent.e_lblk;
	اگر (بلاک >= extent.e_lblk && (انحراف <= extent.e_len)) {
		*phys_blk = extent.e_pblk + انحراف;
		اگر (ret_flags && extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			*ret_flags |= BMAP_RET_UNINIT;
	}
got_بلاک:
	اگر ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
		implied_cluster_alloc(fs, ino, inode, handle, بلاک, &blk64);
		اگر (blk64)
			جاؤ سیٹ_extent;
		retval = extent_bmap(fs, ino, inode, handle, بلاک_buf,
				     0, بلاک-1, 0, بلاکیں_alloc, &blk64);
		اگر (retval)
			blk64 = ext2fs_ڈھونڈو_inode_goal(fs, ino, inode, بلاک);
		alloc_ctx.ino = ino;
		alloc_ctx.inode = inode;
		alloc_ctx.lblk = extent.e_lblk;
		alloc_ctx.flags = بلاک_ALLOC_ڈیٹا;
		retval = ext2fs_alloc_block3(fs, blk64, بلاک_buf, &blk64,
					     &alloc_ctx);
		اگر (retval)
			واپس retval;
		blk64 &= ~EXT2_FS_CLUSTER_نقاب(fs);
		blk64 += EXT2_FS_CLUSTER_نقاب(fs) & بلاک;
		alloc++;
	سیٹ_extent:
		retval = ext2fs_extent_سیٹ_bmap(handle, بلاک,
						blk64, سیٹ_flags);
		اگر (retval) {
			ext2fs_بلاک_alloc_stats2(fs, blk64, -1);
			واپس retval;
		}
		/* Update inode after setting extent */
		retval = ext2fs_read_inode(fs, ino, inode);
		اگر (retval)
			واپس retval;
		*بلاکیں_alloc += alloc;
		*phys_blk = blk64;
	}
	واپس 0;
}

صحیح ext2fs_file_بلاک_انحراف_too_big(ext2_filsys fs,
				     ساخت ext2_inode *inode,
				     blk64_t انحراف)
{
	blk64_t addr_فی_بلاک, max_map_بلاک;

	/* Kernel seems to cut us off at 4294967294 blocks */
	اگر (انحراف >= (1ULL << 32) - 1)
		واپس 1;

	اگر (inode->i_flags & EXT4_EXTENTS_FL)
		واپس 0;

	addr_فی_بلاک = fs->blocksize >> 2;
	max_map_بلاک = addr_فی_بلاک;
	max_map_بلاک += addr_فی_بلاک * addr_فی_بلاک;
	max_map_بلاک += addr_فی_بلاک * addr_فی_بلاک * addr_فی_بلاک;
	max_map_بلاک += 12;

	واپس انحراف >= max_map_بلاک;
}

errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino, ساخت ext2_inode *inode,
		       حرف *بلاک_buf, صحیح bmap_flags, blk64_t بلاک,
		       صحیح *ret_flags, blk64_t *phys_blk)
{
	ساخت ext2_inode inode_buf;
	ext2_extent_handle_t handle = 0;
	blk_t addr_فی_بلاک;
	blk_t	b, blk32;
	blk64_t b64;
	حرف	*buf = 0;
	errcode_t	retval = 0;
	صحیح		بلاکیں_alloc = 0, inode_dirty = 0;
	ساخت blk_alloc_ctx alloc_ctx = {
		.ino	= ino,
		.inode	= inode,
		.lblk	= 0,
		.flags	= بلاک_ALLOC_ڈیٹا,
	};

	اگر (!(bmap_flags & BMAP_سیٹ))
		*phys_blk = 0;

	اگر (ret_flags)
		*ret_flags = 0;

	/* Read inode structure if necessary */
	اگر (!inode) {
		retval = ext2fs_read_inode(fs, ino, &inode_buf);
		اگر (retval)
			واپس retval;
		inode = &inode_buf;
	}
	addr_فی_بلاک = (blk_t) fs->blocksize >> 2;

	اگر (ext2fs_file_بلاک_انحراف_too_big(fs, inode, بلاک))
		واپس EXT2_ET_FILE_TOO_BIG;

	/*
	 * If an inode has inline data, that means that it doesn't have
	 * any blocks and we shouldn't map any blocks for it.
	 */
	اگر (inode->i_flags & EXT4_INLINE_ڈیٹا_FL)
		واپس EXT2_ET_INLINE_ڈیٹا_NO_بلاک;

	اگر (!بلاک_buf) {
		retval = ext2fs_get_array(2, fs->blocksize, &buf);
		اگر (retval)
			واپس retval;
		بلاک_buf = buf;
	}

	اگر (inode->i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		اگر (retval)
			جاؤ مکمل;
		retval = extent_bmap(fs, ino, inode, handle, بلاک_buf,
				     bmap_flags, بلاک, ret_flags,
				     &بلاکیں_alloc, phys_blk);
		جاؤ مکمل;
	}

	اگر (بلاک < EXT2_NDIR_بلاکیں) {
		اگر (bmap_flags & BMAP_سیٹ) {
			b = *phys_blk;
			inode_bmap(inode, بلاک) = b;
			inode_dirty++;
			جاؤ مکمل;
		}

		*phys_blk = inode_bmap(inode, بلاک);
		b = بلاک ? inode_bmap(inode, بلاک - 1) :
			    ext2fs_ڈھونڈو_inode_goal(fs, ino, inode, بلاک);

		اگر ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, بلاک_buf, &b64,
						     &alloc_ctx);
			b = b64;
			اگر (retval)
				جاؤ مکمل;
			inode_bmap(inode, بلاک) = b;
			بلاکیں_alloc++;
			*phys_blk = b;
		}
		جاؤ مکمل;
	}

	/* Indirect block */
	بلاک -= EXT2_NDIR_بلاکیں;
	blk32 = *phys_blk;
	اگر (بلاک < addr_فی_بلاک) {
		b = inode_bmap(inode, EXT2_IND_بلاک);
		اگر (!b) {
			اگر (!(bmap_flags & BMAP_ALLOC)) {
				اگر (bmap_flags & BMAP_سیٹ)
					retval = EXT2_ET_سیٹ_BMAP_NO_IND;
				جاؤ مکمل;
			}

			b = inode_bmap(inode, EXT2_IND_بلاک-1);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, بلاک_buf, &b64,
						     &alloc_ctx);
			b = b64;
			اگر (retval)
				جاؤ مکمل;
			inode_bmap(inode, EXT2_IND_بلاک) = b;
			بلاکیں_alloc++;
		}
		retval = بلاک_ind_bmap(fs, bmap_flags, b, بلاک_buf,
					&بلاکیں_alloc, بلاک, &blk32);
		اگر (retval == 0)
			*phys_blk = blk32;
		جاؤ مکمل;
	}

	/* Doubly indirect block  */
	بلاک -= addr_فی_بلاک;
	اگر (بلاک < addr_فی_بلاک * addr_فی_بلاک) {
		b = inode_bmap(inode, EXT2_DIND_بلاک);
		اگر (!b) {
			اگر (!(bmap_flags & BMAP_ALLOC)) {
				اگر (bmap_flags & BMAP_سیٹ)
					retval = EXT2_ET_سیٹ_BMAP_NO_IND;
				جاؤ مکمل;
			}

			b = inode_bmap(inode, EXT2_IND_بلاک);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, بلاک_buf, &b64,
						     &alloc_ctx);
			b = b64;
			اگر (retval)
				جاؤ مکمل;
			inode_bmap(inode, EXT2_DIND_بلاک) = b;
			بلاکیں_alloc++;
		}
		retval = بلاک_dind_bmap(fs, bmap_flags, b, بلاک_buf,
					 &بلاکیں_alloc, بلاک, &blk32);
		اگر (retval == 0)
			*phys_blk = blk32;
		جاؤ مکمل;
	}

	/* Triply indirect block */
	بلاک -= addr_فی_بلاک * addr_فی_بلاک;
	b = inode_bmap(inode, EXT2_TIND_بلاک);
	اگر (!b) {
		اگر (!(bmap_flags & BMAP_ALLOC)) {
			اگر (bmap_flags & BMAP_سیٹ)
				retval = EXT2_ET_سیٹ_BMAP_NO_IND;
			جاؤ مکمل;
		}

		b = inode_bmap(inode, EXT2_DIND_بلاک);
		b64 = b;
		retval = ext2fs_alloc_block3(fs, b64, بلاک_buf, &b64,
					     &alloc_ctx);
		b = b64;
		اگر (retval)
			جاؤ مکمل;
		inode_bmap(inode, EXT2_TIND_بلاک) = b;
		بلاکیں_alloc++;
	}
	retval = بلاک_tind_bmap(fs, bmap_flags, b, بلاک_buf,
				 &بلاکیں_alloc, بلاک, &blk32);
	اگر (retval == 0)
		*phys_blk = blk32;
مکمل:
	اگر (*phys_blk && retval == 0 && (bmap_flags & BMAP_ZERO))
		retval = ext2fs_zero_blocks2(fs, *phys_blk, 1, تہی, تہی);
	اگر (buf)
		ext2fs_free_mem(&buf);
	اگر (handle)
		ext2fs_extent_free(handle);
	اگر ((retval == 0) && (بلاکیں_alloc || inode_dirty)) {
		ext2fs_iblk_جوڑو_بلاکیں(fs, inode, بلاکیں_alloc);
		retval = ext2fs_write_inode(fs, ino, inode);
	}
	واپس retval;
}

errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino, ساخت ext2_inode *inode,
		      حرف *بلاک_buf, صحیح bmap_flags, blk_t بلاک,
		      blk_t *phys_blk)
{
	errcode_t ret;
	blk64_t	ret_blk = *phys_blk;

	ret = ext2fs_bmap2(fs, ino, inode, بلاک_buf, bmap_flags, بلاک,
			    0, &ret_blk);
	اگر (ret)
		واپس ret;
	اگر (ret_blk >= ((طویل طویل) 1 << 32))
		واپس EOVERFLOW;
	*phys_blk = ret_blk;
	واپس 0;
}
