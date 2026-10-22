/*
 * link.c --- create links in a ext2fs directory
 *
 * Copyright (C) 1993, 1994 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

#define EXT2_DX_ಬೇರು_OFF 24

ಸಂರಚನಾ dx_frame {
	ಶೂನ್ಯ *buf;
	blk64_t pblock;
	ಸಂರಚನಾ ext2_dx_countlimit *head;
	ಸಂರಚನಾ ext2_dx_entry *entries;
	ಸಂರಚನಾ ext2_dx_entry *at;
};

ಸಂರಚನಾ dx_lookup_info {
	ಧ್ರುವ ವರ್ಣ *name;
	ಪೂರ್ಣಾಙ್ಕ namelen;
	ಪೂರ್ಣಾಙ್ಕ hash_alg;
	__u32 hash;
	unsigned levels;
	ಸಂರಚನಾ dx_frame frames[EXT4_HTREE_LEVEL];
};

ಸ್ಥಿರ errcode_t alloc_dx_frame(ext2_filsys fs, ಸಂರಚನಾ dx_frame *frame)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_get_mem(fs->blocksize, &frame->buf);
}

ಸ್ಥಿರ ಶೂನ್ಯ dx_release(ಸಂರಚನಾ dx_lookup_info *info)
{
	unsigned level;

	ಪ್ರತಿ (level = 0; level < info->levels; level++) {
		ಯದಿ (info->frames[level].buf == ರಿಕ್ತ)
			ಭಙ್ಗ;
		ext2fs_free_mem(&(info->frames[level].buf));
	}
	info->levels = 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ dx_ಹುಡುಕು_entry(ಸಂರಚನಾ dx_frame *frame, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, __u32 hash)
{
	ಸಂರಚನಾ ext2_dx_entry *p, *q, *m;

	p = frame->entries + 1;
	q = frame->entries + ಎಣಿಕೆ - 1;
	ಯಾವತ್ (p <= q) {
		m = p + (q - p) / 2;
		ಯದಿ (ext2fs_le32_to_cpu(m->hash) > hash)
			q = m - 1;
		ಅನ್ಯಥಾ
			p = m + 1;
	}
	frame->at = p - 1;
}

ಸ್ಥಿರ errcode_t load_logical_dir_block(ext2_filsys fs, ext2_ino_t dir,
					ಸಂರಚನಾ ext2_inode *diri, blk64_t block,
					blk64_t *pblk, ಶೂನ್ಯ *buf)
{
	errcode_t errcode;
	ಪೂರ್ಣಾಙ್ಕ ret_flags;

	errcode = ext2fs_bmap2(fs, dir, diri, ರಿಕ್ತ, 0, block, &ret_flags,
			       pblk);
	ಯದಿ (errcode)
		ಪ್ರತ್ಯಾಗಮ errcode;
	ಯದಿ (ret_flags & BMAP_RET_UNINIT)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_DIR_CORRUPTED;
	ಪ್ರತ್ಯಾಗಮ ext2fs_read_dir_block4(fs, *pblk, buf, 0, dir);
}

ಸ್ಥಿರ errcode_t dx_lookup(ext2_filsys fs, ext2_ino_t dir,
			   ಸಂರಚನಾ ext2_inode *diri, ಸಂರಚನಾ dx_lookup_info *info)
{
	ಸಂರಚನಾ ext2_dx_ಬೇರು_info *ಬೇರು;
	errcode_t errcode;
	ಪೂರ್ಣಾಙ್ಕ level = 0;
	ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, limit;
	ಪೂರ್ಣಾಙ್ಕ hash_alg;
	ಪೂರ್ಣಾಙ್ಕ hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	__u32 minor_hash;
	ಸಂರಚನಾ dx_frame *frame;

	errcode = alloc_dx_frame(fs, &(info->frames[0]));
	ಯದಿ (errcode)
		ಪ್ರತ್ಯಾಗಮ errcode;
	info->levels = 1;

	errcode = load_logical_dir_block(fs, dir, diri, 0,
					 &(info->frames[0].pblock),
					 info->frames[0].buf);
	ಯದಿ (errcode)
		ಗಚ್ಛ out_err;
	ಬೇರು = (ಸಂರಚನಾ ext2_dx_ಬೇರು_info *) ((ವರ್ಣ *)info->frames[0].buf +
					     EXT2_DX_ಬೇರು_OFF);
	hash_alg = ಬೇರು->hash_version;
	ಯದಿ (hash_alg != EXT2_HASH_TEA && hash_alg != EXT2_HASH_HALF_MD4 &&
	    hash_alg != EXT2_HASH_LEGACY) {
		errcode = EXT2_ET_DIRHASH_UNSUPP;
		ಗಚ್ಛ out_err;
	}
	ಯದಿ (hash_alg <= EXT2_HASH_TEA &&
	    fs->super->s_flags & EXT2_FLAGS_UNSIGNED_HASH)
		hash_alg += 3;
	ಯದಿ (ಬೇರು->indirect_levels >= ext2_dir_htree_level(fs)) {
		errcode = EXT2_ET_DIR_CORRUPTED;
		ಗಚ್ಛ out_err;
	}
	info->hash_alg = hash_alg;

	errcode = ext2fs_dirhash2(hash_alg, info->name, info->namelen,
				  fs->encoding, hash_flags,
				  fs->super->s_hash_seed, &info->hash,
				  &minor_hash);
	ಯದಿ (errcode)
		ಗಚ್ಛ out_err;

	ಪ್ರತಿ (level = 0; level <= ಬೇರು->indirect_levels; level++) {
		frame = &(info->frames[level]);
		ಯದಿ (level > 0) {
			errcode = alloc_dx_frame(fs, frame);
			ಯದಿ (errcode)
				ಗಚ್ಛ out_err;
			info->levels++;

			errcode = load_logical_dir_block(fs, dir, diri,
				ext2fs_le32_to_cpu(info->frames[level-1].at->block) & 0x0fffffff,
				&(frame->pblock), frame->buf);
			ಯದಿ (errcode)
				ಗಚ್ಛ out_err;
		}
		errcode = ext2fs_get_dx_countlimit(fs, frame->buf,
						   &(frame->head), ರಿಕ್ತ);
		ಯದಿ (errcode)
			ಗಚ್ಛ out_err;
		ಎಣಿಕೆ = ext2fs_le16_to_cpu(frame->head->ಎಣಿಕೆ);
		limit = ext2fs_le16_to_cpu(frame->head->limit);
		frame->entries = (ಸಂರಚನಾ ext2_dx_entry *)(frame->head);
		ಯದಿ (!ಎಣಿಕೆ || ಎಣಿಕೆ > limit) {
			errcode = EXT2_ET_DIR_CORRUPTED;
			ಗಚ್ಛ out_err;
		}

		dx_ಹುಡುಕು_entry(frame, ಎಣಿಕೆ, info->hash);
	}
	ಪ್ರತ್ಯಾಗಮ 0;
out_err:
	dx_release(info);
	ಪ್ರತ್ಯಾಗಮ errcode;
}

ಸಂರಚನಾ link_struct  {
	ext2_filsys	fs;
	ಧ್ರುವ ವರ್ಣ	*name;
	ಪೂರ್ಣಾಙ್ಕ		namelen;
	ext2_ino_t	inode;
	ಪೂರ್ಣಾಙ್ಕ		flags;
	ಪೂರ್ಣಾಙ್ಕ		done;
	unsigned ಪೂರ್ಣಾಙ್ಕ	blocksize;
	errcode_t	err;
	ಸಂರಚನಾ ext2_super_block *sb;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ link_proc(ext2_ino_t dir EXT2FS_ATTR((unused)),
		     ಪೂರ್ಣಾಙ್ಕ entru EXT2FS_ATTR((unused)),
		     ಸಂರಚನಾ ext2_dir_entry *dirent,
		     ಪೂರ್ಣಾಙ್ಕ	offset,
		     ಪೂರ್ಣಾಙ್ಕ	blocksize,
		     ವರ್ಣ	*buf,
		     ಶೂನ್ಯ	*priv_data)
{
	ಸಂರಚನಾ link_struct *ls = (ಸಂರಚನಾ link_struct *) priv_data;
	ಸಂರಚನಾ ext2_dir_entry *next;
	unsigned ಪೂರ್ಣಾಙ್ಕ rec_len, min_rec_len, curr_rec_len;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;
	ಪೂರ್ಣಾಙ್ಕ csum_size = 0;

	ಯದಿ (ls->done)
		ಪ್ರತ್ಯಾಗಮ DIRENT_ABORT;

	rec_len = EXT2_DIR_REC_LEN(ls->namelen);

	ls->err = ext2fs_get_rec_len(ls->fs, dirent, &curr_rec_len);
	ಯದಿ (ls->err)
		ಪ್ರತ್ಯಾಗಮ DIRENT_ABORT;

	ಯದಿ (ext2fs_has_feature_metadata_csum(ls->fs->super))
		csum_size = ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_dir_entry_tail);
	/*
	 * See if the following directory entry (if any) is unused;
	 * if so, absorb it into this one.
	 */
	next = (ಸಂರಚನಾ ext2_dir_entry *) (buf + offset + curr_rec_len);
	ಯದಿ ((offset + (ಪೂರ್ಣಾಙ್ಕ) curr_rec_len < blocksize - (8 + csum_size)) &&
	    (next->inode == 0) &&
	    (offset + (ಪೂರ್ಣಾಙ್ಕ) curr_rec_len + (ಪೂರ್ಣಾಙ್ಕ) next->rec_len <= blocksize)) {
		curr_rec_len += next->rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, curr_rec_len, dirent);
		ಯದಿ (ls->err)
			ಪ್ರತ್ಯಾಗಮ DIRENT_ABORT;
		ret = DIRENT_CHANGED;
	}

	/*
	 * If the directory entry is used, see if we can split the
	 * directory entry to make room for the new name.  If so,
	 * truncate it and return.
	 */
	ಯದಿ (dirent->inode) {
		min_rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(dirent));
		ಯದಿ (curr_rec_len < (min_rec_len + rec_len))
			ಪ್ರತ್ಯಾಗಮ ret;
		rec_len = curr_rec_len - min_rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, min_rec_len, dirent);
		ಯದಿ (ls->err)
			ಪ್ರತ್ಯಾಗಮ DIRENT_ABORT;
		next = (ಸಂರಚನಾ ext2_dir_entry *) (buf + offset +
						  dirent->rec_len);
		next->inode = 0;
		ext2fs_dirent_set_name_len(next, 0);
		ext2fs_dirent_set_file_type(next, 0);
		ls->err = ext2fs_set_rec_len(ls->fs, rec_len, next);
		ಯದಿ (ls->err)
			ಪ್ರತ್ಯಾಗಮ DIRENT_ABORT;
		ಪ್ರತ್ಯಾಗಮ DIRENT_CHANGED;
	}

	/*
	 * If we get this far, then the directory entry is not used.
	 * See if we can fit the request entry in.  If so, do it.
	 */
	ಯದಿ (curr_rec_len < rec_len)
		ಪ್ರತ್ಯಾಗಮ ret;
	dirent->inode = ls->inode;
	ext2fs_dirent_set_name_len(dirent, ls->namelen);
	strncpy(dirent->name, ls->name, ls->namelen);
	ಯದಿ (ext2fs_has_feature_filetype(ls->sb))
		ext2fs_dirent_set_file_type(dirent,
					    ls->flags & EXT2FS_LINK_FT_MASK);

	ls->done++;
	ಪ್ರತ್ಯಾಗಮ DIRENT_ABORT|DIRENT_CHANGED;
}

ಸ್ಥಿರ errcode_t add_dirent_to_buf(ext2_filsys fs, e2_blkcnt_t blockcnt,
				   ವರ್ಣ *buf, ext2_ino_t dir,
				   ಸಂರಚನಾ ext2_inode *diri, ಧ್ರುವ ವರ್ಣ *name,
				   ext2_ino_t ino, ಪೂರ್ಣಾಙ್ಕ flags, blk64_t *pblkp)
{
	ಸಂರಚನಾ dir_context ctx;
	ಸಂರಚನಾ link_struct ls;
	errcode_t retval;

	retval = load_logical_dir_block(fs, dir, diri, blockcnt, pblkp, buf);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	ctx.errcode = 0;
	ctx.func = link_proc;
	ctx.dir = dir;
	ctx.flags = DIRENT_FLAG_INCLUDE_EMPTY;
	ctx.buf = buf;
	ctx.priv_data = &ls;

	ls.fs = fs;
	ls.name = name;
	ls.namelen = strlen(name);
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	ext2fs_process_dir_block(fs, pblkp, blockcnt, 0, 0, &ctx);
	ಯದಿ (ctx.errcode)
		ಪ್ರತ್ಯಾಗಮ ctx.errcode;
	ಯದಿ (ls.err)
		ಪ್ರತ್ಯಾಗಮ ls.err;
	ಯದಿ (!ls.done)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_DIR_NO_SPACE;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸಂರಚನಾ dx_hash_map {
	__u32 hash;
	ಪೂರ್ಣಾಙ್ಕ size;
	ಪೂರ್ಣಾಙ್ಕ off;
};

ಸ್ಥಿರ EXT2_QSORT_TYPE dx_hash_map_cmp(ಧ್ರುವ ಶೂನ್ಯ *ap, ಧ್ರುವ ಶೂನ್ಯ *bp)
{
	ಧ್ರುವ ಸಂರಚನಾ dx_hash_map *a = ap, *b = bp;

	ಯದಿ (a->hash < b->hash)
		ಪ್ರತ್ಯಾಗಮ -1;
	ಯದಿ (a->hash > b->hash)
		ಪ್ರತ್ಯಾಗಮ 1;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t dx_move_dirents(ext2_filsys fs, ಸಂರಚನಾ dx_hash_map *map,
				 ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಶೂನ್ಯ *from, ಶೂನ್ಯ *to)
{
	ಸಂರಚನಾ ext2_dir_entry *de;
	ಪೂರ್ಣಾಙ್ಕ i;
	ಪೂರ್ಣಾಙ್ಕ rec_len = 0;
	errcode_t retval;
	ಪೂರ್ಣಾಙ್ಕ csum_size = 0;
	ಶೂನ್ಯ *base = to;

	ಯದಿ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_dir_entry_tail);

	ಪ್ರತಿ (i = 0; i < ಎಣಿಕೆ; i++) {
		de = (ಸಂರಚನಾ ext2_dir_entry *) ((ವರ್ಣ *)from + map[i].off);
		rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(de));
		memcpy(to, de, rec_len);
		retval = ext2fs_set_rec_len(fs, rec_len, to);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
		to = (ವರ್ಣ *)to + rec_len;
	}
	/*
	 * Update rec_len of the last dir entry to stretch to the end of block
	 */
	to = (ವರ್ಣ *)to - rec_len;
	rec_len = fs->blocksize - ((ವರ್ಣ *)to - (ವರ್ಣ *)base) - csum_size;
	retval = ext2fs_set_rec_len(fs, rec_len, to);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	ಯದಿ (csum_size)
		ext2fs_initialize_dirent_tail(fs,
				EXT2_DIRENT_TAIL(base, fs->blocksize));
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t dx_ಸೇರಿಸು_entry(ext2_filsys fs, ext2_ino_t dir,
				 ಸಂರಚನಾ dx_lookup_info *info, ಪೂರ್ಣಾಙ್ಕ level,
				 __u32 hash, blk64_t lblk)
{
	ಪೂರ್ಣಾಙ್ಕ pcount;
	ಸಂರಚನಾ ext2_dx_entry *ತುದಿ, *new;

	pcount = ext2fs_le16_to_cpu(info->frames[level].head->ಎಣಿಕೆ);
	ತುದಿ = info->frames[level].entries + pcount;
	new = info->frames[level].at + 1;
	memmove(new + 1, new, (ವರ್ಣ *)ತುದಿ - (ವರ್ಣ *)new);
	new->hash = ext2fs_cpu_to_le32(hash);
	new->block = ext2fs_cpu_to_le32(lblk);
	info->frames[level].head->ಎಣಿಕೆ = ext2fs_cpu_to_le16(pcount + 1);
	ಪ್ರತ್ಯಾಗಮ ext2fs_write_dir_block4(fs, info->frames[level].pblock,
				       info->frames[level].buf, 0, dir);
}

ಸ್ಥಿರ errcode_t dx_split_leaf(ext2_filsys fs, ext2_ino_t dir,
			       ಸಂರಚನಾ ext2_inode *diri,
			       ಸಂರಚನಾ dx_lookup_info *info, ಶೂನ್ಯ *buf,
			       blk64_t leaf_pblk, blk64_t new_lblk,
			       blk64_t new_pblk)
{
	ಪೂರ್ಣಾಙ್ಕ hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	ಸಂರಚನಾ ext2_dir_entry *de;
	ಶೂನ್ಯ *buf2;
	errcode_t retval = 0;
	unsigned ಪೂರ್ಣಾಙ್ಕ rec_len;
	unsigned ಪೂರ್ಣಾಙ್ಕ offset, move_size;
	ಪೂರ್ಣಾಙ್ಕ i, ಎಣಿಕೆ = 0;
	ಸಂರಚನಾ dx_hash_map *map;
	ಪೂರ್ಣಾಙ್ಕ continued;
	__u32 minor_hash;

	retval = ext2fs_get_mem(fs->blocksize, &buf2);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	retval = ext2fs_get_array(fs->blocksize / 12,
				  ಪರಿಮಾಣ(ಸಂರಚನಾ dx_hash_map), &map);
	ಯದಿ (retval) {
		ext2fs_free_mem(&buf2);
		ಪ್ರತ್ಯಾಗಮ retval;
	}
	ಪ್ರತಿ (offset = 0; offset < fs->blocksize; offset += rec_len) {
		de = (ಸಂರಚನಾ ext2_dir_entry *) ((ವರ್ಣ *)buf + offset);
		retval = ext2fs_get_rec_len(fs, de, &rec_len);
		ಯದಿ (retval)
			ಗಚ್ಛ out;
		ಯದಿ (ext2fs_dirent_name_len(de) > 0 && de->inode) {
			map[ಎಣಿಕೆ].off = offset;
			map[ಎಣಿಕೆ].size = rec_len;
			retval = ext2fs_dirhash2(info->hash_alg, de->name,
					ext2fs_dirent_name_len(de),
					fs->encoding, hash_flags,
					fs->super->s_hash_seed,
					&(map[ಎಣಿಕೆ].hash),
					&minor_hash);
			ಯದಿ (retval)
				ಗಚ್ಛ out;
			ಎಣಿಕೆ++;
		}
	}
	qsort(map, ಎಣಿಕೆ, ಪರಿಮಾಣ(ಸಂರಚನಾ dx_hash_map), dx_hash_map_cmp);
	move_size = 0;
	/* Find place to split block */
	ಪ್ರತಿ (i = ಎಣಿಕೆ - 1; i >= 0; i--) {
		ಯದಿ (move_size + map[i].size / 2 > fs->blocksize / 2)
			ಭಙ್ಗ;
		move_size += map[i].size;
	}
	/* Let i be the first entry to move */
	i++;
	/* Move selected directory entries to new block */
	retval = dx_move_dirents(fs, map + i, ಎಣಿಕೆ - i, buf, buf2);
	ಯದಿ (retval)
		ಗಚ್ಛ out;
	retval = ext2fs_write_dir_block4(fs, new_pblk, buf2, 0, dir);
	ಯದಿ (retval)
		ಗಚ್ಛ out;
	/* Repack remaining entries in the old block */
	retval = dx_move_dirents(fs, map, i, buf, buf2);
	ಯದಿ (retval)
		ಗಚ್ಛ out;
	retval = ext2fs_write_dir_block4(fs, leaf_pblk, buf2, 0, dir);
	ಯದಿ (retval)
		ಗಚ್ಛ out;
	/* Update parent node */
	continued = map[i].hash == map[i-1].hash;
	retval = dx_ಸೇರಿಸು_entry(fs, dir, info, info->levels - 1,
				 map[i].hash + continued, new_lblk);
out:
	ext2fs_free_mem(&buf2);
	ext2fs_free_mem(&map);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ errcode_t dx_grow_ವೃಕ್ಷ(ext2_filsys fs, ext2_ino_t dir,
			      ಸಂರಚನಾ ext2_inode *diri,
			      ಸಂರಚನಾ dx_lookup_info *info, ಶೂನ್ಯ *buf,
			      blk64_t leaf_pblk)
{
	ಪೂರ್ಣಾಙ್ಕ i;
	errcode_t retval;
	ext2_off64_t size = EXT2_I_SIZE(diri);
	blk64_t lblk, pblk;
	ಸಂರಚನಾ ext2_dir_entry *de;
	ಸಂರಚನಾ ext2_dx_countlimit *head;
	ಪೂರ್ಣಾಙ್ಕ csum_size = 0;
	ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ;

	ಯದಿ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_dx_tail);

	/* Find level which can accommodate new child */
	ಪ್ರತಿ (i = info->levels - 1; i >= 0; i--)
		ಯದಿ (ext2fs_le16_to_cpu(info->frames[i].head->ಎಣಿಕೆ) <
		    ext2fs_le16_to_cpu(info->frames[i].head->limit))
			ಭಙ್ಗ;
	/* Need to grow tree depth? */
	ಯದಿ (i < 0 && info->levels >= ext2_dir_htree_level(fs))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_DIR_NO_SPACE;
	lblk = size / fs->blocksize;
	size += fs->blocksize;
	retval = ext2fs_inode_size_set(fs, diri, size);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	retval = ext2fs_fallocate(fs,
			EXT2_FALLOCATE_FORCE_INIT | EXT2_FALLOCATE_ZERO_BLOCKS,
			dir, diri, 0, lblk, 1);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	retval = ext2fs_write_inode(fs, dir, diri);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	retval = ext2fs_bmap2(fs, dir, diri, ರಿಕ್ತ, 0, lblk, ರಿಕ್ತ, &pblk);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	/* Only leaf addition needed? */
	ಯದಿ (i == (ಪೂರ್ಣಾಙ್ಕ)info->levels - 1)
		ಪ್ರತ್ಯಾಗಮ dx_split_leaf(fs, dir, diri, info, buf, leaf_pblk,
				     lblk, pblk);

	de = buf;
	de->inode = 0;
	ext2fs_dirent_set_name_len(de, 0);
	ext2fs_dirent_set_file_type(de, 0);
	retval = ext2fs_set_rec_len(fs, fs->blocksize, de);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	head = (ಸಂರಚನಾ ext2_dx_countlimit *) ((ವರ್ಣ *)buf + 8);
	ಎಣಿಕೆ = ext2fs_le16_to_cpu(info->frames[i+1].head->ಎಣಿಕೆ);
	/* Growing tree depth? */
	ಯದಿ (i < 0) {
		ಸಂರಚನಾ ext2_dx_ಬೇರು_info *ಬೇರು;

		memcpy(head, info->frames[0].entries,
		       ಎಣಿಕೆ * ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_dx_entry));
		head->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_size)) /
				ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_dx_entry));
		/* head->count gets set by memcpy above to correct value */

		/* Now update tree root */
		info->frames[0].head->ಎಣಿಕೆ = ext2fs_cpu_to_le16(1);
		info->frames[0].entries[0].block = ext2fs_cpu_to_le32(lblk);
		ಬೇರು = (ಸಂರಚನಾ ext2_dx_ಬೇರು_info *)
			((ವರ್ಣ *)info->frames[0].buf + EXT2_DX_ಬೇರು_OFF);
		ಬೇರು->indirect_levels++;
	} ಅನ್ಯಥಾ {
		/* Splitting internal node in two */
		ಪೂರ್ಣಾಙ್ಕ count1 = ಎಣಿಕೆ / 2;
		ಪೂರ್ಣಾಙ್ಕ count2 = ಎಣಿಕೆ - count1;
		__u32 split_hash = ext2fs_le32_to_cpu(info->frames[i+1].entries[count1].hash);

		memcpy(head, info->frames[i+1].entries + count1,
		       count2 * ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_dx_entry));
		head->ಎಣಿಕೆ = ext2fs_cpu_to_le16(count2);
		head->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_size)) /
				ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_dx_entry));
		info->frames[i+1].head->ಎಣಿಕೆ = ext2fs_cpu_to_le16(count1);

		/* Update parent node */
		retval = dx_ಸೇರಿಸು_entry(fs, dir, info, i, split_hash, lblk);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;

	}
	/* Writeout split block / updated root */
	retval = ext2fs_write_dir_block4(fs, info->frames[i+1].pblock,
					 info->frames[i+1].buf, 0, dir);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	/* Writeout new tree block */
	retval = ext2fs_write_dir_block4(fs, pblk, buf, 0, dir);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t dx_link(ext2_filsys fs, ext2_ino_t dir,
			 ಸಂರಚನಾ ext2_inode *diri, ಧ್ರುವ ವರ್ಣ *name,
			 ext2_ino_t ino, ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ dx_lookup_info dx_info;
	errcode_t retval;
	ಶೂನ್ಯ *blockbuf;
	unsigned restart = 0;
	blk64_t leaf_pblk;

	retval = ext2fs_get_mem(fs->blocksize, &blockbuf);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	dx_info.name = name;
	dx_info.namelen = strlen(name);
again:
	retval = dx_lookup(fs, dir, diri, &dx_info);
	ಯದಿ (retval)
		ಗಚ್ಛ free_buf;

	retval = add_dirent_to_buf(fs,
		ext2fs_le32_to_cpu(dx_info.frames[dx_info.levels-1].at->block) & 0x0fffffff,
		blockbuf, dir, diri, name, ino, flags, &leaf_pblk);
	/*
	 * Success or error other than ENOSPC...? We are done. We may need upto
	 * two tries to add entry. One to split htree node and another to add
	 * new leaf block.
	 */
	ಯದಿ (restart >= dx_info.levels || retval != EXT2_ET_DIR_NO_SPACE)
		ಗಚ್ಛ free_frames;
	retval = dx_grow_ವೃಕ್ಷ(fs, dir, diri, &dx_info, blockbuf, leaf_pblk);
	ಯದಿ (retval)
		ಗಚ್ಛ free_frames;
	/* Restart everything now that the tree is larger */
	restart++;
	dx_release(&dx_info);
	ಗಚ್ಛ again;
free_frames:
	dx_release(&dx_info);
free_buf:
	ext2fs_free_mem(&blockbuf);
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸಂರಚನಾ fake_dirent
{
	__le32 inode;
	__le16 rec_len;
	__u8 name_len;
	__u8 file_type;
};

/*
 * dx_root_info is laid out so that if it should somehow get overlaid by a
 * dirent the two low bits of the hash version will be zero.  Therefore, the
 * hash version mod 4 should never be 0.  Sincerely, the paranoia department.
 */

ಸಂರಚನಾ dx_ಬೇರು
{
	ಸಂರಚನಾ fake_dirent dot;
	ವರ್ಣ dot_name[4];
	ಸಂರಚನಾ fake_dirent dotdot;
	ವರ್ಣ dotdot_name[4];
	ಸಂರಚನಾ ext2_dx_ಬೇರು_info info;
	ಸಂರಚನಾ ext2_dx_entry entries[];
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ check_dx_ಬೇರು(ext2_filsys fs, ext2_ino_t ino, ಸಂರಚನಾ dx_ಬೇರು *ಬೇರು)
{
	ಸಂರಚನಾ fake_dirent *fde;
	ವರ್ಣ *error_ಸಂದೇಶ;
	unsigned ಪೂರ್ಣಾಙ್ಕ rlen;
	ವರ್ಣ *blockend = (ವರ್ಣ *)ಬೇರು + fs->blocksize;

	fde = &ಬೇರು->dot;
	ಯದಿ (fde->name_len != 1) {
		error_ಸಂದೇಶ = "invalid name_len for '.'";
		ಗಚ್ಛ corrupted;
	}
	ಯದಿ (strncmp(ಬೇರು->dot_name, ".", fde->name_len)) {
		error_ಸಂದೇಶ = "invalid name for '.'";
		ಗಚ್ಛ corrupted;
	}

	ext2fs_get_rec_len(fs, (ಸಂರಚನಾ ext2_dir_entry *)fde, &rlen);
	ಯದಿ ((ವರ್ಣ *)fde + rlen >= blockend) {
		error_ಸಂದೇಶ = "invalid rec_len for '.'";
		ಗಚ್ಛ corrupted;
	}

	fde = &ಬೇರು->dotdot;
	ಯದಿ (fde->name_len != 2) {
		error_ಸಂದೇಶ = "invalid name_len for '..'";
		ಗಚ್ಛ corrupted;
	}
	ಯದಿ (strncmp(ಬೇರು->dotdot_name, "..", fde->name_len)) {
		error_ಸಂದೇಶ = "invalid name for '..'";
		ಗಚ್ಛ corrupted;
	}
	ext2fs_get_rec_len(fs, (ಸಂರಚನಾ ext2_dir_entry *)fde, &rlen);
	ಯದಿ ((ವರ್ಣ *)fde + rlen >= blockend) {
		error_ಸಂದೇಶ = "invalid rec_len for '..'";
		ಗಚ್ಛ corrupted;
	}

	ಪ್ರತ್ಯಾಗಮ 1;

corrupted:
	fprintf(stderr, "Corrupt dir %u: %s, running e2fsck is recommended\n",
			 ino, error_ಸಂದೇಶ);
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ ext2_dir_entry *
ext2fs_next_entry(ext2_filsys fs, ಸಂರಚನಾ ext2_dir_entry *p)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ rlen;

	ext2fs_get_rec_len(fs, p, &rlen);
	ಪ್ರತ್ಯಾಗಮ (ಸಂರಚನಾ ext2_dir_entry *)((ವರ್ಣ *)p + rlen);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ dx_set_block(ಸಂರಚನಾ ext2_dx_entry *entry, unsigned ಮೌಲ್ಯ)
{
	entry->block = ext2fs_cpu_to_le32(ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ dx_set_ಎಣಿಕೆ(ಸಂರಚನಾ ext2_dx_entry *entries, unsigned ಮೌಲ್ಯ)
{
	((ಸಂರಚನಾ ext2_dx_countlimit *) entries)->ಎಣಿಕೆ = ext2fs_cpu_to_le16(ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ dx_set_limit(ಸಂರಚನಾ ext2_dx_entry *entries, unsigned ಮೌಲ್ಯ)
{
	((ಸಂರಚನಾ ext2_dx_countlimit *) entries)->limit = ext2fs_cpu_to_le16(ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned dx_ಬೇರು_limit(ext2_filsys fs, unsigned infosize)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ entry_space = fs->blocksize -
			ext2fs_dir_rec_len(1, 0) -
			ext2fs_dir_rec_len(2, 0) - infosize;

	ಯದಿ (ext2fs_has_feature_metadata_csum(fs->super))
		entry_space -= ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_dx_tail);
	ಪ್ರತ್ಯಾಗಮ entry_space / ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_dx_entry);
}

/*
 * This converts a one block unindexed directory to a 3 block indexed
 * directory, and adds the dentry to the indexed directory.  Returns 0 if the
 * index was not created; EAGAIN if it was; or an errcode_t on error.
 */
ಸ್ಥಿರ errcode_t try_make_indexed_dir(ext2_filsys fs, ext2_ino_t dir,
				      ಧ್ರುವ ವರ್ಣ *name, ext2_ino_t ino,
				      ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ ext2_inode inode;
	ವರ್ಣ *buf0, *buf1, *ತುದಿ;
	ಸಂರಚನಾ dx_ಬೇರು *ಬೇರು;
	ಸಂರಚನಾ ext2_dx_entry *entries;
	ಸಂರಚನಾ fake_dirent *fde;
	ಸಂರಚನಾ ext2_dir_entry *de, *de2;
	blk64_t pblk0, pblk1;
	unsigned ಪೂರ್ಣಾಙ್ಕ len;
	ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ blocksize = fs->blocksize;
	ಪೂರ್ಣಾಙ್ಕ csum_size = 0;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	ಯದಿ (inode.i_size > fs->blocksize || (inode.i_flags & EXT2_INDEX_FL))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = ಪರಿಮಾಣ(ಸಂರಚನಾ ext2_dir_entry_tail);

	retval = ext2fs_get_mem(blocksize, &buf0);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	retval = ext2fs_get_mem(blocksize, &buf1);
	ಯದಿ (retval)
		ಗಚ್ಛ out_buf0;

	retval = ext2fs_bmap2(fs, dir, &inode, ರಿಕ್ತ, 0, 0, ರಿಕ್ತ, &pblk0);
	ಯದಿ (retval)
		ಗಚ್ಛ out_buf1;

	retval = ext2fs_read_dir_block4(fs, pblk0, buf0, 0, dir);
	ಯದಿ (retval)
		ಗಚ್ಛ out_buf1;

	ಬೇರು = (ಸಂರಚನಾ dx_ಬೇರು *)buf0;
	ಯದಿ (!check_dx_ಬೇರು(fs, dir, ಬೇರು)) {
		retval = EXT2_ET_DIR_CORRUPTED;
		ಗಚ್ಛ out_buf1;
	}

	/* The 0th block becomes the root, move the dirents out */
	fde = &ಬೇರು->dotdot;
	de = ext2fs_next_entry(fs, (ಸಂರಚನಾ ext2_dir_entry *)fde);
	len = ((ವರ್ಣ *) ಬೇರು) + (blocksize - csum_size) - (ವರ್ಣ *) de;

	/* Allocate new block for the 0th block's dirents */
	retval = ext2fs_bmap2(fs, dir, &inode, ರಿಕ್ತ, BMAP_ALLOC | BMAP_ZERO, 1,
			      ರಿಕ್ತ, &pblk1);
	ಯದಿ (retval)
		ಗಚ್ಛ out_buf1;

	memcpy(buf1, de, len);
	memset(de, 0, len); /* wipe old data */

	de = (ಸಂರಚನಾ ext2_dir_entry *)buf1;
	ತುದಿ = buf1 + len;
	ಯಾವತ್ ((ವರ್ಣ *)(de2 = ext2fs_next_entry(fs, de)) < ತುದಿ) {
#ಯದಿ 0
		ಯದಿ (ext4_check_dir_entry(dir, ರಿಕ್ತ, de, bh2, buf1, len,
					(ವರ್ಣ *)de - buf1)) {
			retval = EXT2_ET_DIR_CORRUPTED;
			ಗಚ್ಛ out_buf1;
		}
#endif
		de = de2;
	}
	retval = ext2fs_set_rec_len(fs,
			buf1 + (blocksize - csum_size) - (ವರ್ಣ *) de, de);
	ಯದಿ (retval)
		ಗಚ್ಛ out_buf1;

	ಯದಿ (csum_size)
		ext2fs_initialize_dirent_tail(fs,
				EXT2_DIRENT_TAIL(buf1, fs->blocksize));

	/* Initialize the root; the dot dirents already exist */
	de = (ಸಂರಚನಾ ext2_dir_entry *) (&ಬೇರು->dotdot);
	retval = ext2fs_set_rec_len(fs, blocksize - ext2fs_dir_rec_len(2, 0),
				    de);
	memset (&ಬೇರು->info, 0, ಪರಿಮಾಣ(ಬೇರು->info));
	ಬೇರು->info.info_length = ಪರಿಮಾಣ(ಬೇರು->info);
	ಯದಿ (ext4_hash_in_dirent(&inode))
		ಬೇರು->info.hash_version = EXT2_HASH_SIPHASH;
	ಅನ್ಯಥಾ
		ಬೇರು->info.hash_version = fs->super->s_def_hash_version;

	entries = ಬೇರು->entries;
	dx_set_block(entries, 1);
	dx_set_ಎಣಿಕೆ(entries, 1);
	dx_set_limit(entries, dx_ಬೇರು_limit(fs, ಪರಿಮಾಣ(ಬೇರು->info)));

	retval = ext2fs_write_dir_block4(fs, pblk1, buf1, 0, dir);
	ಯದಿ (retval)
		ಗಚ್ಛ out_buf1;

	retval = ext2fs_write_dir_block4(fs, pblk0, buf0, 0, dir);
	ಯದಿ (retval)
		ಗಚ್ಛ out_buf1;

	inode.i_flags |= EXT2_INDEX_FL;
	inode.i_size += fs->blocksize;
	retval = ext2fs_write_inode(fs, dir, &inode);
	ಯದಿ (retval)
		ಗಚ್ಛ out_buf1;

	retval = EAGAIN;
out_buf1:
	ext2fs_free_mem(&buf1);
out_buf0:
	ext2fs_free_mem(&buf0);
	ಪ್ರತ್ಯಾಗಮ retval;
}

/*
 * Note: the low 3 bits of the flags field are used as the directory
 * entry filetype.
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, ಧ್ರುವ ವರ್ಣ *name,
		      ext2_ino_t ino, ಪೂರ್ಣಾಙ್ಕ flags)
{
	errcode_t		retval;
	ಸಂರಚನಾ link_struct	ls;
	ಸಂರಚನಾ ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಯದಿ (!(fs->flags & EXT2_FLAG_RW))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_RO_FILSYS;

retry:
	ಯದಿ ((retval = ext2fs_read_inode(fs, dir, &inode)) != 0)
		ಪ್ರತ್ಯಾಗಮ retval;

	ಯದಿ (inode.i_flags & EXT2_INDEX_FL)
		ಪ್ರತ್ಯಾಗಮ dx_link(fs, dir, &inode, name, ino, flags);

	ls.fs = fs;
	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	ಯದಿ ((flags & EXT2FS_LINK_APPEND) &&
	    !(inode.i_flags & EXT4_INLINE_DATA_FL)) {
		blk64_t		lblk, pblk = 0;
		ಸಂರಚನಾ dir_context ctx;

		lblk = (inode.i_size / fs->blocksize) - 1;
		retval = ext2fs_bmap2(fs, dir, &inode, ರಿಕ್ತ, 0, lblk,
				      ರಿಕ್ತ, &pblk);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;

		ctx.dir = dir;
		ctx.flags = DIRENT_FLAG_INCLUDE_EMPTY;
		ctx.func = link_proc;
		ctx.priv_data = &ls;
		ctx.errcode = 0;
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;

		ext2fs_process_dir_block(fs, &pblk, lblk, 0, 0, &ctx);
		retval = ctx.errcode;
		ext2fs_free_mem(&ctx.buf);
	} ಅನ್ಯಥಾ {
		retval = ext2fs_dir_iterate2(fs, dir,
					     DIRENT_FLAG_INCLUDE_EMPTY,
					     ರಿಕ್ತ, link_proc, &ls);
	}
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;
	ಯದಿ (ls.err)
		ಪ್ರತ್ಯಾಗಮ ls.err;

	ಯದಿ (!ls.done) {
		ಯದಿ (!(flags & EXT2FS_LINK_EXPAND))
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_DIR_NO_SPACE;

		ಯದಿ (ext2fs_has_feature_dir_index(fs->super)) {
			retval = try_make_indexed_dir(fs, dir, name, ino,
						      flags);
			ಯದಿ (retval == EAGAIN)
				ಗಚ್ಛ retry;
			ಯದಿ (retval)
				ಪ್ರತ್ಯಾಗಮ retval;
		}

		retval = ext2fs_expand_dir(fs, dir);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
		ಗಚ್ಛ retry;
	}
	ಪ್ರತ್ಯಾಗಮ 0;
}

/* Does this directory have an htree index? */
ಪೂರ್ಣಾಙ್ಕ ext2fs_dir_is_dx(ext2_filsys fs, ಧ್ರುವ ಸಂರಚನಾ ext2_inode *inode)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_has_feature_dir_index(fs->super) &&
		S_ISDIR(inode->i_mode) && (inode->i_flags & EXT2_INDEX_FL);
}

/*
 * Set directory link count to 1 if nlinks > EXT2_LINK_MAX, or if nlinks == 2
 * since this indicates that nlinks count was previously 1 to avoid overflowing
 * the 16-bit i_links_count field on disk.  Directories with i_nlink == 1 mean
 * that subdirectory link counts are not being maintained accurately.
 *
 * The caller has already checked for i_nlink overflow in case the DIR_LINK
 * feature is not enabled and returned -EMLINK.  The is_dx() check is a proxy
 * for checking S_ISDIR(inode) (since the INODE_INDEX feature will not be set
 * on regular files) and to avoid creating huge/slow non-HTREE directories.
 */
ಶೂನ್ಯ ext2fs_inc_nlink(ext2_filsys fs, ಸಂರಚನಾ ext2_inode *inode)
{
	inode->i_links_ಎಣಿಕೆ++;

	ಯದಿ (ext2fs_dir_is_dx(fs, inode) &&
	    (inode->i_links_ಎಣಿಕೆ > EXT2_LINK_MAX || inode->i_links_ಎಣಿಕೆ == 2))
		inode->i_links_ಎಣಿಕೆ = 1;
}

/*
 * If a directory had nlink == 1, then we should let it be 1. This indicates
 * directory has >EXT2_LINK_MAX subdirs.
 */
ಶೂನ್ಯ ext2fs_dec_nlink(ಸಂರಚನಾ ext2_inode *inode)
{
	ಯದಿ (!S_ISDIR(inode->i_mode) || inode->i_links_ಎಣಿಕೆ > 2)
		inode->i_links_ಎಣಿಕೆ--;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_dir_link_max(ext2_filsys fs, ಸಂರಚನಾ ext2_inode_large *inode)
{
	ಪ್ರತ್ಯಾಗಮ inode->i_links_ಎಣಿಕೆ >= EXT2_LINK_MAX &&
	       !(ext2fs_dir_is_dx(fs, EXT2_INODE(inode)) &&
		 ext2fs_has_feature_dir_nlink(fs->super));
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_dir_link_empty(ಸಂರಚನಾ ext2_inode *inode)
{
	ಪ್ರತ್ಯಾಗಮ inode->i_links_ಎಣಿಕೆ == 2 || inode->i_links_ಎಣಿಕೆ == 1;
}
