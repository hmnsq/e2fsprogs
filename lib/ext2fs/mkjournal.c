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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <assert.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#अन्तर्भाव <fcntl.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#यदि HAVE_SYS_IOCTL_H
#अन्तर्भाव <sys/ioctl.h>
#endif
#यदि HAVE_NETINET_IN_H
#अन्तर्भाव <netinet/in.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "e2p/e2p.h"
#अन्तर्भाव "ext2fsP.h"

#अन्तर्भाव "kernel-jbd.h"

/*
 * This function automatically sets up the journal superblock and
 * returns it as an allocated block.
 */
errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
					   संरचना ext2fs_journal_params *jparams,
					   पूर्णाङ्क flags, वर्ण **ret_jsb)
{
	errcode_t		retval;
	journal_superblock_t	*jsb;

	यदि (jparams->अङ्क_journal_blocks < JBD2_MIN_JOURNAL_BLOCKS)
		फर्काऊ EXT2_ET_JOURNAL_TOO_SMALL;

	यदि ((retval = ext2fs_get_mem(fs->blocksize, &jsb)))
		फर्काऊ retval;

	स_रखो (jsb, 0, fs->blocksize);

	jsb->s_header.h_magic = htonl(JBD2_MAGIC_सङ्ख्या);
	यदि (flags & EXT2_MKJOURNAL_V1_SUPER)
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V1);
	अन्यथा
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V2);
	jsb->s_blocksize = htonl(fs->blocksize);
	jsb->s_maxlen = htonl(jparams->अङ्क_journal_blocks + jparams->अङ्क_fc_blocks);
	jsb->s_nr_users = htonl(1);
	jsb->s_first = htonl(1);
	jsb->s_sequence = htonl(1);
	jsb->s_अङ्क_fc_blks = htonl(jparams->अङ्क_fc_blocks);
	स_नकल(jsb->s_uuid, fs->super->s_uuid, परिमाण(fs->super->s_uuid));
	/*
	 * If we're creating an external journal device, we need to
	 * adjust these fields.
	 */
	यदि (ext2fs_has_feature_journal_dev(fs->super)) {
		jsb->s_nr_users = 0;
		jsb->s_first = htonl(ext2fs_journal_sb_start(fs->blocksize) + 1);
	}

	*ret_jsb = (वर्ण *) jsb;
	फर्काऊ 0;
}

errcode_t ext2fs_create_journal_superblock(ext2_filsys fs, __u32 अङ्क_blocks,
					पूर्णाङ्क flags, वर्ण **ret_sb)
{
	संरचना ext2fs_journal_params jparams;

	jparams.अङ्क_journal_blocks = अङ्क_blocks;
	jparams.अङ्क_fc_blocks = 0;

	फर्काऊ ext2fs_create_journal_superblock2(fs, &jparams, flags, ret_sb);
}

/*
 * This function writes a journal using POSIX routines.  It is used
 * for creating external journals and creating journals on live
 * filesystems.
 */
स्थिर errcode_t write_journal_file(ext2_filsys fs, वर्ण *filename,
				    संरचना ext2fs_journal_params *jparams,
				    पूर्णाङ्क flags)
{
	errcode_t	retval;
	वर्ण		*buf = 0;
	पूर्णाङ्क		fd, ret_size;
	blk_t		i;

	यदि ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		फर्काऊ retval;

	/* Open the device or journal file */
	यदि ((fd = खोलो(filename, O_WRONLY)) < 0) {
		retval = त्रुटि_सं;
		गच्छ errfree;
	}

	/* Write the superblock out */
	retval = EXT2_ET_SHORT_WRITE;
	ret_size = लिख(fd, buf, fs->blocksize);
	यदि (ret_size < 0) {
		retval = त्रुटि_सं;
		गच्छ errout;
	}
	यदि (ret_size != (पूर्णाङ्क) fs->blocksize)
		गच्छ errout;
	स_रखो(buf, 0, fs->blocksize);

	यदि (flags & EXT2_MKJOURNAL_LAZYINIT)
		गच्छ success;

	लागि (i = 1; i < jparams->अङ्क_journal_blocks + jparams->अङ्क_fc_blocks; i++) {
		ret_size = लिख(fd, buf, fs->blocksize);
		यदि (ret_size < 0) {
			retval = त्रुटि_सं;
			गच्छ errout;
		}
		यदि (ret_size != (पूर्णाङ्क) fs->blocksize)
			गच्छ errout;
	}

success:
	retval = 0;
errout:
	बंद(fd);
errfree:
	ext2fs_free_mem(&buf);
	फर्काऊ retval;
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
#define MAX_STRIDE_LENGTH (4194304 / (पूर्णाङ्क) fs->blocksize)
errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, पूर्णाङ्क अङ्क,
			      blk64_t *ret_blk, पूर्णाङ्क *ret_गन्ती)
{
	पूर्णाङ्क		j, गन्ती;
	स्थिर शून्य	*buf;
	स्थिर पूर्णाङ्क	stride_length;
	errcode_t	retval;

	/* If fs is null, clean up the static buffer and return */
	यदि (!fs) {
		यदि (buf) {
			मुक्त(buf);
			buf = 0;
			stride_length = 0;
		}
		फर्काऊ 0;
	}

	/* Deal with zeroing less than 1 block */
	यदि (अङ्क <= 0)
		फर्काऊ 0;

	/* Try a zero out command, if supported */
	retval = io_channel_zeroout(fs->io, blk, अङ्क);
	यदि (retval == 0)
		फर्काऊ 0;

	/* Allocate the zeroizing buffer if necessary */
	यदि (अङ्क > stride_length && stride_length < MAX_STRIDE_LENGTH) {
		शून्य *p;
		पूर्णाङ्क new_stride = अङ्क;

		यदि (new_stride > MAX_STRIDE_LENGTH)
			new_stride = MAX_STRIDE_LENGTH;
		p = पुनः_स्मृति(buf, fs->blocksize * new_stride);
		यदि (!p)
			फर्काऊ EXT2_ET_NO_MEMORY;
		buf = p;
		stride_length = new_stride;
		स_रखो(buf, 0, fs->blocksize * stride_length);
	}
	/* OK, do the write loop */
	j=0;
	जबसम्म (j < अङ्क) {
		यदि (blk % stride_length) {
			गन्ती = stride_length - (blk % stride_length);
			यदि (गन्ती > (अङ्क - j))
				गन्ती = अङ्क - j;
		} अन्यथा {
			गन्ती = अङ्क - j;
			यदि (गन्ती > stride_length)
				गन्ती = stride_length;
		}
		retval = io_channel_write_blk64(fs->io, blk, गन्ती, buf);
		यदि (retval) {
			यदि (ret_गन्ती)
				*ret_गन्ती = गन्ती;
			यदि (ret_blk)
				*ret_blk = blk;
			फर्काऊ retval;
		}
		j += गन्ती; blk += गन्ती;
	}
	फर्काऊ 0;
}

errcode_t ext2fs_zero_blocks(ext2_filsys fs, blk_t blk, पूर्णाङ्क अङ्क,
			     blk_t *ret_blk, पूर्णाङ्क *ret_गन्ती)
{
	blk64_t ret_blk2;
	errcode_t retval;

	retval = ext2fs_zero_blocks2(fs, blk, अङ्क, &ret_blk2, ret_गन्ती);
	यदि (retval)
		*ret_blk = (blk_t) ret_blk2;
	फर्काऊ retval;
}

/*
 * Calculate the initial goal block to be roughly at the middle of the
 * filesystem.  Pick a group that has the largest number of free
 * blocks.
 */
स्थिर blk64_t get_midpoint_journal_block(ext2_filsys fs)
{
	dgrp_t	group, start, end, i, log_flex;

	group = ext2fs_group_of_blk2(fs, (ext2fs_blocks_गन्ती(fs->super) -
					 fs->super->s_first_data_block) / 2);
	log_flex = 1U << fs->super->s_log_groups_per_flex;
	यदि (fs->super->s_log_groups_per_flex && (group > log_flex)) {
		group = group & ~(log_flex - 1);
		जबसम्म ((group < fs->group_desc_गन्ती) &&
		       ext2fs_bg_free_blocks_गन्ती(fs, group) == 0)
			group++;
		यदि (group == fs->group_desc_गन्ती)
			group = 0;
		start = group;
	} अन्यथा
		start = (group > 0) ? group-1 : group;
	end = ((group+1) < fs->group_desc_गन्ती) ? group+1 : group;
	group = start;
	लागि (i = start + 1; i <= end; i++)
		यदि (ext2fs_bg_free_blocks_गन्ती(fs, i) >
		    ext2fs_bg_free_blocks_गन्ती(fs, group))
			group = i;
	फर्काऊ ext2fs_group_first_block2(fs, group);
}

/*
 * This function creates a journal using direct I/O routines.
 */
स्थिर errcode_t write_journal_inode(ext2_filsys fs, ext2_ino_t journal_ino,
				     संरचना ext2fs_journal_params *jparams,
				     blk64_t goal, पूर्णाङ्क flags)
{
	वर्ण			*buf;
	errcode_t		retval;
	संरचना ext2_inode	inode;
	अचिन्हित दीर्घ दीर्घ	inode_size;
	पूर्णाङ्क			falloc_flags = EXT2_FALLOCATE_FORCE_INIT;
	blk64_t			zblk;
	समय_प्रकार			now;

	यदि ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		फर्काऊ retval;

	यदि ((retval = ext2fs_read_bitmaps(fs)))
		गच्छ out2;

	यदि ((retval = ext2fs_read_inode(fs, journal_ino, &inode)))
		गच्छ out2;

	यदि (inode.i_blocks > 0) {
		retval = EEXIST;
		गच्छ out2;
	}

	यदि (goal == ~0ULL)
		goal = get_midpoint_journal_block(fs);

	यदि (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;

	यदि (!(flags & EXT2_MKJOURNAL_LAZYINIT))
		falloc_flags |= EXT2_FALLOCATE_ZERO_BLOCKS;

	inode_size = (अचिन्हित दीर्घ दीर्घ)fs->blocksize *
			(jparams->अङ्क_journal_blocks + jparams->अङ्क_fc_blocks);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	inode.i_links_गन्ती = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	retval = ext2fs_inode_size_set(fs, &inode, inode_size);
	यदि (retval)
		गच्छ out2;

	retval = ext2fs_fallocate(fs, falloc_flags, journal_ino,
				  &inode, goal, 0,
				  jparams->अङ्क_journal_blocks + jparams->अङ्क_fc_blocks);
	यदि (retval)
		गच्छ out2;

	यदि ((retval = ext2fs_write_new_inode(fs, journal_ino, &inode)))
		गच्छ out2;

	retval = ext2fs_bmap2(fs, journal_ino, &inode, रिक्त, 0, 0, रिक्त, &zblk);
	यदि (retval)
		गच्छ out2;

	retval = io_channel_write_blk64(fs->io, zblk, 1, buf);
	यदि (retval)
		गच्छ out2;

	स_नकल(fs->super->s_jnl_blocks, inode.i_block, EXT2_N_BLOCKS*4);
	fs->super->s_jnl_blocks[15] = inode.i_size_माथिल्लो;
	fs->super->s_jnl_blocks[16] = inode.i_size;
	fs->super->s_jnl_backup_type = EXT3_JNL_BACKUP_BLOCKS;
	ext2fs_mark_super_dirty(fs);

out2:
	ext2fs_free_mem(&buf);
	फर्काऊ retval;
}

/*
 * Find a reasonable journal file size (in blocks) given the number of blocks
 * in the filesystem.  For very small filesystems, it is not reasonable to
 * have a journal that fills more than half of the filesystem.
 *
 * n.b. comments assume 4k blocks
 */
पूर्णाङ्क ext2fs_default_journal_size(__u64 अङ्क_blocks)
{
	यदि (अङ्क_blocks < 2048)
		फर्काऊ -1;
	यदि (अङ्क_blocks < 32768)		/* 128 MB */
		फर्काऊ (1024);			/* 4 MB */
	यदि (अङ्क_blocks < 256*1024)	/* 1 GB */
		फर्काऊ (4096);			/* 16 MB */
	यदि (अङ्क_blocks < 512*1024)	/* 2 GB */
		फर्काऊ (8192);			/* 32 MB */
	यदि (अङ्क_blocks < 4096*1024)	/* 16 GB */
		फर्काऊ (16384);			/* 64 MB */
	यदि (अङ्क_blocks < 8192*1024)	/* 32 GB */
		फर्काऊ (32768);			/* 128 MB */
	यदि (अङ्क_blocks < 16384*1024)	/* 64 GB */
		फर्काऊ (65536);			/* 256 MB */
	यदि (अङ्क_blocks < 32768*1024)	/* 128 GB */
		फर्काऊ (131072);		/* 512 MB */
	फर्काऊ 262144;				/* 1 GB */
}

errcode_t ext2fs_get_journal_params(संरचना ext2fs_journal_params *params,
		ext2_filsys fs)
{
	blk_t total_blks;
	पूर्णाङ्क ret;

	स_रखो(params, 0, परिमाण(*params));
	यदि (ext2fs_has_feature_journal_dev(fs->super)) {
		total_blks = ext2fs_blocks_गन्ती(fs->super);
		यदि (total_blks < JBD2_MIN_JOURNAL_BLOCKS)
			फर्काऊ EXT2_ET_JOURNAL_TOO_SMALL;

		यदि (!ext2fs_has_feature_fast_commit(fs->super)) {
			params->अङ्क_journal_blocks = total_blks;
			params->अङ्क_fc_blocks = 0;
			फर्काऊ 0;
		}
		params->अङ्क_journal_blocks = ext2fs_blocks_गन्ती(fs->super) *
				EXT2_JOURNAL_TO_FC_BLKS_RATIO /
				(EXT2_JOURNAL_TO_FC_BLKS_RATIO + 1);
		यदि (JBD2_MIN_JOURNAL_BLOCKS > params->अङ्क_journal_blocks)
			params->अङ्क_journal_blocks = JBD2_MIN_JOURNAL_BLOCKS;
		params->अङ्क_fc_blocks = total_blks - params->अङ्क_journal_blocks;
		फर्काऊ 0;
	}

	ret = ext2fs_default_journal_size(ext2fs_blocks_गन्ती(fs->super));
	यदि (ret < 0)
		फर्काऊ EXT2_ET_JOURNAL_TOO_SMALL;

	params->अङ्क_journal_blocks = ret;
	यदि (ext2fs_has_feature_fast_commit(fs->super))
		params->अङ्क_fc_blocks = params->अङ्क_journal_blocks /
			EXT2_JOURNAL_TO_FC_BLKS_RATIO;
	फर्काऊ 0;
}

पूर्णाङ्क ext2fs_journal_sb_start(पूर्णाङ्क blocksize)
{
	यदि (blocksize == EXT2_MIN_BLOCK_SIZE)
		फर्काऊ 2;
	फर्काऊ 1;
}

/*
 * This function adds a journal device to a filesystem
 */
errcode_t ext2fs_add_journal_device(ext2_filsys fs, ext2_filsys journal_dev)
{
	संरचना stat	st;
	errcode_t	retval;
	वर्ण		buf[SUPERBLOCK_SIZE];
	journal_superblock_t	*jsb;
	पूर्णाङ्क		start;
	__u32		i, nr_users;

	/* Make sure the device exists and is a block device */
	यदि (stat(journal_dev->device_name, &st) < 0)
		फर्काऊ त्रुटि_सं;

	यदि (!S_ISBLK(st.st_mode))
		फर्काऊ EXT2_ET_JOURNAL_NOT_BLOCK; /* Must be a block device */

	/* Get the journal superblock */
	start = ext2fs_journal_sb_start(journal_dev->blocksize);
	यदि ((retval = io_channel_read_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE,
					    buf)))
		फर्काऊ retval;

	jsb = (journal_superblock_t *) buf;
	यदि ((jsb->s_header.h_magic != (अचिन्हित) ntohl(JBD2_MAGIC_सङ्ख्या)) ||
	    (jsb->s_header.h_blocktype != (अचिन्हित) ntohl(JBD2_SUPERBLOCK_V2)))
		फर्काऊ EXT2_ET_NO_JOURNAL_SB;

	यदि (ntohl(jsb->s_blocksize) != (अचिन्हित दीर्घ) fs->blocksize)
		फर्काऊ EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	/* Check and see if this filesystem has already been added */
	nr_users = ntohl(jsb->s_nr_users);
	यदि (nr_users > JBD2_USERS_MAX)
		फर्काऊ EXT2_ET_CORRUPT_JOURNAL_SB;
	लागि (i=0; i < nr_users; i++) {
		यदि (स_भेद(fs->super->s_uuid,
			   &jsb->s_users[i*16], 16) == 0)
			रोक;
	}
	यदि (i >= nr_users) {
		स_नकल(&jsb->s_users[nr_users*16],
		       fs->super->s_uuid, 16);
		jsb->s_nr_users = htonl(nr_users+1);
	}

	/* Writeback the journal superblock */
	यदि ((retval = io_channel_write_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE, buf)))
		फर्काऊ retval;

	fs->super->s_journal_inum = 0;
	fs->super->s_journal_dev = st.st_rdev;
	स_नकल(fs->super->s_journal_uuid, jsb->s_uuid,
	       परिमाण(fs->super->s_journal_uuid));
	स_रखो(fs->super->s_jnl_blocks, 0, परिमाण(fs->super->s_jnl_blocks));
	ext2fs_set_feature_journal(fs->super);
	ext2fs_mark_super_dirty(fs);
	फर्काऊ 0;
}

/*
 * This function adds a journal inode to a filesystem, using either
 * POSIX routines if the filesystem is mounted, or using direct I/O
 * functions if it is not.
 */
errcode_t ext2fs_add_journal_inode3(ext2_filsys fs, संरचना ext2fs_journal_params *jparams,
				    blk64_t goal, पूर्णाङ्क flags)
{
	errcode_t		retval;
	ext2_ino_t		journal_ino;
	संरचना stat		st;
	वर्ण			jfile[1024];
	पूर्णाङ्क			mount_flags;
	पूर्णाङ्क			fd = -1;

	यदि (flags & EXT2_MKJOURNAL_NO_MNT_CHECK)
		mount_flags = 0;
	अन्यथा यदि ((retval = ext2fs_check_mount_point(fs->device_name,
						    &mount_flags,
						    jfile, परिमाण(jfile)-10)))
		फर्काऊ retval;

	यदि (mount_flags & EXT2_MF_MOUNTED) {
#यदि HAVE_EXT2_IOCTLS
		पूर्णाङ्क f = 0;
#endif
		म_जोड़ो(jfile, "/.journal");

		/*
		 * If .../.journal already exists, make sure any
		 * immutable or append-only flags are cleared.
		 */
#यदि defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		(शून्य) chflags (jfile, 0);
#अन्यथा
#यदि HAVE_EXT2_IOCTLS
		fd = खोलो(jfile, O_RDONLY);
		यदि (fd >= 0) {
			retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
			बंद(fd);
			यदि (retval)
				फर्काऊ त्रुटि_सं;
		}
#endif
#endif

		/* Create the journal file */
		यदि ((fd = खोलो(jfile, O_CREAT|O_WRONLY, 0600)) < 0)
			फर्काऊ त्रुटि_सं;

		/* Note that we can't do lazy journal initialization for mounted
		 * filesystems, since the zero writing is also allocating the
		 * journal blocks.  We could use fallocate, but not all kernels
		 * support that, and creating a journal on a mounted ext2
		 * filesystems is extremely rare these days...  Ignore it. */
		flags &= ~EXT2_MKJOURNAL_LAZYINIT;

		यदि ((retval = write_journal_file(fs, jfile, jparams, flags)))
			गच्छ errout;

		/* Get inode number of the journal file */
		यदि (ख_स्थिति(fd, &st) < 0) {
			retval = त्रुटि_सं;
			गच्छ errout;
		}

#यदि defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		retval = fchflags (fd, UF_NODUMP|UF_IMMUTABLE);
#अन्यथा
#यदि HAVE_EXT2_IOCTLS
		यदि (ioctl(fd, EXT2_IOC_GETFLAGS, &f) < 0) {
			retval = त्रुटि_सं;
			गच्छ errout;
		}
		f |= EXT2_NODUMP_FL | EXT2_IMMUTABLE_FL;
		retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
#endif
#endif
		यदि (retval) {
			retval = त्रुटि_सं;
			गच्छ errout;
		}

		यदि (बंद(fd) < 0) {
			retval = त्रुटि_सं;
			fd = -1;
			गच्छ errout;
		}
		journal_ino = st.st_ino;
		स_रखो(fs->super->s_jnl_blocks, 0,
		       परिमाण(fs->super->s_jnl_blocks));
	} अन्यथा {
		यदि ((mount_flags & EXT2_MF_BUSY) &&
		    !(fs->flags & EXT2_FLAG_EXCLUSIVE)) {
			retval = EBUSY;
			गच्छ errout;
		}
		journal_ino = EXT2_JOURNAL_INO;
		यदि ((retval = write_journal_inode(fs, journal_ino,
						  jparams, goal, flags)))
			फर्काऊ retval;
	}

	fs->super->s_journal_inum = journal_ino;
	fs->super->s_journal_dev = 0;
	स_रखो(fs->super->s_journal_uuid, 0,
	       परिमाण(fs->super->s_journal_uuid));
	ext2fs_set_feature_journal(fs->super);

	ext2fs_mark_super_dirty(fs);
	फर्काऊ 0;
errout:
	यदि (fd >= 0)
		बंद(fd);
	फर्काऊ retval;
}

errcode_t ext2fs_add_journal_inode2(ext2_filsys fs, blk_t अङ्क_blocks,
				    blk64_t goal, पूर्णाङ्क flags)
{
	संरचना ext2fs_journal_params jparams;

	jparams.अङ्क_journal_blocks = अङ्क_blocks;
	jparams.अङ्क_fc_blocks = 0;

	फर्काऊ ext2fs_add_journal_inode3(fs, &jparams, goal, flags);
}

errcode_t ext2fs_add_journal_inode(ext2_filsys fs, blk_t अङ्क_blocks, पूर्णाङ्क flags)
{
	फर्काऊ ext2fs_add_journal_inode2(fs, अङ्क_blocks, ~0ULL, flags);
}


#ifdef DEBUG
main(पूर्णाङ्क argc, वर्ण **argv)
{
	errcode_t	retval;
	वर्ण		*device_name;
	ext2_filsys	fs;

	यदि (argc < 2) {
		ख_लिखो(मानक_त्रुटि, "Usage: %s filesystem\n", argv[0]);
		निकास(1);
	}
	device_name = argv[1];

	retval = ext2fs_open (device_name, EXT2_FLAG_RW, 0, 0,
			      unix_io_manager, &fs);
	यदि (retval) {
		com_err(argv[0], retval, "while opening %s", device_name);
		निकास(1);
	}

	retval = ext2fs_add_journal_inode(fs, JBD2_MIN_JOURNAL_BLOCKS, 0);
	यदि (retval) {
		com_err(argv[0], retval, "while adding journal to %s",
			device_name);
		निकास(1);
	}
	retval = ext2fs_flush(fs);
	यदि (retval) {
		म_लिखो("Warning, had trouble writing out superblocks.\n");
	}
	ext2fs_close_free(&fs);
	निकास(0);

}
#endif
