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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <time.h>

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2_ext_attr.h"
#অন্তর্ভাৱ "ext4_acl.h"

#অন্তর্ভাৱ "ext2fsP.h"

স্থির errcode_t read_ea_inode_hash(ext2_filsys fs, ext2_ino_t ino, __u32 *hash)
{
	সংরচনা ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	যদি (retval)
		ঘূৰাই retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	ঘূৰাই 0;
}

#define NAME_HASH_SHIFT 5
#define মান_HASH_SHIFT 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_entry(সংরচনা ext2_ext_attr_entry *entry, শূন্য *data)
{
	__u32 hash = 0;
	unsigned ৱর্ণ *name = (((unsigned ৱর্ণ *) entry) +
			       পরিমাণ(সংরচনা ext2_ext_attr_entry));
	পূর্ণাঙ্ক n;

	বাবে (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*পরিমাণ(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	যদি (entry->e_মান_inum == 0 && entry->e_মান_size != 0) {
		__u32 *মান = (__u32 *)data;
		বাবে (n = (entry->e_মান_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << মান_HASH_SHIFT) ^
			       (hash >> (8*পরিমাণ(hash) - মান_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*মান++);
		}
	}

	ঘূৰাই hash;
}

__u32 ext2fs_ext_attr_hash_entry_signed(সংরচনা ext2_ext_attr_entry *entry,
					শূন্য *data)
{
	__u32 hash = 0;
	signed ৱর্ণ *name = (((signed ৱর্ণ *) entry) +
			     পরিমাণ(সংরচনা ext2_ext_attr_entry));
	পূর্ণাঙ্ক n;

	বাবে (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*পরিমাণ(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	যদি (entry->e_মান_inum == 0 && entry->e_মান_size != 0) {
		__u32 *মান = (__u32 *)data;
		বাবে (n = (entry->e_মান_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << মান_HASH_SHIFT) ^
			       (hash >> (8*পরিমাণ(hash) - মান_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*মান++);
		}
	}

	ঘূৰাই hash;
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
				      সংরচনা ext2_ext_attr_entry *entry,
				      শূন্য *data, __u32 *hash,
				      __u32 *signed_hash)
{
	*hash = ext2fs_ext_attr_hash_entry(entry, data);
	যদি (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_entry_signed(entry, data);

	যদি (entry->e_মান_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, entry->e_মান_inum,
					    &ea_inode_hash);
		যদি (retval)
			ঘূৰাই retval;

		*hash = (*hash << মান_HASH_SHIFT) ^
			(*hash >> (8*পরিমাণ(*hash) - মান_HASH_SHIFT)) ^
			ea_inode_hash;
		যদি (signed_hash)
			*signed_hash = (*signed_hash << মান_HASH_SHIFT) ^
				(*signed_hash >> (8*পরিমাণ(*hash) -
						  মান_HASH_SHIFT)) ^
				ea_inode_hash;
	}
	ঘূৰাই 0;
}

/*
 * ext2fs_ext_attr_hash_entry2()
 *
 * Compute the hash of an extended attribute.
 * This version of the function supports hashing entries that reference
 * external inodes (ea_inode feature).
 */
errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
				      সংরচনা ext2_ext_attr_entry *entry,
				      শূন্য *data, __u32 *hash)
{
	ঘূৰাই ext2fs_ext_attr_hash_entry3(fs, entry, data, hash, রিক্ত);
}

#undef NAME_HASH_SHIFT
#undef মান_HASH_SHIFT

#define BLOCK_HASH_SHIFT 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
শূন্য ext2fs_ext_attr_block_rehash(সংরচনা ext2_ext_attr_header *header,
				  সংরচনা ext2_ext_attr_entry *end)
{
	সংরচনা ext2_ext_attr_entry *here;
	__u32 hash = 0;

	here = (সংরচনা ext2_ext_attr_entry *)(header+1);
	যেতিয়ালৈ (here < end && !EXT2_EXT_IS_LAST_ENTRY(here)) {
		যদি (!here->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			ভঙ্গ;
		}
		hash = (hash << BLOCK_HASH_SHIFT) ^
		       (hash >> (8*পরিমাণ(hash) - BLOCK_HASH_SHIFT)) ^
		       here->e_hash;
		here = EXT2_EXT_ATTR_NEXT(here);
	}
	header->h_hash = hash;
}

#undef BLOCK_HASH_SHIFT

__u32 ext2fs_get_ea_inode_hash(সংরচনা ext2_inode *inode)
{
	ঘূৰাই inode->i_atime;
}

শূন্য ext2fs_set_ea_inode_hash(সংরচনা ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(সংরচনা ext2_inode *inode)
{
	ঘূৰাই ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_version;
}

শূন্য ext2fs_set_ea_inode_ref(সংরচনা ext2_inode *inode, __u64 ref_গুনতি)
{
	inode->i_ctime = (__u32)(ref_গুনতি >> 32);
	inode->osd1.linux1.l_i_version = (__u32)ref_গুনতি;
}

স্থির errcode_t check_ext_attr_header(সংরচনা ext2_ext_attr_header *header)
{
	যদি ((header->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     header->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    header->h_blocks != 1)
		ঘূৰাই EXT2_ET_BAD_EA_HEADER;

	ঘূৰাই 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block, শূন্য *buf,
				ext2_ino_t inum)
{
	পূর্ণাঙ্ক		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	যদি (retval)
		ঘূৰাই retval;

	যদি (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_block_csum_verify(fs, inum, block, buf))
		csum_failed = 1;

#ifdef শব্দগুলি_BIGENDIAN
	ext2fs_swap_ext_attr(buf, buf, fs->blocksize, 1);
#endif

	retval = check_ext_attr_header(buf);
	যদি (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	ঘূৰাই retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block, শূন্য *buf)
{
	ঘূৰাই ext2fs_read_ext_attr3(fs, block, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, শূন্য *buf)
{
	ঘূৰাই ext2fs_read_ext_attr2(fs, block, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block, শূন্য *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	ৱর্ণ		*write_buf;

#ifdef শব্দগুলি_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	যদি (retval)
		ঘূৰাই retval;
	ext2fs_swap_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#অন্যথা
	write_buf = (ৱর্ণ *) inbuf;
#endif

	retval = ext2fs_ext_attr_block_csum_set(fs, inum, block,
			(সংরচনা ext2_ext_attr_header *)write_buf);
	যদি (retval)
		ঘূৰাই retval;

	retval = io_channel_write_blk64(fs->io, block, 1, write_buf);
#ifdef শব্দগুলি_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#endif
	যদি (!retval)
		ext2fs_mark_changed(fs);
	ঘূৰাই retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block, শূন্য *inbuf)
{
	ঘূৰাই ext2fs_write_ext_attr3(fs, block, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block, শূন্য *inbuf)
{
	ঘূৰাই ext2fs_write_ext_attr2(fs, block, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    ৱর্ণ *block_buf, পূর্ণাঙ্ক adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	সংরচনা ext2_ext_attr_header *header;
	ৱর্ণ	*buf = 0;

	যদি ((blk >= ext2fs_blocks_গুনতি(fs->super)) ||
	    (blk < fs->super->s_first_data_block))
		ঘূৰাই EXT2_ET_BAD_EA_BLOCK_অঙ্ক;

	যদি (!block_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		যদি (retval)
			ঘূৰাই retval;
		block_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, block_buf, inum);
	যদি (retval)
		গচ্ছ errout;

	header = (সংরচনা ext2_ext_attr_header *) block_buf;
	header->h_refcount += adjust;
	যদি (newcount)
		*newcount = header->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, block_buf, inum);
	যদি (retval)
		গচ্ছ errout;

errout:
	যদি (buf)
		ext2fs_free_mem(&buf);
	ঘূৰাই retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    ৱর্ণ *block_buf, পূর্ণাঙ্ক adjust,
				    __u32 *newcount)
{
	ঘূৰাই ext2fs_adjust_ea_refcount3(fs, blk, block_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					ৱর্ণ *block_buf, পূর্ণাঙ্ক adjust,
					__u32 *newcount)
{
	ঘূৰাই ext2fs_adjust_ea_refcount2(fs, blk, block_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
সংরচনা ext2_xattr {
	পূর্ণাঙ্ক name_index;
	ৱর্ণ *name;
	ৱর্ণ *short_name;
	শূন্য *মান;
	unsigned পূর্ণাঙ্ক মান_len;
	ext2_ino_t ea_ino;
};

সংরচনা ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	সংরচনা ext2_xattr *attrs;
	পূর্ণাঙ্ক capacity;
	পূর্ণাঙ্ক গুনতি;
	পূর্ণাঙ্ক ibody_গুনতি;
	ext2_ino_t ino;
	unsigned পূর্ণাঙ্ক flags;
};

স্থির errcode_t ext2fs_xattrs_expand(সংরচনা ext2_xattr_handle *h,
				      unsigned পূর্ণাঙ্ক expandby)
{
	সংরচনা ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->capacity + expandby,
				   পরিমাণ(সংরচনা ext2_xattr), &new_attrs);
	যদি (err)
		ঘূৰাই err;

	memcpy(new_attrs, h->attrs, h->capacity * পরিমাণ(সংরচনা ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->capacity += expandby;
	h->attrs = new_attrs;

	ঘূৰাই 0;
}

সংরচনা ea_name_index {
	পূর্ণাঙ্ক index;
	ধ্রুৱ ৱর্ণ *name;
};

/* Keep these names sorted in order of decreasing specificity. */
স্থির সংরচনা ea_name_index ea_names[] = {
	{10, "gnu."},
	{3, "system.posix_acl_default"},
	{2, "system.posix_acl_access"},
	{8, "system.richacl"},
	{6, "security."},
	{4, "trusted."},
	{7, "system."},
	{1, "user."},
	{0, রিক্ত},
};

স্থির ধ্রুৱ ৱর্ণ *find_ea_prefix(পূর্ণাঙ্ক index)
{
	সংরচনা ea_name_index *e;

	বাবে (e = ea_names; e->name; e++)
		যদি (e->index == index)
			ঘূৰাই e->name;

	ঘূৰাই রিক্ত;
}

স্থির পূর্ণাঙ্ক find_ea_index(ধ্রুৱ ৱর্ণ *fullname, ধ্রুৱ ৱর্ণ **name, পূর্ণাঙ্ক *index)
{
	সংরচনা ea_name_index *e;

	বাবে (e = ea_names; e->name; e++) {
		যদি (strncmp(fullname, e->name, strlen(e->name)) == 0) {
			*name = fullname + strlen(e->name);
			*index = e->index;
			ঘূৰাই 1;
		}
	}
	ঘূৰাই 0;
}

errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       সংরচনা ext2_inode_large *inode)
{
	সংরচনা ext2_ext_attr_header *header;
	শূন্য *block_buf = রিক্ত;
	blk64_t blk;
	errcode_t err;
	সংরচনা ext2_inode_large i;

	/* Read inode? */
	যদি (inode == রিক্ত) {
		err = ext2fs_read_inode_full(fs, ino, (সংরচনা ext2_inode *)&i,
					     পরিমাণ(সংরচনা ext2_inode_large));
		যদি (err)
			ঘূৰাই err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (সংরচনা ext2_inode *)inode);
	যদি (blk == 0)
		ঘূৰাই 0;

	/* Find block, zero it, write back */
	যদি ((blk < fs->super->s_first_data_block) ||
	    (blk >= ext2fs_blocks_গুনতি(fs->super))) {
		err = EXT2_ET_BAD_EA_BLOCK_অঙ্ক;
		গচ্ছ out;
	}

	err = ext2fs_get_mem(fs->blocksize, &block_buf);
	যদি (err)
		গচ্ছ out;

	err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
	যদি (err)
		গচ্ছ out2;

	/* We only know how to deal with v2 EA blocks */
	header = (সংরচনা ext2_ext_attr_header *) block_buf;
	যদি (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_HEADER;
		গচ্ছ out2;
	}

	header->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
	যদি (err)
		গচ্ছ out2;

	/* Erase link to block */
	ext2fs_file_acl_block_set(fs, (সংরচনা ext2_inode *)inode, 0);
	যদি (header->h_refcount == 0)
		ext2fs_block_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_blocks(fs, (সংরচনা ext2_inode *)inode, 1);
	যদি (err)
		গচ্ছ out2;

	/* Write inode? */
	যদি (inode == &i) {
		err = ext2fs_write_inode_full(fs, ino, (সংরচনা ext2_inode *)&i,
					      পরিমাণ(সংরচনা ext2_inode_large));
		যদি (err)
			গচ্ছ out2;
	}

out2:
	ext2fs_free_mem(&block_buf);
out:
	ঘূৰাই err;
}

স্থির errcode_t prep_ea_block_for_write(ext2_filsys fs, ext2_ino_t ino,
					 সংরচনা ext2_inode_large *inode)
{
	সংরচনা ext2_ext_attr_header *header;
	শূন্য *block_buf = রিক্ত;
	blk64_t blk, goal;
	errcode_t err;

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (সংরচনা ext2_inode *)inode);
	যদি (blk != 0) {
		যদি ((blk < fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_গুনতি(fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_অঙ্ক;
			গচ্ছ out;
		}

		err = ext2fs_get_mem(fs->blocksize, &block_buf);
		যদি (err)
			গচ্ছ out;

		err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
		যদি (err)
			গচ্ছ out2;

		/* We only know how to deal with v2 EA blocks */
		header = (সংরচনা ext2_ext_attr_header *) block_buf;
		যদি (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			গচ্ছ out2;
		}

		/* Single-user block.  We're done here. */
		যদি (header->h_refcount == 1)
			গচ্ছ out2;

		/* We need to CoW the block. */
		header->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
		যদি (err)
			গচ্ছ out2;
	} অন্যথা {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_add_blocks(fs, (সংরচনা ext2_inode *)inode,
					     1);
		যদি (err)
			গচ্ছ out;
	}

	/* Allocate a block */
	goal = ext2fs_find_inode_goal(fs, ino, (সংরচনা ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, রিক্ত, &blk);
	যদি (err)
		গচ্ছ out2;
	ext2fs_file_acl_block_set(fs, (সংরচনা ext2_inode *)inode, blk);
out2:
	যদি (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	ঘূৰাই err;
}


স্থির অন্তঃস্থ পূর্ণাঙ্ক
posix_acl_xattr_গুনতি(size_t size)
{
        যদি (size < পরিমাণ(posix_acl_xattr_header))
                ঘূৰাই -1;
        size -= পরিমাণ(posix_acl_xattr_header);
        যদি (size % পরিমাণ(posix_acl_xattr_entry))
                ঘূৰাই -1;
        ঘূৰাই size / পরিমাণ(posix_acl_xattr_entry);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
স্থির errcode_t convert_posix_acl_to_disk_buffer(ধ্রুৱ শূন্য *মান, size_t size,
						  শূন্য *out_buf, size_t *size_out)
{
	ধ্রুৱ posix_acl_xattr_header *header =
		(ধ্রুৱ posix_acl_xattr_header*) মান;
	ধ্রুৱ posix_acl_xattr_entry *end, *entry =
		(ধ্রুৱ posix_acl_xattr_entry *)(header+1);
	ext4_acl_header *ext_acl;
	size_t s;
	ৱর্ণ *e;

	পূর্ণাঙ্ক গুনতি;

	যদি (!মান)
		ঘূৰাই EINVAL;
	যদি (size < পরিমাণ(posix_acl_xattr_header))
		ঘূৰাই ENOMEM;
	যদি (header->a_version != ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION))
		ঘূৰাই EINVAL;

	গুনতি = posix_acl_xattr_গুনতি(size);
	ext_acl = out_buf;
	ext_acl->a_version = ext2fs_cpu_to_le32(EXT4_ACL_VERSION);

	যদি (গুনতি <= 0)
		ঘূৰাই EINVAL;

	e = (ৱর্ণ *) out_buf + পরিমাণ(ext4_acl_header);
	s = পরিমাণ(ext4_acl_header);
	বাবে (end = entry + গুনতি; entry != end;entry++) {
		ext4_acl_entry *disk_entry = (ext4_acl_entry*) e;
		disk_entry->e_tag = entry->e_tag;
		disk_entry->e_perm = entry->e_perm;

		ৱিকল্প(ext2fs_le16_to_cpu(entry->e_tag)) {
			পক্ষ ACL_USER_OBJ:
			পক্ষ ACL_GROUP_OBJ:
			পক্ষ ACL_MASK:
			পক্ষ ACL_OTHER:
				e += পরিমাণ(ext4_acl_entry_short);
				s += পরিমাণ(ext4_acl_entry_short);
				ভঙ্গ;
			পক্ষ ACL_USER:
			পক্ষ ACL_GROUP:
				disk_entry->e_id = entry->e_id;
				e += পরিমাণ(ext4_acl_entry);
				s += পরিমাণ(ext4_acl_entry);
				ভঙ্গ;
			স্ৱভাৱ:
				ঘূৰাই EINVAL;
		}
	}
	*size_out = s;
	ঘূৰাই 0;
}

স্থির errcode_t convert_disk_buffer_to_posix_acl(ধ্রুৱ শূন্য *মান, size_t size,
						  শূন্য **out_buf, size_t *size_out)
{
	posix_acl_xattr_header *header;
	posix_acl_xattr_entry *entry;
	ধ্রুৱ ext4_acl_header *ext_acl = (ধ্রুৱ ext4_acl_header *) মান;
	errcode_t err;
	ধ্রুৱ ৱর্ণ *cp;
	ৱর্ণ *out;

	যদি ((!মান) ||
	    (size < পরিমাণ(ext4_acl_header)) ||
	    (ext_acl->a_version != ext2fs_cpu_to_le32(EXT4_ACL_VERSION)))
		ঘূৰাই EINVAL;

	err = ext2fs_get_mem(size * 2, &out);
	যদি (err)
		ঘূৰাই err;

	header = (posix_acl_xattr_header *) out;
	header->a_version = ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION);
	entry = (posix_acl_xattr_entry *) (out + পরিমাণ(posix_acl_xattr_header));

	cp = (ধ্রুৱ ৱর্ণ *) মান + পরিমাণ(ext4_acl_header);
	size -= পরিমাণ(ext4_acl_header);

	যেতিয়ালৈ (size > 0) {
		ধ্রুৱ ext4_acl_entry *disk_entry = (ধ্রুৱ ext4_acl_entry *) cp;

		entry->e_tag = disk_entry->e_tag;
		entry->e_perm = disk_entry->e_perm;

		ৱিকল্প(ext2fs_le16_to_cpu(entry->e_tag)) {
			পক্ষ ACL_USER_OBJ:
			পক্ষ ACL_GROUP_OBJ:
			পক্ষ ACL_MASK:
			পক্ষ ACL_OTHER:
				entry->e_id = 0;
				cp += পরিমাণ(ext4_acl_entry_short);
				size -= পরিমাণ(ext4_acl_entry_short);
				ভঙ্গ;
			পক্ষ ACL_USER:
			পক্ষ ACL_GROUP:
				entry->e_id = disk_entry->e_id;
				cp += পরিমাণ(ext4_acl_entry);
				size -= পরিমাণ(ext4_acl_entry);
				ভঙ্গ;
			স্ৱভাৱ:
				ext2fs_free_mem(&out);
				ঘূৰাই EINVAL;
		}
		entry++;
	}
	*out_buf = out;
	*size_out = ((ৱর্ণ *) entry - out);
	ঘূৰাই 0;
}

স্থির errcode_t
write_xattrs_to_buffer(ext2_filsys fs, সংরচনা ext2_xattr *attrs, পূর্ণাঙ্ক গুনতি,
		       শূন্য *entries_start, unsigned পূর্ণাঙ্ক storage_size,
		       unsigned পূর্ণাঙ্ক মান_offset_correction, পূর্ণাঙ্ক write_hash)
{
	সংরচনা ext2_xattr *x;
	সংরচনা ext2_ext_attr_entry *e = entries_start;
	ৱর্ণ *end = (ৱর্ণ *) entries_start + storage_size;
	unsigned পূর্ণাঙ্ক মান_size;
	errcode_t err;

	memset(entries_start, 0, storage_size);
	বাবে (x = attrs; x < attrs + গুনতি; x++) {
		মান_size = ((x->মান_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = strlen(x->short_name);
		e->e_name_index = x->name_index;

		e->e_মান_size = x->মান_len;
		e->e_মান_inum = x->ea_ino;

		/* Store name */
		memcpy((ৱর্ণ *)e + পরিমাণ(*e), x->short_name, e->e_name_len);
		যদি (x->ea_ino) {
			e->e_মান_offs = 0;
		} অন্যথা {
			end -= মান_size;
			e->e_মান_offs = end - (ৱর্ণ *) entries_start +
						মান_offset_correction;
			memcpy(end, x->মান, e->e_মান_size);
		}

		যদি (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : end,
							  &e->e_hash);
			যদি (err)
				ঘূৰাই err;
		} অন্যথা
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_NEXT(e);
		*(__u32 *)e = 0;
	}
	ঘূৰাই 0;
}

errcode_t ext2fs_xattrs_write(সংরচনা ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	ধ্রুৱ unsigned পূর্ণাঙ্ক inode_size = EXT2_INODE_SIZE(fs->super);
	সংরচনা ext2_inode_large *inode;
	ৱর্ণ *start, *block_buf = রিক্ত;
	সংরচনা ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	blk64_t blk;
	unsigned পূর্ণাঙ্ক storage_size;
	unsigned পূর্ণাঙ্ক i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_size;
	যদি (i < পরিমাণ(*inode))
		i = পরিমাণ(*inode);
	err = ext2fs_get_memzero(i, &inode);
	যদি (err)
		ঘূৰাই err;

	err = ext2fs_read_inode_full(fs, handle->ino, EXT2_INODE(inode),
				     inode_size);
	যদি (err)
		গচ্ছ out;

	/* If extra_isize isn't set, we need to set it now */
	যদি (inode->i_extra_isize == 0 &&
	    inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		ৱর্ণ *p = (ৱর্ণ *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		যদি (extra == 0)
			extra = পরিমাণ(__u32);
		memset(p + EXT2_GOOD_OLD_INODE_SIZE, 0, extra);
		inode->i_extra_isize = extra;
	}
	যদি (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		গচ্ছ out;
	}

	/* Does the inode have space for EA? */
	যদি (inode->i_extra_isize < পরিমাণ(inode->i_extra_isize) ||
	    inode_size <= EXT2_GOOD_OLD_INODE_SIZE + inode->i_extra_isize +
								পরিমাণ(__u32))
		গচ্ছ write_ea_block;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	memcpy(((ৱর্ণ *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, &ea_inode_magic, পরিমাণ(__u32));
	storage_size = inode_size - EXT2_GOOD_OLD_INODE_SIZE -
				inode->i_extra_isize - পরিমাণ(__u32);
	start = ((ৱর্ণ *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
				inode->i_extra_isize + পরিমাণ(__u32);

	err = write_xattrs_to_buffer(fs, handle->attrs, handle->ibody_গুনতি,
				     start, storage_size, 0, 0);
	যদি (err)
		গচ্ছ out;
write_ea_block:
	/* Are we done? */
	যদি (handle->ibody_গুনতি == handle->গুনতি &&
	    !ext2fs_file_acl_block(fs, EXT2_INODE(inode)))
		গচ্ছ skip_ea_block;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &block_buf);
	যদি (err)
		গচ্ছ out;

	storage_size = fs->blocksize - পরিমাণ(সংরচনা ext2_ext_attr_header);
	start = block_buf + পরিমাণ(সংরচনা ext2_ext_attr_header);

	err = write_xattrs_to_buffer(fs, handle->attrs + handle->ibody_গুনতি,
				     handle->গুনতি - handle->ibody_গুনতি, start,
				     storage_size, start - block_buf, 1);
	যদি (err)
		গচ্ছ out2;

	/* Write a header on the EA block */
	header = (সংরচনা ext2_ext_attr_header *) block_buf;
	header->h_magic = EXT2_EXT_ATTR_MAGIC;
	header->h_refcount = 1;
	header->h_blocks = 1;

	/* Get a new block for writing */
	err = prep_ea_block_for_write(fs, handle->ino, inode);
	যদি (err)
		গচ্ছ out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_block(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, handle->ino);
	যদি (err)
		গচ্ছ out2;

skip_ea_block:
	blk = ext2fs_file_acl_block(fs, (সংরচনা ext2_inode *)inode);
	যদি (!block_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		যদি (err)
			গচ্ছ out;
	}

	/* Write the inode */
	err = ext2fs_write_inode_full(fs, handle->ino, EXT2_INODE(inode),
				      inode_size);
	যদি (err)
		গচ্ছ out2;

out2:
	ext2fs_free_mem(&block_buf);
out:
	ext2fs_free_mem(&inode);
	ঘূৰাই err;
}

স্থির errcode_t read_xattrs_from_buffer(সংরচনা ext2_xattr_handle *handle,
					 সংরচনা ext2_inode_large *inode,
					 সংরচনা ext2_ext_attr_entry *entries,
					 unsigned পূর্ণাঙ্ক storage_size,
					 ৱর্ণ *মান_start)
{
	সংরচনা ext2_xattr *x;
	সংরচনা ext2_ext_attr_entry *entry, *end;
	ধ্রুৱ ৱর্ণ *prefix;
	unsigned পূর্ণাঙ্ক remain, prefix_len;
	errcode_t err;
	unsigned পূর্ণাঙ্ক মানসমূহ_size = storage_size +
			((ৱর্ণ *)entries - মান_start);

	/* find the end */
	end = entries;
	remain = storage_size;
	যেতিয়ালৈ (remain >= পরিমাণ(সংরচনা ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(end)) {

		/* header eats this space */
		remain -= পরিমাণ(সংরচনা ext2_ext_attr_entry);

		/* is attribute name valid? */
		যদি (EXT2_EXT_ATTR_SIZE(end->e_name_len) > remain)
			ঘূৰাই EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(end->e_name_len);
		end = EXT2_EXT_ATTR_NEXT(end);
	}

	entry = entries;
	remain = storage_size;
	যেতিয়ালৈ (remain >= পরিমাণ(সংরচনা ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(entry)) {

		/* Allocate space for more attrs? */
		যদি (handle->গুনতি == handle->capacity) {
			err = ext2fs_xattrs_expand(handle, 4);
			যদি (err)
				ঘূৰাই err;
		}

		x = handle->attrs + handle->গুনতি;

		/* header eats this space */
		remain -= পরিমাণ(সংরচনা ext2_ext_attr_entry);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(entry->e_name_len);

		/* Extract name */
		prefix = find_ea_prefix(entry->e_name_index);
		prefix_len = (prefix ? strlen(prefix) : 0);
		err = ext2fs_get_memzero(entry->e_name_len + prefix_len + 1,
					 &x->name);
		যদি (err)
			ঘূৰাই err;
		যদি (prefix)
			memcpy(x->name, prefix, prefix_len);
		যদি (entry->e_name_len)
			memcpy(x->name + prefix_len,
			       (ৱর্ণ *)entry + পরিমাণ(*entry),
			       entry->e_name_len);
		x->short_name = x->name + prefix_len;
		x->name_index = entry->e_name_index;

		/* Check & copy value */
		যদি (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    entry->e_মান_inum != 0)
			ঘূৰাই EXT2_ET_BAD_EA_BLOCK_অঙ্ক;

		যদি (entry->e_মান_inum == 0) {
			যদি (entry->e_মান_size > remain)
				ঘূৰাই EXT2_ET_EA_BAD_মান_SIZE;

			যদি (entry->e_মান_offs + entry->e_মান_size > মানসমূহ_size)
				ঘূৰাই EXT2_ET_EA_BAD_মান_OFFSET;

			যদি (entry->e_মান_size > 0 &&
			    মান_start + entry->e_মান_offs <
			    (ৱর্ণ *)end + পরিমাণ(__u32))
				ঘূৰাই EXT2_ET_EA_BAD_মান_OFFSET;

			remain -= entry->e_মান_size;

			err = ext2fs_get_mem(entry->e_মান_size, &x->মান);
			যদি (err)
				ঘূৰাই err;
			memcpy(x->মান, মান_start + entry->e_মান_offs,
			       entry->e_মান_size);
		} অন্যথা {
			সংরচনা ext2_inode *ea_inode;
			ext2_file_t ea_file;

			যদি (entry->e_মান_offs != 0)
				ঘূৰাই EXT2_ET_EA_BAD_মান_OFFSET;

			যদি (entry->e_মান_size > (64 * 1024))
				ঘূৰাই EXT2_ET_EA_BAD_মান_SIZE;

			err = ext2fs_get_mem(entry->e_মান_size, &x->মান);
			যদি (err)
				ঘূৰাই err;

			err = ext2fs_file_open(handle->fs, entry->e_মান_inum,
					       0, &ea_file);
			যদি (err)
				ঘূৰাই err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			যদি ((ea_inode->i_flags & EXT4_INLINE_DATA_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_গুনতি == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			অন্যথা যদি ((__u64) ext2fs_file_get_size(ea_file) !=
				 entry->e_মান_size)
				err = EXT2_ET_EA_BAD_মান_SIZE;
			অন্যথা
				err = ext2fs_file_read(ea_file, x->মান,
						       entry->e_মান_size, 0);
			ext2fs_file_close(ea_file);
			যদি (err)
				ঘূৰাই err;
		}

		x->ea_ino = entry->e_মান_inum;
		x->মান_len = entry->e_মান_size;

		/* e_hash may be 0 in older inode's ea */
		যদি (entry->e_hash != 0) {
			__u32 hash, signed_hash;

			শূন্য *data = (entry->e_মান_inum != 0) ?
					0 : মান_start + entry->e_মান_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, entry,
							  data, &hash,
							  &signed_hash);
			যদি (err)
				ঘূৰাই err;
			যদি ((entry->e_hash != hash) &&
			    (entry->e_hash != signed_hash)) {
				সংরচনা ext2_inode child;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							entry->e_মান_inum,
							&child);
				যদি (err)
					ঘূৰাই err;
				যদি (child.i_mtime != handle->ino ||
				    child.i_generation != inode->i_generation)
					ঘূৰাই EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->গুনতি++;
		entry = EXT2_EXT_ATTR_NEXT(entry);
	}

	ঘূৰাই 0;
}

স্থির শূন্য xattrs_free_চাবিগুলি(সংরচনা ext2_xattr_handle *h)
{
	সংরচনা ext2_xattr *a = h->attrs;
	পূর্ণাঙ্ক i;

	বাবে (i = 0; i < h->capacity; i++) {
		যদি (a[i].name)
			ext2fs_free_mem(&a[i].name);
		যদি (a[i].মান)
			ext2fs_free_mem(&a[i].মান);
	}
	h->গুনতি = 0;
	h->ibody_গুনতি = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(সংরচনা ext2_xattr_handle *handle,
				   সংরচনা ext2_inode_large *inode)
{
	সংরচনা ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	unsigned পূর্ণাঙ্ক storage_size;
	ৱর্ণ *start, *block_buf = রিক্ত;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_চাবিগুলি(handle);

	/* Does the inode have space for EA? */
	যদি (inode->i_extra_isize < পরিমাণ(inode->i_extra_isize) ||
	    EXT2_INODE_SIZE(handle->fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  পরিমাণ(__u32))
		গচ্ছ read_ea_block;
	যদি (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		গচ্ছ out;
	}

	/* Look for EA in the inode */
	memcpy(&ea_inode_magic, ((ৱর্ণ *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, পরিমাণ(__u32));
	যদি (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_size = EXT2_INODE_SIZE(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_SIZE - inode->i_extra_isize -
			পরিমাণ(__u32);
		start = ((ৱর্ণ *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + পরিমাণ(__u32);

		err = read_xattrs_from_buffer(handle, inode,
					(সংরচনা ext2_ext_attr_entry *) start,
					storage_size, start);
		যদি (err)
			গচ্ছ out;

		handle->ibody_গুনতি = handle->গুনতি;
	}

read_ea_block:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_block(handle->fs, EXT2_INODE(inode));
	যদি (blk != 0) {
		যদি ((blk < handle->fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_গুনতি(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_অঙ্ক;
			গচ্ছ out;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &block_buf);
		যদি (err)
			গচ্ছ out;

		err = ext2fs_read_ext_attr3(handle->fs, blk, block_buf,
					    handle->ino);
		যদি (err)
			গচ্ছ out3;

		/* We only know how to deal with v2 EA blocks */
		header = (সংরচনা ext2_ext_attr_header *) block_buf;
		যদি (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			গচ্ছ out3;
		}

		/* Read EAs */
		storage_size = handle->fs->blocksize -
			পরিমাণ(সংরচনা ext2_ext_attr_header);
		start = block_buf + পরিমাণ(সংরচনা ext2_ext_attr_header);
		err = read_xattrs_from_buffer(handle, inode,
					(সংরচনা ext2_ext_attr_entry *) start,
					storage_size, block_buf);
	}

out3:
	যদি (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	ঘূৰাই err;
}

errcode_t ext2fs_xattrs_read(সংরচনা ext2_xattr_handle *handle)
{
	সংরচনা ext2_inode_large *inode;
	size_t inode_size = EXT2_INODE_SIZE(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	যদি (inode_size < পরিমাণ(*inode))
		inode_size = পরিমাণ(*inode);
	err = ext2fs_get_memzero(inode_size, &inode);
	যদি (err)
		ঘূৰাই err;

	err = ext2fs_read_inode_full(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_SIZE(handle->fs->super));
	যদি (err)
		গচ্ছ out;

	err = ext2fs_xattrs_read_inode(handle, inode);

out:
	ext2fs_free_mem(&inode);

	ঘূৰাই err;
}

errcode_t ext2fs_xattrs_iterate(সংরচনা ext2_xattr_handle *h,
				পূর্ণাঙ্ক (*func)(ৱর্ণ *name, ৱর্ণ *মান,
					    size_t মান_len, শূন্য *data),
				শূন্য *data)
{
	সংরচনা ext2_xattr *x;
	পূর্ণাঙ্ক dirty = 0;
	পূর্ণাঙ্ক ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	বাবে (x = h->attrs; x < h->attrs + h->গুনতি; x++) {
		ret = func(x->name, x->মান, x->মান_len, data);
		যদি (ret & XATTR_CHANGED)
			dirty = 1;
		যদি (ret & XATTR_ABORT)
			ভঙ্গ;
	}

	যদি (dirty)
		ঘূৰাই ext2fs_xattrs_write(h);
	ঘূৰাই 0;
}

errcode_t ext2fs_xattr_get(সংরচনা ext2_xattr_handle *h, ধ্রুৱ ৱর্ণ *চাবি,
			   শূন্য **মান, size_t *মান_len)
{
	সংরচনা ext2_xattr *x;
	ৱর্ণ *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	বাবে (x = h->attrs; x < h->attrs + h->গুনতি; x++) {
		যদি (strcmp(x->name, চাবি))
			অনুৱর্ত;

		যদি (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(চাবি, "system.posix_acl_default") == 0) ||
		     (strcmp(চাবি, "system.posix_acl_access") == 0))) {
			err = convert_disk_buffer_to_posix_acl(x->মান, x->মান_len,
							       মান, মান_len);
			ঘূৰাই err;
		} অন্যথা {
			err = ext2fs_get_mem(x->মান_len, &val);
			যদি (err)
				ঘূৰাই err;
			memcpy(val, x->মান, x->মান_len);
			*মান = val;
			*মান_len = x->মান_len;
			ঘূৰাই 0;
		}
	}

	ঘূৰাই EXT2_ET_EA_চাবি_NOT_FOUND;
}

errcode_t ext2fs_xattr_inode_max_size(ext2_filsys fs, ext2_ino_t ino,
				      size_t *size)
{
	সংরচনা ext2_ext_attr_entry *entry;
	সংরচনা ext2_inode_large *inode;
	__u32 ea_inode_magic;
	unsigned পূর্ণাঙ্ক minoff;
	ৱর্ণ *start;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_SIZE(fs->super);
	যদি (i < পরিমাণ(*inode))
		i = পরিমাণ(*inode);
	err = ext2fs_get_memzero(i, &inode);
	যদি (err)
		ঘূৰাই err;

	err = ext2fs_read_inode_full(fs, ino, (সংরচনা ext2_inode *)inode,
				     EXT2_INODE_SIZE(fs->super));
	যদি (err)
		গচ্ছ out;

	/* Does the inode have size for EA? */
	যদি (EXT2_INODE_SIZE(fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  পরিমাণ(__u32)) {
		err = EXT2_ET_INLINE_DATA_NO_SPACE;
		গচ্ছ out;
	}

	minoff = EXT2_INODE_SIZE(fs->super) - পরিমাণ(*inode) - পরিমাণ(__u32);
	memcpy(&ea_inode_magic, ((ৱর্ণ *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, পরিমাণ(__u32));
	যদি (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		start= ((ৱর্ণ *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + পরিমাণ(__u32);
		entry = (সংরচনা ext2_ext_attr_entry *) start;
		যেতিয়ালৈ (!EXT2_EXT_IS_LAST_ENTRY(entry)) {
			যদি (!entry->e_মান_inum && entry->e_মান_size) {
				unsigned পূর্ণাঙ্ক offs = entry->e_মান_offs;
				যদি (offs < minoff)
					minoff = offs;
			}
			entry = EXT2_EXT_ATTR_NEXT(entry);
		}
		*size = minoff - ((ৱর্ণ *)entry - (ৱর্ণ *)start) - পরিমাণ(__u32);
	} অন্যথা {
		/* no xattr.  return a maximum size */
		*size = EXT2_EXT_ATTR_SIZE(minoff -
					   EXT2_EXT_ATTR_LEN(strlen("data")) -
					   EXT2_EXT_ATTR_ROUND - পরিমাণ(__u32));
	}

out:
	ext2fs_free_mem(&inode);
	ঘূৰাই err;
}

স্থির errcode_t xattr_create_ea_inode(ext2_filsys fs, ধ্রুৱ শূন্য *মান,
				       size_t মান_len, ext2_ino_t *ea_ino)
{
	সংরচনা ext2_inode inode;
	ext2_ino_t ino;
	ext2_file_t file;
	__u32 hash;
	errcode_t ret;

	ret = ext2fs_new_inode(fs, 0, 0, 0, &ino);
	যদি (ret)
		ঘূৰাই ret;

	memset(&inode, 0, পরিমাণ(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	যদি (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_size = 0;
	inode.i_mode = LINUX_S_IFREG | 0600;
	inode.i_links_গুনতি = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	যদি (ret)
		ঘূৰাই ret;
	/*
	 * ref_count and hash utilize inode's i_*time fields.
	 * ext2fs_write_new_inode() call above initializes these fields with
	 * current time. That's why ref count and hash updates are done
	 * separately below.
	 */
	ext2fs_set_ea_inode_ref(&inode, 1);
	hash = ext2fs_crc32c_le(fs->csum_seed, মান, মান_len);
	ext2fs_set_ea_inode_hash(&inode, hash);

	ret = ext2fs_write_inode(fs, ino, &inode);
	যদি (ret)
		ঘূৰাই ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	যদি (ret)
		ঘূৰাই ret;
	ret = ext2fs_file_write(file, মান, মান_len, রিক্ত);
	ext2fs_file_close(file);
	যদি (ret)
		ঘূৰাই ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	ঘূৰাই 0;
}

স্থির errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	সংরচনা ext2_inode_large inode;
	__u64 ref_গুনতি;
	errcode_t ret;

	ret = ext2fs_read_inode_full(fs, ino, (সংরচনা ext2_inode *)&inode,
				     পরিমাণ(inode));
	যদি (ret)
		গচ্ছ out;

	ref_গুনতি = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_গুনতি--;
	ext2fs_set_ea_inode_ref(EXT2_INODE(&inode), ref_গুনতি);

	যদি (ref_গুনতি)
		গচ্ছ write_out;

	inode.i_links_গুনতি = 0;
	ext2fs_set_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	যদি (ret)
		গচ্ছ write_out;

	যদি (ext2fs_inode_has_valid_blocks2(fs, (সংরচনা ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (সংরচনা ext2_inode *)&inode, রিক্ত,
				   0, ~0ULL);
		যদি (ret)
			গচ্ছ out;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_out:
	ret = ext2fs_write_inode_full(fs, ino, (সংরচনা ext2_inode *)&inode,
				      পরিমাণ(inode));
out:
	ঘূৰাই ret;
}

স্থির errcode_t xattr_update_entry(ext2_filsys fs, সংরচনা ext2_xattr *x,
				    ধ্রুৱ ৱর্ণ *name, ধ্রুৱ ৱর্ণ *short_name,
				    পূর্ণাঙ্ক index, ধ্রুৱ শূন্য *মান,
				    size_t মান_len, পূর্ণাঙ্ক in_inode)
{
	ext2_ino_t ea_ino = 0;
	শূন্য *new_মান = রিক্ত;
	ৱর্ণ *new_name = রিক্ত;
	পূর্ণাঙ্ক name_len;
	errcode_t ret;

	যদি (!x->name) {
		name_len = strlen(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		যদি (ret)
			গচ্ছ fail;
		memcpy(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(মান_len, &new_মান);
	যদি (ret)
		গচ্ছ fail;
	memcpy(new_মান, মান, মান_len);

	যদি (in_inode) {
		ret = xattr_create_ea_inode(fs, মান, মান_len, &ea_ino);
		যদি (ret)
			গচ্ছ fail;
	}

	যদি (x->ea_ino) {
		ret = xattr_inode_dec_ref(fs, x->ea_ino);
		যদি (ret)
			গচ্ছ fail;
	}

	যদি (!x->name) {
		x->name = new_name;
		x->short_name = new_name + (short_name  - name);
	}
	x->name_index = index;

	যদি (x->মান)
		ext2fs_free_mem(&x->মান);
	x->মান = new_মান;
	x->মান_len = মান_len;
	x->ea_ino = ea_ino;
	ঘূৰাই 0;
fail:
	যদি (new_name)
		ext2fs_free_mem(&new_name);
	যদি (new_মান)
		ext2fs_free_mem(&new_মান);
	যদি (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক xattr_find_position(সংরচনা ext2_xattr *attrs, পূর্ণাঙ্ক গুনতি,
			       ধ্রুৱ ৱর্ণ *shortname, পূর্ণাঙ্ক name_idx)
{
	সংরচনা ext2_xattr *x;
	পূর্ণাঙ্ক i;
	পূর্ণাঙ্ক shortname_len, x_shortname_len;

	shortname_len = strlen(shortname);

	বাবে (i = 0, x = attrs; i < গুনতি; i++, x++) {
		যদি (name_idx < x->name_index)
			ভঙ্গ;
		যদি (name_idx > x->name_index)
			অনুৱর্ত;

		x_shortname_len = strlen(x->short_name);
		যদি (shortname_len < x_shortname_len)
			ভঙ্গ;
		যদি (shortname_len > x_shortname_len)
			অনুৱর্ত;

		যদি (memcmp(shortname, x->short_name, shortname_len) <= 0)
			ভঙ্গ;
	}
	ঘূৰাই i;
}

স্থির errcode_t xattr_array_update(সংরচনা ext2_xattr_handle *h,
				    ধ্রুৱ ৱর্ণ *name,
				    ধ্রুৱ শূন্য *মান, size_t মান_len,
				    পূর্ণাঙ্ক ibody_free, পূর্ণাঙ্ক block_free,
				    পূর্ণাঙ্ক old_idx, পূর্ণাঙ্ক in_inode)
{
	সংরচনা ext2_xattr tmp;
	পূর্ণাঙ্ক add_to_ibody;
	পূর্ণাঙ্ক needed;
	পূর্ণাঙ্ক name_len, name_idx = 0;
	ধ্রুৱ ৱর্ণ *shortname = name;
	পূর্ণাঙ্ক new_idx;
	পূর্ণাঙ্ক ret;

	find_ea_index(name, &shortname, &name_idx);
	name_len = strlen(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	যদি (!in_inode)
		needed += EXT2_EXT_ATTR_SIZE(মান_len);

	যদি (old_idx >= 0 && old_idx < h->ibody_গুনতি) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		যদি (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_SIZE(
						h->attrs[old_idx].মান_len);
	}

	যদি (needed <= ibody_free) {
		যদি (old_idx < 0) {
			new_idx = h->ibody_গুনতি;
			add_to_ibody = 1;
			গচ্ছ add_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, মান,
					 মান_len, in_inode);
		যদি (ret)
			ঘূৰাই ret;
		যদি (h->ibody_গুনতি <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_গুনতি + 1,
				h->attrs + h->ibody_গুনতি,
				(old_idx - h->ibody_গুনতি) * পরিমাণ(*h->attrs));
			h->attrs[h->ibody_গুনতি] = tmp;
			h->ibody_গুনতি++;
		}
		ঘূৰাই 0;
	}

	যদি (h->ibody_গুনতি <= old_idx) {
		block_free += EXT2_EXT_ATTR_LEN(name_len);
		যদি (!h->attrs[old_idx].ea_ino)
			block_free +=
				EXT2_EXT_ATTR_SIZE(h->attrs[old_idx].মান_len);
	}

	যদি (needed > block_free)
		ঘূৰাই EXT2_ET_EA_NO_SPACE;

	যদি (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, মান,
					 মান_len, in_inode);
		যদি (ret)
			ঘূৰাই ret;
		যদি (old_idx < h->ibody_গুনতি) {
			/*
			 * Move entry from ibody to the block. Note that
			 * entries in the block are sorted.
			 */
			new_idx = xattr_find_position(h->attrs + h->ibody_গুনতি,
						      h->গুনতি - h->ibody_গুনতি,
						      shortname, name_idx);
			new_idx += h->ibody_গুনতি - 1;
			tmp = h->attrs[old_idx];
			memmove(h->attrs + old_idx, h->attrs + old_idx + 1,
				(new_idx - old_idx) * পরিমাণ(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_গুনতি--;
		}
		ঘূৰাই 0;
	}

	new_idx = xattr_find_position(h->attrs + h->ibody_গুনতি,
				      h->গুনতি - h->ibody_গুনতি,
				      shortname, name_idx);
	new_idx += h->ibody_গুনতি;
	add_to_ibody = 0;

add_new:
	যদি (h->গুনতি == h->capacity) {
		ret = ext2fs_xattrs_expand(h, 4);
		যদি (ret)
			ঘূৰাই ret;
	}

	ret = xattr_update_entry(h->fs, &h->attrs[h->গুনতি], name, shortname,
				 name_idx, মান, মান_len, in_inode);
	যদি (ret)
		ঘূৰাই ret;

	tmp = h->attrs[h->গুনতি];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->গুনতি - new_idx)*পরিমাণ(*h->attrs));
	h->attrs[new_idx] = tmp;
	যদি (add_to_ibody)
		h->ibody_গুনতি++;
	h->গুনতি++;
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক space_used(সংরচনা ext2_xattr *attrs, পূর্ণাঙ্ক গুনতি)
{
	পূর্ণাঙ্ক total = 0;
	সংরচনা ext2_xattr *x;
	পূর্ণাঙ্ক i, len;

	বাবে (i = 0, x = attrs; i < গুনতি; i++, x++) {
		len = strlen(x->short_name);
		total += EXT2_EXT_ATTR_LEN(len);
		যদি (!x->ea_ino)
			total += EXT2_EXT_ATTR_SIZE(x->মান_len);
	}
	ঘূৰাই total;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#define EXT4_XATTR_MIN_LARGE_EA_SIZE(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - পরিমাণ(সংরচনা ext2_ext_attr_header) - 4)

errcode_t ext2fs_xattr_set(সংরচনা ext2_xattr_handle *h,
			   ধ্রুৱ ৱর্ণ *name,
			   ধ্রুৱ শূন্য *মান,
			   size_t মান_len)
{
	ext2_filsys fs = h->fs;
	ধ্রুৱ পূর্ণাঙ্ক inode_size = EXT2_INODE_SIZE(fs->super);
	সংরচনা ext2_inode_large *inode = রিক্ত;
	সংরচনা ext2_xattr *x;
	ৱর্ণ *new_মান;
	পূর্ণাঙ্ক ibody_free, block_free;
	পূর্ণাঙ্ক in_inode = 0;
	পূর্ণাঙ্ক old_idx = -1;
	পূর্ণাঙ্ক extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(মান_len, &new_মান);
	যদি (ret)
		ঘূৰাই ret;
	যদি (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((strcmp(name, "system.posix_acl_default") == 0) ||
	     (strcmp(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_to_disk_buffer(মান, মান_len,
						       new_মান, &মান_len);
		যদি (ret)
			গচ্ছ out;
	} অন্যথা যদি (মান_len)
		memcpy(new_মান, মান, মান_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	বাবে (x = h->attrs; x < h->attrs + h->গুনতি; x++) {
		যদি (!strcmp(x->name, name)) {
			যদি (!x->ea_ino && x->মান_len == মান_len &&
			    (!মান_len ||
			     !memcmp(x->মান, new_মান, মান_len))) {
				ret = 0;
				গচ্ছ out;
			}
			old_idx = x - h->attrs;
			ভঙ্গ;
		}
	}

	ret = ext2fs_get_memzero(inode_size, &inode);
	যদি (ret)
		গচ্ছ out;
	ret = ext2fs_read_inode_full(fs, h->ino,
				     (সংরচনা ext2_inode *)inode,
				     inode_size);
	যদি (ret)
		গচ্ছ out;
	যদি (inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		extra_isize = inode->i_extra_isize;
		যদি (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			যদি (extra_isize == 0)
				extra_isize = পরিমাণ(__u32);
		}
		ibody_free = inode_size - EXT2_GOOD_OLD_INODE_SIZE;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= পরিমাণ(__u32) * 2;
		ibody_free -= space_used(h->attrs, h->ibody_গুনতি);
	} অন্যথা
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	যদি (strcmp(name, "system.data") == 0) {
		যদি (h->ibody_গুনতি <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			গচ্ছ out;
		}
		ret = xattr_array_update(h, name, new_মান, মান_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		যদি (ret)
			গচ্ছ out;
		গচ্ছ write_out;
	}

	block_free = fs->blocksize;
	block_free -= পরিমাণ(সংরচনা ext2_ext_attr_header);
	/* Final null entry. */
	block_free -= পরিমাণ(__u32);
	block_free -= space_used(h->attrs + h->ibody_গুনতি,
				 h->গুনতি - h->ibody_গুনতি);

	যদি (ext2fs_has_feature_ea_inode(fs->super) &&
	    মান_len > EXT4_XATTR_MIN_LARGE_EA_SIZE(fs->blocksize))
		in_inode = 1;

	ret = xattr_array_update(h, name, new_মান, মান_len, ibody_free,
				 block_free, old_idx, in_inode);
	যদি (ret == EXT2_ET_EA_NO_SPACE && !in_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_মান, মান_len,
			ibody_free, block_free, old_idx, 1 /* in_inode */);
	যদি (ret)
		গচ্ছ out;

write_out:
	ret = ext2fs_xattrs_write(h);
out:
	যদি (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_মান);
	ঘূৰাই ret;
}

errcode_t ext2fs_xattr_remove(সংরচনা ext2_xattr_handle *handle,
			      ধ্রুৱ ৱর্ণ *চাবি)
{
	সংরচনা ext2_xattr *x;
	সংরচনা ext2_xattr *end = handle->attrs + handle->গুনতি;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	বাবে (x = handle->attrs; x < end; x++) {
		যদি (strcmp(x->name, চাবি) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->মান);
			যদি (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (end - x - 1)*পরিমাণ(*x));
			memset(end - 1, 0, পরিমাণ(*end));
			যদি (x < handle->attrs + handle->ibody_গুনতি)
				handle->ibody_গুনতি--;
			handle->গুনতি--;
			ঘূৰাই ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	ঘূৰাই 0;
}

errcode_t ext2fs_xattr_remove_all(সংরচনা ext2_xattr_handle *handle)
{
	সংরচনা ext2_xattr *x;
	সংরচনা ext2_xattr *end = handle->attrs + handle->গুনতি;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	বাবে (x = handle->attrs; x < end; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->মান);
		যদি (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_গুনতি = 0;
	handle->গুনতি = 0;
	ঘূৰাই ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     সংরচনা ext2_xattr_handle **handle)
{
	সংরচনা ext2_xattr_handle *h;
	errcode_t err;

	যদি (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_data(fs->super))
		ঘূৰাই EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(পরিমাণ(*h), &h);
	যদি (err)
		ঘূৰাই err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->capacity = 4;
	err = ext2fs_get_arrayzero(h->capacity, পরিমাণ(সংরচনা ext2_xattr),
				   &h->attrs);
	যদি (err) {
		ext2fs_free_mem(&h);
		ঘূৰাই err;
	}
	h->গুনতি = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	ঘূৰাই 0;
}

errcode_t ext2fs_xattrs_close(সংরচনা ext2_xattr_handle **handle)
{
	সংরচনা ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_চাবিগুলি(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	ঘূৰাই 0;
}

errcode_t ext2fs_xattrs_গুনতি(সংরচনা ext2_xattr_handle *handle, size_t *গুনতি)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*গুনতি = handle->গুনতি;
	ঘূৰাই 0;
}

errcode_t ext2fs_xattrs_flags(সংরচনা ext2_xattr_handle *handle,
			      unsigned পূর্ণাঙ্ক *new_flags, unsigned পূর্ণাঙ্ক *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	যদি (old_flags)
		*old_flags = handle->flags;
	যদি (new_flags)
		handle->flags = *new_flags;
	ঘূৰাই 0;
}
