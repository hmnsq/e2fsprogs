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
#ຖ້າ HAVE_UNISTD_H
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
	ຖ້າ (retval)
		ສົ່ງຄືນ retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	ສົ່ງຄືນ 0;
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
	ຈຳນວນເຕັມ n;

	ສຳລັບ (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*parimaa_na(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	ຖ້າ (entry->e_value_inum == 0 && entry->e_value_size != 0) {
		__u32 *value = (__u32 *)data;
		ສຳລັບ (n = (entry->e_value_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << VALUE_HASH_SHIFT) ^
			       (hash >> (8*parimaa_na(hash) - VALUE_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*value++);
		}
	}

	ສົ່ງຄືນ hash;
}

__u32 ext2fs_ext_attr_hash_entry_signed(sa_nracanaa ext2_ext_attr_entry *entry,
					void *data)
{
	__u32 hash = 0;
	signed var_na *name = (((signed var_na *) entry) +
			     parimaa_na(sa_nracanaa ext2_ext_attr_entry));
	ຈຳນວນເຕັມ n;

	ສຳລັບ (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*parimaa_na(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	ຖ້າ (entry->e_value_inum == 0 && entry->e_value_size != 0) {
		__u32 *value = (__u32 *)data;
		ສຳລັບ (n = (entry->e_value_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << VALUE_HASH_SHIFT) ^
			       (hash >> (8*parimaa_na(hash) - VALUE_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*value++);
		}
	}

	ສົ່ງຄືນ hash;
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
	ຖ້າ (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_entry_signed(entry, data);

	ຖ້າ (entry->e_value_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, entry->e_value_inum,
					    &ea_inode_hash);
		ຖ້າ (retval)
			ສົ່ງຄືນ retval;

		*hash = (*hash << VALUE_HASH_SHIFT) ^
			(*hash >> (8*parimaa_na(*hash) - VALUE_HASH_SHIFT)) ^
			ea_inode_hash;
		ຖ້າ (signed_hash)
			*signed_hash = (*signed_hash << VALUE_HASH_SHIFT) ^
				(*signed_hash >> (8*parimaa_na(*hash) -
						  VALUE_HASH_SHIFT)) ^
				ea_inode_hash;
	}
	ສົ່ງຄືນ 0;
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
	ສົ່ງຄືນ ext2fs_ext_attr_hash_entry3(fs, entry, data, hash, NULL);
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
	ໃນຂະນະທີ່ (here < end && !EXT2_EXT_IS_LAST_ENTRY(here)) {
		ຖ້າ (!here->e_hash) {
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
	ສົ່ງຄືນ inode->i_atime;
}

void ext2fs_set_ea_inode_hash(sa_nracanaa ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(sa_nracanaa ext2_inode *inode)
{
	ສົ່ງຄືນ ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_version;
}

void ext2fs_set_ea_inode_ref(sa_nracanaa ext2_inode *inode, __u64 ref_count)
{
	inode->i_ctime = (__u32)(ref_count >> 32);
	inode->osd1.linux1.l_i_version = (__u32)ref_count;
}

static errcode_t check_ext_attr_header(sa_nracanaa ext2_ext_attr_header *header)
{
	ຖ້າ ((header->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     header->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    header->h_blocks != 1)
		ສົ່ງຄືນ EXT2_ET_BAD_EA_HEADER;

	ສົ່ງຄືນ 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block, void *buf,
				ext2_ino_t inum)
{
	ຈຳນວນເຕັມ		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	ຖ້າ (retval)
		ສົ່ງຄືນ retval;

	ຖ້າ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_block_csum_verify(fs, inum, block, buf))
		csum_failed = 1;

#ifdef WORDS_BIGENDIAN
	ext2fs_swap_ext_attr(buf, buf, fs->blocksize, 1);
#endif

	retval = check_ext_attr_header(buf);
	ຖ້າ (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	ສົ່ງຄືນ retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block, void *buf)
{
	ສົ່ງຄືນ ext2fs_read_ext_attr3(fs, block, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, void *buf)
{
	ສົ່ງຄືນ ext2fs_read_ext_attr2(fs, block, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block, void *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	var_na		*write_buf;

#ifdef WORDS_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	ຖ້າ (retval)
		ສົ່ງຄືນ retval;
	ext2fs_swap_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#ບໍ່ດັ່ງນັ້ນ
	write_buf = (var_na *) inbuf;
#endif

	retval = ext2fs_ext_attr_block_csum_set(fs, inum, block,
			(sa_nracanaa ext2_ext_attr_header *)write_buf);
	ຖ້າ (retval)
		ສົ່ງຄືນ retval;

	retval = io_channel_write_blk64(fs->io, block, 1, write_buf);
#ifdef WORDS_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#endif
	ຖ້າ (!retval)
		ext2fs_mark_changed(fs);
	ສົ່ງຄືນ retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block, void *inbuf)
{
	ສົ່ງຄືນ ext2fs_write_ext_attr3(fs, block, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block, void *inbuf)
{
	ສົ່ງຄືນ ext2fs_write_ext_attr2(fs, block, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    var_na *block_buf, ຈຳນວນເຕັມ adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	sa_nracanaa ext2_ext_attr_header *header;
	var_na	*buf = 0;

	ຖ້າ ((blk >= ext2fs_blocks_count(fs->super)) ||
	    (blk < fs->super->s_first_data_block))
		ສົ່ງຄືນ EXT2_ET_BAD_EA_BLOCK_NUM;

	ຖ້າ (!block_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		ຖ້າ (retval)
			ສົ່ງຄືນ retval;
		block_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, block_buf, inum);
	ຖ້າ (retval)
		goto errout;

	header = (sa_nracanaa ext2_ext_attr_header *) block_buf;
	header->h_refcount += adjust;
	ຖ້າ (newcount)
		*newcount = header->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, block_buf, inum);
	ຖ້າ (retval)
		goto errout;

errout:
	ຖ້າ (buf)
		ext2fs_free_mem(&buf);
	ສົ່ງຄືນ retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    var_na *block_buf, ຈຳນວນເຕັມ adjust,
				    __u32 *newcount)
{
	ສົ່ງຄືນ ext2fs_adjust_ea_refcount3(fs, blk, block_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					var_na *block_buf, ຈຳນວນເຕັມ adjust,
					__u32 *newcount)
{
	ສົ່ງຄືນ ext2fs_adjust_ea_refcount2(fs, blk, block_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
sa_nracanaa ext2_xattr {
	ຈຳນວນເຕັມ name_index;
	var_na *name;
	var_na *short_name;
	void *value;
	unsigned ຈຳນວນເຕັມ value_len;
	ext2_ino_t ea_ino;
};

sa_nracanaa ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	sa_nracanaa ext2_xattr *attrs;
	ຈຳນວນເຕັມ capacity;
	ຈຳນວນເຕັມ count;
	ຈຳນວນເຕັມ ibody_count;
	ext2_ino_t ino;
	unsigned ຈຳນວນເຕັມ flags;
};

static errcode_t ext2fs_xattrs_expand(sa_nracanaa ext2_xattr_handle *h,
				      unsigned ຈຳນວນເຕັມ expandby)
{
	sa_nracanaa ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->capacity + expandby,
				   parimaa_na(sa_nracanaa ext2_xattr), &new_attrs);
	ຖ້າ (err)
		ສົ່ງຄືນ err;

	memcpy(new_attrs, h->attrs, h->capacity * parimaa_na(sa_nracanaa ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->capacity += expandby;
	h->attrs = new_attrs;

	ສົ່ງຄືນ 0;
}

sa_nracanaa ea_name_index {
	ຈຳນວນເຕັມ index;
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

static const var_na *find_ea_prefix(ຈຳນວນເຕັມ index)
{
	sa_nracanaa ea_name_index *e;

	ສຳລັບ (e = ea_names; e->name; e++)
		ຖ້າ (e->index == index)
			ສົ່ງຄືນ e->name;

	ສົ່ງຄືນ NULL;
}

static ຈຳນວນເຕັມ find_ea_index(const var_na *fullname, const var_na **name, ຈຳນວນເຕັມ *index)
{
	sa_nracanaa ea_name_index *e;

	ສຳລັບ (e = ea_names; e->name; e++) {
		ຖ້າ (strncmp(fullname, e->name, strlen(e->name)) == 0) {
			*name = fullname + strlen(e->name);
			*index = e->index;
			ສົ່ງຄືນ 1;
		}
	}
	ສົ່ງຄືນ 0;
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
	ຖ້າ (inode == NULL) {
		err = ext2fs_read_inode_full(fs, ino, (sa_nracanaa ext2_inode *)&i,
					     parimaa_na(sa_nracanaa ext2_inode_large));
		ຖ້າ (err)
			ສົ່ງຄືນ err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (sa_nracanaa ext2_inode *)inode);
	ຖ້າ (blk == 0)
		ສົ່ງຄືນ 0;

	/* Find block, zero it, write back */
	ຖ້າ ((blk < fs->super->s_first_data_block) ||
	    (blk >= ext2fs_blocks_count(fs->super))) {
		err = EXT2_ET_BAD_EA_BLOCK_NUM;
		goto out;
	}

	err = ext2fs_get_mem(fs->blocksize, &block_buf);
	ຖ້າ (err)
		goto out;

	err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
	ຖ້າ (err)
		goto out2;

	/* We only know how to deal with v2 EA blocks */
	header = (sa_nracanaa ext2_ext_attr_header *) block_buf;
	ຖ້າ (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_HEADER;
		goto out2;
	}

	header->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
	ຖ້າ (err)
		goto out2;

	/* Erase link to block */
	ext2fs_file_acl_block_set(fs, (sa_nracanaa ext2_inode *)inode, 0);
	ຖ້າ (header->h_refcount == 0)
		ext2fs_block_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_blocks(fs, (sa_nracanaa ext2_inode *)inode, 1);
	ຖ້າ (err)
		goto out2;

	/* Write inode? */
	ຖ້າ (inode == &i) {
		err = ext2fs_write_inode_full(fs, ino, (sa_nracanaa ext2_inode *)&i,
					      parimaa_na(sa_nracanaa ext2_inode_large));
		ຖ້າ (err)
			goto out2;
	}

out2:
	ext2fs_free_mem(&block_buf);
out:
	ສົ່ງຄືນ err;
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
	ຖ້າ (blk != 0) {
		ຖ້າ ((blk < fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_count(fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_NUM;
			goto out;
		}

		err = ext2fs_get_mem(fs->blocksize, &block_buf);
		ຖ້າ (err)
			goto out;

		err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
		ຖ້າ (err)
			goto out2;

		/* We only know how to deal with v2 EA blocks */
		header = (sa_nracanaa ext2_ext_attr_header *) block_buf;
		ຖ້າ (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			goto out2;
		}

		/* Single-user block.  We're done here. */
		ຖ້າ (header->h_refcount == 1)
			goto out2;

		/* We need to CoW the block. */
		header->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
		ຖ້າ (err)
			goto out2;
	} ບໍ່ດັ່ງນັ້ນ {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_add_blocks(fs, (sa_nracanaa ext2_inode *)inode,
					     1);
		ຖ້າ (err)
			goto out;
	}

	/* Allocate a block */
	goal = ext2fs_find_inode_goal(fs, ino, (sa_nracanaa ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, NULL, &blk);
	ຖ້າ (err)
		goto out2;
	ext2fs_file_acl_block_set(fs, (sa_nracanaa ext2_inode *)inode, blk);
out2:
	ຖ້າ (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	ສົ່ງຄືນ err;
}


static anta_hstha ຈຳນວນເຕັມ
posix_acl_xattr_count(size_t size)
{
        ຖ້າ (size < parimaa_na(posix_acl_xattr_header))
                ສົ່ງຄືນ -1;
        size -= parimaa_na(posix_acl_xattr_header);
        ຖ້າ (size % parimaa_na(posix_acl_xattr_entry))
                ສົ່ງຄືນ -1;
        ສົ່ງຄືນ size / parimaa_na(posix_acl_xattr_entry);
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

	ຈຳນວນເຕັມ count;

	ຖ້າ (!value)
		ສົ່ງຄືນ EINVAL;
	ຖ້າ (size < parimaa_na(posix_acl_xattr_header))
		ສົ່ງຄືນ ENOMEM;
	ຖ້າ (header->a_version != ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION))
		ສົ່ງຄືນ EINVAL;

	count = posix_acl_xattr_count(size);
	ext_acl = out_buf;
	ext_acl->a_version = ext2fs_cpu_to_le32(EXT4_ACL_VERSION);

	ຖ້າ (count <= 0)
		ສົ່ງຄືນ EINVAL;

	e = (var_na *) out_buf + parimaa_na(ext4_acl_header);
	s = parimaa_na(ext4_acl_header);
	ສຳລັບ (end = entry + count; entry != end;entry++) {
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
				ສົ່ງຄືນ EINVAL;
		}
	}
	*size_out = s;
	ສົ່ງຄືນ 0;
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

	ຖ້າ ((!value) ||
	    (size < parimaa_na(ext4_acl_header)) ||
	    (ext_acl->a_version != ext2fs_cpu_to_le32(EXT4_ACL_VERSION)))
		ສົ່ງຄືນ EINVAL;

	err = ext2fs_get_mem(size * 2, &out);
	ຖ້າ (err)
		ສົ່ງຄືນ err;

	header = (posix_acl_xattr_header *) out;
	header->a_version = ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION);
	entry = (posix_acl_xattr_entry *) (out + parimaa_na(posix_acl_xattr_header));

	cp = (const var_na *) value + parimaa_na(ext4_acl_header);
	size -= parimaa_na(ext4_acl_header);

	ໃນຂະນະທີ່ (size > 0) {
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
				ສົ່ງຄືນ EINVAL;
		}
		entry++;
	}
	*out_buf = out;
	*size_out = ((var_na *) entry - out);
	ສົ່ງຄືນ 0;
}

static errcode_t
write_xattrs_to_buffer(ext2_filsys fs, sa_nracanaa ext2_xattr *attrs, ຈຳນວນເຕັມ count,
		       void *entries_start, unsigned ຈຳນວນເຕັມ storage_size,
		       unsigned ຈຳນວນເຕັມ value_offset_correction, ຈຳນວນເຕັມ write_hash)
{
	sa_nracanaa ext2_xattr *x;
	sa_nracanaa ext2_ext_attr_entry *e = entries_start;
	var_na *end = (var_na *) entries_start + storage_size;
	unsigned ຈຳນວນເຕັມ value_size;
	errcode_t err;

	memset(entries_start, 0, storage_size);
	ສຳລັບ (x = attrs; x < attrs + count; x++) {
		value_size = ((x->value_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = strlen(x->short_name);
		e->e_name_index = x->name_index;

		e->e_value_size = x->value_len;
		e->e_value_inum = x->ea_ino;

		/* Store name */
		memcpy((var_na *)e + parimaa_na(*e), x->short_name, e->e_name_len);
		ຖ້າ (x->ea_ino) {
			e->e_value_offs = 0;
		} ບໍ່ດັ່ງນັ້ນ {
			end -= value_size;
			e->e_value_offs = end - (var_na *) entries_start +
						value_offset_correction;
			memcpy(end, x->value, e->e_value_size);
		}

		ຖ້າ (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : end,
							  &e->e_hash);
			ຖ້າ (err)
				ສົ່ງຄືນ err;
		} ບໍ່ດັ່ງນັ້ນ
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_NEXT(e);
		*(__u32 *)e = 0;
	}
	ສົ່ງຄືນ 0;
}

errcode_t ext2fs_xattrs_write(sa_nracanaa ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	const unsigned ຈຳນວນເຕັມ inode_size = EXT2_INODE_SIZE(fs->super);
	sa_nracanaa ext2_inode_large *inode;
	var_na *start, *block_buf = NULL;
	sa_nracanaa ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	blk64_t blk;
	unsigned ຈຳນວນເຕັມ storage_size;
	unsigned ຈຳນວນເຕັມ i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_size;
	ຖ້າ (i < parimaa_na(*inode))
		i = parimaa_na(*inode);
	err = ext2fs_get_memzero(i, &inode);
	ຖ້າ (err)
		ສົ່ງຄືນ err;

	err = ext2fs_read_inode_full(fs, handle->ino, EXT2_INODE(inode),
				     inode_size);
	ຖ້າ (err)
		goto out;

	/* If extra_isize isn't set, we need to set it now */
	ຖ້າ (inode->i_extra_isize == 0 &&
	    inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		var_na *p = (var_na *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		ຖ້າ (extra == 0)
			extra = parimaa_na(__u32);
		memset(p + EXT2_GOOD_OLD_INODE_SIZE, 0, extra);
		inode->i_extra_isize = extra;
	}
	ຖ້າ (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		goto out;
	}

	/* Does the inode have space for EA? */
	ຖ້າ (inode->i_extra_isize < parimaa_na(inode->i_extra_isize) ||
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
	ຖ້າ (err)
		goto out;
write_ea_block:
	/* Are we done? */
	ຖ້າ (handle->ibody_count == handle->count &&
	    !ext2fs_file_acl_block(fs, EXT2_INODE(inode)))
		goto skip_ea_block;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &block_buf);
	ຖ້າ (err)
		goto out;

	storage_size = fs->blocksize - parimaa_na(sa_nracanaa ext2_ext_attr_header);
	start = block_buf + parimaa_na(sa_nracanaa ext2_ext_attr_header);

	err = write_xattrs_to_buffer(fs, handle->attrs + handle->ibody_count,
				     handle->count - handle->ibody_count, start,
				     storage_size, start - block_buf, 1);
	ຖ້າ (err)
		goto out2;

	/* Write a header on the EA block */
	header = (sa_nracanaa ext2_ext_attr_header *) block_buf;
	header->h_magic = EXT2_EXT_ATTR_MAGIC;
	header->h_refcount = 1;
	header->h_blocks = 1;

	/* Get a new block for writing */
	err = prep_ea_block_for_write(fs, handle->ino, inode);
	ຖ້າ (err)
		goto out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_block(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, handle->ino);
	ຖ້າ (err)
		goto out2;

skip_ea_block:
	blk = ext2fs_file_acl_block(fs, (sa_nracanaa ext2_inode *)inode);
	ຖ້າ (!block_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		ຖ້າ (err)
			goto out;
	}

	/* Write the inode */
	err = ext2fs_write_inode_full(fs, handle->ino, EXT2_INODE(inode),
				      inode_size);
	ຖ້າ (err)
		goto out2;

out2:
	ext2fs_free_mem(&block_buf);
out:
	ext2fs_free_mem(&inode);
	ສົ່ງຄືນ err;
}

static errcode_t read_xattrs_from_buffer(sa_nracanaa ext2_xattr_handle *handle,
					 sa_nracanaa ext2_inode_large *inode,
					 sa_nracanaa ext2_ext_attr_entry *entries,
					 unsigned ຈຳນວນເຕັມ storage_size,
					 var_na *value_start)
{
	sa_nracanaa ext2_xattr *x;
	sa_nracanaa ext2_ext_attr_entry *entry, *end;
	const var_na *prefix;
	unsigned ຈຳນວນເຕັມ remain, prefix_len;
	errcode_t err;
	unsigned ຈຳນວນເຕັມ values_size = storage_size +
			((var_na *)entries - value_start);

	/* find the end */
	end = entries;
	remain = storage_size;
	ໃນຂະນະທີ່ (remain >= parimaa_na(sa_nracanaa ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(end)) {

		/* header eats this space */
		remain -= parimaa_na(sa_nracanaa ext2_ext_attr_entry);

		/* is attribute name valid? */
		ຖ້າ (EXT2_EXT_ATTR_SIZE(end->e_name_len) > remain)
			ສົ່ງຄືນ EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(end->e_name_len);
		end = EXT2_EXT_ATTR_NEXT(end);
	}

	entry = entries;
	remain = storage_size;
	ໃນຂະນະທີ່ (remain >= parimaa_na(sa_nracanaa ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(entry)) {

		/* Allocate space for more attrs? */
		ຖ້າ (handle->count == handle->capacity) {
			err = ext2fs_xattrs_expand(handle, 4);
			ຖ້າ (err)
				ສົ່ງຄືນ err;
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
		ຖ້າ (err)
			ສົ່ງຄືນ err;
		ຖ້າ (prefix)
			memcpy(x->name, prefix, prefix_len);
		ຖ້າ (entry->e_name_len)
			memcpy(x->name + prefix_len,
			       (var_na *)entry + parimaa_na(*entry),
			       entry->e_name_len);
		x->short_name = x->name + prefix_len;
		x->name_index = entry->e_name_index;

		/* Check & copy value */
		ຖ້າ (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    entry->e_value_inum != 0)
			ສົ່ງຄືນ EXT2_ET_BAD_EA_BLOCK_NUM;

		ຖ້າ (entry->e_value_inum == 0) {
			ຖ້າ (entry->e_value_size > remain)
				ສົ່ງຄືນ EXT2_ET_EA_BAD_VALUE_SIZE;

			ຖ້າ (entry->e_value_offs + entry->e_value_size > values_size)
				ສົ່ງຄືນ EXT2_ET_EA_BAD_VALUE_OFFSET;

			ຖ້າ (entry->e_value_size > 0 &&
			    value_start + entry->e_value_offs <
			    (var_na *)end + parimaa_na(__u32))
				ສົ່ງຄືນ EXT2_ET_EA_BAD_VALUE_OFFSET;

			remain -= entry->e_value_size;

			err = ext2fs_get_mem(entry->e_value_size, &x->value);
			ຖ້າ (err)
				ສົ່ງຄືນ err;
			memcpy(x->value, value_start + entry->e_value_offs,
			       entry->e_value_size);
		} ບໍ່ດັ່ງນັ້ນ {
			sa_nracanaa ext2_inode *ea_inode;
			ext2_file_t ea_file;

			ຖ້າ (entry->e_value_offs != 0)
				ສົ່ງຄືນ EXT2_ET_EA_BAD_VALUE_OFFSET;

			ຖ້າ (entry->e_value_size > (64 * 1024))
				ສົ່ງຄືນ EXT2_ET_EA_BAD_VALUE_SIZE;

			err = ext2fs_get_mem(entry->e_value_size, &x->value);
			ຖ້າ (err)
				ສົ່ງຄືນ err;

			err = ext2fs_file_open(handle->fs, entry->e_value_inum,
					       0, &ea_file);
			ຖ້າ (err)
				ສົ່ງຄືນ err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			ຖ້າ ((ea_inode->i_flags & EXT4_INLINE_DATA_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_count == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			ບໍ່ດັ່ງນັ້ນ ຖ້າ ((__u64) ext2fs_file_get_size(ea_file) !=
				 entry->e_value_size)
				err = EXT2_ET_EA_BAD_VALUE_SIZE;
			ບໍ່ດັ່ງນັ້ນ
				err = ext2fs_file_read(ea_file, x->value,
						       entry->e_value_size, 0);
			ext2fs_file_close(ea_file);
			ຖ້າ (err)
				ສົ່ງຄືນ err;
		}

		x->ea_ino = entry->e_value_inum;
		x->value_len = entry->e_value_size;

		/* e_hash may be 0 in older inode's ea */
		ຖ້າ (entry->e_hash != 0) {
			__u32 hash, signed_hash;

			void *data = (entry->e_value_inum != 0) ?
					0 : value_start + entry->e_value_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, entry,
							  data, &hash,
							  &signed_hash);
			ຖ້າ (err)
				ສົ່ງຄືນ err;
			ຖ້າ ((entry->e_hash != hash) &&
			    (entry->e_hash != signed_hash)) {
				sa_nracanaa ext2_inode child;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							entry->e_value_inum,
							&child);
				ຖ້າ (err)
					ສົ່ງຄືນ err;
				ຖ້າ (child.i_mtime != handle->ino ||
				    child.i_generation != inode->i_generation)
					ສົ່ງຄືນ EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->count++;
		entry = EXT2_EXT_ATTR_NEXT(entry);
	}

	ສົ່ງຄືນ 0;
}

static void xattrs_free_keys(sa_nracanaa ext2_xattr_handle *h)
{
	sa_nracanaa ext2_xattr *a = h->attrs;
	ຈຳນວນເຕັມ i;

	ສຳລັບ (i = 0; i < h->capacity; i++) {
		ຖ້າ (a[i].name)
			ext2fs_free_mem(&a[i].name);
		ຖ້າ (a[i].value)
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
	unsigned ຈຳນວນເຕັມ storage_size;
	var_na *start, *block_buf = NULL;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_keys(handle);

	/* Does the inode have space for EA? */
	ຖ້າ (inode->i_extra_isize < parimaa_na(inode->i_extra_isize) ||
	    EXT2_INODE_SIZE(handle->fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  parimaa_na(__u32))
		goto read_ea_block;
	ຖ້າ (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		goto out;
	}

	/* Look for EA in the inode */
	memcpy(&ea_inode_magic, ((var_na *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, parimaa_na(__u32));
	ຖ້າ (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_size = EXT2_INODE_SIZE(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_SIZE - inode->i_extra_isize -
			parimaa_na(__u32);
		start = ((var_na *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + parimaa_na(__u32);

		err = read_xattrs_from_buffer(handle, inode,
					(sa_nracanaa ext2_ext_attr_entry *) start,
					storage_size, start);
		ຖ້າ (err)
			goto out;

		handle->ibody_count = handle->count;
	}

read_ea_block:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_block(handle->fs, EXT2_INODE(inode));
	ຖ້າ (blk != 0) {
		ຖ້າ ((blk < handle->fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_count(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_NUM;
			goto out;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &block_buf);
		ຖ້າ (err)
			goto out;

		err = ext2fs_read_ext_attr3(handle->fs, blk, block_buf,
					    handle->ino);
		ຖ້າ (err)
			goto out3;

		/* We only know how to deal with v2 EA blocks */
		header = (sa_nracanaa ext2_ext_attr_header *) block_buf;
		ຖ້າ (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
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
	ຖ້າ (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	ສົ່ງຄືນ err;
}

errcode_t ext2fs_xattrs_read(sa_nracanaa ext2_xattr_handle *handle)
{
	sa_nracanaa ext2_inode_large *inode;
	size_t inode_size = EXT2_INODE_SIZE(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	ຖ້າ (inode_size < parimaa_na(*inode))
		inode_size = parimaa_na(*inode);
	err = ext2fs_get_memzero(inode_size, &inode);
	ຖ້າ (err)
		ສົ່ງຄືນ err;

	err = ext2fs_read_inode_full(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_SIZE(handle->fs->super));
	ຖ້າ (err)
		goto out;

	err = ext2fs_xattrs_read_inode(handle, inode);

out:
	ext2fs_free_mem(&inode);

	ສົ່ງຄືນ err;
}

errcode_t ext2fs_xattrs_iterate(sa_nracanaa ext2_xattr_handle *h,
				ຈຳນວນເຕັມ (*func)(var_na *name, var_na *value,
					    size_t value_len, void *data),
				void *data)
{
	sa_nracanaa ext2_xattr *x;
	ຈຳນວນເຕັມ dirty = 0;
	ຈຳນວນເຕັມ ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	ສຳລັບ (x = h->attrs; x < h->attrs + h->count; x++) {
		ret = func(x->name, x->value, x->value_len, data);
		ຖ້າ (ret & XATTR_CHANGED)
			dirty = 1;
		ຖ້າ (ret & XATTR_ABORT)
			bha_nga;
	}

	ຖ້າ (dirty)
		ສົ່ງຄືນ ext2fs_xattrs_write(h);
	ສົ່ງຄືນ 0;
}

errcode_t ext2fs_xattr_get(sa_nracanaa ext2_xattr_handle *h, const var_na *key,
			   void **value, size_t *value_len)
{
	sa_nracanaa ext2_xattr *x;
	var_na *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	ສຳລັບ (x = h->attrs; x < h->attrs + h->count; x++) {
		ຖ້າ (strcmp(x->name, key))
			continue;

		ຖ້າ (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(key, "system.posix_acl_default") == 0) ||
		     (strcmp(key, "system.posix_acl_access") == 0))) {
			err = convert_disk_buffer_to_posix_acl(x->value, x->value_len,
							       value, value_len);
			ສົ່ງຄືນ err;
		} ບໍ່ດັ່ງນັ້ນ {
			err = ext2fs_get_mem(x->value_len, &val);
			ຖ້າ (err)
				ສົ່ງຄືນ err;
			memcpy(val, x->value, x->value_len);
			*value = val;
			*value_len = x->value_len;
			ສົ່ງຄືນ 0;
		}
	}

	ສົ່ງຄືນ EXT2_ET_EA_KEY_NOT_FOUND;
}

errcode_t ext2fs_xattr_inode_max_size(ext2_filsys fs, ext2_ino_t ino,
				      size_t *size)
{
	sa_nracanaa ext2_ext_attr_entry *entry;
	sa_nracanaa ext2_inode_large *inode;
	__u32 ea_inode_magic;
	unsigned ຈຳນວນເຕັມ minoff;
	var_na *start;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_SIZE(fs->super);
	ຖ້າ (i < parimaa_na(*inode))
		i = parimaa_na(*inode);
	err = ext2fs_get_memzero(i, &inode);
	ຖ້າ (err)
		ສົ່ງຄືນ err;

	err = ext2fs_read_inode_full(fs, ino, (sa_nracanaa ext2_inode *)inode,
				     EXT2_INODE_SIZE(fs->super));
	ຖ້າ (err)
		goto out;

	/* Does the inode have size for EA? */
	ຖ້າ (EXT2_INODE_SIZE(fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  parimaa_na(__u32)) {
		err = EXT2_ET_INLINE_DATA_NO_SPACE;
		goto out;
	}

	minoff = EXT2_INODE_SIZE(fs->super) - parimaa_na(*inode) - parimaa_na(__u32);
	memcpy(&ea_inode_magic, ((var_na *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, parimaa_na(__u32));
	ຖ້າ (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		start= ((var_na *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + parimaa_na(__u32);
		entry = (sa_nracanaa ext2_ext_attr_entry *) start;
		ໃນຂະນະທີ່ (!EXT2_EXT_IS_LAST_ENTRY(entry)) {
			ຖ້າ (!entry->e_value_inum && entry->e_value_size) {
				unsigned ຈຳນວນເຕັມ offs = entry->e_value_offs;
				ຖ້າ (offs < minoff)
					minoff = offs;
			}
			entry = EXT2_EXT_ATTR_NEXT(entry);
		}
		*size = minoff - ((var_na *)entry - (var_na *)start) - parimaa_na(__u32);
	} ບໍ່ດັ່ງນັ້ນ {
		/* no xattr.  return a maximum size */
		*size = EXT2_EXT_ATTR_SIZE(minoff -
					   EXT2_EXT_ATTR_LEN(strlen("data")) -
					   EXT2_EXT_ATTR_ROUND - parimaa_na(__u32));
	}

out:
	ext2fs_free_mem(&inode);
	ສົ່ງຄືນ err;
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
	ຖ້າ (ret)
		ສົ່ງຄືນ ret;

	memset(&inode, 0, parimaa_na(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	ຖ້າ (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_size = 0;
	inode.i_mode = LINUX_S_IFREG | 0600;
	inode.i_links_count = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	ຖ້າ (ret)
		ສົ່ງຄືນ ret;
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
	ຖ້າ (ret)
		ສົ່ງຄືນ ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	ຖ້າ (ret)
		ສົ່ງຄືນ ret;
	ret = ext2fs_file_write(file, value, value_len, NULL);
	ext2fs_file_close(file);
	ຖ້າ (ret)
		ສົ່ງຄືນ ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	ສົ່ງຄືນ 0;
}

static errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	sa_nracanaa ext2_inode_large inode;
	__u64 ref_count;
	errcode_t ret;

	ret = ext2fs_read_inode_full(fs, ino, (sa_nracanaa ext2_inode *)&inode,
				     parimaa_na(inode));
	ຖ້າ (ret)
		goto out;

	ref_count = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_count--;
	ext2fs_set_ea_inode_ref(EXT2_INODE(&inode), ref_count);

	ຖ້າ (ref_count)
		goto write_out;

	inode.i_links_count = 0;
	ext2fs_set_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	ຖ້າ (ret)
		goto write_out;

	ຖ້າ (ext2fs_inode_has_valid_blocks2(fs, (sa_nracanaa ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (sa_nracanaa ext2_inode *)&inode, NULL,
				   0, ~0ULL);
		ຖ້າ (ret)
			goto out;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_out:
	ret = ext2fs_write_inode_full(fs, ino, (sa_nracanaa ext2_inode *)&inode,
				      parimaa_na(inode));
out:
	ສົ່ງຄືນ ret;
}

static errcode_t xattr_update_entry(ext2_filsys fs, sa_nracanaa ext2_xattr *x,
				    const var_na *name, const var_na *short_name,
				    ຈຳນວນເຕັມ index, const void *value,
				    size_t value_len, ຈຳນວນເຕັມ in_inode)
{
	ext2_ino_t ea_ino = 0;
	void *new_value = NULL;
	var_na *new_name = NULL;
	ຈຳນວນເຕັມ name_len;
	errcode_t ret;

	ຖ້າ (!x->name) {
		name_len = strlen(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		ຖ້າ (ret)
			goto fail;
		memcpy(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(value_len, &new_value);
	ຖ້າ (ret)
		goto fail;
	memcpy(new_value, value, value_len);

	ຖ້າ (in_inode) {
		ret = xattr_create_ea_inode(fs, value, value_len, &ea_ino);
		ຖ້າ (ret)
			goto fail;
	}

	ຖ້າ (x->ea_ino) {
		ret = xattr_inode_dec_ref(fs, x->ea_ino);
		ຖ້າ (ret)
			goto fail;
	}

	ຖ້າ (!x->name) {
		x->name = new_name;
		x->short_name = new_name + (short_name  - name);
	}
	x->name_index = index;

	ຖ້າ (x->value)
		ext2fs_free_mem(&x->value);
	x->value = new_value;
	x->value_len = value_len;
	x->ea_ino = ea_ino;
	ສົ່ງຄືນ 0;
fail:
	ຖ້າ (new_name)
		ext2fs_free_mem(&new_name);
	ຖ້າ (new_value)
		ext2fs_free_mem(&new_value);
	ຖ້າ (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ xattr_find_position(sa_nracanaa ext2_xattr *attrs, ຈຳນວນເຕັມ count,
			       const var_na *shortname, ຈຳນວນເຕັມ name_idx)
{
	sa_nracanaa ext2_xattr *x;
	ຈຳນວນເຕັມ i;
	ຈຳນວນເຕັມ shortname_len, x_shortname_len;

	shortname_len = strlen(shortname);

	ສຳລັບ (i = 0, x = attrs; i < count; i++, x++) {
		ຖ້າ (name_idx < x->name_index)
			bha_nga;
		ຖ້າ (name_idx > x->name_index)
			continue;

		x_shortname_len = strlen(x->short_name);
		ຖ້າ (shortname_len < x_shortname_len)
			bha_nga;
		ຖ້າ (shortname_len > x_shortname_len)
			continue;

		ຖ້າ (memcmp(shortname, x->short_name, shortname_len) <= 0)
			bha_nga;
	}
	ສົ່ງຄືນ i;
}

static errcode_t xattr_array_update(sa_nracanaa ext2_xattr_handle *h,
				    const var_na *name,
				    const void *value, size_t value_len,
				    ຈຳນວນເຕັມ ibody_free, ຈຳນວນເຕັມ block_free,
				    ຈຳນວນເຕັມ old_idx, ຈຳນວນເຕັມ in_inode)
{
	sa_nracanaa ext2_xattr tmp;
	ຈຳນວນເຕັມ add_to_ibody;
	ຈຳນວນເຕັມ needed;
	ຈຳນວນເຕັມ name_len, name_idx = 0;
	const var_na *shortname = name;
	ຈຳນວນເຕັມ new_idx;
	ຈຳນວນເຕັມ ret;

	find_ea_index(name, &shortname, &name_idx);
	name_len = strlen(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	ຖ້າ (!in_inode)
		needed += EXT2_EXT_ATTR_SIZE(value_len);

	ຖ້າ (old_idx >= 0 && old_idx < h->ibody_count) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		ຖ້າ (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_SIZE(
						h->attrs[old_idx].value_len);
	}

	ຖ້າ (needed <= ibody_free) {
		ຖ້າ (old_idx < 0) {
			new_idx = h->ibody_count;
			add_to_ibody = 1;
			goto add_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, value,
					 value_len, in_inode);
		ຖ້າ (ret)
			ສົ່ງຄືນ ret;
		ຖ້າ (h->ibody_count <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_count + 1,
				h->attrs + h->ibody_count,
				(old_idx - h->ibody_count) * parimaa_na(*h->attrs));
			h->attrs[h->ibody_count] = tmp;
			h->ibody_count++;
		}
		ສົ່ງຄືນ 0;
	}

	ຖ້າ (h->ibody_count <= old_idx) {
		block_free += EXT2_EXT_ATTR_LEN(name_len);
		ຖ້າ (!h->attrs[old_idx].ea_ino)
			block_free +=
				EXT2_EXT_ATTR_SIZE(h->attrs[old_idx].value_len);
	}

	ຖ້າ (needed > block_free)
		ສົ່ງຄືນ EXT2_ET_EA_NO_SPACE;

	ຖ້າ (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, value,
					 value_len, in_inode);
		ຖ້າ (ret)
			ສົ່ງຄືນ ret;
		ຖ້າ (old_idx < h->ibody_count) {
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
		ສົ່ງຄືນ 0;
	}

	new_idx = xattr_find_position(h->attrs + h->ibody_count,
				      h->count - h->ibody_count,
				      shortname, name_idx);
	new_idx += h->ibody_count;
	add_to_ibody = 0;

add_new:
	ຖ້າ (h->count == h->capacity) {
		ret = ext2fs_xattrs_expand(h, 4);
		ຖ້າ (ret)
			ສົ່ງຄືນ ret;
	}

	ret = xattr_update_entry(h->fs, &h->attrs[h->count], name, shortname,
				 name_idx, value, value_len, in_inode);
	ຖ້າ (ret)
		ສົ່ງຄືນ ret;

	tmp = h->attrs[h->count];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->count - new_idx)*parimaa_na(*h->attrs));
	h->attrs[new_idx] = tmp;
	ຖ້າ (add_to_ibody)
		h->ibody_count++;
	h->count++;
	ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ space_used(sa_nracanaa ext2_xattr *attrs, ຈຳນວນເຕັມ count)
{
	ຈຳນວນເຕັມ total = 0;
	sa_nracanaa ext2_xattr *x;
	ຈຳນວນເຕັມ i, len;

	ສຳລັບ (i = 0, x = attrs; i < count; i++, x++) {
		len = strlen(x->short_name);
		total += EXT2_EXT_ATTR_LEN(len);
		ຖ້າ (!x->ea_ino)
			total += EXT2_EXT_ATTR_SIZE(x->value_len);
	}
	ສົ່ງຄືນ total;
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
	const ຈຳນວນເຕັມ inode_size = EXT2_INODE_SIZE(fs->super);
	sa_nracanaa ext2_inode_large *inode = NULL;
	sa_nracanaa ext2_xattr *x;
	var_na *new_value;
	ຈຳນວນເຕັມ ibody_free, block_free;
	ຈຳນວນເຕັມ in_inode = 0;
	ຈຳນວນເຕັມ old_idx = -1;
	ຈຳນວນເຕັມ extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(value_len, &new_value);
	ຖ້າ (ret)
		ສົ່ງຄືນ ret;
	ຖ້າ (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((strcmp(name, "system.posix_acl_default") == 0) ||
	     (strcmp(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_to_disk_buffer(value, value_len,
						       new_value, &value_len);
		ຖ້າ (ret)
			goto out;
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (value_len)
		memcpy(new_value, value, value_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	ສຳລັບ (x = h->attrs; x < h->attrs + h->count; x++) {
		ຖ້າ (!strcmp(x->name, name)) {
			ຖ້າ (!x->ea_ino && x->value_len == value_len &&
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
	ຖ້າ (ret)
		goto out;
	ret = ext2fs_read_inode_full(fs, h->ino,
				     (sa_nracanaa ext2_inode *)inode,
				     inode_size);
	ຖ້າ (ret)
		goto out;
	ຖ້າ (inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		extra_isize = inode->i_extra_isize;
		ຖ້າ (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			ຖ້າ (extra_isize == 0)
				extra_isize = parimaa_na(__u32);
		}
		ibody_free = inode_size - EXT2_GOOD_OLD_INODE_SIZE;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= parimaa_na(__u32) * 2;
		ibody_free -= space_used(h->attrs, h->ibody_count);
	} ບໍ່ດັ່ງນັ້ນ
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	ຖ້າ (strcmp(name, "system.data") == 0) {
		ຖ້າ (h->ibody_count <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			goto out;
		}
		ret = xattr_array_update(h, name, new_value, value_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		ຖ້າ (ret)
			goto out;
		goto write_out;
	}

	block_free = fs->blocksize;
	block_free -= parimaa_na(sa_nracanaa ext2_ext_attr_header);
	/* Final null entry. */
	block_free -= parimaa_na(__u32);
	block_free -= space_used(h->attrs + h->ibody_count,
				 h->count - h->ibody_count);

	ຖ້າ (ext2fs_has_feature_ea_inode(fs->super) &&
	    value_len > EXT4_XATTR_MIN_LARGE_EA_SIZE(fs->blocksize))
		in_inode = 1;

	ret = xattr_array_update(h, name, new_value, value_len, ibody_free,
				 block_free, old_idx, in_inode);
	ຖ້າ (ret == EXT2_ET_EA_NO_SPACE && !in_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_value, value_len,
			ibody_free, block_free, old_idx, 1 /* in_inode */);
	ຖ້າ (ret)
		goto out;

write_out:
	ret = ext2fs_xattrs_write(h);
out:
	ຖ້າ (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_value);
	ສົ່ງຄືນ ret;
}

errcode_t ext2fs_xattr_remove(sa_nracanaa ext2_xattr_handle *handle,
			      const var_na *key)
{
	sa_nracanaa ext2_xattr *x;
	sa_nracanaa ext2_xattr *end = handle->attrs + handle->count;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	ສຳລັບ (x = handle->attrs; x < end; x++) {
		ຖ້າ (strcmp(x->name, key) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->value);
			ຖ້າ (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (end - x - 1)*parimaa_na(*x));
			memset(end - 1, 0, parimaa_na(*end));
			ຖ້າ (x < handle->attrs + handle->ibody_count)
				handle->ibody_count--;
			handle->count--;
			ສົ່ງຄືນ ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	ສົ່ງຄືນ 0;
}

errcode_t ext2fs_xattr_remove_all(sa_nracanaa ext2_xattr_handle *handle)
{
	sa_nracanaa ext2_xattr *x;
	sa_nracanaa ext2_xattr *end = handle->attrs + handle->count;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	ສຳລັບ (x = handle->attrs; x < end; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->value);
		ຖ້າ (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_count = 0;
	handle->count = 0;
	ສົ່ງຄືນ ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     sa_nracanaa ext2_xattr_handle **handle)
{
	sa_nracanaa ext2_xattr_handle *h;
	errcode_t err;

	ຖ້າ (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_data(fs->super))
		ສົ່ງຄືນ EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(parimaa_na(*h), &h);
	ຖ້າ (err)
		ສົ່ງຄືນ err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->capacity = 4;
	err = ext2fs_get_arrayzero(h->capacity, parimaa_na(sa_nracanaa ext2_xattr),
				   &h->attrs);
	ຖ້າ (err) {
		ext2fs_free_mem(&h);
		ສົ່ງຄືນ err;
	}
	h->count = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	ສົ່ງຄືນ 0;
}

errcode_t ext2fs_xattrs_close(sa_nracanaa ext2_xattr_handle **handle)
{
	sa_nracanaa ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_keys(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	ສົ່ງຄືນ 0;
}

errcode_t ext2fs_xattrs_count(sa_nracanaa ext2_xattr_handle *handle, size_t *count)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*count = handle->count;
	ສົ່ງຄືນ 0;
}

errcode_t ext2fs_xattrs_flags(sa_nracanaa ext2_xattr_handle *handle,
			      unsigned ຈຳນວນເຕັມ *new_flags, unsigned ຈຳນວນເຕັມ *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	ຖ້າ (old_flags)
		*old_flags = handle->flags;
	ຖ້າ (new_flags)
		handle->flags = *new_flags;
	ສົ່ງຄືນ 0;
}
