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

#include "config.h"
#include <stdio.h>
#නම් HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <string.h>
#include <time.h>

#include "ext2_fs.h"
#include "ext2_ext_attr.h"
#include "ext4_acl.h"

#include "ext2fsP.h"

static errcode_t read_ea_inode_hash(ext2_filsys fs, ext2_ino_t ino, __u32 *hash)
{
	sa_nracanaa ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	නම් (retval)
		ආපසු retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	ආපසු 0;
}

#define NAME_HASH_SHIFT 5
#define VALUE_HASH_SHIFT 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_entry(sa_nracanaa ext2_ext_attr_entry *entry, void *data)
{
	__u32 hash = 0;
	unsigned var_na *name = (((unsigned var_na *) entry) +
			       parimaa_na(sa_nracanaa ext2_ext_attr_entry));
	පූර්ණ_සංඛ්‍යා n;

	සඳහා (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*parimaa_na(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	නම් (entry->e_value_inum == 0 && entry->e_value_size != 0) {
		__u32 *value = (__u32 *)data;
		සඳහා (n = (entry->e_value_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << VALUE_HASH_SHIFT) ^
			       (hash >> (8*parimaa_na(hash) - VALUE_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*value++);
		}
	}

	ආපසු hash;
}

__u32 ext2fs_ext_attr_hash_entry_signed(sa_nracanaa ext2_ext_attr_entry *entry,
					void *data)
{
	__u32 hash = 0;
	signed var_na *name = (((signed var_na *) entry) +
			     parimaa_na(sa_nracanaa ext2_ext_attr_entry));
	පූර්ණ_සංඛ්‍යා n;

	සඳහා (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*parimaa_na(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	නම් (entry->e_value_inum == 0 && entry->e_value_size != 0) {
		__u32 *value = (__u32 *)data;
		සඳහා (n = (entry->e_value_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << VALUE_HASH_SHIFT) ^
			       (hash >> (8*parimaa_na(hash) - VALUE_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*value++);
		}
	}

	ආපසු hash;
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
				      sa_nracanaa ext2_ext_attr_entry *entry,
				      void *data, __u32 *hash,
				      __u32 *signed_hash)
{
	*hash = ext2fs_ext_attr_hash_entry(entry, data);
	නම් (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_entry_signed(entry, data);

	නම් (entry->e_value_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, entry->e_value_inum,
					    &ea_inode_hash);
		නම් (retval)
			ආපසු retval;

		*hash = (*hash << VALUE_HASH_SHIFT) ^
			(*hash >> (8*parimaa_na(*hash) - VALUE_HASH_SHIFT)) ^
			ea_inode_hash;
		නම් (signed_hash)
			*signed_hash = (*signed_hash << VALUE_HASH_SHIFT) ^
				(*signed_hash >> (8*parimaa_na(*hash) -
						  VALUE_HASH_SHIFT)) ^
				ea_inode_hash;
	}
	ආපසු 0;
}

/*
 * ext2fs_ext_attr_hash_entry2()
 *
 * Compute the hash of an extended attribute.
 * This version of the function supports hashing entries that reference
 * external inodes (ea_inode feature).
 */
errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
				      sa_nracanaa ext2_ext_attr_entry *entry,
				      void *data, __u32 *hash)
{
	ආපසු ext2fs_ext_attr_hash_entry3(fs, entry, data, hash, NULL);
}

#undef NAME_HASH_SHIFT
#undef VALUE_HASH_SHIFT

#define BLOCK_HASH_SHIFT 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
void ext2fs_ext_attr_block_rehash(sa_nracanaa ext2_ext_attr_header *header,
				  sa_nracanaa ext2_ext_attr_entry *end)
{
	sa_nracanaa ext2_ext_attr_entry *here;
	__u32 hash = 0;

	here = (sa_nracanaa ext2_ext_attr_entry *)(header+1);
	අතරතුර (here < end && !EXT2_EXT_IS_LAST_ENTRY(here)) {
		නම් (!here->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			bha_nga;
		}
		hash = (hash << BLOCK_HASH_SHIFT) ^
		       (hash >> (8*parimaa_na(hash) - BLOCK_HASH_SHIFT)) ^
		       here->e_hash;
		here = EXT2_EXT_ATTR_NEXT(here);
	}
	header->h_hash = hash;
}

#undef BLOCK_HASH_SHIFT

__u32 ext2fs_get_ea_inode_hash(sa_nracanaa ext2_inode *inode)
{
	ආපසු inode->i_atime;
}

void ext2fs_set_ea_inode_hash(sa_nracanaa ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(sa_nracanaa ext2_inode *inode)
{
	ආපසු ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_version;
}

void ext2fs_set_ea_inode_ref(sa_nracanaa ext2_inode *inode, __u64 ref_count)
{
	inode->i_ctime = (__u32)(ref_count >> 32);
	inode->osd1.linux1.l_i_version = (__u32)ref_count;
}

static errcode_t check_ext_attr_header(sa_nracanaa ext2_ext_attr_header *header)
{
	නම් ((header->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     header->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    header->h_blocks != 1)
		ආපසු EXT2_ET_BAD_EA_HEADER;

	ආපසු 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block, void *buf,
				ext2_ino_t inum)
{
	පූර්ණ_සංඛ්‍යා		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	නම් (retval)
		ආපසු retval;

	නම් (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_block_csum_verify(fs, inum, block, buf))
		csum_failed = 1;

#ifdef WORDS_BIGENDIAN
	ext2fs_swap_ext_attr(buf, buf, fs->blocksize, 1);
#endif

	retval = check_ext_attr_header(buf);
	නම් (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	ආපසු retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block, void *buf)
{
	ආපසු ext2fs_read_ext_attr3(fs, block, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, void *buf)
{
	ආපසු ext2fs_read_ext_attr2(fs, block, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block, void *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	var_na		*write_buf;

#ifdef WORDS_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	නම් (retval)
		ආපසු retval;
	ext2fs_swap_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#නැතහොත්
	write_buf = (var_na *) inbuf;
#endif

	retval = ext2fs_ext_attr_block_csum_set(fs, inum, block,
			(sa_nracanaa ext2_ext_attr_header *)write_buf);
	නම් (retval)
		ආපසු retval;

	retval = io_channel_write_blk64(fs->io, block, 1, write_buf);
#ifdef WORDS_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#endif
	නම් (!retval)
		ext2fs_mark_changed(fs);
	ආපසු retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block, void *inbuf)
{
	ආපසු ext2fs_write_ext_attr3(fs, block, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block, void *inbuf)
{
	ආපසු ext2fs_write_ext_attr2(fs, block, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    var_na *block_buf, පූර්ණ_සංඛ්‍යා adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	sa_nracanaa ext2_ext_attr_header *header;
	var_na	*buf = 0;

	නම් ((blk >= ext2fs_blocks_count(fs->super)) ||
	    (blk < fs->super->s_first_data_block))
		ආපසු EXT2_ET_BAD_EA_BLOCK_NUM;

	නම් (!block_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		නම් (retval)
			ආපසු retval;
		block_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, block_buf, inum);
	නම් (retval)
		goto errout;

	header = (sa_nracanaa ext2_ext_attr_header *) block_buf;
	header->h_refcount += adjust;
	නම් (newcount)
		*newcount = header->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, block_buf, inum);
	නම් (retval)
		goto errout;

errout:
	නම් (buf)
		ext2fs_free_mem(&buf);
	ආපසු retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    var_na *block_buf, පූර්ණ_සංඛ්‍යා adjust,
				    __u32 *newcount)
{
	ආපසු ext2fs_adjust_ea_refcount3(fs, blk, block_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					var_na *block_buf, පූර්ණ_සංඛ්‍යා adjust,
					__u32 *newcount)
{
	ආපසු ext2fs_adjust_ea_refcount2(fs, blk, block_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
sa_nracanaa ext2_xattr {
	පූර්ණ_සංඛ්‍යා name_index;
	var_na *name;
	var_na *short_name;
	void *value;
	unsigned පූර්ණ_සංඛ්‍යා value_len;
	ext2_ino_t ea_ino;
};

sa_nracanaa ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	sa_nracanaa ext2_xattr *attrs;
	පූර්ණ_සංඛ්‍යා capacity;
	පූර්ණ_සංඛ්‍යා count;
	පූර්ණ_සංඛ්‍යා ibody_count;
	ext2_ino_t ino;
	unsigned පූර්ණ_සංඛ්‍යා flags;
};

static errcode_t ext2fs_xattrs_expand(sa_nracanaa ext2_xattr_handle *h,
				      unsigned පූර්ණ_සංඛ්‍යා expandby)
{
	sa_nracanaa ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->capacity + expandby,
				   parimaa_na(sa_nracanaa ext2_xattr), &new_attrs);
	නම් (err)
		ආපසු err;

	memcpy(new_attrs, h->attrs, h->capacity * parimaa_na(sa_nracanaa ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->capacity += expandby;
	h->attrs = new_attrs;

	ආපසු 0;
}

sa_nracanaa ea_name_index {
	පූර්ණ_සංඛ්‍යා index;
	const var_na *name;
};

/* Keep these names sorted in order of decreasing specificity. */
static sa_nracanaa ea_name_index ea_names[] = {
	{10, "gnu."},
	{3, "system.posix_acl_default"},
	{2, "system.posix_acl_access"},
	{8, "system.richacl"},
	{6, "security."},
	{4, "trusted."},
	{7, "system."},
	{1, "user."},
	{0, NULL},
};

static const var_na *find_ea_prefix(පූර්ණ_සංඛ්‍යා index)
{
	sa_nracanaa ea_name_index *e;

	සඳහා (e = ea_names; e->name; e++)
		නම් (e->index == index)
			ආපසු e->name;

	ආපසු NULL;
}

static පූර්ණ_සංඛ්‍යා find_ea_index(const var_na *fullname, const var_na **name, පූර්ණ_සංඛ්‍යා *index)
{
	sa_nracanaa ea_name_index *e;

	සඳහා (e = ea_names; e->name; e++) {
		නම් (strncmp(fullname, e->name, strlen(e->name)) == 0) {
			*name = fullname + strlen(e->name);
			*index = e->index;
			ආපසු 1;
		}
	}
	ආපසු 0;
}

errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       sa_nracanaa ext2_inode_large *inode)
{
	sa_nracanaa ext2_ext_attr_header *header;
	void *block_buf = NULL;
	blk64_t blk;
	errcode_t err;
	sa_nracanaa ext2_inode_large i;

	/* Read inode? */
	නම් (inode == NULL) {
		err = ext2fs_read_inode_full(fs, ino, (sa_nracanaa ext2_inode *)&i,
					     parimaa_na(sa_nracanaa ext2_inode_large));
		නම් (err)
			ආපසු err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (sa_nracanaa ext2_inode *)inode);
	නම් (blk == 0)
		ආපසු 0;

	/* Find block, zero it, write back */
	නම් ((blk < fs->super->s_first_data_block) ||
	    (blk >= ext2fs_blocks_count(fs->super))) {
		err = EXT2_ET_BAD_EA_BLOCK_NUM;
		goto out;
	}

	err = ext2fs_get_mem(fs->blocksize, &block_buf);
	නම් (err)
		goto out;

	err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
	නම් (err)
		goto out2;

	/* We only know how to deal with v2 EA blocks */
	header = (sa_nracanaa ext2_ext_attr_header *) block_buf;
	නම් (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_HEADER;
		goto out2;
	}

	header->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
	නම් (err)
		goto out2;

	/* Erase link to block */
	ext2fs_file_acl_block_set(fs, (sa_nracanaa ext2_inode *)inode, 0);
	නම් (header->h_refcount == 0)
		ext2fs_block_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_blocks(fs, (sa_nracanaa ext2_inode *)inode, 1);
	නම් (err)
		goto out2;

	/* Write inode? */
	නම් (inode == &i) {
		err = ext2fs_write_inode_full(fs, ino, (sa_nracanaa ext2_inode *)&i,
					      parimaa_na(sa_nracanaa ext2_inode_large));
		නම් (err)
			goto out2;
	}

out2:
	ext2fs_free_mem(&block_buf);
out:
	ආපසු err;
}

static errcode_t prep_ea_block_for_write(ext2_filsys fs, ext2_ino_t ino,
					 sa_nracanaa ext2_inode_large *inode)
{
	sa_nracanaa ext2_ext_attr_header *header;
	void *block_buf = NULL;
	blk64_t blk, goal;
	errcode_t err;

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (sa_nracanaa ext2_inode *)inode);
	නම් (blk != 0) {
		නම් ((blk < fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_count(fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_NUM;
			goto out;
		}

		err = ext2fs_get_mem(fs->blocksize, &block_buf);
		නම් (err)
			goto out;

		err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
		නම් (err)
			goto out2;

		/* We only know how to deal with v2 EA blocks */
		header = (sa_nracanaa ext2_ext_attr_header *) block_buf;
		නම් (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			goto out2;
		}

		/* Single-user block.  We're done here. */
		නම් (header->h_refcount == 1)
			goto out2;

		/* We need to CoW the block. */
		header->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
		නම් (err)
			goto out2;
	} නැතහොත් {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_add_blocks(fs, (sa_nracanaa ext2_inode *)inode,
					     1);
		නම් (err)
			goto out;
	}

	/* Allocate a block */
	goal = ext2fs_find_inode_goal(fs, ino, (sa_nracanaa ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, NULL, &blk);
	නම් (err)
		goto out2;
	ext2fs_file_acl_block_set(fs, (sa_nracanaa ext2_inode *)inode, blk);
out2:
	නම් (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	ආපසු err;
}


static anta_hstha පූර්ණ_සංඛ්‍යා
posix_acl_xattr_count(size_t size)
{
        නම් (size < parimaa_na(posix_acl_xattr_header))
                ආපසු -1;
        size -= parimaa_na(posix_acl_xattr_header);
        නම් (size % parimaa_na(posix_acl_xattr_entry))
                ආපසු -1;
        ආපසු size / parimaa_na(posix_acl_xattr_entry);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
static errcode_t convert_posix_acl_to_disk_buffer(const void *value, size_t size,
						  void *out_buf, size_t *size_out)
{
	const posix_acl_xattr_header *header =
		(const posix_acl_xattr_header*) value;
	const posix_acl_xattr_entry *end, *entry =
		(const posix_acl_xattr_entry *)(header+1);
	ext4_acl_header *ext_acl;
	size_t s;
	var_na *e;

	පූර්ණ_සංඛ්‍යා count;

	නම් (!value)
		ආපසු EINVAL;
	නම් (size < parimaa_na(posix_acl_xattr_header))
		ආපසු ENOMEM;
	නම් (header->a_version != ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION))
		ආපසු EINVAL;

	count = posix_acl_xattr_count(size);
	ext_acl = out_buf;
	ext_acl->a_version = ext2fs_cpu_to_le32(EXT4_ACL_VERSION);

	නම් (count <= 0)
		ආපසු EINVAL;

	e = (var_na *) out_buf + parimaa_na(ext4_acl_header);
	s = parimaa_na(ext4_acl_header);
	සඳහා (end = entry + count; entry != end;entry++) {
		ext4_acl_entry *disk_entry = (ext4_acl_entry*) e;
		disk_entry->e_tag = entry->e_tag;
		disk_entry->e_perm = entry->e_perm;

		switch(ext2fs_le16_to_cpu(entry->e_tag)) {
			pak_sa ACL_USER_OBJ:
			pak_sa ACL_GROUP_OBJ:
			pak_sa ACL_MASK:
			pak_sa ACL_OTHER:
				e += parimaa_na(ext4_acl_entry_short);
				s += parimaa_na(ext4_acl_entry_short);
				bha_nga;
			pak_sa ACL_USER:
			pak_sa ACL_GROUP:
				disk_entry->e_id = entry->e_id;
				e += parimaa_na(ext4_acl_entry);
				s += parimaa_na(ext4_acl_entry);
				bha_nga;
			default:
				ආපසු EINVAL;
		}
	}
	*size_out = s;
	ආපසු 0;
}

static errcode_t convert_disk_buffer_to_posix_acl(const void *value, size_t size,
						  void **out_buf, size_t *size_out)
{
	posix_acl_xattr_header *header;
	posix_acl_xattr_entry *entry;
	const ext4_acl_header *ext_acl = (const ext4_acl_header *) value;
	errcode_t err;
	const var_na *cp;
	var_na *out;

	නම් ((!value) ||
	    (size < parimaa_na(ext4_acl_header)) ||
	    (ext_acl->a_version != ext2fs_cpu_to_le32(EXT4_ACL_VERSION)))
		ආපසු EINVAL;

	err = ext2fs_get_mem(size * 2, &out);
	නම් (err)
		ආපසු err;

	header = (posix_acl_xattr_header *) out;
	header->a_version = ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION);
	entry = (posix_acl_xattr_entry *) (out + parimaa_na(posix_acl_xattr_header));

	cp = (const var_na *) value + parimaa_na(ext4_acl_header);
	size -= parimaa_na(ext4_acl_header);

	අතරතුර (size > 0) {
		const ext4_acl_entry *disk_entry = (const ext4_acl_entry *) cp;

		entry->e_tag = disk_entry->e_tag;
		entry->e_perm = disk_entry->e_perm;

		switch(ext2fs_le16_to_cpu(entry->e_tag)) {
			pak_sa ACL_USER_OBJ:
			pak_sa ACL_GROUP_OBJ:
			pak_sa ACL_MASK:
			pak_sa ACL_OTHER:
				entry->e_id = 0;
				cp += parimaa_na(ext4_acl_entry_short);
				size -= parimaa_na(ext4_acl_entry_short);
				bha_nga;
			pak_sa ACL_USER:
			pak_sa ACL_GROUP:
				entry->e_id = disk_entry->e_id;
				cp += parimaa_na(ext4_acl_entry);
				size -= parimaa_na(ext4_acl_entry);
				bha_nga;
			default:
				ext2fs_free_mem(&out);
				ආපසු EINVAL;
		}
		entry++;
	}
	*out_buf = out;
	*size_out = ((var_na *) entry - out);
	ආපසු 0;
}

static errcode_t
write_xattrs_to_buffer(ext2_filsys fs, sa_nracanaa ext2_xattr *attrs, පූර්ණ_සංඛ්‍යා count,
		       void *entries_start, unsigned පූර්ණ_සංඛ්‍යා storage_size,
		       unsigned පූර්ණ_සංඛ්‍යා value_offset_correction, පූර්ණ_සංඛ්‍යා write_hash)
{
	sa_nracanaa ext2_xattr *x;
	sa_nracanaa ext2_ext_attr_entry *e = entries_start;
	var_na *end = (var_na *) entries_start + storage_size;
	unsigned පූර්ණ_සංඛ්‍යා value_size;
	errcode_t err;

	memset(entries_start, 0, storage_size);
	සඳහා (x = attrs; x < attrs + count; x++) {
		value_size = ((x->value_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = strlen(x->short_name);
		e->e_name_index = x->name_index;

		e->e_value_size = x->value_len;
		e->e_value_inum = x->ea_ino;

		/* Store name */
		memcpy((var_na *)e + parimaa_na(*e), x->short_name, e->e_name_len);
		නම් (x->ea_ino) {
			e->e_value_offs = 0;
		} නැතහොත් {
			end -= value_size;
			e->e_value_offs = end - (var_na *) entries_start +
						value_offset_correction;
			memcpy(end, x->value, e->e_value_size);
		}

		නම් (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : end,
							  &e->e_hash);
			නම් (err)
				ආපසු err;
		} නැතහොත්
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_NEXT(e);
		*(__u32 *)e = 0;
	}
	ආපසු 0;
}

errcode_t ext2fs_xattrs_write(sa_nracanaa ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	const unsigned පූර්ණ_සංඛ්‍යා inode_size = EXT2_INODE_SIZE(fs->super);
	sa_nracanaa ext2_inode_large *inode;
	var_na *start, *block_buf = NULL;
	sa_nracanaa ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	blk64_t blk;
	unsigned පූර්ණ_සංඛ්‍යා storage_size;
	unsigned පූර්ණ_සංඛ්‍යා i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_size;
	නම් (i < parimaa_na(*inode))
		i = parimaa_na(*inode);
	err = ext2fs_get_memzero(i, &inode);
	නම් (err)
		ආපසු err;

	err = ext2fs_read_inode_full(fs, handle->ino, EXT2_INODE(inode),
				     inode_size);
	නම් (err)
		goto out;

	/* If extra_isize isn't set, we need to set it now */
	නම් (inode->i_extra_isize == 0 &&
	    inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		var_na *p = (var_na *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		නම් (extra == 0)
			extra = parimaa_na(__u32);
		memset(p + EXT2_GOOD_OLD_INODE_SIZE, 0, extra);
		inode->i_extra_isize = extra;
	}
	නම් (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		goto out;
	}

	/* Does the inode have space for EA? */
	නම් (inode->i_extra_isize < parimaa_na(inode->i_extra_isize) ||
	    inode_size <= EXT2_GOOD_OLD_INODE_SIZE + inode->i_extra_isize +
								parimaa_na(__u32))
		goto write_ea_block;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	memcpy(((var_na *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, &ea_inode_magic, parimaa_na(__u32));
	storage_size = inode_size - EXT2_GOOD_OLD_INODE_SIZE -
				inode->i_extra_isize - parimaa_na(__u32);
	start = ((var_na *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
				inode->i_extra_isize + parimaa_na(__u32);

	err = write_xattrs_to_buffer(fs, handle->attrs, handle->ibody_count,
				     start, storage_size, 0, 0);
	නම් (err)
		goto out;
write_ea_block:
	/* Are we done? */
	නම් (handle->ibody_count == handle->count &&
	    !ext2fs_file_acl_block(fs, EXT2_INODE(inode)))
		goto skip_ea_block;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &block_buf);
	නම් (err)
		goto out;

	storage_size = fs->blocksize - parimaa_na(sa_nracanaa ext2_ext_attr_header);
	start = block_buf + parimaa_na(sa_nracanaa ext2_ext_attr_header);

	err = write_xattrs_to_buffer(fs, handle->attrs + handle->ibody_count,
				     handle->count - handle->ibody_count, start,
				     storage_size, start - block_buf, 1);
	නම් (err)
		goto out2;

	/* Write a header on the EA block */
	header = (sa_nracanaa ext2_ext_attr_header *) block_buf;
	header->h_magic = EXT2_EXT_ATTR_MAGIC;
	header->h_refcount = 1;
	header->h_blocks = 1;

	/* Get a new block for writing */
	err = prep_ea_block_for_write(fs, handle->ino, inode);
	නම් (err)
		goto out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_block(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, handle->ino);
	නම් (err)
		goto out2;

skip_ea_block:
	blk = ext2fs_file_acl_block(fs, (sa_nracanaa ext2_inode *)inode);
	නම් (!block_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		නම් (err)
			goto out;
	}

	/* Write the inode */
	err = ext2fs_write_inode_full(fs, handle->ino, EXT2_INODE(inode),
				      inode_size);
	නම් (err)
		goto out2;

out2:
	ext2fs_free_mem(&block_buf);
out:
	ext2fs_free_mem(&inode);
	ආපසු err;
}

static errcode_t read_xattrs_from_buffer(sa_nracanaa ext2_xattr_handle *handle,
					 sa_nracanaa ext2_inode_large *inode,
					 sa_nracanaa ext2_ext_attr_entry *entries,
					 unsigned පූර්ණ_සංඛ්‍යා storage_size,
					 var_na *value_start)
{
	sa_nracanaa ext2_xattr *x;
	sa_nracanaa ext2_ext_attr_entry *entry, *end;
	const var_na *prefix;
	unsigned පූර්ණ_සංඛ්‍යා remain, prefix_len;
	errcode_t err;
	unsigned පූර්ණ_සංඛ්‍යා values_size = storage_size +
			((var_na *)entries - value_start);

	/* find the end */
	end = entries;
	remain = storage_size;
	අතරතුර (remain >= parimaa_na(sa_nracanaa ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(end)) {

		/* header eats this space */
		remain -= parimaa_na(sa_nracanaa ext2_ext_attr_entry);

		/* is attribute name valid? */
		නම් (EXT2_EXT_ATTR_SIZE(end->e_name_len) > remain)
			ආපසු EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(end->e_name_len);
		end = EXT2_EXT_ATTR_NEXT(end);
	}

	entry = entries;
	remain = storage_size;
	අතරතුර (remain >= parimaa_na(sa_nracanaa ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(entry)) {

		/* Allocate space for more attrs? */
		නම් (handle->count == handle->capacity) {
			err = ext2fs_xattrs_expand(handle, 4);
			නම් (err)
				ආපසු err;
		}

		x = handle->attrs + handle->count;

		/* header eats this space */
		remain -= parimaa_na(sa_nracanaa ext2_ext_attr_entry);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(entry->e_name_len);

		/* Extract name */
		prefix = find_ea_prefix(entry->e_name_index);
		prefix_len = (prefix ? strlen(prefix) : 0);
		err = ext2fs_get_memzero(entry->e_name_len + prefix_len + 1,
					 &x->name);
		නම් (err)
			ආපසු err;
		නම් (prefix)
			memcpy(x->name, prefix, prefix_len);
		නම් (entry->e_name_len)
			memcpy(x->name + prefix_len,
			       (var_na *)entry + parimaa_na(*entry),
			       entry->e_name_len);
		x->short_name = x->name + prefix_len;
		x->name_index = entry->e_name_index;

		/* Check & copy value */
		නම් (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    entry->e_value_inum != 0)
			ආපසු EXT2_ET_BAD_EA_BLOCK_NUM;

		නම් (entry->e_value_inum == 0) {
			නම් (entry->e_value_size > remain)
				ආපසු EXT2_ET_EA_BAD_VALUE_SIZE;

			නම් (entry->e_value_offs + entry->e_value_size > values_size)
				ආපසු EXT2_ET_EA_BAD_VALUE_OFFSET;

			නම් (entry->e_value_size > 0 &&
			    value_start + entry->e_value_offs <
			    (var_na *)end + parimaa_na(__u32))
				ආපසු EXT2_ET_EA_BAD_VALUE_OFFSET;

			remain -= entry->e_value_size;

			err = ext2fs_get_mem(entry->e_value_size, &x->value);
			නම් (err)
				ආපසු err;
			memcpy(x->value, value_start + entry->e_value_offs,
			       entry->e_value_size);
		} නැතහොත් {
			sa_nracanaa ext2_inode *ea_inode;
			ext2_file_t ea_file;

			නම් (entry->e_value_offs != 0)
				ආපසු EXT2_ET_EA_BAD_VALUE_OFFSET;

			නම් (entry->e_value_size > (64 * 1024))
				ආපසු EXT2_ET_EA_BAD_VALUE_SIZE;

			err = ext2fs_get_mem(entry->e_value_size, &x->value);
			නම් (err)
				ආපසු err;

			err = ext2fs_file_open(handle->fs, entry->e_value_inum,
					       0, &ea_file);
			නම් (err)
				ආපසු err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			නම් ((ea_inode->i_flags & EXT4_INLINE_DATA_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_count == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			නැතහොත් නම් ((__u64) ext2fs_file_get_size(ea_file) !=
				 entry->e_value_size)
				err = EXT2_ET_EA_BAD_VALUE_SIZE;
			නැතහොත්
				err = ext2fs_file_read(ea_file, x->value,
						       entry->e_value_size, 0);
			ext2fs_file_close(ea_file);
			නම් (err)
				ආපසු err;
		}

		x->ea_ino = entry->e_value_inum;
		x->value_len = entry->e_value_size;

		/* e_hash may be 0 in older inode's ea */
		නම් (entry->e_hash != 0) {
			__u32 hash, signed_hash;

			void *data = (entry->e_value_inum != 0) ?
					0 : value_start + entry->e_value_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, entry,
							  data, &hash,
							  &signed_hash);
			නම් (err)
				ආපසු err;
			නම් ((entry->e_hash != hash) &&
			    (entry->e_hash != signed_hash)) {
				sa_nracanaa ext2_inode child;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							entry->e_value_inum,
							&child);
				නම් (err)
					ආපසු err;
				නම් (child.i_mtime != handle->ino ||
				    child.i_generation != inode->i_generation)
					ආපසු EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->count++;
		entry = EXT2_EXT_ATTR_NEXT(entry);
	}

	ආපසු 0;
}

static void xattrs_free_keys(sa_nracanaa ext2_xattr_handle *h)
{
	sa_nracanaa ext2_xattr *a = h->attrs;
	පූර්ණ_සංඛ්‍යා i;

	සඳහා (i = 0; i < h->capacity; i++) {
		නම් (a[i].name)
			ext2fs_free_mem(&a[i].name);
		නම් (a[i].value)
			ext2fs_free_mem(&a[i].value);
	}
	h->count = 0;
	h->ibody_count = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(sa_nracanaa ext2_xattr_handle *handle,
				   sa_nracanaa ext2_inode_large *inode)
{
	sa_nracanaa ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	unsigned පූර්ණ_සංඛ්‍යා storage_size;
	var_na *start, *block_buf = NULL;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_keys(handle);

	/* Does the inode have space for EA? */
	නම් (inode->i_extra_isize < parimaa_na(inode->i_extra_isize) ||
	    EXT2_INODE_SIZE(handle->fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  parimaa_na(__u32))
		goto read_ea_block;
	නම් (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		goto out;
	}

	/* Look for EA in the inode */
	memcpy(&ea_inode_magic, ((var_na *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, parimaa_na(__u32));
	නම් (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_size = EXT2_INODE_SIZE(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_SIZE - inode->i_extra_isize -
			parimaa_na(__u32);
		start = ((var_na *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + parimaa_na(__u32);

		err = read_xattrs_from_buffer(handle, inode,
					(sa_nracanaa ext2_ext_attr_entry *) start,
					storage_size, start);
		නම් (err)
			goto out;

		handle->ibody_count = handle->count;
	}

read_ea_block:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_block(handle->fs, EXT2_INODE(inode));
	නම් (blk != 0) {
		නම් ((blk < handle->fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_count(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_NUM;
			goto out;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &block_buf);
		නම් (err)
			goto out;

		err = ext2fs_read_ext_attr3(handle->fs, blk, block_buf,
					    handle->ino);
		නම් (err)
			goto out3;

		/* We only know how to deal with v2 EA blocks */
		header = (sa_nracanaa ext2_ext_attr_header *) block_buf;
		නම් (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			goto out3;
		}

		/* Read EAs */
		storage_size = handle->fs->blocksize -
			parimaa_na(sa_nracanaa ext2_ext_attr_header);
		start = block_buf + parimaa_na(sa_nracanaa ext2_ext_attr_header);
		err = read_xattrs_from_buffer(handle, inode,
					(sa_nracanaa ext2_ext_attr_entry *) start,
					storage_size, block_buf);
	}

out3:
	නම් (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	ආපසු err;
}

errcode_t ext2fs_xattrs_read(sa_nracanaa ext2_xattr_handle *handle)
{
	sa_nracanaa ext2_inode_large *inode;
	size_t inode_size = EXT2_INODE_SIZE(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	නම් (inode_size < parimaa_na(*inode))
		inode_size = parimaa_na(*inode);
	err = ext2fs_get_memzero(inode_size, &inode);
	නම් (err)
		ආපසු err;

	err = ext2fs_read_inode_full(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_SIZE(handle->fs->super));
	නම් (err)
		goto out;

	err = ext2fs_xattrs_read_inode(handle, inode);

out:
	ext2fs_free_mem(&inode);

	ආපසු err;
}

errcode_t ext2fs_xattrs_iterate(sa_nracanaa ext2_xattr_handle *h,
				පූර්ණ_සංඛ්‍යා (*func)(var_na *name, var_na *value,
					    size_t value_len, void *data),
				void *data)
{
	sa_nracanaa ext2_xattr *x;
	පූර්ණ_සංඛ්‍යා dirty = 0;
	පූර්ණ_සංඛ්‍යා ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	සඳහා (x = h->attrs; x < h->attrs + h->count; x++) {
		ret = func(x->name, x->value, x->value_len, data);
		නම් (ret & XATTR_CHANGED)
			dirty = 1;
		නම් (ret & XATTR_ABORT)
			bha_nga;
	}

	නම් (dirty)
		ආපසු ext2fs_xattrs_write(h);
	ආපසු 0;
}

errcode_t ext2fs_xattr_get(sa_nracanaa ext2_xattr_handle *h, const var_na *key,
			   void **value, size_t *value_len)
{
	sa_nracanaa ext2_xattr *x;
	var_na *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	සඳහා (x = h->attrs; x < h->attrs + h->count; x++) {
		නම් (strcmp(x->name, key))
			continue;

		නම් (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(key, "system.posix_acl_default") == 0) ||
		     (strcmp(key, "system.posix_acl_access") == 0))) {
			err = convert_disk_buffer_to_posix_acl(x->value, x->value_len,
							       value, value_len);
			ආපසු err;
		} නැතහොත් {
			err = ext2fs_get_mem(x->value_len, &val);
			නම් (err)
				ආපසු err;
			memcpy(val, x->value, x->value_len);
			*value = val;
			*value_len = x->value_len;
			ආපසු 0;
		}
	}

	ආපසු EXT2_ET_EA_KEY_NOT_FOUND;
}

errcode_t ext2fs_xattr_inode_max_size(ext2_filsys fs, ext2_ino_t ino,
				      size_t *size)
{
	sa_nracanaa ext2_ext_attr_entry *entry;
	sa_nracanaa ext2_inode_large *inode;
	__u32 ea_inode_magic;
	unsigned පූර්ණ_සංඛ්‍යා minoff;
	var_na *start;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_SIZE(fs->super);
	නම් (i < parimaa_na(*inode))
		i = parimaa_na(*inode);
	err = ext2fs_get_memzero(i, &inode);
	නම් (err)
		ආපසු err;

	err = ext2fs_read_inode_full(fs, ino, (sa_nracanaa ext2_inode *)inode,
				     EXT2_INODE_SIZE(fs->super));
	නම් (err)
		goto out;

	/* Does the inode have size for EA? */
	නම් (EXT2_INODE_SIZE(fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  parimaa_na(__u32)) {
		err = EXT2_ET_INLINE_DATA_NO_SPACE;
		goto out;
	}

	minoff = EXT2_INODE_SIZE(fs->super) - parimaa_na(*inode) - parimaa_na(__u32);
	memcpy(&ea_inode_magic, ((var_na *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, parimaa_na(__u32));
	නම් (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		start= ((var_na *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + parimaa_na(__u32);
		entry = (sa_nracanaa ext2_ext_attr_entry *) start;
		අතරතුර (!EXT2_EXT_IS_LAST_ENTRY(entry)) {
			නම් (!entry->e_value_inum && entry->e_value_size) {
				unsigned පූර්ණ_සංඛ්‍යා offs = entry->e_value_offs;
				නම් (offs < minoff)
					minoff = offs;
			}
			entry = EXT2_EXT_ATTR_NEXT(entry);
		}
		*size = minoff - ((var_na *)entry - (var_na *)start) - parimaa_na(__u32);
	} නැතහොත් {
		/* no xattr.  return a maximum size */
		*size = EXT2_EXT_ATTR_SIZE(minoff -
					   EXT2_EXT_ATTR_LEN(strlen("data")) -
					   EXT2_EXT_ATTR_ROUND - parimaa_na(__u32));
	}

out:
	ext2fs_free_mem(&inode);
	ආපසු err;
}

static errcode_t xattr_create_ea_inode(ext2_filsys fs, const void *value,
				       size_t value_len, ext2_ino_t *ea_ino)
{
	sa_nracanaa ext2_inode inode;
	ext2_ino_t ino;
	ext2_file_t file;
	__u32 hash;
	errcode_t ret;

	ret = ext2fs_new_inode(fs, 0, 0, 0, &ino);
	නම් (ret)
		ආපසු ret;

	memset(&inode, 0, parimaa_na(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	නම් (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_size = 0;
	inode.i_mode = LINUX_S_IFREG | 0600;
	inode.i_links_count = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	නම් (ret)
		ආපසු ret;
	/*
	 * ref_count and hash utilize inode's i_*time fields.
	 * ext2fs_write_new_inode() call above initializes these fields with
	 * current time. That's why ref count and hash updates are done
	 * separately below.
	 */
	ext2fs_set_ea_inode_ref(&inode, 1);
	hash = ext2fs_crc32c_le(fs->csum_seed, value, value_len);
	ext2fs_set_ea_inode_hash(&inode, hash);

	ret = ext2fs_write_inode(fs, ino, &inode);
	නම් (ret)
		ආපසු ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	නම් (ret)
		ආපසු ret;
	ret = ext2fs_file_write(file, value, value_len, NULL);
	ext2fs_file_close(file);
	නම් (ret)
		ආපසු ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	ආපසු 0;
}

static errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	sa_nracanaa ext2_inode_large inode;
	__u64 ref_count;
	errcode_t ret;

	ret = ext2fs_read_inode_full(fs, ino, (sa_nracanaa ext2_inode *)&inode,
				     parimaa_na(inode));
	නම් (ret)
		goto out;

	ref_count = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_count--;
	ext2fs_set_ea_inode_ref(EXT2_INODE(&inode), ref_count);

	නම් (ref_count)
		goto write_out;

	inode.i_links_count = 0;
	ext2fs_set_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	නම් (ret)
		goto write_out;

	නම් (ext2fs_inode_has_valid_blocks2(fs, (sa_nracanaa ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (sa_nracanaa ext2_inode *)&inode, NULL,
				   0, ~0ULL);
		නම් (ret)
			goto out;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_out:
	ret = ext2fs_write_inode_full(fs, ino, (sa_nracanaa ext2_inode *)&inode,
				      parimaa_na(inode));
out:
	ආපසු ret;
}

static errcode_t xattr_update_entry(ext2_filsys fs, sa_nracanaa ext2_xattr *x,
				    const var_na *name, const var_na *short_name,
				    පූර්ණ_සංඛ්‍යා index, const void *value,
				    size_t value_len, පූර්ණ_සංඛ්‍යා in_inode)
{
	ext2_ino_t ea_ino = 0;
	void *new_value = NULL;
	var_na *new_name = NULL;
	පූර්ණ_සංඛ්‍යා name_len;
	errcode_t ret;

	නම් (!x->name) {
		name_len = strlen(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		නම් (ret)
			goto fail;
		memcpy(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(value_len, &new_value);
	නම් (ret)
		goto fail;
	memcpy(new_value, value, value_len);

	නම් (in_inode) {
		ret = xattr_create_ea_inode(fs, value, value_len, &ea_ino);
		නම් (ret)
			goto fail;
	}

	නම් (x->ea_ino) {
		ret = xattr_inode_dec_ref(fs, x->ea_ino);
		නම් (ret)
			goto fail;
	}

	නම් (!x->name) {
		x->name = new_name;
		x->short_name = new_name + (short_name  - name);
	}
	x->name_index = index;

	නම් (x->value)
		ext2fs_free_mem(&x->value);
	x->value = new_value;
	x->value_len = value_len;
	x->ea_ino = ea_ino;
	ආපසු 0;
fail:
	නම් (new_name)
		ext2fs_free_mem(&new_name);
	නම් (new_value)
		ext2fs_free_mem(&new_value);
	නම් (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	ආපසු ret;
}

static පූර්ණ_සංඛ්‍යා xattr_find_position(sa_nracanaa ext2_xattr *attrs, පූර්ණ_සංඛ්‍යා count,
			       const var_na *shortname, පූර්ණ_සංඛ්‍යා name_idx)
{
	sa_nracanaa ext2_xattr *x;
	පූර්ණ_සංඛ්‍යා i;
	පූර්ණ_සංඛ්‍යා shortname_len, x_shortname_len;

	shortname_len = strlen(shortname);

	සඳහා (i = 0, x = attrs; i < count; i++, x++) {
		නම් (name_idx < x->name_index)
			bha_nga;
		නම් (name_idx > x->name_index)
			continue;

		x_shortname_len = strlen(x->short_name);
		නම් (shortname_len < x_shortname_len)
			bha_nga;
		නම් (shortname_len > x_shortname_len)
			continue;

		නම් (memcmp(shortname, x->short_name, shortname_len) <= 0)
			bha_nga;
	}
	ආපසු i;
}

static errcode_t xattr_array_update(sa_nracanaa ext2_xattr_handle *h,
				    const var_na *name,
				    const void *value, size_t value_len,
				    පූර්ණ_සංඛ්‍යා ibody_free, පූර්ණ_සංඛ්‍යා block_free,
				    පූර්ණ_සංඛ්‍යා old_idx, පූර්ණ_සංඛ්‍යා in_inode)
{
	sa_nracanaa ext2_xattr tmp;
	පූර්ණ_සංඛ්‍යා add_to_ibody;
	පූර්ණ_සංඛ්‍යා needed;
	පූර්ණ_සංඛ්‍යා name_len, name_idx = 0;
	const var_na *shortname = name;
	පූර්ණ_සංඛ්‍යා new_idx;
	පූර්ණ_සංඛ්‍යා ret;

	find_ea_index(name, &shortname, &name_idx);
	name_len = strlen(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	නම් (!in_inode)
		needed += EXT2_EXT_ATTR_SIZE(value_len);

	නම් (old_idx >= 0 && old_idx < h->ibody_count) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		නම් (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_SIZE(
						h->attrs[old_idx].value_len);
	}

	නම් (needed <= ibody_free) {
		නම් (old_idx < 0) {
			new_idx = h->ibody_count;
			add_to_ibody = 1;
			goto add_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, value,
					 value_len, in_inode);
		නම් (ret)
			ආපසු ret;
		නම් (h->ibody_count <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_count + 1,
				h->attrs + h->ibody_count,
				(old_idx - h->ibody_count) * parimaa_na(*h->attrs));
			h->attrs[h->ibody_count] = tmp;
			h->ibody_count++;
		}
		ආපසු 0;
	}

	නම් (h->ibody_count <= old_idx) {
		block_free += EXT2_EXT_ATTR_LEN(name_len);
		නම් (!h->attrs[old_idx].ea_ino)
			block_free +=
				EXT2_EXT_ATTR_SIZE(h->attrs[old_idx].value_len);
	}

	නම් (needed > block_free)
		ආපසු EXT2_ET_EA_NO_SPACE;

	නම් (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, value,
					 value_len, in_inode);
		නම් (ret)
			ආපසු ret;
		නම් (old_idx < h->ibody_count) {
			/*
			 * Move entry from ibody to the block. Note that
			 * entries in the block are sorted.
			 */
			new_idx = xattr_find_position(h->attrs + h->ibody_count,
						      h->count - h->ibody_count,
						      shortname, name_idx);
			new_idx += h->ibody_count - 1;
			tmp = h->attrs[old_idx];
			memmove(h->attrs + old_idx, h->attrs + old_idx + 1,
				(new_idx - old_idx) * parimaa_na(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_count--;
		}
		ආපසු 0;
	}

	new_idx = xattr_find_position(h->attrs + h->ibody_count,
				      h->count - h->ibody_count,
				      shortname, name_idx);
	new_idx += h->ibody_count;
	add_to_ibody = 0;

add_new:
	නම් (h->count == h->capacity) {
		ret = ext2fs_xattrs_expand(h, 4);
		නම් (ret)
			ආපසු ret;
	}

	ret = xattr_update_entry(h->fs, &h->attrs[h->count], name, shortname,
				 name_idx, value, value_len, in_inode);
	නම් (ret)
		ආපසු ret;

	tmp = h->attrs[h->count];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->count - new_idx)*parimaa_na(*h->attrs));
	h->attrs[new_idx] = tmp;
	නම් (add_to_ibody)
		h->ibody_count++;
	h->count++;
	ආපසු 0;
}

static පූර්ණ_සංඛ්‍යා space_used(sa_nracanaa ext2_xattr *attrs, පූර්ණ_සංඛ්‍යා count)
{
	පූර්ණ_සංඛ්‍යා total = 0;
	sa_nracanaa ext2_xattr *x;
	පූර්ණ_සංඛ්‍යා i, len;

	සඳහා (i = 0, x = attrs; i < count; i++, x++) {
		len = strlen(x->short_name);
		total += EXT2_EXT_ATTR_LEN(len);
		නම් (!x->ea_ino)
			total += EXT2_EXT_ATTR_SIZE(x->value_len);
	}
	ආපසු total;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#define EXT4_XATTR_MIN_LARGE_EA_SIZE(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - parimaa_na(sa_nracanaa ext2_ext_attr_header) - 4)

errcode_t ext2fs_xattr_set(sa_nracanaa ext2_xattr_handle *h,
			   const var_na *name,
			   const void *value,
			   size_t value_len)
{
	ext2_filsys fs = h->fs;
	const පූර්ණ_සංඛ්‍යා inode_size = EXT2_INODE_SIZE(fs->super);
	sa_nracanaa ext2_inode_large *inode = NULL;
	sa_nracanaa ext2_xattr *x;
	var_na *new_value;
	පූර්ණ_සංඛ්‍යා ibody_free, block_free;
	පූර්ණ_සංඛ්‍යා in_inode = 0;
	පූර්ණ_සංඛ්‍යා old_idx = -1;
	පූර්ණ_සංඛ්‍යා extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(value_len, &new_value);
	නම් (ret)
		ආපසු ret;
	නම් (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((strcmp(name, "system.posix_acl_default") == 0) ||
	     (strcmp(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_to_disk_buffer(value, value_len,
						       new_value, &value_len);
		නම් (ret)
			goto out;
	} නැතහොත් නම් (value_len)
		memcpy(new_value, value, value_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	සඳහා (x = h->attrs; x < h->attrs + h->count; x++) {
		නම් (!strcmp(x->name, name)) {
			නම් (!x->ea_ino && x->value_len == value_len &&
			    (!value_len ||
			     !memcmp(x->value, new_value, value_len))) {
				ret = 0;
				goto out;
			}
			old_idx = x - h->attrs;
			bha_nga;
		}
	}

	ret = ext2fs_get_memzero(inode_size, &inode);
	නම් (ret)
		goto out;
	ret = ext2fs_read_inode_full(fs, h->ino,
				     (sa_nracanaa ext2_inode *)inode,
				     inode_size);
	නම් (ret)
		goto out;
	නම් (inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		extra_isize = inode->i_extra_isize;
		නම් (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			නම් (extra_isize == 0)
				extra_isize = parimaa_na(__u32);
		}
		ibody_free = inode_size - EXT2_GOOD_OLD_INODE_SIZE;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= parimaa_na(__u32) * 2;
		ibody_free -= space_used(h->attrs, h->ibody_count);
	} නැතහොත්
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	නම් (strcmp(name, "system.data") == 0) {
		නම් (h->ibody_count <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			goto out;
		}
		ret = xattr_array_update(h, name, new_value, value_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		නම් (ret)
			goto out;
		goto write_out;
	}

	block_free = fs->blocksize;
	block_free -= parimaa_na(sa_nracanaa ext2_ext_attr_header);
	/* Final null entry. */
	block_free -= parimaa_na(__u32);
	block_free -= space_used(h->attrs + h->ibody_count,
				 h->count - h->ibody_count);

	නම් (ext2fs_has_feature_ea_inode(fs->super) &&
	    value_len > EXT4_XATTR_MIN_LARGE_EA_SIZE(fs->blocksize))
		in_inode = 1;

	ret = xattr_array_update(h, name, new_value, value_len, ibody_free,
				 block_free, old_idx, in_inode);
	නම් (ret == EXT2_ET_EA_NO_SPACE && !in_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_value, value_len,
			ibody_free, block_free, old_idx, 1 /* in_inode */);
	නම් (ret)
		goto out;

write_out:
	ret = ext2fs_xattrs_write(h);
out:
	නම් (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_value);
	ආපසු ret;
}

errcode_t ext2fs_xattr_remove(sa_nracanaa ext2_xattr_handle *handle,
			      const var_na *key)
{
	sa_nracanaa ext2_xattr *x;
	sa_nracanaa ext2_xattr *end = handle->attrs + handle->count;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	සඳහා (x = handle->attrs; x < end; x++) {
		නම් (strcmp(x->name, key) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->value);
			නම් (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (end - x - 1)*parimaa_na(*x));
			memset(end - 1, 0, parimaa_na(*end));
			නම් (x < handle->attrs + handle->ibody_count)
				handle->ibody_count--;
			handle->count--;
			ආපසු ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	ආපසු 0;
}

errcode_t ext2fs_xattr_remove_all(sa_nracanaa ext2_xattr_handle *handle)
{
	sa_nracanaa ext2_xattr *x;
	sa_nracanaa ext2_xattr *end = handle->attrs + handle->count;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	සඳහා (x = handle->attrs; x < end; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->value);
		නම් (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_count = 0;
	handle->count = 0;
	ආපසු ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     sa_nracanaa ext2_xattr_handle **handle)
{
	sa_nracanaa ext2_xattr_handle *h;
	errcode_t err;

	නම් (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_data(fs->super))
		ආපසු EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(parimaa_na(*h), &h);
	නම් (err)
		ආපසු err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->capacity = 4;
	err = ext2fs_get_arrayzero(h->capacity, parimaa_na(sa_nracanaa ext2_xattr),
				   &h->attrs);
	නම් (err) {
		ext2fs_free_mem(&h);
		ආපසු err;
	}
	h->count = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	ආපසු 0;
}

errcode_t ext2fs_xattrs_close(sa_nracanaa ext2_xattr_handle **handle)
{
	sa_nracanaa ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_keys(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	ආපසු 0;
}

errcode_t ext2fs_xattrs_count(sa_nracanaa ext2_xattr_handle *handle, size_t *count)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*count = handle->count;
	ආපසු 0;
}

errcode_t ext2fs_xattrs_flags(sa_nracanaa ext2_xattr_handle *handle,
			      unsigned පූර්ණ_සංඛ්‍යා *new_flags, unsigned පූර්ණ_සංඛ්‍යා *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	නම් (old_flags)
		*old_flags = handle->flags;
	නම් (new_flags)
		handle->flags = *new_flags;
	ආපසු 0;
}
