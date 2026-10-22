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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"
#அந்தர்பாவ "ext2fsP.h"

#define EXT2_DX_வேர்_OFF 24

கட்டமைப்பு dx_frame {
	வெறுமை *buf;
	blk64_t pblock;
	கட்டமைப்பு ext2_dx_countlimit *head;
	கட்டமைப்பு ext2_dx_entry *entries;
	கட்டமைப்பு ext2_dx_entry *at;
};

கட்டமைப்பு dx_lookup_info {
	மாறிலி எழுத்து *name;
	முழு namelen;
	முழு hash_alg;
	__u32 hash;
	unsigned levels;
	கட்டமைப்பு dx_frame frames[EXT4_HTREE_LEVEL];
};

நிலையான errcode_t alloc_dx_frame(ext2_filsys fs, கட்டமைப்பு dx_frame *frame)
{
	திரும்பு ext2fs_get_mem(fs->blocksize, &frame->buf);
}

நிலையான வெறுமை dx_release(கட்டமைப்பு dx_lookup_info *info)
{
	unsigned level;

	க்கு (level = 0; level < info->levels; level++) {
		எனில் (info->frames[level].buf == வெற்று)
			நிறுத்து;
		ext2fs_free_mem(&(info->frames[level].buf));
	}
	info->levels = 0;
}

நிலையான வெறுமை dx_தேடல்_entry(கட்டமைப்பு dx_frame *frame, முழு மொத்தம், __u32 hash)
{
	கட்டமைப்பு ext2_dx_entry *p, *q, *m;

	p = frame->entries + 1;
	q = frame->entries + மொத்தம் - 1;
	போது (p <= q) {
		m = p + (q - p) / 2;
		எனில் (ext2fs_le32_to_cpu(m->hash) > hash)
			q = m - 1;
		இல்லையெனில்
			p = m + 1;
	}
	frame->at = p - 1;
}

நிலையான errcode_t load_logical_dir_block(ext2_filsys fs, ext2_ino_t dir,
					கட்டமைப்பு ext2_inode *diri, blk64_t block,
					blk64_t *pblk, வெறுமை *buf)
{
	errcode_t errcode;
	முழு ret_flags;

	errcode = ext2fs_bmap2(fs, dir, diri, வெற்று, 0, block, &ret_flags,
			       pblk);
	எனில் (errcode)
		திரும்பு errcode;
	எனில் (ret_flags & BMAP_RET_UNINIT)
		திரும்பு EXT2_ET_DIR_CORRUPTED;
	திரும்பு ext2fs_read_dir_block4(fs, *pblk, buf, 0, dir);
}

நிலையான errcode_t dx_lookup(ext2_filsys fs, ext2_ino_t dir,
			   கட்டமைப்பு ext2_inode *diri, கட்டமைப்பு dx_lookup_info *info)
{
	கட்டமைப்பு ext2_dx_வேர்_info *வேர்;
	errcode_t errcode;
	முழு level = 0;
	முழு மொத்தம், limit;
	முழு hash_alg;
	முழு hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	__u32 minor_hash;
	கட்டமைப்பு dx_frame *frame;

	errcode = alloc_dx_frame(fs, &(info->frames[0]));
	எனில் (errcode)
		திரும்பு errcode;
	info->levels = 1;

	errcode = load_logical_dir_block(fs, dir, diri, 0,
					 &(info->frames[0].pblock),
					 info->frames[0].buf);
	எனில் (errcode)
		செல் out_err;
	வேர் = (கட்டமைப்பு ext2_dx_வேர்_info *) ((எழுத்து *)info->frames[0].buf +
					     EXT2_DX_வேர்_OFF);
	hash_alg = வேர்->hash_version;
	எனில் (hash_alg != EXT2_HASH_TEA && hash_alg != EXT2_HASH_HALF_MD4 &&
	    hash_alg != EXT2_HASH_LEGACY) {
		errcode = EXT2_ET_DIRHASH_UNSUPP;
		செல் out_err;
	}
	எனில் (hash_alg <= EXT2_HASH_TEA &&
	    fs->super->s_flags & EXT2_FLAGS_UNSIGNED_HASH)
		hash_alg += 3;
	எனில் (வேர்->indirect_levels >= ext2_dir_htree_level(fs)) {
		errcode = EXT2_ET_DIR_CORRUPTED;
		செல் out_err;
	}
	info->hash_alg = hash_alg;

	errcode = ext2fs_dirhash2(hash_alg, info->name, info->namelen,
				  fs->encoding, hash_flags,
				  fs->super->s_hash_seed, &info->hash,
				  &minor_hash);
	எனில் (errcode)
		செல் out_err;

	க்கு (level = 0; level <= வேர்->indirect_levels; level++) {
		frame = &(info->frames[level]);
		எனில் (level > 0) {
			errcode = alloc_dx_frame(fs, frame);
			எனில் (errcode)
				செல் out_err;
			info->levels++;

			errcode = load_logical_dir_block(fs, dir, diri,
				ext2fs_le32_to_cpu(info->frames[level-1].at->block) & 0x0fffffff,
				&(frame->pblock), frame->buf);
			எனில் (errcode)
				செல் out_err;
		}
		errcode = ext2fs_get_dx_countlimit(fs, frame->buf,
						   &(frame->head), வெற்று);
		எனில் (errcode)
			செல் out_err;
		மொத்தம் = ext2fs_le16_to_cpu(frame->head->மொத்தம்);
		limit = ext2fs_le16_to_cpu(frame->head->limit);
		frame->entries = (கட்டமைப்பு ext2_dx_entry *)(frame->head);
		எனில் (!மொத்தம் || மொத்தம் > limit) {
			errcode = EXT2_ET_DIR_CORRUPTED;
			செல் out_err;
		}

		dx_தேடல்_entry(frame, மொத்தம், info->hash);
	}
	திரும்பு 0;
out_err:
	dx_release(info);
	திரும்பு errcode;
}

கட்டமைப்பு link_struct  {
	ext2_filsys	fs;
	மாறிலி எழுத்து	*name;
	முழு		namelen;
	ext2_ino_t	inode;
	முழு		flags;
	முழு		done;
	unsigned முழு	blocksize;
	errcode_t	err;
	கட்டமைப்பு ext2_super_block *sb;
};

நிலையான முழு link_proc(ext2_ino_t dir EXT2FS_ATTR((unused)),
		     முழு entru EXT2FS_ATTR((unused)),
		     கட்டமைப்பு ext2_dir_entry *dirent,
		     முழு	offset,
		     முழு	blocksize,
		     எழுத்து	*buf,
		     வெறுமை	*priv_data)
{
	கட்டமைப்பு link_struct *ls = (கட்டமைப்பு link_struct *) priv_data;
	கட்டமைப்பு ext2_dir_entry *next;
	unsigned முழு rec_len, min_rec_len, curr_rec_len;
	முழு ret = 0;
	முழு csum_size = 0;

	எனில் (ls->done)
		திரும்பு DIRENT_ABORT;

	rec_len = EXT2_DIR_REC_LEN(ls->namelen);

	ls->err = ext2fs_get_rec_len(ls->fs, dirent, &curr_rec_len);
	எனில் (ls->err)
		திரும்பு DIRENT_ABORT;

	எனில் (ext2fs_has_feature_metadata_csum(ls->fs->super))
		csum_size = அளவு(கட்டமைப்பு ext2_dir_entry_tail);
	/*
	 * See if the following directory entry (if any) is unused;
	 * if so, absorb it into this one.
	 */
	next = (கட்டமைப்பு ext2_dir_entry *) (buf + offset + curr_rec_len);
	எனில் ((offset + (முழு) curr_rec_len < blocksize - (8 + csum_size)) &&
	    (next->inode == 0) &&
	    (offset + (முழு) curr_rec_len + (முழு) next->rec_len <= blocksize)) {
		curr_rec_len += next->rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, curr_rec_len, dirent);
		எனில் (ls->err)
			திரும்பு DIRENT_ABORT;
		ret = DIRENT_CHANGED;
	}

	/*
	 * If the directory entry is used, see if we can split the
	 * directory entry to make room for the new name.  If so,
	 * truncate it and return.
	 */
	எனில் (dirent->inode) {
		min_rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(dirent));
		எனில் (curr_rec_len < (min_rec_len + rec_len))
			திரும்பு ret;
		rec_len = curr_rec_len - min_rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, min_rec_len, dirent);
		எனில் (ls->err)
			திரும்பு DIRENT_ABORT;
		next = (கட்டமைப்பு ext2_dir_entry *) (buf + offset +
						  dirent->rec_len);
		next->inode = 0;
		ext2fs_dirent_set_name_len(next, 0);
		ext2fs_dirent_set_file_type(next, 0);
		ls->err = ext2fs_set_rec_len(ls->fs, rec_len, next);
		எனில் (ls->err)
			திரும்பு DIRENT_ABORT;
		திரும்பு DIRENT_CHANGED;
	}

	/*
	 * If we get this far, then the directory entry is not used.
	 * See if we can fit the request entry in.  If so, do it.
	 */
	எனில் (curr_rec_len < rec_len)
		திரும்பு ret;
	dirent->inode = ls->inode;
	ext2fs_dirent_set_name_len(dirent, ls->namelen);
	strncpy(dirent->name, ls->name, ls->namelen);
	எனில் (ext2fs_has_feature_filetype(ls->sb))
		ext2fs_dirent_set_file_type(dirent,
					    ls->flags & EXT2FS_LINK_FT_MASK);

	ls->done++;
	திரும்பு DIRENT_ABORT|DIRENT_CHANGED;
}

நிலையான errcode_t add_dirent_to_buf(ext2_filsys fs, e2_blkcnt_t blockcnt,
				   எழுத்து *buf, ext2_ino_t dir,
				   கட்டமைப்பு ext2_inode *diri, மாறிலி எழுத்து *name,
				   ext2_ino_t ino, முழு flags, blk64_t *pblkp)
{
	கட்டமைப்பு dir_context ctx;
	கட்டமைப்பு link_struct ls;
	errcode_t retval;

	retval = load_logical_dir_block(fs, dir, diri, blockcnt, pblkp, buf);
	எனில் (retval)
		திரும்பு retval;
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
	எனில் (ctx.errcode)
		திரும்பு ctx.errcode;
	எனில் (ls.err)
		திரும்பு ls.err;
	எனில் (!ls.done)
		திரும்பு EXT2_ET_DIR_NO_SPACE;
	திரும்பு 0;
}

கட்டமைப்பு dx_hash_map {
	__u32 hash;
	முழு size;
	முழு off;
};

நிலையான EXT2_QSORT_TYPE dx_hash_map_cmp(மாறிலி வெறுமை *ap, மாறிலி வெறுமை *bp)
{
	மாறிலி கட்டமைப்பு dx_hash_map *a = ap, *b = bp;

	எனில் (a->hash < b->hash)
		திரும்பு -1;
	எனில் (a->hash > b->hash)
		திரும்பு 1;
	திரும்பு 0;
}

நிலையான errcode_t dx_move_dirents(ext2_filsys fs, கட்டமைப்பு dx_hash_map *map,
				 முழு மொத்தம், வெறுமை *from, வெறுமை *to)
{
	கட்டமைப்பு ext2_dir_entry *de;
	முழு i;
	முழு rec_len = 0;
	errcode_t retval;
	முழு csum_size = 0;
	வெறுமை *base = to;

	எனில் (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = அளவு(கட்டமைப்பு ext2_dir_entry_tail);

	க்கு (i = 0; i < மொத்தம்; i++) {
		de = (கட்டமைப்பு ext2_dir_entry *) ((எழுத்து *)from + map[i].off);
		rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(de));
		memcpy(to, de, rec_len);
		retval = ext2fs_set_rec_len(fs, rec_len, to);
		எனில் (retval)
			திரும்பு retval;
		to = (எழுத்து *)to + rec_len;
	}
	/*
	 * Update rec_len of the last dir entry to stretch to the end of block
	 */
	to = (எழுத்து *)to - rec_len;
	rec_len = fs->blocksize - ((எழுத்து *)to - (எழுத்து *)base) - csum_size;
	retval = ext2fs_set_rec_len(fs, rec_len, to);
	எனில் (retval)
		திரும்பு retval;
	எனில் (csum_size)
		ext2fs_initialize_dirent_tail(fs,
				EXT2_DIRENT_TAIL(base, fs->blocksize));
	திரும்பு 0;
}

நிலையான errcode_t dx_செருகு_entry(ext2_filsys fs, ext2_ino_t dir,
				 கட்டமைப்பு dx_lookup_info *info, முழு level,
				 __u32 hash, blk64_t lblk)
{
	முழு pcount;
	கட்டமைப்பு ext2_dx_entry *உச்சி, *new;

	pcount = ext2fs_le16_to_cpu(info->frames[level].head->மொத்தம்);
	உச்சி = info->frames[level].entries + pcount;
	new = info->frames[level].at + 1;
	memmove(new + 1, new, (எழுத்து *)உச்சி - (எழுத்து *)new);
	new->hash = ext2fs_cpu_to_le32(hash);
	new->block = ext2fs_cpu_to_le32(lblk);
	info->frames[level].head->மொத்தம் = ext2fs_cpu_to_le16(pcount + 1);
	திரும்பு ext2fs_write_dir_block4(fs, info->frames[level].pblock,
				       info->frames[level].buf, 0, dir);
}

நிலையான errcode_t dx_split_leaf(ext2_filsys fs, ext2_ino_t dir,
			       கட்டமைப்பு ext2_inode *diri,
			       கட்டமைப்பு dx_lookup_info *info, வெறுமை *buf,
			       blk64_t leaf_pblk, blk64_t new_lblk,
			       blk64_t new_pblk)
{
	முழு hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	கட்டமைப்பு ext2_dir_entry *de;
	வெறுமை *buf2;
	errcode_t retval = 0;
	unsigned முழு rec_len;
	unsigned முழு offset, move_size;
	முழு i, மொத்தம் = 0;
	கட்டமைப்பு dx_hash_map *map;
	முழு continued;
	__u32 minor_hash;

	retval = ext2fs_get_mem(fs->blocksize, &buf2);
	எனில் (retval)
		திரும்பு retval;
	retval = ext2fs_get_array(fs->blocksize / 12,
				  அளவு(கட்டமைப்பு dx_hash_map), &map);
	எனில் (retval) {
		ext2fs_free_mem(&buf2);
		திரும்பு retval;
	}
	க்கு (offset = 0; offset < fs->blocksize; offset += rec_len) {
		de = (கட்டமைப்பு ext2_dir_entry *) ((எழுத்து *)buf + offset);
		retval = ext2fs_get_rec_len(fs, de, &rec_len);
		எனில் (retval)
			செல் out;
		எனில் (ext2fs_dirent_name_len(de) > 0 && de->inode) {
			map[மொத்தம்].off = offset;
			map[மொத்தம்].size = rec_len;
			retval = ext2fs_dirhash2(info->hash_alg, de->name,
					ext2fs_dirent_name_len(de),
					fs->encoding, hash_flags,
					fs->super->s_hash_seed,
					&(map[மொத்தம்].hash),
					&minor_hash);
			எனில் (retval)
				செல் out;
			மொத்தம்++;
		}
	}
	qsort(map, மொத்தம், அளவு(கட்டமைப்பு dx_hash_map), dx_hash_map_cmp);
	move_size = 0;
	/* Find place to split block */
	க்கு (i = மொத்தம் - 1; i >= 0; i--) {
		எனில் (move_size + map[i].size / 2 > fs->blocksize / 2)
			நிறுத்து;
		move_size += map[i].size;
	}
	/* Let i be the first entry to move */
	i++;
	/* Move selected directory entries to new block */
	retval = dx_move_dirents(fs, map + i, மொத்தம் - i, buf, buf2);
	எனில் (retval)
		செல் out;
	retval = ext2fs_write_dir_block4(fs, new_pblk, buf2, 0, dir);
	எனில் (retval)
		செல் out;
	/* Repack remaining entries in the old block */
	retval = dx_move_dirents(fs, map, i, buf, buf2);
	எனில் (retval)
		செல் out;
	retval = ext2fs_write_dir_block4(fs, leaf_pblk, buf2, 0, dir);
	எனில் (retval)
		செல் out;
	/* Update parent node */
	continued = map[i].hash == map[i-1].hash;
	retval = dx_செருகு_entry(fs, dir, info, info->levels - 1,
				 map[i].hash + continued, new_lblk);
out:
	ext2fs_free_mem(&buf2);
	ext2fs_free_mem(&map);
	திரும்பு retval;
}

நிலையான errcode_t dx_grow_மரம்(ext2_filsys fs, ext2_ino_t dir,
			      கட்டமைப்பு ext2_inode *diri,
			      கட்டமைப்பு dx_lookup_info *info, வெறுமை *buf,
			      blk64_t leaf_pblk)
{
	முழு i;
	errcode_t retval;
	ext2_off64_t size = EXT2_I_SIZE(diri);
	blk64_t lblk, pblk;
	கட்டமைப்பு ext2_dir_entry *de;
	கட்டமைப்பு ext2_dx_countlimit *head;
	முழு csum_size = 0;
	முழு மொத்தம்;

	எனில் (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = அளவு(கட்டமைப்பு ext2_dx_tail);

	/* Find level which can accommodate new child */
	க்கு (i = info->levels - 1; i >= 0; i--)
		எனில் (ext2fs_le16_to_cpu(info->frames[i].head->மொத்தம்) <
		    ext2fs_le16_to_cpu(info->frames[i].head->limit))
			நிறுத்து;
	/* Need to grow tree depth? */
	எனில் (i < 0 && info->levels >= ext2_dir_htree_level(fs))
		திரும்பு EXT2_ET_DIR_NO_SPACE;
	lblk = size / fs->blocksize;
	size += fs->blocksize;
	retval = ext2fs_inode_size_set(fs, diri, size);
	எனில் (retval)
		திரும்பு retval;
	retval = ext2fs_fallocate(fs,
			EXT2_FALLOCATE_FORCE_INIT | EXT2_FALLOCATE_ZERO_BLOCKS,
			dir, diri, 0, lblk, 1);
	எனில் (retval)
		திரும்பு retval;
	retval = ext2fs_write_inode(fs, dir, diri);
	எனில் (retval)
		திரும்பு retval;
	retval = ext2fs_bmap2(fs, dir, diri, வெற்று, 0, lblk, வெற்று, &pblk);
	எனில் (retval)
		திரும்பு retval;
	/* Only leaf addition needed? */
	எனில் (i == (முழு)info->levels - 1)
		திரும்பு dx_split_leaf(fs, dir, diri, info, buf, leaf_pblk,
				     lblk, pblk);

	de = buf;
	de->inode = 0;
	ext2fs_dirent_set_name_len(de, 0);
	ext2fs_dirent_set_file_type(de, 0);
	retval = ext2fs_set_rec_len(fs, fs->blocksize, de);
	எனில் (retval)
		திரும்பு retval;
	head = (கட்டமைப்பு ext2_dx_countlimit *) ((எழுத்து *)buf + 8);
	மொத்தம் = ext2fs_le16_to_cpu(info->frames[i+1].head->மொத்தம்);
	/* Growing tree depth? */
	எனில் (i < 0) {
		கட்டமைப்பு ext2_dx_வேர்_info *வேர்;

		memcpy(head, info->frames[0].entries,
		       மொத்தம் * அளவு(கட்டமைப்பு ext2_dx_entry));
		head->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_size)) /
				அளவு(கட்டமைப்பு ext2_dx_entry));
		/* head->count gets set by memcpy above to correct value */

		/* Now update tree root */
		info->frames[0].head->மொத்தம் = ext2fs_cpu_to_le16(1);
		info->frames[0].entries[0].block = ext2fs_cpu_to_le32(lblk);
		வேர் = (கட்டமைப்பு ext2_dx_வேர்_info *)
			((எழுத்து *)info->frames[0].buf + EXT2_DX_வேர்_OFF);
		வேர்->indirect_levels++;
	} இல்லையெனில் {
		/* Splitting internal node in two */
		முழு count1 = மொத்தம் / 2;
		முழு count2 = மொத்தம் - count1;
		__u32 split_hash = ext2fs_le32_to_cpu(info->frames[i+1].entries[count1].hash);

		memcpy(head, info->frames[i+1].entries + count1,
		       count2 * அளவு(கட்டமைப்பு ext2_dx_entry));
		head->மொத்தம் = ext2fs_cpu_to_le16(count2);
		head->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_size)) /
				அளவு(கட்டமைப்பு ext2_dx_entry));
		info->frames[i+1].head->மொத்தம் = ext2fs_cpu_to_le16(count1);

		/* Update parent node */
		retval = dx_செருகு_entry(fs, dir, info, i, split_hash, lblk);
		எனில் (retval)
			திரும்பு retval;

	}
	/* Writeout split block / updated root */
	retval = ext2fs_write_dir_block4(fs, info->frames[i+1].pblock,
					 info->frames[i+1].buf, 0, dir);
	எனில் (retval)
		திரும்பு retval;
	/* Writeout new tree block */
	retval = ext2fs_write_dir_block4(fs, pblk, buf, 0, dir);
	எனில் (retval)
		திரும்பு retval;
	திரும்பு 0;
}

நிலையான errcode_t dx_link(ext2_filsys fs, ext2_ino_t dir,
			 கட்டமைப்பு ext2_inode *diri, மாறிலி எழுத்து *name,
			 ext2_ino_t ino, முழு flags)
{
	கட்டமைப்பு dx_lookup_info dx_info;
	errcode_t retval;
	வெறுமை *blockbuf;
	unsigned restart = 0;
	blk64_t leaf_pblk;

	retval = ext2fs_get_mem(fs->blocksize, &blockbuf);
	எனில் (retval)
		திரும்பு retval;

	dx_info.name = name;
	dx_info.namelen = strlen(name);
again:
	retval = dx_lookup(fs, dir, diri, &dx_info);
	எனில் (retval)
		செல் free_buf;

	retval = add_dirent_to_buf(fs,
		ext2fs_le32_to_cpu(dx_info.frames[dx_info.levels-1].at->block) & 0x0fffffff,
		blockbuf, dir, diri, name, ino, flags, &leaf_pblk);
	/*
	 * Success or error other than ENOSPC...? We are done. We may need upto
	 * two tries to add entry. One to split htree node and another to add
	 * new leaf block.
	 */
	எனில் (restart >= dx_info.levels || retval != EXT2_ET_DIR_NO_SPACE)
		செல் free_frames;
	retval = dx_grow_மரம்(fs, dir, diri, &dx_info, blockbuf, leaf_pblk);
	எனில் (retval)
		செல் free_frames;
	/* Restart everything now that the tree is larger */
	restart++;
	dx_release(&dx_info);
	செல் again;
free_frames:
	dx_release(&dx_info);
free_buf:
	ext2fs_free_mem(&blockbuf);
	திரும்பு retval;
}

கட்டமைப்பு fake_dirent
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

கட்டமைப்பு dx_வேர்
{
	கட்டமைப்பு fake_dirent dot;
	எழுத்து dot_name[4];
	கட்டமைப்பு fake_dirent dotdot;
	எழுத்து dotdot_name[4];
	கட்டமைப்பு ext2_dx_வேர்_info info;
	கட்டமைப்பு ext2_dx_entry entries[];
};

நிலையான முழு check_dx_வேர்(ext2_filsys fs, ext2_ino_t ino, கட்டமைப்பு dx_வேர் *வேர்)
{
	கட்டமைப்பு fake_dirent *fde;
	எழுத்து *error_செய்தி;
	unsigned முழு rlen;
	எழுத்து *blockend = (எழுத்து *)வேர் + fs->blocksize;

	fde = &வேர்->dot;
	எனில் (fde->name_len != 1) {
		error_செய்தி = "invalid name_len for '.'";
		செல் corrupted;
	}
	எனில் (strncmp(வேர்->dot_name, ".", fde->name_len)) {
		error_செய்தி = "invalid name for '.'";
		செல் corrupted;
	}

	ext2fs_get_rec_len(fs, (கட்டமைப்பு ext2_dir_entry *)fde, &rlen);
	எனில் ((எழுத்து *)fde + rlen >= blockend) {
		error_செய்தி = "invalid rec_len for '.'";
		செல் corrupted;
	}

	fde = &வேர்->dotdot;
	எனில் (fde->name_len != 2) {
		error_செய்தி = "invalid name_len for '..'";
		செல் corrupted;
	}
	எனில் (strncmp(வேர்->dotdot_name, "..", fde->name_len)) {
		error_செய்தி = "invalid name for '..'";
		செல் corrupted;
	}
	ext2fs_get_rec_len(fs, (கட்டமைப்பு ext2_dir_entry *)fde, &rlen);
	எனில் ((எழுத்து *)fde + rlen >= blockend) {
		error_செய்தி = "invalid rec_len for '..'";
		செல் corrupted;
	}

	திரும்பு 1;

corrupted:
	fprintf(stderr, "Corrupt dir %u: %s, running e2fsck is recommended\n",
			 ino, error_செய்தி);
	திரும்பு 0;
}

நிலையான உள்வரி கட்டமைப்பு ext2_dir_entry *
ext2fs_next_entry(ext2_filsys fs, கட்டமைப்பு ext2_dir_entry *p)
{
	unsigned முழு rlen;

	ext2fs_get_rec_len(fs, p, &rlen);
	திரும்பு (கட்டமைப்பு ext2_dir_entry *)((எழுத்து *)p + rlen);
}

நிலையான உள்வரி வெறுமை dx_set_block(கட்டமைப்பு ext2_dx_entry *entry, unsigned மதிப்பு)
{
	entry->block = ext2fs_cpu_to_le32(மதிப்பு);
}

நிலையான உள்வரி வெறுமை dx_set_மொத்தம்(கட்டமைப்பு ext2_dx_entry *entries, unsigned மதிப்பு)
{
	((கட்டமைப்பு ext2_dx_countlimit *) entries)->மொத்தம் = ext2fs_cpu_to_le16(மதிப்பு);
}

நிலையான உள்வரி வெறுமை dx_set_limit(கட்டமைப்பு ext2_dx_entry *entries, unsigned மதிப்பு)
{
	((கட்டமைப்பு ext2_dx_countlimit *) entries)->limit = ext2fs_cpu_to_le16(மதிப்பு);
}

நிலையான உள்வரி unsigned dx_வேர்_limit(ext2_filsys fs, unsigned infosize)
{
	unsigned முழு entry_space = fs->blocksize -
			ext2fs_dir_rec_len(1, 0) -
			ext2fs_dir_rec_len(2, 0) - infosize;

	எனில் (ext2fs_has_feature_metadata_csum(fs->super))
		entry_space -= அளவு(கட்டமைப்பு ext2_dx_tail);
	திரும்பு entry_space / அளவு(கட்டமைப்பு ext2_dx_entry);
}

/*
 * This converts a one block unindexed directory to a 3 block indexed
 * directory, and adds the dentry to the indexed directory.  Returns 0 if the
 * index was not created; EAGAIN if it was; or an errcode_t on error.
 */
நிலையான errcode_t try_make_indexed_dir(ext2_filsys fs, ext2_ino_t dir,
				      மாறிலி எழுத்து *name, ext2_ino_t ino,
				      முழு flags)
{
	கட்டமைப்பு ext2_inode inode;
	எழுத்து *buf0, *buf1, *உச்சி;
	கட்டமைப்பு dx_வேர் *வேர்;
	கட்டமைப்பு ext2_dx_entry *entries;
	கட்டமைப்பு fake_dirent *fde;
	கட்டமைப்பு ext2_dir_entry *de, *de2;
	blk64_t pblk0, pblk1;
	unsigned முழு len;
	மாறிலி unsigned முழு blocksize = fs->blocksize;
	முழு csum_size = 0;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	எனில் (retval)
		திரும்பு retval;

	எனில் (inode.i_size > fs->blocksize || (inode.i_flags & EXT2_INDEX_FL))
		திரும்பு 0;

	எனில் (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = அளவு(கட்டமைப்பு ext2_dir_entry_tail);

	retval = ext2fs_get_mem(blocksize, &buf0);
	எனில் (retval)
		திரும்பு retval;

	retval = ext2fs_get_mem(blocksize, &buf1);
	எனில் (retval)
		செல் out_buf0;

	retval = ext2fs_bmap2(fs, dir, &inode, வெற்று, 0, 0, வெற்று, &pblk0);
	எனில் (retval)
		செல் out_buf1;

	retval = ext2fs_read_dir_block4(fs, pblk0, buf0, 0, dir);
	எனில் (retval)
		செல் out_buf1;

	வேர் = (கட்டமைப்பு dx_வேர் *)buf0;
	எனில் (!check_dx_வேர்(fs, dir, வேர்)) {
		retval = EXT2_ET_DIR_CORRUPTED;
		செல் out_buf1;
	}

	/* The 0th block becomes the root, move the dirents out */
	fde = &வேர்->dotdot;
	de = ext2fs_next_entry(fs, (கட்டமைப்பு ext2_dir_entry *)fde);
	len = ((எழுத்து *) வேர்) + (blocksize - csum_size) - (எழுத்து *) de;

	/* Allocate new block for the 0th block's dirents */
	retval = ext2fs_bmap2(fs, dir, &inode, வெற்று, BMAP_ALLOC | BMAP_ZERO, 1,
			      வெற்று, &pblk1);
	எனில் (retval)
		செல் out_buf1;

	memcpy(buf1, de, len);
	memset(de, 0, len); /* wipe old data */

	de = (கட்டமைப்பு ext2_dir_entry *)buf1;
	உச்சி = buf1 + len;
	போது ((எழுத்து *)(de2 = ext2fs_next_entry(fs, de)) < உச்சி) {
#எனில் 0
		எனில் (ext4_check_dir_entry(dir, வெற்று, de, bh2, buf1, len,
					(எழுத்து *)de - buf1)) {
			retval = EXT2_ET_DIR_CORRUPTED;
			செல் out_buf1;
		}
#endif
		de = de2;
	}
	retval = ext2fs_set_rec_len(fs,
			buf1 + (blocksize - csum_size) - (எழுத்து *) de, de);
	எனில் (retval)
		செல் out_buf1;

	எனில் (csum_size)
		ext2fs_initialize_dirent_tail(fs,
				EXT2_DIRENT_TAIL(buf1, fs->blocksize));

	/* Initialize the root; the dot dirents already exist */
	de = (கட்டமைப்பு ext2_dir_entry *) (&வேர்->dotdot);
	retval = ext2fs_set_rec_len(fs, blocksize - ext2fs_dir_rec_len(2, 0),
				    de);
	memset (&வேர்->info, 0, அளவு(வேர்->info));
	வேர்->info.info_length = அளவு(வேர்->info);
	எனில் (ext4_hash_in_dirent(&inode))
		வேர்->info.hash_version = EXT2_HASH_SIPHASH;
	இல்லையெனில்
		வேர்->info.hash_version = fs->super->s_def_hash_version;

	entries = வேர்->entries;
	dx_set_block(entries, 1);
	dx_set_மொத்தம்(entries, 1);
	dx_set_limit(entries, dx_வேர்_limit(fs, அளவு(வேர்->info)));

	retval = ext2fs_write_dir_block4(fs, pblk1, buf1, 0, dir);
	எனில் (retval)
		செல் out_buf1;

	retval = ext2fs_write_dir_block4(fs, pblk0, buf0, 0, dir);
	எனில் (retval)
		செல் out_buf1;

	inode.i_flags |= EXT2_INDEX_FL;
	inode.i_size += fs->blocksize;
	retval = ext2fs_write_inode(fs, dir, &inode);
	எனில் (retval)
		செல் out_buf1;

	retval = EAGAIN;
out_buf1:
	ext2fs_free_mem(&buf1);
out_buf0:
	ext2fs_free_mem(&buf0);
	திரும்பு retval;
}

/*
 * Note: the low 3 bits of the flags field are used as the directory
 * entry filetype.
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, மாறிலி எழுத்து *name,
		      ext2_ino_t ino, முழு flags)
{
	errcode_t		retval;
	கட்டமைப்பு link_struct	ls;
	கட்டமைப்பு ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	எனில் (!(fs->flags & EXT2_FLAG_RW))
		திரும்பு EXT2_ET_RO_FILSYS;

retry:
	எனில் ((retval = ext2fs_read_inode(fs, dir, &inode)) != 0)
		திரும்பு retval;

	எனில் (inode.i_flags & EXT2_INDEX_FL)
		திரும்பு dx_link(fs, dir, &inode, name, ino, flags);

	ls.fs = fs;
	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	எனில் ((flags & EXT2FS_LINK_APPEND) &&
	    !(inode.i_flags & EXT4_INLINE_DATA_FL)) {
		blk64_t		lblk, pblk = 0;
		கட்டமைப்பு dir_context ctx;

		lblk = (inode.i_size / fs->blocksize) - 1;
		retval = ext2fs_bmap2(fs, dir, &inode, வெற்று, 0, lblk,
				      வெற்று, &pblk);
		எனில் (retval)
			திரும்பு retval;

		ctx.dir = dir;
		ctx.flags = DIRENT_FLAG_INCLUDE_EMPTY;
		ctx.func = link_proc;
		ctx.priv_data = &ls;
		ctx.errcode = 0;
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		எனில் (retval)
			திரும்பு retval;

		ext2fs_process_dir_block(fs, &pblk, lblk, 0, 0, &ctx);
		retval = ctx.errcode;
		ext2fs_free_mem(&ctx.buf);
	} இல்லையெனில் {
		retval = ext2fs_dir_iterate2(fs, dir,
					     DIRENT_FLAG_INCLUDE_EMPTY,
					     வெற்று, link_proc, &ls);
	}
	எனில் (retval)
		திரும்பு retval;
	எனில் (ls.err)
		திரும்பு ls.err;

	எனில் (!ls.done) {
		எனில் (!(flags & EXT2FS_LINK_EXPAND))
			திரும்பு EXT2_ET_DIR_NO_SPACE;

		எனில் (ext2fs_has_feature_dir_index(fs->super)) {
			retval = try_make_indexed_dir(fs, dir, name, ino,
						      flags);
			எனில் (retval == EAGAIN)
				செல் retry;
			எனில் (retval)
				திரும்பு retval;
		}

		retval = ext2fs_expand_dir(fs, dir);
		எனில் (retval)
			திரும்பு retval;
		செல் retry;
	}
	திரும்பு 0;
}

/* Does this directory have an htree index? */
முழு ext2fs_dir_is_dx(ext2_filsys fs, மாறிலி கட்டமைப்பு ext2_inode *inode)
{
	திரும்பு ext2fs_has_feature_dir_index(fs->super) &&
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
வெறுமை ext2fs_inc_nlink(ext2_filsys fs, கட்டமைப்பு ext2_inode *inode)
{
	inode->i_links_மொத்தம்++;

	எனில் (ext2fs_dir_is_dx(fs, inode) &&
	    (inode->i_links_மொத்தம் > EXT2_LINK_MAX || inode->i_links_மொத்தம் == 2))
		inode->i_links_மொத்தம் = 1;
}

/*
 * If a directory had nlink == 1, then we should let it be 1. This indicates
 * directory has >EXT2_LINK_MAX subdirs.
 */
வெறுமை ext2fs_dec_nlink(கட்டமைப்பு ext2_inode *inode)
{
	எனில் (!S_ISDIR(inode->i_mode) || inode->i_links_மொத்தம் > 2)
		inode->i_links_மொத்தம்--;
}

முழு ext2fs_dir_link_max(ext2_filsys fs, கட்டமைப்பு ext2_inode_large *inode)
{
	திரும்பு inode->i_links_மொத்தம் >= EXT2_LINK_MAX &&
	       !(ext2fs_dir_is_dx(fs, EXT2_INODE(inode)) &&
		 ext2fs_has_feature_dir_nlink(fs->super));
}

முழு ext2fs_dir_link_empty(கட்டமைப்பு ext2_inode *inode)
{
	திரும்பு inode->i_links_மொத்தம் == 2 || inode->i_links_மொத்தம் == 1;
}
