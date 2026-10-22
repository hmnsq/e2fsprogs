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
	بنانشان ڊگهو ڊگهو mult = 3;
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

		أرجع fs->گروهه_desc_ڳڻپ;
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
	بنية ext2_super_بلاڪ	*sb;
	بنية ext2_inode	inode;
	__u32			*dindir_buf, *gdt_buf;
	بنانشان ڊگهو ڊگهو	apb, inode_سائيز;
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
		اذهب ٻاهريون_free;

	/*
	 * File systems with a blocksize of 1024 and bigalloc have
	 * sb->s_first_data_block of 0; yet the superblock is still at
	 * block #1.  We compensate for it here.
	 */
	sb_blk = sb->s_پهريون_معلومات_بلاڪ;
	إذا (fs->blocksize == 1024 && sb_blk == 0)
		sb_blk = 1;

	/* Maximum possible file size (we only use double indirect blocks) */
	apb = EXT2_ADDR_في_بلاڪ(sb);
	إذا ((dindir_blk = inode.i_بلاڪ[EXT2_DIND_بلاڪ])) {
#جيڪڏهن_وصف_ٿيل RES_GDT_DEBUG
		لکو("reading GDT dindir %u\n", dindir_blk);
#جيڪڏهن_پورو
		retval = ext2fs_read_ind_بلاڪ(fs, dindir_blk, dindir_buf);
		إذا (retval)
			اذهب ٻاهريون_inode;
	} وإلا {
		blk_t goal = sb_blk + fs->desc_بلاڪون +
			sb->s_reserved_gdt_بلاڪون + 2 +
			fs->inode_بلاڪون_في_گروهه;

		retval = ext2fs_alloc_بلاڪ(fs, goal, 0, &dindir_blk);
		إذا (retval)
			اذهب ٻاهريون_free;
		inode.i_گهڻائي = LINUX_S_IFREG | 0600;
		inode.i_links_ڳڻپ = 1;
		inode.i_بلاڪ[EXT2_DIND_بلاڪ] = dindir_blk;
		ext2fs_iblk_سيٽ(fs, &inode, 1);
		memset(dindir_buf, 0, fs->blocksize);
#جيڪڏهن_وصف_ٿيل RES_GDT_DEBUG
		لکو("allocated GDT dindir %u\n", dindir_blk);
#جيڪڏهن_پورو
		dindir_dirty = inode_dirty = 1;
		inode_سائيز = apb*apb + apb + EXT2_NDIR_بلاڪون;
		inode_سائيز *= fs->blocksize;
		retval = ext2fs_inode_سائيز_سيٽ(fs, &inode, inode_سائيز);
		إذا (retval)
			اذهب ٻاهريون_free;
		inode.i_ctime = ext2fs_P_get_وقت(fs);
	}

	لكل (rsv_off = 0, gdt_off = fs->desc_بلاڪون,
	     gdt_blk = sb_blk + 1 + fs->desc_بلاڪون;
	     rsv_off < sb->s_reserved_gdt_بلاڪون;
	     rsv_off++, gdt_off++, gdt_blk++) {
		dgrp_t three = 1, five = 5, seven = 7;
		dgrp_t grp, آخري = 0;
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
			ext2fs_iblk_جوڙيو_بلاڪون(fs, &inode, 1);
#جيڪڏهن_وصف_ٿيل RES_GDT_DEBUG
			لکو("added primary GDT block %u at %u[%u]\n",
			       gdt_blk, dindir_blk, gdt_off);
#جيڪڏهن_پورو
		} وإلا إذا (dindir_buf[gdt_off] == gdt_blk) {
#جيڪڏهن_وصف_ٿيل RES_GDT_DEBUG
			لکو("reading primary GDT block %u\n", gdt_blk);
#جيڪڏهن_پورو
			retval = ext2fs_read_ind_بلاڪ(fs, gdt_blk, gdt_buf);
			إذا (retval)
				اذهب ٻاهريون_dindir;
		} وإلا {
#جيڪڏهن_وصف_ٿيل RES_GDT_DEBUG
			لکو("bad primary GDT %u != %u at %u[%u]\n",
			       dindir_buf[gdt_off], gdt_blk,dindir_blk,gdt_off);
#جيڪڏهن_پورو
			retval = EXT2_ET_RESIZE_INODE_CORRUPT;
			اذهب ٻاهريون_dindir;
		}

		طالما ((grp = ext2fs_list_backups(fs, &three, &five, &seven)) <
		       fs->گروهه_desc_ڳڻپ) {
			blk_t expect = gdt_blk + grp * sb->s_بلاڪون_في_گروهه;

			إذا (!gdt_buf[آخري]) {
#جيڪڏهن_وصف_ٿيل RES_GDT_DEBUG
				لکو("added backup GDT %u grp %u@%u[%u]\n",
				       expect, grp, gdt_blk, آخري);
#جيڪڏهن_پورو
				gdt_buf[آخري] = expect;
				ext2fs_iblk_جوڙيو_بلاڪون(fs, &inode, 1);
				gdt_dirty = inode_dirty = 1;
			} وإلا إذا (gdt_buf[آخري] != expect) {
#جيڪڏهن_وصف_ٿيل RES_GDT_DEBUG
				لکو("bad backup GDT %u != %u at %u[%u]\n",
				       gdt_buf[آخري], expect, gdt_blk, آخري);
#جيڪڏهن_پورو
				retval = EXT2_ET_RESIZE_INODE_CORRUPT;
				اذهب ٻاهريون_dindir;
			}
			آخري++;
		}
		إذا (gdt_dirty) {
#جيڪڏهن_وصف_ٿيل RES_GDT_DEBUG
			لکو("writing primary GDT block %u\n", gdt_blk);
#جيڪڏهن_پورو
			retval = ext2fs_write_ind_بلاڪ(fs, gdt_blk, gdt_buf);
			إذا (retval)
				اذهب ٻاهريون_dindir;
		}
	}

ٻاهريون_dindir:
	إذا (dindir_dirty) {
		retval2 = ext2fs_write_ind_بلاڪ(fs, dindir_blk, dindir_buf);
		إذا (!retval)
			retval = retval2;
	}
ٻاهريون_inode:
#جيڪڏهن_وصف_ٿيل RES_GDT_DEBUG
	لکو("inode.i_blocks = %u, i_size = %lu\n", inode.i_بلاڪون,
	       EXT2_I_سائيز(&inode));
#جيڪڏهن_پورو
	إذا (inode_dirty) {
		time_t now = ext2fs_P_get_وقت(fs);

		ext2fs_inode_xtime_سيٽ(&inode, i_atime, now);
		ext2fs_inode_xtime_سيٽ(&inode, i_mtime, now);
		retval2 = ext2fs_write_new_inode(fs, EXT2_RESIZE_INO, &inode);
		إذا (!retval)
			retval = retval2;
	}
ٻاهريون_free:
	ext2fs_free_mem(&dindir_buf);
	أرجع retval;
}

