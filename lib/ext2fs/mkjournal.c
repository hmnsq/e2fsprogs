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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <assert.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#எனில் HAVE_ERRNO_H
#அந்தர்பாவ <errno.h>
#endif
#அந்தர்பாவ <fcntl.h>
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif
#எனில் HAVE_SYS_IOCTL_H
#அந்தர்பாவ <sys/ioctl.h>
#endif
#எனில் HAVE_NETINET_IN_H
#அந்தர்பாவ <netinet/in.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "e2p/e2p.h"
#அந்தர்பாவ "ext2fsP.h"

#அந்தர்பாவ "kernel-jbd.h"

/*
 * This function automatically sets up the journal superblock and
 * returns it as an allocated block.
 */
errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
					   கட்டமைப்பு ext2fs_journal_params *jparams,
					   முழு flags, எழுத்து **ret_jsb)
{
	errcode_t		retval;
	journal_superblock_t	*jsb;

	எனில் (jparams->இலக்கம்_journal_blocks < JBD2_MIN_JOURNAL_BLOCKS)
		திரும்பு EXT2_ET_JOURNAL_TOO_SMALL;

	எனில் ((retval = ext2fs_get_mem(fs->blocksize, &jsb)))
		திரும்பு retval;

	memset (jsb, 0, fs->blocksize);

	jsb->s_header.h_magic = htonl(JBD2_MAGIC_எண்);
	எனில் (flags & EXT2_MKJOURNAL_V1_SUPER)
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V1);
	இல்லையெனில்
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V2);
	jsb->s_blocksize = htonl(fs->blocksize);
	jsb->s_maxlen = htonl(jparams->இலக்கம்_journal_blocks + jparams->இலக்கம்_fc_blocks);
	jsb->s_nr_users = htonl(1);
	jsb->s_first = htonl(1);
	jsb->s_sequence = htonl(1);
	jsb->s_இலக்கம்_fc_blks = htonl(jparams->இலக்கம்_fc_blocks);
	memcpy(jsb->s_uuid, fs->super->s_uuid, அளவு(fs->super->s_uuid));
	/*
	 * If we're creating an external journal device, we need to
	 * adjust these fields.
	 */
	எனில் (ext2fs_has_feature_journal_dev(fs->super)) {
		jsb->s_nr_users = 0;
		jsb->s_first = htonl(ext2fs_journal_sb_start(fs->blocksize) + 1);
	}

	*ret_jsb = (எழுத்து *) jsb;
	திரும்பு 0;
}

errcode_t ext2fs_create_journal_superblock(ext2_filsys fs, __u32 இலக்கம்_blocks,
					முழு flags, எழுத்து **ret_sb)
{
	கட்டமைப்பு ext2fs_journal_params jparams;

	jparams.இலக்கம்_journal_blocks = இலக்கம்_blocks;
	jparams.இலக்கம்_fc_blocks = 0;

	திரும்பு ext2fs_create_journal_superblock2(fs, &jparams, flags, ret_sb);
}

/*
 * This function writes a journal using POSIX routines.  It is used
 * for creating external journals and creating journals on live
 * filesystems.
 */
நிலையான errcode_t write_journal_file(ext2_filsys fs, எழுத்து *filename,
				    கட்டமைப்பு ext2fs_journal_params *jparams,
				    முழு flags)
{
	errcode_t	retval;
	எழுத்து		*buf = 0;
	முழு		fd, ret_size;
	blk_t		i;

	எனில் ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		திரும்பு retval;

	/* Open the device or journal file */
	எனில் ((fd = open(filename, O_WRONLY)) < 0) {
		retval = errno;
		செல் errfree;
	}

	/* Write the superblock out */
	retval = EXT2_ET_SHORT_WRITE;
	ret_size = லிக(fd, buf, fs->blocksize);
	எனில் (ret_size < 0) {
		retval = errno;
		செல் errout;
	}
	எனில் (ret_size != (முழு) fs->blocksize)
		செல் errout;
	memset(buf, 0, fs->blocksize);

	எனில் (flags & EXT2_MKJOURNAL_LAZYINIT)
		செல் success;

	க்கு (i = 1; i < jparams->இலக்கம்_journal_blocks + jparams->இலக்கம்_fc_blocks; i++) {
		ret_size = லிக(fd, buf, fs->blocksize);
		எனில் (ret_size < 0) {
			retval = errno;
			செல் errout;
		}
		எனில் (ret_size != (முழு) fs->blocksize)
			செல் errout;
	}

success:
	retval = 0;
errout:
	close(fd);
errfree:
	ext2fs_free_mem(&buf);
	திரும்பு retval;
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
#define MAX_STRIDE_LENGTH (4194304 / (முழு) fs->blocksize)
errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, முழு இலக்கம்,
			      blk64_t *ret_blk, முழு *ret_மொத்தம்)
{
	முழு		j, மொத்தம்;
	நிலையான வெறுமை	*buf;
	நிலையான முழு	stride_length;
	errcode_t	retval;

	/* If fs is null, clean up the static buffer and return */
	எனில் (!fs) {
		எனில் (buf) {
			விடுவி(buf);
			buf = 0;
			stride_length = 0;
		}
		திரும்பு 0;
	}

	/* Deal with zeroing less than 1 block */
	எனில் (இலக்கம் <= 0)
		திரும்பு 0;

	/* Try a zero out command, if supported */
	retval = io_channel_zeroout(fs->io, blk, இலக்கம்);
	எனில் (retval == 0)
		திரும்பு 0;

	/* Allocate the zeroizing buffer if necessary */
	எனில் (இலக்கம் > stride_length && stride_length < MAX_STRIDE_LENGTH) {
		வெறுமை *p;
		முழு new_stride = இலக்கம்;

		எனில் (new_stride > MAX_STRIDE_LENGTH)
			new_stride = MAX_STRIDE_LENGTH;
		p = realloc(buf, fs->blocksize * new_stride);
		எனில் (!p)
			திரும்பு EXT2_ET_NO_MEMORY;
		buf = p;
		stride_length = new_stride;
		memset(buf, 0, fs->blocksize * stride_length);
	}
	/* OK, do the write loop */
	j=0;
	போது (j < இலக்கம்) {
		எனில் (blk % stride_length) {
			மொத்தம் = stride_length - (blk % stride_length);
			எனில் (மொத்தம் > (இலக்கம் - j))
				மொத்தம் = இலக்கம் - j;
		} இல்லையெனில் {
			மொத்தம் = இலக்கம் - j;
			எனில் (மொத்தம் > stride_length)
				மொத்தம் = stride_length;
		}
		retval = io_channel_write_blk64(fs->io, blk, மொத்தம், buf);
		எனில் (retval) {
			எனில் (ret_மொத்தம்)
				*ret_மொத்தம் = மொத்தம்;
			எனில் (ret_blk)
				*ret_blk = blk;
			திரும்பு retval;
		}
		j += மொத்தம்; blk += மொத்தம்;
	}
	திரும்பு 0;
}

errcode_t ext2fs_zero_blocks(ext2_filsys fs, blk_t blk, முழு இலக்கம்,
			     blk_t *ret_blk, முழு *ret_மொத்தம்)
{
	blk64_t ret_blk2;
	errcode_t retval;

	retval = ext2fs_zero_blocks2(fs, blk, இலக்கம், &ret_blk2, ret_மொத்தம்);
	எனில் (retval)
		*ret_blk = (blk_t) ret_blk2;
	திரும்பு retval;
}

/*
 * Calculate the initial goal block to be roughly at the middle of the
 * filesystem.  Pick a group that has the largest number of free
 * blocks.
 */
நிலையான blk64_t get_midpoint_journal_block(ext2_filsys fs)
{
	dgrp_t	group, start, end, i, log_flex;

	group = ext2fs_group_of_blk2(fs, (ext2fs_blocks_மொத்தம்(fs->super) -
					 fs->super->s_first_data_block) / 2);
	log_flex = 1U << fs->super->s_log_groups_per_flex;
	எனில் (fs->super->s_log_groups_per_flex && (group > log_flex)) {
		group = group & ~(log_flex - 1);
		போது ((group < fs->group_desc_மொத்தம்) &&
		       ext2fs_bg_free_blocks_மொத்தம்(fs, group) == 0)
			group++;
		எனில் (group == fs->group_desc_மொத்தம்)
			group = 0;
		start = group;
	} இல்லையெனில்
		start = (group > 0) ? group-1 : group;
	end = ((group+1) < fs->group_desc_மொத்தம்) ? group+1 : group;
	group = start;
	க்கு (i = start + 1; i <= end; i++)
		எனில் (ext2fs_bg_free_blocks_மொத்தம்(fs, i) >
		    ext2fs_bg_free_blocks_மொத்தம்(fs, group))
			group = i;
	திரும்பு ext2fs_group_first_block2(fs, group);
}

/*
 * This function creates a journal using direct I/O routines.
 */
நிலையான errcode_t write_journal_inode(ext2_filsys fs, ext2_ino_t journal_ino,
				     கட்டமைப்பு ext2fs_journal_params *jparams,
				     blk64_t goal, முழு flags)
{
	எழுத்து			*buf;
	errcode_t		retval;
	கட்டமைப்பு ext2_inode	inode;
	unsigned long long	inode_size;
	முழு			falloc_flags = EXT2_FALLOCATE_FORCE_INIT;
	blk64_t			zblk;
	time_t			now;

	எனில் ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		திரும்பு retval;

	எனில் ((retval = ext2fs_read_bitmaps(fs)))
		செல் out2;

	எனில் ((retval = ext2fs_read_inode(fs, journal_ino, &inode)))
		செல் out2;

	எனில் (inode.i_blocks > 0) {
		retval = EEXIST;
		செல் out2;
	}

	எனில் (goal == ~0ULL)
		goal = get_midpoint_journal_block(fs);

	எனில் (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;

	எனில் (!(flags & EXT2_MKJOURNAL_LAZYINIT))
		falloc_flags |= EXT2_FALLOCATE_ZERO_BLOCKS;

	inode_size = (unsigned long long)fs->blocksize *
			(jparams->இலக்கம்_journal_blocks + jparams->இலக்கம்_fc_blocks);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	inode.i_links_மொத்தம் = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	retval = ext2fs_inode_size_set(fs, &inode, inode_size);
	எனில் (retval)
		செல் out2;

	retval = ext2fs_fallocate(fs, falloc_flags, journal_ino,
				  &inode, goal, 0,
				  jparams->இலக்கம்_journal_blocks + jparams->இலக்கம்_fc_blocks);
	எனில் (retval)
		செல் out2;

	எனில் ((retval = ext2fs_write_new_inode(fs, journal_ino, &inode)))
		செல் out2;

	retval = ext2fs_bmap2(fs, journal_ino, &inode, வெற்று, 0, 0, வெற்று, &zblk);
	எனில் (retval)
		செல் out2;

	retval = io_channel_write_blk64(fs->io, zblk, 1, buf);
	எனில் (retval)
		செல் out2;

	memcpy(fs->super->s_jnl_blocks, inode.i_block, EXT2_N_BLOCKS*4);
	fs->super->s_jnl_blocks[15] = inode.i_size_உயர்வு;
	fs->super->s_jnl_blocks[16] = inode.i_size;
	fs->super->s_jnl_backup_type = EXT3_JNL_BACKUP_BLOCKS;
	ext2fs_mark_super_dirty(fs);

out2:
	ext2fs_free_mem(&buf);
	திரும்பு retval;
}

/*
 * Find a reasonable journal file size (in blocks) given the number of blocks
 * in the filesystem.  For very small filesystems, it is not reasonable to
 * have a journal that fills more than half of the filesystem.
 *
 * n.b. comments assume 4k blocks
 */
முழு ext2fs_default_journal_size(__u64 இலக்கம்_blocks)
{
	எனில் (இலக்கம்_blocks < 2048)
		திரும்பு -1;
	எனில் (இலக்கம்_blocks < 32768)		/* 128 MB */
		திரும்பு (1024);			/* 4 MB */
	எனில் (இலக்கம்_blocks < 256*1024)	/* 1 GB */
		திரும்பு (4096);			/* 16 MB */
	எனில் (இலக்கம்_blocks < 512*1024)	/* 2 GB */
		திரும்பு (8192);			/* 32 MB */
	எனில் (இலக்கம்_blocks < 4096*1024)	/* 16 GB */
		திரும்பு (16384);			/* 64 MB */
	எனில் (இலக்கம்_blocks < 8192*1024)	/* 32 GB */
		திரும்பு (32768);			/* 128 MB */
	எனில் (இலக்கம்_blocks < 16384*1024)	/* 64 GB */
		திரும்பு (65536);			/* 256 MB */
	எனில் (இலக்கம்_blocks < 32768*1024)	/* 128 GB */
		திரும்பு (131072);		/* 512 MB */
	திரும்பு 262144;				/* 1 GB */
}

errcode_t ext2fs_get_journal_params(கட்டமைப்பு ext2fs_journal_params *params,
		ext2_filsys fs)
{
	blk_t total_blks;
	முழு ret;

	memset(params, 0, அளவு(*params));
	எனில் (ext2fs_has_feature_journal_dev(fs->super)) {
		total_blks = ext2fs_blocks_மொத்தம்(fs->super);
		எனில் (total_blks < JBD2_MIN_JOURNAL_BLOCKS)
			திரும்பு EXT2_ET_JOURNAL_TOO_SMALL;

		எனில் (!ext2fs_has_feature_fast_commit(fs->super)) {
			params->இலக்கம்_journal_blocks = total_blks;
			params->இலக்கம்_fc_blocks = 0;
			திரும்பு 0;
		}
		params->இலக்கம்_journal_blocks = ext2fs_blocks_மொத்தம்(fs->super) *
				EXT2_JOURNAL_TO_FC_BLKS_RATIO /
				(EXT2_JOURNAL_TO_FC_BLKS_RATIO + 1);
		எனில் (JBD2_MIN_JOURNAL_BLOCKS > params->இலக்கம்_journal_blocks)
			params->இலக்கம்_journal_blocks = JBD2_MIN_JOURNAL_BLOCKS;
		params->இலக்கம்_fc_blocks = total_blks - params->இலக்கம்_journal_blocks;
		திரும்பு 0;
	}

	ret = ext2fs_default_journal_size(ext2fs_blocks_மொத்தம்(fs->super));
	எனில் (ret < 0)
		திரும்பு EXT2_ET_JOURNAL_TOO_SMALL;

	params->இலக்கம்_journal_blocks = ret;
	எனில் (ext2fs_has_feature_fast_commit(fs->super))
		params->இலக்கம்_fc_blocks = params->இலக்கம்_journal_blocks /
			EXT2_JOURNAL_TO_FC_BLKS_RATIO;
	திரும்பு 0;
}

முழு ext2fs_journal_sb_start(முழு blocksize)
{
	எனில் (blocksize == EXT2_MIN_BLOCK_SIZE)
		திரும்பு 2;
	திரும்பு 1;
}

/*
 * This function adds a journal device to a filesystem
 */
errcode_t ext2fs_add_journal_device(ext2_filsys fs, ext2_filsys journal_dev)
{
	கட்டமைப்பு stat	st;
	errcode_t	retval;
	எழுத்து		buf[SUPERBLOCK_SIZE];
	journal_superblock_t	*jsb;
	முழு		start;
	__u32		i, nr_users;

	/* Make sure the device exists and is a block device */
	எனில் (stat(journal_dev->device_name, &st) < 0)
		திரும்பு errno;

	எனில் (!S_ISBLK(st.st_mode))
		திரும்பு EXT2_ET_JOURNAL_NOT_BLOCK; /* Must be a block device */

	/* Get the journal superblock */
	start = ext2fs_journal_sb_start(journal_dev->blocksize);
	எனில் ((retval = io_channel_read_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE,
					    buf)))
		திரும்பு retval;

	jsb = (journal_superblock_t *) buf;
	எனில் ((jsb->s_header.h_magic != (unsigned) ntohl(JBD2_MAGIC_எண்)) ||
	    (jsb->s_header.h_blocktype != (unsigned) ntohl(JBD2_SUPERBLOCK_V2)))
		திரும்பு EXT2_ET_NO_JOURNAL_SB;

	எனில் (ntohl(jsb->s_blocksize) != (unsigned long) fs->blocksize)
		திரும்பு EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	/* Check and see if this filesystem has already been added */
	nr_users = ntohl(jsb->s_nr_users);
	எனில் (nr_users > JBD2_USERS_MAX)
		திரும்பு EXT2_ET_CORRUPT_JOURNAL_SB;
	க்கு (i=0; i < nr_users; i++) {
		எனில் (memcmp(fs->super->s_uuid,
			   &jsb->s_users[i*16], 16) == 0)
			நிறுத்து;
	}
	எனில் (i >= nr_users) {
		memcpy(&jsb->s_users[nr_users*16],
		       fs->super->s_uuid, 16);
		jsb->s_nr_users = htonl(nr_users+1);
	}

	/* Writeback the journal superblock */
	எனில் ((retval = io_channel_write_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE, buf)))
		திரும்பு retval;

	fs->super->s_journal_inum = 0;
	fs->super->s_journal_dev = st.st_rdev;
	memcpy(fs->super->s_journal_uuid, jsb->s_uuid,
	       அளவு(fs->super->s_journal_uuid));
	memset(fs->super->s_jnl_blocks, 0, அளவு(fs->super->s_jnl_blocks));
	ext2fs_set_feature_journal(fs->super);
	ext2fs_mark_super_dirty(fs);
	திரும்பு 0;
}

/*
 * This function adds a journal inode to a filesystem, using either
 * POSIX routines if the filesystem is mounted, or using direct I/O
 * functions if it is not.
 */
errcode_t ext2fs_add_journal_inode3(ext2_filsys fs, கட்டமைப்பு ext2fs_journal_params *jparams,
				    blk64_t goal, முழு flags)
{
	errcode_t		retval;
	ext2_ino_t		journal_ino;
	கட்டமைப்பு stat		st;
	எழுத்து			jfile[1024];
	முழு			mount_flags;
	முழு			fd = -1;

	எனில் (flags & EXT2_MKJOURNAL_NO_MNT_CHECK)
		mount_flags = 0;
	இல்லையெனில் எனில் ((retval = ext2fs_check_mount_point(fs->device_name,
						    &mount_flags,
						    jfile, அளவு(jfile)-10)))
		திரும்பு retval;

	எனில் (mount_flags & EXT2_MF_MOUNTED) {
#எனில் HAVE_EXT2_IOCTLS
		முழு f = 0;
#endif
		strcat(jfile, "/.journal");

		/*
		 * If .../.journal already exists, make sure any
		 * immutable or append-only flags are cleared.
		 */
#எனில் defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		(வெறுமை) chflags (jfile, 0);
#இல்லையெனில்
#எனில் HAVE_EXT2_IOCTLS
		fd = open(jfile, O_RDONLY);
		எனில் (fd >= 0) {
			retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
			close(fd);
			எனில் (retval)
				திரும்பு errno;
		}
#endif
#endif

		/* Create the journal file */
		எனில் ((fd = open(jfile, O_CREAT|O_WRONLY, 0600)) < 0)
			திரும்பு errno;

		/* Note that we can't do lazy journal initialization for mounted
		 * filesystems, since the zero writing is also allocating the
		 * journal blocks.  We could use fallocate, but not all kernels
		 * support that, and creating a journal on a mounted ext2
		 * filesystems is extremely rare these days...  Ignore it. */
		flags &= ~EXT2_MKJOURNAL_LAZYINIT;

		எனில் ((retval = write_journal_file(fs, jfile, jparams, flags)))
			செல் errout;

		/* Get inode number of the journal file */
		எனில் (fstat(fd, &st) < 0) {
			retval = errno;
			செல் errout;
		}

#எனில் defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		retval = fchflags (fd, UF_NODUMP|UF_IMMUTABLE);
#இல்லையெனில்
#எனில் HAVE_EXT2_IOCTLS
		எனில் (ioctl(fd, EXT2_IOC_GETFLAGS, &f) < 0) {
			retval = errno;
			செல் errout;
		}
		f |= EXT2_NODUMP_FL | EXT2_IMMUTABLE_FL;
		retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
#endif
#endif
		எனில் (retval) {
			retval = errno;
			செல் errout;
		}

		எனில் (close(fd) < 0) {
			retval = errno;
			fd = -1;
			செல் errout;
		}
		journal_ino = st.st_ino;
		memset(fs->super->s_jnl_blocks, 0,
		       அளவு(fs->super->s_jnl_blocks));
	} இல்லையெனில் {
		எனில் ((mount_flags & EXT2_MF_BUSY) &&
		    !(fs->flags & EXT2_FLAG_EXCLUSIVE)) {
			retval = EBUSY;
			செல் errout;
		}
		journal_ino = EXT2_JOURNAL_INO;
		எனில் ((retval = write_journal_inode(fs, journal_ino,
						  jparams, goal, flags)))
			திரும்பு retval;
	}

	fs->super->s_journal_inum = journal_ino;
	fs->super->s_journal_dev = 0;
	memset(fs->super->s_journal_uuid, 0,
	       அளவு(fs->super->s_journal_uuid));
	ext2fs_set_feature_journal(fs->super);

	ext2fs_mark_super_dirty(fs);
	திரும்பு 0;
errout:
	எனில் (fd >= 0)
		close(fd);
	திரும்பு retval;
}

errcode_t ext2fs_add_journal_inode2(ext2_filsys fs, blk_t இலக்கம்_blocks,
				    blk64_t goal, முழு flags)
{
	கட்டமைப்பு ext2fs_journal_params jparams;

	jparams.இலக்கம்_journal_blocks = இலக்கம்_blocks;
	jparams.இலக்கம்_fc_blocks = 0;

	திரும்பு ext2fs_add_journal_inode3(fs, &jparams, goal, flags);
}

errcode_t ext2fs_add_journal_inode(ext2_filsys fs, blk_t இலக்கம்_blocks, முழு flags)
{
	திரும்பு ext2fs_add_journal_inode2(fs, இலக்கம்_blocks, ~0ULL, flags);
}


#ifdef DEBUG
முதன்மை(முழு argc, எழுத்து **argv)
{
	errcode_t	retval;
	எழுத்து		*device_name;
	ext2_filsys	fs;

	எனில் (argc < 2) {
		fprintf(stderr, "Usage: %s filesystem\n", argv[0]);
		exit(1);
	}
	device_name = argv[1];

	retval = ext2fs_open (device_name, EXT2_FLAG_RW, 0, 0,
			      unix_io_manager, &fs);
	எனில் (retval) {
		com_err(argv[0], retval, "while opening %s", device_name);
		exit(1);
	}

	retval = ext2fs_add_journal_inode(fs, JBD2_MIN_JOURNAL_BLOCKS, 0);
	எனில் (retval) {
		com_err(argv[0], retval, "while adding journal to %s",
			device_name);
		exit(1);
	}
	retval = ext2fs_flush(fs);
	எனில் (retval) {
		printf("Warning, had trouble writing out superblocks.\n");
	}
	ext2fs_close_free(&fs);
	exit(0);

}
#endif
