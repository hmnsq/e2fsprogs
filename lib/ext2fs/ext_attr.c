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
#endif
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
		फर्काऊ retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	फर्काऊ 0;
}

#define NAME_HASH_SHIFT 5
#define मान_HASH_SHIFT 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_entry(संरचना ext2_ext_attr_entry *entry, शून्य *data)
{
	__u32 hash = 0;
	अचिन्हित वर्ण *name = (((अचिन्हित वर्ण *) entry) +
			       परिमाण(संरचना ext2_ext_attr_entry));
	पूर्णाङ्क n;

	लागि (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*परिमाण(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	यदि (entry->e_मान_inum == 0 && entry->e_मान_size != 0) {
		__u32 *मान = (__u32 *)data;
		लागि (n = (entry->e_मान_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << मान_HASH_SHIFT) ^
			       (hash >> (8*परिमाण(hash) - मान_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*मान++);
		}
	}

	फर्काऊ hash;
}

__u32 ext2fs_ext_attr_hash_entry_signed(संरचना ext2_ext_attr_entry *entry,
					शून्य *data)
{
	__u32 hash = 0;
	चिन्हित वर्ण *name = (((चिन्हित वर्ण *) entry) +
			     परिमाण(संरचना ext2_ext_attr_entry));
	पूर्णाङ्क n;

	लागि (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*परिमाण(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	यदि (entry->e_मान_inum == 0 && entry->e_मान_size != 0) {
		__u32 *मान = (__u32 *)data;
		लागि (n = (entry->e_मान_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << मान_HASH_SHIFT) ^
			       (hash >> (8*परिमाण(hash) - मान_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*मान++);
		}
	}

	फर्काऊ hash;
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
				      संरचना ext2_ext_attr_entry *entry,
				      शून्य *data, __u32 *hash,
				      __u32 *signed_hash)
{
	*hash = ext2fs_ext_attr_hash_entry(entry, data);
	यदि (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_entry_signed(entry, data);

	यदि (entry->e_मान_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, entry->e_मान_inum,
					    &ea_inode_hash);
		यदि (retval)
			फर्काऊ retval;

		*hash = (*hash << मान_HASH_SHIFT) ^
			(*hash >> (8*परिमाण(*hash) - मान_HASH_SHIFT)) ^
			ea_inode_hash;
		यदि (signed_hash)
			*signed_hash = (*signed_hash << मान_HASH_SHIFT) ^
				(*signed_hash >> (8*परिमाण(*hash) -
						  मान_HASH_SHIFT)) ^
				ea_inode_hash;
	}
	फर्काऊ 0;
}

/*
 * ext2fs_ext_attr_hash_entry2()
 *
 * Compute the hash of an extended attribute.
 * This version of the function supports hashing entries that reference
 * external inodes (ea_inode feature).
 */
errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
				      संरचना ext2_ext_attr_entry *entry,
				      शून्य *data, __u32 *hash)
{
	फर्काऊ ext2fs_ext_attr_hash_entry3(fs, entry, data, hash, रिक्त);
}

#undef NAME_HASH_SHIFT
#undef मान_HASH_SHIFT

#define BLOCK_HASH_SHIFT 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
शून्य ext2fs_ext_attr_block_rehash(संरचना ext2_ext_attr_header *header,
				  संरचना ext2_ext_attr_entry *end)
{
	संरचना ext2_ext_attr_entry *here;
	__u32 hash = 0;

	here = (संरचना ext2_ext_attr_entry *)(header+1);
	जबसम्म (here < end && !EXT2_EXT_IS_LAST_ENTRY(here)) {
		यदि (!here->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			रोक;
		}
		hash = (hash << BLOCK_HASH_SHIFT) ^
		       (hash >> (8*परिमाण(hash) - BLOCK_HASH_SHIFT)) ^
		       here->e_hash;
		here = EXT2_EXT_ATTR_NEXT(here);
	}
	header->h_hash = hash;
}

#undef BLOCK_HASH_SHIFT

__u32 ext2fs_get_ea_inode_hash(संरचना ext2_inode *inode)
{
	फर्काऊ inode->i_atime;
}

शून्य ext2fs_set_ea_inode_hash(संरचना ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(संरचना ext2_inode *inode)
{
	फर्काऊ ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_version;
}

शून्य ext2fs_set_ea_inode_ref(संरचना ext2_inode *inode, __u64 ref_गन्ती)
{
	inode->i_ctime = (__u32)(ref_गन्ती >> 32);
	inode->osd1.linux1.l_i_version = (__u32)ref_गन्ती;
}

स्थिर errcode_t check_ext_attr_header(संरचना ext2_ext_attr_header *header)
{
	यदि ((header->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     header->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    header->h_blocks != 1)
		फर्काऊ EXT2_ET_BAD_EA_HEADER;

	फर्काऊ 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block, शून्य *buf,
				ext2_ino_t inum)
{
	पूर्णाङ्क		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	यदि (retval)
		फर्काऊ retval;

	यदि (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_block_csum_verify(fs, inum, block, buf))
		csum_failed = 1;

#ifdef शब्दहरू_BIGENDIAN
	ext2fs_swap_ext_attr(buf, buf, fs->blocksize, 1);
#endif

	retval = check_ext_attr_header(buf);
	यदि (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	फर्काऊ retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block, शून्य *buf)
{
	फर्काऊ ext2fs_read_ext_attr3(fs, block, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, शून्य *buf)
{
	फर्काऊ ext2fs_read_ext_attr2(fs, block, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block, शून्य *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	वर्ण		*write_buf;

#ifdef शब्दहरू_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	यदि (retval)
		फर्काऊ retval;
	ext2fs_swap_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#अन्यथा
	write_buf = (वर्ण *) inbuf;
#endif

	retval = ext2fs_ext_attr_block_csum_set(fs, inum, block,
			(संरचना ext2_ext_attr_header *)write_buf);
	यदि (retval)
		फर्काऊ retval;

	retval = io_channel_write_blk64(fs->io, block, 1, write_buf);
#ifdef शब्दहरू_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#endif
	यदि (!retval)
		ext2fs_mark_changed(fs);
	फर्काऊ retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block, शून्य *inbuf)
{
	फर्काऊ ext2fs_write_ext_attr3(fs, block, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block, शून्य *inbuf)
{
	फर्काऊ ext2fs_write_ext_attr2(fs, block, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    वर्ण *block_buf, पूर्णाङ्क adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	संरचना ext2_ext_attr_header *header;
	वर्ण	*buf = 0;

	यदि ((blk >= ext2fs_blocks_गन्ती(fs->super)) ||
	    (blk < fs->super->s_first_data_block))
		फर्काऊ EXT2_ET_BAD_EA_BLOCK_अङ्क;

	यदि (!block_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		यदि (retval)
			फर्काऊ retval;
		block_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, block_buf, inum);
	यदि (retval)
		गच्छ errout;

	header = (संरचना ext2_ext_attr_header *) block_buf;
	header->h_refcount += adjust;
	यदि (newcount)
		*newcount = header->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, block_buf, inum);
	यदि (retval)
		गच्छ errout;

errout:
	यदि (buf)
		ext2fs_free_mem(&buf);
	फर्काऊ retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    वर्ण *block_buf, पूर्णाङ्क adjust,
				    __u32 *newcount)
{
	फर्काऊ ext2fs_adjust_ea_refcount3(fs, blk, block_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					वर्ण *block_buf, पूर्णाङ्क adjust,
					__u32 *newcount)
{
	फर्काऊ ext2fs_adjust_ea_refcount2(fs, blk, block_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
संरचना ext2_xattr {
	पूर्णाङ्क name_index;
	वर्ण *name;
	वर्ण *short_name;
	शून्य *मान;
	अचिन्हित पूर्णाङ्क मान_len;
	ext2_ino_t ea_ino;
};

संरचना ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	संरचना ext2_xattr *attrs;
	पूर्णाङ्क capacity;
	पूर्णाङ्क गन्ती;
	पूर्णाङ्क ibody_गन्ती;
	ext2_ino_t ino;
	अचिन्हित पूर्णाङ्क flags;
};

स्थिर errcode_t ext2fs_xattrs_expand(संरचना ext2_xattr_handle *h,
				      अचिन्हित पूर्णाङ्क expandby)
{
	संरचना ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->capacity + expandby,
				   परिमाण(संरचना ext2_xattr), &new_attrs);
	यदि (err)
		फर्काऊ err;

	स_नकल(new_attrs, h->attrs, h->capacity * परिमाण(संरचना ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->capacity += expandby;
	h->attrs = new_attrs;

	फर्काऊ 0;
}

संरचना ea_name_index {
	पूर्णाङ्क index;
	अचर वर्ण *name;
};

/* Keep these names sorted in order of decreasing specificity. */
स्थिर संरचना ea_name_index ea_names[] = {
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

स्थिर अचर वर्ण *find_ea_prefix(पूर्णाङ्क index)
{
	संरचना ea_name_index *e;

	लागि (e = ea_names; e->name; e++)
		यदि (e->index == index)
			फर्काऊ e->name;

	फर्काऊ रिक्त;
}

स्थिर पूर्णाङ्क find_ea_index(अचर वर्ण *fullname, अचर वर्ण **name, पूर्णाङ्क *index)
{
	संरचना ea_name_index *e;

	लागि (e = ea_names; e->name; e++) {
		यदि (म_भेदन(fullname, e->name, म_माप(e->name)) == 0) {
			*name = fullname + म_माप(e->name);
			*index = e->index;
			फर्काऊ 1;
		}
	}
	फर्काऊ 0;
}

errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       संरचना ext2_inode_large *inode)
{
	संरचना ext2_ext_attr_header *header;
	शून्य *block_buf = रिक्त;
	blk64_t blk;
	errcode_t err;
	संरचना ext2_inode_large i;

	/* Read inode? */
	यदि (inode == रिक्त) {
		err = ext2fs_read_inode_full(fs, ino, (संरचना ext2_inode *)&i,
					     परिमाण(संरचना ext2_inode_large));
		यदि (err)
			फर्काऊ err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (संरचना ext2_inode *)inode);
	यदि (blk == 0)
		फर्काऊ 0;

	/* Find block, zero it, write back */
	यदि ((blk < fs->super->s_first_data_block) ||
	    (blk >= ext2fs_blocks_गन्ती(fs->super))) {
		err = EXT2_ET_BAD_EA_BLOCK_अङ्क;
		गच्छ out;
	}

	err = ext2fs_get_mem(fs->blocksize, &block_buf);
	यदि (err)
		गच्छ out;

	err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
	यदि (err)
		गच्छ out2;

	/* We only know how to deal with v2 EA blocks */
	header = (संरचना ext2_ext_attr_header *) block_buf;
	यदि (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_HEADER;
		गच्छ out2;
	}

	header->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
	यदि (err)
		गच्छ out2;

	/* Erase link to block */
	ext2fs_file_acl_block_set(fs, (संरचना ext2_inode *)inode, 0);
	यदि (header->h_refcount == 0)
		ext2fs_block_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_blocks(fs, (संरचना ext2_inode *)inode, 1);
	यदि (err)
		गच्छ out2;

	/* Write inode? */
	यदि (inode == &i) {
		err = ext2fs_write_inode_full(fs, ino, (संरचना ext2_inode *)&i,
					      परिमाण(संरचना ext2_inode_large));
		यदि (err)
			गच्छ out2;
	}

out2:
	ext2fs_free_mem(&block_buf);
out:
	फर्काऊ err;
}

स्थिर errcode_t prep_ea_block_for_write(ext2_filsys fs, ext2_ino_t ino,
					 संरचना ext2_inode_large *inode)
{
	संरचना ext2_ext_attr_header *header;
	शून्य *block_buf = रिक्त;
	blk64_t blk, goal;
	errcode_t err;

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (संरचना ext2_inode *)inode);
	यदि (blk != 0) {
		यदि ((blk < fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_गन्ती(fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_अङ्क;
			गच्छ out;
		}

		err = ext2fs_get_mem(fs->blocksize, &block_buf);
		यदि (err)
			गच्छ out;

		err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
		यदि (err)
			गच्छ out2;

		/* We only know how to deal with v2 EA blocks */
		header = (संरचना ext2_ext_attr_header *) block_buf;
		यदि (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			गच्छ out2;
		}

		/* Single-user block.  We're done here. */
		यदि (header->h_refcount == 1)
			गच्छ out2;

		/* We need to CoW the block. */
		header->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
		यदि (err)
			गच्छ out2;
	} अन्यथा {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_add_blocks(fs, (संरचना ext2_inode *)inode,
					     1);
		यदि (err)
			गच्छ out;
	}

	/* Allocate a block */
	goal = ext2fs_find_inode_goal(fs, ino, (संरचना ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, रिक्त, &blk);
	यदि (err)
		गच्छ out2;
	ext2fs_file_acl_block_set(fs, (संरचना ext2_inode *)inode, blk);
out2:
	यदि (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	फर्काऊ err;
}


स्थिर अन्तःस्थ पूर्णाङ्क
posix_acl_xattr_गन्ती(माप_प्रकार size)
{
        यदि (size < परिमाण(posix_acl_xattr_header))
                फर्काऊ -1;
        size -= परिमाण(posix_acl_xattr_header);
        यदि (size % परिमाण(posix_acl_xattr_entry))
                फर्काऊ -1;
        फर्काऊ size / परिमाण(posix_acl_xattr_entry);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
स्थिर errcode_t convert_posix_acl_to_disk_buffer(अचर शून्य *मान, माप_प्रकार size,
						  शून्य *out_buf, माप_प्रकार *size_out)
{
	अचर posix_acl_xattr_header *header =
		(अचर posix_acl_xattr_header*) मान;
	अचर posix_acl_xattr_entry *end, *entry =
		(अचर posix_acl_xattr_entry *)(header+1);
	ext4_acl_header *ext_acl;
	माप_प्रकार s;
	वर्ण *e;

	पूर्णाङ्क गन्ती;

	यदि (!मान)
		फर्काऊ EINVAL;
	यदि (size < परिमाण(posix_acl_xattr_header))
		फर्काऊ ENOMEM;
	यदि (header->a_version != ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION))
		फर्काऊ EINVAL;

	गन्ती = posix_acl_xattr_गन्ती(size);
	ext_acl = out_buf;
	ext_acl->a_version = ext2fs_cpu_to_le32(EXT4_ACL_VERSION);

	यदि (गन्ती <= 0)
		फर्काऊ EINVAL;

	e = (वर्ण *) out_buf + परिमाण(ext4_acl_header);
	s = परिमाण(ext4_acl_header);
	लागि (end = entry + गन्ती; entry != end;entry++) {
		ext4_acl_entry *disk_entry = (ext4_acl_entry*) e;
		disk_entry->e_tag = entry->e_tag;
		disk_entry->e_perm = entry->e_perm;

		विकल्प(ext2fs_le16_to_cpu(entry->e_tag)) {
			पक्ष ACL_USER_OBJ:
			पक्ष ACL_GROUP_OBJ:
			पक्ष ACL_MASK:
			पक्ष ACL_OTHER:
				e += परिमाण(ext4_acl_entry_short);
				s += परिमाण(ext4_acl_entry_short);
				रोक;
			पक्ष ACL_USER:
			पक्ष ACL_GROUP:
				disk_entry->e_id = entry->e_id;
				e += परिमाण(ext4_acl_entry);
				s += परिमाण(ext4_acl_entry);
				रोक;
			स्वभाव:
				फर्काऊ EINVAL;
		}
	}
	*size_out = s;
	फर्काऊ 0;
}

स्थिर errcode_t convert_disk_buffer_to_posix_acl(अचर शून्य *मान, माप_प्रकार size,
						  शून्य **out_buf, माप_प्रकार *size_out)
{
	posix_acl_xattr_header *header;
	posix_acl_xattr_entry *entry;
	अचर ext4_acl_header *ext_acl = (अचर ext4_acl_header *) मान;
	errcode_t err;
	अचर वर्ण *cp;
	वर्ण *out;

	यदि ((!मान) ||
	    (size < परिमाण(ext4_acl_header)) ||
	    (ext_acl->a_version != ext2fs_cpu_to_le32(EXT4_ACL_VERSION)))
		फर्काऊ EINVAL;

	err = ext2fs_get_mem(size * 2, &out);
	यदि (err)
		फर्काऊ err;

	header = (posix_acl_xattr_header *) out;
	header->a_version = ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION);
	entry = (posix_acl_xattr_entry *) (out + परिमाण(posix_acl_xattr_header));

	cp = (अचर वर्ण *) मान + परिमाण(ext4_acl_header);
	size -= परिमाण(ext4_acl_header);

	जबसम्म (size > 0) {
		अचर ext4_acl_entry *disk_entry = (अचर ext4_acl_entry *) cp;

		entry->e_tag = disk_entry->e_tag;
		entry->e_perm = disk_entry->e_perm;

		विकल्प(ext2fs_le16_to_cpu(entry->e_tag)) {
			पक्ष ACL_USER_OBJ:
			पक्ष ACL_GROUP_OBJ:
			पक्ष ACL_MASK:
			पक्ष ACL_OTHER:
				entry->e_id = 0;
				cp += परिमाण(ext4_acl_entry_short);
				size -= परिमाण(ext4_acl_entry_short);
				रोक;
			पक्ष ACL_USER:
			पक्ष ACL_GROUP:
				entry->e_id = disk_entry->e_id;
				cp += परिमाण(ext4_acl_entry);
				size -= परिमाण(ext4_acl_entry);
				रोक;
			स्वभाव:
				ext2fs_free_mem(&out);
				फर्काऊ EINVAL;
		}
		entry++;
	}
	*out_buf = out;
	*size_out = ((वर्ण *) entry - out);
	फर्काऊ 0;
}

स्थिर errcode_t
write_xattrs_to_buffer(ext2_filsys fs, संरचना ext2_xattr *attrs, पूर्णाङ्क गन्ती,
		       शून्य *entries_start, अचिन्हित पूर्णाङ्क storage_size,
		       अचिन्हित पूर्णाङ्क मान_offset_correction, पूर्णाङ्क write_hash)
{
	संरचना ext2_xattr *x;
	संरचना ext2_ext_attr_entry *e = entries_start;
	वर्ण *end = (वर्ण *) entries_start + storage_size;
	अचिन्हित पूर्णाङ्क मान_size;
	errcode_t err;

	स_रखो(entries_start, 0, storage_size);
	लागि (x = attrs; x < attrs + गन्ती; x++) {
		मान_size = ((x->मान_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = म_माप(x->short_name);
		e->e_name_index = x->name_index;

		e->e_मान_size = x->मान_len;
		e->e_मान_inum = x->ea_ino;

		/* Store name */
		स_नकल((वर्ण *)e + परिमाण(*e), x->short_name, e->e_name_len);
		यदि (x->ea_ino) {
			e->e_मान_offs = 0;
		} अन्यथा {
			end -= मान_size;
			e->e_मान_offs = end - (वर्ण *) entries_start +
						मान_offset_correction;
			स_नकल(end, x->मान, e->e_मान_size);
		}

		यदि (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : end,
							  &e->e_hash);
			यदि (err)
				फर्काऊ err;
		} अन्यथा
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_NEXT(e);
		*(__u32 *)e = 0;
	}
	फर्काऊ 0;
}

errcode_t ext2fs_xattrs_write(संरचना ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	अचर अचिन्हित पूर्णाङ्क inode_size = EXT2_INODE_SIZE(fs->super);
	संरचना ext2_inode_large *inode;
	वर्ण *start, *block_buf = रिक्त;
	संरचना ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	blk64_t blk;
	अचिन्हित पूर्णाङ्क storage_size;
	अचिन्हित पूर्णाङ्क i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_size;
	यदि (i < परिमाण(*inode))
		i = परिमाण(*inode);
	err = ext2fs_get_memzero(i, &inode);
	यदि (err)
		फर्काऊ err;

	err = ext2fs_read_inode_full(fs, handle->ino, EXT2_INODE(inode),
				     inode_size);
	यदि (err)
		गच्छ out;

	/* If extra_isize isn't set, we need to set it now */
	यदि (inode->i_extra_isize == 0 &&
	    inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		वर्ण *p = (वर्ण *)inode;
		माप_प्रकार extra = fs->super->s_want_extra_isize;

		यदि (extra == 0)
			extra = परिमाण(__u32);
		स_रखो(p + EXT2_GOOD_OLD_INODE_SIZE, 0, extra);
		inode->i_extra_isize = extra;
	}
	यदि (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		गच्छ out;
	}

	/* Does the inode have space for EA? */
	यदि (inode->i_extra_isize < परिमाण(inode->i_extra_isize) ||
	    inode_size <= EXT2_GOOD_OLD_INODE_SIZE + inode->i_extra_isize +
								परिमाण(__u32))
		गच्छ write_ea_block;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	स_नकल(((वर्ण *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, &ea_inode_magic, परिमाण(__u32));
	storage_size = inode_size - EXT2_GOOD_OLD_INODE_SIZE -
				inode->i_extra_isize - परिमाण(__u32);
	start = ((वर्ण *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
				inode->i_extra_isize + परिमाण(__u32);

	err = write_xattrs_to_buffer(fs, handle->attrs, handle->ibody_गन्ती,
				     start, storage_size, 0, 0);
	यदि (err)
		गच्छ out;
write_ea_block:
	/* Are we done? */
	यदि (handle->ibody_गन्ती == handle->गन्ती &&
	    !ext2fs_file_acl_block(fs, EXT2_INODE(inode)))
		गच्छ skip_ea_block;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &block_buf);
	यदि (err)
		गच्छ out;

	storage_size = fs->blocksize - परिमाण(संरचना ext2_ext_attr_header);
	start = block_buf + परिमाण(संरचना ext2_ext_attr_header);

	err = write_xattrs_to_buffer(fs, handle->attrs + handle->ibody_गन्ती,
				     handle->गन्ती - handle->ibody_गन्ती, start,
				     storage_size, start - block_buf, 1);
	यदि (err)
		गच्छ out2;

	/* Write a header on the EA block */
	header = (संरचना ext2_ext_attr_header *) block_buf;
	header->h_magic = EXT2_EXT_ATTR_MAGIC;
	header->h_refcount = 1;
	header->h_blocks = 1;

	/* Get a new block for writing */
	err = prep_ea_block_for_write(fs, handle->ino, inode);
	यदि (err)
		गच्छ out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_block(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, handle->ino);
	यदि (err)
		गच्छ out2;

skip_ea_block:
	blk = ext2fs_file_acl_block(fs, (संरचना ext2_inode *)inode);
	यदि (!block_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		यदि (err)
			गच्छ out;
	}

	/* Write the inode */
	err = ext2fs_write_inode_full(fs, handle->ino, EXT2_INODE(inode),
				      inode_size);
	यदि (err)
		गच्छ out2;

out2:
	ext2fs_free_mem(&block_buf);
out:
	ext2fs_free_mem(&inode);
	फर्काऊ err;
}

स्थिर errcode_t read_xattrs_from_buffer(संरचना ext2_xattr_handle *handle,
					 संरचना ext2_inode_large *inode,
					 संरचना ext2_ext_attr_entry *entries,
					 अचिन्हित पूर्णाङ्क storage_size,
					 वर्ण *मान_start)
{
	संरचना ext2_xattr *x;
	संरचना ext2_ext_attr_entry *entry, *end;
	अचर वर्ण *prefix;
	अचिन्हित पूर्णाङ्क remain, prefix_len;
	errcode_t err;
	अचिन्हित पूर्णाङ्क मानहरू_size = storage_size +
			((वर्ण *)entries - मान_start);

	/* find the end */
	end = entries;
	remain = storage_size;
	जबसम्म (remain >= परिमाण(संरचना ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(end)) {

		/* header eats this space */
		remain -= परिमाण(संरचना ext2_ext_attr_entry);

		/* is attribute name valid? */
		यदि (EXT2_EXT_ATTR_SIZE(end->e_name_len) > remain)
			फर्काऊ EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(end->e_name_len);
		end = EXT2_EXT_ATTR_NEXT(end);
	}

	entry = entries;
	remain = storage_size;
	जबसम्म (remain >= परिमाण(संरचना ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(entry)) {

		/* Allocate space for more attrs? */
		यदि (handle->गन्ती == handle->capacity) {
			err = ext2fs_xattrs_expand(handle, 4);
			यदि (err)
				फर्काऊ err;
		}

		x = handle->attrs + handle->गन्ती;

		/* header eats this space */
		remain -= परिमाण(संरचना ext2_ext_attr_entry);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(entry->e_name_len);

		/* Extract name */
		prefix = find_ea_prefix(entry->e_name_index);
		prefix_len = (prefix ? म_माप(prefix) : 0);
		err = ext2fs_get_memzero(entry->e_name_len + prefix_len + 1,
					 &x->name);
		यदि (err)
			फर्काऊ err;
		यदि (prefix)
			स_नकल(x->name, prefix, prefix_len);
		यदि (entry->e_name_len)
			स_नकल(x->name + prefix_len,
			       (वर्ण *)entry + परिमाण(*entry),
			       entry->e_name_len);
		x->short_name = x->name + prefix_len;
		x->name_index = entry->e_name_index;

		/* Check & copy value */
		यदि (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    entry->e_मान_inum != 0)
			फर्काऊ EXT2_ET_BAD_EA_BLOCK_अङ्क;

		यदि (entry->e_मान_inum == 0) {
			यदि (entry->e_मान_size > remain)
				फर्काऊ EXT2_ET_EA_BAD_मान_SIZE;

			यदि (entry->e_मान_offs + entry->e_मान_size > मानहरू_size)
				फर्काऊ EXT2_ET_EA_BAD_मान_OFFSET;

			यदि (entry->e_मान_size > 0 &&
			    मान_start + entry->e_मान_offs <
			    (वर्ण *)end + परिमाण(__u32))
				फर्काऊ EXT2_ET_EA_BAD_मान_OFFSET;

			remain -= entry->e_मान_size;

			err = ext2fs_get_mem(entry->e_मान_size, &x->मान);
			यदि (err)
				फर्काऊ err;
			स_नकल(x->मान, मान_start + entry->e_मान_offs,
			       entry->e_मान_size);
		} अन्यथा {
			संरचना ext2_inode *ea_inode;
			ext2_file_t ea_file;

			यदि (entry->e_मान_offs != 0)
				फर्काऊ EXT2_ET_EA_BAD_मान_OFFSET;

			यदि (entry->e_मान_size > (64 * 1024))
				फर्काऊ EXT2_ET_EA_BAD_मान_SIZE;

			err = ext2fs_get_mem(entry->e_मान_size, &x->मान);
			यदि (err)
				फर्काऊ err;

			err = ext2fs_file_open(handle->fs, entry->e_मान_inum,
					       0, &ea_file);
			यदि (err)
				फर्काऊ err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			यदि ((ea_inode->i_flags & EXT4_INLINE_DATA_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_गन्ती == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			अन्यथा यदि ((__u64) ext2fs_file_get_size(ea_file) !=
				 entry->e_मान_size)
				err = EXT2_ET_EA_BAD_मान_SIZE;
			अन्यथा
				err = ext2fs_file_read(ea_file, x->मान,
						       entry->e_मान_size, 0);
			ext2fs_file_close(ea_file);
			यदि (err)
				फर्काऊ err;
		}

		x->ea_ino = entry->e_मान_inum;
		x->मान_len = entry->e_मान_size;

		/* e_hash may be 0 in older inode's ea */
		यदि (entry->e_hash != 0) {
			__u32 hash, signed_hash;

			शून्य *data = (entry->e_मान_inum != 0) ?
					0 : मान_start + entry->e_मान_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, entry,
							  data, &hash,
							  &signed_hash);
			यदि (err)
				फर्काऊ err;
			यदि ((entry->e_hash != hash) &&
			    (entry->e_hash != signed_hash)) {
				संरचना ext2_inode child;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							entry->e_मान_inum,
							&child);
				यदि (err)
					फर्काऊ err;
				यदि (child.i_mtime != handle->ino ||
				    child.i_generation != inode->i_generation)
					फर्काऊ EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->गन्ती++;
		entry = EXT2_EXT_ATTR_NEXT(entry);
	}

	फर्काऊ 0;
}

स्थिर शून्य xattrs_free_साँचोहरू(संरचना ext2_xattr_handle *h)
{
	संरचना ext2_xattr *a = h->attrs;
	पूर्णाङ्क i;

	लागि (i = 0; i < h->capacity; i++) {
		यदि (a[i].name)
			ext2fs_free_mem(&a[i].name);
		यदि (a[i].मान)
			ext2fs_free_mem(&a[i].मान);
	}
	h->गन्ती = 0;
	h->ibody_गन्ती = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(संरचना ext2_xattr_handle *handle,
				   संरचना ext2_inode_large *inode)
{
	संरचना ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	अचिन्हित पूर्णाङ्क storage_size;
	वर्ण *start, *block_buf = रिक्त;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_साँचोहरू(handle);

	/* Does the inode have space for EA? */
	यदि (inode->i_extra_isize < परिमाण(inode->i_extra_isize) ||
	    EXT2_INODE_SIZE(handle->fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  परिमाण(__u32))
		गच्छ read_ea_block;
	यदि (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		गच्छ out;
	}

	/* Look for EA in the inode */
	स_नकल(&ea_inode_magic, ((वर्ण *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, परिमाण(__u32));
	यदि (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_size = EXT2_INODE_SIZE(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_SIZE - inode->i_extra_isize -
			परिमाण(__u32);
		start = ((वर्ण *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + परिमाण(__u32);

		err = read_xattrs_from_buffer(handle, inode,
					(संरचना ext2_ext_attr_entry *) start,
					storage_size, start);
		यदि (err)
			गच्छ out;

		handle->ibody_गन्ती = handle->गन्ती;
	}

read_ea_block:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_block(handle->fs, EXT2_INODE(inode));
	यदि (blk != 0) {
		यदि ((blk < handle->fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_गन्ती(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_अङ्क;
			गच्छ out;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &block_buf);
		यदि (err)
			गच्छ out;

		err = ext2fs_read_ext_attr3(handle->fs, blk, block_buf,
					    handle->ino);
		यदि (err)
			गच्छ out3;

		/* We only know how to deal with v2 EA blocks */
		header = (संरचना ext2_ext_attr_header *) block_buf;
		यदि (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			गच्छ out3;
		}

		/* Read EAs */
		storage_size = handle->fs->blocksize -
			परिमाण(संरचना ext2_ext_attr_header);
		start = block_buf + परिमाण(संरचना ext2_ext_attr_header);
		err = read_xattrs_from_buffer(handle, inode,
					(संरचना ext2_ext_attr_entry *) start,
					storage_size, block_buf);
	}

out3:
	यदि (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	फर्काऊ err;
}

errcode_t ext2fs_xattrs_read(संरचना ext2_xattr_handle *handle)
{
	संरचना ext2_inode_large *inode;
	माप_प्रकार inode_size = EXT2_INODE_SIZE(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	यदि (inode_size < परिमाण(*inode))
		inode_size = परिमाण(*inode);
	err = ext2fs_get_memzero(inode_size, &inode);
	यदि (err)
		फर्काऊ err;

	err = ext2fs_read_inode_full(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_SIZE(handle->fs->super));
	यदि (err)
		गच्छ out;

	err = ext2fs_xattrs_read_inode(handle, inode);

out:
	ext2fs_free_mem(&inode);

	फर्काऊ err;
}

errcode_t ext2fs_xattrs_iterate(संरचना ext2_xattr_handle *h,
				पूर्णाङ्क (*func)(वर्ण *name, वर्ण *मान,
					    माप_प्रकार मान_len, शून्य *data),
				शून्य *data)
{
	संरचना ext2_xattr *x;
	पूर्णाङ्क dirty = 0;
	पूर्णाङ्क ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	लागि (x = h->attrs; x < h->attrs + h->गन्ती; x++) {
		ret = func(x->name, x->मान, x->मान_len, data);
		यदि (ret & XATTR_CHANGED)
			dirty = 1;
		यदि (ret & XATTR_ABORT)
			रोक;
	}

	यदि (dirty)
		फर्काऊ ext2fs_xattrs_write(h);
	फर्काऊ 0;
}

errcode_t ext2fs_xattr_get(संरचना ext2_xattr_handle *h, अचर वर्ण *साँचो,
			   शून्य **मान, माप_प्रकार *मान_len)
{
	संरचना ext2_xattr *x;
	वर्ण *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	लागि (x = h->attrs; x < h->attrs + h->गन्ती; x++) {
		यदि (म_भेद(x->name, साँचो))
			जारी;

		यदि (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((म_भेद(साँचो, "system.posix_acl_default") == 0) ||
		     (म_भेद(साँचो, "system.posix_acl_access") == 0))) {
			err = convert_disk_buffer_to_posix_acl(x->मान, x->मान_len,
							       मान, मान_len);
			फर्काऊ err;
		} अन्यथा {
			err = ext2fs_get_mem(x->मान_len, &val);
			यदि (err)
				फर्काऊ err;
			स_नकल(val, x->मान, x->मान_len);
			*मान = val;
			*मान_len = x->मान_len;
			फर्काऊ 0;
		}
	}

	फर्काऊ EXT2_ET_EA_साँचो_NOT_FOUND;
}

errcode_t ext2fs_xattr_inode_max_size(ext2_filsys fs, ext2_ino_t ino,
				      माप_प्रकार *size)
{
	संरचना ext2_ext_attr_entry *entry;
	संरचना ext2_inode_large *inode;
	__u32 ea_inode_magic;
	अचिन्हित पूर्णाङ्क minoff;
	वर्ण *start;
	माप_प्रकार i;
	errcode_t err;

	i = EXT2_INODE_SIZE(fs->super);
	यदि (i < परिमाण(*inode))
		i = परिमाण(*inode);
	err = ext2fs_get_memzero(i, &inode);
	यदि (err)
		फर्काऊ err;

	err = ext2fs_read_inode_full(fs, ino, (संरचना ext2_inode *)inode,
				     EXT2_INODE_SIZE(fs->super));
	यदि (err)
		गच्छ out;

	/* Does the inode have size for EA? */
	यदि (EXT2_INODE_SIZE(fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  परिमाण(__u32)) {
		err = EXT2_ET_INLINE_DATA_NO_SPACE;
		गच्छ out;
	}

	minoff = EXT2_INODE_SIZE(fs->super) - परिमाण(*inode) - परिमाण(__u32);
	स_नकल(&ea_inode_magic, ((वर्ण *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, परिमाण(__u32));
	यदि (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		start= ((वर्ण *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + परिमाण(__u32);
		entry = (संरचना ext2_ext_attr_entry *) start;
		जबसम्म (!EXT2_EXT_IS_LAST_ENTRY(entry)) {
			यदि (!entry->e_मान_inum && entry->e_मान_size) {
				अचिन्हित पूर्णाङ्क offs = entry->e_मान_offs;
				यदि (offs < minoff)
					minoff = offs;
			}
			entry = EXT2_EXT_ATTR_NEXT(entry);
		}
		*size = minoff - ((वर्ण *)entry - (वर्ण *)start) - परिमाण(__u32);
	} अन्यथा {
		/* no xattr.  return a maximum size */
		*size = EXT2_EXT_ATTR_SIZE(minoff -
					   EXT2_EXT_ATTR_LEN(म_माप("data")) -
					   EXT2_EXT_ATTR_ROUND - परिमाण(__u32));
	}

out:
	ext2fs_free_mem(&inode);
	फर्काऊ err;
}

स्थिर errcode_t xattr_create_ea_inode(ext2_filsys fs, अचर शून्य *मान,
				       माप_प्रकार मान_len, ext2_ino_t *ea_ino)
{
	संरचना ext2_inode inode;
	ext2_ino_t ino;
	ext2_file_t file;
	__u32 hash;
	errcode_t ret;

	ret = ext2fs_new_inode(fs, 0, 0, 0, &ino);
	यदि (ret)
		फर्काऊ ret;

	स_रखो(&inode, 0, परिमाण(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	यदि (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_size = 0;
	inode.i_mode = LINUX_S_IFREG | 0600;
	inode.i_links_गन्ती = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	यदि (ret)
		फर्काऊ ret;
	/*
	 * ref_count and hash utilize inode's i_*time fields.
	 * ext2fs_write_new_inode() call above initializes these fields with
	 * current time. That's why ref count and hash updates are done
	 * separately below.
	 */
	ext2fs_set_ea_inode_ref(&inode, 1);
	hash = ext2fs_crc32c_le(fs->csum_seed, मान, मान_len);
	ext2fs_set_ea_inode_hash(&inode, hash);

	ret = ext2fs_write_inode(fs, ino, &inode);
	यदि (ret)
		फर्काऊ ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	यदि (ret)
		फर्काऊ ret;
	ret = ext2fs_file_write(file, मान, मान_len, रिक्त);
	ext2fs_file_close(file);
	यदि (ret)
		फर्काऊ ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	फर्काऊ 0;
}

स्थिर errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	संरचना ext2_inode_large inode;
	__u64 ref_गन्ती;
	errcode_t ret;

	ret = ext2fs_read_inode_full(fs, ino, (संरचना ext2_inode *)&inode,
				     परिमाण(inode));
	यदि (ret)
		गच्छ out;

	ref_गन्ती = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_गन्ती--;
	ext2fs_set_ea_inode_ref(EXT2_INODE(&inode), ref_गन्ती);

	यदि (ref_गन्ती)
		गच्छ write_out;

	inode.i_links_गन्ती = 0;
	ext2fs_set_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	यदि (ret)
		गच्छ write_out;

	यदि (ext2fs_inode_has_valid_blocks2(fs, (संरचना ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (संरचना ext2_inode *)&inode, रिक्त,
				   0, ~0ULL);
		यदि (ret)
			गच्छ out;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_out:
	ret = ext2fs_write_inode_full(fs, ino, (संरचना ext2_inode *)&inode,
				      परिमाण(inode));
out:
	फर्काऊ ret;
}

स्थिर errcode_t xattr_update_entry(ext2_filsys fs, संरचना ext2_xattr *x,
				    अचर वर्ण *name, अचर वर्ण *short_name,
				    पूर्णाङ्क index, अचर शून्य *मान,
				    माप_प्रकार मान_len, पूर्णाङ्क in_inode)
{
	ext2_ino_t ea_ino = 0;
	शून्य *new_मान = रिक्त;
	वर्ण *new_name = रिक्त;
	पूर्णाङ्क name_len;
	errcode_t ret;

	यदि (!x->name) {
		name_len = म_माप(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		यदि (ret)
			गच्छ fail;
		स_नकल(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(मान_len, &new_मान);
	यदि (ret)
		गच्छ fail;
	स_नकल(new_मान, मान, मान_len);

	यदि (in_inode) {
		ret = xattr_create_ea_inode(fs, मान, मान_len, &ea_ino);
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
	x->name_index = index;

	यदि (x->मान)
		ext2fs_free_mem(&x->मान);
	x->मान = new_मान;
	x->मान_len = मान_len;
	x->ea_ino = ea_ino;
	फर्काऊ 0;
fail:
	यदि (new_name)
		ext2fs_free_mem(&new_name);
	यदि (new_मान)
		ext2fs_free_mem(&new_मान);
	यदि (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	फर्काऊ ret;
}

स्थिर पूर्णाङ्क xattr_find_position(संरचना ext2_xattr *attrs, पूर्णाङ्क गन्ती,
			       अचर वर्ण *shortname, पूर्णाङ्क name_idx)
{
	संरचना ext2_xattr *x;
	पूर्णाङ्क i;
	पूर्णाङ्क shortname_len, x_shortname_len;

	shortname_len = म_माप(shortname);

	लागि (i = 0, x = attrs; i < गन्ती; i++, x++) {
		यदि (name_idx < x->name_index)
			रोक;
		यदि (name_idx > x->name_index)
			जारी;

		x_shortname_len = म_माप(x->short_name);
		यदि (shortname_len < x_shortname_len)
			रोक;
		यदि (shortname_len > x_shortname_len)
			जारी;

		यदि (स_भेद(shortname, x->short_name, shortname_len) <= 0)
			रोक;
	}
	फर्काऊ i;
}

स्थिर errcode_t xattr_array_update(संरचना ext2_xattr_handle *h,
				    अचर वर्ण *name,
				    अचर शून्य *मान, माप_प्रकार मान_len,
				    पूर्णाङ्क ibody_free, पूर्णाङ्क block_free,
				    पूर्णाङ्क old_idx, पूर्णाङ्क in_inode)
{
	संरचना ext2_xattr tmp;
	पूर्णाङ्क add_to_ibody;
	पूर्णाङ्क needed;
	पूर्णाङ्क name_len, name_idx = 0;
	अचर वर्ण *shortname = name;
	पूर्णाङ्क new_idx;
	पूर्णाङ्क ret;

	find_ea_index(name, &shortname, &name_idx);
	name_len = म_माप(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	यदि (!in_inode)
		needed += EXT2_EXT_ATTR_SIZE(मान_len);

	यदि (old_idx >= 0 && old_idx < h->ibody_गन्ती) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		यदि (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_SIZE(
						h->attrs[old_idx].मान_len);
	}

	यदि (needed <= ibody_free) {
		यदि (old_idx < 0) {
			new_idx = h->ibody_गन्ती;
			add_to_ibody = 1;
			गच्छ add_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, मान,
					 मान_len, in_inode);
		यदि (ret)
			फर्काऊ ret;
		यदि (h->ibody_गन्ती <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			स_हटाओ(h->attrs + h->ibody_गन्ती + 1,
				h->attrs + h->ibody_गन्ती,
				(old_idx - h->ibody_गन्ती) * परिमाण(*h->attrs));
			h->attrs[h->ibody_गन्ती] = tmp;
			h->ibody_गन्ती++;
		}
		फर्काऊ 0;
	}

	यदि (h->ibody_गन्ती <= old_idx) {
		block_free += EXT2_EXT_ATTR_LEN(name_len);
		यदि (!h->attrs[old_idx].ea_ino)
			block_free +=
				EXT2_EXT_ATTR_SIZE(h->attrs[old_idx].मान_len);
	}

	यदि (needed > block_free)
		फर्काऊ EXT2_ET_EA_NO_SPACE;

	यदि (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, मान,
					 मान_len, in_inode);
		यदि (ret)
			फर्काऊ ret;
		यदि (old_idx < h->ibody_गन्ती) {
			/*
			 * Move entry from ibody to the block. Note that
			 * entries in the block are sorted.
			 */
			new_idx = xattr_find_position(h->attrs + h->ibody_गन्ती,
						      h->गन्ती - h->ibody_गन्ती,
						      shortname, name_idx);
			new_idx += h->ibody_गन्ती - 1;
			tmp = h->attrs[old_idx];
			स_हटाओ(h->attrs + old_idx, h->attrs + old_idx + 1,
				(new_idx - old_idx) * परिमाण(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_गन्ती--;
		}
		फर्काऊ 0;
	}

	new_idx = xattr_find_position(h->attrs + h->ibody_गन्ती,
				      h->गन्ती - h->ibody_गन्ती,
				      shortname, name_idx);
	new_idx += h->ibody_गन्ती;
	add_to_ibody = 0;

add_new:
	यदि (h->गन्ती == h->capacity) {
		ret = ext2fs_xattrs_expand(h, 4);
		यदि (ret)
			फर्काऊ ret;
	}

	ret = xattr_update_entry(h->fs, &h->attrs[h->गन्ती], name, shortname,
				 name_idx, मान, मान_len, in_inode);
	यदि (ret)
		फर्काऊ ret;

	tmp = h->attrs[h->गन्ती];
	स_हटाओ(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->गन्ती - new_idx)*परिमाण(*h->attrs));
	h->attrs[new_idx] = tmp;
	यदि (add_to_ibody)
		h->ibody_गन्ती++;
	h->गन्ती++;
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क space_used(संरचना ext2_xattr *attrs, पूर्णाङ्क गन्ती)
{
	पूर्णाङ्क total = 0;
	संरचना ext2_xattr *x;
	पूर्णाङ्क i, len;

	लागि (i = 0, x = attrs; i < गन्ती; i++, x++) {
		len = म_माप(x->short_name);
		total += EXT2_EXT_ATTR_LEN(len);
		यदि (!x->ea_ino)
			total += EXT2_EXT_ATTR_SIZE(x->मान_len);
	}
	फर्काऊ total;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#define EXT4_XATTR_MIN_LARGE_EA_SIZE(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - परिमाण(संरचना ext2_ext_attr_header) - 4)

errcode_t ext2fs_xattr_set(संरचना ext2_xattr_handle *h,
			   अचर वर्ण *name,
			   अचर शून्य *मान,
			   माप_प्रकार मान_len)
{
	ext2_filsys fs = h->fs;
	अचर पूर्णाङ्क inode_size = EXT2_INODE_SIZE(fs->super);
	संरचना ext2_inode_large *inode = रिक्त;
	संरचना ext2_xattr *x;
	वर्ण *new_मान;
	पूर्णाङ्क ibody_free, block_free;
	पूर्णाङ्क in_inode = 0;
	पूर्णाङ्क old_idx = -1;
	पूर्णाङ्क extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(मान_len, &new_मान);
	यदि (ret)
		फर्काऊ ret;
	यदि (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((म_भेद(name, "system.posix_acl_default") == 0) ||
	     (म_भेद(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_to_disk_buffer(मान, मान_len,
						       new_मान, &मान_len);
		यदि (ret)
			गच्छ out;
	} अन्यथा यदि (मान_len)
		स_नकल(new_मान, मान, मान_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	लागि (x = h->attrs; x < h->attrs + h->गन्ती; x++) {
		यदि (!म_भेद(x->name, name)) {
			यदि (!x->ea_ino && x->मान_len == मान_len &&
			    (!मान_len ||
			     !स_भेद(x->मान, new_मान, मान_len))) {
				ret = 0;
				गच्छ out;
			}
			old_idx = x - h->attrs;
			रोक;
		}
	}

	ret = ext2fs_get_memzero(inode_size, &inode);
	यदि (ret)
		गच्छ out;
	ret = ext2fs_read_inode_full(fs, h->ino,
				     (संरचना ext2_inode *)inode,
				     inode_size);
	यदि (ret)
		गच्छ out;
	यदि (inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		extra_isize = inode->i_extra_isize;
		यदि (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			यदि (extra_isize == 0)
				extra_isize = परिमाण(__u32);
		}
		ibody_free = inode_size - EXT2_GOOD_OLD_INODE_SIZE;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= परिमाण(__u32) * 2;
		ibody_free -= space_used(h->attrs, h->ibody_गन्ती);
	} अन्यथा
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	यदि (म_भेद(name, "system.data") == 0) {
		यदि (h->ibody_गन्ती <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			गच्छ out;
		}
		ret = xattr_array_update(h, name, new_मान, मान_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		यदि (ret)
			गच्छ out;
		गच्छ write_out;
	}

	block_free = fs->blocksize;
	block_free -= परिमाण(संरचना ext2_ext_attr_header);
	/* Final null entry. */
	block_free -= परिमाण(__u32);
	block_free -= space_used(h->attrs + h->ibody_गन्ती,
				 h->गन्ती - h->ibody_गन्ती);

	यदि (ext2fs_has_feature_ea_inode(fs->super) &&
	    मान_len > EXT4_XATTR_MIN_LARGE_EA_SIZE(fs->blocksize))
		in_inode = 1;

	ret = xattr_array_update(h, name, new_मान, मान_len, ibody_free,
				 block_free, old_idx, in_inode);
	यदि (ret == EXT2_ET_EA_NO_SPACE && !in_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_मान, मान_len,
			ibody_free, block_free, old_idx, 1 /* in_inode */);
	यदि (ret)
		गच्छ out;

write_out:
	ret = ext2fs_xattrs_write(h);
out:
	यदि (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_मान);
	फर्काऊ ret;
}

errcode_t ext2fs_xattr_remove(संरचना ext2_xattr_handle *handle,
			      अचर वर्ण *साँचो)
{
	संरचना ext2_xattr *x;
	संरचना ext2_xattr *end = handle->attrs + handle->गन्ती;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	लागि (x = handle->attrs; x < end; x++) {
		यदि (म_भेद(x->name, साँचो) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->मान);
			यदि (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			स_हटाओ(x, x + 1, (end - x - 1)*परिमाण(*x));
			स_रखो(end - 1, 0, परिमाण(*end));
			यदि (x < handle->attrs + handle->ibody_गन्ती)
				handle->ibody_गन्ती--;
			handle->गन्ती--;
			फर्काऊ ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	फर्काऊ 0;
}

errcode_t ext2fs_xattr_remove_all(संरचना ext2_xattr_handle *handle)
{
	संरचना ext2_xattr *x;
	संरचना ext2_xattr *end = handle->attrs + handle->गन्ती;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	लागि (x = handle->attrs; x < end; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->मान);
		यदि (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_गन्ती = 0;
	handle->गन्ती = 0;
	फर्काऊ ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     संरचना ext2_xattr_handle **handle)
{
	संरचना ext2_xattr_handle *h;
	errcode_t err;

	यदि (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_data(fs->super))
		फर्काऊ EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(परिमाण(*h), &h);
	यदि (err)
		फर्काऊ err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->capacity = 4;
	err = ext2fs_get_arrayzero(h->capacity, परिमाण(संरचना ext2_xattr),
				   &h->attrs);
	यदि (err) {
		ext2fs_free_mem(&h);
		फर्काऊ err;
	}
	h->गन्ती = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	फर्काऊ 0;
}

errcode_t ext2fs_xattrs_close(संरचना ext2_xattr_handle **handle)
{
	संरचना ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_साँचोहरू(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	फर्काऊ 0;
}

errcode_t ext2fs_xattrs_गन्ती(संरचना ext2_xattr_handle *handle, माप_प्रकार *गन्ती)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*गन्ती = handle->गन्ती;
	फर्काऊ 0;
}

errcode_t ext2fs_xattrs_flags(संरचना ext2_xattr_handle *handle,
			      अचिन्हित पूर्णाङ्क *new_flags, अचिन्हित पूर्णाङ्क *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	यदि (old_flags)
		*old_flags = handle->flags;
	यदि (new_flags)
		handle->flags = *new_flags;
	फर्काऊ 0;
}
