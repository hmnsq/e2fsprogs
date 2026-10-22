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
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#जर HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#अन्तर्भाव <fcntl.h>
#जर HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जर HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif
#जर HAVE_SYS_IOCTL_H
#अन्तर्भाव <sys/ioctl.h>
#endif
#जर HAVE_NETINET_IN_H
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
					   पूर्णांक flags, अक्षर **ret_jsb)
{
	errcode_t		retval;
	journal_superblock_t	*jsb;

	जर (jparams->अंक_journal_blocks < JBD2_MIN_JOURNAL_BLOCKS)
		परत EXT2_ET_JOURNAL_TOO_SMALL;

	जर ((retval = ext2fs_get_mem(fs->blocksize, &jsb)))
		परत retval;

	स_रखो (jsb, 0, fs->blocksize);

	jsb->s_header.h_magic = htonl(JBD2_MAGIC_संख्या);
	जर (flags & EXT2_MKJOURNAL_V1_SUPER)
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V1);
	अन्यथा
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V2);
	jsb->s_blocksize = htonl(fs->blocksize);
	jsb->s_maxlen = htonl(jparams->अंक_journal_blocks + jparams->अंक_fc_blocks);
	jsb->s_nr_users = htonl(1);
	jsb->s_first = htonl(1);
	jsb->s_sequence = htonl(1);
	jsb->s_अंक_fc_blks = htonl(jparams->अंक_fc_blocks);
	स_नकल(jsb->s_uuid, fs->super->s_uuid, आकार(fs->super->s_uuid));
	/*
	 * If we're creating an external journal device, we need to
	 * adjust these fields.
	 */
	जर (ext2fs_has_feature_journal_dev(fs->super)) {
		jsb->s_nr_users = 0;
		jsb->s_first = htonl(ext2fs_journal_sb_start(fs->blocksize) + 1);
	}

	*ret_jsb = (अक्षर *) jsb;
	परत 0;
}

errcode_t ext2fs_create_journal_superblock(ext2_filsys fs, __u32 अंक_blocks,
					पूर्णांक flags, अक्षर **ret_sb)
{
	संरचना ext2fs_journal_params jparams;

	jparams.अंक_journal_blocks = अंक_blocks;
	jparams.अंक_fc_blocks = 0;

	परत ext2fs_create_journal_superblock2(fs, &jparams, flags, ret_sb);
}

/*
 * This function writes a journal using POSIX routines.  It is used
 * for creating external journals and creating journals on live
 * filesystems.
 */
स्थिर errcode_t write_journal_file(ext2_filsys fs, अक्षर *filename,
				    संरचना ext2fs_journal_params *jparams,
				    पूर्णांक flags)
{
	errcode_t	retval;
	अक्षर		*buf = 0;
	पूर्णांक		fd, ret_size;
	blk_t		i;

	जर ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		परत retval;

	/* Open the device or journal file */
	जर ((fd = खोलो(filename, O_WRONLY)) < 0) {
		retval = त्रुटि_सं;
		जा errfree;
	}

	/* Write the superblock out */
	retval = EXT2_ET_SHORT_WRITE;
	ret_size = लिख(fd, buf, fs->blocksize);
	जर (ret_size < 0) {
		retval = त्रुटि_सं;
		जा errout;
	}
	जर (ret_size != (पूर्णांक) fs->blocksize)
		जा errout;
	स_रखो(buf, 0, fs->blocksize);

	जर (flags & EXT2_MKJOURNAL_LAZYINIT)
		जा success;

	साठी (i = 1; i < jparams->अंक_journal_blocks + jparams->अंक_fc_blocks; i++) {
		ret_size = लिख(fd, buf, fs->blocksize);
		जर (ret_size < 0) {
			retval = त्रुटि_सं;
			जा errout;
		}
		जर (ret_size != (पूर्णांक) fs->blocksize)
			जा errout;
	}

success:
	retval = 0;
errout:
	बंद(fd);
errfree:
	ext2fs_free_mem(&buf);
	परत retval;
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
#define MAX_STRIDE_LENGTH (4194304 / (पूर्णांक) fs->blocksize)
errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, पूर्णांक अंक,
			      blk64_t *ret_blk, पूर्णांक *ret_मोजणी)
{
	पूर्णांक		j, मोजणी;
	स्थिर शून्य	*buf;
	स्थिर पूर्णांक	stride_length;
	errcode_t	retval;

	/* If fs is null, clean up the static buffer and return */
	जर (!fs) {
		जर (buf) {
			मुक्त(buf);
			buf = 0;
			stride_length = 0;
		}
		परत 0;
	}

	/* Deal with zeroing less than 1 block */
	जर (अंक <= 0)
		परत 0;

	/* Try a zero out command, if supported */
	retval = io_channel_zeroout(fs->io, blk, अंक);
	जर (retval == 0)
		परत 0;

	/* Allocate the zeroizing buffer if necessary */
	जर (अंक > stride_length && stride_length < MAX_STRIDE_LENGTH) {
		शून्य *p;
		पूर्णांक new_stride = अंक;

		जर (new_stride > MAX_STRIDE_LENGTH)
			new_stride = MAX_STRIDE_LENGTH;
		p = पुनः_स्मृति(buf, fs->blocksize * new_stride);
		जर (!p)
			परत EXT2_ET_NO_MEMORY;
		buf = p;
		stride_length = new_stride;
		स_रखो(buf, 0, fs->blocksize * stride_length);
	}
	/* OK, do the write loop */
	j=0;
	पर्यंत (j < अंक) {
		जर (blk % stride_length) {
			मोजणी = stride_length - (blk % stride_length);
			जर (मोजणी > (अंक - j))
				मोजणी = अंक - j;
		} अन्यथा {
			मोजणी = अंक - j;
			जर (मोजणी > stride_length)
				मोजणी = stride_length;
		}
		retval = io_channel_write_blk64(fs->io, blk, मोजणी, buf);
		जर (retval) {
			जर (ret_मोजणी)
				*ret_मोजणी = मोजणी;
			जर (ret_blk)
				*ret_blk = blk;
			परत retval;
		}
		j += मोजणी; blk += मोजणी;
	}
	परत 0;
}

errcode_t ext2fs_zero_blocks(ext2_filsys fs, blk_t blk, पूर्णांक अंक,
			     blk_t *ret_blk, पूर्णांक *ret_मोजणी)
{
	blk64_t ret_blk2;
	errcode_t retval;

	retval = ext2fs_zero_blocks2(fs, blk, अंक, &ret_blk2, ret_मोजणी);
	जर (retval)
		*ret_blk = (blk_t) ret_blk2;
	परत retval;
}

/*
 * Calculate the initial goal block to be roughly at the middle of the
 * filesystem.  Pick a group that has the largest number of free
 * blocks.
 */
स्थिर blk64_t get_midpoint_journal_block(ext2_filsys fs)
{
	dgrp_t	group, start, end, i, log_flex;

	group = ext2fs_group_of_blk2(fs, (ext2fs_blocks_मोजणी(fs->super) -
					 fs->super->s_first_data_block) / 2);
	log_flex = 1U << fs->super->s_log_groups_per_flex;
	जर (fs->super->s_log_groups_per_flex && (group > log_flex)) {
		group = group & ~(log_flex - 1);
		पर्यंत ((group < fs->group_desc_मोजणी) &&
		       ext2fs_bg_free_blocks_मोजणी(fs, group) == 0)
			group++;
		जर (group == fs->group_desc_मोजणी)
			group = 0;
		start = group;
	} अन्यथा
		start = (group > 0) ? group-1 : group;
	end = ((group+1) < fs->group_desc_मोजणी) ? group+1 : group;
	group = start;
	साठी (i = start + 1; i <= end; i++)
		जर (ext2fs_bg_free_blocks_मोजणी(fs, i) >
		    ext2fs_bg_free_blocks_मोजणी(fs, group))
			group = i;
	परत ext2fs_group_first_block2(fs, group);
}

/*
 * This function creates a journal using direct I/O routines.
 */
स्थिर errcode_t write_journal_inode(ext2_filsys fs, ext2_ino_t journal_ino,
				     संरचना ext2fs_journal_params *jparams,
				     blk64_t goal, पूर्णांक flags)
{
	अक्षर			*buf;
	errcode_t		retval;
	संरचना ext2_inode	inode;
	अचिन्हित दीर्घ दीर्घ	inode_size;
	पूर्णांक			falloc_flags = EXT2_FALLOCATE_FORCE_INIT;
	blk64_t			zblk;
	समय_प्रकार			now;

	जर ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		परत retval;

	जर ((retval = ext2fs_read_bitmaps(fs)))
		जा out2;

	जर ((retval = ext2fs_read_inode(fs, journal_ino, &inode)))
		जा out2;

	जर (inode.i_blocks > 0) {
		retval = EEXIST;
		जा out2;
	}

	जर (goal == ~0ULL)
		goal = get_midpoint_journal_block(fs);

	जर (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;

	जर (!(flags & EXT2_MKJOURNAL_LAZYINIT))
		falloc_flags |= EXT2_FALLOCATE_ZERO_BLOCKS;

	inode_size = (अचिन्हित दीर्घ दीर्घ)fs->blocksize *
			(jparams->अंक_journal_blocks + jparams->अंक_fc_blocks);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	inode.i_links_मोजणी = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	retval = ext2fs_inode_size_set(fs, &inode, inode_size);
	जर (retval)
		जा out2;

	retval = ext2fs_fallocate(fs, falloc_flags, journal_ino,
				  &inode, goal, 0,
				  jparams->अंक_journal_blocks + jparams->अंक_fc_blocks);
	जर (retval)
		जा out2;

	जर ((retval = ext2fs_write_new_inode(fs, journal_ino, &inode)))
		जा out2;

	retval = ext2fs_bmap2(fs, journal_ino, &inode, रिक्त, 0, 0, रिक्त, &zblk);
	जर (retval)
		जा out2;

	retval = io_channel_write_blk64(fs->io, zblk, 1, buf);
	जर (retval)
		जा out2;

	स_नकल(fs->super->s_jnl_blocks, inode.i_block, EXT2_N_BLOCKS*4);
	fs->super->s_jnl_blocks[15] = inode.i_size_वरचा;
	fs->super->s_jnl_blocks[16] = inode.i_size;
	fs->super->s_jnl_backup_type = EXT3_JNL_BACKUP_BLOCKS;
	ext2fs_mark_super_dirty(fs);

out2:
	ext2fs_free_mem(&buf);
	परत retval;
}

/*
 * Find a reasonable journal file size (in blocks) given the number of blocks
 * in the filesystem.  For very small filesystems, it is not reasonable to
 * have a journal that fills more than half of the filesystem.
 *
 * n.b. comments assume 4k blocks
 */
पूर्णांक ext2fs_default_journal_size(__u64 अंक_blocks)
{
	जर (अंक_blocks < 2048)
		परत -1;
	जर (अंक_blocks < 32768)		/* 128 MB */
		परत (1024);			/* 4 MB */
	जर (अंक_blocks < 256*1024)	/* 1 GB */
		परत (4096);			/* 16 MB */
	जर (अंक_blocks < 512*1024)	/* 2 GB */
		परत (8192);			/* 32 MB */
	जर (अंक_blocks < 4096*1024)	/* 16 GB */
		परत (16384);			/* 64 MB */
	जर (अंक_blocks < 8192*1024)	/* 32 GB */
		परत (32768);			/* 128 MB */
	जर (अंक_blocks < 16384*1024)	/* 64 GB */
		परत (65536);			/* 256 MB */
	जर (अंक_blocks < 32768*1024)	/* 128 GB */
		परत (131072);		/* 512 MB */
	परत 262144;				/* 1 GB */
}

errcode_t ext2fs_get_journal_params(संरचना ext2fs_journal_params *params,
		ext2_filsys fs)
{
	blk_t total_blks;
	पूर्णांक ret;

	स_रखो(params, 0, आकार(*params));
	जर (ext2fs_has_feature_journal_dev(fs->super)) {
		total_blks = ext2fs_blocks_मोजणी(fs->super);
		जर (total_blks < JBD2_MIN_JOURNAL_BLOCKS)
			परत EXT2_ET_JOURNAL_TOO_SMALL;

		जर (!ext2fs_has_feature_fast_commit(fs->super)) {
			params->अंक_journal_blocks = total_blks;
			params->अंक_fc_blocks = 0;
			परत 0;
		}
		params->अंक_journal_blocks = ext2fs_blocks_मोजणी(fs->super) *
				EXT2_JOURNAL_TO_FC_BLKS_RATIO /
				(EXT2_JOURNAL_TO_FC_BLKS_RATIO + 1);
		जर (JBD2_MIN_JOURNAL_BLOCKS > params->अंक_journal_blocks)
			params->अंक_journal_blocks = JBD2_MIN_JOURNAL_BLOCKS;
		params->अंक_fc_blocks = total_blks - params->अंक_journal_blocks;
		परत 0;
	}

	ret = ext2fs_default_journal_size(ext2fs_blocks_मोजणी(fs->super));
	जर (ret < 0)
		परत EXT2_ET_JOURNAL_TOO_SMALL;

	params->अंक_journal_blocks = ret;
	जर (ext2fs_has_feature_fast_commit(fs->super))
		params->अंक_fc_blocks = params->अंक_journal_blocks /
			EXT2_JOURNAL_TO_FC_BLKS_RATIO;
	परत 0;
}

पूर्णांक ext2fs_journal_sb_start(पूर्णांक blocksize)
{
	जर (blocksize == EXT2_MIN_BLOCK_SIZE)
		परत 2;
	परत 1;
}

/*
 * This function adds a journal device to a filesystem
 */
errcode_t ext2fs_add_journal_device(ext2_filsys fs, ext2_filsys journal_dev)
{
	संरचना stat	st;
	errcode_t	retval;
	अक्षर		buf[SUPERBLOCK_SIZE];
	journal_superblock_t	*jsb;
	पूर्णांक		start;
	__u32		i, nr_users;

	/* Make sure the device exists and is a block device */
	जर (stat(journal_dev->device_name, &st) < 0)
		परत त्रुटि_सं;

	जर (!S_ISBLK(st.st_mode))
		परत EXT2_ET_JOURNAL_NOT_BLOCK; /* Must be a block device */

	/* Get the journal superblock */
	start = ext2fs_journal_sb_start(journal_dev->blocksize);
	जर ((retval = io_channel_read_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE,
					    buf)))
		परत retval;

	jsb = (journal_superblock_t *) buf;
	जर ((jsb->s_header.h_magic != (अचिन्हित) ntohl(JBD2_MAGIC_संख्या)) ||
	    (jsb->s_header.h_blocktype != (अचिन्हित) ntohl(JBD2_SUPERBLOCK_V2)))
		परत EXT2_ET_NO_JOURNAL_SB;

	जर (ntohl(jsb->s_blocksize) != (अचिन्हित दीर्घ) fs->blocksize)
		परत EXT2_ET_UNEXPECTED_BLOCK_SIZE;

	/* Check and see if this filesystem has already been added */
	nr_users = ntohl(jsb->s_nr_users);
	जर (nr_users > JBD2_USERS_MAX)
		परत EXT2_ET_CORRUPT_JOURNAL_SB;
	साठी (i=0; i < nr_users; i++) {
		जर (स_भेद(fs->super->s_uuid,
			   &jsb->s_users[i*16], 16) == 0)
			थांबा;
	}
	जर (i >= nr_users) {
		स_नकल(&jsb->s_users[nr_users*16],
		       fs->super->s_uuid, 16);
		jsb->s_nr_users = htonl(nr_users+1);
	}

	/* Writeback the journal superblock */
	जर ((retval = io_channel_write_blk64(journal_dev->io, start,
					    -SUPERBLOCK_SIZE, buf)))
		परत retval;

	fs->super->s_journal_inum = 0;
	fs->super->s_journal_dev = st.st_rdev;
	स_नकल(fs->super->s_journal_uuid, jsb->s_uuid,
	       आकार(fs->super->s_journal_uuid));
	स_रखो(fs->super->s_jnl_blocks, 0, आकार(fs->super->s_jnl_blocks));
	ext2fs_set_feature_journal(fs->super);
	ext2fs_mark_super_dirty(fs);
	परत 0;
}

/*
 * This function adds a journal inode to a filesystem, using either
 * POSIX routines if the filesystem is mounted, or using direct I/O
 * functions if it is not.
 */
errcode_t ext2fs_add_journal_inode3(ext2_filsys fs, संरचना ext2fs_journal_params *jparams,
				    blk64_t goal, पूर्णांक flags)
{
	errcode_t		retval;
	ext2_ino_t		journal_ino;
	संरचना stat		st;
	अक्षर			jfile[1024];
	पूर्णांक			mount_flags;
	पूर्णांक			fd = -1;

	जर (flags & EXT2_MKJOURNAL_NO_MNT_CHECK)
		mount_flags = 0;
	अन्यथा जर ((retval = ext2fs_check_mount_point(fs->device_name,
						    &mount_flags,
						    jfile, आकार(jfile)-10)))
		परत retval;

	जर (mount_flags & EXT2_MF_MOUNTED) {
#जर HAVE_EXT2_IOCTLS
		पूर्णांक f = 0;
#endif
		म_जोड़ो(jfile, "/.journal");

		/*
		 * If .../.journal already exists, make sure any
		 * immutable or append-only flags are cleared.
		 */
#जर defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		(शून्य) chflags (jfile, 0);
#अन्यथा
#जर HAVE_EXT2_IOCTLS
		fd = खोलो(jfile, O_RDONLY);
		जर (fd >= 0) {
			retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
			बंद(fd);
			जर (retval)
				परत त्रुटि_सं;
		}
#endif
#endif

		/* Create the journal file */
		जर ((fd = खोलो(jfile, O_CREAT|O_WRONLY, 0600)) < 0)
			परत त्रुटि_सं;

		/* Note that we can't do lazy journal initialization for mounted
		 * filesystems, since the zero writing is also allocating the
		 * journal blocks.  We could use fallocate, but not all kernels
		 * support that, and creating a journal on a mounted ext2
		 * filesystems is extremely rare these days...  Ignore it. */
		flags &= ~EXT2_MKJOURNAL_LAZYINIT;

		जर ((retval = write_journal_file(fs, jfile, jparams, flags)))
			जा errout;

		/* Get inode number of the journal file */
		जर (ख_स्थिति(fd, &st) < 0) {
			retval = त्रुटि_सं;
			जा errout;
		}

#जर defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		retval = fchflags (fd, UF_NODUMP|UF_IMMUTABLE);
#अन्यथा
#जर HAVE_EXT2_IOCTLS
		जर (ioctl(fd, EXT2_IOC_GETFLAGS, &f) < 0) {
			retval = त्रुटि_सं;
			जा errout;
		}
		f |= EXT2_NODUMP_FL | EXT2_IMMUTABLE_FL;
		retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
#endif
#endif
		जर (retval) {
			retval = त्रुटि_सं;
			जा errout;
		}

		जर (बंद(fd) < 0) {
			retval = त्रुटि_सं;
			fd = -1;
			जा errout;
		}
		journal_ino = st.st_ino;
		स_रखो(fs->super->s_jnl_blocks, 0,
		       आकार(fs->super->s_jnl_blocks));
	} अन्यथा {
		जर ((mount_flags & EXT2_MF_BUSY) &&
		    !(fs->flags & EXT2_FLAG_EXCLUSIVE)) {
			retval = EBUSY;
			जा errout;
		}
		journal_ino = EXT2_JOURNAL_INO;
		जर ((retval = write_journal_inode(fs, journal_ino,
						  jparams, goal, flags)))
			परत retval;
	}

	fs->super->s_journal_inum = journal_ino;
	fs->super->s_journal_dev = 0;
	स_रखो(fs->super->s_journal_uuid, 0,
	       आकार(fs->super->s_journal_uuid));
	ext2fs_set_feature_journal(fs->super);

	ext2fs_mark_super_dirty(fs);
	परत 0;
errout:
	जर (fd >= 0)
		बंद(fd);
	परत retval;
}

errcode_t ext2fs_add_journal_inode2(ext2_filsys fs, blk_t अंक_blocks,
				    blk64_t goal, पूर्णांक flags)
{
	संरचना ext2fs_journal_params jparams;

	jparams.अंक_journal_blocks = अंक_blocks;
	jparams.अंक_fc_blocks = 0;

	परत ext2fs_add_journal_inode3(fs, &jparams, goal, flags);
}

errcode_t ext2fs_add_journal_inode(ext2_filsys fs, blk_t अंक_blocks, पूर्णांक flags)
{
	परत ext2fs_add_journal_inode2(fs, अंक_blocks, ~0ULL, flags);
}


#ifdef DEBUG
main(पूर्णांक argc, अक्षर **argv)
{
	errcode_t	retval;
	अक्षर		*device_name;
	ext2_filsys	fs;

	जर (argc < 2) {
		ख_लिखो(मानक_त्रुटि, "Usage: %s filesystem\n", argv[0]);
		निकास(1);
	}
	device_name = argv[1];

	retval = ext2fs_open (device_name, EXT2_FLAG_RW, 0, 0,
			      unix_io_manager, &fs);
	जर (retval) {
		com_err(argv[0], retval, "while opening %s", device_name);
		निकास(1);
	}

	retval = ext2fs_add_journal_inode(fs, JBD2_MIN_JOURNAL_BLOCKS, 0);
	जर (retval) {
		com_err(argv[0], retval, "while adding journal to %s",
			device_name);
		निकास(1);
	}
	retval = ext2fs_flush(fs);
	जर (retval) {
		म_लिखो("Warning, had trouble writing out superblocks.\n");
	}
	ext2fs_close_free(&fs);
	निकास(0);

}
#endif
