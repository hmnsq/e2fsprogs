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
#ꦪꦼꦤ꧀ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ꦪꦼꦤ꧀ HAVE_ERRNO_H
#include <errno.h>
#endif
#include <fcntl.h>
#include <time.h>
#ꦪꦼꦤ꧀ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ꦪꦼꦤ꧀ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

#ifndef HAVE_TYPE_SSIZE_T
typedef puur_naa_nka ssize_t;
#endif

/*
 * This function returns 1 if the specified block is all zeros
 */
static puur_naa_nka check_zero_block(var_na *buf, puur_naa_nka blocksize)
{
	var_na	*cp = buf;
	puur_naa_nka	left = blocksize;

	ꦱꦼꦢꦼꦭ (left > 0) {
		ꦪꦼꦤ꧀ (*cp++)
			ꦮꦭꦶ 0;
		left--;
	}
	ꦮꦭꦶ 1;
}

/*
 * Write the inode table out as a single block.
 */
#define BUF_BLOCKS	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, puur_naa_nka fd, puur_naa_nka flags)
{
	dgrp_t		group;
	ssize_t		left, c, d;
	var_na		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	ꦪꦼꦤ꧀ (!buf)
		ꦮꦭꦶ ENOMEM;

	ꦏꦁꦒꦺꦴ (group = 0; group < fs->group_desc_count; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		ꦪꦼꦤ꧀ (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			goto errout;
		}
		left = fs->inode_blocks_per_group;
		ꦪꦼꦤ꧀ ((blk < fs->super->s_first_data_block) ||
		    (blk + left - 1 >= ext2fs_blocks_count(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
			goto errout;
		}
		ꦱꦼꦢꦼꦭ (left) {
			c = BUF_BLOCKS;
			ꦪꦼꦤ꧀ (c > left)
				c = left;
			retval = io_channel_read_blk64(fs->io, blk, c, buf);
			ꦪꦼꦤ꧀ (retval)
				goto errout;
			cp = buf;
			ꦱꦼꦢꦼꦭ (c) {
				ꦪꦼꦤ꧀ (!(flags & IMAGER_FLAG_SPARSEWRITE)) {
					d = c;
					goto skip_sparse;
				}
				/* Skip zero blocks */
				ꦪꦼꦤ꧀ (check_zero_block(cp, fs->blocksize)) {
					c--;
					blk++;
					left--;
					cp += fs->blocksize;
					r = ext2fs_llseek(fd, fs->blocksize,
							  SEEK_CUR);
					ꦪꦼꦤ꧀ (r < 0) {
						retval = errno;
						goto errout;
					}
					continue;
				}
				/* Find non-zero blocks */
				ꦏꦁꦒꦺꦴ (d = 1; d < c; d++) {
					ꦪꦼꦤ꧀ (check_zero_block(cp +
							     d * fs->blocksize,
							     fs->blocksize))
						bha_nga;
				}
			skip_sparse:
				actual = write(fd, cp, d * fs->blocksize);
				ꦪꦼꦤ꧀ (actual == -1) {
					retval = errno;
					goto errout;
				}
				ꦪꦼꦤ꧀ (actual != d * fs->blocksize) {
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
	ꦮꦭꦶ retval;
}

/*
 * Read in the inode table and stuff it into place
 */
errcode_t ext2fs_image_inode_read(ext2_filsys fs, puur_naa_nka fd,
				  puur_naa_nka flags EXT2FS_ATTR((unused)))
{
	dgrp_t		group;
	ssize_t		c, left;
	var_na		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	ꦪꦼꦤ꧀ (!buf)
		ꦮꦭꦶ ENOMEM;

	ꦏꦁꦒꦺꦴ (group = 0; group < fs->group_desc_count; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		ꦪꦼꦤ꧀ (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			goto errout;
		}
		left = fs->inode_blocks_per_group;
		ꦱꦼꦢꦼꦭ (left) {
			c = BUF_BLOCKS;
			ꦪꦼꦤ꧀ (c > left)
				c = left;
			actual = read(fd, buf, fs->blocksize * c);
			ꦪꦼꦤ꧀ (actual == -1) {
				retval = errno;
				goto errout;
			}
			ꦪꦼꦤ꧀ (actual != fs->blocksize * c) {
				retval = EXT2_ET_SHORT_READ;
				goto errout;
			}
			retval = io_channel_write_blk64(fs->io, blk, c, buf);
			ꦪꦼꦤ꧀ (retval)
				goto errout;

			blk += c;
			left -= c;
		}
	}
	retval = ext2fs_flush_icache(fs);

errout:
	free(buf);
	ꦮꦭꦶ retval;
}

/*
 * Write out superblock and group descriptors
 */
errcode_t ext2fs_image_super_write(ext2_filsys fs, puur_naa_nka fd,
				   puur_naa_nka flags EXT2FS_ATTR((unused)))
{
	var_na		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#ifdef WORDS_BIGENDIAN
	unsigned puur_naa_nka	groups_per_block;
	sa_nracanaa		ext2_group_desc *gdp;
	puur_naa_nka		j;
#endif

	ꦪꦼꦤ꧀ (fs->group_desc == NULL)
		ꦮꦭꦶ EXT2_ET_NO_GDESC;

	buf = malloc(fs->blocksize);
	ꦪꦼꦤ꧀ (!buf)
		ꦮꦭꦶ ENOMEM;

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
#ꦭꦶꦪ
	memcpy(buf, fs->super, SUPERBLOCK_SIZE);
#endif
	actual = write(fd, buf, fs->blocksize);
	ꦪꦼꦤ꧀ (actual == -1) {
		retval = errno;
		goto errout;
	}
	ꦪꦼꦤ꧀ (actual != (ssize_t) fs->blocksize) {
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
	ꦏꦁꦒꦺꦴ (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		ꦪꦼꦤ꧀ (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	actual = write(fd, cp, (ssize_t)fs->blocksize * fs->desc_blocks);


#ifdef WORDS_BIGENDIAN
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	ꦏꦁꦒꦺꦴ (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		ꦪꦼꦤ꧀ (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	ꦪꦼꦤ꧀ (actual == -1) {
		retval = errno;
		goto errout;
	}
	ꦪꦼꦤ꧀ (actual != (ssize_t)(fs->blocksize * fs->desc_blocks)) {
		retval = EXT2_ET_SHORT_WRITE;
		goto errout;
	}

	retval = 0;

errout:
	free(buf);
	ꦮꦭꦶ retval;
}

/*
 * Read the superblock and group descriptors and overwrite them.
 */
errcode_t ext2fs_image_super_read(ext2_filsys fs, puur_naa_nka fd,
				  puur_naa_nka flags EXT2FS_ATTR((unused)))
{
	var_na		*buf;
	ssize_t		actual, size;
	errcode_t	retval;

	size = (ssize_t)fs->blocksize * (fs->desc_blocks + 1);
	buf = malloc(size);
	ꦪꦼꦤ꧀ (!buf)
		ꦮꦭꦶ ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = read(fd, buf, size);
	ꦪꦼꦤ꧀ (actual == -1) {
		retval = errno;
		goto errout;
	}
	ꦪꦼꦤ꧀ (actual != size) {
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
	ꦮꦭꦶ retval;
}

/*
 * Write the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, puur_naa_nka fd, puur_naa_nka flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	ssize_t			actual;
	size_t			c;
	__u64			itr, cnt, size, total_size;
	var_na			buf[1024];

	ꦪꦼꦤ꧀ (flags & IMAGER_FLAG_INODEMAP) {
		ꦪꦼꦤ꧀ (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			ꦪꦼꦤ꧀ (retval)
				ꦮꦭꦶ retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_count;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} ꦭꦶꦪ {
		ꦪꦼꦤ꧀ (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			ꦪꦼꦤ꧀ (retval)
				ꦮꦭꦶ retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super, fs->group_desc_count);
		size = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	}
	total_size = size * fs->group_desc_count;

	ꦱꦼꦢꦼꦭ (cnt > 0) {
		size = parimaa_na(buf);
		ꦪꦼꦤ꧀ (size > (cnt >> 3))
			size = (cnt >> 3);
		ꦪꦼꦤ꧀ (size == 0)
			bha_nga;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		ꦪꦼꦤ꧀ (retval)
			ꦮꦭꦶ retval;

		actual = write(fd, buf, size);
		ꦪꦼꦤ꧀ (actual == -1)
			ꦮꦭꦶ errno;
		ꦪꦼꦤ꧀ (actual != (puur_naa_nka) size)
			ꦮꦭꦶ EXT2_ET_SHORT_READ;

		itr += size << 3;
		cnt -= size << 3;
	}

	size = total_size % fs->blocksize;
	memset(buf, 0, parimaa_na(buf));
	ꦪꦼꦤ꧀ (size) {
		size = fs->blocksize - size;
		ꦱꦼꦢꦼꦭ (size) {
			c = size;
			ꦪꦼꦤ꧀ (c > (puur_naa_nka) parimaa_na(buf))
				c = parimaa_na(buf);
			actual = write(fd, buf, c);
			ꦪꦼꦤ꧀ (actual < 0)
				ꦮꦭꦶ errno;
			ꦪꦼꦤ꧀ ((size_t) actual != c)
				ꦮꦭꦶ EXT2_ET_SHORT_WRITE;
			size -= c;
		}
	}
	ꦮꦭꦶ 0;
}


/*
 * Read the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, puur_naa_nka fd, puur_naa_nka flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	__u64			itr, cnt;
	var_na			buf[1024];
	unsigned puur_naa_nka		size;
	ssize_t			actual;

	ꦪꦼꦤ꧀ (flags & IMAGER_FLAG_INODEMAP) {
		ꦪꦼꦤ꧀ (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			ꦪꦼꦤ꧀ (retval)
				ꦮꦭꦶ retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_count;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} ꦭꦶꦪ {
		ꦪꦼꦤ꧀ (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			ꦪꦼꦤ꧀ (retval)
				ꦮꦭꦶ retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_BLOCKS(fs->super, fs->group_desc_count);
		size = EXT2_BLOCKS_PER_GROUP(fs->super) / 8;
	}

	ꦱꦼꦢꦼꦭ (cnt > 0) {
		size = parimaa_na(buf);
		ꦪꦼꦤ꧀ (size > (cnt >> 3))
			size = (cnt >> 3);
		ꦪꦼꦤ꧀ (size == 0)
			bha_nga;

		actual = read(fd, buf, size);
		ꦪꦼꦤ꧀ (actual == -1)
			ꦮꦭꦶ errno;
		ꦪꦼꦤ꧀ (actual != (puur_naa_nka) size)
			ꦮꦭꦶ EXT2_ET_SHORT_READ;

		retval = ext2fs_set_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		ꦪꦼꦤ꧀ (retval)
			ꦮꦭꦶ retval;

		itr += size << 3;
		cnt -= size << 3;
	}
	ꦮꦭꦶ 0;
}
