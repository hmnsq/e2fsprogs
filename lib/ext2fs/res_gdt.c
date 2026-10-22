/*
 * res_gdt.c --- reserve blocks for growing the group descriptor table
 *               during online resizing.
 *
 * Copyright (C) 2002 Andreas Dilger
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

/*
 * Iterate through the groups which hold BACKUP superblock/GDT copies in an
 * ext3 filesystem.  The counters should be initialized to 1, 5, and 7 before
 * calling this for the first time.  In a sparse_super filesystem it will be
 * the sequence of powers of 3, 5, and 7: 1, 3, 5, 7, 9, 25, 27, 49, 81, ...
 * For a non-sparse filesystem it will be every group: 1, 2, 3, 4, ...
 * For a sparse_super2 filesystem there are two backups in specific groups.
 */
dgrp_t ext2fs_list_backups(ext2_filsys fs, dgrp_t *three,
			   dgrp_t *five, dgrp_t *seven)
{
	dgrp_t *min = three;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ mult = 3;
	dgrp_t ret;

	ਜੇ (fs && ext2fs_has_feature_sparse_super2(fs->super)) {
		ਜੇ (*min == 1) {
			*min += 1;
			ਜੇ (fs->super->s_backup_bgs[0])
				ਵਾਪਸ fs->super->s_backup_bgs[0];
		}
		ਜੇ (*min == 2) {
			*min += 1;
			ਜੇ (fs->super->s_backup_bgs[1])
				ਵਾਪਸ fs->super->s_backup_bgs[1];
		}

		ਵਾਪਸ fs->ਸਮੂਹ_desc_ਗਿਣਤੀ;
	}

	ਜੇ (fs && !ext2fs_has_feature_sparse_super(fs->super)) {
		ret = *min;
		*min += 1;

		ਵਾਪਸ ret;
	}

	ਜੇ (*five < *min) {
		min = five;
		mult = 5;
	}
	ਜੇ (*seven < *min) {
		min = seven;
		mult = 7;
	}

	ret = *min;
	mult *= *min;
	ਜੇ (mult > (dgrp_t)-1)
		*min = (dgrp_t)-1;
	ਨਹੀਂਤਾਂ
		*min = mult;

	ਵਾਪਸ ret;
}

/*
 * This code assumes that the reserved blocks have already been marked in-use
 * during ext2fs_initialize(), so that they are not allocated for other
 * uses before we can add them to the resize inode (which has to come
 * after the creation of the inode table).
 */
errcode_t ext2fs_create_resize_inode(ext2_filsys fs)
{
	errcode_t		retval, retval2;
	ਬਣਤਰ ext2_super_ਖੰਡ	*sb;
	ਬਣਤਰ ext2_inode	inode;
	__u32			*dindir_buf, *gdt_buf;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ	apb, inode_ਮਾਪ;
	/* FIXME-64 - can't deal with extents */
	blk_t			dindir_blk, rsv_off, gdt_off, gdt_blk;
	ਪੂਰਨ			dindir_dirty = 0, inode_dirty = 0, sb_blk = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	sb = fs->super;

	retval = ext2fs_get_array(2, fs->blocksize, &dindir_buf);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	gdt_buf = (__u32 *)((ਅੱਖਰ *)dindir_buf + fs->blocksize);

	retval = ext2fs_read_inode(fs, EXT2_RESIZE_INO, &inode);
	ਜੇ (retval)
		ਜਾਓ ਬਾਹਰਲਾ_free;

	/*
	 * File systems with a blocksize of 1024 and bigalloc have
	 * sb->s_first_data_block of 0; yet the superblock is still at
	 * block #1.  We compensate for it here.
	 */
	sb_blk = sb->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ;
	ਜੇ (fs->blocksize == 1024 && sb_blk == 0)
		sb_blk = 1;

	/* Maximum possible file size (we only use double indirect blocks) */
	apb = EXT2_ADDR_ਪ੍ਰਤੀ_ਖੰਡ(sb);
	ਜੇ ((dindir_blk = inode.i_ਖੰਡ[EXT2_DIND_ਖੰਡ])) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ RES_GDT_DEBUG
		ਲਿਖੋ("reading GDT dindir %u\n", dindir_blk);
#ਜੇ_ਅੰਤ
		retval = ext2fs_read_ind_ਖੰਡ(fs, dindir_blk, dindir_buf);
		ਜੇ (retval)
			ਜਾਓ ਬਾਹਰਲਾ_inode;
	} ਨਹੀਂਤਾਂ {
		blk_t goal = sb_blk + fs->desc_ਖੰਡਾਂ +
			sb->s_reserved_gdt_ਖੰਡਾਂ + 2 +
			fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ;

		retval = ext2fs_alloc_ਖੰਡ(fs, goal, 0, &dindir_blk);
		ਜੇ (retval)
			ਜਾਓ ਬਾਹਰਲਾ_free;
		inode.i_ਬਹੁਲਕ = LINUX_S_IFREG | 0600;
		inode.i_links_ਗਿਣਤੀ = 1;
		inode.i_ਖੰਡ[EXT2_DIND_ਖੰਡ] = dindir_blk;
		ext2fs_iblk_ਸੈੱਟ(fs, &inode, 1);
		memset(dindir_buf, 0, fs->blocksize);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ RES_GDT_DEBUG
		ਲਿਖੋ("allocated GDT dindir %u\n", dindir_blk);
#ਜੇ_ਅੰਤ
		dindir_dirty = inode_dirty = 1;
		inode_ਮਾਪ = apb*apb + apb + EXT2_NDIR_ਖੰਡਾਂ;
		inode_ਮਾਪ *= fs->blocksize;
		retval = ext2fs_inode_ਮਾਪ_ਸੈੱਟ(fs, &inode, inode_ਮਾਪ);
		ਜੇ (retval)
			ਜਾਓ ਬਾਹਰਲਾ_free;
		inode.i_ctime = ext2fs_P_get_ਸਮਾਂ(fs);
	}

	ਲਈ (rsv_off = 0, gdt_off = fs->desc_ਖੰਡਾਂ,
	     gdt_blk = sb_blk + 1 + fs->desc_ਖੰਡਾਂ;
	     rsv_off < sb->s_reserved_gdt_ਖੰਡਾਂ;
	     rsv_off++, gdt_off++, gdt_blk++) {
		dgrp_t three = 1, five = 5, seven = 7;
		dgrp_t grp, ਆਖਰੀ = 0;
		ਪੂਰਨ gdt_dirty = 0;

		gdt_off %= apb;
		ਜੇ (!dindir_buf[gdt_off]) {
			/* FIXME XXX XXX
			blk_t new_blk;

			retval = ext2fs_new_block(fs, gdt_blk, 0, &new_blk);
			if (retval)
				goto out_free;
			if (new_blk != gdt_blk) {
				// XXX free block
				retval = -1; // XXX
			}
			*/
			gdt_dirty = dindir_dirty = inode_dirty = 1;
			memset(gdt_buf, 0, fs->blocksize);
			dindir_buf[gdt_off] = gdt_blk;
			ext2fs_iblk_ਜੋੜੋ_ਖੰਡਾਂ(fs, &inode, 1);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ RES_GDT_DEBUG
			ਲਿਖੋ("added primary GDT block %u at %u[%u]\n",
			       gdt_blk, dindir_blk, gdt_off);
#ਜੇ_ਅੰਤ
		} ਨਹੀਂਤਾਂ ਜੇ (dindir_buf[gdt_off] == gdt_blk) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ RES_GDT_DEBUG
			ਲਿਖੋ("reading primary GDT block %u\n", gdt_blk);
#ਜੇ_ਅੰਤ
			retval = ext2fs_read_ind_ਖੰਡ(fs, gdt_blk, gdt_buf);
			ਜੇ (retval)
				ਜਾਓ ਬਾਹਰਲਾ_dindir;
		} ਨਹੀਂਤਾਂ {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ RES_GDT_DEBUG
			ਲਿਖੋ("bad primary GDT %u != %u at %u[%u]\n",
			       dindir_buf[gdt_off], gdt_blk,dindir_blk,gdt_off);
#ਜੇ_ਅੰਤ
			retval = EXT2_ET_RESIZE_INODE_CORRUPT;
			ਜਾਓ ਬਾਹਰਲਾ_dindir;
		}

		ਜਦੋਂ ((grp = ext2fs_list_backups(fs, &three, &five, &seven)) <
		       fs->ਸਮੂਹ_desc_ਗਿਣਤੀ) {
			blk_t expect = gdt_blk + grp * sb->s_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ;

			ਜੇ (!gdt_buf[ਆਖਰੀ]) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ RES_GDT_DEBUG
				ਲਿਖੋ("added backup GDT %u grp %u@%u[%u]\n",
				       expect, grp, gdt_blk, ਆਖਰੀ);
#ਜੇ_ਅੰਤ
				gdt_buf[ਆਖਰੀ] = expect;
				ext2fs_iblk_ਜੋੜੋ_ਖੰਡਾਂ(fs, &inode, 1);
				gdt_dirty = inode_dirty = 1;
			} ਨਹੀਂਤਾਂ ਜੇ (gdt_buf[ਆਖਰੀ] != expect) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ RES_GDT_DEBUG
				ਲਿਖੋ("bad backup GDT %u != %u at %u[%u]\n",
				       gdt_buf[ਆਖਰੀ], expect, gdt_blk, ਆਖਰੀ);
#ਜੇ_ਅੰਤ
				retval = EXT2_ET_RESIZE_INODE_CORRUPT;
				ਜਾਓ ਬਾਹਰਲਾ_dindir;
			}
			ਆਖਰੀ++;
		}
		ਜੇ (gdt_dirty) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ RES_GDT_DEBUG
			ਲਿਖੋ("writing primary GDT block %u\n", gdt_blk);
#ਜੇ_ਅੰਤ
			retval = ext2fs_write_ind_ਖੰਡ(fs, gdt_blk, gdt_buf);
			ਜੇ (retval)
				ਜਾਓ ਬਾਹਰਲਾ_dindir;
		}
	}

ਬਾਹਰਲਾ_dindir:
	ਜੇ (dindir_dirty) {
		retval2 = ext2fs_write_ind_ਖੰਡ(fs, dindir_blk, dindir_buf);
		ਜੇ (!retval)
			retval = retval2;
	}
ਬਾਹਰਲਾ_inode:
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ RES_GDT_DEBUG
	ਲਿਖੋ("inode.i_blocks = %u, i_size = %lu\n", inode.i_ਖੰਡਾਂ,
	       EXT2_I_ਮਾਪ(&inode));
#ਜੇ_ਅੰਤ
	ਜੇ (inode_dirty) {
		time_t now = ext2fs_P_get_ਸਮਾਂ(fs);

		ext2fs_inode_xtime_ਸੈੱਟ(&inode, i_atime, now);
		ext2fs_inode_xtime_ਸੈੱਟ(&inode, i_mtime, now);
		retval2 = ext2fs_write_new_inode(fs, EXT2_RESIZE_INO, &inode);
		ਜੇ (!retval)
			retval = retval2;
	}
ਬਾਹਰਲਾ_free:
	ext2fs_free_mem(&dindir_buf);
	ਵਾਪਸ retval;
}

