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
#جيڪڏهن_پورو
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جيڪڏهن_پورو
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#جيڪڏهن_وصف_نه_ٿيل HAVE_TYPE_SSIZE_T
تعريفنوع صحيح ssize_t;
#جيڪڏهن_پورو

/*
 * This function returns 1 if the specified block is all zeros
 */
ساكن صحيح check_zero_بلاڪ(حرف *buf, صحيح blocksize)
{
	حرف	*cp = buf;
	صحيح	کاٻو = blocksize;

	طالما (کاٻو > 0) {
		إذا (*cp++)
			أرجع 0;
		کاٻو--;
	}
	أرجع 1;
}

/*
 * Write the inode table out as a single block.
 */
#وصف_ڪريو BUF_بلاڪون	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, صحيح fd, صحيح flags)
{
	dgrp_t		گروهه;
	ssize_t		کاٻو, c, d;
	حرف		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = يادگيري_ڏيو(fs->blocksize * BUF_بلاڪون);
	إذا (!buf)
		أرجع ENOMEM;

	لكل (گروهه = 0; گروهه < fs->گروهه_desc_ڳڻپ; گروهه++) {
		blk = ext2fs_inode_جدول_loc(fs, گروهه);
		إذا (!blk) {
			retval = EXT2_ET_MISSING_INODE_جدول;
			اذهب errout;
		}
		کاٻو = fs->inode_بلاڪون_في_گروهه;
		إذا ((blk < fs->super->s_پهريون_معلومات_بلاڪ) ||
		    (blk + کاٻو - 1 >= ext2fs_بلاڪون_ڳڻپ(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_جدول;
			اذهب errout;
		}
		طالما (کاٻو) {
			c = BUF_بلاڪون;
			إذا (c > کاٻو)
				c = کاٻو;
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
				إذا (check_zero_بلاڪ(cp, fs->blocksize)) {
					c--;
					blk++;
					کاٻو--;
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
					إذا (check_zero_بلاڪ(cp +
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
				کاٻو -= d;
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
				  صحيح flags EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	dgrp_t		گروهه;
	ssize_t		c, کاٻو;
	حرف		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = يادگيري_ڏيو(fs->blocksize * BUF_بلاڪون);
	إذا (!buf)
		أرجع ENOMEM;

	لكل (گروهه = 0; گروهه < fs->گروهه_desc_ڳڻپ; گروهه++) {
		blk = ext2fs_inode_جدول_loc(fs, گروهه);
		إذا (!blk) {
			retval = EXT2_ET_MISSING_INODE_جدول;
			اذهب errout;
		}
		کاٻو = fs->inode_بلاڪون_في_گروهه;
		طالما (کاٻو) {
			c = BUF_بلاڪون;
			إذا (c > کاٻو)
				c = کاٻو;
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
			کاٻو -= c;
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
				   صحيح flags EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	حرف		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	بنانشان صحيح	groups_في_بلاڪ;
	بنية		ext2_گروهه_desc *gdp;
	صحيح		j;
#جيڪڏهن_پورو

	إذا (fs->گروهه_desc == عدم)
		أرجع EXT2_ET_NO_GDESC;

	buf = يادگيري_ڏيو(fs->blocksize);
	إذا (!buf)
		أرجع ENOMEM;

	/*
	 * Write out the superblock
	 */
	memset(buf, 0, fs->blocksize);
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	/*
	 * We're writing out superblock so let's convert
	 * it to little endian and then back if needed
	 */
	ext2fs_مٽا_سٽا_super(fs->super);
	يادگيري_نقل(buf, fs->super, SUPERBLOCK_سائيز);
	ext2fs_مٽا_سٽا_super(fs->super);
#وإلا
	يادگيري_نقل(buf, fs->super, SUPERBLOCK_سائيز);
#جيڪڏهن_پورو
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

	cp = (حرف *) fs->گروهه_desc;

#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_في_بلاڪ = EXT2_DESC_في_بلاڪ(fs->super);
	لكل (j=0; j < groups_في_بلاڪ*fs->desc_بلاڪون; j++) {
		gdp = ext2fs_گروهه_desc(fs, fs->گروهه_desc, j);
		إذا (gdp)
			ext2fs_مٽا_سٽا_گروهه_desc2(fs, gdp);
	}
#جيڪڏهن_پورو

	actual = اكتب(fd, cp, (ssize_t)fs->blocksize * fs->desc_بلاڪون);


#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	groups_في_بلاڪ = EXT2_DESC_في_بلاڪ(fs->super);
	لكل (j=0; j < groups_في_بلاڪ*fs->desc_بلاڪون; j++) {
		gdp = ext2fs_گروهه_desc(fs, fs->گروهه_desc, j);
		إذا (gdp)
			ext2fs_مٽا_سٽا_گروهه_desc2(fs, gdp);
	}
#جيڪڏهن_پورو

	إذا (actual == -1) {
		retval = errno;
		اذهب errout;
	}
	إذا (actual != (ssize_t)(fs->blocksize * fs->desc_بلاڪون)) {
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
				  صحيح flags EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
	حرف		*buf;
	ssize_t		actual, سائيز;
	errcode_t	retval;

	سائيز = (ssize_t)fs->blocksize * (fs->desc_بلاڪون + 1);
	buf = يادگيري_ڏيو(سائيز);
	إذا (!buf)
		أرجع ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = اقرأ(fd, buf, سائيز);
	إذا (actual == -1) {
		retval = errno;
		اذهب errout;
	}
	إذا (actual != سائيز) {
		retval = EXT2_ET_SHORT_READ;
		اذهب errout;
	}

	/*
	 * Now copy in the superblock and group descriptors
	 */
	يادگيري_نقل(fs->super, buf, SUPERBLOCK_سائيز);

	يادگيري_نقل(fs->گروهه_desc, buf + fs->blocksize,
	       (ssize_t)fs->blocksize * fs->desc_بلاڪون);

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
	__u64			itr, cnt, سائيز, ڪل_سائيز;
	حرف			buf[1024];

	إذا (flags & IMAGER_FLAG_INODEMAP) {
		إذا (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			إذا (retval)
				أرجع retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_في_گروهه(fs->super) *
			fs->گروهه_desc_ڳڻپ;
		سائيز = (EXT2_INODES_في_گروهه(fs->super) / 8);
	} وإلا {
		إذا (!fs->بلاڪ_map) {
			retval = ext2fs_read_بلاڪ_bitmap(fs);
			إذا (retval)
				أرجع retval;
		}
		bmap = fs->بلاڪ_map;
		itr = fs->super->s_پهريون_معلومات_بلاڪ;
		cnt = EXT2_GROUPS_منزل_CLUSTERS(fs->super, fs->گروهه_desc_ڳڻپ);
		سائيز = EXT2_CLUSTERS_في_گروهه(fs->super) / 8;
	}
	ڪل_سائيز = سائيز * fs->گروهه_desc_ڳڻپ;

	طالما (cnt > 0) {
		سائيز = حجم(buf);
		إذا (سائيز > (cnt >> 3))
			سائيز = (cnt >> 3);
		إذا (سائيز == 0)
			اكسر;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       سائيز << 3, buf);
		إذا (retval)
			أرجع retval;

		actual = اكتب(fd, buf, سائيز);
		إذا (actual == -1)
			أرجع errno;
		إذا (actual != (صحيح) سائيز)
			أرجع EXT2_ET_SHORT_READ;

		itr += سائيز << 3;
		cnt -= سائيز << 3;
	}

	سائيز = ڪل_سائيز % fs->blocksize;
	memset(buf, 0, حجم(buf));
	إذا (سائيز) {
		سائيز = fs->blocksize - سائيز;
		طالما (سائيز) {
			c = سائيز;
			إذا (c > (صحيح) حجم(buf))
				c = حجم(buf);
			actual = اكتب(fd, buf, c);
			إذا (actual < 0)
				أرجع errno;
			إذا ((size_t) actual != c)
				أرجع EXT2_ET_SHORT_WRITE;
			سائيز -= c;
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
	بنانشان صحيح		سائيز;
	ssize_t			actual;

	إذا (flags & IMAGER_FLAG_INODEMAP) {
		إذا (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			إذا (retval)
				أرجع retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_في_گروهه(fs->super) *
			fs->گروهه_desc_ڳڻپ;
		سائيز = (EXT2_INODES_في_گروهه(fs->super) / 8);
	} وإلا {
		إذا (!fs->بلاڪ_map) {
			retval = ext2fs_read_بلاڪ_bitmap(fs);
			إذا (retval)
				أرجع retval;
		}
		bmap = fs->بلاڪ_map;
		itr = fs->super->s_پهريون_معلومات_بلاڪ;
		cnt = EXT2_GROUPS_منزل_بلاڪون(fs->super, fs->گروهه_desc_ڳڻپ);
		سائيز = EXT2_بلاڪون_في_گروهه(fs->super) / 8;
	}

	طالما (cnt > 0) {
		سائيز = حجم(buf);
		إذا (سائيز > (cnt >> 3))
			سائيز = (cnt >> 3);
		إذا (سائيز == 0)
			اكسر;

		actual = اقرأ(fd, buf, سائيز);
		إذا (actual == -1)
			أرجع errno;
		إذا (actual != (صحيح) سائيز)
			أرجع EXT2_ET_SHORT_READ;

		retval = ext2fs_سيٽ_generic_bmap_range(bmap, itr,
						       سائيز << 3, buf);
		إذا (retval)
			أرجع retval;

		itr += سائيز << 3;
		cnt -= سائيز << 3;
	}
	أرجع 0;
}
