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
#ܐܢ HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ܐܢ HAVE_ERRNO_H
#ضمن <errno.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#ܐܢ HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#ܐܢ HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

#ifndef HAVE_TYPE_SSIZE_T
تعريفنوع صحيح ssize_t;
#endif

/*
 * This function returns 1 if the specified block is all zeros
 */
ساكن صحيح check_zero_block(حرف *buf, صحيح blocksize)
{
	حرف	*cp = buf;
	صحيح	left = blocksize;

	ܟܡܐ (left > 0) {
		ܐܢ (*cp++)
			ܦܢܐ 0;
		left--;
	}
	ܦܢܐ 1;
}

/*
 * Write the inode table out as a single block.
 */
#define BUF_BLOCKS	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, صحيح fd, صحيح flags)
{
	dgrp_t		group;
	ssize_t		left, c, d;
	حرف		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	ܐܢ (!buf)
		ܦܢܐ ENOMEM;

	لكل (group = 0; group < fs->group_desc_count; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		ܐܢ (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			اذهب errout;
		}
		left = fs->inode_blocks_per_group;
		ܐܢ ((blk < fs->super->s_first_data_block) ||
		    (blk + left - 1 >= ext2fs_blocks_count(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
			اذهب errout;
		}
		ܟܡܐ (left) {
			c = BUF_BLOCKS;
			ܐܢ (c > left)
				c = left;
			retval = io_channel_read_blk64(fs->io, blk, c, buf);
			ܐܢ (retval)
				اذهب errout;
			cp = buf;
			ܟܡܐ (c) {
				ܐܢ (!(flags & IMAGER_FLAG_SPARSEWRITE)) {
					d = c;
					اذهب skip_sparse;
				}
				/* Skip zero blocks */
				ܐܢ (check_zero_block(cp, fs->blocksize)) {
					c--;
					blk++;
					left--;
					cp += fs->blocksize;
					r = ext2fs_llseek(fd, fs->blocksize,
							  SEEK_CUR);
					ܐܢ (r < 0) {
						retval = errno;
						اذهب errout;
					}
					استمر;
				}
				/* Find non-zero blocks */
				لكل (d = 1; d < c; d++) {
					ܐܢ (check_zero_block(cp +
							     d * fs->blocksize,
							     fs->blocksize))
						اكسر;
				}
			skip_sparse:
				actual = اكتب(fd, cp, d * fs->blocksize);
				ܐܢ (actual == -1) {
					retval = errno;
					اذهب errout;
				}
				ܐܢ (actual != d * fs->blocksize) {
					retval = EXT2_ET_SHORT_WRITE;
					اذهب errout;
				}
				blk += d;
				left -= d;
				cp += d * fs->blocksize;
				c -= d;
			}
		}
	}
	retval = 0;

errout:
	حرر(buf);
	ܦܢܐ retval;
}

/*
 * Read in the inode table and stuff it into place
 */
errcode_t ext2fs_image_inode_read(ext2_filsys fs, صحيح fd,
				  صحيح flags EXT2FS_ATTR((unused)))
{
	dgrp_t		group;
	ssize_t		c, left;
	حرف		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	ܐܢ (!buf)
		ܦܢܐ ENOMEM;

	لكل (group = 0; group < fs->group_desc_count; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		ܐܢ (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			اذهب errout;
		}
		left = fs->inode_blocks_per_group;
		ܟܡܐ (left) {
			c = BUF_BLOCKS;
			ܐܢ (c > left)
				c = left;
			actual = اقرأ(fd, buf, fs->blocksize * c);
			ܐܢ (actual == -1) {
				retval = errno;
				اذهب errout;
			}
			ܐܢ (actual != fs->blocksize * c) {
				retval = EXT2_ET_SHORT_READ;
				اذهب errout;
			}
			retval = io_channel_write_blk64(fs->io, blk, c, buf);
			ܐܢ (retval)
				اذهب errout;

			blk += c;
			left -= c;
		}
	}
	retval = ext2fs_flush_icache(fs);

errout:
	حرر(buf);
	ܦܢܐ retval;
}

/*
 * Write out superblock and group descriptors
 */
errcode_t ext2fs_image_super_write(ext2_filsys fs, صحيح fd,
				   صحيح flags EXT2FS_ATTR((unused)))
{
	حرف		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#ifdef WORDS_BIGENDIAN
	unsigned صحيح	groups_per_block;
	بنية		ext2_group_desc *gdp;
	صحيح		j;
#endif

	ܐܢ (fs->group_desc == عدم)
		ܦܢܐ EXT2_ET_NO_GDESC;

	buf = malloc(fs->blocksize);
	ܐܢ (!buf)
		ܦܢܐ ENOMEM;

	/*
	 * Write out the superblock
	 */
	memset(buf, 0, fs->blocksize);
#ifdef WORDS_BIGENDIAN
	/*
	 * We're writing out superblock so let's convert
	 * it to little endian and then back if needed
	 */
	ext2fs_swap_super(fs->super);
	memcpy(buf, fs->super, SUPERBLOCK_SIZE);
	ext2fs_swap_super(fs->super);
#ܐܚܪܢܐ
	memcpy(buf, fs->super, SUPERBLOCK_SIZE);
#endif
	actual = اكتب(fd, buf, fs->blocksize);
	ܐܢ (actual == -1) {
		retval = errno;
		اذهب errout;
	}
	ܐܢ (actual != (ssize_t) fs->blocksize) {
		retval = EXT2_ET_SHORT_WRITE;
		اذهب errout;
	}

	/*
	 * Now write out the block group descriptors
	 */

	cp = (حرف *) fs->group_desc;

#ifdef WORDS_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	لكل (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		ܐܢ (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	actual = اكتب(fd, cp, (ssize_t)fs->blocksize * fs->desc_blocks);


#ifdef WORDS_BIGENDIAN
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	لكل (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		ܐܢ (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	ܐܢ (actual == -1) {
		retval = errno;
		اذهب errout;
	}
	ܐܢ (actual != (ssize_t)(fs->blocksize * fs->desc_blocks)) {
		retval = EXT2_ET_SHORT_WRITE;
		اذهب errout;
	}

	retval = 0;

errout:
	حرر(buf);
	ܦܢܐ retval;
}

/*
 * Read the superblock and group descriptors and overwrite them.
 */
errcode_t ext2fs_image_super_read(ext2_filsys fs, صحيح fd,
				  صحيح flags EXT2FS_ATTR((unused)))
{
	حرف		*buf;
	ssize_t		actual, size;
	errcode_t	retval;

	size = (ssize_t)fs->blocksize * (fs->desc_blocks + 1);
	buf = malloc(size);
	ܐܢ (!buf)
		ܦܢܐ ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = اقرأ(fd, buf, size);
	ܐܢ (actual == -1) {
		retval = errno;
		اذهب errout;
	}
	ܐܢ (actual != size) {
		retval = EXT2_ET_SHORT_READ;
		اذهب errout;
	}

	/*
	 * Now copy in the superblock and group descriptors
	 */
	memcpy(fs->super, buf, SUPERBLOCK_SIZE);

	memcpy(fs->group_desc, buf + fs->blocksize,
	       (ssize_t)fs->blocksize * fs->desc_blocks);

	retval = 0;

errout:
	حرر(buf);
	ܦܢܐ retval;
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
	__u64			itr, cnt, size, total_size;
	حرف			buf[1024];

	ܐܢ (flags & IMAGER_FLAG_INODEMAP) {
		ܐܢ (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			ܐܢ (retval)
				ܦܢܐ retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_count;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} ܐܚܪܢܐ {
		ܐܢ (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			ܐܢ (retval)
				ܦܢܐ retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super, fs->group_desc_count);
		size = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	}
	total_size = size * fs->group_desc_count;

	ܟܡܐ (cnt > 0) {
		size = حجم(buf);
		ܐܢ (size > (cnt >> 3))
			size = (cnt >> 3);
		ܐܢ (size == 0)
			اكسر;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		ܐܢ (retval)
			ܦܢܐ retval;

		actual = اكتب(fd, buf, size);
		ܐܢ (actual == -1)
			ܦܢܐ errno;
		ܐܢ (actual != (صحيح) size)
			ܦܢܐ EXT2_ET_SHORT_READ;

		itr += size << 3;
		cnt -= size << 3;
	}

	size = total_size % fs->blocksize;
	memset(buf, 0, حجم(buf));
	ܐܢ (size) {
		size = fs->blocksize - size;
		ܟܡܐ (size) {
			c = size;
			ܐܢ (c > (صحيح) حجم(buf))
				c = حجم(buf);
			actual = اكتب(fd, buf, c);
			ܐܢ (actual < 0)
				ܦܢܐ errno;
			ܐܢ ((size_t) actual != c)
				ܦܢܐ EXT2_ET_SHORT_WRITE;
			size -= c;
		}
	}
	ܦܢܐ 0;
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
	unsigned صحيح		size;
	ssize_t			actual;

	ܐܢ (flags & IMAGER_FLAG_INODEMAP) {
		ܐܢ (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			ܐܢ (retval)
				ܦܢܐ retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_count;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} ܐܚܪܢܐ {
		ܐܢ (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			ܐܢ (retval)
				ܦܢܐ retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_BLOCKS(fs->super, fs->group_desc_count);
		size = EXT2_BLOCKS_PER_GROUP(fs->super) / 8;
	}

	ܟܡܐ (cnt > 0) {
		size = حجم(buf);
		ܐܢ (size > (cnt >> 3))
			size = (cnt >> 3);
		ܐܢ (size == 0)
			اكسر;

		actual = اقرأ(fd, buf, size);
		ܐܢ (actual == -1)
			ܦܢܐ errno;
		ܐܢ (actual != (صحيح) size)
			ܦܢܐ EXT2_ET_SHORT_READ;

		retval = ext2fs_set_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		ܐܢ (retval)
			ܦܢܐ retval;

		itr += size << 3;
		cnt -= size << 3;
	}
	ܦܢܐ 0;
}
