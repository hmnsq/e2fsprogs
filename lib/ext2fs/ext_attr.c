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
#جے_انت
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

#تعریف_کرو NAME_HASH_کھسکاؤ 5
#تعریف_کرو قدر_HASH_کھسکاؤ 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_اندراج(بنية ext2_ext_attr_اندراج *اندراج, فراغ *انکڑے)
{
	__u32 hash = 0;
	بنانشانی حرف *name = (((بنانشانی حرف *) اندراج) +
			       حجم(بنية ext2_ext_attr_اندراج));
	صحيح n;

	لكل (n = 0; n < اندراج->e_name_len; n++) {
		hash = (hash << NAME_HASH_کھسکاؤ) ^
		       (hash >> (8*حجم(hash) - NAME_HASH_کھسکاؤ)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	إذا (اندراج->e_قدر_inum == 0 && اندراج->e_قدر_ناپ != 0) {
		__u32 *قدر = (__u32 *)انکڑے;
		لكل (n = (اندراج->e_قدر_ناپ + EXT2_EXT_ATTR_گیڑا) >>
			 EXT2_EXT_ATTR_PAD_بٹ; n; n--) {
			hash = (hash << قدر_HASH_کھسکاؤ) ^
			       (hash >> (8*حجم(hash) - قدر_HASH_کھسکاؤ)) ^
			       ext2fs_le32_منزل_cpu(*قدر++);
		}
	}

	أرجع hash;
}

__u32 ext2fs_ext_attr_hash_اندراج_signed(بنية ext2_ext_attr_اندراج *اندراج,
					فراغ *انکڑے)
{
	__u32 hash = 0;
	نشانیوالا حرف *name = (((نشانیوالا حرف *) اندراج) +
			     حجم(بنية ext2_ext_attr_اندراج));
	صحيح n;

	لكل (n = 0; n < اندراج->e_name_len; n++) {
		hash = (hash << NAME_HASH_کھسکاؤ) ^
		       (hash >> (8*حجم(hash) - NAME_HASH_کھسکاؤ)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	إذا (اندراج->e_قدر_inum == 0 && اندراج->e_قدر_ناپ != 0) {
		__u32 *قدر = (__u32 *)انکڑے;
		لكل (n = (اندراج->e_قدر_ناپ + EXT2_EXT_ATTR_گیڑا) >>
			 EXT2_EXT_ATTR_PAD_بٹ; n; n--) {
			hash = (hash << قدر_HASH_کھسکاؤ) ^
			       (hash >> (8*حجم(hash) - قدر_HASH_کھسکاؤ)) ^
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
				      بنية ext2_ext_attr_اندراج *اندراج,
				      فراغ *انکڑے, __u32 *hash,
				      __u32 *signed_hash)
{
	*hash = ext2fs_ext_attr_hash_اندراج(اندراج, انکڑے);
	إذا (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_اندراج_signed(اندراج, انکڑے);

	إذا (اندراج->e_قدر_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, اندراج->e_قدر_inum,
					    &ea_inode_hash);
		إذا (retval)
			أرجع retval;

		*hash = (*hash << قدر_HASH_کھسکاؤ) ^
			(*hash >> (8*حجم(*hash) - قدر_HASH_کھسکاؤ)) ^
			ea_inode_hash;
		إذا (signed_hash)
			*signed_hash = (*signed_hash << قدر_HASH_کھسکاؤ) ^
				(*signed_hash >> (8*حجم(*hash) -
						  قدر_HASH_کھسکاؤ)) ^
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
				      بنية ext2_ext_attr_اندراج *اندراج,
				      فراغ *انکڑے, __u32 *hash)
{
	أرجع ext2fs_ext_attr_hash_entry3(fs, اندراج, انکڑے, hash, عدم);
}

#تعریف_رد NAME_HASH_کھسکاؤ
#تعریف_رد قدر_HASH_کھسکاؤ

#تعریف_کرو بلاک_HASH_کھسکاؤ 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
فراغ ext2fs_ext_attr_بلاک_rehash(بنية ext2_ext_attr_سرخی *سرخی,
				  بنية ext2_ext_attr_اندراج *سرا)
{
	بنية ext2_ext_attr_اندراج *ایتھے;
	__u32 hash = 0;

	ایتھے = (بنية ext2_ext_attr_اندراج *)(سرخی+1);
	طالما (ایتھے < سرا && !EXT2_EXT_IS_آخری_اندراج(ایتھے)) {
		إذا (!ایتھے->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			اكسر;
		}
		hash = (hash << بلاک_HASH_کھسکاؤ) ^
		       (hash >> (8*حجم(hash) - بلاک_HASH_کھسکاؤ)) ^
		       ایتھے->e_hash;
		ایتھے = EXT2_EXT_ATTR_اگلا(ایتھے);
	}
	سرخی->h_hash = hash;
}

#تعریف_رد بلاک_HASH_کھسکاؤ

__u32 ext2fs_get_ea_inode_hash(بنية ext2_inode *inode)
{
	أرجع inode->i_atime;
}

فراغ ext2fs_سیٹ_ea_inode_hash(بنية ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(بنية ext2_inode *inode)
{
	أرجع ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_نسخہ;
}

فراغ ext2fs_سیٹ_ea_inode_ref(بنية ext2_inode *inode, __u64 ref_گنتی)
{
	inode->i_ctime = (__u32)(ref_گنتی >> 32);
	inode->osd1.linux1.l_i_نسخہ = (__u32)ref_گنتی;
}

ساكن errcode_t check_ext_attr_سرخی(بنية ext2_ext_attr_سرخی *سرخی)
{
	إذا ((سرخی->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     سرخی->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    سرخی->h_بلاکاں != 1)
		أرجع EXT2_ET_BAD_EA_سرخی;

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

#جے_تعریف_شدہ الفاظ_BIGENDIAN
	ext2fs_وٹاندرا_ext_attr(buf, buf, fs->blocksize, 1);
#جے_انت

	retval = check_ext_attr_سرخی(buf);
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

#جے_تعریف_شدہ الفاظ_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	إذا (retval)
		أرجع retval;
	ext2fs_وٹاندرا_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#وإلا
	write_buf = (حرف *) inbuf;
#جے_انت

	retval = ext2fs_ext_attr_بلاک_csum_سیٹ(fs, inum, بلاک,
			(بنية ext2_ext_attr_سرخی *)write_buf);
	إذا (retval)
		أرجع retval;

	retval = io_channel_write_blk64(fs->io, بلاک, 1, write_buf);
#جے_تعریف_شدہ الفاظ_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#جے_انت
	إذا (!retval)
		ext2fs_mark_بدلیا(fs);
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
	بنية ext2_ext_attr_سرخی *سرخی;
	حرف	*buf = 0;

	إذا ((blk >= ext2fs_بلاکاں_گنتی(fs->super)) ||
	    (blk < fs->super->s_پہلا_انکڑے_بلاک))
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

	سرخی = (بنية ext2_ext_attr_سرخی *) بلاک_buf;
	سرخی->h_refcount += adjust;
	إذا (newcount)
		*newcount = سرخی->h_refcount;

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
	صحيح name_اشاریہ;
	حرف *name;
	حرف *short_name;
	فراغ *قدر;
	بنانشانی صحيح قدر_len;
	ext2_ino_t ea_ino;
};

بنية ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	بنية ext2_xattr *attrs;
	صحيح سمرتھا;
	صحيح گنتی;
	صحيح ibody_گنتی;
	ext2_ino_t ino;
	بنانشانی صحيح flags;
};

ساكن errcode_t ext2fs_xattrs_expand(بنية ext2_xattr_handle *h,
				      بنانشانی صحيح expandby)
{
	بنية ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->سمرتھا + expandby,
				   حجم(بنية ext2_xattr), &new_attrs);
	إذا (err)
		أرجع err;

	حافظہ_نقل(new_attrs, h->attrs, h->سمرتھا * حجم(بنية ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->سمرتھا += expandby;
	h->attrs = new_attrs;

	أرجع 0;
}

بنية ea_name_اشاریہ {
	صحيح اشاریہ;
	ثابت حرف *name;
};

/* Keep these names sorted in order of decreasing specificity. */
ساكن بنية ea_name_اشاریہ ea_ناں[] = {
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

ساكن ثابت حرف *لبھو_ea_اگیتر(صحيح اشاریہ)
{
	بنية ea_name_اشاریہ *e;

	لكل (e = ea_ناں; e->name; e++)
		إذا (e->اشاریہ == اشاریہ)
			أرجع e->name;

	أرجع عدم;
}

ساكن صحيح لبھو_ea_اشاریہ(ثابت حرف *fullname, ثابت حرف **name, صحيح *اشاریہ)
{
	بنية ea_name_اشاریہ *e;

	لكل (e = ea_ناں; e->name; e++) {
		إذا (لڑی_مقابلہ(fullname, e->name, لڑی_لمبائی(e->name)) == 0) {
			*name = fullname + لڑی_لمبائی(e->name);
			*اشاریہ = e->اشاریہ;
			أرجع 1;
		}
	}
	أرجع 0;
}

errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       بنية ext2_inode_large *inode)
{
	بنية ext2_ext_attr_سرخی *سرخی;
	فراغ *بلاک_buf = عدم;
	blk64_t blk;
	errcode_t err;
	بنية ext2_inode_large i;

	/* Read inode? */
	إذا (inode == عدم) {
		err = ext2fs_read_inode_بھریا(fs, ino, (بنية ext2_inode *)&i,
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
	إذا ((blk < fs->super->s_پہلا_انکڑے_بلاک) ||
	    (blk >= ext2fs_بلاکاں_گنتی(fs->super))) {
		err = EXT2_ET_BAD_EA_بلاک_عدد;
		اذهب باہرلا;
	}

	err = ext2fs_get_mem(fs->blocksize, &بلاک_buf);
	إذا (err)
		اذهب باہرلا;

	err = ext2fs_read_ext_attr3(fs, blk, بلاک_buf, ino);
	إذا (err)
		اذهب out2;

	/* We only know how to deal with v2 EA blocks */
	سرخی = (بنية ext2_ext_attr_سرخی *) بلاک_buf;
	إذا (سرخی->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_سرخی;
		اذهب out2;
	}

	سرخی->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, بلاک_buf, ino);
	إذا (err)
		اذهب out2;

	/* Erase link to block */
	ext2fs_file_acl_بلاک_سیٹ(fs, (بنية ext2_inode *)inode, 0);
	إذا (سرخی->h_refcount == 0)
		ext2fs_بلاک_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_بلاکاں(fs, (بنية ext2_inode *)inode, 1);
	إذا (err)
		اذهب out2;

	/* Write inode? */
	إذا (inode == &i) {
		err = ext2fs_write_inode_بھریا(fs, ino, (بنية ext2_inode *)&i,
					      حجم(بنية ext2_inode_large));
		إذا (err)
			اذهب out2;
	}

out2:
	ext2fs_free_mem(&بلاک_buf);
باہرلا:
	أرجع err;
}

ساكن errcode_t prep_ea_بلاک_for_write(ext2_filsys fs, ext2_ino_t ino,
					 بنية ext2_inode_large *inode)
{
	بنية ext2_ext_attr_سرخی *سرخی;
	فراغ *بلاک_buf = عدم;
	blk64_t blk, goal;
	errcode_t err;

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_بلاک(fs, (بنية ext2_inode *)inode);
	إذا (blk != 0) {
		إذا ((blk < fs->super->s_پہلا_انکڑے_بلاک) ||
		    (blk >= ext2fs_بلاکاں_گنتی(fs->super))) {
			err = EXT2_ET_BAD_EA_بلاک_عدد;
			اذهب باہرلا;
		}

		err = ext2fs_get_mem(fs->blocksize, &بلاک_buf);
		إذا (err)
			اذهب باہرلا;

		err = ext2fs_read_ext_attr3(fs, blk, بلاک_buf, ino);
		إذا (err)
			اذهب out2;

		/* We only know how to deal with v2 EA blocks */
		سرخی = (بنية ext2_ext_attr_سرخی *) بلاک_buf;
		إذا (سرخی->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_سرخی;
			اذهب out2;
		}

		/* Single-user block.  We're done here. */
		إذا (سرخی->h_refcount == 1)
			اذهب out2;

		/* We need to CoW the block. */
		سرخی->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, بلاک_buf, ino);
		إذا (err)
			اذهب out2;
	} وإلا {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_جوڑو_بلاکاں(fs, (بنية ext2_inode *)inode,
					     1);
		إذا (err)
			اذهب باہرلا;
	}

	/* Allocate a block */
	goal = ext2fs_لبھو_inode_goal(fs, ino, (بنية ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, عدم, &blk);
	إذا (err)
		اذهب out2;
	ext2fs_file_acl_بلاک_سیٹ(fs, (بنية ext2_inode *)inode, blk);
out2:
	إذا (بلاک_buf)
		ext2fs_free_mem(&بلاک_buf);
باہرلا:
	أرجع err;
}


ساكن مضمن صحيح
posix_acl_xattr_گنتی(size_t ناپ)
{
        إذا (ناپ < حجم(posix_acl_xattr_سرخی))
                أرجع -1;
        ناپ -= حجم(posix_acl_xattr_سرخی);
        إذا (ناپ % حجم(posix_acl_xattr_اندراج))
                أرجع -1;
        أرجع ناپ / حجم(posix_acl_xattr_اندراج);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
ساكن errcode_t convert_posix_acl_منزل_disk_بفر(ثابت فراغ *قدر, size_t ناپ,
						  فراغ *باہرلا_buf, size_t *ناپ_باہرلا)
{
	ثابت posix_acl_xattr_سرخی *سرخی =
		(ثابت posix_acl_xattr_سرخی*) قدر;
	ثابت posix_acl_xattr_اندراج *سرا, *اندراج =
		(ثابت posix_acl_xattr_اندراج *)(سرخی+1);
	ext4_acl_سرخی *ext_acl;
	size_t s;
	حرف *e;

	صحيح گنتی;

	إذا (!قدر)
		أرجع EINVAL;
	إذا (ناپ < حجم(posix_acl_xattr_سرخی))
		أرجع ENOMEM;
	إذا (سرخی->a_نسخہ != ext2fs_cpu_منزل_le32(POSIX_ACL_XATTR_نسخہ))
		أرجع EINVAL;

	گنتی = posix_acl_xattr_گنتی(ناپ);
	ext_acl = باہرلا_buf;
	ext_acl->a_نسخہ = ext2fs_cpu_منزل_le32(EXT4_ACL_نسخہ);

	إذا (گنتی <= 0)
		أرجع EINVAL;

	e = (حرف *) باہرلا_buf + حجم(ext4_acl_سرخی);
	s = حجم(ext4_acl_سرخی);
	لكل (سرا = اندراج + گنتی; اندراج != سرا;اندراج++) {
		ext4_acl_اندراج *disk_اندراج = (ext4_acl_اندراج*) e;
		disk_اندراج->e_ٹیگ = اندراج->e_ٹیگ;
		disk_اندراج->e_perm = اندراج->e_perm;

		بدل(ext2fs_le16_منزل_cpu(اندراج->e_ٹیگ)) {
			حالة ACL_USER_OBJ:
			حالة ACL_گروہ_OBJ:
			حالة ACL_نقاب:
			حالة ACL_OTHER:
				e += حجم(ext4_acl_اندراج_short);
				s += حجم(ext4_acl_اندراج_short);
				اكسر;
			حالة ACL_USER:
			حالة ACL_گروہ:
				disk_اندراج->e_id = اندراج->e_id;
				e += حجم(ext4_acl_اندراج);
				s += حجم(ext4_acl_اندراج);
				اكسر;
			افتراضي:
				أرجع EINVAL;
		}
	}
	*ناپ_باہرلا = s;
	أرجع 0;
}

ساكن errcode_t convert_disk_بفر_منزل_posix_acl(ثابت فراغ *قدر, size_t ناپ,
						  فراغ **باہرلا_buf, size_t *ناپ_باہرلا)
{
	posix_acl_xattr_سرخی *سرخی;
	posix_acl_xattr_اندراج *اندراج;
	ثابت ext4_acl_سرخی *ext_acl = (ثابت ext4_acl_سرخی *) قدر;
	errcode_t err;
	ثابت حرف *cp;
	حرف *باہرلا;

	إذا ((!قدر) ||
	    (ناپ < حجم(ext4_acl_سرخی)) ||
	    (ext_acl->a_نسخہ != ext2fs_cpu_منزل_le32(EXT4_ACL_نسخہ)))
		أرجع EINVAL;

	err = ext2fs_get_mem(ناپ * 2, &باہرلا);
	إذا (err)
		أرجع err;

	سرخی = (posix_acl_xattr_سرخی *) باہرلا;
	سرخی->a_نسخہ = ext2fs_cpu_منزل_le32(POSIX_ACL_XATTR_نسخہ);
	اندراج = (posix_acl_xattr_اندراج *) (باہرلا + حجم(posix_acl_xattr_سرخی));

	cp = (ثابت حرف *) قدر + حجم(ext4_acl_سرخی);
	ناپ -= حجم(ext4_acl_سرخی);

	طالما (ناپ > 0) {
		ثابت ext4_acl_اندراج *disk_اندراج = (ثابت ext4_acl_اندراج *) cp;

		اندراج->e_ٹیگ = disk_اندراج->e_ٹیگ;
		اندراج->e_perm = disk_اندراج->e_perm;

		بدل(ext2fs_le16_منزل_cpu(اندراج->e_ٹیگ)) {
			حالة ACL_USER_OBJ:
			حالة ACL_گروہ_OBJ:
			حالة ACL_نقاب:
			حالة ACL_OTHER:
				اندراج->e_id = 0;
				cp += حجم(ext4_acl_اندراج_short);
				ناپ -= حجم(ext4_acl_اندراج_short);
				اكسر;
			حالة ACL_USER:
			حالة ACL_گروہ:
				اندراج->e_id = disk_اندراج->e_id;
				cp += حجم(ext4_acl_اندراج);
				ناپ -= حجم(ext4_acl_اندراج);
				اكسر;
			افتراضي:
				ext2fs_free_mem(&باہرلا);
				أرجع EINVAL;
		}
		اندراج++;
	}
	*باہرلا_buf = باہرلا;
	*ناپ_باہرلا = ((حرف *) اندراج - باہرلا);
	أرجع 0;
}

ساكن errcode_t
write_xattrs_منزل_بفر(ext2_filsys fs, بنية ext2_xattr *attrs, صحيح گنتی,
		       فراغ *entries_شروع, بنانشانی صحيح storage_ناپ,
		       بنانشانی صحيح قدر_وتھ_correction, صحيح write_hash)
{
	بنية ext2_xattr *x;
	بنية ext2_ext_attr_اندراج *e = entries_شروع;
	حرف *سرا = (حرف *) entries_شروع + storage_ناپ;
	بنانشانی صحيح قدر_ناپ;
	errcode_t err;

	memset(entries_شروع, 0, storage_ناپ);
	لكل (x = attrs; x < attrs + گنتی; x++) {
		قدر_ناپ = ((x->قدر_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = لڑی_لمبائی(x->short_name);
		e->e_name_اشاریہ = x->name_اشاریہ;

		e->e_قدر_ناپ = x->قدر_len;
		e->e_قدر_inum = x->ea_ino;

		/* Store name */
		حافظہ_نقل((حرف *)e + حجم(*e), x->short_name, e->e_name_len);
		إذا (x->ea_ino) {
			e->e_قدر_offs = 0;
		} وإلا {
			سرا -= قدر_ناپ;
			e->e_قدر_offs = سرا - (حرف *) entries_شروع +
						قدر_وتھ_correction;
			حافظہ_نقل(سرا, x->قدر, e->e_قدر_ناپ);
		}

		إذا (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : سرا,
							  &e->e_hash);
			إذا (err)
				أرجع err;
		} وإلا
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_اگلا(e);
		*(__u32 *)e = 0;
	}
	أرجع 0;
}

errcode_t ext2fs_xattrs_write(بنية ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	ثابت بنانشانی صحيح inode_ناپ = EXT2_INODE_ناپ(fs->super);
	بنية ext2_inode_large *inode;
	حرف *شروع, *بلاک_buf = عدم;
	بنية ext2_ext_attr_سرخی *سرخی;
	__u32 ea_inode_magic;
	blk64_t blk;
	بنانشانی صحيح storage_ناپ;
	بنانشانی صحيح i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_ناپ;
	إذا (i < حجم(*inode))
		i = حجم(*inode);
	err = ext2fs_get_memzero(i, &inode);
	إذا (err)
		أرجع err;

	err = ext2fs_read_inode_بھریا(fs, handle->ino, EXT2_INODE(inode),
				     inode_ناپ);
	إذا (err)
		اذهب باہرلا;

	/* If extra_isize isn't set, we need to set it now */
	إذا (inode->i_extra_isize == 0 &&
	    inode_ناپ > EXT2_GOOD_OLD_INODE_ناپ) {
		حرف *p = (حرف *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		إذا (extra == 0)
			extra = حجم(__u32);
		memset(p + EXT2_GOOD_OLD_INODE_ناپ, 0, extra);
		inode->i_extra_isize = extra;
	}
	إذا (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		اذهب باہرلا;
	}

	/* Does the inode have space for EA? */
	إذا (inode->i_extra_isize < حجم(inode->i_extra_isize) ||
	    inode_ناپ <= EXT2_GOOD_OLD_INODE_ناپ + inode->i_extra_isize +
								حجم(__u32))
		اذهب write_ea_بلاک;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	حافظہ_نقل(((حرف *) inode) + EXT2_GOOD_OLD_INODE_ناپ +
	       inode->i_extra_isize, &ea_inode_magic, حجم(__u32));
	storage_ناپ = inode_ناپ - EXT2_GOOD_OLD_INODE_ناپ -
				inode->i_extra_isize - حجم(__u32);
	شروع = ((حرف *) inode) + EXT2_GOOD_OLD_INODE_ناپ +
				inode->i_extra_isize + حجم(__u32);

	err = write_xattrs_منزل_بفر(fs, handle->attrs, handle->ibody_گنتی,
				     شروع, storage_ناپ, 0, 0);
	إذا (err)
		اذهب باہرلا;
write_ea_بلاک:
	/* Are we done? */
	إذا (handle->ibody_گنتی == handle->گنتی &&
	    !ext2fs_file_acl_بلاک(fs, EXT2_INODE(inode)))
		اذهب skip_ea_بلاک;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &بلاک_buf);
	إذا (err)
		اذهب باہرلا;

	storage_ناپ = fs->blocksize - حجم(بنية ext2_ext_attr_سرخی);
	شروع = بلاک_buf + حجم(بنية ext2_ext_attr_سرخی);

	err = write_xattrs_منزل_بفر(fs, handle->attrs + handle->ibody_گنتی,
				     handle->گنتی - handle->ibody_گنتی, شروع,
				     storage_ناپ, شروع - بلاک_buf, 1);
	إذا (err)
		اذهب out2;

	/* Write a header on the EA block */
	سرخی = (بنية ext2_ext_attr_سرخی *) بلاک_buf;
	سرخی->h_magic = EXT2_EXT_ATTR_MAGIC;
	سرخی->h_refcount = 1;
	سرخی->h_بلاکاں = 1;

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
			اذهب باہرلا;
	}

	/* Write the inode */
	err = ext2fs_write_inode_بھریا(fs, handle->ino, EXT2_INODE(inode),
				      inode_ناپ);
	إذا (err)
		اذهب out2;

out2:
	ext2fs_free_mem(&بلاک_buf);
باہرلا:
	ext2fs_free_mem(&inode);
	أرجع err;
}

ساكن errcode_t read_xattrs_from_بفر(بنية ext2_xattr_handle *handle,
					 بنية ext2_inode_large *inode,
					 بنية ext2_ext_attr_اندراج *entries,
					 بنانشانی صحيح storage_ناپ,
					 حرف *قدر_شروع)
{
	بنية ext2_xattr *x;
	بنية ext2_ext_attr_اندراج *اندراج, *سرا;
	ثابت حرف *اگیتر;
	بنانشانی صحيح remain, اگیتر_len;
	errcode_t err;
	بنانشانی صحيح اقدار_ناپ = storage_ناپ +
			((حرف *)entries - قدر_شروع);

	/* find the end */
	سرا = entries;
	remain = storage_ناپ;
	طالما (remain >= حجم(بنية ext2_ext_attr_اندراج) &&
	       !EXT2_EXT_IS_آخری_اندراج(سرا)) {

		/* header eats this space */
		remain -= حجم(بنية ext2_ext_attr_اندراج);

		/* is attribute name valid? */
		إذا (EXT2_EXT_ATTR_ناپ(سرا->e_name_len) > remain)
			أرجع EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_ناپ(سرا->e_name_len);
		سرا = EXT2_EXT_ATTR_اگلا(سرا);
	}

	اندراج = entries;
	remain = storage_ناپ;
	طالما (remain >= حجم(بنية ext2_ext_attr_اندراج) &&
	       !EXT2_EXT_IS_آخری_اندراج(اندراج)) {

		/* Allocate space for more attrs? */
		إذا (handle->گنتی == handle->سمرتھا) {
			err = ext2fs_xattrs_expand(handle, 4);
			إذا (err)
				أرجع err;
		}

		x = handle->attrs + handle->گنتی;

		/* header eats this space */
		remain -= حجم(بنية ext2_ext_attr_اندراج);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_ناپ(اندراج->e_name_len);

		/* Extract name */
		اگیتر = لبھو_ea_اگیتر(اندراج->e_name_اشاریہ);
		اگیتر_len = (اگیتر ? لڑی_لمبائی(اگیتر) : 0);
		err = ext2fs_get_memzero(اندراج->e_name_len + اگیتر_len + 1,
					 &x->name);
		إذا (err)
			أرجع err;
		إذا (اگیتر)
			حافظہ_نقل(x->name, اگیتر, اگیتر_len);
		إذا (اندراج->e_name_len)
			حافظہ_نقل(x->name + اگیتر_len,
			       (حرف *)اندراج + حجم(*اندراج),
			       اندراج->e_name_len);
		x->short_name = x->name + اگیتر_len;
		x->name_اشاریہ = اندراج->e_name_اشاریہ;

		/* Check & copy value */
		إذا (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    اندراج->e_قدر_inum != 0)
			أرجع EXT2_ET_BAD_EA_بلاک_عدد;

		إذا (اندراج->e_قدر_inum == 0) {
			إذا (اندراج->e_قدر_ناپ > remain)
				أرجع EXT2_ET_EA_BAD_قدر_ناپ;

			إذا (اندراج->e_قدر_offs + اندراج->e_قدر_ناپ > اقدار_ناپ)
				أرجع EXT2_ET_EA_BAD_قدر_وتھ;

			إذا (اندراج->e_قدر_ناپ > 0 &&
			    قدر_شروع + اندراج->e_قدر_offs <
			    (حرف *)سرا + حجم(__u32))
				أرجع EXT2_ET_EA_BAD_قدر_وتھ;

			remain -= اندراج->e_قدر_ناپ;

			err = ext2fs_get_mem(اندراج->e_قدر_ناپ, &x->قدر);
			إذا (err)
				أرجع err;
			حافظہ_نقل(x->قدر, قدر_شروع + اندراج->e_قدر_offs,
			       اندراج->e_قدر_ناپ);
		} وإلا {
			بنية ext2_inode *ea_inode;
			ext2_file_t ea_file;

			إذا (اندراج->e_قدر_offs != 0)
				أرجع EXT2_ET_EA_BAD_قدر_وتھ;

			إذا (اندراج->e_قدر_ناپ > (64 * 1024))
				أرجع EXT2_ET_EA_BAD_قدر_ناپ;

			err = ext2fs_get_mem(اندراج->e_قدر_ناپ, &x->قدر);
			إذا (err)
				أرجع err;

			err = ext2fs_file_open(handle->fs, اندراج->e_قدر_inum,
					       0, &ea_file);
			إذا (err)
				أرجع err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			إذا ((ea_inode->i_flags & EXT4_INLINE_انکڑے_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_گنتی == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			وإلا إذا ((__u64) ext2fs_file_get_ناپ(ea_file) !=
				 اندراج->e_قدر_ناپ)
				err = EXT2_ET_EA_BAD_قدر_ناپ;
			وإلا
				err = ext2fs_file_read(ea_file, x->قدر,
						       اندراج->e_قدر_ناپ, 0);
			ext2fs_file_close(ea_file);
			إذا (err)
				أرجع err;
		}

		x->ea_ino = اندراج->e_قدر_inum;
		x->قدر_len = اندراج->e_قدر_ناپ;

		/* e_hash may be 0 in older inode's ea */
		إذا (اندراج->e_hash != 0) {
			__u32 hash, signed_hash;

			فراغ *انکڑے = (اندراج->e_قدر_inum != 0) ?
					0 : قدر_شروع + اندراج->e_قدر_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, اندراج,
							  انکڑے, &hash,
							  &signed_hash);
			إذا (err)
				أرجع err;
			إذا ((اندراج->e_hash != hash) &&
			    (اندراج->e_hash != signed_hash)) {
				بنية ext2_inode بچہ;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							اندراج->e_قدر_inum,
							&بچہ);
				إذا (err)
					أرجع err;
				إذا (بچہ.i_mtime != handle->ino ||
				    بچہ.i_generation != inode->i_generation)
					أرجع EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->گنتی++;
		اندراج = EXT2_EXT_ATTR_اگلا(اندراج);
	}

	أرجع 0;
}

ساكن فراغ xattrs_free_کلیدیں(بنية ext2_xattr_handle *h)
{
	بنية ext2_xattr *a = h->attrs;
	صحيح i;

	لكل (i = 0; i < h->سمرتھا; i++) {
		إذا (a[i].name)
			ext2fs_free_mem(&a[i].name);
		إذا (a[i].قدر)
			ext2fs_free_mem(&a[i].قدر);
	}
	h->گنتی = 0;
	h->ibody_گنتی = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(بنية ext2_xattr_handle *handle,
				   بنية ext2_inode_large *inode)
{
	بنية ext2_ext_attr_سرخی *سرخی;
	__u32 ea_inode_magic;
	بنانشانی صحيح storage_ناپ;
	حرف *شروع, *بلاک_buf = عدم;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_کلیدیں(handle);

	/* Does the inode have space for EA? */
	إذا (inode->i_extra_isize < حجم(inode->i_extra_isize) ||
	    EXT2_INODE_ناپ(handle->fs->super) <= EXT2_GOOD_OLD_INODE_ناپ +
						  inode->i_extra_isize +
						  حجم(__u32))
		اذهب read_ea_بلاک;
	إذا (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		اذهب باہرلا;
	}

	/* Look for EA in the inode */
	حافظہ_نقل(&ea_inode_magic, ((حرف *) inode) + EXT2_GOOD_OLD_INODE_ناپ +
	       inode->i_extra_isize, حجم(__u32));
	إذا (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_ناپ = EXT2_INODE_ناپ(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_ناپ - inode->i_extra_isize -
			حجم(__u32);
		شروع = ((حرف *) inode) + EXT2_GOOD_OLD_INODE_ناپ +
			inode->i_extra_isize + حجم(__u32);

		err = read_xattrs_from_بفر(handle, inode,
					(بنية ext2_ext_attr_اندراج *) شروع,
					storage_ناپ, شروع);
		إذا (err)
			اذهب باہرلا;

		handle->ibody_گنتی = handle->گنتی;
	}

read_ea_بلاک:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_بلاک(handle->fs, EXT2_INODE(inode));
	إذا (blk != 0) {
		إذا ((blk < handle->fs->super->s_پہلا_انکڑے_بلاک) ||
		    (blk >= ext2fs_بلاکاں_گنتی(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_بلاک_عدد;
			اذهب باہرلا;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &بلاک_buf);
		إذا (err)
			اذهب باہرلا;

		err = ext2fs_read_ext_attr3(handle->fs, blk, بلاک_buf,
					    handle->ino);
		إذا (err)
			اذهب out3;

		/* We only know how to deal with v2 EA blocks */
		سرخی = (بنية ext2_ext_attr_سرخی *) بلاک_buf;
		إذا (سرخی->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_سرخی;
			اذهب out3;
		}

		/* Read EAs */
		storage_ناپ = handle->fs->blocksize -
			حجم(بنية ext2_ext_attr_سرخی);
		شروع = بلاک_buf + حجم(بنية ext2_ext_attr_سرخی);
		err = read_xattrs_from_بفر(handle, inode,
					(بنية ext2_ext_attr_اندراج *) شروع,
					storage_ناپ, بلاک_buf);
	}

out3:
	إذا (بلاک_buf)
		ext2fs_free_mem(&بلاک_buf);
باہرلا:
	أرجع err;
}

errcode_t ext2fs_xattrs_read(بنية ext2_xattr_handle *handle)
{
	بنية ext2_inode_large *inode;
	size_t inode_ناپ = EXT2_INODE_ناپ(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	إذا (inode_ناپ < حجم(*inode))
		inode_ناپ = حجم(*inode);
	err = ext2fs_get_memzero(inode_ناپ, &inode);
	إذا (err)
		أرجع err;

	err = ext2fs_read_inode_بھریا(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_ناپ(handle->fs->super));
	إذا (err)
		اذهب باہرلا;

	err = ext2fs_xattrs_read_inode(handle, inode);

باہرلا:
	ext2fs_free_mem(&inode);

	أرجع err;
}

errcode_t ext2fs_xattrs_iterate(بنية ext2_xattr_handle *h,
				صحيح (*func)(حرف *name, حرف *قدر,
					    size_t قدر_len, فراغ *انکڑے),
				فراغ *انکڑے)
{
	بنية ext2_xattr *x;
	صحيح dirty = 0;
	صحيح ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = h->attrs; x < h->attrs + h->گنتی; x++) {
		ret = func(x->name, x->قدر, x->قدر_len, انکڑے);
		إذا (ret & XATTR_بدلیا)
			dirty = 1;
		إذا (ret & XATTR_ABORT)
			اكسر;
	}

	إذا (dirty)
		أرجع ext2fs_xattrs_write(h);
	أرجع 0;
}

errcode_t ext2fs_xattr_get(بنية ext2_xattr_handle *h, ثابت حرف *کلید,
			   فراغ **قدر, size_t *قدر_len)
{
	بنية ext2_xattr *x;
	حرف *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = h->attrs; x < h->attrs + h->گنتی; x++) {
		إذا (strcmp(x->name, کلید))
			استمر;

		إذا (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(کلید, "system.posix_acl_default") == 0) ||
		     (strcmp(کلید, "system.posix_acl_access") == 0))) {
			err = convert_disk_بفر_منزل_posix_acl(x->قدر, x->قدر_len,
							       قدر, قدر_len);
			أرجع err;
		} وإلا {
			err = ext2fs_get_mem(x->قدر_len, &val);
			إذا (err)
				أرجع err;
			حافظہ_نقل(val, x->قدر, x->قدر_len);
			*قدر = val;
			*قدر_len = x->قدر_len;
			أرجع 0;
		}
	}

	أرجع EXT2_ET_EA_کلید_NOT_لبھیا;
}

errcode_t ext2fs_xattr_inode_max_ناپ(ext2_filsys fs, ext2_ino_t ino,
				      size_t *ناپ)
{
	بنية ext2_ext_attr_اندراج *اندراج;
	بنية ext2_inode_large *inode;
	__u32 ea_inode_magic;
	بنانشانی صحيح minoff;
	حرف *شروع;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_ناپ(fs->super);
	إذا (i < حجم(*inode))
		i = حجم(*inode);
	err = ext2fs_get_memzero(i, &inode);
	إذا (err)
		أرجع err;

	err = ext2fs_read_inode_بھریا(fs, ino, (بنية ext2_inode *)inode,
				     EXT2_INODE_ناپ(fs->super));
	إذا (err)
		اذهب باہرلا;

	/* Does the inode have size for EA? */
	إذا (EXT2_INODE_ناپ(fs->super) <= EXT2_GOOD_OLD_INODE_ناپ +
						  inode->i_extra_isize +
						  حجم(__u32)) {
		err = EXT2_ET_INLINE_انکڑے_NO_SPACE;
		اذهب باہرلا;
	}

	minoff = EXT2_INODE_ناپ(fs->super) - حجم(*inode) - حجم(__u32);
	حافظہ_نقل(&ea_inode_magic, ((حرف *) inode) + EXT2_GOOD_OLD_INODE_ناپ +
	       inode->i_extra_isize, حجم(__u32));
	إذا (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		شروع= ((حرف *) inode) + EXT2_GOOD_OLD_INODE_ناپ +
			inode->i_extra_isize + حجم(__u32);
		اندراج = (بنية ext2_ext_attr_اندراج *) شروع;
		طالما (!EXT2_EXT_IS_آخری_اندراج(اندراج)) {
			إذا (!اندراج->e_قدر_inum && اندراج->e_قدر_ناپ) {
				بنانشانی صحيح offs = اندراج->e_قدر_offs;
				إذا (offs < minoff)
					minoff = offs;
			}
			اندراج = EXT2_EXT_ATTR_اگلا(اندراج);
		}
		*ناپ = minoff - ((حرف *)اندراج - (حرف *)شروع) - حجم(__u32);
	} وإلا {
		/* no xattr.  return a maximum size */
		*ناپ = EXT2_EXT_ATTR_ناپ(minoff -
					   EXT2_EXT_ATTR_LEN(لڑی_لمبائی("data")) -
					   EXT2_EXT_ATTR_گیڑا - حجم(__u32));
	}

باہرلا:
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
	inode.i_ناپ = 0;
	inode.i_منوال = LINUX_S_IFREG | 0600;
	inode.i_links_گنتی = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	إذا (ret)
		أرجع ret;
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
	__u64 ref_گنتی;
	errcode_t ret;

	ret = ext2fs_read_inode_بھریا(fs, ino, (بنية ext2_inode *)&inode,
				     حجم(inode));
	إذا (ret)
		اذهب باہرلا;

	ref_گنتی = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_گنتی--;
	ext2fs_سیٹ_ea_inode_ref(EXT2_INODE(&inode), ref_گنتی);

	إذا (ref_گنتی)
		اذهب write_باہرلا;

	inode.i_links_گنتی = 0;
	ext2fs_سیٹ_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	إذا (ret)
		اذهب write_باہرلا;

	إذا (ext2fs_inode_has_valid_blocks2(fs, (بنية ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (بنية ext2_inode *)&inode, عدم,
				   0, ~0ULL);
		إذا (ret)
			اذهب باہرلا;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_باہرلا:
	ret = ext2fs_write_inode_بھریا(fs, ino, (بنية ext2_inode *)&inode,
				      حجم(inode));
باہرلا:
	أرجع ret;
}

ساكن errcode_t xattr_update_اندراج(ext2_filsys fs, بنية ext2_xattr *x,
				    ثابت حرف *name, ثابت حرف *short_name,
				    صحيح اشاریہ, ثابت فراغ *قدر,
				    size_t قدر_len, صحيح وچ_inode)
{
	ext2_ino_t ea_ino = 0;
	فراغ *new_قدر = عدم;
	حرف *new_name = عدم;
	صحيح name_len;
	errcode_t ret;

	إذا (!x->name) {
		name_len = لڑی_لمبائی(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		إذا (ret)
			اذهب fail;
		حافظہ_نقل(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(قدر_len, &new_قدر);
	إذا (ret)
		اذهب fail;
	حافظہ_نقل(new_قدر, قدر, قدر_len);

	إذا (وچ_inode) {
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
	x->name_اشاریہ = اشاریہ;

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

ساكن صحيح xattr_لبھو_position(بنية ext2_xattr *attrs, صحيح گنتی,
			       ثابت حرف *shortname, صحيح name_idx)
{
	بنية ext2_xattr *x;
	صحيح i;
	صحيح shortname_len, x_shortname_len;

	shortname_len = لڑی_لمبائی(shortname);

	لكل (i = 0, x = attrs; i < گنتی; i++, x++) {
		إذا (name_idx < x->name_اشاریہ)
			اكسر;
		إذا (name_idx > x->name_اشاریہ)
			استمر;

		x_shortname_len = لڑی_لمبائی(x->short_name);
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
				    صحيح ibody_free, صحيح بلاک_free,
				    صحيح old_idx, صحيح وچ_inode)
{
	بنية ext2_xattr tmp;
	صحيح جوڑو_منزل_ibody;
	صحيح needed;
	صحيح name_len, name_idx = 0;
	ثابت حرف *shortname = name;
	صحيح new_idx;
	صحيح ret;

	لبھو_ea_اشاریہ(name, &shortname, &name_idx);
	name_len = لڑی_لمبائی(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	إذا (!وچ_inode)
		needed += EXT2_EXT_ATTR_ناپ(قدر_len);

	إذا (old_idx >= 0 && old_idx < h->ibody_گنتی) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		إذا (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_ناپ(
						h->attrs[old_idx].قدر_len);
	}

	إذا (needed <= ibody_free) {
		إذا (old_idx < 0) {
			new_idx = h->ibody_گنتی;
			جوڑو_منزل_ibody = 1;
			اذهب جوڑو_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_اندراج(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, قدر,
					 قدر_len, وچ_inode);
		إذا (ret)
			أرجع ret;
		إذا (h->ibody_گنتی <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_گنتی + 1,
				h->attrs + h->ibody_گنتی,
				(old_idx - h->ibody_گنتی) * حجم(*h->attrs));
			h->attrs[h->ibody_گنتی] = tmp;
			h->ibody_گنتی++;
		}
		أرجع 0;
	}

	إذا (h->ibody_گنتی <= old_idx) {
		بلاک_free += EXT2_EXT_ATTR_LEN(name_len);
		إذا (!h->attrs[old_idx].ea_ino)
			بلاک_free +=
				EXT2_EXT_ATTR_ناپ(h->attrs[old_idx].قدر_len);
	}

	إذا (needed > بلاک_free)
		أرجع EXT2_ET_EA_NO_SPACE;

	إذا (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_اندراج(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, قدر,
					 قدر_len, وچ_inode);
		إذا (ret)
			أرجع ret;
		إذا (old_idx < h->ibody_گنتی) {
			/*
			 * Move entry from ibody to the block. Note that
			 * entries in the block are sorted.
			 */
			new_idx = xattr_لبھو_position(h->attrs + h->ibody_گنتی,
						      h->گنتی - h->ibody_گنتی,
						      shortname, name_idx);
			new_idx += h->ibody_گنتی - 1;
			tmp = h->attrs[old_idx];
			memmove(h->attrs + old_idx, h->attrs + old_idx + 1,
				(new_idx - old_idx) * حجم(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_گنتی--;
		}
		أرجع 0;
	}

	new_idx = xattr_لبھو_position(h->attrs + h->ibody_گنتی,
				      h->گنتی - h->ibody_گنتی,
				      shortname, name_idx);
	new_idx += h->ibody_گنتی;
	جوڑو_منزل_ibody = 0;

جوڑو_new:
	إذا (h->گنتی == h->سمرتھا) {
		ret = ext2fs_xattrs_expand(h, 4);
		إذا (ret)
			أرجع ret;
	}

	ret = xattr_update_اندراج(h->fs, &h->attrs[h->گنتی], name, shortname,
				 name_idx, قدر, قدر_len, وچ_inode);
	إذا (ret)
		أرجع ret;

	tmp = h->attrs[h->گنتی];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->گنتی - new_idx)*حجم(*h->attrs));
	h->attrs[new_idx] = tmp;
	إذا (جوڑو_منزل_ibody)
		h->ibody_گنتی++;
	h->گنتی++;
	أرجع 0;
}

ساكن صحيح space_ورتیا(بنية ext2_xattr *attrs, صحيح گنتی)
{
	صحيح کل = 0;
	بنية ext2_xattr *x;
	صحيح i, len;

	لكل (i = 0, x = attrs; i < گنتی; i++, x++) {
		len = لڑی_لمبائی(x->short_name);
		کل += EXT2_EXT_ATTR_LEN(len);
		إذا (!x->ea_ino)
			کل += EXT2_EXT_ATTR_ناپ(x->قدر_len);
	}
	أرجع کل;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#تعریف_کرو EXT4_XATTR_MIN_LARGE_EA_ناپ(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - حجم(بنية ext2_ext_attr_سرخی) - 4)

errcode_t ext2fs_xattr_سیٹ(بنية ext2_xattr_handle *h,
			   ثابت حرف *name,
			   ثابت فراغ *قدر,
			   size_t قدر_len)
{
	ext2_filsys fs = h->fs;
	ثابت صحيح inode_ناپ = EXT2_INODE_ناپ(fs->super);
	بنية ext2_inode_large *inode = عدم;
	بنية ext2_xattr *x;
	حرف *new_قدر;
	صحيح ibody_free, بلاک_free;
	صحيح وچ_inode = 0;
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
			اذهب باہرلا;
	} وإلا إذا (قدر_len)
		حافظہ_نقل(new_قدر, قدر, قدر_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	لكل (x = h->attrs; x < h->attrs + h->گنتی; x++) {
		إذا (!strcmp(x->name, name)) {
			إذا (!x->ea_ino && x->قدر_len == قدر_len &&
			    (!قدر_len ||
			     !memcmp(x->قدر, new_قدر, قدر_len))) {
				ret = 0;
				اذهب باہرلا;
			}
			old_idx = x - h->attrs;
			اكسر;
		}
	}

	ret = ext2fs_get_memzero(inode_ناپ, &inode);
	إذا (ret)
		اذهب باہرلا;
	ret = ext2fs_read_inode_بھریا(fs, h->ino,
				     (بنية ext2_inode *)inode,
				     inode_ناپ);
	إذا (ret)
		اذهب باہرلا;
	إذا (inode_ناپ > EXT2_GOOD_OLD_INODE_ناپ) {
		extra_isize = inode->i_extra_isize;
		إذا (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			إذا (extra_isize == 0)
				extra_isize = حجم(__u32);
		}
		ibody_free = inode_ناپ - EXT2_GOOD_OLD_INODE_ناپ;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= حجم(__u32) * 2;
		ibody_free -= space_ورتیا(h->attrs, h->ibody_گنتی);
	} وإلا
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	إذا (strcmp(name, "system.data") == 0) {
		إذا (h->ibody_گنتی <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			اذهب باہرلا;
		}
		ret = xattr_array_update(h, name, new_قدر, قدر_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		إذا (ret)
			اذهب باہرلا;
		اذهب write_باہرلا;
	}

	بلاک_free = fs->blocksize;
	بلاک_free -= حجم(بنية ext2_ext_attr_سرخی);
	/* Final null entry. */
	بلاک_free -= حجم(__u32);
	بلاک_free -= space_ورتیا(h->attrs + h->ibody_گنتی,
				 h->گنتی - h->ibody_گنتی);

	إذا (ext2fs_has_feature_ea_inode(fs->super) &&
	    قدر_len > EXT4_XATTR_MIN_LARGE_EA_ناپ(fs->blocksize))
		وچ_inode = 1;

	ret = xattr_array_update(h, name, new_قدر, قدر_len, ibody_free,
				 بلاک_free, old_idx, وچ_inode);
	إذا (ret == EXT2_ET_EA_NO_SPACE && !وچ_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_قدر, قدر_len,
			ibody_free, بلاک_free, old_idx, 1 /* in_inode */);
	إذا (ret)
		اذهب باہرلا;

write_باہرلا:
	ret = ext2fs_xattrs_write(h);
باہرلا:
	إذا (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_قدر);
	أرجع ret;
}

errcode_t ext2fs_xattr_remove(بنية ext2_xattr_handle *handle,
			      ثابت حرف *کلید)
{
	بنية ext2_xattr *x;
	بنية ext2_xattr *سرا = handle->attrs + handle->گنتی;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = handle->attrs; x < سرا; x++) {
		إذا (strcmp(x->name, کلید) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->قدر);
			إذا (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (سرا - x - 1)*حجم(*x));
			memset(سرا - 1, 0, حجم(*سرا));
			إذا (x < handle->attrs + handle->ibody_گنتی)
				handle->ibody_گنتی--;
			handle->گنتی--;
			أرجع ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	أرجع 0;
}

errcode_t ext2fs_xattr_remove_all(بنية ext2_xattr_handle *handle)
{
	بنية ext2_xattr *x;
	بنية ext2_xattr *سرا = handle->attrs + handle->گنتی;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	لكل (x = handle->attrs; x < سرا; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->قدر);
		إذا (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_گنتی = 0;
	handle->گنتی = 0;
	أرجع ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     بنية ext2_xattr_handle **handle)
{
	بنية ext2_xattr_handle *h;
	errcode_t err;

	إذا (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_انکڑے(fs->super))
		أرجع EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(حجم(*h), &h);
	إذا (err)
		أرجع err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->سمرتھا = 4;
	err = ext2fs_get_arrayzero(h->سمرتھا, حجم(بنية ext2_xattr),
				   &h->attrs);
	إذا (err) {
		ext2fs_free_mem(&h);
		أرجع err;
	}
	h->گنتی = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	أرجع 0;
}

errcode_t ext2fs_xattrs_close(بنية ext2_xattr_handle **handle)
{
	بنية ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_کلیدیں(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	أرجع 0;
}

errcode_t ext2fs_xattrs_گنتی(بنية ext2_xattr_handle *handle, size_t *گنتی)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*گنتی = handle->گنتی;
	أرجع 0;
}

errcode_t ext2fs_xattrs_flags(بنية ext2_xattr_handle *handle,
			      بنانشانی صحيح *new_flags, بنانشانی صحيح *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	إذا (old_flags)
		*old_flags = handle->flags;
	إذا (new_flags)
		handle->flags = *new_flags;
	أرجع 0;
}
