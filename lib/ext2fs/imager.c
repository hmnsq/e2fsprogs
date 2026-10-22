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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଯଦି HAVE_ERRNO_H
#ଅନ୍ତର୍ଭାଵ <errno.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

#ifndef HAVE_TYPE_SSIZE_T
ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା ssize_t;
#endif

/*
 * This function returns 1 if the specified block is all zeros
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_zero_block(ଅକ୍ଷର *buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା blocksize)
{
	ଅକ୍ଷର	*cp = buf;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	ବାମ = blocksize;

	ଯେତେବେଳେ (ବାମ > 0) {
		ଯଦି (*cp++)
			ଫେରନ୍ତୁ 0;
		ବାମ--;
	}
	ଫେରନ୍ତୁ 1;
}

/*
 * Write the inode table out as a single block.
 */
#define BUF_BLOCKS	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	dgrp_t		group;
	ssize_t		ବାମ, c, d;
	ଅକ୍ଷର		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	ଯଦି (!buf)
		ଫେରନ୍ତୁ ENOMEM;

	ପାଇଁ (group = 0; group < fs->group_desc_ଗଣନା; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		ଯଦି (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			ଯାଅ errout;
		}
		ବାମ = fs->inode_blocks_per_group;
		ଯଦି ((blk < fs->super->s_first_data_block) ||
		    (blk + ବାମ - 1 >= ext2fs_blocks_ଗଣନା(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
			ଯାଅ errout;
		}
		ଯେତେବେଳେ (ବାମ) {
			c = BUF_BLOCKS;
			ଯଦି (c > ବାମ)
				c = ବାମ;
			retval = io_channel_read_blk64(fs->io, blk, c, buf);
			ଯଦି (retval)
				ଯାଅ errout;
			cp = buf;
			ଯେତେବେଳେ (c) {
				ଯଦି (!(flags & IMAGER_FLAG_SPARSEWRITE)) {
					d = c;
					ଯାଅ skip_sparse;
				}
				/* Skip zero blocks */
				ଯଦି (check_zero_block(cp, fs->blocksize)) {
					c--;
					blk++;
					ବାମ--;
					cp += fs->blocksize;
					r = ext2fs_llseek(fd, fs->blocksize,
							  SEEK_CUR);
					ଯଦି (r < 0) {
						retval = errno;
						ଯାଅ errout;
					}
					ଜାରି;
				}
				/* Find non-zero blocks */
				ପାଇଁ (d = 1; d < c; d++) {
					ଯଦି (check_zero_block(cp +
							     d * fs->blocksize,
							     fs->blocksize))
						ଭାଙ୍ଗ;
				}
			skip_sparse:
				actual = ଲିଖ(fd, cp, d * fs->blocksize);
				ଯଦି (actual == -1) {
					retval = errno;
					ଯାଅ errout;
				}
				ଯଦି (actual != d * fs->blocksize) {
					retval = EXT2_ET_SHORT_WRITE;
					ଯାଅ errout;
				}
				blk += d;
				ବାମ -= d;
				cp += d * fs->blocksize;
				c -= d;
			}
		}
	}
	retval = 0;

errout:
	ମୁକ୍ତ(buf);
	ଫେରନ୍ତୁ retval;
}

/*
 * Read in the inode table and stuff it into place
 */
errcode_t ext2fs_image_inode_read(ext2_filsys fs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd,
				  ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags EXT2FS_ATTR((unused)))
{
	dgrp_t		group;
	ssize_t		c, ବାମ;
	ଅକ୍ଷର		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	ଯଦି (!buf)
		ଫେରନ୍ତୁ ENOMEM;

	ପାଇଁ (group = 0; group < fs->group_desc_ଗଣନା; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		ଯଦି (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			ଯାଅ errout;
		}
		ବାମ = fs->inode_blocks_per_group;
		ଯେତେବେଳେ (ବାମ) {
			c = BUF_BLOCKS;
			ଯଦି (c > ବାମ)
				c = ବାମ;
			actual = ପଠ(fd, buf, fs->blocksize * c);
			ଯଦି (actual == -1) {
				retval = errno;
				ଯାଅ errout;
			}
			ଯଦି (actual != fs->blocksize * c) {
				retval = EXT2_ET_SHORT_READ;
				ଯାଅ errout;
			}
			retval = io_channel_write_blk64(fs->io, blk, c, buf);
			ଯଦି (retval)
				ଯାଅ errout;

			blk += c;
			ବାମ -= c;
		}
	}
	retval = ext2fs_flush_icache(fs);

errout:
	ମୁକ୍ତ(buf);
	ଫେରନ୍ତୁ retval;
}

/*
 * Write out superblock and group descriptors
 */
errcode_t ext2fs_image_super_write(ext2_filsys fs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd,
				   ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags EXT2FS_ATTR((unused)))
{
	ଅକ୍ଷର		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	groups_per_block;
	ସଂରଚନା		ext2_group_desc *gdp;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		j;
#endif

	ଯଦି (fs->group_desc == ଖାଲି)
		ଫେରନ୍ତୁ EXT2_ET_NO_GDESC;

	buf = malloc(fs->blocksize);
	ଯଦି (!buf)
		ଫେରନ୍ତୁ ENOMEM;

	/*
	 * Write out the superblock
	 */
	memset(buf, 0, fs->blocksize);
#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	/*
	 * We're writing out superblock so let's convert
	 * it to little endian and then back if needed
	 */
	ext2fs_swap_super(fs->super);
	memcpy(buf, fs->super, SUPERBLOCK_SIZE);
	ext2fs_swap_super(fs->super);
#ଅନ୍ୟଥା
	memcpy(buf, fs->super, SUPERBLOCK_SIZE);
#endif
	actual = ଲିଖ(fd, buf, fs->blocksize);
	ଯଦି (actual == -1) {
		retval = errno;
		ଯାଅ errout;
	}
	ଯଦି (actual != (ssize_t) fs->blocksize) {
		retval = EXT2_ET_SHORT_WRITE;
		ଯାଅ errout;
	}

	/*
	 * Now write out the block group descriptors
	 */

	cp = (ଅକ୍ଷର *) fs->group_desc;

#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	ପାଇଁ (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		ଯଦି (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	actual = ଲିଖ(fd, cp, (ssize_t)fs->blocksize * fs->desc_blocks);


#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	ପାଇଁ (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		ଯଦି (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	ଯଦି (actual == -1) {
		retval = errno;
		ଯାଅ errout;
	}
	ଯଦି (actual != (ssize_t)(fs->blocksize * fs->desc_blocks)) {
		retval = EXT2_ET_SHORT_WRITE;
		ଯାଅ errout;
	}

	retval = 0;

errout:
	ମୁକ୍ତ(buf);
	ଫେରନ୍ତୁ retval;
}

/*
 * Read the superblock and group descriptors and overwrite them.
 */
errcode_t ext2fs_image_super_read(ext2_filsys fs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd,
				  ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags EXT2FS_ATTR((unused)))
{
	ଅକ୍ଷର		*buf;
	ssize_t		actual, size;
	errcode_t	retval;

	size = (ssize_t)fs->blocksize * (fs->desc_blocks + 1);
	buf = malloc(size);
	ଯଦି (!buf)
		ଫେରନ୍ତୁ ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = ପଠ(fd, buf, size);
	ଯଦି (actual == -1) {
		retval = errno;
		ଯାଅ errout;
	}
	ଯଦି (actual != size) {
		retval = EXT2_ET_SHORT_READ;
		ଯାଅ errout;
	}

	/*
	 * Now copy in the superblock and group descriptors
	 */
	memcpy(fs->super, buf, SUPERBLOCK_SIZE);

	memcpy(fs->group_desc, buf + fs->blocksize,
	       (ssize_t)fs->blocksize * fs->desc_blocks);

	retval = 0;

errout:
	ମୁକ୍ତ(buf);
	ଫେରନ୍ତୁ retval;
}

/*
 * Write the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	ssize_t			actual;
	size_t			c;
	__u64			itr, cnt, size, total_size;
	ଅକ୍ଷର			buf[1024];

	ଯଦି (flags & IMAGER_FLAG_INODEMAP) {
		ଯଦି (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			ଯଦି (retval)
				ଫେରନ୍ତୁ retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_ଗଣନା;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} ଅନ୍ୟଥା {
		ଯଦି (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			ଯଦି (retval)
				ଫେରନ୍ତୁ retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super, fs->group_desc_ଗଣନା);
		size = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	}
	total_size = size * fs->group_desc_ଗଣନା;

	ଯେତେବେଳେ (cnt > 0) {
		size = ଆକାର(buf);
		ଯଦି (size > (cnt >> 3))
			size = (cnt >> 3);
		ଯଦି (size == 0)
			ଭାଙ୍ଗ;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;

		actual = ଲିଖ(fd, buf, size);
		ଯଦି (actual == -1)
			ଫେରନ୍ତୁ errno;
		ଯଦି (actual != (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) size)
			ଫେରନ୍ତୁ EXT2_ET_SHORT_READ;

		itr += size << 3;
		cnt -= size << 3;
	}

	size = total_size % fs->blocksize;
	memset(buf, 0, ଆକାର(buf));
	ଯଦି (size) {
		size = fs->blocksize - size;
		ଯେତେବେଳେ (size) {
			c = size;
			ଯଦି (c > (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର(buf))
				c = ଆକାର(buf);
			actual = ଲିଖ(fd, buf, c);
			ଯଦି (actual < 0)
				ଫେରନ୍ତୁ errno;
			ଯଦି ((size_t) actual != c)
				ଫେରନ୍ତୁ EXT2_ET_SHORT_WRITE;
			size -= c;
		}
	}
	ଫେରନ୍ତୁ 0;
}


/*
 * Read the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	__u64			itr, cnt;
	ଅକ୍ଷର			buf[1024];
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		size;
	ssize_t			actual;

	ଯଦି (flags & IMAGER_FLAG_INODEMAP) {
		ଯଦି (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			ଯଦି (retval)
				ଫେରନ୍ତୁ retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_ଗଣନା;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} ଅନ୍ୟଥା {
		ଯଦି (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			ଯଦି (retval)
				ଫେରନ୍ତୁ retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_BLOCKS(fs->super, fs->group_desc_ଗଣନା);
		size = EXT2_BLOCKS_PER_GROUP(fs->super) / 8;
	}

	ଯେତେବେଳେ (cnt > 0) {
		size = ଆକାର(buf);
		ଯଦି (size > (cnt >> 3))
			size = (cnt >> 3);
		ଯଦି (size == 0)
			ଭାଙ୍ଗ;

		actual = ପଠ(fd, buf, size);
		ଯଦି (actual == -1)
			ଫେରନ୍ତୁ errno;
		ଯଦି (actual != (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) size)
			ଫେରନ୍ତୁ EXT2_ET_SHORT_READ;

		retval = ext2fs_set_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;

		itr += size << 3;
		cnt -= size << 3;
	}
	ଫେରନ୍ତୁ 0;
}
