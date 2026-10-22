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
#གལ་སྲིད HAVE_UNISTD_H
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
	གལ་སྲིད (retval)
		ལོག retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	ལོག 0;
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
	ཧྲིལ n;

	དོན་ལུ (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*parimaa_na(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	གལ་སྲིད (entry->e_value_inum == 0 && entry->e_value_size != 0) {
		__u32 *value = (__u32 *)data;
		དོན་ལུ (n = (entry->e_value_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << VALUE_HASH_SHIFT) ^
			       (hash >> (8*parimaa_na(hash) - VALUE_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*value++);
		}
	}

	ལོག hash;
}

__u32 ext2fs_ext_attr_hash_entry_signed(sa_nracanaa ext2_ext_attr_entry *entry,
					void *data)
{
	__u32 hash = 0;
	signed var_na *name = (((signed var_na *) entry) +
			     parimaa_na(sa_nracanaa ext2_ext_attr_entry));
	ཧྲིལ n;

	དོན་ལུ (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*parimaa_na(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	གལ་སྲིད (entry->e_value_inum == 0 && entry->e_value_size != 0) {
		__u32 *value = (__u32 *)data;
		དོན་ལུ (n = (entry->e_value_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << VALUE_HASH_SHIFT) ^
			       (hash >> (8*parimaa_na(hash) - VALUE_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*value++);
		}
	}

	ལོག hash;
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
	གལ་སྲིད (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_entry_signed(entry, data);

	གལ་སྲིད (entry->e_value_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, entry->e_value_inum,
					    &ea_inode_hash);
		གལ་སྲིད (retval)
			ལོག retval;

		*hash = (*hash << VALUE_HASH_SHIFT) ^
			(*hash >> (8*parimaa_na(*hash) - VALUE_HASH_SHIFT)) ^
			ea_inode_hash;
		གལ་སྲིད (signed_hash)
			*signed_hash = (*signed_hash << VALUE_HASH_SHIFT) ^
				(*signed_hash >> (8*parimaa_na(*hash) -
						  VALUE_HASH_SHIFT)) ^
				ea_inode_hash;
	}
	ལོག 0;
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
	ལོག ext2fs_ext_attr_hash_entry3(fs, entry, data, hash, NULL);
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
	སྐབས (here < end && !EXT2_EXT_IS_LAST_ENTRY(here)) {
		གལ་སྲིད (!here->e_hash) {
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
	ལོག inode->i_atime;
}

void ext2fs_set_ea_inode_hash(sa_nracanaa ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(sa_nracanaa ext2_inode *inode)
{
	ལོག ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_version;
}

void ext2fs_set_ea_inode_ref(sa_nracanaa ext2_inode *inode, __u64 ref_count)
{
	inode->i_ctime = (__u32)(ref_count >> 32);
	inode->osd1.linux1.l_i_version = (__u32)ref_count;
}

static errcode_t check_ext_attr_header(sa_nracanaa ext2_ext_attr_header *header)
{
	གལ་སྲིད ((header->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     header->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    header->h_blocks != 1)
		ལོག EXT2_ET_BAD_EA_HEADER;

	ལོག 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block, void *buf,
				ext2_ino_t inum)
{
	ཧྲིལ		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	གལ་སྲིད (retval)
		ལོག retval;

	གལ་སྲིད (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_block_csum_verify(fs, inum, block, buf))
		csum_failed = 1;

#ifdef WORDS_BIGENDIAN
	ext2fs_swap_ext_attr(buf, buf, fs->blocksize, 1);
#endif

	retval = check_ext_attr_header(buf);
	གལ་སྲིད (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	ལོག retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block, void *buf)
{
	ལོག ext2fs_read_ext_attr3(fs, block, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, void *buf)
{
	ལོག ext2fs_read_ext_attr2(fs, block, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block, void *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	var_na		*write_buf;

#ifdef WORDS_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	གལ་སྲིད (retval)
		ལོག retval;
	ext2fs_swap_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#དེ་མིན
	write_buf = (var_na *) inbuf;
#endif

	retval = ext2fs_ext_attr_block_csum_set(fs, inum, block,
			(sa_nracanaa ext2_ext_attr_header *)write_buf);
	གལ་སྲིད (retval)
		ལོག retval;

	retval = io_channel_write_blk64(fs->io, block, 1, write_buf);
#ifdef WORDS_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#endif
	གལ་སྲིད (!retval)
		ext2fs_mark_changed(fs);
	ལོག retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block, void *inbuf)
{
	ལོག ext2fs_write_ext_attr3(fs, block, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block, void *inbuf)
{
	ལོག ext2fs_write_ext_attr2(fs, block, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    var_na *block_buf, ཧྲིལ adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	sa_nracanaa ext2_ext_attr_header *header;
	var_na	*buf = 0;

	གལ་སྲིད ((blk >= ext2fs_blocks_count(fs->super)) ||
	    (blk < fs->super->s_first_data_block))
		ལོག EXT2_ET_BAD_EA_BLOCK_NUM;

	གལ་སྲིད (!block_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		གལ་སྲིད (retval)
			ལོག retval;
		block_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, block_buf, inum);
	གལ་སྲིད (retval)
		goto errout;

	header = (sa_nracanaa ext2_ext_attr_header *) block_buf;
	header->h_refcount += adjust;
	གལ་སྲིད (newcount)
		*newcount = header->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, block_buf, inum);
	གལ་སྲིད (retval)
		goto errout;

errout:
	གལ་སྲིད (buf)
		ext2fs_free_mem(&buf);
	ལོག retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    var_na *block_buf, ཧྲིལ adjust,
				    __u32 *newcount)
{
	ལོག ext2fs_adjust_ea_refcount3(fs, blk, block_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					var_na *block_buf, ཧྲིལ adjust,
					__u32 *newcount)
{
	ལོག ext2fs_adjust_ea_refcount2(fs, blk, block_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
sa_nracanaa ext2_xattr {
	ཧྲིལ name_index;
	var_na *name;
	var_na *short_name;
	void *value;
	unsigned ཧྲིལ value_len;
	ext2_ino_t ea_ino;
};

sa_nracanaa ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	sa_nracanaa ext2_xattr *attrs;
	ཧྲིལ capacity;
	ཧྲིལ count;
	ཧྲིལ ibody_count;
	ext2_ino_t ino;
	unsigned ཧྲིལ flags;
};

static errcode_t ext2fs_xattrs_expand(sa_nracanaa ext2_xattr_handle *h,
				      unsigned ཧྲིལ expandby)
{
	sa_nracanaa ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->capacity + expandby,
				   parimaa_na(sa_nracanaa ext2_xattr), &new_attrs);
	གལ་སྲིད (err)
		ལོག err;

	memcpy(new_attrs, h->attrs, h->capacity * parimaa_na(sa_nracanaa ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->capacity += expandby;
	h->attrs = new_attrs;

	ལོག 0;
}

sa_nracanaa ea_name_index {
	ཧྲིལ index;
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

static const var_na *find_ea_prefix(ཧྲིལ index)
{
	sa_nracanaa ea_name_index *e;

	དོན་ལུ (e = ea_names; e->name; e++)
		གལ་སྲིད (e->index == index)
			ལོག e->name;

	ལོག NULL;
}

static ཧྲིལ find_ea_index(const var_na *fullname, const var_na **name, ཧྲིལ *index)
{
	sa_nracanaa ea_name_index *e;

	དོན་ལུ (e = ea_names; e->name; e++) {
		གལ་སྲིད (strncmp(fullname, e->name, strlen(e->name)) == 0) {
			*name = fullname + strlen(e->name);
			*index = e->index;
			ལོག 1;
		}
	}
	ལོག 0;
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
	གལ་སྲིད (inode == NULL) {
		err = ext2fs_read_inode_full(fs, ino, (sa_nracanaa ext2_inode *)&i,
					     parimaa_na(sa_nracanaa ext2_inode_large));
		གལ་སྲིད (err)
			ལོག err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (sa_nracanaa ext2_inode *)inode);
	གལ་སྲིད (blk == 0)
		ལོག 0;

	/* Find block, zero it, write back */
	གལ་སྲིད ((blk < fs->super->s_first_data_block) ||
	    (blk >= ext2fs_blocks_count(fs->super))) {
		err = EXT2_ET_BAD_EA_BLOCK_NUM;
		goto out;
	}

	err = ext2fs_get_mem(fs->blocksize, &block_buf);
	གལ་སྲིད (err)
		goto out;

	err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
	གལ་སྲིད (err)
		goto out2;

	/* We only know how to deal with v2 EA blocks */
	header = (sa_nracanaa ext2_ext_attr_header *) block_buf;
	གལ་སྲིད (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_HEADER;
		goto out2;
	}

	header->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
	གལ་སྲིད (err)
		goto out2;

	/* Erase link to block */
	ext2fs_file_acl_block_set(fs, (sa_nracanaa ext2_inode *)inode, 0);
	གལ་སྲིད (header->h_refcount == 0)
		ext2fs_block_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_blocks(fs, (sa_nracanaa ext2_inode *)inode, 1);
	གལ་སྲིད (err)
		goto out2;

	/* Write inode? */
	གལ་སྲིད (inode == &i) {
		err = ext2fs_write_inode_full(fs, ino, (sa_nracanaa ext2_inode *)&i,
					      parimaa_na(sa_nracanaa ext2_inode_large));
		གལ་སྲིད (err)
			goto out2;
	}

out2:
	ext2fs_free_mem(&block_buf);
out:
	ལོག err;
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
	གལ་སྲིད (blk != 0) {
		གལ་སྲིད ((blk < fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_count(fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_NUM;
			goto out;
		}

		err = ext2fs_get_mem(fs->blocksize, &block_buf);
		གལ་སྲིད (err)
			goto out;

		err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
		གལ་སྲིད (err)
			goto out2;

		/* We only know how to deal with v2 EA blocks */
		header = (sa_nracanaa ext2_ext_attr_header *) block_buf;
		གལ་སྲིད (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			goto out2;
		}

		/* Single-user block.  We're done here. */
		གལ་སྲིད (header->h_refcount == 1)
			goto out2;

		/* We need to CoW the block. */
		header->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
		གལ་སྲིད (err)
			goto out2;
	} དེ་མིན {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_add_blocks(fs, (sa_nracanaa ext2_inode *)inode,
					     1);
		གལ་སྲིད (err)
			goto out;
	}

	/* Allocate a block */
	goal = ext2fs_find_inode_goal(fs, ino, (sa_nracanaa ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, NULL, &blk);
	གལ་སྲིད (err)
		goto out2;
	ext2fs_file_acl_block_set(fs, (sa_nracanaa ext2_inode *)inode, blk);
out2:
	གལ་སྲིད (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	ལོག err;
}


static anta_hstha ཧྲིལ
posix_acl_xattr_count(size_t size)
{
        གལ་སྲིད (size < parimaa_na(posix_acl_xattr_header))
                ལོག -1;
        size -= parimaa_na(posix_acl_xattr_header);
        གལ་སྲིད (size % parimaa_na(posix_acl_xattr_entry))
                ལོག -1;
        ལོག size / parimaa_na(posix_acl_xattr_entry);
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

	ཧྲིལ count;

	གལ་སྲིད (!value)
		ལོག EINVAL;
	གལ་སྲིད (size < parimaa_na(posix_acl_xattr_header))
		ལོག ENOMEM;
	གལ་སྲིད (header->a_version != ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION))
		ལོག EINVAL;

	count = posix_acl_xattr_count(size);
	ext_acl = out_buf;
	ext_acl->a_version = ext2fs_cpu_to_le32(EXT4_ACL_VERSION);

	གལ་སྲིད (count <= 0)
		ལོག EINVAL;

	e = (var_na *) out_buf + parimaa_na(ext4_acl_header);
	s = parimaa_na(ext4_acl_header);
	དོན་ལུ (end = entry + count; entry != end;entry++) {
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
				ལོག EINVAL;
		}
	}
	*size_out = s;
	ལོག 0;
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

	གལ་སྲིད ((!value) ||
	    (size < parimaa_na(ext4_acl_header)) ||
	    (ext_acl->a_version != ext2fs_cpu_to_le32(EXT4_ACL_VERSION)))
		ལོག EINVAL;

	err = ext2fs_get_mem(size * 2, &out);
	གལ་སྲིད (err)
		ལོག err;

	header = (posix_acl_xattr_header *) out;
	header->a_version = ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION);
	entry = (posix_acl_xattr_entry *) (out + parimaa_na(posix_acl_xattr_header));

	cp = (const var_na *) value + parimaa_na(ext4_acl_header);
	size -= parimaa_na(ext4_acl_header);

	སྐབས (size > 0) {
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
				ལོག EINVAL;
		}
		entry++;
	}
	*out_buf = out;
	*size_out = ((var_na *) entry - out);
	ལོག 0;
}

static errcode_t
write_xattrs_to_buffer(ext2_filsys fs, sa_nracanaa ext2_xattr *attrs, ཧྲིལ count,
		       void *entries_start, unsigned ཧྲིལ storage_size,
		       unsigned ཧྲིལ value_offset_correction, ཧྲིལ write_hash)
{
	sa_nracanaa ext2_xattr *x;
	sa_nracanaa ext2_ext_attr_entry *e = entries_start;
	var_na *end = (var_na *) entries_start + storage_size;
	unsigned ཧྲིལ value_size;
	errcode_t err;

	memset(entries_start, 0, storage_size);
	དོན་ལུ (x = attrs; x < attrs + count; x++) {
		value_size = ((x->value_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = strlen(x->short_name);
		e->e_name_index = x->name_index;

		e->e_value_size = x->value_len;
		e->e_value_inum = x->ea_ino;

		/* Store name */
		memcpy((var_na *)e + parimaa_na(*e), x->short_name, e->e_name_len);
		གལ་སྲིད (x->ea_ino) {
			e->e_value_offs = 0;
		} དེ་མིན {
			end -= value_size;
			e->e_value_offs = end - (var_na *) entries_start +
						value_offset_correction;
			memcpy(end, x->value, e->e_value_size);
		}

		གལ་སྲིད (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : end,
							  &e->e_hash);
			གལ་སྲིད (err)
				ལོག err;
		} དེ་མིན
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_NEXT(e);
		*(__u32 *)e = 0;
	}
	ལོག 0;
}

errcode_t ext2fs_xattrs_write(sa_nracanaa ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	const unsigned ཧྲིལ inode_size = EXT2_INODE_SIZE(fs->super);
	sa_nracanaa ext2_inode_large *inode;
	var_na *start, *block_buf = NULL;
	sa_nracanaa ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	blk64_t blk;
	unsigned ཧྲིལ storage_size;
	unsigned ཧྲིལ i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_size;
	གལ་སྲིད (i < parimaa_na(*inode))
		i = parimaa_na(*inode);
	err = ext2fs_get_memzero(i, &inode);
	གལ་སྲིད (err)
		ལོག err;

	err = ext2fs_read_inode_full(fs, handle->ino, EXT2_INODE(inode),
				     inode_size);
	གལ་སྲིད (err)
		goto out;

	/* If extra_isize isn't set, we need to set it now */
	གལ་སྲིད (inode->i_extra_isize == 0 &&
	    inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		var_na *p = (var_na *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		གལ་སྲིད (extra == 0)
			extra = parimaa_na(__u32);
		memset(p + EXT2_GOOD_OLD_INODE_SIZE, 0, extra);
		inode->i_extra_isize = extra;
	}
	གལ་སྲིད (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		goto out;
	}

	/* Does the inode have space for EA? */
	གལ་སྲིད (inode->i_extra_isize < parimaa_na(inode->i_extra_isize) ||
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
	གལ་སྲིད (err)
		goto out;
write_ea_block:
	/* Are we done? */
	གལ་སྲིད (handle->ibody_count == handle->count &&
	    !ext2fs_file_acl_block(fs, EXT2_INODE(inode)))
		goto skip_ea_block;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &block_buf);
	གལ་སྲིད (err)
		goto out;

	storage_size = fs->blocksize - parimaa_na(sa_nracanaa ext2_ext_attr_header);
	start = block_buf + parimaa_na(sa_nracanaa ext2_ext_attr_header);

	err = write_xattrs_to_buffer(fs, handle->attrs + handle->ibody_count,
				     handle->count - handle->ibody_count, start,
				     storage_size, start - block_buf, 1);
	གལ་སྲིད (err)
		goto out2;

	/* Write a header on the EA block */
	header = (sa_nracanaa ext2_ext_attr_header *) block_buf;
	header->h_magic = EXT2_EXT_ATTR_MAGIC;
	header->h_refcount = 1;
	header->h_blocks = 1;

	/* Get a new block for writing */
	err = prep_ea_block_for_write(fs, handle->ino, inode);
	གལ་སྲིད (err)
		goto out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_block(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, handle->ino);
	གལ་སྲིད (err)
		goto out2;

skip_ea_block:
	blk = ext2fs_file_acl_block(fs, (sa_nracanaa ext2_inode *)inode);
	གལ་སྲིད (!block_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		གལ་སྲིད (err)
			goto out;
	}

	/* Write the inode */
	err = ext2fs_write_inode_full(fs, handle->ino, EXT2_INODE(inode),
				      inode_size);
	གལ་སྲིད (err)
		goto out2;

out2:
	ext2fs_free_mem(&block_buf);
out:
	ext2fs_free_mem(&inode);
	ལོག err;
}

static errcode_t read_xattrs_from_buffer(sa_nracanaa ext2_xattr_handle *handle,
					 sa_nracanaa ext2_inode_large *inode,
					 sa_nracanaa ext2_ext_attr_entry *entries,
					 unsigned ཧྲིལ storage_size,
					 var_na *value_start)
{
	sa_nracanaa ext2_xattr *x;
	sa_nracanaa ext2_ext_attr_entry *entry, *end;
	const var_na *prefix;
	unsigned ཧྲིལ remain, prefix_len;
	errcode_t err;
	unsigned ཧྲིལ values_size = storage_size +
			((var_na *)entries - value_start);

	/* find the end */
	end = entries;
	remain = storage_size;
	སྐབས (remain >= parimaa_na(sa_nracanaa ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(end)) {

		/* header eats this space */
		remain -= parimaa_na(sa_nracanaa ext2_ext_attr_entry);

		/* is attribute name valid? */
		གལ་སྲིད (EXT2_EXT_ATTR_SIZE(end->e_name_len) > remain)
			ལོག EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(end->e_name_len);
		end = EXT2_EXT_ATTR_NEXT(end);
	}

	entry = entries;
	remain = storage_size;
	སྐབས (remain >= parimaa_na(sa_nracanaa ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(entry)) {

		/* Allocate space for more attrs? */
		གལ་སྲིད (handle->count == handle->capacity) {
			err = ext2fs_xattrs_expand(handle, 4);
			གལ་སྲིད (err)
				ལོག err;
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
		གལ་སྲིད (err)
			ལོག err;
		གལ་སྲིད (prefix)
			memcpy(x->name, prefix, prefix_len);
		གལ་སྲིད (entry->e_name_len)
			memcpy(x->name + prefix_len,
			       (var_na *)entry + parimaa_na(*entry),
			       entry->e_name_len);
		x->short_name = x->name + prefix_len;
		x->name_index = entry->e_name_index;

		/* Check & copy value */
		གལ་སྲིད (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    entry->e_value_inum != 0)
			ལོག EXT2_ET_BAD_EA_BLOCK_NUM;

		གལ་སྲིད (entry->e_value_inum == 0) {
			གལ་སྲིད (entry->e_value_size > remain)
				ལོག EXT2_ET_EA_BAD_VALUE_SIZE;

			གལ་སྲིད (entry->e_value_offs + entry->e_value_size > values_size)
				ལོག EXT2_ET_EA_BAD_VALUE_OFFSET;

			གལ་སྲིད (entry->e_value_size > 0 &&
			    value_start + entry->e_value_offs <
			    (var_na *)end + parimaa_na(__u32))
				ལོག EXT2_ET_EA_BAD_VALUE_OFFSET;

			remain -= entry->e_value_size;

			err = ext2fs_get_mem(entry->e_value_size, &x->value);
			གལ་སྲིད (err)
				ལོག err;
			memcpy(x->value, value_start + entry->e_value_offs,
			       entry->e_value_size);
		} དེ་མིན {
			sa_nracanaa ext2_inode *ea_inode;
			ext2_file_t ea_file;

			གལ་སྲིད (entry->e_value_offs != 0)
				ལོག EXT2_ET_EA_BAD_VALUE_OFFSET;

			གལ་སྲིད (entry->e_value_size > (64 * 1024))
				ལོག EXT2_ET_EA_BAD_VALUE_SIZE;

			err = ext2fs_get_mem(entry->e_value_size, &x->value);
			གལ་སྲིད (err)
				ལོག err;

			err = ext2fs_file_open(handle->fs, entry->e_value_inum,
					       0, &ea_file);
			གལ་སྲིད (err)
				ལོག err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			གལ་སྲིད ((ea_inode->i_flags & EXT4_INLINE_DATA_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_count == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			དེ་མིན གལ་སྲིད ((__u64) ext2fs_file_get_size(ea_file) !=
				 entry->e_value_size)
				err = EXT2_ET_EA_BAD_VALUE_SIZE;
			དེ་མིན
				err = ext2fs_file_read(ea_file, x->value,
						       entry->e_value_size, 0);
			ext2fs_file_close(ea_file);
			གལ་སྲིད (err)
				ལོག err;
		}

		x->ea_ino = entry->e_value_inum;
		x->value_len = entry->e_value_size;

		/* e_hash may be 0 in older inode's ea */
		གལ་སྲིད (entry->e_hash != 0) {
			__u32 hash, signed_hash;

			void *data = (entry->e_value_inum != 0) ?
					0 : value_start + entry->e_value_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, entry,
							  data, &hash,
							  &signed_hash);
			གལ་སྲིད (err)
				ལོག err;
			གལ་སྲིད ((entry->e_hash != hash) &&
			    (entry->e_hash != signed_hash)) {
				sa_nracanaa ext2_inode child;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							entry->e_value_inum,
							&child);
				གལ་སྲིད (err)
					ལོག err;
				གལ་སྲིད (child.i_mtime != handle->ino ||
				    child.i_generation != inode->i_generation)
					ལོག EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->count++;
		entry = EXT2_EXT_ATTR_NEXT(entry);
	}

	ལོག 0;
}

static void xattrs_free_keys(sa_nracanaa ext2_xattr_handle *h)
{
	sa_nracanaa ext2_xattr *a = h->attrs;
	ཧྲིལ i;

	དོན་ལུ (i = 0; i < h->capacity; i++) {
		གལ་སྲིད (a[i].name)
			ext2fs_free_mem(&a[i].name);
		གལ་སྲིད (a[i].value)
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
	unsigned ཧྲིལ storage_size;
	var_na *start, *block_buf = NULL;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_keys(handle);

	/* Does the inode have space for EA? */
	གལ་སྲིད (inode->i_extra_isize < parimaa_na(inode->i_extra_isize) ||
	    EXT2_INODE_SIZE(handle->fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  parimaa_na(__u32))
		goto read_ea_block;
	གལ་སྲིད (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		goto out;
	}

	/* Look for EA in the inode */
	memcpy(&ea_inode_magic, ((var_na *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, parimaa_na(__u32));
	གལ་སྲིད (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_size = EXT2_INODE_SIZE(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_SIZE - inode->i_extra_isize -
			parimaa_na(__u32);
		start = ((var_na *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + parimaa_na(__u32);

		err = read_xattrs_from_buffer(handle, inode,
					(sa_nracanaa ext2_ext_attr_entry *) start,
					storage_size, start);
		གལ་སྲིད (err)
			goto out;

		handle->ibody_count = handle->count;
	}

read_ea_block:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_block(handle->fs, EXT2_INODE(inode));
	གལ་སྲིད (blk != 0) {
		གལ་སྲིད ((blk < handle->fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_count(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_NUM;
			goto out;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &block_buf);
		གལ་སྲིད (err)
			goto out;

		err = ext2fs_read_ext_attr3(handle->fs, blk, block_buf,
					    handle->ino);
		གལ་སྲིད (err)
			goto out3;

		/* We only know how to deal with v2 EA blocks */
		header = (sa_nracanaa ext2_ext_attr_header *) block_buf;
		གལ་སྲིད (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
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
	གལ་སྲིད (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	ལོག err;
}

errcode_t ext2fs_xattrs_read(sa_nracanaa ext2_xattr_handle *handle)
{
	sa_nracanaa ext2_inode_large *inode;
	size_t inode_size = EXT2_INODE_SIZE(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	གལ་སྲིད (inode_size < parimaa_na(*inode))
		inode_size = parimaa_na(*inode);
	err = ext2fs_get_memzero(inode_size, &inode);
	གལ་སྲིད (err)
		ལོག err;

	err = ext2fs_read_inode_full(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_SIZE(handle->fs->super));
	གལ་སྲིད (err)
		goto out;

	err = ext2fs_xattrs_read_inode(handle, inode);

out:
	ext2fs_free_mem(&inode);

	ལོག err;
}

errcode_t ext2fs_xattrs_iterate(sa_nracanaa ext2_xattr_handle *h,
				ཧྲིལ (*func)(var_na *name, var_na *value,
					    size_t value_len, void *data),
				void *data)
{
	sa_nracanaa ext2_xattr *x;
	ཧྲིལ dirty = 0;
	ཧྲིལ ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	དོན་ལུ (x = h->attrs; x < h->attrs + h->count; x++) {
		ret = func(x->name, x->value, x->value_len, data);
		གལ་སྲིད (ret & XATTR_CHANGED)
			dirty = 1;
		གལ་སྲིད (ret & XATTR_ABORT)
			bha_nga;
	}

	གལ་སྲིད (dirty)
		ལོག ext2fs_xattrs_write(h);
	ལོག 0;
}

errcode_t ext2fs_xattr_get(sa_nracanaa ext2_xattr_handle *h, const var_na *key,
			   void **value, size_t *value_len)
{
	sa_nracanaa ext2_xattr *x;
	var_na *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	དོན་ལུ (x = h->attrs; x < h->attrs + h->count; x++) {
		གལ་སྲིད (strcmp(x->name, key))
			continue;

		གལ་སྲིད (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(key, "system.posix_acl_default") == 0) ||
		     (strcmp(key, "system.posix_acl_access") == 0))) {
			err = convert_disk_buffer_to_posix_acl(x->value, x->value_len,
							       value, value_len);
			ལོག err;
		} དེ་མིན {
			err = ext2fs_get_mem(x->value_len, &val);
			གལ་སྲིད (err)
				ལོག err;
			memcpy(val, x->value, x->value_len);
			*value = val;
			*value_len = x->value_len;
			ལོག 0;
		}
	}

	ལོག EXT2_ET_EA_KEY_NOT_FOUND;
}

errcode_t ext2fs_xattr_inode_max_size(ext2_filsys fs, ext2_ino_t ino,
				      size_t *size)
{
	sa_nracanaa ext2_ext_attr_entry *entry;
	sa_nracanaa ext2_inode_large *inode;
	__u32 ea_inode_magic;
	unsigned ཧྲིལ minoff;
	var_na *start;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_SIZE(fs->super);
	གལ་སྲིད (i < parimaa_na(*inode))
		i = parimaa_na(*inode);
	err = ext2fs_get_memzero(i, &inode);
	གལ་སྲིད (err)
		ལོག err;

	err = ext2fs_read_inode_full(fs, ino, (sa_nracanaa ext2_inode *)inode,
				     EXT2_INODE_SIZE(fs->super));
	གལ་སྲིད (err)
		goto out;

	/* Does the inode have size for EA? */
	གལ་སྲིད (EXT2_INODE_SIZE(fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  parimaa_na(__u32)) {
		err = EXT2_ET_INLINE_DATA_NO_SPACE;
		goto out;
	}

	minoff = EXT2_INODE_SIZE(fs->super) - parimaa_na(*inode) - parimaa_na(__u32);
	memcpy(&ea_inode_magic, ((var_na *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, parimaa_na(__u32));
	གལ་སྲིད (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		start= ((var_na *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + parimaa_na(__u32);
		entry = (sa_nracanaa ext2_ext_attr_entry *) start;
		སྐབས (!EXT2_EXT_IS_LAST_ENTRY(entry)) {
			གལ་སྲིད (!entry->e_value_inum && entry->e_value_size) {
				unsigned ཧྲིལ offs = entry->e_value_offs;
				གལ་སྲིད (offs < minoff)
					minoff = offs;
			}
			entry = EXT2_EXT_ATTR_NEXT(entry);
		}
		*size = minoff - ((var_na *)entry - (var_na *)start) - parimaa_na(__u32);
	} དེ་མིན {
		/* no xattr.  return a maximum size */
		*size = EXT2_EXT_ATTR_SIZE(minoff -
					   EXT2_EXT_ATTR_LEN(strlen("data")) -
					   EXT2_EXT_ATTR_ROUND - parimaa_na(__u32));
	}

out:
	ext2fs_free_mem(&inode);
	ལོག err;
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
	གལ་སྲིད (ret)
		ལོག ret;

	memset(&inode, 0, parimaa_na(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	གལ་སྲིད (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_size = 0;
	inode.i_mode = LINUX_S_IFREG | 0600;
	inode.i_links_count = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	གལ་སྲིད (ret)
		ལོག ret;
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
	གལ་སྲིད (ret)
		ལོག ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	གལ་སྲིད (ret)
		ལོག ret;
	ret = ext2fs_file_write(file, value, value_len, NULL);
	ext2fs_file_close(file);
	གལ་སྲིད (ret)
		ལོག ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	ལོག 0;
}

static errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	sa_nracanaa ext2_inode_large inode;
	__u64 ref_count;
	errcode_t ret;

	ret = ext2fs_read_inode_full(fs, ino, (sa_nracanaa ext2_inode *)&inode,
				     parimaa_na(inode));
	གལ་སྲིད (ret)
		goto out;

	ref_count = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_count--;
	ext2fs_set_ea_inode_ref(EXT2_INODE(&inode), ref_count);

	གལ་སྲིད (ref_count)
		goto write_out;

	inode.i_links_count = 0;
	ext2fs_set_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	གལ་སྲིད (ret)
		goto write_out;

	གལ་སྲིད (ext2fs_inode_has_valid_blocks2(fs, (sa_nracanaa ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (sa_nracanaa ext2_inode *)&inode, NULL,
				   0, ~0ULL);
		གལ་སྲིད (ret)
			goto out;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_out:
	ret = ext2fs_write_inode_full(fs, ino, (sa_nracanaa ext2_inode *)&inode,
				      parimaa_na(inode));
out:
	ལོག ret;
}

static errcode_t xattr_update_entry(ext2_filsys fs, sa_nracanaa ext2_xattr *x,
				    const var_na *name, const var_na *short_name,
				    ཧྲིལ index, const void *value,
				    size_t value_len, ཧྲིལ in_inode)
{
	ext2_ino_t ea_ino = 0;
	void *new_value = NULL;
	var_na *new_name = NULL;
	ཧྲིལ name_len;
	errcode_t ret;

	གལ་སྲིད (!x->name) {
		name_len = strlen(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		གལ་སྲིད (ret)
			goto fail;
		memcpy(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(value_len, &new_value);
	གལ་སྲིད (ret)
		goto fail;
	memcpy(new_value, value, value_len);

	གལ་སྲིད (in_inode) {
		ret = xattr_create_ea_inode(fs, value, value_len, &ea_ino);
		གལ་སྲིད (ret)
			goto fail;
	}

	གལ་སྲིད (x->ea_ino) {
		ret = xattr_inode_dec_ref(fs, x->ea_ino);
		གལ་སྲིད (ret)
			goto fail;
	}

	གལ་སྲིད (!x->name) {
		x->name = new_name;
		x->short_name = new_name + (short_name  - name);
	}
	x->name_index = index;

	གལ་སྲིད (x->value)
		ext2fs_free_mem(&x->value);
	x->value = new_value;
	x->value_len = value_len;
	x->ea_ino = ea_ino;
	ལོག 0;
fail:
	གལ་སྲིད (new_name)
		ext2fs_free_mem(&new_name);
	གལ་སྲིད (new_value)
		ext2fs_free_mem(&new_value);
	གལ་སྲིད (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	ལོག ret;
}

static ཧྲིལ xattr_find_position(sa_nracanaa ext2_xattr *attrs, ཧྲིལ count,
			       const var_na *shortname, ཧྲིལ name_idx)
{
	sa_nracanaa ext2_xattr *x;
	ཧྲིལ i;
	ཧྲིལ shortname_len, x_shortname_len;

	shortname_len = strlen(shortname);

	དོན་ལུ (i = 0, x = attrs; i < count; i++, x++) {
		གལ་སྲིད (name_idx < x->name_index)
			bha_nga;
		གལ་སྲིད (name_idx > x->name_index)
			continue;

		x_shortname_len = strlen(x->short_name);
		གལ་སྲིད (shortname_len < x_shortname_len)
			bha_nga;
		གལ་སྲིད (shortname_len > x_shortname_len)
			continue;

		གལ་སྲིད (memcmp(shortname, x->short_name, shortname_len) <= 0)
			bha_nga;
	}
	ལོག i;
}

static errcode_t xattr_array_update(sa_nracanaa ext2_xattr_handle *h,
				    const var_na *name,
				    const void *value, size_t value_len,
				    ཧྲིལ ibody_free, ཧྲིལ block_free,
				    ཧྲིལ old_idx, ཧྲིལ in_inode)
{
	sa_nracanaa ext2_xattr tmp;
	ཧྲིལ add_to_ibody;
	ཧྲིལ needed;
	ཧྲིལ name_len, name_idx = 0;
	const var_na *shortname = name;
	ཧྲིལ new_idx;
	ཧྲིལ ret;

	find_ea_index(name, &shortname, &name_idx);
	name_len = strlen(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	གལ་སྲིད (!in_inode)
		needed += EXT2_EXT_ATTR_SIZE(value_len);

	གལ་སྲིད (old_idx >= 0 && old_idx < h->ibody_count) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		གལ་སྲིད (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_SIZE(
						h->attrs[old_idx].value_len);
	}

	གལ་སྲིད (needed <= ibody_free) {
		གལ་སྲིད (old_idx < 0) {
			new_idx = h->ibody_count;
			add_to_ibody = 1;
			goto add_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, value,
					 value_len, in_inode);
		གལ་སྲིད (ret)
			ལོག ret;
		གལ་སྲིད (h->ibody_count <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_count + 1,
				h->attrs + h->ibody_count,
				(old_idx - h->ibody_count) * parimaa_na(*h->attrs));
			h->attrs[h->ibody_count] = tmp;
			h->ibody_count++;
		}
		ལོག 0;
	}

	གལ་སྲིད (h->ibody_count <= old_idx) {
		block_free += EXT2_EXT_ATTR_LEN(name_len);
		གལ་སྲིད (!h->attrs[old_idx].ea_ino)
			block_free +=
				EXT2_EXT_ATTR_SIZE(h->attrs[old_idx].value_len);
	}

	གལ་སྲིད (needed > block_free)
		ལོག EXT2_ET_EA_NO_SPACE;

	གལ་སྲིད (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, value,
					 value_len, in_inode);
		གལ་སྲིད (ret)
			ལོག ret;
		གལ་སྲིད (old_idx < h->ibody_count) {
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
		ལོག 0;
	}

	new_idx = xattr_find_position(h->attrs + h->ibody_count,
				      h->count - h->ibody_count,
				      shortname, name_idx);
	new_idx += h->ibody_count;
	add_to_ibody = 0;

add_new:
	གལ་སྲིད (h->count == h->capacity) {
		ret = ext2fs_xattrs_expand(h, 4);
		གལ་སྲིད (ret)
			ལོག ret;
	}

	ret = xattr_update_entry(h->fs, &h->attrs[h->count], name, shortname,
				 name_idx, value, value_len, in_inode);
	གལ་སྲིད (ret)
		ལོག ret;

	tmp = h->attrs[h->count];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->count - new_idx)*parimaa_na(*h->attrs));
	h->attrs[new_idx] = tmp;
	གལ་སྲིད (add_to_ibody)
		h->ibody_count++;
	h->count++;
	ལོག 0;
}

static ཧྲིལ space_used(sa_nracanaa ext2_xattr *attrs, ཧྲིལ count)
{
	ཧྲིལ total = 0;
	sa_nracanaa ext2_xattr *x;
	ཧྲིལ i, len;

	དོན་ལུ (i = 0, x = attrs; i < count; i++, x++) {
		len = strlen(x->short_name);
		total += EXT2_EXT_ATTR_LEN(len);
		གལ་སྲིད (!x->ea_ino)
			total += EXT2_EXT_ATTR_SIZE(x->value_len);
	}
	ལོག total;
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
	const ཧྲིལ inode_size = EXT2_INODE_SIZE(fs->super);
	sa_nracanaa ext2_inode_large *inode = NULL;
	sa_nracanaa ext2_xattr *x;
	var_na *new_value;
	ཧྲིལ ibody_free, block_free;
	ཧྲིལ in_inode = 0;
	ཧྲིལ old_idx = -1;
	ཧྲིལ extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(value_len, &new_value);
	གལ་སྲིད (ret)
		ལོག ret;
	གལ་སྲིད (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((strcmp(name, "system.posix_acl_default") == 0) ||
	     (strcmp(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_to_disk_buffer(value, value_len,
						       new_value, &value_len);
		གལ་སྲིད (ret)
			goto out;
	} དེ་མིན གལ་སྲིད (value_len)
		memcpy(new_value, value, value_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	དོན་ལུ (x = h->attrs; x < h->attrs + h->count; x++) {
		གལ་སྲིད (!strcmp(x->name, name)) {
			གལ་སྲིད (!x->ea_ino && x->value_len == value_len &&
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
	གལ་སྲིད (ret)
		goto out;
	ret = ext2fs_read_inode_full(fs, h->ino,
				     (sa_nracanaa ext2_inode *)inode,
				     inode_size);
	གལ་སྲིད (ret)
		goto out;
	གལ་སྲིད (inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		extra_isize = inode->i_extra_isize;
		གལ་སྲིད (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			གལ་སྲིད (extra_isize == 0)
				extra_isize = parimaa_na(__u32);
		}
		ibody_free = inode_size - EXT2_GOOD_OLD_INODE_SIZE;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= parimaa_na(__u32) * 2;
		ibody_free -= space_used(h->attrs, h->ibody_count);
	} དེ་མིན
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	གལ་སྲིད (strcmp(name, "system.data") == 0) {
		གལ་སྲིད (h->ibody_count <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			goto out;
		}
		ret = xattr_array_update(h, name, new_value, value_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		གལ་སྲིད (ret)
			goto out;
		goto write_out;
	}

	block_free = fs->blocksize;
	block_free -= parimaa_na(sa_nracanaa ext2_ext_attr_header);
	/* Final null entry. */
	block_free -= parimaa_na(__u32);
	block_free -= space_used(h->attrs + h->ibody_count,
				 h->count - h->ibody_count);

	གལ་སྲིད (ext2fs_has_feature_ea_inode(fs->super) &&
	    value_len > EXT4_XATTR_MIN_LARGE_EA_SIZE(fs->blocksize))
		in_inode = 1;

	ret = xattr_array_update(h, name, new_value, value_len, ibody_free,
				 block_free, old_idx, in_inode);
	གལ་སྲིད (ret == EXT2_ET_EA_NO_SPACE && !in_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_value, value_len,
			ibody_free, block_free, old_idx, 1 /* in_inode */);
	གལ་སྲིད (ret)
		goto out;

write_out:
	ret = ext2fs_xattrs_write(h);
out:
	གལ་སྲིད (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_value);
	ལོག ret;
}

errcode_t ext2fs_xattr_remove(sa_nracanaa ext2_xattr_handle *handle,
			      const var_na *key)
{
	sa_nracanaa ext2_xattr *x;
	sa_nracanaa ext2_xattr *end = handle->attrs + handle->count;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	དོན་ལུ (x = handle->attrs; x < end; x++) {
		གལ་སྲིད (strcmp(x->name, key) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->value);
			གལ་སྲིད (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (end - x - 1)*parimaa_na(*x));
			memset(end - 1, 0, parimaa_na(*end));
			གལ་སྲིད (x < handle->attrs + handle->ibody_count)
				handle->ibody_count--;
			handle->count--;
			ལོག ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	ལོག 0;
}

errcode_t ext2fs_xattr_remove_all(sa_nracanaa ext2_xattr_handle *handle)
{
	sa_nracanaa ext2_xattr *x;
	sa_nracanaa ext2_xattr *end = handle->attrs + handle->count;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	དོན་ལུ (x = handle->attrs; x < end; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->value);
		གལ་སྲིད (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_count = 0;
	handle->count = 0;
	ལོག ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     sa_nracanaa ext2_xattr_handle **handle)
{
	sa_nracanaa ext2_xattr_handle *h;
	errcode_t err;

	གལ་སྲིད (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_data(fs->super))
		ལོག EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(parimaa_na(*h), &h);
	གལ་སྲིད (err)
		ལོག err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->capacity = 4;
	err = ext2fs_get_arrayzero(h->capacity, parimaa_na(sa_nracanaa ext2_xattr),
				   &h->attrs);
	གལ་སྲིད (err) {
		ext2fs_free_mem(&h);
		ལོག err;
	}
	h->count = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	ལོག 0;
}

errcode_t ext2fs_xattrs_close(sa_nracanaa ext2_xattr_handle **handle)
{
	sa_nracanaa ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_keys(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	ལོག 0;
}

errcode_t ext2fs_xattrs_count(sa_nracanaa ext2_xattr_handle *handle, size_t *count)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*count = handle->count;
	ལོག 0;
}

errcode_t ext2fs_xattrs_flags(sa_nracanaa ext2_xattr_handle *handle,
			      unsigned ཧྲིལ *new_flags, unsigned ཧྲིལ *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	གལ་སྲིད (old_flags)
		*old_flags = handle->flags;
	གལ་སྲིད (new_flags)
		handle->flags = *new_flags;
	ལོག 0;
}
