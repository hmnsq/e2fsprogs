/*
 * mkjournal.c --- make a journal for a filesystem
 *
 * Copyright (C) 2000 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <assert.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಯದಿ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#endif
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಯದಿ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#endif
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif
#ಯದಿ HAVE_SYS_IOCTL_H
#ಅನ್ತರ್ಭಾವ <sys/ioctl.h>
#endif
#ಯದಿ HAVE_NETINET_IN_H
#ಅನ್ತರ್ಭಾವ <netinet/in.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "e2p/e2p.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

#ಅನ್ತರ್ಭಾವ "kernel-jbd.h"

/*
 * This function automatically sets up the journal superblock and
 * returns it as an allocated block.
 */
errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
					   ಸಂರಚನಾ ext2fs_journal_params *jparams,
					   ಪೂರ್ಣಾಙ್ಕ flags, ವರ್ಣ **ret_jsb)
{
	errcode_t		retval;
	journal_superblock_t	*jsb;

	ಯದಿ (jparams->ಅಂಕೆ_journal_blocks < JBD2_MIN_JOURNAL_BLOCKS)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_JOURNAL_TOO_SMALL;

	ಯದಿ ((retval = ext2fs_get_mem(fs->blocksize, &jsb)))
		ಪ್ರತ್ಯಾಗಮ retval;

	memset (jsb, 0, fs->blocksize);

	jsb->s_header.h_magic = htonl(JBD2_MAGIC_ಸಂಖ್ಯೆ);
	ಯದಿ (flags & EXT2_MKJOURNAL_V1_SUPER)
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V1);
	ಅನ್ಯಥಾ
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V2);
	jsb->s_blocksize = htonl(fs->blocksize);
	jsb->s_maxlen = htonl(jparams->ಅಂಕೆ_journal_blocks + jparams->ಅಂಕೆ_fc_blocks);
	jsb->s_nr_users = htonl(1);
	jsb->s_first = htonl(1);
	jsb->s_sequence = htonl(1);
	jsb->s_ಅಂಕೆ_fc_blks = htonl(jparams->ಅಂಕೆ_fc_blocks);
	memcpy(jsb->s_uuid, fs->super->s_uuid, ಪರಿಮಾಣ(fs->super->s_uuid));
	/*
	 * If we're creating an external journal device, we need to
	 * adjust these fields.
	 */
	ಯದಿ (ext2fs_has_feature_journal_dev(fs->super)) {
		jsb->s_nr_users = 0;
		jsb->s_first = htonl(ext2fs_journal_sb_start(fs->blocksize) + 1);
	}

	*ret_jsb = (ವರ್ಣ *) jsb;
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_create_journal_superblock(ext2_filsys fs, __u32 ಅಂಕೆ_blocks,
					ಪೂರ್ಣಾಙ್ಕ flags, ವರ್ಣ **ret_sb)
{
	ಸಂರಚನಾ ext2fs_journal_params jparams;

	jparams.ಅಂಕೆ_journal_blocks = ಅಂಕೆ_blocks;
	jparams.ಅಂಕೆ_fc_blocks = 0;

	ಪ್ರತ್ಯಾಗಮ ext2fs_create_journal_superblock2(fs, &jparams, flags, ret_sb);
}

/*
 * This function writes a journal using POSIX routines.  It is used
 * for creating external journals and creating journals on live
 * filesystems.
 */
ಸ್ಥಿರ errcode_t write_journal_file(ext2_filsys fs, ವರ್ಣ *filename,
				    ಸಂರಚನಾ ext2fs_journal_params *jparams,
				    ಪೂರ್ಣಾಙ್ಕ flags)
{
	errcode_t	retval;
	ವರ್ಣ		*buf = 0;
	ಪೂರ್ಣಾಙ್ಕ		fd, ret_size;
	blk_t		i;

	ಯದಿ ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		ಪ್ರತ್ಯಾಗಮ retval;

	/* Open the device or journal file */
	ಯದಿ ((fd = open(filename, O_WRONLY)) < 0) {
		retval = errno;
		ಗಚ್ಛ errfree;
	}

	/* Write the superblock out */
	retval = EXT2_ET_SHORT_WRITE;
	ret_size = ಲಿಖ(fd, buf, fs->blocksize);
	ಯದಿ (ret_size < 0) {
		retval = errno;
		ಗಚ್ಛ errout;
	}
	ಯದಿ (ret_size != (ಪೂರ್ಣಾಙ್ಕ) fs->blocksize)
		ಗಚ್ಛ errout;
	memset(buf, 0, fs->blocksize);

	ಯದಿ (flags & EXT2_MKJOURNAL_LAZYINIT)
		ಗಚ್ಛ success;

	ಪ್ರತಿ (i = 1; i < jparams->ಅಂಕೆ_journal_blocks + jparams->ಅಂಕೆ_fc_blocks; i++) {
		ret_size = ಲಿಖ(fd, buf, fs->blocksize);
		ಯದಿ (ret_size < 0) {
			retval = errno;
			ಗಚ್ಛ errout;
		}
		ಯದಿ (ret_size != (ಪೂರ್ಣಾಙ್ಕ) fs->blocksize)
			ಗಚ್ಛ errout;
	}

success:
	retval = 0;
errout:
	close(fd);
errfree:
	ext2fs_free_mem(&buf);
	ಪ್ರತ್ಯಾಗಮ retval;
}

/*
 * Convenience function which zeros out _num_ blocks starting at
 * _blk_.  In case of an error, the details of the error is returned
 * via _ret_blk_ and _ret_count_ if they are non-NULL pointers.
 * Returns 0 on success, and an error code on an error.
 *
 * As a special case, if the first argument is NULL, then it will
 * attempt to free the static zeroizing buffer.  (This is to keep
 * programs that check for memory leaks happy.)
 */
#define MAX_STRIDE_LENGTH (4194304 / (ಪೂರ್ಣಾಙ್ಕ) fs->blocksize)
errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ,
			      blk64_t *ret_blk, ಪೂರ್ಣಾಙ್ಕ *ret_ಎಣಿಕೆ)
{
	ಪೂರ್ಣಾಙ್ಕ		j, ಎಣಿಕೆ;
	ಸ್ಥಿರ ಶೂನ್ಯ	*buf;
	ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ	stride_length;
	errcode_t	retval;

	/* If fs is null, clean up the static buffer and return */
	ಯದಿ (!fs) {
		ಯದಿ (buf) {
			ಮುಕ್ತ(buf);
			buf = 0;
			stride_length = 0;
		}
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	/* Deal with zeroing less than 1 block */
	ಯದಿ (ಅಂಕೆ <= 0)
		ಪ್ರತ್ಯಾಗಮ 0;

	/* Try a zero out command, if supported */
	retval = io_channel_zeroout(fs->io, blk, ಅಂಕೆ);
	ಯದಿ (retval == 0)
		ಪ್ರತ್ಯಾಗಮ 0;

	/* Allocate the zeroizing buffer if necessary */
	ಯದಿ (ಅಂಕೆ > stride_length && stride_length < MAX_STRIDE_LENGTH) {
		ಶೂನ್ಯ *p;
		ಪೂರ್ಣಾಙ್ಕ new_stride = ಅಂಕೆ;

		ಯದಿ (new_stride > MAX_STRIDE_LENGTH)
			new_stride = MAX_STRIDE_LENGTH;
		p = realloc(buf, fs->blocksize * new_stride);
		ಯದಿ (!p)
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_MEMORY;
		buf = p;
		stride_length = new_stride;
		memset(buf, 0, fs->blocksize * stride_length);
	}
	/* OK, do the write loop */
	j=0;
	ಯಾವತ್ (j < ಅಂಕೆ) {
		ಯದಿ (blk % stride_length) {
			ಎಣಿಕೆ = stride_length - (blk % stride_length);
			ಯದಿ (ಎಣಿಕೆ > (ಅಂಕೆ - j))
				ಎಣಿಕೆ = ಅಂಕೆ - j;
		} ಅನ್ಯಥಾ {
			ಎಣಿಕೆ = ಅಂಕೆ - j;
			ಯದಿ (ಎಣಿಕೆ > stride_length)
				ಎಣಿಕೆ = stride_length;
		}
		retval = io_channel_write_blk64(fs->io, blk, ಎಣಿಕೆ, buf);
		ಯದಿ (retval) {
			ಯದಿ (ret_ಎಣಿಕೆ)
				*ret_ಎಣಿಕೆ = ಎಣಿಕೆ;
			ಯದಿ (ret_blk)
				*ret_blk = blk;
			ಪ್ರತ್ಯಾಗಮ retval;
		}
		j += ಎಣಿಕೆ; blk += ಎಣಿಕೆ;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_zero_blocks(ext2_filsys fs, blk_t blk, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ,
			     blk_t *ret_blk, ಪೂರ್ಣಾಙ್ಕ *ret_ಎಣಿಕೆ)
{
	blk64_t ret_blk2;
	errcode_t retval;

	retval = ext2fs_zero_blocks2(fs, blk, ಅಂಕೆ, &ret_blk2, ret_ಎಣಿಕೆ);
	ಯದಿ (retval)
		*ret_blk = (blk_t) ret_blk2;
	ಪ್ರತ್ಯಾಗಮ retval;
}

/*
 * Calculate the initial goal block to be roughly at the middle of the
 * filesystem.  Pick a group that has the largest number of free
 * blocks.
 */
ಸ್ಥಿರ blk64_t get_midpoint_journal_block(ext2_filsys fs)
{
	dgrp_t	group, start, end, i, log_flex;

	group = ext2fs_group_of_blk2(fs, (ext2fs_blocks_ಎಣಿಕೆ(fs->super) -
					 fs->super->s_first_data_block) / 2);
	log_flex = 1U << fs->super->s_log_groups_per_flex;
	ಯದಿ (fs->super->s_log_groups_per_flex && (group > log_flex)) {
		group = group & ~(log_flex - 1);
		ಯಾವತ್ ((group < fs->group_desc_ಎಣಿಕೆ) &&
		       ext2fs_bg_free_blocks_ಎಣಿಕೆ(fs, group) == 0)
			group++;
		ಯದಿ (group == fs->group_desc_ಎಣಿಕೆ)
			group = 0;
		start = group;
	} ಅನ್ಯಥಾ
		start = (group > 0) ? group-1 : group;
	end = ((group+1) < fs->group_desc_ಎಣಿಕೆ) ? group+1 : group;
	group = start;
	ಪ್ರತಿ (i = start + 1; i <= end; i++)
		ಯದಿ (ext2fs_bg_free_blocks_ಎಣಿಕೆ(fs, i) >
		    ext2fs_bg_free_blocks_ಎಣಿಕೆ(fs, group))
			group = i;
	ಪ್ರತ್ಯಾಗಮ ext2fs_group_first_block2(fs, group);
}

/*
 * This function creates a journal using direct I/O routines.
 */
ಸ್ಥಿರ errcode_t write_journal_inode(ext2_filsys fs, ext2_ino_t journal_ino,
				     ಸಂರಚನಾ ext2fs_journal_params *jparams,
				     blk64_t goal, ಪೂರ್ಣಾಙ್ಕ flags)
{
	ವರ್ಣ			*buf;
	errcode_t		retval;
	ಸಂರಚನಾ ext2_inode	inode;
	unsigned long long	inode_size;
	ಪೂರ್ಣಾಙ್ಕ			falloc_flags = EXT2_FALLOCATE_FORCE_INIT;
	blk64_t			zblk;
	time_t			now;

	ಯದಿ ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		ಪ್ರತ್ಯಾಗಮ retval;

	ಯದಿ ((retval = ext2fs_read_bitmaps(fs)))
		ಗಚ್ಛ out2;

	ಯದಿ ((retval = ext2fs_read_inode(fs, journal_ino, &inode)))
		ಗಚ್ಛ out2;

	ಯದಿ (inode.i_blocks > 0) {
		retval = EEXIST;
		ಗಚ್ಛ out2;
	}

	ಯದಿ (goal == ~0ULL)
		goal = get_midpoint_journal_block(fs);

	ಯದಿ (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;

	ಯದಿ (!(flags & EXT2_MKJOURNAL_LAZYINIT))
		falloc_flags |= EXT2_FALLOCATE_ZERO_BLOCKS;

	inode_size = (unsigned long long)fs->blocksize *
			(jparams->ಅಂಕೆ_journal_blocks + jparams->ಅಂಕೆ_fc_blocks);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	inode.i_links_ಎಣಿಕೆ = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	retval = ext2fs_inode_size_set(fs, &inode, inode_size);
	ಯದಿ (retval)
		ಗಚ್ಛ out2;

	retval = ext2fs_fallocate(fs, falloc_flags, journal_ino,
				  &inode, goal, 0,
				  jparams->ಅಂಕೆ_journal_blocks + jparams->ಅಂಕೆ_fc_blocks);
	ಯದಿ (retval)
		ಗಚ್ಛ out2;

	ಯದಿ ((retval = ext2fs_write_new_inode(fs, journal_ino, &inode)))
		ಗಚ್ಛ out2;

	retval = ext2fs_bmap2(fs, journal_ino, &inode, ರಿಕ್ತ, 0, 0, ರಿಕ್ತ, &zblk);
	ಯದಿ (retval)
		ಗಚ್ಛ out2;

	retval = io_channel_write_blk64(fs->io, zblk, 1, buf);
	ಯದಿ (retval)
		ಗಚ್ಛ out2;

	memcpy(fs->super->s_jnl_blocks, inode.i_block, EXT2_N_BLOCKS*4);
	fs->super->s_jnl_blocks[15] = inode.i_size_ಮೇಲಿನ;
	fs->super->s_jnl_blocks[16] = inode.i_size;
	fs->super->s_jnl_backup_type = EXT3_JNL_BACKUP_BLOCKS;
	ext2fs_mark_super_dirty(fs);

out2:
	ext2fs_free_mem(&buf);
	ಪ್ರತ್ಯಾಗಮ retval;
}

/*
 * Find a reasonable journal file size (in blocks) given the number of blocks
 * in the filesystem.  For very small filesystems, it is not reasonable to
 * have a journal that fills more than half of the filesystem.
 *
 * n.b. comments assume 4k blocks
 */
ಪೂರ್ಣಾಙ್ಕ ext2fs_default_journal_size(__u64 ಅಂಕೆ_blocks)
{
	ಯದಿ (ಅಂಕೆ_blocks < 2048)
		ಪ್ರತ್ಯಾಗಮ -1;
	ಯದಿ (ಅಂಕೆ_blocks < 32768)		/* 128 MB */
		ಪ್ರತ್ಯಾಗಮ (1024);			/* 4 MB */
	ಯದಿ (ಅಂಕೆ_blocks < 256*1024)	/* 1 GB */
		ಪ್ರತ್ಯಾಗಮ (4096);			/* 16 MB */
	ಯದಿ (ಅಂಕೆ_blocks < 512*1024)	/* 2 GB */
		ಪ್ರತ್ಯಾಗಮ (8192);			/* 32 MB */
	ಯದಿ (ಅಂಕೆ_blocks < 4096*1024)	/* 16 GB */
		ಪ್ರತ್ಯಾಗಮ (16384);			/* 64 MB */
	ಯದಿ (ಅಂಕೆ_blocks < 8192*1024)	/* 32 GB */
		ಪ್ರತ್ಯಾಗಮ (32768);			/* 128 MB */
	ಯದಿ (ಅಂಕೆ_blocks < 16384*1024)	/* 64 GB */
		ಪ್ರತ್ಯಾಗಮ (65536);			/* 256 MB */
	ಯದಿ (ಅಂಕೆ_blocks < 32768*1024)	/* 128 GB */
		ಪ್ರತ್ಯಾಗಮ (131072);		/* 512 MB */
	ಪ್ರತ್ಯಾಗಮ 262144;				/* 1 GB */
}

errcode_t ext2fs_get_journal_params(ಸಂರಚನಾ ext2fs_journal_params *params,
		ext2_filsys fs)
{
	blk_t total_blks;
	ಪೂರ್ಣಾಙ್ಕ ret;

	memset(params, 0, ಪರಿಮಾಣ(*params));
	ಯದಿ (ext2fs_has_feature_journal_dev(fs->super)) {
		total_blks = ext2fs_blocks_ಎಣಿಕೆ(fs->super);
		ಯದಿ (total_blks < JBD2_MIN_JOURNAL_BLOCKS)
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_JOURNAL_TOO_SMALL;

		ಯದಿ (!ext2fs_has_feature_fast_commit(fs->super)) {
			params->ಅಂಕೆ_journal_blocks = total_blks;
			params->ಅಂಕೆ_fc_blocks = 0;
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		params->ಅಂಕೆ_journal_blocks = ext2fs_blocks_ಎಣಿಕೆ(fs->super) *
				EXT2_JOURNAL_TO_FC_BLKS_RATIO /
				(EXT2_JOURNAL_TO_FC_BLKS_RATIO + 1);
		ಯದಿ (JBD2_MIN_JOURNAL_BLOCKS > params->ಅಂಕೆ_journal_blocks)
			params->ಅಂಕೆ_journal_blocks = JBD2_MIN_JOURNAL_BLOCKS;
		params->ಅಂಕೆ_fc_blocks = total_blks - params->ಅಂಕೆ_journal_blocks;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ret = ext2fs_default_journal_size(ext2fs_blocks_ಎಣಿಕೆ(fs->super));
	ಯದಿ (ret < 0)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_JOURNAL_TOO_SMALL;

	params->ಅಂಕೆ_journal_blocks = ret;
	ಯದಿ (ext2fs_has_feature_fast_commit(fs->super))
		params->ಅಂಕೆ_fc_blocks = params->ಅಂಕೆ_journal_blocks /
			EXT2_JOURNAL_TO_FC_BLKS_RATIO;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_journal_sb_start(ಪೂರ್ಣಾಙ್ಕ blocksize)
{
	ಯದಿ (blocksize == EXT2_MIN_BLOCK_SIZE)
		ಪ್ರತ್ಯಾಗಮ 2;
	ಪ್ರತ್ಯಾಗಮ 1;
}

/*
 * This function adds a journal device to a filesystem
 */
errcode_t ext2fs_add_journal_device(ext2_filsys fs, ext2_filsys journal_dev)
{
	ಸಂರಚನಾ stat	st;
	errcode_t	retval;
	ವರ್ಣ		buf[SUPERBLOCK_SIZE];
	journal_superblock_t	*jsb;
	ಪೂರ್ಣಾಙ್ಕ		start;
	__u32		i, nr_users;

	/* Make sure the device exists and is a block device */
	ಯದಿ (stat(journal_dev->device_name, &st) < 0)
		ಪ್ರತ್ಯಾಗಮ errno;

	ಯದಿ (!S_ISBLK(st.st_mode))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_JOURNAL_NOT_BLOCK; /* Must be a block device */

	/* Get the journal superblock */
	start = ext2fs_journal_sb_start(journal_dev->blocksize);
	ಯದಿ ((retval = io_channel_read_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE,
					    buf)))
		ಪ್ರತ್ಯಾಗಮ retval;

	jsb = (journal_superblock_t *) buf;
	ಯದಿ ((jsb->s_header.h_magic != (unsigned) ntohl(JBD2_MAGIC_ಸಂಖ್ಯೆ)) ||
	    (jsb->s_header.h_blocktype != (unsigned) ntohl(JBD2_SUPERBLOCK_V2)))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_JOURNAL_SB;

	ಯದಿ (ntohl(jsb->s_blocksize) != (unsigned long) fs->blocksize)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	/* Check and see if this filesystem has already been added */
	nr_users = ntohl(jsb->s_nr_users);
	ಯದಿ (nr_users > JBD2_USERS_MAX)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_CORRUPT_JOURNAL_SB;
	ಪ್ರತಿ (i=0; i < nr_users; i++) {
		ಯದಿ (memcmp(fs->super->s_uuid,
			   &jsb->s_users[i*16], 16) == 0)
			ಭಙ್ಗ;
	}
	ಯದಿ (i >= nr_users) {
		memcpy(&jsb->s_users[nr_users*16],
		       fs->super->s_uuid, 16);
		jsb->s_nr_users = htonl(nr_users+1);
	}

	/* Writeback the journal superblock */
	ಯದಿ ((retval = io_channel_write_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE, buf)))
		ಪ್ರತ್ಯಾಗಮ retval;

	fs->super->s_journal_inum = 0;
	fs->super->s_journal_dev = st.st_rdev;
	memcpy(fs->super->s_journal_uuid, jsb->s_uuid,
	       ಪರಿಮಾಣ(fs->super->s_journal_uuid));
	memset(fs->super->s_jnl_blocks, 0, ಪರಿಮಾಣ(fs->super->s_jnl_blocks));
	ext2fs_set_feature_journal(fs->super);
	ext2fs_mark_super_dirty(fs);
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * This function adds a journal inode to a filesystem, using either
 * POSIX routines if the filesystem is mounted, or using direct I/O
 * functions if it is not.
 */
errcode_t ext2fs_add_journal_inode3(ext2_filsys fs, ಸಂರಚನಾ ext2fs_journal_params *jparams,
				    blk64_t goal, ಪೂರ್ಣಾಙ್ಕ flags)
{
	errcode_t		retval;
	ext2_ino_t		journal_ino;
	ಸಂರಚನಾ stat		st;
	ವರ್ಣ			jfile[1024];
	ಪೂರ್ಣಾಙ್ಕ			mount_flags;
	ಪೂರ್ಣಾಙ್ಕ			fd = -1;

	ಯದಿ (flags & EXT2_MKJOURNAL_NO_MNT_CHECK)
		mount_flags = 0;
	ಅನ್ಯಥಾ ಯದಿ ((retval = ext2fs_check_mount_point(fs->device_name,
						    &mount_flags,
						    jfile, ಪರಿಮಾಣ(jfile)-10)))
		ಪ್ರತ್ಯಾಗಮ retval;

	ಯದಿ (mount_flags & EXT2_MF_MOUNTED) {
#ಯದಿ HAVE_EXT2_IOCTLS
		ಪೂರ್ಣಾಙ್ಕ f = 0;
#endif
		strcat(jfile, "/.journal");

		/*
		 * If .../.journal already exists, make sure any
		 * immutable or append-only flags are cleared.
		 */
#ಯದಿ defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		(ಶೂನ್ಯ) chflags (jfile, 0);
#ಅನ್ಯಥಾ
#ಯದಿ HAVE_EXT2_IOCTLS
		fd = open(jfile, O_RDONLY);
		ಯದಿ (fd >= 0) {
			retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
			close(fd);
			ಯದಿ (retval)
				ಪ್ರತ್ಯಾಗಮ errno;
		}
#endif
#endif

		/* Create the journal file */
		ಯದಿ ((fd = open(jfile, O_CREAT|O_WRONLY, 0600)) < 0)
			ಪ್ರತ್ಯಾಗಮ errno;

		/* Note that we can't do lazy journal initialization for mounted
		 * filesystems, since the zero writing is also allocating the
		 * journal blocks.  We could use fallocate, but not all kernels
		 * support that, and creating a journal on a mounted ext2
		 * filesystems is extremely rare these days...  Ignore it. */
		flags &= ~EXT2_MKJOURNAL_LAZYINIT;

		ಯದಿ ((retval = write_journal_file(fs, jfile, jparams, flags)))
			ಗಚ್ಛ errout;

		/* Get inode number of the journal file */
		ಯದಿ (fstat(fd, &st) < 0) {
			retval = errno;
			ಗಚ್ಛ errout;
		}

#ಯದಿ defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		retval = fchflags (fd, UF_NODUMP|UF_IMMUTABLE);
#ಅನ್ಯಥಾ
#ಯದಿ HAVE_EXT2_IOCTLS
		ಯದಿ (ioctl(fd, EXT2_IOC_GETFLAGS, &f) < 0) {
			retval = errno;
			ಗಚ್ಛ errout;
		}
		f |= EXT2_NODUMP_FL | EXT2_IMMUTABLE_FL;
		retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
#endif
#endif
		ಯದಿ (retval) {
			retval = errno;
			ಗಚ್ಛ errout;
		}

		ಯದಿ (close(fd) < 0) {
			retval = errno;
			fd = -1;
			ಗಚ್ಛ errout;
		}
		journal_ino = st.st_ino;
		memset(fs->super->s_jnl_blocks, 0,
		       ಪರಿಮಾಣ(fs->super->s_jnl_blocks));
	} ಅನ್ಯಥಾ {
		ಯದಿ ((mount_flags & EXT2_MF_BUSY) &&
		    !(fs->flags & EXT2_FLAG_EXCLUSIVE)) {
			retval = EBUSY;
			ಗಚ್ಛ errout;
		}
		journal_ino = EXT2_JOURNAL_INO;
		ಯದಿ ((retval = write_journal_inode(fs, journal_ino,
						  jparams, goal, flags)))
			ಪ್ರತ್ಯಾಗಮ retval;
	}

	fs->super->s_journal_inum = journal_ino;
	fs->super->s_journal_dev = 0;
	memset(fs->super->s_journal_uuid, 0,
	       ಪರಿಮಾಣ(fs->super->s_journal_uuid));
	ext2fs_set_feature_journal(fs->super);

	ext2fs_mark_super_dirty(fs);
	ಪ್ರತ್ಯಾಗಮ 0;
errout:
	ಯದಿ (fd >= 0)
		close(fd);
	ಪ್ರತ್ಯಾಗಮ retval;
}

errcode_t ext2fs_add_journal_inode2(ext2_filsys fs, blk_t ಅಂಕೆ_blocks,
				    blk64_t goal, ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ ext2fs_journal_params jparams;

	jparams.ಅಂಕೆ_journal_blocks = ಅಂಕೆ_blocks;
	jparams.ಅಂಕೆ_fc_blocks = 0;

	ಪ್ರತ್ಯಾಗಮ ext2fs_add_journal_inode3(fs, &jparams, goal, flags);
}

errcode_t ext2fs_add_journal_inode(ext2_filsys fs, blk_t ಅಂಕೆ_blocks, ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_add_journal_inode2(fs, ಅಂಕೆ_blocks, ~0ULL, flags);
}


#ifdef DEBUG
main(ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
	errcode_t	retval;
	ವರ್ಣ		*device_name;
	ext2_filsys	fs;

	ಯದಿ (argc < 2) {
		fprintf(stderr, "Usage: %s filesystem\n", argv[0]);
		exit(1);
	}
	device_name = argv[1];

	retval = ext2fs_open (device_name, EXT2_FLAG_RW, 0, 0,
			      unix_io_manager, &fs);
	ಯದಿ (retval) {
		com_err(argv[0], retval, "while opening %s", device_name);
		exit(1);
	}

	retval = ext2fs_add_journal_inode(fs, JBD2_MIN_JOURNAL_BLOCKS, 0);
	ಯದಿ (retval) {
		com_err(argv[0], retval, "while adding journal to %s",
			device_name);
		exit(1);
	}
	retval = ext2fs_flush(fs);
	ಯದಿ (retval) {
		printf("Warning, had trouble writing out superblocks.\n");
	}
	ext2fs_close_free(&fs);
	exit(0);

}
#endif
