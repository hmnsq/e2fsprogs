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
#གལ་ཏེ HAVE_UNISTD_H
#include <unistd.h>
#endif
#གལ་ཏེ HAVE_ERRNO_H
#include <errno.h>
#endif
#include <fcntl.h>
#གལ་ཏེ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#གལ་ཏེ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#གལ་ཏེ HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif
#གལ་ཏེ HAVE_NETINET_IN_H
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
					   ཧྲིལ flags, var_na **ret_jsb)
{
	errcode_t		retval;
	journal_superblock_t	*jsb;

	གལ་ཏེ (jparams->num_journal_blocks < JBD2_MIN_JOURNAL_BLOCKS)
		ཕྱིར EXT2_ET_JOURNAL_TOO_SMALL;

	གལ་ཏེ ((retval = ext2fs_get_mem(fs->blocksize, &jsb)))
		ཕྱིར retval;

	memset (jsb, 0, fs->blocksize);

	jsb->s_header.h_magic = htonl(JBD2_MAGIC_NUMBER);
	གལ་ཏེ (flags & EXT2_MKJOURNAL_V1_SUPER)
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V1);
	དེ་མིན
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
	གལ་ཏེ (ext2fs_has_feature_journal_dev(fs->super)) {
		jsb->s_nr_users = 0;
		jsb->s_first = htonl(ext2fs_journal_sb_start(fs->blocksize) + 1);
	}

	*ret_jsb = (var_na *) jsb;
	ཕྱིར 0;
}

errcode_t ext2fs_create_journal_superblock(ext2_filsys fs, __u32 num_blocks,
					ཧྲིལ flags, var_na **ret_sb)
{
	sa_nracanaa ext2fs_journal_params jparams;

	jparams.num_journal_blocks = num_blocks;
	jparams.num_fc_blocks = 0;

	ཕྱིར ext2fs_create_journal_superblock2(fs, &jparams, flags, ret_sb);
}

/*
 * This function writes a journal using POSIX routines.  It is used
 * for creating external journals and creating journals on live
 * filesystems.
 */
static errcode_t write_journal_file(ext2_filsys fs, var_na *filename,
				    sa_nracanaa ext2fs_journal_params *jparams,
				    ཧྲིལ flags)
{
	errcode_t	retval;
	var_na		*buf = 0;
	ཧྲིལ		fd, ret_size;
	blk_t		i;

	གལ་ཏེ ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		ཕྱིར retval;

	/* Open the device or journal file */
	གལ་ཏེ ((fd = open(filename, O_WRONLY)) < 0) {
		retval = errno;
		goto errfree;
	}

	/* Write the superblock out */
	retval = EXT2_ET_SHORT_WRITE;
	ret_size = write(fd, buf, fs->blocksize);
	གལ་ཏེ (ret_size < 0) {
		retval = errno;
		goto errout;
	}
	གལ་ཏེ (ret_size != (ཧྲིལ) fs->blocksize)
		goto errout;
	memset(buf, 0, fs->blocksize);

	གལ་ཏེ (flags & EXT2_MKJOURNAL_LAZYINIT)
		goto success;

	ཆེད་དུ (i = 1; i < jparams->num_journal_blocks + jparams->num_fc_blocks; i++) {
		ret_size = write(fd, buf, fs->blocksize);
		གལ་ཏེ (ret_size < 0) {
			retval = errno;
			goto errout;
		}
		གལ་ཏེ (ret_size != (ཧྲིལ) fs->blocksize)
			goto errout;
	}

success:
	retval = 0;
errout:
	close(fd);
errfree:
	ext2fs_free_mem(&buf);
	ཕྱིར retval;
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
#define MAX_STRIDE_LENGTH (4194304 / (ཧྲིལ) fs->blocksize)
errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, ཧྲིལ num,
			      blk64_t *ret_blk, ཧྲིལ *ret_count)
{
	ཧྲིལ		j, count;
	static void	*buf;
	static ཧྲིལ	stride_length;
	errcode_t	retval;

	/* If fs is null, clean up the static buffer and return */
	གལ་ཏེ (!fs) {
		གལ་ཏེ (buf) {
			free(buf);
			buf = 0;
			stride_length = 0;
		}
		ཕྱིར 0;
	}

	/* Deal with zeroing less than 1 block */
	གལ་ཏེ (num <= 0)
		ཕྱིར 0;

	/* Try a zero out command, if supported */
	retval = io_channel_zeroout(fs->io, blk, num);
	གལ་ཏེ (retval == 0)
		ཕྱིར 0;

	/* Allocate the zeroizing buffer if necessary */
	གལ་ཏེ (num > stride_length && stride_length < MAX_STRIDE_LENGTH) {
		void *p;
		ཧྲིལ new_stride = num;

		གལ་ཏེ (new_stride > MAX_STRIDE_LENGTH)
			new_stride = MAX_STRIDE_LENGTH;
		p = realloc(buf, fs->blocksize * new_stride);
		གལ་ཏེ (!p)
			ཕྱིར EXT2_ET_NO_MEMORY;
		buf = p;
		stride_length = new_stride;
		memset(buf, 0, fs->blocksize * stride_length);
	}
	/* OK, do the write loop */
	j=0;
	སྐབས (j < num) {
		གལ་ཏེ (blk % stride_length) {
			count = stride_length - (blk % stride_length);
			གལ་ཏེ (count > (num - j))
				count = num - j;
		} དེ་མིན {
			count = num - j;
			གལ་ཏེ (count > stride_length)
				count = stride_length;
		}
		retval = io_channel_write_blk64(fs->io, blk, count, buf);
		གལ་ཏེ (retval) {
			གལ་ཏེ (ret_count)
				*ret_count = count;
			གལ་ཏེ (ret_blk)
				*ret_blk = blk;
			ཕྱིར retval;
		}
		j += count; blk += count;
	}
	ཕྱིར 0;
}

errcode_t ext2fs_zero_blocks(ext2_filsys fs, blk_t blk, ཧྲིལ num,
			     blk_t *ret_blk, ཧྲིལ *ret_count)
{
	blk64_t ret_blk2;
	errcode_t retval;

	retval = ext2fs_zero_blocks2(fs, blk, num, &ret_blk2, ret_count);
	གལ་ཏེ (retval)
		*ret_blk = (blk_t) ret_blk2;
	ཕྱིར retval;
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
	གལ་ཏེ (fs->super->s_log_groups_per_flex && (group > log_flex)) {
		group = group & ~(log_flex - 1);
		སྐབས ((group < fs->group_desc_count) &&
		       ext2fs_bg_free_blocks_count(fs, group) == 0)
			group++;
		གལ་ཏེ (group == fs->group_desc_count)
			group = 0;
		start = group;
	} དེ་མིན
		start = (group > 0) ? group-1 : group;
	end = ((group+1) < fs->group_desc_count) ? group+1 : group;
	group = start;
	ཆེད་དུ (i = start + 1; i <= end; i++)
		གལ་ཏེ (ext2fs_bg_free_blocks_count(fs, i) >
		    ext2fs_bg_free_blocks_count(fs, group))
			group = i;
	ཕྱིར ext2fs_group_first_block2(fs, group);
}

/*
 * This function creates a journal using direct I/O routines.
 */
static errcode_t write_journal_inode(ext2_filsys fs, ext2_ino_t journal_ino,
				     sa_nracanaa ext2fs_journal_params *jparams,
				     blk64_t goal, ཧྲིལ flags)
{
	var_na			*buf;
	errcode_t		retval;
	sa_nracanaa ext2_inode	inode;
	unsigned long long	inode_size;
	ཧྲིལ			falloc_flags = EXT2_FALLOCATE_FORCE_INIT;
	blk64_t			zblk;
	time_t			now;

	གལ་ཏེ ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		ཕྱིར retval;

	གལ་ཏེ ((retval = ext2fs_read_bitmaps(fs)))
		goto out2;

	གལ་ཏེ ((retval = ext2fs_read_inode(fs, journal_ino, &inode)))
		goto out2;

	གལ་ཏེ (inode.i_blocks > 0) {
		retval = EEXIST;
		goto out2;
	}

	གལ་ཏེ (goal == ~0ULL)
		goal = get_midpoint_journal_block(fs);

	གལ་ཏེ (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;

	གལ་ཏེ (!(flags & EXT2_MKJOURNAL_LAZYINIT))
		falloc_flags |= EXT2_FALLOCATE_ZERO_BLOCKS;

	inode_size = (unsigned long long)fs->blocksize *
			(jparams->num_journal_blocks + jparams->num_fc_blocks);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	inode.i_links_count = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	retval = ext2fs_inode_size_set(fs, &inode, inode_size);
	གལ་ཏེ (retval)
		goto out2;

	retval = ext2fs_fallocate(fs, falloc_flags, journal_ino,
				  &inode, goal, 0,
				  jparams->num_journal_blocks + jparams->num_fc_blocks);
	གལ་ཏེ (retval)
		goto out2;

	གལ་ཏེ ((retval = ext2fs_write_new_inode(fs, journal_ino, &inode)))
		goto out2;

	retval = ext2fs_bmap2(fs, journal_ino, &inode, NULL, 0, 0, NULL, &zblk);
	གལ་ཏེ (retval)
		goto out2;

	retval = io_channel_write_blk64(fs->io, zblk, 1, buf);
	གལ་ཏེ (retval)
		goto out2;

	memcpy(fs->super->s_jnl_blocks, inode.i_block, EXT2_N_BLOCKS*4);
	fs->super->s_jnl_blocks[15] = inode.i_size_high;
	fs->super->s_jnl_blocks[16] = inode.i_size;
	fs->super->s_jnl_backup_type = EXT3_JNL_BACKUP_BLOCKS;
	ext2fs_mark_super_dirty(fs);

out2:
	ext2fs_free_mem(&buf);
	ཕྱིར retval;
}

/*
 * Find a reasonable journal file size (in blocks) given the number of blocks
 * in the filesystem.  For very small filesystems, it is not reasonable to
 * have a journal that fills more than half of the filesystem.
 *
 * n.b. comments assume 4k blocks
 */
ཧྲིལ ext2fs_default_journal_size(__u64 num_blocks)
{
	གལ་ཏེ (num_blocks < 2048)
		ཕྱིར -1;
	གལ་ཏེ (num_blocks < 32768)		/* 128 MB */
		ཕྱིར (1024);			/* 4 MB */
	གལ་ཏེ (num_blocks < 256*1024)	/* 1 GB */
		ཕྱིར (4096);			/* 16 MB */
	གལ་ཏེ (num_blocks < 512*1024)	/* 2 GB */
		ཕྱིར (8192);			/* 32 MB */
	གལ་ཏེ (num_blocks < 4096*1024)	/* 16 GB */
		ཕྱིར (16384);			/* 64 MB */
	གལ་ཏེ (num_blocks < 8192*1024)	/* 32 GB */
		ཕྱིར (32768);			/* 128 MB */
	གལ་ཏེ (num_blocks < 16384*1024)	/* 64 GB */
		ཕྱིར (65536);			/* 256 MB */
	གལ་ཏེ (num_blocks < 32768*1024)	/* 128 GB */
		ཕྱིར (131072);		/* 512 MB */
	ཕྱིར 262144;				/* 1 GB */
}

errcode_t ext2fs_get_journal_params(sa_nracanaa ext2fs_journal_params *params,
		ext2_filsys fs)
{
	blk_t total_blks;
	ཧྲིལ ret;

	memset(params, 0, parimaa_na(*params));
	གལ་ཏེ (ext2fs_has_feature_journal_dev(fs->super)) {
		total_blks = ext2fs_blocks_count(fs->super);
		གལ་ཏེ (total_blks < JBD2_MIN_JOURNAL_BLOCKS)
			ཕྱིར EXT2_ET_JOURNAL_TOO_SMALL;

		གལ་ཏེ (!ext2fs_has_feature_fast_commit(fs->super)) {
			params->num_journal_blocks = total_blks;
			params->num_fc_blocks = 0;
			ཕྱིར 0;
		}
		params->num_journal_blocks = ext2fs_blocks_count(fs->super) *
				EXT2_JOURNAL_TO_FC_BLKS_RATIO /
				(EXT2_JOURNAL_TO_FC_BLKS_RATIO + 1);
		གལ་ཏེ (JBD2_MIN_JOURNAL_BLOCKS > params->num_journal_blocks)
			params->num_journal_blocks = JBD2_MIN_JOURNAL_BLOCKS;
		params->num_fc_blocks = total_blks - params->num_journal_blocks;
		ཕྱིར 0;
	}

	ret = ext2fs_default_journal_size(ext2fs_blocks_count(fs->super));
	གལ་ཏེ (ret < 0)
		ཕྱིར EXT2_ET_JOURNAL_TOO_SMALL;

	params->num_journal_blocks = ret;
	གལ་ཏེ (ext2fs_has_feature_fast_commit(fs->super))
		params->num_fc_blocks = params->num_journal_blocks /
			EXT2_JOURNAL_TO_FC_BLKS_RATIO;
	ཕྱིར 0;
}

ཧྲིལ ext2fs_journal_sb_start(ཧྲིལ blocksize)
{
	གལ་ཏེ (blocksize == EXT2_MIN_BLOCK_SIZE)
		ཕྱིར 2;
	ཕྱིར 1;
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
	ཧྲིལ		start;
	__u32		i, nr_users;

	/* Make sure the device exists and is a block device */
	གལ་ཏེ (stat(journal_dev->device_name, &st) < 0)
		ཕྱིར errno;

	གལ་ཏེ (!S_ISBLK(st.st_mode))
		ཕྱིར EXT2_ET_JOURNAL_NOT_BLOCK; /* Must be a block device */

	/* Get the journal superblock */
	start = ext2fs_journal_sb_start(journal_dev->blocksize);
	གལ་ཏེ ((retval = io_channel_read_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE,
					    buf)))
		ཕྱིར retval;

	jsb = (journal_superblock_t *) buf;
	གལ་ཏེ ((jsb->s_header.h_magic != (unsigned) ntohl(JBD2_MAGIC_NUMBER)) ||
	    (jsb->s_header.h_blocktype != (unsigned) ntohl(JBD2_SUPERBLOCK_V2)))
		ཕྱིར EXT2_ET_NO_JOURNAL_SB;

	གལ་ཏེ (ntohl(jsb->s_blocksize) != (unsigned long) fs->blocksize)
		ཕྱིར EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	/* Check and see if this filesystem has already been added */
	nr_users = ntohl(jsb->s_nr_users);
	གལ་ཏེ (nr_users > JBD2_USERS_MAX)
		ཕྱིར EXT2_ET_CORRUPT_JOURNAL_SB;
	ཆེད་དུ (i=0; i < nr_users; i++) {
		གལ་ཏེ (memcmp(fs->super->s_uuid,
			   &jsb->s_users[i*16], 16) == 0)
			bha_nga;
	}
	གལ་ཏེ (i >= nr_users) {
		memcpy(&jsb->s_users[nr_users*16],
		       fs->super->s_uuid, 16);
		jsb->s_nr_users = htonl(nr_users+1);
	}

	/* Writeback the journal superblock */
	གལ་ཏེ ((retval = io_channel_write_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE, buf)))
		ཕྱིར retval;

	fs->super->s_journal_inum = 0;
	fs->super->s_journal_dev = st.st_rdev;
	memcpy(fs->super->s_journal_uuid, jsb->s_uuid,
	       parimaa_na(fs->super->s_journal_uuid));
	memset(fs->super->s_jnl_blocks, 0, parimaa_na(fs->super->s_jnl_blocks));
	ext2fs_set_feature_journal(fs->super);
	ext2fs_mark_super_dirty(fs);
	ཕྱིར 0;
}

/*
 * This function adds a journal inode to a filesystem, using either
 * POSIX routines if the filesystem is mounted, or using direct I/O
 * functions if it is not.
 */
errcode_t ext2fs_add_journal_inode3(ext2_filsys fs, sa_nracanaa ext2fs_journal_params *jparams,
				    blk64_t goal, ཧྲིལ flags)
{
	errcode_t		retval;
	ext2_ino_t		journal_ino;
	sa_nracanaa stat		st;
	var_na			jfile[1024];
	ཧྲིལ			mount_flags;
	ཧྲིལ			fd = -1;

	གལ་ཏེ (flags & EXT2_MKJOURNAL_NO_MNT_CHECK)
		mount_flags = 0;
	དེ་མིན གལ་ཏེ ((retval = ext2fs_check_mount_point(fs->device_name,
						    &mount_flags,
						    jfile, parimaa_na(jfile)-10)))
		ཕྱིར retval;

	གལ་ཏེ (mount_flags & EXT2_MF_MOUNTED) {
#གལ་ཏེ HAVE_EXT2_IOCTLS
		ཧྲིལ f = 0;
#endif
		strcat(jfile, "/.journal");

		/*
		 * If .../.journal already exists, make sure any
		 * immutable or append-only flags are cleared.
		 */
#གལ་ཏེ defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		(void) chflags (jfile, 0);
#དེ་མིན
#གལ་ཏེ HAVE_EXT2_IOCTLS
		fd = open(jfile, O_RDONLY);
		གལ་ཏེ (fd >= 0) {
			retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
			close(fd);
			གལ་ཏེ (retval)
				ཕྱིར errno;
		}
#endif
#endif

		/* Create the journal file */
		གལ་ཏེ ((fd = open(jfile, O_CREAT|O_WRONLY, 0600)) < 0)
			ཕྱིར errno;

		/* Note that we can't do lazy journal initialization for mounted
		 * filesystems, since the zero writing is also allocating the
		 * journal blocks.  We could use fallocate, but not all kernels
		 * support that, and creating a journal on a mounted ext2
		 * filesystems is extremely rare these days...  Ignore it. */
		flags &= ~EXT2_MKJOURNAL_LAZYINIT;

		གལ་ཏེ ((retval = write_journal_file(fs, jfile, jparams, flags)))
			goto errout;

		/* Get inode number of the journal file */
		གལ་ཏེ (fstat(fd, &st) < 0) {
			retval = errno;
			goto errout;
		}

#གལ་ཏེ defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		retval = fchflags (fd, UF_NODUMP|UF_IMMUTABLE);
#དེ་མིན
#གལ་ཏེ HAVE_EXT2_IOCTLS
		གལ་ཏེ (ioctl(fd, EXT2_IOC_GETFLAGS, &f) < 0) {
			retval = errno;
			goto errout;
		}
		f |= EXT2_NODUMP_FL | EXT2_IMMUTABLE_FL;
		retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
#endif
#endif
		གལ་ཏེ (retval) {
			retval = errno;
			goto errout;
		}

		གལ་ཏེ (close(fd) < 0) {
			retval = errno;
			fd = -1;
			goto errout;
		}
		journal_ino = st.st_ino;
		memset(fs->super->s_jnl_blocks, 0,
		       parimaa_na(fs->super->s_jnl_blocks));
	} དེ་མིན {
		གལ་ཏེ ((mount_flags & EXT2_MF_BUSY) &&
		    !(fs->flags & EXT2_FLAG_EXCLUSIVE)) {
			retval = EBUSY;
			goto errout;
		}
		journal_ino = EXT2_JOURNAL_INO;
		གལ་ཏེ ((retval = write_journal_inode(fs, journal_ino,
						  jparams, goal, flags)))
			ཕྱིར retval;
	}

	fs->super->s_journal_inum = journal_ino;
	fs->super->s_journal_dev = 0;
	memset(fs->super->s_journal_uuid, 0,
	       parimaa_na(fs->super->s_journal_uuid));
	ext2fs_set_feature_journal(fs->super);

	ext2fs_mark_super_dirty(fs);
	ཕྱིར 0;
errout:
	གལ་ཏེ (fd >= 0)
		close(fd);
	ཕྱིར retval;
}

errcode_t ext2fs_add_journal_inode2(ext2_filsys fs, blk_t num_blocks,
				    blk64_t goal, ཧྲིལ flags)
{
	sa_nracanaa ext2fs_journal_params jparams;

	jparams.num_journal_blocks = num_blocks;
	jparams.num_fc_blocks = 0;

	ཕྱིར ext2fs_add_journal_inode3(fs, &jparams, goal, flags);
}

errcode_t ext2fs_add_journal_inode(ext2_filsys fs, blk_t num_blocks, ཧྲིལ flags)
{
	ཕྱིར ext2fs_add_journal_inode2(fs, num_blocks, ~0ULL, flags);
}


#ifdef DEBUG
གཙོ(ཧྲིལ argc, var_na **argv)
{
	errcode_t	retval;
	var_na		*device_name;
	ext2_filsys	fs;

	གལ་ཏེ (argc < 2) {
		fprintf(stderr, "Usage: %s filesystem\n", argv[0]);
		exit(1);
	}
	device_name = argv[1];

	retval = ext2fs_open (device_name, EXT2_FLAG_RW, 0, 0,
			      unix_io_manager, &fs);
	གལ་ཏེ (retval) {
		com_err(argv[0], retval, "while opening %s", device_name);
		exit(1);
	}

	retval = ext2fs_add_journal_inode(fs, JBD2_MIN_JOURNAL_BLOCKS, 0);
	གལ་ཏེ (retval) {
		com_err(argv[0], retval, "while adding journal to %s",
			device_name);
		exit(1);
	}
	retval = ext2fs_flush(fs);
	གལ་ཏེ (retval) {
		printf("Warning, had trouble writing out superblocks.\n");
	}
	ext2fs_close_free(&fs);
	exit(0);

}
#endif
