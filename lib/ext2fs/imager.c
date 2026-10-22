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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#எனில் HAVE_ERRNO_H
#அந்தர்பாவ <errno.h>
#endif
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

#ifndef HAVE_TYPE_SSIZE_T
வகைவரை முழு ssize_t;
#endif

/*
 * This function returns 1 if the specified block is all zeros
 */
நிலையான முழு check_zero_block(எழுத்து *buf, முழு blocksize)
{
	எழுத்து	*cp = buf;
	முழு	இடது = blocksize;

	போது (இடது > 0) {
		எனில் (*cp++)
			திரும்பு 0;
		இடது--;
	}
	திரும்பு 1;
}

/*
 * Write the inode table out as a single block.
 */
#define BUF_BLOCKS	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, முழு fd, முழு flags)
{
	dgrp_t		group;
	ssize_t		இடது, c, d;
	எழுத்து		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	எனில் (!buf)
		திரும்பு ENOMEM;

	க்கு (group = 0; group < fs->group_desc_மொத்தம்; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		எனில் (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			செல் errout;
		}
		இடது = fs->inode_blocks_per_group;
		எனில் ((blk < fs->super->s_first_data_block) ||
		    (blk + இடது - 1 >= ext2fs_blocks_மொத்தம்(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
			செல் errout;
		}
		போது (இடது) {
			c = BUF_BLOCKS;
			எனில் (c > இடது)
				c = இடது;
			retval = io_channel_read_blk64(fs->io, blk, c, buf);
			எனில் (retval)
				செல் errout;
			cp = buf;
			போது (c) {
				எனில் (!(flags & IMAGER_FLAG_SPARSEWRITE)) {
					d = c;
					செல் skip_sparse;
				}
				/* Skip zero blocks */
				எனில் (check_zero_block(cp, fs->blocksize)) {
					c--;
					blk++;
					இடது--;
					cp += fs->blocksize;
					r = ext2fs_llseek(fd, fs->blocksize,
							  SEEK_CUR);
					எனில் (r < 0) {
						retval = errno;
						செல் errout;
					}
					தொடர்;
				}
				/* Find non-zero blocks */
				க்கு (d = 1; d < c; d++) {
					எனில் (check_zero_block(cp +
							     d * fs->blocksize,
							     fs->blocksize))
						நிறுத்து;
				}
			skip_sparse:
				actual = லிக(fd, cp, d * fs->blocksize);
				எனில் (actual == -1) {
					retval = errno;
					செல் errout;
				}
				எனில் (actual != d * fs->blocksize) {
					retval = EXT2_ET_SHORT_WRITE;
					செல் errout;
				}
				blk += d;
				இடது -= d;
				cp += d * fs->blocksize;
				c -= d;
			}
		}
	}
	retval = 0;

errout:
	விடுவி(buf);
	திரும்பு retval;
}

/*
 * Read in the inode table and stuff it into place
 */
errcode_t ext2fs_image_inode_read(ext2_filsys fs, முழு fd,
				  முழு flags EXT2FS_ATTR((unused)))
{
	dgrp_t		group;
	ssize_t		c, இடது;
	எழுத்து		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	எனில் (!buf)
		திரும்பு ENOMEM;

	க்கு (group = 0; group < fs->group_desc_மொத்தம்; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		எனில் (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			செல் errout;
		}
		இடது = fs->inode_blocks_per_group;
		போது (இடது) {
			c = BUF_BLOCKS;
			எனில் (c > இடது)
				c = இடது;
			actual = பட(fd, buf, fs->blocksize * c);
			எனில் (actual == -1) {
				retval = errno;
				செல் errout;
			}
			எனில் (actual != fs->blocksize * c) {
				retval = EXT2_ET_SHORT_READ;
				செல் errout;
			}
			retval = io_channel_write_blk64(fs->io, blk, c, buf);
			எனில் (retval)
				செல் errout;

			blk += c;
			இடது -= c;
		}
	}
	retval = ext2fs_flush_icache(fs);

errout:
	விடுவி(buf);
	திரும்பு retval;
}

/*
 * Write out superblock and group descriptors
 */
errcode_t ext2fs_image_super_write(ext2_filsys fs, முழு fd,
				   முழு flags EXT2FS_ATTR((unused)))
{
	எழுத்து		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#ifdef சொற்கள்_BIGENDIAN
	unsigned முழு	groups_per_block;
	கட்டமைப்பு		ext2_group_desc *gdp;
	முழு		j;
#endif

	எனில் (fs->group_desc == வெற்று)
		திரும்பு EXT2_ET_NO_GDESC;

	buf = malloc(fs->blocksize);
	எனில் (!buf)
		திரும்பு ENOMEM;

	/*
	 * Write out the superblock
	 */
	memset(buf, 0, fs->blocksize);
#ifdef சொற்கள்_BIGENDIAN
	/*
	 * We're writing out superblock so let's convert
	 * it to little endian and then back if needed
	 */
	ext2fs_swap_super(fs->super);
	memcpy(buf, fs->super, SUPERBLOCK_SIZE);
	ext2fs_swap_super(fs->super);
#இல்லையெனில்
	memcpy(buf, fs->super, SUPERBLOCK_SIZE);
#endif
	actual = லிக(fd, buf, fs->blocksize);
	எனில் (actual == -1) {
		retval = errno;
		செல் errout;
	}
	எனில் (actual != (ssize_t) fs->blocksize) {
		retval = EXT2_ET_SHORT_WRITE;
		செல் errout;
	}

	/*
	 * Now write out the block group descriptors
	 */

	cp = (எழுத்து *) fs->group_desc;

#ifdef சொற்கள்_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	க்கு (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		எனில் (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	actual = லிக(fd, cp, (ssize_t)fs->blocksize * fs->desc_blocks);


#ifdef சொற்கள்_BIGENDIAN
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	க்கு (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		எனில் (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	எனில் (actual == -1) {
		retval = errno;
		செல் errout;
	}
	எனில் (actual != (ssize_t)(fs->blocksize * fs->desc_blocks)) {
		retval = EXT2_ET_SHORT_WRITE;
		செல் errout;
	}

	retval = 0;

errout:
	விடுவி(buf);
	திரும்பு retval;
}

/*
 * Read the superblock and group descriptors and overwrite them.
 */
errcode_t ext2fs_image_super_read(ext2_filsys fs, முழு fd,
				  முழு flags EXT2FS_ATTR((unused)))
{
	எழுத்து		*buf;
	ssize_t		actual, size;
	errcode_t	retval;

	size = (ssize_t)fs->blocksize * (fs->desc_blocks + 1);
	buf = malloc(size);
	எனில் (!buf)
		திரும்பு ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = பட(fd, buf, size);
	எனில் (actual == -1) {
		retval = errno;
		செல் errout;
	}
	எனில் (actual != size) {
		retval = EXT2_ET_SHORT_READ;
		செல் errout;
	}

	/*
	 * Now copy in the superblock and group descriptors
	 */
	memcpy(fs->super, buf, SUPERBLOCK_SIZE);

	memcpy(fs->group_desc, buf + fs->blocksize,
	       (ssize_t)fs->blocksize * fs->desc_blocks);

	retval = 0;

errout:
	விடுவி(buf);
	திரும்பு retval;
}

/*
 * Write the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, முழு fd, முழு flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	ssize_t			actual;
	size_t			c;
	__u64			itr, cnt, size, total_size;
	எழுத்து			buf[1024];

	எனில் (flags & IMAGER_FLAG_INODEMAP) {
		எனில் (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			எனில் (retval)
				திரும்பு retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_மொத்தம்;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} இல்லையெனில் {
		எனில் (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			எனில் (retval)
				திரும்பு retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super, fs->group_desc_மொத்தம்);
		size = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	}
	total_size = size * fs->group_desc_மொத்தம்;

	போது (cnt > 0) {
		size = அளவு(buf);
		எனில் (size > (cnt >> 3))
			size = (cnt >> 3);
		எனில் (size == 0)
			நிறுத்து;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		எனில் (retval)
			திரும்பு retval;

		actual = லிக(fd, buf, size);
		எனில் (actual == -1)
			திரும்பு errno;
		எனில் (actual != (முழு) size)
			திரும்பு EXT2_ET_SHORT_READ;

		itr += size << 3;
		cnt -= size << 3;
	}

	size = total_size % fs->blocksize;
	memset(buf, 0, அளவு(buf));
	எனில் (size) {
		size = fs->blocksize - size;
		போது (size) {
			c = size;
			எனில் (c > (முழு) அளவு(buf))
				c = அளவு(buf);
			actual = லிக(fd, buf, c);
			எனில் (actual < 0)
				திரும்பு errno;
			எனில் ((size_t) actual != c)
				திரும்பு EXT2_ET_SHORT_WRITE;
			size -= c;
		}
	}
	திரும்பு 0;
}


/*
 * Read the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, முழு fd, முழு flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	__u64			itr, cnt;
	எழுத்து			buf[1024];
	unsigned முழு		size;
	ssize_t			actual;

	எனில் (flags & IMAGER_FLAG_INODEMAP) {
		எனில் (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			எனில் (retval)
				திரும்பு retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_மொத்தம்;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} இல்லையெனில் {
		எனில் (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			எனில் (retval)
				திரும்பு retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_BLOCKS(fs->super, fs->group_desc_மொத்தம்);
		size = EXT2_BLOCKS_PER_GROUP(fs->super) / 8;
	}

	போது (cnt > 0) {
		size = அளவு(buf);
		எனில் (size > (cnt >> 3))
			size = (cnt >> 3);
		எனில் (size == 0)
			நிறுத்து;

		actual = பட(fd, buf, size);
		எனில் (actual == -1)
			திரும்பு errno;
		எனில் (actual != (முழு) size)
			திரும்பு EXT2_ET_SHORT_READ;

		retval = ext2fs_set_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		எனில் (retval)
			திரும்பு retval;

		itr += size << 3;
		cnt -= size << 3;
	}
	திரும்பு 0;
}
