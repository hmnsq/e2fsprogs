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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <assert.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_IOCTL_H
#అన్తర్భావ <sys/ioctl.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_NETINET_లోపల_H
#అన్తర్భావ <netinet/in.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "e2p/e2p.h"
#అన్తర్భావ "ext2fsP.h"

#అన్తర్భావ "kernel-jbd.h"

/*
 * This function automatically sets up the journal superblock and
 * returns it as an allocated block.
 */
errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
					   నిర్మాణం ext2fs_journal_params *jparams,
					   పూర్ణాంకం flags, అక్షరం **ret_jsb)
{
	errcode_t		retval;
	journal_superblock_t	*jsb;

	ఒకవేళ (jparams->అంకె_journal_ఖండాలు < JBD2_MIN_JOURNAL_ఖండాలు)
		తిరిగి EXT2_ET_JOURNAL_TOO_SMALL;

	ఒకవేళ ((retval = ext2fs_get_mem(fs->blocksize, &jsb)))
		తిరిగి retval;

	memset (jsb, 0, fs->blocksize);

	jsb->s_శీర్షిక.h_magic = htonl(JBD2_MAGIC_సంఖ్య);
	ఒకవేళ (flags & EXT2_MKJOURNAL_V1_SUPER)
		jsb->s_శీర్షిక.h_blocktype = htonl(JBD2_SUPERBLOCK_V1);
	లేకపోతే
		jsb->s_శీర్షిక.h_blocktype = htonl(JBD2_SUPERBLOCK_V2);
	jsb->s_blocksize = htonl(fs->blocksize);
	jsb->s_maxlen = htonl(jparams->అంకె_journal_ఖండాలు + jparams->అంకె_fc_ఖండాలు);
	jsb->s_nr_users = htonl(1);
	jsb->s_మొదటి = htonl(1);
	jsb->s_sequence = htonl(1);
	jsb->s_అంకె_fc_blks = htonl(jparams->అంకె_fc_ఖండాలు);
	జ్ఞాపకం_నకలు(jsb->s_uuid, fs->super->s_uuid, పరిమాణం(fs->super->s_uuid));
	/*
	 * If we're creating an external journal device, we need to
	 * adjust these fields.
	 */
	ఒకవేళ (ext2fs_has_feature_journal_dev(fs->super)) {
		jsb->s_nr_users = 0;
		jsb->s_మొదటి = htonl(ext2fs_journal_sb_ప్రారంభం(fs->blocksize) + 1);
	}

	*ret_jsb = (అక్షరం *) jsb;
	తిరిగి 0;
}

errcode_t ext2fs_create_journal_superblock(ext2_filsys fs, __u32 అంకె_ఖండాలు,
					పూర్ణాంకం flags, అక్షరం **ret_sb)
{
	నిర్మాణం ext2fs_journal_params jparams;

	jparams.అంకె_journal_ఖండాలు = అంకె_ఖండాలు;
	jparams.అంకె_fc_ఖండాలు = 0;

	తిరిగి ext2fs_create_journal_superblock2(fs, &jparams, flags, ret_sb);
}

/*
 * This function writes a journal using POSIX routines.  It is used
 * for creating external journals and creating journals on live
 * filesystems.
 */
స్థిర errcode_t write_journal_file(ext2_filsys fs, అక్షరం *filename,
				    నిర్మాణం ext2fs_journal_params *jparams,
				    పూర్ణాంకం flags)
{
	errcode_t	retval;
	అక్షరం		*buf = 0;
	పూర్ణాంకం		fd, ret_కొలత;
	blk_t		i;

	ఒకవేళ ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		తిరిగి retval;

	/* Open the device or journal file */
	ఒకవేళ ((fd = open(filename, O_WRONLY)) < 0) {
		retval = errno;
		వెళ్ళు errfree;
	}

	/* Write the superblock out */
	retval = EXT2_ET_SHORT_WRITE;
	ret_కొలత = లిఖ(fd, buf, fs->blocksize);
	ఒకవేళ (ret_కొలత < 0) {
		retval = errno;
		వెళ్ళు errout;
	}
	ఒకవేళ (ret_కొలత != (పూర్ణాంకం) fs->blocksize)
		వెళ్ళు errout;
	memset(buf, 0, fs->blocksize);

	ఒకవేళ (flags & EXT2_MKJOURNAL_LAZYINIT)
		వెళ్ళు success;

	కొరకు (i = 1; i < jparams->అంకె_journal_ఖండాలు + jparams->అంకె_fc_ఖండాలు; i++) {
		ret_కొలత = లిఖ(fd, buf, fs->blocksize);
		ఒకవేళ (ret_కొలత < 0) {
			retval = errno;
			వెళ్ళు errout;
		}
		ఒకవేళ (ret_కొలత != (పూర్ణాంకం) fs->blocksize)
			వెళ్ళు errout;
	}

success:
	retval = 0;
errout:
	close(fd);
errfree:
	ext2fs_free_mem(&buf);
	తిరిగి retval;
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
#నిర్వచించు MAX_STRIDE_నిడివి (4194304 / (పూర్ణాంకం) fs->blocksize)
errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, పూర్ణాంకం అంకె,
			      blk64_t *ret_blk, పూర్ణాంకం *ret_లెక్క)
{
	పూర్ణాంకం		j, లెక్క;
	స్థిర శూన్య	*buf;
	స్థిర పూర్ణాంకం	stride_నిడివి;
	errcode_t	retval;

	/* If fs is null, clean up the static buffer and return */
	ఒకవేళ (!fs) {
		ఒకవేళ (buf) {
			విడుదల(buf);
			buf = 0;
			stride_నిడివి = 0;
		}
		తిరిగి 0;
	}

	/* Deal with zeroing less than 1 block */
	ఒకవేళ (అంకె <= 0)
		తిరిగి 0;

	/* Try a zero out command, if supported */
	retval = io_channel_zeroout(fs->io, blk, అంకె);
	ఒకవేళ (retval == 0)
		తిరిగి 0;

	/* Allocate the zeroizing buffer if necessary */
	ఒకవేళ (అంకె > stride_నిడివి && stride_నిడివి < MAX_STRIDE_నిడివి) {
		శూన్య *p;
		పూర్ణాంకం new_stride = అంకె;

		ఒకవేళ (new_stride > MAX_STRIDE_నిడివి)
			new_stride = MAX_STRIDE_నిడివి;
		p = realloc(buf, fs->blocksize * new_stride);
		ఒకవేళ (!p)
			తిరిగి EXT2_ET_NO_జ్ఞాపకం;
		buf = p;
		stride_నిడివి = new_stride;
		memset(buf, 0, fs->blocksize * stride_నిడివి);
	}
	/* OK, do the write loop */
	j=0;
	అయితే (j < అంకె) {
		ఒకవేళ (blk % stride_నిడివి) {
			లెక్క = stride_నిడివి - (blk % stride_నిడివి);
			ఒకవేళ (లెక్క > (అంకె - j))
				లెక్క = అంకె - j;
		} లేకపోతే {
			లెక్క = అంకె - j;
			ఒకవేళ (లెక్క > stride_నిడివి)
				లెక్క = stride_నిడివి;
		}
		retval = io_channel_write_blk64(fs->io, blk, లెక్క, buf);
		ఒకవేళ (retval) {
			ఒకవేళ (ret_లెక్క)
				*ret_లెక్క = లెక్క;
			ఒకవేళ (ret_blk)
				*ret_blk = blk;
			తిరిగి retval;
		}
		j += లెక్క; blk += లెక్క;
	}
	తిరిగి 0;
}

errcode_t ext2fs_zero_ఖండాలు(ext2_filsys fs, blk_t blk, పూర్ణాంకం అంకె,
			     blk_t *ret_blk, పూర్ణాంకం *ret_లెక్క)
{
	blk64_t ret_blk2;
	errcode_t retval;

	retval = ext2fs_zero_blocks2(fs, blk, అంకె, &ret_blk2, ret_లెక్క);
	ఒకవేళ (retval)
		*ret_blk = (blk_t) ret_blk2;
	తిరిగి retval;
}

/*
 * Calculate the initial goal block to be roughly at the middle of the
 * filesystem.  Pick a group that has the largest number of free
 * blocks.
 */
స్థిర blk64_t get_midpoint_journal_ఖండం(ext2_filsys fs)
{
	dgrp_t	సమూహం, ప్రారంభం, చివర, i, log_flex;

	సమూహం = ext2fs_సమూహం_యొక్క_blk2(fs, (ext2fs_ఖండాలు_లెక్క(fs->super) -
					 fs->super->s_మొదటి_దత్తాంశం_ఖండం) / 2);
	log_flex = 1U << fs->super->s_log_groups_చొప్పున_flex;
	ఒకవేళ (fs->super->s_log_groups_చొప్పున_flex && (సమూహం > log_flex)) {
		సమూహం = సమూహం & ~(log_flex - 1);
		అయితే ((సమూహం < fs->సమూహం_desc_లెక్క) &&
		       ext2fs_bg_free_ఖండాలు_లెక్క(fs, సమూహం) == 0)
			సమూహం++;
		ఒకవేళ (సమూహం == fs->సమూహం_desc_లెక్క)
			సమూహం = 0;
		ప్రారంభం = సమూహం;
	} లేకపోతే
		ప్రారంభం = (సమూహం > 0) ? సమూహం-1 : సమూహం;
	చివర = ((సమూహం+1) < fs->సమూహం_desc_లెక్క) ? సమూహం+1 : సమూహం;
	సమూహం = ప్రారంభం;
	కొరకు (i = ప్రారంభం + 1; i <= చివర; i++)
		ఒకవేళ (ext2fs_bg_free_ఖండాలు_లెక్క(fs, i) >
		    ext2fs_bg_free_ఖండాలు_లెక్క(fs, సమూహం))
			సమూహం = i;
	తిరిగి ext2fs_సమూహం_మొదటి_block2(fs, సమూహం);
}

/*
 * This function creates a journal using direct I/O routines.
 */
స్థిర errcode_t write_journal_inode(ext2_filsys fs, ext2_ino_t journal_ino,
				     నిర్మాణం ext2fs_journal_params *jparams,
				     blk64_t goal, పూర్ణాంకం flags)
{
	అక్షరం			*buf;
	errcode_t		retval;
	నిర్మాణం ext2_inode	inode;
	గుర్తులేని దీర్ఘ దీర్ఘ	inode_కొలత;
	పూర్ణాంకం			falloc_flags = EXT2_FALLOCATE_FORCE_INIT;
	blk64_t			zblk;
	time_t			now;

	ఒకవేళ ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		తిరిగి retval;

	ఒకవేళ ((retval = ext2fs_read_bitmaps(fs)))
		వెళ్ళు out2;

	ఒకవేళ ((retval = ext2fs_read_inode(fs, journal_ino, &inode)))
		వెళ్ళు out2;

	ఒకవేళ (inode.i_ఖండాలు > 0) {
		retval = EEXIST;
		వెళ్ళు out2;
	}

	ఒకవేళ (goal == ~0ULL)
		goal = get_midpoint_journal_ఖండం(fs);

	ఒకవేళ (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;

	ఒకవేళ (!(flags & EXT2_MKJOURNAL_LAZYINIT))
		falloc_flags |= EXT2_FALLOCATE_ZERO_ఖండాలు;

	inode_కొలత = (గుర్తులేని దీర్ఘ దీర్ఘ)fs->blocksize *
			(jparams->అంకె_journal_ఖండాలు + jparams->అంకె_fc_ఖండాలు);
	now = ext2fs_P_get_సమయం(fs);
	ext2fs_inode_xtime_సమితి(&inode, i_mtime, now);
	ext2fs_inode_xtime_సమితి(&inode, i_ctime, now);
	inode.i_links_లెక్క = 1;
	inode.i_బహుళకం = LINUX_S_IFREG | 0600;
	retval = ext2fs_inode_కొలత_సమితి(fs, &inode, inode_కొలత);
	ఒకవేళ (retval)
		వెళ్ళు out2;

	retval = ext2fs_fallocate(fs, falloc_flags, journal_ino,
				  &inode, goal, 0,
				  jparams->అంకె_journal_ఖండాలు + jparams->అంకె_fc_ఖండాలు);
	ఒకవేళ (retval)
		వెళ్ళు out2;

	ఒకవేళ ((retval = ext2fs_write_new_inode(fs, journal_ino, &inode)))
		వెళ్ళు out2;

	retval = ext2fs_bmap2(fs, journal_ino, &inode, ఖాళీ, 0, 0, ఖాళీ, &zblk);
	ఒకవేళ (retval)
		వెళ్ళు out2;

	retval = io_channel_write_blk64(fs->io, zblk, 1, buf);
	ఒకవేళ (retval)
		వెళ్ళు out2;

	జ్ఞాపకం_నకలు(fs->super->s_jnl_ఖండాలు, inode.i_ఖండం, EXT2_N_ఖండాలు*4);
	fs->super->s_jnl_ఖండాలు[15] = inode.i_కొలత_ఎగువ;
	fs->super->s_jnl_ఖండాలు[16] = inode.i_కొలత;
	fs->super->s_jnl_backup_type = EXT3_JNL_BACKUP_ఖండాలు;
	ext2fs_mark_super_dirty(fs);

out2:
	ext2fs_free_mem(&buf);
	తిరిగి retval;
}

/*
 * Find a reasonable journal file size (in blocks) given the number of blocks
 * in the filesystem.  For very small filesystems, it is not reasonable to
 * have a journal that fills more than half of the filesystem.
 *
 * n.b. comments assume 4k blocks
 */
పూర్ణాంకం ext2fs_default_journal_కొలత(__u64 అంకె_ఖండాలు)
{
	ఒకవేళ (అంకె_ఖండాలు < 2048)
		తిరిగి -1;
	ఒకవేళ (అంకె_ఖండాలు < 32768)		/* 128 MB */
		తిరిగి (1024);			/* 4 MB */
	ఒకవేళ (అంకె_ఖండాలు < 256*1024)	/* 1 GB */
		తిరిగి (4096);			/* 16 MB */
	ఒకవేళ (అంకె_ఖండాలు < 512*1024)	/* 2 GB */
		తిరిగి (8192);			/* 32 MB */
	ఒకవేళ (అంకె_ఖండాలు < 4096*1024)	/* 16 GB */
		తిరిగి (16384);			/* 64 MB */
	ఒకవేళ (అంకె_ఖండాలు < 8192*1024)	/* 32 GB */
		తిరిగి (32768);			/* 128 MB */
	ఒకవేళ (అంకె_ఖండాలు < 16384*1024)	/* 64 GB */
		తిరిగి (65536);			/* 256 MB */
	ఒకవేళ (అంకె_ఖండాలు < 32768*1024)	/* 128 GB */
		తిరిగి (131072);		/* 512 MB */
	తిరిగి 262144;				/* 1 GB */
}

errcode_t ext2fs_get_journal_params(నిర్మాణం ext2fs_journal_params *params,
		ext2_filsys fs)
{
	blk_t మొత్తం_blks;
	పూర్ణాంకం ret;

	memset(params, 0, పరిమాణం(*params));
	ఒకవేళ (ext2fs_has_feature_journal_dev(fs->super)) {
		మొత్తం_blks = ext2fs_ఖండాలు_లెక్క(fs->super);
		ఒకవేళ (మొత్తం_blks < JBD2_MIN_JOURNAL_ఖండాలు)
			తిరిగి EXT2_ET_JOURNAL_TOO_SMALL;

		ఒకవేళ (!ext2fs_has_feature_fast_commit(fs->super)) {
			params->అంకె_journal_ఖండాలు = మొత్తం_blks;
			params->అంకె_fc_ఖండాలు = 0;
			తిరిగి 0;
		}
		params->అంకె_journal_ఖండాలు = ext2fs_ఖండాలు_లెక్క(fs->super) *
				EXT2_JOURNAL_గమ్యం_FC_BLKS_నిష్పత్తి /
				(EXT2_JOURNAL_గమ్యం_FC_BLKS_నిష్పత్తి + 1);
		ఒకవేళ (JBD2_MIN_JOURNAL_ఖండాలు > params->అంకె_journal_ఖండాలు)
			params->అంకె_journal_ఖండాలు = JBD2_MIN_JOURNAL_ఖండాలు;
		params->అంకె_fc_ఖండాలు = మొత్తం_blks - params->అంకె_journal_ఖండాలు;
		తిరిగి 0;
	}

	ret = ext2fs_default_journal_కొలత(ext2fs_ఖండాలు_లెక్క(fs->super));
	ఒకవేళ (ret < 0)
		తిరిగి EXT2_ET_JOURNAL_TOO_SMALL;

	params->అంకె_journal_ఖండాలు = ret;
	ఒకవేళ (ext2fs_has_feature_fast_commit(fs->super))
		params->అంకె_fc_ఖండాలు = params->అంకె_journal_ఖండాలు /
			EXT2_JOURNAL_గమ్యం_FC_BLKS_నిష్పత్తి;
	తిరిగి 0;
}

పూర్ణాంకం ext2fs_journal_sb_ప్రారంభం(పూర్ణాంకం blocksize)
{
	ఒకవేళ (blocksize == EXT2_MIN_ఖండం_కొలత)
		తిరిగి 2;
	తిరిగి 1;
}

/*
 * This function adds a journal device to a filesystem
 */
errcode_t ext2fs_కూడు_journal_device(ext2_filsys fs, ext2_filsys journal_dev)
{
	నిర్మాణం stat	st;
	errcode_t	retval;
	అక్షరం		buf[SUPERBLOCK_కొలత];
	journal_superblock_t	*jsb;
	పూర్ణాంకం		ప్రారంభం;
	__u32		i, nr_users;

	/* Make sure the device exists and is a block device */
	ఒకవేళ (stat(journal_dev->device_name, &st) < 0)
		తిరిగి errno;

	ఒకవేళ (!S_ISBLK(st.st_బహుళకం))
		తిరిగి EXT2_ET_JOURNAL_NOT_ఖండం; /* Must be a block device */

	/* Get the journal superblock */
	ప్రారంభం = ext2fs_journal_sb_ప్రారంభం(journal_dev->blocksize);
	ఒకవేళ ((retval = io_channel_read_blk64(journal_dev->io, ప్రారంభం,
					    -SUPERBLOCK_కొలత,
					    buf)))
		తిరిగి retval;

	jsb = (journal_superblock_t *) buf;
	ఒకవేళ ((jsb->s_శీర్షిక.h_magic != (గుర్తులేని) ntohl(JBD2_MAGIC_సంఖ్య)) ||
	    (jsb->s_శీర్షిక.h_blocktype != (గుర్తులేని) ntohl(JBD2_SUPERBLOCK_V2)))
		తిరిగి EXT2_ET_NO_JOURNAL_SB;

	ఒకవేళ (ntohl(jsb->s_blocksize) != (గుర్తులేని దీర్ఘ) fs->blocksize)
		తిరిగి EXT2_ET_UNEXPECTED_ఖండం_కొలత;

	/* Check and see if this filesystem has already been added */
	nr_users = ntohl(jsb->s_nr_users);
	ఒకవేళ (nr_users > JBD2_USERS_MAX)
		తిరిగి EXT2_ET_CORRUPT_JOURNAL_SB;
	కొరకు (i=0; i < nr_users; i++) {
		ఒకవేళ (memcmp(fs->super->s_uuid,
			   &jsb->s_users[i*16], 16) == 0)
			ఆపు;
	}
	ఒకవేళ (i >= nr_users) {
		జ్ఞాపకం_నకలు(&jsb->s_users[nr_users*16],
		       fs->super->s_uuid, 16);
		jsb->s_nr_users = htonl(nr_users+1);
	}

	/* Writeback the journal superblock */
	ఒకవేళ ((retval = io_channel_write_blk64(journal_dev->io, ప్రారంభం,
					    -SUPERBLOCK_కొలత, buf)))
		తిరిగి retval;

	fs->super->s_journal_inum = 0;
	fs->super->s_journal_dev = st.st_rdev;
	జ్ఞాపకం_నకలు(fs->super->s_journal_uuid, jsb->s_uuid,
	       పరిమాణం(fs->super->s_journal_uuid));
	memset(fs->super->s_jnl_ఖండాలు, 0, పరిమాణం(fs->super->s_jnl_ఖండాలు));
	ext2fs_సమితి_feature_journal(fs->super);
	ext2fs_mark_super_dirty(fs);
	తిరిగి 0;
}

/*
 * This function adds a journal inode to a filesystem, using either
 * POSIX routines if the filesystem is mounted, or using direct I/O
 * functions if it is not.
 */
errcode_t ext2fs_కూడు_journal_inode3(ext2_filsys fs, నిర్మాణం ext2fs_journal_params *jparams,
				    blk64_t goal, పూర్ణాంకం flags)
{
	errcode_t		retval;
	ext2_ino_t		journal_ino;
	నిర్మాణం stat		st;
	అక్షరం			jfile[1024];
	పూర్ణాంకం			mount_flags;
	పూర్ణాంకం			fd = -1;

	ఒకవేళ (flags & EXT2_MKJOURNAL_NO_MNT_CHECK)
		mount_flags = 0;
	లేకపోతే ఒకవేళ ((retval = ext2fs_check_mount_బిందువు(fs->device_name,
						    &mount_flags,
						    jfile, పరిమాణం(jfile)-10)))
		తిరిగి retval;

	ఒకవేళ (mount_flags & EXT2_MF_MOUNTED) {
#ఒకవేళ HAVE_EXT2_IOCTLS
		పూర్ణాంకం f = 0;
#అయితే_ముగింపు
		strcat(jfile, "/.journal");

		/*
		 * If .../.journal already exists, make sure any
		 * immutable or append-only flags are cleared.
		 */
#ఒకవేళ defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		(శూన్య) chflags (jfile, 0);
#లేకపోతే
#ఒకవేళ HAVE_EXT2_IOCTLS
		fd = open(jfile, O_RDONLY);
		ఒకవేళ (fd >= 0) {
			retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
			close(fd);
			ఒకవేళ (retval)
				తిరిగి errno;
		}
#అయితే_ముగింపు
#అయితే_ముగింపు

		/* Create the journal file */
		ఒకవేళ ((fd = open(jfile, O_CREAT|O_WRONLY, 0600)) < 0)
			తిరిగి errno;

		/* Note that we can't do lazy journal initialization for mounted
		 * filesystems, since the zero writing is also allocating the
		 * journal blocks.  We could use fallocate, but not all kernels
		 * support that, and creating a journal on a mounted ext2
		 * filesystems is extremely rare these days...  Ignore it. */
		flags &= ~EXT2_MKJOURNAL_LAZYINIT;

		ఒకవేళ ((retval = write_journal_file(fs, jfile, jparams, flags)))
			వెళ్ళు errout;

		/* Get inode number of the journal file */
		ఒకవేళ (fstat(fd, &st) < 0) {
			retval = errno;
			వెళ్ళు errout;
		}

#ఒకవేళ defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		retval = fchflags (fd, UF_NODUMP|UF_IMMUTABLE);
#లేకపోతే
#ఒకవేళ HAVE_EXT2_IOCTLS
		ఒకవేళ (ioctl(fd, EXT2_IOC_GETFLAGS, &f) < 0) {
			retval = errno;
			వెళ్ళు errout;
		}
		f |= EXT2_NODUMP_FL | EXT2_IMMUTABLE_FL;
		retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
#అయితే_ముగింపు
#అయితే_ముగింపు
		ఒకవేళ (retval) {
			retval = errno;
			వెళ్ళు errout;
		}

		ఒకవేళ (close(fd) < 0) {
			retval = errno;
			fd = -1;
			వెళ్ళు errout;
		}
		journal_ino = st.st_ino;
		memset(fs->super->s_jnl_ఖండాలు, 0,
		       పరిమాణం(fs->super->s_jnl_ఖండాలు));
	} లేకపోతే {
		ఒకవేళ ((mount_flags & EXT2_MF_BUSY) &&
		    !(fs->flags & EXT2_FLAG_EXCLUSIVE)) {
			retval = EBUSY;
			వెళ్ళు errout;
		}
		journal_ino = EXT2_JOURNAL_INO;
		ఒకవేళ ((retval = write_journal_inode(fs, journal_ino,
						  jparams, goal, flags)))
			తిరిగి retval;
	}

	fs->super->s_journal_inum = journal_ino;
	fs->super->s_journal_dev = 0;
	memset(fs->super->s_journal_uuid, 0,
	       పరిమాణం(fs->super->s_journal_uuid));
	ext2fs_సమితి_feature_journal(fs->super);

	ext2fs_mark_super_dirty(fs);
	తిరిగి 0;
errout:
	ఒకవేళ (fd >= 0)
		close(fd);
	తిరిగి retval;
}

errcode_t ext2fs_కూడు_journal_inode2(ext2_filsys fs, blk_t అంకె_ఖండాలు,
				    blk64_t goal, పూర్ణాంకం flags)
{
	నిర్మాణం ext2fs_journal_params jparams;

	jparams.అంకె_journal_ఖండాలు = అంకె_ఖండాలు;
	jparams.అంకె_fc_ఖండాలు = 0;

	తిరిగి ext2fs_కూడు_journal_inode3(fs, &jparams, goal, flags);
}

errcode_t ext2fs_కూడు_journal_inode(ext2_filsys fs, blk_t అంకె_ఖండాలు, పూర్ణాంకం flags)
{
	తిరిగి ext2fs_కూడు_journal_inode2(fs, అంకె_ఖండాలు, ~0ULL, flags);
}


#నిర్వచితమైతే DEBUG
ప్రధాన(పూర్ణాంకం argc, అక్షరం **argv)
{
	errcode_t	retval;
	అక్షరం		*device_name;
	ext2_filsys	fs;

	ఒకవేళ (argc < 2) {
		fprintf(stderr, "Usage: %s filesystem\n", argv[0]);
		నిష్క్రమించు(1);
	}
	device_name = argv[1];

	retval = ext2fs_open (device_name, EXT2_FLAG_RW, 0, 0,
			      unix_io_manager, &fs);
	ఒకవేళ (retval) {
		com_err(argv[0], retval, "while opening %s", device_name);
		నిష్క్రమించు(1);
	}

	retval = ext2fs_కూడు_journal_inode(fs, JBD2_MIN_JOURNAL_ఖండాలు, 0);
	ఒకవేళ (retval) {
		com_err(argv[0], retval, "while adding journal to %s",
			device_name);
		నిష్క్రమించు(1);
	}
	retval = ext2fs_flush(fs);
	ఒకవేళ (retval) {
		రాయి("Warning, had trouble writing out superblocks.\n");
	}
	ext2fs_close_free(&fs);
	నిష్క్రమించు(0);

}
#అయితే_ముగింపు
