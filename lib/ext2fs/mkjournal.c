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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#ضمن <assert.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جے_انت
#ضمن <fcntl.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت
#إذا HAVE_SYS_IOCTL_H
#ضمن <sys/ioctl.h>
#جے_انت
#إذا HAVE_NETINET_وچ_H
#ضمن <netinet/in.h>
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "e2p/e2p.h"
#ضمن "ext2fsP.h"

#ضمن "kernel-jbd.h"

/*
 * This function automatically sets up the journal superblock and
 * returns it as an allocated block.
 */
errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
					   بنية ext2fs_journal_params *jparams,
					   صحيح flags, حرف **ret_jsb)
{
	errcode_t		retval;
	journal_superblock_t	*jsb;

	إذا (jparams->عدد_journal_بلاکاں < JBD2_MIN_JOURNAL_بلاکاں)
		أرجع EXT2_ET_JOURNAL_TOO_SMALL;

	إذا ((retval = ext2fs_get_mem(fs->blocksize, &jsb)))
		أرجع retval;

	memset (jsb, 0, fs->blocksize);

	jsb->s_سرخی.h_magic = htonl(JBD2_MAGIC_نمبر);
	إذا (flags & EXT2_MKJOURNAL_V1_SUPER)
		jsb->s_سرخی.h_blocktype = htonl(JBD2_SUPERBLOCK_V1);
	وإلا
		jsb->s_سرخی.h_blocktype = htonl(JBD2_SUPERBLOCK_V2);
	jsb->s_blocksize = htonl(fs->blocksize);
	jsb->s_maxlen = htonl(jparams->عدد_journal_بلاکاں + jparams->عدد_fc_بلاکاں);
	jsb->s_nr_users = htonl(1);
	jsb->s_پہلا = htonl(1);
	jsb->s_sequence = htonl(1);
	jsb->s_عدد_fc_blks = htonl(jparams->عدد_fc_بلاکاں);
	حافظہ_نقل(jsb->s_uuid, fs->super->s_uuid, حجم(fs->super->s_uuid));
	/*
	 * If we're creating an external journal device, we need to
	 * adjust these fields.
	 */
	إذا (ext2fs_has_feature_journal_dev(fs->super)) {
		jsb->s_nr_users = 0;
		jsb->s_پہلا = htonl(ext2fs_journal_sb_شروع(fs->blocksize) + 1);
	}

	*ret_jsb = (حرف *) jsb;
	أرجع 0;
}

errcode_t ext2fs_create_journal_superblock(ext2_filsys fs, __u32 عدد_بلاکاں,
					صحيح flags, حرف **ret_sb)
{
	بنية ext2fs_journal_params jparams;

	jparams.عدد_journal_بلاکاں = عدد_بلاکاں;
	jparams.عدد_fc_بلاکاں = 0;

	أرجع ext2fs_create_journal_superblock2(fs, &jparams, flags, ret_sb);
}

/*
 * This function writes a journal using POSIX routines.  It is used
 * for creating external journals and creating journals on live
 * filesystems.
 */
ساكن errcode_t write_journal_file(ext2_filsys fs, حرف *filename,
				    بنية ext2fs_journal_params *jparams,
				    صحيح flags)
{
	errcode_t	retval;
	حرف		*buf = 0;
	صحيح		fd, ret_ناپ;
	blk_t		i;

	إذا ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		أرجع retval;

	/* Open the device or journal file */
	إذا ((fd = open(filename, O_WRONLY)) < 0) {
		retval = errno;
		اذهب errfree;
	}

	/* Write the superblock out */
	retval = EXT2_ET_SHORT_WRITE;
	ret_ناپ = اكتب(fd, buf, fs->blocksize);
	إذا (ret_ناپ < 0) {
		retval = errno;
		اذهب errout;
	}
	إذا (ret_ناپ != (صحيح) fs->blocksize)
		اذهب errout;
	memset(buf, 0, fs->blocksize);

	إذا (flags & EXT2_MKJOURNAL_LAZYINIT)
		اذهب success;

	لكل (i = 1; i < jparams->عدد_journal_بلاکاں + jparams->عدد_fc_بلاکاں; i++) {
		ret_ناپ = اكتب(fd, buf, fs->blocksize);
		إذا (ret_ناپ < 0) {
			retval = errno;
			اذهب errout;
		}
		إذا (ret_ناپ != (صحيح) fs->blocksize)
			اذهب errout;
	}

success:
	retval = 0;
errout:
	close(fd);
errfree:
	ext2fs_free_mem(&buf);
	أرجع retval;
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
#تعریف_کرو MAX_STRIDE_لمائی (4194304 / (صحيح) fs->blocksize)
errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, صحيح عدد,
			      blk64_t *ret_blk, صحيح *ret_گنتی)
{
	صحيح		j, گنتی;
	ساكن فراغ	*buf;
	ساكن صحيح	stride_لمائی;
	errcode_t	retval;

	/* If fs is null, clean up the static buffer and return */
	إذا (!fs) {
		إذا (buf) {
			حرر(buf);
			buf = 0;
			stride_لمائی = 0;
		}
		أرجع 0;
	}

	/* Deal with zeroing less than 1 block */
	إذا (عدد <= 0)
		أرجع 0;

	/* Try a zero out command, if supported */
	retval = io_channel_zeroout(fs->io, blk, عدد);
	إذا (retval == 0)
		أرجع 0;

	/* Allocate the zeroizing buffer if necessary */
	إذا (عدد > stride_لمائی && stride_لمائی < MAX_STRIDE_لمائی) {
		فراغ *p;
		صحيح new_stride = عدد;

		إذا (new_stride > MAX_STRIDE_لمائی)
			new_stride = MAX_STRIDE_لمائی;
		p = realloc(buf, fs->blocksize * new_stride);
		إذا (!p)
			أرجع EXT2_ET_NO_حافظہ;
		buf = p;
		stride_لمائی = new_stride;
		memset(buf, 0, fs->blocksize * stride_لمائی);
	}
	/* OK, do the write loop */
	j=0;
	طالما (j < عدد) {
		إذا (blk % stride_لمائی) {
			گنتی = stride_لمائی - (blk % stride_لمائی);
			إذا (گنتی > (عدد - j))
				گنتی = عدد - j;
		} وإلا {
			گنتی = عدد - j;
			إذا (گنتی > stride_لمائی)
				گنتی = stride_لمائی;
		}
		retval = io_channel_write_blk64(fs->io, blk, گنتی, buf);
		إذا (retval) {
			إذا (ret_گنتی)
				*ret_گنتی = گنتی;
			إذا (ret_blk)
				*ret_blk = blk;
			أرجع retval;
		}
		j += گنتی; blk += گنتی;
	}
	أرجع 0;
}

errcode_t ext2fs_zero_بلاکاں(ext2_filsys fs, blk_t blk, صحيح عدد,
			     blk_t *ret_blk, صحيح *ret_گنتی)
{
	blk64_t ret_blk2;
	errcode_t retval;

	retval = ext2fs_zero_blocks2(fs, blk, عدد, &ret_blk2, ret_گنتی);
	إذا (retval)
		*ret_blk = (blk_t) ret_blk2;
	أرجع retval;
}

/*
 * Calculate the initial goal block to be roughly at the middle of the
 * filesystem.  Pick a group that has the largest number of free
 * blocks.
 */
ساكن blk64_t get_midpoint_journal_بلاک(ext2_filsys fs)
{
	dgrp_t	گروہ, شروع, سرا, i, log_flex;

	گروہ = ext2fs_گروہ_دا_blk2(fs, (ext2fs_بلاکاں_گنتی(fs->super) -
					 fs->super->s_پہلا_انکڑے_بلاک) / 2);
	log_flex = 1U << fs->super->s_log_groups_فی_flex;
	إذا (fs->super->s_log_groups_فی_flex && (گروہ > log_flex)) {
		گروہ = گروہ & ~(log_flex - 1);
		طالما ((گروہ < fs->گروہ_desc_گنتی) &&
		       ext2fs_bg_free_بلاکاں_گنتی(fs, گروہ) == 0)
			گروہ++;
		إذا (گروہ == fs->گروہ_desc_گنتی)
			گروہ = 0;
		شروع = گروہ;
	} وإلا
		شروع = (گروہ > 0) ? گروہ-1 : گروہ;
	سرا = ((گروہ+1) < fs->گروہ_desc_گنتی) ? گروہ+1 : گروہ;
	گروہ = شروع;
	لكل (i = شروع + 1; i <= سرا; i++)
		إذا (ext2fs_bg_free_بلاکاں_گنتی(fs, i) >
		    ext2fs_bg_free_بلاکاں_گنتی(fs, گروہ))
			گروہ = i;
	أرجع ext2fs_گروہ_پہلا_block2(fs, گروہ);
}

/*
 * This function creates a journal using direct I/O routines.
 */
ساكن errcode_t write_journal_inode(ext2_filsys fs, ext2_ino_t journal_ino,
				     بنية ext2fs_journal_params *jparams,
				     blk64_t goal, صحيح flags)
{
	حرف			*buf;
	errcode_t		retval;
	بنية ext2_inode	inode;
	بنانشانی لما لما	inode_ناپ;
	صحيح			falloc_flags = EXT2_FALLOCATE_FORCE_INIT;
	blk64_t			zblk;
	time_t			now;

	إذا ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		أرجع retval;

	إذا ((retval = ext2fs_read_bitmaps(fs)))
		اذهب out2;

	إذا ((retval = ext2fs_read_inode(fs, journal_ino, &inode)))
		اذهب out2;

	إذا (inode.i_بلاکاں > 0) {
		retval = EEXIST;
		اذهب out2;
	}

	إذا (goal == ~0ULL)
		goal = get_midpoint_journal_بلاک(fs);

	إذا (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;

	إذا (!(flags & EXT2_MKJOURNAL_LAZYINIT))
		falloc_flags |= EXT2_FALLOCATE_ZERO_بلاکاں;

	inode_ناپ = (بنانشانی لما لما)fs->blocksize *
			(jparams->عدد_journal_بلاکاں + jparams->عدد_fc_بلاکاں);
	now = ext2fs_P_get_ویلا(fs);
	ext2fs_inode_xtime_سیٹ(&inode, i_mtime, now);
	ext2fs_inode_xtime_سیٹ(&inode, i_ctime, now);
	inode.i_links_گنتی = 1;
	inode.i_منوال = LINUX_S_IFREG | 0600;
	retval = ext2fs_inode_ناپ_سیٹ(fs, &inode, inode_ناپ);
	إذا (retval)
		اذهب out2;

	retval = ext2fs_fallocate(fs, falloc_flags, journal_ino,
				  &inode, goal, 0,
				  jparams->عدد_journal_بلاکاں + jparams->عدد_fc_بلاکاں);
	إذا (retval)
		اذهب out2;

	إذا ((retval = ext2fs_write_new_inode(fs, journal_ino, &inode)))
		اذهب out2;

	retval = ext2fs_bmap2(fs, journal_ino, &inode, عدم, 0, 0, عدم, &zblk);
	إذا (retval)
		اذهب out2;

	retval = io_channel_write_blk64(fs->io, zblk, 1, buf);
	إذا (retval)
		اذهب out2;

	حافظہ_نقل(fs->super->s_jnl_بلاکاں, inode.i_بلاک, EXT2_N_بلاکاں*4);
	fs->super->s_jnl_بلاکاں[15] = inode.i_ناپ_بلند;
	fs->super->s_jnl_بلاکاں[16] = inode.i_ناپ;
	fs->super->s_jnl_backup_type = EXT3_JNL_BACKUP_بلاکاں;
	ext2fs_mark_super_dirty(fs);

out2:
	ext2fs_free_mem(&buf);
	أرجع retval;
}

/*
 * Find a reasonable journal file size (in blocks) given the number of blocks
 * in the filesystem.  For very small filesystems, it is not reasonable to
 * have a journal that fills more than half of the filesystem.
 *
 * n.b. comments assume 4k blocks
 */
صحيح ext2fs_default_journal_ناپ(__u64 عدد_بلاکاں)
{
	إذا (عدد_بلاکاں < 2048)
		أرجع -1;
	إذا (عدد_بلاکاں < 32768)		/* 128 MB */
		أرجع (1024);			/* 4 MB */
	إذا (عدد_بلاکاں < 256*1024)	/* 1 GB */
		أرجع (4096);			/* 16 MB */
	إذا (عدد_بلاکاں < 512*1024)	/* 2 GB */
		أرجع (8192);			/* 32 MB */
	إذا (عدد_بلاکاں < 4096*1024)	/* 16 GB */
		أرجع (16384);			/* 64 MB */
	إذا (عدد_بلاکاں < 8192*1024)	/* 32 GB */
		أرجع (32768);			/* 128 MB */
	إذا (عدد_بلاکاں < 16384*1024)	/* 64 GB */
		أرجع (65536);			/* 256 MB */
	إذا (عدد_بلاکاں < 32768*1024)	/* 128 GB */
		أرجع (131072);		/* 512 MB */
	أرجع 262144;				/* 1 GB */
}

errcode_t ext2fs_get_journal_params(بنية ext2fs_journal_params *params,
		ext2_filsys fs)
{
	blk_t کل_blks;
	صحيح ret;

	memset(params, 0, حجم(*params));
	إذا (ext2fs_has_feature_journal_dev(fs->super)) {
		کل_blks = ext2fs_بلاکاں_گنتی(fs->super);
		إذا (کل_blks < JBD2_MIN_JOURNAL_بلاکاں)
			أرجع EXT2_ET_JOURNAL_TOO_SMALL;

		إذا (!ext2fs_has_feature_fast_commit(fs->super)) {
			params->عدد_journal_بلاکاں = کل_blks;
			params->عدد_fc_بلاکاں = 0;
			أرجع 0;
		}
		params->عدد_journal_بلاکاں = ext2fs_بلاکاں_گنتی(fs->super) *
				EXT2_JOURNAL_منزل_FC_BLKS_تناسب /
				(EXT2_JOURNAL_منزل_FC_BLKS_تناسب + 1);
		إذا (JBD2_MIN_JOURNAL_بلاکاں > params->عدد_journal_بلاکاں)
			params->عدد_journal_بلاکاں = JBD2_MIN_JOURNAL_بلاکاں;
		params->عدد_fc_بلاکاں = کل_blks - params->عدد_journal_بلاکاں;
		أرجع 0;
	}

	ret = ext2fs_default_journal_ناپ(ext2fs_بلاکاں_گنتی(fs->super));
	إذا (ret < 0)
		أرجع EXT2_ET_JOURNAL_TOO_SMALL;

	params->عدد_journal_بلاکاں = ret;
	إذا (ext2fs_has_feature_fast_commit(fs->super))
		params->عدد_fc_بلاکاں = params->عدد_journal_بلاکاں /
			EXT2_JOURNAL_منزل_FC_BLKS_تناسب;
	أرجع 0;
}

صحيح ext2fs_journal_sb_شروع(صحيح blocksize)
{
	إذا (blocksize == EXT2_MIN_بلاک_ناپ)
		أرجع 2;
	أرجع 1;
}

/*
 * This function adds a journal device to a filesystem
 */
errcode_t ext2fs_جوڑو_journal_device(ext2_filsys fs, ext2_filsys journal_dev)
{
	بنية stat	st;
	errcode_t	retval;
	حرف		buf[SUPERBLOCK_ناپ];
	journal_superblock_t	*jsb;
	صحيح		شروع;
	__u32		i, nr_users;

	/* Make sure the device exists and is a block device */
	إذا (stat(journal_dev->device_name, &st) < 0)
		أرجع errno;

	إذا (!S_ISBLK(st.st_منوال))
		أرجع EXT2_ET_JOURNAL_NOT_بلاک; /* Must be a block device */

	/* Get the journal superblock */
	شروع = ext2fs_journal_sb_شروع(journal_dev->blocksize);
	إذا ((retval = io_channel_read_blk64(journal_dev->io, شروع,
					    -SUPERBLOCK_ناپ,
					    buf)))
		أرجع retval;

	jsb = (journal_superblock_t *) buf;
	إذا ((jsb->s_سرخی.h_magic != (بنانشانی) ntohl(JBD2_MAGIC_نمبر)) ||
	    (jsb->s_سرخی.h_blocktype != (بنانشانی) ntohl(JBD2_SUPERBLOCK_V2)))
		أرجع EXT2_ET_NO_JOURNAL_SB;

	إذا (ntohl(jsb->s_blocksize) != (بنانشانی لما) fs->blocksize)
		أرجع EXT2_ET_UNEXPECTED_بلاک_ناپ;

	/* Check and see if this filesystem has already been added */
	nr_users = ntohl(jsb->s_nr_users);
	إذا (nr_users > JBD2_USERS_MAX)
		أرجع EXT2_ET_CORRUPT_JOURNAL_SB;
	لكل (i=0; i < nr_users; i++) {
		إذا (memcmp(fs->super->s_uuid,
			   &jsb->s_users[i*16], 16) == 0)
			اكسر;
	}
	إذا (i >= nr_users) {
		حافظہ_نقل(&jsb->s_users[nr_users*16],
		       fs->super->s_uuid, 16);
		jsb->s_nr_users = htonl(nr_users+1);
	}

	/* Writeback the journal superblock */
	إذا ((retval = io_channel_write_blk64(journal_dev->io, شروع,
					    -SUPERBLOCK_ناپ, buf)))
		أرجع retval;

	fs->super->s_journal_inum = 0;
	fs->super->s_journal_dev = st.st_rdev;
	حافظہ_نقل(fs->super->s_journal_uuid, jsb->s_uuid,
	       حجم(fs->super->s_journal_uuid));
	memset(fs->super->s_jnl_بلاکاں, 0, حجم(fs->super->s_jnl_بلاکاں));
	ext2fs_سیٹ_feature_journal(fs->super);
	ext2fs_mark_super_dirty(fs);
	أرجع 0;
}

/*
 * This function adds a journal inode to a filesystem, using either
 * POSIX routines if the filesystem is mounted, or using direct I/O
 * functions if it is not.
 */
errcode_t ext2fs_جوڑو_journal_inode3(ext2_filsys fs, بنية ext2fs_journal_params *jparams,
				    blk64_t goal, صحيح flags)
{
	errcode_t		retval;
	ext2_ino_t		journal_ino;
	بنية stat		st;
	حرف			jfile[1024];
	صحيح			mount_flags;
	صحيح			fd = -1;

	إذا (flags & EXT2_MKJOURNAL_NO_MNT_CHECK)
		mount_flags = 0;
	وإلا إذا ((retval = ext2fs_check_mount_نقطہ(fs->device_name,
						    &mount_flags,
						    jfile, حجم(jfile)-10)))
		أرجع retval;

	إذا (mount_flags & EXT2_MF_MOUNTED) {
#إذا HAVE_EXT2_IOCTLS
		صحيح f = 0;
#جے_انت
		strcat(jfile, "/.journal");

		/*
		 * If .../.journal already exists, make sure any
		 * immutable or append-only flags are cleared.
		 */
#إذا defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		(فراغ) chflags (jfile, 0);
#وإلا
#إذا HAVE_EXT2_IOCTLS
		fd = open(jfile, O_RDONLY);
		إذا (fd >= 0) {
			retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
			close(fd);
			إذا (retval)
				أرجع errno;
		}
#جے_انت
#جے_انت

		/* Create the journal file */
		إذا ((fd = open(jfile, O_CREAT|O_WRONLY, 0600)) < 0)
			أرجع errno;

		/* Note that we can't do lazy journal initialization for mounted
		 * filesystems, since the zero writing is also allocating the
		 * journal blocks.  We could use fallocate, but not all kernels
		 * support that, and creating a journal on a mounted ext2
		 * filesystems is extremely rare these days...  Ignore it. */
		flags &= ~EXT2_MKJOURNAL_LAZYINIT;

		إذا ((retval = write_journal_file(fs, jfile, jparams, flags)))
			اذهب errout;

		/* Get inode number of the journal file */
		إذا (fstat(fd, &st) < 0) {
			retval = errno;
			اذهب errout;
		}

#إذا defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		retval = fchflags (fd, UF_NODUMP|UF_IMMUTABLE);
#وإلا
#إذا HAVE_EXT2_IOCTLS
		إذا (ioctl(fd, EXT2_IOC_GETFLAGS, &f) < 0) {
			retval = errno;
			اذهب errout;
		}
		f |= EXT2_NODUMP_FL | EXT2_IMMUTABLE_FL;
		retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
#جے_انت
#جے_انت
		إذا (retval) {
			retval = errno;
			اذهب errout;
		}

		إذا (close(fd) < 0) {
			retval = errno;
			fd = -1;
			اذهب errout;
		}
		journal_ino = st.st_ino;
		memset(fs->super->s_jnl_بلاکاں, 0,
		       حجم(fs->super->s_jnl_بلاکاں));
	} وإلا {
		إذا ((mount_flags & EXT2_MF_BUSY) &&
		    !(fs->flags & EXT2_FLAG_EXCLUSIVE)) {
			retval = EBUSY;
			اذهب errout;
		}
		journal_ino = EXT2_JOURNAL_INO;
		إذا ((retval = write_journal_inode(fs, journal_ino,
						  jparams, goal, flags)))
			أرجع retval;
	}

	fs->super->s_journal_inum = journal_ino;
	fs->super->s_journal_dev = 0;
	memset(fs->super->s_journal_uuid, 0,
	       حجم(fs->super->s_journal_uuid));
	ext2fs_سیٹ_feature_journal(fs->super);

	ext2fs_mark_super_dirty(fs);
	أرجع 0;
errout:
	إذا (fd >= 0)
		close(fd);
	أرجع retval;
}

errcode_t ext2fs_جوڑو_journal_inode2(ext2_filsys fs, blk_t عدد_بلاکاں,
				    blk64_t goal, صحيح flags)
{
	بنية ext2fs_journal_params jparams;

	jparams.عدد_journal_بلاکاں = عدد_بلاکاں;
	jparams.عدد_fc_بلاکاں = 0;

	أرجع ext2fs_جوڑو_journal_inode3(fs, &jparams, goal, flags);
}

errcode_t ext2fs_جوڑو_journal_inode(ext2_filsys fs, blk_t عدد_بلاکاں, صحيح flags)
{
	أرجع ext2fs_جوڑو_journal_inode2(fs, عدد_بلاکاں, ~0ULL, flags);
}


#جے_تعریف_شدہ DEBUG
مکھ(صحيح argc, حرف **argv)
{
	errcode_t	retval;
	حرف		*device_name;
	ext2_filsys	fs;

	إذا (argc < 2) {
		fprintf(stderr, "Usage: %s filesystem\n", argv[0]);
		باہر_نکلو(1);
	}
	device_name = argv[1];

	retval = ext2fs_open (device_name, EXT2_FLAG_RW, 0, 0,
			      unix_io_manager, &fs);
	إذا (retval) {
		com_err(argv[0], retval, "while opening %s", device_name);
		باہر_نکلو(1);
	}

	retval = ext2fs_جوڑو_journal_inode(fs, JBD2_MIN_JOURNAL_بلاکاں, 0);
	إذا (retval) {
		com_err(argv[0], retval, "while adding journal to %s",
			device_name);
		باہر_نکلو(1);
	}
	retval = ext2fs_flush(fs);
	إذا (retval) {
		لکھو("Warning, had trouble writing out superblocks.\n");
	}
	ext2fs_close_free(&fs);
	باہر_نکلو(0);

}
#جے_انت
