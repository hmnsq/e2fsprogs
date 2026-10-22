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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

#define EXT2_DX_വേര്_OFF 24

ഘടന dx_frame {
	ശൂന്യം *buf;
	blk64_t pblock;
	ഘടന ext2_dx_countlimit *head;
	ഘടന ext2_dx_entry *entries;
	ഘടന ext2_dx_entry *at;
};

ഘടന dx_lookup_info {
	സ്ഥിരം അക്ഷരം *name;
	പൂർണ്ണസംഖ്യ namelen;
	പൂർണ്ണസംഖ്യ hash_alg;
	__u32 hash;
	unsigned levels;
	ഘടന dx_frame frames[EXT4_HTREE_LEVEL];
};

സ്ഥിതം errcode_t alloc_dx_frame(ext2_filsys fs, ഘടന dx_frame *frame)
{
	തിരികെ ext2fs_get_mem(fs->blocksize, &frame->buf);
}

സ്ഥിതം ശൂന്യം dx_release(ഘടന dx_lookup_info *info)
{
	unsigned level;

	വേണ്ടി (level = 0; level < info->levels; level++) {
		എങ്കിൽ (info->frames[level].buf == ശൂന്യ)
			നിർത്തുക;
		ext2fs_free_mem(&(info->frames[level].buf));
	}
	info->levels = 0;
}

സ്ഥിതം ശൂന്യം dx_തിരയൽ_entry(ഘടന dx_frame *frame, പൂർണ്ണസംഖ്യ എണ്ണം, __u32 hash)
{
	ഘടന ext2_dx_entry *p, *q, *m;

	p = frame->entries + 1;
	q = frame->entries + എണ്ണം - 1;
	ആകുമ്പോൾ (p <= q) {
		m = p + (q - p) / 2;
		എങ്കിൽ (ext2fs_le32_to_cpu(m->hash) > hash)
			q = m - 1;
		അല്ലെങ്കിൽ
			p = m + 1;
	}
	frame->at = p - 1;
}

സ്ഥിതം errcode_t load_logical_dir_block(ext2_filsys fs, ext2_ino_t dir,
					ഘടന ext2_inode *diri, blk64_t block,
					blk64_t *pblk, ശൂന്യം *buf)
{
	errcode_t errcode;
	പൂർണ്ണസംഖ്യ ret_flags;

	errcode = ext2fs_bmap2(fs, dir, diri, ശൂന്യ, 0, block, &ret_flags,
			       pblk);
	എങ്കിൽ (errcode)
		തിരികെ errcode;
	എങ്കിൽ (ret_flags & BMAP_RET_UNINIT)
		തിരികെ EXT2_ET_DIR_CORRUPTED;
	തിരികെ ext2fs_read_dir_block4(fs, *pblk, buf, 0, dir);
}

സ്ഥിതം errcode_t dx_lookup(ext2_filsys fs, ext2_ino_t dir,
			   ഘടന ext2_inode *diri, ഘടന dx_lookup_info *info)
{
	ഘടന ext2_dx_വേര്_info *വേര്;
	errcode_t errcode;
	പൂർണ്ണസംഖ്യ level = 0;
	പൂർണ്ണസംഖ്യ എണ്ണം, limit;
	പൂർണ്ണസംഖ്യ hash_alg;
	പൂർണ്ണസംഖ്യ hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	__u32 minor_hash;
	ഘടന dx_frame *frame;

	errcode = alloc_dx_frame(fs, &(info->frames[0]));
	എങ്കിൽ (errcode)
		തിരികെ errcode;
	info->levels = 1;

	errcode = load_logical_dir_block(fs, dir, diri, 0,
					 &(info->frames[0].pblock),
					 info->frames[0].buf);
	എങ്കിൽ (errcode)
		പോകുക out_err;
	വേര് = (ഘടന ext2_dx_വേര്_info *) ((അക്ഷരം *)info->frames[0].buf +
					     EXT2_DX_വേര്_OFF);
	hash_alg = വേര്->hash_version;
	എങ്കിൽ (hash_alg != EXT2_HASH_TEA && hash_alg != EXT2_HASH_HALF_MD4 &&
	    hash_alg != EXT2_HASH_LEGACY) {
		errcode = EXT2_ET_DIRHASH_UNSUPP;
		പോകുക out_err;
	}
	എങ്കിൽ (hash_alg <= EXT2_HASH_TEA &&
	    fs->super->s_flags & EXT2_FLAGS_UNSIGNED_HASH)
		hash_alg += 3;
	എങ്കിൽ (വേര്->indirect_levels >= ext2_dir_htree_level(fs)) {
		errcode = EXT2_ET_DIR_CORRUPTED;
		പോകുക out_err;
	}
	info->hash_alg = hash_alg;

	errcode = ext2fs_dirhash2(hash_alg, info->name, info->namelen,
				  fs->encoding, hash_flags,
				  fs->super->s_hash_seed, &info->hash,
				  &minor_hash);
	എങ്കിൽ (errcode)
		പോകുക out_err;

	വേണ്ടി (level = 0; level <= വേര്->indirect_levels; level++) {
		frame = &(info->frames[level]);
		എങ്കിൽ (level > 0) {
			errcode = alloc_dx_frame(fs, frame);
			എങ്കിൽ (errcode)
				പോകുക out_err;
			info->levels++;

			errcode = load_logical_dir_block(fs, dir, diri,
				ext2fs_le32_to_cpu(info->frames[level-1].at->block) & 0x0fffffff,
				&(frame->pblock), frame->buf);
			എങ്കിൽ (errcode)
				പോകുക out_err;
		}
		errcode = ext2fs_get_dx_countlimit(fs, frame->buf,
						   &(frame->head), ശൂന്യ);
		എങ്കിൽ (errcode)
			പോകുക out_err;
		എണ്ണം = ext2fs_le16_to_cpu(frame->head->എണ്ണം);
		limit = ext2fs_le16_to_cpu(frame->head->limit);
		frame->entries = (ഘടന ext2_dx_entry *)(frame->head);
		എങ്കിൽ (!എണ്ണം || എണ്ണം > limit) {
			errcode = EXT2_ET_DIR_CORRUPTED;
			പോകുക out_err;
		}

		dx_തിരയൽ_entry(frame, എണ്ണം, info->hash);
	}
	തിരികെ 0;
out_err:
	dx_release(info);
	തിരികെ errcode;
}

ഘടന link_struct  {
	ext2_filsys	fs;
	സ്ഥിരം അക്ഷരം	*name;
	പൂർണ്ണസംഖ്യ		namelen;
	ext2_ino_t	inode;
	പൂർണ്ണസംഖ്യ		flags;
	പൂർണ്ണസംഖ്യ		done;
	unsigned പൂർണ്ണസംഖ്യ	blocksize;
	errcode_t	err;
	ഘടന ext2_super_block *sb;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ link_proc(ext2_ino_t dir EXT2FS_ATTR((unused)),
		     പൂർണ്ണസംഖ്യ entru EXT2FS_ATTR((unused)),
		     ഘടന ext2_dir_entry *dirent,
		     പൂർണ്ണസംഖ്യ	offset,
		     പൂർണ്ണസംഖ്യ	blocksize,
		     അക്ഷരം	*buf,
		     ശൂന്യം	*priv_data)
{
	ഘടന link_struct *ls = (ഘടന link_struct *) priv_data;
	ഘടന ext2_dir_entry *next;
	unsigned പൂർണ്ണസംഖ്യ rec_len, min_rec_len, curr_rec_len;
	പൂർണ്ണസംഖ്യ ret = 0;
	പൂർണ്ണസംഖ്യ csum_size = 0;

	എങ്കിൽ (ls->done)
		തിരികെ DIRENT_ABORT;

	rec_len = EXT2_DIR_REC_LEN(ls->namelen);

	ls->err = ext2fs_get_rec_len(ls->fs, dirent, &curr_rec_len);
	എങ്കിൽ (ls->err)
		തിരികെ DIRENT_ABORT;

	എങ്കിൽ (ext2fs_has_feature_metadata_csum(ls->fs->super))
		csum_size = വലുപ്പം(ഘടന ext2_dir_entry_tail);
	/*
	 * See if the following directory entry (if any) is unused;
	 * if so, absorb it into this one.
	 */
	next = (ഘടന ext2_dir_entry *) (buf + offset + curr_rec_len);
	എങ്കിൽ ((offset + (പൂർണ്ണസംഖ്യ) curr_rec_len < blocksize - (8 + csum_size)) &&
	    (next->inode == 0) &&
	    (offset + (പൂർണ്ണസംഖ്യ) curr_rec_len + (പൂർണ്ണസംഖ്യ) next->rec_len <= blocksize)) {
		curr_rec_len += next->rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, curr_rec_len, dirent);
		എങ്കിൽ (ls->err)
			തിരികെ DIRENT_ABORT;
		ret = DIRENT_CHANGED;
	}

	/*
	 * If the directory entry is used, see if we can split the
	 * directory entry to make room for the new name.  If so,
	 * truncate it and return.
	 */
	എങ്കിൽ (dirent->inode) {
		min_rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(dirent));
		എങ്കിൽ (curr_rec_len < (min_rec_len + rec_len))
			തിരികെ ret;
		rec_len = curr_rec_len - min_rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, min_rec_len, dirent);
		എങ്കിൽ (ls->err)
			തിരികെ DIRENT_ABORT;
		next = (ഘടന ext2_dir_entry *) (buf + offset +
						  dirent->rec_len);
		next->inode = 0;
		ext2fs_dirent_set_name_len(next, 0);
		ext2fs_dirent_set_file_type(next, 0);
		ls->err = ext2fs_set_rec_len(ls->fs, rec_len, next);
		എങ്കിൽ (ls->err)
			തിരികെ DIRENT_ABORT;
		തിരികെ DIRENT_CHANGED;
	}

	/*
	 * If we get this far, then the directory entry is not used.
	 * See if we can fit the request entry in.  If so, do it.
	 */
	എങ്കിൽ (curr_rec_len < rec_len)
		തിരികെ ret;
	dirent->inode = ls->inode;
	ext2fs_dirent_set_name_len(dirent, ls->namelen);
	strncpy(dirent->name, ls->name, ls->namelen);
	എങ്കിൽ (ext2fs_has_feature_filetype(ls->sb))
		ext2fs_dirent_set_file_type(dirent,
					    ls->flags & EXT2FS_LINK_FT_MASK);

	ls->done++;
	തിരികെ DIRENT_ABORT|DIRENT_CHANGED;
}

സ്ഥിതം errcode_t add_dirent_to_buf(ext2_filsys fs, e2_blkcnt_t blockcnt,
				   അക്ഷരം *buf, ext2_ino_t dir,
				   ഘടന ext2_inode *diri, സ്ഥിരം അക്ഷരം *name,
				   ext2_ino_t ino, പൂർണ്ണസംഖ്യ flags, blk64_t *pblkp)
{
	ഘടന dir_context ctx;
	ഘടന link_struct ls;
	errcode_t retval;

	retval = load_logical_dir_block(fs, dir, diri, blockcnt, pblkp, buf);
	എങ്കിൽ (retval)
		തിരികെ retval;
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
	എങ്കിൽ (ctx.errcode)
		തിരികെ ctx.errcode;
	എങ്കിൽ (ls.err)
		തിരികെ ls.err;
	എങ്കിൽ (!ls.done)
		തിരികെ EXT2_ET_DIR_NO_SPACE;
	തിരികെ 0;
}

ഘടന dx_hash_map {
	__u32 hash;
	പൂർണ്ണസംഖ്യ size;
	പൂർണ്ണസംഖ്യ off;
};

സ്ഥിതം EXT2_QSORT_TYPE dx_hash_map_cmp(സ്ഥിരം ശൂന്യം *ap, സ്ഥിരം ശൂന്യം *bp)
{
	സ്ഥിരം ഘടന dx_hash_map *a = ap, *b = bp;

	എങ്കിൽ (a->hash < b->hash)
		തിരികെ -1;
	എങ്കിൽ (a->hash > b->hash)
		തിരികെ 1;
	തിരികെ 0;
}

സ്ഥിതം errcode_t dx_move_dirents(ext2_filsys fs, ഘടന dx_hash_map *map,
				 പൂർണ്ണസംഖ്യ എണ്ണം, ശൂന്യം *from, ശൂന്യം *to)
{
	ഘടന ext2_dir_entry *de;
	പൂർണ്ണസംഖ്യ i;
	പൂർണ്ണസംഖ്യ rec_len = 0;
	errcode_t retval;
	പൂർണ്ണസംഖ്യ csum_size = 0;
	ശൂന്യം *base = to;

	എങ്കിൽ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = വലുപ്പം(ഘടന ext2_dir_entry_tail);

	വേണ്ടി (i = 0; i < എണ്ണം; i++) {
		de = (ഘടന ext2_dir_entry *) ((അക്ഷരം *)from + map[i].off);
		rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(de));
		memcpy(to, de, rec_len);
		retval = ext2fs_set_rec_len(fs, rec_len, to);
		എങ്കിൽ (retval)
			തിരികെ retval;
		to = (അക്ഷരം *)to + rec_len;
	}
	/*
	 * Update rec_len of the last dir entry to stretch to the end of block
	 */
	to = (അക്ഷരം *)to - rec_len;
	rec_len = fs->blocksize - ((അക്ഷരം *)to - (അക്ഷരം *)base) - csum_size;
	retval = ext2fs_set_rec_len(fs, rec_len, to);
	എങ്കിൽ (retval)
		തിരികെ retval;
	എങ്കിൽ (csum_size)
		ext2fs_initialize_dirent_tail(fs,
				EXT2_DIRENT_TAIL(base, fs->blocksize));
	തിരികെ 0;
}

സ്ഥിതം errcode_t dx_തിരുകുക_entry(ext2_filsys fs, ext2_ino_t dir,
				 ഘടന dx_lookup_info *info, പൂർണ്ണസംഖ്യ level,
				 __u32 hash, blk64_t lblk)
{
	പൂർണ്ണസംഖ്യ pcount;
	ഘടന ext2_dx_entry *മുകൾ, *new;

	pcount = ext2fs_le16_to_cpu(info->frames[level].head->എണ്ണം);
	മുകൾ = info->frames[level].entries + pcount;
	new = info->frames[level].at + 1;
	memmove(new + 1, new, (അക്ഷരം *)മുകൾ - (അക്ഷരം *)new);
	new->hash = ext2fs_cpu_to_le32(hash);
	new->block = ext2fs_cpu_to_le32(lblk);
	info->frames[level].head->എണ്ണം = ext2fs_cpu_to_le16(pcount + 1);
	തിരികെ ext2fs_write_dir_block4(fs, info->frames[level].pblock,
				       info->frames[level].buf, 0, dir);
}

സ്ഥിതം errcode_t dx_split_leaf(ext2_filsys fs, ext2_ino_t dir,
			       ഘടന ext2_inode *diri,
			       ഘടന dx_lookup_info *info, ശൂന്യം *buf,
			       blk64_t leaf_pblk, blk64_t new_lblk,
			       blk64_t new_pblk)
{
	പൂർണ്ണസംഖ്യ hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	ഘടന ext2_dir_entry *de;
	ശൂന്യം *buf2;
	errcode_t retval = 0;
	unsigned പൂർണ്ണസംഖ്യ rec_len;
	unsigned പൂർണ്ണസംഖ്യ offset, move_size;
	പൂർണ്ണസംഖ്യ i, എണ്ണം = 0;
	ഘടന dx_hash_map *map;
	പൂർണ്ണസംഖ്യ continued;
	__u32 minor_hash;

	retval = ext2fs_get_mem(fs->blocksize, &buf2);
	എങ്കിൽ (retval)
		തിരികെ retval;
	retval = ext2fs_get_array(fs->blocksize / 12,
				  വലുപ്പം(ഘടന dx_hash_map), &map);
	എങ്കിൽ (retval) {
		ext2fs_free_mem(&buf2);
		തിരികെ retval;
	}
	വേണ്ടി (offset = 0; offset < fs->blocksize; offset += rec_len) {
		de = (ഘടന ext2_dir_entry *) ((അക്ഷരം *)buf + offset);
		retval = ext2fs_get_rec_len(fs, de, &rec_len);
		എങ്കിൽ (retval)
			പോകുക out;
		എങ്കിൽ (ext2fs_dirent_name_len(de) > 0 && de->inode) {
			map[എണ്ണം].off = offset;
			map[എണ്ണം].size = rec_len;
			retval = ext2fs_dirhash2(info->hash_alg, de->name,
					ext2fs_dirent_name_len(de),
					fs->encoding, hash_flags,
					fs->super->s_hash_seed,
					&(map[എണ്ണം].hash),
					&minor_hash);
			എങ്കിൽ (retval)
				പോകുക out;
			എണ്ണം++;
		}
	}
	qsort(map, എണ്ണം, വലുപ്പം(ഘടന dx_hash_map), dx_hash_map_cmp);
	move_size = 0;
	/* Find place to split block */
	വേണ്ടി (i = എണ്ണം - 1; i >= 0; i--) {
		എങ്കിൽ (move_size + map[i].size / 2 > fs->blocksize / 2)
			നിർത്തുക;
		move_size += map[i].size;
	}
	/* Let i be the first entry to move */
	i++;
	/* Move selected directory entries to new block */
	retval = dx_move_dirents(fs, map + i, എണ്ണം - i, buf, buf2);
	എങ്കിൽ (retval)
		പോകുക out;
	retval = ext2fs_write_dir_block4(fs, new_pblk, buf2, 0, dir);
	എങ്കിൽ (retval)
		പോകുക out;
	/* Repack remaining entries in the old block */
	retval = dx_move_dirents(fs, map, i, buf, buf2);
	എങ്കിൽ (retval)
		പോകുക out;
	retval = ext2fs_write_dir_block4(fs, leaf_pblk, buf2, 0, dir);
	എങ്കിൽ (retval)
		പോകുക out;
	/* Update parent node */
	continued = map[i].hash == map[i-1].hash;
	retval = dx_തിരുകുക_entry(fs, dir, info, info->levels - 1,
				 map[i].hash + continued, new_lblk);
out:
	ext2fs_free_mem(&buf2);
	ext2fs_free_mem(&map);
	തിരികെ retval;
}

സ്ഥിതം errcode_t dx_grow_വൃക്ഷം(ext2_filsys fs, ext2_ino_t dir,
			      ഘടന ext2_inode *diri,
			      ഘടന dx_lookup_info *info, ശൂന്യം *buf,
			      blk64_t leaf_pblk)
{
	പൂർണ്ണസംഖ്യ i;
	errcode_t retval;
	ext2_off64_t size = EXT2_I_SIZE(diri);
	blk64_t lblk, pblk;
	ഘടന ext2_dir_entry *de;
	ഘടന ext2_dx_countlimit *head;
	പൂർണ്ണസംഖ്യ csum_size = 0;
	പൂർണ്ണസംഖ്യ എണ്ണം;

	എങ്കിൽ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = വലുപ്പം(ഘടന ext2_dx_tail);

	/* Find level which can accommodate new child */
	വേണ്ടി (i = info->levels - 1; i >= 0; i--)
		എങ്കിൽ (ext2fs_le16_to_cpu(info->frames[i].head->എണ്ണം) <
		    ext2fs_le16_to_cpu(info->frames[i].head->limit))
			നിർത്തുക;
	/* Need to grow tree depth? */
	എങ്കിൽ (i < 0 && info->levels >= ext2_dir_htree_level(fs))
		തിരികെ EXT2_ET_DIR_NO_SPACE;
	lblk = size / fs->blocksize;
	size += fs->blocksize;
	retval = ext2fs_inode_size_set(fs, diri, size);
	എങ്കിൽ (retval)
		തിരികെ retval;
	retval = ext2fs_fallocate(fs,
			EXT2_FALLOCATE_FORCE_INIT | EXT2_FALLOCATE_ZERO_BLOCKS,
			dir, diri, 0, lblk, 1);
	എങ്കിൽ (retval)
		തിരികെ retval;
	retval = ext2fs_write_inode(fs, dir, diri);
	എങ്കിൽ (retval)
		തിരികെ retval;
	retval = ext2fs_bmap2(fs, dir, diri, ശൂന്യ, 0, lblk, ശൂന്യ, &pblk);
	എങ്കിൽ (retval)
		തിരികെ retval;
	/* Only leaf addition needed? */
	എങ്കിൽ (i == (പൂർണ്ണസംഖ്യ)info->levels - 1)
		തിരികെ dx_split_leaf(fs, dir, diri, info, buf, leaf_pblk,
				     lblk, pblk);

	de = buf;
	de->inode = 0;
	ext2fs_dirent_set_name_len(de, 0);
	ext2fs_dirent_set_file_type(de, 0);
	retval = ext2fs_set_rec_len(fs, fs->blocksize, de);
	എങ്കിൽ (retval)
		തിരികെ retval;
	head = (ഘടന ext2_dx_countlimit *) ((അക്ഷരം *)buf + 8);
	എണ്ണം = ext2fs_le16_to_cpu(info->frames[i+1].head->എണ്ണം);
	/* Growing tree depth? */
	എങ്കിൽ (i < 0) {
		ഘടന ext2_dx_വേര്_info *വേര്;

		memcpy(head, info->frames[0].entries,
		       എണ്ണം * വലുപ്പം(ഘടന ext2_dx_entry));
		head->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_size)) /
				വലുപ്പം(ഘടന ext2_dx_entry));
		/* head->count gets set by memcpy above to correct value */

		/* Now update tree root */
		info->frames[0].head->എണ്ണം = ext2fs_cpu_to_le16(1);
		info->frames[0].entries[0].block = ext2fs_cpu_to_le32(lblk);
		വേര് = (ഘടന ext2_dx_വേര്_info *)
			((അക്ഷരം *)info->frames[0].buf + EXT2_DX_വേര്_OFF);
		വേര്->indirect_levels++;
	} അല്ലെങ്കിൽ {
		/* Splitting internal node in two */
		പൂർണ്ണസംഖ്യ count1 = എണ്ണം / 2;
		പൂർണ്ണസംഖ്യ count2 = എണ്ണം - count1;
		__u32 split_hash = ext2fs_le32_to_cpu(info->frames[i+1].entries[count1].hash);

		memcpy(head, info->frames[i+1].entries + count1,
		       count2 * വലുപ്പം(ഘടന ext2_dx_entry));
		head->എണ്ണം = ext2fs_cpu_to_le16(count2);
		head->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_size)) /
				വലുപ്പം(ഘടന ext2_dx_entry));
		info->frames[i+1].head->എണ്ണം = ext2fs_cpu_to_le16(count1);

		/* Update parent node */
		retval = dx_തിരുകുക_entry(fs, dir, info, i, split_hash, lblk);
		എങ്കിൽ (retval)
			തിരികെ retval;

	}
	/* Writeout split block / updated root */
	retval = ext2fs_write_dir_block4(fs, info->frames[i+1].pblock,
					 info->frames[i+1].buf, 0, dir);
	എങ്കിൽ (retval)
		തിരികെ retval;
	/* Writeout new tree block */
	retval = ext2fs_write_dir_block4(fs, pblk, buf, 0, dir);
	എങ്കിൽ (retval)
		തിരികെ retval;
	തിരികെ 0;
}

സ്ഥിതം errcode_t dx_link(ext2_filsys fs, ext2_ino_t dir,
			 ഘടന ext2_inode *diri, സ്ഥിരം അക്ഷരം *name,
			 ext2_ino_t ino, പൂർണ്ണസംഖ്യ flags)
{
	ഘടന dx_lookup_info dx_info;
	errcode_t retval;
	ശൂന്യം *blockbuf;
	unsigned restart = 0;
	blk64_t leaf_pblk;

	retval = ext2fs_get_mem(fs->blocksize, &blockbuf);
	എങ്കിൽ (retval)
		തിരികെ retval;

	dx_info.name = name;
	dx_info.namelen = strlen(name);
again:
	retval = dx_lookup(fs, dir, diri, &dx_info);
	എങ്കിൽ (retval)
		പോകുക free_buf;

	retval = add_dirent_to_buf(fs,
		ext2fs_le32_to_cpu(dx_info.frames[dx_info.levels-1].at->block) & 0x0fffffff,
		blockbuf, dir, diri, name, ino, flags, &leaf_pblk);
	/*
	 * Success or error other than ENOSPC...? We are done. We may need upto
	 * two tries to add entry. One to split htree node and another to add
	 * new leaf block.
	 */
	എങ്കിൽ (restart >= dx_info.levels || retval != EXT2_ET_DIR_NO_SPACE)
		പോകുക free_frames;
	retval = dx_grow_വൃക്ഷം(fs, dir, diri, &dx_info, blockbuf, leaf_pblk);
	എങ്കിൽ (retval)
		പോകുക free_frames;
	/* Restart everything now that the tree is larger */
	restart++;
	dx_release(&dx_info);
	പോകുക again;
free_frames:
	dx_release(&dx_info);
free_buf:
	ext2fs_free_mem(&blockbuf);
	തിരികെ retval;
}

ഘടന fake_dirent
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

ഘടന dx_വേര്
{
	ഘടന fake_dirent dot;
	അക്ഷരം dot_name[4];
	ഘടന fake_dirent dotdot;
	അക്ഷരം dotdot_name[4];
	ഘടന ext2_dx_വേര്_info info;
	ഘടന ext2_dx_entry entries[];
};

സ്ഥിതം പൂർണ്ണസംഖ്യ check_dx_വേര്(ext2_filsys fs, ext2_ino_t ino, ഘടന dx_വേര് *വേര്)
{
	ഘടന fake_dirent *fde;
	അക്ഷരം *error_സന്ദേശം;
	unsigned പൂർണ്ണസംഖ്യ rlen;
	അക്ഷരം *blockend = (അക്ഷരം *)വേര് + fs->blocksize;

	fde = &വേര്->dot;
	എങ്കിൽ (fde->name_len != 1) {
		error_സന്ദേശം = "invalid name_len for '.'";
		പോകുക corrupted;
	}
	എങ്കിൽ (strncmp(വേര്->dot_name, ".", fde->name_len)) {
		error_സന്ദേശം = "invalid name for '.'";
		പോകുക corrupted;
	}

	ext2fs_get_rec_len(fs, (ഘടന ext2_dir_entry *)fde, &rlen);
	എങ്കിൽ ((അക്ഷരം *)fde + rlen >= blockend) {
		error_സന്ദേശം = "invalid rec_len for '.'";
		പോകുക corrupted;
	}

	fde = &വേര്->dotdot;
	എങ്കിൽ (fde->name_len != 2) {
		error_സന്ദേശം = "invalid name_len for '..'";
		പോകുക corrupted;
	}
	എങ്കിൽ (strncmp(വേര്->dotdot_name, "..", fde->name_len)) {
		error_സന്ദേശം = "invalid name for '..'";
		പോകുക corrupted;
	}
	ext2fs_get_rec_len(fs, (ഘടന ext2_dir_entry *)fde, &rlen);
	എങ്കിൽ ((അക്ഷരം *)fde + rlen >= blockend) {
		error_സന്ദേശം = "invalid rec_len for '..'";
		പോകുക corrupted;
	}

	തിരികെ 1;

corrupted:
	fprintf(stderr, "Corrupt dir %u: %s, running e2fsck is recommended\n",
			 ino, error_സന്ദേശം);
	തിരികെ 0;
}

സ്ഥിതം വരിയിൽ ഘടന ext2_dir_entry *
ext2fs_next_entry(ext2_filsys fs, ഘടന ext2_dir_entry *p)
{
	unsigned പൂർണ്ണസംഖ്യ rlen;

	ext2fs_get_rec_len(fs, p, &rlen);
	തിരികെ (ഘടന ext2_dir_entry *)((അക്ഷരം *)p + rlen);
}

സ്ഥിതം വരിയിൽ ശൂന്യം dx_set_block(ഘടന ext2_dx_entry *entry, unsigned മൂല്യം)
{
	entry->block = ext2fs_cpu_to_le32(മൂല്യം);
}

സ്ഥിതം വരിയിൽ ശൂന്യം dx_set_എണ്ണം(ഘടന ext2_dx_entry *entries, unsigned മൂല്യം)
{
	((ഘടന ext2_dx_countlimit *) entries)->എണ്ണം = ext2fs_cpu_to_le16(മൂല്യം);
}

സ്ഥിതം വരിയിൽ ശൂന്യം dx_set_limit(ഘടന ext2_dx_entry *entries, unsigned മൂല്യം)
{
	((ഘടന ext2_dx_countlimit *) entries)->limit = ext2fs_cpu_to_le16(മൂല്യം);
}

സ്ഥിതം വരിയിൽ unsigned dx_വേര്_limit(ext2_filsys fs, unsigned infosize)
{
	unsigned പൂർണ്ണസംഖ്യ entry_space = fs->blocksize -
			ext2fs_dir_rec_len(1, 0) -
			ext2fs_dir_rec_len(2, 0) - infosize;

	എങ്കിൽ (ext2fs_has_feature_metadata_csum(fs->super))
		entry_space -= വലുപ്പം(ഘടന ext2_dx_tail);
	തിരികെ entry_space / വലുപ്പം(ഘടന ext2_dx_entry);
}

/*
 * This converts a one block unindexed directory to a 3 block indexed
 * directory, and adds the dentry to the indexed directory.  Returns 0 if the
 * index was not created; EAGAIN if it was; or an errcode_t on error.
 */
സ്ഥിതം errcode_t try_make_indexed_dir(ext2_filsys fs, ext2_ino_t dir,
				      സ്ഥിരം അക്ഷരം *name, ext2_ino_t ino,
				      പൂർണ്ണസംഖ്യ flags)
{
	ഘടന ext2_inode inode;
	അക്ഷരം *buf0, *buf1, *മുകൾ;
	ഘടന dx_വേര് *വേര്;
	ഘടന ext2_dx_entry *entries;
	ഘടന fake_dirent *fde;
	ഘടന ext2_dir_entry *de, *de2;
	blk64_t pblk0, pblk1;
	unsigned പൂർണ്ണസംഖ്യ len;
	സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ blocksize = fs->blocksize;
	പൂർണ്ണസംഖ്യ csum_size = 0;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	എങ്കിൽ (retval)
		തിരികെ retval;

	എങ്കിൽ (inode.i_size > fs->blocksize || (inode.i_flags & EXT2_INDEX_FL))
		തിരികെ 0;

	എങ്കിൽ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = വലുപ്പം(ഘടന ext2_dir_entry_tail);

	retval = ext2fs_get_mem(blocksize, &buf0);
	എങ്കിൽ (retval)
		തിരികെ retval;

	retval = ext2fs_get_mem(blocksize, &buf1);
	എങ്കിൽ (retval)
		പോകുക out_buf0;

	retval = ext2fs_bmap2(fs, dir, &inode, ശൂന്യ, 0, 0, ശൂന്യ, &pblk0);
	എങ്കിൽ (retval)
		പോകുക out_buf1;

	retval = ext2fs_read_dir_block4(fs, pblk0, buf0, 0, dir);
	എങ്കിൽ (retval)
		പോകുക out_buf1;

	വേര് = (ഘടന dx_വേര് *)buf0;
	എങ്കിൽ (!check_dx_വേര്(fs, dir, വേര്)) {
		retval = EXT2_ET_DIR_CORRUPTED;
		പോകുക out_buf1;
	}

	/* The 0th block becomes the root, move the dirents out */
	fde = &വേര്->dotdot;
	de = ext2fs_next_entry(fs, (ഘടന ext2_dir_entry *)fde);
	len = ((അക്ഷരം *) വേര്) + (blocksize - csum_size) - (അക്ഷരം *) de;

	/* Allocate new block for the 0th block's dirents */
	retval = ext2fs_bmap2(fs, dir, &inode, ശൂന്യ, BMAP_ALLOC | BMAP_ZERO, 1,
			      ശൂന്യ, &pblk1);
	എങ്കിൽ (retval)
		പോകുക out_buf1;

	memcpy(buf1, de, len);
	memset(de, 0, len); /* wipe old data */

	de = (ഘടന ext2_dir_entry *)buf1;
	മുകൾ = buf1 + len;
	ആകുമ്പോൾ ((അക്ഷരം *)(de2 = ext2fs_next_entry(fs, de)) < മുകൾ) {
#എങ്കിൽ 0
		എങ്കിൽ (ext4_check_dir_entry(dir, ശൂന്യ, de, bh2, buf1, len,
					(അക്ഷരം *)de - buf1)) {
			retval = EXT2_ET_DIR_CORRUPTED;
			പോകുക out_buf1;
		}
#endif
		de = de2;
	}
	retval = ext2fs_set_rec_len(fs,
			buf1 + (blocksize - csum_size) - (അക്ഷരം *) de, de);
	എങ്കിൽ (retval)
		പോകുക out_buf1;

	എങ്കിൽ (csum_size)
		ext2fs_initialize_dirent_tail(fs,
				EXT2_DIRENT_TAIL(buf1, fs->blocksize));

	/* Initialize the root; the dot dirents already exist */
	de = (ഘടന ext2_dir_entry *) (&വേര്->dotdot);
	retval = ext2fs_set_rec_len(fs, blocksize - ext2fs_dir_rec_len(2, 0),
				    de);
	memset (&വേര്->info, 0, വലുപ്പം(വേര്->info));
	വേര്->info.info_length = വലുപ്പം(വേര്->info);
	എങ്കിൽ (ext4_hash_in_dirent(&inode))
		വേര്->info.hash_version = EXT2_HASH_SIPHASH;
	അല്ലെങ്കിൽ
		വേര്->info.hash_version = fs->super->s_def_hash_version;

	entries = വേര്->entries;
	dx_set_block(entries, 1);
	dx_set_എണ്ണം(entries, 1);
	dx_set_limit(entries, dx_വേര്_limit(fs, വലുപ്പം(വേര്->info)));

	retval = ext2fs_write_dir_block4(fs, pblk1, buf1, 0, dir);
	എങ്കിൽ (retval)
		പോകുക out_buf1;

	retval = ext2fs_write_dir_block4(fs, pblk0, buf0, 0, dir);
	എങ്കിൽ (retval)
		പോകുക out_buf1;

	inode.i_flags |= EXT2_INDEX_FL;
	inode.i_size += fs->blocksize;
	retval = ext2fs_write_inode(fs, dir, &inode);
	എങ്കിൽ (retval)
		പോകുക out_buf1;

	retval = EAGAIN;
out_buf1:
	ext2fs_free_mem(&buf1);
out_buf0:
	ext2fs_free_mem(&buf0);
	തിരികെ retval;
}

/*
 * Note: the low 3 bits of the flags field are used as the directory
 * entry filetype.
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, സ്ഥിരം അക്ഷരം *name,
		      ext2_ino_t ino, പൂർണ്ണസംഖ്യ flags)
{
	errcode_t		retval;
	ഘടന link_struct	ls;
	ഘടന ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	എങ്കിൽ (!(fs->flags & EXT2_FLAG_RW))
		തിരികെ EXT2_ET_RO_FILSYS;

retry:
	എങ്കിൽ ((retval = ext2fs_read_inode(fs, dir, &inode)) != 0)
		തിരികെ retval;

	എങ്കിൽ (inode.i_flags & EXT2_INDEX_FL)
		തിരികെ dx_link(fs, dir, &inode, name, ino, flags);

	ls.fs = fs;
	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	എങ്കിൽ ((flags & EXT2FS_LINK_APPEND) &&
	    !(inode.i_flags & EXT4_INLINE_DATA_FL)) {
		blk64_t		lblk, pblk = 0;
		ഘടന dir_context ctx;

		lblk = (inode.i_size / fs->blocksize) - 1;
		retval = ext2fs_bmap2(fs, dir, &inode, ശൂന്യ, 0, lblk,
				      ശൂന്യ, &pblk);
		എങ്കിൽ (retval)
			തിരികെ retval;

		ctx.dir = dir;
		ctx.flags = DIRENT_FLAG_INCLUDE_EMPTY;
		ctx.func = link_proc;
		ctx.priv_data = &ls;
		ctx.errcode = 0;
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		എങ്കിൽ (retval)
			തിരികെ retval;

		ext2fs_process_dir_block(fs, &pblk, lblk, 0, 0, &ctx);
		retval = ctx.errcode;
		ext2fs_free_mem(&ctx.buf);
	} അല്ലെങ്കിൽ {
		retval = ext2fs_dir_iterate2(fs, dir,
					     DIRENT_FLAG_INCLUDE_EMPTY,
					     ശൂന്യ, link_proc, &ls);
	}
	എങ്കിൽ (retval)
		തിരികെ retval;
	എങ്കിൽ (ls.err)
		തിരികെ ls.err;

	എങ്കിൽ (!ls.done) {
		എങ്കിൽ (!(flags & EXT2FS_LINK_EXPAND))
			തിരികെ EXT2_ET_DIR_NO_SPACE;

		എങ്കിൽ (ext2fs_has_feature_dir_index(fs->super)) {
			retval = try_make_indexed_dir(fs, dir, name, ino,
						      flags);
			എങ്കിൽ (retval == EAGAIN)
				പോകുക retry;
			എങ്കിൽ (retval)
				തിരികെ retval;
		}

		retval = ext2fs_expand_dir(fs, dir);
		എങ്കിൽ (retval)
			തിരികെ retval;
		പോകുക retry;
	}
	തിരികെ 0;
}

/* Does this directory have an htree index? */
പൂർണ്ണസംഖ്യ ext2fs_dir_is_dx(ext2_filsys fs, സ്ഥിരം ഘടന ext2_inode *inode)
{
	തിരികെ ext2fs_has_feature_dir_index(fs->super) &&
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
ശൂന്യം ext2fs_inc_nlink(ext2_filsys fs, ഘടന ext2_inode *inode)
{
	inode->i_links_എണ്ണം++;

	എങ്കിൽ (ext2fs_dir_is_dx(fs, inode) &&
	    (inode->i_links_എണ്ണം > EXT2_LINK_MAX || inode->i_links_എണ്ണം == 2))
		inode->i_links_എണ്ണം = 1;
}

/*
 * If a directory had nlink == 1, then we should let it be 1. This indicates
 * directory has >EXT2_LINK_MAX subdirs.
 */
ശൂന്യം ext2fs_dec_nlink(ഘടന ext2_inode *inode)
{
	എങ്കിൽ (!S_ISDIR(inode->i_mode) || inode->i_links_എണ്ണം > 2)
		inode->i_links_എണ്ണം--;
}

പൂർണ്ണസംഖ്യ ext2fs_dir_link_max(ext2_filsys fs, ഘടന ext2_inode_large *inode)
{
	തിരികെ inode->i_links_എണ്ണം >= EXT2_LINK_MAX &&
	       !(ext2fs_dir_is_dx(fs, EXT2_INODE(inode)) &&
		 ext2fs_has_feature_dir_nlink(fs->super));
}

പൂർണ്ണസംഖ്യ ext2fs_dir_link_empty(ഘടന ext2_inode *inode)
{
	തിരികെ inode->i_links_എണ്ണം == 2 || inode->i_links_എണ്ണം == 1;
}
