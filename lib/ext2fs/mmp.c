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

#నిర్వచితం_కాకపోతే GNU_ఆధారం
#నిర్వచించు GNU_ఆధారం
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే _DEFAULT_SOURCE
#నిర్వచించు _DEFAULT_SOURCE	/* since glibc 2.20 _SVID_SOURCE is deprecated */
#అయితే_ముగింపు

#అన్తర్భావ "config.h"

#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <sys/time.h>

#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <fcntl.h>

#అన్తర్భావ "ext2fs/ext2_fs.h"
#అన్తర్భావ "ext2fs/ext2fs.h"

#నిర్వచితం_కాకపోతే O_ప్రత్యక్ష
#నిర్వచించు O_ప్రత్యక్ష 0
#అయితే_ముగింపు

#ఒకవేళ __GNUC_PREREQ (4, 6)
#సూచన GCC diagnostic నెట్టు
#నిర్వచితం_కాకపోతే CONFIG_MMP
#సూచన GCC diagnostic ignored "-Wunused-parameter"
#అయితే_ముగింపు
#అయితే_ముగింపు

errcode_t ext2fs_mmp_get_mem(ext2_filsys fs, శూన్య **ptr)
{
	పూర్ణాంకం align = ext2fs_get_dio_alignment(fs->mmp_fd);

	తిరిగి ext2fs_get_memalign(fs->blocksize, align, ptr);
}

errcode_t ext2fs_mmp_read(ext2_filsys fs, blk64_t mmp_blk, శూన్య *buf)
{
#నిర్వచితమైతే CONFIG_MMP
	నిర్మాణం mmp_struct *mmp_cmp;
	errcode_t retval = 0;

	ఒకవేళ ((mmp_blk <= fs->super->s_మొదటి_దత్తాంశం_ఖండం) ||
	    (mmp_blk >= ext2fs_ఖండాలు_లెక్క(fs->super)))
		తిరిగి EXT2_ET_MMP_BAD_ఖండం;

	/* ext2fs_open() reserves fd0,1,2 to avoid stdio collision, so checking
	 * mmp_fd <= 0 is OK to validate that the fd is valid.  This opens its
	 * own fd to read the MMP block to ensure that it is using O_DIRECT,
	 * regardless of how the io_manager is doing reads, to avoid caching of
	 * the MMP block by the io_manager or the VM.  It needs to be fresh. */
	ఒకవేళ (fs->mmp_fd <= 0) {
		నిర్మాణం stat st;
		పూర్ణాంకం flags = O_RDONLY | O_ప్రత్యక్ష;

		/*
		 * There is no reason for using O_DIRECT if we're working with
		 * regular file. Disabling it also avoids problems with
		 * alignment when the device of the host file system has sector
		 * size larger than blocksize of the fs we're working with.
		 */
		ఒకవేళ (stat(fs->device_name, &st) == 0 &&
		    S_ISREG(st.st_బహుళకం))
			flags &= ~O_ప్రత్యక్ష;

		fs->mmp_fd = open(fs->device_name, flags);
		ఒకవేళ (fs->mmp_fd < 0) {
			retval = EXT2_ET_MMP_OPEN_ప్రత్యక్ష;
			వెళ్ళు బహిర్గతం;
		}
	}

	ఒకవేళ (fs->mmp_cmp == ఖాళీ) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_cmp);
		ఒకవేళ (retval)
			తిరిగి retval;
	}

	ఒకవేళ ((blk64_t) ext2fs_llseek(fs->mmp_fd, mmp_blk * fs->blocksize,
				    SEEK_SET) !=
	    mmp_blk * fs->blocksize) {
		retval = EXT2_ET_LLSEEK_FAILED;
		వెళ్ళు బహిర్గతం;
	}

	ఒకవేళ (పఠ(fs->mmp_fd, fs->mmp_cmp, fs->blocksize) != fs->blocksize) {
		retval = EXT2_ET_SHORT_READ;
		వెళ్ళు బహిర్గతం;
	}

	mmp_cmp = fs->mmp_cmp;

	ఒకవేళ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_mmp_csum_verify(fs, mmp_cmp))
		retval = EXT2_ET_MMP_CSUM_INVALID;

#నిర్వచితమైతే పదాలు_BIGENDIAN
	ext2fs_మార్పిడి_mmp(mmp_cmp);
#అయితే_ముగింపు

	ఒకవేళ (buf != ఖాళీ && buf != fs->mmp_cmp)
		జ్ఞాపకం_నకలు(buf, fs->mmp_cmp, fs->blocksize);

	ఒకవేళ (mmp_cmp->mmp_magic != EXT4_MMP_MAGIC) {
		retval = EXT2_ET_MMP_MAGIC_INVALID;
		వెళ్ళు బహిర్గతం;
	}

బహిర్గతం:
	తిరిగి retval;
#లేకపోతే
	తిరిగి EXT2_ET_OP_NOT_SUPPORTED;
#అయితే_ముగింపు
}

errcode_t ext2fs_mmp_write(ext2_filsys fs, blk64_t mmp_blk, శూన్య *buf)
{
#నిర్వచితమైతే CONFIG_MMP
	నిర్మాణం mmp_struct *mmp_s = buf;
	నిర్మాణం timeval tv;
	errcode_t retval = 0;

	gettimeofday(&tv, 0);
	mmp_s->mmp_సమయం = tv.tv_sec;
	fs->mmp_చివరి_written = tv.tv_sec;

	ఒకవేళ (fs->super->s_mmp_ఖండం < fs->super->s_మొదటి_దత్తాంశం_ఖండం ||
	    fs->super->s_mmp_ఖండం > ext2fs_ఖండాలు_లెక్క(fs->super))
		తిరిగి EXT2_ET_MMP_BAD_ఖండం;

#నిర్వచితమైతే పదాలు_BIGENDIAN
	ext2fs_మార్పిడి_mmp(mmp_s);
#అయితే_ముగింపు

	retval = ext2fs_mmp_csum_సమితి(fs, mmp_s);
	ఒకవేళ (retval)
		తిరిగి retval;

	/* I was tempted to make this use O_DIRECT and the mmp_fd, but
	 * this caused no end of grief, while leaving it as-is works. */
	retval = io_channel_write_blk64(fs->io, mmp_blk, -(పూర్ణాంకం)పరిమాణం(నిర్మాణం mmp_struct), buf);

#నిర్వచితమైతే పదాలు_BIGENDIAN
	ext2fs_మార్పిడి_mmp(mmp_s);
#అయితే_ముగింపు

	/* Make sure the block gets to disk quickly */
	io_channel_flush(fs->io);
	తిరిగి retval;
#లేకపోతే
	తిరిగి EXT2_ET_OP_NOT_SUPPORTED;
#అయితే_ముగింపు
}

#నిర్వచితమైతే HAVE_SRANDOM
#నిర్వచించు srand(x)	srandom(x)
#నిర్వచించు rand()		random()
#అయితే_ముగింపు

గుర్తులేని ext2fs_mmp_new_seq(శూన్య)
{
#నిర్వచితమైతే CONFIG_MMP
	గుర్తులేని new_seq;
	నిర్మాణం timeval tv;
	గుర్తులేని దీర్ఘ pid = getpid();

	gettimeofday(&tv, 0);
	pid = (pid >> 16) | ((pid & 0xFFFF) << 16);
	srand(pid ^ getuid() ^ tv.tv_sec ^ tv.tv_usec);

	gettimeofday(&tv, 0);
	/* Crank the random number generator a few times */
	కొరకు (new_seq = (tv.tv_sec ^ tv.tv_usec) & 0x1F; new_seq > 0; new_seq--)
		rand();

	చేయి {
		new_seq = rand();
	} అయితే (new_seq > EXT4_MMP_SEQ_MAX);

	తిరిగి new_seq;
#లేకపోతే
	తిరిగి EXT2_ET_OP_NOT_SUPPORTED;
#అయితే_ముగింపు
}

#నిర్వచితమైతే CONFIG_MMP
స్థిర errcode_t ext2fs_mmp_reset(ext2_filsys fs)
{
	నిర్మాణం mmp_struct *mmp_s = ఖాళీ;
	errcode_t retval = 0;

	ఒకవేళ (fs->mmp_buf == ఖాళీ) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		ఒకవేళ (retval)
			వెళ్ళు బహిర్గతం;
	}

	memset(fs->mmp_buf, 0, fs->blocksize);
	mmp_s = fs->mmp_buf;

	mmp_s->mmp_magic = EXT4_MMP_MAGIC;
	mmp_s->mmp_seq = EXT4_MMP_SEQ_CLEAN;
	mmp_s->mmp_సమయం = 0;
#నిర్వచితమైతే HAVE_GETHOSTNAME
	gethostname((అక్షరం *) mmp_s->mmp_nodename, పరిమాణం(mmp_s->mmp_nodename));
#లేకపోతే
	mmp_s->mmp_nodename[0] = '\0';
#అయితే_ముగింపు
	strncpy((అక్షరం *) mmp_s->mmp_bdevname, fs->device_name,
		పరిమాణం(mmp_s->mmp_bdevname));

	mmp_s->mmp_check_interval = fs->super->s_mmp_update_interval;
	ఒకవేళ (mmp_s->mmp_check_interval < EXT4_MMP_MIN_CHECK_INTERVAL)
		mmp_s->mmp_check_interval = EXT4_MMP_MIN_CHECK_INTERVAL;

	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_ఖండం, fs->mmp_buf);
బహిర్గతం:
	తిరిగి retval;
}
#అయితే_ముగింపు

errcode_t ext2fs_mmp_update(ext2_filsys fs)
{
	తిరిగి ext2fs_mmp_update2(fs, 0);
}

errcode_t ext2fs_mmp_clear(ext2_filsys fs)
{
#నిర్వచితమైతే CONFIG_MMP
	errcode_t retval = 0;

	ఒకవేళ (!(fs->flags & EXT2_FLAG_RW))
		తిరిగి EXT2_ET_RO_FILSYS;

	retval = ext2fs_mmp_reset(fs);

	తిరిగి retval;
#లేకపోతే
	తిరిగి EXT2_ET_OP_NOT_SUPPORTED;
#అయితే_ముగింపు
}

errcode_t ext2fs_mmp_init(ext2_filsys fs)
{
#నిర్వచితమైతే CONFIG_MMP
	నిర్మాణం ext2_super_ఖండం *sb = fs->super;
	blk64_t mmp_ఖండం;
	errcode_t retval;

	ఒకవేళ (sb->s_mmp_update_interval == 0)
		sb->s_mmp_update_interval = EXT4_MMP_UPDATE_INTERVAL;
	/* This is probably excessively large, but who knows? */
	లేకపోతే ఒకవేళ (sb->s_mmp_update_interval > EXT4_MMP_MAX_UPDATE_INTERVAL)
		తిరిగి EXT2_ET_INVALID_వాదం;

	ఒకవేళ (fs->mmp_buf == ఖాళీ) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		ఒకవేళ (retval)
			వెళ్ళు బహిర్గతం;
	}

	retval = ext2fs_alloc_block2(fs, 0, fs->mmp_buf, &mmp_ఖండం);
	ఒకవేళ (retval)
		వెళ్ళు బహిర్గతం;

	sb->s_mmp_ఖండం = mmp_ఖండం;

	retval = ext2fs_mmp_reset(fs);
	ఒకవేళ (retval)
		వెళ్ళు బహిర్గతం;

బహిర్గతం:
	తిరిగి retval;
#లేకపోతే
	తిరిగి EXT2_ET_OP_NOT_SUPPORTED;
#అయితే_ముగింపు
}

#నిర్వచితం_కాకపోతే min
#నిర్వచించు min(x, y) ((x) < (y) ? (x) : (y))
#అయితే_ముగింపు

/*
 * Make sure that the fs is not mounted or being fsck'ed while opening the fs.
 */
errcode_t ext2fs_mmp_ప్రారంభం(ext2_filsys fs)
{
#నిర్వచితమైతే CONFIG_MMP
	నిర్మాణం mmp_struct *mmp_s;
	గుర్తులేని seq;
	గుర్తులేని పూర్ణాంకం mmp_check_interval;
	errcode_t retval = 0;

	ఒకవేళ (fs->mmp_buf == ఖాళీ) {
		retval = ext2fs_mmp_get_mem(fs, &fs->mmp_buf);
		ఒకవేళ (retval)
			వెళ్ళు mmp_error;
	}

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_ఖండం, fs->mmp_buf);
	ఒకవేళ (retval)
		వెళ్ళు mmp_error;

	mmp_s = fs->mmp_buf;

	mmp_check_interval = fs->super->s_mmp_update_interval;
	ఒకవేళ (mmp_check_interval < EXT4_MMP_MIN_CHECK_INTERVAL)
		mmp_check_interval = EXT4_MMP_MIN_CHECK_INTERVAL;

	seq = mmp_s->mmp_seq;
	ఒకవేళ (seq == EXT4_MMP_SEQ_CLEAN)
		వెళ్ళు clean_seq;
	ఒకవేళ (seq == EXT4_MMP_SEQ_FSCK) {
		retval = EXT2_ET_MMP_FSCK_ON;
		వెళ్ళు mmp_error;
	}

	ఒకవేళ (seq > EXT4_MMP_SEQ_FSCK) {
		retval = EXT2_ET_MMP_UNKNOWN_SEQ;
		వెళ్ళు mmp_error;
	}

	/*
	 * If check_interval in MMP block is larger, use that instead of
	 * check_interval from the superblock.
	 */
	ఒకవేళ (mmp_s->mmp_check_interval > mmp_check_interval)
		mmp_check_interval = mmp_s->mmp_check_interval;

	sleep(min(mmp_check_interval * 2 + 1, mmp_check_interval + 60));

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_ఖండం, fs->mmp_buf);
	ఒకవేళ (retval)
		వెళ్ళు mmp_error;

	ఒకవేళ (seq != mmp_s->mmp_seq) {
		retval = EXT2_ET_MMP_FAILED;
		వెళ్ళు mmp_error;
	}

clean_seq:
	ఒకవేళ (!(fs->flags & EXT2_FLAG_RW))
		వెళ్ళు mmp_error;

	mmp_s->mmp_seq = seq = ext2fs_mmp_new_seq();
#నిర్వచితమైతే HAVE_GETHOSTNAME
	gethostname((అక్షరం *) mmp_s->mmp_nodename, పరిమాణం(mmp_s->mmp_nodename));
#లేకపోతే
	strcpy((అక్షరం *) mmp_s->mmp_nodename, "unknown host");
#అయితే_ముగింపు
	strncpy((అక్షరం *) mmp_s->mmp_bdevname, fs->device_name,
		పరిమాణం(mmp_s->mmp_bdevname));

	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_ఖండం, fs->mmp_buf);
	ఒకవేళ (retval)
		వెళ్ళు mmp_error;

	sleep(min(2 * mmp_check_interval + 1, mmp_check_interval + 60));

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_ఖండం, fs->mmp_buf);
	ఒకవేళ (retval)
		వెళ్ళు mmp_error;

	ఒకవేళ (seq != mmp_s->mmp_seq) {
		retval = EXT2_ET_MMP_FAILED;
		వెళ్ళు mmp_error;
	}

	mmp_s->mmp_seq = EXT4_MMP_SEQ_FSCK;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_ఖండం, fs->mmp_buf);
	ఒకవేళ (retval)
		వెళ్ళు mmp_error;

	తిరిగి 0;

mmp_error:
	తిరిగి retval;
#లేకపోతే
	తిరిగి EXT2_ET_OP_NOT_SUPPORTED;
#అయితే_ముగింపు
}

/*
 * Clear the MMP usage in the filesystem.  If this function returns an
 * error EXT2_ET_MMP_CHANGE_ABORT it means the filesystem was modified
 * by some other process while in use, and changes should be dropped, or
 * risk filesystem corruption.
 */
errcode_t ext2fs_mmp_stop(ext2_filsys fs)
{
#నిర్వచితమైతే CONFIG_MMP
	నిర్మాణం mmp_struct *mmp, *mmp_cmp;
	errcode_t retval = 0;

	ఒకవేళ (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP) ||
	    (fs->mmp_buf == ఖాళీ) || (fs->mmp_cmp == ఖాళీ))
		వెళ్ళు mmp_error;

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_ఖండం, ఖాళీ);
	ఒకవేళ (retval)
		వెళ్ళు mmp_error;

	/* Check if the MMP block is not changed. */
	mmp = fs->mmp_buf;
	mmp_cmp = fs->mmp_cmp;
	ఒకవేళ (memcmp(mmp, mmp_cmp, పరిమాణం(*mmp_cmp))) {
		retval = EXT2_ET_MMP_CHANGE_ABORT;
		వెళ్ళు mmp_error;
	}

	mmp_cmp->mmp_seq = EXT4_MMP_SEQ_CLEAN;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_ఖండం, fs->mmp_cmp);

mmp_error:
	ఒకవేళ (fs->mmp_fd > 0) {
		close(fs->mmp_fd);
		fs->mmp_fd = -1;
	}

	తిరిగి retval;
#లేకపోతే
	ఒకవేళ (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		తిరిగి 0;

	తిరిగి EXT2_ET_OP_NOT_SUPPORTED;
#అయితే_ముగింపు
}

#నిర్వచించు EXT2_MIN_MMP_UPDATE_INTERVAL 60

/*
 * Update the on-disk mmp buffer, after checking that it hasn't been changed.
 */
errcode_t ext2fs_mmp_update2(ext2_filsys fs, పూర్ణాంకం immediately)
{
#నిర్వచితమైతే CONFIG_MMP
	నిర్మాణం mmp_struct *mmp, *mmp_cmp;
	నిర్మాణం timeval tv;
	errcode_t retval = 0;

	ఒకవేళ (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		తిరిగి 0;

	gettimeofday(&tv, 0);
	ఒకవేళ (!immediately &&
	    tv.tv_sec - fs->mmp_చివరి_written < EXT2_MIN_MMP_UPDATE_INTERVAL)
		తిరిగి 0;

	retval = ext2fs_mmp_read(fs, fs->super->s_mmp_ఖండం, ఖాళీ);
	ఒకవేళ (retval)
		వెళ్ళు mmp_error;

	mmp = fs->mmp_buf;
	mmp_cmp = fs->mmp_cmp;

	ఒకవేళ (memcmp(mmp, mmp_cmp, పరిమాణం(*mmp_cmp)))
		తిరిగి EXT2_ET_MMP_CHANGE_ABORT;

	/*
	 * Believe it or not, ext2fs_mmp_read actually overwrites fs->mmp_cmp
	 * and leaves fs->mmp_buf untouched.  Hence we copy mmp_cmp into
	 * mmp_buf, update mmp_buf, and write mmp_buf out to disk.
	 */
	జ్ఞాపకం_నకలు(mmp, mmp_cmp, పరిమాణం(*mmp_cmp));
	mmp->mmp_సమయం = tv.tv_sec;
	mmp->mmp_seq = EXT4_MMP_SEQ_FSCK;
	retval = ext2fs_mmp_write(fs, fs->super->s_mmp_ఖండం, fs->mmp_buf);

mmp_error:
	తిరిగి retval;
#లేకపోతే
	ఒకవేళ (!ext2fs_has_feature_mmp(fs->super) ||
	    !(fs->flags & EXT2_FLAG_RW) || (fs->flags & EXT2_FLAG_SKIP_MMP))
		తిరిగి 0;

	తిరిగి EXT2_ET_OP_NOT_SUPPORTED;
#అయితే_ముగింపు
}
#ఒకవేళ __GNUC_PREREQ (4, 6)
#సూచన GCC diagnostic తీయి
#అయితే_ముగింపు
