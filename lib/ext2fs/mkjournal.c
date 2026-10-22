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
#ប្រសិនបើ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ប្រសិនបើ HAVE_ERRNO_H
#include <errno.h>
#endif
#include <fcntl.h>
#ប្រសិនបើ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ប្រសិនបើ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ប្រសិនបើ HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif
#ប្រសិនបើ HAVE_NETINET_IN_H
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
					   ចំនួនគត់ flags, var_na **ret_jsb)
{
	errcode_t		retval;
	journal_superblock_t	*jsb;

	ប្រសិនបើ (jparams->num_journal_blocks < JBD2_MIN_JOURNAL_BLOCKS)
		ត្រឡប់ EXT2_ET_JOURNAL_TOO_SMALL;

	ប្រសិនបើ ((retval = ext2fs_get_mem(fs->blocksize, &jsb)))
		ត្រឡប់ retval;

	memset (jsb, 0, fs->blocksize);

	jsb->s_header.h_magic = htonl(JBD2_MAGIC_NUMBER);
	ប្រសិនបើ (flags & EXT2_MKJOURNAL_V1_SUPER)
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V1);
	បើមិនដូច្នេះ
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
	ប្រសិនបើ (ext2fs_has_feature_journal_dev(fs->super)) {
		jsb->s_nr_users = 0;
		jsb->s_first = htonl(ext2fs_journal_sb_start(fs->blocksize) + 1);
	}

	*ret_jsb = (var_na *) jsb;
	ត្រឡប់ 0;
}

errcode_t ext2fs_create_journal_superblock(ext2_filsys fs, __u32 num_blocks,
					ចំនួនគត់ flags, var_na **ret_sb)
{
	sa_nracanaa ext2fs_journal_params jparams;

	jparams.num_journal_blocks = num_blocks;
	jparams.num_fc_blocks = 0;

	ត្រឡប់ ext2fs_create_journal_superblock2(fs, &jparams, flags, ret_sb);
}

/*
 * This function writes a journal using POSIX routines.  It is used
 * for creating external journals and creating journals on live
 * filesystems.
 */
static errcode_t write_journal_file(ext2_filsys fs, var_na *filename,
				    sa_nracanaa ext2fs_journal_params *jparams,
				    ចំនួនគត់ flags)
{
	errcode_t	retval;
	var_na		*buf = 0;
	ចំនួនគត់		fd, ret_size;
	blk_t		i;

	ប្រសិនបើ ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		ត្រឡប់ retval;

	/* Open the device or journal file */
	ប្រសិនបើ ((fd = open(filename, O_WRONLY)) < 0) {
		retval = errno;
		goto errfree;
	}

	/* Write the superblock out */
	retval = EXT2_ET_SHORT_WRITE;
	ret_size = write(fd, buf, fs->blocksize);
	ប្រសិនបើ (ret_size < 0) {
		retval = errno;
		goto errout;
	}
	ប្រសិនបើ (ret_size != (ចំនួនគត់) fs->blocksize)
		goto errout;
	memset(buf, 0, fs->blocksize);

	ប្រសិនបើ (flags & EXT2_MKJOURNAL_LAZYINIT)
		goto success;

	សម្រាប់ (i = 1; i < jparams->num_journal_blocks + jparams->num_fc_blocks; i++) {
		ret_size = write(fd, buf, fs->blocksize);
		ប្រសិនបើ (ret_size < 0) {
			retval = errno;
			goto errout;
		}
		ប្រសិនបើ (ret_size != (ចំនួនគត់) fs->blocksize)
			goto errout;
	}

success:
	retval = 0;
errout:
	close(fd);
errfree:
	ext2fs_free_mem(&buf);
	ត្រឡប់ retval;
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
#define MAX_STRIDE_LENGTH (4194304 / (ចំនួនគត់) fs->blocksize)
errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, ចំនួនគត់ num,
			      blk64_t *ret_blk, ចំនួនគត់ *ret_count)
{
	ចំនួនគត់		j, count;
	static void	*buf;
	static ចំនួនគត់	stride_length;
	errcode_t	retval;

	/* If fs is null, clean up the static buffer and return */
	ប្រសិនបើ (!fs) {
		ប្រសិនបើ (buf) {
			free(buf);
			buf = 0;
			stride_length = 0;
		}
		ត្រឡប់ 0;
	}

	/* Deal with zeroing less than 1 block */
	ប្រសិនបើ (num <= 0)
		ត្រឡប់ 0;

	/* Try a zero out command, if supported */
	retval = io_channel_zeroout(fs->io, blk, num);
	ប្រសិនបើ (retval == 0)
		ត្រឡប់ 0;

	/* Allocate the zeroizing buffer if necessary */
	ប្រសិនបើ (num > stride_length && stride_length < MAX_STRIDE_LENGTH) {
		void *p;
		ចំនួនគត់ new_stride = num;

		ប្រសិនបើ (new_stride > MAX_STRIDE_LENGTH)
			new_stride = MAX_STRIDE_LENGTH;
		p = realloc(buf, fs->blocksize * new_stride);
		ប្រសិនបើ (!p)
			ត្រឡប់ EXT2_ET_NO_MEMORY;
		buf = p;
		stride_length = new_stride;
		memset(buf, 0, fs->blocksize * stride_length);
	}
	/* OK, do the write loop */
	j=0;
	ខណៈពេល (j < num) {
		ប្រសិនបើ (blk % stride_length) {
			count = stride_length - (blk % stride_length);
			ប្រសិនបើ (count > (num - j))
				count = num - j;
		} បើមិនដូច្នេះ {
			count = num - j;
			ប្រសិនបើ (count > stride_length)
				count = stride_length;
		}
		retval = io_channel_write_blk64(fs->io, blk, count, buf);
		ប្រសិនបើ (retval) {
			ប្រសិនបើ (ret_count)
				*ret_count = count;
			ប្រសិនបើ (ret_blk)
				*ret_blk = blk;
			ត្រឡប់ retval;
		}
		j += count; blk += count;
	}
	ត្រឡប់ 0;
}

errcode_t ext2fs_zero_blocks(ext2_filsys fs, blk_t blk, ចំនួនគត់ num,
			     blk_t *ret_blk, ចំនួនគត់ *ret_count)
{
	blk64_t ret_blk2;
	errcode_t retval;

	retval = ext2fs_zero_blocks2(fs, blk, num, &ret_blk2, ret_count);
	ប្រសិនបើ (retval)
		*ret_blk = (blk_t) ret_blk2;
	ត្រឡប់ retval;
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
	ប្រសិនបើ (fs->super->s_log_groups_per_flex && (group > log_flex)) {
		group = group & ~(log_flex - 1);
		ខណៈពេល ((group < fs->group_desc_count) &&
		       ext2fs_bg_free_blocks_count(fs, group) == 0)
			group++;
		ប្រសិនបើ (group == fs->group_desc_count)
			group = 0;
		start = group;
	} បើមិនដូច្នេះ
		start = (group > 0) ? group-1 : group;
	end = ((group+1) < fs->group_desc_count) ? group+1 : group;
	group = start;
	សម្រាប់ (i = start + 1; i <= end; i++)
		ប្រសិនបើ (ext2fs_bg_free_blocks_count(fs, i) >
		    ext2fs_bg_free_blocks_count(fs, group))
			group = i;
	ត្រឡប់ ext2fs_group_first_block2(fs, group);
}

/*
 * This function creates a journal using direct I/O routines.
 */
static errcode_t write_journal_inode(ext2_filsys fs, ext2_ino_t journal_ino,
				     sa_nracanaa ext2fs_journal_params *jparams,
				     blk64_t goal, ចំនួនគត់ flags)
{
	var_na			*buf;
	errcode_t		retval;
	sa_nracanaa ext2_inode	inode;
	unsigned long long	inode_size;
	ចំនួនគត់			falloc_flags = EXT2_FALLOCATE_FORCE_INIT;
	blk64_t			zblk;
	time_t			now;

	ប្រសិនបើ ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		ត្រឡប់ retval;

	ប្រសិនបើ ((retval = ext2fs_read_bitmaps(fs)))
		goto out2;

	ប្រសិនបើ ((retval = ext2fs_read_inode(fs, journal_ino, &inode)))
		goto out2;

	ប្រសិនបើ (inode.i_blocks > 0) {
		retval = EEXIST;
		goto out2;
	}

	ប្រសិនបើ (goal == ~0ULL)
		goal = get_midpoint_journal_block(fs);

	ប្រសិនបើ (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;

	ប្រសិនបើ (!(flags & EXT2_MKJOURNAL_LAZYINIT))
		falloc_flags |= EXT2_FALLOCATE_ZERO_BLOCKS;

	inode_size = (unsigned long long)fs->blocksize *
			(jparams->num_journal_blocks + jparams->num_fc_blocks);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	inode.i_links_count = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	retval = ext2fs_inode_size_set(fs, &inode, inode_size);
	ប្រសិនបើ (retval)
		goto out2;

	retval = ext2fs_fallocate(fs, falloc_flags, journal_ino,
				  &inode, goal, 0,
				  jparams->num_journal_blocks + jparams->num_fc_blocks);
	ប្រសិនបើ (retval)
		goto out2;

	ប្រសិនបើ ((retval = ext2fs_write_new_inode(fs, journal_ino, &inode)))
		goto out2;

	retval = ext2fs_bmap2(fs, journal_ino, &inode, NULL, 0, 0, NULL, &zblk);
	ប្រសិនបើ (retval)
		goto out2;

	retval = io_channel_write_blk64(fs->io, zblk, 1, buf);
	ប្រសិនបើ (retval)
		goto out2;

	memcpy(fs->super->s_jnl_blocks, inode.i_block, EXT2_N_BLOCKS*4);
	fs->super->s_jnl_blocks[15] = inode.i_size_high;
	fs->super->s_jnl_blocks[16] = inode.i_size;
	fs->super->s_jnl_backup_type = EXT3_JNL_BACKUP_BLOCKS;
	ext2fs_mark_super_dirty(fs);

out2:
	ext2fs_free_mem(&buf);
	ត្រឡប់ retval;
}

/*
 * Find a reasonable journal file size (in blocks) given the number of blocks
 * in the filesystem.  For very small filesystems, it is not reasonable to
 * have a journal that fills more than half of the filesystem.
 *
 * n.b. comments assume 4k blocks
 */
ចំនួនគត់ ext2fs_default_journal_size(__u64 num_blocks)
{
	ប្រសិនបើ (num_blocks < 2048)
		ត្រឡប់ -1;
	ប្រសិនបើ (num_blocks < 32768)		/* 128 MB */
		ត្រឡប់ (1024);			/* 4 MB */
	ប្រសិនបើ (num_blocks < 256*1024)	/* 1 GB */
		ត្រឡប់ (4096);			/* 16 MB */
	ប្រសិនបើ (num_blocks < 512*1024)	/* 2 GB */
		ត្រឡប់ (8192);			/* 32 MB */
	ប្រសិនបើ (num_blocks < 4096*1024)	/* 16 GB */
		ត្រឡប់ (16384);			/* 64 MB */
	ប្រសិនបើ (num_blocks < 8192*1024)	/* 32 GB */
		ត្រឡប់ (32768);			/* 128 MB */
	ប្រសិនបើ (num_blocks < 16384*1024)	/* 64 GB */
		ត្រឡប់ (65536);			/* 256 MB */
	ប្រសិនបើ (num_blocks < 32768*1024)	/* 128 GB */
		ត្រឡប់ (131072);		/* 512 MB */
	ត្រឡប់ 262144;				/* 1 GB */
}

errcode_t ext2fs_get_journal_params(sa_nracanaa ext2fs_journal_params *params,
		ext2_filsys fs)
{
	blk_t total_blks;
	ចំនួនគត់ ret;

	memset(params, 0, parimaa_na(*params));
	ប្រសិនបើ (ext2fs_has_feature_journal_dev(fs->super)) {
		total_blks = ext2fs_blocks_count(fs->super);
		ប្រសិនបើ (total_blks < JBD2_MIN_JOURNAL_BLOCKS)
			ត្រឡប់ EXT2_ET_JOURNAL_TOO_SMALL;

		ប្រសិនបើ (!ext2fs_has_feature_fast_commit(fs->super)) {
			params->num_journal_blocks = total_blks;
			params->num_fc_blocks = 0;
			ត្រឡប់ 0;
		}
		params->num_journal_blocks = ext2fs_blocks_count(fs->super) *
				EXT2_JOURNAL_TO_FC_BLKS_RATIO /
				(EXT2_JOURNAL_TO_FC_BLKS_RATIO + 1);
		ប្រសិនបើ (JBD2_MIN_JOURNAL_BLOCKS > params->num_journal_blocks)
			params->num_journal_blocks = JBD2_MIN_JOURNAL_BLOCKS;
		params->num_fc_blocks = total_blks - params->num_journal_blocks;
		ត្រឡប់ 0;
	}

	ret = ext2fs_default_journal_size(ext2fs_blocks_count(fs->super));
	ប្រសិនបើ (ret < 0)
		ត្រឡប់ EXT2_ET_JOURNAL_TOO_SMALL;

	params->num_journal_blocks = ret;
	ប្រសិនបើ (ext2fs_has_feature_fast_commit(fs->super))
		params->num_fc_blocks = params->num_journal_blocks /
			EXT2_JOURNAL_TO_FC_BLKS_RATIO;
	ត្រឡប់ 0;
}

ចំនួនគត់ ext2fs_journal_sb_start(ចំនួនគត់ blocksize)
{
	ប្រសិនបើ (blocksize == EXT2_MIN_BLOCK_SIZE)
		ត្រឡប់ 2;
	ត្រឡប់ 1;
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
	ចំនួនគត់		start;
	__u32		i, nr_users;

	/* Make sure the device exists and is a block device */
	ប្រសិនបើ (stat(journal_dev->device_name, &st) < 0)
		ត្រឡប់ errno;

	ប្រសិនបើ (!S_ISBLK(st.st_mode))
		ត្រឡប់ EXT2_ET_JOURNAL_NOT_BLOCK; /* Must be a block device */

	/* Get the journal superblock */
	start = ext2fs_journal_sb_start(journal_dev->blocksize);
	ប្រសិនបើ ((retval = io_channel_read_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE,
					    buf)))
		ត្រឡប់ retval;

	jsb = (journal_superblock_t *) buf;
	ប្រសិនបើ ((jsb->s_header.h_magic != (unsigned) ntohl(JBD2_MAGIC_NUMBER)) ||
	    (jsb->s_header.h_blocktype != (unsigned) ntohl(JBD2_SUPERBLOCK_V2)))
		ត្រឡប់ EXT2_ET_NO_JOURNAL_SB;

	ប្រសិនបើ (ntohl(jsb->s_blocksize) != (unsigned long) fs->blocksize)
		ត្រឡប់ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	/* Check and see if this filesystem has already been added */
	nr_users = ntohl(jsb->s_nr_users);
	ប្រសិនបើ (nr_users > JBD2_USERS_MAX)
		ត្រឡប់ EXT2_ET_CORRUPT_JOURNAL_SB;
	សម្រាប់ (i=0; i < nr_users; i++) {
		ប្រសិនបើ (memcmp(fs->super->s_uuid,
			   &jsb->s_users[i*16], 16) == 0)
			bha_nga;
	}
	ប្រសិនបើ (i >= nr_users) {
		memcpy(&jsb->s_users[nr_users*16],
		       fs->super->s_uuid, 16);
		jsb->s_nr_users = htonl(nr_users+1);
	}

	/* Writeback the journal superblock */
	ប្រសិនបើ ((retval = io_channel_write_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE, buf)))
		ត្រឡប់ retval;

	fs->super->s_journal_inum = 0;
	fs->super->s_journal_dev = st.st_rdev;
	memcpy(fs->super->s_journal_uuid, jsb->s_uuid,
	       parimaa_na(fs->super->s_journal_uuid));
	memset(fs->super->s_jnl_blocks, 0, parimaa_na(fs->super->s_jnl_blocks));
	ext2fs_set_feature_journal(fs->super);
	ext2fs_mark_super_dirty(fs);
	ត្រឡប់ 0;
}

/*
 * This function adds a journal inode to a filesystem, using either
 * POSIX routines if the filesystem is mounted, or using direct I/O
 * functions if it is not.
 */
errcode_t ext2fs_add_journal_inode3(ext2_filsys fs, sa_nracanaa ext2fs_journal_params *jparams,
				    blk64_t goal, ចំនួនគត់ flags)
{
	errcode_t		retval;
	ext2_ino_t		journal_ino;
	sa_nracanaa stat		st;
	var_na			jfile[1024];
	ចំនួនគត់			mount_flags;
	ចំនួនគត់			fd = -1;

	ប្រសិនបើ (flags & EXT2_MKJOURNAL_NO_MNT_CHECK)
		mount_flags = 0;
	បើមិនដូច្នេះ ប្រសិនបើ ((retval = ext2fs_check_mount_point(fs->device_name,
						    &mount_flags,
						    jfile, parimaa_na(jfile)-10)))
		ត្រឡប់ retval;

	ប្រសិនបើ (mount_flags & EXT2_MF_MOUNTED) {
#ប្រសិនបើ HAVE_EXT2_IOCTLS
		ចំនួនគត់ f = 0;
#endif
		strcat(jfile, "/.journal");

		/*
		 * If .../.journal already exists, make sure any
		 * immutable or append-only flags are cleared.
		 */
#ប្រសិនបើ defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		(void) chflags (jfile, 0);
#បើមិនដូច្នេះ
#ប្រសិនបើ HAVE_EXT2_IOCTLS
		fd = open(jfile, O_RDONLY);
		ប្រសិនបើ (fd >= 0) {
			retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
			close(fd);
			ប្រសិនបើ (retval)
				ត្រឡប់ errno;
		}
#endif
#endif

		/* Create the journal file */
		ប្រសិនបើ ((fd = open(jfile, O_CREAT|O_WRONLY, 0600)) < 0)
			ត្រឡប់ errno;

		/* Note that we can't do lazy journal initialization for mounted
		 * filesystems, since the zero writing is also allocating the
		 * journal blocks.  We could use fallocate, but not all kernels
		 * support that, and creating a journal on a mounted ext2
		 * filesystems is extremely rare these days...  Ignore it. */
		flags &= ~EXT2_MKJOURNAL_LAZYINIT;

		ប្រសិនបើ ((retval = write_journal_file(fs, jfile, jparams, flags)))
			goto errout;

		/* Get inode number of the journal file */
		ប្រសិនបើ (fstat(fd, &st) < 0) {
			retval = errno;
			goto errout;
		}

#ប្រសិនបើ defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		retval = fchflags (fd, UF_NODUMP|UF_IMMUTABLE);
#បើមិនដូច្នេះ
#ប្រសិនបើ HAVE_EXT2_IOCTLS
		ប្រសិនបើ (ioctl(fd, EXT2_IOC_GETFLAGS, &f) < 0) {
			retval = errno;
			goto errout;
		}
		f |= EXT2_NODUMP_FL | EXT2_IMMUTABLE_FL;
		retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
#endif
#endif
		ប្រសិនបើ (retval) {
			retval = errno;
			goto errout;
		}

		ប្រសិនបើ (close(fd) < 0) {
			retval = errno;
			fd = -1;
			goto errout;
		}
		journal_ino = st.st_ino;
		memset(fs->super->s_jnl_blocks, 0,
		       parimaa_na(fs->super->s_jnl_blocks));
	} បើមិនដូច្នេះ {
		ប្រសិនបើ ((mount_flags & EXT2_MF_BUSY) &&
		    !(fs->flags & EXT2_FLAG_EXCLUSIVE)) {
			retval = EBUSY;
			goto errout;
		}
		journal_ino = EXT2_JOURNAL_INO;
		ប្រសិនបើ ((retval = write_journal_inode(fs, journal_ino,
						  jparams, goal, flags)))
			ត្រឡប់ retval;
	}

	fs->super->s_journal_inum = journal_ino;
	fs->super->s_journal_dev = 0;
	memset(fs->super->s_journal_uuid, 0,
	       parimaa_na(fs->super->s_journal_uuid));
	ext2fs_set_feature_journal(fs->super);

	ext2fs_mark_super_dirty(fs);
	ត្រឡប់ 0;
errout:
	ប្រសិនបើ (fd >= 0)
		close(fd);
	ត្រឡប់ retval;
}

errcode_t ext2fs_add_journal_inode2(ext2_filsys fs, blk_t num_blocks,
				    blk64_t goal, ចំនួនគត់ flags)
{
	sa_nracanaa ext2fs_journal_params jparams;

	jparams.num_journal_blocks = num_blocks;
	jparams.num_fc_blocks = 0;

	ត្រឡប់ ext2fs_add_journal_inode3(fs, &jparams, goal, flags);
}

errcode_t ext2fs_add_journal_inode(ext2_filsys fs, blk_t num_blocks, ចំនួនគត់ flags)
{
	ត្រឡប់ ext2fs_add_journal_inode2(fs, num_blocks, ~0ULL, flags);
}


#ifdef DEBUG
main(ចំនួនគត់ argc, var_na **argv)
{
	errcode_t	retval;
	var_na		*device_name;
	ext2_filsys	fs;

	ប្រសិនបើ (argc < 2) {
		fprintf(stderr, "Usage: %s filesystem\n", argv[0]);
		exit(1);
	}
	device_name = argv[1];

	retval = ext2fs_open (device_name, EXT2_FLAG_RW, 0, 0,
			      unix_io_manager, &fs);
	ប្រសិនបើ (retval) {
		com_err(argv[0], retval, "while opening %s", device_name);
		exit(1);
	}

	retval = ext2fs_add_journal_inode(fs, JBD2_MIN_JOURNAL_BLOCKS, 0);
	ប្រសិនបើ (retval) {
		com_err(argv[0], retval, "while adding journal to %s",
			device_name);
		exit(1);
	}
	retval = ext2fs_flush(fs);
	ប្រសិនបើ (retval) {
		printf("Warning, had trouble writing out superblocks.\n");
	}
	ext2fs_close_free(&fs);
	exit(0);

}
#endif
