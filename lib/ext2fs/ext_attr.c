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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <time.h>

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2_ext_attr.h"
#অন্তর্ভাব "ext4_acl.h"

#অন্তর্ভাব "ext2fsP.h"

স্থির errcode_t read_ea_inode_hash(ext2_filsys fs, ext2_ino_t ino, __u32 *hash)
{
	কাঠামো ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	যদি (retval)
		ফেরত retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	ফেরত 0;
}

#define NAME_HASH_SHIFT 5
#define মান_HASH_SHIFT 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_entry(কাঠামো ext2_ext_attr_entry *entry, শূন্য *তথ্য)
{
	__u32 hash = 0;
	unsigned অক্ষর *name = (((unsigned অক্ষর *) entry) +
			       আকার(কাঠামো ext2_ext_attr_entry));
	পূর্ণসংখ্যা n;

	জন্য (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*আকার(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	যদি (entry->e_মান_inum == 0 && entry->e_মান_আকার != 0) {
		__u32 *মান = (__u32 *)তথ্য;
		জন্য (n = (entry->e_মান_আকার + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << মান_HASH_SHIFT) ^
			       (hash >> (8*আকার(hash) - মান_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*মান++);
		}
	}

	ফেরত hash;
}

__u32 ext2fs_ext_attr_hash_entry_signed(কাঠামো ext2_ext_attr_entry *entry,
					শূন্য *তথ্য)
{
	__u32 hash = 0;
	signed অক্ষর *name = (((signed অক্ষর *) entry) +
			     আকার(কাঠামো ext2_ext_attr_entry));
	পূর্ণসংখ্যা n;

	জন্য (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*আকার(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	যদি (entry->e_মান_inum == 0 && entry->e_মান_আকার != 0) {
		__u32 *মান = (__u32 *)তথ্য;
		জন্য (n = (entry->e_মান_আকার + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << মান_HASH_SHIFT) ^
			       (hash >> (8*আকার(hash) - মান_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*মান++);
		}
	}

	ফেরত hash;
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
				      কাঠামো ext2_ext_attr_entry *entry,
				      শূন্য *তথ্য, __u32 *hash,
				      __u32 *signed_hash)
{
	*hash = ext2fs_ext_attr_hash_entry(entry, তথ্য);
	যদি (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_entry_signed(entry, তথ্য);

	যদি (entry->e_মান_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, entry->e_মান_inum,
					    &ea_inode_hash);
		যদি (retval)
			ফেরত retval;

		*hash = (*hash << মান_HASH_SHIFT) ^
			(*hash >> (8*আকার(*hash) - মান_HASH_SHIFT)) ^
			ea_inode_hash;
		যদি (signed_hash)
			*signed_hash = (*signed_hash << মান_HASH_SHIFT) ^
				(*signed_hash >> (8*আকার(*hash) -
						  মান_HASH_SHIFT)) ^
				ea_inode_hash;
	}
	ফেরত 0;
}

/*
 * ext2fs_ext_attr_hash_entry2()
 *
 * Compute the hash of an extended attribute.
 * This version of the function supports hashing entries that reference
 * external inodes (ea_inode feature).
 */
errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
				      কাঠামো ext2_ext_attr_entry *entry,
				      শূন্য *তথ্য, __u32 *hash)
{
	ফেরত ext2fs_ext_attr_hash_entry3(fs, entry, তথ্য, hash, ফাঁকা);
}

#undef NAME_HASH_SHIFT
#undef মান_HASH_SHIFT

#define BLOCK_HASH_SHIFT 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
শূন্য ext2fs_ext_attr_block_rehash(কাঠামো ext2_ext_attr_header *header,
				  কাঠামো ext2_ext_attr_entry *end)
{
	কাঠামো ext2_ext_attr_entry *here;
	__u32 hash = 0;

	here = (কাঠামো ext2_ext_attr_entry *)(header+1);
	যখন (here < end && !EXT2_EXT_IS_LAST_ENTRY(here)) {
		যদি (!here->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			বিরতি;
		}
		hash = (hash << BLOCK_HASH_SHIFT) ^
		       (hash >> (8*আকার(hash) - BLOCK_HASH_SHIFT)) ^
		       here->e_hash;
		here = EXT2_EXT_ATTR_পরবর্তী(here);
	}
	header->h_hash = hash;
}

#undef BLOCK_HASH_SHIFT

__u32 ext2fs_get_ea_inode_hash(কাঠামো ext2_inode *inode)
{
	ফেরত inode->i_atime;
}

শূন্য ext2fs_set_ea_inode_hash(কাঠামো ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(কাঠামো ext2_inode *inode)
{
	ফেরত ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_version;
}

শূন্য ext2fs_set_ea_inode_ref(কাঠামো ext2_inode *inode, __u64 ref_গুনতি)
{
	inode->i_ctime = (__u32)(ref_গুনতি >> 32);
	inode->osd1.linux1.l_i_version = (__u32)ref_গুনতি;
}

স্থির errcode_t check_ext_attr_header(কাঠামো ext2_ext_attr_header *header)
{
	যদি ((header->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     header->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    header->h_blocks != 1)
		ফেরত EXT2_ET_BAD_EA_HEADER;

	ফেরত 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block, শূন্য *buf,
				ext2_ino_t inum)
{
	পূর্ণসংখ্যা		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	যদি (retval)
		ফেরত retval;

	যদি (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_block_csum_verify(fs, inum, block, buf))
		csum_failed = 1;

#ifdef শব্দগুলি_BIGENDIAN
	ext2fs_swap_ext_attr(buf, buf, fs->blocksize, 1);
#endif

	retval = check_ext_attr_header(buf);
	যদি (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	ফেরত retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block, শূন্য *buf)
{
	ফেরত ext2fs_read_ext_attr3(fs, block, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, শূন্য *buf)
{
	ফেরত ext2fs_read_ext_attr2(fs, block, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block, শূন্য *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	অক্ষর		*write_buf;

#ifdef শব্দগুলি_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	যদি (retval)
		ফেরত retval;
	ext2fs_swap_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#অন্যথা
	write_buf = (অক্ষর *) inbuf;
#endif

	retval = ext2fs_ext_attr_block_csum_set(fs, inum, block,
			(কাঠামো ext2_ext_attr_header *)write_buf);
	যদি (retval)
		ফেরত retval;

	retval = io_channel_write_blk64(fs->io, block, 1, write_buf);
#ifdef শব্দগুলি_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#endif
	যদি (!retval)
		ext2fs_mark_changed(fs);
	ফেরত retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block, শূন্য *inbuf)
{
	ফেরত ext2fs_write_ext_attr3(fs, block, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block, শূন্য *inbuf)
{
	ফেরত ext2fs_write_ext_attr2(fs, block, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    অক্ষর *block_buf, পূর্ণসংখ্যা adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	কাঠামো ext2_ext_attr_header *header;
	অক্ষর	*buf = 0;

	যদি ((blk >= ext2fs_blocks_গুনতি(fs->super)) ||
	    (blk < fs->super->s_first_তথ্য_block))
		ফেরত EXT2_ET_BAD_EA_BLOCK_অঙ্ক;

	যদি (!block_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		যদি (retval)
			ফেরত retval;
		block_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, block_buf, inum);
	যদি (retval)
		যাও errout;

	header = (কাঠামো ext2_ext_attr_header *) block_buf;
	header->h_refcount += adjust;
	যদি (newcount)
		*newcount = header->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, block_buf, inum);
	যদি (retval)
		যাও errout;

errout:
	যদি (buf)
		ext2fs_free_mem(&buf);
	ফেরত retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    অক্ষর *block_buf, পূর্ণসংখ্যা adjust,
				    __u32 *newcount)
{
	ফেরত ext2fs_adjust_ea_refcount3(fs, blk, block_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					অক্ষর *block_buf, পূর্ণসংখ্যা adjust,
					__u32 *newcount)
{
	ফেরত ext2fs_adjust_ea_refcount2(fs, blk, block_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
কাঠামো ext2_xattr {
	পূর্ণসংখ্যা name_সূচক;
	অক্ষর *name;
	অক্ষর *short_name;
	শূন্য *মান;
	unsigned পূর্ণসংখ্যা মান_len;
	ext2_ino_t ea_ino;
};

কাঠামো ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	কাঠামো ext2_xattr *attrs;
	পূর্ণসংখ্যা capacity;
	পূর্ণসংখ্যা গুনতি;
	পূর্ণসংখ্যা ibody_গুনতি;
	ext2_ino_t ino;
	unsigned পূর্ণসংখ্যা flags;
};

স্থির errcode_t ext2fs_xattrs_expand(কাঠামো ext2_xattr_handle *h,
				      unsigned পূর্ণসংখ্যা expandby)
{
	কাঠামো ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->capacity + expandby,
				   আকার(কাঠামো ext2_xattr), &new_attrs);
	যদি (err)
		ফেরত err;

	memcpy(new_attrs, h->attrs, h->capacity * আকার(কাঠামো ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->capacity += expandby;
	h->attrs = new_attrs;

	ফেরত 0;
}

কাঠামো ea_name_সূচক {
	পূর্ণসংখ্যা সূচক;
	ধ্রুবক অক্ষর *name;
};

/* Keep these names sorted in order of decreasing specificity. */
স্থির কাঠামো ea_name_সূচক ea_names[] = {
	{10, "gnu."},
	{3, "system.posix_acl_default"},
	{2, "system.posix_acl_access"},
	{8, "system.richacl"},
	{6, "security."},
	{4, "trusted."},
	{7, "system."},
	{1, "user."},
	{0, ফাঁকা},
};

স্থির ধ্রুবক অক্ষর *find_ea_prefix(পূর্ণসংখ্যা সূচক)
{
	কাঠামো ea_name_সূচক *e;

	জন্য (e = ea_names; e->name; e++)
		যদি (e->সূচক == সূচক)
			ফেরত e->name;

	ফেরত ফাঁকা;
}

স্থির পূর্ণসংখ্যা find_ea_সূচক(ধ্রুবক অক্ষর *fullname, ধ্রুবক অক্ষর **name, পূর্ণসংখ্যা *সূচক)
{
	কাঠামো ea_name_সূচক *e;

	জন্য (e = ea_names; e->name; e++) {
		যদি (strncmp(fullname, e->name, strlen(e->name)) == 0) {
			*name = fullname + strlen(e->name);
			*সূচক = e->সূচক;
			ফেরত 1;
		}
	}
	ফেরত 0;
}

errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       কাঠামো ext2_inode_large *inode)
{
	কাঠামো ext2_ext_attr_header *header;
	শূন্য *block_buf = ফাঁকা;
	blk64_t blk;
	errcode_t err;
	কাঠামো ext2_inode_large i;

	/* Read inode? */
	যদি (inode == ফাঁকা) {
		err = ext2fs_read_inode_full(fs, ino, (কাঠামো ext2_inode *)&i,
					     আকার(কাঠামো ext2_inode_large));
		যদি (err)
			ফেরত err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (কাঠামো ext2_inode *)inode);
	যদি (blk == 0)
		ফেরত 0;

	/* Find block, zero it, write back */
	যদি ((blk < fs->super->s_first_তথ্য_block) ||
	    (blk >= ext2fs_blocks_গুনতি(fs->super))) {
		err = EXT2_ET_BAD_EA_BLOCK_অঙ্ক;
		যাও out;
	}

	err = ext2fs_get_mem(fs->blocksize, &block_buf);
	যদি (err)
		যাও out;

	err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
	যদি (err)
		যাও out2;

	/* We only know how to deal with v2 EA blocks */
	header = (কাঠামো ext2_ext_attr_header *) block_buf;
	যদি (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_HEADER;
		যাও out2;
	}

	header->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
	যদি (err)
		যাও out2;

	/* Erase link to block */
	ext2fs_file_acl_block_set(fs, (কাঠামো ext2_inode *)inode, 0);
	যদি (header->h_refcount == 0)
		ext2fs_block_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_blocks(fs, (কাঠামো ext2_inode *)inode, 1);
	যদি (err)
		যাও out2;

	/* Write inode? */
	যদি (inode == &i) {
		err = ext2fs_write_inode_full(fs, ino, (কাঠামো ext2_inode *)&i,
					      আকার(কাঠামো ext2_inode_large));
		যদি (err)
			যাও out2;
	}

out2:
	ext2fs_free_mem(&block_buf);
out:
	ফেরত err;
}

স্থির errcode_t prep_ea_block_for_write(ext2_filsys fs, ext2_ino_t ino,
					 কাঠামো ext2_inode_large *inode)
{
	কাঠামো ext2_ext_attr_header *header;
	শূন্য *block_buf = ফাঁকা;
	blk64_t blk, goal;
	errcode_t err;

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (কাঠামো ext2_inode *)inode);
	যদি (blk != 0) {
		যদি ((blk < fs->super->s_first_তথ্য_block) ||
		    (blk >= ext2fs_blocks_গুনতি(fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_অঙ্ক;
			যাও out;
		}

		err = ext2fs_get_mem(fs->blocksize, &block_buf);
		যদি (err)
			যাও out;

		err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
		যদি (err)
			যাও out2;

		/* We only know how to deal with v2 EA blocks */
		header = (কাঠামো ext2_ext_attr_header *) block_buf;
		যদি (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			যাও out2;
		}

		/* Single-user block.  We're done here. */
		যদি (header->h_refcount == 1)
			যাও out2;

		/* We need to CoW the block. */
		header->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
		যদি (err)
			যাও out2;
	} অন্যথা {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_add_blocks(fs, (কাঠামো ext2_inode *)inode,
					     1);
		যদি (err)
			যাও out;
	}

	/* Allocate a block */
	goal = ext2fs_find_inode_goal(fs, ino, (কাঠামো ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, ফাঁকা, &blk);
	যদি (err)
		যাও out2;
	ext2fs_file_acl_block_set(fs, (কাঠামো ext2_inode *)inode, blk);
out2:
	যদি (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	ফেরত err;
}


স্থির অন্তঃরেখ পূর্ণসংখ্যা
posix_acl_xattr_গুনতি(size_t আকার_2)
{
        যদি (আকার_2 < আকার(posix_acl_xattr_header))
                ফেরত -1;
        আকার_2 -= আকার(posix_acl_xattr_header);
        যদি (আকার_2 % আকার(posix_acl_xattr_entry))
                ফেরত -1;
        ফেরত আকার_2 / আকার(posix_acl_xattr_entry);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
স্থির errcode_t convert_posix_acl_to_disk_বাফার(ধ্রুবক শূন্য *মান, size_t আকার_2,
						  শূন্য *out_buf, size_t *আকার_out)
{
	ধ্রুবক posix_acl_xattr_header *header =
		(ধ্রুবক posix_acl_xattr_header*) মান;
	ধ্রুবক posix_acl_xattr_entry *end, *entry =
		(ধ্রুবক posix_acl_xattr_entry *)(header+1);
	ext4_acl_header *ext_acl;
	size_t s;
	অক্ষর *e;

	পূর্ণসংখ্যা গুনতি;

	যদি (!মান)
		ফেরত EINVAL;
	যদি (আকার_2 < আকার(posix_acl_xattr_header))
		ফেরত ENOMEM;
	যদি (header->a_version != ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION))
		ফেরত EINVAL;

	গুনতি = posix_acl_xattr_গুনতি(আকার_2);
	ext_acl = out_buf;
	ext_acl->a_version = ext2fs_cpu_to_le32(EXT4_ACL_VERSION);

	যদি (গুনতি <= 0)
		ফেরত EINVAL;

	e = (অক্ষর *) out_buf + আকার(ext4_acl_header);
	s = আকার(ext4_acl_header);
	জন্য (end = entry + গুনতি; entry != end;entry++) {
		ext4_acl_entry *disk_entry = (ext4_acl_entry*) e;
		disk_entry->e_tag = entry->e_tag;
		disk_entry->e_perm = entry->e_perm;

		নির্বাচন(ext2fs_le16_to_cpu(entry->e_tag)) {
			ক্ষেত্র ACL_USER_OBJ:
			ক্ষেত্র ACL_GROUP_OBJ:
			ক্ষেত্র ACL_MASK:
			ক্ষেত্র ACL_OTHER:
				e += আকার(ext4_acl_entry_short);
				s += আকার(ext4_acl_entry_short);
				বিরতি;
			ক্ষেত্র ACL_USER:
			ক্ষেত্র ACL_GROUP:
				disk_entry->e_id = entry->e_id;
				e += আকার(ext4_acl_entry);
				s += আকার(ext4_acl_entry);
				বিরতি;
			পূর্বনির্ধারিত:
				ফেরত EINVAL;
		}
	}
	*আকার_out = s;
	ফেরত 0;
}

স্থির errcode_t convert_disk_বাফার_to_posix_acl(ধ্রুবক শূন্য *মান, size_t আকার_2,
						  শূন্য **out_buf, size_t *আকার_out)
{
	posix_acl_xattr_header *header;
	posix_acl_xattr_entry *entry;
	ধ্রুবক ext4_acl_header *ext_acl = (ধ্রুবক ext4_acl_header *) মান;
	errcode_t err;
	ধ্রুবক অক্ষর *cp;
	অক্ষর *out;

	যদি ((!মান) ||
	    (আকার_2 < আকার(ext4_acl_header)) ||
	    (ext_acl->a_version != ext2fs_cpu_to_le32(EXT4_ACL_VERSION)))
		ফেরত EINVAL;

	err = ext2fs_get_mem(আকার_2 * 2, &out);
	যদি (err)
		ফেরত err;

	header = (posix_acl_xattr_header *) out;
	header->a_version = ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION);
	entry = (posix_acl_xattr_entry *) (out + আকার(posix_acl_xattr_header));

	cp = (ধ্রুবক অক্ষর *) মান + আকার(ext4_acl_header);
	আকার_2 -= আকার(ext4_acl_header);

	যখন (আকার_2 > 0) {
		ধ্রুবক ext4_acl_entry *disk_entry = (ধ্রুবক ext4_acl_entry *) cp;

		entry->e_tag = disk_entry->e_tag;
		entry->e_perm = disk_entry->e_perm;

		নির্বাচন(ext2fs_le16_to_cpu(entry->e_tag)) {
			ক্ষেত্র ACL_USER_OBJ:
			ক্ষেত্র ACL_GROUP_OBJ:
			ক্ষেত্র ACL_MASK:
			ক্ষেত্র ACL_OTHER:
				entry->e_id = 0;
				cp += আকার(ext4_acl_entry_short);
				আকার_2 -= আকার(ext4_acl_entry_short);
				বিরতি;
			ক্ষেত্র ACL_USER:
			ক্ষেত্র ACL_GROUP:
				entry->e_id = disk_entry->e_id;
				cp += আকার(ext4_acl_entry);
				আকার_2 -= আকার(ext4_acl_entry);
				বিরতি;
			পূর্বনির্ধারিত:
				ext2fs_free_mem(&out);
				ফেরত EINVAL;
		}
		entry++;
	}
	*out_buf = out;
	*আকার_out = ((অক্ষর *) entry - out);
	ফেরত 0;
}

স্থির errcode_t
write_xattrs_to_বাফার(ext2_filsys fs, কাঠামো ext2_xattr *attrs, পূর্ণসংখ্যা গুনতি,
		       শূন্য *entries_start, unsigned পূর্ণসংখ্যা storage_আকার,
		       unsigned পূর্ণসংখ্যা মান_offset_correction, পূর্ণসংখ্যা write_hash)
{
	কাঠামো ext2_xattr *x;
	কাঠামো ext2_ext_attr_entry *e = entries_start;
	অক্ষর *end = (অক্ষর *) entries_start + storage_আকার;
	unsigned পূর্ণসংখ্যা মান_আকার;
	errcode_t err;

	memset(entries_start, 0, storage_আকার);
	জন্য (x = attrs; x < attrs + গুনতি; x++) {
		মান_আকার = ((x->মান_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = strlen(x->short_name);
		e->e_name_সূচক = x->name_সূচক;

		e->e_মান_আকার = x->মান_len;
		e->e_মান_inum = x->ea_ino;

		/* Store name */
		memcpy((অক্ষর *)e + আকার(*e), x->short_name, e->e_name_len);
		যদি (x->ea_ino) {
			e->e_মান_offs = 0;
		} অন্যথা {
			end -= মান_আকার;
			e->e_মান_offs = end - (অক্ষর *) entries_start +
						মান_offset_correction;
			memcpy(end, x->মান, e->e_মান_আকার);
		}

		যদি (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : end,
							  &e->e_hash);
			যদি (err)
				ফেরত err;
		} অন্যথা
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_পরবর্তী(e);
		*(__u32 *)e = 0;
	}
	ফেরত 0;
}

errcode_t ext2fs_xattrs_write(কাঠামো ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	ধ্রুবক unsigned পূর্ণসংখ্যা inode_আকার = EXT2_INODE_আকার(fs->super);
	কাঠামো ext2_inode_large *inode;
	অক্ষর *start, *block_buf = ফাঁকা;
	কাঠামো ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	blk64_t blk;
	unsigned পূর্ণসংখ্যা storage_আকার;
	unsigned পূর্ণসংখ্যা i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_আকার;
	যদি (i < আকার(*inode))
		i = আকার(*inode);
	err = ext2fs_get_memzero(i, &inode);
	যদি (err)
		ফেরত err;

	err = ext2fs_read_inode_full(fs, handle->ino, EXT2_INODE(inode),
				     inode_আকার);
	যদি (err)
		যাও out;

	/* If extra_isize isn't set, we need to set it now */
	যদি (inode->i_extra_isize == 0 &&
	    inode_আকার > EXT2_GOOD_OLD_INODE_আকার) {
		অক্ষর *p = (অক্ষর *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		যদি (extra == 0)
			extra = আকার(__u32);
		memset(p + EXT2_GOOD_OLD_INODE_আকার, 0, extra);
		inode->i_extra_isize = extra;
	}
	যদি (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		যাও out;
	}

	/* Does the inode have space for EA? */
	যদি (inode->i_extra_isize < আকার(inode->i_extra_isize) ||
	    inode_আকার <= EXT2_GOOD_OLD_INODE_আকার + inode->i_extra_isize +
								আকার(__u32))
		যাও write_ea_block;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	memcpy(((অক্ষর *) inode) + EXT2_GOOD_OLD_INODE_আকার +
	       inode->i_extra_isize, &ea_inode_magic, আকার(__u32));
	storage_আকার = inode_আকার - EXT2_GOOD_OLD_INODE_আকার -
				inode->i_extra_isize - আকার(__u32);
	start = ((অক্ষর *) inode) + EXT2_GOOD_OLD_INODE_আকার +
				inode->i_extra_isize + আকার(__u32);

	err = write_xattrs_to_বাফার(fs, handle->attrs, handle->ibody_গুনতি,
				     start, storage_আকার, 0, 0);
	যদি (err)
		যাও out;
write_ea_block:
	/* Are we done? */
	যদি (handle->ibody_গুনতি == handle->গুনতি &&
	    !ext2fs_file_acl_block(fs, EXT2_INODE(inode)))
		যাও skip_ea_block;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &block_buf);
	যদি (err)
		যাও out;

	storage_আকার = fs->blocksize - আকার(কাঠামো ext2_ext_attr_header);
	start = block_buf + আকার(কাঠামো ext2_ext_attr_header);

	err = write_xattrs_to_বাফার(fs, handle->attrs + handle->ibody_গুনতি,
				     handle->গুনতি - handle->ibody_গুনতি, start,
				     storage_আকার, start - block_buf, 1);
	যদি (err)
		যাও out2;

	/* Write a header on the EA block */
	header = (কাঠামো ext2_ext_attr_header *) block_buf;
	header->h_magic = EXT2_EXT_ATTR_MAGIC;
	header->h_refcount = 1;
	header->h_blocks = 1;

	/* Get a new block for writing */
	err = prep_ea_block_for_write(fs, handle->ino, inode);
	যদি (err)
		যাও out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_block(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, handle->ino);
	যদি (err)
		যাও out2;

skip_ea_block:
	blk = ext2fs_file_acl_block(fs, (কাঠামো ext2_inode *)inode);
	যদি (!block_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		যদি (err)
			যাও out;
	}

	/* Write the inode */
	err = ext2fs_write_inode_full(fs, handle->ino, EXT2_INODE(inode),
				      inode_আকার);
	যদি (err)
		যাও out2;

out2:
	ext2fs_free_mem(&block_buf);
out:
	ext2fs_free_mem(&inode);
	ফেরত err;
}

স্থির errcode_t read_xattrs_from_বাফার(কাঠামো ext2_xattr_handle *handle,
					 কাঠামো ext2_inode_large *inode,
					 কাঠামো ext2_ext_attr_entry *entries,
					 unsigned পূর্ণসংখ্যা storage_আকার,
					 অক্ষর *মান_start)
{
	কাঠামো ext2_xattr *x;
	কাঠামো ext2_ext_attr_entry *entry, *end;
	ধ্রুবক অক্ষর *prefix;
	unsigned পূর্ণসংখ্যা remain, prefix_len;
	errcode_t err;
	unsigned পূর্ণসংখ্যা মানসমূহ_আকার = storage_আকার +
			((অক্ষর *)entries - মান_start);

	/* find the end */
	end = entries;
	remain = storage_আকার;
	যখন (remain >= আকার(কাঠামো ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(end)) {

		/* header eats this space */
		remain -= আকার(কাঠামো ext2_ext_attr_entry);

		/* is attribute name valid? */
		যদি (EXT2_EXT_ATTR_আকার(end->e_name_len) > remain)
			ফেরত EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_আকার(end->e_name_len);
		end = EXT2_EXT_ATTR_পরবর্তী(end);
	}

	entry = entries;
	remain = storage_আকার;
	যখন (remain >= আকার(কাঠামো ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(entry)) {

		/* Allocate space for more attrs? */
		যদি (handle->গুনতি == handle->capacity) {
			err = ext2fs_xattrs_expand(handle, 4);
			যদি (err)
				ফেরত err;
		}

		x = handle->attrs + handle->গুনতি;

		/* header eats this space */
		remain -= আকার(কাঠামো ext2_ext_attr_entry);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_আকার(entry->e_name_len);

		/* Extract name */
		prefix = find_ea_prefix(entry->e_name_সূচক);
		prefix_len = (prefix ? strlen(prefix) : 0);
		err = ext2fs_get_memzero(entry->e_name_len + prefix_len + 1,
					 &x->name);
		যদি (err)
			ফেরত err;
		যদি (prefix)
			memcpy(x->name, prefix, prefix_len);
		যদি (entry->e_name_len)
			memcpy(x->name + prefix_len,
			       (অক্ষর *)entry + আকার(*entry),
			       entry->e_name_len);
		x->short_name = x->name + prefix_len;
		x->name_সূচক = entry->e_name_সূচক;

		/* Check & copy value */
		যদি (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    entry->e_মান_inum != 0)
			ফেরত EXT2_ET_BAD_EA_BLOCK_অঙ্ক;

		যদি (entry->e_মান_inum == 0) {
			যদি (entry->e_মান_আকার > remain)
				ফেরত EXT2_ET_EA_BAD_মান_আকার;

			যদি (entry->e_মান_offs + entry->e_মান_আকার > মানসমূহ_আকার)
				ফেরত EXT2_ET_EA_BAD_মান_OFFSET;

			যদি (entry->e_মান_আকার > 0 &&
			    মান_start + entry->e_মান_offs <
			    (অক্ষর *)end + আকার(__u32))
				ফেরত EXT2_ET_EA_BAD_মান_OFFSET;

			remain -= entry->e_মান_আকার;

			err = ext2fs_get_mem(entry->e_মান_আকার, &x->মান);
			যদি (err)
				ফেরত err;
			memcpy(x->মান, মান_start + entry->e_মান_offs,
			       entry->e_মান_আকার);
		} অন্যথা {
			কাঠামো ext2_inode *ea_inode;
			ext2_file_t ea_file;

			যদি (entry->e_মান_offs != 0)
				ফেরত EXT2_ET_EA_BAD_মান_OFFSET;

			যদি (entry->e_মান_আকার > (64 * 1024))
				ফেরত EXT2_ET_EA_BAD_মান_আকার;

			err = ext2fs_get_mem(entry->e_মান_আকার, &x->মান);
			যদি (err)
				ফেরত err;

			err = ext2fs_file_open(handle->fs, entry->e_মান_inum,
					       0, &ea_file);
			যদি (err)
				ফেরত err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			যদি ((ea_inode->i_flags & EXT4_INLINE_তথ্য_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_গুনতি == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			অন্যথা যদি ((__u64) ext2fs_file_get_আকার(ea_file) !=
				 entry->e_মান_আকার)
				err = EXT2_ET_EA_BAD_মান_আকার;
			অন্যথা
				err = ext2fs_file_read(ea_file, x->মান,
						       entry->e_মান_আকার, 0);
			ext2fs_file_close(ea_file);
			যদি (err)
				ফেরত err;
		}

		x->ea_ino = entry->e_মান_inum;
		x->মান_len = entry->e_মান_আকার;

		/* e_hash may be 0 in older inode's ea */
		যদি (entry->e_hash != 0) {
			__u32 hash, signed_hash;

			শূন্য *তথ্য = (entry->e_মান_inum != 0) ?
					0 : মান_start + entry->e_মান_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, entry,
							  তথ্য, &hash,
							  &signed_hash);
			যদি (err)
				ফেরত err;
			যদি ((entry->e_hash != hash) &&
			    (entry->e_hash != signed_hash)) {
				কাঠামো ext2_inode child;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							entry->e_মান_inum,
							&child);
				যদি (err)
					ফেরত err;
				যদি (child.i_mtime != handle->ino ||
				    child.i_generation != inode->i_generation)
					ফেরত EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->গুনতি++;
		entry = EXT2_EXT_ATTR_পরবর্তী(entry);
	}

	ফেরত 0;
}

স্থির শূন্য xattrs_free_চাবিগুলি(কাঠামো ext2_xattr_handle *h)
{
	কাঠামো ext2_xattr *a = h->attrs;
	পূর্ণসংখ্যা i;

	জন্য (i = 0; i < h->capacity; i++) {
		যদি (a[i].name)
			ext2fs_free_mem(&a[i].name);
		যদি (a[i].মান)
			ext2fs_free_mem(&a[i].মান);
	}
	h->গুনতি = 0;
	h->ibody_গুনতি = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(কাঠামো ext2_xattr_handle *handle,
				   কাঠামো ext2_inode_large *inode)
{
	কাঠামো ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	unsigned পূর্ণসংখ্যা storage_আকার;
	অক্ষর *start, *block_buf = ফাঁকা;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_চাবিগুলি(handle);

	/* Does the inode have space for EA? */
	যদি (inode->i_extra_isize < আকার(inode->i_extra_isize) ||
	    EXT2_INODE_আকার(handle->fs->super) <= EXT2_GOOD_OLD_INODE_আকার +
						  inode->i_extra_isize +
						  আকার(__u32))
		যাও read_ea_block;
	যদি (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		যাও out;
	}

	/* Look for EA in the inode */
	memcpy(&ea_inode_magic, ((অক্ষর *) inode) + EXT2_GOOD_OLD_INODE_আকার +
	       inode->i_extra_isize, আকার(__u32));
	যদি (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_আকার = EXT2_INODE_আকার(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_আকার - inode->i_extra_isize -
			আকার(__u32);
		start = ((অক্ষর *) inode) + EXT2_GOOD_OLD_INODE_আকার +
			inode->i_extra_isize + আকার(__u32);

		err = read_xattrs_from_বাফার(handle, inode,
					(কাঠামো ext2_ext_attr_entry *) start,
					storage_আকার, start);
		যদি (err)
			যাও out;

		handle->ibody_গুনতি = handle->গুনতি;
	}

read_ea_block:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_block(handle->fs, EXT2_INODE(inode));
	যদি (blk != 0) {
		যদি ((blk < handle->fs->super->s_first_তথ্য_block) ||
		    (blk >= ext2fs_blocks_গুনতি(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_অঙ্ক;
			যাও out;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &block_buf);
		যদি (err)
			যাও out;

		err = ext2fs_read_ext_attr3(handle->fs, blk, block_buf,
					    handle->ino);
		যদি (err)
			যাও out3;

		/* We only know how to deal with v2 EA blocks */
		header = (কাঠামো ext2_ext_attr_header *) block_buf;
		যদি (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			যাও out3;
		}

		/* Read EAs */
		storage_আকার = handle->fs->blocksize -
			আকার(কাঠামো ext2_ext_attr_header);
		start = block_buf + আকার(কাঠামো ext2_ext_attr_header);
		err = read_xattrs_from_বাফার(handle, inode,
					(কাঠামো ext2_ext_attr_entry *) start,
					storage_আকার, block_buf);
	}

out3:
	যদি (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	ফেরত err;
}

errcode_t ext2fs_xattrs_read(কাঠামো ext2_xattr_handle *handle)
{
	কাঠামো ext2_inode_large *inode;
	size_t inode_আকার = EXT2_INODE_আকার(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	যদি (inode_আকার < আকার(*inode))
		inode_আকার = আকার(*inode);
	err = ext2fs_get_memzero(inode_আকার, &inode);
	যদি (err)
		ফেরত err;

	err = ext2fs_read_inode_full(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_আকার(handle->fs->super));
	যদি (err)
		যাও out;

	err = ext2fs_xattrs_read_inode(handle, inode);

out:
	ext2fs_free_mem(&inode);

	ফেরত err;
}

errcode_t ext2fs_xattrs_iterate(কাঠামো ext2_xattr_handle *h,
				পূর্ণসংখ্যা (*func)(অক্ষর *name, অক্ষর *মান,
					    size_t মান_len, শূন্য *তথ্য),
				শূন্য *তথ্য)
{
	কাঠামো ext2_xattr *x;
	পূর্ণসংখ্যা dirty = 0;
	পূর্ণসংখ্যা ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	জন্য (x = h->attrs; x < h->attrs + h->গুনতি; x++) {
		ret = func(x->name, x->মান, x->মান_len, তথ্য);
		যদি (ret & XATTR_CHANGED)
			dirty = 1;
		যদি (ret & XATTR_ABORT)
			বিরতি;
	}

	যদি (dirty)
		ফেরত ext2fs_xattrs_write(h);
	ফেরত 0;
}

errcode_t ext2fs_xattr_get(কাঠামো ext2_xattr_handle *h, ধ্রুবক অক্ষর *চাবি,
			   শূন্য **মান, size_t *মান_len)
{
	কাঠামো ext2_xattr *x;
	অক্ষর *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	জন্য (x = h->attrs; x < h->attrs + h->গুনতি; x++) {
		যদি (strcmp(x->name, চাবি))
			চালু;

		যদি (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(চাবি, "system.posix_acl_default") == 0) ||
		     (strcmp(চাবি, "system.posix_acl_access") == 0))) {
			err = convert_disk_বাফার_to_posix_acl(x->মান, x->মান_len,
							       মান, মান_len);
			ফেরত err;
		} অন্যথা {
			err = ext2fs_get_mem(x->মান_len, &val);
			যদি (err)
				ফেরত err;
			memcpy(val, x->মান, x->মান_len);
			*মান = val;
			*মান_len = x->মান_len;
			ফেরত 0;
		}
	}

	ফেরত EXT2_ET_EA_চাবি_NOT_FOUND;
}

errcode_t ext2fs_xattr_inode_max_আকার(ext2_filsys fs, ext2_ino_t ino,
				      size_t *আকার_2)
{
	কাঠামো ext2_ext_attr_entry *entry;
	কাঠামো ext2_inode_large *inode;
	__u32 ea_inode_magic;
	unsigned পূর্ণসংখ্যা minoff;
	অক্ষর *start;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_আকার(fs->super);
	যদি (i < আকার(*inode))
		i = আকার(*inode);
	err = ext2fs_get_memzero(i, &inode);
	যদি (err)
		ফেরত err;

	err = ext2fs_read_inode_full(fs, ino, (কাঠামো ext2_inode *)inode,
				     EXT2_INODE_আকার(fs->super));
	যদি (err)
		যাও out;

	/* Does the inode have size for EA? */
	যদি (EXT2_INODE_আকার(fs->super) <= EXT2_GOOD_OLD_INODE_আকার +
						  inode->i_extra_isize +
						  আকার(__u32)) {
		err = EXT2_ET_INLINE_তথ্য_NO_SPACE;
		যাও out;
	}

	minoff = EXT2_INODE_আকার(fs->super) - আকার(*inode) - আকার(__u32);
	memcpy(&ea_inode_magic, ((অক্ষর *) inode) + EXT2_GOOD_OLD_INODE_আকার +
	       inode->i_extra_isize, আকার(__u32));
	যদি (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		start= ((অক্ষর *) inode) + EXT2_GOOD_OLD_INODE_আকার +
			inode->i_extra_isize + আকার(__u32);
		entry = (কাঠামো ext2_ext_attr_entry *) start;
		যখন (!EXT2_EXT_IS_LAST_ENTRY(entry)) {
			যদি (!entry->e_মান_inum && entry->e_মান_আকার) {
				unsigned পূর্ণসংখ্যা offs = entry->e_মান_offs;
				যদি (offs < minoff)
					minoff = offs;
			}
			entry = EXT2_EXT_ATTR_পরবর্তী(entry);
		}
		*আকার_2 = minoff - ((অক্ষর *)entry - (অক্ষর *)start) - আকার(__u32);
	} অন্যথা {
		/* no xattr.  return a maximum size */
		*আকার_2 = EXT2_EXT_ATTR_আকার(minoff -
					   EXT2_EXT_ATTR_LEN(strlen("data")) -
					   EXT2_EXT_ATTR_ROUND - আকার(__u32));
	}

out:
	ext2fs_free_mem(&inode);
	ফেরত err;
}

স্থির errcode_t xattr_create_ea_inode(ext2_filsys fs, ধ্রুবক শূন্য *মান,
				       size_t মান_len, ext2_ino_t *ea_ino)
{
	কাঠামো ext2_inode inode;
	ext2_ino_t ino;
	ext2_file_t file;
	__u32 hash;
	errcode_t ret;

	ret = ext2fs_new_inode(fs, 0, 0, 0, &ino);
	যদি (ret)
		ফেরত ret;

	memset(&inode, 0, আকার(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	যদি (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_আকার = 0;
	inode.i_mode = LINUX_S_IFREG | 0600;
	inode.i_links_গুনতি = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	যদি (ret)
		ফেরত ret;
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
		ফেরত ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	যদি (ret)
		ফেরত ret;
	ret = ext2fs_file_write(file, মান, মান_len, ফাঁকা);
	ext2fs_file_close(file);
	যদি (ret)
		ফেরত ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	ফেরত 0;
}

স্থির errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	কাঠামো ext2_inode_large inode;
	__u64 ref_গুনতি;
	errcode_t ret;

	ret = ext2fs_read_inode_full(fs, ino, (কাঠামো ext2_inode *)&inode,
				     আকার(inode));
	যদি (ret)
		যাও out;

	ref_গুনতি = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_গুনতি--;
	ext2fs_set_ea_inode_ref(EXT2_INODE(&inode), ref_গুনতি);

	যদি (ref_গুনতি)
		যাও write_out;

	inode.i_links_গুনতি = 0;
	ext2fs_set_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	যদি (ret)
		যাও write_out;

	যদি (ext2fs_inode_has_valid_blocks2(fs, (কাঠামো ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (কাঠামো ext2_inode *)&inode, ফাঁকা,
				   0, ~0ULL);
		যদি (ret)
			যাও out;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_out:
	ret = ext2fs_write_inode_full(fs, ino, (কাঠামো ext2_inode *)&inode,
				      আকার(inode));
out:
	ফেরত ret;
}

স্থির errcode_t xattr_update_entry(ext2_filsys fs, কাঠামো ext2_xattr *x,
				    ধ্রুবক অক্ষর *name, ধ্রুবক অক্ষর *short_name,
				    পূর্ণসংখ্যা সূচক, ধ্রুবক শূন্য *মান,
				    size_t মান_len, পূর্ণসংখ্যা in_inode)
{
	ext2_ino_t ea_ino = 0;
	শূন্য *new_মান = ফাঁকা;
	অক্ষর *new_name = ফাঁকা;
	পূর্ণসংখ্যা name_len;
	errcode_t ret;

	যদি (!x->name) {
		name_len = strlen(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		যদি (ret)
			যাও fail;
		memcpy(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(মান_len, &new_মান);
	যদি (ret)
		যাও fail;
	memcpy(new_মান, মান, মান_len);

	যদি (in_inode) {
		ret = xattr_create_ea_inode(fs, মান, মান_len, &ea_ino);
		যদি (ret)
			যাও fail;
	}

	যদি (x->ea_ino) {
		ret = xattr_inode_dec_ref(fs, x->ea_ino);
		যদি (ret)
			যাও fail;
	}

	যদি (!x->name) {
		x->name = new_name;
		x->short_name = new_name + (short_name  - name);
	}
	x->name_সূচক = সূচক;

	যদি (x->মান)
		ext2fs_free_mem(&x->মান);
	x->মান = new_মান;
	x->মান_len = মান_len;
	x->ea_ino = ea_ino;
	ফেরত 0;
fail:
	যদি (new_name)
		ext2fs_free_mem(&new_name);
	যদি (new_মান)
		ext2fs_free_mem(&new_মান);
	যদি (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা xattr_find_position(কাঠামো ext2_xattr *attrs, পূর্ণসংখ্যা গুনতি,
			       ধ্রুবক অক্ষর *shortname, পূর্ণসংখ্যা name_idx)
{
	কাঠামো ext2_xattr *x;
	পূর্ণসংখ্যা i;
	পূর্ণসংখ্যা shortname_len, x_shortname_len;

	shortname_len = strlen(shortname);

	জন্য (i = 0, x = attrs; i < গুনতি; i++, x++) {
		যদি (name_idx < x->name_সূচক)
			বিরতি;
		যদি (name_idx > x->name_সূচক)
			চালু;

		x_shortname_len = strlen(x->short_name);
		যদি (shortname_len < x_shortname_len)
			বিরতি;
		যদি (shortname_len > x_shortname_len)
			চালু;

		যদি (memcmp(shortname, x->short_name, shortname_len) <= 0)
			বিরতি;
	}
	ফেরত i;
}

স্থির errcode_t xattr_array_update(কাঠামো ext2_xattr_handle *h,
				    ধ্রুবক অক্ষর *name,
				    ধ্রুবক শূন্য *মান, size_t মান_len,
				    পূর্ণসংখ্যা ibody_free, পূর্ণসংখ্যা block_free,
				    পূর্ণসংখ্যা old_idx, পূর্ণসংখ্যা in_inode)
{
	কাঠামো ext2_xattr tmp;
	পূর্ণসংখ্যা add_to_ibody;
	পূর্ণসংখ্যা needed;
	পূর্ণসংখ্যা name_len, name_idx = 0;
	ধ্রুবক অক্ষর *shortname = name;
	পূর্ণসংখ্যা new_idx;
	পূর্ণসংখ্যা ret;

	find_ea_সূচক(name, &shortname, &name_idx);
	name_len = strlen(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	যদি (!in_inode)
		needed += EXT2_EXT_ATTR_আকার(মান_len);

	যদি (old_idx >= 0 && old_idx < h->ibody_গুনতি) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		যদি (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_আকার(
						h->attrs[old_idx].মান_len);
	}

	যদি (needed <= ibody_free) {
		যদি (old_idx < 0) {
			new_idx = h->ibody_গুনতি;
			add_to_ibody = 1;
			যাও add_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, মান,
					 মান_len, in_inode);
		যদি (ret)
			ফেরত ret;
		যদি (h->ibody_গুনতি <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_গুনতি + 1,
				h->attrs + h->ibody_গুনতি,
				(old_idx - h->ibody_গুনতি) * আকার(*h->attrs));
			h->attrs[h->ibody_গুনতি] = tmp;
			h->ibody_গুনতি++;
		}
		ফেরত 0;
	}

	যদি (h->ibody_গুনতি <= old_idx) {
		block_free += EXT2_EXT_ATTR_LEN(name_len);
		যদি (!h->attrs[old_idx].ea_ino)
			block_free +=
				EXT2_EXT_ATTR_আকার(h->attrs[old_idx].মান_len);
	}

	যদি (needed > block_free)
		ফেরত EXT2_ET_EA_NO_SPACE;

	যদি (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, মান,
					 মান_len, in_inode);
		যদি (ret)
			ফেরত ret;
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
				(new_idx - old_idx) * আকার(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_গুনতি--;
		}
		ফেরত 0;
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
			ফেরত ret;
	}

	ret = xattr_update_entry(h->fs, &h->attrs[h->গুনতি], name, shortname,
				 name_idx, মান, মান_len, in_inode);
	যদি (ret)
		ফেরত ret;

	tmp = h->attrs[h->গুনতি];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->গুনতি - new_idx)*আকার(*h->attrs));
	h->attrs[new_idx] = tmp;
	যদি (add_to_ibody)
		h->ibody_গুনতি++;
	h->গুনতি++;
	ফেরত 0;
}

স্থির পূর্ণসংখ্যা space_used(কাঠামো ext2_xattr *attrs, পূর্ণসংখ্যা গুনতি)
{
	পূর্ণসংখ্যা যোগ_2 = 0;
	কাঠামো ext2_xattr *x;
	পূর্ণসংখ্যা i, len;

	জন্য (i = 0, x = attrs; i < গুনতি; i++, x++) {
		len = strlen(x->short_name);
		যোগ_2 += EXT2_EXT_ATTR_LEN(len);
		যদি (!x->ea_ino)
			যোগ_2 += EXT2_EXT_ATTR_আকার(x->মান_len);
	}
	ফেরত যোগ_2;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#define EXT4_XATTR_MIN_LARGE_EA_আকার(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - আকার(কাঠামো ext2_ext_attr_header) - 4)

errcode_t ext2fs_xattr_set(কাঠামো ext2_xattr_handle *h,
			   ধ্রুবক অক্ষর *name,
			   ধ্রুবক শূন্য *মান,
			   size_t মান_len)
{
	ext2_filsys fs = h->fs;
	ধ্রুবক পূর্ণসংখ্যা inode_আকার = EXT2_INODE_আকার(fs->super);
	কাঠামো ext2_inode_large *inode = ফাঁকা;
	কাঠামো ext2_xattr *x;
	অক্ষর *new_মান;
	পূর্ণসংখ্যা ibody_free, block_free;
	পূর্ণসংখ্যা in_inode = 0;
	পূর্ণসংখ্যা old_idx = -1;
	পূর্ণসংখ্যা extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(মান_len, &new_মান);
	যদি (ret)
		ফেরত ret;
	যদি (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((strcmp(name, "system.posix_acl_default") == 0) ||
	     (strcmp(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_to_disk_বাফার(মান, মান_len,
						       new_মান, &মান_len);
		যদি (ret)
			যাও out;
	} অন্যথা যদি (মান_len)
		memcpy(new_মান, মান, মান_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	জন্য (x = h->attrs; x < h->attrs + h->গুনতি; x++) {
		যদি (!strcmp(x->name, name)) {
			যদি (!x->ea_ino && x->মান_len == মান_len &&
			    (!মান_len ||
			     !memcmp(x->মান, new_মান, মান_len))) {
				ret = 0;
				যাও out;
			}
			old_idx = x - h->attrs;
			বিরতি;
		}
	}

	ret = ext2fs_get_memzero(inode_আকার, &inode);
	যদি (ret)
		যাও out;
	ret = ext2fs_read_inode_full(fs, h->ino,
				     (কাঠামো ext2_inode *)inode,
				     inode_আকার);
	যদি (ret)
		যাও out;
	যদি (inode_আকার > EXT2_GOOD_OLD_INODE_আকার) {
		extra_isize = inode->i_extra_isize;
		যদি (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			যদি (extra_isize == 0)
				extra_isize = আকার(__u32);
		}
		ibody_free = inode_আকার - EXT2_GOOD_OLD_INODE_আকার;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= আকার(__u32) * 2;
		ibody_free -= space_used(h->attrs, h->ibody_গুনতি);
	} অন্যথা
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	যদি (strcmp(name, "system.data") == 0) {
		যদি (h->ibody_গুনতি <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			যাও out;
		}
		ret = xattr_array_update(h, name, new_মান, মান_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		যদি (ret)
			যাও out;
		যাও write_out;
	}

	block_free = fs->blocksize;
	block_free -= আকার(কাঠামো ext2_ext_attr_header);
	/* Final null entry. */
	block_free -= আকার(__u32);
	block_free -= space_used(h->attrs + h->ibody_গুনতি,
				 h->গুনতি - h->ibody_গুনতি);

	যদি (ext2fs_has_feature_ea_inode(fs->super) &&
	    মান_len > EXT4_XATTR_MIN_LARGE_EA_আকার(fs->blocksize))
		in_inode = 1;

	ret = xattr_array_update(h, name, new_মান, মান_len, ibody_free,
				 block_free, old_idx, in_inode);
	যদি (ret == EXT2_ET_EA_NO_SPACE && !in_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_মান, মান_len,
			ibody_free, block_free, old_idx, 1 /* in_inode */);
	যদি (ret)
		যাও out;

write_out:
	ret = ext2fs_xattrs_write(h);
out:
	যদি (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_মান);
	ফেরত ret;
}

errcode_t ext2fs_xattr_remove(কাঠামো ext2_xattr_handle *handle,
			      ধ্রুবক অক্ষর *চাবি)
{
	কাঠামো ext2_xattr *x;
	কাঠামো ext2_xattr *end = handle->attrs + handle->গুনতি;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	জন্য (x = handle->attrs; x < end; x++) {
		যদি (strcmp(x->name, চাবি) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->মান);
			যদি (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (end - x - 1)*আকার(*x));
			memset(end - 1, 0, আকার(*end));
			যদি (x < handle->attrs + handle->ibody_গুনতি)
				handle->ibody_গুনতি--;
			handle->গুনতি--;
			ফেরত ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	ফেরত 0;
}

errcode_t ext2fs_xattr_remove_all(কাঠামো ext2_xattr_handle *handle)
{
	কাঠামো ext2_xattr *x;
	কাঠামো ext2_xattr *end = handle->attrs + handle->গুনতি;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	জন্য (x = handle->attrs; x < end; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->মান);
		যদি (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_গুনতি = 0;
	handle->গুনতি = 0;
	ফেরত ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     কাঠামো ext2_xattr_handle **handle)
{
	কাঠামো ext2_xattr_handle *h;
	errcode_t err;

	যদি (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_তথ্য(fs->super))
		ফেরত EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(আকার(*h), &h);
	যদি (err)
		ফেরত err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->capacity = 4;
	err = ext2fs_get_arrayzero(h->capacity, আকার(কাঠামো ext2_xattr),
				   &h->attrs);
	যদি (err) {
		ext2fs_free_mem(&h);
		ফেরত err;
	}
	h->গুনতি = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	ফেরত 0;
}

errcode_t ext2fs_xattrs_close(কাঠামো ext2_xattr_handle **handle)
{
	কাঠামো ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_চাবিগুলি(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	ফেরত 0;
}

errcode_t ext2fs_xattrs_গুনতি(কাঠামো ext2_xattr_handle *handle, size_t *গুনতি)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*গুনতি = handle->গুনতি;
	ফেরত 0;
}

errcode_t ext2fs_xattrs_flags(কাঠামো ext2_xattr_handle *handle,
			      unsigned পূর্ণসংখ্যা *new_flags, unsigned পূর্ণসংখ্যা *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	যদি (old_flags)
		*old_flags = handle->flags;
	যদি (new_flags)
		handle->flags = *new_flags;
	ফেরত 0;
}
