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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

#define EXT2_DX_ریشه_OFF 24

ساختار dx_frame {
	تهی *buf;
	blk64_t pblock;
	ساختار ext2_dx_countlimit *head;
	ساختار ext2_dx_entry *entries;
	ساختار ext2_dx_entry *at;
};

ساختار dx_lookup_info {
	ثابت نویسه *name;
	صحیح namelen;
	صحیح hash_alg;
	__u32 hash;
	unsigned levels;
	ساختار dx_frame frames[EXT4_HTREE_LEVEL];
};

ایستا errcode_t alloc_dx_frame(ext2_filsys fs, ساختار dx_frame *frame)
{
	بازگشت ext2fs_get_mem(fs->blocksize, &frame->buf);
}

ایستا تهی dx_release(ساختار dx_lookup_info *info)
{
	unsigned level;

	برای (level = 0; level < info->levels; level++) {
		اگر (info->frames[level].buf == پوچ)
			بشکن;
		ext2fs_free_mem(&(info->frames[level].buf));
	}
	info->levels = 0;
}

ایستا تهی dx_جستجو_entry(ساختار dx_frame *frame, صحیح شمار, __u32 hash)
{
	ساختار ext2_dx_entry *p, *q, *m;

	p = frame->entries + 1;
	q = frame->entries + شمار - 1;
	وقتی (p <= q) {
		m = p + (q - p) / 2;
		اگر (ext2fs_le32_to_cpu(m->hash) > hash)
			q = m - 1;
		وگرنه
			p = m + 1;
	}
	frame->at = p - 1;
}

ایستا errcode_t load_logical_dir_block(ext2_filsys fs, ext2_ino_t dir,
					ساختار ext2_inode *diri, blk64_t block,
					blk64_t *pblk, تهی *buf)
{
	errcode_t errcode;
	صحیح ret_flags;

	errcode = ext2fs_bmap2(fs, dir, diri, پوچ, 0, block, &ret_flags,
			       pblk);
	اگر (errcode)
		بازگشت errcode;
	اگر (ret_flags & BMAP_RET_UNINIT)
		بازگشت EXT2_ET_DIR_CORRUPTED;
	بازگشت ext2fs_read_dir_block4(fs, *pblk, buf, 0, dir);
}

ایستا errcode_t dx_lookup(ext2_filsys fs, ext2_ino_t dir,
			   ساختار ext2_inode *diri, ساختار dx_lookup_info *info)
{
	ساختار ext2_dx_ریشه_info *ریشه;
	errcode_t errcode;
	صحیح level = 0;
	صحیح شمار, limit;
	صحیح hash_alg;
	صحیح hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	__u32 minor_hash;
	ساختار dx_frame *frame;

	errcode = alloc_dx_frame(fs, &(info->frames[0]));
	اگر (errcode)
		بازگشت errcode;
	info->levels = 1;

	errcode = load_logical_dir_block(fs, dir, diri, 0,
					 &(info->frames[0].pblock),
					 info->frames[0].buf);
	اگر (errcode)
		برو out_err;
	ریشه = (ساختار ext2_dx_ریشه_info *) ((نویسه *)info->frames[0].buf +
					     EXT2_DX_ریشه_OFF);
	hash_alg = ریشه->hash_version;
	اگر (hash_alg != EXT2_HASH_TEA && hash_alg != EXT2_HASH_HALF_MD4 &&
	    hash_alg != EXT2_HASH_LEGACY) {
		errcode = EXT2_ET_DIRHASH_UNSUPP;
		برو out_err;
	}
	اگر (hash_alg <= EXT2_HASH_TEA &&
	    fs->super->s_flags & EXT2_FLAGS_UNSIGNED_HASH)
		hash_alg += 3;
	اگر (ریشه->indirect_levels >= ext2_dir_htree_level(fs)) {
		errcode = EXT2_ET_DIR_CORRUPTED;
		برو out_err;
	}
	info->hash_alg = hash_alg;

	errcode = ext2fs_dirhash2(hash_alg, info->name, info->namelen,
				  fs->encoding, hash_flags,
				  fs->super->s_hash_seed, &info->hash,
				  &minor_hash);
	اگر (errcode)
		برو out_err;

	برای (level = 0; level <= ریشه->indirect_levels; level++) {
		frame = &(info->frames[level]);
		اگر (level > 0) {
			errcode = alloc_dx_frame(fs, frame);
			اگر (errcode)
				برو out_err;
			info->levels++;

			errcode = load_logical_dir_block(fs, dir, diri,
				ext2fs_le32_to_cpu(info->frames[level-1].at->block) & 0x0fffffff,
				&(frame->pblock), frame->buf);
			اگر (errcode)
				برو out_err;
		}
		errcode = ext2fs_get_dx_countlimit(fs, frame->buf,
						   &(frame->head), پوچ);
		اگر (errcode)
			برو out_err;
		شمار = ext2fs_le16_to_cpu(frame->head->شمار);
		limit = ext2fs_le16_to_cpu(frame->head->limit);
		frame->entries = (ساختار ext2_dx_entry *)(frame->head);
		اگر (!شمار || شمار > limit) {
			errcode = EXT2_ET_DIR_CORRUPTED;
			برو out_err;
		}

		dx_جستجو_entry(frame, شمار, info->hash);
	}
	بازگشت 0;
out_err:
	dx_release(info);
	بازگشت errcode;
}

ساختار link_struct  {
	ext2_filsys	fs;
	ثابت نویسه	*name;
	صحیح		namelen;
	ext2_ino_t	inode;
	صحیح		flags;
	صحیح		done;
	unsigned صحیح	blocksize;
	errcode_t	err;
	ساختار ext2_super_block *sb;
};

ایستا صحیح link_proc(ext2_ino_t dir EXT2FS_ATTR((unused)),
		     صحیح entru EXT2FS_ATTR((unused)),
		     ساختار ext2_dir_entry *dirent,
		     صحیح	offset,
		     صحیح	blocksize,
		     نویسه	*buf,
		     تهی	*priv_data)
{
	ساختار link_struct *ls = (ساختار link_struct *) priv_data;
	ساختار ext2_dir_entry *next;
	unsigned صحیح rec_len, min_rec_len, curr_rec_len;
	صحیح ret = 0;
	صحیح csum_size = 0;

	اگر (ls->done)
		بازگشت DIRENT_ABORT;

	rec_len = EXT2_DIR_REC_LEN(ls->namelen);

	ls->err = ext2fs_get_rec_len(ls->fs, dirent, &curr_rec_len);
	اگر (ls->err)
		بازگشت DIRENT_ABORT;

	اگر (ext2fs_has_feature_metadata_csum(ls->fs->super))
		csum_size = اندازه(ساختار ext2_dir_entry_tail);
	/*
	 * See if the following directory entry (if any) is unused;
	 * if so, absorb it into this one.
	 */
	next = (ساختار ext2_dir_entry *) (buf + offset + curr_rec_len);
	اگر ((offset + (صحیح) curr_rec_len < blocksize - (8 + csum_size)) &&
	    (next->inode == 0) &&
	    (offset + (صحیح) curr_rec_len + (صحیح) next->rec_len <= blocksize)) {
		curr_rec_len += next->rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, curr_rec_len, dirent);
		اگر (ls->err)
			بازگشت DIRENT_ABORT;
		ret = DIRENT_CHANGED;
	}

	/*
	 * If the directory entry is used, see if we can split the
	 * directory entry to make room for the new name.  If so,
	 * truncate it and return.
	 */
	اگر (dirent->inode) {
		min_rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(dirent));
		اگر (curr_rec_len < (min_rec_len + rec_len))
			بازگشت ret;
		rec_len = curr_rec_len - min_rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, min_rec_len, dirent);
		اگر (ls->err)
			بازگشت DIRENT_ABORT;
		next = (ساختار ext2_dir_entry *) (buf + offset +
						  dirent->rec_len);
		next->inode = 0;
		ext2fs_dirent_set_name_len(next, 0);
		ext2fs_dirent_set_file_type(next, 0);
		ls->err = ext2fs_set_rec_len(ls->fs, rec_len, next);
		اگر (ls->err)
			بازگشت DIRENT_ABORT;
		بازگشت DIRENT_CHANGED;
	}

	/*
	 * If we get this far, then the directory entry is not used.
	 * See if we can fit the request entry in.  If so, do it.
	 */
	اگر (curr_rec_len < rec_len)
		بازگشت ret;
	dirent->inode = ls->inode;
	ext2fs_dirent_set_name_len(dirent, ls->namelen);
	strncpy(dirent->name, ls->name, ls->namelen);
	اگر (ext2fs_has_feature_filetype(ls->sb))
		ext2fs_dirent_set_file_type(dirent,
					    ls->flags & EXT2FS_LINK_FT_MASK);

	ls->done++;
	بازگشت DIRENT_ABORT|DIRENT_CHANGED;
}

ایستا errcode_t add_dirent_to_buf(ext2_filsys fs, e2_blkcnt_t blockcnt,
				   نویسه *buf, ext2_ino_t dir,
				   ساختار ext2_inode *diri, ثابت نویسه *name,
				   ext2_ino_t ino, صحیح flags, blk64_t *pblkp)
{
	ساختار dir_context ctx;
	ساختار link_struct ls;
	errcode_t retval;

	retval = load_logical_dir_block(fs, dir, diri, blockcnt, pblkp, buf);
	اگر (retval)
		بازگشت retval;
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
	اگر (ctx.errcode)
		بازگشت ctx.errcode;
	اگر (ls.err)
		بازگشت ls.err;
	اگر (!ls.done)
		بازگشت EXT2_ET_DIR_NO_SPACE;
	بازگشت 0;
}

ساختار dx_hash_map {
	__u32 hash;
	صحیح size;
	صحیح off;
};

ایستا EXT2_QSORT_TYPE dx_hash_map_cmp(ثابت تهی *ap, ثابت تهی *bp)
{
	ثابت ساختار dx_hash_map *a = ap, *b = bp;

	اگر (a->hash < b->hash)
		بازگشت -1;
	اگر (a->hash > b->hash)
		بازگشت 1;
	بازگشت 0;
}

ایستا errcode_t dx_move_dirents(ext2_filsys fs, ساختار dx_hash_map *map,
				 صحیح شمار, تهی *from, تهی *to)
{
	ساختار ext2_dir_entry *de;
	صحیح i;
	صحیح rec_len = 0;
	errcode_t retval;
	صحیح csum_size = 0;
	تهی *base = to;

	اگر (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = اندازه(ساختار ext2_dir_entry_tail);

	برای (i = 0; i < شمار; i++) {
		de = (ساختار ext2_dir_entry *) ((نویسه *)from + map[i].off);
		rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(de));
		memcpy(to, de, rec_len);
		retval = ext2fs_set_rec_len(fs, rec_len, to);
		اگر (retval)
			بازگشت retval;
		to = (نویسه *)to + rec_len;
	}
	/*
	 * Update rec_len of the last dir entry to stretch to the end of block
	 */
	to = (نویسه *)to - rec_len;
	rec_len = fs->blocksize - ((نویسه *)to - (نویسه *)base) - csum_size;
	retval = ext2fs_set_rec_len(fs, rec_len, to);
	اگر (retval)
		بازگشت retval;
	اگر (csum_size)
		ext2fs_initialize_dirent_tail(fs,
				EXT2_DIRENT_TAIL(base, fs->blocksize));
	بازگشت 0;
}

ایستا errcode_t dx_درج_entry(ext2_filsys fs, ext2_ino_t dir,
				 ساختار dx_lookup_info *info, صحیح level,
				 __u32 hash, blk64_t lblk)
{
	صحیح pcount;
	ساختار ext2_dx_entry *بالاترین, *new;

	pcount = ext2fs_le16_to_cpu(info->frames[level].head->شمار);
	بالاترین = info->frames[level].entries + pcount;
	new = info->frames[level].at + 1;
	memmove(new + 1, new, (نویسه *)بالاترین - (نویسه *)new);
	new->hash = ext2fs_cpu_to_le32(hash);
	new->block = ext2fs_cpu_to_le32(lblk);
	info->frames[level].head->شمار = ext2fs_cpu_to_le16(pcount + 1);
	بازگشت ext2fs_write_dir_block4(fs, info->frames[level].pblock,
				       info->frames[level].buf, 0, dir);
}

ایستا errcode_t dx_split_leaf(ext2_filsys fs, ext2_ino_t dir,
			       ساختار ext2_inode *diri,
			       ساختار dx_lookup_info *info, تهی *buf,
			       blk64_t leaf_pblk, blk64_t new_lblk,
			       blk64_t new_pblk)
{
	صحیح hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	ساختار ext2_dir_entry *de;
	تهی *buf2;
	errcode_t retval = 0;
	unsigned صحیح rec_len;
	unsigned صحیح offset, move_size;
	صحیح i, شمار = 0;
	ساختار dx_hash_map *map;
	صحیح continued;
	__u32 minor_hash;

	retval = ext2fs_get_mem(fs->blocksize, &buf2);
	اگر (retval)
		بازگشت retval;
	retval = ext2fs_get_array(fs->blocksize / 12,
				  اندازه(ساختار dx_hash_map), &map);
	اگر (retval) {
		ext2fs_free_mem(&buf2);
		بازگشت retval;
	}
	برای (offset = 0; offset < fs->blocksize; offset += rec_len) {
		de = (ساختار ext2_dir_entry *) ((نویسه *)buf + offset);
		retval = ext2fs_get_rec_len(fs, de, &rec_len);
		اگر (retval)
			برو out;
		اگر (ext2fs_dirent_name_len(de) > 0 && de->inode) {
			map[شمار].off = offset;
			map[شمار].size = rec_len;
			retval = ext2fs_dirhash2(info->hash_alg, de->name,
					ext2fs_dirent_name_len(de),
					fs->encoding, hash_flags,
					fs->super->s_hash_seed,
					&(map[شمار].hash),
					&minor_hash);
			اگر (retval)
				برو out;
			شمار++;
		}
	}
	qsort(map, شمار, اندازه(ساختار dx_hash_map), dx_hash_map_cmp);
	move_size = 0;
	/* Find place to split block */
	برای (i = شمار - 1; i >= 0; i--) {
		اگر (move_size + map[i].size / 2 > fs->blocksize / 2)
			بشکن;
		move_size += map[i].size;
	}
	/* Let i be the first entry to move */
	i++;
	/* Move selected directory entries to new block */
	retval = dx_move_dirents(fs, map + i, شمار - i, buf, buf2);
	اگر (retval)
		برو out;
	retval = ext2fs_write_dir_block4(fs, new_pblk, buf2, 0, dir);
	اگر (retval)
		برو out;
	/* Repack remaining entries in the old block */
	retval = dx_move_dirents(fs, map, i, buf, buf2);
	اگر (retval)
		برو out;
	retval = ext2fs_write_dir_block4(fs, leaf_pblk, buf2, 0, dir);
	اگر (retval)
		برو out;
	/* Update parent node */
	continued = map[i].hash == map[i-1].hash;
	retval = dx_درج_entry(fs, dir, info, info->levels - 1,
				 map[i].hash + continued, new_lblk);
out:
	ext2fs_free_mem(&buf2);
	ext2fs_free_mem(&map);
	بازگشت retval;
}

ایستا errcode_t dx_grow_درخت(ext2_filsys fs, ext2_ino_t dir,
			      ساختار ext2_inode *diri,
			      ساختار dx_lookup_info *info, تهی *buf,
			      blk64_t leaf_pblk)
{
	صحیح i;
	errcode_t retval;
	ext2_off64_t size = EXT2_I_SIZE(diri);
	blk64_t lblk, pblk;
	ساختار ext2_dir_entry *de;
	ساختار ext2_dx_countlimit *head;
	صحیح csum_size = 0;
	صحیح شمار;

	اگر (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = اندازه(ساختار ext2_dx_tail);

	/* Find level which can accommodate new child */
	برای (i = info->levels - 1; i >= 0; i--)
		اگر (ext2fs_le16_to_cpu(info->frames[i].head->شمار) <
		    ext2fs_le16_to_cpu(info->frames[i].head->limit))
			بشکن;
	/* Need to grow tree depth? */
	اگر (i < 0 && info->levels >= ext2_dir_htree_level(fs))
		بازگشت EXT2_ET_DIR_NO_SPACE;
	lblk = size / fs->blocksize;
	size += fs->blocksize;
	retval = ext2fs_inode_size_set(fs, diri, size);
	اگر (retval)
		بازگشت retval;
	retval = ext2fs_fallocate(fs,
			EXT2_FALLOCATE_FORCE_INIT | EXT2_FALLOCATE_ZERO_BLOCKS,
			dir, diri, 0, lblk, 1);
	اگر (retval)
		بازگشت retval;
	retval = ext2fs_write_inode(fs, dir, diri);
	اگر (retval)
		بازگشت retval;
	retval = ext2fs_bmap2(fs, dir, diri, پوچ, 0, lblk, پوچ, &pblk);
	اگر (retval)
		بازگشت retval;
	/* Only leaf addition needed? */
	اگر (i == (صحیح)info->levels - 1)
		بازگشت dx_split_leaf(fs, dir, diri, info, buf, leaf_pblk,
				     lblk, pblk);

	de = buf;
	de->inode = 0;
	ext2fs_dirent_set_name_len(de, 0);
	ext2fs_dirent_set_file_type(de, 0);
	retval = ext2fs_set_rec_len(fs, fs->blocksize, de);
	اگر (retval)
		بازگشت retval;
	head = (ساختار ext2_dx_countlimit *) ((نویسه *)buf + 8);
	شمار = ext2fs_le16_to_cpu(info->frames[i+1].head->شمار);
	/* Growing tree depth? */
	اگر (i < 0) {
		ساختار ext2_dx_ریشه_info *ریشه;

		memcpy(head, info->frames[0].entries,
		       شمار * اندازه(ساختار ext2_dx_entry));
		head->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_size)) /
				اندازه(ساختار ext2_dx_entry));
		/* head->count gets set by memcpy above to correct value */

		/* Now update tree root */
		info->frames[0].head->شمار = ext2fs_cpu_to_le16(1);
		info->frames[0].entries[0].block = ext2fs_cpu_to_le32(lblk);
		ریشه = (ساختار ext2_dx_ریشه_info *)
			((نویسه *)info->frames[0].buf + EXT2_DX_ریشه_OFF);
		ریشه->indirect_levels++;
	} وگرنه {
		/* Splitting internal node in two */
		صحیح count1 = شمار / 2;
		صحیح count2 = شمار - count1;
		__u32 split_hash = ext2fs_le32_to_cpu(info->frames[i+1].entries[count1].hash);

		memcpy(head, info->frames[i+1].entries + count1,
		       count2 * اندازه(ساختار ext2_dx_entry));
		head->شمار = ext2fs_cpu_to_le16(count2);
		head->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_size)) /
				اندازه(ساختار ext2_dx_entry));
		info->frames[i+1].head->شمار = ext2fs_cpu_to_le16(count1);

		/* Update parent node */
		retval = dx_درج_entry(fs, dir, info, i, split_hash, lblk);
		اگر (retval)
			بازگشت retval;

	}
	/* Writeout split block / updated root */
	retval = ext2fs_write_dir_block4(fs, info->frames[i+1].pblock,
					 info->frames[i+1].buf, 0, dir);
	اگر (retval)
		بازگشت retval;
	/* Writeout new tree block */
	retval = ext2fs_write_dir_block4(fs, pblk, buf, 0, dir);
	اگر (retval)
		بازگشت retval;
	بازگشت 0;
}

ایستا errcode_t dx_link(ext2_filsys fs, ext2_ino_t dir,
			 ساختار ext2_inode *diri, ثابت نویسه *name,
			 ext2_ino_t ino, صحیح flags)
{
	ساختار dx_lookup_info dx_info;
	errcode_t retval;
	تهی *blockbuf;
	unsigned restart = 0;
	blk64_t leaf_pblk;

	retval = ext2fs_get_mem(fs->blocksize, &blockbuf);
	اگر (retval)
		بازگشت retval;

	dx_info.name = name;
	dx_info.namelen = strlen(name);
again:
	retval = dx_lookup(fs, dir, diri, &dx_info);
	اگر (retval)
		برو free_buf;

	retval = add_dirent_to_buf(fs,
		ext2fs_le32_to_cpu(dx_info.frames[dx_info.levels-1].at->block) & 0x0fffffff,
		blockbuf, dir, diri, name, ino, flags, &leaf_pblk);
	/*
	 * Success or error other than ENOSPC...? We are done. We may need upto
	 * two tries to add entry. One to split htree node and another to add
	 * new leaf block.
	 */
	اگر (restart >= dx_info.levels || retval != EXT2_ET_DIR_NO_SPACE)
		برو free_frames;
	retval = dx_grow_درخت(fs, dir, diri, &dx_info, blockbuf, leaf_pblk);
	اگر (retval)
		برو free_frames;
	/* Restart everything now that the tree is larger */
	restart++;
	dx_release(&dx_info);
	برو again;
free_frames:
	dx_release(&dx_info);
free_buf:
	ext2fs_free_mem(&blockbuf);
	بازگشت retval;
}

ساختار fake_dirent
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

ساختار dx_ریشه
{
	ساختار fake_dirent dot;
	نویسه dot_name[4];
	ساختار fake_dirent dotdot;
	نویسه dotdot_name[4];
	ساختار ext2_dx_ریشه_info info;
	ساختار ext2_dx_entry entries[];
};

ایستا صحیح check_dx_ریشه(ext2_filsys fs, ext2_ino_t ino, ساختار dx_ریشه *ریشه)
{
	ساختار fake_dirent *fde;
	نویسه *error_پیام;
	unsigned صحیح rlen;
	نویسه *blockend = (نویسه *)ریشه + fs->blocksize;

	fde = &ریشه->dot;
	اگر (fde->name_len != 1) {
		error_پیام = "invalid name_len for '.'";
		برو corrupted;
	}
	اگر (strncmp(ریشه->dot_name, ".", fde->name_len)) {
		error_پیام = "invalid name for '.'";
		برو corrupted;
	}

	ext2fs_get_rec_len(fs, (ساختار ext2_dir_entry *)fde, &rlen);
	اگر ((نویسه *)fde + rlen >= blockend) {
		error_پیام = "invalid rec_len for '.'";
		برو corrupted;
	}

	fde = &ریشه->dotdot;
	اگر (fde->name_len != 2) {
		error_پیام = "invalid name_len for '..'";
		برو corrupted;
	}
	اگر (strncmp(ریشه->dotdot_name, "..", fde->name_len)) {
		error_پیام = "invalid name for '..'";
		برو corrupted;
	}
	ext2fs_get_rec_len(fs, (ساختار ext2_dir_entry *)fde, &rlen);
	اگر ((نویسه *)fde + rlen >= blockend) {
		error_پیام = "invalid rec_len for '..'";
		برو corrupted;
	}

	بازگشت 1;

corrupted:
	fprintf(stderr, "Corrupt dir %u: %s, running e2fsck is recommended\n",
			 ino, error_پیام);
	بازگشت 0;
}

ایستا درونخطی ساختار ext2_dir_entry *
ext2fs_next_entry(ext2_filsys fs, ساختار ext2_dir_entry *p)
{
	unsigned صحیح rlen;

	ext2fs_get_rec_len(fs, p, &rlen);
	بازگشت (ساختار ext2_dir_entry *)((نویسه *)p + rlen);
}

ایستا درونخطی تهی dx_set_block(ساختار ext2_dx_entry *entry, unsigned مقدار)
{
	entry->block = ext2fs_cpu_to_le32(مقدار);
}

ایستا درونخطی تهی dx_set_شمار(ساختار ext2_dx_entry *entries, unsigned مقدار)
{
	((ساختار ext2_dx_countlimit *) entries)->شمار = ext2fs_cpu_to_le16(مقدار);
}

ایستا درونخطی تهی dx_set_limit(ساختار ext2_dx_entry *entries, unsigned مقدار)
{
	((ساختار ext2_dx_countlimit *) entries)->limit = ext2fs_cpu_to_le16(مقدار);
}

ایستا درونخطی unsigned dx_ریشه_limit(ext2_filsys fs, unsigned infosize)
{
	unsigned صحیح entry_space = fs->blocksize -
			ext2fs_dir_rec_len(1, 0) -
			ext2fs_dir_rec_len(2, 0) - infosize;

	اگر (ext2fs_has_feature_metadata_csum(fs->super))
		entry_space -= اندازه(ساختار ext2_dx_tail);
	بازگشت entry_space / اندازه(ساختار ext2_dx_entry);
}

/*
 * This converts a one block unindexed directory to a 3 block indexed
 * directory, and adds the dentry to the indexed directory.  Returns 0 if the
 * index was not created; EAGAIN if it was; or an errcode_t on error.
 */
ایستا errcode_t try_make_indexed_dir(ext2_filsys fs, ext2_ino_t dir,
				      ثابت نویسه *name, ext2_ino_t ino,
				      صحیح flags)
{
	ساختار ext2_inode inode;
	نویسه *buf0, *buf1, *بالاترین;
	ساختار dx_ریشه *ریشه;
	ساختار ext2_dx_entry *entries;
	ساختار fake_dirent *fde;
	ساختار ext2_dir_entry *de, *de2;
	blk64_t pblk0, pblk1;
	unsigned صحیح len;
	ثابت unsigned صحیح blocksize = fs->blocksize;
	صحیح csum_size = 0;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	اگر (retval)
		بازگشت retval;

	اگر (inode.i_size > fs->blocksize || (inode.i_flags & EXT2_INDEX_FL))
		بازگشت 0;

	اگر (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = اندازه(ساختار ext2_dir_entry_tail);

	retval = ext2fs_get_mem(blocksize, &buf0);
	اگر (retval)
		بازگشت retval;

	retval = ext2fs_get_mem(blocksize, &buf1);
	اگر (retval)
		برو out_buf0;

	retval = ext2fs_bmap2(fs, dir, &inode, پوچ, 0, 0, پوچ, &pblk0);
	اگر (retval)
		برو out_buf1;

	retval = ext2fs_read_dir_block4(fs, pblk0, buf0, 0, dir);
	اگر (retval)
		برو out_buf1;

	ریشه = (ساختار dx_ریشه *)buf0;
	اگر (!check_dx_ریشه(fs, dir, ریشه)) {
		retval = EXT2_ET_DIR_CORRUPTED;
		برو out_buf1;
	}

	/* The 0th block becomes the root, move the dirents out */
	fde = &ریشه->dotdot;
	de = ext2fs_next_entry(fs, (ساختار ext2_dir_entry *)fde);
	len = ((نویسه *) ریشه) + (blocksize - csum_size) - (نویسه *) de;

	/* Allocate new block for the 0th block's dirents */
	retval = ext2fs_bmap2(fs, dir, &inode, پوچ, BMAP_ALLOC | BMAP_ZERO, 1,
			      پوچ, &pblk1);
	اگر (retval)
		برو out_buf1;

	memcpy(buf1, de, len);
	memset(de, 0, len); /* wipe old data */

	de = (ساختار ext2_dir_entry *)buf1;
	بالاترین = buf1 + len;
	وقتی ((نویسه *)(de2 = ext2fs_next_entry(fs, de)) < بالاترین) {
#اگر 0
		اگر (ext4_check_dir_entry(dir, پوچ, de, bh2, buf1, len,
					(نویسه *)de - buf1)) {
			retval = EXT2_ET_DIR_CORRUPTED;
			برو out_buf1;
		}
#endif
		de = de2;
	}
	retval = ext2fs_set_rec_len(fs,
			buf1 + (blocksize - csum_size) - (نویسه *) de, de);
	اگر (retval)
		برو out_buf1;

	اگر (csum_size)
		ext2fs_initialize_dirent_tail(fs,
				EXT2_DIRENT_TAIL(buf1, fs->blocksize));

	/* Initialize the root; the dot dirents already exist */
	de = (ساختار ext2_dir_entry *) (&ریشه->dotdot);
	retval = ext2fs_set_rec_len(fs, blocksize - ext2fs_dir_rec_len(2, 0),
				    de);
	memset (&ریشه->info, 0, اندازه(ریشه->info));
	ریشه->info.info_length = اندازه(ریشه->info);
	اگر (ext4_hash_in_dirent(&inode))
		ریشه->info.hash_version = EXT2_HASH_SIPHASH;
	وگرنه
		ریشه->info.hash_version = fs->super->s_def_hash_version;

	entries = ریشه->entries;
	dx_set_block(entries, 1);
	dx_set_شمار(entries, 1);
	dx_set_limit(entries, dx_ریشه_limit(fs, اندازه(ریشه->info)));

	retval = ext2fs_write_dir_block4(fs, pblk1, buf1, 0, dir);
	اگر (retval)
		برو out_buf1;

	retval = ext2fs_write_dir_block4(fs, pblk0, buf0, 0, dir);
	اگر (retval)
		برو out_buf1;

	inode.i_flags |= EXT2_INDEX_FL;
	inode.i_size += fs->blocksize;
	retval = ext2fs_write_inode(fs, dir, &inode);
	اگر (retval)
		برو out_buf1;

	retval = EAGAIN;
out_buf1:
	ext2fs_free_mem(&buf1);
out_buf0:
	ext2fs_free_mem(&buf0);
	بازگشت retval;
}

/*
 * Note: the low 3 bits of the flags field are used as the directory
 * entry filetype.
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, ثابت نویسه *name,
		      ext2_ino_t ino, صحیح flags)
{
	errcode_t		retval;
	ساختار link_struct	ls;
	ساختار ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (!(fs->flags & EXT2_FLAG_RW))
		بازگشت EXT2_ET_RO_FILSYS;

retry:
	اگر ((retval = ext2fs_read_inode(fs, dir, &inode)) != 0)
		بازگشت retval;

	اگر (inode.i_flags & EXT2_INDEX_FL)
		بازگشت dx_link(fs, dir, &inode, name, ino, flags);

	ls.fs = fs;
	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	اگر ((flags & EXT2FS_LINK_APPEND) &&
	    !(inode.i_flags & EXT4_INLINE_DATA_FL)) {
		blk64_t		lblk, pblk = 0;
		ساختار dir_context ctx;

		lblk = (inode.i_size / fs->blocksize) - 1;
		retval = ext2fs_bmap2(fs, dir, &inode, پوچ, 0, lblk,
				      پوچ, &pblk);
		اگر (retval)
			بازگشت retval;

		ctx.dir = dir;
		ctx.flags = DIRENT_FLAG_INCLUDE_EMPTY;
		ctx.func = link_proc;
		ctx.priv_data = &ls;
		ctx.errcode = 0;
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		اگر (retval)
			بازگشت retval;

		ext2fs_process_dir_block(fs, &pblk, lblk, 0, 0, &ctx);
		retval = ctx.errcode;
		ext2fs_free_mem(&ctx.buf);
	} وگرنه {
		retval = ext2fs_dir_iterate2(fs, dir,
					     DIRENT_FLAG_INCLUDE_EMPTY,
					     پوچ, link_proc, &ls);
	}
	اگر (retval)
		بازگشت retval;
	اگر (ls.err)
		بازگشت ls.err;

	اگر (!ls.done) {
		اگر (!(flags & EXT2FS_LINK_EXPAND))
			بازگشت EXT2_ET_DIR_NO_SPACE;

		اگر (ext2fs_has_feature_dir_index(fs->super)) {
			retval = try_make_indexed_dir(fs, dir, name, ino,
						      flags);
			اگر (retval == EAGAIN)
				برو retry;
			اگر (retval)
				بازگشت retval;
		}

		retval = ext2fs_expand_dir(fs, dir);
		اگر (retval)
			بازگشت retval;
		برو retry;
	}
	بازگشت 0;
}

/* Does this directory have an htree index? */
صحیح ext2fs_dir_is_dx(ext2_filsys fs, ثابت ساختار ext2_inode *inode)
{
	بازگشت ext2fs_has_feature_dir_index(fs->super) &&
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
تهی ext2fs_inc_nlink(ext2_filsys fs, ساختار ext2_inode *inode)
{
	inode->i_links_شمار++;

	اگر (ext2fs_dir_is_dx(fs, inode) &&
	    (inode->i_links_شمار > EXT2_LINK_MAX || inode->i_links_شمار == 2))
		inode->i_links_شمار = 1;
}

/*
 * If a directory had nlink == 1, then we should let it be 1. This indicates
 * directory has >EXT2_LINK_MAX subdirs.
 */
تهی ext2fs_dec_nlink(ساختار ext2_inode *inode)
{
	اگر (!S_ISDIR(inode->i_mode) || inode->i_links_شمار > 2)
		inode->i_links_شمار--;
}

صحیح ext2fs_dir_link_max(ext2_filsys fs, ساختار ext2_inode_large *inode)
{
	بازگشت inode->i_links_شمار >= EXT2_LINK_MAX &&
	       !(ext2fs_dir_is_dx(fs, EXT2_INODE(inode)) &&
		 ext2fs_has_feature_dir_nlink(fs->super));
}

صحیح ext2fs_dir_link_empty(ساختار ext2_inode *inode)
{
	بازگشت inode->i_links_شمار == 2 || inode->i_links_شمار == 1;
}
