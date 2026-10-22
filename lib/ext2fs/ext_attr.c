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
#ако HAVE_UNISTD_H
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
	struct ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	ако (retval)
		върни retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	върни 0;
}

#define NAME_HASH_SHIFT 5
#define VALUE_HASH_SHIFT 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_entry(struct ext2_ext_attr_entry *entry, void *data)
{
	__u32 hash = 0;
	unsigned char *name = (((unsigned char *) entry) +
			       sizeof(struct ext2_ext_attr_entry));
	цяло n;

	за (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*sizeof(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	ако (entry->e_value_inum == 0 && entry->e_value_size != 0) {
		__u32 *value = (__u32 *)data;
		за (n = (entry->e_value_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << VALUE_HASH_SHIFT) ^
			       (hash >> (8*sizeof(hash) - VALUE_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*value++);
		}
	}

	върни hash;
}

__u32 ext2fs_ext_attr_hash_entry_signed(struct ext2_ext_attr_entry *entry,
					void *data)
{
	__u32 hash = 0;
	signed char *name = (((signed char *) entry) +
			     sizeof(struct ext2_ext_attr_entry));
	цяло n;

	за (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*sizeof(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	ако (entry->e_value_inum == 0 && entry->e_value_size != 0) {
		__u32 *value = (__u32 *)data;
		за (n = (entry->e_value_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << VALUE_HASH_SHIFT) ^
			       (hash >> (8*sizeof(hash) - VALUE_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*value++);
		}
	}

	върни hash;
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
				      struct ext2_ext_attr_entry *entry,
				      void *data, __u32 *hash,
				      __u32 *signed_hash)
{
	*hash = ext2fs_ext_attr_hash_entry(entry, data);
	ако (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_entry_signed(entry, data);

	ако (entry->e_value_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, entry->e_value_inum,
					    &ea_inode_hash);
		ако (retval)
			върни retval;

		*hash = (*hash << VALUE_HASH_SHIFT) ^
			(*hash >> (8*sizeof(*hash) - VALUE_HASH_SHIFT)) ^
			ea_inode_hash;
		ако (signed_hash)
			*signed_hash = (*signed_hash << VALUE_HASH_SHIFT) ^
				(*signed_hash >> (8*sizeof(*hash) -
						  VALUE_HASH_SHIFT)) ^
				ea_inode_hash;
	}
	върни 0;
}

/*
 * ext2fs_ext_attr_hash_entry2()
 *
 * Compute the hash of an extended attribute.
 * This version of the function supports hashing entries that reference
 * external inodes (ea_inode feature).
 */
errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
				      struct ext2_ext_attr_entry *entry,
				      void *data, __u32 *hash)
{
	върни ext2fs_ext_attr_hash_entry3(fs, entry, data, hash, NULL);
}

#undef NAME_HASH_SHIFT
#undef VALUE_HASH_SHIFT

#define BLOCK_HASH_SHIFT 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
void ext2fs_ext_attr_block_rehash(struct ext2_ext_attr_header *header,
				  struct ext2_ext_attr_entry *end)
{
	struct ext2_ext_attr_entry *here;
	__u32 hash = 0;

	here = (struct ext2_ext_attr_entry *)(header+1);
	докато (here < end && !EXT2_EXT_IS_LAST_ENTRY(here)) {
		ако (!here->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			break;
		}
		hash = (hash << BLOCK_HASH_SHIFT) ^
		       (hash >> (8*sizeof(hash) - BLOCK_HASH_SHIFT)) ^
		       here->e_hash;
		here = EXT2_EXT_ATTR_NEXT(here);
	}
	header->h_hash = hash;
}

#undef BLOCK_HASH_SHIFT

__u32 ext2fs_get_ea_inode_hash(struct ext2_inode *inode)
{
	върни inode->i_atime;
}

void ext2fs_set_ea_inode_hash(struct ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(struct ext2_inode *inode)
{
	върни ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_version;
}

void ext2fs_set_ea_inode_ref(struct ext2_inode *inode, __u64 ref_count)
{
	inode->i_ctime = (__u32)(ref_count >> 32);
	inode->osd1.linux1.l_i_version = (__u32)ref_count;
}

static errcode_t check_ext_attr_header(struct ext2_ext_attr_header *header)
{
	ако ((header->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     header->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    header->h_blocks != 1)
		върни EXT2_ET_BAD_EA_HEADER;

	върни 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block, void *buf,
				ext2_ino_t inum)
{
	цяло		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	ако (retval)
		върни retval;

	ако (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_block_csum_verify(fs, inum, block, buf))
		csum_failed = 1;

#ifdef WORDS_BIGENDIAN
	ext2fs_swap_ext_attr(buf, buf, fs->blocksize, 1);
#endif

	retval = check_ext_attr_header(buf);
	ако (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	върни retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block, void *buf)
{
	върни ext2fs_read_ext_attr3(fs, block, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, void *buf)
{
	върни ext2fs_read_ext_attr2(fs, block, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block, void *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	char		*write_buf;

#ifdef WORDS_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	ако (retval)
		върни retval;
	ext2fs_swap_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#иначе
	write_buf = (char *) inbuf;
#endif

	retval = ext2fs_ext_attr_block_csum_set(fs, inum, block,
			(struct ext2_ext_attr_header *)write_buf);
	ако (retval)
		върни retval;

	retval = io_channel_write_blk64(fs->io, block, 1, write_buf);
#ifdef WORDS_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#endif
	ако (!retval)
		ext2fs_mark_changed(fs);
	върни retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block, void *inbuf)
{
	върни ext2fs_write_ext_attr3(fs, block, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block, void *inbuf)
{
	върни ext2fs_write_ext_attr2(fs, block, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    char *block_buf, цяло adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	struct ext2_ext_attr_header *header;
	char	*buf = 0;

	ако ((blk >= ext2fs_blocks_count(fs->super)) ||
	    (blk < fs->super->s_first_data_block))
		върни EXT2_ET_BAD_EA_BLOCK_NUM;

	ако (!block_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		ако (retval)
			върни retval;
		block_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, block_buf, inum);
	ако (retval)
		goto errout;

	header = (struct ext2_ext_attr_header *) block_buf;
	header->h_refcount += adjust;
	ако (newcount)
		*newcount = header->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, block_buf, inum);
	ако (retval)
		goto errout;

errout:
	ако (buf)
		ext2fs_free_mem(&buf);
	върни retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    char *block_buf, цяло adjust,
				    __u32 *newcount)
{
	върни ext2fs_adjust_ea_refcount3(fs, blk, block_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					char *block_buf, цяло adjust,
					__u32 *newcount)
{
	върни ext2fs_adjust_ea_refcount2(fs, blk, block_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
struct ext2_xattr {
	цяло name_index;
	char *name;
	char *short_name;
	void *value;
	unsigned цяло value_len;
	ext2_ino_t ea_ino;
};

struct ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	struct ext2_xattr *attrs;
	цяло capacity;
	цяло count;
	цяло ibody_count;
	ext2_ino_t ino;
	unsigned цяло flags;
};

static errcode_t ext2fs_xattrs_expand(struct ext2_xattr_handle *h,
				      unsigned цяло expandby)
{
	struct ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->capacity + expandby,
				   sizeof(struct ext2_xattr), &new_attrs);
	ако (err)
		върни err;

	memcpy(new_attrs, h->attrs, h->capacity * sizeof(struct ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->capacity += expandby;
	h->attrs = new_attrs;

	върни 0;
}

struct ea_name_index {
	цяло index;
	const char *name;
};

/* Keep these names sorted in order of decreasing specificity. */
static struct ea_name_index ea_names[] = {
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

static const char *find_ea_prefix(цяло index)
{
	struct ea_name_index *e;

	за (e = ea_names; e->name; e++)
		ако (e->index == index)
			върни e->name;

	върни NULL;
}

static цяло find_ea_index(const char *fullname, const char **name, цяло *index)
{
	struct ea_name_index *e;

	за (e = ea_names; e->name; e++) {
		ако (strncmp(fullname, e->name, strlen(e->name)) == 0) {
			*name = fullname + strlen(e->name);
			*index = e->index;
			върни 1;
		}
	}
	върни 0;
}

errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       struct ext2_inode_large *inode)
{
	struct ext2_ext_attr_header *header;
	void *block_buf = NULL;
	blk64_t blk;
	errcode_t err;
	struct ext2_inode_large i;

	/* Read inode? */
	ако (inode == NULL) {
		err = ext2fs_read_inode_full(fs, ino, (struct ext2_inode *)&i,
					     sizeof(struct ext2_inode_large));
		ако (err)
			върни err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (struct ext2_inode *)inode);
	ако (blk == 0)
		върни 0;

	/* Find block, zero it, write back */
	ако ((blk < fs->super->s_first_data_block) ||
	    (blk >= ext2fs_blocks_count(fs->super))) {
		err = EXT2_ET_BAD_EA_BLOCK_NUM;
		goto out;
	}

	err = ext2fs_get_mem(fs->blocksize, &block_buf);
	ако (err)
		goto out;

	err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
	ако (err)
		goto out2;

	/* We only know how to deal with v2 EA blocks */
	header = (struct ext2_ext_attr_header *) block_buf;
	ако (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_HEADER;
		goto out2;
	}

	header->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
	ако (err)
		goto out2;

	/* Erase link to block */
	ext2fs_file_acl_block_set(fs, (struct ext2_inode *)inode, 0);
	ако (header->h_refcount == 0)
		ext2fs_block_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_blocks(fs, (struct ext2_inode *)inode, 1);
	ако (err)
		goto out2;

	/* Write inode? */
	ако (inode == &i) {
		err = ext2fs_write_inode_full(fs, ino, (struct ext2_inode *)&i,
					      sizeof(struct ext2_inode_large));
		ако (err)
			goto out2;
	}

out2:
	ext2fs_free_mem(&block_buf);
out:
	върни err;
}

static errcode_t prep_ea_block_for_write(ext2_filsys fs, ext2_ino_t ino,
					 struct ext2_inode_large *inode)
{
	struct ext2_ext_attr_header *header;
	void *block_buf = NULL;
	blk64_t blk, goal;
	errcode_t err;

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (struct ext2_inode *)inode);
	ако (blk != 0) {
		ако ((blk < fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_count(fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_NUM;
			goto out;
		}

		err = ext2fs_get_mem(fs->blocksize, &block_buf);
		ако (err)
			goto out;

		err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
		ако (err)
			goto out2;

		/* We only know how to deal with v2 EA blocks */
		header = (struct ext2_ext_attr_header *) block_buf;
		ако (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			goto out2;
		}

		/* Single-user block.  We're done here. */
		ако (header->h_refcount == 1)
			goto out2;

		/* We need to CoW the block. */
		header->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
		ако (err)
			goto out2;
	} иначе {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_add_blocks(fs, (struct ext2_inode *)inode,
					     1);
		ако (err)
			goto out;
	}

	/* Allocate a block */
	goal = ext2fs_find_inode_goal(fs, ino, (struct ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, NULL, &blk);
	ако (err)
		goto out2;
	ext2fs_file_acl_block_set(fs, (struct ext2_inode *)inode, blk);
out2:
	ако (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	върни err;
}


static inline цяло
posix_acl_xattr_count(size_t size)
{
        ако (size < sizeof(posix_acl_xattr_header))
                върни -1;
        size -= sizeof(posix_acl_xattr_header);
        ако (size % sizeof(posix_acl_xattr_entry))
                върни -1;
        върни size / sizeof(posix_acl_xattr_entry);
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
	char *e;

	цяло count;

	ако (!value)
		върни EINVAL;
	ако (size < sizeof(posix_acl_xattr_header))
		върни ENOMEM;
	ако (header->a_version != ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION))
		върни EINVAL;

	count = posix_acl_xattr_count(size);
	ext_acl = out_buf;
	ext_acl->a_version = ext2fs_cpu_to_le32(EXT4_ACL_VERSION);

	ако (count <= 0)
		върни EINVAL;

	e = (char *) out_buf + sizeof(ext4_acl_header);
	s = sizeof(ext4_acl_header);
	за (end = entry + count; entry != end;entry++) {
		ext4_acl_entry *disk_entry = (ext4_acl_entry*) e;
		disk_entry->e_tag = entry->e_tag;
		disk_entry->e_perm = entry->e_perm;

		switch(ext2fs_le16_to_cpu(entry->e_tag)) {
			case ACL_USER_OBJ:
			case ACL_GROUP_OBJ:
			case ACL_MASK:
			case ACL_OTHER:
				e += sizeof(ext4_acl_entry_short);
				s += sizeof(ext4_acl_entry_short);
				break;
			case ACL_USER:
			case ACL_GROUP:
				disk_entry->e_id = entry->e_id;
				e += sizeof(ext4_acl_entry);
				s += sizeof(ext4_acl_entry);
				break;
			default:
				върни EINVAL;
		}
	}
	*size_out = s;
	върни 0;
}

static errcode_t convert_disk_buffer_to_posix_acl(const void *value, size_t size,
						  void **out_buf, size_t *size_out)
{
	posix_acl_xattr_header *header;
	posix_acl_xattr_entry *entry;
	const ext4_acl_header *ext_acl = (const ext4_acl_header *) value;
	errcode_t err;
	const char *cp;
	char *out;

	ако ((!value) ||
	    (size < sizeof(ext4_acl_header)) ||
	    (ext_acl->a_version != ext2fs_cpu_to_le32(EXT4_ACL_VERSION)))
		върни EINVAL;

	err = ext2fs_get_mem(size * 2, &out);
	ако (err)
		върни err;

	header = (posix_acl_xattr_header *) out;
	header->a_version = ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION);
	entry = (posix_acl_xattr_entry *) (out + sizeof(posix_acl_xattr_header));

	cp = (const char *) value + sizeof(ext4_acl_header);
	size -= sizeof(ext4_acl_header);

	докато (size > 0) {
		const ext4_acl_entry *disk_entry = (const ext4_acl_entry *) cp;

		entry->e_tag = disk_entry->e_tag;
		entry->e_perm = disk_entry->e_perm;

		switch(ext2fs_le16_to_cpu(entry->e_tag)) {
			case ACL_USER_OBJ:
			case ACL_GROUP_OBJ:
			case ACL_MASK:
			case ACL_OTHER:
				entry->e_id = 0;
				cp += sizeof(ext4_acl_entry_short);
				size -= sizeof(ext4_acl_entry_short);
				break;
			case ACL_USER:
			case ACL_GROUP:
				entry->e_id = disk_entry->e_id;
				cp += sizeof(ext4_acl_entry);
				size -= sizeof(ext4_acl_entry);
				break;
			default:
				ext2fs_free_mem(&out);
				върни EINVAL;
		}
		entry++;
	}
	*out_buf = out;
	*size_out = ((char *) entry - out);
	върни 0;
}

static errcode_t
write_xattrs_to_buffer(ext2_filsys fs, struct ext2_xattr *attrs, цяло count,
		       void *entries_start, unsigned цяло storage_size,
		       unsigned цяло value_offset_correction, цяло write_hash)
{
	struct ext2_xattr *x;
	struct ext2_ext_attr_entry *e = entries_start;
	char *end = (char *) entries_start + storage_size;
	unsigned цяло value_size;
	errcode_t err;

	memset(entries_start, 0, storage_size);
	за (x = attrs; x < attrs + count; x++) {
		value_size = ((x->value_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = strlen(x->short_name);
		e->e_name_index = x->name_index;

		e->e_value_size = x->value_len;
		e->e_value_inum = x->ea_ino;

		/* Store name */
		memcpy((char *)e + sizeof(*e), x->short_name, e->e_name_len);
		ако (x->ea_ino) {
			e->e_value_offs = 0;
		} иначе {
			end -= value_size;
			e->e_value_offs = end - (char *) entries_start +
						value_offset_correction;
			memcpy(end, x->value, e->e_value_size);
		}

		ако (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : end,
							  &e->e_hash);
			ако (err)
				върни err;
		} иначе
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_NEXT(e);
		*(__u32 *)e = 0;
	}
	върни 0;
}

errcode_t ext2fs_xattrs_write(struct ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	const unsigned цяло inode_size = EXT2_INODE_SIZE(fs->super);
	struct ext2_inode_large *inode;
	char *start, *block_buf = NULL;
	struct ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	blk64_t blk;
	unsigned цяло storage_size;
	unsigned цяло i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_size;
	ако (i < sizeof(*inode))
		i = sizeof(*inode);
	err = ext2fs_get_memzero(i, &inode);
	ако (err)
		върни err;

	err = ext2fs_read_inode_full(fs, handle->ino, EXT2_INODE(inode),
				     inode_size);
	ако (err)
		goto out;

	/* If extra_isize isn't set, we need to set it now */
	ако (inode->i_extra_isize == 0 &&
	    inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		char *p = (char *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		ако (extra == 0)
			extra = sizeof(__u32);
		memset(p + EXT2_GOOD_OLD_INODE_SIZE, 0, extra);
		inode->i_extra_isize = extra;
	}
	ако (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		goto out;
	}

	/* Does the inode have space for EA? */
	ако (inode->i_extra_isize < sizeof(inode->i_extra_isize) ||
	    inode_size <= EXT2_GOOD_OLD_INODE_SIZE + inode->i_extra_isize +
								sizeof(__u32))
		goto write_ea_block;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	memcpy(((char *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, &ea_inode_magic, sizeof(__u32));
	storage_size = inode_size - EXT2_GOOD_OLD_INODE_SIZE -
				inode->i_extra_isize - sizeof(__u32);
	start = ((char *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
				inode->i_extra_isize + sizeof(__u32);

	err = write_xattrs_to_buffer(fs, handle->attrs, handle->ibody_count,
				     start, storage_size, 0, 0);
	ако (err)
		goto out;
write_ea_block:
	/* Are we done? */
	ако (handle->ibody_count == handle->count &&
	    !ext2fs_file_acl_block(fs, EXT2_INODE(inode)))
		goto skip_ea_block;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &block_buf);
	ако (err)
		goto out;

	storage_size = fs->blocksize - sizeof(struct ext2_ext_attr_header);
	start = block_buf + sizeof(struct ext2_ext_attr_header);

	err = write_xattrs_to_buffer(fs, handle->attrs + handle->ibody_count,
				     handle->count - handle->ibody_count, start,
				     storage_size, start - block_buf, 1);
	ако (err)
		goto out2;

	/* Write a header on the EA block */
	header = (struct ext2_ext_attr_header *) block_buf;
	header->h_magic = EXT2_EXT_ATTR_MAGIC;
	header->h_refcount = 1;
	header->h_blocks = 1;

	/* Get a new block for writing */
	err = prep_ea_block_for_write(fs, handle->ino, inode);
	ако (err)
		goto out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_block(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, handle->ino);
	ако (err)
		goto out2;

skip_ea_block:
	blk = ext2fs_file_acl_block(fs, (struct ext2_inode *)inode);
	ако (!block_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		ако (err)
			goto out;
	}

	/* Write the inode */
	err = ext2fs_write_inode_full(fs, handle->ino, EXT2_INODE(inode),
				      inode_size);
	ако (err)
		goto out2;

out2:
	ext2fs_free_mem(&block_buf);
out:
	ext2fs_free_mem(&inode);
	върни err;
}

static errcode_t read_xattrs_from_buffer(struct ext2_xattr_handle *handle,
					 struct ext2_inode_large *inode,
					 struct ext2_ext_attr_entry *entries,
					 unsigned цяло storage_size,
					 char *value_start)
{
	struct ext2_xattr *x;
	struct ext2_ext_attr_entry *entry, *end;
	const char *prefix;
	unsigned цяло remain, prefix_len;
	errcode_t err;
	unsigned цяло values_size = storage_size +
			((char *)entries - value_start);

	/* find the end */
	end = entries;
	remain = storage_size;
	докато (remain >= sizeof(struct ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(end)) {

		/* header eats this space */
		remain -= sizeof(struct ext2_ext_attr_entry);

		/* is attribute name valid? */
		ако (EXT2_EXT_ATTR_SIZE(end->e_name_len) > remain)
			върни EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(end->e_name_len);
		end = EXT2_EXT_ATTR_NEXT(end);
	}

	entry = entries;
	remain = storage_size;
	докато (remain >= sizeof(struct ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(entry)) {

		/* Allocate space for more attrs? */
		ако (handle->count == handle->capacity) {
			err = ext2fs_xattrs_expand(handle, 4);
			ако (err)
				върни err;
		}

		x = handle->attrs + handle->count;

		/* header eats this space */
		remain -= sizeof(struct ext2_ext_attr_entry);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(entry->e_name_len);

		/* Extract name */
		prefix = find_ea_prefix(entry->e_name_index);
		prefix_len = (prefix ? strlen(prefix) : 0);
		err = ext2fs_get_memzero(entry->e_name_len + prefix_len + 1,
					 &x->name);
		ако (err)
			върни err;
		ако (prefix)
			memcpy(x->name, prefix, prefix_len);
		ако (entry->e_name_len)
			memcpy(x->name + prefix_len,
			       (char *)entry + sizeof(*entry),
			       entry->e_name_len);
		x->short_name = x->name + prefix_len;
		x->name_index = entry->e_name_index;

		/* Check & copy value */
		ако (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    entry->e_value_inum != 0)
			върни EXT2_ET_BAD_EA_BLOCK_NUM;

		ако (entry->e_value_inum == 0) {
			ако (entry->e_value_size > remain)
				върни EXT2_ET_EA_BAD_VALUE_SIZE;

			ако (entry->e_value_offs + entry->e_value_size > values_size)
				върни EXT2_ET_EA_BAD_VALUE_OFFSET;

			ако (entry->e_value_size > 0 &&
			    value_start + entry->e_value_offs <
			    (char *)end + sizeof(__u32))
				върни EXT2_ET_EA_BAD_VALUE_OFFSET;

			remain -= entry->e_value_size;

			err = ext2fs_get_mem(entry->e_value_size, &x->value);
			ако (err)
				върни err;
			memcpy(x->value, value_start + entry->e_value_offs,
			       entry->e_value_size);
		} иначе {
			struct ext2_inode *ea_inode;
			ext2_file_t ea_file;

			ако (entry->e_value_offs != 0)
				върни EXT2_ET_EA_BAD_VALUE_OFFSET;

			ако (entry->e_value_size > (64 * 1024))
				върни EXT2_ET_EA_BAD_VALUE_SIZE;

			err = ext2fs_get_mem(entry->e_value_size, &x->value);
			ако (err)
				върни err;

			err = ext2fs_file_open(handle->fs, entry->e_value_inum,
					       0, &ea_file);
			ако (err)
				върни err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			ако ((ea_inode->i_flags & EXT4_INLINE_DATA_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_count == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			иначе ако ((__u64) ext2fs_file_get_size(ea_file) !=
				 entry->e_value_size)
				err = EXT2_ET_EA_BAD_VALUE_SIZE;
			иначе
				err = ext2fs_file_read(ea_file, x->value,
						       entry->e_value_size, 0);
			ext2fs_file_close(ea_file);
			ако (err)
				върни err;
		}

		x->ea_ino = entry->e_value_inum;
		x->value_len = entry->e_value_size;

		/* e_hash may be 0 in older inode's ea */
		ако (entry->e_hash != 0) {
			__u32 hash, signed_hash;

			void *data = (entry->e_value_inum != 0) ?
					0 : value_start + entry->e_value_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, entry,
							  data, &hash,
							  &signed_hash);
			ако (err)
				върни err;
			ако ((entry->e_hash != hash) &&
			    (entry->e_hash != signed_hash)) {
				struct ext2_inode child;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							entry->e_value_inum,
							&child);
				ако (err)
					върни err;
				ако (child.i_mtime != handle->ino ||
				    child.i_generation != inode->i_generation)
					върни EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->count++;
		entry = EXT2_EXT_ATTR_NEXT(entry);
	}

	върни 0;
}

static void xattrs_free_keys(struct ext2_xattr_handle *h)
{
	struct ext2_xattr *a = h->attrs;
	цяло i;

	за (i = 0; i < h->capacity; i++) {
		ако (a[i].name)
			ext2fs_free_mem(&a[i].name);
		ако (a[i].value)
			ext2fs_free_mem(&a[i].value);
	}
	h->count = 0;
	h->ibody_count = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(struct ext2_xattr_handle *handle,
				   struct ext2_inode_large *inode)
{
	struct ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	unsigned цяло storage_size;
	char *start, *block_buf = NULL;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_keys(handle);

	/* Does the inode have space for EA? */
	ако (inode->i_extra_isize < sizeof(inode->i_extra_isize) ||
	    EXT2_INODE_SIZE(handle->fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  sizeof(__u32))
		goto read_ea_block;
	ако (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		goto out;
	}

	/* Look for EA in the inode */
	memcpy(&ea_inode_magic, ((char *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, sizeof(__u32));
	ако (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_size = EXT2_INODE_SIZE(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_SIZE - inode->i_extra_isize -
			sizeof(__u32);
		start = ((char *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + sizeof(__u32);

		err = read_xattrs_from_buffer(handle, inode,
					(struct ext2_ext_attr_entry *) start,
					storage_size, start);
		ако (err)
			goto out;

		handle->ibody_count = handle->count;
	}

read_ea_block:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_block(handle->fs, EXT2_INODE(inode));
	ако (blk != 0) {
		ако ((blk < handle->fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_count(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_NUM;
			goto out;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &block_buf);
		ако (err)
			goto out;

		err = ext2fs_read_ext_attr3(handle->fs, blk, block_buf,
					    handle->ino);
		ако (err)
			goto out3;

		/* We only know how to deal with v2 EA blocks */
		header = (struct ext2_ext_attr_header *) block_buf;
		ако (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			goto out3;
		}

		/* Read EAs */
		storage_size = handle->fs->blocksize -
			sizeof(struct ext2_ext_attr_header);
		start = block_buf + sizeof(struct ext2_ext_attr_header);
		err = read_xattrs_from_buffer(handle, inode,
					(struct ext2_ext_attr_entry *) start,
					storage_size, block_buf);
	}

out3:
	ако (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	върни err;
}

errcode_t ext2fs_xattrs_read(struct ext2_xattr_handle *handle)
{
	struct ext2_inode_large *inode;
	size_t inode_size = EXT2_INODE_SIZE(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	ако (inode_size < sizeof(*inode))
		inode_size = sizeof(*inode);
	err = ext2fs_get_memzero(inode_size, &inode);
	ако (err)
		върни err;

	err = ext2fs_read_inode_full(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_SIZE(handle->fs->super));
	ако (err)
		goto out;

	err = ext2fs_xattrs_read_inode(handle, inode);

out:
	ext2fs_free_mem(&inode);

	върни err;
}

errcode_t ext2fs_xattrs_iterate(struct ext2_xattr_handle *h,
				цяло (*func)(char *name, char *value,
					    size_t value_len, void *data),
				void *data)
{
	struct ext2_xattr *x;
	цяло dirty = 0;
	цяло ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	за (x = h->attrs; x < h->attrs + h->count; x++) {
		ret = func(x->name, x->value, x->value_len, data);
		ако (ret & XATTR_CHANGED)
			dirty = 1;
		ако (ret & XATTR_ABORT)
			break;
	}

	ако (dirty)
		върни ext2fs_xattrs_write(h);
	върни 0;
}

errcode_t ext2fs_xattr_get(struct ext2_xattr_handle *h, const char *key,
			   void **value, size_t *value_len)
{
	struct ext2_xattr *x;
	char *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	за (x = h->attrs; x < h->attrs + h->count; x++) {
		ако (strcmp(x->name, key))
			continue;

		ако (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(key, "system.posix_acl_default") == 0) ||
		     (strcmp(key, "system.posix_acl_access") == 0))) {
			err = convert_disk_buffer_to_posix_acl(x->value, x->value_len,
							       value, value_len);
			върни err;
		} иначе {
			err = ext2fs_get_mem(x->value_len, &val);
			ако (err)
				върни err;
			memcpy(val, x->value, x->value_len);
			*value = val;
			*value_len = x->value_len;
			върни 0;
		}
	}

	върни EXT2_ET_EA_KEY_NOT_FOUND;
}

errcode_t ext2fs_xattr_inode_max_size(ext2_filsys fs, ext2_ino_t ino,
				      size_t *size)
{
	struct ext2_ext_attr_entry *entry;
	struct ext2_inode_large *inode;
	__u32 ea_inode_magic;
	unsigned цяло minoff;
	char *start;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_SIZE(fs->super);
	ако (i < sizeof(*inode))
		i = sizeof(*inode);
	err = ext2fs_get_memzero(i, &inode);
	ако (err)
		върни err;

	err = ext2fs_read_inode_full(fs, ino, (struct ext2_inode *)inode,
				     EXT2_INODE_SIZE(fs->super));
	ако (err)
		goto out;

	/* Does the inode have size for EA? */
	ако (EXT2_INODE_SIZE(fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  sizeof(__u32)) {
		err = EXT2_ET_INLINE_DATA_NO_SPACE;
		goto out;
	}

	minoff = EXT2_INODE_SIZE(fs->super) - sizeof(*inode) - sizeof(__u32);
	memcpy(&ea_inode_magic, ((char *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, sizeof(__u32));
	ако (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		start= ((char *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + sizeof(__u32);
		entry = (struct ext2_ext_attr_entry *) start;
		докато (!EXT2_EXT_IS_LAST_ENTRY(entry)) {
			ако (!entry->e_value_inum && entry->e_value_size) {
				unsigned цяло offs = entry->e_value_offs;
				ако (offs < minoff)
					minoff = offs;
			}
			entry = EXT2_EXT_ATTR_NEXT(entry);
		}
		*size = minoff - ((char *)entry - (char *)start) - sizeof(__u32);
	} иначе {
		/* no xattr.  return a maximum size */
		*size = EXT2_EXT_ATTR_SIZE(minoff -
					   EXT2_EXT_ATTR_LEN(strlen("data")) -
					   EXT2_EXT_ATTR_ROUND - sizeof(__u32));
	}

out:
	ext2fs_free_mem(&inode);
	върни err;
}

static errcode_t xattr_create_ea_inode(ext2_filsys fs, const void *value,
				       size_t value_len, ext2_ino_t *ea_ino)
{
	struct ext2_inode inode;
	ext2_ino_t ino;
	ext2_file_t file;
	__u32 hash;
	errcode_t ret;

	ret = ext2fs_new_inode(fs, 0, 0, 0, &ino);
	ако (ret)
		върни ret;

	memset(&inode, 0, sizeof(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	ако (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_size = 0;
	inode.i_mode = LINUX_S_IFREG | 0600;
	inode.i_links_count = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	ако (ret)
		върни ret;
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
	ако (ret)
		върни ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	ако (ret)
		върни ret;
	ret = ext2fs_file_write(file, value, value_len, NULL);
	ext2fs_file_close(file);
	ако (ret)
		върни ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	върни 0;
}

static errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	struct ext2_inode_large inode;
	__u64 ref_count;
	errcode_t ret;

	ret = ext2fs_read_inode_full(fs, ino, (struct ext2_inode *)&inode,
				     sizeof(inode));
	ако (ret)
		goto out;

	ref_count = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_count--;
	ext2fs_set_ea_inode_ref(EXT2_INODE(&inode), ref_count);

	ако (ref_count)
		goto write_out;

	inode.i_links_count = 0;
	ext2fs_set_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	ако (ret)
		goto write_out;

	ако (ext2fs_inode_has_valid_blocks2(fs, (struct ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (struct ext2_inode *)&inode, NULL,
				   0, ~0ULL);
		ако (ret)
			goto out;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_out:
	ret = ext2fs_write_inode_full(fs, ino, (struct ext2_inode *)&inode,
				      sizeof(inode));
out:
	върни ret;
}

static errcode_t xattr_update_entry(ext2_filsys fs, struct ext2_xattr *x,
				    const char *name, const char *short_name,
				    цяло index, const void *value,
				    size_t value_len, цяло in_inode)
{
	ext2_ino_t ea_ino = 0;
	void *new_value = NULL;
	char *new_name = NULL;
	цяло name_len;
	errcode_t ret;

	ако (!x->name) {
		name_len = strlen(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		ако (ret)
			goto fail;
		memcpy(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(value_len, &new_value);
	ако (ret)
		goto fail;
	memcpy(new_value, value, value_len);

	ако (in_inode) {
		ret = xattr_create_ea_inode(fs, value, value_len, &ea_ino);
		ако (ret)
			goto fail;
	}

	ако (x->ea_ino) {
		ret = xattr_inode_dec_ref(fs, x->ea_ino);
		ако (ret)
			goto fail;
	}

	ако (!x->name) {
		x->name = new_name;
		x->short_name = new_name + (short_name  - name);
	}
	x->name_index = index;

	ако (x->value)
		ext2fs_free_mem(&x->value);
	x->value = new_value;
	x->value_len = value_len;
	x->ea_ino = ea_ino;
	върни 0;
fail:
	ако (new_name)
		ext2fs_free_mem(&new_name);
	ако (new_value)
		ext2fs_free_mem(&new_value);
	ако (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	върни ret;
}

static цяло xattr_find_position(struct ext2_xattr *attrs, цяло count,
			       const char *shortname, цяло name_idx)
{
	struct ext2_xattr *x;
	цяло i;
	цяло shortname_len, x_shortname_len;

	shortname_len = strlen(shortname);

	за (i = 0, x = attrs; i < count; i++, x++) {
		ако (name_idx < x->name_index)
			break;
		ако (name_idx > x->name_index)
			continue;

		x_shortname_len = strlen(x->short_name);
		ако (shortname_len < x_shortname_len)
			break;
		ако (shortname_len > x_shortname_len)
			continue;

		ако (memcmp(shortname, x->short_name, shortname_len) <= 0)
			break;
	}
	върни i;
}

static errcode_t xattr_array_update(struct ext2_xattr_handle *h,
				    const char *name,
				    const void *value, size_t value_len,
				    цяло ibody_free, цяло block_free,
				    цяло old_idx, цяло in_inode)
{
	struct ext2_xattr tmp;
	цяло add_to_ibody;
	цяло needed;
	цяло name_len, name_idx = 0;
	const char *shortname = name;
	цяло new_idx;
	цяло ret;

	find_ea_index(name, &shortname, &name_idx);
	name_len = strlen(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	ако (!in_inode)
		needed += EXT2_EXT_ATTR_SIZE(value_len);

	ако (old_idx >= 0 && old_idx < h->ibody_count) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		ако (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_SIZE(
						h->attrs[old_idx].value_len);
	}

	ако (needed <= ibody_free) {
		ако (old_idx < 0) {
			new_idx = h->ibody_count;
			add_to_ibody = 1;
			goto add_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, value,
					 value_len, in_inode);
		ако (ret)
			върни ret;
		ако (h->ibody_count <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_count + 1,
				h->attrs + h->ibody_count,
				(old_idx - h->ibody_count) * sizeof(*h->attrs));
			h->attrs[h->ibody_count] = tmp;
			h->ibody_count++;
		}
		върни 0;
	}

	ако (h->ibody_count <= old_idx) {
		block_free += EXT2_EXT_ATTR_LEN(name_len);
		ако (!h->attrs[old_idx].ea_ino)
			block_free +=
				EXT2_EXT_ATTR_SIZE(h->attrs[old_idx].value_len);
	}

	ако (needed > block_free)
		върни EXT2_ET_EA_NO_SPACE;

	ако (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, value,
					 value_len, in_inode);
		ако (ret)
			върни ret;
		ако (old_idx < h->ibody_count) {
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
				(new_idx - old_idx) * sizeof(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_count--;
		}
		върни 0;
	}

	new_idx = xattr_find_position(h->attrs + h->ibody_count,
				      h->count - h->ibody_count,
				      shortname, name_idx);
	new_idx += h->ibody_count;
	add_to_ibody = 0;

add_new:
	ако (h->count == h->capacity) {
		ret = ext2fs_xattrs_expand(h, 4);
		ако (ret)
			върни ret;
	}

	ret = xattr_update_entry(h->fs, &h->attrs[h->count], name, shortname,
				 name_idx, value, value_len, in_inode);
	ако (ret)
		върни ret;

	tmp = h->attrs[h->count];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->count - new_idx)*sizeof(*h->attrs));
	h->attrs[new_idx] = tmp;
	ако (add_to_ibody)
		h->ibody_count++;
	h->count++;
	върни 0;
}

static цяло space_used(struct ext2_xattr *attrs, цяло count)
{
	цяло total = 0;
	struct ext2_xattr *x;
	цяло i, len;

	за (i = 0, x = attrs; i < count; i++, x++) {
		len = strlen(x->short_name);
		total += EXT2_EXT_ATTR_LEN(len);
		ако (!x->ea_ino)
			total += EXT2_EXT_ATTR_SIZE(x->value_len);
	}
	върни total;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#define EXT4_XATTR_MIN_LARGE_EA_SIZE(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - sizeof(struct ext2_ext_attr_header) - 4)

errcode_t ext2fs_xattr_set(struct ext2_xattr_handle *h,
			   const char *name,
			   const void *value,
			   size_t value_len)
{
	ext2_filsys fs = h->fs;
	const цяло inode_size = EXT2_INODE_SIZE(fs->super);
	struct ext2_inode_large *inode = NULL;
	struct ext2_xattr *x;
	char *new_value;
	цяло ibody_free, block_free;
	цяло in_inode = 0;
	цяло old_idx = -1;
	цяло extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(value_len, &new_value);
	ако (ret)
		върни ret;
	ако (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((strcmp(name, "system.posix_acl_default") == 0) ||
	     (strcmp(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_to_disk_buffer(value, value_len,
						       new_value, &value_len);
		ако (ret)
			goto out;
	} иначе ако (value_len)
		memcpy(new_value, value, value_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	за (x = h->attrs; x < h->attrs + h->count; x++) {
		ако (!strcmp(x->name, name)) {
			ако (!x->ea_ino && x->value_len == value_len &&
			    (!value_len ||
			     !memcmp(x->value, new_value, value_len))) {
				ret = 0;
				goto out;
			}
			old_idx = x - h->attrs;
			break;
		}
	}

	ret = ext2fs_get_memzero(inode_size, &inode);
	ако (ret)
		goto out;
	ret = ext2fs_read_inode_full(fs, h->ino,
				     (struct ext2_inode *)inode,
				     inode_size);
	ако (ret)
		goto out;
	ако (inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		extra_isize = inode->i_extra_isize;
		ако (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			ако (extra_isize == 0)
				extra_isize = sizeof(__u32);
		}
		ibody_free = inode_size - EXT2_GOOD_OLD_INODE_SIZE;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= sizeof(__u32) * 2;
		ibody_free -= space_used(h->attrs, h->ibody_count);
	} иначе
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	ако (strcmp(name, "system.data") == 0) {
		ако (h->ibody_count <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			goto out;
		}
		ret = xattr_array_update(h, name, new_value, value_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		ако (ret)
			goto out;
		goto write_out;
	}

	block_free = fs->blocksize;
	block_free -= sizeof(struct ext2_ext_attr_header);
	/* Final null entry. */
	block_free -= sizeof(__u32);
	block_free -= space_used(h->attrs + h->ibody_count,
				 h->count - h->ibody_count);

	ако (ext2fs_has_feature_ea_inode(fs->super) &&
	    value_len > EXT4_XATTR_MIN_LARGE_EA_SIZE(fs->blocksize))
		in_inode = 1;

	ret = xattr_array_update(h, name, new_value, value_len, ibody_free,
				 block_free, old_idx, in_inode);
	ако (ret == EXT2_ET_EA_NO_SPACE && !in_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_value, value_len,
			ibody_free, block_free, old_idx, 1 /* in_inode */);
	ако (ret)
		goto out;

write_out:
	ret = ext2fs_xattrs_write(h);
out:
	ако (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_value);
	върни ret;
}

errcode_t ext2fs_xattr_remove(struct ext2_xattr_handle *handle,
			      const char *key)
{
	struct ext2_xattr *x;
	struct ext2_xattr *end = handle->attrs + handle->count;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	за (x = handle->attrs; x < end; x++) {
		ако (strcmp(x->name, key) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->value);
			ако (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (end - x - 1)*sizeof(*x));
			memset(end - 1, 0, sizeof(*end));
			ако (x < handle->attrs + handle->ibody_count)
				handle->ibody_count--;
			handle->count--;
			върни ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	върни 0;
}

errcode_t ext2fs_xattr_remove_all(struct ext2_xattr_handle *handle)
{
	struct ext2_xattr *x;
	struct ext2_xattr *end = handle->attrs + handle->count;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	за (x = handle->attrs; x < end; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->value);
		ако (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_count = 0;
	handle->count = 0;
	върни ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     struct ext2_xattr_handle **handle)
{
	struct ext2_xattr_handle *h;
	errcode_t err;

	ако (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_data(fs->super))
		върни EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(sizeof(*h), &h);
	ако (err)
		върни err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->capacity = 4;
	err = ext2fs_get_arrayzero(h->capacity, sizeof(struct ext2_xattr),
				   &h->attrs);
	ако (err) {
		ext2fs_free_mem(&h);
		върни err;
	}
	h->count = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	върни 0;
}

errcode_t ext2fs_xattrs_close(struct ext2_xattr_handle **handle)
{
	struct ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_keys(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	върни 0;
}

errcode_t ext2fs_xattrs_count(struct ext2_xattr_handle *handle, size_t *count)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*count = handle->count;
	върни 0;
}

errcode_t ext2fs_xattrs_flags(struct ext2_xattr_handle *handle,
			      unsigned цяло *new_flags, unsigned цяло *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	ако (old_flags)
		*old_flags = handle->flags;
	ако (new_flags)
		handle->flags = *new_flags;
	върни 0;
}
