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
#endif
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#endif
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

#ifndef HAVE_TYPE_SSIZE_T
تعریفنوع صحیح ssize_t;
#endif

/*
 * This function returns 1 if the specified block is all zeros
 */
ایستا صحیح check_zero_block(نویسه *buf, صحیح blocksize)
{
	نویسه	*cp = buf;
	صحیح	چپ = blocksize;

	وقتی (چپ > 0) {
		اگر (*cp++)
			بازگشت 0;
		چپ--;
	}
	بازگشت 1;
}

/*
 * Write the inode table out as a single block.
 */
#define BUF_BLOCKS	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, صحیح fd, صحیح flags)
{
	dgrp_t		group;
	ssize_t		چپ, c, d;
	نویسه		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	اگر (!buf)
		بازگشت ENOMEM;

	برای (group = 0; group < fs->group_desc_شمار; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		اگر (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			برو errout;
		}
		چپ = fs->inode_blocks_per_group;
		اگر ((blk < fs->super->s_first_data_block) ||
		    (blk + چپ - 1 >= ext2fs_blocks_شمار(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
			برو errout;
		}
		وقتی (چپ) {
			c = BUF_BLOCKS;
			اگر (c > چپ)
				c = چپ;
			retval = io_channel_read_blk64(fs->io, blk, c, buf);
			اگر (retval)
				برو errout;
			cp = buf;
			وقتی (c) {
				اگر (!(flags & IMAGER_FLAG_SPARSEWRITE)) {
					d = c;
					برو skip_sparse;
				}
				/* Skip zero blocks */
				اگر (check_zero_block(cp, fs->blocksize)) {
					c--;
					blk++;
					چپ--;
					cp += fs->blocksize;
					r = ext2fs_llseek(fd, fs->blocksize,
							  SEEK_CUR);
					اگر (r < 0) {
						retval = errno;
						برو errout;
					}
					ادامه;
				}
				/* Find non-zero blocks */
				برای (d = 1; d < c; d++) {
					اگر (check_zero_block(cp +
							     d * fs->blocksize,
							     fs->blocksize))
						بشکن;
				}
			skip_sparse:
				actual = اكتب(fd, cp, d * fs->blocksize);
				اگر (actual == -1) {
					retval = errno;
					برو errout;
				}
				اگر (actual != d * fs->blocksize) {
					retval = EXT2_ET_SHORT_WRITE;
					برو errout;
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
	آزاد(buf);
	بازگشت retval;
}

/*
 * Read in the inode table and stuff it into place
 */
errcode_t ext2fs_image_inode_read(ext2_filsys fs, صحیح fd,
				  صحیح flags EXT2FS_ATTR((unused)))
{
	dgrp_t		group;
	ssize_t		c, چپ;
	نویسه		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	اگر (!buf)
		بازگشت ENOMEM;

	برای (group = 0; group < fs->group_desc_شمار; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		اگر (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			برو errout;
		}
		چپ = fs->inode_blocks_per_group;
		وقتی (چپ) {
			c = BUF_BLOCKS;
			اگر (c > چپ)
				c = چپ;
			actual = اقرأ(fd, buf, fs->blocksize * c);
			اگر (actual == -1) {
				retval = errno;
				برو errout;
			}
			اگر (actual != fs->blocksize * c) {
				retval = EXT2_ET_SHORT_READ;
				برو errout;
			}
			retval = io_channel_write_blk64(fs->io, blk, c, buf);
			اگر (retval)
				برو errout;

			blk += c;
			چپ -= c;
		}
	}
	retval = ext2fs_flush_icache(fs);

errout:
	آزاد(buf);
	بازگشت retval;
}

/*
 * Write out superblock and group descriptors
 */
errcode_t ext2fs_image_super_write(ext2_filsys fs, صحیح fd,
				   صحیح flags EXT2FS_ATTR((unused)))
{
	نویسه		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#ifdef واژگان_BIGENDIAN
	unsigned صحیح	groups_per_block;
	ساختار		ext2_group_desc *gdp;
	صحیح		j;
#endif

	اگر (fs->group_desc == پوچ)
		بازگشت EXT2_ET_NO_GDESC;

	buf = malloc(fs->blocksize);
	اگر (!buf)
		بازگشت ENOMEM;

	/*
	 * Write out the superblock
	 */
	memset(buf, 0, fs->blocksize);
#ifdef واژگان_BIGENDIAN
	/*
	 * We're writing out superblock so let's convert
	 * it to little endian and then back if needed
	 */
	ext2fs_swap_super(fs->super);
	memcpy(buf, fs->super, SUPERBLOCK_SIZE);
	ext2fs_swap_super(fs->super);
#وگرنه
	memcpy(buf, fs->super, SUPERBLOCK_SIZE);
#endif
	actual = اكتب(fd, buf, fs->blocksize);
	اگر (actual == -1) {
		retval = errno;
		برو errout;
	}
	اگر (actual != (ssize_t) fs->blocksize) {
		retval = EXT2_ET_SHORT_WRITE;
		برو errout;
	}

	/*
	 * Now write out the block group descriptors
	 */

	cp = (نویسه *) fs->group_desc;

#ifdef واژگان_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	برای (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		اگر (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	actual = اكتب(fd, cp, (ssize_t)fs->blocksize * fs->desc_blocks);


#ifdef واژگان_BIGENDIAN
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	برای (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		اگر (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	اگر (actual == -1) {
		retval = errno;
		برو errout;
	}
	اگر (actual != (ssize_t)(fs->blocksize * fs->desc_blocks)) {
		retval = EXT2_ET_SHORT_WRITE;
		برو errout;
	}

	retval = 0;

errout:
	آزاد(buf);
	بازگشت retval;
}

/*
 * Read the superblock and group descriptors and overwrite them.
 */
errcode_t ext2fs_image_super_read(ext2_filsys fs, صحیح fd,
				  صحیح flags EXT2FS_ATTR((unused)))
{
	نویسه		*buf;
	ssize_t		actual, size;
	errcode_t	retval;

	size = (ssize_t)fs->blocksize * (fs->desc_blocks + 1);
	buf = malloc(size);
	اگر (!buf)
		بازگشت ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = اقرأ(fd, buf, size);
	اگر (actual == -1) {
		retval = errno;
		برو errout;
	}
	اگر (actual != size) {
		retval = EXT2_ET_SHORT_READ;
		برو errout;
	}

	/*
	 * Now copy in the superblock and group descriptors
	 */
	memcpy(fs->super, buf, SUPERBLOCK_SIZE);

	memcpy(fs->group_desc, buf + fs->blocksize,
	       (ssize_t)fs->blocksize * fs->desc_blocks);

	retval = 0;

errout:
	آزاد(buf);
	بازگشت retval;
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
	__u64			itr, cnt, size, total_size;
	نویسه			buf[1024];

	اگر (flags & IMAGER_FLAG_INODEMAP) {
		اگر (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			اگر (retval)
				بازگشت retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_شمار;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} وگرنه {
		اگر (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			اگر (retval)
				بازگشت retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super, fs->group_desc_شمار);
		size = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	}
	total_size = size * fs->group_desc_شمار;

	وقتی (cnt > 0) {
		size = اندازه(buf);
		اگر (size > (cnt >> 3))
			size = (cnt >> 3);
		اگر (size == 0)
			بشکن;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		اگر (retval)
			بازگشت retval;

		actual = اكتب(fd, buf, size);
		اگر (actual == -1)
			بازگشت errno;
		اگر (actual != (صحیح) size)
			بازگشت EXT2_ET_SHORT_READ;

		itr += size << 3;
		cnt -= size << 3;
	}

	size = total_size % fs->blocksize;
	memset(buf, 0, اندازه(buf));
	اگر (size) {
		size = fs->blocksize - size;
		وقتی (size) {
			c = size;
			اگر (c > (صحیح) اندازه(buf))
				c = اندازه(buf);
			actual = اكتب(fd, buf, c);
			اگر (actual < 0)
				بازگشت errno;
			اگر ((size_t) actual != c)
				بازگشت EXT2_ET_SHORT_WRITE;
			size -= c;
		}
	}
	بازگشت 0;
}


/*
 * Read the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, صحیح fd, صحیح flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	__u64			itr, cnt;
	نویسه			buf[1024];
	unsigned صحیح		size;
	ssize_t			actual;

	اگر (flags & IMAGER_FLAG_INODEMAP) {
		اگر (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			اگر (retval)
				بازگشت retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_شمار;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} وگرنه {
		اگر (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			اگر (retval)
				بازگشت retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_BLOCKS(fs->super, fs->group_desc_شمار);
		size = EXT2_BLOCKS_PER_GROUP(fs->super) / 8;
	}

	وقتی (cnt > 0) {
		size = اندازه(buf);
		اگر (size > (cnt >> 3))
			size = (cnt >> 3);
		اگر (size == 0)
			بشکن;

		actual = اقرأ(fd, buf, size);
		اگر (actual == -1)
			بازگشت errno;
		اگر (actual != (صحیح) size)
			بازگشت EXT2_ET_SHORT_READ;

		retval = ext2fs_set_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		اگر (retval)
			بازگشت retval;

		itr += size << 3;
		cnt -= size << 3;
	}
	بازگشت 0;
}
