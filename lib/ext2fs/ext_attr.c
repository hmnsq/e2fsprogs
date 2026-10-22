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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2_ext_attr.h"
#ಅನ್ತರ್ಭಾವ "ext4_acl.h"

#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ಸ್ಥಿರ errcode_t read_ea_inode_hash(ext2_filsys fs, ext2_ino_t ino, __u32 *hash)
{
	ರಚನೆ ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿಸು NAME_HASH_ಸರಿಸು 5
#ವ್ಯಾಖ್ಯಾನಿಸು ಮೌಲ್ಯ_HASH_ಸರಿಸು 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_ನಮೂದು(ರಚನೆ ext2_ext_attr_ನಮೂದು *ನಮೂದು, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	__u32 hash = 0;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *name = (((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) ನಮೂದು) +
			       ಗಾತ್ರ(ರಚನೆ ext2_ext_attr_ನಮೂದು));
	ಪೂರ್ಣಾಂಕ n;

	ಗಾಗಿ (n = 0; n < ನಮೂದು->e_name_len; n++) {
		hash = (hash << NAME_HASH_ಸರಿಸು) ^
		       (hash >> (8*ಗಾತ್ರ(hash) - NAME_HASH_ಸರಿಸು)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	ಆದರೆ (ನಮೂದು->e_ಮೌಲ್ಯ_inum == 0 && ನಮೂದು->e_ಮೌಲ್ಯ_ಗಾತ್ರ != 0) {
		__u32 *ಮೌಲ್ಯ = (__u32 *)ದತ್ತಾಂಶ;
		ಗಾಗಿ (n = (ನಮೂದು->e_ಮೌಲ್ಯ_ಗಾತ್ರ + EXT2_EXT_ATTR_ಸುತ್ತು) >>
			 EXT2_EXT_ATTR_PAD_ಬಿಟ್ಟುಗಳು; n; n--) {
			hash = (hash << ಮೌಲ್ಯ_HASH_ಸರಿಸು) ^
			       (hash >> (8*ಗಾತ್ರ(hash) - ಮೌಲ್ಯ_HASH_ಸರಿಸು)) ^
			       ext2fs_le32_ಗಮ್ಯ_cpu(*ಮೌಲ್ಯ++);
		}
	}

	ಹಿಂತಿರುಗಿ hash;
}

__u32 ext2fs_ext_attr_hash_ನಮೂದು_signed(ರಚನೆ ext2_ext_attr_ನಮೂದು *ನಮೂದು,
					ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	__u32 hash = 0;
	ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ *name = (((ಚಿಹ್ನೆಯುಳ್ಳ ಅಕ್ಷರ *) ನಮೂದು) +
			     ಗಾತ್ರ(ರಚನೆ ext2_ext_attr_ನಮೂದು));
	ಪೂರ್ಣಾಂಕ n;

	ಗಾಗಿ (n = 0; n < ನಮೂದು->e_name_len; n++) {
		hash = (hash << NAME_HASH_ಸರಿಸು) ^
		       (hash >> (8*ಗಾತ್ರ(hash) - NAME_HASH_ಸರಿಸು)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	ಆದರೆ (ನಮೂದು->e_ಮೌಲ್ಯ_inum == 0 && ನಮೂದು->e_ಮೌಲ್ಯ_ಗಾತ್ರ != 0) {
		__u32 *ಮೌಲ್ಯ = (__u32 *)ದತ್ತಾಂಶ;
		ಗಾಗಿ (n = (ನಮೂದು->e_ಮೌಲ್ಯ_ಗಾತ್ರ + EXT2_EXT_ATTR_ಸುತ್ತು) >>
			 EXT2_EXT_ATTR_PAD_ಬಿಟ್ಟುಗಳು; n; n--) {
			hash = (hash << ಮೌಲ್ಯ_HASH_ಸರಿಸು) ^
			       (hash >> (8*ಗಾತ್ರ(hash) - ಮೌಲ್ಯ_HASH_ಸರಿಸು)) ^
			       ext2fs_le32_ಗಮ್ಯ_cpu(*ಮೌಲ್ಯ++);
		}
	}

	ಹಿಂತಿರುಗಿ hash;
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
				      ರಚನೆ ext2_ext_attr_ನಮೂದು *ನಮೂದು,
				      ಶೂನ್ಯ *ದತ್ತಾಂಶ, __u32 *hash,
				      __u32 *signed_hash)
{
	*hash = ext2fs_ext_attr_hash_ನಮೂದು(ನಮೂದು, ದತ್ತಾಂಶ);
	ಆದರೆ (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_ನಮೂದು_signed(ನಮೂದು, ದತ್ತಾಂಶ);

	ಆದರೆ (ನಮೂದು->e_ಮೌಲ್ಯ_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, ನಮೂದು->e_ಮೌಲ್ಯ_inum,
					    &ea_inode_hash);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

		*hash = (*hash << ಮೌಲ್ಯ_HASH_ಸರಿಸು) ^
			(*hash >> (8*ಗಾತ್ರ(*hash) - ಮೌಲ್ಯ_HASH_ಸರಿಸು)) ^
			ea_inode_hash;
		ಆದರೆ (signed_hash)
			*signed_hash = (*signed_hash << ಮೌಲ್ಯ_HASH_ಸರಿಸು) ^
				(*signed_hash >> (8*ಗಾತ್ರ(*hash) -
						  ಮೌಲ್ಯ_HASH_ಸರಿಸು)) ^
				ea_inode_hash;
	}
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * ext2fs_ext_attr_hash_entry2()
 *
 * Compute the hash of an extended attribute.
 * This version of the function supports hashing entries that reference
 * external inodes (ea_inode feature).
 */
errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
				      ರಚನೆ ext2_ext_attr_ನಮೂದು *ನಮೂದು,
				      ಶೂನ್ಯ *ದತ್ತಾಂಶ, __u32 *hash)
{
	ಹಿಂತಿರುಗಿ ext2fs_ext_attr_hash_entry3(fs, ನಮೂದು, ದತ್ತಾಂಶ, hash, ಖಾಲಿ);
}

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು NAME_HASH_ಸರಿಸು
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ಮೌಲ್ಯ_HASH_ಸರಿಸು

#ವ್ಯಾಖ್ಯಾನಿಸು ಖಂಡ_HASH_ಸರಿಸು 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
ಶೂನ್ಯ ext2fs_ext_attr_ಖಂಡ_rehash(ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *ಶೀರ್ಷಿಕೆ,
				  ರಚನೆ ext2_ext_attr_ನಮೂದು *ಕೊನೆ)
{
	ರಚನೆ ext2_ext_attr_ನಮೂದು *ಇಲ್ಲಿ;
	__u32 hash = 0;

	ಇಲ್ಲಿ = (ರಚನೆ ext2_ext_attr_ನಮೂದು *)(ಶೀರ್ಷಿಕೆ+1);
	ಆಗ (ಇಲ್ಲಿ < ಕೊನೆ && !EXT2_EXT_IS_ಕೊನೆಯ_ನಮೂದು(ಇಲ್ಲಿ)) {
		ಆದರೆ (!ಇಲ್ಲಿ->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			ನಿಲ್ಲಿಸು;
		}
		hash = (hash << ಖಂಡ_HASH_ಸರಿಸು) ^
		       (hash >> (8*ಗಾತ್ರ(hash) - ಖಂಡ_HASH_ಸರಿಸು)) ^
		       ಇಲ್ಲಿ->e_hash;
		ಇಲ್ಲಿ = EXT2_EXT_ATTR_ಮುಂದಿನ(ಇಲ್ಲಿ);
	}
	ಶೀರ್ಷಿಕೆ->h_hash = hash;
}

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ಖಂಡ_HASH_ಸರಿಸು

__u32 ext2fs_get_ea_inode_hash(ರಚನೆ ext2_inode *inode)
{
	ಹಿಂತಿರುಗಿ inode->i_atime;
}

ಶೂನ್ಯ ext2fs_ಗಣ_ea_inode_hash(ರಚನೆ ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(ರಚನೆ ext2_inode *inode)
{
	ಹಿಂತಿರುಗಿ ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_ಆವೃತ್ತಿ;
}

ಶೂನ್ಯ ext2fs_ಗಣ_ea_inode_ref(ರಚನೆ ext2_inode *inode, __u64 ref_ಎಣಿಕೆ)
{
	inode->i_ctime = (__u32)(ref_ಎಣಿಕೆ >> 32);
	inode->osd1.linux1.l_i_ಆವೃತ್ತಿ = (__u32)ref_ಎಣಿಕೆ;
}

ಸ್ಥಿರ errcode_t check_ext_attr_ಶೀರ್ಷಿಕೆ(ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *ಶೀರ್ಷಿಕೆ)
{
	ಆದರೆ ((ಶೀರ್ಷಿಕೆ->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     ಶೀರ್ಷಿಕೆ->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    ಶೀರ್ಷಿಕೆ->h_ಖಂಡಗಳು != 1)
		ಹಿಂತಿರುಗಿ EXT2_ET_BAD_EA_ಶೀರ್ಷಿಕೆ;

	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t ಖಂಡ, ಶೂನ್ಯ *buf,
				ext2_ino_t inum)
{
	ಪೂರ್ಣಾಂಕ		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, ಖಂಡ, 1, buf);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಆದರೆ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_ಖಂಡ_csum_verify(fs, inum, ಖಂಡ, buf))
		csum_failed = 1;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	ext2fs_ಅದಲುಬದಲು_ext_attr(buf, buf, fs->blocksize, 1);
#ಆದರೆ_ಅಂತ್ಯ

	retval = check_ext_attr_ಶೀರ್ಷಿಕೆ(buf);
	ಆದರೆ (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t ಖಂಡ, ಶೂನ್ಯ *buf)
{
	ಹಿಂತಿರುಗಿ ext2fs_read_ext_attr3(fs, ಖಂಡ, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t ಖಂಡ, ಶೂನ್ಯ *buf)
{
	ಹಿಂತಿರುಗಿ ext2fs_read_ext_attr2(fs, ಖಂಡ, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t ಖಂಡ, ಶೂನ್ಯ *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	ಅಕ್ಷರ		*write_buf;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ext2fs_ಅದಲುಬದಲು_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#ಇಲ್ಲದಿದ್ದರೆ
	write_buf = (ಅಕ್ಷರ *) inbuf;
#ಆದರೆ_ಅಂತ್ಯ

	retval = ext2fs_ext_attr_ಖಂಡ_csum_ಗಣ(fs, inum, ಖಂಡ,
			(ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *)write_buf);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	retval = io_channel_write_blk64(fs->io, ಖಂಡ, 1, write_buf);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#ಆದರೆ_ಅಂತ್ಯ
	ಆದರೆ (!retval)
		ext2fs_mark_ಬದಲಾಯಿತು(fs);
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t ಖಂಡ, ಶೂನ್ಯ *inbuf)
{
	ಹಿಂತಿರುಗಿ ext2fs_write_ext_attr3(fs, ಖಂಡ, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t ಖಂಡ, ಶೂನ್ಯ *inbuf)
{
	ಹಿಂತಿರುಗಿ ext2fs_write_ext_attr2(fs, ಖಂಡ, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    ಅಕ್ಷರ *ಖಂಡ_buf, ಪೂರ್ಣಾಂಕ adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *ಶೀರ್ಷಿಕೆ;
	ಅಕ್ಷರ	*buf = 0;

	ಆದರೆ ((blk >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super)) ||
	    (blk < fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ))
		ಹಿಂತಿರುಗಿ EXT2_ET_BAD_EA_ಖಂಡ_ಅಂಕೆ;

	ಆದರೆ (!ಖಂಡ_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		ಖಂಡ_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, ಖಂಡ_buf, inum);
	ಆದರೆ (retval)
		ಹೋಗು errout;

	ಶೀರ್ಷಿಕೆ = (ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *) ಖಂಡ_buf;
	ಶೀರ್ಷಿಕೆ->h_refcount += adjust;
	ಆದರೆ (newcount)
		*newcount = ಶೀರ್ಷಿಕೆ->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, ಖಂಡ_buf, inum);
	ಆದರೆ (retval)
		ಹೋಗು errout;

errout:
	ಆದರೆ (buf)
		ext2fs_free_mem(&buf);
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    ಅಕ್ಷರ *ಖಂಡ_buf, ಪೂರ್ಣಾಂಕ adjust,
				    __u32 *newcount)
{
	ಹಿಂತಿರುಗಿ ext2fs_adjust_ea_refcount3(fs, blk, ಖಂಡ_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					ಅಕ್ಷರ *ಖಂಡ_buf, ಪೂರ್ಣಾಂಕ adjust,
					__u32 *newcount)
{
	ಹಿಂತಿರುಗಿ ext2fs_adjust_ea_refcount2(fs, blk, ಖಂಡ_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
ರಚನೆ ext2_xattr {
	ಪೂರ್ಣಾಂಕ name_ಸೂಚ್ಯಂಕ;
	ಅಕ್ಷರ *name;
	ಅಕ್ಷರ *short_name;
	ಶೂನ್ಯ *ಮೌಲ್ಯ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ_len;
	ext2_ino_t ea_ino;
};

ರಚನೆ ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	ರಚನೆ ext2_xattr *attrs;
	ಪೂರ್ಣಾಂಕ ಸಾಮರ್ಥ್ಯ;
	ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2;
	ಪೂರ್ಣಾಂಕ ibody_ಎಣಿಕೆ;
	ext2_ino_t ino;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags;
};

ಸ್ಥಿರ errcode_t ext2fs_xattrs_expand(ರಚನೆ ext2_xattr_handle *h,
				      ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ expandby)
{
	ರಚನೆ ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->ಸಾಮರ್ಥ್ಯ + expandby,
				   ಗಾತ್ರ(ರಚನೆ ext2_xattr), &new_attrs);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ err;

	ಸ್ಮರಣೆ_ನಕಲು(new_attrs, h->attrs, h->ಸಾಮರ್ಥ್ಯ * ಗಾತ್ರ(ರಚನೆ ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->ಸಾಮರ್ಥ್ಯ += expandby;
	h->attrs = new_attrs;

	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ ea_name_ಸೂಚ್ಯಂಕ {
	ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
};

/* Keep these names sorted in order of decreasing specificity. */
ಸ್ಥಿರ ರಚನೆ ea_name_ಸೂಚ್ಯಂಕ ea_ಹೆಸರುಗಳು[] = {
	{10, "gnu."},
	{3, "system.posix_acl_default"},
	{2, "system.posix_acl_access"},
	{8, "system.richacl"},
	{6, "security."},
	{4, "trusted."},
	{7, "system."},
	{1, "user."},
	{0, ಖಾಲಿ},
};

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕಂಡುಹಿಡಿ_ea_ಪೂರ್ವಪ್ರತ್ಯಯ(ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ)
{
	ರಚನೆ ea_name_ಸೂಚ್ಯಂಕ *e;

	ಗಾಗಿ (e = ea_ಹೆಸರುಗಳು; e->name; e++)
		ಆದರೆ (e->ಸೂಚ್ಯಂಕ == ಸೂಚ್ಯಂಕ)
			ಹಿಂತಿರುಗಿ e->name;

	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕಂಡುಹಿಡಿ_ea_ಸೂಚ್ಯಂಕ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fullname, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **name, ಪೂರ್ಣಾಂಕ *ಸೂಚ್ಯಂಕ)
{
	ರಚನೆ ea_name_ಸೂಚ್ಯಂಕ *e;

	ಗಾಗಿ (e = ea_ಹೆಸರುಗಳು; e->name; e++) {
		ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(fullname, e->name, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(e->name)) == 0) {
			*name = fullname + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(e->name);
			*ಸೂಚ್ಯಂಕ = e->ಸೂಚ್ಯಂಕ;
			ಹಿಂತಿರುಗಿ 1;
		}
	}
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       ರಚನೆ ext2_inode_large *inode)
{
	ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *ಶೀರ್ಷಿಕೆ;
	ಶೂನ್ಯ *ಖಂಡ_buf = ಖಾಲಿ;
	blk64_t blk;
	errcode_t err;
	ರಚನೆ ext2_inode_large i;

	/* Read inode? */
	ಆದರೆ (inode == ಖಾಲಿ) {
		err = ext2fs_read_inode_ತುಂಬಿದ(fs, ino, (ರಚನೆ ext2_inode *)&i,
					     ಗಾತ್ರ(ರಚನೆ ext2_inode_large));
		ಆದರೆ (err)
			ಹಿಂತಿರುಗಿ err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_ಖಂಡ(fs, (ರಚನೆ ext2_inode *)inode);
	ಆದರೆ (blk == 0)
		ಹಿಂತಿರುಗಿ 0;

	/* Find block, zero it, write back */
	ಆದರೆ ((blk < fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ) ||
	    (blk >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super))) {
		err = EXT2_ET_BAD_EA_ಖಂಡ_ಅಂಕೆ;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	err = ext2fs_get_mem(fs->blocksize, &ಖಂಡ_buf);
	ಆದರೆ (err)
		ಹೋಗು ಹೊರಹೋಗು;

	err = ext2fs_read_ext_attr3(fs, blk, ಖಂಡ_buf, ino);
	ಆದರೆ (err)
		ಹೋಗು out2;

	/* We only know how to deal with v2 EA blocks */
	ಶೀರ್ಷಿಕೆ = (ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *) ಖಂಡ_buf;
	ಆದರೆ (ಶೀರ್ಷಿಕೆ->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_ಶೀರ್ಷಿಕೆ;
		ಹೋಗು out2;
	}

	ಶೀರ್ಷಿಕೆ->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, ಖಂಡ_buf, ino);
	ಆದರೆ (err)
		ಹೋಗು out2;

	/* Erase link to block */
	ext2fs_file_acl_ಖಂಡ_ಗಣ(fs, (ರಚನೆ ext2_inode *)inode, 0);
	ಆದರೆ (ಶೀರ್ಷಿಕೆ->h_refcount == 0)
		ext2fs_ಖಂಡ_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_ಖಂಡಗಳು(fs, (ರಚನೆ ext2_inode *)inode, 1);
	ಆದರೆ (err)
		ಹೋಗು out2;

	/* Write inode? */
	ಆದರೆ (inode == &i) {
		err = ext2fs_write_inode_ತುಂಬಿದ(fs, ino, (ರಚನೆ ext2_inode *)&i,
					      ಗಾತ್ರ(ರಚನೆ ext2_inode_large));
		ಆದರೆ (err)
			ಹೋಗು out2;
	}

out2:
	ext2fs_free_mem(&ಖಂಡ_buf);
ಹೊರಹೋಗು:
	ಹಿಂತಿರುಗಿ err;
}

ಸ್ಥಿರ errcode_t prep_ea_ಖಂಡ_for_write(ext2_filsys fs, ext2_ino_t ino,
					 ರಚನೆ ext2_inode_large *inode)
{
	ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *ಶೀರ್ಷಿಕೆ;
	ಶೂನ್ಯ *ಖಂಡ_buf = ಖಾಲಿ;
	blk64_t blk, goal;
	errcode_t err;

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_ಖಂಡ(fs, (ರಚನೆ ext2_inode *)inode);
	ಆದರೆ (blk != 0) {
		ಆದರೆ ((blk < fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ) ||
		    (blk >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super))) {
			err = EXT2_ET_BAD_EA_ಖಂಡ_ಅಂಕೆ;
			ಹೋಗು ಹೊರಹೋಗು;
		}

		err = ext2fs_get_mem(fs->blocksize, &ಖಂಡ_buf);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;

		err = ext2fs_read_ext_attr3(fs, blk, ಖಂಡ_buf, ino);
		ಆದರೆ (err)
			ಹೋಗು out2;

		/* We only know how to deal with v2 EA blocks */
		ಶೀರ್ಷಿಕೆ = (ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *) ಖಂಡ_buf;
		ಆದರೆ (ಶೀರ್ಷಿಕೆ->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_ಶೀರ್ಷಿಕೆ;
			ಹೋಗು out2;
		}

		/* Single-user block.  We're done here. */
		ಆದರೆ (ಶೀರ್ಷಿಕೆ->h_refcount == 1)
			ಹೋಗು out2;

		/* We need to CoW the block. */
		ಶೀರ್ಷಿಕೆ->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, ಖಂಡ_buf, ino);
		ಆದರೆ (err)
			ಹೋಗು out2;
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_ಕೂಡಿಸು_ಖಂಡಗಳು(fs, (ರಚನೆ ext2_inode *)inode,
					     1);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;
	}

	/* Allocate a block */
	goal = ext2fs_ಕಂಡುಹಿಡಿ_inode_goal(fs, ino, (ರಚನೆ ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, ಖಾಲಿ, &blk);
	ಆದರೆ (err)
		ಹೋಗು out2;
	ext2fs_file_acl_ಖಂಡ_ಗಣ(fs, (ರಚನೆ ext2_inode *)inode, blk);
out2:
	ಆದರೆ (ಖಂಡ_buf)
		ext2fs_free_mem(&ಖಂಡ_buf);
ಹೊರಹೋಗು:
	ಹಿಂತಿರುಗಿ err;
}


ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
posix_acl_xattr_ಎಣಿಕೆ(size_t ಗಾತ್ರ_2)
{
        ಆದರೆ (ಗಾತ್ರ_2 < ಗಾತ್ರ(posix_acl_xattr_ಶೀರ್ಷಿಕೆ))
                ಹಿಂತಿರುಗಿ -1;
        ಗಾತ್ರ_2 -= ಗಾತ್ರ(posix_acl_xattr_ಶೀರ್ಷಿಕೆ);
        ಆದರೆ (ಗಾತ್ರ_2 % ಗಾತ್ರ(posix_acl_xattr_ನಮೂದು))
                ಹಿಂತಿರುಗಿ -1;
        ಹಿಂತಿರುಗಿ ಗಾತ್ರ_2 / ಗಾತ್ರ(posix_acl_xattr_ನಮೂದು);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
ಸ್ಥಿರ errcode_t convert_posix_acl_ಗಮ್ಯ_disk_ಬಫರ್(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಮೌಲ್ಯ, size_t ಗಾತ್ರ_2,
						  ಶೂನ್ಯ *ಹೊರಹೋಗು_buf, size_t *ಗಾತ್ರ_ಹೊರಹೋಗು)
{
	ಸ್ಥಿರಾಂಕ posix_acl_xattr_ಶೀರ್ಷಿಕೆ *ಶೀರ್ಷಿಕೆ =
		(ಸ್ಥಿರಾಂಕ posix_acl_xattr_ಶೀರ್ಷಿಕೆ*) ಮೌಲ್ಯ;
	ಸ್ಥಿರಾಂಕ posix_acl_xattr_ನಮೂದು *ಕೊನೆ, *ನಮೂದು =
		(ಸ್ಥಿರಾಂಕ posix_acl_xattr_ನಮೂದು *)(ಶೀರ್ಷಿಕೆ+1);
	ext4_acl_ಶೀರ್ಷಿಕೆ *ext_acl;
	size_t s;
	ಅಕ್ಷರ *e;

	ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2;

	ಆದರೆ (!ಮೌಲ್ಯ)
		ಹಿಂತಿರುಗಿ EINVAL;
	ಆದರೆ (ಗಾತ್ರ_2 < ಗಾತ್ರ(posix_acl_xattr_ಶೀರ್ಷಿಕೆ))
		ಹಿಂತಿರುಗಿ ENOMEM;
	ಆದರೆ (ಶೀರ್ಷಿಕೆ->a_ಆವೃತ್ತಿ != ext2fs_cpu_ಗಮ್ಯ_le32(POSIX_ACL_XATTR_ಆವೃತ್ತಿ))
		ಹಿಂತಿರುಗಿ EINVAL;

	ಎಣಿಕೆ_2 = posix_acl_xattr_ಎಣಿಕೆ(ಗಾತ್ರ_2);
	ext_acl = ಹೊರಹೋಗು_buf;
	ext_acl->a_ಆವೃತ್ತಿ = ext2fs_cpu_ಗಮ್ಯ_le32(EXT4_ACL_ಆವೃತ್ತಿ);

	ಆದರೆ (ಎಣಿಕೆ_2 <= 0)
		ಹಿಂತಿರುಗಿ EINVAL;

	e = (ಅಕ್ಷರ *) ಹೊರಹೋಗು_buf + ಗಾತ್ರ(ext4_acl_ಶೀರ್ಷಿಕೆ);
	s = ಗಾತ್ರ(ext4_acl_ಶೀರ್ಷಿಕೆ);
	ಗಾಗಿ (ಕೊನೆ = ನಮೂದು + ಎಣಿಕೆ_2; ನಮೂದು != ಕೊನೆ;ನಮೂದು++) {
		ext4_acl_ನಮೂದು *disk_ನಮೂದು = (ext4_acl_ನಮೂದು*) e;
		disk_ನಮೂದು->e_ಟ್ಯಾಗ್ = ನಮೂದು->e_ಟ್ಯಾಗ್;
		disk_ನಮೂದು->e_perm = ನಮೂದು->e_perm;

		ಆಯ್ಕೆ(ext2fs_le16_ಗಮ್ಯ_cpu(ನಮೂದು->e_ಟ್ಯಾಗ್)) {
			ಸ್ಥಿತಿ ACL_USER_OBJ:
			ಸ್ಥಿತಿ ACL_ಗುಂಪು_OBJ:
			ಸ್ಥಿತಿ ACL_ಮುಖವಾಡ:
			ಸ್ಥಿತಿ ACL_OTHER:
				e += ಗಾತ್ರ(ext4_acl_ನಮೂದು_short);
				s += ಗಾತ್ರ(ext4_acl_ನಮೂದು_short);
				ನಿಲ್ಲಿಸು;
			ಸ್ಥಿತಿ ACL_USER:
			ಸ್ಥಿತಿ ACL_ಗುಂಪು:
				disk_ನಮೂದು->e_id = ನಮೂದು->e_id;
				e += ಗಾತ್ರ(ext4_acl_ನಮೂದು);
				s += ಗಾತ್ರ(ext4_acl_ನಮೂದು);
				ನಿಲ್ಲಿಸು;
			ಪೂರ್ವನಿಯೋಜಿತ:
				ಹಿಂತಿರುಗಿ EINVAL;
		}
	}
	*ಗಾತ್ರ_ಹೊರಹೋಗು = s;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t convert_disk_ಬಫರ್_ಗಮ್ಯ_posix_acl(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಮೌಲ್ಯ, size_t ಗಾತ್ರ_2,
						  ಶೂನ್ಯ **ಹೊರಹೋಗು_buf, size_t *ಗಾತ್ರ_ಹೊರಹೋಗು)
{
	posix_acl_xattr_ಶೀರ್ಷಿಕೆ *ಶೀರ್ಷಿಕೆ;
	posix_acl_xattr_ನಮೂದು *ನಮೂದು;
	ಸ್ಥಿರಾಂಕ ext4_acl_ಶೀರ್ಷಿಕೆ *ext_acl = (ಸ್ಥಿರಾಂಕ ext4_acl_ಶೀರ್ಷಿಕೆ *) ಮೌಲ್ಯ;
	errcode_t err;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cp;
	ಅಕ್ಷರ *ಹೊರಹೋಗು;

	ಆದರೆ ((!ಮೌಲ್ಯ) ||
	    (ಗಾತ್ರ_2 < ಗಾತ್ರ(ext4_acl_ಶೀರ್ಷಿಕೆ)) ||
	    (ext_acl->a_ಆವೃತ್ತಿ != ext2fs_cpu_ಗಮ್ಯ_le32(EXT4_ACL_ಆವೃತ್ತಿ)))
		ಹಿಂತಿರುಗಿ EINVAL;

	err = ext2fs_get_mem(ಗಾತ್ರ_2 * 2, &ಹೊರಹೋಗು);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ err;

	ಶೀರ್ಷಿಕೆ = (posix_acl_xattr_ಶೀರ್ಷಿಕೆ *) ಹೊರಹೋಗು;
	ಶೀರ್ಷಿಕೆ->a_ಆವೃತ್ತಿ = ext2fs_cpu_ಗಮ್ಯ_le32(POSIX_ACL_XATTR_ಆವೃತ್ತಿ);
	ನಮೂದು = (posix_acl_xattr_ನಮೂದು *) (ಹೊರಹೋಗು + ಗಾತ್ರ(posix_acl_xattr_ಶೀರ್ಷಿಕೆ));

	cp = (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *) ಮೌಲ್ಯ + ಗಾತ್ರ(ext4_acl_ಶೀರ್ಷಿಕೆ);
	ಗಾತ್ರ_2 -= ಗಾತ್ರ(ext4_acl_ಶೀರ್ಷಿಕೆ);

	ಆಗ (ಗಾತ್ರ_2 > 0) {
		ಸ್ಥಿರಾಂಕ ext4_acl_ನಮೂದು *disk_ನಮೂದು = (ಸ್ಥಿರಾಂಕ ext4_acl_ನಮೂದು *) cp;

		ನಮೂದು->e_ಟ್ಯಾಗ್ = disk_ನಮೂದು->e_ಟ್ಯಾಗ್;
		ನಮೂದು->e_perm = disk_ನಮೂದು->e_perm;

		ಆಯ್ಕೆ(ext2fs_le16_ಗಮ್ಯ_cpu(ನಮೂದು->e_ಟ್ಯಾಗ್)) {
			ಸ್ಥಿತಿ ACL_USER_OBJ:
			ಸ್ಥಿತಿ ACL_ಗುಂಪು_OBJ:
			ಸ್ಥಿತಿ ACL_ಮುಖವಾಡ:
			ಸ್ಥಿತಿ ACL_OTHER:
				ನಮೂದು->e_id = 0;
				cp += ಗಾತ್ರ(ext4_acl_ನಮೂದು_short);
				ಗಾತ್ರ_2 -= ಗಾತ್ರ(ext4_acl_ನಮೂದು_short);
				ನಿಲ್ಲಿಸು;
			ಸ್ಥಿತಿ ACL_USER:
			ಸ್ಥಿತಿ ACL_ಗುಂಪು:
				ನಮೂದು->e_id = disk_ನಮೂದು->e_id;
				cp += ಗಾತ್ರ(ext4_acl_ನಮೂದು);
				ಗಾತ್ರ_2 -= ಗಾತ್ರ(ext4_acl_ನಮೂದು);
				ನಿಲ್ಲಿಸು;
			ಪೂರ್ವನಿಯೋಜಿತ:
				ext2fs_free_mem(&ಹೊರಹೋಗು);
				ಹಿಂತಿರುಗಿ EINVAL;
		}
		ನಮೂದು++;
	}
	*ಹೊರಹೋಗು_buf = ಹೊರಹೋಗು;
	*ಗಾತ್ರ_ಹೊರಹೋಗು = ((ಅಕ್ಷರ *) ನಮೂದು - ಹೊರಹೋಗು);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t
write_xattrs_ಗಮ್ಯ_ಬಫರ್(ext2_filsys fs, ರಚನೆ ext2_xattr *attrs, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2,
		       ಶೂನ್ಯ *entries_ಆರಂಭ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ storage_ಗಾತ್ರ,
		       ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ_ಸ್ಥಾನಾಂತರ_correction, ಪೂರ್ಣಾಂಕ write_hash)
{
	ರಚನೆ ext2_xattr *x;
	ರಚನೆ ext2_ext_attr_ನಮೂದು *e = entries_ಆರಂಭ;
	ಅಕ್ಷರ *ಕೊನೆ = (ಅಕ್ಷರ *) entries_ಆರಂಭ + storage_ಗಾತ್ರ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ_ಗಾತ್ರ;
	errcode_t err;

	memset(entries_ಆರಂಭ, 0, storage_ಗಾತ್ರ);
	ಗಾಗಿ (x = attrs; x < attrs + ಎಣಿಕೆ_2; x++) {
		ಮೌಲ್ಯ_ಗಾತ್ರ = ((x->ಮೌಲ್ಯ_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(x->short_name);
		e->e_name_ಸೂಚ್ಯಂಕ = x->name_ಸೂಚ್ಯಂಕ;

		e->e_ಮೌಲ್ಯ_ಗಾತ್ರ = x->ಮೌಲ್ಯ_len;
		e->e_ಮೌಲ್ಯ_inum = x->ea_ino;

		/* Store name */
		ಸ್ಮರಣೆ_ನಕಲು((ಅಕ್ಷರ *)e + ಗಾತ್ರ(*e), x->short_name, e->e_name_len);
		ಆದರೆ (x->ea_ino) {
			e->e_ಮೌಲ್ಯ_offs = 0;
		} ಇಲ್ಲದಿದ್ದರೆ {
			ಕೊನೆ -= ಮೌಲ್ಯ_ಗಾತ್ರ;
			e->e_ಮೌಲ್ಯ_offs = ಕೊನೆ - (ಅಕ್ಷರ *) entries_ಆರಂಭ +
						ಮೌಲ್ಯ_ಸ್ಥಾನಾಂತರ_correction;
			ಸ್ಮರಣೆ_ನಕಲು(ಕೊನೆ, x->ಮೌಲ್ಯ, e->e_ಮೌಲ್ಯ_ಗಾತ್ರ);
		}

		ಆದರೆ (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : ಕೊನೆ,
							  &e->e_hash);
			ಆದರೆ (err)
				ಹಿಂತಿರುಗಿ err;
		} ಇಲ್ಲದಿದ್ದರೆ
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_ಮುಂದಿನ(e);
		*(__u32 *)e = 0;
	}
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_xattrs_write(ರಚನೆ ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ inode_ಗಾತ್ರ = EXT2_INODE_ಗಾತ್ರ(fs->super);
	ರಚನೆ ext2_inode_large *inode;
	ಅಕ್ಷರ *ಆರಂಭ, *ಖಂಡ_buf = ಖಾಲಿ;
	ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *ಶೀರ್ಷಿಕೆ;
	__u32 ea_inode_magic;
	blk64_t blk;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ storage_ಗಾತ್ರ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_ಗಾತ್ರ;
	ಆದರೆ (i < ಗಾತ್ರ(*inode))
		i = ಗಾತ್ರ(*inode);
	err = ext2fs_get_memzero(i, &inode);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ err;

	err = ext2fs_read_inode_ತುಂಬಿದ(fs, handle->ino, EXT2_INODE(inode),
				     inode_ಗಾತ್ರ);
	ಆದರೆ (err)
		ಹೋಗು ಹೊರಹೋಗು;

	/* If extra_isize isn't set, we need to set it now */
	ಆದರೆ (inode->i_extra_isize == 0 &&
	    inode_ಗಾತ್ರ > EXT2_GOOD_OLD_INODE_ಗಾತ್ರ) {
		ಅಕ್ಷರ *p = (ಅಕ್ಷರ *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		ಆದರೆ (extra == 0)
			extra = ಗಾತ್ರ(__u32);
		memset(p + EXT2_GOOD_OLD_INODE_ಗಾತ್ರ, 0, extra);
		inode->i_extra_isize = extra;
	}
	ಆದರೆ (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/* Does the inode have space for EA? */
	ಆದರೆ (inode->i_extra_isize < ಗಾತ್ರ(inode->i_extra_isize) ||
	    inode_ಗಾತ್ರ <= EXT2_GOOD_OLD_INODE_ಗಾತ್ರ + inode->i_extra_isize +
								ಗಾತ್ರ(__u32))
		ಹೋಗು write_ea_ಖಂಡ;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	ಸ್ಮರಣೆ_ನಕಲು(((ಅಕ್ಷರ *) inode) + EXT2_GOOD_OLD_INODE_ಗಾತ್ರ +
	       inode->i_extra_isize, &ea_inode_magic, ಗಾತ್ರ(__u32));
	storage_ಗಾತ್ರ = inode_ಗಾತ್ರ - EXT2_GOOD_OLD_INODE_ಗಾತ್ರ -
				inode->i_extra_isize - ಗಾತ್ರ(__u32);
	ಆರಂಭ = ((ಅಕ್ಷರ *) inode) + EXT2_GOOD_OLD_INODE_ಗಾತ್ರ +
				inode->i_extra_isize + ಗಾತ್ರ(__u32);

	err = write_xattrs_ಗಮ್ಯ_ಬಫರ್(fs, handle->attrs, handle->ibody_ಎಣಿಕೆ,
				     ಆರಂಭ, storage_ಗಾತ್ರ, 0, 0);
	ಆದರೆ (err)
		ಹೋಗು ಹೊರಹೋಗು;
write_ea_ಖಂಡ:
	/* Are we done? */
	ಆದರೆ (handle->ibody_ಎಣಿಕೆ == handle->ಎಣಿಕೆ_2 &&
	    !ext2fs_file_acl_ಖಂಡ(fs, EXT2_INODE(inode)))
		ಹೋಗು skip_ea_ಖಂಡ;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &ಖಂಡ_buf);
	ಆದರೆ (err)
		ಹೋಗು ಹೊರಹೋಗು;

	storage_ಗಾತ್ರ = fs->blocksize - ಗಾತ್ರ(ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ);
	ಆರಂಭ = ಖಂಡ_buf + ಗಾತ್ರ(ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ);

	err = write_xattrs_ಗಮ್ಯ_ಬಫರ್(fs, handle->attrs + handle->ibody_ಎಣಿಕೆ,
				     handle->ಎಣಿಕೆ_2 - handle->ibody_ಎಣಿಕೆ, ಆರಂಭ,
				     storage_ಗಾತ್ರ, ಆರಂಭ - ಖಂಡ_buf, 1);
	ಆದರೆ (err)
		ಹೋಗು out2;

	/* Write a header on the EA block */
	ಶೀರ್ಷಿಕೆ = (ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *) ಖಂಡ_buf;
	ಶೀರ್ಷಿಕೆ->h_magic = EXT2_EXT_ATTR_MAGIC;
	ಶೀರ್ಷಿಕೆ->h_refcount = 1;
	ಶೀರ್ಷಿಕೆ->h_ಖಂಡಗಳು = 1;

	/* Get a new block for writing */
	err = prep_ea_ಖಂಡ_for_write(fs, handle->ino, inode);
	ಆದರೆ (err)
		ಹೋಗು out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_ಖಂಡ(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, ಖಂಡ_buf, handle->ino);
	ಆದರೆ (err)
		ಹೋಗು out2;

skip_ea_ಖಂಡ:
	blk = ext2fs_file_acl_ಖಂಡ(fs, (ರಚನೆ ext2_inode *)inode);
	ಆದರೆ (!ಖಂಡ_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;
	}

	/* Write the inode */
	err = ext2fs_write_inode_ತುಂಬಿದ(fs, handle->ino, EXT2_INODE(inode),
				      inode_ಗಾತ್ರ);
	ಆದರೆ (err)
		ಹೋಗು out2;

out2:
	ext2fs_free_mem(&ಖಂಡ_buf);
ಹೊರಹೋಗು:
	ext2fs_free_mem(&inode);
	ಹಿಂತಿರುಗಿ err;
}

ಸ್ಥಿರ errcode_t read_xattrs_from_ಬಫರ್(ರಚನೆ ext2_xattr_handle *handle,
					 ರಚನೆ ext2_inode_large *inode,
					 ರಚನೆ ext2_ext_attr_ನಮೂದು *entries,
					 ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ storage_ಗಾತ್ರ,
					 ಅಕ್ಷರ *ಮೌಲ್ಯ_ಆರಂಭ)
{
	ರಚನೆ ext2_xattr *x;
	ರಚನೆ ext2_ext_attr_ನಮೂದು *ನಮೂದು, *ಕೊನೆ;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಪೂರ್ವಪ್ರತ್ಯಯ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ remain, ಪೂರ್ವಪ್ರತ್ಯಯ_len;
	errcode_t err;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯಗಳು_ಗಾತ್ರ = storage_ಗಾತ್ರ +
			((ಅಕ್ಷರ *)entries - ಮೌಲ್ಯ_ಆರಂಭ);

	/* find the end */
	ಕೊನೆ = entries;
	remain = storage_ಗಾತ್ರ;
	ಆಗ (remain >= ಗಾತ್ರ(ರಚನೆ ext2_ext_attr_ನಮೂದು) &&
	       !EXT2_EXT_IS_ಕೊನೆಯ_ನಮೂದು(ಕೊನೆ)) {

		/* header eats this space */
		remain -= ಗಾತ್ರ(ರಚನೆ ext2_ext_attr_ನಮೂದು);

		/* is attribute name valid? */
		ಆದರೆ (EXT2_EXT_ATTR_ಗಾತ್ರ(ಕೊನೆ->e_name_len) > remain)
			ಹಿಂತಿರುಗಿ EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_ಗಾತ್ರ(ಕೊನೆ->e_name_len);
		ಕೊನೆ = EXT2_EXT_ATTR_ಮುಂದಿನ(ಕೊನೆ);
	}

	ನಮೂದು = entries;
	remain = storage_ಗಾತ್ರ;
	ಆಗ (remain >= ಗಾತ್ರ(ರಚನೆ ext2_ext_attr_ನಮೂದು) &&
	       !EXT2_EXT_IS_ಕೊನೆಯ_ನಮೂದು(ನಮೂದು)) {

		/* Allocate space for more attrs? */
		ಆದರೆ (handle->ಎಣಿಕೆ_2 == handle->ಸಾಮರ್ಥ್ಯ) {
			err = ext2fs_xattrs_expand(handle, 4);
			ಆದರೆ (err)
				ಹಿಂತಿರುಗಿ err;
		}

		x = handle->attrs + handle->ಎಣಿಕೆ_2;

		/* header eats this space */
		remain -= ಗಾತ್ರ(ರಚನೆ ext2_ext_attr_ನಮೂದು);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_ಗಾತ್ರ(ನಮೂದು->e_name_len);

		/* Extract name */
		ಪೂರ್ವಪ್ರತ್ಯಯ = ಕಂಡುಹಿಡಿ_ea_ಪೂರ್ವಪ್ರತ್ಯಯ(ನಮೂದು->e_name_ಸೂಚ್ಯಂಕ);
		ಪೂರ್ವಪ್ರತ್ಯಯ_len = (ಪೂರ್ವಪ್ರತ್ಯಯ ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಪೂರ್ವಪ್ರತ್ಯಯ) : 0);
		err = ext2fs_get_memzero(ನಮೂದು->e_name_len + ಪೂರ್ವಪ್ರತ್ಯಯ_len + 1,
					 &x->name);
		ಆದರೆ (err)
			ಹಿಂತಿರುಗಿ err;
		ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ)
			ಸ್ಮರಣೆ_ನಕಲು(x->name, ಪೂರ್ವಪ್ರತ್ಯಯ, ಪೂರ್ವಪ್ರತ್ಯಯ_len);
		ಆದರೆ (ನಮೂದು->e_name_len)
			ಸ್ಮರಣೆ_ನಕಲು(x->name + ಪೂರ್ವಪ್ರತ್ಯಯ_len,
			       (ಅಕ್ಷರ *)ನಮೂದು + ಗಾತ್ರ(*ನಮೂದು),
			       ನಮೂದು->e_name_len);
		x->short_name = x->name + ಪೂರ್ವಪ್ರತ್ಯಯ_len;
		x->name_ಸೂಚ್ಯಂಕ = ನಮೂದು->e_name_ಸೂಚ್ಯಂಕ;

		/* Check & copy value */
		ಆದರೆ (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    ನಮೂದು->e_ಮೌಲ್ಯ_inum != 0)
			ಹಿಂತಿರುಗಿ EXT2_ET_BAD_EA_ಖಂಡ_ಅಂಕೆ;

		ಆದರೆ (ನಮೂದು->e_ಮೌಲ್ಯ_inum == 0) {
			ಆದರೆ (ನಮೂದು->e_ಮೌಲ್ಯ_ಗಾತ್ರ > remain)
				ಹಿಂತಿರುಗಿ EXT2_ET_EA_BAD_ಮೌಲ್ಯ_ಗಾತ್ರ;

			ಆದರೆ (ನಮೂದು->e_ಮೌಲ್ಯ_offs + ನಮೂದು->e_ಮೌಲ್ಯ_ಗಾತ್ರ > ಮೌಲ್ಯಗಳು_ಗಾತ್ರ)
				ಹಿಂತಿರುಗಿ EXT2_ET_EA_BAD_ಮೌಲ್ಯ_ಸ್ಥಾನಾಂತರ;

			ಆದರೆ (ನಮೂದು->e_ಮೌಲ್ಯ_ಗಾತ್ರ > 0 &&
			    ಮೌಲ್ಯ_ಆರಂಭ + ನಮೂದು->e_ಮೌಲ್ಯ_offs <
			    (ಅಕ್ಷರ *)ಕೊನೆ + ಗಾತ್ರ(__u32))
				ಹಿಂತಿರುಗಿ EXT2_ET_EA_BAD_ಮೌಲ್ಯ_ಸ್ಥಾನಾಂತರ;

			remain -= ನಮೂದು->e_ಮೌಲ್ಯ_ಗಾತ್ರ;

			err = ext2fs_get_mem(ನಮೂದು->e_ಮೌಲ್ಯ_ಗಾತ್ರ, &x->ಮೌಲ್ಯ);
			ಆದರೆ (err)
				ಹಿಂತಿರುಗಿ err;
			ಸ್ಮರಣೆ_ನಕಲು(x->ಮೌಲ್ಯ, ಮೌಲ್ಯ_ಆರಂಭ + ನಮೂದು->e_ಮೌಲ್ಯ_offs,
			       ನಮೂದು->e_ಮೌಲ್ಯ_ಗಾತ್ರ);
		} ಇಲ್ಲದಿದ್ದರೆ {
			ರಚನೆ ext2_inode *ea_inode;
			ext2_file_t ea_file;

			ಆದರೆ (ನಮೂದು->e_ಮೌಲ್ಯ_offs != 0)
				ಹಿಂತಿರುಗಿ EXT2_ET_EA_BAD_ಮೌಲ್ಯ_ಸ್ಥಾನಾಂತರ;

			ಆದರೆ (ನಮೂದು->e_ಮೌಲ್ಯ_ಗಾತ್ರ > (64 * 1024))
				ಹಿಂತಿರುಗಿ EXT2_ET_EA_BAD_ಮೌಲ್ಯ_ಗಾತ್ರ;

			err = ext2fs_get_mem(ನಮೂದು->e_ಮೌಲ್ಯ_ಗಾತ್ರ, &x->ಮೌಲ್ಯ);
			ಆದರೆ (err)
				ಹಿಂತಿರುಗಿ err;

			err = ext2fs_file_open(handle->fs, ನಮೂದು->e_ಮೌಲ್ಯ_inum,
					       0, &ea_file);
			ಆದರೆ (err)
				ಹಿಂತಿರುಗಿ err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			ಆದರೆ ((ea_inode->i_flags & EXT4_INLINE_ದತ್ತಾಂಶ_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_ಎಣಿಕೆ == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((__u64) ext2fs_file_get_ಗಾತ್ರ(ea_file) !=
				 ನಮೂದು->e_ಮೌಲ್ಯ_ಗಾತ್ರ)
				err = EXT2_ET_EA_BAD_ಮೌಲ್ಯ_ಗಾತ್ರ;
			ಇಲ್ಲದಿದ್ದರೆ
				err = ext2fs_file_read(ea_file, x->ಮೌಲ್ಯ,
						       ನಮೂದು->e_ಮೌಲ್ಯ_ಗಾತ್ರ, 0);
			ext2fs_file_close(ea_file);
			ಆದರೆ (err)
				ಹಿಂತಿರುಗಿ err;
		}

		x->ea_ino = ನಮೂದು->e_ಮೌಲ್ಯ_inum;
		x->ಮೌಲ್ಯ_len = ನಮೂದು->e_ಮೌಲ್ಯ_ಗಾತ್ರ;

		/* e_hash may be 0 in older inode's ea */
		ಆದರೆ (ನಮೂದು->e_hash != 0) {
			__u32 hash, signed_hash;

			ಶೂನ್ಯ *ದತ್ತಾಂಶ = (ನಮೂದು->e_ಮೌಲ್ಯ_inum != 0) ?
					0 : ಮೌಲ್ಯ_ಆರಂಭ + ನಮೂದು->e_ಮೌಲ್ಯ_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, ನಮೂದು,
							  ದತ್ತಾಂಶ, &hash,
							  &signed_hash);
			ಆದರೆ (err)
				ಹಿಂತಿರುಗಿ err;
			ಆದರೆ ((ನಮೂದು->e_hash != hash) &&
			    (ನಮೂದು->e_hash != signed_hash)) {
				ರಚನೆ ext2_inode ಮಗು;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							ನಮೂದು->e_ಮೌಲ್ಯ_inum,
							&ಮಗು);
				ಆದರೆ (err)
					ಹಿಂತಿರುಗಿ err;
				ಆದರೆ (ಮಗು.i_mtime != handle->ino ||
				    ಮಗು.i_generation != inode->i_generation)
					ಹಿಂತಿರುಗಿ EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->ಎಣಿಕೆ_2++;
		ನಮೂದು = EXT2_EXT_ATTR_ಮುಂದಿನ(ನಮೂದು);
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ xattrs_free_ಕೀಲಿಗಳು(ರಚನೆ ext2_xattr_handle *h)
{
	ರಚನೆ ext2_xattr *a = h->attrs;
	ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; i < h->ಸಾಮರ್ಥ್ಯ; i++) {
		ಆದರೆ (a[i].name)
			ext2fs_free_mem(&a[i].name);
		ಆದರೆ (a[i].ಮೌಲ್ಯ)
			ext2fs_free_mem(&a[i].ಮೌಲ್ಯ);
	}
	h->ಎಣಿಕೆ_2 = 0;
	h->ibody_ಎಣಿಕೆ = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(ರಚನೆ ext2_xattr_handle *handle,
				   ರಚನೆ ext2_inode_large *inode)
{
	ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *ಶೀರ್ಷಿಕೆ;
	__u32 ea_inode_magic;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ storage_ಗಾತ್ರ;
	ಅಕ್ಷರ *ಆರಂಭ, *ಖಂಡ_buf = ಖಾಲಿ;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_ಕೀಲಿಗಳು(handle);

	/* Does the inode have space for EA? */
	ಆದರೆ (inode->i_extra_isize < ಗಾತ್ರ(inode->i_extra_isize) ||
	    EXT2_INODE_ಗಾತ್ರ(handle->fs->super) <= EXT2_GOOD_OLD_INODE_ಗಾತ್ರ +
						  inode->i_extra_isize +
						  ಗಾತ್ರ(__u32))
		ಹೋಗು read_ea_ಖಂಡ;
	ಆದರೆ (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	/* Look for EA in the inode */
	ಸ್ಮರಣೆ_ನಕಲು(&ea_inode_magic, ((ಅಕ್ಷರ *) inode) + EXT2_GOOD_OLD_INODE_ಗಾತ್ರ +
	       inode->i_extra_isize, ಗಾತ್ರ(__u32));
	ಆದರೆ (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_ಗಾತ್ರ = EXT2_INODE_ಗಾತ್ರ(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_ಗಾತ್ರ - inode->i_extra_isize -
			ಗಾತ್ರ(__u32);
		ಆರಂಭ = ((ಅಕ್ಷರ *) inode) + EXT2_GOOD_OLD_INODE_ಗಾತ್ರ +
			inode->i_extra_isize + ಗಾತ್ರ(__u32);

		err = read_xattrs_from_ಬಫರ್(handle, inode,
					(ರಚನೆ ext2_ext_attr_ನಮೂದು *) ಆರಂಭ,
					storage_ಗಾತ್ರ, ಆರಂಭ);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;

		handle->ibody_ಎಣಿಕೆ = handle->ಎಣಿಕೆ_2;
	}

read_ea_ಖಂಡ:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_ಖಂಡ(handle->fs, EXT2_INODE(inode));
	ಆದರೆ (blk != 0) {
		ಆದರೆ ((blk < handle->fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ) ||
		    (blk >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_ಖಂಡ_ಅಂಕೆ;
			ಹೋಗು ಹೊರಹೋಗು;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &ಖಂಡ_buf);
		ಆದರೆ (err)
			ಹೋಗು ಹೊರಹೋಗು;

		err = ext2fs_read_ext_attr3(handle->fs, blk, ಖಂಡ_buf,
					    handle->ino);
		ಆದರೆ (err)
			ಹೋಗು out3;

		/* We only know how to deal with v2 EA blocks */
		ಶೀರ್ಷಿಕೆ = (ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ *) ಖಂಡ_buf;
		ಆದರೆ (ಶೀರ್ಷಿಕೆ->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_ಶೀರ್ಷಿಕೆ;
			ಹೋಗು out3;
		}

		/* Read EAs */
		storage_ಗಾತ್ರ = handle->fs->blocksize -
			ಗಾತ್ರ(ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ);
		ಆರಂಭ = ಖಂಡ_buf + ಗಾತ್ರ(ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ);
		err = read_xattrs_from_ಬಫರ್(handle, inode,
					(ರಚನೆ ext2_ext_attr_ನಮೂದು *) ಆರಂಭ,
					storage_ಗಾತ್ರ, ಖಂಡ_buf);
	}

out3:
	ಆದರೆ (ಖಂಡ_buf)
		ext2fs_free_mem(&ಖಂಡ_buf);
ಹೊರಹೋಗು:
	ಹಿಂತಿರುಗಿ err;
}

errcode_t ext2fs_xattrs_read(ರಚನೆ ext2_xattr_handle *handle)
{
	ರಚನೆ ext2_inode_large *inode;
	size_t inode_ಗಾತ್ರ = EXT2_INODE_ಗಾತ್ರ(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	ಆದರೆ (inode_ಗಾತ್ರ < ಗಾತ್ರ(*inode))
		inode_ಗಾತ್ರ = ಗಾತ್ರ(*inode);
	err = ext2fs_get_memzero(inode_ಗಾತ್ರ, &inode);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ err;

	err = ext2fs_read_inode_ತುಂಬಿದ(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_ಗಾತ್ರ(handle->fs->super));
	ಆದರೆ (err)
		ಹೋಗು ಹೊರಹೋಗು;

	err = ext2fs_xattrs_read_inode(handle, inode);

ಹೊರಹೋಗು:
	ext2fs_free_mem(&inode);

	ಹಿಂತಿರುಗಿ err;
}

errcode_t ext2fs_xattrs_iterate(ರಚನೆ ext2_xattr_handle *h,
				ಪೂರ್ಣಾಂಕ (*func)(ಅಕ್ಷರ *name, ಅಕ್ಷರ *ಮೌಲ್ಯ,
					    size_t ಮೌಲ್ಯ_len, ಶೂನ್ಯ *ದತ್ತಾಂಶ),
				ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ ext2_xattr *x;
	ಪೂರ್ಣಾಂಕ dirty = 0;
	ಪೂರ್ಣಾಂಕ ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	ಗಾಗಿ (x = h->attrs; x < h->attrs + h->ಎಣಿಕೆ_2; x++) {
		ret = func(x->name, x->ಮೌಲ್ಯ, x->ಮೌಲ್ಯ_len, ದತ್ತಾಂಶ);
		ಆದರೆ (ret & XATTR_ಬದಲಾಯಿತು)
			dirty = 1;
		ಆದರೆ (ret & XATTR_ABORT)
			ನಿಲ್ಲಿಸು;
	}

	ಆದರೆ (dirty)
		ಹಿಂತಿರುಗಿ ext2fs_xattrs_write(h);
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_xattr_get(ರಚನೆ ext2_xattr_handle *h, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ,
			   ಶೂನ್ಯ **ಮೌಲ್ಯ, size_t *ಮೌಲ್ಯ_len)
{
	ರಚನೆ ext2_xattr *x;
	ಅಕ್ಷರ *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	ಗಾಗಿ (x = h->attrs; x < h->attrs + h->ಎಣಿಕೆ_2; x++) {
		ಆದರೆ (strcmp(x->name, ಕೀಲಿ))
			ಮುಂದುವರಿ;

		ಆದರೆ (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(ಕೀಲಿ, "system.posix_acl_default") == 0) ||
		     (strcmp(ಕೀಲಿ, "system.posix_acl_access") == 0))) {
			err = convert_disk_ಬಫರ್_ಗಮ್ಯ_posix_acl(x->ಮೌಲ್ಯ, x->ಮೌಲ್ಯ_len,
							       ಮೌಲ್ಯ, ಮೌಲ್ಯ_len);
			ಹಿಂತಿರುಗಿ err;
		} ಇಲ್ಲದಿದ್ದರೆ {
			err = ext2fs_get_mem(x->ಮೌಲ್ಯ_len, &val);
			ಆದರೆ (err)
				ಹಿಂತಿರುಗಿ err;
			ಸ್ಮರಣೆ_ನಕಲು(val, x->ಮೌಲ್ಯ, x->ಮೌಲ್ಯ_len);
			*ಮೌಲ್ಯ = val;
			*ಮೌಲ್ಯ_len = x->ಮೌಲ್ಯ_len;
			ಹಿಂತಿರುಗಿ 0;
		}
	}

	ಹಿಂತಿರುಗಿ EXT2_ET_EA_ಕೀಲಿ_NOT_ಸಿಕ್ಕಿತು;
}

errcode_t ext2fs_xattr_inode_max_ಗಾತ್ರ(ext2_filsys fs, ext2_ino_t ino,
				      size_t *ಗಾತ್ರ_2)
{
	ರಚನೆ ext2_ext_attr_ನಮೂದು *ನಮೂದು;
	ರಚನೆ ext2_inode_large *inode;
	__u32 ea_inode_magic;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ minoff;
	ಅಕ್ಷರ *ಆರಂಭ;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_ಗಾತ್ರ(fs->super);
	ಆದರೆ (i < ಗಾತ್ರ(*inode))
		i = ಗಾತ್ರ(*inode);
	err = ext2fs_get_memzero(i, &inode);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ err;

	err = ext2fs_read_inode_ತುಂಬಿದ(fs, ino, (ರಚನೆ ext2_inode *)inode,
				     EXT2_INODE_ಗಾತ್ರ(fs->super));
	ಆದರೆ (err)
		ಹೋಗು ಹೊರಹೋಗು;

	/* Does the inode have size for EA? */
	ಆದರೆ (EXT2_INODE_ಗಾತ್ರ(fs->super) <= EXT2_GOOD_OLD_INODE_ಗಾತ್ರ +
						  inode->i_extra_isize +
						  ಗಾತ್ರ(__u32)) {
		err = EXT2_ET_INLINE_ದತ್ತಾಂಶ_NO_SPACE;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	minoff = EXT2_INODE_ಗಾತ್ರ(fs->super) - ಗಾತ್ರ(*inode) - ಗಾತ್ರ(__u32);
	ಸ್ಮರಣೆ_ನಕಲು(&ea_inode_magic, ((ಅಕ್ಷರ *) inode) + EXT2_GOOD_OLD_INODE_ಗಾತ್ರ +
	       inode->i_extra_isize, ಗಾತ್ರ(__u32));
	ಆದರೆ (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		ಆರಂಭ= ((ಅಕ್ಷರ *) inode) + EXT2_GOOD_OLD_INODE_ಗಾತ್ರ +
			inode->i_extra_isize + ಗಾತ್ರ(__u32);
		ನಮೂದು = (ರಚನೆ ext2_ext_attr_ನಮೂದು *) ಆರಂಭ;
		ಆಗ (!EXT2_EXT_IS_ಕೊನೆಯ_ನಮೂದು(ನಮೂದು)) {
			ಆದರೆ (!ನಮೂದು->e_ಮೌಲ್ಯ_inum && ನಮೂದು->e_ಮೌಲ್ಯ_ಗಾತ್ರ) {
				ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ offs = ನಮೂದು->e_ಮೌಲ್ಯ_offs;
				ಆದರೆ (offs < minoff)
					minoff = offs;
			}
			ನಮೂದು = EXT2_EXT_ATTR_ಮುಂದಿನ(ನಮೂದು);
		}
		*ಗಾತ್ರ_2 = minoff - ((ಅಕ್ಷರ *)ನಮೂದು - (ಅಕ್ಷರ *)ಆರಂಭ) - ಗಾತ್ರ(__u32);
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* no xattr.  return a maximum size */
		*ಗಾತ್ರ_2 = EXT2_EXT_ATTR_ಗಾತ್ರ(minoff -
					   EXT2_EXT_ATTR_LEN(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ("data")) -
					   EXT2_EXT_ATTR_ಸುತ್ತು - ಗಾತ್ರ(__u32));
	}

ಹೊರಹೋಗು:
	ext2fs_free_mem(&inode);
	ಹಿಂತಿರುಗಿ err;
}

ಸ್ಥಿರ errcode_t xattr_create_ea_inode(ext2_filsys fs, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಮೌಲ್ಯ,
				       size_t ಮೌಲ್ಯ_len, ext2_ino_t *ea_ino)
{
	ರಚನೆ ext2_inode inode;
	ext2_ino_t ino;
	ext2_file_t file;
	__u32 hash;
	errcode_t ret;

	ret = ext2fs_new_inode(fs, 0, 0, 0, &ino);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ret;

	memset(&inode, 0, ಗಾತ್ರ(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	ಆದರೆ (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_ಗಾತ್ರ = 0;
	inode.i_ಬಹುಲಕ = LINUX_S_IFREG | 0600;
	inode.i_links_ಎಣಿಕೆ = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ret;
	/*
	 * ref_count and hash utilize inode's i_*time fields.
	 * ext2fs_write_new_inode() call above initializes these fields with
	 * current time. That's why ref count and hash updates are done
	 * separately below.
	 */
	ext2fs_ಗಣ_ea_inode_ref(&inode, 1);
	hash = ext2fs_crc32c_le(fs->csum_ಬೀಜ, ಮೌಲ್ಯ, ಮೌಲ್ಯ_len);
	ext2fs_ಗಣ_ea_inode_hash(&inode, hash);

	ret = ext2fs_write_inode(fs, ino, &inode);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ret;
	ret = ext2fs_file_write(file, ಮೌಲ್ಯ, ಮೌಲ್ಯ_len, ಖಾಲಿ);
	ext2fs_file_close(file);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	ರಚನೆ ext2_inode_large inode;
	__u64 ref_ಎಣಿಕೆ;
	errcode_t ret;

	ret = ext2fs_read_inode_ತುಂಬಿದ(fs, ino, (ರಚನೆ ext2_inode *)&inode,
				     ಗಾತ್ರ(inode));
	ಆದರೆ (ret)
		ಹೋಗು ಹೊರಹೋಗು;

	ref_ಎಣಿಕೆ = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_ಎಣಿಕೆ--;
	ext2fs_ಗಣ_ea_inode_ref(EXT2_INODE(&inode), ref_ಎಣಿಕೆ);

	ಆದರೆ (ref_ಎಣಿಕೆ)
		ಹೋಗು write_ಹೊರಹೋಗು;

	inode.i_links_ಎಣಿಕೆ = 0;
	ext2fs_ಗಣ_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	ಆದರೆ (ret)
		ಹೋಗು write_ಹೊರಹೋಗು;

	ಆದರೆ (ext2fs_inode_has_valid_blocks2(fs, (ರಚನೆ ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (ರಚನೆ ext2_inode *)&inode, ಖಾಲಿ,
				   0, ~0ULL);
		ಆದರೆ (ret)
			ಹೋಗು ಹೊರಹೋಗು;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_ಹೊರಹೋಗು:
	ret = ext2fs_write_inode_ತುಂಬಿದ(fs, ino, (ರಚನೆ ext2_inode *)&inode,
				      ಗಾತ್ರ(inode));
ಹೊರಹೋಗು:
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ errcode_t xattr_update_ನಮೂದು(ext2_filsys fs, ರಚನೆ ext2_xattr *x,
				    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *short_name,
				    ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಮೌಲ್ಯ,
				    size_t ಮೌಲ್ಯ_len, ಪೂರ್ಣಾಂಕ ಒಳ_inode)
{
	ext2_ino_t ea_ino = 0;
	ಶೂನ್ಯ *new_ಮೌಲ್ಯ = ಖಾಲಿ;
	ಅಕ್ಷರ *new_name = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ name_len;
	errcode_t ret;

	ಆದರೆ (!x->name) {
		name_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		ಆದರೆ (ret)
			ಹೋಗು fail;
		ಸ್ಮರಣೆ_ನಕಲು(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(ಮೌಲ್ಯ_len, &new_ಮೌಲ್ಯ);
	ಆದರೆ (ret)
		ಹೋಗು fail;
	ಸ್ಮರಣೆ_ನಕಲು(new_ಮೌಲ್ಯ, ಮೌಲ್ಯ, ಮೌಲ್ಯ_len);

	ಆದರೆ (ಒಳ_inode) {
		ret = xattr_create_ea_inode(fs, ಮೌಲ್ಯ, ಮೌಲ್ಯ_len, &ea_ino);
		ಆದರೆ (ret)
			ಹೋಗು fail;
	}

	ಆದರೆ (x->ea_ino) {
		ret = xattr_inode_dec_ref(fs, x->ea_ino);
		ಆದರೆ (ret)
			ಹೋಗು fail;
	}

	ಆದರೆ (!x->name) {
		x->name = new_name;
		x->short_name = new_name + (short_name  - name);
	}
	x->name_ಸೂಚ್ಯಂಕ = ಸೂಚ್ಯಂಕ;

	ಆದರೆ (x->ಮೌಲ್ಯ)
		ext2fs_free_mem(&x->ಮೌಲ್ಯ);
	x->ಮೌಲ್ಯ = new_ಮೌಲ್ಯ;
	x->ಮೌಲ್ಯ_len = ಮೌಲ್ಯ_len;
	x->ea_ino = ea_ino;
	ಹಿಂತಿರುಗಿ 0;
fail:
	ಆದರೆ (new_name)
		ext2fs_free_mem(&new_name);
	ಆದರೆ (new_ಮೌಲ್ಯ)
		ext2fs_free_mem(&new_ಮೌಲ್ಯ);
	ಆದರೆ (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ xattr_ಕಂಡುಹಿಡಿ_position(ರಚನೆ ext2_xattr *attrs, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *shortname, ಪೂರ್ಣಾಂಕ name_idx)
{
	ರಚನೆ ext2_xattr *x;
	ಪೂರ್ಣಾಂಕ i;
	ಪೂರ್ಣಾಂಕ shortname_len, x_shortname_len;

	shortname_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(shortname);

	ಗಾಗಿ (i = 0, x = attrs; i < ಎಣಿಕೆ_2; i++, x++) {
		ಆದರೆ (name_idx < x->name_ಸೂಚ್ಯಂಕ)
			ನಿಲ್ಲಿಸು;
		ಆದರೆ (name_idx > x->name_ಸೂಚ್ಯಂಕ)
			ಮುಂದುವರಿ;

		x_shortname_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(x->short_name);
		ಆದರೆ (shortname_len < x_shortname_len)
			ನಿಲ್ಲಿಸು;
		ಆದರೆ (shortname_len > x_shortname_len)
			ಮುಂದುವರಿ;

		ಆದರೆ (memcmp(shortname, x->short_name, shortname_len) <= 0)
			ನಿಲ್ಲಿಸು;
	}
	ಹಿಂತಿರುಗಿ i;
}

ಸ್ಥಿರ errcode_t xattr_array_update(ರಚನೆ ext2_xattr_handle *h,
				    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
				    ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಮೌಲ್ಯ, size_t ಮೌಲ್ಯ_len,
				    ಪೂರ್ಣಾಂಕ ibody_free, ಪೂರ್ಣಾಂಕ ಖಂಡ_free,
				    ಪೂರ್ಣಾಂಕ old_idx, ಪೂರ್ಣಾಂಕ ಒಳ_inode)
{
	ರಚನೆ ext2_xattr tmp;
	ಪೂರ್ಣಾಂಕ ಕೂಡಿಸು_ಗಮ್ಯ_ibody;
	ಪೂರ್ಣಾಂಕ needed;
	ಪೂರ್ಣಾಂಕ name_len, name_idx = 0;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *shortname = name;
	ಪೂರ್ಣಾಂಕ new_idx;
	ಪೂರ್ಣಾಂಕ ret;

	ಕಂಡುಹಿಡಿ_ea_ಸೂಚ್ಯಂಕ(name, &shortname, &name_idx);
	name_len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	ಆದರೆ (!ಒಳ_inode)
		needed += EXT2_EXT_ATTR_ಗಾತ್ರ(ಮೌಲ್ಯ_len);

	ಆದರೆ (old_idx >= 0 && old_idx < h->ibody_ಎಣಿಕೆ) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		ಆದರೆ (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_ಗಾತ್ರ(
						h->attrs[old_idx].ಮೌಲ್ಯ_len);
	}

	ಆದರೆ (needed <= ibody_free) {
		ಆದರೆ (old_idx < 0) {
			new_idx = h->ibody_ಎಣಿಕೆ;
			ಕೂಡಿಸು_ಗಮ್ಯ_ibody = 1;
			ಹೋಗು ಕೂಡಿಸು_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_ನಮೂದು(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, ಮೌಲ್ಯ,
					 ಮೌಲ್ಯ_len, ಒಳ_inode);
		ಆದರೆ (ret)
			ಹಿಂತಿರುಗಿ ret;
		ಆದರೆ (h->ibody_ಎಣಿಕೆ <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_ಎಣಿಕೆ + 1,
				h->attrs + h->ibody_ಎಣಿಕೆ,
				(old_idx - h->ibody_ಎಣಿಕೆ) * ಗಾತ್ರ(*h->attrs));
			h->attrs[h->ibody_ಎಣಿಕೆ] = tmp;
			h->ibody_ಎಣಿಕೆ++;
		}
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (h->ibody_ಎಣಿಕೆ <= old_idx) {
		ಖಂಡ_free += EXT2_EXT_ATTR_LEN(name_len);
		ಆದರೆ (!h->attrs[old_idx].ea_ino)
			ಖಂಡ_free +=
				EXT2_EXT_ATTR_ಗಾತ್ರ(h->attrs[old_idx].ಮೌಲ್ಯ_len);
	}

	ಆದರೆ (needed > ಖಂಡ_free)
		ಹಿಂತಿರುಗಿ EXT2_ET_EA_NO_SPACE;

	ಆದರೆ (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_ನಮೂದು(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, ಮೌಲ್ಯ,
					 ಮೌಲ್ಯ_len, ಒಳ_inode);
		ಆದರೆ (ret)
			ಹಿಂತಿರುಗಿ ret;
		ಆದರೆ (old_idx < h->ibody_ಎಣಿಕೆ) {
			/*
			 * Move entry from ibody to the block. Note that
			 * entries in the block are sorted.
			 */
			new_idx = xattr_ಕಂಡುಹಿಡಿ_position(h->attrs + h->ibody_ಎಣಿಕೆ,
						      h->ಎಣಿಕೆ_2 - h->ibody_ಎಣಿಕೆ,
						      shortname, name_idx);
			new_idx += h->ibody_ಎಣಿಕೆ - 1;
			tmp = h->attrs[old_idx];
			memmove(h->attrs + old_idx, h->attrs + old_idx + 1,
				(new_idx - old_idx) * ಗಾತ್ರ(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_ಎಣಿಕೆ--;
		}
		ಹಿಂತಿರುಗಿ 0;
	}

	new_idx = xattr_ಕಂಡುಹಿಡಿ_position(h->attrs + h->ibody_ಎಣಿಕೆ,
				      h->ಎಣಿಕೆ_2 - h->ibody_ಎಣಿಕೆ,
				      shortname, name_idx);
	new_idx += h->ibody_ಎಣಿಕೆ;
	ಕೂಡಿಸು_ಗಮ್ಯ_ibody = 0;

ಕೂಡಿಸು_new:
	ಆದರೆ (h->ಎಣಿಕೆ_2 == h->ಸಾಮರ್ಥ್ಯ) {
		ret = ext2fs_xattrs_expand(h, 4);
		ಆದರೆ (ret)
			ಹಿಂತಿರುಗಿ ret;
	}

	ret = xattr_update_ನಮೂದು(h->fs, &h->attrs[h->ಎಣಿಕೆ_2], name, shortname,
				 name_idx, ಮೌಲ್ಯ, ಮೌಲ್ಯ_len, ಒಳ_inode);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ret;

	tmp = h->attrs[h->ಎಣಿಕೆ_2];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->ಎಣಿಕೆ_2 - new_idx)*ಗಾತ್ರ(*h->attrs));
	h->attrs[new_idx] = tmp;
	ಆದರೆ (ಕೂಡಿಸು_ಗಮ್ಯ_ibody)
		h->ibody_ಎಣಿಕೆ++;
	h->ಎಣಿಕೆ_2++;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ space_ಬಳಸಿದ(ರಚನೆ ext2_xattr *attrs, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2)
{
	ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = 0;
	ರಚನೆ ext2_xattr *x;
	ಪೂರ್ಣಾಂಕ i, len;

	ಗಾಗಿ (i = 0, x = attrs; i < ಎಣಿಕೆ_2; i++, x++) {
		len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(x->short_name);
		ಒಟ್ಟು += EXT2_EXT_ATTR_LEN(len);
		ಆದರೆ (!x->ea_ino)
			ಒಟ್ಟು += EXT2_EXT_ATTR_ಗಾತ್ರ(x->ಮೌಲ್ಯ_len);
	}
	ಹಿಂತಿರುಗಿ ಒಟ್ಟು;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EXT4_XATTR_MIN_LARGE_EA_ಗಾತ್ರ(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - ಗಾತ್ರ(ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ) - 4)

errcode_t ext2fs_xattr_ಗಣ(ರಚನೆ ext2_xattr_handle *h,
			   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
			   ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ಮೌಲ್ಯ,
			   size_t ಮೌಲ್ಯ_len)
{
	ext2_filsys fs = h->fs;
	ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ inode_ಗಾತ್ರ = EXT2_INODE_ಗಾತ್ರ(fs->super);
	ರಚನೆ ext2_inode_large *inode = ಖಾಲಿ;
	ರಚನೆ ext2_xattr *x;
	ಅಕ್ಷರ *new_ಮೌಲ್ಯ;
	ಪೂರ್ಣಾಂಕ ibody_free, ಖಂಡ_free;
	ಪೂರ್ಣಾಂಕ ಒಳ_inode = 0;
	ಪೂರ್ಣಾಂಕ old_idx = -1;
	ಪೂರ್ಣಾಂಕ extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(ಮೌಲ್ಯ_len, &new_ಮೌಲ್ಯ);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ret;
	ಆದರೆ (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((strcmp(name, "system.posix_acl_default") == 0) ||
	     (strcmp(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_ಗಮ್ಯ_disk_ಬಫರ್(ಮೌಲ್ಯ, ಮೌಲ್ಯ_len,
						       new_ಮೌಲ್ಯ, &ಮೌಲ್ಯ_len);
		ಆದರೆ (ret)
			ಹೋಗು ಹೊರಹೋಗು;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಮೌಲ್ಯ_len)
		ಸ್ಮರಣೆ_ನಕಲು(new_ಮೌಲ್ಯ, ಮೌಲ್ಯ, ಮೌಲ್ಯ_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	ಗಾಗಿ (x = h->attrs; x < h->attrs + h->ಎಣಿಕೆ_2; x++) {
		ಆದರೆ (!strcmp(x->name, name)) {
			ಆದರೆ (!x->ea_ino && x->ಮೌಲ್ಯ_len == ಮೌಲ್ಯ_len &&
			    (!ಮೌಲ್ಯ_len ||
			     !memcmp(x->ಮೌಲ್ಯ, new_ಮೌಲ್ಯ, ಮೌಲ್ಯ_len))) {
				ret = 0;
				ಹೋಗು ಹೊರಹೋಗು;
			}
			old_idx = x - h->attrs;
			ನಿಲ್ಲಿಸು;
		}
	}

	ret = ext2fs_get_memzero(inode_ಗಾತ್ರ, &inode);
	ಆದರೆ (ret)
		ಹೋಗು ಹೊರಹೋಗು;
	ret = ext2fs_read_inode_ತುಂಬಿದ(fs, h->ino,
				     (ರಚನೆ ext2_inode *)inode,
				     inode_ಗಾತ್ರ);
	ಆದರೆ (ret)
		ಹೋಗು ಹೊರಹೋಗು;
	ಆದರೆ (inode_ಗಾತ್ರ > EXT2_GOOD_OLD_INODE_ಗಾತ್ರ) {
		extra_isize = inode->i_extra_isize;
		ಆದರೆ (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			ಆದರೆ (extra_isize == 0)
				extra_isize = ಗಾತ್ರ(__u32);
		}
		ibody_free = inode_ಗಾತ್ರ - EXT2_GOOD_OLD_INODE_ಗಾತ್ರ;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= ಗಾತ್ರ(__u32) * 2;
		ibody_free -= space_ಬಳಸಿದ(h->attrs, h->ibody_ಎಣಿಕೆ);
	} ಇಲ್ಲದಿದ್ದರೆ
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	ಆದರೆ (strcmp(name, "system.data") == 0) {
		ಆದರೆ (h->ibody_ಎಣಿಕೆ <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			ಹೋಗು ಹೊರಹೋಗು;
		}
		ret = xattr_array_update(h, name, new_ಮೌಲ್ಯ, ಮೌಲ್ಯ_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		ಆದರೆ (ret)
			ಹೋಗು ಹೊರಹೋಗು;
		ಹೋಗು write_ಹೊರಹೋಗು;
	}

	ಖಂಡ_free = fs->blocksize;
	ಖಂಡ_free -= ಗಾತ್ರ(ರಚನೆ ext2_ext_attr_ಶೀರ್ಷಿಕೆ);
	/* Final null entry. */
	ಖಂಡ_free -= ಗಾತ್ರ(__u32);
	ಖಂಡ_free -= space_ಬಳಸಿದ(h->attrs + h->ibody_ಎಣಿಕೆ,
				 h->ಎಣಿಕೆ_2 - h->ibody_ಎಣಿಕೆ);

	ಆದರೆ (ext2fs_has_feature_ea_inode(fs->super) &&
	    ಮೌಲ್ಯ_len > EXT4_XATTR_MIN_LARGE_EA_ಗಾತ್ರ(fs->blocksize))
		ಒಳ_inode = 1;

	ret = xattr_array_update(h, name, new_ಮೌಲ್ಯ, ಮೌಲ್ಯ_len, ibody_free,
				 ಖಂಡ_free, old_idx, ಒಳ_inode);
	ಆದರೆ (ret == EXT2_ET_EA_NO_SPACE && !ಒಳ_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_ಮೌಲ್ಯ, ಮೌಲ್ಯ_len,
			ibody_free, ಖಂಡ_free, old_idx, 1 /* in_inode */);
	ಆದರೆ (ret)
		ಹೋಗು ಹೊರಹೋಗು;

write_ಹೊರಹೋಗು:
	ret = ext2fs_xattrs_write(h);
ಹೊರಹೋಗು:
	ಆದರೆ (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_ಮೌಲ್ಯ);
	ಹಿಂತಿರುಗಿ ret;
}

errcode_t ext2fs_xattr_remove(ರಚನೆ ext2_xattr_handle *handle,
			      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಕೀಲಿ)
{
	ರಚನೆ ext2_xattr *x;
	ರಚನೆ ext2_xattr *ಕೊನೆ = handle->attrs + handle->ಎಣಿಕೆ_2;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	ಗಾಗಿ (x = handle->attrs; x < ಕೊನೆ; x++) {
		ಆದರೆ (strcmp(x->name, ಕೀಲಿ) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->ಮೌಲ್ಯ);
			ಆದರೆ (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (ಕೊನೆ - x - 1)*ಗಾತ್ರ(*x));
			memset(ಕೊನೆ - 1, 0, ಗಾತ್ರ(*ಕೊನೆ));
			ಆದರೆ (x < handle->attrs + handle->ibody_ಎಣಿಕೆ)
				handle->ibody_ಎಣಿಕೆ--;
			handle->ಎಣಿಕೆ_2--;
			ಹಿಂತಿರುಗಿ ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_xattr_remove_all(ರಚನೆ ext2_xattr_handle *handle)
{
	ರಚನೆ ext2_xattr *x;
	ರಚನೆ ext2_xattr *ಕೊನೆ = handle->attrs + handle->ಎಣಿಕೆ_2;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	ಗಾಗಿ (x = handle->attrs; x < ಕೊನೆ; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->ಮೌಲ್ಯ);
		ಆದರೆ (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_ಎಣಿಕೆ = 0;
	handle->ಎಣಿಕೆ_2 = 0;
	ಹಿಂತಿರುಗಿ ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     ರಚನೆ ext2_xattr_handle **handle)
{
	ರಚನೆ ext2_xattr_handle *h;
	errcode_t err;

	ಆದರೆ (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_ದತ್ತಾಂಶ(fs->super))
		ಹಿಂತಿರುಗಿ EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(ಗಾತ್ರ(*h), &h);
	ಆದರೆ (err)
		ಹಿಂತಿರುಗಿ err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->ಸಾಮರ್ಥ್ಯ = 4;
	err = ext2fs_get_arrayzero(h->ಸಾಮರ್ಥ್ಯ, ಗಾತ್ರ(ರಚನೆ ext2_xattr),
				   &h->attrs);
	ಆದರೆ (err) {
		ext2fs_free_mem(&h);
		ಹಿಂತಿರುಗಿ err;
	}
	h->ಎಣಿಕೆ_2 = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_xattrs_close(ರಚನೆ ext2_xattr_handle **handle)
{
	ರಚನೆ ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_ಕೀಲಿಗಳು(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_xattrs_ಎಣಿಕೆ(ರಚನೆ ext2_xattr_handle *handle, size_t *ಎಣಿಕೆ_2)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*ಎಣಿಕೆ_2 = handle->ಎಣಿಕೆ_2;
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_xattrs_flags(ರಚನೆ ext2_xattr_handle *handle,
			      ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *new_flags, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	ಆದರೆ (old_flags)
		*old_flags = handle->flags;
	ಆದರೆ (new_flags)
		handle->flags = *new_flags;
	ಹಿಂತಿರುಗಿ 0;
}
