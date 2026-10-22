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
#جيڪڏهن_پورو
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جيڪڏهن_پورو
#ضمن <fcntl.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_IOCTL_H
#ضمن <sys/ioctl.h>
#جيڪڏهن_پورو
#إذا HAVE_NETINET_اندريون_H
#ضمن <netinet/in.h>
#جيڪڏهن_پورو

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

	إذا (jparams->انگ_journal_بلاڪون < JBD2_MIN_JOURNAL_بلاڪون)
		أرجع EXT2_ET_JOURNAL_TOO_SMALL;

	إذا ((retval = ext2fs_get_mem(fs->blocksize, &jsb)))
		أرجع retval;

	memset (jsb, 0, fs->blocksize);

	jsb->s_سرو.h_magic = htonl(JBD2_MAGIC_عدد);
	إذا (flags & EXT2_MKJOURNAL_V1_SUPER)
		jsb->s_سرو.h_blocktype = htonl(JBD2_SUPERBLOCK_V1);
	وإلا
		jsb->s_سرو.h_blocktype = htonl(JBD2_SUPERBLOCK_V2);
	jsb->s_blocksize = htonl(fs->blocksize);
	jsb->s_maxlen = htonl(jparams->انگ_journal_بلاڪون + jparams->انگ_fc_بلاڪون);
	jsb->s_nr_users = htonl(1);
	jsb->s_پهريون = htonl(1);
	jsb->s_sequence = htonl(1);
	jsb->s_انگ_fc_blks = htonl(jparams->انگ_fc_بلاڪون);
	يادگيري_نقل(jsb->s_uuid, fs->super->s_uuid, حجم(fs->super->s_uuid));
	/*
	 * If we're creating an external journal device, we need to
	 * adjust these fields.
	 */
	إذا (ext2fs_has_feature_journal_dev(fs->super)) {
		jsb->s_nr_users = 0;
		jsb->s_پهريون = htonl(ext2fs_journal_sb_شروعات(fs->blocksize) + 1);
	}

	*ret_jsb = (حرف *) jsb;
	أرجع 0;
}

errcode_t ext2fs_create_journal_superblock(ext2_filsys fs, __u32 انگ_بلاڪون,
					صحيح flags, حرف **ret_sb)
{
	بنية ext2fs_journal_params jparams;

	jparams.انگ_journal_بلاڪون = انگ_بلاڪون;
	jparams.انگ_fc_بلاڪون = 0;

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
	صحيح		fd, ret_سائيز;
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
	ret_سائيز = اكتب(fd, buf, fs->blocksize);
	إذا (ret_سائيز < 0) {
		retval = errno;
		اذهب errout;
	}
	إذا (ret_سائيز != (صحيح) fs->blocksize)
		اذهب errout;
	memset(buf, 0, fs->blocksize);

	إذا (flags & EXT2_MKJOURNAL_LAZYINIT)
		اذهب success;

	لكل (i = 1; i < jparams->انگ_journal_بلاڪون + jparams->انگ_fc_بلاڪون; i++) {
		ret_سائيز = اكتب(fd, buf, fs->blocksize);
		إذا (ret_سائيز < 0) {
			retval = errno;
			اذهب errout;
		}
		إذا (ret_سائيز != (صحيح) fs->blocksize)
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
#وصف_ڪريو MAX_STRIDE_ڊيگهائي (4194304 / (صحيح) fs->blocksize)
errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, صحيح انگ,
			      blk64_t *ret_blk, صحيح *ret_ڳڻپ)
{
	صحيح		j, ڳڻپ;
	ساكن فراغ	*buf;
	ساكن صحيح	stride_ڊيگهائي;
	errcode_t	retval;

	/* If fs is null, clean up the static buffer and return */
	إذا (!fs) {
		إذا (buf) {
			حرر(buf);
			buf = 0;
			stride_ڊيگهائي = 0;
		}
		أرجع 0;
	}

	/* Deal with zeroing less than 1 block */
	إذا (انگ <= 0)
		أرجع 0;

	/* Try a zero out command, if supported */
	retval = io_channel_zeroout(fs->io, blk, انگ);
	إذا (retval == 0)
		أرجع 0;

	/* Allocate the zeroizing buffer if necessary */
	إذا (انگ > stride_ڊيگهائي && stride_ڊيگهائي < MAX_STRIDE_ڊيگهائي) {
		فراغ *p;
		صحيح new_stride = انگ;

		إذا (new_stride > MAX_STRIDE_ڊيگهائي)
			new_stride = MAX_STRIDE_ڊيگهائي;
		p = realloc(buf, fs->blocksize * new_stride);
		إذا (!p)
			أرجع EXT2_ET_NO_يادگيري;
		buf = p;
		stride_ڊيگهائي = new_stride;
		memset(buf, 0, fs->blocksize * stride_ڊيگهائي);
	}
	/* OK, do the write loop */
	j=0;
	طالما (j < انگ) {
		إذا (blk % stride_ڊيگهائي) {
			ڳڻپ = stride_ڊيگهائي - (blk % stride_ڊيگهائي);
			إذا (ڳڻپ > (انگ - j))
				ڳڻپ = انگ - j;
		} وإلا {
			ڳڻپ = انگ - j;
			إذا (ڳڻپ > stride_ڊيگهائي)
				ڳڻپ = stride_ڊيگهائي;
		}
		retval = io_channel_write_blk64(fs->io, blk, ڳڻپ, buf);
		إذا (retval) {
			إذا (ret_ڳڻپ)
				*ret_ڳڻپ = ڳڻپ;
			إذا (ret_blk)
				*ret_blk = blk;
			أرجع retval;
		}
		j += ڳڻپ; blk += ڳڻپ;
	}
	أرجع 0;
}

errcode_t ext2fs_zero_بلاڪون(ext2_filsys fs, blk_t blk, صحيح انگ,
			     blk_t *ret_blk, صحيح *ret_ڳڻپ)
{
	blk64_t ret_blk2;
	errcode_t retval;

	retval = ext2fs_zero_blocks2(fs, blk, انگ, &ret_blk2, ret_ڳڻپ);
	إذا (retval)
		*ret_blk = (blk_t) ret_blk2;
	أرجع retval;
}

/*
 * Calculate the initial goal block to be roughly at the middle of the
 * filesystem.  Pick a group that has the largest number of free
 * blocks.
 */
ساكن blk64_t get_midpoint_journal_بلاڪ(ext2_filsys fs)
{
	dgrp_t	گروهه, شروعات, پڇاڙي, i, log_flex;

	گروهه = ext2fs_گروهه_جو_blk2(fs, (ext2fs_بلاڪون_ڳڻپ(fs->super) -
					 fs->super->s_پهريون_معلومات_بلاڪ) / 2);
	log_flex = 1U << fs->super->s_log_groups_في_flex;
	إذا (fs->super->s_log_groups_في_flex && (گروهه > log_flex)) {
		گروهه = گروهه & ~(log_flex - 1);
		طالما ((گروهه < fs->گروهه_desc_ڳڻپ) &&
		       ext2fs_bg_free_بلاڪون_ڳڻپ(fs, گروهه) == 0)
			گروهه++;
		إذا (گروهه == fs->گروهه_desc_ڳڻپ)
			گروهه = 0;
		شروعات = گروهه;
	} وإلا
		شروعات = (گروهه > 0) ? گروهه-1 : گروهه;
	پڇاڙي = ((گروهه+1) < fs->گروهه_desc_ڳڻپ) ? گروهه+1 : گروهه;
	گروهه = شروعات;
	لكل (i = شروعات + 1; i <= پڇاڙي; i++)
		إذا (ext2fs_bg_free_بلاڪون_ڳڻپ(fs, i) >
		    ext2fs_bg_free_بلاڪون_ڳڻپ(fs, گروهه))
			گروهه = i;
	أرجع ext2fs_گروهه_پهريون_block2(fs, گروهه);
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
	بنانشان ڊگهو ڊگهو	inode_سائيز;
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

	إذا (inode.i_بلاڪون > 0) {
		retval = EEXIST;
		اذهب out2;
	}

	إذا (goal == ~0ULL)
		goal = get_midpoint_journal_بلاڪ(fs);

	إذا (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;

	إذا (!(flags & EXT2_MKJOURNAL_LAZYINIT))
		falloc_flags |= EXT2_FALLOCATE_ZERO_بلاڪون;

	inode_سائيز = (بنانشان ڊگهو ڊگهو)fs->blocksize *
			(jparams->انگ_journal_بلاڪون + jparams->انگ_fc_بلاڪون);
	now = ext2fs_P_get_وقت(fs);
	ext2fs_inode_xtime_سيٽ(&inode, i_mtime, now);
	ext2fs_inode_xtime_سيٽ(&inode, i_ctime, now);
	inode.i_links_ڳڻپ = 1;
	inode.i_گهڻائي = LINUX_S_IFREG | 0600;
	retval = ext2fs_inode_سائيز_سيٽ(fs, &inode, inode_سائيز);
	إذا (retval)
		اذهب out2;

	retval = ext2fs_fallocate(fs, falloc_flags, journal_ino,
				  &inode, goal, 0,
				  jparams->انگ_journal_بلاڪون + jparams->انگ_fc_بلاڪون);
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

	يادگيري_نقل(fs->super->s_jnl_بلاڪون, inode.i_بلاڪ, EXT2_N_بلاڪون*4);
	fs->super->s_jnl_بلاڪون[15] = inode.i_سائيز_مٿاهون;
	fs->super->s_jnl_بلاڪون[16] = inode.i_سائيز;
	fs->super->s_jnl_backup_type = EXT3_JNL_BACKUP_بلاڪون;
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
صحيح ext2fs_default_journal_سائيز(__u64 انگ_بلاڪون)
{
	إذا (انگ_بلاڪون < 2048)
		أرجع -1;
	إذا (انگ_بلاڪون < 32768)		/* 128 MB */
		أرجع (1024);			/* 4 MB */
	إذا (انگ_بلاڪون < 256*1024)	/* 1 GB */
		أرجع (4096);			/* 16 MB */
	إذا (انگ_بلاڪون < 512*1024)	/* 2 GB */
		أرجع (8192);			/* 32 MB */
	إذا (انگ_بلاڪون < 4096*1024)	/* 16 GB */
		أرجع (16384);			/* 64 MB */
	إذا (انگ_بلاڪون < 8192*1024)	/* 32 GB */
		أرجع (32768);			/* 128 MB */
	إذا (انگ_بلاڪون < 16384*1024)	/* 64 GB */
		أرجع (65536);			/* 256 MB */
	إذا (انگ_بلاڪون < 32768*1024)	/* 128 GB */
		أرجع (131072);		/* 512 MB */
	أرجع 262144;				/* 1 GB */
}

errcode_t ext2fs_get_journal_params(بنية ext2fs_journal_params *params,
		ext2_filsys fs)
{
	blk_t ڪل_blks;
	صحيح ret;

	memset(params, 0, حجم(*params));
	إذا (ext2fs_has_feature_journal_dev(fs->super)) {
		ڪل_blks = ext2fs_بلاڪون_ڳڻپ(fs->super);
		إذا (ڪل_blks < JBD2_MIN_JOURNAL_بلاڪون)
			أرجع EXT2_ET_JOURNAL_TOO_SMALL;

		إذا (!ext2fs_has_feature_fast_commit(fs->super)) {
			params->انگ_journal_بلاڪون = ڪل_blks;
			params->انگ_fc_بلاڪون = 0;
			أرجع 0;
		}
		params->انگ_journal_بلاڪون = ext2fs_بلاڪون_ڳڻپ(fs->super) *
				EXT2_JOURNAL_منزل_FC_BLKS_تناسب /
				(EXT2_JOURNAL_منزل_FC_BLKS_تناسب + 1);
		إذا (JBD2_MIN_JOURNAL_بلاڪون > params->انگ_journal_بلاڪون)
			params->انگ_journal_بلاڪون = JBD2_MIN_JOURNAL_بلاڪون;
		params->انگ_fc_بلاڪون = ڪل_blks - params->انگ_journal_بلاڪون;
		أرجع 0;
	}

	ret = ext2fs_default_journal_سائيز(ext2fs_بلاڪون_ڳڻپ(fs->super));
	إذا (ret < 0)
		أرجع EXT2_ET_JOURNAL_TOO_SMALL;

	params->انگ_journal_بلاڪون = ret;
	إذا (ext2fs_has_feature_fast_commit(fs->super))
		params->انگ_fc_بلاڪون = params->انگ_journal_بلاڪون /
			EXT2_JOURNAL_منزل_FC_BLKS_تناسب;
	أرجع 0;
}

صحيح ext2fs_journal_sb_شروعات(صحيح blocksize)
{
	إذا (blocksize == EXT2_MIN_بلاڪ_سائيز)
		أرجع 2;
	أرجع 1;
}

/*
 * This function adds a journal device to a filesystem
 */
errcode_t ext2fs_جوڙيو_journal_device(ext2_filsys fs, ext2_filsys journal_dev)
{
	بنية stat	st;
	errcode_t	retval;
	حرف		buf[SUPERBLOCK_سائيز];
	journal_superblock_t	*jsb;
	صحيح		شروعات;
	__u32		i, nr_users;

	/* Make sure the device exists and is a block device */
	إذا (stat(journal_dev->device_name, &st) < 0)
		أرجع errno;

	إذا (!S_ISBLK(st.st_گهڻائي))
		أرجع EXT2_ET_JOURNAL_NOT_بلاڪ; /* Must be a block device */

	/* Get the journal superblock */
	شروعات = ext2fs_journal_sb_شروعات(journal_dev->blocksize);
	إذا ((retval = io_channel_read_blk64(journal_dev->io, شروعات,
					    -SUPERBLOCK_سائيز,
					    buf)))
		أرجع retval;

	jsb = (journal_superblock_t *) buf;
	إذا ((jsb->s_سرو.h_magic != (بنانشان) ntohl(JBD2_MAGIC_عدد)) ||
	    (jsb->s_سرو.h_blocktype != (بنانشان) ntohl(JBD2_SUPERBLOCK_V2)))
		أرجع EXT2_ET_NO_JOURNAL_SB;

	إذا (ntohl(jsb->s_blocksize) != (بنانشان ڊگهو) fs->blocksize)
		أرجع EXT2_ET_UNEXPECTED_بلاڪ_سائيز;

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
		يادگيري_نقل(&jsb->s_users[nr_users*16],
		       fs->super->s_uuid, 16);
		jsb->s_nr_users = htonl(nr_users+1);
	}

	/* Writeback the journal superblock */
	إذا ((retval = io_channel_write_blk64(journal_dev->io, شروعات,
					    -SUPERBLOCK_سائيز, buf)))
		أرجع retval;

	fs->super->s_journal_inum = 0;
	fs->super->s_journal_dev = st.st_rdev;
	يادگيري_نقل(fs->super->s_journal_uuid, jsb->s_uuid,
	       حجم(fs->super->s_journal_uuid));
	memset(fs->super->s_jnl_بلاڪون, 0, حجم(fs->super->s_jnl_بلاڪون));
	ext2fs_سيٽ_feature_journal(fs->super);
	ext2fs_mark_super_dirty(fs);
	أرجع 0;
}

/*
 * This function adds a journal inode to a filesystem, using either
 * POSIX routines if the filesystem is mounted, or using direct I/O
 * functions if it is not.
 */
errcode_t ext2fs_جوڙيو_journal_inode3(ext2_filsys fs, بنية ext2fs_journal_params *jparams,
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
	وإلا إذا ((retval = ext2fs_check_mount_نقطو(fs->device_name,
						    &mount_flags,
						    jfile, حجم(jfile)-10)))
		أرجع retval;

	إذا (mount_flags & EXT2_MF_MOUNTED) {
#إذا HAVE_EXT2_IOCTLS
		صحيح f = 0;
#جيڪڏهن_پورو
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
#جيڪڏهن_پورو
#جيڪڏهن_پورو

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
#جيڪڏهن_پورو
#جيڪڏهن_پورو
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
		memset(fs->super->s_jnl_بلاڪون, 0,
		       حجم(fs->super->s_jnl_بلاڪون));
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
	ext2fs_سيٽ_feature_journal(fs->super);

	ext2fs_mark_super_dirty(fs);
	أرجع 0;
errout:
	إذا (fd >= 0)
		close(fd);
	أرجع retval;
}

errcode_t ext2fs_جوڙيو_journal_inode2(ext2_filsys fs, blk_t انگ_بلاڪون,
				    blk64_t goal, صحيح flags)
{
	بنية ext2fs_journal_params jparams;

	jparams.انگ_journal_بلاڪون = انگ_بلاڪون;
	jparams.انگ_fc_بلاڪون = 0;

	أرجع ext2fs_جوڙيو_journal_inode3(fs, &jparams, goal, flags);
}

errcode_t ext2fs_جوڙيو_journal_inode(ext2_filsys fs, blk_t انگ_بلاڪون, صحيح flags)
{
	أرجع ext2fs_جوڙيو_journal_inode2(fs, انگ_بلاڪون, ~0ULL, flags);
}


#جيڪڏهن_وصف_ٿيل DEBUG
مکيه(صحيح argc, حرف **argv)
{
	errcode_t	retval;
	حرف		*device_name;
	ext2_filsys	fs;

	إذا (argc < 2) {
		fprintf(stderr, "Usage: %s filesystem\n", argv[0]);
		ٻاهر_نڪرو(1);
	}
	device_name = argv[1];

	retval = ext2fs_open (device_name, EXT2_FLAG_RW, 0, 0,
			      unix_io_manager, &fs);
	إذا (retval) {
		com_err(argv[0], retval, "while opening %s", device_name);
		ٻاهر_نڪرو(1);
	}

	retval = ext2fs_جوڙيو_journal_inode(fs, JBD2_MIN_JOURNAL_بلاڪون, 0);
	إذا (retval) {
		com_err(argv[0], retval, "while adding journal to %s",
			device_name);
		ٻاهر_نڪرو(1);
	}
	retval = ext2fs_flush(fs);
	إذا (retval) {
		لکو("Warning, had trouble writing out superblocks.\n");
	}
	ext2fs_close_free(&fs);
	ٻاهر_نڪرو(0);

}
#جيڪڏهن_پورو
