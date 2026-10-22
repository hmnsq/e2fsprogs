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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <time.h>
#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fsP.h"

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
	గుర్తులేని దీర్ఘ దీర్ఘ mult = 3;
	dgrp_t ret;

	ఒకవేళ (fs && ext2fs_has_feature_sparse_super2(fs->super)) {
		ఒకవేళ (*min == 1) {
			*min += 1;
			ఒకవేళ (fs->super->s_backup_bgs[0])
				తిరిగి fs->super->s_backup_bgs[0];
		}
		ఒకవేళ (*min == 2) {
			*min += 1;
			ఒకవేళ (fs->super->s_backup_bgs[1])
				తిరిగి fs->super->s_backup_bgs[1];
		}

		తిరిగి fs->సమూహం_desc_లెక్క;
	}

	ఒకవేళ (fs && !ext2fs_has_feature_sparse_super(fs->super)) {
		ret = *min;
		*min += 1;

		తిరిగి ret;
	}

	ఒకవేళ (*five < *min) {
		min = five;
		mult = 5;
	}
	ఒకవేళ (*seven < *min) {
		min = seven;
		mult = 7;
	}

	ret = *min;
	mult *= *min;
	ఒకవేళ (mult > (dgrp_t)-1)
		*min = (dgrp_t)-1;
	లేకపోతే
		*min = mult;

	తిరిగి ret;
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
	నిర్మాణం ext2_super_ఖండం	*sb;
	నిర్మాణం ext2_inode	inode;
	__u32			*dindir_buf, *gdt_buf;
	గుర్తులేని దీర్ఘ దీర్ఘ	apb, inode_కొలత;
	/* FIXME-64 - can't deal with extents */
	blk_t			dindir_blk, rsv_off, gdt_off, gdt_blk;
	పూర్ణాంకం			dindir_dirty = 0, inode_dirty = 0, sb_blk = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	sb = fs->super;

	retval = ext2fs_get_array(2, fs->blocksize, &dindir_buf);
	ఒకవేళ (retval)
		తిరిగి retval;
	gdt_buf = (__u32 *)((అక్షరం *)dindir_buf + fs->blocksize);

	retval = ext2fs_read_inode(fs, EXT2_RESIZE_INO, &inode);
	ఒకవేళ (retval)
		వెళ్ళు బహిర్గతం_free;

	/*
	 * File systems with a blocksize of 1024 and bigalloc have
	 * sb->s_first_data_block of 0; yet the superblock is still at
	 * block #1.  We compensate for it here.
	 */
	sb_blk = sb->s_మొదటి_దత్తాంశం_ఖండం;
	ఒకవేళ (fs->blocksize == 1024 && sb_blk == 0)
		sb_blk = 1;

	/* Maximum possible file size (we only use double indirect blocks) */
	apb = EXT2_ADDR_చొప్పున_ఖండం(sb);
	ఒకవేళ ((dindir_blk = inode.i_ఖండం[EXT2_DIND_ఖండం])) {
#నిర్వచితమైతే RES_GDT_DEBUG
		రాయి("reading GDT dindir %u\n", dindir_blk);
#అయితే_ముగింపు
		retval = ext2fs_read_ind_ఖండం(fs, dindir_blk, dindir_buf);
		ఒకవేళ (retval)
			వెళ్ళు బహిర్గతం_inode;
	} లేకపోతే {
		blk_t goal = sb_blk + fs->desc_ఖండాలు +
			sb->s_reserved_gdt_ఖండాలు + 2 +
			fs->inode_ఖండాలు_చొప్పున_సమూహం;

		retval = ext2fs_alloc_ఖండం(fs, goal, 0, &dindir_blk);
		ఒకవేళ (retval)
			వెళ్ళు బహిర్గతం_free;
		inode.i_బహుళకం = LINUX_S_IFREG | 0600;
		inode.i_links_లెక్క = 1;
		inode.i_ఖండం[EXT2_DIND_ఖండం] = dindir_blk;
		ext2fs_iblk_సమితి(fs, &inode, 1);
		memset(dindir_buf, 0, fs->blocksize);
#నిర్వచితమైతే RES_GDT_DEBUG
		రాయి("allocated GDT dindir %u\n", dindir_blk);
#అయితే_ముగింపు
		dindir_dirty = inode_dirty = 1;
		inode_కొలత = apb*apb + apb + EXT2_NDIR_ఖండాలు;
		inode_కొలత *= fs->blocksize;
		retval = ext2fs_inode_కొలత_సమితి(fs, &inode, inode_కొలత);
		ఒకవేళ (retval)
			వెళ్ళు బహిర్గతం_free;
		inode.i_ctime = ext2fs_P_get_సమయం(fs);
	}

	కొరకు (rsv_off = 0, gdt_off = fs->desc_ఖండాలు,
	     gdt_blk = sb_blk + 1 + fs->desc_ఖండాలు;
	     rsv_off < sb->s_reserved_gdt_ఖండాలు;
	     rsv_off++, gdt_off++, gdt_blk++) {
		dgrp_t three = 1, five = 5, seven = 7;
		dgrp_t grp, చివరి = 0;
		పూర్ణాంకం gdt_dirty = 0;

		gdt_off %= apb;
		ఒకవేళ (!dindir_buf[gdt_off]) {
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
			ext2fs_iblk_కూడు_ఖండాలు(fs, &inode, 1);
#నిర్వచితమైతే RES_GDT_DEBUG
			రాయి("added primary GDT block %u at %u[%u]\n",
			       gdt_blk, dindir_blk, gdt_off);
#అయితే_ముగింపు
		} లేకపోతే ఒకవేళ (dindir_buf[gdt_off] == gdt_blk) {
#నిర్వచితమైతే RES_GDT_DEBUG
			రాయి("reading primary GDT block %u\n", gdt_blk);
#అయితే_ముగింపు
			retval = ext2fs_read_ind_ఖండం(fs, gdt_blk, gdt_buf);
			ఒకవేళ (retval)
				వెళ్ళు బహిర్గతం_dindir;
		} లేకపోతే {
#నిర్వచితమైతే RES_GDT_DEBUG
			రాయి("bad primary GDT %u != %u at %u[%u]\n",
			       dindir_buf[gdt_off], gdt_blk,dindir_blk,gdt_off);
#అయితే_ముగింపు
			retval = EXT2_ET_RESIZE_INODE_CORRUPT;
			వెళ్ళు బహిర్గతం_dindir;
		}

		అయితే ((grp = ext2fs_list_backups(fs, &three, &five, &seven)) <
		       fs->సమూహం_desc_లెక్క) {
			blk_t expect = gdt_blk + grp * sb->s_ఖండాలు_చొప్పున_సమూహం;

			ఒకవేళ (!gdt_buf[చివరి]) {
#నిర్వచితమైతే RES_GDT_DEBUG
				రాయి("added backup GDT %u grp %u@%u[%u]\n",
				       expect, grp, gdt_blk, చివరి);
#అయితే_ముగింపు
				gdt_buf[చివరి] = expect;
				ext2fs_iblk_కూడు_ఖండాలు(fs, &inode, 1);
				gdt_dirty = inode_dirty = 1;
			} లేకపోతే ఒకవేళ (gdt_buf[చివరి] != expect) {
#నిర్వచితమైతే RES_GDT_DEBUG
				రాయి("bad backup GDT %u != %u at %u[%u]\n",
				       gdt_buf[చివరి], expect, gdt_blk, చివరి);
#అయితే_ముగింపు
				retval = EXT2_ET_RESIZE_INODE_CORRUPT;
				వెళ్ళు బహిర్గతం_dindir;
			}
			చివరి++;
		}
		ఒకవేళ (gdt_dirty) {
#నిర్వచితమైతే RES_GDT_DEBUG
			రాయి("writing primary GDT block %u\n", gdt_blk);
#అయితే_ముగింపు
			retval = ext2fs_write_ind_ఖండం(fs, gdt_blk, gdt_buf);
			ఒకవేళ (retval)
				వెళ్ళు బహిర్గతం_dindir;
		}
	}

బహిర్గతం_dindir:
	ఒకవేళ (dindir_dirty) {
		retval2 = ext2fs_write_ind_ఖండం(fs, dindir_blk, dindir_buf);
		ఒకవేళ (!retval)
			retval = retval2;
	}
బహిర్గతం_inode:
#నిర్వచితమైతే RES_GDT_DEBUG
	రాయి("inode.i_blocks = %u, i_size = %lu\n", inode.i_ఖండాలు,
	       EXT2_I_కొలత(&inode));
#అయితే_ముగింపు
	ఒకవేళ (inode_dirty) {
		time_t now = ext2fs_P_get_సమయం(fs);

		ext2fs_inode_xtime_సమితి(&inode, i_atime, now);
		ext2fs_inode_xtime_సమితి(&inode, i_mtime, now);
		retval2 = ext2fs_write_new_inode(fs, EXT2_RESIZE_INO, &inode);
		ఒకవేళ (!retval)
			retval = retval2;
	}
బహిర్గతం_free:
	ext2fs_free_mem(&dindir_buf);
	తిరిగి retval;
}

