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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#જો HAVE_ERRNO_H
#અન્તર્ભાવ <errno.h>
#endif
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <time.h>
#જો HAVE_SYS_STAT_H
#અન્તર્ભાવ <sys/stat.h>
#endif
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"

#ifndef HAVE_TYPE_SSIZE_T
પ્રકારવ્યાખ્યા પૂર્ણાંક ssize_t;
#endif

/*
 * This function returns 1 if the specified block is all zeros
 */
સ્થિર પૂર્ણાંક check_zero_block(અક્ષર *buf, પૂર્ણાંક blocksize)
{
	અક્ષર	*cp = buf;
	પૂર્ણાંક	ડાબું = blocksize;

	જ્યારે (ડાબું > 0) {
		જો (*cp++)
			પાછા 0;
		ડાબું--;
	}
	પાછા 1;
}

/*
 * Write the inode table out as a single block.
 */
#define BUF_BLOCKS	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, પૂર્ણાંક fd, પૂર્ણાંક flags)
{
	dgrp_t		group;
	ssize_t		ડાબું, c, d;
	અક્ષર		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	જો (!buf)
		પાછા ENOMEM;

	માટે (group = 0; group < fs->group_desc_ગણતરી; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		જો (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			જાઓ errout;
		}
		ડાબું = fs->inode_blocks_per_group;
		જો ((blk < fs->super->s_first_data_block) ||
		    (blk + ડાબું - 1 >= ext2fs_blocks_ગણતરી(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
			જાઓ errout;
		}
		જ્યારે (ડાબું) {
			c = BUF_BLOCKS;
			જો (c > ડાબું)
				c = ડાબું;
			retval = io_channel_read_blk64(fs->io, blk, c, buf);
			જો (retval)
				જાઓ errout;
			cp = buf;
			જ્યારે (c) {
				જો (!(flags & IMAGER_FLAG_SPARSEWRITE)) {
					d = c;
					જાઓ skip_sparse;
				}
				/* Skip zero blocks */
				જો (check_zero_block(cp, fs->blocksize)) {
					c--;
					blk++;
					ડાબું--;
					cp += fs->blocksize;
					r = ext2fs_llseek(fd, fs->blocksize,
							  SEEK_CUR);
					જો (r < 0) {
						retval = errno;
						જાઓ errout;
					}
					ચાલુ;
				}
				/* Find non-zero blocks */
				માટે (d = 1; d < c; d++) {
					જો (check_zero_block(cp +
							     d * fs->blocksize,
							     fs->blocksize))
						તોડો;
				}
			skip_sparse:
				actual = લિખ(fd, cp, d * fs->blocksize);
				જો (actual == -1) {
					retval = errno;
					જાઓ errout;
				}
				જો (actual != d * fs->blocksize) {
					retval = EXT2_ET_SHORT_WRITE;
					જાઓ errout;
				}
				blk += d;
				ડાબું -= d;
				cp += d * fs->blocksize;
				c -= d;
			}
		}
	}
	retval = 0;

errout:
	મુક્ત(buf);
	પાછા retval;
}

/*
 * Read in the inode table and stuff it into place
 */
errcode_t ext2fs_image_inode_read(ext2_filsys fs, પૂર્ણાંક fd,
				  પૂર્ણાંક flags EXT2FS_ATTR((unused)))
{
	dgrp_t		group;
	ssize_t		c, ડાબું;
	અક્ષર		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	જો (!buf)
		પાછા ENOMEM;

	માટે (group = 0; group < fs->group_desc_ગણતરી; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		જો (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			જાઓ errout;
		}
		ડાબું = fs->inode_blocks_per_group;
		જ્યારે (ડાબું) {
			c = BUF_BLOCKS;
			જો (c > ડાબું)
				c = ડાબું;
			actual = પઠ(fd, buf, fs->blocksize * c);
			જો (actual == -1) {
				retval = errno;
				જાઓ errout;
			}
			જો (actual != fs->blocksize * c) {
				retval = EXT2_ET_SHORT_READ;
				જાઓ errout;
			}
			retval = io_channel_write_blk64(fs->io, blk, c, buf);
			જો (retval)
				જાઓ errout;

			blk += c;
			ડાબું -= c;
		}
	}
	retval = ext2fs_flush_icache(fs);

errout:
	મુક્ત(buf);
	પાછા retval;
}

/*
 * Write out superblock and group descriptors
 */
errcode_t ext2fs_image_super_write(ext2_filsys fs, પૂર્ણાંક fd,
				   પૂર્ણાંક flags EXT2FS_ATTR((unused)))
{
	અક્ષર		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#ifdef શબ્દો_BIGENDIAN
	unsigned પૂર્ણાંક	groups_per_block;
	સંરચના		ext2_group_desc *gdp;
	પૂર્ણાંક		j;
#endif

	જો (fs->group_desc == ખાલી)
		પાછા EXT2_ET_NO_GDESC;

	buf = malloc(fs->blocksize);
	જો (!buf)
		પાછા ENOMEM;

	/*
	 * Write out the superblock
	 */
	memset(buf, 0, fs->blocksize);
#ifdef શબ્દો_BIGENDIAN
	/*
	 * We're writing out superblock so let's convert
	 * it to little endian and then back if needed
	 */
	ext2fs_swap_super(fs->super);
	memcpy(buf, fs->super, SUPERBLOCK_SIZE);
	ext2fs_swap_super(fs->super);
#નહીંતર
	memcpy(buf, fs->super, SUPERBLOCK_SIZE);
#endif
	actual = લિખ(fd, buf, fs->blocksize);
	જો (actual == -1) {
		retval = errno;
		જાઓ errout;
	}
	જો (actual != (ssize_t) fs->blocksize) {
		retval = EXT2_ET_SHORT_WRITE;
		જાઓ errout;
	}

	/*
	 * Now write out the block group descriptors
	 */

	cp = (અક્ષર *) fs->group_desc;

#ifdef શબ્દો_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	માટે (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		જો (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	actual = લિખ(fd, cp, (ssize_t)fs->blocksize * fs->desc_blocks);


#ifdef શબ્દો_BIGENDIAN
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	માટે (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		જો (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	જો (actual == -1) {
		retval = errno;
		જાઓ errout;
	}
	જો (actual != (ssize_t)(fs->blocksize * fs->desc_blocks)) {
		retval = EXT2_ET_SHORT_WRITE;
		જાઓ errout;
	}

	retval = 0;

errout:
	મુક્ત(buf);
	પાછા retval;
}

/*
 * Read the superblock and group descriptors and overwrite them.
 */
errcode_t ext2fs_image_super_read(ext2_filsys fs, પૂર્ણાંક fd,
				  પૂર્ણાંક flags EXT2FS_ATTR((unused)))
{
	અક્ષર		*buf;
	ssize_t		actual, size;
	errcode_t	retval;

	size = (ssize_t)fs->blocksize * (fs->desc_blocks + 1);
	buf = malloc(size);
	જો (!buf)
		પાછા ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = પઠ(fd, buf, size);
	જો (actual == -1) {
		retval = errno;
		જાઓ errout;
	}
	જો (actual != size) {
		retval = EXT2_ET_SHORT_READ;
		જાઓ errout;
	}

	/*
	 * Now copy in the superblock and group descriptors
	 */
	memcpy(fs->super, buf, SUPERBLOCK_SIZE);

	memcpy(fs->group_desc, buf + fs->blocksize,
	       (ssize_t)fs->blocksize * fs->desc_blocks);

	retval = 0;

errout:
	મુક્ત(buf);
	પાછા retval;
}

/*
 * Write the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, પૂર્ણાંક fd, પૂર્ણાંક flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	ssize_t			actual;
	size_t			c;
	__u64			itr, cnt, size, total_size;
	અક્ષર			buf[1024];

	જો (flags & IMAGER_FLAG_INODEMAP) {
		જો (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			જો (retval)
				પાછા retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_ગણતરી;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} નહીંતર {
		જો (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			જો (retval)
				પાછા retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super, fs->group_desc_ગણતરી);
		size = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	}
	total_size = size * fs->group_desc_ગણતરી;

	જ્યારે (cnt > 0) {
		size = કદ(buf);
		જો (size > (cnt >> 3))
			size = (cnt >> 3);
		જો (size == 0)
			તોડો;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		જો (retval)
			પાછા retval;

		actual = લિખ(fd, buf, size);
		જો (actual == -1)
			પાછા errno;
		જો (actual != (પૂર્ણાંક) size)
			પાછા EXT2_ET_SHORT_READ;

		itr += size << 3;
		cnt -= size << 3;
	}

	size = total_size % fs->blocksize;
	memset(buf, 0, કદ(buf));
	જો (size) {
		size = fs->blocksize - size;
		જ્યારે (size) {
			c = size;
			જો (c > (પૂર્ણાંક) કદ(buf))
				c = કદ(buf);
			actual = લિખ(fd, buf, c);
			જો (actual < 0)
				પાછા errno;
			જો ((size_t) actual != c)
				પાછા EXT2_ET_SHORT_WRITE;
			size -= c;
		}
	}
	પાછા 0;
}


/*
 * Read the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, પૂર્ણાંક fd, પૂર્ણાંક flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	__u64			itr, cnt;
	અક્ષર			buf[1024];
	unsigned પૂર્ણાંક		size;
	ssize_t			actual;

	જો (flags & IMAGER_FLAG_INODEMAP) {
		જો (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			જો (retval)
				પાછા retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_ગણતરી;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} નહીંતર {
		જો (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			જો (retval)
				પાછા retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_BLOCKS(fs->super, fs->group_desc_ગણતરી);
		size = EXT2_BLOCKS_PER_GROUP(fs->super) / 8;
	}

	જ્યારે (cnt > 0) {
		size = કદ(buf);
		જો (size > (cnt >> 3))
			size = (cnt >> 3);
		જો (size == 0)
			તોડો;

		actual = પઠ(fd, buf, size);
		જો (actual == -1)
			પાછા errno;
		જો (actual != (પૂર્ણાંક) size)
			પાછા EXT2_ET_SHORT_READ;

		retval = ext2fs_set_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		જો (retval)
			પાછા retval;

		itr += size << 3;
		cnt -= size << 3;
	}
	પાછા 0;
}
