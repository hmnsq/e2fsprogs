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
#만약 HAVE_UNISTD_H
#include <unistd.h>
#endif
#만약 HAVE_ERRNO_H
#include <errno.h>
#endif
#include <fcntl.h>
#include <time.h>
#만약 HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#만약 HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

#ifndef HAVE_TYPE_SSIZE_T
typedef 정수 ssize_t;
#endif

/*
 * This function returns 1 if the specified block is all zeros
 */
static 정수 check_zero_block(char *buf, 정수 blocksize)
{
	char	*cp = buf;
	정수	left = blocksize;

	동안 (left > 0) {
		만약 (*cp++)
			반환 0;
		left--;
	}
	반환 1;
}

/*
 * Write the inode table out as a single block.
 */
#define BUF_BLOCKS	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, 정수 fd, 정수 flags)
{
	dgrp_t		group;
	ssize_t		left, c, d;
	char		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	만약 (!buf)
		반환 ENOMEM;

	위해 (group = 0; group < fs->group_desc_count; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		만약 (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			goto errout;
		}
		left = fs->inode_blocks_per_group;
		만약 ((blk < fs->super->s_first_data_block) ||
		    (blk + left - 1 >= ext2fs_blocks_count(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
			goto errout;
		}
		동안 (left) {
			c = BUF_BLOCKS;
			만약 (c > left)
				c = left;
			retval = io_channel_read_blk64(fs->io, blk, c, buf);
			만약 (retval)
				goto errout;
			cp = buf;
			동안 (c) {
				만약 (!(flags & IMAGER_FLAG_SPARSEWRITE)) {
					d = c;
					goto skip_sparse;
				}
				/* Skip zero blocks */
				만약 (check_zero_block(cp, fs->blocksize)) {
					c--;
					blk++;
					left--;
					cp += fs->blocksize;
					r = ext2fs_llseek(fd, fs->blocksize,
							  SEEK_CUR);
					만약 (r < 0) {
						retval = errno;
						goto errout;
					}
					continue;
				}
				/* Find non-zero blocks */
				위해 (d = 1; d < c; d++) {
					만약 (check_zero_block(cp +
							     d * fs->blocksize,
							     fs->blocksize))
						break;
				}
			skip_sparse:
				actual = write(fd, cp, d * fs->blocksize);
				만약 (actual == -1) {
					retval = errno;
					goto errout;
				}
				만약 (actual != d * fs->blocksize) {
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
	반환 retval;
}

/*
 * Read in the inode table and stuff it into place
 */
errcode_t ext2fs_image_inode_read(ext2_filsys fs, 정수 fd,
				  정수 flags EXT2FS_ATTR((unused)))
{
	dgrp_t		group;
	ssize_t		c, left;
	char		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	만약 (!buf)
		반환 ENOMEM;

	위해 (group = 0; group < fs->group_desc_count; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		만약 (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			goto errout;
		}
		left = fs->inode_blocks_per_group;
		동안 (left) {
			c = BUF_BLOCKS;
			만약 (c > left)
				c = left;
			actual = read(fd, buf, fs->blocksize * c);
			만약 (actual == -1) {
				retval = errno;
				goto errout;
			}
			만약 (actual != fs->blocksize * c) {
				retval = EXT2_ET_SHORT_READ;
				goto errout;
			}
			retval = io_channel_write_blk64(fs->io, blk, c, buf);
			만약 (retval)
				goto errout;

			blk += c;
			left -= c;
		}
	}
	retval = ext2fs_flush_icache(fs);

errout:
	free(buf);
	반환 retval;
}

/*
 * Write out superblock and group descriptors
 */
errcode_t ext2fs_image_super_write(ext2_filsys fs, 정수 fd,
				   정수 flags EXT2FS_ATTR((unused)))
{
	char		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#ifdef WORDS_BIGENDIAN
	unsigned 정수	groups_per_block;
	struct		ext2_group_desc *gdp;
	정수		j;
#endif

	만약 (fs->group_desc == NULL)
		반환 EXT2_ET_NO_GDESC;

	buf = malloc(fs->blocksize);
	만약 (!buf)
		반환 ENOMEM;

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
#아니면
	memcpy(buf, fs->super, SUPERBLOCK_SIZE);
#endif
	actual = write(fd, buf, fs->blocksize);
	만약 (actual == -1) {
		retval = errno;
		goto errout;
	}
	만약 (actual != (ssize_t) fs->blocksize) {
		retval = EXT2_ET_SHORT_WRITE;
		goto errout;
	}

	/*
	 * Now write out the block group descriptors
	 */

	cp = (char *) fs->group_desc;

#ifdef WORDS_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	위해 (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		만약 (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	actual = write(fd, cp, (ssize_t)fs->blocksize * fs->desc_blocks);


#ifdef WORDS_BIGENDIAN
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	위해 (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		만약 (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	만약 (actual == -1) {
		retval = errno;
		goto errout;
	}
	만약 (actual != (ssize_t)(fs->blocksize * fs->desc_blocks)) {
		retval = EXT2_ET_SHORT_WRITE;
		goto errout;
	}

	retval = 0;

errout:
	free(buf);
	반환 retval;
}

/*
 * Read the superblock and group descriptors and overwrite them.
 */
errcode_t ext2fs_image_super_read(ext2_filsys fs, 정수 fd,
				  정수 flags EXT2FS_ATTR((unused)))
{
	char		*buf;
	ssize_t		actual, size;
	errcode_t	retval;

	size = (ssize_t)fs->blocksize * (fs->desc_blocks + 1);
	buf = malloc(size);
	만약 (!buf)
		반환 ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = read(fd, buf, size);
	만약 (actual == -1) {
		retval = errno;
		goto errout;
	}
	만약 (actual != size) {
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
	반환 retval;
}

/*
 * Write the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, 정수 fd, 정수 flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	ssize_t			actual;
	size_t			c;
	__u64			itr, cnt, size, total_size;
	char			buf[1024];

	만약 (flags & IMAGER_FLAG_INODEMAP) {
		만약 (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			만약 (retval)
				반환 retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_count;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} 아니면 {
		만약 (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			만약 (retval)
				반환 retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super, fs->group_desc_count);
		size = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	}
	total_size = size * fs->group_desc_count;

	동안 (cnt > 0) {
		size = sizeof(buf);
		만약 (size > (cnt >> 3))
			size = (cnt >> 3);
		만약 (size == 0)
			break;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		만약 (retval)
			반환 retval;

		actual = write(fd, buf, size);
		만약 (actual == -1)
			반환 errno;
		만약 (actual != (정수) size)
			반환 EXT2_ET_SHORT_READ;

		itr += size << 3;
		cnt -= size << 3;
	}

	size = total_size % fs->blocksize;
	memset(buf, 0, sizeof(buf));
	만약 (size) {
		size = fs->blocksize - size;
		동안 (size) {
			c = size;
			만약 (c > (정수) sizeof(buf))
				c = sizeof(buf);
			actual = write(fd, buf, c);
			만약 (actual < 0)
				반환 errno;
			만약 ((size_t) actual != c)
				반환 EXT2_ET_SHORT_WRITE;
			size -= c;
		}
	}
	반환 0;
}


/*
 * Read the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, 정수 fd, 정수 flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	__u64			itr, cnt;
	char			buf[1024];
	unsigned 정수		size;
	ssize_t			actual;

	만약 (flags & IMAGER_FLAG_INODEMAP) {
		만약 (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			만약 (retval)
				반환 retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_count;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} 아니면 {
		만약 (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			만약 (retval)
				반환 retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_BLOCKS(fs->super, fs->group_desc_count);
		size = EXT2_BLOCKS_PER_GROUP(fs->super) / 8;
	}

	동안 (cnt > 0) {
		size = sizeof(buf);
		만약 (size > (cnt >> 3))
			size = (cnt >> 3);
		만약 (size == 0)
			break;

		actual = read(fd, buf, size);
		만약 (actual == -1)
			반환 errno;
		만약 (actual != (정수) size)
			반환 EXT2_ET_SHORT_READ;

		retval = ext2fs_set_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		만약 (retval)
			반환 retval;

		itr += size << 3;
		cnt -= size << 3;
	}
	반환 0;
}
