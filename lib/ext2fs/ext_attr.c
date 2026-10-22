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
#जर HAVE_UNISTD_H
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
	जर (retval)
		परत retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	परत 0;
}

#define NAME_HASH_SHIFT 5
#define मूल्य_HASH_SHIFT 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_entry(संरचना ext2_ext_attr_entry *entry, शून्य *data)
{
	__u32 hash = 0;
	अचिन्हित अक्षर *name = (((अचिन्हित अक्षर *) entry) +
			       आकार(संरचना ext2_ext_attr_entry));
	पूर्णांक n;

	साठी (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*आकार(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	जर (entry->e_मूल्य_inum == 0 && entry->e_मूल्य_size != 0) {
		__u32 *मूल्य = (__u32 *)data;
		साठी (n = (entry->e_मूल्य_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << मूल्य_HASH_SHIFT) ^
			       (hash >> (8*आकार(hash) - मूल्य_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*मूल्य++);
		}
	}

	परत hash;
}

__u32 ext2fs_ext_attr_hash_entry_signed(संरचना ext2_ext_attr_entry *entry,
					शून्य *data)
{
	__u32 hash = 0;
	चिन्हित अक्षर *name = (((चिन्हित अक्षर *) entry) +
			     आकार(संरचना ext2_ext_attr_entry));
	पूर्णांक n;

	साठी (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*आकार(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	जर (entry->e_मूल्य_inum == 0 && entry->e_मूल्य_size != 0) {
		__u32 *मूल्य = (__u32 *)data;
		साठी (n = (entry->e_मूल्य_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << मूल्य_HASH_SHIFT) ^
			       (hash >> (8*आकार(hash) - मूल्य_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*मूल्य++);
		}
	}

	परत hash;
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
	जर (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_entry_signed(entry, data);

	जर (entry->e_मूल्य_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, entry->e_मूल्य_inum,
					    &ea_inode_hash);
		जर (retval)
			परत retval;

		*hash = (*hash << मूल्य_HASH_SHIFT) ^
			(*hash >> (8*आकार(*hash) - मूल्य_HASH_SHIFT)) ^
			ea_inode_hash;
		जर (signed_hash)
			*signed_hash = (*signed_hash << मूल्य_HASH_SHIFT) ^
				(*signed_hash >> (8*आकार(*hash) -
						  मूल्य_HASH_SHIFT)) ^
				ea_inode_hash;
	}
	परत 0;
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
	परत ext2fs_ext_attr_hash_entry3(fs, entry, data, hash, रिक्त);
}

#undef NAME_HASH_SHIFT
#undef मूल्य_HASH_SHIFT

#define BLOCK_HASH_SHIFT 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
शून्य ext2fs_ext_attr_block_rehash(संरचना ext2_ext_attr_header *header,
				  संरचना ext2_ext_attr_entry *end)
{
	संरचना ext2_ext_attr_entry *here;
	__u32 hash = 0;

	here = (संरचना ext2_ext_attr_entry *)(header+1);
	पर्यंत (here < end && !EXT2_EXT_IS_LAST_ENTRY(here)) {
		जर (!here->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			थांबा;
		}
		hash = (hash << BLOCK_HASH_SHIFT) ^
		       (hash >> (8*आकार(hash) - BLOCK_HASH_SHIFT)) ^
		       here->e_hash;
		here = EXT2_EXT_ATTR_NEXT(here);
	}
	header->h_hash = hash;
}

#undef BLOCK_HASH_SHIFT

__u32 ext2fs_get_ea_inode_hash(संरचना ext2_inode *inode)
{
	परत inode->i_atime;
}

शून्य ext2fs_set_ea_inode_hash(संरचना ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(संरचना ext2_inode *inode)
{
	परत ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_version;
}

शून्य ext2fs_set_ea_inode_ref(संरचना ext2_inode *inode, __u64 ref_मोजणी)
{
	inode->i_ctime = (__u32)(ref_मोजणी >> 32);
	inode->osd1.linux1.l_i_version = (__u32)ref_मोजणी;
}

स्थिर errcode_t check_ext_attr_header(संरचना ext2_ext_attr_header *header)
{
	जर ((header->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     header->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    header->h_blocks != 1)
		परत EXT2_ET_BAD_EA_HEADER;

	परत 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block, शून्य *buf,
				ext2_ino_t inum)
{
	पूर्णांक		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	जर (retval)
		परत retval;

	जर (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_block_csum_verify(fs, inum, block, buf))
		csum_failed = 1;

#ifdef शब्द_BIGENDIAN
	ext2fs_swap_ext_attr(buf, buf, fs->blocksize, 1);
#endif

	retval = check_ext_attr_header(buf);
	जर (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	परत retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block, शून्य *buf)
{
	परत ext2fs_read_ext_attr3(fs, block, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, शून्य *buf)
{
	परत ext2fs_read_ext_attr2(fs, block, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block, शून्य *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	अक्षर		*write_buf;

#ifdef शब्द_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	जर (retval)
		परत retval;
	ext2fs_swap_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#अन्यथा
	write_buf = (अक्षर *) inbuf;
#endif

	retval = ext2fs_ext_attr_block_csum_set(fs, inum, block,
			(संरचना ext2_ext_attr_header *)write_buf);
	जर (retval)
		परत retval;

	retval = io_channel_write_blk64(fs->io, block, 1, write_buf);
#ifdef शब्द_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#endif
	जर (!retval)
		ext2fs_mark_changed(fs);
	परत retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block, शून्य *inbuf)
{
	परत ext2fs_write_ext_attr3(fs, block, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block, शून्य *inbuf)
{
	परत ext2fs_write_ext_attr2(fs, block, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    अक्षर *block_buf, पूर्णांक adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	संरचना ext2_ext_attr_header *header;
	अक्षर	*buf = 0;

	जर ((blk >= ext2fs_blocks_मोजणी(fs->super)) ||
	    (blk < fs->super->s_first_data_block))
		परत EXT2_ET_BAD_EA_BLOCK_अंक;

	जर (!block_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		जर (retval)
			परत retval;
		block_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, block_buf, inum);
	जर (retval)
		जा errout;

	header = (संरचना ext2_ext_attr_header *) block_buf;
	header->h_refcount += adjust;
	जर (newcount)
		*newcount = header->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, block_buf, inum);
	जर (retval)
		जा errout;

errout:
	जर (buf)
		ext2fs_free_mem(&buf);
	परत retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    अक्षर *block_buf, पूर्णांक adjust,
				    __u32 *newcount)
{
	परत ext2fs_adjust_ea_refcount3(fs, blk, block_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					अक्षर *block_buf, पूर्णांक adjust,
					__u32 *newcount)
{
	परत ext2fs_adjust_ea_refcount2(fs, blk, block_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
संरचना ext2_xattr {
	पूर्णांक name_index;
	अक्षर *name;
	अक्षर *short_name;
	शून्य *मूल्य;
	अचिन्हित पूर्णांक मूल्य_len;
	ext2_ino_t ea_ino;
};

संरचना ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	संरचना ext2_xattr *attrs;
	पूर्णांक capacity;
	पूर्णांक मोजणी;
	पूर्णांक ibody_मोजणी;
	ext2_ino_t ino;
	अचिन्हित पूर्णांक flags;
};

स्थिर errcode_t ext2fs_xattrs_expand(संरचना ext2_xattr_handle *h,
				      अचिन्हित पूर्णांक expandby)
{
	संरचना ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->capacity + expandby,
				   आकार(संरचना ext2_xattr), &new_attrs);
	जर (err)
		परत err;

	स_नकल(new_attrs, h->attrs, h->capacity * आकार(संरचना ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->capacity += expandby;
	h->attrs = new_attrs;

	परत 0;
}

संरचना ea_name_index {
	पूर्णांक index;
	स्थिरांक अक्षर *name;
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

स्थिर स्थिरांक अक्षर *find_ea_prefix(पूर्णांक index)
{
	संरचना ea_name_index *e;

	साठी (e = ea_names; e->name; e++)
		जर (e->index == index)
			परत e->name;

	परत रिक्त;
}

स्थिर पूर्णांक find_ea_index(स्थिरांक अक्षर *fullname, स्थिरांक अक्षर **name, पूर्णांक *index)
{
	संरचना ea_name_index *e;

	साठी (e = ea_names; e->name; e++) {
		जर (म_भेदन(fullname, e->name, म_माप(e->name)) == 0) {
			*name = fullname + म_माप(e->name);
			*index = e->index;
			परत 1;
		}
	}
	परत 0;
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
	जर (inode == रिक्त) {
		err = ext2fs_read_inode_full(fs, ino, (संरचना ext2_inode *)&i,
					     आकार(संरचना ext2_inode_large));
		जर (err)
			परत err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (संरचना ext2_inode *)inode);
	जर (blk == 0)
		परत 0;

	/* Find block, zero it, write back */
	जर ((blk < fs->super->s_first_data_block) ||
	    (blk >= ext2fs_blocks_मोजणी(fs->super))) {
		err = EXT2_ET_BAD_EA_BLOCK_अंक;
		जा out;
	}

	err = ext2fs_get_mem(fs->blocksize, &block_buf);
	जर (err)
		जा out;

	err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
	जर (err)
		जा out2;

	/* We only know how to deal with v2 EA blocks */
	header = (संरचना ext2_ext_attr_header *) block_buf;
	जर (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_HEADER;
		जा out2;
	}

	header->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
	जर (err)
		जा out2;

	/* Erase link to block */
	ext2fs_file_acl_block_set(fs, (संरचना ext2_inode *)inode, 0);
	जर (header->h_refcount == 0)
		ext2fs_block_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_blocks(fs, (संरचना ext2_inode *)inode, 1);
	जर (err)
		जा out2;

	/* Write inode? */
	जर (inode == &i) {
		err = ext2fs_write_inode_full(fs, ino, (संरचना ext2_inode *)&i,
					      आकार(संरचना ext2_inode_large));
		जर (err)
			जा out2;
	}

out2:
	ext2fs_free_mem(&block_buf);
out:
	परत err;
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
	जर (blk != 0) {
		जर ((blk < fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_मोजणी(fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_अंक;
			जा out;
		}

		err = ext2fs_get_mem(fs->blocksize, &block_buf);
		जर (err)
			जा out;

		err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
		जर (err)
			जा out2;

		/* We only know how to deal with v2 EA blocks */
		header = (संरचना ext2_ext_attr_header *) block_buf;
		जर (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			जा out2;
		}

		/* Single-user block.  We're done here. */
		जर (header->h_refcount == 1)
			जा out2;

		/* We need to CoW the block. */
		header->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
		जर (err)
			जा out2;
	} अन्यथा {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_add_blocks(fs, (संरचना ext2_inode *)inode,
					     1);
		जर (err)
			जा out;
	}

	/* Allocate a block */
	goal = ext2fs_find_inode_goal(fs, ino, (संरचना ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, रिक्त, &blk);
	जर (err)
		जा out2;
	ext2fs_file_acl_block_set(fs, (संरचना ext2_inode *)inode, blk);
out2:
	जर (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	परत err;
}


स्थिर ओळीत पूर्णांक
posix_acl_xattr_मोजणी(माप_प्रकार size)
{
        जर (size < आकार(posix_acl_xattr_header))
                परत -1;
        size -= आकार(posix_acl_xattr_header);
        जर (size % आकार(posix_acl_xattr_entry))
                परत -1;
        परत size / आकार(posix_acl_xattr_entry);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
स्थिर errcode_t convert_posix_acl_to_disk_buffer(स्थिरांक शून्य *मूल्य, माप_प्रकार size,
						  शून्य *out_buf, माप_प्रकार *size_out)
{
	स्थिरांक posix_acl_xattr_header *header =
		(स्थिरांक posix_acl_xattr_header*) मूल्य;
	स्थिरांक posix_acl_xattr_entry *end, *entry =
		(स्थिरांक posix_acl_xattr_entry *)(header+1);
	ext4_acl_header *ext_acl;
	माप_प्रकार s;
	अक्षर *e;

	पूर्णांक मोजणी;

	जर (!मूल्य)
		परत EINVAL;
	जर (size < आकार(posix_acl_xattr_header))
		परत ENOMEM;
	जर (header->a_version != ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION))
		परत EINVAL;

	मोजणी = posix_acl_xattr_मोजणी(size);
	ext_acl = out_buf;
	ext_acl->a_version = ext2fs_cpu_to_le32(EXT4_ACL_VERSION);

	जर (मोजणी <= 0)
		परत EINVAL;

	e = (अक्षर *) out_buf + आकार(ext4_acl_header);
	s = आकार(ext4_acl_header);
	साठी (end = entry + मोजणी; entry != end;entry++) {
		ext4_acl_entry *disk_entry = (ext4_acl_entry*) e;
		disk_entry->e_tag = entry->e_tag;
		disk_entry->e_perm = entry->e_perm;

		निवड(ext2fs_le16_to_cpu(entry->e_tag)) {
			स्थिती ACL_USER_OBJ:
			स्थिती ACL_GROUP_OBJ:
			स्थिती ACL_MASK:
			स्थिती ACL_OTHER:
				e += आकार(ext4_acl_entry_short);
				s += आकार(ext4_acl_entry_short);
				थांबा;
			स्थिती ACL_USER:
			स्थिती ACL_GROUP:
				disk_entry->e_id = entry->e_id;
				e += आकार(ext4_acl_entry);
				s += आकार(ext4_acl_entry);
				थांबा;
			पूर्वनिर्धारित:
				परत EINVAL;
		}
	}
	*size_out = s;
	परत 0;
}

स्थिर errcode_t convert_disk_buffer_to_posix_acl(स्थिरांक शून्य *मूल्य, माप_प्रकार size,
						  शून्य **out_buf, माप_प्रकार *size_out)
{
	posix_acl_xattr_header *header;
	posix_acl_xattr_entry *entry;
	स्थिरांक ext4_acl_header *ext_acl = (स्थिरांक ext4_acl_header *) मूल्य;
	errcode_t err;
	स्थिरांक अक्षर *cp;
	अक्षर *out;

	जर ((!मूल्य) ||
	    (size < आकार(ext4_acl_header)) ||
	    (ext_acl->a_version != ext2fs_cpu_to_le32(EXT4_ACL_VERSION)))
		परत EINVAL;

	err = ext2fs_get_mem(size * 2, &out);
	जर (err)
		परत err;

	header = (posix_acl_xattr_header *) out;
	header->a_version = ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION);
	entry = (posix_acl_xattr_entry *) (out + आकार(posix_acl_xattr_header));

	cp = (स्थिरांक अक्षर *) मूल्य + आकार(ext4_acl_header);
	size -= आकार(ext4_acl_header);

	पर्यंत (size > 0) {
		स्थिरांक ext4_acl_entry *disk_entry = (स्थिरांक ext4_acl_entry *) cp;

		entry->e_tag = disk_entry->e_tag;
		entry->e_perm = disk_entry->e_perm;

		निवड(ext2fs_le16_to_cpu(entry->e_tag)) {
			स्थिती ACL_USER_OBJ:
			स्थिती ACL_GROUP_OBJ:
			स्थिती ACL_MASK:
			स्थिती ACL_OTHER:
				entry->e_id = 0;
				cp += आकार(ext4_acl_entry_short);
				size -= आकार(ext4_acl_entry_short);
				थांबा;
			स्थिती ACL_USER:
			स्थिती ACL_GROUP:
				entry->e_id = disk_entry->e_id;
				cp += आकार(ext4_acl_entry);
				size -= आकार(ext4_acl_entry);
				थांबा;
			पूर्वनिर्धारित:
				ext2fs_free_mem(&out);
				परत EINVAL;
		}
		entry++;
	}
	*out_buf = out;
	*size_out = ((अक्षर *) entry - out);
	परत 0;
}

स्थिर errcode_t
write_xattrs_to_buffer(ext2_filsys fs, संरचना ext2_xattr *attrs, पूर्णांक मोजणी,
		       शून्य *entries_start, अचिन्हित पूर्णांक storage_size,
		       अचिन्हित पूर्णांक मूल्य_offset_correction, पूर्णांक write_hash)
{
	संरचना ext2_xattr *x;
	संरचना ext2_ext_attr_entry *e = entries_start;
	अक्षर *end = (अक्षर *) entries_start + storage_size;
	अचिन्हित पूर्णांक मूल्य_size;
	errcode_t err;

	स_रखो(entries_start, 0, storage_size);
	साठी (x = attrs; x < attrs + मोजणी; x++) {
		मूल्य_size = ((x->मूल्य_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = म_माप(x->short_name);
		e->e_name_index = x->name_index;

		e->e_मूल्य_size = x->मूल्य_len;
		e->e_मूल्य_inum = x->ea_ino;

		/* Store name */
		स_नकल((अक्षर *)e + आकार(*e), x->short_name, e->e_name_len);
		जर (x->ea_ino) {
			e->e_मूल्य_offs = 0;
		} अन्यथा {
			end -= मूल्य_size;
			e->e_मूल्य_offs = end - (अक्षर *) entries_start +
						मूल्य_offset_correction;
			स_नकल(end, x->मूल्य, e->e_मूल्य_size);
		}

		जर (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : end,
							  &e->e_hash);
			जर (err)
				परत err;
		} अन्यथा
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_NEXT(e);
		*(__u32 *)e = 0;
	}
	परत 0;
}

errcode_t ext2fs_xattrs_write(संरचना ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	स्थिरांक अचिन्हित पूर्णांक inode_size = EXT2_INODE_SIZE(fs->super);
	संरचना ext2_inode_large *inode;
	अक्षर *start, *block_buf = रिक्त;
	संरचना ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	blk64_t blk;
	अचिन्हित पूर्णांक storage_size;
	अचिन्हित पूर्णांक i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_size;
	जर (i < आकार(*inode))
		i = आकार(*inode);
	err = ext2fs_get_memzero(i, &inode);
	जर (err)
		परत err;

	err = ext2fs_read_inode_full(fs, handle->ino, EXT2_INODE(inode),
				     inode_size);
	जर (err)
		जा out;

	/* If extra_isize isn't set, we need to set it now */
	जर (inode->i_extra_isize == 0 &&
	    inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		अक्षर *p = (अक्षर *)inode;
		माप_प्रकार extra = fs->super->s_want_extra_isize;

		जर (extra == 0)
			extra = आकार(__u32);
		स_रखो(p + EXT2_GOOD_OLD_INODE_SIZE, 0, extra);
		inode->i_extra_isize = extra;
	}
	जर (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		जा out;
	}

	/* Does the inode have space for EA? */
	जर (inode->i_extra_isize < आकार(inode->i_extra_isize) ||
	    inode_size <= EXT2_GOOD_OLD_INODE_SIZE + inode->i_extra_isize +
								आकार(__u32))
		जा write_ea_block;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	स_नकल(((अक्षर *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, &ea_inode_magic, आकार(__u32));
	storage_size = inode_size - EXT2_GOOD_OLD_INODE_SIZE -
				inode->i_extra_isize - आकार(__u32);
	start = ((अक्षर *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
				inode->i_extra_isize + आकार(__u32);

	err = write_xattrs_to_buffer(fs, handle->attrs, handle->ibody_मोजणी,
				     start, storage_size, 0, 0);
	जर (err)
		जा out;
write_ea_block:
	/* Are we done? */
	जर (handle->ibody_मोजणी == handle->मोजणी &&
	    !ext2fs_file_acl_block(fs, EXT2_INODE(inode)))
		जा skip_ea_block;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &block_buf);
	जर (err)
		जा out;

	storage_size = fs->blocksize - आकार(संरचना ext2_ext_attr_header);
	start = block_buf + आकार(संरचना ext2_ext_attr_header);

	err = write_xattrs_to_buffer(fs, handle->attrs + handle->ibody_मोजणी,
				     handle->मोजणी - handle->ibody_मोजणी, start,
				     storage_size, start - block_buf, 1);
	जर (err)
		जा out2;

	/* Write a header on the EA block */
	header = (संरचना ext2_ext_attr_header *) block_buf;
	header->h_magic = EXT2_EXT_ATTR_MAGIC;
	header->h_refcount = 1;
	header->h_blocks = 1;

	/* Get a new block for writing */
	err = prep_ea_block_for_write(fs, handle->ino, inode);
	जर (err)
		जा out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_block(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, handle->ino);
	जर (err)
		जा out2;

skip_ea_block:
	blk = ext2fs_file_acl_block(fs, (संरचना ext2_inode *)inode);
	जर (!block_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		जर (err)
			जा out;
	}

	/* Write the inode */
	err = ext2fs_write_inode_full(fs, handle->ino, EXT2_INODE(inode),
				      inode_size);
	जर (err)
		जा out2;

out2:
	ext2fs_free_mem(&block_buf);
out:
	ext2fs_free_mem(&inode);
	परत err;
}

स्थिर errcode_t read_xattrs_from_buffer(संरचना ext2_xattr_handle *handle,
					 संरचना ext2_inode_large *inode,
					 संरचना ext2_ext_attr_entry *entries,
					 अचिन्हित पूर्णांक storage_size,
					 अक्षर *मूल्य_start)
{
	संरचना ext2_xattr *x;
	संरचना ext2_ext_attr_entry *entry, *end;
	स्थिरांक अक्षर *prefix;
	अचिन्हित पूर्णांक remain, prefix_len;
	errcode_t err;
	अचिन्हित पूर्णांक मूल्ये_size = storage_size +
			((अक्षर *)entries - मूल्य_start);

	/* find the end */
	end = entries;
	remain = storage_size;
	पर्यंत (remain >= आकार(संरचना ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(end)) {

		/* header eats this space */
		remain -= आकार(संरचना ext2_ext_attr_entry);

		/* is attribute name valid? */
		जर (EXT2_EXT_ATTR_SIZE(end->e_name_len) > remain)
			परत EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(end->e_name_len);
		end = EXT2_EXT_ATTR_NEXT(end);
	}

	entry = entries;
	remain = storage_size;
	पर्यंत (remain >= आकार(संरचना ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(entry)) {

		/* Allocate space for more attrs? */
		जर (handle->मोजणी == handle->capacity) {
			err = ext2fs_xattrs_expand(handle, 4);
			जर (err)
				परत err;
		}

		x = handle->attrs + handle->मोजणी;

		/* header eats this space */
		remain -= आकार(संरचना ext2_ext_attr_entry);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(entry->e_name_len);

		/* Extract name */
		prefix = find_ea_prefix(entry->e_name_index);
		prefix_len = (prefix ? म_माप(prefix) : 0);
		err = ext2fs_get_memzero(entry->e_name_len + prefix_len + 1,
					 &x->name);
		जर (err)
			परत err;
		जर (prefix)
			स_नकल(x->name, prefix, prefix_len);
		जर (entry->e_name_len)
			स_नकल(x->name + prefix_len,
			       (अक्षर *)entry + आकार(*entry),
			       entry->e_name_len);
		x->short_name = x->name + prefix_len;
		x->name_index = entry->e_name_index;

		/* Check & copy value */
		जर (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    entry->e_मूल्य_inum != 0)
			परत EXT2_ET_BAD_EA_BLOCK_अंक;

		जर (entry->e_मूल्य_inum == 0) {
			जर (entry->e_मूल्य_size > remain)
				परत EXT2_ET_EA_BAD_मूल्य_SIZE;

			जर (entry->e_मूल्य_offs + entry->e_मूल्य_size > मूल्ये_size)
				परत EXT2_ET_EA_BAD_मूल्य_OFFSET;

			जर (entry->e_मूल्य_size > 0 &&
			    मूल्य_start + entry->e_मूल्य_offs <
			    (अक्षर *)end + आकार(__u32))
				परत EXT2_ET_EA_BAD_मूल्य_OFFSET;

			remain -= entry->e_मूल्य_size;

			err = ext2fs_get_mem(entry->e_मूल्य_size, &x->मूल्य);
			जर (err)
				परत err;
			स_नकल(x->मूल्य, मूल्य_start + entry->e_मूल्य_offs,
			       entry->e_मूल्य_size);
		} अन्यथा {
			संरचना ext2_inode *ea_inode;
			ext2_file_t ea_file;

			जर (entry->e_मूल्य_offs != 0)
				परत EXT2_ET_EA_BAD_मूल्य_OFFSET;

			जर (entry->e_मूल्य_size > (64 * 1024))
				परत EXT2_ET_EA_BAD_मूल्य_SIZE;

			err = ext2fs_get_mem(entry->e_मूल्य_size, &x->मूल्य);
			जर (err)
				परत err;

			err = ext2fs_file_open(handle->fs, entry->e_मूल्य_inum,
					       0, &ea_file);
			जर (err)
				परत err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			जर ((ea_inode->i_flags & EXT4_INLINE_DATA_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_मोजणी == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			अन्यथा जर ((__u64) ext2fs_file_get_size(ea_file) !=
				 entry->e_मूल्य_size)
				err = EXT2_ET_EA_BAD_मूल्य_SIZE;
			अन्यथा
				err = ext2fs_file_read(ea_file, x->मूल्य,
						       entry->e_मूल्य_size, 0);
			ext2fs_file_close(ea_file);
			जर (err)
				परत err;
		}

		x->ea_ino = entry->e_मूल्य_inum;
		x->मूल्य_len = entry->e_मूल्य_size;

		/* e_hash may be 0 in older inode's ea */
		जर (entry->e_hash != 0) {
			__u32 hash, signed_hash;

			शून्य *data = (entry->e_मूल्य_inum != 0) ?
					0 : मूल्य_start + entry->e_मूल्य_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, entry,
							  data, &hash,
							  &signed_hash);
			जर (err)
				परत err;
			जर ((entry->e_hash != hash) &&
			    (entry->e_hash != signed_hash)) {
				संरचना ext2_inode child;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							entry->e_मूल्य_inum,
							&child);
				जर (err)
					परत err;
				जर (child.i_mtime != handle->ino ||
				    child.i_generation != inode->i_generation)
					परत EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->मोजणी++;
		entry = EXT2_EXT_ATTR_NEXT(entry);
	}

	परत 0;
}

स्थिर शून्य xattrs_free_किल्ल्या(संरचना ext2_xattr_handle *h)
{
	संरचना ext2_xattr *a = h->attrs;
	पूर्णांक i;

	साठी (i = 0; i < h->capacity; i++) {
		जर (a[i].name)
			ext2fs_free_mem(&a[i].name);
		जर (a[i].मूल्य)
			ext2fs_free_mem(&a[i].मूल्य);
	}
	h->मोजणी = 0;
	h->ibody_मोजणी = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(संरचना ext2_xattr_handle *handle,
				   संरचना ext2_inode_large *inode)
{
	संरचना ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	अचिन्हित पूर्णांक storage_size;
	अक्षर *start, *block_buf = रिक्त;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_किल्ल्या(handle);

	/* Does the inode have space for EA? */
	जर (inode->i_extra_isize < आकार(inode->i_extra_isize) ||
	    EXT2_INODE_SIZE(handle->fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  आकार(__u32))
		जा read_ea_block;
	जर (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		जा out;
	}

	/* Look for EA in the inode */
	स_नकल(&ea_inode_magic, ((अक्षर *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, आकार(__u32));
	जर (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_size = EXT2_INODE_SIZE(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_SIZE - inode->i_extra_isize -
			आकार(__u32);
		start = ((अक्षर *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + आकार(__u32);

		err = read_xattrs_from_buffer(handle, inode,
					(संरचना ext2_ext_attr_entry *) start,
					storage_size, start);
		जर (err)
			जा out;

		handle->ibody_मोजणी = handle->मोजणी;
	}

read_ea_block:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_block(handle->fs, EXT2_INODE(inode));
	जर (blk != 0) {
		जर ((blk < handle->fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_मोजणी(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_अंक;
			जा out;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &block_buf);
		जर (err)
			जा out;

		err = ext2fs_read_ext_attr3(handle->fs, blk, block_buf,
					    handle->ino);
		जर (err)
			जा out3;

		/* We only know how to deal with v2 EA blocks */
		header = (संरचना ext2_ext_attr_header *) block_buf;
		जर (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			जा out3;
		}

		/* Read EAs */
		storage_size = handle->fs->blocksize -
			आकार(संरचना ext2_ext_attr_header);
		start = block_buf + आकार(संरचना ext2_ext_attr_header);
		err = read_xattrs_from_buffer(handle, inode,
					(संरचना ext2_ext_attr_entry *) start,
					storage_size, block_buf);
	}

out3:
	जर (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	परत err;
}

errcode_t ext2fs_xattrs_read(संरचना ext2_xattr_handle *handle)
{
	संरचना ext2_inode_large *inode;
	माप_प्रकार inode_size = EXT2_INODE_SIZE(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	जर (inode_size < आकार(*inode))
		inode_size = आकार(*inode);
	err = ext2fs_get_memzero(inode_size, &inode);
	जर (err)
		परत err;

	err = ext2fs_read_inode_full(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_SIZE(handle->fs->super));
	जर (err)
		जा out;

	err = ext2fs_xattrs_read_inode(handle, inode);

out:
	ext2fs_free_mem(&inode);

	परत err;
}

errcode_t ext2fs_xattrs_iterate(संरचना ext2_xattr_handle *h,
				पूर्णांक (*func)(अक्षर *name, अक्षर *मूल्य,
					    माप_प्रकार मूल्य_len, शून्य *data),
				शून्य *data)
{
	संरचना ext2_xattr *x;
	पूर्णांक dirty = 0;
	पूर्णांक ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	साठी (x = h->attrs; x < h->attrs + h->मोजणी; x++) {
		ret = func(x->name, x->मूल्य, x->मूल्य_len, data);
		जर (ret & XATTR_CHANGED)
			dirty = 1;
		जर (ret & XATTR_ABORT)
			थांबा;
	}

	जर (dirty)
		परत ext2fs_xattrs_write(h);
	परत 0;
}

errcode_t ext2fs_xattr_get(संरचना ext2_xattr_handle *h, स्थिरांक अक्षर *किल्ली,
			   शून्य **मूल्य, माप_प्रकार *मूल्य_len)
{
	संरचना ext2_xattr *x;
	अक्षर *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	साठी (x = h->attrs; x < h->attrs + h->मोजणी; x++) {
		जर (म_भेद(x->name, किल्ली))
			सुरूठेवा;

		जर (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((म_भेद(किल्ली, "system.posix_acl_default") == 0) ||
		     (म_भेद(किल्ली, "system.posix_acl_access") == 0))) {
			err = convert_disk_buffer_to_posix_acl(x->मूल्य, x->मूल्य_len,
							       मूल्य, मूल्य_len);
			परत err;
		} अन्यथा {
			err = ext2fs_get_mem(x->मूल्य_len, &val);
			जर (err)
				परत err;
			स_नकल(val, x->मूल्य, x->मूल्य_len);
			*मूल्य = val;
			*मूल्य_len = x->मूल्य_len;
			परत 0;
		}
	}

	परत EXT2_ET_EA_किल्ली_NOT_FOUND;
}

errcode_t ext2fs_xattr_inode_max_size(ext2_filsys fs, ext2_ino_t ino,
				      माप_प्रकार *size)
{
	संरचना ext2_ext_attr_entry *entry;
	संरचना ext2_inode_large *inode;
	__u32 ea_inode_magic;
	अचिन्हित पूर्णांक minoff;
	अक्षर *start;
	माप_प्रकार i;
	errcode_t err;

	i = EXT2_INODE_SIZE(fs->super);
	जर (i < आकार(*inode))
		i = आकार(*inode);
	err = ext2fs_get_memzero(i, &inode);
	जर (err)
		परत err;

	err = ext2fs_read_inode_full(fs, ino, (संरचना ext2_inode *)inode,
				     EXT2_INODE_SIZE(fs->super));
	जर (err)
		जा out;

	/* Does the inode have size for EA? */
	जर (EXT2_INODE_SIZE(fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  आकार(__u32)) {
		err = EXT2_ET_INLINE_DATA_NO_SPACE;
		जा out;
	}

	minoff = EXT2_INODE_SIZE(fs->super) - आकार(*inode) - आकार(__u32);
	स_नकल(&ea_inode_magic, ((अक्षर *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, आकार(__u32));
	जर (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		start= ((अक्षर *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + आकार(__u32);
		entry = (संरचना ext2_ext_attr_entry *) start;
		पर्यंत (!EXT2_EXT_IS_LAST_ENTRY(entry)) {
			जर (!entry->e_मूल्य_inum && entry->e_मूल्य_size) {
				अचिन्हित पूर्णांक offs = entry->e_मूल्य_offs;
				जर (offs < minoff)
					minoff = offs;
			}
			entry = EXT2_EXT_ATTR_NEXT(entry);
		}
		*size = minoff - ((अक्षर *)entry - (अक्षर *)start) - आकार(__u32);
	} अन्यथा {
		/* no xattr.  return a maximum size */
		*size = EXT2_EXT_ATTR_SIZE(minoff -
					   EXT2_EXT_ATTR_LEN(म_माप("data")) -
					   EXT2_EXT_ATTR_ROUND - आकार(__u32));
	}

out:
	ext2fs_free_mem(&inode);
	परत err;
}

स्थिर errcode_t xattr_create_ea_inode(ext2_filsys fs, स्थिरांक शून्य *मूल्य,
				       माप_प्रकार मूल्य_len, ext2_ino_t *ea_ino)
{
	संरचना ext2_inode inode;
	ext2_ino_t ino;
	ext2_file_t file;
	__u32 hash;
	errcode_t ret;

	ret = ext2fs_new_inode(fs, 0, 0, 0, &ino);
	जर (ret)
		परत ret;

	स_रखो(&inode, 0, आकार(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	जर (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_size = 0;
	inode.i_mode = LINUX_S_IFREG | 0600;
	inode.i_links_मोजणी = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	जर (ret)
		परत ret;
	/*
	 * ref_count and hash utilize inode's i_*time fields.
	 * ext2fs_write_new_inode() call above initializes these fields with
	 * current time. That's why ref count and hash updates are done
	 * separately below.
	 */
	ext2fs_set_ea_inode_ref(&inode, 1);
	hash = ext2fs_crc32c_le(fs->csum_seed, मूल्य, मूल्य_len);
	ext2fs_set_ea_inode_hash(&inode, hash);

	ret = ext2fs_write_inode(fs, ino, &inode);
	जर (ret)
		परत ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	जर (ret)
		परत ret;
	ret = ext2fs_file_write(file, मूल्य, मूल्य_len, रिक्त);
	ext2fs_file_close(file);
	जर (ret)
		परत ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	परत 0;
}

स्थिर errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	संरचना ext2_inode_large inode;
	__u64 ref_मोजणी;
	errcode_t ret;

	ret = ext2fs_read_inode_full(fs, ino, (संरचना ext2_inode *)&inode,
				     आकार(inode));
	जर (ret)
		जा out;

	ref_मोजणी = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_मोजणी--;
	ext2fs_set_ea_inode_ref(EXT2_INODE(&inode), ref_मोजणी);

	जर (ref_मोजणी)
		जा write_out;

	inode.i_links_मोजणी = 0;
	ext2fs_set_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	जर (ret)
		जा write_out;

	जर (ext2fs_inode_has_valid_blocks2(fs, (संरचना ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (संरचना ext2_inode *)&inode, रिक्त,
				   0, ~0ULL);
		जर (ret)
			जा out;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_out:
	ret = ext2fs_write_inode_full(fs, ino, (संरचना ext2_inode *)&inode,
				      आकार(inode));
out:
	परत ret;
}

स्थिर errcode_t xattr_update_entry(ext2_filsys fs, संरचना ext2_xattr *x,
				    स्थिरांक अक्षर *name, स्थिरांक अक्षर *short_name,
				    पूर्णांक index, स्थिरांक शून्य *मूल्य,
				    माप_प्रकार मूल्य_len, पूर्णांक in_inode)
{
	ext2_ino_t ea_ino = 0;
	शून्य *new_मूल्य = रिक्त;
	अक्षर *new_name = रिक्त;
	पूर्णांक name_len;
	errcode_t ret;

	जर (!x->name) {
		name_len = म_माप(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		जर (ret)
			जा fail;
		स_नकल(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(मूल्य_len, &new_मूल्य);
	जर (ret)
		जा fail;
	स_नकल(new_मूल्य, मूल्य, मूल्य_len);

	जर (in_inode) {
		ret = xattr_create_ea_inode(fs, मूल्य, मूल्य_len, &ea_ino);
		जर (ret)
			जा fail;
	}

	जर (x->ea_ino) {
		ret = xattr_inode_dec_ref(fs, x->ea_ino);
		जर (ret)
			जा fail;
	}

	जर (!x->name) {
		x->name = new_name;
		x->short_name = new_name + (short_name  - name);
	}
	x->name_index = index;

	जर (x->मूल्य)
		ext2fs_free_mem(&x->मूल्य);
	x->मूल्य = new_मूल्य;
	x->मूल्य_len = मूल्य_len;
	x->ea_ino = ea_ino;
	परत 0;
fail:
	जर (new_name)
		ext2fs_free_mem(&new_name);
	जर (new_मूल्य)
		ext2fs_free_mem(&new_मूल्य);
	जर (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	परत ret;
}

स्थिर पूर्णांक xattr_find_position(संरचना ext2_xattr *attrs, पूर्णांक मोजणी,
			       स्थिरांक अक्षर *shortname, पूर्णांक name_idx)
{
	संरचना ext2_xattr *x;
	पूर्णांक i;
	पूर्णांक shortname_len, x_shortname_len;

	shortname_len = म_माप(shortname);

	साठी (i = 0, x = attrs; i < मोजणी; i++, x++) {
		जर (name_idx < x->name_index)
			थांबा;
		जर (name_idx > x->name_index)
			सुरूठेवा;

		x_shortname_len = म_माप(x->short_name);
		जर (shortname_len < x_shortname_len)
			थांबा;
		जर (shortname_len > x_shortname_len)
			सुरूठेवा;

		जर (स_भेद(shortname, x->short_name, shortname_len) <= 0)
			थांबा;
	}
	परत i;
}

स्थिर errcode_t xattr_array_update(संरचना ext2_xattr_handle *h,
				    स्थिरांक अक्षर *name,
				    स्थिरांक शून्य *मूल्य, माप_प्रकार मूल्य_len,
				    पूर्णांक ibody_free, पूर्णांक block_free,
				    पूर्णांक old_idx, पूर्णांक in_inode)
{
	संरचना ext2_xattr tmp;
	पूर्णांक add_to_ibody;
	पूर्णांक needed;
	पूर्णांक name_len, name_idx = 0;
	स्थिरांक अक्षर *shortname = name;
	पूर्णांक new_idx;
	पूर्णांक ret;

	find_ea_index(name, &shortname, &name_idx);
	name_len = म_माप(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	जर (!in_inode)
		needed += EXT2_EXT_ATTR_SIZE(मूल्य_len);

	जर (old_idx >= 0 && old_idx < h->ibody_मोजणी) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		जर (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_SIZE(
						h->attrs[old_idx].मूल्य_len);
	}

	जर (needed <= ibody_free) {
		जर (old_idx < 0) {
			new_idx = h->ibody_मोजणी;
			add_to_ibody = 1;
			जा add_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, मूल्य,
					 मूल्य_len, in_inode);
		जर (ret)
			परत ret;
		जर (h->ibody_मोजणी <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			स_हटाओ(h->attrs + h->ibody_मोजणी + 1,
				h->attrs + h->ibody_मोजणी,
				(old_idx - h->ibody_मोजणी) * आकार(*h->attrs));
			h->attrs[h->ibody_मोजणी] = tmp;
			h->ibody_मोजणी++;
		}
		परत 0;
	}

	जर (h->ibody_मोजणी <= old_idx) {
		block_free += EXT2_EXT_ATTR_LEN(name_len);
		जर (!h->attrs[old_idx].ea_ino)
			block_free +=
				EXT2_EXT_ATTR_SIZE(h->attrs[old_idx].मूल्य_len);
	}

	जर (needed > block_free)
		परत EXT2_ET_EA_NO_SPACE;

	जर (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, मूल्य,
					 मूल्य_len, in_inode);
		जर (ret)
			परत ret;
		जर (old_idx < h->ibody_मोजणी) {
			/*
			 * Move entry from ibody to the block. Note that
			 * entries in the block are sorted.
			 */
			new_idx = xattr_find_position(h->attrs + h->ibody_मोजणी,
						      h->मोजणी - h->ibody_मोजणी,
						      shortname, name_idx);
			new_idx += h->ibody_मोजणी - 1;
			tmp = h->attrs[old_idx];
			स_हटाओ(h->attrs + old_idx, h->attrs + old_idx + 1,
				(new_idx - old_idx) * आकार(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_मोजणी--;
		}
		परत 0;
	}

	new_idx = xattr_find_position(h->attrs + h->ibody_मोजणी,
				      h->मोजणी - h->ibody_मोजणी,
				      shortname, name_idx);
	new_idx += h->ibody_मोजणी;
	add_to_ibody = 0;

add_new:
	जर (h->मोजणी == h->capacity) {
		ret = ext2fs_xattrs_expand(h, 4);
		जर (ret)
			परत ret;
	}

	ret = xattr_update_entry(h->fs, &h->attrs[h->मोजणी], name, shortname,
				 name_idx, मूल्य, मूल्य_len, in_inode);
	जर (ret)
		परत ret;

	tmp = h->attrs[h->मोजणी];
	स_हटाओ(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->मोजणी - new_idx)*आकार(*h->attrs));
	h->attrs[new_idx] = tmp;
	जर (add_to_ibody)
		h->ibody_मोजणी++;
	h->मोजणी++;
	परत 0;
}

स्थिर पूर्णांक space_used(संरचना ext2_xattr *attrs, पूर्णांक मोजणी)
{
	पूर्णांक total = 0;
	संरचना ext2_xattr *x;
	पूर्णांक i, len;

	साठी (i = 0, x = attrs; i < मोजणी; i++, x++) {
		len = म_माप(x->short_name);
		total += EXT2_EXT_ATTR_LEN(len);
		जर (!x->ea_ino)
			total += EXT2_EXT_ATTR_SIZE(x->मूल्य_len);
	}
	परत total;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#define EXT4_XATTR_MIN_LARGE_EA_SIZE(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - आकार(संरचना ext2_ext_attr_header) - 4)

errcode_t ext2fs_xattr_set(संरचना ext2_xattr_handle *h,
			   स्थिरांक अक्षर *name,
			   स्थिरांक शून्य *मूल्य,
			   माप_प्रकार मूल्य_len)
{
	ext2_filsys fs = h->fs;
	स्थिरांक पूर्णांक inode_size = EXT2_INODE_SIZE(fs->super);
	संरचना ext2_inode_large *inode = रिक्त;
	संरचना ext2_xattr *x;
	अक्षर *new_मूल्य;
	पूर्णांक ibody_free, block_free;
	पूर्णांक in_inode = 0;
	पूर्णांक old_idx = -1;
	पूर्णांक extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(मूल्य_len, &new_मूल्य);
	जर (ret)
		परत ret;
	जर (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((म_भेद(name, "system.posix_acl_default") == 0) ||
	     (म_भेद(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_to_disk_buffer(मूल्य, मूल्य_len,
						       new_मूल्य, &मूल्य_len);
		जर (ret)
			जा out;
	} अन्यथा जर (मूल्य_len)
		स_नकल(new_मूल्य, मूल्य, मूल्य_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	साठी (x = h->attrs; x < h->attrs + h->मोजणी; x++) {
		जर (!म_भेद(x->name, name)) {
			जर (!x->ea_ino && x->मूल्य_len == मूल्य_len &&
			    (!मूल्य_len ||
			     !स_भेद(x->मूल्य, new_मूल्य, मूल्य_len))) {
				ret = 0;
				जा out;
			}
			old_idx = x - h->attrs;
			थांबा;
		}
	}

	ret = ext2fs_get_memzero(inode_size, &inode);
	जर (ret)
		जा out;
	ret = ext2fs_read_inode_full(fs, h->ino,
				     (संरचना ext2_inode *)inode,
				     inode_size);
	जर (ret)
		जा out;
	जर (inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		extra_isize = inode->i_extra_isize;
		जर (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			जर (extra_isize == 0)
				extra_isize = आकार(__u32);
		}
		ibody_free = inode_size - EXT2_GOOD_OLD_INODE_SIZE;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= आकार(__u32) * 2;
		ibody_free -= space_used(h->attrs, h->ibody_मोजणी);
	} अन्यथा
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	जर (म_भेद(name, "system.data") == 0) {
		जर (h->ibody_मोजणी <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			जा out;
		}
		ret = xattr_array_update(h, name, new_मूल्य, मूल्य_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		जर (ret)
			जा out;
		जा write_out;
	}

	block_free = fs->blocksize;
	block_free -= आकार(संरचना ext2_ext_attr_header);
	/* Final null entry. */
	block_free -= आकार(__u32);
	block_free -= space_used(h->attrs + h->ibody_मोजणी,
				 h->मोजणी - h->ibody_मोजणी);

	जर (ext2fs_has_feature_ea_inode(fs->super) &&
	    मूल्य_len > EXT4_XATTR_MIN_LARGE_EA_SIZE(fs->blocksize))
		in_inode = 1;

	ret = xattr_array_update(h, name, new_मूल्य, मूल्य_len, ibody_free,
				 block_free, old_idx, in_inode);
	जर (ret == EXT2_ET_EA_NO_SPACE && !in_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_मूल्य, मूल्य_len,
			ibody_free, block_free, old_idx, 1 /* in_inode */);
	जर (ret)
		जा out;

write_out:
	ret = ext2fs_xattrs_write(h);
out:
	जर (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_मूल्य);
	परत ret;
}

errcode_t ext2fs_xattr_remove(संरचना ext2_xattr_handle *handle,
			      स्थिरांक अक्षर *किल्ली)
{
	संरचना ext2_xattr *x;
	संरचना ext2_xattr *end = handle->attrs + handle->मोजणी;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	साठी (x = handle->attrs; x < end; x++) {
		जर (म_भेद(x->name, किल्ली) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->मूल्य);
			जर (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			स_हटाओ(x, x + 1, (end - x - 1)*आकार(*x));
			स_रखो(end - 1, 0, आकार(*end));
			जर (x < handle->attrs + handle->ibody_मोजणी)
				handle->ibody_मोजणी--;
			handle->मोजणी--;
			परत ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	परत 0;
}

errcode_t ext2fs_xattr_remove_all(संरचना ext2_xattr_handle *handle)
{
	संरचना ext2_xattr *x;
	संरचना ext2_xattr *end = handle->attrs + handle->मोजणी;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	साठी (x = handle->attrs; x < end; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->मूल्य);
		जर (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_मोजणी = 0;
	handle->मोजणी = 0;
	परत ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     संरचना ext2_xattr_handle **handle)
{
	संरचना ext2_xattr_handle *h;
	errcode_t err;

	जर (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_data(fs->super))
		परत EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(आकार(*h), &h);
	जर (err)
		परत err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->capacity = 4;
	err = ext2fs_get_arrayzero(h->capacity, आकार(संरचना ext2_xattr),
				   &h->attrs);
	जर (err) {
		ext2fs_free_mem(&h);
		परत err;
	}
	h->मोजणी = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	परत 0;
}

errcode_t ext2fs_xattrs_close(संरचना ext2_xattr_handle **handle)
{
	संरचना ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_किल्ल्या(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	परत 0;
}

errcode_t ext2fs_xattrs_मोजणी(संरचना ext2_xattr_handle *handle, माप_प्रकार *मोजणी)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*मोजणी = handle->मोजणी;
	परत 0;
}

errcode_t ext2fs_xattrs_flags(संरचना ext2_xattr_handle *handle,
			      अचिन्हित पूर्णांक *new_flags, अचिन्हित पूर्णांक *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	जर (old_flags)
		*old_flags = handle->flags;
	जर (new_flags)
		handle->flags = *new_flags;
	परत 0;
}
