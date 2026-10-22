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

#اگر_غیر_تعریف_شدہ GNU_ماخذ
#تعریف_کرو GNU_ماخذ
#اگر_ختم
#اگر_غیر_تعریف_شدہ _DEFAULT_SOURCE
#تعریف_کرو _DEFAULT_SOURCE	/* since glibc 2.20 _SVID_SOURCE is deprecated */
#اگر_ختم

#شامل "config.h"

#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <sys/time.h>

#شامل <sys/types.h>
#شامل <sys/stat.h>
#شامل <fcntl.h>

#شامل "ext2fs/ext2_fs.h"
#شامل "ext2fs/ext2fs.h"

#اگر_غیر_تعریف_شدہ O_براہ_راست
#تعریف_کرو O_براہ_راست 0
#اگر_ختم

#اگر __GNUC_PREREQ (4, 6)
#ہدایت GCC diagnostic ڈالو
#اگر_غیر_تعریف_شدہ CONFIG_MMP
#ہدایت GCC diagnostic ignored "-Wunused-parameter"
#اگر_ختم
#اگر_ختم

errcode_t ext2fs_mmp_get_mem(ext2_filsys fs, خالی **ptr)
{
	صحیح align = ext2fs_get_dio_alignment(fs->mmp_fd);

	واپس ext2fs_get_memalign(fs->blocksize, align, ptr);
}

errcode_t ext2fs_mmp_read(ext2_filsys fs, blk64_t mmp_blk, خالی *buf)
{
#اگر_تعریف_شدہ CONFIG_MMP
	ساخت mmp_struct *mmp_cmp;
	errcode_t retval = 0;

	اگر ((mmp_blk <= fs->super->s_پہلا_ڈیٹا_بلاک) ||
	    (mmp_blk >= ext2fs_بلاکیں_گنتی(fs->super)))
		واپس EXT2_ET_MMP_BAD_بلاک;

	/* ext2fs_open() reserves fd0,1,2 to avoid stdio collision, so checking
	 * mmp_fd <= 0 is OK to validate that the fd is valid.  This opens its
	 * own fd to read the MMP block to ensure that it is using O_DIRECT,
	 * regardless of how the io_manager is doing reads, to avoid caching of
	 * the MMP block by the io_manager or the VM.  It needs to be fresh. */
	اگر (fs->mmp_fd <= 0) {
		ساخت stat st;
		صحیح flags = O_RDONLY | O_براہ_راست;

		/*
		 * There is no reason for using O_DIRECT if we're working with
		 * regular file. Disabling it also avoids problems with
		 * alignment when the device of the host file system has sector
		 * size larger than blocksize of the fs we're working with.
		 */
		اگر (stat(fs->device_name, &st) == 0 &&
		    S_ISREG(st.st_منوال))
			flags &= ~O_براہ_راست;

		fs->mmp_fd = open(fs->device_name, flags);
		اگر (fs->mmp_fd < 0) {
			retval = EXT2_ET_MMP_OPEN_براہ_راست;
			جاؤ برآمد;
		}
	}

	اگر (fs->mmp_cmp == تہی) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_cmp);
		اگر (retval)
			واپس retval;
	}

	اگر ((blk64_t) ext2fs_llseek(fs->mmp_fd, mmp_blk * fs->blocksize,
				    SEEK_SET) !=
	    mmp_blk * fs->blocksize) {
		retval = EXT2_ET_LLSEEK_FAILED;
		جاؤ برآمد;
	}

	اگر (اقرأ(fs->mmp_fd, fs->mmp_cmp, fs->blocksize) != fs->blocksize) {
		retval = EXT2_ET_SHORT_READ;
		جاؤ برآمد;
	}

	mmp_cmp = fs->mmp_cmp;

	اگر (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_mmp_csum_verify(fs, mmp_cmp))
		retval = EXT2_ET_MMP_CSUM_INVALID;

#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	ext2fs_تبادلہ_mmp(mmp_cmp);
#اگر_ختم

	اگر (buf != تہی && buf != fs->mmp_cmp)
		حافظہ_نقل(buf, fs->mmp_cmp, fs->blocksize);

	اگر (mmp_cmp->mmp_magic != EXT4_MMP_MAGIC) {
		retval = EXT2_ET_MMP_MAGIC_INVALID;
		جاؤ برآمد;
	}

برآمد:
	واپس retval;
#ورنہ
	واپس EXT2_ET_OP_NOT_SUPPORTED;
#اگر_ختم
}

errcode_t ext2fs_mmp_write(ext2_filsys fs, blk64_t mmp_blk, خالی *buf)
{
#اگر_تعریف_شدہ CONFIG_MMP
	ساخت mmp_struct *mmp_s = buf;
	ساخت timeval tv;
	errcode_t retval = 0;

	gettimeofday(&tv, 0);
	mmp_s->mmp_وقت = tv.tv_sec;
	fs->mmp_آخری_written = tv.tv_sec;

	اگر (fs->super->s_mmp_بلاک < fs->super->s_پہلا_ڈیٹا_بلاک ||
	    fs->super->s_mmp_بلاک > ext2fs_بلاکیں_گنتی(fs->super))
		واپس EXT2_ET_MMP_BAD_بلاک;

#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	ext2fs_تبادلہ_mmp(mmp_s);
#اگر_ختم

	retval = ext2fs_mmp_csum_سیٹ(fs, mmp_s);
	اگر (retval)
		واپس retval;

	/* I was tempted to make this use O_DIRECT and the mmp_fd, but
	 * this caused no end of grief, while leaving it as-is works. */
	retval = io_channel_write_blk64(fs->io, mmp_blk, -(صحیح)حجم(ساخت mmp_struct), buf);

#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	ext2fs_تبادلہ_mmp(mmp_s);
#اگر_ختم

	/* Make sure the block gets to disk quickly */
	io_channel_flush(fs->io);
	واپس retval;
#ورنہ
	واپس EXT2_ET_OP_NOT_SUPPORTED;
#اگر_ختم
}

#اگر_تعریف_شدہ HAVE_SRANDOM
#تعریف_کرو srand(x)	srandom(x)
#تعریف_کرو rand()		random()
#اگر_ختم

غیرعلامتی ext2fs_mmp_new_seq(خالی)
{
#اگر_تعریف_شدہ CONFIG_MMP
	غیرعلامتی new_seq;
	ساخت timeval tv;
	غیرعلامتی طویل pid = getpid();

	gettimeofday(&tv, 0);
	pid = (pid >> 16) | ((pid & 0xFFFF) << 16);
	srand(pid ^ getuid() ^ tv.tv_sec ^ tv.tv_usec);

	gettimeofday(&tv, 0);
	/* Crank the random number generator a few times */
	برائے (new_seq = (tv.tv_sec ^ tv.tv_usec) & 0x1F; new_seq > 0; new_seq--)
		rand();

	کرو {
		new_seq = rand();
	} جب (new_seq > EXT4_MMP_SEQ_MAX);

	واپس new_seq;
#ورنہ
	واپس EXT2_ET_OP_NOT_SUPPORTED;
#اگر_ختم
}

#اگر_تعریف_شدہ CONFIG_MMP
ساکن errcode_t ext2fs_mmp_reset(ext2_filsys fs)
{
	ساخت mmp_struct *mmp_s = تہی;
	errcode_t retval = 0;

	اگر (fs->mmp_buf == تہی) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		اگر (retval)
			جاؤ برآمد;
	}

	memset(fs->mmp_buf, 0, fs->blocksize);
	mmp_s = fs->mmp_buf;

	mmp_s->mmp_magic = EXT4_MMP_MAGIC;
	mmp_s->mmp_seq = EXT4_MMP_SEQ_CLEAN;
	mmp_s->mmp_وقت = 0;
#اگر_تعریف_شدہ HAVE_GETHOSTNAME
	gethostname((حرف *) mmp_s->mmp_nodename, حجم(mmp_s->mmp_nodename));
#ورنہ
	mmp_s->mmp_nodename[0] = '\0';
#اگر_ختم
	strncpy((حرف *) mmp_s->mmp_bdevname, fs->device_name,
		حجم(mmp_s->mmp_bdevname));

	mmp_s->mmp_check_interval = fs->super->s_mmp_update_interval;
	اگر (mmp_s->mmp_check_interval < EXT4_MMP_MIN_CHECK_INTERVAL)
		mmp_s->mmp_check_interval = EXT4_MMP_MIN_CHECK_INTERVAL;

	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_بلاک, fs->mmp_buf);
برآمد:
	واپس retval;
}
#اگر_ختم

errcode_t ext2fs_mmp_update(ext2_filsys fs)
{
	واپس ext2fs_mmp_update2(fs, 0);
}

errcode_t ext2fs_mmp_clear(ext2_filsys fs)
{
#اگر_تعریف_شدہ CONFIG_MMP
	errcode_t retval = 0;

	اگر (!(fs->flags & EXT2_FLAG_RW))
		واپس EXT2_ET_RO_FILSYS;

	retval = ext2fs_mmp_reset(fs);

	واپس retval;
#ورنہ
	واپس EXT2_ET_OP_NOT_SUPPORTED;
#اگر_ختم
}

errcode_t ext2fs_mmp_init(ext2_filsys fs)
{
#اگر_تعریف_شدہ CONFIG_MMP
	ساخت ext2_super_بلاک *sb = fs->super;
	blk64_t mmp_بلاک;
	errcode_t retval;

	اگر (sb->s_mmp_update_interval == 0)
		sb->s_mmp_update_interval = EXT4_MMP_UPDATE_INTERVAL;
	/* This is probably excessively large, but who knows? */
	ورنہ اگر (sb->s_mmp_update_interval > EXT4_MMP_MAX_UPDATE_INTERVAL)
		واپس EXT2_ET_INVALID_دلیل;

	اگر (fs->mmp_buf == تہی) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		اگر (retval)
			جاؤ برآمد;
	}

	retval = ext2fs_alloc_block2(fs, 0, fs->mmp_buf, &mmp_بلاک);
	اگر (retval)
		جاؤ برآمد;

	sb->s_mmp_بلاک = mmp_بلاک;

	retval = ext2fs_mmp_reset(fs);
	اگر (retval)
		جاؤ برآمد;

برآمد:
	واپس retval;
#ورنہ
	واپس EXT2_ET_OP_NOT_SUPPORTED;
#اگر_ختم
}

#اگر_غیر_تعریف_شدہ min
#تعریف_کرو min(x, y) ((x) < (y) ? (x) : (y))
#اگر_ختم

/*
 * Make sure that the fs is not mounted or being fsck'ed while opening the fs.
 */
errcode_t ext2fs_mmp_آغاز(ext2_filsys fs)
{
#اگر_تعریف_شدہ CONFIG_MMP
	ساخت mmp_struct *mmp_s;
	غیرعلامتی seq;
	غیرعلامتی صحیح mmp_check_interval;
	errcode_t retval = 0;

	اگر (fs->mmp_buf == تہی) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		اگر (retval)
			جاؤ mmp_error;
	}

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_بلاک, fs->mmp_buf);
	اگر (retval)
		جاؤ mmp_error;

	mmp_s = fs->mmp_buf;

	mmp_check_interval = fs->super->s_mmp_update_interval;
	اگر (mmp_check_interval < EXT4_MMP_MIN_CHECK_INTERVAL)
		mmp_check_interval = EXT4_MMP_MIN_CHECK_INTERVAL;

	seq = mmp_s->mmp_seq;
	اگر (seq == EXT4_MMP_SEQ_CLEAN)
		جاؤ clean_seq;
	اگر (seq == EXT4_MMP_SEQ_FSCK) {
		retval = EXT2_ET_MMP_FSCK_ON;
		جاؤ mmp_error;
	}

	اگر (seq > EXT4_MMP_SEQ_FSCK) {
		retval = EXT2_ET_MMP_UNKNOWN_SEQ;
		جاؤ mmp_error;
	}

	/*
	 * If check_interval in MMP block is larger, use that instead of
	 * check_interval from the superblock.
	 */
	اگر (mmp_s->mmp_check_interval > mmp_check_interval)
		mmp_check_interval = mmp_s->mmp_check_interval;

	sleep(min(mmp_check_interval * 2 + 1, mmp_check_interval + 60));

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_بلاک, fs->mmp_buf);
	اگر (retval)
		جاؤ mmp_error;

	اگر (seq != mmp_s->mmp_seq) {
		retval = EXT2_ET_MMP_FAILED;
		جاؤ mmp_error;
	}

clean_seq:
	اگر (!(fs->flags & EXT2_FLAG_RW))
		جاؤ mmp_error;

	mmp_s->mmp_seq = seq = ext2fs_mmp_new_seq();
#اگر_تعریف_شدہ HAVE_GETHOSTNAME
	gethostname((حرف *) mmp_s->mmp_nodename, حجم(mmp_s->mmp_nodename));
#ورنہ
	strcpy((حرف *) mmp_s->mmp_nodename, "unknown host");
#اگر_ختم
	strncpy((حرف *) mmp_s->mmp_bdevname, fs->device_name,
		حجم(mmp_s->mmp_bdevname));

	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_بلاک, fs->mmp_buf);
	اگر (retval)
		جاؤ mmp_error;

	sleep(min(2 * mmp_check_interval + 1, mmp_check_interval + 60));

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_بلاک, fs->mmp_buf);
	اگر (retval)
		جاؤ mmp_error;

	اگر (seq != mmp_s->mmp_seq) {
		retval = EXT2_ET_MMP_FAILED;
		جاؤ mmp_error;
	}

	mmp_s->mmp_seq = EXT4_MMP_SEQ_FSCK;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_بلاک, fs->mmp_buf);
	اگر (retval)
		جاؤ mmp_error;

	واپس 0;

mmp_error:
	واپس retval;
#ورنہ
	واپس EXT2_ET_OP_NOT_SUPPORTED;
#اگر_ختم
}

/*
 * Clear the MMP usage in the filesystem.  If this function returns an
 * error EXT2_ET_MMP_CHANGE_ABORT it means the filesystem was modified
 * by some other process while in use, and changes should be dropped, or
 * risk filesystem corruption.
 */
errcode_t ext2fs_mmp_stop(ext2_filsys fs)
{
#اگر_تعریف_شدہ CONFIG_MMP
	ساخت mmp_struct *mmp, *mmp_cmp;
	errcode_t retval = 0;

	اگر (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP) ||
	    (fs->mmp_buf == تہی) || (fs->mmp_cmp == تہی))
		جاؤ mmp_error;

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_بلاک, تہی);
	اگر (retval)
		جاؤ mmp_error;

	/* Check if the MMP block is not changed. */
	mmp = fs->mmp_buf;
	mmp_cmp = fs->mmp_cmp;
	اگر (memcmp(mmp, mmp_cmp, حجم(*mmp_cmp))) {
		retval = EXT2_ET_MMP_CHANGE_ABORT;
		جاؤ mmp_error;
	}

	mmp_cmp->mmp_seq = EXT4_MMP_SEQ_CLEAN;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_بلاک, fs->mmp_cmp);

mmp_error:
	اگر (fs->mmp_fd > 0) {
		close(fs->mmp_fd);
		fs->mmp_fd = -1;
	}

	واپس retval;
#ورنہ
	اگر (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		واپس 0;

	واپس EXT2_ET_OP_NOT_SUPPORTED;
#اگر_ختم
}

#تعریف_کرو EXT2_MIN_MMP_UPDATE_INTERVAL 60

/*
 * Update the on-disk mmp buffer, after checking that it hasn't been changed.
 */
errcode_t ext2fs_mmp_update2(ext2_filsys fs, صحیح immediately)
{
#اگر_تعریف_شدہ CONFIG_MMP
	ساخت mmp_struct *mmp, *mmp_cmp;
	ساخت timeval tv;
	errcode_t retval = 0;

	اگر (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		واپس 0;

	gettimeofday(&tv, 0);
	اگر (!immediately &&
	    tv.tv_sec - fs->mmp_آخری_written < EXT2_MIN_MMP_UPDATE_INTERVAL)
		واپس 0;

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_بلاک, تہی);
	اگر (retval)
		جاؤ mmp_error;

	mmp = fs->mmp_buf;
	mmp_cmp = fs->mmp_cmp;

	اگر (memcmp(mmp, mmp_cmp, حجم(*mmp_cmp)))
		واپس EXT2_ET_MMP_CHANGE_ABORT;

	/*
	 * Believe it or not, ext2fs_mmp_read actually overwrites fs->mmp_cmp
	 * and leaves fs->mmp_buf untouched.  Hence we copy mmp_cmp into
	 * mmp_buf, update mmp_buf, and write mmp_buf out to disk.
	 */
	حافظہ_نقل(mmp, mmp_cmp, حجم(*mmp_cmp));
	mmp->mmp_وقت = tv.tv_sec;
	mmp->mmp_seq = EXT4_MMP_SEQ_FSCK;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_بلاک, fs->mmp_buf);

mmp_error:
	واپس retval;
#ورنہ
	اگر (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		واپس 0;

	واپس EXT2_ET_OP_NOT_SUPPORTED;
#اگر_ختم
}
#اگر __GNUC_PREREQ (4, 6)
#ہدایت GCC diagnostic نکالو
#اگر_ختم
