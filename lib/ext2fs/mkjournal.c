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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଯଦି HAVE_ERRNO_H
#ଅନ୍ତର୍ଭାଵ <errno.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif
#ଯଦି HAVE_SYS_IOCTL_H
#ଅନ୍ତର୍ଭାଵ <sys/ioctl.h>
#endif
#ଯଦି HAVE_NETINET_IN_H
#ଅନ୍ତର୍ଭାଵ <netinet/in.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "e2p/e2p.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

#ଅନ୍ତର୍ଭାଵ "kernel-jbd.h"

/*
 * This function automatically sets up the journal superblock and
 * returns it as an allocated block.
 */
errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
					   ସଂରଚନା ext2fs_journal_params *jparams,
					   ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ଅକ୍ଷର **ret_jsb)
{
	errcode_t		retval;
	journal_superblock_t	*jsb;

	ଯଦି (jparams->ଅଙ୍କ_journal_blocks < JBD2_MIN_JOURNAL_BLOCKS)
		ଫେରନ୍ତୁ EXT2_ET_JOURNAL_TOO_SMALL;

	ଯଦି ((retval = ext2fs_get_mem(fs->blocksize, &jsb)))
		ଫେରନ୍ତୁ retval;

	memset (jsb, 0, fs->blocksize);

	jsb->s_header.h_magic = htonl(JBD2_MAGIC_ସଂଖ୍ୟା);
	ଯଦି (flags & EXT2_MKJOURNAL_V1_SUPER)
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V1);
	ଅନ୍ୟଥା
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V2);
	jsb->s_blocksize = htonl(fs->blocksize);
	jsb->s_maxlen = htonl(jparams->ଅଙ୍କ_journal_blocks + jparams->ଅଙ୍କ_fc_blocks);
	jsb->s_nr_users = htonl(1);
	jsb->s_first = htonl(1);
	jsb->s_sequence = htonl(1);
	jsb->s_ଅଙ୍କ_fc_blks = htonl(jparams->ଅଙ୍କ_fc_blocks);
	memcpy(jsb->s_uuid, fs->super->s_uuid, ଆକାର(fs->super->s_uuid));
	/*
	 * If we're creating an external journal device, we need to
	 * adjust these fields.
	 */
	ଯଦି (ext2fs_has_feature_journal_dev(fs->super)) {
		jsb->s_nr_users = 0;
		jsb->s_first = htonl(ext2fs_journal_sb_start(fs->blocksize) + 1);
	}

	*ret_jsb = (ଅକ୍ଷର *) jsb;
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_create_journal_superblock(ext2_filsys fs, __u32 ଅଙ୍କ_blocks,
					ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ଅକ୍ଷର **ret_sb)
{
	ସଂରଚନା ext2fs_journal_params jparams;

	jparams.ଅଙ୍କ_journal_blocks = ଅଙ୍କ_blocks;
	jparams.ଅଙ୍କ_fc_blocks = 0;

	ଫେରନ୍ତୁ ext2fs_create_journal_superblock2(fs, &jparams, flags, ret_sb);
}

/*
 * This function writes a journal using POSIX routines.  It is used
 * for creating external journals and creating journals on live
 * filesystems.
 */
ସ୍ଥିର errcode_t write_journal_file(ext2_filsys fs, ଅକ୍ଷର *filename,
				    ସଂରଚନା ext2fs_journal_params *jparams,
				    ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	errcode_t	retval;
	ଅକ୍ଷର		*buf = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		fd, ret_size;
	blk_t		i;

	ଯଦି ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		ଫେରନ୍ତୁ retval;

	/* Open the device or journal file */
	ଯଦି ((fd = open(filename, O_WRONLY)) < 0) {
		retval = errno;
		ଯାଅ errfree;
	}

	/* Write the superblock out */
	retval = EXT2_ET_SHORT_WRITE;
	ret_size = ଲିଖ(fd, buf, fs->blocksize);
	ଯଦି (ret_size < 0) {
		retval = errno;
		ଯାଅ errout;
	}
	ଯଦି (ret_size != (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) fs->blocksize)
		ଯାଅ errout;
	memset(buf, 0, fs->blocksize);

	ଯଦି (flags & EXT2_MKJOURNAL_LAZYINIT)
		ଯାଅ success;

	ପାଇଁ (i = 1; i < jparams->ଅଙ୍କ_journal_blocks + jparams->ଅଙ୍କ_fc_blocks; i++) {
		ret_size = ଲିଖ(fd, buf, fs->blocksize);
		ଯଦି (ret_size < 0) {
			retval = errno;
			ଯାଅ errout;
		}
		ଯଦି (ret_size != (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) fs->blocksize)
			ଯାଅ errout;
	}

success:
	retval = 0;
errout:
	close(fd);
errfree:
	ext2fs_free_mem(&buf);
	ଫେରନ୍ତୁ retval;
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
#define MAX_STRIDE_LENGTH (4194304 / (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) fs->blocksize)
errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ,
			      blk64_t *ret_blk, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ret_ଗଣନା)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		j, ଗଣନା_2;
	ସ୍ଥିର ଶୂନ୍ୟ	*buf;
	ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା	stride_length;
	errcode_t	retval;

	/* If fs is null, clean up the static buffer and return */
	ଯଦି (!fs) {
		ଯଦି (buf) {
			ମୁକ୍ତ(buf);
			buf = 0;
			stride_length = 0;
		}
		ଫେରନ୍ତୁ 0;
	}

	/* Deal with zeroing less than 1 block */
	ଯଦି (ଅଙ୍କ <= 0)
		ଫେରନ୍ତୁ 0;

	/* Try a zero out command, if supported */
	retval = io_channel_zeroout(fs->io, blk, ଅଙ୍କ);
	ଯଦି (retval == 0)
		ଫେରନ୍ତୁ 0;

	/* Allocate the zeroizing buffer if necessary */
	ଯଦି (ଅଙ୍କ > stride_length && stride_length < MAX_STRIDE_LENGTH) {
		ଶୂନ୍ୟ *p;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_stride = ଅଙ୍କ;

		ଯଦି (new_stride > MAX_STRIDE_LENGTH)
			new_stride = MAX_STRIDE_LENGTH;
		p = realloc(buf, fs->blocksize * new_stride);
		ଯଦି (!p)
			ଫେରନ୍ତୁ EXT2_ET_NO_MEMORY;
		buf = p;
		stride_length = new_stride;
		memset(buf, 0, fs->blocksize * stride_length);
	}
	/* OK, do the write loop */
	j=0;
	ଯେତେବେଳେ (j < ଅଙ୍କ) {
		ଯଦି (blk % stride_length) {
			ଗଣନା_2 = stride_length - (blk % stride_length);
			ଯଦି (ଗଣନା_2 > (ଅଙ୍କ - j))
				ଗଣନା_2 = ଅଙ୍କ - j;
		} ଅନ୍ୟଥା {
			ଗଣନା_2 = ଅଙ୍କ - j;
			ଯଦି (ଗଣନା_2 > stride_length)
				ଗଣନା_2 = stride_length;
		}
		retval = io_channel_write_blk64(fs->io, blk, ଗଣନା_2, buf);
		ଯଦି (retval) {
			ଯଦି (ret_ଗଣନା)
				*ret_ଗଣନା = ଗଣନା_2;
			ଯଦି (ret_blk)
				*ret_blk = blk;
			ଫେରନ୍ତୁ retval;
		}
		j += ଗଣନା_2; blk += ଗଣନା_2;
	}
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_zero_blocks(ext2_filsys fs, blk_t blk, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ,
			     blk_t *ret_blk, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ret_ଗଣନା)
{
	blk64_t ret_blk2;
	errcode_t retval;

	retval = ext2fs_zero_blocks2(fs, blk, ଅଙ୍କ, &ret_blk2, ret_ଗଣନା);
	ଯଦି (retval)
		*ret_blk = (blk_t) ret_blk2;
	ଫେରନ୍ତୁ retval;
}

/*
 * Calculate the initial goal block to be roughly at the middle of the
 * filesystem.  Pick a group that has the largest number of free
 * blocks.
 */
ସ୍ଥିର blk64_t get_midpoint_journal_block(ext2_filsys fs)
{
	dgrp_t	group, start, end, i, log_flex;

	group = ext2fs_group_of_blk2(fs, (ext2fs_blocks_ଗଣନା(fs->super) -
					 fs->super->s_first_data_block) / 2);
	log_flex = 1U << fs->super->s_log_groups_per_flex;
	ଯଦି (fs->super->s_log_groups_per_flex && (group > log_flex)) {
		group = group & ~(log_flex - 1);
		ଯେତେବେଳେ ((group < fs->group_desc_ଗଣନା) &&
		       ext2fs_bg_free_blocks_ଗଣନା(fs, group) == 0)
			group++;
		ଯଦି (group == fs->group_desc_ଗଣନା)
			group = 0;
		start = group;
	} ଅନ୍ୟଥା
		start = (group > 0) ? group-1 : group;
	end = ((group+1) < fs->group_desc_ଗଣନା) ? group+1 : group;
	group = start;
	ପାଇଁ (i = start + 1; i <= end; i++)
		ଯଦି (ext2fs_bg_free_blocks_ଗଣନା(fs, i) >
		    ext2fs_bg_free_blocks_ଗଣନା(fs, group))
			group = i;
	ଫେରନ୍ତୁ ext2fs_group_first_block2(fs, group);
}

/*
 * This function creates a journal using direct I/O routines.
 */
ସ୍ଥିର errcode_t write_journal_inode(ext2_filsys fs, ext2_ino_t journal_ino,
				     ସଂରଚନା ext2fs_journal_params *jparams,
				     blk64_t goal, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ଅକ୍ଷର			*buf;
	errcode_t		retval;
	ସଂରଚନା ext2_inode	inode;
	unsigned long long	inode_size;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			falloc_flags = EXT2_FALLOCATE_FORCE_INIT;
	blk64_t			zblk;
	time_t			now;

	ଯଦି ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		ଫେରନ୍ତୁ retval;

	ଯଦି ((retval = ext2fs_read_bitmaps(fs)))
		ଯାଅ out2;

	ଯଦି ((retval = ext2fs_read_inode(fs, journal_ino, &inode)))
		ଯାଅ out2;

	ଯଦି (inode.i_blocks > 0) {
		retval = EEXIST;
		ଯାଅ out2;
	}

	ଯଦି (goal == ~0ULL)
		goal = get_midpoint_journal_block(fs);

	ଯଦି (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;

	ଯଦି (!(flags & EXT2_MKJOURNAL_LAZYINIT))
		falloc_flags |= EXT2_FALLOCATE_ZERO_BLOCKS;

	inode_size = (unsigned long long)fs->blocksize *
			(jparams->ଅଙ୍କ_journal_blocks + jparams->ଅଙ୍କ_fc_blocks);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	inode.i_links_ଗଣନା = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	retval = ext2fs_inode_size_set(fs, &inode, inode_size);
	ଯଦି (retval)
		ଯାଅ out2;

	retval = ext2fs_fallocate(fs, falloc_flags, journal_ino,
				  &inode, goal, 0,
				  jparams->ଅଙ୍କ_journal_blocks + jparams->ଅଙ୍କ_fc_blocks);
	ଯଦି (retval)
		ଯାଅ out2;

	ଯଦି ((retval = ext2fs_write_new_inode(fs, journal_ino, &inode)))
		ଯାଅ out2;

	retval = ext2fs_bmap2(fs, journal_ino, &inode, ଖାଲି, 0, 0, ଖାଲି, &zblk);
	ଯଦି (retval)
		ଯାଅ out2;

	retval = io_channel_write_blk64(fs->io, zblk, 1, buf);
	ଯଦି (retval)
		ଯାଅ out2;

	memcpy(fs->super->s_jnl_blocks, inode.i_block, EXT2_N_BLOCKS*4);
	fs->super->s_jnl_blocks[15] = inode.i_size_ଉପର;
	fs->super->s_jnl_blocks[16] = inode.i_size;
	fs->super->s_jnl_backup_type = EXT3_JNL_BACKUP_BLOCKS;
	ext2fs_mark_super_dirty(fs);

out2:
	ext2fs_free_mem(&buf);
	ଫେରନ୍ତୁ retval;
}

/*
 * Find a reasonable journal file size (in blocks) given the number of blocks
 * in the filesystem.  For very small filesystems, it is not reasonable to
 * have a journal that fills more than half of the filesystem.
 *
 * n.b. comments assume 4k blocks
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_default_journal_size(__u64 ଅଙ୍କ_blocks)
{
	ଯଦି (ଅଙ୍କ_blocks < 2048)
		ଫେରନ୍ତୁ -1;
	ଯଦି (ଅଙ୍କ_blocks < 32768)		/* 128 MB */
		ଫେରନ୍ତୁ (1024);			/* 4 MB */
	ଯଦି (ଅଙ୍କ_blocks < 256*1024)	/* 1 GB */
		ଫେରନ୍ତୁ (4096);			/* 16 MB */
	ଯଦି (ଅଙ୍କ_blocks < 512*1024)	/* 2 GB */
		ଫେରନ୍ତୁ (8192);			/* 32 MB */
	ଯଦି (ଅଙ୍କ_blocks < 4096*1024)	/* 16 GB */
		ଫେରନ୍ତୁ (16384);			/* 64 MB */
	ଯଦି (ଅଙ୍କ_blocks < 8192*1024)	/* 32 GB */
		ଫେରନ୍ତୁ (32768);			/* 128 MB */
	ଯଦି (ଅଙ୍କ_blocks < 16384*1024)	/* 64 GB */
		ଫେରନ୍ତୁ (65536);			/* 256 MB */
	ଯଦି (ଅଙ୍କ_blocks < 32768*1024)	/* 128 GB */
		ଫେରନ୍ତୁ (131072);		/* 512 MB */
	ଫେରନ୍ତୁ 262144;				/* 1 GB */
}

errcode_t ext2fs_get_journal_params(ସଂରଚନା ext2fs_journal_params *params,
		ext2_filsys fs)
{
	blk_t total_blks;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	memset(params, 0, ଆକାର(*params));
	ଯଦି (ext2fs_has_feature_journal_dev(fs->super)) {
		total_blks = ext2fs_blocks_ଗଣନା(fs->super);
		ଯଦି (total_blks < JBD2_MIN_JOURNAL_BLOCKS)
			ଫେରନ୍ତୁ EXT2_ET_JOURNAL_TOO_SMALL;

		ଯଦି (!ext2fs_has_feature_fast_commit(fs->super)) {
			params->ଅଙ୍କ_journal_blocks = total_blks;
			params->ଅଙ୍କ_fc_blocks = 0;
			ଫେରନ୍ତୁ 0;
		}
		params->ଅଙ୍କ_journal_blocks = ext2fs_blocks_ଗଣନା(fs->super) *
				EXT2_JOURNAL_TO_FC_BLKS_RATIO /
				(EXT2_JOURNAL_TO_FC_BLKS_RATIO + 1);
		ଯଦି (JBD2_MIN_JOURNAL_BLOCKS > params->ଅଙ୍କ_journal_blocks)
			params->ଅଙ୍କ_journal_blocks = JBD2_MIN_JOURNAL_BLOCKS;
		params->ଅଙ୍କ_fc_blocks = total_blks - params->ଅଙ୍କ_journal_blocks;
		ଫେରନ୍ତୁ 0;
	}

	ret = ext2fs_default_journal_size(ext2fs_blocks_ଗଣନା(fs->super));
	ଯଦି (ret < 0)
		ଫେରନ୍ତୁ EXT2_ET_JOURNAL_TOO_SMALL;

	params->ଅଙ୍କ_journal_blocks = ret;
	ଯଦି (ext2fs_has_feature_fast_commit(fs->super))
		params->ଅଙ୍କ_fc_blocks = params->ଅଙ୍କ_journal_blocks /
			EXT2_JOURNAL_TO_FC_BLKS_RATIO;
	ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_journal_sb_start(ପୂର୍ଣ୍ଣସଂଖ୍ୟା blocksize)
{
	ଯଦି (blocksize == EXT2_MIN_BLOCK_SIZE)
		ଫେରନ୍ତୁ 2;
	ଫେରନ୍ତୁ 1;
}

/*
 * This function adds a journal device to a filesystem
 */
errcode_t ext2fs_add_journal_device(ext2_filsys fs, ext2_filsys journal_dev)
{
	ସଂରଚନା stat	st;
	errcode_t	retval;
	ଅକ୍ଷର		buf[SUPERBLOCK_SIZE];
	journal_superblock_t	*jsb;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		start;
	__u32		i, nr_users;

	/* Make sure the device exists and is a block device */
	ଯଦି (stat(journal_dev->device_name, &st) < 0)
		ଫେରନ୍ତୁ errno;

	ଯଦି (!S_ISBLK(st.st_mode))
		ଫେରନ୍ତୁ EXT2_ET_JOURNAL_NOT_BLOCK; /* Must be a block device */

	/* Get the journal superblock */
	start = ext2fs_journal_sb_start(journal_dev->blocksize);
	ଯଦି ((retval = io_channel_read_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE,
					    buf)))
		ଫେରନ୍ତୁ retval;

	jsb = (journal_superblock_t *) buf;
	ଯଦି ((jsb->s_header.h_magic != (unsigned) ntohl(JBD2_MAGIC_ସଂଖ୍ୟା)) ||
	    (jsb->s_header.h_blocktype != (unsigned) ntohl(JBD2_SUPERBLOCK_V2)))
		ଫେରନ୍ତୁ EXT2_ET_NO_JOURNAL_SB;

	ଯଦି (ntohl(jsb->s_blocksize) != (unsigned long) fs->blocksize)
		ଫେରନ୍ତୁ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	/* Check and see if this filesystem has already been added */
	nr_users = ntohl(jsb->s_nr_users);
	ଯଦି (nr_users > JBD2_USERS_MAX)
		ଫେରନ୍ତୁ EXT2_ET_CORRUPT_JOURNAL_SB;
	ପାଇଁ (i=0; i < nr_users; i++) {
		ଯଦି (memcmp(fs->super->s_uuid,
			   &jsb->s_users[i*16], 16) == 0)
			ଭାଙ୍ଗ;
	}
	ଯଦି (i >= nr_users) {
		memcpy(&jsb->s_users[nr_users*16],
		       fs->super->s_uuid, 16);
		jsb->s_nr_users = htonl(nr_users+1);
	}

	/* Writeback the journal superblock */
	ଯଦି ((retval = io_channel_write_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE, buf)))
		ଫେରନ୍ତୁ retval;

	fs->super->s_journal_inum = 0;
	fs->super->s_journal_dev = st.st_rdev;
	memcpy(fs->super->s_journal_uuid, jsb->s_uuid,
	       ଆକାର(fs->super->s_journal_uuid));
	memset(fs->super->s_jnl_blocks, 0, ଆକାର(fs->super->s_jnl_blocks));
	ext2fs_set_feature_journal(fs->super);
	ext2fs_mark_super_dirty(fs);
	ଫେରନ୍ତୁ 0;
}

/*
 * This function adds a journal inode to a filesystem, using either
 * POSIX routines if the filesystem is mounted, or using direct I/O
 * functions if it is not.
 */
errcode_t ext2fs_add_journal_inode3(ext2_filsys fs, ସଂରଚନା ext2fs_journal_params *jparams,
				    blk64_t goal, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	errcode_t		retval;
	ext2_ino_t		journal_ino;
	ସଂରଚନା stat		st;
	ଅକ୍ଷର			jfile[1024];
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			mount_flags;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			fd = -1;

	ଯଦି (flags & EXT2_MKJOURNAL_NO_MNT_CHECK)
		mount_flags = 0;
	ଅନ୍ୟଥା ଯଦି ((retval = ext2fs_check_mount_point(fs->device_name,
						    &mount_flags,
						    jfile, ଆକାର(jfile)-10)))
		ଫେରନ୍ତୁ retval;

	ଯଦି (mount_flags & EXT2_MF_MOUNTED) {
#ଯଦି HAVE_EXT2_IOCTLS
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା f = 0;
#endif
		strcat(jfile, "/.journal");

		/*
		 * If .../.journal already exists, make sure any
		 * immutable or append-only flags are cleared.
		 */
#ଯଦି defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		(ଶୂନ୍ୟ) chflags (jfile, 0);
#ଅନ୍ୟଥା
#ଯଦି HAVE_EXT2_IOCTLS
		fd = open(jfile, O_RDONLY);
		ଯଦି (fd >= 0) {
			retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
			close(fd);
			ଯଦି (retval)
				ଫେରନ୍ତୁ errno;
		}
#endif
#endif

		/* Create the journal file */
		ଯଦି ((fd = open(jfile, O_CREAT|O_WRONLY, 0600)) < 0)
			ଫେରନ୍ତୁ errno;

		/* Note that we can't do lazy journal initialization for mounted
		 * filesystems, since the zero writing is also allocating the
		 * journal blocks.  We could use fallocate, but not all kernels
		 * support that, and creating a journal on a mounted ext2
		 * filesystems is extremely rare these days...  Ignore it. */
		flags &= ~EXT2_MKJOURNAL_LAZYINIT;

		ଯଦି ((retval = write_journal_file(fs, jfile, jparams, flags)))
			ଯାଅ errout;

		/* Get inode number of the journal file */
		ଯଦି (fstat(fd, &st) < 0) {
			retval = errno;
			ଯାଅ errout;
		}

#ଯଦି defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		retval = fchflags (fd, UF_NODUMP|UF_IMMUTABLE);
#ଅନ୍ୟଥା
#ଯଦି HAVE_EXT2_IOCTLS
		ଯଦି (ioctl(fd, EXT2_IOC_GETFLAGS, &f) < 0) {
			retval = errno;
			ଯାଅ errout;
		}
		f |= EXT2_NODUMP_FL | EXT2_IMMUTABLE_FL;
		retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
#endif
#endif
		ଯଦି (retval) {
			retval = errno;
			ଯାଅ errout;
		}

		ଯଦି (close(fd) < 0) {
			retval = errno;
			fd = -1;
			ଯାଅ errout;
		}
		journal_ino = st.st_ino;
		memset(fs->super->s_jnl_blocks, 0,
		       ଆକାର(fs->super->s_jnl_blocks));
	} ଅନ୍ୟଥା {
		ଯଦି ((mount_flags & EXT2_MF_BUSY) &&
		    !(fs->flags & EXT2_FLAG_EXCLUSIVE)) {
			retval = EBUSY;
			ଯାଅ errout;
		}
		journal_ino = EXT2_JOURNAL_INO;
		ଯଦି ((retval = write_journal_inode(fs, journal_ino,
						  jparams, goal, flags)))
			ଫେରନ୍ତୁ retval;
	}

	fs->super->s_journal_inum = journal_ino;
	fs->super->s_journal_dev = 0;
	memset(fs->super->s_journal_uuid, 0,
	       ଆକାର(fs->super->s_journal_uuid));
	ext2fs_set_feature_journal(fs->super);

	ext2fs_mark_super_dirty(fs);
	ଫେରନ୍ତୁ 0;
errout:
	ଯଦି (fd >= 0)
		close(fd);
	ଫେରନ୍ତୁ retval;
}

errcode_t ext2fs_add_journal_inode2(ext2_filsys fs, blk_t ଅଙ୍କ_blocks,
				    blk64_t goal, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା ext2fs_journal_params jparams;

	jparams.ଅଙ୍କ_journal_blocks = ଅଙ୍କ_blocks;
	jparams.ଅଙ୍କ_fc_blocks = 0;

	ଫେରନ୍ତୁ ext2fs_add_journal_inode3(fs, &jparams, goal, flags);
}

errcode_t ext2fs_add_journal_inode(ext2_filsys fs, blk_t ଅଙ୍କ_blocks, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ଫେରନ୍ତୁ ext2fs_add_journal_inode2(fs, ଅଙ୍କ_blocks, ~0ULL, flags);
}


#ifdef DEBUG
ମୁଖ୍ୟ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
	errcode_t	retval;
	ଅକ୍ଷର		*device_name;
	ext2_filsys	fs;

	ଯଦି (argc < 2) {
		fprintf(stderr, "Usage: %s filesystem\n", argv[0]);
		exit(1);
	}
	device_name = argv[1];

	retval = ext2fs_open (device_name, EXT2_FLAG_RW, 0, 0,
			      unix_io_manager, &fs);
	ଯଦି (retval) {
		com_err(argv[0], retval, "while opening %s", device_name);
		exit(1);
	}

	retval = ext2fs_add_journal_inode(fs, JBD2_MIN_JOURNAL_BLOCKS, 0);
	ଯଦି (retval) {
		com_err(argv[0], retval, "while adding journal to %s",
			device_name);
		exit(1);
	}
	retval = ext2fs_flush(fs);
	ଯଦି (retval) {
		printf("Warning, had trouble writing out superblocks.\n");
	}
	ext2fs_close_free(&fs);
	exit(0);

}
#endif
