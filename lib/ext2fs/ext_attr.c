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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <time.h>

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2_ext_attr.h"
#அந்தர்பாவ "ext4_acl.h"

#அந்தர்பாவ "ext2fsP.h"

நிலையான errcode_t read_ea_inode_hash(ext2_filsys fs, ext2_ino_t ino, __u32 *hash)
{
	கட்டமைப்பு ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	எனில் (retval)
		திரும்பு retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	திரும்பு 0;
}

#define NAME_HASH_SHIFT 5
#define மதிப்பு_HASH_SHIFT 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_entry(கட்டமைப்பு ext2_ext_attr_entry *entry, வெறுமை *data)
{
	__u32 hash = 0;
	unsigned எழுத்து *name = (((unsigned எழுத்து *) entry) +
			       அளவு(கட்டமைப்பு ext2_ext_attr_entry));
	முழு n;

	க்கு (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*அளவு(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	எனில் (entry->e_மதிப்பு_inum == 0 && entry->e_மதிப்பு_size != 0) {
		__u32 *மதிப்பு = (__u32 *)data;
		க்கு (n = (entry->e_மதிப்பு_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << மதிப்பு_HASH_SHIFT) ^
			       (hash >> (8*அளவு(hash) - மதிப்பு_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*மதிப்பு++);
		}
	}

	திரும்பு hash;
}

__u32 ext2fs_ext_attr_hash_entry_signed(கட்டமைப்பு ext2_ext_attr_entry *entry,
					வெறுமை *data)
{
	__u32 hash = 0;
	signed எழுத்து *name = (((signed எழுத்து *) entry) +
			     அளவு(கட்டமைப்பு ext2_ext_attr_entry));
	முழு n;

	க்கு (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*அளவு(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	எனில் (entry->e_மதிப்பு_inum == 0 && entry->e_மதிப்பு_size != 0) {
		__u32 *மதிப்பு = (__u32 *)data;
		க்கு (n = (entry->e_மதிப்பு_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << மதிப்பு_HASH_SHIFT) ^
			       (hash >> (8*அளவு(hash) - மதிப்பு_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*மதிப்பு++);
		}
	}

	திரும்பு hash;
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
				      கட்டமைப்பு ext2_ext_attr_entry *entry,
				      வெறுமை *data, __u32 *hash,
				      __u32 *signed_hash)
{
	*hash = ext2fs_ext_attr_hash_entry(entry, data);
	எனில் (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_entry_signed(entry, data);

	எனில் (entry->e_மதிப்பு_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, entry->e_மதிப்பு_inum,
					    &ea_inode_hash);
		எனில் (retval)
			திரும்பு retval;

		*hash = (*hash << மதிப்பு_HASH_SHIFT) ^
			(*hash >> (8*அளவு(*hash) - மதிப்பு_HASH_SHIFT)) ^
			ea_inode_hash;
		எனில் (signed_hash)
			*signed_hash = (*signed_hash << மதிப்பு_HASH_SHIFT) ^
				(*signed_hash >> (8*அளவு(*hash) -
						  மதிப்பு_HASH_SHIFT)) ^
				ea_inode_hash;
	}
	திரும்பு 0;
}

/*
 * ext2fs_ext_attr_hash_entry2()
 *
 * Compute the hash of an extended attribute.
 * This version of the function supports hashing entries that reference
 * external inodes (ea_inode feature).
 */
errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
				      கட்டமைப்பு ext2_ext_attr_entry *entry,
				      வெறுமை *data, __u32 *hash)
{
	திரும்பு ext2fs_ext_attr_hash_entry3(fs, entry, data, hash, வெற்று);
}

#undef NAME_HASH_SHIFT
#undef மதிப்பு_HASH_SHIFT

#define BLOCK_HASH_SHIFT 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
வெறுமை ext2fs_ext_attr_block_rehash(கட்டமைப்பு ext2_ext_attr_header *header,
				  கட்டமைப்பு ext2_ext_attr_entry *end)
{
	கட்டமைப்பு ext2_ext_attr_entry *here;
	__u32 hash = 0;

	here = (கட்டமைப்பு ext2_ext_attr_entry *)(header+1);
	போது (here < end && !EXT2_EXT_IS_LAST_ENTRY(here)) {
		எனில் (!here->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			நிறுத்து;
		}
		hash = (hash << BLOCK_HASH_SHIFT) ^
		       (hash >> (8*அளவு(hash) - BLOCK_HASH_SHIFT)) ^
		       here->e_hash;
		here = EXT2_EXT_ATTR_NEXT(here);
	}
	header->h_hash = hash;
}

#undef BLOCK_HASH_SHIFT

__u32 ext2fs_get_ea_inode_hash(கட்டமைப்பு ext2_inode *inode)
{
	திரும்பு inode->i_atime;
}

வெறுமை ext2fs_set_ea_inode_hash(கட்டமைப்பு ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(கட்டமைப்பு ext2_inode *inode)
{
	திரும்பு ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_version;
}

வெறுமை ext2fs_set_ea_inode_ref(கட்டமைப்பு ext2_inode *inode, __u64 ref_மொத்தம்)
{
	inode->i_ctime = (__u32)(ref_மொத்தம் >> 32);
	inode->osd1.linux1.l_i_version = (__u32)ref_மொத்தம்;
}

நிலையான errcode_t check_ext_attr_header(கட்டமைப்பு ext2_ext_attr_header *header)
{
	எனில் ((header->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     header->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    header->h_blocks != 1)
		திரும்பு EXT2_ET_BAD_EA_HEADER;

	திரும்பு 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block, வெறுமை *buf,
				ext2_ino_t inum)
{
	முழு		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	எனில் (retval)
		திரும்பு retval;

	எனில் (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_block_csum_verify(fs, inum, block, buf))
		csum_failed = 1;

#ifdef சொற்கள்_BIGENDIAN
	ext2fs_swap_ext_attr(buf, buf, fs->blocksize, 1);
#endif

	retval = check_ext_attr_header(buf);
	எனில் (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	திரும்பு retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block, வெறுமை *buf)
{
	திரும்பு ext2fs_read_ext_attr3(fs, block, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, வெறுமை *buf)
{
	திரும்பு ext2fs_read_ext_attr2(fs, block, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block, வெறுமை *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	எழுத்து		*write_buf;

#ifdef சொற்கள்_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	எனில் (retval)
		திரும்பு retval;
	ext2fs_swap_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#இல்லையெனில்
	write_buf = (எழுத்து *) inbuf;
#endif

	retval = ext2fs_ext_attr_block_csum_set(fs, inum, block,
			(கட்டமைப்பு ext2_ext_attr_header *)write_buf);
	எனில் (retval)
		திரும்பு retval;

	retval = io_channel_write_blk64(fs->io, block, 1, write_buf);
#ifdef சொற்கள்_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#endif
	எனில் (!retval)
		ext2fs_mark_changed(fs);
	திரும்பு retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block, வெறுமை *inbuf)
{
	திரும்பு ext2fs_write_ext_attr3(fs, block, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block, வெறுமை *inbuf)
{
	திரும்பு ext2fs_write_ext_attr2(fs, block, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    எழுத்து *block_buf, முழு adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	கட்டமைப்பு ext2_ext_attr_header *header;
	எழுத்து	*buf = 0;

	எனில் ((blk >= ext2fs_blocks_மொத்தம்(fs->super)) ||
	    (blk < fs->super->s_first_data_block))
		திரும்பு EXT2_ET_BAD_EA_BLOCK_இலக்கம்;

	எனில் (!block_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		எனில் (retval)
			திரும்பு retval;
		block_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, block_buf, inum);
	எனில் (retval)
		செல் errout;

	header = (கட்டமைப்பு ext2_ext_attr_header *) block_buf;
	header->h_refcount += adjust;
	எனில் (newcount)
		*newcount = header->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, block_buf, inum);
	எனில் (retval)
		செல் errout;

errout:
	எனில் (buf)
		ext2fs_free_mem(&buf);
	திரும்பு retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    எழுத்து *block_buf, முழு adjust,
				    __u32 *newcount)
{
	திரும்பு ext2fs_adjust_ea_refcount3(fs, blk, block_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					எழுத்து *block_buf, முழு adjust,
					__u32 *newcount)
{
	திரும்பு ext2fs_adjust_ea_refcount2(fs, blk, block_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
கட்டமைப்பு ext2_xattr {
	முழு name_index;
	எழுத்து *name;
	எழுத்து *short_name;
	வெறுமை *மதிப்பு;
	unsigned முழு மதிப்பு_len;
	ext2_ino_t ea_ino;
};

கட்டமைப்பு ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	கட்டமைப்பு ext2_xattr *attrs;
	முழு capacity;
	முழு மொத்தம்;
	முழு ibody_மொத்தம்;
	ext2_ino_t ino;
	unsigned முழு flags;
};

நிலையான errcode_t ext2fs_xattrs_expand(கட்டமைப்பு ext2_xattr_handle *h,
				      unsigned முழு expandby)
{
	கட்டமைப்பு ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->capacity + expandby,
				   அளவு(கட்டமைப்பு ext2_xattr), &new_attrs);
	எனில் (err)
		திரும்பு err;

	memcpy(new_attrs, h->attrs, h->capacity * அளவு(கட்டமைப்பு ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->capacity += expandby;
	h->attrs = new_attrs;

	திரும்பு 0;
}

கட்டமைப்பு ea_name_index {
	முழு index;
	மாறிலி எழுத்து *name;
};

/* Keep these names sorted in order of decreasing specificity. */
நிலையான கட்டமைப்பு ea_name_index ea_names[] = {
	{10, "gnu."},
	{3, "system.posix_acl_default"},
	{2, "system.posix_acl_access"},
	{8, "system.richacl"},
	{6, "security."},
	{4, "trusted."},
	{7, "system."},
	{1, "user."},
	{0, வெற்று},
};

நிலையான மாறிலி எழுத்து *find_ea_prefix(முழு index)
{
	கட்டமைப்பு ea_name_index *e;

	க்கு (e = ea_names; e->name; e++)
		எனில் (e->index == index)
			திரும்பு e->name;

	திரும்பு வெற்று;
}

நிலையான முழு find_ea_index(மாறிலி எழுத்து *fullname, மாறிலி எழுத்து **name, முழு *index)
{
	கட்டமைப்பு ea_name_index *e;

	க்கு (e = ea_names; e->name; e++) {
		எனில் (strncmp(fullname, e->name, strlen(e->name)) == 0) {
			*name = fullname + strlen(e->name);
			*index = e->index;
			திரும்பு 1;
		}
	}
	திரும்பு 0;
}

errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       கட்டமைப்பு ext2_inode_large *inode)
{
	கட்டமைப்பு ext2_ext_attr_header *header;
	வெறுமை *block_buf = வெற்று;
	blk64_t blk;
	errcode_t err;
	கட்டமைப்பு ext2_inode_large i;

	/* Read inode? */
	எனில் (inode == வெற்று) {
		err = ext2fs_read_inode_full(fs, ino, (கட்டமைப்பு ext2_inode *)&i,
					     அளவு(கட்டமைப்பு ext2_inode_large));
		எனில் (err)
			திரும்பு err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (கட்டமைப்பு ext2_inode *)inode);
	எனில் (blk == 0)
		திரும்பு 0;

	/* Find block, zero it, write back */
	எனில் ((blk < fs->super->s_first_data_block) ||
	    (blk >= ext2fs_blocks_மொத்தம்(fs->super))) {
		err = EXT2_ET_BAD_EA_BLOCK_இலக்கம்;
		செல் out;
	}

	err = ext2fs_get_mem(fs->blocksize, &block_buf);
	எனில் (err)
		செல் out;

	err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
	எனில் (err)
		செல் out2;

	/* We only know how to deal with v2 EA blocks */
	header = (கட்டமைப்பு ext2_ext_attr_header *) block_buf;
	எனில் (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_HEADER;
		செல் out2;
	}

	header->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
	எனில் (err)
		செல் out2;

	/* Erase link to block */
	ext2fs_file_acl_block_set(fs, (கட்டமைப்பு ext2_inode *)inode, 0);
	எனில் (header->h_refcount == 0)
		ext2fs_block_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_blocks(fs, (கட்டமைப்பு ext2_inode *)inode, 1);
	எனில் (err)
		செல் out2;

	/* Write inode? */
	எனில் (inode == &i) {
		err = ext2fs_write_inode_full(fs, ino, (கட்டமைப்பு ext2_inode *)&i,
					      அளவு(கட்டமைப்பு ext2_inode_large));
		எனில் (err)
			செல் out2;
	}

out2:
	ext2fs_free_mem(&block_buf);
out:
	திரும்பு err;
}

நிலையான errcode_t prep_ea_block_for_write(ext2_filsys fs, ext2_ino_t ino,
					 கட்டமைப்பு ext2_inode_large *inode)
{
	கட்டமைப்பு ext2_ext_attr_header *header;
	வெறுமை *block_buf = வெற்று;
	blk64_t blk, goal;
	errcode_t err;

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (கட்டமைப்பு ext2_inode *)inode);
	எனில் (blk != 0) {
		எனில் ((blk < fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_மொத்தம்(fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_இலக்கம்;
			செல் out;
		}

		err = ext2fs_get_mem(fs->blocksize, &block_buf);
		எனில் (err)
			செல் out;

		err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
		எனில் (err)
			செல் out2;

		/* We only know how to deal with v2 EA blocks */
		header = (கட்டமைப்பு ext2_ext_attr_header *) block_buf;
		எனில் (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			செல் out2;
		}

		/* Single-user block.  We're done here. */
		எனில் (header->h_refcount == 1)
			செல் out2;

		/* We need to CoW the block. */
		header->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
		எனில் (err)
			செல் out2;
	} இல்லையெனில் {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_add_blocks(fs, (கட்டமைப்பு ext2_inode *)inode,
					     1);
		எனில் (err)
			செல் out;
	}

	/* Allocate a block */
	goal = ext2fs_find_inode_goal(fs, ino, (கட்டமைப்பு ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, வெற்று, &blk);
	எனில் (err)
		செல் out2;
	ext2fs_file_acl_block_set(fs, (கட்டமைப்பு ext2_inode *)inode, blk);
out2:
	எனில் (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	திரும்பு err;
}


நிலையான உள்வரி முழு
posix_acl_xattr_மொத்தம்(size_t size)
{
        எனில் (size < அளவு(posix_acl_xattr_header))
                திரும்பு -1;
        size -= அளவு(posix_acl_xattr_header);
        எனில் (size % அளவு(posix_acl_xattr_entry))
                திரும்பு -1;
        திரும்பு size / அளவு(posix_acl_xattr_entry);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
நிலையான errcode_t convert_posix_acl_to_disk_buffer(மாறிலி வெறுமை *மதிப்பு, size_t size,
						  வெறுமை *out_buf, size_t *size_out)
{
	மாறிலி posix_acl_xattr_header *header =
		(மாறிலி posix_acl_xattr_header*) மதிப்பு;
	மாறிலி posix_acl_xattr_entry *end, *entry =
		(மாறிலி posix_acl_xattr_entry *)(header+1);
	ext4_acl_header *ext_acl;
	size_t s;
	எழுத்து *e;

	முழு மொத்தம்;

	எனில் (!மதிப்பு)
		திரும்பு EINVAL;
	எனில் (size < அளவு(posix_acl_xattr_header))
		திரும்பு ENOMEM;
	எனில் (header->a_version != ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION))
		திரும்பு EINVAL;

	மொத்தம் = posix_acl_xattr_மொத்தம்(size);
	ext_acl = out_buf;
	ext_acl->a_version = ext2fs_cpu_to_le32(EXT4_ACL_VERSION);

	எனில் (மொத்தம் <= 0)
		திரும்பு EINVAL;

	e = (எழுத்து *) out_buf + அளவு(ext4_acl_header);
	s = அளவு(ext4_acl_header);
	க்கு (end = entry + மொத்தம்; entry != end;entry++) {
		ext4_acl_entry *disk_entry = (ext4_acl_entry*) e;
		disk_entry->e_tag = entry->e_tag;
		disk_entry->e_perm = entry->e_perm;

		தேர்வு(ext2fs_le16_to_cpu(entry->e_tag)) {
			நிலை ACL_USER_OBJ:
			நிலை ACL_GROUP_OBJ:
			நிலை ACL_MASK:
			நிலை ACL_OTHER:
				e += அளவு(ext4_acl_entry_short);
				s += அளவு(ext4_acl_entry_short);
				நிறுத்து;
			நிலை ACL_USER:
			நிலை ACL_GROUP:
				disk_entry->e_id = entry->e_id;
				e += அளவு(ext4_acl_entry);
				s += அளவு(ext4_acl_entry);
				நிறுத்து;
			இயல்பு:
				திரும்பு EINVAL;
		}
	}
	*size_out = s;
	திரும்பு 0;
}

நிலையான errcode_t convert_disk_buffer_to_posix_acl(மாறிலி வெறுமை *மதிப்பு, size_t size,
						  வெறுமை **out_buf, size_t *size_out)
{
	posix_acl_xattr_header *header;
	posix_acl_xattr_entry *entry;
	மாறிலி ext4_acl_header *ext_acl = (மாறிலி ext4_acl_header *) மதிப்பு;
	errcode_t err;
	மாறிலி எழுத்து *cp;
	எழுத்து *out;

	எனில் ((!மதிப்பு) ||
	    (size < அளவு(ext4_acl_header)) ||
	    (ext_acl->a_version != ext2fs_cpu_to_le32(EXT4_ACL_VERSION)))
		திரும்பு EINVAL;

	err = ext2fs_get_mem(size * 2, &out);
	எனில் (err)
		திரும்பு err;

	header = (posix_acl_xattr_header *) out;
	header->a_version = ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION);
	entry = (posix_acl_xattr_entry *) (out + அளவு(posix_acl_xattr_header));

	cp = (மாறிலி எழுத்து *) மதிப்பு + அளவு(ext4_acl_header);
	size -= அளவு(ext4_acl_header);

	போது (size > 0) {
		மாறிலி ext4_acl_entry *disk_entry = (மாறிலி ext4_acl_entry *) cp;

		entry->e_tag = disk_entry->e_tag;
		entry->e_perm = disk_entry->e_perm;

		தேர்வு(ext2fs_le16_to_cpu(entry->e_tag)) {
			நிலை ACL_USER_OBJ:
			நிலை ACL_GROUP_OBJ:
			நிலை ACL_MASK:
			நிலை ACL_OTHER:
				entry->e_id = 0;
				cp += அளவு(ext4_acl_entry_short);
				size -= அளவு(ext4_acl_entry_short);
				நிறுத்து;
			நிலை ACL_USER:
			நிலை ACL_GROUP:
				entry->e_id = disk_entry->e_id;
				cp += அளவு(ext4_acl_entry);
				size -= அளவு(ext4_acl_entry);
				நிறுத்து;
			இயல்பு:
				ext2fs_free_mem(&out);
				திரும்பு EINVAL;
		}
		entry++;
	}
	*out_buf = out;
	*size_out = ((எழுத்து *) entry - out);
	திரும்பு 0;
}

நிலையான errcode_t
write_xattrs_to_buffer(ext2_filsys fs, கட்டமைப்பு ext2_xattr *attrs, முழு மொத்தம்,
		       வெறுமை *entries_start, unsigned முழு storage_size,
		       unsigned முழு மதிப்பு_offset_correction, முழு write_hash)
{
	கட்டமைப்பு ext2_xattr *x;
	கட்டமைப்பு ext2_ext_attr_entry *e = entries_start;
	எழுத்து *end = (எழுத்து *) entries_start + storage_size;
	unsigned முழு மதிப்பு_size;
	errcode_t err;

	memset(entries_start, 0, storage_size);
	க்கு (x = attrs; x < attrs + மொத்தம்; x++) {
		மதிப்பு_size = ((x->மதிப்பு_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = strlen(x->short_name);
		e->e_name_index = x->name_index;

		e->e_மதிப்பு_size = x->மதிப்பு_len;
		e->e_மதிப்பு_inum = x->ea_ino;

		/* Store name */
		memcpy((எழுத்து *)e + அளவு(*e), x->short_name, e->e_name_len);
		எனில் (x->ea_ino) {
			e->e_மதிப்பு_offs = 0;
		} இல்லையெனில் {
			end -= மதிப்பு_size;
			e->e_மதிப்பு_offs = end - (எழுத்து *) entries_start +
						மதிப்பு_offset_correction;
			memcpy(end, x->மதிப்பு, e->e_மதிப்பு_size);
		}

		எனில் (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : end,
							  &e->e_hash);
			எனில் (err)
				திரும்பு err;
		} இல்லையெனில்
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_NEXT(e);
		*(__u32 *)e = 0;
	}
	திரும்பு 0;
}

errcode_t ext2fs_xattrs_write(கட்டமைப்பு ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	மாறிலி unsigned முழு inode_size = EXT2_INODE_SIZE(fs->super);
	கட்டமைப்பு ext2_inode_large *inode;
	எழுத்து *start, *block_buf = வெற்று;
	கட்டமைப்பு ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	blk64_t blk;
	unsigned முழு storage_size;
	unsigned முழு i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_size;
	எனில் (i < அளவு(*inode))
		i = அளவு(*inode);
	err = ext2fs_get_memzero(i, &inode);
	எனில் (err)
		திரும்பு err;

	err = ext2fs_read_inode_full(fs, handle->ino, EXT2_INODE(inode),
				     inode_size);
	எனில் (err)
		செல் out;

	/* If extra_isize isn't set, we need to set it now */
	எனில் (inode->i_extra_isize == 0 &&
	    inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		எழுத்து *p = (எழுத்து *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		எனில் (extra == 0)
			extra = அளவு(__u32);
		memset(p + EXT2_GOOD_OLD_INODE_SIZE, 0, extra);
		inode->i_extra_isize = extra;
	}
	எனில் (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		செல் out;
	}

	/* Does the inode have space for EA? */
	எனில் (inode->i_extra_isize < அளவு(inode->i_extra_isize) ||
	    inode_size <= EXT2_GOOD_OLD_INODE_SIZE + inode->i_extra_isize +
								அளவு(__u32))
		செல் write_ea_block;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	memcpy(((எழுத்து *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, &ea_inode_magic, அளவு(__u32));
	storage_size = inode_size - EXT2_GOOD_OLD_INODE_SIZE -
				inode->i_extra_isize - அளவு(__u32);
	start = ((எழுத்து *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
				inode->i_extra_isize + அளவு(__u32);

	err = write_xattrs_to_buffer(fs, handle->attrs, handle->ibody_மொத்தம்,
				     start, storage_size, 0, 0);
	எனில் (err)
		செல் out;
write_ea_block:
	/* Are we done? */
	எனில் (handle->ibody_மொத்தம் == handle->மொத்தம் &&
	    !ext2fs_file_acl_block(fs, EXT2_INODE(inode)))
		செல் skip_ea_block;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &block_buf);
	எனில் (err)
		செல் out;

	storage_size = fs->blocksize - அளவு(கட்டமைப்பு ext2_ext_attr_header);
	start = block_buf + அளவு(கட்டமைப்பு ext2_ext_attr_header);

	err = write_xattrs_to_buffer(fs, handle->attrs + handle->ibody_மொத்தம்,
				     handle->மொத்தம் - handle->ibody_மொத்தம், start,
				     storage_size, start - block_buf, 1);
	எனில் (err)
		செல் out2;

	/* Write a header on the EA block */
	header = (கட்டமைப்பு ext2_ext_attr_header *) block_buf;
	header->h_magic = EXT2_EXT_ATTR_MAGIC;
	header->h_refcount = 1;
	header->h_blocks = 1;

	/* Get a new block for writing */
	err = prep_ea_block_for_write(fs, handle->ino, inode);
	எனில் (err)
		செல் out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_block(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, handle->ino);
	எனில் (err)
		செல் out2;

skip_ea_block:
	blk = ext2fs_file_acl_block(fs, (கட்டமைப்பு ext2_inode *)inode);
	எனில் (!block_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		எனில் (err)
			செல் out;
	}

	/* Write the inode */
	err = ext2fs_write_inode_full(fs, handle->ino, EXT2_INODE(inode),
				      inode_size);
	எனில் (err)
		செல் out2;

out2:
	ext2fs_free_mem(&block_buf);
out:
	ext2fs_free_mem(&inode);
	திரும்பு err;
}

நிலையான errcode_t read_xattrs_from_buffer(கட்டமைப்பு ext2_xattr_handle *handle,
					 கட்டமைப்பு ext2_inode_large *inode,
					 கட்டமைப்பு ext2_ext_attr_entry *entries,
					 unsigned முழு storage_size,
					 எழுத்து *மதிப்பு_start)
{
	கட்டமைப்பு ext2_xattr *x;
	கட்டமைப்பு ext2_ext_attr_entry *entry, *end;
	மாறிலி எழுத்து *prefix;
	unsigned முழு remain, prefix_len;
	errcode_t err;
	unsigned முழு மதிப்புகள்_size = storage_size +
			((எழுத்து *)entries - மதிப்பு_start);

	/* find the end */
	end = entries;
	remain = storage_size;
	போது (remain >= அளவு(கட்டமைப்பு ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(end)) {

		/* header eats this space */
		remain -= அளவு(கட்டமைப்பு ext2_ext_attr_entry);

		/* is attribute name valid? */
		எனில் (EXT2_EXT_ATTR_SIZE(end->e_name_len) > remain)
			திரும்பு EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(end->e_name_len);
		end = EXT2_EXT_ATTR_NEXT(end);
	}

	entry = entries;
	remain = storage_size;
	போது (remain >= அளவு(கட்டமைப்பு ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(entry)) {

		/* Allocate space for more attrs? */
		எனில் (handle->மொத்தம் == handle->capacity) {
			err = ext2fs_xattrs_expand(handle, 4);
			எனில் (err)
				திரும்பு err;
		}

		x = handle->attrs + handle->மொத்தம்;

		/* header eats this space */
		remain -= அளவு(கட்டமைப்பு ext2_ext_attr_entry);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(entry->e_name_len);

		/* Extract name */
		prefix = find_ea_prefix(entry->e_name_index);
		prefix_len = (prefix ? strlen(prefix) : 0);
		err = ext2fs_get_memzero(entry->e_name_len + prefix_len + 1,
					 &x->name);
		எனில் (err)
			திரும்பு err;
		எனில் (prefix)
			memcpy(x->name, prefix, prefix_len);
		எனில் (entry->e_name_len)
			memcpy(x->name + prefix_len,
			       (எழுத்து *)entry + அளவு(*entry),
			       entry->e_name_len);
		x->short_name = x->name + prefix_len;
		x->name_index = entry->e_name_index;

		/* Check & copy value */
		எனில் (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    entry->e_மதிப்பு_inum != 0)
			திரும்பு EXT2_ET_BAD_EA_BLOCK_இலக்கம்;

		எனில் (entry->e_மதிப்பு_inum == 0) {
			எனில் (entry->e_மதிப்பு_size > remain)
				திரும்பு EXT2_ET_EA_BAD_மதிப்பு_SIZE;

			எனில் (entry->e_மதிப்பு_offs + entry->e_மதிப்பு_size > மதிப்புகள்_size)
				திரும்பு EXT2_ET_EA_BAD_மதிப்பு_OFFSET;

			எனில் (entry->e_மதிப்பு_size > 0 &&
			    மதிப்பு_start + entry->e_மதிப்பு_offs <
			    (எழுத்து *)end + அளவு(__u32))
				திரும்பு EXT2_ET_EA_BAD_மதிப்பு_OFFSET;

			remain -= entry->e_மதிப்பு_size;

			err = ext2fs_get_mem(entry->e_மதிப்பு_size, &x->மதிப்பு);
			எனில் (err)
				திரும்பு err;
			memcpy(x->மதிப்பு, மதிப்பு_start + entry->e_மதிப்பு_offs,
			       entry->e_மதிப்பு_size);
		} இல்லையெனில் {
			கட்டமைப்பு ext2_inode *ea_inode;
			ext2_file_t ea_file;

			எனில் (entry->e_மதிப்பு_offs != 0)
				திரும்பு EXT2_ET_EA_BAD_மதிப்பு_OFFSET;

			எனில் (entry->e_மதிப்பு_size > (64 * 1024))
				திரும்பு EXT2_ET_EA_BAD_மதிப்பு_SIZE;

			err = ext2fs_get_mem(entry->e_மதிப்பு_size, &x->மதிப்பு);
			எனில் (err)
				திரும்பு err;

			err = ext2fs_file_open(handle->fs, entry->e_மதிப்பு_inum,
					       0, &ea_file);
			எனில் (err)
				திரும்பு err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			எனில் ((ea_inode->i_flags & EXT4_INLINE_DATA_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_மொத்தம் == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			இல்லையெனில் எனில் ((__u64) ext2fs_file_get_size(ea_file) !=
				 entry->e_மதிப்பு_size)
				err = EXT2_ET_EA_BAD_மதிப்பு_SIZE;
			இல்லையெனில்
				err = ext2fs_file_read(ea_file, x->மதிப்பு,
						       entry->e_மதிப்பு_size, 0);
			ext2fs_file_close(ea_file);
			எனில் (err)
				திரும்பு err;
		}

		x->ea_ino = entry->e_மதிப்பு_inum;
		x->மதிப்பு_len = entry->e_மதிப்பு_size;

		/* e_hash may be 0 in older inode's ea */
		எனில் (entry->e_hash != 0) {
			__u32 hash, signed_hash;

			வெறுமை *data = (entry->e_மதிப்பு_inum != 0) ?
					0 : மதிப்பு_start + entry->e_மதிப்பு_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, entry,
							  data, &hash,
							  &signed_hash);
			எனில் (err)
				திரும்பு err;
			எனில் ((entry->e_hash != hash) &&
			    (entry->e_hash != signed_hash)) {
				கட்டமைப்பு ext2_inode child;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							entry->e_மதிப்பு_inum,
							&child);
				எனில் (err)
					திரும்பு err;
				எனில் (child.i_mtime != handle->ino ||
				    child.i_generation != inode->i_generation)
					திரும்பு EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->மொத்தம்++;
		entry = EXT2_EXT_ATTR_NEXT(entry);
	}

	திரும்பு 0;
}

நிலையான வெறுமை xattrs_free_சாவிகள்(கட்டமைப்பு ext2_xattr_handle *h)
{
	கட்டமைப்பு ext2_xattr *a = h->attrs;
	முழு i;

	க்கு (i = 0; i < h->capacity; i++) {
		எனில் (a[i].name)
			ext2fs_free_mem(&a[i].name);
		எனில் (a[i].மதிப்பு)
			ext2fs_free_mem(&a[i].மதிப்பு);
	}
	h->மொத்தம் = 0;
	h->ibody_மொத்தம் = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(கட்டமைப்பு ext2_xattr_handle *handle,
				   கட்டமைப்பு ext2_inode_large *inode)
{
	கட்டமைப்பு ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	unsigned முழு storage_size;
	எழுத்து *start, *block_buf = வெற்று;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_சாவிகள்(handle);

	/* Does the inode have space for EA? */
	எனில் (inode->i_extra_isize < அளவு(inode->i_extra_isize) ||
	    EXT2_INODE_SIZE(handle->fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  அளவு(__u32))
		செல் read_ea_block;
	எனில் (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		செல் out;
	}

	/* Look for EA in the inode */
	memcpy(&ea_inode_magic, ((எழுத்து *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, அளவு(__u32));
	எனில் (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_size = EXT2_INODE_SIZE(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_SIZE - inode->i_extra_isize -
			அளவு(__u32);
		start = ((எழுத்து *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + அளவு(__u32);

		err = read_xattrs_from_buffer(handle, inode,
					(கட்டமைப்பு ext2_ext_attr_entry *) start,
					storage_size, start);
		எனில் (err)
			செல் out;

		handle->ibody_மொத்தம் = handle->மொத்தம்;
	}

read_ea_block:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_block(handle->fs, EXT2_INODE(inode));
	எனில் (blk != 0) {
		எனில் ((blk < handle->fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_மொத்தம்(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_இலக்கம்;
			செல் out;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &block_buf);
		எனில் (err)
			செல் out;

		err = ext2fs_read_ext_attr3(handle->fs, blk, block_buf,
					    handle->ino);
		எனில் (err)
			செல் out3;

		/* We only know how to deal with v2 EA blocks */
		header = (கட்டமைப்பு ext2_ext_attr_header *) block_buf;
		எனில் (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			செல் out3;
		}

		/* Read EAs */
		storage_size = handle->fs->blocksize -
			அளவு(கட்டமைப்பு ext2_ext_attr_header);
		start = block_buf + அளவு(கட்டமைப்பு ext2_ext_attr_header);
		err = read_xattrs_from_buffer(handle, inode,
					(கட்டமைப்பு ext2_ext_attr_entry *) start,
					storage_size, block_buf);
	}

out3:
	எனில் (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	திரும்பு err;
}

errcode_t ext2fs_xattrs_read(கட்டமைப்பு ext2_xattr_handle *handle)
{
	கட்டமைப்பு ext2_inode_large *inode;
	size_t inode_size = EXT2_INODE_SIZE(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	எனில் (inode_size < அளவு(*inode))
		inode_size = அளவு(*inode);
	err = ext2fs_get_memzero(inode_size, &inode);
	எனில் (err)
		திரும்பு err;

	err = ext2fs_read_inode_full(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_SIZE(handle->fs->super));
	எனில் (err)
		செல் out;

	err = ext2fs_xattrs_read_inode(handle, inode);

out:
	ext2fs_free_mem(&inode);

	திரும்பு err;
}

errcode_t ext2fs_xattrs_iterate(கட்டமைப்பு ext2_xattr_handle *h,
				முழு (*func)(எழுத்து *name, எழுத்து *மதிப்பு,
					    size_t மதிப்பு_len, வெறுமை *data),
				வெறுமை *data)
{
	கட்டமைப்பு ext2_xattr *x;
	முழு dirty = 0;
	முழு ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	க்கு (x = h->attrs; x < h->attrs + h->மொத்தம்; x++) {
		ret = func(x->name, x->மதிப்பு, x->மதிப்பு_len, data);
		எனில் (ret & XATTR_CHANGED)
			dirty = 1;
		எனில் (ret & XATTR_ABORT)
			நிறுத்து;
	}

	எனில் (dirty)
		திரும்பு ext2fs_xattrs_write(h);
	திரும்பு 0;
}

errcode_t ext2fs_xattr_get(கட்டமைப்பு ext2_xattr_handle *h, மாறிலி எழுத்து *சாவி,
			   வெறுமை **மதிப்பு, size_t *மதிப்பு_len)
{
	கட்டமைப்பு ext2_xattr *x;
	எழுத்து *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	க்கு (x = h->attrs; x < h->attrs + h->மொத்தம்; x++) {
		எனில் (strcmp(x->name, சாவி))
			தொடர்;

		எனில் (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(சாவி, "system.posix_acl_default") == 0) ||
		     (strcmp(சாவி, "system.posix_acl_access") == 0))) {
			err = convert_disk_buffer_to_posix_acl(x->மதிப்பு, x->மதிப்பு_len,
							       மதிப்பு, மதிப்பு_len);
			திரும்பு err;
		} இல்லையெனில் {
			err = ext2fs_get_mem(x->மதிப்பு_len, &val);
			எனில் (err)
				திரும்பு err;
			memcpy(val, x->மதிப்பு, x->மதிப்பு_len);
			*மதிப்பு = val;
			*மதிப்பு_len = x->மதிப்பு_len;
			திரும்பு 0;
		}
	}

	திரும்பு EXT2_ET_EA_சாவி_NOT_FOUND;
}

errcode_t ext2fs_xattr_inode_max_size(ext2_filsys fs, ext2_ino_t ino,
				      size_t *size)
{
	கட்டமைப்பு ext2_ext_attr_entry *entry;
	கட்டமைப்பு ext2_inode_large *inode;
	__u32 ea_inode_magic;
	unsigned முழு minoff;
	எழுத்து *start;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_SIZE(fs->super);
	எனில் (i < அளவு(*inode))
		i = அளவு(*inode);
	err = ext2fs_get_memzero(i, &inode);
	எனில் (err)
		திரும்பு err;

	err = ext2fs_read_inode_full(fs, ino, (கட்டமைப்பு ext2_inode *)inode,
				     EXT2_INODE_SIZE(fs->super));
	எனில் (err)
		செல் out;

	/* Does the inode have size for EA? */
	எனில் (EXT2_INODE_SIZE(fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  அளவு(__u32)) {
		err = EXT2_ET_INLINE_DATA_NO_SPACE;
		செல் out;
	}

	minoff = EXT2_INODE_SIZE(fs->super) - அளவு(*inode) - அளவு(__u32);
	memcpy(&ea_inode_magic, ((எழுத்து *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, அளவு(__u32));
	எனில் (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		start= ((எழுத்து *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + அளவு(__u32);
		entry = (கட்டமைப்பு ext2_ext_attr_entry *) start;
		போது (!EXT2_EXT_IS_LAST_ENTRY(entry)) {
			எனில் (!entry->e_மதிப்பு_inum && entry->e_மதிப்பு_size) {
				unsigned முழு offs = entry->e_மதிப்பு_offs;
				எனில் (offs < minoff)
					minoff = offs;
			}
			entry = EXT2_EXT_ATTR_NEXT(entry);
		}
		*size = minoff - ((எழுத்து *)entry - (எழுத்து *)start) - அளவு(__u32);
	} இல்லையெனில் {
		/* no xattr.  return a maximum size */
		*size = EXT2_EXT_ATTR_SIZE(minoff -
					   EXT2_EXT_ATTR_LEN(strlen("data")) -
					   EXT2_EXT_ATTR_ROUND - அளவு(__u32));
	}

out:
	ext2fs_free_mem(&inode);
	திரும்பு err;
}

நிலையான errcode_t xattr_create_ea_inode(ext2_filsys fs, மாறிலி வெறுமை *மதிப்பு,
				       size_t மதிப்பு_len, ext2_ino_t *ea_ino)
{
	கட்டமைப்பு ext2_inode inode;
	ext2_ino_t ino;
	ext2_file_t file;
	__u32 hash;
	errcode_t ret;

	ret = ext2fs_new_inode(fs, 0, 0, 0, &ino);
	எனில் (ret)
		திரும்பு ret;

	memset(&inode, 0, அளவு(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	எனில் (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_size = 0;
	inode.i_mode = LINUX_S_IFREG | 0600;
	inode.i_links_மொத்தம் = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	எனில் (ret)
		திரும்பு ret;
	/*
	 * ref_count and hash utilize inode's i_*time fields.
	 * ext2fs_write_new_inode() call above initializes these fields with
	 * current time. That's why ref count and hash updates are done
	 * separately below.
	 */
	ext2fs_set_ea_inode_ref(&inode, 1);
	hash = ext2fs_crc32c_le(fs->csum_seed, மதிப்பு, மதிப்பு_len);
	ext2fs_set_ea_inode_hash(&inode, hash);

	ret = ext2fs_write_inode(fs, ino, &inode);
	எனில் (ret)
		திரும்பு ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	எனில் (ret)
		திரும்பு ret;
	ret = ext2fs_file_write(file, மதிப்பு, மதிப்பு_len, வெற்று);
	ext2fs_file_close(file);
	எனில் (ret)
		திரும்பு ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	திரும்பு 0;
}

நிலையான errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	கட்டமைப்பு ext2_inode_large inode;
	__u64 ref_மொத்தம்;
	errcode_t ret;

	ret = ext2fs_read_inode_full(fs, ino, (கட்டமைப்பு ext2_inode *)&inode,
				     அளவு(inode));
	எனில் (ret)
		செல் out;

	ref_மொத்தம் = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_மொத்தம்--;
	ext2fs_set_ea_inode_ref(EXT2_INODE(&inode), ref_மொத்தம்);

	எனில் (ref_மொத்தம்)
		செல் write_out;

	inode.i_links_மொத்தம் = 0;
	ext2fs_set_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	எனில் (ret)
		செல் write_out;

	எனில் (ext2fs_inode_has_valid_blocks2(fs, (கட்டமைப்பு ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (கட்டமைப்பு ext2_inode *)&inode, வெற்று,
				   0, ~0ULL);
		எனில் (ret)
			செல் out;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_out:
	ret = ext2fs_write_inode_full(fs, ino, (கட்டமைப்பு ext2_inode *)&inode,
				      அளவு(inode));
out:
	திரும்பு ret;
}

நிலையான errcode_t xattr_update_entry(ext2_filsys fs, கட்டமைப்பு ext2_xattr *x,
				    மாறிலி எழுத்து *name, மாறிலி எழுத்து *short_name,
				    முழு index, மாறிலி வெறுமை *மதிப்பு,
				    size_t மதிப்பு_len, முழு in_inode)
{
	ext2_ino_t ea_ino = 0;
	வெறுமை *new_மதிப்பு = வெற்று;
	எழுத்து *new_name = வெற்று;
	முழு name_len;
	errcode_t ret;

	எனில் (!x->name) {
		name_len = strlen(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		எனில் (ret)
			செல் fail;
		memcpy(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(மதிப்பு_len, &new_மதிப்பு);
	எனில் (ret)
		செல் fail;
	memcpy(new_மதிப்பு, மதிப்பு, மதிப்பு_len);

	எனில் (in_inode) {
		ret = xattr_create_ea_inode(fs, மதிப்பு, மதிப்பு_len, &ea_ino);
		எனில் (ret)
			செல் fail;
	}

	எனில் (x->ea_ino) {
		ret = xattr_inode_dec_ref(fs, x->ea_ino);
		எனில் (ret)
			செல் fail;
	}

	எனில் (!x->name) {
		x->name = new_name;
		x->short_name = new_name + (short_name  - name);
	}
	x->name_index = index;

	எனில் (x->மதிப்பு)
		ext2fs_free_mem(&x->மதிப்பு);
	x->மதிப்பு = new_மதிப்பு;
	x->மதிப்பு_len = மதிப்பு_len;
	x->ea_ino = ea_ino;
	திரும்பு 0;
fail:
	எனில் (new_name)
		ext2fs_free_mem(&new_name);
	எனில் (new_மதிப்பு)
		ext2fs_free_mem(&new_மதிப்பு);
	எனில் (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	திரும்பு ret;
}

நிலையான முழு xattr_find_position(கட்டமைப்பு ext2_xattr *attrs, முழு மொத்தம்,
			       மாறிலி எழுத்து *shortname, முழு name_idx)
{
	கட்டமைப்பு ext2_xattr *x;
	முழு i;
	முழு shortname_len, x_shortname_len;

	shortname_len = strlen(shortname);

	க்கு (i = 0, x = attrs; i < மொத்தம்; i++, x++) {
		எனில் (name_idx < x->name_index)
			நிறுத்து;
		எனில் (name_idx > x->name_index)
			தொடர்;

		x_shortname_len = strlen(x->short_name);
		எனில் (shortname_len < x_shortname_len)
			நிறுத்து;
		எனில் (shortname_len > x_shortname_len)
			தொடர்;

		எனில் (memcmp(shortname, x->short_name, shortname_len) <= 0)
			நிறுத்து;
	}
	திரும்பு i;
}

நிலையான errcode_t xattr_array_update(கட்டமைப்பு ext2_xattr_handle *h,
				    மாறிலி எழுத்து *name,
				    மாறிலி வெறுமை *மதிப்பு, size_t மதிப்பு_len,
				    முழு ibody_free, முழு block_free,
				    முழு old_idx, முழு in_inode)
{
	கட்டமைப்பு ext2_xattr tmp;
	முழு add_to_ibody;
	முழு needed;
	முழு name_len, name_idx = 0;
	மாறிலி எழுத்து *shortname = name;
	முழு new_idx;
	முழு ret;

	find_ea_index(name, &shortname, &name_idx);
	name_len = strlen(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	எனில் (!in_inode)
		needed += EXT2_EXT_ATTR_SIZE(மதிப்பு_len);

	எனில் (old_idx >= 0 && old_idx < h->ibody_மொத்தம்) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		எனில் (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_SIZE(
						h->attrs[old_idx].மதிப்பு_len);
	}

	எனில் (needed <= ibody_free) {
		எனில் (old_idx < 0) {
			new_idx = h->ibody_மொத்தம்;
			add_to_ibody = 1;
			செல் add_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, மதிப்பு,
					 மதிப்பு_len, in_inode);
		எனில் (ret)
			திரும்பு ret;
		எனில் (h->ibody_மொத்தம் <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_மொத்தம் + 1,
				h->attrs + h->ibody_மொத்தம்,
				(old_idx - h->ibody_மொத்தம்) * அளவு(*h->attrs));
			h->attrs[h->ibody_மொத்தம்] = tmp;
			h->ibody_மொத்தம்++;
		}
		திரும்பு 0;
	}

	எனில் (h->ibody_மொத்தம் <= old_idx) {
		block_free += EXT2_EXT_ATTR_LEN(name_len);
		எனில் (!h->attrs[old_idx].ea_ino)
			block_free +=
				EXT2_EXT_ATTR_SIZE(h->attrs[old_idx].மதிப்பு_len);
	}

	எனில் (needed > block_free)
		திரும்பு EXT2_ET_EA_NO_SPACE;

	எனில் (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, மதிப்பு,
					 மதிப்பு_len, in_inode);
		எனில் (ret)
			திரும்பு ret;
		எனில் (old_idx < h->ibody_மொத்தம்) {
			/*
			 * Move entry from ibody to the block. Note that
			 * entries in the block are sorted.
			 */
			new_idx = xattr_find_position(h->attrs + h->ibody_மொத்தம்,
						      h->மொத்தம் - h->ibody_மொத்தம்,
						      shortname, name_idx);
			new_idx += h->ibody_மொத்தம் - 1;
			tmp = h->attrs[old_idx];
			memmove(h->attrs + old_idx, h->attrs + old_idx + 1,
				(new_idx - old_idx) * அளவு(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_மொத்தம்--;
		}
		திரும்பு 0;
	}

	new_idx = xattr_find_position(h->attrs + h->ibody_மொத்தம்,
				      h->மொத்தம் - h->ibody_மொத்தம்,
				      shortname, name_idx);
	new_idx += h->ibody_மொத்தம்;
	add_to_ibody = 0;

add_new:
	எனில் (h->மொத்தம் == h->capacity) {
		ret = ext2fs_xattrs_expand(h, 4);
		எனில் (ret)
			திரும்பு ret;
	}

	ret = xattr_update_entry(h->fs, &h->attrs[h->மொத்தம்], name, shortname,
				 name_idx, மதிப்பு, மதிப்பு_len, in_inode);
	எனில் (ret)
		திரும்பு ret;

	tmp = h->attrs[h->மொத்தம்];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->மொத்தம் - new_idx)*அளவு(*h->attrs));
	h->attrs[new_idx] = tmp;
	எனில் (add_to_ibody)
		h->ibody_மொத்தம்++;
	h->மொத்தம்++;
	திரும்பு 0;
}

நிலையான முழு space_used(கட்டமைப்பு ext2_xattr *attrs, முழு மொத்தம்)
{
	முழு total = 0;
	கட்டமைப்பு ext2_xattr *x;
	முழு i, len;

	க்கு (i = 0, x = attrs; i < மொத்தம்; i++, x++) {
		len = strlen(x->short_name);
		total += EXT2_EXT_ATTR_LEN(len);
		எனில் (!x->ea_ino)
			total += EXT2_EXT_ATTR_SIZE(x->மதிப்பு_len);
	}
	திரும்பு total;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#define EXT4_XATTR_MIN_LARGE_EA_SIZE(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - அளவு(கட்டமைப்பு ext2_ext_attr_header) - 4)

errcode_t ext2fs_xattr_set(கட்டமைப்பு ext2_xattr_handle *h,
			   மாறிலி எழுத்து *name,
			   மாறிலி வெறுமை *மதிப்பு,
			   size_t மதிப்பு_len)
{
	ext2_filsys fs = h->fs;
	மாறிலி முழு inode_size = EXT2_INODE_SIZE(fs->super);
	கட்டமைப்பு ext2_inode_large *inode = வெற்று;
	கட்டமைப்பு ext2_xattr *x;
	எழுத்து *new_மதிப்பு;
	முழு ibody_free, block_free;
	முழு in_inode = 0;
	முழு old_idx = -1;
	முழு extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(மதிப்பு_len, &new_மதிப்பு);
	எனில் (ret)
		திரும்பு ret;
	எனில் (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((strcmp(name, "system.posix_acl_default") == 0) ||
	     (strcmp(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_to_disk_buffer(மதிப்பு, மதிப்பு_len,
						       new_மதிப்பு, &மதிப்பு_len);
		எனில் (ret)
			செல் out;
	} இல்லையெனில் எனில் (மதிப்பு_len)
		memcpy(new_மதிப்பு, மதிப்பு, மதிப்பு_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	க்கு (x = h->attrs; x < h->attrs + h->மொத்தம்; x++) {
		எனில் (!strcmp(x->name, name)) {
			எனில் (!x->ea_ino && x->மதிப்பு_len == மதிப்பு_len &&
			    (!மதிப்பு_len ||
			     !memcmp(x->மதிப்பு, new_மதிப்பு, மதிப்பு_len))) {
				ret = 0;
				செல் out;
			}
			old_idx = x - h->attrs;
			நிறுத்து;
		}
	}

	ret = ext2fs_get_memzero(inode_size, &inode);
	எனில் (ret)
		செல் out;
	ret = ext2fs_read_inode_full(fs, h->ino,
				     (கட்டமைப்பு ext2_inode *)inode,
				     inode_size);
	எனில் (ret)
		செல் out;
	எனில் (inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		extra_isize = inode->i_extra_isize;
		எனில் (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			எனில் (extra_isize == 0)
				extra_isize = அளவு(__u32);
		}
		ibody_free = inode_size - EXT2_GOOD_OLD_INODE_SIZE;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= அளவு(__u32) * 2;
		ibody_free -= space_used(h->attrs, h->ibody_மொத்தம்);
	} இல்லையெனில்
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	எனில் (strcmp(name, "system.data") == 0) {
		எனில் (h->ibody_மொத்தம் <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			செல் out;
		}
		ret = xattr_array_update(h, name, new_மதிப்பு, மதிப்பு_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		எனில் (ret)
			செல் out;
		செல் write_out;
	}

	block_free = fs->blocksize;
	block_free -= அளவு(கட்டமைப்பு ext2_ext_attr_header);
	/* Final null entry. */
	block_free -= அளவு(__u32);
	block_free -= space_used(h->attrs + h->ibody_மொத்தம்,
				 h->மொத்தம் - h->ibody_மொத்தம்);

	எனில் (ext2fs_has_feature_ea_inode(fs->super) &&
	    மதிப்பு_len > EXT4_XATTR_MIN_LARGE_EA_SIZE(fs->blocksize))
		in_inode = 1;

	ret = xattr_array_update(h, name, new_மதிப்பு, மதிப்பு_len, ibody_free,
				 block_free, old_idx, in_inode);
	எனில் (ret == EXT2_ET_EA_NO_SPACE && !in_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_மதிப்பு, மதிப்பு_len,
			ibody_free, block_free, old_idx, 1 /* in_inode */);
	எனில் (ret)
		செல் out;

write_out:
	ret = ext2fs_xattrs_write(h);
out:
	எனில் (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_மதிப்பு);
	திரும்பு ret;
}

errcode_t ext2fs_xattr_remove(கட்டமைப்பு ext2_xattr_handle *handle,
			      மாறிலி எழுத்து *சாவி)
{
	கட்டமைப்பு ext2_xattr *x;
	கட்டமைப்பு ext2_xattr *end = handle->attrs + handle->மொத்தம்;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	க்கு (x = handle->attrs; x < end; x++) {
		எனில் (strcmp(x->name, சாவி) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->மதிப்பு);
			எனில் (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (end - x - 1)*அளவு(*x));
			memset(end - 1, 0, அளவு(*end));
			எனில் (x < handle->attrs + handle->ibody_மொத்தம்)
				handle->ibody_மொத்தம்--;
			handle->மொத்தம்--;
			திரும்பு ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	திரும்பு 0;
}

errcode_t ext2fs_xattr_remove_all(கட்டமைப்பு ext2_xattr_handle *handle)
{
	கட்டமைப்பு ext2_xattr *x;
	கட்டமைப்பு ext2_xattr *end = handle->attrs + handle->மொத்தம்;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	க்கு (x = handle->attrs; x < end; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->மதிப்பு);
		எனில் (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_மொத்தம் = 0;
	handle->மொத்தம் = 0;
	திரும்பு ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     கட்டமைப்பு ext2_xattr_handle **handle)
{
	கட்டமைப்பு ext2_xattr_handle *h;
	errcode_t err;

	எனில் (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_data(fs->super))
		திரும்பு EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(அளவு(*h), &h);
	எனில் (err)
		திரும்பு err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->capacity = 4;
	err = ext2fs_get_arrayzero(h->capacity, அளவு(கட்டமைப்பு ext2_xattr),
				   &h->attrs);
	எனில் (err) {
		ext2fs_free_mem(&h);
		திரும்பு err;
	}
	h->மொத்தம் = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	திரும்பு 0;
}

errcode_t ext2fs_xattrs_close(கட்டமைப்பு ext2_xattr_handle **handle)
{
	கட்டமைப்பு ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_சாவிகள்(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	திரும்பு 0;
}

errcode_t ext2fs_xattrs_மொத்தம்(கட்டமைப்பு ext2_xattr_handle *handle, size_t *மொத்தம்)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*மொத்தம் = handle->மொத்தம்;
	திரும்பு 0;
}

errcode_t ext2fs_xattrs_flags(கட்டமைப்பு ext2_xattr_handle *handle,
			      unsigned முழு *new_flags, unsigned முழு *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	எனில் (old_flags)
		*old_flags = handle->flags;
	எனில் (new_flags)
		handle->flags = *new_flags;
	திரும்பு 0;
}
