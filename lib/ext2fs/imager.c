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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಯದಿ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#endif
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಯದಿ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#endif
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

#ifndef HAVE_TYPE_SSIZE_T
ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ ssize_t;
#endif

/*
 * This function returns 1 if the specified block is all zeros
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_zero_block(ವರ್ಣ *buf, ಪೂರ್ಣಾಙ್ಕ blocksize)
{
	ವರ್ಣ	*cp = buf;
	ಪೂರ್ಣಾಙ್ಕ	ಎಡ = blocksize;

	ಯಾವತ್ (ಎಡ > 0) {
		ಯದಿ (*cp++)
			ಪ್ರತ್ಯಾಗಮ 0;
		ಎಡ--;
	}
	ಪ್ರತ್ಯಾಗಮ 1;
}

/*
 * Write the inode table out as a single block.
 */
#define BUF_BLOCKS	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ fd, ಪೂರ್ಣಾಙ್ಕ flags)
{
	dgrp_t		group;
	ssize_t		ಎಡ, c, d;
	ವರ್ಣ		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	ಯದಿ (!buf)
		ಪ್ರತ್ಯಾಗಮ ENOMEM;

	ಪ್ರತಿ (group = 0; group < fs->group_desc_ಎಣಿಕೆ; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		ಯದಿ (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			ಗಚ್ಛ errout;
		}
		ಎಡ = fs->inode_blocks_per_group;
		ಯದಿ ((blk < fs->super->s_first_data_block) ||
		    (blk + ಎಡ - 1 >= ext2fs_blocks_ಎಣಿಕೆ(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
			ಗಚ್ಛ errout;
		}
		ಯಾವತ್ (ಎಡ) {
			c = BUF_BLOCKS;
			ಯದಿ (c > ಎಡ)
				c = ಎಡ;
			retval = io_channel_read_blk64(fs->io, blk, c, buf);
			ಯದಿ (retval)
				ಗಚ್ಛ errout;
			cp = buf;
			ಯಾವತ್ (c) {
				ಯದಿ (!(flags & IMAGER_FLAG_SPARSEWRITE)) {
					d = c;
					ಗಚ್ಛ skip_sparse;
				}
				/* Skip zero blocks */
				ಯದಿ (check_zero_block(cp, fs->blocksize)) {
					c--;
					blk++;
					ಎಡ--;
					cp += fs->blocksize;
					r = ext2fs_llseek(fd, fs->blocksize,
							  SEEK_CUR);
					ಯದಿ (r < 0) {
						retval = errno;
						ಗಚ್ಛ errout;
					}
					ಅನುವರ್ತ;
				}
				/* Find non-zero blocks */
				ಪ್ರತಿ (d = 1; d < c; d++) {
					ಯದಿ (check_zero_block(cp +
							     d * fs->blocksize,
							     fs->blocksize))
						ಭಙ್ಗ;
				}
			skip_sparse:
				actual = ಲಿಖ(fd, cp, d * fs->blocksize);
				ಯದಿ (actual == -1) {
					retval = errno;
					ಗಚ್ಛ errout;
				}
				ಯದಿ (actual != d * fs->blocksize) {
					retval = EXT2_ET_SHORT_WRITE;
					ಗಚ್ಛ errout;
				}
				blk += d;
				ಎಡ -= d;
				cp += d * fs->blocksize;
				c -= d;
			}
		}
	}
	retval = 0;

errout:
	ಮುಕ್ತ(buf);
	ಪ್ರತ್ಯಾಗಮ retval;
}

/*
 * Read in the inode table and stuff it into place
 */
errcode_t ext2fs_image_inode_read(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ fd,
				  ಪೂರ್ಣಾಙ್ಕ flags EXT2FS_ATTR((unused)))
{
	dgrp_t		group;
	ssize_t		c, ಎಡ;
	ವರ್ಣ		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	ಯದಿ (!buf)
		ಪ್ರತ್ಯಾಗಮ ENOMEM;

	ಪ್ರತಿ (group = 0; group < fs->group_desc_ಎಣಿಕೆ; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		ಯದಿ (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			ಗಚ್ಛ errout;
		}
		ಎಡ = fs->inode_blocks_per_group;
		ಯಾವತ್ (ಎಡ) {
			c = BUF_BLOCKS;
			ಯದಿ (c > ಎಡ)
				c = ಎಡ;
			actual = ಪಠ(fd, buf, fs->blocksize * c);
			ಯದಿ (actual == -1) {
				retval = errno;
				ಗಚ್ಛ errout;
			}
			ಯದಿ (actual != fs->blocksize * c) {
				retval = EXT2_ET_SHORT_READ;
				ಗಚ್ಛ errout;
			}
			retval = io_channel_write_blk64(fs->io, blk, c, buf);
			ಯದಿ (retval)
				ಗಚ್ಛ errout;

			blk += c;
			ಎಡ -= c;
		}
	}
	retval = ext2fs_flush_icache(fs);

errout:
	ಮುಕ್ತ(buf);
	ಪ್ರತ್ಯಾಗಮ retval;
}

/*
 * Write out superblock and group descriptors
 */
errcode_t ext2fs_image_super_write(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ fd,
				   ಪೂರ್ಣಾಙ್ಕ flags EXT2FS_ATTR((unused)))
{
	ವರ್ಣ		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#ifdef ಪದಗಳು_BIGENDIAN
	unsigned ಪೂರ್ಣಾಙ್ಕ	groups_per_block;
	ಸಂರಚನಾ		ext2_group_desc *gdp;
	ಪೂರ್ಣಾಙ್ಕ		j;
#endif

	ಯದಿ (fs->group_desc == ರಿಕ್ತ)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_NO_GDESC;

	buf = malloc(fs->blocksize);
	ಯದಿ (!buf)
		ಪ್ರತ್ಯಾಗಮ ENOMEM;

	/*
	 * Write out the superblock
	 */
	memset(buf, 0, fs->blocksize);
#ifdef ಪದಗಳು_BIGENDIAN
	/*
	 * We're writing out superblock so let's convert
	 * it to little endian and then back if needed
	 */
	ext2fs_swap_super(fs->super);
	memcpy(buf, fs->super, SUPERBLOCK_SIZE);
	ext2fs_swap_super(fs->super);
#ಅನ್ಯಥಾ
	memcpy(buf, fs->super, SUPERBLOCK_SIZE);
#endif
	actual = ಲಿಖ(fd, buf, fs->blocksize);
	ಯದಿ (actual == -1) {
		retval = errno;
		ಗಚ್ಛ errout;
	}
	ಯದಿ (actual != (ssize_t) fs->blocksize) {
		retval = EXT2_ET_SHORT_WRITE;
		ಗಚ್ಛ errout;
	}

	/*
	 * Now write out the block group descriptors
	 */

	cp = (ವರ್ಣ *) fs->group_desc;

#ifdef ಪದಗಳು_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	ಪ್ರತಿ (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		ಯದಿ (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	actual = ಲಿಖ(fd, cp, (ssize_t)fs->blocksize * fs->desc_blocks);


#ifdef ಪದಗಳು_BIGENDIAN
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	ಪ್ರತಿ (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		ಯದಿ (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	ಯದಿ (actual == -1) {
		retval = errno;
		ಗಚ್ಛ errout;
	}
	ಯದಿ (actual != (ssize_t)(fs->blocksize * fs->desc_blocks)) {
		retval = EXT2_ET_SHORT_WRITE;
		ಗಚ್ಛ errout;
	}

	retval = 0;

errout:
	ಮುಕ್ತ(buf);
	ಪ್ರತ್ಯಾಗಮ retval;
}

/*
 * Read the superblock and group descriptors and overwrite them.
 */
errcode_t ext2fs_image_super_read(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ fd,
				  ಪೂರ್ಣಾಙ್ಕ flags EXT2FS_ATTR((unused)))
{
	ವರ್ಣ		*buf;
	ssize_t		actual, size;
	errcode_t	retval;

	size = (ssize_t)fs->blocksize * (fs->desc_blocks + 1);
	buf = malloc(size);
	ಯದಿ (!buf)
		ಪ್ರತ್ಯಾಗಮ ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = ಪಠ(fd, buf, size);
	ಯದಿ (actual == -1) {
		retval = errno;
		ಗಚ್ಛ errout;
	}
	ಯದಿ (actual != size) {
		retval = EXT2_ET_SHORT_READ;
		ಗಚ್ಛ errout;
	}

	/*
	 * Now copy in the superblock and group descriptors
	 */
	memcpy(fs->super, buf, SUPERBLOCK_SIZE);

	memcpy(fs->group_desc, buf + fs->blocksize,
	       (ssize_t)fs->blocksize * fs->desc_blocks);

	retval = 0;

errout:
	ಮುಕ್ತ(buf);
	ಪ್ರತ್ಯಾಗಮ retval;
}

/*
 * Write the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ fd, ಪೂರ್ಣಾಙ್ಕ flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	ssize_t			actual;
	size_t			c;
	__u64			itr, cnt, size, total_size;
	ವರ್ಣ			buf[1024];

	ಯದಿ (flags & IMAGER_FLAG_INODEMAP) {
		ಯದಿ (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			ಯದಿ (retval)
				ಪ್ರತ್ಯಾಗಮ retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_ಎಣಿಕೆ;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} ಅನ್ಯಥಾ {
		ಯದಿ (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			ಯದಿ (retval)
				ಪ್ರತ್ಯಾಗಮ retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super, fs->group_desc_ಎಣಿಕೆ);
		size = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	}
	total_size = size * fs->group_desc_ಎಣಿಕೆ;

	ಯಾವತ್ (cnt > 0) {
		size = ಪರಿಮಾಣ(buf);
		ಯದಿ (size > (cnt >> 3))
			size = (cnt >> 3);
		ಯದಿ (size == 0)
			ಭಙ್ಗ;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;

		actual = ಲಿಖ(fd, buf, size);
		ಯದಿ (actual == -1)
			ಪ್ರತ್ಯಾಗಮ errno;
		ಯದಿ (actual != (ಪೂರ್ಣಾಙ್ಕ) size)
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_SHORT_READ;

		itr += size << 3;
		cnt -= size << 3;
	}

	size = total_size % fs->blocksize;
	memset(buf, 0, ಪರಿಮಾಣ(buf));
	ಯದಿ (size) {
		size = fs->blocksize - size;
		ಯಾವತ್ (size) {
			c = size;
			ಯದಿ (c > (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ(buf))
				c = ಪರಿಮಾಣ(buf);
			actual = ಲಿಖ(fd, buf, c);
			ಯದಿ (actual < 0)
				ಪ್ರತ್ಯಾಗಮ errno;
			ಯದಿ ((size_t) actual != c)
				ಪ್ರತ್ಯಾಗಮ EXT2_ET_SHORT_WRITE;
			size -= c;
		}
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}


/*
 * Read the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, ಪೂರ್ಣಾಙ್ಕ fd, ಪೂರ್ಣಾಙ್ಕ flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	__u64			itr, cnt;
	ವರ್ಣ			buf[1024];
	unsigned ಪೂರ್ಣಾಙ್ಕ		size;
	ssize_t			actual;

	ಯದಿ (flags & IMAGER_FLAG_INODEMAP) {
		ಯದಿ (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			ಯದಿ (retval)
				ಪ್ರತ್ಯಾಗಮ retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_ಎಣಿಕೆ;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} ಅನ್ಯಥಾ {
		ಯದಿ (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			ಯದಿ (retval)
				ಪ್ರತ್ಯಾಗಮ retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_BLOCKS(fs->super, fs->group_desc_ಎಣಿಕೆ);
		size = EXT2_BLOCKS_PER_GROUP(fs->super) / 8;
	}

	ಯಾವತ್ (cnt > 0) {
		size = ಪರಿಮಾಣ(buf);
		ಯದಿ (size > (cnt >> 3))
			size = (cnt >> 3);
		ಯದಿ (size == 0)
			ಭಙ್ಗ;

		actual = ಪಠ(fd, buf, size);
		ಯದಿ (actual == -1)
			ಪ್ರತ್ಯಾಗಮ errno;
		ಯದಿ (actual != (ಪೂರ್ಣಾಙ್ಕ) size)
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_SHORT_READ;

		retval = ext2fs_set_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;

		itr += size << 3;
		cnt -= size << 3;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}
