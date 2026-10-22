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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE	/* since glibc 2.20 _SVID_SOURCE is deprecated */
#endif

#अन्तर्भाव "config.h"

#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <sys/time.h>

#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <fcntl.h>

#अन्तर्भाव "ext2fs/ext2_fs.h"
#अन्तर्भाव "ext2fs/ext2fs.h"

#ifndef O_DIRECT
#define O_DIRECT 0
#endif

#यदि __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic धकेलो
#ifndef CONFIG_MMP
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
#endif

errcode_t ext2fs_mmp_get_mem(ext2_filsys fs, शून्य **ptr)
{
	पूर्णाङ्क align = ext2fs_get_dio_alignment(fs->mmp_fd);

	प्रत्यागम ext2fs_get_memalign(fs->blocksize, align, ptr);
}

errcode_t ext2fs_mmp_read(ext2_filsys fs, blk64_t mmp_blk, शून्य *buf)
{
#ifdef CONFIG_MMP
	संरचना mmp_struct *mmp_cmp;
	errcode_t retval = 0;

	यदि ((mmp_blk <= fs->super->s_first_data_block) ||
	    (mmp_blk >= ext2fs_blocks_गिनती(fs->super)))
		प्रत्यागम EXT2_ET_MMP_BAD_BLOCK;

	/* ext2fs_open() reserves fd0,1,2 to avoid stdio collision, so checking
	 * mmp_fd <= 0 is OK to validate that the fd is valid.  This opens its
	 * own fd to read the MMP block to ensure that it is using O_DIRECT,
	 * regardless of how the io_manager is doing reads, to avoid caching of
	 * the MMP block by the io_manager or the VM.  It needs to be fresh. */
	यदि (fs->mmp_fd <= 0) {
		संरचना stat st;
		पूर्णाङ्क flags = O_RDONLY | O_DIRECT;

		/*
		 * There is no reason for using O_DIRECT if we're working with
		 * regular file. Disabling it also avoids problems with
		 * alignment when the device of the host file system has sector
		 * size larger than blocksize of the fs we're working with.
		 */
		यदि (stat(fs->device_name, &st) == 0 &&
		    S_ISREG(st.st_mode))
			flags &= ~O_DIRECT;

		fs->mmp_fd = खोलो(fs->device_name, flags);
		यदि (fs->mmp_fd < 0) {
			retval = EXT2_ET_MMP_OPEN_DIRECT;
			गच्छ out;
		}
	}

	यदि (fs->mmp_cmp == रिक्त) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_cmp);
		यदि (retval)
			प्रत्यागम retval;
	}

	यदि ((blk64_t) ext2fs_llseek(fs->mmp_fd, mmp_blk * fs->blocksize,
				    शुरू_से) !=
	    mmp_blk * fs->blocksize) {
		retval = EXT2_ET_LLSEEK_FAILED;
		गच्छ out;
	}

	यदि (पठ(fs->mmp_fd, fs->mmp_cmp, fs->blocksize) != fs->blocksize) {
		retval = EXT2_ET_SHORT_READ;
		गच्छ out;
	}

	mmp_cmp = fs->mmp_cmp;

	यदि (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_mmp_csum_verify(fs, mmp_cmp))
		retval = EXT2_ET_MMP_CSUM_INVALID;

#ifdef शब्दगण_BIGENDIAN
	ext2fs_swap_mmp(mmp_cmp);
#endif

	यदि (buf != रिक्त && buf != fs->mmp_cmp)
		स_नकल(buf, fs->mmp_cmp, fs->blocksize);

	यदि (mmp_cmp->mmp_magic != EXT4_MMP_MAGIC) {
		retval = EXT2_ET_MMP_MAGIC_INVALID;
		गच्छ out;
	}

out:
	प्रत्यागम retval;
#अन्यथा
	प्रत्यागम EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

errcode_t ext2fs_mmp_write(ext2_filsys fs, blk64_t mmp_blk, शून्य *buf)
{
#ifdef CONFIG_MMP
	संरचना mmp_struct *mmp_s = buf;
	संरचना timeval tv;
	errcode_t retval = 0;

	gettimeofday(&tv, 0);
	mmp_s->mmp_time = tv.tv_sec;
	fs->mmp_last_written = tv.tv_sec;

	यदि (fs->super->s_mmp_block < fs->super->s_first_data_block ||
	    fs->super->s_mmp_block > ext2fs_blocks_गिनती(fs->super))
		प्रत्यागम EXT2_ET_MMP_BAD_BLOCK;

#ifdef शब्दगण_BIGENDIAN
	ext2fs_swap_mmp(mmp_s);
#endif

	retval = ext2fs_mmp_csum_set(fs, mmp_s);
	यदि (retval)
		प्रत्यागम retval;

	/* I was tempted to make this use O_DIRECT and the mmp_fd, but
	 * this caused no end of grief, while leaving it as-is works. */
	retval = io_channel_write_blk64(fs->io, mmp_blk, -(पूर्णाङ्क)परिमाण(संरचना mmp_struct), buf);

#ifdef शब्दगण_BIGENDIAN
	ext2fs_swap_mmp(mmp_s);
#endif

	/* Make sure the block gets to disk quickly */
	io_channel_flush(fs->io);
	प्रत्यागम retval;
#अन्यथा
	प्रत्यागम EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

#ifdef HAVE_SRANDOM
#define बेक्रम(x)	srandom(x)
#define अक्रम()		random()
#endif

अचिन्हित ext2fs_mmp_new_seq(शून्य)
{
#ifdef CONFIG_MMP
	अचिन्हित new_seq;
	संरचना timeval tv;
	अचिन्हित दीर्घ pid = getpid();

	gettimeofday(&tv, 0);
	pid = (pid >> 16) | ((pid & 0xFFFF) << 16);
	बेक्रम(pid ^ getuid() ^ tv.tv_sec ^ tv.tv_usec);

	gettimeofday(&tv, 0);
	/* Crank the random number generator a few times */
	प्रति (new_seq = (tv.tv_sec ^ tv.tv_usec) & 0x1F; new_seq > 0; new_seq--)
		अक्रम();

	कुरु {
		new_seq = अक्रम();
	} यावत् (new_seq > EXT4_MMP_SEQ_MAX);

	प्रत्यागम new_seq;
#अन्यथा
	प्रत्यागम EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

#ifdef CONFIG_MMP
स्थिर errcode_t ext2fs_mmp_reset(ext2_filsys fs)
{
	संरचना mmp_struct *mmp_s = रिक्त;
	errcode_t retval = 0;

	यदि (fs->mmp_buf == रिक्त) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		यदि (retval)
			गच्छ out;
	}

	स_रखो(fs->mmp_buf, 0, fs->blocksize);
	mmp_s = fs->mmp_buf;

	mmp_s->mmp_magic = EXT4_MMP_MAGIC;
	mmp_s->mmp_seq = EXT4_MMP_SEQ_CLEAN;
	mmp_s->mmp_time = 0;
#ifdef HAVE_GETHOSTNAME
	gethostname((वर्ण *) mmp_s->mmp_nodename, परिमाण(mmp_s->mmp_nodename));
#अन्यथा
	mmp_s->mmp_nodename[0] = '\0';
#endif
	म_नकलन((वर्ण *) mmp_s->mmp_bdevname, fs->device_name,
		परिमाण(mmp_s->mmp_bdevname));

	mmp_s->mmp_check_interval = fs->super->s_mmp_update_interval;
	यदि (mmp_s->mmp_check_interval < EXT4_MMP_MIN_CHECK_INTERVAL)
		mmp_s->mmp_check_interval = EXT4_MMP_MIN_CHECK_INTERVAL;

	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_block, fs->mmp_buf);
out:
	प्रत्यागम retval;
}
#endif

errcode_t ext2fs_mmp_update(ext2_filsys fs)
{
	प्रत्यागम ext2fs_mmp_update2(fs, 0);
}

errcode_t ext2fs_mmp_clear(ext2_filsys fs)
{
#ifdef CONFIG_MMP
	errcode_t retval = 0;

	यदि (!(fs->flags & EXT2_FLAG_RW))
		प्रत्यागम EXT2_ET_RO_FILSYS;

	retval = ext2fs_mmp_reset(fs);

	प्रत्यागम retval;
#अन्यथा
	प्रत्यागम EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

errcode_t ext2fs_mmp_init(ext2_filsys fs)
{
#ifdef CONFIG_MMP
	संरचना ext2_super_block *sb = fs->super;
	blk64_t mmp_block;
	errcode_t retval;

	यदि (sb->s_mmp_update_interval == 0)
		sb->s_mmp_update_interval = EXT4_MMP_UPDATE_INTERVAL;
	/* This is probably excessively large, but who knows? */
	अन्यथा यदि (sb->s_mmp_update_interval > EXT4_MMP_MAX_UPDATE_INTERVAL)
		प्रत्यागम EXT2_ET_INVALID_ARGUMENT;

	यदि (fs->mmp_buf == रिक्त) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		यदि (retval)
			गच्छ out;
	}

	retval = ext2fs_alloc_block2(fs, 0, fs->mmp_buf, &mmp_block);
	यदि (retval)
		गच्छ out;

	sb->s_mmp_block = mmp_block;

	retval = ext2fs_mmp_reset(fs);
	यदि (retval)
		गच्छ out;

out:
	प्रत्यागम retval;
#अन्यथा
	प्रत्यागम EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

#ifndef min
#define min(x, y) ((x) < (y) ? (x) : (y))
#endif

/*
 * Make sure that the fs is not mounted or being fsck'ed while opening the fs.
 */
errcode_t ext2fs_mmp_start(ext2_filsys fs)
{
#ifdef CONFIG_MMP
	संरचना mmp_struct *mmp_s;
	अचिन्हित seq;
	अचिन्हित पूर्णाङ्क mmp_check_interval;
	errcode_t retval = 0;

	यदि (fs->mmp_buf == रिक्त) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		यदि (retval)
			गच्छ mmp_error;
	}

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_block, fs->mmp_buf);
	यदि (retval)
		गच्छ mmp_error;

	mmp_s = fs->mmp_buf;

	mmp_check_interval = fs->super->s_mmp_update_interval;
	यदि (mmp_check_interval < EXT4_MMP_MIN_CHECK_INTERVAL)
		mmp_check_interval = EXT4_MMP_MIN_CHECK_INTERVAL;

	seq = mmp_s->mmp_seq;
	यदि (seq == EXT4_MMP_SEQ_CLEAN)
		गच्छ clean_seq;
	यदि (seq == EXT4_MMP_SEQ_FSCK) {
		retval = EXT2_ET_MMP_FSCK_ON;
		गच्छ mmp_error;
	}

	यदि (seq > EXT4_MMP_SEQ_FSCK) {
		retval = EXT2_ET_MMP_UNKNOWN_SEQ;
		गच्छ mmp_error;
	}

	/*
	 * If check_interval in MMP block is larger, use that instead of
	 * check_interval from the superblock.
	 */
	यदि (mmp_s->mmp_check_interval > mmp_check_interval)
		mmp_check_interval = mmp_s->mmp_check_interval;

	sleep(min(mmp_check_interval * 2 + 1, mmp_check_interval + 60));

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_block, fs->mmp_buf);
	यदि (retval)
		गच्छ mmp_error;

	यदि (seq != mmp_s->mmp_seq) {
		retval = EXT2_ET_MMP_FAILED;
		गच्छ mmp_error;
	}

clean_seq:
	यदि (!(fs->flags & EXT2_FLAG_RW))
		गच्छ mmp_error;

	mmp_s->mmp_seq = seq = ext2fs_mmp_new_seq();
#ifdef HAVE_GETHOSTNAME
	gethostname((वर्ण *) mmp_s->mmp_nodename, परिमाण(mmp_s->mmp_nodename));
#अन्यथा
	म_नकल((वर्ण *) mmp_s->mmp_nodename, "unknown host");
#endif
	म_नकलन((वर्ण *) mmp_s->mmp_bdevname, fs->device_name,
		परिमाण(mmp_s->mmp_bdevname));

	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_block, fs->mmp_buf);
	यदि (retval)
		गच्छ mmp_error;

	sleep(min(2 * mmp_check_interval + 1, mmp_check_interval + 60));

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_block, fs->mmp_buf);
	यदि (retval)
		गच्छ mmp_error;

	यदि (seq != mmp_s->mmp_seq) {
		retval = EXT2_ET_MMP_FAILED;
		गच्छ mmp_error;
	}

	mmp_s->mmp_seq = EXT4_MMP_SEQ_FSCK;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_block, fs->mmp_buf);
	यदि (retval)
		गच्छ mmp_error;

	प्रत्यागम 0;

mmp_error:
	प्रत्यागम retval;
#अन्यथा
	प्रत्यागम EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

/*
 * Clear the MMP usage in the filesystem.  If this function returns an
 * error EXT2_ET_MMP_CHANGE_ABORT it means the filesystem was modified
 * by some other process while in use, and changes should be dropped, or
 * risk filesystem corruption.
 */
errcode_t ext2fs_mmp_stop(ext2_filsys fs)
{
#ifdef CONFIG_MMP
	संरचना mmp_struct *mmp, *mmp_cmp;
	errcode_t retval = 0;

	यदि (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP) ||
	    (fs->mmp_buf == रिक्त) || (fs->mmp_cmp == रिक्त))
		गच्छ mmp_error;

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_block, रिक्त);
	यदि (retval)
		गच्छ mmp_error;

	/* Check if the MMP block is not changed. */
	mmp = fs->mmp_buf;
	mmp_cmp = fs->mmp_cmp;
	यदि (स_भेद(mmp, mmp_cmp, परिमाण(*mmp_cmp))) {
		retval = EXT2_ET_MMP_CHANGE_ABORT;
		गच्छ mmp_error;
	}

	mmp_cmp->mmp_seq = EXT4_MMP_SEQ_CLEAN;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_block, fs->mmp_cmp);

mmp_error:
	यदि (fs->mmp_fd > 0) {
		बंद(fs->mmp_fd);
		fs->mmp_fd = -1;
	}

	प्रत्यागम retval;
#अन्यथा
	यदि (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		प्रत्यागम 0;

	प्रत्यागम EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

#define EXT2_MIN_MMP_UPDATE_INTERVAL 60

/*
 * Update the on-disk mmp buffer, after checking that it hasn't been changed.
 */
errcode_t ext2fs_mmp_update2(ext2_filsys fs, पूर्णाङ्क immediately)
{
#ifdef CONFIG_MMP
	संरचना mmp_struct *mmp, *mmp_cmp;
	संरचना timeval tv;
	errcode_t retval = 0;

	यदि (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		प्रत्यागम 0;

	gettimeofday(&tv, 0);
	यदि (!immediately &&
	    tv.tv_sec - fs->mmp_last_written < EXT2_MIN_MMP_UPDATE_INTERVAL)
		प्रत्यागम 0;

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_block, रिक्त);
	यदि (retval)
		गच्छ mmp_error;

	mmp = fs->mmp_buf;
	mmp_cmp = fs->mmp_cmp;

	यदि (स_भेद(mmp, mmp_cmp, परिमाण(*mmp_cmp)))
		प्रत्यागम EXT2_ET_MMP_CHANGE_ABORT;

	/*
	 * Believe it or not, ext2fs_mmp_read actually overwrites fs->mmp_cmp
	 * and leaves fs->mmp_buf untouched.  Hence we copy mmp_cmp into
	 * mmp_buf, update mmp_buf, and write mmp_buf out to disk.
	 */
	स_नकल(mmp, mmp_cmp, परिमाण(*mmp_cmp));
	mmp->mmp_time = tv.tv_sec;
	mmp->mmp_seq = EXT4_MMP_SEQ_FSCK;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_block, fs->mmp_buf);

mmp_error:
	प्रत्यागम retval;
#अन्यथा
	यदि (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		प्रत्यागम 0;

	प्रत्यागम EXT2_ET_OP_NOT_SUPPORTED;
#endif
}
#यदि __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic निकालो
#endif
