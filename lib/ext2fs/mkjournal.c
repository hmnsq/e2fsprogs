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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <assert.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#যদি HAVE_ERRNO_H
#অন্তর্ভাৱ <errno.h>
#endif
#অন্তর্ভাৱ <fcntl.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাৱ <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাৱ <sys/types.h>
#endif
#যদি HAVE_SYS_IOCTL_H
#অন্তর্ভাৱ <sys/ioctl.h>
#endif
#যদি HAVE_NETINET_IN_H
#অন্তর্ভাৱ <netinet/in.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "e2p/e2p.h"
#অন্তর্ভাৱ "ext2fsP.h"

#অন্তর্ভাৱ "kernel-jbd.h"

/*
 * This function automatically sets up the journal superblock and
 * returns it as an allocated block.
 */
errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
					   সংরচনা ext2fs_journal_params *jparams,
					   পূর্ণাঙ্ক flags, ৱর্ণ **ret_jsb)
{
	errcode_t		retval;
	journal_superblock_t	*jsb;

	যদি (jparams->অঙ্ক_journal_blocks < JBD2_MIN_JOURNAL_BLOCKS)
		ঘূৰাই EXT2_ET_JOURNAL_TOO_SMALL;

	যদি ((retval = ext2fs_get_mem(fs->blocksize, &jsb)))
		ঘূৰাই retval;

	memset (jsb, 0, fs->blocksize);

	jsb->s_header.h_magic = htonl(JBD2_MAGIC_সংখ্যা);
	যদি (flags & EXT2_MKJOURNAL_V1_SUPER)
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V1);
	অন্যথা
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V2);
	jsb->s_blocksize = htonl(fs->blocksize);
	jsb->s_maxlen = htonl(jparams->অঙ্ক_journal_blocks + jparams->অঙ্ক_fc_blocks);
	jsb->s_nr_users = htonl(1);
	jsb->s_first = htonl(1);
	jsb->s_sequence = htonl(1);
	jsb->s_অঙ্ক_fc_blks = htonl(jparams->অঙ্ক_fc_blocks);
	memcpy(jsb->s_uuid, fs->super->s_uuid, পরিমাণ(fs->super->s_uuid));
	/*
	 * If we're creating an external journal device, we need to
	 * adjust these fields.
	 */
	যদি (ext2fs_has_feature_journal_dev(fs->super)) {
		jsb->s_nr_users = 0;
		jsb->s_first = htonl(ext2fs_journal_sb_start(fs->blocksize) + 1);
	}

	*ret_jsb = (ৱর্ণ *) jsb;
	ঘূৰাই 0;
}

errcode_t ext2fs_create_journal_superblock(ext2_filsys fs, __u32 অঙ্ক_blocks,
					পূর্ণাঙ্ক flags, ৱর্ণ **ret_sb)
{
	সংরচনা ext2fs_journal_params jparams;

	jparams.অঙ্ক_journal_blocks = অঙ্ক_blocks;
	jparams.অঙ্ক_fc_blocks = 0;

	ঘূৰাই ext2fs_create_journal_superblock2(fs, &jparams, flags, ret_sb);
}

/*
 * This function writes a journal using POSIX routines.  It is used
 * for creating external journals and creating journals on live
 * filesystems.
 */
স্থির errcode_t write_journal_file(ext2_filsys fs, ৱর্ণ *filename,
				    সংরচনা ext2fs_journal_params *jparams,
				    পূর্ণাঙ্ক flags)
{
	errcode_t	retval;
	ৱর্ণ		*buf = 0;
	পূর্ণাঙ্ক		fd, ret_size;
	blk_t		i;

	যদি ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		ঘূৰাই retval;

	/* Open the device or journal file */
	যদি ((fd = open(filename, O_WRONLY)) < 0) {
		retval = errno;
		গচ্ছ errfree;
	}

	/* Write the superblock out */
	retval = EXT2_ET_SHORT_WRITE;
	ret_size = লিখ(fd, buf, fs->blocksize);
	যদি (ret_size < 0) {
		retval = errno;
		গচ্ছ errout;
	}
	যদি (ret_size != (পূর্ণাঙ্ক) fs->blocksize)
		গচ্ছ errout;
	memset(buf, 0, fs->blocksize);

	যদি (flags & EXT2_MKJOURNAL_LAZYINIT)
		গচ্ছ success;

	বাবে (i = 1; i < jparams->অঙ্ক_journal_blocks + jparams->অঙ্ক_fc_blocks; i++) {
		ret_size = লিখ(fd, buf, fs->blocksize);
		যদি (ret_size < 0) {
			retval = errno;
			গচ্ছ errout;
		}
		যদি (ret_size != (পূর্ণাঙ্ক) fs->blocksize)
			গচ্ছ errout;
	}

success:
	retval = 0;
errout:
	close(fd);
errfree:
	ext2fs_free_mem(&buf);
	ঘূৰাই retval;
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
#define MAX_STRIDE_LENGTH (4194304 / (পূর্ণাঙ্ক) fs->blocksize)
errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, পূর্ণাঙ্ক অঙ্ক,
			      blk64_t *ret_blk, পূর্ণাঙ্ক *ret_গুনতি)
{
	পূর্ণাঙ্ক		j, গুনতি;
	স্থির শূন্য	*buf;
	স্থির পূর্ণাঙ্ক	stride_length;
	errcode_t	retval;

	/* If fs is null, clean up the static buffer and return */
	যদি (!fs) {
		যদি (buf) {
			মুক্ত(buf);
			buf = 0;
			stride_length = 0;
		}
		ঘূৰাই 0;
	}

	/* Deal with zeroing less than 1 block */
	যদি (অঙ্ক <= 0)
		ঘূৰাই 0;

	/* Try a zero out command, if supported */
	retval = io_channel_zeroout(fs->io, blk, অঙ্ক);
	যদি (retval == 0)
		ঘূৰাই 0;

	/* Allocate the zeroizing buffer if necessary */
	যদি (অঙ্ক > stride_length && stride_length < MAX_STRIDE_LENGTH) {
		শূন্য *p;
		পূর্ণাঙ্ক new_stride = অঙ্ক;

		যদি (new_stride > MAX_STRIDE_LENGTH)
			new_stride = MAX_STRIDE_LENGTH;
		p = realloc(buf, fs->blocksize * new_stride);
		যদি (!p)
			ঘূৰাই EXT2_ET_NO_MEMORY;
		buf = p;
		stride_length = new_stride;
		memset(buf, 0, fs->blocksize * stride_length);
	}
	/* OK, do the write loop */
	j=0;
	যেতিয়ালৈ (j < অঙ্ক) {
		যদি (blk % stride_length) {
			গুনতি = stride_length - (blk % stride_length);
			যদি (গুনতি > (অঙ্ক - j))
				গুনতি = অঙ্ক - j;
		} অন্যথা {
			গুনতি = অঙ্ক - j;
			যদি (গুনতি > stride_length)
				গুনতি = stride_length;
		}
		retval = io_channel_write_blk64(fs->io, blk, গুনতি, buf);
		যদি (retval) {
			যদি (ret_গুনতি)
				*ret_গুনতি = গুনতি;
			যদি (ret_blk)
				*ret_blk = blk;
			ঘূৰাই retval;
		}
		j += গুনতি; blk += গুনতি;
	}
	ঘূৰাই 0;
}

errcode_t ext2fs_zero_blocks(ext2_filsys fs, blk_t blk, পূর্ণাঙ্ক অঙ্ক,
			     blk_t *ret_blk, পূর্ণাঙ্ক *ret_গুনতি)
{
	blk64_t ret_blk2;
	errcode_t retval;

	retval = ext2fs_zero_blocks2(fs, blk, অঙ্ক, &ret_blk2, ret_গুনতি);
	যদি (retval)
		*ret_blk = (blk_t) ret_blk2;
	ঘূৰাই retval;
}

/*
 * Calculate the initial goal block to be roughly at the middle of the
 * filesystem.  Pick a group that has the largest number of free
 * blocks.
 */
স্থির blk64_t get_midpoint_journal_block(ext2_filsys fs)
{
	dgrp_t	group, start, end, i, log_flex;

	group = ext2fs_group_of_blk2(fs, (ext2fs_blocks_গুনতি(fs->super) -
					 fs->super->s_first_data_block) / 2);
	log_flex = 1U << fs->super->s_log_groups_per_flex;
	যদি (fs->super->s_log_groups_per_flex && (group > log_flex)) {
		group = group & ~(log_flex - 1);
		যেতিয়ালৈ ((group < fs->group_desc_গুনতি) &&
		       ext2fs_bg_free_blocks_গুনতি(fs, group) == 0)
			group++;
		যদি (group == fs->group_desc_গুনতি)
			group = 0;
		start = group;
	} অন্যথা
		start = (group > 0) ? group-1 : group;
	end = ((group+1) < fs->group_desc_গুনতি) ? group+1 : group;
	group = start;
	বাবে (i = start + 1; i <= end; i++)
		যদি (ext2fs_bg_free_blocks_গুনতি(fs, i) >
		    ext2fs_bg_free_blocks_গুনতি(fs, group))
			group = i;
	ঘূৰাই ext2fs_group_first_block2(fs, group);
}

/*
 * This function creates a journal using direct I/O routines.
 */
স্থির errcode_t write_journal_inode(ext2_filsys fs, ext2_ino_t journal_ino,
				     সংরচনা ext2fs_journal_params *jparams,
				     blk64_t goal, পূর্ণাঙ্ক flags)
{
	ৱর্ণ			*buf;
	errcode_t		retval;
	সংরচনা ext2_inode	inode;
	unsigned long long	inode_size;
	পূর্ণাঙ্ক			falloc_flags = EXT2_FALLOCATE_FORCE_INIT;
	blk64_t			zblk;
	time_t			now;

	যদি ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		ঘূৰাই retval;

	যদি ((retval = ext2fs_read_bitmaps(fs)))
		গচ্ছ out2;

	যদি ((retval = ext2fs_read_inode(fs, journal_ino, &inode)))
		গচ্ছ out2;

	যদি (inode.i_blocks > 0) {
		retval = EEXIST;
		গচ্ছ out2;
	}

	যদি (goal == ~0ULL)
		goal = get_midpoint_journal_block(fs);

	যদি (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;

	যদি (!(flags & EXT2_MKJOURNAL_LAZYINIT))
		falloc_flags |= EXT2_FALLOCATE_ZERO_BLOCKS;

	inode_size = (unsigned long long)fs->blocksize *
			(jparams->অঙ্ক_journal_blocks + jparams->অঙ্ক_fc_blocks);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	inode.i_links_গুনতি = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	retval = ext2fs_inode_size_set(fs, &inode, inode_size);
	যদি (retval)
		গচ্ছ out2;

	retval = ext2fs_fallocate(fs, falloc_flags, journal_ino,
				  &inode, goal, 0,
				  jparams->অঙ্ক_journal_blocks + jparams->অঙ্ক_fc_blocks);
	যদি (retval)
		গচ্ছ out2;

	যদি ((retval = ext2fs_write_new_inode(fs, journal_ino, &inode)))
		গচ্ছ out2;

	retval = ext2fs_bmap2(fs, journal_ino, &inode, রিক্ত, 0, 0, রিক্ত, &zblk);
	যদি (retval)
		গচ্ছ out2;

	retval = io_channel_write_blk64(fs->io, zblk, 1, buf);
	যদি (retval)
		গচ্ছ out2;

	memcpy(fs->super->s_jnl_blocks, inode.i_block, EXT2_N_BLOCKS*4);
	fs->super->s_jnl_blocks[15] = inode.i_size_উপৰেৰ;
	fs->super->s_jnl_blocks[16] = inode.i_size;
	fs->super->s_jnl_backup_type = EXT3_JNL_BACKUP_BLOCKS;
	ext2fs_mark_super_dirty(fs);

out2:
	ext2fs_free_mem(&buf);
	ঘূৰাই retval;
}

/*
 * Find a reasonable journal file size (in blocks) given the number of blocks
 * in the filesystem.  For very small filesystems, it is not reasonable to
 * have a journal that fills more than half of the filesystem.
 *
 * n.b. comments assume 4k blocks
 */
পূর্ণাঙ্ক ext2fs_default_journal_size(__u64 অঙ্ক_blocks)
{
	যদি (অঙ্ক_blocks < 2048)
		ঘূৰাই -1;
	যদি (অঙ্ক_blocks < 32768)		/* 128 MB */
		ঘূৰাই (1024);			/* 4 MB */
	যদি (অঙ্ক_blocks < 256*1024)	/* 1 GB */
		ঘূৰাই (4096);			/* 16 MB */
	যদি (অঙ্ক_blocks < 512*1024)	/* 2 GB */
		ঘূৰাই (8192);			/* 32 MB */
	যদি (অঙ্ক_blocks < 4096*1024)	/* 16 GB */
		ঘূৰাই (16384);			/* 64 MB */
	যদি (অঙ্ক_blocks < 8192*1024)	/* 32 GB */
		ঘূৰাই (32768);			/* 128 MB */
	যদি (অঙ্ক_blocks < 16384*1024)	/* 64 GB */
		ঘূৰাই (65536);			/* 256 MB */
	যদি (অঙ্ক_blocks < 32768*1024)	/* 128 GB */
		ঘূৰাই (131072);		/* 512 MB */
	ঘূৰাই 262144;				/* 1 GB */
}

errcode_t ext2fs_get_journal_params(সংরচনা ext2fs_journal_params *params,
		ext2_filsys fs)
{
	blk_t total_blks;
	পূর্ণাঙ্ক ret;

	memset(params, 0, পরিমাণ(*params));
	যদি (ext2fs_has_feature_journal_dev(fs->super)) {
		total_blks = ext2fs_blocks_গুনতি(fs->super);
		যদি (total_blks < JBD2_MIN_JOURNAL_BLOCKS)
			ঘূৰাই EXT2_ET_JOURNAL_TOO_SMALL;

		যদি (!ext2fs_has_feature_fast_commit(fs->super)) {
			params->অঙ্ক_journal_blocks = total_blks;
			params->অঙ্ক_fc_blocks = 0;
			ঘূৰাই 0;
		}
		params->অঙ্ক_journal_blocks = ext2fs_blocks_গুনতি(fs->super) *
				EXT2_JOURNAL_TO_FC_BLKS_RATIO /
				(EXT2_JOURNAL_TO_FC_BLKS_RATIO + 1);
		যদি (JBD2_MIN_JOURNAL_BLOCKS > params->অঙ্ক_journal_blocks)
			params->অঙ্ক_journal_blocks = JBD2_MIN_JOURNAL_BLOCKS;
		params->অঙ্ক_fc_blocks = total_blks - params->অঙ্ক_journal_blocks;
		ঘূৰাই 0;
	}

	ret = ext2fs_default_journal_size(ext2fs_blocks_গুনতি(fs->super));
	যদি (ret < 0)
		ঘূৰাই EXT2_ET_JOURNAL_TOO_SMALL;

	params->অঙ্ক_journal_blocks = ret;
	যদি (ext2fs_has_feature_fast_commit(fs->super))
		params->অঙ্ক_fc_blocks = params->অঙ্ক_journal_blocks /
			EXT2_JOURNAL_TO_FC_BLKS_RATIO;
	ঘূৰাই 0;
}

পূর্ণাঙ্ক ext2fs_journal_sb_start(পূর্ণাঙ্ক blocksize)
{
	যদি (blocksize == EXT2_MIN_BLOCK_SIZE)
		ঘূৰাই 2;
	ঘূৰাই 1;
}

/*
 * This function adds a journal device to a filesystem
 */
errcode_t ext2fs_add_journal_device(ext2_filsys fs, ext2_filsys journal_dev)
{
	সংরচনা stat	st;
	errcode_t	retval;
	ৱর্ণ		buf[SUPERBLOCK_SIZE];
	journal_superblock_t	*jsb;
	পূর্ণাঙ্ক		start;
	__u32		i, nr_users;

	/* Make sure the device exists and is a block device */
	যদি (stat(journal_dev->device_name, &st) < 0)
		ঘূৰাই errno;

	যদি (!S_ISBLK(st.st_mode))
		ঘূৰাই EXT2_ET_JOURNAL_NOT_BLOCK; /* Must be a block device */

	/* Get the journal superblock */
	start = ext2fs_journal_sb_start(journal_dev->blocksize);
	যদি ((retval = io_channel_read_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE,
					    buf)))
		ঘূৰাই retval;

	jsb = (journal_superblock_t *) buf;
	যদি ((jsb->s_header.h_magic != (unsigned) ntohl(JBD2_MAGIC_সংখ্যা)) ||
	    (jsb->s_header.h_blocktype != (unsigned) ntohl(JBD2_SUPERBLOCK_V2)))
		ঘূৰাই EXT2_ET_NO_JOURNAL_SB;

	যদি (ntohl(jsb->s_blocksize) != (unsigned long) fs->blocksize)
		ঘূৰাই EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	/* Check and see if this filesystem has already been added */
	nr_users = ntohl(jsb->s_nr_users);
	যদি (nr_users > JBD2_USERS_MAX)
		ঘূৰাই EXT2_ET_CORRUPT_JOURNAL_SB;
	বাবে (i=0; i < nr_users; i++) {
		যদি (memcmp(fs->super->s_uuid,
			   &jsb->s_users[i*16], 16) == 0)
			ভঙ্গ;
	}
	যদি (i >= nr_users) {
		memcpy(&jsb->s_users[nr_users*16],
		       fs->super->s_uuid, 16);
		jsb->s_nr_users = htonl(nr_users+1);
	}

	/* Writeback the journal superblock */
	যদি ((retval = io_channel_write_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE, buf)))
		ঘূৰাই retval;

	fs->super->s_journal_inum = 0;
	fs->super->s_journal_dev = st.st_rdev;
	memcpy(fs->super->s_journal_uuid, jsb->s_uuid,
	       পরিমাণ(fs->super->s_journal_uuid));
	memset(fs->super->s_jnl_blocks, 0, পরিমাণ(fs->super->s_jnl_blocks));
	ext2fs_set_feature_journal(fs->super);
	ext2fs_mark_super_dirty(fs);
	ঘূৰাই 0;
}

/*
 * This function adds a journal inode to a filesystem, using either
 * POSIX routines if the filesystem is mounted, or using direct I/O
 * functions if it is not.
 */
errcode_t ext2fs_add_journal_inode3(ext2_filsys fs, সংরচনা ext2fs_journal_params *jparams,
				    blk64_t goal, পূর্ণাঙ্ক flags)
{
	errcode_t		retval;
	ext2_ino_t		journal_ino;
	সংরচনা stat		st;
	ৱর্ণ			jfile[1024];
	পূর্ণাঙ্ক			mount_flags;
	পূর্ণাঙ্ক			fd = -1;

	যদি (flags & EXT2_MKJOURNAL_NO_MNT_CHECK)
		mount_flags = 0;
	অন্যথা যদি ((retval = ext2fs_check_mount_point(fs->device_name,
						    &mount_flags,
						    jfile, পরিমাণ(jfile)-10)))
		ঘূৰাই retval;

	যদি (mount_flags & EXT2_MF_MOUNTED) {
#যদি HAVE_EXT2_IOCTLS
		পূর্ণাঙ্ক f = 0;
#endif
		strcat(jfile, "/.journal");

		/*
		 * If .../.journal already exists, make sure any
		 * immutable or append-only flags are cleared.
		 */
#যদি defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		(শূন্য) chflags (jfile, 0);
#অন্যথা
#যদি HAVE_EXT2_IOCTLS
		fd = open(jfile, O_RDONLY);
		যদি (fd >= 0) {
			retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
			close(fd);
			যদি (retval)
				ঘূৰাই errno;
		}
#endif
#endif

		/* Create the journal file */
		যদি ((fd = open(jfile, O_CREAT|O_WRONLY, 0600)) < 0)
			ঘূৰাই errno;

		/* Note that we can't do lazy journal initialization for mounted
		 * filesystems, since the zero writing is also allocating the
		 * journal blocks.  We could use fallocate, but not all kernels
		 * support that, and creating a journal on a mounted ext2
		 * filesystems is extremely rare these days...  Ignore it. */
		flags &= ~EXT2_MKJOURNAL_LAZYINIT;

		যদি ((retval = write_journal_file(fs, jfile, jparams, flags)))
			গচ্ছ errout;

		/* Get inode number of the journal file */
		যদি (fstat(fd, &st) < 0) {
			retval = errno;
			গচ্ছ errout;
		}

#যদি defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		retval = fchflags (fd, UF_NODUMP|UF_IMMUTABLE);
#অন্যথা
#যদি HAVE_EXT2_IOCTLS
		যদি (ioctl(fd, EXT2_IOC_GETFLAGS, &f) < 0) {
			retval = errno;
			গচ্ছ errout;
		}
		f |= EXT2_NODUMP_FL | EXT2_IMMUTABLE_FL;
		retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
#endif
#endif
		যদি (retval) {
			retval = errno;
			গচ্ছ errout;
		}

		যদি (close(fd) < 0) {
			retval = errno;
			fd = -1;
			গচ্ছ errout;
		}
		journal_ino = st.st_ino;
		memset(fs->super->s_jnl_blocks, 0,
		       পরিমাণ(fs->super->s_jnl_blocks));
	} অন্যথা {
		যদি ((mount_flags & EXT2_MF_BUSY) &&
		    !(fs->flags & EXT2_FLAG_EXCLUSIVE)) {
			retval = EBUSY;
			গচ্ছ errout;
		}
		journal_ino = EXT2_JOURNAL_INO;
		যদি ((retval = write_journal_inode(fs, journal_ino,
						  jparams, goal, flags)))
			ঘূৰাই retval;
	}

	fs->super->s_journal_inum = journal_ino;
	fs->super->s_journal_dev = 0;
	memset(fs->super->s_journal_uuid, 0,
	       পরিমাণ(fs->super->s_journal_uuid));
	ext2fs_set_feature_journal(fs->super);

	ext2fs_mark_super_dirty(fs);
	ঘূৰাই 0;
errout:
	যদি (fd >= 0)
		close(fd);
	ঘূৰাই retval;
}

errcode_t ext2fs_add_journal_inode2(ext2_filsys fs, blk_t অঙ্ক_blocks,
				    blk64_t goal, পূর্ণাঙ্ক flags)
{
	সংরচনা ext2fs_journal_params jparams;

	jparams.অঙ্ক_journal_blocks = অঙ্ক_blocks;
	jparams.অঙ্ক_fc_blocks = 0;

	ঘূৰাই ext2fs_add_journal_inode3(fs, &jparams, goal, flags);
}

errcode_t ext2fs_add_journal_inode(ext2_filsys fs, blk_t অঙ্ক_blocks, পূর্ণাঙ্ক flags)
{
	ঘূৰাই ext2fs_add_journal_inode2(fs, অঙ্ক_blocks, ~0ULL, flags);
}


#ifdef DEBUG
main(পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
	errcode_t	retval;
	ৱর্ণ		*device_name;
	ext2_filsys	fs;

	যদি (argc < 2) {
		fprintf(stderr, "Usage: %s filesystem\n", argv[0]);
		exit(1);
	}
	device_name = argv[1];

	retval = ext2fs_open (device_name, EXT2_FLAG_RW, 0, 0,
			      unix_io_manager, &fs);
	যদি (retval) {
		com_err(argv[0], retval, "while opening %s", device_name);
		exit(1);
	}

	retval = ext2fs_add_journal_inode(fs, JBD2_MIN_JOURNAL_BLOCKS, 0);
	যদি (retval) {
		com_err(argv[0], retval, "while adding journal to %s",
			device_name);
		exit(1);
	}
	retval = ext2fs_flush(fs);
	যদি (retval) {
		printf("Warning, had trouble writing out superblocks.\n");
	}
	ext2fs_close_free(&fs);
	exit(0);

}
#endif
