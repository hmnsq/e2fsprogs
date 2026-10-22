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
#د_که_پای
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

#تعریف_کړه NAME_HASH_ښويدنه 5
#تعریف_کړه ارزښت_HASH_ښويدنه 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_ثبت(بنية ext2_ext_attr_ثبت *ثبت, فراغ *معلومات)
{
	__u32 hash = 0;
	بینښه حرف *name = (((بینښه حرف *) ثبت) +
			       حجم(بنية ext2_ext_attr_ثبت));
	صحيح n;

	لكل (n = 0; n < ثبت->e_name_len; n++) {
		hash = (hash << NAME_HASH_ښويدنه) ^
		       (hash >> (8*حجم(hash) - NAME_HASH_ښويدنه)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	إذا (ثبت->e_ارزښت_inum == 0 && ثبت->e_ارزښت_اندازه != 0) {
		__u32 *ارزښت = (__u32 *)معلومات;
		لكل (n = (ثبت->e_ارزښت_اندازه + EXT2_EXT_ATTR_پړاو) >>
			 EXT2_EXT_ATTR_PAD_بټونه; n; n--) {
			hash = (hash << ارزښت_HASH_ښويدنه) ^
			       (hash >> (8*حجم(hash) - ارزښت_HASH_ښويدنه)) ^
			       ext2fs_le32_منزل_cpu(*ارزښت++);
		}
	}

	أرجع hash;
}

__u32 ext2fs_ext_attr_hash_ثبت_signed(بنية ext2_ext_attr_ثبت *ثبت,
					فراغ *معلومات)
{
	__u32 hash = 0;
	نښهدار حرف *name = (((نښهدار حرف *) ثبت) +
			     حجم(بنية ext2_ext_attr_ثبت));
	صحيح n;

	لكل (n = 0; n < ثبت->e_name_len; n++) {
		hash = (hash << NAME_HASH_ښويدنه) ^
		       (hash >> (8*حجم(hash) - NAME_HASH_ښويدنه)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	إذا (ثبت->e_ارزښت_inum == 0 && ثبت->e_ارزښت_اندازه != 0) {
		__u32 *ارزښت = (__u32 *)معلومات;
		لكل (n = (ثبت->e_ارزښت_اندازه + EXT2_EXT_ATTR_پړاو) >>
			 EXT2_EXT_ATTR_PAD_بټونه; n; n--) {
			hash = (hash << ارزښت_HASH_ښويدنه) ^
			       (hash >> (8*حجم(hash) - ارزښت_HASH_ښويدنه)) ^
			       ext2fs_le32_منزل_cpu(*ارزښت++);
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
				      بنية ext2_ext_attr_ثبت *ثبت,
				      فراغ *معلومات, __u32 *hash,
				      __u32 *signed_hash)
{
	*hash = ext2fs_ext_attr_hash_ثبت(ثبت, معلومات);
	إذا (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_ثبت_signed(ثبت, معلومات);

	إذا (ثبت->e_ارزښت_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, ثبت->e_ارزښت_inum,
					    &ea_inode_hash);
		إذا (retval)
			أرجع retval;

		*hash = (*hash << ارزښت_HASH_ښويدنه) ^
			(*hash >> (8*حجم(*hash) - ارزښت_HASH_ښويدنه)) ^
			ea_inode_hash;
		إذا (signed_hash)
			*signed_hash = (*signed_hash << ارزښت_HASH_ښويدنه) ^
				(*signed_hash >> (8*حجم(*hash) -
						  ارزښت_HASH_ښويدنه)) ^
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
				      بنية ext2_ext_attr_ثبت *ثبت,
				      فراغ *معلومات, __u32 *hash)
{
	أرجع ext2fs_ext_attr_hash_entry3(fs, ثبت, معلومات, hash, عدم);
}

#تعریف_لغوه NAME_HASH_ښويدنه
#تعریف_لغوه ارزښت_HASH_ښويدنه

#تعریف_کړه بلاک_HASH_ښويدنه 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
فراغ ext2fs_ext_attr_بلاک_rehash(بنية ext2_ext_attr_سرلیک *سرلیک,
				  بنية ext2_ext_attr_ثبت *پای_ټکی)
{
	بنية ext2_ext_attr_ثبت *دلته;
	__u32 hash = 0;

	دلته = (بنية ext2_ext_attr_ثبت *)(سرلیک+1);
	طالما (دلته < پای_ټکی && !EXT2_EXT_IS_وروستی_ثبت(دلته)) {
		إذا (!دلته->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			اكسر;
		}
		hash = (hash << بلاک_HASH_ښويدنه) ^
		       (hash >> (8*حجم(hash) - بلاک_HASH_ښويدنه)) ^
		       دلته->e_hash;
		دلته = EXT2_EXT_ATTR_بل(دلته);
	}
	سرلیک->h_hash = hash;
}

#تعریف_لغوه بلاک_HASH_ښويدنه

__u32 ext2fs_get_ea_inode_hash(بنية ext2_inode *inode)
{
	أرجع inode->i_atime;
}

فراغ ext2fs_سېټ_ea_inode_hash(بنية ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(بنية ext2_inode *inode)
{
	أرجع ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_نسخه;
}

فراغ ext2fs_سېټ_ea_inode_ref(بنية ext2_inode *inode, __u64 ref_شمېر)
{
	inode->i_ctime = (__u32)(ref_شمېر >> 32);
	inode->osd1.linux1.l_i_نسخه = (__u32)ref_شمېر;
}

ساكن errcode_t check_ext_attr_سرلیک(بنية ext2_ext_attr_سرلیک *سرلیک)
{
	إذا ((سرلیک->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     سرلیک->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    سرلیک->h_بلاکونه != 1)
		أرجع EXT2_ET_BAD_EA_سرلیک;

	أرجع 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t بلاک, فراغ *buf,
				ext2_ino_t inum)
{
	صحيح		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, بلاک, 1, buf);
	إذا (retval)
		أرجع retval;

	إذا (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_بلاک_csum_verify(fs, inum, بلاک, buf))
		csum_failed = 1;

#که_تعریف_شوی ټکي_BIGENDIAN
	ext2fs_بدلون_ext_attr(buf, buf, fs->blocksize, 1);
#د_که_پای

	retval = check_ext_attr_سرلیک(buf);
	إذا (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	أرجع retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t بلاک, فراغ *buf)
{
	أرجع ext2fs_read_ext_attr3(fs, بلاک, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t بلاک, فراغ *buf)
{
	أرجع ext2fs_read_ext_attr2(fs, بلاک, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t بلاک, فراغ *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	حرف		*write_buf;

#که_تعریف_شوی ټکي_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	إذا (retval)
		أرجع retval;
	ext2fs_بدلون_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#وإلا
	write_buf = (حرف *) inbuf;
#د_که_پای

	retval = ext2fs_ext_attr_بلاک_csum_سېټ(fs, inum, بلاک,
			(بنية ext2_ext_attr_سرلیک *)write_buf);
	إذا (retval)
		أرجع retval;

	retval = io_channel_write_blk64(fs->io, بلاک, 1, write_buf);
#که_تعریف_شوی ټکي_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#د_که_پای
	إذا (!retval)
		ext2fs_mark_بدل_شوی(fs);
	أرجع retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t بلاک, فراغ *inbuf)
{
	أرجع ext2fs_write_ext_attr3(fs, بلاک, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t بلاک, فراغ *inbuf)
{
	أرجع ext2fs_write_ext_attr2(fs, بلاک, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    حرف *بلاک_buf, صحيح adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	بنية ext2_ext_attr_سرلیک *سرلیک;
	حرف	*buf = 0;

	إذا ((blk >= ext2fs_بلاکونه_شمېر(fs->super)) ||
	    (blk < fs->super->s_لومړی_معلومات_بلاک))
		أرجع EXT2_ET_BAD_EA_بلاک_عدد;

	إذا (!بلاک_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		إذا (retval)
			أرجع retval;
		بلاک_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, بلاک_buf, inum);
	إذا (retval)
		اذهب errout;

	سرلیک = (بنية ext2_ext_attr_سرلیک *) بلاک_buf;
	سرلیک->h_refcount += adjust;
	إذا (newcount)
		*newcount = سرلیک->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, بلاک_buf, inum);
	إذا (retval)
		اذهب errout;

errout:
	إذا (buf)
		ext2fs_free_mem(&buf);
	أرجع retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    حرف *بلاک_buf, صحيح adjust,
				    __u32 *newcount)
{
	أرجع ext2fs_adjust_ea_refcount3(fs, blk, بلاک_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					حرف *بلاک_buf, صحيح adjust,
					__u32 *newcount)
{
	أرجع ext2fs_adjust_ea_refcount2(fs, blk, بلاک_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
بنية ext2_xattr {
	صحيح name_شاخص;
	حرف *name;
	حرف *short_name;
	فراغ *ارزښت;
	بینښه صحيح ارزښت_len;
	ext2_ino_t ea_ino;
};

بنية ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	بنية ext2_xattr *attrs;
	صحيح ظرفیت;
	صحيح شمېر;
	صحيح ibody_شمېر;
	ext2_ino_t ino;
	بینښه صحيح flags;
};

ساكن errcode_t ext2fs_xattrs_expand(بنية ext2_xattr_handle *h,
				      بینښه صحيح expandby)
{
	بنية ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->ظرفیت + expandby,
				   حجم(بنية ext2_xattr), &new_attrs);
	إذا (err)
		أرجع err;

	حافظه_نقل(new_attrs, h->attrs, h->ظرفیت * حجم(بنية ext2_xattr));
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
ساكن بنية ea_name_شاخص ea_نومونه[] = {
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

ساكن ثابت حرف *پیدا_کړه_ea_مختاړی(صحيح شاخص)
{
	بنية ea_name_شاخص *e;

	لكل (e = ea_نومونه; e->name; e++)
		إذا (e->شاخص == شاخص)
			أرجع e->name;

	أرجع عدم;
}

ساكن صحيح پیدا_کړه_ea_شاخص(ثابت حرف *fullname, ثابت حرف **name, صحيح *شاخص)
{
	بنية ea_name_شاخص *e;

	لكل (e = ea_نومونه; e->name; e++) {
		إذا (مزي_پرتله(fullname, e->name, مزي_اوږدوالی(e->name)) == 0) {
			*name = fullname + مزي_اوږدوالی(e->name);
			*شاخص = e->شاخص;
			أرجع 1;
		}
	}
	أرجع 0;
}

errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       بنية ext2_inode_large *inode)
{
	بنية ext2_ext_attr_سرلیک *سرلیک;
	فراغ *بلاک_buf = عدم;
	blk64_t blk;
	errcode_t err;
	بنية ext2_inode_large i;

	/* Read inode? */
	إذا (inode == عدم) {
		err = ext2fs_read_inode_ډک(fs, ino, (بنية ext2_inode *)&i,
					     حجم(بنية ext2_inode_large));
		إذا (err)
			أرجع err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_بلاک(fs, (بنية ext2_inode *)inode);
	إذا (blk == 0)
		أرجع 0;

	/* Find block, zero it, write back */
	إذا ((blk < fs->super->s_لومړی_معلومات_بلاک) ||
	    (blk >= ext2fs_بلاکونه_شمېر(fs->super))) {
		err = EXT2_ET_BAD_EA_بلاک_عدد;
		اذهب بهر;
	}

	err = ext2fs_get_mem(fs->blocksize, &بلاک_buf);
	إذا (err)
		اذهب بهر;

	err = ext2fs_read_ext_attr3(fs, blk, بلاک_buf, ino);
	إذا (err)
		اذهب out2;

	/* We only know how to deal with v2 EA blocks */
	سرلیک = (بنية ext2_ext_attr_سرلیک *) بلاک_buf;
	إذا (سرلیک->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_سرلیک;
		اذهب out2;
	}

	سرلیک->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, بلاک_buf, ino);
	إذا (err)
		اذهب out2;

	/* Erase link to block */
	ext2fs_file_acl_بلاک_سېټ(fs, (بنية ext2_inode *)inode, 0);
	إذا (سرلیک->h_refcount == 0)
		ext2fs_بلاک_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_بلاکونه(fs, (بنية ext2_inode *)inode, 1);
	إذا (err)
		اذهب out2;

	/* Write inode? */
	إذا (inode == &i) {
		err = ext2fs_write_inode_ډک(fs, ino, (بنية ext2_inode *)&i,
					      حجم(بنية ext2_inode_large));
		إذا (err)
			اذهب out2;
	}

out2:
	ext2fs_free_mem(&بلاک_buf);
بهر:
	أرجع err;
}

ساكن errcode_t prep_ea_بلاک_for_write(ext2_filsys fs, ext2_ino_t ino,
					 بنية ext2_inode_large *inode)
{
	بنية ext2_ext_attr_سرلیک *سرلیک;
	فراغ *بلاک_buf = عدم;
	blk64_t blk, goal;
	errcode_t err;

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_بلاک(fs, (بنية ext2_inode *)inode);
	إذا (blk != 0) {
		إذا ((blk < fs->super->s_لومړی_معلومات_بلاک) ||
		    (blk >= ext2fs_بلاکونه_شمېر(fs->super))) {
			err = EXT2_ET_BAD_EA_بلاک_عدد;
			اذهب بهر;
		}

		err = ext2fs_get_mem(fs->blocksize, &بلاک_buf);
		إذا (err)
			اذهب بهر;

		err = ext2fs_read_ext_attr3(fs, blk, بلاک_buf, ino);
		إذا (err)
			اذهب out2;

		/* We only know how to deal with v2 EA blocks */
		سرلیک = (بنية ext2_ext_attr_سرلیک *) بلاک_buf;
		إذا (سرلیک->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_سرلیک;
			اذهب out2;
		}

		/* Single-user block.  We're done here. */
		إذا (سرلیک->h_refcount == 1)
			اذهب out2;

		/* We need to CoW the block. */
		سرلیک->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, بلاک_buf, ino);
		إذا (err)
			اذهب out2;
	} وإلا {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_جمع_کړه_بلاکونه(fs, (بنية ext2_inode *)inode,
					     1);
		إذا (err)
			اذهب بهر;
	}

	/* Allocate a block */
	goal = ext2fs_پیدا_کړه_inode_goal(fs, ino, (بنية ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, عدم, &blk);
	إذا (err)
		اذهب out2;
	ext2fs_file_acl_بلاک_سېټ(fs, (بنية ext2_inode *)inode, blk);
out2:
	إذا (بلاک_buf)
		ext2fs_free_mem(&بلاک_buf);
بهر:
	أرجع err;
}


ساكن مضمن صحيح
posix_acl_xattr_شمېر(size_t اندازه)
{
        إذا (اندازه < حجم(posix_acl_xattr_سرلیک))
                أرجع -1;
        اندازه -= حجم(posix_acl_xattr_سرلیک);
        إذا (اندازه % حجم(posix_acl_xattr_ثبت))
                أرجع -1;
        أرجع اندازه / حجم(posix_acl_xattr_ثبت);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
ساكن errcode_t convert_posix_acl_منزل_disk_بفر(ثابت فراغ *ارزښت, size_t اندازه,
						  فراغ *بهر_buf, size_t *اندازه_بهر)
{
	ثابت posix_acl_xattr_سرلیک *سرلیک =
		(ثابت posix_acl_xattr_سرلیک*) ارزښت;
	ثابت posix_acl_xattr_ثبت *پای_ټکی, *ثبت =
		(ثابت posix_acl_xattr_ثبت *)(سرلیک+1);
	ext4_acl_سرلیک *ext_acl;
	size_t s;
	حرف *e;

	صحيح شمېر;

	إذا (!ارزښت)
		أرجع EINVAL;
	إذا (اندازه < حجم(posix_acl_xattr_سرلیک))
		أرجع ENOMEM;
	إذا (سرلیک->a_نسخه != ext2fs_cpu_منزل_le32(POSIX_ACL_XATTR_نسخه))
		أرجع EINVAL;

	شمېر = posix_acl_xattr_شمېر(اندازه);
	ext_acl = بهر_buf;
	ext_acl->a_نسخه = ext2fs_cpu_منزل_le32(EXT4_ACL_نسخه);

	إذا (شمېر <= 0)
		أرجع EINVAL;

	e = (حرف *) بهر_buf + حجم(ext4_acl_سرلیک);
	s = حجم(ext4_acl_سرلیک);
	لكل (پای_ټکی = ثبت + شمېر; ثبت != پای_ټکی;ثبت++) {
		ext4_acl_ثبت *disk_ثبت = (ext4_acl_ثبت*) e;
		disk_ثبت->e_نښان = ثبت->e_نښان;
		disk_ثبت->e_perm = ثبت->e_perm;

		بدل(ext2fs_le16_منزل_cpu(ثبت->e_نښان)) {
			حالة ACL_USER_OBJ:
			حالة ACL_ډله_OBJ:
			حالة ACL_ماسک:
			حالة ACL_OTHER:
				e += حجم(ext4_acl_ثبت_short);
				s += حجم(ext4_acl_ثبت_short);
				اكسر;
			حالة ACL_USER:
			حالة ACL_ډله:
				disk_ثبت->e_id = ثبت->e_id;
				e += حجم(ext4_acl_ثبت);
				s += حجم(ext4_acl_ثبت);
				اكسر;
			افتراضي:
				أرجع EINVAL;
		}
	}
	*اندازه_بهر = s;
	أرجع 0;
}

ساكن errcode_t convert_disk_بفر_منزل_posix_acl(ثابت فراغ *ارزښت, size_t اندازه,
						  فراغ **بهر_buf, size_t *اندازه_بهر)
{
	posix_acl_xattr_سرلیک *سرلیک;
	posix_acl_xattr_ثبت *ثبت;
	ثابت ext4_acl_سرلیک *ext_acl = (ثابت ext4_acl_سرلیک *) ارزښت;
	errcode_t err;
	ثابت حرف *cp;
	حرف *بهر;

	إذا ((!ارزښت) ||
	    (اندازه < حجم(ext4_acl_سرلیک)) ||
	    (ext_acl->a_نسخه != ext2fs_cpu_منزل_le32(EXT4_ACL_نسخه)))
		أرجع EINVAL;

	err = ext2fs_get_mem(اندازه * 2, &بهر);
	إذا (err)
		أرجع err;

	سرلیک = (posix_acl_xattr_سرلیک *) بهر;
	سرلیک->a_نسخه = ext2fs_cpu_منزل_le32(POSIX_ACL_XATTR_نسخه);
	ثبت = (posix_acl_xattr_ثبت *) (بهر + حجم(posix_acl_xattr_سرلیک));

	cp = (ثابت حرف *) ارزښت + حجم(ext4_acl_سرلیک);
	اندازه -= حجم(ext4_acl_سرلیک);

	طالما (اندازه > 0) {
		ثابت ext4_acl_ثبت *disk_ثبت = (ثابت ext4_acl_ثبت *) cp;

		ثبت->e_نښان = disk_ثبت->e_نښان;
		ثبت->e_perm = disk_ثبت->e_perm;

		بدل(ext2fs_le16_منزل_cpu(ثبت->e_نښان)) {
			حالة ACL_USER_OBJ:
			حالة ACL_ډله_OBJ:
			حالة ACL_ماسک:
			حالة ACL_OTHER:
				ثبت->e_id = 0;
				cp += حجم(ext4_acl_ثبت_short);
				اندازه -= حجم(ext4_acl_ثبت_short);
				اكسر;
			حالة ACL_USER:
			حالة ACL_ډله:
				ثبت->e_id = disk_ثبت->e_id;
				cp += حجم(ext4_acl_ثبت);
				اندازه -= حجم(ext4_acl_ثبت);
				اكسر;
			افتراضي:
				ext2fs_free_mem(&بهر);
				أرجع EINVAL;
		}
		ثبت++;
	}
	*بهر_buf = بهر;
	*اندازه_بهر = ((حرف *) ثبت - بهر);
	أرجع 0;
}

ساكن errcode_t
write_xattrs_منزل_بفر(ext2_filsys fs, بنية ext2_xattr *attrs, صحيح شمېر,
		       فراغ *entries_پیل, بینښه صحيح storage_اندازه,
		       بینښه صحيح ارزښت_دننه_واټن_correction, صحيح write_hash)
{
	بنية ext2_xattr *x;
	بنية ext2_ext_attr_ثبت *e = entries_پیل;
	حرف *پای_ټکی = (حرف *) entries_پیل + storage_اندازه;
	بینښه صحيح ارزښت_اندازه;
	errcode_t err;

	memset(entries_پیل, 0, storage_اندازه);
	لكل (x = attrs; x < attrs + شمېر; x++) {
		ارزښت_اندازه = ((x->ارزښت_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = مزي_اوږدوالی(x->short_name);
		e->e_name_شاخص = x->name_شاخص;

		e->e_ارزښت_اندازه = x->ارزښت_len;
		e->e_ارزښت_inum = x->ea_ino;

		/* Store name */
		حافظه_نقل((حرف *)e + حجم(*e), x->short_name, e->e_name_len);
		إذا (x->ea_ino) {
			e->e_ارزښت_offs = 0;
		} وإلا {
			پای_ټکی -= ارزښت_اندازه;
			e->e_ارزښت_offs = پای_ټکی - (حرف *) entries_پیل +
						ارزښت_دننه_واټن_correction;
			حافظه_نقل(پای_ټکی, x->ارزښت, e->e_ارزښت_اندازه);
		}

		إذا (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : پای_ټکی,
							  &e->e_hash);
			إذا (err)
				أرجع err;
		} وإلا
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_بل(e);
		*(__u32 *)e = 0;
	}
	أرجع 0;
}

errcode_t ext2fs_xattrs_write(بنية ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	ثابت بینښه صحيح inode_اندازه = EXT2_INODE_اندازه(fs->super);
	بنية ext2_inode_large *inode;
	حرف *پیل, *بلاک_buf = عدم;
	بنية ext2_ext_attr_سرلیک *سرلیک;
	__u32 ea_inode_magic;
	blk64_t blk;
	بینښه صحيح storage_اندازه;
	بینښه صحيح i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_اندازه;
	إذا (i < حجم(*inode))
		i = حجم(*inode);
	err = ext2fs_get_memzero(i, &inode);
	إذا (err)
		أرجع err;

	err = ext2fs_read_inode_ډک(fs, handle->ino, EXT2_INODE(inode),
				     inode_اندازه);
	إذا (err)
		اذهب بهر;

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
		اذهب بهر;
	}

	/* Does the inode have space for EA? */
	إذا (inode->i_extra_isize < حجم(inode->i_extra_isize) ||
	    inode_اندازه <= EXT2_GOOD_OLD_INODE_اندازه + inode->i_extra_isize +
								حجم(__u32))
		اذهب write_ea_بلاک;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	حافظه_نقل(((حرف *) inode) + EXT2_GOOD_OLD_INODE_اندازه +
	       inode->i_extra_isize, &ea_inode_magic, حجم(__u32));
	storage_اندازه = inode_اندازه - EXT2_GOOD_OLD_INODE_اندازه -
				inode->i_extra_isize - حجم(__u32);
	پیل = ((حرف *) inode) + EXT2_GOOD_OLD_INODE_اندازه +
				inode->i_extra_isize + حجم(__u32);

	err = write_xattrs_منزل_بفر(fs, handle->attrs, handle->ibody_شمېر,
				     پیل, storage_اندازه, 0, 0);
	إذا (err)
		اذهب بهر;
write_ea_بلاک:
	/* Are we done? */
	إذا (handle->ibody_شمېر == handle->شمېر &&
	    !ext2fs_file_acl_بلاک(fs, EXT2_INODE(inode)))
		اذهب skip_ea_بلاک;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &بلاک_buf);
	إذا (err)
		اذهب بهر;

	storage_اندازه = fs->blocksize - حجم(بنية ext2_ext_attr_سرلیک);
	پیل = بلاک_buf + حجم(بنية ext2_ext_attr_سرلیک);

	err = write_xattrs_منزل_بفر(fs, handle->attrs + handle->ibody_شمېر,
				     handle->شمېر - handle->ibody_شمېر, پیل,
				     storage_اندازه, پیل - بلاک_buf, 1);
	إذا (err)
		اذهب out2;

	/* Write a header on the EA block */
	سرلیک = (بنية ext2_ext_attr_سرلیک *) بلاک_buf;
	سرلیک->h_magic = EXT2_EXT_ATTR_MAGIC;
	سرلیک->h_refcount = 1;
	سرلیک->h_بلاکونه = 1;

	/* Get a new block for writing */
	err = prep_ea_بلاک_for_write(fs, handle->ino, inode);
	إذا (err)
		اذهب out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_بلاک(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, بلاک_buf, handle->ino);
	إذا (err)
		اذهب out2;

skip_ea_بلاک:
	blk = ext2fs_file_acl_بلاک(fs, (بنية ext2_inode *)inode);
	إذا (!بلاک_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		إذا (err)
			اذهب بهر;
	}

	/* Write the inode */
	err = ext2fs_write_inode_ډک(fs, handle->ino, EXT2_INODE(inode),
				      inode_اندازه);
	إذا (err)
		اذهب out2;

out2:
	ext2fs_free_mem(&بلاک_buf);
بهر:
	ext2fs_free_mem(&inode);
	أرجع err;
}

ساكن errcode_t read_xattrs_from_بفر(بنية ext2_xattr_handle *handle,
					 بنية ext2_inode_large *inode,
					 بنية ext2_ext_attr_ثبت *entries,
					 بینښه صحيح storage_اندازه,
					 حرف *ارزښت_پیل)
{
	بنية ext2_xattr *x;
	بنية ext2_ext_attr_ثبت *ثبت, *پای_ټکی;
	ثابت حرف *مختاړی;
	بینښه صحيح remain, مختاړی_len;
	errcode_t err;
	بینښه صحيح ارزښتونه_اندازه = storage_اندازه +
			((حرف *)entries - ارزښت_پیل);

	/* find the end */
	پای_ټکی = entries;
	remain = storage_اندازه;
	طالما (remain >= حجم(بنية ext2_ext_attr_ثبت) &&
	       !EXT2_EXT_IS_وروستی_ثبت(پای_ټکی)) {

		/* header eats this space */
		remain -= حجم(بنية ext2_ext_attr_ثبت);

		/* is attribute name valid? */
		إذا (EXT2_EXT_ATTR_اندازه(پای_ټکی->e_name_len) > remain)
			أرجع EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_اندازه(پای_ټکی->e_name_len);
		پای_ټکی = EXT2_EXT_ATTR_بل(پای_ټکی);
	}

	ثبت = entries;
	remain = storage_اندازه;
	طالما (remain >= حجم(بنية ext2_ext_attr_ثبت) &&
	       !EXT2_EXT_IS_وروستی_ثبت(ثبت)) {

		/* Allocate space for more attrs? */
		إذا (handle->شمېر == handle->ظرفیت) {
			err = ext2fs_xattrs_expand(handle, 4);
			إذا (err)
				أرجع err;
		}

		x = handle->attrs + handle->شمېر;

		/* header eats this space */
		remain -= حجم(بنية ext2_ext_attr_ثبت);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_اندازه(ثبت->e_name_len);

		/* Extract name */
		مختاړی = پیدا_کړه_ea_مختاړی(ثبت->e_name_شاخص);
		مختاړی_len = (مختاړی ? مزي_اوږدوالی(مختاړی) : 0);
		err = ext2fs_get_memzero(ثبت->e_name_len + مختاړی_len + 1,
					 &x->name);
		إذا (err)
			أرجع err;
		إذا (مختاړی)
			حافظه_نقل(x->name, مختاړی, مختاړی_len);
		إذا (ثبت->e_name_len)
			حافظه_نقل(x->name + مختاړی_len,
			       (حرف *)ثبت + حجم(*ثبت),
			       ثبت->e_name_len);
		x->short_name = x->name + مختاړی_len;
		x->name_شاخص = ثبت->e_name_شاخص;

		/* Check & copy value */
		إذا (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    ثبت->e_ارزښت_inum != 0)
			أرجع EXT2_ET_BAD_EA_بلاک_عدد;

		إذا (ثبت->e_ارزښت_inum == 0) {
			إذا (ثبت->e_ارزښت_اندازه > remain)
				أرجع EXT2_ET_EA_BAD_ارزښت_اندازه;

			إذا (ثبت->e_ارزښت_offs + ثبت->e_ارزښت_اندازه > ارزښتونه_اندازه)
				أرجع EXT2_ET_EA_BAD_ارزښت_دننه_واټن;

			إذا (ثبت->e_ارزښت_اندازه > 0 &&
			    ارزښت_پیل + ثبت->e_ارزښت_offs <
			    (حرف *)پای_ټکی + حجم(__u32))
				أرجع EXT2_ET_EA_BAD_ارزښت_دننه_واټن;

			remain -= ثبت->e_ارزښت_اندازه;

			err = ext2fs_get_mem(ثبت->e_ارزښت_اندازه, &x->ارزښت);
			إذا (err)
				أرجع err;
			حافظه_نقل(x->ارزښت, ارزښت_پیل + ثبت->e_ارزښت_offs,
			       ثبت->e_ارزښت_اندازه);
		} وإلا {
			بنية ext2_inode *ea_inode;
			ext2_file_t ea_file;

			إذا (ثبت->e_ارزښت_offs != 0)
				أرجع EXT2_ET_EA_BAD_ارزښت_دننه_واټن;

			إذا (ثبت->e_ارزښت_اندازه > (64 * 1024))
				أرجع EXT2_ET_EA_BAD_ارزښت_اندازه;

			err = ext2fs_get_mem(ثبت->e_ارزښت_اندازه, &x->ارزښت);
			إذا (err)
				أرجع err;

			err = ext2fs_file_open(handle->fs, ثبت->e_ارزښت_inum,
					       0, &ea_file);
			إذا (err)
				أرجع err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			إذا ((ea_inode->i_flags & EXT4_INLINE_معلومات_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_شمېر == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			وإلا إذا ((__u64) ext2fs_file_get_اندازه(ea_file) !=
				 ثبت->e_ارزښت_اندازه)
				err = EXT2_ET_EA_BAD_ارزښت_اندازه;
			وإلا
				err = ext2fs_file_read(ea_file, x->ارزښت,
						       ثبت->e_ارزښت_اندازه, 0);
			ext2fs_file_close(ea_file);
			إذا (err)
				أرجع err;
		}

		x->ea_ino = ثبت->e_ارزښت_inum;
		x->ارزښت_len = ثبت->e_ارزښت_اندازه;

		/* e_hash may be 0 in older inode's ea */
		إذا (ثبت->e_hash != 0) {
			__u32 hash, signed_hash;

			فراغ *معلومات = (ثبت->e_ارزښت_inum != 0) ?
					0 : ارزښت_پیل + ثبت->e_ارزښت_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, ثبت,
							  معلومات, &hash,
							  &signed_hash);
			إذا (err)
				أرجع err;
			إذا ((ثبت->e_hash != hash) &&
			    (ثبت->e_hash != signed_hash)) {
				بنية ext2_inode ماشوم;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							ثبت->e_ارزښت_inum,
							&ماشوم);
				إذا (err)
					أرجع err;
				إذا (ماشوم.i_mtime != handle->ino ||
				    ماشوم.i_generation != inode->i_generation)
					أرجع EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->شمېر++;
		ثبت = EXT2_EXT_ATTR_بل(ثبت);
	}

	أرجع 0;
}

ساكن فراغ xattrs_free_کلیګانې(بنية ext2_xattr_handle *h)
{
	بنية ext2_xattr *a = h->attrs;
	صحيح i;

	لكل (i = 0; i < h->ظرفیت; i++) {
		إذا (a[i].name)
			ext2fs_free_mem(&a[i].name);
		إذا (a[i].ارزښت)
			ext2fs_free_mem(&a[i].ارزښت);
	}
	h->شمېر = 0;
	h->ibody_شمېر = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(بنية ext2_xattr_handle *handle,
				   بنية ext2_inode_large *inode)
{
	بنية ext2_ext_attr_سرلیک *سرلیک;
	__u32 ea_inode_magic;
	بینښه صحيح storage_اندازه;
	حرف *پیل, *بلاک_buf = عدم;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_کلیګانې(handle);

	/* Does the inode have space for EA? */
	إذا (inode->i_extra_isize < حجم(inode->i_extra_isize) ||
	    EXT2_INODE_اندازه(handle->fs->super) <= EXT2_GOOD_OLD_INODE_اندازه +
						  inode->i_extra_isize +
						  حجم(__u32))
		اذهب read_ea_بلاک;
	إذا (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		اذهب بهر;
	}

	/* Look for EA in the inode */
	حافظه_نقل(&ea_inode_magic, ((حرف *) inode) + EXT2_GOOD_OLD_INODE_اندازه +
	       inode->i_extra_isize, حجم(__u32));
	إذا (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_اندازه = EXT2_INODE_اندازه(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_اندازه - inode->i_extra_isize -
			حجم(__u32);
		پیل = ((حرف *) inode) + EXT2_GOOD_OLD_INODE_اندازه +
			inode->i_extra_isize + حجم(__u32);

		err = read_xattrs_from_بفر(handle, inode,
					(بنية ext2_ext_attr_ثبت *) پیل,
					storage_اندازه, پیل);
		إذا (err)
			اذهب بهر;

		handle->ibody_شمېر = handle->شمېر;
	}

read_ea_بلاک:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_بلاک(handle->fs, EXT2_INODE(inode));
	إذا (blk != 0) {
		إذا ((blk < handle->fs->super->s_لومړی_معلومات_بلاک) ||
		    (blk >= ext2fs_بلاکونه_شمېر(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_بلاک_عدد;
			اذهب بهر;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &بلاک_buf);
		إذا (err)
			اذهب بهر;

		err = ext2fs_read_ext_attr3(handle->fs, blk, بلاک_buf,
					    handle->ino);
		إذا (err)
			اذهب out3;

		/* We only know how to deal with v2 EA blocks */
		سرلیک = (بنية ext2_ext_attr_سرلیک *) بلاک_buf;
		إذا (سرلیک->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_سرلیک;
			اذهب out3;
		}

		/* Read EAs */
		storage_اندازه = handle->fs->blocksize -
			حجم(بنية ext2_ext_attr_سرلیک);
		پیل = بلاک_buf + حجم(بنية ext2_ext_attr_سرلیک);
		err = read_xattrs_from_بفر(handle, inode,
					(بنية ext2_ext_attr_ثبت *) پیل,
					storage_اندازه, بلاک_buf);
	}

out3:
	إذا (بلاک_buf)
		ext2fs_free_mem(&بلاک_buf);
بهر:
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

	err = ext2fs_read_inode_ډک(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_اندازه(handle->fs->super));
	إذا (err)
		اذهب بهر;

	err = ext2fs_xattrs_read_inode(handle, inode);

بهر:
	ext2fs_free_mem(&inode);

	أرجع err;
}

errcode_t ext2fs_xattrs_iterate(بنية ext2_xattr_handle *h,
				صحيح (*func)(حرف *name, حرف *ارزښت,
					    size_t ارزښت_len, فراغ *معلومات),
				فراغ *معلومات)
{
	بنية ext2_xattr *x;
	صحيح dirty = 0;
	صحيح ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = h->attrs; x < h->attrs + h->شمېر; x++) {
		ret = func(x->name, x->ارزښت, x->ارزښت_len, معلومات);
		إذا (ret & XATTR_بدل_شوی)
			dirty = 1;
		إذا (ret & XATTR_ABORT)
			اكسر;
	}

	إذا (dirty)
		أرجع ext2fs_xattrs_write(h);
	أرجع 0;
}

errcode_t ext2fs_xattr_get(بنية ext2_xattr_handle *h, ثابت حرف *کلی,
			   فراغ **ارزښت, size_t *ارزښت_len)
{
	بنية ext2_xattr *x;
	حرف *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = h->attrs; x < h->attrs + h->شمېر; x++) {
		إذا (strcmp(x->name, کلی))
			استمر;

		إذا (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(کلی, "system.posix_acl_default") == 0) ||
		     (strcmp(کلی, "system.posix_acl_access") == 0))) {
			err = convert_disk_بفر_منزل_posix_acl(x->ارزښت, x->ارزښت_len,
							       ارزښت, ارزښت_len);
			أرجع err;
		} وإلا {
			err = ext2fs_get_mem(x->ارزښت_len, &val);
			إذا (err)
				أرجع err;
			حافظه_نقل(val, x->ارزښت, x->ارزښت_len);
			*ارزښت = val;
			*ارزښت_len = x->ارزښت_len;
			أرجع 0;
		}
	}

	أرجع EXT2_ET_EA_کلی_NOT_وموندل;
}

errcode_t ext2fs_xattr_inode_max_اندازه(ext2_filsys fs, ext2_ino_t ino,
				      size_t *اندازه)
{
	بنية ext2_ext_attr_ثبت *ثبت;
	بنية ext2_inode_large *inode;
	__u32 ea_inode_magic;
	بینښه صحيح minoff;
	حرف *پیل;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_اندازه(fs->super);
	إذا (i < حجم(*inode))
		i = حجم(*inode);
	err = ext2fs_get_memzero(i, &inode);
	إذا (err)
		أرجع err;

	err = ext2fs_read_inode_ډک(fs, ino, (بنية ext2_inode *)inode,
				     EXT2_INODE_اندازه(fs->super));
	إذا (err)
		اذهب بهر;

	/* Does the inode have size for EA? */
	إذا (EXT2_INODE_اندازه(fs->super) <= EXT2_GOOD_OLD_INODE_اندازه +
						  inode->i_extra_isize +
						  حجم(__u32)) {
		err = EXT2_ET_INLINE_معلومات_NO_SPACE;
		اذهب بهر;
	}

	minoff = EXT2_INODE_اندازه(fs->super) - حجم(*inode) - حجم(__u32);
	حافظه_نقل(&ea_inode_magic, ((حرف *) inode) + EXT2_GOOD_OLD_INODE_اندازه +
	       inode->i_extra_isize, حجم(__u32));
	إذا (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		پیل= ((حرف *) inode) + EXT2_GOOD_OLD_INODE_اندازه +
			inode->i_extra_isize + حجم(__u32);
		ثبت = (بنية ext2_ext_attr_ثبت *) پیل;
		طالما (!EXT2_EXT_IS_وروستی_ثبت(ثبت)) {
			إذا (!ثبت->e_ارزښت_inum && ثبت->e_ارزښت_اندازه) {
				بینښه صحيح offs = ثبت->e_ارزښت_offs;
				إذا (offs < minoff)
					minoff = offs;
			}
			ثبت = EXT2_EXT_ATTR_بل(ثبت);
		}
		*اندازه = minoff - ((حرف *)ثبت - (حرف *)پیل) - حجم(__u32);
	} وإلا {
		/* no xattr.  return a maximum size */
		*اندازه = EXT2_EXT_ATTR_اندازه(minoff -
					   EXT2_EXT_ATTR_LEN(مزي_اوږدوالی("data")) -
					   EXT2_EXT_ATTR_پړاو - حجم(__u32));
	}

بهر:
	ext2fs_free_mem(&inode);
	أرجع err;
}

ساكن errcode_t xattr_create_ea_inode(ext2_filsys fs, ثابت فراغ *ارزښت,
				       size_t ارزښت_len, ext2_ino_t *ea_ino)
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
	inode.i_ډېرپېښ = LINUX_S_IFREG | 0600;
	inode.i_links_شمېر = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	إذا (ret)
		أرجع ret;
	/*
	 * ref_count and hash utilize inode's i_*time fields.
	 * ext2fs_write_new_inode() call above initializes these fields with
	 * current time. That's why ref count and hash updates are done
	 * separately below.
	 */
	ext2fs_سېټ_ea_inode_ref(&inode, 1);
	hash = ext2fs_crc32c_le(fs->csum_تخم, ارزښت, ارزښت_len);
	ext2fs_سېټ_ea_inode_hash(&inode, hash);

	ret = ext2fs_write_inode(fs, ino, &inode);
	إذا (ret)
		أرجع ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	إذا (ret)
		أرجع ret;
	ret = ext2fs_file_write(file, ارزښت, ارزښت_len, عدم);
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
	__u64 ref_شمېر;
	errcode_t ret;

	ret = ext2fs_read_inode_ډک(fs, ino, (بنية ext2_inode *)&inode,
				     حجم(inode));
	إذا (ret)
		اذهب بهر;

	ref_شمېر = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_شمېر--;
	ext2fs_سېټ_ea_inode_ref(EXT2_INODE(&inode), ref_شمېر);

	إذا (ref_شمېر)
		اذهب write_بهر;

	inode.i_links_شمېر = 0;
	ext2fs_سېټ_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	إذا (ret)
		اذهب write_بهر;

	إذا (ext2fs_inode_has_valid_blocks2(fs, (بنية ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (بنية ext2_inode *)&inode, عدم,
				   0, ~0ULL);
		إذا (ret)
			اذهب بهر;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_بهر:
	ret = ext2fs_write_inode_ډک(fs, ino, (بنية ext2_inode *)&inode,
				      حجم(inode));
بهر:
	أرجع ret;
}

ساكن errcode_t xattr_update_ثبت(ext2_filsys fs, بنية ext2_xattr *x,
				    ثابت حرف *name, ثابت حرف *short_name,
				    صحيح شاخص, ثابت فراغ *ارزښت,
				    size_t ارزښت_len, صحيح دننه_inode)
{
	ext2_ino_t ea_ino = 0;
	فراغ *new_ارزښت = عدم;
	حرف *new_name = عدم;
	صحيح name_len;
	errcode_t ret;

	إذا (!x->name) {
		name_len = مزي_اوږدوالی(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		إذا (ret)
			اذهب fail;
		حافظه_نقل(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(ارزښت_len, &new_ارزښت);
	إذا (ret)
		اذهب fail;
	حافظه_نقل(new_ارزښت, ارزښت, ارزښت_len);

	إذا (دننه_inode) {
		ret = xattr_create_ea_inode(fs, ارزښت, ارزښت_len, &ea_ino);
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

	إذا (x->ارزښت)
		ext2fs_free_mem(&x->ارزښت);
	x->ارزښت = new_ارزښت;
	x->ارزښت_len = ارزښت_len;
	x->ea_ino = ea_ino;
	أرجع 0;
fail:
	إذا (new_name)
		ext2fs_free_mem(&new_name);
	إذا (new_ارزښت)
		ext2fs_free_mem(&new_ارزښت);
	إذا (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	أرجع ret;
}

ساكن صحيح xattr_پیدا_کړه_position(بنية ext2_xattr *attrs, صحيح شمېر,
			       ثابت حرف *shortname, صحيح name_idx)
{
	بنية ext2_xattr *x;
	صحيح i;
	صحيح shortname_len, x_shortname_len;

	shortname_len = مزي_اوږدوالی(shortname);

	لكل (i = 0, x = attrs; i < شمېر; i++, x++) {
		إذا (name_idx < x->name_شاخص)
			اكسر;
		إذا (name_idx > x->name_شاخص)
			استمر;

		x_shortname_len = مزي_اوږدوالی(x->short_name);
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
				    ثابت فراغ *ارزښت, size_t ارزښت_len,
				    صحيح ibody_free, صحيح بلاک_free,
				    صحيح old_idx, صحيح دننه_inode)
{
	بنية ext2_xattr tmp;
	صحيح جمع_کړه_منزل_ibody;
	صحيح needed;
	صحيح name_len, name_idx = 0;
	ثابت حرف *shortname = name;
	صحيح new_idx;
	صحيح ret;

	پیدا_کړه_ea_شاخص(name, &shortname, &name_idx);
	name_len = مزي_اوږدوالی(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	إذا (!دننه_inode)
		needed += EXT2_EXT_ATTR_اندازه(ارزښت_len);

	إذا (old_idx >= 0 && old_idx < h->ibody_شمېر) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		إذا (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_اندازه(
						h->attrs[old_idx].ارزښت_len);
	}

	إذا (needed <= ibody_free) {
		إذا (old_idx < 0) {
			new_idx = h->ibody_شمېر;
			جمع_کړه_منزل_ibody = 1;
			اذهب جمع_کړه_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_ثبت(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, ارزښت,
					 ارزښت_len, دننه_inode);
		إذا (ret)
			أرجع ret;
		إذا (h->ibody_شمېر <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_شمېر + 1,
				h->attrs + h->ibody_شمېر,
				(old_idx - h->ibody_شمېر) * حجم(*h->attrs));
			h->attrs[h->ibody_شمېر] = tmp;
			h->ibody_شمېر++;
		}
		أرجع 0;
	}

	إذا (h->ibody_شمېر <= old_idx) {
		بلاک_free += EXT2_EXT_ATTR_LEN(name_len);
		إذا (!h->attrs[old_idx].ea_ino)
			بلاک_free +=
				EXT2_EXT_ATTR_اندازه(h->attrs[old_idx].ارزښت_len);
	}

	إذا (needed > بلاک_free)
		أرجع EXT2_ET_EA_NO_SPACE;

	إذا (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_ثبت(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, ارزښت,
					 ارزښت_len, دننه_inode);
		إذا (ret)
			أرجع ret;
		إذا (old_idx < h->ibody_شمېر) {
			/*
			 * Move entry from ibody to the block. Note that
			 * entries in the block are sorted.
			 */
			new_idx = xattr_پیدا_کړه_position(h->attrs + h->ibody_شمېر,
						      h->شمېر - h->ibody_شمېر,
						      shortname, name_idx);
			new_idx += h->ibody_شمېر - 1;
			tmp = h->attrs[old_idx];
			memmove(h->attrs + old_idx, h->attrs + old_idx + 1,
				(new_idx - old_idx) * حجم(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_شمېر--;
		}
		أرجع 0;
	}

	new_idx = xattr_پیدا_کړه_position(h->attrs + h->ibody_شمېر,
				      h->شمېر - h->ibody_شمېر,
				      shortname, name_idx);
	new_idx += h->ibody_شمېر;
	جمع_کړه_منزل_ibody = 0;

جمع_کړه_new:
	إذا (h->شمېر == h->ظرفیت) {
		ret = ext2fs_xattrs_expand(h, 4);
		إذا (ret)
			أرجع ret;
	}

	ret = xattr_update_ثبت(h->fs, &h->attrs[h->شمېر], name, shortname,
				 name_idx, ارزښت, ارزښت_len, دننه_inode);
	إذا (ret)
		أرجع ret;

	tmp = h->attrs[h->شمېر];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->شمېر - new_idx)*حجم(*h->attrs));
	h->attrs[new_idx] = tmp;
	إذا (جمع_کړه_منزل_ibody)
		h->ibody_شمېر++;
	h->شمېر++;
	أرجع 0;
}

ساكن صحيح space_کارول_شوی(بنية ext2_xattr *attrs, صحيح شمېر)
{
	صحيح ټول = 0;
	بنية ext2_xattr *x;
	صحيح i, len;

	لكل (i = 0, x = attrs; i < شمېر; i++, x++) {
		len = مزي_اوږدوالی(x->short_name);
		ټول += EXT2_EXT_ATTR_LEN(len);
		إذا (!x->ea_ino)
			ټول += EXT2_EXT_ATTR_اندازه(x->ارزښت_len);
	}
	أرجع ټول;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#تعریف_کړه EXT4_XATTR_MIN_LARGE_EA_اندازه(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - حجم(بنية ext2_ext_attr_سرلیک) - 4)

errcode_t ext2fs_xattr_سېټ(بنية ext2_xattr_handle *h,
			   ثابت حرف *name,
			   ثابت فراغ *ارزښت,
			   size_t ارزښت_len)
{
	ext2_filsys fs = h->fs;
	ثابت صحيح inode_اندازه = EXT2_INODE_اندازه(fs->super);
	بنية ext2_inode_large *inode = عدم;
	بنية ext2_xattr *x;
	حرف *new_ارزښت;
	صحيح ibody_free, بلاک_free;
	صحيح دننه_inode = 0;
	صحيح old_idx = -1;
	صحيح extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(ارزښت_len, &new_ارزښت);
	إذا (ret)
		أرجع ret;
	إذا (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((strcmp(name, "system.posix_acl_default") == 0) ||
	     (strcmp(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_منزل_disk_بفر(ارزښت, ارزښت_len,
						       new_ارزښت, &ارزښت_len);
		إذا (ret)
			اذهب بهر;
	} وإلا إذا (ارزښت_len)
		حافظه_نقل(new_ارزښت, ارزښت, ارزښت_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	لكل (x = h->attrs; x < h->attrs + h->شمېر; x++) {
		إذا (!strcmp(x->name, name)) {
			إذا (!x->ea_ino && x->ارزښت_len == ارزښت_len &&
			    (!ارزښت_len ||
			     !memcmp(x->ارزښت, new_ارزښت, ارزښت_len))) {
				ret = 0;
				اذهب بهر;
			}
			old_idx = x - h->attrs;
			اكسر;
		}
	}

	ret = ext2fs_get_memzero(inode_اندازه, &inode);
	إذا (ret)
		اذهب بهر;
	ret = ext2fs_read_inode_ډک(fs, h->ino,
				     (بنية ext2_inode *)inode,
				     inode_اندازه);
	إذا (ret)
		اذهب بهر;
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
		ibody_free -= space_کارول_شوی(h->attrs, h->ibody_شمېر);
	} وإلا
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	إذا (strcmp(name, "system.data") == 0) {
		إذا (h->ibody_شمېر <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			اذهب بهر;
		}
		ret = xattr_array_update(h, name, new_ارزښت, ارزښت_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		إذا (ret)
			اذهب بهر;
		اذهب write_بهر;
	}

	بلاک_free = fs->blocksize;
	بلاک_free -= حجم(بنية ext2_ext_attr_سرلیک);
	/* Final null entry. */
	بلاک_free -= حجم(__u32);
	بلاک_free -= space_کارول_شوی(h->attrs + h->ibody_شمېر,
				 h->شمېر - h->ibody_شمېر);

	إذا (ext2fs_has_feature_ea_inode(fs->super) &&
	    ارزښت_len > EXT4_XATTR_MIN_LARGE_EA_اندازه(fs->blocksize))
		دننه_inode = 1;

	ret = xattr_array_update(h, name, new_ارزښت, ارزښت_len, ibody_free,
				 بلاک_free, old_idx, دننه_inode);
	إذا (ret == EXT2_ET_EA_NO_SPACE && !دننه_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_ارزښت, ارزښت_len,
			ibody_free, بلاک_free, old_idx, 1 /* in_inode */);
	إذا (ret)
		اذهب بهر;

write_بهر:
	ret = ext2fs_xattrs_write(h);
بهر:
	إذا (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_ارزښت);
	أرجع ret;
}

errcode_t ext2fs_xattr_remove(بنية ext2_xattr_handle *handle,
			      ثابت حرف *کلی)
{
	بنية ext2_xattr *x;
	بنية ext2_xattr *پای_ټکی = handle->attrs + handle->شمېر;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = handle->attrs; x < پای_ټکی; x++) {
		إذا (strcmp(x->name, کلی) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->ارزښت);
			إذا (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (پای_ټکی - x - 1)*حجم(*x));
			memset(پای_ټکی - 1, 0, حجم(*پای_ټکی));
			إذا (x < handle->attrs + handle->ibody_شمېر)
				handle->ibody_شمېر--;
			handle->شمېر--;
			أرجع ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	أرجع 0;
}

errcode_t ext2fs_xattr_remove_all(بنية ext2_xattr_handle *handle)
{
	بنية ext2_xattr *x;
	بنية ext2_xattr *پای_ټکی = handle->attrs + handle->شمېر;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = handle->attrs; x < پای_ټکی; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->ارزښت);
		إذا (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_شمېر = 0;
	handle->شمېر = 0;
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
	h->ظرفیت = 4;
	err = ext2fs_get_arrayzero(h->ظرفیت, حجم(بنية ext2_xattr),
				   &h->attrs);
	إذا (err) {
		ext2fs_free_mem(&h);
		أرجع err;
	}
	h->شمېر = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	أرجع 0;
}

errcode_t ext2fs_xattrs_close(بنية ext2_xattr_handle **handle)
{
	بنية ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_کلیګانې(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	أرجع 0;
}

errcode_t ext2fs_xattrs_شمېر(بنية ext2_xattr_handle *handle, size_t *شمېر)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*شمېر = handle->شمېر;
	أرجع 0;
}

errcode_t ext2fs_xattrs_flags(بنية ext2_xattr_handle *handle,
			      بینښه صحيح *new_flags, بینښه صحيح *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	إذا (old_flags)
		*old_flags = handle->flags;
	إذا (new_flags)
		handle->flags = *new_flags;
	أرجع 0;
}
