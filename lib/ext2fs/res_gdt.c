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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#شامل <time.h>
#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

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
	غیرعلامتی طویل طویل mult = 3;
	dgrp_t ret;

	اگر (fs && ext2fs_has_feature_sparse_super2(fs->super)) {
		اگر (*min == 1) {
			*min += 1;
			اگر (fs->super->s_backup_bgs[0])
				واپس fs->super->s_backup_bgs[0];
		}
		اگر (*min == 2) {
			*min += 1;
			اگر (fs->super->s_backup_bgs[1])
				واپس fs->super->s_backup_bgs[1];
		}

		واپس fs->گروہ_desc_گنتی;
	}

	اگر (fs && !ext2fs_has_feature_sparse_super(fs->super)) {
		ret = *min;
		*min += 1;

		واپس ret;
	}

	اگر (*five < *min) {
		min = five;
		mult = 5;
	}
	اگر (*seven < *min) {
		min = seven;
		mult = 7;
	}

	ret = *min;
	mult *= *min;
	اگر (mult > (dgrp_t)-1)
		*min = (dgrp_t)-1;
	ورنہ
		*min = mult;

	واپس ret;
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
	ساخت ext2_super_بلاک	*sb;
	ساخت ext2_inode	inode;
	__u32			*dindir_buf, *gdt_buf;
	غیرعلامتی طویل طویل	apb, inode_سائز;
	/* FIXME-64 - can't deal with extents */
	blk_t			dindir_blk, rsv_off, gdt_off, gdt_blk;
	صحیح			dindir_dirty = 0, inode_dirty = 0, sb_blk = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	sb = fs->super;

	retval = ext2fs_get_array(2, fs->blocksize, &dindir_buf);
	اگر (retval)
		واپس retval;
	gdt_buf = (__u32 *)((حرف *)dindir_buf + fs->blocksize);

	retval = ext2fs_read_inode(fs, EXT2_RESIZE_INO, &inode);
	اگر (retval)
		جاؤ برآمد_free;

	/*
	 * File systems with a blocksize of 1024 and bigalloc have
	 * sb->s_first_data_block of 0; yet the superblock is still at
	 * block #1.  We compensate for it here.
	 */
	sb_blk = sb->s_پہلا_ڈیٹا_بلاک;
	اگر (fs->blocksize == 1024 && sb_blk == 0)
		sb_blk = 1;

	/* Maximum possible file size (we only use double indirect blocks) */
	apb = EXT2_ADDR_فی_بلاک(sb);
	اگر ((dindir_blk = inode.i_بلاک[EXT2_DIND_بلاک])) {
#اگر_تعریف_شدہ RES_GDT_DEBUG
		لکھو("reading GDT dindir %u\n", dindir_blk);
#اگر_ختم
		retval = ext2fs_read_ind_بلاک(fs, dindir_blk, dindir_buf);
		اگر (retval)
			جاؤ برآمد_inode;
	} ورنہ {
		blk_t goal = sb_blk + fs->desc_بلاکیں +
			sb->s_reserved_gdt_بلاکیں + 2 +
			fs->inode_بلاکیں_فی_گروہ;

		retval = ext2fs_alloc_بلاک(fs, goal, 0, &dindir_blk);
		اگر (retval)
			جاؤ برآمد_free;
		inode.i_منوال = LINUX_S_IFREG | 0600;
		inode.i_links_گنتی = 1;
		inode.i_بلاک[EXT2_DIND_بلاک] = dindir_blk;
		ext2fs_iblk_سیٹ(fs, &inode, 1);
		memset(dindir_buf, 0, fs->blocksize);
#اگر_تعریف_شدہ RES_GDT_DEBUG
		لکھو("allocated GDT dindir %u\n", dindir_blk);
#اگر_ختم
		dindir_dirty = inode_dirty = 1;
		inode_سائز = apb*apb + apb + EXT2_NDIR_بلاکیں;
		inode_سائز *= fs->blocksize;
		retval = ext2fs_inode_سائز_سیٹ(fs, &inode, inode_سائز);
		اگر (retval)
			جاؤ برآمد_free;
		inode.i_ctime = ext2fs_P_get_وقت(fs);
	}

	برائے (rsv_off = 0, gdt_off = fs->desc_بلاکیں,
	     gdt_blk = sb_blk + 1 + fs->desc_بلاکیں;
	     rsv_off < sb->s_reserved_gdt_بلاکیں;
	     rsv_off++, gdt_off++, gdt_blk++) {
		dgrp_t three = 1, five = 5, seven = 7;
		dgrp_t grp, آخری = 0;
		صحیح gdt_dirty = 0;

		gdt_off %= apb;
		اگر (!dindir_buf[gdt_off]) {
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
			ext2fs_iblk_جوڑو_بلاکیں(fs, &inode, 1);
#اگر_تعریف_شدہ RES_GDT_DEBUG
			لکھو("added primary GDT block %u at %u[%u]\n",
			       gdt_blk, dindir_blk, gdt_off);
#اگر_ختم
		} ورنہ اگر (dindir_buf[gdt_off] == gdt_blk) {
#اگر_تعریف_شدہ RES_GDT_DEBUG
			لکھو("reading primary GDT block %u\n", gdt_blk);
#اگر_ختم
			retval = ext2fs_read_ind_بلاک(fs, gdt_blk, gdt_buf);
			اگر (retval)
				جاؤ برآمد_dindir;
		} ورنہ {
#اگر_تعریف_شدہ RES_GDT_DEBUG
			لکھو("bad primary GDT %u != %u at %u[%u]\n",
			       dindir_buf[gdt_off], gdt_blk,dindir_blk,gdt_off);
#اگر_ختم
			retval = EXT2_ET_RESIZE_INODE_CORRUPT;
			جاؤ برآمد_dindir;
		}

		جب ((grp = ext2fs_list_backups(fs, &three, &five, &seven)) <
		       fs->گروہ_desc_گنتی) {
			blk_t expect = gdt_blk + grp * sb->s_بلاکیں_فی_گروہ;

			اگر (!gdt_buf[آخری]) {
#اگر_تعریف_شدہ RES_GDT_DEBUG
				لکھو("added backup GDT %u grp %u@%u[%u]\n",
				       expect, grp, gdt_blk, آخری);
#اگر_ختم
				gdt_buf[آخری] = expect;
				ext2fs_iblk_جوڑو_بلاکیں(fs, &inode, 1);
				gdt_dirty = inode_dirty = 1;
			} ورنہ اگر (gdt_buf[آخری] != expect) {
#اگر_تعریف_شدہ RES_GDT_DEBUG
				لکھو("bad backup GDT %u != %u at %u[%u]\n",
				       gdt_buf[آخری], expect, gdt_blk, آخری);
#اگر_ختم
				retval = EXT2_ET_RESIZE_INODE_CORRUPT;
				جاؤ برآمد_dindir;
			}
			آخری++;
		}
		اگر (gdt_dirty) {
#اگر_تعریف_شدہ RES_GDT_DEBUG
			لکھو("writing primary GDT block %u\n", gdt_blk);
#اگر_ختم
			retval = ext2fs_write_ind_بلاک(fs, gdt_blk, gdt_buf);
			اگر (retval)
				جاؤ برآمد_dindir;
		}
	}

برآمد_dindir:
	اگر (dindir_dirty) {
		retval2 = ext2fs_write_ind_بلاک(fs, dindir_blk, dindir_buf);
		اگر (!retval)
			retval = retval2;
	}
برآمد_inode:
#اگر_تعریف_شدہ RES_GDT_DEBUG
	لکھو("inode.i_blocks = %u, i_size = %lu\n", inode.i_بلاکیں,
	       EXT2_I_سائز(&inode));
#اگر_ختم
	اگر (inode_dirty) {
		time_t now = ext2fs_P_get_وقت(fs);

		ext2fs_inode_xtime_سیٹ(&inode, i_atime, now);
		ext2fs_inode_xtime_سیٹ(&inode, i_mtime, now);
		retval2 = ext2fs_write_new_inode(fs, EXT2_RESIZE_INO, &inode);
		اگر (!retval)
			retval = retval2;
	}
برآمد_free:
	ext2fs_free_mem(&dindir_buf);
	واپس retval;
}

