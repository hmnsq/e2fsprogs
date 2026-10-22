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
#پایان_اگر
#ضمن <errno.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

#إذا defined(__GNUC__) && !defined(NO_INLINE_FUNCS)
#تعریف_کن _BMAP_INLINE_	__inline__
#وإلا
#تعریف_کن _BMAP_INLINE_
#پایان_اگر

خارجي errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_inode *inode,
			     حرف *بلوک_buf, صحيح bmap_flags,
			     blk_t بلوک, blk_t *phys_blk);

#تعریف_کن inode_bmap(inode, nr) ((inode)->i_بلوک[(nr)])

ساكن _BMAP_INLINE_ errcode_t بلوک_ind_bmap(ext2_filsys fs, صحيح flags,
					      blk_t ind, حرف *بلوک_buf,
					      صحيح *بلوکها_alloc,
					      blk_t nr, blk_t *ret_blk)
{
	errcode_t	retval;
	blk_t		b;

	إذا (!ind) {
		إذا (flags & BMAP_مجموعه)
			أرجع EXT2_ET_مجموعه_BMAP_NO_IND;
		*ret_blk = 0;
		أرجع 0;
	}
	retval = io_channel_read_blk(fs->io, ind, 1, بلوک_buf);
	إذا (retval)
		أرجع retval;

	إذا (flags & BMAP_مجموعه) {
		b = *ret_blk;
#اگر_تعریف_شده واژگان_BIGENDIAN
		b = ext2fs_swab32(b);
#پایان_اگر
		((blk_t *) بلوک_buf)[nr] = b;
		أرجع io_channel_write_blk(fs->io, ind, 1, بلوک_buf);
	}

	b = ((blk_t *) بلوک_buf)[nr];

#اگر_تعریف_شده واژگان_BIGENDIAN
	b = ext2fs_swab32(b);
#پایان_اگر

	إذا (!b && (flags & BMAP_ALLOC)) {
		b = nr ? ext2fs_le32_مقصد_cpu(((blk_t *)بلوک_buf)[nr - 1]) : ind;
		retval = ext2fs_alloc_بلوک(fs, b,
					    بلوک_buf + fs->blocksize, &b);
		إذا (retval)
			أرجع retval;

#اگر_تعریف_شده واژگان_BIGENDIAN
		((blk_t *) بلوک_buf)[nr] = ext2fs_swab32(b);
#وإلا
		((blk_t *) بلوک_buf)[nr] = b;
#پایان_اگر

		retval = io_channel_write_blk(fs->io, ind, 1, بلوک_buf);
		إذا (retval)
			أرجع retval;

		(*بلوکها_alloc)++;
	}

	*ret_blk = b;
	أرجع 0;
}

ساكن _BMAP_INLINE_ errcode_t بلوک_dind_bmap(ext2_filsys fs, صحيح flags,
					       blk_t dind, حرف *بلوک_buf,
					       صحيح *بلوکها_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_در_هر_بلوک;

	addr_در_هر_بلوک = (blk_t) fs->blocksize >> 2;

	retval = بلوک_ind_bmap(fs, flags & ~BMAP_مجموعه, dind, بلوک_buf,
				بلوکها_alloc, nr / addr_در_هر_بلوک, &b);
	إذا (retval)
		أرجع retval;
	retval = بلوک_ind_bmap(fs, flags, b, بلوک_buf, بلوکها_alloc,
				nr % addr_در_هر_بلوک, ret_blk);
	أرجع retval;
}

ساكن _BMAP_INLINE_ errcode_t بلوک_tind_bmap(ext2_filsys fs, صحيح flags,
					       blk_t tind, حرف *بلوک_buf,
					       صحيح *بلوکها_alloc,
					       blk_t nr, blk_t *ret_blk)
{
	blk_t		b = 0;
	errcode_t	retval;
	blk_t		addr_در_هر_بلوک;

	addr_در_هر_بلوک = (blk_t) fs->blocksize >> 2;

	retval = بلوک_dind_bmap(fs, flags & ~BMAP_مجموعه, tind, بلوک_buf,
				 بلوکها_alloc, nr / addr_در_هر_بلوک, &b);
	إذا (retval)
		أرجع retval;
	retval = بلوک_ind_bmap(fs, flags, b, بلوک_buf, بلوکها_alloc,
				nr % addr_در_هر_بلوک, ret_blk);
	أرجع retval;
}

ساكن errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     حرف *بلوک_buf, صحيح bmap_flags, blk64_t بلوک,
			     صحيح *ret_flags, صحيح *بلوکها_alloc,
			     blk64_t *phys_blk);

ساكن errcode_t implied_cluster_alloc(ext2_filsys fs, ext2_ino_t ino,
				       بنية ext2_inode *inode,
				       ext2_extent_handle_t handle,
				       blk64_t lblk, blk64_t *phys_blk)
{
	blk64_t	پایه_بلوک, pblock = 0;
	صحيح i;

	إذا (!ext2fs_has_feature_bigalloc(fs->super))
		أرجع 0;

	پایه_بلوک = lblk & ~EXT2_FS_CLUSTER_ماسک(fs);
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
		إذا (پایه_بلوک + i == lblk)
			استمر;
		extent_bmap(fs, ino, inode, handle, 0, 0,
			    پایه_بلوک + i, 0, 0, &pblock);
		إذا (pblock)
			اكسر;
	}
	إذا (pblock == 0)
		أرجع 0;
	*phys_blk = pblock - i + (lblk - پایه_بلوک);
	أرجع 0;
}

/* Try to map a logical block to an already-allocated physical cluster. */
errcode_t ext2fs_map_cluster_بلوک(ext2_filsys fs, ext2_ino_t ino,
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
		اذهب بیرون;

	retval = implied_cluster_alloc(fs, ino, inode, handle, lblk, pblk);
	إذا (retval)
		اذهب out2;

out2:
	ext2fs_extent_free(handle);
بیرون:
	أرجع retval;
}

ساكن errcode_t extent_bmap(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_inode *inode,
			     ext2_extent_handle_t handle,
			     حرف *بلوک_buf, صحيح bmap_flags, blk64_t بلوک,
			     صحيح *ret_flags, صحيح *بلوکها_alloc,
			     blk64_t *phys_blk)
{
	بنية blk_alloc_ctx	alloc_ctx;
	بنية ext2fs_extent	extent;
	بیعلامت صحيح		جابه_جایی;
	errcode_t		retval = 0;
	blk64_t			blk64 = 0;
	صحيح			alloc = 0;
	صحيح			مجموعه_flags;

	مجموعه_flags = bmap_flags & BMAP_UNINIT ? EXT2_EXTENT_مجموعه_BMAP_UNINIT : 0;

	إذا (bmap_flags & BMAP_مجموعه) {
		retval = ext2fs_extent_مجموعه_bmap(handle, بلوک,
						*phys_blk, مجموعه_flags);
		أرجع retval;
	}
	retval = ext2fs_extent_goto(handle, بلوک);
	إذا (retval) {
		/* If the extent is not found, return phys_blk = 0 */
		إذا (retval == EXT2_ET_EXTENT_NOT_یافت) {
			extent.e_lblk = بلوک;
			اذهب got_بلوک;
		}
		أرجع retval;
	}
	retval = ext2fs_extent_get(handle, EXT2_EXTENT_CURRENT, &extent);
	إذا (retval)
		أرجع retval;
	جابه_جایی = بلوک - extent.e_lblk;
	إذا (بلوک >= extent.e_lblk && (جابه_جایی <= extent.e_len)) {
		*phys_blk = extent.e_pblk + جابه_جایی;
		إذا (ret_flags && extent.e_flags & EXT2_EXTENT_FLAGS_UNINIT)
			*ret_flags |= BMAP_RET_UNINIT;
	}
got_بلوک:
	إذا ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
		implied_cluster_alloc(fs, ino, inode, handle, بلوک, &blk64);
		إذا (blk64)
			اذهب مجموعه_extent;
		retval = extent_bmap(fs, ino, inode, handle, بلوک_buf,
				     0, بلوک-1, 0, بلوکها_alloc, &blk64);
		إذا (retval)
			blk64 = ext2fs_پیدا_کن_inode_goal(fs, ino, inode, بلوک);
		alloc_ctx.ino = ino;
		alloc_ctx.inode = inode;
		alloc_ctx.lblk = extent.e_lblk;
		alloc_ctx.flags = بلوک_ALLOC_داده;
		retval = ext2fs_alloc_block3(fs, blk64, بلوک_buf, &blk64,
					     &alloc_ctx);
		إذا (retval)
			أرجع retval;
		blk64 &= ~EXT2_FS_CLUSTER_ماسک(fs);
		blk64 += EXT2_FS_CLUSTER_ماسک(fs) & بلوک;
		alloc++;
	مجموعه_extent:
		retval = ext2fs_extent_مجموعه_bmap(handle, بلوک,
						blk64, مجموعه_flags);
		إذا (retval) {
			ext2fs_بلوک_alloc_stats2(fs, blk64, -1);
			أرجع retval;
		}
		/* Update inode after setting extent */
		retval = ext2fs_read_inode(fs, ino, inode);
		إذا (retval)
			أرجع retval;
		*بلوکها_alloc += alloc;
		*phys_blk = blk64;
	}
	أرجع 0;
}

صحيح ext2fs_file_بلوک_جابه_جایی_too_big(ext2_filsys fs,
				     بنية ext2_inode *inode,
				     blk64_t جابه_جایی)
{
	blk64_t addr_در_هر_بلوک, max_map_بلوک;

	/* Kernel seems to cut us off at 4294967294 blocks */
	إذا (جابه_جایی >= (1ULL << 32) - 1)
		أرجع 1;

	إذا (inode->i_flags & EXT4_EXTENTS_FL)
		أرجع 0;

	addr_در_هر_بلوک = fs->blocksize >> 2;
	max_map_بلوک = addr_در_هر_بلوک;
	max_map_بلوک += addr_در_هر_بلوک * addr_در_هر_بلوک;
	max_map_بلوک += addr_در_هر_بلوک * addr_در_هر_بلوک * addr_در_هر_بلوک;
	max_map_بلوک += 12;

	أرجع جابه_جایی >= max_map_بلوک;
}

errcode_t ext2fs_bmap2(ext2_filsys fs, ext2_ino_t ino, بنية ext2_inode *inode,
		       حرف *بلوک_buf, صحيح bmap_flags, blk64_t بلوک,
		       صحيح *ret_flags, blk64_t *phys_blk)
{
	بنية ext2_inode inode_buf;
	ext2_extent_handle_t handle = 0;
	blk_t addr_در_هر_بلوک;
	blk_t	b, blk32;
	blk64_t b64;
	حرف	*buf = 0;
	errcode_t	retval = 0;
	صحيح		بلوکها_alloc = 0, inode_dirty = 0;
	بنية blk_alloc_ctx alloc_ctx = {
		.ino	= ino,
		.inode	= inode,
		.lblk	= 0,
		.flags	= بلوک_ALLOC_داده,
	};

	إذا (!(bmap_flags & BMAP_مجموعه))
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
	addr_در_هر_بلوک = (blk_t) fs->blocksize >> 2;

	إذا (ext2fs_file_بلوک_جابه_جایی_too_big(fs, inode, بلوک))
		أرجع EXT2_ET_FILE_TOO_BIG;

	/*
	 * If an inode has inline data, that means that it doesn't have
	 * any blocks and we shouldn't map any blocks for it.
	 */
	إذا (inode->i_flags & EXT4_INLINE_داده_FL)
		أرجع EXT2_ET_INLINE_داده_NO_بلوک;

	إذا (!بلوک_buf) {
		retval = ext2fs_get_array(2, fs->blocksize, &buf);
		إذا (retval)
			أرجع retval;
		بلوک_buf = buf;
	}

	إذا (inode->i_flags & EXT4_EXTENTS_FL) {
		retval = ext2fs_extent_open2(fs, ino, inode, &handle);
		إذا (retval)
			اذهب انجامشده;
		retval = extent_bmap(fs, ino, inode, handle, بلوک_buf,
				     bmap_flags, بلوک, ret_flags,
				     &بلوکها_alloc, phys_blk);
		اذهب انجامشده;
	}

	إذا (بلوک < EXT2_NDIR_بلوکها) {
		إذا (bmap_flags & BMAP_مجموعه) {
			b = *phys_blk;
			inode_bmap(inode, بلوک) = b;
			inode_dirty++;
			اذهب انجامشده;
		}

		*phys_blk = inode_bmap(inode, بلوک);
		b = بلوک ? inode_bmap(inode, بلوک - 1) :
			    ext2fs_پیدا_کن_inode_goal(fs, ino, inode, بلوک);

		إذا ((*phys_blk == 0) && (bmap_flags & BMAP_ALLOC)) {
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, بلوک_buf, &b64,
						     &alloc_ctx);
			b = b64;
			إذا (retval)
				اذهب انجامشده;
			inode_bmap(inode, بلوک) = b;
			بلوکها_alloc++;
			*phys_blk = b;
		}
		اذهب انجامشده;
	}

	/* Indirect block */
	بلوک -= EXT2_NDIR_بلوکها;
	blk32 = *phys_blk;
	إذا (بلوک < addr_در_هر_بلوک) {
		b = inode_bmap(inode, EXT2_IND_بلوک);
		إذا (!b) {
			إذا (!(bmap_flags & BMAP_ALLOC)) {
				إذا (bmap_flags & BMAP_مجموعه)
					retval = EXT2_ET_مجموعه_BMAP_NO_IND;
				اذهب انجامشده;
			}

			b = inode_bmap(inode, EXT2_IND_بلوک-1);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, بلوک_buf, &b64,
						     &alloc_ctx);
			b = b64;
			إذا (retval)
				اذهب انجامشده;
			inode_bmap(inode, EXT2_IND_بلوک) = b;
			بلوکها_alloc++;
		}
		retval = بلوک_ind_bmap(fs, bmap_flags, b, بلوک_buf,
					&بلوکها_alloc, بلوک, &blk32);
		إذا (retval == 0)
			*phys_blk = blk32;
		اذهب انجامشده;
	}

	/* Doubly indirect block  */
	بلوک -= addr_در_هر_بلوک;
	إذا (بلوک < addr_در_هر_بلوک * addr_در_هر_بلوک) {
		b = inode_bmap(inode, EXT2_DIND_بلوک);
		إذا (!b) {
			إذا (!(bmap_flags & BMAP_ALLOC)) {
				إذا (bmap_flags & BMAP_مجموعه)
					retval = EXT2_ET_مجموعه_BMAP_NO_IND;
				اذهب انجامشده;
			}

			b = inode_bmap(inode, EXT2_IND_بلوک);
			b64 = b;
			retval = ext2fs_alloc_block3(fs, b64, بلوک_buf, &b64,
						     &alloc_ctx);
			b = b64;
			إذا (retval)
				اذهب انجامشده;
			inode_bmap(inode, EXT2_DIND_بلوک) = b;
			بلوکها_alloc++;
		}
		retval = بلوک_dind_bmap(fs, bmap_flags, b, بلوک_buf,
					 &بلوکها_alloc, بلوک, &blk32);
		إذا (retval == 0)
			*phys_blk = blk32;
		اذهب انجامشده;
	}

	/* Triply indirect block */
	بلوک -= addr_در_هر_بلوک * addr_در_هر_بلوک;
	b = inode_bmap(inode, EXT2_TIND_بلوک);
	إذا (!b) {
		إذا (!(bmap_flags & BMAP_ALLOC)) {
			إذا (bmap_flags & BMAP_مجموعه)
				retval = EXT2_ET_مجموعه_BMAP_NO_IND;
			اذهب انجامشده;
		}

		b = inode_bmap(inode, EXT2_DIND_بلوک);
		b64 = b;
		retval = ext2fs_alloc_block3(fs, b64, بلوک_buf, &b64,
					     &alloc_ctx);
		b = b64;
		إذا (retval)
			اذهب انجامشده;
		inode_bmap(inode, EXT2_TIND_بلوک) = b;
		بلوکها_alloc++;
	}
	retval = بلوک_tind_bmap(fs, bmap_flags, b, بلوک_buf,
				 &بلوکها_alloc, بلوک, &blk32);
	إذا (retval == 0)
		*phys_blk = blk32;
انجامشده:
	إذا (*phys_blk && retval == 0 && (bmap_flags & BMAP_ZERO))
		retval = ext2fs_zero_blocks2(fs, *phys_blk, 1, عدم, عدم);
	إذا (buf)
		ext2fs_free_mem(&buf);
	إذا (handle)
		ext2fs_extent_free(handle);
	إذا ((retval == 0) && (بلوکها_alloc || inode_dirty)) {
		ext2fs_iblk_جمع_کن_بلوکها(fs, inode, بلوکها_alloc);
		retval = ext2fs_write_inode(fs, ino, inode);
	}
	أرجع retval;
}

errcode_t ext2fs_bmap(ext2_filsys fs, ext2_ino_t ino, بنية ext2_inode *inode,
		      حرف *بلوک_buf, صحيح bmap_flags, blk_t بلوک,
		      blk_t *phys_blk)
{
	errcode_t ret;
	blk64_t	ret_blk = *phys_blk;

	ret = ext2fs_bmap2(fs, ino, inode, بلوک_buf, bmap_flags, بلوک,
			    0, &ret_blk);
	إذا (ret)
		أرجع ret;
	إذا (ret_blk >= ((دراز دراز) 1 << 32))
		أرجع EOVERFLOW;
	*phys_blk = ret_blk;
	أرجع 0;
}
