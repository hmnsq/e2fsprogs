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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_TYPE_SSIZE_T
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ ssize_t;
#ಆದರೆ_ಅಂತ್ಯ

/*
 * This function returns 1 if the specified block is all zeros
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_zero_ಖಂಡ(ಅಕ್ಷರ *buf, ಪೂರ್ಣಾಂಕ blocksize)
{
	ಅಕ್ಷರ	*cp = buf;
	ಪೂರ್ಣಾಂಕ	ಎಡ = blocksize;

	ಆಗ (ಎಡ > 0) {
		ಆದರೆ (*cp++)
			ಹಿಂತಿರುಗಿ 0;
		ಎಡ--;
	}
	ಹಿಂತಿರುಗಿ 1;
}

/*
 * Write the inode table out as a single block.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು BUF_ಖಂಡಗಳು	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, ಪೂರ್ಣಾಂಕ fd, ಪೂರ್ಣಾಂಕ flags)
{
	dgrp_t		ಗುಂಪು;
	ssize_t		ಎಡ, c, d;
	ಅಕ್ಷರ		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = ಸ್ಮರಣೆ_ಕೊಡು(fs->blocksize * BUF_ಖಂಡಗಳು);
	ಆದರೆ (!buf)
		ಹಿಂತಿರುಗಿ ENOMEM;

	ಗಾಗಿ (ಗುಂಪು = 0; ಗುಂಪು < fs->ಗುಂಪು_desc_ಎಣಿಕೆ; ಗುಂಪು++) {
		blk = ext2fs_inode_ಕೋಷ್ಟಕ_loc(fs, ಗುಂಪು);
		ಆದರೆ (!blk) {
			retval = EXT2_ET_MISSING_INODE_ಕೋಷ್ಟಕ;
			ಹೋಗು errout;
		}
		ಎಡ = fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು;
		ಆದರೆ ((blk < fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ) ||
		    (blk + ಎಡ - 1 >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_ಕೋಷ್ಟಕ;
			ಹೋಗು errout;
		}
		ಆಗ (ಎಡ) {
			c = BUF_ಖಂಡಗಳು;
			ಆದರೆ (c > ಎಡ)
				c = ಎಡ;
			retval = io_channel_read_blk64(fs->io, blk, c, buf);
			ಆದರೆ (retval)
				ಹೋಗು errout;
			cp = buf;
			ಆಗ (c) {
				ಆದರೆ (!(flags & IMAGER_FLAG_SPARSEWRITE)) {
					d = c;
					ಹೋಗು skip_sparse;
				}
				/* Skip zero blocks */
				ಆದರೆ (check_zero_ಖಂಡ(cp, fs->blocksize)) {
					c--;
					blk++;
					ಎಡ--;
					cp += fs->blocksize;
					r = ext2fs_llseek(fd, fs->blocksize,
							  SEEK_CUR);
					ಆದರೆ (r < 0) {
						retval = errno;
						ಹೋಗು errout;
					}
					ಮುಂದುವರಿ;
				}
				/* Find non-zero blocks */
				ಗಾಗಿ (d = 1; d < c; d++) {
					ಆದರೆ (check_zero_ಖಂಡ(cp +
							     d * fs->blocksize,
							     fs->blocksize))
						ನಿಲ್ಲಿಸು;
				}
			skip_sparse:
				actual = ಲಿಖ(fd, cp, d * fs->blocksize);
				ಆದರೆ (actual == -1) {
					retval = errno;
					ಹೋಗು errout;
				}
				ಆದರೆ (actual != d * fs->blocksize) {
					retval = EXT2_ET_SHORT_WRITE;
					ಹೋಗು errout;
				}
				blk += d;
				ಎಡ -= d;
				cp += d * fs->blocksize;
				c -= d;
			}
		}
	}
	retval = 0;

errout:
	ಬಿಡುಗಡೆ(buf);
	ಹಿಂತಿರುಗಿ retval;
}

/*
 * Read in the inode table and stuff it into place
 */
errcode_t ext2fs_image_inode_read(ext2_filsys fs, ಪೂರ್ಣಾಂಕ fd,
				  ಪೂರ್ಣಾಂಕ flags EXT2FS_ATTR((ಬಳಸದ)))
{
	dgrp_t		ಗುಂಪು;
	ssize_t		c, ಎಡ;
	ಅಕ್ಷರ		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = ಸ್ಮರಣೆ_ಕೊಡು(fs->blocksize * BUF_ಖಂಡಗಳು);
	ಆದರೆ (!buf)
		ಹಿಂತಿರುಗಿ ENOMEM;

	ಗಾಗಿ (ಗುಂಪು = 0; ಗುಂಪು < fs->ಗುಂಪು_desc_ಎಣಿಕೆ; ಗುಂಪು++) {
		blk = ext2fs_inode_ಕೋಷ್ಟಕ_loc(fs, ಗುಂಪು);
		ಆದರೆ (!blk) {
			retval = EXT2_ET_MISSING_INODE_ಕೋಷ್ಟಕ;
			ಹೋಗು errout;
		}
		ಎಡ = fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು;
		ಆಗ (ಎಡ) {
			c = BUF_ಖಂಡಗಳು;
			ಆದರೆ (c > ಎಡ)
				c = ಎಡ;
			actual = ಪಠ(fd, buf, fs->blocksize * c);
			ಆದರೆ (actual == -1) {
				retval = errno;
				ಹೋಗು errout;
			}
			ಆದರೆ (actual != fs->blocksize * c) {
				retval = EXT2_ET_SHORT_READ;
				ಹೋಗು errout;
			}
			retval = io_channel_write_blk64(fs->io, blk, c, buf);
			ಆದರೆ (retval)
				ಹೋಗು errout;

			blk += c;
			ಎಡ -= c;
		}
	}
	retval = ext2fs_flush_icache(fs);

errout:
	ಬಿಡುಗಡೆ(buf);
	ಹಿಂತಿರುಗಿ retval;
}

/*
 * Write out superblock and group descriptors
 */
errcode_t ext2fs_image_super_write(ext2_filsys fs, ಪೂರ್ಣಾಂಕ fd,
				   ಪೂರ್ಣಾಂಕ flags EXT2FS_ATTR((ಬಳಸದ)))
{
	ಅಕ್ಷರ		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	groups_ಪ್ರತಿ_ಖಂಡ;
	ರಚನೆ		ext2_ಗುಂಪು_desc *gdp;
	ಪೂರ್ಣಾಂಕ		j;
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (fs->ಗುಂಪು_desc == ಖಾಲಿ)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_GDESC;

	buf = ಸ್ಮರಣೆ_ಕೊಡು(fs->blocksize);
	ಆದರೆ (!buf)
		ಹಿಂತಿರುಗಿ ENOMEM;

	/*
	 * Write out the superblock
	 */
	memset(buf, 0, fs->blocksize);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	/*
	 * We're writing out superblock so let's convert
	 * it to little endian and then back if needed
	 */
	ext2fs_ಅದಲುಬದಲು_super(fs->super);
	ಸ್ಮರಣೆ_ನಕಲು(buf, fs->super, SUPERBLOCK_ಗಾತ್ರ);
	ext2fs_ಅದಲುಬದಲು_super(fs->super);
#ಇಲ್ಲದಿದ್ದರೆ
	ಸ್ಮರಣೆ_ನಕಲು(buf, fs->super, SUPERBLOCK_ಗಾತ್ರ);
#ಆದರೆ_ಅಂತ್ಯ
	actual = ಲಿಖ(fd, buf, fs->blocksize);
	ಆದರೆ (actual == -1) {
		retval = errno;
		ಹೋಗು errout;
	}
	ಆದರೆ (actual != (ssize_t) fs->blocksize) {
		retval = EXT2_ET_SHORT_WRITE;
		ಹೋಗು errout;
	}

	/*
	 * Now write out the block group descriptors
	 */

	cp = (ಅಕ್ಷರ *) fs->ಗುಂಪು_desc;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_ಪ್ರತಿ_ಖಂಡ = EXT2_DESC_ಪ್ರತಿ_ಖಂಡ(fs->super);
	ಗಾಗಿ (j=0; j < groups_ಪ್ರತಿ_ಖಂಡ*fs->desc_ಖಂಡಗಳು; j++) {
		gdp = ext2fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, j);
		ಆದರೆ (gdp)
			ext2fs_ಅದಲುಬದಲು_ಗುಂಪು_desc2(fs, gdp);
	}
#ಆದರೆ_ಅಂತ್ಯ

	actual = ಲಿಖ(fd, cp, (ssize_t)fs->blocksize * fs->desc_ಖಂಡಗಳು);


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	groups_ಪ್ರತಿ_ಖಂಡ = EXT2_DESC_ಪ್ರತಿ_ಖಂಡ(fs->super);
	ಗಾಗಿ (j=0; j < groups_ಪ್ರತಿ_ಖಂಡ*fs->desc_ಖಂಡಗಳು; j++) {
		gdp = ext2fs_ಗುಂಪು_desc(fs, fs->ಗುಂಪು_desc, j);
		ಆದರೆ (gdp)
			ext2fs_ಅದಲುಬದಲು_ಗುಂಪು_desc2(fs, gdp);
	}
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (actual == -1) {
		retval = errno;
		ಹೋಗು errout;
	}
	ಆದರೆ (actual != (ssize_t)(fs->blocksize * fs->desc_ಖಂಡಗಳು)) {
		retval = EXT2_ET_SHORT_WRITE;
		ಹೋಗು errout;
	}

	retval = 0;

errout:
	ಬಿಡುಗಡೆ(buf);
	ಹಿಂತಿರುಗಿ retval;
}

/*
 * Read the superblock and group descriptors and overwrite them.
 */
errcode_t ext2fs_image_super_read(ext2_filsys fs, ಪೂರ್ಣಾಂಕ fd,
				  ಪೂರ್ಣಾಂಕ flags EXT2FS_ATTR((ಬಳಸದ)))
{
	ಅಕ್ಷರ		*buf;
	ssize_t		actual, ಗಾತ್ರ_2;
	errcode_t	retval;

	ಗಾತ್ರ_2 = (ssize_t)fs->blocksize * (fs->desc_ಖಂಡಗಳು + 1);
	buf = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ_2);
	ಆದರೆ (!buf)
		ಹಿಂತಿರುಗಿ ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = ಪಠ(fd, buf, ಗಾತ್ರ_2);
	ಆದರೆ (actual == -1) {
		retval = errno;
		ಹೋಗು errout;
	}
	ಆದರೆ (actual != ಗಾತ್ರ_2) {
		retval = EXT2_ET_SHORT_READ;
		ಹೋಗು errout;
	}

	/*
	 * Now copy in the superblock and group descriptors
	 */
	ಸ್ಮರಣೆ_ನಕಲು(fs->super, buf, SUPERBLOCK_ಗಾತ್ರ);

	ಸ್ಮರಣೆ_ನಕಲು(fs->ಗುಂಪು_desc, buf + fs->blocksize,
	       (ssize_t)fs->blocksize * fs->desc_ಖಂಡಗಳು);

	retval = 0;

errout:
	ಬಿಡುಗಡೆ(buf);
	ಹಿಂತಿರುಗಿ retval;
}

/*
 * Write the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, ಪೂರ್ಣಾಂಕ fd, ಪೂರ್ಣಾಂಕ flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	ssize_t			actual;
	size_t			c;
	__u64			itr, cnt, ಗಾತ್ರ_2, ಒಟ್ಟು_ಗಾತ್ರ;
	ಅಕ್ಷರ			buf[1024];

	ಆದರೆ (flags & IMAGER_FLAG_INODEMAP) {
		ಆದರೆ (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			ಆದರೆ (retval)
				ಹಿಂತಿರುಗಿ retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(fs->super) *
			fs->ಗುಂಪು_desc_ಎಣಿಕೆ;
		ಗಾತ್ರ_2 = (EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(fs->super) / 8);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (!fs->ಖಂಡ_map) {
			retval = ext2fs_read_ಖಂಡ_bitmap(fs);
			ಆದರೆ (retval)
				ಹಿಂತಿರುಗಿ retval;
		}
		bmap = fs->ಖಂಡ_map;
		itr = fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ;
		cnt = EXT2_GROUPS_ಗಮ್ಯ_CLUSTERS(fs->super, fs->ಗುಂಪು_desc_ಎಣಿಕೆ);
		ಗಾತ್ರ_2 = EXT2_CLUSTERS_ಪ್ರತಿ_ಗುಂಪು(fs->super) / 8;
	}
	ಒಟ್ಟು_ಗಾತ್ರ = ಗಾತ್ರ_2 * fs->ಗುಂಪು_desc_ಎಣಿಕೆ;

	ಆಗ (cnt > 0) {
		ಗಾತ್ರ_2 = ಗಾತ್ರ(buf);
		ಆದರೆ (ಗಾತ್ರ_2 > (cnt >> 3))
			ಗಾತ್ರ_2 = (cnt >> 3);
		ಆದರೆ (ಗಾತ್ರ_2 == 0)
			ನಿಲ್ಲಿಸು;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       ಗಾತ್ರ_2 << 3, buf);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

		actual = ಲಿಖ(fd, buf, ಗಾತ್ರ_2);
		ಆದರೆ (actual == -1)
			ಹಿಂತಿರುಗಿ errno;
		ಆದರೆ (actual != (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ_2)
			ಹಿಂತಿರುಗಿ EXT2_ET_SHORT_READ;

		itr += ಗಾತ್ರ_2 << 3;
		cnt -= ಗಾತ್ರ_2 << 3;
	}

	ಗಾತ್ರ_2 = ಒಟ್ಟು_ಗಾತ್ರ % fs->blocksize;
	memset(buf, 0, ಗಾತ್ರ(buf));
	ಆದರೆ (ಗಾತ್ರ_2) {
		ಗಾತ್ರ_2 = fs->blocksize - ಗಾತ್ರ_2;
		ಆಗ (ಗಾತ್ರ_2) {
			c = ಗಾತ್ರ_2;
			ಆದರೆ (c > (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ(buf))
				c = ಗಾತ್ರ(buf);
			actual = ಲಿಖ(fd, buf, c);
			ಆದರೆ (actual < 0)
				ಹಿಂತಿರುಗಿ errno;
			ಆದರೆ ((size_t) actual != c)
				ಹಿಂತಿರುಗಿ EXT2_ET_SHORT_WRITE;
			ಗಾತ್ರ_2 -= c;
		}
	}
	ಹಿಂತಿರುಗಿ 0;
}


/*
 * Read the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, ಪೂರ್ಣಾಂಕ fd, ಪೂರ್ಣಾಂಕ flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	__u64			itr, cnt;
	ಅಕ್ಷರ			buf[1024];
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		ಗಾತ್ರ_2;
	ssize_t			actual;

	ಆದರೆ (flags & IMAGER_FLAG_INODEMAP) {
		ಆದರೆ (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			ಆದರೆ (retval)
				ಹಿಂತಿರುಗಿ retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(fs->super) *
			fs->ಗುಂಪು_desc_ಎಣಿಕೆ;
		ಗಾತ್ರ_2 = (EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(fs->super) / 8);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (!fs->ಖಂಡ_map) {
			retval = ext2fs_read_ಖಂಡ_bitmap(fs);
			ಆದರೆ (retval)
				ಹಿಂತಿರುಗಿ retval;
		}
		bmap = fs->ಖಂಡ_map;
		itr = fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ;
		cnt = EXT2_GROUPS_ಗಮ್ಯ_ಖಂಡಗಳು(fs->super, fs->ಗುಂಪು_desc_ಎಣಿಕೆ);
		ಗಾತ್ರ_2 = EXT2_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು(fs->super) / 8;
	}

	ಆಗ (cnt > 0) {
		ಗಾತ್ರ_2 = ಗಾತ್ರ(buf);
		ಆದರೆ (ಗಾತ್ರ_2 > (cnt >> 3))
			ಗಾತ್ರ_2 = (cnt >> 3);
		ಆದರೆ (ಗಾತ್ರ_2 == 0)
			ನಿಲ್ಲಿಸು;

		actual = ಪಠ(fd, buf, ಗಾತ್ರ_2);
		ಆದರೆ (actual == -1)
			ಹಿಂತಿರುಗಿ errno;
		ಆದರೆ (actual != (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ_2)
			ಹಿಂತಿರುಗಿ EXT2_ET_SHORT_READ;

		retval = ext2fs_ಗಣ_generic_bmap_range(bmap, itr,
						       ಗಾತ್ರ_2 << 3, buf);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

		itr += ಗಾತ್ರ_2 << 3;
		cnt -= ಗಾತ್ರ_2 << 3;
	}
	ಹಿಂತಿರುಗಿ 0;
}
