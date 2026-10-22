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
#जइ HAVE_UNISTD_H
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
	जइ (retval)
		प्रत्यागम retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	प्रत्यागम 0;
}

#define NAME_HASH_SHIFT 5
#define मानम्_HASH_SHIFT 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_entry(संरचना ext2_ext_attr_entry *entry, शून्य *data)
{
	__u32 hash = 0;
	unsigned वर्ण *name = (((unsigned वर्ण *) entry) +
			       परिमाण(संरचना ext2_ext_attr_entry));
	पूर्णाङ्क n;

	प्रति (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*परिमाण(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	जइ (entry->e_मानम्_inum == 0 && entry->e_मानम्_size != 0) {
		__u32 *मानम् = (__u32 *)data;
		प्रति (n = (entry->e_मानम्_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << मानम्_HASH_SHIFT) ^
			       (hash >> (8*परिमाण(hash) - मानम्_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*मानम्++);
		}
	}

	प्रत्यागम hash;
}

__u32 ext2fs_ext_attr_hash_entry_signed(संरचना ext2_ext_attr_entry *entry,
					शून्य *data)
{
	__u32 hash = 0;
	signed वर्ण *name = (((signed वर्ण *) entry) +
			     परिमाण(संरचना ext2_ext_attr_entry));
	पूर्णाङ्क n;

	प्रति (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*परिमाण(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	जइ (entry->e_मानम्_inum == 0 && entry->e_मानम्_size != 0) {
		__u32 *मानम् = (__u32 *)data;
		प्रति (n = (entry->e_मानम्_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << मानम्_HASH_SHIFT) ^
			       (hash >> (8*परिमाण(hash) - मानम्_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*मानम्++);
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
				      संरचना ext2_ext_attr_entry *entry,
				      शून्य *data, __u32 *hash,
				      __u32 *signed_hash)
{
	*hash = ext2fs_ext_attr_hash_entry(entry, data);
	जइ (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_entry_signed(entry, data);

	जइ (entry->e_मानम्_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, entry->e_मानम्_inum,
					    &ea_inode_hash);
		जइ (retval)
			प्रत्यागम retval;

		*hash = (*hash << मानम्_HASH_SHIFT) ^
			(*hash >> (8*परिमाण(*hash) - मानम्_HASH_SHIFT)) ^
			ea_inode_hash;
		जइ (signed_hash)
			*signed_hash = (*signed_hash << मानम्_HASH_SHIFT) ^
				(*signed_hash >> (8*परिमाण(*hash) -
						  मानम्_HASH_SHIFT)) ^
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
				      संरचना ext2_ext_attr_entry *entry,
				      शून्य *data, __u32 *hash)
{
	प्रत्यागम ext2fs_ext_attr_hash_entry3(fs, entry, data, hash, रिक्त);
}

#undef NAME_HASH_SHIFT
#undef मानम्_HASH_SHIFT

#define BLOCK_HASH_SHIFT 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
शून्य ext2fs_ext_attr_block_rehash(संरचना ext2_ext_attr_header *header,
				  संरचना ext2_ext_attr_entry *end)
{
	संरचना ext2_ext_attr_entry *here;
	__u32 hash = 0;

	here = (संरचना ext2_ext_attr_entry *)(header+1);
	जाव (here < end && !EXT2_EXT_IS_LAST_ENTRY(here)) {
		जइ (!here->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			भङ्ग;
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
	प्रत्यागम inode->i_atime;
}

शून्य ext2fs_set_ea_inode_hash(संरचना ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(संरचना ext2_inode *inode)
{
	प्रत्यागम ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_version;
}

शून्य ext2fs_set_ea_inode_ref(संरचना ext2_inode *inode, __u64 ref_गणनम्)
{
	inode->i_ctime = (__u32)(ref_गणनम् >> 32);
	inode->osd1.linux1.l_i_version = (__u32)ref_गणनम्;
}

स्थिर errcode_t check_ext_attr_header(संरचना ext2_ext_attr_header *header)
{
	जइ ((header->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     header->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    header->h_blocks != 1)
		प्रत्यागम EXT2_ET_BAD_EA_HEADER;

	प्रत्यागम 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block, शून्य *buf,
				ext2_ino_t inum)
{
	पूर्णाङ्क		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	जइ (retval)
		प्रत्यागम retval;

	जइ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_block_csum_verify(fs, inum, block, buf))
		csum_failed = 1;

#ifdef शब्दाः_BIGENDIAN
	ext2fs_swap_ext_attr(buf, buf, fs->blocksize, 1);
#endif

	retval = check_ext_attr_header(buf);
	जइ (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	प्रत्यागम retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block, शून्य *buf)
{
	प्रत्यागम ext2fs_read_ext_attr3(fs, block, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, शून्य *buf)
{
	प्रत्यागम ext2fs_read_ext_attr2(fs, block, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block, शून्य *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	वर्ण		*write_buf;

#ifdef शब्दाः_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	जइ (retval)
		प्रत्यागम retval;
	ext2fs_swap_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#अण्णहा
	write_buf = (वर्ण *) inbuf;
#endif

	retval = ext2fs_ext_attr_block_csum_set(fs, inum, block,
			(संरचना ext2_ext_attr_header *)write_buf);
	जइ (retval)
		प्रत्यागम retval;

	retval = io_channel_write_blk64(fs->io, block, 1, write_buf);
#ifdef शब्दाः_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#endif
	जइ (!retval)
		ext2fs_mark_changed(fs);
	प्रत्यागम retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block, शून्य *inbuf)
{
	प्रत्यागम ext2fs_write_ext_attr3(fs, block, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block, शून्य *inbuf)
{
	प्रत्यागम ext2fs_write_ext_attr2(fs, block, inbuf);
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

	जइ ((blk >= ext2fs_blocks_गणनम्(fs->super)) ||
	    (blk < fs->super->s_first_data_block))
		प्रत्यागम EXT2_ET_BAD_EA_BLOCK_अङ्कः;

	जइ (!block_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		जइ (retval)
			प्रत्यागम retval;
		block_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, block_buf, inum);
	जइ (retval)
		गच्छ errout;

	header = (संरचना ext2_ext_attr_header *) block_buf;
	header->h_refcount += adjust;
	जइ (newcount)
		*newcount = header->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, block_buf, inum);
	जइ (retval)
		गच्छ errout;

errout:
	जइ (buf)
		ext2fs_free_mem(&buf);
	प्रत्यागम retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    वर्ण *block_buf, पूर्णाङ्क adjust,
				    __u32 *newcount)
{
	प्रत्यागम ext2fs_adjust_ea_refcount3(fs, blk, block_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					वर्ण *block_buf, पूर्णाङ्क adjust,
					__u32 *newcount)
{
	प्रत्यागम ext2fs_adjust_ea_refcount2(fs, blk, block_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
संरचना ext2_xattr {
	पूर्णाङ्क name_index;
	वर्ण *name;
	वर्ण *short_name;
	शून्य *मानम्;
	unsigned पूर्णाङ्क मानम्_len;
	ext2_ino_t ea_ino;
};

संरचना ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	संरचना ext2_xattr *attrs;
	पूर्णाङ्क capacity;
	पूर्णाङ्क गणनम्;
	पूर्णाङ्क ibody_गणनम्;
	ext2_ino_t ino;
	unsigned पूर्णाङ्क flags;
};

स्थिर errcode_t ext2fs_xattrs_expand(संरचना ext2_xattr_handle *h,
				      unsigned पूर्णाङ्क expandby)
{
	संरचना ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->capacity + expandby,
				   परिमाण(संरचना ext2_xattr), &new_attrs);
	जइ (err)
		प्रत्यागम err;

	memcpy(new_attrs, h->attrs, h->capacity * परिमाण(संरचना ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->capacity += expandby;
	h->attrs = new_attrs;

	प्रत्यागम 0;
}

संरचना ea_name_index {
	पूर्णाङ्क index;
	ध्रुव वर्ण *name;
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

स्थिर ध्रुव वर्ण *find_ea_prefix(पूर्णाङ्क index)
{
	संरचना ea_name_index *e;

	प्रति (e = ea_names; e->name; e++)
		जइ (e->index == index)
			प्रत्यागम e->name;

	प्रत्यागम रिक्त;
}

स्थिर पूर्णाङ्क find_ea_index(ध्रुव वर्ण *fullname, ध्रुव वर्ण **name, पूर्णाङ्क *index)
{
	संरचना ea_name_index *e;

	प्रति (e = ea_names; e->name; e++) {
		जइ (strncmp(fullname, e->name, strlen(e->name)) == 0) {
			*name = fullname + strlen(e->name);
			*index = e->index;
			प्रत्यागम 1;
		}
	}
	प्रत्यागम 0;
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
	जइ (inode == रिक्त) {
		err = ext2fs_read_inode_full(fs, ino, (संरचना ext2_inode *)&i,
					     परिमाण(संरचना ext2_inode_large));
		जइ (err)
			प्रत्यागम err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (संरचना ext2_inode *)inode);
	जइ (blk == 0)
		प्रत्यागम 0;

	/* Find block, zero it, write back */
	जइ ((blk < fs->super->s_first_data_block) ||
	    (blk >= ext2fs_blocks_गणनम्(fs->super))) {
		err = EXT2_ET_BAD_EA_BLOCK_अङ्कः;
		गच्छ out;
	}

	err = ext2fs_get_mem(fs->blocksize, &block_buf);
	जइ (err)
		गच्छ out;

	err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
	जइ (err)
		गच्छ out2;

	/* We only know how to deal with v2 EA blocks */
	header = (संरचना ext2_ext_attr_header *) block_buf;
	जइ (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_HEADER;
		गच्छ out2;
	}

	header->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
	जइ (err)
		गच्छ out2;

	/* Erase link to block */
	ext2fs_file_acl_block_set(fs, (संरचना ext2_inode *)inode, 0);
	जइ (header->h_refcount == 0)
		ext2fs_block_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_blocks(fs, (संरचना ext2_inode *)inode, 1);
	जइ (err)
		गच्छ out2;

	/* Write inode? */
	जइ (inode == &i) {
		err = ext2fs_write_inode_full(fs, ino, (संरचना ext2_inode *)&i,
					      परिमाण(संरचना ext2_inode_large));
		जइ (err)
			गच्छ out2;
	}

out2:
	ext2fs_free_mem(&block_buf);
out:
	प्रत्यागम err;
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
	जइ (blk != 0) {
		जइ ((blk < fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_गणनम्(fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_अङ्कः;
			गच्छ out;
		}

		err = ext2fs_get_mem(fs->blocksize, &block_buf);
		जइ (err)
			गच्छ out;

		err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
		जइ (err)
			गच्छ out2;

		/* We only know how to deal with v2 EA blocks */
		header = (संरचना ext2_ext_attr_header *) block_buf;
		जइ (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			गच्छ out2;
		}

		/* Single-user block.  We're done here. */
		जइ (header->h_refcount == 1)
			गच्छ out2;

		/* We need to CoW the block. */
		header->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
		जइ (err)
			गच्छ out2;
	} अण्णहा {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_add_blocks(fs, (संरचना ext2_inode *)inode,
					     1);
		जइ (err)
			गच्छ out;
	}

	/* Allocate a block */
	goal = ext2fs_find_inode_goal(fs, ino, (संरचना ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, रिक्त, &blk);
	जइ (err)
		गच्छ out2;
	ext2fs_file_acl_block_set(fs, (संरचना ext2_inode *)inode, blk);
out2:
	जइ (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	प्रत्यागम err;
}


स्थिर अन्तःस्थ पूर्णाङ्क
posix_acl_xattr_गणनम्(size_t size)
{
        जइ (size < परिमाण(posix_acl_xattr_header))
                प्रत्यागम -1;
        size -= परिमाण(posix_acl_xattr_header);
        जइ (size % परिमाण(posix_acl_xattr_entry))
                प्रत्यागम -1;
        प्रत्यागम size / परिमाण(posix_acl_xattr_entry);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
स्थिर errcode_t convert_posix_acl_to_disk_buffer(ध्रुव शून्य *मानम्, size_t size,
						  शून्य *out_buf, size_t *size_out)
{
	ध्रुव posix_acl_xattr_header *header =
		(ध्रुव posix_acl_xattr_header*) मानम्;
	ध्रुव posix_acl_xattr_entry *end, *entry =
		(ध्रुव posix_acl_xattr_entry *)(header+1);
	ext4_acl_header *ext_acl;
	size_t s;
	वर्ण *e;

	पूर्णाङ्क गणनम्;

	जइ (!मानम्)
		प्रत्यागम EINVAL;
	जइ (size < परिमाण(posix_acl_xattr_header))
		प्रत्यागम ENOMEM;
	जइ (header->a_version != ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION))
		प्रत्यागम EINVAL;

	गणनम् = posix_acl_xattr_गणनम्(size);
	ext_acl = out_buf;
	ext_acl->a_version = ext2fs_cpu_to_le32(EXT4_ACL_VERSION);

	जइ (गणनम् <= 0)
		प्रत्यागम EINVAL;

	e = (वर्ण *) out_buf + परिमाण(ext4_acl_header);
	s = परिमाण(ext4_acl_header);
	प्रति (end = entry + गणनम्; entry != end;entry++) {
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
				भङ्ग;
			पक्ष ACL_USER:
			पक्ष ACL_GROUP:
				disk_entry->e_id = entry->e_id;
				e += परिमाण(ext4_acl_entry);
				s += परिमाण(ext4_acl_entry);
				भङ्ग;
			स्वभाव:
				प्रत्यागम EINVAL;
		}
	}
	*size_out = s;
	प्रत्यागम 0;
}

स्थिर errcode_t convert_disk_buffer_to_posix_acl(ध्रुव शून्य *मानम्, size_t size,
						  शून्य **out_buf, size_t *size_out)
{
	posix_acl_xattr_header *header;
	posix_acl_xattr_entry *entry;
	ध्रुव ext4_acl_header *ext_acl = (ध्रुव ext4_acl_header *) मानम्;
	errcode_t err;
	ध्रुव वर्ण *cp;
	वर्ण *out;

	जइ ((!मानम्) ||
	    (size < परिमाण(ext4_acl_header)) ||
	    (ext_acl->a_version != ext2fs_cpu_to_le32(EXT4_ACL_VERSION)))
		प्रत्यागम EINVAL;

	err = ext2fs_get_mem(size * 2, &out);
	जइ (err)
		प्रत्यागम err;

	header = (posix_acl_xattr_header *) out;
	header->a_version = ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION);
	entry = (posix_acl_xattr_entry *) (out + परिमाण(posix_acl_xattr_header));

	cp = (ध्रुव वर्ण *) मानम् + परिमाण(ext4_acl_header);
	size -= परिमाण(ext4_acl_header);

	जाव (size > 0) {
		ध्रुव ext4_acl_entry *disk_entry = (ध्रुव ext4_acl_entry *) cp;

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
				भङ्ग;
			पक्ष ACL_USER:
			पक्ष ACL_GROUP:
				entry->e_id = disk_entry->e_id;
				cp += परिमाण(ext4_acl_entry);
				size -= परिमाण(ext4_acl_entry);
				भङ्ग;
			स्वभाव:
				ext2fs_free_mem(&out);
				प्रत्यागम EINVAL;
		}
		entry++;
	}
	*out_buf = out;
	*size_out = ((वर्ण *) entry - out);
	प्रत्यागम 0;
}

स्थिर errcode_t
write_xattrs_to_buffer(ext2_filsys fs, संरचना ext2_xattr *attrs, पूर्णाङ्क गणनम्,
		       शून्य *entries_start, unsigned पूर्णाङ्क storage_size,
		       unsigned पूर्णाङ्क मानम्_offset_correction, पूर्णाङ्क write_hash)
{
	संरचना ext2_xattr *x;
	संरचना ext2_ext_attr_entry *e = entries_start;
	वर्ण *end = (वर्ण *) entries_start + storage_size;
	unsigned पूर्णाङ्क मानम्_size;
	errcode_t err;

	memset(entries_start, 0, storage_size);
	प्रति (x = attrs; x < attrs + गणनम्; x++) {
		मानम्_size = ((x->मानम्_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = strlen(x->short_name);
		e->e_name_index = x->name_index;

		e->e_मानम्_size = x->मानम्_len;
		e->e_मानम्_inum = x->ea_ino;

		/* Store name */
		memcpy((वर्ण *)e + परिमाण(*e), x->short_name, e->e_name_len);
		जइ (x->ea_ino) {
			e->e_मानम्_offs = 0;
		} अण्णहा {
			end -= मानम्_size;
			e->e_मानम्_offs = end - (वर्ण *) entries_start +
						मानम्_offset_correction;
			memcpy(end, x->मानम्, e->e_मानम्_size);
		}

		जइ (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : end,
							  &e->e_hash);
			जइ (err)
				प्रत्यागम err;
		} अण्णहा
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_NEXT(e);
		*(__u32 *)e = 0;
	}
	प्रत्यागम 0;
}

errcode_t ext2fs_xattrs_write(संरचना ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	ध्रुव unsigned पूर्णाङ्क inode_size = EXT2_INODE_SIZE(fs->super);
	संरचना ext2_inode_large *inode;
	वर्ण *start, *block_buf = रिक्त;
	संरचना ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	blk64_t blk;
	unsigned पूर्णाङ्क storage_size;
	unsigned पूर्णाङ्क i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_size;
	जइ (i < परिमाण(*inode))
		i = परिमाण(*inode);
	err = ext2fs_get_memzero(i, &inode);
	जइ (err)
		प्रत्यागम err;

	err = ext2fs_read_inode_full(fs, handle->ino, EXT2_INODE(inode),
				     inode_size);
	जइ (err)
		गच्छ out;

	/* If extra_isize isn't set, we need to set it now */
	जइ (inode->i_extra_isize == 0 &&
	    inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		वर्ण *p = (वर्ण *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		जइ (extra == 0)
			extra = परिमाण(__u32);
		memset(p + EXT2_GOOD_OLD_INODE_SIZE, 0, extra);
		inode->i_extra_isize = extra;
	}
	जइ (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		गच्छ out;
	}

	/* Does the inode have space for EA? */
	जइ (inode->i_extra_isize < परिमाण(inode->i_extra_isize) ||
	    inode_size <= EXT2_GOOD_OLD_INODE_SIZE + inode->i_extra_isize +
								परिमाण(__u32))
		गच्छ write_ea_block;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	memcpy(((वर्ण *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, &ea_inode_magic, परिमाण(__u32));
	storage_size = inode_size - EXT2_GOOD_OLD_INODE_SIZE -
				inode->i_extra_isize - परिमाण(__u32);
	start = ((वर्ण *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
				inode->i_extra_isize + परिमाण(__u32);

	err = write_xattrs_to_buffer(fs, handle->attrs, handle->ibody_गणनम्,
				     start, storage_size, 0, 0);
	जइ (err)
		गच्छ out;
write_ea_block:
	/* Are we done? */
	जइ (handle->ibody_गणनम् == handle->गणनम् &&
	    !ext2fs_file_acl_block(fs, EXT2_INODE(inode)))
		गच्छ skip_ea_block;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &block_buf);
	जइ (err)
		गच्छ out;

	storage_size = fs->blocksize - परिमाण(संरचना ext2_ext_attr_header);
	start = block_buf + परिमाण(संरचना ext2_ext_attr_header);

	err = write_xattrs_to_buffer(fs, handle->attrs + handle->ibody_गणनम्,
				     handle->गणनम् - handle->ibody_गणनम्, start,
				     storage_size, start - block_buf, 1);
	जइ (err)
		गच्छ out2;

	/* Write a header on the EA block */
	header = (संरचना ext2_ext_attr_header *) block_buf;
	header->h_magic = EXT2_EXT_ATTR_MAGIC;
	header->h_refcount = 1;
	header->h_blocks = 1;

	/* Get a new block for writing */
	err = prep_ea_block_for_write(fs, handle->ino, inode);
	जइ (err)
		गच्छ out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_block(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, handle->ino);
	जइ (err)
		गच्छ out2;

skip_ea_block:
	blk = ext2fs_file_acl_block(fs, (संरचना ext2_inode *)inode);
	जइ (!block_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		जइ (err)
			गच्छ out;
	}

	/* Write the inode */
	err = ext2fs_write_inode_full(fs, handle->ino, EXT2_INODE(inode),
				      inode_size);
	जइ (err)
		गच्छ out2;

out2:
	ext2fs_free_mem(&block_buf);
out:
	ext2fs_free_mem(&inode);
	प्रत्यागम err;
}

स्थिर errcode_t read_xattrs_from_buffer(संरचना ext2_xattr_handle *handle,
					 संरचना ext2_inode_large *inode,
					 संरचना ext2_ext_attr_entry *entries,
					 unsigned पूर्णाङ्क storage_size,
					 वर्ण *मानम्_start)
{
	संरचना ext2_xattr *x;
	संरचना ext2_ext_attr_entry *entry, *end;
	ध्रुव वर्ण *prefix;
	unsigned पूर्णाङ्क remain, prefix_len;
	errcode_t err;
	unsigned पूर्णाङ्क मानानि_size = storage_size +
			((वर्ण *)entries - मानम्_start);

	/* find the end */
	end = entries;
	remain = storage_size;
	जाव (remain >= परिमाण(संरचना ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(end)) {

		/* header eats this space */
		remain -= परिमाण(संरचना ext2_ext_attr_entry);

		/* is attribute name valid? */
		जइ (EXT2_EXT_ATTR_SIZE(end->e_name_len) > remain)
			प्रत्यागम EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(end->e_name_len);
		end = EXT2_EXT_ATTR_NEXT(end);
	}

	entry = entries;
	remain = storage_size;
	जाव (remain >= परिमाण(संरचना ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(entry)) {

		/* Allocate space for more attrs? */
		जइ (handle->गणनम् == handle->capacity) {
			err = ext2fs_xattrs_expand(handle, 4);
			जइ (err)
				प्रत्यागम err;
		}

		x = handle->attrs + handle->गणनम्;

		/* header eats this space */
		remain -= परिमाण(संरचना ext2_ext_attr_entry);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(entry->e_name_len);

		/* Extract name */
		prefix = find_ea_prefix(entry->e_name_index);
		prefix_len = (prefix ? strlen(prefix) : 0);
		err = ext2fs_get_memzero(entry->e_name_len + prefix_len + 1,
					 &x->name);
		जइ (err)
			प्रत्यागम err;
		जइ (prefix)
			memcpy(x->name, prefix, prefix_len);
		जइ (entry->e_name_len)
			memcpy(x->name + prefix_len,
			       (वर्ण *)entry + परिमाण(*entry),
			       entry->e_name_len);
		x->short_name = x->name + prefix_len;
		x->name_index = entry->e_name_index;

		/* Check & copy value */
		जइ (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    entry->e_मानम्_inum != 0)
			प्रत्यागम EXT2_ET_BAD_EA_BLOCK_अङ्कः;

		जइ (entry->e_मानम्_inum == 0) {
			जइ (entry->e_मानम्_size > remain)
				प्रत्यागम EXT2_ET_EA_BAD_मानम्_SIZE;

			जइ (entry->e_मानम्_offs + entry->e_मानम्_size > मानानि_size)
				प्रत्यागम EXT2_ET_EA_BAD_मानम्_OFFSET;

			जइ (entry->e_मानम्_size > 0 &&
			    मानम्_start + entry->e_मानम्_offs <
			    (वर्ण *)end + परिमाण(__u32))
				प्रत्यागम EXT2_ET_EA_BAD_मानम्_OFFSET;

			remain -= entry->e_मानम्_size;

			err = ext2fs_get_mem(entry->e_मानम्_size, &x->मानम्);
			जइ (err)
				प्रत्यागम err;
			memcpy(x->मानम्, मानम्_start + entry->e_मानम्_offs,
			       entry->e_मानम्_size);
		} अण्णहा {
			संरचना ext2_inode *ea_inode;
			ext2_file_t ea_file;

			जइ (entry->e_मानम्_offs != 0)
				प्रत्यागम EXT2_ET_EA_BAD_मानम्_OFFSET;

			जइ (entry->e_मानम्_size > (64 * 1024))
				प्रत्यागम EXT2_ET_EA_BAD_मानम्_SIZE;

			err = ext2fs_get_mem(entry->e_मानम्_size, &x->मानम्);
			जइ (err)
				प्रत्यागम err;

			err = ext2fs_file_open(handle->fs, entry->e_मानम्_inum,
					       0, &ea_file);
			जइ (err)
				प्रत्यागम err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			जइ ((ea_inode->i_flags & EXT4_INLINE_DATA_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_गणनम् == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			अण्णहा जइ ((__u64) ext2fs_file_get_size(ea_file) !=
				 entry->e_मानम्_size)
				err = EXT2_ET_EA_BAD_मानम्_SIZE;
			अण्णहा
				err = ext2fs_file_read(ea_file, x->मानम्,
						       entry->e_मानम्_size, 0);
			ext2fs_file_close(ea_file);
			जइ (err)
				प्रत्यागम err;
		}

		x->ea_ino = entry->e_मानम्_inum;
		x->मानम्_len = entry->e_मानम्_size;

		/* e_hash may be 0 in older inode's ea */
		जइ (entry->e_hash != 0) {
			__u32 hash, signed_hash;

			शून्य *data = (entry->e_मानम्_inum != 0) ?
					0 : मानम्_start + entry->e_मानम्_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, entry,
							  data, &hash,
							  &signed_hash);
			जइ (err)
				प्रत्यागम err;
			जइ ((entry->e_hash != hash) &&
			    (entry->e_hash != signed_hash)) {
				संरचना ext2_inode child;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							entry->e_मानम्_inum,
							&child);
				जइ (err)
					प्रत्यागम err;
				जइ (child.i_mtime != handle->ino ||
				    child.i_generation != inode->i_generation)
					प्रत्यागम EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->गणनम्++;
		entry = EXT2_EXT_ATTR_NEXT(entry);
	}

	प्रत्यागम 0;
}

स्थिर शून्य xattrs_free_कुञ्चिकाः(संरचना ext2_xattr_handle *h)
{
	संरचना ext2_xattr *a = h->attrs;
	पूर्णाङ्क i;

	प्रति (i = 0; i < h->capacity; i++) {
		जइ (a[i].name)
			ext2fs_free_mem(&a[i].name);
		जइ (a[i].मानम्)
			ext2fs_free_mem(&a[i].मानम्);
	}
	h->गणनम् = 0;
	h->ibody_गणनम् = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(संरचना ext2_xattr_handle *handle,
				   संरचना ext2_inode_large *inode)
{
	संरचना ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	unsigned पूर्णाङ्क storage_size;
	वर्ण *start, *block_buf = रिक्त;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_कुञ्चिकाः(handle);

	/* Does the inode have space for EA? */
	जइ (inode->i_extra_isize < परिमाण(inode->i_extra_isize) ||
	    EXT2_INODE_SIZE(handle->fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  परिमाण(__u32))
		गच्छ read_ea_block;
	जइ (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		गच्छ out;
	}

	/* Look for EA in the inode */
	memcpy(&ea_inode_magic, ((वर्ण *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, परिमाण(__u32));
	जइ (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_size = EXT2_INODE_SIZE(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_SIZE - inode->i_extra_isize -
			परिमाण(__u32);
		start = ((वर्ण *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + परिमाण(__u32);

		err = read_xattrs_from_buffer(handle, inode,
					(संरचना ext2_ext_attr_entry *) start,
					storage_size, start);
		जइ (err)
			गच्छ out;

		handle->ibody_गणनम् = handle->गणनम्;
	}

read_ea_block:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_block(handle->fs, EXT2_INODE(inode));
	जइ (blk != 0) {
		जइ ((blk < handle->fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_गणनम्(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_अङ्कः;
			गच्छ out;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &block_buf);
		जइ (err)
			गच्छ out;

		err = ext2fs_read_ext_attr3(handle->fs, blk, block_buf,
					    handle->ino);
		जइ (err)
			गच्छ out3;

		/* We only know how to deal with v2 EA blocks */
		header = (संरचना ext2_ext_attr_header *) block_buf;
		जइ (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
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
	जइ (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	प्रत्यागम err;
}

errcode_t ext2fs_xattrs_read(संरचना ext2_xattr_handle *handle)
{
	संरचना ext2_inode_large *inode;
	size_t inode_size = EXT2_INODE_SIZE(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	जइ (inode_size < परिमाण(*inode))
		inode_size = परिमाण(*inode);
	err = ext2fs_get_memzero(inode_size, &inode);
	जइ (err)
		प्रत्यागम err;

	err = ext2fs_read_inode_full(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_SIZE(handle->fs->super));
	जइ (err)
		गच्छ out;

	err = ext2fs_xattrs_read_inode(handle, inode);

out:
	ext2fs_free_mem(&inode);

	प्रत्यागम err;
}

errcode_t ext2fs_xattrs_iterate(संरचना ext2_xattr_handle *h,
				पूर्णाङ्क (*func)(वर्ण *name, वर्ण *मानम्,
					    size_t मानम्_len, शून्य *data),
				शून्य *data)
{
	संरचना ext2_xattr *x;
	पूर्णाङ्क dirty = 0;
	पूर्णाङ्क ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	प्रति (x = h->attrs; x < h->attrs + h->गणनम्; x++) {
		ret = func(x->name, x->मानम्, x->मानम्_len, data);
		जइ (ret & XATTR_CHANGED)
			dirty = 1;
		जइ (ret & XATTR_ABORT)
			भङ्ग;
	}

	जइ (dirty)
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
		जइ (strcmp(x->name, कुञ्चिका))
			अनुवर्त;

		जइ (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(कुञ्चिका, "system.posix_acl_default") == 0) ||
		     (strcmp(कुञ्चिका, "system.posix_acl_access") == 0))) {
			err = convert_disk_buffer_to_posix_acl(x->मानम्, x->मानम्_len,
							       मानम्, मानम्_len);
			प्रत्यागम err;
		} अण्णहा {
			err = ext2fs_get_mem(x->मानम्_len, &val);
			जइ (err)
				प्रत्यागम err;
			memcpy(val, x->मानम्, x->मानम्_len);
			*मानम् = val;
			*मानम्_len = x->मानम्_len;
			प्रत्यागम 0;
		}
	}

	प्रत्यागम EXT2_ET_EA_कुञ्चिका_NOT_FOUND;
}

errcode_t ext2fs_xattr_inode_max_size(ext2_filsys fs, ext2_ino_t ino,
				      size_t *size)
{
	संरचना ext2_ext_attr_entry *entry;
	संरचना ext2_inode_large *inode;
	__u32 ea_inode_magic;
	unsigned पूर्णाङ्क minoff;
	वर्ण *start;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_SIZE(fs->super);
	जइ (i < परिमाण(*inode))
		i = परिमाण(*inode);
	err = ext2fs_get_memzero(i, &inode);
	जइ (err)
		प्रत्यागम err;

	err = ext2fs_read_inode_full(fs, ino, (संरचना ext2_inode *)inode,
				     EXT2_INODE_SIZE(fs->super));
	जइ (err)
		गच्छ out;

	/* Does the inode have size for EA? */
	जइ (EXT2_INODE_SIZE(fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  परिमाण(__u32)) {
		err = EXT2_ET_INLINE_DATA_NO_SPACE;
		गच्छ out;
	}

	minoff = EXT2_INODE_SIZE(fs->super) - परिमाण(*inode) - परिमाण(__u32);
	memcpy(&ea_inode_magic, ((वर्ण *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, परिमाण(__u32));
	जइ (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		start= ((वर्ण *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + परिमाण(__u32);
		entry = (संरचना ext2_ext_attr_entry *) start;
		जाव (!EXT2_EXT_IS_LAST_ENTRY(entry)) {
			जइ (!entry->e_मानम्_inum && entry->e_मानम्_size) {
				unsigned पूर्णाङ्क offs = entry->e_मानम्_offs;
				जइ (offs < minoff)
					minoff = offs;
			}
			entry = EXT2_EXT_ATTR_NEXT(entry);
		}
		*size = minoff - ((वर्ण *)entry - (वर्ण *)start) - परिमाण(__u32);
	} अण्णहा {
		/* no xattr.  return a maximum size */
		*size = EXT2_EXT_ATTR_SIZE(minoff -
					   EXT2_EXT_ATTR_LEN(strlen("data")) -
					   EXT2_EXT_ATTR_ROUND - परिमाण(__u32));
	}

out:
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
	जइ (ret)
		प्रत्यागम ret;

	memset(&inode, 0, परिमाण(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	जइ (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_size = 0;
	inode.i_mode = LINUX_S_IFREG | 0600;
	inode.i_links_गणनम् = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	जइ (ret)
		प्रत्यागम ret;
	/*
	 * ref_count and hash utilize inode's i_*time fields.
	 * ext2fs_write_new_inode() call above initializes these fields with
	 * current time. That's why ref count and hash updates are done
	 * separately below.
	 */
	ext2fs_set_ea_inode_ref(&inode, 1);
	hash = ext2fs_crc32c_le(fs->csum_seed, मानम्, मानम्_len);
	ext2fs_set_ea_inode_hash(&inode, hash);

	ret = ext2fs_write_inode(fs, ino, &inode);
	जइ (ret)
		प्रत्यागम ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	जइ (ret)
		प्रत्यागम ret;
	ret = ext2fs_file_write(file, मानम्, मानम्_len, रिक्त);
	ext2fs_file_close(file);
	जइ (ret)
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

	ret = ext2fs_read_inode_full(fs, ino, (संरचना ext2_inode *)&inode,
				     परिमाण(inode));
	जइ (ret)
		गच्छ out;

	ref_गणनम् = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_गणनम्--;
	ext2fs_set_ea_inode_ref(EXT2_INODE(&inode), ref_गणनम्);

	जइ (ref_गणनम्)
		गच्छ write_out;

	inode.i_links_गणनम् = 0;
	ext2fs_set_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	जइ (ret)
		गच्छ write_out;

	जइ (ext2fs_inode_has_valid_blocks2(fs, (संरचना ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (संरचना ext2_inode *)&inode, रिक्त,
				   0, ~0ULL);
		जइ (ret)
			गच्छ out;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_out:
	ret = ext2fs_write_inode_full(fs, ino, (संरचना ext2_inode *)&inode,
				      परिमाण(inode));
out:
	प्रत्यागम ret;
}

स्थिर errcode_t xattr_update_entry(ext2_filsys fs, संरचना ext2_xattr *x,
				    ध्रुव वर्ण *name, ध्रुव वर्ण *short_name,
				    पूर्णाङ्क index, ध्रुव शून्य *मानम्,
				    size_t मानम्_len, पूर्णाङ्क in_inode)
{
	ext2_ino_t ea_ino = 0;
	शून्य *new_मानम् = रिक्त;
	वर्ण *new_name = रिक्त;
	पूर्णाङ्क name_len;
	errcode_t ret;

	जइ (!x->name) {
		name_len = strlen(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		जइ (ret)
			गच्छ fail;
		memcpy(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(मानम्_len, &new_मानम्);
	जइ (ret)
		गच्छ fail;
	memcpy(new_मानम्, मानम्, मानम्_len);

	जइ (in_inode) {
		ret = xattr_create_ea_inode(fs, मानम्, मानम्_len, &ea_ino);
		जइ (ret)
			गच्छ fail;
	}

	जइ (x->ea_ino) {
		ret = xattr_inode_dec_ref(fs, x->ea_ino);
		जइ (ret)
			गच्छ fail;
	}

	जइ (!x->name) {
		x->name = new_name;
		x->short_name = new_name + (short_name  - name);
	}
	x->name_index = index;

	जइ (x->मानम्)
		ext2fs_free_mem(&x->मानम्);
	x->मानम् = new_मानम्;
	x->मानम्_len = मानम्_len;
	x->ea_ino = ea_ino;
	प्रत्यागम 0;
fail:
	जइ (new_name)
		ext2fs_free_mem(&new_name);
	जइ (new_मानम्)
		ext2fs_free_mem(&new_मानम्);
	जइ (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क xattr_find_position(संरचना ext2_xattr *attrs, पूर्णाङ्क गणनम्,
			       ध्रुव वर्ण *shortname, पूर्णाङ्क name_idx)
{
	संरचना ext2_xattr *x;
	पूर्णाङ्क i;
	पूर्णाङ्क shortname_len, x_shortname_len;

	shortname_len = strlen(shortname);

	प्रति (i = 0, x = attrs; i < गणनम्; i++, x++) {
		जइ (name_idx < x->name_index)
			भङ्ग;
		जइ (name_idx > x->name_index)
			अनुवर्त;

		x_shortname_len = strlen(x->short_name);
		जइ (shortname_len < x_shortname_len)
			भङ्ग;
		जइ (shortname_len > x_shortname_len)
			अनुवर्त;

		जइ (memcmp(shortname, x->short_name, shortname_len) <= 0)
			भङ्ग;
	}
	प्रत्यागम i;
}

स्थिर errcode_t xattr_array_update(संरचना ext2_xattr_handle *h,
				    ध्रुव वर्ण *name,
				    ध्रुव शून्य *मानम्, size_t मानम्_len,
				    पूर्णाङ्क ibody_free, पूर्णाङ्क block_free,
				    पूर्णाङ्क old_idx, पूर्णाङ्क in_inode)
{
	संरचना ext2_xattr tmp;
	पूर्णाङ्क add_to_ibody;
	पूर्णाङ्क needed;
	पूर्णाङ्क name_len, name_idx = 0;
	ध्रुव वर्ण *shortname = name;
	पूर्णाङ्क new_idx;
	पूर्णाङ्क ret;

	find_ea_index(name, &shortname, &name_idx);
	name_len = strlen(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	जइ (!in_inode)
		needed += EXT2_EXT_ATTR_SIZE(मानम्_len);

	जइ (old_idx >= 0 && old_idx < h->ibody_गणनम्) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		जइ (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_SIZE(
						h->attrs[old_idx].मानम्_len);
	}

	जइ (needed <= ibody_free) {
		जइ (old_idx < 0) {
			new_idx = h->ibody_गणनम्;
			add_to_ibody = 1;
			गच्छ add_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, मानम्,
					 मानम्_len, in_inode);
		जइ (ret)
			प्रत्यागम ret;
		जइ (h->ibody_गणनम् <= old_idx) {
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

	जइ (h->ibody_गणनम् <= old_idx) {
		block_free += EXT2_EXT_ATTR_LEN(name_len);
		जइ (!h->attrs[old_idx].ea_ino)
			block_free +=
				EXT2_EXT_ATTR_SIZE(h->attrs[old_idx].मानम्_len);
	}

	जइ (needed > block_free)
		प्रत्यागम EXT2_ET_EA_NO_SPACE;

	जइ (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, मानम्,
					 मानम्_len, in_inode);
		जइ (ret)
			प्रत्यागम ret;
		जइ (old_idx < h->ibody_गणनम्) {
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
	add_to_ibody = 0;

add_new:
	जइ (h->गणनम् == h->capacity) {
		ret = ext2fs_xattrs_expand(h, 4);
		जइ (ret)
			प्रत्यागम ret;
	}

	ret = xattr_update_entry(h->fs, &h->attrs[h->गणनम्], name, shortname,
				 name_idx, मानम्, मानम्_len, in_inode);
	जइ (ret)
		प्रत्यागम ret;

	tmp = h->attrs[h->गणनम्];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->गणनम् - new_idx)*परिमाण(*h->attrs));
	h->attrs[new_idx] = tmp;
	जइ (add_to_ibody)
		h->ibody_गणनम्++;
	h->गणनम्++;
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क space_used(संरचना ext2_xattr *attrs, पूर्णाङ्क गणनम्)
{
	पूर्णाङ्क total = 0;
	संरचना ext2_xattr *x;
	पूर्णाङ्क i, len;

	प्रति (i = 0, x = attrs; i < गणनम्; i++, x++) {
		len = strlen(x->short_name);
		total += EXT2_EXT_ATTR_LEN(len);
		जइ (!x->ea_ino)
			total += EXT2_EXT_ATTR_SIZE(x->मानम्_len);
	}
	प्रत्यागम total;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#define EXT4_XATTR_MIN_LARGE_EA_SIZE(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - परिमाण(संरचना ext2_ext_attr_header) - 4)

errcode_t ext2fs_xattr_set(संरचना ext2_xattr_handle *h,
			   ध्रुव वर्ण *name,
			   ध्रुव शून्य *मानम्,
			   size_t मानम्_len)
{
	ext2_filsys fs = h->fs;
	ध्रुव पूर्णाङ्क inode_size = EXT2_INODE_SIZE(fs->super);
	संरचना ext2_inode_large *inode = रिक्त;
	संरचना ext2_xattr *x;
	वर्ण *new_मानम्;
	पूर्णाङ्क ibody_free, block_free;
	पूर्णाङ्क in_inode = 0;
	पूर्णाङ्क old_idx = -1;
	पूर्णाङ्क extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(मानम्_len, &new_मानम्);
	जइ (ret)
		प्रत्यागम ret;
	जइ (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((strcmp(name, "system.posix_acl_default") == 0) ||
	     (strcmp(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_to_disk_buffer(मानम्, मानम्_len,
						       new_मानम्, &मानम्_len);
		जइ (ret)
			गच्छ out;
	} अण्णहा जइ (मानम्_len)
		memcpy(new_मानम्, मानम्, मानम्_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	प्रति (x = h->attrs; x < h->attrs + h->गणनम्; x++) {
		जइ (!strcmp(x->name, name)) {
			जइ (!x->ea_ino && x->मानम्_len == मानम्_len &&
			    (!मानम्_len ||
			     !memcmp(x->मानम्, new_मानम्, मानम्_len))) {
				ret = 0;
				गच्छ out;
			}
			old_idx = x - h->attrs;
			भङ्ग;
		}
	}

	ret = ext2fs_get_memzero(inode_size, &inode);
	जइ (ret)
		गच्छ out;
	ret = ext2fs_read_inode_full(fs, h->ino,
				     (संरचना ext2_inode *)inode,
				     inode_size);
	जइ (ret)
		गच्छ out;
	जइ (inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		extra_isize = inode->i_extra_isize;
		जइ (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			जइ (extra_isize == 0)
				extra_isize = परिमाण(__u32);
		}
		ibody_free = inode_size - EXT2_GOOD_OLD_INODE_SIZE;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= परिमाण(__u32) * 2;
		ibody_free -= space_used(h->attrs, h->ibody_गणनम्);
	} अण्णहा
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	जइ (strcmp(name, "system.data") == 0) {
		जइ (h->ibody_गणनम् <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			गच्छ out;
		}
		ret = xattr_array_update(h, name, new_मानम्, मानम्_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		जइ (ret)
			गच्छ out;
		गच्छ write_out;
	}

	block_free = fs->blocksize;
	block_free -= परिमाण(संरचना ext2_ext_attr_header);
	/* Final null entry. */
	block_free -= परिमाण(__u32);
	block_free -= space_used(h->attrs + h->ibody_गणनम्,
				 h->गणनम् - h->ibody_गणनम्);

	जइ (ext2fs_has_feature_ea_inode(fs->super) &&
	    मानम्_len > EXT4_XATTR_MIN_LARGE_EA_SIZE(fs->blocksize))
		in_inode = 1;

	ret = xattr_array_update(h, name, new_मानम्, मानम्_len, ibody_free,
				 block_free, old_idx, in_inode);
	जइ (ret == EXT2_ET_EA_NO_SPACE && !in_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_मानम्, मानम्_len,
			ibody_free, block_free, old_idx, 1 /* in_inode */);
	जइ (ret)
		गच्छ out;

write_out:
	ret = ext2fs_xattrs_write(h);
out:
	जइ (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_मानम्);
	प्रत्यागम ret;
}

errcode_t ext2fs_xattr_remove(संरचना ext2_xattr_handle *handle,
			      ध्रुव वर्ण *कुञ्चिका)
{
	संरचना ext2_xattr *x;
	संरचना ext2_xattr *end = handle->attrs + handle->गणनम्;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	प्रति (x = handle->attrs; x < end; x++) {
		जइ (strcmp(x->name, कुञ्चिका) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->मानम्);
			जइ (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (end - x - 1)*परिमाण(*x));
			memset(end - 1, 0, परिमाण(*end));
			जइ (x < handle->attrs + handle->ibody_गणनम्)
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
	संरचना ext2_xattr *end = handle->attrs + handle->गणनम्;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	प्रति (x = handle->attrs; x < end; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->मानम्);
		जइ (x->ea_ino)
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

	जइ (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_data(fs->super))
		प्रत्यागम EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(परिमाण(*h), &h);
	जइ (err)
		प्रत्यागम err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->capacity = 4;
	err = ext2fs_get_arrayzero(h->capacity, परिमाण(संरचना ext2_xattr),
				   &h->attrs);
	जइ (err) {
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
			      unsigned पूर्णाङ्क *new_flags, unsigned पूर्णाङ्क *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	जइ (old_flags)
		*old_flags = handle->flags;
	जइ (new_flags)
		handle->flags = *new_flags;
	प्रत्यागम 0;
}
