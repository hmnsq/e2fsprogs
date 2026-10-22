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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <time.h>

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2_ext_attr.h"
#ଅନ୍ତର୍ଭାଵ "ext4_acl.h"

#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

ସ୍ଥିର errcode_t read_ea_inode_hash(ext2_filsys fs, ext2_ino_t ino, __u32 *hash)
{
	ସଂରଚନା ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	ଫେରନ୍ତୁ 0;
}

#define NAME_HASH_SHIFT 5
#define ମୂଲ୍ୟ_HASH_SHIFT 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_entry(ସଂରଚନା ext2_ext_attr_entry *entry, ଶୂନ୍ୟ *data)
{
	__u32 hash = 0;
	unsigned ଅକ୍ଷର *name = (((unsigned ଅକ୍ଷର *) entry) +
			       ଆକାର(ସଂରଚନା ext2_ext_attr_entry));
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା n;

	ପାଇଁ (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*ଆକାର(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	ଯଦି (entry->e_ମୂଲ୍ୟ_inum == 0 && entry->e_ମୂଲ୍ୟ_size != 0) {
		__u32 *ମୂଲ୍ୟ = (__u32 *)data;
		ପାଇଁ (n = (entry->e_ମୂଲ୍ୟ_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << ମୂଲ୍ୟ_HASH_SHIFT) ^
			       (hash >> (8*ଆକାର(hash) - ମୂଲ୍ୟ_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*ମୂଲ୍ୟ++);
		}
	}

	ଫେରନ୍ତୁ hash;
}

__u32 ext2fs_ext_attr_hash_entry_signed(ସଂରଚନା ext2_ext_attr_entry *entry,
					ଶୂନ୍ୟ *data)
{
	__u32 hash = 0;
	signed ଅକ୍ଷର *name = (((signed ଅକ୍ଷର *) entry) +
			     ଆକାର(ସଂରଚନା ext2_ext_attr_entry));
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା n;

	ପାଇଁ (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*ଆକାର(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	ଯଦି (entry->e_ମୂଲ୍ୟ_inum == 0 && entry->e_ମୂଲ୍ୟ_size != 0) {
		__u32 *ମୂଲ୍ୟ = (__u32 *)data;
		ପାଇଁ (n = (entry->e_ମୂଲ୍ୟ_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << ମୂଲ୍ୟ_HASH_SHIFT) ^
			       (hash >> (8*ଆକାର(hash) - ମୂଲ୍ୟ_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*ମୂଲ୍ୟ++);
		}
	}

	ଫେରନ୍ତୁ hash;
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
				      ସଂରଚନା ext2_ext_attr_entry *entry,
				      ଶୂନ୍ୟ *data, __u32 *hash,
				      __u32 *signed_hash)
{
	*hash = ext2fs_ext_attr_hash_entry(entry, data);
	ଯଦି (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_entry_signed(entry, data);

	ଯଦି (entry->e_ମୂଲ୍ୟ_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, entry->e_ମୂଲ୍ୟ_inum,
					    &ea_inode_hash);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;

		*hash = (*hash << ମୂଲ୍ୟ_HASH_SHIFT) ^
			(*hash >> (8*ଆକାର(*hash) - ମୂଲ୍ୟ_HASH_SHIFT)) ^
			ea_inode_hash;
		ଯଦି (signed_hash)
			*signed_hash = (*signed_hash << ମୂଲ୍ୟ_HASH_SHIFT) ^
				(*signed_hash >> (8*ଆକାର(*hash) -
						  ମୂଲ୍ୟ_HASH_SHIFT)) ^
				ea_inode_hash;
	}
	ଫେରନ୍ତୁ 0;
}

/*
 * ext2fs_ext_attr_hash_entry2()
 *
 * Compute the hash of an extended attribute.
 * This version of the function supports hashing entries that reference
 * external inodes (ea_inode feature).
 */
errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
				      ସଂରଚନା ext2_ext_attr_entry *entry,
				      ଶୂନ୍ୟ *data, __u32 *hash)
{
	ଫେରନ୍ତୁ ext2fs_ext_attr_hash_entry3(fs, entry, data, hash, ଖାଲି);
}

#undef NAME_HASH_SHIFT
#undef ମୂଲ୍ୟ_HASH_SHIFT

#define BLOCK_HASH_SHIFT 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
ଶୂନ୍ୟ ext2fs_ext_attr_block_rehash(ସଂରଚନା ext2_ext_attr_header *header,
				  ସଂରଚନା ext2_ext_attr_entry *end)
{
	ସଂରଚନା ext2_ext_attr_entry *here;
	__u32 hash = 0;

	here = (ସଂରଚନା ext2_ext_attr_entry *)(header+1);
	ଯେତେବେଳେ (here < end && !EXT2_EXT_IS_LAST_ENTRY(here)) {
		ଯଦି (!here->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			ଭାଙ୍ଗ;
		}
		hash = (hash << BLOCK_HASH_SHIFT) ^
		       (hash >> (8*ଆକାର(hash) - BLOCK_HASH_SHIFT)) ^
		       here->e_hash;
		here = EXT2_EXT_ATTR_NEXT(here);
	}
	header->h_hash = hash;
}

#undef BLOCK_HASH_SHIFT

__u32 ext2fs_get_ea_inode_hash(ସଂରଚନା ext2_inode *inode)
{
	ଫେରନ୍ତୁ inode->i_atime;
}

ଶୂନ୍ୟ ext2fs_set_ea_inode_hash(ସଂରଚନା ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(ସଂରଚନା ext2_inode *inode)
{
	ଫେରନ୍ତୁ ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_version;
}

ଶୂନ୍ୟ ext2fs_set_ea_inode_ref(ସଂରଚନା ext2_inode *inode, __u64 ref_ଗଣନା)
{
	inode->i_ctime = (__u32)(ref_ଗଣନା >> 32);
	inode->osd1.linux1.l_i_version = (__u32)ref_ଗଣନା;
}

ସ୍ଥିର errcode_t check_ext_attr_header(ସଂରଚନା ext2_ext_attr_header *header)
{
	ଯଦି ((header->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     header->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    header->h_blocks != 1)
		ଫେରନ୍ତୁ EXT2_ET_BAD_EA_HEADER;

	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block, ଶୂନ୍ୟ *buf,
				ext2_ino_t inum)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	ଯଦି (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_block_csum_verify(fs, inum, block, buf))
		csum_failed = 1;

#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	ext2fs_swap_ext_attr(buf, buf, fs->blocksize, 1);
#endif

	retval = check_ext_attr_header(buf);
	ଯଦି (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	ଫେରନ୍ତୁ retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block, ଶୂନ୍ୟ *buf)
{
	ଫେରନ୍ତୁ ext2fs_read_ext_attr3(fs, block, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, ଶୂନ୍ୟ *buf)
{
	ଫେରନ୍ତୁ ext2fs_read_ext_attr2(fs, block, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block, ଶୂନ୍ୟ *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	ଅକ୍ଷର		*write_buf;

#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	ext2fs_swap_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#ଅନ୍ୟଥା
	write_buf = (ଅକ୍ଷର *) inbuf;
#endif

	retval = ext2fs_ext_attr_block_csum_set(fs, inum, block,
			(ସଂରଚନା ext2_ext_attr_header *)write_buf);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	retval = io_channel_write_blk64(fs->io, block, 1, write_buf);
#ifdef ଶବ୍ଦଗୁଡ଼ିକ_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#endif
	ଯଦି (!retval)
		ext2fs_mark_changed(fs);
	ଫେରନ୍ତୁ retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block, ଶୂନ୍ୟ *inbuf)
{
	ଫେରନ୍ତୁ ext2fs_write_ext_attr3(fs, block, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block, ଶୂନ୍ୟ *inbuf)
{
	ଫେରନ୍ତୁ ext2fs_write_ext_attr2(fs, block, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    ଅକ୍ଷର *block_buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	ସଂରଚନା ext2_ext_attr_header *header;
	ଅକ୍ଷର	*buf = 0;

	ଯଦି ((blk >= ext2fs_blocks_ଗଣନା(fs->super)) ||
	    (blk < fs->super->s_first_data_block))
		ଫେରନ୍ତୁ EXT2_ET_BAD_EA_BLOCK_ଅଙ୍କ;

	ଯଦି (!block_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
		block_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, block_buf, inum);
	ଯଦି (retval)
		ଯାଅ errout;

	header = (ସଂରଚନା ext2_ext_attr_header *) block_buf;
	header->h_refcount += adjust;
	ଯଦି (newcount)
		*newcount = header->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, block_buf, inum);
	ଯଦି (retval)
		ଯାଅ errout;

errout:
	ଯଦି (buf)
		ext2fs_free_mem(&buf);
	ଫେରନ୍ତୁ retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    ଅକ୍ଷର *block_buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା adjust,
				    __u32 *newcount)
{
	ଫେରନ୍ତୁ ext2fs_adjust_ea_refcount3(fs, blk, block_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					ଅକ୍ଷର *block_buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା adjust,
					__u32 *newcount)
{
	ଫେରନ୍ତୁ ext2fs_adjust_ea_refcount2(fs, blk, block_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
ସଂରଚନା ext2_xattr {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା name_index;
	ଅକ୍ଷର *name;
	ଅକ୍ଷର *short_name;
	ଶୂନ୍ୟ *ମୂଲ୍ୟ;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ_len;
	ext2_ino_t ea_ino;
};

ସଂରଚନା ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	ସଂରଚନା ext2_xattr *attrs;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା capacity;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ibody_ଗଣନା;
	ext2_ino_t ino;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags;
};

ସ୍ଥିର errcode_t ext2fs_xattrs_expand(ସଂରଚନା ext2_xattr_handle *h,
				      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା expandby)
{
	ସଂରଚନା ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->capacity + expandby,
				   ଆକାର(ସଂରଚନା ext2_xattr), &new_attrs);
	ଯଦି (err)
		ଫେରନ୍ତୁ err;

	memcpy(new_attrs, h->attrs, h->capacity * ଆକାର(ସଂରଚନା ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->capacity += expandby;
	h->attrs = new_attrs;

	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା ea_name_index {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା index;
	ଅଚଳ ଅକ୍ଷର *name;
};

/* Keep these names sorted in order of decreasing specificity. */
ସ୍ଥିର ସଂରଚନା ea_name_index ea_names[] = {
	{10, "gnu."},
	{3, "system.posix_acl_default"},
	{2, "system.posix_acl_access"},
	{8, "system.richacl"},
	{6, "security."},
	{4, "trusted."},
	{7, "system."},
	{1, "user."},
	{0, ଖାଲି},
};

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *find_ea_prefix(ପୂର୍ଣ୍ଣସଂଖ୍ୟା index)
{
	ସଂରଚନା ea_name_index *e;

	ପାଇଁ (e = ea_names; e->name; e++)
		ଯଦି (e->index == index)
			ଫେରନ୍ତୁ e->name;

	ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା find_ea_index(ଅଚଳ ଅକ୍ଷର *fullname, ଅଚଳ ଅକ୍ଷର **name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *index)
{
	ସଂରଚନା ea_name_index *e;

	ପାଇଁ (e = ea_names; e->name; e++) {
		ଯଦି (strncmp(fullname, e->name, strlen(e->name)) == 0) {
			*name = fullname + strlen(e->name);
			*index = e->index;
			ଫେରନ୍ତୁ 1;
		}
	}
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       ସଂରଚନା ext2_inode_large *inode)
{
	ସଂରଚନା ext2_ext_attr_header *header;
	ଶୂନ୍ୟ *block_buf = ଖାଲି;
	blk64_t blk;
	errcode_t err;
	ସଂରଚନା ext2_inode_large i;

	/* Read inode? */
	ଯଦି (inode == ଖାଲି) {
		err = ext2fs_read_inode_full(fs, ino, (ସଂରଚନା ext2_inode *)&i,
					     ଆକାର(ସଂରଚନା ext2_inode_large));
		ଯଦି (err)
			ଫେରନ୍ତୁ err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (ସଂରଚନା ext2_inode *)inode);
	ଯଦି (blk == 0)
		ଫେରନ୍ତୁ 0;

	/* Find block, zero it, write back */
	ଯଦି ((blk < fs->super->s_first_data_block) ||
	    (blk >= ext2fs_blocks_ଗଣନା(fs->super))) {
		err = EXT2_ET_BAD_EA_BLOCK_ଅଙ୍କ;
		ଯାଅ out;
	}

	err = ext2fs_get_mem(fs->blocksize, &block_buf);
	ଯଦି (err)
		ଯାଅ out;

	err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
	ଯଦି (err)
		ଯାଅ out2;

	/* We only know how to deal with v2 EA blocks */
	header = (ସଂରଚନା ext2_ext_attr_header *) block_buf;
	ଯଦି (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_HEADER;
		ଯାଅ out2;
	}

	header->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
	ଯଦି (err)
		ଯାଅ out2;

	/* Erase link to block */
	ext2fs_file_acl_block_set(fs, (ସଂରଚନା ext2_inode *)inode, 0);
	ଯଦି (header->h_refcount == 0)
		ext2fs_block_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_blocks(fs, (ସଂରଚନା ext2_inode *)inode, 1);
	ଯଦି (err)
		ଯାଅ out2;

	/* Write inode? */
	ଯଦି (inode == &i) {
		err = ext2fs_write_inode_full(fs, ino, (ସଂରଚନା ext2_inode *)&i,
					      ଆକାର(ସଂରଚନା ext2_inode_large));
		ଯଦି (err)
			ଯାଅ out2;
	}

out2:
	ext2fs_free_mem(&block_buf);
out:
	ଫେରନ୍ତୁ err;
}

ସ୍ଥିର errcode_t prep_ea_block_for_write(ext2_filsys fs, ext2_ino_t ino,
					 ସଂରଚନା ext2_inode_large *inode)
{
	ସଂରଚନା ext2_ext_attr_header *header;
	ଶୂନ୍ୟ *block_buf = ଖାଲି;
	blk64_t blk, goal;
	errcode_t err;

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (ସଂରଚନା ext2_inode *)inode);
	ଯଦି (blk != 0) {
		ଯଦି ((blk < fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_ଗଣନା(fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_ଅଙ୍କ;
			ଯାଅ out;
		}

		err = ext2fs_get_mem(fs->blocksize, &block_buf);
		ଯଦି (err)
			ଯାଅ out;

		err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
		ଯଦି (err)
			ଯାଅ out2;

		/* We only know how to deal with v2 EA blocks */
		header = (ସଂରଚନା ext2_ext_attr_header *) block_buf;
		ଯଦି (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			ଯାଅ out2;
		}

		/* Single-user block.  We're done here. */
		ଯଦି (header->h_refcount == 1)
			ଯାଅ out2;

		/* We need to CoW the block. */
		header->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
		ଯଦି (err)
			ଯାଅ out2;
	} ଅନ୍ୟଥା {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_add_blocks(fs, (ସଂରଚନା ext2_inode *)inode,
					     1);
		ଯଦି (err)
			ଯାଅ out;
	}

	/* Allocate a block */
	goal = ext2fs_find_inode_goal(fs, ino, (ସଂରଚନା ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, ଖାଲି, &blk);
	ଯଦି (err)
		ଯାଅ out2;
	ext2fs_file_acl_block_set(fs, (ସଂରଚନା ext2_inode *)inode, blk);
out2:
	ଯଦି (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	ଫେରନ୍ତୁ err;
}


ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
posix_acl_xattr_ଗଣନା(size_t size)
{
        ଯଦି (size < ଆକାର(posix_acl_xattr_header))
                ଫେରନ୍ତୁ -1;
        size -= ଆକାର(posix_acl_xattr_header);
        ଯଦି (size % ଆକାର(posix_acl_xattr_entry))
                ଫେରନ୍ତୁ -1;
        ଫେରନ୍ତୁ size / ଆକାର(posix_acl_xattr_entry);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
ସ୍ଥିର errcode_t convert_posix_acl_to_disk_buffer(ଅଚଳ ଶୂନ୍ୟ *ମୂଲ୍ୟ, size_t size,
						  ଶୂନ୍ୟ *out_buf, size_t *size_out)
{
	ଅଚଳ posix_acl_xattr_header *header =
		(ଅଚଳ posix_acl_xattr_header*) ମୂଲ୍ୟ;
	ଅଚଳ posix_acl_xattr_entry *end, *entry =
		(ଅଚଳ posix_acl_xattr_entry *)(header+1);
	ext4_acl_header *ext_acl;
	size_t s;
	ଅକ୍ଷର *e;

	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2;

	ଯଦି (!ମୂଲ୍ୟ)
		ଫେରନ୍ତୁ EINVAL;
	ଯଦି (size < ଆକାର(posix_acl_xattr_header))
		ଫେରନ୍ତୁ ENOMEM;
	ଯଦି (header->a_version != ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION))
		ଫେରନ୍ତୁ EINVAL;

	ଗଣନା_2 = posix_acl_xattr_ଗଣନା(size);
	ext_acl = out_buf;
	ext_acl->a_version = ext2fs_cpu_to_le32(EXT4_ACL_VERSION);

	ଯଦି (ଗଣନା_2 <= 0)
		ଫେରନ୍ତୁ EINVAL;

	e = (ଅକ୍ଷର *) out_buf + ଆକାର(ext4_acl_header);
	s = ଆକାର(ext4_acl_header);
	ପାଇଁ (end = entry + ଗଣନା_2; entry != end;entry++) {
		ext4_acl_entry *disk_entry = (ext4_acl_entry*) e;
		disk_entry->e_tag = entry->e_tag;
		disk_entry->e_perm = entry->e_perm;

		ବାଛ(ext2fs_le16_to_cpu(entry->e_tag)) {
			ସ୍ଥିତି ACL_USER_OBJ:
			ସ୍ଥିତି ACL_GROUP_OBJ:
			ସ୍ଥିତି ACL_MASK:
			ସ୍ଥିତି ACL_OTHER:
				e += ଆକାର(ext4_acl_entry_short);
				s += ଆକାର(ext4_acl_entry_short);
				ଭାଙ୍ଗ;
			ସ୍ଥିତି ACL_USER:
			ସ୍ଥିତି ACL_GROUP:
				disk_entry->e_id = entry->e_id;
				e += ଆକାର(ext4_acl_entry);
				s += ଆକାର(ext4_acl_entry);
				ଭାଙ୍ଗ;
			ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
				ଫେରନ୍ତୁ EINVAL;
		}
	}
	*size_out = s;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t convert_disk_buffer_to_posix_acl(ଅଚଳ ଶୂନ୍ୟ *ମୂଲ୍ୟ, size_t size,
						  ଶୂନ୍ୟ **out_buf, size_t *size_out)
{
	posix_acl_xattr_header *header;
	posix_acl_xattr_entry *entry;
	ଅଚଳ ext4_acl_header *ext_acl = (ଅଚଳ ext4_acl_header *) ମୂଲ୍ୟ;
	errcode_t err;
	ଅଚଳ ଅକ୍ଷର *cp;
	ଅକ୍ଷର *out;

	ଯଦି ((!ମୂଲ୍ୟ) ||
	    (size < ଆକାର(ext4_acl_header)) ||
	    (ext_acl->a_version != ext2fs_cpu_to_le32(EXT4_ACL_VERSION)))
		ଫେରନ୍ତୁ EINVAL;

	err = ext2fs_get_mem(size * 2, &out);
	ଯଦି (err)
		ଫେରନ୍ତୁ err;

	header = (posix_acl_xattr_header *) out;
	header->a_version = ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION);
	entry = (posix_acl_xattr_entry *) (out + ଆକାର(posix_acl_xattr_header));

	cp = (ଅଚଳ ଅକ୍ଷର *) ମୂଲ୍ୟ + ଆକାର(ext4_acl_header);
	size -= ଆକାର(ext4_acl_header);

	ଯେତେବେଳେ (size > 0) {
		ଅଚଳ ext4_acl_entry *disk_entry = (ଅଚଳ ext4_acl_entry *) cp;

		entry->e_tag = disk_entry->e_tag;
		entry->e_perm = disk_entry->e_perm;

		ବାଛ(ext2fs_le16_to_cpu(entry->e_tag)) {
			ସ୍ଥିତି ACL_USER_OBJ:
			ସ୍ଥିତି ACL_GROUP_OBJ:
			ସ୍ଥିତି ACL_MASK:
			ସ୍ଥିତି ACL_OTHER:
				entry->e_id = 0;
				cp += ଆକାର(ext4_acl_entry_short);
				size -= ଆକାର(ext4_acl_entry_short);
				ଭାଙ୍ଗ;
			ସ୍ଥିତି ACL_USER:
			ସ୍ଥିତି ACL_GROUP:
				entry->e_id = disk_entry->e_id;
				cp += ଆକାର(ext4_acl_entry);
				size -= ଆକାର(ext4_acl_entry);
				ଭାଙ୍ଗ;
			ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
				ext2fs_free_mem(&out);
				ଫେରନ୍ତୁ EINVAL;
		}
		entry++;
	}
	*out_buf = out;
	*size_out = ((ଅକ୍ଷର *) entry - out);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t
write_xattrs_to_buffer(ext2_filsys fs, ସଂରଚନା ext2_xattr *attrs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2,
		       ଶୂନ୍ୟ *entries_start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା storage_size,
		       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ_offset_correction, ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_hash)
{
	ସଂରଚନା ext2_xattr *x;
	ସଂରଚନା ext2_ext_attr_entry *e = entries_start;
	ଅକ୍ଷର *end = (ଅକ୍ଷର *) entries_start + storage_size;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ_size;
	errcode_t err;

	memset(entries_start, 0, storage_size);
	ପାଇଁ (x = attrs; x < attrs + ଗଣନା_2; x++) {
		ମୂଲ୍ୟ_size = ((x->ମୂଲ୍ୟ_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = strlen(x->short_name);
		e->e_name_index = x->name_index;

		e->e_ମୂଲ୍ୟ_size = x->ମୂଲ୍ୟ_len;
		e->e_ମୂଲ୍ୟ_inum = x->ea_ino;

		/* Store name */
		memcpy((ଅକ୍ଷର *)e + ଆକାର(*e), x->short_name, e->e_name_len);
		ଯଦି (x->ea_ino) {
			e->e_ମୂଲ୍ୟ_offs = 0;
		} ଅନ୍ୟଥା {
			end -= ମୂଲ୍ୟ_size;
			e->e_ମୂଲ୍ୟ_offs = end - (ଅକ୍ଷର *) entries_start +
						ମୂଲ୍ୟ_offset_correction;
			memcpy(end, x->ମୂଲ୍ୟ, e->e_ମୂଲ୍ୟ_size);
		}

		ଯଦି (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : end,
							  &e->e_hash);
			ଯଦି (err)
				ଫେରନ୍ତୁ err;
		} ଅନ୍ୟଥା
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_NEXT(e);
		*(__u32 *)e = 0;
	}
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_xattrs_write(ସଂରଚନା ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା inode_size = EXT2_INODE_SIZE(fs->super);
	ସଂରଚନା ext2_inode_large *inode;
	ଅକ୍ଷର *start, *block_buf = ଖାଲି;
	ସଂରଚନା ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	blk64_t blk;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା storage_size;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_size;
	ଯଦି (i < ଆକାର(*inode))
		i = ଆକାର(*inode);
	err = ext2fs_get_memzero(i, &inode);
	ଯଦି (err)
		ଫେରନ୍ତୁ err;

	err = ext2fs_read_inode_full(fs, handle->ino, EXT2_INODE(inode),
				     inode_size);
	ଯଦି (err)
		ଯାଅ out;

	/* If extra_isize isn't set, we need to set it now */
	ଯଦି (inode->i_extra_isize == 0 &&
	    inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		ଅକ୍ଷର *p = (ଅକ୍ଷର *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		ଯଦି (extra == 0)
			extra = ଆକାର(__u32);
		memset(p + EXT2_GOOD_OLD_INODE_SIZE, 0, extra);
		inode->i_extra_isize = extra;
	}
	ଯଦି (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		ଯାଅ out;
	}

	/* Does the inode have space for EA? */
	ଯଦି (inode->i_extra_isize < ଆକାର(inode->i_extra_isize) ||
	    inode_size <= EXT2_GOOD_OLD_INODE_SIZE + inode->i_extra_isize +
								ଆକାର(__u32))
		ଯାଅ write_ea_block;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	memcpy(((ଅକ୍ଷର *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, &ea_inode_magic, ଆକାର(__u32));
	storage_size = inode_size - EXT2_GOOD_OLD_INODE_SIZE -
				inode->i_extra_isize - ଆକାର(__u32);
	start = ((ଅକ୍ଷର *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
				inode->i_extra_isize + ଆକାର(__u32);

	err = write_xattrs_to_buffer(fs, handle->attrs, handle->ibody_ଗଣନା,
				     start, storage_size, 0, 0);
	ଯଦି (err)
		ଯାଅ out;
write_ea_block:
	/* Are we done? */
	ଯଦି (handle->ibody_ଗଣନା == handle->ଗଣନା_2 &&
	    !ext2fs_file_acl_block(fs, EXT2_INODE(inode)))
		ଯାଅ skip_ea_block;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &block_buf);
	ଯଦି (err)
		ଯାଅ out;

	storage_size = fs->blocksize - ଆକାର(ସଂରଚନା ext2_ext_attr_header);
	start = block_buf + ଆକାର(ସଂରଚନା ext2_ext_attr_header);

	err = write_xattrs_to_buffer(fs, handle->attrs + handle->ibody_ଗଣନା,
				     handle->ଗଣନା_2 - handle->ibody_ଗଣନା, start,
				     storage_size, start - block_buf, 1);
	ଯଦି (err)
		ଯାଅ out2;

	/* Write a header on the EA block */
	header = (ସଂରଚନା ext2_ext_attr_header *) block_buf;
	header->h_magic = EXT2_EXT_ATTR_MAGIC;
	header->h_refcount = 1;
	header->h_blocks = 1;

	/* Get a new block for writing */
	err = prep_ea_block_for_write(fs, handle->ino, inode);
	ଯଦି (err)
		ଯାଅ out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_block(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, handle->ino);
	ଯଦି (err)
		ଯାଅ out2;

skip_ea_block:
	blk = ext2fs_file_acl_block(fs, (ସଂରଚନା ext2_inode *)inode);
	ଯଦି (!block_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		ଯଦି (err)
			ଯାଅ out;
	}

	/* Write the inode */
	err = ext2fs_write_inode_full(fs, handle->ino, EXT2_INODE(inode),
				      inode_size);
	ଯଦି (err)
		ଯାଅ out2;

out2:
	ext2fs_free_mem(&block_buf);
out:
	ext2fs_free_mem(&inode);
	ଫେରନ୍ତୁ err;
}

ସ୍ଥିର errcode_t read_xattrs_from_buffer(ସଂରଚନା ext2_xattr_handle *handle,
					 ସଂରଚନା ext2_inode_large *inode,
					 ସଂରଚନା ext2_ext_attr_entry *entries,
					 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା storage_size,
					 ଅକ୍ଷର *ମୂଲ୍ୟ_start)
{
	ସଂରଚନା ext2_xattr *x;
	ସଂରଚନା ext2_ext_attr_entry *entry, *end;
	ଅଚଳ ଅକ୍ଷର *prefix;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା remain, prefix_len;
	errcode_t err;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟଗୁଡ଼ିକ_size = storage_size +
			((ଅକ୍ଷର *)entries - ମୂଲ୍ୟ_start);

	/* find the end */
	end = entries;
	remain = storage_size;
	ଯେତେବେଳେ (remain >= ଆକାର(ସଂରଚନା ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(end)) {

		/* header eats this space */
		remain -= ଆକାର(ସଂରଚନା ext2_ext_attr_entry);

		/* is attribute name valid? */
		ଯଦି (EXT2_EXT_ATTR_SIZE(end->e_name_len) > remain)
			ଫେରନ୍ତୁ EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(end->e_name_len);
		end = EXT2_EXT_ATTR_NEXT(end);
	}

	entry = entries;
	remain = storage_size;
	ଯେତେବେଳେ (remain >= ଆକାର(ସଂରଚନା ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(entry)) {

		/* Allocate space for more attrs? */
		ଯଦି (handle->ଗଣନା_2 == handle->capacity) {
			err = ext2fs_xattrs_expand(handle, 4);
			ଯଦି (err)
				ଫେରନ୍ତୁ err;
		}

		x = handle->attrs + handle->ଗଣନା_2;

		/* header eats this space */
		remain -= ଆକାର(ସଂରଚନା ext2_ext_attr_entry);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(entry->e_name_len);

		/* Extract name */
		prefix = find_ea_prefix(entry->e_name_index);
		prefix_len = (prefix ? strlen(prefix) : 0);
		err = ext2fs_get_memzero(entry->e_name_len + prefix_len + 1,
					 &x->name);
		ଯଦି (err)
			ଫେରନ୍ତୁ err;
		ଯଦି (prefix)
			memcpy(x->name, prefix, prefix_len);
		ଯଦି (entry->e_name_len)
			memcpy(x->name + prefix_len,
			       (ଅକ୍ଷର *)entry + ଆକାର(*entry),
			       entry->e_name_len);
		x->short_name = x->name + prefix_len;
		x->name_index = entry->e_name_index;

		/* Check & copy value */
		ଯଦି (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    entry->e_ମୂଲ୍ୟ_inum != 0)
			ଫେରନ୍ତୁ EXT2_ET_BAD_EA_BLOCK_ଅଙ୍କ;

		ଯଦି (entry->e_ମୂଲ୍ୟ_inum == 0) {
			ଯଦି (entry->e_ମୂଲ୍ୟ_size > remain)
				ଫେରନ୍ତୁ EXT2_ET_EA_BAD_ମୂଲ୍ୟ_SIZE;

			ଯଦି (entry->e_ମୂଲ୍ୟ_offs + entry->e_ମୂଲ୍ୟ_size > ମୂଲ୍ୟଗୁଡ଼ିକ_size)
				ଫେରନ୍ତୁ EXT2_ET_EA_BAD_ମୂଲ୍ୟ_OFFSET;

			ଯଦି (entry->e_ମୂଲ୍ୟ_size > 0 &&
			    ମୂଲ୍ୟ_start + entry->e_ମୂଲ୍ୟ_offs <
			    (ଅକ୍ଷର *)end + ଆକାର(__u32))
				ଫେରନ୍ତୁ EXT2_ET_EA_BAD_ମୂଲ୍ୟ_OFFSET;

			remain -= entry->e_ମୂଲ୍ୟ_size;

			err = ext2fs_get_mem(entry->e_ମୂଲ୍ୟ_size, &x->ମୂଲ୍ୟ);
			ଯଦି (err)
				ଫେରନ୍ତୁ err;
			memcpy(x->ମୂଲ୍ୟ, ମୂଲ୍ୟ_start + entry->e_ମୂଲ୍ୟ_offs,
			       entry->e_ମୂଲ୍ୟ_size);
		} ଅନ୍ୟଥା {
			ସଂରଚନା ext2_inode *ea_inode;
			ext2_file_t ea_file;

			ଯଦି (entry->e_ମୂଲ୍ୟ_offs != 0)
				ଫେରନ୍ତୁ EXT2_ET_EA_BAD_ମୂଲ୍ୟ_OFFSET;

			ଯଦି (entry->e_ମୂଲ୍ୟ_size > (64 * 1024))
				ଫେରନ୍ତୁ EXT2_ET_EA_BAD_ମୂଲ୍ୟ_SIZE;

			err = ext2fs_get_mem(entry->e_ମୂଲ୍ୟ_size, &x->ମୂଲ୍ୟ);
			ଯଦି (err)
				ଫେରନ୍ତୁ err;

			err = ext2fs_file_open(handle->fs, entry->e_ମୂଲ୍ୟ_inum,
					       0, &ea_file);
			ଯଦି (err)
				ଫେରନ୍ତୁ err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			ଯଦି ((ea_inode->i_flags & EXT4_INLINE_DATA_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_ଗଣନା == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			ଅନ୍ୟଥା ଯଦି ((__u64) ext2fs_file_get_size(ea_file) !=
				 entry->e_ମୂଲ୍ୟ_size)
				err = EXT2_ET_EA_BAD_ମୂଲ୍ୟ_SIZE;
			ଅନ୍ୟଥା
				err = ext2fs_file_read(ea_file, x->ମୂଲ୍ୟ,
						       entry->e_ମୂଲ୍ୟ_size, 0);
			ext2fs_file_close(ea_file);
			ଯଦି (err)
				ଫେରନ୍ତୁ err;
		}

		x->ea_ino = entry->e_ମୂଲ୍ୟ_inum;
		x->ମୂଲ୍ୟ_len = entry->e_ମୂଲ୍ୟ_size;

		/* e_hash may be 0 in older inode's ea */
		ଯଦି (entry->e_hash != 0) {
			__u32 hash, signed_hash;

			ଶୂନ୍ୟ *data = (entry->e_ମୂଲ୍ୟ_inum != 0) ?
					0 : ମୂଲ୍ୟ_start + entry->e_ମୂଲ୍ୟ_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, entry,
							  data, &hash,
							  &signed_hash);
			ଯଦି (err)
				ଫେରନ୍ତୁ err;
			ଯଦି ((entry->e_hash != hash) &&
			    (entry->e_hash != signed_hash)) {
				ସଂରଚନା ext2_inode child;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							entry->e_ମୂଲ୍ୟ_inum,
							&child);
				ଯଦି (err)
					ଫେରନ୍ତୁ err;
				ଯଦି (child.i_mtime != handle->ino ||
				    child.i_generation != inode->i_generation)
					ଫେରନ୍ତୁ EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->ଗଣନା_2++;
		entry = EXT2_EXT_ATTR_NEXT(entry);
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ xattrs_free_ଚାବିଗୁଡ଼ିକ(ସଂରଚନା ext2_xattr_handle *h)
{
	ସଂରଚନା ext2_xattr *a = h->attrs;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = 0; i < h->capacity; i++) {
		ଯଦି (a[i].name)
			ext2fs_free_mem(&a[i].name);
		ଯଦି (a[i].ମୂଲ୍ୟ)
			ext2fs_free_mem(&a[i].ମୂଲ୍ୟ);
	}
	h->ଗଣନା_2 = 0;
	h->ibody_ଗଣନା = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(ସଂରଚନା ext2_xattr_handle *handle,
				   ସଂରଚନା ext2_inode_large *inode)
{
	ସଂରଚନା ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା storage_size;
	ଅକ୍ଷର *start, *block_buf = ଖାଲି;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_ଚାବିଗୁଡ଼ିକ(handle);

	/* Does the inode have space for EA? */
	ଯଦି (inode->i_extra_isize < ଆକାର(inode->i_extra_isize) ||
	    EXT2_INODE_SIZE(handle->fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  ଆକାର(__u32))
		ଯାଅ read_ea_block;
	ଯଦି (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		ଯାଅ out;
	}

	/* Look for EA in the inode */
	memcpy(&ea_inode_magic, ((ଅକ୍ଷର *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, ଆକାର(__u32));
	ଯଦି (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_size = EXT2_INODE_SIZE(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_SIZE - inode->i_extra_isize -
			ଆକାର(__u32);
		start = ((ଅକ୍ଷର *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + ଆକାର(__u32);

		err = read_xattrs_from_buffer(handle, inode,
					(ସଂରଚନା ext2_ext_attr_entry *) start,
					storage_size, start);
		ଯଦି (err)
			ଯାଅ out;

		handle->ibody_ଗଣନା = handle->ଗଣନା_2;
	}

read_ea_block:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_block(handle->fs, EXT2_INODE(inode));
	ଯଦି (blk != 0) {
		ଯଦି ((blk < handle->fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_ଗଣନା(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_ଅଙ୍କ;
			ଯାଅ out;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &block_buf);
		ଯଦି (err)
			ଯାଅ out;

		err = ext2fs_read_ext_attr3(handle->fs, blk, block_buf,
					    handle->ino);
		ଯଦି (err)
			ଯାଅ out3;

		/* We only know how to deal with v2 EA blocks */
		header = (ସଂରଚନା ext2_ext_attr_header *) block_buf;
		ଯଦି (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			ଯାଅ out3;
		}

		/* Read EAs */
		storage_size = handle->fs->blocksize -
			ଆକାର(ସଂରଚନା ext2_ext_attr_header);
		start = block_buf + ଆକାର(ସଂରଚନା ext2_ext_attr_header);
		err = read_xattrs_from_buffer(handle, inode,
					(ସଂରଚନା ext2_ext_attr_entry *) start,
					storage_size, block_buf);
	}

out3:
	ଯଦି (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	ଫେରନ୍ତୁ err;
}

errcode_t ext2fs_xattrs_read(ସଂରଚନା ext2_xattr_handle *handle)
{
	ସଂରଚନା ext2_inode_large *inode;
	size_t inode_size = EXT2_INODE_SIZE(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	ଯଦି (inode_size < ଆକାର(*inode))
		inode_size = ଆକାର(*inode);
	err = ext2fs_get_memzero(inode_size, &inode);
	ଯଦି (err)
		ଫେରନ୍ତୁ err;

	err = ext2fs_read_inode_full(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_SIZE(handle->fs->super));
	ଯଦି (err)
		ଯାଅ out;

	err = ext2fs_xattrs_read_inode(handle, inode);

out:
	ext2fs_free_mem(&inode);

	ଫେରନ୍ତୁ err;
}

errcode_t ext2fs_xattrs_iterate(ସଂରଚନା ext2_xattr_handle *h,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*func)(ଅକ୍ଷର *name, ଅକ୍ଷର *ମୂଲ୍ୟ,
					    size_t ମୂଲ୍ୟ_len, ଶୂନ୍ୟ *data),
				ଶୂନ୍ୟ *data)
{
	ସଂରଚନା ext2_xattr *x;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirty = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	ପାଇଁ (x = h->attrs; x < h->attrs + h->ଗଣନା_2; x++) {
		ret = func(x->name, x->ମୂଲ୍ୟ, x->ମୂଲ୍ୟ_len, data);
		ଯଦି (ret & XATTR_CHANGED)
			dirty = 1;
		ଯଦି (ret & XATTR_ABORT)
			ଭାଙ୍ଗ;
	}

	ଯଦି (dirty)
		ଫେରନ୍ତୁ ext2fs_xattrs_write(h);
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_xattr_get(ସଂରଚନା ext2_xattr_handle *h, ଅଚଳ ଅକ୍ଷର *ଚାବି,
			   ଶୂନ୍ୟ **ମୂଲ୍ୟ, size_t *ମୂଲ୍ୟ_len)
{
	ସଂରଚନା ext2_xattr *x;
	ଅକ୍ଷର *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	ପାଇଁ (x = h->attrs; x < h->attrs + h->ଗଣନା_2; x++) {
		ଯଦି (strcmp(x->name, ଚାବି))
			ଜାରି;

		ଯଦି (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(ଚାବି, "system.posix_acl_default") == 0) ||
		     (strcmp(ଚାବି, "system.posix_acl_access") == 0))) {
			err = convert_disk_buffer_to_posix_acl(x->ମୂଲ୍ୟ, x->ମୂଲ୍ୟ_len,
							       ମୂଲ୍ୟ, ମୂଲ୍ୟ_len);
			ଫେରନ୍ତୁ err;
		} ଅନ୍ୟଥା {
			err = ext2fs_get_mem(x->ମୂଲ୍ୟ_len, &val);
			ଯଦି (err)
				ଫେରନ୍ତୁ err;
			memcpy(val, x->ମୂଲ୍ୟ, x->ମୂଲ୍ୟ_len);
			*ମୂଲ୍ୟ = val;
			*ମୂଲ୍ୟ_len = x->ମୂଲ୍ୟ_len;
			ଫେରନ୍ତୁ 0;
		}
	}

	ଫେରନ୍ତୁ EXT2_ET_EA_ଚାବି_NOT_FOUND;
}

errcode_t ext2fs_xattr_inode_max_size(ext2_filsys fs, ext2_ino_t ino,
				      size_t *size)
{
	ସଂରଚନା ext2_ext_attr_entry *entry;
	ସଂରଚନା ext2_inode_large *inode;
	__u32 ea_inode_magic;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା minoff;
	ଅକ୍ଷର *start;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_SIZE(fs->super);
	ଯଦି (i < ଆକାର(*inode))
		i = ଆକାର(*inode);
	err = ext2fs_get_memzero(i, &inode);
	ଯଦି (err)
		ଫେରନ୍ତୁ err;

	err = ext2fs_read_inode_full(fs, ino, (ସଂରଚନା ext2_inode *)inode,
				     EXT2_INODE_SIZE(fs->super));
	ଯଦି (err)
		ଯାଅ out;

	/* Does the inode have size for EA? */
	ଯଦି (EXT2_INODE_SIZE(fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  ଆକାର(__u32)) {
		err = EXT2_ET_INLINE_DATA_NO_SPACE;
		ଯାଅ out;
	}

	minoff = EXT2_INODE_SIZE(fs->super) - ଆକାର(*inode) - ଆକାର(__u32);
	memcpy(&ea_inode_magic, ((ଅକ୍ଷର *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, ଆକାର(__u32));
	ଯଦି (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		start= ((ଅକ୍ଷର *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + ଆକାର(__u32);
		entry = (ସଂରଚନା ext2_ext_attr_entry *) start;
		ଯେତେବେଳେ (!EXT2_EXT_IS_LAST_ENTRY(entry)) {
			ଯଦି (!entry->e_ମୂଲ୍ୟ_inum && entry->e_ମୂଲ୍ୟ_size) {
				unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offs = entry->e_ମୂଲ୍ୟ_offs;
				ଯଦି (offs < minoff)
					minoff = offs;
			}
			entry = EXT2_EXT_ATTR_NEXT(entry);
		}
		*size = minoff - ((ଅକ୍ଷର *)entry - (ଅକ୍ଷର *)start) - ଆକାର(__u32);
	} ଅନ୍ୟଥା {
		/* no xattr.  return a maximum size */
		*size = EXT2_EXT_ATTR_SIZE(minoff -
					   EXT2_EXT_ATTR_LEN(strlen("data")) -
					   EXT2_EXT_ATTR_ROUND - ଆକାର(__u32));
	}

out:
	ext2fs_free_mem(&inode);
	ଫେରନ୍ତୁ err;
}

ସ୍ଥିର errcode_t xattr_create_ea_inode(ext2_filsys fs, ଅଚଳ ଶୂନ୍ୟ *ମୂଲ୍ୟ,
				       size_t ମୂଲ୍ୟ_len, ext2_ino_t *ea_ino)
{
	ସଂରଚନା ext2_inode inode;
	ext2_ino_t ino;
	ext2_file_t file;
	__u32 hash;
	errcode_t ret;

	ret = ext2fs_new_inode(fs, 0, 0, 0, &ino);
	ଯଦି (ret)
		ଫେରନ୍ତୁ ret;

	memset(&inode, 0, ଆକାର(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	ଯଦି (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_size = 0;
	inode.i_mode = LINUX_S_IFREG | 0600;
	inode.i_links_ଗଣନା = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	ଯଦି (ret)
		ଫେରନ୍ତୁ ret;
	/*
	 * ref_count and hash utilize inode's i_*time fields.
	 * ext2fs_write_new_inode() call above initializes these fields with
	 * current time. That's why ref count and hash updates are done
	 * separately below.
	 */
	ext2fs_set_ea_inode_ref(&inode, 1);
	hash = ext2fs_crc32c_le(fs->csum_seed, ମୂଲ୍ୟ, ମୂଲ୍ୟ_len);
	ext2fs_set_ea_inode_hash(&inode, hash);

	ret = ext2fs_write_inode(fs, ino, &inode);
	ଯଦି (ret)
		ଫେରନ୍ତୁ ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	ଯଦି (ret)
		ଫେରନ୍ତୁ ret;
	ret = ext2fs_file_write(file, ମୂଲ୍ୟ, ମୂଲ୍ୟ_len, ଖାଲି);
	ext2fs_file_close(file);
	ଯଦି (ret)
		ଫେରନ୍ତୁ ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	ସଂରଚନା ext2_inode_large inode;
	__u64 ref_ଗଣନା;
	errcode_t ret;

	ret = ext2fs_read_inode_full(fs, ino, (ସଂରଚନା ext2_inode *)&inode,
				     ଆକାର(inode));
	ଯଦି (ret)
		ଯାଅ out;

	ref_ଗଣନା = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_ଗଣନା--;
	ext2fs_set_ea_inode_ref(EXT2_INODE(&inode), ref_ଗଣନା);

	ଯଦି (ref_ଗଣନା)
		ଯାଅ write_out;

	inode.i_links_ଗଣନା = 0;
	ext2fs_set_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	ଯଦି (ret)
		ଯାଅ write_out;

	ଯଦି (ext2fs_inode_has_valid_blocks2(fs, (ସଂରଚନା ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (ସଂରଚନା ext2_inode *)&inode, ଖାଲି,
				   0, ~0ULL);
		ଯଦି (ret)
			ଯାଅ out;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_out:
	ret = ext2fs_write_inode_full(fs, ino, (ସଂରଚନା ext2_inode *)&inode,
				      ଆକାର(inode));
out:
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର errcode_t xattr_update_entry(ext2_filsys fs, ସଂରଚନା ext2_xattr *x,
				    ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *short_name,
				    ପୂର୍ଣ୍ଣସଂଖ୍ୟା index, ଅଚଳ ଶୂନ୍ୟ *ମୂଲ୍ୟ,
				    size_t ମୂଲ୍ୟ_len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା in_inode)
{
	ext2_ino_t ea_ino = 0;
	ଶୂନ୍ୟ *new_ମୂଲ୍ୟ = ଖାଲି;
	ଅକ୍ଷର *new_name = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା name_len;
	errcode_t ret;

	ଯଦି (!x->name) {
		name_len = strlen(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		ଯଦି (ret)
			ଯାଅ fail;
		memcpy(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(ମୂଲ୍ୟ_len, &new_ମୂଲ୍ୟ);
	ଯଦି (ret)
		ଯାଅ fail;
	memcpy(new_ମୂଲ୍ୟ, ମୂଲ୍ୟ, ମୂଲ୍ୟ_len);

	ଯଦି (in_inode) {
		ret = xattr_create_ea_inode(fs, ମୂଲ୍ୟ, ମୂଲ୍ୟ_len, &ea_ino);
		ଯଦି (ret)
			ଯାଅ fail;
	}

	ଯଦି (x->ea_ino) {
		ret = xattr_inode_dec_ref(fs, x->ea_ino);
		ଯଦି (ret)
			ଯାଅ fail;
	}

	ଯଦି (!x->name) {
		x->name = new_name;
		x->short_name = new_name + (short_name  - name);
	}
	x->name_index = index;

	ଯଦି (x->ମୂଲ୍ୟ)
		ext2fs_free_mem(&x->ମୂଲ୍ୟ);
	x->ମୂଲ୍ୟ = new_ମୂଲ୍ୟ;
	x->ମୂଲ୍ୟ_len = ମୂଲ୍ୟ_len;
	x->ea_ino = ea_ino;
	ଫେରନ୍ତୁ 0;
fail:
	ଯଦି (new_name)
		ext2fs_free_mem(&new_name);
	ଯଦି (new_ମୂଲ୍ୟ)
		ext2fs_free_mem(&new_ମୂଲ୍ୟ);
	ଯଦି (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା xattr_find_position(ସଂରଚନା ext2_xattr *attrs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2,
			       ଅଚଳ ଅକ୍ଷର *shortname, ପୂର୍ଣ୍ଣସଂଖ୍ୟା name_idx)
{
	ସଂରଚନା ext2_xattr *x;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା shortname_len, x_shortname_len;

	shortname_len = strlen(shortname);

	ପାଇଁ (i = 0, x = attrs; i < ଗଣନା_2; i++, x++) {
		ଯଦି (name_idx < x->name_index)
			ଭାଙ୍ଗ;
		ଯଦି (name_idx > x->name_index)
			ଜାରି;

		x_shortname_len = strlen(x->short_name);
		ଯଦି (shortname_len < x_shortname_len)
			ଭାଙ୍ଗ;
		ଯଦି (shortname_len > x_shortname_len)
			ଜାରି;

		ଯଦି (memcmp(shortname, x->short_name, shortname_len) <= 0)
			ଭାଙ୍ଗ;
	}
	ଫେରନ୍ତୁ i;
}

ସ୍ଥିର errcode_t xattr_array_update(ସଂରଚନା ext2_xattr_handle *h,
				    ଅଚଳ ଅକ୍ଷର *name,
				    ଅଚଳ ଶୂନ୍ୟ *ମୂଲ୍ୟ, size_t ମୂଲ୍ୟ_len,
				    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ibody_free, ପୂର୍ଣ୍ଣସଂଖ୍ୟା block_free,
				    ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_idx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା in_inode)
{
	ସଂରଚନା ext2_xattr tmp;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା add_to_ibody;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା needed;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା name_len, name_idx = 0;
	ଅଚଳ ଅକ୍ଷର *shortname = name;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_idx;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	find_ea_index(name, &shortname, &name_idx);
	name_len = strlen(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	ଯଦି (!in_inode)
		needed += EXT2_EXT_ATTR_SIZE(ମୂଲ୍ୟ_len);

	ଯଦି (old_idx >= 0 && old_idx < h->ibody_ଗଣନା) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		ଯଦି (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_SIZE(
						h->attrs[old_idx].ମୂଲ୍ୟ_len);
	}

	ଯଦି (needed <= ibody_free) {
		ଯଦି (old_idx < 0) {
			new_idx = h->ibody_ଗଣନା;
			add_to_ibody = 1;
			ଯାଅ add_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, ମୂଲ୍ୟ,
					 ମୂଲ୍ୟ_len, in_inode);
		ଯଦି (ret)
			ଫେରନ୍ତୁ ret;
		ଯଦି (h->ibody_ଗଣନା <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_ଗଣନା + 1,
				h->attrs + h->ibody_ଗଣନା,
				(old_idx - h->ibody_ଗଣନା) * ଆକାର(*h->attrs));
			h->attrs[h->ibody_ଗଣନା] = tmp;
			h->ibody_ଗଣନା++;
		}
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (h->ibody_ଗଣନା <= old_idx) {
		block_free += EXT2_EXT_ATTR_LEN(name_len);
		ଯଦି (!h->attrs[old_idx].ea_ino)
			block_free +=
				EXT2_EXT_ATTR_SIZE(h->attrs[old_idx].ମୂଲ୍ୟ_len);
	}

	ଯଦି (needed > block_free)
		ଫେରନ୍ତୁ EXT2_ET_EA_NO_SPACE;

	ଯଦି (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, ମୂଲ୍ୟ,
					 ମୂଲ୍ୟ_len, in_inode);
		ଯଦି (ret)
			ଫେରନ୍ତୁ ret;
		ଯଦି (old_idx < h->ibody_ଗଣନା) {
			/*
			 * Move entry from ibody to the block. Note that
			 * entries in the block are sorted.
			 */
			new_idx = xattr_find_position(h->attrs + h->ibody_ଗଣନା,
						      h->ଗଣନା_2 - h->ibody_ଗଣନା,
						      shortname, name_idx);
			new_idx += h->ibody_ଗଣନା - 1;
			tmp = h->attrs[old_idx];
			memmove(h->attrs + old_idx, h->attrs + old_idx + 1,
				(new_idx - old_idx) * ଆକାର(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_ଗଣନା--;
		}
		ଫେରନ୍ତୁ 0;
	}

	new_idx = xattr_find_position(h->attrs + h->ibody_ଗଣନା,
				      h->ଗଣନା_2 - h->ibody_ଗଣନା,
				      shortname, name_idx);
	new_idx += h->ibody_ଗଣନା;
	add_to_ibody = 0;

add_new:
	ଯଦି (h->ଗଣନା_2 == h->capacity) {
		ret = ext2fs_xattrs_expand(h, 4);
		ଯଦି (ret)
			ଫେରନ୍ତୁ ret;
	}

	ret = xattr_update_entry(h->fs, &h->attrs[h->ଗଣନା_2], name, shortname,
				 name_idx, ମୂଲ୍ୟ, ମୂଲ୍ୟ_len, in_inode);
	ଯଦି (ret)
		ଫେରନ୍ତୁ ret;

	tmp = h->attrs[h->ଗଣନା_2];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->ଗଣନା_2 - new_idx)*ଆକାର(*h->attrs));
	h->attrs[new_idx] = tmp;
	ଯଦି (add_to_ibody)
		h->ibody_ଗଣନା++;
	h->ଗଣନା_2++;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା space_used(ସଂରଚନା ext2_xattr *attrs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା total = 0;
	ସଂରଚନା ext2_xattr *x;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, len;

	ପାଇଁ (i = 0, x = attrs; i < ଗଣନା_2; i++, x++) {
		len = strlen(x->short_name);
		total += EXT2_EXT_ATTR_LEN(len);
		ଯଦି (!x->ea_ino)
			total += EXT2_EXT_ATTR_SIZE(x->ମୂଲ୍ୟ_len);
	}
	ଫେରନ୍ତୁ total;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#define EXT4_XATTR_MIN_LARGE_EA_SIZE(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - ଆକାର(ସଂରଚନା ext2_ext_attr_header) - 4)

errcode_t ext2fs_xattr_set(ସଂରଚନା ext2_xattr_handle *h,
			   ଅଚଳ ଅକ୍ଷର *name,
			   ଅଚଳ ଶୂନ୍ୟ *ମୂଲ୍ୟ,
			   size_t ମୂଲ୍ୟ_len)
{
	ext2_filsys fs = h->fs;
	ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା inode_size = EXT2_INODE_SIZE(fs->super);
	ସଂରଚନା ext2_inode_large *inode = ଖାଲି;
	ସଂରଚନା ext2_xattr *x;
	ଅକ୍ଷର *new_ମୂଲ୍ୟ;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ibody_free, block_free;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା in_inode = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_idx = -1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(ମୂଲ୍ୟ_len, &new_ମୂଲ୍ୟ);
	ଯଦି (ret)
		ଫେରନ୍ତୁ ret;
	ଯଦି (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((strcmp(name, "system.posix_acl_default") == 0) ||
	     (strcmp(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_to_disk_buffer(ମୂଲ୍ୟ, ମୂଲ୍ୟ_len,
						       new_ମୂଲ୍ୟ, &ମୂଲ୍ୟ_len);
		ଯଦି (ret)
			ଯାଅ out;
	} ଅନ୍ୟଥା ଯଦି (ମୂଲ୍ୟ_len)
		memcpy(new_ମୂଲ୍ୟ, ମୂଲ୍ୟ, ମୂଲ୍ୟ_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	ପାଇଁ (x = h->attrs; x < h->attrs + h->ଗଣନା_2; x++) {
		ଯଦି (!strcmp(x->name, name)) {
			ଯଦି (!x->ea_ino && x->ମୂଲ୍ୟ_len == ମୂଲ୍ୟ_len &&
			    (!ମୂଲ୍ୟ_len ||
			     !memcmp(x->ମୂଲ୍ୟ, new_ମୂଲ୍ୟ, ମୂଲ୍ୟ_len))) {
				ret = 0;
				ଯାଅ out;
			}
			old_idx = x - h->attrs;
			ଭାଙ୍ଗ;
		}
	}

	ret = ext2fs_get_memzero(inode_size, &inode);
	ଯଦି (ret)
		ଯାଅ out;
	ret = ext2fs_read_inode_full(fs, h->ino,
				     (ସଂରଚନା ext2_inode *)inode,
				     inode_size);
	ଯଦି (ret)
		ଯାଅ out;
	ଯଦି (inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		extra_isize = inode->i_extra_isize;
		ଯଦି (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			ଯଦି (extra_isize == 0)
				extra_isize = ଆକାର(__u32);
		}
		ibody_free = inode_size - EXT2_GOOD_OLD_INODE_SIZE;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= ଆକାର(__u32) * 2;
		ibody_free -= space_used(h->attrs, h->ibody_ଗଣନା);
	} ଅନ୍ୟଥା
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	ଯଦି (strcmp(name, "system.data") == 0) {
		ଯଦି (h->ibody_ଗଣନା <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			ଯାଅ out;
		}
		ret = xattr_array_update(h, name, new_ମୂଲ୍ୟ, ମୂଲ୍ୟ_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		ଯଦି (ret)
			ଯାଅ out;
		ଯାଅ write_out;
	}

	block_free = fs->blocksize;
	block_free -= ଆକାର(ସଂରଚନା ext2_ext_attr_header);
	/* Final null entry. */
	block_free -= ଆକାର(__u32);
	block_free -= space_used(h->attrs + h->ibody_ଗଣନା,
				 h->ଗଣନା_2 - h->ibody_ଗଣନା);

	ଯଦି (ext2fs_has_feature_ea_inode(fs->super) &&
	    ମୂଲ୍ୟ_len > EXT4_XATTR_MIN_LARGE_EA_SIZE(fs->blocksize))
		in_inode = 1;

	ret = xattr_array_update(h, name, new_ମୂଲ୍ୟ, ମୂଲ୍ୟ_len, ibody_free,
				 block_free, old_idx, in_inode);
	ଯଦି (ret == EXT2_ET_EA_NO_SPACE && !in_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_ମୂଲ୍ୟ, ମୂଲ୍ୟ_len,
			ibody_free, block_free, old_idx, 1 /* in_inode */);
	ଯଦି (ret)
		ଯାଅ out;

write_out:
	ret = ext2fs_xattrs_write(h);
out:
	ଯଦି (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_ମୂଲ୍ୟ);
	ଫେରନ୍ତୁ ret;
}

errcode_t ext2fs_xattr_remove(ସଂରଚନା ext2_xattr_handle *handle,
			      ଅଚଳ ଅକ୍ଷର *ଚାବି)
{
	ସଂରଚନା ext2_xattr *x;
	ସଂରଚନା ext2_xattr *end = handle->attrs + handle->ଗଣନା_2;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	ପାଇଁ (x = handle->attrs; x < end; x++) {
		ଯଦି (strcmp(x->name, ଚାବି) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->ମୂଲ୍ୟ);
			ଯଦି (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (end - x - 1)*ଆକାର(*x));
			memset(end - 1, 0, ଆକାର(*end));
			ଯଦି (x < handle->attrs + handle->ibody_ଗଣନା)
				handle->ibody_ଗଣନା--;
			handle->ଗଣନା_2--;
			ଫେରନ୍ତୁ ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_xattr_remove_all(ସଂରଚନା ext2_xattr_handle *handle)
{
	ସଂରଚନା ext2_xattr *x;
	ସଂରଚନା ext2_xattr *end = handle->attrs + handle->ଗଣନା_2;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	ପାଇଁ (x = handle->attrs; x < end; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->ମୂଲ୍ୟ);
		ଯଦି (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_ଗଣନା = 0;
	handle->ଗଣନା_2 = 0;
	ଫେରନ୍ତୁ ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     ସଂରଚନା ext2_xattr_handle **handle)
{
	ସଂରଚନା ext2_xattr_handle *h;
	errcode_t err;

	ଯଦି (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_data(fs->super))
		ଫେରନ୍ତୁ EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(ଆକାର(*h), &h);
	ଯଦି (err)
		ଫେରନ୍ତୁ err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->capacity = 4;
	err = ext2fs_get_arrayzero(h->capacity, ଆକାର(ସଂରଚନା ext2_xattr),
				   &h->attrs);
	ଯଦି (err) {
		ext2fs_free_mem(&h);
		ଫେରନ୍ତୁ err;
	}
	h->ଗଣନା_2 = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_xattrs_close(ସଂରଚନା ext2_xattr_handle **handle)
{
	ସଂରଚନା ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_ଚାବିଗୁଡ଼ିକ(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_xattrs_ଗଣନା(ସଂରଚନା ext2_xattr_handle *handle, size_t *ଗଣନା_2)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*ଗଣନା_2 = handle->ଗଣନା_2;
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_xattrs_flags(ସଂରଚନା ext2_xattr_handle *handle,
			      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *new_flags, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	ଯଦି (old_flags)
		*old_flags = handle->flags;
	ଯଦି (new_flags)
		handle->flags = *new_flags;
	ଫେରନ୍ତୁ 0;
}
