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

#include "config.h"
#include <stdio.h>
#include <string.h>
#གལ་ཏེ HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

#define EXT2_DX_ROOT_OFF 24

sa_nracanaa dx_frame {
	void *buf;
	blk64_t pblock;
	sa_nracanaa ext2_dx_countlimit *head;
	sa_nracanaa ext2_dx_entry *entries;
	sa_nracanaa ext2_dx_entry *at;
};

sa_nracanaa dx_lookup_info {
	const var_na *name;
	ཧྲིལ namelen;
	ཧྲིལ hash_alg;
	__u32 hash;
	unsigned levels;
	sa_nracanaa dx_frame frames[EXT4_HTREE_LEVEL];
};

static errcode_t alloc_dx_frame(ext2_filsys fs, sa_nracanaa dx_frame *frame)
{
	ཕྱིར ext2fs_get_mem(fs->blocksize, &frame->buf);
}

static void dx_release(sa_nracanaa dx_lookup_info *info)
{
	unsigned level;

	ཆེད་དུ (level = 0; level < info->levels; level++) {
		གལ་ཏེ (info->frames[level].buf == NULL)
			bha_nga;
		ext2fs_free_mem(&(info->frames[level].buf));
	}
	info->levels = 0;
}

static void dx_search_entry(sa_nracanaa dx_frame *frame, ཧྲིལ count, __u32 hash)
{
	sa_nracanaa ext2_dx_entry *p, *q, *m;

	p = frame->entries + 1;
	q = frame->entries + count - 1;
	སྐབས (p <= q) {
		m = p + (q - p) / 2;
		གལ་ཏེ (ext2fs_le32_to_cpu(m->hash) > hash)
			q = m - 1;
		དེ་མིན
			p = m + 1;
	}
	frame->at = p - 1;
}

static errcode_t load_logical_dir_block(ext2_filsys fs, ext2_ino_t dir,
					sa_nracanaa ext2_inode *diri, blk64_t block,
					blk64_t *pblk, void *buf)
{
	errcode_t errcode;
	ཧྲིལ ret_flags;

	errcode = ext2fs_bmap2(fs, dir, diri, NULL, 0, block, &ret_flags,
			       pblk);
	གལ་ཏེ (errcode)
		ཕྱིར errcode;
	གལ་ཏེ (ret_flags & BMAP_RET_UNINIT)
		ཕྱིར EXT2_ET_DIR_CORRUPTED;
	ཕྱིར ext2fs_read_dir_block4(fs, *pblk, buf, 0, dir);
}

static errcode_t dx_lookup(ext2_filsys fs, ext2_ino_t dir,
			   sa_nracanaa ext2_inode *diri, sa_nracanaa dx_lookup_info *info)
{
	sa_nracanaa ext2_dx_root_info *root;
	errcode_t errcode;
	ཧྲིལ level = 0;
	ཧྲིལ count, limit;
	ཧྲིལ hash_alg;
	ཧྲིལ hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	__u32 minor_hash;
	sa_nracanaa dx_frame *frame;

	errcode = alloc_dx_frame(fs, &(info->frames[0]));
	གལ་ཏེ (errcode)
		ཕྱིར errcode;
	info->levels = 1;

	errcode = load_logical_dir_block(fs, dir, diri, 0,
					 &(info->frames[0].pblock),
					 info->frames[0].buf);
	གལ་ཏེ (errcode)
		goto out_err;
	root = (sa_nracanaa ext2_dx_root_info *) ((var_na *)info->frames[0].buf +
					     EXT2_DX_ROOT_OFF);
	hash_alg = root->hash_version;
	གལ་ཏེ (hash_alg != EXT2_HASH_TEA && hash_alg != EXT2_HASH_HALF_MD4 &&
	    hash_alg != EXT2_HASH_LEGACY) {
		errcode = EXT2_ET_DIRHASH_UNSUPP;
		goto out_err;
	}
	གལ་ཏེ (hash_alg <= EXT2_HASH_TEA &&
	    fs->super->s_flags & EXT2_FLAGS_UNSIGNED_HASH)
		hash_alg += 3;
	གལ་ཏེ (root->indirect_levels >= ext2_dir_htree_level(fs)) {
		errcode = EXT2_ET_DIR_CORRUPTED;
		goto out_err;
	}
	info->hash_alg = hash_alg;

	errcode = ext2fs_dirhash2(hash_alg, info->name, info->namelen,
				  fs->encoding, hash_flags,
				  fs->super->s_hash_seed, &info->hash,
				  &minor_hash);
	གལ་ཏེ (errcode)
		goto out_err;

	ཆེད་དུ (level = 0; level <= root->indirect_levels; level++) {
		frame = &(info->frames[level]);
		གལ་ཏེ (level > 0) {
			errcode = alloc_dx_frame(fs, frame);
			གལ་ཏེ (errcode)
				goto out_err;
			info->levels++;

			errcode = load_logical_dir_block(fs, dir, diri,
				ext2fs_le32_to_cpu(info->frames[level-1].at->block) & 0x0fffffff,
				&(frame->pblock), frame->buf);
			གལ་ཏེ (errcode)
				goto out_err;
		}
		errcode = ext2fs_get_dx_countlimit(fs, frame->buf,
						   &(frame->head), NULL);
		གལ་ཏེ (errcode)
			goto out_err;
		count = ext2fs_le16_to_cpu(frame->head->count);
		limit = ext2fs_le16_to_cpu(frame->head->limit);
		frame->entries = (sa_nracanaa ext2_dx_entry *)(frame->head);
		གལ་ཏེ (!count || count > limit) {
			errcode = EXT2_ET_DIR_CORRUPTED;
			goto out_err;
		}

		dx_search_entry(frame, count, info->hash);
	}
	ཕྱིར 0;
out_err:
	dx_release(info);
	ཕྱིར errcode;
}

sa_nracanaa link_struct  {
	ext2_filsys	fs;
	const var_na	*name;
	ཧྲིལ		namelen;
	ext2_ino_t	inode;
	ཧྲིལ		flags;
	ཧྲིལ		done;
	unsigned ཧྲིལ	blocksize;
	errcode_t	err;
	sa_nracanaa ext2_super_block *sb;
};

static ཧྲིལ link_proc(ext2_ino_t dir EXT2FS_ATTR((unused)),
		     ཧྲིལ entru EXT2FS_ATTR((unused)),
		     sa_nracanaa ext2_dir_entry *dirent,
		     ཧྲིལ	offset,
		     ཧྲིལ	blocksize,
		     var_na	*buf,
		     void	*priv_data)
{
	sa_nracanaa link_struct *ls = (sa_nracanaa link_struct *) priv_data;
	sa_nracanaa ext2_dir_entry *next;
	unsigned ཧྲིལ rec_len, min_rec_len, curr_rec_len;
	ཧྲིལ ret = 0;
	ཧྲིལ csum_size = 0;

	གལ་ཏེ (ls->done)
		ཕྱིར DIRENT_ABORT;

	rec_len = EXT2_DIR_REC_LEN(ls->namelen);

	ls->err = ext2fs_get_rec_len(ls->fs, dirent, &curr_rec_len);
	གལ་ཏེ (ls->err)
		ཕྱིར DIRENT_ABORT;

	གལ་ཏེ (ext2fs_has_feature_metadata_csum(ls->fs->super))
		csum_size = parimaa_na(sa_nracanaa ext2_dir_entry_tail);
	/*
	 * See if the following directory entry (if any) is unused;
	 * if so, absorb it into this one.
	 */
	next = (sa_nracanaa ext2_dir_entry *) (buf + offset + curr_rec_len);
	གལ་ཏེ ((offset + (ཧྲིལ) curr_rec_len < blocksize - (8 + csum_size)) &&
	    (next->inode == 0) &&
	    (offset + (ཧྲིལ) curr_rec_len + (ཧྲིལ) next->rec_len <= blocksize)) {
		curr_rec_len += next->rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, curr_rec_len, dirent);
		གལ་ཏེ (ls->err)
			ཕྱིར DIRENT_ABORT;
		ret = DIRENT_CHANGED;
	}

	/*
	 * If the directory entry is used, see if we can split the
	 * directory entry to make room for the new name.  If so,
	 * truncate it and return.
	 */
	གལ་ཏེ (dirent->inode) {
		min_rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(dirent));
		གལ་ཏེ (curr_rec_len < (min_rec_len + rec_len))
			ཕྱིར ret;
		rec_len = curr_rec_len - min_rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, min_rec_len, dirent);
		གལ་ཏེ (ls->err)
			ཕྱིར DIRENT_ABORT;
		next = (sa_nracanaa ext2_dir_entry *) (buf + offset +
						  dirent->rec_len);
		next->inode = 0;
		ext2fs_dirent_set_name_len(next, 0);
		ext2fs_dirent_set_file_type(next, 0);
		ls->err = ext2fs_set_rec_len(ls->fs, rec_len, next);
		གལ་ཏེ (ls->err)
			ཕྱིར DIRENT_ABORT;
		ཕྱིར DIRENT_CHANGED;
	}

	/*
	 * If we get this far, then the directory entry is not used.
	 * See if we can fit the request entry in.  If so, do it.
	 */
	གལ་ཏེ (curr_rec_len < rec_len)
		ཕྱིར ret;
	dirent->inode = ls->inode;
	ext2fs_dirent_set_name_len(dirent, ls->namelen);
	strncpy(dirent->name, ls->name, ls->namelen);
	གལ་ཏེ (ext2fs_has_feature_filetype(ls->sb))
		ext2fs_dirent_set_file_type(dirent,
					    ls->flags & EXT2FS_LINK_FT_MASK);

	ls->done++;
	ཕྱིར DIRENT_ABORT|DIRENT_CHANGED;
}

static errcode_t add_dirent_to_buf(ext2_filsys fs, e2_blkcnt_t blockcnt,
				   var_na *buf, ext2_ino_t dir,
				   sa_nracanaa ext2_inode *diri, const var_na *name,
				   ext2_ino_t ino, ཧྲིལ flags, blk64_t *pblkp)
{
	sa_nracanaa dir_context ctx;
	sa_nracanaa link_struct ls;
	errcode_t retval;

	retval = load_logical_dir_block(fs, dir, diri, blockcnt, pblkp, buf);
	གལ་ཏེ (retval)
		ཕྱིར retval;
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
	གལ་ཏེ (ctx.errcode)
		ཕྱིར ctx.errcode;
	གལ་ཏེ (ls.err)
		ཕྱིར ls.err;
	གལ་ཏེ (!ls.done)
		ཕྱིར EXT2_ET_DIR_NO_SPACE;
	ཕྱིར 0;
}

sa_nracanaa dx_hash_map {
	__u32 hash;
	ཧྲིལ size;
	ཧྲིལ off;
};

static EXT2_QSORT_TYPE dx_hash_map_cmp(const void *ap, const void *bp)
{
	const sa_nracanaa dx_hash_map *a = ap, *b = bp;

	གལ་ཏེ (a->hash < b->hash)
		ཕྱིར -1;
	གལ་ཏེ (a->hash > b->hash)
		ཕྱིར 1;
	ཕྱིར 0;
}

static errcode_t dx_move_dirents(ext2_filsys fs, sa_nracanaa dx_hash_map *map,
				 ཧྲིལ count, void *from, void *to)
{
	sa_nracanaa ext2_dir_entry *de;
	ཧྲིལ i;
	ཧྲིལ rec_len = 0;
	errcode_t retval;
	ཧྲིལ csum_size = 0;
	void *base = to;

	གལ་ཏེ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = parimaa_na(sa_nracanaa ext2_dir_entry_tail);

	ཆེད་དུ (i = 0; i < count; i++) {
		de = (sa_nracanaa ext2_dir_entry *) ((var_na *)from + map[i].off);
		rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(de));
		memcpy(to, de, rec_len);
		retval = ext2fs_set_rec_len(fs, rec_len, to);
		གལ་ཏེ (retval)
			ཕྱིར retval;
		to = (var_na *)to + rec_len;
	}
	/*
	 * Update rec_len of the last dir entry to stretch to the end of block
	 */
	to = (var_na *)to - rec_len;
	rec_len = fs->blocksize - ((var_na *)to - (var_na *)base) - csum_size;
	retval = ext2fs_set_rec_len(fs, rec_len, to);
	གལ་ཏེ (retval)
		ཕྱིར retval;
	གལ་ཏེ (csum_size)
		ext2fs_initialize_dirent_tail(fs,
				EXT2_DIRENT_TAIL(base, fs->blocksize));
	ཕྱིར 0;
}

static errcode_t dx_insert_entry(ext2_filsys fs, ext2_ino_t dir,
				 sa_nracanaa dx_lookup_info *info, ཧྲིལ level,
				 __u32 hash, blk64_t lblk)
{
	ཧྲིལ pcount;
	sa_nracanaa ext2_dx_entry *top, *new;

	pcount = ext2fs_le16_to_cpu(info->frames[level].head->count);
	top = info->frames[level].entries + pcount;
	new = info->frames[level].at + 1;
	memmove(new + 1, new, (var_na *)top - (var_na *)new);
	new->hash = ext2fs_cpu_to_le32(hash);
	new->block = ext2fs_cpu_to_le32(lblk);
	info->frames[level].head->count = ext2fs_cpu_to_le16(pcount + 1);
	ཕྱིར ext2fs_write_dir_block4(fs, info->frames[level].pblock,
				       info->frames[level].buf, 0, dir);
}

static errcode_t dx_split_leaf(ext2_filsys fs, ext2_ino_t dir,
			       sa_nracanaa ext2_inode *diri,
			       sa_nracanaa dx_lookup_info *info, void *buf,
			       blk64_t leaf_pblk, blk64_t new_lblk,
			       blk64_t new_pblk)
{
	ཧྲིལ hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	sa_nracanaa ext2_dir_entry *de;
	void *buf2;
	errcode_t retval = 0;
	unsigned ཧྲིལ rec_len;
	unsigned ཧྲིལ offset, move_size;
	ཧྲིལ i, count = 0;
	sa_nracanaa dx_hash_map *map;
	ཧྲིལ continued;
	__u32 minor_hash;

	retval = ext2fs_get_mem(fs->blocksize, &buf2);
	གལ་ཏེ (retval)
		ཕྱིར retval;
	retval = ext2fs_get_array(fs->blocksize / 12,
				  parimaa_na(sa_nracanaa dx_hash_map), &map);
	གལ་ཏེ (retval) {
		ext2fs_free_mem(&buf2);
		ཕྱིར retval;
	}
	ཆེད་དུ (offset = 0; offset < fs->blocksize; offset += rec_len) {
		de = (sa_nracanaa ext2_dir_entry *) ((var_na *)buf + offset);
		retval = ext2fs_get_rec_len(fs, de, &rec_len);
		གལ་ཏེ (retval)
			goto out;
		གལ་ཏེ (ext2fs_dirent_name_len(de) > 0 && de->inode) {
			map[count].off = offset;
			map[count].size = rec_len;
			retval = ext2fs_dirhash2(info->hash_alg, de->name,
					ext2fs_dirent_name_len(de),
					fs->encoding, hash_flags,
					fs->super->s_hash_seed,
					&(map[count].hash),
					&minor_hash);
			གལ་ཏེ (retval)
				goto out;
			count++;
		}
	}
	qsort(map, count, parimaa_na(sa_nracanaa dx_hash_map), dx_hash_map_cmp);
	move_size = 0;
	/* Find place to split block */
	ཆེད་དུ (i = count - 1; i >= 0; i--) {
		གལ་ཏེ (move_size + map[i].size / 2 > fs->blocksize / 2)
			bha_nga;
		move_size += map[i].size;
	}
	/* Let i be the first entry to move */
	i++;
	/* Move selected directory entries to new block */
	retval = dx_move_dirents(fs, map + i, count - i, buf, buf2);
	གལ་ཏེ (retval)
		goto out;
	retval = ext2fs_write_dir_block4(fs, new_pblk, buf2, 0, dir);
	གལ་ཏེ (retval)
		goto out;
	/* Repack remaining entries in the old block */
	retval = dx_move_dirents(fs, map, i, buf, buf2);
	གལ་ཏེ (retval)
		goto out;
	retval = ext2fs_write_dir_block4(fs, leaf_pblk, buf2, 0, dir);
	གལ་ཏེ (retval)
		goto out;
	/* Update parent node */
	continued = map[i].hash == map[i-1].hash;
	retval = dx_insert_entry(fs, dir, info, info->levels - 1,
				 map[i].hash + continued, new_lblk);
out:
	ext2fs_free_mem(&buf2);
	ext2fs_free_mem(&map);
	ཕྱིར retval;
}

static errcode_t dx_grow_tree(ext2_filsys fs, ext2_ino_t dir,
			      sa_nracanaa ext2_inode *diri,
			      sa_nracanaa dx_lookup_info *info, void *buf,
			      blk64_t leaf_pblk)
{
	ཧྲིལ i;
	errcode_t retval;
	ext2_off64_t size = EXT2_I_SIZE(diri);
	blk64_t lblk, pblk;
	sa_nracanaa ext2_dir_entry *de;
	sa_nracanaa ext2_dx_countlimit *head;
	ཧྲིལ csum_size = 0;
	ཧྲིལ count;

	གལ་ཏེ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = parimaa_na(sa_nracanaa ext2_dx_tail);

	/* Find level which can accommodate new child */
	ཆེད་དུ (i = info->levels - 1; i >= 0; i--)
		གལ་ཏེ (ext2fs_le16_to_cpu(info->frames[i].head->count) <
		    ext2fs_le16_to_cpu(info->frames[i].head->limit))
			bha_nga;
	/* Need to grow tree depth? */
	གལ་ཏེ (i < 0 && info->levels >= ext2_dir_htree_level(fs))
		ཕྱིར EXT2_ET_DIR_NO_SPACE;
	lblk = size / fs->blocksize;
	size += fs->blocksize;
	retval = ext2fs_inode_size_set(fs, diri, size);
	གལ་ཏེ (retval)
		ཕྱིར retval;
	retval = ext2fs_fallocate(fs,
			EXT2_FALLOCATE_FORCE_INIT | EXT2_FALLOCATE_ZERO_BLOCKS,
			dir, diri, 0, lblk, 1);
	གལ་ཏེ (retval)
		ཕྱིར retval;
	retval = ext2fs_write_inode(fs, dir, diri);
	གལ་ཏེ (retval)
		ཕྱིར retval;
	retval = ext2fs_bmap2(fs, dir, diri, NULL, 0, lblk, NULL, &pblk);
	གལ་ཏེ (retval)
		ཕྱིར retval;
	/* Only leaf addition needed? */
	གལ་ཏེ (i == (ཧྲིལ)info->levels - 1)
		ཕྱིར dx_split_leaf(fs, dir, diri, info, buf, leaf_pblk,
				     lblk, pblk);

	de = buf;
	de->inode = 0;
	ext2fs_dirent_set_name_len(de, 0);
	ext2fs_dirent_set_file_type(de, 0);
	retval = ext2fs_set_rec_len(fs, fs->blocksize, de);
	གལ་ཏེ (retval)
		ཕྱིར retval;
	head = (sa_nracanaa ext2_dx_countlimit *) ((var_na *)buf + 8);
	count = ext2fs_le16_to_cpu(info->frames[i+1].head->count);
	/* Growing tree depth? */
	གལ་ཏེ (i < 0) {
		sa_nracanaa ext2_dx_root_info *root;

		memcpy(head, info->frames[0].entries,
		       count * parimaa_na(sa_nracanaa ext2_dx_entry));
		head->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_size)) /
				parimaa_na(sa_nracanaa ext2_dx_entry));
		/* head->count gets set by memcpy above to correct value */

		/* Now update tree root */
		info->frames[0].head->count = ext2fs_cpu_to_le16(1);
		info->frames[0].entries[0].block = ext2fs_cpu_to_le32(lblk);
		root = (sa_nracanaa ext2_dx_root_info *)
			((var_na *)info->frames[0].buf + EXT2_DX_ROOT_OFF);
		root->indirect_levels++;
	} དེ་མིན {
		/* Splitting internal node in two */
		ཧྲིལ count1 = count / 2;
		ཧྲིལ count2 = count - count1;
		__u32 split_hash = ext2fs_le32_to_cpu(info->frames[i+1].entries[count1].hash);

		memcpy(head, info->frames[i+1].entries + count1,
		       count2 * parimaa_na(sa_nracanaa ext2_dx_entry));
		head->count = ext2fs_cpu_to_le16(count2);
		head->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_size)) /
				parimaa_na(sa_nracanaa ext2_dx_entry));
		info->frames[i+1].head->count = ext2fs_cpu_to_le16(count1);

		/* Update parent node */
		retval = dx_insert_entry(fs, dir, info, i, split_hash, lblk);
		གལ་ཏེ (retval)
			ཕྱིར retval;

	}
	/* Writeout split block / updated root */
	retval = ext2fs_write_dir_block4(fs, info->frames[i+1].pblock,
					 info->frames[i+1].buf, 0, dir);
	གལ་ཏེ (retval)
		ཕྱིར retval;
	/* Writeout new tree block */
	retval = ext2fs_write_dir_block4(fs, pblk, buf, 0, dir);
	གལ་ཏེ (retval)
		ཕྱིར retval;
	ཕྱིར 0;
}

static errcode_t dx_link(ext2_filsys fs, ext2_ino_t dir,
			 sa_nracanaa ext2_inode *diri, const var_na *name,
			 ext2_ino_t ino, ཧྲིལ flags)
{
	sa_nracanaa dx_lookup_info dx_info;
	errcode_t retval;
	void *blockbuf;
	unsigned restart = 0;
	blk64_t leaf_pblk;

	retval = ext2fs_get_mem(fs->blocksize, &blockbuf);
	གལ་ཏེ (retval)
		ཕྱིར retval;

	dx_info.name = name;
	dx_info.namelen = strlen(name);
again:
	retval = dx_lookup(fs, dir, diri, &dx_info);
	གལ་ཏེ (retval)
		goto free_buf;

	retval = add_dirent_to_buf(fs,
		ext2fs_le32_to_cpu(dx_info.frames[dx_info.levels-1].at->block) & 0x0fffffff,
		blockbuf, dir, diri, name, ino, flags, &leaf_pblk);
	/*
	 * Success or error other than ENOSPC...? We are done. We may need upto
	 * two tries to add entry. One to split htree node and another to add
	 * new leaf block.
	 */
	གལ་ཏེ (restart >= dx_info.levels || retval != EXT2_ET_DIR_NO_SPACE)
		goto free_frames;
	retval = dx_grow_tree(fs, dir, diri, &dx_info, blockbuf, leaf_pblk);
	གལ་ཏེ (retval)
		goto free_frames;
	/* Restart everything now that the tree is larger */
	restart++;
	dx_release(&dx_info);
	goto again;
free_frames:
	dx_release(&dx_info);
free_buf:
	ext2fs_free_mem(&blockbuf);
	ཕྱིར retval;
}

sa_nracanaa fake_dirent
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

sa_nracanaa dx_root
{
	sa_nracanaa fake_dirent dot;
	var_na dot_name[4];
	sa_nracanaa fake_dirent dotdot;
	var_na dotdot_name[4];
	sa_nracanaa ext2_dx_root_info info;
	sa_nracanaa ext2_dx_entry entries[];
};

static ཧྲིལ check_dx_root(ext2_filsys fs, ext2_ino_t ino, sa_nracanaa dx_root *root)
{
	sa_nracanaa fake_dirent *fde;
	var_na *error_msg;
	unsigned ཧྲིལ rlen;
	var_na *blockend = (var_na *)root + fs->blocksize;

	fde = &root->dot;
	གལ་ཏེ (fde->name_len != 1) {
		error_msg = "invalid name_len for '.'";
		goto corrupted;
	}
	གལ་ཏེ (strncmp(root->dot_name, ".", fde->name_len)) {
		error_msg = "invalid name for '.'";
		goto corrupted;
	}

	ext2fs_get_rec_len(fs, (sa_nracanaa ext2_dir_entry *)fde, &rlen);
	གལ་ཏེ ((var_na *)fde + rlen >= blockend) {
		error_msg = "invalid rec_len for '.'";
		goto corrupted;
	}

	fde = &root->dotdot;
	གལ་ཏེ (fde->name_len != 2) {
		error_msg = "invalid name_len for '..'";
		goto corrupted;
	}
	གལ་ཏེ (strncmp(root->dotdot_name, "..", fde->name_len)) {
		error_msg = "invalid name for '..'";
		goto corrupted;
	}
	ext2fs_get_rec_len(fs, (sa_nracanaa ext2_dir_entry *)fde, &rlen);
	གལ་ཏེ ((var_na *)fde + rlen >= blockend) {
		error_msg = "invalid rec_len for '..'";
		goto corrupted;
	}

	ཕྱིར 1;

corrupted:
	fprintf(stderr, "Corrupt dir %u: %s, running e2fsck is recommended\n",
			 ino, error_msg);
	ཕྱིར 0;
}

static anta_hstha sa_nracanaa ext2_dir_entry *
ext2fs_next_entry(ext2_filsys fs, sa_nracanaa ext2_dir_entry *p)
{
	unsigned ཧྲིལ rlen;

	ext2fs_get_rec_len(fs, p, &rlen);
	ཕྱིར (sa_nracanaa ext2_dir_entry *)((var_na *)p + rlen);
}

static anta_hstha void dx_set_block(sa_nracanaa ext2_dx_entry *entry, unsigned value)
{
	entry->block = ext2fs_cpu_to_le32(value);
}

static anta_hstha void dx_set_count(sa_nracanaa ext2_dx_entry *entries, unsigned value)
{
	((sa_nracanaa ext2_dx_countlimit *) entries)->count = ext2fs_cpu_to_le16(value);
}

static anta_hstha void dx_set_limit(sa_nracanaa ext2_dx_entry *entries, unsigned value)
{
	((sa_nracanaa ext2_dx_countlimit *) entries)->limit = ext2fs_cpu_to_le16(value);
}

static anta_hstha unsigned dx_root_limit(ext2_filsys fs, unsigned infosize)
{
	unsigned ཧྲིལ entry_space = fs->blocksize -
			ext2fs_dir_rec_len(1, 0) -
			ext2fs_dir_rec_len(2, 0) - infosize;

	གལ་ཏེ (ext2fs_has_feature_metadata_csum(fs->super))
		entry_space -= parimaa_na(sa_nracanaa ext2_dx_tail);
	ཕྱིར entry_space / parimaa_na(sa_nracanaa ext2_dx_entry);
}

/*
 * This converts a one block unindexed directory to a 3 block indexed
 * directory, and adds the dentry to the indexed directory.  Returns 0 if the
 * index was not created; EAGAIN if it was; or an errcode_t on error.
 */
static errcode_t try_make_indexed_dir(ext2_filsys fs, ext2_ino_t dir,
				      const var_na *name, ext2_ino_t ino,
				      ཧྲིལ flags)
{
	sa_nracanaa ext2_inode inode;
	var_na *buf0, *buf1, *top;
	sa_nracanaa dx_root *root;
	sa_nracanaa ext2_dx_entry *entries;
	sa_nracanaa fake_dirent *fde;
	sa_nracanaa ext2_dir_entry *de, *de2;
	blk64_t pblk0, pblk1;
	unsigned ཧྲིལ len;
	const unsigned ཧྲིལ blocksize = fs->blocksize;
	ཧྲིལ csum_size = 0;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	གལ་ཏེ (retval)
		ཕྱིར retval;

	གལ་ཏེ (inode.i_size > fs->blocksize || (inode.i_flags & EXT2_INDEX_FL))
		ཕྱིར 0;

	གལ་ཏེ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = parimaa_na(sa_nracanaa ext2_dir_entry_tail);

	retval = ext2fs_get_mem(blocksize, &buf0);
	གལ་ཏེ (retval)
		ཕྱིར retval;

	retval = ext2fs_get_mem(blocksize, &buf1);
	གལ་ཏེ (retval)
		goto out_buf0;

	retval = ext2fs_bmap2(fs, dir, &inode, NULL, 0, 0, NULL, &pblk0);
	གལ་ཏེ (retval)
		goto out_buf1;

	retval = ext2fs_read_dir_block4(fs, pblk0, buf0, 0, dir);
	གལ་ཏེ (retval)
		goto out_buf1;

	root = (sa_nracanaa dx_root *)buf0;
	གལ་ཏེ (!check_dx_root(fs, dir, root)) {
		retval = EXT2_ET_DIR_CORRUPTED;
		goto out_buf1;
	}

	/* The 0th block becomes the root, move the dirents out */
	fde = &root->dotdot;
	de = ext2fs_next_entry(fs, (sa_nracanaa ext2_dir_entry *)fde);
	len = ((var_na *) root) + (blocksize - csum_size) - (var_na *) de;

	/* Allocate new block for the 0th block's dirents */
	retval = ext2fs_bmap2(fs, dir, &inode, NULL, BMAP_ALLOC | BMAP_ZERO, 1,
			      NULL, &pblk1);
	གལ་ཏེ (retval)
		goto out_buf1;

	memcpy(buf1, de, len);
	memset(de, 0, len); /* wipe old data */

	de = (sa_nracanaa ext2_dir_entry *)buf1;
	top = buf1 + len;
	སྐབས ((var_na *)(de2 = ext2fs_next_entry(fs, de)) < top) {
#གལ་ཏེ 0
		གལ་ཏེ (ext4_check_dir_entry(dir, NULL, de, bh2, buf1, len,
					(var_na *)de - buf1)) {
			retval = EXT2_ET_DIR_CORRUPTED;
			goto out_buf1;
		}
#endif
		de = de2;
	}
	retval = ext2fs_set_rec_len(fs,
			buf1 + (blocksize - csum_size) - (var_na *) de, de);
	གལ་ཏེ (retval)
		goto out_buf1;

	གལ་ཏེ (csum_size)
		ext2fs_initialize_dirent_tail(fs,
				EXT2_DIRENT_TAIL(buf1, fs->blocksize));

	/* Initialize the root; the dot dirents already exist */
	de = (sa_nracanaa ext2_dir_entry *) (&root->dotdot);
	retval = ext2fs_set_rec_len(fs, blocksize - ext2fs_dir_rec_len(2, 0),
				    de);
	memset (&root->info, 0, parimaa_na(root->info));
	root->info.info_length = parimaa_na(root->info);
	གལ་ཏེ (ext4_hash_in_dirent(&inode))
		root->info.hash_version = EXT2_HASH_SIPHASH;
	དེ་མིན
		root->info.hash_version = fs->super->s_def_hash_version;

	entries = root->entries;
	dx_set_block(entries, 1);
	dx_set_count(entries, 1);
	dx_set_limit(entries, dx_root_limit(fs, parimaa_na(root->info)));

	retval = ext2fs_write_dir_block4(fs, pblk1, buf1, 0, dir);
	གལ་ཏེ (retval)
		goto out_buf1;

	retval = ext2fs_write_dir_block4(fs, pblk0, buf0, 0, dir);
	གལ་ཏེ (retval)
		goto out_buf1;

	inode.i_flags |= EXT2_INDEX_FL;
	inode.i_size += fs->blocksize;
	retval = ext2fs_write_inode(fs, dir, &inode);
	གལ་ཏེ (retval)
		goto out_buf1;

	retval = EAGAIN;
out_buf1:
	ext2fs_free_mem(&buf1);
out_buf0:
	ext2fs_free_mem(&buf0);
	ཕྱིར retval;
}

/*
 * Note: the low 3 bits of the flags field are used as the directory
 * entry filetype.
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, const var_na *name,
		      ext2_ino_t ino, ཧྲིལ flags)
{
	errcode_t		retval;
	sa_nracanaa link_struct	ls;
	sa_nracanaa ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	གལ་ཏེ (!(fs->flags & EXT2_FLAG_RW))
		ཕྱིར EXT2_ET_RO_FILSYS;

retry:
	གལ་ཏེ ((retval = ext2fs_read_inode(fs, dir, &inode)) != 0)
		ཕྱིར retval;

	གལ་ཏེ (inode.i_flags & EXT2_INDEX_FL)
		ཕྱིར dx_link(fs, dir, &inode, name, ino, flags);

	ls.fs = fs;
	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	གལ་ཏེ ((flags & EXT2FS_LINK_APPEND) &&
	    !(inode.i_flags & EXT4_INLINE_DATA_FL)) {
		blk64_t		lblk, pblk = 0;
		sa_nracanaa dir_context ctx;

		lblk = (inode.i_size / fs->blocksize) - 1;
		retval = ext2fs_bmap2(fs, dir, &inode, NULL, 0, lblk,
				      NULL, &pblk);
		གལ་ཏེ (retval)
			ཕྱིར retval;

		ctx.dir = dir;
		ctx.flags = DIRENT_FLAG_INCLUDE_EMPTY;
		ctx.func = link_proc;
		ctx.priv_data = &ls;
		ctx.errcode = 0;
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		གལ་ཏེ (retval)
			ཕྱིར retval;

		ext2fs_process_dir_block(fs, &pblk, lblk, 0, 0, &ctx);
		retval = ctx.errcode;
		ext2fs_free_mem(&ctx.buf);
	} དེ་མིན {
		retval = ext2fs_dir_iterate2(fs, dir,
					     DIRENT_FLAG_INCLUDE_EMPTY,
					     NULL, link_proc, &ls);
	}
	གལ་ཏེ (retval)
		ཕྱིར retval;
	གལ་ཏེ (ls.err)
		ཕྱིར ls.err;

	གལ་ཏེ (!ls.done) {
		གལ་ཏེ (!(flags & EXT2FS_LINK_EXPAND))
			ཕྱིར EXT2_ET_DIR_NO_SPACE;

		གལ་ཏེ (ext2fs_has_feature_dir_index(fs->super)) {
			retval = try_make_indexed_dir(fs, dir, name, ino,
						      flags);
			གལ་ཏེ (retval == EAGAIN)
				goto retry;
			གལ་ཏེ (retval)
				ཕྱིར retval;
		}

		retval = ext2fs_expand_dir(fs, dir);
		གལ་ཏེ (retval)
			ཕྱིར retval;
		goto retry;
	}
	ཕྱིར 0;
}

/* Does this directory have an htree index? */
ཧྲིལ ext2fs_dir_is_dx(ext2_filsys fs, const sa_nracanaa ext2_inode *inode)
{
	ཕྱིར ext2fs_has_feature_dir_index(fs->super) &&
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
void ext2fs_inc_nlink(ext2_filsys fs, sa_nracanaa ext2_inode *inode)
{
	inode->i_links_count++;

	གལ་ཏེ (ext2fs_dir_is_dx(fs, inode) &&
	    (inode->i_links_count > EXT2_LINK_MAX || inode->i_links_count == 2))
		inode->i_links_count = 1;
}

/*
 * If a directory had nlink == 1, then we should let it be 1. This indicates
 * directory has >EXT2_LINK_MAX subdirs.
 */
void ext2fs_dec_nlink(sa_nracanaa ext2_inode *inode)
{
	གལ་ཏེ (!S_ISDIR(inode->i_mode) || inode->i_links_count > 2)
		inode->i_links_count--;
}

ཧྲིལ ext2fs_dir_link_max(ext2_filsys fs, sa_nracanaa ext2_inode_large *inode)
{
	ཕྱིར inode->i_links_count >= EXT2_LINK_MAX &&
	       !(ext2fs_dir_is_dx(fs, EXT2_INODE(inode)) &&
		 ext2fs_has_feature_dir_nlink(fs->super));
}

ཧྲིལ ext2fs_dir_link_empty(sa_nracanaa ext2_inode *inode)
{
	ཕྱིར inode->i_links_count == 2 || inode->i_links_count == 1;
}
