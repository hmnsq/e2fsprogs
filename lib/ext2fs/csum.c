/*
 * csum.c --- checksumming of ext3 structures
 *
 * Copyright (C) 2006 Cluster File Systems, Inc.
 * Copyright (C) 2006, 2007 by Andreas Dilger <adilger@clusterfs.com>
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#અન્તર્ભાવ "config.h"
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"
#અન્તર્ભાવ "crc16.h"
#અન્તર્ભાવ <assert.h>

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifdef DEBUG
#define STATIC
#નહીંતર
#define STATIC સ્થિર
#endif

શૂન્ય ext2fs_init_csum_seed(ext2_filsys fs)
{
	જો (ext2fs_has_feature_csum_seed(fs->super))
		fs->csum_seed = fs->super->s_checksum_seed;
	નહીંતર જો (ext2fs_has_feature_metadata_csum(fs->super) ||
		 ext2fs_has_feature_ea_inode(fs->super))
		fs->csum_seed = ext2fs_crc32c_le(~0, fs->super->s_uuid,
						 કદ(fs->super->s_uuid));
}

સ્થિર __u32 ext2fs_mmp_csum(ext2_filsys fs, સંરચના mmp_struct *mmp)
{
	પૂર્ણાંક offset = offsetof(સંરચના mmp_struct, mmp_checksum);

	પાછા ext2fs_crc32c_le(fs->csum_seed, (unsigned અક્ષર *)mmp, offset);
}

પૂર્ણાંક ext2fs_mmp_csum_verify(ext2_filsys fs, સંરચના mmp_struct *mmp)
{
	__u32 calculated;

	જો (!ext2fs_has_feature_metadata_csum(fs->super))
		પાછા 1;

	calculated = ext2fs_mmp_csum(fs, mmp);

	પાછા ext2fs_le32_to_cpu(mmp->mmp_checksum) == calculated;
}

errcode_t ext2fs_mmp_csum_set(ext2_filsys fs, સંરચના mmp_struct *mmp)
{
	__u32 crc;

	જો (!ext2fs_has_feature_metadata_csum(fs->super))
		પાછા 0;

	crc = ext2fs_mmp_csum(fs, mmp);
	mmp->mmp_checksum = ext2fs_cpu_to_le32(crc);

	પાછા 0;
}

પૂર્ણાંક ext2fs_verify_csum_type(ext2_filsys fs, સંરચના ext2_super_block *sb)
{
	જો (!ext2fs_has_feature_metadata_csum(fs->super))
		પાછા 1;

	પાછા sb->s_checksum_type == EXT2_CRC32C_CHKSUM;
}

સ્થિર __u32 ext2fs_superblock_csum(ext2_filsys fs EXT2FS_ATTR((unused)),
				    સંરચના ext2_super_block *sb)
{
	પૂર્ણાંક offset = offsetof(સંરચના ext2_super_block, s_checksum);

	પાછા ext2fs_crc32c_le(~0, (unsigned અક્ષર *)sb, offset);
}

/* NOTE: The input to this function MUST be in LE order */
પૂર્ણાંક ext2fs_superblock_csum_verify(ext2_filsys fs, સંરચના ext2_super_block *sb)
{
	__u32 flag, calculated;

	જો (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	નહીંતર
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	જો (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		પાછા 1;

	calculated = ext2fs_superblock_csum(fs, sb);

	પાછા ext2fs_le32_to_cpu(sb->s_checksum) == calculated;
}

/* NOTE: The input to this function MUST be in LE order */
errcode_t ext2fs_superblock_csum_set(ext2_filsys fs,
				     સંરચના ext2_super_block *sb)
{
	__u32 flag, crc;

	જો (fs->flags & EXT2_FLAG_SWAP_BYTES)
		flag = EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;
	નહીંતર
		flag = ext2fs_cpu_to_le32(EXT4_FEATURE_RO_COMPAT_METADATA_CSUM);

	જો (!EXT2_HAS_RO_COMPAT_FEATURE(fs->super, flag))
		પાછા 0;

	crc = ext2fs_superblock_csum(fs, sb);
	sb->s_checksum = ext2fs_cpu_to_le32(crc);

	પાછા 0;
}

સ્થિર errcode_t ext2fs_ext_attr_block_csum(ext2_filsys fs,
					    ext2_ino_t inum EXT2FS_ATTR((unused)),
					    blk64_t block,
					    સંરચના ext2_ext_attr_header *hdr,
					    __u32 *crc)
{
	અક્ષર *buf = (અક્ષર *)hdr;
	__u32 old_crc = hdr->h_checksum;

	hdr->h_checksum = 0;
	block = ext2fs_cpu_to_le64(block);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned અક્ષર *)&block,
				કદ(block));
	*crc = ext2fs_crc32c_le(*crc, (unsigned અક્ષર *)buf, fs->blocksize);
	hdr->h_checksum = old_crc;

	પાછા 0;
}

પૂર્ણાંક ext2fs_ext_attr_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				      blk64_t block,
				      સંરચના ext2_ext_attr_header *hdr)
{
	__u32 calculated;
	errcode_t retval;

	જો (!ext2fs_has_feature_metadata_csum(fs->super))
		પાછા 1;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &calculated);
	જો (retval)
		પાછા 0;

	પાછા ext2fs_le32_to_cpu(hdr->h_checksum) == calculated;
}

errcode_t ext2fs_ext_attr_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
					 blk64_t block,
					 સંરચના ext2_ext_attr_header *hdr)
{
	errcode_t retval;
	__u32 crc;

	જો (!ext2fs_has_feature_metadata_csum(fs->super))
		પાછા 0;

	retval = ext2fs_ext_attr_block_csum(fs, inum, block, hdr, &crc);
	જો (retval)
		પાછા retval;
	hdr->h_checksum = ext2fs_cpu_to_le32(crc);
	પાછા 0;
}

સ્થિર __u16 do_nothing16(__u16 x)
{
	પાછા x;
}

સ્થિર __u16 disk_to_host16(__u16 x)
{
	પાછા ext2fs_le16_to_cpu(x);
}

સ્થિર errcode_t __get_dx_countlimit(ext2_filsys fs,
				     સંરચના ext2_dir_entry *dirent,
				     સંરચના ext2_dx_countlimit **cc,
				     પૂર્ણાંક *offset,
				     પૂર્ણાંક need_swab)
{
	સંરચના ext2_dir_entry *dp;
	સંરચના ext2_dx_મૂળ_info *મૂળ;
	સંરચના ext2_dx_countlimit *c;
	પૂર્ણાંક ગણતરી_offset, max_sane_entries;
	unsigned પૂર્ણાંક rec_len;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	rec_len = translate(dirent->rec_len);

	જો (rec_len == fs->blocksize && translate(dirent->name_len) == 0)
		ગણતરી_offset = 8;
	નહીંતર જો (rec_len == 12) {
		dp = (સંરચના ext2_dir_entry *)(((અક્ષર *)dirent) + rec_len);
		rec_len = translate(dp->rec_len);
		જો (rec_len != fs->blocksize - 12)
			પાછા EXT2_ET_DB_NOT_FOUND;
		મૂળ = (સંરચના ext2_dx_મૂળ_info *)(((અક્ષર *)dp + 12));
		જો (મૂળ->reserved_zero ||
		    મૂળ->info_length != કદ(સંરચના ext2_dx_મૂળ_info))
			પાછા EXT2_ET_DB_NOT_FOUND;
		ગણતરી_offset = 32;
	} નહીંતર
		પાછા EXT2_ET_DB_NOT_FOUND;

	c = (સંરચના ext2_dx_countlimit *)(((અક્ષર *)dirent) + ગણતરી_offset);
	max_sane_entries = (fs->blocksize - ગણતરી_offset) /
			   કદ(સંરચના ext2_dx_entry);
	જો (ext2fs_le16_to_cpu(c->limit) > max_sane_entries ||
	    ext2fs_le16_to_cpu(c->ગણતરી) > max_sane_entries)
		પાછા EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	જો (offset)
		*offset = ગણતરી_offset;
	જો (cc)
		*cc = c;

	પાછા 0;
}

errcode_t ext2fs_get_dx_countlimit(ext2_filsys fs,
				   સંરચના ext2_dir_entry *dirent,
				   સંરચના ext2_dx_countlimit **cc,
				   પૂર્ણાંક *offset)
{
	પાછા __get_dx_countlimit(fs, dirent, cc, offset, 0);
}

શૂન્ય ext2fs_initialize_dirent_tail(ext2_filsys fs,
				   સંરચના ext2_dir_entry_tail *t)
{
	memset(t, 0, કદ(સંરચના ext2_dir_entry_tail));
	ext2fs_set_rec_len(fs, કદ(સંરચના ext2_dir_entry_tail),
			   (સંરચના ext2_dir_entry *)t);
	t->det_reserved_name_len = EXT2_DIR_NAME_LEN_CSUM;
}

સ્થિર errcode_t __get_dirent_tail(ext2_filsys fs,
				   સંરચના ext2_dir_entry *dirent,
				   સંરચના ext2_dir_entry_tail **tt,
				   પૂર્ણાંક need_swab)
{
	સંરચના ext2_dir_entry *d;
	શૂન્ય *ટોચ;
	સંરચના ext2_dir_entry_tail *t;
	unsigned પૂર્ણાંક rec_len;
	errcode_t retval = 0;
	__u16 (*translate)(__u16) = (need_swab ? disk_to_host16 : do_nothing16);

	જો (fs->blocksize < 1024)
		પાછા EXT2_FILSYS_CORRUPTED; /* Should never happen */

	d = dirent;
	ટોચ = EXT2_DIRENT_TAIL(dirent, fs->blocksize);

	જ્યારે ((શૂન્ય *) d < ટોચ) {
		rec_len = translate(d->rec_len);
		જો ((rec_len < 8) || (rec_len & 0x03))
			પાછા EXT2_ET_DIR_CORRUPTED;
		d = (સંરચના ext2_dir_entry *)(((અક્ષર *)d) + rec_len);
	}

	જો ((અક્ષર *)d > ((અક્ષર *)dirent + fs->blocksize))
			પાછા EXT2_ET_DIR_CORRUPTED;
	જો (d != ટોચ)
		પાછા EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	t = (સંરચના ext2_dir_entry_tail *)d;
	જો (t->det_reserved_zero1 ||
	    translate(t->det_rec_len) != કદ(સંરચના ext2_dir_entry_tail) ||
	    translate(t->det_reserved_name_len) != EXT2_DIR_NAME_LEN_CSUM)
		પાછા EXT2_ET_DIR_NO_SPACE_FOR_CSUM;

	જો (tt)
		*tt = t;
	પાછા retval;
}

પૂર્ણાંક ext2fs_dirent_has_tail(ext2_filsys fs, સંરચના ext2_dir_entry *dirent)
{
	પાછા __get_dirent_tail(fs, dirent, ખાલી, 0) !=
		EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

સ્થિર errcode_t ext2fs_dirent_csum(ext2_filsys fs, ext2_ino_t inum,
				    સંરચના ext2_dir_entry *dirent, __u32 *crc,
				    પૂર્ણાંક size)
{
	errcode_t retval;
	અક્ષર *buf = (અક્ષર *)dirent;
	__u32 gen;
	સંરચના ext2_inode inode;

	retval = ext2fs_read_inode(fs, inum, &inode);
	જો (retval)
		પાછા retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned અક્ષર *)&inum,
				કદ(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned અક્ષર *)&gen, કદ(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned અક્ષર *)buf, size);

	પાછા 0;
}

પૂર્ણાંક ext2fs_dirent_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			      સંરચના ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 calculated;
	સંરચના ext2_dir_entry_tail *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	જો (retval)
		પાછા 1;

	/*
	 * The checksum field is overlaid with the dirent->name field
	 * so the swapfs.c functions won't change the endianness.
	 */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &calculated,
				    (અક્ષર *)t - (અક્ષર *)dirent);
	જો (retval)
		પાછા 0;
	પાછા ext2fs_le32_to_cpu(t->det_checksum) == calculated;
}

સ્થિર errcode_t ext2fs_dirent_csum_set(ext2_filsys fs, ext2_ino_t inum,
					સંરચના ext2_dir_entry *dirent)
{
	errcode_t retval;
	__u32 crc;
	સંરચના ext2_dir_entry_tail *t;

	retval = __get_dirent_tail(fs, dirent, &t, 1);
	જો (retval)
		પાછા retval;

	/* swapfs.c functions don't change the checksum endianness */
	retval = ext2fs_dirent_csum(fs, inum, dirent, &crc,
				    (અક્ષર *)t - (અક્ષર *)dirent);
	જો (retval)
		પાછા retval;
	t->det_checksum = ext2fs_cpu_to_le32(crc);
	પાછા 0;
}

errcode_t ext2fs_dx_csum(ext2_filsys fs, ext2_ino_t inum,
			 સંરચના ext2_dir_entry *dirent,
			 __u32 *crc, સંરચના ext2_dx_tail **ret_t)
{
	errcode_t retval;
	અક્ષર *buf = (અક્ષર *)dirent;
	પૂર્ણાંક size;
	__u32 gen, dummy_csum = 0;
	સંરચના ext2_inode inode;
	સંરચના ext2_dx_tail *t;
	સંરચના ext2_dx_countlimit *c;
	પૂર્ણાંક ગણતરી_offset, limit, ગણતરી;

	retval = __get_dx_countlimit(fs, dirent, &c, &ગણતરી_offset, 1);
	જો (retval)
		પાછા retval;
	limit = ext2fs_le16_to_cpu(c->limit);
	ગણતરી = ext2fs_le16_to_cpu(c->ગણતરી);
	જો (ગણતરી_offset + (limit * કદ(સંરચના ext2_dx_entry)) >
	    fs->blocksize - કદ(સંરચના ext2_dx_tail))
		પાછા EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
	/* htree structs are accessed in LE order */
	t = (સંરચના ext2_dx_tail *)(((સંરચના ext2_dx_entry *)c) + limit);

	size = ગણતરી_offset + (ગણતરી * કદ(સંરચના ext2_dx_entry));

	retval = ext2fs_read_inode(fs, inum, &inode);
	જો (retval)
		પાછા retval;

	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned અક્ષર *)&inum,
				કદ(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned અક્ષર *)&gen, કદ(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned અક્ષર *)buf, size);
	*crc = ext2fs_crc32c_le(*crc, (unsigned અક્ષર *)t, 4);
	*crc = ext2fs_crc32c_le(*crc, (unsigned અક્ષર *)&dummy_csum, 4);

	જો (ret_t)
		*ret_t = t;
	પાછા 0;
}

સ્થિર પૂર્ણાંક ext2fs_dx_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 સંરચના ext2_dir_entry *dirent)
{
	__u32 calculated;
	errcode_t retval;
	સંરચના ext2_dx_tail *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &calculated, &t);
	જો (retval)
		પાછા 0;

	પાછા ext2fs_le32_to_cpu(t->dt_checksum) == calculated;
}

સ્થિર errcode_t ext2fs_dx_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    સંરચના ext2_dir_entry *dirent)
{
	__u32 crc;
	errcode_t retval = 0;
	સંરચના ext2_dx_tail *t;

	retval = ext2fs_dx_csum(fs, inum, dirent, &crc, &t);
	જો (retval)
		પાછા retval;
	t->dt_checksum = ext2fs_cpu_to_le32(crc);
	પાછા retval;
}

પૂર્ણાંક ext2fs_dir_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				 સંરચના ext2_dir_entry *dirent)
{
	જો (!ext2fs_has_feature_metadata_csum(fs->super))
		પાછા 1;

	જો (__get_dirent_tail(fs, dirent, ખાલી, 1) == 0)
		પાછા ext2fs_dirent_csum_verify(fs, inum, dirent);
	જો (__get_dx_countlimit(fs, dirent, ખાલી, ખાલી, 1) == 0)
		પાછા ext2fs_dx_csum_verify(fs, inum, dirent);

	પાછા 0;
}

errcode_t ext2fs_dir_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				    સંરચના ext2_dir_entry *dirent)
{
	જો (!ext2fs_has_feature_metadata_csum(fs->super))
		પાછા 0;

	જો (__get_dirent_tail(fs, dirent, ખાલી, 1) == 0)
		પાછા ext2fs_dirent_csum_set(fs, inum, dirent);
	જો (__get_dx_countlimit(fs, dirent, ખાલી, ખાલી, 1) == 0)
		પાછા ext2fs_dx_csum_set(fs, inum, dirent);

	જો (fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS)
		પાછા 0;
	પાછા EXT2_ET_DIR_NO_SPACE_FOR_CSUM;
}

#define EXT3_EXTENT_TAIL_OFFSET(hdr)	(કદ(સંરચના ext3_extent_header) + \
	(કદ(સંરચના ext3_extent) * ext2fs_le16_to_cpu((hdr)->eh_max)))

સ્થિર સંરચના ext3_extent_tail *get_extent_tail(સંરચના ext3_extent_header *h)
{
	પાછા (સંરચના ext3_extent_tail *)(((અક્ષર *)h) +
					   EXT3_EXTENT_TAIL_OFFSET(h));
}

સ્થિર errcode_t ext2fs_extent_block_csum(ext2_filsys fs, ext2_ino_t inum,
					  સંરચના ext3_extent_header *eh,
					  __u32 *crc)
{
	પૂર્ણાંક size;
	__u32 gen;
	errcode_t retval;
	સંરચના ext2_inode inode;

	size = EXT3_EXTENT_TAIL_OFFSET(eh) + offsetof(સંરચના ext3_extent_tail,
						      et_checksum);

	retval = ext2fs_read_inode(fs, inum, &inode);
	જો (retval)
		પાછા retval;
	inum = ext2fs_cpu_to_le32(inum);
	gen = ext2fs_cpu_to_le32(inode.i_generation);
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned અક્ષર *)&inum,
				કદ(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned અક્ષર *)&gen, કદ(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned અક્ષર *)eh, size);

	પાછા 0;
}

પૂર્ણાંક ext2fs_extent_block_csum_verify(ext2_filsys fs, ext2_ino_t inum,
				    સંરચના ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 provided, calculated;
	સંરચના ext3_extent_tail *t = get_extent_tail(eh);

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	જો (!ext2fs_has_feature_metadata_csum(fs->super))
		પાછા 1;

	provided = ext2fs_le32_to_cpu(t->et_checksum);
	retval = ext2fs_extent_block_csum(fs, inum, eh, &calculated);
	જો (retval)
		પાછા 0;

	પાછા provided == calculated;
}

errcode_t ext2fs_extent_block_csum_set(ext2_filsys fs, ext2_ino_t inum,
				       સંરચના ext3_extent_header *eh)
{
	errcode_t retval;
	__u32 crc;
	સંરચના ext3_extent_tail *t = get_extent_tail(eh);

	જો (!ext2fs_has_feature_metadata_csum(fs->super))
		પાછા 0;

	/*
	 * The extent tree structures are accessed in LE order, so we must
	 * swap the checksum bytes here.
	 */
	retval = ext2fs_extent_block_csum(fs, inum, eh, &crc);
	જો (retval)
		પાછા retval;
	t->et_checksum = ext2fs_cpu_to_le32(crc);
	પાછા retval;
}

પૂર્ણાંક ext2fs_inode_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    અક્ષર *bitmap, પૂર્ણાંક size)
{
	સંરચના ext4_group_desc *gdp = (સંરચના ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	જો (!ext2fs_has_feature_metadata_csum(fs->super))
		પાછા 1;
	provided = gdp->bg_inode_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned અક્ષર *)bitmap,
				      size);
	જો (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		provided |= (__u32)gdp->bg_inode_bitmap_csum_hi << 16;
	નહીંતર
		calculated &= 0xFFFF;

	પાછા provided == calculated;
}

errcode_t ext2fs_inode_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       અક્ષર *bitmap, પૂર્ણાંક size)
{
	__u32 crc;
	સંરચના ext4_group_desc *gdp = (સંરચના ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	જો (!ext2fs_has_feature_metadata_csum(fs->super))
		પાછા 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned અક્ષર *)bitmap, size);
	gdp->bg_inode_bitmap_csum_lo = crc & 0xFFFF;
	જો (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_INODE_BITMAP_CSUM_HI_END)
		gdp->bg_inode_bitmap_csum_hi = crc >> 16;

	પાછા 0;
}

પૂર્ણાંક ext2fs_block_bitmap_csum_verify(ext2_filsys fs, dgrp_t group,
				    અક્ષર *bitmap, પૂર્ણાંક size)
{
	સંરચના ext4_group_desc *gdp = (સંરચના ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);
	__u32 provided, calculated;

	જો (!ext2fs_has_feature_metadata_csum(fs->super))
		પાછા 1;
	provided = gdp->bg_block_bitmap_csum_lo;
	calculated = ext2fs_crc32c_le(fs->csum_seed, (unsigned અક્ષર *)bitmap,
				      size);
	જો (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		provided |= (__u32)gdp->bg_block_bitmap_csum_hi << 16;
	નહીંતર
		calculated &= 0xFFFF;

	પાછા provided == calculated;
}

errcode_t ext2fs_block_bitmap_csum_set(ext2_filsys fs, dgrp_t group,
				       અક્ષર *bitmap, પૂર્ણાંક size)
{
	__u32 crc;
	સંરચના ext4_group_desc *gdp = (સંરચના ext4_group_desc *)
			ext2fs_group_desc(fs, fs->group_desc, group);

	જો (!ext2fs_has_feature_metadata_csum(fs->super))
		પાછા 0;

	crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned અક્ષર *)bitmap, size);
	gdp->bg_block_bitmap_csum_lo = crc & 0xFFFF;
	જો (EXT2_DESC_SIZE(fs->super) >= EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION)
		gdp->bg_block_bitmap_csum_hi = crc >> 16;

	પાછા 0;
}

સ્થિર errcode_t ext2fs_inode_csum(ext2_filsys fs, ext2_ino_t inum,
			       સંરચના ext2_inode_large *inode,
			       __u32 *crc, પૂર્ણાંક has_hi)
{
	__u32 gen;
	સંરચના ext2_inode_large *desc = inode;
	size_t size = EXT2_INODE_SIZE(fs->super);
	__u16 old_lo;
	__u16 old_hi = 0;

	old_lo = inode->i_checksum_lo;
	inode->i_checksum_lo = 0;
	જો (has_hi) {
		old_hi = inode->i_checksum_hi;
		inode->i_checksum_hi = 0;
	}

	inum = ext2fs_cpu_to_le32(inum);
	gen = inode->i_generation;
	*crc = ext2fs_crc32c_le(fs->csum_seed, (unsigned અક્ષર *)&inum,
				કદ(inum));
	*crc = ext2fs_crc32c_le(*crc, (unsigned અક્ષર *)&gen, કદ(gen));
	*crc = ext2fs_crc32c_le(*crc, (unsigned અક્ષર *)desc, size);

	inode->i_checksum_lo = old_lo;
	જો (has_hi)
		inode->i_checksum_hi = old_hi;
	પાછા 0;
}

પૂર્ણાંક ext2fs_inode_csum_verify(ext2_filsys fs, ext2_ino_t inum,
			     સંરચના ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 provided, calculated;
	unsigned પૂર્ણાંક i, has_hi;
	અક્ષર *cp;

	જો (!ext2fs_has_feature_metadata_csum(fs->super))
		પાછા 1;

	has_hi = (EXT2_INODE_SIZE(fs->super) > EXT2_GOOD_OLD_INODE_SIZE &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	provided = ext2fs_le16_to_cpu(inode->i_checksum_lo);
	retval = ext2fs_inode_csum(fs, inum, inode, &calculated, has_hi);
	જો (retval)
		પાછા 0;
	જો (has_hi) {
		__u32 hi = ext2fs_le16_to_cpu(inode->i_checksum_hi);
		provided |= hi << 16;
	} નહીંતર
		calculated &= 0xFFFF;

	જો (provided == calculated)
		પાછા 1;

	/*
	 * If the checksum didn't match, it's possible it was due to
	 * the inode being all zero's.  It's unlikely this is the
	 * case, but it can happen.  So check for it here.  (We only
	 * check the base inode since that's good enough, and it's not
	 * worth the bother to figure out how much of the extended
	 * inode, if any, is present.)
	 */
	માટે (cp = (અક્ષર *) inode, i = 0;
	     i < કદ(સંરચના ext2_inode);
	     cp++, i++)
		જો (*cp)
			પાછા 0;
	પાછા 1;		/* Inode must have been all zero's */
}

errcode_t ext2fs_inode_csum_set(ext2_filsys fs, ext2_ino_t inum,
			   સંરચના ext2_inode_large *inode)
{
	errcode_t retval;
	__u32 crc;
	પૂર્ણાંક has_hi;

	જો (!ext2fs_has_feature_metadata_csum(fs->super))
		પાછા 0;

	has_hi = (EXT2_INODE_SIZE(fs->super) > EXT2_GOOD_OLD_INODE_SIZE &&
		  inode->i_extra_isize >= EXT4_INODE_CSUM_HI_EXTRA_END);

	retval = ext2fs_inode_csum(fs, inum, inode, &crc, has_hi);
	જો (retval)
		પાછા retval;
	inode->i_checksum_lo = ext2fs_cpu_to_le16(crc & 0xFFFF);
	જો (has_hi)
		inode->i_checksum_hi = ext2fs_cpu_to_le16(crc >> 16);
	પાછા 0;
}

__u16 ext2fs_group_desc_csum(ext2_filsys fs, dgrp_t group)
{
	સંરચના ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	size_t offset, size = EXT2_DESC_SIZE(fs->super);
	__u16 crc = 0;
#ifdef શબ્દો_BIGENDIAN
	સંરચના ext4_group_desc swabdesc;
	size_t save_size = size;
	અચલ size_t ext4_bg_size = કદ(સંરચના ext4_group_desc);
	સંરચના ext2_group_desc *save_desc = desc;

	/* Have to swab back to little-endian to do the checksum */
	જો (size > ext4_bg_size)
		size = ext4_bg_size;
	memcpy(&swabdesc, desc, size);
	ext2fs_swap_group_desc2(fs, (સંરચના ext2_group_desc *) &swabdesc);
	desc = (સંરચના ext2_group_desc *) &swabdesc;
	group = ext2fs_swab32(group);
#endif

	જો (ext2fs_has_feature_metadata_csum(fs->super)) {
		/* new metadata csum code */
		__u16 old_crc;
		__u32 crc32;

		old_crc = desc->bg_checksum;
		desc->bg_checksum = 0;
		crc32 = ext2fs_crc32c_le(fs->csum_seed, (unsigned અક્ષર *)&group,
					 કદ(group));
		crc32 = ext2fs_crc32c_le(crc32, (unsigned અક્ષર *)desc,
					 size);
		desc->bg_checksum = old_crc;
#ifdef શબ્દો_BIGENDIAN
		જો (save_size > ext4_bg_size)
			crc32 = ext2fs_crc32c_le(crc32,
				     (unsigned અક્ષર *)save_desc + ext4_bg_size,
	                             save_size - ext4_bg_size);
#endif
		crc = crc32 & 0xFFFF;
		જાઓ out;
	}

	/* old crc16 code */
	offset = offsetof(સંરચના ext2_group_desc, bg_checksum);
	crc = ext2fs_crc16(~0, fs->super->s_uuid,
			   કદ(fs->super->s_uuid));
	crc = ext2fs_crc16(crc, &group, કદ(group));
	crc = ext2fs_crc16(crc, desc, offset);
	offset += કદ(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	જો (offset < size) {
		crc = ext2fs_crc16(crc, (અક્ષર *)desc + offset,
				   size - offset);
	}
#ifdef શબ્દો_BIGENDIAN
	/*
	 * If the size of the bg descriptor is greater than 64
	 * bytes, which is the size of the traditional ext4 bg
	 * descriptor, checksum the rest of the descriptor here
	 */
	જો (save_size > ext4_bg_size)
		crc = ext2fs_crc16(crc, (અક્ષર *)save_desc + ext4_bg_size,
				   save_size - ext4_bg_size);
#endif

out:
	પાછા crc;
}

પૂર્ણાંક ext2fs_group_desc_csum_verify(ext2_filsys fs, dgrp_t group)
{
	જો (ext2fs_has_group_desc_csum(fs) &&
	    (ext2fs_bg_checksum(fs, group) !=
	     ext2fs_group_desc_csum(fs, group)))
		પાછા 0;

	પાછા 1;
}

શૂન્ય ext2fs_group_desc_csum_set(ext2_filsys fs, dgrp_t group)
{
	જો (!ext2fs_has_group_desc_csum(fs))
		પાછા;

	/* ext2fs_bg_checksum_set() sets the actual checksum field but
	 * does not calculate the checksum itself. */
	ext2fs_bg_checksum_set(fs, group, ext2fs_group_desc_csum(fs, group));
}

સ્થિર __u32 find_last_inode_ingrp(ext2fs_inode_bitmap bitmap,
				   __u32 inodes_per_grp, dgrp_t grp_no)
{
	ext2_ino_t i, start_ino, end_ino;

	start_ino = grp_no * inodes_per_grp + 1;
	end_ino = start_ino + inodes_per_grp - 1;

	માટે (i = end_ino; i >= start_ino; i--) {
		જો (ext2fs_fast_test_inode_bitmap2(bitmap, i))
			પાછા i - start_ino + 1;
	}
	પાછા inodes_per_grp;
}

/* update the bitmap flags, set the itable high watermark, and calculate
 * checksums for the group descriptors */
errcode_t ext2fs_set_gdt_csum(ext2_filsys fs)
{
	સંરચના ext2_super_block *sb = fs->super;
	પૂર્ણાંક dirty = 0;
	dgrp_t i;

	જો (!fs->inode_map)
		પાછા EXT2_ET_NO_INODE_BITMAP;

	જો (!ext2fs_has_group_desc_csum(fs))
		પાછા 0;

	માટે (i = 0; i < fs->group_desc_ગણતરી; i++) {
		__u32 old_csum = ext2fs_bg_checksum(fs, i);
		__u32 old_unused = ext2fs_bg_itable_unused(fs, i);
		__u32 old_flags = ext2fs_bg_flags(fs, i);
		__u32 old_free_inodes_ગણતરી = ext2fs_bg_free_inodes_ગણતરી(fs, i);
		__u32 old_free_blocks_ગણતરી = ext2fs_bg_free_blocks_ગણતરી(fs, i);

		જો (old_free_blocks_ગણતરી == sb->s_blocks_per_group &&
		    i != fs->group_desc_ગણતરી - 1)
			ext2fs_bg_flags_set(fs, i, EXT2_BG_BLOCK_UNINIT);

		જો (old_free_inodes_ગણતરી == sb->s_inodes_per_group) {
			ext2fs_bg_flags_set(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, i, sb->s_inodes_per_group);
		} નહીંતર {
			પૂર્ણાંક unused =
				sb->s_inodes_per_group -
				find_last_inode_ingrp(fs->inode_map,
						      sb->s_inodes_per_group, i);

			ext2fs_bg_flags_clear(fs, i, EXT2_BG_INODE_UNINIT);
			ext2fs_bg_itable_unused_set(fs, i, unused);
		}

		ext2fs_group_desc_csum_set(fs, i);
		જો (old_flags != ext2fs_bg_flags(fs, i))
			dirty = 1;
		જો (old_unused != ext2fs_bg_itable_unused(fs, i))
			dirty = 1;
		જો (old_csum != ext2fs_bg_checksum(fs, i))
			dirty = 1;
	}
	જો (dirty)
		ext2fs_mark_super_dirty(fs);
	પાછા 0;
}

#ifdef DEBUG
#અન્તર્ભાવ "e2p/e2p.h"

શૂન્ય print_csum(અચલ અક્ષર *સંદેશ, ext2_filsys fs, dgrp_t group)
{
	__u16 crc1, crc2, crc3;
	dgrp_t swabgroup;
	સંરચના ext2_group_desc *desc = ext2fs_group_desc(fs, fs->group_desc,
							 group);
	size_t size = EXT2_DESC_SIZE(fs->super);
	સંરચના ext2_super_block *sb = fs->super;
	પૂર્ણાંક offset = offsetof(સંરચના ext2_group_desc, bg_checksum);
#ifdef શબ્દો_BIGENDIAN
	સંરચના ext4_group_desc swabdesc;
	સંરચના ext2_group_desc *save_desc = desc;
	અચલ size_t ext4_bg_size = કદ(સંરચના ext4_group_desc);
	size_t save_size = size;
#endif

#ifdef શબ્દો_BIGENDIAN
	/* Have to swab back to little-endian to do the checksum */
	જો (size > ext4_bg_size)
		size = ext4_bg_size;
	memcpy(&swabdesc, desc, size);
	ext2fs_swap_group_desc2(fs, (સંરચના ext2_group_desc *) &swabdesc);
	desc = (સંરચના ext2_group_desc *) &swabdesc;

	swabgroup = ext2fs_swab32(group);
#નહીંતર
	swabgroup = group;
#endif

	crc1 = ext2fs_crc16(~0, sb->s_uuid, કદ(fs->super->s_uuid));
	crc2 = ext2fs_crc16(crc1, &swabgroup, કદ(swabgroup));
	crc3 = ext2fs_crc16(crc2, desc, offset);
	offset += કદ(desc->bg_checksum); /* skip checksum */
	/* for checksum of struct ext4_group_desc do the rest...*/
	જો (offset < size)
		crc3 = ext2fs_crc16(crc3, (અક્ષર *)desc + offset, size - offset);
#ifdef શબ્દો_BIGENDIAN
	જો (save_size > ext4_bg_size)
		crc3 = ext2fs_crc16(crc3, (અક્ષર *)save_desc + ext4_bg_size,
				    save_size - ext4_bg_size);
#endif

	printf("%s UUID %s=%04x, grp %u=%04x: %04x=%04x\n",
	       સંદેશ, e2p_uuid2str(sb->s_uuid), crc1, group, crc2, crc3,
	       ext2fs_group_desc_csum(fs, group));
}

unsigned અક્ષર sb_uuid[16] = { 0x4f, 0x25, 0xe8, 0xcf, 0xe7, 0x97, 0x48, 0x23,
			      0xbe, 0xfa, 0xa7, 0x88, 0x4b, 0xae, 0xec, 0xdb };

પૂર્ણાંક મુખ્ય(પૂર્ણાંક argc, અક્ષર **argv)
{
	સંરચના ext2_super_block param;
	errcode_t		retval;
	ext2_filsys		fs;
	પૂર્ણાંક			i;
	__u16 csum1, csum2, csum_known = 0xd3a4;

	memset(&param, 0, કદ(param));
	ext2fs_blocks_ગણતરી_set(&param, 32768);
#જો 0
	param.s_feature_incompat |= EXT4_FEATURE_INCOMPAT_64BIT;
	param.s_desc_size = 128;
	csum_known = 0x5b6e;
#endif

	retval = ext2fs_initialize("test fs", EXT2_FLAG_64BITS, &param,
				   test_io_manager, &fs);
	જો (retval) {
		com_err("setup", retval,
			"While initializing filesystem");
		exit(1);
	}
	memcpy(fs->super->s_uuid, sb_uuid, 16);
	fs->super->s_feature_ro_compat = EXT4_FEATURE_RO_COMPAT_GDT_CSUM;

	માટે (i=0; i < fs->group_desc_ગણતરી; i++) {
		ext2fs_block_bitmap_loc_set(fs, i, 124);
		ext2fs_inode_bitmap_loc_set(fs, i, 125);
		ext2fs_inode_table_loc_set(fs, i, 126);
		ext2fs_bg_free_blocks_ગણતરી_set(fs, i, 31119);
		ext2fs_bg_free_inodes_ગણતરી_set(fs, i, 15701);
		ext2fs_bg_used_dirs_ગણતરી_set(fs, i, 2);
		ext2fs_bg_flags_zap(fs, i);
	};

	csum1 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum0000", fs, 0);

	જો (csum1 != csum_known) {
		printf("checksum for group 0 should be %04x\n", csum_known);
		exit(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 1);
	print_csum("csum0001", fs, 1);
	જો (csum1 == csum2) {
		printf("checksums for different groups shouldn't match\n");
		exit(1);
	}
	csum2 = ext2fs_group_desc_csum(fs, 2);
	print_csum("csumffff", fs, 2);
	જો (csum1 == csum2) {
		printf("checksums for different groups shouldn't match\n");
		exit(1);
	}
	ext2fs_bg_checksum_set(fs, 0, csum1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_set", fs, 0);
	જો (csum1 != csum2) {
		printf("checksums should not depend on checksum field\n");
		exit(1);
	}
	જો (!ext2fs_group_desc_csum_verify(fs, 0)) {
		printf("checksums should verify against gd_checksum\n");
		exit(1);
	}
	memset(fs->super->s_uuid, 0x30, કદ(fs->super->s_uuid));
	print_csum("new_uuid", fs, 0);
	જો (ext2fs_group_desc_csum_verify(fs, 0) != 0) {
		printf("checksums for different filesystems shouldn't match\n");
		exit(1);
	}
	csum1 = ext2fs_group_desc_csum(fs, 0);
	ext2fs_bg_checksum_set(fs, 0, csum1);
	print_csum("csum_new", fs, 0);
	ext2fs_bg_free_blocks_ગણતરી_set(fs, 0, 1);
	csum2 = ext2fs_group_desc_csum(fs, 0);
	print_csum("csum_blk", fs, 0);
	જો (csum1 == csum2) {
		printf("checksums for different data shouldn't match\n");
		exit(1);
	}
	ext2fs_free(fs);

	પાછા 0;
}
#endif
