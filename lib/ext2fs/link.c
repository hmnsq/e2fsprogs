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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

#define EXT2_DX_ମୂଳ_OFF 24

ସଂରଚନା dx_frame {
	ଶୂନ୍ୟ *buf;
	blk64_t pblock;
	ସଂରଚନା ext2_dx_countlimit *head;
	ସଂରଚନା ext2_dx_entry *entries;
	ସଂରଚନା ext2_dx_entry *at;
};

ସଂରଚନା dx_lookup_info {
	ଅଚଳ ଅକ୍ଷର *name;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା namelen;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା hash_alg;
	__u32 hash;
	unsigned levels;
	ସଂରଚନା dx_frame frames[EXT4_HTREE_LEVEL];
};

ସ୍ଥିର errcode_t alloc_dx_frame(ext2_filsys fs, ସଂରଚନା dx_frame *frame)
{
	ଫେରନ୍ତୁ ext2fs_get_mem(fs->blocksize, &frame->buf);
}

ସ୍ଥିର ଶୂନ୍ୟ dx_release(ସଂରଚନା dx_lookup_info *info)
{
	unsigned level;

	ପାଇଁ (level = 0; level < info->levels; level++) {
		ଯଦି (info->frames[level].buf == ଖାଲି)
			ଭାଙ୍ଗ;
		ext2fs_free_mem(&(info->frames[level].buf));
	}
	info->levels = 0;
}

ସ୍ଥିର ଶୂନ୍ୟ dx_ସନ୍ଧାନ_entry(ସଂରଚନା dx_frame *frame, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, __u32 hash)
{
	ସଂରଚନା ext2_dx_entry *p, *q, *m;

	p = frame->entries + 1;
	q = frame->entries + ଗଣନା_2 - 1;
	ଯେତେବେଳେ (p <= q) {
		m = p + (q - p) / 2;
		ଯଦି (ext2fs_le32_to_cpu(m->hash) > hash)
			q = m - 1;
		ଅନ୍ୟଥା
			p = m + 1;
	}
	frame->at = p - 1;
}

ସ୍ଥିର errcode_t load_logical_dir_block(ext2_filsys fs, ext2_ino_t dir,
					ସଂରଚନା ext2_inode *diri, blk64_t block,
					blk64_t *pblk, ଶୂନ୍ୟ *buf)
{
	errcode_t errcode;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret_flags;

	errcode = ext2fs_bmap2(fs, dir, diri, ଖାଲି, 0, block, &ret_flags,
			       pblk);
	ଯଦି (errcode)
		ଫେରନ୍ତୁ errcode;
	ଯଦି (ret_flags & BMAP_RET_UNINIT)
		ଫେରନ୍ତୁ EXT2_ET_DIR_CORRUPTED;
	ଫେରନ୍ତୁ ext2fs_read_dir_block4(fs, *pblk, buf, 0, dir);
}

ସ୍ଥିର errcode_t dx_lookup(ext2_filsys fs, ext2_ino_t dir,
			   ସଂରଚନା ext2_inode *diri, ସଂରଚନା dx_lookup_info *info)
{
	ସଂରଚନା ext2_dx_ମୂଳ_info *ମୂଳ;
	errcode_t errcode;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା level = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, limit;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା hash_alg;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	__u32 minor_hash;
	ସଂରଚନା dx_frame *frame;

	errcode = alloc_dx_frame(fs, &(info->frames[0]));
	ଯଦି (errcode)
		ଫେରନ୍ତୁ errcode;
	info->levels = 1;

	errcode = load_logical_dir_block(fs, dir, diri, 0,
					 &(info->frames[0].pblock),
					 info->frames[0].buf);
	ଯଦି (errcode)
		ଯାଅ out_err;
	ମୂଳ = (ସଂରଚନା ext2_dx_ମୂଳ_info *) ((ଅକ୍ଷର *)info->frames[0].buf +
					     EXT2_DX_ମୂଳ_OFF);
	hash_alg = ମୂଳ->hash_version;
	ଯଦି (hash_alg != EXT2_HASH_TEA && hash_alg != EXT2_HASH_HALF_MD4 &&
	    hash_alg != EXT2_HASH_LEGACY) {
		errcode = EXT2_ET_DIRHASH_UNSUPP;
		ଯାଅ out_err;
	}
	ଯଦି (hash_alg <= EXT2_HASH_TEA &&
	    fs->super->s_flags & EXT2_FLAGS_UNSIGNED_HASH)
		hash_alg += 3;
	ଯଦି (ମୂଳ->indirect_levels >= ext2_dir_htree_level(fs)) {
		errcode = EXT2_ET_DIR_CORRUPTED;
		ଯାଅ out_err;
	}
	info->hash_alg = hash_alg;

	errcode = ext2fs_dirhash2(hash_alg, info->name, info->namelen,
				  fs->encoding, hash_flags,
				  fs->super->s_hash_seed, &info->hash,
				  &minor_hash);
	ଯଦି (errcode)
		ଯାଅ out_err;

	ପାଇଁ (level = 0; level <= ମୂଳ->indirect_levels; level++) {
		frame = &(info->frames[level]);
		ଯଦି (level > 0) {
			errcode = alloc_dx_frame(fs, frame);
			ଯଦି (errcode)
				ଯାଅ out_err;
			info->levels++;

			errcode = load_logical_dir_block(fs, dir, diri,
				ext2fs_le32_to_cpu(info->frames[level-1].at->block) & 0x0fffffff,
				&(frame->pblock), frame->buf);
			ଯଦି (errcode)
				ଯାଅ out_err;
		}
		errcode = ext2fs_get_dx_countlimit(fs, frame->buf,
						   &(frame->head), ଖାଲି);
		ଯଦି (errcode)
			ଯାଅ out_err;
		ଗଣନା_2 = ext2fs_le16_to_cpu(frame->head->ଗଣନା_2);
		limit = ext2fs_le16_to_cpu(frame->head->limit);
		frame->entries = (ସଂରଚନା ext2_dx_entry *)(frame->head);
		ଯଦି (!ଗଣନା_2 || ଗଣନା_2 > limit) {
			errcode = EXT2_ET_DIR_CORRUPTED;
			ଯାଅ out_err;
		}

		dx_ସନ୍ଧାନ_entry(frame, ଗଣନା_2, info->hash);
	}
	ଫେରନ୍ତୁ 0;
out_err:
	dx_release(info);
	ଫେରନ୍ତୁ errcode;
}

ସଂରଚନା link_struct  {
	ext2_filsys	fs;
	ଅଚଳ ଅକ୍ଷର	*name;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		namelen;
	ext2_ino_t	inode;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		flags;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		done;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା	blocksize;
	errcode_t	err;
	ସଂରଚନା ext2_super_block *sb;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା link_proc(ext2_ino_t dir EXT2FS_ATTR((unused)),
		     ପୂର୍ଣ୍ଣସଂଖ୍ୟା entru EXT2FS_ATTR((unused)),
		     ସଂରଚନା ext2_dir_entry *dirent,
		     ପୂର୍ଣ୍ଣସଂଖ୍ୟା	offset,
		     ପୂର୍ଣ୍ଣସଂଖ୍ୟା	blocksize,
		     ଅକ୍ଷର	*buf,
		     ଶୂନ୍ୟ	*priv_data)
{
	ସଂରଚନା link_struct *ls = (ସଂରଚନା link_struct *) priv_data;
	ସଂରଚନା ext2_dir_entry *next;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା rec_len, min_rec_len, curr_rec_len;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା csum_size = 0;

	ଯଦି (ls->done)
		ଫେରନ୍ତୁ DIRENT_ABORT;

	rec_len = EXT2_DIR_REC_LEN(ls->namelen);

	ls->err = ext2fs_get_rec_len(ls->fs, dirent, &curr_rec_len);
	ଯଦି (ls->err)
		ଫେରନ୍ତୁ DIRENT_ABORT;

	ଯଦି (ext2fs_has_feature_metadata_csum(ls->fs->super))
		csum_size = ଆକାର(ସଂରଚନା ext2_dir_entry_tail);
	/*
	 * See if the following directory entry (if any) is unused;
	 * if so, absorb it into this one.
	 */
	next = (ସଂରଚନା ext2_dir_entry *) (buf + offset + curr_rec_len);
	ଯଦି ((offset + (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) curr_rec_len < blocksize - (8 + csum_size)) &&
	    (next->inode == 0) &&
	    (offset + (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) curr_rec_len + (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) next->rec_len <= blocksize)) {
		curr_rec_len += next->rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, curr_rec_len, dirent);
		ଯଦି (ls->err)
			ଫେରନ୍ତୁ DIRENT_ABORT;
		ret = DIRENT_CHANGED;
	}

	/*
	 * If the directory entry is used, see if we can split the
	 * directory entry to make room for the new name.  If so,
	 * truncate it and return.
	 */
	ଯଦି (dirent->inode) {
		min_rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(dirent));
		ଯଦି (curr_rec_len < (min_rec_len + rec_len))
			ଫେରନ୍ତୁ ret;
		rec_len = curr_rec_len - min_rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, min_rec_len, dirent);
		ଯଦି (ls->err)
			ଫେରନ୍ତୁ DIRENT_ABORT;
		next = (ସଂରଚନା ext2_dir_entry *) (buf + offset +
						  dirent->rec_len);
		next->inode = 0;
		ext2fs_dirent_set_name_len(next, 0);
		ext2fs_dirent_set_file_type(next, 0);
		ls->err = ext2fs_set_rec_len(ls->fs, rec_len, next);
		ଯଦି (ls->err)
			ଫେରନ୍ତୁ DIRENT_ABORT;
		ଫେରନ୍ତୁ DIRENT_CHANGED;
	}

	/*
	 * If we get this far, then the directory entry is not used.
	 * See if we can fit the request entry in.  If so, do it.
	 */
	ଯଦି (curr_rec_len < rec_len)
		ଫେରନ୍ତୁ ret;
	dirent->inode = ls->inode;
	ext2fs_dirent_set_name_len(dirent, ls->namelen);
	strncpy(dirent->name, ls->name, ls->namelen);
	ଯଦି (ext2fs_has_feature_filetype(ls->sb))
		ext2fs_dirent_set_file_type(dirent,
					    ls->flags & EXT2FS_LINK_FT_MASK);

	ls->done++;
	ଫେରନ୍ତୁ DIRENT_ABORT|DIRENT_CHANGED;
}

ସ୍ଥିର errcode_t add_dirent_to_buf(ext2_filsys fs, e2_blkcnt_t blockcnt,
				   ଅକ୍ଷର *buf, ext2_ino_t dir,
				   ସଂରଚନା ext2_inode *diri, ଅଚଳ ଅକ୍ଷର *name,
				   ext2_ino_t ino, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, blk64_t *pblkp)
{
	ସଂରଚନା dir_context ctx;
	ସଂରଚନା link_struct ls;
	errcode_t retval;

	retval = load_logical_dir_block(fs, dir, diri, blockcnt, pblkp, buf);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
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
	ଯଦି (ctx.errcode)
		ଫେରନ୍ତୁ ctx.errcode;
	ଯଦି (ls.err)
		ଫେରନ୍ତୁ ls.err;
	ଯଦି (!ls.done)
		ଫେରନ୍ତୁ EXT2_ET_DIR_NO_SPACE;
	ଫେରନ୍ତୁ 0;
}

ସଂରଚନା dx_hash_map {
	__u32 hash;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା size;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା off;
};

ସ୍ଥିର EXT2_QSORT_TYPE dx_hash_map_cmp(ଅଚଳ ଶୂନ୍ୟ *ap, ଅଚଳ ଶୂନ୍ୟ *bp)
{
	ଅଚଳ ସଂରଚନା dx_hash_map *a = ap, *b = bp;

	ଯଦି (a->hash < b->hash)
		ଫେରନ୍ତୁ -1;
	ଯଦି (a->hash > b->hash)
		ଫେରନ୍ତୁ 1;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t dx_move_dirents(ext2_filsys fs, ସଂରଚନା dx_hash_map *map,
				 ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଶୂନ୍ୟ *from, ଶୂନ୍ୟ *to)
{
	ସଂରଚନା ext2_dir_entry *de;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା rec_len = 0;
	errcode_t retval;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା csum_size = 0;
	ଶୂନ୍ୟ *base = to;

	ଯଦି (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = ଆକାର(ସଂରଚନା ext2_dir_entry_tail);

	ପାଇଁ (i = 0; i < ଗଣନା_2; i++) {
		de = (ସଂରଚନା ext2_dir_entry *) ((ଅକ୍ଷର *)from + map[i].off);
		rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(de));
		memcpy(to, de, rec_len);
		retval = ext2fs_set_rec_len(fs, rec_len, to);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
		to = (ଅକ୍ଷର *)to + rec_len;
	}
	/*
	 * Update rec_len of the last dir entry to stretch to the end of block
	 */
	to = (ଅକ୍ଷର *)to - rec_len;
	rec_len = fs->blocksize - ((ଅକ୍ଷର *)to - (ଅକ୍ଷର *)base) - csum_size;
	retval = ext2fs_set_rec_len(fs, rec_len, to);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	ଯଦି (csum_size)
		ext2fs_initialize_dirent_tail(fs,
				EXT2_DIRENT_TAIL(base, fs->blocksize));
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t dx_ଭର୍ତ୍ତି_entry(ext2_filsys fs, ext2_ino_t dir,
				 ସଂରଚନା dx_lookup_info *info, ପୂର୍ଣ୍ଣସଂଖ୍ୟା level,
				 __u32 hash, blk64_t lblk)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା pcount;
	ସଂରଚନା ext2_dx_entry *ଶୀର୍ଷ, *new;

	pcount = ext2fs_le16_to_cpu(info->frames[level].head->ଗଣନା_2);
	ଶୀର୍ଷ = info->frames[level].entries + pcount;
	new = info->frames[level].at + 1;
	memmove(new + 1, new, (ଅକ୍ଷର *)ଶୀର୍ଷ - (ଅକ୍ଷର *)new);
	new->hash = ext2fs_cpu_to_le32(hash);
	new->block = ext2fs_cpu_to_le32(lblk);
	info->frames[level].head->ଗଣନା_2 = ext2fs_cpu_to_le16(pcount + 1);
	ଫେରନ୍ତୁ ext2fs_write_dir_block4(fs, info->frames[level].pblock,
				       info->frames[level].buf, 0, dir);
}

ସ୍ଥିର errcode_t dx_split_leaf(ext2_filsys fs, ext2_ino_t dir,
			       ସଂରଚନା ext2_inode *diri,
			       ସଂରଚନା dx_lookup_info *info, ଶୂନ୍ୟ *buf,
			       blk64_t leaf_pblk, blk64_t new_lblk,
			       blk64_t new_pblk)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	ସଂରଚନା ext2_dir_entry *de;
	ଶୂନ୍ୟ *buf2;
	errcode_t retval = 0;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା rec_len;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset, move_size;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ଗଣନା_2 = 0;
	ସଂରଚନା dx_hash_map *map;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା continued;
	__u32 minor_hash;

	retval = ext2fs_get_mem(fs->blocksize, &buf2);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	retval = ext2fs_get_array(fs->blocksize / 12,
				  ଆକାର(ସଂରଚନା dx_hash_map), &map);
	ଯଦି (retval) {
		ext2fs_free_mem(&buf2);
		ଫେରନ୍ତୁ retval;
	}
	ପାଇଁ (offset = 0; offset < fs->blocksize; offset += rec_len) {
		de = (ସଂରଚନା ext2_dir_entry *) ((ଅକ୍ଷର *)buf + offset);
		retval = ext2fs_get_rec_len(fs, de, &rec_len);
		ଯଦି (retval)
			ଯାଅ out;
		ଯଦି (ext2fs_dirent_name_len(de) > 0 && de->inode) {
			map[ଗଣନା_2].off = offset;
			map[ଗଣନା_2].size = rec_len;
			retval = ext2fs_dirhash2(info->hash_alg, de->name,
					ext2fs_dirent_name_len(de),
					fs->encoding, hash_flags,
					fs->super->s_hash_seed,
					&(map[ଗଣନା_2].hash),
					&minor_hash);
			ଯଦି (retval)
				ଯାଅ out;
			ଗଣନା_2++;
		}
	}
	qsort(map, ଗଣନା_2, ଆକାର(ସଂରଚନା dx_hash_map), dx_hash_map_cmp);
	move_size = 0;
	/* Find place to split block */
	ପାଇଁ (i = ଗଣନା_2 - 1; i >= 0; i--) {
		ଯଦି (move_size + map[i].size / 2 > fs->blocksize / 2)
			ଭାଙ୍ଗ;
		move_size += map[i].size;
	}
	/* Let i be the first entry to move */
	i++;
	/* Move selected directory entries to new block */
	retval = dx_move_dirents(fs, map + i, ଗଣନା_2 - i, buf, buf2);
	ଯଦି (retval)
		ଯାଅ out;
	retval = ext2fs_write_dir_block4(fs, new_pblk, buf2, 0, dir);
	ଯଦି (retval)
		ଯାଅ out;
	/* Repack remaining entries in the old block */
	retval = dx_move_dirents(fs, map, i, buf, buf2);
	ଯଦି (retval)
		ଯାଅ out;
	retval = ext2fs_write_dir_block4(fs, leaf_pblk, buf2, 0, dir);
	ଯଦି (retval)
		ଯାଅ out;
	/* Update parent node */
	continued = map[i].hash == map[i-1].hash;
	retval = dx_ଭର୍ତ୍ତି_entry(fs, dir, info, info->levels - 1,
				 map[i].hash + continued, new_lblk);
out:
	ext2fs_free_mem(&buf2);
	ext2fs_free_mem(&map);
	ଫେରନ୍ତୁ retval;
}

ସ୍ଥିର errcode_t dx_grow_ବୃକ୍ଷ(ext2_filsys fs, ext2_ino_t dir,
			      ସଂରଚନା ext2_inode *diri,
			      ସଂରଚନା dx_lookup_info *info, ଶୂନ୍ୟ *buf,
			      blk64_t leaf_pblk)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	errcode_t retval;
	ext2_off64_t size = EXT2_I_SIZE(diri);
	blk64_t lblk, pblk;
	ସଂରଚନା ext2_dir_entry *de;
	ସଂରଚନା ext2_dx_countlimit *head;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା csum_size = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2;

	ଯଦି (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = ଆକାର(ସଂରଚନା ext2_dx_tail);

	/* Find level which can accommodate new child */
	ପାଇଁ (i = info->levels - 1; i >= 0; i--)
		ଯଦି (ext2fs_le16_to_cpu(info->frames[i].head->ଗଣନା_2) <
		    ext2fs_le16_to_cpu(info->frames[i].head->limit))
			ଭାଙ୍ଗ;
	/* Need to grow tree depth? */
	ଯଦି (i < 0 && info->levels >= ext2_dir_htree_level(fs))
		ଫେରନ୍ତୁ EXT2_ET_DIR_NO_SPACE;
	lblk = size / fs->blocksize;
	size += fs->blocksize;
	retval = ext2fs_inode_size_set(fs, diri, size);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	retval = ext2fs_fallocate(fs,
			EXT2_FALLOCATE_FORCE_INIT | EXT2_FALLOCATE_ZERO_BLOCKS,
			dir, diri, 0, lblk, 1);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	retval = ext2fs_write_inode(fs, dir, diri);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	retval = ext2fs_bmap2(fs, dir, diri, ଖାଲି, 0, lblk, ଖାଲି, &pblk);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	/* Only leaf addition needed? */
	ଯଦି (i == (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)info->levels - 1)
		ଫେରନ୍ତୁ dx_split_leaf(fs, dir, diri, info, buf, leaf_pblk,
				     lblk, pblk);

	de = buf;
	de->inode = 0;
	ext2fs_dirent_set_name_len(de, 0);
	ext2fs_dirent_set_file_type(de, 0);
	retval = ext2fs_set_rec_len(fs, fs->blocksize, de);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	head = (ସଂରଚନା ext2_dx_countlimit *) ((ଅକ୍ଷର *)buf + 8);
	ଗଣନା_2 = ext2fs_le16_to_cpu(info->frames[i+1].head->ଗଣନା_2);
	/* Growing tree depth? */
	ଯଦି (i < 0) {
		ସଂରଚନା ext2_dx_ମୂଳ_info *ମୂଳ;

		memcpy(head, info->frames[0].entries,
		       ଗଣନା_2 * ଆକାର(ସଂରଚନା ext2_dx_entry));
		head->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_size)) /
				ଆକାର(ସଂରଚନା ext2_dx_entry));
		/* head->count gets set by memcpy above to correct value */

		/* Now update tree root */
		info->frames[0].head->ଗଣନା_2 = ext2fs_cpu_to_le16(1);
		info->frames[0].entries[0].block = ext2fs_cpu_to_le32(lblk);
		ମୂଳ = (ସଂରଚନା ext2_dx_ମୂଳ_info *)
			((ଅକ୍ଷର *)info->frames[0].buf + EXT2_DX_ମୂଳ_OFF);
		ମୂଳ->indirect_levels++;
	} ଅନ୍ୟଥା {
		/* Splitting internal node in two */
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା count1 = ଗଣନା_2 / 2;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା count2 = ଗଣନା_2 - count1;
		__u32 split_hash = ext2fs_le32_to_cpu(info->frames[i+1].entries[count1].hash);

		memcpy(head, info->frames[i+1].entries + count1,
		       count2 * ଆକାର(ସଂରଚନା ext2_dx_entry));
		head->ଗଣନା_2 = ext2fs_cpu_to_le16(count2);
		head->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_size)) /
				ଆକାର(ସଂରଚନା ext2_dx_entry));
		info->frames[i+1].head->ଗଣନା_2 = ext2fs_cpu_to_le16(count1);

		/* Update parent node */
		retval = dx_ଭର୍ତ୍ତି_entry(fs, dir, info, i, split_hash, lblk);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;

	}
	/* Writeout split block / updated root */
	retval = ext2fs_write_dir_block4(fs, info->frames[i+1].pblock,
					 info->frames[i+1].buf, 0, dir);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	/* Writeout new tree block */
	retval = ext2fs_write_dir_block4(fs, pblk, buf, 0, dir);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t dx_link(ext2_filsys fs, ext2_ino_t dir,
			 ସଂରଚନା ext2_inode *diri, ଅଚଳ ଅକ୍ଷର *name,
			 ext2_ino_t ino, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା dx_lookup_info dx_info;
	errcode_t retval;
	ଶୂନ୍ୟ *blockbuf;
	unsigned restart = 0;
	blk64_t leaf_pblk;

	retval = ext2fs_get_mem(fs->blocksize, &blockbuf);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	dx_info.name = name;
	dx_info.namelen = strlen(name);
again:
	retval = dx_lookup(fs, dir, diri, &dx_info);
	ଯଦି (retval)
		ଯାଅ free_buf;

	retval = add_dirent_to_buf(fs,
		ext2fs_le32_to_cpu(dx_info.frames[dx_info.levels-1].at->block) & 0x0fffffff,
		blockbuf, dir, diri, name, ino, flags, &leaf_pblk);
	/*
	 * Success or error other than ENOSPC...? We are done. We may need upto
	 * two tries to add entry. One to split htree node and another to add
	 * new leaf block.
	 */
	ଯଦି (restart >= dx_info.levels || retval != EXT2_ET_DIR_NO_SPACE)
		ଯାଅ free_frames;
	retval = dx_grow_ବୃକ୍ଷ(fs, dir, diri, &dx_info, blockbuf, leaf_pblk);
	ଯଦି (retval)
		ଯାଅ free_frames;
	/* Restart everything now that the tree is larger */
	restart++;
	dx_release(&dx_info);
	ଯାଅ again;
free_frames:
	dx_release(&dx_info);
free_buf:
	ext2fs_free_mem(&blockbuf);
	ଫେରନ୍ତୁ retval;
}

ସଂରଚନା fake_dirent
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

ସଂରଚନା dx_ମୂଳ
{
	ସଂରଚନା fake_dirent dot;
	ଅକ୍ଷର dot_name[4];
	ସଂରଚନା fake_dirent dotdot;
	ଅକ୍ଷର dotdot_name[4];
	ସଂରଚନା ext2_dx_ମୂଳ_info info;
	ସଂରଚନା ext2_dx_entry entries[];
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା check_dx_ମୂଳ(ext2_filsys fs, ext2_ino_t ino, ସଂରଚନା dx_ମୂଳ *ମୂଳ)
{
	ସଂରଚନା fake_dirent *fde;
	ଅକ୍ଷର *error_ସନ୍ଦେଶ;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା rlen;
	ଅକ୍ଷର *blockend = (ଅକ୍ଷର *)ମୂଳ + fs->blocksize;

	fde = &ମୂଳ->dot;
	ଯଦି (fde->name_len != 1) {
		error_ସନ୍ଦେଶ = "invalid name_len for '.'";
		ଯାଅ corrupted;
	}
	ଯଦି (strncmp(ମୂଳ->dot_name, ".", fde->name_len)) {
		error_ସନ୍ଦେଶ = "invalid name for '.'";
		ଯାଅ corrupted;
	}

	ext2fs_get_rec_len(fs, (ସଂରଚନା ext2_dir_entry *)fde, &rlen);
	ଯଦି ((ଅକ୍ଷର *)fde + rlen >= blockend) {
		error_ସନ୍ଦେଶ = "invalid rec_len for '.'";
		ଯାଅ corrupted;
	}

	fde = &ମୂଳ->dotdot;
	ଯଦି (fde->name_len != 2) {
		error_ସନ୍ଦେଶ = "invalid name_len for '..'";
		ଯାଅ corrupted;
	}
	ଯଦି (strncmp(ମୂଳ->dotdot_name, "..", fde->name_len)) {
		error_ସନ୍ଦେଶ = "invalid name for '..'";
		ଯାଅ corrupted;
	}
	ext2fs_get_rec_len(fs, (ସଂରଚନା ext2_dir_entry *)fde, &rlen);
	ଯଦି ((ଅକ୍ଷର *)fde + rlen >= blockend) {
		error_ସନ୍ଦେଶ = "invalid rec_len for '..'";
		ଯାଅ corrupted;
	}

	ଫେରନ୍ତୁ 1;

corrupted:
	fprintf(stderr, "Corrupt dir %u: %s, running e2fsck is recommended\n",
			 ino, error_ସନ୍ଦେଶ);
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା ext2_dir_entry *
ext2fs_next_entry(ext2_filsys fs, ସଂରଚନା ext2_dir_entry *p)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା rlen;

	ext2fs_get_rec_len(fs, p, &rlen);
	ଫେରନ୍ତୁ (ସଂରଚନା ext2_dir_entry *)((ଅକ୍ଷର *)p + rlen);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ dx_set_block(ସଂରଚନା ext2_dx_entry *entry, unsigned ମୂଲ୍ୟ)
{
	entry->block = ext2fs_cpu_to_le32(ମୂଲ୍ୟ);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ dx_set_ଗଣନା(ସଂରଚନା ext2_dx_entry *entries, unsigned ମୂଲ୍ୟ)
{
	((ସଂରଚନା ext2_dx_countlimit *) entries)->ଗଣନା_2 = ext2fs_cpu_to_le16(ମୂଲ୍ୟ);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ dx_set_limit(ସଂରଚନା ext2_dx_entry *entries, unsigned ମୂଲ୍ୟ)
{
	((ସଂରଚନା ext2_dx_countlimit *) entries)->limit = ext2fs_cpu_to_le16(ମୂଲ୍ୟ);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned dx_ମୂଳ_limit(ext2_filsys fs, unsigned infosize)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା entry_space = fs->blocksize -
			ext2fs_dir_rec_len(1, 0) -
			ext2fs_dir_rec_len(2, 0) - infosize;

	ଯଦି (ext2fs_has_feature_metadata_csum(fs->super))
		entry_space -= ଆକାର(ସଂରଚନା ext2_dx_tail);
	ଫେରନ୍ତୁ entry_space / ଆକାର(ସଂରଚନା ext2_dx_entry);
}

/*
 * This converts a one block unindexed directory to a 3 block indexed
 * directory, and adds the dentry to the indexed directory.  Returns 0 if the
 * index was not created; EAGAIN if it was; or an errcode_t on error.
 */
ସ୍ଥିର errcode_t try_make_indexed_dir(ext2_filsys fs, ext2_ino_t dir,
				      ଅଚଳ ଅକ୍ଷର *name, ext2_ino_t ino,
				      ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା ext2_inode inode;
	ଅକ୍ଷର *buf0, *buf1, *ଶୀର୍ଷ;
	ସଂରଚନା dx_ମୂଳ *ମୂଳ;
	ସଂରଚନା ext2_dx_entry *entries;
	ସଂରଚନା fake_dirent *fde;
	ସଂରଚନା ext2_dir_entry *de, *de2;
	blk64_t pblk0, pblk1;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
	ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା blocksize = fs->blocksize;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା csum_size = 0;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	ଯଦି (inode.i_size > fs->blocksize || (inode.i_flags & EXT2_INDEX_FL))
		ଫେରନ୍ତୁ 0;

	ଯଦି (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = ଆକାର(ସଂରଚନା ext2_dir_entry_tail);

	retval = ext2fs_get_mem(blocksize, &buf0);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	retval = ext2fs_get_mem(blocksize, &buf1);
	ଯଦି (retval)
		ଯାଅ out_buf0;

	retval = ext2fs_bmap2(fs, dir, &inode, ଖାଲି, 0, 0, ଖାଲି, &pblk0);
	ଯଦି (retval)
		ଯାଅ out_buf1;

	retval = ext2fs_read_dir_block4(fs, pblk0, buf0, 0, dir);
	ଯଦି (retval)
		ଯାଅ out_buf1;

	ମୂଳ = (ସଂରଚନା dx_ମୂଳ *)buf0;
	ଯଦି (!check_dx_ମୂଳ(fs, dir, ମୂଳ)) {
		retval = EXT2_ET_DIR_CORRUPTED;
		ଯାଅ out_buf1;
	}

	/* The 0th block becomes the root, move the dirents out */
	fde = &ମୂଳ->dotdot;
	de = ext2fs_next_entry(fs, (ସଂରଚନା ext2_dir_entry *)fde);
	len = ((ଅକ୍ଷର *) ମୂଳ) + (blocksize - csum_size) - (ଅକ୍ଷର *) de;

	/* Allocate new block for the 0th block's dirents */
	retval = ext2fs_bmap2(fs, dir, &inode, ଖାଲି, BMAP_ALLOC | BMAP_ZERO, 1,
			      ଖାଲି, &pblk1);
	ଯଦି (retval)
		ଯାଅ out_buf1;

	memcpy(buf1, de, len);
	memset(de, 0, len); /* wipe old data */

	de = (ସଂରଚନା ext2_dir_entry *)buf1;
	ଶୀର୍ଷ = buf1 + len;
	ଯେତେବେଳେ ((ଅକ୍ଷର *)(de2 = ext2fs_next_entry(fs, de)) < ଶୀର୍ଷ) {
#ଯଦି 0
		ଯଦି (ext4_check_dir_entry(dir, ଖାଲି, de, bh2, buf1, len,
					(ଅକ୍ଷର *)de - buf1)) {
			retval = EXT2_ET_DIR_CORRUPTED;
			ଯାଅ out_buf1;
		}
#endif
		de = de2;
	}
	retval = ext2fs_set_rec_len(fs,
			buf1 + (blocksize - csum_size) - (ଅକ୍ଷର *) de, de);
	ଯଦି (retval)
		ଯାଅ out_buf1;

	ଯଦି (csum_size)
		ext2fs_initialize_dirent_tail(fs,
				EXT2_DIRENT_TAIL(buf1, fs->blocksize));

	/* Initialize the root; the dot dirents already exist */
	de = (ସଂରଚନା ext2_dir_entry *) (&ମୂଳ->dotdot);
	retval = ext2fs_set_rec_len(fs, blocksize - ext2fs_dir_rec_len(2, 0),
				    de);
	memset (&ମୂଳ->info, 0, ଆକାର(ମୂଳ->info));
	ମୂଳ->info.info_length = ଆକାର(ମୂଳ->info);
	ଯଦି (ext4_hash_in_dirent(&inode))
		ମୂଳ->info.hash_version = EXT2_HASH_SIPHASH;
	ଅନ୍ୟଥା
		ମୂଳ->info.hash_version = fs->super->s_def_hash_version;

	entries = ମୂଳ->entries;
	dx_set_block(entries, 1);
	dx_set_ଗଣନା(entries, 1);
	dx_set_limit(entries, dx_ମୂଳ_limit(fs, ଆକାର(ମୂଳ->info)));

	retval = ext2fs_write_dir_block4(fs, pblk1, buf1, 0, dir);
	ଯଦି (retval)
		ଯାଅ out_buf1;

	retval = ext2fs_write_dir_block4(fs, pblk0, buf0, 0, dir);
	ଯଦି (retval)
		ଯାଅ out_buf1;

	inode.i_flags |= EXT2_INDEX_FL;
	inode.i_size += fs->blocksize;
	retval = ext2fs_write_inode(fs, dir, &inode);
	ଯଦି (retval)
		ଯାଅ out_buf1;

	retval = EAGAIN;
out_buf1:
	ext2fs_free_mem(&buf1);
out_buf0:
	ext2fs_free_mem(&buf0);
	ଫେରନ୍ତୁ retval;
}

/*
 * Note: the low 3 bits of the flags field are used as the directory
 * entry filetype.
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, ଅଚଳ ଅକ୍ଷର *name,
		      ext2_ino_t ino, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	errcode_t		retval;
	ସଂରଚନା link_struct	ls;
	ସଂରଚନା ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ଯଦି (!(fs->flags & EXT2_FLAG_RW))
		ଫେରନ୍ତୁ EXT2_ET_RO_FILSYS;

retry:
	ଯଦି ((retval = ext2fs_read_inode(fs, dir, &inode)) != 0)
		ଫେରନ୍ତୁ retval;

	ଯଦି (inode.i_flags & EXT2_INDEX_FL)
		ଫେରନ୍ତୁ dx_link(fs, dir, &inode, name, ino, flags);

	ls.fs = fs;
	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	ଯଦି ((flags & EXT2FS_LINK_APPEND) &&
	    !(inode.i_flags & EXT4_INLINE_DATA_FL)) {
		blk64_t		lblk, pblk = 0;
		ସଂରଚନା dir_context ctx;

		lblk = (inode.i_size / fs->blocksize) - 1;
		retval = ext2fs_bmap2(fs, dir, &inode, ଖାଲି, 0, lblk,
				      ଖାଲି, &pblk);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;

		ctx.dir = dir;
		ctx.flags = DIRENT_FLAG_INCLUDE_EMPTY;
		ctx.func = link_proc;
		ctx.priv_data = &ls;
		ctx.errcode = 0;
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;

		ext2fs_process_dir_block(fs, &pblk, lblk, 0, 0, &ctx);
		retval = ctx.errcode;
		ext2fs_free_mem(&ctx.buf);
	} ଅନ୍ୟଥା {
		retval = ext2fs_dir_iterate2(fs, dir,
					     DIRENT_FLAG_INCLUDE_EMPTY,
					     ଖାଲି, link_proc, &ls);
	}
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;
	ଯଦି (ls.err)
		ଫେରନ୍ତୁ ls.err;

	ଯଦି (!ls.done) {
		ଯଦି (!(flags & EXT2FS_LINK_EXPAND))
			ଫେରନ୍ତୁ EXT2_ET_DIR_NO_SPACE;

		ଯଦି (ext2fs_has_feature_dir_index(fs->super)) {
			retval = try_make_indexed_dir(fs, dir, name, ino,
						      flags);
			ଯଦି (retval == EAGAIN)
				ଯାଅ retry;
			ଯଦି (retval)
				ଫେରନ୍ତୁ retval;
		}

		retval = ext2fs_expand_dir(fs, dir);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
		ଯାଅ retry;
	}
	ଫେରନ୍ତୁ 0;
}

/* Does this directory have an htree index? */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_dir_is_dx(ext2_filsys fs, ଅଚଳ ସଂରଚନା ext2_inode *inode)
{
	ଫେରନ୍ତୁ ext2fs_has_feature_dir_index(fs->super) &&
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
ଶୂନ୍ୟ ext2fs_inc_nlink(ext2_filsys fs, ସଂରଚନା ext2_inode *inode)
{
	inode->i_links_ଗଣନା++;

	ଯଦି (ext2fs_dir_is_dx(fs, inode) &&
	    (inode->i_links_ଗଣନା > EXT2_LINK_MAX || inode->i_links_ଗଣନା == 2))
		inode->i_links_ଗଣନା = 1;
}

/*
 * If a directory had nlink == 1, then we should let it be 1. This indicates
 * directory has >EXT2_LINK_MAX subdirs.
 */
ଶୂନ୍ୟ ext2fs_dec_nlink(ସଂରଚନା ext2_inode *inode)
{
	ଯଦି (!S_ISDIR(inode->i_mode) || inode->i_links_ଗଣନା > 2)
		inode->i_links_ଗଣନା--;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_dir_link_max(ext2_filsys fs, ସଂରଚନା ext2_inode_large *inode)
{
	ଫେରନ୍ତୁ inode->i_links_ଗଣନା >= EXT2_LINK_MAX &&
	       !(ext2fs_dir_is_dx(fs, EXT2_INODE(inode)) &&
		 ext2fs_has_feature_dir_nlink(fs->super));
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_dir_link_empty(ସଂରଚନା ext2_inode *inode)
{
	ଫେରନ୍ତୁ inode->i_links_ଗଣନା == 2 || inode->i_links_ଗଣନା == 1;
}
