/*
 * image.c --- writes out the critical parts of the filesystem as a
 * 	flat file.
 *
 * Copyright (C) 2000 Theodore Ts'o.
 *
 * Note: this uses the POSIX IO interfaces, unlike most of the other
 * functions in this library.  So sue me.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

#నిర్వచితం_కాకపోతే HAVE_TYPE_SSIZE_T
రకనిర్వచనం పూర్ణాంకం ssize_t;
#అయితే_ముగింపు

/*
 * This function returns 1 if the specified block is all zeros
 */
స్థిర పూర్ణాంకం check_zero_ఖండం(అక్షరం *buf, పూర్ణాంకం blocksize)
{
	అక్షరం	*cp = buf;
	పూర్ణాంకం	ఎడమ = blocksize;

	అయితే (ఎడమ > 0) {
		ఒకవేళ (*cp++)
			తిరిగి 0;
		ఎడమ--;
	}
	తిరిగి 1;
}

/*
 * Write the inode table out as a single block.
 */
#నిర్వచించు BUF_ఖండాలు	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, పూర్ణాంకం fd, పూర్ణాంకం flags)
{
	dgrp_t		సమూహం;
	ssize_t		ఎడమ, c, d;
	అక్షరం		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = జ్ఞాపకం_ఇవ్వు(fs->blocksize * BUF_ఖండాలు);
	ఒకవేళ (!buf)
		తిరిగి ENOMEM;

	కొరకు (సమూహం = 0; సమూహం < fs->సమూహం_desc_లెక్క; సమూహం++) {
		blk = ext2fs_inode_పట్టిక_loc(fs, సమూహం);
		ఒకవేళ (!blk) {
			retval = EXT2_ET_MISSING_INODE_పట్టిక;
			వెళ్ళు errout;
		}
		ఎడమ = fs->inode_ఖండాలు_చొప్పున_సమూహం;
		ఒకవేళ ((blk < fs->super->s_మొదటి_దత్తాంశం_ఖండం) ||
		    (blk + ఎడమ - 1 >= ext2fs_ఖండాలు_లెక్క(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_పట్టిక;
			వెళ్ళు errout;
		}
		అయితే (ఎడమ) {
			c = BUF_ఖండాలు;
			ఒకవేళ (c > ఎడమ)
				c = ఎడమ;
			retval = io_channel_read_blk64(fs->io, blk, c, buf);
			ఒకవేళ (retval)
				వెళ్ళు errout;
			cp = buf;
			అయితే (c) {
				ఒకవేళ (!(flags & IMAGER_FLAG_SPARSEWRITE)) {
					d = c;
					వెళ్ళు skip_sparse;
				}
				/* Skip zero blocks */
				ఒకవేళ (check_zero_ఖండం(cp, fs->blocksize)) {
					c--;
					blk++;
					ఎడమ--;
					cp += fs->blocksize;
					r = ext2fs_llseek(fd, fs->blocksize,
							  SEEK_CUR);
					ఒకవేళ (r < 0) {
						retval = errno;
						వెళ్ళు errout;
					}
					కొనసాగు;
				}
				/* Find non-zero blocks */
				కొరకు (d = 1; d < c; d++) {
					ఒకవేళ (check_zero_ఖండం(cp +
							     d * fs->blocksize,
							     fs->blocksize))
						ఆపు;
				}
			skip_sparse:
				actual = లిఖ(fd, cp, d * fs->blocksize);
				ఒకవేళ (actual == -1) {
					retval = errno;
					వెళ్ళు errout;
				}
				ఒకవేళ (actual != d * fs->blocksize) {
					retval = EXT2_ET_SHORT_WRITE;
					వెళ్ళు errout;
				}
				blk += d;
				ఎడమ -= d;
				cp += d * fs->blocksize;
				c -= d;
			}
		}
	}
	retval = 0;

errout:
	విడుదల(buf);
	తిరిగి retval;
}

/*
 * Read in the inode table and stuff it into place
 */
errcode_t ext2fs_image_inode_read(ext2_filsys fs, పూర్ణాంకం fd,
				  పూర్ణాంకం flags EXT2FS_ATTR((వాడని)))
{
	dgrp_t		సమూహం;
	ssize_t		c, ఎడమ;
	అక్షరం		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = జ్ఞాపకం_ఇవ్వు(fs->blocksize * BUF_ఖండాలు);
	ఒకవేళ (!buf)
		తిరిగి ENOMEM;

	కొరకు (సమూహం = 0; సమూహం < fs->సమూహం_desc_లెక్క; సమూహం++) {
		blk = ext2fs_inode_పట్టిక_loc(fs, సమూహం);
		ఒకవేళ (!blk) {
			retval = EXT2_ET_MISSING_INODE_పట్టిక;
			వెళ్ళు errout;
		}
		ఎడమ = fs->inode_ఖండాలు_చొప్పున_సమూహం;
		అయితే (ఎడమ) {
			c = BUF_ఖండాలు;
			ఒకవేళ (c > ఎడమ)
				c = ఎడమ;
			actual = పఠ(fd, buf, fs->blocksize * c);
			ఒకవేళ (actual == -1) {
				retval = errno;
				వెళ్ళు errout;
			}
			ఒకవేళ (actual != fs->blocksize * c) {
				retval = EXT2_ET_SHORT_READ;
				వెళ్ళు errout;
			}
			retval = io_channel_write_blk64(fs->io, blk, c, buf);
			ఒకవేళ (retval)
				వెళ్ళు errout;

			blk += c;
			ఎడమ -= c;
		}
	}
	retval = ext2fs_flush_icache(fs);

errout:
	విడుదల(buf);
	తిరిగి retval;
}

/*
 * Write out superblock and group descriptors
 */
errcode_t ext2fs_image_super_write(ext2_filsys fs, పూర్ణాంకం fd,
				   పూర్ణాంకం flags EXT2FS_ATTR((వాడని)))
{
	అక్షరం		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#నిర్వచితమైతే పదాలు_BIGENDIAN
	గుర్తులేని పూర్ణాంకం	groups_చొప్పున_ఖండం;
	నిర్మాణం		ext2_సమూహం_desc *gdp;
	పూర్ణాంకం		j;
#అయితే_ముగింపు

	ఒకవేళ (fs->సమూహం_desc == ఖాళీ)
		తిరిగి EXT2_ET_NO_GDESC;

	buf = జ్ఞాపకం_ఇవ్వు(fs->blocksize);
	ఒకవేళ (!buf)
		తిరిగి ENOMEM;

	/*
	 * Write out the superblock
	 */
	memset(buf, 0, fs->blocksize);
#నిర్వచితమైతే పదాలు_BIGENDIAN
	/*
	 * We're writing out superblock so let's convert
	 * it to little endian and then back if needed
	 */
	ext2fs_మార్పిడి_super(fs->super);
	జ్ఞాపకం_నకలు(buf, fs->super, SUPERBLOCK_కొలత);
	ext2fs_మార్పిడి_super(fs->super);
#లేకపోతే
	జ్ఞాపకం_నకలు(buf, fs->super, SUPERBLOCK_కొలత);
#అయితే_ముగింపు
	actual = లిఖ(fd, buf, fs->blocksize);
	ఒకవేళ (actual == -1) {
		retval = errno;
		వెళ్ళు errout;
	}
	ఒకవేళ (actual != (ssize_t) fs->blocksize) {
		retval = EXT2_ET_SHORT_WRITE;
		వెళ్ళు errout;
	}

	/*
	 * Now write out the block group descriptors
	 */

	cp = (అక్షరం *) fs->సమూహం_desc;

#నిర్వచితమైతే పదాలు_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_చొప్పున_ఖండం = EXT2_DESC_చొప్పున_ఖండం(fs->super);
	కొరకు (j=0; j < groups_చొప్పున_ఖండం*fs->desc_ఖండాలు; j++) {
		gdp = ext2fs_సమూహం_desc(fs, fs->సమూహం_desc, j);
		ఒకవేళ (gdp)
			ext2fs_మార్పిడి_సమూహం_desc2(fs, gdp);
	}
#అయితే_ముగింపు

	actual = లిఖ(fd, cp, (ssize_t)fs->blocksize * fs->desc_ఖండాలు);


#నిర్వచితమైతే పదాలు_BIGENDIAN
	groups_చొప్పున_ఖండం = EXT2_DESC_చొప్పున_ఖండం(fs->super);
	కొరకు (j=0; j < groups_చొప్పున_ఖండం*fs->desc_ఖండాలు; j++) {
		gdp = ext2fs_సమూహం_desc(fs, fs->సమూహం_desc, j);
		ఒకవేళ (gdp)
			ext2fs_మార్పిడి_సమూహం_desc2(fs, gdp);
	}
#అయితే_ముగింపు

	ఒకవేళ (actual == -1) {
		retval = errno;
		వెళ్ళు errout;
	}
	ఒకవేళ (actual != (ssize_t)(fs->blocksize * fs->desc_ఖండాలు)) {
		retval = EXT2_ET_SHORT_WRITE;
		వెళ్ళు errout;
	}

	retval = 0;

errout:
	విడుదల(buf);
	తిరిగి retval;
}

/*
 * Read the superblock and group descriptors and overwrite them.
 */
errcode_t ext2fs_image_super_read(ext2_filsys fs, పూర్ణాంకం fd,
				  పూర్ణాంకం flags EXT2FS_ATTR((వాడని)))
{
	అక్షరం		*buf;
	ssize_t		actual, కొలత;
	errcode_t	retval;

	కొలత = (ssize_t)fs->blocksize * (fs->desc_ఖండాలు + 1);
	buf = జ్ఞాపకం_ఇవ్వు(కొలత);
	ఒకవేళ (!buf)
		తిరిగి ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = పఠ(fd, buf, కొలత);
	ఒకవేళ (actual == -1) {
		retval = errno;
		వెళ్ళు errout;
	}
	ఒకవేళ (actual != కొలత) {
		retval = EXT2_ET_SHORT_READ;
		వెళ్ళు errout;
	}

	/*
	 * Now copy in the superblock and group descriptors
	 */
	జ్ఞాపకం_నకలు(fs->super, buf, SUPERBLOCK_కొలత);

	జ్ఞాపకం_నకలు(fs->సమూహం_desc, buf + fs->blocksize,
	       (ssize_t)fs->blocksize * fs->desc_ఖండాలు);

	retval = 0;

errout:
	విడుదల(buf);
	తిరిగి retval;
}

/*
 * Write the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, పూర్ణాంకం fd, పూర్ణాంకం flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	ssize_t			actual;
	size_t			c;
	__u64			itr, cnt, కొలత, మొత్తం_కొలత;
	అక్షరం			buf[1024];

	ఒకవేళ (flags & IMAGER_FLAG_INODEMAP) {
		ఒకవేళ (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			ఒకవేళ (retval)
				తిరిగి retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_చొప్పున_సమూహం(fs->super) *
			fs->సమూహం_desc_లెక్క;
		కొలత = (EXT2_INODES_చొప్పున_సమూహం(fs->super) / 8);
	} లేకపోతే {
		ఒకవేళ (!fs->ఖండం_map) {
			retval = ext2fs_read_ఖండం_bitmap(fs);
			ఒకవేళ (retval)
				తిరిగి retval;
		}
		bmap = fs->ఖండం_map;
		itr = fs->super->s_మొదటి_దత్తాంశం_ఖండం;
		cnt = EXT2_GROUPS_గమ్యం_CLUSTERS(fs->super, fs->సమూహం_desc_లెక్క);
		కొలత = EXT2_CLUSTERS_చొప్పున_సమూహం(fs->super) / 8;
	}
	మొత్తం_కొలత = కొలత * fs->సమూహం_desc_లెక్క;

	అయితే (cnt > 0) {
		కొలత = పరిమాణం(buf);
		ఒకవేళ (కొలత > (cnt >> 3))
			కొలత = (cnt >> 3);
		ఒకవేళ (కొలత == 0)
			ఆపు;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       కొలత << 3, buf);
		ఒకవేళ (retval)
			తిరిగి retval;

		actual = లిఖ(fd, buf, కొలత);
		ఒకవేళ (actual == -1)
			తిరిగి errno;
		ఒకవేళ (actual != (పూర్ణాంకం) కొలత)
			తిరిగి EXT2_ET_SHORT_READ;

		itr += కొలత << 3;
		cnt -= కొలత << 3;
	}

	కొలత = మొత్తం_కొలత % fs->blocksize;
	memset(buf, 0, పరిమాణం(buf));
	ఒకవేళ (కొలత) {
		కొలత = fs->blocksize - కొలత;
		అయితే (కొలత) {
			c = కొలత;
			ఒకవేళ (c > (పూర్ణాంకం) పరిమాణం(buf))
				c = పరిమాణం(buf);
			actual = లిఖ(fd, buf, c);
			ఒకవేళ (actual < 0)
				తిరిగి errno;
			ఒకవేళ ((size_t) actual != c)
				తిరిగి EXT2_ET_SHORT_WRITE;
			కొలత -= c;
		}
	}
	తిరిగి 0;
}


/*
 * Read the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, పూర్ణాంకం fd, పూర్ణాంకం flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	__u64			itr, cnt;
	అక్షరం			buf[1024];
	గుర్తులేని పూర్ణాంకం		కొలత;
	ssize_t			actual;

	ఒకవేళ (flags & IMAGER_FLAG_INODEMAP) {
		ఒకవేళ (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			ఒకవేళ (retval)
				తిరిగి retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_చొప్పున_సమూహం(fs->super) *
			fs->సమూహం_desc_లెక్క;
		కొలత = (EXT2_INODES_చొప్పున_సమూహం(fs->super) / 8);
	} లేకపోతే {
		ఒకవేళ (!fs->ఖండం_map) {
			retval = ext2fs_read_ఖండం_bitmap(fs);
			ఒకవేళ (retval)
				తిరిగి retval;
		}
		bmap = fs->ఖండం_map;
		itr = fs->super->s_మొదటి_దత్తాంశం_ఖండం;
		cnt = EXT2_GROUPS_గమ్యం_ఖండాలు(fs->super, fs->సమూహం_desc_లెక్క);
		కొలత = EXT2_ఖండాలు_చొప్పున_సమూహం(fs->super) / 8;
	}

	అయితే (cnt > 0) {
		కొలత = పరిమాణం(buf);
		ఒకవేళ (కొలత > (cnt >> 3))
			కొలత = (cnt >> 3);
		ఒకవేళ (కొలత == 0)
			ఆపు;

		actual = పఠ(fd, buf, కొలత);
		ఒకవేళ (actual == -1)
			తిరిగి errno;
		ఒకవేళ (actual != (పూర్ణాంకం) కొలత)
			తిరిగి EXT2_ET_SHORT_READ;

		retval = ext2fs_సమితి_generic_bmap_range(bmap, itr,
						       కొలత << 3, buf);
		ఒకవేళ (retval)
			తిరిగి retval;

		itr += కొలత << 3;
		cnt -= కొలత << 3;
	}
	తిరిగి 0;
}
