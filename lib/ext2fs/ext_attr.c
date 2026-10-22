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
#اگر_ختم
#شامل <string.h>
#شامل <time.h>

#شامل "ext2_fs.h"
#شامل "ext2_ext_attr.h"
#شامل "ext4_acl.h"

#شامل "ext2fsP.h"

ساکن errcode_t read_ea_inode_hash(ext2_filsys fs, ext2_ino_t ino, __u32 *hash)
{
	ساخت ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	اگر (retval)
		واپس retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	واپس 0;
}

#تعریف_کرو NAME_HASH_سرکاؤ 5
#تعریف_کرو قدر_HASH_سرکاؤ 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_اندراج(ساخت ext2_ext_attr_اندراج *اندراج_2, خالی *ڈیٹا)
{
	__u32 hash = 0;
	غیرعلامتی حرف *name = (((غیرعلامتی حرف *) اندراج_2) +
			       حجم(ساخت ext2_ext_attr_اندراج));
	صحیح n;

	برائے (n = 0; n < اندراج_2->e_name_len; n++) {
		hash = (hash << NAME_HASH_سرکاؤ) ^
		       (hash >> (8*حجم(hash) - NAME_HASH_سرکاؤ)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	اگر (اندراج_2->e_قدر_inum == 0 && اندراج_2->e_قدر_سائز != 0) {
		__u32 *قدر = (__u32 *)ڈیٹا;
		برائے (n = (اندراج_2->e_قدر_سائز + EXT2_EXT_ATTR_باری) >>
			 EXT2_EXT_ATTR_PAD_بٹ; n; n--) {
			hash = (hash << قدر_HASH_سرکاؤ) ^
			       (hash >> (8*حجم(hash) - قدر_HASH_سرکاؤ)) ^
			       ext2fs_le32_مقصد_cpu(*قدر++);
		}
	}

	واپس hash;
}

__u32 ext2fs_ext_attr_hash_اندراج_signed(ساخت ext2_ext_attr_اندراج *اندراج_2,
					خالی *ڈیٹا)
{
	__u32 hash = 0;
	علامتی حرف *name = (((علامتی حرف *) اندراج_2) +
			     حجم(ساخت ext2_ext_attr_اندراج));
	صحیح n;

	برائے (n = 0; n < اندراج_2->e_name_len; n++) {
		hash = (hash << NAME_HASH_سرکاؤ) ^
		       (hash >> (8*حجم(hash) - NAME_HASH_سرکاؤ)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	اگر (اندراج_2->e_قدر_inum == 0 && اندراج_2->e_قدر_سائز != 0) {
		__u32 *قدر = (__u32 *)ڈیٹا;
		برائے (n = (اندراج_2->e_قدر_سائز + EXT2_EXT_ATTR_باری) >>
			 EXT2_EXT_ATTR_PAD_بٹ; n; n--) {
			hash = (hash << قدر_HASH_سرکاؤ) ^
			       (hash >> (8*حجم(hash) - قدر_HASH_سرکاؤ)) ^
			       ext2fs_le32_مقصد_cpu(*قدر++);
		}
	}

	واپس hash;
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
				      ساخت ext2_ext_attr_اندراج *اندراج_2,
				      خالی *ڈیٹا, __u32 *hash,
				      __u32 *signed_hash)
{
	*hash = ext2fs_ext_attr_hash_اندراج(اندراج_2, ڈیٹا);
	اگر (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_اندراج_signed(اندراج_2, ڈیٹا);

	اگر (اندراج_2->e_قدر_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, اندراج_2->e_قدر_inum,
					    &ea_inode_hash);
		اگر (retval)
			واپس retval;

		*hash = (*hash << قدر_HASH_سرکاؤ) ^
			(*hash >> (8*حجم(*hash) - قدر_HASH_سرکاؤ)) ^
			ea_inode_hash;
		اگر (signed_hash)
			*signed_hash = (*signed_hash << قدر_HASH_سرکاؤ) ^
				(*signed_hash >> (8*حجم(*hash) -
						  قدر_HASH_سرکاؤ)) ^
				ea_inode_hash;
	}
	واپس 0;
}

/*
 * ext2fs_ext_attr_hash_entry2()
 *
 * Compute the hash of an extended attribute.
 * This version of the function supports hashing entries that reference
 * external inodes (ea_inode feature).
 */
errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
				      ساخت ext2_ext_attr_اندراج *اندراج_2,
				      خالی *ڈیٹا, __u32 *hash)
{
	واپس ext2fs_ext_attr_hash_entry3(fs, اندراج_2, ڈیٹا, hash, تہی);
}

#تعریف_منسوخ NAME_HASH_سرکاؤ
#تعریف_منسوخ قدر_HASH_سرکاؤ

#تعریف_کرو بلاک_HASH_سرکاؤ 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
خالی ext2fs_ext_attr_بلاک_rehash(ساخت ext2_ext_attr_سرخی *سرخی,
				  ساخت ext2_ext_attr_اندراج *انتہا)
{
	ساخت ext2_ext_attr_اندراج *یہاں;
	__u32 hash = 0;

	یہاں = (ساخت ext2_ext_attr_اندراج *)(سرخی+1);
	جب (یہاں < انتہا && !EXT2_EXT_IS_آخری_اندراج(یہاں)) {
		اگر (!یہاں->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			توڑو;
		}
		hash = (hash << بلاک_HASH_سرکاؤ) ^
		       (hash >> (8*حجم(hash) - بلاک_HASH_سرکاؤ)) ^
		       یہاں->e_hash;
		یہاں = EXT2_EXT_ATTR_اگلا(یہاں);
	}
	سرخی->h_hash = hash;
}

#تعریف_منسوخ بلاک_HASH_سرکاؤ

__u32 ext2fs_get_ea_inode_hash(ساخت ext2_inode *inode)
{
	واپس inode->i_atime;
}

خالی ext2fs_سیٹ_ea_inode_hash(ساخت ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(ساخت ext2_inode *inode)
{
	واپس ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_نسخہ;
}

خالی ext2fs_سیٹ_ea_inode_ref(ساخت ext2_inode *inode, __u64 ref_گنتی)
{
	inode->i_ctime = (__u32)(ref_گنتی >> 32);
	inode->osd1.linux1.l_i_نسخہ = (__u32)ref_گنتی;
}

ساکن errcode_t check_ext_attr_سرخی(ساخت ext2_ext_attr_سرخی *سرخی)
{
	اگر ((سرخی->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     سرخی->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    سرخی->h_بلاکیں != 1)
		واپس EXT2_ET_BAD_EA_سرخی;

	واپس 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t بلاک, خالی *buf,
				ext2_ino_t inum)
{
	صحیح		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, بلاک, 1, buf);
	اگر (retval)
		واپس retval;

	اگر (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_بلاک_csum_verify(fs, inum, بلاک, buf))
		csum_failed = 1;

#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	ext2fs_تبادلہ_ext_attr(buf, buf, fs->blocksize, 1);
#اگر_ختم

	retval = check_ext_attr_سرخی(buf);
	اگر (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	واپس retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t بلاک, خالی *buf)
{
	واپس ext2fs_read_ext_attr3(fs, بلاک, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t بلاک, خالی *buf)
{
	واپس ext2fs_read_ext_attr2(fs, بلاک, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t بلاک, خالی *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	حرف		*write_buf;

#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	اگر (retval)
		واپس retval;
	ext2fs_تبادلہ_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#ورنہ
	write_buf = (حرف *) inbuf;
#اگر_ختم

	retval = ext2fs_ext_attr_بلاک_csum_سیٹ(fs, inum, بلاک,
			(ساخت ext2_ext_attr_سرخی *)write_buf);
	اگر (retval)
		واپس retval;

	retval = io_channel_write_blk64(fs->io, بلاک, 1, write_buf);
#اگر_تعریف_شدہ الفاظ_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#اگر_ختم
	اگر (!retval)
		ext2fs_mark_بدلا(fs);
	واپس retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t بلاک, خالی *inbuf)
{
	واپس ext2fs_write_ext_attr3(fs, بلاک, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t بلاک, خالی *inbuf)
{
	واپس ext2fs_write_ext_attr2(fs, بلاک, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    حرف *بلاک_buf, صحیح adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	ساخت ext2_ext_attr_سرخی *سرخی;
	حرف	*buf = 0;

	اگر ((blk >= ext2fs_بلاکیں_گنتی(fs->super)) ||
	    (blk < fs->super->s_پہلا_ڈیٹا_بلاک))
		واپس EXT2_ET_BAD_EA_بلاک_عدد;

	اگر (!بلاک_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		اگر (retval)
			واپس retval;
		بلاک_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, بلاک_buf, inum);
	اگر (retval)
		جاؤ errout;

	سرخی = (ساخت ext2_ext_attr_سرخی *) بلاک_buf;
	سرخی->h_refcount += adjust;
	اگر (newcount)
		*newcount = سرخی->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, بلاک_buf, inum);
	اگر (retval)
		جاؤ errout;

errout:
	اگر (buf)
		ext2fs_free_mem(&buf);
	واپس retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    حرف *بلاک_buf, صحیح adjust,
				    __u32 *newcount)
{
	واپس ext2fs_adjust_ea_refcount3(fs, blk, بلاک_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					حرف *بلاک_buf, صحیح adjust,
					__u32 *newcount)
{
	واپس ext2fs_adjust_ea_refcount2(fs, blk, بلاک_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
ساخت ext2_xattr {
	صحیح name_اشاریہ;
	حرف *name;
	حرف *short_name;
	خالی *قدر;
	غیرعلامتی صحیح قدر_len;
	ext2_ino_t ea_ino;
};

ساخت ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	ساخت ext2_xattr *attrs;
	صحیح استعداد;
	صحیح گنتی;
	صحیح ibody_گنتی;
	ext2_ino_t ino;
	غیرعلامتی صحیح flags;
};

ساکن errcode_t ext2fs_xattrs_expand(ساخت ext2_xattr_handle *h,
				      غیرعلامتی صحیح expandby)
{
	ساخت ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->استعداد + expandby,
				   حجم(ساخت ext2_xattr), &new_attrs);
	اگر (err)
		واپس err;

	حافظہ_نقل(new_attrs, h->attrs, h->استعداد * حجم(ساخت ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->استعداد += expandby;
	h->attrs = new_attrs;

	واپس 0;
}

ساخت ea_name_اشاریہ {
	صحیح اشاریہ;
	ثابت حرف *name;
};

/* Keep these names sorted in order of decreasing specificity. */
ساکن ساخت ea_name_اشاریہ ea_نام[] = {
	{10, "gnu."},
	{3, "system.posix_acl_default"},
	{2, "system.posix_acl_access"},
	{8, "system.richacl"},
	{6, "security."},
	{4, "trusted."},
	{7, "system."},
	{1, "user."},
	{0, تہی},
};

ساکن ثابت حرف *ڈھونڈو_ea_سابقہ(صحیح اشاریہ)
{
	ساخت ea_name_اشاریہ *e;

	برائے (e = ea_نام; e->name; e++)
		اگر (e->اشاریہ == اشاریہ)
			واپس e->name;

	واپس تہی;
}

ساکن صحیح ڈھونڈو_ea_اشاریہ(ثابت حرف *fullname, ثابت حرف **name, صحیح *اشاریہ)
{
	ساخت ea_name_اشاریہ *e;

	برائے (e = ea_نام; e->name; e++) {
		اگر (لڑی_موازنہ(fullname, e->name, لڑی_لمبائی(e->name)) == 0) {
			*name = fullname + لڑی_لمبائی(e->name);
			*اشاریہ = e->اشاریہ;
			واپس 1;
		}
	}
	واپس 0;
}

errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       ساخت ext2_inode_large *inode)
{
	ساخت ext2_ext_attr_سرخی *سرخی;
	خالی *بلاک_buf = تہی;
	blk64_t blk;
	errcode_t err;
	ساخت ext2_inode_large i;

	/* Read inode? */
	اگر (inode == تہی) {
		err = ext2fs_read_inode_بھرا(fs, ino, (ساخت ext2_inode *)&i,
					     حجم(ساخت ext2_inode_large));
		اگر (err)
			واپس err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_بلاک(fs, (ساخت ext2_inode *)inode);
	اگر (blk == 0)
		واپس 0;

	/* Find block, zero it, write back */
	اگر ((blk < fs->super->s_پہلا_ڈیٹا_بلاک) ||
	    (blk >= ext2fs_بلاکیں_گنتی(fs->super))) {
		err = EXT2_ET_BAD_EA_بلاک_عدد;
		جاؤ برآمد;
	}

	err = ext2fs_get_mem(fs->blocksize, &بلاک_buf);
	اگر (err)
		جاؤ برآمد;

	err = ext2fs_read_ext_attr3(fs, blk, بلاک_buf, ino);
	اگر (err)
		جاؤ out2;

	/* We only know how to deal with v2 EA blocks */
	سرخی = (ساخت ext2_ext_attr_سرخی *) بلاک_buf;
	اگر (سرخی->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_سرخی;
		جاؤ out2;
	}

	سرخی->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, بلاک_buf, ino);
	اگر (err)
		جاؤ out2;

	/* Erase link to block */
	ext2fs_file_acl_بلاک_سیٹ(fs, (ساخت ext2_inode *)inode, 0);
	اگر (سرخی->h_refcount == 0)
		ext2fs_بلاک_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_بلاکیں(fs, (ساخت ext2_inode *)inode, 1);
	اگر (err)
		جاؤ out2;

	/* Write inode? */
	اگر (inode == &i) {
		err = ext2fs_write_inode_بھرا(fs, ino, (ساخت ext2_inode *)&i,
					      حجم(ساخت ext2_inode_large));
		اگر (err)
			جاؤ out2;
	}

out2:
	ext2fs_free_mem(&بلاک_buf);
برآمد:
	واپس err;
}

ساکن errcode_t prep_ea_بلاک_for_write(ext2_filsys fs, ext2_ino_t ino,
					 ساخت ext2_inode_large *inode)
{
	ساخت ext2_ext_attr_سرخی *سرخی;
	خالی *بلاک_buf = تہی;
	blk64_t blk, goal;
	errcode_t err;

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_بلاک(fs, (ساخت ext2_inode *)inode);
	اگر (blk != 0) {
		اگر ((blk < fs->super->s_پہلا_ڈیٹا_بلاک) ||
		    (blk >= ext2fs_بلاکیں_گنتی(fs->super))) {
			err = EXT2_ET_BAD_EA_بلاک_عدد;
			جاؤ برآمد;
		}

		err = ext2fs_get_mem(fs->blocksize, &بلاک_buf);
		اگر (err)
			جاؤ برآمد;

		err = ext2fs_read_ext_attr3(fs, blk, بلاک_buf, ino);
		اگر (err)
			جاؤ out2;

		/* We only know how to deal with v2 EA blocks */
		سرخی = (ساخت ext2_ext_attr_سرخی *) بلاک_buf;
		اگر (سرخی->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_سرخی;
			جاؤ out2;
		}

		/* Single-user block.  We're done here. */
		اگر (سرخی->h_refcount == 1)
			جاؤ out2;

		/* We need to CoW the block. */
		سرخی->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, بلاک_buf, ino);
		اگر (err)
			جاؤ out2;
	} ورنہ {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_جوڑو_بلاکیں(fs, (ساخت ext2_inode *)inode,
					     1);
		اگر (err)
			جاؤ برآمد;
	}

	/* Allocate a block */
	goal = ext2fs_ڈھونڈو_inode_goal(fs, ino, (ساخت ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, تہی, &blk);
	اگر (err)
		جاؤ out2;
	ext2fs_file_acl_بلاک_سیٹ(fs, (ساخت ext2_inode *)inode, blk);
out2:
	اگر (بلاک_buf)
		ext2fs_free_mem(&بلاک_buf);
برآمد:
	واپس err;
}


ساکن اندرونی صحیح
posix_acl_xattr_گنتی(size_t سائز)
{
        اگر (سائز < حجم(posix_acl_xattr_سرخی))
                واپس -1;
        سائز -= حجم(posix_acl_xattr_سرخی);
        اگر (سائز % حجم(posix_acl_xattr_اندراج))
                واپس -1;
        واپس سائز / حجم(posix_acl_xattr_اندراج);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
ساکن errcode_t convert_posix_acl_مقصد_disk_بفر(ثابت خالی *قدر, size_t سائز,
						  خالی *برآمد_buf, size_t *سائز_برآمد)
{
	ثابت posix_acl_xattr_سرخی *سرخی =
		(ثابت posix_acl_xattr_سرخی*) قدر;
	ثابت posix_acl_xattr_اندراج *انتہا, *اندراج_2 =
		(ثابت posix_acl_xattr_اندراج *)(سرخی+1);
	ext4_acl_سرخی *ext_acl;
	size_t s;
	حرف *e;

	صحیح گنتی;

	اگر (!قدر)
		واپس EINVAL;
	اگر (سائز < حجم(posix_acl_xattr_سرخی))
		واپس ENOMEM;
	اگر (سرخی->a_نسخہ != ext2fs_cpu_مقصد_le32(POSIX_ACL_XATTR_نسخہ))
		واپس EINVAL;

	گنتی = posix_acl_xattr_گنتی(سائز);
	ext_acl = برآمد_buf;
	ext_acl->a_نسخہ = ext2fs_cpu_مقصد_le32(EXT4_ACL_نسخہ);

	اگر (گنتی <= 0)
		واپس EINVAL;

	e = (حرف *) برآمد_buf + حجم(ext4_acl_سرخی);
	s = حجم(ext4_acl_سرخی);
	برائے (انتہا = اندراج_2 + گنتی; اندراج_2 != انتہا;اندراج_2++) {
		ext4_acl_اندراج *disk_اندراج = (ext4_acl_اندراج*) e;
		disk_اندراج->e_لیبل = اندراج_2->e_لیبل;
		disk_اندراج->e_perm = اندراج_2->e_perm;

		انتخاب(ext2fs_le16_مقصد_cpu(اندراج_2->e_لیبل)) {
			حالت ACL_USER_OBJ:
			حالت ACL_گروہ_OBJ:
			حالت ACL_نقاب:
			حالت ACL_OTHER:
				e += حجم(ext4_acl_اندراج_short);
				s += حجم(ext4_acl_اندراج_short);
				توڑو;
			حالت ACL_USER:
			حالت ACL_گروہ:
				disk_اندراج->e_id = اندراج_2->e_id;
				e += حجم(ext4_acl_اندراج);
				s += حجم(ext4_acl_اندراج);
				توڑو;
			طےشدہ:
				واپس EINVAL;
		}
	}
	*سائز_برآمد = s;
	واپس 0;
}

ساکن errcode_t convert_disk_بفر_مقصد_posix_acl(ثابت خالی *قدر, size_t سائز,
						  خالی **برآمد_buf, size_t *سائز_برآمد)
{
	posix_acl_xattr_سرخی *سرخی;
	posix_acl_xattr_اندراج *اندراج_2;
	ثابت ext4_acl_سرخی *ext_acl = (ثابت ext4_acl_سرخی *) قدر;
	errcode_t err;
	ثابت حرف *cp;
	حرف *برآمد;

	اگر ((!قدر) ||
	    (سائز < حجم(ext4_acl_سرخی)) ||
	    (ext_acl->a_نسخہ != ext2fs_cpu_مقصد_le32(EXT4_ACL_نسخہ)))
		واپس EINVAL;

	err = ext2fs_get_mem(سائز * 2, &برآمد);
	اگر (err)
		واپس err;

	سرخی = (posix_acl_xattr_سرخی *) برآمد;
	سرخی->a_نسخہ = ext2fs_cpu_مقصد_le32(POSIX_ACL_XATTR_نسخہ);
	اندراج_2 = (posix_acl_xattr_اندراج *) (برآمد + حجم(posix_acl_xattr_سرخی));

	cp = (ثابت حرف *) قدر + حجم(ext4_acl_سرخی);
	سائز -= حجم(ext4_acl_سرخی);

	جب (سائز > 0) {
		ثابت ext4_acl_اندراج *disk_اندراج = (ثابت ext4_acl_اندراج *) cp;

		اندراج_2->e_لیبل = disk_اندراج->e_لیبل;
		اندراج_2->e_perm = disk_اندراج->e_perm;

		انتخاب(ext2fs_le16_مقصد_cpu(اندراج_2->e_لیبل)) {
			حالت ACL_USER_OBJ:
			حالت ACL_گروہ_OBJ:
			حالت ACL_نقاب:
			حالت ACL_OTHER:
				اندراج_2->e_id = 0;
				cp += حجم(ext4_acl_اندراج_short);
				سائز -= حجم(ext4_acl_اندراج_short);
				توڑو;
			حالت ACL_USER:
			حالت ACL_گروہ:
				اندراج_2->e_id = disk_اندراج->e_id;
				cp += حجم(ext4_acl_اندراج);
				سائز -= حجم(ext4_acl_اندراج);
				توڑو;
			طےشدہ:
				ext2fs_free_mem(&برآمد);
				واپس EINVAL;
		}
		اندراج_2++;
	}
	*برآمد_buf = برآمد;
	*سائز_برآمد = ((حرف *) اندراج_2 - برآمد);
	واپس 0;
}

ساکن errcode_t
write_xattrs_مقصد_بفر(ext2_filsys fs, ساخت ext2_xattr *attrs, صحیح گنتی,
		       خالی *entries_آغاز, غیرعلامتی صحیح storage_سائز,
		       غیرعلامتی صحیح قدر_انحراف_correction, صحیح write_hash)
{
	ساخت ext2_xattr *x;
	ساخت ext2_ext_attr_اندراج *e = entries_آغاز;
	حرف *انتہا = (حرف *) entries_آغاز + storage_سائز;
	غیرعلامتی صحیح قدر_سائز;
	errcode_t err;

	memset(entries_آغاز, 0, storage_سائز);
	برائے (x = attrs; x < attrs + گنتی; x++) {
		قدر_سائز = ((x->قدر_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = لڑی_لمبائی(x->short_name);
		e->e_name_اشاریہ = x->name_اشاریہ;

		e->e_قدر_سائز = x->قدر_len;
		e->e_قدر_inum = x->ea_ino;

		/* Store name */
		حافظہ_نقل((حرف *)e + حجم(*e), x->short_name, e->e_name_len);
		اگر (x->ea_ino) {
			e->e_قدر_offs = 0;
		} ورنہ {
			انتہا -= قدر_سائز;
			e->e_قدر_offs = انتہا - (حرف *) entries_آغاز +
						قدر_انحراف_correction;
			حافظہ_نقل(انتہا, x->قدر, e->e_قدر_سائز);
		}

		اگر (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : انتہا,
							  &e->e_hash);
			اگر (err)
				واپس err;
		} ورنہ
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_اگلا(e);
		*(__u32 *)e = 0;
	}
	واپس 0;
}

errcode_t ext2fs_xattrs_write(ساخت ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	ثابت غیرعلامتی صحیح inode_سائز = EXT2_INODE_سائز(fs->super);
	ساخت ext2_inode_large *inode;
	حرف *آغاز, *بلاک_buf = تہی;
	ساخت ext2_ext_attr_سرخی *سرخی;
	__u32 ea_inode_magic;
	blk64_t blk;
	غیرعلامتی صحیح storage_سائز;
	غیرعلامتی صحیح i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_سائز;
	اگر (i < حجم(*inode))
		i = حجم(*inode);
	err = ext2fs_get_memzero(i, &inode);
	اگر (err)
		واپس err;

	err = ext2fs_read_inode_بھرا(fs, handle->ino, EXT2_INODE(inode),
				     inode_سائز);
	اگر (err)
		جاؤ برآمد;

	/* If extra_isize isn't set, we need to set it now */
	اگر (inode->i_extra_isize == 0 &&
	    inode_سائز > EXT2_GOOD_OLD_INODE_سائز) {
		حرف *p = (حرف *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		اگر (extra == 0)
			extra = حجم(__u32);
		memset(p + EXT2_GOOD_OLD_INODE_سائز, 0, extra);
		inode->i_extra_isize = extra;
	}
	اگر (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		جاؤ برآمد;
	}

	/* Does the inode have space for EA? */
	اگر (inode->i_extra_isize < حجم(inode->i_extra_isize) ||
	    inode_سائز <= EXT2_GOOD_OLD_INODE_سائز + inode->i_extra_isize +
								حجم(__u32))
		جاؤ write_ea_بلاک;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	حافظہ_نقل(((حرف *) inode) + EXT2_GOOD_OLD_INODE_سائز +
	       inode->i_extra_isize, &ea_inode_magic, حجم(__u32));
	storage_سائز = inode_سائز - EXT2_GOOD_OLD_INODE_سائز -
				inode->i_extra_isize - حجم(__u32);
	آغاز = ((حرف *) inode) + EXT2_GOOD_OLD_INODE_سائز +
				inode->i_extra_isize + حجم(__u32);

	err = write_xattrs_مقصد_بفر(fs, handle->attrs, handle->ibody_گنتی,
				     آغاز, storage_سائز, 0, 0);
	اگر (err)
		جاؤ برآمد;
write_ea_بلاک:
	/* Are we done? */
	اگر (handle->ibody_گنتی == handle->گنتی &&
	    !ext2fs_file_acl_بلاک(fs, EXT2_INODE(inode)))
		جاؤ skip_ea_بلاک;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &بلاک_buf);
	اگر (err)
		جاؤ برآمد;

	storage_سائز = fs->blocksize - حجم(ساخت ext2_ext_attr_سرخی);
	آغاز = بلاک_buf + حجم(ساخت ext2_ext_attr_سرخی);

	err = write_xattrs_مقصد_بفر(fs, handle->attrs + handle->ibody_گنتی,
				     handle->گنتی - handle->ibody_گنتی, آغاز,
				     storage_سائز, آغاز - بلاک_buf, 1);
	اگر (err)
		جاؤ out2;

	/* Write a header on the EA block */
	سرخی = (ساخت ext2_ext_attr_سرخی *) بلاک_buf;
	سرخی->h_magic = EXT2_EXT_ATTR_MAGIC;
	سرخی->h_refcount = 1;
	سرخی->h_بلاکیں = 1;

	/* Get a new block for writing */
	err = prep_ea_بلاک_for_write(fs, handle->ino, inode);
	اگر (err)
		جاؤ out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_بلاک(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, بلاک_buf, handle->ino);
	اگر (err)
		جاؤ out2;

skip_ea_بلاک:
	blk = ext2fs_file_acl_بلاک(fs, (ساخت ext2_inode *)inode);
	اگر (!بلاک_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		اگر (err)
			جاؤ برآمد;
	}

	/* Write the inode */
	err = ext2fs_write_inode_بھرا(fs, handle->ino, EXT2_INODE(inode),
				      inode_سائز);
	اگر (err)
		جاؤ out2;

out2:
	ext2fs_free_mem(&بلاک_buf);
برآمد:
	ext2fs_free_mem(&inode);
	واپس err;
}

ساکن errcode_t read_xattrs_from_بفر(ساخت ext2_xattr_handle *handle,
					 ساخت ext2_inode_large *inode,
					 ساخت ext2_ext_attr_اندراج *entries,
					 غیرعلامتی صحیح storage_سائز,
					 حرف *قدر_آغاز)
{
	ساخت ext2_xattr *x;
	ساخت ext2_ext_attr_اندراج *اندراج_2, *انتہا;
	ثابت حرف *سابقہ;
	غیرعلامتی صحیح remain, سابقہ_len;
	errcode_t err;
	غیرعلامتی صحیح اقدار_سائز = storage_سائز +
			((حرف *)entries - قدر_آغاز);

	/* find the end */
	انتہا = entries;
	remain = storage_سائز;
	جب (remain >= حجم(ساخت ext2_ext_attr_اندراج) &&
	       !EXT2_EXT_IS_آخری_اندراج(انتہا)) {

		/* header eats this space */
		remain -= حجم(ساخت ext2_ext_attr_اندراج);

		/* is attribute name valid? */
		اگر (EXT2_EXT_ATTR_سائز(انتہا->e_name_len) > remain)
			واپس EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_سائز(انتہا->e_name_len);
		انتہا = EXT2_EXT_ATTR_اگلا(انتہا);
	}

	اندراج_2 = entries;
	remain = storage_سائز;
	جب (remain >= حجم(ساخت ext2_ext_attr_اندراج) &&
	       !EXT2_EXT_IS_آخری_اندراج(اندراج_2)) {

		/* Allocate space for more attrs? */
		اگر (handle->گنتی == handle->استعداد) {
			err = ext2fs_xattrs_expand(handle, 4);
			اگر (err)
				واپس err;
		}

		x = handle->attrs + handle->گنتی;

		/* header eats this space */
		remain -= حجم(ساخت ext2_ext_attr_اندراج);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_سائز(اندراج_2->e_name_len);

		/* Extract name */
		سابقہ = ڈھونڈو_ea_سابقہ(اندراج_2->e_name_اشاریہ);
		سابقہ_len = (سابقہ ? لڑی_لمبائی(سابقہ) : 0);
		err = ext2fs_get_memzero(اندراج_2->e_name_len + سابقہ_len + 1,
					 &x->name);
		اگر (err)
			واپس err;
		اگر (سابقہ)
			حافظہ_نقل(x->name, سابقہ, سابقہ_len);
		اگر (اندراج_2->e_name_len)
			حافظہ_نقل(x->name + سابقہ_len,
			       (حرف *)اندراج_2 + حجم(*اندراج_2),
			       اندراج_2->e_name_len);
		x->short_name = x->name + سابقہ_len;
		x->name_اشاریہ = اندراج_2->e_name_اشاریہ;

		/* Check & copy value */
		اگر (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    اندراج_2->e_قدر_inum != 0)
			واپس EXT2_ET_BAD_EA_بلاک_عدد;

		اگر (اندراج_2->e_قدر_inum == 0) {
			اگر (اندراج_2->e_قدر_سائز > remain)
				واپس EXT2_ET_EA_BAD_قدر_سائز;

			اگر (اندراج_2->e_قدر_offs + اندراج_2->e_قدر_سائز > اقدار_سائز)
				واپس EXT2_ET_EA_BAD_قدر_انحراف;

			اگر (اندراج_2->e_قدر_سائز > 0 &&
			    قدر_آغاز + اندراج_2->e_قدر_offs <
			    (حرف *)انتہا + حجم(__u32))
				واپس EXT2_ET_EA_BAD_قدر_انحراف;

			remain -= اندراج_2->e_قدر_سائز;

			err = ext2fs_get_mem(اندراج_2->e_قدر_سائز, &x->قدر);
			اگر (err)
				واپس err;
			حافظہ_نقل(x->قدر, قدر_آغاز + اندراج_2->e_قدر_offs,
			       اندراج_2->e_قدر_سائز);
		} ورنہ {
			ساخت ext2_inode *ea_inode;
			ext2_file_t ea_file;

			اگر (اندراج_2->e_قدر_offs != 0)
				واپس EXT2_ET_EA_BAD_قدر_انحراف;

			اگر (اندراج_2->e_قدر_سائز > (64 * 1024))
				واپس EXT2_ET_EA_BAD_قدر_سائز;

			err = ext2fs_get_mem(اندراج_2->e_قدر_سائز, &x->قدر);
			اگر (err)
				واپس err;

			err = ext2fs_file_open(handle->fs, اندراج_2->e_قدر_inum,
					       0, &ea_file);
			اگر (err)
				واپس err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			اگر ((ea_inode->i_flags & EXT4_INLINE_ڈیٹا_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_گنتی == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			ورنہ اگر ((__u64) ext2fs_file_get_سائز(ea_file) !=
				 اندراج_2->e_قدر_سائز)
				err = EXT2_ET_EA_BAD_قدر_سائز;
			ورنہ
				err = ext2fs_file_read(ea_file, x->قدر,
						       اندراج_2->e_قدر_سائز, 0);
			ext2fs_file_close(ea_file);
			اگر (err)
				واپس err;
		}

		x->ea_ino = اندراج_2->e_قدر_inum;
		x->قدر_len = اندراج_2->e_قدر_سائز;

		/* e_hash may be 0 in older inode's ea */
		اگر (اندراج_2->e_hash != 0) {
			__u32 hash, signed_hash;

			خالی *ڈیٹا = (اندراج_2->e_قدر_inum != 0) ?
					0 : قدر_آغاز + اندراج_2->e_قدر_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, اندراج_2,
							  ڈیٹا, &hash,
							  &signed_hash);
			اگر (err)
				واپس err;
			اگر ((اندراج_2->e_hash != hash) &&
			    (اندراج_2->e_hash != signed_hash)) {
				ساخت ext2_inode بچہ;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							اندراج_2->e_قدر_inum,
							&بچہ);
				اگر (err)
					واپس err;
				اگر (بچہ.i_mtime != handle->ino ||
				    بچہ.i_generation != inode->i_generation)
					واپس EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->گنتی++;
		اندراج_2 = EXT2_EXT_ATTR_اگلا(اندراج_2);
	}

	واپس 0;
}

ساکن خالی xattrs_free_کلیدیں(ساخت ext2_xattr_handle *h)
{
	ساخت ext2_xattr *a = h->attrs;
	صحیح i;

	برائے (i = 0; i < h->استعداد; i++) {
		اگر (a[i].name)
			ext2fs_free_mem(&a[i].name);
		اگر (a[i].قدر)
			ext2fs_free_mem(&a[i].قدر);
	}
	h->گنتی = 0;
	h->ibody_گنتی = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(ساخت ext2_xattr_handle *handle,
				   ساخت ext2_inode_large *inode)
{
	ساخت ext2_ext_attr_سرخی *سرخی;
	__u32 ea_inode_magic;
	غیرعلامتی صحیح storage_سائز;
	حرف *آغاز, *بلاک_buf = تہی;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_کلیدیں(handle);

	/* Does the inode have space for EA? */
	اگر (inode->i_extra_isize < حجم(inode->i_extra_isize) ||
	    EXT2_INODE_سائز(handle->fs->super) <= EXT2_GOOD_OLD_INODE_سائز +
						  inode->i_extra_isize +
						  حجم(__u32))
		جاؤ read_ea_بلاک;
	اگر (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		جاؤ برآمد;
	}

	/* Look for EA in the inode */
	حافظہ_نقل(&ea_inode_magic, ((حرف *) inode) + EXT2_GOOD_OLD_INODE_سائز +
	       inode->i_extra_isize, حجم(__u32));
	اگر (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_سائز = EXT2_INODE_سائز(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_سائز - inode->i_extra_isize -
			حجم(__u32);
		آغاز = ((حرف *) inode) + EXT2_GOOD_OLD_INODE_سائز +
			inode->i_extra_isize + حجم(__u32);

		err = read_xattrs_from_بفر(handle, inode,
					(ساخت ext2_ext_attr_اندراج *) آغاز,
					storage_سائز, آغاز);
		اگر (err)
			جاؤ برآمد;

		handle->ibody_گنتی = handle->گنتی;
	}

read_ea_بلاک:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_بلاک(handle->fs, EXT2_INODE(inode));
	اگر (blk != 0) {
		اگر ((blk < handle->fs->super->s_پہلا_ڈیٹا_بلاک) ||
		    (blk >= ext2fs_بلاکیں_گنتی(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_بلاک_عدد;
			جاؤ برآمد;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &بلاک_buf);
		اگر (err)
			جاؤ برآمد;

		err = ext2fs_read_ext_attr3(handle->fs, blk, بلاک_buf,
					    handle->ino);
		اگر (err)
			جاؤ out3;

		/* We only know how to deal with v2 EA blocks */
		سرخی = (ساخت ext2_ext_attr_سرخی *) بلاک_buf;
		اگر (سرخی->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_سرخی;
			جاؤ out3;
		}

		/* Read EAs */
		storage_سائز = handle->fs->blocksize -
			حجم(ساخت ext2_ext_attr_سرخی);
		آغاز = بلاک_buf + حجم(ساخت ext2_ext_attr_سرخی);
		err = read_xattrs_from_بفر(handle, inode,
					(ساخت ext2_ext_attr_اندراج *) آغاز,
					storage_سائز, بلاک_buf);
	}

out3:
	اگر (بلاک_buf)
		ext2fs_free_mem(&بلاک_buf);
برآمد:
	واپس err;
}

errcode_t ext2fs_xattrs_read(ساخت ext2_xattr_handle *handle)
{
	ساخت ext2_inode_large *inode;
	size_t inode_سائز = EXT2_INODE_سائز(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	اگر (inode_سائز < حجم(*inode))
		inode_سائز = حجم(*inode);
	err = ext2fs_get_memzero(inode_سائز, &inode);
	اگر (err)
		واپس err;

	err = ext2fs_read_inode_بھرا(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_سائز(handle->fs->super));
	اگر (err)
		جاؤ برآمد;

	err = ext2fs_xattrs_read_inode(handle, inode);

برآمد:
	ext2fs_free_mem(&inode);

	واپس err;
}

errcode_t ext2fs_xattrs_iterate(ساخت ext2_xattr_handle *h,
				صحیح (*func)(حرف *name, حرف *قدر,
					    size_t قدر_len, خالی *ڈیٹا),
				خالی *ڈیٹا)
{
	ساخت ext2_xattr *x;
	صحیح dirty = 0;
	صحیح ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	برائے (x = h->attrs; x < h->attrs + h->گنتی; x++) {
		ret = func(x->name, x->قدر, x->قدر_len, ڈیٹا);
		اگر (ret & XATTR_بدلا)
			dirty = 1;
		اگر (ret & XATTR_ABORT)
			توڑو;
	}

	اگر (dirty)
		واپس ext2fs_xattrs_write(h);
	واپس 0;
}

errcode_t ext2fs_xattr_get(ساخت ext2_xattr_handle *h, ثابت حرف *کلید,
			   خالی **قدر, size_t *قدر_len)
{
	ساخت ext2_xattr *x;
	حرف *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	برائے (x = h->attrs; x < h->attrs + h->گنتی; x++) {
		اگر (strcmp(x->name, کلید))
			جاری;

		اگر (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(کلید, "system.posix_acl_default") == 0) ||
		     (strcmp(کلید, "system.posix_acl_access") == 0))) {
			err = convert_disk_بفر_مقصد_posix_acl(x->قدر, x->قدر_len,
							       قدر, قدر_len);
			واپس err;
		} ورنہ {
			err = ext2fs_get_mem(x->قدر_len, &val);
			اگر (err)
				واپس err;
			حافظہ_نقل(val, x->قدر, x->قدر_len);
			*قدر = val;
			*قدر_len = x->قدر_len;
			واپس 0;
		}
	}

	واپس EXT2_ET_EA_کلید_NOT_ملا;
}

errcode_t ext2fs_xattr_inode_max_سائز(ext2_filsys fs, ext2_ino_t ino,
				      size_t *سائز)
{
	ساخت ext2_ext_attr_اندراج *اندراج_2;
	ساخت ext2_inode_large *inode;
	__u32 ea_inode_magic;
	غیرعلامتی صحیح minoff;
	حرف *آغاز;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_سائز(fs->super);
	اگر (i < حجم(*inode))
		i = حجم(*inode);
	err = ext2fs_get_memzero(i, &inode);
	اگر (err)
		واپس err;

	err = ext2fs_read_inode_بھرا(fs, ino, (ساخت ext2_inode *)inode,
				     EXT2_INODE_سائز(fs->super));
	اگر (err)
		جاؤ برآمد;

	/* Does the inode have size for EA? */
	اگر (EXT2_INODE_سائز(fs->super) <= EXT2_GOOD_OLD_INODE_سائز +
						  inode->i_extra_isize +
						  حجم(__u32)) {
		err = EXT2_ET_INLINE_ڈیٹا_NO_SPACE;
		جاؤ برآمد;
	}

	minoff = EXT2_INODE_سائز(fs->super) - حجم(*inode) - حجم(__u32);
	حافظہ_نقل(&ea_inode_magic, ((حرف *) inode) + EXT2_GOOD_OLD_INODE_سائز +
	       inode->i_extra_isize, حجم(__u32));
	اگر (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		آغاز= ((حرف *) inode) + EXT2_GOOD_OLD_INODE_سائز +
			inode->i_extra_isize + حجم(__u32);
		اندراج_2 = (ساخت ext2_ext_attr_اندراج *) آغاز;
		جب (!EXT2_EXT_IS_آخری_اندراج(اندراج_2)) {
			اگر (!اندراج_2->e_قدر_inum && اندراج_2->e_قدر_سائز) {
				غیرعلامتی صحیح offs = اندراج_2->e_قدر_offs;
				اگر (offs < minoff)
					minoff = offs;
			}
			اندراج_2 = EXT2_EXT_ATTR_اگلا(اندراج_2);
		}
		*سائز = minoff - ((حرف *)اندراج_2 - (حرف *)آغاز) - حجم(__u32);
	} ورنہ {
		/* no xattr.  return a maximum size */
		*سائز = EXT2_EXT_ATTR_سائز(minoff -
					   EXT2_EXT_ATTR_LEN(لڑی_لمبائی("data")) -
					   EXT2_EXT_ATTR_باری - حجم(__u32));
	}

برآمد:
	ext2fs_free_mem(&inode);
	واپس err;
}

ساکن errcode_t xattr_create_ea_inode(ext2_filsys fs, ثابت خالی *قدر,
				       size_t قدر_len, ext2_ino_t *ea_ino)
{
	ساخت ext2_inode inode;
	ext2_ino_t ino;
	ext2_file_t file;
	__u32 hash;
	errcode_t ret;

	ret = ext2fs_new_inode(fs, 0, 0, 0, &ino);
	اگر (ret)
		واپس ret;

	memset(&inode, 0, حجم(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	اگر (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_سائز = 0;
	inode.i_منوال = LINUX_S_IFREG | 0600;
	inode.i_links_گنتی = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	اگر (ret)
		واپس ret;
	/*
	 * ref_count and hash utilize inode's i_*time fields.
	 * ext2fs_write_new_inode() call above initializes these fields with
	 * current time. That's why ref count and hash updates are done
	 * separately below.
	 */
	ext2fs_سیٹ_ea_inode_ref(&inode, 1);
	hash = ext2fs_crc32c_le(fs->csum_بیج, قدر, قدر_len);
	ext2fs_سیٹ_ea_inode_hash(&inode, hash);

	ret = ext2fs_write_inode(fs, ino, &inode);
	اگر (ret)
		واپس ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	اگر (ret)
		واپس ret;
	ret = ext2fs_file_write(file, قدر, قدر_len, تہی);
	ext2fs_file_close(file);
	اگر (ret)
		واپس ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	واپس 0;
}

ساکن errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	ساخت ext2_inode_large inode;
	__u64 ref_گنتی;
	errcode_t ret;

	ret = ext2fs_read_inode_بھرا(fs, ino, (ساخت ext2_inode *)&inode,
				     حجم(inode));
	اگر (ret)
		جاؤ برآمد;

	ref_گنتی = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_گنتی--;
	ext2fs_سیٹ_ea_inode_ref(EXT2_INODE(&inode), ref_گنتی);

	اگر (ref_گنتی)
		جاؤ write_برآمد;

	inode.i_links_گنتی = 0;
	ext2fs_سیٹ_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	اگر (ret)
		جاؤ write_برآمد;

	اگر (ext2fs_inode_has_valid_blocks2(fs, (ساخت ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (ساخت ext2_inode *)&inode, تہی,
				   0, ~0ULL);
		اگر (ret)
			جاؤ برآمد;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_برآمد:
	ret = ext2fs_write_inode_بھرا(fs, ino, (ساخت ext2_inode *)&inode,
				      حجم(inode));
برآمد:
	واپس ret;
}

ساکن errcode_t xattr_update_اندراج(ext2_filsys fs, ساخت ext2_xattr *x,
				    ثابت حرف *name, ثابت حرف *short_name,
				    صحیح اشاریہ, ثابت خالی *قدر,
				    size_t قدر_len, صحیح اندر_inode)
{
	ext2_ino_t ea_ino = 0;
	خالی *new_قدر = تہی;
	حرف *new_name = تہی;
	صحیح name_len;
	errcode_t ret;

	اگر (!x->name) {
		name_len = لڑی_لمبائی(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		اگر (ret)
			جاؤ fail;
		حافظہ_نقل(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(قدر_len, &new_قدر);
	اگر (ret)
		جاؤ fail;
	حافظہ_نقل(new_قدر, قدر, قدر_len);

	اگر (اندر_inode) {
		ret = xattr_create_ea_inode(fs, قدر, قدر_len, &ea_ino);
		اگر (ret)
			جاؤ fail;
	}

	اگر (x->ea_ino) {
		ret = xattr_inode_dec_ref(fs, x->ea_ino);
		اگر (ret)
			جاؤ fail;
	}

	اگر (!x->name) {
		x->name = new_name;
		x->short_name = new_name + (short_name  - name);
	}
	x->name_اشاریہ = اشاریہ;

	اگر (x->قدر)
		ext2fs_free_mem(&x->قدر);
	x->قدر = new_قدر;
	x->قدر_len = قدر_len;
	x->ea_ino = ea_ino;
	واپس 0;
fail:
	اگر (new_name)
		ext2fs_free_mem(&new_name);
	اگر (new_قدر)
		ext2fs_free_mem(&new_قدر);
	اگر (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	واپس ret;
}

ساکن صحیح xattr_ڈھونڈو_position(ساخت ext2_xattr *attrs, صحیح گنتی,
			       ثابت حرف *shortname, صحیح name_idx)
{
	ساخت ext2_xattr *x;
	صحیح i;
	صحیح shortname_len, x_shortname_len;

	shortname_len = لڑی_لمبائی(shortname);

	برائے (i = 0, x = attrs; i < گنتی; i++, x++) {
		اگر (name_idx < x->name_اشاریہ)
			توڑو;
		اگر (name_idx > x->name_اشاریہ)
			جاری;

		x_shortname_len = لڑی_لمبائی(x->short_name);
		اگر (shortname_len < x_shortname_len)
			توڑو;
		اگر (shortname_len > x_shortname_len)
			جاری;

		اگر (memcmp(shortname, x->short_name, shortname_len) <= 0)
			توڑو;
	}
	واپس i;
}

ساکن errcode_t xattr_array_update(ساخت ext2_xattr_handle *h,
				    ثابت حرف *name,
				    ثابت خالی *قدر, size_t قدر_len,
				    صحیح ibody_free, صحیح بلاک_free,
				    صحیح old_idx, صحیح اندر_inode)
{
	ساخت ext2_xattr tmp;
	صحیح جوڑو_مقصد_ibody;
	صحیح needed;
	صحیح name_len, name_idx = 0;
	ثابت حرف *shortname = name;
	صحیح new_idx;
	صحیح ret;

	ڈھونڈو_ea_اشاریہ(name, &shortname, &name_idx);
	name_len = لڑی_لمبائی(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	اگر (!اندر_inode)
		needed += EXT2_EXT_ATTR_سائز(قدر_len);

	اگر (old_idx >= 0 && old_idx < h->ibody_گنتی) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		اگر (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_سائز(
						h->attrs[old_idx].قدر_len);
	}

	اگر (needed <= ibody_free) {
		اگر (old_idx < 0) {
			new_idx = h->ibody_گنتی;
			جوڑو_مقصد_ibody = 1;
			جاؤ جوڑو_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_اندراج(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, قدر,
					 قدر_len, اندر_inode);
		اگر (ret)
			واپس ret;
		اگر (h->ibody_گنتی <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_گنتی + 1,
				h->attrs + h->ibody_گنتی,
				(old_idx - h->ibody_گنتی) * حجم(*h->attrs));
			h->attrs[h->ibody_گنتی] = tmp;
			h->ibody_گنتی++;
		}
		واپس 0;
	}

	اگر (h->ibody_گنتی <= old_idx) {
		بلاک_free += EXT2_EXT_ATTR_LEN(name_len);
		اگر (!h->attrs[old_idx].ea_ino)
			بلاک_free +=
				EXT2_EXT_ATTR_سائز(h->attrs[old_idx].قدر_len);
	}

	اگر (needed > بلاک_free)
		واپس EXT2_ET_EA_NO_SPACE;

	اگر (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_اندراج(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, قدر,
					 قدر_len, اندر_inode);
		اگر (ret)
			واپس ret;
		اگر (old_idx < h->ibody_گنتی) {
			/*
			 * Move entry from ibody to the block. Note that
			 * entries in the block are sorted.
			 */
			new_idx = xattr_ڈھونڈو_position(h->attrs + h->ibody_گنتی,
						      h->گنتی - h->ibody_گنتی,
						      shortname, name_idx);
			new_idx += h->ibody_گنتی - 1;
			tmp = h->attrs[old_idx];
			memmove(h->attrs + old_idx, h->attrs + old_idx + 1,
				(new_idx - old_idx) * حجم(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_گنتی--;
		}
		واپس 0;
	}

	new_idx = xattr_ڈھونڈو_position(h->attrs + h->ibody_گنتی,
				      h->گنتی - h->ibody_گنتی,
				      shortname, name_idx);
	new_idx += h->ibody_گنتی;
	جوڑو_مقصد_ibody = 0;

جوڑو_new:
	اگر (h->گنتی == h->استعداد) {
		ret = ext2fs_xattrs_expand(h, 4);
		اگر (ret)
			واپس ret;
	}

	ret = xattr_update_اندراج(h->fs, &h->attrs[h->گنتی], name, shortname,
				 name_idx, قدر, قدر_len, اندر_inode);
	اگر (ret)
		واپس ret;

	tmp = h->attrs[h->گنتی];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->گنتی - new_idx)*حجم(*h->attrs));
	h->attrs[new_idx] = tmp;
	اگر (جوڑو_مقصد_ibody)
		h->ibody_گنتی++;
	h->گنتی++;
	واپس 0;
}

ساکن صحیح space_مستعمل(ساخت ext2_xattr *attrs, صحیح گنتی)
{
	صحیح کل = 0;
	ساخت ext2_xattr *x;
	صحیح i, len;

	برائے (i = 0, x = attrs; i < گنتی; i++, x++) {
		len = لڑی_لمبائی(x->short_name);
		کل += EXT2_EXT_ATTR_LEN(len);
		اگر (!x->ea_ino)
			کل += EXT2_EXT_ATTR_سائز(x->قدر_len);
	}
	واپس کل;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#تعریف_کرو EXT4_XATTR_MIN_LARGE_EA_سائز(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - حجم(ساخت ext2_ext_attr_سرخی) - 4)

errcode_t ext2fs_xattr_سیٹ(ساخت ext2_xattr_handle *h,
			   ثابت حرف *name,
			   ثابت خالی *قدر,
			   size_t قدر_len)
{
	ext2_filsys fs = h->fs;
	ثابت صحیح inode_سائز = EXT2_INODE_سائز(fs->super);
	ساخت ext2_inode_large *inode = تہی;
	ساخت ext2_xattr *x;
	حرف *new_قدر;
	صحیح ibody_free, بلاک_free;
	صحیح اندر_inode = 0;
	صحیح old_idx = -1;
	صحیح extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(قدر_len, &new_قدر);
	اگر (ret)
		واپس ret;
	اگر (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((strcmp(name, "system.posix_acl_default") == 0) ||
	     (strcmp(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_مقصد_disk_بفر(قدر, قدر_len,
						       new_قدر, &قدر_len);
		اگر (ret)
			جاؤ برآمد;
	} ورنہ اگر (قدر_len)
		حافظہ_نقل(new_قدر, قدر, قدر_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	برائے (x = h->attrs; x < h->attrs + h->گنتی; x++) {
		اگر (!strcmp(x->name, name)) {
			اگر (!x->ea_ino && x->قدر_len == قدر_len &&
			    (!قدر_len ||
			     !memcmp(x->قدر, new_قدر, قدر_len))) {
				ret = 0;
				جاؤ برآمد;
			}
			old_idx = x - h->attrs;
			توڑو;
		}
	}

	ret = ext2fs_get_memzero(inode_سائز, &inode);
	اگر (ret)
		جاؤ برآمد;
	ret = ext2fs_read_inode_بھرا(fs, h->ino,
				     (ساخت ext2_inode *)inode,
				     inode_سائز);
	اگر (ret)
		جاؤ برآمد;
	اگر (inode_سائز > EXT2_GOOD_OLD_INODE_سائز) {
		extra_isize = inode->i_extra_isize;
		اگر (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			اگر (extra_isize == 0)
				extra_isize = حجم(__u32);
		}
		ibody_free = inode_سائز - EXT2_GOOD_OLD_INODE_سائز;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= حجم(__u32) * 2;
		ibody_free -= space_مستعمل(h->attrs, h->ibody_گنتی);
	} ورنہ
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	اگر (strcmp(name, "system.data") == 0) {
		اگر (h->ibody_گنتی <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			جاؤ برآمد;
		}
		ret = xattr_array_update(h, name, new_قدر, قدر_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		اگر (ret)
			جاؤ برآمد;
		جاؤ write_برآمد;
	}

	بلاک_free = fs->blocksize;
	بلاک_free -= حجم(ساخت ext2_ext_attr_سرخی);
	/* Final null entry. */
	بلاک_free -= حجم(__u32);
	بلاک_free -= space_مستعمل(h->attrs + h->ibody_گنتی,
				 h->گنتی - h->ibody_گنتی);

	اگر (ext2fs_has_feature_ea_inode(fs->super) &&
	    قدر_len > EXT4_XATTR_MIN_LARGE_EA_سائز(fs->blocksize))
		اندر_inode = 1;

	ret = xattr_array_update(h, name, new_قدر, قدر_len, ibody_free,
				 بلاک_free, old_idx, اندر_inode);
	اگر (ret == EXT2_ET_EA_NO_SPACE && !اندر_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_قدر, قدر_len,
			ibody_free, بلاک_free, old_idx, 1 /* in_inode */);
	اگر (ret)
		جاؤ برآمد;

write_برآمد:
	ret = ext2fs_xattrs_write(h);
برآمد:
	اگر (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_قدر);
	واپس ret;
}

errcode_t ext2fs_xattr_remove(ساخت ext2_xattr_handle *handle,
			      ثابت حرف *کلید)
{
	ساخت ext2_xattr *x;
	ساخت ext2_xattr *انتہا = handle->attrs + handle->گنتی;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	برائے (x = handle->attrs; x < انتہا; x++) {
		اگر (strcmp(x->name, کلید) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->قدر);
			اگر (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (انتہا - x - 1)*حجم(*x));
			memset(انتہا - 1, 0, حجم(*انتہا));
			اگر (x < handle->attrs + handle->ibody_گنتی)
				handle->ibody_گنتی--;
			handle->گنتی--;
			واپس ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	واپس 0;
}

errcode_t ext2fs_xattr_remove_all(ساخت ext2_xattr_handle *handle)
{
	ساخت ext2_xattr *x;
	ساخت ext2_xattr *انتہا = handle->attrs + handle->گنتی;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	برائے (x = handle->attrs; x < انتہا; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->قدر);
		اگر (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_گنتی = 0;
	handle->گنتی = 0;
	واپس ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     ساخت ext2_xattr_handle **handle)
{
	ساخت ext2_xattr_handle *h;
	errcode_t err;

	اگر (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_ڈیٹا(fs->super))
		واپس EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(حجم(*h), &h);
	اگر (err)
		واپس err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->استعداد = 4;
	err = ext2fs_get_arrayzero(h->استعداد, حجم(ساخت ext2_xattr),
				   &h->attrs);
	اگر (err) {
		ext2fs_free_mem(&h);
		واپس err;
	}
	h->گنتی = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	واپس 0;
}

errcode_t ext2fs_xattrs_close(ساخت ext2_xattr_handle **handle)
{
	ساخت ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_کلیدیں(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	واپس 0;
}

errcode_t ext2fs_xattrs_گنتی(ساخت ext2_xattr_handle *handle, size_t *گنتی)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*گنتی = handle->گنتی;
	واپس 0;
}

errcode_t ext2fs_xattrs_flags(ساخت ext2_xattr_handle *handle,
			      غیرعلامتی صحیح *new_flags, غیرعلامتی صحیح *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	اگر (old_flags)
		*old_flags = handle->flags;
	اگر (new_flags)
		handle->flags = *new_flags;
	واپس 0;
}
