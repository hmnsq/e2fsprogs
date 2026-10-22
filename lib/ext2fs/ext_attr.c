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

#ضمن "config.h"
#ضمن <stdio.h>
#ܐܢ HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <string.h>
#ضمن <time.h>

#ضمن "ext2_fs.h"
#ضمن "ext2_ext_attr.h"
#ضمن "ext4_acl.h"

#ضمن "ext2fsP.h"

ساكن errcode_t read_ea_inode_hash(ext2_filsys fs, ext2_ino_t ino, __u32 *hash)
{
	بنية ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	ܐܢ (retval)
		ܦܢܐ retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	ܦܢܐ 0;
}

#define NAME_HASH_SHIFT 5
#define VALUE_HASH_SHIFT 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_entry(بنية ext2_ext_attr_entry *entry, فراغ *data)
{
	__u32 hash = 0;
	unsigned حرف *name = (((unsigned حرف *) entry) +
			       حجم(بنية ext2_ext_attr_entry));
	صحيح n;

	لكل (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*حجم(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	ܐܢ (entry->e_value_inum == 0 && entry->e_value_size != 0) {
		__u32 *value = (__u32 *)data;
		لكل (n = (entry->e_value_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << VALUE_HASH_SHIFT) ^
			       (hash >> (8*حجم(hash) - VALUE_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*value++);
		}
	}

	ܦܢܐ hash;
}

__u32 ext2fs_ext_attr_hash_entry_signed(بنية ext2_ext_attr_entry *entry,
					فراغ *data)
{
	__u32 hash = 0;
	signed حرف *name = (((signed حرف *) entry) +
			     حجم(بنية ext2_ext_attr_entry));
	صحيح n;

	لكل (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*حجم(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	ܐܢ (entry->e_value_inum == 0 && entry->e_value_size != 0) {
		__u32 *value = (__u32 *)data;
		لكل (n = (entry->e_value_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << VALUE_HASH_SHIFT) ^
			       (hash >> (8*حجم(hash) - VALUE_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*value++);
		}
	}

	ܦܢܐ hash;
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
				      بنية ext2_ext_attr_entry *entry,
				      فراغ *data, __u32 *hash,
				      __u32 *signed_hash)
{
	*hash = ext2fs_ext_attr_hash_entry(entry, data);
	ܐܢ (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_entry_signed(entry, data);

	ܐܢ (entry->e_value_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, entry->e_value_inum,
					    &ea_inode_hash);
		ܐܢ (retval)
			ܦܢܐ retval;

		*hash = (*hash << VALUE_HASH_SHIFT) ^
			(*hash >> (8*حجم(*hash) - VALUE_HASH_SHIFT)) ^
			ea_inode_hash;
		ܐܢ (signed_hash)
			*signed_hash = (*signed_hash << VALUE_HASH_SHIFT) ^
				(*signed_hash >> (8*حجم(*hash) -
						  VALUE_HASH_SHIFT)) ^
				ea_inode_hash;
	}
	ܦܢܐ 0;
}

/*
 * ext2fs_ext_attr_hash_entry2()
 *
 * Compute the hash of an extended attribute.
 * This version of the function supports hashing entries that reference
 * external inodes (ea_inode feature).
 */
errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
				      بنية ext2_ext_attr_entry *entry,
				      فراغ *data, __u32 *hash)
{
	ܦܢܐ ext2fs_ext_attr_hash_entry3(fs, entry, data, hash, عدم);
}

#undef NAME_HASH_SHIFT
#undef VALUE_HASH_SHIFT

#define BLOCK_HASH_SHIFT 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
فراغ ext2fs_ext_attr_block_rehash(بنية ext2_ext_attr_header *header,
				  بنية ext2_ext_attr_entry *end)
{
	بنية ext2_ext_attr_entry *here;
	__u32 hash = 0;

	here = (بنية ext2_ext_attr_entry *)(header+1);
	ܟܡܐ (here < end && !EXT2_EXT_IS_LAST_ENTRY(here)) {
		ܐܢ (!here->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			اكسر;
		}
		hash = (hash << BLOCK_HASH_SHIFT) ^
		       (hash >> (8*حجم(hash) - BLOCK_HASH_SHIFT)) ^
		       here->e_hash;
		here = EXT2_EXT_ATTR_NEXT(here);
	}
	header->h_hash = hash;
}

#undef BLOCK_HASH_SHIFT

__u32 ext2fs_get_ea_inode_hash(بنية ext2_inode *inode)
{
	ܦܢܐ inode->i_atime;
}

فراغ ext2fs_set_ea_inode_hash(بنية ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(بنية ext2_inode *inode)
{
	ܦܢܐ ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_version;
}

فراغ ext2fs_set_ea_inode_ref(بنية ext2_inode *inode, __u64 ref_count)
{
	inode->i_ctime = (__u32)(ref_count >> 32);
	inode->osd1.linux1.l_i_version = (__u32)ref_count;
}

ساكن errcode_t check_ext_attr_header(بنية ext2_ext_attr_header *header)
{
	ܐܢ ((header->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     header->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    header->h_blocks != 1)
		ܦܢܐ EXT2_ET_BAD_EA_HEADER;

	ܦܢܐ 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block, فراغ *buf,
				ext2_ino_t inum)
{
	صحيح		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	ܐܢ (retval)
		ܦܢܐ retval;

	ܐܢ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_block_csum_verify(fs, inum, block, buf))
		csum_failed = 1;

#ifdef WORDS_BIGENDIAN
	ext2fs_swap_ext_attr(buf, buf, fs->blocksize, 1);
#endif

	retval = check_ext_attr_header(buf);
	ܐܢ (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	ܦܢܐ retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block, فراغ *buf)
{
	ܦܢܐ ext2fs_read_ext_attr3(fs, block, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, فراغ *buf)
{
	ܦܢܐ ext2fs_read_ext_attr2(fs, block, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block, فراغ *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	حرف		*write_buf;

#ifdef WORDS_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	ܐܢ (retval)
		ܦܢܐ retval;
	ext2fs_swap_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#ܐܚܪܢܐ
	write_buf = (حرف *) inbuf;
#endif

	retval = ext2fs_ext_attr_block_csum_set(fs, inum, block,
			(بنية ext2_ext_attr_header *)write_buf);
	ܐܢ (retval)
		ܦܢܐ retval;

	retval = io_channel_write_blk64(fs->io, block, 1, write_buf);
#ifdef WORDS_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#endif
	ܐܢ (!retval)
		ext2fs_mark_changed(fs);
	ܦܢܐ retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block, فراغ *inbuf)
{
	ܦܢܐ ext2fs_write_ext_attr3(fs, block, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block, فراغ *inbuf)
{
	ܦܢܐ ext2fs_write_ext_attr2(fs, block, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    حرف *block_buf, صحيح adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	بنية ext2_ext_attr_header *header;
	حرف	*buf = 0;

	ܐܢ ((blk >= ext2fs_blocks_count(fs->super)) ||
	    (blk < fs->super->s_first_data_block))
		ܦܢܐ EXT2_ET_BAD_EA_BLOCK_NUM;

	ܐܢ (!block_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		ܐܢ (retval)
			ܦܢܐ retval;
		block_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, block_buf, inum);
	ܐܢ (retval)
		اذهب errout;

	header = (بنية ext2_ext_attr_header *) block_buf;
	header->h_refcount += adjust;
	ܐܢ (newcount)
		*newcount = header->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, block_buf, inum);
	ܐܢ (retval)
		اذهب errout;

errout:
	ܐܢ (buf)
		ext2fs_free_mem(&buf);
	ܦܢܐ retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    حرف *block_buf, صحيح adjust,
				    __u32 *newcount)
{
	ܦܢܐ ext2fs_adjust_ea_refcount3(fs, blk, block_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					حرف *block_buf, صحيح adjust,
					__u32 *newcount)
{
	ܦܢܐ ext2fs_adjust_ea_refcount2(fs, blk, block_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
بنية ext2_xattr {
	صحيح name_index;
	حرف *name;
	حرف *short_name;
	فراغ *value;
	unsigned صحيح value_len;
	ext2_ino_t ea_ino;
};

بنية ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	بنية ext2_xattr *attrs;
	صحيح capacity;
	صحيح count;
	صحيح ibody_count;
	ext2_ino_t ino;
	unsigned صحيح flags;
};

ساكن errcode_t ext2fs_xattrs_expand(بنية ext2_xattr_handle *h,
				      unsigned صحيح expandby)
{
	بنية ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->capacity + expandby,
				   حجم(بنية ext2_xattr), &new_attrs);
	ܐܢ (err)
		ܦܢܐ err;

	memcpy(new_attrs, h->attrs, h->capacity * حجم(بنية ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->capacity += expandby;
	h->attrs = new_attrs;

	ܦܢܐ 0;
}

بنية ea_name_index {
	صحيح index;
	ثابت حرف *name;
};

/* Keep these names sorted in order of decreasing specificity. */
ساكن بنية ea_name_index ea_names[] = {
	{10, "gnu."},
	{3, "system.posix_acl_default"},
	{2, "system.posix_acl_access"},
	{8, "system.richacl"},
	{6, "security."},
	{4, "trusted."},
	{7, "system."},
	{1, "user."},
	{0, عدم},
};

ساكن ثابت حرف *find_ea_prefix(صحيح index)
{
	بنية ea_name_index *e;

	لكل (e = ea_names; e->name; e++)
		ܐܢ (e->index == index)
			ܦܢܐ e->name;

	ܦܢܐ عدم;
}

ساكن صحيح find_ea_index(ثابت حرف *fullname, ثابت حرف **name, صحيح *index)
{
	بنية ea_name_index *e;

	لكل (e = ea_names; e->name; e++) {
		ܐܢ (strncmp(fullname, e->name, strlen(e->name)) == 0) {
			*name = fullname + strlen(e->name);
			*index = e->index;
			ܦܢܐ 1;
		}
	}
	ܦܢܐ 0;
}

errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       بنية ext2_inode_large *inode)
{
	بنية ext2_ext_attr_header *header;
	فراغ *block_buf = عدم;
	blk64_t blk;
	errcode_t err;
	بنية ext2_inode_large i;

	/* Read inode? */
	ܐܢ (inode == عدم) {
		err = ext2fs_read_inode_full(fs, ino, (بنية ext2_inode *)&i,
					     حجم(بنية ext2_inode_large));
		ܐܢ (err)
			ܦܢܐ err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (بنية ext2_inode *)inode);
	ܐܢ (blk == 0)
		ܦܢܐ 0;

	/* Find block, zero it, write back */
	ܐܢ ((blk < fs->super->s_first_data_block) ||
	    (blk >= ext2fs_blocks_count(fs->super))) {
		err = EXT2_ET_BAD_EA_BLOCK_NUM;
		اذهب out;
	}

	err = ext2fs_get_mem(fs->blocksize, &block_buf);
	ܐܢ (err)
		اذهب out;

	err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
	ܐܢ (err)
		اذهب out2;

	/* We only know how to deal with v2 EA blocks */
	header = (بنية ext2_ext_attr_header *) block_buf;
	ܐܢ (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_HEADER;
		اذهب out2;
	}

	header->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
	ܐܢ (err)
		اذهب out2;

	/* Erase link to block */
	ext2fs_file_acl_block_set(fs, (بنية ext2_inode *)inode, 0);
	ܐܢ (header->h_refcount == 0)
		ext2fs_block_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_blocks(fs, (بنية ext2_inode *)inode, 1);
	ܐܢ (err)
		اذهب out2;

	/* Write inode? */
	ܐܢ (inode == &i) {
		err = ext2fs_write_inode_full(fs, ino, (بنية ext2_inode *)&i,
					      حجم(بنية ext2_inode_large));
		ܐܢ (err)
			اذهب out2;
	}

out2:
	ext2fs_free_mem(&block_buf);
out:
	ܦܢܐ err;
}

ساكن errcode_t prep_ea_block_for_write(ext2_filsys fs, ext2_ino_t ino,
					 بنية ext2_inode_large *inode)
{
	بنية ext2_ext_attr_header *header;
	فراغ *block_buf = عدم;
	blk64_t blk, goal;
	errcode_t err;

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (بنية ext2_inode *)inode);
	ܐܢ (blk != 0) {
		ܐܢ ((blk < fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_count(fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_NUM;
			اذهب out;
		}

		err = ext2fs_get_mem(fs->blocksize, &block_buf);
		ܐܢ (err)
			اذهب out;

		err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
		ܐܢ (err)
			اذهب out2;

		/* We only know how to deal with v2 EA blocks */
		header = (بنية ext2_ext_attr_header *) block_buf;
		ܐܢ (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			اذهب out2;
		}

		/* Single-user block.  We're done here. */
		ܐܢ (header->h_refcount == 1)
			اذهب out2;

		/* We need to CoW the block. */
		header->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
		ܐܢ (err)
			اذهب out2;
	} ܐܚܪܢܐ {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_add_blocks(fs, (بنية ext2_inode *)inode,
					     1);
		ܐܢ (err)
			اذهب out;
	}

	/* Allocate a block */
	goal = ext2fs_find_inode_goal(fs, ino, (بنية ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, عدم, &blk);
	ܐܢ (err)
		اذهب out2;
	ext2fs_file_acl_block_set(fs, (بنية ext2_inode *)inode, blk);
out2:
	ܐܢ (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	ܦܢܐ err;
}


ساكن مضمن صحيح
posix_acl_xattr_count(size_t size)
{
        ܐܢ (size < حجم(posix_acl_xattr_header))
                ܦܢܐ -1;
        size -= حجم(posix_acl_xattr_header);
        ܐܢ (size % حجم(posix_acl_xattr_entry))
                ܦܢܐ -1;
        ܦܢܐ size / حجم(posix_acl_xattr_entry);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
ساكن errcode_t convert_posix_acl_to_disk_buffer(ثابت فراغ *value, size_t size,
						  فراغ *out_buf, size_t *size_out)
{
	ثابت posix_acl_xattr_header *header =
		(ثابت posix_acl_xattr_header*) value;
	ثابت posix_acl_xattr_entry *end, *entry =
		(ثابت posix_acl_xattr_entry *)(header+1);
	ext4_acl_header *ext_acl;
	size_t s;
	حرف *e;

	صحيح count;

	ܐܢ (!value)
		ܦܢܐ EINVAL;
	ܐܢ (size < حجم(posix_acl_xattr_header))
		ܦܢܐ ENOMEM;
	ܐܢ (header->a_version != ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION))
		ܦܢܐ EINVAL;

	count = posix_acl_xattr_count(size);
	ext_acl = out_buf;
	ext_acl->a_version = ext2fs_cpu_to_le32(EXT4_ACL_VERSION);

	ܐܢ (count <= 0)
		ܦܢܐ EINVAL;

	e = (حرف *) out_buf + حجم(ext4_acl_header);
	s = حجم(ext4_acl_header);
	لكل (end = entry + count; entry != end;entry++) {
		ext4_acl_entry *disk_entry = (ext4_acl_entry*) e;
		disk_entry->e_tag = entry->e_tag;
		disk_entry->e_perm = entry->e_perm;

		بدل(ext2fs_le16_to_cpu(entry->e_tag)) {
			حالة ACL_USER_OBJ:
			حالة ACL_GROUP_OBJ:
			حالة ACL_MASK:
			حالة ACL_OTHER:
				e += حجم(ext4_acl_entry_short);
				s += حجم(ext4_acl_entry_short);
				اكسر;
			حالة ACL_USER:
			حالة ACL_GROUP:
				disk_entry->e_id = entry->e_id;
				e += حجم(ext4_acl_entry);
				s += حجم(ext4_acl_entry);
				اكسر;
			افتراضي:
				ܦܢܐ EINVAL;
		}
	}
	*size_out = s;
	ܦܢܐ 0;
}

ساكن errcode_t convert_disk_buffer_to_posix_acl(ثابت فراغ *value, size_t size,
						  فراغ **out_buf, size_t *size_out)
{
	posix_acl_xattr_header *header;
	posix_acl_xattr_entry *entry;
	ثابت ext4_acl_header *ext_acl = (ثابت ext4_acl_header *) value;
	errcode_t err;
	ثابت حرف *cp;
	حرف *out;

	ܐܢ ((!value) ||
	    (size < حجم(ext4_acl_header)) ||
	    (ext_acl->a_version != ext2fs_cpu_to_le32(EXT4_ACL_VERSION)))
		ܦܢܐ EINVAL;

	err = ext2fs_get_mem(size * 2, &out);
	ܐܢ (err)
		ܦܢܐ err;

	header = (posix_acl_xattr_header *) out;
	header->a_version = ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION);
	entry = (posix_acl_xattr_entry *) (out + حجم(posix_acl_xattr_header));

	cp = (ثابت حرف *) value + حجم(ext4_acl_header);
	size -= حجم(ext4_acl_header);

	ܟܡܐ (size > 0) {
		ثابت ext4_acl_entry *disk_entry = (ثابت ext4_acl_entry *) cp;

		entry->e_tag = disk_entry->e_tag;
		entry->e_perm = disk_entry->e_perm;

		بدل(ext2fs_le16_to_cpu(entry->e_tag)) {
			حالة ACL_USER_OBJ:
			حالة ACL_GROUP_OBJ:
			حالة ACL_MASK:
			حالة ACL_OTHER:
				entry->e_id = 0;
				cp += حجم(ext4_acl_entry_short);
				size -= حجم(ext4_acl_entry_short);
				اكسر;
			حالة ACL_USER:
			حالة ACL_GROUP:
				entry->e_id = disk_entry->e_id;
				cp += حجم(ext4_acl_entry);
				size -= حجم(ext4_acl_entry);
				اكسر;
			افتراضي:
				ext2fs_free_mem(&out);
				ܦܢܐ EINVAL;
		}
		entry++;
	}
	*out_buf = out;
	*size_out = ((حرف *) entry - out);
	ܦܢܐ 0;
}

ساكن errcode_t
write_xattrs_to_buffer(ext2_filsys fs, بنية ext2_xattr *attrs, صحيح count,
		       فراغ *entries_start, unsigned صحيح storage_size,
		       unsigned صحيح value_offset_correction, صحيح write_hash)
{
	بنية ext2_xattr *x;
	بنية ext2_ext_attr_entry *e = entries_start;
	حرف *end = (حرف *) entries_start + storage_size;
	unsigned صحيح value_size;
	errcode_t err;

	memset(entries_start, 0, storage_size);
	لكل (x = attrs; x < attrs + count; x++) {
		value_size = ((x->value_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = strlen(x->short_name);
		e->e_name_index = x->name_index;

		e->e_value_size = x->value_len;
		e->e_value_inum = x->ea_ino;

		/* Store name */
		memcpy((حرف *)e + حجم(*e), x->short_name, e->e_name_len);
		ܐܢ (x->ea_ino) {
			e->e_value_offs = 0;
		} ܐܚܪܢܐ {
			end -= value_size;
			e->e_value_offs = end - (حرف *) entries_start +
						value_offset_correction;
			memcpy(end, x->value, e->e_value_size);
		}

		ܐܢ (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : end,
							  &e->e_hash);
			ܐܢ (err)
				ܦܢܐ err;
		} ܐܚܪܢܐ
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_NEXT(e);
		*(__u32 *)e = 0;
	}
	ܦܢܐ 0;
}

errcode_t ext2fs_xattrs_write(بنية ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	ثابت unsigned صحيح inode_size = EXT2_INODE_SIZE(fs->super);
	بنية ext2_inode_large *inode;
	حرف *start, *block_buf = عدم;
	بنية ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	blk64_t blk;
	unsigned صحيح storage_size;
	unsigned صحيح i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_size;
	ܐܢ (i < حجم(*inode))
		i = حجم(*inode);
	err = ext2fs_get_memzero(i, &inode);
	ܐܢ (err)
		ܦܢܐ err;

	err = ext2fs_read_inode_full(fs, handle->ino, EXT2_INODE(inode),
				     inode_size);
	ܐܢ (err)
		اذهب out;

	/* If extra_isize isn't set, we need to set it now */
	ܐܢ (inode->i_extra_isize == 0 &&
	    inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		حرف *p = (حرف *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		ܐܢ (extra == 0)
			extra = حجم(__u32);
		memset(p + EXT2_GOOD_OLD_INODE_SIZE, 0, extra);
		inode->i_extra_isize = extra;
	}
	ܐܢ (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		اذهب out;
	}

	/* Does the inode have space for EA? */
	ܐܢ (inode->i_extra_isize < حجم(inode->i_extra_isize) ||
	    inode_size <= EXT2_GOOD_OLD_INODE_SIZE + inode->i_extra_isize +
								حجم(__u32))
		اذهب write_ea_block;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	memcpy(((حرف *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, &ea_inode_magic, حجم(__u32));
	storage_size = inode_size - EXT2_GOOD_OLD_INODE_SIZE -
				inode->i_extra_isize - حجم(__u32);
	start = ((حرف *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
				inode->i_extra_isize + حجم(__u32);

	err = write_xattrs_to_buffer(fs, handle->attrs, handle->ibody_count,
				     start, storage_size, 0, 0);
	ܐܢ (err)
		اذهب out;
write_ea_block:
	/* Are we done? */
	ܐܢ (handle->ibody_count == handle->count &&
	    !ext2fs_file_acl_block(fs, EXT2_INODE(inode)))
		اذهب skip_ea_block;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &block_buf);
	ܐܢ (err)
		اذهب out;

	storage_size = fs->blocksize - حجم(بنية ext2_ext_attr_header);
	start = block_buf + حجم(بنية ext2_ext_attr_header);

	err = write_xattrs_to_buffer(fs, handle->attrs + handle->ibody_count,
				     handle->count - handle->ibody_count, start,
				     storage_size, start - block_buf, 1);
	ܐܢ (err)
		اذهب out2;

	/* Write a header on the EA block */
	header = (بنية ext2_ext_attr_header *) block_buf;
	header->h_magic = EXT2_EXT_ATTR_MAGIC;
	header->h_refcount = 1;
	header->h_blocks = 1;

	/* Get a new block for writing */
	err = prep_ea_block_for_write(fs, handle->ino, inode);
	ܐܢ (err)
		اذهب out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_block(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, handle->ino);
	ܐܢ (err)
		اذهب out2;

skip_ea_block:
	blk = ext2fs_file_acl_block(fs, (بنية ext2_inode *)inode);
	ܐܢ (!block_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		ܐܢ (err)
			اذهب out;
	}

	/* Write the inode */
	err = ext2fs_write_inode_full(fs, handle->ino, EXT2_INODE(inode),
				      inode_size);
	ܐܢ (err)
		اذهب out2;

out2:
	ext2fs_free_mem(&block_buf);
out:
	ext2fs_free_mem(&inode);
	ܦܢܐ err;
}

ساكن errcode_t read_xattrs_from_buffer(بنية ext2_xattr_handle *handle,
					 بنية ext2_inode_large *inode,
					 بنية ext2_ext_attr_entry *entries,
					 unsigned صحيح storage_size,
					 حرف *value_start)
{
	بنية ext2_xattr *x;
	بنية ext2_ext_attr_entry *entry, *end;
	ثابت حرف *prefix;
	unsigned صحيح remain, prefix_len;
	errcode_t err;
	unsigned صحيح values_size = storage_size +
			((حرف *)entries - value_start);

	/* find the end */
	end = entries;
	remain = storage_size;
	ܟܡܐ (remain >= حجم(بنية ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(end)) {

		/* header eats this space */
		remain -= حجم(بنية ext2_ext_attr_entry);

		/* is attribute name valid? */
		ܐܢ (EXT2_EXT_ATTR_SIZE(end->e_name_len) > remain)
			ܦܢܐ EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(end->e_name_len);
		end = EXT2_EXT_ATTR_NEXT(end);
	}

	entry = entries;
	remain = storage_size;
	ܟܡܐ (remain >= حجم(بنية ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(entry)) {

		/* Allocate space for more attrs? */
		ܐܢ (handle->count == handle->capacity) {
			err = ext2fs_xattrs_expand(handle, 4);
			ܐܢ (err)
				ܦܢܐ err;
		}

		x = handle->attrs + handle->count;

		/* header eats this space */
		remain -= حجم(بنية ext2_ext_attr_entry);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(entry->e_name_len);

		/* Extract name */
		prefix = find_ea_prefix(entry->e_name_index);
		prefix_len = (prefix ? strlen(prefix) : 0);
		err = ext2fs_get_memzero(entry->e_name_len + prefix_len + 1,
					 &x->name);
		ܐܢ (err)
			ܦܢܐ err;
		ܐܢ (prefix)
			memcpy(x->name, prefix, prefix_len);
		ܐܢ (entry->e_name_len)
			memcpy(x->name + prefix_len,
			       (حرف *)entry + حجم(*entry),
			       entry->e_name_len);
		x->short_name = x->name + prefix_len;
		x->name_index = entry->e_name_index;

		/* Check & copy value */
		ܐܢ (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    entry->e_value_inum != 0)
			ܦܢܐ EXT2_ET_BAD_EA_BLOCK_NUM;

		ܐܢ (entry->e_value_inum == 0) {
			ܐܢ (entry->e_value_size > remain)
				ܦܢܐ EXT2_ET_EA_BAD_VALUE_SIZE;

			ܐܢ (entry->e_value_offs + entry->e_value_size > values_size)
				ܦܢܐ EXT2_ET_EA_BAD_VALUE_OFFSET;

			ܐܢ (entry->e_value_size > 0 &&
			    value_start + entry->e_value_offs <
			    (حرف *)end + حجم(__u32))
				ܦܢܐ EXT2_ET_EA_BAD_VALUE_OFFSET;

			remain -= entry->e_value_size;

			err = ext2fs_get_mem(entry->e_value_size, &x->value);
			ܐܢ (err)
				ܦܢܐ err;
			memcpy(x->value, value_start + entry->e_value_offs,
			       entry->e_value_size);
		} ܐܚܪܢܐ {
			بنية ext2_inode *ea_inode;
			ext2_file_t ea_file;

			ܐܢ (entry->e_value_offs != 0)
				ܦܢܐ EXT2_ET_EA_BAD_VALUE_OFFSET;

			ܐܢ (entry->e_value_size > (64 * 1024))
				ܦܢܐ EXT2_ET_EA_BAD_VALUE_SIZE;

			err = ext2fs_get_mem(entry->e_value_size, &x->value);
			ܐܢ (err)
				ܦܢܐ err;

			err = ext2fs_file_open(handle->fs, entry->e_value_inum,
					       0, &ea_file);
			ܐܢ (err)
				ܦܢܐ err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			ܐܢ ((ea_inode->i_flags & EXT4_INLINE_DATA_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_count == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			ܐܚܪܢܐ ܐܢ ((__u64) ext2fs_file_get_size(ea_file) !=
				 entry->e_value_size)
				err = EXT2_ET_EA_BAD_VALUE_SIZE;
			ܐܚܪܢܐ
				err = ext2fs_file_read(ea_file, x->value,
						       entry->e_value_size, 0);
			ext2fs_file_close(ea_file);
			ܐܢ (err)
				ܦܢܐ err;
		}

		x->ea_ino = entry->e_value_inum;
		x->value_len = entry->e_value_size;

		/* e_hash may be 0 in older inode's ea */
		ܐܢ (entry->e_hash != 0) {
			__u32 hash, signed_hash;

			فراغ *data = (entry->e_value_inum != 0) ?
					0 : value_start + entry->e_value_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, entry,
							  data, &hash,
							  &signed_hash);
			ܐܢ (err)
				ܦܢܐ err;
			ܐܢ ((entry->e_hash != hash) &&
			    (entry->e_hash != signed_hash)) {
				بنية ext2_inode child;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							entry->e_value_inum,
							&child);
				ܐܢ (err)
					ܦܢܐ err;
				ܐܢ (child.i_mtime != handle->ino ||
				    child.i_generation != inode->i_generation)
					ܦܢܐ EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->count++;
		entry = EXT2_EXT_ATTR_NEXT(entry);
	}

	ܦܢܐ 0;
}

ساكن فراغ xattrs_free_keys(بنية ext2_xattr_handle *h)
{
	بنية ext2_xattr *a = h->attrs;
	صحيح i;

	لكل (i = 0; i < h->capacity; i++) {
		ܐܢ (a[i].name)
			ext2fs_free_mem(&a[i].name);
		ܐܢ (a[i].value)
			ext2fs_free_mem(&a[i].value);
	}
	h->count = 0;
	h->ibody_count = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(بنية ext2_xattr_handle *handle,
				   بنية ext2_inode_large *inode)
{
	بنية ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	unsigned صحيح storage_size;
	حرف *start, *block_buf = عدم;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_keys(handle);

	/* Does the inode have space for EA? */
	ܐܢ (inode->i_extra_isize < حجم(inode->i_extra_isize) ||
	    EXT2_INODE_SIZE(handle->fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  حجم(__u32))
		اذهب read_ea_block;
	ܐܢ (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		اذهب out;
	}

	/* Look for EA in the inode */
	memcpy(&ea_inode_magic, ((حرف *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, حجم(__u32));
	ܐܢ (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_size = EXT2_INODE_SIZE(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_SIZE - inode->i_extra_isize -
			حجم(__u32);
		start = ((حرف *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + حجم(__u32);

		err = read_xattrs_from_buffer(handle, inode,
					(بنية ext2_ext_attr_entry *) start,
					storage_size, start);
		ܐܢ (err)
			اذهب out;

		handle->ibody_count = handle->count;
	}

read_ea_block:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_block(handle->fs, EXT2_INODE(inode));
	ܐܢ (blk != 0) {
		ܐܢ ((blk < handle->fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_count(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_NUM;
			اذهب out;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &block_buf);
		ܐܢ (err)
			اذهب out;

		err = ext2fs_read_ext_attr3(handle->fs, blk, block_buf,
					    handle->ino);
		ܐܢ (err)
			اذهب out3;

		/* We only know how to deal with v2 EA blocks */
		header = (بنية ext2_ext_attr_header *) block_buf;
		ܐܢ (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			اذهب out3;
		}

		/* Read EAs */
		storage_size = handle->fs->blocksize -
			حجم(بنية ext2_ext_attr_header);
		start = block_buf + حجم(بنية ext2_ext_attr_header);
		err = read_xattrs_from_buffer(handle, inode,
					(بنية ext2_ext_attr_entry *) start,
					storage_size, block_buf);
	}

out3:
	ܐܢ (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	ܦܢܐ err;
}

errcode_t ext2fs_xattrs_read(بنية ext2_xattr_handle *handle)
{
	بنية ext2_inode_large *inode;
	size_t inode_size = EXT2_INODE_SIZE(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	ܐܢ (inode_size < حجم(*inode))
		inode_size = حجم(*inode);
	err = ext2fs_get_memzero(inode_size, &inode);
	ܐܢ (err)
		ܦܢܐ err;

	err = ext2fs_read_inode_full(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_SIZE(handle->fs->super));
	ܐܢ (err)
		اذهب out;

	err = ext2fs_xattrs_read_inode(handle, inode);

out:
	ext2fs_free_mem(&inode);

	ܦܢܐ err;
}

errcode_t ext2fs_xattrs_iterate(بنية ext2_xattr_handle *h,
				صحيح (*func)(حرف *name, حرف *value,
					    size_t value_len, فراغ *data),
				فراغ *data)
{
	بنية ext2_xattr *x;
	صحيح dirty = 0;
	صحيح ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = h->attrs; x < h->attrs + h->count; x++) {
		ret = func(x->name, x->value, x->value_len, data);
		ܐܢ (ret & XATTR_CHANGED)
			dirty = 1;
		ܐܢ (ret & XATTR_ABORT)
			اكسر;
	}

	ܐܢ (dirty)
		ܦܢܐ ext2fs_xattrs_write(h);
	ܦܢܐ 0;
}

errcode_t ext2fs_xattr_get(بنية ext2_xattr_handle *h, ثابت حرف *key,
			   فراغ **value, size_t *value_len)
{
	بنية ext2_xattr *x;
	حرف *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = h->attrs; x < h->attrs + h->count; x++) {
		ܐܢ (strcmp(x->name, key))
			استمر;

		ܐܢ (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(key, "system.posix_acl_default") == 0) ||
		     (strcmp(key, "system.posix_acl_access") == 0))) {
			err = convert_disk_buffer_to_posix_acl(x->value, x->value_len,
							       value, value_len);
			ܦܢܐ err;
		} ܐܚܪܢܐ {
			err = ext2fs_get_mem(x->value_len, &val);
			ܐܢ (err)
				ܦܢܐ err;
			memcpy(val, x->value, x->value_len);
			*value = val;
			*value_len = x->value_len;
			ܦܢܐ 0;
		}
	}

	ܦܢܐ EXT2_ET_EA_KEY_NOT_FOUND;
}

errcode_t ext2fs_xattr_inode_max_size(ext2_filsys fs, ext2_ino_t ino,
				      size_t *size)
{
	بنية ext2_ext_attr_entry *entry;
	بنية ext2_inode_large *inode;
	__u32 ea_inode_magic;
	unsigned صحيح minoff;
	حرف *start;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_SIZE(fs->super);
	ܐܢ (i < حجم(*inode))
		i = حجم(*inode);
	err = ext2fs_get_memzero(i, &inode);
	ܐܢ (err)
		ܦܢܐ err;

	err = ext2fs_read_inode_full(fs, ino, (بنية ext2_inode *)inode,
				     EXT2_INODE_SIZE(fs->super));
	ܐܢ (err)
		اذهب out;

	/* Does the inode have size for EA? */
	ܐܢ (EXT2_INODE_SIZE(fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  حجم(__u32)) {
		err = EXT2_ET_INLINE_DATA_NO_SPACE;
		اذهب out;
	}

	minoff = EXT2_INODE_SIZE(fs->super) - حجم(*inode) - حجم(__u32);
	memcpy(&ea_inode_magic, ((حرف *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, حجم(__u32));
	ܐܢ (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		start= ((حرف *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + حجم(__u32);
		entry = (بنية ext2_ext_attr_entry *) start;
		ܟܡܐ (!EXT2_EXT_IS_LAST_ENTRY(entry)) {
			ܐܢ (!entry->e_value_inum && entry->e_value_size) {
				unsigned صحيح offs = entry->e_value_offs;
				ܐܢ (offs < minoff)
					minoff = offs;
			}
			entry = EXT2_EXT_ATTR_NEXT(entry);
		}
		*size = minoff - ((حرف *)entry - (حرف *)start) - حجم(__u32);
	} ܐܚܪܢܐ {
		/* no xattr.  return a maximum size */
		*size = EXT2_EXT_ATTR_SIZE(minoff -
					   EXT2_EXT_ATTR_LEN(strlen("data")) -
					   EXT2_EXT_ATTR_ROUND - حجم(__u32));
	}

out:
	ext2fs_free_mem(&inode);
	ܦܢܐ err;
}

ساكن errcode_t xattr_create_ea_inode(ext2_filsys fs, ثابت فراغ *value,
				       size_t value_len, ext2_ino_t *ea_ino)
{
	بنية ext2_inode inode;
	ext2_ino_t ino;
	ext2_file_t file;
	__u32 hash;
	errcode_t ret;

	ret = ext2fs_new_inode(fs, 0, 0, 0, &ino);
	ܐܢ (ret)
		ܦܢܐ ret;

	memset(&inode, 0, حجم(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	ܐܢ (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_size = 0;
	inode.i_mode = LINUX_S_IFREG | 0600;
	inode.i_links_count = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	ܐܢ (ret)
		ܦܢܐ ret;
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
	ܐܢ (ret)
		ܦܢܐ ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	ܐܢ (ret)
		ܦܢܐ ret;
	ret = ext2fs_file_write(file, value, value_len, عدم);
	ext2fs_file_close(file);
	ܐܢ (ret)
		ܦܢܐ ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	ܦܢܐ 0;
}

ساكن errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	بنية ext2_inode_large inode;
	__u64 ref_count;
	errcode_t ret;

	ret = ext2fs_read_inode_full(fs, ino, (بنية ext2_inode *)&inode,
				     حجم(inode));
	ܐܢ (ret)
		اذهب out;

	ref_count = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_count--;
	ext2fs_set_ea_inode_ref(EXT2_INODE(&inode), ref_count);

	ܐܢ (ref_count)
		اذهب write_out;

	inode.i_links_count = 0;
	ext2fs_set_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	ܐܢ (ret)
		اذهب write_out;

	ܐܢ (ext2fs_inode_has_valid_blocks2(fs, (بنية ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (بنية ext2_inode *)&inode, عدم,
				   0, ~0ULL);
		ܐܢ (ret)
			اذهب out;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_out:
	ret = ext2fs_write_inode_full(fs, ino, (بنية ext2_inode *)&inode,
				      حجم(inode));
out:
	ܦܢܐ ret;
}

ساكن errcode_t xattr_update_entry(ext2_filsys fs, بنية ext2_xattr *x,
				    ثابت حرف *name, ثابت حرف *short_name,
				    صحيح index, ثابت فراغ *value,
				    size_t value_len, صحيح in_inode)
{
	ext2_ino_t ea_ino = 0;
	فراغ *new_value = عدم;
	حرف *new_name = عدم;
	صحيح name_len;
	errcode_t ret;

	ܐܢ (!x->name) {
		name_len = strlen(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		ܐܢ (ret)
			اذهب fail;
		memcpy(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(value_len, &new_value);
	ܐܢ (ret)
		اذهب fail;
	memcpy(new_value, value, value_len);

	ܐܢ (in_inode) {
		ret = xattr_create_ea_inode(fs, value, value_len, &ea_ino);
		ܐܢ (ret)
			اذهب fail;
	}

	ܐܢ (x->ea_ino) {
		ret = xattr_inode_dec_ref(fs, x->ea_ino);
		ܐܢ (ret)
			اذهب fail;
	}

	ܐܢ (!x->name) {
		x->name = new_name;
		x->short_name = new_name + (short_name  - name);
	}
	x->name_index = index;

	ܐܢ (x->value)
		ext2fs_free_mem(&x->value);
	x->value = new_value;
	x->value_len = value_len;
	x->ea_ino = ea_ino;
	ܦܢܐ 0;
fail:
	ܐܢ (new_name)
		ext2fs_free_mem(&new_name);
	ܐܢ (new_value)
		ext2fs_free_mem(&new_value);
	ܐܢ (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	ܦܢܐ ret;
}

ساكن صحيح xattr_find_position(بنية ext2_xattr *attrs, صحيح count,
			       ثابت حرف *shortname, صحيح name_idx)
{
	بنية ext2_xattr *x;
	صحيح i;
	صحيح shortname_len, x_shortname_len;

	shortname_len = strlen(shortname);

	لكل (i = 0, x = attrs; i < count; i++, x++) {
		ܐܢ (name_idx < x->name_index)
			اكسر;
		ܐܢ (name_idx > x->name_index)
			استمر;

		x_shortname_len = strlen(x->short_name);
		ܐܢ (shortname_len < x_shortname_len)
			اكسر;
		ܐܢ (shortname_len > x_shortname_len)
			استمر;

		ܐܢ (memcmp(shortname, x->short_name, shortname_len) <= 0)
			اكسر;
	}
	ܦܢܐ i;
}

ساكن errcode_t xattr_array_update(بنية ext2_xattr_handle *h,
				    ثابت حرف *name,
				    ثابت فراغ *value, size_t value_len,
				    صحيح ibody_free, صحيح block_free,
				    صحيح old_idx, صحيح in_inode)
{
	بنية ext2_xattr tmp;
	صحيح add_to_ibody;
	صحيح needed;
	صحيح name_len, name_idx = 0;
	ثابت حرف *shortname = name;
	صحيح new_idx;
	صحيح ret;

	find_ea_index(name, &shortname, &name_idx);
	name_len = strlen(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	ܐܢ (!in_inode)
		needed += EXT2_EXT_ATTR_SIZE(value_len);

	ܐܢ (old_idx >= 0 && old_idx < h->ibody_count) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		ܐܢ (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_SIZE(
						h->attrs[old_idx].value_len);
	}

	ܐܢ (needed <= ibody_free) {
		ܐܢ (old_idx < 0) {
			new_idx = h->ibody_count;
			add_to_ibody = 1;
			اذهب add_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, value,
					 value_len, in_inode);
		ܐܢ (ret)
			ܦܢܐ ret;
		ܐܢ (h->ibody_count <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_count + 1,
				h->attrs + h->ibody_count,
				(old_idx - h->ibody_count) * حجم(*h->attrs));
			h->attrs[h->ibody_count] = tmp;
			h->ibody_count++;
		}
		ܦܢܐ 0;
	}

	ܐܢ (h->ibody_count <= old_idx) {
		block_free += EXT2_EXT_ATTR_LEN(name_len);
		ܐܢ (!h->attrs[old_idx].ea_ino)
			block_free +=
				EXT2_EXT_ATTR_SIZE(h->attrs[old_idx].value_len);
	}

	ܐܢ (needed > block_free)
		ܦܢܐ EXT2_ET_EA_NO_SPACE;

	ܐܢ (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, value,
					 value_len, in_inode);
		ܐܢ (ret)
			ܦܢܐ ret;
		ܐܢ (old_idx < h->ibody_count) {
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
				(new_idx - old_idx) * حجم(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_count--;
		}
		ܦܢܐ 0;
	}

	new_idx = xattr_find_position(h->attrs + h->ibody_count,
				      h->count - h->ibody_count,
				      shortname, name_idx);
	new_idx += h->ibody_count;
	add_to_ibody = 0;

add_new:
	ܐܢ (h->count == h->capacity) {
		ret = ext2fs_xattrs_expand(h, 4);
		ܐܢ (ret)
			ܦܢܐ ret;
	}

	ret = xattr_update_entry(h->fs, &h->attrs[h->count], name, shortname,
				 name_idx, value, value_len, in_inode);
	ܐܢ (ret)
		ܦܢܐ ret;

	tmp = h->attrs[h->count];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->count - new_idx)*حجم(*h->attrs));
	h->attrs[new_idx] = tmp;
	ܐܢ (add_to_ibody)
		h->ibody_count++;
	h->count++;
	ܦܢܐ 0;
}

ساكن صحيح space_used(بنية ext2_xattr *attrs, صحيح count)
{
	صحيح total = 0;
	بنية ext2_xattr *x;
	صحيح i, len;

	لكل (i = 0, x = attrs; i < count; i++, x++) {
		len = strlen(x->short_name);
		total += EXT2_EXT_ATTR_LEN(len);
		ܐܢ (!x->ea_ino)
			total += EXT2_EXT_ATTR_SIZE(x->value_len);
	}
	ܦܢܐ total;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#define EXT4_XATTR_MIN_LARGE_EA_SIZE(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - حجم(بنية ext2_ext_attr_header) - 4)

errcode_t ext2fs_xattr_set(بنية ext2_xattr_handle *h,
			   ثابت حرف *name,
			   ثابت فراغ *value,
			   size_t value_len)
{
	ext2_filsys fs = h->fs;
	ثابت صحيح inode_size = EXT2_INODE_SIZE(fs->super);
	بنية ext2_inode_large *inode = عدم;
	بنية ext2_xattr *x;
	حرف *new_value;
	صحيح ibody_free, block_free;
	صحيح in_inode = 0;
	صحيح old_idx = -1;
	صحيح extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(value_len, &new_value);
	ܐܢ (ret)
		ܦܢܐ ret;
	ܐܢ (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((strcmp(name, "system.posix_acl_default") == 0) ||
	     (strcmp(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_to_disk_buffer(value, value_len,
						       new_value, &value_len);
		ܐܢ (ret)
			اذهب out;
	} ܐܚܪܢܐ ܐܢ (value_len)
		memcpy(new_value, value, value_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	لكل (x = h->attrs; x < h->attrs + h->count; x++) {
		ܐܢ (!strcmp(x->name, name)) {
			ܐܢ (!x->ea_ino && x->value_len == value_len &&
			    (!value_len ||
			     !memcmp(x->value, new_value, value_len))) {
				ret = 0;
				اذهب out;
			}
			old_idx = x - h->attrs;
			اكسر;
		}
	}

	ret = ext2fs_get_memzero(inode_size, &inode);
	ܐܢ (ret)
		اذهب out;
	ret = ext2fs_read_inode_full(fs, h->ino,
				     (بنية ext2_inode *)inode,
				     inode_size);
	ܐܢ (ret)
		اذهب out;
	ܐܢ (inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		extra_isize = inode->i_extra_isize;
		ܐܢ (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			ܐܢ (extra_isize == 0)
				extra_isize = حجم(__u32);
		}
		ibody_free = inode_size - EXT2_GOOD_OLD_INODE_SIZE;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= حجم(__u32) * 2;
		ibody_free -= space_used(h->attrs, h->ibody_count);
	} ܐܚܪܢܐ
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	ܐܢ (strcmp(name, "system.data") == 0) {
		ܐܢ (h->ibody_count <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			اذهب out;
		}
		ret = xattr_array_update(h, name, new_value, value_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		ܐܢ (ret)
			اذهب out;
		اذهب write_out;
	}

	block_free = fs->blocksize;
	block_free -= حجم(بنية ext2_ext_attr_header);
	/* Final null entry. */
	block_free -= حجم(__u32);
	block_free -= space_used(h->attrs + h->ibody_count,
				 h->count - h->ibody_count);

	ܐܢ (ext2fs_has_feature_ea_inode(fs->super) &&
	    value_len > EXT4_XATTR_MIN_LARGE_EA_SIZE(fs->blocksize))
		in_inode = 1;

	ret = xattr_array_update(h, name, new_value, value_len, ibody_free,
				 block_free, old_idx, in_inode);
	ܐܢ (ret == EXT2_ET_EA_NO_SPACE && !in_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_value, value_len,
			ibody_free, block_free, old_idx, 1 /* in_inode */);
	ܐܢ (ret)
		اذهب out;

write_out:
	ret = ext2fs_xattrs_write(h);
out:
	ܐܢ (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_value);
	ܦܢܐ ret;
}

errcode_t ext2fs_xattr_remove(بنية ext2_xattr_handle *handle,
			      ثابت حرف *key)
{
	بنية ext2_xattr *x;
	بنية ext2_xattr *end = handle->attrs + handle->count;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = handle->attrs; x < end; x++) {
		ܐܢ (strcmp(x->name, key) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->value);
			ܐܢ (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (end - x - 1)*حجم(*x));
			memset(end - 1, 0, حجم(*end));
			ܐܢ (x < handle->attrs + handle->ibody_count)
				handle->ibody_count--;
			handle->count--;
			ܦܢܐ ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	ܦܢܐ 0;
}

errcode_t ext2fs_xattr_remove_all(بنية ext2_xattr_handle *handle)
{
	بنية ext2_xattr *x;
	بنية ext2_xattr *end = handle->attrs + handle->count;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = handle->attrs; x < end; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->value);
		ܐܢ (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_count = 0;
	handle->count = 0;
	ܦܢܐ ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_xattr_handle **handle)
{
	بنية ext2_xattr_handle *h;
	errcode_t err;

	ܐܢ (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_data(fs->super))
		ܦܢܐ EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(حجم(*h), &h);
	ܐܢ (err)
		ܦܢܐ err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->capacity = 4;
	err = ext2fs_get_arrayzero(h->capacity, حجم(بنية ext2_xattr),
				   &h->attrs);
	ܐܢ (err) {
		ext2fs_free_mem(&h);
		ܦܢܐ err;
	}
	h->count = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	ܦܢܐ 0;
}

errcode_t ext2fs_xattrs_close(بنية ext2_xattr_handle **handle)
{
	بنية ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_keys(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	ܦܢܐ 0;
}

errcode_t ext2fs_xattrs_count(بنية ext2_xattr_handle *handle, size_t *count)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*count = handle->count;
	ܦܢܐ 0;
}

errcode_t ext2fs_xattrs_flags(بنية ext2_xattr_handle *handle,
			      unsigned صحيح *new_flags, unsigned صحيح *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	ܐܢ (old_flags)
		*old_flags = handle->flags;
	ܐܢ (new_flags)
		handle->flags = *new_flags;
	ܦܢܐ 0;
}
