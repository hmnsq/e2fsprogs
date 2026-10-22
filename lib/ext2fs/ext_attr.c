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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <time.h>

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2_ext_attr.h"
#અન્તર્ભાવ "ext4_acl.h"

#અન્તર્ભાવ "ext2fsP.h"

સ્થિર errcode_t read_ea_inode_hash(ext2_filsys fs, ext2_ino_t ino, __u32 *hash)
{
	સંરચના ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	જો (retval)
		પાછા retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	પાછા 0;
}

#define NAME_HASH_SHIFT 5
#define કિંમત_HASH_SHIFT 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_entry(સંરચના ext2_ext_attr_entry *entry, શૂન્ય *data)
{
	__u32 hash = 0;
	unsigned અક્ષર *name = (((unsigned અક્ષર *) entry) +
			       કદ(સંરચના ext2_ext_attr_entry));
	પૂર્ણાંક n;

	માટે (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*કદ(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	જો (entry->e_કિંમત_inum == 0 && entry->e_કિંમત_size != 0) {
		__u32 *કિંમત = (__u32 *)data;
		માટે (n = (entry->e_કિંમત_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << કિંમત_HASH_SHIFT) ^
			       (hash >> (8*કદ(hash) - કિંમત_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*કિંમત++);
		}
	}

	પાછા hash;
}

__u32 ext2fs_ext_attr_hash_entry_signed(સંરચના ext2_ext_attr_entry *entry,
					શૂન્ય *data)
{
	__u32 hash = 0;
	signed અક્ષર *name = (((signed અક્ષર *) entry) +
			     કદ(સંરચના ext2_ext_attr_entry));
	પૂર્ણાંક n;

	માટે (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*કદ(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	જો (entry->e_કિંમત_inum == 0 && entry->e_કિંમત_size != 0) {
		__u32 *કિંમત = (__u32 *)data;
		માટે (n = (entry->e_કિંમત_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << કિંમત_HASH_SHIFT) ^
			       (hash >> (8*કદ(hash) - કિંમત_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*કિંમત++);
		}
	}

	પાછા hash;
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
				      સંરચના ext2_ext_attr_entry *entry,
				      શૂન્ય *data, __u32 *hash,
				      __u32 *signed_hash)
{
	*hash = ext2fs_ext_attr_hash_entry(entry, data);
	જો (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_entry_signed(entry, data);

	જો (entry->e_કિંમત_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, entry->e_કિંમત_inum,
					    &ea_inode_hash);
		જો (retval)
			પાછા retval;

		*hash = (*hash << કિંમત_HASH_SHIFT) ^
			(*hash >> (8*કદ(*hash) - કિંમત_HASH_SHIFT)) ^
			ea_inode_hash;
		જો (signed_hash)
			*signed_hash = (*signed_hash << કિંમત_HASH_SHIFT) ^
				(*signed_hash >> (8*કદ(*hash) -
						  કિંમત_HASH_SHIFT)) ^
				ea_inode_hash;
	}
	પાછા 0;
}

/*
 * ext2fs_ext_attr_hash_entry2()
 *
 * Compute the hash of an extended attribute.
 * This version of the function supports hashing entries that reference
 * external inodes (ea_inode feature).
 */
errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
				      સંરચના ext2_ext_attr_entry *entry,
				      શૂન્ય *data, __u32 *hash)
{
	પાછા ext2fs_ext_attr_hash_entry3(fs, entry, data, hash, ખાલી);
}

#undef NAME_HASH_SHIFT
#undef કિંમત_HASH_SHIFT

#define BLOCK_HASH_SHIFT 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
શૂન્ય ext2fs_ext_attr_block_rehash(સંરચના ext2_ext_attr_header *header,
				  સંરચના ext2_ext_attr_entry *end)
{
	સંરચના ext2_ext_attr_entry *here;
	__u32 hash = 0;

	here = (સંરચના ext2_ext_attr_entry *)(header+1);
	જ્યારે (here < end && !EXT2_EXT_IS_LAST_ENTRY(here)) {
		જો (!here->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			તોડો;
		}
		hash = (hash << BLOCK_HASH_SHIFT) ^
		       (hash >> (8*કદ(hash) - BLOCK_HASH_SHIFT)) ^
		       here->e_hash;
		here = EXT2_EXT_ATTR_NEXT(here);
	}
	header->h_hash = hash;
}

#undef BLOCK_HASH_SHIFT

__u32 ext2fs_get_ea_inode_hash(સંરચના ext2_inode *inode)
{
	પાછા inode->i_atime;
}

શૂન્ય ext2fs_set_ea_inode_hash(સંરચના ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(સંરચના ext2_inode *inode)
{
	પાછા ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_version;
}

શૂન્ય ext2fs_set_ea_inode_ref(સંરચના ext2_inode *inode, __u64 ref_ગણતરી)
{
	inode->i_ctime = (__u32)(ref_ગણતરી >> 32);
	inode->osd1.linux1.l_i_version = (__u32)ref_ગણતરી;
}

સ્થિર errcode_t check_ext_attr_header(સંરચના ext2_ext_attr_header *header)
{
	જો ((header->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     header->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    header->h_blocks != 1)
		પાછા EXT2_ET_BAD_EA_HEADER;

	પાછા 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block, શૂન્ય *buf,
				ext2_ino_t inum)
{
	પૂર્ણાંક		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	જો (retval)
		પાછા retval;

	જો (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_block_csum_verify(fs, inum, block, buf))
		csum_failed = 1;

#ifdef શબ્દો_BIGENDIAN
	ext2fs_swap_ext_attr(buf, buf, fs->blocksize, 1);
#endif

	retval = check_ext_attr_header(buf);
	જો (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	પાછા retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block, શૂન્ય *buf)
{
	પાછા ext2fs_read_ext_attr3(fs, block, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, શૂન્ય *buf)
{
	પાછા ext2fs_read_ext_attr2(fs, block, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block, શૂન્ય *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	અક્ષર		*write_buf;

#ifdef શબ્દો_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	જો (retval)
		પાછા retval;
	ext2fs_swap_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#નહીંતર
	write_buf = (અક્ષર *) inbuf;
#endif

	retval = ext2fs_ext_attr_block_csum_set(fs, inum, block,
			(સંરચના ext2_ext_attr_header *)write_buf);
	જો (retval)
		પાછા retval;

	retval = io_channel_write_blk64(fs->io, block, 1, write_buf);
#ifdef શબ્દો_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#endif
	જો (!retval)
		ext2fs_mark_changed(fs);
	પાછા retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block, શૂન્ય *inbuf)
{
	પાછા ext2fs_write_ext_attr3(fs, block, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block, શૂન્ય *inbuf)
{
	પાછા ext2fs_write_ext_attr2(fs, block, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    અક્ષર *block_buf, પૂર્ણાંક adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	સંરચના ext2_ext_attr_header *header;
	અક્ષર	*buf = 0;

	જો ((blk >= ext2fs_blocks_ગણતરી(fs->super)) ||
	    (blk < fs->super->s_first_data_block))
		પાછા EXT2_ET_BAD_EA_BLOCK_અંક;

	જો (!block_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		જો (retval)
			પાછા retval;
		block_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, block_buf, inum);
	જો (retval)
		જાઓ errout;

	header = (સંરચના ext2_ext_attr_header *) block_buf;
	header->h_refcount += adjust;
	જો (newcount)
		*newcount = header->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, block_buf, inum);
	જો (retval)
		જાઓ errout;

errout:
	જો (buf)
		ext2fs_free_mem(&buf);
	પાછા retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    અક્ષર *block_buf, પૂર્ણાંક adjust,
				    __u32 *newcount)
{
	પાછા ext2fs_adjust_ea_refcount3(fs, blk, block_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					અક્ષર *block_buf, પૂર્ણાંક adjust,
					__u32 *newcount)
{
	પાછા ext2fs_adjust_ea_refcount2(fs, blk, block_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
સંરચના ext2_xattr {
	પૂર્ણાંક name_index;
	અક્ષર *name;
	અક્ષર *short_name;
	શૂન્ય *કિંમત;
	unsigned પૂર્ણાંક કિંમત_len;
	ext2_ino_t ea_ino;
};

સંરચના ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	સંરચના ext2_xattr *attrs;
	પૂર્ણાંક capacity;
	પૂર્ણાંક ગણતરી;
	પૂર્ણાંક ibody_ગણતરી;
	ext2_ino_t ino;
	unsigned પૂર્ણાંક flags;
};

સ્થિર errcode_t ext2fs_xattrs_expand(સંરચના ext2_xattr_handle *h,
				      unsigned પૂર્ણાંક expandby)
{
	સંરચના ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->capacity + expandby,
				   કદ(સંરચના ext2_xattr), &new_attrs);
	જો (err)
		પાછા err;

	memcpy(new_attrs, h->attrs, h->capacity * કદ(સંરચના ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->capacity += expandby;
	h->attrs = new_attrs;

	પાછા 0;
}

સંરચના ea_name_index {
	પૂર્ણાંક index;
	અચલ અક્ષર *name;
};

/* Keep these names sorted in order of decreasing specificity. */
સ્થિર સંરચના ea_name_index ea_names[] = {
	{10, "gnu."},
	{3, "system.posix_acl_default"},
	{2, "system.posix_acl_access"},
	{8, "system.richacl"},
	{6, "security."},
	{4, "trusted."},
	{7, "system."},
	{1, "user."},
	{0, ખાલી},
};

સ્થિર અચલ અક્ષર *find_ea_prefix(પૂર્ણાંક index)
{
	સંરચના ea_name_index *e;

	માટે (e = ea_names; e->name; e++)
		જો (e->index == index)
			પાછા e->name;

	પાછા ખાલી;
}

સ્થિર પૂર્ણાંક find_ea_index(અચલ અક્ષર *fullname, અચલ અક્ષર **name, પૂર્ણાંક *index)
{
	સંરચના ea_name_index *e;

	માટે (e = ea_names; e->name; e++) {
		જો (strncmp(fullname, e->name, strlen(e->name)) == 0) {
			*name = fullname + strlen(e->name);
			*index = e->index;
			પાછા 1;
		}
	}
	પાછા 0;
}

errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       સંરચના ext2_inode_large *inode)
{
	સંરચના ext2_ext_attr_header *header;
	શૂન્ય *block_buf = ખાલી;
	blk64_t blk;
	errcode_t err;
	સંરચના ext2_inode_large i;

	/* Read inode? */
	જો (inode == ખાલી) {
		err = ext2fs_read_inode_full(fs, ino, (સંરચના ext2_inode *)&i,
					     કદ(સંરચના ext2_inode_large));
		જો (err)
			પાછા err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (સંરચના ext2_inode *)inode);
	જો (blk == 0)
		પાછા 0;

	/* Find block, zero it, write back */
	જો ((blk < fs->super->s_first_data_block) ||
	    (blk >= ext2fs_blocks_ગણતરી(fs->super))) {
		err = EXT2_ET_BAD_EA_BLOCK_અંક;
		જાઓ out;
	}

	err = ext2fs_get_mem(fs->blocksize, &block_buf);
	જો (err)
		જાઓ out;

	err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
	જો (err)
		જાઓ out2;

	/* We only know how to deal with v2 EA blocks */
	header = (સંરચના ext2_ext_attr_header *) block_buf;
	જો (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_HEADER;
		જાઓ out2;
	}

	header->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
	જો (err)
		જાઓ out2;

	/* Erase link to block */
	ext2fs_file_acl_block_set(fs, (સંરચના ext2_inode *)inode, 0);
	જો (header->h_refcount == 0)
		ext2fs_block_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_blocks(fs, (સંરચના ext2_inode *)inode, 1);
	જો (err)
		જાઓ out2;

	/* Write inode? */
	જો (inode == &i) {
		err = ext2fs_write_inode_full(fs, ino, (સંરચના ext2_inode *)&i,
					      કદ(સંરચના ext2_inode_large));
		જો (err)
			જાઓ out2;
	}

out2:
	ext2fs_free_mem(&block_buf);
out:
	પાછા err;
}

સ્થિર errcode_t prep_ea_block_for_write(ext2_filsys fs, ext2_ino_t ino,
					 સંરચના ext2_inode_large *inode)
{
	સંરચના ext2_ext_attr_header *header;
	શૂન્ય *block_buf = ખાલી;
	blk64_t blk, goal;
	errcode_t err;

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (સંરચના ext2_inode *)inode);
	જો (blk != 0) {
		જો ((blk < fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_ગણતરી(fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_અંક;
			જાઓ out;
		}

		err = ext2fs_get_mem(fs->blocksize, &block_buf);
		જો (err)
			જાઓ out;

		err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
		જો (err)
			જાઓ out2;

		/* We only know how to deal with v2 EA blocks */
		header = (સંરચના ext2_ext_attr_header *) block_buf;
		જો (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			જાઓ out2;
		}

		/* Single-user block.  We're done here. */
		જો (header->h_refcount == 1)
			જાઓ out2;

		/* We need to CoW the block. */
		header->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
		જો (err)
			જાઓ out2;
	} નહીંતર {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_add_blocks(fs, (સંરચના ext2_inode *)inode,
					     1);
		જો (err)
			જાઓ out;
	}

	/* Allocate a block */
	goal = ext2fs_find_inode_goal(fs, ino, (સંરચના ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, ખાલી, &blk);
	જો (err)
		જાઓ out2;
	ext2fs_file_acl_block_set(fs, (સંરચના ext2_inode *)inode, blk);
out2:
	જો (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	પાછા err;
}


સ્થિર અંતઃરેખ પૂર્ણાંક
posix_acl_xattr_ગણતરી(size_t size)
{
        જો (size < કદ(posix_acl_xattr_header))
                પાછા -1;
        size -= કદ(posix_acl_xattr_header);
        જો (size % કદ(posix_acl_xattr_entry))
                પાછા -1;
        પાછા size / કદ(posix_acl_xattr_entry);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
સ્થિર errcode_t convert_posix_acl_to_disk_buffer(અચલ શૂન્ય *કિંમત, size_t size,
						  શૂન્ય *out_buf, size_t *size_out)
{
	અચલ posix_acl_xattr_header *header =
		(અચલ posix_acl_xattr_header*) કિંમત;
	અચલ posix_acl_xattr_entry *end, *entry =
		(અચલ posix_acl_xattr_entry *)(header+1);
	ext4_acl_header *ext_acl;
	size_t s;
	અક્ષર *e;

	પૂર્ણાંક ગણતરી;

	જો (!કિંમત)
		પાછા EINVAL;
	જો (size < કદ(posix_acl_xattr_header))
		પાછા ENOMEM;
	જો (header->a_version != ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION))
		પાછા EINVAL;

	ગણતરી = posix_acl_xattr_ગણતરી(size);
	ext_acl = out_buf;
	ext_acl->a_version = ext2fs_cpu_to_le32(EXT4_ACL_VERSION);

	જો (ગણતરી <= 0)
		પાછા EINVAL;

	e = (અક્ષર *) out_buf + કદ(ext4_acl_header);
	s = કદ(ext4_acl_header);
	માટે (end = entry + ગણતરી; entry != end;entry++) {
		ext4_acl_entry *disk_entry = (ext4_acl_entry*) e;
		disk_entry->e_tag = entry->e_tag;
		disk_entry->e_perm = entry->e_perm;

		પસંદગી(ext2fs_le16_to_cpu(entry->e_tag)) {
			સ્થિતિ ACL_USER_OBJ:
			સ્થિતિ ACL_GROUP_OBJ:
			સ્થિતિ ACL_MASK:
			સ્થિતિ ACL_OTHER:
				e += કદ(ext4_acl_entry_short);
				s += કદ(ext4_acl_entry_short);
				તોડો;
			સ્થિતિ ACL_USER:
			સ્થિતિ ACL_GROUP:
				disk_entry->e_id = entry->e_id;
				e += કદ(ext4_acl_entry);
				s += કદ(ext4_acl_entry);
				તોડો;
			મૂળભૂત:
				પાછા EINVAL;
		}
	}
	*size_out = s;
	પાછા 0;
}

સ્થિર errcode_t convert_disk_buffer_to_posix_acl(અચલ શૂન્ય *કિંમત, size_t size,
						  શૂન્ય **out_buf, size_t *size_out)
{
	posix_acl_xattr_header *header;
	posix_acl_xattr_entry *entry;
	અચલ ext4_acl_header *ext_acl = (અચલ ext4_acl_header *) કિંમત;
	errcode_t err;
	અચલ અક્ષર *cp;
	અક્ષર *out;

	જો ((!કિંમત) ||
	    (size < કદ(ext4_acl_header)) ||
	    (ext_acl->a_version != ext2fs_cpu_to_le32(EXT4_ACL_VERSION)))
		પાછા EINVAL;

	err = ext2fs_get_mem(size * 2, &out);
	જો (err)
		પાછા err;

	header = (posix_acl_xattr_header *) out;
	header->a_version = ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION);
	entry = (posix_acl_xattr_entry *) (out + કદ(posix_acl_xattr_header));

	cp = (અચલ અક્ષર *) કિંમત + કદ(ext4_acl_header);
	size -= કદ(ext4_acl_header);

	જ્યારે (size > 0) {
		અચલ ext4_acl_entry *disk_entry = (અચલ ext4_acl_entry *) cp;

		entry->e_tag = disk_entry->e_tag;
		entry->e_perm = disk_entry->e_perm;

		પસંદગી(ext2fs_le16_to_cpu(entry->e_tag)) {
			સ્થિતિ ACL_USER_OBJ:
			સ્થિતિ ACL_GROUP_OBJ:
			સ્થિતિ ACL_MASK:
			સ્થિતિ ACL_OTHER:
				entry->e_id = 0;
				cp += કદ(ext4_acl_entry_short);
				size -= કદ(ext4_acl_entry_short);
				તોડો;
			સ્થિતિ ACL_USER:
			સ્થિતિ ACL_GROUP:
				entry->e_id = disk_entry->e_id;
				cp += કદ(ext4_acl_entry);
				size -= કદ(ext4_acl_entry);
				તોડો;
			મૂળભૂત:
				ext2fs_free_mem(&out);
				પાછા EINVAL;
		}
		entry++;
	}
	*out_buf = out;
	*size_out = ((અક્ષર *) entry - out);
	પાછા 0;
}

સ્થિર errcode_t
write_xattrs_to_buffer(ext2_filsys fs, સંરચના ext2_xattr *attrs, પૂર્ણાંક ગણતરી,
		       શૂન્ય *entries_start, unsigned પૂર્ણાંક storage_size,
		       unsigned પૂર્ણાંક કિંમત_offset_correction, પૂર્ણાંક write_hash)
{
	સંરચના ext2_xattr *x;
	સંરચના ext2_ext_attr_entry *e = entries_start;
	અક્ષર *end = (અક્ષર *) entries_start + storage_size;
	unsigned પૂર્ણાંક કિંમત_size;
	errcode_t err;

	memset(entries_start, 0, storage_size);
	માટે (x = attrs; x < attrs + ગણતરી; x++) {
		કિંમત_size = ((x->કિંમત_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = strlen(x->short_name);
		e->e_name_index = x->name_index;

		e->e_કિંમત_size = x->કિંમત_len;
		e->e_કિંમત_inum = x->ea_ino;

		/* Store name */
		memcpy((અક્ષર *)e + કદ(*e), x->short_name, e->e_name_len);
		જો (x->ea_ino) {
			e->e_કિંમત_offs = 0;
		} નહીંતર {
			end -= કિંમત_size;
			e->e_કિંમત_offs = end - (અક્ષર *) entries_start +
						કિંમત_offset_correction;
			memcpy(end, x->કિંમત, e->e_કિંમત_size);
		}

		જો (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : end,
							  &e->e_hash);
			જો (err)
				પાછા err;
		} નહીંતર
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_NEXT(e);
		*(__u32 *)e = 0;
	}
	પાછા 0;
}

errcode_t ext2fs_xattrs_write(સંરચના ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	અચલ unsigned પૂર્ણાંક inode_size = EXT2_INODE_SIZE(fs->super);
	સંરચના ext2_inode_large *inode;
	અક્ષર *start, *block_buf = ખાલી;
	સંરચના ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	blk64_t blk;
	unsigned પૂર્ણાંક storage_size;
	unsigned પૂર્ણાંક i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_size;
	જો (i < કદ(*inode))
		i = કદ(*inode);
	err = ext2fs_get_memzero(i, &inode);
	જો (err)
		પાછા err;

	err = ext2fs_read_inode_full(fs, handle->ino, EXT2_INODE(inode),
				     inode_size);
	જો (err)
		જાઓ out;

	/* If extra_isize isn't set, we need to set it now */
	જો (inode->i_extra_isize == 0 &&
	    inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		અક્ષર *p = (અક્ષર *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		જો (extra == 0)
			extra = કદ(__u32);
		memset(p + EXT2_GOOD_OLD_INODE_SIZE, 0, extra);
		inode->i_extra_isize = extra;
	}
	જો (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		જાઓ out;
	}

	/* Does the inode have space for EA? */
	જો (inode->i_extra_isize < કદ(inode->i_extra_isize) ||
	    inode_size <= EXT2_GOOD_OLD_INODE_SIZE + inode->i_extra_isize +
								કદ(__u32))
		જાઓ write_ea_block;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	memcpy(((અક્ષર *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, &ea_inode_magic, કદ(__u32));
	storage_size = inode_size - EXT2_GOOD_OLD_INODE_SIZE -
				inode->i_extra_isize - કદ(__u32);
	start = ((અક્ષર *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
				inode->i_extra_isize + કદ(__u32);

	err = write_xattrs_to_buffer(fs, handle->attrs, handle->ibody_ગણતરી,
				     start, storage_size, 0, 0);
	જો (err)
		જાઓ out;
write_ea_block:
	/* Are we done? */
	જો (handle->ibody_ગણતરી == handle->ગણતરી &&
	    !ext2fs_file_acl_block(fs, EXT2_INODE(inode)))
		જાઓ skip_ea_block;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &block_buf);
	જો (err)
		જાઓ out;

	storage_size = fs->blocksize - કદ(સંરચના ext2_ext_attr_header);
	start = block_buf + કદ(સંરચના ext2_ext_attr_header);

	err = write_xattrs_to_buffer(fs, handle->attrs + handle->ibody_ગણતરી,
				     handle->ગણતરી - handle->ibody_ગણતરી, start,
				     storage_size, start - block_buf, 1);
	જો (err)
		જાઓ out2;

	/* Write a header on the EA block */
	header = (સંરચના ext2_ext_attr_header *) block_buf;
	header->h_magic = EXT2_EXT_ATTR_MAGIC;
	header->h_refcount = 1;
	header->h_blocks = 1;

	/* Get a new block for writing */
	err = prep_ea_block_for_write(fs, handle->ino, inode);
	જો (err)
		જાઓ out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_block(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, handle->ino);
	જો (err)
		જાઓ out2;

skip_ea_block:
	blk = ext2fs_file_acl_block(fs, (સંરચના ext2_inode *)inode);
	જો (!block_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		જો (err)
			જાઓ out;
	}

	/* Write the inode */
	err = ext2fs_write_inode_full(fs, handle->ino, EXT2_INODE(inode),
				      inode_size);
	જો (err)
		જાઓ out2;

out2:
	ext2fs_free_mem(&block_buf);
out:
	ext2fs_free_mem(&inode);
	પાછા err;
}

સ્થિર errcode_t read_xattrs_from_buffer(સંરચના ext2_xattr_handle *handle,
					 સંરચના ext2_inode_large *inode,
					 સંરચના ext2_ext_attr_entry *entries,
					 unsigned પૂર્ણાંક storage_size,
					 અક્ષર *કિંમત_start)
{
	સંરચના ext2_xattr *x;
	સંરચના ext2_ext_attr_entry *entry, *end;
	અચલ અક્ષર *prefix;
	unsigned પૂર્ણાંક remain, prefix_len;
	errcode_t err;
	unsigned પૂર્ણાંક કિંમતો_size = storage_size +
			((અક્ષર *)entries - કિંમત_start);

	/* find the end */
	end = entries;
	remain = storage_size;
	જ્યારે (remain >= કદ(સંરચના ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(end)) {

		/* header eats this space */
		remain -= કદ(સંરચના ext2_ext_attr_entry);

		/* is attribute name valid? */
		જો (EXT2_EXT_ATTR_SIZE(end->e_name_len) > remain)
			પાછા EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(end->e_name_len);
		end = EXT2_EXT_ATTR_NEXT(end);
	}

	entry = entries;
	remain = storage_size;
	જ્યારે (remain >= કદ(સંરચના ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(entry)) {

		/* Allocate space for more attrs? */
		જો (handle->ગણતરી == handle->capacity) {
			err = ext2fs_xattrs_expand(handle, 4);
			જો (err)
				પાછા err;
		}

		x = handle->attrs + handle->ગણતરી;

		/* header eats this space */
		remain -= કદ(સંરચના ext2_ext_attr_entry);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(entry->e_name_len);

		/* Extract name */
		prefix = find_ea_prefix(entry->e_name_index);
		prefix_len = (prefix ? strlen(prefix) : 0);
		err = ext2fs_get_memzero(entry->e_name_len + prefix_len + 1,
					 &x->name);
		જો (err)
			પાછા err;
		જો (prefix)
			memcpy(x->name, prefix, prefix_len);
		જો (entry->e_name_len)
			memcpy(x->name + prefix_len,
			       (અક્ષર *)entry + કદ(*entry),
			       entry->e_name_len);
		x->short_name = x->name + prefix_len;
		x->name_index = entry->e_name_index;

		/* Check & copy value */
		જો (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    entry->e_કિંમત_inum != 0)
			પાછા EXT2_ET_BAD_EA_BLOCK_અંક;

		જો (entry->e_કિંમત_inum == 0) {
			જો (entry->e_કિંમત_size > remain)
				પાછા EXT2_ET_EA_BAD_કિંમત_SIZE;

			જો (entry->e_કિંમત_offs + entry->e_કિંમત_size > કિંમતો_size)
				પાછા EXT2_ET_EA_BAD_કિંમત_OFFSET;

			જો (entry->e_કિંમત_size > 0 &&
			    કિંમત_start + entry->e_કિંમત_offs <
			    (અક્ષર *)end + કદ(__u32))
				પાછા EXT2_ET_EA_BAD_કિંમત_OFFSET;

			remain -= entry->e_કિંમત_size;

			err = ext2fs_get_mem(entry->e_કિંમત_size, &x->કિંમત);
			જો (err)
				પાછા err;
			memcpy(x->કિંમત, કિંમત_start + entry->e_કિંમત_offs,
			       entry->e_કિંમત_size);
		} નહીંતર {
			સંરચના ext2_inode *ea_inode;
			ext2_file_t ea_file;

			જો (entry->e_કિંમત_offs != 0)
				પાછા EXT2_ET_EA_BAD_કિંમત_OFFSET;

			જો (entry->e_કિંમત_size > (64 * 1024))
				પાછા EXT2_ET_EA_BAD_કિંમત_SIZE;

			err = ext2fs_get_mem(entry->e_કિંમત_size, &x->કિંમત);
			જો (err)
				પાછા err;

			err = ext2fs_file_open(handle->fs, entry->e_કિંમત_inum,
					       0, &ea_file);
			જો (err)
				પાછા err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			જો ((ea_inode->i_flags & EXT4_INLINE_DATA_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_ગણતરી == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			નહીંતર જો ((__u64) ext2fs_file_get_size(ea_file) !=
				 entry->e_કિંમત_size)
				err = EXT2_ET_EA_BAD_કિંમત_SIZE;
			નહીંતર
				err = ext2fs_file_read(ea_file, x->કિંમત,
						       entry->e_કિંમત_size, 0);
			ext2fs_file_close(ea_file);
			જો (err)
				પાછા err;
		}

		x->ea_ino = entry->e_કિંમત_inum;
		x->કિંમત_len = entry->e_કિંમત_size;

		/* e_hash may be 0 in older inode's ea */
		જો (entry->e_hash != 0) {
			__u32 hash, signed_hash;

			શૂન્ય *data = (entry->e_કિંમત_inum != 0) ?
					0 : કિંમત_start + entry->e_કિંમત_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, entry,
							  data, &hash,
							  &signed_hash);
			જો (err)
				પાછા err;
			જો ((entry->e_hash != hash) &&
			    (entry->e_hash != signed_hash)) {
				સંરચના ext2_inode child;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							entry->e_કિંમત_inum,
							&child);
				જો (err)
					પાછા err;
				જો (child.i_mtime != handle->ino ||
				    child.i_generation != inode->i_generation)
					પાછા EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->ગણતરી++;
		entry = EXT2_EXT_ATTR_NEXT(entry);
	}

	પાછા 0;
}

સ્થિર શૂન્ય xattrs_free_ચાવીઓ(સંરચના ext2_xattr_handle *h)
{
	સંરચના ext2_xattr *a = h->attrs;
	પૂર્ણાંક i;

	માટે (i = 0; i < h->capacity; i++) {
		જો (a[i].name)
			ext2fs_free_mem(&a[i].name);
		જો (a[i].કિંમત)
			ext2fs_free_mem(&a[i].કિંમત);
	}
	h->ગણતરી = 0;
	h->ibody_ગણતરી = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(સંરચના ext2_xattr_handle *handle,
				   સંરચના ext2_inode_large *inode)
{
	સંરચના ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	unsigned પૂર્ણાંક storage_size;
	અક્ષર *start, *block_buf = ખાલી;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_ચાવીઓ(handle);

	/* Does the inode have space for EA? */
	જો (inode->i_extra_isize < કદ(inode->i_extra_isize) ||
	    EXT2_INODE_SIZE(handle->fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  કદ(__u32))
		જાઓ read_ea_block;
	જો (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		જાઓ out;
	}

	/* Look for EA in the inode */
	memcpy(&ea_inode_magic, ((અક્ષર *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, કદ(__u32));
	જો (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_size = EXT2_INODE_SIZE(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_SIZE - inode->i_extra_isize -
			કદ(__u32);
		start = ((અક્ષર *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + કદ(__u32);

		err = read_xattrs_from_buffer(handle, inode,
					(સંરચના ext2_ext_attr_entry *) start,
					storage_size, start);
		જો (err)
			જાઓ out;

		handle->ibody_ગણતરી = handle->ગણતરી;
	}

read_ea_block:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_block(handle->fs, EXT2_INODE(inode));
	જો (blk != 0) {
		જો ((blk < handle->fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_ગણતરી(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_અંક;
			જાઓ out;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &block_buf);
		જો (err)
			જાઓ out;

		err = ext2fs_read_ext_attr3(handle->fs, blk, block_buf,
					    handle->ino);
		જો (err)
			જાઓ out3;

		/* We only know how to deal with v2 EA blocks */
		header = (સંરચના ext2_ext_attr_header *) block_buf;
		જો (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			જાઓ out3;
		}

		/* Read EAs */
		storage_size = handle->fs->blocksize -
			કદ(સંરચના ext2_ext_attr_header);
		start = block_buf + કદ(સંરચના ext2_ext_attr_header);
		err = read_xattrs_from_buffer(handle, inode,
					(સંરચના ext2_ext_attr_entry *) start,
					storage_size, block_buf);
	}

out3:
	જો (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	પાછા err;
}

errcode_t ext2fs_xattrs_read(સંરચના ext2_xattr_handle *handle)
{
	સંરચના ext2_inode_large *inode;
	size_t inode_size = EXT2_INODE_SIZE(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	જો (inode_size < કદ(*inode))
		inode_size = કદ(*inode);
	err = ext2fs_get_memzero(inode_size, &inode);
	જો (err)
		પાછા err;

	err = ext2fs_read_inode_full(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_SIZE(handle->fs->super));
	જો (err)
		જાઓ out;

	err = ext2fs_xattrs_read_inode(handle, inode);

out:
	ext2fs_free_mem(&inode);

	પાછા err;
}

errcode_t ext2fs_xattrs_iterate(સંરચના ext2_xattr_handle *h,
				પૂર્ણાંક (*func)(અક્ષર *name, અક્ષર *કિંમત,
					    size_t કિંમત_len, શૂન્ય *data),
				શૂન્ય *data)
{
	સંરચના ext2_xattr *x;
	પૂર્ણાંક dirty = 0;
	પૂર્ણાંક ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	માટે (x = h->attrs; x < h->attrs + h->ગણતરી; x++) {
		ret = func(x->name, x->કિંમત, x->કિંમત_len, data);
		જો (ret & XATTR_CHANGED)
			dirty = 1;
		જો (ret & XATTR_ABORT)
			તોડો;
	}

	જો (dirty)
		પાછા ext2fs_xattrs_write(h);
	પાછા 0;
}

errcode_t ext2fs_xattr_get(સંરચના ext2_xattr_handle *h, અચલ અક્ષર *ચાવી,
			   શૂન્ય **કિંમત, size_t *કિંમત_len)
{
	સંરચના ext2_xattr *x;
	અક્ષર *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	માટે (x = h->attrs; x < h->attrs + h->ગણતરી; x++) {
		જો (strcmp(x->name, ચાવી))
			ચાલુ;

		જો (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(ચાવી, "system.posix_acl_default") == 0) ||
		     (strcmp(ચાવી, "system.posix_acl_access") == 0))) {
			err = convert_disk_buffer_to_posix_acl(x->કિંમત, x->કિંમત_len,
							       કિંમત, કિંમત_len);
			પાછા err;
		} નહીંતર {
			err = ext2fs_get_mem(x->કિંમત_len, &val);
			જો (err)
				પાછા err;
			memcpy(val, x->કિંમત, x->કિંમત_len);
			*કિંમત = val;
			*કિંમત_len = x->કિંમત_len;
			પાછા 0;
		}
	}

	પાછા EXT2_ET_EA_ચાવી_NOT_FOUND;
}

errcode_t ext2fs_xattr_inode_max_size(ext2_filsys fs, ext2_ino_t ino,
				      size_t *size)
{
	સંરચના ext2_ext_attr_entry *entry;
	સંરચના ext2_inode_large *inode;
	__u32 ea_inode_magic;
	unsigned પૂર્ણાંક minoff;
	અક્ષર *start;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_SIZE(fs->super);
	જો (i < કદ(*inode))
		i = કદ(*inode);
	err = ext2fs_get_memzero(i, &inode);
	જો (err)
		પાછા err;

	err = ext2fs_read_inode_full(fs, ino, (સંરચના ext2_inode *)inode,
				     EXT2_INODE_SIZE(fs->super));
	જો (err)
		જાઓ out;

	/* Does the inode have size for EA? */
	જો (EXT2_INODE_SIZE(fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  કદ(__u32)) {
		err = EXT2_ET_INLINE_DATA_NO_SPACE;
		જાઓ out;
	}

	minoff = EXT2_INODE_SIZE(fs->super) - કદ(*inode) - કદ(__u32);
	memcpy(&ea_inode_magic, ((અક્ષર *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, કદ(__u32));
	જો (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		start= ((અક્ષર *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + કદ(__u32);
		entry = (સંરચના ext2_ext_attr_entry *) start;
		જ્યારે (!EXT2_EXT_IS_LAST_ENTRY(entry)) {
			જો (!entry->e_કિંમત_inum && entry->e_કિંમત_size) {
				unsigned પૂર્ણાંક offs = entry->e_કિંમત_offs;
				જો (offs < minoff)
					minoff = offs;
			}
			entry = EXT2_EXT_ATTR_NEXT(entry);
		}
		*size = minoff - ((અક્ષર *)entry - (અક્ષર *)start) - કદ(__u32);
	} નહીંતર {
		/* no xattr.  return a maximum size */
		*size = EXT2_EXT_ATTR_SIZE(minoff -
					   EXT2_EXT_ATTR_LEN(strlen("data")) -
					   EXT2_EXT_ATTR_ROUND - કદ(__u32));
	}

out:
	ext2fs_free_mem(&inode);
	પાછા err;
}

સ્થિર errcode_t xattr_create_ea_inode(ext2_filsys fs, અચલ શૂન્ય *કિંમત,
				       size_t કિંમત_len, ext2_ino_t *ea_ino)
{
	સંરચના ext2_inode inode;
	ext2_ino_t ino;
	ext2_file_t file;
	__u32 hash;
	errcode_t ret;

	ret = ext2fs_new_inode(fs, 0, 0, 0, &ino);
	જો (ret)
		પાછા ret;

	memset(&inode, 0, કદ(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	જો (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_size = 0;
	inode.i_mode = LINUX_S_IFREG | 0600;
	inode.i_links_ગણતરી = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	જો (ret)
		પાછા ret;
	/*
	 * ref_count and hash utilize inode's i_*time fields.
	 * ext2fs_write_new_inode() call above initializes these fields with
	 * current time. That's why ref count and hash updates are done
	 * separately below.
	 */
	ext2fs_set_ea_inode_ref(&inode, 1);
	hash = ext2fs_crc32c_le(fs->csum_seed, કિંમત, કિંમત_len);
	ext2fs_set_ea_inode_hash(&inode, hash);

	ret = ext2fs_write_inode(fs, ino, &inode);
	જો (ret)
		પાછા ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	જો (ret)
		પાછા ret;
	ret = ext2fs_file_write(file, કિંમત, કિંમત_len, ખાલી);
	ext2fs_file_close(file);
	જો (ret)
		પાછા ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	પાછા 0;
}

સ્થિર errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	સંરચના ext2_inode_large inode;
	__u64 ref_ગણતરી;
	errcode_t ret;

	ret = ext2fs_read_inode_full(fs, ino, (સંરચના ext2_inode *)&inode,
				     કદ(inode));
	જો (ret)
		જાઓ out;

	ref_ગણતરી = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_ગણતરી--;
	ext2fs_set_ea_inode_ref(EXT2_INODE(&inode), ref_ગણતરી);

	જો (ref_ગણતરી)
		જાઓ write_out;

	inode.i_links_ગણતરી = 0;
	ext2fs_set_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	જો (ret)
		જાઓ write_out;

	જો (ext2fs_inode_has_valid_blocks2(fs, (સંરચના ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (સંરચના ext2_inode *)&inode, ખાલી,
				   0, ~0ULL);
		જો (ret)
			જાઓ out;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_out:
	ret = ext2fs_write_inode_full(fs, ino, (સંરચના ext2_inode *)&inode,
				      કદ(inode));
out:
	પાછા ret;
}

સ્થિર errcode_t xattr_update_entry(ext2_filsys fs, સંરચના ext2_xattr *x,
				    અચલ અક્ષર *name, અચલ અક્ષર *short_name,
				    પૂર્ણાંક index, અચલ શૂન્ય *કિંમત,
				    size_t કિંમત_len, પૂર્ણાંક in_inode)
{
	ext2_ino_t ea_ino = 0;
	શૂન્ય *new_કિંમત = ખાલી;
	અક્ષર *new_name = ખાલી;
	પૂર્ણાંક name_len;
	errcode_t ret;

	જો (!x->name) {
		name_len = strlen(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		જો (ret)
			જાઓ fail;
		memcpy(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(કિંમત_len, &new_કિંમત);
	જો (ret)
		જાઓ fail;
	memcpy(new_કિંમત, કિંમત, કિંમત_len);

	જો (in_inode) {
		ret = xattr_create_ea_inode(fs, કિંમત, કિંમત_len, &ea_ino);
		જો (ret)
			જાઓ fail;
	}

	જો (x->ea_ino) {
		ret = xattr_inode_dec_ref(fs, x->ea_ino);
		જો (ret)
			જાઓ fail;
	}

	જો (!x->name) {
		x->name = new_name;
		x->short_name = new_name + (short_name  - name);
	}
	x->name_index = index;

	જો (x->કિંમત)
		ext2fs_free_mem(&x->કિંમત);
	x->કિંમત = new_કિંમત;
	x->કિંમત_len = કિંમત_len;
	x->ea_ino = ea_ino;
	પાછા 0;
fail:
	જો (new_name)
		ext2fs_free_mem(&new_name);
	જો (new_કિંમત)
		ext2fs_free_mem(&new_કિંમત);
	જો (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	પાછા ret;
}

સ્થિર પૂર્ણાંક xattr_find_position(સંરચના ext2_xattr *attrs, પૂર્ણાંક ગણતરી,
			       અચલ અક્ષર *shortname, પૂર્ણાંક name_idx)
{
	સંરચના ext2_xattr *x;
	પૂર્ણાંક i;
	પૂર્ણાંક shortname_len, x_shortname_len;

	shortname_len = strlen(shortname);

	માટે (i = 0, x = attrs; i < ગણતરી; i++, x++) {
		જો (name_idx < x->name_index)
			તોડો;
		જો (name_idx > x->name_index)
			ચાલુ;

		x_shortname_len = strlen(x->short_name);
		જો (shortname_len < x_shortname_len)
			તોડો;
		જો (shortname_len > x_shortname_len)
			ચાલુ;

		જો (memcmp(shortname, x->short_name, shortname_len) <= 0)
			તોડો;
	}
	પાછા i;
}

સ્થિર errcode_t xattr_array_update(સંરચના ext2_xattr_handle *h,
				    અચલ અક્ષર *name,
				    અચલ શૂન્ય *કિંમત, size_t કિંમત_len,
				    પૂર્ણાંક ibody_free, પૂર્ણાંક block_free,
				    પૂર્ણાંક old_idx, પૂર્ણાંક in_inode)
{
	સંરચના ext2_xattr tmp;
	પૂર્ણાંક add_to_ibody;
	પૂર્ણાંક needed;
	પૂર્ણાંક name_len, name_idx = 0;
	અચલ અક્ષર *shortname = name;
	પૂર્ણાંક new_idx;
	પૂર્ણાંક ret;

	find_ea_index(name, &shortname, &name_idx);
	name_len = strlen(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	જો (!in_inode)
		needed += EXT2_EXT_ATTR_SIZE(કિંમત_len);

	જો (old_idx >= 0 && old_idx < h->ibody_ગણતરી) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		જો (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_SIZE(
						h->attrs[old_idx].કિંમત_len);
	}

	જો (needed <= ibody_free) {
		જો (old_idx < 0) {
			new_idx = h->ibody_ગણતરી;
			add_to_ibody = 1;
			જાઓ add_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, કિંમત,
					 કિંમત_len, in_inode);
		જો (ret)
			પાછા ret;
		જો (h->ibody_ગણતરી <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_ગણતરી + 1,
				h->attrs + h->ibody_ગણતરી,
				(old_idx - h->ibody_ગણતરી) * કદ(*h->attrs));
			h->attrs[h->ibody_ગણતરી] = tmp;
			h->ibody_ગણતરી++;
		}
		પાછા 0;
	}

	જો (h->ibody_ગણતરી <= old_idx) {
		block_free += EXT2_EXT_ATTR_LEN(name_len);
		જો (!h->attrs[old_idx].ea_ino)
			block_free +=
				EXT2_EXT_ATTR_SIZE(h->attrs[old_idx].કિંમત_len);
	}

	જો (needed > block_free)
		પાછા EXT2_ET_EA_NO_SPACE;

	જો (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, કિંમત,
					 કિંમત_len, in_inode);
		જો (ret)
			પાછા ret;
		જો (old_idx < h->ibody_ગણતરી) {
			/*
			 * Move entry from ibody to the block. Note that
			 * entries in the block are sorted.
			 */
			new_idx = xattr_find_position(h->attrs + h->ibody_ગણતરી,
						      h->ગણતરી - h->ibody_ગણતરી,
						      shortname, name_idx);
			new_idx += h->ibody_ગણતરી - 1;
			tmp = h->attrs[old_idx];
			memmove(h->attrs + old_idx, h->attrs + old_idx + 1,
				(new_idx - old_idx) * કદ(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_ગણતરી--;
		}
		પાછા 0;
	}

	new_idx = xattr_find_position(h->attrs + h->ibody_ગણતરી,
				      h->ગણતરી - h->ibody_ગણતરી,
				      shortname, name_idx);
	new_idx += h->ibody_ગણતરી;
	add_to_ibody = 0;

add_new:
	જો (h->ગણતરી == h->capacity) {
		ret = ext2fs_xattrs_expand(h, 4);
		જો (ret)
			પાછા ret;
	}

	ret = xattr_update_entry(h->fs, &h->attrs[h->ગણતરી], name, shortname,
				 name_idx, કિંમત, કિંમત_len, in_inode);
	જો (ret)
		પાછા ret;

	tmp = h->attrs[h->ગણતરી];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->ગણતરી - new_idx)*કદ(*h->attrs));
	h->attrs[new_idx] = tmp;
	જો (add_to_ibody)
		h->ibody_ગણતરી++;
	h->ગણતરી++;
	પાછા 0;
}

સ્થિર પૂર્ણાંક space_used(સંરચના ext2_xattr *attrs, પૂર્ણાંક ગણતરી)
{
	પૂર્ણાંક total = 0;
	સંરચના ext2_xattr *x;
	પૂર્ણાંક i, len;

	માટે (i = 0, x = attrs; i < ગણતરી; i++, x++) {
		len = strlen(x->short_name);
		total += EXT2_EXT_ATTR_LEN(len);
		જો (!x->ea_ino)
			total += EXT2_EXT_ATTR_SIZE(x->કિંમત_len);
	}
	પાછા total;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#define EXT4_XATTR_MIN_LARGE_EA_SIZE(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - કદ(સંરચના ext2_ext_attr_header) - 4)

errcode_t ext2fs_xattr_set(સંરચના ext2_xattr_handle *h,
			   અચલ અક્ષર *name,
			   અચલ શૂન્ય *કિંમત,
			   size_t કિંમત_len)
{
	ext2_filsys fs = h->fs;
	અચલ પૂર્ણાંક inode_size = EXT2_INODE_SIZE(fs->super);
	સંરચના ext2_inode_large *inode = ખાલી;
	સંરચના ext2_xattr *x;
	અક્ષર *new_કિંમત;
	પૂર્ણાંક ibody_free, block_free;
	પૂર્ણાંક in_inode = 0;
	પૂર્ણાંક old_idx = -1;
	પૂર્ણાંક extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(કિંમત_len, &new_કિંમત);
	જો (ret)
		પાછા ret;
	જો (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((strcmp(name, "system.posix_acl_default") == 0) ||
	     (strcmp(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_to_disk_buffer(કિંમત, કિંમત_len,
						       new_કિંમત, &કિંમત_len);
		જો (ret)
			જાઓ out;
	} નહીંતર જો (કિંમત_len)
		memcpy(new_કિંમત, કિંમત, કિંમત_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	માટે (x = h->attrs; x < h->attrs + h->ગણતરી; x++) {
		જો (!strcmp(x->name, name)) {
			જો (!x->ea_ino && x->કિંમત_len == કિંમત_len &&
			    (!કિંમત_len ||
			     !memcmp(x->કિંમત, new_કિંમત, કિંમત_len))) {
				ret = 0;
				જાઓ out;
			}
			old_idx = x - h->attrs;
			તોડો;
		}
	}

	ret = ext2fs_get_memzero(inode_size, &inode);
	જો (ret)
		જાઓ out;
	ret = ext2fs_read_inode_full(fs, h->ino,
				     (સંરચના ext2_inode *)inode,
				     inode_size);
	જો (ret)
		જાઓ out;
	જો (inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		extra_isize = inode->i_extra_isize;
		જો (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			જો (extra_isize == 0)
				extra_isize = કદ(__u32);
		}
		ibody_free = inode_size - EXT2_GOOD_OLD_INODE_SIZE;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= કદ(__u32) * 2;
		ibody_free -= space_used(h->attrs, h->ibody_ગણતરી);
	} નહીંતર
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	જો (strcmp(name, "system.data") == 0) {
		જો (h->ibody_ગણતરી <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			જાઓ out;
		}
		ret = xattr_array_update(h, name, new_કિંમત, કિંમત_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		જો (ret)
			જાઓ out;
		જાઓ write_out;
	}

	block_free = fs->blocksize;
	block_free -= કદ(સંરચના ext2_ext_attr_header);
	/* Final null entry. */
	block_free -= કદ(__u32);
	block_free -= space_used(h->attrs + h->ibody_ગણતરી,
				 h->ગણતરી - h->ibody_ગણતરી);

	જો (ext2fs_has_feature_ea_inode(fs->super) &&
	    કિંમત_len > EXT4_XATTR_MIN_LARGE_EA_SIZE(fs->blocksize))
		in_inode = 1;

	ret = xattr_array_update(h, name, new_કિંમત, કિંમત_len, ibody_free,
				 block_free, old_idx, in_inode);
	જો (ret == EXT2_ET_EA_NO_SPACE && !in_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_કિંમત, કિંમત_len,
			ibody_free, block_free, old_idx, 1 /* in_inode */);
	જો (ret)
		જાઓ out;

write_out:
	ret = ext2fs_xattrs_write(h);
out:
	જો (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_કિંમત);
	પાછા ret;
}

errcode_t ext2fs_xattr_remove(સંરચના ext2_xattr_handle *handle,
			      અચલ અક્ષર *ચાવી)
{
	સંરચના ext2_xattr *x;
	સંરચના ext2_xattr *end = handle->attrs + handle->ગણતરી;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	માટે (x = handle->attrs; x < end; x++) {
		જો (strcmp(x->name, ચાવી) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->કિંમત);
			જો (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (end - x - 1)*કદ(*x));
			memset(end - 1, 0, કદ(*end));
			જો (x < handle->attrs + handle->ibody_ગણતરી)
				handle->ibody_ગણતરી--;
			handle->ગણતરી--;
			પાછા ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	પાછા 0;
}

errcode_t ext2fs_xattr_remove_all(સંરચના ext2_xattr_handle *handle)
{
	સંરચના ext2_xattr *x;
	સંરચના ext2_xattr *end = handle->attrs + handle->ગણતરી;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	માટે (x = handle->attrs; x < end; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->કિંમત);
		જો (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_ગણતરી = 0;
	handle->ગણતરી = 0;
	પાછા ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     સંરચના ext2_xattr_handle **handle)
{
	સંરચના ext2_xattr_handle *h;
	errcode_t err;

	જો (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_data(fs->super))
		પાછા EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(કદ(*h), &h);
	જો (err)
		પાછા err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->capacity = 4;
	err = ext2fs_get_arrayzero(h->capacity, કદ(સંરચના ext2_xattr),
				   &h->attrs);
	જો (err) {
		ext2fs_free_mem(&h);
		પાછા err;
	}
	h->ગણતરી = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	પાછા 0;
}

errcode_t ext2fs_xattrs_close(સંરચના ext2_xattr_handle **handle)
{
	સંરચના ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_ચાવીઓ(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	પાછા 0;
}

errcode_t ext2fs_xattrs_ગણતરી(સંરચના ext2_xattr_handle *handle, size_t *ગણતરી)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*ગણતરી = handle->ગણતરી;
	પાછા 0;
}

errcode_t ext2fs_xattrs_flags(સંરચના ext2_xattr_handle *handle,
			      unsigned પૂર્ણાંક *new_flags, unsigned પૂર્ણાંક *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	જો (old_flags)
		*old_flags = handle->flags;
	જો (new_flags)
		handle->flags = *new_flags;
	પાછા 0;
}
