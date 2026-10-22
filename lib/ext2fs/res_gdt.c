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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#ضمن <time.h>
#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

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
	بیعلامت دراز دراز mult = 3;
	dgrp_t ret;

	إذا (fs && ext2fs_has_feature_sparse_super2(fs->super)) {
		إذا (*min == 1) {
			*min += 1;
			إذا (fs->super->s_backup_bgs[0])
				أرجع fs->super->s_backup_bgs[0];
		}
		إذا (*min == 2) {
			*min += 1;
			إذا (fs->super->s_backup_bgs[1])
				أرجع fs->super->s_backup_bgs[1];
		}

		أرجع fs->گروه_desc_شمار;
	}

	إذا (fs && !ext2fs_has_feature_sparse_super(fs->super)) {
		ret = *min;
		*min += 1;

		أرجع ret;
	}

	إذا (*five < *min) {
		min = five;
		mult = 5;
	}
	إذا (*seven < *min) {
		min = seven;
		mult = 7;
	}

	ret = *min;
	mult *= *min;
	إذا (mult > (dgrp_t)-1)
		*min = (dgrp_t)-1;
	وإلا
		*min = mult;

	أرجع ret;
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
	بنية ext2_super_بلوک	*sb;
	بنية ext2_inode	inode;
	__u32			*dindir_buf, *gdt_buf;
	بیعلامت دراز دراز	apb, inode_اندازه;
	/* FIXME-64 - can't deal with extents */
	blk_t			dindir_blk, rsv_off, gdt_off, gdt_blk;
	صحيح			dindir_dirty = 0, inode_dirty = 0, sb_blk = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	sb = fs->super;

	retval = ext2fs_get_array(2, fs->blocksize, &dindir_buf);
	إذا (retval)
		أرجع retval;
	gdt_buf = (__u32 *)((حرف *)dindir_buf + fs->blocksize);

	retval = ext2fs_read_inode(fs, EXT2_RESIZE_INO, &inode);
	إذا (retval)
		اذهب بیرون_free;

	/*
	 * File systems with a blocksize of 1024 and bigalloc have
	 * sb->s_first_data_block of 0; yet the superblock is still at
	 * block #1.  We compensate for it here.
	 */
	sb_blk = sb->s_اولی_داده_بلوک;
	إذا (fs->blocksize == 1024 && sb_blk == 0)
		sb_blk = 1;

	/* Maximum possible file size (we only use double indirect blocks) */
	apb = EXT2_ADDR_در_هر_بلوک(sb);
	إذا ((dindir_blk = inode.i_بلوک[EXT2_DIND_بلوک])) {
#اگر_تعریف_شده RES_GDT_DEBUG
		چاپ_کن("reading GDT dindir %u\n", dindir_blk);
#پایان_اگر
		retval = ext2fs_read_ind_بلوک(fs, dindir_blk, dindir_buf);
		إذا (retval)
			اذهب بیرون_inode;
	} وإلا {
		blk_t goal = sb_blk + fs->desc_بلوکها +
			sb->s_reserved_gdt_بلوکها + 2 +
			fs->inode_بلوکها_در_هر_گروه;

		retval = ext2fs_alloc_بلوک(fs, goal, 0, &dindir_blk);
		إذا (retval)
			اذهب بیرون_free;
		inode.i_نما = LINUX_S_IFREG | 0600;
		inode.i_links_شمار = 1;
		inode.i_بلوک[EXT2_DIND_بلوک] = dindir_blk;
		ext2fs_iblk_مجموعه(fs, &inode, 1);
		memset(dindir_buf, 0, fs->blocksize);
#اگر_تعریف_شده RES_GDT_DEBUG
		چاپ_کن("allocated GDT dindir %u\n", dindir_blk);
#پایان_اگر
		dindir_dirty = inode_dirty = 1;
		inode_اندازه = apb*apb + apb + EXT2_NDIR_بلوکها;
		inode_اندازه *= fs->blocksize;
		retval = ext2fs_inode_اندازه_مجموعه(fs, &inode, inode_اندازه);
		إذا (retval)
			اذهب بیرون_free;
		inode.i_ctime = ext2fs_P_get_زمان(fs);
	}

	لكل (rsv_off = 0, gdt_off = fs->desc_بلوکها,
	     gdt_blk = sb_blk + 1 + fs->desc_بلوکها;
	     rsv_off < sb->s_reserved_gdt_بلوکها;
	     rsv_off++, gdt_off++, gdt_blk++) {
		dgrp_t three = 1, five = 5, seven = 7;
		dgrp_t grp, آخرین = 0;
		صحيح gdt_dirty = 0;

		gdt_off %= apb;
		إذا (!dindir_buf[gdt_off]) {
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
			ext2fs_iblk_جمع_کن_بلوکها(fs, &inode, 1);
#اگر_تعریف_شده RES_GDT_DEBUG
			چاپ_کن("added primary GDT block %u at %u[%u]\n",
			       gdt_blk, dindir_blk, gdt_off);
#پایان_اگر
		} وإلا إذا (dindir_buf[gdt_off] == gdt_blk) {
#اگر_تعریف_شده RES_GDT_DEBUG
			چاپ_کن("reading primary GDT block %u\n", gdt_blk);
#پایان_اگر
			retval = ext2fs_read_ind_بلوک(fs, gdt_blk, gdt_buf);
			إذا (retval)
				اذهب بیرون_dindir;
		} وإلا {
#اگر_تعریف_شده RES_GDT_DEBUG
			چاپ_کن("bad primary GDT %u != %u at %u[%u]\n",
			       dindir_buf[gdt_off], gdt_blk,dindir_blk,gdt_off);
#پایان_اگر
			retval = EXT2_ET_RESIZE_INODE_CORRUPT;
			اذهب بیرون_dindir;
		}

		طالما ((grp = ext2fs_list_backups(fs, &three, &five, &seven)) <
		       fs->گروه_desc_شمار) {
			blk_t expect = gdt_blk + grp * sb->s_بلوکها_در_هر_گروه;

			إذا (!gdt_buf[آخرین]) {
#اگر_تعریف_شده RES_GDT_DEBUG
				چاپ_کن("added backup GDT %u grp %u@%u[%u]\n",
				       expect, grp, gdt_blk, آخرین);
#پایان_اگر
				gdt_buf[آخرین] = expect;
				ext2fs_iblk_جمع_کن_بلوکها(fs, &inode, 1);
				gdt_dirty = inode_dirty = 1;
			} وإلا إذا (gdt_buf[آخرین] != expect) {
#اگر_تعریف_شده RES_GDT_DEBUG
				چاپ_کن("bad backup GDT %u != %u at %u[%u]\n",
				       gdt_buf[آخرین], expect, gdt_blk, آخرین);
#پایان_اگر
				retval = EXT2_ET_RESIZE_INODE_CORRUPT;
				اذهب بیرون_dindir;
			}
			آخرین++;
		}
		إذا (gdt_dirty) {
#اگر_تعریف_شده RES_GDT_DEBUG
			چاپ_کن("writing primary GDT block %u\n", gdt_blk);
#پایان_اگر
			retval = ext2fs_write_ind_بلوک(fs, gdt_blk, gdt_buf);
			إذا (retval)
				اذهب بیرون_dindir;
		}
	}

بیرون_dindir:
	إذا (dindir_dirty) {
		retval2 = ext2fs_write_ind_بلوک(fs, dindir_blk, dindir_buf);
		إذا (!retval)
			retval = retval2;
	}
بیرون_inode:
#اگر_تعریف_شده RES_GDT_DEBUG
	چاپ_کن("inode.i_blocks = %u, i_size = %lu\n", inode.i_بلوکها,
	       EXT2_I_اندازه(&inode));
#پایان_اگر
	إذا (inode_dirty) {
		time_t now = ext2fs_P_get_زمان(fs);

		ext2fs_inode_xtime_مجموعه(&inode, i_atime, now);
		ext2fs_inode_xtime_مجموعه(&inode, i_mtime, now);
		retval2 = ext2fs_write_new_inode(fs, EXT2_RESIZE_INO, &inode);
		إذا (!retval)
			retval = retval2;
	}
بیرون_free:
	ext2fs_free_mem(&dindir_buf);
	أرجع retval;
}

