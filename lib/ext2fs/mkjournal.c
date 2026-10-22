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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#شامل <assert.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#اگر_ختم
#شامل <fcntl.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم
#اگر HAVE_SYS_IOCTL_H
#شامل <sys/ioctl.h>
#اگر_ختم
#اگر HAVE_NETINET_اندر_H
#شامل <netinet/in.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "e2p/e2p.h"
#شامل "ext2fsP.h"

#شامل "kernel-jbd.h"

/*
 * This function automatically sets up the journal superblock and
 * returns it as an allocated block.
 */
errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
					   ساخت ext2fs_journal_params *jparams,
					   صحیح flags, حرف **ret_jsb)
{
	errcode_t		retval;
	journal_superblock_t	*jsb;

	اگر (jparams->عدد_journal_بلاکیں < JBD2_MIN_JOURNAL_بلاکیں)
		واپس EXT2_ET_JOURNAL_TOO_SMALL;

	اگر ((retval = ext2fs_get_mem(fs->blocksize, &jsb)))
		واپس retval;

	memset (jsb, 0, fs->blocksize);

	jsb->s_سرخی.h_magic = htonl(JBD2_MAGIC_نمبر);
	اگر (flags & EXT2_MKJOURNAL_V1_SUPER)
		jsb->s_سرخی.h_blocktype = htonl(JBD2_SUPERBLOCK_V1);
	ورنہ
		jsb->s_سرخی.h_blocktype = htonl(JBD2_SUPERBLOCK_V2);
	jsb->s_blocksize = htonl(fs->blocksize);
	jsb->s_maxlen = htonl(jparams->عدد_journal_بلاکیں + jparams->عدد_fc_بلاکیں);
	jsb->s_nr_users = htonl(1);
	jsb->s_پہلا = htonl(1);
	jsb->s_sequence = htonl(1);
	jsb->s_عدد_fc_blks = htonl(jparams->عدد_fc_بلاکیں);
	حافظہ_نقل(jsb->s_uuid, fs->super->s_uuid, حجم(fs->super->s_uuid));
	/*
	 * If we're creating an external journal device, we need to
	 * adjust these fields.
	 */
	اگر (ext2fs_has_feature_journal_dev(fs->super)) {
		jsb->s_nr_users = 0;
		jsb->s_پہلا = htonl(ext2fs_journal_sb_آغاز(fs->blocksize) + 1);
	}

	*ret_jsb = (حرف *) jsb;
	واپس 0;
}

errcode_t ext2fs_create_journal_superblock(ext2_filsys fs, __u32 عدد_بلاکیں,
					صحیح flags, حرف **ret_sb)
{
	ساخت ext2fs_journal_params jparams;

	jparams.عدد_journal_بلاکیں = عدد_بلاکیں;
	jparams.عدد_fc_بلاکیں = 0;

	واپس ext2fs_create_journal_superblock2(fs, &jparams, flags, ret_sb);
}

/*
 * This function writes a journal using POSIX routines.  It is used
 * for creating external journals and creating journals on live
 * filesystems.
 */
ساکن errcode_t write_journal_file(ext2_filsys fs, حرف *filename,
				    ساخت ext2fs_journal_params *jparams,
				    صحیح flags)
{
	errcode_t	retval;
	حرف		*buf = 0;
	صحیح		fd, ret_سائز;
	blk_t		i;

	اگر ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		واپس retval;

	/* Open the device or journal file */
	اگر ((fd = open(filename, O_WRONLY)) < 0) {
		retval = errno;
		جاؤ errfree;
	}

	/* Write the superblock out */
	retval = EXT2_ET_SHORT_WRITE;
	ret_سائز = اكتب(fd, buf, fs->blocksize);
	اگر (ret_سائز < 0) {
		retval = errno;
		جاؤ errout;
	}
	اگر (ret_سائز != (صحیح) fs->blocksize)
		جاؤ errout;
	memset(buf, 0, fs->blocksize);

	اگر (flags & EXT2_MKJOURNAL_LAZYINIT)
		جاؤ success;

	برائے (i = 1; i < jparams->عدد_journal_بلاکیں + jparams->عدد_fc_بلاکیں; i++) {
		ret_سائز = اكتب(fd, buf, fs->blocksize);
		اگر (ret_سائز < 0) {
			retval = errno;
			جاؤ errout;
		}
		اگر (ret_سائز != (صحیح) fs->blocksize)
			جاؤ errout;
	}

success:
	retval = 0;
errout:
	close(fd);
errfree:
	ext2fs_free_mem(&buf);
	واپس retval;
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
#تعریف_کرو MAX_STRIDE_لمبائی (4194304 / (صحیح) fs->blocksize)
errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, صحیح عدد,
			      blk64_t *ret_blk, صحیح *ret_گنتی)
{
	صحیح		j, گنتی;
	ساکن خالی	*buf;
	ساکن صحیح	stride_لمبائی;
	errcode_t	retval;

	/* If fs is null, clean up the static buffer and return */
	اگر (!fs) {
		اگر (buf) {
			آزاد(buf);
			buf = 0;
			stride_لمبائی = 0;
		}
		واپس 0;
	}

	/* Deal with zeroing less than 1 block */
	اگر (عدد <= 0)
		واپس 0;

	/* Try a zero out command, if supported */
	retval = io_channel_zeroout(fs->io, blk, عدد);
	اگر (retval == 0)
		واپس 0;

	/* Allocate the zeroizing buffer if necessary */
	اگر (عدد > stride_لمبائی && stride_لمبائی < MAX_STRIDE_لمبائی) {
		خالی *p;
		صحیح new_stride = عدد;

		اگر (new_stride > MAX_STRIDE_لمبائی)
			new_stride = MAX_STRIDE_لمبائی;
		p = realloc(buf, fs->blocksize * new_stride);
		اگر (!p)
			واپس EXT2_ET_NO_حافظہ;
		buf = p;
		stride_لمبائی = new_stride;
		memset(buf, 0, fs->blocksize * stride_لمبائی);
	}
	/* OK, do the write loop */
	j=0;
	جب (j < عدد) {
		اگر (blk % stride_لمبائی) {
			گنتی = stride_لمبائی - (blk % stride_لمبائی);
			اگر (گنتی > (عدد - j))
				گنتی = عدد - j;
		} ورنہ {
			گنتی = عدد - j;
			اگر (گنتی > stride_لمبائی)
				گنتی = stride_لمبائی;
		}
		retval = io_channel_write_blk64(fs->io, blk, گنتی, buf);
		اگر (retval) {
			اگر (ret_گنتی)
				*ret_گنتی = گنتی;
			اگر (ret_blk)
				*ret_blk = blk;
			واپس retval;
		}
		j += گنتی; blk += گنتی;
	}
	واپس 0;
}

errcode_t ext2fs_zero_بلاکیں(ext2_filsys fs, blk_t blk, صحیح عدد,
			     blk_t *ret_blk, صحیح *ret_گنتی)
{
	blk64_t ret_blk2;
	errcode_t retval;

	retval = ext2fs_zero_blocks2(fs, blk, عدد, &ret_blk2, ret_گنتی);
	اگر (retval)
		*ret_blk = (blk_t) ret_blk2;
	واپس retval;
}

/*
 * Calculate the initial goal block to be roughly at the middle of the
 * filesystem.  Pick a group that has the largest number of free
 * blocks.
 */
ساکن blk64_t get_midpoint_journal_بلاک(ext2_filsys fs)
{
	dgrp_t	گروہ, آغاز, انتہا, i, log_flex;

	گروہ = ext2fs_گروہ_کا_blk2(fs, (ext2fs_بلاکیں_گنتی(fs->super) -
					 fs->super->s_پہلا_ڈیٹا_بلاک) / 2);
	log_flex = 1U << fs->super->s_log_groups_فی_flex;
	اگر (fs->super->s_log_groups_فی_flex && (گروہ > log_flex)) {
		گروہ = گروہ & ~(log_flex - 1);
		جب ((گروہ < fs->گروہ_desc_گنتی) &&
		       ext2fs_bg_free_بلاکیں_گنتی(fs, گروہ) == 0)
			گروہ++;
		اگر (گروہ == fs->گروہ_desc_گنتی)
			گروہ = 0;
		آغاز = گروہ;
	} ورنہ
		آغاز = (گروہ > 0) ? گروہ-1 : گروہ;
	انتہا = ((گروہ+1) < fs->گروہ_desc_گنتی) ? گروہ+1 : گروہ;
	گروہ = آغاز;
	برائے (i = آغاز + 1; i <= انتہا; i++)
		اگر (ext2fs_bg_free_بلاکیں_گنتی(fs, i) >
		    ext2fs_bg_free_بلاکیں_گنتی(fs, گروہ))
			گروہ = i;
	واپس ext2fs_گروہ_پہلا_block2(fs, گروہ);
}

/*
 * This function creates a journal using direct I/O routines.
 */
ساکن errcode_t write_journal_inode(ext2_filsys fs, ext2_ino_t journal_ino,
				     ساخت ext2fs_journal_params *jparams,
				     blk64_t goal, صحیح flags)
{
	حرف			*buf;
	errcode_t		retval;
	ساخت ext2_inode	inode;
	غیرعلامتی طویل طویل	inode_سائز;
	صحیح			falloc_flags = EXT2_FALLOCATE_FORCE_INIT;
	blk64_t			zblk;
	time_t			now;

	اگر ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		واپس retval;

	اگر ((retval = ext2fs_read_bitmaps(fs)))
		جاؤ out2;

	اگر ((retval = ext2fs_read_inode(fs, journal_ino, &inode)))
		جاؤ out2;

	اگر (inode.i_بلاکیں > 0) {
		retval = EEXIST;
		جاؤ out2;
	}

	اگر (goal == ~0ULL)
		goal = get_midpoint_journal_بلاک(fs);

	اگر (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;

	اگر (!(flags & EXT2_MKJOURNAL_LAZYINIT))
		falloc_flags |= EXT2_FALLOCATE_ZERO_بلاکیں;

	inode_سائز = (غیرعلامتی طویل طویل)fs->blocksize *
			(jparams->عدد_journal_بلاکیں + jparams->عدد_fc_بلاکیں);
	now = ext2fs_P_get_وقت(fs);
	ext2fs_inode_xtime_سیٹ(&inode, i_mtime, now);
	ext2fs_inode_xtime_سیٹ(&inode, i_ctime, now);
	inode.i_links_گنتی = 1;
	inode.i_منوال = LINUX_S_IFREG | 0600;
	retval = ext2fs_inode_سائز_سیٹ(fs, &inode, inode_سائز);
	اگر (retval)
		جاؤ out2;

	retval = ext2fs_fallocate(fs, falloc_flags, journal_ino,
				  &inode, goal, 0,
				  jparams->عدد_journal_بلاکیں + jparams->عدد_fc_بلاکیں);
	اگر (retval)
		جاؤ out2;

	اگر ((retval = ext2fs_write_new_inode(fs, journal_ino, &inode)))
		جاؤ out2;

	retval = ext2fs_bmap2(fs, journal_ino, &inode, تہی, 0, 0, تہی, &zblk);
	اگر (retval)
		جاؤ out2;

	retval = io_channel_write_blk64(fs->io, zblk, 1, buf);
	اگر (retval)
		جاؤ out2;

	حافظہ_نقل(fs->super->s_jnl_بلاکیں, inode.i_بلاک, EXT2_N_بلاکیں*4);
	fs->super->s_jnl_بلاکیں[15] = inode.i_سائز_بلند;
	fs->super->s_jnl_بلاکیں[16] = inode.i_سائز;
	fs->super->s_jnl_backup_type = EXT3_JNL_BACKUP_بلاکیں;
	ext2fs_mark_super_dirty(fs);

out2:
	ext2fs_free_mem(&buf);
	واپس retval;
}

/*
 * Find a reasonable journal file size (in blocks) given the number of blocks
 * in the filesystem.  For very small filesystems, it is not reasonable to
 * have a journal that fills more than half of the filesystem.
 *
 * n.b. comments assume 4k blocks
 */
صحیح ext2fs_default_journal_سائز(__u64 عدد_بلاکیں)
{
	اگر (عدد_بلاکیں < 2048)
		واپس -1;
	اگر (عدد_بلاکیں < 32768)		/* 128 MB */
		واپس (1024);			/* 4 MB */
	اگر (عدد_بلاکیں < 256*1024)	/* 1 GB */
		واپس (4096);			/* 16 MB */
	اگر (عدد_بلاکیں < 512*1024)	/* 2 GB */
		واپس (8192);			/* 32 MB */
	اگر (عدد_بلاکیں < 4096*1024)	/* 16 GB */
		واپس (16384);			/* 64 MB */
	اگر (عدد_بلاکیں < 8192*1024)	/* 32 GB */
		واپس (32768);			/* 128 MB */
	اگر (عدد_بلاکیں < 16384*1024)	/* 64 GB */
		واپس (65536);			/* 256 MB */
	اگر (عدد_بلاکیں < 32768*1024)	/* 128 GB */
		واپس (131072);		/* 512 MB */
	واپس 262144;				/* 1 GB */
}

errcode_t ext2fs_get_journal_params(ساخت ext2fs_journal_params *params,
		ext2_filsys fs)
{
	blk_t کل_blks;
	صحیح ret;

	memset(params, 0, حجم(*params));
	اگر (ext2fs_has_feature_journal_dev(fs->super)) {
		کل_blks = ext2fs_بلاکیں_گنتی(fs->super);
		اگر (کل_blks < JBD2_MIN_JOURNAL_بلاکیں)
			واپس EXT2_ET_JOURNAL_TOO_SMALL;

		اگر (!ext2fs_has_feature_fast_commit(fs->super)) {
			params->عدد_journal_بلاکیں = کل_blks;
			params->عدد_fc_بلاکیں = 0;
			واپس 0;
		}
		params->عدد_journal_بلاکیں = ext2fs_بلاکیں_گنتی(fs->super) *
				EXT2_JOURNAL_مقصد_FC_BLKS_تناسب /
				(EXT2_JOURNAL_مقصد_FC_BLKS_تناسب + 1);
		اگر (JBD2_MIN_JOURNAL_بلاکیں > params->عدد_journal_بلاکیں)
			params->عدد_journal_بلاکیں = JBD2_MIN_JOURNAL_بلاکیں;
		params->عدد_fc_بلاکیں = کل_blks - params->عدد_journal_بلاکیں;
		واپس 0;
	}

	ret = ext2fs_default_journal_سائز(ext2fs_بلاکیں_گنتی(fs->super));
	اگر (ret < 0)
		واپس EXT2_ET_JOURNAL_TOO_SMALL;

	params->عدد_journal_بلاکیں = ret;
	اگر (ext2fs_has_feature_fast_commit(fs->super))
		params->عدد_fc_بلاکیں = params->عدد_journal_بلاکیں /
			EXT2_JOURNAL_مقصد_FC_BLKS_تناسب;
	واپس 0;
}

صحیح ext2fs_journal_sb_آغاز(صحیح blocksize)
{
	اگر (blocksize == EXT2_MIN_بلاک_سائز)
		واپس 2;
	واپس 1;
}

/*
 * This function adds a journal device to a filesystem
 */
errcode_t ext2fs_جوڑو_journal_device(ext2_filsys fs, ext2_filsys journal_dev)
{
	ساخت stat	st;
	errcode_t	retval;
	حرف		buf[SUPERBLOCK_سائز];
	journal_superblock_t	*jsb;
	صحیح		آغاز;
	__u32		i, nr_users;

	/* Make sure the device exists and is a block device */
	اگر (stat(journal_dev->device_name, &st) < 0)
		واپس errno;

	اگر (!S_ISBLK(st.st_منوال))
		واپس EXT2_ET_JOURNAL_NOT_بلاک; /* Must be a block device */

	/* Get the journal superblock */
	آغاز = ext2fs_journal_sb_آغاز(journal_dev->blocksize);
	اگر ((retval = io_channel_read_blk64(journal_dev->io, آغاز,
					    -SUPERBLOCK_سائز,
					    buf)))
		واپس retval;

	jsb = (journal_superblock_t *) buf;
	اگر ((jsb->s_سرخی.h_magic != (غیرعلامتی) ntohl(JBD2_MAGIC_نمبر)) ||
	    (jsb->s_سرخی.h_blocktype != (غیرعلامتی) ntohl(JBD2_SUPERBLOCK_V2)))
		واپس EXT2_ET_NO_JOURNAL_SB;

	اگر (ntohl(jsb->s_blocksize) != (غیرعلامتی طویل) fs->blocksize)
		واپس EXT2_ET_UNEXPECTED_بلاک_سائز;

	/* Check and see if this filesystem has already been added */
	nr_users = ntohl(jsb->s_nr_users);
	اگر (nr_users > JBD2_USERS_MAX)
		واپس EXT2_ET_CORRUPT_JOURNAL_SB;
	برائے (i=0; i < nr_users; i++) {
		اگر (memcmp(fs->super->s_uuid,
			   &jsb->s_users[i*16], 16) == 0)
			توڑو;
	}
	اگر (i >= nr_users) {
		حافظہ_نقل(&jsb->s_users[nr_users*16],
		       fs->super->s_uuid, 16);
		jsb->s_nr_users = htonl(nr_users+1);
	}

	/* Writeback the journal superblock */
	اگر ((retval = io_channel_write_blk64(journal_dev->io, آغاز,
					    -SUPERBLOCK_سائز, buf)))
		واپس retval;

	fs->super->s_journal_inum = 0;
	fs->super->s_journal_dev = st.st_rdev;
	حافظہ_نقل(fs->super->s_journal_uuid, jsb->s_uuid,
	       حجم(fs->super->s_journal_uuid));
	memset(fs->super->s_jnl_بلاکیں, 0, حجم(fs->super->s_jnl_بلاکیں));
	ext2fs_سیٹ_feature_journal(fs->super);
	ext2fs_mark_super_dirty(fs);
	واپس 0;
}

/*
 * This function adds a journal inode to a filesystem, using either
 * POSIX routines if the filesystem is mounted, or using direct I/O
 * functions if it is not.
 */
errcode_t ext2fs_جوڑو_journal_inode3(ext2_filsys fs, ساخت ext2fs_journal_params *jparams,
				    blk64_t goal, صحیح flags)
{
	errcode_t		retval;
	ext2_ino_t		journal_ino;
	ساخت stat		st;
	حرف			jfile[1024];
	صحیح			mount_flags;
	صحیح			fd = -1;

	اگر (flags & EXT2_MKJOURNAL_NO_MNT_CHECK)
		mount_flags = 0;
	ورنہ اگر ((retval = ext2fs_check_mount_نقطہ(fs->device_name,
						    &mount_flags,
						    jfile, حجم(jfile)-10)))
		واپس retval;

	اگر (mount_flags & EXT2_MF_MOUNTED) {
#اگر HAVE_EXT2_IOCTLS
		صحیح f = 0;
#اگر_ختم
		strcat(jfile, "/.journal");

		/*
		 * If .../.journal already exists, make sure any
		 * immutable or append-only flags are cleared.
		 */
#اگر defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		(خالی) chflags (jfile, 0);
#ورنہ
#اگر HAVE_EXT2_IOCTLS
		fd = open(jfile, O_RDONLY);
		اگر (fd >= 0) {
			retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
			close(fd);
			اگر (retval)
				واپس errno;
		}
#اگر_ختم
#اگر_ختم

		/* Create the journal file */
		اگر ((fd = open(jfile, O_CREAT|O_WRONLY, 0600)) < 0)
			واپس errno;

		/* Note that we can't do lazy journal initialization for mounted
		 * filesystems, since the zero writing is also allocating the
		 * journal blocks.  We could use fallocate, but not all kernels
		 * support that, and creating a journal on a mounted ext2
		 * filesystems is extremely rare these days...  Ignore it. */
		flags &= ~EXT2_MKJOURNAL_LAZYINIT;

		اگر ((retval = write_journal_file(fs, jfile, jparams, flags)))
			جاؤ errout;

		/* Get inode number of the journal file */
		اگر (fstat(fd, &st) < 0) {
			retval = errno;
			جاؤ errout;
		}

#اگر defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		retval = fchflags (fd, UF_NODUMP|UF_IMMUTABLE);
#ورنہ
#اگر HAVE_EXT2_IOCTLS
		اگر (ioctl(fd, EXT2_IOC_GETFLAGS, &f) < 0) {
			retval = errno;
			جاؤ errout;
		}
		f |= EXT2_NODUMP_FL | EXT2_IMMUTABLE_FL;
		retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
#اگر_ختم
#اگر_ختم
		اگر (retval) {
			retval = errno;
			جاؤ errout;
		}

		اگر (close(fd) < 0) {
			retval = errno;
			fd = -1;
			جاؤ errout;
		}
		journal_ino = st.st_ino;
		memset(fs->super->s_jnl_بلاکیں, 0,
		       حجم(fs->super->s_jnl_بلاکیں));
	} ورنہ {
		اگر ((mount_flags & EXT2_MF_BUSY) &&
		    !(fs->flags & EXT2_FLAG_EXCLUSIVE)) {
			retval = EBUSY;
			جاؤ errout;
		}
		journal_ino = EXT2_JOURNAL_INO;
		اگر ((retval = write_journal_inode(fs, journal_ino,
						  jparams, goal, flags)))
			واپس retval;
	}

	fs->super->s_journal_inum = journal_ino;
	fs->super->s_journal_dev = 0;
	memset(fs->super->s_journal_uuid, 0,
	       حجم(fs->super->s_journal_uuid));
	ext2fs_سیٹ_feature_journal(fs->super);

	ext2fs_mark_super_dirty(fs);
	واپس 0;
errout:
	اگر (fd >= 0)
		close(fd);
	واپس retval;
}

errcode_t ext2fs_جوڑو_journal_inode2(ext2_filsys fs, blk_t عدد_بلاکیں,
				    blk64_t goal, صحیح flags)
{
	ساخت ext2fs_journal_params jparams;

	jparams.عدد_journal_بلاکیں = عدد_بلاکیں;
	jparams.عدد_fc_بلاکیں = 0;

	واپس ext2fs_جوڑو_journal_inode3(fs, &jparams, goal, flags);
}

errcode_t ext2fs_جوڑو_journal_inode(ext2_filsys fs, blk_t عدد_بلاکیں, صحیح flags)
{
	واپس ext2fs_جوڑو_journal_inode2(fs, عدد_بلاکیں, ~0ULL, flags);
}


#اگر_تعریف_شدہ DEBUG
مین(صحیح argc, حرف **argv)
{
	errcode_t	retval;
	حرف		*device_name;
	ext2_filsys	fs;

	اگر (argc < 2) {
		fprintf(stderr, "Usage: %s filesystem\n", argv[0]);
		باہر_نکلو(1);
	}
	device_name = argv[1];

	retval = ext2fs_open (device_name, EXT2_FLAG_RW, 0, 0,
			      unix_io_manager, &fs);
	اگر (retval) {
		com_err(argv[0], retval, "while opening %s", device_name);
		باہر_نکلو(1);
	}

	retval = ext2fs_جوڑو_journal_inode(fs, JBD2_MIN_JOURNAL_بلاکیں, 0);
	اگر (retval) {
		com_err(argv[0], retval, "while adding journal to %s",
			device_name);
		باہر_نکلو(1);
	}
	retval = ext2fs_flush(fs);
	اگر (retval) {
		لکھو("Warning, had trouble writing out superblocks.\n");
	}
	ext2fs_close_free(&fs);
	باہر_نکلو(0);

}
#اگر_ختم
