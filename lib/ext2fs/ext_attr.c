/*
 * ext_attr.c --- extended attribute blocks
 *
 * Copyright (C) 2001 Andreas Gruenbacher, <a.gruenbacher@computer.org>
 *
 * Copyright (C) 2002 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2_ext_attr.h"
#ಅನ್ತರ್ಭಾವ "ext4_acl.h"

#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ಸ್ಥಿರ errcode_t read_ea_inode_hash(ext2_filsys fs, ext2_ino_t ino, __u32 *hash)
{
	ಸಂರಚನಾ ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	ಪ್ರತ್ಯಾಗಮ 0;
}

#define NAME_HASH_SHIFT 5
#define ಮೌಲ್ಯ_HASH_SHIFT 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_entry(ಸಂರಚನಾ ext2_ext_attr_entry *entry, ಶೂನ್ಯ *data)
{
	__u32 hash = 0;
	unsigned ವರ್ಣ *name = (((unsigned ವರ್ಣ *) entry) +
			       ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_ext_attr_entry));
	ಪೂರ್ಣಾಙ್ಕ n;

	ಪ್ರತಿ (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*ಪರಿಮಾಣ(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	ಯದಿ (entry->e_ಮೌಲ್ಯ_inum == 0 && entry->e_ಮೌಲ್ಯ_size != 0) {
		__u32 *ಮೌಲ್ಯ = (__u32 *)data;
		ಪ್ರತಿ (n = (entry->e_ಮೌಲ್ಯ_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << ಮೌಲ್ಯ_HASH_SHIFT) ^
			       (hash >> (8*ಪರಿಮಾಣ(hash) - ಮೌಲ್ಯ_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*ಮೌಲ್ಯ++);
		}
	}

	ಪ್ರತ್ಯಾಗಮ hash;
}

__u32 ext2fs_ext_attr_hash_entry_signed(ಸಂರಚನಾ ext2_ext_attr_entry *entry,
					ಶೂನ್ಯ *data)
{
	__u32 hash = 0;
	signed ವರ್ಣ *name = (((signed ವರ್ಣ *) entry) +
			     ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_ext_attr_entry));
	ಪೂರ್ಣಾಙ್ಕ n;

	ಪ್ರತಿ (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*ಪರಿಮಾಣ(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	ಯದಿ (entry->e_ಮೌಲ್ಯ_inum == 0 && entry->e_ಮೌಲ್ಯ_size != 0) {
		__u32 *ಮೌಲ್ಯ = (__u32 *)data;
		ಪ್ರತಿ (n = (entry->e_ಮೌಲ್ಯ_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << ಮೌಲ್ಯ_HASH_SHIFT) ^
			       (hash >> (8*ಪರಿಮಾಣ(hash) - ಮೌಲ್ಯ_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*ಮೌಲ್ಯ++);
		}
	}

	ಪ್ರತ್ಯಾಗಮ hash;
}


/*
 * ext2fs_ext_attr_hash_entry3()
 *
 * Compute the hash of an extended attribute.  This version of the
 * function supports hashing entries that reference external inodes
 * (ea_inode feature) as well as calculating the old legacy signed
 * hash variant.
 */
errcode_t ext2fs_ext_attr_hash_entry3(ext2_filsys fs,
				      ಸಂರಚನಾ ext2_ext_attr_entry *entry,
				      ಶೂನ್ಯ *data, __u32 *hash,
				      __u32 *signed_hash)
{
	*hash = ext2fs_ext_attr_hash_entry(entry, data);
	ಯದಿ (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_entry_signed(entry, data);

	ಯದಿ (entry->e_ಮೌಲ್ಯ_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, entry->e_ಮೌಲ್ಯ_inum,
					    &ea_inode_hash);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;

		*hash = (*hash << ಮೌಲ್ಯ_HASH_SHIFT) ^
			(*hash >> (8*ಪರಿಮಾಣ(*hash) - ಮೌಲ್ಯ_HASH_SHIFT)) ^
			ea_inode_hash;
		ಯದಿ (signed_hash)
			*signed_hash = (*signed_hash << ಮೌಲ್ಯ_HASH_SHIFT) ^
				(*signed_hash >> (8*ಪರಿಮಾಣ(*hash) -
						  ಮೌಲ್ಯ_HASH_SHIFT)) ^
				ea_inode_hash;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * ext2fs_ext_attr_hash_entry2()
 *
 * Compute the hash of an extended attribute.
 * This version of the function supports hashing entries that reference
 * external inodes (ea_inode feature).
 */
errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
				      ಸಂರಚನಾ ext2_ext_attr_entry *entry,
				      ಶೂನ್ಯ *data, __u32 *hash)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_ext_attr_hash_entry3(fs, entry, data, hash, ರಿಕ್ತ);
}

#undef NAME_HASH_SHIFT
#undef ಮೌಲ್ಯ_HASH_SHIFT

#define BLOCK_HASH_SHIFT 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
ಶೂನ್ಯ ext2fs_ext_attr_block_rehash(ಸಂರಚನಾ ext2_ext_attr_header *header,
				  ಸಂರಚನಾ ext2_ext_attr_entry *end)
{
	ಸಂರಚನಾ ext2_ext_attr_entry *here;
	__u32 hash = 0;

	here = (ಸಂರಚನಾ ext2_ext_attr_entry *)(header+1);
	ಯಾವತ್ (here < end && !EXT2_EXT_IS_LAST_ENTRY(here)) {
		ಯದಿ (!here->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			ಭಙ್ಗ;
		}
		hash = (hash << BLOCK_HASH_SHIFT) ^
		       (hash >> (8*ಪರಿಮಾಣ(hash) - BLOCK_HASH_SHIFT)) ^
		       here->e_hash;
		here = EXT2_EXT_ATTR_NEXT(here);
	}
	header->h_hash = hash;
}

#undef BLOCK_HASH_SHIFT

__u32 ext2fs_get_ea_inode_hash(ಸಂರಚನಾ ext2_inode *inode)
{
	ಪ್ರತ್ಯಾಗಮ inode->i_atime;
}

ಶೂನ್ಯ ext2fs_set_ea_inode_hash(ಸಂರಚನಾ ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(ಸಂರಚನಾ ext2_inode *inode)
{
	ಪ್ರತ್ಯಾಗಮ ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_version;
}

ಶೂನ್ಯ ext2fs_set_ea_inode_ref(ಸಂರಚನಾ ext2_inode *inode, __u64 ref_ಎಣಿಕೆ)
{
	inode->i_ctime = (__u32)(ref_ಎಣಿಕೆ >> 32);
	inode->osd1.linux1.l_i_version = (__u32)ref_ಎಣಿಕೆ;
}

ಸ್ಥಿರ errcode_t check_ext_attr_header(ಸಂರಚನಾ ext2_ext_attr_header *header)
{
	ಯದಿ ((header->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     header->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    header->h_blocks != 1)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_BAD_EA_HEADER;

	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block, ಶೂನ್ಯ *buf,
				ext2_ino_t inum)
{
	ಪೂರ್ಣಾಙ್ಕ		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	ಯದಿ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_block_csum_verify(fs, inum, block, buf))
		csum_failed = 1;

#ifdef ಪದಗಳು_BIGENDIAN
	ext2fs_swap_ext_attr(buf, buf, fs->blocksize, 1);
#endif

	retval = check_ext_attr_header(buf);
	ಯದಿ (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	ಪ್ರತ್ಯಾಗಮ retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block, ಶೂನ್ಯ *buf)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_read_ext_attr3(fs, block, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, ಶೂನ್ಯ *buf)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_read_ext_attr2(fs, block, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block, ಶೂನ್ಯ *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	ವರ್ಣ		*write_buf;

#ifdef ಪದಗಳು_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	ext2fs_swap_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#ಅನ್ಯಥಾ
	write_buf = (ವರ್ಣ *) inbuf;
#endif

	retval = ext2fs_ext_attr_block_csum_set(fs, inum, block,
			(ಸಂರಚನಾ ext2_ext_attr_header *)write_buf);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	retval = io_channel_write_blk64(fs->io, block, 1, write_buf);
#ifdef ಪದಗಳು_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#endif
	ಯದಿ (!retval)
		ext2fs_mark_changed(fs);
	ಪ್ರತ್ಯಾಗಮ retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block, ಶೂನ್ಯ *inbuf)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_write_ext_attr3(fs, block, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block, ಶೂನ್ಯ *inbuf)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_write_ext_attr2(fs, block, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    ವರ್ಣ *block_buf, ಪೂರ್ಣಾಙ್ಕ adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	ಸಂರಚನಾ ext2_ext_attr_header *header;
	ವರ್ಣ	*buf = 0;

	ಯದಿ ((blk >= ext2fs_blocks_ಎಣಿಕೆ(fs->super)) ||
	    (blk < fs->super->s_first_data_block))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_BAD_EA_BLOCK_ಅಂಕೆ;

	ಯದಿ (!block_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
		block_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, block_buf, inum);
	ಯದಿ (retval)
		ಗಚ್ಛ errout;

	header = (ಸಂರಚನಾ ext2_ext_attr_header *) block_buf;
	header->h_refcount += adjust;
	ಯದಿ (newcount)
		*newcount = header->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, block_buf, inum);
	ಯದಿ (retval)
		ಗಚ್ಛ errout;

errout:
	ಯದಿ (buf)
		ext2fs_free_mem(&buf);
	ಪ್ರತ್ಯಾಗಮ retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    ವರ್ಣ *block_buf, ಪೂರ್ಣಾಙ್ಕ adjust,
				    __u32 *newcount)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_adjust_ea_refcount3(fs, blk, block_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					ವರ್ಣ *block_buf, ಪೂರ್ಣಾಙ್ಕ adjust,
					__u32 *newcount)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_adjust_ea_refcount2(fs, blk, block_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
ಸಂರಚನಾ ext2_xattr {
	ಪೂರ್ಣಾಙ್ಕ name_index;
	ವರ್ಣ *name;
	ವರ್ಣ *short_name;
	ಶೂನ್ಯ *ಮೌಲ್ಯ;
	unsigned ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ_len;
	ext2_ino_t ea_ino;
};

ಸಂರಚನಾ ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	ಸಂರಚನಾ ext2_xattr *attrs;
	ಪೂರ್ಣಾಙ್ಕ capacity;
	ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ;
	ಪೂರ್ಣಾಙ್ಕ ibody_ಎಣಿಕೆ;
	ext2_ino_t ino;
	unsigned ಪೂರ್ಣಾಙ್ಕ flags;
};

ಸ್ಥಿರ errcode_t ext2fs_xattrs_expand(ಸಂರಚನಾ ext2_xattr_handle *h,
				      unsigned ಪೂರ್ಣಾಙ್ಕ expandby)
{
	ಸಂರಚನಾ ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->capacity + expandby,
				   ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_xattr), &new_attrs);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ err;

	memcpy(new_attrs, h->attrs, h->capacity * ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->capacity += expandby;
	h->attrs = new_attrs;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ ea_name_index {
	ಪೂರ್ಣಾಙ್ಕ index;
	ಧ್ರುವ ವರ್ಣ *name;
};

/* Keep these names sorted in order of decreasing specificity. */
ಸ್ಥಿರ ಸಂರಚನಾ ea_name_index ea_names[] = {
	{10, "gnu."},
	{3, "system.posix_acl_default"},
	{2, "system.posix_acl_access"},
	{8, "system.richacl"},
	{6, "security."},
	{4, "trusted."},
	{7, "system."},
	{1, "user."},
	{0, ರಿಕ್ತ},
};

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *find_ea_prefix(ಪೂರ್ಣಾಙ್ಕ index)
{
	ಸಂರಚನಾ ea_name_index *e;

	ಪ್ರತಿ (e = ea_names; e->name; e++)
		ಯದಿ (e->index == index)
			ಪ್ರತ್ಯಾಗಮ e->name;

	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ find_ea_index(ಧ್ರುವ ವರ್ಣ *fullname, ಧ್ರುವ ವರ್ಣ **name, ಪೂರ್ಣಾಙ್ಕ *index)
{
	ಸಂರಚನಾ ea_name_index *e;

	ಪ್ರತಿ (e = ea_names; e->name; e++) {
		ಯದಿ (strncmp(fullname, e->name, strlen(e->name)) == 0) {
			*name = fullname + strlen(e->name);
			*index = e->index;
			ಪ್ರತ್ಯಾಗಮ 1;
		}
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       ಸಂರಚನಾ ext2_inode_large *inode)
{
	ಸಂರಚನಾ ext2_ext_attr_header *header;
	ಶೂನ್ಯ *block_buf = ರಿಕ್ತ;
	blk64_t blk;
	errcode_t err;
	ಸಂರಚನಾ ext2_inode_large i;

	/* Read inode? */
	ಯದಿ (inode == ರಿಕ್ತ) {
		err = ext2fs_read_inode_full(fs, ino, (ಸಂರಚನಾ ext2_inode *)&i,
					     ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode_large));
		ಯದಿ (err)
			ಪ್ರತ್ಯಾಗಮ err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (ಸಂರಚನಾ ext2_inode *)inode);
	ಯದಿ (blk == 0)
		ಪ್ರತ್ಯಾಗಮ 0;

	/* Find block, zero it, write back */
	ಯದಿ ((blk < fs->super->s_first_data_block) ||
	    (blk >= ext2fs_blocks_ಎಣಿಕೆ(fs->super))) {
		err = EXT2_ET_BAD_EA_BLOCK_ಅಂಕೆ;
		ಗಚ್ಛ out;
	}

	err = ext2fs_get_mem(fs->blocksize, &block_buf);
	ಯದಿ (err)
		ಗಚ್ಛ out;

	err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
	ಯದಿ (err)
		ಗಚ್ಛ out2;

	/* We only know how to deal with v2 EA blocks */
	header = (ಸಂರಚನಾ ext2_ext_attr_header *) block_buf;
	ಯದಿ (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_HEADER;
		ಗಚ್ಛ out2;
	}

	header->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
	ಯದಿ (err)
		ಗಚ್ಛ out2;

	/* Erase link to block */
	ext2fs_file_acl_block_set(fs, (ಸಂರಚನಾ ext2_inode *)inode, 0);
	ಯದಿ (header->h_refcount == 0)
		ext2fs_block_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_blocks(fs, (ಸಂರಚನಾ ext2_inode *)inode, 1);
	ಯದಿ (err)
		ಗಚ್ಛ out2;

	/* Write inode? */
	ಯದಿ (inode == &i) {
		err = ext2fs_write_inode_full(fs, ino, (ಸಂರಚನಾ ext2_inode *)&i,
					      ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_inode_large));
		ಯದಿ (err)
			ಗಚ್ಛ out2;
	}

out2:
	ext2fs_free_mem(&block_buf);
out:
	ಪ್ರತ್ಯಾಗಮ err;
}

ಸ್ಥಿರ errcode_t prep_ea_block_for_write(ext2_filsys fs, ext2_ino_t ino,
					 ಸಂರಚನಾ ext2_inode_large *inode)
{
	ಸಂರಚನಾ ext2_ext_attr_header *header;
	ಶೂನ್ಯ *block_buf = ರಿಕ್ತ;
	blk64_t blk, goal;
	errcode_t err;

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (ಸಂರಚನಾ ext2_inode *)inode);
	ಯದಿ (blk != 0) {
		ಯದಿ ((blk < fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_ಎಣಿಕೆ(fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_ಅಂಕೆ;
			ಗಚ್ಛ out;
		}

		err = ext2fs_get_mem(fs->blocksize, &block_buf);
		ಯದಿ (err)
			ಗಚ್ಛ out;

		err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
		ಯದಿ (err)
			ಗಚ್ಛ out2;

		/* We only know how to deal with v2 EA blocks */
		header = (ಸಂರಚನಾ ext2_ext_attr_header *) block_buf;
		ಯದಿ (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			ಗಚ್ಛ out2;
		}

		/* Single-user block.  We're done here. */
		ಯದಿ (header->h_refcount == 1)
			ಗಚ್ಛ out2;

		/* We need to CoW the block. */
		header->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
		ಯದಿ (err)
			ಗಚ್ಛ out2;
	} ಅನ್ಯಥಾ {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_add_blocks(fs, (ಸಂರಚನಾ ext2_inode *)inode,
					     1);
		ಯದಿ (err)
			ಗಚ್ಛ out;
	}

	/* Allocate a block */
	goal = ext2fs_find_inode_goal(fs, ino, (ಸಂರಚನಾ ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, ರಿಕ್ತ, &blk);
	ಯದಿ (err)
		ಗಚ್ಛ out2;
	ext2fs_file_acl_block_set(fs, (ಸಂರಚನಾ ext2_inode *)inode, blk);
out2:
	ಯದಿ (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	ಪ್ರತ್ಯಾಗಮ err;
}


ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ
posix_acl_xattr_ಎಣಿಕೆ(size_t size)
{
        ಯದಿ (size < ಪರಿಮಾಣ(posix_acl_xattr_header))
                ಪ್ರತ್ಯಾಗಮ -1;
        size -= ಪರಿಮಾಣ(posix_acl_xattr_header);
        ಯದಿ (size % ಪರಿಮಾಣ(posix_acl_xattr_entry))
                ಪ್ರತ್ಯಾಗಮ -1;
        ಪ್ರತ್ಯಾಗಮ size / ಪರಿಮಾಣ(posix_acl_xattr_entry);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
ಸ್ಥಿರ errcode_t convert_posix_acl_to_disk_buffer(ಧ್ರುವ ಶೂನ್ಯ *ಮೌಲ್ಯ, size_t size,
						  ಶೂನ್ಯ *out_buf, size_t *size_out)
{
	ಧ್ರುವ posix_acl_xattr_header *header =
		(ಧ್ರುವ posix_acl_xattr_header*) ಮೌಲ್ಯ;
	ಧ್ರುವ posix_acl_xattr_entry *end, *entry =
		(ಧ್ರುವ posix_acl_xattr_entry *)(header+1);
	ext4_acl_header *ext_acl;
	size_t s;
	ವರ್ಣ *e;

	ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ;

	ಯದಿ (!ಮೌಲ್ಯ)
		ಪ್ರತ್ಯಾಗಮ EINVAL;
	ಯದಿ (size < ಪರಿಮಾಣ(posix_acl_xattr_header))
		ಪ್ರತ್ಯಾಗಮ ENOMEM;
	ಯದಿ (header->a_version != ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION))
		ಪ್ರತ್ಯಾಗಮ EINVAL;

	ಎಣಿಕೆ = posix_acl_xattr_ಎಣಿಕೆ(size);
	ext_acl = out_buf;
	ext_acl->a_version = ext2fs_cpu_to_le32(EXT4_ACL_VERSION);

	ಯದಿ (ಎಣಿಕೆ <= 0)
		ಪ್ರತ್ಯಾಗಮ EINVAL;

	e = (ವರ್ಣ *) out_buf + ಪರಿಮಾಣ(ext4_acl_header);
	s = ಪರಿಮಾಣ(ext4_acl_header);
	ಪ್ರತಿ (end = entry + ಎಣಿಕೆ; entry != end;entry++) {
		ext4_acl_entry *disk_entry = (ext4_acl_entry*) e;
		disk_entry->e_tag = entry->e_tag;
		disk_entry->e_perm = entry->e_perm;

		ವಿಕಲ್ಪ(ext2fs_le16_to_cpu(entry->e_tag)) {
			ಪಕ್ಷ ACL_USER_OBJ:
			ಪಕ್ಷ ACL_GROUP_OBJ:
			ಪಕ್ಷ ACL_MASK:
			ಪಕ್ಷ ACL_OTHER:
				e += ಪರಿಮಾಣ(ext4_acl_entry_short);
				s += ಪರಿಮಾಣ(ext4_acl_entry_short);
				ಭಙ್ಗ;
			ಪಕ್ಷ ACL_USER:
			ಪಕ್ಷ ACL_GROUP:
				disk_entry->e_id = entry->e_id;
				e += ಪರಿಮಾಣ(ext4_acl_entry);
				s += ಪರಿಮಾಣ(ext4_acl_entry);
				ಭಙ್ಗ;
			ಸ್ವಭಾವ:
				ಪ್ರತ್ಯಾಗಮ EINVAL;
		}
	}
	*size_out = s;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t convert_disk_buffer_to_posix_acl(ಧ್ರುವ ಶೂನ್ಯ *ಮೌಲ್ಯ, size_t size,
						  ಶೂನ್ಯ **out_buf, size_t *size_out)
{
	posix_acl_xattr_header *header;
	posix_acl_xattr_entry *entry;
	ಧ್ರುವ ext4_acl_header *ext_acl = (ಧ್ರುವ ext4_acl_header *) ಮೌಲ್ಯ;
	errcode_t err;
	ಧ್ರುವ ವರ್ಣ *cp;
	ವರ್ಣ *out;

	ಯದಿ ((!ಮೌಲ್ಯ) ||
	    (size < ಪರಿಮಾಣ(ext4_acl_header)) ||
	    (ext_acl->a_version != ext2fs_cpu_to_le32(EXT4_ACL_VERSION)))
		ಪ್ರತ್ಯಾಗಮ EINVAL;

	err = ext2fs_get_mem(size * 2, &out);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ err;

	header = (posix_acl_xattr_header *) out;
	header->a_version = ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION);
	entry = (posix_acl_xattr_entry *) (out + ಪರಿಮಾಣ(posix_acl_xattr_header));

	cp = (ಧ್ರುವ ವರ್ಣ *) ಮೌಲ್ಯ + ಪರಿಮಾಣ(ext4_acl_header);
	size -= ಪರಿಮಾಣ(ext4_acl_header);

	ಯಾವತ್ (size > 0) {
		ಧ್ರುವ ext4_acl_entry *disk_entry = (ಧ್ರುವ ext4_acl_entry *) cp;

		entry->e_tag = disk_entry->e_tag;
		entry->e_perm = disk_entry->e_perm;

		ವಿಕಲ್ಪ(ext2fs_le16_to_cpu(entry->e_tag)) {
			ಪಕ್ಷ ACL_USER_OBJ:
			ಪಕ್ಷ ACL_GROUP_OBJ:
			ಪಕ್ಷ ACL_MASK:
			ಪಕ್ಷ ACL_OTHER:
				entry->e_id = 0;
				cp += ಪರಿಮಾಣ(ext4_acl_entry_short);
				size -= ಪರಿಮಾಣ(ext4_acl_entry_short);
				ಭಙ್ಗ;
			ಪಕ್ಷ ACL_USER:
			ಪಕ್ಷ ACL_GROUP:
				entry->e_id = disk_entry->e_id;
				cp += ಪರಿಮಾಣ(ext4_acl_entry);
				size -= ಪರಿಮಾಣ(ext4_acl_entry);
				ಭಙ್ಗ;
			ಸ್ವಭಾವ:
				ext2fs_free_mem(&out);
				ಪ್ರತ್ಯಾಗಮ EINVAL;
		}
		entry++;
	}
	*out_buf = out;
	*size_out = ((ವರ್ಣ *) entry - out);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t
write_xattrs_to_buffer(ext2_filsys fs, ಸಂರಚನಾ ext2_xattr *attrs, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ,
		       ಶೂನ್ಯ *entries_start, unsigned ಪೂರ್ಣಾಙ್ಕ storage_size,
		       unsigned ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ_offset_correction, ಪೂರ್ಣಾಙ್ಕ write_hash)
{
	ಸಂರಚನಾ ext2_xattr *x;
	ಸಂರಚನಾ ext2_ext_attr_entry *e = entries_start;
	ವರ್ಣ *end = (ವರ್ಣ *) entries_start + storage_size;
	unsigned ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ_size;
	errcode_t err;

	memset(entries_start, 0, storage_size);
	ಪ್ರತಿ (x = attrs; x < attrs + ಎಣಿಕೆ; x++) {
		ಮೌಲ್ಯ_size = ((x->ಮೌಲ್ಯ_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = strlen(x->short_name);
		e->e_name_index = x->name_index;

		e->e_ಮೌಲ್ಯ_size = x->ಮೌಲ್ಯ_len;
		e->e_ಮೌಲ್ಯ_inum = x->ea_ino;

		/* Store name */
		memcpy((ವರ್ಣ *)e + ಪರಿಮಾಣ(*e), x->short_name, e->e_name_len);
		ಯದಿ (x->ea_ino) {
			e->e_ಮೌಲ್ಯ_offs = 0;
		} ಅನ್ಯಥಾ {
			end -= ಮೌಲ್ಯ_size;
			e->e_ಮೌಲ್ಯ_offs = end - (ವರ್ಣ *) entries_start +
						ಮೌಲ್ಯ_offset_correction;
			memcpy(end, x->ಮೌಲ್ಯ, e->e_ಮೌಲ್ಯ_size);
		}

		ಯದಿ (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : end,
							  &e->e_hash);
			ಯದಿ (err)
				ಪ್ರತ್ಯಾಗಮ err;
		} ಅನ್ಯಥಾ
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_NEXT(e);
		*(__u32 *)e = 0;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_xattrs_write(ಸಂರಚನಾ ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ inode_size = EXT2_INODE_SIZE(fs->super);
	ಸಂರಚನಾ ext2_inode_large *inode;
	ವರ್ಣ *start, *block_buf = ರಿಕ್ತ;
	ಸಂರಚನಾ ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	blk64_t blk;
	unsigned ಪೂರ್ಣಾಙ್ಕ storage_size;
	unsigned ಪೂರ್ಣಾಙ್ಕ i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_size;
	ಯದಿ (i < ಪರಿಮಾಣ(*inode))
		i = ಪರಿಮಾಣ(*inode);
	err = ext2fs_get_memzero(i, &inode);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ err;

	err = ext2fs_read_inode_full(fs, handle->ino, EXT2_INODE(inode),
				     inode_size);
	ಯದಿ (err)
		ಗಚ್ಛ out;

	/* If extra_isize isn't set, we need to set it now */
	ಯದಿ (inode->i_extra_isize == 0 &&
	    inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		ವರ್ಣ *p = (ವರ್ಣ *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		ಯದಿ (extra == 0)
			extra = ಪರಿಮಾಣ(__u32);
		memset(p + EXT2_GOOD_OLD_INODE_SIZE, 0, extra);
		inode->i_extra_isize = extra;
	}
	ಯದಿ (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		ಗಚ್ಛ out;
	}

	/* Does the inode have space for EA? */
	ಯದಿ (inode->i_extra_isize < ಪರಿಮಾಣ(inode->i_extra_isize) ||
	    inode_size <= EXT2_GOOD_OLD_INODE_SIZE + inode->i_extra_isize +
								ಪರಿಮಾಣ(__u32))
		ಗಚ್ಛ write_ea_block;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	memcpy(((ವರ್ಣ *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, &ea_inode_magic, ಪರಿಮಾಣ(__u32));
	storage_size = inode_size - EXT2_GOOD_OLD_INODE_SIZE -
				inode->i_extra_isize - ಪರಿಮಾಣ(__u32);
	start = ((ವರ್ಣ *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
				inode->i_extra_isize + ಪರಿಮಾಣ(__u32);

	err = write_xattrs_to_buffer(fs, handle->attrs, handle->ibody_ಎಣಿಕೆ,
				     start, storage_size, 0, 0);
	ಯದಿ (err)
		ಗಚ್ಛ out;
write_ea_block:
	/* Are we done? */
	ಯದಿ (handle->ibody_ಎಣಿಕೆ == handle->ಎಣಿಕೆ &&
	    !ext2fs_file_acl_block(fs, EXT2_INODE(inode)))
		ಗಚ್ಛ skip_ea_block;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &block_buf);
	ಯದಿ (err)
		ಗಚ್ಛ out;

	storage_size = fs->blocksize - ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_ext_attr_header);
	start = block_buf + ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_ext_attr_header);

	err = write_xattrs_to_buffer(fs, handle->attrs + handle->ibody_ಎಣಿಕೆ,
				     handle->ಎಣಿಕೆ - handle->ibody_ಎಣಿಕೆ, start,
				     storage_size, start - block_buf, 1);
	ಯದಿ (err)
		ಗಚ್ಛ out2;

	/* Write a header on the EA block */
	header = (ಸಂರಚನಾ ext2_ext_attr_header *) block_buf;
	header->h_magic = EXT2_EXT_ATTR_MAGIC;
	header->h_refcount = 1;
	header->h_blocks = 1;

	/* Get a new block for writing */
	err = prep_ea_block_for_write(fs, handle->ino, inode);
	ಯದಿ (err)
		ಗಚ್ಛ out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_block(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, handle->ino);
	ಯದಿ (err)
		ಗಚ್ಛ out2;

skip_ea_block:
	blk = ext2fs_file_acl_block(fs, (ಸಂರಚನಾ ext2_inode *)inode);
	ಯದಿ (!block_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		ಯದಿ (err)
			ಗಚ್ಛ out;
	}

	/* Write the inode */
	err = ext2fs_write_inode_full(fs, handle->ino, EXT2_INODE(inode),
				      inode_size);
	ಯದಿ (err)
		ಗಚ್ಛ out2;

out2:
	ext2fs_free_mem(&block_buf);
out:
	ext2fs_free_mem(&inode);
	ಪ್ರತ್ಯಾಗಮ err;
}

ಸ್ಥಿರ errcode_t read_xattrs_from_buffer(ಸಂರಚನಾ ext2_xattr_handle *handle,
					 ಸಂರಚನಾ ext2_inode_large *inode,
					 ಸಂರಚನಾ ext2_ext_attr_entry *entries,
					 unsigned ಪೂರ್ಣಾಙ್ಕ storage_size,
					 ವರ್ಣ *ಮೌಲ್ಯ_start)
{
	ಸಂರಚನಾ ext2_xattr *x;
	ಸಂರಚನಾ ext2_ext_attr_entry *entry, *end;
	ಧ್ರುವ ವರ್ಣ *prefix;
	unsigned ಪೂರ್ಣಾಙ್ಕ remain, prefix_len;
	errcode_t err;
	unsigned ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯಗಳು_size = storage_size +
			((ವರ್ಣ *)entries - ಮೌಲ್ಯ_start);

	/* find the end */
	end = entries;
	remain = storage_size;
	ಯಾವತ್ (remain >= ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(end)) {

		/* header eats this space */
		remain -= ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_ext_attr_entry);

		/* is attribute name valid? */
		ಯದಿ (EXT2_EXT_ATTR_SIZE(end->e_name_len) > remain)
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(end->e_name_len);
		end = EXT2_EXT_ATTR_NEXT(end);
	}

	entry = entries;
	remain = storage_size;
	ಯಾವತ್ (remain >= ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(entry)) {

		/* Allocate space for more attrs? */
		ಯದಿ (handle->ಎಣಿಕೆ == handle->capacity) {
			err = ext2fs_xattrs_expand(handle, 4);
			ಯದಿ (err)
				ಪ್ರತ್ಯಾಗಮ err;
		}

		x = handle->attrs + handle->ಎಣಿಕೆ;

		/* header eats this space */
		remain -= ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_ext_attr_entry);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(entry->e_name_len);

		/* Extract name */
		prefix = find_ea_prefix(entry->e_name_index);
		prefix_len = (prefix ? strlen(prefix) : 0);
		err = ext2fs_get_memzero(entry->e_name_len + prefix_len + 1,
					 &x->name);
		ಯದಿ (err)
			ಪ್ರತ್ಯಾಗಮ err;
		ಯದಿ (prefix)
			memcpy(x->name, prefix, prefix_len);
		ಯದಿ (entry->e_name_len)
			memcpy(x->name + prefix_len,
			       (ವರ್ಣ *)entry + ಪರಿಮಾಣ(*entry),
			       entry->e_name_len);
		x->short_name = x->name + prefix_len;
		x->name_index = entry->e_name_index;

		/* Check & copy value */
		ಯದಿ (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    entry->e_ಮೌಲ್ಯ_inum != 0)
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_BAD_EA_BLOCK_ಅಂಕೆ;

		ಯದಿ (entry->e_ಮೌಲ್ಯ_inum == 0) {
			ಯದಿ (entry->e_ಮೌಲ್ಯ_size > remain)
				ಪ್ರತ್ಯಾಗಮ EXT2_ET_EA_BAD_ಮೌಲ್ಯ_SIZE;

			ಯದಿ (entry->e_ಮೌಲ್ಯ_offs + entry->e_ಮೌಲ್ಯ_size > ಮೌಲ್ಯಗಳು_size)
				ಪ್ರತ್ಯಾಗಮ EXT2_ET_EA_BAD_ಮೌಲ್ಯ_OFFSET;

			ಯದಿ (entry->e_ಮೌಲ್ಯ_size > 0 &&
			    ಮೌಲ್ಯ_start + entry->e_ಮೌಲ್ಯ_offs <
			    (ವರ್ಣ *)end + ಪರಿಮಾಣ(__u32))
				ಪ್ರತ್ಯಾಗಮ EXT2_ET_EA_BAD_ಮೌಲ್ಯ_OFFSET;

			remain -= entry->e_ಮೌಲ್ಯ_size;

			err = ext2fs_get_mem(entry->e_ಮೌಲ್ಯ_size, &x->ಮೌಲ್ಯ);
			ಯದಿ (err)
				ಪ್ರತ್ಯಾಗಮ err;
			memcpy(x->ಮೌಲ್ಯ, ಮೌಲ್ಯ_start + entry->e_ಮೌಲ್ಯ_offs,
			       entry->e_ಮೌಲ್ಯ_size);
		} ಅನ್ಯಥಾ {
			ಸಂರಚನಾ ext2_inode *ea_inode;
			ext2_file_t ea_file;

			ಯದಿ (entry->e_ಮೌಲ್ಯ_offs != 0)
				ಪ್ರತ್ಯಾಗಮ EXT2_ET_EA_BAD_ಮೌಲ್ಯ_OFFSET;

			ಯದಿ (entry->e_ಮೌಲ್ಯ_size > (64 * 1024))
				ಪ್ರತ್ಯಾಗಮ EXT2_ET_EA_BAD_ಮೌಲ್ಯ_SIZE;

			err = ext2fs_get_mem(entry->e_ಮೌಲ್ಯ_size, &x->ಮೌಲ್ಯ);
			ಯದಿ (err)
				ಪ್ರತ್ಯಾಗಮ err;

			err = ext2fs_file_open(handle->fs, entry->e_ಮೌಲ್ಯ_inum,
					       0, &ea_file);
			ಯದಿ (err)
				ಪ್ರತ್ಯಾಗಮ err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			ಯದಿ ((ea_inode->i_flags & EXT4_INLINE_DATA_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_ಎಣಿಕೆ == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			ಅನ್ಯಥಾ ಯದಿ ((__u64) ext2fs_file_get_size(ea_file) !=
				 entry->e_ಮೌಲ್ಯ_size)
				err = EXT2_ET_EA_BAD_ಮೌಲ್ಯ_SIZE;
			ಅನ್ಯಥಾ
				err = ext2fs_file_read(ea_file, x->ಮೌಲ್ಯ,
						       entry->e_ಮೌಲ್ಯ_size, 0);
			ext2fs_file_close(ea_file);
			ಯದಿ (err)
				ಪ್ರತ್ಯಾಗಮ err;
		}

		x->ea_ino = entry->e_ಮೌಲ್ಯ_inum;
		x->ಮೌಲ್ಯ_len = entry->e_ಮೌಲ್ಯ_size;

		/* e_hash may be 0 in older inode's ea */
		ಯದಿ (entry->e_hash != 0) {
			__u32 hash, signed_hash;

			ಶೂನ್ಯ *data = (entry->e_ಮೌಲ್ಯ_inum != 0) ?
					0 : ಮೌಲ್ಯ_start + entry->e_ಮೌಲ್ಯ_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, entry,
							  data, &hash,
							  &signed_hash);
			ಯದಿ (err)
				ಪ್ರತ್ಯಾಗಮ err;
			ಯದಿ ((entry->e_hash != hash) &&
			    (entry->e_hash != signed_hash)) {
				ಸಂರಚನಾ ext2_inode child;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							entry->e_ಮೌಲ್ಯ_inum,
							&child);
				ಯದಿ (err)
					ಪ್ರತ್ಯಾಗಮ err;
				ಯದಿ (child.i_mtime != handle->ino ||
				    child.i_generation != inode->i_generation)
					ಪ್ರತ್ಯಾಗಮ EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->ಎಣಿಕೆ++;
		entry = EXT2_EXT_ATTR_NEXT(entry);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ xattrs_free_ಕೀಲಿಗಳು(ಸಂರಚನಾ ext2_xattr_handle *h)
{
	ಸಂರಚನಾ ext2_xattr *a = h->attrs;
	ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = 0; i < h->capacity; i++) {
		ಯದಿ (a[i].name)
			ext2fs_free_mem(&a[i].name);
		ಯದಿ (a[i].ಮೌಲ್ಯ)
			ext2fs_free_mem(&a[i].ಮೌಲ್ಯ);
	}
	h->ಎಣಿಕೆ = 0;
	h->ibody_ಎಣಿಕೆ = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(ಸಂರಚನಾ ext2_xattr_handle *handle,
				   ಸಂರಚನಾ ext2_inode_large *inode)
{
	ಸಂರಚನಾ ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	unsigned ಪೂರ್ಣಾಙ್ಕ storage_size;
	ವರ್ಣ *start, *block_buf = ರಿಕ್ತ;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_ಕೀಲಿಗಳು(handle);

	/* Does the inode have space for EA? */
	ಯದಿ (inode->i_extra_isize < ಪರಿಮಾಣ(inode->i_extra_isize) ||
	    EXT2_INODE_SIZE(handle->fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  ಪರಿಮಾಣ(__u32))
		ಗಚ್ಛ read_ea_block;
	ಯದಿ (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		ಗಚ್ಛ out;
	}

	/* Look for EA in the inode */
	memcpy(&ea_inode_magic, ((ವರ್ಣ *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, ಪರಿಮಾಣ(__u32));
	ಯದಿ (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_size = EXT2_INODE_SIZE(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_SIZE - inode->i_extra_isize -
			ಪರಿಮಾಣ(__u32);
		start = ((ವರ್ಣ *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + ಪರಿಮಾಣ(__u32);

		err = read_xattrs_from_buffer(handle, inode,
					(ಸಂರಚನಾ ext2_ext_attr_entry *) start,
					storage_size, start);
		ಯದಿ (err)
			ಗಚ್ಛ out;

		handle->ibody_ಎಣಿಕೆ = handle->ಎಣಿಕೆ;
	}

read_ea_block:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_block(handle->fs, EXT2_INODE(inode));
	ಯದಿ (blk != 0) {
		ಯದಿ ((blk < handle->fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_ಎಣಿಕೆ(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_ಅಂಕೆ;
			ಗಚ್ಛ out;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &block_buf);
		ಯದಿ (err)
			ಗಚ್ಛ out;

		err = ext2fs_read_ext_attr3(handle->fs, blk, block_buf,
					    handle->ino);
		ಯದಿ (err)
			ಗಚ್ಛ out3;

		/* We only know how to deal with v2 EA blocks */
		header = (ಸಂರಚನಾ ext2_ext_attr_header *) block_buf;
		ಯದಿ (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			ಗಚ್ಛ out3;
		}

		/* Read EAs */
		storage_size = handle->fs->blocksize -
			ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_ext_attr_header);
		start = block_buf + ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_ext_attr_header);
		err = read_xattrs_from_buffer(handle, inode,
					(ಸಂರಚನಾ ext2_ext_attr_entry *) start,
					storage_size, block_buf);
	}

out3:
	ಯದಿ (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	ಪ್ರತ್ಯಾಗಮ err;
}

errcode_t ext2fs_xattrs_read(ಸಂರಚನಾ ext2_xattr_handle *handle)
{
	ಸಂರಚನಾ ext2_inode_large *inode;
	size_t inode_size = EXT2_INODE_SIZE(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	ಯದಿ (inode_size < ಪರಿಮಾಣ(*inode))
		inode_size = ಪರಿಮಾಣ(*inode);
	err = ext2fs_get_memzero(inode_size, &inode);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ err;

	err = ext2fs_read_inode_full(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_SIZE(handle->fs->super));
	ಯದಿ (err)
		ಗಚ್ಛ out;

	err = ext2fs_xattrs_read_inode(handle, inode);

out:
	ext2fs_free_mem(&inode);

	ಪ್ರತ್ಯಾಗಮ err;
}

errcode_t ext2fs_xattrs_iterate(ಸಂರಚನಾ ext2_xattr_handle *h,
				ಪೂರ್ಣಾಙ್ಕ (*func)(ವರ್ಣ *name, ವರ್ಣ *ಮೌಲ್ಯ,
					    size_t ಮೌಲ್ಯ_len, ಶೂನ್ಯ *data),
				ಶೂನ್ಯ *data)
{
	ಸಂರಚನಾ ext2_xattr *x;
	ಪೂರ್ಣಾಙ್ಕ dirty = 0;
	ಪೂರ್ಣಾಙ್ಕ ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	ಪ್ರತಿ (x = h->attrs; x < h->attrs + h->ಎಣಿಕೆ; x++) {
		ret = func(x->name, x->ಮೌಲ್ಯ, x->ಮೌಲ್ಯ_len, data);
		ಯದಿ (ret & XATTR_CHANGED)
			dirty = 1;
		ಯದಿ (ret & XATTR_ABORT)
			ಭಙ್ಗ;
	}

	ಯದಿ (dirty)
		ಪ್ರತ್ಯಾಗಮ ext2fs_xattrs_write(h);
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_xattr_get(ಸಂರಚನಾ ext2_xattr_handle *h, ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ,
			   ಶೂನ್ಯ **ಮೌಲ್ಯ, size_t *ಮೌಲ್ಯ_len)
{
	ಸಂರಚನಾ ext2_xattr *x;
	ವರ್ಣ *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	ಪ್ರತಿ (x = h->attrs; x < h->attrs + h->ಎಣಿಕೆ; x++) {
		ಯದಿ (strcmp(x->name, ಕೀಲಿ))
			ಅನುವರ್ತ;

		ಯದಿ (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(ಕೀಲಿ, "system.posix_acl_default") == 0) ||
		     (strcmp(ಕೀಲಿ, "system.posix_acl_access") == 0))) {
			err = convert_disk_buffer_to_posix_acl(x->ಮೌಲ್ಯ, x->ಮೌಲ್ಯ_len,
							       ಮೌಲ್ಯ, ಮೌಲ್ಯ_len);
			ಪ್ರತ್ಯಾಗಮ err;
		} ಅನ್ಯಥಾ {
			err = ext2fs_get_mem(x->ಮೌಲ್ಯ_len, &val);
			ಯದಿ (err)
				ಪ್ರತ್ಯಾಗಮ err;
			memcpy(val, x->ಮೌಲ್ಯ, x->ಮೌಲ್ಯ_len);
			*ಮೌಲ್ಯ = val;
			*ಮೌಲ್ಯ_len = x->ಮೌಲ್ಯ_len;
			ಪ್ರತ್ಯಾಗಮ 0;
		}
	}

	ಪ್ರತ್ಯಾಗಮ EXT2_ET_EA_ಕೀಲಿ_NOT_FOUND;
}

errcode_t ext2fs_xattr_inode_max_size(ext2_filsys fs, ext2_ino_t ino,
				      size_t *size)
{
	ಸಂರಚನಾ ext2_ext_attr_entry *entry;
	ಸಂರಚನಾ ext2_inode_large *inode;
	__u32 ea_inode_magic;
	unsigned ಪೂರ್ಣಾಙ್ಕ minoff;
	ವರ್ಣ *start;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_SIZE(fs->super);
	ಯದಿ (i < ಪರಿಮಾಣ(*inode))
		i = ಪರಿಮಾಣ(*inode);
	err = ext2fs_get_memzero(i, &inode);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ err;

	err = ext2fs_read_inode_full(fs, ino, (ಸಂರಚನಾ ext2_inode *)inode,
				     EXT2_INODE_SIZE(fs->super));
	ಯದಿ (err)
		ಗಚ್ಛ out;

	/* Does the inode have size for EA? */
	ಯದಿ (EXT2_INODE_SIZE(fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  ಪರಿಮಾಣ(__u32)) {
		err = EXT2_ET_INLINE_DATA_NO_SPACE;
		ಗಚ್ಛ out;
	}

	minoff = EXT2_INODE_SIZE(fs->super) - ಪರಿಮಾಣ(*inode) - ಪರಿಮಾಣ(__u32);
	memcpy(&ea_inode_magic, ((ವರ್ಣ *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, ಪರಿಮಾಣ(__u32));
	ಯದಿ (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		start= ((ವರ್ಣ *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + ಪರಿಮಾಣ(__u32);
		entry = (ಸಂರಚನಾ ext2_ext_attr_entry *) start;
		ಯಾವತ್ (!EXT2_EXT_IS_LAST_ENTRY(entry)) {
			ಯದಿ (!entry->e_ಮೌಲ್ಯ_inum && entry->e_ಮೌಲ್ಯ_size) {
				unsigned ಪೂರ್ಣಾಙ್ಕ offs = entry->e_ಮೌಲ್ಯ_offs;
				ಯದಿ (offs < minoff)
					minoff = offs;
			}
			entry = EXT2_EXT_ATTR_NEXT(entry);
		}
		*size = minoff - ((ವರ್ಣ *)entry - (ವರ್ಣ *)start) - ಪರಿಮಾಣ(__u32);
	} ಅನ್ಯಥಾ {
		/* no xattr.  return a maximum size */
		*size = EXT2_EXT_ATTR_SIZE(minoff -
					   EXT2_EXT_ATTR_LEN(strlen("data")) -
					   EXT2_EXT_ATTR_ROUND - ಪರಿಮಾಣ(__u32));
	}

out:
	ext2fs_free_mem(&inode);
	ಪ್ರತ್ಯಾಗಮ err;
}

ಸ್ಥಿರ errcode_t xattr_create_ea_inode(ext2_filsys fs, ಧ್ರುವ ಶೂನ್ಯ *ಮೌಲ್ಯ,
				       size_t ಮೌಲ್ಯ_len, ext2_ino_t *ea_ino)
{
	ಸಂರಚನಾ ext2_inode inode;
	ext2_ino_t ino;
	ext2_file_t file;
	__u32 hash;
	errcode_t ret;

	ret = ext2fs_new_inode(fs, 0, 0, 0, &ino);
	ಯದಿ (ret)
		ಪ್ರತ್ಯಾಗಮ ret;

	memset(&inode, 0, ಪರಿಮಾಣ(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	ಯದಿ (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_size = 0;
	inode.i_mode = LINUX_S_IFREG | 0600;
	inode.i_links_ಎಣಿಕೆ = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	ಯದಿ (ret)
		ಪ್ರತ್ಯಾಗಮ ret;
	/*
	 * ref_count and hash utilize inode's i_*time fields.
	 * ext2fs_write_new_inode() call above initializes these fields with
	 * current time. That's why ref count and hash updates are done
	 * separately below.
	 */
	ext2fs_set_ea_inode_ref(&inode, 1);
	hash = ext2fs_crc32c_le(fs->csum_seed, ಮೌಲ್ಯ, ಮೌಲ್ಯ_len);
	ext2fs_set_ea_inode_hash(&inode, hash);

	ret = ext2fs_write_inode(fs, ino, &inode);
	ಯದಿ (ret)
		ಪ್ರತ್ಯಾಗಮ ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	ಯದಿ (ret)
		ಪ್ರತ್ಯಾಗಮ ret;
	ret = ext2fs_file_write(file, ಮೌಲ್ಯ, ಮೌಲ್ಯ_len, ರಿಕ್ತ);
	ext2fs_file_close(file);
	ಯದಿ (ret)
		ಪ್ರತ್ಯಾಗಮ ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	ಸಂರಚನಾ ext2_inode_large inode;
	__u64 ref_ಎಣಿಕೆ;
	errcode_t ret;

	ret = ext2fs_read_inode_full(fs, ino, (ಸಂರಚನಾ ext2_inode *)&inode,
				     ಪರಿಮಾಣ(inode));
	ಯದಿ (ret)
		ಗಚ್ಛ out;

	ref_ಎಣಿಕೆ = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_ಎಣಿಕೆ--;
	ext2fs_set_ea_inode_ref(EXT2_INODE(&inode), ref_ಎಣಿಕೆ);

	ಯದಿ (ref_ಎಣಿಕೆ)
		ಗಚ್ಛ write_out;

	inode.i_links_ಎಣಿಕೆ = 0;
	ext2fs_set_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	ಯದಿ (ret)
		ಗಚ್ಛ write_out;

	ಯದಿ (ext2fs_inode_has_valid_blocks2(fs, (ಸಂರಚನಾ ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (ಸಂರಚನಾ ext2_inode *)&inode, ರಿಕ್ತ,
				   0, ~0ULL);
		ಯದಿ (ret)
			ಗಚ್ಛ out;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_out:
	ret = ext2fs_write_inode_full(fs, ino, (ಸಂರಚನಾ ext2_inode *)&inode,
				      ಪರಿಮಾಣ(inode));
out:
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ errcode_t xattr_update_entry(ext2_filsys fs, ಸಂರಚನಾ ext2_xattr *x,
				    ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *short_name,
				    ಪೂರ್ಣಾಙ್ಕ index, ಧ್ರುವ ಶೂನ್ಯ *ಮೌಲ್ಯ,
				    size_t ಮೌಲ್ಯ_len, ಪೂರ್ಣಾಙ್ಕ in_inode)
{
	ext2_ino_t ea_ino = 0;
	ಶೂನ್ಯ *new_ಮೌಲ್ಯ = ರಿಕ್ತ;
	ವರ್ಣ *new_name = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ name_len;
	errcode_t ret;

	ಯದಿ (!x->name) {
		name_len = strlen(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		ಯದಿ (ret)
			ಗಚ್ಛ fail;
		memcpy(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(ಮೌಲ್ಯ_len, &new_ಮೌಲ್ಯ);
	ಯದಿ (ret)
		ಗಚ್ಛ fail;
	memcpy(new_ಮೌಲ್ಯ, ಮೌಲ್ಯ, ಮೌಲ್ಯ_len);

	ಯದಿ (in_inode) {
		ret = xattr_create_ea_inode(fs, ಮೌಲ್ಯ, ಮೌಲ್ಯ_len, &ea_ino);
		ಯದಿ (ret)
			ಗಚ್ಛ fail;
	}

	ಯದಿ (x->ea_ino) {
		ret = xattr_inode_dec_ref(fs, x->ea_ino);
		ಯದಿ (ret)
			ಗಚ್ಛ fail;
	}

	ಯದಿ (!x->name) {
		x->name = new_name;
		x->short_name = new_name + (short_name  - name);
	}
	x->name_index = index;

	ಯದಿ (x->ಮೌಲ್ಯ)
		ext2fs_free_mem(&x->ಮೌಲ್ಯ);
	x->ಮೌಲ್ಯ = new_ಮೌಲ್ಯ;
	x->ಮೌಲ್ಯ_len = ಮೌಲ್ಯ_len;
	x->ea_ino = ea_ino;
	ಪ್ರತ್ಯಾಗಮ 0;
fail:
	ಯದಿ (new_name)
		ext2fs_free_mem(&new_name);
	ಯದಿ (new_ಮೌಲ್ಯ)
		ext2fs_free_mem(&new_ಮೌಲ್ಯ);
	ಯದಿ (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ xattr_find_position(ಸಂರಚನಾ ext2_xattr *attrs, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ,
			       ಧ್ರುವ ವರ್ಣ *shortname, ಪೂರ್ಣಾಙ್ಕ name_idx)
{
	ಸಂರಚನಾ ext2_xattr *x;
	ಪೂರ್ಣಾಙ್ಕ i;
	ಪೂರ್ಣಾಙ್ಕ shortname_len, x_shortname_len;

	shortname_len = strlen(shortname);

	ಪ್ರತಿ (i = 0, x = attrs; i < ಎಣಿಕೆ; i++, x++) {
		ಯದಿ (name_idx < x->name_index)
			ಭಙ್ಗ;
		ಯದಿ (name_idx > x->name_index)
			ಅನುವರ್ತ;

		x_shortname_len = strlen(x->short_name);
		ಯದಿ (shortname_len < x_shortname_len)
			ಭಙ್ಗ;
		ಯದಿ (shortname_len > x_shortname_len)
			ಅನುವರ್ತ;

		ಯದಿ (memcmp(shortname, x->short_name, shortname_len) <= 0)
			ಭಙ್ಗ;
	}
	ಪ್ರತ್ಯಾಗಮ i;
}

ಸ್ಥಿರ errcode_t xattr_array_update(ಸಂರಚನಾ ext2_xattr_handle *h,
				    ಧ್ರುವ ವರ್ಣ *name,
				    ಧ್ರುವ ಶೂನ್ಯ *ಮೌಲ್ಯ, size_t ಮೌಲ್ಯ_len,
				    ಪೂರ್ಣಾಙ್ಕ ibody_free, ಪೂರ್ಣಾಙ್ಕ block_free,
				    ಪೂರ್ಣಾಙ್ಕ old_idx, ಪೂರ್ಣಾಙ್ಕ in_inode)
{
	ಸಂರಚನಾ ext2_xattr tmp;
	ಪೂರ್ಣಾಙ್ಕ add_to_ibody;
	ಪೂರ್ಣಾಙ್ಕ needed;
	ಪೂರ್ಣಾಙ್ಕ name_len, name_idx = 0;
	ಧ್ರುವ ವರ್ಣ *shortname = name;
	ಪೂರ್ಣಾಙ್ಕ new_idx;
	ಪೂರ್ಣಾಙ್ಕ ret;

	find_ea_index(name, &shortname, &name_idx);
	name_len = strlen(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	ಯದಿ (!in_inode)
		needed += EXT2_EXT_ATTR_SIZE(ಮೌಲ್ಯ_len);

	ಯದಿ (old_idx >= 0 && old_idx < h->ibody_ಎಣಿಕೆ) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		ಯದಿ (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_SIZE(
						h->attrs[old_idx].ಮೌಲ್ಯ_len);
	}

	ಯದಿ (needed <= ibody_free) {
		ಯದಿ (old_idx < 0) {
			new_idx = h->ibody_ಎಣಿಕೆ;
			add_to_ibody = 1;
			ಗಚ್ಛ add_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, ಮೌಲ್ಯ,
					 ಮೌಲ್ಯ_len, in_inode);
		ಯದಿ (ret)
			ಪ್ರತ್ಯಾಗಮ ret;
		ಯದಿ (h->ibody_ಎಣಿಕೆ <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_ಎಣಿಕೆ + 1,
				h->attrs + h->ibody_ಎಣಿಕೆ,
				(old_idx - h->ibody_ಎಣಿಕೆ) * ಪರಿಮಾಣ(*h->attrs));
			h->attrs[h->ibody_ಎಣಿಕೆ] = tmp;
			h->ibody_ಎಣಿಕೆ++;
		}
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (h->ibody_ಎಣಿಕೆ <= old_idx) {
		block_free += EXT2_EXT_ATTR_LEN(name_len);
		ಯದಿ (!h->attrs[old_idx].ea_ino)
			block_free +=
				EXT2_EXT_ATTR_SIZE(h->attrs[old_idx].ಮೌಲ್ಯ_len);
	}

	ಯದಿ (needed > block_free)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_EA_NO_SPACE;

	ಯದಿ (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, ಮೌಲ್ಯ,
					 ಮೌಲ್ಯ_len, in_inode);
		ಯದಿ (ret)
			ಪ್ರತ್ಯಾಗಮ ret;
		ಯದಿ (old_idx < h->ibody_ಎಣಿಕೆ) {
			/*
			 * Move entry from ibody to the block. Note that
			 * entries in the block are sorted.
			 */
			new_idx = xattr_find_position(h->attrs + h->ibody_ಎಣಿಕೆ,
						      h->ಎಣಿಕೆ - h->ibody_ಎಣಿಕೆ,
						      shortname, name_idx);
			new_idx += h->ibody_ಎಣಿಕೆ - 1;
			tmp = h->attrs[old_idx];
			memmove(h->attrs + old_idx, h->attrs + old_idx + 1,
				(new_idx - old_idx) * ಪರಿಮಾಣ(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_ಎಣಿಕೆ--;
		}
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	new_idx = xattr_find_position(h->attrs + h->ibody_ಎಣಿಕೆ,
				      h->ಎಣಿಕೆ - h->ibody_ಎಣಿಕೆ,
				      shortname, name_idx);
	new_idx += h->ibody_ಎಣಿಕೆ;
	add_to_ibody = 0;

add_new:
	ಯದಿ (h->ಎಣಿಕೆ == h->capacity) {
		ret = ext2fs_xattrs_expand(h, 4);
		ಯದಿ (ret)
			ಪ್ರತ್ಯಾಗಮ ret;
	}

	ret = xattr_update_entry(h->fs, &h->attrs[h->ಎಣಿಕೆ], name, shortname,
				 name_idx, ಮೌಲ್ಯ, ಮೌಲ್ಯ_len, in_inode);
	ಯದಿ (ret)
		ಪ್ರತ್ಯಾಗಮ ret;

	tmp = h->attrs[h->ಎಣಿಕೆ];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->ಎಣಿಕೆ - new_idx)*ಪರಿಮಾಣ(*h->attrs));
	h->attrs[new_idx] = tmp;
	ಯದಿ (add_to_ibody)
		h->ibody_ಎಣಿಕೆ++;
	h->ಎಣಿಕೆ++;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ space_used(ಸಂರಚನಾ ext2_xattr *attrs, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ)
{
	ಪೂರ್ಣಾಙ್ಕ total = 0;
	ಸಂರಚನಾ ext2_xattr *x;
	ಪೂರ್ಣಾಙ್ಕ i, len;

	ಪ್ರತಿ (i = 0, x = attrs; i < ಎಣಿಕೆ; i++, x++) {
		len = strlen(x->short_name);
		total += EXT2_EXT_ATTR_LEN(len);
		ಯದಿ (!x->ea_ino)
			total += EXT2_EXT_ATTR_SIZE(x->ಮೌಲ್ಯ_len);
	}
	ಪ್ರತ್ಯಾಗಮ total;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#define EXT4_XATTR_MIN_LARGE_EA_SIZE(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_ext_attr_header) - 4)

errcode_t ext2fs_xattr_set(ಸಂರಚನಾ ext2_xattr_handle *h,
			   ಧ್ರುವ ವರ್ಣ *name,
			   ಧ್ರುವ ಶೂನ್ಯ *ಮೌಲ್ಯ,
			   size_t ಮೌಲ್ಯ_len)
{
	ext2_filsys fs = h->fs;
	ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ inode_size = EXT2_INODE_SIZE(fs->super);
	ಸಂರಚನಾ ext2_inode_large *inode = ರಿಕ್ತ;
	ಸಂರಚನಾ ext2_xattr *x;
	ವರ್ಣ *new_ಮೌಲ್ಯ;
	ಪೂರ್ಣಾಙ್ಕ ibody_free, block_free;
	ಪೂರ್ಣಾಙ್ಕ in_inode = 0;
	ಪೂರ್ಣಾಙ್ಕ old_idx = -1;
	ಪೂರ್ಣಾಙ್ಕ extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(ಮೌಲ್ಯ_len, &new_ಮೌಲ್ಯ);
	ಯದಿ (ret)
		ಪ್ರತ್ಯಾಗಮ ret;
	ಯದಿ (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((strcmp(name, "system.posix_acl_default") == 0) ||
	     (strcmp(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_to_disk_buffer(ಮೌಲ್ಯ, ಮೌಲ್ಯ_len,
						       new_ಮೌಲ್ಯ, &ಮೌಲ್ಯ_len);
		ಯದಿ (ret)
			ಗಚ್ಛ out;
	} ಅನ್ಯಥಾ ಯದಿ (ಮೌಲ್ಯ_len)
		memcpy(new_ಮೌಲ್ಯ, ಮೌಲ್ಯ, ಮೌಲ್ಯ_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	ಪ್ರತಿ (x = h->attrs; x < h->attrs + h->ಎಣಿಕೆ; x++) {
		ಯದಿ (!strcmp(x->name, name)) {
			ಯದಿ (!x->ea_ino && x->ಮೌಲ್ಯ_len == ಮೌಲ್ಯ_len &&
			    (!ಮೌಲ್ಯ_len ||
			     !memcmp(x->ಮೌಲ್ಯ, new_ಮೌಲ್ಯ, ಮೌಲ್ಯ_len))) {
				ret = 0;
				ಗಚ್ಛ out;
			}
			old_idx = x - h->attrs;
			ಭಙ್ಗ;
		}
	}

	ret = ext2fs_get_memzero(inode_size, &inode);
	ಯದಿ (ret)
		ಗಚ್ಛ out;
	ret = ext2fs_read_inode_full(fs, h->ino,
				     (ಸಂರಚನಾ ext2_inode *)inode,
				     inode_size);
	ಯದಿ (ret)
		ಗಚ್ಛ out;
	ಯದಿ (inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		extra_isize = inode->i_extra_isize;
		ಯದಿ (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			ಯದಿ (extra_isize == 0)
				extra_isize = ಪರಿಮಾಣ(__u32);
		}
		ibody_free = inode_size - EXT2_GOOD_OLD_INODE_SIZE;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= ಪರಿಮಾಣ(__u32) * 2;
		ibody_free -= space_used(h->attrs, h->ibody_ಎಣಿಕೆ);
	} ಅನ್ಯಥಾ
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	ಯದಿ (strcmp(name, "system.data") == 0) {
		ಯದಿ (h->ibody_ಎಣಿಕೆ <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			ಗಚ್ಛ out;
		}
		ret = xattr_array_update(h, name, new_ಮೌಲ್ಯ, ಮೌಲ್ಯ_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		ಯದಿ (ret)
			ಗಚ್ಛ out;
		ಗಚ್ಛ write_out;
	}

	block_free = fs->blocksize;
	block_free -= ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_ext_attr_header);
	/* Final null entry. */
	block_free -= ಪರಿಮಾಣ(__u32);
	block_free -= space_used(h->attrs + h->ibody_ಎಣಿಕೆ,
				 h->ಎಣಿಕೆ - h->ibody_ಎಣಿಕೆ);

	ಯದಿ (ext2fs_has_feature_ea_inode(fs->super) &&
	    ಮೌಲ್ಯ_len > EXT4_XATTR_MIN_LARGE_EA_SIZE(fs->blocksize))
		in_inode = 1;

	ret = xattr_array_update(h, name, new_ಮೌಲ್ಯ, ಮೌಲ್ಯ_len, ibody_free,
				 block_free, old_idx, in_inode);
	ಯದಿ (ret == EXT2_ET_EA_NO_SPACE && !in_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_ಮೌಲ್ಯ, ಮೌಲ್ಯ_len,
			ibody_free, block_free, old_idx, 1 /* in_inode */);
	ಯದಿ (ret)
		ಗಚ್ಛ out;

write_out:
	ret = ext2fs_xattrs_write(h);
out:
	ಯದಿ (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_ಮೌಲ್ಯ);
	ಪ್ರತ್ಯಾಗಮ ret;
}

errcode_t ext2fs_xattr_remove(ಸಂರಚನಾ ext2_xattr_handle *handle,
			      ಧ್ರುವ ವರ್ಣ *ಕೀಲಿ)
{
	ಸಂರಚನಾ ext2_xattr *x;
	ಸಂರಚನಾ ext2_xattr *end = handle->attrs + handle->ಎಣಿಕೆ;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	ಪ್ರತಿ (x = handle->attrs; x < end; x++) {
		ಯದಿ (strcmp(x->name, ಕೀಲಿ) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->ಮೌಲ್ಯ);
			ಯದಿ (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (end - x - 1)*ಪರಿಮಾಣ(*x));
			memset(end - 1, 0, ಪರಿಮಾಣ(*end));
			ಯದಿ (x < handle->attrs + handle->ibody_ಎಣಿಕೆ)
				handle->ibody_ಎಣಿಕೆ--;
			handle->ಎಣಿಕೆ--;
			ಪ್ರತ್ಯಾಗಮ ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_xattr_remove_all(ಸಂರಚನಾ ext2_xattr_handle *handle)
{
	ಸಂರಚನಾ ext2_xattr *x;
	ಸಂರಚನಾ ext2_xattr *end = handle->attrs + handle->ಎಣಿಕೆ;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	ಪ್ರತಿ (x = handle->attrs; x < end; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->ಮೌಲ್ಯ);
		ಯದಿ (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_ಎಣಿಕೆ = 0;
	handle->ಎಣಿಕೆ = 0;
	ಪ್ರತ್ಯಾಗಮ ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     ಸಂರಚನಾ ext2_xattr_handle **handle)
{
	ಸಂರಚನಾ ext2_xattr_handle *h;
	errcode_t err;

	ಯದಿ (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_data(fs->super))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(ಪರಿಮಾಣ(*h), &h);
	ಯದಿ (err)
		ಪ್ರತ್ಯಾಗಮ err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->capacity = 4;
	err = ext2fs_get_arrayzero(h->capacity, ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_xattr),
				   &h->attrs);
	ಯದಿ (err) {
		ext2fs_free_mem(&h);
		ಪ್ರತ್ಯಾಗಮ err;
	}
	h->ಎಣಿಕೆ = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_xattrs_close(ಸಂರಚನಾ ext2_xattr_handle **handle)
{
	ಸಂರಚನಾ ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_ಕೀಲಿಗಳು(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_xattrs_ಎಣಿಕೆ(ಸಂರಚನಾ ext2_xattr_handle *handle, size_t *ಎಣಿಕೆ)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*ಎಣಿಕೆ = handle->ಎಣಿಕೆ;
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_xattrs_flags(ಸಂರಚನಾ ext2_xattr_handle *handle,
			      unsigned ಪೂರ್ಣಾಙ್ಕ *new_flags, unsigned ಪೂರ್ಣಾಙ್ಕ *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	ಯದಿ (old_flags)
		*old_flags = handle->flags;
	ಯದಿ (new_flags)
		handle->flags = *new_flags;
	ಪ್ರತ್ಯಾಗಮ 0;
}
