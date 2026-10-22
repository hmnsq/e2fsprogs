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
#endif
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#endif
#شامل <fcntl.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif
#اگر HAVE_SYS_IOCTL_H
#شامل <sys/ioctl.h>
#endif
#اگر HAVE_NETINET_IN_H
#شامل <netinet/in.h>
#endif

#شامل "ext2_fs.h"
#شامل "e2p/e2p.h"
#شامل "ext2fsP.h"

#شامل "kernel-jbd.h"

/*
 * This function automatically sets up the journal superblock and
 * returns it as an allocated block.
 */
errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
					   ساختار ext2fs_journal_params *jparams,
					   صحیح flags, نویسه **ret_jsb)
{
	errcode_t		retval;
	journal_superblock_t	*jsb;

	اگر (jparams->رقم_journal_blocks < JBD2_MIN_JOURNAL_BLOCKS)
		بازگشت EXT2_ET_JOURNAL_TOO_SMALL;

	اگر ((retval = ext2fs_get_mem(fs->blocksize, &jsb)))
		بازگشت retval;

	memset (jsb, 0, fs->blocksize);

	jsb->s_header.h_magic = htonl(JBD2_MAGIC_عدد);
	اگر (flags & EXT2_MKJOURNAL_V1_SUPER)
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V1);
	وگرنه
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V2);
	jsb->s_blocksize = htonl(fs->blocksize);
	jsb->s_maxlen = htonl(jparams->رقم_journal_blocks + jparams->رقم_fc_blocks);
	jsb->s_nr_users = htonl(1);
	jsb->s_first = htonl(1);
	jsb->s_sequence = htonl(1);
	jsb->s_رقم_fc_blks = htonl(jparams->رقم_fc_blocks);
	memcpy(jsb->s_uuid, fs->super->s_uuid, اندازه(fs->super->s_uuid));
	/*
	 * If we're creating an external journal device, we need to
	 * adjust these fields.
	 */
	اگر (ext2fs_has_feature_journal_dev(fs->super)) {
		jsb->s_nr_users = 0;
		jsb->s_first = htonl(ext2fs_journal_sb_start(fs->blocksize) + 1);
	}

	*ret_jsb = (نویسه *) jsb;
	بازگشت 0;
}

errcode_t ext2fs_create_journal_superblock(ext2_filsys fs, __u32 رقم_blocks,
					صحیح flags, نویسه **ret_sb)
{
	ساختار ext2fs_journal_params jparams;

	jparams.رقم_journal_blocks = رقم_blocks;
	jparams.رقم_fc_blocks = 0;

	بازگشت ext2fs_create_journal_superblock2(fs, &jparams, flags, ret_sb);
}

/*
 * This function writes a journal using POSIX routines.  It is used
 * for creating external journals and creating journals on live
 * filesystems.
 */
ایستا errcode_t write_journal_file(ext2_filsys fs, نویسه *filename,
				    ساختار ext2fs_journal_params *jparams,
				    صحیح flags)
{
	errcode_t	retval;
	نویسه		*buf = 0;
	صحیح		fd, ret_size;
	blk_t		i;

	اگر ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		بازگشت retval;

	/* Open the device or journal file */
	اگر ((fd = open(filename, O_WRONLY)) < 0) {
		retval = errno;
		برو errfree;
	}

	/* Write the superblock out */
	retval = EXT2_ET_SHORT_WRITE;
	ret_size = اكتب(fd, buf, fs->blocksize);
	اگر (ret_size < 0) {
		retval = errno;
		برو errout;
	}
	اگر (ret_size != (صحیح) fs->blocksize)
		برو errout;
	memset(buf, 0, fs->blocksize);

	اگر (flags & EXT2_MKJOURNAL_LAZYINIT)
		برو success;

	برای (i = 1; i < jparams->رقم_journal_blocks + jparams->رقم_fc_blocks; i++) {
		ret_size = اكتب(fd, buf, fs->blocksize);
		اگر (ret_size < 0) {
			retval = errno;
			برو errout;
		}
		اگر (ret_size != (صحیح) fs->blocksize)
			برو errout;
	}

success:
	retval = 0;
errout:
	close(fd);
errfree:
	ext2fs_free_mem(&buf);
	بازگشت retval;
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
#define MAX_STRIDE_LENGTH (4194304 / (صحیح) fs->blocksize)
errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, صحیح رقم,
			      blk64_t *ret_blk, صحیح *ret_شمار)
{
	صحیح		j, شمار;
	ایستا تهی	*buf;
	ایستا صحیح	stride_length;
	errcode_t	retval;

	/* If fs is null, clean up the static buffer and return */
	اگر (!fs) {
		اگر (buf) {
			آزاد(buf);
			buf = 0;
			stride_length = 0;
		}
		بازگشت 0;
	}

	/* Deal with zeroing less than 1 block */
	اگر (رقم <= 0)
		بازگشت 0;

	/* Try a zero out command, if supported */
	retval = io_channel_zeroout(fs->io, blk, رقم);
	اگر (retval == 0)
		بازگشت 0;

	/* Allocate the zeroizing buffer if necessary */
	اگر (رقم > stride_length && stride_length < MAX_STRIDE_LENGTH) {
		تهی *p;
		صحیح new_stride = رقم;

		اگر (new_stride > MAX_STRIDE_LENGTH)
			new_stride = MAX_STRIDE_LENGTH;
		p = realloc(buf, fs->blocksize * new_stride);
		اگر (!p)
			بازگشت EXT2_ET_NO_MEMORY;
		buf = p;
		stride_length = new_stride;
		memset(buf, 0, fs->blocksize * stride_length);
	}
	/* OK, do the write loop */
	j=0;
	وقتی (j < رقم) {
		اگر (blk % stride_length) {
			شمار = stride_length - (blk % stride_length);
			اگر (شمار > (رقم - j))
				شمار = رقم - j;
		} وگرنه {
			شمار = رقم - j;
			اگر (شمار > stride_length)
				شمار = stride_length;
		}
		retval = io_channel_write_blk64(fs->io, blk, شمار, buf);
		اگر (retval) {
			اگر (ret_شمار)
				*ret_شمار = شمار;
			اگر (ret_blk)
				*ret_blk = blk;
			بازگشت retval;
		}
		j += شمار; blk += شمار;
	}
	بازگشت 0;
}

errcode_t ext2fs_zero_blocks(ext2_filsys fs, blk_t blk, صحیح رقم,
			     blk_t *ret_blk, صحیح *ret_شمار)
{
	blk64_t ret_blk2;
	errcode_t retval;

	retval = ext2fs_zero_blocks2(fs, blk, رقم, &ret_blk2, ret_شمار);
	اگر (retval)
		*ret_blk = (blk_t) ret_blk2;
	بازگشت retval;
}

/*
 * Calculate the initial goal block to be roughly at the middle of the
 * filesystem.  Pick a group that has the largest number of free
 * blocks.
 */
ایستا blk64_t get_midpoint_journal_block(ext2_filsys fs)
{
	dgrp_t	group, start, end, i, log_flex;

	group = ext2fs_group_of_blk2(fs, (ext2fs_blocks_شمار(fs->super) -
					 fs->super->s_first_data_block) / 2);
	log_flex = 1U << fs->super->s_log_groups_per_flex;
	اگر (fs->super->s_log_groups_per_flex && (group > log_flex)) {
		group = group & ~(log_flex - 1);
		وقتی ((group < fs->group_desc_شمار) &&
		       ext2fs_bg_free_blocks_شمار(fs, group) == 0)
			group++;
		اگر (group == fs->group_desc_شمار)
			group = 0;
		start = group;
	} وگرنه
		start = (group > 0) ? group-1 : group;
	end = ((group+1) < fs->group_desc_شمار) ? group+1 : group;
	group = start;
	برای (i = start + 1; i <= end; i++)
		اگر (ext2fs_bg_free_blocks_شمار(fs, i) >
		    ext2fs_bg_free_blocks_شمار(fs, group))
			group = i;
	بازگشت ext2fs_group_first_block2(fs, group);
}

/*
 * This function creates a journal using direct I/O routines.
 */
ایستا errcode_t write_journal_inode(ext2_filsys fs, ext2_ino_t journal_ino,
				     ساختار ext2fs_journal_params *jparams,
				     blk64_t goal, صحیح flags)
{
	نویسه			*buf;
	errcode_t		retval;
	ساختار ext2_inode	inode;
	unsigned long long	inode_size;
	صحیح			falloc_flags = EXT2_FALLOCATE_FORCE_INIT;
	blk64_t			zblk;
	time_t			now;

	اگر ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		بازگشت retval;

	اگر ((retval = ext2fs_read_bitmaps(fs)))
		برو out2;

	اگر ((retval = ext2fs_read_inode(fs, journal_ino, &inode)))
		برو out2;

	اگر (inode.i_blocks > 0) {
		retval = EEXIST;
		برو out2;
	}

	اگر (goal == ~0ULL)
		goal = get_midpoint_journal_block(fs);

	اگر (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;

	اگر (!(flags & EXT2_MKJOURNAL_LAZYINIT))
		falloc_flags |= EXT2_FALLOCATE_ZERO_BLOCKS;

	inode_size = (unsigned long long)fs->blocksize *
			(jparams->رقم_journal_blocks + jparams->رقم_fc_blocks);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	inode.i_links_شمار = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	retval = ext2fs_inode_size_set(fs, &inode, inode_size);
	اگر (retval)
		برو out2;

	retval = ext2fs_fallocate(fs, falloc_flags, journal_ino,
				  &inode, goal, 0,
				  jparams->رقم_journal_blocks + jparams->رقم_fc_blocks);
	اگر (retval)
		برو out2;

	اگر ((retval = ext2fs_write_new_inode(fs, journal_ino, &inode)))
		برو out2;

	retval = ext2fs_bmap2(fs, journal_ino, &inode, پوچ, 0, 0, پوچ, &zblk);
	اگر (retval)
		برو out2;

	retval = io_channel_write_blk64(fs->io, zblk, 1, buf);
	اگر (retval)
		برو out2;

	memcpy(fs->super->s_jnl_blocks, inode.i_block, EXT2_N_BLOCKS*4);
	fs->super->s_jnl_blocks[15] = inode.i_size_بالا;
	fs->super->s_jnl_blocks[16] = inode.i_size;
	fs->super->s_jnl_backup_type = EXT3_JNL_BACKUP_BLOCKS;
	ext2fs_mark_super_dirty(fs);

out2:
	ext2fs_free_mem(&buf);
	بازگشت retval;
}

/*
 * Find a reasonable journal file size (in blocks) given the number of blocks
 * in the filesystem.  For very small filesystems, it is not reasonable to
 * have a journal that fills more than half of the filesystem.
 *
 * n.b. comments assume 4k blocks
 */
صحیح ext2fs_default_journal_size(__u64 رقم_blocks)
{
	اگر (رقم_blocks < 2048)
		بازگشت -1;
	اگر (رقم_blocks < 32768)		/* 128 MB */
		بازگشت (1024);			/* 4 MB */
	اگر (رقم_blocks < 256*1024)	/* 1 GB */
		بازگشت (4096);			/* 16 MB */
	اگر (رقم_blocks < 512*1024)	/* 2 GB */
		بازگشت (8192);			/* 32 MB */
	اگر (رقم_blocks < 4096*1024)	/* 16 GB */
		بازگشت (16384);			/* 64 MB */
	اگر (رقم_blocks < 8192*1024)	/* 32 GB */
		بازگشت (32768);			/* 128 MB */
	اگر (رقم_blocks < 16384*1024)	/* 64 GB */
		بازگشت (65536);			/* 256 MB */
	اگر (رقم_blocks < 32768*1024)	/* 128 GB */
		بازگشت (131072);		/* 512 MB */
	بازگشت 262144;				/* 1 GB */
}

errcode_t ext2fs_get_journal_params(ساختار ext2fs_journal_params *params,
		ext2_filsys fs)
{
	blk_t total_blks;
	صحیح ret;

	memset(params, 0, اندازه(*params));
	اگر (ext2fs_has_feature_journal_dev(fs->super)) {
		total_blks = ext2fs_blocks_شمار(fs->super);
		اگر (total_blks < JBD2_MIN_JOURNAL_BLOCKS)
			بازگشت EXT2_ET_JOURNAL_TOO_SMALL;

		اگر (!ext2fs_has_feature_fast_commit(fs->super)) {
			params->رقم_journal_blocks = total_blks;
			params->رقم_fc_blocks = 0;
			بازگشت 0;
		}
		params->رقم_journal_blocks = ext2fs_blocks_شمار(fs->super) *
				EXT2_JOURNAL_TO_FC_BLKS_RATIO /
				(EXT2_JOURNAL_TO_FC_BLKS_RATIO + 1);
		اگر (JBD2_MIN_JOURNAL_BLOCKS > params->رقم_journal_blocks)
			params->رقم_journal_blocks = JBD2_MIN_JOURNAL_BLOCKS;
		params->رقم_fc_blocks = total_blks - params->رقم_journal_blocks;
		بازگشت 0;
	}

	ret = ext2fs_default_journal_size(ext2fs_blocks_شمار(fs->super));
	اگر (ret < 0)
		بازگشت EXT2_ET_JOURNAL_TOO_SMALL;

	params->رقم_journal_blocks = ret;
	اگر (ext2fs_has_feature_fast_commit(fs->super))
		params->رقم_fc_blocks = params->رقم_journal_blocks /
			EXT2_JOURNAL_TO_FC_BLKS_RATIO;
	بازگشت 0;
}

صحیح ext2fs_journal_sb_start(صحیح blocksize)
{
	اگر (blocksize == EXT2_MIN_BLOCK_SIZE)
		بازگشت 2;
	بازگشت 1;
}

/*
 * This function adds a journal device to a filesystem
 */
errcode_t ext2fs_add_journal_device(ext2_filsys fs, ext2_filsys journal_dev)
{
	ساختار stat	st;
	errcode_t	retval;
	نویسه		buf[SUPERBLOCK_SIZE];
	journal_superblock_t	*jsb;
	صحیح		start;
	__u32		i, nr_users;

	/* Make sure the device exists and is a block device */
	اگر (stat(journal_dev->device_name, &st) < 0)
		بازگشت errno;

	اگر (!S_ISBLK(st.st_mode))
		بازگشت EXT2_ET_JOURNAL_NOT_BLOCK; /* Must be a block device */

	/* Get the journal superblock */
	start = ext2fs_journal_sb_start(journal_dev->blocksize);
	اگر ((retval = io_channel_read_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE,
					    buf)))
		بازگشت retval;

	jsb = (journal_superblock_t *) buf;
	اگر ((jsb->s_header.h_magic != (unsigned) ntohl(JBD2_MAGIC_عدد)) ||
	    (jsb->s_header.h_blocktype != (unsigned) ntohl(JBD2_SUPERBLOCK_V2)))
		بازگشت EXT2_ET_NO_JOURNAL_SB;

	اگر (ntohl(jsb->s_blocksize) != (unsigned long) fs->blocksize)
		بازگشت EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	/* Check and see if this filesystem has already been added */
	nr_users = ntohl(jsb->s_nr_users);
	اگر (nr_users > JBD2_USERS_MAX)
		بازگشت EXT2_ET_CORRUPT_JOURNAL_SB;
	برای (i=0; i < nr_users; i++) {
		اگر (memcmp(fs->super->s_uuid,
			   &jsb->s_users[i*16], 16) == 0)
			بشکن;
	}
	اگر (i >= nr_users) {
		memcpy(&jsb->s_users[nr_users*16],
		       fs->super->s_uuid, 16);
		jsb->s_nr_users = htonl(nr_users+1);
	}

	/* Writeback the journal superblock */
	اگر ((retval = io_channel_write_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE, buf)))
		بازگشت retval;

	fs->super->s_journal_inum = 0;
	fs->super->s_journal_dev = st.st_rdev;
	memcpy(fs->super->s_journal_uuid, jsb->s_uuid,
	       اندازه(fs->super->s_journal_uuid));
	memset(fs->super->s_jnl_blocks, 0, اندازه(fs->super->s_jnl_blocks));
	ext2fs_set_feature_journal(fs->super);
	ext2fs_mark_super_dirty(fs);
	بازگشت 0;
}

/*
 * This function adds a journal inode to a filesystem, using either
 * POSIX routines if the filesystem is mounted, or using direct I/O
 * functions if it is not.
 */
errcode_t ext2fs_add_journal_inode3(ext2_filsys fs, ساختار ext2fs_journal_params *jparams,
				    blk64_t goal, صحیح flags)
{
	errcode_t		retval;
	ext2_ino_t		journal_ino;
	ساختار stat		st;
	نویسه			jfile[1024];
	صحیح			mount_flags;
	صحیح			fd = -1;

	اگر (flags & EXT2_MKJOURNAL_NO_MNT_CHECK)
		mount_flags = 0;
	وگرنه اگر ((retval = ext2fs_check_mount_point(fs->device_name,
						    &mount_flags,
						    jfile, اندازه(jfile)-10)))
		بازگشت retval;

	اگر (mount_flags & EXT2_MF_MOUNTED) {
#اگر HAVE_EXT2_IOCTLS
		صحیح f = 0;
#endif
		strcat(jfile, "/.journal");

		/*
		 * If .../.journal already exists, make sure any
		 * immutable or append-only flags are cleared.
		 */
#اگر defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		(تهی) chflags (jfile, 0);
#وگرنه
#اگر HAVE_EXT2_IOCTLS
		fd = open(jfile, O_RDONLY);
		اگر (fd >= 0) {
			retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
			close(fd);
			اگر (retval)
				بازگشت errno;
		}
#endif
#endif

		/* Create the journal file */
		اگر ((fd = open(jfile, O_CREAT|O_WRONLY, 0600)) < 0)
			بازگشت errno;

		/* Note that we can't do lazy journal initialization for mounted
		 * filesystems, since the zero writing is also allocating the
		 * journal blocks.  We could use fallocate, but not all kernels
		 * support that, and creating a journal on a mounted ext2
		 * filesystems is extremely rare these days...  Ignore it. */
		flags &= ~EXT2_MKJOURNAL_LAZYINIT;

		اگر ((retval = write_journal_file(fs, jfile, jparams, flags)))
			برو errout;

		/* Get inode number of the journal file */
		اگر (fstat(fd, &st) < 0) {
			retval = errno;
			برو errout;
		}

#اگر defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		retval = fchflags (fd, UF_NODUMP|UF_IMMUTABLE);
#وگرنه
#اگر HAVE_EXT2_IOCTLS
		اگر (ioctl(fd, EXT2_IOC_GETFLAGS, &f) < 0) {
			retval = errno;
			برو errout;
		}
		f |= EXT2_NODUMP_FL | EXT2_IMMUTABLE_FL;
		retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
#endif
#endif
		اگر (retval) {
			retval = errno;
			برو errout;
		}

		اگر (close(fd) < 0) {
			retval = errno;
			fd = -1;
			برو errout;
		}
		journal_ino = st.st_ino;
		memset(fs->super->s_jnl_blocks, 0,
		       اندازه(fs->super->s_jnl_blocks));
	} وگرنه {
		اگر ((mount_flags & EXT2_MF_BUSY) &&
		    !(fs->flags & EXT2_FLAG_EXCLUSIVE)) {
			retval = EBUSY;
			برو errout;
		}
		journal_ino = EXT2_JOURNAL_INO;
		اگر ((retval = write_journal_inode(fs, journal_ino,
						  jparams, goal, flags)))
			بازگشت retval;
	}

	fs->super->s_journal_inum = journal_ino;
	fs->super->s_journal_dev = 0;
	memset(fs->super->s_journal_uuid, 0,
	       اندازه(fs->super->s_journal_uuid));
	ext2fs_set_feature_journal(fs->super);

	ext2fs_mark_super_dirty(fs);
	بازگشت 0;
errout:
	اگر (fd >= 0)
		close(fd);
	بازگشت retval;
}

errcode_t ext2fs_add_journal_inode2(ext2_filsys fs, blk_t رقم_blocks,
				    blk64_t goal, صحیح flags)
{
	ساختار ext2fs_journal_params jparams;

	jparams.رقم_journal_blocks = رقم_blocks;
	jparams.رقم_fc_blocks = 0;

	بازگشت ext2fs_add_journal_inode3(fs, &jparams, goal, flags);
}

errcode_t ext2fs_add_journal_inode(ext2_filsys fs, blk_t رقم_blocks, صحیح flags)
{
	بازگشت ext2fs_add_journal_inode2(fs, رقم_blocks, ~0ULL, flags);
}


#ifdef DEBUG
اصلی(صحیح argc, نویسه **argv)
{
	errcode_t	retval;
	نویسه		*device_name;
	ext2_filsys	fs;

	اگر (argc < 2) {
		fprintf(stderr, "Usage: %s filesystem\n", argv[0]);
		exit(1);
	}
	device_name = argv[1];

	retval = ext2fs_open (device_name, EXT2_FLAG_RW, 0, 0,
			      unix_io_manager, &fs);
	اگر (retval) {
		com_err(argv[0], retval, "while opening %s", device_name);
		exit(1);
	}

	retval = ext2fs_add_journal_inode(fs, JBD2_MIN_JOURNAL_BLOCKS, 0);
	اگر (retval) {
		com_err(argv[0], retval, "while adding journal to %s",
			device_name);
		exit(1);
	}
	retval = ext2fs_flush(fs);
	اگر (retval) {
		printf("Warning, had trouble writing out superblocks.\n");
	}
	ext2fs_close_free(&fs);
	exit(0);

}
#endif
