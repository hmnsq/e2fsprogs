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
#پایان_اگر
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

#تعریف_کن NAME_HASH_لغزش 5
#تعریف_کن مقدار_HASH_لغزش 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_مدخل(بنية ext2_ext_attr_مدخل *مدخل, فراغ *داده)
{
	__u32 hash = 0;
	بیعلامت حرف *name = (((بیعلامت حرف *) مدخل) +
			       حجم(بنية ext2_ext_attr_مدخل));
	صحيح n;

	لكل (n = 0; n < مدخل->e_name_len; n++) {
		hash = (hash << NAME_HASH_لغزش) ^
		       (hash >> (8*حجم(hash) - NAME_HASH_لغزش)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	إذا (مدخل->e_مقدار_inum == 0 && مدخل->e_مقدار_اندازه != 0) {
		__u32 *مقدار = (__u32 *)داده;
		لكل (n = (مدخل->e_مقدار_اندازه + EXT2_EXT_ATTR_دور) >>
			 EXT2_EXT_ATTR_PAD_بیتها; n; n--) {
			hash = (hash << مقدار_HASH_لغزش) ^
			       (hash >> (8*حجم(hash) - مقدار_HASH_لغزش)) ^
			       ext2fs_le32_مقصد_cpu(*مقدار++);
		}
	}

	أرجع hash;
}

__u32 ext2fs_ext_attr_hash_مدخل_signed(بنية ext2_ext_attr_مدخل *مدخل,
					فراغ *داده)
{
	__u32 hash = 0;
	علامتدار حرف *name = (((علامتدار حرف *) مدخل) +
			     حجم(بنية ext2_ext_attr_مدخل));
	صحيح n;

	لكل (n = 0; n < مدخل->e_name_len; n++) {
		hash = (hash << NAME_HASH_لغزش) ^
		       (hash >> (8*حجم(hash) - NAME_HASH_لغزش)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	إذا (مدخل->e_مقدار_inum == 0 && مدخل->e_مقدار_اندازه != 0) {
		__u32 *مقدار = (__u32 *)داده;
		لكل (n = (مدخل->e_مقدار_اندازه + EXT2_EXT_ATTR_دور) >>
			 EXT2_EXT_ATTR_PAD_بیتها; n; n--) {
			hash = (hash << مقدار_HASH_لغزش) ^
			       (hash >> (8*حجم(hash) - مقدار_HASH_لغزش)) ^
			       ext2fs_le32_مقصد_cpu(*مقدار++);
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
				      بنية ext2_ext_attr_مدخل *مدخل,
				      فراغ *داده, __u32 *hash,
				      __u32 *signed_hash)
{
	*hash = ext2fs_ext_attr_hash_مدخل(مدخل, داده);
	إذا (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_مدخل_signed(مدخل, داده);

	إذا (مدخل->e_مقدار_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, مدخل->e_مقدار_inum,
					    &ea_inode_hash);
		إذا (retval)
			أرجع retval;

		*hash = (*hash << مقدار_HASH_لغزش) ^
			(*hash >> (8*حجم(*hash) - مقدار_HASH_لغزش)) ^
			ea_inode_hash;
		إذا (signed_hash)
			*signed_hash = (*signed_hash << مقدار_HASH_لغزش) ^
				(*signed_hash >> (8*حجم(*hash) -
						  مقدار_HASH_لغزش)) ^
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
				      بنية ext2_ext_attr_مدخل *مدخل,
				      فراغ *داده, __u32 *hash)
{
	أرجع ext2fs_ext_attr_hash_entry3(fs, مدخل, داده, hash, عدم);
}

#لغو_تعریف NAME_HASH_لغزش
#لغو_تعریف مقدار_HASH_لغزش

#تعریف_کن بلوک_HASH_لغزش 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
فراغ ext2fs_ext_attr_بلوک_rehash(بنية ext2_ext_attr_سرآیند *سرآیند,
				  بنية ext2_ext_attr_مدخل *انتها)
{
	بنية ext2_ext_attr_مدخل *اینجا;
	__u32 hash = 0;

	اینجا = (بنية ext2_ext_attr_مدخل *)(سرآیند+1);
	طالما (اینجا < انتها && !EXT2_EXT_IS_آخرین_مدخل(اینجا)) {
		إذا (!اینجا->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			اكسر;
		}
		hash = (hash << بلوک_HASH_لغزش) ^
		       (hash >> (8*حجم(hash) - بلوک_HASH_لغزش)) ^
		       اینجا->e_hash;
		اینجا = EXT2_EXT_ATTR_بعدی(اینجا);
	}
	سرآیند->h_hash = hash;
}

#لغو_تعریف بلوک_HASH_لغزش

__u32 ext2fs_get_ea_inode_hash(بنية ext2_inode *inode)
{
	أرجع inode->i_atime;
}

فراغ ext2fs_مجموعه_ea_inode_hash(بنية ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(بنية ext2_inode *inode)
{
	أرجع ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_نسخه;
}

فراغ ext2fs_مجموعه_ea_inode_ref(بنية ext2_inode *inode, __u64 ref_شمار)
{
	inode->i_ctime = (__u32)(ref_شمار >> 32);
	inode->osd1.linux1.l_i_نسخه = (__u32)ref_شمار;
}

ساكن errcode_t check_ext_attr_سرآیند(بنية ext2_ext_attr_سرآیند *سرآیند)
{
	إذا ((سرآیند->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     سرآیند->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    سرآیند->h_بلوکها != 1)
		أرجع EXT2_ET_BAD_EA_سرآیند;

	أرجع 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t بلوک, فراغ *buf,
				ext2_ino_t inum)
{
	صحيح		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, بلوک, 1, buf);
	إذا (retval)
		أرجع retval;

	إذا (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_بلوک_csum_verify(fs, inum, بلوک, buf))
		csum_failed = 1;

#اگر_تعریف_شده واژگان_BIGENDIAN
	ext2fs_جابجا_ext_attr(buf, buf, fs->blocksize, 1);
#پایان_اگر

	retval = check_ext_attr_سرآیند(buf);
	إذا (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	أرجع retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t بلوک, فراغ *buf)
{
	أرجع ext2fs_read_ext_attr3(fs, بلوک, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t بلوک, فراغ *buf)
{
	أرجع ext2fs_read_ext_attr2(fs, بلوک, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t بلوک, فراغ *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	حرف		*write_buf;

#اگر_تعریف_شده واژگان_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	إذا (retval)
		أرجع retval;
	ext2fs_جابجا_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#وإلا
	write_buf = (حرف *) inbuf;
#پایان_اگر

	retval = ext2fs_ext_attr_بلوک_csum_مجموعه(fs, inum, بلوک,
			(بنية ext2_ext_attr_سرآیند *)write_buf);
	إذا (retval)
		أرجع retval;

	retval = io_channel_write_blk64(fs->io, بلوک, 1, write_buf);
#اگر_تعریف_شده واژگان_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#پایان_اگر
	إذا (!retval)
		ext2fs_mark_تغییریافته(fs);
	أرجع retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t بلوک, فراغ *inbuf)
{
	أرجع ext2fs_write_ext_attr3(fs, بلوک, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t بلوک, فراغ *inbuf)
{
	أرجع ext2fs_write_ext_attr2(fs, بلوک, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    حرف *بلوک_buf, صحيح adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	بنية ext2_ext_attr_سرآیند *سرآیند;
	حرف	*buf = 0;

	إذا ((blk >= ext2fs_بلوکها_شمار(fs->super)) ||
	    (blk < fs->super->s_اولی_داده_بلوک))
		أرجع EXT2_ET_BAD_EA_بلوک_رقم;

	إذا (!بلوک_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		إذا (retval)
			أرجع retval;
		بلوک_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, بلوک_buf, inum);
	إذا (retval)
		اذهب errout;

	سرآیند = (بنية ext2_ext_attr_سرآیند *) بلوک_buf;
	سرآیند->h_refcount += adjust;
	إذا (newcount)
		*newcount = سرآیند->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, بلوک_buf, inum);
	إذا (retval)
		اذهب errout;

errout:
	إذا (buf)
		ext2fs_free_mem(&buf);
	أرجع retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    حرف *بلوک_buf, صحيح adjust,
				    __u32 *newcount)
{
	أرجع ext2fs_adjust_ea_refcount3(fs, blk, بلوک_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					حرف *بلوک_buf, صحيح adjust,
					__u32 *newcount)
{
	أرجع ext2fs_adjust_ea_refcount2(fs, blk, بلوک_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
بنية ext2_xattr {
	صحيح name_شاخص;
	حرف *name;
	حرف *short_name;
	فراغ *مقدار;
	بیعلامت صحيح مقدار_len;
	ext2_ino_t ea_ino;
};

بنية ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	بنية ext2_xattr *attrs;
	صحيح ظرفیت;
	صحيح شمار;
	صحيح ibody_شمار;
	ext2_ino_t ino;
	بیعلامت صحيح flags;
};

ساكن errcode_t ext2fs_xattrs_expand(بنية ext2_xattr_handle *h,
				      بیعلامت صحيح expandby)
{
	بنية ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->ظرفیت + expandby,
				   حجم(بنية ext2_xattr), &new_attrs);
	إذا (err)
		أرجع err;

	کپی_حافظه(new_attrs, h->attrs, h->ظرفیت * حجم(بنية ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->ظرفیت += expandby;
	h->attrs = new_attrs;

	أرجع 0;
}

بنية ea_name_شاخص {
	صحيح شاخص;
	ثابت حرف *name;
};

/* Keep these names sorted in order of decreasing specificity. */
ساكن بنية ea_name_شاخص ea_نامها[] = {
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

ساكن ثابت حرف *پیدا_کن_ea_پیشوند(صحيح شاخص)
{
	بنية ea_name_شاخص *e;

	لكل (e = ea_نامها; e->name; e++)
		إذا (e->شاخص == شاخص)
			أرجع e->name;

	أرجع عدم;
}

ساكن صحيح پیدا_کن_ea_شاخص(ثابت حرف *fullname, ثابت حرف **name, صحيح *شاخص)
{
	بنية ea_name_شاخص *e;

	لكل (e = ea_نامها; e->name; e++) {
		إذا (مقایسه_رشته(fullname, e->name, طول_رشته(e->name)) == 0) {
			*name = fullname + طول_رشته(e->name);
			*شاخص = e->شاخص;
			أرجع 1;
		}
	}
	أرجع 0;
}

errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       بنية ext2_inode_large *inode)
{
	بنية ext2_ext_attr_سرآیند *سرآیند;
	فراغ *بلوک_buf = عدم;
	blk64_t blk;
	errcode_t err;
	بنية ext2_inode_large i;

	/* Read inode? */
	إذا (inode == عدم) {
		err = ext2fs_read_inode_پر(fs, ino, (بنية ext2_inode *)&i,
					     حجم(بنية ext2_inode_large));
		إذا (err)
			أرجع err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_بلوک(fs, (بنية ext2_inode *)inode);
	إذا (blk == 0)
		أرجع 0;

	/* Find block, zero it, write back */
	إذا ((blk < fs->super->s_اولی_داده_بلوک) ||
	    (blk >= ext2fs_بلوکها_شمار(fs->super))) {
		err = EXT2_ET_BAD_EA_بلوک_رقم;
		اذهب بیرون;
	}

	err = ext2fs_get_mem(fs->blocksize, &بلوک_buf);
	إذا (err)
		اذهب بیرون;

	err = ext2fs_read_ext_attr3(fs, blk, بلوک_buf, ino);
	إذا (err)
		اذهب out2;

	/* We only know how to deal with v2 EA blocks */
	سرآیند = (بنية ext2_ext_attr_سرآیند *) بلوک_buf;
	إذا (سرآیند->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_سرآیند;
		اذهب out2;
	}

	سرآیند->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, بلوک_buf, ino);
	إذا (err)
		اذهب out2;

	/* Erase link to block */
	ext2fs_file_acl_بلوک_مجموعه(fs, (بنية ext2_inode *)inode, 0);
	إذا (سرآیند->h_refcount == 0)
		ext2fs_بلوک_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_بلوکها(fs, (بنية ext2_inode *)inode, 1);
	إذا (err)
		اذهب out2;

	/* Write inode? */
	إذا (inode == &i) {
		err = ext2fs_write_inode_پر(fs, ino, (بنية ext2_inode *)&i,
					      حجم(بنية ext2_inode_large));
		إذا (err)
			اذهب out2;
	}

out2:
	ext2fs_free_mem(&بلوک_buf);
بیرون:
	أرجع err;
}

ساكن errcode_t prep_ea_بلوک_for_write(ext2_filsys fs, ext2_ino_t ino,
					 بنية ext2_inode_large *inode)
{
	بنية ext2_ext_attr_سرآیند *سرآیند;
	فراغ *بلوک_buf = عدم;
	blk64_t blk, goal;
	errcode_t err;

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_بلوک(fs, (بنية ext2_inode *)inode);
	إذا (blk != 0) {
		إذا ((blk < fs->super->s_اولی_داده_بلوک) ||
		    (blk >= ext2fs_بلوکها_شمار(fs->super))) {
			err = EXT2_ET_BAD_EA_بلوک_رقم;
			اذهب بیرون;
		}

		err = ext2fs_get_mem(fs->blocksize, &بلوک_buf);
		إذا (err)
			اذهب بیرون;

		err = ext2fs_read_ext_attr3(fs, blk, بلوک_buf, ino);
		إذا (err)
			اذهب out2;

		/* We only know how to deal with v2 EA blocks */
		سرآیند = (بنية ext2_ext_attr_سرآیند *) بلوک_buf;
		إذا (سرآیند->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_سرآیند;
			اذهب out2;
		}

		/* Single-user block.  We're done here. */
		إذا (سرآیند->h_refcount == 1)
			اذهب out2;

		/* We need to CoW the block. */
		سرآیند->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, بلوک_buf, ino);
		إذا (err)
			اذهب out2;
	} وإلا {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_جمع_کن_بلوکها(fs, (بنية ext2_inode *)inode,
					     1);
		إذا (err)
			اذهب بیرون;
	}

	/* Allocate a block */
	goal = ext2fs_پیدا_کن_inode_goal(fs, ino, (بنية ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, عدم, &blk);
	إذا (err)
		اذهب out2;
	ext2fs_file_acl_بلوک_مجموعه(fs, (بنية ext2_inode *)inode, blk);
out2:
	إذا (بلوک_buf)
		ext2fs_free_mem(&بلوک_buf);
بیرون:
	أرجع err;
}


ساكن مضمن صحيح
posix_acl_xattr_شمار(size_t اندازه)
{
        إذا (اندازه < حجم(posix_acl_xattr_سرآیند))
                أرجع -1;
        اندازه -= حجم(posix_acl_xattr_سرآیند);
        إذا (اندازه % حجم(posix_acl_xattr_مدخل))
                أرجع -1;
        أرجع اندازه / حجم(posix_acl_xattr_مدخل);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
ساكن errcode_t convert_posix_acl_مقصد_disk_میانگیر(ثابت فراغ *مقدار, size_t اندازه,
						  فراغ *بیرون_buf, size_t *اندازه_بیرون)
{
	ثابت posix_acl_xattr_سرآیند *سرآیند =
		(ثابت posix_acl_xattr_سرآیند*) مقدار;
	ثابت posix_acl_xattr_مدخل *انتها, *مدخل =
		(ثابت posix_acl_xattr_مدخل *)(سرآیند+1);
	ext4_acl_سرآیند *ext_acl;
	size_t s;
	حرف *e;

	صحيح شمار;

	إذا (!مقدار)
		أرجع EINVAL;
	إذا (اندازه < حجم(posix_acl_xattr_سرآیند))
		أرجع ENOMEM;
	إذا (سرآیند->a_نسخه != ext2fs_cpu_مقصد_le32(POSIX_ACL_XATTR_نسخه))
		أرجع EINVAL;

	شمار = posix_acl_xattr_شمار(اندازه);
	ext_acl = بیرون_buf;
	ext_acl->a_نسخه = ext2fs_cpu_مقصد_le32(EXT4_ACL_نسخه);

	إذا (شمار <= 0)
		أرجع EINVAL;

	e = (حرف *) بیرون_buf + حجم(ext4_acl_سرآیند);
	s = حجم(ext4_acl_سرآیند);
	لكل (انتها = مدخل + شمار; مدخل != انتها;مدخل++) {
		ext4_acl_مدخل *disk_مدخل = (ext4_acl_مدخل*) e;
		disk_مدخل->e_برچسب = مدخل->e_برچسب;
		disk_مدخل->e_perm = مدخل->e_perm;

		بدل(ext2fs_le16_مقصد_cpu(مدخل->e_برچسب)) {
			حالة ACL_USER_OBJ:
			حالة ACL_گروه_OBJ:
			حالة ACL_ماسک:
			حالة ACL_OTHER:
				e += حجم(ext4_acl_مدخل_short);
				s += حجم(ext4_acl_مدخل_short);
				اكسر;
			حالة ACL_USER:
			حالة ACL_گروه:
				disk_مدخل->e_id = مدخل->e_id;
				e += حجم(ext4_acl_مدخل);
				s += حجم(ext4_acl_مدخل);
				اكسر;
			افتراضي:
				أرجع EINVAL;
		}
	}
	*اندازه_بیرون = s;
	أرجع 0;
}

ساكن errcode_t convert_disk_میانگیر_مقصد_posix_acl(ثابت فراغ *مقدار, size_t اندازه,
						  فراغ **بیرون_buf, size_t *اندازه_بیرون)
{
	posix_acl_xattr_سرآیند *سرآیند;
	posix_acl_xattr_مدخل *مدخل;
	ثابت ext4_acl_سرآیند *ext_acl = (ثابت ext4_acl_سرآیند *) مقدار;
	errcode_t err;
	ثابت حرف *cp;
	حرف *بیرون;

	إذا ((!مقدار) ||
	    (اندازه < حجم(ext4_acl_سرآیند)) ||
	    (ext_acl->a_نسخه != ext2fs_cpu_مقصد_le32(EXT4_ACL_نسخه)))
		أرجع EINVAL;

	err = ext2fs_get_mem(اندازه * 2, &بیرون);
	إذا (err)
		أرجع err;

	سرآیند = (posix_acl_xattr_سرآیند *) بیرون;
	سرآیند->a_نسخه = ext2fs_cpu_مقصد_le32(POSIX_ACL_XATTR_نسخه);
	مدخل = (posix_acl_xattr_مدخل *) (بیرون + حجم(posix_acl_xattr_سرآیند));

	cp = (ثابت حرف *) مقدار + حجم(ext4_acl_سرآیند);
	اندازه -= حجم(ext4_acl_سرآیند);

	طالما (اندازه > 0) {
		ثابت ext4_acl_مدخل *disk_مدخل = (ثابت ext4_acl_مدخل *) cp;

		مدخل->e_برچسب = disk_مدخل->e_برچسب;
		مدخل->e_perm = disk_مدخل->e_perm;

		بدل(ext2fs_le16_مقصد_cpu(مدخل->e_برچسب)) {
			حالة ACL_USER_OBJ:
			حالة ACL_گروه_OBJ:
			حالة ACL_ماسک:
			حالة ACL_OTHER:
				مدخل->e_id = 0;
				cp += حجم(ext4_acl_مدخل_short);
				اندازه -= حجم(ext4_acl_مدخل_short);
				اكسر;
			حالة ACL_USER:
			حالة ACL_گروه:
				مدخل->e_id = disk_مدخل->e_id;
				cp += حجم(ext4_acl_مدخل);
				اندازه -= حجم(ext4_acl_مدخل);
				اكسر;
			افتراضي:
				ext2fs_free_mem(&بیرون);
				أرجع EINVAL;
		}
		مدخل++;
	}
	*بیرون_buf = بیرون;
	*اندازه_بیرون = ((حرف *) مدخل - بیرون);
	أرجع 0;
}

ساكن errcode_t
write_xattrs_مقصد_میانگیر(ext2_filsys fs, بنية ext2_xattr *attrs, صحيح شمار,
		       فراغ *entries_شروع, بیعلامت صحيح storage_اندازه,
		       بیعلامت صحيح مقدار_جابه_جایی_correction, صحيح write_hash)
{
	بنية ext2_xattr *x;
	بنية ext2_ext_attr_مدخل *e = entries_شروع;
	حرف *انتها = (حرف *) entries_شروع + storage_اندازه;
	بیعلامت صحيح مقدار_اندازه;
	errcode_t err;

	memset(entries_شروع, 0, storage_اندازه);
	لكل (x = attrs; x < attrs + شمار; x++) {
		مقدار_اندازه = ((x->مقدار_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = طول_رشته(x->short_name);
		e->e_name_شاخص = x->name_شاخص;

		e->e_مقدار_اندازه = x->مقدار_len;
		e->e_مقدار_inum = x->ea_ino;

		/* Store name */
		کپی_حافظه((حرف *)e + حجم(*e), x->short_name, e->e_name_len);
		إذا (x->ea_ino) {
			e->e_مقدار_offs = 0;
		} وإلا {
			انتها -= مقدار_اندازه;
			e->e_مقدار_offs = انتها - (حرف *) entries_شروع +
						مقدار_جابه_جایی_correction;
			کپی_حافظه(انتها, x->مقدار, e->e_مقدار_اندازه);
		}

		إذا (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : انتها,
							  &e->e_hash);
			إذا (err)
				أرجع err;
		} وإلا
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_بعدی(e);
		*(__u32 *)e = 0;
	}
	أرجع 0;
}

errcode_t ext2fs_xattrs_write(بنية ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	ثابت بیعلامت صحيح inode_اندازه = EXT2_INODE_اندازه(fs->super);
	بنية ext2_inode_large *inode;
	حرف *شروع, *بلوک_buf = عدم;
	بنية ext2_ext_attr_سرآیند *سرآیند;
	__u32 ea_inode_magic;
	blk64_t blk;
	بیعلامت صحيح storage_اندازه;
	بیعلامت صحيح i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_اندازه;
	إذا (i < حجم(*inode))
		i = حجم(*inode);
	err = ext2fs_get_memzero(i, &inode);
	إذا (err)
		أرجع err;

	err = ext2fs_read_inode_پر(fs, handle->ino, EXT2_INODE(inode),
				     inode_اندازه);
	إذا (err)
		اذهب بیرون;

	/* If extra_isize isn't set, we need to set it now */
	إذا (inode->i_extra_isize == 0 &&
	    inode_اندازه > EXT2_GOOD_OLD_INODE_اندازه) {
		حرف *p = (حرف *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		إذا (extra == 0)
			extra = حجم(__u32);
		memset(p + EXT2_GOOD_OLD_INODE_اندازه, 0, extra);
		inode->i_extra_isize = extra;
	}
	إذا (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		اذهب بیرون;
	}

	/* Does the inode have space for EA? */
	إذا (inode->i_extra_isize < حجم(inode->i_extra_isize) ||
	    inode_اندازه <= EXT2_GOOD_OLD_INODE_اندازه + inode->i_extra_isize +
								حجم(__u32))
		اذهب write_ea_بلوک;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	کپی_حافظه(((حرف *) inode) + EXT2_GOOD_OLD_INODE_اندازه +
	       inode->i_extra_isize, &ea_inode_magic, حجم(__u32));
	storage_اندازه = inode_اندازه - EXT2_GOOD_OLD_INODE_اندازه -
				inode->i_extra_isize - حجم(__u32);
	شروع = ((حرف *) inode) + EXT2_GOOD_OLD_INODE_اندازه +
				inode->i_extra_isize + حجم(__u32);

	err = write_xattrs_مقصد_میانگیر(fs, handle->attrs, handle->ibody_شمار,
				     شروع, storage_اندازه, 0, 0);
	إذا (err)
		اذهب بیرون;
write_ea_بلوک:
	/* Are we done? */
	إذا (handle->ibody_شمار == handle->شمار &&
	    !ext2fs_file_acl_بلوک(fs, EXT2_INODE(inode)))
		اذهب skip_ea_بلوک;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &بلوک_buf);
	إذا (err)
		اذهب بیرون;

	storage_اندازه = fs->blocksize - حجم(بنية ext2_ext_attr_سرآیند);
	شروع = بلوک_buf + حجم(بنية ext2_ext_attr_سرآیند);

	err = write_xattrs_مقصد_میانگیر(fs, handle->attrs + handle->ibody_شمار,
				     handle->شمار - handle->ibody_شمار, شروع,
				     storage_اندازه, شروع - بلوک_buf, 1);
	إذا (err)
		اذهب out2;

	/* Write a header on the EA block */
	سرآیند = (بنية ext2_ext_attr_سرآیند *) بلوک_buf;
	سرآیند->h_magic = EXT2_EXT_ATTR_MAGIC;
	سرآیند->h_refcount = 1;
	سرآیند->h_بلوکها = 1;

	/* Get a new block for writing */
	err = prep_ea_بلوک_for_write(fs, handle->ino, inode);
	إذا (err)
		اذهب out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_بلوک(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, بلوک_buf, handle->ino);
	إذا (err)
		اذهب out2;

skip_ea_بلوک:
	blk = ext2fs_file_acl_بلوک(fs, (بنية ext2_inode *)inode);
	إذا (!بلوک_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		إذا (err)
			اذهب بیرون;
	}

	/* Write the inode */
	err = ext2fs_write_inode_پر(fs, handle->ino, EXT2_INODE(inode),
				      inode_اندازه);
	إذا (err)
		اذهب out2;

out2:
	ext2fs_free_mem(&بلوک_buf);
بیرون:
	ext2fs_free_mem(&inode);
	أرجع err;
}

ساكن errcode_t read_xattrs_from_میانگیر(بنية ext2_xattr_handle *handle,
					 بنية ext2_inode_large *inode,
					 بنية ext2_ext_attr_مدخل *entries,
					 بیعلامت صحيح storage_اندازه,
					 حرف *مقدار_شروع)
{
	بنية ext2_xattr *x;
	بنية ext2_ext_attr_مدخل *مدخل, *انتها;
	ثابت حرف *پیشوند;
	بیعلامت صحيح remain, پیشوند_len;
	errcode_t err;
	بیعلامت صحيح مقادیر_اندازه = storage_اندازه +
			((حرف *)entries - مقدار_شروع);

	/* find the end */
	انتها = entries;
	remain = storage_اندازه;
	طالما (remain >= حجم(بنية ext2_ext_attr_مدخل) &&
	       !EXT2_EXT_IS_آخرین_مدخل(انتها)) {

		/* header eats this space */
		remain -= حجم(بنية ext2_ext_attr_مدخل);

		/* is attribute name valid? */
		إذا (EXT2_EXT_ATTR_اندازه(انتها->e_name_len) > remain)
			أرجع EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_اندازه(انتها->e_name_len);
		انتها = EXT2_EXT_ATTR_بعدی(انتها);
	}

	مدخل = entries;
	remain = storage_اندازه;
	طالما (remain >= حجم(بنية ext2_ext_attr_مدخل) &&
	       !EXT2_EXT_IS_آخرین_مدخل(مدخل)) {

		/* Allocate space for more attrs? */
		إذا (handle->شمار == handle->ظرفیت) {
			err = ext2fs_xattrs_expand(handle, 4);
			إذا (err)
				أرجع err;
		}

		x = handle->attrs + handle->شمار;

		/* header eats this space */
		remain -= حجم(بنية ext2_ext_attr_مدخل);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_اندازه(مدخل->e_name_len);

		/* Extract name */
		پیشوند = پیدا_کن_ea_پیشوند(مدخل->e_name_شاخص);
		پیشوند_len = (پیشوند ? طول_رشته(پیشوند) : 0);
		err = ext2fs_get_memzero(مدخل->e_name_len + پیشوند_len + 1,
					 &x->name);
		إذا (err)
			أرجع err;
		إذا (پیشوند)
			کپی_حافظه(x->name, پیشوند, پیشوند_len);
		إذا (مدخل->e_name_len)
			کپی_حافظه(x->name + پیشوند_len,
			       (حرف *)مدخل + حجم(*مدخل),
			       مدخل->e_name_len);
		x->short_name = x->name + پیشوند_len;
		x->name_شاخص = مدخل->e_name_شاخص;

		/* Check & copy value */
		إذا (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    مدخل->e_مقدار_inum != 0)
			أرجع EXT2_ET_BAD_EA_بلوک_رقم;

		إذا (مدخل->e_مقدار_inum == 0) {
			إذا (مدخل->e_مقدار_اندازه > remain)
				أرجع EXT2_ET_EA_BAD_مقدار_اندازه;

			إذا (مدخل->e_مقدار_offs + مدخل->e_مقدار_اندازه > مقادیر_اندازه)
				أرجع EXT2_ET_EA_BAD_مقدار_جابه_جایی;

			إذا (مدخل->e_مقدار_اندازه > 0 &&
			    مقدار_شروع + مدخل->e_مقدار_offs <
			    (حرف *)انتها + حجم(__u32))
				أرجع EXT2_ET_EA_BAD_مقدار_جابه_جایی;

			remain -= مدخل->e_مقدار_اندازه;

			err = ext2fs_get_mem(مدخل->e_مقدار_اندازه, &x->مقدار);
			إذا (err)
				أرجع err;
			کپی_حافظه(x->مقدار, مقدار_شروع + مدخل->e_مقدار_offs,
			       مدخل->e_مقدار_اندازه);
		} وإلا {
			بنية ext2_inode *ea_inode;
			ext2_file_t ea_file;

			إذا (مدخل->e_مقدار_offs != 0)
				أرجع EXT2_ET_EA_BAD_مقدار_جابه_جایی;

			إذا (مدخل->e_مقدار_اندازه > (64 * 1024))
				أرجع EXT2_ET_EA_BAD_مقدار_اندازه;

			err = ext2fs_get_mem(مدخل->e_مقدار_اندازه, &x->مقدار);
			إذا (err)
				أرجع err;

			err = ext2fs_file_open(handle->fs, مدخل->e_مقدار_inum,
					       0, &ea_file);
			إذا (err)
				أرجع err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			إذا ((ea_inode->i_flags & EXT4_INLINE_داده_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_شمار == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			وإلا إذا ((__u64) ext2fs_file_get_اندازه(ea_file) !=
				 مدخل->e_مقدار_اندازه)
				err = EXT2_ET_EA_BAD_مقدار_اندازه;
			وإلا
				err = ext2fs_file_read(ea_file, x->مقدار,
						       مدخل->e_مقدار_اندازه, 0);
			ext2fs_file_close(ea_file);
			إذا (err)
				أرجع err;
		}

		x->ea_ino = مدخل->e_مقدار_inum;
		x->مقدار_len = مدخل->e_مقدار_اندازه;

		/* e_hash may be 0 in older inode's ea */
		إذا (مدخل->e_hash != 0) {
			__u32 hash, signed_hash;

			فراغ *داده = (مدخل->e_مقدار_inum != 0) ?
					0 : مقدار_شروع + مدخل->e_مقدار_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, مدخل,
							  داده, &hash,
							  &signed_hash);
			إذا (err)
				أرجع err;
			إذا ((مدخل->e_hash != hash) &&
			    (مدخل->e_hash != signed_hash)) {
				بنية ext2_inode فرزند;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							مدخل->e_مقدار_inum,
							&فرزند);
				إذا (err)
					أرجع err;
				إذا (فرزند.i_mtime != handle->ino ||
				    فرزند.i_generation != inode->i_generation)
					أرجع EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->شمار++;
		مدخل = EXT2_EXT_ATTR_بعدی(مدخل);
	}

	أرجع 0;
}

ساكن فراغ xattrs_free_کلیدها(بنية ext2_xattr_handle *h)
{
	بنية ext2_xattr *a = h->attrs;
	صحيح i;

	لكل (i = 0; i < h->ظرفیت; i++) {
		إذا (a[i].name)
			ext2fs_free_mem(&a[i].name);
		إذا (a[i].مقدار)
			ext2fs_free_mem(&a[i].مقدار);
	}
	h->شمار = 0;
	h->ibody_شمار = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(بنية ext2_xattr_handle *handle,
				   بنية ext2_inode_large *inode)
{
	بنية ext2_ext_attr_سرآیند *سرآیند;
	__u32 ea_inode_magic;
	بیعلامت صحيح storage_اندازه;
	حرف *شروع, *بلوک_buf = عدم;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_کلیدها(handle);

	/* Does the inode have space for EA? */
	إذا (inode->i_extra_isize < حجم(inode->i_extra_isize) ||
	    EXT2_INODE_اندازه(handle->fs->super) <= EXT2_GOOD_OLD_INODE_اندازه +
						  inode->i_extra_isize +
						  حجم(__u32))
		اذهب read_ea_بلوک;
	إذا (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		اذهب بیرون;
	}

	/* Look for EA in the inode */
	کپی_حافظه(&ea_inode_magic, ((حرف *) inode) + EXT2_GOOD_OLD_INODE_اندازه +
	       inode->i_extra_isize, حجم(__u32));
	إذا (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_اندازه = EXT2_INODE_اندازه(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_اندازه - inode->i_extra_isize -
			حجم(__u32);
		شروع = ((حرف *) inode) + EXT2_GOOD_OLD_INODE_اندازه +
			inode->i_extra_isize + حجم(__u32);

		err = read_xattrs_from_میانگیر(handle, inode,
					(بنية ext2_ext_attr_مدخل *) شروع,
					storage_اندازه, شروع);
		إذا (err)
			اذهب بیرون;

		handle->ibody_شمار = handle->شمار;
	}

read_ea_بلوک:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_بلوک(handle->fs, EXT2_INODE(inode));
	إذا (blk != 0) {
		إذا ((blk < handle->fs->super->s_اولی_داده_بلوک) ||
		    (blk >= ext2fs_بلوکها_شمار(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_بلوک_رقم;
			اذهب بیرون;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &بلوک_buf);
		إذا (err)
			اذهب بیرون;

		err = ext2fs_read_ext_attr3(handle->fs, blk, بلوک_buf,
					    handle->ino);
		إذا (err)
			اذهب out3;

		/* We only know how to deal with v2 EA blocks */
		سرآیند = (بنية ext2_ext_attr_سرآیند *) بلوک_buf;
		إذا (سرآیند->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_سرآیند;
			اذهب out3;
		}

		/* Read EAs */
		storage_اندازه = handle->fs->blocksize -
			حجم(بنية ext2_ext_attr_سرآیند);
		شروع = بلوک_buf + حجم(بنية ext2_ext_attr_سرآیند);
		err = read_xattrs_from_میانگیر(handle, inode,
					(بنية ext2_ext_attr_مدخل *) شروع,
					storage_اندازه, بلوک_buf);
	}

out3:
	إذا (بلوک_buf)
		ext2fs_free_mem(&بلوک_buf);
بیرون:
	أرجع err;
}

errcode_t ext2fs_xattrs_read(بنية ext2_xattr_handle *handle)
{
	بنية ext2_inode_large *inode;
	size_t inode_اندازه = EXT2_INODE_اندازه(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	إذا (inode_اندازه < حجم(*inode))
		inode_اندازه = حجم(*inode);
	err = ext2fs_get_memzero(inode_اندازه, &inode);
	إذا (err)
		أرجع err;

	err = ext2fs_read_inode_پر(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_اندازه(handle->fs->super));
	إذا (err)
		اذهب بیرون;

	err = ext2fs_xattrs_read_inode(handle, inode);

بیرون:
	ext2fs_free_mem(&inode);

	أرجع err;
}

errcode_t ext2fs_xattrs_iterate(بنية ext2_xattr_handle *h,
				صحيح (*func)(حرف *name, حرف *مقدار,
					    size_t مقدار_len, فراغ *داده),
				فراغ *داده)
{
	بنية ext2_xattr *x;
	صحيح dirty = 0;
	صحيح ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = h->attrs; x < h->attrs + h->شمار; x++) {
		ret = func(x->name, x->مقدار, x->مقدار_len, داده);
		إذا (ret & XATTR_تغییریافته)
			dirty = 1;
		إذا (ret & XATTR_ABORT)
			اكسر;
	}

	إذا (dirty)
		أرجع ext2fs_xattrs_write(h);
	أرجع 0;
}

errcode_t ext2fs_xattr_get(بنية ext2_xattr_handle *h, ثابت حرف *کلید,
			   فراغ **مقدار, size_t *مقدار_len)
{
	بنية ext2_xattr *x;
	حرف *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = h->attrs; x < h->attrs + h->شمار; x++) {
		إذا (strcmp(x->name, کلید))
			استمر;

		إذا (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(کلید, "system.posix_acl_default") == 0) ||
		     (strcmp(کلید, "system.posix_acl_access") == 0))) {
			err = convert_disk_میانگیر_مقصد_posix_acl(x->مقدار, x->مقدار_len,
							       مقدار, مقدار_len);
			أرجع err;
		} وإلا {
			err = ext2fs_get_mem(x->مقدار_len, &val);
			إذا (err)
				أرجع err;
			کپی_حافظه(val, x->مقدار, x->مقدار_len);
			*مقدار = val;
			*مقدار_len = x->مقدار_len;
			أرجع 0;
		}
	}

	أرجع EXT2_ET_EA_کلید_NOT_یافت;
}

errcode_t ext2fs_xattr_inode_max_اندازه(ext2_filsys fs, ext2_ino_t ino,
				      size_t *اندازه)
{
	بنية ext2_ext_attr_مدخل *مدخل;
	بنية ext2_inode_large *inode;
	__u32 ea_inode_magic;
	بیعلامت صحيح minoff;
	حرف *شروع;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_اندازه(fs->super);
	إذا (i < حجم(*inode))
		i = حجم(*inode);
	err = ext2fs_get_memzero(i, &inode);
	إذا (err)
		أرجع err;

	err = ext2fs_read_inode_پر(fs, ino, (بنية ext2_inode *)inode,
				     EXT2_INODE_اندازه(fs->super));
	إذا (err)
		اذهب بیرون;

	/* Does the inode have size for EA? */
	إذا (EXT2_INODE_اندازه(fs->super) <= EXT2_GOOD_OLD_INODE_اندازه +
						  inode->i_extra_isize +
						  حجم(__u32)) {
		err = EXT2_ET_INLINE_داده_NO_SPACE;
		اذهب بیرون;
	}

	minoff = EXT2_INODE_اندازه(fs->super) - حجم(*inode) - حجم(__u32);
	کپی_حافظه(&ea_inode_magic, ((حرف *) inode) + EXT2_GOOD_OLD_INODE_اندازه +
	       inode->i_extra_isize, حجم(__u32));
	إذا (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		شروع= ((حرف *) inode) + EXT2_GOOD_OLD_INODE_اندازه +
			inode->i_extra_isize + حجم(__u32);
		مدخل = (بنية ext2_ext_attr_مدخل *) شروع;
		طالما (!EXT2_EXT_IS_آخرین_مدخل(مدخل)) {
			إذا (!مدخل->e_مقدار_inum && مدخل->e_مقدار_اندازه) {
				بیعلامت صحيح offs = مدخل->e_مقدار_offs;
				إذا (offs < minoff)
					minoff = offs;
			}
			مدخل = EXT2_EXT_ATTR_بعدی(مدخل);
		}
		*اندازه = minoff - ((حرف *)مدخل - (حرف *)شروع) - حجم(__u32);
	} وإلا {
		/* no xattr.  return a maximum size */
		*اندازه = EXT2_EXT_ATTR_اندازه(minoff -
					   EXT2_EXT_ATTR_LEN(طول_رشته("data")) -
					   EXT2_EXT_ATTR_دور - حجم(__u32));
	}

بیرون:
	ext2fs_free_mem(&inode);
	أرجع err;
}

ساكن errcode_t xattr_create_ea_inode(ext2_filsys fs, ثابت فراغ *مقدار,
				       size_t مقدار_len, ext2_ino_t *ea_ino)
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
	inode.i_اندازه = 0;
	inode.i_نما = LINUX_S_IFREG | 0600;
	inode.i_links_شمار = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	إذا (ret)
		أرجع ret;
	/*
	 * ref_count and hash utilize inode's i_*time fields.
	 * ext2fs_write_new_inode() call above initializes these fields with
	 * current time. That's why ref count and hash updates are done
	 * separately below.
	 */
	ext2fs_مجموعه_ea_inode_ref(&inode, 1);
	hash = ext2fs_crc32c_le(fs->csum_دانه, مقدار, مقدار_len);
	ext2fs_مجموعه_ea_inode_hash(&inode, hash);

	ret = ext2fs_write_inode(fs, ino, &inode);
	إذا (ret)
		أرجع ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	إذا (ret)
		أرجع ret;
	ret = ext2fs_file_write(file, مقدار, مقدار_len, عدم);
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
	__u64 ref_شمار;
	errcode_t ret;

	ret = ext2fs_read_inode_پر(fs, ino, (بنية ext2_inode *)&inode,
				     حجم(inode));
	إذا (ret)
		اذهب بیرون;

	ref_شمار = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_شمار--;
	ext2fs_مجموعه_ea_inode_ref(EXT2_INODE(&inode), ref_شمار);

	إذا (ref_شمار)
		اذهب write_بیرون;

	inode.i_links_شمار = 0;
	ext2fs_مجموعه_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	إذا (ret)
		اذهب write_بیرون;

	إذا (ext2fs_inode_has_valid_blocks2(fs, (بنية ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (بنية ext2_inode *)&inode, عدم,
				   0, ~0ULL);
		إذا (ret)
			اذهب بیرون;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_بیرون:
	ret = ext2fs_write_inode_پر(fs, ino, (بنية ext2_inode *)&inode,
				      حجم(inode));
بیرون:
	أرجع ret;
}

ساكن errcode_t xattr_update_مدخل(ext2_filsys fs, بنية ext2_xattr *x,
				    ثابت حرف *name, ثابت حرف *short_name,
				    صحيح شاخص, ثابت فراغ *مقدار,
				    size_t مقدار_len, صحيح درون_inode)
{
	ext2_ino_t ea_ino = 0;
	فراغ *new_مقدار = عدم;
	حرف *new_name = عدم;
	صحيح name_len;
	errcode_t ret;

	إذا (!x->name) {
		name_len = طول_رشته(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		إذا (ret)
			اذهب fail;
		کپی_حافظه(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(مقدار_len, &new_مقدار);
	إذا (ret)
		اذهب fail;
	کپی_حافظه(new_مقدار, مقدار, مقدار_len);

	إذا (درون_inode) {
		ret = xattr_create_ea_inode(fs, مقدار, مقدار_len, &ea_ino);
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
	x->name_شاخص = شاخص;

	إذا (x->مقدار)
		ext2fs_free_mem(&x->مقدار);
	x->مقدار = new_مقدار;
	x->مقدار_len = مقدار_len;
	x->ea_ino = ea_ino;
	أرجع 0;
fail:
	إذا (new_name)
		ext2fs_free_mem(&new_name);
	إذا (new_مقدار)
		ext2fs_free_mem(&new_مقدار);
	إذا (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	أرجع ret;
}

ساكن صحيح xattr_پیدا_کن_position(بنية ext2_xattr *attrs, صحيح شمار,
			       ثابت حرف *shortname, صحيح name_idx)
{
	بنية ext2_xattr *x;
	صحيح i;
	صحيح shortname_len, x_shortname_len;

	shortname_len = طول_رشته(shortname);

	لكل (i = 0, x = attrs; i < شمار; i++, x++) {
		إذا (name_idx < x->name_شاخص)
			اكسر;
		إذا (name_idx > x->name_شاخص)
			استمر;

		x_shortname_len = طول_رشته(x->short_name);
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
				    ثابت فراغ *مقدار, size_t مقدار_len,
				    صحيح ibody_free, صحيح بلوک_free,
				    صحيح old_idx, صحيح درون_inode)
{
	بنية ext2_xattr tmp;
	صحيح جمع_کن_مقصد_ibody;
	صحيح needed;
	صحيح name_len, name_idx = 0;
	ثابت حرف *shortname = name;
	صحيح new_idx;
	صحيح ret;

	پیدا_کن_ea_شاخص(name, &shortname, &name_idx);
	name_len = طول_رشته(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	إذا (!درون_inode)
		needed += EXT2_EXT_ATTR_اندازه(مقدار_len);

	إذا (old_idx >= 0 && old_idx < h->ibody_شمار) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		إذا (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_اندازه(
						h->attrs[old_idx].مقدار_len);
	}

	إذا (needed <= ibody_free) {
		إذا (old_idx < 0) {
			new_idx = h->ibody_شمار;
			جمع_کن_مقصد_ibody = 1;
			اذهب جمع_کن_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_مدخل(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, مقدار,
					 مقدار_len, درون_inode);
		إذا (ret)
			أرجع ret;
		إذا (h->ibody_شمار <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_شمار + 1,
				h->attrs + h->ibody_شمار,
				(old_idx - h->ibody_شمار) * حجم(*h->attrs));
			h->attrs[h->ibody_شمار] = tmp;
			h->ibody_شمار++;
		}
		أرجع 0;
	}

	إذا (h->ibody_شمار <= old_idx) {
		بلوک_free += EXT2_EXT_ATTR_LEN(name_len);
		إذا (!h->attrs[old_idx].ea_ino)
			بلوک_free +=
				EXT2_EXT_ATTR_اندازه(h->attrs[old_idx].مقدار_len);
	}

	إذا (needed > بلوک_free)
		أرجع EXT2_ET_EA_NO_SPACE;

	إذا (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_مدخل(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, مقدار,
					 مقدار_len, درون_inode);
		إذا (ret)
			أرجع ret;
		إذا (old_idx < h->ibody_شمار) {
			/*
			 * Move entry from ibody to the block. Note that
			 * entries in the block are sorted.
			 */
			new_idx = xattr_پیدا_کن_position(h->attrs + h->ibody_شمار,
						      h->شمار - h->ibody_شمار,
						      shortname, name_idx);
			new_idx += h->ibody_شمار - 1;
			tmp = h->attrs[old_idx];
			memmove(h->attrs + old_idx, h->attrs + old_idx + 1,
				(new_idx - old_idx) * حجم(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_شمار--;
		}
		أرجع 0;
	}

	new_idx = xattr_پیدا_کن_position(h->attrs + h->ibody_شمار,
				      h->شمار - h->ibody_شمار,
				      shortname, name_idx);
	new_idx += h->ibody_شمار;
	جمع_کن_مقصد_ibody = 0;

جمع_کن_new:
	إذا (h->شمار == h->ظرفیت) {
		ret = ext2fs_xattrs_expand(h, 4);
		إذا (ret)
			أرجع ret;
	}

	ret = xattr_update_مدخل(h->fs, &h->attrs[h->شمار], name, shortname,
				 name_idx, مقدار, مقدار_len, درون_inode);
	إذا (ret)
		أرجع ret;

	tmp = h->attrs[h->شمار];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->شمار - new_idx)*حجم(*h->attrs));
	h->attrs[new_idx] = tmp;
	إذا (جمع_کن_مقصد_ibody)
		h->ibody_شمار++;
	h->شمار++;
	أرجع 0;
}

ساكن صحيح space_استفاده_شده(بنية ext2_xattr *attrs, صحيح شمار)
{
	صحيح مجموع = 0;
	بنية ext2_xattr *x;
	صحيح i, len;

	لكل (i = 0, x = attrs; i < شمار; i++, x++) {
		len = طول_رشته(x->short_name);
		مجموع += EXT2_EXT_ATTR_LEN(len);
		إذا (!x->ea_ino)
			مجموع += EXT2_EXT_ATTR_اندازه(x->مقدار_len);
	}
	أرجع مجموع;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#تعریف_کن EXT4_XATTR_MIN_LARGE_EA_اندازه(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - حجم(بنية ext2_ext_attr_سرآیند) - 4)

errcode_t ext2fs_xattr_مجموعه(بنية ext2_xattr_handle *h,
			   ثابت حرف *name,
			   ثابت فراغ *مقدار,
			   size_t مقدار_len)
{
	ext2_filsys fs = h->fs;
	ثابت صحيح inode_اندازه = EXT2_INODE_اندازه(fs->super);
	بنية ext2_inode_large *inode = عدم;
	بنية ext2_xattr *x;
	حرف *new_مقدار;
	صحيح ibody_free, بلوک_free;
	صحيح درون_inode = 0;
	صحيح old_idx = -1;
	صحيح extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(مقدار_len, &new_مقدار);
	إذا (ret)
		أرجع ret;
	إذا (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((strcmp(name, "system.posix_acl_default") == 0) ||
	     (strcmp(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_مقصد_disk_میانگیر(مقدار, مقدار_len,
						       new_مقدار, &مقدار_len);
		إذا (ret)
			اذهب بیرون;
	} وإلا إذا (مقدار_len)
		کپی_حافظه(new_مقدار, مقدار, مقدار_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	لكل (x = h->attrs; x < h->attrs + h->شمار; x++) {
		إذا (!strcmp(x->name, name)) {
			إذا (!x->ea_ino && x->مقدار_len == مقدار_len &&
			    (!مقدار_len ||
			     !memcmp(x->مقدار, new_مقدار, مقدار_len))) {
				ret = 0;
				اذهب بیرون;
			}
			old_idx = x - h->attrs;
			اكسر;
		}
	}

	ret = ext2fs_get_memzero(inode_اندازه, &inode);
	إذا (ret)
		اذهب بیرون;
	ret = ext2fs_read_inode_پر(fs, h->ino,
				     (بنية ext2_inode *)inode,
				     inode_اندازه);
	إذا (ret)
		اذهب بیرون;
	إذا (inode_اندازه > EXT2_GOOD_OLD_INODE_اندازه) {
		extra_isize = inode->i_extra_isize;
		إذا (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			إذا (extra_isize == 0)
				extra_isize = حجم(__u32);
		}
		ibody_free = inode_اندازه - EXT2_GOOD_OLD_INODE_اندازه;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= حجم(__u32) * 2;
		ibody_free -= space_استفاده_شده(h->attrs, h->ibody_شمار);
	} وإلا
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	إذا (strcmp(name, "system.data") == 0) {
		إذا (h->ibody_شمار <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			اذهب بیرون;
		}
		ret = xattr_array_update(h, name, new_مقدار, مقدار_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		إذا (ret)
			اذهب بیرون;
		اذهب write_بیرون;
	}

	بلوک_free = fs->blocksize;
	بلوک_free -= حجم(بنية ext2_ext_attr_سرآیند);
	/* Final null entry. */
	بلوک_free -= حجم(__u32);
	بلوک_free -= space_استفاده_شده(h->attrs + h->ibody_شمار,
				 h->شمار - h->ibody_شمار);

	إذا (ext2fs_has_feature_ea_inode(fs->super) &&
	    مقدار_len > EXT4_XATTR_MIN_LARGE_EA_اندازه(fs->blocksize))
		درون_inode = 1;

	ret = xattr_array_update(h, name, new_مقدار, مقدار_len, ibody_free,
				 بلوک_free, old_idx, درون_inode);
	إذا (ret == EXT2_ET_EA_NO_SPACE && !درون_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_مقدار, مقدار_len,
			ibody_free, بلوک_free, old_idx, 1 /* in_inode */);
	إذا (ret)
		اذهب بیرون;

write_بیرون:
	ret = ext2fs_xattrs_write(h);
بیرون:
	إذا (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_مقدار);
	أرجع ret;
}

errcode_t ext2fs_xattr_remove(بنية ext2_xattr_handle *handle,
			      ثابت حرف *کلید)
{
	بنية ext2_xattr *x;
	بنية ext2_xattr *انتها = handle->attrs + handle->شمار;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = handle->attrs; x < انتها; x++) {
		إذا (strcmp(x->name, کلید) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->مقدار);
			إذا (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (انتها - x - 1)*حجم(*x));
			memset(انتها - 1, 0, حجم(*انتها));
			إذا (x < handle->attrs + handle->ibody_شمار)
				handle->ibody_شمار--;
			handle->شمار--;
			أرجع ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	أرجع 0;
}

errcode_t ext2fs_xattr_remove_all(بنية ext2_xattr_handle *handle)
{
	بنية ext2_xattr *x;
	بنية ext2_xattr *انتها = handle->attrs + handle->شمار;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = handle->attrs; x < انتها; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->مقدار);
		إذا (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_شمار = 0;
	handle->شمار = 0;
	أرجع ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_xattr_handle **handle)
{
	بنية ext2_xattr_handle *h;
	errcode_t err;

	إذا (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_داده(fs->super))
		أرجع EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(حجم(*h), &h);
	إذا (err)
		أرجع err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->ظرفیت = 4;
	err = ext2fs_get_arrayzero(h->ظرفیت, حجم(بنية ext2_xattr),
				   &h->attrs);
	إذا (err) {
		ext2fs_free_mem(&h);
		أرجع err;
	}
	h->شمار = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	أرجع 0;
}

errcode_t ext2fs_xattrs_close(بنية ext2_xattr_handle **handle)
{
	بنية ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_کلیدها(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	أرجع 0;
}

errcode_t ext2fs_xattrs_شمار(بنية ext2_xattr_handle *handle, size_t *شمار)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*شمار = handle->شمار;
	أرجع 0;
}

errcode_t ext2fs_xattrs_flags(بنية ext2_xattr_handle *handle,
			      بیعلامت صحيح *new_flags, بیعلامت صحيح *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	إذا (old_flags)
		*old_flags = handle->flags;
	إذا (new_flags)
		handle->flags = *new_flags;
	أرجع 0;
}
