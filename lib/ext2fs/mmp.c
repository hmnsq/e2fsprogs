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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GNU_ਸਰੋਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _DEFAULT_SOURCE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _DEFAULT_SOURCE	/* since glibc 2.20 _SVID_SOURCE is deprecated */
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"

#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/time.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs/ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs/ext2fs.h"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ O_ਸਿੱਧਾ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ O_ਸਿੱਧਾ 0
#ਜੇ_ਅੰਤ

#ਜੇ __GNUC_PREREQ (4, 6)
#ਹਦਾਇਤ GCC diagnostic ਧੱਕੋ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CONFIG_MMP
#ਹਦਾਇਤ GCC diagnostic ignored "-Wunused-parameter"
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

errcode_t ext2fs_mmp_get_mem(ext2_filsys fs, ਖਾਲੀ **ptr)
{
	ਪੂਰਨ align = ext2fs_get_dio_alignment(fs->mmp_fd);

	ਵਾਪਸ ext2fs_get_memalign(fs->blocksize, align, ptr);
}

errcode_t ext2fs_mmp_read(ext2_filsys fs, blk64_t mmp_blk, ਖਾਲੀ *buf)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_MMP
	ਬਣਤਰ mmp_struct *mmp_cmp;
	errcode_t retval = 0;

	ਜੇ ((mmp_blk <= fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ) ||
	    (mmp_blk >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super)))
		ਵਾਪਸ EXT2_ET_MMP_BAD_ਖੰਡ;

	/* ext2fs_open() reserves fd0,1,2 to avoid stdio collision, so checking
	 * mmp_fd <= 0 is OK to validate that the fd is valid.  This opens its
	 * own fd to read the MMP block to ensure that it is using O_DIRECT,
	 * regardless of how the io_manager is doing reads, to avoid caching of
	 * the MMP block by the io_manager or the VM.  It needs to be fresh. */
	ਜੇ (fs->mmp_fd <= 0) {
		ਬਣਤਰ stat st;
		ਪੂਰਨ flags = O_RDONLY | O_ਸਿੱਧਾ;

		/*
		 * There is no reason for using O_DIRECT if we're working with
		 * regular file. Disabling it also avoids problems with
		 * alignment when the device of the host file system has sector
		 * size larger than blocksize of the fs we're working with.
		 */
		ਜੇ (stat(fs->device_name, &st) == 0 &&
		    S_ISREG(st.st_ਬਹੁਲਕ))
			flags &= ~O_ਸਿੱਧਾ;

		fs->mmp_fd = open(fs->device_name, flags);
		ਜੇ (fs->mmp_fd < 0) {
			retval = EXT2_ET_MMP_OPEN_ਸਿੱਧਾ;
			ਜਾਓ ਬਾਹਰਲਾ;
		}
	}

	ਜੇ (fs->mmp_cmp == ਨਿਰਾ) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_cmp);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}

	ਜੇ ((blk64_t) ext2fs_llseek(fs->mmp_fd, mmp_blk * fs->blocksize,
				    SEEK_SET) !=
	    mmp_blk * fs->blocksize) {
		retval = EXT2_ET_LLSEEK_FAILED;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	ਜੇ (ਪਠ(fs->mmp_fd, fs->mmp_cmp, fs->blocksize) != fs->blocksize) {
		retval = EXT2_ET_SHORT_READ;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	mmp_cmp = fs->mmp_cmp;

	ਜੇ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_mmp_csum_verify(fs, mmp_cmp))
		retval = EXT2_ET_MMP_CSUM_INVALID;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	ext2fs_ਅਦਲਾ_ਬਦਲੀ_mmp(mmp_cmp);
#ਜੇ_ਅੰਤ

	ਜੇ (buf != ਨਿਰਾ && buf != fs->mmp_cmp)
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf, fs->mmp_cmp, fs->blocksize);

	ਜੇ (mmp_cmp->mmp_magic != EXT4_MMP_MAGIC) {
		retval = EXT2_ET_MMP_MAGIC_INVALID;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

ਬਾਹਰਲਾ:
	ਵਾਪਸ retval;
#ਨਹੀਂਤਾਂ
	ਵਾਪਸ EXT2_ET_OP_NOT_SUPPORTED;
#ਜੇ_ਅੰਤ
}

errcode_t ext2fs_mmp_write(ext2_filsys fs, blk64_t mmp_blk, ਖਾਲੀ *buf)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_MMP
	ਬਣਤਰ mmp_struct *mmp_s = buf;
	ਬਣਤਰ timeval tv;
	errcode_t retval = 0;

	gettimeofday(&tv, 0);
	mmp_s->mmp_ਸਮਾਂ = tv.tv_sec;
	fs->mmp_ਆਖਰੀ_written = tv.tv_sec;

	ਜੇ (fs->super->s_mmp_ਖੰਡ < fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ ||
	    fs->super->s_mmp_ਖੰਡ > ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super))
		ਵਾਪਸ EXT2_ET_MMP_BAD_ਖੰਡ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	ext2fs_ਅਦਲਾ_ਬਦਲੀ_mmp(mmp_s);
#ਜੇ_ਅੰਤ

	retval = ext2fs_mmp_csum_ਸੈੱਟ(fs, mmp_s);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	/* I was tempted to make this use O_DIRECT and the mmp_fd, but
	 * this caused no end of grief, while leaving it as-is works. */
	retval = io_channel_write_blk64(fs->io, mmp_blk, -(ਪੂਰਨ)ਆਕਾਰ(ਬਣਤਰ mmp_struct), buf);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	ext2fs_ਅਦਲਾ_ਬਦਲੀ_mmp(mmp_s);
#ਜੇ_ਅੰਤ

	/* Make sure the block gets to disk quickly */
	io_channel_flush(fs->io);
	ਵਾਪਸ retval;
#ਨਹੀਂਤਾਂ
	ਵਾਪਸ EXT2_ET_OP_NOT_SUPPORTED;
#ਜੇ_ਅੰਤ
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SRANDOM
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ srand(x)	srandom(x)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ rand()		random()
#ਜੇ_ਅੰਤ

ਬਿਨਾਨਿਸ਼ਾਨੀ ext2fs_mmp_new_seq(ਖਾਲੀ)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_MMP
	ਬਿਨਾਨਿਸ਼ਾਨੀ new_seq;
	ਬਣਤਰ timeval tv;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ pid = getpid();

	gettimeofday(&tv, 0);
	pid = (pid >> 16) | ((pid & 0xFFFF) << 16);
	srand(pid ^ getuid() ^ tv.tv_sec ^ tv.tv_usec);

	gettimeofday(&tv, 0);
	/* Crank the random number generator a few times */
	ਲਈ (new_seq = (tv.tv_sec ^ tv.tv_usec) & 0x1F; new_seq > 0; new_seq--)
		rand();

	ਕਰੋ {
		new_seq = rand();
	} ਜਦੋਂ (new_seq > EXT4_MMP_SEQ_MAX);

	ਵਾਪਸ new_seq;
#ਨਹੀਂਤਾਂ
	ਵਾਪਸ EXT2_ET_OP_NOT_SUPPORTED;
#ਜੇ_ਅੰਤ
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_MMP
ਸਥਿਰ errcode_t ext2fs_mmp_reset(ext2_filsys fs)
{
	ਬਣਤਰ mmp_struct *mmp_s = ਨਿਰਾ;
	errcode_t retval = 0;

	ਜੇ (fs->mmp_buf == ਨਿਰਾ) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		ਜੇ (retval)
			ਜਾਓ ਬਾਹਰਲਾ;
	}

	memset(fs->mmp_buf, 0, fs->blocksize);
	mmp_s = fs->mmp_buf;

	mmp_s->mmp_magic = EXT4_MMP_MAGIC;
	mmp_s->mmp_seq = EXT4_MMP_SEQ_CLEAN;
	mmp_s->mmp_ਸਮਾਂ = 0;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_GETHOSTNAME
	gethostname((ਅੱਖਰ *) mmp_s->mmp_nodename, ਆਕਾਰ(mmp_s->mmp_nodename));
#ਨਹੀਂਤਾਂ
	mmp_s->mmp_nodename[0] = '\0';
#ਜੇ_ਅੰਤ
	strncpy((ਅੱਖਰ *) mmp_s->mmp_bdevname, fs->device_name,
		ਆਕਾਰ(mmp_s->mmp_bdevname));

	mmp_s->mmp_check_interval = fs->super->s_mmp_update_interval;
	ਜੇ (mmp_s->mmp_check_interval < EXT4_MMP_MIN_CHECK_INTERVAL)
		mmp_s->mmp_check_interval = EXT4_MMP_MIN_CHECK_INTERVAL;

	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_ਖੰਡ, fs->mmp_buf);
ਬਾਹਰਲਾ:
	ਵਾਪਸ retval;
}
#ਜੇ_ਅੰਤ

errcode_t ext2fs_mmp_update(ext2_filsys fs)
{
	ਵਾਪਸ ext2fs_mmp_update2(fs, 0);
}

errcode_t ext2fs_mmp_clear(ext2_filsys fs)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_MMP
	errcode_t retval = 0;

	ਜੇ (!(fs->flags & EXT2_FLAG_RW))
		ਵਾਪਸ EXT2_ET_RO_FILSYS;

	retval = ext2fs_mmp_reset(fs);

	ਵਾਪਸ retval;
#ਨਹੀਂਤਾਂ
	ਵਾਪਸ EXT2_ET_OP_NOT_SUPPORTED;
#ਜੇ_ਅੰਤ
}

errcode_t ext2fs_mmp_init(ext2_filsys fs)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_MMP
	ਬਣਤਰ ext2_super_ਖੰਡ *sb = fs->super;
	blk64_t mmp_ਖੰਡ;
	errcode_t retval;

	ਜੇ (sb->s_mmp_update_interval == 0)
		sb->s_mmp_update_interval = EXT4_MMP_UPDATE_INTERVAL;
	/* This is probably excessively large, but who knows? */
	ਨਹੀਂਤਾਂ ਜੇ (sb->s_mmp_update_interval > EXT4_MMP_MAX_UPDATE_INTERVAL)
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

	ਜੇ (fs->mmp_buf == ਨਿਰਾ) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		ਜੇ (retval)
			ਜਾਓ ਬਾਹਰਲਾ;
	}

	retval = ext2fs_alloc_block2(fs, 0, fs->mmp_buf, &mmp_ਖੰਡ);
	ਜੇ (retval)
		ਜਾਓ ਬਾਹਰਲਾ;

	sb->s_mmp_ਖੰਡ = mmp_ਖੰਡ;

	retval = ext2fs_mmp_reset(fs);
	ਜੇ (retval)
		ਜਾਓ ਬਾਹਰਲਾ;

ਬਾਹਰਲਾ:
	ਵਾਪਸ retval;
#ਨਹੀਂਤਾਂ
	ਵਾਪਸ EXT2_ET_OP_NOT_SUPPORTED;
#ਜੇ_ਅੰਤ
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ min
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ min(x, y) ((x) < (y) ? (x) : (y))
#ਜੇ_ਅੰਤ

/*
 * Make sure that the fs is not mounted or being fsck'ed while opening the fs.
 */
errcode_t ext2fs_mmp_ਸ਼ੁਰੂ(ext2_filsys fs)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_MMP
	ਬਣਤਰ mmp_struct *mmp_s;
	ਬਿਨਾਨਿਸ਼ਾਨੀ seq;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mmp_check_interval;
	errcode_t retval = 0;

	ਜੇ (fs->mmp_buf == ਨਿਰਾ) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		ਜੇ (retval)
			ਜਾਓ mmp_error;
	}

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_ਖੰਡ, fs->mmp_buf);
	ਜੇ (retval)
		ਜਾਓ mmp_error;

	mmp_s = fs->mmp_buf;

	mmp_check_interval = fs->super->s_mmp_update_interval;
	ਜੇ (mmp_check_interval < EXT4_MMP_MIN_CHECK_INTERVAL)
		mmp_check_interval = EXT4_MMP_MIN_CHECK_INTERVAL;

	seq = mmp_s->mmp_seq;
	ਜੇ (seq == EXT4_MMP_SEQ_CLEAN)
		ਜਾਓ clean_seq;
	ਜੇ (seq == EXT4_MMP_SEQ_FSCK) {
		retval = EXT2_ET_MMP_FSCK_ON;
		ਜਾਓ mmp_error;
	}

	ਜੇ (seq > EXT4_MMP_SEQ_FSCK) {
		retval = EXT2_ET_MMP_UNKNOWN_SEQ;
		ਜਾਓ mmp_error;
	}

	/*
	 * If check_interval in MMP block is larger, use that instead of
	 * check_interval from the superblock.
	 */
	ਜੇ (mmp_s->mmp_check_interval > mmp_check_interval)
		mmp_check_interval = mmp_s->mmp_check_interval;

	sleep(min(mmp_check_interval * 2 + 1, mmp_check_interval + 60));

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_ਖੰਡ, fs->mmp_buf);
	ਜੇ (retval)
		ਜਾਓ mmp_error;

	ਜੇ (seq != mmp_s->mmp_seq) {
		retval = EXT2_ET_MMP_FAILED;
		ਜਾਓ mmp_error;
	}

clean_seq:
	ਜੇ (!(fs->flags & EXT2_FLAG_RW))
		ਜਾਓ mmp_error;

	mmp_s->mmp_seq = seq = ext2fs_mmp_new_seq();
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_GETHOSTNAME
	gethostname((ਅੱਖਰ *) mmp_s->mmp_nodename, ਆਕਾਰ(mmp_s->mmp_nodename));
#ਨਹੀਂਤਾਂ
	strcpy((ਅੱਖਰ *) mmp_s->mmp_nodename, "unknown host");
#ਜੇ_ਅੰਤ
	strncpy((ਅੱਖਰ *) mmp_s->mmp_bdevname, fs->device_name,
		ਆਕਾਰ(mmp_s->mmp_bdevname));

	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_ਖੰਡ, fs->mmp_buf);
	ਜੇ (retval)
		ਜਾਓ mmp_error;

	sleep(min(2 * mmp_check_interval + 1, mmp_check_interval + 60));

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_ਖੰਡ, fs->mmp_buf);
	ਜੇ (retval)
		ਜਾਓ mmp_error;

	ਜੇ (seq != mmp_s->mmp_seq) {
		retval = EXT2_ET_MMP_FAILED;
		ਜਾਓ mmp_error;
	}

	mmp_s->mmp_seq = EXT4_MMP_SEQ_FSCK;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_ਖੰਡ, fs->mmp_buf);
	ਜੇ (retval)
		ਜਾਓ mmp_error;

	ਵਾਪਸ 0;

mmp_error:
	ਵਾਪਸ retval;
#ਨਹੀਂਤਾਂ
	ਵਾਪਸ EXT2_ET_OP_NOT_SUPPORTED;
#ਜੇ_ਅੰਤ
}

/*
 * Clear the MMP usage in the filesystem.  If this function returns an
 * error EXT2_ET_MMP_CHANGE_ABORT it means the filesystem was modified
 * by some other process while in use, and changes should be dropped, or
 * risk filesystem corruption.
 */
errcode_t ext2fs_mmp_stop(ext2_filsys fs)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_MMP
	ਬਣਤਰ mmp_struct *mmp, *mmp_cmp;
	errcode_t retval = 0;

	ਜੇ (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP) ||
	    (fs->mmp_buf == ਨਿਰਾ) || (fs->mmp_cmp == ਨਿਰਾ))
		ਜਾਓ mmp_error;

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_ਖੰਡ, ਨਿਰਾ);
	ਜੇ (retval)
		ਜਾਓ mmp_error;

	/* Check if the MMP block is not changed. */
	mmp = fs->mmp_buf;
	mmp_cmp = fs->mmp_cmp;
	ਜੇ (memcmp(mmp, mmp_cmp, ਆਕਾਰ(*mmp_cmp))) {
		retval = EXT2_ET_MMP_CHANGE_ABORT;
		ਜਾਓ mmp_error;
	}

	mmp_cmp->mmp_seq = EXT4_MMP_SEQ_CLEAN;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_ਖੰਡ, fs->mmp_cmp);

mmp_error:
	ਜੇ (fs->mmp_fd > 0) {
		close(fs->mmp_fd);
		fs->mmp_fd = -1;
	}

	ਵਾਪਸ retval;
#ਨਹੀਂਤਾਂ
	ਜੇ (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		ਵਾਪਸ 0;

	ਵਾਪਸ EXT2_ET_OP_NOT_SUPPORTED;
#ਜੇ_ਅੰਤ
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_MIN_MMP_UPDATE_INTERVAL 60

/*
 * Update the on-disk mmp buffer, after checking that it hasn't been changed.
 */
errcode_t ext2fs_mmp_update2(ext2_filsys fs, ਪੂਰਨ immediately)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_MMP
	ਬਣਤਰ mmp_struct *mmp, *mmp_cmp;
	ਬਣਤਰ timeval tv;
	errcode_t retval = 0;

	ਜੇ (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		ਵਾਪਸ 0;

	gettimeofday(&tv, 0);
	ਜੇ (!immediately &&
	    tv.tv_sec - fs->mmp_ਆਖਰੀ_written < EXT2_MIN_MMP_UPDATE_INTERVAL)
		ਵਾਪਸ 0;

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_ਖੰਡ, ਨਿਰਾ);
	ਜੇ (retval)
		ਜਾਓ mmp_error;

	mmp = fs->mmp_buf;
	mmp_cmp = fs->mmp_cmp;

	ਜੇ (memcmp(mmp, mmp_cmp, ਆਕਾਰ(*mmp_cmp)))
		ਵਾਪਸ EXT2_ET_MMP_CHANGE_ABORT;

	/*
	 * Believe it or not, ext2fs_mmp_read actually overwrites fs->mmp_cmp
	 * and leaves fs->mmp_buf untouched.  Hence we copy mmp_cmp into
	 * mmp_buf, update mmp_buf, and write mmp_buf out to disk.
	 */
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(mmp, mmp_cmp, ਆਕਾਰ(*mmp_cmp));
	mmp->mmp_ਸਮਾਂ = tv.tv_sec;
	mmp->mmp_seq = EXT4_MMP_SEQ_FSCK;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_ਖੰਡ, fs->mmp_buf);

mmp_error:
	ਵਾਪਸ retval;
#ਨਹੀਂਤਾਂ
	ਜੇ (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		ਵਾਪਸ 0;

	ਵਾਪਸ EXT2_ET_OP_NOT_SUPPORTED;
#ਜੇ_ਅੰਤ
}
#ਜੇ __GNUC_PREREQ (4, 6)
#ਹਦਾਇਤ GCC diagnostic ਕੱਢੋ
#ਜੇ_ਅੰਤ
