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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"
#অন্তর্ভাৱ "ext2fsP.h"

#define EXT2_DX_মূল_OFF 24

সংরচনা dx_frame {
	শূন্য *buf;
	blk64_t pblock;
	সংরচনা ext2_dx_countlimit *head;
	সংরচনা ext2_dx_entry *entries;
	সংরচনা ext2_dx_entry *at;
};

সংরচনা dx_lookup_info {
	ধ্রুৱ ৱর্ণ *name;
	পূর্ণাঙ্ক namelen;
	পূর্ণাঙ্ক hash_alg;
	__u32 hash;
	unsigned levels;
	সংরচনা dx_frame frames[EXT4_HTREE_LEVEL];
};

স্থির errcode_t alloc_dx_frame(ext2_filsys fs, সংরচনা dx_frame *frame)
{
	ঘূৰাই ext2fs_get_mem(fs->blocksize, &frame->buf);
}

স্থির শূন্য dx_release(সংরচনা dx_lookup_info *info)
{
	unsigned level;

	বাবে (level = 0; level < info->levels; level++) {
		যদি (info->frames[level].buf == রিক্ত)
			ভঙ্গ;
		ext2fs_free_mem(&(info->frames[level].buf));
	}
	info->levels = 0;
}

স্থির শূন্য dx_অনুসন্ধান_entry(সংরচনা dx_frame *frame, পূর্ণাঙ্ক গুনতি, __u32 hash)
{
	সংরচনা ext2_dx_entry *p, *q, *m;

	p = frame->entries + 1;
	q = frame->entries + গুনতি - 1;
	যেতিয়ালৈ (p <= q) {
		m = p + (q - p) / 2;
		যদি (ext2fs_le32_to_cpu(m->hash) > hash)
			q = m - 1;
		অন্যথা
			p = m + 1;
	}
	frame->at = p - 1;
}

স্থির errcode_t load_logical_dir_block(ext2_filsys fs, ext2_ino_t dir,
					সংরচনা ext2_inode *diri, blk64_t block,
					blk64_t *pblk, শূন্য *buf)
{
	errcode_t errcode;
	পূর্ণাঙ্ক ret_flags;

	errcode = ext2fs_bmap2(fs, dir, diri, রিক্ত, 0, block, &ret_flags,
			       pblk);
	যদি (errcode)
		ঘূৰাই errcode;
	যদি (ret_flags & BMAP_RET_UNINIT)
		ঘূৰাই EXT2_ET_DIR_CORRUPTED;
	ঘূৰাই ext2fs_read_dir_block4(fs, *pblk, buf, 0, dir);
}

স্থির errcode_t dx_lookup(ext2_filsys fs, ext2_ino_t dir,
			   সংরচনা ext2_inode *diri, সংরচনা dx_lookup_info *info)
{
	সংরচনা ext2_dx_মূল_info *মূল;
	errcode_t errcode;
	পূর্ণাঙ্ক level = 0;
	পূর্ণাঙ্ক গুনতি, limit;
	পূর্ণাঙ্ক hash_alg;
	পূর্ণাঙ্ক hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	__u32 minor_hash;
	সংরচনা dx_frame *frame;

	errcode = alloc_dx_frame(fs, &(info->frames[0]));
	যদি (errcode)
		ঘূৰাই errcode;
	info->levels = 1;

	errcode = load_logical_dir_block(fs, dir, diri, 0,
					 &(info->frames[0].pblock),
					 info->frames[0].buf);
	যদি (errcode)
		গচ্ছ out_err;
	মূল = (সংরচনা ext2_dx_মূল_info *) ((ৱর্ণ *)info->frames[0].buf +
					     EXT2_DX_মূল_OFF);
	hash_alg = মূল->hash_version;
	যদি (hash_alg != EXT2_HASH_TEA && hash_alg != EXT2_HASH_HALF_MD4 &&
	    hash_alg != EXT2_HASH_LEGACY) {
		errcode = EXT2_ET_DIRHASH_UNSUPP;
		গচ্ছ out_err;
	}
	যদি (hash_alg <= EXT2_HASH_TEA &&
	    fs->super->s_flags & EXT2_FLAGS_UNSIGNED_HASH)
		hash_alg += 3;
	যদি (মূল->indirect_levels >= ext2_dir_htree_level(fs)) {
		errcode = EXT2_ET_DIR_CORRUPTED;
		গচ্ছ out_err;
	}
	info->hash_alg = hash_alg;

	errcode = ext2fs_dirhash2(hash_alg, info->name, info->namelen,
				  fs->encoding, hash_flags,
				  fs->super->s_hash_seed, &info->hash,
				  &minor_hash);
	যদি (errcode)
		গচ্ছ out_err;

	বাবে (level = 0; level <= মূল->indirect_levels; level++) {
		frame = &(info->frames[level]);
		যদি (level > 0) {
			errcode = alloc_dx_frame(fs, frame);
			যদি (errcode)
				গচ্ছ out_err;
			info->levels++;

			errcode = load_logical_dir_block(fs, dir, diri,
				ext2fs_le32_to_cpu(info->frames[level-1].at->block) & 0x0fffffff,
				&(frame->pblock), frame->buf);
			যদি (errcode)
				গচ্ছ out_err;
		}
		errcode = ext2fs_get_dx_countlimit(fs, frame->buf,
						   &(frame->head), রিক্ত);
		যদি (errcode)
			গচ্ছ out_err;
		গুনতি = ext2fs_le16_to_cpu(frame->head->গুনতি);
		limit = ext2fs_le16_to_cpu(frame->head->limit);
		frame->entries = (সংরচনা ext2_dx_entry *)(frame->head);
		যদি (!গুনতি || গুনতি > limit) {
			errcode = EXT2_ET_DIR_CORRUPTED;
			গচ্ছ out_err;
		}

		dx_অনুসন্ধান_entry(frame, গুনতি, info->hash);
	}
	ঘূৰাই 0;
out_err:
	dx_release(info);
	ঘূৰাই errcode;
}

সংরচনা link_struct  {
	ext2_filsys	fs;
	ধ্রুৱ ৱর্ণ	*name;
	পূর্ণাঙ্ক		namelen;
	ext2_ino_t	inode;
	পূর্ণাঙ্ক		flags;
	পূর্ণাঙ্ক		done;
	unsigned পূর্ণাঙ্ক	blocksize;
	errcode_t	err;
	সংরচনা ext2_super_block *sb;
};

স্থির পূর্ণাঙ্ক link_proc(ext2_ino_t dir EXT2FS_ATTR((unused)),
		     পূর্ণাঙ্ক entru EXT2FS_ATTR((unused)),
		     সংরচনা ext2_dir_entry *dirent,
		     পূর্ণাঙ্ক	offset,
		     পূর্ণাঙ্ক	blocksize,
		     ৱর্ণ	*buf,
		     শূন্য	*priv_data)
{
	সংরচনা link_struct *ls = (সংরচনা link_struct *) priv_data;
	সংরচনা ext2_dir_entry *next;
	unsigned পূর্ণাঙ্ক rec_len, min_rec_len, curr_rec_len;
	পূর্ণাঙ্ক ret = 0;
	পূর্ণাঙ্ক csum_size = 0;

	যদি (ls->done)
		ঘূৰাই DIRENT_ABORT;

	rec_len = EXT2_DIR_REC_LEN(ls->namelen);

	ls->err = ext2fs_get_rec_len(ls->fs, dirent, &curr_rec_len);
	যদি (ls->err)
		ঘূৰাই DIRENT_ABORT;

	যদি (ext2fs_has_feature_metadata_csum(ls->fs->super))
		csum_size = পরিমাণ(সংরচনা ext2_dir_entry_tail);
	/*
	 * See if the following directory entry (if any) is unused;
	 * if so, absorb it into this one.
	 */
	next = (সংরচনা ext2_dir_entry *) (buf + offset + curr_rec_len);
	যদি ((offset + (পূর্ণাঙ্ক) curr_rec_len < blocksize - (8 + csum_size)) &&
	    (next->inode == 0) &&
	    (offset + (পূর্ণাঙ্ক) curr_rec_len + (পূর্ণাঙ্ক) next->rec_len <= blocksize)) {
		curr_rec_len += next->rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, curr_rec_len, dirent);
		যদি (ls->err)
			ঘূৰাই DIRENT_ABORT;
		ret = DIRENT_CHANGED;
	}

	/*
	 * If the directory entry is used, see if we can split the
	 * directory entry to make room for the new name.  If so,
	 * truncate it and return.
	 */
	যদি (dirent->inode) {
		min_rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(dirent));
		যদি (curr_rec_len < (min_rec_len + rec_len))
			ঘূৰাই ret;
		rec_len = curr_rec_len - min_rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, min_rec_len, dirent);
		যদি (ls->err)
			ঘূৰাই DIRENT_ABORT;
		next = (সংরচনা ext2_dir_entry *) (buf + offset +
						  dirent->rec_len);
		next->inode = 0;
		ext2fs_dirent_set_name_len(next, 0);
		ext2fs_dirent_set_file_type(next, 0);
		ls->err = ext2fs_set_rec_len(ls->fs, rec_len, next);
		যদি (ls->err)
			ঘূৰাই DIRENT_ABORT;
		ঘূৰাই DIRENT_CHANGED;
	}

	/*
	 * If we get this far, then the directory entry is not used.
	 * See if we can fit the request entry in.  If so, do it.
	 */
	যদি (curr_rec_len < rec_len)
		ঘূৰাই ret;
	dirent->inode = ls->inode;
	ext2fs_dirent_set_name_len(dirent, ls->namelen);
	strncpy(dirent->name, ls->name, ls->namelen);
	যদি (ext2fs_has_feature_filetype(ls->sb))
		ext2fs_dirent_set_file_type(dirent,
					    ls->flags & EXT2FS_LINK_FT_MASK);

	ls->done++;
	ঘূৰাই DIRENT_ABORT|DIRENT_CHANGED;
}

স্থির errcode_t add_dirent_to_buf(ext2_filsys fs, e2_blkcnt_t blockcnt,
				   ৱর্ণ *buf, ext2_ino_t dir,
				   সংরচনা ext2_inode *diri, ধ্রুৱ ৱর্ণ *name,
				   ext2_ino_t ino, পূর্ণাঙ্ক flags, blk64_t *pblkp)
{
	সংরচনা dir_context ctx;
	সংরচনা link_struct ls;
	errcode_t retval;

	retval = load_logical_dir_block(fs, dir, diri, blockcnt, pblkp, buf);
	যদি (retval)
		ঘূৰাই retval;
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
	যদি (ctx.errcode)
		ঘূৰাই ctx.errcode;
	যদি (ls.err)
		ঘূৰাই ls.err;
	যদি (!ls.done)
		ঘূৰাই EXT2_ET_DIR_NO_SPACE;
	ঘূৰাই 0;
}

সংরচনা dx_hash_map {
	__u32 hash;
	পূর্ণাঙ্ক size;
	পূর্ণাঙ্ক off;
};

স্থির EXT2_QSORT_TYPE dx_hash_map_cmp(ধ্রুৱ শূন্য *ap, ধ্রুৱ শূন্য *bp)
{
	ধ্রুৱ সংরচনা dx_hash_map *a = ap, *b = bp;

	যদি (a->hash < b->hash)
		ঘূৰাই -1;
	যদি (a->hash > b->hash)
		ঘূৰাই 1;
	ঘূৰাই 0;
}

স্থির errcode_t dx_move_dirents(ext2_filsys fs, সংরচনা dx_hash_map *map,
				 পূর্ণাঙ্ক গুনতি, শূন্য *from, শূন্য *to)
{
	সংরচনা ext2_dir_entry *de;
	পূর্ণাঙ্ক i;
	পূর্ণাঙ্ক rec_len = 0;
	errcode_t retval;
	পূর্ণাঙ্ক csum_size = 0;
	শূন্য *base = to;

	যদি (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = পরিমাণ(সংরচনা ext2_dir_entry_tail);

	বাবে (i = 0; i < গুনতি; i++) {
		de = (সংরচনা ext2_dir_entry *) ((ৱর্ণ *)from + map[i].off);
		rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(de));
		memcpy(to, de, rec_len);
		retval = ext2fs_set_rec_len(fs, rec_len, to);
		যদি (retval)
			ঘূৰাই retval;
		to = (ৱর্ণ *)to + rec_len;
	}
	/*
	 * Update rec_len of the last dir entry to stretch to the end of block
	 */
	to = (ৱর্ণ *)to - rec_len;
	rec_len = fs->blocksize - ((ৱর্ণ *)to - (ৱর্ণ *)base) - csum_size;
	retval = ext2fs_set_rec_len(fs, rec_len, to);
	যদি (retval)
		ঘূৰাই retval;
	যদি (csum_size)
		ext2fs_initialize_dirent_tail(fs,
				EXT2_DIRENT_TAIL(base, fs->blocksize));
	ঘূৰাই 0;
}

স্থির errcode_t dx_ঢোকাও_entry(ext2_filsys fs, ext2_ino_t dir,
				 সংরচনা dx_lookup_info *info, পূর্ণাঙ্ক level,
				 __u32 hash, blk64_t lblk)
{
	পূর্ণাঙ্ক pcount;
	সংরচনা ext2_dx_entry *শীৰ্ষ, *new;

	pcount = ext2fs_le16_to_cpu(info->frames[level].head->গুনতি);
	শীৰ্ষ = info->frames[level].entries + pcount;
	new = info->frames[level].at + 1;
	memmove(new + 1, new, (ৱর্ণ *)শীৰ্ষ - (ৱর্ণ *)new);
	new->hash = ext2fs_cpu_to_le32(hash);
	new->block = ext2fs_cpu_to_le32(lblk);
	info->frames[level].head->গুনতি = ext2fs_cpu_to_le16(pcount + 1);
	ঘূৰাই ext2fs_write_dir_block4(fs, info->frames[level].pblock,
				       info->frames[level].buf, 0, dir);
}

স্থির errcode_t dx_split_leaf(ext2_filsys fs, ext2_ino_t dir,
			       সংরচনা ext2_inode *diri,
			       সংরচনা dx_lookup_info *info, শূন্য *buf,
			       blk64_t leaf_pblk, blk64_t new_lblk,
			       blk64_t new_pblk)
{
	পূর্ণাঙ্ক hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	সংরচনা ext2_dir_entry *de;
	শূন্য *buf2;
	errcode_t retval = 0;
	unsigned পূর্ণাঙ্ক rec_len;
	unsigned পূর্ণাঙ্ক offset, move_size;
	পূর্ণাঙ্ক i, গুনতি = 0;
	সংরচনা dx_hash_map *map;
	পূর্ণাঙ্ক continued;
	__u32 minor_hash;

	retval = ext2fs_get_mem(fs->blocksize, &buf2);
	যদি (retval)
		ঘূৰাই retval;
	retval = ext2fs_get_array(fs->blocksize / 12,
				  পরিমাণ(সংরচনা dx_hash_map), &map);
	যদি (retval) {
		ext2fs_free_mem(&buf2);
		ঘূৰাই retval;
	}
	বাবে (offset = 0; offset < fs->blocksize; offset += rec_len) {
		de = (সংরচনা ext2_dir_entry *) ((ৱর্ণ *)buf + offset);
		retval = ext2fs_get_rec_len(fs, de, &rec_len);
		যদি (retval)
			গচ্ছ out;
		যদি (ext2fs_dirent_name_len(de) > 0 && de->inode) {
			map[গুনতি].off = offset;
			map[গুনতি].size = rec_len;
			retval = ext2fs_dirhash2(info->hash_alg, de->name,
					ext2fs_dirent_name_len(de),
					fs->encoding, hash_flags,
					fs->super->s_hash_seed,
					&(map[গুনতি].hash),
					&minor_hash);
			যদি (retval)
				গচ্ছ out;
			গুনতি++;
		}
	}
	qsort(map, গুনতি, পরিমাণ(সংরচনা dx_hash_map), dx_hash_map_cmp);
	move_size = 0;
	/* Find place to split block */
	বাবে (i = গুনতি - 1; i >= 0; i--) {
		যদি (move_size + map[i].size / 2 > fs->blocksize / 2)
			ভঙ্গ;
		move_size += map[i].size;
	}
	/* Let i be the first entry to move */
	i++;
	/* Move selected directory entries to new block */
	retval = dx_move_dirents(fs, map + i, গুনতি - i, buf, buf2);
	যদি (retval)
		গচ্ছ out;
	retval = ext2fs_write_dir_block4(fs, new_pblk, buf2, 0, dir);
	যদি (retval)
		গচ্ছ out;
	/* Repack remaining entries in the old block */
	retval = dx_move_dirents(fs, map, i, buf, buf2);
	যদি (retval)
		গচ্ছ out;
	retval = ext2fs_write_dir_block4(fs, leaf_pblk, buf2, 0, dir);
	যদি (retval)
		গচ্ছ out;
	/* Update parent node */
	continued = map[i].hash == map[i-1].hash;
	retval = dx_ঢোকাও_entry(fs, dir, info, info->levels - 1,
				 map[i].hash + continued, new_lblk);
out:
	ext2fs_free_mem(&buf2);
	ext2fs_free_mem(&map);
	ঘূৰাই retval;
}

স্থির errcode_t dx_grow_বৃক্ষ(ext2_filsys fs, ext2_ino_t dir,
			      সংরচনা ext2_inode *diri,
			      সংরচনা dx_lookup_info *info, শূন্য *buf,
			      blk64_t leaf_pblk)
{
	পূর্ণাঙ্ক i;
	errcode_t retval;
	ext2_off64_t size = EXT2_I_SIZE(diri);
	blk64_t lblk, pblk;
	সংরচনা ext2_dir_entry *de;
	সংরচনা ext2_dx_countlimit *head;
	পূর্ণাঙ্ক csum_size = 0;
	পূর্ণাঙ্ক গুনতি;

	যদি (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = পরিমাণ(সংরচনা ext2_dx_tail);

	/* Find level which can accommodate new child */
	বাবে (i = info->levels - 1; i >= 0; i--)
		যদি (ext2fs_le16_to_cpu(info->frames[i].head->গুনতি) <
		    ext2fs_le16_to_cpu(info->frames[i].head->limit))
			ভঙ্গ;
	/* Need to grow tree depth? */
	যদি (i < 0 && info->levels >= ext2_dir_htree_level(fs))
		ঘূৰাই EXT2_ET_DIR_NO_SPACE;
	lblk = size / fs->blocksize;
	size += fs->blocksize;
	retval = ext2fs_inode_size_set(fs, diri, size);
	যদি (retval)
		ঘূৰাই retval;
	retval = ext2fs_fallocate(fs,
			EXT2_FALLOCATE_FORCE_INIT | EXT2_FALLOCATE_ZERO_BLOCKS,
			dir, diri, 0, lblk, 1);
	যদি (retval)
		ঘূৰাই retval;
	retval = ext2fs_write_inode(fs, dir, diri);
	যদি (retval)
		ঘূৰাই retval;
	retval = ext2fs_bmap2(fs, dir, diri, রিক্ত, 0, lblk, রিক্ত, &pblk);
	যদি (retval)
		ঘূৰাই retval;
	/* Only leaf addition needed? */
	যদি (i == (পূর্ণাঙ্ক)info->levels - 1)
		ঘূৰাই dx_split_leaf(fs, dir, diri, info, buf, leaf_pblk,
				     lblk, pblk);

	de = buf;
	de->inode = 0;
	ext2fs_dirent_set_name_len(de, 0);
	ext2fs_dirent_set_file_type(de, 0);
	retval = ext2fs_set_rec_len(fs, fs->blocksize, de);
	যদি (retval)
		ঘূৰাই retval;
	head = (সংরচনা ext2_dx_countlimit *) ((ৱর্ণ *)buf + 8);
	গুনতি = ext2fs_le16_to_cpu(info->frames[i+1].head->গুনতি);
	/* Growing tree depth? */
	যদি (i < 0) {
		সংরচনা ext2_dx_মূল_info *মূল;

		memcpy(head, info->frames[0].entries,
		       গুনতি * পরিমাণ(সংরচনা ext2_dx_entry));
		head->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_size)) /
				পরিমাণ(সংরচনা ext2_dx_entry));
		/* head->count gets set by memcpy above to correct value */

		/* Now update tree root */
		info->frames[0].head->গুনতি = ext2fs_cpu_to_le16(1);
		info->frames[0].entries[0].block = ext2fs_cpu_to_le32(lblk);
		মূল = (সংরচনা ext2_dx_মূল_info *)
			((ৱর্ণ *)info->frames[0].buf + EXT2_DX_মূল_OFF);
		মূল->indirect_levels++;
	} অন্যথা {
		/* Splitting internal node in two */
		পূর্ণাঙ্ক count1 = গুনতি / 2;
		পূর্ণাঙ্ক count2 = গুনতি - count1;
		__u32 split_hash = ext2fs_le32_to_cpu(info->frames[i+1].entries[count1].hash);

		memcpy(head, info->frames[i+1].entries + count1,
		       count2 * পরিমাণ(সংরচনা ext2_dx_entry));
		head->গুনতি = ext2fs_cpu_to_le16(count2);
		head->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_size)) /
				পরিমাণ(সংরচনা ext2_dx_entry));
		info->frames[i+1].head->গুনতি = ext2fs_cpu_to_le16(count1);

		/* Update parent node */
		retval = dx_ঢোকাও_entry(fs, dir, info, i, split_hash, lblk);
		যদি (retval)
			ঘূৰাই retval;

	}
	/* Writeout split block / updated root */
	retval = ext2fs_write_dir_block4(fs, info->frames[i+1].pblock,
					 info->frames[i+1].buf, 0, dir);
	যদি (retval)
		ঘূৰাই retval;
	/* Writeout new tree block */
	retval = ext2fs_write_dir_block4(fs, pblk, buf, 0, dir);
	যদি (retval)
		ঘূৰাই retval;
	ঘূৰাই 0;
}

স্থির errcode_t dx_link(ext2_filsys fs, ext2_ino_t dir,
			 সংরচনা ext2_inode *diri, ধ্রুৱ ৱর্ণ *name,
			 ext2_ino_t ino, পূর্ণাঙ্ক flags)
{
	সংরচনা dx_lookup_info dx_info;
	errcode_t retval;
	শূন্য *blockbuf;
	unsigned restart = 0;
	blk64_t leaf_pblk;

	retval = ext2fs_get_mem(fs->blocksize, &blockbuf);
	যদি (retval)
		ঘূৰাই retval;

	dx_info.name = name;
	dx_info.namelen = strlen(name);
again:
	retval = dx_lookup(fs, dir, diri, &dx_info);
	যদি (retval)
		গচ্ছ free_buf;

	retval = add_dirent_to_buf(fs,
		ext2fs_le32_to_cpu(dx_info.frames[dx_info.levels-1].at->block) & 0x0fffffff,
		blockbuf, dir, diri, name, ino, flags, &leaf_pblk);
	/*
	 * Success or error other than ENOSPC...? We are done. We may need upto
	 * two tries to add entry. One to split htree node and another to add
	 * new leaf block.
	 */
	যদি (restart >= dx_info.levels || retval != EXT2_ET_DIR_NO_SPACE)
		গচ্ছ free_frames;
	retval = dx_grow_বৃক্ষ(fs, dir, diri, &dx_info, blockbuf, leaf_pblk);
	যদি (retval)
		গচ্ছ free_frames;
	/* Restart everything now that the tree is larger */
	restart++;
	dx_release(&dx_info);
	গচ্ছ again;
free_frames:
	dx_release(&dx_info);
free_buf:
	ext2fs_free_mem(&blockbuf);
	ঘূৰাই retval;
}

সংরচনা fake_dirent
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

সংরচনা dx_মূল
{
	সংরচনা fake_dirent dot;
	ৱর্ণ dot_name[4];
	সংরচনা fake_dirent dotdot;
	ৱর্ণ dotdot_name[4];
	সংরচনা ext2_dx_মূল_info info;
	সংরচনা ext2_dx_entry entries[];
};

স্থির পূর্ণাঙ্ক check_dx_মূল(ext2_filsys fs, ext2_ino_t ino, সংরচনা dx_মূল *মূল)
{
	সংরচনা fake_dirent *fde;
	ৱর্ণ *error_বাৰ্তা;
	unsigned পূর্ণাঙ্ক rlen;
	ৱর্ণ *blockend = (ৱর্ণ *)মূল + fs->blocksize;

	fde = &মূল->dot;
	যদি (fde->name_len != 1) {
		error_বাৰ্তা = "invalid name_len for '.'";
		গচ্ছ corrupted;
	}
	যদি (strncmp(মূল->dot_name, ".", fde->name_len)) {
		error_বাৰ্তা = "invalid name for '.'";
		গচ্ছ corrupted;
	}

	ext2fs_get_rec_len(fs, (সংরচনা ext2_dir_entry *)fde, &rlen);
	যদি ((ৱর্ণ *)fde + rlen >= blockend) {
		error_বাৰ্তা = "invalid rec_len for '.'";
		গচ্ছ corrupted;
	}

	fde = &মূল->dotdot;
	যদি (fde->name_len != 2) {
		error_বাৰ্তা = "invalid name_len for '..'";
		গচ্ছ corrupted;
	}
	যদি (strncmp(মূল->dotdot_name, "..", fde->name_len)) {
		error_বাৰ্তা = "invalid name for '..'";
		গচ্ছ corrupted;
	}
	ext2fs_get_rec_len(fs, (সংরচনা ext2_dir_entry *)fde, &rlen);
	যদি ((ৱর্ণ *)fde + rlen >= blockend) {
		error_বাৰ্তা = "invalid rec_len for '..'";
		গচ্ছ corrupted;
	}

	ঘূৰাই 1;

corrupted:
	fprintf(stderr, "Corrupt dir %u: %s, running e2fsck is recommended\n",
			 ino, error_বাৰ্তা);
	ঘূৰাই 0;
}

স্থির অন্তঃস্থ সংরচনা ext2_dir_entry *
ext2fs_next_entry(ext2_filsys fs, সংরচনা ext2_dir_entry *p)
{
	unsigned পূর্ণাঙ্ক rlen;

	ext2fs_get_rec_len(fs, p, &rlen);
	ঘূৰাই (সংরচনা ext2_dir_entry *)((ৱর্ণ *)p + rlen);
}

স্থির অন্তঃস্থ শূন্য dx_set_block(সংরচনা ext2_dx_entry *entry, unsigned মান)
{
	entry->block = ext2fs_cpu_to_le32(মান);
}

স্থির অন্তঃস্থ শূন্য dx_set_গুনতি(সংরচনা ext2_dx_entry *entries, unsigned মান)
{
	((সংরচনা ext2_dx_countlimit *) entries)->গুনতি = ext2fs_cpu_to_le16(মান);
}

স্থির অন্তঃস্থ শূন্য dx_set_limit(সংরচনা ext2_dx_entry *entries, unsigned মান)
{
	((সংরচনা ext2_dx_countlimit *) entries)->limit = ext2fs_cpu_to_le16(মান);
}

স্থির অন্তঃস্থ unsigned dx_মূল_limit(ext2_filsys fs, unsigned infosize)
{
	unsigned পূর্ণাঙ্ক entry_space = fs->blocksize -
			ext2fs_dir_rec_len(1, 0) -
			ext2fs_dir_rec_len(2, 0) - infosize;

	যদি (ext2fs_has_feature_metadata_csum(fs->super))
		entry_space -= পরিমাণ(সংরচনা ext2_dx_tail);
	ঘূৰাই entry_space / পরিমাণ(সংরচনা ext2_dx_entry);
}

/*
 * This converts a one block unindexed directory to a 3 block indexed
 * directory, and adds the dentry to the indexed directory.  Returns 0 if the
 * index was not created; EAGAIN if it was; or an errcode_t on error.
 */
স্থির errcode_t try_make_indexed_dir(ext2_filsys fs, ext2_ino_t dir,
				      ধ্রুৱ ৱর্ণ *name, ext2_ino_t ino,
				      পূর্ণাঙ্ক flags)
{
	সংরচনা ext2_inode inode;
	ৱর্ণ *buf0, *buf1, *শীৰ্ষ;
	সংরচনা dx_মূল *মূল;
	সংরচনা ext2_dx_entry *entries;
	সংরচনা fake_dirent *fde;
	সংরচনা ext2_dir_entry *de, *de2;
	blk64_t pblk0, pblk1;
	unsigned পূর্ণাঙ্ক len;
	ধ্রুৱ unsigned পূর্ণাঙ্ক blocksize = fs->blocksize;
	পূর্ণাঙ্ক csum_size = 0;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	যদি (retval)
		ঘূৰাই retval;

	যদি (inode.i_size > fs->blocksize || (inode.i_flags & EXT2_INDEX_FL))
		ঘূৰাই 0;

	যদি (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = পরিমাণ(সংরচনা ext2_dir_entry_tail);

	retval = ext2fs_get_mem(blocksize, &buf0);
	যদি (retval)
		ঘূৰাই retval;

	retval = ext2fs_get_mem(blocksize, &buf1);
	যদি (retval)
		গচ্ছ out_buf0;

	retval = ext2fs_bmap2(fs, dir, &inode, রিক্ত, 0, 0, রিক্ত, &pblk0);
	যদি (retval)
		গচ্ছ out_buf1;

	retval = ext2fs_read_dir_block4(fs, pblk0, buf0, 0, dir);
	যদি (retval)
		গচ্ছ out_buf1;

	মূল = (সংরচনা dx_মূল *)buf0;
	যদি (!check_dx_মূল(fs, dir, মূল)) {
		retval = EXT2_ET_DIR_CORRUPTED;
		গচ্ছ out_buf1;
	}

	/* The 0th block becomes the root, move the dirents out */
	fde = &মূল->dotdot;
	de = ext2fs_next_entry(fs, (সংরচনা ext2_dir_entry *)fde);
	len = ((ৱর্ণ *) মূল) + (blocksize - csum_size) - (ৱর্ণ *) de;

	/* Allocate new block for the 0th block's dirents */
	retval = ext2fs_bmap2(fs, dir, &inode, রিক্ত, BMAP_ALLOC | BMAP_ZERO, 1,
			      রিক্ত, &pblk1);
	যদি (retval)
		গচ্ছ out_buf1;

	memcpy(buf1, de, len);
	memset(de, 0, len); /* wipe old data */

	de = (সংরচনা ext2_dir_entry *)buf1;
	শীৰ্ষ = buf1 + len;
	যেতিয়ালৈ ((ৱর্ণ *)(de2 = ext2fs_next_entry(fs, de)) < শীৰ্ষ) {
#যদি 0
		যদি (ext4_check_dir_entry(dir, রিক্ত, de, bh2, buf1, len,
					(ৱর্ণ *)de - buf1)) {
			retval = EXT2_ET_DIR_CORRUPTED;
			গচ্ছ out_buf1;
		}
#endif
		de = de2;
	}
	retval = ext2fs_set_rec_len(fs,
			buf1 + (blocksize - csum_size) - (ৱর্ণ *) de, de);
	যদি (retval)
		গচ্ছ out_buf1;

	যদি (csum_size)
		ext2fs_initialize_dirent_tail(fs,
				EXT2_DIRENT_TAIL(buf1, fs->blocksize));

	/* Initialize the root; the dot dirents already exist */
	de = (সংরচনা ext2_dir_entry *) (&মূল->dotdot);
	retval = ext2fs_set_rec_len(fs, blocksize - ext2fs_dir_rec_len(2, 0),
				    de);
	memset (&মূল->info, 0, পরিমাণ(মূল->info));
	মূল->info.info_length = পরিমাণ(মূল->info);
	যদি (ext4_hash_in_dirent(&inode))
		মূল->info.hash_version = EXT2_HASH_SIPHASH;
	অন্যথা
		মূল->info.hash_version = fs->super->s_def_hash_version;

	entries = মূল->entries;
	dx_set_block(entries, 1);
	dx_set_গুনতি(entries, 1);
	dx_set_limit(entries, dx_মূল_limit(fs, পরিমাণ(মূল->info)));

	retval = ext2fs_write_dir_block4(fs, pblk1, buf1, 0, dir);
	যদি (retval)
		গচ্ছ out_buf1;

	retval = ext2fs_write_dir_block4(fs, pblk0, buf0, 0, dir);
	যদি (retval)
		গচ্ছ out_buf1;

	inode.i_flags |= EXT2_INDEX_FL;
	inode.i_size += fs->blocksize;
	retval = ext2fs_write_inode(fs, dir, &inode);
	যদি (retval)
		গচ্ছ out_buf1;

	retval = EAGAIN;
out_buf1:
	ext2fs_free_mem(&buf1);
out_buf0:
	ext2fs_free_mem(&buf0);
	ঘূৰাই retval;
}

/*
 * Note: the low 3 bits of the flags field are used as the directory
 * entry filetype.
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, ধ্রুৱ ৱর্ণ *name,
		      ext2_ino_t ino, পূর্ণাঙ্ক flags)
{
	errcode_t		retval;
	সংরচনা link_struct	ls;
	সংরচনা ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (!(fs->flags & EXT2_FLAG_RW))
		ঘূৰাই EXT2_ET_RO_FILSYS;

retry:
	যদি ((retval = ext2fs_read_inode(fs, dir, &inode)) != 0)
		ঘূৰাই retval;

	যদি (inode.i_flags & EXT2_INDEX_FL)
		ঘূৰাই dx_link(fs, dir, &inode, name, ino, flags);

	ls.fs = fs;
	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	যদি ((flags & EXT2FS_LINK_APPEND) &&
	    !(inode.i_flags & EXT4_INLINE_DATA_FL)) {
		blk64_t		lblk, pblk = 0;
		সংরচনা dir_context ctx;

		lblk = (inode.i_size / fs->blocksize) - 1;
		retval = ext2fs_bmap2(fs, dir, &inode, রিক্ত, 0, lblk,
				      রিক্ত, &pblk);
		যদি (retval)
			ঘূৰাই retval;

		ctx.dir = dir;
		ctx.flags = DIRENT_FLAG_INCLUDE_EMPTY;
		ctx.func = link_proc;
		ctx.priv_data = &ls;
		ctx.errcode = 0;
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		যদি (retval)
			ঘূৰাই retval;

		ext2fs_process_dir_block(fs, &pblk, lblk, 0, 0, &ctx);
		retval = ctx.errcode;
		ext2fs_free_mem(&ctx.buf);
	} অন্যথা {
		retval = ext2fs_dir_iterate2(fs, dir,
					     DIRENT_FLAG_INCLUDE_EMPTY,
					     রিক্ত, link_proc, &ls);
	}
	যদি (retval)
		ঘূৰাই retval;
	যদি (ls.err)
		ঘূৰাই ls.err;

	যদি (!ls.done) {
		যদি (!(flags & EXT2FS_LINK_EXPAND))
			ঘূৰাই EXT2_ET_DIR_NO_SPACE;

		যদি (ext2fs_has_feature_dir_index(fs->super)) {
			retval = try_make_indexed_dir(fs, dir, name, ino,
						      flags);
			যদি (retval == EAGAIN)
				গচ্ছ retry;
			যদি (retval)
				ঘূৰাই retval;
		}

		retval = ext2fs_expand_dir(fs, dir);
		যদি (retval)
			ঘূৰাই retval;
		গচ্ছ retry;
	}
	ঘূৰাই 0;
}

/* Does this directory have an htree index? */
পূর্ণাঙ্ক ext2fs_dir_is_dx(ext2_filsys fs, ধ্রুৱ সংরচনা ext2_inode *inode)
{
	ঘূৰাই ext2fs_has_feature_dir_index(fs->super) &&
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
শূন্য ext2fs_inc_nlink(ext2_filsys fs, সংরচনা ext2_inode *inode)
{
	inode->i_links_গুনতি++;

	যদি (ext2fs_dir_is_dx(fs, inode) &&
	    (inode->i_links_গুনতি > EXT2_LINK_MAX || inode->i_links_গুনতি == 2))
		inode->i_links_গুনতি = 1;
}

/*
 * If a directory had nlink == 1, then we should let it be 1. This indicates
 * directory has >EXT2_LINK_MAX subdirs.
 */
শূন্য ext2fs_dec_nlink(সংরচনা ext2_inode *inode)
{
	যদি (!S_ISDIR(inode->i_mode) || inode->i_links_গুনতি > 2)
		inode->i_links_গুনতি--;
}

পূর্ণাঙ্ক ext2fs_dir_link_max(ext2_filsys fs, সংরচনা ext2_inode_large *inode)
{
	ঘূৰাই inode->i_links_গুনতি >= EXT2_LINK_MAX &&
	       !(ext2fs_dir_is_dx(fs, EXT2_INODE(inode)) &&
		 ext2fs_has_feature_dir_nlink(fs->super));
}

পূর্ণাঙ্ক ext2fs_dir_link_empty(সংরচনা ext2_inode *inode)
{
	ঘূৰাই inode->i_links_গুনতি == 2 || inode->i_links_গুনতি == 1;
}
