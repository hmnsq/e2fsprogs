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

#include "config.h"

#ᯀᯮᯝ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <sys/time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "ext2fs/ext2_fs.h"
#include "ext2fs/ext2fs.h"

#ifndef O_DIRECT
#define O_DIRECT 0
#endif

#ᯀᯮᯝ __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic push
#ifndef CONFIG_MMP
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
#endif

errcode_t ext2fs_mmp_get_mem(ext2_filsys fs, void **ptr)
{
	puur_naa_nka align = ext2fs_get_dio_alignment(fs->mmp_fd);

	ᯀᯮᯞ ext2fs_get_memalign(fs->blocksize, align, ptr);
}

errcode_t ext2fs_mmp_read(ext2_filsys fs, blk64_t mmp_blk, void *buf)
{
#ifdef CONFIG_MMP
	sa_nracanaa mmp_struct *mmp_cmp;
	errcode_t retval = 0;

	ᯀᯮᯝ ((mmp_blk <= fs->super->s_first_data_block) ||
	    (mmp_blk >= ext2fs_blocks_count(fs->super)))
		ᯀᯮᯞ EXT2_ET_MMP_BAD_BLOCK;

	/* ext2fs_open() reserves fd0,1,2 to avoid stdio collision, so checking
	 * mmp_fd <= 0 is OK to validate that the fd is valid.  This opens its
	 * own fd to read the MMP block to ensure that it is using O_DIRECT,
	 * regardless of how the io_manager is doing reads, to avoid caching of
	 * the MMP block by the io_manager or the VM.  It needs to be fresh. */
	ᯀᯮᯝ (fs->mmp_fd <= 0) {
		sa_nracanaa stat st;
		puur_naa_nka flags = O_RDONLY | O_DIRECT;

		/*
		 * There is no reason for using O_DIRECT if we're working with
		 * regular file. Disabling it also avoids problems with
		 * alignment when the device of the host file system has sector
		 * size larger than blocksize of the fs we're working with.
		 */
		ᯀᯮᯝ (stat(fs->device_name, &st) == 0 &&
		    S_ISREG(st.st_mode))
			flags &= ~O_DIRECT;

		fs->mmp_fd = open(fs->device_name, flags);
		ᯀᯮᯝ (fs->mmp_fd < 0) {
			retval = EXT2_ET_MMP_OPEN_DIRECT;
			goto out;
		}
	}

	ᯀᯮᯝ (fs->mmp_cmp == NULL) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_cmp);
		ᯀᯮᯝ (retval)
			ᯀᯮᯞ retval;
	}

	ᯀᯮᯝ ((blk64_t) ext2fs_llseek(fs->mmp_fd, mmp_blk * fs->blocksize,
				    SEEK_SET) !=
	    mmp_blk * fs->blocksize) {
		retval = EXT2_ET_LLSEEK_FAILED;
		goto out;
	}

	ᯀᯮᯝ (read(fs->mmp_fd, fs->mmp_cmp, fs->blocksize) != fs->blocksize) {
		retval = EXT2_ET_SHORT_READ;
		goto out;
	}

	mmp_cmp = fs->mmp_cmp;

	ᯀᯮᯝ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_mmp_csum_verify(fs, mmp_cmp))
		retval = EXT2_ET_MMP_CSUM_INVALID;

#ifdef WORDS_BIGENDIAN
	ext2fs_swap_mmp(mmp_cmp);
#endif

	ᯀᯮᯝ (buf != NULL && buf != fs->mmp_cmp)
		memcpy(buf, fs->mmp_cmp, fs->blocksize);

	ᯀᯮᯝ (mmp_cmp->mmp_magic != EXT4_MMP_MAGIC) {
		retval = EXT2_ET_MMP_MAGIC_INVALID;
		goto out;
	}

out:
	ᯀᯮᯞ retval;
#ᯀᯮᯝᯂᯪ
	ᯀᯮᯞ EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

errcode_t ext2fs_mmp_write(ext2_filsys fs, blk64_t mmp_blk, void *buf)
{
#ifdef CONFIG_MMP
	sa_nracanaa mmp_struct *mmp_s = buf;
	sa_nracanaa timeval tv;
	errcode_t retval = 0;

	gettimeofday(&tv, 0);
	mmp_s->mmp_time = tv.tv_sec;
	fs->mmp_last_written = tv.tv_sec;

	ᯀᯮᯝ (fs->super->s_mmp_block < fs->super->s_first_data_block ||
	    fs->super->s_mmp_block > ext2fs_blocks_count(fs->super))
		ᯀᯮᯞ EXT2_ET_MMP_BAD_BLOCK;

#ifdef WORDS_BIGENDIAN
	ext2fs_swap_mmp(mmp_s);
#endif

	retval = ext2fs_mmp_csum_set(fs, mmp_s);
	ᯀᯮᯝ (retval)
		ᯀᯮᯞ retval;

	/* I was tempted to make this use O_DIRECT and the mmp_fd, but
	 * this caused no end of grief, while leaving it as-is works. */
	retval = io_channel_write_blk64(fs->io, mmp_blk, -(puur_naa_nka)parimaa_na(sa_nracanaa mmp_struct), buf);

#ifdef WORDS_BIGENDIAN
	ext2fs_swap_mmp(mmp_s);
#endif

	/* Make sure the block gets to disk quickly */
	io_channel_flush(fs->io);
	ᯀᯮᯞ retval;
#ᯀᯮᯝᯂᯪ
	ᯀᯮᯞ EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

#ifdef HAVE_SRANDOM
#define srand(x)	srandom(x)
#define rand()		random()
#endif

unsigned ext2fs_mmp_new_seq(void)
{
#ifdef CONFIG_MMP
	unsigned new_seq;
	sa_nracanaa timeval tv;
	unsigned long pid = getpid();

	gettimeofday(&tv, 0);
	pid = (pid >> 16) | ((pid & 0xFFFF) << 16);
	srand(pid ^ getuid() ^ tv.tv_sec ^ tv.tv_usec);

	gettimeofday(&tv, 0);
	/* Crank the random number generator a few times */
	for (new_seq = (tv.tv_sec ^ tv.tv_usec) & 0x1F; new_seq > 0; new_seq--)
		rand();

	do {
		new_seq = rand();
	} ᯀᯬᯝ (new_seq > EXT4_MMP_SEQ_MAX);

	ᯀᯮᯞ new_seq;
#ᯀᯮᯝᯂᯪ
	ᯀᯮᯞ EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

#ifdef CONFIG_MMP
static errcode_t ext2fs_mmp_reset(ext2_filsys fs)
{
	sa_nracanaa mmp_struct *mmp_s = NULL;
	errcode_t retval = 0;

	ᯀᯮᯝ (fs->mmp_buf == NULL) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		ᯀᯮᯝ (retval)
			goto out;
	}

	memset(fs->mmp_buf, 0, fs->blocksize);
	mmp_s = fs->mmp_buf;

	mmp_s->mmp_magic = EXT4_MMP_MAGIC;
	mmp_s->mmp_seq = EXT4_MMP_SEQ_CLEAN;
	mmp_s->mmp_time = 0;
#ifdef HAVE_GETHOSTNAME
	gethostname((var_na *) mmp_s->mmp_nodename, parimaa_na(mmp_s->mmp_nodename));
#ᯀᯮᯝᯂᯪ
	mmp_s->mmp_nodename[0] = '\0';
#endif
	strncpy((var_na *) mmp_s->mmp_bdevname, fs->device_name,
		parimaa_na(mmp_s->mmp_bdevname));

	mmp_s->mmp_check_interval = fs->super->s_mmp_update_interval;
	ᯀᯮᯝ (mmp_s->mmp_check_interval < EXT4_MMP_MIN_CHECK_INTERVAL)
		mmp_s->mmp_check_interval = EXT4_MMP_MIN_CHECK_INTERVAL;

	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_block, fs->mmp_buf);
out:
	ᯀᯮᯞ retval;
}
#endif

errcode_t ext2fs_mmp_update(ext2_filsys fs)
{
	ᯀᯮᯞ ext2fs_mmp_update2(fs, 0);
}

errcode_t ext2fs_mmp_clear(ext2_filsys fs)
{
#ifdef CONFIG_MMP
	errcode_t retval = 0;

	ᯀᯮᯝ (!(fs->flags & EXT2_FLAG_RW))
		ᯀᯮᯞ EXT2_ET_RO_FILSYS;

	retval = ext2fs_mmp_reset(fs);

	ᯀᯮᯞ retval;
#ᯀᯮᯝᯂᯪ
	ᯀᯮᯞ EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

errcode_t ext2fs_mmp_init(ext2_filsys fs)
{
#ifdef CONFIG_MMP
	sa_nracanaa ext2_super_block *sb = fs->super;
	blk64_t mmp_block;
	errcode_t retval;

	ᯀᯮᯝ (sb->s_mmp_update_interval == 0)
		sb->s_mmp_update_interval = EXT4_MMP_UPDATE_INTERVAL;
	/* This is probably excessively large, but who knows? */
	ᯀᯮᯝᯂᯪ ᯀᯮᯝ (sb->s_mmp_update_interval > EXT4_MMP_MAX_UPDATE_INTERVAL)
		ᯀᯮᯞ EXT2_ET_INVALID_ARGUMENT;

	ᯀᯮᯝ (fs->mmp_buf == NULL) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		ᯀᯮᯝ (retval)
			goto out;
	}

	retval = ext2fs_alloc_block2(fs, 0, fs->mmp_buf, &mmp_block);
	ᯀᯮᯝ (retval)
		goto out;

	sb->s_mmp_block = mmp_block;

	retval = ext2fs_mmp_reset(fs);
	ᯀᯮᯝ (retval)
		goto out;

out:
	ᯀᯮᯞ retval;
#ᯀᯮᯝᯂᯪ
	ᯀᯮᯞ EXT2_ET_OP_NOT_SUPPORTED;
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
	sa_nracanaa mmp_struct *mmp_s;
	unsigned seq;
	unsigned puur_naa_nka mmp_check_interval;
	errcode_t retval = 0;

	ᯀᯮᯝ (fs->mmp_buf == NULL) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		ᯀᯮᯝ (retval)
			goto mmp_error;
	}

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_block, fs->mmp_buf);
	ᯀᯮᯝ (retval)
		goto mmp_error;

	mmp_s = fs->mmp_buf;

	mmp_check_interval = fs->super->s_mmp_update_interval;
	ᯀᯮᯝ (mmp_check_interval < EXT4_MMP_MIN_CHECK_INTERVAL)
		mmp_check_interval = EXT4_MMP_MIN_CHECK_INTERVAL;

	seq = mmp_s->mmp_seq;
	ᯀᯮᯝ (seq == EXT4_MMP_SEQ_CLEAN)
		goto clean_seq;
	ᯀᯮᯝ (seq == EXT4_MMP_SEQ_FSCK) {
		retval = EXT2_ET_MMP_FSCK_ON;
		goto mmp_error;
	}

	ᯀᯮᯝ (seq > EXT4_MMP_SEQ_FSCK) {
		retval = EXT2_ET_MMP_UNKNOWN_SEQ;
		goto mmp_error;
	}

	/*
	 * If check_interval in MMP block is larger, use that instead of
	 * check_interval from the superblock.
	 */
	ᯀᯮᯝ (mmp_s->mmp_check_interval > mmp_check_interval)
		mmp_check_interval = mmp_s->mmp_check_interval;

	sleep(min(mmp_check_interval * 2 + 1, mmp_check_interval + 60));

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_block, fs->mmp_buf);
	ᯀᯮᯝ (retval)
		goto mmp_error;

	ᯀᯮᯝ (seq != mmp_s->mmp_seq) {
		retval = EXT2_ET_MMP_FAILED;
		goto mmp_error;
	}

clean_seq:
	ᯀᯮᯝ (!(fs->flags & EXT2_FLAG_RW))
		goto mmp_error;

	mmp_s->mmp_seq = seq = ext2fs_mmp_new_seq();
#ifdef HAVE_GETHOSTNAME
	gethostname((var_na *) mmp_s->mmp_nodename, parimaa_na(mmp_s->mmp_nodename));
#ᯀᯮᯝᯂᯪ
	strcpy((var_na *) mmp_s->mmp_nodename, "unknown host");
#endif
	strncpy((var_na *) mmp_s->mmp_bdevname, fs->device_name,
		parimaa_na(mmp_s->mmp_bdevname));

	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_block, fs->mmp_buf);
	ᯀᯮᯝ (retval)
		goto mmp_error;

	sleep(min(2 * mmp_check_interval + 1, mmp_check_interval + 60));

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_block, fs->mmp_buf);
	ᯀᯮᯝ (retval)
		goto mmp_error;

	ᯀᯮᯝ (seq != mmp_s->mmp_seq) {
		retval = EXT2_ET_MMP_FAILED;
		goto mmp_error;
	}

	mmp_s->mmp_seq = EXT4_MMP_SEQ_FSCK;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_block, fs->mmp_buf);
	ᯀᯮᯝ (retval)
		goto mmp_error;

	ᯀᯮᯞ 0;

mmp_error:
	ᯀᯮᯞ retval;
#ᯀᯮᯝᯂᯪ
	ᯀᯮᯞ EXT2_ET_OP_NOT_SUPPORTED;
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
	sa_nracanaa mmp_struct *mmp, *mmp_cmp;
	errcode_t retval = 0;

	ᯀᯮᯝ (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP) ||
	    (fs->mmp_buf == NULL) || (fs->mmp_cmp == NULL))
		goto mmp_error;

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_block, NULL);
	ᯀᯮᯝ (retval)
		goto mmp_error;

	/* Check if the MMP block is not changed. */
	mmp = fs->mmp_buf;
	mmp_cmp = fs->mmp_cmp;
	ᯀᯮᯝ (memcmp(mmp, mmp_cmp, parimaa_na(*mmp_cmp))) {
		retval = EXT2_ET_MMP_CHANGE_ABORT;
		goto mmp_error;
	}

	mmp_cmp->mmp_seq = EXT4_MMP_SEQ_CLEAN;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_block, fs->mmp_cmp);

mmp_error:
	ᯀᯮᯝ (fs->mmp_fd > 0) {
		close(fs->mmp_fd);
		fs->mmp_fd = -1;
	}

	ᯀᯮᯞ retval;
#ᯀᯮᯝᯂᯪ
	ᯀᯮᯝ (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		ᯀᯮᯞ 0;

	ᯀᯮᯞ EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

#define EXT2_MIN_MMP_UPDATE_INTERVAL 60

/*
 * Update the on-disk mmp buffer, after checking that it hasn't been changed.
 */
errcode_t ext2fs_mmp_update2(ext2_filsys fs, puur_naa_nka immediately)
{
#ifdef CONFIG_MMP
	sa_nracanaa mmp_struct *mmp, *mmp_cmp;
	sa_nracanaa timeval tv;
	errcode_t retval = 0;

	ᯀᯮᯝ (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		ᯀᯮᯞ 0;

	gettimeofday(&tv, 0);
	ᯀᯮᯝ (!immediately &&
	    tv.tv_sec - fs->mmp_last_written < EXT2_MIN_MMP_UPDATE_INTERVAL)
		ᯀᯮᯞ 0;

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_block, NULL);
	ᯀᯮᯝ (retval)
		goto mmp_error;

	mmp = fs->mmp_buf;
	mmp_cmp = fs->mmp_cmp;

	ᯀᯮᯝ (memcmp(mmp, mmp_cmp, parimaa_na(*mmp_cmp)))
		ᯀᯮᯞ EXT2_ET_MMP_CHANGE_ABORT;

	/*
	 * Believe it or not, ext2fs_mmp_read actually overwrites fs->mmp_cmp
	 * and leaves fs->mmp_buf untouched.  Hence we copy mmp_cmp into
	 * mmp_buf, update mmp_buf, and write mmp_buf out to disk.
	 */
	memcpy(mmp, mmp_cmp, parimaa_na(*mmp_cmp));
	mmp->mmp_time = tv.tv_sec;
	mmp->mmp_seq = EXT4_MMP_SEQ_FSCK;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_block, fs->mmp_buf);

mmp_error:
	ᯀᯮᯞ retval;
#ᯀᯮᯝᯂᯪ
	ᯀᯮᯝ (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		ᯀᯮᯞ 0;

	ᯀᯮᯞ EXT2_ET_OP_NOT_SUPPORTED;
#endif
}
#ᯀᯮᯝ __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic pop
#endif
