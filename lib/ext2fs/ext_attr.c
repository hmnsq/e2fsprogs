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
#جيڪڏهن_پورو
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
		أرجع retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	أرجع 0;
}

#وصف_ڪريو NAME_HASH_سرڪايو 5
#وصف_ڪريو قدر_HASH_سرڪايو 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_داخلا(بنية ext2_ext_attr_داخلا *داخلا, فراغ *معلومات)
{
	__u32 hash = 0;
	بنانشان حرف *name = (((بنانشان حرف *) داخلا) +
			       حجم(بنية ext2_ext_attr_داخلا));
	صحيح n;

	لكل (n = 0; n < داخلا->e_name_len; n++) {
		hash = (hash << NAME_HASH_سرڪايو) ^
		       (hash >> (8*حجم(hash) - NAME_HASH_سرڪايو)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	إذا (داخلا->e_قدر_inum == 0 && داخلا->e_قدر_سائيز != 0) {
		__u32 *قدر = (__u32 *)معلومات;
		لكل (n = (داخلا->e_قدر_سائيز + EXT2_EXT_ATTR_ڦيرو) >>
			 EXT2_EXT_ATTR_PAD_بٽ; n; n--) {
			hash = (hash << قدر_HASH_سرڪايو) ^
			       (hash >> (8*حجم(hash) - قدر_HASH_سرڪايو)) ^
			       ext2fs_le32_منزل_cpu(*قدر++);
		}
	}

	أرجع hash;
}

__u32 ext2fs_ext_attr_hash_داخلا_signed(بنية ext2_ext_attr_داخلا *داخلا,
					فراغ *معلومات)
{
	__u32 hash = 0;
	نشانوارو حرف *name = (((نشانوارو حرف *) داخلا) +
			     حجم(بنية ext2_ext_attr_داخلا));
	صحيح n;

	لكل (n = 0; n < داخلا->e_name_len; n++) {
		hash = (hash << NAME_HASH_سرڪايو) ^
		       (hash >> (8*حجم(hash) - NAME_HASH_سرڪايو)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	إذا (داخلا->e_قدر_inum == 0 && داخلا->e_قدر_سائيز != 0) {
		__u32 *قدر = (__u32 *)معلومات;
		لكل (n = (داخلا->e_قدر_سائيز + EXT2_EXT_ATTR_ڦيرو) >>
			 EXT2_EXT_ATTR_PAD_بٽ; n; n--) {
			hash = (hash << قدر_HASH_سرڪايو) ^
			       (hash >> (8*حجم(hash) - قدر_HASH_سرڪايو)) ^
			       ext2fs_le32_منزل_cpu(*قدر++);
		}
	}

	أرجع hash;
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
				      بنية ext2_ext_attr_داخلا *داخلا,
				      فراغ *معلومات, __u32 *hash,
				      __u32 *signed_hash)
{
	*hash = ext2fs_ext_attr_hash_داخلا(داخلا, معلومات);
	إذا (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_داخلا_signed(داخلا, معلومات);

	إذا (داخلا->e_قدر_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, داخلا->e_قدر_inum,
					    &ea_inode_hash);
		إذا (retval)
			أرجع retval;

		*hash = (*hash << قدر_HASH_سرڪايو) ^
			(*hash >> (8*حجم(*hash) - قدر_HASH_سرڪايو)) ^
			ea_inode_hash;
		إذا (signed_hash)
			*signed_hash = (*signed_hash << قدر_HASH_سرڪايو) ^
				(*signed_hash >> (8*حجم(*hash) -
						  قدر_HASH_سرڪايو)) ^
				ea_inode_hash;
	}
	أرجع 0;
}

/*
 * ext2fs_ext_attr_hash_entry2()
 *
 * Compute the hash of an extended attribute.
 * This version of the function supports hashing entries that reference
 * external inodes (ea_inode feature).
 */
errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
				      بنية ext2_ext_attr_داخلا *داخلا,
				      فراغ *معلومات, __u32 *hash)
{
	أرجع ext2fs_ext_attr_hash_entry3(fs, داخلا, معلومات, hash, عدم);
}

#وصف_رد NAME_HASH_سرڪايو
#وصف_رد قدر_HASH_سرڪايو

#وصف_ڪريو بلاڪ_HASH_سرڪايو 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
فراغ ext2fs_ext_attr_بلاڪ_rehash(بنية ext2_ext_attr_سرو *سرو,
				  بنية ext2_ext_attr_داخلا *پڇاڙي)
{
	بنية ext2_ext_attr_داخلا *هتي;
	__u32 hash = 0;

	هتي = (بنية ext2_ext_attr_داخلا *)(سرو+1);
	طالما (هتي < پڇاڙي && !EXT2_EXT_IS_آخري_داخلا(هتي)) {
		إذا (!هتي->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			اكسر;
		}
		hash = (hash << بلاڪ_HASH_سرڪايو) ^
		       (hash >> (8*حجم(hash) - بلاڪ_HASH_سرڪايو)) ^
		       هتي->e_hash;
		هتي = EXT2_EXT_ATTR_ايندڙ(هتي);
	}
	سرو->h_hash = hash;
}

#وصف_رد بلاڪ_HASH_سرڪايو

__u32 ext2fs_get_ea_inode_hash(بنية ext2_inode *inode)
{
	أرجع inode->i_atime;
}

فراغ ext2fs_سيٽ_ea_inode_hash(بنية ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(بنية ext2_inode *inode)
{
	أرجع ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_نسخو;
}

فراغ ext2fs_سيٽ_ea_inode_ref(بنية ext2_inode *inode, __u64 ref_ڳڻپ)
{
	inode->i_ctime = (__u32)(ref_ڳڻپ >> 32);
	inode->osd1.linux1.l_i_نسخو = (__u32)ref_ڳڻپ;
}

ساكن errcode_t check_ext_attr_سرو(بنية ext2_ext_attr_سرو *سرو)
{
	إذا ((سرو->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     سرو->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    سرو->h_بلاڪون != 1)
		أرجع EXT2_ET_BAD_EA_سرو;

	أرجع 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t بلاڪ, فراغ *buf,
				ext2_ino_t inum)
{
	صحيح		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, بلاڪ, 1, buf);
	إذا (retval)
		أرجع retval;

	إذا (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_بلاڪ_csum_verify(fs, inum, بلاڪ, buf))
		csum_failed = 1;

#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	ext2fs_مٽا_سٽا_ext_attr(buf, buf, fs->blocksize, 1);
#جيڪڏهن_پورو

	retval = check_ext_attr_سرو(buf);
	إذا (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	أرجع retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t بلاڪ, فراغ *buf)
{
	أرجع ext2fs_read_ext_attr3(fs, بلاڪ, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t بلاڪ, فراغ *buf)
{
	أرجع ext2fs_read_ext_attr2(fs, بلاڪ, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t بلاڪ, فراغ *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	حرف		*write_buf;

#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	إذا (retval)
		أرجع retval;
	ext2fs_مٽا_سٽا_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#وإلا
	write_buf = (حرف *) inbuf;
#جيڪڏهن_پورو

	retval = ext2fs_ext_attr_بلاڪ_csum_سيٽ(fs, inum, بلاڪ,
			(بنية ext2_ext_attr_سرو *)write_buf);
	إذا (retval)
		أرجع retval;

	retval = io_channel_write_blk64(fs->io, بلاڪ, 1, write_buf);
#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#جيڪڏهن_پورو
	إذا (!retval)
		ext2fs_mark_بدليو(fs);
	أرجع retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t بلاڪ, فراغ *inbuf)
{
	أرجع ext2fs_write_ext_attr3(fs, بلاڪ, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t بلاڪ, فراغ *inbuf)
{
	أرجع ext2fs_write_ext_attr2(fs, بلاڪ, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    حرف *بلاڪ_buf, صحيح adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	بنية ext2_ext_attr_سرو *سرو;
	حرف	*buf = 0;

	إذا ((blk >= ext2fs_بلاڪون_ڳڻپ(fs->super)) ||
	    (blk < fs->super->s_پهريون_معلومات_بلاڪ))
		أرجع EXT2_ET_BAD_EA_بلاڪ_انگ;

	إذا (!بلاڪ_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		إذا (retval)
			أرجع retval;
		بلاڪ_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, بلاڪ_buf, inum);
	إذا (retval)
		اذهب errout;

	سرو = (بنية ext2_ext_attr_سرو *) بلاڪ_buf;
	سرو->h_refcount += adjust;
	إذا (newcount)
		*newcount = سرو->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, بلاڪ_buf, inum);
	إذا (retval)
		اذهب errout;

errout:
	إذا (buf)
		ext2fs_free_mem(&buf);
	أرجع retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    حرف *بلاڪ_buf, صحيح adjust,
				    __u32 *newcount)
{
	أرجع ext2fs_adjust_ea_refcount3(fs, blk, بلاڪ_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					حرف *بلاڪ_buf, صحيح adjust,
					__u32 *newcount)
{
	أرجع ext2fs_adjust_ea_refcount2(fs, blk, بلاڪ_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
بنية ext2_xattr {
	صحيح name_اشاريو;
	حرف *name;
	حرف *short_name;
	فراغ *قدر;
	بنانشان صحيح قدر_len;
	ext2_ino_t ea_ino;
};

بنية ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	بنية ext2_xattr *attrs;
	صحيح صلاحيت;
	صحيح ڳڻپ;
	صحيح ibody_ڳڻپ;
	ext2_ino_t ino;
	بنانشان صحيح flags;
};

ساكن errcode_t ext2fs_xattrs_expand(بنية ext2_xattr_handle *h,
				      بنانشان صحيح expandby)
{
	بنية ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->صلاحيت + expandby,
				   حجم(بنية ext2_xattr), &new_attrs);
	إذا (err)
		أرجع err;

	يادگيري_نقل(new_attrs, h->attrs, h->صلاحيت * حجم(بنية ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->صلاحيت += expandby;
	h->attrs = new_attrs;

	أرجع 0;
}

بنية ea_name_اشاريو {
	صحيح اشاريو;
	ثابت حرف *name;
};

/* Keep these names sorted in order of decreasing specificity. */
ساكن بنية ea_name_اشاريو ea_نالا[] = {
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

ساكن ثابت حرف *ڳوليو_ea_اڳياڙي(صحيح اشاريو)
{
	بنية ea_name_اشاريو *e;

	لكل (e = ea_نالا; e->name; e++)
		إذا (e->اشاريو == اشاريو)
			أرجع e->name;

	أرجع عدم;
}

ساكن صحيح ڳوليو_ea_اشاريو(ثابت حرف *fullname, ثابت حرف **name, صحيح *اشاريو)
{
	بنية ea_name_اشاريو *e;

	لكل (e = ea_نالا; e->name; e++) {
		إذا (لڙي_ڀيٽ(fullname, e->name, لڙي_ڊيگهه(e->name)) == 0) {
			*name = fullname + لڙي_ڊيگهه(e->name);
			*اشاريو = e->اشاريو;
			أرجع 1;
		}
	}
	أرجع 0;
}

errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       بنية ext2_inode_large *inode)
{
	بنية ext2_ext_attr_سرو *سرو;
	فراغ *بلاڪ_buf = عدم;
	blk64_t blk;
	errcode_t err;
	بنية ext2_inode_large i;

	/* Read inode? */
	إذا (inode == عدم) {
		err = ext2fs_read_inode_ڀريل(fs, ino, (بنية ext2_inode *)&i,
					     حجم(بنية ext2_inode_large));
		إذا (err)
			أرجع err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_بلاڪ(fs, (بنية ext2_inode *)inode);
	إذا (blk == 0)
		أرجع 0;

	/* Find block, zero it, write back */
	إذا ((blk < fs->super->s_پهريون_معلومات_بلاڪ) ||
	    (blk >= ext2fs_بلاڪون_ڳڻپ(fs->super))) {
		err = EXT2_ET_BAD_EA_بلاڪ_انگ;
		اذهب ٻاهريون;
	}

	err = ext2fs_get_mem(fs->blocksize, &بلاڪ_buf);
	إذا (err)
		اذهب ٻاهريون;

	err = ext2fs_read_ext_attr3(fs, blk, بلاڪ_buf, ino);
	إذا (err)
		اذهب out2;

	/* We only know how to deal with v2 EA blocks */
	سرو = (بنية ext2_ext_attr_سرو *) بلاڪ_buf;
	إذا (سرو->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_سرو;
		اذهب out2;
	}

	سرو->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, بلاڪ_buf, ino);
	إذا (err)
		اذهب out2;

	/* Erase link to block */
	ext2fs_file_acl_بلاڪ_سيٽ(fs, (بنية ext2_inode *)inode, 0);
	إذا (سرو->h_refcount == 0)
		ext2fs_بلاڪ_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_بلاڪون(fs, (بنية ext2_inode *)inode, 1);
	إذا (err)
		اذهب out2;

	/* Write inode? */
	إذا (inode == &i) {
		err = ext2fs_write_inode_ڀريل(fs, ino, (بنية ext2_inode *)&i,
					      حجم(بنية ext2_inode_large));
		إذا (err)
			اذهب out2;
	}

out2:
	ext2fs_free_mem(&بلاڪ_buf);
ٻاهريون:
	أرجع err;
}

ساكن errcode_t prep_ea_بلاڪ_for_write(ext2_filsys fs, ext2_ino_t ino,
					 بنية ext2_inode_large *inode)
{
	بنية ext2_ext_attr_سرو *سرو;
	فراغ *بلاڪ_buf = عدم;
	blk64_t blk, goal;
	errcode_t err;

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_بلاڪ(fs, (بنية ext2_inode *)inode);
	إذا (blk != 0) {
		إذا ((blk < fs->super->s_پهريون_معلومات_بلاڪ) ||
		    (blk >= ext2fs_بلاڪون_ڳڻپ(fs->super))) {
			err = EXT2_ET_BAD_EA_بلاڪ_انگ;
			اذهب ٻاهريون;
		}

		err = ext2fs_get_mem(fs->blocksize, &بلاڪ_buf);
		إذا (err)
			اذهب ٻاهريون;

		err = ext2fs_read_ext_attr3(fs, blk, بلاڪ_buf, ino);
		إذا (err)
			اذهب out2;

		/* We only know how to deal with v2 EA blocks */
		سرو = (بنية ext2_ext_attr_سرو *) بلاڪ_buf;
		إذا (سرو->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_سرو;
			اذهب out2;
		}

		/* Single-user block.  We're done here. */
		إذا (سرو->h_refcount == 1)
			اذهب out2;

		/* We need to CoW the block. */
		سرو->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, بلاڪ_buf, ino);
		إذا (err)
			اذهب out2;
	} وإلا {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_جوڙيو_بلاڪون(fs, (بنية ext2_inode *)inode,
					     1);
		إذا (err)
			اذهب ٻاهريون;
	}

	/* Allocate a block */
	goal = ext2fs_ڳوليو_inode_goal(fs, ino, (بنية ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, عدم, &blk);
	إذا (err)
		اذهب out2;
	ext2fs_file_acl_بلاڪ_سيٽ(fs, (بنية ext2_inode *)inode, blk);
out2:
	إذا (بلاڪ_buf)
		ext2fs_free_mem(&بلاڪ_buf);
ٻاهريون:
	أرجع err;
}


ساكن مضمن صحيح
posix_acl_xattr_ڳڻپ(size_t سائيز)
{
        إذا (سائيز < حجم(posix_acl_xattr_سرو))
                أرجع -1;
        سائيز -= حجم(posix_acl_xattr_سرو);
        إذا (سائيز % حجم(posix_acl_xattr_داخلا))
                أرجع -1;
        أرجع سائيز / حجم(posix_acl_xattr_داخلا);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
ساكن errcode_t convert_posix_acl_منزل_disk_بفر(ثابت فراغ *قدر, size_t سائيز,
						  فراغ *ٻاهريون_buf, size_t *سائيز_ٻاهريون)
{
	ثابت posix_acl_xattr_سرو *سرو =
		(ثابت posix_acl_xattr_سرو*) قدر;
	ثابت posix_acl_xattr_داخلا *پڇاڙي, *داخلا =
		(ثابت posix_acl_xattr_داخلا *)(سرو+1);
	ext4_acl_سرو *ext_acl;
	size_t s;
	حرف *e;

	صحيح ڳڻپ;

	إذا (!قدر)
		أرجع EINVAL;
	إذا (سائيز < حجم(posix_acl_xattr_سرو))
		أرجع ENOMEM;
	إذا (سرو->a_نسخو != ext2fs_cpu_منزل_le32(POSIX_ACL_XATTR_نسخو))
		أرجع EINVAL;

	ڳڻپ = posix_acl_xattr_ڳڻپ(سائيز);
	ext_acl = ٻاهريون_buf;
	ext_acl->a_نسخو = ext2fs_cpu_منزل_le32(EXT4_ACL_نسخو);

	إذا (ڳڻپ <= 0)
		أرجع EINVAL;

	e = (حرف *) ٻاهريون_buf + حجم(ext4_acl_سرو);
	s = حجم(ext4_acl_سرو);
	لكل (پڇاڙي = داخلا + ڳڻپ; داخلا != پڇاڙي;داخلا++) {
		ext4_acl_داخلا *disk_داخلا = (ext4_acl_داخلا*) e;
		disk_داخلا->e_ٽيگ = داخلا->e_ٽيگ;
		disk_داخلا->e_perm = داخلا->e_perm;

		بدل(ext2fs_le16_منزل_cpu(داخلا->e_ٽيگ)) {
			حالة ACL_USER_OBJ:
			حالة ACL_گروهه_OBJ:
			حالة ACL_نقاب:
			حالة ACL_OTHER:
				e += حجم(ext4_acl_داخلا_short);
				s += حجم(ext4_acl_داخلا_short);
				اكسر;
			حالة ACL_USER:
			حالة ACL_گروهه:
				disk_داخلا->e_id = داخلا->e_id;
				e += حجم(ext4_acl_داخلا);
				s += حجم(ext4_acl_داخلا);
				اكسر;
			افتراضي:
				أرجع EINVAL;
		}
	}
	*سائيز_ٻاهريون = s;
	أرجع 0;
}

ساكن errcode_t convert_disk_بفر_منزل_posix_acl(ثابت فراغ *قدر, size_t سائيز,
						  فراغ **ٻاهريون_buf, size_t *سائيز_ٻاهريون)
{
	posix_acl_xattr_سرو *سرو;
	posix_acl_xattr_داخلا *داخلا;
	ثابت ext4_acl_سرو *ext_acl = (ثابت ext4_acl_سرو *) قدر;
	errcode_t err;
	ثابت حرف *cp;
	حرف *ٻاهريون;

	إذا ((!قدر) ||
	    (سائيز < حجم(ext4_acl_سرو)) ||
	    (ext_acl->a_نسخو != ext2fs_cpu_منزل_le32(EXT4_ACL_نسخو)))
		أرجع EINVAL;

	err = ext2fs_get_mem(سائيز * 2, &ٻاهريون);
	إذا (err)
		أرجع err;

	سرو = (posix_acl_xattr_سرو *) ٻاهريون;
	سرو->a_نسخو = ext2fs_cpu_منزل_le32(POSIX_ACL_XATTR_نسخو);
	داخلا = (posix_acl_xattr_داخلا *) (ٻاهريون + حجم(posix_acl_xattr_سرو));

	cp = (ثابت حرف *) قدر + حجم(ext4_acl_سرو);
	سائيز -= حجم(ext4_acl_سرو);

	طالما (سائيز > 0) {
		ثابت ext4_acl_داخلا *disk_داخلا = (ثابت ext4_acl_داخلا *) cp;

		داخلا->e_ٽيگ = disk_داخلا->e_ٽيگ;
		داخلا->e_perm = disk_داخلا->e_perm;

		بدل(ext2fs_le16_منزل_cpu(داخلا->e_ٽيگ)) {
			حالة ACL_USER_OBJ:
			حالة ACL_گروهه_OBJ:
			حالة ACL_نقاب:
			حالة ACL_OTHER:
				داخلا->e_id = 0;
				cp += حجم(ext4_acl_داخلا_short);
				سائيز -= حجم(ext4_acl_داخلا_short);
				اكسر;
			حالة ACL_USER:
			حالة ACL_گروهه:
				داخلا->e_id = disk_داخلا->e_id;
				cp += حجم(ext4_acl_داخلا);
				سائيز -= حجم(ext4_acl_داخلا);
				اكسر;
			افتراضي:
				ext2fs_free_mem(&ٻاهريون);
				أرجع EINVAL;
		}
		داخلا++;
	}
	*ٻاهريون_buf = ٻاهريون;
	*سائيز_ٻاهريون = ((حرف *) داخلا - ٻاهريون);
	أرجع 0;
}

ساكن errcode_t
write_xattrs_منزل_بفر(ext2_filsys fs, بنية ext2_xattr *attrs, صحيح ڳڻپ,
		       فراغ *entries_شروعات, بنانشان صحيح storage_سائيز,
		       بنانشان صحيح قدر_وٿي_correction, صحيح write_hash)
{
	بنية ext2_xattr *x;
	بنية ext2_ext_attr_داخلا *e = entries_شروعات;
	حرف *پڇاڙي = (حرف *) entries_شروعات + storage_سائيز;
	بنانشان صحيح قدر_سائيز;
	errcode_t err;

	memset(entries_شروعات, 0, storage_سائيز);
	لكل (x = attrs; x < attrs + ڳڻپ; x++) {
		قدر_سائيز = ((x->قدر_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = لڙي_ڊيگهه(x->short_name);
		e->e_name_اشاريو = x->name_اشاريو;

		e->e_قدر_سائيز = x->قدر_len;
		e->e_قدر_inum = x->ea_ino;

		/* Store name */
		يادگيري_نقل((حرف *)e + حجم(*e), x->short_name, e->e_name_len);
		إذا (x->ea_ino) {
			e->e_قدر_offs = 0;
		} وإلا {
			پڇاڙي -= قدر_سائيز;
			e->e_قدر_offs = پڇاڙي - (حرف *) entries_شروعات +
						قدر_وٿي_correction;
			يادگيري_نقل(پڇاڙي, x->قدر, e->e_قدر_سائيز);
		}

		إذا (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : پڇاڙي,
							  &e->e_hash);
			إذا (err)
				أرجع err;
		} وإلا
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_ايندڙ(e);
		*(__u32 *)e = 0;
	}
	أرجع 0;
}

errcode_t ext2fs_xattrs_write(بنية ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	ثابت بنانشان صحيح inode_سائيز = EXT2_INODE_سائيز(fs->super);
	بنية ext2_inode_large *inode;
	حرف *شروعات, *بلاڪ_buf = عدم;
	بنية ext2_ext_attr_سرو *سرو;
	__u32 ea_inode_magic;
	blk64_t blk;
	بنانشان صحيح storage_سائيز;
	بنانشان صحيح i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_سائيز;
	إذا (i < حجم(*inode))
		i = حجم(*inode);
	err = ext2fs_get_memzero(i, &inode);
	إذا (err)
		أرجع err;

	err = ext2fs_read_inode_ڀريل(fs, handle->ino, EXT2_INODE(inode),
				     inode_سائيز);
	إذا (err)
		اذهب ٻاهريون;

	/* If extra_isize isn't set, we need to set it now */
	إذا (inode->i_extra_isize == 0 &&
	    inode_سائيز > EXT2_GOOD_OLD_INODE_سائيز) {
		حرف *p = (حرف *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		إذا (extra == 0)
			extra = حجم(__u32);
		memset(p + EXT2_GOOD_OLD_INODE_سائيز, 0, extra);
		inode->i_extra_isize = extra;
	}
	إذا (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		اذهب ٻاهريون;
	}

	/* Does the inode have space for EA? */
	إذا (inode->i_extra_isize < حجم(inode->i_extra_isize) ||
	    inode_سائيز <= EXT2_GOOD_OLD_INODE_سائيز + inode->i_extra_isize +
								حجم(__u32))
		اذهب write_ea_بلاڪ;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	يادگيري_نقل(((حرف *) inode) + EXT2_GOOD_OLD_INODE_سائيز +
	       inode->i_extra_isize, &ea_inode_magic, حجم(__u32));
	storage_سائيز = inode_سائيز - EXT2_GOOD_OLD_INODE_سائيز -
				inode->i_extra_isize - حجم(__u32);
	شروعات = ((حرف *) inode) + EXT2_GOOD_OLD_INODE_سائيز +
				inode->i_extra_isize + حجم(__u32);

	err = write_xattrs_منزل_بفر(fs, handle->attrs, handle->ibody_ڳڻپ,
				     شروعات, storage_سائيز, 0, 0);
	إذا (err)
		اذهب ٻاهريون;
write_ea_بلاڪ:
	/* Are we done? */
	إذا (handle->ibody_ڳڻپ == handle->ڳڻپ &&
	    !ext2fs_file_acl_بلاڪ(fs, EXT2_INODE(inode)))
		اذهب skip_ea_بلاڪ;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &بلاڪ_buf);
	إذا (err)
		اذهب ٻاهريون;

	storage_سائيز = fs->blocksize - حجم(بنية ext2_ext_attr_سرو);
	شروعات = بلاڪ_buf + حجم(بنية ext2_ext_attr_سرو);

	err = write_xattrs_منزل_بفر(fs, handle->attrs + handle->ibody_ڳڻپ,
				     handle->ڳڻپ - handle->ibody_ڳڻپ, شروعات,
				     storage_سائيز, شروعات - بلاڪ_buf, 1);
	إذا (err)
		اذهب out2;

	/* Write a header on the EA block */
	سرو = (بنية ext2_ext_attr_سرو *) بلاڪ_buf;
	سرو->h_magic = EXT2_EXT_ATTR_MAGIC;
	سرو->h_refcount = 1;
	سرو->h_بلاڪون = 1;

	/* Get a new block for writing */
	err = prep_ea_بلاڪ_for_write(fs, handle->ino, inode);
	إذا (err)
		اذهب out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_بلاڪ(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, بلاڪ_buf, handle->ino);
	إذا (err)
		اذهب out2;

skip_ea_بلاڪ:
	blk = ext2fs_file_acl_بلاڪ(fs, (بنية ext2_inode *)inode);
	إذا (!بلاڪ_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		إذا (err)
			اذهب ٻاهريون;
	}

	/* Write the inode */
	err = ext2fs_write_inode_ڀريل(fs, handle->ino, EXT2_INODE(inode),
				      inode_سائيز);
	إذا (err)
		اذهب out2;

out2:
	ext2fs_free_mem(&بلاڪ_buf);
ٻاهريون:
	ext2fs_free_mem(&inode);
	أرجع err;
}

ساكن errcode_t read_xattrs_from_بفر(بنية ext2_xattr_handle *handle,
					 بنية ext2_inode_large *inode,
					 بنية ext2_ext_attr_داخلا *entries,
					 بنانشان صحيح storage_سائيز,
					 حرف *قدر_شروعات)
{
	بنية ext2_xattr *x;
	بنية ext2_ext_attr_داخلا *داخلا, *پڇاڙي;
	ثابت حرف *اڳياڙي;
	بنانشان صحيح remain, اڳياڙي_len;
	errcode_t err;
	بنانشان صحيح قدرون_سائيز = storage_سائيز +
			((حرف *)entries - قدر_شروعات);

	/* find the end */
	پڇاڙي = entries;
	remain = storage_سائيز;
	طالما (remain >= حجم(بنية ext2_ext_attr_داخلا) &&
	       !EXT2_EXT_IS_آخري_داخلا(پڇاڙي)) {

		/* header eats this space */
		remain -= حجم(بنية ext2_ext_attr_داخلا);

		/* is attribute name valid? */
		إذا (EXT2_EXT_ATTR_سائيز(پڇاڙي->e_name_len) > remain)
			أرجع EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_سائيز(پڇاڙي->e_name_len);
		پڇاڙي = EXT2_EXT_ATTR_ايندڙ(پڇاڙي);
	}

	داخلا = entries;
	remain = storage_سائيز;
	طالما (remain >= حجم(بنية ext2_ext_attr_داخلا) &&
	       !EXT2_EXT_IS_آخري_داخلا(داخلا)) {

		/* Allocate space for more attrs? */
		إذا (handle->ڳڻپ == handle->صلاحيت) {
			err = ext2fs_xattrs_expand(handle, 4);
			إذا (err)
				أرجع err;
		}

		x = handle->attrs + handle->ڳڻپ;

		/* header eats this space */
		remain -= حجم(بنية ext2_ext_attr_داخلا);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_سائيز(داخلا->e_name_len);

		/* Extract name */
		اڳياڙي = ڳوليو_ea_اڳياڙي(داخلا->e_name_اشاريو);
		اڳياڙي_len = (اڳياڙي ? لڙي_ڊيگهه(اڳياڙي) : 0);
		err = ext2fs_get_memzero(داخلا->e_name_len + اڳياڙي_len + 1,
					 &x->name);
		إذا (err)
			أرجع err;
		إذا (اڳياڙي)
			يادگيري_نقل(x->name, اڳياڙي, اڳياڙي_len);
		إذا (داخلا->e_name_len)
			يادگيري_نقل(x->name + اڳياڙي_len,
			       (حرف *)داخلا + حجم(*داخلا),
			       داخلا->e_name_len);
		x->short_name = x->name + اڳياڙي_len;
		x->name_اشاريو = داخلا->e_name_اشاريو;

		/* Check & copy value */
		إذا (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    داخلا->e_قدر_inum != 0)
			أرجع EXT2_ET_BAD_EA_بلاڪ_انگ;

		إذا (داخلا->e_قدر_inum == 0) {
			إذا (داخلا->e_قدر_سائيز > remain)
				أرجع EXT2_ET_EA_BAD_قدر_سائيز;

			إذا (داخلا->e_قدر_offs + داخلا->e_قدر_سائيز > قدرون_سائيز)
				أرجع EXT2_ET_EA_BAD_قدر_وٿي;

			إذا (داخلا->e_قدر_سائيز > 0 &&
			    قدر_شروعات + داخلا->e_قدر_offs <
			    (حرف *)پڇاڙي + حجم(__u32))
				أرجع EXT2_ET_EA_BAD_قدر_وٿي;

			remain -= داخلا->e_قدر_سائيز;

			err = ext2fs_get_mem(داخلا->e_قدر_سائيز, &x->قدر);
			إذا (err)
				أرجع err;
			يادگيري_نقل(x->قدر, قدر_شروعات + داخلا->e_قدر_offs,
			       داخلا->e_قدر_سائيز);
		} وإلا {
			بنية ext2_inode *ea_inode;
			ext2_file_t ea_file;

			إذا (داخلا->e_قدر_offs != 0)
				أرجع EXT2_ET_EA_BAD_قدر_وٿي;

			إذا (داخلا->e_قدر_سائيز > (64 * 1024))
				أرجع EXT2_ET_EA_BAD_قدر_سائيز;

			err = ext2fs_get_mem(داخلا->e_قدر_سائيز, &x->قدر);
			إذا (err)
				أرجع err;

			err = ext2fs_file_open(handle->fs, داخلا->e_قدر_inum,
					       0, &ea_file);
			إذا (err)
				أرجع err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			إذا ((ea_inode->i_flags & EXT4_INLINE_معلومات_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_ڳڻپ == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			وإلا إذا ((__u64) ext2fs_file_get_سائيز(ea_file) !=
				 داخلا->e_قدر_سائيز)
				err = EXT2_ET_EA_BAD_قدر_سائيز;
			وإلا
				err = ext2fs_file_read(ea_file, x->قدر,
						       داخلا->e_قدر_سائيز, 0);
			ext2fs_file_close(ea_file);
			إذا (err)
				أرجع err;
		}

		x->ea_ino = داخلا->e_قدر_inum;
		x->قدر_len = داخلا->e_قدر_سائيز;

		/* e_hash may be 0 in older inode's ea */
		إذا (داخلا->e_hash != 0) {
			__u32 hash, signed_hash;

			فراغ *معلومات = (داخلا->e_قدر_inum != 0) ?
					0 : قدر_شروعات + داخلا->e_قدر_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, داخلا,
							  معلومات, &hash,
							  &signed_hash);
			إذا (err)
				أرجع err;
			إذا ((داخلا->e_hash != hash) &&
			    (داخلا->e_hash != signed_hash)) {
				بنية ext2_inode ٻار;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							داخلا->e_قدر_inum,
							&ٻار);
				إذا (err)
					أرجع err;
				إذا (ٻار.i_mtime != handle->ino ||
				    ٻار.i_generation != inode->i_generation)
					أرجع EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->ڳڻپ++;
		داخلا = EXT2_EXT_ATTR_ايندڙ(داخلا);
	}

	أرجع 0;
}

ساكن فراغ xattrs_free_ڪنجيون(بنية ext2_xattr_handle *h)
{
	بنية ext2_xattr *a = h->attrs;
	صحيح i;

	لكل (i = 0; i < h->صلاحيت; i++) {
		إذا (a[i].name)
			ext2fs_free_mem(&a[i].name);
		إذا (a[i].قدر)
			ext2fs_free_mem(&a[i].قدر);
	}
	h->ڳڻپ = 0;
	h->ibody_ڳڻپ = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(بنية ext2_xattr_handle *handle,
				   بنية ext2_inode_large *inode)
{
	بنية ext2_ext_attr_سرو *سرو;
	__u32 ea_inode_magic;
	بنانشان صحيح storage_سائيز;
	حرف *شروعات, *بلاڪ_buf = عدم;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_ڪنجيون(handle);

	/* Does the inode have space for EA? */
	إذا (inode->i_extra_isize < حجم(inode->i_extra_isize) ||
	    EXT2_INODE_سائيز(handle->fs->super) <= EXT2_GOOD_OLD_INODE_سائيز +
						  inode->i_extra_isize +
						  حجم(__u32))
		اذهب read_ea_بلاڪ;
	إذا (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		اذهب ٻاهريون;
	}

	/* Look for EA in the inode */
	يادگيري_نقل(&ea_inode_magic, ((حرف *) inode) + EXT2_GOOD_OLD_INODE_سائيز +
	       inode->i_extra_isize, حجم(__u32));
	إذا (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_سائيز = EXT2_INODE_سائيز(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_سائيز - inode->i_extra_isize -
			حجم(__u32);
		شروعات = ((حرف *) inode) + EXT2_GOOD_OLD_INODE_سائيز +
			inode->i_extra_isize + حجم(__u32);

		err = read_xattrs_from_بفر(handle, inode,
					(بنية ext2_ext_attr_داخلا *) شروعات,
					storage_سائيز, شروعات);
		إذا (err)
			اذهب ٻاهريون;

		handle->ibody_ڳڻپ = handle->ڳڻپ;
	}

read_ea_بلاڪ:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_بلاڪ(handle->fs, EXT2_INODE(inode));
	إذا (blk != 0) {
		إذا ((blk < handle->fs->super->s_پهريون_معلومات_بلاڪ) ||
		    (blk >= ext2fs_بلاڪون_ڳڻپ(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_بلاڪ_انگ;
			اذهب ٻاهريون;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &بلاڪ_buf);
		إذا (err)
			اذهب ٻاهريون;

		err = ext2fs_read_ext_attr3(handle->fs, blk, بلاڪ_buf,
					    handle->ino);
		إذا (err)
			اذهب out3;

		/* We only know how to deal with v2 EA blocks */
		سرو = (بنية ext2_ext_attr_سرو *) بلاڪ_buf;
		إذا (سرو->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_سرو;
			اذهب out3;
		}

		/* Read EAs */
		storage_سائيز = handle->fs->blocksize -
			حجم(بنية ext2_ext_attr_سرو);
		شروعات = بلاڪ_buf + حجم(بنية ext2_ext_attr_سرو);
		err = read_xattrs_from_بفر(handle, inode,
					(بنية ext2_ext_attr_داخلا *) شروعات,
					storage_سائيز, بلاڪ_buf);
	}

out3:
	إذا (بلاڪ_buf)
		ext2fs_free_mem(&بلاڪ_buf);
ٻاهريون:
	أرجع err;
}

errcode_t ext2fs_xattrs_read(بنية ext2_xattr_handle *handle)
{
	بنية ext2_inode_large *inode;
	size_t inode_سائيز = EXT2_INODE_سائيز(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	إذا (inode_سائيز < حجم(*inode))
		inode_سائيز = حجم(*inode);
	err = ext2fs_get_memzero(inode_سائيز, &inode);
	إذا (err)
		أرجع err;

	err = ext2fs_read_inode_ڀريل(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_سائيز(handle->fs->super));
	إذا (err)
		اذهب ٻاهريون;

	err = ext2fs_xattrs_read_inode(handle, inode);

ٻاهريون:
	ext2fs_free_mem(&inode);

	أرجع err;
}

errcode_t ext2fs_xattrs_iterate(بنية ext2_xattr_handle *h,
				صحيح (*func)(حرف *name, حرف *قدر,
					    size_t قدر_len, فراغ *معلومات),
				فراغ *معلومات)
{
	بنية ext2_xattr *x;
	صحيح dirty = 0;
	صحيح ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = h->attrs; x < h->attrs + h->ڳڻپ; x++) {
		ret = func(x->name, x->قدر, x->قدر_len, معلومات);
		إذا (ret & XATTR_بدليو)
			dirty = 1;
		إذا (ret & XATTR_ABORT)
			اكسر;
	}

	إذا (dirty)
		أرجع ext2fs_xattrs_write(h);
	أرجع 0;
}

errcode_t ext2fs_xattr_get(بنية ext2_xattr_handle *h, ثابت حرف *ڪنجي,
			   فراغ **قدر, size_t *قدر_len)
{
	بنية ext2_xattr *x;
	حرف *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = h->attrs; x < h->attrs + h->ڳڻپ; x++) {
		إذا (strcmp(x->name, ڪنجي))
			استمر;

		إذا (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(ڪنجي, "system.posix_acl_default") == 0) ||
		     (strcmp(ڪنجي, "system.posix_acl_access") == 0))) {
			err = convert_disk_بفر_منزل_posix_acl(x->قدر, x->قدر_len,
							       قدر, قدر_len);
			أرجع err;
		} وإلا {
			err = ext2fs_get_mem(x->قدر_len, &val);
			إذا (err)
				أرجع err;
			يادگيري_نقل(val, x->قدر, x->قدر_len);
			*قدر = val;
			*قدر_len = x->قدر_len;
			أرجع 0;
		}
	}

	أرجع EXT2_ET_EA_ڪنجي_NOT_مليو;
}

errcode_t ext2fs_xattr_inode_max_سائيز(ext2_filsys fs, ext2_ino_t ino,
				      size_t *سائيز)
{
	بنية ext2_ext_attr_داخلا *داخلا;
	بنية ext2_inode_large *inode;
	__u32 ea_inode_magic;
	بنانشان صحيح minoff;
	حرف *شروعات;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_سائيز(fs->super);
	إذا (i < حجم(*inode))
		i = حجم(*inode);
	err = ext2fs_get_memzero(i, &inode);
	إذا (err)
		أرجع err;

	err = ext2fs_read_inode_ڀريل(fs, ino, (بنية ext2_inode *)inode,
				     EXT2_INODE_سائيز(fs->super));
	إذا (err)
		اذهب ٻاهريون;

	/* Does the inode have size for EA? */
	إذا (EXT2_INODE_سائيز(fs->super) <= EXT2_GOOD_OLD_INODE_سائيز +
						  inode->i_extra_isize +
						  حجم(__u32)) {
		err = EXT2_ET_INLINE_معلومات_NO_SPACE;
		اذهب ٻاهريون;
	}

	minoff = EXT2_INODE_سائيز(fs->super) - حجم(*inode) - حجم(__u32);
	يادگيري_نقل(&ea_inode_magic, ((حرف *) inode) + EXT2_GOOD_OLD_INODE_سائيز +
	       inode->i_extra_isize, حجم(__u32));
	إذا (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		شروعات= ((حرف *) inode) + EXT2_GOOD_OLD_INODE_سائيز +
			inode->i_extra_isize + حجم(__u32);
		داخلا = (بنية ext2_ext_attr_داخلا *) شروعات;
		طالما (!EXT2_EXT_IS_آخري_داخلا(داخلا)) {
			إذا (!داخلا->e_قدر_inum && داخلا->e_قدر_سائيز) {
				بنانشان صحيح offs = داخلا->e_قدر_offs;
				إذا (offs < minoff)
					minoff = offs;
			}
			داخلا = EXT2_EXT_ATTR_ايندڙ(داخلا);
		}
		*سائيز = minoff - ((حرف *)داخلا - (حرف *)شروعات) - حجم(__u32);
	} وإلا {
		/* no xattr.  return a maximum size */
		*سائيز = EXT2_EXT_ATTR_سائيز(minoff -
					   EXT2_EXT_ATTR_LEN(لڙي_ڊيگهه("data")) -
					   EXT2_EXT_ATTR_ڦيرو - حجم(__u32));
	}

ٻاهريون:
	ext2fs_free_mem(&inode);
	أرجع err;
}

ساكن errcode_t xattr_create_ea_inode(ext2_filsys fs, ثابت فراغ *قدر,
				       size_t قدر_len, ext2_ino_t *ea_ino)
{
	بنية ext2_inode inode;
	ext2_ino_t ino;
	ext2_file_t file;
	__u32 hash;
	errcode_t ret;

	ret = ext2fs_new_inode(fs, 0, 0, 0, &ino);
	إذا (ret)
		أرجع ret;

	memset(&inode, 0, حجم(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	إذا (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_سائيز = 0;
	inode.i_گهڻائي = LINUX_S_IFREG | 0600;
	inode.i_links_ڳڻپ = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	إذا (ret)
		أرجع ret;
	/*
	 * ref_count and hash utilize inode's i_*time fields.
	 * ext2fs_write_new_inode() call above initializes these fields with
	 * current time. That's why ref count and hash updates are done
	 * separately below.
	 */
	ext2fs_سيٽ_ea_inode_ref(&inode, 1);
	hash = ext2fs_crc32c_le(fs->csum_ٻج, قدر, قدر_len);
	ext2fs_سيٽ_ea_inode_hash(&inode, hash);

	ret = ext2fs_write_inode(fs, ino, &inode);
	إذا (ret)
		أرجع ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	إذا (ret)
		أرجع ret;
	ret = ext2fs_file_write(file, قدر, قدر_len, عدم);
	ext2fs_file_close(file);
	إذا (ret)
		أرجع ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	أرجع 0;
}

ساكن errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	بنية ext2_inode_large inode;
	__u64 ref_ڳڻپ;
	errcode_t ret;

	ret = ext2fs_read_inode_ڀريل(fs, ino, (بنية ext2_inode *)&inode,
				     حجم(inode));
	إذا (ret)
		اذهب ٻاهريون;

	ref_ڳڻپ = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_ڳڻپ--;
	ext2fs_سيٽ_ea_inode_ref(EXT2_INODE(&inode), ref_ڳڻپ);

	إذا (ref_ڳڻپ)
		اذهب write_ٻاهريون;

	inode.i_links_ڳڻپ = 0;
	ext2fs_سيٽ_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	إذا (ret)
		اذهب write_ٻاهريون;

	إذا (ext2fs_inode_has_valid_blocks2(fs, (بنية ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (بنية ext2_inode *)&inode, عدم,
				   0, ~0ULL);
		إذا (ret)
			اذهب ٻاهريون;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_ٻاهريون:
	ret = ext2fs_write_inode_ڀريل(fs, ino, (بنية ext2_inode *)&inode,
				      حجم(inode));
ٻاهريون:
	أرجع ret;
}

ساكن errcode_t xattr_update_داخلا(ext2_filsys fs, بنية ext2_xattr *x,
				    ثابت حرف *name, ثابت حرف *short_name,
				    صحيح اشاريو, ثابت فراغ *قدر,
				    size_t قدر_len, صحيح اندريون_inode)
{
	ext2_ino_t ea_ino = 0;
	فراغ *new_قدر = عدم;
	حرف *new_name = عدم;
	صحيح name_len;
	errcode_t ret;

	إذا (!x->name) {
		name_len = لڙي_ڊيگهه(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		إذا (ret)
			اذهب fail;
		يادگيري_نقل(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(قدر_len, &new_قدر);
	إذا (ret)
		اذهب fail;
	يادگيري_نقل(new_قدر, قدر, قدر_len);

	إذا (اندريون_inode) {
		ret = xattr_create_ea_inode(fs, قدر, قدر_len, &ea_ino);
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
	x->name_اشاريو = اشاريو;

	إذا (x->قدر)
		ext2fs_free_mem(&x->قدر);
	x->قدر = new_قدر;
	x->قدر_len = قدر_len;
	x->ea_ino = ea_ino;
	أرجع 0;
fail:
	إذا (new_name)
		ext2fs_free_mem(&new_name);
	إذا (new_قدر)
		ext2fs_free_mem(&new_قدر);
	إذا (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	أرجع ret;
}

ساكن صحيح xattr_ڳوليو_position(بنية ext2_xattr *attrs, صحيح ڳڻپ,
			       ثابت حرف *shortname, صحيح name_idx)
{
	بنية ext2_xattr *x;
	صحيح i;
	صحيح shortname_len, x_shortname_len;

	shortname_len = لڙي_ڊيگهه(shortname);

	لكل (i = 0, x = attrs; i < ڳڻپ; i++, x++) {
		إذا (name_idx < x->name_اشاريو)
			اكسر;
		إذا (name_idx > x->name_اشاريو)
			استمر;

		x_shortname_len = لڙي_ڊيگهه(x->short_name);
		إذا (shortname_len < x_shortname_len)
			اكسر;
		إذا (shortname_len > x_shortname_len)
			استمر;

		إذا (memcmp(shortname, x->short_name, shortname_len) <= 0)
			اكسر;
	}
	أرجع i;
}

ساكن errcode_t xattr_array_update(بنية ext2_xattr_handle *h,
				    ثابت حرف *name,
				    ثابت فراغ *قدر, size_t قدر_len,
				    صحيح ibody_free, صحيح بلاڪ_free,
				    صحيح old_idx, صحيح اندريون_inode)
{
	بنية ext2_xattr tmp;
	صحيح جوڙيو_منزل_ibody;
	صحيح needed;
	صحيح name_len, name_idx = 0;
	ثابت حرف *shortname = name;
	صحيح new_idx;
	صحيح ret;

	ڳوليو_ea_اشاريو(name, &shortname, &name_idx);
	name_len = لڙي_ڊيگهه(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	إذا (!اندريون_inode)
		needed += EXT2_EXT_ATTR_سائيز(قدر_len);

	إذا (old_idx >= 0 && old_idx < h->ibody_ڳڻپ) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		إذا (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_سائيز(
						h->attrs[old_idx].قدر_len);
	}

	إذا (needed <= ibody_free) {
		إذا (old_idx < 0) {
			new_idx = h->ibody_ڳڻپ;
			جوڙيو_منزل_ibody = 1;
			اذهب جوڙيو_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_داخلا(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, قدر,
					 قدر_len, اندريون_inode);
		إذا (ret)
			أرجع ret;
		إذا (h->ibody_ڳڻپ <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_ڳڻپ + 1,
				h->attrs + h->ibody_ڳڻپ,
				(old_idx - h->ibody_ڳڻپ) * حجم(*h->attrs));
			h->attrs[h->ibody_ڳڻپ] = tmp;
			h->ibody_ڳڻپ++;
		}
		أرجع 0;
	}

	إذا (h->ibody_ڳڻپ <= old_idx) {
		بلاڪ_free += EXT2_EXT_ATTR_LEN(name_len);
		إذا (!h->attrs[old_idx].ea_ino)
			بلاڪ_free +=
				EXT2_EXT_ATTR_سائيز(h->attrs[old_idx].قدر_len);
	}

	إذا (needed > بلاڪ_free)
		أرجع EXT2_ET_EA_NO_SPACE;

	إذا (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_داخلا(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, قدر,
					 قدر_len, اندريون_inode);
		إذا (ret)
			أرجع ret;
		إذا (old_idx < h->ibody_ڳڻپ) {
			/*
			 * Move entry from ibody to the block. Note that
			 * entries in the block are sorted.
			 */
			new_idx = xattr_ڳوليو_position(h->attrs + h->ibody_ڳڻپ,
						      h->ڳڻپ - h->ibody_ڳڻپ,
						      shortname, name_idx);
			new_idx += h->ibody_ڳڻپ - 1;
			tmp = h->attrs[old_idx];
			memmove(h->attrs + old_idx, h->attrs + old_idx + 1,
				(new_idx - old_idx) * حجم(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_ڳڻپ--;
		}
		أرجع 0;
	}

	new_idx = xattr_ڳوليو_position(h->attrs + h->ibody_ڳڻپ,
				      h->ڳڻپ - h->ibody_ڳڻپ,
				      shortname, name_idx);
	new_idx += h->ibody_ڳڻپ;
	جوڙيو_منزل_ibody = 0;

جوڙيو_new:
	إذا (h->ڳڻپ == h->صلاحيت) {
		ret = ext2fs_xattrs_expand(h, 4);
		إذا (ret)
			أرجع ret;
	}

	ret = xattr_update_داخلا(h->fs, &h->attrs[h->ڳڻپ], name, shortname,
				 name_idx, قدر, قدر_len, اندريون_inode);
	إذا (ret)
		أرجع ret;

	tmp = h->attrs[h->ڳڻپ];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->ڳڻپ - new_idx)*حجم(*h->attrs));
	h->attrs[new_idx] = tmp;
	إذا (جوڙيو_منزل_ibody)
		h->ibody_ڳڻپ++;
	h->ڳڻپ++;
	أرجع 0;
}

ساكن صحيح space_استعمال_ٿيل(بنية ext2_xattr *attrs, صحيح ڳڻپ)
{
	صحيح ڪل = 0;
	بنية ext2_xattr *x;
	صحيح i, len;

	لكل (i = 0, x = attrs; i < ڳڻپ; i++, x++) {
		len = لڙي_ڊيگهه(x->short_name);
		ڪل += EXT2_EXT_ATTR_LEN(len);
		إذا (!x->ea_ino)
			ڪل += EXT2_EXT_ATTR_سائيز(x->قدر_len);
	}
	أرجع ڪل;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#وصف_ڪريو EXT4_XATTR_MIN_LARGE_EA_سائيز(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - حجم(بنية ext2_ext_attr_سرو) - 4)

errcode_t ext2fs_xattr_سيٽ(بنية ext2_xattr_handle *h,
			   ثابت حرف *name,
			   ثابت فراغ *قدر,
			   size_t قدر_len)
{
	ext2_filsys fs = h->fs;
	ثابت صحيح inode_سائيز = EXT2_INODE_سائيز(fs->super);
	بنية ext2_inode_large *inode = عدم;
	بنية ext2_xattr *x;
	حرف *new_قدر;
	صحيح ibody_free, بلاڪ_free;
	صحيح اندريون_inode = 0;
	صحيح old_idx = -1;
	صحيح extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(قدر_len, &new_قدر);
	إذا (ret)
		أرجع ret;
	إذا (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((strcmp(name, "system.posix_acl_default") == 0) ||
	     (strcmp(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_منزل_disk_بفر(قدر, قدر_len,
						       new_قدر, &قدر_len);
		إذا (ret)
			اذهب ٻاهريون;
	} وإلا إذا (قدر_len)
		يادگيري_نقل(new_قدر, قدر, قدر_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	لكل (x = h->attrs; x < h->attrs + h->ڳڻپ; x++) {
		إذا (!strcmp(x->name, name)) {
			إذا (!x->ea_ino && x->قدر_len == قدر_len &&
			    (!قدر_len ||
			     !memcmp(x->قدر, new_قدر, قدر_len))) {
				ret = 0;
				اذهب ٻاهريون;
			}
			old_idx = x - h->attrs;
			اكسر;
		}
	}

	ret = ext2fs_get_memzero(inode_سائيز, &inode);
	إذا (ret)
		اذهب ٻاهريون;
	ret = ext2fs_read_inode_ڀريل(fs, h->ino,
				     (بنية ext2_inode *)inode,
				     inode_سائيز);
	إذا (ret)
		اذهب ٻاهريون;
	إذا (inode_سائيز > EXT2_GOOD_OLD_INODE_سائيز) {
		extra_isize = inode->i_extra_isize;
		إذا (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			إذا (extra_isize == 0)
				extra_isize = حجم(__u32);
		}
		ibody_free = inode_سائيز - EXT2_GOOD_OLD_INODE_سائيز;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= حجم(__u32) * 2;
		ibody_free -= space_استعمال_ٿيل(h->attrs, h->ibody_ڳڻپ);
	} وإلا
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	إذا (strcmp(name, "system.data") == 0) {
		إذا (h->ibody_ڳڻپ <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			اذهب ٻاهريون;
		}
		ret = xattr_array_update(h, name, new_قدر, قدر_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		إذا (ret)
			اذهب ٻاهريون;
		اذهب write_ٻاهريون;
	}

	بلاڪ_free = fs->blocksize;
	بلاڪ_free -= حجم(بنية ext2_ext_attr_سرو);
	/* Final null entry. */
	بلاڪ_free -= حجم(__u32);
	بلاڪ_free -= space_استعمال_ٿيل(h->attrs + h->ibody_ڳڻپ,
				 h->ڳڻپ - h->ibody_ڳڻپ);

	إذا (ext2fs_has_feature_ea_inode(fs->super) &&
	    قدر_len > EXT4_XATTR_MIN_LARGE_EA_سائيز(fs->blocksize))
		اندريون_inode = 1;

	ret = xattr_array_update(h, name, new_قدر, قدر_len, ibody_free,
				 بلاڪ_free, old_idx, اندريون_inode);
	إذا (ret == EXT2_ET_EA_NO_SPACE && !اندريون_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_قدر, قدر_len,
			ibody_free, بلاڪ_free, old_idx, 1 /* in_inode */);
	إذا (ret)
		اذهب ٻاهريون;

write_ٻاهريون:
	ret = ext2fs_xattrs_write(h);
ٻاهريون:
	إذا (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_قدر);
	أرجع ret;
}

errcode_t ext2fs_xattr_remove(بنية ext2_xattr_handle *handle,
			      ثابت حرف *ڪنجي)
{
	بنية ext2_xattr *x;
	بنية ext2_xattr *پڇاڙي = handle->attrs + handle->ڳڻپ;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = handle->attrs; x < پڇاڙي; x++) {
		إذا (strcmp(x->name, ڪنجي) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->قدر);
			إذا (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (پڇاڙي - x - 1)*حجم(*x));
			memset(پڇاڙي - 1, 0, حجم(*پڇاڙي));
			إذا (x < handle->attrs + handle->ibody_ڳڻپ)
				handle->ibody_ڳڻپ--;
			handle->ڳڻپ--;
			أرجع ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	أرجع 0;
}

errcode_t ext2fs_xattr_remove_all(بنية ext2_xattr_handle *handle)
{
	بنية ext2_xattr *x;
	بنية ext2_xattr *پڇاڙي = handle->attrs + handle->ڳڻپ;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = handle->attrs; x < پڇاڙي; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->قدر);
		إذا (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_ڳڻپ = 0;
	handle->ڳڻپ = 0;
	أرجع ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_xattr_handle **handle)
{
	بنية ext2_xattr_handle *h;
	errcode_t err;

	إذا (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_معلومات(fs->super))
		أرجع EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(حجم(*h), &h);
	إذا (err)
		أرجع err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->صلاحيت = 4;
	err = ext2fs_get_arrayzero(h->صلاحيت, حجم(بنية ext2_xattr),
				   &h->attrs);
	إذا (err) {
		ext2fs_free_mem(&h);
		أرجع err;
	}
	h->ڳڻپ = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	أرجع 0;
}

errcode_t ext2fs_xattrs_close(بنية ext2_xattr_handle **handle)
{
	بنية ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_ڪنجيون(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	أرجع 0;
}

errcode_t ext2fs_xattrs_ڳڻپ(بنية ext2_xattr_handle *handle, size_t *ڳڻپ)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*ڳڻپ = handle->ڳڻپ;
	أرجع 0;
}

errcode_t ext2fs_xattrs_flags(بنية ext2_xattr_handle *handle,
			      بنانشان صحيح *new_flags, بنانشان صحيح *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	إذا (old_flags)
		*old_flags = handle->flags;
	إذا (new_flags)
		handle->flags = *new_flags;
	أرجع 0;
}
