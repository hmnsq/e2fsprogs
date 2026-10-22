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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <assert.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#എങ്കിൽ HAVE_ERRNO_H
#അന്തര്ഭാവ <errno.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif
#എങ്കിൽ HAVE_SYS_IOCTL_H
#അന്തര്ഭാവ <sys/ioctl.h>
#endif
#എങ്കിൽ HAVE_NETINET_IN_H
#അന്തര്ഭാവ <netinet/in.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "e2p/e2p.h"
#അന്തര്ഭാവ "ext2fsP.h"

#അന്തര്ഭാവ "kernel-jbd.h"

/*
 * This function automatically sets up the journal superblock and
 * returns it as an allocated block.
 */
errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
					   ഘടന ext2fs_journal_params *jparams,
					   പൂർണ്ണസംഖ്യ flags, അക്ഷരം **ret_jsb)
{
	errcode_t		retval;
	journal_superblock_t	*jsb;

	എങ്കിൽ (jparams->അക്കം_journal_blocks < JBD2_MIN_JOURNAL_BLOCKS)
		തിരികെ EXT2_ET_JOURNAL_TOO_SMALL;

	എങ്കിൽ ((retval = ext2fs_get_mem(fs->blocksize, &jsb)))
		തിരികെ retval;

	memset (jsb, 0, fs->blocksize);

	jsb->s_header.h_magic = htonl(JBD2_MAGIC_സംഖ്യ);
	എങ്കിൽ (flags & EXT2_MKJOURNAL_V1_SUPER)
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V1);
	അല്ലെങ്കിൽ
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V2);
	jsb->s_blocksize = htonl(fs->blocksize);
	jsb->s_maxlen = htonl(jparams->അക്കം_journal_blocks + jparams->അക്കം_fc_blocks);
	jsb->s_nr_users = htonl(1);
	jsb->s_first = htonl(1);
	jsb->s_sequence = htonl(1);
	jsb->s_അക്കം_fc_blks = htonl(jparams->അക്കം_fc_blocks);
	memcpy(jsb->s_uuid, fs->super->s_uuid, വലുപ്പം(fs->super->s_uuid));
	/*
	 * If we're creating an external journal device, we need to
	 * adjust these fields.
	 */
	എങ്കിൽ (ext2fs_has_feature_journal_dev(fs->super)) {
		jsb->s_nr_users = 0;
		jsb->s_first = htonl(ext2fs_journal_sb_start(fs->blocksize) + 1);
	}

	*ret_jsb = (അക്ഷരം *) jsb;
	തിരികെ 0;
}

errcode_t ext2fs_create_journal_superblock(ext2_filsys fs, __u32 അക്കം_blocks,
					പൂർണ്ണസംഖ്യ flags, അക്ഷരം **ret_sb)
{
	ഘടന ext2fs_journal_params jparams;

	jparams.അക്കം_journal_blocks = അക്കം_blocks;
	jparams.അക്കം_fc_blocks = 0;

	തിരികെ ext2fs_create_journal_superblock2(fs, &jparams, flags, ret_sb);
}

/*
 * This function writes a journal using POSIX routines.  It is used
 * for creating external journals and creating journals on live
 * filesystems.
 */
സ്ഥിതം errcode_t write_journal_file(ext2_filsys fs, അക്ഷരം *filename,
				    ഘടന ext2fs_journal_params *jparams,
				    പൂർണ്ണസംഖ്യ flags)
{
	errcode_t	retval;
	അക്ഷരം		*buf = 0;
	പൂർണ്ണസംഖ്യ		fd, ret_size;
	blk_t		i;

	എങ്കിൽ ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		തിരികെ retval;

	/* Open the device or journal file */
	എങ്കിൽ ((fd = open(filename, O_WRONLY)) < 0) {
		retval = errno;
		പോകുക errfree;
	}

	/* Write the superblock out */
	retval = EXT2_ET_SHORT_WRITE;
	ret_size = ലിഖ(fd, buf, fs->blocksize);
	എങ്കിൽ (ret_size < 0) {
		retval = errno;
		പോകുക errout;
	}
	എങ്കിൽ (ret_size != (പൂർണ്ണസംഖ്യ) fs->blocksize)
		പോകുക errout;
	memset(buf, 0, fs->blocksize);

	എങ്കിൽ (flags & EXT2_MKJOURNAL_LAZYINIT)
		പോകുക success;

	വേണ്ടി (i = 1; i < jparams->അക്കം_journal_blocks + jparams->അക്കം_fc_blocks; i++) {
		ret_size = ലിഖ(fd, buf, fs->blocksize);
		എങ്കിൽ (ret_size < 0) {
			retval = errno;
			പോകുക errout;
		}
		എങ്കിൽ (ret_size != (പൂർണ്ണസംഖ്യ) fs->blocksize)
			പോകുക errout;
	}

success:
	retval = 0;
errout:
	close(fd);
errfree:
	ext2fs_free_mem(&buf);
	തിരികെ retval;
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
#define MAX_STRIDE_LENGTH (4194304 / (പൂർണ്ണസംഖ്യ) fs->blocksize)
errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, പൂർണ്ണസംഖ്യ അക്കം,
			      blk64_t *ret_blk, പൂർണ്ണസംഖ്യ *ret_എണ്ണം)
{
	പൂർണ്ണസംഖ്യ		j, എണ്ണം;
	സ്ഥിതം ശൂന്യം	*buf;
	സ്ഥിതം പൂർണ്ണസംഖ്യ	stride_length;
	errcode_t	retval;

	/* If fs is null, clean up the static buffer and return */
	എങ്കിൽ (!fs) {
		എങ്കിൽ (buf) {
			സ്വതന്ത്രം(buf);
			buf = 0;
			stride_length = 0;
		}
		തിരികെ 0;
	}

	/* Deal with zeroing less than 1 block */
	എങ്കിൽ (അക്കം <= 0)
		തിരികെ 0;

	/* Try a zero out command, if supported */
	retval = io_channel_zeroout(fs->io, blk, അക്കം);
	എങ്കിൽ (retval == 0)
		തിരികെ 0;

	/* Allocate the zeroizing buffer if necessary */
	എങ്കിൽ (അക്കം > stride_length && stride_length < MAX_STRIDE_LENGTH) {
		ശൂന്യം *p;
		പൂർണ്ണസംഖ്യ new_stride = അക്കം;

		എങ്കിൽ (new_stride > MAX_STRIDE_LENGTH)
			new_stride = MAX_STRIDE_LENGTH;
		p = realloc(buf, fs->blocksize * new_stride);
		എങ്കിൽ (!p)
			തിരികെ EXT2_ET_NO_MEMORY;
		buf = p;
		stride_length = new_stride;
		memset(buf, 0, fs->blocksize * stride_length);
	}
	/* OK, do the write loop */
	j=0;
	ആകുമ്പോൾ (j < അക്കം) {
		എങ്കിൽ (blk % stride_length) {
			എണ്ണം = stride_length - (blk % stride_length);
			എങ്കിൽ (എണ്ണം > (അക്കം - j))
				എണ്ണം = അക്കം - j;
		} അല്ലെങ്കിൽ {
			എണ്ണം = അക്കം - j;
			എങ്കിൽ (എണ്ണം > stride_length)
				എണ്ണം = stride_length;
		}
		retval = io_channel_write_blk64(fs->io, blk, എണ്ണം, buf);
		എങ്കിൽ (retval) {
			എങ്കിൽ (ret_എണ്ണം)
				*ret_എണ്ണം = എണ്ണം;
			എങ്കിൽ (ret_blk)
				*ret_blk = blk;
			തിരികെ retval;
		}
		j += എണ്ണം; blk += എണ്ണം;
	}
	തിരികെ 0;
}

errcode_t ext2fs_zero_blocks(ext2_filsys fs, blk_t blk, പൂർണ്ണസംഖ്യ അക്കം,
			     blk_t *ret_blk, പൂർണ്ണസംഖ്യ *ret_എണ്ണം)
{
	blk64_t ret_blk2;
	errcode_t retval;

	retval = ext2fs_zero_blocks2(fs, blk, അക്കം, &ret_blk2, ret_എണ്ണം);
	എങ്കിൽ (retval)
		*ret_blk = (blk_t) ret_blk2;
	തിരികെ retval;
}

/*
 * Calculate the initial goal block to be roughly at the middle of the
 * filesystem.  Pick a group that has the largest number of free
 * blocks.
 */
സ്ഥിതം blk64_t get_midpoint_journal_block(ext2_filsys fs)
{
	dgrp_t	group, start, end, i, log_flex;

	group = ext2fs_group_of_blk2(fs, (ext2fs_blocks_എണ്ണം(fs->super) -
					 fs->super->s_first_data_block) / 2);
	log_flex = 1U << fs->super->s_log_groups_per_flex;
	എങ്കിൽ (fs->super->s_log_groups_per_flex && (group > log_flex)) {
		group = group & ~(log_flex - 1);
		ആകുമ്പോൾ ((group < fs->group_desc_എണ്ണം) &&
		       ext2fs_bg_free_blocks_എണ്ണം(fs, group) == 0)
			group++;
		എങ്കിൽ (group == fs->group_desc_എണ്ണം)
			group = 0;
		start = group;
	} അല്ലെങ്കിൽ
		start = (group > 0) ? group-1 : group;
	end = ((group+1) < fs->group_desc_എണ്ണം) ? group+1 : group;
	group = start;
	വേണ്ടി (i = start + 1; i <= end; i++)
		എങ്കിൽ (ext2fs_bg_free_blocks_എണ്ണം(fs, i) >
		    ext2fs_bg_free_blocks_എണ്ണം(fs, group))
			group = i;
	തിരികെ ext2fs_group_first_block2(fs, group);
}

/*
 * This function creates a journal using direct I/O routines.
 */
സ്ഥിതം errcode_t write_journal_inode(ext2_filsys fs, ext2_ino_t journal_ino,
				     ഘടന ext2fs_journal_params *jparams,
				     blk64_t goal, പൂർണ്ണസംഖ്യ flags)
{
	അക്ഷരം			*buf;
	errcode_t		retval;
	ഘടന ext2_inode	inode;
	unsigned long long	inode_size;
	പൂർണ്ണസംഖ്യ			falloc_flags = EXT2_FALLOCATE_FORCE_INIT;
	blk64_t			zblk;
	time_t			now;

	എങ്കിൽ ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		തിരികെ retval;

	എങ്കിൽ ((retval = ext2fs_read_bitmaps(fs)))
		പോകുക out2;

	എങ്കിൽ ((retval = ext2fs_read_inode(fs, journal_ino, &inode)))
		പോകുക out2;

	എങ്കിൽ (inode.i_blocks > 0) {
		retval = EEXIST;
		പോകുക out2;
	}

	എങ്കിൽ (goal == ~0ULL)
		goal = get_midpoint_journal_block(fs);

	എങ്കിൽ (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;

	എങ്കിൽ (!(flags & EXT2_MKJOURNAL_LAZYINIT))
		falloc_flags |= EXT2_FALLOCATE_ZERO_BLOCKS;

	inode_size = (unsigned long long)fs->blocksize *
			(jparams->അക്കം_journal_blocks + jparams->അക്കം_fc_blocks);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	inode.i_links_എണ്ണം = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	retval = ext2fs_inode_size_set(fs, &inode, inode_size);
	എങ്കിൽ (retval)
		പോകുക out2;

	retval = ext2fs_fallocate(fs, falloc_flags, journal_ino,
				  &inode, goal, 0,
				  jparams->അക്കം_journal_blocks + jparams->അക്കം_fc_blocks);
	എങ്കിൽ (retval)
		പോകുക out2;

	എങ്കിൽ ((retval = ext2fs_write_new_inode(fs, journal_ino, &inode)))
		പോകുക out2;

	retval = ext2fs_bmap2(fs, journal_ino, &inode, ശൂന്യ, 0, 0, ശൂന്യ, &zblk);
	എങ്കിൽ (retval)
		പോകുക out2;

	retval = io_channel_write_blk64(fs->io, zblk, 1, buf);
	എങ്കിൽ (retval)
		പോകുക out2;

	memcpy(fs->super->s_jnl_blocks, inode.i_block, EXT2_N_BLOCKS*4);
	fs->super->s_jnl_blocks[15] = inode.i_size_മുകളിലെ;
	fs->super->s_jnl_blocks[16] = inode.i_size;
	fs->super->s_jnl_backup_type = EXT3_JNL_BACKUP_BLOCKS;
	ext2fs_mark_super_dirty(fs);

out2:
	ext2fs_free_mem(&buf);
	തിരികെ retval;
}

/*
 * Find a reasonable journal file size (in blocks) given the number of blocks
 * in the filesystem.  For very small filesystems, it is not reasonable to
 * have a journal that fills more than half of the filesystem.
 *
 * n.b. comments assume 4k blocks
 */
പൂർണ്ണസംഖ്യ ext2fs_default_journal_size(__u64 അക്കം_blocks)
{
	എങ്കിൽ (അക്കം_blocks < 2048)
		തിരികെ -1;
	എങ്കിൽ (അക്കം_blocks < 32768)		/* 128 MB */
		തിരികെ (1024);			/* 4 MB */
	എങ്കിൽ (അക്കം_blocks < 256*1024)	/* 1 GB */
		തിരികെ (4096);			/* 16 MB */
	എങ്കിൽ (അക്കം_blocks < 512*1024)	/* 2 GB */
		തിരികെ (8192);			/* 32 MB */
	എങ്കിൽ (അക്കം_blocks < 4096*1024)	/* 16 GB */
		തിരികെ (16384);			/* 64 MB */
	എങ്കിൽ (അക്കം_blocks < 8192*1024)	/* 32 GB */
		തിരികെ (32768);			/* 128 MB */
	എങ്കിൽ (അക്കം_blocks < 16384*1024)	/* 64 GB */
		തിരികെ (65536);			/* 256 MB */
	എങ്കിൽ (അക്കം_blocks < 32768*1024)	/* 128 GB */
		തിരികെ (131072);		/* 512 MB */
	തിരികെ 262144;				/* 1 GB */
}

errcode_t ext2fs_get_journal_params(ഘടന ext2fs_journal_params *params,
		ext2_filsys fs)
{
	blk_t total_blks;
	പൂർണ്ണസംഖ്യ ret;

	memset(params, 0, വലുപ്പം(*params));
	എങ്കിൽ (ext2fs_has_feature_journal_dev(fs->super)) {
		total_blks = ext2fs_blocks_എണ്ണം(fs->super);
		എങ്കിൽ (total_blks < JBD2_MIN_JOURNAL_BLOCKS)
			തിരികെ EXT2_ET_JOURNAL_TOO_SMALL;

		എങ്കിൽ (!ext2fs_has_feature_fast_commit(fs->super)) {
			params->അക്കം_journal_blocks = total_blks;
			params->അക്കം_fc_blocks = 0;
			തിരികെ 0;
		}
		params->അക്കം_journal_blocks = ext2fs_blocks_എണ്ണം(fs->super) *
				EXT2_JOURNAL_TO_FC_BLKS_RATIO /
				(EXT2_JOURNAL_TO_FC_BLKS_RATIO + 1);
		എങ്കിൽ (JBD2_MIN_JOURNAL_BLOCKS > params->അക്കം_journal_blocks)
			params->അക്കം_journal_blocks = JBD2_MIN_JOURNAL_BLOCKS;
		params->അക്കം_fc_blocks = total_blks - params->അക്കം_journal_blocks;
		തിരികെ 0;
	}

	ret = ext2fs_default_journal_size(ext2fs_blocks_എണ്ണം(fs->super));
	എങ്കിൽ (ret < 0)
		തിരികെ EXT2_ET_JOURNAL_TOO_SMALL;

	params->അക്കം_journal_blocks = ret;
	എങ്കിൽ (ext2fs_has_feature_fast_commit(fs->super))
		params->അക്കം_fc_blocks = params->അക്കം_journal_blocks /
			EXT2_JOURNAL_TO_FC_BLKS_RATIO;
	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ ext2fs_journal_sb_start(പൂർണ്ണസംഖ്യ blocksize)
{
	എങ്കിൽ (blocksize == EXT2_MIN_BLOCK_SIZE)
		തിരികെ 2;
	തിരികെ 1;
}

/*
 * This function adds a journal device to a filesystem
 */
errcode_t ext2fs_add_journal_device(ext2_filsys fs, ext2_filsys journal_dev)
{
	ഘടന stat	st;
	errcode_t	retval;
	അക്ഷരം		buf[SUPERBLOCK_SIZE];
	journal_superblock_t	*jsb;
	പൂർണ്ണസംഖ്യ		start;
	__u32		i, nr_users;

	/* Make sure the device exists and is a block device */
	എങ്കിൽ (stat(journal_dev->device_name, &st) < 0)
		തിരികെ errno;

	എങ്കിൽ (!S_ISBLK(st.st_mode))
		തിരികെ EXT2_ET_JOURNAL_NOT_BLOCK; /* Must be a block device */

	/* Get the journal superblock */
	start = ext2fs_journal_sb_start(journal_dev->blocksize);
	എങ്കിൽ ((retval = io_channel_read_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE,
					    buf)))
		തിരികെ retval;

	jsb = (journal_superblock_t *) buf;
	എങ്കിൽ ((jsb->s_header.h_magic != (unsigned) ntohl(JBD2_MAGIC_സംഖ്യ)) ||
	    (jsb->s_header.h_blocktype != (unsigned) ntohl(JBD2_SUPERBLOCK_V2)))
		തിരികെ EXT2_ET_NO_JOURNAL_SB;

	എങ്കിൽ (ntohl(jsb->s_blocksize) != (unsigned long) fs->blocksize)
		തിരികെ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	/* Check and see if this filesystem has already been added */
	nr_users = ntohl(jsb->s_nr_users);
	എങ്കിൽ (nr_users > JBD2_USERS_MAX)
		തിരികെ EXT2_ET_CORRUPT_JOURNAL_SB;
	വേണ്ടി (i=0; i < nr_users; i++) {
		എങ്കിൽ (memcmp(fs->super->s_uuid,
			   &jsb->s_users[i*16], 16) == 0)
			നിർത്തുക;
	}
	എങ്കിൽ (i >= nr_users) {
		memcpy(&jsb->s_users[nr_users*16],
		       fs->super->s_uuid, 16);
		jsb->s_nr_users = htonl(nr_users+1);
	}

	/* Writeback the journal superblock */
	എങ്കിൽ ((retval = io_channel_write_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE, buf)))
		തിരികെ retval;

	fs->super->s_journal_inum = 0;
	fs->super->s_journal_dev = st.st_rdev;
	memcpy(fs->super->s_journal_uuid, jsb->s_uuid,
	       വലുപ്പം(fs->super->s_journal_uuid));
	memset(fs->super->s_jnl_blocks, 0, വലുപ്പം(fs->super->s_jnl_blocks));
	ext2fs_set_feature_journal(fs->super);
	ext2fs_mark_super_dirty(fs);
	തിരികെ 0;
}

/*
 * This function adds a journal inode to a filesystem, using either
 * POSIX routines if the filesystem is mounted, or using direct I/O
 * functions if it is not.
 */
errcode_t ext2fs_add_journal_inode3(ext2_filsys fs, ഘടന ext2fs_journal_params *jparams,
				    blk64_t goal, പൂർണ്ണസംഖ്യ flags)
{
	errcode_t		retval;
	ext2_ino_t		journal_ino;
	ഘടന stat		st;
	അക്ഷരം			jfile[1024];
	പൂർണ്ണസംഖ്യ			mount_flags;
	പൂർണ്ണസംഖ്യ			fd = -1;

	എങ്കിൽ (flags & EXT2_MKJOURNAL_NO_MNT_CHECK)
		mount_flags = 0;
	അല്ലെങ്കിൽ എങ്കിൽ ((retval = ext2fs_check_mount_point(fs->device_name,
						    &mount_flags,
						    jfile, വലുപ്പം(jfile)-10)))
		തിരികെ retval;

	എങ്കിൽ (mount_flags & EXT2_MF_MOUNTED) {
#എങ്കിൽ HAVE_EXT2_IOCTLS
		പൂർണ്ണസംഖ്യ f = 0;
#endif
		strcat(jfile, "/.journal");

		/*
		 * If .../.journal already exists, make sure any
		 * immutable or append-only flags are cleared.
		 */
#എങ്കിൽ defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		(ശൂന്യം) chflags (jfile, 0);
#അല്ലെങ്കിൽ
#എങ്കിൽ HAVE_EXT2_IOCTLS
		fd = open(jfile, O_RDONLY);
		എങ്കിൽ (fd >= 0) {
			retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
			close(fd);
			എങ്കിൽ (retval)
				തിരികെ errno;
		}
#endif
#endif

		/* Create the journal file */
		എങ്കിൽ ((fd = open(jfile, O_CREAT|O_WRONLY, 0600)) < 0)
			തിരികെ errno;

		/* Note that we can't do lazy journal initialization for mounted
		 * filesystems, since the zero writing is also allocating the
		 * journal blocks.  We could use fallocate, but not all kernels
		 * support that, and creating a journal on a mounted ext2
		 * filesystems is extremely rare these days...  Ignore it. */
		flags &= ~EXT2_MKJOURNAL_LAZYINIT;

		എങ്കിൽ ((retval = write_journal_file(fs, jfile, jparams, flags)))
			പോകുക errout;

		/* Get inode number of the journal file */
		എങ്കിൽ (fstat(fd, &st) < 0) {
			retval = errno;
			പോകുക errout;
		}

#എങ്കിൽ defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		retval = fchflags (fd, UF_NODUMP|UF_IMMUTABLE);
#അല്ലെങ്കിൽ
#എങ്കിൽ HAVE_EXT2_IOCTLS
		എങ്കിൽ (ioctl(fd, EXT2_IOC_GETFLAGS, &f) < 0) {
			retval = errno;
			പോകുക errout;
		}
		f |= EXT2_NODUMP_FL | EXT2_IMMUTABLE_FL;
		retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
#endif
#endif
		എങ്കിൽ (retval) {
			retval = errno;
			പോകുക errout;
		}

		എങ്കിൽ (close(fd) < 0) {
			retval = errno;
			fd = -1;
			പോകുക errout;
		}
		journal_ino = st.st_ino;
		memset(fs->super->s_jnl_blocks, 0,
		       വലുപ്പം(fs->super->s_jnl_blocks));
	} അല്ലെങ്കിൽ {
		എങ്കിൽ ((mount_flags & EXT2_MF_BUSY) &&
		    !(fs->flags & EXT2_FLAG_EXCLUSIVE)) {
			retval = EBUSY;
			പോകുക errout;
		}
		journal_ino = EXT2_JOURNAL_INO;
		എങ്കിൽ ((retval = write_journal_inode(fs, journal_ino,
						  jparams, goal, flags)))
			തിരികെ retval;
	}

	fs->super->s_journal_inum = journal_ino;
	fs->super->s_journal_dev = 0;
	memset(fs->super->s_journal_uuid, 0,
	       വലുപ്പം(fs->super->s_journal_uuid));
	ext2fs_set_feature_journal(fs->super);

	ext2fs_mark_super_dirty(fs);
	തിരികെ 0;
errout:
	എങ്കിൽ (fd >= 0)
		close(fd);
	തിരികെ retval;
}

errcode_t ext2fs_add_journal_inode2(ext2_filsys fs, blk_t അക്കം_blocks,
				    blk64_t goal, പൂർണ്ണസംഖ്യ flags)
{
	ഘടന ext2fs_journal_params jparams;

	jparams.അക്കം_journal_blocks = അക്കം_blocks;
	jparams.അക്കം_fc_blocks = 0;

	തിരികെ ext2fs_add_journal_inode3(fs, &jparams, goal, flags);
}

errcode_t ext2fs_add_journal_inode(ext2_filsys fs, blk_t അക്കം_blocks, പൂർണ്ണസംഖ്യ flags)
{
	തിരികെ ext2fs_add_journal_inode2(fs, അക്കം_blocks, ~0ULL, flags);
}


#ifdef DEBUG
പ്രധാന(പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
	errcode_t	retval;
	അക്ഷരം		*device_name;
	ext2_filsys	fs;

	എങ്കിൽ (argc < 2) {
		fprintf(stderr, "Usage: %s filesystem\n", argv[0]);
		exit(1);
	}
	device_name = argv[1];

	retval = ext2fs_open (device_name, EXT2_FLAG_RW, 0, 0,
			      unix_io_manager, &fs);
	എങ്കിൽ (retval) {
		com_err(argv[0], retval, "while opening %s", device_name);
		exit(1);
	}

	retval = ext2fs_add_journal_inode(fs, JBD2_MIN_JOURNAL_BLOCKS, 0);
	എങ്കിൽ (retval) {
		com_err(argv[0], retval, "while adding journal to %s",
			device_name);
		exit(1);
	}
	retval = ext2fs_flush(fs);
	എങ്കിൽ (retval) {
		printf("Warning, had trouble writing out superblocks.\n");
	}
	ext2fs_close_free(&fs);
	exit(0);

}
#endif
