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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_ext_attr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext4_acl.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

ਸਥਿਰ errcode_t read_ea_inode_hash(ext2_filsys fs, ext2_ino_t ino, __u32 *hash)
{
	ਬਣਤਰ ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	ਵਾਪਸ 0;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NAME_HASH_ਖਿਸਕਾਓ 5
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਮੁੱਲ_HASH_ਖਿਸਕਾਓ 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_ਇੰਦਰਾਜ(ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ, ਖਾਲੀ *ਅੰਕੜੇ)
{
	__u32 hash = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *name = (((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) ਇੰਦਰਾਜ) +
			       ਆਕਾਰ(ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ));
	ਪੂਰਨ n;

	ਲਈ (n = 0; n < ਇੰਦਰਾਜ->e_name_len; n++) {
		hash = (hash << NAME_HASH_ਖਿਸਕਾਓ) ^
		       (hash >> (8*ਆਕਾਰ(hash) - NAME_HASH_ਖਿਸਕਾਓ)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	ਜੇ (ਇੰਦਰਾਜ->e_ਮੁੱਲ_inum == 0 && ਇੰਦਰਾਜ->e_ਮੁੱਲ_ਮਾਪ != 0) {
		__u32 *ਮੁੱਲ = (__u32 *)ਅੰਕੜੇ;
		ਲਈ (n = (ਇੰਦਰਾਜ->e_ਮੁੱਲ_ਮਾਪ + EXT2_EXT_ATTR_ਗੇੜਾ) >>
			 EXT2_EXT_ATTR_PAD_ਬਿੱਟ; n; n--) {
			hash = (hash << ਮੁੱਲ_HASH_ਖਿਸਕਾਓ) ^
			       (hash >> (8*ਆਕਾਰ(hash) - ਮੁੱਲ_HASH_ਖਿਸਕਾਓ)) ^
			       ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(*ਮੁੱਲ++);
		}
	}

	ਵਾਪਸ hash;
}

__u32 ext2fs_ext_attr_hash_ਇੰਦਰਾਜ_signed(ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ,
					ਖਾਲੀ *ਅੰਕੜੇ)
{
	__u32 hash = 0;
	ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ *name = (((ਨਿਸ਼ਾਨੀਵਾਲਾ ਅੱਖਰ *) ਇੰਦਰਾਜ) +
			     ਆਕਾਰ(ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ));
	ਪੂਰਨ n;

	ਲਈ (n = 0; n < ਇੰਦਰਾਜ->e_name_len; n++) {
		hash = (hash << NAME_HASH_ਖਿਸਕਾਓ) ^
		       (hash >> (8*ਆਕਾਰ(hash) - NAME_HASH_ਖਿਸਕਾਓ)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	ਜੇ (ਇੰਦਰਾਜ->e_ਮੁੱਲ_inum == 0 && ਇੰਦਰਾਜ->e_ਮੁੱਲ_ਮਾਪ != 0) {
		__u32 *ਮੁੱਲ = (__u32 *)ਅੰਕੜੇ;
		ਲਈ (n = (ਇੰਦਰਾਜ->e_ਮੁੱਲ_ਮਾਪ + EXT2_EXT_ATTR_ਗੇੜਾ) >>
			 EXT2_EXT_ATTR_PAD_ਬਿੱਟ; n; n--) {
			hash = (hash << ਮੁੱਲ_HASH_ਖਿਸਕਾਓ) ^
			       (hash >> (8*ਆਕਾਰ(hash) - ਮੁੱਲ_HASH_ਖਿਸਕਾਓ)) ^
			       ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(*ਮੁੱਲ++);
		}
	}

	ਵਾਪਸ hash;
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
				      ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ,
				      ਖਾਲੀ *ਅੰਕੜੇ, __u32 *hash,
				      __u32 *signed_hash)
{
	*hash = ext2fs_ext_attr_hash_ਇੰਦਰਾਜ(ਇੰਦਰਾਜ, ਅੰਕੜੇ);
	ਜੇ (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_ਇੰਦਰਾਜ_signed(ਇੰਦਰਾਜ, ਅੰਕੜੇ);

	ਜੇ (ਇੰਦਰਾਜ->e_ਮੁੱਲ_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, ਇੰਦਰਾਜ->e_ਮੁੱਲ_inum,
					    &ea_inode_hash);
		ਜੇ (retval)
			ਵਾਪਸ retval;

		*hash = (*hash << ਮੁੱਲ_HASH_ਖਿਸਕਾਓ) ^
			(*hash >> (8*ਆਕਾਰ(*hash) - ਮੁੱਲ_HASH_ਖਿਸਕਾਓ)) ^
			ea_inode_hash;
		ਜੇ (signed_hash)
			*signed_hash = (*signed_hash << ਮੁੱਲ_HASH_ਖਿਸਕਾਓ) ^
				(*signed_hash >> (8*ਆਕਾਰ(*hash) -
						  ਮੁੱਲ_HASH_ਖਿਸਕਾਓ)) ^
				ea_inode_hash;
	}
	ਵਾਪਸ 0;
}

/*
 * ext2fs_ext_attr_hash_entry2()
 *
 * Compute the hash of an extended attribute.
 * This version of the function supports hashing entries that reference
 * external inodes (ea_inode feature).
 */
errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
				      ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ,
				      ਖਾਲੀ *ਅੰਕੜੇ, __u32 *hash)
{
	ਵਾਪਸ ext2fs_ext_attr_hash_entry3(fs, ਇੰਦਰਾਜ, ਅੰਕੜੇ, hash, ਨਿਰਾ);
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ NAME_HASH_ਖਿਸਕਾਓ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ਮੁੱਲ_HASH_ਖਿਸਕਾਓ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਖੰਡ_HASH_ਖਿਸਕਾਓ 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
ਖਾਲੀ ext2fs_ext_attr_ਖੰਡ_rehash(ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *ਸਿਰਲੇਖ,
				  ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *ਸਿਰਾ)
{
	ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *ਇੱਥੇ;
	__u32 hash = 0;

	ਇੱਥੇ = (ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *)(ਸਿਰਲੇਖ+1);
	ਜਦੋਂ (ਇੱਥੇ < ਸਿਰਾ && !EXT2_EXT_IS_ਆਖਰੀ_ਇੰਦਰਾਜ(ਇੱਥੇ)) {
		ਜੇ (!ਇੱਥੇ->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			ਰੋਕੋ;
		}
		hash = (hash << ਖੰਡ_HASH_ਖਿਸਕਾਓ) ^
		       (hash >> (8*ਆਕਾਰ(hash) - ਖੰਡ_HASH_ਖਿਸਕਾਓ)) ^
		       ਇੱਥੇ->e_hash;
		ਇੱਥੇ = EXT2_EXT_ATTR_ਅਗਲਾ(ਇੱਥੇ);
	}
	ਸਿਰਲੇਖ->h_hash = hash;
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ਖੰਡ_HASH_ਖਿਸਕਾਓ

__u32 ext2fs_get_ea_inode_hash(ਬਣਤਰ ext2_inode *inode)
{
	ਵਾਪਸ inode->i_atime;
}

ਖਾਲੀ ext2fs_ਸੈੱਟ_ea_inode_hash(ਬਣਤਰ ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(ਬਣਤਰ ext2_inode *inode)
{
	ਵਾਪਸ ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_ਸੰਸਕਰਣ;
}

ਖਾਲੀ ext2fs_ਸੈੱਟ_ea_inode_ref(ਬਣਤਰ ext2_inode *inode, __u64 ref_ਗਿਣਤੀ)
{
	inode->i_ctime = (__u32)(ref_ਗਿਣਤੀ >> 32);
	inode->osd1.linux1.l_i_ਸੰਸਕਰਣ = (__u32)ref_ਗਿਣਤੀ;
}

ਸਥਿਰ errcode_t check_ext_attr_ਸਿਰਲੇਖ(ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *ਸਿਰਲੇਖ)
{
	ਜੇ ((ਸਿਰਲੇਖ->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     ਸਿਰਲੇਖ->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    ਸਿਰਲੇਖ->h_ਖੰਡਾਂ != 1)
		ਵਾਪਸ EXT2_ET_BAD_EA_ਸਿਰਲੇਖ;

	ਵਾਪਸ 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t ਖੰਡ, ਖਾਲੀ *buf,
				ext2_ino_t inum)
{
	ਪੂਰਨ		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, ਖੰਡ, 1, buf);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਜੇ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_ਖੰਡ_csum_verify(fs, inum, ਖੰਡ, buf))
		csum_failed = 1;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	ext2fs_ਅਦਲਾ_ਬਦਲੀ_ext_attr(buf, buf, fs->blocksize, 1);
#ਜੇ_ਅੰਤ

	retval = check_ext_attr_ਸਿਰਲੇਖ(buf);
	ਜੇ (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	ਵਾਪਸ retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t ਖੰਡ, ਖਾਲੀ *buf)
{
	ਵਾਪਸ ext2fs_read_ext_attr3(fs, ਖੰਡ, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t ਖੰਡ, ਖਾਲੀ *buf)
{
	ਵਾਪਸ ext2fs_read_ext_attr2(fs, ਖੰਡ, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t ਖੰਡ, ਖਾਲੀ *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	ਅੱਖਰ		*write_buf;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ext2fs_ਅਦਲਾ_ਬਦਲੀ_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#ਨਹੀਂਤਾਂ
	write_buf = (ਅੱਖਰ *) inbuf;
#ਜੇ_ਅੰਤ

	retval = ext2fs_ext_attr_ਖੰਡ_csum_ਸੈੱਟ(fs, inum, ਖੰਡ,
			(ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *)write_buf);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	retval = io_channel_write_blk64(fs->io, ਖੰਡ, 1, write_buf);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#ਜੇ_ਅੰਤ
	ਜੇ (!retval)
		ext2fs_mark_ਬਦਲਿਆ(fs);
	ਵਾਪਸ retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t ਖੰਡ, ਖਾਲੀ *inbuf)
{
	ਵਾਪਸ ext2fs_write_ext_attr3(fs, ਖੰਡ, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t ਖੰਡ, ਖਾਲੀ *inbuf)
{
	ਵਾਪਸ ext2fs_write_ext_attr2(fs, ਖੰਡ, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    ਅੱਖਰ *ਖੰਡ_buf, ਪੂਰਨ adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *ਸਿਰਲੇਖ;
	ਅੱਖਰ	*buf = 0;

	ਜੇ ((blk >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super)) ||
	    (blk < fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ))
		ਵਾਪਸ EXT2_ET_BAD_EA_ਖੰਡ_ਅੰਕ;

	ਜੇ (!ਖੰਡ_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		ਖੰਡ_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, ਖੰਡ_buf, inum);
	ਜੇ (retval)
		ਜਾਓ errout;

	ਸਿਰਲੇਖ = (ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *) ਖੰਡ_buf;
	ਸਿਰਲੇਖ->h_refcount += adjust;
	ਜੇ (newcount)
		*newcount = ਸਿਰਲੇਖ->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, ਖੰਡ_buf, inum);
	ਜੇ (retval)
		ਜਾਓ errout;

errout:
	ਜੇ (buf)
		ext2fs_free_mem(&buf);
	ਵਾਪਸ retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    ਅੱਖਰ *ਖੰਡ_buf, ਪੂਰਨ adjust,
				    __u32 *newcount)
{
	ਵਾਪਸ ext2fs_adjust_ea_refcount3(fs, blk, ਖੰਡ_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					ਅੱਖਰ *ਖੰਡ_buf, ਪੂਰਨ adjust,
					__u32 *newcount)
{
	ਵਾਪਸ ext2fs_adjust_ea_refcount2(fs, blk, ਖੰਡ_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
ਬਣਤਰ ext2_xattr {
	ਪੂਰਨ name_ਸੂਚਕਾਂਕ;
	ਅੱਖਰ *name;
	ਅੱਖਰ *short_name;
	ਖਾਲੀ *ਮੁੱਲ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੁੱਲ_len;
	ext2_ino_t ea_ino;
};

ਬਣਤਰ ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	ਬਣਤਰ ext2_xattr *attrs;
	ਪੂਰਨ ਸਮਰੱਥਾ;
	ਪੂਰਨ ਗਿਣਤੀ_2;
	ਪੂਰਨ ibody_ਗਿਣਤੀ;
	ext2_ino_t ino;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags;
};

ਸਥਿਰ errcode_t ext2fs_xattrs_expand(ਬਣਤਰ ext2_xattr_handle *h,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ expandby)
{
	ਬਣਤਰ ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->ਸਮਰੱਥਾ + expandby,
				   ਆਕਾਰ(ਬਣਤਰ ext2_xattr), &new_attrs);
	ਜੇ (err)
		ਵਾਪਸ err;

	ਸਮ੍ਰਿਤੀ_ਨਕਲ(new_attrs, h->attrs, h->ਸਮਰੱਥਾ * ਆਕਾਰ(ਬਣਤਰ ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->ਸਮਰੱਥਾ += expandby;
	h->attrs = new_attrs;

	ਵਾਪਸ 0;
}

ਬਣਤਰ ea_name_ਸੂਚਕਾਂਕ {
	ਪੂਰਨ ਸੂਚਕਾਂਕ;
	ਅਚਲ ਅੱਖਰ *name;
};

/* Keep these names sorted in order of decreasing specificity. */
ਸਥਿਰ ਬਣਤਰ ea_name_ਸੂਚਕਾਂਕ ea_ਨਾਂ[] = {
	{10, "gnu."},
	{3, "system.posix_acl_default"},
	{2, "system.posix_acl_access"},
	{8, "system.richacl"},
	{6, "security."},
	{4, "trusted."},
	{7, "system."},
	{1, "user."},
	{0, ਨਿਰਾ},
};

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਲੱਭੋ_ea_ਅਗੇਤਰ(ਪੂਰਨ ਸੂਚਕਾਂਕ)
{
	ਬਣਤਰ ea_name_ਸੂਚਕਾਂਕ *e;

	ਲਈ (e = ea_ਨਾਂ; e->name; e++)
		ਜੇ (e->ਸੂਚਕਾਂਕ == ਸੂਚਕਾਂਕ)
			ਵਾਪਸ e->name;

	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਪੂਰਨ ਲੱਭੋ_ea_ਸੂਚਕਾਂਕ(ਅਚਲ ਅੱਖਰ *fullname, ਅਚਲ ਅੱਖਰ **name, ਪੂਰਨ *ਸੂਚਕਾਂਕ)
{
	ਬਣਤਰ ea_name_ਸੂਚਕਾਂਕ *e;

	ਲਈ (e = ea_ਨਾਂ; e->name; e++) {
		ਜੇ (ਲੜੀ_ਤੁਲਨਾ(fullname, e->name, ਲੜੀ_ਲੰਬਾਈ(e->name)) == 0) {
			*name = fullname + ਲੜੀ_ਲੰਬਾਈ(e->name);
			*ਸੂਚਕਾਂਕ = e->ਸੂਚਕਾਂਕ;
			ਵਾਪਸ 1;
		}
	}
	ਵਾਪਸ 0;
}

errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       ਬਣਤਰ ext2_inode_large *inode)
{
	ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *ਸਿਰਲੇਖ;
	ਖਾਲੀ *ਖੰਡ_buf = ਨਿਰਾ;
	blk64_t blk;
	errcode_t err;
	ਬਣਤਰ ext2_inode_large i;

	/* Read inode? */
	ਜੇ (inode == ਨਿਰਾ) {
		err = ext2fs_read_inode_ਭਰਿਆ(fs, ino, (ਬਣਤਰ ext2_inode *)&i,
					     ਆਕਾਰ(ਬਣਤਰ ext2_inode_large));
		ਜੇ (err)
			ਵਾਪਸ err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_ਖੰਡ(fs, (ਬਣਤਰ ext2_inode *)inode);
	ਜੇ (blk == 0)
		ਵਾਪਸ 0;

	/* Find block, zero it, write back */
	ਜੇ ((blk < fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ) ||
	    (blk >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super))) {
		err = EXT2_ET_BAD_EA_ਖੰਡ_ਅੰਕ;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	err = ext2fs_get_mem(fs->blocksize, &ਖੰਡ_buf);
	ਜੇ (err)
		ਜਾਓ ਬਾਹਰਲਾ;

	err = ext2fs_read_ext_attr3(fs, blk, ਖੰਡ_buf, ino);
	ਜੇ (err)
		ਜਾਓ out2;

	/* We only know how to deal with v2 EA blocks */
	ਸਿਰਲੇਖ = (ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *) ਖੰਡ_buf;
	ਜੇ (ਸਿਰਲੇਖ->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_ਸਿਰਲੇਖ;
		ਜਾਓ out2;
	}

	ਸਿਰਲੇਖ->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, ਖੰਡ_buf, ino);
	ਜੇ (err)
		ਜਾਓ out2;

	/* Erase link to block */
	ext2fs_file_acl_ਖੰਡ_ਸੈੱਟ(fs, (ਬਣਤਰ ext2_inode *)inode, 0);
	ਜੇ (ਸਿਰਲੇਖ->h_refcount == 0)
		ext2fs_ਖੰਡ_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_ਖੰਡਾਂ(fs, (ਬਣਤਰ ext2_inode *)inode, 1);
	ਜੇ (err)
		ਜਾਓ out2;

	/* Write inode? */
	ਜੇ (inode == &i) {
		err = ext2fs_write_inode_ਭਰਿਆ(fs, ino, (ਬਣਤਰ ext2_inode *)&i,
					      ਆਕਾਰ(ਬਣਤਰ ext2_inode_large));
		ਜੇ (err)
			ਜਾਓ out2;
	}

out2:
	ext2fs_free_mem(&ਖੰਡ_buf);
ਬਾਹਰਲਾ:
	ਵਾਪਸ err;
}

ਸਥਿਰ errcode_t prep_ea_ਖੰਡ_for_write(ext2_filsys fs, ext2_ino_t ino,
					 ਬਣਤਰ ext2_inode_large *inode)
{
	ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *ਸਿਰਲੇਖ;
	ਖਾਲੀ *ਖੰਡ_buf = ਨਿਰਾ;
	blk64_t blk, goal;
	errcode_t err;

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_ਖੰਡ(fs, (ਬਣਤਰ ext2_inode *)inode);
	ਜੇ (blk != 0) {
		ਜੇ ((blk < fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ) ||
		    (blk >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super))) {
			err = EXT2_ET_BAD_EA_ਖੰਡ_ਅੰਕ;
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		err = ext2fs_get_mem(fs->blocksize, &ਖੰਡ_buf);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;

		err = ext2fs_read_ext_attr3(fs, blk, ਖੰਡ_buf, ino);
		ਜੇ (err)
			ਜਾਓ out2;

		/* We only know how to deal with v2 EA blocks */
		ਸਿਰਲੇਖ = (ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *) ਖੰਡ_buf;
		ਜੇ (ਸਿਰਲੇਖ->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_ਸਿਰਲੇਖ;
			ਜਾਓ out2;
		}

		/* Single-user block.  We're done here. */
		ਜੇ (ਸਿਰਲੇਖ->h_refcount == 1)
			ਜਾਓ out2;

		/* We need to CoW the block. */
		ਸਿਰਲੇਖ->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, ਖੰਡ_buf, ino);
		ਜੇ (err)
			ਜਾਓ out2;
	} ਨਹੀਂਤਾਂ {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_ਜੋੜੋ_ਖੰਡਾਂ(fs, (ਬਣਤਰ ext2_inode *)inode,
					     1);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;
	}

	/* Allocate a block */
	goal = ext2fs_ਲੱਭੋ_inode_goal(fs, ino, (ਬਣਤਰ ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, ਨਿਰਾ, &blk);
	ਜੇ (err)
		ਜਾਓ out2;
	ext2fs_file_acl_ਖੰਡ_ਸੈੱਟ(fs, (ਬਣਤਰ ext2_inode *)inode, blk);
out2:
	ਜੇ (ਖੰਡ_buf)
		ext2fs_free_mem(&ਖੰਡ_buf);
ਬਾਹਰਲਾ:
	ਵਾਪਸ err;
}


ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
posix_acl_xattr_ਗਿਣਤੀ(size_t ਮਾਪ)
{
        ਜੇ (ਮਾਪ < ਆਕਾਰ(posix_acl_xattr_ਸਿਰਲੇਖ))
                ਵਾਪਸ -1;
        ਮਾਪ -= ਆਕਾਰ(posix_acl_xattr_ਸਿਰਲੇਖ);
        ਜੇ (ਮਾਪ % ਆਕਾਰ(posix_acl_xattr_ਇੰਦਰਾਜ))
                ਵਾਪਸ -1;
        ਵਾਪਸ ਮਾਪ / ਆਕਾਰ(posix_acl_xattr_ਇੰਦਰਾਜ);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
ਸਥਿਰ errcode_t convert_posix_acl_ਮੰਜ਼ਿਲ_disk_ਬਫਰ(ਅਚਲ ਖਾਲੀ *ਮੁੱਲ, size_t ਮਾਪ,
						  ਖਾਲੀ *ਬਾਹਰਲਾ_buf, size_t *ਮਾਪ_ਬਾਹਰਲਾ)
{
	ਅਚਲ posix_acl_xattr_ਸਿਰਲੇਖ *ਸਿਰਲੇਖ =
		(ਅਚਲ posix_acl_xattr_ਸਿਰਲੇਖ*) ਮੁੱਲ;
	ਅਚਲ posix_acl_xattr_ਇੰਦਰਾਜ *ਸਿਰਾ, *ਇੰਦਰਾਜ =
		(ਅਚਲ posix_acl_xattr_ਇੰਦਰਾਜ *)(ਸਿਰਲੇਖ+1);
	ext4_acl_ਸਿਰਲੇਖ *ext_acl;
	size_t s;
	ਅੱਖਰ *e;

	ਪੂਰਨ ਗਿਣਤੀ_2;

	ਜੇ (!ਮੁੱਲ)
		ਵਾਪਸ EINVAL;
	ਜੇ (ਮਾਪ < ਆਕਾਰ(posix_acl_xattr_ਸਿਰਲੇਖ))
		ਵਾਪਸ ENOMEM;
	ਜੇ (ਸਿਰਲੇਖ->a_ਸੰਸਕਰਣ != ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(POSIX_ACL_XATTR_ਸੰਸਕਰਣ))
		ਵਾਪਸ EINVAL;

	ਗਿਣਤੀ_2 = posix_acl_xattr_ਗਿਣਤੀ(ਮਾਪ);
	ext_acl = ਬਾਹਰਲਾ_buf;
	ext_acl->a_ਸੰਸਕਰਣ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(EXT4_ACL_ਸੰਸਕਰਣ);

	ਜੇ (ਗਿਣਤੀ_2 <= 0)
		ਵਾਪਸ EINVAL;

	e = (ਅੱਖਰ *) ਬਾਹਰਲਾ_buf + ਆਕਾਰ(ext4_acl_ਸਿਰਲੇਖ);
	s = ਆਕਾਰ(ext4_acl_ਸਿਰਲੇਖ);
	ਲਈ (ਸਿਰਾ = ਇੰਦਰਾਜ + ਗਿਣਤੀ_2; ਇੰਦਰਾਜ != ਸਿਰਾ;ਇੰਦਰਾਜ++) {
		ext4_acl_ਇੰਦਰਾਜ *disk_ਇੰਦਰਾਜ = (ext4_acl_ਇੰਦਰਾਜ*) e;
		disk_ਇੰਦਰਾਜ->e_ਟੈਗ = ਇੰਦਰਾਜ->e_ਟੈਗ;
		disk_ਇੰਦਰਾਜ->e_perm = ਇੰਦਰਾਜ->e_perm;

		ਚੋਣ(ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(ਇੰਦਰਾਜ->e_ਟੈਗ)) {
			ਹਾਲਤ ACL_USER_OBJ:
			ਹਾਲਤ ACL_ਸਮੂਹ_OBJ:
			ਹਾਲਤ ACL_ਨਕਾਬ:
			ਹਾਲਤ ACL_OTHER:
				e += ਆਕਾਰ(ext4_acl_ਇੰਦਰਾਜ_short);
				s += ਆਕਾਰ(ext4_acl_ਇੰਦਰਾਜ_short);
				ਰੋਕੋ;
			ਹਾਲਤ ACL_USER:
			ਹਾਲਤ ACL_ਸਮੂਹ:
				disk_ਇੰਦਰਾਜ->e_id = ਇੰਦਰਾਜ->e_id;
				e += ਆਕਾਰ(ext4_acl_ਇੰਦਰਾਜ);
				s += ਆਕਾਰ(ext4_acl_ਇੰਦਰਾਜ);
				ਰੋਕੋ;
			ਮੂਲ:
				ਵਾਪਸ EINVAL;
		}
	}
	*ਮਾਪ_ਬਾਹਰਲਾ = s;
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t convert_disk_ਬਫਰ_ਮੰਜ਼ਿਲ_posix_acl(ਅਚਲ ਖਾਲੀ *ਮੁੱਲ, size_t ਮਾਪ,
						  ਖਾਲੀ **ਬਾਹਰਲਾ_buf, size_t *ਮਾਪ_ਬਾਹਰਲਾ)
{
	posix_acl_xattr_ਸਿਰਲੇਖ *ਸਿਰਲੇਖ;
	posix_acl_xattr_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;
	ਅਚਲ ext4_acl_ਸਿਰਲੇਖ *ext_acl = (ਅਚਲ ext4_acl_ਸਿਰਲੇਖ *) ਮੁੱਲ;
	errcode_t err;
	ਅਚਲ ਅੱਖਰ *cp;
	ਅੱਖਰ *ਬਾਹਰਲਾ;

	ਜੇ ((!ਮੁੱਲ) ||
	    (ਮਾਪ < ਆਕਾਰ(ext4_acl_ਸਿਰਲੇਖ)) ||
	    (ext_acl->a_ਸੰਸਕਰਣ != ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(EXT4_ACL_ਸੰਸਕਰਣ)))
		ਵਾਪਸ EINVAL;

	err = ext2fs_get_mem(ਮਾਪ * 2, &ਬਾਹਰਲਾ);
	ਜੇ (err)
		ਵਾਪਸ err;

	ਸਿਰਲੇਖ = (posix_acl_xattr_ਸਿਰਲੇਖ *) ਬਾਹਰਲਾ;
	ਸਿਰਲੇਖ->a_ਸੰਸਕਰਣ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(POSIX_ACL_XATTR_ਸੰਸਕਰਣ);
	ਇੰਦਰਾਜ = (posix_acl_xattr_ਇੰਦਰਾਜ *) (ਬਾਹਰਲਾ + ਆਕਾਰ(posix_acl_xattr_ਸਿਰਲੇਖ));

	cp = (ਅਚਲ ਅੱਖਰ *) ਮੁੱਲ + ਆਕਾਰ(ext4_acl_ਸਿਰਲੇਖ);
	ਮਾਪ -= ਆਕਾਰ(ext4_acl_ਸਿਰਲੇਖ);

	ਜਦੋਂ (ਮਾਪ > 0) {
		ਅਚਲ ext4_acl_ਇੰਦਰਾਜ *disk_ਇੰਦਰਾਜ = (ਅਚਲ ext4_acl_ਇੰਦਰਾਜ *) cp;

		ਇੰਦਰਾਜ->e_ਟੈਗ = disk_ਇੰਦਰਾਜ->e_ਟੈਗ;
		ਇੰਦਰਾਜ->e_perm = disk_ਇੰਦਰਾਜ->e_perm;

		ਚੋਣ(ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(ਇੰਦਰਾਜ->e_ਟੈਗ)) {
			ਹਾਲਤ ACL_USER_OBJ:
			ਹਾਲਤ ACL_ਸਮੂਹ_OBJ:
			ਹਾਲਤ ACL_ਨਕਾਬ:
			ਹਾਲਤ ACL_OTHER:
				ਇੰਦਰਾਜ->e_id = 0;
				cp += ਆਕਾਰ(ext4_acl_ਇੰਦਰਾਜ_short);
				ਮਾਪ -= ਆਕਾਰ(ext4_acl_ਇੰਦਰਾਜ_short);
				ਰੋਕੋ;
			ਹਾਲਤ ACL_USER:
			ਹਾਲਤ ACL_ਸਮੂਹ:
				ਇੰਦਰਾਜ->e_id = disk_ਇੰਦਰਾਜ->e_id;
				cp += ਆਕਾਰ(ext4_acl_ਇੰਦਰਾਜ);
				ਮਾਪ -= ਆਕਾਰ(ext4_acl_ਇੰਦਰਾਜ);
				ਰੋਕੋ;
			ਮੂਲ:
				ext2fs_free_mem(&ਬਾਹਰਲਾ);
				ਵਾਪਸ EINVAL;
		}
		ਇੰਦਰਾਜ++;
	}
	*ਬਾਹਰਲਾ_buf = ਬਾਹਰਲਾ;
	*ਮਾਪ_ਬਾਹਰਲਾ = ((ਅੱਖਰ *) ਇੰਦਰਾਜ - ਬਾਹਰਲਾ);
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t
write_xattrs_ਮੰਜ਼ਿਲ_ਬਫਰ(ext2_filsys fs, ਬਣਤਰ ext2_xattr *attrs, ਪੂਰਨ ਗਿਣਤੀ_2,
		       ਖਾਲੀ *entries_ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ storage_ਮਾਪ,
		       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੁੱਲ_ਵਿੱਥ_correction, ਪੂਰਨ write_hash)
{
	ਬਣਤਰ ext2_xattr *x;
	ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *e = entries_ਸ਼ੁਰੂ;
	ਅੱਖਰ *ਸਿਰਾ = (ਅੱਖਰ *) entries_ਸ਼ੁਰੂ + storage_ਮਾਪ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੁੱਲ_ਮਾਪ;
	errcode_t err;

	memset(entries_ਸ਼ੁਰੂ, 0, storage_ਮਾਪ);
	ਲਈ (x = attrs; x < attrs + ਗਿਣਤੀ_2; x++) {
		ਮੁੱਲ_ਮਾਪ = ((x->ਮੁੱਲ_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = ਲੜੀ_ਲੰਬਾਈ(x->short_name);
		e->e_name_ਸੂਚਕਾਂਕ = x->name_ਸੂਚਕਾਂਕ;

		e->e_ਮੁੱਲ_ਮਾਪ = x->ਮੁੱਲ_len;
		e->e_ਮੁੱਲ_inum = x->ea_ino;

		/* Store name */
		ਸਮ੍ਰਿਤੀ_ਨਕਲ((ਅੱਖਰ *)e + ਆਕਾਰ(*e), x->short_name, e->e_name_len);
		ਜੇ (x->ea_ino) {
			e->e_ਮੁੱਲ_offs = 0;
		} ਨਹੀਂਤਾਂ {
			ਸਿਰਾ -= ਮੁੱਲ_ਮਾਪ;
			e->e_ਮੁੱਲ_offs = ਸਿਰਾ - (ਅੱਖਰ *) entries_ਸ਼ੁਰੂ +
						ਮੁੱਲ_ਵਿੱਥ_correction;
			ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਸਿਰਾ, x->ਮੁੱਲ, e->e_ਮੁੱਲ_ਮਾਪ);
		}

		ਜੇ (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : ਸਿਰਾ,
							  &e->e_hash);
			ਜੇ (err)
				ਵਾਪਸ err;
		} ਨਹੀਂਤਾਂ
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_ਅਗਲਾ(e);
		*(__u32 *)e = 0;
	}
	ਵਾਪਸ 0;
}

errcode_t ext2fs_xattrs_write(ਬਣਤਰ ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ inode_ਮਾਪ = EXT2_INODE_ਮਾਪ(fs->super);
	ਬਣਤਰ ext2_inode_large *inode;
	ਅੱਖਰ *ਸ਼ੁਰੂ, *ਖੰਡ_buf = ਨਿਰਾ;
	ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *ਸਿਰਲੇਖ;
	__u32 ea_inode_magic;
	blk64_t blk;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ storage_ਮਾਪ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_ਮਾਪ;
	ਜੇ (i < ਆਕਾਰ(*inode))
		i = ਆਕਾਰ(*inode);
	err = ext2fs_get_memzero(i, &inode);
	ਜੇ (err)
		ਵਾਪਸ err;

	err = ext2fs_read_inode_ਭਰਿਆ(fs, handle->ino, EXT2_INODE(inode),
				     inode_ਮਾਪ);
	ਜੇ (err)
		ਜਾਓ ਬਾਹਰਲਾ;

	/* If extra_isize isn't set, we need to set it now */
	ਜੇ (inode->i_extra_isize == 0 &&
	    inode_ਮਾਪ > EXT2_GOOD_OLD_INODE_ਮਾਪ) {
		ਅੱਖਰ *p = (ਅੱਖਰ *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		ਜੇ (extra == 0)
			extra = ਆਕਾਰ(__u32);
		memset(p + EXT2_GOOD_OLD_INODE_ਮਾਪ, 0, extra);
		inode->i_extra_isize = extra;
	}
	ਜੇ (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/* Does the inode have space for EA? */
	ਜੇ (inode->i_extra_isize < ਆਕਾਰ(inode->i_extra_isize) ||
	    inode_ਮਾਪ <= EXT2_GOOD_OLD_INODE_ਮਾਪ + inode->i_extra_isize +
								ਆਕਾਰ(__u32))
		ਜਾਓ write_ea_ਖੰਡ;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(((ਅੱਖਰ *) inode) + EXT2_GOOD_OLD_INODE_ਮਾਪ +
	       inode->i_extra_isize, &ea_inode_magic, ਆਕਾਰ(__u32));
	storage_ਮਾਪ = inode_ਮਾਪ - EXT2_GOOD_OLD_INODE_ਮਾਪ -
				inode->i_extra_isize - ਆਕਾਰ(__u32);
	ਸ਼ੁਰੂ = ((ਅੱਖਰ *) inode) + EXT2_GOOD_OLD_INODE_ਮਾਪ +
				inode->i_extra_isize + ਆਕਾਰ(__u32);

	err = write_xattrs_ਮੰਜ਼ਿਲ_ਬਫਰ(fs, handle->attrs, handle->ibody_ਗਿਣਤੀ,
				     ਸ਼ੁਰੂ, storage_ਮਾਪ, 0, 0);
	ਜੇ (err)
		ਜਾਓ ਬਾਹਰਲਾ;
write_ea_ਖੰਡ:
	/* Are we done? */
	ਜੇ (handle->ibody_ਗਿਣਤੀ == handle->ਗਿਣਤੀ_2 &&
	    !ext2fs_file_acl_ਖੰਡ(fs, EXT2_INODE(inode)))
		ਜਾਓ skip_ea_ਖੰਡ;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &ਖੰਡ_buf);
	ਜੇ (err)
		ਜਾਓ ਬਾਹਰਲਾ;

	storage_ਮਾਪ = fs->blocksize - ਆਕਾਰ(ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ);
	ਸ਼ੁਰੂ = ਖੰਡ_buf + ਆਕਾਰ(ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ);

	err = write_xattrs_ਮੰਜ਼ਿਲ_ਬਫਰ(fs, handle->attrs + handle->ibody_ਗਿਣਤੀ,
				     handle->ਗਿਣਤੀ_2 - handle->ibody_ਗਿਣਤੀ, ਸ਼ੁਰੂ,
				     storage_ਮਾਪ, ਸ਼ੁਰੂ - ਖੰਡ_buf, 1);
	ਜੇ (err)
		ਜਾਓ out2;

	/* Write a header on the EA block */
	ਸਿਰਲੇਖ = (ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *) ਖੰਡ_buf;
	ਸਿਰਲੇਖ->h_magic = EXT2_EXT_ATTR_MAGIC;
	ਸਿਰਲੇਖ->h_refcount = 1;
	ਸਿਰਲੇਖ->h_ਖੰਡਾਂ = 1;

	/* Get a new block for writing */
	err = prep_ea_ਖੰਡ_for_write(fs, handle->ino, inode);
	ਜੇ (err)
		ਜਾਓ out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_ਖੰਡ(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, ਖੰਡ_buf, handle->ino);
	ਜੇ (err)
		ਜਾਓ out2;

skip_ea_ਖੰਡ:
	blk = ext2fs_file_acl_ਖੰਡ(fs, (ਬਣਤਰ ext2_inode *)inode);
	ਜੇ (!ਖੰਡ_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;
	}

	/* Write the inode */
	err = ext2fs_write_inode_ਭਰਿਆ(fs, handle->ino, EXT2_INODE(inode),
				      inode_ਮਾਪ);
	ਜੇ (err)
		ਜਾਓ out2;

out2:
	ext2fs_free_mem(&ਖੰਡ_buf);
ਬਾਹਰਲਾ:
	ext2fs_free_mem(&inode);
	ਵਾਪਸ err;
}

ਸਥਿਰ errcode_t read_xattrs_from_ਬਫਰ(ਬਣਤਰ ext2_xattr_handle *handle,
					 ਬਣਤਰ ext2_inode_large *inode,
					 ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *entries,
					 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ storage_ਮਾਪ,
					 ਅੱਖਰ *ਮੁੱਲ_ਸ਼ੁਰੂ)
{
	ਬਣਤਰ ext2_xattr *x;
	ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ, *ਸਿਰਾ;
	ਅਚਲ ਅੱਖਰ *ਅਗੇਤਰ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ remain, ਅਗੇਤਰ_len;
	errcode_t err;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੁੱਲਾਂ_ਮਾਪ = storage_ਮਾਪ +
			((ਅੱਖਰ *)entries - ਮੁੱਲ_ਸ਼ੁਰੂ);

	/* find the end */
	ਸਿਰਾ = entries;
	remain = storage_ਮਾਪ;
	ਜਦੋਂ (remain >= ਆਕਾਰ(ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ) &&
	       !EXT2_EXT_IS_ਆਖਰੀ_ਇੰਦਰਾਜ(ਸਿਰਾ)) {

		/* header eats this space */
		remain -= ਆਕਾਰ(ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ);

		/* is attribute name valid? */
		ਜੇ (EXT2_EXT_ATTR_ਮਾਪ(ਸਿਰਾ->e_name_len) > remain)
			ਵਾਪਸ EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_ਮਾਪ(ਸਿਰਾ->e_name_len);
		ਸਿਰਾ = EXT2_EXT_ATTR_ਅਗਲਾ(ਸਿਰਾ);
	}

	ਇੰਦਰਾਜ = entries;
	remain = storage_ਮਾਪ;
	ਜਦੋਂ (remain >= ਆਕਾਰ(ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ) &&
	       !EXT2_EXT_IS_ਆਖਰੀ_ਇੰਦਰਾਜ(ਇੰਦਰਾਜ)) {

		/* Allocate space for more attrs? */
		ਜੇ (handle->ਗਿਣਤੀ_2 == handle->ਸਮਰੱਥਾ) {
			err = ext2fs_xattrs_expand(handle, 4);
			ਜੇ (err)
				ਵਾਪਸ err;
		}

		x = handle->attrs + handle->ਗਿਣਤੀ_2;

		/* header eats this space */
		remain -= ਆਕਾਰ(ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_ਮਾਪ(ਇੰਦਰਾਜ->e_name_len);

		/* Extract name */
		ਅਗੇਤਰ = ਲੱਭੋ_ea_ਅਗੇਤਰ(ਇੰਦਰਾਜ->e_name_ਸੂਚਕਾਂਕ);
		ਅਗੇਤਰ_len = (ਅਗੇਤਰ ? ਲੜੀ_ਲੰਬਾਈ(ਅਗੇਤਰ) : 0);
		err = ext2fs_get_memzero(ਇੰਦਰਾਜ->e_name_len + ਅਗੇਤਰ_len + 1,
					 &x->name);
		ਜੇ (err)
			ਵਾਪਸ err;
		ਜੇ (ਅਗੇਤਰ)
			ਸਮ੍ਰਿਤੀ_ਨਕਲ(x->name, ਅਗੇਤਰ, ਅਗੇਤਰ_len);
		ਜੇ (ਇੰਦਰਾਜ->e_name_len)
			ਸਮ੍ਰਿਤੀ_ਨਕਲ(x->name + ਅਗੇਤਰ_len,
			       (ਅੱਖਰ *)ਇੰਦਰਾਜ + ਆਕਾਰ(*ਇੰਦਰਾਜ),
			       ਇੰਦਰਾਜ->e_name_len);
		x->short_name = x->name + ਅਗੇਤਰ_len;
		x->name_ਸੂਚਕਾਂਕ = ਇੰਦਰਾਜ->e_name_ਸੂਚਕਾਂਕ;

		/* Check & copy value */
		ਜੇ (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    ਇੰਦਰਾਜ->e_ਮੁੱਲ_inum != 0)
			ਵਾਪਸ EXT2_ET_BAD_EA_ਖੰਡ_ਅੰਕ;

		ਜੇ (ਇੰਦਰਾਜ->e_ਮੁੱਲ_inum == 0) {
			ਜੇ (ਇੰਦਰਾਜ->e_ਮੁੱਲ_ਮਾਪ > remain)
				ਵਾਪਸ EXT2_ET_EA_BAD_ਮੁੱਲ_ਮਾਪ;

			ਜੇ (ਇੰਦਰਾਜ->e_ਮੁੱਲ_offs + ਇੰਦਰਾਜ->e_ਮੁੱਲ_ਮਾਪ > ਮੁੱਲਾਂ_ਮਾਪ)
				ਵਾਪਸ EXT2_ET_EA_BAD_ਮੁੱਲ_ਵਿੱਥ;

			ਜੇ (ਇੰਦਰਾਜ->e_ਮੁੱਲ_ਮਾਪ > 0 &&
			    ਮੁੱਲ_ਸ਼ੁਰੂ + ਇੰਦਰਾਜ->e_ਮੁੱਲ_offs <
			    (ਅੱਖਰ *)ਸਿਰਾ + ਆਕਾਰ(__u32))
				ਵਾਪਸ EXT2_ET_EA_BAD_ਮੁੱਲ_ਵਿੱਥ;

			remain -= ਇੰਦਰਾਜ->e_ਮੁੱਲ_ਮਾਪ;

			err = ext2fs_get_mem(ਇੰਦਰਾਜ->e_ਮੁੱਲ_ਮਾਪ, &x->ਮੁੱਲ);
			ਜੇ (err)
				ਵਾਪਸ err;
			ਸਮ੍ਰਿਤੀ_ਨਕਲ(x->ਮੁੱਲ, ਮੁੱਲ_ਸ਼ੁਰੂ + ਇੰਦਰਾਜ->e_ਮੁੱਲ_offs,
			       ਇੰਦਰਾਜ->e_ਮੁੱਲ_ਮਾਪ);
		} ਨਹੀਂਤਾਂ {
			ਬਣਤਰ ext2_inode *ea_inode;
			ext2_file_t ea_file;

			ਜੇ (ਇੰਦਰਾਜ->e_ਮੁੱਲ_offs != 0)
				ਵਾਪਸ EXT2_ET_EA_BAD_ਮੁੱਲ_ਵਿੱਥ;

			ਜੇ (ਇੰਦਰਾਜ->e_ਮੁੱਲ_ਮਾਪ > (64 * 1024))
				ਵਾਪਸ EXT2_ET_EA_BAD_ਮੁੱਲ_ਮਾਪ;

			err = ext2fs_get_mem(ਇੰਦਰਾਜ->e_ਮੁੱਲ_ਮਾਪ, &x->ਮੁੱਲ);
			ਜੇ (err)
				ਵਾਪਸ err;

			err = ext2fs_file_open(handle->fs, ਇੰਦਰਾਜ->e_ਮੁੱਲ_inum,
					       0, &ea_file);
			ਜੇ (err)
				ਵਾਪਸ err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			ਜੇ ((ea_inode->i_flags & EXT4_INLINE_ਅੰਕੜੇ_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_ਗਿਣਤੀ == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			ਨਹੀਂਤਾਂ ਜੇ ((__u64) ext2fs_file_get_ਮਾਪ(ea_file) !=
				 ਇੰਦਰਾਜ->e_ਮੁੱਲ_ਮਾਪ)
				err = EXT2_ET_EA_BAD_ਮੁੱਲ_ਮਾਪ;
			ਨਹੀਂਤਾਂ
				err = ext2fs_file_read(ea_file, x->ਮੁੱਲ,
						       ਇੰਦਰਾਜ->e_ਮੁੱਲ_ਮਾਪ, 0);
			ext2fs_file_close(ea_file);
			ਜੇ (err)
				ਵਾਪਸ err;
		}

		x->ea_ino = ਇੰਦਰਾਜ->e_ਮੁੱਲ_inum;
		x->ਮੁੱਲ_len = ਇੰਦਰਾਜ->e_ਮੁੱਲ_ਮਾਪ;

		/* e_hash may be 0 in older inode's ea */
		ਜੇ (ਇੰਦਰਾਜ->e_hash != 0) {
			__u32 hash, signed_hash;

			ਖਾਲੀ *ਅੰਕੜੇ = (ਇੰਦਰਾਜ->e_ਮੁੱਲ_inum != 0) ?
					0 : ਮੁੱਲ_ਸ਼ੁਰੂ + ਇੰਦਰਾਜ->e_ਮੁੱਲ_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, ਇੰਦਰਾਜ,
							  ਅੰਕੜੇ, &hash,
							  &signed_hash);
			ਜੇ (err)
				ਵਾਪਸ err;
			ਜੇ ((ਇੰਦਰਾਜ->e_hash != hash) &&
			    (ਇੰਦਰਾਜ->e_hash != signed_hash)) {
				ਬਣਤਰ ext2_inode ਬੱਚਾ;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							ਇੰਦਰਾਜ->e_ਮੁੱਲ_inum,
							&ਬੱਚਾ);
				ਜੇ (err)
					ਵਾਪਸ err;
				ਜੇ (ਬੱਚਾ.i_mtime != handle->ino ||
				    ਬੱਚਾ.i_generation != inode->i_generation)
					ਵਾਪਸ EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->ਗਿਣਤੀ_2++;
		ਇੰਦਰਾਜ = EXT2_EXT_ATTR_ਅਗਲਾ(ਇੰਦਰਾਜ);
	}

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ xattrs_free_ਕੁੰਜੀਆਂ(ਬਣਤਰ ext2_xattr_handle *h)
{
	ਬਣਤਰ ext2_xattr *a = h->attrs;
	ਪੂਰਨ i;

	ਲਈ (i = 0; i < h->ਸਮਰੱਥਾ; i++) {
		ਜੇ (a[i].name)
			ext2fs_free_mem(&a[i].name);
		ਜੇ (a[i].ਮੁੱਲ)
			ext2fs_free_mem(&a[i].ਮੁੱਲ);
	}
	h->ਗਿਣਤੀ_2 = 0;
	h->ibody_ਗਿਣਤੀ = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(ਬਣਤਰ ext2_xattr_handle *handle,
				   ਬਣਤਰ ext2_inode_large *inode)
{
	ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *ਸਿਰਲੇਖ;
	__u32 ea_inode_magic;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ storage_ਮਾਪ;
	ਅੱਖਰ *ਸ਼ੁਰੂ, *ਖੰਡ_buf = ਨਿਰਾ;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_ਕੁੰਜੀਆਂ(handle);

	/* Does the inode have space for EA? */
	ਜੇ (inode->i_extra_isize < ਆਕਾਰ(inode->i_extra_isize) ||
	    EXT2_INODE_ਮਾਪ(handle->fs->super) <= EXT2_GOOD_OLD_INODE_ਮਾਪ +
						  inode->i_extra_isize +
						  ਆਕਾਰ(__u32))
		ਜਾਓ read_ea_ਖੰਡ;
	ਜੇ (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/* Look for EA in the inode */
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(&ea_inode_magic, ((ਅੱਖਰ *) inode) + EXT2_GOOD_OLD_INODE_ਮਾਪ +
	       inode->i_extra_isize, ਆਕਾਰ(__u32));
	ਜੇ (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_ਮਾਪ = EXT2_INODE_ਮਾਪ(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_ਮਾਪ - inode->i_extra_isize -
			ਆਕਾਰ(__u32);
		ਸ਼ੁਰੂ = ((ਅੱਖਰ *) inode) + EXT2_GOOD_OLD_INODE_ਮਾਪ +
			inode->i_extra_isize + ਆਕਾਰ(__u32);

		err = read_xattrs_from_ਬਫਰ(handle, inode,
					(ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *) ਸ਼ੁਰੂ,
					storage_ਮਾਪ, ਸ਼ੁਰੂ);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;

		handle->ibody_ਗਿਣਤੀ = handle->ਗਿਣਤੀ_2;
	}

read_ea_ਖੰਡ:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_ਖੰਡ(handle->fs, EXT2_INODE(inode));
	ਜੇ (blk != 0) {
		ਜੇ ((blk < handle->fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ) ||
		    (blk >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_ਖੰਡ_ਅੰਕ;
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &ਖੰਡ_buf);
		ਜੇ (err)
			ਜਾਓ ਬਾਹਰਲਾ;

		err = ext2fs_read_ext_attr3(handle->fs, blk, ਖੰਡ_buf,
					    handle->ino);
		ਜੇ (err)
			ਜਾਓ out3;

		/* We only know how to deal with v2 EA blocks */
		ਸਿਰਲੇਖ = (ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ *) ਖੰਡ_buf;
		ਜੇ (ਸਿਰਲੇਖ->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_ਸਿਰਲੇਖ;
			ਜਾਓ out3;
		}

		/* Read EAs */
		storage_ਮਾਪ = handle->fs->blocksize -
			ਆਕਾਰ(ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ);
		ਸ਼ੁਰੂ = ਖੰਡ_buf + ਆਕਾਰ(ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ);
		err = read_xattrs_from_ਬਫਰ(handle, inode,
					(ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *) ਸ਼ੁਰੂ,
					storage_ਮਾਪ, ਖੰਡ_buf);
	}

out3:
	ਜੇ (ਖੰਡ_buf)
		ext2fs_free_mem(&ਖੰਡ_buf);
ਬਾਹਰਲਾ:
	ਵਾਪਸ err;
}

errcode_t ext2fs_xattrs_read(ਬਣਤਰ ext2_xattr_handle *handle)
{
	ਬਣਤਰ ext2_inode_large *inode;
	size_t inode_ਮਾਪ = EXT2_INODE_ਮਾਪ(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	ਜੇ (inode_ਮਾਪ < ਆਕਾਰ(*inode))
		inode_ਮਾਪ = ਆਕਾਰ(*inode);
	err = ext2fs_get_memzero(inode_ਮਾਪ, &inode);
	ਜੇ (err)
		ਵਾਪਸ err;

	err = ext2fs_read_inode_ਭਰਿਆ(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_ਮਾਪ(handle->fs->super));
	ਜੇ (err)
		ਜਾਓ ਬਾਹਰਲਾ;

	err = ext2fs_xattrs_read_inode(handle, inode);

ਬਾਹਰਲਾ:
	ext2fs_free_mem(&inode);

	ਵਾਪਸ err;
}

errcode_t ext2fs_xattrs_iterate(ਬਣਤਰ ext2_xattr_handle *h,
				ਪੂਰਨ (*func)(ਅੱਖਰ *name, ਅੱਖਰ *ਮੁੱਲ,
					    size_t ਮੁੱਲ_len, ਖਾਲੀ *ਅੰਕੜੇ),
				ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ ext2_xattr *x;
	ਪੂਰਨ dirty = 0;
	ਪੂਰਨ ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	ਲਈ (x = h->attrs; x < h->attrs + h->ਗਿਣਤੀ_2; x++) {
		ret = func(x->name, x->ਮੁੱਲ, x->ਮੁੱਲ_len, ਅੰਕੜੇ);
		ਜੇ (ret & XATTR_ਬਦਲਿਆ)
			dirty = 1;
		ਜੇ (ret & XATTR_ABORT)
			ਰੋਕੋ;
	}

	ਜੇ (dirty)
		ਵਾਪਸ ext2fs_xattrs_write(h);
	ਵਾਪਸ 0;
}

errcode_t ext2fs_xattr_get(ਬਣਤਰ ext2_xattr_handle *h, ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ,
			   ਖਾਲੀ **ਮੁੱਲ, size_t *ਮੁੱਲ_len)
{
	ਬਣਤਰ ext2_xattr *x;
	ਅੱਖਰ *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	ਲਈ (x = h->attrs; x < h->attrs + h->ਗਿਣਤੀ_2; x++) {
		ਜੇ (strcmp(x->name, ਕੁੰਜੀ))
			ਜਾਰੀ;

		ਜੇ (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(ਕੁੰਜੀ, "system.posix_acl_default") == 0) ||
		     (strcmp(ਕੁੰਜੀ, "system.posix_acl_access") == 0))) {
			err = convert_disk_ਬਫਰ_ਮੰਜ਼ਿਲ_posix_acl(x->ਮੁੱਲ, x->ਮੁੱਲ_len,
							       ਮੁੱਲ, ਮੁੱਲ_len);
			ਵਾਪਸ err;
		} ਨਹੀਂਤਾਂ {
			err = ext2fs_get_mem(x->ਮੁੱਲ_len, &val);
			ਜੇ (err)
				ਵਾਪਸ err;
			ਸਮ੍ਰਿਤੀ_ਨਕਲ(val, x->ਮੁੱਲ, x->ਮੁੱਲ_len);
			*ਮੁੱਲ = val;
			*ਮੁੱਲ_len = x->ਮੁੱਲ_len;
			ਵਾਪਸ 0;
		}
	}

	ਵਾਪਸ EXT2_ET_EA_ਕੁੰਜੀ_NOT_ਮਿਲਿਆ;
}

errcode_t ext2fs_xattr_inode_max_ਮਾਪ(ext2_filsys fs, ext2_ino_t ino,
				      size_t *ਮਾਪ)
{
	ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ;
	ਬਣਤਰ ext2_inode_large *inode;
	__u32 ea_inode_magic;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ minoff;
	ਅੱਖਰ *ਸ਼ੁਰੂ;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_ਮਾਪ(fs->super);
	ਜੇ (i < ਆਕਾਰ(*inode))
		i = ਆਕਾਰ(*inode);
	err = ext2fs_get_memzero(i, &inode);
	ਜੇ (err)
		ਵਾਪਸ err;

	err = ext2fs_read_inode_ਭਰਿਆ(fs, ino, (ਬਣਤਰ ext2_inode *)inode,
				     EXT2_INODE_ਮਾਪ(fs->super));
	ਜੇ (err)
		ਜਾਓ ਬਾਹਰਲਾ;

	/* Does the inode have size for EA? */
	ਜੇ (EXT2_INODE_ਮਾਪ(fs->super) <= EXT2_GOOD_OLD_INODE_ਮਾਪ +
						  inode->i_extra_isize +
						  ਆਕਾਰ(__u32)) {
		err = EXT2_ET_INLINE_ਅੰਕੜੇ_NO_SPACE;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	minoff = EXT2_INODE_ਮਾਪ(fs->super) - ਆਕਾਰ(*inode) - ਆਕਾਰ(__u32);
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(&ea_inode_magic, ((ਅੱਖਰ *) inode) + EXT2_GOOD_OLD_INODE_ਮਾਪ +
	       inode->i_extra_isize, ਆਕਾਰ(__u32));
	ਜੇ (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		ਸ਼ੁਰੂ= ((ਅੱਖਰ *) inode) + EXT2_GOOD_OLD_INODE_ਮਾਪ +
			inode->i_extra_isize + ਆਕਾਰ(__u32);
		ਇੰਦਰਾਜ = (ਬਣਤਰ ext2_ext_attr_ਇੰਦਰਾਜ *) ਸ਼ੁਰੂ;
		ਜਦੋਂ (!EXT2_EXT_IS_ਆਖਰੀ_ਇੰਦਰਾਜ(ਇੰਦਰਾਜ)) {
			ਜੇ (!ਇੰਦਰਾਜ->e_ਮੁੱਲ_inum && ਇੰਦਰਾਜ->e_ਮੁੱਲ_ਮਾਪ) {
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ offs = ਇੰਦਰਾਜ->e_ਮੁੱਲ_offs;
				ਜੇ (offs < minoff)
					minoff = offs;
			}
			ਇੰਦਰਾਜ = EXT2_EXT_ATTR_ਅਗਲਾ(ਇੰਦਰਾਜ);
		}
		*ਮਾਪ = minoff - ((ਅੱਖਰ *)ਇੰਦਰਾਜ - (ਅੱਖਰ *)ਸ਼ੁਰੂ) - ਆਕਾਰ(__u32);
	} ਨਹੀਂਤਾਂ {
		/* no xattr.  return a maximum size */
		*ਮਾਪ = EXT2_EXT_ATTR_ਮਾਪ(minoff -
					   EXT2_EXT_ATTR_LEN(ਲੜੀ_ਲੰਬਾਈ("data")) -
					   EXT2_EXT_ATTR_ਗੇੜਾ - ਆਕਾਰ(__u32));
	}

ਬਾਹਰਲਾ:
	ext2fs_free_mem(&inode);
	ਵਾਪਸ err;
}

ਸਥਿਰ errcode_t xattr_create_ea_inode(ext2_filsys fs, ਅਚਲ ਖਾਲੀ *ਮੁੱਲ,
				       size_t ਮੁੱਲ_len, ext2_ino_t *ea_ino)
{
	ਬਣਤਰ ext2_inode inode;
	ext2_ino_t ino;
	ext2_file_t file;
	__u32 hash;
	errcode_t ret;

	ret = ext2fs_new_inode(fs, 0, 0, 0, &ino);
	ਜੇ (ret)
		ਵਾਪਸ ret;

	memset(&inode, 0, ਆਕਾਰ(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	ਜੇ (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_ਮਾਪ = 0;
	inode.i_ਬਹੁਲਕ = LINUX_S_IFREG | 0600;
	inode.i_links_ਗਿਣਤੀ = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	ਜੇ (ret)
		ਵਾਪਸ ret;
	/*
	 * ref_count and hash utilize inode's i_*time fields.
	 * ext2fs_write_new_inode() call above initializes these fields with
	 * current time. That's why ref count and hash updates are done
	 * separately below.
	 */
	ext2fs_ਸੈੱਟ_ea_inode_ref(&inode, 1);
	hash = ext2fs_crc32c_le(fs->csum_ਬੀਜ, ਮੁੱਲ, ਮੁੱਲ_len);
	ext2fs_ਸੈੱਟ_ea_inode_hash(&inode, hash);

	ret = ext2fs_write_inode(fs, ino, &inode);
	ਜੇ (ret)
		ਵਾਪਸ ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	ਜੇ (ret)
		ਵਾਪਸ ret;
	ret = ext2fs_file_write(file, ਮੁੱਲ, ਮੁੱਲ_len, ਨਿਰਾ);
	ext2fs_file_close(file);
	ਜੇ (ret)
		ਵਾਪਸ ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	ਬਣਤਰ ext2_inode_large inode;
	__u64 ref_ਗਿਣਤੀ;
	errcode_t ret;

	ret = ext2fs_read_inode_ਭਰਿਆ(fs, ino, (ਬਣਤਰ ext2_inode *)&inode,
				     ਆਕਾਰ(inode));
	ਜੇ (ret)
		ਜਾਓ ਬਾਹਰਲਾ;

	ref_ਗਿਣਤੀ = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_ਗਿਣਤੀ--;
	ext2fs_ਸੈੱਟ_ea_inode_ref(EXT2_INODE(&inode), ref_ਗਿਣਤੀ);

	ਜੇ (ref_ਗਿਣਤੀ)
		ਜਾਓ write_ਬਾਹਰਲਾ;

	inode.i_links_ਗਿਣਤੀ = 0;
	ext2fs_ਸੈੱਟ_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	ਜੇ (ret)
		ਜਾਓ write_ਬਾਹਰਲਾ;

	ਜੇ (ext2fs_inode_has_valid_blocks2(fs, (ਬਣਤਰ ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (ਬਣਤਰ ext2_inode *)&inode, ਨਿਰਾ,
				   0, ~0ULL);
		ਜੇ (ret)
			ਜਾਓ ਬਾਹਰਲਾ;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_ਬਾਹਰਲਾ:
	ret = ext2fs_write_inode_ਭਰਿਆ(fs, ino, (ਬਣਤਰ ext2_inode *)&inode,
				      ਆਕਾਰ(inode));
ਬਾਹਰਲਾ:
	ਵਾਪਸ ret;
}

ਸਥਿਰ errcode_t xattr_update_ਇੰਦਰਾਜ(ext2_filsys fs, ਬਣਤਰ ext2_xattr *x,
				    ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *short_name,
				    ਪੂਰਨ ਸੂਚਕਾਂਕ, ਅਚਲ ਖਾਲੀ *ਮੁੱਲ,
				    size_t ਮੁੱਲ_len, ਪੂਰਨ ਵਿੱਚ_inode)
{
	ext2_ino_t ea_ino = 0;
	ਖਾਲੀ *new_ਮੁੱਲ = ਨਿਰਾ;
	ਅੱਖਰ *new_name = ਨਿਰਾ;
	ਪੂਰਨ name_len;
	errcode_t ret;

	ਜੇ (!x->name) {
		name_len = ਲੜੀ_ਲੰਬਾਈ(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		ਜੇ (ret)
			ਜਾਓ fail;
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(ਮੁੱਲ_len, &new_ਮੁੱਲ);
	ਜੇ (ret)
		ਜਾਓ fail;
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(new_ਮੁੱਲ, ਮੁੱਲ, ਮੁੱਲ_len);

	ਜੇ (ਵਿੱਚ_inode) {
		ret = xattr_create_ea_inode(fs, ਮੁੱਲ, ਮੁੱਲ_len, &ea_ino);
		ਜੇ (ret)
			ਜਾਓ fail;
	}

	ਜੇ (x->ea_ino) {
		ret = xattr_inode_dec_ref(fs, x->ea_ino);
		ਜੇ (ret)
			ਜਾਓ fail;
	}

	ਜੇ (!x->name) {
		x->name = new_name;
		x->short_name = new_name + (short_name  - name);
	}
	x->name_ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ;

	ਜੇ (x->ਮੁੱਲ)
		ext2fs_free_mem(&x->ਮੁੱਲ);
	x->ਮੁੱਲ = new_ਮੁੱਲ;
	x->ਮੁੱਲ_len = ਮੁੱਲ_len;
	x->ea_ino = ea_ino;
	ਵਾਪਸ 0;
fail:
	ਜੇ (new_name)
		ext2fs_free_mem(&new_name);
	ਜੇ (new_ਮੁੱਲ)
		ext2fs_free_mem(&new_ਮੁੱਲ);
	ਜੇ (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ xattr_ਲੱਭੋ_position(ਬਣਤਰ ext2_xattr *attrs, ਪੂਰਨ ਗਿਣਤੀ_2,
			       ਅਚਲ ਅੱਖਰ *shortname, ਪੂਰਨ name_idx)
{
	ਬਣਤਰ ext2_xattr *x;
	ਪੂਰਨ i;
	ਪੂਰਨ shortname_len, x_shortname_len;

	shortname_len = ਲੜੀ_ਲੰਬਾਈ(shortname);

	ਲਈ (i = 0, x = attrs; i < ਗਿਣਤੀ_2; i++, x++) {
		ਜੇ (name_idx < x->name_ਸੂਚਕਾਂਕ)
			ਰੋਕੋ;
		ਜੇ (name_idx > x->name_ਸੂਚਕਾਂਕ)
			ਜਾਰੀ;

		x_shortname_len = ਲੜੀ_ਲੰਬਾਈ(x->short_name);
		ਜੇ (shortname_len < x_shortname_len)
			ਰੋਕੋ;
		ਜੇ (shortname_len > x_shortname_len)
			ਜਾਰੀ;

		ਜੇ (memcmp(shortname, x->short_name, shortname_len) <= 0)
			ਰੋਕੋ;
	}
	ਵਾਪਸ i;
}

ਸਥਿਰ errcode_t xattr_array_update(ਬਣਤਰ ext2_xattr_handle *h,
				    ਅਚਲ ਅੱਖਰ *name,
				    ਅਚਲ ਖਾਲੀ *ਮੁੱਲ, size_t ਮੁੱਲ_len,
				    ਪੂਰਨ ibody_free, ਪੂਰਨ ਖੰਡ_free,
				    ਪੂਰਨ old_idx, ਪੂਰਨ ਵਿੱਚ_inode)
{
	ਬਣਤਰ ext2_xattr tmp;
	ਪੂਰਨ ਜੋੜੋ_ਮੰਜ਼ਿਲ_ibody;
	ਪੂਰਨ needed;
	ਪੂਰਨ name_len, name_idx = 0;
	ਅਚਲ ਅੱਖਰ *shortname = name;
	ਪੂਰਨ new_idx;
	ਪੂਰਨ ret;

	ਲੱਭੋ_ea_ਸੂਚਕਾਂਕ(name, &shortname, &name_idx);
	name_len = ਲੜੀ_ਲੰਬਾਈ(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	ਜੇ (!ਵਿੱਚ_inode)
		needed += EXT2_EXT_ATTR_ਮਾਪ(ਮੁੱਲ_len);

	ਜੇ (old_idx >= 0 && old_idx < h->ibody_ਗਿਣਤੀ) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		ਜੇ (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_ਮਾਪ(
						h->attrs[old_idx].ਮੁੱਲ_len);
	}

	ਜੇ (needed <= ibody_free) {
		ਜੇ (old_idx < 0) {
			new_idx = h->ibody_ਗਿਣਤੀ;
			ਜੋੜੋ_ਮੰਜ਼ਿਲ_ibody = 1;
			ਜਾਓ ਜੋੜੋ_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_ਇੰਦਰਾਜ(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, ਮੁੱਲ,
					 ਮੁੱਲ_len, ਵਿੱਚ_inode);
		ਜੇ (ret)
			ਵਾਪਸ ret;
		ਜੇ (h->ibody_ਗਿਣਤੀ <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_ਗਿਣਤੀ + 1,
				h->attrs + h->ibody_ਗਿਣਤੀ,
				(old_idx - h->ibody_ਗਿਣਤੀ) * ਆਕਾਰ(*h->attrs));
			h->attrs[h->ibody_ਗਿਣਤੀ] = tmp;
			h->ibody_ਗਿਣਤੀ++;
		}
		ਵਾਪਸ 0;
	}

	ਜੇ (h->ibody_ਗਿਣਤੀ <= old_idx) {
		ਖੰਡ_free += EXT2_EXT_ATTR_LEN(name_len);
		ਜੇ (!h->attrs[old_idx].ea_ino)
			ਖੰਡ_free +=
				EXT2_EXT_ATTR_ਮਾਪ(h->attrs[old_idx].ਮੁੱਲ_len);
	}

	ਜੇ (needed > ਖੰਡ_free)
		ਵਾਪਸ EXT2_ET_EA_NO_SPACE;

	ਜੇ (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_ਇੰਦਰਾਜ(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, ਮੁੱਲ,
					 ਮੁੱਲ_len, ਵਿੱਚ_inode);
		ਜੇ (ret)
			ਵਾਪਸ ret;
		ਜੇ (old_idx < h->ibody_ਗਿਣਤੀ) {
			/*
			 * Move entry from ibody to the block. Note that
			 * entries in the block are sorted.
			 */
			new_idx = xattr_ਲੱਭੋ_position(h->attrs + h->ibody_ਗਿਣਤੀ,
						      h->ਗਿਣਤੀ_2 - h->ibody_ਗਿਣਤੀ,
						      shortname, name_idx);
			new_idx += h->ibody_ਗਿਣਤੀ - 1;
			tmp = h->attrs[old_idx];
			memmove(h->attrs + old_idx, h->attrs + old_idx + 1,
				(new_idx - old_idx) * ਆਕਾਰ(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_ਗਿਣਤੀ--;
		}
		ਵਾਪਸ 0;
	}

	new_idx = xattr_ਲੱਭੋ_position(h->attrs + h->ibody_ਗਿਣਤੀ,
				      h->ਗਿਣਤੀ_2 - h->ibody_ਗਿਣਤੀ,
				      shortname, name_idx);
	new_idx += h->ibody_ਗਿਣਤੀ;
	ਜੋੜੋ_ਮੰਜ਼ਿਲ_ibody = 0;

ਜੋੜੋ_new:
	ਜੇ (h->ਗਿਣਤੀ_2 == h->ਸਮਰੱਥਾ) {
		ret = ext2fs_xattrs_expand(h, 4);
		ਜੇ (ret)
			ਵਾਪਸ ret;
	}

	ret = xattr_update_ਇੰਦਰਾਜ(h->fs, &h->attrs[h->ਗਿਣਤੀ_2], name, shortname,
				 name_idx, ਮੁੱਲ, ਮੁੱਲ_len, ਵਿੱਚ_inode);
	ਜੇ (ret)
		ਵਾਪਸ ret;

	tmp = h->attrs[h->ਗਿਣਤੀ_2];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->ਗਿਣਤੀ_2 - new_idx)*ਆਕਾਰ(*h->attrs));
	h->attrs[new_idx] = tmp;
	ਜੇ (ਜੋੜੋ_ਮੰਜ਼ਿਲ_ibody)
		h->ibody_ਗਿਣਤੀ++;
	h->ਗਿਣਤੀ_2++;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ space_ਵਰਤਿਆ(ਬਣਤਰ ext2_xattr *attrs, ਪੂਰਨ ਗਿਣਤੀ_2)
{
	ਪੂਰਨ ਕੁੱਲ = 0;
	ਬਣਤਰ ext2_xattr *x;
	ਪੂਰਨ i, len;

	ਲਈ (i = 0, x = attrs; i < ਗਿਣਤੀ_2; i++, x++) {
		len = ਲੜੀ_ਲੰਬਾਈ(x->short_name);
		ਕੁੱਲ += EXT2_EXT_ATTR_LEN(len);
		ਜੇ (!x->ea_ino)
			ਕੁੱਲ += EXT2_EXT_ATTR_ਮਾਪ(x->ਮੁੱਲ_len);
	}
	ਵਾਪਸ ਕੁੱਲ;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT4_XATTR_MIN_LARGE_EA_ਮਾਪ(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - ਆਕਾਰ(ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ) - 4)

errcode_t ext2fs_xattr_ਸੈੱਟ(ਬਣਤਰ ext2_xattr_handle *h,
			   ਅਚਲ ਅੱਖਰ *name,
			   ਅਚਲ ਖਾਲੀ *ਮੁੱਲ,
			   size_t ਮੁੱਲ_len)
{
	ext2_filsys fs = h->fs;
	ਅਚਲ ਪੂਰਨ inode_ਮਾਪ = EXT2_INODE_ਮਾਪ(fs->super);
	ਬਣਤਰ ext2_inode_large *inode = ਨਿਰਾ;
	ਬਣਤਰ ext2_xattr *x;
	ਅੱਖਰ *new_ਮੁੱਲ;
	ਪੂਰਨ ibody_free, ਖੰਡ_free;
	ਪੂਰਨ ਵਿੱਚ_inode = 0;
	ਪੂਰਨ old_idx = -1;
	ਪੂਰਨ extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(ਮੁੱਲ_len, &new_ਮੁੱਲ);
	ਜੇ (ret)
		ਵਾਪਸ ret;
	ਜੇ (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((strcmp(name, "system.posix_acl_default") == 0) ||
	     (strcmp(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_ਮੰਜ਼ਿਲ_disk_ਬਫਰ(ਮੁੱਲ, ਮੁੱਲ_len,
						       new_ਮੁੱਲ, &ਮੁੱਲ_len);
		ਜੇ (ret)
			ਜਾਓ ਬਾਹਰਲਾ;
	} ਨਹੀਂਤਾਂ ਜੇ (ਮੁੱਲ_len)
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(new_ਮੁੱਲ, ਮੁੱਲ, ਮੁੱਲ_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	ਲਈ (x = h->attrs; x < h->attrs + h->ਗਿਣਤੀ_2; x++) {
		ਜੇ (!strcmp(x->name, name)) {
			ਜੇ (!x->ea_ino && x->ਮੁੱਲ_len == ਮੁੱਲ_len &&
			    (!ਮੁੱਲ_len ||
			     !memcmp(x->ਮੁੱਲ, new_ਮੁੱਲ, ਮੁੱਲ_len))) {
				ret = 0;
				ਜਾਓ ਬਾਹਰਲਾ;
			}
			old_idx = x - h->attrs;
			ਰੋਕੋ;
		}
	}

	ret = ext2fs_get_memzero(inode_ਮਾਪ, &inode);
	ਜੇ (ret)
		ਜਾਓ ਬਾਹਰਲਾ;
	ret = ext2fs_read_inode_ਭਰਿਆ(fs, h->ino,
				     (ਬਣਤਰ ext2_inode *)inode,
				     inode_ਮਾਪ);
	ਜੇ (ret)
		ਜਾਓ ਬਾਹਰਲਾ;
	ਜੇ (inode_ਮਾਪ > EXT2_GOOD_OLD_INODE_ਮਾਪ) {
		extra_isize = inode->i_extra_isize;
		ਜੇ (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			ਜੇ (extra_isize == 0)
				extra_isize = ਆਕਾਰ(__u32);
		}
		ibody_free = inode_ਮਾਪ - EXT2_GOOD_OLD_INODE_ਮਾਪ;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= ਆਕਾਰ(__u32) * 2;
		ibody_free -= space_ਵਰਤਿਆ(h->attrs, h->ibody_ਗਿਣਤੀ);
	} ਨਹੀਂਤਾਂ
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	ਜੇ (strcmp(name, "system.data") == 0) {
		ਜੇ (h->ibody_ਗਿਣਤੀ <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			ਜਾਓ ਬਾਹਰਲਾ;
		}
		ret = xattr_array_update(h, name, new_ਮੁੱਲ, ਮੁੱਲ_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		ਜੇ (ret)
			ਜਾਓ ਬਾਹਰਲਾ;
		ਜਾਓ write_ਬਾਹਰਲਾ;
	}

	ਖੰਡ_free = fs->blocksize;
	ਖੰਡ_free -= ਆਕਾਰ(ਬਣਤਰ ext2_ext_attr_ਸਿਰਲੇਖ);
	/* Final null entry. */
	ਖੰਡ_free -= ਆਕਾਰ(__u32);
	ਖੰਡ_free -= space_ਵਰਤਿਆ(h->attrs + h->ibody_ਗਿਣਤੀ,
				 h->ਗਿਣਤੀ_2 - h->ibody_ਗਿਣਤੀ);

	ਜੇ (ext2fs_has_feature_ea_inode(fs->super) &&
	    ਮੁੱਲ_len > EXT4_XATTR_MIN_LARGE_EA_ਮਾਪ(fs->blocksize))
		ਵਿੱਚ_inode = 1;

	ret = xattr_array_update(h, name, new_ਮੁੱਲ, ਮੁੱਲ_len, ibody_free,
				 ਖੰਡ_free, old_idx, ਵਿੱਚ_inode);
	ਜੇ (ret == EXT2_ET_EA_NO_SPACE && !ਵਿੱਚ_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_ਮੁੱਲ, ਮੁੱਲ_len,
			ibody_free, ਖੰਡ_free, old_idx, 1 /* in_inode */);
	ਜੇ (ret)
		ਜਾਓ ਬਾਹਰਲਾ;

write_ਬਾਹਰਲਾ:
	ret = ext2fs_xattrs_write(h);
ਬਾਹਰਲਾ:
	ਜੇ (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_ਮੁੱਲ);
	ਵਾਪਸ ret;
}

errcode_t ext2fs_xattr_remove(ਬਣਤਰ ext2_xattr_handle *handle,
			      ਅਚਲ ਅੱਖਰ *ਕੁੰਜੀ)
{
	ਬਣਤਰ ext2_xattr *x;
	ਬਣਤਰ ext2_xattr *ਸਿਰਾ = handle->attrs + handle->ਗਿਣਤੀ_2;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	ਲਈ (x = handle->attrs; x < ਸਿਰਾ; x++) {
		ਜੇ (strcmp(x->name, ਕੁੰਜੀ) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->ਮੁੱਲ);
			ਜੇ (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (ਸਿਰਾ - x - 1)*ਆਕਾਰ(*x));
			memset(ਸਿਰਾ - 1, 0, ਆਕਾਰ(*ਸਿਰਾ));
			ਜੇ (x < handle->attrs + handle->ibody_ਗਿਣਤੀ)
				handle->ibody_ਗਿਣਤੀ--;
			handle->ਗਿਣਤੀ_2--;
			ਵਾਪਸ ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	ਵਾਪਸ 0;
}

errcode_t ext2fs_xattr_remove_all(ਬਣਤਰ ext2_xattr_handle *handle)
{
	ਬਣਤਰ ext2_xattr *x;
	ਬਣਤਰ ext2_xattr *ਸਿਰਾ = handle->attrs + handle->ਗਿਣਤੀ_2;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	ਲਈ (x = handle->attrs; x < ਸਿਰਾ; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->ਮੁੱਲ);
		ਜੇ (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_ਗਿਣਤੀ = 0;
	handle->ਗਿਣਤੀ_2 = 0;
	ਵਾਪਸ ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     ਬਣਤਰ ext2_xattr_handle **handle)
{
	ਬਣਤਰ ext2_xattr_handle *h;
	errcode_t err;

	ਜੇ (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_ਅੰਕੜੇ(fs->super))
		ਵਾਪਸ EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(ਆਕਾਰ(*h), &h);
	ਜੇ (err)
		ਵਾਪਸ err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->ਸਮਰੱਥਾ = 4;
	err = ext2fs_get_arrayzero(h->ਸਮਰੱਥਾ, ਆਕਾਰ(ਬਣਤਰ ext2_xattr),
				   &h->attrs);
	ਜੇ (err) {
		ext2fs_free_mem(&h);
		ਵਾਪਸ err;
	}
	h->ਗਿਣਤੀ_2 = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	ਵਾਪਸ 0;
}

errcode_t ext2fs_xattrs_close(ਬਣਤਰ ext2_xattr_handle **handle)
{
	ਬਣਤਰ ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_ਕੁੰਜੀਆਂ(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	ਵਾਪਸ 0;
}

errcode_t ext2fs_xattrs_ਗਿਣਤੀ(ਬਣਤਰ ext2_xattr_handle *handle, size_t *ਗਿਣਤੀ_2)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*ਗਿਣਤੀ_2 = handle->ਗਿਣਤੀ_2;
	ਵਾਪਸ 0;
}

errcode_t ext2fs_xattrs_flags(ਬਣਤਰ ext2_xattr_handle *handle,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *new_flags, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	ਜੇ (old_flags)
		*old_flags = handle->flags;
	ਜੇ (new_flags)
		handle->flags = *new_flags;
	ਵਾਪਸ 0;
}
