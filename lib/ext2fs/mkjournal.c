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
#ܐܢ HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ܐܢ HAVE_ERRNO_H
#ضمن <errno.h>
#endif
#ضمن <fcntl.h>
#ܐܢ HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#ܐܢ HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif
#ܐܢ HAVE_SYS_IOCTL_H
#ضمن <sys/ioctl.h>
#endif
#ܐܢ HAVE_NETINET_IN_H
#ضمن <netinet/in.h>
#endif

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

	ܐܢ (jparams->num_journal_blocks < JBD2_MIN_JOURNAL_BLOCKS)
		ܦܢܐ EXT2_ET_JOURNAL_TOO_SMALL;

	ܐܢ ((retval = ext2fs_get_mem(fs->blocksize, &jsb)))
		ܦܢܐ retval;

	memset (jsb, 0, fs->blocksize);

	jsb->s_header.h_magic = htonl(JBD2_MAGIC_NUMBER);
	ܐܢ (flags & EXT2_MKJOURNAL_V1_SUPER)
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V1);
	ܐܚܪܢܐ
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V2);
	jsb->s_blocksize = htonl(fs->blocksize);
	jsb->s_maxlen = htonl(jparams->num_journal_blocks + jparams->num_fc_blocks);
	jsb->s_nr_users = htonl(1);
	jsb->s_first = htonl(1);
	jsb->s_sequence = htonl(1);
	jsb->s_num_fc_blks = htonl(jparams->num_fc_blocks);
	memcpy(jsb->s_uuid, fs->super->s_uuid, حجم(fs->super->s_uuid));
	/*
	 * If we're creating an external journal device, we need to
	 * adjust these fields.
	 */
	ܐܢ (ext2fs_has_feature_journal_dev(fs->super)) {
		jsb->s_nr_users = 0;
		jsb->s_first = htonl(ext2fs_journal_sb_start(fs->blocksize) + 1);
	}

	*ret_jsb = (حرف *) jsb;
	ܦܢܐ 0;
}

errcode_t ext2fs_create_journal_superblock(ext2_filsys fs, __u32 num_blocks,
					صحيح flags, حرف **ret_sb)
{
	بنية ext2fs_journal_params jparams;

	jparams.num_journal_blocks = num_blocks;
	jparams.num_fc_blocks = 0;

	ܦܢܐ ext2fs_create_journal_superblock2(fs, &jparams, flags, ret_sb);
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
	صحيح		fd, ret_size;
	blk_t		i;

	ܐܢ ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		ܦܢܐ retval;

	/* Open the device or journal file */
	ܐܢ ((fd = open(filename, O_WRONLY)) < 0) {
		retval = errno;
		اذهب errfree;
	}

	/* Write the superblock out */
	retval = EXT2_ET_SHORT_WRITE;
	ret_size = اكتب(fd, buf, fs->blocksize);
	ܐܢ (ret_size < 0) {
		retval = errno;
		اذهب errout;
	}
	ܐܢ (ret_size != (صحيح) fs->blocksize)
		اذهب errout;
	memset(buf, 0, fs->blocksize);

	ܐܢ (flags & EXT2_MKJOURNAL_LAZYINIT)
		اذهب success;

	لكل (i = 1; i < jparams->num_journal_blocks + jparams->num_fc_blocks; i++) {
		ret_size = اكتب(fd, buf, fs->blocksize);
		ܐܢ (ret_size < 0) {
			retval = errno;
			اذهب errout;
		}
		ܐܢ (ret_size != (صحيح) fs->blocksize)
			اذهب errout;
	}

success:
	retval = 0;
errout:
	close(fd);
errfree:
	ext2fs_free_mem(&buf);
	ܦܢܐ retval;
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
#define MAX_STRIDE_LENGTH (4194304 / (صحيح) fs->blocksize)
errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, صحيح num,
			      blk64_t *ret_blk, صحيح *ret_count)
{
	صحيح		j, count;
	ساكن فراغ	*buf;
	ساكن صحيح	stride_length;
	errcode_t	retval;

	/* If fs is null, clean up the static buffer and return */
	ܐܢ (!fs) {
		ܐܢ (buf) {
			حرر(buf);
			buf = 0;
			stride_length = 0;
		}
		ܦܢܐ 0;
	}

	/* Deal with zeroing less than 1 block */
	ܐܢ (num <= 0)
		ܦܢܐ 0;

	/* Try a zero out command, if supported */
	retval = io_channel_zeroout(fs->io, blk, num);
	ܐܢ (retval == 0)
		ܦܢܐ 0;

	/* Allocate the zeroizing buffer if necessary */
	ܐܢ (num > stride_length && stride_length < MAX_STRIDE_LENGTH) {
		فراغ *p;
		صحيح new_stride = num;

		ܐܢ (new_stride > MAX_STRIDE_LENGTH)
			new_stride = MAX_STRIDE_LENGTH;
		p = realloc(buf, fs->blocksize * new_stride);
		ܐܢ (!p)
			ܦܢܐ EXT2_ET_NO_MEMORY;
		buf = p;
		stride_length = new_stride;
		memset(buf, 0, fs->blocksize * stride_length);
	}
	/* OK, do the write loop */
	j=0;
	ܟܡܐ (j < num) {
		ܐܢ (blk % stride_length) {
			count = stride_length - (blk % stride_length);
			ܐܢ (count > (num - j))
				count = num - j;
		} ܐܚܪܢܐ {
			count = num - j;
			ܐܢ (count > stride_length)
				count = stride_length;
		}
		retval = io_channel_write_blk64(fs->io, blk, count, buf);
		ܐܢ (retval) {
			ܐܢ (ret_count)
				*ret_count = count;
			ܐܢ (ret_blk)
				*ret_blk = blk;
			ܦܢܐ retval;
		}
		j += count; blk += count;
	}
	ܦܢܐ 0;
}

errcode_t ext2fs_zero_blocks(ext2_filsys fs, blk_t blk, صحيح num,
			     blk_t *ret_blk, صحيح *ret_count)
{
	blk64_t ret_blk2;
	errcode_t retval;

	retval = ext2fs_zero_blocks2(fs, blk, num, &ret_blk2, ret_count);
	ܐܢ (retval)
		*ret_blk = (blk_t) ret_blk2;
	ܦܢܐ retval;
}

/*
 * Calculate the initial goal block to be roughly at the middle of the
 * filesystem.  Pick a group that has the largest number of free
 * blocks.
 */
ساكن blk64_t get_midpoint_journal_block(ext2_filsys fs)
{
	dgrp_t	group, start, end, i, log_flex;

	group = ext2fs_group_of_blk2(fs, (ext2fs_blocks_count(fs->super) -
					 fs->super->s_first_data_block) / 2);
	log_flex = 1U << fs->super->s_log_groups_per_flex;
	ܐܢ (fs->super->s_log_groups_per_flex && (group > log_flex)) {
		group = group & ~(log_flex - 1);
		ܟܡܐ ((group < fs->group_desc_count) &&
		       ext2fs_bg_free_blocks_count(fs, group) == 0)
			group++;
		ܐܢ (group == fs->group_desc_count)
			group = 0;
		start = group;
	} ܐܚܪܢܐ
		start = (group > 0) ? group-1 : group;
	end = ((group+1) < fs->group_desc_count) ? group+1 : group;
	group = start;
	لكل (i = start + 1; i <= end; i++)
		ܐܢ (ext2fs_bg_free_blocks_count(fs, i) >
		    ext2fs_bg_free_blocks_count(fs, group))
			group = i;
	ܦܢܐ ext2fs_group_first_block2(fs, group);
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
	unsigned long long	inode_size;
	صحيح			falloc_flags = EXT2_FALLOCATE_FORCE_INIT;
	blk64_t			zblk;
	time_t			now;

	ܐܢ ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		ܦܢܐ retval;

	ܐܢ ((retval = ext2fs_read_bitmaps(fs)))
		اذهب out2;

	ܐܢ ((retval = ext2fs_read_inode(fs, journal_ino, &inode)))
		اذهب out2;

	ܐܢ (inode.i_blocks > 0) {
		retval = EEXIST;
		اذهب out2;
	}

	ܐܢ (goal == ~0ULL)
		goal = get_midpoint_journal_block(fs);

	ܐܢ (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;

	ܐܢ (!(flags & EXT2_MKJOURNAL_LAZYINIT))
		falloc_flags |= EXT2_FALLOCATE_ZERO_BLOCKS;

	inode_size = (unsigned long long)fs->blocksize *
			(jparams->num_journal_blocks + jparams->num_fc_blocks);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	inode.i_links_count = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	retval = ext2fs_inode_size_set(fs, &inode, inode_size);
	ܐܢ (retval)
		اذهب out2;

	retval = ext2fs_fallocate(fs, falloc_flags, journal_ino,
				  &inode, goal, 0,
				  jparams->num_journal_blocks + jparams->num_fc_blocks);
	ܐܢ (retval)
		اذهب out2;

	ܐܢ ((retval = ext2fs_write_new_inode(fs, journal_ino, &inode)))
		اذهب out2;

	retval = ext2fs_bmap2(fs, journal_ino, &inode, عدم, 0, 0, عدم, &zblk);
	ܐܢ (retval)
		اذهب out2;

	retval = io_channel_write_blk64(fs->io, zblk, 1, buf);
	ܐܢ (retval)
		اذهب out2;

	memcpy(fs->super->s_jnl_blocks, inode.i_block, EXT2_N_BLOCKS*4);
	fs->super->s_jnl_blocks[15] = inode.i_size_high;
	fs->super->s_jnl_blocks[16] = inode.i_size;
	fs->super->s_jnl_backup_type = EXT3_JNL_BACKUP_BLOCKS;
	ext2fs_mark_super_dirty(fs);

out2:
	ext2fs_free_mem(&buf);
	ܦܢܐ retval;
}

/*
 * Find a reasonable journal file size (in blocks) given the number of blocks
 * in the filesystem.  For very small filesystems, it is not reasonable to
 * have a journal that fills more than half of the filesystem.
 *
 * n.b. comments assume 4k blocks
 */
صحيح ext2fs_default_journal_size(__u64 num_blocks)
{
	ܐܢ (num_blocks < 2048)
		ܦܢܐ -1;
	ܐܢ (num_blocks < 32768)		/* 128 MB */
		ܦܢܐ (1024);			/* 4 MB */
	ܐܢ (num_blocks < 256*1024)	/* 1 GB */
		ܦܢܐ (4096);			/* 16 MB */
	ܐܢ (num_blocks < 512*1024)	/* 2 GB */
		ܦܢܐ (8192);			/* 32 MB */
	ܐܢ (num_blocks < 4096*1024)	/* 16 GB */
		ܦܢܐ (16384);			/* 64 MB */
	ܐܢ (num_blocks < 8192*1024)	/* 32 GB */
		ܦܢܐ (32768);			/* 128 MB */
	ܐܢ (num_blocks < 16384*1024)	/* 64 GB */
		ܦܢܐ (65536);			/* 256 MB */
	ܐܢ (num_blocks < 32768*1024)	/* 128 GB */
		ܦܢܐ (131072);		/* 512 MB */
	ܦܢܐ 262144;				/* 1 GB */
}

errcode_t ext2fs_get_journal_params(بنية ext2fs_journal_params *params,
		ext2_filsys fs)
{
	blk_t total_blks;
	صحيح ret;

	memset(params, 0, حجم(*params));
	ܐܢ (ext2fs_has_feature_journal_dev(fs->super)) {
		total_blks = ext2fs_blocks_count(fs->super);
		ܐܢ (total_blks < JBD2_MIN_JOURNAL_BLOCKS)
			ܦܢܐ EXT2_ET_JOURNAL_TOO_SMALL;

		ܐܢ (!ext2fs_has_feature_fast_commit(fs->super)) {
			params->num_journal_blocks = total_blks;
			params->num_fc_blocks = 0;
			ܦܢܐ 0;
		}
		params->num_journal_blocks = ext2fs_blocks_count(fs->super) *
				EXT2_JOURNAL_TO_FC_BLKS_RATIO /
				(EXT2_JOURNAL_TO_FC_BLKS_RATIO + 1);
		ܐܢ (JBD2_MIN_JOURNAL_BLOCKS > params->num_journal_blocks)
			params->num_journal_blocks = JBD2_MIN_JOURNAL_BLOCKS;
		params->num_fc_blocks = total_blks - params->num_journal_blocks;
		ܦܢܐ 0;
	}

	ret = ext2fs_default_journal_size(ext2fs_blocks_count(fs->super));
	ܐܢ (ret < 0)
		ܦܢܐ EXT2_ET_JOURNAL_TOO_SMALL;

	params->num_journal_blocks = ret;
	ܐܢ (ext2fs_has_feature_fast_commit(fs->super))
		params->num_fc_blocks = params->num_journal_blocks /
			EXT2_JOURNAL_TO_FC_BLKS_RATIO;
	ܦܢܐ 0;
}

صحيح ext2fs_journal_sb_start(صحيح blocksize)
{
	ܐܢ (blocksize == EXT2_MIN_BLOCK_SIZE)
		ܦܢܐ 2;
	ܦܢܐ 1;
}

/*
 * This function adds a journal device to a filesystem
 */
errcode_t ext2fs_add_journal_device(ext2_filsys fs, ext2_filsys journal_dev)
{
	بنية stat	st;
	errcode_t	retval;
	حرف		buf[SUPERBLOCK_SIZE];
	journal_superblock_t	*jsb;
	صحيح		start;
	__u32		i, nr_users;

	/* Make sure the device exists and is a block device */
	ܐܢ (stat(journal_dev->device_name, &st) < 0)
		ܦܢܐ errno;

	ܐܢ (!S_ISBLK(st.st_mode))
		ܦܢܐ EXT2_ET_JOURNAL_NOT_BLOCK; /* Must be a block device */

	/* Get the journal superblock */
	start = ext2fs_journal_sb_start(journal_dev->blocksize);
	ܐܢ ((retval = io_channel_read_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE,
					    buf)))
		ܦܢܐ retval;

	jsb = (journal_superblock_t *) buf;
	ܐܢ ((jsb->s_header.h_magic != (unsigned) ntohl(JBD2_MAGIC_NUMBER)) ||
	    (jsb->s_header.h_blocktype != (unsigned) ntohl(JBD2_SUPERBLOCK_V2)))
		ܦܢܐ EXT2_ET_NO_JOURNAL_SB;

	ܐܢ (ntohl(jsb->s_blocksize) != (unsigned long) fs->blocksize)
		ܦܢܐ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	/* Check and see if this filesystem has already been added */
	nr_users = ntohl(jsb->s_nr_users);
	ܐܢ (nr_users > JBD2_USERS_MAX)
		ܦܢܐ EXT2_ET_CORRUPT_JOURNAL_SB;
	لكل (i=0; i < nr_users; i++) {
		ܐܢ (memcmp(fs->super->s_uuid,
			   &jsb->s_users[i*16], 16) == 0)
			اكسر;
	}
	ܐܢ (i >= nr_users) {
		memcpy(&jsb->s_users[nr_users*16],
		       fs->super->s_uuid, 16);
		jsb->s_nr_users = htonl(nr_users+1);
	}

	/* Writeback the journal superblock */
	ܐܢ ((retval = io_channel_write_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE, buf)))
		ܦܢܐ retval;

	fs->super->s_journal_inum = 0;
	fs->super->s_journal_dev = st.st_rdev;
	memcpy(fs->super->s_journal_uuid, jsb->s_uuid,
	       حجم(fs->super->s_journal_uuid));
	memset(fs->super->s_jnl_blocks, 0, حجم(fs->super->s_jnl_blocks));
	ext2fs_set_feature_journal(fs->super);
	ext2fs_mark_super_dirty(fs);
	ܦܢܐ 0;
}

/*
 * This function adds a journal inode to a filesystem, using either
 * POSIX routines if the filesystem is mounted, or using direct I/O
 * functions if it is not.
 */
errcode_t ext2fs_add_journal_inode3(ext2_filsys fs, بنية ext2fs_journal_params *jparams,
				    blk64_t goal, صحيح flags)
{
	errcode_t		retval;
	ext2_ino_t		journal_ino;
	بنية stat		st;
	حرف			jfile[1024];
	صحيح			mount_flags;
	صحيح			fd = -1;

	ܐܢ (flags & EXT2_MKJOURNAL_NO_MNT_CHECK)
		mount_flags = 0;
	ܐܚܪܢܐ ܐܢ ((retval = ext2fs_check_mount_point(fs->device_name,
						    &mount_flags,
						    jfile, حجم(jfile)-10)))
		ܦܢܐ retval;

	ܐܢ (mount_flags & EXT2_MF_MOUNTED) {
#ܐܢ HAVE_EXT2_IOCTLS
		صحيح f = 0;
#endif
		strcat(jfile, "/.journal");

		/*
		 * If .../.journal already exists, make sure any
		 * immutable or append-only flags are cleared.
		 */
#ܐܢ defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		(فراغ) chflags (jfile, 0);
#ܐܚܪܢܐ
#ܐܢ HAVE_EXT2_IOCTLS
		fd = open(jfile, O_RDONLY);
		ܐܢ (fd >= 0) {
			retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
			close(fd);
			ܐܢ (retval)
				ܦܢܐ errno;
		}
#endif
#endif

		/* Create the journal file */
		ܐܢ ((fd = open(jfile, O_CREAT|O_WRONLY, 0600)) < 0)
			ܦܢܐ errno;

		/* Note that we can't do lazy journal initialization for mounted
		 * filesystems, since the zero writing is also allocating the
		 * journal blocks.  We could use fallocate, but not all kernels
		 * support that, and creating a journal on a mounted ext2
		 * filesystems is extremely rare these days...  Ignore it. */
		flags &= ~EXT2_MKJOURNAL_LAZYINIT;

		ܐܢ ((retval = write_journal_file(fs, jfile, jparams, flags)))
			اذهب errout;

		/* Get inode number of the journal file */
		ܐܢ (fstat(fd, &st) < 0) {
			retval = errno;
			اذهب errout;
		}

#ܐܢ defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		retval = fchflags (fd, UF_NODUMP|UF_IMMUTABLE);
#ܐܚܪܢܐ
#ܐܢ HAVE_EXT2_IOCTLS
		ܐܢ (ioctl(fd, EXT2_IOC_GETFLAGS, &f) < 0) {
			retval = errno;
			اذهب errout;
		}
		f |= EXT2_NODUMP_FL | EXT2_IMMUTABLE_FL;
		retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
#endif
#endif
		ܐܢ (retval) {
			retval = errno;
			اذهب errout;
		}

		ܐܢ (close(fd) < 0) {
			retval = errno;
			fd = -1;
			اذهب errout;
		}
		journal_ino = st.st_ino;
		memset(fs->super->s_jnl_blocks, 0,
		       حجم(fs->super->s_jnl_blocks));
	} ܐܚܪܢܐ {
		ܐܢ ((mount_flags & EXT2_MF_BUSY) &&
		    !(fs->flags & EXT2_FLAG_EXCLUSIVE)) {
			retval = EBUSY;
			اذهب errout;
		}
		journal_ino = EXT2_JOURNAL_INO;
		ܐܢ ((retval = write_journal_inode(fs, journal_ino,
						  jparams, goal, flags)))
			ܦܢܐ retval;
	}

	fs->super->s_journal_inum = journal_ino;
	fs->super->s_journal_dev = 0;
	memset(fs->super->s_journal_uuid, 0,
	       حجم(fs->super->s_journal_uuid));
	ext2fs_set_feature_journal(fs->super);

	ext2fs_mark_super_dirty(fs);
	ܦܢܐ 0;
errout:
	ܐܢ (fd >= 0)
		close(fd);
	ܦܢܐ retval;
}

errcode_t ext2fs_add_journal_inode2(ext2_filsys fs, blk_t num_blocks,
				    blk64_t goal, صحيح flags)
{
	بنية ext2fs_journal_params jparams;

	jparams.num_journal_blocks = num_blocks;
	jparams.num_fc_blocks = 0;

	ܦܢܐ ext2fs_add_journal_inode3(fs, &jparams, goal, flags);
}

errcode_t ext2fs_add_journal_inode(ext2_filsys fs, blk_t num_blocks, صحيح flags)
{
	ܦܢܐ ext2fs_add_journal_inode2(fs, num_blocks, ~0ULL, flags);
}


#ifdef DEBUG
main(صحيح argc, حرف **argv)
{
	errcode_t	retval;
	حرف		*device_name;
	ext2_filsys	fs;

	ܐܢ (argc < 2) {
		fprintf(stderr, "Usage: %s filesystem\n", argv[0]);
		exit(1);
	}
	device_name = argv[1];

	retval = ext2fs_open (device_name, EXT2_FLAG_RW, 0, 0,
			      unix_io_manager, &fs);
	ܐܢ (retval) {
		com_err(argv[0], retval, "while opening %s", device_name);
		exit(1);
	}

	retval = ext2fs_add_journal_inode(fs, JBD2_MIN_JOURNAL_BLOCKS, 0);
	ܐܢ (retval) {
		com_err(argv[0], retval, "while adding journal to %s",
			device_name);
		exit(1);
	}
	retval = ext2fs_flush(fs);
	ܐܢ (retval) {
		printf("Warning, had trouble writing out superblocks.\n");
	}
	ext2fs_close_free(&fs);
	exit(0);

}
#endif
