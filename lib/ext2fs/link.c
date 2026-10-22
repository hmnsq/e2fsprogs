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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

#define EXT2_DX_سيو_OFF 24

بنية dx_frame {
	فراغ *buf;
	blk64_t pblock;
	بنية ext2_dx_countlimit *head;
	بنية ext2_dx_entry *entries;
	بنية ext2_dx_entry *at;
};

بنية dx_lookup_info {
	ثابت حرف *name;
	صحيح namelen;
	صحيح hash_alg;
	__u32 hash;
	unsigned levels;
	بنية dx_frame frames[EXT4_HTREE_LEVEL];
};

ساكن errcode_t alloc_dx_frame(ext2_filsys fs, بنية dx_frame *frame)
{
	أرجع ext2fs_get_mem(fs->blocksize, &frame->buf);
}

ساكن فراغ dx_release(بنية dx_lookup_info *info)
{
	unsigned level;

	لكل (level = 0; level < info->levels; level++) {
		إذا (info->frames[level].buf == عدم)
			اكسر;
		ext2fs_free_mem(&(info->frames[level].buf));
	}
	info->levels = 0;
}

ساكن فراغ dx_نيما_entry(بنية dx_frame *frame, صحيح كدي, __u32 hash)
{
	بنية ext2_dx_entry *p, *q, *m;

	p = frame->entries + 1;
	q = frame->entries + كدي - 1;
	طالما (p <= q) {
		m = p + (q - p) / 2;
		إذا (ext2fs_le32_to_cpu(m->hash) > hash)
			q = m - 1;
		وإلا
			p = m + 1;
	}
	frame->at = p - 1;
}

ساكن errcode_t load_logical_dir_block(ext2_filsys fs, ext2_ino_t dir,
					بنية ext2_inode *diri, blk64_t block,
					blk64_t *pblk, فراغ *buf)
{
	errcode_t errcode;
	صحيح ret_flags;

	errcode = ext2fs_bmap2(fs, dir, diri, عدم, 0, block, &ret_flags,
			       pblk);
	إذا (errcode)
		أرجع errcode;
	إذا (ret_flags & BMAP_RET_UNINIT)
		أرجع EXT2_ET_DIR_CORRUPTED;
	أرجع ext2fs_read_dir_block4(fs, *pblk, buf, 0, dir);
}

ساكن errcode_t dx_lookup(ext2_filsys fs, ext2_ino_t dir,
			   بنية ext2_inode *diri, بنية dx_lookup_info *info)
{
	بنية ext2_dx_سيو_info *سيو;
	errcode_t errcode;
	صحيح level = 0;
	صحيح كدي, limit;
	صحيح hash_alg;
	صحيح hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	__u32 minor_hash;
	بنية dx_frame *frame;

	errcode = alloc_dx_frame(fs, &(info->frames[0]));
	إذا (errcode)
		أرجع errcode;
	info->levels = 1;

	errcode = load_logical_dir_block(fs, dir, diri, 0,
					 &(info->frames[0].pblock),
					 info->frames[0].buf);
	إذا (errcode)
		اذهب out_err;
	سيو = (بنية ext2_dx_سيو_info *) ((حرف *)info->frames[0].buf +
					     EXT2_DX_سيو_OFF);
	hash_alg = سيو->hash_version;
	إذا (hash_alg != EXT2_HASH_TEA && hash_alg != EXT2_HASH_HALF_MD4 &&
	    hash_alg != EXT2_HASH_LEGACY) {
		errcode = EXT2_ET_DIRHASH_UNSUPP;
		اذهب out_err;
	}
	إذا (hash_alg <= EXT2_HASH_TEA &&
	    fs->super->s_flags & EXT2_FLAGS_UNSIGNED_HASH)
		hash_alg += 3;
	إذا (سيو->indirect_levels >= ext2_dir_htree_level(fs)) {
		errcode = EXT2_ET_DIR_CORRUPTED;
		اذهب out_err;
	}
	info->hash_alg = hash_alg;

	errcode = ext2fs_dirhash2(hash_alg, info->name, info->namelen,
				  fs->encoding, hash_flags,
				  fs->super->s_hash_seed, &info->hash,
				  &minor_hash);
	إذا (errcode)
		اذهب out_err;

	لكل (level = 0; level <= سيو->indirect_levels; level++) {
		frame = &(info->frames[level]);
		إذا (level > 0) {
			errcode = alloc_dx_frame(fs, frame);
			إذا (errcode)
				اذهب out_err;
			info->levels++;

			errcode = load_logical_dir_block(fs, dir, diri,
				ext2fs_le32_to_cpu(info->frames[level-1].at->block) & 0x0fffffff,
				&(frame->pblock), frame->buf);
			إذا (errcode)
				اذهب out_err;
		}
		errcode = ext2fs_get_dx_countlimit(fs, frame->buf,
						   &(frame->head), عدم);
		إذا (errcode)
			اذهب out_err;
		كدي = ext2fs_le16_to_cpu(frame->head->كدي);
		limit = ext2fs_le16_to_cpu(frame->head->limit);
		frame->entries = (بنية ext2_dx_entry *)(frame->head);
		إذا (!كدي || كدي > limit) {
			errcode = EXT2_ET_DIR_CORRUPTED;
			اذهب out_err;
		}

		dx_نيما_entry(frame, كدي, info->hash);
	}
	أرجع 0;
out_err:
	dx_release(info);
	أرجع errcode;
}

بنية link_struct  {
	ext2_filsys	fs;
	ثابت حرف	*name;
	صحيح		namelen;
	ext2_ino_t	inode;
	صحيح		flags;
	صحيح		done;
	unsigned صحيح	blocksize;
	errcode_t	err;
	بنية ext2_super_block *sb;
};

ساكن صحيح link_proc(ext2_ino_t dir EXT2FS_ATTR((unused)),
		     صحيح entru EXT2FS_ATTR((unused)),
		     بنية ext2_dir_entry *dirent,
		     صحيح	offset,
		     صحيح	blocksize,
		     حرف	*buf,
		     فراغ	*priv_data)
{
	بنية link_struct *ls = (بنية link_struct *) priv_data;
	بنية ext2_dir_entry *next;
	unsigned صحيح rec_len, min_rec_len, curr_rec_len;
	صحيح ret = 0;
	صحيح csum_size = 0;

	إذا (ls->done)
		أرجع DIRENT_ABORT;

	rec_len = EXT2_DIR_REC_LEN(ls->namelen);

	ls->err = ext2fs_get_rec_len(ls->fs, dirent, &curr_rec_len);
	إذا (ls->err)
		أرجع DIRENT_ABORT;

	إذا (ext2fs_has_feature_metadata_csum(ls->fs->super))
		csum_size = حجم(بنية ext2_dir_entry_tail);
	/*
	 * See if the following directory entry (if any) is unused;
	 * if so, absorb it into this one.
	 */
	next = (بنية ext2_dir_entry *) (buf + offset + curr_rec_len);
	إذا ((offset + (صحيح) curr_rec_len < blocksize - (8 + csum_size)) &&
	    (next->inode == 0) &&
	    (offset + (صحيح) curr_rec_len + (صحيح) next->rec_len <= blocksize)) {
		curr_rec_len += next->rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, curr_rec_len, dirent);
		إذا (ls->err)
			أرجع DIRENT_ABORT;
		ret = DIRENT_CHANGED;
	}

	/*
	 * If the directory entry is used, see if we can split the
	 * directory entry to make room for the new name.  If so,
	 * truncate it and return.
	 */
	إذا (dirent->inode) {
		min_rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(dirent));
		إذا (curr_rec_len < (min_rec_len + rec_len))
			أرجع ret;
		rec_len = curr_rec_len - min_rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, min_rec_len, dirent);
		إذا (ls->err)
			أرجع DIRENT_ABORT;
		next = (بنية ext2_dir_entry *) (buf + offset +
						  dirent->rec_len);
		next->inode = 0;
		ext2fs_dirent_set_name_len(next, 0);
		ext2fs_dirent_set_file_type(next, 0);
		ls->err = ext2fs_set_rec_len(ls->fs, rec_len, next);
		إذا (ls->err)
			أرجع DIRENT_ABORT;
		أرجع DIRENT_CHANGED;
	}

	/*
	 * If we get this far, then the directory entry is not used.
	 * See if we can fit the request entry in.  If so, do it.
	 */
	إذا (curr_rec_len < rec_len)
		أرجع ret;
	dirent->inode = ls->inode;
	ext2fs_dirent_set_name_len(dirent, ls->namelen);
	strncpy(dirent->name, ls->name, ls->namelen);
	إذا (ext2fs_has_feature_filetype(ls->sb))
		ext2fs_dirent_set_file_type(dirent,
					    ls->flags & EXT2FS_LINK_FT_MASK);

	ls->done++;
	أرجع DIRENT_ABORT|DIRENT_CHANGED;
}

ساكن errcode_t add_dirent_to_buf(ext2_filsys fs, e2_blkcnt_t blockcnt,
				   حرف *buf, ext2_ino_t dir,
				   بنية ext2_inode *diri, ثابت حرف *name,
				   ext2_ino_t ino, صحيح flags, blk64_t *pblkp)
{
	بنية dir_context ctx;
	بنية link_struct ls;
	errcode_t retval;

	retval = load_logical_dir_block(fs, dir, diri, blockcnt, pblkp, buf);
	إذا (retval)
		أرجع retval;
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
	إذا (ctx.errcode)
		أرجع ctx.errcode;
	إذا (ls.err)
		أرجع ls.err;
	إذا (!ls.done)
		أرجع EXT2_ET_DIR_NO_SPACE;
	أرجع 0;
}

بنية dx_hash_map {
	__u32 hash;
	صحيح size;
	صحيح off;
};

ساكن EXT2_QSORT_TYPE dx_hash_map_cmp(ثابت فراغ *ap, ثابت فراغ *bp)
{
	ثابت بنية dx_hash_map *a = ap, *b = bp;

	إذا (a->hash < b->hash)
		أرجع -1;
	إذا (a->hash > b->hash)
		أرجع 1;
	أرجع 0;
}

ساكن errcode_t dx_move_dirents(ext2_filsys fs, بنية dx_hash_map *map,
				 صحيح كدي, فراغ *from, فراغ *to)
{
	بنية ext2_dir_entry *de;
	صحيح i;
	صحيح rec_len = 0;
	errcode_t retval;
	صحيح csum_size = 0;
	فراغ *base = to;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = حجم(بنية ext2_dir_entry_tail);

	لكل (i = 0; i < كدي; i++) {
		de = (بنية ext2_dir_entry *) ((حرف *)from + map[i].off);
		rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(de));
		memcpy(to, de, rec_len);
		retval = ext2fs_set_rec_len(fs, rec_len, to);
		إذا (retval)
			أرجع retval;
		to = (حرف *)to + rec_len;
	}
	/*
	 * Update rec_len of the last dir entry to stretch to the end of block
	 */
	to = (حرف *)to - rec_len;
	rec_len = fs->blocksize - ((حرف *)to - (حرف *)base) - csum_size;
	retval = ext2fs_set_rec_len(fs, rec_len, to);
	إذا (retval)
		أرجع retval;
	إذا (csum_size)
		ext2fs_initialize_dirent_tail(fs,
				EXT2_DIRENT_TAIL(base, fs->blocksize));
	أرجع 0;
}

ساكن errcode_t dx_سك_entry(ext2_filsys fs, ext2_ino_t dir,
				 بنية dx_lookup_info *info, صحيح level,
				 __u32 hash, blk64_t lblk)
{
	صحيح pcount;
	بنية ext2_dx_entry *كولي, *new;

	pcount = ext2fs_le16_to_cpu(info->frames[level].head->كدي);
	كولي = info->frames[level].entries + pcount;
	new = info->frames[level].at + 1;
	memmove(new + 1, new, (حرف *)كولي - (حرف *)new);
	new->hash = ext2fs_cpu_to_le32(hash);
	new->block = ext2fs_cpu_to_le32(lblk);
	info->frames[level].head->كدي = ext2fs_cpu_to_le16(pcount + 1);
	أرجع ext2fs_write_dir_block4(fs, info->frames[level].pblock,
				       info->frames[level].buf, 0, dir);
}

ساكن errcode_t dx_split_leaf(ext2_filsys fs, ext2_ino_t dir,
			       بنية ext2_inode *diri,
			       بنية dx_lookup_info *info, فراغ *buf,
			       blk64_t leaf_pblk, blk64_t new_lblk,
			       blk64_t new_pblk)
{
	صحيح hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	بنية ext2_dir_entry *de;
	فراغ *buf2;
	errcode_t retval = 0;
	unsigned صحيح rec_len;
	unsigned صحيح offset, move_size;
	صحيح i, كدي = 0;
	بنية dx_hash_map *map;
	صحيح continued;
	__u32 minor_hash;

	retval = ext2fs_get_mem(fs->blocksize, &buf2);
	إذا (retval)
		أرجع retval;
	retval = ext2fs_get_array(fs->blocksize / 12,
				  حجم(بنية dx_hash_map), &map);
	إذا (retval) {
		ext2fs_free_mem(&buf2);
		أرجع retval;
	}
	لكل (offset = 0; offset < fs->blocksize; offset += rec_len) {
		de = (بنية ext2_dir_entry *) ((حرف *)buf + offset);
		retval = ext2fs_get_rec_len(fs, de, &rec_len);
		إذا (retval)
			اذهب out;
		إذا (ext2fs_dirent_name_len(de) > 0 && de->inode) {
			map[كدي].off = offset;
			map[كدي].size = rec_len;
			retval = ext2fs_dirhash2(info->hash_alg, de->name,
					ext2fs_dirent_name_len(de),
					fs->encoding, hash_flags,
					fs->super->s_hash_seed,
					&(map[كدي].hash),
					&minor_hash);
			إذا (retval)
				اذهب out;
			كدي++;
		}
	}
	qsort(map, كدي, حجم(بنية dx_hash_map), dx_hash_map_cmp);
	move_size = 0;
	/* Find place to split block */
	لكل (i = كدي - 1; i >= 0; i--) {
		إذا (move_size + map[i].size / 2 > fs->blocksize / 2)
			اكسر;
		move_size += map[i].size;
	}
	/* Let i be the first entry to move */
	i++;
	/* Move selected directory entries to new block */
	retval = dx_move_dirents(fs, map + i, كدي - i, buf, buf2);
	إذا (retval)
		اذهب out;
	retval = ext2fs_write_dir_block4(fs, new_pblk, buf2, 0, dir);
	إذا (retval)
		اذهب out;
	/* Repack remaining entries in the old block */
	retval = dx_move_dirents(fs, map, i, buf, buf2);
	إذا (retval)
		اذهب out;
	retval = ext2fs_write_dir_block4(fs, leaf_pblk, buf2, 0, dir);
	إذا (retval)
		اذهب out;
	/* Update parent node */
	continued = map[i].hash == map[i-1].hash;
	retval = dx_سك_entry(fs, dir, info, info->levels - 1,
				 map[i].hash + continued, new_lblk);
out:
	ext2fs_free_mem(&buf2);
	ext2fs_free_mem(&map);
	أرجع retval;
}

ساكن errcode_t dx_grow_بشيا(ext2_filsys fs, ext2_ino_t dir,
			      بنية ext2_inode *diri,
			      بنية dx_lookup_info *info, فراغ *buf,
			      blk64_t leaf_pblk)
{
	صحيح i;
	errcode_t retval;
	ext2_off64_t size = EXT2_I_SIZE(diri);
	blk64_t lblk, pblk;
	بنية ext2_dir_entry *de;
	بنية ext2_dx_countlimit *head;
	صحيح csum_size = 0;
	صحيح كدي;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = حجم(بنية ext2_dx_tail);

	/* Find level which can accommodate new child */
	لكل (i = info->levels - 1; i >= 0; i--)
		إذا (ext2fs_le16_to_cpu(info->frames[i].head->كدي) <
		    ext2fs_le16_to_cpu(info->frames[i].head->limit))
			اكسر;
	/* Need to grow tree depth? */
	إذا (i < 0 && info->levels >= ext2_dir_htree_level(fs))
		أرجع EXT2_ET_DIR_NO_SPACE;
	lblk = size / fs->blocksize;
	size += fs->blocksize;
	retval = ext2fs_inode_size_set(fs, diri, size);
	إذا (retval)
		أرجع retval;
	retval = ext2fs_fallocate(fs,
			EXT2_FALLOCATE_FORCE_INIT | EXT2_FALLOCATE_ZERO_BLOCKS,
			dir, diri, 0, lblk, 1);
	إذا (retval)
		أرجع retval;
	retval = ext2fs_write_inode(fs, dir, diri);
	إذا (retval)
		أرجع retval;
	retval = ext2fs_bmap2(fs, dir, diri, عدم, 0, lblk, عدم, &pblk);
	إذا (retval)
		أرجع retval;
	/* Only leaf addition needed? */
	إذا (i == (صحيح)info->levels - 1)
		أرجع dx_split_leaf(fs, dir, diri, info, buf, leaf_pblk,
				     lblk, pblk);

	de = buf;
	de->inode = 0;
	ext2fs_dirent_set_name_len(de, 0);
	ext2fs_dirent_set_file_type(de, 0);
	retval = ext2fs_set_rec_len(fs, fs->blocksize, de);
	إذا (retval)
		أرجع retval;
	head = (بنية ext2_dx_countlimit *) ((حرف *)buf + 8);
	كدي = ext2fs_le16_to_cpu(info->frames[i+1].head->كدي);
	/* Growing tree depth? */
	إذا (i < 0) {
		بنية ext2_dx_سيو_info *سيو;

		memcpy(head, info->frames[0].entries,
		       كدي * حجم(بنية ext2_dx_entry));
		head->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_size)) /
				حجم(بنية ext2_dx_entry));
		/* head->count gets set by memcpy above to correct value */

		/* Now update tree root */
		info->frames[0].head->كدي = ext2fs_cpu_to_le16(1);
		info->frames[0].entries[0].block = ext2fs_cpu_to_le32(lblk);
		سيو = (بنية ext2_dx_سيو_info *)
			((حرف *)info->frames[0].buf + EXT2_DX_سيو_OFF);
		سيو->indirect_levels++;
	} وإلا {
		/* Splitting internal node in two */
		صحيح count1 = كدي / 2;
		صحيح count2 = كدي - count1;
		__u32 split_hash = ext2fs_le32_to_cpu(info->frames[i+1].entries[count1].hash);

		memcpy(head, info->frames[i+1].entries + count1,
		       count2 * حجم(بنية ext2_dx_entry));
		head->كدي = ext2fs_cpu_to_le16(count2);
		head->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_size)) /
				حجم(بنية ext2_dx_entry));
		info->frames[i+1].head->كدي = ext2fs_cpu_to_le16(count1);

		/* Update parent node */
		retval = dx_سك_entry(fs, dir, info, i, split_hash, lblk);
		إذا (retval)
			أرجع retval;

	}
	/* Writeout split block / updated root */
	retval = ext2fs_write_dir_block4(fs, info->frames[i+1].pblock,
					 info->frames[i+1].buf, 0, dir);
	إذا (retval)
		أرجع retval;
	/* Writeout new tree block */
	retval = ext2fs_write_dir_block4(fs, pblk, buf, 0, dir);
	إذا (retval)
		أرجع retval;
	أرجع 0;
}

ساكن errcode_t dx_link(ext2_filsys fs, ext2_ino_t dir,
			 بنية ext2_inode *diri, ثابت حرف *name,
			 ext2_ino_t ino, صحيح flags)
{
	بنية dx_lookup_info dx_info;
	errcode_t retval;
	فراغ *blockbuf;
	unsigned restart = 0;
	blk64_t leaf_pblk;

	retval = ext2fs_get_mem(fs->blocksize, &blockbuf);
	إذا (retval)
		أرجع retval;

	dx_info.name = name;
	dx_info.namelen = strlen(name);
again:
	retval = dx_lookup(fs, dir, diri, &dx_info);
	إذا (retval)
		اذهب free_buf;

	retval = add_dirent_to_buf(fs,
		ext2fs_le32_to_cpu(dx_info.frames[dx_info.levels-1].at->block) & 0x0fffffff,
		blockbuf, dir, diri, name, ino, flags, &leaf_pblk);
	/*
	 * Success or error other than ENOSPC...? We are done. We may need upto
	 * two tries to add entry. One to split htree node and another to add
	 * new leaf block.
	 */
	إذا (restart >= dx_info.levels || retval != EXT2_ET_DIR_NO_SPACE)
		اذهب free_frames;
	retval = dx_grow_بشيا(fs, dir, diri, &dx_info, blockbuf, leaf_pblk);
	إذا (retval)
		اذهب free_frames;
	/* Restart everything now that the tree is larger */
	restart++;
	dx_release(&dx_info);
	اذهب again;
free_frames:
	dx_release(&dx_info);
free_buf:
	ext2fs_free_mem(&blockbuf);
	أرجع retval;
}

بنية fake_dirent
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

بنية dx_سيو
{
	بنية fake_dirent dot;
	حرف dot_name[4];
	بنية fake_dirent dotdot;
	حرف dotdot_name[4];
	بنية ext2_dx_سيو_info info;
	بنية ext2_dx_entry entries[];
};

ساكن صحيح check_dx_سيو(ext2_filsys fs, ext2_ino_t ino, بنية dx_سيو *سيو)
{
	بنية fake_dirent *fde;
	حرف *error_سكو;
	unsigned صحيح rlen;
	حرف *blockend = (حرف *)سيو + fs->blocksize;

	fde = &سيو->dot;
	إذا (fde->name_len != 1) {
		error_سكو = "invalid name_len for '.'";
		اذهب corrupted;
	}
	إذا (strncmp(سيو->dot_name, ".", fde->name_len)) {
		error_سكو = "invalid name for '.'";
		اذهب corrupted;
	}

	ext2fs_get_rec_len(fs, (بنية ext2_dir_entry *)fde, &rlen);
	إذا ((حرف *)fde + rlen >= blockend) {
		error_سكو = "invalid rec_len for '.'";
		اذهب corrupted;
	}

	fde = &سيو->dotdot;
	إذا (fde->name_len != 2) {
		error_سكو = "invalid name_len for '..'";
		اذهب corrupted;
	}
	إذا (strncmp(سيو->dotdot_name, "..", fde->name_len)) {
		error_سكو = "invalid name for '..'";
		اذهب corrupted;
	}
	ext2fs_get_rec_len(fs, (بنية ext2_dir_entry *)fde, &rlen);
	إذا ((حرف *)fde + rlen >= blockend) {
		error_سكو = "invalid rec_len for '..'";
		اذهب corrupted;
	}

	أرجع 1;

corrupted:
	fprintf(stderr, "Corrupt dir %u: %s, running e2fsck is recommended\n",
			 ino, error_سكو);
	أرجع 0;
}

ساكن مضمن بنية ext2_dir_entry *
ext2fs_next_entry(ext2_filsys fs, بنية ext2_dir_entry *p)
{
	unsigned صحيح rlen;

	ext2fs_get_rec_len(fs, p, &rlen);
	أرجع (بنية ext2_dir_entry *)((حرف *)p + rlen);
}

ساكن مضمن فراغ dx_set_block(بنية ext2_dx_entry *entry, unsigned درج)
{
	entry->block = ext2fs_cpu_to_le32(درج);
}

ساكن مضمن فراغ dx_set_كدي(بنية ext2_dx_entry *entries, unsigned درج)
{
	((بنية ext2_dx_countlimit *) entries)->كدي = ext2fs_cpu_to_le16(درج);
}

ساكن مضمن فراغ dx_set_limit(بنية ext2_dx_entry *entries, unsigned درج)
{
	((بنية ext2_dx_countlimit *) entries)->limit = ext2fs_cpu_to_le16(درج);
}

ساكن مضمن unsigned dx_سيو_limit(ext2_filsys fs, unsigned infosize)
{
	unsigned صحيح entry_space = fs->blocksize -
			ext2fs_dir_rec_len(1, 0) -
			ext2fs_dir_rec_len(2, 0) - infosize;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		entry_space -= حجم(بنية ext2_dx_tail);
	أرجع entry_space / حجم(بنية ext2_dx_entry);
}

/*
 * This converts a one block unindexed directory to a 3 block indexed
 * directory, and adds the dentry to the indexed directory.  Returns 0 if the
 * index was not created; EAGAIN if it was; or an errcode_t on error.
 */
ساكن errcode_t try_make_indexed_dir(ext2_filsys fs, ext2_ino_t dir,
				      ثابت حرف *name, ext2_ino_t ino,
				      صحيح flags)
{
	بنية ext2_inode inode;
	حرف *buf0, *buf1, *كولي;
	بنية dx_سيو *سيو;
	بنية ext2_dx_entry *entries;
	بنية fake_dirent *fde;
	بنية ext2_dir_entry *de, *de2;
	blk64_t pblk0, pblk1;
	unsigned صحيح len;
	ثابت unsigned صحيح blocksize = fs->blocksize;
	صحيح csum_size = 0;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	إذا (retval)
		أرجع retval;

	إذا (inode.i_size > fs->blocksize || (inode.i_flags & EXT2_INDEX_FL))
		أرجع 0;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = حجم(بنية ext2_dir_entry_tail);

	retval = ext2fs_get_mem(blocksize, &buf0);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_get_mem(blocksize, &buf1);
	إذا (retval)
		اذهب out_buf0;

	retval = ext2fs_bmap2(fs, dir, &inode, عدم, 0, 0, عدم, &pblk0);
	إذا (retval)
		اذهب out_buf1;

	retval = ext2fs_read_dir_block4(fs, pblk0, buf0, 0, dir);
	إذا (retval)
		اذهب out_buf1;

	سيو = (بنية dx_سيو *)buf0;
	إذا (!check_dx_سيو(fs, dir, سيو)) {
		retval = EXT2_ET_DIR_CORRUPTED;
		اذهب out_buf1;
	}

	/* The 0th block becomes the root, move the dirents out */
	fde = &سيو->dotdot;
	de = ext2fs_next_entry(fs, (بنية ext2_dir_entry *)fde);
	len = ((حرف *) سيو) + (blocksize - csum_size) - (حرف *) de;

	/* Allocate new block for the 0th block's dirents */
	retval = ext2fs_bmap2(fs, dir, &inode, عدم, BMAP_ALLOC | BMAP_ZERO, 1,
			      عدم, &pblk1);
	إذا (retval)
		اذهب out_buf1;

	memcpy(buf1, de, len);
	memset(de, 0, len); /* wipe old data */

	de = (بنية ext2_dir_entry *)buf1;
	كولي = buf1 + len;
	طالما ((حرف *)(de2 = ext2fs_next_entry(fs, de)) < كولي) {
#إذا 0
		إذا (ext4_check_dir_entry(dir, عدم, de, bh2, buf1, len,
					(حرف *)de - buf1)) {
			retval = EXT2_ET_DIR_CORRUPTED;
			اذهب out_buf1;
		}
#endif
		de = de2;
	}
	retval = ext2fs_set_rec_len(fs,
			buf1 + (blocksize - csum_size) - (حرف *) de, de);
	إذا (retval)
		اذهب out_buf1;

	إذا (csum_size)
		ext2fs_initialize_dirent_tail(fs,
				EXT2_DIRENT_TAIL(buf1, fs->blocksize));

	/* Initialize the root; the dot dirents already exist */
	de = (بنية ext2_dir_entry *) (&سيو->dotdot);
	retval = ext2fs_set_rec_len(fs, blocksize - ext2fs_dir_rec_len(2, 0),
				    de);
	memset (&سيو->info, 0, حجم(سيو->info));
	سيو->info.info_length = حجم(سيو->info);
	إذا (ext4_hash_in_dirent(&inode))
		سيو->info.hash_version = EXT2_HASH_SIPHASH;
	وإلا
		سيو->info.hash_version = fs->super->s_def_hash_version;

	entries = سيو->entries;
	dx_set_block(entries, 1);
	dx_set_كدي(entries, 1);
	dx_set_limit(entries, dx_سيو_limit(fs, حجم(سيو->info)));

	retval = ext2fs_write_dir_block4(fs, pblk1, buf1, 0, dir);
	إذا (retval)
		اذهب out_buf1;

	retval = ext2fs_write_dir_block4(fs, pblk0, buf0, 0, dir);
	إذا (retval)
		اذهب out_buf1;

	inode.i_flags |= EXT2_INDEX_FL;
	inode.i_size += fs->blocksize;
	retval = ext2fs_write_inode(fs, dir, &inode);
	إذا (retval)
		اذهب out_buf1;

	retval = EAGAIN;
out_buf1:
	ext2fs_free_mem(&buf1);
out_buf0:
	ext2fs_free_mem(&buf0);
	أرجع retval;
}

/*
 * Note: the low 3 bits of the flags field are used as the directory
 * entry filetype.
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, ثابت حرف *name,
		      ext2_ino_t ino, صحيح flags)
{
	errcode_t		retval;
	بنية link_struct	ls;
	بنية ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!(fs->flags & EXT2_FLAG_RW))
		أرجع EXT2_ET_RO_FILSYS;

retry:
	إذا ((retval = ext2fs_read_inode(fs, dir, &inode)) != 0)
		أرجع retval;

	إذا (inode.i_flags & EXT2_INDEX_FL)
		أرجع dx_link(fs, dir, &inode, name, ino, flags);

	ls.fs = fs;
	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	إذا ((flags & EXT2FS_LINK_APPEND) &&
	    !(inode.i_flags & EXT4_INLINE_DATA_FL)) {
		blk64_t		lblk, pblk = 0;
		بنية dir_context ctx;

		lblk = (inode.i_size / fs->blocksize) - 1;
		retval = ext2fs_bmap2(fs, dir, &inode, عدم, 0, lblk,
				      عدم, &pblk);
		إذا (retval)
			أرجع retval;

		ctx.dir = dir;
		ctx.flags = DIRENT_FLAG_INCLUDE_EMPTY;
		ctx.func = link_proc;
		ctx.priv_data = &ls;
		ctx.errcode = 0;
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		إذا (retval)
			أرجع retval;

		ext2fs_process_dir_block(fs, &pblk, lblk, 0, 0, &ctx);
		retval = ctx.errcode;
		ext2fs_free_mem(&ctx.buf);
	} وإلا {
		retval = ext2fs_dir_iterate2(fs, dir,
					     DIRENT_FLAG_INCLUDE_EMPTY,
					     عدم, link_proc, &ls);
	}
	إذا (retval)
		أرجع retval;
	إذا (ls.err)
		أرجع ls.err;

	إذا (!ls.done) {
		إذا (!(flags & EXT2FS_LINK_EXPAND))
			أرجع EXT2_ET_DIR_NO_SPACE;

		إذا (ext2fs_has_feature_dir_index(fs->super)) {
			retval = try_make_indexed_dir(fs, dir, name, ino,
						      flags);
			إذا (retval == EAGAIN)
				اذهب retry;
			إذا (retval)
				أرجع retval;
		}

		retval = ext2fs_expand_dir(fs, dir);
		إذا (retval)
			أرجع retval;
		اذهب retry;
	}
	أرجع 0;
}

/* Does this directory have an htree index? */
صحيح ext2fs_dir_is_dx(ext2_filsys fs, ثابت بنية ext2_inode *inode)
{
	أرجع ext2fs_has_feature_dir_index(fs->super) &&
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
فراغ ext2fs_inc_nlink(ext2_filsys fs, بنية ext2_inode *inode)
{
	inode->i_links_كدي++;

	إذا (ext2fs_dir_is_dx(fs, inode) &&
	    (inode->i_links_كدي > EXT2_LINK_MAX || inode->i_links_كدي == 2))
		inode->i_links_كدي = 1;
}

/*
 * If a directory had nlink == 1, then we should let it be 1. This indicates
 * directory has >EXT2_LINK_MAX subdirs.
 */
فراغ ext2fs_dec_nlink(بنية ext2_inode *inode)
{
	إذا (!S_ISDIR(inode->i_mode) || inode->i_links_كدي > 2)
		inode->i_links_كدي--;
}

صحيح ext2fs_dir_link_max(ext2_filsys fs, بنية ext2_inode_large *inode)
{
	أرجع inode->i_links_كدي >= EXT2_LINK_MAX &&
	       !(ext2fs_dir_is_dx(fs, EXT2_INODE(inode)) &&
		 ext2fs_has_feature_dir_nlink(fs->super));
}

صحيح ext2fs_dir_link_empty(بنية ext2_inode *inode)
{
	أرجع inode->i_links_كدي == 2 || inode->i_links_كدي == 1;
}
