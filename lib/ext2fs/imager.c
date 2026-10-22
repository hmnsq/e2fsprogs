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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#എങ്കിൽ HAVE_ERRNO_H
#അന്തര്ഭാവ <errno.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <time.h>
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

#ifndef HAVE_TYPE_SSIZE_T
തരനിർവചനം പൂർണ്ണസംഖ്യ ssize_t;
#endif

/*
 * This function returns 1 if the specified block is all zeros
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ check_zero_block(അക്ഷരം *buf, പൂർണ്ണസംഖ്യ blocksize)
{
	അക്ഷരം	*cp = buf;
	പൂർണ്ണസംഖ്യ	ഇടത് = blocksize;

	ആകുമ്പോൾ (ഇടത് > 0) {
		എങ്കിൽ (*cp++)
			തിരികെ 0;
		ഇടത്--;
	}
	തിരികെ 1;
}

/*
 * Write the inode table out as a single block.
 */
#define BUF_BLOCKS	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ flags)
{
	dgrp_t		group;
	ssize_t		ഇടത്, c, d;
	അക്ഷരം		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	എങ്കിൽ (!buf)
		തിരികെ ENOMEM;

	വേണ്ടി (group = 0; group < fs->group_desc_എണ്ണം; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		എങ്കിൽ (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			പോകുക errout;
		}
		ഇടത് = fs->inode_blocks_per_group;
		എങ്കിൽ ((blk < fs->super->s_first_data_block) ||
		    (blk + ഇടത് - 1 >= ext2fs_blocks_എണ്ണം(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
			പോകുക errout;
		}
		ആകുമ്പോൾ (ഇടത്) {
			c = BUF_BLOCKS;
			എങ്കിൽ (c > ഇടത്)
				c = ഇടത്;
			retval = io_channel_read_blk64(fs->io, blk, c, buf);
			എങ്കിൽ (retval)
				പോകുക errout;
			cp = buf;
			ആകുമ്പോൾ (c) {
				എങ്കിൽ (!(flags & IMAGER_FLAG_SPARSEWRITE)) {
					d = c;
					പോകുക skip_sparse;
				}
				/* Skip zero blocks */
				എങ്കിൽ (check_zero_block(cp, fs->blocksize)) {
					c--;
					blk++;
					ഇടത്--;
					cp += fs->blocksize;
					r = ext2fs_llseek(fd, fs->blocksize,
							  SEEK_CUR);
					എങ്കിൽ (r < 0) {
						retval = errno;
						പോകുക errout;
					}
					തുടരുക;
				}
				/* Find non-zero blocks */
				വേണ്ടി (d = 1; d < c; d++) {
					എങ്കിൽ (check_zero_block(cp +
							     d * fs->blocksize,
							     fs->blocksize))
						നിർത്തുക;
				}
			skip_sparse:
				actual = ലിഖ(fd, cp, d * fs->blocksize);
				എങ്കിൽ (actual == -1) {
					retval = errno;
					പോകുക errout;
				}
				എങ്കിൽ (actual != d * fs->blocksize) {
					retval = EXT2_ET_SHORT_WRITE;
					പോകുക errout;
				}
				blk += d;
				ഇടത് -= d;
				cp += d * fs->blocksize;
				c -= d;
			}
		}
	}
	retval = 0;

errout:
	സ്വതന്ത്രം(buf);
	തിരികെ retval;
}

/*
 * Read in the inode table and stuff it into place
 */
errcode_t ext2fs_image_inode_read(ext2_filsys fs, പൂർണ്ണസംഖ്യ fd,
				  പൂർണ്ണസംഖ്യ flags EXT2FS_ATTR((unused)))
{
	dgrp_t		group;
	ssize_t		c, ഇടത്;
	അക്ഷരം		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = malloc(fs->blocksize * BUF_BLOCKS);
	എങ്കിൽ (!buf)
		തിരികെ ENOMEM;

	വേണ്ടി (group = 0; group < fs->group_desc_എണ്ണം; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		എങ്കിൽ (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			പോകുക errout;
		}
		ഇടത് = fs->inode_blocks_per_group;
		ആകുമ്പോൾ (ഇടത്) {
			c = BUF_BLOCKS;
			എങ്കിൽ (c > ഇടത്)
				c = ഇടത്;
			actual = പഠ(fd, buf, fs->blocksize * c);
			എങ്കിൽ (actual == -1) {
				retval = errno;
				പോകുക errout;
			}
			എങ്കിൽ (actual != fs->blocksize * c) {
				retval = EXT2_ET_SHORT_READ;
				പോകുക errout;
			}
			retval = io_channel_write_blk64(fs->io, blk, c, buf);
			എങ്കിൽ (retval)
				പോകുക errout;

			blk += c;
			ഇടത് -= c;
		}
	}
	retval = ext2fs_flush_icache(fs);

errout:
	സ്വതന്ത്രം(buf);
	തിരികെ retval;
}

/*
 * Write out superblock and group descriptors
 */
errcode_t ext2fs_image_super_write(ext2_filsys fs, പൂർണ്ണസംഖ്യ fd,
				   പൂർണ്ണസംഖ്യ flags EXT2FS_ATTR((unused)))
{
	അക്ഷരം		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#ifdef വാക്കുകൾ_BIGENDIAN
	unsigned പൂർണ്ണസംഖ്യ	groups_per_block;
	ഘടന		ext2_group_desc *gdp;
	പൂർണ്ണസംഖ്യ		j;
#endif

	എങ്കിൽ (fs->group_desc == ശൂന്യ)
		തിരികെ EXT2_ET_NO_GDESC;

	buf = malloc(fs->blocksize);
	എങ്കിൽ (!buf)
		തിരികെ ENOMEM;

	/*
	 * Write out the superblock
	 */
	memset(buf, 0, fs->blocksize);
#ifdef വാക്കുകൾ_BIGENDIAN
	/*
	 * We're writing out superblock so let's convert
	 * it to little endian and then back if needed
	 */
	ext2fs_swap_super(fs->super);
	memcpy(buf, fs->super, SUPERBLOCK_SIZE);
	ext2fs_swap_super(fs->super);
#അല്ലെങ്കിൽ
	memcpy(buf, fs->super, SUPERBLOCK_SIZE);
#endif
	actual = ലിഖ(fd, buf, fs->blocksize);
	എങ്കിൽ (actual == -1) {
		retval = errno;
		പോകുക errout;
	}
	എങ്കിൽ (actual != (ssize_t) fs->blocksize) {
		retval = EXT2_ET_SHORT_WRITE;
		പോകുക errout;
	}

	/*
	 * Now write out the block group descriptors
	 */

	cp = (അക്ഷരം *) fs->group_desc;

#ifdef വാക്കുകൾ_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	വേണ്ടി (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		എങ്കിൽ (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	actual = ലിഖ(fd, cp, (ssize_t)fs->blocksize * fs->desc_blocks);


#ifdef വാക്കുകൾ_BIGENDIAN
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	വേണ്ടി (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		എങ്കിൽ (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#endif

	എങ്കിൽ (actual == -1) {
		retval = errno;
		പോകുക errout;
	}
	എങ്കിൽ (actual != (ssize_t)(fs->blocksize * fs->desc_blocks)) {
		retval = EXT2_ET_SHORT_WRITE;
		പോകുക errout;
	}

	retval = 0;

errout:
	സ്വതന്ത്രം(buf);
	തിരികെ retval;
}

/*
 * Read the superblock and group descriptors and overwrite them.
 */
errcode_t ext2fs_image_super_read(ext2_filsys fs, പൂർണ്ണസംഖ്യ fd,
				  പൂർണ്ണസംഖ്യ flags EXT2FS_ATTR((unused)))
{
	അക്ഷരം		*buf;
	ssize_t		actual, size;
	errcode_t	retval;

	size = (ssize_t)fs->blocksize * (fs->desc_blocks + 1);
	buf = malloc(size);
	എങ്കിൽ (!buf)
		തിരികെ ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = പഠ(fd, buf, size);
	എങ്കിൽ (actual == -1) {
		retval = errno;
		പോകുക errout;
	}
	എങ്കിൽ (actual != size) {
		retval = EXT2_ET_SHORT_READ;
		പോകുക errout;
	}

	/*
	 * Now copy in the superblock and group descriptors
	 */
	memcpy(fs->super, buf, SUPERBLOCK_SIZE);

	memcpy(fs->group_desc, buf + fs->blocksize,
	       (ssize_t)fs->blocksize * fs->desc_blocks);

	retval = 0;

errout:
	സ്വതന്ത്രം(buf);
	തിരികെ retval;
}

/*
 * Write the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	ssize_t			actual;
	size_t			c;
	__u64			itr, cnt, size, total_size;
	അക്ഷരം			buf[1024];

	എങ്കിൽ (flags & IMAGER_FLAG_INODEMAP) {
		എങ്കിൽ (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			എങ്കിൽ (retval)
				തിരികെ retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_എണ്ണം;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			എങ്കിൽ (retval)
				തിരികെ retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super, fs->group_desc_എണ്ണം);
		size = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	}
	total_size = size * fs->group_desc_എണ്ണം;

	ആകുമ്പോൾ (cnt > 0) {
		size = വലുപ്പം(buf);
		എങ്കിൽ (size > (cnt >> 3))
			size = (cnt >> 3);
		എങ്കിൽ (size == 0)
			നിർത്തുക;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		എങ്കിൽ (retval)
			തിരികെ retval;

		actual = ലിഖ(fd, buf, size);
		എങ്കിൽ (actual == -1)
			തിരികെ errno;
		എങ്കിൽ (actual != (പൂർണ്ണസംഖ്യ) size)
			തിരികെ EXT2_ET_SHORT_READ;

		itr += size << 3;
		cnt -= size << 3;
	}

	size = total_size % fs->blocksize;
	memset(buf, 0, വലുപ്പം(buf));
	എങ്കിൽ (size) {
		size = fs->blocksize - size;
		ആകുമ്പോൾ (size) {
			c = size;
			എങ്കിൽ (c > (പൂർണ്ണസംഖ്യ) വലുപ്പം(buf))
				c = വലുപ്പം(buf);
			actual = ലിഖ(fd, buf, c);
			എങ്കിൽ (actual < 0)
				തിരികെ errno;
			എങ്കിൽ ((size_t) actual != c)
				തിരികെ EXT2_ET_SHORT_WRITE;
			size -= c;
		}
	}
	തിരികെ 0;
}


/*
 * Read the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, പൂർണ്ണസംഖ്യ fd, പൂർണ്ണസംഖ്യ flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	__u64			itr, cnt;
	അക്ഷരം			buf[1024];
	unsigned പൂർണ്ണസംഖ്യ		size;
	ssize_t			actual;

	എങ്കിൽ (flags & IMAGER_FLAG_INODEMAP) {
		എങ്കിൽ (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			എങ്കിൽ (retval)
				തിരികെ retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_എണ്ണം;
		size = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} അല്ലെങ്കിൽ {
		എങ്കിൽ (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			എങ്കിൽ (retval)
				തിരികെ retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_data_block;
		cnt = EXT2_GROUPS_TO_BLOCKS(fs->super, fs->group_desc_എണ്ണം);
		size = EXT2_BLOCKS_PER_GROUP(fs->super) / 8;
	}

	ആകുമ്പോൾ (cnt > 0) {
		size = വലുപ്പം(buf);
		എങ്കിൽ (size > (cnt >> 3))
			size = (cnt >> 3);
		എങ്കിൽ (size == 0)
			നിർത്തുക;

		actual = പഠ(fd, buf, size);
		എങ്കിൽ (actual == -1)
			തിരികെ errno;
		എങ്കിൽ (actual != (പൂർണ്ണസംഖ്യ) size)
			തിരികെ EXT2_ET_SHORT_READ;

		retval = ext2fs_set_generic_bmap_range(bmap, itr,
						       size << 3, buf);
		എങ്കിൽ (retval)
			തിരികെ retval;

		itr += size << 3;
		cnt -= size << 3;
	}
	തിരികെ 0;
}
