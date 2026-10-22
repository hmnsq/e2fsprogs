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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

#define EXT2_DX_मूल_OFF 24

संरचना dx_frame {
	शून्य *buf;
	blk64_t pblock;
	संरचना ext2_dx_countlimit *head;
	संरचना ext2_dx_entry *entries;
	संरचना ext2_dx_entry *at;
};

संरचना dx_lookup_info {
	ध्रुव वर्ण *name;
	पूर्णाङ्क namelen;
	पूर्णाङ्क hash_alg;
	__u32 hash;
	अचिन्हित levels;
	संरचना dx_frame frames[EXT4_HTREE_LEVEL];
};

स्थिर errcode_t alloc_dx_frame(ext2_filsys fs, संरचना dx_frame *frame)
{
	प्रत्यागम ext2fs_get_mem(fs->blocksize, &frame->buf);
}

स्थिर शून्य dx_release(संरचना dx_lookup_info *info)
{
	अचिन्हित level;

	प्रति (level = 0; level < info->levels; level++) {
		यदि (info->frames[level].buf == रिक्त)
			भङ्ग;
		ext2fs_free_mem(&(info->frames[level].buf));
	}
	info->levels = 0;
}

स्थिर शून्य dx_खोज_entry(संरचना dx_frame *frame, पूर्णाङ्क गिनती, __u32 hash)
{
	संरचना ext2_dx_entry *p, *q, *m;

	p = frame->entries + 1;
	q = frame->entries + गिनती - 1;
	यावत् (p <= q) {
		m = p + (q - p) / 2;
		यदि (ext2fs_le32_to_cpu(m->hash) > hash)
			q = m - 1;
		अन्यथा
			p = m + 1;
	}
	frame->at = p - 1;
}

स्थिर errcode_t load_logical_dir_block(ext2_filsys fs, ext2_ino_t dir,
					संरचना ext2_inode *diri, blk64_t block,
					blk64_t *pblk, शून्य *buf)
{
	errcode_t errcode;
	पूर्णाङ्क ret_flags;

	errcode = ext2fs_bmap2(fs, dir, diri, रिक्त, 0, block, &ret_flags,
			       pblk);
	यदि (errcode)
		प्रत्यागम errcode;
	यदि (ret_flags & BMAP_RET_UNINIT)
		प्रत्यागम EXT2_ET_DIR_CORRUPTED;
	प्रत्यागम ext2fs_read_dir_block4(fs, *pblk, buf, 0, dir);
}

स्थिर errcode_t dx_lookup(ext2_filsys fs, ext2_ino_t dir,
			   संरचना ext2_inode *diri, संरचना dx_lookup_info *info)
{
	संरचना ext2_dx_मूल_info *मूल;
	errcode_t errcode;
	पूर्णाङ्क level = 0;
	पूर्णाङ्क गिनती, limit;
	पूर्णाङ्क hash_alg;
	पूर्णाङ्क hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	__u32 minor_hash;
	संरचना dx_frame *frame;

	errcode = alloc_dx_frame(fs, &(info->frames[0]));
	यदि (errcode)
		प्रत्यागम errcode;
	info->levels = 1;

	errcode = load_logical_dir_block(fs, dir, diri, 0,
					 &(info->frames[0].pblock),
					 info->frames[0].buf);
	यदि (errcode)
		गच्छ out_err;
	मूल = (संरचना ext2_dx_मूल_info *) ((वर्ण *)info->frames[0].buf +
					     EXT2_DX_मूल_OFF);
	hash_alg = मूल->hash_version;
	यदि (hash_alg != EXT2_HASH_TEA && hash_alg != EXT2_HASH_HALF_MD4 &&
	    hash_alg != EXT2_HASH_LEGACY) {
		errcode = EXT2_ET_DIRHASH_UNSUPP;
		गच्छ out_err;
	}
	यदि (hash_alg <= EXT2_HASH_TEA &&
	    fs->super->s_flags & EXT2_FLAGS_UNSIGNED_HASH)
		hash_alg += 3;
	यदि (मूल->indirect_levels >= ext2_dir_htree_level(fs)) {
		errcode = EXT2_ET_DIR_CORRUPTED;
		गच्छ out_err;
	}
	info->hash_alg = hash_alg;

	errcode = ext2fs_dirhash2(hash_alg, info->name, info->namelen,
				  fs->encoding, hash_flags,
				  fs->super->s_hash_seed, &info->hash,
				  &minor_hash);
	यदि (errcode)
		गच्छ out_err;

	प्रति (level = 0; level <= मूल->indirect_levels; level++) {
		frame = &(info->frames[level]);
		यदि (level > 0) {
			errcode = alloc_dx_frame(fs, frame);
			यदि (errcode)
				गच्छ out_err;
			info->levels++;

			errcode = load_logical_dir_block(fs, dir, diri,
				ext2fs_le32_to_cpu(info->frames[level-1].at->block) & 0x0fffffff,
				&(frame->pblock), frame->buf);
			यदि (errcode)
				गच्छ out_err;
		}
		errcode = ext2fs_get_dx_countlimit(fs, frame->buf,
						   &(frame->head), रिक्त);
		यदि (errcode)
			गच्छ out_err;
		गिनती = ext2fs_le16_to_cpu(frame->head->गिनती);
		limit = ext2fs_le16_to_cpu(frame->head->limit);
		frame->entries = (संरचना ext2_dx_entry *)(frame->head);
		यदि (!गिनती || गिनती > limit) {
			errcode = EXT2_ET_DIR_CORRUPTED;
			गच्छ out_err;
		}

		dx_खोज_entry(frame, गिनती, info->hash);
	}
	प्रत्यागम 0;
out_err:
	dx_release(info);
	प्रत्यागम errcode;
}

संरचना link_struct  {
	ext2_filsys	fs;
	ध्रुव वर्ण	*name;
	पूर्णाङ्क		namelen;
	ext2_ino_t	inode;
	पूर्णाङ्क		flags;
	पूर्णाङ्क		done;
	अचिन्हित पूर्णाङ्क	blocksize;
	errcode_t	err;
	संरचना ext2_super_block *sb;
};

स्थिर पूर्णाङ्क link_proc(ext2_ino_t dir EXT2FS_ATTR((unused)),
		     पूर्णाङ्क entru EXT2FS_ATTR((unused)),
		     संरचना ext2_dir_entry *dirent,
		     पूर्णाङ्क	offset,
		     पूर्णाङ्क	blocksize,
		     वर्ण	*buf,
		     शून्य	*priv_data)
{
	संरचना link_struct *ls = (संरचना link_struct *) priv_data;
	संरचना ext2_dir_entry *next;
	अचिन्हित पूर्णाङ्क rec_len, min_rec_len, curr_rec_len;
	पूर्णाङ्क ret = 0;
	पूर्णाङ्क csum_size = 0;

	यदि (ls->done)
		प्रत्यागम DIRENT_ABORT;

	rec_len = EXT2_DIR_REC_LEN(ls->namelen);

	ls->err = ext2fs_get_rec_len(ls->fs, dirent, &curr_rec_len);
	यदि (ls->err)
		प्रत्यागम DIRENT_ABORT;

	यदि (ext2fs_has_feature_metadata_csum(ls->fs->super))
		csum_size = परिमाण(संरचना ext2_dir_entry_tail);
	/*
	 * See if the following directory entry (if any) is unused;
	 * if so, absorb it into this one.
	 */
	next = (संरचना ext2_dir_entry *) (buf + offset + curr_rec_len);
	यदि ((offset + (पूर्णाङ्क) curr_rec_len < blocksize - (8 + csum_size)) &&
	    (next->inode == 0) &&
	    (offset + (पूर्णाङ्क) curr_rec_len + (पूर्णाङ्क) next->rec_len <= blocksize)) {
		curr_rec_len += next->rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, curr_rec_len, dirent);
		यदि (ls->err)
			प्रत्यागम DIRENT_ABORT;
		ret = DIRENT_CHANGED;
	}

	/*
	 * If the directory entry is used, see if we can split the
	 * directory entry to make room for the new name.  If so,
	 * truncate it and return.
	 */
	यदि (dirent->inode) {
		min_rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(dirent));
		यदि (curr_rec_len < (min_rec_len + rec_len))
			प्रत्यागम ret;
		rec_len = curr_rec_len - min_rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, min_rec_len, dirent);
		यदि (ls->err)
			प्रत्यागम DIRENT_ABORT;
		next = (संरचना ext2_dir_entry *) (buf + offset +
						  dirent->rec_len);
		next->inode = 0;
		ext2fs_dirent_set_name_len(next, 0);
		ext2fs_dirent_set_file_type(next, 0);
		ls->err = ext2fs_set_rec_len(ls->fs, rec_len, next);
		यदि (ls->err)
			प्रत्यागम DIRENT_ABORT;
		प्रत्यागम DIRENT_CHANGED;
	}

	/*
	 * If we get this far, then the directory entry is not used.
	 * See if we can fit the request entry in.  If so, do it.
	 */
	यदि (curr_rec_len < rec_len)
		प्रत्यागम ret;
	dirent->inode = ls->inode;
	ext2fs_dirent_set_name_len(dirent, ls->namelen);
	म_नकलन(dirent->name, ls->name, ls->namelen);
	यदि (ext2fs_has_feature_filetype(ls->sb))
		ext2fs_dirent_set_file_type(dirent,
					    ls->flags & EXT2FS_LINK_FT_MASK);

	ls->done++;
	प्रत्यागम DIRENT_ABORT|DIRENT_CHANGED;
}

स्थिर errcode_t add_dirent_to_buf(ext2_filsys fs, e2_blkcnt_t blockcnt,
				   वर्ण *buf, ext2_ino_t dir,
				   संरचना ext2_inode *diri, ध्रुव वर्ण *name,
				   ext2_ino_t ino, पूर्णाङ्क flags, blk64_t *pblkp)
{
	संरचना dir_context ctx;
	संरचना link_struct ls;
	errcode_t retval;

	retval = load_logical_dir_block(fs, dir, diri, blockcnt, pblkp, buf);
	यदि (retval)
		प्रत्यागम retval;
	ctx.errcode = 0;
	ctx.func = link_proc;
	ctx.dir = dir;
	ctx.flags = DIRENT_FLAG_INCLUDE_EMPTY;
	ctx.buf = buf;
	ctx.priv_data = &ls;

	ls.fs = fs;
	ls.name = name;
	ls.namelen = म_माप(name);
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	ext2fs_process_dir_block(fs, pblkp, blockcnt, 0, 0, &ctx);
	यदि (ctx.errcode)
		प्रत्यागम ctx.errcode;
	यदि (ls.err)
		प्रत्यागम ls.err;
	यदि (!ls.done)
		प्रत्यागम EXT2_ET_DIR_NO_SPACE;
	प्रत्यागम 0;
}

संरचना dx_hash_map {
	__u32 hash;
	पूर्णाङ्क size;
	पूर्णाङ्क off;
};

स्थिर EXT2_QSORT_TYPE dx_hash_map_cmp(ध्रुव शून्य *ap, ध्रुव शून्य *bp)
{
	ध्रुव संरचना dx_hash_map *a = ap, *b = bp;

	यदि (a->hash < b->hash)
		प्रत्यागम -1;
	यदि (a->hash > b->hash)
		प्रत्यागम 1;
	प्रत्यागम 0;
}

स्थिर errcode_t dx_move_dirents(ext2_filsys fs, संरचना dx_hash_map *map,
				 पूर्णाङ्क गिनती, शून्य *from, शून्य *to)
{
	संरचना ext2_dir_entry *de;
	पूर्णाङ्क i;
	पूर्णाङ्क rec_len = 0;
	errcode_t retval;
	पूर्णाङ्क csum_size = 0;
	शून्य *base = to;

	यदि (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = परिमाण(संरचना ext2_dir_entry_tail);

	प्रति (i = 0; i < गिनती; i++) {
		de = (संरचना ext2_dir_entry *) ((वर्ण *)from + map[i].off);
		rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(de));
		स_नकल(to, de, rec_len);
		retval = ext2fs_set_rec_len(fs, rec_len, to);
		यदि (retval)
			प्रत्यागम retval;
		to = (वर्ण *)to + rec_len;
	}
	/*
	 * Update rec_len of the last dir entry to stretch to the end of block
	 */
	to = (वर्ण *)to - rec_len;
	rec_len = fs->blocksize - ((वर्ण *)to - (वर्ण *)base) - csum_size;
	retval = ext2fs_set_rec_len(fs, rec_len, to);
	यदि (retval)
		प्रत्यागम retval;
	यदि (csum_size)
		ext2fs_initialize_dirent_tail(fs,
				EXT2_DIRENT_TAIL(base, fs->blocksize));
	प्रत्यागम 0;
}

स्थिर errcode_t dx_जोड़ो_entry(ext2_filsys fs, ext2_ino_t dir,
				 संरचना dx_lookup_info *info, पूर्णाङ्क level,
				 __u32 hash, blk64_t lblk)
{
	पूर्णाङ्क pcount;
	संरचना ext2_dx_entry *शीर्ष, *new;

	pcount = ext2fs_le16_to_cpu(info->frames[level].head->गिनती);
	शीर्ष = info->frames[level].entries + pcount;
	new = info->frames[level].at + 1;
	स_हटाओ(new + 1, new, (वर्ण *)शीर्ष - (वर्ण *)new);
	new->hash = ext2fs_cpu_to_le32(hash);
	new->block = ext2fs_cpu_to_le32(lblk);
	info->frames[level].head->गिनती = ext2fs_cpu_to_le16(pcount + 1);
	प्रत्यागम ext2fs_write_dir_block4(fs, info->frames[level].pblock,
				       info->frames[level].buf, 0, dir);
}

स्थिर errcode_t dx_split_leaf(ext2_filsys fs, ext2_ino_t dir,
			       संरचना ext2_inode *diri,
			       संरचना dx_lookup_info *info, शून्य *buf,
			       blk64_t leaf_pblk, blk64_t new_lblk,
			       blk64_t new_pblk)
{
	पूर्णाङ्क hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	संरचना ext2_dir_entry *de;
	शून्य *buf2;
	errcode_t retval = 0;
	अचिन्हित पूर्णाङ्क rec_len;
	अचिन्हित पूर्णाङ्क offset, move_size;
	पूर्णाङ्क i, गिनती = 0;
	संरचना dx_hash_map *map;
	पूर्णाङ्क continued;
	__u32 minor_hash;

	retval = ext2fs_get_mem(fs->blocksize, &buf2);
	यदि (retval)
		प्रत्यागम retval;
	retval = ext2fs_get_array(fs->blocksize / 12,
				  परिमाण(संरचना dx_hash_map), &map);
	यदि (retval) {
		ext2fs_free_mem(&buf2);
		प्रत्यागम retval;
	}
	प्रति (offset = 0; offset < fs->blocksize; offset += rec_len) {
		de = (संरचना ext2_dir_entry *) ((वर्ण *)buf + offset);
		retval = ext2fs_get_rec_len(fs, de, &rec_len);
		यदि (retval)
			गच्छ out;
		यदि (ext2fs_dirent_name_len(de) > 0 && de->inode) {
			map[गिनती].off = offset;
			map[गिनती].size = rec_len;
			retval = ext2fs_dirhash2(info->hash_alg, de->name,
					ext2fs_dirent_name_len(de),
					fs->encoding, hash_flags,
					fs->super->s_hash_seed,
					&(map[गिनती].hash),
					&minor_hash);
			यदि (retval)
				गच्छ out;
			गिनती++;
		}
	}
	क्विक(map, गिनती, परिमाण(संरचना dx_hash_map), dx_hash_map_cmp);
	move_size = 0;
	/* Find place to split block */
	प्रति (i = गिनती - 1; i >= 0; i--) {
		यदि (move_size + map[i].size / 2 > fs->blocksize / 2)
			भङ्ग;
		move_size += map[i].size;
	}
	/* Let i be the first entry to move */
	i++;
	/* Move selected directory entries to new block */
	retval = dx_move_dirents(fs, map + i, गिनती - i, buf, buf2);
	यदि (retval)
		गच्छ out;
	retval = ext2fs_write_dir_block4(fs, new_pblk, buf2, 0, dir);
	यदि (retval)
		गच्छ out;
	/* Repack remaining entries in the old block */
	retval = dx_move_dirents(fs, map, i, buf, buf2);
	यदि (retval)
		गच्छ out;
	retval = ext2fs_write_dir_block4(fs, leaf_pblk, buf2, 0, dir);
	यदि (retval)
		गच्छ out;
	/* Update parent node */
	continued = map[i].hash == map[i-1].hash;
	retval = dx_जोड़ो_entry(fs, dir, info, info->levels - 1,
				 map[i].hash + continued, new_lblk);
out:
	ext2fs_free_mem(&buf2);
	ext2fs_free_mem(&map);
	प्रत्यागम retval;
}

स्थिर errcode_t dx_grow_वृक्ष(ext2_filsys fs, ext2_ino_t dir,
			      संरचना ext2_inode *diri,
			      संरचना dx_lookup_info *info, शून्य *buf,
			      blk64_t leaf_pblk)
{
	पूर्णाङ्क i;
	errcode_t retval;
	ext2_off64_t size = EXT2_I_SIZE(diri);
	blk64_t lblk, pblk;
	संरचना ext2_dir_entry *de;
	संरचना ext2_dx_countlimit *head;
	पूर्णाङ्क csum_size = 0;
	पूर्णाङ्क गिनती;

	यदि (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = परिमाण(संरचना ext2_dx_tail);

	/* Find level which can accommodate new child */
	प्रति (i = info->levels - 1; i >= 0; i--)
		यदि (ext2fs_le16_to_cpu(info->frames[i].head->गिनती) <
		    ext2fs_le16_to_cpu(info->frames[i].head->limit))
			भङ्ग;
	/* Need to grow tree depth? */
	यदि (i < 0 && info->levels >= ext2_dir_htree_level(fs))
		प्रत्यागम EXT2_ET_DIR_NO_SPACE;
	lblk = size / fs->blocksize;
	size += fs->blocksize;
	retval = ext2fs_inode_size_set(fs, diri, size);
	यदि (retval)
		प्रत्यागम retval;
	retval = ext2fs_fallocate(fs,
			EXT2_FALLOCATE_FORCE_INIT | EXT2_FALLOCATE_ZERO_BLOCKS,
			dir, diri, 0, lblk, 1);
	यदि (retval)
		प्रत्यागम retval;
	retval = ext2fs_write_inode(fs, dir, diri);
	यदि (retval)
		प्रत्यागम retval;
	retval = ext2fs_bmap2(fs, dir, diri, रिक्त, 0, lblk, रिक्त, &pblk);
	यदि (retval)
		प्रत्यागम retval;
	/* Only leaf addition needed? */
	यदि (i == (पूर्णाङ्क)info->levels - 1)
		प्रत्यागम dx_split_leaf(fs, dir, diri, info, buf, leaf_pblk,
				     lblk, pblk);

	de = buf;
	de->inode = 0;
	ext2fs_dirent_set_name_len(de, 0);
	ext2fs_dirent_set_file_type(de, 0);
	retval = ext2fs_set_rec_len(fs, fs->blocksize, de);
	यदि (retval)
		प्रत्यागम retval;
	head = (संरचना ext2_dx_countlimit *) ((वर्ण *)buf + 8);
	गिनती = ext2fs_le16_to_cpu(info->frames[i+1].head->गिनती);
	/* Growing tree depth? */
	यदि (i < 0) {
		संरचना ext2_dx_मूल_info *मूल;

		स_नकल(head, info->frames[0].entries,
		       गिनती * परिमाण(संरचना ext2_dx_entry));
		head->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_size)) /
				परिमाण(संरचना ext2_dx_entry));
		/* head->count gets set by memcpy above to correct value */

		/* Now update tree root */
		info->frames[0].head->गिनती = ext2fs_cpu_to_le16(1);
		info->frames[0].entries[0].block = ext2fs_cpu_to_le32(lblk);
		मूल = (संरचना ext2_dx_मूल_info *)
			((वर्ण *)info->frames[0].buf + EXT2_DX_मूल_OFF);
		मूल->indirect_levels++;
	} अन्यथा {
		/* Splitting internal node in two */
		पूर्णाङ्क count1 = गिनती / 2;
		पूर्णाङ्क count2 = गिनती - count1;
		__u32 split_hash = ext2fs_le32_to_cpu(info->frames[i+1].entries[count1].hash);

		स_नकल(head, info->frames[i+1].entries + count1,
		       count2 * परिमाण(संरचना ext2_dx_entry));
		head->गिनती = ext2fs_cpu_to_le16(count2);
		head->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_size)) /
				परिमाण(संरचना ext2_dx_entry));
		info->frames[i+1].head->गिनती = ext2fs_cpu_to_le16(count1);

		/* Update parent node */
		retval = dx_जोड़ो_entry(fs, dir, info, i, split_hash, lblk);
		यदि (retval)
			प्रत्यागम retval;

	}
	/* Writeout split block / updated root */
	retval = ext2fs_write_dir_block4(fs, info->frames[i+1].pblock,
					 info->frames[i+1].buf, 0, dir);
	यदि (retval)
		प्रत्यागम retval;
	/* Writeout new tree block */
	retval = ext2fs_write_dir_block4(fs, pblk, buf, 0, dir);
	यदि (retval)
		प्रत्यागम retval;
	प्रत्यागम 0;
}

स्थिर errcode_t dx_link(ext2_filsys fs, ext2_ino_t dir,
			 संरचना ext2_inode *diri, ध्रुव वर्ण *name,
			 ext2_ino_t ino, पूर्णाङ्क flags)
{
	संरचना dx_lookup_info dx_info;
	errcode_t retval;
	शून्य *blockbuf;
	अचिन्हित restart = 0;
	blk64_t leaf_pblk;

	retval = ext2fs_get_mem(fs->blocksize, &blockbuf);
	यदि (retval)
		प्रत्यागम retval;

	dx_info.name = name;
	dx_info.namelen = म_माप(name);
again:
	retval = dx_lookup(fs, dir, diri, &dx_info);
	यदि (retval)
		गच्छ free_buf;

	retval = add_dirent_to_buf(fs,
		ext2fs_le32_to_cpu(dx_info.frames[dx_info.levels-1].at->block) & 0x0fffffff,
		blockbuf, dir, diri, name, ino, flags, &leaf_pblk);
	/*
	 * Success or error other than ENOSPC...? We are done. We may need upto
	 * two tries to add entry. One to split htree node and another to add
	 * new leaf block.
	 */
	यदि (restart >= dx_info.levels || retval != EXT2_ET_DIR_NO_SPACE)
		गच्छ free_frames;
	retval = dx_grow_वृक्ष(fs, dir, diri, &dx_info, blockbuf, leaf_pblk);
	यदि (retval)
		गच्छ free_frames;
	/* Restart everything now that the tree is larger */
	restart++;
	dx_release(&dx_info);
	गच्छ again;
free_frames:
	dx_release(&dx_info);
free_buf:
	ext2fs_free_mem(&blockbuf);
	प्रत्यागम retval;
}

संरचना fake_dirent
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

संरचना dx_मूल
{
	संरचना fake_dirent dot;
	वर्ण dot_name[4];
	संरचना fake_dirent dotdot;
	वर्ण dotdot_name[4];
	संरचना ext2_dx_मूल_info info;
	संरचना ext2_dx_entry entries[];
};

स्थिर पूर्णाङ्क check_dx_मूल(ext2_filsys fs, ext2_ino_t ino, संरचना dx_मूल *मूल)
{
	संरचना fake_dirent *fde;
	वर्ण *error_संदेश;
	अचिन्हित पूर्णाङ्क rlen;
	वर्ण *blockend = (वर्ण *)मूल + fs->blocksize;

	fde = &मूल->dot;
	यदि (fde->name_len != 1) {
		error_संदेश = "invalid name_len for '.'";
		गच्छ corrupted;
	}
	यदि (म_भेदन(मूल->dot_name, ".", fde->name_len)) {
		error_संदेश = "invalid name for '.'";
		गच्छ corrupted;
	}

	ext2fs_get_rec_len(fs, (संरचना ext2_dir_entry *)fde, &rlen);
	यदि ((वर्ण *)fde + rlen >= blockend) {
		error_संदेश = "invalid rec_len for '.'";
		गच्छ corrupted;
	}

	fde = &मूल->dotdot;
	यदि (fde->name_len != 2) {
		error_संदेश = "invalid name_len for '..'";
		गच्छ corrupted;
	}
	यदि (म_भेदन(मूल->dotdot_name, "..", fde->name_len)) {
		error_संदेश = "invalid name for '..'";
		गच्छ corrupted;
	}
	ext2fs_get_rec_len(fs, (संरचना ext2_dir_entry *)fde, &rlen);
	यदि ((वर्ण *)fde + rlen >= blockend) {
		error_संदेश = "invalid rec_len for '..'";
		गच्छ corrupted;
	}

	प्रत्यागम 1;

corrupted:
	ख_लिखो(मानक_त्रुटि, "Corrupt dir %u: %s, running e2fsck is recommended\n",
			 ino, error_संदेश);
	प्रत्यागम 0;
}

स्थिर अन्तःस्थ संरचना ext2_dir_entry *
ext2fs_next_entry(ext2_filsys fs, संरचना ext2_dir_entry *p)
{
	अचिन्हित पूर्णाङ्क rlen;

	ext2fs_get_rec_len(fs, p, &rlen);
	प्रत्यागम (संरचना ext2_dir_entry *)((वर्ण *)p + rlen);
}

स्थिर अन्तःस्थ शून्य dx_set_block(संरचना ext2_dx_entry *entry, अचिन्हित मूल्य)
{
	entry->block = ext2fs_cpu_to_le32(मूल्य);
}

स्थिर अन्तःस्थ शून्य dx_set_गिनती(संरचना ext2_dx_entry *entries, अचिन्हित मूल्य)
{
	((संरचना ext2_dx_countlimit *) entries)->गिनती = ext2fs_cpu_to_le16(मूल्य);
}

स्थिर अन्तःस्थ शून्य dx_set_limit(संरचना ext2_dx_entry *entries, अचिन्हित मूल्य)
{
	((संरचना ext2_dx_countlimit *) entries)->limit = ext2fs_cpu_to_le16(मूल्य);
}

स्थिर अन्तःस्थ अचिन्हित dx_मूल_limit(ext2_filsys fs, अचिन्हित infosize)
{
	अचिन्हित पूर्णाङ्क entry_space = fs->blocksize -
			ext2fs_dir_rec_len(1, 0) -
			ext2fs_dir_rec_len(2, 0) - infosize;

	यदि (ext2fs_has_feature_metadata_csum(fs->super))
		entry_space -= परिमाण(संरचना ext2_dx_tail);
	प्रत्यागम entry_space / परिमाण(संरचना ext2_dx_entry);
}

/*
 * This converts a one block unindexed directory to a 3 block indexed
 * directory, and adds the dentry to the indexed directory.  Returns 0 if the
 * index was not created; EAGAIN if it was; or an errcode_t on error.
 */
स्थिर errcode_t try_make_indexed_dir(ext2_filsys fs, ext2_ino_t dir,
				      ध्रुव वर्ण *name, ext2_ino_t ino,
				      पूर्णाङ्क flags)
{
	संरचना ext2_inode inode;
	वर्ण *buf0, *buf1, *शीर्ष;
	संरचना dx_मूल *मूल;
	संरचना ext2_dx_entry *entries;
	संरचना fake_dirent *fde;
	संरचना ext2_dir_entry *de, *de2;
	blk64_t pblk0, pblk1;
	अचिन्हित पूर्णाङ्क len;
	ध्रुव अचिन्हित पूर्णाङ्क blocksize = fs->blocksize;
	पूर्णाङ्क csum_size = 0;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	यदि (retval)
		प्रत्यागम retval;

	यदि (inode.i_size > fs->blocksize || (inode.i_flags & EXT2_INDEX_FL))
		प्रत्यागम 0;

	यदि (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = परिमाण(संरचना ext2_dir_entry_tail);

	retval = ext2fs_get_mem(blocksize, &buf0);
	यदि (retval)
		प्रत्यागम retval;

	retval = ext2fs_get_mem(blocksize, &buf1);
	यदि (retval)
		गच्छ out_buf0;

	retval = ext2fs_bmap2(fs, dir, &inode, रिक्त, 0, 0, रिक्त, &pblk0);
	यदि (retval)
		गच्छ out_buf1;

	retval = ext2fs_read_dir_block4(fs, pblk0, buf0, 0, dir);
	यदि (retval)
		गच्छ out_buf1;

	मूल = (संरचना dx_मूल *)buf0;
	यदि (!check_dx_मूल(fs, dir, मूल)) {
		retval = EXT2_ET_DIR_CORRUPTED;
		गच्छ out_buf1;
	}

	/* The 0th block becomes the root, move the dirents out */
	fde = &मूल->dotdot;
	de = ext2fs_next_entry(fs, (संरचना ext2_dir_entry *)fde);
	len = ((वर्ण *) मूल) + (blocksize - csum_size) - (वर्ण *) de;

	/* Allocate new block for the 0th block's dirents */
	retval = ext2fs_bmap2(fs, dir, &inode, रिक्त, BMAP_ALLOC | BMAP_ZERO, 1,
			      रिक्त, &pblk1);
	यदि (retval)
		गच्छ out_buf1;

	स_नकल(buf1, de, len);
	स_रखो(de, 0, len); /* wipe old data */

	de = (संरचना ext2_dir_entry *)buf1;
	शीर्ष = buf1 + len;
	यावत् ((वर्ण *)(de2 = ext2fs_next_entry(fs, de)) < शीर्ष) {
#यदि 0
		यदि (ext4_check_dir_entry(dir, रिक्त, de, bh2, buf1, len,
					(वर्ण *)de - buf1)) {
			retval = EXT2_ET_DIR_CORRUPTED;
			गच्छ out_buf1;
		}
#endif
		de = de2;
	}
	retval = ext2fs_set_rec_len(fs,
			buf1 + (blocksize - csum_size) - (वर्ण *) de, de);
	यदि (retval)
		गच्छ out_buf1;

	यदि (csum_size)
		ext2fs_initialize_dirent_tail(fs,
				EXT2_DIRENT_TAIL(buf1, fs->blocksize));

	/* Initialize the root; the dot dirents already exist */
	de = (संरचना ext2_dir_entry *) (&मूल->dotdot);
	retval = ext2fs_set_rec_len(fs, blocksize - ext2fs_dir_rec_len(2, 0),
				    de);
	स_रखो (&मूल->info, 0, परिमाण(मूल->info));
	मूल->info.info_length = परिमाण(मूल->info);
	यदि (ext4_hash_in_dirent(&inode))
		मूल->info.hash_version = EXT2_HASH_SIPHASH;
	अन्यथा
		मूल->info.hash_version = fs->super->s_def_hash_version;

	entries = मूल->entries;
	dx_set_block(entries, 1);
	dx_set_गिनती(entries, 1);
	dx_set_limit(entries, dx_मूल_limit(fs, परिमाण(मूल->info)));

	retval = ext2fs_write_dir_block4(fs, pblk1, buf1, 0, dir);
	यदि (retval)
		गच्छ out_buf1;

	retval = ext2fs_write_dir_block4(fs, pblk0, buf0, 0, dir);
	यदि (retval)
		गच्छ out_buf1;

	inode.i_flags |= EXT2_INDEX_FL;
	inode.i_size += fs->blocksize;
	retval = ext2fs_write_inode(fs, dir, &inode);
	यदि (retval)
		गच्छ out_buf1;

	retval = EAGAIN;
out_buf1:
	ext2fs_free_mem(&buf1);
out_buf0:
	ext2fs_free_mem(&buf0);
	प्रत्यागम retval;
}

/*
 * Note: the low 3 bits of the flags field are used as the directory
 * entry filetype.
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, ध्रुव वर्ण *name,
		      ext2_ino_t ino, पूर्णाङ्क flags)
{
	errcode_t		retval;
	संरचना link_struct	ls;
	संरचना ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (!(fs->flags & EXT2_FLAG_RW))
		प्रत्यागम EXT2_ET_RO_FILSYS;

retry:
	यदि ((retval = ext2fs_read_inode(fs, dir, &inode)) != 0)
		प्रत्यागम retval;

	यदि (inode.i_flags & EXT2_INDEX_FL)
		प्रत्यागम dx_link(fs, dir, &inode, name, ino, flags);

	ls.fs = fs;
	ls.name = name;
	ls.namelen = name ? म_माप(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	यदि ((flags & EXT2FS_LINK_APPEND) &&
	    !(inode.i_flags & EXT4_INLINE_DATA_FL)) {
		blk64_t		lblk, pblk = 0;
		संरचना dir_context ctx;

		lblk = (inode.i_size / fs->blocksize) - 1;
		retval = ext2fs_bmap2(fs, dir, &inode, रिक्त, 0, lblk,
				      रिक्त, &pblk);
		यदि (retval)
			प्रत्यागम retval;

		ctx.dir = dir;
		ctx.flags = DIRENT_FLAG_INCLUDE_EMPTY;
		ctx.func = link_proc;
		ctx.priv_data = &ls;
		ctx.errcode = 0;
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		यदि (retval)
			प्रत्यागम retval;

		ext2fs_process_dir_block(fs, &pblk, lblk, 0, 0, &ctx);
		retval = ctx.errcode;
		ext2fs_free_mem(&ctx.buf);
	} अन्यथा {
		retval = ext2fs_dir_iterate2(fs, dir,
					     DIRENT_FLAG_INCLUDE_EMPTY,
					     रिक्त, link_proc, &ls);
	}
	यदि (retval)
		प्रत्यागम retval;
	यदि (ls.err)
		प्रत्यागम ls.err;

	यदि (!ls.done) {
		यदि (!(flags & EXT2FS_LINK_EXPAND))
			प्रत्यागम EXT2_ET_DIR_NO_SPACE;

		यदि (ext2fs_has_feature_dir_index(fs->super)) {
			retval = try_make_indexed_dir(fs, dir, name, ino,
						      flags);
			यदि (retval == EAGAIN)
				गच्छ retry;
			यदि (retval)
				प्रत्यागम retval;
		}

		retval = ext2fs_expand_dir(fs, dir);
		यदि (retval)
			प्रत्यागम retval;
		गच्छ retry;
	}
	प्रत्यागम 0;
}

/* Does this directory have an htree index? */
पूर्णाङ्क ext2fs_dir_is_dx(ext2_filsys fs, ध्रुव संरचना ext2_inode *inode)
{
	प्रत्यागम ext2fs_has_feature_dir_index(fs->super) &&
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
शून्य ext2fs_inc_nlink(ext2_filsys fs, संरचना ext2_inode *inode)
{
	inode->i_links_गिनती++;

	यदि (ext2fs_dir_is_dx(fs, inode) &&
	    (inode->i_links_गिनती > EXT2_LINK_MAX || inode->i_links_गिनती == 2))
		inode->i_links_गिनती = 1;
}

/*
 * If a directory had nlink == 1, then we should let it be 1. This indicates
 * directory has >EXT2_LINK_MAX subdirs.
 */
शून्य ext2fs_dec_nlink(संरचना ext2_inode *inode)
{
	यदि (!S_ISDIR(inode->i_mode) || inode->i_links_गिनती > 2)
		inode->i_links_गिनती--;
}

पूर्णाङ्क ext2fs_dir_link_max(ext2_filsys fs, संरचना ext2_inode_large *inode)
{
	प्रत्यागम inode->i_links_गिनती >= EXT2_LINK_MAX &&
	       !(ext2fs_dir_is_dx(fs, EXT2_INODE(inode)) &&
		 ext2fs_has_feature_dir_nlink(fs->super));
}

पूर्णाङ्क ext2fs_dir_link_empty(संरचना ext2_inode *inode)
{
	प्रत्यागम inode->i_links_गिनती == 2 || inode->i_links_गिनती == 1;
}
