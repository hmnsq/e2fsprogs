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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GNU_ಮೂಲ
#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _DEFAULT_SOURCE
#ವ್ಯಾಖ್ಯಾನಿಸು _DEFAULT_SOURCE	/* since glibc 2.20 _SVID_SOURCE is deprecated */
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "config.h"

#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <sys/time.h>

#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>

#ಅನ್ತರ್ಭಾವ "ext2fs/ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs/ext2fs.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ O_ನೇರ
#ವ್ಯಾಖ್ಯಾನಿಸು O_ನೇರ 0
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ __GNUC_PREREQ (4, 6)
#ಸೂಚನೆ GCC diagnostic ತಳ್ಳು
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CONFIG_MMP
#ಸೂಚನೆ GCC diagnostic ignored "-Wunused-parameter"
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

errcode_t ext2fs_mmp_get_mem(ext2_filsys fs, ಶೂನ್ಯ **ptr)
{
	ಪೂರ್ಣಾಂಕ align = ext2fs_get_dio_alignment(fs->mmp_fd);

	ಹಿಂತಿರುಗಿ ext2fs_get_memalign(fs->blocksize, align, ptr);
}

errcode_t ext2fs_mmp_read(ext2_filsys fs, blk64_t mmp_blk, ಶೂನ್ಯ *buf)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_MMP
	ರಚನೆ mmp_struct *mmp_cmp;
	errcode_t retval = 0;

	ಆದರೆ ((mmp_blk <= fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ) ||
	    (mmp_blk >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super)))
		ಹಿಂತಿರುಗಿ EXT2_ET_MMP_BAD_ಖಂಡ;

	/* ext2fs_open() reserves fd0,1,2 to avoid stdio collision, so checking
	 * mmp_fd <= 0 is OK to validate that the fd is valid.  This opens its
	 * own fd to read the MMP block to ensure that it is using O_DIRECT,
	 * regardless of how the io_manager is doing reads, to avoid caching of
	 * the MMP block by the io_manager or the VM.  It needs to be fresh. */
	ಆದರೆ (fs->mmp_fd <= 0) {
		ರಚನೆ stat st;
		ಪೂರ್ಣಾಂಕ flags = O_RDONLY | O_ನೇರ;

		/*
		 * There is no reason for using O_DIRECT if we're working with
		 * regular file. Disabling it also avoids problems with
		 * alignment when the device of the host file system has sector
		 * size larger than blocksize of the fs we're working with.
		 */
		ಆದರೆ (stat(fs->device_name, &st) == 0 &&
		    S_ISREG(st.st_ಬಹುಲಕ))
			flags &= ~O_ನೇರ;

		fs->mmp_fd = open(fs->device_name, flags);
		ಆದರೆ (fs->mmp_fd < 0) {
			retval = EXT2_ET_MMP_OPEN_ನೇರ;
			ಹೋಗು ಹೊರಹೋಗು;
		}
	}

	ಆದರೆ (fs->mmp_cmp == ಖಾಲಿ) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_cmp);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}

	ಆದರೆ ((blk64_t) ext2fs_llseek(fs->mmp_fd, mmp_blk * fs->blocksize,
				    SEEK_SET) !=
	    mmp_blk * fs->blocksize) {
		retval = EXT2_ET_LLSEEK_FAILED;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	ಆದರೆ (ಪಠ(fs->mmp_fd, fs->mmp_cmp, fs->blocksize) != fs->blocksize) {
		retval = EXT2_ET_SHORT_READ;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	mmp_cmp = fs->mmp_cmp;

	ಆದರೆ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_mmp_csum_verify(fs, mmp_cmp))
		retval = EXT2_ET_MMP_CSUM_INVALID;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	ext2fs_ಅದಲುಬದಲು_mmp(mmp_cmp);
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (buf != ಖಾಲಿ && buf != fs->mmp_cmp)
		ಸ್ಮರಣೆ_ನಕಲು(buf, fs->mmp_cmp, fs->blocksize);

	ಆದರೆ (mmp_cmp->mmp_magic != EXT4_MMP_MAGIC) {
		retval = EXT2_ET_MMP_MAGIC_INVALID;
		ಹೋಗು ಹೊರಹೋಗು;
	}

ಹೊರಹೋಗು:
	ಹಿಂತಿರುಗಿ retval;
#ಇಲ್ಲದಿದ್ದರೆ
	ಹಿಂತಿರುಗಿ EXT2_ET_OP_NOT_SUPPORTED;
#ಆದರೆ_ಅಂತ್ಯ
}

errcode_t ext2fs_mmp_write(ext2_filsys fs, blk64_t mmp_blk, ಶೂನ್ಯ *buf)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_MMP
	ರಚನೆ mmp_struct *mmp_s = buf;
	ರಚನೆ timeval tv;
	errcode_t retval = 0;

	gettimeofday(&tv, 0);
	mmp_s->mmp_ಸಮಯ = tv.tv_sec;
	fs->mmp_ಕೊನೆಯ_written = tv.tv_sec;

	ಆದರೆ (fs->super->s_mmp_ಖಂಡ < fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ ||
	    fs->super->s_mmp_ಖಂಡ > ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super))
		ಹಿಂತಿರುಗಿ EXT2_ET_MMP_BAD_ಖಂಡ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	ext2fs_ಅದಲುಬದಲು_mmp(mmp_s);
#ಆದರೆ_ಅಂತ್ಯ

	retval = ext2fs_mmp_csum_ಗಣ(fs, mmp_s);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	/* I was tempted to make this use O_DIRECT and the mmp_fd, but
	 * this caused no end of grief, while leaving it as-is works. */
	retval = io_channel_write_blk64(fs->io, mmp_blk, -(ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(ರಚನೆ mmp_struct), buf);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	ext2fs_ಅದಲುಬದಲು_mmp(mmp_s);
#ಆದರೆ_ಅಂತ್ಯ

	/* Make sure the block gets to disk quickly */
	io_channel_flush(fs->io);
	ಹಿಂತಿರುಗಿ retval;
#ಇಲ್ಲದಿದ್ದರೆ
	ಹಿಂತಿರುಗಿ EXT2_ET_OP_NOT_SUPPORTED;
#ಆದರೆ_ಅಂತ್ಯ
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SRANDOM
#ವ್ಯಾಖ್ಯಾನಿಸು srand(x)	srandom(x)
#ವ್ಯಾಖ್ಯಾನಿಸು rand()		random()
#ಆದರೆ_ಅಂತ್ಯ

ಚಿಹ್ನೆರಹಿತ ext2fs_mmp_new_seq(ಶೂನ್ಯ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_MMP
	ಚಿಹ್ನೆರಹಿತ new_seq;
	ರಚನೆ timeval tv;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ pid = getpid();

	gettimeofday(&tv, 0);
	pid = (pid >> 16) | ((pid & 0xFFFF) << 16);
	srand(pid ^ getuid() ^ tv.tv_sec ^ tv.tv_usec);

	gettimeofday(&tv, 0);
	/* Crank the random number generator a few times */
	ಗಾಗಿ (new_seq = (tv.tv_sec ^ tv.tv_usec) & 0x1F; new_seq > 0; new_seq--)
		rand();

	ಮಾಡು {
		new_seq = rand();
	} ಆಗ (new_seq > EXT4_MMP_SEQ_MAX);

	ಹಿಂತಿರುಗಿ new_seq;
#ಇಲ್ಲದಿದ್ದರೆ
	ಹಿಂತಿರುಗಿ EXT2_ET_OP_NOT_SUPPORTED;
#ಆದರೆ_ಅಂತ್ಯ
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_MMP
ಸ್ಥಿರ errcode_t ext2fs_mmp_reset(ext2_filsys fs)
{
	ರಚನೆ mmp_struct *mmp_s = ಖಾಲಿ;
	errcode_t retval = 0;

	ಆದರೆ (fs->mmp_buf == ಖಾಲಿ) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		ಆದರೆ (retval)
			ಹೋಗು ಹೊರಹೋಗು;
	}

	memset(fs->mmp_buf, 0, fs->blocksize);
	mmp_s = fs->mmp_buf;

	mmp_s->mmp_magic = EXT4_MMP_MAGIC;
	mmp_s->mmp_seq = EXT4_MMP_SEQ_CLEAN;
	mmp_s->mmp_ಸಮಯ = 0;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_GETHOSTNAME
	gethostname((ಅಕ್ಷರ *) mmp_s->mmp_nodename, ಗಾತ್ರ(mmp_s->mmp_nodename));
#ಇಲ್ಲದಿದ್ದರೆ
	mmp_s->mmp_nodename[0] = '\0';
#ಆದರೆ_ಅಂತ್ಯ
	strncpy((ಅಕ್ಷರ *) mmp_s->mmp_bdevname, fs->device_name,
		ಗಾತ್ರ(mmp_s->mmp_bdevname));

	mmp_s->mmp_check_interval = fs->super->s_mmp_update_interval;
	ಆದರೆ (mmp_s->mmp_check_interval < EXT4_MMP_MIN_CHECK_INTERVAL)
		mmp_s->mmp_check_interval = EXT4_MMP_MIN_CHECK_INTERVAL;

	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_ಖಂಡ, fs->mmp_buf);
ಹೊರಹೋಗು:
	ಹಿಂತಿರುಗಿ retval;
}
#ಆದರೆ_ಅಂತ್ಯ

errcode_t ext2fs_mmp_update(ext2_filsys fs)
{
	ಹಿಂತಿರುಗಿ ext2fs_mmp_update2(fs, 0);
}

errcode_t ext2fs_mmp_clear(ext2_filsys fs)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_MMP
	errcode_t retval = 0;

	ಆದರೆ (!(fs->flags & EXT2_FLAG_RW))
		ಹಿಂತಿರುಗಿ EXT2_ET_RO_FILSYS;

	retval = ext2fs_mmp_reset(fs);

	ಹಿಂತಿರುಗಿ retval;
#ಇಲ್ಲದಿದ್ದರೆ
	ಹಿಂತಿರುಗಿ EXT2_ET_OP_NOT_SUPPORTED;
#ಆದರೆ_ಅಂತ್ಯ
}

errcode_t ext2fs_mmp_init(ext2_filsys fs)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_MMP
	ರಚನೆ ext2_super_ಖಂಡ *sb = fs->super;
	blk64_t mmp_ಖಂಡ;
	errcode_t retval;

	ಆದರೆ (sb->s_mmp_update_interval == 0)
		sb->s_mmp_update_interval = EXT4_MMP_UPDATE_INTERVAL;
	/* This is probably excessively large, but who knows? */
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (sb->s_mmp_update_interval > EXT4_MMP_MAX_UPDATE_INTERVAL)
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

	ಆದರೆ (fs->mmp_buf == ಖಾಲಿ) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		ಆದರೆ (retval)
			ಹೋಗು ಹೊರಹೋಗು;
	}

	retval = ext2fs_alloc_block2(fs, 0, fs->mmp_buf, &mmp_ಖಂಡ);
	ಆದರೆ (retval)
		ಹೋಗು ಹೊರಹೋಗು;

	sb->s_mmp_ಖಂಡ = mmp_ಖಂಡ;

	retval = ext2fs_mmp_reset(fs);
	ಆದರೆ (retval)
		ಹೋಗು ಹೊರಹೋಗು;

ಹೊರಹೋಗು:
	ಹಿಂತಿರುಗಿ retval;
#ಇಲ್ಲದಿದ್ದರೆ
	ಹಿಂತಿರುಗಿ EXT2_ET_OP_NOT_SUPPORTED;
#ಆದರೆ_ಅಂತ್ಯ
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ min
#ವ್ಯಾಖ್ಯಾನಿಸು min(x, y) ((x) < (y) ? (x) : (y))
#ಆದರೆ_ಅಂತ್ಯ

/*
 * Make sure that the fs is not mounted or being fsck'ed while opening the fs.
 */
errcode_t ext2fs_mmp_ಆರಂಭ(ext2_filsys fs)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_MMP
	ರಚನೆ mmp_struct *mmp_s;
	ಚಿಹ್ನೆರಹಿತ seq;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ mmp_check_interval;
	errcode_t retval = 0;

	ಆದರೆ (fs->mmp_buf == ಖಾಲಿ) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		ಆದರೆ (retval)
			ಹೋಗು mmp_error;
	}

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_ಖಂಡ, fs->mmp_buf);
	ಆದರೆ (retval)
		ಹೋಗು mmp_error;

	mmp_s = fs->mmp_buf;

	mmp_check_interval = fs->super->s_mmp_update_interval;
	ಆದರೆ (mmp_check_interval < EXT4_MMP_MIN_CHECK_INTERVAL)
		mmp_check_interval = EXT4_MMP_MIN_CHECK_INTERVAL;

	seq = mmp_s->mmp_seq;
	ಆದರೆ (seq == EXT4_MMP_SEQ_CLEAN)
		ಹೋಗು clean_seq;
	ಆದರೆ (seq == EXT4_MMP_SEQ_FSCK) {
		retval = EXT2_ET_MMP_FSCK_ON;
		ಹೋಗು mmp_error;
	}

	ಆದರೆ (seq > EXT4_MMP_SEQ_FSCK) {
		retval = EXT2_ET_MMP_UNKNOWN_SEQ;
		ಹೋಗು mmp_error;
	}

	/*
	 * If check_interval in MMP block is larger, use that instead of
	 * check_interval from the superblock.
	 */
	ಆದರೆ (mmp_s->mmp_check_interval > mmp_check_interval)
		mmp_check_interval = mmp_s->mmp_check_interval;

	sleep(min(mmp_check_interval * 2 + 1, mmp_check_interval + 60));

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_ಖಂಡ, fs->mmp_buf);
	ಆದರೆ (retval)
		ಹೋಗು mmp_error;

	ಆದರೆ (seq != mmp_s->mmp_seq) {
		retval = EXT2_ET_MMP_FAILED;
		ಹೋಗು mmp_error;
	}

clean_seq:
	ಆದರೆ (!(fs->flags & EXT2_FLAG_RW))
		ಹೋಗು mmp_error;

	mmp_s->mmp_seq = seq = ext2fs_mmp_new_seq();
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_GETHOSTNAME
	gethostname((ಅಕ್ಷರ *) mmp_s->mmp_nodename, ಗಾತ್ರ(mmp_s->mmp_nodename));
#ಇಲ್ಲದಿದ್ದರೆ
	strcpy((ಅಕ್ಷರ *) mmp_s->mmp_nodename, "unknown host");
#ಆದರೆ_ಅಂತ್ಯ
	strncpy((ಅಕ್ಷರ *) mmp_s->mmp_bdevname, fs->device_name,
		ಗಾತ್ರ(mmp_s->mmp_bdevname));

	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_ಖಂಡ, fs->mmp_buf);
	ಆದರೆ (retval)
		ಹೋಗು mmp_error;

	sleep(min(2 * mmp_check_interval + 1, mmp_check_interval + 60));

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_ಖಂಡ, fs->mmp_buf);
	ಆದರೆ (retval)
		ಹೋಗು mmp_error;

	ಆದರೆ (seq != mmp_s->mmp_seq) {
		retval = EXT2_ET_MMP_FAILED;
		ಹೋಗು mmp_error;
	}

	mmp_s->mmp_seq = EXT4_MMP_SEQ_FSCK;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_ಖಂಡ, fs->mmp_buf);
	ಆದರೆ (retval)
		ಹೋಗು mmp_error;

	ಹಿಂತಿರುಗಿ 0;

mmp_error:
	ಹಿಂತಿರುಗಿ retval;
#ಇಲ್ಲದಿದ್ದರೆ
	ಹಿಂತಿರುಗಿ EXT2_ET_OP_NOT_SUPPORTED;
#ಆದರೆ_ಅಂತ್ಯ
}

/*
 * Clear the MMP usage in the filesystem.  If this function returns an
 * error EXT2_ET_MMP_CHANGE_ABORT it means the filesystem was modified
 * by some other process while in use, and changes should be dropped, or
 * risk filesystem corruption.
 */
errcode_t ext2fs_mmp_stop(ext2_filsys fs)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_MMP
	ರಚನೆ mmp_struct *mmp, *mmp_cmp;
	errcode_t retval = 0;

	ಆದರೆ (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP) ||
	    (fs->mmp_buf == ಖಾಲಿ) || (fs->mmp_cmp == ಖಾಲಿ))
		ಹೋಗು mmp_error;

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_ಖಂಡ, ಖಾಲಿ);
	ಆದರೆ (retval)
		ಹೋಗು mmp_error;

	/* Check if the MMP block is not changed. */
	mmp = fs->mmp_buf;
	mmp_cmp = fs->mmp_cmp;
	ಆದರೆ (memcmp(mmp, mmp_cmp, ಗಾತ್ರ(*mmp_cmp))) {
		retval = EXT2_ET_MMP_CHANGE_ABORT;
		ಹೋಗು mmp_error;
	}

	mmp_cmp->mmp_seq = EXT4_MMP_SEQ_CLEAN;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_ಖಂಡ, fs->mmp_cmp);

mmp_error:
	ಆದರೆ (fs->mmp_fd > 0) {
		close(fs->mmp_fd);
		fs->mmp_fd = -1;
	}

	ಹಿಂತಿರುಗಿ retval;
#ಇಲ್ಲದಿದ್ದರೆ
	ಆದರೆ (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ EXT2_ET_OP_NOT_SUPPORTED;
#ಆದರೆ_ಅಂತ್ಯ
}

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_MIN_MMP_UPDATE_INTERVAL 60

/*
 * Update the on-disk mmp buffer, after checking that it hasn't been changed.
 */
errcode_t ext2fs_mmp_update2(ext2_filsys fs, ಪೂರ್ಣಾಂಕ immediately)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_MMP
	ರಚನೆ mmp_struct *mmp, *mmp_cmp;
	ರಚನೆ timeval tv;
	errcode_t retval = 0;

	ಆದರೆ (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		ಹಿಂತಿರುಗಿ 0;

	gettimeofday(&tv, 0);
	ಆದರೆ (!immediately &&
	    tv.tv_sec - fs->mmp_ಕೊನೆಯ_written < EXT2_MIN_MMP_UPDATE_INTERVAL)
		ಹಿಂತಿರುಗಿ 0;

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_ಖಂಡ, ಖಾಲಿ);
	ಆದರೆ (retval)
		ಹೋಗು mmp_error;

	mmp = fs->mmp_buf;
	mmp_cmp = fs->mmp_cmp;

	ಆದರೆ (memcmp(mmp, mmp_cmp, ಗಾತ್ರ(*mmp_cmp)))
		ಹಿಂತಿರುಗಿ EXT2_ET_MMP_CHANGE_ABORT;

	/*
	 * Believe it or not, ext2fs_mmp_read actually overwrites fs->mmp_cmp
	 * and leaves fs->mmp_buf untouched.  Hence we copy mmp_cmp into
	 * mmp_buf, update mmp_buf, and write mmp_buf out to disk.
	 */
	ಸ್ಮರಣೆ_ನಕಲು(mmp, mmp_cmp, ಗಾತ್ರ(*mmp_cmp));
	mmp->mmp_ಸಮಯ = tv.tv_sec;
	mmp->mmp_seq = EXT4_MMP_SEQ_FSCK;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_ಖಂಡ, fs->mmp_buf);

mmp_error:
	ಹಿಂತಿರುಗಿ retval;
#ಇಲ್ಲದಿದ್ದರೆ
	ಆದರೆ (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ EXT2_ET_OP_NOT_SUPPORTED;
#ಆದರೆ_ಅಂತ್ಯ
}
#ಆದರೆ __GNUC_PREREQ (4, 6)
#ಸೂಚನೆ GCC diagnostic ತೆಗೆ
#ಆದರೆ_ಅಂತ್ಯ
