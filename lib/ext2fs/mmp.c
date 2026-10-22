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

#ଅନ୍ତର୍ଭାଵ "config.h"

#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <sys/time.h>

#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>

#ଅନ୍ତର୍ଭାଵ "ext2fs/ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs/ext2fs.h"

#ifndef O_DIRECT
#define O_DIRECT 0
#endif

#ଯଦି __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic ଠେଲ
#ifndef CONFIG_MMP
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
#endif

errcode_t ext2fs_mmp_get_mem(ext2_filsys fs, ଶୂନ୍ୟ **ptr)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା align = ext2fs_get_dio_alignment(fs->mmp_fd);

	ଫେରନ୍ତୁ ext2fs_get_memalign(fs->blocksize, align, ptr);
}

errcode_t ext2fs_mmp_read(ext2_filsys fs, blk64_t mmp_blk, ଶୂନ୍ୟ *buf)
{
#ifdef CONFIG_MMP
	ସଂରଚନା mmp_struct *mmp_cmp;
	errcode_t retval = 0;

	ଯଦି ((mmp_blk <= fs->super->s_first_data_block) ||
	    (mmp_blk >= ext2fs_blocks_ଗଣନା(fs->super)))
		ଫେରନ୍ତୁ EXT2_ET_MMP_BAD_BLOCK;

	/* ext2fs_open() reserves fd0,1,2 to avoid stdio collision, so checking
	 * mmp_fd <= 0 is OK to validate that the fd is valid.  This opens its
	 * own fd to read the MMP block to ensure that it is using O_DIRECT,
	 * regardless of how the io_manager is doing reads, to avoid caching of
	 * the MMP block by the io_manager or the VM.  It needs to be fresh. */
	ଯଦି (fs->mmp_fd <= 0) {
		ସଂରଚନା stat st;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags = O_RDONLY | O_DIRECT;

		/*
		 * There is no reason for using O_DIRECT if we're working with
		 * regular file. Disabling it also avoids problems with
		 * alignment when the device of the host file system has sector
		 * size larger than blocksize of the fs we're working with.
		 */
		ଯଦି (stat(fs->device_name, &st) == 0 &&
		    S_ISREG(st.st_mode))
			flags &= ~O_DIRECT;

		fs->mmp_fd = open(fs->device_name, flags);
		ଯଦି (fs->mmp_fd < 0) {
			retval = EXT2_ET_MMP_OPEN_DIRECT;
			ଯାଅ out;
		}
	}

	ଯଦି (fs->mmp_cmp == ଖାଲି) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_cmp);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
	}

	ଯଦି ((blk64_t) ext2fs_llseek(fs->mmp_fd, mmp_blk * fs->blocksize,
				    SEEK_SET) !=
	    mmp_blk * fs->blocksize) {
		retval = EXT2_ET_LLSEEK_FAILED;
		ଯାଅ out;
	}

	ଯଦି (ପଠ(fs->mmp_fd, fs->mmp_cmp, fs->blocksize) != fs->blocksize) {
		retval = EXT2_ET_SHORT_READ;
		ଯାଅ out;
	}

	mmp_cmp = fs->mmp_cmp;

	ଯଦି (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_mmp_csum_verify(fs, mmp_cmp))
		retval = EXT2_ET_MMP_CSUM_INVALID;

#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	ext2fs_swap_mmp(mmp_cmp);
#endif

	ଯଦି (buf != ଖାଲି && buf != fs->mmp_cmp)
		memcpy(buf, fs->mmp_cmp, fs->blocksize);

	ଯଦି (mmp_cmp->mmp_magic != EXT4_MMP_MAGIC) {
		retval = EXT2_ET_MMP_MAGIC_INVALID;
		ଯାଅ out;
	}

out:
	ଫେରନ୍ତୁ retval;
#ଅନ୍ୟଥା
	ଫେରନ୍ତୁ EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

errcode_t ext2fs_mmp_write(ext2_filsys fs, blk64_t mmp_blk, ଶୂନ୍ୟ *buf)
{
#ifdef CONFIG_MMP
	ସଂରଚନା mmp_struct *mmp_s = buf;
	ସଂରଚନା timeval tv;
	errcode_t retval = 0;

	gettimeofday(&tv, 0);
	mmp_s->mmp_time = tv.tv_sec;
	fs->mmp_last_written = tv.tv_sec;

	ଯଦି (fs->super->s_mmp_block < fs->super->s_first_data_block ||
	    fs->super->s_mmp_block > ext2fs_blocks_ଗଣନା(fs->super))
		ଫେରନ୍ତୁ EXT2_ET_MMP_BAD_BLOCK;

#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	ext2fs_swap_mmp(mmp_s);
#endif

	retval = ext2fs_mmp_csum_set(fs, mmp_s);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	/* I was tempted to make this use O_DIRECT and the mmp_fd, but
	 * this caused no end of grief, while leaving it as-is works. */
	retval = io_channel_write_blk64(fs->io, mmp_blk, -(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଆକାର(ସଂରଚନା mmp_struct), buf);

#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	ext2fs_swap_mmp(mmp_s);
#endif

	/* Make sure the block gets to disk quickly */
	io_channel_flush(fs->io);
	ଫେରନ୍ତୁ retval;
#ଅନ୍ୟଥା
	ଫେରନ୍ତୁ EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

#ifdef HAVE_SRANDOM
#define srand(x)	srandom(x)
#define rand()		random()
#endif

unsigned ext2fs_mmp_new_seq(ଶୂନ୍ୟ)
{
#ifdef CONFIG_MMP
	unsigned new_seq;
	ସଂରଚନା timeval tv;
	unsigned long pid = getpid();

	gettimeofday(&tv, 0);
	pid = (pid >> 16) | ((pid & 0xFFFF) << 16);
	srand(pid ^ getuid() ^ tv.tv_sec ^ tv.tv_usec);

	gettimeofday(&tv, 0);
	/* Crank the random number generator a few times */
	ପାଇଁ (new_seq = (tv.tv_sec ^ tv.tv_usec) & 0x1F; new_seq > 0; new_seq--)
		rand();

	କର {
		new_seq = rand();
	} ଯେତେବେଳେ (new_seq > EXT4_MMP_SEQ_MAX);

	ଫେରନ୍ତୁ new_seq;
#ଅନ୍ୟଥା
	ଫେରନ୍ତୁ EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

#ifdef CONFIG_MMP
ସ୍ଥିର errcode_t ext2fs_mmp_reset(ext2_filsys fs)
{
	ସଂରଚନା mmp_struct *mmp_s = ଖାଲି;
	errcode_t retval = 0;

	ଯଦି (fs->mmp_buf == ଖାଲି) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		ଯଦି (retval)
			ଯାଅ out;
	}

	memset(fs->mmp_buf, 0, fs->blocksize);
	mmp_s = fs->mmp_buf;

	mmp_s->mmp_magic = EXT4_MMP_MAGIC;
	mmp_s->mmp_seq = EXT4_MMP_SEQ_CLEAN;
	mmp_s->mmp_time = 0;
#ifdef HAVE_GETHOSTNAME
	gethostname((ଅକ୍ଷର *) mmp_s->mmp_nodename, ଆକାର(mmp_s->mmp_nodename));
#ଅନ୍ୟଥା
	mmp_s->mmp_nodename[0] = '\0';
#endif
	strncpy((ଅକ୍ଷର *) mmp_s->mmp_bdevname, fs->device_name,
		ଆକାର(mmp_s->mmp_bdevname));

	mmp_s->mmp_check_interval = fs->super->s_mmp_update_interval;
	ଯଦି (mmp_s->mmp_check_interval < EXT4_MMP_MIN_CHECK_INTERVAL)
		mmp_s->mmp_check_interval = EXT4_MMP_MIN_CHECK_INTERVAL;

	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_block, fs->mmp_buf);
out:
	ଫେରନ୍ତୁ retval;
}
#endif

errcode_t ext2fs_mmp_update(ext2_filsys fs)
{
	ଫେରନ୍ତୁ ext2fs_mmp_update2(fs, 0);
}

errcode_t ext2fs_mmp_clear(ext2_filsys fs)
{
#ifdef CONFIG_MMP
	errcode_t retval = 0;

	ଯଦି (!(fs->flags & EXT2_FLAG_RW))
		ଫେରନ୍ତୁ EXT2_ET_RO_FILSYS;

	retval = ext2fs_mmp_reset(fs);

	ଫେରନ୍ତୁ retval;
#ଅନ୍ୟଥା
	ଫେରନ୍ତୁ EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

errcode_t ext2fs_mmp_init(ext2_filsys fs)
{
#ifdef CONFIG_MMP
	ସଂରଚନା ext2_super_block *sb = fs->super;
	blk64_t mmp_block;
	errcode_t retval;

	ଯଦି (sb->s_mmp_update_interval == 0)
		sb->s_mmp_update_interval = EXT4_MMP_UPDATE_INTERVAL;
	/* This is probably excessively large, but who knows? */
	ଅନ୍ୟଥା ଯଦି (sb->s_mmp_update_interval > EXT4_MMP_MAX_UPDATE_INTERVAL)
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

	ଯଦି (fs->mmp_buf == ଖାଲି) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		ଯଦି (retval)
			ଯାଅ out;
	}

	retval = ext2fs_alloc_block2(fs, 0, fs->mmp_buf, &mmp_block);
	ଯଦି (retval)
		ଯାଅ out;

	sb->s_mmp_block = mmp_block;

	retval = ext2fs_mmp_reset(fs);
	ଯଦି (retval)
		ଯାଅ out;

out:
	ଫେରନ୍ତୁ retval;
#ଅନ୍ୟଥା
	ଫେରନ୍ତୁ EXT2_ET_OP_NOT_SUPPORTED;
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
	ସଂରଚନା mmp_struct *mmp_s;
	unsigned seq;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mmp_check_interval;
	errcode_t retval = 0;

	ଯଦି (fs->mmp_buf == ଖାଲି) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		ଯଦି (retval)
			ଯାଅ mmp_error;
	}

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_block, fs->mmp_buf);
	ଯଦି (retval)
		ଯାଅ mmp_error;

	mmp_s = fs->mmp_buf;

	mmp_check_interval = fs->super->s_mmp_update_interval;
	ଯଦି (mmp_check_interval < EXT4_MMP_MIN_CHECK_INTERVAL)
		mmp_check_interval = EXT4_MMP_MIN_CHECK_INTERVAL;

	seq = mmp_s->mmp_seq;
	ଯଦି (seq == EXT4_MMP_SEQ_CLEAN)
		ଯାଅ clean_seq;
	ଯଦି (seq == EXT4_MMP_SEQ_FSCK) {
		retval = EXT2_ET_MMP_FSCK_ON;
		ଯାଅ mmp_error;
	}

	ଯଦି (seq > EXT4_MMP_SEQ_FSCK) {
		retval = EXT2_ET_MMP_UNKNOWN_SEQ;
		ଯାଅ mmp_error;
	}

	/*
	 * If check_interval in MMP block is larger, use that instead of
	 * check_interval from the superblock.
	 */
	ଯଦି (mmp_s->mmp_check_interval > mmp_check_interval)
		mmp_check_interval = mmp_s->mmp_check_interval;

	sleep(min(mmp_check_interval * 2 + 1, mmp_check_interval + 60));

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_block, fs->mmp_buf);
	ଯଦି (retval)
		ଯାଅ mmp_error;

	ଯଦି (seq != mmp_s->mmp_seq) {
		retval = EXT2_ET_MMP_FAILED;
		ଯାଅ mmp_error;
	}

clean_seq:
	ଯଦି (!(fs->flags & EXT2_FLAG_RW))
		ଯାଅ mmp_error;

	mmp_s->mmp_seq = seq = ext2fs_mmp_new_seq();
#ifdef HAVE_GETHOSTNAME
	gethostname((ଅକ୍ଷର *) mmp_s->mmp_nodename, ଆକାର(mmp_s->mmp_nodename));
#ଅନ୍ୟଥା
	strcpy((ଅକ୍ଷର *) mmp_s->mmp_nodename, "unknown host");
#endif
	strncpy((ଅକ୍ଷର *) mmp_s->mmp_bdevname, fs->device_name,
		ଆକାର(mmp_s->mmp_bdevname));

	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_block, fs->mmp_buf);
	ଯଦି (retval)
		ଯାଅ mmp_error;

	sleep(min(2 * mmp_check_interval + 1, mmp_check_interval + 60));

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_block, fs->mmp_buf);
	ଯଦି (retval)
		ଯାଅ mmp_error;

	ଯଦି (seq != mmp_s->mmp_seq) {
		retval = EXT2_ET_MMP_FAILED;
		ଯାଅ mmp_error;
	}

	mmp_s->mmp_seq = EXT4_MMP_SEQ_FSCK;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_block, fs->mmp_buf);
	ଯଦି (retval)
		ଯାଅ mmp_error;

	ଫେରନ୍ତୁ 0;

mmp_error:
	ଫେରନ୍ତୁ retval;
#ଅନ୍ୟଥା
	ଫେରନ୍ତୁ EXT2_ET_OP_NOT_SUPPORTED;
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
	ସଂରଚନା mmp_struct *mmp, *mmp_cmp;
	errcode_t retval = 0;

	ଯଦି (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP) ||
	    (fs->mmp_buf == ଖାଲି) || (fs->mmp_cmp == ଖାଲି))
		ଯାଅ mmp_error;

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_block, ଖାଲି);
	ଯଦି (retval)
		ଯାଅ mmp_error;

	/* Check if the MMP block is not changed. */
	mmp = fs->mmp_buf;
	mmp_cmp = fs->mmp_cmp;
	ଯଦି (memcmp(mmp, mmp_cmp, ଆକାର(*mmp_cmp))) {
		retval = EXT2_ET_MMP_CHANGE_ABORT;
		ଯାଅ mmp_error;
	}

	mmp_cmp->mmp_seq = EXT4_MMP_SEQ_CLEAN;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_block, fs->mmp_cmp);

mmp_error:
	ଯଦି (fs->mmp_fd > 0) {
		close(fs->mmp_fd);
		fs->mmp_fd = -1;
	}

	ଫେରନ୍ତୁ retval;
#ଅନ୍ୟଥା
	ଯଦି (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ EXT2_ET_OP_NOT_SUPPORTED;
#endif
}

#define EXT2_MIN_MMP_UPDATE_INTERVAL 60

/*
 * Update the on-disk mmp buffer, after checking that it hasn't been changed.
 */
errcode_t ext2fs_mmp_update2(ext2_filsys fs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା immediately)
{
#ifdef CONFIG_MMP
	ସଂରଚନା mmp_struct *mmp, *mmp_cmp;
	ସଂରଚନା timeval tv;
	errcode_t retval = 0;

	ଯଦି (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		ଫେରନ୍ତୁ 0;

	gettimeofday(&tv, 0);
	ଯଦି (!immediately &&
	    tv.tv_sec - fs->mmp_last_written < EXT2_MIN_MMP_UPDATE_INTERVAL)
		ଫେରନ୍ତୁ 0;

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_block, ଖାଲି);
	ଯଦି (retval)
		ଯାଅ mmp_error;

	mmp = fs->mmp_buf;
	mmp_cmp = fs->mmp_cmp;

	ଯଦି (memcmp(mmp, mmp_cmp, ଆକାର(*mmp_cmp)))
		ଫେରନ୍ତୁ EXT2_ET_MMP_CHANGE_ABORT;

	/*
	 * Believe it or not, ext2fs_mmp_read actually overwrites fs->mmp_cmp
	 * and leaves fs->mmp_buf untouched.  Hence we copy mmp_cmp into
	 * mmp_buf, update mmp_buf, and write mmp_buf out to disk.
	 */
	memcpy(mmp, mmp_cmp, ଆକାର(*mmp_cmp));
	mmp->mmp_time = tv.tv_sec;
	mmp->mmp_seq = EXT4_MMP_SEQ_FSCK;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_block, fs->mmp_buf);

mmp_error:
	ଫେରନ୍ତୁ retval;
#ଅନ୍ୟଥା
	ଯଦି (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ EXT2_ET_OP_NOT_SUPPORTED;
#endif
}
#ଯଦି __GNUC_PREREQ (4, 6)
#pragma GCC diagnostic କାଢ଼
#endif
