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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_DX_ਜੜ੍ਹ_OFF 24

ਬਣਤਰ dx_ਚੌਖਟਾ {
	ਖਾਲੀ *buf;
	blk64_t pblock;
	ਬਣਤਰ ext2_dx_countlimit *ਸਿਰ;
	ਬਣਤਰ ext2_dx_ਇੰਦਰਾਜ *entries;
	ਬਣਤਰ ext2_dx_ਇੰਦਰਾਜ *at;
};

ਬਣਤਰ dx_lookup_info {
	ਅਚਲ ਅੱਖਰ *name;
	ਪੂਰਨ namelen;
	ਪੂਰਨ hash_alg;
	__u32 hash;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੱਧਰ;
	ਬਣਤਰ dx_ਚੌਖਟਾ ਚੌਖਟੇ[EXT4_HTREE_LEVEL];
};

ਸਥਿਰ errcode_t alloc_dx_ਚੌਖਟਾ(ext2_filsys fs, ਬਣਤਰ dx_ਚੌਖਟਾ *ਚੌਖਟਾ)
{
	ਵਾਪਸ ext2fs_get_mem(fs->blocksize, &ਚੌਖਟਾ->buf);
}

ਸਥਿਰ ਖਾਲੀ dx_release(ਬਣਤਰ dx_lookup_info *info)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ level;

	ਲਈ (level = 0; level < info->ਪੱਧਰ; level++) {
		ਜੇ (info->ਚੌਖਟੇ[level].buf == ਨਿਰਾ)
			ਰੋਕੋ;
		ext2fs_free_mem(&(info->ਚੌਖਟੇ[level].buf));
	}
	info->ਪੱਧਰ = 0;
}

ਸਥਿਰ ਖਾਲੀ dx_ਖੋਜ_ਇੰਦਰਾਜ(ਬਣਤਰ dx_ਚੌਖਟਾ *ਚੌਖਟਾ, ਪੂਰਨ ਗਿਣਤੀ_2, __u32 hash)
{
	ਬਣਤਰ ext2_dx_ਇੰਦਰਾਜ *p, *q, *m;

	p = ਚੌਖਟਾ->entries + 1;
	q = ਚੌਖਟਾ->entries + ਗਿਣਤੀ_2 - 1;
	ਜਦੋਂ (p <= q) {
		m = p + (q - p) / 2;
		ਜੇ (ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(m->hash) > hash)
			q = m - 1;
		ਨਹੀਂਤਾਂ
			p = m + 1;
	}
	ਚੌਖਟਾ->at = p - 1;
}

ਸਥਿਰ errcode_t ਲੱਦੋ_logical_dir_ਖੰਡ(ext2_filsys fs, ext2_ino_t dir,
					ਬਣਤਰ ext2_inode *diri, blk64_t ਖੰਡ,
					blk64_t *pblk, ਖਾਲੀ *buf)
{
	errcode_t errcode;
	ਪੂਰਨ ret_flags;

	errcode = ext2fs_bmap2(fs, dir, diri, ਨਿਰਾ, 0, ਖੰਡ, &ret_flags,
			       pblk);
	ਜੇ (errcode)
		ਵਾਪਸ errcode;
	ਜੇ (ret_flags & BMAP_RET_UNINIT)
		ਵਾਪਸ EXT2_ET_DIR_CORRUPTED;
	ਵਾਪਸ ext2fs_read_dir_block4(fs, *pblk, buf, 0, dir);
}

ਸਥਿਰ errcode_t dx_lookup(ext2_filsys fs, ext2_ino_t dir,
			   ਬਣਤਰ ext2_inode *diri, ਬਣਤਰ dx_lookup_info *info)
{
	ਬਣਤਰ ext2_dx_ਜੜ੍ਹ_info *ਜੜ੍ਹ;
	errcode_t errcode;
	ਪੂਰਨ level = 0;
	ਪੂਰਨ ਗਿਣਤੀ_2, ਹੱਦਬੰਦੀ;
	ਪੂਰਨ hash_alg;
	ਪੂਰਨ hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	__u32 minor_hash;
	ਬਣਤਰ dx_ਚੌਖਟਾ *ਚੌਖਟਾ;

	errcode = alloc_dx_ਚੌਖਟਾ(fs, &(info->ਚੌਖਟੇ[0]));
	ਜੇ (errcode)
		ਵਾਪਸ errcode;
	info->ਪੱਧਰ = 1;

	errcode = ਲੱਦੋ_logical_dir_ਖੰਡ(fs, dir, diri, 0,
					 &(info->ਚੌਖਟੇ[0].pblock),
					 info->ਚੌਖਟੇ[0].buf);
	ਜੇ (errcode)
		ਜਾਓ ਬਾਹਰਲਾ_err;
	ਜੜ੍ਹ = (ਬਣਤਰ ext2_dx_ਜੜ੍ਹ_info *) ((ਅੱਖਰ *)info->ਚੌਖਟੇ[0].buf +
					     EXT2_DX_ਜੜ੍ਹ_OFF);
	hash_alg = ਜੜ੍ਹ->hash_ਸੰਸਕਰਣ;
	ਜੇ (hash_alg != EXT2_HASH_TEA && hash_alg != EXT2_HASH_HALF_MD4 &&
	    hash_alg != EXT2_HASH_LEGACY) {
		errcode = EXT2_ET_DIRHASH_UNSUPP;
		ਜਾਓ ਬਾਹਰਲਾ_err;
	}
	ਜੇ (hash_alg <= EXT2_HASH_TEA &&
	    fs->super->s_flags & EXT2_FLAGS_UNSIGNED_HASH)
		hash_alg += 3;
	ਜੇ (ਜੜ੍ਹ->ਅਸਿੱਧਾ_ਪੱਧਰ >= ext2_dir_htree_level(fs)) {
		errcode = EXT2_ET_DIR_CORRUPTED;
		ਜਾਓ ਬਾਹਰਲਾ_err;
	}
	info->hash_alg = hash_alg;

	errcode = ext2fs_dirhash2(hash_alg, info->name, info->namelen,
				  fs->encoding, hash_flags,
				  fs->super->s_hash_ਬੀਜ, &info->hash,
				  &minor_hash);
	ਜੇ (errcode)
		ਜਾਓ ਬਾਹਰਲਾ_err;

	ਲਈ (level = 0; level <= ਜੜ੍ਹ->ਅਸਿੱਧਾ_ਪੱਧਰ; level++) {
		ਚੌਖਟਾ = &(info->ਚੌਖਟੇ[level]);
		ਜੇ (level > 0) {
			errcode = alloc_dx_ਚੌਖਟਾ(fs, ਚੌਖਟਾ);
			ਜੇ (errcode)
				ਜਾਓ ਬਾਹਰਲਾ_err;
			info->ਪੱਧਰ++;

			errcode = ਲੱਦੋ_logical_dir_ਖੰਡ(fs, dir, diri,
				ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(info->ਚੌਖਟੇ[level-1].at->ਖੰਡ) & 0x0fffffff,
				&(ਚੌਖਟਾ->pblock), ਚੌਖਟਾ->buf);
			ਜੇ (errcode)
				ਜਾਓ ਬਾਹਰਲਾ_err;
		}
		errcode = ext2fs_get_dx_countlimit(fs, ਚੌਖਟਾ->buf,
						   &(ਚੌਖਟਾ->ਸਿਰ), ਨਿਰਾ);
		ਜੇ (errcode)
			ਜਾਓ ਬਾਹਰਲਾ_err;
		ਗਿਣਤੀ_2 = ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(ਚੌਖਟਾ->ਸਿਰ->ਗਿਣਤੀ_2);
		ਹੱਦਬੰਦੀ = ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(ਚੌਖਟਾ->ਸਿਰ->ਹੱਦਬੰਦੀ);
		ਚੌਖਟਾ->entries = (ਬਣਤਰ ext2_dx_ਇੰਦਰਾਜ *)(ਚੌਖਟਾ->ਸਿਰ);
		ਜੇ (!ਗਿਣਤੀ_2 || ਗਿਣਤੀ_2 > ਹੱਦਬੰਦੀ) {
			errcode = EXT2_ET_DIR_CORRUPTED;
			ਜਾਓ ਬਾਹਰਲਾ_err;
		}

		dx_ਖੋਜ_ਇੰਦਰਾਜ(ਚੌਖਟਾ, ਗਿਣਤੀ_2, info->hash);
	}
	ਵਾਪਸ 0;
ਬਾਹਰਲਾ_err:
	dx_release(info);
	ਵਾਪਸ errcode;
}

ਬਣਤਰ link_struct  {
	ext2_filsys	fs;
	ਅਚਲ ਅੱਖਰ	*name;
	ਪੂਰਨ		namelen;
	ext2_ino_t	inode;
	ਪੂਰਨ		flags;
	ਪੂਰਨ		ਪੂਰਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ	blocksize;
	errcode_t	err;
	ਬਣਤਰ ext2_super_ਖੰਡ *sb;
};

ਸਥਿਰ ਪੂਰਨ link_proc(ext2_ino_t dir EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
		     ਪੂਰਨ entru EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
		     ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *dirent,
		     ਪੂਰਨ	ਵਿੱਥ,
		     ਪੂਰਨ	blocksize,
		     ਅੱਖਰ	*buf,
		     ਖਾਲੀ	*priv_ਅੰਕੜੇ)
{
	ਬਣਤਰ link_struct *ls = (ਬਣਤਰ link_struct *) priv_ਅੰਕੜੇ;
	ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *ਅਗਲਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ rec_len, min_rec_len, curr_rec_len;
	ਪੂਰਨ ret = 0;
	ਪੂਰਨ csum_ਮਾਪ = 0;

	ਜੇ (ls->ਪੂਰਾ)
		ਵਾਪਸ DIRENT_ABORT;

	rec_len = EXT2_DIR_REC_LEN(ls->namelen);

	ls->err = ext2fs_get_rec_len(ls->fs, dirent, &curr_rec_len);
	ਜੇ (ls->err)
		ਵਾਪਸ DIRENT_ABORT;

	ਜੇ (ext2fs_has_feature_metadata_csum(ls->fs->super))
		csum_ਮਾਪ = ਆਕਾਰ(ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_ਪੂਛ);
	/*
	 * See if the following directory entry (if any) is unused;
	 * if so, absorb it into this one.
	 */
	ਅਗਲਾ = (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *) (buf + ਵਿੱਥ + curr_rec_len);
	ਜੇ ((ਵਿੱਥ + (ਪੂਰਨ) curr_rec_len < blocksize - (8 + csum_ਮਾਪ)) &&
	    (ਅਗਲਾ->inode == 0) &&
	    (ਵਿੱਥ + (ਪੂਰਨ) curr_rec_len + (ਪੂਰਨ) ਅਗਲਾ->rec_len <= blocksize)) {
		curr_rec_len += ਅਗਲਾ->rec_len;
		ls->err = ext2fs_ਸੈੱਟ_rec_len(ls->fs, curr_rec_len, dirent);
		ਜੇ (ls->err)
			ਵਾਪਸ DIRENT_ABORT;
		ret = DIRENT_ਬਦਲਿਆ;
	}

	/*
	 * If the directory entry is used, see if we can split the
	 * directory entry to make room for the new name.  If so,
	 * truncate it and return.
	 */
	ਜੇ (dirent->inode) {
		min_rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(dirent));
		ਜੇ (curr_rec_len < (min_rec_len + rec_len))
			ਵਾਪਸ ret;
		rec_len = curr_rec_len - min_rec_len;
		ls->err = ext2fs_ਸੈੱਟ_rec_len(ls->fs, min_rec_len, dirent);
		ਜੇ (ls->err)
			ਵਾਪਸ DIRENT_ABORT;
		ਅਗਲਾ = (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *) (buf + ਵਿੱਥ +
						  dirent->rec_len);
		ਅਗਲਾ->inode = 0;
		ext2fs_dirent_ਸੈੱਟ_name_len(ਅਗਲਾ, 0);
		ext2fs_dirent_ਸੈੱਟ_file_type(ਅਗਲਾ, 0);
		ls->err = ext2fs_ਸੈੱਟ_rec_len(ls->fs, rec_len, ਅਗਲਾ);
		ਜੇ (ls->err)
			ਵਾਪਸ DIRENT_ABORT;
		ਵਾਪਸ DIRENT_ਬਦਲਿਆ;
	}

	/*
	 * If we get this far, then the directory entry is not used.
	 * See if we can fit the request entry in.  If so, do it.
	 */
	ਜੇ (curr_rec_len < rec_len)
		ਵਾਪਸ ret;
	dirent->inode = ls->inode;
	ext2fs_dirent_ਸੈੱਟ_name_len(dirent, ls->namelen);
	strncpy(dirent->name, ls->name, ls->namelen);
	ਜੇ (ext2fs_has_feature_filetype(ls->sb))
		ext2fs_dirent_ਸੈੱਟ_file_type(dirent,
					    ls->flags & EXT2_FS_LINK_FT_ਨਕਾਬ);

	ls->ਪੂਰਾ++;
	ਵਾਪਸ DIRENT_ABORT|DIRENT_ਬਦਲਿਆ;
}

ਸਥਿਰ errcode_t ਜੋੜੋ_dirent_ਮੰਜ਼ਿਲ_buf(ext2_filsys fs, e2_blkcnt_t blockcnt,
				   ਅੱਖਰ *buf, ext2_ino_t dir,
				   ਬਣਤਰ ext2_inode *diri, ਅਚਲ ਅੱਖਰ *name,
				   ext2_ino_t ino, ਪੂਰਨ flags, blk64_t *pblkp)
{
	ਬਣਤਰ dir_context ctx;
	ਬਣਤਰ link_struct ls;
	errcode_t retval;

	retval = ਲੱਦੋ_logical_dir_ਖੰਡ(fs, dir, diri, blockcnt, pblkp, buf);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ctx.errcode = 0;
	ctx.func = link_proc;
	ctx.dir = dir;
	ctx.flags = DIRENT_FLAG_INCLUDE_ਖਾਲੀ_ਥਾਵਾਂ;
	ctx.buf = buf;
	ctx.priv_ਅੰਕੜੇ = &ls;

	ls.fs = fs;
	ls.name = name;
	ls.namelen = ਲੜੀ_ਲੰਬਾਈ(name);
	ls.inode = ino;
	ls.flags = flags;
	ls.ਪੂਰਾ = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	ext2fs_process_dir_ਖੰਡ(fs, pblkp, blockcnt, 0, 0, &ctx);
	ਜੇ (ctx.errcode)
		ਵਾਪਸ ctx.errcode;
	ਜੇ (ls.err)
		ਵਾਪਸ ls.err;
	ਜੇ (!ls.ਪੂਰਾ)
		ਵਾਪਸ EXT2_ET_DIR_NO_SPACE;
	ਵਾਪਸ 0;
}

ਬਣਤਰ dx_hash_map {
	__u32 hash;
	ਪੂਰਨ ਮਾਪ;
	ਪੂਰਨ off;
};

ਸਥਿਰ EXT2_QSORT_TYPE dx_hash_map_cmp(ਅਚਲ ਖਾਲੀ *ap, ਅਚਲ ਖਾਲੀ *bp)
{
	ਅਚਲ ਬਣਤਰ dx_hash_map *a = ap, *b = bp;

	ਜੇ (a->hash < b->hash)
		ਵਾਪਸ -1;
	ਜੇ (a->hash > b->hash)
		ਵਾਪਸ 1;
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t dx_move_dirents(ext2_filsys fs, ਬਣਤਰ dx_hash_map *map,
				 ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *from, ਖਾਲੀ *ਮੰਜ਼ਿਲ)
{
	ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *de;
	ਪੂਰਨ i;
	ਪੂਰਨ rec_len = 0;
	errcode_t retval;
	ਪੂਰਨ csum_ਮਾਪ = 0;
	ਖਾਲੀ *ਅਧਾਰ = ਮੰਜ਼ਿਲ;

	ਜੇ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_ਮਾਪ = ਆਕਾਰ(ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_ਪੂਛ);

	ਲਈ (i = 0; i < ਗਿਣਤੀ_2; i++) {
		de = (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *) ((ਅੱਖਰ *)from + map[i].off);
		rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(de));
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਮੰਜ਼ਿਲ, de, rec_len);
		retval = ext2fs_ਸੈੱਟ_rec_len(fs, rec_len, ਮੰਜ਼ਿਲ);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		ਮੰਜ਼ਿਲ = (ਅੱਖਰ *)ਮੰਜ਼ਿਲ + rec_len;
	}
	/*
	 * Update rec_len of the last dir entry to stretch to the end of block
	 */
	ਮੰਜ਼ਿਲ = (ਅੱਖਰ *)ਮੰਜ਼ਿਲ - rec_len;
	rec_len = fs->blocksize - ((ਅੱਖਰ *)ਮੰਜ਼ਿਲ - (ਅੱਖਰ *)ਅਧਾਰ) - csum_ਮਾਪ;
	retval = ext2fs_ਸੈੱਟ_rec_len(fs, rec_len, ਮੰਜ਼ਿਲ);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ਜੇ (csum_ਮਾਪ)
		ext2fs_initialize_dirent_ਪੂਛ(fs,
				EXT2_DIRENT_ਪੂਛ(ਅਧਾਰ, fs->blocksize));
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t dx_ਪਾਓ_ਇੰਦਰਾਜ(ext2_filsys fs, ext2_ino_t dir,
				 ਬਣਤਰ dx_lookup_info *info, ਪੂਰਨ level,
				 __u32 hash, blk64_t lblk)
{
	ਪੂਰਨ pcount;
	ਬਣਤਰ ext2_dx_ਇੰਦਰਾਜ *ਸਿਖਰ, *new;

	pcount = ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(info->ਚੌਖਟੇ[level].ਸਿਰ->ਗਿਣਤੀ_2);
	ਸਿਖਰ = info->ਚੌਖਟੇ[level].entries + pcount;
	new = info->ਚੌਖਟੇ[level].at + 1;
	memmove(new + 1, new, (ਅੱਖਰ *)ਸਿਖਰ - (ਅੱਖਰ *)new);
	new->hash = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(hash);
	new->ਖੰਡ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(lblk);
	info->ਚੌਖਟੇ[level].ਸਿਰ->ਗਿਣਤੀ_2 = ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(pcount + 1);
	ਵਾਪਸ ext2fs_write_dir_block4(fs, info->ਚੌਖਟੇ[level].pblock,
				       info->ਚੌਖਟੇ[level].buf, 0, dir);
}

ਸਥਿਰ errcode_t dx_ਵੰਡੋ_ਪੱਤੀ(ext2_filsys fs, ext2_ino_t dir,
			       ਬਣਤਰ ext2_inode *diri,
			       ਬਣਤਰ dx_lookup_info *info, ਖਾਲੀ *buf,
			       blk64_t ਪੱਤੀ_pblk, blk64_t new_lblk,
			       blk64_t new_pblk)
{
	ਪੂਰਨ hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *de;
	ਖਾਲੀ *buf2;
	errcode_t retval = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ rec_len;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਵਿੱਥ, move_ਮਾਪ;
	ਪੂਰਨ i, ਗਿਣਤੀ_2 = 0;
	ਬਣਤਰ dx_hash_map *map;
	ਪੂਰਨ continued;
	__u32 minor_hash;

	retval = ext2fs_get_mem(fs->blocksize, &buf2);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	retval = ext2fs_get_array(fs->blocksize / 12,
				  ਆਕਾਰ(ਬਣਤਰ dx_hash_map), &map);
	ਜੇ (retval) {
		ext2fs_free_mem(&buf2);
		ਵਾਪਸ retval;
	}
	ਲਈ (ਵਿੱਥ = 0; ਵਿੱਥ < fs->blocksize; ਵਿੱਥ += rec_len) {
		de = (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *) ((ਅੱਖਰ *)buf + ਵਿੱਥ);
		retval = ext2fs_get_rec_len(fs, de, &rec_len);
		ਜੇ (retval)
			ਜਾਓ ਬਾਹਰਲਾ;
		ਜੇ (ext2fs_dirent_name_len(de) > 0 && de->inode) {
			map[ਗਿਣਤੀ_2].off = ਵਿੱਥ;
			map[ਗਿਣਤੀ_2].ਮਾਪ = rec_len;
			retval = ext2fs_dirhash2(info->hash_alg, de->name,
					ext2fs_dirent_name_len(de),
					fs->encoding, hash_flags,
					fs->super->s_hash_ਬੀਜ,
					&(map[ਗਿਣਤੀ_2].hash),
					&minor_hash);
			ਜੇ (retval)
				ਜਾਓ ਬਾਹਰਲਾ;
			ਗਿਣਤੀ_2++;
		}
	}
	qsort(map, ਗਿਣਤੀ_2, ਆਕਾਰ(ਬਣਤਰ dx_hash_map), dx_hash_map_cmp);
	move_ਮਾਪ = 0;
	/* Find place to split block */
	ਲਈ (i = ਗਿਣਤੀ_2 - 1; i >= 0; i--) {
		ਜੇ (move_ਮਾਪ + map[i].ਮਾਪ / 2 > fs->blocksize / 2)
			ਰੋਕੋ;
		move_ਮਾਪ += map[i].ਮਾਪ;
	}
	/* Let i be the first entry to move */
	i++;
	/* Move selected directory entries to new block */
	retval = dx_move_dirents(fs, map + i, ਗਿਣਤੀ_2 - i, buf, buf2);
	ਜੇ (retval)
		ਜਾਓ ਬਾਹਰਲਾ;
	retval = ext2fs_write_dir_block4(fs, new_pblk, buf2, 0, dir);
	ਜੇ (retval)
		ਜਾਓ ਬਾਹਰਲਾ;
	/* Repack remaining entries in the old block */
	retval = dx_move_dirents(fs, map, i, buf, buf2);
	ਜੇ (retval)
		ਜਾਓ ਬਾਹਰਲਾ;
	retval = ext2fs_write_dir_block4(fs, ਪੱਤੀ_pblk, buf2, 0, dir);
	ਜੇ (retval)
		ਜਾਓ ਬਾਹਰਲਾ;
	/* Update parent node */
	continued = map[i].hash == map[i-1].hash;
	retval = dx_ਪਾਓ_ਇੰਦਰਾਜ(fs, dir, info, info->ਪੱਧਰ - 1,
				 map[i].hash + continued, new_lblk);
ਬਾਹਰਲਾ:
	ext2fs_free_mem(&buf2);
	ext2fs_free_mem(&map);
	ਵਾਪਸ retval;
}

ਸਥਿਰ errcode_t dx_grow_ਰੁੱਖ(ext2_filsys fs, ext2_ino_t dir,
			      ਬਣਤਰ ext2_inode *diri,
			      ਬਣਤਰ dx_lookup_info *info, ਖਾਲੀ *buf,
			      blk64_t ਪੱਤੀ_pblk)
{
	ਪੂਰਨ i;
	errcode_t retval;
	ext2_off64_t ਮਾਪ = EXT2_I_ਮਾਪ(diri);
	blk64_t lblk, pblk;
	ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *de;
	ਬਣਤਰ ext2_dx_countlimit *ਸਿਰ;
	ਪੂਰਨ csum_ਮਾਪ = 0;
	ਪੂਰਨ ਗਿਣਤੀ_2;

	ਜੇ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_ਮਾਪ = ਆਕਾਰ(ਬਣਤਰ ext2_dx_ਪੂਛ);

	/* Find level which can accommodate new child */
	ਲਈ (i = info->ਪੱਧਰ - 1; i >= 0; i--)
		ਜੇ (ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(info->ਚੌਖਟੇ[i].ਸਿਰ->ਗਿਣਤੀ_2) <
		    ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(info->ਚੌਖਟੇ[i].ਸਿਰ->ਹੱਦਬੰਦੀ))
			ਰੋਕੋ;
	/* Need to grow tree depth? */
	ਜੇ (i < 0 && info->ਪੱਧਰ >= ext2_dir_htree_level(fs))
		ਵਾਪਸ EXT2_ET_DIR_NO_SPACE;
	lblk = ਮਾਪ / fs->blocksize;
	ਮਾਪ += fs->blocksize;
	retval = ext2fs_inode_ਮਾਪ_ਸੈੱਟ(fs, diri, ਮਾਪ);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	retval = ext2fs_fallocate(fs,
			EXT2_FALLOCATE_FORCE_INIT | EXT2_FALLOCATE_ZERO_ਖੰਡਾਂ,
			dir, diri, 0, lblk, 1);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	retval = ext2fs_write_inode(fs, dir, diri);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	retval = ext2fs_bmap2(fs, dir, diri, ਨਿਰਾ, 0, lblk, ਨਿਰਾ, &pblk);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	/* Only leaf addition needed? */
	ਜੇ (i == (ਪੂਰਨ)info->ਪੱਧਰ - 1)
		ਵਾਪਸ dx_ਵੰਡੋ_ਪੱਤੀ(fs, dir, diri, info, buf, ਪੱਤੀ_pblk,
				     lblk, pblk);

	de = buf;
	de->inode = 0;
	ext2fs_dirent_ਸੈੱਟ_name_len(de, 0);
	ext2fs_dirent_ਸੈੱਟ_file_type(de, 0);
	retval = ext2fs_ਸੈੱਟ_rec_len(fs, fs->blocksize, de);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ਸਿਰ = (ਬਣਤਰ ext2_dx_countlimit *) ((ਅੱਖਰ *)buf + 8);
	ਗਿਣਤੀ_2 = ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(info->ਚੌਖਟੇ[i+1].ਸਿਰ->ਗਿਣਤੀ_2);
	/* Growing tree depth? */
	ਜੇ (i < 0) {
		ਬਣਤਰ ext2_dx_ਜੜ੍ਹ_info *ਜੜ੍ਹ;

		ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਸਿਰ, info->ਚੌਖਟੇ[0].entries,
		       ਗਿਣਤੀ_2 * ਆਕਾਰ(ਬਣਤਰ ext2_dx_ਇੰਦਰਾਜ));
		ਸਿਰ->ਹੱਦਬੰਦੀ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(
				(fs->blocksize - (8 + csum_ਮਾਪ)) /
				ਆਕਾਰ(ਬਣਤਰ ext2_dx_ਇੰਦਰਾਜ));
		/* head->count gets set by memcpy above to correct value */

		/* Now update tree root */
		info->ਚੌਖਟੇ[0].ਸਿਰ->ਗਿਣਤੀ_2 = ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(1);
		info->ਚੌਖਟੇ[0].entries[0].ਖੰਡ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(lblk);
		ਜੜ੍ਹ = (ਬਣਤਰ ext2_dx_ਜੜ੍ਹ_info *)
			((ਅੱਖਰ *)info->ਚੌਖਟੇ[0].buf + EXT2_DX_ਜੜ੍ਹ_OFF);
		ਜੜ੍ਹ->ਅਸਿੱਧਾ_ਪੱਧਰ++;
	} ਨਹੀਂਤਾਂ {
		/* Splitting internal node in two */
		ਪੂਰਨ count1 = ਗਿਣਤੀ_2 / 2;
		ਪੂਰਨ count2 = ਗਿਣਤੀ_2 - count1;
		__u32 ਵੰਡੋ_hash = ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(info->ਚੌਖਟੇ[i+1].entries[count1].hash);

		ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਸਿਰ, info->ਚੌਖਟੇ[i+1].entries + count1,
		       count2 * ਆਕਾਰ(ਬਣਤਰ ext2_dx_ਇੰਦਰਾਜ));
		ਸਿਰ->ਗਿਣਤੀ_2 = ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(count2);
		ਸਿਰ->ਹੱਦਬੰਦੀ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(
				(fs->blocksize - (8 + csum_ਮਾਪ)) /
				ਆਕਾਰ(ਬਣਤਰ ext2_dx_ਇੰਦਰਾਜ));
		info->ਚੌਖਟੇ[i+1].ਸਿਰ->ਗਿਣਤੀ_2 = ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(count1);

		/* Update parent node */
		retval = dx_ਪਾਓ_ਇੰਦਰਾਜ(fs, dir, info, i, ਵੰਡੋ_hash, lblk);
		ਜੇ (retval)
			ਵਾਪਸ retval;

	}
	/* Writeout split block / updated root */
	retval = ext2fs_write_dir_block4(fs, info->ਚੌਖਟੇ[i+1].pblock,
					 info->ਚੌਖਟੇ[i+1].buf, 0, dir);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	/* Writeout new tree block */
	retval = ext2fs_write_dir_block4(fs, pblk, buf, 0, dir);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t dx_link(ext2_filsys fs, ext2_ino_t dir,
			 ਬਣਤਰ ext2_inode *diri, ਅਚਲ ਅੱਖਰ *name,
			 ext2_ino_t ino, ਪੂਰਨ flags)
{
	ਬਣਤਰ dx_lookup_info dx_info;
	errcode_t retval;
	ਖਾਲੀ *blockbuf;
	ਬਿਨਾਨਿਸ਼ਾਨੀ restart = 0;
	blk64_t ਪੱਤੀ_pblk;

	retval = ext2fs_get_mem(fs->blocksize, &blockbuf);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	dx_info.name = name;
	dx_info.namelen = ਲੜੀ_ਲੰਬਾਈ(name);
again:
	retval = dx_lookup(fs, dir, diri, &dx_info);
	ਜੇ (retval)
		ਜਾਓ free_buf;

	retval = ਜੋੜੋ_dirent_ਮੰਜ਼ਿਲ_buf(fs,
		ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(dx_info.ਚੌਖਟੇ[dx_info.ਪੱਧਰ-1].at->ਖੰਡ) & 0x0fffffff,
		blockbuf, dir, diri, name, ino, flags, &ਪੱਤੀ_pblk);
	/*
	 * Success or error other than ENOSPC...? We are done. We may need upto
	 * two tries to add entry. One to split htree node and another to add
	 * new leaf block.
	 */
	ਜੇ (restart >= dx_info.ਪੱਧਰ || retval != EXT2_ET_DIR_NO_SPACE)
		ਜਾਓ free_ਚੌਖਟੇ;
	retval = dx_grow_ਰੁੱਖ(fs, dir, diri, &dx_info, blockbuf, ਪੱਤੀ_pblk);
	ਜੇ (retval)
		ਜਾਓ free_ਚੌਖਟੇ;
	/* Restart everything now that the tree is larger */
	restart++;
	dx_release(&dx_info);
	ਜਾਓ again;
free_ਚੌਖਟੇ:
	dx_release(&dx_info);
free_buf:
	ext2fs_free_mem(&blockbuf);
	ਵਾਪਸ retval;
}

ਬਣਤਰ fake_dirent
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

ਬਣਤਰ dx_ਜੜ੍ਹ
{
	ਬਣਤਰ fake_dirent dot;
	ਅੱਖਰ dot_name[4];
	ਬਣਤਰ fake_dirent dotdot;
	ਅੱਖਰ dotdot_name[4];
	ਬਣਤਰ ext2_dx_ਜੜ੍ਹ_info info;
	ਬਣਤਰ ext2_dx_ਇੰਦਰਾਜ entries[];
};

ਸਥਿਰ ਪੂਰਨ check_dx_ਜੜ੍ਹ(ext2_filsys fs, ext2_ino_t ino, ਬਣਤਰ dx_ਜੜ੍ਹ *ਜੜ੍ਹ)
{
	ਬਣਤਰ fake_dirent *fde;
	ਅੱਖਰ *error_ਸੁਨੇਹਾ_2;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ rlen;
	ਅੱਖਰ *blockend = (ਅੱਖਰ *)ਜੜ੍ਹ + fs->blocksize;

	fde = &ਜੜ੍ਹ->dot;
	ਜੇ (fde->name_len != 1) {
		error_ਸੁਨੇਹਾ_2 = "invalid name_len for '.'";
		ਜਾਓ corrupted;
	}
	ਜੇ (ਲੜੀ_ਤੁਲਨਾ(ਜੜ੍ਹ->dot_name, ".", fde->name_len)) {
		error_ਸੁਨੇਹਾ_2 = "invalid name for '.'";
		ਜਾਓ corrupted;
	}

	ext2fs_get_rec_len(fs, (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *)fde, &rlen);
	ਜੇ ((ਅੱਖਰ *)fde + rlen >= blockend) {
		error_ਸੁਨੇਹਾ_2 = "invalid rec_len for '.'";
		ਜਾਓ corrupted;
	}

	fde = &ਜੜ੍ਹ->dotdot;
	ਜੇ (fde->name_len != 2) {
		error_ਸੁਨੇਹਾ_2 = "invalid name_len for '..'";
		ਜਾਓ corrupted;
	}
	ਜੇ (ਲੜੀ_ਤੁਲਨਾ(ਜੜ੍ਹ->dotdot_name, "..", fde->name_len)) {
		error_ਸੁਨੇਹਾ_2 = "invalid name for '..'";
		ਜਾਓ corrupted;
	}
	ext2fs_get_rec_len(fs, (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *)fde, &rlen);
	ਜੇ ((ਅੱਖਰ *)fde + rlen >= blockend) {
		error_ਸੁਨੇਹਾ_2 = "invalid rec_len for '..'";
		ਜਾਓ corrupted;
	}

	ਵਾਪਸ 1;

corrupted:
	fprintf(stderr, "Corrupt dir %u: %s, running e2fsck is recommended\n",
			 ino, error_ਸੁਨੇਹਾ_2);
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *
ext2fs_ਅਗਲਾ_ਇੰਦਰਾਜ(ext2_filsys fs, ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *p)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ rlen;

	ext2fs_get_rec_len(fs, p, &rlen);
	ਵਾਪਸ (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *)((ਅੱਖਰ *)p + rlen);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ dx_ਸੈੱਟ_ਖੰਡ(ਬਣਤਰ ext2_dx_ਇੰਦਰਾਜ *ਇੰਦਰਾਜ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਮੁੱਲ)
{
	ਇੰਦਰਾਜ->ਖੰਡ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(ਮੁੱਲ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ dx_ਸੈੱਟ_ਗਿਣਤੀ(ਬਣਤਰ ext2_dx_ਇੰਦਰਾਜ *entries, ਬਿਨਾਨਿਸ਼ਾਨੀ ਮੁੱਲ)
{
	((ਬਣਤਰ ext2_dx_countlimit *) entries)->ਗਿਣਤੀ_2 = ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(ਮੁੱਲ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ dx_ਸੈੱਟ_ਹੱਦਬੰਦੀ(ਬਣਤਰ ext2_dx_ਇੰਦਰਾਜ *entries, ਬਿਨਾਨਿਸ਼ਾਨੀ ਮੁੱਲ)
{
	((ਬਣਤਰ ext2_dx_countlimit *) entries)->ਹੱਦਬੰਦੀ = ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(ਮੁੱਲ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ dx_ਜੜ੍ਹ_ਹੱਦਬੰਦੀ(ext2_filsys fs, ਬਿਨਾਨਿਸ਼ਾਨੀ infosize)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਇੰਦਰਾਜ_space = fs->blocksize -
			ext2fs_dir_rec_len(1, 0) -
			ext2fs_dir_rec_len(2, 0) - infosize;

	ਜੇ (ext2fs_has_feature_metadata_csum(fs->super))
		ਇੰਦਰਾਜ_space -= ਆਕਾਰ(ਬਣਤਰ ext2_dx_ਪੂਛ);
	ਵਾਪਸ ਇੰਦਰਾਜ_space / ਆਕਾਰ(ਬਣਤਰ ext2_dx_ਇੰਦਰਾਜ);
}

/*
 * This converts a one block unindexed directory to a 3 block indexed
 * directory, and adds the dentry to the indexed directory.  Returns 0 if the
 * index was not created; EAGAIN if it was; or an errcode_t on error.
 */
ਸਥਿਰ errcode_t try_make_ਸੂਚਕਾਂਕਿਤ_dir(ext2_filsys fs, ext2_ino_t dir,
				      ਅਚਲ ਅੱਖਰ *name, ext2_ino_t ino,
				      ਪੂਰਨ flags)
{
	ਬਣਤਰ ext2_inode inode;
	ਅੱਖਰ *buf0, *buf1, *ਸਿਖਰ;
	ਬਣਤਰ dx_ਜੜ੍ਹ *ਜੜ੍ਹ;
	ਬਣਤਰ ext2_dx_ਇੰਦਰਾਜ *entries;
	ਬਣਤਰ fake_dirent *fde;
	ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *de, *de2;
	blk64_t pblk0, pblk1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len;
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ blocksize = fs->blocksize;
	ਪੂਰਨ csum_ਮਾਪ = 0;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਜੇ (inode.i_ਮਾਪ > fs->blocksize || (inode.i_flags & EXT2_ਸੂਚਕਾਂਕ_FL))
		ਵਾਪਸ 0;

	ਜੇ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_ਮਾਪ = ਆਕਾਰ(ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ_ਪੂਛ);

	retval = ext2fs_get_mem(blocksize, &buf0);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	retval = ext2fs_get_mem(blocksize, &buf1);
	ਜੇ (retval)
		ਜਾਓ ਬਾਹਰਲਾ_buf0;

	retval = ext2fs_bmap2(fs, dir, &inode, ਨਿਰਾ, 0, 0, ਨਿਰਾ, &pblk0);
	ਜੇ (retval)
		ਜਾਓ ਬਾਹਰਲਾ_buf1;

	retval = ext2fs_read_dir_block4(fs, pblk0, buf0, 0, dir);
	ਜੇ (retval)
		ਜਾਓ ਬਾਹਰਲਾ_buf1;

	ਜੜ੍ਹ = (ਬਣਤਰ dx_ਜੜ੍ਹ *)buf0;
	ਜੇ (!check_dx_ਜੜ੍ਹ(fs, dir, ਜੜ੍ਹ)) {
		retval = EXT2_ET_DIR_CORRUPTED;
		ਜਾਓ ਬਾਹਰਲਾ_buf1;
	}

	/* The 0th block becomes the root, move the dirents out */
	fde = &ਜੜ੍ਹ->dotdot;
	de = ext2fs_ਅਗਲਾ_ਇੰਦਰਾਜ(fs, (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *)fde);
	len = ((ਅੱਖਰ *) ਜੜ੍ਹ) + (blocksize - csum_ਮਾਪ) - (ਅੱਖਰ *) de;

	/* Allocate new block for the 0th block's dirents */
	retval = ext2fs_bmap2(fs, dir, &inode, ਨਿਰਾ, BMAP_ALLOC | BMAP_ZERO, 1,
			      ਨਿਰਾ, &pblk1);
	ਜੇ (retval)
		ਜਾਓ ਬਾਹਰਲਾ_buf1;

	ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf1, de, len);
	memset(de, 0, len); /* wipe old data */

	de = (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *)buf1;
	ਸਿਖਰ = buf1 + len;
	ਜਦੋਂ ((ਅੱਖਰ *)(de2 = ext2fs_ਅਗਲਾ_ਇੰਦਰਾਜ(fs, de)) < ਸਿਖਰ) {
#ਜੇ 0
		ਜੇ (ext4_check_dir_ਇੰਦਰਾਜ(dir, ਨਿਰਾ, de, bh2, buf1, len,
					(ਅੱਖਰ *)de - buf1)) {
			retval = EXT2_ET_DIR_CORRUPTED;
			ਜਾਓ ਬਾਹਰਲਾ_buf1;
		}
#ਜੇ_ਅੰਤ
		de = de2;
	}
	retval = ext2fs_ਸੈੱਟ_rec_len(fs,
			buf1 + (blocksize - csum_ਮਾਪ) - (ਅੱਖਰ *) de, de);
	ਜੇ (retval)
		ਜਾਓ ਬਾਹਰਲਾ_buf1;

	ਜੇ (csum_ਮਾਪ)
		ext2fs_initialize_dirent_ਪੂਛ(fs,
				EXT2_DIRENT_ਪੂਛ(buf1, fs->blocksize));

	/* Initialize the root; the dot dirents already exist */
	de = (ਬਣਤਰ ext2_dir_ਇੰਦਰਾਜ *) (&ਜੜ੍ਹ->dotdot);
	retval = ext2fs_ਸੈੱਟ_rec_len(fs, blocksize - ext2fs_dir_rec_len(2, 0),
				    de);
	memset (&ਜੜ੍ਹ->info, 0, ਆਕਾਰ(ਜੜ੍ਹ->info));
	ਜੜ੍ਹ->info.info_ਲੰਮਾਈ = ਆਕਾਰ(ਜੜ੍ਹ->info);
	ਜੇ (ext4_hash_ਵਿੱਚ_dirent(&inode))
		ਜੜ੍ਹ->info.hash_ਸੰਸਕਰਣ = EXT2_HASH_SIPHASH;
	ਨਹੀਂਤਾਂ
		ਜੜ੍ਹ->info.hash_ਸੰਸਕਰਣ = fs->super->s_def_hash_ਸੰਸਕਰਣ;

	entries = ਜੜ੍ਹ->entries;
	dx_ਸੈੱਟ_ਖੰਡ(entries, 1);
	dx_ਸੈੱਟ_ਗਿਣਤੀ(entries, 1);
	dx_ਸੈੱਟ_ਹੱਦਬੰਦੀ(entries, dx_ਜੜ੍ਹ_ਹੱਦਬੰਦੀ(fs, ਆਕਾਰ(ਜੜ੍ਹ->info)));

	retval = ext2fs_write_dir_block4(fs, pblk1, buf1, 0, dir);
	ਜੇ (retval)
		ਜਾਓ ਬਾਹਰਲਾ_buf1;

	retval = ext2fs_write_dir_block4(fs, pblk0, buf0, 0, dir);
	ਜੇ (retval)
		ਜਾਓ ਬਾਹਰਲਾ_buf1;

	inode.i_flags |= EXT2_ਸੂਚਕਾਂਕ_FL;
	inode.i_ਮਾਪ += fs->blocksize;
	retval = ext2fs_write_inode(fs, dir, &inode);
	ਜੇ (retval)
		ਜਾਓ ਬਾਹਰਲਾ_buf1;

	retval = EAGAIN;
ਬਾਹਰਲਾ_buf1:
	ext2fs_free_mem(&buf1);
ਬਾਹਰਲਾ_buf0:
	ext2fs_free_mem(&buf0);
	ਵਾਪਸ retval;
}

/*
 * Note: the low 3 bits of the flags field are used as the directory
 * entry filetype.
 */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __TURBOC__
 #ਹਦਾਇਤ argsused
#ਜੇ_ਅੰਤ
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, ਅਚਲ ਅੱਖਰ *name,
		      ext2_ino_t ino, ਪੂਰਨ flags)
{
	errcode_t		retval;
	ਬਣਤਰ link_struct	ls;
	ਬਣਤਰ ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ (!(fs->flags & EXT2_FLAG_RW))
		ਵਾਪਸ EXT2_ET_RO_FILSYS;

retry:
	ਜੇ ((retval = ext2fs_read_inode(fs, dir, &inode)) != 0)
		ਵਾਪਸ retval;

	ਜੇ (inode.i_flags & EXT2_ਸੂਚਕਾਂਕ_FL)
		ਵਾਪਸ dx_link(fs, dir, &inode, name, ino, flags);

	ls.fs = fs;
	ls.name = name;
	ls.namelen = name ? ਲੜੀ_ਲੰਬਾਈ(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.ਪੂਰਾ = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	ਜੇ ((flags & EXT2FS_LINK_APPEND) &&
	    !(inode.i_flags & EXT4_INLINE_ਅੰਕੜੇ_FL)) {
		blk64_t		lblk, pblk = 0;
		ਬਣਤਰ dir_context ctx;

		lblk = (inode.i_ਮਾਪ / fs->blocksize) - 1;
		retval = ext2fs_bmap2(fs, dir, &inode, ਨਿਰਾ, 0, lblk,
				      ਨਿਰਾ, &pblk);
		ਜੇ (retval)
			ਵਾਪਸ retval;

		ctx.dir = dir;
		ctx.flags = DIRENT_FLAG_INCLUDE_ਖਾਲੀ_ਥਾਵਾਂ;
		ctx.func = link_proc;
		ctx.priv_ਅੰਕੜੇ = &ls;
		ctx.errcode = 0;
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		ਜੇ (retval)
			ਵਾਪਸ retval;

		ext2fs_process_dir_ਖੰਡ(fs, &pblk, lblk, 0, 0, &ctx);
		retval = ctx.errcode;
		ext2fs_free_mem(&ctx.buf);
	} ਨਹੀਂਤਾਂ {
		retval = ext2fs_dir_iterate2(fs, dir,
					     DIRENT_FLAG_INCLUDE_ਖਾਲੀ_ਥਾਵਾਂ,
					     ਨਿਰਾ, link_proc, &ls);
	}
	ਜੇ (retval)
		ਵਾਪਸ retval;
	ਜੇ (ls.err)
		ਵਾਪਸ ls.err;

	ਜੇ (!ls.ਪੂਰਾ) {
		ਜੇ (!(flags & EXT2FS_LINK_EXPAND))
			ਵਾਪਸ EXT2_ET_DIR_NO_SPACE;

		ਜੇ (ext2fs_has_feature_dir_ਸੂਚਕਾਂਕ(fs->super)) {
			retval = try_make_ਸੂਚਕਾਂਕਿਤ_dir(fs, dir, name, ino,
						      flags);
			ਜੇ (retval == EAGAIN)
				ਜਾਓ retry;
			ਜੇ (retval)
				ਵਾਪਸ retval;
		}

		retval = ext2fs_expand_dir(fs, dir);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		ਜਾਓ retry;
	}
	ਵਾਪਸ 0;
}

/* Does this directory have an htree index? */
ਪੂਰਨ ext2fs_dir_is_dx(ext2_filsys fs, ਅਚਲ ਬਣਤਰ ext2_inode *inode)
{
	ਵਾਪਸ ext2fs_has_feature_dir_ਸੂਚਕਾਂਕ(fs->super) &&
		S_ISDIR(inode->i_ਬਹੁਲਕ) && (inode->i_flags & EXT2_ਸੂਚਕਾਂਕ_FL);
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
ਖਾਲੀ ext2fs_inc_nlink(ext2_filsys fs, ਬਣਤਰ ext2_inode *inode)
{
	inode->i_links_ਗਿਣਤੀ++;

	ਜੇ (ext2fs_dir_is_dx(fs, inode) &&
	    (inode->i_links_ਗਿਣਤੀ > EXT2_LINK_MAX || inode->i_links_ਗਿਣਤੀ == 2))
		inode->i_links_ਗਿਣਤੀ = 1;
}

/*
 * If a directory had nlink == 1, then we should let it be 1. This indicates
 * directory has >EXT2_LINK_MAX subdirs.
 */
ਖਾਲੀ ext2fs_dec_nlink(ਬਣਤਰ ext2_inode *inode)
{
	ਜੇ (!S_ISDIR(inode->i_ਬਹੁਲਕ) || inode->i_links_ਗਿਣਤੀ > 2)
		inode->i_links_ਗਿਣਤੀ--;
}

ਪੂਰਨ ext2fs_dir_link_max(ext2_filsys fs, ਬਣਤਰ ext2_inode_large *inode)
{
	ਵਾਪਸ inode->i_links_ਗਿਣਤੀ >= EXT2_LINK_MAX &&
	       !(ext2fs_dir_is_dx(fs, EXT2_INODE(inode)) &&
		 ext2fs_has_feature_dir_nlink(fs->super));
}

ਪੂਰਨ ext2fs_dir_link_ਖਾਲੀ_ਥਾਵਾਂ(ਬਣਤਰ ext2_inode *inode)
{
	ਵਾਪਸ inode->i_links_ਗਿਣਤੀ == 2 || inode->i_links_ਗਿਣਤੀ == 1;
}
