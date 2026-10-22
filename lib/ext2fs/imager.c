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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#अगर HAVE_ERRNO_H
#समावेश <errno.h>
#पूर्ण_अगर
#समावेश <fcntl.h>
#समावेश <time.h>
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

#अगर_अघोषित HAVE_TYPE_SSIZE_T
प्रकार पूर्णांक ssize_t;
#पूर्ण_अगर

/*
 * This function returns 1 if the specified block is all zeros
 */
अटल पूर्णांक check_zero_block(अक्षर *buf, पूर्णांक blocksize)
{
	अक्षर	*cp = buf;
	पूर्णांक	बायाँ = blocksize;

	जबतक (बायाँ > 0) {
		अगर (*cp++)
			वापस 0;
		बायाँ--;
	}
	वापस 1;
}

/*
 * Write the inode table out as a single block.
 */
#घोषणा BUF_BLOCKS	32

errcode_t ext2fs_image_inode_write(ext2_filsys fs, पूर्णांक fd, पूर्णांक flags)
{
	dgrp_t		group;
	ssize_t		बायाँ, c, d;
	अक्षर		*buf, *cp;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;
	ext2_loff_t	r;

	buf = दो_स्मृति(fs->blocksize * BUF_BLOCKS);
	अगर (!buf)
		वापस ENOMEM;

	क्रम (group = 0; group < fs->group_desc_गिनती; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		अगर (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			जाओ errout;
		}
		बायाँ = fs->inode_blocks_per_group;
		अगर ((blk < fs->super->s_first_तथ्य_block) ||
		    (blk + बायाँ - 1 >= ext2fs_blocks_गिनती(fs->super))) {
			retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
			जाओ errout;
		}
		जबतक (बायाँ) {
			c = BUF_BLOCKS;
			अगर (c > बायाँ)
				c = बायाँ;
			retval = io_channel_read_blk64(fs->io, blk, c, buf);
			अगर (retval)
				जाओ errout;
			cp = buf;
			जबतक (c) {
				अगर (!(flags & IMAGER_FLAG_SPARSEWRITE)) {
					d = c;
					जाओ skip_sparse;
				}
				/* Skip zero blocks */
				अगर (check_zero_block(cp, fs->blocksize)) {
					c--;
					blk++;
					बायाँ--;
					cp += fs->blocksize;
					r = ext2fs_llseek(fd, fs->blocksize,
							  प्रस्तुत_से);
					अगर (r < 0) {
						retval = त्रुटि_सं;
						जाओ errout;
					}
					जारी;
				}
				/* Find non-zero blocks */
				क्रम (d = 1; d < c; d++) {
					अगर (check_zero_block(cp +
							     d * fs->blocksize,
							     fs->blocksize))
						अवरोध;
				}
			skip_sparse:
				actual = ग_लिखो(fd, cp, d * fs->blocksize);
				अगर (actual == -1) {
					retval = त्रुटि_सं;
					जाओ errout;
				}
				अगर (actual != d * fs->blocksize) {
					retval = EXT2_ET_SHORT_WRITE;
					जाओ errout;
				}
				blk += d;
				बायाँ -= d;
				cp += d * fs->blocksize;
				c -= d;
			}
		}
	}
	retval = 0;

errout:
	मुक्त(buf);
	वापस retval;
}

/*
 * Read in the inode table and stuff it into place
 */
errcode_t ext2fs_image_inode_read(ext2_filsys fs, पूर्णांक fd,
				  पूर्णांक flags EXT2FS_ATTR((unused)))
{
	dgrp_t		group;
	ssize_t		c, बायाँ;
	अक्षर		*buf;
	blk64_t		blk;
	ssize_t		actual;
	errcode_t	retval;

	buf = दो_स्मृति(fs->blocksize * BUF_BLOCKS);
	अगर (!buf)
		वापस ENOMEM;

	क्रम (group = 0; group < fs->group_desc_गिनती; group++) {
		blk = ext2fs_inode_table_loc(fs, group);
		अगर (!blk) {
			retval = EXT2_ET_MISSING_INODE_TABLE;
			जाओ errout;
		}
		बायाँ = fs->inode_blocks_per_group;
		जबतक (बायाँ) {
			c = BUF_BLOCKS;
			अगर (c > बायाँ)
				c = बायाँ;
			actual = पढ़ो(fd, buf, fs->blocksize * c);
			अगर (actual == -1) {
				retval = त्रुटि_सं;
				जाओ errout;
			}
			अगर (actual != fs->blocksize * c) {
				retval = EXT2_ET_SHORT_READ;
				जाओ errout;
			}
			retval = io_channel_write_blk64(fs->io, blk, c, buf);
			अगर (retval)
				जाओ errout;

			blk += c;
			बायाँ -= c;
		}
	}
	retval = ext2fs_flush_icache(fs);

errout:
	मुक्त(buf);
	वापस retval;
}

/*
 * Write out superblock and group descriptors
 */
errcode_t ext2fs_image_super_write(ext2_filsys fs, पूर्णांक fd,
				   पूर्णांक flags EXT2FS_ATTR((unused)))
{
	अक्षर		*buf, *cp;
	ssize_t		actual;
	errcode_t	retval;
#अगर_घोषित शब्दगण_BIGENDIAN
	अचिन्हित पूर्णांक	groups_per_block;
	काष्ठा		ext2_group_desc *gdp;
	पूर्णांक		j;
#पूर्ण_अगर

	अगर (fs->group_desc == शून्य)
		वापस EXT2_ET_NO_GDESC;

	buf = दो_स्मृति(fs->blocksize);
	अगर (!buf)
		वापस ENOMEM;

	/*
	 * Write out the superblock
	 */
	स_रखो(buf, 0, fs->blocksize);
#अगर_घोषित शब्दगण_BIGENDIAN
	/*
	 * We're writing out superblock so let's convert
	 * it to little endian and then back if needed
	 */
	ext2fs_swap_super(fs->super);
	स_नकल(buf, fs->super, SUPERBLOCK_आकार);
	ext2fs_swap_super(fs->super);
#अन्यथा
	स_नकल(buf, fs->super, SUPERBLOCK_आकार);
#पूर्ण_अगर
	actual = ग_लिखो(fd, buf, fs->blocksize);
	अगर (actual == -1) {
		retval = त्रुटि_सं;
		जाओ errout;
	}
	अगर (actual != (ssize_t) fs->blocksize) {
		retval = EXT2_ET_SHORT_WRITE;
		जाओ errout;
	}

	/*
	 * Now write out the block group descriptors
	 */

	cp = (अक्षर *) fs->group_desc;

#अगर_घोषित शब्दगण_BIGENDIAN
	/*
	 * Convert group descriptors to little endian and back
	 * if needed
	 */
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	क्रम (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		अगर (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#पूर्ण_अगर

	actual = ग_लिखो(fd, cp, (ssize_t)fs->blocksize * fs->desc_blocks);


#अगर_घोषित शब्दगण_BIGENDIAN
	groups_per_block = EXT2_DESC_PER_BLOCK(fs->super);
	क्रम (j=0; j < groups_per_block*fs->desc_blocks; j++) {
		gdp = ext2fs_group_desc(fs, fs->group_desc, j);
		अगर (gdp)
			ext2fs_swap_group_desc2(fs, gdp);
	}
#पूर्ण_अगर

	अगर (actual == -1) {
		retval = त्रुटि_सं;
		जाओ errout;
	}
	अगर (actual != (ssize_t)(fs->blocksize * fs->desc_blocks)) {
		retval = EXT2_ET_SHORT_WRITE;
		जाओ errout;
	}

	retval = 0;

errout:
	मुक्त(buf);
	वापस retval;
}

/*
 * Read the superblock and group descriptors and overwrite them.
 */
errcode_t ext2fs_image_super_read(ext2_filsys fs, पूर्णांक fd,
				  पूर्णांक flags EXT2FS_ATTR((unused)))
{
	अक्षर		*buf;
	ssize_t		actual, आकार;
	errcode_t	retval;

	आकार = (ssize_t)fs->blocksize * (fs->desc_blocks + 1);
	buf = दो_स्मृति(आकार);
	अगर (!buf)
		वापस ENOMEM;

	/*
	 * Read it all in.
	 */
	actual = पढ़ो(fd, buf, आकार);
	अगर (actual == -1) {
		retval = त्रुटि_सं;
		जाओ errout;
	}
	अगर (actual != आकार) {
		retval = EXT2_ET_SHORT_READ;
		जाओ errout;
	}

	/*
	 * Now copy in the superblock and group descriptors
	 */
	स_नकल(fs->super, buf, SUPERBLOCK_आकार);

	स_नकल(fs->group_desc, buf + fs->blocksize,
	       (ssize_t)fs->blocksize * fs->desc_blocks);

	retval = 0;

errout:
	मुक्त(buf);
	वापस retval;
}

/*
 * Write the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_write(ext2_filsys fs, पूर्णांक fd, पूर्णांक flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	ssize_t			actual;
	माप_प्रकार			c;
	__u64			itr, cnt, आकार, योग_आकार;
	अक्षर			buf[1024];

	अगर (flags & IMAGER_FLAG_INODEMAP) {
		अगर (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			अगर (retval)
				वापस retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_गिनती;
		आकार = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} अन्यथा {
		अगर (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			अगर (retval)
				वापस retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_तथ्य_block;
		cnt = EXT2_GROUPS_TO_CLUSTERS(fs->super, fs->group_desc_गिनती);
		आकार = EXT2_CLUSTERS_PER_GROUP(fs->super) / 8;
	}
	योग_आकार = आकार * fs->group_desc_गिनती;

	जबतक (cnt > 0) {
		आकार = माप(buf);
		अगर (आकार > (cnt >> 3))
			आकार = (cnt >> 3);
		अगर (आकार == 0)
			अवरोध;

		retval = ext2fs_get_generic_bmap_range(bmap, itr,
						       आकार << 3, buf);
		अगर (retval)
			वापस retval;

		actual = ग_लिखो(fd, buf, आकार);
		अगर (actual == -1)
			वापस त्रुटि_सं;
		अगर (actual != (पूर्णांक) आकार)
			वापस EXT2_ET_SHORT_READ;

		itr += आकार << 3;
		cnt -= आकार << 3;
	}

	आकार = योग_आकार % fs->blocksize;
	स_रखो(buf, 0, माप(buf));
	अगर (आकार) {
		आकार = fs->blocksize - आकार;
		जबतक (आकार) {
			c = आकार;
			अगर (c > (पूर्णांक) माप(buf))
				c = माप(buf);
			actual = ग_लिखो(fd, buf, c);
			अगर (actual < 0)
				वापस त्रुटि_सं;
			अगर ((माप_प्रकार) actual != c)
				वापस EXT2_ET_SHORT_WRITE;
			आकार -= c;
		}
	}
	वापस 0;
}


/*
 * Read the block/inode bitmaps.
 */
errcode_t ext2fs_image_bitmap_read(ext2_filsys fs, पूर्णांक fd, पूर्णांक flags)
{
	ext2fs_generic_bitmap	bmap;
	errcode_t		retval;
	__u64			itr, cnt;
	अक्षर			buf[1024];
	अचिन्हित पूर्णांक		आकार;
	ssize_t			actual;

	अगर (flags & IMAGER_FLAG_INODEMAP) {
		अगर (!fs->inode_map) {
			retval = ext2fs_read_inode_bitmap(fs);
			अगर (retval)
				वापस retval;
		}
		bmap = fs->inode_map;
		itr = 1;
		cnt = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
			fs->group_desc_गिनती;
		आकार = (EXT2_INODES_PER_GROUP(fs->super) / 8);
	} अन्यथा {
		अगर (!fs->block_map) {
			retval = ext2fs_read_block_bitmap(fs);
			अगर (retval)
				वापस retval;
		}
		bmap = fs->block_map;
		itr = fs->super->s_first_तथ्य_block;
		cnt = EXT2_GROUPS_TO_BLOCKS(fs->super, fs->group_desc_गिनती);
		आकार = EXT2_BLOCKS_PER_GROUP(fs->super) / 8;
	}

	जबतक (cnt > 0) {
		आकार = माप(buf);
		अगर (आकार > (cnt >> 3))
			आकार = (cnt >> 3);
		अगर (आकार == 0)
			अवरोध;

		actual = पढ़ो(fd, buf, आकार);
		अगर (actual == -1)
			वापस त्रुटि_सं;
		अगर (actual != (पूर्णांक) आकार)
			वापस EXT2_ET_SHORT_READ;

		retval = ext2fs_set_generic_bmap_range(bmap, itr,
						       आकार << 3, buf);
		अगर (retval)
			वापस retval;

		itr += आकार << 3;
		cnt -= आकार << 3;
	}
	वापस 0;
}
