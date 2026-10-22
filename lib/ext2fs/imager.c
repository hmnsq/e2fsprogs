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
#د_که_پای
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#د_که_پای
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#که_نه_تعریف_شوی HAVE_TYPE_SSIZE_T
تعريفنوع صحيح ssize_t;
#د_که_پای

/*
 * This function returns 1 if the specified block is all zeros
 */
ساكن صحيح check_zero_بلاک(حرف *buf, صحيح blocksize)
{
	حرف	*cp = buf;
	صحيح	کیڼ = blocksize;

	طالما (کیڼ > 0) {
		إذا (*cp++)
			أرجع 0;
		کیڼ--;
	}
	أرجع 1;
}

/*
 * Write the inode table out as a single block.
 */
#تعریف_کړه BUF_بلاکونه	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, صحيح fd, صحيح flags)
{
	dgrp_t		ډله;
	ssize_t		کیڼ, c, d;
	حرف		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = حافظه_ورکړه(fs->blocksize * BUF_بلاکونه);
	إذا (!buf)
		أرجع ENOMEM;

	لكل (ډله = 0; ډله < fs->ډله_desc_شمېر; ډله++) {
		blk = ext2fs_inode_جدول_loc(fs, ډله);
		إذا (!blk) {
			retval = EXT2_ET_MISSING_INODE_جدول;
			اذهب errout;
		}
		کیڼ = fs->inode_بلاکونه_فی_ډله;
		إذا ((blk < fs->super->s_لومړی_معلومات_بلاک) ||
		    (blk + کیڼ - 1 >= ext2fs_بلاکونه_شمېر(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_جدول;
			اذهب errout;
		}
		طالما (کیڼ) {
			c = BUF_بلاکونه;
			إذا (c > کیڼ)
				c = کیڼ;
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
					کیڼ--;
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
				کیڼ -= d;
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
				  صحيح flags EXT2FS_ATTR((ناکارول_شوی)))
{
	dgrp_t		ډله;
	ssize_t		c, کیڼ;
	حرف		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = حافظه_ورکړه(fs->blocksize * BUF_بلاکونه);
	إذا (!buf)
		أرجع ENOMEM;

	لكل (ډله = 0; ډله < fs->ډله_desc_شمېر; ډله++) {
		blk = ext2fs_inode_جدول_loc(fs, ډله);
		إذا (!blk) {
			retval = EXT2_ET_MISSING_INODE_جدول;
			اذهب errout;
		}
		کیڼ = fs->inode_بلاکونه_فی_ډله;
		طالما (کیڼ) {
			c = BUF_بلاکونه;
			إذا (c > کیڼ)
				c = کیڼ;
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
			کیڼ -= c;
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
				   صحيح flags EXT2FS_ATTR((ناکارول_شوی)))
{
	حرف		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#که_تعریف_شوی ټکي_BIGENDIAN
	بینښه صحيح	groups_فی_بلاک;
	بنية		ext2_ډله_desc *gdp;
	صحيح		j;
#د_که_پای

	إذا (fs->ډله_desc == عدم)
		أرجع EXT2_ET_NO_GDESC;

	buf = حافظه_ورکړه(fs->blocksize);
	إذا (!buf)
		أرجع ENOMEM;

	/*
	 * Write out the superblock
	 */
	memset(buf, 0, fs->blocksize);
#که_تعریف_شوی ټکي_BIGENDIAN
	/*
	 * We're writing out superblock so let's convert
	 * it to little endian and then back if needed
	 */
	ext2fs_بدلون_super(fs->super);
	حافظه_نقل(buf, fs->super, SUPERBLOCK_اندازه);
	ext2fs_بدلون_super(fs->super);
#وإلا
	حافظه_نقل(buf, fs->super, SUPERBLOCK_اندازه);
#د_که_پای
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

	cp = (حرف *) fs->ډله_desc;

#که_تعریف_شوی ټکي_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_فی_بلاک = EXT2_DESC_فی_بلاک(fs->super);
	لكل (j=0; j < groups_فی_بلاک*fs->desc_بلاکونه; j++) {
		gdp = ext2fs_ډله_desc(fs, fs->ډله_desc, j);
		إذا (gdp)
			ext2fs_بدلون_ډله_desc2(fs, gdp);
	}
#د_که_پای

	actual = اكتب(fd, cp, (ssize_t)fs->blocksize * fs->desc_بلاکونه);


#که_تعریف_شوی ټکي_BIGENDIAN
	groups_فی_بلاک = EXT2_DESC_فی_بلاک(fs->super);
	لكل (j=0; j < groups_فی_بلاک*fs->desc_بلاکونه; j++) {
		gdp = ext2fs_ډله_desc(fs, fs->ډله_desc, j);
		إذا (gdp)
			ext2fs_بدلون_ډله_desc2(fs, gdp);
	}
#د_که_پای

	إذا (actual == -1) {
		retval = errno;
		اذهب errout;
	}
	إذا (actual != (ssize_t)(fs->blocksize * fs->desc_بلاکونه)) {
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
				  صحيح flags EXT2FS_ATTR((ناکارول_شوی)))
{
	حرف		*buf;
	ssize_t		actual, اندازه;
	errcode_t	retval;

	اندازه = (ssize_t)fs->blocksize * (fs->desc_بلاکونه + 1);
	buf = حافظه_ورکړه(اندازه);
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
	حافظه_نقل(fs->super, buf, SUPERBLOCK_اندازه);

	حافظه_نقل(fs->ډله_desc, buf + fs->blocksize,
	       (ssize_t)fs->blocksize * fs->desc_بلاکونه);

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
	__u64			itr, cnt, اندازه, ټول_اندازه;
	حرف			buf[1024];

	إذا (flags & IMAGER_FLAG_INODEMAP) {
		إذا (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			إذا (retval)
				أرجع retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_فی_ډله(fs->super) *
			fs->ډله_desc_شمېر;
		اندازه = (EXT2_INODES_فی_ډله(fs->super) / 8);
	} وإلا {
		إذا (!fs->بلاک_map) {
			retval = ext2fs_read_بلاک_bitmap(fs);
			إذا (retval)
				أرجع retval;
		}
		bmap = fs->بلاک_map;
		itr = fs->super->s_لومړی_معلومات_بلاک;
		cnt = EXT2_GROUPS_منزل_CLUSTERS(fs->super, fs->ډله_desc_شمېر);
		اندازه = EXT2_CLUSTERS_فی_ډله(fs->super) / 8;
	}
	ټول_اندازه = اندازه * fs->ډله_desc_شمېر;

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

	اندازه = ټول_اندازه % fs->blocksize;
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
	بینښه صحيح		اندازه;
	ssize_t			actual;

	إذا (flags & IMAGER_FLAG_INODEMAP) {
		إذا (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			إذا (retval)
				أرجع retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_فی_ډله(fs->super) *
			fs->ډله_desc_شمېر;
		اندازه = (EXT2_INODES_فی_ډله(fs->super) / 8);
	} وإلا {
		إذا (!fs->بلاک_map) {
			retval = ext2fs_read_بلاک_bitmap(fs);
			إذا (retval)
				أرجع retval;
		}
		bmap = fs->بلاک_map;
		itr = fs->super->s_لومړی_معلومات_بلاک;
		cnt = EXT2_GROUPS_منزل_بلاکونه(fs->super, fs->ډله_desc_شمېر);
		اندازه = EXT2_بلاکونه_فی_ډله(fs->super) / 8;
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

		retval = ext2fs_سېټ_generic_bmap_range(bmap, itr,
						       اندازه << 3, buf);
		إذا (retval)
			أرجع retval;

		itr += اندازه << 3;
		cnt -= اندازه << 3;
	}
	أرجع 0;
}
