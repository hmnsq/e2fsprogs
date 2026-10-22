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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

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
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ mult = 3;
	dgrp_t ret;

	ಆದರೆ (fs && ext2fs_has_feature_sparse_super2(fs->super)) {
		ಆದರೆ (*min == 1) {
			*min += 1;
			ಆದರೆ (fs->super->s_backup_bgs[0])
				ಹಿಂತಿರುಗಿ fs->super->s_backup_bgs[0];
		}
		ಆದರೆ (*min == 2) {
			*min += 1;
			ಆದರೆ (fs->super->s_backup_bgs[1])
				ಹಿಂತಿರುಗಿ fs->super->s_backup_bgs[1];
		}

		ಹಿಂತಿರುಗಿ fs->ಗುಂಪು_desc_ಎಣಿಕೆ;
	}

	ಆದರೆ (fs && !ext2fs_has_feature_sparse_super(fs->super)) {
		ret = *min;
		*min += 1;

		ಹಿಂತಿರುಗಿ ret;
	}

	ಆದರೆ (*five < *min) {
		min = five;
		mult = 5;
	}
	ಆದರೆ (*seven < *min) {
		min = seven;
		mult = 7;
	}

	ret = *min;
	mult *= *min;
	ಆದರೆ (mult > (dgrp_t)-1)
		*min = (dgrp_t)-1;
	ಇಲ್ಲದಿದ್ದರೆ
		*min = mult;

	ಹಿಂತಿರುಗಿ ret;
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
	ರಚನೆ ext2_super_ಖಂಡ	*sb;
	ರಚನೆ ext2_inode	inode;
	__u32			*dindir_buf, *gdt_buf;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ	apb, inode_ಗಾತ್ರ;
	/* FIXME-64 - can't deal with extents */
	blk_t			dindir_blk, rsv_off, gdt_off, gdt_blk;
	ಪೂರ್ಣಾಂಕ			dindir_dirty = 0, inode_dirty = 0, sb_blk = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	sb = fs->super;

	retval = ext2fs_get_array(2, fs->blocksize, &dindir_buf);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	gdt_buf = (__u32 *)((ಅಕ್ಷರ *)dindir_buf + fs->blocksize);

	retval = ext2fs_read_inode(fs, EXT2_RESIZE_INO, &inode);
	ಆದರೆ (retval)
		ಹೋಗು ಹೊರಹೋಗು_free;

	/*
	 * File systems with a blocksize of 1024 and bigalloc have
	 * sb->s_first_data_block of 0; yet the superblock is still at
	 * block #1.  We compensate for it here.
	 */
	sb_blk = sb->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ;
	ಆದರೆ (fs->blocksize == 1024 && sb_blk == 0)
		sb_blk = 1;

	/* Maximum possible file size (we only use double indirect blocks) */
	apb = EXT2_ADDR_ಪ್ರತಿ_ಖಂಡ(sb);
	ಆದರೆ ((dindir_blk = inode.i_ಖಂಡ[EXT2_DIND_ಖಂಡ])) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ RES_GDT_DEBUG
		ಬರೆ("reading GDT dindir %u\n", dindir_blk);
#ಆದರೆ_ಅಂತ್ಯ
		retval = ext2fs_read_ind_ಖಂಡ(fs, dindir_blk, dindir_buf);
		ಆದರೆ (retval)
			ಹೋಗು ಹೊರಹೋಗು_inode;
	} ಇಲ್ಲದಿದ್ದರೆ {
		blk_t goal = sb_blk + fs->desc_ಖಂಡಗಳು +
			sb->s_reserved_gdt_ಖಂಡಗಳು + 2 +
			fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು;

		retval = ext2fs_alloc_ಖಂಡ(fs, goal, 0, &dindir_blk);
		ಆದರೆ (retval)
			ಹೋಗು ಹೊರಹೋಗು_free;
		inode.i_ಬಹುಲಕ = LINUX_S_IFREG | 0600;
		inode.i_links_ಎಣಿಕೆ = 1;
		inode.i_ಖಂಡ[EXT2_DIND_ಖಂಡ] = dindir_blk;
		ext2fs_iblk_ಗಣ(fs, &inode, 1);
		memset(dindir_buf, 0, fs->blocksize);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ RES_GDT_DEBUG
		ಬರೆ("allocated GDT dindir %u\n", dindir_blk);
#ಆದರೆ_ಅಂತ್ಯ
		dindir_dirty = inode_dirty = 1;
		inode_ಗಾತ್ರ = apb*apb + apb + EXT2_NDIR_ಖಂಡಗಳು;
		inode_ಗಾತ್ರ *= fs->blocksize;
		retval = ext2fs_inode_ಗಾತ್ರ_ಗಣ(fs, &inode, inode_ಗಾತ್ರ);
		ಆದರೆ (retval)
			ಹೋಗು ಹೊರಹೋಗು_free;
		inode.i_ctime = ext2fs_P_get_ಸಮಯ(fs);
	}

	ಗಾಗಿ (rsv_off = 0, gdt_off = fs->desc_ಖಂಡಗಳು,
	     gdt_blk = sb_blk + 1 + fs->desc_ಖಂಡಗಳು;
	     rsv_off < sb->s_reserved_gdt_ಖಂಡಗಳು;
	     rsv_off++, gdt_off++, gdt_blk++) {
		dgrp_t three = 1, five = 5, seven = 7;
		dgrp_t grp, ಕೊನೆಯ = 0;
		ಪೂರ್ಣಾಂಕ gdt_dirty = 0;

		gdt_off %= apb;
		ಆದರೆ (!dindir_buf[gdt_off]) {
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
			ext2fs_iblk_ಕೂಡಿಸು_ಖಂಡಗಳು(fs, &inode, 1);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ RES_GDT_DEBUG
			ಬರೆ("added primary GDT block %u at %u[%u]\n",
			       gdt_blk, dindir_blk, gdt_off);
#ಆದರೆ_ಅಂತ್ಯ
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (dindir_buf[gdt_off] == gdt_blk) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ RES_GDT_DEBUG
			ಬರೆ("reading primary GDT block %u\n", gdt_blk);
#ಆದರೆ_ಅಂತ್ಯ
			retval = ext2fs_read_ind_ಖಂಡ(fs, gdt_blk, gdt_buf);
			ಆದರೆ (retval)
				ಹೋಗು ಹೊರಹೋಗು_dindir;
		} ಇಲ್ಲದಿದ್ದರೆ {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ RES_GDT_DEBUG
			ಬರೆ("bad primary GDT %u != %u at %u[%u]\n",
			       dindir_buf[gdt_off], gdt_blk,dindir_blk,gdt_off);
#ಆದರೆ_ಅಂತ್ಯ
			retval = EXT2_ET_RESIZE_INODE_CORRUPT;
			ಹೋಗು ಹೊರಹೋಗು_dindir;
		}

		ಆಗ ((grp = ext2fs_list_backups(fs, &three, &five, &seven)) <
		       fs->ಗುಂಪು_desc_ಎಣಿಕೆ) {
			blk_t expect = gdt_blk + grp * sb->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು;

			ಆದರೆ (!gdt_buf[ಕೊನೆಯ]) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ RES_GDT_DEBUG
				ಬರೆ("added backup GDT %u grp %u@%u[%u]\n",
				       expect, grp, gdt_blk, ಕೊನೆಯ);
#ಆದರೆ_ಅಂತ್ಯ
				gdt_buf[ಕೊನೆಯ] = expect;
				ext2fs_iblk_ಕೂಡಿಸು_ಖಂಡಗಳು(fs, &inode, 1);
				gdt_dirty = inode_dirty = 1;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (gdt_buf[ಕೊನೆಯ] != expect) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ RES_GDT_DEBUG
				ಬರೆ("bad backup GDT %u != %u at %u[%u]\n",
				       gdt_buf[ಕೊನೆಯ], expect, gdt_blk, ಕೊನೆಯ);
#ಆದರೆ_ಅಂತ್ಯ
				retval = EXT2_ET_RESIZE_INODE_CORRUPT;
				ಹೋಗು ಹೊರಹೋಗು_dindir;
			}
			ಕೊನೆಯ++;
		}
		ಆದರೆ (gdt_dirty) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ RES_GDT_DEBUG
			ಬರೆ("writing primary GDT block %u\n", gdt_blk);
#ಆದರೆ_ಅಂತ್ಯ
			retval = ext2fs_write_ind_ಖಂಡ(fs, gdt_blk, gdt_buf);
			ಆದರೆ (retval)
				ಹೋಗು ಹೊರಹೋಗು_dindir;
		}
	}

ಹೊರಹೋಗು_dindir:
	ಆದರೆ (dindir_dirty) {
		retval2 = ext2fs_write_ind_ಖಂಡ(fs, dindir_blk, dindir_buf);
		ಆದರೆ (!retval)
			retval = retval2;
	}
ಹೊರಹೋಗು_inode:
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ RES_GDT_DEBUG
	ಬರೆ("inode.i_blocks = %u, i_size = %lu\n", inode.i_ಖಂಡಗಳು,
	       EXT2_I_ಗಾತ್ರ(&inode));
#ಆದರೆ_ಅಂತ್ಯ
	ಆದರೆ (inode_dirty) {
		time_t now = ext2fs_P_get_ಸಮಯ(fs);

		ext2fs_inode_xtime_ಗಣ(&inode, i_atime, now);
		ext2fs_inode_xtime_ಗಣ(&inode, i_mtime, now);
		retval2 = ext2fs_write_new_inode(fs, EXT2_RESIZE_INO, &inode);
		ಆದರೆ (!retval)
			retval = retval2;
	}
ಹೊರಹೋಗು_free:
	ext2fs_free_mem(&dindir_buf);
	ಹಿಂತಿರುಗಿ retval;
}

