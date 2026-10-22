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

#include "config.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#ꦪꦼꦤ꧀ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ꦪꦼꦤ꧀ HAVE_ERRNO_H
#include <errno.h>
#endif
#include <fcntl.h>
#ꦪꦼꦤ꧀ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ꦪꦼꦤ꧀ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ꦪꦼꦤ꧀ HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif
#ꦪꦼꦤ꧀ HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#include "ext2_fs.h"
#include "e2p/e2p.h"
#include "ext2fsP.h"

#include "kernel-jbd.h"

/*
 * This function automatically sets up the journal superblock and
 * returns it as an allocated block.
 */
errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
					   sa_nracanaa ext2fs_journal_params *jparams,
					   puur_naa_nka flags, var_na **ret_jsb)
{
	errcode_t		retval;
	journal_superblock_t	*jsb;

	ꦪꦼꦤ꧀ (jparams->num_journal_blocks < JBD2_MIN_JOURNAL_BLOCKS)
		ꦮꦭꦶ EXT2_ET_JOURNAL_TOO_SMALL;

	ꦪꦼꦤ꧀ ((retval = ext2fs_get_mem(fs->blocksize, &jsb)))
		ꦮꦭꦶ retval;

	memset (jsb, 0, fs->blocksize);

	jsb->s_header.h_magic = htonl(JBD2_MAGIC_NUMBER);
	ꦪꦼꦤ꧀ (flags & EXT2_MKJOURNAL_V1_SUPER)
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V1);
	ꦭꦶꦪ
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V2);
	jsb->s_blocksize = htonl(fs->blocksize);
	jsb->s_maxlen = htonl(jparams->num_journal_blocks + jparams->num_fc_blocks);
	jsb->s_nr_users = htonl(1);
	jsb->s_first = htonl(1);
	jsb->s_sequence = htonl(1);
	jsb->s_num_fc_blks = htonl(jparams->num_fc_blocks);
	memcpy(jsb->s_uuid, fs->super->s_uuid, parimaa_na(fs->super->s_uuid));
	/*
	 * If we're creating an external journal device, we need to
	 * adjust these fields.
	 */
	ꦪꦼꦤ꧀ (ext2fs_has_feature_journal_dev(fs->super)) {
		jsb->s_nr_users = 0;
		jsb->s_first = htonl(ext2fs_journal_sb_start(fs->blocksize) + 1);
	}

	*ret_jsb = (var_na *) jsb;
	ꦮꦭꦶ 0;
}

errcode_t ext2fs_create_journal_superblock(ext2_filsys fs, __u32 num_blocks,
					puur_naa_nka flags, var_na **ret_sb)
{
	sa_nracanaa ext2fs_journal_params jparams;

	jparams.num_journal_blocks = num_blocks;
	jparams.num_fc_blocks = 0;

	ꦮꦭꦶ ext2fs_create_journal_superblock2(fs, &jparams, flags, ret_sb);
}

/*
 * This function writes a journal using POSIX routines.  It is used
 * for creating external journals and creating journals on live
 * filesystems.
 */
static errcode_t write_journal_file(ext2_filsys fs, var_na *filename,
				    sa_nracanaa ext2fs_journal_params *jparams,
				    puur_naa_nka flags)
{
	errcode_t	retval;
	var_na		*buf = 0;
	puur_naa_nka		fd, ret_size;
	blk_t		i;

	ꦪꦼꦤ꧀ ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		ꦮꦭꦶ retval;

	/* Open the device or journal file */
	ꦪꦼꦤ꧀ ((fd = open(filename, O_WRONLY)) < 0) {
		retval = errno;
		goto errfree;
	}

	/* Write the superblock out */
	retval = EXT2_ET_SHORT_WRITE;
	ret_size = write(fd, buf, fs->blocksize);
	ꦪꦼꦤ꧀ (ret_size < 0) {
		retval = errno;
		goto errout;
	}
	ꦪꦼꦤ꧀ (ret_size != (puur_naa_nka) fs->blocksize)
		goto errout;
	memset(buf, 0, fs->blocksize);

	ꦪꦼꦤ꧀ (flags & EXT2_MKJOURNAL_LAZYINIT)
		goto success;

	ꦏꦁꦒꦺꦴ (i = 1; i < jparams->num_journal_blocks + jparams->num_fc_blocks; i++) {
		ret_size = write(fd, buf, fs->blocksize);
		ꦪꦼꦤ꧀ (ret_size < 0) {
			retval = errno;
			goto errout;
		}
		ꦪꦼꦤ꧀ (ret_size != (puur_naa_nka) fs->blocksize)
			goto errout;
	}

success:
	retval = 0;
errout:
	close(fd);
errfree:
	ext2fs_free_mem(&buf);
	ꦮꦭꦶ retval;
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
#define MAX_STRIDE_LENGTH (4194304 / (puur_naa_nka) fs->blocksize)
errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, puur_naa_nka num,
			      blk64_t *ret_blk, puur_naa_nka *ret_count)
{
	puur_naa_nka		j, count;
	static void	*buf;
	static puur_naa_nka	stride_length;
	errcode_t	retval;

	/* If fs is null, clean up the static buffer and return */
	ꦪꦼꦤ꧀ (!fs) {
		ꦪꦼꦤ꧀ (buf) {
			free(buf);
			buf = 0;
			stride_length = 0;
		}
		ꦮꦭꦶ 0;
	}

	/* Deal with zeroing less than 1 block */
	ꦪꦼꦤ꧀ (num <= 0)
		ꦮꦭꦶ 0;

	/* Try a zero out command, if supported */
	retval = io_channel_zeroout(fs->io, blk, num);
	ꦪꦼꦤ꧀ (retval == 0)
		ꦮꦭꦶ 0;

	/* Allocate the zeroizing buffer if necessary */
	ꦪꦼꦤ꧀ (num > stride_length && stride_length < MAX_STRIDE_LENGTH) {
		void *p;
		puur_naa_nka new_stride = num;

		ꦪꦼꦤ꧀ (new_stride > MAX_STRIDE_LENGTH)
			new_stride = MAX_STRIDE_LENGTH;
		p = realloc(buf, fs->blocksize * new_stride);
		ꦪꦼꦤ꧀ (!p)
			ꦮꦭꦶ EXT2_ET_NO_MEMORY;
		buf = p;
		stride_length = new_stride;
		memset(buf, 0, fs->blocksize * stride_length);
	}
	/* OK, do the write loop */
	j=0;
	ꦱꦼꦢꦼꦭ (j < num) {
		ꦪꦼꦤ꧀ (blk % stride_length) {
			count = stride_length - (blk % stride_length);
			ꦪꦼꦤ꧀ (count > (num - j))
				count = num - j;
		} ꦭꦶꦪ {
			count = num - j;
			ꦪꦼꦤ꧀ (count > stride_length)
				count = stride_length;
		}
		retval = io_channel_write_blk64(fs->io, blk, count, buf);
		ꦪꦼꦤ꧀ (retval) {
			ꦪꦼꦤ꧀ (ret_count)
				*ret_count = count;
			ꦪꦼꦤ꧀ (ret_blk)
				*ret_blk = blk;
			ꦮꦭꦶ retval;
		}
		j += count; blk += count;
	}
	ꦮꦭꦶ 0;
}

errcode_t ext2fs_zero_blocks(ext2_filsys fs, blk_t blk, puur_naa_nka num,
			     blk_t *ret_blk, puur_naa_nka *ret_count)
{
	blk64_t ret_blk2;
	errcode_t retval;

	retval = ext2fs_zero_blocks2(fs, blk, num, &ret_blk2, ret_count);
	ꦪꦼꦤ꧀ (retval)
		*ret_blk = (blk_t) ret_blk2;
	ꦮꦭꦶ retval;
}

/*
 * Calculate the initial goal block to be roughly at the middle of the
 * filesystem.  Pick a group that has the largest number of free
 * blocks.
 */
static blk64_t get_midpoint_journal_block(ext2_filsys fs)
{
	dgrp_t	group, start, end, i, log_flex;

	group = ext2fs_group_of_blk2(fs, (ext2fs_blocks_count(fs->super) -
					 fs->super->s_first_data_block) / 2);
	log_flex = 1U << fs->super->s_log_groups_per_flex;
	ꦪꦼꦤ꧀ (fs->super->s_log_groups_per_flex && (group > log_flex)) {
		group = group & ~(log_flex - 1);
		ꦱꦼꦢꦼꦭ ((group < fs->group_desc_count) &&
		       ext2fs_bg_free_blocks_count(fs, group) == 0)
			group++;
		ꦪꦼꦤ꧀ (group == fs->group_desc_count)
			group = 0;
		start = group;
	} ꦭꦶꦪ
		start = (group > 0) ? group-1 : group;
	end = ((group+1) < fs->group_desc_count) ? group+1 : group;
	group = start;
	ꦏꦁꦒꦺꦴ (i = start + 1; i <= end; i++)
		ꦪꦼꦤ꧀ (ext2fs_bg_free_blocks_count(fs, i) >
		    ext2fs_bg_free_blocks_count(fs, group))
			group = i;
	ꦮꦭꦶ ext2fs_group_first_block2(fs, group);
}

/*
 * This function creates a journal using direct I/O routines.
 */
static errcode_t write_journal_inode(ext2_filsys fs, ext2_ino_t journal_ino,
				     sa_nracanaa ext2fs_journal_params *jparams,
				     blk64_t goal, puur_naa_nka flags)
{
	var_na			*buf;
	errcode_t		retval;
	sa_nracanaa ext2_inode	inode;
	unsigned long long	inode_size;
	puur_naa_nka			falloc_flags = EXT2_FALLOCATE_FORCE_INIT;
	blk64_t			zblk;
	time_t			now;

	ꦪꦼꦤ꧀ ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		ꦮꦭꦶ retval;

	ꦪꦼꦤ꧀ ((retval = ext2fs_read_bitmaps(fs)))
		goto out2;

	ꦪꦼꦤ꧀ ((retval = ext2fs_read_inode(fs, journal_ino, &inode)))
		goto out2;

	ꦪꦼꦤ꧀ (inode.i_blocks > 0) {
		retval = EEXIST;
		goto out2;
	}

	ꦪꦼꦤ꧀ (goal == ~0ULL)
		goal = get_midpoint_journal_block(fs);

	ꦪꦼꦤ꧀ (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;

	ꦪꦼꦤ꧀ (!(flags & EXT2_MKJOURNAL_LAZYINIT))
		falloc_flags |= EXT2_FALLOCATE_ZERO_BLOCKS;

	inode_size = (unsigned long long)fs->blocksize *
			(jparams->num_journal_blocks + jparams->num_fc_blocks);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	inode.i_links_count = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	retval = ext2fs_inode_size_set(fs, &inode, inode_size);
	ꦪꦼꦤ꧀ (retval)
		goto out2;

	retval = ext2fs_fallocate(fs, falloc_flags, journal_ino,
				  &inode, goal, 0,
				  jparams->num_journal_blocks + jparams->num_fc_blocks);
	ꦪꦼꦤ꧀ (retval)
		goto out2;

	ꦪꦼꦤ꧀ ((retval = ext2fs_write_new_inode(fs, journal_ino, &inode)))
		goto out2;

	retval = ext2fs_bmap2(fs, journal_ino, &inode, NULL, 0, 0, NULL, &zblk);
	ꦪꦼꦤ꧀ (retval)
		goto out2;

	retval = io_channel_write_blk64(fs->io, zblk, 1, buf);
	ꦪꦼꦤ꧀ (retval)
		goto out2;

	memcpy(fs->super->s_jnl_blocks, inode.i_block, EXT2_N_BLOCKS*4);
	fs->super->s_jnl_blocks[15] = inode.i_size_high;
	fs->super->s_jnl_blocks[16] = inode.i_size;
	fs->super->s_jnl_backup_type = EXT3_JNL_BACKUP_BLOCKS;
	ext2fs_mark_super_dirty(fs);

out2:
	ext2fs_free_mem(&buf);
	ꦮꦭꦶ retval;
}

/*
 * Find a reasonable journal file size (in blocks) given the number of blocks
 * in the filesystem.  For very small filesystems, it is not reasonable to
 * have a journal that fills more than half of the filesystem.
 *
 * n.b. comments assume 4k blocks
 */
puur_naa_nka ext2fs_default_journal_size(__u64 num_blocks)
{
	ꦪꦼꦤ꧀ (num_blocks < 2048)
		ꦮꦭꦶ -1;
	ꦪꦼꦤ꧀ (num_blocks < 32768)		/* 128 MB */
		ꦮꦭꦶ (1024);			/* 4 MB */
	ꦪꦼꦤ꧀ (num_blocks < 256*1024)	/* 1 GB */
		ꦮꦭꦶ (4096);			/* 16 MB */
	ꦪꦼꦤ꧀ (num_blocks < 512*1024)	/* 2 GB */
		ꦮꦭꦶ (8192);			/* 32 MB */
	ꦪꦼꦤ꧀ (num_blocks < 4096*1024)	/* 16 GB */
		ꦮꦭꦶ (16384);			/* 64 MB */
	ꦪꦼꦤ꧀ (num_blocks < 8192*1024)	/* 32 GB */
		ꦮꦭꦶ (32768);			/* 128 MB */
	ꦪꦼꦤ꧀ (num_blocks < 16384*1024)	/* 64 GB */
		ꦮꦭꦶ (65536);			/* 256 MB */
	ꦪꦼꦤ꧀ (num_blocks < 32768*1024)	/* 128 GB */
		ꦮꦭꦶ (131072);		/* 512 MB */
	ꦮꦭꦶ 262144;				/* 1 GB */
}

errcode_t ext2fs_get_journal_params(sa_nracanaa ext2fs_journal_params *params,
		ext2_filsys fs)
{
	blk_t total_blks;
	puur_naa_nka ret;

	memset(params, 0, parimaa_na(*params));
	ꦪꦼꦤ꧀ (ext2fs_has_feature_journal_dev(fs->super)) {
		total_blks = ext2fs_blocks_count(fs->super);
		ꦪꦼꦤ꧀ (total_blks < JBD2_MIN_JOURNAL_BLOCKS)
			ꦮꦭꦶ EXT2_ET_JOURNAL_TOO_SMALL;

		ꦪꦼꦤ꧀ (!ext2fs_has_feature_fast_commit(fs->super)) {
			params->num_journal_blocks = total_blks;
			params->num_fc_blocks = 0;
			ꦮꦭꦶ 0;
		}
		params->num_journal_blocks = ext2fs_blocks_count(fs->super) *
				EXT2_JOURNAL_TO_FC_BLKS_RATIO /
				(EXT2_JOURNAL_TO_FC_BLKS_RATIO + 1);
		ꦪꦼꦤ꧀ (JBD2_MIN_JOURNAL_BLOCKS > params->num_journal_blocks)
			params->num_journal_blocks = JBD2_MIN_JOURNAL_BLOCKS;
		params->num_fc_blocks = total_blks - params->num_journal_blocks;
		ꦮꦭꦶ 0;
	}

	ret = ext2fs_default_journal_size(ext2fs_blocks_count(fs->super));
	ꦪꦼꦤ꧀ (ret < 0)
		ꦮꦭꦶ EXT2_ET_JOURNAL_TOO_SMALL;

	params->num_journal_blocks = ret;
	ꦪꦼꦤ꧀ (ext2fs_has_feature_fast_commit(fs->super))
		params->num_fc_blocks = params->num_journal_blocks /
			EXT2_JOURNAL_TO_FC_BLKS_RATIO;
	ꦮꦭꦶ 0;
}

puur_naa_nka ext2fs_journal_sb_start(puur_naa_nka blocksize)
{
	ꦪꦼꦤ꧀ (blocksize == EXT2_MIN_BLOCK_SIZE)
		ꦮꦭꦶ 2;
	ꦮꦭꦶ 1;
}

/*
 * This function adds a journal device to a filesystem
 */
errcode_t ext2fs_add_journal_device(ext2_filsys fs, ext2_filsys journal_dev)
{
	sa_nracanaa stat	st;
	errcode_t	retval;
	var_na		buf[SUPERBLOCK_SIZE];
	journal_superblock_t	*jsb;
	puur_naa_nka		start;
	__u32		i, nr_users;

	/* Make sure the device exists and is a block device */
	ꦪꦼꦤ꧀ (stat(journal_dev->device_name, &st) < 0)
		ꦮꦭꦶ errno;

	ꦪꦼꦤ꧀ (!S_ISBLK(st.st_mode))
		ꦮꦭꦶ EXT2_ET_JOURNAL_NOT_BLOCK; /* Must be a block device */

	/* Get the journal superblock */
	start = ext2fs_journal_sb_start(journal_dev->blocksize);
	ꦪꦼꦤ꧀ ((retval = io_channel_read_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE,
					    buf)))
		ꦮꦭꦶ retval;

	jsb = (journal_superblock_t *) buf;
	ꦪꦼꦤ꧀ ((jsb->s_header.h_magic != (unsigned) ntohl(JBD2_MAGIC_NUMBER)) ||
	    (jsb->s_header.h_blocktype != (unsigned) ntohl(JBD2_SUPERBLOCK_V2)))
		ꦮꦭꦶ EXT2_ET_NO_JOURNAL_SB;

	ꦪꦼꦤ꧀ (ntohl(jsb->s_blocksize) != (unsigned long) fs->blocksize)
		ꦮꦭꦶ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	/* Check and see if this filesystem has already been added */
	nr_users = ntohl(jsb->s_nr_users);
	ꦪꦼꦤ꧀ (nr_users > JBD2_USERS_MAX)
		ꦮꦭꦶ EXT2_ET_CORRUPT_JOURNAL_SB;
	ꦏꦁꦒꦺꦴ (i=0; i < nr_users; i++) {
		ꦪꦼꦤ꧀ (memcmp(fs->super->s_uuid,
			   &jsb->s_users[i*16], 16) == 0)
			bha_nga;
	}
	ꦪꦼꦤ꧀ (i >= nr_users) {
		memcpy(&jsb->s_users[nr_users*16],
		       fs->super->s_uuid, 16);
		jsb->s_nr_users = htonl(nr_users+1);
	}

	/* Writeback the journal superblock */
	ꦪꦼꦤ꧀ ((retval = io_channel_write_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE, buf)))
		ꦮꦭꦶ retval;

	fs->super->s_journal_inum = 0;
	fs->super->s_journal_dev = st.st_rdev;
	memcpy(fs->super->s_journal_uuid, jsb->s_uuid,
	       parimaa_na(fs->super->s_journal_uuid));
	memset(fs->super->s_jnl_blocks, 0, parimaa_na(fs->super->s_jnl_blocks));
	ext2fs_set_feature_journal(fs->super);
	ext2fs_mark_super_dirty(fs);
	ꦮꦭꦶ 0;
}

/*
 * This function adds a journal inode to a filesystem, using either
 * POSIX routines if the filesystem is mounted, or using direct I/O
 * functions if it is not.
 */
errcode_t ext2fs_add_journal_inode3(ext2_filsys fs, sa_nracanaa ext2fs_journal_params *jparams,
				    blk64_t goal, puur_naa_nka flags)
{
	errcode_t		retval;
	ext2_ino_t		journal_ino;
	sa_nracanaa stat		st;
	var_na			jfile[1024];
	puur_naa_nka			mount_flags;
	puur_naa_nka			fd = -1;

	ꦪꦼꦤ꧀ (flags & EXT2_MKJOURNAL_NO_MNT_CHECK)
		mount_flags = 0;
	ꦭꦶꦪ ꦪꦼꦤ꧀ ((retval = ext2fs_check_mount_point(fs->device_name,
						    &mount_flags,
						    jfile, parimaa_na(jfile)-10)))
		ꦮꦭꦶ retval;

	ꦪꦼꦤ꧀ (mount_flags & EXT2_MF_MOUNTED) {
#ꦪꦼꦤ꧀ HAVE_EXT2_IOCTLS
		puur_naa_nka f = 0;
#endif
		strcat(jfile, "/.journal");

		/*
		 * If .../.journal already exists, make sure any
		 * immutable or append-only flags are cleared.
		 */
#ꦪꦼꦤ꧀ defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		(void) chflags (jfile, 0);
#ꦭꦶꦪ
#ꦪꦼꦤ꧀ HAVE_EXT2_IOCTLS
		fd = open(jfile, O_RDONLY);
		ꦪꦼꦤ꧀ (fd >= 0) {
			retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
			close(fd);
			ꦪꦼꦤ꧀ (retval)
				ꦮꦭꦶ errno;
		}
#endif
#endif

		/* Create the journal file */
		ꦪꦼꦤ꧀ ((fd = open(jfile, O_CREAT|O_WRONLY, 0600)) < 0)
			ꦮꦭꦶ errno;

		/* Note that we can't do lazy journal initialization for mounted
		 * filesystems, since the zero writing is also allocating the
		 * journal blocks.  We could use fallocate, but not all kernels
		 * support that, and creating a journal on a mounted ext2
		 * filesystems is extremely rare these days...  Ignore it. */
		flags &= ~EXT2_MKJOURNAL_LAZYINIT;

		ꦪꦼꦤ꧀ ((retval = write_journal_file(fs, jfile, jparams, flags)))
			goto errout;

		/* Get inode number of the journal file */
		ꦪꦼꦤ꧀ (fstat(fd, &st) < 0) {
			retval = errno;
			goto errout;
		}

#ꦪꦼꦤ꧀ defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		retval = fchflags (fd, UF_NODUMP|UF_IMMUTABLE);
#ꦭꦶꦪ
#ꦪꦼꦤ꧀ HAVE_EXT2_IOCTLS
		ꦪꦼꦤ꧀ (ioctl(fd, EXT2_IOC_GETFLAGS, &f) < 0) {
			retval = errno;
			goto errout;
		}
		f |= EXT2_NODUMP_FL | EXT2_IMMUTABLE_FL;
		retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
#endif
#endif
		ꦪꦼꦤ꧀ (retval) {
			retval = errno;
			goto errout;
		}

		ꦪꦼꦤ꧀ (close(fd) < 0) {
			retval = errno;
			fd = -1;
			goto errout;
		}
		journal_ino = st.st_ino;
		memset(fs->super->s_jnl_blocks, 0,
		       parimaa_na(fs->super->s_jnl_blocks));
	} ꦭꦶꦪ {
		ꦪꦼꦤ꧀ ((mount_flags & EXT2_MF_BUSY) &&
		    !(fs->flags & EXT2_FLAG_EXCLUSIVE)) {
			retval = EBUSY;
			goto errout;
		}
		journal_ino = EXT2_JOURNAL_INO;
		ꦪꦼꦤ꧀ ((retval = write_journal_inode(fs, journal_ino,
						  jparams, goal, flags)))
			ꦮꦭꦶ retval;
	}

	fs->super->s_journal_inum = journal_ino;
	fs->super->s_journal_dev = 0;
	memset(fs->super->s_journal_uuid, 0,
	       parimaa_na(fs->super->s_journal_uuid));
	ext2fs_set_feature_journal(fs->super);

	ext2fs_mark_super_dirty(fs);
	ꦮꦭꦶ 0;
errout:
	ꦪꦼꦤ꧀ (fd >= 0)
		close(fd);
	ꦮꦭꦶ retval;
}

errcode_t ext2fs_add_journal_inode2(ext2_filsys fs, blk_t num_blocks,
				    blk64_t goal, puur_naa_nka flags)
{
	sa_nracanaa ext2fs_journal_params jparams;

	jparams.num_journal_blocks = num_blocks;
	jparams.num_fc_blocks = 0;

	ꦮꦭꦶ ext2fs_add_journal_inode3(fs, &jparams, goal, flags);
}

errcode_t ext2fs_add_journal_inode(ext2_filsys fs, blk_t num_blocks, puur_naa_nka flags)
{
	ꦮꦭꦶ ext2fs_add_journal_inode2(fs, num_blocks, ~0ULL, flags);
}


#ifdef DEBUG
main(puur_naa_nka argc, var_na **argv)
{
	errcode_t	retval;
	var_na		*device_name;
	ext2_filsys	fs;

	ꦪꦼꦤ꧀ (argc < 2) {
		fprintf(stderr, "Usage: %s filesystem\n", argv[0]);
		exit(1);
	}
	device_name = argv[1];

	retval = ext2fs_open (device_name, EXT2_FLAG_RW, 0, 0,
			      unix_io_manager, &fs);
	ꦪꦼꦤ꧀ (retval) {
		com_err(argv[0], retval, "while opening %s", device_name);
		exit(1);
	}

	retval = ext2fs_add_journal_inode(fs, JBD2_MIN_JOURNAL_BLOCKS, 0);
	ꦪꦼꦤ꧀ (retval) {
		com_err(argv[0], retval, "while adding journal to %s",
			device_name);
		exit(1);
	}
	retval = ext2fs_flush(fs);
	ꦪꦼꦤ꧀ (retval) {
		printf("Warning, had trouble writing out superblocks.\n");
	}
	ext2fs_close_free(&fs);
	exit(0);

}
#endif
