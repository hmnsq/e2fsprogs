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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_ERRNO_H
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_IOCTL_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/ioctl.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_NETINET_ਵਿੱਚ_H
#ਅਨ੍ਤਰ੍ਭਾਵ <netinet/in.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "e2p/e2p.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "kernel-jbd.h"

/*
 * This function automatically sets up the journal superblock and
 * returns it as an allocated block.
 */
errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
					   ਬਣਤਰ ext2fs_journal_params *jparams,
					   ਪੂਰਨ flags, ਅੱਖਰ **ret_jsb)
{
	errcode_t		retval;
	journal_superblock_t	*jsb;

	ਜੇ (jparams->ਅੰਕ_journal_ਖੰਡਾਂ < JBD2_MIN_JOURNAL_ਖੰਡਾਂ)
		ਵਾਪਸ EXT2_ET_JOURNAL_TOO_SMALL;

	ਜੇ ((retval = ext2fs_get_mem(fs->blocksize, &jsb)))
		ਵਾਪਸ retval;

	memset (jsb, 0, fs->blocksize);

	jsb->s_ਸਿਰਲੇਖ.h_magic = htonl(JBD2_MAGIC_ਸੰਖਿਆ);
	ਜੇ (flags & EXT2_MKJOURNAL_V1_SUPER)
		jsb->s_ਸਿਰਲੇਖ.h_blocktype = htonl(JBD2_SUPERBLOCK_V1);
	ਨਹੀਂਤਾਂ
		jsb->s_ਸਿਰਲੇਖ.h_blocktype = htonl(JBD2_SUPERBLOCK_V2);
	jsb->s_blocksize = htonl(fs->blocksize);
	jsb->s_maxlen = htonl(jparams->ਅੰਕ_journal_ਖੰਡਾਂ + jparams->ਅੰਕ_fc_ਖੰਡਾਂ);
	jsb->s_nr_users = htonl(1);
	jsb->s_ਪਹਿਲਾ = htonl(1);
	jsb->s_sequence = htonl(1);
	jsb->s_ਅੰਕ_fc_blks = htonl(jparams->ਅੰਕ_fc_ਖੰਡਾਂ);
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(jsb->s_uuid, fs->super->s_uuid, ਆਕਾਰ(fs->super->s_uuid));
	/*
	 * If we're creating an external journal device, we need to
	 * adjust these fields.
	 */
	ਜੇ (ext2fs_has_feature_journal_dev(fs->super)) {
		jsb->s_nr_users = 0;
		jsb->s_ਪਹਿਲਾ = htonl(ext2fs_journal_sb_ਸ਼ੁਰੂ(fs->blocksize) + 1);
	}

	*ret_jsb = (ਅੱਖਰ *) jsb;
	ਵਾਪਸ 0;
}

errcode_t ext2fs_create_journal_superblock(ext2_filsys fs, __u32 ਅੰਕ_ਖੰਡਾਂ,
					ਪੂਰਨ flags, ਅੱਖਰ **ret_sb)
{
	ਬਣਤਰ ext2fs_journal_params jparams;

	jparams.ਅੰਕ_journal_ਖੰਡਾਂ = ਅੰਕ_ਖੰਡਾਂ;
	jparams.ਅੰਕ_fc_ਖੰਡਾਂ = 0;

	ਵਾਪਸ ext2fs_create_journal_superblock2(fs, &jparams, flags, ret_sb);
}

/*
 * This function writes a journal using POSIX routines.  It is used
 * for creating external journals and creating journals on live
 * filesystems.
 */
ਸਥਿਰ errcode_t write_journal_file(ext2_filsys fs, ਅੱਖਰ *filename,
				    ਬਣਤਰ ext2fs_journal_params *jparams,
				    ਪੂਰਨ flags)
{
	errcode_t	retval;
	ਅੱਖਰ		*buf = 0;
	ਪੂਰਨ		fd, ret_ਮਾਪ;
	blk_t		i;

	ਜੇ ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		ਵਾਪਸ retval;

	/* Open the device or journal file */
	ਜੇ ((fd = open(filename, O_WRONLY)) < 0) {
		retval = errno;
		ਜਾਓ errfree;
	}

	/* Write the superblock out */
	retval = EXT2_ET_SHORT_WRITE;
	ret_ਮਾਪ = ਲਿਖ(fd, buf, fs->blocksize);
	ਜੇ (ret_ਮਾਪ < 0) {
		retval = errno;
		ਜਾਓ errout;
	}
	ਜੇ (ret_ਮਾਪ != (ਪੂਰਨ) fs->blocksize)
		ਜਾਓ errout;
	memset(buf, 0, fs->blocksize);

	ਜੇ (flags & EXT2_MKJOURNAL_LAZYINIT)
		ਜਾਓ success;

	ਲਈ (i = 1; i < jparams->ਅੰਕ_journal_ਖੰਡਾਂ + jparams->ਅੰਕ_fc_ਖੰਡਾਂ; i++) {
		ret_ਮਾਪ = ਲਿਖ(fd, buf, fs->blocksize);
		ਜੇ (ret_ਮਾਪ < 0) {
			retval = errno;
			ਜਾਓ errout;
		}
		ਜੇ (ret_ਮਾਪ != (ਪੂਰਨ) fs->blocksize)
			ਜਾਓ errout;
	}

success:
	retval = 0;
errout:
	close(fd);
errfree:
	ext2fs_free_mem(&buf);
	ਵਾਪਸ retval;
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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_STRIDE_ਲੰਮਾਈ (4194304 / (ਪੂਰਨ) fs->blocksize)
errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, ਪੂਰਨ ਅੰਕ,
			      blk64_t *ret_blk, ਪੂਰਨ *ret_ਗਿਣਤੀ)
{
	ਪੂਰਨ		j, ਗਿਣਤੀ_2;
	ਸਥਿਰ ਖਾਲੀ	*buf;
	ਸਥਿਰ ਪੂਰਨ	stride_ਲੰਮਾਈ;
	errcode_t	retval;

	/* If fs is null, clean up the static buffer and return */
	ਜੇ (!fs) {
		ਜੇ (buf) {
			ਮੁਕਤ(buf);
			buf = 0;
			stride_ਲੰਮਾਈ = 0;
		}
		ਵਾਪਸ 0;
	}

	/* Deal with zeroing less than 1 block */
	ਜੇ (ਅੰਕ <= 0)
		ਵਾਪਸ 0;

	/* Try a zero out command, if supported */
	retval = io_channel_zeroout(fs->io, blk, ਅੰਕ);
	ਜੇ (retval == 0)
		ਵਾਪਸ 0;

	/* Allocate the zeroizing buffer if necessary */
	ਜੇ (ਅੰਕ > stride_ਲੰਮਾਈ && stride_ਲੰਮਾਈ < MAX_STRIDE_ਲੰਮਾਈ) {
		ਖਾਲੀ *p;
		ਪੂਰਨ new_stride = ਅੰਕ;

		ਜੇ (new_stride > MAX_STRIDE_ਲੰਮਾਈ)
			new_stride = MAX_STRIDE_ਲੰਮਾਈ;
		p = realloc(buf, fs->blocksize * new_stride);
		ਜੇ (!p)
			ਵਾਪਸ EXT2_ET_NO_ਸਮ੍ਰਿਤੀ;
		buf = p;
		stride_ਲੰਮਾਈ = new_stride;
		memset(buf, 0, fs->blocksize * stride_ਲੰਮਾਈ);
	}
	/* OK, do the write loop */
	j=0;
	ਜਦੋਂ (j < ਅੰਕ) {
		ਜੇ (blk % stride_ਲੰਮਾਈ) {
			ਗਿਣਤੀ_2 = stride_ਲੰਮਾਈ - (blk % stride_ਲੰਮਾਈ);
			ਜੇ (ਗਿਣਤੀ_2 > (ਅੰਕ - j))
				ਗਿਣਤੀ_2 = ਅੰਕ - j;
		} ਨਹੀਂਤਾਂ {
			ਗਿਣਤੀ_2 = ਅੰਕ - j;
			ਜੇ (ਗਿਣਤੀ_2 > stride_ਲੰਮਾਈ)
				ਗਿਣਤੀ_2 = stride_ਲੰਮਾਈ;
		}
		retval = io_channel_write_blk64(fs->io, blk, ਗਿਣਤੀ_2, buf);
		ਜੇ (retval) {
			ਜੇ (ret_ਗਿਣਤੀ)
				*ret_ਗਿਣਤੀ = ਗਿਣਤੀ_2;
			ਜੇ (ret_blk)
				*ret_blk = blk;
			ਵਾਪਸ retval;
		}
		j += ਗਿਣਤੀ_2; blk += ਗਿਣਤੀ_2;
	}
	ਵਾਪਸ 0;
}

errcode_t ext2fs_zero_ਖੰਡਾਂ(ext2_filsys fs, blk_t blk, ਪੂਰਨ ਅੰਕ,
			     blk_t *ret_blk, ਪੂਰਨ *ret_ਗਿਣਤੀ)
{
	blk64_t ret_blk2;
	errcode_t retval;

	retval = ext2fs_zero_blocks2(fs, blk, ਅੰਕ, &ret_blk2, ret_ਗਿਣਤੀ);
	ਜੇ (retval)
		*ret_blk = (blk_t) ret_blk2;
	ਵਾਪਸ retval;
}

/*
 * Calculate the initial goal block to be roughly at the middle of the
 * filesystem.  Pick a group that has the largest number of free
 * blocks.
 */
ਸਥਿਰ blk64_t get_midpoint_journal_ਖੰਡ(ext2_filsys fs)
{
	dgrp_t	ਸਮੂਹ, ਸ਼ੁਰੂ, ਸਿਰਾ, i, log_flex;

	ਸਮੂਹ = ext2fs_ਸਮੂਹ_ਦਾ_blk2(fs, (ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super) -
					 fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ) / 2);
	log_flex = 1U << fs->super->s_log_groups_ਪ੍ਰਤੀ_flex;
	ਜੇ (fs->super->s_log_groups_ਪ੍ਰਤੀ_flex && (ਸਮੂਹ > log_flex)) {
		ਸਮੂਹ = ਸਮੂਹ & ~(log_flex - 1);
		ਜਦੋਂ ((ਸਮੂਹ < fs->ਸਮੂਹ_desc_ਗਿਣਤੀ) &&
		       ext2fs_bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ(fs, ਸਮੂਹ) == 0)
			ਸਮੂਹ++;
		ਜੇ (ਸਮੂਹ == fs->ਸਮੂਹ_desc_ਗਿਣਤੀ)
			ਸਮੂਹ = 0;
		ਸ਼ੁਰੂ = ਸਮੂਹ;
	} ਨਹੀਂਤਾਂ
		ਸ਼ੁਰੂ = (ਸਮੂਹ > 0) ? ਸਮੂਹ-1 : ਸਮੂਹ;
	ਸਿਰਾ = ((ਸਮੂਹ+1) < fs->ਸਮੂਹ_desc_ਗਿਣਤੀ) ? ਸਮੂਹ+1 : ਸਮੂਹ;
	ਸਮੂਹ = ਸ਼ੁਰੂ;
	ਲਈ (i = ਸ਼ੁਰੂ + 1; i <= ਸਿਰਾ; i++)
		ਜੇ (ext2fs_bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ(fs, i) >
		    ext2fs_bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ(fs, ਸਮੂਹ))
			ਸਮੂਹ = i;
	ਵਾਪਸ ext2fs_ਸਮੂਹ_ਪਹਿਲਾ_block2(fs, ਸਮੂਹ);
}

/*
 * This function creates a journal using direct I/O routines.
 */
ਸਥਿਰ errcode_t write_journal_inode(ext2_filsys fs, ext2_ino_t journal_ino,
				     ਬਣਤਰ ext2fs_journal_params *jparams,
				     blk64_t goal, ਪੂਰਨ flags)
{
	ਅੱਖਰ			*buf;
	errcode_t		retval;
	ਬਣਤਰ ext2_inode	inode;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ	inode_ਮਾਪ;
	ਪੂਰਨ			falloc_flags = EXT2_FALLOCATE_FORCE_INIT;
	blk64_t			zblk;
	time_t			now;

	ਜੇ ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		ਵਾਪਸ retval;

	ਜੇ ((retval = ext2fs_read_bitmaps(fs)))
		ਜਾਓ out2;

	ਜੇ ((retval = ext2fs_read_inode(fs, journal_ino, &inode)))
		ਜਾਓ out2;

	ਜੇ (inode.i_ਖੰਡਾਂ > 0) {
		retval = EEXIST;
		ਜਾਓ out2;
	}

	ਜੇ (goal == ~0ULL)
		goal = get_midpoint_journal_ਖੰਡ(fs);

	ਜੇ (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;

	ਜੇ (!(flags & EXT2_MKJOURNAL_LAZYINIT))
		falloc_flags |= EXT2_FALLOCATE_ZERO_ਖੰਡਾਂ;

	inode_ਮਾਪ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ)fs->blocksize *
			(jparams->ਅੰਕ_journal_ਖੰਡਾਂ + jparams->ਅੰਕ_fc_ਖੰਡਾਂ);
	now = ext2fs_P_get_ਸਮਾਂ(fs);
	ext2fs_inode_xtime_ਸੈੱਟ(&inode, i_mtime, now);
	ext2fs_inode_xtime_ਸੈੱਟ(&inode, i_ctime, now);
	inode.i_links_ਗਿਣਤੀ = 1;
	inode.i_ਬਹੁਲਕ = LINUX_S_IFREG | 0600;
	retval = ext2fs_inode_ਮਾਪ_ਸੈੱਟ(fs, &inode, inode_ਮਾਪ);
	ਜੇ (retval)
		ਜਾਓ out2;

	retval = ext2fs_fallocate(fs, falloc_flags, journal_ino,
				  &inode, goal, 0,
				  jparams->ਅੰਕ_journal_ਖੰਡਾਂ + jparams->ਅੰਕ_fc_ਖੰਡਾਂ);
	ਜੇ (retval)
		ਜਾਓ out2;

	ਜੇ ((retval = ext2fs_write_new_inode(fs, journal_ino, &inode)))
		ਜਾਓ out2;

	retval = ext2fs_bmap2(fs, journal_ino, &inode, ਨਿਰਾ, 0, 0, ਨਿਰਾ, &zblk);
	ਜੇ (retval)
		ਜਾਓ out2;

	retval = io_channel_write_blk64(fs->io, zblk, 1, buf);
	ਜੇ (retval)
		ਜਾਓ out2;

	ਸਮ੍ਰਿਤੀ_ਨਕਲ(fs->super->s_jnl_ਖੰਡਾਂ, inode.i_ਖੰਡ, EXT2_N_ਖੰਡਾਂ*4);
	fs->super->s_jnl_ਖੰਡਾਂ[15] = inode.i_ਮਾਪ_ਉੱਚਾ;
	fs->super->s_jnl_ਖੰਡਾਂ[16] = inode.i_ਮਾਪ;
	fs->super->s_jnl_backup_type = EXT3_JNL_BACKUP_ਖੰਡਾਂ;
	ext2fs_mark_super_dirty(fs);

out2:
	ext2fs_free_mem(&buf);
	ਵਾਪਸ retval;
}

/*
 * Find a reasonable journal file size (in blocks) given the number of blocks
 * in the filesystem.  For very small filesystems, it is not reasonable to
 * have a journal that fills more than half of the filesystem.
 *
 * n.b. comments assume 4k blocks
 */
ਪੂਰਨ ext2fs_default_journal_ਮਾਪ(__u64 ਅੰਕ_ਖੰਡਾਂ)
{
	ਜੇ (ਅੰਕ_ਖੰਡਾਂ < 2048)
		ਵਾਪਸ -1;
	ਜੇ (ਅੰਕ_ਖੰਡਾਂ < 32768)		/* 128 MB */
		ਵਾਪਸ (1024);			/* 4 MB */
	ਜੇ (ਅੰਕ_ਖੰਡਾਂ < 256*1024)	/* 1 GB */
		ਵਾਪਸ (4096);			/* 16 MB */
	ਜੇ (ਅੰਕ_ਖੰਡਾਂ < 512*1024)	/* 2 GB */
		ਵਾਪਸ (8192);			/* 32 MB */
	ਜੇ (ਅੰਕ_ਖੰਡਾਂ < 4096*1024)	/* 16 GB */
		ਵਾਪਸ (16384);			/* 64 MB */
	ਜੇ (ਅੰਕ_ਖੰਡਾਂ < 8192*1024)	/* 32 GB */
		ਵਾਪਸ (32768);			/* 128 MB */
	ਜੇ (ਅੰਕ_ਖੰਡਾਂ < 16384*1024)	/* 64 GB */
		ਵਾਪਸ (65536);			/* 256 MB */
	ਜੇ (ਅੰਕ_ਖੰਡਾਂ < 32768*1024)	/* 128 GB */
		ਵਾਪਸ (131072);		/* 512 MB */
	ਵਾਪਸ 262144;				/* 1 GB */
}

errcode_t ext2fs_get_journal_params(ਬਣਤਰ ext2fs_journal_params *params,
		ext2_filsys fs)
{
	blk_t ਕੁੱਲ_blks;
	ਪੂਰਨ ret;

	memset(params, 0, ਆਕਾਰ(*params));
	ਜੇ (ext2fs_has_feature_journal_dev(fs->super)) {
		ਕੁੱਲ_blks = ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super);
		ਜੇ (ਕੁੱਲ_blks < JBD2_MIN_JOURNAL_ਖੰਡਾਂ)
			ਵਾਪਸ EXT2_ET_JOURNAL_TOO_SMALL;

		ਜੇ (!ext2fs_has_feature_fast_commit(fs->super)) {
			params->ਅੰਕ_journal_ਖੰਡਾਂ = ਕੁੱਲ_blks;
			params->ਅੰਕ_fc_ਖੰਡਾਂ = 0;
			ਵਾਪਸ 0;
		}
		params->ਅੰਕ_journal_ਖੰਡਾਂ = ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super) *
				EXT2_JOURNAL_ਮੰਜ਼ਿਲ_FC_BLKS_ਅਨੁਪਾਤ /
				(EXT2_JOURNAL_ਮੰਜ਼ਿਲ_FC_BLKS_ਅਨੁਪਾਤ + 1);
		ਜੇ (JBD2_MIN_JOURNAL_ਖੰਡਾਂ > params->ਅੰਕ_journal_ਖੰਡਾਂ)
			params->ਅੰਕ_journal_ਖੰਡਾਂ = JBD2_MIN_JOURNAL_ਖੰਡਾਂ;
		params->ਅੰਕ_fc_ਖੰਡਾਂ = ਕੁੱਲ_blks - params->ਅੰਕ_journal_ਖੰਡਾਂ;
		ਵਾਪਸ 0;
	}

	ret = ext2fs_default_journal_ਮਾਪ(ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super));
	ਜੇ (ret < 0)
		ਵਾਪਸ EXT2_ET_JOURNAL_TOO_SMALL;

	params->ਅੰਕ_journal_ਖੰਡਾਂ = ret;
	ਜੇ (ext2fs_has_feature_fast_commit(fs->super))
		params->ਅੰਕ_fc_ਖੰਡਾਂ = params->ਅੰਕ_journal_ਖੰਡਾਂ /
			EXT2_JOURNAL_ਮੰਜ਼ਿਲ_FC_BLKS_ਅਨੁਪਾਤ;
	ਵਾਪਸ 0;
}

ਪੂਰਨ ext2fs_journal_sb_ਸ਼ੁਰੂ(ਪੂਰਨ blocksize)
{
	ਜੇ (blocksize == EXT2_MIN_ਖੰਡ_ਮਾਪ)
		ਵਾਪਸ 2;
	ਵਾਪਸ 1;
}

/*
 * This function adds a journal device to a filesystem
 */
errcode_t ext2fs_ਜੋੜੋ_journal_device(ext2_filsys fs, ext2_filsys journal_dev)
{
	ਬਣਤਰ stat	st;
	errcode_t	retval;
	ਅੱਖਰ		buf[SUPERBLOCK_ਮਾਪ];
	journal_superblock_t	*jsb;
	ਪੂਰਨ		ਸ਼ੁਰੂ;
	__u32		i, nr_users;

	/* Make sure the device exists and is a block device */
	ਜੇ (stat(journal_dev->device_name, &st) < 0)
		ਵਾਪਸ errno;

	ਜੇ (!S_ISBLK(st.st_ਬਹੁਲਕ))
		ਵਾਪਸ EXT2_ET_JOURNAL_NOT_ਖੰਡ; /* Must be a block device */

	/* Get the journal superblock */
	ਸ਼ੁਰੂ = ext2fs_journal_sb_ਸ਼ੁਰੂ(journal_dev->blocksize);
	ਜੇ ((retval = io_channel_read_blk64(journal_dev->io, ਸ਼ੁਰੂ,
					    -SUPERBLOCK_ਮਾਪ,
					    buf)))
		ਵਾਪਸ retval;

	jsb = (journal_superblock_t *) buf;
	ਜੇ ((jsb->s_ਸਿਰਲੇਖ.h_magic != (ਬਿਨਾਨਿਸ਼ਾਨੀ) ntohl(JBD2_MAGIC_ਸੰਖਿਆ)) ||
	    (jsb->s_ਸਿਰਲੇਖ.h_blocktype != (ਬਿਨਾਨਿਸ਼ਾਨੀ) ntohl(JBD2_SUPERBLOCK_V2)))
		ਵਾਪਸ EXT2_ET_NO_JOURNAL_SB;

	ਜੇ (ntohl(jsb->s_blocksize) != (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) fs->blocksize)
		ਵਾਪਸ EXT2_ET_UNEXPECTED_ਖੰਡ_ਮਾਪ;

	/* Check and see if this filesystem has already been added */
	nr_users = ntohl(jsb->s_nr_users);
	ਜੇ (nr_users > JBD2_USERS_MAX)
		ਵਾਪਸ EXT2_ET_CORRUPT_JOURNAL_SB;
	ਲਈ (i=0; i < nr_users; i++) {
		ਜੇ (memcmp(fs->super->s_uuid,
			   &jsb->s_users[i*16], 16) == 0)
			ਰੋਕੋ;
	}
	ਜੇ (i >= nr_users) {
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(&jsb->s_users[nr_users*16],
		       fs->super->s_uuid, 16);
		jsb->s_nr_users = htonl(nr_users+1);
	}

	/* Writeback the journal superblock */
	ਜੇ ((retval = io_channel_write_blk64(journal_dev->io, ਸ਼ੁਰੂ,
					    -SUPERBLOCK_ਮਾਪ, buf)))
		ਵਾਪਸ retval;

	fs->super->s_journal_inum = 0;
	fs->super->s_journal_dev = st.st_rdev;
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(fs->super->s_journal_uuid, jsb->s_uuid,
	       ਆਕਾਰ(fs->super->s_journal_uuid));
	memset(fs->super->s_jnl_ਖੰਡਾਂ, 0, ਆਕਾਰ(fs->super->s_jnl_ਖੰਡਾਂ));
	ext2fs_ਸੈੱਟ_feature_journal(fs->super);
	ext2fs_mark_super_dirty(fs);
	ਵਾਪਸ 0;
}

/*
 * This function adds a journal inode to a filesystem, using either
 * POSIX routines if the filesystem is mounted, or using direct I/O
 * functions if it is not.
 */
errcode_t ext2fs_ਜੋੜੋ_journal_inode3(ext2_filsys fs, ਬਣਤਰ ext2fs_journal_params *jparams,
				    blk64_t goal, ਪੂਰਨ flags)
{
	errcode_t		retval;
	ext2_ino_t		journal_ino;
	ਬਣਤਰ stat		st;
	ਅੱਖਰ			jfile[1024];
	ਪੂਰਨ			mount_flags;
	ਪੂਰਨ			fd = -1;

	ਜੇ (flags & EXT2_MKJOURNAL_NO_MNT_CHECK)
		mount_flags = 0;
	ਨਹੀਂਤਾਂ ਜੇ ((retval = ext2fs_check_mount_ਬਿੰਦੂ(fs->device_name,
						    &mount_flags,
						    jfile, ਆਕਾਰ(jfile)-10)))
		ਵਾਪਸ retval;

	ਜੇ (mount_flags & EXT2_MF_MOUNTED) {
#ਜੇ HAVE_EXT2_IOCTLS
		ਪੂਰਨ f = 0;
#ਜੇ_ਅੰਤ
		strcat(jfile, "/.journal");

		/*
		 * If .../.journal already exists, make sure any
		 * immutable or append-only flags are cleared.
		 */
#ਜੇ defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		(ਖਾਲੀ) chflags (jfile, 0);
#ਨਹੀਂਤਾਂ
#ਜੇ HAVE_EXT2_IOCTLS
		fd = open(jfile, O_RDONLY);
		ਜੇ (fd >= 0) {
			retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
			close(fd);
			ਜੇ (retval)
				ਵਾਪਸ errno;
		}
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

		/* Create the journal file */
		ਜੇ ((fd = open(jfile, O_CREAT|O_WRONLY, 0600)) < 0)
			ਵਾਪਸ errno;

		/* Note that we can't do lazy journal initialization for mounted
		 * filesystems, since the zero writing is also allocating the
		 * journal blocks.  We could use fallocate, but not all kernels
		 * support that, and creating a journal on a mounted ext2
		 * filesystems is extremely rare these days...  Ignore it. */
		flags &= ~EXT2_MKJOURNAL_LAZYINIT;

		ਜੇ ((retval = write_journal_file(fs, jfile, jparams, flags)))
			ਜਾਓ errout;

		/* Get inode number of the journal file */
		ਜੇ (fstat(fd, &st) < 0) {
			retval = errno;
			ਜਾਓ errout;
		}

#ਜੇ defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		retval = fchflags (fd, UF_NODUMP|UF_IMMUTABLE);
#ਨਹੀਂਤਾਂ
#ਜੇ HAVE_EXT2_IOCTLS
		ਜੇ (ioctl(fd, EXT2_IOC_GETFLAGS, &f) < 0) {
			retval = errno;
			ਜਾਓ errout;
		}
		f |= EXT2_NODUMP_FL | EXT2_IMMUTABLE_FL;
		retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
		ਜੇ (retval) {
			retval = errno;
			ਜਾਓ errout;
		}

		ਜੇ (close(fd) < 0) {
			retval = errno;
			fd = -1;
			ਜਾਓ errout;
		}
		journal_ino = st.st_ino;
		memset(fs->super->s_jnl_ਖੰਡਾਂ, 0,
		       ਆਕਾਰ(fs->super->s_jnl_ਖੰਡਾਂ));
	} ਨਹੀਂਤਾਂ {
		ਜੇ ((mount_flags & EXT2_MF_BUSY) &&
		    !(fs->flags & EXT2_FLAG_EXCLUSIVE)) {
			retval = EBUSY;
			ਜਾਓ errout;
		}
		journal_ino = EXT2_JOURNAL_INO;
		ਜੇ ((retval = write_journal_inode(fs, journal_ino,
						  jparams, goal, flags)))
			ਵਾਪਸ retval;
	}

	fs->super->s_journal_inum = journal_ino;
	fs->super->s_journal_dev = 0;
	memset(fs->super->s_journal_uuid, 0,
	       ਆਕਾਰ(fs->super->s_journal_uuid));
	ext2fs_ਸੈੱਟ_feature_journal(fs->super);

	ext2fs_mark_super_dirty(fs);
	ਵਾਪਸ 0;
errout:
	ਜੇ (fd >= 0)
		close(fd);
	ਵਾਪਸ retval;
}

errcode_t ext2fs_ਜੋੜੋ_journal_inode2(ext2_filsys fs, blk_t ਅੰਕ_ਖੰਡਾਂ,
				    blk64_t goal, ਪੂਰਨ flags)
{
	ਬਣਤਰ ext2fs_journal_params jparams;

	jparams.ਅੰਕ_journal_ਖੰਡਾਂ = ਅੰਕ_ਖੰਡਾਂ;
	jparams.ਅੰਕ_fc_ਖੰਡਾਂ = 0;

	ਵਾਪਸ ext2fs_ਜੋੜੋ_journal_inode3(fs, &jparams, goal, flags);
}

errcode_t ext2fs_ਜੋੜੋ_journal_inode(ext2_filsys fs, blk_t ਅੰਕ_ਖੰਡਾਂ, ਪੂਰਨ flags)
{
	ਵਾਪਸ ext2fs_ਜੋੜੋ_journal_inode2(fs, ਅੰਕ_ਖੰਡਾਂ, ~0ULL, flags);
}


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DEBUG
ਮੁੱਖ(ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
	errcode_t	retval;
	ਅੱਖਰ		*device_name;
	ext2_filsys	fs;

	ਜੇ (argc < 2) {
		fprintf(stderr, "Usage: %s filesystem\n", argv[0]);
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	device_name = argv[1];

	retval = ext2fs_open (device_name, EXT2_FLAG_RW, 0, 0,
			      unix_io_manager, &fs);
	ਜੇ (retval) {
		com_err(argv[0], retval, "while opening %s", device_name);
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}

	retval = ext2fs_ਜੋੜੋ_journal_inode(fs, JBD2_MIN_JOURNAL_ਖੰਡਾਂ, 0);
	ਜੇ (retval) {
		com_err(argv[0], retval, "while adding journal to %s",
			device_name);
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	retval = ext2fs_flush(fs);
	ਜੇ (retval) {
		ਲਿਖੋ("Warning, had trouble writing out superblocks.\n");
	}
	ext2fs_close_free(&fs);
	ਬਾਹਰ_ਨਿਕਲੋ(0);

}
#ਜੇ_ਅੰਤ
