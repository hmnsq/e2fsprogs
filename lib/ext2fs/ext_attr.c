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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <time.h>

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2_ext_attr.h"
#അന്തര്ഭാവ "ext4_acl.h"

#അന്തര്ഭാവ "ext2fsP.h"

സ്ഥിതം errcode_t read_ea_inode_hash(ext2_filsys fs, ext2_ino_t ino, __u32 *hash)
{
	ഘടന ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	എങ്കിൽ (retval)
		തിരികെ retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	തിരികെ 0;
}

#define NAME_HASH_SHIFT 5
#define മൂല്യം_HASH_SHIFT 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_entry(ഘടന ext2_ext_attr_entry *entry, ശൂന്യം *data)
{
	__u32 hash = 0;
	unsigned അക്ഷരം *name = (((unsigned അക്ഷരം *) entry) +
			       വലുപ്പം(ഘടന ext2_ext_attr_entry));
	പൂർണ്ണസംഖ്യ n;

	വേണ്ടി (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*വലുപ്പം(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	എങ്കിൽ (entry->e_മൂല്യം_inum == 0 && entry->e_മൂല്യം_size != 0) {
		__u32 *മൂല്യം = (__u32 *)data;
		വേണ്ടി (n = (entry->e_മൂല്യം_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << മൂല്യം_HASH_SHIFT) ^
			       (hash >> (8*വലുപ്പം(hash) - മൂല്യം_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*മൂല്യം++);
		}
	}

	തിരികെ hash;
}

__u32 ext2fs_ext_attr_hash_entry_signed(ഘടന ext2_ext_attr_entry *entry,
					ശൂന്യം *data)
{
	__u32 hash = 0;
	signed അക്ഷരം *name = (((signed അക്ഷരം *) entry) +
			     വലുപ്പം(ഘടന ext2_ext_attr_entry));
	പൂർണ്ണസംഖ്യ n;

	വേണ്ടി (n = 0; n < entry->e_name_len; n++) {
		hash = (hash << NAME_HASH_SHIFT) ^
		       (hash >> (8*വലുപ്പം(hash) - NAME_HASH_SHIFT)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	എങ്കിൽ (entry->e_മൂല്യം_inum == 0 && entry->e_മൂല്യം_size != 0) {
		__u32 *മൂല്യം = (__u32 *)data;
		വേണ്ടി (n = (entry->e_മൂല്യം_size + EXT2_EXT_ATTR_ROUND) >>
			 EXT2_EXT_ATTR_PAD_BITS; n; n--) {
			hash = (hash << മൂല്യം_HASH_SHIFT) ^
			       (hash >> (8*വലുപ്പം(hash) - മൂല്യം_HASH_SHIFT)) ^
			       ext2fs_le32_to_cpu(*മൂല്യം++);
		}
	}

	തിരികെ hash;
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
				      ഘടന ext2_ext_attr_entry *entry,
				      ശൂന്യം *data, __u32 *hash,
				      __u32 *signed_hash)
{
	*hash = ext2fs_ext_attr_hash_entry(entry, data);
	എങ്കിൽ (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_entry_signed(entry, data);

	എങ്കിൽ (entry->e_മൂല്യം_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, entry->e_മൂല്യം_inum,
					    &ea_inode_hash);
		എങ്കിൽ (retval)
			തിരികെ retval;

		*hash = (*hash << മൂല്യം_HASH_SHIFT) ^
			(*hash >> (8*വലുപ്പം(*hash) - മൂല്യം_HASH_SHIFT)) ^
			ea_inode_hash;
		എങ്കിൽ (signed_hash)
			*signed_hash = (*signed_hash << മൂല്യം_HASH_SHIFT) ^
				(*signed_hash >> (8*വലുപ്പം(*hash) -
						  മൂല്യം_HASH_SHIFT)) ^
				ea_inode_hash;
	}
	തിരികെ 0;
}

/*
 * ext2fs_ext_attr_hash_entry2()
 *
 * Compute the hash of an extended attribute.
 * This version of the function supports hashing entries that reference
 * external inodes (ea_inode feature).
 */
errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
				      ഘടന ext2_ext_attr_entry *entry,
				      ശൂന്യം *data, __u32 *hash)
{
	തിരികെ ext2fs_ext_attr_hash_entry3(fs, entry, data, hash, ശൂന്യ);
}

#undef NAME_HASH_SHIFT
#undef മൂല്യം_HASH_SHIFT

#define BLOCK_HASH_SHIFT 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
ശൂന്യം ext2fs_ext_attr_block_rehash(ഘടന ext2_ext_attr_header *header,
				  ഘടന ext2_ext_attr_entry *end)
{
	ഘടന ext2_ext_attr_entry *here;
	__u32 hash = 0;

	here = (ഘടന ext2_ext_attr_entry *)(header+1);
	ആകുമ്പോൾ (here < end && !EXT2_EXT_IS_LAST_ENTRY(here)) {
		എങ്കിൽ (!here->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			നിർത്തുക;
		}
		hash = (hash << BLOCK_HASH_SHIFT) ^
		       (hash >> (8*വലുപ്പം(hash) - BLOCK_HASH_SHIFT)) ^
		       here->e_hash;
		here = EXT2_EXT_ATTR_NEXT(here);
	}
	header->h_hash = hash;
}

#undef BLOCK_HASH_SHIFT

__u32 ext2fs_get_ea_inode_hash(ഘടന ext2_inode *inode)
{
	തിരികെ inode->i_atime;
}

ശൂന്യം ext2fs_set_ea_inode_hash(ഘടന ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(ഘടന ext2_inode *inode)
{
	തിരികെ ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_version;
}

ശൂന്യം ext2fs_set_ea_inode_ref(ഘടന ext2_inode *inode, __u64 ref_എണ്ണം)
{
	inode->i_ctime = (__u32)(ref_എണ്ണം >> 32);
	inode->osd1.linux1.l_i_version = (__u32)ref_എണ്ണം;
}

സ്ഥിതം errcode_t check_ext_attr_header(ഘടന ext2_ext_attr_header *header)
{
	എങ്കിൽ ((header->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     header->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    header->h_blocks != 1)
		തിരികെ EXT2_ET_BAD_EA_HEADER;

	തിരികെ 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t block, ശൂന്യം *buf,
				ext2_ino_t inum)
{
	പൂർണ്ണസംഖ്യ		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, block, 1, buf);
	എങ്കിൽ (retval)
		തിരികെ retval;

	എങ്കിൽ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_block_csum_verify(fs, inum, block, buf))
		csum_failed = 1;

#ifdef വാക്കുകൾ_BIGENDIAN
	ext2fs_swap_ext_attr(buf, buf, fs->blocksize, 1);
#endif

	retval = check_ext_attr_header(buf);
	എങ്കിൽ (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	തിരികെ retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t block, ശൂന്യം *buf)
{
	തിരികെ ext2fs_read_ext_attr3(fs, block, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t block, ശൂന്യം *buf)
{
	തിരികെ ext2fs_read_ext_attr2(fs, block, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t block, ശൂന്യം *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	അക്ഷരം		*write_buf;

#ifdef വാക്കുകൾ_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	എങ്കിൽ (retval)
		തിരികെ retval;
	ext2fs_swap_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#അല്ലെങ്കിൽ
	write_buf = (അക്ഷരം *) inbuf;
#endif

	retval = ext2fs_ext_attr_block_csum_set(fs, inum, block,
			(ഘടന ext2_ext_attr_header *)write_buf);
	എങ്കിൽ (retval)
		തിരികെ retval;

	retval = io_channel_write_blk64(fs->io, block, 1, write_buf);
#ifdef വാക്കുകൾ_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#endif
	എങ്കിൽ (!retval)
		ext2fs_mark_changed(fs);
	തിരികെ retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t block, ശൂന്യം *inbuf)
{
	തിരികെ ext2fs_write_ext_attr3(fs, block, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t block, ശൂന്യം *inbuf)
{
	തിരികെ ext2fs_write_ext_attr2(fs, block, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    അക്ഷരം *block_buf, പൂർണ്ണസംഖ്യ adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	ഘടന ext2_ext_attr_header *header;
	അക്ഷരം	*buf = 0;

	എങ്കിൽ ((blk >= ext2fs_blocks_എണ്ണം(fs->super)) ||
	    (blk < fs->super->s_first_data_block))
		തിരികെ EXT2_ET_BAD_EA_BLOCK_അക്കം;

	എങ്കിൽ (!block_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		എങ്കിൽ (retval)
			തിരികെ retval;
		block_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, block_buf, inum);
	എങ്കിൽ (retval)
		പോകുക errout;

	header = (ഘടന ext2_ext_attr_header *) block_buf;
	header->h_refcount += adjust;
	എങ്കിൽ (newcount)
		*newcount = header->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, block_buf, inum);
	എങ്കിൽ (retval)
		പോകുക errout;

errout:
	എങ്കിൽ (buf)
		ext2fs_free_mem(&buf);
	തിരികെ retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    അക്ഷരം *block_buf, പൂർണ്ണസംഖ്യ adjust,
				    __u32 *newcount)
{
	തിരികെ ext2fs_adjust_ea_refcount3(fs, blk, block_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					അക്ഷരം *block_buf, പൂർണ്ണസംഖ്യ adjust,
					__u32 *newcount)
{
	തിരികെ ext2fs_adjust_ea_refcount2(fs, blk, block_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
ഘടന ext2_xattr {
	പൂർണ്ണസംഖ്യ name_index;
	അക്ഷരം *name;
	അക്ഷരം *short_name;
	ശൂന്യം *മൂല്യം;
	unsigned പൂർണ്ണസംഖ്യ മൂല്യം_len;
	ext2_ino_t ea_ino;
};

ഘടന ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	ഘടന ext2_xattr *attrs;
	പൂർണ്ണസംഖ്യ capacity;
	പൂർണ്ണസംഖ്യ എണ്ണം;
	പൂർണ്ണസംഖ്യ ibody_എണ്ണം;
	ext2_ino_t ino;
	unsigned പൂർണ്ണസംഖ്യ flags;
};

സ്ഥിതം errcode_t ext2fs_xattrs_expand(ഘടന ext2_xattr_handle *h,
				      unsigned പൂർണ്ണസംഖ്യ expandby)
{
	ഘടന ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->capacity + expandby,
				   വലുപ്പം(ഘടന ext2_xattr), &new_attrs);
	എങ്കിൽ (err)
		തിരികെ err;

	memcpy(new_attrs, h->attrs, h->capacity * വലുപ്പം(ഘടന ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->capacity += expandby;
	h->attrs = new_attrs;

	തിരികെ 0;
}

ഘടന ea_name_index {
	പൂർണ്ണസംഖ്യ index;
	സ്ഥിരം അക്ഷരം *name;
};

/* Keep these names sorted in order of decreasing specificity. */
സ്ഥിതം ഘടന ea_name_index ea_names[] = {
	{10, "gnu."},
	{3, "system.posix_acl_default"},
	{2, "system.posix_acl_access"},
	{8, "system.richacl"},
	{6, "security."},
	{4, "trusted."},
	{7, "system."},
	{1, "user."},
	{0, ശൂന്യ},
};

സ്ഥിതം സ്ഥിരം അക്ഷരം *find_ea_prefix(പൂർണ്ണസംഖ്യ index)
{
	ഘടന ea_name_index *e;

	വേണ്ടി (e = ea_names; e->name; e++)
		എങ്കിൽ (e->index == index)
			തിരികെ e->name;

	തിരികെ ശൂന്യ;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ find_ea_index(സ്ഥിരം അക്ഷരം *fullname, സ്ഥിരം അക്ഷരം **name, പൂർണ്ണസംഖ്യ *index)
{
	ഘടന ea_name_index *e;

	വേണ്ടി (e = ea_names; e->name; e++) {
		എങ്കിൽ (strncmp(fullname, e->name, strlen(e->name)) == 0) {
			*name = fullname + strlen(e->name);
			*index = e->index;
			തിരികെ 1;
		}
	}
	തിരികെ 0;
}

errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       ഘടന ext2_inode_large *inode)
{
	ഘടന ext2_ext_attr_header *header;
	ശൂന്യം *block_buf = ശൂന്യ;
	blk64_t blk;
	errcode_t err;
	ഘടന ext2_inode_large i;

	/* Read inode? */
	എങ്കിൽ (inode == ശൂന്യ) {
		err = ext2fs_read_inode_full(fs, ino, (ഘടന ext2_inode *)&i,
					     വലുപ്പം(ഘടന ext2_inode_large));
		എങ്കിൽ (err)
			തിരികെ err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (ഘടന ext2_inode *)inode);
	എങ്കിൽ (blk == 0)
		തിരികെ 0;

	/* Find block, zero it, write back */
	എങ്കിൽ ((blk < fs->super->s_first_data_block) ||
	    (blk >= ext2fs_blocks_എണ്ണം(fs->super))) {
		err = EXT2_ET_BAD_EA_BLOCK_അക്കം;
		പോകുക out;
	}

	err = ext2fs_get_mem(fs->blocksize, &block_buf);
	എങ്കിൽ (err)
		പോകുക out;

	err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
	എങ്കിൽ (err)
		പോകുക out2;

	/* We only know how to deal with v2 EA blocks */
	header = (ഘടന ext2_ext_attr_header *) block_buf;
	എങ്കിൽ (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_HEADER;
		പോകുക out2;
	}

	header->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
	എങ്കിൽ (err)
		പോകുക out2;

	/* Erase link to block */
	ext2fs_file_acl_block_set(fs, (ഘടന ext2_inode *)inode, 0);
	എങ്കിൽ (header->h_refcount == 0)
		ext2fs_block_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_blocks(fs, (ഘടന ext2_inode *)inode, 1);
	എങ്കിൽ (err)
		പോകുക out2;

	/* Write inode? */
	എങ്കിൽ (inode == &i) {
		err = ext2fs_write_inode_full(fs, ino, (ഘടന ext2_inode *)&i,
					      വലുപ്പം(ഘടന ext2_inode_large));
		എങ്കിൽ (err)
			പോകുക out2;
	}

out2:
	ext2fs_free_mem(&block_buf);
out:
	തിരികെ err;
}

സ്ഥിതം errcode_t prep_ea_block_for_write(ext2_filsys fs, ext2_ino_t ino,
					 ഘടന ext2_inode_large *inode)
{
	ഘടന ext2_ext_attr_header *header;
	ശൂന്യം *block_buf = ശൂന്യ;
	blk64_t blk, goal;
	errcode_t err;

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_block(fs, (ഘടന ext2_inode *)inode);
	എങ്കിൽ (blk != 0) {
		എങ്കിൽ ((blk < fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_എണ്ണം(fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_അക്കം;
			പോകുക out;
		}

		err = ext2fs_get_mem(fs->blocksize, &block_buf);
		എങ്കിൽ (err)
			പോകുക out;

		err = ext2fs_read_ext_attr3(fs, blk, block_buf, ino);
		എങ്കിൽ (err)
			പോകുക out2;

		/* We only know how to deal with v2 EA blocks */
		header = (ഘടന ext2_ext_attr_header *) block_buf;
		എങ്കിൽ (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			പോകുക out2;
		}

		/* Single-user block.  We're done here. */
		എങ്കിൽ (header->h_refcount == 1)
			പോകുക out2;

		/* We need to CoW the block. */
		header->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, block_buf, ino);
		എങ്കിൽ (err)
			പോകുക out2;
	} അല്ലെങ്കിൽ {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_add_blocks(fs, (ഘടന ext2_inode *)inode,
					     1);
		എങ്കിൽ (err)
			പോകുക out;
	}

	/* Allocate a block */
	goal = ext2fs_find_inode_goal(fs, ino, (ഘടന ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, ശൂന്യ, &blk);
	എങ്കിൽ (err)
		പോകുക out2;
	ext2fs_file_acl_block_set(fs, (ഘടന ext2_inode *)inode, blk);
out2:
	എങ്കിൽ (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	തിരികെ err;
}


സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
posix_acl_xattr_എണ്ണം(size_t size)
{
        എങ്കിൽ (size < വലുപ്പം(posix_acl_xattr_header))
                തിരികെ -1;
        size -= വലുപ്പം(posix_acl_xattr_header);
        എങ്കിൽ (size % വലുപ്പം(posix_acl_xattr_entry))
                തിരികെ -1;
        തിരികെ size / വലുപ്പം(posix_acl_xattr_entry);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
സ്ഥിതം errcode_t convert_posix_acl_to_disk_buffer(സ്ഥിരം ശൂന്യം *മൂല്യം, size_t size,
						  ശൂന്യം *out_buf, size_t *size_out)
{
	സ്ഥിരം posix_acl_xattr_header *header =
		(സ്ഥിരം posix_acl_xattr_header*) മൂല്യം;
	സ്ഥിരം posix_acl_xattr_entry *end, *entry =
		(സ്ഥിരം posix_acl_xattr_entry *)(header+1);
	ext4_acl_header *ext_acl;
	size_t s;
	അക്ഷരം *e;

	പൂർണ്ണസംഖ്യ എണ്ണം;

	എങ്കിൽ (!മൂല്യം)
		തിരികെ EINVAL;
	എങ്കിൽ (size < വലുപ്പം(posix_acl_xattr_header))
		തിരികെ ENOMEM;
	എങ്കിൽ (header->a_version != ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION))
		തിരികെ EINVAL;

	എണ്ണം = posix_acl_xattr_എണ്ണം(size);
	ext_acl = out_buf;
	ext_acl->a_version = ext2fs_cpu_to_le32(EXT4_ACL_VERSION);

	എങ്കിൽ (എണ്ണം <= 0)
		തിരികെ EINVAL;

	e = (അക്ഷരം *) out_buf + വലുപ്പം(ext4_acl_header);
	s = വലുപ്പം(ext4_acl_header);
	വേണ്ടി (end = entry + എണ്ണം; entry != end;entry++) {
		ext4_acl_entry *disk_entry = (ext4_acl_entry*) e;
		disk_entry->e_tag = entry->e_tag;
		disk_entry->e_perm = entry->e_perm;

		തിരഞ്ഞെടുപ്പ്(ext2fs_le16_to_cpu(entry->e_tag)) {
			അവസ്ഥ ACL_USER_OBJ:
			അവസ്ഥ ACL_GROUP_OBJ:
			അവസ്ഥ ACL_MASK:
			അവസ്ഥ ACL_OTHER:
				e += വലുപ്പം(ext4_acl_entry_short);
				s += വലുപ്പം(ext4_acl_entry_short);
				നിർത്തുക;
			അവസ്ഥ ACL_USER:
			അവസ്ഥ ACL_GROUP:
				disk_entry->e_id = entry->e_id;
				e += വലുപ്പം(ext4_acl_entry);
				s += വലുപ്പം(ext4_acl_entry);
				നിർത്തുക;
			സ്ഥിരസ്ഥിതി:
				തിരികെ EINVAL;
		}
	}
	*size_out = s;
	തിരികെ 0;
}

സ്ഥിതം errcode_t convert_disk_buffer_to_posix_acl(സ്ഥിരം ശൂന്യം *മൂല്യം, size_t size,
						  ശൂന്യം **out_buf, size_t *size_out)
{
	posix_acl_xattr_header *header;
	posix_acl_xattr_entry *entry;
	സ്ഥിരം ext4_acl_header *ext_acl = (സ്ഥിരം ext4_acl_header *) മൂല്യം;
	errcode_t err;
	സ്ഥിരം അക്ഷരം *cp;
	അക്ഷരം *out;

	എങ്കിൽ ((!മൂല്യം) ||
	    (size < വലുപ്പം(ext4_acl_header)) ||
	    (ext_acl->a_version != ext2fs_cpu_to_le32(EXT4_ACL_VERSION)))
		തിരികെ EINVAL;

	err = ext2fs_get_mem(size * 2, &out);
	എങ്കിൽ (err)
		തിരികെ err;

	header = (posix_acl_xattr_header *) out;
	header->a_version = ext2fs_cpu_to_le32(POSIX_ACL_XATTR_VERSION);
	entry = (posix_acl_xattr_entry *) (out + വലുപ്പം(posix_acl_xattr_header));

	cp = (സ്ഥിരം അക്ഷരം *) മൂല്യം + വലുപ്പം(ext4_acl_header);
	size -= വലുപ്പം(ext4_acl_header);

	ആകുമ്പോൾ (size > 0) {
		സ്ഥിരം ext4_acl_entry *disk_entry = (സ്ഥിരം ext4_acl_entry *) cp;

		entry->e_tag = disk_entry->e_tag;
		entry->e_perm = disk_entry->e_perm;

		തിരഞ്ഞെടുപ്പ്(ext2fs_le16_to_cpu(entry->e_tag)) {
			അവസ്ഥ ACL_USER_OBJ:
			അവസ്ഥ ACL_GROUP_OBJ:
			അവസ്ഥ ACL_MASK:
			അവസ്ഥ ACL_OTHER:
				entry->e_id = 0;
				cp += വലുപ്പം(ext4_acl_entry_short);
				size -= വലുപ്പം(ext4_acl_entry_short);
				നിർത്തുക;
			അവസ്ഥ ACL_USER:
			അവസ്ഥ ACL_GROUP:
				entry->e_id = disk_entry->e_id;
				cp += വലുപ്പം(ext4_acl_entry);
				size -= വലുപ്പം(ext4_acl_entry);
				നിർത്തുക;
			സ്ഥിരസ്ഥിതി:
				ext2fs_free_mem(&out);
				തിരികെ EINVAL;
		}
		entry++;
	}
	*out_buf = out;
	*size_out = ((അക്ഷരം *) entry - out);
	തിരികെ 0;
}

സ്ഥിതം errcode_t
write_xattrs_to_buffer(ext2_filsys fs, ഘടന ext2_xattr *attrs, പൂർണ്ണസംഖ്യ എണ്ണം,
		       ശൂന്യം *entries_start, unsigned പൂർണ്ണസംഖ്യ storage_size,
		       unsigned പൂർണ്ണസംഖ്യ മൂല്യം_offset_correction, പൂർണ്ണസംഖ്യ write_hash)
{
	ഘടന ext2_xattr *x;
	ഘടന ext2_ext_attr_entry *e = entries_start;
	അക്ഷരം *end = (അക്ഷരം *) entries_start + storage_size;
	unsigned പൂർണ്ണസംഖ്യ മൂല്യം_size;
	errcode_t err;

	memset(entries_start, 0, storage_size);
	വേണ്ടി (x = attrs; x < attrs + എണ്ണം; x++) {
		മൂല്യം_size = ((x->മൂല്യം_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = strlen(x->short_name);
		e->e_name_index = x->name_index;

		e->e_മൂല്യം_size = x->മൂല്യം_len;
		e->e_മൂല്യം_inum = x->ea_ino;

		/* Store name */
		memcpy((അക്ഷരം *)e + വലുപ്പം(*e), x->short_name, e->e_name_len);
		എങ്കിൽ (x->ea_ino) {
			e->e_മൂല്യം_offs = 0;
		} അല്ലെങ്കിൽ {
			end -= മൂല്യം_size;
			e->e_മൂല്യം_offs = end - (അക്ഷരം *) entries_start +
						മൂല്യം_offset_correction;
			memcpy(end, x->മൂല്യം, e->e_മൂല്യം_size);
		}

		എങ്കിൽ (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : end,
							  &e->e_hash);
			എങ്കിൽ (err)
				തിരികെ err;
		} അല്ലെങ്കിൽ
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_NEXT(e);
		*(__u32 *)e = 0;
	}
	തിരികെ 0;
}

errcode_t ext2fs_xattrs_write(ഘടന ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ inode_size = EXT2_INODE_SIZE(fs->super);
	ഘടന ext2_inode_large *inode;
	അക്ഷരം *start, *block_buf = ശൂന്യ;
	ഘടന ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	blk64_t blk;
	unsigned പൂർണ്ണസംഖ്യ storage_size;
	unsigned പൂർണ്ണസംഖ്യ i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_size;
	എങ്കിൽ (i < വലുപ്പം(*inode))
		i = വലുപ്പം(*inode);
	err = ext2fs_get_memzero(i, &inode);
	എങ്കിൽ (err)
		തിരികെ err;

	err = ext2fs_read_inode_full(fs, handle->ino, EXT2_INODE(inode),
				     inode_size);
	എങ്കിൽ (err)
		പോകുക out;

	/* If extra_isize isn't set, we need to set it now */
	എങ്കിൽ (inode->i_extra_isize == 0 &&
	    inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		അക്ഷരം *p = (അക്ഷരം *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		എങ്കിൽ (extra == 0)
			extra = വലുപ്പം(__u32);
		memset(p + EXT2_GOOD_OLD_INODE_SIZE, 0, extra);
		inode->i_extra_isize = extra;
	}
	എങ്കിൽ (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		പോകുക out;
	}

	/* Does the inode have space for EA? */
	എങ്കിൽ (inode->i_extra_isize < വലുപ്പം(inode->i_extra_isize) ||
	    inode_size <= EXT2_GOOD_OLD_INODE_SIZE + inode->i_extra_isize +
								വലുപ്പം(__u32))
		പോകുക write_ea_block;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	memcpy(((അക്ഷരം *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, &ea_inode_magic, വലുപ്പം(__u32));
	storage_size = inode_size - EXT2_GOOD_OLD_INODE_SIZE -
				inode->i_extra_isize - വലുപ്പം(__u32);
	start = ((അക്ഷരം *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
				inode->i_extra_isize + വലുപ്പം(__u32);

	err = write_xattrs_to_buffer(fs, handle->attrs, handle->ibody_എണ്ണം,
				     start, storage_size, 0, 0);
	എങ്കിൽ (err)
		പോകുക out;
write_ea_block:
	/* Are we done? */
	എങ്കിൽ (handle->ibody_എണ്ണം == handle->എണ്ണം &&
	    !ext2fs_file_acl_block(fs, EXT2_INODE(inode)))
		പോകുക skip_ea_block;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &block_buf);
	എങ്കിൽ (err)
		പോകുക out;

	storage_size = fs->blocksize - വലുപ്പം(ഘടന ext2_ext_attr_header);
	start = block_buf + വലുപ്പം(ഘടന ext2_ext_attr_header);

	err = write_xattrs_to_buffer(fs, handle->attrs + handle->ibody_എണ്ണം,
				     handle->എണ്ണം - handle->ibody_എണ്ണം, start,
				     storage_size, start - block_buf, 1);
	എങ്കിൽ (err)
		പോകുക out2;

	/* Write a header on the EA block */
	header = (ഘടന ext2_ext_attr_header *) block_buf;
	header->h_magic = EXT2_EXT_ATTR_MAGIC;
	header->h_refcount = 1;
	header->h_blocks = 1;

	/* Get a new block for writing */
	err = prep_ea_block_for_write(fs, handle->ino, inode);
	എങ്കിൽ (err)
		പോകുക out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_block(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, block_buf, handle->ino);
	എങ്കിൽ (err)
		പോകുക out2;

skip_ea_block:
	blk = ext2fs_file_acl_block(fs, (ഘടന ext2_inode *)inode);
	എങ്കിൽ (!block_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		എങ്കിൽ (err)
			പോകുക out;
	}

	/* Write the inode */
	err = ext2fs_write_inode_full(fs, handle->ino, EXT2_INODE(inode),
				      inode_size);
	എങ്കിൽ (err)
		പോകുക out2;

out2:
	ext2fs_free_mem(&block_buf);
out:
	ext2fs_free_mem(&inode);
	തിരികെ err;
}

സ്ഥിതം errcode_t read_xattrs_from_buffer(ഘടന ext2_xattr_handle *handle,
					 ഘടന ext2_inode_large *inode,
					 ഘടന ext2_ext_attr_entry *entries,
					 unsigned പൂർണ്ണസംഖ്യ storage_size,
					 അക്ഷരം *മൂല്യം_start)
{
	ഘടന ext2_xattr *x;
	ഘടന ext2_ext_attr_entry *entry, *end;
	സ്ഥിരം അക്ഷരം *prefix;
	unsigned പൂർണ്ണസംഖ്യ remain, prefix_len;
	errcode_t err;
	unsigned പൂർണ്ണസംഖ്യ മൂല്യങ്ങൾ_size = storage_size +
			((അക്ഷരം *)entries - മൂല്യം_start);

	/* find the end */
	end = entries;
	remain = storage_size;
	ആകുമ്പോൾ (remain >= വലുപ്പം(ഘടന ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(end)) {

		/* header eats this space */
		remain -= വലുപ്പം(ഘടന ext2_ext_attr_entry);

		/* is attribute name valid? */
		എങ്കിൽ (EXT2_EXT_ATTR_SIZE(end->e_name_len) > remain)
			തിരികെ EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(end->e_name_len);
		end = EXT2_EXT_ATTR_NEXT(end);
	}

	entry = entries;
	remain = storage_size;
	ആകുമ്പോൾ (remain >= വലുപ്പം(ഘടന ext2_ext_attr_entry) &&
	       !EXT2_EXT_IS_LAST_ENTRY(entry)) {

		/* Allocate space for more attrs? */
		എങ്കിൽ (handle->എണ്ണം == handle->capacity) {
			err = ext2fs_xattrs_expand(handle, 4);
			എങ്കിൽ (err)
				തിരികെ err;
		}

		x = handle->attrs + handle->എണ്ണം;

		/* header eats this space */
		remain -= വലുപ്പം(ഘടന ext2_ext_attr_entry);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_SIZE(entry->e_name_len);

		/* Extract name */
		prefix = find_ea_prefix(entry->e_name_index);
		prefix_len = (prefix ? strlen(prefix) : 0);
		err = ext2fs_get_memzero(entry->e_name_len + prefix_len + 1,
					 &x->name);
		എങ്കിൽ (err)
			തിരികെ err;
		എങ്കിൽ (prefix)
			memcpy(x->name, prefix, prefix_len);
		എങ്കിൽ (entry->e_name_len)
			memcpy(x->name + prefix_len,
			       (അക്ഷരം *)entry + വലുപ്പം(*entry),
			       entry->e_name_len);
		x->short_name = x->name + prefix_len;
		x->name_index = entry->e_name_index;

		/* Check & copy value */
		എങ്കിൽ (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    entry->e_മൂല്യം_inum != 0)
			തിരികെ EXT2_ET_BAD_EA_BLOCK_അക്കം;

		എങ്കിൽ (entry->e_മൂല്യം_inum == 0) {
			എങ്കിൽ (entry->e_മൂല്യം_size > remain)
				തിരികെ EXT2_ET_EA_BAD_മൂല്യം_SIZE;

			എങ്കിൽ (entry->e_മൂല്യം_offs + entry->e_മൂല്യം_size > മൂല്യങ്ങൾ_size)
				തിരികെ EXT2_ET_EA_BAD_മൂല്യം_OFFSET;

			എങ്കിൽ (entry->e_മൂല്യം_size > 0 &&
			    മൂല്യം_start + entry->e_മൂല്യം_offs <
			    (അക്ഷരം *)end + വലുപ്പം(__u32))
				തിരികെ EXT2_ET_EA_BAD_മൂല്യം_OFFSET;

			remain -= entry->e_മൂല്യം_size;

			err = ext2fs_get_mem(entry->e_മൂല്യം_size, &x->മൂല്യം);
			എങ്കിൽ (err)
				തിരികെ err;
			memcpy(x->മൂല്യം, മൂല്യം_start + entry->e_മൂല്യം_offs,
			       entry->e_മൂല്യം_size);
		} അല്ലെങ്കിൽ {
			ഘടന ext2_inode *ea_inode;
			ext2_file_t ea_file;

			എങ്കിൽ (entry->e_മൂല്യം_offs != 0)
				തിരികെ EXT2_ET_EA_BAD_മൂല്യം_OFFSET;

			എങ്കിൽ (entry->e_മൂല്യം_size > (64 * 1024))
				തിരികെ EXT2_ET_EA_BAD_മൂല്യം_SIZE;

			err = ext2fs_get_mem(entry->e_മൂല്യം_size, &x->മൂല്യം);
			എങ്കിൽ (err)
				തിരികെ err;

			err = ext2fs_file_open(handle->fs, entry->e_മൂല്യം_inum,
					       0, &ea_file);
			എങ്കിൽ (err)
				തിരികെ err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			എങ്കിൽ ((ea_inode->i_flags & EXT4_INLINE_DATA_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_എണ്ണം == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			അല്ലെങ്കിൽ എങ്കിൽ ((__u64) ext2fs_file_get_size(ea_file) !=
				 entry->e_മൂല്യം_size)
				err = EXT2_ET_EA_BAD_മൂല്യം_SIZE;
			അല്ലെങ്കിൽ
				err = ext2fs_file_read(ea_file, x->മൂല്യം,
						       entry->e_മൂല്യം_size, 0);
			ext2fs_file_close(ea_file);
			എങ്കിൽ (err)
				തിരികെ err;
		}

		x->ea_ino = entry->e_മൂല്യം_inum;
		x->മൂല്യം_len = entry->e_മൂല്യം_size;

		/* e_hash may be 0 in older inode's ea */
		എങ്കിൽ (entry->e_hash != 0) {
			__u32 hash, signed_hash;

			ശൂന്യം *data = (entry->e_മൂല്യം_inum != 0) ?
					0 : മൂല്യം_start + entry->e_മൂല്യം_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, entry,
							  data, &hash,
							  &signed_hash);
			എങ്കിൽ (err)
				തിരികെ err;
			എങ്കിൽ ((entry->e_hash != hash) &&
			    (entry->e_hash != signed_hash)) {
				ഘടന ext2_inode child;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							entry->e_മൂല്യം_inum,
							&child);
				എങ്കിൽ (err)
					തിരികെ err;
				എങ്കിൽ (child.i_mtime != handle->ino ||
				    child.i_generation != inode->i_generation)
					തിരികെ EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->എണ്ണം++;
		entry = EXT2_EXT_ATTR_NEXT(entry);
	}

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം xattrs_free_താക്കോലുകൾ(ഘടന ext2_xattr_handle *h)
{
	ഘടന ext2_xattr *a = h->attrs;
	പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = 0; i < h->capacity; i++) {
		എങ്കിൽ (a[i].name)
			ext2fs_free_mem(&a[i].name);
		എങ്കിൽ (a[i].മൂല്യം)
			ext2fs_free_mem(&a[i].മൂല്യം);
	}
	h->എണ്ണം = 0;
	h->ibody_എണ്ണം = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(ഘടന ext2_xattr_handle *handle,
				   ഘടന ext2_inode_large *inode)
{
	ഘടന ext2_ext_attr_header *header;
	__u32 ea_inode_magic;
	unsigned പൂർണ്ണസംഖ്യ storage_size;
	അക്ഷരം *start, *block_buf = ശൂന്യ;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_താക്കോലുകൾ(handle);

	/* Does the inode have space for EA? */
	എങ്കിൽ (inode->i_extra_isize < വലുപ്പം(inode->i_extra_isize) ||
	    EXT2_INODE_SIZE(handle->fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  വലുപ്പം(__u32))
		പോകുക read_ea_block;
	എങ്കിൽ (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		പോകുക out;
	}

	/* Look for EA in the inode */
	memcpy(&ea_inode_magic, ((അക്ഷരം *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, വലുപ്പം(__u32));
	എങ്കിൽ (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_size = EXT2_INODE_SIZE(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_SIZE - inode->i_extra_isize -
			വലുപ്പം(__u32);
		start = ((അക്ഷരം *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + വലുപ്പം(__u32);

		err = read_xattrs_from_buffer(handle, inode,
					(ഘടന ext2_ext_attr_entry *) start,
					storage_size, start);
		എങ്കിൽ (err)
			പോകുക out;

		handle->ibody_എണ്ണം = handle->എണ്ണം;
	}

read_ea_block:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_block(handle->fs, EXT2_INODE(inode));
	എങ്കിൽ (blk != 0) {
		എങ്കിൽ ((blk < handle->fs->super->s_first_data_block) ||
		    (blk >= ext2fs_blocks_എണ്ണം(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_BLOCK_അക്കം;
			പോകുക out;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &block_buf);
		എങ്കിൽ (err)
			പോകുക out;

		err = ext2fs_read_ext_attr3(handle->fs, blk, block_buf,
					    handle->ino);
		എങ്കിൽ (err)
			പോകുക out3;

		/* We only know how to deal with v2 EA blocks */
		header = (ഘടന ext2_ext_attr_header *) block_buf;
		എങ്കിൽ (header->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_HEADER;
			പോകുക out3;
		}

		/* Read EAs */
		storage_size = handle->fs->blocksize -
			വലുപ്പം(ഘടന ext2_ext_attr_header);
		start = block_buf + വലുപ്പം(ഘടന ext2_ext_attr_header);
		err = read_xattrs_from_buffer(handle, inode,
					(ഘടന ext2_ext_attr_entry *) start,
					storage_size, block_buf);
	}

out3:
	എങ്കിൽ (block_buf)
		ext2fs_free_mem(&block_buf);
out:
	തിരികെ err;
}

errcode_t ext2fs_xattrs_read(ഘടന ext2_xattr_handle *handle)
{
	ഘടന ext2_inode_large *inode;
	size_t inode_size = EXT2_INODE_SIZE(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	എങ്കിൽ (inode_size < വലുപ്പം(*inode))
		inode_size = വലുപ്പം(*inode);
	err = ext2fs_get_memzero(inode_size, &inode);
	എങ്കിൽ (err)
		തിരികെ err;

	err = ext2fs_read_inode_full(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_SIZE(handle->fs->super));
	എങ്കിൽ (err)
		പോകുക out;

	err = ext2fs_xattrs_read_inode(handle, inode);

out:
	ext2fs_free_mem(&inode);

	തിരികെ err;
}

errcode_t ext2fs_xattrs_iterate(ഘടന ext2_xattr_handle *h,
				പൂർണ്ണസംഖ്യ (*func)(അക്ഷരം *name, അക്ഷരം *മൂല്യം,
					    size_t മൂല്യം_len, ശൂന്യം *data),
				ശൂന്യം *data)
{
	ഘടന ext2_xattr *x;
	പൂർണ്ണസംഖ്യ dirty = 0;
	പൂർണ്ണസംഖ്യ ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	വേണ്ടി (x = h->attrs; x < h->attrs + h->എണ്ണം; x++) {
		ret = func(x->name, x->മൂല്യം, x->മൂല്യം_len, data);
		എങ്കിൽ (ret & XATTR_CHANGED)
			dirty = 1;
		എങ്കിൽ (ret & XATTR_ABORT)
			നിർത്തുക;
	}

	എങ്കിൽ (dirty)
		തിരികെ ext2fs_xattrs_write(h);
	തിരികെ 0;
}

errcode_t ext2fs_xattr_get(ഘടന ext2_xattr_handle *h, സ്ഥിരം അക്ഷരം *താക്കോൽ,
			   ശൂന്യം **മൂല്യം, size_t *മൂല്യം_len)
{
	ഘടന ext2_xattr *x;
	അക്ഷരം *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	വേണ്ടി (x = h->attrs; x < h->attrs + h->എണ്ണം; x++) {
		എങ്കിൽ (strcmp(x->name, താക്കോൽ))
			തുടരുക;

		എങ്കിൽ (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(താക്കോൽ, "system.posix_acl_default") == 0) ||
		     (strcmp(താക്കോൽ, "system.posix_acl_access") == 0))) {
			err = convert_disk_buffer_to_posix_acl(x->മൂല്യം, x->മൂല്യം_len,
							       മൂല്യം, മൂല്യം_len);
			തിരികെ err;
		} അല്ലെങ്കിൽ {
			err = ext2fs_get_mem(x->മൂല്യം_len, &val);
			എങ്കിൽ (err)
				തിരികെ err;
			memcpy(val, x->മൂല്യം, x->മൂല്യം_len);
			*മൂല്യം = val;
			*മൂല്യം_len = x->മൂല്യം_len;
			തിരികെ 0;
		}
	}

	തിരികെ EXT2_ET_EA_താക്കോൽ_NOT_FOUND;
}

errcode_t ext2fs_xattr_inode_max_size(ext2_filsys fs, ext2_ino_t ino,
				      size_t *size)
{
	ഘടന ext2_ext_attr_entry *entry;
	ഘടന ext2_inode_large *inode;
	__u32 ea_inode_magic;
	unsigned പൂർണ്ണസംഖ്യ minoff;
	അക്ഷരം *start;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_SIZE(fs->super);
	എങ്കിൽ (i < വലുപ്പം(*inode))
		i = വലുപ്പം(*inode);
	err = ext2fs_get_memzero(i, &inode);
	എങ്കിൽ (err)
		തിരികെ err;

	err = ext2fs_read_inode_full(fs, ino, (ഘടന ext2_inode *)inode,
				     EXT2_INODE_SIZE(fs->super));
	എങ്കിൽ (err)
		പോകുക out;

	/* Does the inode have size for EA? */
	എങ്കിൽ (EXT2_INODE_SIZE(fs->super) <= EXT2_GOOD_OLD_INODE_SIZE +
						  inode->i_extra_isize +
						  വലുപ്പം(__u32)) {
		err = EXT2_ET_INLINE_DATA_NO_SPACE;
		പോകുക out;
	}

	minoff = EXT2_INODE_SIZE(fs->super) - വലുപ്പം(*inode) - വലുപ്പം(__u32);
	memcpy(&ea_inode_magic, ((അക്ഷരം *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
	       inode->i_extra_isize, വലുപ്പം(__u32));
	എങ്കിൽ (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		start= ((അക്ഷരം *) inode) + EXT2_GOOD_OLD_INODE_SIZE +
			inode->i_extra_isize + വലുപ്പം(__u32);
		entry = (ഘടന ext2_ext_attr_entry *) start;
		ആകുമ്പോൾ (!EXT2_EXT_IS_LAST_ENTRY(entry)) {
			എങ്കിൽ (!entry->e_മൂല്യം_inum && entry->e_മൂല്യം_size) {
				unsigned പൂർണ്ണസംഖ്യ offs = entry->e_മൂല്യം_offs;
				എങ്കിൽ (offs < minoff)
					minoff = offs;
			}
			entry = EXT2_EXT_ATTR_NEXT(entry);
		}
		*size = minoff - ((അക്ഷരം *)entry - (അക്ഷരം *)start) - വലുപ്പം(__u32);
	} അല്ലെങ്കിൽ {
		/* no xattr.  return a maximum size */
		*size = EXT2_EXT_ATTR_SIZE(minoff -
					   EXT2_EXT_ATTR_LEN(strlen("data")) -
					   EXT2_EXT_ATTR_ROUND - വലുപ്പം(__u32));
	}

out:
	ext2fs_free_mem(&inode);
	തിരികെ err;
}

സ്ഥിതം errcode_t xattr_create_ea_inode(ext2_filsys fs, സ്ഥിരം ശൂന്യം *മൂല്യം,
				       size_t മൂല്യം_len, ext2_ino_t *ea_ino)
{
	ഘടന ext2_inode inode;
	ext2_ino_t ino;
	ext2_file_t file;
	__u32 hash;
	errcode_t ret;

	ret = ext2fs_new_inode(fs, 0, 0, 0, &ino);
	എങ്കിൽ (ret)
		തിരികെ ret;

	memset(&inode, 0, വലുപ്പം(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	എങ്കിൽ (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_size = 0;
	inode.i_mode = LINUX_S_IFREG | 0600;
	inode.i_links_എണ്ണം = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	എങ്കിൽ (ret)
		തിരികെ ret;
	/*
	 * ref_count and hash utilize inode's i_*time fields.
	 * ext2fs_write_new_inode() call above initializes these fields with
	 * current time. That's why ref count and hash updates are done
	 * separately below.
	 */
	ext2fs_set_ea_inode_ref(&inode, 1);
	hash = ext2fs_crc32c_le(fs->csum_seed, മൂല്യം, മൂല്യം_len);
	ext2fs_set_ea_inode_hash(&inode, hash);

	ret = ext2fs_write_inode(fs, ino, &inode);
	എങ്കിൽ (ret)
		തിരികെ ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	എങ്കിൽ (ret)
		തിരികെ ret;
	ret = ext2fs_file_write(file, മൂല്യം, മൂല്യം_len, ശൂന്യ);
	ext2fs_file_close(file);
	എങ്കിൽ (ret)
		തിരികെ ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	തിരികെ 0;
}

സ്ഥിതം errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	ഘടന ext2_inode_large inode;
	__u64 ref_എണ്ണം;
	errcode_t ret;

	ret = ext2fs_read_inode_full(fs, ino, (ഘടന ext2_inode *)&inode,
				     വലുപ്പം(inode));
	എങ്കിൽ (ret)
		പോകുക out;

	ref_എണ്ണം = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_എണ്ണം--;
	ext2fs_set_ea_inode_ref(EXT2_INODE(&inode), ref_എണ്ണം);

	എങ്കിൽ (ref_എണ്ണം)
		പോകുക write_out;

	inode.i_links_എണ്ണം = 0;
	ext2fs_set_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	എങ്കിൽ (ret)
		പോകുക write_out;

	എങ്കിൽ (ext2fs_inode_has_valid_blocks2(fs, (ഘടന ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (ഘടന ext2_inode *)&inode, ശൂന്യ,
				   0, ~0ULL);
		എങ്കിൽ (ret)
			പോകുക out;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_out:
	ret = ext2fs_write_inode_full(fs, ino, (ഘടന ext2_inode *)&inode,
				      വലുപ്പം(inode));
out:
	തിരികെ ret;
}

സ്ഥിതം errcode_t xattr_update_entry(ext2_filsys fs, ഘടന ext2_xattr *x,
				    സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *short_name,
				    പൂർണ്ണസംഖ്യ index, സ്ഥിരം ശൂന്യം *മൂല്യം,
				    size_t മൂല്യം_len, പൂർണ്ണസംഖ്യ in_inode)
{
	ext2_ino_t ea_ino = 0;
	ശൂന്യം *new_മൂല്യം = ശൂന്യ;
	അക്ഷരം *new_name = ശൂന്യ;
	പൂർണ്ണസംഖ്യ name_len;
	errcode_t ret;

	എങ്കിൽ (!x->name) {
		name_len = strlen(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		എങ്കിൽ (ret)
			പോകുക fail;
		memcpy(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(മൂല്യം_len, &new_മൂല്യം);
	എങ്കിൽ (ret)
		പോകുക fail;
	memcpy(new_മൂല്യം, മൂല്യം, മൂല്യം_len);

	എങ്കിൽ (in_inode) {
		ret = xattr_create_ea_inode(fs, മൂല്യം, മൂല്യം_len, &ea_ino);
		എങ്കിൽ (ret)
			പോകുക fail;
	}

	എങ്കിൽ (x->ea_ino) {
		ret = xattr_inode_dec_ref(fs, x->ea_ino);
		എങ്കിൽ (ret)
			പോകുക fail;
	}

	എങ്കിൽ (!x->name) {
		x->name = new_name;
		x->short_name = new_name + (short_name  - name);
	}
	x->name_index = index;

	എങ്കിൽ (x->മൂല്യം)
		ext2fs_free_mem(&x->മൂല്യം);
	x->മൂല്യം = new_മൂല്യം;
	x->മൂല്യം_len = മൂല്യം_len;
	x->ea_ino = ea_ino;
	തിരികെ 0;
fail:
	എങ്കിൽ (new_name)
		ext2fs_free_mem(&new_name);
	എങ്കിൽ (new_മൂല്യം)
		ext2fs_free_mem(&new_മൂല്യം);
	എങ്കിൽ (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ xattr_find_position(ഘടന ext2_xattr *attrs, പൂർണ്ണസംഖ്യ എണ്ണം,
			       സ്ഥിരം അക്ഷരം *shortname, പൂർണ്ണസംഖ്യ name_idx)
{
	ഘടന ext2_xattr *x;
	പൂർണ്ണസംഖ്യ i;
	പൂർണ്ണസംഖ്യ shortname_len, x_shortname_len;

	shortname_len = strlen(shortname);

	വേണ്ടി (i = 0, x = attrs; i < എണ്ണം; i++, x++) {
		എങ്കിൽ (name_idx < x->name_index)
			നിർത്തുക;
		എങ്കിൽ (name_idx > x->name_index)
			തുടരുക;

		x_shortname_len = strlen(x->short_name);
		എങ്കിൽ (shortname_len < x_shortname_len)
			നിർത്തുക;
		എങ്കിൽ (shortname_len > x_shortname_len)
			തുടരുക;

		എങ്കിൽ (memcmp(shortname, x->short_name, shortname_len) <= 0)
			നിർത്തുക;
	}
	തിരികെ i;
}

സ്ഥിതം errcode_t xattr_array_update(ഘടന ext2_xattr_handle *h,
				    സ്ഥിരം അക്ഷരം *name,
				    സ്ഥിരം ശൂന്യം *മൂല്യം, size_t മൂല്യം_len,
				    പൂർണ്ണസംഖ്യ ibody_free, പൂർണ്ണസംഖ്യ block_free,
				    പൂർണ്ണസംഖ്യ old_idx, പൂർണ്ണസംഖ്യ in_inode)
{
	ഘടന ext2_xattr tmp;
	പൂർണ്ണസംഖ്യ add_to_ibody;
	പൂർണ്ണസംഖ്യ needed;
	പൂർണ്ണസംഖ്യ name_len, name_idx = 0;
	സ്ഥിരം അക്ഷരം *shortname = name;
	പൂർണ്ണസംഖ്യ new_idx;
	പൂർണ്ണസംഖ്യ ret;

	find_ea_index(name, &shortname, &name_idx);
	name_len = strlen(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	എങ്കിൽ (!in_inode)
		needed += EXT2_EXT_ATTR_SIZE(മൂല്യം_len);

	എങ്കിൽ (old_idx >= 0 && old_idx < h->ibody_എണ്ണം) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		എങ്കിൽ (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_SIZE(
						h->attrs[old_idx].മൂല്യം_len);
	}

	എങ്കിൽ (needed <= ibody_free) {
		എങ്കിൽ (old_idx < 0) {
			new_idx = h->ibody_എണ്ണം;
			add_to_ibody = 1;
			പോകുക add_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, മൂല്യം,
					 മൂല്യം_len, in_inode);
		എങ്കിൽ (ret)
			തിരികെ ret;
		എങ്കിൽ (h->ibody_എണ്ണം <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_എണ്ണം + 1,
				h->attrs + h->ibody_എണ്ണം,
				(old_idx - h->ibody_എണ്ണം) * വലുപ്പം(*h->attrs));
			h->attrs[h->ibody_എണ്ണം] = tmp;
			h->ibody_എണ്ണം++;
		}
		തിരികെ 0;
	}

	എങ്കിൽ (h->ibody_എണ്ണം <= old_idx) {
		block_free += EXT2_EXT_ATTR_LEN(name_len);
		എങ്കിൽ (!h->attrs[old_idx].ea_ino)
			block_free +=
				EXT2_EXT_ATTR_SIZE(h->attrs[old_idx].മൂല്യം_len);
	}

	എങ്കിൽ (needed > block_free)
		തിരികെ EXT2_ET_EA_NO_SPACE;

	എങ്കിൽ (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_entry(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, മൂല്യം,
					 മൂല്യം_len, in_inode);
		എങ്കിൽ (ret)
			തിരികെ ret;
		എങ്കിൽ (old_idx < h->ibody_എണ്ണം) {
			/*
			 * Move entry from ibody to the block. Note that
			 * entries in the block are sorted.
			 */
			new_idx = xattr_find_position(h->attrs + h->ibody_എണ്ണം,
						      h->എണ്ണം - h->ibody_എണ്ണം,
						      shortname, name_idx);
			new_idx += h->ibody_എണ്ണം - 1;
			tmp = h->attrs[old_idx];
			memmove(h->attrs + old_idx, h->attrs + old_idx + 1,
				(new_idx - old_idx) * വലുപ്പം(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_എണ്ണം--;
		}
		തിരികെ 0;
	}

	new_idx = xattr_find_position(h->attrs + h->ibody_എണ്ണം,
				      h->എണ്ണം - h->ibody_എണ്ണം,
				      shortname, name_idx);
	new_idx += h->ibody_എണ്ണം;
	add_to_ibody = 0;

add_new:
	എങ്കിൽ (h->എണ്ണം == h->capacity) {
		ret = ext2fs_xattrs_expand(h, 4);
		എങ്കിൽ (ret)
			തിരികെ ret;
	}

	ret = xattr_update_entry(h->fs, &h->attrs[h->എണ്ണം], name, shortname,
				 name_idx, മൂല്യം, മൂല്യം_len, in_inode);
	എങ്കിൽ (ret)
		തിരികെ ret;

	tmp = h->attrs[h->എണ്ണം];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->എണ്ണം - new_idx)*വലുപ്പം(*h->attrs));
	h->attrs[new_idx] = tmp;
	എങ്കിൽ (add_to_ibody)
		h->ibody_എണ്ണം++;
	h->എണ്ണം++;
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ space_used(ഘടന ext2_xattr *attrs, പൂർണ്ണസംഖ്യ എണ്ണം)
{
	പൂർണ്ണസംഖ്യ total = 0;
	ഘടന ext2_xattr *x;
	പൂർണ്ണസംഖ്യ i, len;

	വേണ്ടി (i = 0, x = attrs; i < എണ്ണം; i++, x++) {
		len = strlen(x->short_name);
		total += EXT2_EXT_ATTR_LEN(len);
		എങ്കിൽ (!x->ea_ino)
			total += EXT2_EXT_ATTR_SIZE(x->മൂല്യം_len);
	}
	തിരികെ total;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#define EXT4_XATTR_MIN_LARGE_EA_SIZE(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - വലുപ്പം(ഘടന ext2_ext_attr_header) - 4)

errcode_t ext2fs_xattr_set(ഘടന ext2_xattr_handle *h,
			   സ്ഥിരം അക്ഷരം *name,
			   സ്ഥിരം ശൂന്യം *മൂല്യം,
			   size_t മൂല്യം_len)
{
	ext2_filsys fs = h->fs;
	സ്ഥിരം പൂർണ്ണസംഖ്യ inode_size = EXT2_INODE_SIZE(fs->super);
	ഘടന ext2_inode_large *inode = ശൂന്യ;
	ഘടന ext2_xattr *x;
	അക്ഷരം *new_മൂല്യം;
	പൂർണ്ണസംഖ്യ ibody_free, block_free;
	പൂർണ്ണസംഖ്യ in_inode = 0;
	പൂർണ്ണസംഖ്യ old_idx = -1;
	പൂർണ്ണസംഖ്യ extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(മൂല്യം_len, &new_മൂല്യം);
	എങ്കിൽ (ret)
		തിരികെ ret;
	എങ്കിൽ (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((strcmp(name, "system.posix_acl_default") == 0) ||
	     (strcmp(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_to_disk_buffer(മൂല്യം, മൂല്യം_len,
						       new_മൂല്യം, &മൂല്യം_len);
		എങ്കിൽ (ret)
			പോകുക out;
	} അല്ലെങ്കിൽ എങ്കിൽ (മൂല്യം_len)
		memcpy(new_മൂല്യം, മൂല്യം, മൂല്യം_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	വേണ്ടി (x = h->attrs; x < h->attrs + h->എണ്ണം; x++) {
		എങ്കിൽ (!strcmp(x->name, name)) {
			എങ്കിൽ (!x->ea_ino && x->മൂല്യം_len == മൂല്യം_len &&
			    (!മൂല്യം_len ||
			     !memcmp(x->മൂല്യം, new_മൂല്യം, മൂല്യം_len))) {
				ret = 0;
				പോകുക out;
			}
			old_idx = x - h->attrs;
			നിർത്തുക;
		}
	}

	ret = ext2fs_get_memzero(inode_size, &inode);
	എങ്കിൽ (ret)
		പോകുക out;
	ret = ext2fs_read_inode_full(fs, h->ino,
				     (ഘടന ext2_inode *)inode,
				     inode_size);
	എങ്കിൽ (ret)
		പോകുക out;
	എങ്കിൽ (inode_size > EXT2_GOOD_OLD_INODE_SIZE) {
		extra_isize = inode->i_extra_isize;
		എങ്കിൽ (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			എങ്കിൽ (extra_isize == 0)
				extra_isize = വലുപ്പം(__u32);
		}
		ibody_free = inode_size - EXT2_GOOD_OLD_INODE_SIZE;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= വലുപ്പം(__u32) * 2;
		ibody_free -= space_used(h->attrs, h->ibody_എണ്ണം);
	} അല്ലെങ്കിൽ
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	എങ്കിൽ (strcmp(name, "system.data") == 0) {
		എങ്കിൽ (h->ibody_എണ്ണം <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			പോകുക out;
		}
		ret = xattr_array_update(h, name, new_മൂല്യം, മൂല്യം_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		എങ്കിൽ (ret)
			പോകുക out;
		പോകുക write_out;
	}

	block_free = fs->blocksize;
	block_free -= വലുപ്പം(ഘടന ext2_ext_attr_header);
	/* Final null entry. */
	block_free -= വലുപ്പം(__u32);
	block_free -= space_used(h->attrs + h->ibody_എണ്ണം,
				 h->എണ്ണം - h->ibody_എണ്ണം);

	എങ്കിൽ (ext2fs_has_feature_ea_inode(fs->super) &&
	    മൂല്യം_len > EXT4_XATTR_MIN_LARGE_EA_SIZE(fs->blocksize))
		in_inode = 1;

	ret = xattr_array_update(h, name, new_മൂല്യം, മൂല്യം_len, ibody_free,
				 block_free, old_idx, in_inode);
	എങ്കിൽ (ret == EXT2_ET_EA_NO_SPACE && !in_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_മൂല്യം, മൂല്യം_len,
			ibody_free, block_free, old_idx, 1 /* in_inode */);
	എങ്കിൽ (ret)
		പോകുക out;

write_out:
	ret = ext2fs_xattrs_write(h);
out:
	എങ്കിൽ (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_മൂല്യം);
	തിരികെ ret;
}

errcode_t ext2fs_xattr_remove(ഘടന ext2_xattr_handle *handle,
			      സ്ഥിരം അക്ഷരം *താക്കോൽ)
{
	ഘടന ext2_xattr *x;
	ഘടന ext2_xattr *end = handle->attrs + handle->എണ്ണം;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	വേണ്ടി (x = handle->attrs; x < end; x++) {
		എങ്കിൽ (strcmp(x->name, താക്കോൽ) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->മൂല്യം);
			എങ്കിൽ (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (end - x - 1)*വലുപ്പം(*x));
			memset(end - 1, 0, വലുപ്പം(*end));
			എങ്കിൽ (x < handle->attrs + handle->ibody_എണ്ണം)
				handle->ibody_എണ്ണം--;
			handle->എണ്ണം--;
			തിരികെ ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	തിരികെ 0;
}

errcode_t ext2fs_xattr_remove_all(ഘടന ext2_xattr_handle *handle)
{
	ഘടന ext2_xattr *x;
	ഘടന ext2_xattr *end = handle->attrs + handle->എണ്ണം;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	വേണ്ടി (x = handle->attrs; x < end; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->മൂല്യം);
		എങ്കിൽ (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_എണ്ണം = 0;
	handle->എണ്ണം = 0;
	തിരികെ ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     ഘടന ext2_xattr_handle **handle)
{
	ഘടന ext2_xattr_handle *h;
	errcode_t err;

	എങ്കിൽ (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_data(fs->super))
		തിരികെ EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(വലുപ്പം(*h), &h);
	എങ്കിൽ (err)
		തിരികെ err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->capacity = 4;
	err = ext2fs_get_arrayzero(h->capacity, വലുപ്പം(ഘടന ext2_xattr),
				   &h->attrs);
	എങ്കിൽ (err) {
		ext2fs_free_mem(&h);
		തിരികെ err;
	}
	h->എണ്ണം = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	തിരികെ 0;
}

errcode_t ext2fs_xattrs_close(ഘടന ext2_xattr_handle **handle)
{
	ഘടന ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_താക്കോലുകൾ(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	തിരികെ 0;
}

errcode_t ext2fs_xattrs_എണ്ണം(ഘടന ext2_xattr_handle *handle, size_t *എണ്ണം)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*എണ്ണം = handle->എണ്ണം;
	തിരികെ 0;
}

errcode_t ext2fs_xattrs_flags(ഘടന ext2_xattr_handle *handle,
			      unsigned പൂർണ്ണസംഖ്യ *new_flags, unsigned പൂർണ്ണസംഖ്യ *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	എങ്കിൽ (old_flags)
		*old_flags = handle->flags;
	എങ്കിൽ (new_flags)
		handle->flags = *new_flags;
	തിരികെ 0;
}
