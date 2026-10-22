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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#اگر_ختم
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

#اگر_غیر_تعریف_شدہ HAVE_TYPE_SSIZE_T
قسمتعریف صحیح ssize_t;
#اگر_ختم

/*
 * This function returns 1 if the specified block is all zeros
 */
ساکن صحیح check_zero_بلاک(حرف *buf, صحیح blocksize)
{
	حرف	*cp = buf;
	صحیح	بایاں = blocksize;

	جب (بایاں > 0) {
		اگر (*cp++)
			واپس 0;
		بایاں--;
	}
	واپس 1;
}

/*
 * Write the inode table out as a single block.
 */
#تعریف_کرو BUF_بلاکیں	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, صحیح fd, صحیح flags)
{
	dgrp_t		گروہ;
	ssize_t		بایاں, c, d;
	حرف		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = حافظہ_دو(fs->blocksize * BUF_بلاکیں);
	اگر (!buf)
		واپس ENOMEM;

	برائے (گروہ = 0; گروہ < fs->گروہ_desc_گنتی; گروہ++) {
		blk = ext2fs_inode_جدول_loc(fs, گروہ);
		اگر (!blk) {
			retval = EXT2_ET_MISSING_INODE_جدول;
			جاؤ errout;
		}
		بایاں = fs->inode_بلاکیں_فی_گروہ;
		اگر ((blk < fs->super->s_پہلا_ڈیٹا_بلاک) ||
		    (blk + بایاں - 1 >= ext2fs_بلاکیں_گنتی(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_جدول;
			جاؤ errout;
		}
		جب (بایاں) {
			c = BUF_بلاکیں;
			اگر (c > بایاں)
				c = بایاں;
			retval = io_channel_read_blk64(fs->io, blk, c, buf);
			اگر (retval)
				جاؤ errout;
			cp = buf;
			جب (c) {
				اگر (!(flags & IMAGER_FLAG_SPARSEWRITE)) {
					d = c;
					جاؤ skip_sparse;
				}
				/* Skip zero blocks */
				اگر (check_zero_بلاک(cp, fs->blocksize)) {
					c--;
					blk++;
					بایاں--;
					cp += fs->blocksize;
					r = ext2fs_llseek(fd, fs->blocksize,
							  SEEK_CUR);
					اگر (r < 0) {
						retval = errno;
						جاؤ errout;
					}
					جاری;
				}
				/* Find non-zero blocks */
				برائے (d = 1; d < c; d++) {
					اگر (check_zero_بلاک(cp +
							     d * fs->blocksize,
							     fs->blocksize))
						توڑو;
				}
			skip_sparse:
				actual = اكتب(fd, cp, d * fs->blocksize);
				اگر (actual == -1) {
					retval = errno;
					جاؤ errout;
				}
				اگر (actual != d * fs->blocksize) {
					retval = EXT2_ET_SHORT_WRITE;
					جاؤ errout;
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
	آزاد(buf);
	واپس retval;
}

/*
 * Read in the inode table and stuff it into place
 */
errcode_t ext2fs_image_inode_read(ext2_filsys fs, صحیح fd,
				  صحیح flags EXT2FS_ATTR((غیر_مستعمل)))
{
	dgrp_t		گروہ;
	ssize_t		c, بایاں;
	حرف		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = حافظہ_دو(fs->blocksize * BUF_بلاکیں);
	اگر (!buf)
		واپس ENOMEM;

	برائے (گروہ = 0; گروہ < fs->گروہ_desc_گنتی; گروہ++) {
		blk = ext2fs_inode_جدول_loc(fs, گروہ);
		اگر (!blk) {
			retval = EXT2_ET_MISSING_INODE_جدول;
			جاؤ errout;
		}
		بایاں = fs->inode_بلاکیں_فی_گروہ;
		جب (بایاں) {
			c = BUF_بلاکیں;
			اگر (c > بایاں)
				c = بایاں;
			actual = اقرأ(fd, buf, fs->blocksize * c);
			اگر (actual == -1) {
				retval = errno;
				جاؤ errout;
			}
			اگر (actual != fs->blocksize * c) {
				retval = EXT2_ET_SHORT_READ;
				جاؤ errout;
			}
			retval = io_channel_write_blk64(fs->io, blk, c, buf);
			اگر (retval)
				جاؤ errout;

			blk += c;
			بایاں -= c;
		}
	}
	retval = ext2fs_flush_icache(fs);

errout:
	آزاد(buf);
	واپس retval;
}

/*
 * Write out superblock and group descriptors
 */
errcode_t ext2fs_image_super_write(ext2_filsys fs, صحیح fd,
				   صحیح flags EXT2FS_ATTR((غیر_مستعمل)))
{
	حرف		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	غیرعلامتی صحیح	groups_فی_بلاک;
	ساخت		ext2_گروہ_desc *gdp;
	صحیح		j;
#اگر_ختم

	اگر (fs->گروہ_desc == تہی)
		واپس EXT2_ET_NO_GDESC;

	buf = حافظہ_دو(fs->blocksize);
	اگر (!buf)
		واپس ENOMEM;

	/*
	 * Write out the superblock
	 */
	memset(buf, 0, fs->blocksize);
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	/*
	 * We're writing out superblock so let's convert
	 * it to little endian and then back if needed
	 */
	ext2fs_تبادلہ_super(fs->super);
	حافظہ_نقل(buf, fs->super, SUPERBLOCK_سائز);
	ext2fs_تبادلہ_super(fs->super);
#ورنہ
	حافظہ_نقل(buf, fs->super, SUPERBLOCK_سائز);
#اگر_ختم
	actual = اكتب(fd, buf, fs->blocksize);
	اگر (actual == -1) {
		retval = errno;
		جاؤ errout;
	}
	اگر (actual != (ssize_t) fs->blocksize) {
		retval = EXT2_ET_SHORT_WRITE;
		جاؤ errout;
	}

	/*
	 * Now write out the block group descriptors
	 */

	cp = (حرف *) fs->گروہ_desc;

#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_فی_بلاک = EXT2_DESC_فی_بلاک(fs->super);
	برائے (j=0; j < groups_فی_بلاک*fs->desc_بلاکیں; j++) {
		gdp = ext2fs_گروہ_desc(fs, fs->گروہ_desc, j);
		اگر (gdp)
			ext2fs_تبادلہ_گروہ_desc2(fs, gdp);
	}
#اگر_ختم

	actual = اكتب(fd, cp, (ssize_t)fs->blocksize * fs->desc_بلاکیں);


#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	groups_فی_بلاک = EXT2_DESC_فی_بلاک(fs->super);
	برائے (j=0; j < groups_فی_بلاک*fs->desc_بلاکیں; j++) {
		gdp = ext2fs_گروہ_desc(fs, fs->گروہ_desc, j);
		اگر (gdp)
			ext2fs_تبادلہ_گروہ_desc2(fs, gdp);
	}
#اگر_ختم

	اگر (actual == -1) {
		retval = errno;
		جاؤ errout;
	}
	اگر (actual != (ssize_t)(fs->blocksize * fs->desc_بلاکیں)) {
		retval = EXT2_ET_SHORT_WRITE;
		جاؤ errout;
	}

	retval = 0;

errout:
	آزاد(buf);
	واپس retval;
}

/*
 * Read the superblock and group descriptors and overwrite them.
 */
errcode_t ext2fs_image_super_read(ext2_filsys fs, صحیح fd,
				  صحیح flags EXT2FS_ATTR((غیر_مستعمل)))
{
	حرف		*buf;
	ssize_t		actual, سائز;
	errcode_t	retval;

	سائز = (ssize_t)fs->blocksize * (fs->desc_بلاکیں + 1);
	buf = حافظہ_دو(سائز);
	اگر (!buf)
		واپس ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = اقرأ(fd, buf, سائز);
	اگر (actual == -1) {
		retval = errno;
		جاؤ errout;
	}
	اگر (actual != سائز) {
		retval = EXT2_ET_SHORT_READ;
		جاؤ errout;
	}

	/*
	 * Now copy in the superblock and group descriptors
	 */
	حافظہ_نقل(fs->super, buf, SUPERBLOCK_سائز);

	حافظہ_نقل(fs->گروہ_desc, buf + fs->blocksize,
	       (ssize_t)fs->blocksize * fs->desc_بلاکیں);

	retval = 0;

errout:
	آزاد(buf);
	واپس retval;
}

/*
 * Write the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, صحیح fd, صحیح flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	ssize_t			actual;
	size_t			c;
	__u64			itr, cnt, سائز, کل_سائز;
	حرف			buf[1024];

	اگر (flags & IMAGER_FLAG_INODEMAP) {
		اگر (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			اگر (retval)
				واپس retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_فی_گروہ(fs->super) *
			fs->گروہ_desc_گنتی;
		سائز = (EXT2_INODES_فی_گروہ(fs->super) / 8);
	} ورنہ {
		اگر (!fs->بلاک_map) {
			retval = ext2fs_read_بلاک_bitmap(fs);
			اگر (retval)
				واپس retval;
		}
		bmap = fs->بلاک_map;
		itr = fs->super->s_پہلا_ڈیٹا_بلاک;
		cnt = EXT2_GROUPS_مقصد_CLUSTERS(fs->super, fs->گروہ_desc_گنتی);
		سائز = EXT2_CLUSTERS_فی_گروہ(fs->super) / 8;
	}
	کل_سائز = سائز * fs->گروہ_desc_گنتی;

	جب (cnt > 0) {
		سائز = حجم(buf);
		اگر (سائز > (cnt >> 3))
			سائز = (cnt >> 3);
		اگر (سائز == 0)
			توڑو;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       سائز << 3, buf);
		اگر (retval)
			واپس retval;

		actual = اكتب(fd, buf, سائز);
		اگر (actual == -1)
			واپس errno;
		اگر (actual != (صحیح) سائز)
			واپس EXT2_ET_SHORT_READ;

		itr += سائز << 3;
		cnt -= سائز << 3;
	}

	سائز = کل_سائز % fs->blocksize;
	memset(buf, 0, حجم(buf));
	اگر (سائز) {
		سائز = fs->blocksize - سائز;
		جب (سائز) {
			c = سائز;
			اگر (c > (صحیح) حجم(buf))
				c = حجم(buf);
			actual = اكتب(fd, buf, c);
			اگر (actual < 0)
				واپس errno;
			اگر ((size_t) actual != c)
				واپس EXT2_ET_SHORT_WRITE;
			سائز -= c;
		}
	}
	واپس 0;
}


/*
 * Read the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, صحیح fd, صحیح flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	__u64			itr, cnt;
	حرف			buf[1024];
	غیرعلامتی صحیح		سائز;
	ssize_t			actual;

	اگر (flags & IMAGER_FLAG_INODEMAP) {
		اگر (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			اگر (retval)
				واپس retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_فی_گروہ(fs->super) *
			fs->گروہ_desc_گنتی;
		سائز = (EXT2_INODES_فی_گروہ(fs->super) / 8);
	} ورنہ {
		اگر (!fs->بلاک_map) {
			retval = ext2fs_read_بلاک_bitmap(fs);
			اگر (retval)
				واپس retval;
		}
		bmap = fs->بلاک_map;
		itr = fs->super->s_پہلا_ڈیٹا_بلاک;
		cnt = EXT2_GROUPS_مقصد_بلاکیں(fs->super, fs->گروہ_desc_گنتی);
		سائز = EXT2_بلاکیں_فی_گروہ(fs->super) / 8;
	}

	جب (cnt > 0) {
		سائز = حجم(buf);
		اگر (سائز > (cnt >> 3))
			سائز = (cnt >> 3);
		اگر (سائز == 0)
			توڑو;

		actual = اقرأ(fd, buf, سائز);
		اگر (actual == -1)
			واپس errno;
		اگر (actual != (صحیح) سائز)
			واپس EXT2_ET_SHORT_READ;

		retval = ext2fs_سیٹ_generic_bmap_range(bmap, itr,
						       سائز << 3, buf);
		اگر (retval)
			واپس retval;

		itr += سائز << 3;
		cnt -= سائز << 3;
	}
	واپس 0;
}
