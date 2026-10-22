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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#समावेश <time.h>
#समावेश "ext2_fs.h"
#समावेश "ext2fsP.h"

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
	अचिन्हित दीर्घ दीर्घ mult = 3;
	dgrp_t ret;

	अगर (fs && ext2fs_has_feature_sparse_super2(fs->super)) {
		अगर (*min == 1) {
			*min += 1;
			अगर (fs->super->s_backup_bgs[0])
				वापस fs->super->s_backup_bgs[0];
		}
		अगर (*min == 2) {
			*min += 1;
			अगर (fs->super->s_backup_bgs[1])
				वापस fs->super->s_backup_bgs[1];
		}

		वापस fs->group_desc_गिनती;
	}

	अगर (fs && !ext2fs_has_feature_sparse_super(fs->super)) {
		ret = *min;
		*min += 1;

		वापस ret;
	}

	अगर (*five < *min) {
		min = five;
		mult = 5;
	}
	अगर (*seven < *min) {
		min = seven;
		mult = 7;
	}

	ret = *min;
	mult *= *min;
	अगर (mult > (dgrp_t)-1)
		*min = (dgrp_t)-1;
	अन्यथा
		*min = mult;

	वापस ret;
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
	काष्ठा ext2_super_block	*sb;
	काष्ठा ext2_inode	inode;
	__u32			*dindir_buf, *gdt_buf;
	अचिन्हित दीर्घ दीर्घ	apb, inode_आकार;
	/* FIXME-64 - can't deal with extents */
	blk_t			dindir_blk, rsv_off, gdt_off, gdt_blk;
	पूर्णांक			dindir_dirty = 0, inode_dirty = 0, sb_blk = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	sb = fs->super;

	retval = ext2fs_get_array(2, fs->blocksize, &dindir_buf);
	अगर (retval)
		वापस retval;
	gdt_buf = (__u32 *)((अक्षर *)dindir_buf + fs->blocksize);

	retval = ext2fs_read_inode(fs, EXT2_RESIZE_INO, &inode);
	अगर (retval)
		जाओ out_free;

	/*
	 * File systems with a blocksize of 1024 and bigalloc have
	 * sb->s_first_data_block of 0; yet the superblock is still at
	 * block #1.  We compensate for it here.
	 */
	sb_blk = sb->s_first_तथ्य_block;
	अगर (fs->blocksize == 1024 && sb_blk == 0)
		sb_blk = 1;

	/* Maximum possible file size (we only use double indirect blocks) */
	apb = EXT2_ADDR_PER_BLOCK(sb);
	अगर ((dindir_blk = inode.i_block[EXT2_DIND_BLOCK])) {
#अगर_घोषित RES_GDT_DEBUG
		म_लिखो("reading GDT dindir %u\n", dindir_blk);
#पूर्ण_अगर
		retval = ext2fs_read_ind_block(fs, dindir_blk, dindir_buf);
		अगर (retval)
			जाओ out_inode;
	} अन्यथा {
		blk_t goal = sb_blk + fs->desc_blocks +
			sb->s_reserved_gdt_blocks + 2 +
			fs->inode_blocks_per_group;

		retval = ext2fs_alloc_block(fs, goal, 0, &dindir_blk);
		अगर (retval)
			जाओ out_free;
		inode.i_mode = LINUX_S_IFREG | 0600;
		inode.i_links_गिनती = 1;
		inode.i_block[EXT2_DIND_BLOCK] = dindir_blk;
		ext2fs_iblk_set(fs, &inode, 1);
		स_रखो(dindir_buf, 0, fs->blocksize);
#अगर_घोषित RES_GDT_DEBUG
		म_लिखो("allocated GDT dindir %u\n", dindir_blk);
#पूर्ण_अगर
		dindir_dirty = inode_dirty = 1;
		inode_आकार = apb*apb + apb + EXT2_NDIR_BLOCKS;
		inode_आकार *= fs->blocksize;
		retval = ext2fs_inode_आकार_set(fs, &inode, inode_आकार);
		अगर (retval)
			जाओ out_free;
		inode.i_ctime = ext2fsP_get_time(fs);
	}

	क्रम (rsv_off = 0, gdt_off = fs->desc_blocks,
	     gdt_blk = sb_blk + 1 + fs->desc_blocks;
	     rsv_off < sb->s_reserved_gdt_blocks;
	     rsv_off++, gdt_off++, gdt_blk++) {
		dgrp_t three = 1, five = 5, seven = 7;
		dgrp_t grp, last = 0;
		पूर्णांक gdt_dirty = 0;

		gdt_off %= apb;
		अगर (!dindir_buf[gdt_off]) {
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
			स_रखो(gdt_buf, 0, fs->blocksize);
			dindir_buf[gdt_off] = gdt_blk;
			ext2fs_iblk_add_blocks(fs, &inode, 1);
#अगर_घोषित RES_GDT_DEBUG
			म_लिखो("added primary GDT block %u at %u[%u]\n",
			       gdt_blk, dindir_blk, gdt_off);
#पूर्ण_अगर
		} अन्यथा अगर (dindir_buf[gdt_off] == gdt_blk) {
#अगर_घोषित RES_GDT_DEBUG
			म_लिखो("reading primary GDT block %u\n", gdt_blk);
#पूर्ण_अगर
			retval = ext2fs_read_ind_block(fs, gdt_blk, gdt_buf);
			अगर (retval)
				जाओ out_dindir;
		} अन्यथा {
#अगर_घोषित RES_GDT_DEBUG
			म_लिखो("bad primary GDT %u != %u at %u[%u]\n",
			       dindir_buf[gdt_off], gdt_blk,dindir_blk,gdt_off);
#पूर्ण_अगर
			retval = EXT2_ET_RESIZE_INODE_CORRUPT;
			जाओ out_dindir;
		}

		जबतक ((grp = ext2fs_list_backups(fs, &three, &five, &seven)) <
		       fs->group_desc_गिनती) {
			blk_t expect = gdt_blk + grp * sb->s_blocks_per_group;

			अगर (!gdt_buf[last]) {
#अगर_घोषित RES_GDT_DEBUG
				म_लिखो("added backup GDT %u grp %u@%u[%u]\n",
				       expect, grp, gdt_blk, last);
#पूर्ण_अगर
				gdt_buf[last] = expect;
				ext2fs_iblk_add_blocks(fs, &inode, 1);
				gdt_dirty = inode_dirty = 1;
			} अन्यथा अगर (gdt_buf[last] != expect) {
#अगर_घोषित RES_GDT_DEBUG
				म_लिखो("bad backup GDT %u != %u at %u[%u]\n",
				       gdt_buf[last], expect, gdt_blk, last);
#पूर्ण_अगर
				retval = EXT2_ET_RESIZE_INODE_CORRUPT;
				जाओ out_dindir;
			}
			last++;
		}
		अगर (gdt_dirty) {
#अगर_घोषित RES_GDT_DEBUG
			म_लिखो("writing primary GDT block %u\n", gdt_blk);
#पूर्ण_अगर
			retval = ext2fs_write_ind_block(fs, gdt_blk, gdt_buf);
			अगर (retval)
				जाओ out_dindir;
		}
	}

out_dindir:
	अगर (dindir_dirty) {
		retval2 = ext2fs_write_ind_block(fs, dindir_blk, dindir_buf);
		अगर (!retval)
			retval = retval2;
	}
out_inode:
#अगर_घोषित RES_GDT_DEBUG
	म_लिखो("inode.i_blocks = %u, i_size = %lu\n", inode.i_blocks,
	       EXT2_I_आकार(&inode));
#पूर्ण_अगर
	अगर (inode_dirty) {
		समय_प्रकार now = ext2fsP_get_time(fs);

		ext2fs_inode_xtime_set(&inode, i_atime, now);
		ext2fs_inode_xtime_set(&inode, i_mtime, now);
		retval2 = ext2fs_write_new_inode(fs, EXT2_RESIZE_INO, &inode);
		अगर (!retval)
			retval = retval2;
	}
out_free:
	ext2fs_free_mem(&dindir_buf);
	वापस retval;
}

