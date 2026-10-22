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

#شامل "config.h"
#شامل <stdio.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#شامل <string.h>
#شامل <time.h>

#شامل "ext2_fs.h"
#شامل "ext2_ext_attr.h"
#شامل "ext4_acl.h"

#شامل "ext2fsP.h"

ایستا errcode_t read_ea_inode_hash(ext2_filsys fs, ext2_ino_t ino, __u32 *hash)
{
	ساختار ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	اگر (retval)
		بازگشت retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	بازگشت 0;
}

#define NAME_HASH_SHIFT 5
#define مقدار_HASH_SHIFT 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_entry(ساختار ext2_ext_attr_entry *entry, تهی *data)
{
	__u32 hash = 0;
	unsigned نویسه *name = (((unsigned نویسه *) entry) +
			       اندازه(ساختار ext2_ext_attr_entry));
	صحیح n;

	برای (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*اندازه(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	اگر (entry->e_مقدار_inum == 0 && entry->e_مقدار_size != 0) {
		__u32 *مقدار = (__u32 *)data;
		برای (n = (entry->e_مقدار_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << مقدار_HASH_SHIFT) ^
			       (hash >> (8*اندازه(hash) - مقدار_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*مقدار++);
		}
	}

	بازگشت hash;
}

__u32 ext2fs_ext_attr_hash_entry_signed(ساختار ext2_ext_attr_entry *entry,
					تهی *data)
{
	__u32 hash = 0;
	signed نویسه *name = (((signed نویسه *) entry) +
			     اندازه(ساختار ext2_ext_attr_entry));
	صحیح n;

	برای (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*اندازه(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	اگر (entry->e_مقدار_inum == 0 && entry->e_مقدار_size != 0) {
		__u32 *مقدار = (__u32 *)data;
		برای (n = (entry->e_مقدار_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << مقدار_HASH_SHIFT) ^
			       (hash >> (8*اندازه(hash) - مقدار_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*مقدار++);
		}
	}

	بازگشت hash;
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
				      ساختار ext2_ext_attr_entry *entry,
				      تهی *data, __u32 *hash,
				      __u32 *signed_hash)
{
	*hash = ext2fs_ext_attr_hash_entry(entry, data);
	اگر (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_entry_signed(entry, data);

	اگر (entry->e_مقدار_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, entry->e_مقدار_inum,
					    &ea_inode_hash);
		اگر (retval)
			بازگشت retval;

		*hash = (*hash << مقدار_HASH_SHIFT) ^
			(*hash >> (8*اندازه(*hash) - مقدار_HASH_SHIFT)) ^
			ea_inode_hash;
		اگر (signed_hash)
			*signed_hash = (*signed_hash << مقدار_HASH_SHIFT) ^
				(*signed_hash >> (8*اندازه(*hash) -
						  مقدار_HASH_SHIFT)) ^
				ea_inode_hash;
	}
	بازگشت 0;
}

/*
 * ext2fs_ext_attr_hash_entry2()
 *
 * Compute the hash of an extended attribute.
 * This version of the function supports hashing entries that reference
 * external inodes (ea_inode feature).
 */
errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
				      ساختار ext2_ext_attr_entry *entry,
				      تهی *data, __u32 *hash)
{
	بازگشت ext2fs_ext_attr_hash_entry3(fs, entry, data, hash, پوچ);
}

#undef NAME_HASH_SHIFT
#undef مقدار_HASH_SHIFT

#define BLOCK_HASH_SHIFT 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
تهی ext2fs_ext_attr_block_rehash(ساختار ext2_ext_attr_header *header,
				  ساختار ext2_ext_attr_entry *end)
{
	ساختار ext2_ext_attr_entry *here;
	__u32 hash = 0;

	here = (ساختار ext2_ext_attr_entry *)(header+1);
	وقتی (here < end && !EXT2_EXT_IS_LAST_ENTRY(here)) {
		اگر (!here->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			بشکن;
		}
		hash = (hash << BLOCK_HASH_SHIFT) ^
		       (hash >> (8*اندازه(hash) - BLOCK_HASH_SHIFT)) ^
		       here->e_hash;
		here = EXT2_EXT_ATTR_NEXT(here);
	}
	header->h_hash = hash;
}

#undef BLOCK_HASH_SHIFT

__u32 ext2fs_get_ea_inode_hash(ساختار ext2_inode *inode)
{
	بازگشت inode->i_atime;
}

تهی ext2fs_set_ea_inode_hash(ساختار ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(ساختار ext2_inode *inode)
{
	بازگشت ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_version;
}

تهی ext2fs_set_ea_inode_ref(ساختار ext2_inode *inode, __u64 ref_شمار)
{
	inode->i_ctime = (__u32)(ref_شمار >> 32);
	inode->osd1.linux1.l_i_version = (__u32)ref_شمار;
}

ایستا errcode_t check_ext_attr_header(ساختار ext2_ext_attr_header *header)
{
	اگر ((header->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     header->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    header->h_blocks != 1)
		بازگشت EXT2_ET_BAD_EA_HEADER;

	بازگشت 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block, تهی *buf,
				ext2_ino_t inum)
{
	صحیح		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	اگر (retval)
		بازگشت retval;

	اگر (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_block_csum_verify(fs, inum, block, buf))
		csum_failed = 1;

#ifdef واژگان_BIGENDIAN
	ext2fs_swap_ext_attr(buf, buf, fs->blocksize, 1);
#endif

	retval = check_ext_attr_header(buf);
	اگر (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	بازگشت retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block, تهی *buf)
{
	بازگشت ext2fs_read_ext_attr3(fs, block, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, تهی *buf)
{
	بازگشت ext2fs_read_ext_attr2(fs, block, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block, تهی *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	نویسه		*write_buf;

#ifdef واژگان_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	اگر (retval)
		بازگشت retval;
	ext2fs_swap_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#وگرنه
	write_buf = (نویسه *) inbuf;
#endif

	retval = ext2fs_ext_attr_block_csum_set(fs, inum, block,
			(ساختار ext2_ext_attr_header *)write_buf);
	اگر (retval)
		بازگشت retval;

	retval = io_channel_write_blk64(fs->io, block, 1, write_buf);
#ifdef واژگان_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#endif
	اگر (!retval)
		ext2fs_mark_changed(fs);
	بازگشت retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block, تهی *inbuf)
{
	بازگشت ext2fs_write_ext_attr3(fs, block, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block, تهی *inbuf)
{
	بازگشت ext2fs_write_ext_attr2(fs, block, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    نویسه *block_buf, صحیح adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	ساختار ext2_ext_attr_header *header;
	نویسه	*buf = 0;

	اگر ((blk >= ext2fs_blocks_شمار(fs->super)) ||
	    (blk < fs->super->s_first_data_block))
		بازگشت EXT2_ET_BAD_EA_BLOCK_رقم;

	اگر (!block_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		اگر (retval)
			بازگشت retval;
		block_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, block_buf, inum);
	اگر (retval)
		برو errout;

	header = (ساختار ext2_ext_attr_header *) block_buf;
	header->h_refcount += adjust;
	اگر (newcount)
		*newcount = header->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, block_buf, inum);
	اگر (retval)
		برو errout;

errout:
	اگر (buf)
		ext2fs_free_mem(&buf);
	بازگشت retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    نویسه *block_buf, صحیح adjust,
				    __u32 *newcount)
{
	بازگشت ext2fs_adjust_ea_refcount3(fs, blk, block_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					نویسه *block_buf, صحیح adjust,
					__u32 *newcount)
{
	بازگشت ext2fs_adjust_ea_refcount2(fs, blk, block_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
ساختار ext2_xattr {
	صحیح name_index;
	نویسه *name;
	نویسه *short_name;
	تهی *مقدار;
	unsigned صحیح مقدار_len;
	ext2_ino_t ea_ino;
};

ساختار ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	ساختار ext2_xattr *attrs;
	صحیح capacity;
	صحیح شمار;
	صحیح ibody_شمار;
	ext2_ino_t ino;
	unsigned صحیح flags;
};

ایستا errcode_t ext2fs_xattrs_expand(ساختار ext2_xattr_handle *h,
				      unsigned صحیح expandby)
{
	ساختار ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->capacity + expandby,
				   اندازه(ساختار ext2_xattr), &new_attrs);
	اگر (err)
		بازگشت err;

	memcpy(new_attrs, h->attrs, h->capacity * اندازه(ساختار ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->capacity += expandby;
	h->attrs = new_attrs;

	بازگشت 0;
}

ساختار ea_name_index {
	صحیح index;
	ثابت نویسه *name;
};

/* Keep these names sorted in order of decreasing specificity. */
ایستا ساختار ea_name_index ea_names[] = {
	{10, "gnu."},
	{3, "system.posix_acl_default"},
	{2, "system.posix_acl_access"},
	{8, "system.richacl"},
	{6, "security."},
	{4, "trusted."},
	{7, "system."},
	{1, "user."},
	{0, پوچ},
};

ایستا ثابت نویسه *find_ea_prefix(صحیح index)
{
	ساختار ea_name_index *e;

	برای (e = ea_names; e->name; e++)
		اگر (e->index == index)
			بازگشت e->name;

	بازگشت پوچ;
}

ایستا صحیح find_ea_index(ثابت نویسه *fullname, ثابت نویسه **name, صحیح *index)
{
	ساختار ea_name_index *e;

	برای (e = ea_names; e->name; e++) {
		اگر (strncmp(fullname, e->name, strlen(e->name)) == 0) {
			*name = fullname + strlen(e->name);
			*index = e->index;
			بازگشت 1;
		}
	}
	بازگشت 0;
}

errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       ساختار ext2_inode_large *inode)
{
	ساختار ext2_ext_attr_header *header;
	تهی *block_buf = پوچ;
	blk64_t blk;
	errcode_t err;
	ساختار ext2_inode_large i;

	/* Read inode? */
	اگر (inode == پوچ) {
		err = ext2fs_read_inode_full(fs, ino, (ساختار ext2_inode *)&i,
					     اندازه(ساختار ext2_inode_large));
		اگر (err)
			بازگشت err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (ساختار ext2_inode *)inode);
	اگر (blk == 0)
		بازگشت 0;

	/* Find block, zero it, write back */
	اگر ((blk < fs->super->s_first_data_block) ||
	    (blk >= ext2fs_blocks_شمار(fs->super))) {
		err = EXT2_ET_BAD_EA_BLOCK_رقم;
		برو out;
	}

	err = ext2fs_get_mem(fs->blocksize, &block_buf);
	اگر (err)
		برو out;

	err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
	اگر (err)
		برو out2;

	/* We only know how to deal with v2 EA blocks */
	header = (ساختار ext2_ext_attr_header *) block_buf;
	اگر (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_HEADER;
		برو out2;
	}

	header->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
	اگر (err)
		برو out2;

	/* Erase link to block */
	ext2fs_file_acl_block_set(fs, (ساختار ext2_inode *)inode, 0);
	اگر (header->h_refcount == 0)
		ext2fs_block_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_blocks(fs, (ساختار ext2_inode *)inode, 1);
	اگر (err)
		برو out2;

	/* Write inode? */
	اگر (inode == &i) {
		err = ext2fs_write_inode_full(fs, ino, (ساختار ext2_inode *)&i,
					      اندازه(ساختار ext2_inode_large));
		اگر (err)
			برو out2;
	}

out2:
	ext2fs_free_mem(&block_buf);
out:
	بازگشت err;
}

ایستا errcode_t prep_ea_block_for_write(ext2_filsys fs, ext2_ino_t ino,
					 ساختار ext2_inode_large *inode)
{
	ساختار ext2_ext_attr_header *header;
	تهی *block_buf = پوچ;
	blk64_t blk, goal;
	errcode_t err;

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (ساختار ext2_inode *)inode);
	اگر (blk != 0) {
		اگر ((blk < fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_شمار(fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_رقم;
			برو out;
		}

		err = ext2fs_get_mem(fs->blocksize, &block_buf);
		اگر (err)
			برو out;

		err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
		اگر (err)
			برو out2;

		/* We only know how to deal with v2 EA blocks */
		header = (ساختار ext2_ext_attr_header *) block_buf;
		اگر (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			برو out2;
		}

		/* Single-user block.  We're done here. */
		اگر (header->h_refcount == 1)
			برو out2;

		/* We need to CoW the block. */
		header->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
		اگر (err)
			برو out2;
	} وگرنه {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_add_blocks(fs, (ساختار ext2_inode *)inode,
					     1);
		اگر (err)
			برو out;
	}

	/* Allocate a block */
	goal = ext2fs_find_inode_goal(fs, ino, (ساختار ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, پوچ, &blk);
	اگر (err)
		برو out2;
	ext2fs_file_acl_block_set(fs, (ساختار ext2_inode *)inode, blk);
out2:
	اگر (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	بازگشت err;
}


ایستا درونخطی صحیح
posix_acl_xattr_شمار(size_t size)
{
        اگر (size < اندازه(posix_acl_xattr_header))
                بازگشت -1;
        size -= اندازه(posix_acl_xattr_header);
        اگر (size % اندازه(posix_acl_xattr_entry))
                بازگشت -1;
        بازگشت size / اندازه(posix_acl_xattr_entry);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
ایستا errcode_t convert_posix_acl_to_disk_buffer(ثابت تهی *مقدار, size_t size,
						  تهی *out_buf, size_t *size_out)
{
	ثابت posix_acl_xattr_header *header =
		(ثابت posix_acl_xattr_header*) مقدار;
	ثابت posix_acl_xattr_entry *end, *entry =
		(ثابت posix_acl_xattr_entry *)(header+1);
	ext4_acl_header *ext_acl;
	size_t s;
	نویسه *e;

	صحیح شمار;

	اگر (!مقدار)
		بازگشت EINVAL;
	اگر (size < اندازه(posix_acl_xattr_header))
		بازگشت ENOMEM;
	اگر (header->a_version != ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION))
		بازگشت EINVAL;

	شمار = posix_acl_xattr_شمار(size);
	ext_acl = out_buf;
	ext_acl->a_version = ext2fs_cpu_to_le32(EXT4_ACL_VERSION);

	اگر (شمار <= 0)
		بازگشت EINVAL;

	e = (نویسه *) out_buf + اندازه(ext4_acl_header);
	s = اندازه(ext4_acl_header);
	برای (end = entry + شمار; entry != end;entry++) {
		ext4_acl_entry *disk_entry = (ext4_acl_entry*) e;
		disk_entry->e_tag = entry->e_tag;
		disk_entry->e_perm = entry->e_perm;

		انتخاب(ext2fs_le16_to_cpu(entry->e_tag)) {
			حالت ACL_USER_OBJ:
			حالت ACL_GROUP_OBJ:
			حالت ACL_MASK:
			حالت ACL_OTHER:
				e += اندازه(ext4_acl_entry_short);
				s += اندازه(ext4_acl_entry_short);
				بشکن;
			حالت ACL_USER:
			حالت ACL_GROUP:
				disk_entry->e_id = entry->e_id;
				e += اندازه(ext4_acl_entry);
				s += اندازه(ext4_acl_entry);
				بشکن;
			پیشفرض:
				بازگشت EINVAL;
		}
	}
	*size_out = s;
	بازگشت 0;
}

ایستا errcode_t convert_disk_buffer_to_posix_acl(ثابت تهی *مقدار, size_t size,
						  تهی **out_buf, size_t *size_out)
{
	posix_acl_xattr_header *header;
	posix_acl_xattr_entry *entry;
	ثابت ext4_acl_header *ext_acl = (ثابت ext4_acl_header *) مقدار;
	errcode_t err;
	ثابت نویسه *cp;
	نویسه *out;

	اگر ((!مقدار) ||
	    (size < اندازه(ext4_acl_header)) ||
	    (ext_acl->a_version != ext2fs_cpu_to_le32(EXT4_ACL_VERSION)))
		بازگشت EINVAL;

	err = ext2fs_get_mem(size * 2, &out);
	اگر (err)
		بازگشت err;

	header = (posix_acl_xattr_header *) out;
	header->a_version = ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION);
	entry = (posix_acl_xattr_entry *) (out + اندازه(posix_acl_xattr_header));

	cp = (ثابت نویسه *) مقدار + اندازه(ext4_acl_header);
	size -= اندازه(ext4_acl_header);

	وقتی (size > 0) {
		ثابت ext4_acl_entry *disk_entry = (ثابت ext4_acl_entry *) cp;

		entry->e_tag = disk_entry->e_tag;
		entry->e_perm = disk_entry->e_perm;

		انتخاب(ext2fs_le16_to_cpu(entry->e_tag)) {
			حالت ACL_USER_OBJ:
			حالت ACL_GROUP_OBJ:
			حالت ACL_MASK:
			حالت ACL_OTHER:
				entry->e_id = 0;
				cp += اندازه(ext4_acl_entry_short);
				size -= اندازه(ext4_acl_entry_short);
				بشکن;
			حالت ACL_USER:
			حالت ACL_GROUP:
				entry->e_id = disk_entry->e_id;
				cp += اندازه(ext4_acl_entry);
				size -= اندازه(ext4_acl_entry);
				بشکن;
			پیشفرض:
				ext2fs_free_mem(&out);
				بازگشت EINVAL;
		}
		entry++;
	}
	*out_buf = out;
	*size_out = ((نویسه *) entry - out);
	بازگشت 0;
}

ایستا errcode_t
write_xattrs_to_buffer(ext2_filsys fs, ساختار ext2_xattr *attrs, صحیح شمار,
		       تهی *entries_start, unsigned صحیح storage_size,
		       unsigned صحیح مقدار_offset_correction, صحیح write_hash)
{
	ساختار ext2_xattr *x;
	ساختار ext2_ext_attr_entry *e = entries_start;
	نویسه *end = (نویسه *) entries_start + storage_size;
	unsigned صحیح مقدار_size;
	errcode_t err;

	memset(entries_start, 0, storage_size);
	برای (x = attrs; x < attrs + شمار; x++) {
		مقدار_size = ((x->مقدار_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = strlen(x->short_name);
		e->e_name_index = x->name_index;

		e->e_مقدار_size = x->مقدار_len;
		e->e_مقدار_inum = x->ea_ino;

		/* Store name */
		memcpy((نویسه *)e + اندازه(*e), x->short_name, e->e_name_len);
		اگر (x->ea_ino) {
			e->e_مقدار_offs = 0;
		} وگرنه {
			end -= مقدار_size;
			e->e_مقدار_offs = end - (نویسه *) entries_start +
						مقدار_offset_correction;
			memcpy(end, x->مقدار, e->e_مقدار_size);
		}

		اگر (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : end,
							  &e->e_hash);
			اگر (err)
				بازگشت err;
		} وگرنه
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_NEXT(e);
		*(__u32 *)e = 0;
	}
	بازگشت 0;
}

errcode_t ext2fs_xattrs_write(ساختار ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	ثابت unsigned صحیح inode_size = EXT2_INODE_SIZE(fs->super);
	ساختار ext2_inode_large *inode;
	نویسه *start, *block_buf = پوچ;
	ساختار ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	blk64_t blk;
	unsigned صحیح storage_size;
	unsigned صحیح i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_size;
	اگر (i < اندازه(*inode))
		i = اندازه(*inode);
	err = ext2fs_get_memzero(i, &inode);
	اگر (err)
		بازگشت err;

	err = ext2fs_read_inode_full(fs, handle->ino, EXT2_INODE(inode),
				     inode_size);
	اگر (err)
		برو out;

	/* If extra_isize isn't set, we need to set it now */
	اگر (inode->i_extra_isize == 0 &&
	    inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		نویسه *p = (نویسه *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		اگر (extra == 0)
			extra = اندازه(__u32);
		memset(p + EXT2_GOOD_OLD_INODE_SIZE, 0, extra);
		inode->i_extra_isize = extra;
	}
	اگر (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		برو out;
	}

	/* Does the inode have space for EA? */
	اگر (inode->i_extra_isize < اندازه(inode->i_extra_isize) ||
	    inode_size <= EXT2_GOOD_OLD_INODE_SIZE + inode->i_extra_isize +
								اندازه(__u32))
		برو write_ea_block;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	memcpy(((نویسه *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, &ea_inode_magic, اندازه(__u32));
	storage_size = inode_size - EXT2_GOOD_OLD_INODE_SIZE -
				inode->i_extra_isize - اندازه(__u32);
	start = ((نویسه *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
				inode->i_extra_isize + اندازه(__u32);

	err = write_xattrs_to_buffer(fs, handle->attrs, handle->ibody_شمار,
				     start, storage_size, 0, 0);
	اگر (err)
		برو out;
write_ea_block:
	/* Are we done? */
	اگر (handle->ibody_شمار == handle->شمار &&
	    !ext2fs_file_acl_block(fs, EXT2_INODE(inode)))
		برو skip_ea_block;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &block_buf);
	اگر (err)
		برو out;

	storage_size = fs->blocksize - اندازه(ساختار ext2_ext_attr_header);
	start = block_buf + اندازه(ساختار ext2_ext_attr_header);

	err = write_xattrs_to_buffer(fs, handle->attrs + handle->ibody_شمار,
				     handle->شمار - handle->ibody_شمار, start,
				     storage_size, start - block_buf, 1);
	اگر (err)
		برو out2;

	/* Write a header on the EA block */
	header = (ساختار ext2_ext_attr_header *) block_buf;
	header->h_magic = EXT2_EXT_ATTR_MAGIC;
	header->h_refcount = 1;
	header->h_blocks = 1;

	/* Get a new block for writing */
	err = prep_ea_block_for_write(fs, handle->ino, inode);
	اگر (err)
		برو out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_block(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, handle->ino);
	اگر (err)
		برو out2;

skip_ea_block:
	blk = ext2fs_file_acl_block(fs, (ساختار ext2_inode *)inode);
	اگر (!block_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		اگر (err)
			برو out;
	}

	/* Write the inode */
	err = ext2fs_write_inode_full(fs, handle->ino, EXT2_INODE(inode),
				      inode_size);
	اگر (err)
		برو out2;

out2:
	ext2fs_free_mem(&block_buf);
out:
	ext2fs_free_mem(&inode);
	بازگشت err;
}

ایستا errcode_t read_xattrs_from_buffer(ساختار ext2_xattr_handle *handle,
					 ساختار ext2_inode_large *inode,
					 ساختار ext2_ext_attr_entry *entries,
					 unsigned صحیح storage_size,
					 نویسه *مقدار_start)
{
	ساختار ext2_xattr *x;
	ساختار ext2_ext_attr_entry *entry, *end;
	ثابت نویسه *prefix;
	unsigned صحیح remain, prefix_len;
	errcode_t err;
	unsigned صحیح مقادیر_size = storage_size +
			((نویسه *)entries - مقدار_start);

	/* find the end */
	end = entries;
	remain = storage_size;
	وقتی (remain >= اندازه(ساختار ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(end)) {

		/* header eats this space */
		remain -= اندازه(ساختار ext2_ext_attr_entry);

		/* is attribute name valid? */
		اگر (EXT2_EXT_ATTR_SIZE(end->e_name_len) > remain)
			بازگشت EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(end->e_name_len);
		end = EXT2_EXT_ATTR_NEXT(end);
	}

	entry = entries;
	remain = storage_size;
	وقتی (remain >= اندازه(ساختار ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(entry)) {

		/* Allocate space for more attrs? */
		اگر (handle->شمار == handle->capacity) {
			err = ext2fs_xattrs_expand(handle, 4);
			اگر (err)
				بازگشت err;
		}

		x = handle->attrs + handle->شمار;

		/* header eats this space */
		remain -= اندازه(ساختار ext2_ext_attr_entry);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(entry->e_name_len);

		/* Extract name */
		prefix = find_ea_prefix(entry->e_name_index);
		prefix_len = (prefix ? strlen(prefix) : 0);
		err = ext2fs_get_memzero(entry->e_name_len + prefix_len + 1,
					 &x->name);
		اگر (err)
			بازگشت err;
		اگر (prefix)
			memcpy(x->name, prefix, prefix_len);
		اگر (entry->e_name_len)
			memcpy(x->name + prefix_len,
			       (نویسه *)entry + اندازه(*entry),
			       entry->e_name_len);
		x->short_name = x->name + prefix_len;
		x->name_index = entry->e_name_index;

		/* Check & copy value */
		اگر (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    entry->e_مقدار_inum != 0)
			بازگشت EXT2_ET_BAD_EA_BLOCK_رقم;

		اگر (entry->e_مقدار_inum == 0) {
			اگر (entry->e_مقدار_size > remain)
				بازگشت EXT2_ET_EA_BAD_مقدار_SIZE;

			اگر (entry->e_مقدار_offs + entry->e_مقدار_size > مقادیر_size)
				بازگشت EXT2_ET_EA_BAD_مقدار_OFFSET;

			اگر (entry->e_مقدار_size > 0 &&
			    مقدار_start + entry->e_مقدار_offs <
			    (نویسه *)end + اندازه(__u32))
				بازگشت EXT2_ET_EA_BAD_مقدار_OFFSET;

			remain -= entry->e_مقدار_size;

			err = ext2fs_get_mem(entry->e_مقدار_size, &x->مقدار);
			اگر (err)
				بازگشت err;
			memcpy(x->مقدار, مقدار_start + entry->e_مقدار_offs,
			       entry->e_مقدار_size);
		} وگرنه {
			ساختار ext2_inode *ea_inode;
			ext2_file_t ea_file;

			اگر (entry->e_مقدار_offs != 0)
				بازگشت EXT2_ET_EA_BAD_مقدار_OFFSET;

			اگر (entry->e_مقدار_size > (64 * 1024))
				بازگشت EXT2_ET_EA_BAD_مقدار_SIZE;

			err = ext2fs_get_mem(entry->e_مقدار_size, &x->مقدار);
			اگر (err)
				بازگشت err;

			err = ext2fs_file_open(handle->fs, entry->e_مقدار_inum,
					       0, &ea_file);
			اگر (err)
				بازگشت err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			اگر ((ea_inode->i_flags & EXT4_INLINE_DATA_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_شمار == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			وگرنه اگر ((__u64) ext2fs_file_get_size(ea_file) !=
				 entry->e_مقدار_size)
				err = EXT2_ET_EA_BAD_مقدار_SIZE;
			وگرنه
				err = ext2fs_file_read(ea_file, x->مقدار,
						       entry->e_مقدار_size, 0);
			ext2fs_file_close(ea_file);
			اگر (err)
				بازگشت err;
		}

		x->ea_ino = entry->e_مقدار_inum;
		x->مقدار_len = entry->e_مقدار_size;

		/* e_hash may be 0 in older inode's ea */
		اگر (entry->e_hash != 0) {
			__u32 hash, signed_hash;

			تهی *data = (entry->e_مقدار_inum != 0) ?
					0 : مقدار_start + entry->e_مقدار_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, entry,
							  data, &hash,
							  &signed_hash);
			اگر (err)
				بازگشت err;
			اگر ((entry->e_hash != hash) &&
			    (entry->e_hash != signed_hash)) {
				ساختار ext2_inode child;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							entry->e_مقدار_inum,
							&child);
				اگر (err)
					بازگشت err;
				اگر (child.i_mtime != handle->ino ||
				    child.i_generation != inode->i_generation)
					بازگشت EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->شمار++;
		entry = EXT2_EXT_ATTR_NEXT(entry);
	}

	بازگشت 0;
}

ایستا تهی xattrs_free_کلیدها(ساختار ext2_xattr_handle *h)
{
	ساختار ext2_xattr *a = h->attrs;
	صحیح i;

	برای (i = 0; i < h->capacity; i++) {
		اگر (a[i].name)
			ext2fs_free_mem(&a[i].name);
		اگر (a[i].مقدار)
			ext2fs_free_mem(&a[i].مقدار);
	}
	h->شمار = 0;
	h->ibody_شمار = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(ساختار ext2_xattr_handle *handle,
				   ساختار ext2_inode_large *inode)
{
	ساختار ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	unsigned صحیح storage_size;
	نویسه *start, *block_buf = پوچ;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_کلیدها(handle);

	/* Does the inode have space for EA? */
	اگر (inode->i_extra_isize < اندازه(inode->i_extra_isize) ||
	    EXT2_INODE_SIZE(handle->fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  اندازه(__u32))
		برو read_ea_block;
	اگر (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		برو out;
	}

	/* Look for EA in the inode */
	memcpy(&ea_inode_magic, ((نویسه *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, اندازه(__u32));
	اگر (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_size = EXT2_INODE_SIZE(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_SIZE - inode->i_extra_isize -
			اندازه(__u32);
		start = ((نویسه *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + اندازه(__u32);

		err = read_xattrs_from_buffer(handle, inode,
					(ساختار ext2_ext_attr_entry *) start,
					storage_size, start);
		اگر (err)
			برو out;

		handle->ibody_شمار = handle->شمار;
	}

read_ea_block:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_block(handle->fs, EXT2_INODE(inode));
	اگر (blk != 0) {
		اگر ((blk < handle->fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_شمار(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_رقم;
			برو out;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &block_buf);
		اگر (err)
			برو out;

		err = ext2fs_read_ext_attr3(handle->fs, blk, block_buf,
					    handle->ino);
		اگر (err)
			برو out3;

		/* We only know how to deal with v2 EA blocks */
		header = (ساختار ext2_ext_attr_header *) block_buf;
		اگر (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			برو out3;
		}

		/* Read EAs */
		storage_size = handle->fs->blocksize -
			اندازه(ساختار ext2_ext_attr_header);
		start = block_buf + اندازه(ساختار ext2_ext_attr_header);
		err = read_xattrs_from_buffer(handle, inode,
					(ساختار ext2_ext_attr_entry *) start,
					storage_size, block_buf);
	}

out3:
	اگر (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	بازگشت err;
}

errcode_t ext2fs_xattrs_read(ساختار ext2_xattr_handle *handle)
{
	ساختار ext2_inode_large *inode;
	size_t inode_size = EXT2_INODE_SIZE(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	اگر (inode_size < اندازه(*inode))
		inode_size = اندازه(*inode);
	err = ext2fs_get_memzero(inode_size, &inode);
	اگر (err)
		بازگشت err;

	err = ext2fs_read_inode_full(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_SIZE(handle->fs->super));
	اگر (err)
		برو out;

	err = ext2fs_xattrs_read_inode(handle, inode);

out:
	ext2fs_free_mem(&inode);

	بازگشت err;
}

errcode_t ext2fs_xattrs_iterate(ساختار ext2_xattr_handle *h,
				صحیح (*func)(نویسه *name, نویسه *مقدار,
					    size_t مقدار_len, تهی *data),
				تهی *data)
{
	ساختار ext2_xattr *x;
	صحیح dirty = 0;
	صحیح ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	برای (x = h->attrs; x < h->attrs + h->شمار; x++) {
		ret = func(x->name, x->مقدار, x->مقدار_len, data);
		اگر (ret & XATTR_CHANGED)
			dirty = 1;
		اگر (ret & XATTR_ABORT)
			بشکن;
	}

	اگر (dirty)
		بازگشت ext2fs_xattrs_write(h);
	بازگشت 0;
}

errcode_t ext2fs_xattr_get(ساختار ext2_xattr_handle *h, ثابت نویسه *کلید,
			   تهی **مقدار, size_t *مقدار_len)
{
	ساختار ext2_xattr *x;
	نویسه *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	برای (x = h->attrs; x < h->attrs + h->شمار; x++) {
		اگر (strcmp(x->name, کلید))
			ادامه;

		اگر (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(کلید, "system.posix_acl_default") == 0) ||
		     (strcmp(کلید, "system.posix_acl_access") == 0))) {
			err = convert_disk_buffer_to_posix_acl(x->مقدار, x->مقدار_len,
							       مقدار, مقدار_len);
			بازگشت err;
		} وگرنه {
			err = ext2fs_get_mem(x->مقدار_len, &val);
			اگر (err)
				بازگشت err;
			memcpy(val, x->مقدار, x->مقدار_len);
			*مقدار = val;
			*مقدار_len = x->مقدار_len;
			بازگشت 0;
		}
	}

	بازگشت EXT2_ET_EA_کلید_NOT_FOUND;
}

errcode_t ext2fs_xattr_inode_max_size(ext2_filsys fs, ext2_ino_t ino,
				      size_t *size)
{
	ساختار ext2_ext_attr_entry *entry;
	ساختار ext2_inode_large *inode;
	__u32 ea_inode_magic;
	unsigned صحیح minoff;
	نویسه *start;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_SIZE(fs->super);
	اگر (i < اندازه(*inode))
		i = اندازه(*inode);
	err = ext2fs_get_memzero(i, &inode);
	اگر (err)
		بازگشت err;

	err = ext2fs_read_inode_full(fs, ino, (ساختار ext2_inode *)inode,
				     EXT2_INODE_SIZE(fs->super));
	اگر (err)
		برو out;

	/* Does the inode have size for EA? */
	اگر (EXT2_INODE_SIZE(fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  اندازه(__u32)) {
		err = EXT2_ET_INLINE_DATA_NO_SPACE;
		برو out;
	}

	minoff = EXT2_INODE_SIZE(fs->super) - اندازه(*inode) - اندازه(__u32);
	memcpy(&ea_inode_magic, ((نویسه *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, اندازه(__u32));
	اگر (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		start= ((نویسه *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + اندازه(__u32);
		entry = (ساختار ext2_ext_attr_entry *) start;
		وقتی (!EXT2_EXT_IS_LAST_ENTRY(entry)) {
			اگر (!entry->e_مقدار_inum && entry->e_مقدار_size) {
				unsigned صحیح offs = entry->e_مقدار_offs;
				اگر (offs < minoff)
					minoff = offs;
			}
			entry = EXT2_EXT_ATTR_NEXT(entry);
		}
		*size = minoff - ((نویسه *)entry - (نویسه *)start) - اندازه(__u32);
	} وگرنه {
		/* no xattr.  return a maximum size */
		*size = EXT2_EXT_ATTR_SIZE(minoff -
					   EXT2_EXT_ATTR_LEN(strlen("data")) -
					   EXT2_EXT_ATTR_ROUND - اندازه(__u32));
	}

out:
	ext2fs_free_mem(&inode);
	بازگشت err;
}

ایستا errcode_t xattr_create_ea_inode(ext2_filsys fs, ثابت تهی *مقدار,
				       size_t مقدار_len, ext2_ino_t *ea_ino)
{
	ساختار ext2_inode inode;
	ext2_ino_t ino;
	ext2_file_t file;
	__u32 hash;
	errcode_t ret;

	ret = ext2fs_new_inode(fs, 0, 0, 0, &ino);
	اگر (ret)
		بازگشت ret;

	memset(&inode, 0, اندازه(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	اگر (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_size = 0;
	inode.i_mode = LINUX_S_IFREG | 0600;
	inode.i_links_شمار = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	اگر (ret)
		بازگشت ret;
	/*
	 * ref_count and hash utilize inode's i_*time fields.
	 * ext2fs_write_new_inode() call above initializes these fields with
	 * current time. That's why ref count and hash updates are done
	 * separately below.
	 */
	ext2fs_set_ea_inode_ref(&inode, 1);
	hash = ext2fs_crc32c_le(fs->csum_seed, مقدار, مقدار_len);
	ext2fs_set_ea_inode_hash(&inode, hash);

	ret = ext2fs_write_inode(fs, ino, &inode);
	اگر (ret)
		بازگشت ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	اگر (ret)
		بازگشت ret;
	ret = ext2fs_file_write(file, مقدار, مقدار_len, پوچ);
	ext2fs_file_close(file);
	اگر (ret)
		بازگشت ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	بازگشت 0;
}

ایستا errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	ساختار ext2_inode_large inode;
	__u64 ref_شمار;
	errcode_t ret;

	ret = ext2fs_read_inode_full(fs, ino, (ساختار ext2_inode *)&inode,
				     اندازه(inode));
	اگر (ret)
		برو out;

	ref_شمار = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_شمار--;
	ext2fs_set_ea_inode_ref(EXT2_INODE(&inode), ref_شمار);

	اگر (ref_شمار)
		برو write_out;

	inode.i_links_شمار = 0;
	ext2fs_set_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	اگر (ret)
		برو write_out;

	اگر (ext2fs_inode_has_valid_blocks2(fs, (ساختار ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (ساختار ext2_inode *)&inode, پوچ,
				   0, ~0ULL);
		اگر (ret)
			برو out;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_out:
	ret = ext2fs_write_inode_full(fs, ino, (ساختار ext2_inode *)&inode,
				      اندازه(inode));
out:
	بازگشت ret;
}

ایستا errcode_t xattr_update_entry(ext2_filsys fs, ساختار ext2_xattr *x,
				    ثابت نویسه *name, ثابت نویسه *short_name,
				    صحیح index, ثابت تهی *مقدار,
				    size_t مقدار_len, صحیح in_inode)
{
	ext2_ino_t ea_ino = 0;
	تهی *new_مقدار = پوچ;
	نویسه *new_name = پوچ;
	صحیح name_len;
	errcode_t ret;

	اگر (!x->name) {
		name_len = strlen(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		اگر (ret)
			برو fail;
		memcpy(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(مقدار_len, &new_مقدار);
	اگر (ret)
		برو fail;
	memcpy(new_مقدار, مقدار, مقدار_len);

	اگر (in_inode) {
		ret = xattr_create_ea_inode(fs, مقدار, مقدار_len, &ea_ino);
		اگر (ret)
			برو fail;
	}

	اگر (x->ea_ino) {
		ret = xattr_inode_dec_ref(fs, x->ea_ino);
		اگر (ret)
			برو fail;
	}

	اگر (!x->name) {
		x->name = new_name;
		x->short_name = new_name + (short_name  - name);
	}
	x->name_index = index;

	اگر (x->مقدار)
		ext2fs_free_mem(&x->مقدار);
	x->مقدار = new_مقدار;
	x->مقدار_len = مقدار_len;
	x->ea_ino = ea_ino;
	بازگشت 0;
fail:
	اگر (new_name)
		ext2fs_free_mem(&new_name);
	اگر (new_مقدار)
		ext2fs_free_mem(&new_مقدار);
	اگر (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	بازگشت ret;
}

ایستا صحیح xattr_find_position(ساختار ext2_xattr *attrs, صحیح شمار,
			       ثابت نویسه *shortname, صحیح name_idx)
{
	ساختار ext2_xattr *x;
	صحیح i;
	صحیح shortname_len, x_shortname_len;

	shortname_len = strlen(shortname);

	برای (i = 0, x = attrs; i < شمار; i++, x++) {
		اگر (name_idx < x->name_index)
			بشکن;
		اگر (name_idx > x->name_index)
			ادامه;

		x_shortname_len = strlen(x->short_name);
		اگر (shortname_len < x_shortname_len)
			بشکن;
		اگر (shortname_len > x_shortname_len)
			ادامه;

		اگر (memcmp(shortname, x->short_name, shortname_len) <= 0)
			بشکن;
	}
	بازگشت i;
}

ایستا errcode_t xattr_array_update(ساختار ext2_xattr_handle *h,
				    ثابت نویسه *name,
				    ثابت تهی *مقدار, size_t مقدار_len,
				    صحیح ibody_free, صحیح block_free,
				    صحیح old_idx, صحیح in_inode)
{
	ساختار ext2_xattr tmp;
	صحیح add_to_ibody;
	صحیح needed;
	صحیح name_len, name_idx = 0;
	ثابت نویسه *shortname = name;
	صحیح new_idx;
	صحیح ret;

	find_ea_index(name, &shortname, &name_idx);
	name_len = strlen(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	اگر (!in_inode)
		needed += EXT2_EXT_ATTR_SIZE(مقدار_len);

	اگر (old_idx >= 0 && old_idx < h->ibody_شمار) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		اگر (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_SIZE(
						h->attrs[old_idx].مقدار_len);
	}

	اگر (needed <= ibody_free) {
		اگر (old_idx < 0) {
			new_idx = h->ibody_شمار;
			add_to_ibody = 1;
			برو add_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, مقدار,
					 مقدار_len, in_inode);
		اگر (ret)
			بازگشت ret;
		اگر (h->ibody_شمار <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_شمار + 1,
				h->attrs + h->ibody_شمار,
				(old_idx - h->ibody_شمار) * اندازه(*h->attrs));
			h->attrs[h->ibody_شمار] = tmp;
			h->ibody_شمار++;
		}
		بازگشت 0;
	}

	اگر (h->ibody_شمار <= old_idx) {
		block_free += EXT2_EXT_ATTR_LEN(name_len);
		اگر (!h->attrs[old_idx].ea_ino)
			block_free +=
				EXT2_EXT_ATTR_SIZE(h->attrs[old_idx].مقدار_len);
	}

	اگر (needed > block_free)
		بازگشت EXT2_ET_EA_NO_SPACE;

	اگر (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, مقدار,
					 مقدار_len, in_inode);
		اگر (ret)
			بازگشت ret;
		اگر (old_idx < h->ibody_شمار) {
			/*
			 * Move entry from ibody to the block. Note that
			 * entries in the block are sorted.
			 */
			new_idx = xattr_find_position(h->attrs + h->ibody_شمار,
						      h->شمار - h->ibody_شمار,
						      shortname, name_idx);
			new_idx += h->ibody_شمار - 1;
			tmp = h->attrs[old_idx];
			memmove(h->attrs + old_idx, h->attrs + old_idx + 1,
				(new_idx - old_idx) * اندازه(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_شمار--;
		}
		بازگشت 0;
	}

	new_idx = xattr_find_position(h->attrs + h->ibody_شمار,
				      h->شمار - h->ibody_شمار,
				      shortname, name_idx);
	new_idx += h->ibody_شمار;
	add_to_ibody = 0;

add_new:
	اگر (h->شمار == h->capacity) {
		ret = ext2fs_xattrs_expand(h, 4);
		اگر (ret)
			بازگشت ret;
	}

	ret = xattr_update_entry(h->fs, &h->attrs[h->شمار], name, shortname,
				 name_idx, مقدار, مقدار_len, in_inode);
	اگر (ret)
		بازگشت ret;

	tmp = h->attrs[h->شمار];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->شمار - new_idx)*اندازه(*h->attrs));
	h->attrs[new_idx] = tmp;
	اگر (add_to_ibody)
		h->ibody_شمار++;
	h->شمار++;
	بازگشت 0;
}

ایستا صحیح space_used(ساختار ext2_xattr *attrs, صحیح شمار)
{
	صحیح total = 0;
	ساختار ext2_xattr *x;
	صحیح i, len;

	برای (i = 0, x = attrs; i < شمار; i++, x++) {
		len = strlen(x->short_name);
		total += EXT2_EXT_ATTR_LEN(len);
		اگر (!x->ea_ino)
			total += EXT2_EXT_ATTR_SIZE(x->مقدار_len);
	}
	بازگشت total;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#define EXT4_XATTR_MIN_LARGE_EA_SIZE(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - اندازه(ساختار ext2_ext_attr_header) - 4)

errcode_t ext2fs_xattr_set(ساختار ext2_xattr_handle *h,
			   ثابت نویسه *name,
			   ثابت تهی *مقدار,
			   size_t مقدار_len)
{
	ext2_filsys fs = h->fs;
	ثابت صحیح inode_size = EXT2_INODE_SIZE(fs->super);
	ساختار ext2_inode_large *inode = پوچ;
	ساختار ext2_xattr *x;
	نویسه *new_مقدار;
	صحیح ibody_free, block_free;
	صحیح in_inode = 0;
	صحیح old_idx = -1;
	صحیح extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(مقدار_len, &new_مقدار);
	اگر (ret)
		بازگشت ret;
	اگر (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((strcmp(name, "system.posix_acl_default") == 0) ||
	     (strcmp(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_to_disk_buffer(مقدار, مقدار_len,
						       new_مقدار, &مقدار_len);
		اگر (ret)
			برو out;
	} وگرنه اگر (مقدار_len)
		memcpy(new_مقدار, مقدار, مقدار_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	برای (x = h->attrs; x < h->attrs + h->شمار; x++) {
		اگر (!strcmp(x->name, name)) {
			اگر (!x->ea_ino && x->مقدار_len == مقدار_len &&
			    (!مقدار_len ||
			     !memcmp(x->مقدار, new_مقدار, مقدار_len))) {
				ret = 0;
				برو out;
			}
			old_idx = x - h->attrs;
			بشکن;
		}
	}

	ret = ext2fs_get_memzero(inode_size, &inode);
	اگر (ret)
		برو out;
	ret = ext2fs_read_inode_full(fs, h->ino,
				     (ساختار ext2_inode *)inode,
				     inode_size);
	اگر (ret)
		برو out;
	اگر (inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		extra_isize = inode->i_extra_isize;
		اگر (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			اگر (extra_isize == 0)
				extra_isize = اندازه(__u32);
		}
		ibody_free = inode_size - EXT2_GOOD_OLD_INODE_SIZE;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= اندازه(__u32) * 2;
		ibody_free -= space_used(h->attrs, h->ibody_شمار);
	} وگرنه
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	اگر (strcmp(name, "system.data") == 0) {
		اگر (h->ibody_شمار <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			برو out;
		}
		ret = xattr_array_update(h, name, new_مقدار, مقدار_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		اگر (ret)
			برو out;
		برو write_out;
	}

	block_free = fs->blocksize;
	block_free -= اندازه(ساختار ext2_ext_attr_header);
	/* Final null entry. */
	block_free -= اندازه(__u32);
	block_free -= space_used(h->attrs + h->ibody_شمار,
				 h->شمار - h->ibody_شمار);

	اگر (ext2fs_has_feature_ea_inode(fs->super) &&
	    مقدار_len > EXT4_XATTR_MIN_LARGE_EA_SIZE(fs->blocksize))
		in_inode = 1;

	ret = xattr_array_update(h, name, new_مقدار, مقدار_len, ibody_free,
				 block_free, old_idx, in_inode);
	اگر (ret == EXT2_ET_EA_NO_SPACE && !in_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_مقدار, مقدار_len,
			ibody_free, block_free, old_idx, 1 /* in_inode */);
	اگر (ret)
		برو out;

write_out:
	ret = ext2fs_xattrs_write(h);
out:
	اگر (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_مقدار);
	بازگشت ret;
}

errcode_t ext2fs_xattr_remove(ساختار ext2_xattr_handle *handle,
			      ثابت نویسه *کلید)
{
	ساختار ext2_xattr *x;
	ساختار ext2_xattr *end = handle->attrs + handle->شمار;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	برای (x = handle->attrs; x < end; x++) {
		اگر (strcmp(x->name, کلید) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->مقدار);
			اگر (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (end - x - 1)*اندازه(*x));
			memset(end - 1, 0, اندازه(*end));
			اگر (x < handle->attrs + handle->ibody_شمار)
				handle->ibody_شمار--;
			handle->شمار--;
			بازگشت ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	بازگشت 0;
}

errcode_t ext2fs_xattr_remove_all(ساختار ext2_xattr_handle *handle)
{
	ساختار ext2_xattr *x;
	ساختار ext2_xattr *end = handle->attrs + handle->شمار;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	برای (x = handle->attrs; x < end; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->مقدار);
		اگر (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_شمار = 0;
	handle->شمار = 0;
	بازگشت ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     ساختار ext2_xattr_handle **handle)
{
	ساختار ext2_xattr_handle *h;
	errcode_t err;

	اگر (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_data(fs->super))
		بازگشت EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(اندازه(*h), &h);
	اگر (err)
		بازگشت err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->capacity = 4;
	err = ext2fs_get_arrayzero(h->capacity, اندازه(ساختار ext2_xattr),
				   &h->attrs);
	اگر (err) {
		ext2fs_free_mem(&h);
		بازگشت err;
	}
	h->شمار = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	بازگشت 0;
}

errcode_t ext2fs_xattrs_close(ساختار ext2_xattr_handle **handle)
{
	ساختار ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_کلیدها(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	بازگشت 0;
}

errcode_t ext2fs_xattrs_شمار(ساختار ext2_xattr_handle *handle, size_t *شمار)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*شمار = handle->شمار;
	بازگشت 0;
}

errcode_t ext2fs_xattrs_flags(ساختار ext2_xattr_handle *handle,
			      unsigned صحیح *new_flags, unsigned صحیح *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	اگر (old_flags)
		*old_flags = handle->flags;
	اگر (new_flags)
		handle->flags = *new_flags;
	بازگشت 0;
}
