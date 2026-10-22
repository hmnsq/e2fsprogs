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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <assert.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः
#यदि HAVE_SYS_IOCTL_H
#अन्तर्भाव <sys/ioctl.h>
#यदि_अन्तः
#यदि HAVE_NETINET_अन्तर्_H
#अन्तर्भाव <netinet/in.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "e2p/e2p.h"
#अन्तर्भाव "ext2fsP.h"

#अन्तर्भाव "kernel-jbd.h"

/*
 * This function automatically sets up the journal superblock and
 * returns it as an allocated block.
 */
errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
					   संरचना ext2fs_journal_params *jparams,
					   पूर्णाङ्क flags, वर्ण **ret_jsb)
{
	errcode_t		retval;
	journal_superblock_t	*jsb;

	यदि (jparams->अङ्कः_journal_खण्डानि < JBD2_MIN_JOURNAL_खण्डानि)
		प्रत्यागम EXT2_ET_JOURNAL_TOO_SMALL;

	यदि ((retval = ext2fs_get_mem(fs->blocksize, &jsb)))
		प्रत्यागम retval;

	स्मृतिं_पूरय (jsb, 0, fs->blocksize);

	jsb->s_शीर्षकम्.h_magic = htonl(JBD2_MAGIC_सङ्ख्या);
	यदि (flags & EXT2_MKJOURNAL_V1_SUPER)
		jsb->s_शीर्षकम्.h_blocktype = htonl(JBD2_SUPERBLOCK_V1);
	अन्यथा
		jsb->s_शीर्षकम्.h_blocktype = htonl(JBD2_SUPERBLOCK_V2);
	jsb->s_blocksize = htonl(fs->blocksize);
	jsb->s_maxlen = htonl(jparams->अङ्कः_journal_खण्डानि + jparams->अङ्कः_fc_खण्डानि);
	jsb->s_nr_users = htonl(1);
	jsb->s_first = htonl(1);
	jsb->s_sequence = htonl(1);
	jsb->s_अङ्कः_fc_blks = htonl(jparams->अङ्कः_fc_खण्डानि);
	स्मृतिं_प्रतिलिख(jsb->s_uuid, fs->super->s_uuid, परिमाण(fs->super->s_uuid));
	/*
	 * If we're creating an external journal device, we need to
	 * adjust these fields.
	 */
	यदि (ext2fs_has_feature_journal_dev(fs->super)) {
		jsb->s_nr_users = 0;
		jsb->s_first = htonl(ext2fs_journal_sb_start(fs->blocksize) + 1);
	}

	*ret_jsb = (वर्ण *) jsb;
	प्रत्यागम 0;
}

errcode_t ext2fs_create_journal_superblock(ext2_filsys fs, __u32 अङ्कः_खण्डानि,
					पूर्णाङ्क flags, वर्ण **ret_sb)
{
	संरचना ext2fs_journal_params jparams;

	jparams.अङ्कः_journal_खण्डानि = अङ्कः_खण्डानि;
	jparams.अङ्कः_fc_खण्डानि = 0;

	प्रत्यागम ext2fs_create_journal_superblock2(fs, &jparams, flags, ret_sb);
}

/*
 * This function writes a journal using POSIX routines.  It is used
 * for creating external journals and creating journals on live
 * filesystems.
 */
स्थिर errcode_t write_journal_file(ext2_filsys fs, वर्ण *filename,
				    संरचना ext2fs_journal_params *jparams,
				    पूर्णाङ्क flags)
{
	errcode_t	retval;
	वर्ण		*buf = 0;
	पूर्णाङ्क		fd, ret_आकार;
	blk_t		i;

	यदि ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		प्रत्यागम retval;

	/* Open the device or journal file */
	यदि ((fd = उद्घाटय(filename, O_WRONLY)) < 0) {
		retval = errno;
		गच्छ errfree;
	}

	/* Write the superblock out */
	retval = EXT2_ET_SHORT_WRITE;
	ret_आकार = लिख(fd, buf, fs->blocksize);
	यदि (ret_आकार < 0) {
		retval = errno;
		गच्छ errout;
	}
	यदि (ret_आकार != (पूर्णाङ्क) fs->blocksize)
		गच्छ errout;
	स्मृतिं_पूरय(buf, 0, fs->blocksize);

	यदि (flags & EXT2_MKJOURNAL_LAZYINIT)
		गच्छ success;

	प्रति (i = 1; i < jparams->अङ्कः_journal_खण्डानि + jparams->अङ्कः_fc_खण्डानि; i++) {
		ret_आकार = लिख(fd, buf, fs->blocksize);
		यदि (ret_आकार < 0) {
			retval = errno;
			गच्छ errout;
		}
		यदि (ret_आकार != (पूर्णाङ्क) fs->blocksize)
			गच्छ errout;
	}

success:
	retval = 0;
errout:
	close(fd);
errfree:
	ext2fs_free_mem(&buf);
	प्रत्यागम retval;
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
#परिभाषय MAX_STRIDE_दीर्घता (4194304 / (पूर्णाङ्क) fs->blocksize)
errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, पूर्णाङ्क अङ्कः,
			      blk64_t *ret_blk, पूर्णाङ्क *ret_गणनम्)
{
	पूर्णाङ्क		j, गणनम्;
	स्थिर शून्य	*buf;
	स्थिर पूर्णाङ्क	stride_दीर्घता;
	errcode_t	retval;

	/* If fs is null, clean up the static buffer and return */
	यदि (!fs) {
		यदि (buf) {
			मुक्त(buf);
			buf = 0;
			stride_दीर्घता = 0;
		}
		प्रत्यागम 0;
	}

	/* Deal with zeroing less than 1 block */
	यदि (अङ्कः <= 0)
		प्रत्यागम 0;

	/* Try a zero out command, if supported */
	retval = io_channel_zeroout(fs->io, blk, अङ्कः);
	यदि (retval == 0)
		प्रत्यागम 0;

	/* Allocate the zeroizing buffer if necessary */
	यदि (अङ्कः > stride_दीर्घता && stride_दीर्घता < MAX_STRIDE_दीर्घता) {
		शून्य *p;
		पूर्णाङ्क new_stride = अङ्कः;

		यदि (new_stride > MAX_STRIDE_दीर्घता)
			new_stride = MAX_STRIDE_दीर्घता;
		p = realloc(buf, fs->blocksize * new_stride);
		यदि (!p)
			प्रत्यागम EXT2_ET_NO_स्मृतिः;
		buf = p;
		stride_दीर्घता = new_stride;
		स्मृतिं_पूरय(buf, 0, fs->blocksize * stride_दीर्घता);
	}
	/* OK, do the write loop */
	j=0;
	यावत् (j < अङ्कः) {
		यदि (blk % stride_दीर्घता) {
			गणनम् = stride_दीर्घता - (blk % stride_दीर्घता);
			यदि (गणनम् > (अङ्कः - j))
				गणनम् = अङ्कः - j;
		} अन्यथा {
			गणनम् = अङ्कः - j;
			यदि (गणनम् > stride_दीर्घता)
				गणनम् = stride_दीर्घता;
		}
		retval = io_channel_write_blk64(fs->io, blk, गणनम्, buf);
		यदि (retval) {
			यदि (ret_गणनम्)
				*ret_गणनम् = गणनम्;
			यदि (ret_blk)
				*ret_blk = blk;
			प्रत्यागम retval;
		}
		j += गणनम्; blk += गणनम्;
	}
	प्रत्यागम 0;
}

errcode_t ext2fs_zero_खण्डानि(ext2_filsys fs, blk_t blk, पूर्णाङ्क अङ्कः,
			     blk_t *ret_blk, पूर्णाङ्क *ret_गणनम्)
{
	blk64_t ret_blk2;
	errcode_t retval;

	retval = ext2fs_zero_blocks2(fs, blk, अङ्कः, &ret_blk2, ret_गणनम्);
	यदि (retval)
		*ret_blk = (blk_t) ret_blk2;
	प्रत्यागम retval;
}

/*
 * Calculate the initial goal block to be roughly at the middle of the
 * filesystem.  Pick a group that has the largest number of free
 * blocks.
 */
स्थिर blk64_t get_midpoint_journal_खण्डम्(ext2_filsys fs)
{
	dgrp_t	गणः, start, अन्तः, i, log_flex;

	गणः = ext2fs_गणः_सम्बन्धि_blk2(fs, (ext2fs_खण्डानि_गणनम्(fs->super) -
					 fs->super->s_first_दत्त_खण्डम्) / 2);
	log_flex = 1U << fs->super->s_log_groups_प्रत्येकम्_flex;
	यदि (fs->super->s_log_groups_प्रत्येकम्_flex && (गणः > log_flex)) {
		गणः = गणः & ~(log_flex - 1);
		यावत् ((गणः < fs->गणः_desc_गणनम्) &&
		       ext2fs_bg_free_खण्डानि_गणनम्(fs, गणः) == 0)
			गणः++;
		यदि (गणः == fs->गणः_desc_गणनम्)
			गणः = 0;
		start = गणः;
	} अन्यथा
		start = (गणः > 0) ? गणः-1 : गणः;
	अन्तः = ((गणः+1) < fs->गणः_desc_गणनम्) ? गणः+1 : गणः;
	गणः = start;
	प्रति (i = start + 1; i <= अन्तः; i++)
		यदि (ext2fs_bg_free_खण्डानि_गणनम्(fs, i) >
		    ext2fs_bg_free_खण्डानि_गणनम्(fs, गणः))
			गणः = i;
	प्रत्यागम ext2fs_गणः_first_block2(fs, गणः);
}

/*
 * This function creates a journal using direct I/O routines.
 */
स्थिर errcode_t write_journal_inode(ext2_filsys fs, ext2_ino_t journal_ino,
				     संरचना ext2fs_journal_params *jparams,
				     blk64_t goal, पूर्णाङ्क flags)
{
	वर्ण			*buf;
	errcode_t		retval;
	संरचना ext2_inode	inode;
	अचिह्नित दीर्घ दीर्घ	inode_आकार;
	पूर्णाङ्क			falloc_flags = EXT2_FALLOCATE_FORCE_INIT;
	blk64_t			zblk;
	time_t			now;

	यदि ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		प्रत्यागम retval;

	यदि ((retval = ext2fs_read_bitmaps(fs)))
		गच्छ out2;

	यदि ((retval = ext2fs_read_inode(fs, journal_ino, &inode)))
		गच्छ out2;

	यदि (inode.i_खण्डानि > 0) {
		retval = EEXIST;
		गच्छ out2;
	}

	यदि (goal == ~0ULL)
		goal = get_midpoint_journal_खण्डम्(fs);

	यदि (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;

	यदि (!(flags & EXT2_MKJOURNAL_LAZYINIT))
		falloc_flags |= EXT2_FALLOCATE_ZERO_खण्डानि;

	inode_आकार = (अचिह्नित दीर्घ दीर्घ)fs->blocksize *
			(jparams->अङ्कः_journal_खण्डानि + jparams->अङ्कः_fc_खण्डानि);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_समुच्चयः(&inode, i_mtime, now);
	ext2fs_inode_xtime_समुच्चयः(&inode, i_ctime, now);
	inode.i_links_गणनम् = 1;
	inode.i_बहुलकम् = LINUX_S_IFREG | 0600;
	retval = ext2fs_inode_आकार_समुच्चयः(fs, &inode, inode_आकार);
	यदि (retval)
		गच्छ out2;

	retval = ext2fs_fallocate(fs, falloc_flags, journal_ino,
				  &inode, goal, 0,
				  jparams->अङ्कः_journal_खण्डानि + jparams->अङ्कः_fc_खण्डानि);
	यदि (retval)
		गच्छ out2;

	यदि ((retval = ext2fs_write_new_inode(fs, journal_ino, &inode)))
		गच्छ out2;

	retval = ext2fs_bmap2(fs, journal_ino, &inode, रिक्त, 0, 0, रिक्त, &zblk);
	यदि (retval)
		गच्छ out2;

	retval = io_channel_write_blk64(fs->io, zblk, 1, buf);
	यदि (retval)
		गच्छ out2;

	स्मृतिं_प्रतिलिख(fs->super->s_jnl_खण्डानि, inode.i_खण्डम्, EXT2_N_खण्डानि*4);
	fs->super->s_jnl_खण्डानि[15] = inode.i_आकार_उच्चः;
	fs->super->s_jnl_खण्डानि[16] = inode.i_आकार;
	fs->super->s_jnl_backup_type = EXT3_JNL_BACKUP_खण्डानि;
	ext2fs_mark_super_dirty(fs);

out2:
	ext2fs_free_mem(&buf);
	प्रत्यागम retval;
}

/*
 * Find a reasonable journal file size (in blocks) given the number of blocks
 * in the filesystem.  For very small filesystems, it is not reasonable to
 * have a journal that fills more than half of the filesystem.
 *
 * n.b. comments assume 4k blocks
 */
पूर्णाङ्क ext2fs_default_journal_आकार(__u64 अङ्कः_खण्डानि)
{
	यदि (अङ्कः_खण्डानि < 2048)
		प्रत्यागम -1;
	यदि (अङ्कः_खण्डानि < 32768)		/* 128 MB */
		प्रत्यागम (1024);			/* 4 MB */
	यदि (अङ्कः_खण्डानि < 256*1024)	/* 1 GB */
		प्रत्यागम (4096);			/* 16 MB */
	यदि (अङ्कः_खण्डानि < 512*1024)	/* 2 GB */
		प्रत्यागम (8192);			/* 32 MB */
	यदि (अङ्कः_खण्डानि < 4096*1024)	/* 16 GB */
		प्रत्यागम (16384);			/* 64 MB */
	यदि (अङ्कः_खण्डानि < 8192*1024)	/* 32 GB */
		प्रत्यागम (32768);			/* 128 MB */
	यदि (अङ्कः_खण्डानि < 16384*1024)	/* 64 GB */
		प्रत्यागम (65536);			/* 256 MB */
	यदि (अङ्कः_खण्डानि < 32768*1024)	/* 128 GB */
		प्रत्यागम (131072);		/* 512 MB */
	प्रत्यागम 262144;				/* 1 GB */
}

errcode_t ext2fs_get_journal_params(संरचना ext2fs_journal_params *params,
		ext2_filsys fs)
{
	blk_t योग_blks;
	पूर्णाङ्क ret;

	स्मृतिं_पूरय(params, 0, परिमाण(*params));
	यदि (ext2fs_has_feature_journal_dev(fs->super)) {
		योग_blks = ext2fs_खण्डानि_गणनम्(fs->super);
		यदि (योग_blks < JBD2_MIN_JOURNAL_खण्डानि)
			प्रत्यागम EXT2_ET_JOURNAL_TOO_SMALL;

		यदि (!ext2fs_has_feature_fast_commit(fs->super)) {
			params->अङ्कः_journal_खण्डानि = योग_blks;
			params->अङ्कः_fc_खण्डानि = 0;
			प्रत्यागम 0;
		}
		params->अङ्कः_journal_खण्डानि = ext2fs_खण्डानि_गणनम्(fs->super) *
				EXT2_JOURNAL_गन्तव्यम्_FC_BLKS_अनुपातः /
				(EXT2_JOURNAL_गन्तव्यम्_FC_BLKS_अनुपातः + 1);
		यदि (JBD2_MIN_JOURNAL_खण्डानि > params->अङ्कः_journal_खण्डानि)
			params->अङ्कः_journal_खण्डानि = JBD2_MIN_JOURNAL_खण्डानि;
		params->अङ्कः_fc_खण्डानि = योग_blks - params->अङ्कः_journal_खण्डानि;
		प्रत्यागम 0;
	}

	ret = ext2fs_default_journal_आकार(ext2fs_खण्डानि_गणनम्(fs->super));
	यदि (ret < 0)
		प्रत्यागम EXT2_ET_JOURNAL_TOO_SMALL;

	params->अङ्कः_journal_खण्डानि = ret;
	यदि (ext2fs_has_feature_fast_commit(fs->super))
		params->अङ्कः_fc_खण्डानि = params->अङ्कः_journal_खण्डानि /
			EXT2_JOURNAL_गन्तव्यम्_FC_BLKS_अनुपातः;
	प्रत्यागम 0;
}

पूर्णाङ्क ext2fs_journal_sb_start(पूर्णाङ्क blocksize)
{
	यदि (blocksize == EXT2_MIN_खण्डम्_आकार)
		प्रत्यागम 2;
	प्रत्यागम 1;
}

/*
 * This function adds a journal device to a filesystem
 */
errcode_t ext2fs_सङ्कलय_journal_device(ext2_filsys fs, ext2_filsys journal_dev)
{
	संरचना stat	st;
	errcode_t	retval;
	वर्ण		buf[SUPERBLOCK_आकार];
	journal_superblock_t	*jsb;
	पूर्णाङ्क		start;
	__u32		i, nr_users;

	/* Make sure the device exists and is a block device */
	यदि (stat(journal_dev->device_name, &st) < 0)
		प्रत्यागम errno;

	यदि (!S_ISBLK(st.st_बहुलकम्))
		प्रत्यागम EXT2_ET_JOURNAL_NOT_खण्डम्; /* Must be a block device */

	/* Get the journal superblock */
	start = ext2fs_journal_sb_start(journal_dev->blocksize);
	यदि ((retval = io_channel_read_blk64(journal_dev->io, start,
					    -SUPERBLOCK_आकार,
					    buf)))
		प्रत्यागम retval;

	jsb = (journal_superblock_t *) buf;
	यदि ((jsb->s_शीर्षकम्.h_magic != (अचिह्नित) ntohl(JBD2_MAGIC_सङ्ख्या)) ||
	    (jsb->s_शीर्षकम्.h_blocktype != (अचिह्नित) ntohl(JBD2_SUPERBLOCK_V2)))
		प्रत्यागम EXT2_ET_NO_JOURNAL_SB;

	यदि (ntohl(jsb->s_blocksize) != (अचिह्नित दीर्घ) fs->blocksize)
		प्रत्यागम EXT2_ET_UNEXPECTED_खण्डम्_आकार;

	/* Check and see if this filesystem has already been added */
	nr_users = ntohl(jsb->s_nr_users);
	यदि (nr_users > JBD2_USERS_MAX)
		प्रत्यागम EXT2_ET_CORRUPT_JOURNAL_SB;
	प्रति (i=0; i < nr_users; i++) {
		यदि (memcmp(fs->super->s_uuid,
			   &jsb->s_users[i*16], 16) == 0)
			भङ्ग;
	}
	यदि (i >= nr_users) {
		स्मृतिं_प्रतिलिख(&jsb->s_users[nr_users*16],
		       fs->super->s_uuid, 16);
		jsb->s_nr_users = htonl(nr_users+1);
	}

	/* Writeback the journal superblock */
	यदि ((retval = io_channel_write_blk64(journal_dev->io, start,
					    -SUPERBLOCK_आकार, buf)))
		प्रत्यागम retval;

	fs->super->s_journal_inum = 0;
	fs->super->s_journal_dev = st.st_rdev;
	स्मृतिं_प्रतिलिख(fs->super->s_journal_uuid, jsb->s_uuid,
	       परिमाण(fs->super->s_journal_uuid));
	स्मृतिं_पूरय(fs->super->s_jnl_खण्डानि, 0, परिमाण(fs->super->s_jnl_खण्डानि));
	ext2fs_समुच्चयः_feature_journal(fs->super);
	ext2fs_mark_super_dirty(fs);
	प्रत्यागम 0;
}

/*
 * This function adds a journal inode to a filesystem, using either
 * POSIX routines if the filesystem is mounted, or using direct I/O
 * functions if it is not.
 */
errcode_t ext2fs_सङ्कलय_journal_inode3(ext2_filsys fs, संरचना ext2fs_journal_params *jparams,
				    blk64_t goal, पूर्णाङ्क flags)
{
	errcode_t		retval;
	ext2_ino_t		journal_ino;
	संरचना stat		st;
	वर्ण			jfile[1024];
	पूर्णाङ्क			mount_flags;
	पूर्णाङ्क			fd = -1;

	यदि (flags & EXT2_MKJOURNAL_NO_MNT_CHECK)
		mount_flags = 0;
	अन्यथा यदि ((retval = ext2fs_check_mount_point(fs->device_name,
						    &mount_flags,
						    jfile, परिमाण(jfile)-10)))
		प्रत्यागम retval;

	यदि (mount_flags & EXT2_MF_MOUNTED) {
#यदि HAVE_EXT2_IOCTLS
		पूर्णाङ्क f = 0;
#यदि_अन्तः
		वाक्यं_योजय(jfile, "/.journal");

		/*
		 * If .../.journal already exists, make sure any
		 * immutable or append-only flags are cleared.
		 */
#यदि defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		(शून्य) chflags (jfile, 0);
#अन्यथा
#यदि HAVE_EXT2_IOCTLS
		fd = उद्घाटय(jfile, O_RDONLY);
		यदि (fd >= 0) {
			retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
			close(fd);
			यदि (retval)
				प्रत्यागम errno;
		}
#यदि_अन्तः
#यदि_अन्तः

		/* Create the journal file */
		यदि ((fd = उद्घाटय(jfile, O_CREAT|O_WRONLY, 0600)) < 0)
			प्रत्यागम errno;

		/* Note that we can't do lazy journal initialization for mounted
		 * filesystems, since the zero writing is also allocating the
		 * journal blocks.  We could use fallocate, but not all kernels
		 * support that, and creating a journal on a mounted ext2
		 * filesystems is extremely rare these days...  Ignore it. */
		flags &= ~EXT2_MKJOURNAL_LAZYINIT;

		यदि ((retval = write_journal_file(fs, jfile, jparams, flags)))
			गच्छ errout;

		/* Get inode number of the journal file */
		यदि (fstat(fd, &st) < 0) {
			retval = errno;
			गच्छ errout;
		}

#यदि defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		retval = fchflags (fd, UF_NODUMP|UF_IMMUTABLE);
#अन्यथा
#यदि HAVE_EXT2_IOCTLS
		यदि (ioctl(fd, EXT2_IOC_GETFLAGS, &f) < 0) {
			retval = errno;
			गच्छ errout;
		}
		f |= EXT2_NODUMP_FL | EXT2_IMMUTABLE_FL;
		retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
#यदि_अन्तः
#यदि_अन्तः
		यदि (retval) {
			retval = errno;
			गच्छ errout;
		}

		यदि (close(fd) < 0) {
			retval = errno;
			fd = -1;
			गच्छ errout;
		}
		journal_ino = st.st_ino;
		स्मृतिं_पूरय(fs->super->s_jnl_खण्डानि, 0,
		       परिमाण(fs->super->s_jnl_खण्डानि));
	} अन्यथा {
		यदि ((mount_flags & EXT2_MF_BUSY) &&
		    !(fs->flags & EXT2_FLAG_EXCLUSIVE)) {
			retval = EBUSY;
			गच्छ errout;
		}
		journal_ino = EXT2_JOURNAL_INO;
		यदि ((retval = write_journal_inode(fs, journal_ino,
						  jparams, goal, flags)))
			प्रत्यागम retval;
	}

	fs->super->s_journal_inum = journal_ino;
	fs->super->s_journal_dev = 0;
	स्मृतिं_पूरय(fs->super->s_journal_uuid, 0,
	       परिमाण(fs->super->s_journal_uuid));
	ext2fs_समुच्चयः_feature_journal(fs->super);

	ext2fs_mark_super_dirty(fs);
	प्रत्यागम 0;
errout:
	यदि (fd >= 0)
		close(fd);
	प्रत्यागम retval;
}

errcode_t ext2fs_सङ्कलय_journal_inode2(ext2_filsys fs, blk_t अङ्कः_खण्डानि,
				    blk64_t goal, पूर्णाङ्क flags)
{
	संरचना ext2fs_journal_params jparams;

	jparams.अङ्कः_journal_खण्डानि = अङ्कः_खण्डानि;
	jparams.अङ्कः_fc_खण्डानि = 0;

	प्रत्यागम ext2fs_सङ्कलय_journal_inode3(fs, &jparams, goal, flags);
}

errcode_t ext2fs_सङ्कलय_journal_inode(ext2_filsys fs, blk_t अङ्कः_खण्डानि, पूर्णाङ्क flags)
{
	प्रत्यागम ext2fs_सङ्कलय_journal_inode2(fs, अङ्कः_खण्डानि, ~0ULL, flags);
}


#यदि_परिभाषितम् DEBUG
मुख्यम्(पूर्णाङ्क argc, वर्ण **argv)
{
	errcode_t	retval;
	वर्ण		*device_name;
	ext2_filsys	fs;

	यदि (argc < 2) {
		fprintf(stderr, "Usage: %s filesystem\n", argv[0]);
		निर्गच्छ(1);
	}
	device_name = argv[1];

	retval = ext2fs_open (device_name, EXT2_FLAG_RW, 0, 0,
			      unix_io_manager, &fs);
	यदि (retval) {
		com_err(argv[0], retval, "while opening %s", device_name);
		निर्गच्छ(1);
	}

	retval = ext2fs_सङ्कलय_journal_inode(fs, JBD2_MIN_JOURNAL_खण्डानि, 0);
	यदि (retval) {
		com_err(argv[0], retval, "while adding journal to %s",
			device_name);
		निर्गच्छ(1);
	}
	retval = ext2fs_flush(fs);
	यदि (retval) {
		प्रारूप_लिख("Warning, had trouble writing out superblocks.\n");
	}
	ext2fs_close_free(&fs);
	निर्गच्छ(0);

}
#यदि_अन्तः
