/*
 * closefs.c --- close an ext2 filesystem
 *
 * Copyright (C) 1993, 1994, 1995, 1996 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#समावेश "config.h"
#समावेश <stdio.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <time.h>
#समावेश <string.h>

#समावेश "ext2_fs.h"
#समावेश "ext2fsP.h"

अटल पूर्णांक test_मूल(अचिन्हित पूर्णांक a, अचिन्हित पूर्णांक b)
{
	जबतक (1) {
		अगर (a < b)
			वापस 0;
		अगर (a == b)
			वापस 1;
		अगर (a % b)
			वापस 0;
		a = a / b;
	}
}

पूर्णांक ext2fs_bg_has_super(ext2_filsys fs, dgrp_t group)
{
	अगर (group == 0)
		वापस 1;
	अगर (ext2fs_has_feature_sparse_super2(fs->super)) {
		अगर (group == fs->super->s_backup_bgs[0] ||
		    group == fs->super->s_backup_bgs[1])
			वापस 1;
		वापस 0;
	}
	अगर ((group <= 1) || !ext2fs_has_feature_sparse_super(fs->super))
		वापस 1;
	अगर (!(group & 1))
		वापस 0;
	अगर (test_मूल(group, 3) || (test_मूल(group, 5)) ||
	    test_मूल(group, 7))
		वापस 1;

	वापस 0;
}

/*
 * ext2fs_super_and_bgd_loc2()
 * @fs:			ext2 fs pointer
 * @group		given block group
 * @ret_super_blk:	if !NULL, returns super block location
 * @ret_old_desc_blk:	if !NULL, returns location of the old block
 *			group descriptor
 * @ret_new_desc_blk:	if !NULL, returns location of meta_bg block
 *			group descriptor
 * @ret_used_blks:	if !NULL, returns number of blocks used by
 *			super block and group_descriptors.
 *
 * Returns errcode_t of 0
 */
errcode_t ext2fs_super_and_bgd_loc2(ext2_filsys fs,
					   dgrp_t group,
					   blk64_t *ret_super_blk,
					   blk64_t *ret_old_desc_blk,
					   blk64_t *ret_new_desc_blk,
					   blk_t *ret_used_blks)
{
	blk64_t	group_block, super_blk = 0, old_desc_blk = 0, new_desc_blk = 0;
	अचिन्हित पूर्णांक meta_bg, meta_bg_आकार;
	blk_t	numblocks = 0;
	blk64_t old_desc_blocks;
	पूर्णांक	has_super;

	group_block = ext2fs_group_first_block2(fs, group);
	अगर (group_block == 0 && fs->blocksize == 1024)
		group_block = 1; /* Deal with 1024 blocksize && bigalloc */

	अगर (ext2fs_has_feature_meta_bg(fs->super))
		old_desc_blocks = fs->super->s_first_meta_bg;
	अन्यथा
		old_desc_blocks =
			fs->desc_blocks + fs->super->s_reserved_gdt_blocks;

	has_super = ext2fs_bg_has_super(fs, group);

	अगर (has_super) {
		super_blk = group_block;
		numblocks++;
	}
	meta_bg_आकार = EXT2_DESC_PER_BLOCK(fs->super);
	meta_bg = group / meta_bg_आकार;

	अगर (!ext2fs_has_feature_meta_bg(fs->super) ||
	    (meta_bg < fs->super->s_first_meta_bg)) {
		अगर (has_super) {
			old_desc_blk = group_block + 1;
			numblocks += old_desc_blocks;
		}
	} अन्यथा {
		अगर (((group % meta_bg_आकार) == 0) ||
		    ((group % meta_bg_आकार) == 1) ||
		    ((group % meta_bg_आकार) == (meta_bg_आकार-1))) {
			अगर (has_super)
				has_super = 1;
			new_desc_blk = group_block + has_super;
			numblocks++;
		}
	}

	अगर (ret_super_blk)
		*ret_super_blk = super_blk;
	अगर (ret_old_desc_blk)
		*ret_old_desc_blk = old_desc_blk;
	अगर (ret_new_desc_blk)
		*ret_new_desc_blk = new_desc_blk;
	अगर (ret_used_blks)
		*ret_used_blks = numblocks;

	वापस 0;
}

/*
 * This function returns the location of the superblock, block group
 * descriptors for a given block group.  It currently returns the
 * number of free blocks assuming that inode table and allocation
 * bitmaps will be in the group.  This is not necessarily the case
 * when the flex_bg feature is enabled, so callers should take care!
 * It was only really intended for use by mke2fs, and even there it's
 * not that useful.
 *
 * The ext2fs_super_and_bgd_loc2() function is 64-bit block number
 * capable and returns the number of blocks used by super block and
 * group descriptors.
 */
पूर्णांक ext2fs_super_and_bgd_loc(ext2_filsys fs,
			     dgrp_t group,
			     blk_t *ret_super_blk,
			     blk_t *ret_old_desc_blk,
			     blk_t *ret_new_desc_blk,
			     पूर्णांक *ret_meta_bg)
{
	blk64_t ret_super_blk2;
	blk64_t ret_old_desc_blk2;
	blk64_t ret_new_desc_blk2;
	blk_t ret_used_blks;
	blk_t numblocks;
	अचिन्हित पूर्णांक meta_bg_आकार;

	ext2fs_super_and_bgd_loc2(fs, group, &ret_super_blk2,
					&ret_old_desc_blk2,
					&ret_new_desc_blk2,
					&ret_used_blks);

	numblocks = ext2fs_group_blocks_गिनती(fs, group);

	अगर (ret_super_blk)
		*ret_super_blk = (blk_t)ret_super_blk2;
	अगर (ret_old_desc_blk)
		*ret_old_desc_blk = (blk_t)ret_old_desc_blk2;
	अगर (ret_new_desc_blk)
		*ret_new_desc_blk = (blk_t)ret_new_desc_blk2;
	अगर (ret_meta_bg) {
		meta_bg_आकार = EXT2_DESC_PER_BLOCK(fs->super);
		*ret_meta_bg = group / meta_bg_आकार;
	}

	numblocks -= 2 + fs->inode_blocks_per_group + ret_used_blks;

	वापस numblocks;
}

/*
 * This function forces out the primary superblock.  We need to only
 * write out those fields which we have changed, since if the
 * filesystem is mounted, it may have changed some of the other
 * fields.
 *
 * It takes as input a superblock which has already been byte swapped
 * (if necessary).
 *
 */
अटल errcode_t write_primary_superblock(ext2_filsys fs,
					  काष्ठा ext2_super_block *super)
{
	__u16		*old_super, *new_super;
	पूर्णांक		check_idx, write_idx, आकार;
	errcode_t	retval;

	अगर (!fs->io->manager->write_byte || !fs->orig_super) {
	fallback:
		io_channel_set_blksize(fs->io, SUPERBLOCK_OFFSET);
		retval = io_channel_write_blk64(fs->io, 1, -SUPERBLOCK_आकार,
					      super);
		io_channel_set_blksize(fs->io, fs->blocksize);
		वापस retval;
	}

	old_super = (__u16 *) fs->orig_super;
	new_super = (__u16 *) super;

	क्रम (check_idx = 0; check_idx < SUPERBLOCK_आकार/2; check_idx++) {
		अगर (old_super[check_idx] == new_super[check_idx])
			जारी;
		write_idx = check_idx;
		क्रम (check_idx++; check_idx < SUPERBLOCK_आकार/2; check_idx++)
			अगर (old_super[check_idx] == new_super[check_idx])
				अवरोध;
		आकार = 2 * (check_idx - write_idx);
#अगर 0
		म_लिखो("Writing %d bytes starting at %d\n",
		       आकार, write_idx*2);
#पूर्ण_अगर
		retval = io_channel_write_byte(fs->io,
			       SUPERBLOCK_OFFSET + (2 * write_idx), आकार,
					       new_super + write_idx);
		अगर (retval == EXT2_ET_UNIMPLEMENTED)
			जाओ fallback;
		अगर (retval)
			वापस retval;
	}
	स_नकल(fs->orig_super, super, SUPERBLOCK_आकार);
	वापस 0;
}


/*
 * Updates the revision to EXT2_DYNAMIC_REV
 */
व्योम ext2fs_update_dynamic_rev(ext2_filsys fs)
{
	काष्ठा ext2_super_block *sb = fs->super;

	अगर (sb->s_rev_level > EXT2_GOOD_OLD_REV)
		वापस;

	sb->s_rev_level = EXT2_DYNAMIC_REV;
	sb->s_first_ino = EXT2_GOOD_OLD_FIRST_INO;
	sb->s_inode_आकार = EXT2_GOOD_OLD_INODE_आकार;
	/* s_uuid is handled by e2fsck already */
	/* other fields should be left alone */
}

अटल errcode_t write_backup_super(ext2_filsys fs, dgrp_t group,
				    blk64_t group_block,
				    काष्ठा ext2_super_block *super_shadow)
{
	errcode_t retval;
	dgrp_t	sgrp = group;

	अगर (sgrp > ((1 << 16) - 1))
		sgrp = (1 << 16) - 1;

	super_shadow->s_block_group_nr = ext2fs_cpu_to_le16(sgrp);

	retval = ext2fs_superblock_csum_set(fs, super_shadow);
	अगर (retval)
		वापस retval;

	वापस io_channel_write_blk64(fs->io, group_block, -SUPERBLOCK_आकार,
				    super_shadow);
}

errcode_t ext2fs_flush(ext2_filsys fs)
{
	वापस ext2fs_flush2(fs, 0);
}

errcode_t ext2fs_flush2(ext2_filsys fs, पूर्णांक flags)
{
	dgrp_t		i;
	errcode_t	retval;
	अचिन्हित दीर्घ	fs_state;
	__u32		feature_incompat;
	काष्ठा ext2_super_block *super_shadow = 0;
	काष्ठा opaque_ext2_group_desc *group_shadow = 0;
#अगर_घोषित शब्दगण_BIGENDIAN
	काष्ठा ext2_group_desc *gdp;
	dgrp_t		j;
#पूर्ण_अगर
	अक्षर	*group_ptr;
	blk64_t	old_desc_blocks;
	काष्ठा ext2fs_numeric_progress_struct progress;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	अगर ((fs->flags & EXT2_FLAG_SUPER_ONLY) == 0 &&
	    !ext2fs_has_feature_journal_dev(fs->super) &&
	    fs->group_desc == शून्य)
		वापस EXT2_ET_NO_GDESC;

	fs_state = fs->super->s_state;
	feature_incompat = fs->super->s_feature_incompat;

	ext2fs_set_tstamp(fs->super, s_wtime, ext2fsP_get_time(fs));
	fs->super->s_block_group_nr = 0;

	/*
	 * If the write_bitmaps() function is present, call it to
	 * flush the bitmaps.  This is done this way so that a simple
	 * program that doesn't mess with the bitmaps doesn't need to
	 * drag in the bitmaps.c code.
	 *
	 * Bitmap checksums live in the group descriptor, so the
	 * bitmaps need to be written before the descriptors.
	 */
	अगर (fs->write_bitmaps) {
		retval = fs->write_bitmaps(fs);
		अगर (retval)
			जाओ errout;
	}

	/*
	 * Set the state of the FS to be non-valid.  (The state has
	 * already been backed up earlier, and will be restored after
	 * we write out the backup superblocks.)
	 */
	fs->super->s_state &= ~EXT2_VALID_FS;
	ext2fs_clear_feature_journal_needs_recovery(fs->super);

	/* Byte swap the superblock and the group descriptors if necessary */
#अगर_घोषित शब्दगण_BIGENDIAN
	retval = EXT2_ET_NO_MEMORY;
	retval = ext2fs_get_mem(SUPERBLOCK_आकार, &super_shadow);
	अगर (retval)
		जाओ errout;
	स_नकल(super_shadow, fs->super, माप(काष्ठा ext2_super_block));
	ext2fs_swap_super(super_shadow);

	अगर (((fs->flags & EXT2_FLAG_SUPER_ONLY) == 0) &&
	    !ext2fs_has_feature_journal_dev(fs->super)) {
		retval = ext2fs_get_array(fs->desc_blocks, fs->blocksize,
					  &group_shadow);
		अगर (retval)
			जाओ errout;
		स_नकल(group_shadow, fs->group_desc, (माप_प्रकार) fs->blocksize *
		       fs->desc_blocks);

		क्रम (j = 0; j < fs->group_desc_गिनती; j++) {
			gdp = ext2fs_group_desc(fs, group_shadow, j);
			ext2fs_swap_group_desc2(fs, gdp);
		}
	}
#अन्यथा
	super_shadow = fs->super;
	group_shadow = fs->group_desc;
#पूर्ण_अगर

	/*
	 * If this is an external journal device, don't write out the
	 * block group descriptors or any of the backup superblocks
	 */
	अगर (ext2fs_has_feature_journal_dev(fs->super))
		जाओ write_primary_superblock_only;

	/*
	 * Write out the master group descriptors, and the backup
	 * superblocks and group descriptors.
	 */
	group_ptr = (अक्षर *) group_shadow;
	अगर (ext2fs_has_feature_meta_bg(fs->super)) {
		old_desc_blocks = fs->super->s_first_meta_bg;
		अगर (old_desc_blocks > fs->desc_blocks)
			old_desc_blocks = fs->desc_blocks;
	} अन्यथा
		old_desc_blocks = fs->desc_blocks;

	अगर (fs->progress_ops && fs->progress_ops->init)
		(fs->progress_ops->init)(fs, &progress, शून्य,
					 fs->group_desc_गिनती);


	क्रम (i = 0; i < fs->group_desc_गिनती; i++) {
		blk64_t	super_blk, old_desc_blk, new_desc_blk;

		अगर (fs->progress_ops && fs->progress_ops->update)
			(fs->progress_ops->update)(fs, &progress, i);
		ext2fs_super_and_bgd_loc2(fs, i, &super_blk, &old_desc_blk,
					 &new_desc_blk, 0);

		अगर (!(fs->flags & EXT2_FLAG_MASTER_SB_ONLY) &&i && super_blk) {
			retval = write_backup_super(fs, i, super_blk,
						    super_shadow);
			अगर (retval)
				जाओ errout;
		}
		अगर (fs->flags & EXT2_FLAG_SUPER_ONLY)
			जारी;
		अगर ((old_desc_blk) &&
		    (!(fs->flags & EXT2_FLAG_MASTER_SB_ONLY) || (i == 0))) {
			retval = io_channel_write_blk64(fs->io,
			      old_desc_blk, old_desc_blocks, group_ptr);
			अगर (retval)
				जाओ errout;
		}
		अगर (new_desc_blk) {
			पूर्णांक meta_bg = i / EXT2_DESC_PER_BLOCK(fs->super);

			retval = io_channel_write_blk64(fs->io, new_desc_blk,
				1, group_ptr + (meta_bg*fs->blocksize));
			अगर (retval)
				जाओ errout;
		}
	}

	अगर (fs->progress_ops && fs->progress_ops->बंद)
		(fs->progress_ops->बंद)(fs, &progress, शून्य);

write_primary_superblock_only:
	/*
	 * Write out master superblock.  This has to be done
	 * separately, since it is located at a fixed location
	 * (SUPERBLOCK_OFFSET).  We flush all other pending changes
	 * out to disk first, just to avoid a race condition with an
	 * insy-tinsy window....
	 */

	fs->super->s_block_group_nr = 0;
	fs->super->s_state = fs_state;
	fs->super->s_feature_incompat = feature_incompat;
#अगर_घोषित शब्दगण_BIGENDIAN
	*super_shadow = *fs->super;
	ext2fs_swap_super(super_shadow);
#पूर्ण_अगर

	retval = ext2fs_superblock_csum_set(fs, super_shadow);
	अगर (retval)
		वापस retval;

	अगर (!(flags & EXT2_FLAG_FLUSH_NO_SYNC)) {
		retval = io_channel_flush(fs->io);
		अगर (retval)
			जाओ errout;
	}
	retval = write_primary_superblock(fs, super_shadow);
	अगर (retval)
		जाओ errout;

	fs->flags &= ~EXT2_FLAG_DIRTY;

	अगर (!(flags & EXT2_FLAG_FLUSH_NO_SYNC)) {
		retval = io_channel_flush(fs->io);
		अगर (retval)
			जाओ errout;
	}
errout:
	fs->super->s_state = fs_state;
#अगर_घोषित शब्दगण_BIGENDIAN
	अगर (super_shadow)
		ext2fs_free_mem(&super_shadow);
	अगर (group_shadow)
		ext2fs_free_mem(&group_shadow);
#पूर्ण_अगर
	वापस retval;
}

errcode_t ext2fs_close_free(ext2_filsys *fs_ptr)
{
	errcode_t ret;
	ext2_filsys fs = *fs_ptr;

	ret = ext2fs_close2(fs, 0);
	अगर (ret)
		ext2fs_free(fs);
	*fs_ptr = शून्य;
	वापस ret;
}

errcode_t ext2fs_close(ext2_filsys fs)
{
	वापस ext2fs_close2(fs, 0);
}

errcode_t ext2fs_close2(ext2_filsys fs, पूर्णांक flags)
{
	errcode_t	retval;
	पूर्णांक		meta_blks;
	io_stats stats = 0;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	अगर (fs->write_bitmaps) {
		retval = fs->write_bitmaps(fs);
		अगर (retval)
			वापस retval;
	}
	अगर (fs->super->s_kbytes_written &&
	    fs->io->manager->get_stats)
		fs->io->manager->get_stats(fs->io, &stats);
	अगर (stats && stats->bytes_written && (fs->flags & EXT2_FLAG_RW)) {
		fs->super->s_kbytes_written += stats->bytes_written >> 10;
		meta_blks = fs->desc_blocks + 1;
		अगर (!(fs->flags & EXT2_FLAG_SUPER_ONLY))
			fs->super->s_kbytes_written += meta_blks /
				(fs->blocksize / 1024);
		अगर ((fs->flags & EXT2_FLAG_DIRTY) == 0)
			fs->flags |= EXT2_FLAG_SUPER_ONLY | EXT2_FLAG_DIRTY;
	}
	अगर (fs->flags & EXT2_FLAG_DIRTY) {
		retval = ext2fs_flush2(fs, flags);
		अगर (retval)
			वापस retval;
	}

	retval = ext2fs_mmp_stop(fs);
	अगर (retval)
		वापस retval;

	ext2fs_free(fs);
	वापस 0;
}

