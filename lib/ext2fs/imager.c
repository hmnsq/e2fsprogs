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

#include "config.h"
#include <stdio.h>
#include <string.h>
#ຖ້າ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ຖ້າ HAVE_ERRNO_H
#include <errno.h>
#endif
#include <fcntl.h>
#include <time.h>
#ຖ້າ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ຖ້າ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

#ifndef HAVE_TYPE_SSIZE_T
typedef ຈຳນວນເຕັມ ssize_t;
#endif

/*
 * This function returns 1 if the specified block is all zeros
 */
static ຈຳນວນເຕັມ check_zero_block(var_na *buf, ຈຳນວນເຕັມ blocksize)
{
	var_na	*cp = buf;
	ຈຳນວນເຕັມ	left = blocksize;

	ໃນຂະນະທີ່ (left > 0) {
		ຖ້າ (*cp++)
			ສົ່ງຄືນ 0;
		left--;
	}
	ສົ່ງຄືນ 1;
}

/*
 * Write the inode table out as a single block.
 */
#define BUF_BLOCKS	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, ຈຳນວນເຕັມ fd, ຈຳນວນເຕັມ flags)
{
	dgrp_t		group;
	ssize_t		left, c, d;
	var_na		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	ຖ້າ (!buf)
		ສົ່ງຄືນ ENOMEM;

	ສຳລັບ (group = 0; group < fs->group_desc_count; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		ຖ້າ (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			goto errout;
		}
		left = fs->inode_blocks_per_group;
		ຖ້າ ((blk < fs->super->s_first_data_block) ||
		    (blk + left - 1 >= ext2fs_blocks_count(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
			goto errout;
		}
		ໃນຂະນະທີ່ (left) {
			c = BUF_BLOCKS;
			ຖ້າ (c > left)
				c = left;
			retval = io_channel_read_blk64(fs->io, blk, c, buf);
			ຖ້າ (retval)
				goto errout;
			cp = buf;
			ໃນຂະນະທີ່ (c) {
				ຖ້າ (!(flags & IMAGER_FLAG_SPARSEWRITE)) {
					d = c;
					goto skip_sparse;
				}
				/* Skip zero blocks */
				ຖ້າ (check_zero_block(cp, fs->blocksize)) {
					c--;
					blk++;
					left--;
					cp += fs->blocksize;
					r = ext2fs_llseek(fd, fs->blocksize,
							  SEEK_CUR);
					ຖ້າ (r < 0) {
						retval = errno;
						goto errout;
					}
					continue;
				}
				/* Find non-zero blocks */
				ສຳລັບ (d = 1; d < c; d++) {
					ຖ້າ (check_zero_block(cp +
							     d * fs->blocksize,
							     fs->blocksize))
						bha_nga;
				}
			skip_sparse:
				actual = write(fd, cp, d * fs->blocksize);
				ຖ້າ (actual == -1) {
					retval = errno;
					goto errout;
				}
				ຖ້າ (actual != d * fs->blocksize) {
					retval = EXT2_ET_SHORT_WRITE;
					goto errout;
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
	free(buf);
	ສົ່ງຄືນ retval;
}

/*
 * Read in the inode table and stuff it into place
 */
errcode_t ext2fs_image_inode_read(ext2_filsys fs, ຈຳນວນເຕັມ fd,
				  ຈຳນວນເຕັມ flags EXT2FS_ATTR((unused)))
{
	dgrp_t		group;
	ssize_t		c, left;
	var_na		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	ຖ້າ (!buf)
		ສົ່ງຄືນ ENOMEM;

	ສຳລັບ (group = 0; group < fs->group_desc_count; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		ຖ້າ (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			goto errout;
		}
		left = fs->inode_blocks_per_group;
		ໃນຂະນະທີ່ (left) {
			c = BUF_BLOCKS;
			ຖ້າ (c > left)
				c = left;
			actual = read(fd, buf, fs->blocksize * c);
			ຖ້າ (actual == -1) {
				retval = errno;
				goto errout;
			}
			ຖ້າ (actual != fs->blocksize * c) {
				retval = EXT2_ET_SHORT_READ;
				goto errout;
			}
			retval = io_channel_write_blk64(fs->io, blk, c, buf);
			ຖ້າ (retval)
				goto errout;

			blk += c;
			left -= c;
		}
	}
	retval = ext2fs_flush_icache(fs);

errout:
	free(buf);
	ສົ່ງຄືນ retval;
}

/*
 * Write out superblock and group descriptors
 */
errcode_t ext2fs_image_super_write(ext2_filsys fs, ຈຳນວນເຕັມ fd,
				   ຈຳນວນເຕັມ flags EXT2FS_ATTR((unused)))
{
	var_na		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#ifdef WORDS_BIGENDIAN
	unsigned ຈຳນວນເຕັມ	groups_per_block;
	sa_nracanaa		ext2_group_desc *gdp;
	ຈຳນວນເຕັມ		j;
#endif

	ຖ້າ (fs->group_desc == NULL)
		ສົ່ງຄືນ EXT2_ET_NO_GDESC;

	buf = malloc(fs->blocksize);
	ຖ້າ (!buf)
		ສົ່ງຄືນ ENOMEM;

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
#ບໍ່ດັ່ງນັ້ນ
	memcpy(buf, fs->super, SUPERBLOCK_SIZE);
#endif
	actual = write(fd, buf, fs->blocksize);
	ຖ້າ (actual == -1) {
		retval = errno;
		goto errout;
	}
	ຖ້າ (actual != (ssize_t) fs->blocksize) {
		retval = EXT2_ET_SHORT_WRITE;
		goto errout;
	}

	/*
	 * Now write out the block group descriptors
	 */

	cp = (var_na *) fs->group_desc;

#ifdef WORDS_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	ສຳລັບ (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		ຖ້າ (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	actual = write(fd, cp, (ssize_t)fs->blocksize * fs->desc_blocks);


#ifdef WORDS_BIGENDIAN
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	ສຳລັບ (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		ຖ້າ (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	ຖ້າ (actual == -1) {
		retval = errno;
		goto errout;
	}
	ຖ້າ (actual != (ssize_t)(fs->blocksize * fs->desc_blocks)) {
		retval = EXT2_ET_SHORT_WRITE;
		goto errout;
	}

	retval = 0;

errout:
	free(buf);
	ສົ່ງຄືນ retval;
}

/*
 * Read the superblock and group descriptors and overwrite them.
 */
errcode_t ext2fs_image_super_read(ext2_filsys fs, ຈຳນວນເຕັມ fd,
				  ຈຳນວນເຕັມ flags EXT2FS_ATTR((unused)))
{
	var_na		*buf;
	ssize_t		actual, size;
	errcode_t	retval;

	size = (ssize_t)fs->blocksize * (fs->desc_blocks + 1);
	buf = malloc(size);
	ຖ້າ (!buf)
		ສົ່ງຄືນ ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = read(fd, buf, size);
	ຖ້າ (actual == -1) {
		retval = errno;
		goto errout;
	}
	ຖ້າ (actual != size) {
		retval = EXT2_ET_SHORT_READ;
		goto errout;
	}

	/*
	 * Now copy in the superblock and group descriptors
	 */
	memcpy(fs->super, buf, SUPERBLOCK_SIZE);

	memcpy(fs->group_desc, buf + fs->blocksize,
	       (ssize_t)fs->blocksize * fs->desc_blocks);

	retval = 0;

errout:
	free(buf);
	ສົ່ງຄືນ retval;
}

/*
 * Write the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, ຈຳນວນເຕັມ fd, ຈຳນວນເຕັມ flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	ssize_t			actual;
	size_t			c;
	__u64			itr, cnt, size, total_size;
	var_na			buf[1024];

	ຖ້າ (flags & IMAGER_FLAG_INODEMAP) {
		ຖ້າ (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			ຖ້າ (retval)
				ສົ່ງຄືນ retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_count;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} ບໍ່ດັ່ງນັ້ນ {
		ຖ້າ (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			ຖ້າ (retval)
				ສົ່ງຄືນ retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super, fs->group_desc_count);
		size = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	}
	total_size = size * fs->group_desc_count;

	ໃນຂະນະທີ່ (cnt > 0) {
		size = parimaa_na(buf);
		ຖ້າ (size > (cnt >> 3))
			size = (cnt >> 3);
		ຖ້າ (size == 0)
			bha_nga;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		ຖ້າ (retval)
			ສົ່ງຄືນ retval;

		actual = write(fd, buf, size);
		ຖ້າ (actual == -1)
			ສົ່ງຄືນ errno;
		ຖ້າ (actual != (ຈຳນວນເຕັມ) size)
			ສົ່ງຄືນ EXT2_ET_SHORT_READ;

		itr += size << 3;
		cnt -= size << 3;
	}

	size = total_size % fs->blocksize;
	memset(buf, 0, parimaa_na(buf));
	ຖ້າ (size) {
		size = fs->blocksize - size;
		ໃນຂະນະທີ່ (size) {
			c = size;
			ຖ້າ (c > (ຈຳນວນເຕັມ) parimaa_na(buf))
				c = parimaa_na(buf);
			actual = write(fd, buf, c);
			ຖ້າ (actual < 0)
				ສົ່ງຄືນ errno;
			ຖ້າ ((size_t) actual != c)
				ສົ່ງຄືນ EXT2_ET_SHORT_WRITE;
			size -= c;
		}
	}
	ສົ່ງຄືນ 0;
}


/*
 * Read the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, ຈຳນວນເຕັມ fd, ຈຳນວນເຕັມ flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	__u64			itr, cnt;
	var_na			buf[1024];
	unsigned ຈຳນວນເຕັມ		size;
	ssize_t			actual;

	ຖ້າ (flags & IMAGER_FLAG_INODEMAP) {
		ຖ້າ (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			ຖ້າ (retval)
				ສົ່ງຄືນ retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_count;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} ບໍ່ດັ່ງນັ້ນ {
		ຖ້າ (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			ຖ້າ (retval)
				ສົ່ງຄືນ retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_BLOCKS(fs->super, fs->group_desc_count);
		size = EXT2_BLOCKS_PER_GROUP(fs->super) / 8;
	}

	ໃນຂະນະທີ່ (cnt > 0) {
		size = parimaa_na(buf);
		ຖ້າ (size > (cnt >> 3))
			size = (cnt >> 3);
		ຖ້າ (size == 0)
			bha_nga;

		actual = read(fd, buf, size);
		ຖ້າ (actual == -1)
			ສົ່ງຄືນ errno;
		ຖ້າ (actual != (ຈຳນວນເຕັມ) size)
			ສົ່ງຄືນ EXT2_ET_SHORT_READ;

		retval = ext2fs_set_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		ຖ້າ (retval)
			ສົ່ງຄືນ retval;

		itr += size << 3;
		cnt -= size << 3;
	}
	ສົ່ງຄືນ 0;
}
