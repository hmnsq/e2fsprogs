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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_ERRNO_H
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_TYPE_SSIZE_T
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ ssize_t;
#ਜੇ_ਅੰਤ

/*
 * This function returns 1 if the specified block is all zeros
 */
ਸਥਿਰ ਪੂਰਨ check_zero_ਖੰਡ(ਅੱਖਰ *buf, ਪੂਰਨ blocksize)
{
	ਅੱਖਰ	*cp = buf;
	ਪੂਰਨ	ਖੱਬਾ = blocksize;

	ਜਦੋਂ (ਖੱਬਾ > 0) {
		ਜੇ (*cp++)
			ਵਾਪਸ 0;
		ਖੱਬਾ--;
	}
	ਵਾਪਸ 1;
}

/*
 * Write the inode table out as a single block.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BUF_ਖੰਡਾਂ	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, ਪੂਰਨ fd, ਪੂਰਨ flags)
{
	dgrp_t		ਸਮੂਹ;
	ssize_t		ਖੱਬਾ, c, d;
	ਅੱਖਰ		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = ਸਮ੍ਰਿਤੀ_ਦਿਓ(fs->blocksize * BUF_ਖੰਡਾਂ);
	ਜੇ (!buf)
		ਵਾਪਸ ENOMEM;

	ਲਈ (ਸਮੂਹ = 0; ਸਮੂਹ < fs->ਸਮੂਹ_desc_ਗਿਣਤੀ; ਸਮੂਹ++) {
		blk = ext2fs_inode_ਸਾਰਣੀ_loc(fs, ਸਮੂਹ);
		ਜੇ (!blk) {
			retval = EXT2_ET_MISSING_INODE_ਸਾਰਣੀ;
			ਜਾਓ errout;
		}
		ਖੱਬਾ = fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ;
		ਜੇ ((blk < fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ) ||
		    (blk + ਖੱਬਾ - 1 >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_ਸਾਰਣੀ;
			ਜਾਓ errout;
		}
		ਜਦੋਂ (ਖੱਬਾ) {
			c = BUF_ਖੰਡਾਂ;
			ਜੇ (c > ਖੱਬਾ)
				c = ਖੱਬਾ;
			retval = io_channel_read_blk64(fs->io, blk, c, buf);
			ਜੇ (retval)
				ਜਾਓ errout;
			cp = buf;
			ਜਦੋਂ (c) {
				ਜੇ (!(flags & IMAGER_FLAG_SPARSEWRITE)) {
					d = c;
					ਜਾਓ skip_sparse;
				}
				/* Skip zero blocks */
				ਜੇ (check_zero_ਖੰਡ(cp, fs->blocksize)) {
					c--;
					blk++;
					ਖੱਬਾ--;
					cp += fs->blocksize;
					r = ext2fs_llseek(fd, fs->blocksize,
							  SEEK_CUR);
					ਜੇ (r < 0) {
						retval = errno;
						ਜਾਓ errout;
					}
					ਜਾਰੀ;
				}
				/* Find non-zero blocks */
				ਲਈ (d = 1; d < c; d++) {
					ਜੇ (check_zero_ਖੰਡ(cp +
							     d * fs->blocksize,
							     fs->blocksize))
						ਰੋਕੋ;
				}
			skip_sparse:
				actual = ਲਿਖ(fd, cp, d * fs->blocksize);
				ਜੇ (actual == -1) {
					retval = errno;
					ਜਾਓ errout;
				}
				ਜੇ (actual != d * fs->blocksize) {
					retval = EXT2_ET_SHORT_WRITE;
					ਜਾਓ errout;
				}
				blk += d;
				ਖੱਬਾ -= d;
				cp += d * fs->blocksize;
				c -= d;
			}
		}
	}
	retval = 0;

errout:
	ਮੁਕਤ(buf);
	ਵਾਪਸ retval;
}

/*
 * Read in the inode table and stuff it into place
 */
errcode_t ext2fs_image_inode_read(ext2_filsys fs, ਪੂਰਨ fd,
				  ਪੂਰਨ flags EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	dgrp_t		ਸਮੂਹ;
	ssize_t		c, ਖੱਬਾ;
	ਅੱਖਰ		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = ਸਮ੍ਰਿਤੀ_ਦਿਓ(fs->blocksize * BUF_ਖੰਡਾਂ);
	ਜੇ (!buf)
		ਵਾਪਸ ENOMEM;

	ਲਈ (ਸਮੂਹ = 0; ਸਮੂਹ < fs->ਸਮੂਹ_desc_ਗਿਣਤੀ; ਸਮੂਹ++) {
		blk = ext2fs_inode_ਸਾਰਣੀ_loc(fs, ਸਮੂਹ);
		ਜੇ (!blk) {
			retval = EXT2_ET_MISSING_INODE_ਸਾਰਣੀ;
			ਜਾਓ errout;
		}
		ਖੱਬਾ = fs->inode_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ;
		ਜਦੋਂ (ਖੱਬਾ) {
			c = BUF_ਖੰਡਾਂ;
			ਜੇ (c > ਖੱਬਾ)
				c = ਖੱਬਾ;
			actual = ਪਠ(fd, buf, fs->blocksize * c);
			ਜੇ (actual == -1) {
				retval = errno;
				ਜਾਓ errout;
			}
			ਜੇ (actual != fs->blocksize * c) {
				retval = EXT2_ET_SHORT_READ;
				ਜਾਓ errout;
			}
			retval = io_channel_write_blk64(fs->io, blk, c, buf);
			ਜੇ (retval)
				ਜਾਓ errout;

			blk += c;
			ਖੱਬਾ -= c;
		}
	}
	retval = ext2fs_flush_icache(fs);

errout:
	ਮੁਕਤ(buf);
	ਵਾਪਸ retval;
}

/*
 * Write out superblock and group descriptors
 */
errcode_t ext2fs_image_super_write(ext2_filsys fs, ਪੂਰਨ fd,
				   ਪੂਰਨ flags EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਅੱਖਰ		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	groups_ਪ੍ਰਤੀ_ਖੰਡ;
	ਬਣਤਰ		ext2_ਸਮੂਹ_desc *gdp;
	ਪੂਰਨ		j;
#ਜੇ_ਅੰਤ

	ਜੇ (fs->ਸਮੂਹ_desc == ਨਿਰਾ)
		ਵਾਪਸ EXT2_ET_NO_GDESC;

	buf = ਸਮ੍ਰਿਤੀ_ਦਿਓ(fs->blocksize);
	ਜੇ (!buf)
		ਵਾਪਸ ENOMEM;

	/*
	 * Write out the superblock
	 */
	memset(buf, 0, fs->blocksize);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	/*
	 * We're writing out superblock so let's convert
	 * it to little endian and then back if needed
	 */
	ext2fs_ਅਦਲਾ_ਬਦਲੀ_super(fs->super);
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf, fs->super, SUPERBLOCK_ਮਾਪ);
	ext2fs_ਅਦਲਾ_ਬਦਲੀ_super(fs->super);
#ਨਹੀਂਤਾਂ
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf, fs->super, SUPERBLOCK_ਮਾਪ);
#ਜੇ_ਅੰਤ
	actual = ਲਿਖ(fd, buf, fs->blocksize);
	ਜੇ (actual == -1) {
		retval = errno;
		ਜਾਓ errout;
	}
	ਜੇ (actual != (ssize_t) fs->blocksize) {
		retval = EXT2_ET_SHORT_WRITE;
		ਜਾਓ errout;
	}

	/*
	 * Now write out the block group descriptors
	 */

	cp = (ਅੱਖਰ *) fs->ਸਮੂਹ_desc;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_ਪ੍ਰਤੀ_ਖੰਡ = EXT2_DESC_ਪ੍ਰਤੀ_ਖੰਡ(fs->super);
	ਲਈ (j=0; j < groups_ਪ੍ਰਤੀ_ਖੰਡ*fs->desc_ਖੰਡਾਂ; j++) {
		gdp = ext2fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, j);
		ਜੇ (gdp)
			ext2fs_ਅਦਲਾ_ਬਦਲੀ_ਸਮੂਹ_desc2(fs, gdp);
	}
#ਜੇ_ਅੰਤ

	actual = ਲਿਖ(fd, cp, (ssize_t)fs->blocksize * fs->desc_ਖੰਡਾਂ);


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	groups_ਪ੍ਰਤੀ_ਖੰਡ = EXT2_DESC_ਪ੍ਰਤੀ_ਖੰਡ(fs->super);
	ਲਈ (j=0; j < groups_ਪ੍ਰਤੀ_ਖੰਡ*fs->desc_ਖੰਡਾਂ; j++) {
		gdp = ext2fs_ਸਮੂਹ_desc(fs, fs->ਸਮੂਹ_desc, j);
		ਜੇ (gdp)
			ext2fs_ਅਦਲਾ_ਬਦਲੀ_ਸਮੂਹ_desc2(fs, gdp);
	}
#ਜੇ_ਅੰਤ

	ਜੇ (actual == -1) {
		retval = errno;
		ਜਾਓ errout;
	}
	ਜੇ (actual != (ssize_t)(fs->blocksize * fs->desc_ਖੰਡਾਂ)) {
		retval = EXT2_ET_SHORT_WRITE;
		ਜਾਓ errout;
	}

	retval = 0;

errout:
	ਮੁਕਤ(buf);
	ਵਾਪਸ retval;
}

/*
 * Read the superblock and group descriptors and overwrite them.
 */
errcode_t ext2fs_image_super_read(ext2_filsys fs, ਪੂਰਨ fd,
				  ਪੂਰਨ flags EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਅੱਖਰ		*buf;
	ssize_t		actual, ਮਾਪ;
	errcode_t	retval;

	ਮਾਪ = (ssize_t)fs->blocksize * (fs->desc_ਖੰਡਾਂ + 1);
	buf = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਮਾਪ);
	ਜੇ (!buf)
		ਵਾਪਸ ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = ਪਠ(fd, buf, ਮਾਪ);
	ਜੇ (actual == -1) {
		retval = errno;
		ਜਾਓ errout;
	}
	ਜੇ (actual != ਮਾਪ) {
		retval = EXT2_ET_SHORT_READ;
		ਜਾਓ errout;
	}

	/*
	 * Now copy in the superblock and group descriptors
	 */
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(fs->super, buf, SUPERBLOCK_ਮਾਪ);

	ਸਮ੍ਰਿਤੀ_ਨਕਲ(fs->ਸਮੂਹ_desc, buf + fs->blocksize,
	       (ssize_t)fs->blocksize * fs->desc_ਖੰਡਾਂ);

	retval = 0;

errout:
	ਮੁਕਤ(buf);
	ਵਾਪਸ retval;
}

/*
 * Write the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, ਪੂਰਨ fd, ਪੂਰਨ flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	ssize_t			actual;
	size_t			c;
	__u64			itr, cnt, ਮਾਪ, ਕੁੱਲ_ਮਾਪ;
	ਅੱਖਰ			buf[1024];

	ਜੇ (flags & IMAGER_FLAG_INODEMAP) {
		ਜੇ (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			ਜੇ (retval)
				ਵਾਪਸ retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super) *
			fs->ਸਮੂਹ_desc_ਗਿਣਤੀ;
		ਮਾਪ = (EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super) / 8);
	} ਨਹੀਂਤਾਂ {
		ਜੇ (!fs->ਖੰਡ_map) {
			retval = ext2fs_read_ਖੰਡ_bitmap(fs);
			ਜੇ (retval)
				ਵਾਪਸ retval;
		}
		bmap = fs->ਖੰਡ_map;
		itr = fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ;
		cnt = EXT2_GROUPS_ਮੰਜ਼ਿਲ_CLUSTERS(fs->super, fs->ਸਮੂਹ_desc_ਗਿਣਤੀ);
		ਮਾਪ = EXT2_CLUSTERS_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super) / 8;
	}
	ਕੁੱਲ_ਮਾਪ = ਮਾਪ * fs->ਸਮੂਹ_desc_ਗਿਣਤੀ;

	ਜਦੋਂ (cnt > 0) {
		ਮਾਪ = ਆਕਾਰ(buf);
		ਜੇ (ਮਾਪ > (cnt >> 3))
			ਮਾਪ = (cnt >> 3);
		ਜੇ (ਮਾਪ == 0)
			ਰੋਕੋ;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       ਮਾਪ << 3, buf);
		ਜੇ (retval)
			ਵਾਪਸ retval;

		actual = ਲਿਖ(fd, buf, ਮਾਪ);
		ਜੇ (actual == -1)
			ਵਾਪਸ errno;
		ਜੇ (actual != (ਪੂਰਨ) ਮਾਪ)
			ਵਾਪਸ EXT2_ET_SHORT_READ;

		itr += ਮਾਪ << 3;
		cnt -= ਮਾਪ << 3;
	}

	ਮਾਪ = ਕੁੱਲ_ਮਾਪ % fs->blocksize;
	memset(buf, 0, ਆਕਾਰ(buf));
	ਜੇ (ਮਾਪ) {
		ਮਾਪ = fs->blocksize - ਮਾਪ;
		ਜਦੋਂ (ਮਾਪ) {
			c = ਮਾਪ;
			ਜੇ (c > (ਪੂਰਨ) ਆਕਾਰ(buf))
				c = ਆਕਾਰ(buf);
			actual = ਲਿਖ(fd, buf, c);
			ਜੇ (actual < 0)
				ਵਾਪਸ errno;
			ਜੇ ((size_t) actual != c)
				ਵਾਪਸ EXT2_ET_SHORT_WRITE;
			ਮਾਪ -= c;
		}
	}
	ਵਾਪਸ 0;
}


/*
 * Read the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, ਪੂਰਨ fd, ਪੂਰਨ flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	__u64			itr, cnt;
	ਅੱਖਰ			buf[1024];
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		ਮਾਪ;
	ssize_t			actual;

	ਜੇ (flags & IMAGER_FLAG_INODEMAP) {
		ਜੇ (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			ਜੇ (retval)
				ਵਾਪਸ retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super) *
			fs->ਸਮੂਹ_desc_ਗਿਣਤੀ;
		ਮਾਪ = (EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super) / 8);
	} ਨਹੀਂਤਾਂ {
		ਜੇ (!fs->ਖੰਡ_map) {
			retval = ext2fs_read_ਖੰਡ_bitmap(fs);
			ਜੇ (retval)
				ਵਾਪਸ retval;
		}
		bmap = fs->ਖੰਡ_map;
		itr = fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ;
		cnt = EXT2_GROUPS_ਮੰਜ਼ਿਲ_ਖੰਡਾਂ(fs->super, fs->ਸਮੂਹ_desc_ਗਿਣਤੀ);
		ਮਾਪ = EXT2_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super) / 8;
	}

	ਜਦੋਂ (cnt > 0) {
		ਮਾਪ = ਆਕਾਰ(buf);
		ਜੇ (ਮਾਪ > (cnt >> 3))
			ਮਾਪ = (cnt >> 3);
		ਜੇ (ਮਾਪ == 0)
			ਰੋਕੋ;

		actual = ਪਠ(fd, buf, ਮਾਪ);
		ਜੇ (actual == -1)
			ਵਾਪਸ errno;
		ਜੇ (actual != (ਪੂਰਨ) ਮਾਪ)
			ਵਾਪਸ EXT2_ET_SHORT_READ;

		retval = ext2fs_ਸੈੱਟ_generic_bmap_range(bmap, itr,
						       ਮਾਪ << 3, buf);
		ਜੇ (retval)
			ਵਾਪਸ retval;

		itr += ਮਾਪ << 3;
		cnt -= ਮਾਪ << 3;
	}
	ਵਾਪਸ 0;
}
