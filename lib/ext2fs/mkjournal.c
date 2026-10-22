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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#समावेश <assert.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#अगर HAVE_ERRNO_H
#समावेश <errno.h>
#पूर्ण_अगर
#समावेश <fcntl.h>
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर
#अगर HAVE_SYS_IOCTL_H
#समावेश <sys/ioctl.h>
#पूर्ण_अगर
#अगर HAVE_NETINET_IN_H
#समावेश <netinet/in.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "e2p/e2p.h"
#समावेश "ext2fsP.h"

#समावेश "kernel-jbd.h"

/*
 * This function automatically sets up the journal superblock and
 * returns it as an allocated block.
 */
errcode_t ext2fs_create_journal_superblock2(ext2_filsys fs,
					   काष्ठा ext2fs_journal_params *jparams,
					   पूर्णांक flags, अक्षर **ret_jsb)
{
	errcode_t		retval;
	journal_superblock_t	*jsb;

	अगर (jparams->अंक_journal_blocks < JBD2_MIN_JOURNAL_BLOCKS)
		वापस EXT2_ET_JOURNAL_TOO_SMALL;

	अगर ((retval = ext2fs_get_mem(fs->blocksize, &jsb)))
		वापस retval;

	स_रखो (jsb, 0, fs->blocksize);

	jsb->s_header.h_magic = htonl(JBD2_MAGIC_संख्या);
	अगर (flags & EXT2_MKJOURNAL_V1_SUPER)
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V1);
	अन्यथा
		jsb->s_header.h_blocktype = htonl(JBD2_SUPERBLOCK_V2);
	jsb->s_blocksize = htonl(fs->blocksize);
	jsb->s_maxlen = htonl(jparams->अंक_journal_blocks + jparams->अंक_fc_blocks);
	jsb->s_nr_users = htonl(1);
	jsb->s_first = htonl(1);
	jsb->s_sequence = htonl(1);
	jsb->s_अंक_fc_blks = htonl(jparams->अंक_fc_blocks);
	स_नकल(jsb->s_uuid, fs->super->s_uuid, माप(fs->super->s_uuid));
	/*
	 * If we're creating an external journal device, we need to
	 * adjust these fields.
	 */
	अगर (ext2fs_has_feature_journal_dev(fs->super)) {
		jsb->s_nr_users = 0;
		jsb->s_first = htonl(ext2fs_journal_sb_start(fs->blocksize) + 1);
	}

	*ret_jsb = (अक्षर *) jsb;
	वापस 0;
}

errcode_t ext2fs_create_journal_superblock(ext2_filsys fs, __u32 अंक_blocks,
					पूर्णांक flags, अक्षर **ret_sb)
{
	काष्ठा ext2fs_journal_params jparams;

	jparams.अंक_journal_blocks = अंक_blocks;
	jparams.अंक_fc_blocks = 0;

	वापस ext2fs_create_journal_superblock2(fs, &jparams, flags, ret_sb);
}

/*
 * This function writes a journal using POSIX routines.  It is used
 * for creating external journals and creating journals on live
 * filesystems.
 */
अटल errcode_t write_journal_file(ext2_filsys fs, अक्षर *filename,
				    काष्ठा ext2fs_journal_params *jparams,
				    पूर्णांक flags)
{
	errcode_t	retval;
	अक्षर		*buf = 0;
	पूर्णांक		fd, ret_आकार;
	blk_t		i;

	अगर ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		वापस retval;

	/* Open the device or journal file */
	अगर ((fd = खोलो(filename, O_WRONLY)) < 0) {
		retval = त्रुटि_सं;
		जाओ errfree;
	}

	/* Write the superblock out */
	retval = EXT2_ET_SHORT_WRITE;
	ret_आकार = ग_लिखो(fd, buf, fs->blocksize);
	अगर (ret_आकार < 0) {
		retval = त्रुटि_सं;
		जाओ errout;
	}
	अगर (ret_आकार != (पूर्णांक) fs->blocksize)
		जाओ errout;
	स_रखो(buf, 0, fs->blocksize);

	अगर (flags & EXT2_MKJOURNAL_LAZYINIT)
		जाओ success;

	क्रम (i = 1; i < jparams->अंक_journal_blocks + jparams->अंक_fc_blocks; i++) {
		ret_आकार = ग_लिखो(fd, buf, fs->blocksize);
		अगर (ret_आकार < 0) {
			retval = त्रुटि_सं;
			जाओ errout;
		}
		अगर (ret_आकार != (पूर्णांक) fs->blocksize)
			जाओ errout;
	}

success:
	retval = 0;
errout:
	बंद(fd);
errfree:
	ext2fs_free_mem(&buf);
	वापस retval;
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
#घोषणा MAX_STRIDE_लंबाई (4194304 / (पूर्णांक) fs->blocksize)
errcode_t ext2fs_zero_blocks2(ext2_filsys fs, blk64_t blk, पूर्णांक अंक,
			      blk64_t *ret_blk, पूर्णांक *ret_गिनती)
{
	पूर्णांक		j, गिनती;
	अटल व्योम	*buf;
	अटल पूर्णांक	stride_लंबाई;
	errcode_t	retval;

	/* If fs is null, clean up the static buffer and return */
	अगर (!fs) {
		अगर (buf) {
			मुक्त(buf);
			buf = 0;
			stride_लंबाई = 0;
		}
		वापस 0;
	}

	/* Deal with zeroing less than 1 block */
	अगर (अंक <= 0)
		वापस 0;

	/* Try a zero out command, if supported */
	retval = io_channel_zeroout(fs->io, blk, अंक);
	अगर (retval == 0)
		वापस 0;

	/* Allocate the zeroizing buffer if necessary */
	अगर (अंक > stride_लंबाई && stride_लंबाई < MAX_STRIDE_लंबाई) {
		व्योम *p;
		पूर्णांक new_stride = अंक;

		अगर (new_stride > MAX_STRIDE_लंबाई)
			new_stride = MAX_STRIDE_लंबाई;
		p = पुनः_स्मृति(buf, fs->blocksize * new_stride);
		अगर (!p)
			वापस EXT2_ET_NO_MEMORY;
		buf = p;
		stride_लंबाई = new_stride;
		स_रखो(buf, 0, fs->blocksize * stride_लंबाई);
	}
	/* OK, do the write loop */
	j=0;
	जबतक (j < अंक) {
		अगर (blk % stride_लंबाई) {
			गिनती = stride_लंबाई - (blk % stride_लंबाई);
			अगर (गिनती > (अंक - j))
				गिनती = अंक - j;
		} अन्यथा {
			गिनती = अंक - j;
			अगर (गिनती > stride_लंबाई)
				गिनती = stride_लंबाई;
		}
		retval = io_channel_write_blk64(fs->io, blk, गिनती, buf);
		अगर (retval) {
			अगर (ret_गिनती)
				*ret_गिनती = गिनती;
			अगर (ret_blk)
				*ret_blk = blk;
			वापस retval;
		}
		j += गिनती; blk += गिनती;
	}
	वापस 0;
}

errcode_t ext2fs_zero_blocks(ext2_filsys fs, blk_t blk, पूर्णांक अंक,
			     blk_t *ret_blk, पूर्णांक *ret_गिनती)
{
	blk64_t ret_blk2;
	errcode_t retval;

	retval = ext2fs_zero_blocks2(fs, blk, अंक, &ret_blk2, ret_गिनती);
	अगर (retval)
		*ret_blk = (blk_t) ret_blk2;
	वापस retval;
}

/*
 * Calculate the initial goal block to be roughly at the middle of the
 * filesystem.  Pick a group that has the largest number of free
 * blocks.
 */
अटल blk64_t get_midpoint_journal_block(ext2_filsys fs)
{
	dgrp_t	group, start, end, i, log_flex;

	group = ext2fs_group_of_blk2(fs, (ext2fs_blocks_गिनती(fs->super) -
					 fs->super->s_first_तथ्य_block) / 2);
	log_flex = 1U << fs->super->s_log_groups_per_flex;
	अगर (fs->super->s_log_groups_per_flex && (group > log_flex)) {
		group = group & ~(log_flex - 1);
		जबतक ((group < fs->group_desc_गिनती) &&
		       ext2fs_bg_free_blocks_गिनती(fs, group) == 0)
			group++;
		अगर (group == fs->group_desc_गिनती)
			group = 0;
		start = group;
	} अन्यथा
		start = (group > 0) ? group-1 : group;
	end = ((group+1) < fs->group_desc_गिनती) ? group+1 : group;
	group = start;
	क्रम (i = start + 1; i <= end; i++)
		अगर (ext2fs_bg_free_blocks_गिनती(fs, i) >
		    ext2fs_bg_free_blocks_गिनती(fs, group))
			group = i;
	वापस ext2fs_group_first_block2(fs, group);
}

/*
 * This function creates a journal using direct I/O routines.
 */
अटल errcode_t write_journal_inode(ext2_filsys fs, ext2_ino_t journal_ino,
				     काष्ठा ext2fs_journal_params *jparams,
				     blk64_t goal, पूर्णांक flags)
{
	अक्षर			*buf;
	errcode_t		retval;
	काष्ठा ext2_inode	inode;
	अचिन्हित दीर्घ दीर्घ	inode_आकार;
	पूर्णांक			falloc_flags = EXT2_FALLOCATE_FORCE_INIT;
	blk64_t			zblk;
	समय_प्रकार			now;

	अगर ((retval = ext2fs_create_journal_superblock2(fs, jparams, flags,
						       &buf)))
		वापस retval;

	अगर ((retval = ext2fs_read_bitmaps(fs)))
		जाओ out2;

	अगर ((retval = ext2fs_read_inode(fs, journal_ino, &inode)))
		जाओ out2;

	अगर (inode.i_blocks > 0) {
		retval = EEXIST;
		जाओ out2;
	}

	अगर (goal == ~0ULL)
		goal = get_midpoint_journal_block(fs);

	अगर (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;

	अगर (!(flags & EXT2_MKJOURNAL_LAZYINIT))
		falloc_flags |= EXT2_FALLOCATE_ZERO_BLOCKS;

	inode_आकार = (अचिन्हित दीर्घ दीर्घ)fs->blocksize *
			(jparams->अंक_journal_blocks + jparams->अंक_fc_blocks);
	now = ext2fsP_get_time(fs);
	ext2fs_inode_xtime_set(&inode, i_mtime, now);
	ext2fs_inode_xtime_set(&inode, i_ctime, now);
	inode.i_links_गिनती = 1;
	inode.i_mode = LINUX_S_IFREG | 0600;
	retval = ext2fs_inode_आकार_set(fs, &inode, inode_आकार);
	अगर (retval)
		जाओ out2;

	retval = ext2fs_fallocate(fs, falloc_flags, journal_ino,
				  &inode, goal, 0,
				  jparams->अंक_journal_blocks + jparams->अंक_fc_blocks);
	अगर (retval)
		जाओ out2;

	अगर ((retval = ext2fs_write_new_inode(fs, journal_ino, &inode)))
		जाओ out2;

	retval = ext2fs_bmap2(fs, journal_ino, &inode, शून्य, 0, 0, शून्य, &zblk);
	अगर (retval)
		जाओ out2;

	retval = io_channel_write_blk64(fs->io, zblk, 1, buf);
	अगर (retval)
		जाओ out2;

	स_नकल(fs->super->s_jnl_blocks, inode.i_block, EXT2_N_BLOCKS*4);
	fs->super->s_jnl_blocks[15] = inode.i_आकार_ऊपरी;
	fs->super->s_jnl_blocks[16] = inode.i_आकार;
	fs->super->s_jnl_backup_type = EXT3_JNL_BACKUP_BLOCKS;
	ext2fs_mark_super_dirty(fs);

out2:
	ext2fs_free_mem(&buf);
	वापस retval;
}

/*
 * Find a reasonable journal file size (in blocks) given the number of blocks
 * in the filesystem.  For very small filesystems, it is not reasonable to
 * have a journal that fills more than half of the filesystem.
 *
 * n.b. comments assume 4k blocks
 */
पूर्णांक ext2fs_default_journal_आकार(__u64 अंक_blocks)
{
	अगर (अंक_blocks < 2048)
		वापस -1;
	अगर (अंक_blocks < 32768)		/* 128 MB */
		वापस (1024);			/* 4 MB */
	अगर (अंक_blocks < 256*1024)	/* 1 GB */
		वापस (4096);			/* 16 MB */
	अगर (अंक_blocks < 512*1024)	/* 2 GB */
		वापस (8192);			/* 32 MB */
	अगर (अंक_blocks < 4096*1024)	/* 16 GB */
		वापस (16384);			/* 64 MB */
	अगर (अंक_blocks < 8192*1024)	/* 32 GB */
		वापस (32768);			/* 128 MB */
	अगर (अंक_blocks < 16384*1024)	/* 64 GB */
		वापस (65536);			/* 256 MB */
	अगर (अंक_blocks < 32768*1024)	/* 128 GB */
		वापस (131072);		/* 512 MB */
	वापस 262144;				/* 1 GB */
}

errcode_t ext2fs_get_journal_params(काष्ठा ext2fs_journal_params *params,
		ext2_filsys fs)
{
	blk_t योग_blks;
	पूर्णांक ret;

	स_रखो(params, 0, माप(*params));
	अगर (ext2fs_has_feature_journal_dev(fs->super)) {
		योग_blks = ext2fs_blocks_गिनती(fs->super);
		अगर (योग_blks < JBD2_MIN_JOURNAL_BLOCKS)
			वापस EXT2_ET_JOURNAL_TOO_SMALL;

		अगर (!ext2fs_has_feature_fast_commit(fs->super)) {
			params->अंक_journal_blocks = योग_blks;
			params->अंक_fc_blocks = 0;
			वापस 0;
		}
		params->अंक_journal_blocks = ext2fs_blocks_गिनती(fs->super) *
				EXT2_JOURNAL_TO_FC_BLKS_RATIO /
				(EXT2_JOURNAL_TO_FC_BLKS_RATIO + 1);
		अगर (JBD2_MIN_JOURNAL_BLOCKS > params->अंक_journal_blocks)
			params->अंक_journal_blocks = JBD2_MIN_JOURNAL_BLOCKS;
		params->अंक_fc_blocks = योग_blks - params->अंक_journal_blocks;
		वापस 0;
	}

	ret = ext2fs_default_journal_आकार(ext2fs_blocks_गिनती(fs->super));
	अगर (ret < 0)
		वापस EXT2_ET_JOURNAL_TOO_SMALL;

	params->अंक_journal_blocks = ret;
	अगर (ext2fs_has_feature_fast_commit(fs->super))
		params->अंक_fc_blocks = params->अंक_journal_blocks /
			EXT2_JOURNAL_TO_FC_BLKS_RATIO;
	वापस 0;
}

पूर्णांक ext2fs_journal_sb_start(पूर्णांक blocksize)
{
	अगर (blocksize == EXT2_MIN_BLOCK_आकार)
		वापस 2;
	वापस 1;
}

/*
 * This function adds a journal device to a filesystem
 */
errcode_t ext2fs_add_journal_device(ext2_filsys fs, ext2_filsys journal_dev)
{
	काष्ठा stat	st;
	errcode_t	retval;
	अक्षर		buf[SUPERBLOCK_आकार];
	journal_superblock_t	*jsb;
	पूर्णांक		start;
	__u32		i, nr_users;

	/* Make sure the device exists and is a block device */
	अगर (stat(journal_dev->device_name, &st) < 0)
		वापस त्रुटि_सं;

	अगर (!S_ISBLK(st.st_mode))
		वापस EXT2_ET_JOURNAL_NOT_BLOCK; /* Must be a block device */

	/* Get the journal superblock */
	start = ext2fs_journal_sb_start(journal_dev->blocksize);
	अगर ((retval = io_channel_read_blk64(journal_dev->io, start,
					    -SUPERBLOCK_आकार,
					    buf)))
		वापस retval;

	jsb = (journal_superblock_t *) buf;
	अगर ((jsb->s_header.h_magic != (अचिन्हित) ntohl(JBD2_MAGIC_संख्या)) ||
	    (jsb->s_header.h_blocktype != (अचिन्हित) ntohl(JBD2_SUPERBLOCK_V2)))
		वापस EXT2_ET_NO_JOURNAL_SB;

	अगर (ntohl(jsb->s_blocksize) != (अचिन्हित दीर्घ) fs->blocksize)
		वापस EXT2_ET_UNEXPECTED_BLOCK_आकार;

	/* Check and see if this filesystem has already been added */
	nr_users = ntohl(jsb->s_nr_users);
	अगर (nr_users > JBD2_USERS_MAX)
		वापस EXT2_ET_CORRUPT_JOURNAL_SB;
	क्रम (i=0; i < nr_users; i++) {
		अगर (स_भेद(fs->super->s_uuid,
			   &jsb->s_users[i*16], 16) == 0)
			अवरोध;
	}
	अगर (i >= nr_users) {
		स_नकल(&jsb->s_users[nr_users*16],
		       fs->super->s_uuid, 16);
		jsb->s_nr_users = htonl(nr_users+1);
	}

	/* Writeback the journal superblock */
	अगर ((retval = io_channel_write_blk64(journal_dev->io, start,
					    -SUPERBLOCK_आकार, buf)))
		वापस retval;

	fs->super->s_journal_inum = 0;
	fs->super->s_journal_dev = st.st_rdev;
	स_नकल(fs->super->s_journal_uuid, jsb->s_uuid,
	       माप(fs->super->s_journal_uuid));
	स_रखो(fs->super->s_jnl_blocks, 0, माप(fs->super->s_jnl_blocks));
	ext2fs_set_feature_journal(fs->super);
	ext2fs_mark_super_dirty(fs);
	वापस 0;
}

/*
 * This function adds a journal inode to a filesystem, using either
 * POSIX routines if the filesystem is mounted, or using direct I/O
 * functions if it is not.
 */
errcode_t ext2fs_add_journal_inode3(ext2_filsys fs, काष्ठा ext2fs_journal_params *jparams,
				    blk64_t goal, पूर्णांक flags)
{
	errcode_t		retval;
	ext2_ino_t		journal_ino;
	काष्ठा stat		st;
	अक्षर			jfile[1024];
	पूर्णांक			mount_flags;
	पूर्णांक			fd = -1;

	अगर (flags & EXT2_MKJOURNAL_NO_MNT_CHECK)
		mount_flags = 0;
	अन्यथा अगर ((retval = ext2fs_check_mount_point(fs->device_name,
						    &mount_flags,
						    jfile, माप(jfile)-10)))
		वापस retval;

	अगर (mount_flags & EXT2_MF_MOUNTED) {
#अगर HAVE_EXT2_IOCTLS
		पूर्णांक f = 0;
#पूर्ण_अगर
		म_जोड़ो(jfile, "/.journal");

		/*
		 * If .../.journal already exists, make sure any
		 * immutable or append-only flags are cleared.
		 */
#अगर defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		(व्योम) chflags (jfile, 0);
#अन्यथा
#अगर HAVE_EXT2_IOCTLS
		fd = खोलो(jfile, O_RDONLY);
		अगर (fd >= 0) {
			retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
			बंद(fd);
			अगर (retval)
				वापस त्रुटि_सं;
		}
#पूर्ण_अगर
#पूर्ण_अगर

		/* Create the journal file */
		अगर ((fd = खोलो(jfile, O_CREAT|O_WRONLY, 0600)) < 0)
			वापस त्रुटि_सं;

		/* Note that we can't do lazy journal initialization for mounted
		 * filesystems, since the zero writing is also allocating the
		 * journal blocks.  We could use fallocate, but not all kernels
		 * support that, and creating a journal on a mounted ext2
		 * filesystems is extremely rare these days...  Ignore it. */
		flags &= ~EXT2_MKJOURNAL_LAZYINIT;

		अगर ((retval = write_journal_file(fs, jfile, jparams, flags)))
			जाओ errout;

		/* Get inode number of the journal file */
		अगर (ख_स्थिति(fd, &st) < 0) {
			retval = त्रुटि_सं;
			जाओ errout;
		}

#अगर defined(HAVE_CHFLAGS) && defined(UF_NODUMP)
		retval = fchflags (fd, UF_NODUMP|UF_IMMUTABLE);
#अन्यथा
#अगर HAVE_EXT2_IOCTLS
		अगर (ioctl(fd, EXT2_IOC_GETFLAGS, &f) < 0) {
			retval = त्रुटि_सं;
			जाओ errout;
		}
		f |= EXT2_NODUMP_FL | EXT2_IMMUTABLE_FL;
		retval = ioctl(fd, EXT2_IOC_SETFLAGS, &f);
#पूर्ण_अगर
#पूर्ण_अगर
		अगर (retval) {
			retval = त्रुटि_सं;
			जाओ errout;
		}

		अगर (बंद(fd) < 0) {
			retval = त्रुटि_सं;
			fd = -1;
			जाओ errout;
		}
		journal_ino = st.st_ino;
		स_रखो(fs->super->s_jnl_blocks, 0,
		       माप(fs->super->s_jnl_blocks));
	} अन्यथा {
		अगर ((mount_flags & EXT2_MF_BUSY) &&
		    !(fs->flags & EXT2_FLAG_EXCLUSIVE)) {
			retval = EBUSY;
			जाओ errout;
		}
		journal_ino = EXT2_JOURNAL_INO;
		अगर ((retval = write_journal_inode(fs, journal_ino,
						  jparams, goal, flags)))
			वापस retval;
	}

	fs->super->s_journal_inum = journal_ino;
	fs->super->s_journal_dev = 0;
	स_रखो(fs->super->s_journal_uuid, 0,
	       माप(fs->super->s_journal_uuid));
	ext2fs_set_feature_journal(fs->super);

	ext2fs_mark_super_dirty(fs);
	वापस 0;
errout:
	अगर (fd >= 0)
		बंद(fd);
	वापस retval;
}

errcode_t ext2fs_add_journal_inode2(ext2_filsys fs, blk_t अंक_blocks,
				    blk64_t goal, पूर्णांक flags)
{
	काष्ठा ext2fs_journal_params jparams;

	jparams.अंक_journal_blocks = अंक_blocks;
	jparams.अंक_fc_blocks = 0;

	वापस ext2fs_add_journal_inode3(fs, &jparams, goal, flags);
}

errcode_t ext2fs_add_journal_inode(ext2_filsys fs, blk_t अंक_blocks, पूर्णांक flags)
{
	वापस ext2fs_add_journal_inode2(fs, अंक_blocks, ~0ULL, flags);
}


#अगर_घोषित DEBUG
मुख्य(पूर्णांक argc, अक्षर **argv)
{
	errcode_t	retval;
	अक्षर		*device_name;
	ext2_filsys	fs;

	अगर (argc < 2) {
		ख_लिखो(मानक_त्रुटि, "Usage: %s filesystem\n", argv[0]);
		निकास(1);
	}
	device_name = argv[1];

	retval = ext2fs_open (device_name, EXT2_FLAG_RW, 0, 0,
			      unix_io_manager, &fs);
	अगर (retval) {
		com_err(argv[0], retval, "while opening %s", device_name);
		निकास(1);
	}

	retval = ext2fs_add_journal_inode(fs, JBD2_MIN_JOURNAL_BLOCKS, 0);
	अगर (retval) {
		com_err(argv[0], retval, "while adding journal to %s",
			device_name);
		निकास(1);
	}
	retval = ext2fs_flush(fs);
	अगर (retval) {
		म_लिखो("Warning, had trouble writing out superblocks.\n");
	}
	ext2fs_close_free(&fs);
	निकास(0);

}
#पूर्ण_अगर
