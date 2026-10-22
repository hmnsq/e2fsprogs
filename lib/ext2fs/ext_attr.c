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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#अन्तर्भाव <string.h>
#अन्तर्भाव <time.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2_ext_attr.h"
#अन्तर्भाव "ext4_acl.h"

#अन्तर्भाव "ext2fsP.h"

स्थिर errcode_t read_ea_inode_hash(ext2_filsys fs, ext2_ino_t ino, __u32 *hash)
{
	संरचना ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	यदि (retval)
		प्रत्यागम retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	प्रत्यागम 0;
}

#परिभाषय NAME_HASH_सरणम् 5
#परिभाषय मानम्_HASH_सरणम् 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_प्रविष्टिः(संरचना ext2_ext_attr_प्रविष्टिः *प्रविष्टिः, शून्य *दत्त)
{
	__u32 hash = 0;
	अचिह्नित वर्ण *name = (((अचिह्नित वर्ण *) प्रविष्टिः) +
			       परिमाण(संरचना ext2_ext_attr_प्रविष्टिः));
	पूर्णाङ्क n;

	प्रति (n = 0; n < प्रविष्टिः->e_name_len; n++) {
		hash = (hash << NAME_HASH_सरणम्) ^
		       (hash >> (8*परिमाण(hash) - NAME_HASH_सरणम्)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	यदि (प्रविष्टिः->e_मानम्_inum == 0 && प्रविष्टिः->e_मानम्_आकार != 0) {
		__u32 *मानम् = (__u32 *)दत्त;
		प्रति (n = (प्रविष्टिः->e_मानम्_आकार + EXT2_EXT_ATTR_आवर्तनम्) >>
			 EXT2_EXT_ATTR_PAD_द्वयङ्काः; n; n--) {
			hash = (hash << मानम्_HASH_सरणम्) ^
			       (hash >> (8*परिमाण(hash) - मानम्_HASH_सरणम्)) ^
			       ext2fs_le32_गन्तव्यम्_cpu(*मानम्++);
		}
	}

	प्रत्यागम hash;
}

__u32 ext2fs_ext_attr_hash_प्रविष्टिः_signed(संरचना ext2_ext_attr_प्रविष्टिः *प्रविष्टिः,
					शून्य *दत्त)
{
	__u32 hash = 0;
	चिह्नित वर्ण *name = (((चिह्नित वर्ण *) प्रविष्टिः) +
			     परिमाण(संरचना ext2_ext_attr_प्रविष्टिः));
	पूर्णाङ्क n;

	प्रति (n = 0; n < प्रविष्टिः->e_name_len; n++) {
		hash = (hash << NAME_HASH_सरणम्) ^
		       (hash >> (8*परिमाण(hash) - NAME_HASH_सरणम्)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	यदि (प्रविष्टिः->e_मानम्_inum == 0 && प्रविष्टिः->e_मानम्_आकार != 0) {
		__u32 *मानम् = (__u32 *)दत्त;
		प्रति (n = (प्रविष्टिः->e_मानम्_आकार + EXT2_EXT_ATTR_आवर्तनम्) >>
			 EXT2_EXT_ATTR_PAD_द्वयङ्काः; n; n--) {
			hash = (hash << मानम्_HASH_सरणम्) ^
			       (hash >> (8*परिमाण(hash) - मानम्_HASH_सरणम्)) ^
			       ext2fs_le32_गन्तव्यम्_cpu(*मानम्++);
		}
	}

	प्रत्यागम hash;
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
				      संरचना ext2_ext_attr_प्रविष्टिः *प्रविष्टिः,
				      शून्य *दत्त, __u32 *hash,
				      __u32 *signed_hash)
{
	*hash = ext2fs_ext_attr_hash_प्रविष्टिः(प्रविष्टिः, दत्त);
	यदि (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_प्रविष्टिः_signed(प्रविष्टिः, दत्त);

	यदि (प्रविष्टिः->e_मानम्_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, प्रविष्टिः->e_मानम्_inum,
					    &ea_inode_hash);
		यदि (retval)
			प्रत्यागम retval;

		*hash = (*hash << मानम्_HASH_सरणम्) ^
			(*hash >> (8*परिमाण(*hash) - मानम्_HASH_सरणम्)) ^
			ea_inode_hash;
		यदि (signed_hash)
			*signed_hash = (*signed_hash << मानम्_HASH_सरणम्) ^
				(*signed_hash >> (8*परिमाण(*hash) -
						  मानम्_HASH_सरणम्)) ^
				ea_inode_hash;
	}
	प्रत्यागम 0;
}

/*
 * ext2fs_ext_attr_hash_entry2()
 *
 * Compute the hash of an extended attribute.
 * This version of the function supports hashing entries that reference
 * external inodes (ea_inode feature).
 */
errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
				      संरचना ext2_ext_attr_प्रविष्टिः *प्रविष्टिः,
				      शून्य *दत्त, __u32 *hash)
{
	प्रत्यागम ext2fs_ext_attr_hash_entry3(fs, प्रविष्टिः, दत्त, hash, रिक्त);
}

#अपरिभाषय NAME_HASH_सरणम्
#अपरिभाषय मानम्_HASH_सरणम्

#परिभाषय खण्डम्_HASH_सरणम् 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
शून्य ext2fs_ext_attr_खण्डम्_rehash(संरचना ext2_ext_attr_शीर्षकम् *शीर्षकम्,
				  संरचना ext2_ext_attr_प्रविष्टिः *अन्तः)
{
	संरचना ext2_ext_attr_प्रविष्टिः *अत्र;
	__u32 hash = 0;

	अत्र = (संरचना ext2_ext_attr_प्रविष्टिः *)(शीर्षकम्+1);
	यावत् (अत्र < अन्तः && !EXT2_EXT_IS_चरमः_प्रविष्टिः(अत्र)) {
		यदि (!अत्र->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			भङ्ग;
		}
		hash = (hash << खण्डम्_HASH_सरणम्) ^
		       (hash >> (8*परिमाण(hash) - खण्डम्_HASH_सरणम्)) ^
		       अत्र->e_hash;
		अत्र = EXT2_EXT_ATTR_अग्र(अत्र);
	}
	शीर्षकम्->h_hash = hash;
}

#अपरिभाषय खण्डम्_HASH_सरणम्

__u32 ext2fs_get_ea_inode_hash(संरचना ext2_inode *inode)
{
	प्रत्यागम inode->i_atime;
}

शून्य ext2fs_समुच्चयः_ea_inode_hash(संरचना ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(संरचना ext2_inode *inode)
{
	प्रत्यागम ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_संस्करणम्;
}

शून्य ext2fs_समुच्चयः_ea_inode_ref(संरचना ext2_inode *inode, __u64 ref_गणनम्)
{
	inode->i_ctime = (__u32)(ref_गणनम् >> 32);
	inode->osd1.linux1.l_i_संस्करणम् = (__u32)ref_गणनम्;
}

स्थिर errcode_t check_ext_attr_शीर्षकम्(संरचना ext2_ext_attr_शीर्षकम् *शीर्षकम्)
{
	यदि ((शीर्षकम्->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     शीर्षकम्->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    शीर्षकम्->h_खण्डानि != 1)
		प्रत्यागम EXT2_ET_BAD_EA_शीर्षकम्;

	प्रत्यागम 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t खण्डम्, शून्य *buf,
				ext2_ino_t inum)
{
	पूर्णाङ्क		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, खण्डम्, 1, buf);
	यदि (retval)
		प्रत्यागम retval;

	यदि (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_खण्डम्_csum_verify(fs, inum, खण्डम्, buf))
		csum_failed = 1;

#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	ext2fs_swap_ext_attr(buf, buf, fs->blocksize, 1);
#यदि_अन्तः

	retval = check_ext_attr_शीर्षकम्(buf);
	यदि (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	प्रत्यागम retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t खण्डम्, शून्य *buf)
{
	प्रत्यागम ext2fs_read_ext_attr3(fs, खण्डम्, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t खण्डम्, शून्य *buf)
{
	प्रत्यागम ext2fs_read_ext_attr2(fs, खण्डम्, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t खण्डम्, शून्य *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	वर्ण		*write_buf;

#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	यदि (retval)
		प्रत्यागम retval;
	ext2fs_swap_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#अन्यथा
	write_buf = (वर्ण *) inbuf;
#यदि_अन्तः

	retval = ext2fs_ext_attr_खण्डम्_csum_समुच्चयः(fs, inum, खण्डम्,
			(संरचना ext2_ext_attr_शीर्षकम् *)write_buf);
	यदि (retval)
		प्रत्यागम retval;

	retval = io_channel_write_blk64(fs->io, खण्डम्, 1, write_buf);
#यदि_परिभाषितम् शब्दाः_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#यदि_अन्तः
	यदि (!retval)
		ext2fs_mark_परिवर्तितम्(fs);
	प्रत्यागम retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t खण्डम्, शून्य *inbuf)
{
	प्रत्यागम ext2fs_write_ext_attr3(fs, खण्डम्, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t खण्डम्, शून्य *inbuf)
{
	प्रत्यागम ext2fs_write_ext_attr2(fs, खण्डम्, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    वर्ण *खण्डम्_buf, पूर्णाङ्क adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	संरचना ext2_ext_attr_शीर्षकम् *शीर्षकम्;
	वर्ण	*buf = 0;

	यदि ((blk >= ext2fs_खण्डानि_गणनम्(fs->super)) ||
	    (blk < fs->super->s_first_दत्त_खण्डम्))
		प्रत्यागम EXT2_ET_BAD_EA_खण्डम्_अङ्कः;

	यदि (!खण्डम्_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		यदि (retval)
			प्रत्यागम retval;
		खण्डम्_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, खण्डम्_buf, inum);
	यदि (retval)
		गच्छ errout;

	शीर्षकम् = (संरचना ext2_ext_attr_शीर्षकम् *) खण्डम्_buf;
	शीर्षकम्->h_refcount += adjust;
	यदि (newcount)
		*newcount = शीर्षकम्->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, खण्डम्_buf, inum);
	यदि (retval)
		गच्छ errout;

errout:
	यदि (buf)
		ext2fs_free_mem(&buf);
	प्रत्यागम retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    वर्ण *खण्डम्_buf, पूर्णाङ्क adjust,
				    __u32 *newcount)
{
	प्रत्यागम ext2fs_adjust_ea_refcount3(fs, blk, खण्डम्_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					वर्ण *खण्डम्_buf, पूर्णाङ्क adjust,
					__u32 *newcount)
{
	प्रत्यागम ext2fs_adjust_ea_refcount2(fs, blk, खण्डम्_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
संरचना ext2_xattr {
	पूर्णाङ्क name_सूचकांक;
	वर्ण *name;
	वर्ण *short_name;
	शून्य *मानम्;
	अचिह्नित पूर्णाङ्क मानम्_len;
	ext2_ino_t ea_ino;
};

संरचना ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	संरचना ext2_xattr *attrs;
	पूर्णाङ्क क्षमता;
	पूर्णाङ्क गणनम्;
	पूर्णाङ्क ibody_गणनम्;
	ext2_ino_t ino;
	अचिह्नित पूर्णाङ्क flags;
};

स्थिर errcode_t ext2fs_xattrs_expand(संरचना ext2_xattr_handle *h,
				      अचिह्नित पूर्णाङ्क expandby)
{
	संरचना ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->क्षमता + expandby,
				   परिमाण(संरचना ext2_xattr), &new_attrs);
	यदि (err)
		प्रत्यागम err;

	स्मृतिं_प्रतिलिख(new_attrs, h->attrs, h->क्षमता * परिमाण(संरचना ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->क्षमता += expandby;
	h->attrs = new_attrs;

	प्रत्यागम 0;
}

संरचना ea_name_सूचकांक {
	पूर्णाङ्क सूचकांक;
	ध्रुव वर्ण *name;
};

/* Keep these names sorted in order of decreasing specificity. */
स्थिर संरचना ea_name_सूचकांक ea_नामानि[] = {
	{10, "gnu."},
	{3, "system.posix_acl_default"},
	{2, "system.posix_acl_access"},
	{8, "system.richacl"},
	{6, "security."},
	{4, "trusted."},
	{7, "system."},
	{1, "user."},
	{0, रिक्त},
};

स्थिर ध्रुव वर्ण *find_ea_उपसर्गः(पूर्णाङ्क सूचकांक)
{
	संरचना ea_name_सूचकांक *e;

	प्रति (e = ea_नामानि; e->name; e++)
		यदि (e->सूचकांक == सूचकांक)
			प्रत्यागम e->name;

	प्रत्यागम रिक्त;
}

स्थिर पूर्णाङ्क find_ea_सूचकांक(ध्रुव वर्ण *fullname, ध्रुव वर्ण **name, पूर्णाङ्क *सूचकांक)
{
	संरचना ea_name_सूचकांक *e;

	प्रति (e = ea_नामानि; e->name; e++) {
		यदि (वाक्ये_तुलय(fullname, e->name, वाक्यदैर्घ्यम्(e->name)) == 0) {
			*name = fullname + वाक्यदैर्घ्यम्(e->name);
			*सूचकांक = e->सूचकांक;
			प्रत्यागम 1;
		}
	}
	प्रत्यागम 0;
}

errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       संरचना ext2_inode_large *inode)
{
	संरचना ext2_ext_attr_शीर्षकम् *शीर्षकम्;
	शून्य *खण्डम्_buf = रिक्त;
	blk64_t blk;
	errcode_t err;
	संरचना ext2_inode_large i;

	/* Read inode? */
	यदि (inode == रिक्त) {
		err = ext2fs_read_inode_पूर्णम्(fs, ino, (संरचना ext2_inode *)&i,
					     परिमाण(संरचना ext2_inode_large));
		यदि (err)
			प्रत्यागम err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_खण्डम्(fs, (संरचना ext2_inode *)inode);
	यदि (blk == 0)
		प्रत्यागम 0;

	/* Find block, zero it, write back */
	यदि ((blk < fs->super->s_first_दत्त_खण्डम्) ||
	    (blk >= ext2fs_खण्डानि_गणनम्(fs->super))) {
		err = EXT2_ET_BAD_EA_खण्डम्_अङ्कः;
		गच्छ बहिर्गमनम्;
	}

	err = ext2fs_get_mem(fs->blocksize, &खण्डम्_buf);
	यदि (err)
		गच्छ बहिर्गमनम्;

	err = ext2fs_read_ext_attr3(fs, blk, खण्डम्_buf, ino);
	यदि (err)
		गच्छ out2;

	/* We only know how to deal with v2 EA blocks */
	शीर्षकम् = (संरचना ext2_ext_attr_शीर्षकम् *) खण्डम्_buf;
	यदि (शीर्षकम्->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_शीर्षकम्;
		गच्छ out2;
	}

	शीर्षकम्->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, खण्डम्_buf, ino);
	यदि (err)
		गच्छ out2;

	/* Erase link to block */
	ext2fs_file_acl_खण्डम्_समुच्चयः(fs, (संरचना ext2_inode *)inode, 0);
	यदि (शीर्षकम्->h_refcount == 0)
		ext2fs_खण्डम्_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_खण्डानि(fs, (संरचना ext2_inode *)inode, 1);
	यदि (err)
		गच्छ out2;

	/* Write inode? */
	यदि (inode == &i) {
		err = ext2fs_write_inode_पूर्णम्(fs, ino, (संरचना ext2_inode *)&i,
					      परिमाण(संरचना ext2_inode_large));
		यदि (err)
			गच्छ out2;
	}

out2:
	ext2fs_free_mem(&खण्डम्_buf);
बहिर्गमनम्:
	प्रत्यागम err;
}

स्थिर errcode_t prep_ea_खण्डम्_for_write(ext2_filsys fs, ext2_ino_t ino,
					 संरचना ext2_inode_large *inode)
{
	संरचना ext2_ext_attr_शीर्षकम् *शीर्षकम्;
	शून्य *खण्डम्_buf = रिक्त;
	blk64_t blk, goal;
	errcode_t err;

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_खण्डम्(fs, (संरचना ext2_inode *)inode);
	यदि (blk != 0) {
		यदि ((blk < fs->super->s_first_दत्त_खण्डम्) ||
		    (blk >= ext2fs_खण्डानि_गणनम्(fs->super))) {
			err = EXT2_ET_BAD_EA_खण्डम्_अङ्कः;
			गच्छ बहिर्गमनम्;
		}

		err = ext2fs_get_mem(fs->blocksize, &खण्डम्_buf);
		यदि (err)
			गच्छ बहिर्गमनम्;

		err = ext2fs_read_ext_attr3(fs, blk, खण्डम्_buf, ino);
		यदि (err)
			गच्छ out2;

		/* We only know how to deal with v2 EA blocks */
		शीर्षकम् = (संरचना ext2_ext_attr_शीर्षकम् *) खण्डम्_buf;
		यदि (शीर्षकम्->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_शीर्षकम्;
			गच्छ out2;
		}

		/* Single-user block.  We're done here. */
		यदि (शीर्षकम्->h_refcount == 1)
			गच्छ out2;

		/* We need to CoW the block. */
		शीर्षकम्->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, खण्डम्_buf, ino);
		यदि (err)
			गच्छ out2;
	} अन्यथा {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_सङ्कलय_खण्डानि(fs, (संरचना ext2_inode *)inode,
					     1);
		यदि (err)
			गच्छ बहिर्गमनम्;
	}

	/* Allocate a block */
	goal = ext2fs_find_inode_goal(fs, ino, (संरचना ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, रिक्त, &blk);
	यदि (err)
		गच्छ out2;
	ext2fs_file_acl_खण्डम्_समुच्चयः(fs, (संरचना ext2_inode *)inode, blk);
out2:
	यदि (खण्डम्_buf)
		ext2fs_free_mem(&खण्डम्_buf);
बहिर्गमनम्:
	प्रत्यागम err;
}


स्थिर अन्तःस्थ पूर्णाङ्क
posix_acl_xattr_गणनम्(size_t आकार)
{
        यदि (आकार < परिमाण(posix_acl_xattr_शीर्षकम्))
                प्रत्यागम -1;
        आकार -= परिमाण(posix_acl_xattr_शीर्षकम्);
        यदि (आकार % परिमाण(posix_acl_xattr_प्रविष्टिः))
                प्रत्यागम -1;
        प्रत्यागम आकार / परिमाण(posix_acl_xattr_प्रविष्टिः);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
स्थिर errcode_t convert_posix_acl_गन्तव्यम्_disk_बफर(ध्रुव शून्य *मानम्, size_t आकार,
						  शून्य *बहिर्गमनम्_buf, size_t *आकार_बहिर्गमनम्)
{
	ध्रुव posix_acl_xattr_शीर्षकम् *शीर्षकम् =
		(ध्रुव posix_acl_xattr_शीर्षकम्*) मानम्;
	ध्रुव posix_acl_xattr_प्रविष्टिः *अन्तः, *प्रविष्टिः =
		(ध्रुव posix_acl_xattr_प्रविष्टिः *)(शीर्षकम्+1);
	ext4_acl_शीर्षकम् *ext_acl;
	size_t s;
	वर्ण *e;

	पूर्णाङ्क गणनम्;

	यदि (!मानम्)
		प्रत्यागम EINVAL;
	यदि (आकार < परिमाण(posix_acl_xattr_शीर्षकम्))
		प्रत्यागम ENOMEM;
	यदि (शीर्षकम्->a_संस्करणम् != ext2fs_cpu_गन्तव्यम्_le32(POSIX_ACL_XATTR_संस्करणम्))
		प्रत्यागम EINVAL;

	गणनम् = posix_acl_xattr_गणनम्(आकार);
	ext_acl = बहिर्गमनम्_buf;
	ext_acl->a_संस्करणम् = ext2fs_cpu_गन्तव्यम्_le32(EXT4_ACL_संस्करणम्);

	यदि (गणनम् <= 0)
		प्रत्यागम EINVAL;

	e = (वर्ण *) बहिर्गमनम्_buf + परिमाण(ext4_acl_शीर्षकम्);
	s = परिमाण(ext4_acl_शीर्षकम्);
	प्रति (अन्तः = प्रविष्टिः + गणनम्; प्रविष्टिः != अन्तः;प्रविष्टिः++) {
		ext4_acl_प्रविष्टिः *disk_प्रविष्टिः = (ext4_acl_प्रविष्टिः*) e;
		disk_प्रविष्टिः->e_चिह्नकम् = प्रविष्टिः->e_चिह्नकम्;
		disk_प्रविष्टिः->e_perm = प्रविष्टिः->e_perm;

		विकल्प(ext2fs_le16_गन्तव्यम्_cpu(प्रविष्टिः->e_चिह्नकम्)) {
			पक्ष ACL_USER_OBJ:
			पक्ष ACL_गणः_OBJ:
			पक्ष ACL_आवरणम्:
			पक्ष ACL_OTHER:
				e += परिमाण(ext4_acl_प्रविष्टिः_short);
				s += परिमाण(ext4_acl_प्रविष्टिः_short);
				भङ्ग;
			पक्ष ACL_USER:
			पक्ष ACL_गणः:
				disk_प्रविष्टिः->e_id = प्रविष्टिः->e_id;
				e += परिमाण(ext4_acl_प्रविष्टिः);
				s += परिमाण(ext4_acl_प्रविष्टिः);
				भङ्ग;
			स्वभाव:
				प्रत्यागम EINVAL;
		}
	}
	*आकार_बहिर्गमनम् = s;
	प्रत्यागम 0;
}

स्थिर errcode_t convert_disk_बफर_गन्तव्यम्_posix_acl(ध्रुव शून्य *मानम्, size_t आकार,
						  शून्य **बहिर्गमनम्_buf, size_t *आकार_बहिर्गमनम्)
{
	posix_acl_xattr_शीर्षकम् *शीर्षकम्;
	posix_acl_xattr_प्रविष्टिः *प्रविष्टिः;
	ध्रुव ext4_acl_शीर्षकम् *ext_acl = (ध्रुव ext4_acl_शीर्षकम् *) मानम्;
	errcode_t err;
	ध्रुव वर्ण *cp;
	वर्ण *बहिर्गमनम्;

	यदि ((!मानम्) ||
	    (आकार < परिमाण(ext4_acl_शीर्षकम्)) ||
	    (ext_acl->a_संस्करणम् != ext2fs_cpu_गन्तव्यम्_le32(EXT4_ACL_संस्करणम्)))
		प्रत्यागम EINVAL;

	err = ext2fs_get_mem(आकार * 2, &बहिर्गमनम्);
	यदि (err)
		प्रत्यागम err;

	शीर्षकम् = (posix_acl_xattr_शीर्षकम् *) बहिर्गमनम्;
	शीर्षकम्->a_संस्करणम् = ext2fs_cpu_गन्तव्यम्_le32(POSIX_ACL_XATTR_संस्करणम्);
	प्रविष्टिः = (posix_acl_xattr_प्रविष्टिः *) (बहिर्गमनम् + परिमाण(posix_acl_xattr_शीर्षकम्));

	cp = (ध्रुव वर्ण *) मानम् + परिमाण(ext4_acl_शीर्षकम्);
	आकार -= परिमाण(ext4_acl_शीर्षकम्);

	यावत् (आकार > 0) {
		ध्रुव ext4_acl_प्रविष्टिः *disk_प्रविष्टिः = (ध्रुव ext4_acl_प्रविष्टिः *) cp;

		प्रविष्टिः->e_चिह्नकम् = disk_प्रविष्टिः->e_चिह्नकम्;
		प्रविष्टिः->e_perm = disk_प्रविष्टिः->e_perm;

		विकल्प(ext2fs_le16_गन्तव्यम्_cpu(प्रविष्टिः->e_चिह्नकम्)) {
			पक्ष ACL_USER_OBJ:
			पक्ष ACL_गणः_OBJ:
			पक्ष ACL_आवरणम्:
			पक्ष ACL_OTHER:
				प्रविष्टिः->e_id = 0;
				cp += परिमाण(ext4_acl_प्रविष्टिः_short);
				आकार -= परिमाण(ext4_acl_प्रविष्टिः_short);
				भङ्ग;
			पक्ष ACL_USER:
			पक्ष ACL_गणः:
				प्रविष्टिः->e_id = disk_प्रविष्टिः->e_id;
				cp += परिमाण(ext4_acl_प्रविष्टिः);
				आकार -= परिमाण(ext4_acl_प्रविष्टिः);
				भङ्ग;
			स्वभाव:
				ext2fs_free_mem(&बहिर्गमनम्);
				प्रत्यागम EINVAL;
		}
		प्रविष्टिः++;
	}
	*बहिर्गमनम्_buf = बहिर्गमनम्;
	*आकार_बहिर्गमनम् = ((वर्ण *) प्रविष्टिः - बहिर्गमनम्);
	प्रत्यागम 0;
}

स्थिर errcode_t
write_xattrs_गन्तव्यम्_बफर(ext2_filsys fs, संरचना ext2_xattr *attrs, पूर्णाङ्क गणनम्,
		       शून्य *entries_start, अचिह्नित पूर्णाङ्क storage_आकार,
		       अचिह्नित पूर्णाङ्क मानम्_विस्थापनम्_correction, पूर्णाङ्क write_hash)
{
	संरचना ext2_xattr *x;
	संरचना ext2_ext_attr_प्रविष्टिः *e = entries_start;
	वर्ण *अन्तः = (वर्ण *) entries_start + storage_आकार;
	अचिह्नित पूर्णाङ्क मानम्_आकार;
	errcode_t err;

	स्मृतिं_पूरय(entries_start, 0, storage_आकार);
	प्रति (x = attrs; x < attrs + गणनम्; x++) {
		मानम्_आकार = ((x->मानम्_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = वाक्यदैर्घ्यम्(x->short_name);
		e->e_name_सूचकांक = x->name_सूचकांक;

		e->e_मानम्_आकार = x->मानम्_len;
		e->e_मानम्_inum = x->ea_ino;

		/* Store name */
		स्मृतिं_प्रतिलिख((वर्ण *)e + परिमाण(*e), x->short_name, e->e_name_len);
		यदि (x->ea_ino) {
			e->e_मानम्_offs = 0;
		} अन्यथा {
			अन्तः -= मानम्_आकार;
			e->e_मानम्_offs = अन्तः - (वर्ण *) entries_start +
						मानम्_विस्थापनम्_correction;
			स्मृतिं_प्रतिलिख(अन्तः, x->मानम्, e->e_मानम्_आकार);
		}

		यदि (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : अन्तः,
							  &e->e_hash);
			यदि (err)
				प्रत्यागम err;
		} अन्यथा
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_अग्र(e);
		*(__u32 *)e = 0;
	}
	प्रत्यागम 0;
}

errcode_t ext2fs_xattrs_write(संरचना ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	ध्रुव अचिह्नित पूर्णाङ्क inode_आकार = EXT2_INODE_आकार(fs->super);
	संरचना ext2_inode_large *inode;
	वर्ण *start, *खण्डम्_buf = रिक्त;
	संरचना ext2_ext_attr_शीर्षकम् *शीर्षकम्;
	__u32 ea_inode_magic;
	blk64_t blk;
	अचिह्नित पूर्णाङ्क storage_आकार;
	अचिह्नित पूर्णाङ्क i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_आकार;
	यदि (i < परिमाण(*inode))
		i = परिमाण(*inode);
	err = ext2fs_get_memzero(i, &inode);
	यदि (err)
		प्रत्यागम err;

	err = ext2fs_read_inode_पूर्णम्(fs, handle->ino, EXT2_INODE(inode),
				     inode_आकार);
	यदि (err)
		गच्छ बहिर्गमनम्;

	/* If extra_isize isn't set, we need to set it now */
	यदि (inode->i_extra_isize == 0 &&
	    inode_आकार > EXT2_GOOD_OLD_INODE_आकार) {
		वर्ण *p = (वर्ण *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		यदि (extra == 0)
			extra = परिमाण(__u32);
		स्मृतिं_पूरय(p + EXT2_GOOD_OLD_INODE_आकार, 0, extra);
		inode->i_extra_isize = extra;
	}
	यदि (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		गच्छ बहिर्गमनम्;
	}

	/* Does the inode have space for EA? */
	यदि (inode->i_extra_isize < परिमाण(inode->i_extra_isize) ||
	    inode_आकार <= EXT2_GOOD_OLD_INODE_आकार + inode->i_extra_isize +
								परिमाण(__u32))
		गच्छ write_ea_खण्डम्;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	स्मृतिं_प्रतिलिख(((वर्ण *) inode) + EXT2_GOOD_OLD_INODE_आकार +
	       inode->i_extra_isize, &ea_inode_magic, परिमाण(__u32));
	storage_आकार = inode_आकार - EXT2_GOOD_OLD_INODE_आकार -
				inode->i_extra_isize - परिमाण(__u32);
	start = ((वर्ण *) inode) + EXT2_GOOD_OLD_INODE_आकार +
				inode->i_extra_isize + परिमाण(__u32);

	err = write_xattrs_गन्तव्यम्_बफर(fs, handle->attrs, handle->ibody_गणनम्,
				     start, storage_आकार, 0, 0);
	यदि (err)
		गच्छ बहिर्गमनम्;
write_ea_खण्डम्:
	/* Are we done? */
	यदि (handle->ibody_गणनम् == handle->गणनम् &&
	    !ext2fs_file_acl_खण्डम्(fs, EXT2_INODE(inode)))
		गच्छ skip_ea_खण्डम्;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &खण्डम्_buf);
	यदि (err)
		गच्छ बहिर्गमनम्;

	storage_आकार = fs->blocksize - परिमाण(संरचना ext2_ext_attr_शीर्षकम्);
	start = खण्डम्_buf + परिमाण(संरचना ext2_ext_attr_शीर्षकम्);

	err = write_xattrs_गन्तव्यम्_बफर(fs, handle->attrs + handle->ibody_गणनम्,
				     handle->गणनम् - handle->ibody_गणनम्, start,
				     storage_आकार, start - खण्डम्_buf, 1);
	यदि (err)
		गच्छ out2;

	/* Write a header on the EA block */
	शीर्षकम् = (संरचना ext2_ext_attr_शीर्षकम् *) खण्डम्_buf;
	शीर्षकम्->h_magic = EXT2_EXT_ATTR_MAGIC;
	शीर्षकम्->h_refcount = 1;
	शीर्षकम्->h_खण्डानि = 1;

	/* Get a new block for writing */
	err = prep_ea_खण्डम्_for_write(fs, handle->ino, inode);
	यदि (err)
		गच्छ out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_खण्डम्(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, खण्डम्_buf, handle->ino);
	यदि (err)
		गच्छ out2;

skip_ea_खण्डम्:
	blk = ext2fs_file_acl_खण्डम्(fs, (संरचना ext2_inode *)inode);
	यदि (!खण्डम्_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		यदि (err)
			गच्छ बहिर्गमनम्;
	}

	/* Write the inode */
	err = ext2fs_write_inode_पूर्णम्(fs, handle->ino, EXT2_INODE(inode),
				      inode_आकार);
	यदि (err)
		गच्छ out2;

out2:
	ext2fs_free_mem(&खण्डम्_buf);
बहिर्गमनम्:
	ext2fs_free_mem(&inode);
	प्रत्यागम err;
}

स्थिर errcode_t read_xattrs_from_बफर(संरचना ext2_xattr_handle *handle,
					 संरचना ext2_inode_large *inode,
					 संरचना ext2_ext_attr_प्रविष्टिः *entries,
					 अचिह्नित पूर्णाङ्क storage_आकार,
					 वर्ण *मानम्_start)
{
	संरचना ext2_xattr *x;
	संरचना ext2_ext_attr_प्रविष्टिः *प्रविष्टिः, *अन्तः;
	ध्रुव वर्ण *उपसर्गः;
	अचिह्नित पूर्णाङ्क remain, उपसर्गः_len;
	errcode_t err;
	अचिह्नित पूर्णाङ्क मानानि_आकार = storage_आकार +
			((वर्ण *)entries - मानम्_start);

	/* find the end */
	अन्तः = entries;
	remain = storage_आकार;
	यावत् (remain >= परिमाण(संरचना ext2_ext_attr_प्रविष्टिः) &&
	       !EXT2_EXT_IS_चरमः_प्रविष्टिः(अन्तः)) {

		/* header eats this space */
		remain -= परिमाण(संरचना ext2_ext_attr_प्रविष्टिः);

		/* is attribute name valid? */
		यदि (EXT2_EXT_ATTR_आकार(अन्तः->e_name_len) > remain)
			प्रत्यागम EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_आकार(अन्तः->e_name_len);
		अन्तः = EXT2_EXT_ATTR_अग्र(अन्तः);
	}

	प्रविष्टिः = entries;
	remain = storage_आकार;
	यावत् (remain >= परिमाण(संरचना ext2_ext_attr_प्रविष्टिः) &&
	       !EXT2_EXT_IS_चरमः_प्रविष्टिः(प्रविष्टिः)) {

		/* Allocate space for more attrs? */
		यदि (handle->गणनम् == handle->क्षमता) {
			err = ext2fs_xattrs_expand(handle, 4);
			यदि (err)
				प्रत्यागम err;
		}

		x = handle->attrs + handle->गणनम्;

		/* header eats this space */
		remain -= परिमाण(संरचना ext2_ext_attr_प्रविष्टिः);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_आकार(प्रविष्टिः->e_name_len);

		/* Extract name */
		उपसर्गः = find_ea_उपसर्गः(प्रविष्टिः->e_name_सूचकांक);
		उपसर्गः_len = (उपसर्गः ? वाक्यदैर्घ्यम्(उपसर्गः) : 0);
		err = ext2fs_get_memzero(प्रविष्टिः->e_name_len + उपसर्गः_len + 1,
					 &x->name);
		यदि (err)
			प्रत्यागम err;
		यदि (उपसर्गः)
			स्मृतिं_प्रतिलिख(x->name, उपसर्गः, उपसर्गः_len);
		यदि (प्रविष्टिः->e_name_len)
			स्मृतिं_प्रतिलिख(x->name + उपसर्गः_len,
			       (वर्ण *)प्रविष्टिः + परिमाण(*प्रविष्टिः),
			       प्रविष्टिः->e_name_len);
		x->short_name = x->name + उपसर्गः_len;
		x->name_सूचकांक = प्रविष्टिः->e_name_सूचकांक;

		/* Check & copy value */
		यदि (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    प्रविष्टिः->e_मानम्_inum != 0)
			प्रत्यागम EXT2_ET_BAD_EA_खण्डम्_अङ्कः;

		यदि (प्रविष्टिः->e_मानम्_inum == 0) {
			यदि (प्रविष्टिः->e_मानम्_आकार > remain)
				प्रत्यागम EXT2_ET_EA_BAD_मानम्_आकार;

			यदि (प्रविष्टिः->e_मानम्_offs + प्रविष्टिः->e_मानम्_आकार > मानानि_आकार)
				प्रत्यागम EXT2_ET_EA_BAD_मानम्_विस्थापनम्;

			यदि (प्रविष्टिः->e_मानम्_आकार > 0 &&
			    मानम्_start + प्रविष्टिः->e_मानम्_offs <
			    (वर्ण *)अन्तः + परिमाण(__u32))
				प्रत्यागम EXT2_ET_EA_BAD_मानम्_विस्थापनम्;

			remain -= प्रविष्टिः->e_मानम्_आकार;

			err = ext2fs_get_mem(प्रविष्टिः->e_मानम्_आकार, &x->मानम्);
			यदि (err)
				प्रत्यागम err;
			स्मृतिं_प्रतिलिख(x->मानम्, मानम्_start + प्रविष्टिः->e_मानम्_offs,
			       प्रविष्टिः->e_मानम्_आकार);
		} अन्यथा {
			संरचना ext2_inode *ea_inode;
			ext2_file_t ea_file;

			यदि (प्रविष्टिः->e_मानम्_offs != 0)
				प्रत्यागम EXT2_ET_EA_BAD_मानम्_विस्थापनम्;

			यदि (प्रविष्टिः->e_मानम्_आकार > (64 * 1024))
				प्रत्यागम EXT2_ET_EA_BAD_मानम्_आकार;

			err = ext2fs_get_mem(प्रविष्टिः->e_मानम्_आकार, &x->मानम्);
			यदि (err)
				प्रत्यागम err;

			err = ext2fs_file_open(handle->fs, प्रविष्टिः->e_मानम्_inum,
					       0, &ea_file);
			यदि (err)
				प्रत्यागम err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			यदि ((ea_inode->i_flags & EXT4_INLINE_दत्त_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_गणनम् == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			अन्यथा यदि ((__u64) ext2fs_file_get_आकार(ea_file) !=
				 प्रविष्टिः->e_मानम्_आकार)
				err = EXT2_ET_EA_BAD_मानम्_आकार;
			अन्यथा
				err = ext2fs_file_read(ea_file, x->मानम्,
						       प्रविष्टिः->e_मानम्_आकार, 0);
			ext2fs_file_close(ea_file);
			यदि (err)
				प्रत्यागम err;
		}

		x->ea_ino = प्रविष्टिः->e_मानम्_inum;
		x->मानम्_len = प्रविष्टिः->e_मानम्_आकार;

		/* e_hash may be 0 in older inode's ea */
		यदि (प्रविष्टिः->e_hash != 0) {
			__u32 hash, signed_hash;

			शून्य *दत्त = (प्रविष्टिः->e_मानम्_inum != 0) ?
					0 : मानम्_start + प्रविष्टिः->e_मानम्_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, प्रविष्टिः,
							  दत्त, &hash,
							  &signed_hash);
			यदि (err)
				प्रत्यागम err;
			यदि ((प्रविष्टिः->e_hash != hash) &&
			    (प्रविष्टिः->e_hash != signed_hash)) {
				संरचना ext2_inode child;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							प्रविष्टिः->e_मानम्_inum,
							&child);
				यदि (err)
					प्रत्यागम err;
				यदि (child.i_mtime != handle->ino ||
				    child.i_generation != inode->i_generation)
					प्रत्यागम EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->गणनम्++;
		प्रविष्टिः = EXT2_EXT_ATTR_अग्र(प्रविष्टिः);
	}

	प्रत्यागम 0;
}

स्थिर शून्य xattrs_free_कुञ्चिकाः(संरचना ext2_xattr_handle *h)
{
	संरचना ext2_xattr *a = h->attrs;
	पूर्णाङ्क i;

	प्रति (i = 0; i < h->क्षमता; i++) {
		यदि (a[i].name)
			ext2fs_free_mem(&a[i].name);
		यदि (a[i].मानम्)
			ext2fs_free_mem(&a[i].मानम्);
	}
	h->गणनम् = 0;
	h->ibody_गणनम् = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(संरचना ext2_xattr_handle *handle,
				   संरचना ext2_inode_large *inode)
{
	संरचना ext2_ext_attr_शीर्षकम् *शीर्षकम्;
	__u32 ea_inode_magic;
	अचिह्नित पूर्णाङ्क storage_आकार;
	वर्ण *start, *खण्डम्_buf = रिक्त;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_कुञ्चिकाः(handle);

	/* Does the inode have space for EA? */
	यदि (inode->i_extra_isize < परिमाण(inode->i_extra_isize) ||
	    EXT2_INODE_आकार(handle->fs->super) <= EXT2_GOOD_OLD_INODE_आकार +
						  inode->i_extra_isize +
						  परिमाण(__u32))
		गच्छ read_ea_खण्डम्;
	यदि (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		गच्छ बहिर्गमनम्;
	}

	/* Look for EA in the inode */
	स्मृतिं_प्रतिलिख(&ea_inode_magic, ((वर्ण *) inode) + EXT2_GOOD_OLD_INODE_आकार +
	       inode->i_extra_isize, परिमाण(__u32));
	यदि (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_आकार = EXT2_INODE_आकार(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_आकार - inode->i_extra_isize -
			परिमाण(__u32);
		start = ((वर्ण *) inode) + EXT2_GOOD_OLD_INODE_आकार +
			inode->i_extra_isize + परिमाण(__u32);

		err = read_xattrs_from_बफर(handle, inode,
					(संरचना ext2_ext_attr_प्रविष्टिः *) start,
					storage_आकार, start);
		यदि (err)
			गच्छ बहिर्गमनम्;

		handle->ibody_गणनम् = handle->गणनम्;
	}

read_ea_खण्डम्:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_खण्डम्(handle->fs, EXT2_INODE(inode));
	यदि (blk != 0) {
		यदि ((blk < handle->fs->super->s_first_दत्त_खण्डम्) ||
		    (blk >= ext2fs_खण्डानि_गणनम्(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_खण्डम्_अङ्कः;
			गच्छ बहिर्गमनम्;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &खण्डम्_buf);
		यदि (err)
			गच्छ बहिर्गमनम्;

		err = ext2fs_read_ext_attr3(handle->fs, blk, खण्डम्_buf,
					    handle->ino);
		यदि (err)
			गच्छ out3;

		/* We only know how to deal with v2 EA blocks */
		शीर्षकम् = (संरचना ext2_ext_attr_शीर्षकम् *) खण्डम्_buf;
		यदि (शीर्षकम्->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_शीर्षकम्;
			गच्छ out3;
		}

		/* Read EAs */
		storage_आकार = handle->fs->blocksize -
			परिमाण(संरचना ext2_ext_attr_शीर्षकम्);
		start = खण्डम्_buf + परिमाण(संरचना ext2_ext_attr_शीर्षकम्);
		err = read_xattrs_from_बफर(handle, inode,
					(संरचना ext2_ext_attr_प्रविष्टिः *) start,
					storage_आकार, खण्डम्_buf);
	}

out3:
	यदि (खण्डम्_buf)
		ext2fs_free_mem(&खण्डम्_buf);
बहिर्गमनम्:
	प्रत्यागम err;
}

errcode_t ext2fs_xattrs_read(संरचना ext2_xattr_handle *handle)
{
	संरचना ext2_inode_large *inode;
	size_t inode_आकार = EXT2_INODE_आकार(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	यदि (inode_आकार < परिमाण(*inode))
		inode_आकार = परिमाण(*inode);
	err = ext2fs_get_memzero(inode_आकार, &inode);
	यदि (err)
		प्रत्यागम err;

	err = ext2fs_read_inode_पूर्णम्(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_आकार(handle->fs->super));
	यदि (err)
		गच्छ बहिर्गमनम्;

	err = ext2fs_xattrs_read_inode(handle, inode);

बहिर्गमनम्:
	ext2fs_free_mem(&inode);

	प्रत्यागम err;
}

errcode_t ext2fs_xattrs_iterate(संरचना ext2_xattr_handle *h,
				पूर्णाङ्क (*func)(वर्ण *name, वर्ण *मानम्,
					    size_t मानम्_len, शून्य *दत्त),
				शून्य *दत्त)
{
	संरचना ext2_xattr *x;
	पूर्णाङ्क dirty = 0;
	पूर्णाङ्क ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	प्रति (x = h->attrs; x < h->attrs + h->गणनम्; x++) {
		ret = func(x->name, x->मानम्, x->मानम्_len, दत्त);
		यदि (ret & XATTR_परिवर्तितम्)
			dirty = 1;
		यदि (ret & XATTR_ABORT)
			भङ्ग;
	}

	यदि (dirty)
		प्रत्यागम ext2fs_xattrs_write(h);
	प्रत्यागम 0;
}

errcode_t ext2fs_xattr_get(संरचना ext2_xattr_handle *h, ध्रुव वर्ण *कुञ्चिका,
			   शून्य **मानम्, size_t *मानम्_len)
{
	संरचना ext2_xattr *x;
	वर्ण *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	प्रति (x = h->attrs; x < h->attrs + h->गणनम्; x++) {
		यदि (वाक्यं_तुलय(x->name, कुञ्चिका))
			अनुवर्त;

		यदि (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((वाक्यं_तुलय(कुञ्चिका, "system.posix_acl_default") == 0) ||
		     (वाक्यं_तुलय(कुञ्चिका, "system.posix_acl_access") == 0))) {
			err = convert_disk_बफर_गन्तव्यम्_posix_acl(x->मानम्, x->मानम्_len,
							       मानम्, मानम्_len);
			प्रत्यागम err;
		} अन्यथा {
			err = ext2fs_get_mem(x->मानम्_len, &val);
			यदि (err)
				प्रत्यागम err;
			स्मृतिं_प्रतिलिख(val, x->मानम्, x->मानम्_len);
			*मानम् = val;
			*मानम्_len = x->मानम्_len;
			प्रत्यागम 0;
		}
	}

	प्रत्यागम EXT2_ET_EA_कुञ्चिका_NOT_प्राप्तम्;
}

errcode_t ext2fs_xattr_inode_max_आकार(ext2_filsys fs, ext2_ino_t ino,
				      size_t *आकार)
{
	संरचना ext2_ext_attr_प्रविष्टिः *प्रविष्टिः;
	संरचना ext2_inode_large *inode;
	__u32 ea_inode_magic;
	अचिह्नित पूर्णाङ्क minoff;
	वर्ण *start;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_आकार(fs->super);
	यदि (i < परिमाण(*inode))
		i = परिमाण(*inode);
	err = ext2fs_get_memzero(i, &inode);
	यदि (err)
		प्रत्यागम err;

	err = ext2fs_read_inode_पूर्णम्(fs, ino, (संरचना ext2_inode *)inode,
				     EXT2_INODE_आकार(fs->super));
	यदि (err)
		गच्छ बहिर्गमनम्;

	/* Does the inode have size for EA? */
	यदि (EXT2_INODE_आकार(fs->super) <= EXT2_GOOD_OLD_INODE_आकार +
						  inode->i_extra_isize +
						  परिमाण(__u32)) {
		err = EXT2_ET_INLINE_दत्त_NO_SPACE;
		गच्छ बहिर्गमनम्;
	}

	minoff = EXT2_INODE_आकार(fs->super) - परिमाण(*inode) - परिमाण(__u32);
	स्मृतिं_प्रतिलिख(&ea_inode_magic, ((वर्ण *) inode) + EXT2_GOOD_OLD_INODE_आकार +
	       inode->i_extra_isize, परिमाण(__u32));
	यदि (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		start= ((वर्ण *) inode) + EXT2_GOOD_OLD_INODE_आकार +
			inode->i_extra_isize + परिमाण(__u32);
		प्रविष्टिः = (संरचना ext2_ext_attr_प्रविष्टिः *) start;
		यावत् (!EXT2_EXT_IS_चरमः_प्रविष्टिः(प्रविष्टिः)) {
			यदि (!प्रविष्टिः->e_मानम्_inum && प्रविष्टिः->e_मानम्_आकार) {
				अचिह्नित पूर्णाङ्क offs = प्रविष्टिः->e_मानम्_offs;
				यदि (offs < minoff)
					minoff = offs;
			}
			प्रविष्टिः = EXT2_EXT_ATTR_अग्र(प्रविष्टिः);
		}
		*आकार = minoff - ((वर्ण *)प्रविष्टिः - (वर्ण *)start) - परिमाण(__u32);
	} अन्यथा {
		/* no xattr.  return a maximum size */
		*आकार = EXT2_EXT_ATTR_आकार(minoff -
					   EXT2_EXT_ATTR_LEN(वाक्यदैर्घ्यम्("data")) -
					   EXT2_EXT_ATTR_आवर्तनम् - परिमाण(__u32));
	}

बहिर्गमनम्:
	ext2fs_free_mem(&inode);
	प्रत्यागम err;
}

स्थिर errcode_t xattr_create_ea_inode(ext2_filsys fs, ध्रुव शून्य *मानम्,
				       size_t मानम्_len, ext2_ino_t *ea_ino)
{
	संरचना ext2_inode inode;
	ext2_ino_t ino;
	ext2_file_t file;
	__u32 hash;
	errcode_t ret;

	ret = ext2fs_new_inode(fs, 0, 0, 0, &ino);
	यदि (ret)
		प्रत्यागम ret;

	स्मृतिं_पूरय(&inode, 0, परिमाण(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	यदि (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_आकार = 0;
	inode.i_बहुलकम् = LINUX_S_IFREG | 0600;
	inode.i_links_गणनम् = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	यदि (ret)
		प्रत्यागम ret;
	/*
	 * ref_count and hash utilize inode's i_*time fields.
	 * ext2fs_write_new_inode() call above initializes these fields with
	 * current time. That's why ref count and hash updates are done
	 * separately below.
	 */
	ext2fs_समुच्चयः_ea_inode_ref(&inode, 1);
	hash = ext2fs_crc32c_le(fs->csum_बीजम्, मानम्, मानम्_len);
	ext2fs_समुच्चयः_ea_inode_hash(&inode, hash);

	ret = ext2fs_write_inode(fs, ino, &inode);
	यदि (ret)
		प्रत्यागम ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	यदि (ret)
		प्रत्यागम ret;
	ret = ext2fs_file_write(file, मानम्, मानम्_len, रिक्त);
	ext2fs_file_close(file);
	यदि (ret)
		प्रत्यागम ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	प्रत्यागम 0;
}

स्थिर errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	संरचना ext2_inode_large inode;
	__u64 ref_गणनम्;
	errcode_t ret;

	ret = ext2fs_read_inode_पूर्णम्(fs, ino, (संरचना ext2_inode *)&inode,
				     परिमाण(inode));
	यदि (ret)
		गच्छ बहिर्गमनम्;

	ref_गणनम् = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_गणनम्--;
	ext2fs_समुच्चयः_ea_inode_ref(EXT2_INODE(&inode), ref_गणनम्);

	यदि (ref_गणनम्)
		गच्छ write_बहिर्गमनम्;

	inode.i_links_गणनम् = 0;
	ext2fs_समुच्चयः_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	यदि (ret)
		गच्छ write_बहिर्गमनम्;

	यदि (ext2fs_inode_has_valid_blocks2(fs, (संरचना ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (संरचना ext2_inode *)&inode, रिक्त,
				   0, ~0ULL);
		यदि (ret)
			गच्छ बहिर्गमनम्;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_बहिर्गमनम्:
	ret = ext2fs_write_inode_पूर्णम्(fs, ino, (संरचना ext2_inode *)&inode,
				      परिमाण(inode));
बहिर्गमनम्:
	प्रत्यागम ret;
}

स्थिर errcode_t xattr_update_प्रविष्टिः(ext2_filsys fs, संरचना ext2_xattr *x,
				    ध्रुव वर्ण *name, ध्रुव वर्ण *short_name,
				    पूर्णाङ्क सूचकांक, ध्रुव शून्य *मानम्,
				    size_t मानम्_len, पूर्णाङ्क अन्तर्_inode)
{
	ext2_ino_t ea_ino = 0;
	शून्य *new_मानम् = रिक्त;
	वर्ण *new_name = रिक्त;
	पूर्णाङ्क name_len;
	errcode_t ret;

	यदि (!x->name) {
		name_len = वाक्यदैर्घ्यम्(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		यदि (ret)
			गच्छ fail;
		स्मृतिं_प्रतिलिख(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(मानम्_len, &new_मानम्);
	यदि (ret)
		गच्छ fail;
	स्मृतिं_प्रतिलिख(new_मानम्, मानम्, मानम्_len);

	यदि (अन्तर्_inode) {
		ret = xattr_create_ea_inode(fs, मानम्, मानम्_len, &ea_ino);
		यदि (ret)
			गच्छ fail;
	}

	यदि (x->ea_ino) {
		ret = xattr_inode_dec_ref(fs, x->ea_ino);
		यदि (ret)
			गच्छ fail;
	}

	यदि (!x->name) {
		x->name = new_name;
		x->short_name = new_name + (short_name  - name);
	}
	x->name_सूचकांक = सूचकांक;

	यदि (x->मानम्)
		ext2fs_free_mem(&x->मानम्);
	x->मानम् = new_मानम्;
	x->मानम्_len = मानम्_len;
	x->ea_ino = ea_ino;
	प्रत्यागम 0;
fail:
	यदि (new_name)
		ext2fs_free_mem(&new_name);
	यदि (new_मानम्)
		ext2fs_free_mem(&new_मानम्);
	यदि (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क xattr_find_position(संरचना ext2_xattr *attrs, पूर्णाङ्क गणनम्,
			       ध्रुव वर्ण *shortname, पूर्णाङ्क name_idx)
{
	संरचना ext2_xattr *x;
	पूर्णाङ्क i;
	पूर्णाङ्क shortname_len, x_shortname_len;

	shortname_len = वाक्यदैर्घ्यम्(shortname);

	प्रति (i = 0, x = attrs; i < गणनम्; i++, x++) {
		यदि (name_idx < x->name_सूचकांक)
			भङ्ग;
		यदि (name_idx > x->name_सूचकांक)
			अनुवर्त;

		x_shortname_len = वाक्यदैर्घ्यम्(x->short_name);
		यदि (shortname_len < x_shortname_len)
			भङ्ग;
		यदि (shortname_len > x_shortname_len)
			अनुवर्त;

		यदि (memcmp(shortname, x->short_name, shortname_len) <= 0)
			भङ्ग;
	}
	प्रत्यागम i;
}

स्थिर errcode_t xattr_array_update(संरचना ext2_xattr_handle *h,
				    ध्रुव वर्ण *name,
				    ध्रुव शून्य *मानम्, size_t मानम्_len,
				    पूर्णाङ्क ibody_free, पूर्णाङ्क खण्डम्_free,
				    पूर्णाङ्क old_idx, पूर्णाङ्क अन्तर्_inode)
{
	संरचना ext2_xattr tmp;
	पूर्णाङ्क सङ्कलय_गन्तव्यम्_ibody;
	पूर्णाङ्क needed;
	पूर्णाङ्क name_len, name_idx = 0;
	ध्रुव वर्ण *shortname = name;
	पूर्णाङ्क new_idx;
	पूर्णाङ्क ret;

	find_ea_सूचकांक(name, &shortname, &name_idx);
	name_len = वाक्यदैर्घ्यम्(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	यदि (!अन्तर्_inode)
		needed += EXT2_EXT_ATTR_आकार(मानम्_len);

	यदि (old_idx >= 0 && old_idx < h->ibody_गणनम्) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		यदि (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_आकार(
						h->attrs[old_idx].मानम्_len);
	}

	यदि (needed <= ibody_free) {
		यदि (old_idx < 0) {
			new_idx = h->ibody_गणनम्;
			सङ्कलय_गन्तव्यम्_ibody = 1;
			गच्छ सङ्कलय_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_प्रविष्टिः(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, मानम्,
					 मानम्_len, अन्तर्_inode);
		यदि (ret)
			प्रत्यागम ret;
		यदि (h->ibody_गणनम् <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_गणनम् + 1,
				h->attrs + h->ibody_गणनम्,
				(old_idx - h->ibody_गणनम्) * परिमाण(*h->attrs));
			h->attrs[h->ibody_गणनम्] = tmp;
			h->ibody_गणनम्++;
		}
		प्रत्यागम 0;
	}

	यदि (h->ibody_गणनम् <= old_idx) {
		खण्डम्_free += EXT2_EXT_ATTR_LEN(name_len);
		यदि (!h->attrs[old_idx].ea_ino)
			खण्डम्_free +=
				EXT2_EXT_ATTR_आकार(h->attrs[old_idx].मानम्_len);
	}

	यदि (needed > खण्डम्_free)
		प्रत्यागम EXT2_ET_EA_NO_SPACE;

	यदि (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_प्रविष्टिः(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, मानम्,
					 मानम्_len, अन्तर्_inode);
		यदि (ret)
			प्रत्यागम ret;
		यदि (old_idx < h->ibody_गणनम्) {
			/*
			 * Move entry from ibody to the block. Note that
			 * entries in the block are sorted.
			 */
			new_idx = xattr_find_position(h->attrs + h->ibody_गणनम्,
						      h->गणनम् - h->ibody_गणनम्,
						      shortname, name_idx);
			new_idx += h->ibody_गणनम् - 1;
			tmp = h->attrs[old_idx];
			memmove(h->attrs + old_idx, h->attrs + old_idx + 1,
				(new_idx - old_idx) * परिमाण(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_गणनम्--;
		}
		प्रत्यागम 0;
	}

	new_idx = xattr_find_position(h->attrs + h->ibody_गणनम्,
				      h->गणनम् - h->ibody_गणनम्,
				      shortname, name_idx);
	new_idx += h->ibody_गणनम्;
	सङ्कलय_गन्तव्यम्_ibody = 0;

सङ्कलय_new:
	यदि (h->गणनम् == h->क्षमता) {
		ret = ext2fs_xattrs_expand(h, 4);
		यदि (ret)
			प्रत्यागम ret;
	}

	ret = xattr_update_प्रविष्टिः(h->fs, &h->attrs[h->गणनम्], name, shortname,
				 name_idx, मानम्, मानम्_len, अन्तर्_inode);
	यदि (ret)
		प्रत्यागम ret;

	tmp = h->attrs[h->गणनम्];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->गणनम् - new_idx)*परिमाण(*h->attrs));
	h->attrs[new_idx] = tmp;
	यदि (सङ्कलय_गन्तव्यम्_ibody)
		h->ibody_गणनम्++;
	h->गणनम्++;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क space_प्रयुक्तम्(संरचना ext2_xattr *attrs, पूर्णाङ्क गणनम्)
{
	पूर्णाङ्क योग_2 = 0;
	संरचना ext2_xattr *x;
	पूर्णाङ्क i, len;

	प्रति (i = 0, x = attrs; i < गणनम्; i++, x++) {
		len = वाक्यदैर्घ्यम्(x->short_name);
		योग_2 += EXT2_EXT_ATTR_LEN(len);
		यदि (!x->ea_ino)
			योग_2 += EXT2_EXT_ATTR_आकार(x->मानम्_len);
	}
	प्रत्यागम योग_2;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#परिभाषय EXT4_XATTR_MIN_LARGE_EA_आकार(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - परिमाण(संरचना ext2_ext_attr_शीर्षकम्) - 4)

errcode_t ext2fs_xattr_समुच्चयः(संरचना ext2_xattr_handle *h,
			   ध्रुव वर्ण *name,
			   ध्रुव शून्य *मानम्,
			   size_t मानम्_len)
{
	ext2_filsys fs = h->fs;
	ध्रुव पूर्णाङ्क inode_आकार = EXT2_INODE_आकार(fs->super);
	संरचना ext2_inode_large *inode = रिक्त;
	संरचना ext2_xattr *x;
	वर्ण *new_मानम्;
	पूर्णाङ्क ibody_free, खण्डम्_free;
	पूर्णाङ्क अन्तर्_inode = 0;
	पूर्णाङ्क old_idx = -1;
	पूर्णाङ्क extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(मानम्_len, &new_मानम्);
	यदि (ret)
		प्रत्यागम ret;
	यदि (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((वाक्यं_तुलय(name, "system.posix_acl_default") == 0) ||
	     (वाक्यं_तुलय(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_गन्तव्यम्_disk_बफर(मानम्, मानम्_len,
						       new_मानम्, &मानम्_len);
		यदि (ret)
			गच्छ बहिर्गमनम्;
	} अन्यथा यदि (मानम्_len)
		स्मृतिं_प्रतिलिख(new_मानम्, मानम्, मानम्_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	प्रति (x = h->attrs; x < h->attrs + h->गणनम्; x++) {
		यदि (!वाक्यं_तुलय(x->name, name)) {
			यदि (!x->ea_ino && x->मानम्_len == मानम्_len &&
			    (!मानम्_len ||
			     !memcmp(x->मानम्, new_मानम्, मानम्_len))) {
				ret = 0;
				गच्छ बहिर्गमनम्;
			}
			old_idx = x - h->attrs;
			भङ्ग;
		}
	}

	ret = ext2fs_get_memzero(inode_आकार, &inode);
	यदि (ret)
		गच्छ बहिर्गमनम्;
	ret = ext2fs_read_inode_पूर्णम्(fs, h->ino,
				     (संरचना ext2_inode *)inode,
				     inode_आकार);
	यदि (ret)
		गच्छ बहिर्गमनम्;
	यदि (inode_आकार > EXT2_GOOD_OLD_INODE_आकार) {
		extra_isize = inode->i_extra_isize;
		यदि (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			यदि (extra_isize == 0)
				extra_isize = परिमाण(__u32);
		}
		ibody_free = inode_आकार - EXT2_GOOD_OLD_INODE_आकार;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= परिमाण(__u32) * 2;
		ibody_free -= space_प्रयुक्तम्(h->attrs, h->ibody_गणनम्);
	} अन्यथा
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	यदि (वाक्यं_तुलय(name, "system.data") == 0) {
		यदि (h->ibody_गणनम् <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			गच्छ बहिर्गमनम्;
		}
		ret = xattr_array_update(h, name, new_मानम्, मानम्_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		यदि (ret)
			गच्छ बहिर्गमनम्;
		गच्छ write_बहिर्गमनम्;
	}

	खण्डम्_free = fs->blocksize;
	खण्डम्_free -= परिमाण(संरचना ext2_ext_attr_शीर्षकम्);
	/* Final null entry. */
	खण्डम्_free -= परिमाण(__u32);
	खण्डम्_free -= space_प्रयुक्तम्(h->attrs + h->ibody_गणनम्,
				 h->गणनम् - h->ibody_गणनम्);

	यदि (ext2fs_has_feature_ea_inode(fs->super) &&
	    मानम्_len > EXT4_XATTR_MIN_LARGE_EA_आकार(fs->blocksize))
		अन्तर्_inode = 1;

	ret = xattr_array_update(h, name, new_मानम्, मानम्_len, ibody_free,
				 खण्डम्_free, old_idx, अन्तर्_inode);
	यदि (ret == EXT2_ET_EA_NO_SPACE && !अन्तर्_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_मानम्, मानम्_len,
			ibody_free, खण्डम्_free, old_idx, 1 /* in_inode */);
	यदि (ret)
		गच्छ बहिर्गमनम्;

write_बहिर्गमनम्:
	ret = ext2fs_xattrs_write(h);
बहिर्गमनम्:
	यदि (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_मानम्);
	प्रत्यागम ret;
}

errcode_t ext2fs_xattr_remove(संरचना ext2_xattr_handle *handle,
			      ध्रुव वर्ण *कुञ्चिका)
{
	संरचना ext2_xattr *x;
	संरचना ext2_xattr *अन्तः = handle->attrs + handle->गणनम्;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	प्रति (x = handle->attrs; x < अन्तः; x++) {
		यदि (वाक्यं_तुलय(x->name, कुञ्चिका) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->मानम्);
			यदि (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (अन्तः - x - 1)*परिमाण(*x));
			स्मृतिं_पूरय(अन्तः - 1, 0, परिमाण(*अन्तः));
			यदि (x < handle->attrs + handle->ibody_गणनम्)
				handle->ibody_गणनम्--;
			handle->गणनम्--;
			प्रत्यागम ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	प्रत्यागम 0;
}

errcode_t ext2fs_xattr_remove_all(संरचना ext2_xattr_handle *handle)
{
	संरचना ext2_xattr *x;
	संरचना ext2_xattr *अन्तः = handle->attrs + handle->गणनम्;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	प्रति (x = handle->attrs; x < अन्तः; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->मानम्);
		यदि (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_गणनम् = 0;
	handle->गणनम् = 0;
	प्रत्यागम ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     संरचना ext2_xattr_handle **handle)
{
	संरचना ext2_xattr_handle *h;
	errcode_t err;

	यदि (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_दत्त(fs->super))
		प्रत्यागम EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(परिमाण(*h), &h);
	यदि (err)
		प्रत्यागम err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->क्षमता = 4;
	err = ext2fs_get_arrayzero(h->क्षमता, परिमाण(संरचना ext2_xattr),
				   &h->attrs);
	यदि (err) {
		ext2fs_free_mem(&h);
		प्रत्यागम err;
	}
	h->गणनम् = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	प्रत्यागम 0;
}

errcode_t ext2fs_xattrs_close(संरचना ext2_xattr_handle **handle)
{
	संरचना ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_कुञ्चिकाः(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	प्रत्यागम 0;
}

errcode_t ext2fs_xattrs_गणनम्(संरचना ext2_xattr_handle *handle, size_t *गणनम्)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*गणनम् = handle->गणनम्;
	प्रत्यागम 0;
}

errcode_t ext2fs_xattrs_flags(संरचना ext2_xattr_handle *handle,
			      अचिह्नित पूर्णाङ्क *new_flags, अचिह्नित पूर्णाङ्क *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	यदि (old_flags)
		*old_flags = handle->flags;
	यदि (new_flags)
		handle->flags = *new_flags;
	प्रत्यागम 0;
}
