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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#যদি HAVE_ERRNO_H
#অন্তর্ভাব <errno.h>
#endif
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <time.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"

#ifndef HAVE_TYPE_SSIZE_T
প্রকারলেখ পূর্ণসংখ্যা ssize_t;
#endif

/*
 * This function returns 1 if the specified block is all zeros
 */
স্থির পূর্ণসংখ্যা check_zero_block(অক্ষর *buf, পূর্ণসংখ্যা blocksize)
{
	অক্ষর	*cp = buf;
	পূর্ণসংখ্যা	বাম = blocksize;

	যখন (বাম > 0) {
		যদি (*cp++)
			ফেরত 0;
		বাম--;
	}
	ফেরত 1;
}

/*
 * Write the inode table out as a single block.
 */
#define BUF_BLOCKS	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, পূর্ণসংখ্যা fd, পূর্ণসংখ্যা flags)
{
	dgrp_t		group;
	ssize_t		বাম, c, d;
	অক্ষর		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	যদি (!buf)
		ফেরত ENOMEM;

	জন্য (group = 0; group < fs->group_desc_গুনতি; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		যদি (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			যাও errout;
		}
		বাম = fs->inode_blocks_per_group;
		যদি ((blk < fs->super->s_first_তথ্য_block) ||
		    (blk + বাম - 1 >= ext2fs_blocks_গুনতি(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
			যাও errout;
		}
		যখন (বাম) {
			c = BUF_BLOCKS;
			যদি (c > বাম)
				c = বাম;
			retval = io_channel_read_blk64(fs->io, blk, c, buf);
			যদি (retval)
				যাও errout;
			cp = buf;
			যখন (c) {
				যদি (!(flags & IMAGER_FLAG_SPARSEWRITE)) {
					d = c;
					যাও skip_sparse;
				}
				/* Skip zero blocks */
				যদি (check_zero_block(cp, fs->blocksize)) {
					c--;
					blk++;
					বাম--;
					cp += fs->blocksize;
					r = ext2fs_llseek(fd, fs->blocksize,
							  SEEK_CUR);
					যদি (r < 0) {
						retval = errno;
						যাও errout;
					}
					চালু;
				}
				/* Find non-zero blocks */
				জন্য (d = 1; d < c; d++) {
					যদি (check_zero_block(cp +
							     d * fs->blocksize,
							     fs->blocksize))
						বিরতি;
				}
			skip_sparse:
				actual = লিখ(fd, cp, d * fs->blocksize);
				যদি (actual == -1) {
					retval = errno;
					যাও errout;
				}
				যদি (actual != d * fs->blocksize) {
					retval = EXT2_ET_SHORT_WRITE;
					যাও errout;
				}
				blk += d;
				বাম -= d;
				cp += d * fs->blocksize;
				c -= d;
			}
		}
	}
	retval = 0;

errout:
	মুক্ত(buf);
	ফেরত retval;
}

/*
 * Read in the inode table and stuff it into place
 */
errcode_t ext2fs_image_inode_read(ext2_filsys fs, পূর্ণসংখ্যা fd,
				  পূর্ণসংখ্যা flags EXT2FS_ATTR((unused)))
{
	dgrp_t		group;
	ssize_t		c, বাম;
	অক্ষর		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	যদি (!buf)
		ফেরত ENOMEM;

	জন্য (group = 0; group < fs->group_desc_গুনতি; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		যদি (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			যাও errout;
		}
		বাম = fs->inode_blocks_per_group;
		যখন (বাম) {
			c = BUF_BLOCKS;
			যদি (c > বাম)
				c = বাম;
			actual = পঠ(fd, buf, fs->blocksize * c);
			যদি (actual == -1) {
				retval = errno;
				যাও errout;
			}
			যদি (actual != fs->blocksize * c) {
				retval = EXT2_ET_SHORT_READ;
				যাও errout;
			}
			retval = io_channel_write_blk64(fs->io, blk, c, buf);
			যদি (retval)
				যাও errout;

			blk += c;
			বাম -= c;
		}
	}
	retval = ext2fs_flush_icache(fs);

errout:
	মুক্ত(buf);
	ফেরত retval;
}

/*
 * Write out superblock and group descriptors
 */
errcode_t ext2fs_image_super_write(ext2_filsys fs, পূর্ণসংখ্যা fd,
				   পূর্ণসংখ্যা flags EXT2FS_ATTR((unused)))
{
	অক্ষর		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#ifdef শব্দগুলি_BIGENDIAN
	unsigned পূর্ণসংখ্যা	groups_per_block;
	কাঠামো		ext2_group_desc *gdp;
	পূর্ণসংখ্যা		j;
#endif

	যদি (fs->group_desc == ফাঁকা)
		ফেরত EXT2_ET_NO_GDESC;

	buf = malloc(fs->blocksize);
	যদি (!buf)
		ফেরত ENOMEM;

	/*
	 * Write out the superblock
	 */
	memset(buf, 0, fs->blocksize);
#ifdef শব্দগুলি_BIGENDIAN
	/*
	 * We're writing out superblock so let's convert
	 * it to little endian and then back if needed
	 */
	ext2fs_swap_super(fs->super);
	memcpy(buf, fs->super, SUPERBLOCK_আকার);
	ext2fs_swap_super(fs->super);
#অন্যথা
	memcpy(buf, fs->super, SUPERBLOCK_আকার);
#endif
	actual = লিখ(fd, buf, fs->blocksize);
	যদি (actual == -1) {
		retval = errno;
		যাও errout;
	}
	যদি (actual != (ssize_t) fs->blocksize) {
		retval = EXT2_ET_SHORT_WRITE;
		যাও errout;
	}

	/*
	 * Now write out the block group descriptors
	 */

	cp = (অক্ষর *) fs->group_desc;

#ifdef শব্দগুলি_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	জন্য (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		যদি (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	actual = লিখ(fd, cp, (ssize_t)fs->blocksize * fs->desc_blocks);


#ifdef শব্দগুলি_BIGENDIAN
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	জন্য (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		যদি (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	যদি (actual == -1) {
		retval = errno;
		যাও errout;
	}
	যদি (actual != (ssize_t)(fs->blocksize * fs->desc_blocks)) {
		retval = EXT2_ET_SHORT_WRITE;
		যাও errout;
	}

	retval = 0;

errout:
	মুক্ত(buf);
	ফেরত retval;
}

/*
 * Read the superblock and group descriptors and overwrite them.
 */
errcode_t ext2fs_image_super_read(ext2_filsys fs, পূর্ণসংখ্যা fd,
				  পূর্ণসংখ্যা flags EXT2FS_ATTR((unused)))
{
	অক্ষর		*buf;
	ssize_t		actual, আকার_2;
	errcode_t	retval;

	আকার_2 = (ssize_t)fs->blocksize * (fs->desc_blocks + 1);
	buf = malloc(আকার_2);
	যদি (!buf)
		ফেরত ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = পঠ(fd, buf, আকার_2);
	যদি (actual == -1) {
		retval = errno;
		যাও errout;
	}
	যদি (actual != আকার_2) {
		retval = EXT2_ET_SHORT_READ;
		যাও errout;
	}

	/*
	 * Now copy in the superblock and group descriptors
	 */
	memcpy(fs->super, buf, SUPERBLOCK_আকার);

	memcpy(fs->group_desc, buf + fs->blocksize,
	       (ssize_t)fs->blocksize * fs->desc_blocks);

	retval = 0;

errout:
	মুক্ত(buf);
	ফেরত retval;
}

/*
 * Write the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, পূর্ণসংখ্যা fd, পূর্ণসংখ্যা flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	ssize_t			actual;
	size_t			c;
	__u64			itr, cnt, আকার_2, যোগ_আকার;
	অক্ষর			buf[1024];

	যদি (flags & IMAGER_FLAG_INODEMAP) {
		যদি (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			যদি (retval)
				ফেরত retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_গুনতি;
		আকার_2 = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} অন্যথা {
		যদি (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			যদি (retval)
				ফেরত retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_তথ্য_block;
		cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super, fs->group_desc_গুনতি);
		আকার_2 = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	}
	যোগ_আকার = আকার_2 * fs->group_desc_গুনতি;

	যখন (cnt > 0) {
		আকার_2 = আকার(buf);
		যদি (আকার_2 > (cnt >> 3))
			আকার_2 = (cnt >> 3);
		যদি (আকার_2 == 0)
			বিরতি;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       আকার_2 << 3, buf);
		যদি (retval)
			ফেরত retval;

		actual = লিখ(fd, buf, আকার_2);
		যদি (actual == -1)
			ফেরত errno;
		যদি (actual != (পূর্ণসংখ্যা) আকার_2)
			ফেরত EXT2_ET_SHORT_READ;

		itr += আকার_2 << 3;
		cnt -= আকার_2 << 3;
	}

	আকার_2 = যোগ_আকার % fs->blocksize;
	memset(buf, 0, আকার(buf));
	যদি (আকার_2) {
		আকার_2 = fs->blocksize - আকার_2;
		যখন (আকার_2) {
			c = আকার_2;
			যদি (c > (পূর্ণসংখ্যা) আকার(buf))
				c = আকার(buf);
			actual = লিখ(fd, buf, c);
			যদি (actual < 0)
				ফেরত errno;
			যদি ((size_t) actual != c)
				ফেরত EXT2_ET_SHORT_WRITE;
			আকার_2 -= c;
		}
	}
	ফেরত 0;
}


/*
 * Read the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, পূর্ণসংখ্যা fd, পূর্ণসংখ্যা flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	__u64			itr, cnt;
	অক্ষর			buf[1024];
	unsigned পূর্ণসংখ্যা		আকার_2;
	ssize_t			actual;

	যদি (flags & IMAGER_FLAG_INODEMAP) {
		যদি (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			যদি (retval)
				ফেরত retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_গুনতি;
		আকার_2 = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} অন্যথা {
		যদি (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			যদি (retval)
				ফেরত retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_তথ্য_block;
		cnt = EXT2_GROUPS_TO_BLOCKS(fs->super, fs->group_desc_গুনতি);
		আকার_2 = EXT2_BLOCKS_PER_GROUP(fs->super) / 8;
	}

	যখন (cnt > 0) {
		আকার_2 = আকার(buf);
		যদি (আকার_2 > (cnt >> 3))
			আকার_2 = (cnt >> 3);
		যদি (আকার_2 == 0)
			বিরতি;

		actual = পঠ(fd, buf, আকার_2);
		যদি (actual == -1)
			ফেরত errno;
		যদি (actual != (পূর্ণসংখ্যা) আকার_2)
			ফেরত EXT2_ET_SHORT_READ;

		retval = ext2fs_set_generic_bmap_range(bmap, itr,
						       আকার_2 << 3, buf);
		যদি (retval)
			ফেরত retval;

		itr += আকার_2 << 3;
		cnt -= আকার_2 << 3;
	}
	ফেরত 0;
}
