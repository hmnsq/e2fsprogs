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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#যদি HAVE_ERRNO_H
#অন্তর্ভাৱ <errno.h>
#endif
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <time.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাৱ <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাৱ <sys/types.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"

#ifndef HAVE_TYPE_SSIZE_T
প্রকারনির্দেশ পূর্ণাঙ্ক ssize_t;
#endif

/*
 * This function returns 1 if the specified block is all zeros
 */
স্থির পূর্ণাঙ্ক check_zero_block(ৱর্ণ *buf, পূর্ণাঙ্ক blocksize)
{
	ৱর্ণ	*cp = buf;
	পূর্ণাঙ্ক	বাম = blocksize;

	যেতিয়ালৈ (বাম > 0) {
		যদি (*cp++)
			ঘূৰাই 0;
		বাম--;
	}
	ঘূৰাই 1;
}

/*
 * Write the inode table out as a single block.
 */
#define BUF_BLOCKS	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, পূর্ণাঙ্ক fd, পূর্ণাঙ্ক flags)
{
	dgrp_t		group;
	ssize_t		বাম, c, d;
	ৱর্ণ		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	যদি (!buf)
		ঘূৰাই ENOMEM;

	বাবে (group = 0; group < fs->group_desc_গুনতি; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		যদি (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			গচ্ছ errout;
		}
		বাম = fs->inode_blocks_per_group;
		যদি ((blk < fs->super->s_first_data_block) ||
		    (blk + বাম - 1 >= ext2fs_blocks_গুনতি(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
			গচ্ছ errout;
		}
		যেতিয়ালৈ (বাম) {
			c = BUF_BLOCKS;
			যদি (c > বাম)
				c = বাম;
			retval = io_channel_read_blk64(fs->io, blk, c, buf);
			যদি (retval)
				গচ্ছ errout;
			cp = buf;
			যেতিয়ালৈ (c) {
				যদি (!(flags & IMAGER_FLAG_SPARSEWRITE)) {
					d = c;
					গচ্ছ skip_sparse;
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
						গচ্ছ errout;
					}
					অনুৱর্ত;
				}
				/* Find non-zero blocks */
				বাবে (d = 1; d < c; d++) {
					যদি (check_zero_block(cp +
							     d * fs->blocksize,
							     fs->blocksize))
						ভঙ্গ;
				}
			skip_sparse:
				actual = লিখ(fd, cp, d * fs->blocksize);
				যদি (actual == -1) {
					retval = errno;
					গচ্ছ errout;
				}
				যদি (actual != d * fs->blocksize) {
					retval = EXT2_ET_SHORT_WRITE;
					গচ্ছ errout;
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
	ঘূৰাই retval;
}

/*
 * Read in the inode table and stuff it into place
 */
errcode_t ext2fs_image_inode_read(ext2_filsys fs, পূর্ণাঙ্ক fd,
				  পূর্ণাঙ্ক flags EXT2FS_ATTR((unused)))
{
	dgrp_t		group;
	ssize_t		c, বাম;
	ৱর্ণ		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	যদি (!buf)
		ঘূৰাই ENOMEM;

	বাবে (group = 0; group < fs->group_desc_গুনতি; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		যদি (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			গচ্ছ errout;
		}
		বাম = fs->inode_blocks_per_group;
		যেতিয়ালৈ (বাম) {
			c = BUF_BLOCKS;
			যদি (c > বাম)
				c = বাম;
			actual = পঠ(fd, buf, fs->blocksize * c);
			যদি (actual == -1) {
				retval = errno;
				গচ্ছ errout;
			}
			যদি (actual != fs->blocksize * c) {
				retval = EXT2_ET_SHORT_READ;
				গচ্ছ errout;
			}
			retval = io_channel_write_blk64(fs->io, blk, c, buf);
			যদি (retval)
				গচ্ছ errout;

			blk += c;
			বাম -= c;
		}
	}
	retval = ext2fs_flush_icache(fs);

errout:
	মুক্ত(buf);
	ঘূৰাই retval;
}

/*
 * Write out superblock and group descriptors
 */
errcode_t ext2fs_image_super_write(ext2_filsys fs, পূর্ণাঙ্ক fd,
				   পূর্ণাঙ্ক flags EXT2FS_ATTR((unused)))
{
	ৱর্ণ		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#ifdef শব্দগুলি_BIGENDIAN
	unsigned পূর্ণাঙ্ক	groups_per_block;
	সংরচনা		ext2_group_desc *gdp;
	পূর্ণাঙ্ক		j;
#endif

	যদি (fs->group_desc == রিক্ত)
		ঘূৰাই EXT2_ET_NO_GDESC;

	buf = malloc(fs->blocksize);
	যদি (!buf)
		ঘূৰাই ENOMEM;

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
	memcpy(buf, fs->super, SUPERBLOCK_SIZE);
	ext2fs_swap_super(fs->super);
#অন্যথা
	memcpy(buf, fs->super, SUPERBLOCK_SIZE);
#endif
	actual = লিখ(fd, buf, fs->blocksize);
	যদি (actual == -1) {
		retval = errno;
		গচ্ছ errout;
	}
	যদি (actual != (ssize_t) fs->blocksize) {
		retval = EXT2_ET_SHORT_WRITE;
		গচ্ছ errout;
	}

	/*
	 * Now write out the block group descriptors
	 */

	cp = (ৱর্ণ *) fs->group_desc;

#ifdef শব্দগুলি_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	বাবে (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		যদি (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	actual = লিখ(fd, cp, (ssize_t)fs->blocksize * fs->desc_blocks);


#ifdef শব্দগুলি_BIGENDIAN
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	বাবে (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		যদি (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	যদি (actual == -1) {
		retval = errno;
		গচ্ছ errout;
	}
	যদি (actual != (ssize_t)(fs->blocksize * fs->desc_blocks)) {
		retval = EXT2_ET_SHORT_WRITE;
		গচ্ছ errout;
	}

	retval = 0;

errout:
	মুক্ত(buf);
	ঘূৰাই retval;
}

/*
 * Read the superblock and group descriptors and overwrite them.
 */
errcode_t ext2fs_image_super_read(ext2_filsys fs, পূর্ণাঙ্ক fd,
				  পূর্ণাঙ্ক flags EXT2FS_ATTR((unused)))
{
	ৱর্ণ		*buf;
	ssize_t		actual, size;
	errcode_t	retval;

	size = (ssize_t)fs->blocksize * (fs->desc_blocks + 1);
	buf = malloc(size);
	যদি (!buf)
		ঘূৰাই ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = পঠ(fd, buf, size);
	যদি (actual == -1) {
		retval = errno;
		গচ্ছ errout;
	}
	যদি (actual != size) {
		retval = EXT2_ET_SHORT_READ;
		গচ্ছ errout;
	}

	/*
	 * Now copy in the superblock and group descriptors
	 */
	memcpy(fs->super, buf, SUPERBLOCK_SIZE);

	memcpy(fs->group_desc, buf + fs->blocksize,
	       (ssize_t)fs->blocksize * fs->desc_blocks);

	retval = 0;

errout:
	মুক্ত(buf);
	ঘূৰাই retval;
}

/*
 * Write the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, পূর্ণাঙ্ক fd, পূর্ণাঙ্ক flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	ssize_t			actual;
	size_t			c;
	__u64			itr, cnt, size, total_size;
	ৱর্ণ			buf[1024];

	যদি (flags & IMAGER_FLAG_INODEMAP) {
		যদি (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			যদি (retval)
				ঘূৰাই retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_গুনতি;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} অন্যথা {
		যদি (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			যদি (retval)
				ঘূৰাই retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super, fs->group_desc_গুনতি);
		size = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	}
	total_size = size * fs->group_desc_গুনতি;

	যেতিয়ালৈ (cnt > 0) {
		size = পরিমাণ(buf);
		যদি (size > (cnt >> 3))
			size = (cnt >> 3);
		যদি (size == 0)
			ভঙ্গ;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		যদি (retval)
			ঘূৰাই retval;

		actual = লিখ(fd, buf, size);
		যদি (actual == -1)
			ঘূৰাই errno;
		যদি (actual != (পূর্ণাঙ্ক) size)
			ঘূৰাই EXT2_ET_SHORT_READ;

		itr += size << 3;
		cnt -= size << 3;
	}

	size = total_size % fs->blocksize;
	memset(buf, 0, পরিমাণ(buf));
	যদি (size) {
		size = fs->blocksize - size;
		যেতিয়ালৈ (size) {
			c = size;
			যদি (c > (পূর্ণাঙ্ক) পরিমাণ(buf))
				c = পরিমাণ(buf);
			actual = লিখ(fd, buf, c);
			যদি (actual < 0)
				ঘূৰাই errno;
			যদি ((size_t) actual != c)
				ঘূৰাই EXT2_ET_SHORT_WRITE;
			size -= c;
		}
	}
	ঘূৰাই 0;
}


/*
 * Read the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, পূর্ণাঙ্ক fd, পূর্ণাঙ্ক flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	__u64			itr, cnt;
	ৱর্ণ			buf[1024];
	unsigned পূর্ণাঙ্ক		size;
	ssize_t			actual;

	যদি (flags & IMAGER_FLAG_INODEMAP) {
		যদি (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			যদি (retval)
				ঘূৰাই retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_গুনতি;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} অন্যথা {
		যদি (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			যদি (retval)
				ঘূৰাই retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_BLOCKS(fs->super, fs->group_desc_গুনতি);
		size = EXT2_BLOCKS_PER_GROUP(fs->super) / 8;
	}

	যেতিয়ালৈ (cnt > 0) {
		size = পরিমাণ(buf);
		যদি (size > (cnt >> 3))
			size = (cnt >> 3);
		যদি (size == 0)
			ভঙ্গ;

		actual = পঠ(fd, buf, size);
		যদি (actual == -1)
			ঘূৰাই errno;
		যদি (actual != (পূর্ণাঙ্ক) size)
			ঘূৰাই EXT2_ET_SHORT_READ;

		retval = ext2fs_set_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		যদি (retval)
			ঘূৰাই retval;

		itr += size << 3;
		cnt -= size << 3;
	}
	ঘূৰাই 0;
}
