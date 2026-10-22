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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <assert.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#જો HAVE_ERRNO_H
#અન્તર્ભાવ <errno.h>
#endif
#અન્તર્ભાવ <fcntl.h>
#જો HAVE_SYS_STAT_H
#અન્તર્ભાવ <sys/stat.h>
#endif
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif
#જો HAVE_SYS_IOCTL_H
#અન્તર્ભાવ <sys/ioctl.h>
#endif
#જો HAVE_NETINET_IN_H
#અન્તર્ભાવ <netinet/in.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "e2p/e2p.h"
#અન્તર્ભાવ "ext2fsP.h"

#અન્તર્ભાવ "kernel-jbd.h"

/*
 * This function automatically sets up the journal superblock and
 * returns it as an allocated block.
 */
errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
					   સંરચના ext2fs_journal_params *jparams,
					   પૂર્ણાંક flags, અક્ષર **ret_jsb)
{
	errcode_t		retval;
	journal_superblock_t	*jsb;

	જો (jparams->અંક_journal_blocks < JBD2_MIN_JOURNAL_BLOCKS)
		પાછા EXT2_ET_JOURNAL_TOO_SMALL;

	જો ((retval = ext2fs_get_mem(fs->blocksize, &jsb)))
		પાછા retval;

	memset (jsb, 0, fs->blocksize);

	jsb->s_header.h_magic = htonl(JBD2_MAGIC_સંખ્યા);
	જો (flags & EXT2_MKJOURNAL_V1_SUPER)
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V1);
	નહીંતર
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V2);
	jsb->s_blocksize = htonl(fs->blocksize);
	jsb->s_maxlen = htonl(jparams->અંક_journal_blocks + jparams->અંક_fc_blocks);
	jsb->s_nr_users = htonl(1);
	jsb->s_first = htonl(1);
	jsb->s_sequence = htonl(1);
	jsb->s_અંક_fc_blks = htonl(jparams->અંક_fc_blocks);
	memcpy(jsb->s_uuid, fs->super->s_uuid, કદ(fs->super->s_uuid));
	/*
	 * If we're creating an external journal device, we need to
	 * adjust these fields.
	 */
	જો (ext2fs_has_feature_journal_dev(fs->super)) {
		jsb->s_nr_users = 0;
		jsb->s_first = htonl(ext2fs_journal_sb_start(fs->blocksize) + 1);
	}

	*ret_jsb = (અક્ષર *) jsb;
	પાછા 0;
}

errcode_t ext2fs_create_journal_superblock(ext2_filsys fs, __u32 અંક_blocks,
					પૂર્ણાંક flags, અક્ષર **ret_sb)
{
	સંરચના ext2fs_journal_params jparams;

	jparams.અંક_journal_blocks = અંક_blocks;
	jparams.અંક_fc_blocks = 0;

	પાછા ext2fs_create_journal_superblock2(fs, &jparams, flags, ret_sb);
}

/*
 * This function writes a journal using POSIX routines.  It is used
 * for creating external journals and creating journals on live
 * filesystems.
 */
સ્થિર errcode_t write_journal_file(ext2_filsys fs, અક્ષર *filename,
				    સંરચના ext2fs_journal_params *jparams,
				    પૂર્ણાંક flags)
{
	errcode_t	retval;
	અક્ષર		*buf = 0;
	પૂર્ણાંક		fd, ret_size;
	blk_t		i;

	જો ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		પાછા retval;

	/* Open the device or journal file */
	જો ((fd = open(filename, O_WRONLY)) < 0) {
		retval = errno;
		જાઓ errfree;
	}

	/* Write the superblock out */
	retval = EXT2_ET_SHORT_WRITE;
	ret_size = લિખ(fd, buf, fs->blocksize);
	જો (ret_size < 0) {
		retval = errno;
		જાઓ errout;
	}
	જો (ret_size != (પૂર્ણાંક) fs->blocksize)
		જાઓ errout;
	memset(buf, 0, fs->blocksize);

	જો (flags & EXT2_MKJOURNAL_LAZYINIT)
		જાઓ success;

	માટે (i = 1; i < jparams->અંક_journal_blocks + jparams->અંક_fc_blocks; i++) {
		ret_size = લિખ(fd, buf, fs->blocksize);
		જો (ret_size < 0) {
			retval = errno;
			જાઓ errout;
		}
		જો (ret_size != (પૂર્ણાંક) fs->blocksize)
			જાઓ errout;
	}

success:
	retval = 0;
errout:
	close(fd);
errfree:
	ext2fs_free_mem(&buf);
	પાછા retval;
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
#define MAX_STRIDE_LENGTH (4194304 / (પૂર્ણાંક) fs->blocksize)
errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, પૂર્ણાંક અંક,
			      blk64_t *ret_blk, પૂર્ણાંક *ret_ગણતરી)
{
	પૂર્ણાંક		j, ગણતરી;
	સ્થિર શૂન્ય	*buf;
	સ્થિર પૂર્ણાંક	stride_length;
	errcode_t	retval;

	/* If fs is null, clean up the static buffer and return */
	જો (!fs) {
		જો (buf) {
			મુક્ત(buf);
			buf = 0;
			stride_length = 0;
		}
		પાછા 0;
	}

	/* Deal with zeroing less than 1 block */
	જો (અંક <= 0)
		પાછા 0;

	/* Try a zero out command, if supported */
	retval = io_channel_zeroout(fs->io, blk, અંક);
	જો (retval == 0)
		પાછા 0;

	/* Allocate the zeroizing buffer if necessary */
	જો (અંક > stride_length && stride_length < MAX_STRIDE_LENGTH) {
		શૂન્ય *p;
		પૂર્ણાંક new_stride = અંક;

		જો (new_stride > MAX_STRIDE_LENGTH)
			new_stride = MAX_STRIDE_LENGTH;
		p = realloc(buf, fs->blocksize * new_stride);
		જો (!p)
			પાછા EXT2_ET_NO_MEMORY;
		buf = p;
		stride_length = new_stride;
		memset(buf, 0, fs->blocksize * stride_length);
	}
	/* OK, do the write loop */
	j=0;
	જ્યારે (j < અંક) {
		જો (blk % stride_length) {
			ગણતરી = stride_length - (blk % stride_length);
			જો (ગણતરી > (અંક - j))
				ગણતરી = અંક - j;
		} નહીંતર {
			ગણતરી = અંક - j;
			જો (ગણતરી > stride_length)
				ગણતરી = stride_length;
		}
		retval = io_channel_write_blk64(fs->io, blk, ગણતરી, buf);
		જો (retval) {
			જો (ret_ગણતરી)
				*ret_ગણતરી = ગણતરી;
			જો (ret_blk)
				*ret_blk = blk;
			પાછા retval;
		}
		j += ગણતરી; blk += ગણતરી;
	}
	પાછા 0;
}

errcode_t ext2fs_zero_blocks(ext2_filsys fs, blk_t blk, પૂર્ણાંક અંક,
			     blk_t *ret_blk, પૂર્ણાંક *ret_ગણતરી)
{
	blk64_t ret_blk2;
	errcode_t retval;

	retval = ext2fs_zero_blocks2(fs, blk, અંક, &ret_blk2, ret_ગણતરી);
	જો (retval)
		*ret_blk = (blk_t) ret_blk2;
	પાછા retval;
}

/*
 * Calculate the initial goal block to be roughly at the middle of the
 * filesystem.  Pick a group that has the largest number of free
 * blocks.
 */
સ્થિર blk64_t get_midpoint_journal_block(ext2_filsys fs)
{
	dgrp_t	group, start, end, i, log_flex;

	group = ext2fs_group_of_blk2(fs, (ext2fs_blocks_ગણતરી(fs->super) -
					 fs->super->s_first_data_block) / 2);
	log_flex = 1U << fs->super->s_log_groups_per_flex;
	જો (fs->super->s_log_groups_per_flex && (group > log_flex)) {
		group = group & ~(log_flex - 1);
		જ્યારે ((group < fs->group_desc_ગણતરી) &&
		       ext2fs_bg_free_blocks_ગણતરી(fs, group) == 0)
			group++;
		જો (group == fs->group_desc_ગણતરી)
			group = 0;
		start = group;
	} નહીંતર
		start = (group > 0) ? group-1 : group;
	end = ((group+1) < fs->group_desc_ગણતરી) ? group+1 : group;
	group = start;
	માટે (i = start + 1; i <= end; i++)
		જો (ext2fs_bg_free_blocks_ગણતરી(fs, i) >
		    ext2fs_bg_free_blocks_ગણતરી(fs, group))
			group = i;
	પાછા ext2fs_group_first_block2(fs, group);
}

/*
 * This function creates a journal using direct I/O routines.
 */
સ્થિર errcode_t write_journal_inode(ext2_filsys fs, ext2_ino_t journal_ino,
				     સંરચના ext2fs_journal_params *jparams,
				     blk64_t goal, પૂર્ણાંક flags)
{
	અક્ષર			*buf;
	errcode_t		retval;
	સંરચના ext2_inode	inode;
	unsigned long long	inode_size;
	પૂર્ણાંક			falloc_flags = EXT2_FALLOCATE_FORCE_INIT;
	blk64_t			zblk;
	time_t			now;

	જો ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		પાછા retval;

	જો ((retval = ext2fs_read_bitmaps(fs)))
		જાઓ out2;

	જો ((retval = ext2fs_read_inode(fs, journal_ino, &inode)))
		જાઓ out2;

	જો (inode.i_blocks > 0) {
		retval = EEXIST;
		જાઓ out2;
	}

	જો (goal == ~0ULL)
		goal = get_midpoint_journal_block(fs);

	જો (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;

	જો (!(flags & EXT2_MKJOURNAL_LAZYINIT))
		falloc_flags |= EXT2_FALLOCATE_ZERO_BLOCKS;

	inode_size = (unsigned long long)fs->blocksize *
			(jparams->અંક_journal_blocks + jparams->અંક_fc_blocks);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	inode.i_links_ગણતરી = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	retval = ext2fs_inode_size_set(fs, &inode, inode_size);
	જો (retval)
		જાઓ out2;

	retval = ext2fs_fallocate(fs, falloc_flags, journal_ino,
				  &inode, goal, 0,
				  jparams->અંક_journal_blocks + jparams->અંક_fc_blocks);
	જો (retval)
		જાઓ out2;

	જો ((retval = ext2fs_write_new_inode(fs, journal_ino, &inode)))
		જાઓ out2;

	retval = ext2fs_bmap2(fs, journal_ino, &inode, ખાલી, 0, 0, ખાલી, &zblk);
	જો (retval)
		જાઓ out2;

	retval = io_channel_write_blk64(fs->io, zblk, 1, buf);
	જો (retval)
		જાઓ out2;

	memcpy(fs->super->s_jnl_blocks, inode.i_block, EXT2_N_BLOCKS*4);
	fs->super->s_jnl_blocks[15] = inode.i_size_ઉપલું;
	fs->super->s_jnl_blocks[16] = inode.i_size;
	fs->super->s_jnl_backup_type = EXT3_JNL_BACKUP_BLOCKS;
	ext2fs_mark_super_dirty(fs);

out2:
	ext2fs_free_mem(&buf);
	પાછા retval;
}

/*
 * Find a reasonable journal file size (in blocks) given the number of blocks
 * in the filesystem.  For very small filesystems, it is not reasonable to
 * have a journal that fills more than half of the filesystem.
 *
 * n.b. comments assume 4k blocks
 */
પૂર્ણાંક ext2fs_default_journal_size(__u64 અંક_blocks)
{
	જો (અંક_blocks < 2048)
		પાછા -1;
	જો (અંક_blocks < 32768)		/* 128 MB */
		પાછા (1024);			/* 4 MB */
	જો (અંક_blocks < 256*1024)	/* 1 GB */
		પાછા (4096);			/* 16 MB */
	જો (અંક_blocks < 512*1024)	/* 2 GB */
		પાછા (8192);			/* 32 MB */
	જો (અંક_blocks < 4096*1024)	/* 16 GB */
		પાછા (16384);			/* 64 MB */
	જો (અંક_blocks < 8192*1024)	/* 32 GB */
		પાછા (32768);			/* 128 MB */
	જો (અંક_blocks < 16384*1024)	/* 64 GB */
		પાછા (65536);			/* 256 MB */
	જો (અંક_blocks < 32768*1024)	/* 128 GB */
		પાછા (131072);		/* 512 MB */
	પાછા 262144;				/* 1 GB */
}

errcode_t ext2fs_get_journal_params(સંરચના ext2fs_journal_params *params,
		ext2_filsys fs)
{
	blk_t total_blks;
	પૂર્ણાંક ret;

	memset(params, 0, કદ(*params));
	જો (ext2fs_has_feature_journal_dev(fs->super)) {
		total_blks = ext2fs_blocks_ગણતરી(fs->super);
		જો (total_blks < JBD2_MIN_JOURNAL_BLOCKS)
			પાછા EXT2_ET_JOURNAL_TOO_SMALL;

		જો (!ext2fs_has_feature_fast_commit(fs->super)) {
			params->અંક_journal_blocks = total_blks;
			params->અંક_fc_blocks = 0;
			પાછા 0;
		}
		params->અંક_journal_blocks = ext2fs_blocks_ગણતરી(fs->super) *
				EXT2_JOURNAL_TO_FC_BLKS_RATIO /
				(EXT2_JOURNAL_TO_FC_BLKS_RATIO + 1);
		જો (JBD2_MIN_JOURNAL_BLOCKS > params->અંક_journal_blocks)
			params->અંક_journal_blocks = JBD2_MIN_JOURNAL_BLOCKS;
		params->અંક_fc_blocks = total_blks - params->અંક_journal_blocks;
		પાછા 0;
	}

	ret = ext2fs_default_journal_size(ext2fs_blocks_ગણતરી(fs->super));
	જો (ret < 0)
		પાછા EXT2_ET_JOURNAL_TOO_SMALL;

	params->અંક_journal_blocks = ret;
	જો (ext2fs_has_feature_fast_commit(fs->super))
		params->અંક_fc_blocks = params->અંક_journal_blocks /
			EXT2_JOURNAL_TO_FC_BLKS_RATIO;
	પાછા 0;
}

પૂર્ણાંક ext2fs_journal_sb_start(પૂર્ણાંક blocksize)
{
	જો (blocksize == EXT2_MIN_BLOCK_SIZE)
		પાછા 2;
	પાછા 1;
}

/*
 * This function adds a journal device to a filesystem
 */
errcode_t ext2fs_add_journal_device(ext2_filsys fs, ext2_filsys journal_dev)
{
	સંરચના stat	st;
	errcode_t	retval;
	અક્ષર		buf[SUPERBLOCK_SIZE];
	journal_superblock_t	*jsb;
	પૂર્ણાંક		start;
	__u32		i, nr_users;

	/* Make sure the device exists and is a block device */
	જો (stat(journal_dev->device_name, &st) < 0)
		પાછા errno;

	જો (!S_ISBLK(st.st_mode))
		પાછા EXT2_ET_JOURNAL_NOT_BLOCK; /* Must be a block device */

	/* Get the journal superblock */
	start = ext2fs_journal_sb_start(journal_dev->blocksize);
	જો ((retval = io_channel_read_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE,
					    buf)))
		પાછા retval;

	jsb = (journal_superblock_t *) buf;
	જો ((jsb->s_header.h_magic != (unsigned) ntohl(JBD2_MAGIC_સંખ્યા)) ||
	    (jsb->s_header.h_blocktype != (unsigned) ntohl(JBD2_SUPERBLOCK_V2)))
		પાછા EXT2_ET_NO_JOURNAL_SB;

	જો (ntohl(jsb->s_blocksize) != (unsigned long) fs->blocksize)
		પાછા EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	/* Check and see if this filesystem has already been added */
	nr_users = ntohl(jsb->s_nr_users);
	જો (nr_users > JBD2_USERS_MAX)
		પાછા EXT2_ET_CORRUPT_JOURNAL_SB;
	માટે (i=0; i < nr_users; i++) {
		જો (memcmp(fs->super->s_uuid,
			   &jsb->s_users[i*16], 16) == 0)
			તોડો;
	}
	જો (i >= nr_users) {
		memcpy(&jsb->s_users[nr_users*16],
		       fs->super->s_uuid, 16);
		jsb->s_nr_users = htonl(nr_users+1);
	}

	/* Writeback the journal superblock */
	જો ((retval = io_channel_write_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE, buf)))
		પાછા retval;

	fs->super->s_journal_inum = 0;
	fs->super->s_journal_dev = st.st_rdev;
	memcpy(fs->super->s_journal_uuid, jsb->s_uuid,
	       કદ(fs->super->s_journal_uuid));
	memset(fs->super->s_jnl_blocks, 0, કદ(fs->super->s_jnl_blocks));
	ext2fs_set_feature_journal(fs->super);
	ext2fs_mark_super_dirty(fs);
	પાછા 0;
}

/*
 * This function adds a journal inode to a filesystem, using either
 * POSIX routines if the filesystem is mounted, or using direct I/O
 * functions if it is not.
 */
errcode_t ext2fs_add_journal_inode3(ext2_filsys fs, સંરચના ext2fs_journal_params *jparams,
				    blk64_t goal, પૂર્ણાંક flags)
{
	errcode_t		retval;
	ext2_ino_t		journal_ino;
	સંરચના stat		st;
	અક્ષર			jfile[1024];
	પૂર્ણાંક			mount_flags;
	પૂર્ણાંક			fd = -1;

	જો (flags & EXT2_MKJOURNAL_NO_MNT_CHECK)
		mount_flags = 0;
	નહીંતર જો ((retval = ext2fs_check_mount_point(fs->device_name,
						    &mount_flags,
						    jfile, કદ(jfile)-10)))
		પાછા retval;

	જો (mount_flags & EXT2_MF_MOUNTED) {
#જો HAVE_EXT2_IOCTLS
		પૂર્ણાંક f = 0;
#endif
		strcat(jfile, "/.journal");

		/*
		 * If .../.journal already exists, make sure any
		 * immutable or append-only flags are cleared.
		 */
#જો defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		(શૂન્ય) chflags (jfile, 0);
#નહીંતર
#જો HAVE_EXT2_IOCTLS
		fd = open(jfile, O_RDONLY);
		જો (fd >= 0) {
			retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
			close(fd);
			જો (retval)
				પાછા errno;
		}
#endif
#endif

		/* Create the journal file */
		જો ((fd = open(jfile, O_CREAT|O_WRONLY, 0600)) < 0)
			પાછા errno;

		/* Note that we can't do lazy journal initialization for mounted
		 * filesystems, since the zero writing is also allocating the
		 * journal blocks.  We could use fallocate, but not all kernels
		 * support that, and creating a journal on a mounted ext2
		 * filesystems is extremely rare these days...  Ignore it. */
		flags &= ~EXT2_MKJOURNAL_LAZYINIT;

		જો ((retval = write_journal_file(fs, jfile, jparams, flags)))
			જાઓ errout;

		/* Get inode number of the journal file */
		જો (fstat(fd, &st) < 0) {
			retval = errno;
			જાઓ errout;
		}

#જો defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		retval = fchflags (fd, UF_NODUMP|UF_IMMUTABLE);
#નહીંતર
#જો HAVE_EXT2_IOCTLS
		જો (ioctl(fd, EXT2_IOC_GETFLAGS, &f) < 0) {
			retval = errno;
			જાઓ errout;
		}
		f |= EXT2_NODUMP_FL | EXT2_IMMUTABLE_FL;
		retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
#endif
#endif
		જો (retval) {
			retval = errno;
			જાઓ errout;
		}

		જો (close(fd) < 0) {
			retval = errno;
			fd = -1;
			જાઓ errout;
		}
		journal_ino = st.st_ino;
		memset(fs->super->s_jnl_blocks, 0,
		       કદ(fs->super->s_jnl_blocks));
	} નહીંતર {
		જો ((mount_flags & EXT2_MF_BUSY) &&
		    !(fs->flags & EXT2_FLAG_EXCLUSIVE)) {
			retval = EBUSY;
			જાઓ errout;
		}
		journal_ino = EXT2_JOURNAL_INO;
		જો ((retval = write_journal_inode(fs, journal_ino,
						  jparams, goal, flags)))
			પાછા retval;
	}

	fs->super->s_journal_inum = journal_ino;
	fs->super->s_journal_dev = 0;
	memset(fs->super->s_journal_uuid, 0,
	       કદ(fs->super->s_journal_uuid));
	ext2fs_set_feature_journal(fs->super);

	ext2fs_mark_super_dirty(fs);
	પાછા 0;
errout:
	જો (fd >= 0)
		close(fd);
	પાછા retval;
}

errcode_t ext2fs_add_journal_inode2(ext2_filsys fs, blk_t અંક_blocks,
				    blk64_t goal, પૂર્ણાંક flags)
{
	સંરચના ext2fs_journal_params jparams;

	jparams.અંક_journal_blocks = અંક_blocks;
	jparams.અંક_fc_blocks = 0;

	પાછા ext2fs_add_journal_inode3(fs, &jparams, goal, flags);
}

errcode_t ext2fs_add_journal_inode(ext2_filsys fs, blk_t અંક_blocks, પૂર્ણાંક flags)
{
	પાછા ext2fs_add_journal_inode2(fs, અંક_blocks, ~0ULL, flags);
}


#ifdef DEBUG
મુખ્ય(પૂર્ણાંક argc, અક્ષર **argv)
{
	errcode_t	retval;
	અક્ષર		*device_name;
	ext2_filsys	fs;

	જો (argc < 2) {
		fprintf(stderr, "Usage: %s filesystem\n", argv[0]);
		exit(1);
	}
	device_name = argv[1];

	retval = ext2fs_open (device_name, EXT2_FLAG_RW, 0, 0,
			      unix_io_manager, &fs);
	જો (retval) {
		com_err(argv[0], retval, "while opening %s", device_name);
		exit(1);
	}

	retval = ext2fs_add_journal_inode(fs, JBD2_MIN_JOURNAL_BLOCKS, 0);
	જો (retval) {
		com_err(argv[0], retval, "while adding journal to %s",
			device_name);
		exit(1);
	}
	retval = ext2fs_flush(fs);
	જો (retval) {
		printf("Warning, had trouble writing out superblocks.\n");
	}
	ext2fs_close_free(&fs);
	exit(0);

}
#endif
