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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"
#समावेश "ext2fsP.h"

#घोषणा EXT2_DX_मूल_OFF 24

काष्ठा dx_frame {
	व्योम *buf;
	blk64_t pblock;
	काष्ठा ext2_dx_countlimit *शीर्ष_2;
	काष्ठा ext2_dx_entry *entries;
	काष्ठा ext2_dx_entry *at;
};

काष्ठा dx_lookup_info {
	स्थिर अक्षर *name;
	पूर्णांक namelen;
	पूर्णांक hash_alg;
	__u32 hash;
	अचिन्हित levels;
	काष्ठा dx_frame frames[EXT4_HTREE_LEVEL];
};

अटल errcode_t alloc_dx_frame(ext2_filsys fs, काष्ठा dx_frame *frame)
{
	वापस ext2fs_get_mem(fs->blocksize, &frame->buf);
}

अटल व्योम dx_release(काष्ठा dx_lookup_info *info)
{
	अचिन्हित level;

	क्रम (level = 0; level < info->levels; level++) {
		अगर (info->frames[level].buf == शून्य)
			अवरोध;
		ext2fs_free_mem(&(info->frames[level].buf));
	}
	info->levels = 0;
}

अटल व्योम dx_खोज_entry(काष्ठा dx_frame *frame, पूर्णांक गिनती, __u32 hash)
{
	काष्ठा ext2_dx_entry *p, *q, *m;

	p = frame->entries + 1;
	q = frame->entries + गिनती - 1;
	जबतक (p <= q) {
		m = p + (q - p) / 2;
		अगर (ext2fs_le32_to_cpu(m->hash) > hash)
			q = m - 1;
		अन्यथा
			p = m + 1;
	}
	frame->at = p - 1;
}

अटल errcode_t load_logical_dir_block(ext2_filsys fs, ext2_ino_t dir,
					काष्ठा ext2_inode *diri, blk64_t block,
					blk64_t *pblk, व्योम *buf)
{
	errcode_t errcode;
	पूर्णांक ret_flags;

	errcode = ext2fs_bmap2(fs, dir, diri, शून्य, 0, block, &ret_flags,
			       pblk);
	अगर (errcode)
		वापस errcode;
	अगर (ret_flags & BMAP_RET_UNINIT)
		वापस EXT2_ET_DIR_CORRUPTED;
	वापस ext2fs_read_dir_block4(fs, *pblk, buf, 0, dir);
}

अटल errcode_t dx_lookup(ext2_filsys fs, ext2_ino_t dir,
			   काष्ठा ext2_inode *diri, काष्ठा dx_lookup_info *info)
{
	काष्ठा ext2_dx_मूल_info *मूल;
	errcode_t errcode;
	पूर्णांक level = 0;
	पूर्णांक गिनती, limit;
	पूर्णांक hash_alg;
	पूर्णांक hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	__u32 minor_hash;
	काष्ठा dx_frame *frame;

	errcode = alloc_dx_frame(fs, &(info->frames[0]));
	अगर (errcode)
		वापस errcode;
	info->levels = 1;

	errcode = load_logical_dir_block(fs, dir, diri, 0,
					 &(info->frames[0].pblock),
					 info->frames[0].buf);
	अगर (errcode)
		जाओ out_err;
	मूल = (काष्ठा ext2_dx_मूल_info *) ((अक्षर *)info->frames[0].buf +
					     EXT2_DX_मूल_OFF);
	hash_alg = मूल->hash_version;
	अगर (hash_alg != EXT2_HASH_TEA && hash_alg != EXT2_HASH_HALF_MD4 &&
	    hash_alg != EXT2_HASH_LEGACY) {
		errcode = EXT2_ET_DIRHASH_UNSUPP;
		जाओ out_err;
	}
	अगर (hash_alg <= EXT2_HASH_TEA &&
	    fs->super->s_flags & EXT2_FLAGS_UNSIGNED_HASH)
		hash_alg += 3;
	अगर (मूल->indirect_levels >= ext2_dir_htree_level(fs)) {
		errcode = EXT2_ET_DIR_CORRUPTED;
		जाओ out_err;
	}
	info->hash_alg = hash_alg;

	errcode = ext2fs_dirhash2(hash_alg, info->name, info->namelen,
				  fs->encoding, hash_flags,
				  fs->super->s_hash_seed, &info->hash,
				  &minor_hash);
	अगर (errcode)
		जाओ out_err;

	क्रम (level = 0; level <= मूल->indirect_levels; level++) {
		frame = &(info->frames[level]);
		अगर (level > 0) {
			errcode = alloc_dx_frame(fs, frame);
			अगर (errcode)
				जाओ out_err;
			info->levels++;

			errcode = load_logical_dir_block(fs, dir, diri,
				ext2fs_le32_to_cpu(info->frames[level-1].at->block) & 0x0fffffff,
				&(frame->pblock), frame->buf);
			अगर (errcode)
				जाओ out_err;
		}
		errcode = ext2fs_get_dx_countlimit(fs, frame->buf,
						   &(frame->शीर्ष_2), शून्य);
		अगर (errcode)
			जाओ out_err;
		गिनती = ext2fs_le16_to_cpu(frame->शीर्ष_2->गिनती);
		limit = ext2fs_le16_to_cpu(frame->शीर्ष_2->limit);
		frame->entries = (काष्ठा ext2_dx_entry *)(frame->शीर्ष_2);
		अगर (!गिनती || गिनती > limit) {
			errcode = EXT2_ET_DIR_CORRUPTED;
			जाओ out_err;
		}

		dx_खोज_entry(frame, गिनती, info->hash);
	}
	वापस 0;
out_err:
	dx_release(info);
	वापस errcode;
}

काष्ठा link_struct  {
	ext2_filsys	fs;
	स्थिर अक्षर	*name;
	पूर्णांक		namelen;
	ext2_ino_t	inode;
	पूर्णांक		flags;
	पूर्णांक		done;
	अचिन्हित पूर्णांक	blocksize;
	errcode_t	err;
	काष्ठा ext2_super_block *sb;
};

अटल पूर्णांक link_proc(ext2_ino_t dir EXT2FS_ATTR((unused)),
		     पूर्णांक entru EXT2FS_ATTR((unused)),
		     काष्ठा ext2_dir_entry *dirent,
		     पूर्णांक	offset,
		     पूर्णांक	blocksize,
		     अक्षर	*buf,
		     व्योम	*priv_तथ्य)
{
	काष्ठा link_struct *ls = (काष्ठा link_struct *) priv_तथ्य;
	काष्ठा ext2_dir_entry *अग्र;
	अचिन्हित पूर्णांक rec_len, min_rec_len, curr_rec_len;
	पूर्णांक ret = 0;
	पूर्णांक csum_आकार = 0;

	अगर (ls->done)
		वापस DIRENT_ABORT;

	rec_len = EXT2_DIR_REC_LEN(ls->namelen);

	ls->err = ext2fs_get_rec_len(ls->fs, dirent, &curr_rec_len);
	अगर (ls->err)
		वापस DIRENT_ABORT;

	अगर (ext2fs_has_feature_metadata_csum(ls->fs->super))
		csum_आकार = माप(काष्ठा ext2_dir_entry_पुच्छ);
	/*
	 * See if the following directory entry (if any) is unused;
	 * if so, absorb it into this one.
	 */
	अग्र = (काष्ठा ext2_dir_entry *) (buf + offset + curr_rec_len);
	अगर ((offset + (पूर्णांक) curr_rec_len < blocksize - (8 + csum_आकार)) &&
	    (अग्र->inode == 0) &&
	    (offset + (पूर्णांक) curr_rec_len + (पूर्णांक) अग्र->rec_len <= blocksize)) {
		curr_rec_len += अग्र->rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, curr_rec_len, dirent);
		अगर (ls->err)
			वापस DIRENT_ABORT;
		ret = DIRENT_CHANGED;
	}

	/*
	 * If the directory entry is used, see if we can split the
	 * directory entry to make room for the new name.  If so,
	 * truncate it and return.
	 */
	अगर (dirent->inode) {
		min_rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(dirent));
		अगर (curr_rec_len < (min_rec_len + rec_len))
			वापस ret;
		rec_len = curr_rec_len - min_rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, min_rec_len, dirent);
		अगर (ls->err)
			वापस DIRENT_ABORT;
		अग्र = (काष्ठा ext2_dir_entry *) (buf + offset +
						  dirent->rec_len);
		अग्र->inode = 0;
		ext2fs_dirent_set_name_len(अग्र, 0);
		ext2fs_dirent_set_file_type(अग्र, 0);
		ls->err = ext2fs_set_rec_len(ls->fs, rec_len, अग्र);
		अगर (ls->err)
			वापस DIRENT_ABORT;
		वापस DIRENT_CHANGED;
	}

	/*
	 * If we get this far, then the directory entry is not used.
	 * See if we can fit the request entry in.  If so, do it.
	 */
	अगर (curr_rec_len < rec_len)
		वापस ret;
	dirent->inode = ls->inode;
	ext2fs_dirent_set_name_len(dirent, ls->namelen);
	म_नकलन(dirent->name, ls->name, ls->namelen);
	अगर (ext2fs_has_feature_filetype(ls->sb))
		ext2fs_dirent_set_file_type(dirent,
					    ls->flags & EXT2FS_LINK_FT_MASK);

	ls->done++;
	वापस DIRENT_ABORT|DIRENT_CHANGED;
}

अटल errcode_t add_dirent_to_buf(ext2_filsys fs, e2_blkcnt_t blockcnt,
				   अक्षर *buf, ext2_ino_t dir,
				   काष्ठा ext2_inode *diri, स्थिर अक्षर *name,
				   ext2_ino_t ino, पूर्णांक flags, blk64_t *pblkp)
{
	काष्ठा dir_context ctx;
	काष्ठा link_struct ls;
	errcode_t retval;

	retval = load_logical_dir_block(fs, dir, diri, blockcnt, pblkp, buf);
	अगर (retval)
		वापस retval;
	ctx.errcode = 0;
	ctx.func = link_proc;
	ctx.dir = dir;
	ctx.flags = DIRENT_FLAG_INCLUDE_EMPTY;
	ctx.buf = buf;
	ctx.priv_तथ्य = &ls;

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
	अगर (ctx.errcode)
		वापस ctx.errcode;
	अगर (ls.err)
		वापस ls.err;
	अगर (!ls.done)
		वापस EXT2_ET_DIR_NO_SPACE;
	वापस 0;
}

काष्ठा dx_hash_map {
	__u32 hash;
	पूर्णांक आकार;
	पूर्णांक off;
};

अटल EXT2_QSORT_TYPE dx_hash_map_cmp(स्थिर व्योम *ap, स्थिर व्योम *bp)
{
	स्थिर काष्ठा dx_hash_map *a = ap, *b = bp;

	अगर (a->hash < b->hash)
		वापस -1;
	अगर (a->hash > b->hash)
		वापस 1;
	वापस 0;
}

अटल errcode_t dx_move_dirents(ext2_filsys fs, काष्ठा dx_hash_map *map,
				 पूर्णांक गिनती, व्योम *from, व्योम *to)
{
	काष्ठा ext2_dir_entry *de;
	पूर्णांक i;
	पूर्णांक rec_len = 0;
	errcode_t retval;
	पूर्णांक csum_आकार = 0;
	व्योम *base = to;

	अगर (ext2fs_has_feature_metadata_csum(fs->super))
		csum_आकार = माप(काष्ठा ext2_dir_entry_पुच्छ);

	क्रम (i = 0; i < गिनती; i++) {
		de = (काष्ठा ext2_dir_entry *) ((अक्षर *)from + map[i].off);
		rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(de));
		स_नकल(to, de, rec_len);
		retval = ext2fs_set_rec_len(fs, rec_len, to);
		अगर (retval)
			वापस retval;
		to = (अक्षर *)to + rec_len;
	}
	/*
	 * Update rec_len of the last dir entry to stretch to the end of block
	 */
	to = (अक्षर *)to - rec_len;
	rec_len = fs->blocksize - ((अक्षर *)to - (अक्षर *)base) - csum_आकार;
	retval = ext2fs_set_rec_len(fs, rec_len, to);
	अगर (retval)
		वापस retval;
	अगर (csum_आकार)
		ext2fs_initialize_dirent_पुच्छ(fs,
				EXT2_DIRENT_पुच्छ(base, fs->blocksize));
	वापस 0;
}

अटल errcode_t dx_जोड़ो_entry(ext2_filsys fs, ext2_ino_t dir,
				 काष्ठा dx_lookup_info *info, पूर्णांक level,
				 __u32 hash, blk64_t lblk)
{
	पूर्णांक pcount;
	काष्ठा ext2_dx_entry *शीर्ष, *new;

	pcount = ext2fs_le16_to_cpu(info->frames[level].शीर्ष_2->गिनती);
	शीर्ष = info->frames[level].entries + pcount;
	new = info->frames[level].at + 1;
	स_हटाओ(new + 1, new, (अक्षर *)शीर्ष - (अक्षर *)new);
	new->hash = ext2fs_cpu_to_le32(hash);
	new->block = ext2fs_cpu_to_le32(lblk);
	info->frames[level].शीर्ष_2->गिनती = ext2fs_cpu_to_le16(pcount + 1);
	वापस ext2fs_write_dir_block4(fs, info->frames[level].pblock,
				       info->frames[level].buf, 0, dir);
}

अटल errcode_t dx_split_leaf(ext2_filsys fs, ext2_ino_t dir,
			       काष्ठा ext2_inode *diri,
			       काष्ठा dx_lookup_info *info, व्योम *buf,
			       blk64_t leaf_pblk, blk64_t new_lblk,
			       blk64_t new_pblk)
{
	पूर्णांक hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	काष्ठा ext2_dir_entry *de;
	व्योम *buf2;
	errcode_t retval = 0;
	अचिन्हित पूर्णांक rec_len;
	अचिन्हित पूर्णांक offset, move_आकार;
	पूर्णांक i, गिनती = 0;
	काष्ठा dx_hash_map *map;
	पूर्णांक continued;
	__u32 minor_hash;

	retval = ext2fs_get_mem(fs->blocksize, &buf2);
	अगर (retval)
		वापस retval;
	retval = ext2fs_get_array(fs->blocksize / 12,
				  माप(काष्ठा dx_hash_map), &map);
	अगर (retval) {
		ext2fs_free_mem(&buf2);
		वापस retval;
	}
	क्रम (offset = 0; offset < fs->blocksize; offset += rec_len) {
		de = (काष्ठा ext2_dir_entry *) ((अक्षर *)buf + offset);
		retval = ext2fs_get_rec_len(fs, de, &rec_len);
		अगर (retval)
			जाओ out;
		अगर (ext2fs_dirent_name_len(de) > 0 && de->inode) {
			map[गिनती].off = offset;
			map[गिनती].आकार = rec_len;
			retval = ext2fs_dirhash2(info->hash_alg, de->name,
					ext2fs_dirent_name_len(de),
					fs->encoding, hash_flags,
					fs->super->s_hash_seed,
					&(map[गिनती].hash),
					&minor_hash);
			अगर (retval)
				जाओ out;
			गिनती++;
		}
	}
	क्विक(map, गिनती, माप(काष्ठा dx_hash_map), dx_hash_map_cmp);
	move_आकार = 0;
	/* Find place to split block */
	क्रम (i = गिनती - 1; i >= 0; i--) {
		अगर (move_आकार + map[i].आकार / 2 > fs->blocksize / 2)
			अवरोध;
		move_आकार += map[i].आकार;
	}
	/* Let i be the first entry to move */
	i++;
	/* Move selected directory entries to new block */
	retval = dx_move_dirents(fs, map + i, गिनती - i, buf, buf2);
	अगर (retval)
		जाओ out;
	retval = ext2fs_write_dir_block4(fs, new_pblk, buf2, 0, dir);
	अगर (retval)
		जाओ out;
	/* Repack remaining entries in the old block */
	retval = dx_move_dirents(fs, map, i, buf, buf2);
	अगर (retval)
		जाओ out;
	retval = ext2fs_write_dir_block4(fs, leaf_pblk, buf2, 0, dir);
	अगर (retval)
		जाओ out;
	/* Update parent node */
	continued = map[i].hash == map[i-1].hash;
	retval = dx_जोड़ो_entry(fs, dir, info, info->levels - 1,
				 map[i].hash + continued, new_lblk);
out:
	ext2fs_free_mem(&buf2);
	ext2fs_free_mem(&map);
	वापस retval;
}

अटल errcode_t dx_grow_वृक्ष(ext2_filsys fs, ext2_ino_t dir,
			      काष्ठा ext2_inode *diri,
			      काष्ठा dx_lookup_info *info, व्योम *buf,
			      blk64_t leaf_pblk)
{
	पूर्णांक i;
	errcode_t retval;
	ext2_off64_t आकार = EXT2_I_आकार(diri);
	blk64_t lblk, pblk;
	काष्ठा ext2_dir_entry *de;
	काष्ठा ext2_dx_countlimit *शीर्ष_2;
	पूर्णांक csum_आकार = 0;
	पूर्णांक गिनती;

	अगर (ext2fs_has_feature_metadata_csum(fs->super))
		csum_आकार = माप(काष्ठा ext2_dx_पुच्छ);

	/* Find level which can accommodate new child */
	क्रम (i = info->levels - 1; i >= 0; i--)
		अगर (ext2fs_le16_to_cpu(info->frames[i].शीर्ष_2->गिनती) <
		    ext2fs_le16_to_cpu(info->frames[i].शीर्ष_2->limit))
			अवरोध;
	/* Need to grow tree depth? */
	अगर (i < 0 && info->levels >= ext2_dir_htree_level(fs))
		वापस EXT2_ET_DIR_NO_SPACE;
	lblk = आकार / fs->blocksize;
	आकार += fs->blocksize;
	retval = ext2fs_inode_आकार_set(fs, diri, आकार);
	अगर (retval)
		वापस retval;
	retval = ext2fs_fallocate(fs,
			EXT2_FALLOCATE_FORCE_INIT | EXT2_FALLOCATE_ZERO_BLOCKS,
			dir, diri, 0, lblk, 1);
	अगर (retval)
		वापस retval;
	retval = ext2fs_write_inode(fs, dir, diri);
	अगर (retval)
		वापस retval;
	retval = ext2fs_bmap2(fs, dir, diri, शून्य, 0, lblk, शून्य, &pblk);
	अगर (retval)
		वापस retval;
	/* Only leaf addition needed? */
	अगर (i == (पूर्णांक)info->levels - 1)
		वापस dx_split_leaf(fs, dir, diri, info, buf, leaf_pblk,
				     lblk, pblk);

	de = buf;
	de->inode = 0;
	ext2fs_dirent_set_name_len(de, 0);
	ext2fs_dirent_set_file_type(de, 0);
	retval = ext2fs_set_rec_len(fs, fs->blocksize, de);
	अगर (retval)
		वापस retval;
	शीर्ष_2 = (काष्ठा ext2_dx_countlimit *) ((अक्षर *)buf + 8);
	गिनती = ext2fs_le16_to_cpu(info->frames[i+1].शीर्ष_2->गिनती);
	/* Growing tree depth? */
	अगर (i < 0) {
		काष्ठा ext2_dx_मूल_info *मूल;

		स_नकल(शीर्ष_2, info->frames[0].entries,
		       गिनती * माप(काष्ठा ext2_dx_entry));
		शीर्ष_2->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_आकार)) /
				माप(काष्ठा ext2_dx_entry));
		/* head->count gets set by memcpy above to correct value */

		/* Now update tree root */
		info->frames[0].शीर्ष_2->गिनती = ext2fs_cpu_to_le16(1);
		info->frames[0].entries[0].block = ext2fs_cpu_to_le32(lblk);
		मूल = (काष्ठा ext2_dx_मूल_info *)
			((अक्षर *)info->frames[0].buf + EXT2_DX_मूल_OFF);
		मूल->indirect_levels++;
	} अन्यथा {
		/* Splitting internal node in two */
		पूर्णांक count1 = गिनती / 2;
		पूर्णांक count2 = गिनती - count1;
		__u32 split_hash = ext2fs_le32_to_cpu(info->frames[i+1].entries[count1].hash);

		स_नकल(शीर्ष_2, info->frames[i+1].entries + count1,
		       count2 * माप(काष्ठा ext2_dx_entry));
		शीर्ष_2->गिनती = ext2fs_cpu_to_le16(count2);
		शीर्ष_2->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_आकार)) /
				माप(काष्ठा ext2_dx_entry));
		info->frames[i+1].शीर्ष_2->गिनती = ext2fs_cpu_to_le16(count1);

		/* Update parent node */
		retval = dx_जोड़ो_entry(fs, dir, info, i, split_hash, lblk);
		अगर (retval)
			वापस retval;

	}
	/* Writeout split block / updated root */
	retval = ext2fs_write_dir_block4(fs, info->frames[i+1].pblock,
					 info->frames[i+1].buf, 0, dir);
	अगर (retval)
		वापस retval;
	/* Writeout new tree block */
	retval = ext2fs_write_dir_block4(fs, pblk, buf, 0, dir);
	अगर (retval)
		वापस retval;
	वापस 0;
}

अटल errcode_t dx_link(ext2_filsys fs, ext2_ino_t dir,
			 काष्ठा ext2_inode *diri, स्थिर अक्षर *name,
			 ext2_ino_t ino, पूर्णांक flags)
{
	काष्ठा dx_lookup_info dx_info;
	errcode_t retval;
	व्योम *blockbuf;
	अचिन्हित restart = 0;
	blk64_t leaf_pblk;

	retval = ext2fs_get_mem(fs->blocksize, &blockbuf);
	अगर (retval)
		वापस retval;

	dx_info.name = name;
	dx_info.namelen = म_माप(name);
again:
	retval = dx_lookup(fs, dir, diri, &dx_info);
	अगर (retval)
		जाओ free_buf;

	retval = add_dirent_to_buf(fs,
		ext2fs_le32_to_cpu(dx_info.frames[dx_info.levels-1].at->block) & 0x0fffffff,
		blockbuf, dir, diri, name, ino, flags, &leaf_pblk);
	/*
	 * Success or error other than ENOSPC...? We are done. We may need upto
	 * two tries to add entry. One to split htree node and another to add
	 * new leaf block.
	 */
	अगर (restart >= dx_info.levels || retval != EXT2_ET_DIR_NO_SPACE)
		जाओ free_frames;
	retval = dx_grow_वृक्ष(fs, dir, diri, &dx_info, blockbuf, leaf_pblk);
	अगर (retval)
		जाओ free_frames;
	/* Restart everything now that the tree is larger */
	restart++;
	dx_release(&dx_info);
	जाओ again;
free_frames:
	dx_release(&dx_info);
free_buf:
	ext2fs_free_mem(&blockbuf);
	वापस retval;
}

काष्ठा fake_dirent
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

काष्ठा dx_मूल
{
	काष्ठा fake_dirent dot;
	अक्षर dot_name[4];
	काष्ठा fake_dirent dotdot;
	अक्षर dotdot_name[4];
	काष्ठा ext2_dx_मूल_info info;
	काष्ठा ext2_dx_entry entries[];
};

अटल पूर्णांक check_dx_मूल(ext2_filsys fs, ext2_ino_t ino, काष्ठा dx_मूल *मूल)
{
	काष्ठा fake_dirent *fde;
	अक्षर *error_संदेश;
	अचिन्हित पूर्णांक rlen;
	अक्षर *blockend = (अक्षर *)मूल + fs->blocksize;

	fde = &मूल->dot;
	अगर (fde->name_len != 1) {
		error_संदेश = "invalid name_len for '.'";
		जाओ corrupted;
	}
	अगर (म_भेदन(मूल->dot_name, ".", fde->name_len)) {
		error_संदेश = "invalid name for '.'";
		जाओ corrupted;
	}

	ext2fs_get_rec_len(fs, (काष्ठा ext2_dir_entry *)fde, &rlen);
	अगर ((अक्षर *)fde + rlen >= blockend) {
		error_संदेश = "invalid rec_len for '.'";
		जाओ corrupted;
	}

	fde = &मूल->dotdot;
	अगर (fde->name_len != 2) {
		error_संदेश = "invalid name_len for '..'";
		जाओ corrupted;
	}
	अगर (म_भेदन(मूल->dotdot_name, "..", fde->name_len)) {
		error_संदेश = "invalid name for '..'";
		जाओ corrupted;
	}
	ext2fs_get_rec_len(fs, (काष्ठा ext2_dir_entry *)fde, &rlen);
	अगर ((अक्षर *)fde + rlen >= blockend) {
		error_संदेश = "invalid rec_len for '..'";
		जाओ corrupted;
	}

	वापस 1;

corrupted:
	ख_लिखो(मानक_त्रुटि, "Corrupt dir %u: %s, running e2fsck is recommended\n",
			 ino, error_संदेश);
	वापस 0;
}

अटल अंतरभूत काष्ठा ext2_dir_entry *
ext2fs_अग्र_entry(ext2_filsys fs, काष्ठा ext2_dir_entry *p)
{
	अचिन्हित पूर्णांक rlen;

	ext2fs_get_rec_len(fs, p, &rlen);
	वापस (काष्ठा ext2_dir_entry *)((अक्षर *)p + rlen);
}

अटल अंतरभूत व्योम dx_set_block(काष्ठा ext2_dx_entry *entry, अचिन्हित मूल्य)
{
	entry->block = ext2fs_cpu_to_le32(मूल्य);
}

अटल अंतरभूत व्योम dx_set_गिनती(काष्ठा ext2_dx_entry *entries, अचिन्हित मूल्य)
{
	((काष्ठा ext2_dx_countlimit *) entries)->गिनती = ext2fs_cpu_to_le16(मूल्य);
}

अटल अंतरभूत व्योम dx_set_limit(काष्ठा ext2_dx_entry *entries, अचिन्हित मूल्य)
{
	((काष्ठा ext2_dx_countlimit *) entries)->limit = ext2fs_cpu_to_le16(मूल्य);
}

अटल अंतरभूत अचिन्हित dx_मूल_limit(ext2_filsys fs, अचिन्हित infosize)
{
	अचिन्हित पूर्णांक entry_space = fs->blocksize -
			ext2fs_dir_rec_len(1, 0) -
			ext2fs_dir_rec_len(2, 0) - infosize;

	अगर (ext2fs_has_feature_metadata_csum(fs->super))
		entry_space -= माप(काष्ठा ext2_dx_पुच्छ);
	वापस entry_space / माप(काष्ठा ext2_dx_entry);
}

/*
 * This converts a one block unindexed directory to a 3 block indexed
 * directory, and adds the dentry to the indexed directory.  Returns 0 if the
 * index was not created; EAGAIN if it was; or an errcode_t on error.
 */
अटल errcode_t try_make_indexed_dir(ext2_filsys fs, ext2_ino_t dir,
				      स्थिर अक्षर *name, ext2_ino_t ino,
				      पूर्णांक flags)
{
	काष्ठा ext2_inode inode;
	अक्षर *buf0, *buf1, *शीर्ष;
	काष्ठा dx_मूल *मूल;
	काष्ठा ext2_dx_entry *entries;
	काष्ठा fake_dirent *fde;
	काष्ठा ext2_dir_entry *de, *de2;
	blk64_t pblk0, pblk1;
	अचिन्हित पूर्णांक len;
	स्थिर अचिन्हित पूर्णांक blocksize = fs->blocksize;
	पूर्णांक csum_आकार = 0;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	अगर (retval)
		वापस retval;

	अगर (inode.i_आकार > fs->blocksize || (inode.i_flags & EXT2_सूचकांक_FL))
		वापस 0;

	अगर (ext2fs_has_feature_metadata_csum(fs->super))
		csum_आकार = माप(काष्ठा ext2_dir_entry_पुच्छ);

	retval = ext2fs_get_mem(blocksize, &buf0);
	अगर (retval)
		वापस retval;

	retval = ext2fs_get_mem(blocksize, &buf1);
	अगर (retval)
		जाओ out_buf0;

	retval = ext2fs_bmap2(fs, dir, &inode, शून्य, 0, 0, शून्य, &pblk0);
	अगर (retval)
		जाओ out_buf1;

	retval = ext2fs_read_dir_block4(fs, pblk0, buf0, 0, dir);
	अगर (retval)
		जाओ out_buf1;

	मूल = (काष्ठा dx_मूल *)buf0;
	अगर (!check_dx_मूल(fs, dir, मूल)) {
		retval = EXT2_ET_DIR_CORRUPTED;
		जाओ out_buf1;
	}

	/* The 0th block becomes the root, move the dirents out */
	fde = &मूल->dotdot;
	de = ext2fs_अग्र_entry(fs, (काष्ठा ext2_dir_entry *)fde);
	len = ((अक्षर *) मूल) + (blocksize - csum_आकार) - (अक्षर *) de;

	/* Allocate new block for the 0th block's dirents */
	retval = ext2fs_bmap2(fs, dir, &inode, शून्य, BMAP_ALLOC | BMAP_ZERO, 1,
			      शून्य, &pblk1);
	अगर (retval)
		जाओ out_buf1;

	स_नकल(buf1, de, len);
	स_रखो(de, 0, len); /* wipe old data */

	de = (काष्ठा ext2_dir_entry *)buf1;
	शीर्ष = buf1 + len;
	जबतक ((अक्षर *)(de2 = ext2fs_अग्र_entry(fs, de)) < शीर्ष) {
#अगर 0
		अगर (ext4_check_dir_entry(dir, शून्य, de, bh2, buf1, len,
					(अक्षर *)de - buf1)) {
			retval = EXT2_ET_DIR_CORRUPTED;
			जाओ out_buf1;
		}
#पूर्ण_अगर
		de = de2;
	}
	retval = ext2fs_set_rec_len(fs,
			buf1 + (blocksize - csum_आकार) - (अक्षर *) de, de);
	अगर (retval)
		जाओ out_buf1;

	अगर (csum_आकार)
		ext2fs_initialize_dirent_पुच्छ(fs,
				EXT2_DIRENT_पुच्छ(buf1, fs->blocksize));

	/* Initialize the root; the dot dirents already exist */
	de = (काष्ठा ext2_dir_entry *) (&मूल->dotdot);
	retval = ext2fs_set_rec_len(fs, blocksize - ext2fs_dir_rec_len(2, 0),
				    de);
	स_रखो (&मूल->info, 0, माप(मूल->info));
	मूल->info.info_लंबाई = माप(मूल->info);
	अगर (ext4_hash_in_dirent(&inode))
		मूल->info.hash_version = EXT2_HASH_SIPHASH;
	अन्यथा
		मूल->info.hash_version = fs->super->s_def_hash_version;

	entries = मूल->entries;
	dx_set_block(entries, 1);
	dx_set_गिनती(entries, 1);
	dx_set_limit(entries, dx_मूल_limit(fs, माप(मूल->info)));

	retval = ext2fs_write_dir_block4(fs, pblk1, buf1, 0, dir);
	अगर (retval)
		जाओ out_buf1;

	retval = ext2fs_write_dir_block4(fs, pblk0, buf0, 0, dir);
	अगर (retval)
		जाओ out_buf1;

	inode.i_flags |= EXT2_सूचकांक_FL;
	inode.i_आकार += fs->blocksize;
	retval = ext2fs_write_inode(fs, dir, &inode);
	अगर (retval)
		जाओ out_buf1;

	retval = EAGAIN;
out_buf1:
	ext2fs_free_mem(&buf1);
out_buf0:
	ext2fs_free_mem(&buf0);
	वापस retval;
}

/*
 * Note: the low 3 bits of the flags field are used as the directory
 * entry filetype.
 */
#अगर_घोषित __TURBOC__
 #आशय argsused
#पूर्ण_अगर
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, स्थिर अक्षर *name,
		      ext2_ino_t ino, पूर्णांक flags)
{
	errcode_t		retval;
	काष्ठा link_struct	ls;
	काष्ठा ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	अगर (!(fs->flags & EXT2_FLAG_RW))
		वापस EXT2_ET_RO_FILSYS;

retry:
	अगर ((retval = ext2fs_read_inode(fs, dir, &inode)) != 0)
		वापस retval;

	अगर (inode.i_flags & EXT2_सूचकांक_FL)
		वापस dx_link(fs, dir, &inode, name, ino, flags);

	ls.fs = fs;
	ls.name = name;
	ls.namelen = name ? म_माप(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	अगर ((flags & EXT2FS_LINK_APPEND) &&
	    !(inode.i_flags & EXT4_INLINE_तथ्य_FL)) {
		blk64_t		lblk, pblk = 0;
		काष्ठा dir_context ctx;

		lblk = (inode.i_आकार / fs->blocksize) - 1;
		retval = ext2fs_bmap2(fs, dir, &inode, शून्य, 0, lblk,
				      शून्य, &pblk);
		अगर (retval)
			वापस retval;

		ctx.dir = dir;
		ctx.flags = DIRENT_FLAG_INCLUDE_EMPTY;
		ctx.func = link_proc;
		ctx.priv_तथ्य = &ls;
		ctx.errcode = 0;
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		अगर (retval)
			वापस retval;

		ext2fs_process_dir_block(fs, &pblk, lblk, 0, 0, &ctx);
		retval = ctx.errcode;
		ext2fs_free_mem(&ctx.buf);
	} अन्यथा {
		retval = ext2fs_dir_iterate2(fs, dir,
					     DIRENT_FLAG_INCLUDE_EMPTY,
					     शून्य, link_proc, &ls);
	}
	अगर (retval)
		वापस retval;
	अगर (ls.err)
		वापस ls.err;

	अगर (!ls.done) {
		अगर (!(flags & EXT2FS_LINK_EXPAND))
			वापस EXT2_ET_DIR_NO_SPACE;

		अगर (ext2fs_has_feature_dir_सूचकांक(fs->super)) {
			retval = try_make_indexed_dir(fs, dir, name, ino,
						      flags);
			अगर (retval == EAGAIN)
				जाओ retry;
			अगर (retval)
				वापस retval;
		}

		retval = ext2fs_expand_dir(fs, dir);
		अगर (retval)
			वापस retval;
		जाओ retry;
	}
	वापस 0;
}

/* Does this directory have an htree index? */
पूर्णांक ext2fs_dir_is_dx(ext2_filsys fs, स्थिर काष्ठा ext2_inode *inode)
{
	वापस ext2fs_has_feature_dir_सूचकांक(fs->super) &&
		S_ISDIR(inode->i_mode) && (inode->i_flags & EXT2_सूचकांक_FL);
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
व्योम ext2fs_inc_nlink(ext2_filsys fs, काष्ठा ext2_inode *inode)
{
	inode->i_links_गिनती++;

	अगर (ext2fs_dir_is_dx(fs, inode) &&
	    (inode->i_links_गिनती > EXT2_LINK_MAX || inode->i_links_गिनती == 2))
		inode->i_links_गिनती = 1;
}

/*
 * If a directory had nlink == 1, then we should let it be 1. This indicates
 * directory has >EXT2_LINK_MAX subdirs.
 */
व्योम ext2fs_dec_nlink(काष्ठा ext2_inode *inode)
{
	अगर (!S_ISDIR(inode->i_mode) || inode->i_links_गिनती > 2)
		inode->i_links_गिनती--;
}

पूर्णांक ext2fs_dir_link_max(ext2_filsys fs, काष्ठा ext2_inode_large *inode)
{
	वापस inode->i_links_गिनती >= EXT2_LINK_MAX &&
	       !(ext2fs_dir_is_dx(fs, EXT2_INODE(inode)) &&
		 ext2fs_has_feature_dir_nlink(fs->super));
}

पूर्णांक ext2fs_dir_link_empty(काष्ठा ext2_inode *inode)
{
	वापस inode->i_links_गिनती == 2 || inode->i_links_गिनती == 1;
}
