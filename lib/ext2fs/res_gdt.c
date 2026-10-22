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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

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
	अचिह्नित दीर्घ दीर्घ mult = 3;
	dgrp_t ret;

	यदि (fs && ext2fs_has_feature_sparse_super2(fs->super)) {
		यदि (*min == 1) {
			*min += 1;
			यदि (fs->super->s_backup_bgs[0])
				प्रत्यागम fs->super->s_backup_bgs[0];
		}
		यदि (*min == 2) {
			*min += 1;
			यदि (fs->super->s_backup_bgs[1])
				प्रत्यागम fs->super->s_backup_bgs[1];
		}

		प्रत्यागम fs->गणः_desc_गणनम्;
	}

	यदि (fs && !ext2fs_has_feature_sparse_super(fs->super)) {
		ret = *min;
		*min += 1;

		प्रत्यागम ret;
	}

	यदि (*five < *min) {
		min = five;
		mult = 5;
	}
	यदि (*seven < *min) {
		min = seven;
		mult = 7;
	}

	ret = *min;
	mult *= *min;
	यदि (mult > (dgrp_t)-1)
		*min = (dgrp_t)-1;
	अन्यथा
		*min = mult;

	प्रत्यागम ret;
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
	संरचना ext2_super_खण्डम्	*sb;
	संरचना ext2_inode	inode;
	__u32			*dindir_buf, *gdt_buf;
	अचिह्नित दीर्घ दीर्घ	apb, inode_आकार;
	/* FIXME-64 - can't deal with extents */
	blk_t			dindir_blk, rsv_off, gdt_off, gdt_blk;
	पूर्णाङ्क			dindir_dirty = 0, inode_dirty = 0, sb_blk = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	sb = fs->super;

	retval = ext2fs_get_array(2, fs->blocksize, &dindir_buf);
	यदि (retval)
		प्रत्यागम retval;
	gdt_buf = (__u32 *)((वर्ण *)dindir_buf + fs->blocksize);

	retval = ext2fs_read_inode(fs, EXT2_RESIZE_INO, &inode);
	यदि (retval)
		गच्छ बहिर्गमनम्_free;

	/*
	 * File systems with a blocksize of 1024 and bigalloc have
	 * sb->s_first_data_block of 0; yet the superblock is still at
	 * block #1.  We compensate for it here.
	 */
	sb_blk = sb->s_first_दत्त_खण्डम्;
	यदि (fs->blocksize == 1024 && sb_blk == 0)
		sb_blk = 1;

	/* Maximum possible file size (we only use double indirect blocks) */
	apb = EXT2_ADDR_प्रत्येकम्_खण्डम्(sb);
	यदि ((dindir_blk = inode.i_खण्डम्[EXT2_DIND_खण्डम्])) {
#यदि_परिभाषितम् RES_GDT_DEBUG
		प्रारूप_लिख("reading GDT dindir %u\n", dindir_blk);
#यदि_अन्तः
		retval = ext2fs_read_ind_खण्डम्(fs, dindir_blk, dindir_buf);
		यदि (retval)
			गच्छ बहिर्गमनम्_inode;
	} अन्यथा {
		blk_t goal = sb_blk + fs->desc_खण्डानि +
			sb->s_reserved_gdt_खण्डानि + 2 +
			fs->inode_खण्डानि_प्रत्येकम्_गणः;

		retval = ext2fs_alloc_खण्डम्(fs, goal, 0, &dindir_blk);
		यदि (retval)
			गच्छ बहिर्गमनम्_free;
		inode.i_बहुलकम् = LINUX_S_IFREG | 0600;
		inode.i_links_गणनम् = 1;
		inode.i_खण्डम्[EXT2_DIND_खण्डम्] = dindir_blk;
		ext2fs_iblk_समुच्चयः(fs, &inode, 1);
		स्मृतिं_पूरय(dindir_buf, 0, fs->blocksize);
#यदि_परिभाषितम् RES_GDT_DEBUG
		प्रारूप_लिख("allocated GDT dindir %u\n", dindir_blk);
#यदि_अन्तः
		dindir_dirty = inode_dirty = 1;
		inode_आकार = apb*apb + apb + EXT2_NDIR_खण्डानि;
		inode_आकार *= fs->blocksize;
		retval = ext2fs_inode_आकार_समुच्चयः(fs, &inode, inode_आकार);
		यदि (retval)
			गच्छ बहिर्गमनम्_free;
		inode.i_ctime = ext2fsP_get_time(fs);
	}

	प्रति (rsv_off = 0, gdt_off = fs->desc_खण्डानि,
	     gdt_blk = sb_blk + 1 + fs->desc_खण्डानि;
	     rsv_off < sb->s_reserved_gdt_खण्डानि;
	     rsv_off++, gdt_off++, gdt_blk++) {
		dgrp_t three = 1, five = 5, seven = 7;
		dgrp_t grp, चरमः = 0;
		पूर्णाङ्क gdt_dirty = 0;

		gdt_off %= apb;
		यदि (!dindir_buf[gdt_off]) {
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
			स्मृतिं_पूरय(gdt_buf, 0, fs->blocksize);
			dindir_buf[gdt_off] = gdt_blk;
			ext2fs_iblk_सङ्कलय_खण्डानि(fs, &inode, 1);
#यदि_परिभाषितम् RES_GDT_DEBUG
			प्रारूप_लिख("added primary GDT block %u at %u[%u]\n",
			       gdt_blk, dindir_blk, gdt_off);
#यदि_अन्तः
		} अन्यथा यदि (dindir_buf[gdt_off] == gdt_blk) {
#यदि_परिभाषितम् RES_GDT_DEBUG
			प्रारूप_लिख("reading primary GDT block %u\n", gdt_blk);
#यदि_अन्तः
			retval = ext2fs_read_ind_खण्डम्(fs, gdt_blk, gdt_buf);
			यदि (retval)
				गच्छ बहिर्गमनम्_dindir;
		} अन्यथा {
#यदि_परिभाषितम् RES_GDT_DEBUG
			प्रारूप_लिख("bad primary GDT %u != %u at %u[%u]\n",
			       dindir_buf[gdt_off], gdt_blk,dindir_blk,gdt_off);
#यदि_अन्तः
			retval = EXT2_ET_RESIZE_INODE_CORRUPT;
			गच्छ बहिर्गमनम्_dindir;
		}

		यावत् ((grp = ext2fs_list_backups(fs, &three, &five, &seven)) <
		       fs->गणः_desc_गणनम्) {
			blk_t expect = gdt_blk + grp * sb->s_खण्डानि_प्रत्येकम्_गणः;

			यदि (!gdt_buf[चरमः]) {
#यदि_परिभाषितम् RES_GDT_DEBUG
				प्रारूप_लिख("added backup GDT %u grp %u@%u[%u]\n",
				       expect, grp, gdt_blk, चरमः);
#यदि_अन्तः
				gdt_buf[चरमः] = expect;
				ext2fs_iblk_सङ्कलय_खण्डानि(fs, &inode, 1);
				gdt_dirty = inode_dirty = 1;
			} अन्यथा यदि (gdt_buf[चरमः] != expect) {
#यदि_परिभाषितम् RES_GDT_DEBUG
				प्रारूप_लिख("bad backup GDT %u != %u at %u[%u]\n",
				       gdt_buf[चरमः], expect, gdt_blk, चरमः);
#यदि_अन्तः
				retval = EXT2_ET_RESIZE_INODE_CORRUPT;
				गच्छ बहिर्गमनम्_dindir;
			}
			चरमः++;
		}
		यदि (gdt_dirty) {
#यदि_परिभाषितम् RES_GDT_DEBUG
			प्रारूप_लिख("writing primary GDT block %u\n", gdt_blk);
#यदि_अन्तः
			retval = ext2fs_write_ind_खण्डम्(fs, gdt_blk, gdt_buf);
			यदि (retval)
				गच्छ बहिर्गमनम्_dindir;
		}
	}

बहिर्गमनम्_dindir:
	यदि (dindir_dirty) {
		retval2 = ext2fs_write_ind_खण्डम्(fs, dindir_blk, dindir_buf);
		यदि (!retval)
			retval = retval2;
	}
बहिर्गमनम्_inode:
#यदि_परिभाषितम् RES_GDT_DEBUG
	प्रारूप_लिख("inode.i_blocks = %u, i_size = %lu\n", inode.i_खण्डानि,
	       EXT2_I_आकार(&inode));
#यदि_अन्तः
	यदि (inode_dirty) {
		time_t now = ext2fsP_get_time(fs);

		ext2fs_inode_xtime_समुच्चयः(&inode, i_atime, now);
		ext2fs_inode_xtime_समुच्चयः(&inode, i_mtime, now);
		retval2 = ext2fs_write_new_inode(fs, EXT2_RESIZE_INO, &inode);
		यदि (!retval)
			retval = retval2;
	}
बहिर्गमनम्_free:
	ext2fs_free_mem(&dindir_buf);
	प्रत्यागम retval;
}

