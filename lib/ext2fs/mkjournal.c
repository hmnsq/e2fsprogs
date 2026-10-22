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
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_IOCTL_H
#ಅನ್ತರ್ಭಾವ <sys/ioctl.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_NETINET_ಒಳ_H
#ಅನ್ತರ್ಭಾವ <netinet/in.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "e2p/e2p.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

#ಅನ್ತರ್ಭಾವ "kernel-jbd.h"

/*
 * This function automatically sets up the journal superblock and
 * returns it as an allocated block.
 */
errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
					   ರಚನೆ ext2fs_journal_params *jparams,
					   ಪೂರ್ಣಾಂಕ flags, ಅಕ್ಷರ **ret_jsb)
{
	errcode_t		retval;
	journal_superblock_t	*jsb;

	ಆದರೆ (jparams->ಅಂಕೆ_journal_ಖಂಡಗಳು < JBD2_MIN_JOURNAL_ಖಂಡಗಳು)
		ಹಿಂತಿರುಗಿ EXT2_ET_JOURNAL_TOO_SMALL;

	ಆದರೆ ((retval = ext2fs_get_mem(fs->blocksize, &jsb)))
		ಹಿಂತಿರುಗಿ retval;

	memset (jsb, 0, fs->blocksize);

	jsb->s_ಶೀರ್ಷಿಕೆ.h_magic = htonl(JBD2_MAGIC_ಸಂಖ್ಯೆ);
	ಆದರೆ (flags & EXT2_MKJOURNAL_V1_SUPER)
		jsb->s_ಶೀರ್ಷಿಕೆ.h_blocktype = htonl(JBD2_SUPERBLOCK_V1);
	ಇಲ್ಲದಿದ್ದರೆ
		jsb->s_ಶೀರ್ಷಿಕೆ.h_blocktype = htonl(JBD2_SUPERBLOCK_V2);
	jsb->s_blocksize = htonl(fs->blocksize);
	jsb->s_maxlen = htonl(jparams->ಅಂಕೆ_journal_ಖಂಡಗಳು + jparams->ಅಂಕೆ_fc_ಖಂಡಗಳು);
	jsb->s_nr_users = htonl(1);
	jsb->s_ಮೊದಲ = htonl(1);
	jsb->s_sequence = htonl(1);
	jsb->s_ಅಂಕೆ_fc_blks = htonl(jparams->ಅಂಕೆ_fc_ಖಂಡಗಳು);
	ಸ್ಮರಣೆ_ನಕಲು(jsb->s_uuid, fs->super->s_uuid, ಗಾತ್ರ(fs->super->s_uuid));
	/*
	 * If we're creating an external journal device, we need to
	 * adjust these fields.
	 */
	ಆದರೆ (ext2fs_has_feature_journal_dev(fs->super)) {
		jsb->s_nr_users = 0;
		jsb->s_ಮೊದಲ = htonl(ext2fs_journal_sb_ಆರಂಭ(fs->blocksize) + 1);
	}

	*ret_jsb = (ಅಕ್ಷರ *) jsb;
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_create_journal_superblock(ext2_filsys fs, __u32 ಅಂಕೆ_ಖಂಡಗಳು,
					ಪೂರ್ಣಾಂಕ flags, ಅಕ್ಷರ **ret_sb)
{
	ರಚನೆ ext2fs_journal_params jparams;

	jparams.ಅಂಕೆ_journal_ಖಂಡಗಳು = ಅಂಕೆ_ಖಂಡಗಳು;
	jparams.ಅಂಕೆ_fc_ಖಂಡಗಳು = 0;

	ಹಿಂತಿರುಗಿ ext2fs_create_journal_superblock2(fs, &jparams, flags, ret_sb);
}

/*
 * This function writes a journal using POSIX routines.  It is used
 * for creating external journals and creating journals on live
 * filesystems.
 */
ಸ್ಥಿರ errcode_t write_journal_file(ext2_filsys fs, ಅಕ್ಷರ *filename,
				    ರಚನೆ ext2fs_journal_params *jparams,
				    ಪೂರ್ಣಾಂಕ flags)
{
	errcode_t	retval;
	ಅಕ್ಷರ		*buf = 0;
	ಪೂರ್ಣಾಂಕ		fd, ret_ಗಾತ್ರ;
	blk_t		i;

	ಆದರೆ ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		ಹಿಂತಿರುಗಿ retval;

	/* Open the device or journal file */
	ಆದರೆ ((fd = open(filename, O_WRONLY)) < 0) {
		retval = errno;
		ಹೋಗು errfree;
	}

	/* Write the superblock out */
	retval = EXT2_ET_SHORT_WRITE;
	ret_ಗಾತ್ರ = ಲಿಖ(fd, buf, fs->blocksize);
	ಆದರೆ (ret_ಗಾತ್ರ < 0) {
		retval = errno;
		ಹೋಗು errout;
	}
	ಆದರೆ (ret_ಗಾತ್ರ != (ಪೂರ್ಣಾಂಕ) fs->blocksize)
		ಹೋಗು errout;
	memset(buf, 0, fs->blocksize);

	ಆದರೆ (flags & EXT2_MKJOURNAL_LAZYINIT)
		ಹೋಗು success;

	ಗಾಗಿ (i = 1; i < jparams->ಅಂಕೆ_journal_ಖಂಡಗಳು + jparams->ಅಂಕೆ_fc_ಖಂಡಗಳು; i++) {
		ret_ಗಾತ್ರ = ಲಿಖ(fd, buf, fs->blocksize);
		ಆದರೆ (ret_ಗಾತ್ರ < 0) {
			retval = errno;
			ಹೋಗು errout;
		}
		ಆದರೆ (ret_ಗಾತ್ರ != (ಪೂರ್ಣಾಂಕ) fs->blocksize)
			ಹೋಗು errout;
	}

success:
	retval = 0;
errout:
	close(fd);
errfree:
	ext2fs_free_mem(&buf);
	ಹಿಂತಿರುಗಿ retval;
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
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_STRIDE_ಉದ್ದಳತೆ (4194304 / (ಪೂರ್ಣಾಂಕ) fs->blocksize)
errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, ಪೂರ್ಣಾಂಕ ಅಂಕೆ,
			      blk64_t *ret_blk, ಪೂರ್ಣಾಂಕ *ret_ಎಣಿಕೆ)
{
	ಪೂರ್ಣಾಂಕ		j, ಎಣಿಕೆ_2;
	ಸ್ಥಿರ ಶೂನ್ಯ	*buf;
	ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ	stride_ಉದ್ದಳತೆ;
	errcode_t	retval;

	/* If fs is null, clean up the static buffer and return */
	ಆದರೆ (!fs) {
		ಆದರೆ (buf) {
			ಬಿಡುಗಡೆ(buf);
			buf = 0;
			stride_ಉದ್ದಳತೆ = 0;
		}
		ಹಿಂತಿರುಗಿ 0;
	}

	/* Deal with zeroing less than 1 block */
	ಆದರೆ (ಅಂಕೆ <= 0)
		ಹಿಂತಿರುಗಿ 0;

	/* Try a zero out command, if supported */
	retval = io_channel_zeroout(fs->io, blk, ಅಂಕೆ);
	ಆದರೆ (retval == 0)
		ಹಿಂತಿರುಗಿ 0;

	/* Allocate the zeroizing buffer if necessary */
	ಆದರೆ (ಅಂಕೆ > stride_ಉದ್ದಳತೆ && stride_ಉದ್ದಳತೆ < MAX_STRIDE_ಉದ್ದಳತೆ) {
		ಶೂನ್ಯ *p;
		ಪೂರ್ಣಾಂಕ new_stride = ಅಂಕೆ;

		ಆದರೆ (new_stride > MAX_STRIDE_ಉದ್ದಳತೆ)
			new_stride = MAX_STRIDE_ಉದ್ದಳತೆ;
		p = realloc(buf, fs->blocksize * new_stride);
		ಆದರೆ (!p)
			ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;
		buf = p;
		stride_ಉದ್ದಳತೆ = new_stride;
		memset(buf, 0, fs->blocksize * stride_ಉದ್ದಳತೆ);
	}
	/* OK, do the write loop */
	j=0;
	ಆಗ (j < ಅಂಕೆ) {
		ಆದರೆ (blk % stride_ಉದ್ದಳತೆ) {
			ಎಣಿಕೆ_2 = stride_ಉದ್ದಳತೆ - (blk % stride_ಉದ್ದಳತೆ);
			ಆದರೆ (ಎಣಿಕೆ_2 > (ಅಂಕೆ - j))
				ಎಣಿಕೆ_2 = ಅಂಕೆ - j;
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಎಣಿಕೆ_2 = ಅಂಕೆ - j;
			ಆದರೆ (ಎಣಿಕೆ_2 > stride_ಉದ್ದಳತೆ)
				ಎಣಿಕೆ_2 = stride_ಉದ್ದಳತೆ;
		}
		retval = io_channel_write_blk64(fs->io, blk, ಎಣಿಕೆ_2, buf);
		ಆದರೆ (retval) {
			ಆದರೆ (ret_ಎಣಿಕೆ)
				*ret_ಎಣಿಕೆ = ಎಣಿಕೆ_2;
			ಆದರೆ (ret_blk)
				*ret_blk = blk;
			ಹಿಂತಿರುಗಿ retval;
		}
		j += ಎಣಿಕೆ_2; blk += ಎಣಿಕೆ_2;
	}
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_zero_ಖಂಡಗಳು(ext2_filsys fs, blk_t blk, ಪೂರ್ಣಾಂಕ ಅಂಕೆ,
			     blk_t *ret_blk, ಪೂರ್ಣಾಂಕ *ret_ಎಣಿಕೆ)
{
	blk64_t ret_blk2;
	errcode_t retval;

	retval = ext2fs_zero_blocks2(fs, blk, ಅಂಕೆ, &ret_blk2, ret_ಎಣಿಕೆ);
	ಆದರೆ (retval)
		*ret_blk = (blk_t) ret_blk2;
	ಹಿಂತಿರುಗಿ retval;
}

/*
 * Calculate the initial goal block to be roughly at the middle of the
 * filesystem.  Pick a group that has the largest number of free
 * blocks.
 */
ಸ್ಥಿರ blk64_t get_midpoint_journal_ಖಂಡ(ext2_filsys fs)
{
	dgrp_t	ಗುಂಪು, ಆರಂಭ, ಕೊನೆ, i, log_flex;

	ಗುಂಪು = ext2fs_ಗುಂಪು_ದ_blk2(fs, (ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super) -
					 fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ) / 2);
	log_flex = 1U << fs->super->s_log_groups_ಪ್ರತಿ_flex;
	ಆದರೆ (fs->super->s_log_groups_ಪ್ರತಿ_flex && (ಗುಂಪು > log_flex)) {
		ಗುಂಪು = ಗುಂಪು & ~(log_flex - 1);
		ಆಗ ((ಗುಂಪು < fs->ಗುಂಪು_desc_ಎಣಿಕೆ) &&
		       ext2fs_bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ(fs, ಗುಂಪು) == 0)
			ಗುಂಪು++;
		ಆದರೆ (ಗುಂಪು == fs->ಗುಂಪು_desc_ಎಣಿಕೆ)
			ಗುಂಪು = 0;
		ಆರಂಭ = ಗುಂಪು;
	} ಇಲ್ಲದಿದ್ದರೆ
		ಆರಂಭ = (ಗುಂಪು > 0) ? ಗುಂಪು-1 : ಗುಂಪು;
	ಕೊನೆ = ((ಗುಂಪು+1) < fs->ಗುಂಪು_desc_ಎಣಿಕೆ) ? ಗುಂಪು+1 : ಗುಂಪು;
	ಗುಂಪು = ಆರಂಭ;
	ಗಾಗಿ (i = ಆರಂಭ + 1; i <= ಕೊನೆ; i++)
		ಆದರೆ (ext2fs_bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ(fs, i) >
		    ext2fs_bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ(fs, ಗುಂಪು))
			ಗುಂಪು = i;
	ಹಿಂತಿರುಗಿ ext2fs_ಗುಂಪು_ಮೊದಲ_block2(fs, ಗುಂಪು);
}

/*
 * This function creates a journal using direct I/O routines.
 */
ಸ್ಥಿರ errcode_t write_journal_inode(ext2_filsys fs, ext2_ino_t journal_ino,
				     ರಚನೆ ext2fs_journal_params *jparams,
				     blk64_t goal, ಪೂರ್ಣಾಂಕ flags)
{
	ಅಕ್ಷರ			*buf;
	errcode_t		retval;
	ರಚನೆ ext2_inode	inode;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ	inode_ಗಾತ್ರ;
	ಪೂರ್ಣಾಂಕ			falloc_flags = EXT2_FALLOCATE_FORCE_INIT;
	blk64_t			zblk;
	time_t			now;

	ಆದರೆ ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		ಹಿಂತಿರುಗಿ retval;

	ಆದರೆ ((retval = ext2fs_read_bitmaps(fs)))
		ಹೋಗು out2;

	ಆದರೆ ((retval = ext2fs_read_inode(fs, journal_ino, &inode)))
		ಹೋಗು out2;

	ಆದರೆ (inode.i_ಖಂಡಗಳು > 0) {
		retval = EEXIST;
		ಹೋಗು out2;
	}

	ಆದರೆ (goal == ~0ULL)
		goal = get_midpoint_journal_ಖಂಡ(fs);

	ಆದರೆ (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;

	ಆದರೆ (!(flags & EXT2_MKJOURNAL_LAZYINIT))
		falloc_flags |= EXT2_FALLOCATE_ZERO_ಖಂಡಗಳು;

	inode_ಗಾತ್ರ = (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ)fs->blocksize *
			(jparams->ಅಂಕೆ_journal_ಖಂಡಗಳು + jparams->ಅಂಕೆ_fc_ಖಂಡಗಳು);
	now = ext2fs_P_get_ಸಮಯ(fs);
	ext2fs_inode_xtime_ಗಣ(&inode, i_mtime, now);
	ext2fs_inode_xtime_ಗಣ(&inode, i_ctime, now);
	inode.i_links_ಎಣಿಕೆ = 1;
	inode.i_ಬಹುಲಕ = LINUX_S_IFREG | 0600;
	retval = ext2fs_inode_ಗಾತ್ರ_ಗಣ(fs, &inode, inode_ಗಾತ್ರ);
	ಆದರೆ (retval)
		ಹೋಗು out2;

	retval = ext2fs_fallocate(fs, falloc_flags, journal_ino,
				  &inode, goal, 0,
				  jparams->ಅಂಕೆ_journal_ಖಂಡಗಳು + jparams->ಅಂಕೆ_fc_ಖಂಡಗಳು);
	ಆದರೆ (retval)
		ಹೋಗು out2;

	ಆದರೆ ((retval = ext2fs_write_new_inode(fs, journal_ino, &inode)))
		ಹೋಗು out2;

	retval = ext2fs_bmap2(fs, journal_ino, &inode, ಖಾಲಿ, 0, 0, ಖಾಲಿ, &zblk);
	ಆದರೆ (retval)
		ಹೋಗು out2;

	retval = io_channel_write_blk64(fs->io, zblk, 1, buf);
	ಆದರೆ (retval)
		ಹೋಗು out2;

	ಸ್ಮರಣೆ_ನಕಲು(fs->super->s_jnl_ಖಂಡಗಳು, inode.i_ಖಂಡ, EXT2_N_ಖಂಡಗಳು*4);
	fs->super->s_jnl_ಖಂಡಗಳು[15] = inode.i_ಗಾತ್ರ_ಮೇಲಿನ;
	fs->super->s_jnl_ಖಂಡಗಳು[16] = inode.i_ಗಾತ್ರ;
	fs->super->s_jnl_backup_type = EXT3_JNL_BACKUP_ಖಂಡಗಳು;
	ext2fs_mark_super_dirty(fs);

out2:
	ext2fs_free_mem(&buf);
	ಹಿಂತಿರುಗಿ retval;
}

/*
 * Find a reasonable journal file size (in blocks) given the number of blocks
 * in the filesystem.  For very small filesystems, it is not reasonable to
 * have a journal that fills more than half of the filesystem.
 *
 * n.b. comments assume 4k blocks
 */
ಪೂರ್ಣಾಂಕ ext2fs_default_journal_ಗಾತ್ರ(__u64 ಅಂಕೆ_ಖಂಡಗಳು)
{
	ಆದರೆ (ಅಂಕೆ_ಖಂಡಗಳು < 2048)
		ಹಿಂತಿರುಗಿ -1;
	ಆದರೆ (ಅಂಕೆ_ಖಂಡಗಳು < 32768)		/* 128 MB */
		ಹಿಂತಿರುಗಿ (1024);			/* 4 MB */
	ಆದರೆ (ಅಂಕೆ_ಖಂಡಗಳು < 256*1024)	/* 1 GB */
		ಹಿಂತಿರುಗಿ (4096);			/* 16 MB */
	ಆದರೆ (ಅಂಕೆ_ಖಂಡಗಳು < 512*1024)	/* 2 GB */
		ಹಿಂತಿರುಗಿ (8192);			/* 32 MB */
	ಆದರೆ (ಅಂಕೆ_ಖಂಡಗಳು < 4096*1024)	/* 16 GB */
		ಹಿಂತಿರುಗಿ (16384);			/* 64 MB */
	ಆದರೆ (ಅಂಕೆ_ಖಂಡಗಳು < 8192*1024)	/* 32 GB */
		ಹಿಂತಿರುಗಿ (32768);			/* 128 MB */
	ಆದರೆ (ಅಂಕೆ_ಖಂಡಗಳು < 16384*1024)	/* 64 GB */
		ಹಿಂತಿರುಗಿ (65536);			/* 256 MB */
	ಆದರೆ (ಅಂಕೆ_ಖಂಡಗಳು < 32768*1024)	/* 128 GB */
		ಹಿಂತಿರುಗಿ (131072);		/* 512 MB */
	ಹಿಂತಿರುಗಿ 262144;				/* 1 GB */
}

errcode_t ext2fs_get_journal_params(ರಚನೆ ext2fs_journal_params *params,
		ext2_filsys fs)
{
	blk_t ಒಟ್ಟು_blks;
	ಪೂರ್ಣಾಂಕ ret;

	memset(params, 0, ಗಾತ್ರ(*params));
	ಆದರೆ (ext2fs_has_feature_journal_dev(fs->super)) {
		ಒಟ್ಟು_blks = ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super);
		ಆದರೆ (ಒಟ್ಟು_blks < JBD2_MIN_JOURNAL_ಖಂಡಗಳು)
			ಹಿಂತಿರುಗಿ EXT2_ET_JOURNAL_TOO_SMALL;

		ಆದರೆ (!ext2fs_has_feature_fast_commit(fs->super)) {
			params->ಅಂಕೆ_journal_ಖಂಡಗಳು = ಒಟ್ಟು_blks;
			params->ಅಂಕೆ_fc_ಖಂಡಗಳು = 0;
			ಹಿಂತಿರುಗಿ 0;
		}
		params->ಅಂಕೆ_journal_ಖಂಡಗಳು = ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super) *
				EXT2_JOURNAL_ಗಮ್ಯ_FC_BLKS_ಅನುಪಾತ /
				(EXT2_JOURNAL_ಗಮ್ಯ_FC_BLKS_ಅನುಪಾತ + 1);
		ಆದರೆ (JBD2_MIN_JOURNAL_ಖಂಡಗಳು > params->ಅಂಕೆ_journal_ಖಂಡಗಳು)
			params->ಅಂಕೆ_journal_ಖಂಡಗಳು = JBD2_MIN_JOURNAL_ಖಂಡಗಳು;
		params->ಅಂಕೆ_fc_ಖಂಡಗಳು = ಒಟ್ಟು_blks - params->ಅಂಕೆ_journal_ಖಂಡಗಳು;
		ಹಿಂತಿರುಗಿ 0;
	}

	ret = ext2fs_default_journal_ಗಾತ್ರ(ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super));
	ಆದರೆ (ret < 0)
		ಹಿಂತಿರುಗಿ EXT2_ET_JOURNAL_TOO_SMALL;

	params->ಅಂಕೆ_journal_ಖಂಡಗಳು = ret;
	ಆದರೆ (ext2fs_has_feature_fast_commit(fs->super))
		params->ಅಂಕೆ_fc_ಖಂಡಗಳು = params->ಅಂಕೆ_journal_ಖಂಡಗಳು /
			EXT2_JOURNAL_ಗಮ್ಯ_FC_BLKS_ಅನುಪಾತ;
	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ ext2fs_journal_sb_ಆರಂಭ(ಪೂರ್ಣಾಂಕ blocksize)
{
	ಆದರೆ (blocksize == EXT2_MIN_ಖಂಡ_ಗಾತ್ರ)
		ಹಿಂತಿರುಗಿ 2;
	ಹಿಂತಿರುಗಿ 1;
}

/*
 * This function adds a journal device to a filesystem
 */
errcode_t ext2fs_ಕೂಡಿಸು_journal_device(ext2_filsys fs, ext2_filsys journal_dev)
{
	ರಚನೆ stat	st;
	errcode_t	retval;
	ಅಕ್ಷರ		buf[SUPERBLOCK_ಗಾತ್ರ];
	journal_superblock_t	*jsb;
	ಪೂರ್ಣಾಂಕ		ಆರಂಭ;
	__u32		i, nr_users;

	/* Make sure the device exists and is a block device */
	ಆದರೆ (stat(journal_dev->device_name, &st) < 0)
		ಹಿಂತಿರುಗಿ errno;

	ಆದರೆ (!S_ISBLK(st.st_ಬಹುಲಕ))
		ಹಿಂತಿರುಗಿ EXT2_ET_JOURNAL_NOT_ಖಂಡ; /* Must be a block device */

	/* Get the journal superblock */
	ಆರಂಭ = ext2fs_journal_sb_ಆರಂಭ(journal_dev->blocksize);
	ಆದರೆ ((retval = io_channel_read_blk64(journal_dev->io, ಆರಂಭ,
					    -SUPERBLOCK_ಗಾತ್ರ,
					    buf)))
		ಹಿಂತಿರುಗಿ retval;

	jsb = (journal_superblock_t *) buf;
	ಆದರೆ ((jsb->s_ಶೀರ್ಷಿಕೆ.h_magic != (ಚಿಹ್ನೆರಹಿತ) ntohl(JBD2_MAGIC_ಸಂಖ್ಯೆ)) ||
	    (jsb->s_ಶೀರ್ಷಿಕೆ.h_blocktype != (ಚಿಹ್ನೆರಹಿತ) ntohl(JBD2_SUPERBLOCK_V2)))
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_JOURNAL_SB;

	ಆದರೆ (ntohl(jsb->s_blocksize) != (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ) fs->blocksize)
		ಹಿಂತಿರುಗಿ EXT2_ET_UNEXPECTED_ಖಂಡ_ಗಾತ್ರ;

	/* Check and see if this filesystem has already been added */
	nr_users = ntohl(jsb->s_nr_users);
	ಆದರೆ (nr_users > JBD2_USERS_MAX)
		ಹಿಂತಿರುಗಿ EXT2_ET_CORRUPT_JOURNAL_SB;
	ಗಾಗಿ (i=0; i < nr_users; i++) {
		ಆದರೆ (memcmp(fs->super->s_uuid,
			   &jsb->s_users[i*16], 16) == 0)
			ನಿಲ್ಲಿಸು;
	}
	ಆದರೆ (i >= nr_users) {
		ಸ್ಮರಣೆ_ನಕಲು(&jsb->s_users[nr_users*16],
		       fs->super->s_uuid, 16);
		jsb->s_nr_users = htonl(nr_users+1);
	}

	/* Writeback the journal superblock */
	ಆದರೆ ((retval = io_channel_write_blk64(journal_dev->io, ಆರಂಭ,
					    -SUPERBLOCK_ಗಾತ್ರ, buf)))
		ಹಿಂತಿರುಗಿ retval;

	fs->super->s_journal_inum = 0;
	fs->super->s_journal_dev = st.st_rdev;
	ಸ್ಮರಣೆ_ನಕಲು(fs->super->s_journal_uuid, jsb->s_uuid,
	       ಗಾತ್ರ(fs->super->s_journal_uuid));
	memset(fs->super->s_jnl_ಖಂಡಗಳು, 0, ಗಾತ್ರ(fs->super->s_jnl_ಖಂಡಗಳು));
	ext2fs_ಗಣ_feature_journal(fs->super);
	ext2fs_mark_super_dirty(fs);
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * This function adds a journal inode to a filesystem, using either
 * POSIX routines if the filesystem is mounted, or using direct I/O
 * functions if it is not.
 */
errcode_t ext2fs_ಕೂಡಿಸು_journal_inode3(ext2_filsys fs, ರಚನೆ ext2fs_journal_params *jparams,
				    blk64_t goal, ಪೂರ್ಣಾಂಕ flags)
{
	errcode_t		retval;
	ext2_ino_t		journal_ino;
	ರಚನೆ stat		st;
	ಅಕ್ಷರ			jfile[1024];
	ಪೂರ್ಣಾಂಕ			mount_flags;
	ಪೂರ್ಣಾಂಕ			fd = -1;

	ಆದರೆ (flags & EXT2_MKJOURNAL_NO_MNT_CHECK)
		mount_flags = 0;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((retval = ext2fs_check_mount_ಬಿಂದು(fs->device_name,
						    &mount_flags,
						    jfile, ಗಾತ್ರ(jfile)-10)))
		ಹಿಂತಿರುಗಿ retval;

	ಆದರೆ (mount_flags & EXT2_MF_MOUNTED) {
#ಆದರೆ HAVE_EXT2_IOCTLS
		ಪೂರ್ಣಾಂಕ f = 0;
#ಆದರೆ_ಅಂತ್ಯ
		strcat(jfile, "/.journal");

		/*
		 * If .../.journal already exists, make sure any
		 * immutable or append-only flags are cleared.
		 */
#ಆದರೆ defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		(ಶೂನ್ಯ) chflags (jfile, 0);
#ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ HAVE_EXT2_IOCTLS
		fd = open(jfile, O_RDONLY);
		ಆದರೆ (fd >= 0) {
			retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
			close(fd);
			ಆದರೆ (retval)
				ಹಿಂತಿರುಗಿ errno;
		}
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

		/* Create the journal file */
		ಆದರೆ ((fd = open(jfile, O_CREAT|O_WRONLY, 0600)) < 0)
			ಹಿಂತಿರುಗಿ errno;

		/* Note that we can't do lazy journal initialization for mounted
		 * filesystems, since the zero writing is also allocating the
		 * journal blocks.  We could use fallocate, but not all kernels
		 * support that, and creating a journal on a mounted ext2
		 * filesystems is extremely rare these days...  Ignore it. */
		flags &= ~EXT2_MKJOURNAL_LAZYINIT;

		ಆದರೆ ((retval = write_journal_file(fs, jfile, jparams, flags)))
			ಹೋಗು errout;

		/* Get inode number of the journal file */
		ಆದರೆ (fstat(fd, &st) < 0) {
			retval = errno;
			ಹೋಗು errout;
		}

#ಆದರೆ defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		retval = fchflags (fd, UF_NODUMP|UF_IMMUTABLE);
#ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ HAVE_EXT2_IOCTLS
		ಆದರೆ (ioctl(fd, EXT2_IOC_GETFLAGS, &f) < 0) {
			retval = errno;
			ಹೋಗು errout;
		}
		f |= EXT2_NODUMP_FL | EXT2_IMMUTABLE_FL;
		retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
		ಆದರೆ (retval) {
			retval = errno;
			ಹೋಗು errout;
		}

		ಆದರೆ (close(fd) < 0) {
			retval = errno;
			fd = -1;
			ಹೋಗು errout;
		}
		journal_ino = st.st_ino;
		memset(fs->super->s_jnl_ಖಂಡಗಳು, 0,
		       ಗಾತ್ರ(fs->super->s_jnl_ಖಂಡಗಳು));
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ ((mount_flags & EXT2_MF_BUSY) &&
		    !(fs->flags & EXT2_FLAG_EXCLUSIVE)) {
			retval = EBUSY;
			ಹೋಗು errout;
		}
		journal_ino = EXT2_JOURNAL_INO;
		ಆದರೆ ((retval = write_journal_inode(fs, journal_ino,
						  jparams, goal, flags)))
			ಹಿಂತಿರುಗಿ retval;
	}

	fs->super->s_journal_inum = journal_ino;
	fs->super->s_journal_dev = 0;
	memset(fs->super->s_journal_uuid, 0,
	       ಗಾತ್ರ(fs->super->s_journal_uuid));
	ext2fs_ಗಣ_feature_journal(fs->super);

	ext2fs_mark_super_dirty(fs);
	ಹಿಂತಿರುಗಿ 0;
errout:
	ಆದರೆ (fd >= 0)
		close(fd);
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_ಕೂಡಿಸು_journal_inode2(ext2_filsys fs, blk_t ಅಂಕೆ_ಖಂಡಗಳು,
				    blk64_t goal, ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ ext2fs_journal_params jparams;

	jparams.ಅಂಕೆ_journal_ಖಂಡಗಳು = ಅಂಕೆ_ಖಂಡಗಳು;
	jparams.ಅಂಕೆ_fc_ಖಂಡಗಳು = 0;

	ಹಿಂತಿರುಗಿ ext2fs_ಕೂಡಿಸು_journal_inode3(fs, &jparams, goal, flags);
}

errcode_t ext2fs_ಕೂಡಿಸು_journal_inode(ext2_filsys fs, blk_t ಅಂಕೆ_ಖಂಡಗಳು, ಪೂರ್ಣಾಂಕ flags)
{
	ಹಿಂತಿರುಗಿ ext2fs_ಕೂಡಿಸು_journal_inode2(fs, ಅಂಕೆ_ಖಂಡಗಳು, ~0ULL, flags);
}


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
ಮುಖ್ಯ(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
	errcode_t	retval;
	ಅಕ್ಷರ		*device_name;
	ext2_filsys	fs;

	ಆದರೆ (argc < 2) {
		fprintf(stderr, "Usage: %s filesystem\n", argv[0]);
		ನಿರ್ಗಮಿಸು(1);
	}
	device_name = argv[1];

	retval = ext2fs_open (device_name, EXT2_FLAG_RW, 0, 0,
			      unix_io_manager, &fs);
	ಆದರೆ (retval) {
		com_err(argv[0], retval, "while opening %s", device_name);
		ನಿರ್ಗಮಿಸು(1);
	}

	retval = ext2fs_ಕೂಡಿಸು_journal_inode(fs, JBD2_MIN_JOURNAL_ಖಂಡಗಳು, 0);
	ಆದರೆ (retval) {
		com_err(argv[0], retval, "while adding journal to %s",
			device_name);
		ನಿರ್ಗಮಿಸು(1);
	}
	retval = ext2fs_flush(fs);
	ಆದರೆ (retval) {
		ಬರೆ("Warning, had trouble writing out superblocks.\n");
	}
	ext2fs_close_free(&fs);
	ನಿರ್ಗಮಿಸು(0);

}
#ಆದರೆ_ಅಂತ್ಯ
