/*
 * Helper functions for multiple mount protection (MMP).
 *
 * Copyright (C) 2011 Whamcloud, Inc.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#अगर_अघोषित _GNU_SOURCE
#घोषणा _GNU_SOURCE
#पूर्ण_अगर
#अगर_अघोषित _DEFAULT_SOURCE
#घोषणा _DEFAULT_SOURCE	/* since glibc 2.20 _SVID_SOURCE is deprecated */
#पूर्ण_अगर

#समावेश "config.h"

#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <sys/time.h>

#समावेश <sys/types.h>
#समावेश <sys/stat.h>
#समावेश <fcntl.h>

#समावेश "ext2fs/ext2_fs.h"
#समावेश "ext2fs/ext2fs.h"

#अगर_अघोषित O_DIRECT
#घोषणा O_DIRECT 0
#पूर्ण_अगर

#अगर __GNUC_PREREQ (4, 6)
#आशय GCC diagnostic धकेलो
#अगर_अघोषित CONFIG_MMP
#आशय GCC diagnostic ignored "-Wunused-parameter"
#पूर्ण_अगर
#पूर्ण_अगर

errcode_t ext2fs_mmp_get_mem(ext2_filsys fs, व्योम **ptr)
{
	पूर्णांक align = ext2fs_get_dio_alignment(fs->mmp_fd);

	वापस ext2fs_get_memalign(fs->blocksize, align, ptr);
}

errcode_t ext2fs_mmp_read(ext2_filsys fs, blk64_t mmp_blk, व्योम *buf)
{
#अगर_घोषित CONFIG_MMP
	काष्ठा mmp_struct *mmp_cmp;
	errcode_t retval = 0;

	अगर ((mmp_blk <= fs->super->s_first_तथ्य_block) ||
	    (mmp_blk >= ext2fs_blocks_गिनती(fs->super)))
		वापस EXT2_ET_MMP_BAD_BLOCK;

	/* ext2fs_open() reserves fd0,1,2 to avoid stdio collision, so checking
	 * mmp_fd <= 0 is OK to validate that the fd is valid.  This opens its
	 * own fd to read the MMP block to ensure that it is using O_DIRECT,
	 * regardless of how the io_manager is doing reads, to avoid caching of
	 * the MMP block by the io_manager or the VM.  It needs to be fresh. */
	अगर (fs->mmp_fd <= 0) {
		काष्ठा stat st;
		पूर्णांक flags = O_RDONLY | O_DIRECT;

		/*
		 * There is no reason for using O_DIRECT if we're working with
		 * regular file. Disabling it also avoids problems with
		 * alignment when the device of the host file system has sector
		 * size larger than blocksize of the fs we're working with.
		 */
		अगर (stat(fs->device_name, &st) == 0 &&
		    S_ISREG(st.st_mode))
			flags &= ~O_DIRECT;

		fs->mmp_fd = खोलो(fs->device_name, flags);
		अगर (fs->mmp_fd < 0) {
			retval = EXT2_ET_MMP_OPEN_DIRECT;
			जाओ out;
		}
	}

	अगर (fs->mmp_cmp == शून्य) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_cmp);
		अगर (retval)
			वापस retval;
	}

	अगर ((blk64_t) ext2fs_llseek(fs->mmp_fd, mmp_blk * fs->blocksize,
				    शुरू_से) !=
	    mmp_blk * fs->blocksize) {
		retval = EXT2_ET_LLSEEK_FAILED;
		जाओ out;
	}

	अगर (पढ़ो(fs->mmp_fd, fs->mmp_cmp, fs->blocksize) != fs->blocksize) {
		retval = EXT2_ET_SHORT_READ;
		जाओ out;
	}

	mmp_cmp = fs->mmp_cmp;

	अगर (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_mmp_csum_verify(fs, mmp_cmp))
		retval = EXT2_ET_MMP_CSUM_INVALID;

#अगर_घोषित शब्दगण_BIGENDIAN
	ext2fs_swap_mmp(mmp_cmp);
#पूर्ण_अगर

	अगर (buf != शून्य && buf != fs->mmp_cmp)
		स_नकल(buf, fs->mmp_cmp, fs->blocksize);

	अगर (mmp_cmp->mmp_magic != EXT4_MMP_MAGIC) {
		retval = EXT2_ET_MMP_MAGIC_INVALID;
		जाओ out;
	}

out:
	वापस retval;
#अन्यथा
	वापस EXT2_ET_OP_NOT_SUPPORTED;
#पूर्ण_अगर
}

errcode_t ext2fs_mmp_write(ext2_filsys fs, blk64_t mmp_blk, व्योम *buf)
{
#अगर_घोषित CONFIG_MMP
	काष्ठा mmp_struct *mmp_s = buf;
	काष्ठा timeval tv;
	errcode_t retval = 0;

	gettimeofday(&tv, 0);
	mmp_s->mmp_time = tv.tv_sec;
	fs->mmp_last_written = tv.tv_sec;

	अगर (fs->super->s_mmp_block < fs->super->s_first_तथ्य_block ||
	    fs->super->s_mmp_block > ext2fs_blocks_गिनती(fs->super))
		वापस EXT2_ET_MMP_BAD_BLOCK;

#अगर_घोषित शब्दगण_BIGENDIAN
	ext2fs_swap_mmp(mmp_s);
#पूर्ण_अगर

	retval = ext2fs_mmp_csum_set(fs, mmp_s);
	अगर (retval)
		वापस retval;

	/* I was tempted to make this use O_DIRECT and the mmp_fd, but
	 * this caused no end of grief, while leaving it as-is works. */
	retval = io_channel_write_blk64(fs->io, mmp_blk, -(पूर्णांक)माप(काष्ठा mmp_struct), buf);

#अगर_घोषित शब्दगण_BIGENDIAN
	ext2fs_swap_mmp(mmp_s);
#पूर्ण_अगर

	/* Make sure the block gets to disk quickly */
	io_channel_flush(fs->io);
	वापस retval;
#अन्यथा
	वापस EXT2_ET_OP_NOT_SUPPORTED;
#पूर्ण_अगर
}

#अगर_घोषित HAVE_SRANDOM
#घोषणा बेक्रम(x)	srandom(x)
#घोषणा अक्रम()		random()
#पूर्ण_अगर

अचिन्हित ext2fs_mmp_new_seq(व्योम)
{
#अगर_घोषित CONFIG_MMP
	अचिन्हित new_seq;
	काष्ठा timeval tv;
	अचिन्हित दीर्घ pid = getpid();

	gettimeofday(&tv, 0);
	pid = (pid >> 16) | ((pid & 0xFFFF) << 16);
	बेक्रम(pid ^ getuid() ^ tv.tv_sec ^ tv.tv_usec);

	gettimeofday(&tv, 0);
	/* Crank the random number generator a few times */
	क्रम (new_seq = (tv.tv_sec ^ tv.tv_usec) & 0x1F; new_seq > 0; new_seq--)
		अक्रम();

	करो {
		new_seq = अक्रम();
	} जबतक (new_seq > EXT4_MMP_SEQ_MAX);

	वापस new_seq;
#अन्यथा
	वापस EXT2_ET_OP_NOT_SUPPORTED;
#पूर्ण_अगर
}

#अगर_घोषित CONFIG_MMP
अटल errcode_t ext2fs_mmp_reset(ext2_filsys fs)
{
	काष्ठा mmp_struct *mmp_s = शून्य;
	errcode_t retval = 0;

	अगर (fs->mmp_buf == शून्य) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		अगर (retval)
			जाओ out;
	}

	स_रखो(fs->mmp_buf, 0, fs->blocksize);
	mmp_s = fs->mmp_buf;

	mmp_s->mmp_magic = EXT4_MMP_MAGIC;
	mmp_s->mmp_seq = EXT4_MMP_SEQ_CLEAN;
	mmp_s->mmp_time = 0;
#अगर_घोषित HAVE_GETHOSTNAME
	gethostname((अक्षर *) mmp_s->mmp_nodename, माप(mmp_s->mmp_nodename));
#अन्यथा
	mmp_s->mmp_nodename[0] = '\0';
#पूर्ण_अगर
	म_नकलन((अक्षर *) mmp_s->mmp_bdevname, fs->device_name,
		माप(mmp_s->mmp_bdevname));

	mmp_s->mmp_check_interval = fs->super->s_mmp_update_interval;
	अगर (mmp_s->mmp_check_interval < EXT4_MMP_MIN_CHECK_INTERVAL)
		mmp_s->mmp_check_interval = EXT4_MMP_MIN_CHECK_INTERVAL;

	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_block, fs->mmp_buf);
out:
	वापस retval;
}
#पूर्ण_अगर

errcode_t ext2fs_mmp_update(ext2_filsys fs)
{
	वापस ext2fs_mmp_update2(fs, 0);
}

errcode_t ext2fs_mmp_clear(ext2_filsys fs)
{
#अगर_घोषित CONFIG_MMP
	errcode_t retval = 0;

	अगर (!(fs->flags & EXT2_FLAG_RW))
		वापस EXT2_ET_RO_FILSYS;

	retval = ext2fs_mmp_reset(fs);

	वापस retval;
#अन्यथा
	वापस EXT2_ET_OP_NOT_SUPPORTED;
#पूर्ण_अगर
}

errcode_t ext2fs_mmp_init(ext2_filsys fs)
{
#अगर_घोषित CONFIG_MMP
	काष्ठा ext2_super_block *sb = fs->super;
	blk64_t mmp_block;
	errcode_t retval;

	अगर (sb->s_mmp_update_interval == 0)
		sb->s_mmp_update_interval = EXT4_MMP_UPDATE_INTERVAL;
	/* This is probably excessively large, but who knows? */
	अन्यथा अगर (sb->s_mmp_update_interval > EXT4_MMP_MAX_UPDATE_INTERVAL)
		वापस EXT2_ET_INVALID_ARGUMENT;

	अगर (fs->mmp_buf == शून्य) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		अगर (retval)
			जाओ out;
	}

	retval = ext2fs_alloc_block2(fs, 0, fs->mmp_buf, &mmp_block);
	अगर (retval)
		जाओ out;

	sb->s_mmp_block = mmp_block;

	retval = ext2fs_mmp_reset(fs);
	अगर (retval)
		जाओ out;

out:
	वापस retval;
#अन्यथा
	वापस EXT2_ET_OP_NOT_SUPPORTED;
#पूर्ण_अगर
}

#अगर_अघोषित min
#घोषणा min(x, y) ((x) < (y) ? (x) : (y))
#पूर्ण_अगर

/*
 * Make sure that the fs is not mounted or being fsck'ed while opening the fs.
 */
errcode_t ext2fs_mmp_start(ext2_filsys fs)
{
#अगर_घोषित CONFIG_MMP
	काष्ठा mmp_struct *mmp_s;
	अचिन्हित seq;
	अचिन्हित पूर्णांक mmp_check_interval;
	errcode_t retval = 0;

	अगर (fs->mmp_buf == शून्य) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		अगर (retval)
			जाओ mmp_error;
	}

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_block, fs->mmp_buf);
	अगर (retval)
		जाओ mmp_error;

	mmp_s = fs->mmp_buf;

	mmp_check_interval = fs->super->s_mmp_update_interval;
	अगर (mmp_check_interval < EXT4_MMP_MIN_CHECK_INTERVAL)
		mmp_check_interval = EXT4_MMP_MIN_CHECK_INTERVAL;

	seq = mmp_s->mmp_seq;
	अगर (seq == EXT4_MMP_SEQ_CLEAN)
		जाओ clean_seq;
	अगर (seq == EXT4_MMP_SEQ_FSCK) {
		retval = EXT2_ET_MMP_FSCK_ON;
		जाओ mmp_error;
	}

	अगर (seq > EXT4_MMP_SEQ_FSCK) {
		retval = EXT2_ET_MMP_UNKNOWN_SEQ;
		जाओ mmp_error;
	}

	/*
	 * If check_interval in MMP block is larger, use that instead of
	 * check_interval from the superblock.
	 */
	अगर (mmp_s->mmp_check_interval > mmp_check_interval)
		mmp_check_interval = mmp_s->mmp_check_interval;

	sleep(min(mmp_check_interval * 2 + 1, mmp_check_interval + 60));

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_block, fs->mmp_buf);
	अगर (retval)
		जाओ mmp_error;

	अगर (seq != mmp_s->mmp_seq) {
		retval = EXT2_ET_MMP_FAILED;
		जाओ mmp_error;
	}

clean_seq:
	अगर (!(fs->flags & EXT2_FLAG_RW))
		जाओ mmp_error;

	mmp_s->mmp_seq = seq = ext2fs_mmp_new_seq();
#अगर_घोषित HAVE_GETHOSTNAME
	gethostname((अक्षर *) mmp_s->mmp_nodename, माप(mmp_s->mmp_nodename));
#अन्यथा
	म_नकल((अक्षर *) mmp_s->mmp_nodename, "unknown host");
#पूर्ण_अगर
	म_नकलन((अक्षर *) mmp_s->mmp_bdevname, fs->device_name,
		माप(mmp_s->mmp_bdevname));

	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_block, fs->mmp_buf);
	अगर (retval)
		जाओ mmp_error;

	sleep(min(2 * mmp_check_interval + 1, mmp_check_interval + 60));

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_block, fs->mmp_buf);
	अगर (retval)
		जाओ mmp_error;

	अगर (seq != mmp_s->mmp_seq) {
		retval = EXT2_ET_MMP_FAILED;
		जाओ mmp_error;
	}

	mmp_s->mmp_seq = EXT4_MMP_SEQ_FSCK;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_block, fs->mmp_buf);
	अगर (retval)
		जाओ mmp_error;

	वापस 0;

mmp_error:
	वापस retval;
#अन्यथा
	वापस EXT2_ET_OP_NOT_SUPPORTED;
#पूर्ण_अगर
}

/*
 * Clear the MMP usage in the filesystem.  If this function returns an
 * error EXT2_ET_MMP_CHANGE_ABORT it means the filesystem was modified
 * by some other process while in use, and changes should be dropped, or
 * risk filesystem corruption.
 */
errcode_t ext2fs_mmp_stop(ext2_filsys fs)
{
#अगर_घोषित CONFIG_MMP
	काष्ठा mmp_struct *mmp, *mmp_cmp;
	errcode_t retval = 0;

	अगर (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP) ||
	    (fs->mmp_buf == शून्य) || (fs->mmp_cmp == शून्य))
		जाओ mmp_error;

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_block, शून्य);
	अगर (retval)
		जाओ mmp_error;

	/* Check if the MMP block is not changed. */
	mmp = fs->mmp_buf;
	mmp_cmp = fs->mmp_cmp;
	अगर (स_भेद(mmp, mmp_cmp, माप(*mmp_cmp))) {
		retval = EXT2_ET_MMP_CHANGE_ABORT;
		जाओ mmp_error;
	}

	mmp_cmp->mmp_seq = EXT4_MMP_SEQ_CLEAN;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_block, fs->mmp_cmp);

mmp_error:
	अगर (fs->mmp_fd > 0) {
		बंद(fs->mmp_fd);
		fs->mmp_fd = -1;
	}

	वापस retval;
#अन्यथा
	अगर (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		वापस 0;

	वापस EXT2_ET_OP_NOT_SUPPORTED;
#पूर्ण_अगर
}

#घोषणा EXT2_MIN_MMP_UPDATE_INTERVAL 60

/*
 * Update the on-disk mmp buffer, after checking that it hasn't been changed.
 */
errcode_t ext2fs_mmp_update2(ext2_filsys fs, पूर्णांक immediately)
{
#अगर_घोषित CONFIG_MMP
	काष्ठा mmp_struct *mmp, *mmp_cmp;
	काष्ठा timeval tv;
	errcode_t retval = 0;

	अगर (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		वापस 0;

	gettimeofday(&tv, 0);
	अगर (!immediately &&
	    tv.tv_sec - fs->mmp_last_written < EXT2_MIN_MMP_UPDATE_INTERVAL)
		वापस 0;

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_block, शून्य);
	अगर (retval)
		जाओ mmp_error;

	mmp = fs->mmp_buf;
	mmp_cmp = fs->mmp_cmp;

	अगर (स_भेद(mmp, mmp_cmp, माप(*mmp_cmp)))
		वापस EXT2_ET_MMP_CHANGE_ABORT;

	/*
	 * Believe it or not, ext2fs_mmp_read actually overwrites fs->mmp_cmp
	 * and leaves fs->mmp_buf untouched.  Hence we copy mmp_cmp into
	 * mmp_buf, update mmp_buf, and write mmp_buf out to disk.
	 */
	स_नकल(mmp, mmp_cmp, माप(*mmp_cmp));
	mmp->mmp_time = tv.tv_sec;
	mmp->mmp_seq = EXT4_MMP_SEQ_FSCK;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_block, fs->mmp_buf);

mmp_error:
	वापस retval;
#अन्यथा
	अगर (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		वापस 0;

	वापस EXT2_ET_OP_NOT_SUPPORTED;
#पूर्ण_अगर
}
#अगर __GNUC_PREREQ (4, 6)
#आशय GCC diagnostic निकालो
#पूर्ण_अगर
