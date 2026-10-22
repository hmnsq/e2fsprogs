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
#もし HAVE_UNISTD_H
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
	もし (retval)
		戻る retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	戻る 0;
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
	整数 n;

	ために (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*sizeof(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	もし (entry->e_value_inum == 0 && entry->e_value_size != 0) {
		__u32 *value = (__u32 *)data;
		ために (n = (entry->e_value_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << VALUE_HASH_SHIFT) ^
			       (hash >> (8*sizeof(hash) - VALUE_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*value++);
		}
	}

	戻る hash;
}

__u32 ext2fs_ext_attr_hash_entry_signed(struct ext2_ext_attr_entry *entry,
					void *data)
{
	__u32 hash = 0;
	signed char *name = (((signed char *) entry) +
			     sizeof(struct ext2_ext_attr_entry));
	整数 n;

	ために (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*sizeof(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	もし (entry->e_value_inum == 0 && entry->e_value_size != 0) {
		__u32 *value = (__u32 *)data;
		ために (n = (entry->e_value_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << VALUE_HASH_SHIFT) ^
			       (hash >> (8*sizeof(hash) - VALUE_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*value++);
		}
	}

	戻る hash;
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
	もし (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_entry_signed(entry, data);

	もし (entry->e_value_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, entry->e_value_inum,
					    &ea_inode_hash);
		もし (retval)
			戻る retval;

		*hash = (*hash << VALUE_HASH_SHIFT) ^
			(*hash >> (8*sizeof(*hash) - VALUE_HASH_SHIFT)) ^
			ea_inode_hash;
		もし (signed_hash)
			*signed_hash = (*signed_hash << VALUE_HASH_SHIFT) ^
				(*signed_hash >> (8*sizeof(*hash) -
						  VALUE_HASH_SHIFT)) ^
				ea_inode_hash;
	}
	戻る 0;
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
	戻る ext2fs_ext_attr_hash_entry3(fs, entry, data, hash, NULL);
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
	ながら (here < end && !EXT2_EXT_IS_LAST_ENTRY(here)) {
		もし (!here->e_hash) {
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
	戻る inode->i_atime;
}

void ext2fs_set_ea_inode_hash(struct ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(struct ext2_inode *inode)
{
	戻る ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_version;
}

void ext2fs_set_ea_inode_ref(struct ext2_inode *inode, __u64 ref_count)
{
	inode->i_ctime = (__u32)(ref_count >> 32);
	inode->osd1.linux1.l_i_version = (__u32)ref_count;
}

static errcode_t check_ext_attr_header(struct ext2_ext_attr_header *header)
{
	もし ((header->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     header->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    header->h_blocks != 1)
		戻る EXT2_ET_BAD_EA_HEADER;

	戻る 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block, void *buf,
				ext2_ino_t inum)
{
	整数		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	もし (retval)
		戻る retval;

	もし (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_block_csum_verify(fs, inum, block, buf))
		csum_failed = 1;

#ifdef WORDS_BIGENDIAN
	ext2fs_swap_ext_attr(buf, buf, fs->blocksize, 1);
#endif

	retval = check_ext_attr_header(buf);
	もし (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	戻る retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block, void *buf)
{
	戻る ext2fs_read_ext_attr3(fs, block, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, void *buf)
{
	戻る ext2fs_read_ext_attr2(fs, block, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block, void *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	char		*write_buf;

#ifdef WORDS_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	もし (retval)
		戻る retval;
	ext2fs_swap_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#否则
	write_buf = (char *) inbuf;
#endif

	retval = ext2fs_ext_attr_block_csum_set(fs, inum, block,
			(struct ext2_ext_attr_header *)write_buf);
	もし (retval)
		戻る retval;

	retval = io_channel_write_blk64(fs->io, block, 1, write_buf);
#ifdef WORDS_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#endif
	もし (!retval)
		ext2fs_mark_changed(fs);
	戻る retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block, void *inbuf)
{
	戻る ext2fs_write_ext_attr3(fs, block, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block, void *inbuf)
{
	戻る ext2fs_write_ext_attr2(fs, block, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    char *block_buf, 整数 adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	struct ext2_ext_attr_header *header;
	char	*buf = 0;

	もし ((blk >= ext2fs_blocks_count(fs->super)) ||
	    (blk < fs->super->s_first_data_block))
		戻る EXT2_ET_BAD_EA_BLOCK_NUM;

	もし (!block_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		もし (retval)
			戻る retval;
		block_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, block_buf, inum);
	もし (retval)
		goto errout;

	header = (struct ext2_ext_attr_header *) block_buf;
	header->h_refcount += adjust;
	もし (newcount)
		*newcount = header->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, block_buf, inum);
	もし (retval)
		goto errout;

errout:
	もし (buf)
		ext2fs_free_mem(&buf);
	戻る retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    char *block_buf, 整数 adjust,
				    __u32 *newcount)
{
	戻る ext2fs_adjust_ea_refcount3(fs, blk, block_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					char *block_buf, 整数 adjust,
					__u32 *newcount)
{
	戻る ext2fs_adjust_ea_refcount2(fs, blk, block_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
struct ext2_xattr {
	整数 name_index;
	char *name;
	char *short_name;
	void *value;
	unsigned 整数 value_len;
	ext2_ino_t ea_ino;
};

struct ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	struct ext2_xattr *attrs;
	整数 capacity;
	整数 count;
	整数 ibody_count;
	ext2_ino_t ino;
	unsigned 整数 flags;
};

static errcode_t ext2fs_xattrs_expand(struct ext2_xattr_handle *h,
				      unsigned 整数 expandby)
{
	struct ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->capacity + expandby,
				   sizeof(struct ext2_xattr), &new_attrs);
	もし (err)
		戻る err;

	memcpy(new_attrs, h->attrs, h->capacity * sizeof(struct ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->capacity += expandby;
	h->attrs = new_attrs;

	戻る 0;
}

struct ea_name_index {
	整数 index;
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

static const char *find_ea_prefix(整数 index)
{
	struct ea_name_index *e;

	ために (e = ea_names; e->name; e++)
		もし (e->index == index)
			戻る e->name;

	戻る NULL;
}

static 整数 find_ea_index(const char *fullname, const char **name, 整数 *index)
{
	struct ea_name_index *e;

	ために (e = ea_names; e->name; e++) {
		もし (strncmp(fullname, e->name, strlen(e->name)) == 0) {
			*name = fullname + strlen(e->name);
			*index = e->index;
			戻る 1;
		}
	}
	戻る 0;
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
	もし (inode == NULL) {
		err = ext2fs_read_inode_full(fs, ino, (struct ext2_inode *)&i,
					     sizeof(struct ext2_inode_large));
		もし (err)
			戻る err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (struct ext2_inode *)inode);
	もし (blk == 0)
		戻る 0;

	/* Find block, zero it, write back */
	もし ((blk < fs->super->s_first_data_block) ||
	    (blk >= ext2fs_blocks_count(fs->super))) {
		err = EXT2_ET_BAD_EA_BLOCK_NUM;
		goto out;
	}

	err = ext2fs_get_mem(fs->blocksize, &block_buf);
	もし (err)
		goto out;

	err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
	もし (err)
		goto out2;

	/* We only know how to deal with v2 EA blocks */
	header = (struct ext2_ext_attr_header *) block_buf;
	もし (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_HEADER;
		goto out2;
	}

	header->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
	もし (err)
		goto out2;

	/* Erase link to block */
	ext2fs_file_acl_block_set(fs, (struct ext2_inode *)inode, 0);
	もし (header->h_refcount == 0)
		ext2fs_block_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_blocks(fs, (struct ext2_inode *)inode, 1);
	もし (err)
		goto out2;

	/* Write inode? */
	もし (inode == &i) {
		err = ext2fs_write_inode_full(fs, ino, (struct ext2_inode *)&i,
					      sizeof(struct ext2_inode_large));
		もし (err)
			goto out2;
	}

out2:
	ext2fs_free_mem(&block_buf);
out:
	戻る err;
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
	もし (blk != 0) {
		もし ((blk < fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_count(fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_NUM;
			goto out;
		}

		err = ext2fs_get_mem(fs->blocksize, &block_buf);
		もし (err)
			goto out;

		err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
		もし (err)
			goto out2;

		/* We only know how to deal with v2 EA blocks */
		header = (struct ext2_ext_attr_header *) block_buf;
		もし (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			goto out2;
		}

		/* Single-user block.  We're done here. */
		もし (header->h_refcount == 1)
			goto out2;

		/* We need to CoW the block. */
		header->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
		もし (err)
			goto out2;
	} 否则 {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_add_blocks(fs, (struct ext2_inode *)inode,
					     1);
		もし (err)
			goto out;
	}

	/* Allocate a block */
	goal = ext2fs_find_inode_goal(fs, ino, (struct ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, NULL, &blk);
	もし (err)
		goto out2;
	ext2fs_file_acl_block_set(fs, (struct ext2_inode *)inode, blk);
out2:
	もし (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	戻る err;
}


static inline 整数
posix_acl_xattr_count(size_t size)
{
        もし (size < sizeof(posix_acl_xattr_header))
                戻る -1;
        size -= sizeof(posix_acl_xattr_header);
        もし (size % sizeof(posix_acl_xattr_entry))
                戻る -1;
        戻る size / sizeof(posix_acl_xattr_entry);
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

	整数 count;

	もし (!value)
		戻る EINVAL;
	もし (size < sizeof(posix_acl_xattr_header))
		戻る ENOMEM;
	もし (header->a_version != ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION))
		戻る EINVAL;

	count = posix_acl_xattr_count(size);
	ext_acl = out_buf;
	ext_acl->a_version = ext2fs_cpu_to_le32(EXT4_ACL_VERSION);

	もし (count <= 0)
		戻る EINVAL;

	e = (char *) out_buf + sizeof(ext4_acl_header);
	s = sizeof(ext4_acl_header);
	ために (end = entry + count; entry != end;entry++) {
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
				戻る EINVAL;
		}
	}
	*size_out = s;
	戻る 0;
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

	もし ((!value) ||
	    (size < sizeof(ext4_acl_header)) ||
	    (ext_acl->a_version != ext2fs_cpu_to_le32(EXT4_ACL_VERSION)))
		戻る EINVAL;

	err = ext2fs_get_mem(size * 2, &out);
	もし (err)
		戻る err;

	header = (posix_acl_xattr_header *) out;
	header->a_version = ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION);
	entry = (posix_acl_xattr_entry *) (out + sizeof(posix_acl_xattr_header));

	cp = (const char *) value + sizeof(ext4_acl_header);
	size -= sizeof(ext4_acl_header);

	ながら (size > 0) {
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
				戻る EINVAL;
		}
		entry++;
	}
	*out_buf = out;
	*size_out = ((char *) entry - out);
	戻る 0;
}

static errcode_t
write_xattrs_to_buffer(ext2_filsys fs, struct ext2_xattr *attrs, 整数 count,
		       void *entries_start, unsigned 整数 storage_size,
		       unsigned 整数 value_offset_correction, 整数 write_hash)
{
	struct ext2_xattr *x;
	struct ext2_ext_attr_entry *e = entries_start;
	char *end = (char *) entries_start + storage_size;
	unsigned 整数 value_size;
	errcode_t err;

	memset(entries_start, 0, storage_size);
	ために (x = attrs; x < attrs + count; x++) {
		value_size = ((x->value_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = strlen(x->short_name);
		e->e_name_index = x->name_index;

		e->e_value_size = x->value_len;
		e->e_value_inum = x->ea_ino;

		/* Store name */
		memcpy((char *)e + sizeof(*e), x->short_name, e->e_name_len);
		もし (x->ea_ino) {
			e->e_value_offs = 0;
		} 否则 {
			end -= value_size;
			e->e_value_offs = end - (char *) entries_start +
						value_offset_correction;
			memcpy(end, x->value, e->e_value_size);
		}

		もし (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : end,
							  &e->e_hash);
			もし (err)
				戻る err;
		} 否则
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_NEXT(e);
		*(__u32 *)e = 0;
	}
	戻る 0;
}

errcode_t ext2fs_xattrs_write(struct ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	const unsigned 整数 inode_size = EXT2_INODE_SIZE(fs->super);
	struct ext2_inode_large *inode;
	char *start, *block_buf = NULL;
	struct ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	blk64_t blk;
	unsigned 整数 storage_size;
	unsigned 整数 i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_size;
	もし (i < sizeof(*inode))
		i = sizeof(*inode);
	err = ext2fs_get_memzero(i, &inode);
	もし (err)
		戻る err;

	err = ext2fs_read_inode_full(fs, handle->ino, EXT2_INODE(inode),
				     inode_size);
	もし (err)
		goto out;

	/* If extra_isize isn't set, we need to set it now */
	もし (inode->i_extra_isize == 0 &&
	    inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		char *p = (char *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		もし (extra == 0)
			extra = sizeof(__u32);
		memset(p + EXT2_GOOD_OLD_INODE_SIZE, 0, extra);
		inode->i_extra_isize = extra;
	}
	もし (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		goto out;
	}

	/* Does the inode have space for EA? */
	もし (inode->i_extra_isize < sizeof(inode->i_extra_isize) ||
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
	もし (err)
		goto out;
write_ea_block:
	/* Are we done? */
	もし (handle->ibody_count == handle->count &&
	    !ext2fs_file_acl_block(fs, EXT2_INODE(inode)))
		goto skip_ea_block;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &block_buf);
	もし (err)
		goto out;

	storage_size = fs->blocksize - sizeof(struct ext2_ext_attr_header);
	start = block_buf + sizeof(struct ext2_ext_attr_header);

	err = write_xattrs_to_buffer(fs, handle->attrs + handle->ibody_count,
				     handle->count - handle->ibody_count, start,
				     storage_size, start - block_buf, 1);
	もし (err)
		goto out2;

	/* Write a header on the EA block */
	header = (struct ext2_ext_attr_header *) block_buf;
	header->h_magic = EXT2_EXT_ATTR_MAGIC;
	header->h_refcount = 1;
	header->h_blocks = 1;

	/* Get a new block for writing */
	err = prep_ea_block_for_write(fs, handle->ino, inode);
	もし (err)
		goto out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_block(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, handle->ino);
	もし (err)
		goto out2;

skip_ea_block:
	blk = ext2fs_file_acl_block(fs, (struct ext2_inode *)inode);
	もし (!block_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		もし (err)
			goto out;
	}

	/* Write the inode */
	err = ext2fs_write_inode_full(fs, handle->ino, EXT2_INODE(inode),
				      inode_size);
	もし (err)
		goto out2;

out2:
	ext2fs_free_mem(&block_buf);
out:
	ext2fs_free_mem(&inode);
	戻る err;
}

static errcode_t read_xattrs_from_buffer(struct ext2_xattr_handle *handle,
					 struct ext2_inode_large *inode,
					 struct ext2_ext_attr_entry *entries,
					 unsigned 整数 storage_size,
					 char *value_start)
{
	struct ext2_xattr *x;
	struct ext2_ext_attr_entry *entry, *end;
	const char *prefix;
	unsigned 整数 remain, prefix_len;
	errcode_t err;
	unsigned 整数 values_size = storage_size +
			((char *)entries - value_start);

	/* find the end */
	end = entries;
	remain = storage_size;
	ながら (remain >= sizeof(struct ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(end)) {

		/* header eats this space */
		remain -= sizeof(struct ext2_ext_attr_entry);

		/* is attribute name valid? */
		もし (EXT2_EXT_ATTR_SIZE(end->e_name_len) > remain)
			戻る EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(end->e_name_len);
		end = EXT2_EXT_ATTR_NEXT(end);
	}

	entry = entries;
	remain = storage_size;
	ながら (remain >= sizeof(struct ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(entry)) {

		/* Allocate space for more attrs? */
		もし (handle->count == handle->capacity) {
			err = ext2fs_xattrs_expand(handle, 4);
			もし (err)
				戻る err;
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
		もし (err)
			戻る err;
		もし (prefix)
			memcpy(x->name, prefix, prefix_len);
		もし (entry->e_name_len)
			memcpy(x->name + prefix_len,
			       (char *)entry + sizeof(*entry),
			       entry->e_name_len);
		x->short_name = x->name + prefix_len;
		x->name_index = entry->e_name_index;

		/* Check & copy value */
		もし (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    entry->e_value_inum != 0)
			戻る EXT2_ET_BAD_EA_BLOCK_NUM;

		もし (entry->e_value_inum == 0) {
			もし (entry->e_value_size > remain)
				戻る EXT2_ET_EA_BAD_VALUE_SIZE;

			もし (entry->e_value_offs + entry->e_value_size > values_size)
				戻る EXT2_ET_EA_BAD_VALUE_OFFSET;

			もし (entry->e_value_size > 0 &&
			    value_start + entry->e_value_offs <
			    (char *)end + sizeof(__u32))
				戻る EXT2_ET_EA_BAD_VALUE_OFFSET;

			remain -= entry->e_value_size;

			err = ext2fs_get_mem(entry->e_value_size, &x->value);
			もし (err)
				戻る err;
			memcpy(x->value, value_start + entry->e_value_offs,
			       entry->e_value_size);
		} 否则 {
			struct ext2_inode *ea_inode;
			ext2_file_t ea_file;

			もし (entry->e_value_offs != 0)
				戻る EXT2_ET_EA_BAD_VALUE_OFFSET;

			もし (entry->e_value_size > (64 * 1024))
				戻る EXT2_ET_EA_BAD_VALUE_SIZE;

			err = ext2fs_get_mem(entry->e_value_size, &x->value);
			もし (err)
				戻る err;

			err = ext2fs_file_open(handle->fs, entry->e_value_inum,
					       0, &ea_file);
			もし (err)
				戻る err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			もし ((ea_inode->i_flags & EXT4_INLINE_DATA_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_count == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			否则 もし ((__u64) ext2fs_file_get_size(ea_file) !=
				 entry->e_value_size)
				err = EXT2_ET_EA_BAD_VALUE_SIZE;
			否则
				err = ext2fs_file_read(ea_file, x->value,
						       entry->e_value_size, 0);
			ext2fs_file_close(ea_file);
			もし (err)
				戻る err;
		}

		x->ea_ino = entry->e_value_inum;
		x->value_len = entry->e_value_size;

		/* e_hash may be 0 in older inode's ea */
		もし (entry->e_hash != 0) {
			__u32 hash, signed_hash;

			void *data = (entry->e_value_inum != 0) ?
					0 : value_start + entry->e_value_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, entry,
							  data, &hash,
							  &signed_hash);
			もし (err)
				戻る err;
			もし ((entry->e_hash != hash) &&
			    (entry->e_hash != signed_hash)) {
				struct ext2_inode child;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							entry->e_value_inum,
							&child);
				もし (err)
					戻る err;
				もし (child.i_mtime != handle->ino ||
				    child.i_generation != inode->i_generation)
					戻る EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->count++;
		entry = EXT2_EXT_ATTR_NEXT(entry);
	}

	戻る 0;
}

static void xattrs_free_keys(struct ext2_xattr_handle *h)
{
	struct ext2_xattr *a = h->attrs;
	整数 i;

	ために (i = 0; i < h->capacity; i++) {
		もし (a[i].name)
			ext2fs_free_mem(&a[i].name);
		もし (a[i].value)
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
	unsigned 整数 storage_size;
	char *start, *block_buf = NULL;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_keys(handle);

	/* Does the inode have space for EA? */
	もし (inode->i_extra_isize < sizeof(inode->i_extra_isize) ||
	    EXT2_INODE_SIZE(handle->fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  sizeof(__u32))
		goto read_ea_block;
	もし (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		goto out;
	}

	/* Look for EA in the inode */
	memcpy(&ea_inode_magic, ((char *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, sizeof(__u32));
	もし (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_size = EXT2_INODE_SIZE(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_SIZE - inode->i_extra_isize -
			sizeof(__u32);
		start = ((char *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + sizeof(__u32);

		err = read_xattrs_from_buffer(handle, inode,
					(struct ext2_ext_attr_entry *) start,
					storage_size, start);
		もし (err)
			goto out;

		handle->ibody_count = handle->count;
	}

read_ea_block:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_block(handle->fs, EXT2_INODE(inode));
	もし (blk != 0) {
		もし ((blk < handle->fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_count(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_NUM;
			goto out;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &block_buf);
		もし (err)
			goto out;

		err = ext2fs_read_ext_attr3(handle->fs, blk, block_buf,
					    handle->ino);
		もし (err)
			goto out3;

		/* We only know how to deal with v2 EA blocks */
		header = (struct ext2_ext_attr_header *) block_buf;
		もし (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
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
	もし (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	戻る err;
}

errcode_t ext2fs_xattrs_read(struct ext2_xattr_handle *handle)
{
	struct ext2_inode_large *inode;
	size_t inode_size = EXT2_INODE_SIZE(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	もし (inode_size < sizeof(*inode))
		inode_size = sizeof(*inode);
	err = ext2fs_get_memzero(inode_size, &inode);
	もし (err)
		戻る err;

	err = ext2fs_read_inode_full(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_SIZE(handle->fs->super));
	もし (err)
		goto out;

	err = ext2fs_xattrs_read_inode(handle, inode);

out:
	ext2fs_free_mem(&inode);

	戻る err;
}

errcode_t ext2fs_xattrs_iterate(struct ext2_xattr_handle *h,
				整数 (*func)(char *name, char *value,
					    size_t value_len, void *data),
				void *data)
{
	struct ext2_xattr *x;
	整数 dirty = 0;
	整数 ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	ために (x = h->attrs; x < h->attrs + h->count; x++) {
		ret = func(x->name, x->value, x->value_len, data);
		もし (ret & XATTR_CHANGED)
			dirty = 1;
		もし (ret & XATTR_ABORT)
			break;
	}

	もし (dirty)
		戻る ext2fs_xattrs_write(h);
	戻る 0;
}

errcode_t ext2fs_xattr_get(struct ext2_xattr_handle *h, const char *key,
			   void **value, size_t *value_len)
{
	struct ext2_xattr *x;
	char *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	ために (x = h->attrs; x < h->attrs + h->count; x++) {
		もし (strcmp(x->name, key))
			continue;

		もし (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(key, "system.posix_acl_default") == 0) ||
		     (strcmp(key, "system.posix_acl_access") == 0))) {
			err = convert_disk_buffer_to_posix_acl(x->value, x->value_len,
							       value, value_len);
			戻る err;
		} 否则 {
			err = ext2fs_get_mem(x->value_len, &val);
			もし (err)
				戻る err;
			memcpy(val, x->value, x->value_len);
			*value = val;
			*value_len = x->value_len;
			戻る 0;
		}
	}

	戻る EXT2_ET_EA_KEY_NOT_FOUND;
}

errcode_t ext2fs_xattr_inode_max_size(ext2_filsys fs, ext2_ino_t ino,
				      size_t *size)
{
	struct ext2_ext_attr_entry *entry;
	struct ext2_inode_large *inode;
	__u32 ea_inode_magic;
	unsigned 整数 minoff;
	char *start;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_SIZE(fs->super);
	もし (i < sizeof(*inode))
		i = sizeof(*inode);
	err = ext2fs_get_memzero(i, &inode);
	もし (err)
		戻る err;

	err = ext2fs_read_inode_full(fs, ino, (struct ext2_inode *)inode,
				     EXT2_INODE_SIZE(fs->super));
	もし (err)
		goto out;

	/* Does the inode have size for EA? */
	もし (EXT2_INODE_SIZE(fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  sizeof(__u32)) {
		err = EXT2_ET_INLINE_DATA_NO_SPACE;
		goto out;
	}

	minoff = EXT2_INODE_SIZE(fs->super) - sizeof(*inode) - sizeof(__u32);
	memcpy(&ea_inode_magic, ((char *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, sizeof(__u32));
	もし (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		start= ((char *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + sizeof(__u32);
		entry = (struct ext2_ext_attr_entry *) start;
		ながら (!EXT2_EXT_IS_LAST_ENTRY(entry)) {
			もし (!entry->e_value_inum && entry->e_value_size) {
				unsigned 整数 offs = entry->e_value_offs;
				もし (offs < minoff)
					minoff = offs;
			}
			entry = EXT2_EXT_ATTR_NEXT(entry);
		}
		*size = minoff - ((char *)entry - (char *)start) - sizeof(__u32);
	} 否则 {
		/* no xattr.  return a maximum size */
		*size = EXT2_EXT_ATTR_SIZE(minoff -
					   EXT2_EXT_ATTR_LEN(strlen("data")) -
					   EXT2_EXT_ATTR_ROUND - sizeof(__u32));
	}

out:
	ext2fs_free_mem(&inode);
	戻る err;
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
	もし (ret)
		戻る ret;

	memset(&inode, 0, sizeof(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	もし (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_size = 0;
	inode.i_mode = LINUX_S_IFREG | 0600;
	inode.i_links_count = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	もし (ret)
		戻る ret;
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
	もし (ret)
		戻る ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	もし (ret)
		戻る ret;
	ret = ext2fs_file_write(file, value, value_len, NULL);
	ext2fs_file_close(file);
	もし (ret)
		戻る ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	戻る 0;
}

static errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	struct ext2_inode_large inode;
	__u64 ref_count;
	errcode_t ret;

	ret = ext2fs_read_inode_full(fs, ino, (struct ext2_inode *)&inode,
				     sizeof(inode));
	もし (ret)
		goto out;

	ref_count = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_count--;
	ext2fs_set_ea_inode_ref(EXT2_INODE(&inode), ref_count);

	もし (ref_count)
		goto write_out;

	inode.i_links_count = 0;
	ext2fs_set_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	もし (ret)
		goto write_out;

	もし (ext2fs_inode_has_valid_blocks2(fs, (struct ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (struct ext2_inode *)&inode, NULL,
				   0, ~0ULL);
		もし (ret)
			goto out;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_out:
	ret = ext2fs_write_inode_full(fs, ino, (struct ext2_inode *)&inode,
				      sizeof(inode));
out:
	戻る ret;
}

static errcode_t xattr_update_entry(ext2_filsys fs, struct ext2_xattr *x,
				    const char *name, const char *short_name,
				    整数 index, const void *value,
				    size_t value_len, 整数 in_inode)
{
	ext2_ino_t ea_ino = 0;
	void *new_value = NULL;
	char *new_name = NULL;
	整数 name_len;
	errcode_t ret;

	もし (!x->name) {
		name_len = strlen(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		もし (ret)
			goto fail;
		memcpy(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(value_len, &new_value);
	もし (ret)
		goto fail;
	memcpy(new_value, value, value_len);

	もし (in_inode) {
		ret = xattr_create_ea_inode(fs, value, value_len, &ea_ino);
		もし (ret)
			goto fail;
	}

	もし (x->ea_ino) {
		ret = xattr_inode_dec_ref(fs, x->ea_ino);
		もし (ret)
			goto fail;
	}

	もし (!x->name) {
		x->name = new_name;
		x->short_name = new_name + (short_name  - name);
	}
	x->name_index = index;

	もし (x->value)
		ext2fs_free_mem(&x->value);
	x->value = new_value;
	x->value_len = value_len;
	x->ea_ino = ea_ino;
	戻る 0;
fail:
	もし (new_name)
		ext2fs_free_mem(&new_name);
	もし (new_value)
		ext2fs_free_mem(&new_value);
	もし (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	戻る ret;
}

static 整数 xattr_find_position(struct ext2_xattr *attrs, 整数 count,
			       const char *shortname, 整数 name_idx)
{
	struct ext2_xattr *x;
	整数 i;
	整数 shortname_len, x_shortname_len;

	shortname_len = strlen(shortname);

	ために (i = 0, x = attrs; i < count; i++, x++) {
		もし (name_idx < x->name_index)
			break;
		もし (name_idx > x->name_index)
			continue;

		x_shortname_len = strlen(x->short_name);
		もし (shortname_len < x_shortname_len)
			break;
		もし (shortname_len > x_shortname_len)
			continue;

		もし (memcmp(shortname, x->short_name, shortname_len) <= 0)
			break;
	}
	戻る i;
}

static errcode_t xattr_array_update(struct ext2_xattr_handle *h,
				    const char *name,
				    const void *value, size_t value_len,
				    整数 ibody_free, 整数 block_free,
				    整数 old_idx, 整数 in_inode)
{
	struct ext2_xattr tmp;
	整数 add_to_ibody;
	整数 needed;
	整数 name_len, name_idx = 0;
	const char *shortname = name;
	整数 new_idx;
	整数 ret;

	find_ea_index(name, &shortname, &name_idx);
	name_len = strlen(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	もし (!in_inode)
		needed += EXT2_EXT_ATTR_SIZE(value_len);

	もし (old_idx >= 0 && old_idx < h->ibody_count) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		もし (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_SIZE(
						h->attrs[old_idx].value_len);
	}

	もし (needed <= ibody_free) {
		もし (old_idx < 0) {
			new_idx = h->ibody_count;
			add_to_ibody = 1;
			goto add_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, value,
					 value_len, in_inode);
		もし (ret)
			戻る ret;
		もし (h->ibody_count <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_count + 1,
				h->attrs + h->ibody_count,
				(old_idx - h->ibody_count) * sizeof(*h->attrs));
			h->attrs[h->ibody_count] = tmp;
			h->ibody_count++;
		}
		戻る 0;
	}

	もし (h->ibody_count <= old_idx) {
		block_free += EXT2_EXT_ATTR_LEN(name_len);
		もし (!h->attrs[old_idx].ea_ino)
			block_free +=
				EXT2_EXT_ATTR_SIZE(h->attrs[old_idx].value_len);
	}

	もし (needed > block_free)
		戻る EXT2_ET_EA_NO_SPACE;

	もし (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, value,
					 value_len, in_inode);
		もし (ret)
			戻る ret;
		もし (old_idx < h->ibody_count) {
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
		戻る 0;
	}

	new_idx = xattr_find_position(h->attrs + h->ibody_count,
				      h->count - h->ibody_count,
				      shortname, name_idx);
	new_idx += h->ibody_count;
	add_to_ibody = 0;

add_new:
	もし (h->count == h->capacity) {
		ret = ext2fs_xattrs_expand(h, 4);
		もし (ret)
			戻る ret;
	}

	ret = xattr_update_entry(h->fs, &h->attrs[h->count], name, shortname,
				 name_idx, value, value_len, in_inode);
	もし (ret)
		戻る ret;

	tmp = h->attrs[h->count];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->count - new_idx)*sizeof(*h->attrs));
	h->attrs[new_idx] = tmp;
	もし (add_to_ibody)
		h->ibody_count++;
	h->count++;
	戻る 0;
}

static 整数 space_used(struct ext2_xattr *attrs, 整数 count)
{
	整数 total = 0;
	struct ext2_xattr *x;
	整数 i, len;

	ために (i = 0, x = attrs; i < count; i++, x++) {
		len = strlen(x->short_name);
		total += EXT2_EXT_ATTR_LEN(len);
		もし (!x->ea_ino)
			total += EXT2_EXT_ATTR_SIZE(x->value_len);
	}
	戻る total;
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
	const 整数 inode_size = EXT2_INODE_SIZE(fs->super);
	struct ext2_inode_large *inode = NULL;
	struct ext2_xattr *x;
	char *new_value;
	整数 ibody_free, block_free;
	整数 in_inode = 0;
	整数 old_idx = -1;
	整数 extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(value_len, &new_value);
	もし (ret)
		戻る ret;
	もし (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((strcmp(name, "system.posix_acl_default") == 0) ||
	     (strcmp(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_to_disk_buffer(value, value_len,
						       new_value, &value_len);
		もし (ret)
			goto out;
	} 否则 もし (value_len)
		memcpy(new_value, value, value_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	ために (x = h->attrs; x < h->attrs + h->count; x++) {
		もし (!strcmp(x->name, name)) {
			もし (!x->ea_ino && x->value_len == value_len &&
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
	もし (ret)
		goto out;
	ret = ext2fs_read_inode_full(fs, h->ino,
				     (struct ext2_inode *)inode,
				     inode_size);
	もし (ret)
		goto out;
	もし (inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		extra_isize = inode->i_extra_isize;
		もし (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			もし (extra_isize == 0)
				extra_isize = sizeof(__u32);
		}
		ibody_free = inode_size - EXT2_GOOD_OLD_INODE_SIZE;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= sizeof(__u32) * 2;
		ibody_free -= space_used(h->attrs, h->ibody_count);
	} 否则
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	もし (strcmp(name, "system.data") == 0) {
		もし (h->ibody_count <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			goto out;
		}
		ret = xattr_array_update(h, name, new_value, value_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		もし (ret)
			goto out;
		goto write_out;
	}

	block_free = fs->blocksize;
	block_free -= sizeof(struct ext2_ext_attr_header);
	/* Final null entry. */
	block_free -= sizeof(__u32);
	block_free -= space_used(h->attrs + h->ibody_count,
				 h->count - h->ibody_count);

	もし (ext2fs_has_feature_ea_inode(fs->super) &&
	    value_len > EXT4_XATTR_MIN_LARGE_EA_SIZE(fs->blocksize))
		in_inode = 1;

	ret = xattr_array_update(h, name, new_value, value_len, ibody_free,
				 block_free, old_idx, in_inode);
	もし (ret == EXT2_ET_EA_NO_SPACE && !in_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_value, value_len,
			ibody_free, block_free, old_idx, 1 /* in_inode */);
	もし (ret)
		goto out;

write_out:
	ret = ext2fs_xattrs_write(h);
out:
	もし (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_value);
	戻る ret;
}

errcode_t ext2fs_xattr_remove(struct ext2_xattr_handle *handle,
			      const char *key)
{
	struct ext2_xattr *x;
	struct ext2_xattr *end = handle->attrs + handle->count;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	ために (x = handle->attrs; x < end; x++) {
		もし (strcmp(x->name, key) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->value);
			もし (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (end - x - 1)*sizeof(*x));
			memset(end - 1, 0, sizeof(*end));
			もし (x < handle->attrs + handle->ibody_count)
				handle->ibody_count--;
			handle->count--;
			戻る ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	戻る 0;
}

errcode_t ext2fs_xattr_remove_all(struct ext2_xattr_handle *handle)
{
	struct ext2_xattr *x;
	struct ext2_xattr *end = handle->attrs + handle->count;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	ために (x = handle->attrs; x < end; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->value);
		もし (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_count = 0;
	handle->count = 0;
	戻る ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     struct ext2_xattr_handle **handle)
{
	struct ext2_xattr_handle *h;
	errcode_t err;

	もし (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_data(fs->super))
		戻る EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(sizeof(*h), &h);
	もし (err)
		戻る err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->capacity = 4;
	err = ext2fs_get_arrayzero(h->capacity, sizeof(struct ext2_xattr),
				   &h->attrs);
	もし (err) {
		ext2fs_free_mem(&h);
		戻る err;
	}
	h->count = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	戻る 0;
}

errcode_t ext2fs_xattrs_close(struct ext2_xattr_handle **handle)
{
	struct ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_keys(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	戻る 0;
}

errcode_t ext2fs_xattrs_count(struct ext2_xattr_handle *handle, size_t *count)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*count = handle->count;
	戻る 0;
}

errcode_t ext2fs_xattrs_flags(struct ext2_xattr_handle *handle,
			      unsigned 整数 *new_flags, unsigned 整数 *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	もし (old_flags)
		*old_flags = handle->flags;
	もし (new_flags)
		handle->flags = *new_flags;
	戻る 0;
}
