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
#پایان_اگر
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#پایان_اگر
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#اگر_تعریف_نشده HAVE_TYPE_SSIZE_T
تعريفنوع صحيح ssize_t;
#پایان_اگر

/*
 * This function returns 1 if the specified block is all zeros
 */
ساكن صحيح check_zero_بلوک(حرف *buf, صحيح blocksize)
{
	حرف	*cp = buf;
	صحيح	چپ = blocksize;

	طالما (چپ > 0) {
		إذا (*cp++)
			أرجع 0;
		چپ--;
	}
	أرجع 1;
}

/*
 * Write the inode table out as a single block.
 */
#تعریف_کن BUF_بلوکها	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, صحيح fd, صحيح flags)
{
	dgrp_t		گروه;
	ssize_t		چپ, c, d;
	حرف		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = حافظه_بده(fs->blocksize * BUF_بلوکها);
	إذا (!buf)
		أرجع ENOMEM;

	لكل (گروه = 0; گروه < fs->گروه_desc_شمار; گروه++) {
		blk = ext2fs_inode_جدول_loc(fs, گروه);
		إذا (!blk) {
			retval = EXT2_ET_MISSING_INODE_جدول;
			اذهب errout;
		}
		چپ = fs->inode_بلوکها_در_هر_گروه;
		إذا ((blk < fs->super->s_اولی_داده_بلوک) ||
		    (blk + چپ - 1 >= ext2fs_بلوکها_شمار(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_جدول;
			اذهب errout;
		}
		طالما (چپ) {
			c = BUF_بلوکها;
			إذا (c > چپ)
				c = چپ;
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
				إذا (check_zero_بلوک(cp, fs->blocksize)) {
					c--;
					blk++;
					چپ--;
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
					إذا (check_zero_بلوک(cp +
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
				چپ -= d;
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
				  صحيح flags EXT2FS_ATTR((استفادهنشده)))
{
	dgrp_t		گروه;
	ssize_t		c, چپ;
	حرف		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = حافظه_بده(fs->blocksize * BUF_بلوکها);
	إذا (!buf)
		أرجع ENOMEM;

	لكل (گروه = 0; گروه < fs->گروه_desc_شمار; گروه++) {
		blk = ext2fs_inode_جدول_loc(fs, گروه);
		إذا (!blk) {
			retval = EXT2_ET_MISSING_INODE_جدول;
			اذهب errout;
		}
		چپ = fs->inode_بلوکها_در_هر_گروه;
		طالما (چپ) {
			c = BUF_بلوکها;
			إذا (c > چپ)
				c = چپ;
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
			چپ -= c;
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
				   صحيح flags EXT2FS_ATTR((استفادهنشده)))
{
	حرف		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#اگر_تعریف_شده واژگان_BIGENDIAN
	بیعلامت صحيح	groups_در_هر_بلوک;
	بنية		ext2_گروه_desc *gdp;
	صحيح		j;
#پایان_اگر

	إذا (fs->گروه_desc == عدم)
		أرجع EXT2_ET_NO_GDESC;

	buf = حافظه_بده(fs->blocksize);
	إذا (!buf)
		أرجع ENOMEM;

	/*
	 * Write out the superblock
	 */
	memset(buf, 0, fs->blocksize);
#اگر_تعریف_شده واژگان_BIGENDIAN
	/*
	 * We're writing out superblock so let's convert
	 * it to little endian and then back if needed
	 */
	ext2fs_جابجا_super(fs->super);
	کپی_حافظه(buf, fs->super, SUPERBLOCK_اندازه);
	ext2fs_جابجا_super(fs->super);
#وإلا
	کپی_حافظه(buf, fs->super, SUPERBLOCK_اندازه);
#پایان_اگر
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

	cp = (حرف *) fs->گروه_desc;

#اگر_تعریف_شده واژگان_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_در_هر_بلوک = EXT2_DESC_در_هر_بلوک(fs->super);
	لكل (j=0; j < groups_در_هر_بلوک*fs->desc_بلوکها; j++) {
		gdp = ext2fs_گروه_desc(fs, fs->گروه_desc, j);
		إذا (gdp)
			ext2fs_جابجا_گروه_desc2(fs, gdp);
	}
#پایان_اگر

	actual = اكتب(fd, cp, (ssize_t)fs->blocksize * fs->desc_بلوکها);


#اگر_تعریف_شده واژگان_BIGENDIAN
	groups_در_هر_بلوک = EXT2_DESC_در_هر_بلوک(fs->super);
	لكل (j=0; j < groups_در_هر_بلوک*fs->desc_بلوکها; j++) {
		gdp = ext2fs_گروه_desc(fs, fs->گروه_desc, j);
		إذا (gdp)
			ext2fs_جابجا_گروه_desc2(fs, gdp);
	}
#پایان_اگر

	إذا (actual == -1) {
		retval = errno;
		اذهب errout;
	}
	إذا (actual != (ssize_t)(fs->blocksize * fs->desc_بلوکها)) {
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
				  صحيح flags EXT2FS_ATTR((استفادهنشده)))
{
	حرف		*buf;
	ssize_t		actual, اندازه;
	errcode_t	retval;

	اندازه = (ssize_t)fs->blocksize * (fs->desc_بلوکها + 1);
	buf = حافظه_بده(اندازه);
	إذا (!buf)
		أرجع ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = اقرأ(fd, buf, اندازه);
	إذا (actual == -1) {
		retval = errno;
		اذهب errout;
	}
	إذا (actual != اندازه) {
		retval = EXT2_ET_SHORT_READ;
		اذهب errout;
	}

	/*
	 * Now copy in the superblock and group descriptors
	 */
	کپی_حافظه(fs->super, buf, SUPERBLOCK_اندازه);

	کپی_حافظه(fs->گروه_desc, buf + fs->blocksize,
	       (ssize_t)fs->blocksize * fs->desc_بلوکها);

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
	__u64			itr, cnt, اندازه, مجموع_اندازه;
	حرف			buf[1024];

	إذا (flags & IMAGER_FLAG_INODEMAP) {
		إذا (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			إذا (retval)
				أرجع retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_در_هر_گروه(fs->super) *
			fs->گروه_desc_شمار;
		اندازه = (EXT2_INODES_در_هر_گروه(fs->super) / 8);
	} وإلا {
		إذا (!fs->بلوک_map) {
			retval = ext2fs_read_بلوک_bitmap(fs);
			إذا (retval)
				أرجع retval;
		}
		bmap = fs->بلوک_map;
		itr = fs->super->s_اولی_داده_بلوک;
		cnt = EXT2_GROUPS_مقصد_CLUSTERS(fs->super, fs->گروه_desc_شمار);
		اندازه = EXT2_CLUSTERS_در_هر_گروه(fs->super) / 8;
	}
	مجموع_اندازه = اندازه * fs->گروه_desc_شمار;

	طالما (cnt > 0) {
		اندازه = حجم(buf);
		إذا (اندازه > (cnt >> 3))
			اندازه = (cnt >> 3);
		إذا (اندازه == 0)
			اكسر;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       اندازه << 3, buf);
		إذا (retval)
			أرجع retval;

		actual = اكتب(fd, buf, اندازه);
		إذا (actual == -1)
			أرجع errno;
		إذا (actual != (صحيح) اندازه)
			أرجع EXT2_ET_SHORT_READ;

		itr += اندازه << 3;
		cnt -= اندازه << 3;
	}

	اندازه = مجموع_اندازه % fs->blocksize;
	memset(buf, 0, حجم(buf));
	إذا (اندازه) {
		اندازه = fs->blocksize - اندازه;
		طالما (اندازه) {
			c = اندازه;
			إذا (c > (صحيح) حجم(buf))
				c = حجم(buf);
			actual = اكتب(fd, buf, c);
			إذا (actual < 0)
				أرجع errno;
			إذا ((size_t) actual != c)
				أرجع EXT2_ET_SHORT_WRITE;
			اندازه -= c;
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
	بیعلامت صحيح		اندازه;
	ssize_t			actual;

	إذا (flags & IMAGER_FLAG_INODEMAP) {
		إذا (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			إذا (retval)
				أرجع retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_در_هر_گروه(fs->super) *
			fs->گروه_desc_شمار;
		اندازه = (EXT2_INODES_در_هر_گروه(fs->super) / 8);
	} وإلا {
		إذا (!fs->بلوک_map) {
			retval = ext2fs_read_بلوک_bitmap(fs);
			إذا (retval)
				أرجع retval;
		}
		bmap = fs->بلوک_map;
		itr = fs->super->s_اولی_داده_بلوک;
		cnt = EXT2_GROUPS_مقصد_بلوکها(fs->super, fs->گروه_desc_شمار);
		اندازه = EXT2_بلوکها_در_هر_گروه(fs->super) / 8;
	}

	طالما (cnt > 0) {
		اندازه = حجم(buf);
		إذا (اندازه > (cnt >> 3))
			اندازه = (cnt >> 3);
		إذا (اندازه == 0)
			اكسر;

		actual = اقرأ(fd, buf, اندازه);
		إذا (actual == -1)
			أرجع errno;
		إذا (actual != (صحيح) اندازه)
			أرجع EXT2_ET_SHORT_READ;

		retval = ext2fs_مجموعه_generic_bmap_range(bmap, itr,
						       اندازه << 3, buf);
		إذا (retval)
			أرجع retval;

		itr += اندازه << 3;
		cnt -= اندازه << 3;
	}
	أرجع 0;
}
