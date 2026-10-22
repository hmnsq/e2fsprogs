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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"
#અન્તર્ભાવ "ext2fsP.h"

#define EXT2_DX_મૂળ_OFF 24

સંરચના dx_frame {
	શૂન્ય *buf;
	blk64_t pblock;
	સંરચના ext2_dx_countlimit *head;
	સંરચના ext2_dx_entry *entries;
	સંરચના ext2_dx_entry *at;
};

સંરચના dx_lookup_info {
	અચલ અક્ષર *name;
	પૂર્ણાંક namelen;
	પૂર્ણાંક hash_alg;
	__u32 hash;
	unsigned levels;
	સંરચના dx_frame frames[EXT4_HTREE_LEVEL];
};

સ્થિર errcode_t alloc_dx_frame(ext2_filsys fs, સંરચના dx_frame *frame)
{
	પાછા ext2fs_get_mem(fs->blocksize, &frame->buf);
}

સ્થિર શૂન્ય dx_release(સંરચના dx_lookup_info *info)
{
	unsigned level;

	માટે (level = 0; level < info->levels; level++) {
		જો (info->frames[level].buf == ખાલી)
			તોડો;
		ext2fs_free_mem(&(info->frames[level].buf));
	}
	info->levels = 0;
}

સ્થિર શૂન્ય dx_શોધ_entry(સંરચના dx_frame *frame, પૂર્ણાંક ગણતરી, __u32 hash)
{
	સંરચના ext2_dx_entry *p, *q, *m;

	p = frame->entries + 1;
	q = frame->entries + ગણતરી - 1;
	જ્યારે (p <= q) {
		m = p + (q - p) / 2;
		જો (ext2fs_le32_to_cpu(m->hash) > hash)
			q = m - 1;
		નહીંતર
			p = m + 1;
	}
	frame->at = p - 1;
}

સ્થિર errcode_t load_logical_dir_block(ext2_filsys fs, ext2_ino_t dir,
					સંરચના ext2_inode *diri, blk64_t block,
					blk64_t *pblk, શૂન્ય *buf)
{
	errcode_t errcode;
	પૂર્ણાંક ret_flags;

	errcode = ext2fs_bmap2(fs, dir, diri, ખાલી, 0, block, &ret_flags,
			       pblk);
	જો (errcode)
		પાછા errcode;
	જો (ret_flags & BMAP_RET_UNINIT)
		પાછા EXT2_ET_DIR_CORRUPTED;
	પાછા ext2fs_read_dir_block4(fs, *pblk, buf, 0, dir);
}

સ્થિર errcode_t dx_lookup(ext2_filsys fs, ext2_ino_t dir,
			   સંરચના ext2_inode *diri, સંરચના dx_lookup_info *info)
{
	સંરચના ext2_dx_મૂળ_info *મૂળ;
	errcode_t errcode;
	પૂર્ણાંક level = 0;
	પૂર્ણાંક ગણતરી, limit;
	પૂર્ણાંક hash_alg;
	પૂર્ણાંક hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	__u32 minor_hash;
	સંરચના dx_frame *frame;

	errcode = alloc_dx_frame(fs, &(info->frames[0]));
	જો (errcode)
		પાછા errcode;
	info->levels = 1;

	errcode = load_logical_dir_block(fs, dir, diri, 0,
					 &(info->frames[0].pblock),
					 info->frames[0].buf);
	જો (errcode)
		જાઓ out_err;
	મૂળ = (સંરચના ext2_dx_મૂળ_info *) ((અક્ષર *)info->frames[0].buf +
					     EXT2_DX_મૂળ_OFF);
	hash_alg = મૂળ->hash_version;
	જો (hash_alg != EXT2_HASH_TEA && hash_alg != EXT2_HASH_HALF_MD4 &&
	    hash_alg != EXT2_HASH_LEGACY) {
		errcode = EXT2_ET_DIRHASH_UNSUPP;
		જાઓ out_err;
	}
	જો (hash_alg <= EXT2_HASH_TEA &&
	    fs->super->s_flags & EXT2_FLAGS_UNSIGNED_HASH)
		hash_alg += 3;
	જો (મૂળ->indirect_levels >= ext2_dir_htree_level(fs)) {
		errcode = EXT2_ET_DIR_CORRUPTED;
		જાઓ out_err;
	}
	info->hash_alg = hash_alg;

	errcode = ext2fs_dirhash2(hash_alg, info->name, info->namelen,
				  fs->encoding, hash_flags,
				  fs->super->s_hash_seed, &info->hash,
				  &minor_hash);
	જો (errcode)
		જાઓ out_err;

	માટે (level = 0; level <= મૂળ->indirect_levels; level++) {
		frame = &(info->frames[level]);
		જો (level > 0) {
			errcode = alloc_dx_frame(fs, frame);
			જો (errcode)
				જાઓ out_err;
			info->levels++;

			errcode = load_logical_dir_block(fs, dir, diri,
				ext2fs_le32_to_cpu(info->frames[level-1].at->block) & 0x0fffffff,
				&(frame->pblock), frame->buf);
			જો (errcode)
				જાઓ out_err;
		}
		errcode = ext2fs_get_dx_countlimit(fs, frame->buf,
						   &(frame->head), ખાલી);
		જો (errcode)
			જાઓ out_err;
		ગણતરી = ext2fs_le16_to_cpu(frame->head->ગણતરી);
		limit = ext2fs_le16_to_cpu(frame->head->limit);
		frame->entries = (સંરચના ext2_dx_entry *)(frame->head);
		જો (!ગણતરી || ગણતરી > limit) {
			errcode = EXT2_ET_DIR_CORRUPTED;
			જાઓ out_err;
		}

		dx_શોધ_entry(frame, ગણતરી, info->hash);
	}
	પાછા 0;
out_err:
	dx_release(info);
	પાછા errcode;
}

સંરચના link_struct  {
	ext2_filsys	fs;
	અચલ અક્ષર	*name;
	પૂર્ણાંક		namelen;
	ext2_ino_t	inode;
	પૂર્ણાંક		flags;
	પૂર્ણાંક		done;
	unsigned પૂર્ણાંક	blocksize;
	errcode_t	err;
	સંરચના ext2_super_block *sb;
};

સ્થિર પૂર્ણાંક link_proc(ext2_ino_t dir EXT2FS_ATTR((unused)),
		     પૂર્ણાંક entru EXT2FS_ATTR((unused)),
		     સંરચના ext2_dir_entry *dirent,
		     પૂર્ણાંક	offset,
		     પૂર્ણાંક	blocksize,
		     અક્ષર	*buf,
		     શૂન્ય	*priv_data)
{
	સંરચના link_struct *ls = (સંરચના link_struct *) priv_data;
	સંરચના ext2_dir_entry *next;
	unsigned પૂર્ણાંક rec_len, min_rec_len, curr_rec_len;
	પૂર્ણાંક ret = 0;
	પૂર્ણાંક csum_size = 0;

	જો (ls->done)
		પાછા DIRENT_ABORT;

	rec_len = EXT2_DIR_REC_LEN(ls->namelen);

	ls->err = ext2fs_get_rec_len(ls->fs, dirent, &curr_rec_len);
	જો (ls->err)
		પાછા DIRENT_ABORT;

	જો (ext2fs_has_feature_metadata_csum(ls->fs->super))
		csum_size = કદ(સંરચના ext2_dir_entry_tail);
	/*
	 * See if the following directory entry (if any) is unused;
	 * if so, absorb it into this one.
	 */
	next = (સંરચના ext2_dir_entry *) (buf + offset + curr_rec_len);
	જો ((offset + (પૂર્ણાંક) curr_rec_len < blocksize - (8 + csum_size)) &&
	    (next->inode == 0) &&
	    (offset + (પૂર્ણાંક) curr_rec_len + (પૂર્ણાંક) next->rec_len <= blocksize)) {
		curr_rec_len += next->rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, curr_rec_len, dirent);
		જો (ls->err)
			પાછા DIRENT_ABORT;
		ret = DIRENT_CHANGED;
	}

	/*
	 * If the directory entry is used, see if we can split the
	 * directory entry to make room for the new name.  If so,
	 * truncate it and return.
	 */
	જો (dirent->inode) {
		min_rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(dirent));
		જો (curr_rec_len < (min_rec_len + rec_len))
			પાછા ret;
		rec_len = curr_rec_len - min_rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, min_rec_len, dirent);
		જો (ls->err)
			પાછા DIRENT_ABORT;
		next = (સંરચના ext2_dir_entry *) (buf + offset +
						  dirent->rec_len);
		next->inode = 0;
		ext2fs_dirent_set_name_len(next, 0);
		ext2fs_dirent_set_file_type(next, 0);
		ls->err = ext2fs_set_rec_len(ls->fs, rec_len, next);
		જો (ls->err)
			પાછા DIRENT_ABORT;
		પાછા DIRENT_CHANGED;
	}

	/*
	 * If we get this far, then the directory entry is not used.
	 * See if we can fit the request entry in.  If so, do it.
	 */
	જો (curr_rec_len < rec_len)
		પાછા ret;
	dirent->inode = ls->inode;
	ext2fs_dirent_set_name_len(dirent, ls->namelen);
	strncpy(dirent->name, ls->name, ls->namelen);
	જો (ext2fs_has_feature_filetype(ls->sb))
		ext2fs_dirent_set_file_type(dirent,
					    ls->flags & EXT2FS_LINK_FT_MASK);

	ls->done++;
	પાછા DIRENT_ABORT|DIRENT_CHANGED;
}

સ્થિર errcode_t add_dirent_to_buf(ext2_filsys fs, e2_blkcnt_t blockcnt,
				   અક્ષર *buf, ext2_ino_t dir,
				   સંરચના ext2_inode *diri, અચલ અક્ષર *name,
				   ext2_ino_t ino, પૂર્ણાંક flags, blk64_t *pblkp)
{
	સંરચના dir_context ctx;
	સંરચના link_struct ls;
	errcode_t retval;

	retval = load_logical_dir_block(fs, dir, diri, blockcnt, pblkp, buf);
	જો (retval)
		પાછા retval;
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
	જો (ctx.errcode)
		પાછા ctx.errcode;
	જો (ls.err)
		પાછા ls.err;
	જો (!ls.done)
		પાછા EXT2_ET_DIR_NO_SPACE;
	પાછા 0;
}

સંરચના dx_hash_map {
	__u32 hash;
	પૂર્ણાંક size;
	પૂર્ણાંક off;
};

સ્થિર EXT2_QSORT_TYPE dx_hash_map_cmp(અચલ શૂન્ય *ap, અચલ શૂન્ય *bp)
{
	અચલ સંરચના dx_hash_map *a = ap, *b = bp;

	જો (a->hash < b->hash)
		પાછા -1;
	જો (a->hash > b->hash)
		પાછા 1;
	પાછા 0;
}

સ્થિર errcode_t dx_move_dirents(ext2_filsys fs, સંરચના dx_hash_map *map,
				 પૂર્ણાંક ગણતરી, શૂન્ય *from, શૂન્ય *to)
{
	સંરચના ext2_dir_entry *de;
	પૂર્ણાંક i;
	પૂર્ણાંક rec_len = 0;
	errcode_t retval;
	પૂર્ણાંક csum_size = 0;
	શૂન્ય *base = to;

	જો (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = કદ(સંરચના ext2_dir_entry_tail);

	માટે (i = 0; i < ગણતરી; i++) {
		de = (સંરચના ext2_dir_entry *) ((અક્ષર *)from + map[i].off);
		rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(de));
		memcpy(to, de, rec_len);
		retval = ext2fs_set_rec_len(fs, rec_len, to);
		જો (retval)
			પાછા retval;
		to = (અક્ષર *)to + rec_len;
	}
	/*
	 * Update rec_len of the last dir entry to stretch to the end of block
	 */
	to = (અક્ષર *)to - rec_len;
	rec_len = fs->blocksize - ((અક્ષર *)to - (અક્ષર *)base) - csum_size;
	retval = ext2fs_set_rec_len(fs, rec_len, to);
	જો (retval)
		પાછા retval;
	જો (csum_size)
		ext2fs_initialize_dirent_tail(fs,
				EXT2_DIRENT_TAIL(base, fs->blocksize));
	પાછા 0;
}

સ્થિર errcode_t dx_ઉમેરો_entry(ext2_filsys fs, ext2_ino_t dir,
				 સંરચના dx_lookup_info *info, પૂર્ણાંક level,
				 __u32 hash, blk64_t lblk)
{
	પૂર્ણાંક pcount;
	સંરચના ext2_dx_entry *ટોચ, *new;

	pcount = ext2fs_le16_to_cpu(info->frames[level].head->ગણતરી);
	ટોચ = info->frames[level].entries + pcount;
	new = info->frames[level].at + 1;
	memmove(new + 1, new, (અક્ષર *)ટોચ - (અક્ષર *)new);
	new->hash = ext2fs_cpu_to_le32(hash);
	new->block = ext2fs_cpu_to_le32(lblk);
	info->frames[level].head->ગણતરી = ext2fs_cpu_to_le16(pcount + 1);
	પાછા ext2fs_write_dir_block4(fs, info->frames[level].pblock,
				       info->frames[level].buf, 0, dir);
}

સ્થિર errcode_t dx_split_leaf(ext2_filsys fs, ext2_ino_t dir,
			       સંરચના ext2_inode *diri,
			       સંરચના dx_lookup_info *info, શૂન્ય *buf,
			       blk64_t leaf_pblk, blk64_t new_lblk,
			       blk64_t new_pblk)
{
	પૂર્ણાંક hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	સંરચના ext2_dir_entry *de;
	શૂન્ય *buf2;
	errcode_t retval = 0;
	unsigned પૂર્ણાંક rec_len;
	unsigned પૂર્ણાંક offset, move_size;
	પૂર્ણાંક i, ગણતરી = 0;
	સંરચના dx_hash_map *map;
	પૂર્ણાંક continued;
	__u32 minor_hash;

	retval = ext2fs_get_mem(fs->blocksize, &buf2);
	જો (retval)
		પાછા retval;
	retval = ext2fs_get_array(fs->blocksize / 12,
				  કદ(સંરચના dx_hash_map), &map);
	જો (retval) {
		ext2fs_free_mem(&buf2);
		પાછા retval;
	}
	માટે (offset = 0; offset < fs->blocksize; offset += rec_len) {
		de = (સંરચના ext2_dir_entry *) ((અક્ષર *)buf + offset);
		retval = ext2fs_get_rec_len(fs, de, &rec_len);
		જો (retval)
			જાઓ out;
		જો (ext2fs_dirent_name_len(de) > 0 && de->inode) {
			map[ગણતરી].off = offset;
			map[ગણતરી].size = rec_len;
			retval = ext2fs_dirhash2(info->hash_alg, de->name,
					ext2fs_dirent_name_len(de),
					fs->encoding, hash_flags,
					fs->super->s_hash_seed,
					&(map[ગણતરી].hash),
					&minor_hash);
			જો (retval)
				જાઓ out;
			ગણતરી++;
		}
	}
	qsort(map, ગણતરી, કદ(સંરચના dx_hash_map), dx_hash_map_cmp);
	move_size = 0;
	/* Find place to split block */
	માટે (i = ગણતરી - 1; i >= 0; i--) {
		જો (move_size + map[i].size / 2 > fs->blocksize / 2)
			તોડો;
		move_size += map[i].size;
	}
	/* Let i be the first entry to move */
	i++;
	/* Move selected directory entries to new block */
	retval = dx_move_dirents(fs, map + i, ગણતરી - i, buf, buf2);
	જો (retval)
		જાઓ out;
	retval = ext2fs_write_dir_block4(fs, new_pblk, buf2, 0, dir);
	જો (retval)
		જાઓ out;
	/* Repack remaining entries in the old block */
	retval = dx_move_dirents(fs, map, i, buf, buf2);
	જો (retval)
		જાઓ out;
	retval = ext2fs_write_dir_block4(fs, leaf_pblk, buf2, 0, dir);
	જો (retval)
		જાઓ out;
	/* Update parent node */
	continued = map[i].hash == map[i-1].hash;
	retval = dx_ઉમેરો_entry(fs, dir, info, info->levels - 1,
				 map[i].hash + continued, new_lblk);
out:
	ext2fs_free_mem(&buf2);
	ext2fs_free_mem(&map);
	પાછા retval;
}

સ્થિર errcode_t dx_grow_વૃક્ષ(ext2_filsys fs, ext2_ino_t dir,
			      સંરચના ext2_inode *diri,
			      સંરચના dx_lookup_info *info, શૂન્ય *buf,
			      blk64_t leaf_pblk)
{
	પૂર્ણાંક i;
	errcode_t retval;
	ext2_off64_t size = EXT2_I_SIZE(diri);
	blk64_t lblk, pblk;
	સંરચના ext2_dir_entry *de;
	સંરચના ext2_dx_countlimit *head;
	પૂર્ણાંક csum_size = 0;
	પૂર્ણાંક ગણતરી;

	જો (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = કદ(સંરચના ext2_dx_tail);

	/* Find level which can accommodate new child */
	માટે (i = info->levels - 1; i >= 0; i--)
		જો (ext2fs_le16_to_cpu(info->frames[i].head->ગણતરી) <
		    ext2fs_le16_to_cpu(info->frames[i].head->limit))
			તોડો;
	/* Need to grow tree depth? */
	જો (i < 0 && info->levels >= ext2_dir_htree_level(fs))
		પાછા EXT2_ET_DIR_NO_SPACE;
	lblk = size / fs->blocksize;
	size += fs->blocksize;
	retval = ext2fs_inode_size_set(fs, diri, size);
	જો (retval)
		પાછા retval;
	retval = ext2fs_fallocate(fs,
			EXT2_FALLOCATE_FORCE_INIT | EXT2_FALLOCATE_ZERO_BLOCKS,
			dir, diri, 0, lblk, 1);
	જો (retval)
		પાછા retval;
	retval = ext2fs_write_inode(fs, dir, diri);
	જો (retval)
		પાછા retval;
	retval = ext2fs_bmap2(fs, dir, diri, ખાલી, 0, lblk, ખાલી, &pblk);
	જો (retval)
		પાછા retval;
	/* Only leaf addition needed? */
	જો (i == (પૂર્ણાંક)info->levels - 1)
		પાછા dx_split_leaf(fs, dir, diri, info, buf, leaf_pblk,
				     lblk, pblk);

	de = buf;
	de->inode = 0;
	ext2fs_dirent_set_name_len(de, 0);
	ext2fs_dirent_set_file_type(de, 0);
	retval = ext2fs_set_rec_len(fs, fs->blocksize, de);
	જો (retval)
		પાછા retval;
	head = (સંરચના ext2_dx_countlimit *) ((અક્ષર *)buf + 8);
	ગણતરી = ext2fs_le16_to_cpu(info->frames[i+1].head->ગણતરી);
	/* Growing tree depth? */
	જો (i < 0) {
		સંરચના ext2_dx_મૂળ_info *મૂળ;

		memcpy(head, info->frames[0].entries,
		       ગણતરી * કદ(સંરચના ext2_dx_entry));
		head->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_size)) /
				કદ(સંરચના ext2_dx_entry));
		/* head->count gets set by memcpy above to correct value */

		/* Now update tree root */
		info->frames[0].head->ગણતરી = ext2fs_cpu_to_le16(1);
		info->frames[0].entries[0].block = ext2fs_cpu_to_le32(lblk);
		મૂળ = (સંરચના ext2_dx_મૂળ_info *)
			((અક્ષર *)info->frames[0].buf + EXT2_DX_મૂળ_OFF);
		મૂળ->indirect_levels++;
	} નહીંતર {
		/* Splitting internal node in two */
		પૂર્ણાંક count1 = ગણતરી / 2;
		પૂર્ણાંક count2 = ગણતરી - count1;
		__u32 split_hash = ext2fs_le32_to_cpu(info->frames[i+1].entries[count1].hash);

		memcpy(head, info->frames[i+1].entries + count1,
		       count2 * કદ(સંરચના ext2_dx_entry));
		head->ગણતરી = ext2fs_cpu_to_le16(count2);
		head->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_size)) /
				કદ(સંરચના ext2_dx_entry));
		info->frames[i+1].head->ગણતરી = ext2fs_cpu_to_le16(count1);

		/* Update parent node */
		retval = dx_ઉમેરો_entry(fs, dir, info, i, split_hash, lblk);
		જો (retval)
			પાછા retval;

	}
	/* Writeout split block / updated root */
	retval = ext2fs_write_dir_block4(fs, info->frames[i+1].pblock,
					 info->frames[i+1].buf, 0, dir);
	જો (retval)
		પાછા retval;
	/* Writeout new tree block */
	retval = ext2fs_write_dir_block4(fs, pblk, buf, 0, dir);
	જો (retval)
		પાછા retval;
	પાછા 0;
}

સ્થિર errcode_t dx_link(ext2_filsys fs, ext2_ino_t dir,
			 સંરચના ext2_inode *diri, અચલ અક્ષર *name,
			 ext2_ino_t ino, પૂર્ણાંક flags)
{
	સંરચના dx_lookup_info dx_info;
	errcode_t retval;
	શૂન્ય *blockbuf;
	unsigned restart = 0;
	blk64_t leaf_pblk;

	retval = ext2fs_get_mem(fs->blocksize, &blockbuf);
	જો (retval)
		પાછા retval;

	dx_info.name = name;
	dx_info.namelen = strlen(name);
again:
	retval = dx_lookup(fs, dir, diri, &dx_info);
	જો (retval)
		જાઓ free_buf;

	retval = add_dirent_to_buf(fs,
		ext2fs_le32_to_cpu(dx_info.frames[dx_info.levels-1].at->block) & 0x0fffffff,
		blockbuf, dir, diri, name, ino, flags, &leaf_pblk);
	/*
	 * Success or error other than ENOSPC...? We are done. We may need upto
	 * two tries to add entry. One to split htree node and another to add
	 * new leaf block.
	 */
	જો (restart >= dx_info.levels || retval != EXT2_ET_DIR_NO_SPACE)
		જાઓ free_frames;
	retval = dx_grow_વૃક્ષ(fs, dir, diri, &dx_info, blockbuf, leaf_pblk);
	જો (retval)
		જાઓ free_frames;
	/* Restart everything now that the tree is larger */
	restart++;
	dx_release(&dx_info);
	જાઓ again;
free_frames:
	dx_release(&dx_info);
free_buf:
	ext2fs_free_mem(&blockbuf);
	પાછા retval;
}

સંરચના fake_dirent
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

સંરચના dx_મૂળ
{
	સંરચના fake_dirent dot;
	અક્ષર dot_name[4];
	સંરચના fake_dirent dotdot;
	અક્ષર dotdot_name[4];
	સંરચના ext2_dx_મૂળ_info info;
	સંરચના ext2_dx_entry entries[];
};

સ્થિર પૂર્ણાંક check_dx_મૂળ(ext2_filsys fs, ext2_ino_t ino, સંરચના dx_મૂળ *મૂળ)
{
	સંરચના fake_dirent *fde;
	અક્ષર *error_સંદેશ;
	unsigned પૂર્ણાંક rlen;
	અક્ષર *blockend = (અક્ષર *)મૂળ + fs->blocksize;

	fde = &મૂળ->dot;
	જો (fde->name_len != 1) {
		error_સંદેશ = "invalid name_len for '.'";
		જાઓ corrupted;
	}
	જો (strncmp(મૂળ->dot_name, ".", fde->name_len)) {
		error_સંદેશ = "invalid name for '.'";
		જાઓ corrupted;
	}

	ext2fs_get_rec_len(fs, (સંરચના ext2_dir_entry *)fde, &rlen);
	જો ((અક્ષર *)fde + rlen >= blockend) {
		error_સંદેશ = "invalid rec_len for '.'";
		જાઓ corrupted;
	}

	fde = &મૂળ->dotdot;
	જો (fde->name_len != 2) {
		error_સંદેશ = "invalid name_len for '..'";
		જાઓ corrupted;
	}
	જો (strncmp(મૂળ->dotdot_name, "..", fde->name_len)) {
		error_સંદેશ = "invalid name for '..'";
		જાઓ corrupted;
	}
	ext2fs_get_rec_len(fs, (સંરચના ext2_dir_entry *)fde, &rlen);
	જો ((અક્ષર *)fde + rlen >= blockend) {
		error_સંદેશ = "invalid rec_len for '..'";
		જાઓ corrupted;
	}

	પાછા 1;

corrupted:
	fprintf(stderr, "Corrupt dir %u: %s, running e2fsck is recommended\n",
			 ino, error_સંદેશ);
	પાછા 0;
}

સ્થિર અંતઃરેખ સંરચના ext2_dir_entry *
ext2fs_next_entry(ext2_filsys fs, સંરચના ext2_dir_entry *p)
{
	unsigned પૂર્ણાંક rlen;

	ext2fs_get_rec_len(fs, p, &rlen);
	પાછા (સંરચના ext2_dir_entry *)((અક્ષર *)p + rlen);
}

સ્થિર અંતઃરેખ શૂન્ય dx_set_block(સંરચના ext2_dx_entry *entry, unsigned કિંમત)
{
	entry->block = ext2fs_cpu_to_le32(કિંમત);
}

સ્થિર અંતઃરેખ શૂન્ય dx_set_ગણતરી(સંરચના ext2_dx_entry *entries, unsigned કિંમત)
{
	((સંરચના ext2_dx_countlimit *) entries)->ગણતરી = ext2fs_cpu_to_le16(કિંમત);
}

સ્થિર અંતઃરેખ શૂન્ય dx_set_limit(સંરચના ext2_dx_entry *entries, unsigned કિંમત)
{
	((સંરચના ext2_dx_countlimit *) entries)->limit = ext2fs_cpu_to_le16(કિંમત);
}

સ્થિર અંતઃરેખ unsigned dx_મૂળ_limit(ext2_filsys fs, unsigned infosize)
{
	unsigned પૂર્ણાંક entry_space = fs->blocksize -
			ext2fs_dir_rec_len(1, 0) -
			ext2fs_dir_rec_len(2, 0) - infosize;

	જો (ext2fs_has_feature_metadata_csum(fs->super))
		entry_space -= કદ(સંરચના ext2_dx_tail);
	પાછા entry_space / કદ(સંરચના ext2_dx_entry);
}

/*
 * This converts a one block unindexed directory to a 3 block indexed
 * directory, and adds the dentry to the indexed directory.  Returns 0 if the
 * index was not created; EAGAIN if it was; or an errcode_t on error.
 */
સ્થિર errcode_t try_make_indexed_dir(ext2_filsys fs, ext2_ino_t dir,
				      અચલ અક્ષર *name, ext2_ino_t ino,
				      પૂર્ણાંક flags)
{
	સંરચના ext2_inode inode;
	અક્ષર *buf0, *buf1, *ટોચ;
	સંરચના dx_મૂળ *મૂળ;
	સંરચના ext2_dx_entry *entries;
	સંરચના fake_dirent *fde;
	સંરચના ext2_dir_entry *de, *de2;
	blk64_t pblk0, pblk1;
	unsigned પૂર્ણાંક len;
	અચલ unsigned પૂર્ણાંક blocksize = fs->blocksize;
	પૂર્ણાંક csum_size = 0;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	જો (retval)
		પાછા retval;

	જો (inode.i_size > fs->blocksize || (inode.i_flags & EXT2_INDEX_FL))
		પાછા 0;

	જો (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = કદ(સંરચના ext2_dir_entry_tail);

	retval = ext2fs_get_mem(blocksize, &buf0);
	જો (retval)
		પાછા retval;

	retval = ext2fs_get_mem(blocksize, &buf1);
	જો (retval)
		જાઓ out_buf0;

	retval = ext2fs_bmap2(fs, dir, &inode, ખાલી, 0, 0, ખાલી, &pblk0);
	જો (retval)
		જાઓ out_buf1;

	retval = ext2fs_read_dir_block4(fs, pblk0, buf0, 0, dir);
	જો (retval)
		જાઓ out_buf1;

	મૂળ = (સંરચના dx_મૂળ *)buf0;
	જો (!check_dx_મૂળ(fs, dir, મૂળ)) {
		retval = EXT2_ET_DIR_CORRUPTED;
		જાઓ out_buf1;
	}

	/* The 0th block becomes the root, move the dirents out */
	fde = &મૂળ->dotdot;
	de = ext2fs_next_entry(fs, (સંરચના ext2_dir_entry *)fde);
	len = ((અક્ષર *) મૂળ) + (blocksize - csum_size) - (અક્ષર *) de;

	/* Allocate new block for the 0th block's dirents */
	retval = ext2fs_bmap2(fs, dir, &inode, ખાલી, BMAP_ALLOC | BMAP_ZERO, 1,
			      ખાલી, &pblk1);
	જો (retval)
		જાઓ out_buf1;

	memcpy(buf1, de, len);
	memset(de, 0, len); /* wipe old data */

	de = (સંરચના ext2_dir_entry *)buf1;
	ટોચ = buf1 + len;
	જ્યારે ((અક્ષર *)(de2 = ext2fs_next_entry(fs, de)) < ટોચ) {
#જો 0
		જો (ext4_check_dir_entry(dir, ખાલી, de, bh2, buf1, len,
					(અક્ષર *)de - buf1)) {
			retval = EXT2_ET_DIR_CORRUPTED;
			જાઓ out_buf1;
		}
#endif
		de = de2;
	}
	retval = ext2fs_set_rec_len(fs,
			buf1 + (blocksize - csum_size) - (અક્ષર *) de, de);
	જો (retval)
		જાઓ out_buf1;

	જો (csum_size)
		ext2fs_initialize_dirent_tail(fs,
				EXT2_DIRENT_TAIL(buf1, fs->blocksize));

	/* Initialize the root; the dot dirents already exist */
	de = (સંરચના ext2_dir_entry *) (&મૂળ->dotdot);
	retval = ext2fs_set_rec_len(fs, blocksize - ext2fs_dir_rec_len(2, 0),
				    de);
	memset (&મૂળ->info, 0, કદ(મૂળ->info));
	મૂળ->info.info_length = કદ(મૂળ->info);
	જો (ext4_hash_in_dirent(&inode))
		મૂળ->info.hash_version = EXT2_HASH_SIPHASH;
	નહીંતર
		મૂળ->info.hash_version = fs->super->s_def_hash_version;

	entries = મૂળ->entries;
	dx_set_block(entries, 1);
	dx_set_ગણતરી(entries, 1);
	dx_set_limit(entries, dx_મૂળ_limit(fs, કદ(મૂળ->info)));

	retval = ext2fs_write_dir_block4(fs, pblk1, buf1, 0, dir);
	જો (retval)
		જાઓ out_buf1;

	retval = ext2fs_write_dir_block4(fs, pblk0, buf0, 0, dir);
	જો (retval)
		જાઓ out_buf1;

	inode.i_flags |= EXT2_INDEX_FL;
	inode.i_size += fs->blocksize;
	retval = ext2fs_write_inode(fs, dir, &inode);
	જો (retval)
		જાઓ out_buf1;

	retval = EAGAIN;
out_buf1:
	ext2fs_free_mem(&buf1);
out_buf0:
	ext2fs_free_mem(&buf0);
	પાછા retval;
}

/*
 * Note: the low 3 bits of the flags field are used as the directory
 * entry filetype.
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, અચલ અક્ષર *name,
		      ext2_ino_t ino, પૂર્ણાંક flags)
{
	errcode_t		retval;
	સંરચના link_struct	ls;
	સંરચના ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	જો (!(fs->flags & EXT2_FLAG_RW))
		પાછા EXT2_ET_RO_FILSYS;

retry:
	જો ((retval = ext2fs_read_inode(fs, dir, &inode)) != 0)
		પાછા retval;

	જો (inode.i_flags & EXT2_INDEX_FL)
		પાછા dx_link(fs, dir, &inode, name, ino, flags);

	ls.fs = fs;
	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	જો ((flags & EXT2FS_LINK_APPEND) &&
	    !(inode.i_flags & EXT4_INLINE_DATA_FL)) {
		blk64_t		lblk, pblk = 0;
		સંરચના dir_context ctx;

		lblk = (inode.i_size / fs->blocksize) - 1;
		retval = ext2fs_bmap2(fs, dir, &inode, ખાલી, 0, lblk,
				      ખાલી, &pblk);
		જો (retval)
			પાછા retval;

		ctx.dir = dir;
		ctx.flags = DIRENT_FLAG_INCLUDE_EMPTY;
		ctx.func = link_proc;
		ctx.priv_data = &ls;
		ctx.errcode = 0;
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		જો (retval)
			પાછા retval;

		ext2fs_process_dir_block(fs, &pblk, lblk, 0, 0, &ctx);
		retval = ctx.errcode;
		ext2fs_free_mem(&ctx.buf);
	} નહીંતર {
		retval = ext2fs_dir_iterate2(fs, dir,
					     DIRENT_FLAG_INCLUDE_EMPTY,
					     ખાલી, link_proc, &ls);
	}
	જો (retval)
		પાછા retval;
	જો (ls.err)
		પાછા ls.err;

	જો (!ls.done) {
		જો (!(flags & EXT2FS_LINK_EXPAND))
			પાછા EXT2_ET_DIR_NO_SPACE;

		જો (ext2fs_has_feature_dir_index(fs->super)) {
			retval = try_make_indexed_dir(fs, dir, name, ino,
						      flags);
			જો (retval == EAGAIN)
				જાઓ retry;
			જો (retval)
				પાછા retval;
		}

		retval = ext2fs_expand_dir(fs, dir);
		જો (retval)
			પાછા retval;
		જાઓ retry;
	}
	પાછા 0;
}

/* Does this directory have an htree index? */
પૂર્ણાંક ext2fs_dir_is_dx(ext2_filsys fs, અચલ સંરચના ext2_inode *inode)
{
	પાછા ext2fs_has_feature_dir_index(fs->super) &&
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
શૂન્ય ext2fs_inc_nlink(ext2_filsys fs, સંરચના ext2_inode *inode)
{
	inode->i_links_ગણતરી++;

	જો (ext2fs_dir_is_dx(fs, inode) &&
	    (inode->i_links_ગણતરી > EXT2_LINK_MAX || inode->i_links_ગણતરી == 2))
		inode->i_links_ગણતરી = 1;
}

/*
 * If a directory had nlink == 1, then we should let it be 1. This indicates
 * directory has >EXT2_LINK_MAX subdirs.
 */
શૂન્ય ext2fs_dec_nlink(સંરચના ext2_inode *inode)
{
	જો (!S_ISDIR(inode->i_mode) || inode->i_links_ગણતરી > 2)
		inode->i_links_ગણતરી--;
}

પૂર્ણાંક ext2fs_dir_link_max(ext2_filsys fs, સંરચના ext2_inode_large *inode)
{
	પાછા inode->i_links_ગણતરી >= EXT2_LINK_MAX &&
	       !(ext2fs_dir_is_dx(fs, EXT2_INODE(inode)) &&
		 ext2fs_has_feature_dir_nlink(fs->super));
}

પૂર્ણાંક ext2fs_dir_link_empty(સંરચના ext2_inode *inode)
{
	પાછા inode->i_links_ગણતરી == 2 || inode->i_links_ગણતરી == 1;
}
