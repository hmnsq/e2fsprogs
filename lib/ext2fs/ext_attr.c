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

#समावेश "config.h"
#समावेश <stdio.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <string.h>
#समावेश <time.h>

#समावेश "ext2_fs.h"
#समावेश "ext2_ext_attr.h"
#समावेश "ext4_acl.h"

#समावेश "ext2fsP.h"

अटल errcode_t read_ea_inode_hash(ext2_filsys fs, ext2_ino_t ino, __u32 *hash)
{
	काष्ठा ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	अगर (retval)
		वापस retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	वापस 0;
}

#घोषणा NAME_HASH_SHIFT 5
#घोषणा मूल्य_HASH_SHIFT 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_entry(काष्ठा ext2_ext_attr_entry *entry, व्योम *तथ्य)
{
	__u32 hash = 0;
	अचिन्हित अक्षर *name = (((अचिन्हित अक्षर *) entry) +
			       माप(काष्ठा ext2_ext_attr_entry));
	पूर्णांक n;

	क्रम (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*माप(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	अगर (entry->e_मूल्य_inum == 0 && entry->e_मूल्य_आकार != 0) {
		__u32 *मूल्य = (__u32 *)तथ्य;
		क्रम (n = (entry->e_मूल्य_आकार + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << मूल्य_HASH_SHIFT) ^
			       (hash >> (8*माप(hash) - मूल्य_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*मूल्य++);
		}
	}

	वापस hash;
}

__u32 ext2fs_ext_attr_hash_entry_signed(काष्ठा ext2_ext_attr_entry *entry,
					व्योम *तथ्य)
{
	__u32 hash = 0;
	चिन्हित अक्षर *name = (((चिन्हित अक्षर *) entry) +
			     माप(काष्ठा ext2_ext_attr_entry));
	पूर्णांक n;

	क्रम (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*माप(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	अगर (entry->e_मूल्य_inum == 0 && entry->e_मूल्य_आकार != 0) {
		__u32 *मूल्य = (__u32 *)तथ्य;
		क्रम (n = (entry->e_मूल्य_आकार + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << मूल्य_HASH_SHIFT) ^
			       (hash >> (8*माप(hash) - मूल्य_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*मूल्य++);
		}
	}

	वापस hash;
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
				      काष्ठा ext2_ext_attr_entry *entry,
				      व्योम *तथ्य, __u32 *hash,
				      __u32 *signed_hash)
{
	*hash = ext2fs_ext_attr_hash_entry(entry, तथ्य);
	अगर (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_entry_signed(entry, तथ्य);

	अगर (entry->e_मूल्य_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, entry->e_मूल्य_inum,
					    &ea_inode_hash);
		अगर (retval)
			वापस retval;

		*hash = (*hash << मूल्य_HASH_SHIFT) ^
			(*hash >> (8*माप(*hash) - मूल्य_HASH_SHIFT)) ^
			ea_inode_hash;
		अगर (signed_hash)
			*signed_hash = (*signed_hash << मूल्य_HASH_SHIFT) ^
				(*signed_hash >> (8*माप(*hash) -
						  मूल्य_HASH_SHIFT)) ^
				ea_inode_hash;
	}
	वापस 0;
}

/*
 * ext2fs_ext_attr_hash_entry2()
 *
 * Compute the hash of an extended attribute.
 * This version of the function supports hashing entries that reference
 * external inodes (ea_inode feature).
 */
errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
				      काष्ठा ext2_ext_attr_entry *entry,
				      व्योम *तथ्य, __u32 *hash)
{
	वापस ext2fs_ext_attr_hash_entry3(fs, entry, तथ्य, hash, शून्य);
}

#अघोषित NAME_HASH_SHIFT
#अघोषित मूल्य_HASH_SHIFT

#घोषणा BLOCK_HASH_SHIFT 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
व्योम ext2fs_ext_attr_block_rehash(काष्ठा ext2_ext_attr_header *header,
				  काष्ठा ext2_ext_attr_entry *end)
{
	काष्ठा ext2_ext_attr_entry *here;
	__u32 hash = 0;

	here = (काष्ठा ext2_ext_attr_entry *)(header+1);
	जबतक (here < end && !EXT2_EXT_IS_LAST_ENTRY(here)) {
		अगर (!here->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			अवरोध;
		}
		hash = (hash << BLOCK_HASH_SHIFT) ^
		       (hash >> (8*माप(hash) - BLOCK_HASH_SHIFT)) ^
		       here->e_hash;
		here = EXT2_EXT_ATTR_अग्र(here);
	}
	header->h_hash = hash;
}

#अघोषित BLOCK_HASH_SHIFT

__u32 ext2fs_get_ea_inode_hash(काष्ठा ext2_inode *inode)
{
	वापस inode->i_atime;
}

व्योम ext2fs_set_ea_inode_hash(काष्ठा ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(काष्ठा ext2_inode *inode)
{
	वापस ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_version;
}

व्योम ext2fs_set_ea_inode_ref(काष्ठा ext2_inode *inode, __u64 ref_गिनती)
{
	inode->i_ctime = (__u32)(ref_गिनती >> 32);
	inode->osd1.linux1.l_i_version = (__u32)ref_गिनती;
}

अटल errcode_t check_ext_attr_header(काष्ठा ext2_ext_attr_header *header)
{
	अगर ((header->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     header->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    header->h_blocks != 1)
		वापस EXT2_ET_BAD_EA_HEADER;

	वापस 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block, व्योम *buf,
				ext2_ino_t inum)
{
	पूर्णांक		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	अगर (retval)
		वापस retval;

	अगर (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_block_csum_verify(fs, inum, block, buf))
		csum_failed = 1;

#अगर_घोषित शब्दगण_BIGENDIAN
	ext2fs_swap_ext_attr(buf, buf, fs->blocksize, 1);
#पूर्ण_अगर

	retval = check_ext_attr_header(buf);
	अगर (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	वापस retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block, व्योम *buf)
{
	वापस ext2fs_read_ext_attr3(fs, block, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, व्योम *buf)
{
	वापस ext2fs_read_ext_attr2(fs, block, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block, व्योम *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	अक्षर		*write_buf;

#अगर_घोषित शब्दगण_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	अगर (retval)
		वापस retval;
	ext2fs_swap_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#अन्यथा
	write_buf = (अक्षर *) inbuf;
#पूर्ण_अगर

	retval = ext2fs_ext_attr_block_csum_set(fs, inum, block,
			(काष्ठा ext2_ext_attr_header *)write_buf);
	अगर (retval)
		वापस retval;

	retval = io_channel_write_blk64(fs->io, block, 1, write_buf);
#अगर_घोषित शब्दगण_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#पूर्ण_अगर
	अगर (!retval)
		ext2fs_mark_changed(fs);
	वापस retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block, व्योम *inbuf)
{
	वापस ext2fs_write_ext_attr3(fs, block, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block, व्योम *inbuf)
{
	वापस ext2fs_write_ext_attr2(fs, block, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    अक्षर *block_buf, पूर्णांक adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	काष्ठा ext2_ext_attr_header *header;
	अक्षर	*buf = 0;

	अगर ((blk >= ext2fs_blocks_गिनती(fs->super)) ||
	    (blk < fs->super->s_first_तथ्य_block))
		वापस EXT2_ET_BAD_EA_BLOCK_अंक;

	अगर (!block_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		अगर (retval)
			वापस retval;
		block_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, block_buf, inum);
	अगर (retval)
		जाओ errout;

	header = (काष्ठा ext2_ext_attr_header *) block_buf;
	header->h_refcount += adjust;
	अगर (newcount)
		*newcount = header->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, block_buf, inum);
	अगर (retval)
		जाओ errout;

errout:
	अगर (buf)
		ext2fs_free_mem(&buf);
	वापस retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    अक्षर *block_buf, पूर्णांक adjust,
				    __u32 *newcount)
{
	वापस ext2fs_adjust_ea_refcount3(fs, blk, block_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					अक्षर *block_buf, पूर्णांक adjust,
					__u32 *newcount)
{
	वापस ext2fs_adjust_ea_refcount2(fs, blk, block_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
काष्ठा ext2_xattr {
	पूर्णांक name_सूचकांक;
	अक्षर *name;
	अक्षर *short_name;
	व्योम *मूल्य;
	अचिन्हित पूर्णांक मूल्य_len;
	ext2_ino_t ea_ino;
};

काष्ठा ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	काष्ठा ext2_xattr *attrs;
	पूर्णांक capacity;
	पूर्णांक गिनती;
	पूर्णांक ibody_गिनती;
	ext2_ino_t ino;
	अचिन्हित पूर्णांक flags;
};

अटल errcode_t ext2fs_xattrs_expand(काष्ठा ext2_xattr_handle *h,
				      अचिन्हित पूर्णांक expandby)
{
	काष्ठा ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->capacity + expandby,
				   माप(काष्ठा ext2_xattr), &new_attrs);
	अगर (err)
		वापस err;

	स_नकल(new_attrs, h->attrs, h->capacity * माप(काष्ठा ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->capacity += expandby;
	h->attrs = new_attrs;

	वापस 0;
}

काष्ठा ea_name_सूचकांक {
	पूर्णांक सूचकांक;
	स्थिर अक्षर *name;
};

/* Keep these names sorted in order of decreasing specificity. */
अटल काष्ठा ea_name_सूचकांक ea_names[] = {
	{10, "gnu."},
	{3, "system.posix_acl_default"},
	{2, "system.posix_acl_access"},
	{8, "system.richacl"},
	{6, "security."},
	{4, "trusted."},
	{7, "system."},
	{1, "user."},
	{0, शून्य},
};

अटल स्थिर अक्षर *find_ea_prefix(पूर्णांक सूचकांक)
{
	काष्ठा ea_name_सूचकांक *e;

	क्रम (e = ea_names; e->name; e++)
		अगर (e->सूचकांक == सूचकांक)
			वापस e->name;

	वापस शून्य;
}

अटल पूर्णांक find_ea_सूचकांक(स्थिर अक्षर *fullname, स्थिर अक्षर **name, पूर्णांक *सूचकांक)
{
	काष्ठा ea_name_सूचकांक *e;

	क्रम (e = ea_names; e->name; e++) {
		अगर (म_भेदन(fullname, e->name, म_माप(e->name)) == 0) {
			*name = fullname + म_माप(e->name);
			*सूचकांक = e->सूचकांक;
			वापस 1;
		}
	}
	वापस 0;
}

errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       काष्ठा ext2_inode_large *inode)
{
	काष्ठा ext2_ext_attr_header *header;
	व्योम *block_buf = शून्य;
	blk64_t blk;
	errcode_t err;
	काष्ठा ext2_inode_large i;

	/* Read inode? */
	अगर (inode == शून्य) {
		err = ext2fs_read_inode_full(fs, ino, (काष्ठा ext2_inode *)&i,
					     माप(काष्ठा ext2_inode_large));
		अगर (err)
			वापस err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (काष्ठा ext2_inode *)inode);
	अगर (blk == 0)
		वापस 0;

	/* Find block, zero it, write back */
	अगर ((blk < fs->super->s_first_तथ्य_block) ||
	    (blk >= ext2fs_blocks_गिनती(fs->super))) {
		err = EXT2_ET_BAD_EA_BLOCK_अंक;
		जाओ out;
	}

	err = ext2fs_get_mem(fs->blocksize, &block_buf);
	अगर (err)
		जाओ out;

	err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
	अगर (err)
		जाओ out2;

	/* We only know how to deal with v2 EA blocks */
	header = (काष्ठा ext2_ext_attr_header *) block_buf;
	अगर (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_HEADER;
		जाओ out2;
	}

	header->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
	अगर (err)
		जाओ out2;

	/* Erase link to block */
	ext2fs_file_acl_block_set(fs, (काष्ठा ext2_inode *)inode, 0);
	अगर (header->h_refcount == 0)
		ext2fs_block_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_blocks(fs, (काष्ठा ext2_inode *)inode, 1);
	अगर (err)
		जाओ out2;

	/* Write inode? */
	अगर (inode == &i) {
		err = ext2fs_write_inode_full(fs, ino, (काष्ठा ext2_inode *)&i,
					      माप(काष्ठा ext2_inode_large));
		अगर (err)
			जाओ out2;
	}

out2:
	ext2fs_free_mem(&block_buf);
out:
	वापस err;
}

अटल errcode_t prep_ea_block_for_write(ext2_filsys fs, ext2_ino_t ino,
					 काष्ठा ext2_inode_large *inode)
{
	काष्ठा ext2_ext_attr_header *header;
	व्योम *block_buf = शून्य;
	blk64_t blk, goal;
	errcode_t err;

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (काष्ठा ext2_inode *)inode);
	अगर (blk != 0) {
		अगर ((blk < fs->super->s_first_तथ्य_block) ||
		    (blk >= ext2fs_blocks_गिनती(fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_अंक;
			जाओ out;
		}

		err = ext2fs_get_mem(fs->blocksize, &block_buf);
		अगर (err)
			जाओ out;

		err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
		अगर (err)
			जाओ out2;

		/* We only know how to deal with v2 EA blocks */
		header = (काष्ठा ext2_ext_attr_header *) block_buf;
		अगर (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			जाओ out2;
		}

		/* Single-user block.  We're done here. */
		अगर (header->h_refcount == 1)
			जाओ out2;

		/* We need to CoW the block. */
		header->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
		अगर (err)
			जाओ out2;
	} अन्यथा {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_add_blocks(fs, (काष्ठा ext2_inode *)inode,
					     1);
		अगर (err)
			जाओ out;
	}

	/* Allocate a block */
	goal = ext2fs_find_inode_goal(fs, ino, (काष्ठा ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, शून्य, &blk);
	अगर (err)
		जाओ out2;
	ext2fs_file_acl_block_set(fs, (काष्ठा ext2_inode *)inode, blk);
out2:
	अगर (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	वापस err;
}


अटल अंतरभूत पूर्णांक
posix_acl_xattr_गिनती(माप_प्रकार आकार)
{
        अगर (आकार < माप(posix_acl_xattr_header))
                वापस -1;
        आकार -= माप(posix_acl_xattr_header);
        अगर (आकार % माप(posix_acl_xattr_entry))
                वापस -1;
        वापस आकार / माप(posix_acl_xattr_entry);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
अटल errcode_t convert_posix_acl_to_disk_बफर(स्थिर व्योम *मूल्य, माप_प्रकार आकार,
						  व्योम *out_buf, माप_प्रकार *आकार_out)
{
	स्थिर posix_acl_xattr_header *header =
		(स्थिर posix_acl_xattr_header*) मूल्य;
	स्थिर posix_acl_xattr_entry *end, *entry =
		(स्थिर posix_acl_xattr_entry *)(header+1);
	ext4_acl_header *ext_acl;
	माप_प्रकार s;
	अक्षर *e;

	पूर्णांक गिनती;

	अगर (!मूल्य)
		वापस EINVAL;
	अगर (आकार < माप(posix_acl_xattr_header))
		वापस ENOMEM;
	अगर (header->a_version != ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION))
		वापस EINVAL;

	गिनती = posix_acl_xattr_गिनती(आकार);
	ext_acl = out_buf;
	ext_acl->a_version = ext2fs_cpu_to_le32(EXT4_ACL_VERSION);

	अगर (गिनती <= 0)
		वापस EINVAL;

	e = (अक्षर *) out_buf + माप(ext4_acl_header);
	s = माप(ext4_acl_header);
	क्रम (end = entry + गिनती; entry != end;entry++) {
		ext4_acl_entry *disk_entry = (ext4_acl_entry*) e;
		disk_entry->e_tag = entry->e_tag;
		disk_entry->e_perm = entry->e_perm;

		चयन(ext2fs_le16_to_cpu(entry->e_tag)) {
			हाल ACL_USER_OBJ:
			हाल ACL_GROUP_OBJ:
			हाल ACL_MASK:
			हाल ACL_OTHER:
				e += माप(ext4_acl_entry_short);
				s += माप(ext4_acl_entry_short);
				अवरोध;
			हाल ACL_USER:
			हाल ACL_GROUP:
				disk_entry->e_id = entry->e_id;
				e += माप(ext4_acl_entry);
				s += माप(ext4_acl_entry);
				अवरोध;
			शेष:
				वापस EINVAL;
		}
	}
	*आकार_out = s;
	वापस 0;
}

अटल errcode_t convert_disk_बफर_to_posix_acl(स्थिर व्योम *मूल्य, माप_प्रकार आकार,
						  व्योम **out_buf, माप_प्रकार *आकार_out)
{
	posix_acl_xattr_header *header;
	posix_acl_xattr_entry *entry;
	स्थिर ext4_acl_header *ext_acl = (स्थिर ext4_acl_header *) मूल्य;
	errcode_t err;
	स्थिर अक्षर *cp;
	अक्षर *out;

	अगर ((!मूल्य) ||
	    (आकार < माप(ext4_acl_header)) ||
	    (ext_acl->a_version != ext2fs_cpu_to_le32(EXT4_ACL_VERSION)))
		वापस EINVAL;

	err = ext2fs_get_mem(आकार * 2, &out);
	अगर (err)
		वापस err;

	header = (posix_acl_xattr_header *) out;
	header->a_version = ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION);
	entry = (posix_acl_xattr_entry *) (out + माप(posix_acl_xattr_header));

	cp = (स्थिर अक्षर *) मूल्य + माप(ext4_acl_header);
	आकार -= माप(ext4_acl_header);

	जबतक (आकार > 0) {
		स्थिर ext4_acl_entry *disk_entry = (स्थिर ext4_acl_entry *) cp;

		entry->e_tag = disk_entry->e_tag;
		entry->e_perm = disk_entry->e_perm;

		चयन(ext2fs_le16_to_cpu(entry->e_tag)) {
			हाल ACL_USER_OBJ:
			हाल ACL_GROUP_OBJ:
			हाल ACL_MASK:
			हाल ACL_OTHER:
				entry->e_id = 0;
				cp += माप(ext4_acl_entry_short);
				आकार -= माप(ext4_acl_entry_short);
				अवरोध;
			हाल ACL_USER:
			हाल ACL_GROUP:
				entry->e_id = disk_entry->e_id;
				cp += माप(ext4_acl_entry);
				आकार -= माप(ext4_acl_entry);
				अवरोध;
			शेष:
				ext2fs_free_mem(&out);
				वापस EINVAL;
		}
		entry++;
	}
	*out_buf = out;
	*आकार_out = ((अक्षर *) entry - out);
	वापस 0;
}

अटल errcode_t
write_xattrs_to_बफर(ext2_filsys fs, काष्ठा ext2_xattr *attrs, पूर्णांक गिनती,
		       व्योम *entries_start, अचिन्हित पूर्णांक storage_आकार,
		       अचिन्हित पूर्णांक मूल्य_offset_correction, पूर्णांक write_hash)
{
	काष्ठा ext2_xattr *x;
	काष्ठा ext2_ext_attr_entry *e = entries_start;
	अक्षर *end = (अक्षर *) entries_start + storage_आकार;
	अचिन्हित पूर्णांक मूल्य_आकार;
	errcode_t err;

	स_रखो(entries_start, 0, storage_आकार);
	क्रम (x = attrs; x < attrs + गिनती; x++) {
		मूल्य_आकार = ((x->मूल्य_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = म_माप(x->short_name);
		e->e_name_सूचकांक = x->name_सूचकांक;

		e->e_मूल्य_आकार = x->मूल्य_len;
		e->e_मूल्य_inum = x->ea_ino;

		/* Store name */
		स_नकल((अक्षर *)e + माप(*e), x->short_name, e->e_name_len);
		अगर (x->ea_ino) {
			e->e_मूल्य_offs = 0;
		} अन्यथा {
			end -= मूल्य_आकार;
			e->e_मूल्य_offs = end - (अक्षर *) entries_start +
						मूल्य_offset_correction;
			स_नकल(end, x->मूल्य, e->e_मूल्य_आकार);
		}

		अगर (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : end,
							  &e->e_hash);
			अगर (err)
				वापस err;
		} अन्यथा
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_अग्र(e);
		*(__u32 *)e = 0;
	}
	वापस 0;
}

errcode_t ext2fs_xattrs_write(काष्ठा ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	स्थिर अचिन्हित पूर्णांक inode_आकार = EXT2_INODE_आकार(fs->super);
	काष्ठा ext2_inode_large *inode;
	अक्षर *start, *block_buf = शून्य;
	काष्ठा ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	blk64_t blk;
	अचिन्हित पूर्णांक storage_आकार;
	अचिन्हित पूर्णांक i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_आकार;
	अगर (i < माप(*inode))
		i = माप(*inode);
	err = ext2fs_get_memzero(i, &inode);
	अगर (err)
		वापस err;

	err = ext2fs_read_inode_full(fs, handle->ino, EXT2_INODE(inode),
				     inode_आकार);
	अगर (err)
		जाओ out;

	/* If extra_isize isn't set, we need to set it now */
	अगर (inode->i_extra_isize == 0 &&
	    inode_आकार > EXT2_GOOD_OLD_INODE_आकार) {
		अक्षर *p = (अक्षर *)inode;
		माप_प्रकार extra = fs->super->s_want_extra_isize;

		अगर (extra == 0)
			extra = माप(__u32);
		स_रखो(p + EXT2_GOOD_OLD_INODE_आकार, 0, extra);
		inode->i_extra_isize = extra;
	}
	अगर (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		जाओ out;
	}

	/* Does the inode have space for EA? */
	अगर (inode->i_extra_isize < माप(inode->i_extra_isize) ||
	    inode_आकार <= EXT2_GOOD_OLD_INODE_आकार + inode->i_extra_isize +
								माप(__u32))
		जाओ write_ea_block;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	स_नकल(((अक्षर *) inode) + EXT2_GOOD_OLD_INODE_आकार +
	       inode->i_extra_isize, &ea_inode_magic, माप(__u32));
	storage_आकार = inode_आकार - EXT2_GOOD_OLD_INODE_आकार -
				inode->i_extra_isize - माप(__u32);
	start = ((अक्षर *) inode) + EXT2_GOOD_OLD_INODE_आकार +
				inode->i_extra_isize + माप(__u32);

	err = write_xattrs_to_बफर(fs, handle->attrs, handle->ibody_गिनती,
				     start, storage_आकार, 0, 0);
	अगर (err)
		जाओ out;
write_ea_block:
	/* Are we done? */
	अगर (handle->ibody_गिनती == handle->गिनती &&
	    !ext2fs_file_acl_block(fs, EXT2_INODE(inode)))
		जाओ skip_ea_block;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &block_buf);
	अगर (err)
		जाओ out;

	storage_आकार = fs->blocksize - माप(काष्ठा ext2_ext_attr_header);
	start = block_buf + माप(काष्ठा ext2_ext_attr_header);

	err = write_xattrs_to_बफर(fs, handle->attrs + handle->ibody_गिनती,
				     handle->गिनती - handle->ibody_गिनती, start,
				     storage_आकार, start - block_buf, 1);
	अगर (err)
		जाओ out2;

	/* Write a header on the EA block */
	header = (काष्ठा ext2_ext_attr_header *) block_buf;
	header->h_magic = EXT2_EXT_ATTR_MAGIC;
	header->h_refcount = 1;
	header->h_blocks = 1;

	/* Get a new block for writing */
	err = prep_ea_block_for_write(fs, handle->ino, inode);
	अगर (err)
		जाओ out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_block(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, handle->ino);
	अगर (err)
		जाओ out2;

skip_ea_block:
	blk = ext2fs_file_acl_block(fs, (काष्ठा ext2_inode *)inode);
	अगर (!block_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		अगर (err)
			जाओ out;
	}

	/* Write the inode */
	err = ext2fs_write_inode_full(fs, handle->ino, EXT2_INODE(inode),
				      inode_आकार);
	अगर (err)
		जाओ out2;

out2:
	ext2fs_free_mem(&block_buf);
out:
	ext2fs_free_mem(&inode);
	वापस err;
}

अटल errcode_t read_xattrs_from_बफर(काष्ठा ext2_xattr_handle *handle,
					 काष्ठा ext2_inode_large *inode,
					 काष्ठा ext2_ext_attr_entry *entries,
					 अचिन्हित पूर्णांक storage_आकार,
					 अक्षर *मूल्य_start)
{
	काष्ठा ext2_xattr *x;
	काष्ठा ext2_ext_attr_entry *entry, *end;
	स्थिर अक्षर *prefix;
	अचिन्हित पूर्णांक remain, prefix_len;
	errcode_t err;
	अचिन्हित पूर्णांक मूल्यावली_आकार = storage_आकार +
			((अक्षर *)entries - मूल्य_start);

	/* find the end */
	end = entries;
	remain = storage_आकार;
	जबतक (remain >= माप(काष्ठा ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(end)) {

		/* header eats this space */
		remain -= माप(काष्ठा ext2_ext_attr_entry);

		/* is attribute name valid? */
		अगर (EXT2_EXT_ATTR_आकार(end->e_name_len) > remain)
			वापस EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_आकार(end->e_name_len);
		end = EXT2_EXT_ATTR_अग्र(end);
	}

	entry = entries;
	remain = storage_आकार;
	जबतक (remain >= माप(काष्ठा ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(entry)) {

		/* Allocate space for more attrs? */
		अगर (handle->गिनती == handle->capacity) {
			err = ext2fs_xattrs_expand(handle, 4);
			अगर (err)
				वापस err;
		}

		x = handle->attrs + handle->गिनती;

		/* header eats this space */
		remain -= माप(काष्ठा ext2_ext_attr_entry);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_आकार(entry->e_name_len);

		/* Extract name */
		prefix = find_ea_prefix(entry->e_name_सूचकांक);
		prefix_len = (prefix ? म_माप(prefix) : 0);
		err = ext2fs_get_memzero(entry->e_name_len + prefix_len + 1,
					 &x->name);
		अगर (err)
			वापस err;
		अगर (prefix)
			स_नकल(x->name, prefix, prefix_len);
		अगर (entry->e_name_len)
			स_नकल(x->name + prefix_len,
			       (अक्षर *)entry + माप(*entry),
			       entry->e_name_len);
		x->short_name = x->name + prefix_len;
		x->name_सूचकांक = entry->e_name_सूचकांक;

		/* Check & copy value */
		अगर (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    entry->e_मूल्य_inum != 0)
			वापस EXT2_ET_BAD_EA_BLOCK_अंक;

		अगर (entry->e_मूल्य_inum == 0) {
			अगर (entry->e_मूल्य_आकार > remain)
				वापस EXT2_ET_EA_BAD_मूल्य_आकार;

			अगर (entry->e_मूल्य_offs + entry->e_मूल्य_आकार > मूल्यावली_आकार)
				वापस EXT2_ET_EA_BAD_मूल्य_OFFSET;

			अगर (entry->e_मूल्य_आकार > 0 &&
			    मूल्य_start + entry->e_मूल्य_offs <
			    (अक्षर *)end + माप(__u32))
				वापस EXT2_ET_EA_BAD_मूल्य_OFFSET;

			remain -= entry->e_मूल्य_आकार;

			err = ext2fs_get_mem(entry->e_मूल्य_आकार, &x->मूल्य);
			अगर (err)
				वापस err;
			स_नकल(x->मूल्य, मूल्य_start + entry->e_मूल्य_offs,
			       entry->e_मूल्य_आकार);
		} अन्यथा {
			काष्ठा ext2_inode *ea_inode;
			ext2_file_t ea_file;

			अगर (entry->e_मूल्य_offs != 0)
				वापस EXT2_ET_EA_BAD_मूल्य_OFFSET;

			अगर (entry->e_मूल्य_आकार > (64 * 1024))
				वापस EXT2_ET_EA_BAD_मूल्य_आकार;

			err = ext2fs_get_mem(entry->e_मूल्य_आकार, &x->मूल्य);
			अगर (err)
				वापस err;

			err = ext2fs_file_open(handle->fs, entry->e_मूल्य_inum,
					       0, &ea_file);
			अगर (err)
				वापस err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			अगर ((ea_inode->i_flags & EXT4_INLINE_तथ्य_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_गिनती == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			अन्यथा अगर ((__u64) ext2fs_file_get_आकार(ea_file) !=
				 entry->e_मूल्य_आकार)
				err = EXT2_ET_EA_BAD_मूल्य_आकार;
			अन्यथा
				err = ext2fs_file_read(ea_file, x->मूल्य,
						       entry->e_मूल्य_आकार, 0);
			ext2fs_file_close(ea_file);
			अगर (err)
				वापस err;
		}

		x->ea_ino = entry->e_मूल्य_inum;
		x->मूल्य_len = entry->e_मूल्य_आकार;

		/* e_hash may be 0 in older inode's ea */
		अगर (entry->e_hash != 0) {
			__u32 hash, signed_hash;

			व्योम *तथ्य = (entry->e_मूल्य_inum != 0) ?
					0 : मूल्य_start + entry->e_मूल्य_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, entry,
							  तथ्य, &hash,
							  &signed_hash);
			अगर (err)
				वापस err;
			अगर ((entry->e_hash != hash) &&
			    (entry->e_hash != signed_hash)) {
				काष्ठा ext2_inode child;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							entry->e_मूल्य_inum,
							&child);
				अगर (err)
					वापस err;
				अगर (child.i_mtime != handle->ino ||
				    child.i_generation != inode->i_generation)
					वापस EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->गिनती++;
		entry = EXT2_EXT_ATTR_अग्र(entry);
	}

	वापस 0;
}

अटल व्योम xattrs_free_कुंजियाँ(काष्ठा ext2_xattr_handle *h)
{
	काष्ठा ext2_xattr *a = h->attrs;
	पूर्णांक i;

	क्रम (i = 0; i < h->capacity; i++) {
		अगर (a[i].name)
			ext2fs_free_mem(&a[i].name);
		अगर (a[i].मूल्य)
			ext2fs_free_mem(&a[i].मूल्य);
	}
	h->गिनती = 0;
	h->ibody_गिनती = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(काष्ठा ext2_xattr_handle *handle,
				   काष्ठा ext2_inode_large *inode)
{
	काष्ठा ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	अचिन्हित पूर्णांक storage_आकार;
	अक्षर *start, *block_buf = शून्य;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_कुंजियाँ(handle);

	/* Does the inode have space for EA? */
	अगर (inode->i_extra_isize < माप(inode->i_extra_isize) ||
	    EXT2_INODE_आकार(handle->fs->super) <= EXT2_GOOD_OLD_INODE_आकार +
						  inode->i_extra_isize +
						  माप(__u32))
		जाओ read_ea_block;
	अगर (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		जाओ out;
	}

	/* Look for EA in the inode */
	स_नकल(&ea_inode_magic, ((अक्षर *) inode) + EXT2_GOOD_OLD_INODE_आकार +
	       inode->i_extra_isize, माप(__u32));
	अगर (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_आकार = EXT2_INODE_आकार(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_आकार - inode->i_extra_isize -
			माप(__u32);
		start = ((अक्षर *) inode) + EXT2_GOOD_OLD_INODE_आकार +
			inode->i_extra_isize + माप(__u32);

		err = read_xattrs_from_बफर(handle, inode,
					(काष्ठा ext2_ext_attr_entry *) start,
					storage_आकार, start);
		अगर (err)
			जाओ out;

		handle->ibody_गिनती = handle->गिनती;
	}

read_ea_block:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_block(handle->fs, EXT2_INODE(inode));
	अगर (blk != 0) {
		अगर ((blk < handle->fs->super->s_first_तथ्य_block) ||
		    (blk >= ext2fs_blocks_गिनती(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_अंक;
			जाओ out;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &block_buf);
		अगर (err)
			जाओ out;

		err = ext2fs_read_ext_attr3(handle->fs, blk, block_buf,
					    handle->ino);
		अगर (err)
			जाओ out3;

		/* We only know how to deal with v2 EA blocks */
		header = (काष्ठा ext2_ext_attr_header *) block_buf;
		अगर (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			जाओ out3;
		}

		/* Read EAs */
		storage_आकार = handle->fs->blocksize -
			माप(काष्ठा ext2_ext_attr_header);
		start = block_buf + माप(काष्ठा ext2_ext_attr_header);
		err = read_xattrs_from_बफर(handle, inode,
					(काष्ठा ext2_ext_attr_entry *) start,
					storage_आकार, block_buf);
	}

out3:
	अगर (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	वापस err;
}

errcode_t ext2fs_xattrs_read(काष्ठा ext2_xattr_handle *handle)
{
	काष्ठा ext2_inode_large *inode;
	माप_प्रकार inode_आकार = EXT2_INODE_आकार(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	अगर (inode_आकार < माप(*inode))
		inode_आकार = माप(*inode);
	err = ext2fs_get_memzero(inode_आकार, &inode);
	अगर (err)
		वापस err;

	err = ext2fs_read_inode_full(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_आकार(handle->fs->super));
	अगर (err)
		जाओ out;

	err = ext2fs_xattrs_read_inode(handle, inode);

out:
	ext2fs_free_mem(&inode);

	वापस err;
}

errcode_t ext2fs_xattrs_iterate(काष्ठा ext2_xattr_handle *h,
				पूर्णांक (*func)(अक्षर *name, अक्षर *मूल्य,
					    माप_प्रकार मूल्य_len, व्योम *तथ्य),
				व्योम *तथ्य)
{
	काष्ठा ext2_xattr *x;
	पूर्णांक dirty = 0;
	पूर्णांक ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	क्रम (x = h->attrs; x < h->attrs + h->गिनती; x++) {
		ret = func(x->name, x->मूल्य, x->मूल्य_len, तथ्य);
		अगर (ret & XATTR_CHANGED)
			dirty = 1;
		अगर (ret & XATTR_ABORT)
			अवरोध;
	}

	अगर (dirty)
		वापस ext2fs_xattrs_write(h);
	वापस 0;
}

errcode_t ext2fs_xattr_get(काष्ठा ext2_xattr_handle *h, स्थिर अक्षर *कुंजी,
			   व्योम **मूल्य, माप_प्रकार *मूल्य_len)
{
	काष्ठा ext2_xattr *x;
	अक्षर *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	क्रम (x = h->attrs; x < h->attrs + h->गिनती; x++) {
		अगर (म_भेद(x->name, कुंजी))
			जारी;

		अगर (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((म_भेद(कुंजी, "system.posix_acl_default") == 0) ||
		     (म_भेद(कुंजी, "system.posix_acl_access") == 0))) {
			err = convert_disk_बफर_to_posix_acl(x->मूल्य, x->मूल्य_len,
							       मूल्य, मूल्य_len);
			वापस err;
		} अन्यथा {
			err = ext2fs_get_mem(x->मूल्य_len, &val);
			अगर (err)
				वापस err;
			स_नकल(val, x->मूल्य, x->मूल्य_len);
			*मूल्य = val;
			*मूल्य_len = x->मूल्य_len;
			वापस 0;
		}
	}

	वापस EXT2_ET_EA_कुंजी_NOT_FOUND;
}

errcode_t ext2fs_xattr_inode_max_आकार(ext2_filsys fs, ext2_ino_t ino,
				      माप_प्रकार *आकार)
{
	काष्ठा ext2_ext_attr_entry *entry;
	काष्ठा ext2_inode_large *inode;
	__u32 ea_inode_magic;
	अचिन्हित पूर्णांक minoff;
	अक्षर *start;
	माप_प्रकार i;
	errcode_t err;

	i = EXT2_INODE_आकार(fs->super);
	अगर (i < माप(*inode))
		i = माप(*inode);
	err = ext2fs_get_memzero(i, &inode);
	अगर (err)
		वापस err;

	err = ext2fs_read_inode_full(fs, ino, (काष्ठा ext2_inode *)inode,
				     EXT2_INODE_आकार(fs->super));
	अगर (err)
		जाओ out;

	/* Does the inode have size for EA? */
	अगर (EXT2_INODE_आकार(fs->super) <= EXT2_GOOD_OLD_INODE_आकार +
						  inode->i_extra_isize +
						  माप(__u32)) {
		err = EXT2_ET_INLINE_तथ्य_NO_SPACE;
		जाओ out;
	}

	minoff = EXT2_INODE_आकार(fs->super) - माप(*inode) - माप(__u32);
	स_नकल(&ea_inode_magic, ((अक्षर *) inode) + EXT2_GOOD_OLD_INODE_आकार +
	       inode->i_extra_isize, माप(__u32));
	अगर (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		start= ((अक्षर *) inode) + EXT2_GOOD_OLD_INODE_आकार +
			inode->i_extra_isize + माप(__u32);
		entry = (काष्ठा ext2_ext_attr_entry *) start;
		जबतक (!EXT2_EXT_IS_LAST_ENTRY(entry)) {
			अगर (!entry->e_मूल्य_inum && entry->e_मूल्य_आकार) {
				अचिन्हित पूर्णांक offs = entry->e_मूल्य_offs;
				अगर (offs < minoff)
					minoff = offs;
			}
			entry = EXT2_EXT_ATTR_अग्र(entry);
		}
		*आकार = minoff - ((अक्षर *)entry - (अक्षर *)start) - माप(__u32);
	} अन्यथा {
		/* no xattr.  return a maximum size */
		*आकार = EXT2_EXT_ATTR_आकार(minoff -
					   EXT2_EXT_ATTR_LEN(म_माप("data")) -
					   EXT2_EXT_ATTR_ROUND - माप(__u32));
	}

out:
	ext2fs_free_mem(&inode);
	वापस err;
}

अटल errcode_t xattr_create_ea_inode(ext2_filsys fs, स्थिर व्योम *मूल्य,
				       माप_प्रकार मूल्य_len, ext2_ino_t *ea_ino)
{
	काष्ठा ext2_inode inode;
	ext2_ino_t ino;
	ext2_file_t file;
	__u32 hash;
	errcode_t ret;

	ret = ext2fs_new_inode(fs, 0, 0, 0, &ino);
	अगर (ret)
		वापस ret;

	स_रखो(&inode, 0, माप(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	अगर (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_आकार = 0;
	inode.i_mode = LINUX_S_IFREG | 0600;
	inode.i_links_गिनती = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	अगर (ret)
		वापस ret;
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
	अगर (ret)
		वापस ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	अगर (ret)
		वापस ret;
	ret = ext2fs_file_write(file, मूल्य, मूल्य_len, शून्य);
	ext2fs_file_close(file);
	अगर (ret)
		वापस ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	वापस 0;
}

अटल errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	काष्ठा ext2_inode_large inode;
	__u64 ref_गिनती;
	errcode_t ret;

	ret = ext2fs_read_inode_full(fs, ino, (काष्ठा ext2_inode *)&inode,
				     माप(inode));
	अगर (ret)
		जाओ out;

	ref_गिनती = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_गिनती--;
	ext2fs_set_ea_inode_ref(EXT2_INODE(&inode), ref_गिनती);

	अगर (ref_गिनती)
		जाओ write_out;

	inode.i_links_गिनती = 0;
	ext2fs_set_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	अगर (ret)
		जाओ write_out;

	अगर (ext2fs_inode_has_valid_blocks2(fs, (काष्ठा ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (काष्ठा ext2_inode *)&inode, शून्य,
				   0, ~0ULL);
		अगर (ret)
			जाओ out;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_out:
	ret = ext2fs_write_inode_full(fs, ino, (काष्ठा ext2_inode *)&inode,
				      माप(inode));
out:
	वापस ret;
}

अटल errcode_t xattr_update_entry(ext2_filsys fs, काष्ठा ext2_xattr *x,
				    स्थिर अक्षर *name, स्थिर अक्षर *short_name,
				    पूर्णांक सूचकांक, स्थिर व्योम *मूल्य,
				    माप_प्रकार मूल्य_len, पूर्णांक in_inode)
{
	ext2_ino_t ea_ino = 0;
	व्योम *new_मूल्य = शून्य;
	अक्षर *new_name = शून्य;
	पूर्णांक name_len;
	errcode_t ret;

	अगर (!x->name) {
		name_len = म_माप(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		अगर (ret)
			जाओ fail;
		स_नकल(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(मूल्य_len, &new_मूल्य);
	अगर (ret)
		जाओ fail;
	स_नकल(new_मूल्य, मूल्य, मूल्य_len);

	अगर (in_inode) {
		ret = xattr_create_ea_inode(fs, मूल्य, मूल्य_len, &ea_ino);
		अगर (ret)
			जाओ fail;
	}

	अगर (x->ea_ino) {
		ret = xattr_inode_dec_ref(fs, x->ea_ino);
		अगर (ret)
			जाओ fail;
	}

	अगर (!x->name) {
		x->name = new_name;
		x->short_name = new_name + (short_name  - name);
	}
	x->name_सूचकांक = सूचकांक;

	अगर (x->मूल्य)
		ext2fs_free_mem(&x->मूल्य);
	x->मूल्य = new_मूल्य;
	x->मूल्य_len = मूल्य_len;
	x->ea_ino = ea_ino;
	वापस 0;
fail:
	अगर (new_name)
		ext2fs_free_mem(&new_name);
	अगर (new_मूल्य)
		ext2fs_free_mem(&new_मूल्य);
	अगर (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	वापस ret;
}

अटल पूर्णांक xattr_find_position(काष्ठा ext2_xattr *attrs, पूर्णांक गिनती,
			       स्थिर अक्षर *shortname, पूर्णांक name_idx)
{
	काष्ठा ext2_xattr *x;
	पूर्णांक i;
	पूर्णांक shortname_len, x_shortname_len;

	shortname_len = म_माप(shortname);

	क्रम (i = 0, x = attrs; i < गिनती; i++, x++) {
		अगर (name_idx < x->name_सूचकांक)
			अवरोध;
		अगर (name_idx > x->name_सूचकांक)
			जारी;

		x_shortname_len = म_माप(x->short_name);
		अगर (shortname_len < x_shortname_len)
			अवरोध;
		अगर (shortname_len > x_shortname_len)
			जारी;

		अगर (स_भेद(shortname, x->short_name, shortname_len) <= 0)
			अवरोध;
	}
	वापस i;
}

अटल errcode_t xattr_array_update(काष्ठा ext2_xattr_handle *h,
				    स्थिर अक्षर *name,
				    स्थिर व्योम *मूल्य, माप_प्रकार मूल्य_len,
				    पूर्णांक ibody_free, पूर्णांक block_free,
				    पूर्णांक old_idx, पूर्णांक in_inode)
{
	काष्ठा ext2_xattr tmp;
	पूर्णांक add_to_ibody;
	पूर्णांक needed;
	पूर्णांक name_len, name_idx = 0;
	स्थिर अक्षर *shortname = name;
	पूर्णांक new_idx;
	पूर्णांक ret;

	find_ea_सूचकांक(name, &shortname, &name_idx);
	name_len = म_माप(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	अगर (!in_inode)
		needed += EXT2_EXT_ATTR_आकार(मूल्य_len);

	अगर (old_idx >= 0 && old_idx < h->ibody_गिनती) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		अगर (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_आकार(
						h->attrs[old_idx].मूल्य_len);
	}

	अगर (needed <= ibody_free) {
		अगर (old_idx < 0) {
			new_idx = h->ibody_गिनती;
			add_to_ibody = 1;
			जाओ add_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, मूल्य,
					 मूल्य_len, in_inode);
		अगर (ret)
			वापस ret;
		अगर (h->ibody_गिनती <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			स_हटाओ(h->attrs + h->ibody_गिनती + 1,
				h->attrs + h->ibody_गिनती,
				(old_idx - h->ibody_गिनती) * माप(*h->attrs));
			h->attrs[h->ibody_गिनती] = tmp;
			h->ibody_गिनती++;
		}
		वापस 0;
	}

	अगर (h->ibody_गिनती <= old_idx) {
		block_free += EXT2_EXT_ATTR_LEN(name_len);
		अगर (!h->attrs[old_idx].ea_ino)
			block_free +=
				EXT2_EXT_ATTR_आकार(h->attrs[old_idx].मूल्य_len);
	}

	अगर (needed > block_free)
		वापस EXT2_ET_EA_NO_SPACE;

	अगर (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, मूल्य,
					 मूल्य_len, in_inode);
		अगर (ret)
			वापस ret;
		अगर (old_idx < h->ibody_गिनती) {
			/*
			 * Move entry from ibody to the block. Note that
			 * entries in the block are sorted.
			 */
			new_idx = xattr_find_position(h->attrs + h->ibody_गिनती,
						      h->गिनती - h->ibody_गिनती,
						      shortname, name_idx);
			new_idx += h->ibody_गिनती - 1;
			tmp = h->attrs[old_idx];
			स_हटाओ(h->attrs + old_idx, h->attrs + old_idx + 1,
				(new_idx - old_idx) * माप(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_गिनती--;
		}
		वापस 0;
	}

	new_idx = xattr_find_position(h->attrs + h->ibody_गिनती,
				      h->गिनती - h->ibody_गिनती,
				      shortname, name_idx);
	new_idx += h->ibody_गिनती;
	add_to_ibody = 0;

add_new:
	अगर (h->गिनती == h->capacity) {
		ret = ext2fs_xattrs_expand(h, 4);
		अगर (ret)
			वापस ret;
	}

	ret = xattr_update_entry(h->fs, &h->attrs[h->गिनती], name, shortname,
				 name_idx, मूल्य, मूल्य_len, in_inode);
	अगर (ret)
		वापस ret;

	tmp = h->attrs[h->गिनती];
	स_हटाओ(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->गिनती - new_idx)*माप(*h->attrs));
	h->attrs[new_idx] = tmp;
	अगर (add_to_ibody)
		h->ibody_गिनती++;
	h->गिनती++;
	वापस 0;
}

अटल पूर्णांक space_used(काष्ठा ext2_xattr *attrs, पूर्णांक गिनती)
{
	पूर्णांक योग_2 = 0;
	काष्ठा ext2_xattr *x;
	पूर्णांक i, len;

	क्रम (i = 0, x = attrs; i < गिनती; i++, x++) {
		len = म_माप(x->short_name);
		योग_2 += EXT2_EXT_ATTR_LEN(len);
		अगर (!x->ea_ino)
			योग_2 += EXT2_EXT_ATTR_आकार(x->मूल्य_len);
	}
	वापस योग_2;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#घोषणा EXT4_XATTR_MIN_LARGE_EA_आकार(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - माप(काष्ठा ext2_ext_attr_header) - 4)

errcode_t ext2fs_xattr_set(काष्ठा ext2_xattr_handle *h,
			   स्थिर अक्षर *name,
			   स्थिर व्योम *मूल्य,
			   माप_प्रकार मूल्य_len)
{
	ext2_filsys fs = h->fs;
	स्थिर पूर्णांक inode_आकार = EXT2_INODE_आकार(fs->super);
	काष्ठा ext2_inode_large *inode = शून्य;
	काष्ठा ext2_xattr *x;
	अक्षर *new_मूल्य;
	पूर्णांक ibody_free, block_free;
	पूर्णांक in_inode = 0;
	पूर्णांक old_idx = -1;
	पूर्णांक extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(मूल्य_len, &new_मूल्य);
	अगर (ret)
		वापस ret;
	अगर (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((म_भेद(name, "system.posix_acl_default") == 0) ||
	     (म_भेद(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_to_disk_बफर(मूल्य, मूल्य_len,
						       new_मूल्य, &मूल्य_len);
		अगर (ret)
			जाओ out;
	} अन्यथा अगर (मूल्य_len)
		स_नकल(new_मूल्य, मूल्य, मूल्य_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	क्रम (x = h->attrs; x < h->attrs + h->गिनती; x++) {
		अगर (!म_भेद(x->name, name)) {
			अगर (!x->ea_ino && x->मूल्य_len == मूल्य_len &&
			    (!मूल्य_len ||
			     !स_भेद(x->मूल्य, new_मूल्य, मूल्य_len))) {
				ret = 0;
				जाओ out;
			}
			old_idx = x - h->attrs;
			अवरोध;
		}
	}

	ret = ext2fs_get_memzero(inode_आकार, &inode);
	अगर (ret)
		जाओ out;
	ret = ext2fs_read_inode_full(fs, h->ino,
				     (काष्ठा ext2_inode *)inode,
				     inode_आकार);
	अगर (ret)
		जाओ out;
	अगर (inode_आकार > EXT2_GOOD_OLD_INODE_आकार) {
		extra_isize = inode->i_extra_isize;
		अगर (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			अगर (extra_isize == 0)
				extra_isize = माप(__u32);
		}
		ibody_free = inode_आकार - EXT2_GOOD_OLD_INODE_आकार;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= माप(__u32) * 2;
		ibody_free -= space_used(h->attrs, h->ibody_गिनती);
	} अन्यथा
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	अगर (म_भेद(name, "system.data") == 0) {
		अगर (h->ibody_गिनती <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			जाओ out;
		}
		ret = xattr_array_update(h, name, new_मूल्य, मूल्य_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		अगर (ret)
			जाओ out;
		जाओ write_out;
	}

	block_free = fs->blocksize;
	block_free -= माप(काष्ठा ext2_ext_attr_header);
	/* Final null entry. */
	block_free -= माप(__u32);
	block_free -= space_used(h->attrs + h->ibody_गिनती,
				 h->गिनती - h->ibody_गिनती);

	अगर (ext2fs_has_feature_ea_inode(fs->super) &&
	    मूल्य_len > EXT4_XATTR_MIN_LARGE_EA_आकार(fs->blocksize))
		in_inode = 1;

	ret = xattr_array_update(h, name, new_मूल्य, मूल्य_len, ibody_free,
				 block_free, old_idx, in_inode);
	अगर (ret == EXT2_ET_EA_NO_SPACE && !in_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_मूल्य, मूल्य_len,
			ibody_free, block_free, old_idx, 1 /* in_inode */);
	अगर (ret)
		जाओ out;

write_out:
	ret = ext2fs_xattrs_write(h);
out:
	अगर (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_मूल्य);
	वापस ret;
}

errcode_t ext2fs_xattr_remove(काष्ठा ext2_xattr_handle *handle,
			      स्थिर अक्षर *कुंजी)
{
	काष्ठा ext2_xattr *x;
	काष्ठा ext2_xattr *end = handle->attrs + handle->गिनती;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	क्रम (x = handle->attrs; x < end; x++) {
		अगर (म_भेद(x->name, कुंजी) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->मूल्य);
			अगर (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			स_हटाओ(x, x + 1, (end - x - 1)*माप(*x));
			स_रखो(end - 1, 0, माप(*end));
			अगर (x < handle->attrs + handle->ibody_गिनती)
				handle->ibody_गिनती--;
			handle->गिनती--;
			वापस ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	वापस 0;
}

errcode_t ext2fs_xattr_remove_all(काष्ठा ext2_xattr_handle *handle)
{
	काष्ठा ext2_xattr *x;
	काष्ठा ext2_xattr *end = handle->attrs + handle->गिनती;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	क्रम (x = handle->attrs; x < end; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->मूल्य);
		अगर (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_गिनती = 0;
	handle->गिनती = 0;
	वापस ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     काष्ठा ext2_xattr_handle **handle)
{
	काष्ठा ext2_xattr_handle *h;
	errcode_t err;

	अगर (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_तथ्य(fs->super))
		वापस EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(माप(*h), &h);
	अगर (err)
		वापस err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->capacity = 4;
	err = ext2fs_get_arrayzero(h->capacity, माप(काष्ठा ext2_xattr),
				   &h->attrs);
	अगर (err) {
		ext2fs_free_mem(&h);
		वापस err;
	}
	h->गिनती = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	वापस 0;
}

errcode_t ext2fs_xattrs_close(काष्ठा ext2_xattr_handle **handle)
{
	काष्ठा ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_कुंजियाँ(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	वापस 0;
}

errcode_t ext2fs_xattrs_गिनती(काष्ठा ext2_xattr_handle *handle, माप_प्रकार *गिनती)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*गिनती = handle->गिनती;
	वापस 0;
}

errcode_t ext2fs_xattrs_flags(काष्ठा ext2_xattr_handle *handle,
			      अचिन्हित पूर्णांक *new_flags, अचिन्हित पूर्णांक *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	अगर (old_flags)
		*old_flags = handle->flags;
	अगर (new_flags)
		handle->flags = *new_flags;
	वापस 0;
}
