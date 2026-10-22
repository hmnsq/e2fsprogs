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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جے_انت
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جے_انت
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#جے_بنا_تعریف HAVE_TYPE_SSIZE_T
تعريفنوع صحيح ssize_t;
#جے_انت

/*
 * This function returns 1 if the specified block is all zeros
 */
ساكن صحيح check_zero_بلاک(حرف *buf, صحيح blocksize)
{
	حرف	*cp = buf;
	صحيح	بایاں = blocksize;

	طالما (بایاں > 0) {
		إذا (*cp++)
			أرجع 0;
		بایاں--;
	}
	أرجع 1;
}

/*
 * Write the inode table out as a single block.
 */
#تعریف_کرو BUF_بلاکاں	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, صحيح fd, صحيح flags)
{
	dgrp_t		گروہ;
	ssize_t		بایاں, c, d;
	حرف		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = حافظہ_دیو(fs->blocksize * BUF_بلاکاں);
	إذا (!buf)
		أرجع ENOMEM;

	لكل (گروہ = 0; گروہ < fs->گروہ_desc_گنتی; گروہ++) {
		blk = ext2fs_inode_جدول_loc(fs, گروہ);
		إذا (!blk) {
			retval = EXT2_ET_MISSING_INODE_جدول;
			اذهب errout;
		}
		بایاں = fs->inode_بلاکاں_فی_گروہ;
		إذا ((blk < fs->super->s_پہلا_انکڑے_بلاک) ||
		    (blk + بایاں - 1 >= ext2fs_بلاکاں_گنتی(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_جدول;
			اذهب errout;
		}
		طالما (بایاں) {
			c = BUF_بلاکاں;
			إذا (c > بایاں)
				c = بایاں;
			retval = io_channel_read_blk64(fs->io, blk, c, buf);
			إذا (retval)
				اذهب errout;
			cp = buf;
			طالما (c) {
				إذا (!(flags & IMAGER_FLAG_SPARSEWRITE)) {
					d = c;
					اذهب skip_sparse;
				}
				/* Skip zero blocks */
				إذا (check_zero_بلاک(cp, fs->blocksize)) {
					c--;
					blk++;
					بایاں--;
					cp += fs->blocksize;
					r = ext2fs_llseek(fd, fs->blocksize,
							  SEEK_CUR);
					إذا (r < 0) {
						retval = errno;
						اذهب errout;
					}
					استمر;
				}
				/* Find non-zero blocks */
				لكل (d = 1; d < c; d++) {
					إذا (check_zero_بلاک(cp +
							     d * fs->blocksize,
							     fs->blocksize))
						اكسر;
				}
			skip_sparse:
				actual = اكتب(fd, cp, d * fs->blocksize);
				إذا (actual == -1) {
					retval = errno;
					اذهب errout;
				}
				إذا (actual != d * fs->blocksize) {
					retval = EXT2_ET_SHORT_WRITE;
					اذهب errout;
				}
				blk += d;
				بایاں -= d;
				cp += d * fs->blocksize;
				c -= d;
			}
		}
	}
	retval = 0;

errout:
	حرر(buf);
	أرجع retval;
}

/*
 * Read in the inode table and stuff it into place
 */
errcode_t ext2fs_image_inode_read(ext2_filsys fs, صحيح fd,
				  صحيح flags EXT2FS_ATTR((بن_ورتیا)))
{
	dgrp_t		گروہ;
	ssize_t		c, بایاں;
	حرف		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = حافظہ_دیو(fs->blocksize * BUF_بلاکاں);
	إذا (!buf)
		أرجع ENOMEM;

	لكل (گروہ = 0; گروہ < fs->گروہ_desc_گنتی; گروہ++) {
		blk = ext2fs_inode_جدول_loc(fs, گروہ);
		إذا (!blk) {
			retval = EXT2_ET_MISSING_INODE_جدول;
			اذهب errout;
		}
		بایاں = fs->inode_بلاکاں_فی_گروہ;
		طالما (بایاں) {
			c = BUF_بلاکاں;
			إذا (c > بایاں)
				c = بایاں;
			actual = اقرأ(fd, buf, fs->blocksize * c);
			إذا (actual == -1) {
				retval = errno;
				اذهب errout;
			}
			إذا (actual != fs->blocksize * c) {
				retval = EXT2_ET_SHORT_READ;
				اذهب errout;
			}
			retval = io_channel_write_blk64(fs->io, blk, c, buf);
			إذا (retval)
				اذهب errout;

			blk += c;
			بایاں -= c;
		}
	}
	retval = ext2fs_flush_icache(fs);

errout:
	حرر(buf);
	أرجع retval;
}

/*
 * Write out superblock and group descriptors
 */
errcode_t ext2fs_image_super_write(ext2_filsys fs, صحيح fd,
				   صحيح flags EXT2FS_ATTR((بن_ورتیا)))
{
	حرف		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#جے_تعریف_شدہ الفاظ_BIGENDIAN
	بنانشانی صحيح	groups_فی_بلاک;
	بنية		ext2_گروہ_desc *gdp;
	صحيح		j;
#جے_انت

	إذا (fs->گروہ_desc == عدم)
		أرجع EXT2_ET_NO_GDESC;

	buf = حافظہ_دیو(fs->blocksize);
	إذا (!buf)
		أرجع ENOMEM;

	/*
	 * Write out the superblock
	 */
	memset(buf, 0, fs->blocksize);
#جے_تعریف_شدہ الفاظ_BIGENDIAN
	/*
	 * We're writing out superblock so let's convert
	 * it to little endian and then back if needed
	 */
	ext2fs_وٹاندرا_super(fs->super);
	حافظہ_نقل(buf, fs->super, SUPERBLOCK_ناپ);
	ext2fs_وٹاندرا_super(fs->super);
#وإلا
	حافظہ_نقل(buf, fs->super, SUPERBLOCK_ناپ);
#جے_انت
	actual = اكتب(fd, buf, fs->blocksize);
	إذا (actual == -1) {
		retval = errno;
		اذهب errout;
	}
	إذا (actual != (ssize_t) fs->blocksize) {
		retval = EXT2_ET_SHORT_WRITE;
		اذهب errout;
	}

	/*
	 * Now write out the block group descriptors
	 */

	cp = (حرف *) fs->گروہ_desc;

#جے_تعریف_شدہ الفاظ_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_فی_بلاک = EXT2_DESC_فی_بلاک(fs->super);
	لكل (j=0; j < groups_فی_بلاک*fs->desc_بلاکاں; j++) {
		gdp = ext2fs_گروہ_desc(fs, fs->گروہ_desc, j);
		إذا (gdp)
			ext2fs_وٹاندرا_گروہ_desc2(fs, gdp);
	}
#جے_انت

	actual = اكتب(fd, cp, (ssize_t)fs->blocksize * fs->desc_بلاکاں);


#جے_تعریف_شدہ الفاظ_BIGENDIAN
	groups_فی_بلاک = EXT2_DESC_فی_بلاک(fs->super);
	لكل (j=0; j < groups_فی_بلاک*fs->desc_بلاکاں; j++) {
		gdp = ext2fs_گروہ_desc(fs, fs->گروہ_desc, j);
		إذا (gdp)
			ext2fs_وٹاندرا_گروہ_desc2(fs, gdp);
	}
#جے_انت

	إذا (actual == -1) {
		retval = errno;
		اذهب errout;
	}
	إذا (actual != (ssize_t)(fs->blocksize * fs->desc_بلاکاں)) {
		retval = EXT2_ET_SHORT_WRITE;
		اذهب errout;
	}

	retval = 0;

errout:
	حرر(buf);
	أرجع retval;
}

/*
 * Read the superblock and group descriptors and overwrite them.
 */
errcode_t ext2fs_image_super_read(ext2_filsys fs, صحيح fd,
				  صحيح flags EXT2FS_ATTR((بن_ورتیا)))
{
	حرف		*buf;
	ssize_t		actual, ناپ;
	errcode_t	retval;

	ناپ = (ssize_t)fs->blocksize * (fs->desc_بلاکاں + 1);
	buf = حافظہ_دیو(ناپ);
	إذا (!buf)
		أرجع ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = اقرأ(fd, buf, ناپ);
	إذا (actual == -1) {
		retval = errno;
		اذهب errout;
	}
	إذا (actual != ناپ) {
		retval = EXT2_ET_SHORT_READ;
		اذهب errout;
	}

	/*
	 * Now copy in the superblock and group descriptors
	 */
	حافظہ_نقل(fs->super, buf, SUPERBLOCK_ناپ);

	حافظہ_نقل(fs->گروہ_desc, buf + fs->blocksize,
	       (ssize_t)fs->blocksize * fs->desc_بلاکاں);

	retval = 0;

errout:
	حرر(buf);
	أرجع retval;
}

/*
 * Write the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, صحيح fd, صحيح flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	ssize_t			actual;
	size_t			c;
	__u64			itr, cnt, ناپ, کل_ناپ;
	حرف			buf[1024];

	إذا (flags & IMAGER_FLAG_INODEMAP) {
		إذا (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			إذا (retval)
				أرجع retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_فی_گروہ(fs->super) *
			fs->گروہ_desc_گنتی;
		ناپ = (EXT2_INODES_فی_گروہ(fs->super) / 8);
	} وإلا {
		إذا (!fs->بلاک_map) {
			retval = ext2fs_read_بلاک_bitmap(fs);
			إذا (retval)
				أرجع retval;
		}
		bmap = fs->بلاک_map;
		itr = fs->super->s_پہلا_انکڑے_بلاک;
		cnt = EXT2_GROUPS_منزل_CLUSTERS(fs->super, fs->گروہ_desc_گنتی);
		ناپ = EXT2_CLUSTERS_فی_گروہ(fs->super) / 8;
	}
	کل_ناپ = ناپ * fs->گروہ_desc_گنتی;

	طالما (cnt > 0) {
		ناپ = حجم(buf);
		إذا (ناپ > (cnt >> 3))
			ناپ = (cnt >> 3);
		إذا (ناپ == 0)
			اكسر;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       ناپ << 3, buf);
		إذا (retval)
			أرجع retval;

		actual = اكتب(fd, buf, ناپ);
		إذا (actual == -1)
			أرجع errno;
		إذا (actual != (صحيح) ناپ)
			أرجع EXT2_ET_SHORT_READ;

		itr += ناپ << 3;
		cnt -= ناپ << 3;
	}

	ناپ = کل_ناپ % fs->blocksize;
	memset(buf, 0, حجم(buf));
	إذا (ناپ) {
		ناپ = fs->blocksize - ناپ;
		طالما (ناپ) {
			c = ناپ;
			إذا (c > (صحيح) حجم(buf))
				c = حجم(buf);
			actual = اكتب(fd, buf, c);
			إذا (actual < 0)
				أرجع errno;
			إذا ((size_t) actual != c)
				أرجع EXT2_ET_SHORT_WRITE;
			ناپ -= c;
		}
	}
	أرجع 0;
}


/*
 * Read the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, صحيح fd, صحيح flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	__u64			itr, cnt;
	حرف			buf[1024];
	بنانشانی صحيح		ناپ;
	ssize_t			actual;

	إذا (flags & IMAGER_FLAG_INODEMAP) {
		إذا (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			إذا (retval)
				أرجع retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_فی_گروہ(fs->super) *
			fs->گروہ_desc_گنتی;
		ناپ = (EXT2_INODES_فی_گروہ(fs->super) / 8);
	} وإلا {
		إذا (!fs->بلاک_map) {
			retval = ext2fs_read_بلاک_bitmap(fs);
			إذا (retval)
				أرجع retval;
		}
		bmap = fs->بلاک_map;
		itr = fs->super->s_پہلا_انکڑے_بلاک;
		cnt = EXT2_GROUPS_منزل_بلاکاں(fs->super, fs->گروہ_desc_گنتی);
		ناپ = EXT2_بلاکاں_فی_گروہ(fs->super) / 8;
	}

	طالما (cnt > 0) {
		ناپ = حجم(buf);
		إذا (ناپ > (cnt >> 3))
			ناپ = (cnt >> 3);
		إذا (ناپ == 0)
			اكسر;

		actual = اقرأ(fd, buf, ناپ);
		إذا (actual == -1)
			أرجع errno;
		إذا (actual != (صحيح) ناپ)
			أرجع EXT2_ET_SHORT_READ;

		retval = ext2fs_سیٹ_generic_bmap_range(bmap, itr,
						       ناپ << 3, buf);
		إذا (retval)
			أرجع retval;

		itr += ناپ << 3;
		cnt -= ناپ << 3;
	}
	أرجع 0;
}
