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
#إذا HAVE_UNISTD_H
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
	إذا (retval)
		إرجاع retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	إرجاع 0;
}

#define NAME_HASH_SHIFT 5
#define قيمة_HASH_SHIFT 16

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
	إذا (entry->e_قيمة_inum == 0 && entry->e_قيمة_size != 0) {
		__u32 *قيمة = (__u32 *)data;
		لكل (n = (entry->e_قيمة_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << قيمة_HASH_SHIFT) ^
			       (hash >> (8*حجم(hash) - قيمة_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*قيمة++);
		}
	}

	إرجاع hash;
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
	إذا (entry->e_قيمة_inum == 0 && entry->e_قيمة_size != 0) {
		__u32 *قيمة = (__u32 *)data;
		لكل (n = (entry->e_قيمة_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << قيمة_HASH_SHIFT) ^
			       (hash >> (8*حجم(hash) - قيمة_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*قيمة++);
		}
	}

	إرجاع hash;
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
	إذا (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_entry_signed(entry, data);

	إذا (entry->e_قيمة_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, entry->e_قيمة_inum,
					    &ea_inode_hash);
		إذا (retval)
			إرجاع retval;

		*hash = (*hash << قيمة_HASH_SHIFT) ^
			(*hash >> (8*حجم(*hash) - قيمة_HASH_SHIFT)) ^
			ea_inode_hash;
		إذا (signed_hash)
			*signed_hash = (*signed_hash << قيمة_HASH_SHIFT) ^
				(*signed_hash >> (8*حجم(*hash) -
						  قيمة_HASH_SHIFT)) ^
				ea_inode_hash;
	}
	إرجاع 0;
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
	إرجاع ext2fs_ext_attr_hash_entry3(fs, entry, data, hash, عدم);
}

#undef NAME_HASH_SHIFT
#undef قيمة_HASH_SHIFT

#define BLOCK_HASH_SHIFT 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
فراغ ext2fs_ext_attr_block_rehash(بنية ext2_ext_attr_header *header,
				  بنية ext2_ext_attr_entry *end)
{
	بنية ext2_ext_attr_entry *here;
	__u32 hash = 0;

	here = (بنية ext2_ext_attr_entry *)(header+1);
	بينما (here < end && !EXT2_EXT_IS_LAST_ENTRY(here)) {
		إذا (!here->e_hash) {
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
	إرجاع inode->i_atime;
}

فراغ ext2fs_set_ea_inode_hash(بنية ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(بنية ext2_inode *inode)
{
	إرجاع ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_version;
}

فراغ ext2fs_set_ea_inode_ref(بنية ext2_inode *inode, __u64 ref_تعداد)
{
	inode->i_ctime = (__u32)(ref_تعداد >> 32);
	inode->osd1.linux1.l_i_version = (__u32)ref_تعداد;
}

ساكن errcode_t check_ext_attr_header(بنية ext2_ext_attr_header *header)
{
	إذا ((header->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     header->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    header->h_blocks != 1)
		إرجاع EXT2_ET_BAD_EA_HEADER;

	إرجاع 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block, فراغ *buf,
				ext2_ino_t inum)
{
	صحيح		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	إذا (retval)
		إرجاع retval;

	إذا (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_block_csum_verify(fs, inum, block, buf))
		csum_failed = 1;

#ifdef كلمات_BIGENDIAN
	ext2fs_swap_ext_attr(buf, buf, fs->blocksize, 1);
#endif

	retval = check_ext_attr_header(buf);
	إذا (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	إرجاع retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block, فراغ *buf)
{
	إرجاع ext2fs_read_ext_attr3(fs, block, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, فراغ *buf)
{
	إرجاع ext2fs_read_ext_attr2(fs, block, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block, فراغ *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	حرف		*write_buf;

#ifdef كلمات_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	إذا (retval)
		إرجاع retval;
	ext2fs_swap_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#وإلا
	write_buf = (حرف *) inbuf;
#endif

	retval = ext2fs_ext_attr_block_csum_set(fs, inum, block,
			(بنية ext2_ext_attr_header *)write_buf);
	إذا (retval)
		إرجاع retval;

	retval = io_channel_write_blk64(fs->io, block, 1, write_buf);
#ifdef كلمات_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#endif
	إذا (!retval)
		ext2fs_mark_changed(fs);
	إرجاع retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block, فراغ *inbuf)
{
	إرجاع ext2fs_write_ext_attr3(fs, block, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block, فراغ *inbuf)
{
	إرجاع ext2fs_write_ext_attr2(fs, block, inbuf);
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

	إذا ((blk >= ext2fs_blocks_تعداد(fs->super)) ||
	    (blk < fs->super->s_first_data_block))
		إرجاع EXT2_ET_BAD_EA_BLOCK_رقم;

	إذا (!block_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		إذا (retval)
			إرجاع retval;
		block_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, block_buf, inum);
	إذا (retval)
		اذهب errout;

	header = (بنية ext2_ext_attr_header *) block_buf;
	header->h_refcount += adjust;
	إذا (newcount)
		*newcount = header->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, block_buf, inum);
	إذا (retval)
		اذهب errout;

errout:
	إذا (buf)
		ext2fs_free_mem(&buf);
	إرجاع retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    حرف *block_buf, صحيح adjust,
				    __u32 *newcount)
{
	إرجاع ext2fs_adjust_ea_refcount3(fs, blk, block_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					حرف *block_buf, صحيح adjust,
					__u32 *newcount)
{
	إرجاع ext2fs_adjust_ea_refcount2(fs, blk, block_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
بنية ext2_xattr {
	صحيح name_index;
	حرف *name;
	حرف *short_name;
	فراغ *قيمة;
	unsigned صحيح قيمة_len;
	ext2_ino_t ea_ino;
};

بنية ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	بنية ext2_xattr *attrs;
	صحيح capacity;
	صحيح تعداد_2;
	صحيح ibody_تعداد;
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
	إذا (err)
		إرجاع err;

	memcpy(new_attrs, h->attrs, h->capacity * حجم(بنية ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->capacity += expandby;
	h->attrs = new_attrs;

	إرجاع 0;
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
		إذا (e->index == index)
			إرجاع e->name;

	إرجاع عدم;
}

ساكن صحيح find_ea_index(ثابت حرف *fullname, ثابت حرف **name, صحيح *index)
{
	بنية ea_name_index *e;

	لكل (e = ea_names; e->name; e++) {
		إذا (strncmp(fullname, e->name, strlen(e->name)) == 0) {
			*name = fullname + strlen(e->name);
			*index = e->index;
			إرجاع 1;
		}
	}
	إرجاع 0;
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
	إذا (inode == عدم) {
		err = ext2fs_read_inode_full(fs, ino, (بنية ext2_inode *)&i,
					     حجم(بنية ext2_inode_large));
		إذا (err)
			إرجاع err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (بنية ext2_inode *)inode);
	إذا (blk == 0)
		إرجاع 0;

	/* Find block, zero it, write back */
	إذا ((blk < fs->super->s_first_data_block) ||
	    (blk >= ext2fs_blocks_تعداد(fs->super))) {
		err = EXT2_ET_BAD_EA_BLOCK_رقم;
		اذهب out;
	}

	err = ext2fs_get_mem(fs->blocksize, &block_buf);
	إذا (err)
		اذهب out;

	err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
	إذا (err)
		اذهب out2;

	/* We only know how to deal with v2 EA blocks */
	header = (بنية ext2_ext_attr_header *) block_buf;
	إذا (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_HEADER;
		اذهب out2;
	}

	header->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
	إذا (err)
		اذهب out2;

	/* Erase link to block */
	ext2fs_file_acl_block_set(fs, (بنية ext2_inode *)inode, 0);
	إذا (header->h_refcount == 0)
		ext2fs_block_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_blocks(fs, (بنية ext2_inode *)inode, 1);
	إذا (err)
		اذهب out2;

	/* Write inode? */
	إذا (inode == &i) {
		err = ext2fs_write_inode_full(fs, ino, (بنية ext2_inode *)&i,
					      حجم(بنية ext2_inode_large));
		إذا (err)
			اذهب out2;
	}

out2:
	ext2fs_free_mem(&block_buf);
out:
	إرجاع err;
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
	إذا (blk != 0) {
		إذا ((blk < fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_تعداد(fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_رقم;
			اذهب out;
		}

		err = ext2fs_get_mem(fs->blocksize, &block_buf);
		إذا (err)
			اذهب out;

		err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
		إذا (err)
			اذهب out2;

		/* We only know how to deal with v2 EA blocks */
		header = (بنية ext2_ext_attr_header *) block_buf;
		إذا (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			اذهب out2;
		}

		/* Single-user block.  We're done here. */
		إذا (header->h_refcount == 1)
			اذهب out2;

		/* We need to CoW the block. */
		header->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
		إذا (err)
			اذهب out2;
	} وإلا {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_add_blocks(fs, (بنية ext2_inode *)inode,
					     1);
		إذا (err)
			اذهب out;
	}

	/* Allocate a block */
	goal = ext2fs_find_inode_goal(fs, ino, (بنية ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, عدم, &blk);
	إذا (err)
		اذهب out2;
	ext2fs_file_acl_block_set(fs, (بنية ext2_inode *)inode, blk);
out2:
	إذا (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	إرجاع err;
}


ساكن مضمن صحيح
posix_acl_xattr_تعداد(size_t size)
{
        إذا (size < حجم(posix_acl_xattr_header))
                إرجاع -1;
        size -= حجم(posix_acl_xattr_header);
        إذا (size % حجم(posix_acl_xattr_entry))
                إرجاع -1;
        إرجاع size / حجم(posix_acl_xattr_entry);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
ساكن errcode_t convert_posix_acl_to_disk_buffer(ثابت فراغ *قيمة, size_t size,
						  فراغ *out_buf, size_t *size_out)
{
	ثابت posix_acl_xattr_header *header =
		(ثابت posix_acl_xattr_header*) قيمة;
	ثابت posix_acl_xattr_entry *end, *entry =
		(ثابت posix_acl_xattr_entry *)(header+1);
	ext4_acl_header *ext_acl;
	size_t s;
	حرف *e;

	صحيح تعداد_2;

	إذا (!قيمة)
		إرجاع EINVAL;
	إذا (size < حجم(posix_acl_xattr_header))
		إرجاع ENOMEM;
	إذا (header->a_version != ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION))
		إرجاع EINVAL;

	تعداد_2 = posix_acl_xattr_تعداد(size);
	ext_acl = out_buf;
	ext_acl->a_version = ext2fs_cpu_to_le32(EXT4_ACL_VERSION);

	إذا (تعداد_2 <= 0)
		إرجاع EINVAL;

	e = (حرف *) out_buf + حجم(ext4_acl_header);
	s = حجم(ext4_acl_header);
	لكل (end = entry + تعداد_2; entry != end;entry++) {
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
				إرجاع EINVAL;
		}
	}
	*size_out = s;
	إرجاع 0;
}

ساكن errcode_t convert_disk_buffer_to_posix_acl(ثابت فراغ *قيمة, size_t size,
						  فراغ **out_buf, size_t *size_out)
{
	posix_acl_xattr_header *header;
	posix_acl_xattr_entry *entry;
	ثابت ext4_acl_header *ext_acl = (ثابت ext4_acl_header *) قيمة;
	errcode_t err;
	ثابت حرف *cp;
	حرف *out;

	إذا ((!قيمة) ||
	    (size < حجم(ext4_acl_header)) ||
	    (ext_acl->a_version != ext2fs_cpu_to_le32(EXT4_ACL_VERSION)))
		إرجاع EINVAL;

	err = ext2fs_get_mem(size * 2, &out);
	إذا (err)
		إرجاع err;

	header = (posix_acl_xattr_header *) out;
	header->a_version = ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION);
	entry = (posix_acl_xattr_entry *) (out + حجم(posix_acl_xattr_header));

	cp = (ثابت حرف *) قيمة + حجم(ext4_acl_header);
	size -= حجم(ext4_acl_header);

	بينما (size > 0) {
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
				إرجاع EINVAL;
		}
		entry++;
	}
	*out_buf = out;
	*size_out = ((حرف *) entry - out);
	إرجاع 0;
}

ساكن errcode_t
write_xattrs_to_buffer(ext2_filsys fs, بنية ext2_xattr *attrs, صحيح تعداد_2,
		       فراغ *entries_start, unsigned صحيح storage_size,
		       unsigned صحيح قيمة_offset_correction, صحيح write_hash)
{
	بنية ext2_xattr *x;
	بنية ext2_ext_attr_entry *e = entries_start;
	حرف *end = (حرف *) entries_start + storage_size;
	unsigned صحيح قيمة_size;
	errcode_t err;

	memset(entries_start, 0, storage_size);
	لكل (x = attrs; x < attrs + تعداد_2; x++) {
		قيمة_size = ((x->قيمة_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = strlen(x->short_name);
		e->e_name_index = x->name_index;

		e->e_قيمة_size = x->قيمة_len;
		e->e_قيمة_inum = x->ea_ino;

		/* Store name */
		memcpy((حرف *)e + حجم(*e), x->short_name, e->e_name_len);
		إذا (x->ea_ino) {
			e->e_قيمة_offs = 0;
		} وإلا {
			end -= قيمة_size;
			e->e_قيمة_offs = end - (حرف *) entries_start +
						قيمة_offset_correction;
			memcpy(end, x->قيمة, e->e_قيمة_size);
		}

		إذا (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : end,
							  &e->e_hash);
			إذا (err)
				إرجاع err;
		} وإلا
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_NEXT(e);
		*(__u32 *)e = 0;
	}
	إرجاع 0;
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
	إذا (i < حجم(*inode))
		i = حجم(*inode);
	err = ext2fs_get_memzero(i, &inode);
	إذا (err)
		إرجاع err;

	err = ext2fs_read_inode_full(fs, handle->ino, EXT2_INODE(inode),
				     inode_size);
	إذا (err)
		اذهب out;

	/* If extra_isize isn't set, we need to set it now */
	إذا (inode->i_extra_isize == 0 &&
	    inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		حرف *p = (حرف *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		إذا (extra == 0)
			extra = حجم(__u32);
		memset(p + EXT2_GOOD_OLD_INODE_SIZE, 0, extra);
		inode->i_extra_isize = extra;
	}
	إذا (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		اذهب out;
	}

	/* Does the inode have space for EA? */
	إذا (inode->i_extra_isize < حجم(inode->i_extra_isize) ||
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

	err = write_xattrs_to_buffer(fs, handle->attrs, handle->ibody_تعداد,
				     start, storage_size, 0, 0);
	إذا (err)
		اذهب out;
write_ea_block:
	/* Are we done? */
	إذا (handle->ibody_تعداد == handle->تعداد_2 &&
	    !ext2fs_file_acl_block(fs, EXT2_INODE(inode)))
		اذهب skip_ea_block;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &block_buf);
	إذا (err)
		اذهب out;

	storage_size = fs->blocksize - حجم(بنية ext2_ext_attr_header);
	start = block_buf + حجم(بنية ext2_ext_attr_header);

	err = write_xattrs_to_buffer(fs, handle->attrs + handle->ibody_تعداد,
				     handle->تعداد_2 - handle->ibody_تعداد, start,
				     storage_size, start - block_buf, 1);
	إذا (err)
		اذهب out2;

	/* Write a header on the EA block */
	header = (بنية ext2_ext_attr_header *) block_buf;
	header->h_magic = EXT2_EXT_ATTR_MAGIC;
	header->h_refcount = 1;
	header->h_blocks = 1;

	/* Get a new block for writing */
	err = prep_ea_block_for_write(fs, handle->ino, inode);
	إذا (err)
		اذهب out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_block(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, handle->ino);
	إذا (err)
		اذهب out2;

skip_ea_block:
	blk = ext2fs_file_acl_block(fs, (بنية ext2_inode *)inode);
	إذا (!block_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		إذا (err)
			اذهب out;
	}

	/* Write the inode */
	err = ext2fs_write_inode_full(fs, handle->ino, EXT2_INODE(inode),
				      inode_size);
	إذا (err)
		اذهب out2;

out2:
	ext2fs_free_mem(&block_buf);
out:
	ext2fs_free_mem(&inode);
	إرجاع err;
}

ساكن errcode_t read_xattrs_from_buffer(بنية ext2_xattr_handle *handle,
					 بنية ext2_inode_large *inode,
					 بنية ext2_ext_attr_entry *entries,
					 unsigned صحيح storage_size,
					 حرف *قيمة_start)
{
	بنية ext2_xattr *x;
	بنية ext2_ext_attr_entry *entry, *end;
	ثابت حرف *prefix;
	unsigned صحيح remain, prefix_len;
	errcode_t err;
	unsigned صحيح قيم_size = storage_size +
			((حرف *)entries - قيمة_start);

	/* find the end */
	end = entries;
	remain = storage_size;
	بينما (remain >= حجم(بنية ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(end)) {

		/* header eats this space */
		remain -= حجم(بنية ext2_ext_attr_entry);

		/* is attribute name valid? */
		إذا (EXT2_EXT_ATTR_SIZE(end->e_name_len) > remain)
			إرجاع EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(end->e_name_len);
		end = EXT2_EXT_ATTR_NEXT(end);
	}

	entry = entries;
	remain = storage_size;
	بينما (remain >= حجم(بنية ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(entry)) {

		/* Allocate space for more attrs? */
		إذا (handle->تعداد_2 == handle->capacity) {
			err = ext2fs_xattrs_expand(handle, 4);
			إذا (err)
				إرجاع err;
		}

		x = handle->attrs + handle->تعداد_2;

		/* header eats this space */
		remain -= حجم(بنية ext2_ext_attr_entry);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(entry->e_name_len);

		/* Extract name */
		prefix = find_ea_prefix(entry->e_name_index);
		prefix_len = (prefix ? strlen(prefix) : 0);
		err = ext2fs_get_memzero(entry->e_name_len + prefix_len + 1,
					 &x->name);
		إذا (err)
			إرجاع err;
		إذا (prefix)
			memcpy(x->name, prefix, prefix_len);
		إذا (entry->e_name_len)
			memcpy(x->name + prefix_len,
			       (حرف *)entry + حجم(*entry),
			       entry->e_name_len);
		x->short_name = x->name + prefix_len;
		x->name_index = entry->e_name_index;

		/* Check & copy value */
		إذا (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    entry->e_قيمة_inum != 0)
			إرجاع EXT2_ET_BAD_EA_BLOCK_رقم;

		إذا (entry->e_قيمة_inum == 0) {
			إذا (entry->e_قيمة_size > remain)
				إرجاع EXT2_ET_EA_BAD_قيمة_SIZE;

			إذا (entry->e_قيمة_offs + entry->e_قيمة_size > قيم_size)
				إرجاع EXT2_ET_EA_BAD_قيمة_OFFSET;

			إذا (entry->e_قيمة_size > 0 &&
			    قيمة_start + entry->e_قيمة_offs <
			    (حرف *)end + حجم(__u32))
				إرجاع EXT2_ET_EA_BAD_قيمة_OFFSET;

			remain -= entry->e_قيمة_size;

			err = ext2fs_get_mem(entry->e_قيمة_size, &x->قيمة);
			إذا (err)
				إرجاع err;
			memcpy(x->قيمة, قيمة_start + entry->e_قيمة_offs,
			       entry->e_قيمة_size);
		} وإلا {
			بنية ext2_inode *ea_inode;
			ext2_file_t ea_file;

			إذا (entry->e_قيمة_offs != 0)
				إرجاع EXT2_ET_EA_BAD_قيمة_OFFSET;

			إذا (entry->e_قيمة_size > (64 * 1024))
				إرجاع EXT2_ET_EA_BAD_قيمة_SIZE;

			err = ext2fs_get_mem(entry->e_قيمة_size, &x->قيمة);
			إذا (err)
				إرجاع err;

			err = ext2fs_file_open(handle->fs, entry->e_قيمة_inum,
					       0, &ea_file);
			إذا (err)
				إرجاع err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			إذا ((ea_inode->i_flags & EXT4_INLINE_DATA_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_تعداد == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			وإلا إذا ((__u64) ext2fs_file_get_size(ea_file) !=
				 entry->e_قيمة_size)
				err = EXT2_ET_EA_BAD_قيمة_SIZE;
			وإلا
				err = ext2fs_file_read(ea_file, x->قيمة,
						       entry->e_قيمة_size, 0);
			ext2fs_file_close(ea_file);
			إذا (err)
				إرجاع err;
		}

		x->ea_ino = entry->e_قيمة_inum;
		x->قيمة_len = entry->e_قيمة_size;

		/* e_hash may be 0 in older inode's ea */
		إذا (entry->e_hash != 0) {
			__u32 hash, signed_hash;

			فراغ *data = (entry->e_قيمة_inum != 0) ?
					0 : قيمة_start + entry->e_قيمة_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, entry,
							  data, &hash,
							  &signed_hash);
			إذا (err)
				إرجاع err;
			إذا ((entry->e_hash != hash) &&
			    (entry->e_hash != signed_hash)) {
				بنية ext2_inode child;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							entry->e_قيمة_inum,
							&child);
				إذا (err)
					إرجاع err;
				إذا (child.i_mtime != handle->ino ||
				    child.i_generation != inode->i_generation)
					إرجاع EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->تعداد_2++;
		entry = EXT2_EXT_ATTR_NEXT(entry);
	}

	إرجاع 0;
}

ساكن فراغ xattrs_free_مفاتيح(بنية ext2_xattr_handle *h)
{
	بنية ext2_xattr *a = h->attrs;
	صحيح i;

	لكل (i = 0; i < h->capacity; i++) {
		إذا (a[i].name)
			ext2fs_free_mem(&a[i].name);
		إذا (a[i].قيمة)
			ext2fs_free_mem(&a[i].قيمة);
	}
	h->تعداد_2 = 0;
	h->ibody_تعداد = 0;
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

	xattrs_free_مفاتيح(handle);

	/* Does the inode have space for EA? */
	إذا (inode->i_extra_isize < حجم(inode->i_extra_isize) ||
	    EXT2_INODE_SIZE(handle->fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  حجم(__u32))
		اذهب read_ea_block;
	إذا (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		اذهب out;
	}

	/* Look for EA in the inode */
	memcpy(&ea_inode_magic, ((حرف *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, حجم(__u32));
	إذا (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_size = EXT2_INODE_SIZE(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_SIZE - inode->i_extra_isize -
			حجم(__u32);
		start = ((حرف *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + حجم(__u32);

		err = read_xattrs_from_buffer(handle, inode,
					(بنية ext2_ext_attr_entry *) start,
					storage_size, start);
		إذا (err)
			اذهب out;

		handle->ibody_تعداد = handle->تعداد_2;
	}

read_ea_block:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_block(handle->fs, EXT2_INODE(inode));
	إذا (blk != 0) {
		إذا ((blk < handle->fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_تعداد(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_رقم;
			اذهب out;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &block_buf);
		إذا (err)
			اذهب out;

		err = ext2fs_read_ext_attr3(handle->fs, blk, block_buf,
					    handle->ino);
		إذا (err)
			اذهب out3;

		/* We only know how to deal with v2 EA blocks */
		header = (بنية ext2_ext_attr_header *) block_buf;
		إذا (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
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
	إذا (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	إرجاع err;
}

errcode_t ext2fs_xattrs_read(بنية ext2_xattr_handle *handle)
{
	بنية ext2_inode_large *inode;
	size_t inode_size = EXT2_INODE_SIZE(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	إذا (inode_size < حجم(*inode))
		inode_size = حجم(*inode);
	err = ext2fs_get_memzero(inode_size, &inode);
	إذا (err)
		إرجاع err;

	err = ext2fs_read_inode_full(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_SIZE(handle->fs->super));
	إذا (err)
		اذهب out;

	err = ext2fs_xattrs_read_inode(handle, inode);

out:
	ext2fs_free_mem(&inode);

	إرجاع err;
}

errcode_t ext2fs_xattrs_iterate(بنية ext2_xattr_handle *h,
				صحيح (*func)(حرف *name, حرف *قيمة,
					    size_t قيمة_len, فراغ *data),
				فراغ *data)
{
	بنية ext2_xattr *x;
	صحيح dirty = 0;
	صحيح ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = h->attrs; x < h->attrs + h->تعداد_2; x++) {
		ret = func(x->name, x->قيمة, x->قيمة_len, data);
		إذا (ret & XATTR_CHANGED)
			dirty = 1;
		إذا (ret & XATTR_ABORT)
			اكسر;
	}

	إذا (dirty)
		إرجاع ext2fs_xattrs_write(h);
	إرجاع 0;
}

errcode_t ext2fs_xattr_get(بنية ext2_xattr_handle *h, ثابت حرف *مفتاح,
			   فراغ **قيمة, size_t *قيمة_len)
{
	بنية ext2_xattr *x;
	حرف *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = h->attrs; x < h->attrs + h->تعداد_2; x++) {
		إذا (strcmp(x->name, مفتاح))
			استمر;

		إذا (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(مفتاح, "system.posix_acl_default") == 0) ||
		     (strcmp(مفتاح, "system.posix_acl_access") == 0))) {
			err = convert_disk_buffer_to_posix_acl(x->قيمة, x->قيمة_len,
							       قيمة, قيمة_len);
			إرجاع err;
		} وإلا {
			err = ext2fs_get_mem(x->قيمة_len, &val);
			إذا (err)
				إرجاع err;
			memcpy(val, x->قيمة, x->قيمة_len);
			*قيمة = val;
			*قيمة_len = x->قيمة_len;
			إرجاع 0;
		}
	}

	إرجاع EXT2_ET_EA_مفتاح_NOT_FOUND;
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
	إذا (i < حجم(*inode))
		i = حجم(*inode);
	err = ext2fs_get_memzero(i, &inode);
	إذا (err)
		إرجاع err;

	err = ext2fs_read_inode_full(fs, ino, (بنية ext2_inode *)inode,
				     EXT2_INODE_SIZE(fs->super));
	إذا (err)
		اذهب out;

	/* Does the inode have size for EA? */
	إذا (EXT2_INODE_SIZE(fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  حجم(__u32)) {
		err = EXT2_ET_INLINE_DATA_NO_SPACE;
		اذهب out;
	}

	minoff = EXT2_INODE_SIZE(fs->super) - حجم(*inode) - حجم(__u32);
	memcpy(&ea_inode_magic, ((حرف *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, حجم(__u32));
	إذا (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		start= ((حرف *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + حجم(__u32);
		entry = (بنية ext2_ext_attr_entry *) start;
		بينما (!EXT2_EXT_IS_LAST_ENTRY(entry)) {
			إذا (!entry->e_قيمة_inum && entry->e_قيمة_size) {
				unsigned صحيح offs = entry->e_قيمة_offs;
				إذا (offs < minoff)
					minoff = offs;
			}
			entry = EXT2_EXT_ATTR_NEXT(entry);
		}
		*size = minoff - ((حرف *)entry - (حرف *)start) - حجم(__u32);
	} وإلا {
		/* no xattr.  return a maximum size */
		*size = EXT2_EXT_ATTR_SIZE(minoff -
					   EXT2_EXT_ATTR_LEN(strlen("data")) -
					   EXT2_EXT_ATTR_ROUND - حجم(__u32));
	}

out:
	ext2fs_free_mem(&inode);
	إرجاع err;
}

ساكن errcode_t xattr_create_ea_inode(ext2_filsys fs, ثابت فراغ *قيمة,
				       size_t قيمة_len, ext2_ino_t *ea_ino)
{
	بنية ext2_inode inode;
	ext2_ino_t ino;
	ext2_file_t file;
	__u32 hash;
	errcode_t ret;

	ret = ext2fs_new_inode(fs, 0, 0, 0, &ino);
	إذا (ret)
		إرجاع ret;

	memset(&inode, 0, حجم(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	إذا (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_size = 0;
	inode.i_mode = LINUX_S_IFREG | 0600;
	inode.i_links_تعداد = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	إذا (ret)
		إرجاع ret;
	/*
	 * ref_count and hash utilize inode's i_*time fields.
	 * ext2fs_write_new_inode() call above initializes these fields with
	 * current time. That's why ref count and hash updates are done
	 * separately below.
	 */
	ext2fs_set_ea_inode_ref(&inode, 1);
	hash = ext2fs_crc32c_le(fs->csum_seed, قيمة, قيمة_len);
	ext2fs_set_ea_inode_hash(&inode, hash);

	ret = ext2fs_write_inode(fs, ino, &inode);
	إذا (ret)
		إرجاع ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	إذا (ret)
		إرجاع ret;
	ret = ext2fs_file_write(file, قيمة, قيمة_len, عدم);
	ext2fs_file_close(file);
	إذا (ret)
		إرجاع ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	إرجاع 0;
}

ساكن errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	بنية ext2_inode_large inode;
	__u64 ref_تعداد;
	errcode_t ret;

	ret = ext2fs_read_inode_full(fs, ino, (بنية ext2_inode *)&inode,
				     حجم(inode));
	إذا (ret)
		اذهب out;

	ref_تعداد = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_تعداد--;
	ext2fs_set_ea_inode_ref(EXT2_INODE(&inode), ref_تعداد);

	إذا (ref_تعداد)
		اذهب write_out;

	inode.i_links_تعداد = 0;
	ext2fs_set_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	إذا (ret)
		اذهب write_out;

	إذا (ext2fs_inode_has_valid_blocks2(fs, (بنية ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (بنية ext2_inode *)&inode, عدم,
				   0, ~0ULL);
		إذا (ret)
			اذهب out;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_out:
	ret = ext2fs_write_inode_full(fs, ino, (بنية ext2_inode *)&inode,
				      حجم(inode));
out:
	إرجاع ret;
}

ساكن errcode_t xattr_update_entry(ext2_filsys fs, بنية ext2_xattr *x,
				    ثابت حرف *name, ثابت حرف *short_name,
				    صحيح index, ثابت فراغ *قيمة,
				    size_t قيمة_len, صحيح in_inode)
{
	ext2_ino_t ea_ino = 0;
	فراغ *new_قيمة = عدم;
	حرف *new_name = عدم;
	صحيح name_len;
	errcode_t ret;

	إذا (!x->name) {
		name_len = strlen(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		إذا (ret)
			اذهب fail;
		memcpy(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(قيمة_len, &new_قيمة);
	إذا (ret)
		اذهب fail;
	memcpy(new_قيمة, قيمة, قيمة_len);

	إذا (in_inode) {
		ret = xattr_create_ea_inode(fs, قيمة, قيمة_len, &ea_ino);
		إذا (ret)
			اذهب fail;
	}

	إذا (x->ea_ino) {
		ret = xattr_inode_dec_ref(fs, x->ea_ino);
		إذا (ret)
			اذهب fail;
	}

	إذا (!x->name) {
		x->name = new_name;
		x->short_name = new_name + (short_name  - name);
	}
	x->name_index = index;

	إذا (x->قيمة)
		ext2fs_free_mem(&x->قيمة);
	x->قيمة = new_قيمة;
	x->قيمة_len = قيمة_len;
	x->ea_ino = ea_ino;
	إرجاع 0;
fail:
	إذا (new_name)
		ext2fs_free_mem(&new_name);
	إذا (new_قيمة)
		ext2fs_free_mem(&new_قيمة);
	إذا (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	إرجاع ret;
}

ساكن صحيح xattr_find_position(بنية ext2_xattr *attrs, صحيح تعداد_2,
			       ثابت حرف *shortname, صحيح name_idx)
{
	بنية ext2_xattr *x;
	صحيح i;
	صحيح shortname_len, x_shortname_len;

	shortname_len = strlen(shortname);

	لكل (i = 0, x = attrs; i < تعداد_2; i++, x++) {
		إذا (name_idx < x->name_index)
			اكسر;
		إذا (name_idx > x->name_index)
			استمر;

		x_shortname_len = strlen(x->short_name);
		إذا (shortname_len < x_shortname_len)
			اكسر;
		إذا (shortname_len > x_shortname_len)
			استمر;

		إذا (memcmp(shortname, x->short_name, shortname_len) <= 0)
			اكسر;
	}
	إرجاع i;
}

ساكن errcode_t xattr_array_update(بنية ext2_xattr_handle *h,
				    ثابت حرف *name,
				    ثابت فراغ *قيمة, size_t قيمة_len,
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
	إذا (!in_inode)
		needed += EXT2_EXT_ATTR_SIZE(قيمة_len);

	إذا (old_idx >= 0 && old_idx < h->ibody_تعداد) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		إذا (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_SIZE(
						h->attrs[old_idx].قيمة_len);
	}

	إذا (needed <= ibody_free) {
		إذا (old_idx < 0) {
			new_idx = h->ibody_تعداد;
			add_to_ibody = 1;
			اذهب add_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, قيمة,
					 قيمة_len, in_inode);
		إذا (ret)
			إرجاع ret;
		إذا (h->ibody_تعداد <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_تعداد + 1,
				h->attrs + h->ibody_تعداد,
				(old_idx - h->ibody_تعداد) * حجم(*h->attrs));
			h->attrs[h->ibody_تعداد] = tmp;
			h->ibody_تعداد++;
		}
		إرجاع 0;
	}

	إذا (h->ibody_تعداد <= old_idx) {
		block_free += EXT2_EXT_ATTR_LEN(name_len);
		إذا (!h->attrs[old_idx].ea_ino)
			block_free +=
				EXT2_EXT_ATTR_SIZE(h->attrs[old_idx].قيمة_len);
	}

	إذا (needed > block_free)
		إرجاع EXT2_ET_EA_NO_SPACE;

	إذا (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, قيمة,
					 قيمة_len, in_inode);
		إذا (ret)
			إرجاع ret;
		إذا (old_idx < h->ibody_تعداد) {
			/*
			 * Move entry from ibody to the block. Note that
			 * entries in the block are sorted.
			 */
			new_idx = xattr_find_position(h->attrs + h->ibody_تعداد,
						      h->تعداد_2 - h->ibody_تعداد,
						      shortname, name_idx);
			new_idx += h->ibody_تعداد - 1;
			tmp = h->attrs[old_idx];
			memmove(h->attrs + old_idx, h->attrs + old_idx + 1,
				(new_idx - old_idx) * حجم(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_تعداد--;
		}
		إرجاع 0;
	}

	new_idx = xattr_find_position(h->attrs + h->ibody_تعداد,
				      h->تعداد_2 - h->ibody_تعداد,
				      shortname, name_idx);
	new_idx += h->ibody_تعداد;
	add_to_ibody = 0;

add_new:
	إذا (h->تعداد_2 == h->capacity) {
		ret = ext2fs_xattrs_expand(h, 4);
		إذا (ret)
			إرجاع ret;
	}

	ret = xattr_update_entry(h->fs, &h->attrs[h->تعداد_2], name, shortname,
				 name_idx, قيمة, قيمة_len, in_inode);
	إذا (ret)
		إرجاع ret;

	tmp = h->attrs[h->تعداد_2];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->تعداد_2 - new_idx)*حجم(*h->attrs));
	h->attrs[new_idx] = tmp;
	إذا (add_to_ibody)
		h->ibody_تعداد++;
	h->تعداد_2++;
	إرجاع 0;
}

ساكن صحيح space_used(بنية ext2_xattr *attrs, صحيح تعداد_2)
{
	صحيح total = 0;
	بنية ext2_xattr *x;
	صحيح i, len;

	لكل (i = 0, x = attrs; i < تعداد_2; i++, x++) {
		len = strlen(x->short_name);
		total += EXT2_EXT_ATTR_LEN(len);
		إذا (!x->ea_ino)
			total += EXT2_EXT_ATTR_SIZE(x->قيمة_len);
	}
	إرجاع total;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#define EXT4_XATTR_MIN_LARGE_EA_SIZE(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - حجم(بنية ext2_ext_attr_header) - 4)

errcode_t ext2fs_xattr_set(بنية ext2_xattr_handle *h,
			   ثابت حرف *name,
			   ثابت فراغ *قيمة,
			   size_t قيمة_len)
{
	ext2_filsys fs = h->fs;
	ثابت صحيح inode_size = EXT2_INODE_SIZE(fs->super);
	بنية ext2_inode_large *inode = عدم;
	بنية ext2_xattr *x;
	حرف *new_قيمة;
	صحيح ibody_free, block_free;
	صحيح in_inode = 0;
	صحيح old_idx = -1;
	صحيح extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(قيمة_len, &new_قيمة);
	إذا (ret)
		إرجاع ret;
	إذا (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((strcmp(name, "system.posix_acl_default") == 0) ||
	     (strcmp(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_to_disk_buffer(قيمة, قيمة_len,
						       new_قيمة, &قيمة_len);
		إذا (ret)
			اذهب out;
	} وإلا إذا (قيمة_len)
		memcpy(new_قيمة, قيمة, قيمة_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	لكل (x = h->attrs; x < h->attrs + h->تعداد_2; x++) {
		إذا (!strcmp(x->name, name)) {
			إذا (!x->ea_ino && x->قيمة_len == قيمة_len &&
			    (!قيمة_len ||
			     !memcmp(x->قيمة, new_قيمة, قيمة_len))) {
				ret = 0;
				اذهب out;
			}
			old_idx = x - h->attrs;
			اكسر;
		}
	}

	ret = ext2fs_get_memzero(inode_size, &inode);
	إذا (ret)
		اذهب out;
	ret = ext2fs_read_inode_full(fs, h->ino,
				     (بنية ext2_inode *)inode,
				     inode_size);
	إذا (ret)
		اذهب out;
	إذا (inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		extra_isize = inode->i_extra_isize;
		إذا (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			إذا (extra_isize == 0)
				extra_isize = حجم(__u32);
		}
		ibody_free = inode_size - EXT2_GOOD_OLD_INODE_SIZE;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= حجم(__u32) * 2;
		ibody_free -= space_used(h->attrs, h->ibody_تعداد);
	} وإلا
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	إذا (strcmp(name, "system.data") == 0) {
		إذا (h->ibody_تعداد <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			اذهب out;
		}
		ret = xattr_array_update(h, name, new_قيمة, قيمة_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		إذا (ret)
			اذهب out;
		اذهب write_out;
	}

	block_free = fs->blocksize;
	block_free -= حجم(بنية ext2_ext_attr_header);
	/* Final null entry. */
	block_free -= حجم(__u32);
	block_free -= space_used(h->attrs + h->ibody_تعداد,
				 h->تعداد_2 - h->ibody_تعداد);

	إذا (ext2fs_has_feature_ea_inode(fs->super) &&
	    قيمة_len > EXT4_XATTR_MIN_LARGE_EA_SIZE(fs->blocksize))
		in_inode = 1;

	ret = xattr_array_update(h, name, new_قيمة, قيمة_len, ibody_free,
				 block_free, old_idx, in_inode);
	إذا (ret == EXT2_ET_EA_NO_SPACE && !in_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_قيمة, قيمة_len,
			ibody_free, block_free, old_idx, 1 /* in_inode */);
	إذا (ret)
		اذهب out;

write_out:
	ret = ext2fs_xattrs_write(h);
out:
	إذا (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_قيمة);
	إرجاع ret;
}

errcode_t ext2fs_xattr_remove(بنية ext2_xattr_handle *handle,
			      ثابت حرف *مفتاح)
{
	بنية ext2_xattr *x;
	بنية ext2_xattr *end = handle->attrs + handle->تعداد_2;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = handle->attrs; x < end; x++) {
		إذا (strcmp(x->name, مفتاح) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->قيمة);
			إذا (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (end - x - 1)*حجم(*x));
			memset(end - 1, 0, حجم(*end));
			إذا (x < handle->attrs + handle->ibody_تعداد)
				handle->ibody_تعداد--;
			handle->تعداد_2--;
			إرجاع ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	إرجاع 0;
}

errcode_t ext2fs_xattr_remove_all(بنية ext2_xattr_handle *handle)
{
	بنية ext2_xattr *x;
	بنية ext2_xattr *end = handle->attrs + handle->تعداد_2;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = handle->attrs; x < end; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->قيمة);
		إذا (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_تعداد = 0;
	handle->تعداد_2 = 0;
	إرجاع ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_xattr_handle **handle)
{
	بنية ext2_xattr_handle *h;
	errcode_t err;

	إذا (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_data(fs->super))
		إرجاع EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(حجم(*h), &h);
	إذا (err)
		إرجاع err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->capacity = 4;
	err = ext2fs_get_arrayzero(h->capacity, حجم(بنية ext2_xattr),
				   &h->attrs);
	إذا (err) {
		ext2fs_free_mem(&h);
		إرجاع err;
	}
	h->تعداد_2 = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	إرجاع 0;
}

errcode_t ext2fs_xattrs_close(بنية ext2_xattr_handle **handle)
{
	بنية ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_مفاتيح(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	إرجاع 0;
}

errcode_t ext2fs_xattrs_تعداد(بنية ext2_xattr_handle *handle, size_t *تعداد_2)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*تعداد_2 = handle->تعداد_2;
	إرجاع 0;
}

errcode_t ext2fs_xattrs_flags(بنية ext2_xattr_handle *handle,
			      unsigned صحيح *new_flags, unsigned صحيح *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	إذا (old_flags)
		*old_flags = handle->flags;
	إذا (new_flags)
		handle->flags = *new_flags;
	إرجاع 0;
}
