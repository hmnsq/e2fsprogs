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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_DX_ಬೇರು_OFF 24

ರಚನೆ dx_ಚೌಕಟ್ಟು {
	ಶೂನ್ಯ *buf;
	blk64_t pblock;
	ರಚನೆ ext2_dx_countlimit *ತಲೆ;
	ರಚನೆ ext2_dx_ನಮೂದು *entries;
	ರಚನೆ ext2_dx_ನಮೂದು *at;
};

ರಚನೆ dx_lookup_info {
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
	ಪೂರ್ಣಾಂಕ namelen;
	ಪೂರ್ಣಾಂಕ hash_alg;
	__u32 hash;
	ಚಿಹ್ನೆರಹಿತ ಮಟ್ಟಗಳು;
	ರಚನೆ dx_ಚೌಕಟ್ಟು ಚೌಕಟ್ಟುಗಳು[EXT4_HTREE_LEVEL];
};

ಸ್ಥಿರ errcode_t alloc_dx_ಚೌಕಟ್ಟು(ext2_filsys fs, ರಚನೆ dx_ಚೌಕಟ್ಟು *ಚೌಕಟ್ಟು)
{
	ಹಿಂತಿರುಗಿ ext2fs_get_mem(fs->blocksize, &ಚೌಕಟ್ಟು->buf);
}

ಸ್ಥಿರ ಶೂನ್ಯ dx_release(ರಚನೆ dx_lookup_info *info)
{
	ಚಿಹ್ನೆರಹಿತ level;

	ಗಾಗಿ (level = 0; level < info->ಮಟ್ಟಗಳು; level++) {
		ಆದರೆ (info->ಚೌಕಟ್ಟುಗಳು[level].buf == ಖಾಲಿ)
			ನಿಲ್ಲಿಸು;
		ext2fs_free_mem(&(info->ಚೌಕಟ್ಟುಗಳು[level].buf));
	}
	info->ಮಟ್ಟಗಳು = 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ dx_ಹುಡುಕು_ನಮೂದು(ರಚನೆ dx_ಚೌಕಟ್ಟು *ಚೌಕಟ್ಟು, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, __u32 hash)
{
	ರಚನೆ ext2_dx_ನಮೂದು *p, *q, *m;

	p = ಚೌಕಟ್ಟು->entries + 1;
	q = ಚೌಕಟ್ಟು->entries + ಎಣಿಕೆ_2 - 1;
	ಆಗ (p <= q) {
		m = p + (q - p) / 2;
		ಆದರೆ (ext2fs_le32_ಗಮ್ಯ_cpu(m->hash) > hash)
			q = m - 1;
		ಇಲ್ಲದಿದ್ದರೆ
			p = m + 1;
	}
	ಚೌಕಟ್ಟು->at = p - 1;
}

ಸ್ಥಿರ errcode_t ತುಂಬು_logical_dir_ಖಂಡ(ext2_filsys fs, ext2_ino_t dir,
					ರಚನೆ ext2_inode *diri, blk64_t ಖಂಡ,
					blk64_t *pblk, ಶೂನ್ಯ *buf)
{
	errcode_t errcode;
	ಪೂರ್ಣಾಂಕ ret_flags;

	errcode = ext2fs_bmap2(fs, dir, diri, ಖಾಲಿ, 0, ಖಂಡ, &ret_flags,
			       pblk);
	ಆದರೆ (errcode)
		ಹಿಂತಿರುಗಿ errcode;
	ಆದರೆ (ret_flags & BMAP_RET_UNINIT)
		ಹಿಂತಿರುಗಿ EXT2_ET_DIR_CORRUPTED;
	ಹಿಂತಿರುಗಿ ext2fs_read_dir_block4(fs, *pblk, buf, 0, dir);
}

ಸ್ಥಿರ errcode_t dx_lookup(ext2_filsys fs, ext2_ino_t dir,
			   ರಚನೆ ext2_inode *diri, ರಚನೆ dx_lookup_info *info)
{
	ರಚನೆ ext2_dx_ಬೇರು_info *ಬೇರು;
	errcode_t errcode;
	ಪೂರ್ಣಾಂಕ level = 0;
	ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಎಲ್ಲೆ;
	ಪೂರ್ಣಾಂಕ hash_alg;
	ಪೂರ್ಣಾಂಕ hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	__u32 minor_hash;
	ರಚನೆ dx_ಚೌಕಟ್ಟು *ಚೌಕಟ್ಟು;

	errcode = alloc_dx_ಚೌಕಟ್ಟು(fs, &(info->ಚೌಕಟ್ಟುಗಳು[0]));
	ಆದರೆ (errcode)
		ಹಿಂತಿರುಗಿ errcode;
	info->ಮಟ್ಟಗಳು = 1;

	errcode = ತುಂಬು_logical_dir_ಖಂಡ(fs, dir, diri, 0,
					 &(info->ಚೌಕಟ್ಟುಗಳು[0].pblock),
					 info->ಚೌಕಟ್ಟುಗಳು[0].buf);
	ಆದರೆ (errcode)
		ಹೋಗು ಹೊರಹೋಗು_err;
	ಬೇರು = (ರಚನೆ ext2_dx_ಬೇರು_info *) ((ಅಕ್ಷರ *)info->ಚೌಕಟ್ಟುಗಳು[0].buf +
					     EXT2_DX_ಬೇರು_OFF);
	hash_alg = ಬೇರು->hash_ಆವೃತ್ತಿ;
	ಆದರೆ (hash_alg != EXT2_HASH_TEA && hash_alg != EXT2_HASH_HALF_MD4 &&
	    hash_alg != EXT2_HASH_LEGACY) {
		errcode = EXT2_ET_DIRHASH_UNSUPP;
		ಹೋಗು ಹೊರಹೋಗು_err;
	}
	ಆದರೆ (hash_alg <= EXT2_HASH_TEA &&
	    fs->super->s_flags & EXT2_FLAGS_UNSIGNED_HASH)
		hash_alg += 3;
	ಆದರೆ (ಬೇರು->ಪರೋಕ್ಷ_ಮಟ್ಟಗಳು >= ext2_dir_htree_level(fs)) {
		errcode = EXT2_ET_DIR_CORRUPTED;
		ಹೋಗು ಹೊರಹೋಗು_err;
	}
	info->hash_alg = hash_alg;

	errcode = ext2fs_dirhash2(hash_alg, info->name, info->namelen,
				  fs->encoding, hash_flags,
				  fs->super->s_hash_ಬೀಜ, &info->hash,
				  &minor_hash);
	ಆದರೆ (errcode)
		ಹೋಗು ಹೊರಹೋಗು_err;

	ಗಾಗಿ (level = 0; level <= ಬೇರು->ಪರೋಕ್ಷ_ಮಟ್ಟಗಳು; level++) {
		ಚೌಕಟ್ಟು = &(info->ಚೌಕಟ್ಟುಗಳು[level]);
		ಆದರೆ (level > 0) {
			errcode = alloc_dx_ಚೌಕಟ್ಟು(fs, ಚೌಕಟ್ಟು);
			ಆದರೆ (errcode)
				ಹೋಗು ಹೊರಹೋಗು_err;
			info->ಮಟ್ಟಗಳು++;

			errcode = ತುಂಬು_logical_dir_ಖಂಡ(fs, dir, diri,
				ext2fs_le32_ಗಮ್ಯ_cpu(info->ಚೌಕಟ್ಟುಗಳು[level-1].at->ಖಂಡ) & 0x0fffffff,
				&(ಚೌಕಟ್ಟು->pblock), ಚೌಕಟ್ಟು->buf);
			ಆದರೆ (errcode)
				ಹೋಗು ಹೊರಹೋಗು_err;
		}
		errcode = ext2fs_get_dx_countlimit(fs, ಚೌಕಟ್ಟು->buf,
						   &(ಚೌಕಟ್ಟು->ತಲೆ), ಖಾಲಿ);
		ಆದರೆ (errcode)
			ಹೋಗು ಹೊರಹೋಗು_err;
		ಎಣಿಕೆ_2 = ext2fs_le16_ಗಮ್ಯ_cpu(ಚೌಕಟ್ಟು->ತಲೆ->ಎಣಿಕೆ_2);
		ಎಲ್ಲೆ = ext2fs_le16_ಗಮ್ಯ_cpu(ಚೌಕಟ್ಟು->ತಲೆ->ಎಲ್ಲೆ);
		ಚೌಕಟ್ಟು->entries = (ರಚನೆ ext2_dx_ನಮೂದು *)(ಚೌಕಟ್ಟು->ತಲೆ);
		ಆದರೆ (!ಎಣಿಕೆ_2 || ಎಣಿಕೆ_2 > ಎಲ್ಲೆ) {
			errcode = EXT2_ET_DIR_CORRUPTED;
			ಹೋಗು ಹೊರಹೋಗು_err;
		}

		dx_ಹುಡುಕು_ನಮೂದು(ಚೌಕಟ್ಟು, ಎಣಿಕೆ_2, info->hash);
	}
	ಹಿಂತಿರುಗಿ 0;
ಹೊರಹೋಗು_err:
	dx_release(info);
	ಹಿಂತಿರುಗಿ errcode;
}

ರಚನೆ link_struct  {
	ext2_filsys	fs;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ	*name;
	ಪೂರ್ಣಾಂಕ		namelen;
	ext2_ino_t	inode;
	ಪೂರ್ಣಾಂಕ		flags;
	ಪೂರ್ಣಾಂಕ		ಮುಗಿದ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ	blocksize;
	errcode_t	err;
	ರಚನೆ ext2_super_ಖಂಡ *sb;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ link_proc(ext2_ino_t dir EXT2FS_ATTR((ಬಳಸದ)),
		     ಪೂರ್ಣಾಂಕ entru EXT2FS_ATTR((ಬಳಸದ)),
		     ರಚನೆ ext2_dir_ನಮೂದು *dirent,
		     ಪೂರ್ಣಾಂಕ	ಸ್ಥಾನಾಂತರ,
		     ಪೂರ್ಣಾಂಕ	blocksize,
		     ಅಕ್ಷರ	*buf,
		     ಶೂನ್ಯ	*priv_ದತ್ತಾಂಶ)
{
	ರಚನೆ link_struct *ls = (ರಚನೆ link_struct *) priv_ದತ್ತಾಂಶ;
	ರಚನೆ ext2_dir_ನಮೂದು *ಮುಂದಿನ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ rec_len, min_rec_len, curr_rec_len;
	ಪೂರ್ಣಾಂಕ ret = 0;
	ಪೂರ್ಣಾಂಕ csum_ಗಾತ್ರ = 0;

	ಆದರೆ (ls->ಮುಗಿದ)
		ಹಿಂತಿರುಗಿ DIRENT_ABORT;

	rec_len = EXT2_DIR_REC_LEN(ls->namelen);

	ls->err = ext2fs_get_rec_len(ls->fs, dirent, &curr_rec_len);
	ಆದರೆ (ls->err)
		ಹಿಂತಿರುಗಿ DIRENT_ABORT;

	ಆದರೆ (ext2fs_has_feature_metadata_csum(ls->fs->super))
		csum_ಗಾತ್ರ = ಗಾತ್ರ(ರಚನೆ ext2_dir_ನಮೂದು_ಬಾಲ);
	/*
	 * See if the following directory entry (if any) is unused;
	 * if so, absorb it into this one.
	 */
	ಮುಂದಿನ = (ರಚನೆ ext2_dir_ನಮೂದು *) (buf + ಸ್ಥಾನಾಂತರ + curr_rec_len);
	ಆದರೆ ((ಸ್ಥಾನಾಂತರ + (ಪೂರ್ಣಾಂಕ) curr_rec_len < blocksize - (8 + csum_ಗಾತ್ರ)) &&
	    (ಮುಂದಿನ->inode == 0) &&
	    (ಸ್ಥಾನಾಂತರ + (ಪೂರ್ಣಾಂಕ) curr_rec_len + (ಪೂರ್ಣಾಂಕ) ಮುಂದಿನ->rec_len <= blocksize)) {
		curr_rec_len += ಮುಂದಿನ->rec_len;
		ls->err = ext2fs_ಗಣ_rec_len(ls->fs, curr_rec_len, dirent);
		ಆದರೆ (ls->err)
			ಹಿಂತಿರುಗಿ DIRENT_ABORT;
		ret = DIRENT_ಬದಲಾಯಿತು;
	}

	/*
	 * If the directory entry is used, see if we can split the
	 * directory entry to make room for the new name.  If so,
	 * truncate it and return.
	 */
	ಆದರೆ (dirent->inode) {
		min_rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(dirent));
		ಆದರೆ (curr_rec_len < (min_rec_len + rec_len))
			ಹಿಂತಿರುಗಿ ret;
		rec_len = curr_rec_len - min_rec_len;
		ls->err = ext2fs_ಗಣ_rec_len(ls->fs, min_rec_len, dirent);
		ಆದರೆ (ls->err)
			ಹಿಂತಿರುಗಿ DIRENT_ABORT;
		ಮುಂದಿನ = (ರಚನೆ ext2_dir_ನಮೂದು *) (buf + ಸ್ಥಾನಾಂತರ +
						  dirent->rec_len);
		ಮುಂದಿನ->inode = 0;
		ext2fs_dirent_ಗಣ_name_len(ಮುಂದಿನ, 0);
		ext2fs_dirent_ಗಣ_file_type(ಮುಂದಿನ, 0);
		ls->err = ext2fs_ಗಣ_rec_len(ls->fs, rec_len, ಮುಂದಿನ);
		ಆದರೆ (ls->err)
			ಹಿಂತಿರುಗಿ DIRENT_ABORT;
		ಹಿಂತಿರುಗಿ DIRENT_ಬದಲಾಯಿತು;
	}

	/*
	 * If we get this far, then the directory entry is not used.
	 * See if we can fit the request entry in.  If so, do it.
	 */
	ಆದರೆ (curr_rec_len < rec_len)
		ಹಿಂತಿರುಗಿ ret;
	dirent->inode = ls->inode;
	ext2fs_dirent_ಗಣ_name_len(dirent, ls->namelen);
	strncpy(dirent->name, ls->name, ls->namelen);
	ಆದರೆ (ext2fs_has_feature_filetype(ls->sb))
		ext2fs_dirent_ಗಣ_file_type(dirent,
					    ls->flags & EXT2_FS_LINK_FT_ಮುಖವಾಡ);

	ls->ಮುಗಿದ++;
	ಹಿಂತಿರುಗಿ DIRENT_ABORT|DIRENT_ಬದಲಾಯಿತು;
}

ಸ್ಥಿರ errcode_t ಕೂಡಿಸು_dirent_ಗಮ್ಯ_buf(ext2_filsys fs, e2_blkcnt_t blockcnt,
				   ಅಕ್ಷರ *buf, ext2_ino_t dir,
				   ರಚನೆ ext2_inode *diri, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
				   ext2_ino_t ino, ಪೂರ್ಣಾಂಕ flags, blk64_t *pblkp)
{
	ರಚನೆ dir_context ctx;
	ರಚನೆ link_struct ls;
	errcode_t retval;

	retval = ತುಂಬು_logical_dir_ಖಂಡ(fs, dir, diri, blockcnt, pblkp, buf);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ctx.errcode = 0;
	ctx.func = link_proc;
	ctx.dir = dir;
	ctx.flags = DIRENT_FLAG_INCLUDE_ಖಾಲಿಗಳು;
	ctx.buf = buf;
	ctx.priv_ದತ್ತಾಂಶ = &ls;

	ls.fs = fs;
	ls.name = name;
	ls.namelen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name);
	ls.inode = ino;
	ls.flags = flags;
	ls.ಮುಗಿದ = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	ext2fs_process_dir_ಖಂಡ(fs, pblkp, blockcnt, 0, 0, &ctx);
	ಆದರೆ (ctx.errcode)
		ಹಿಂತಿರುಗಿ ctx.errcode;
	ಆದರೆ (ls.err)
		ಹಿಂತಿರುಗಿ ls.err;
	ಆದರೆ (!ls.ಮುಗಿದ)
		ಹಿಂತಿರುಗಿ EXT2_ET_DIR_NO_SPACE;
	ಹಿಂತಿರುಗಿ 0;
}

ರಚನೆ dx_hash_map {
	__u32 hash;
	ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2;
	ಪೂರ್ಣಾಂಕ off;
};

ಸ್ಥಿರ EXT2_QSORT_TYPE dx_hash_map_cmp(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ap, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *bp)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ dx_hash_map *a = ap, *b = bp;

	ಆದರೆ (a->hash < b->hash)
		ಹಿಂತಿರುಗಿ -1;
	ಆದರೆ (a->hash > b->hash)
		ಹಿಂತಿರುಗಿ 1;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t dx_move_dirents(ext2_filsys fs, ರಚನೆ dx_hash_map *map,
				 ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *from, ಶೂನ್ಯ *ಗಮ್ಯ)
{
	ರಚನೆ ext2_dir_ನಮೂದು *de;
	ಪೂರ್ಣಾಂಕ i;
	ಪೂರ್ಣಾಂಕ rec_len = 0;
	errcode_t retval;
	ಪೂರ್ಣಾಂಕ csum_ಗಾತ್ರ = 0;
	ಶೂನ್ಯ *ತಳ = ಗಮ್ಯ;

	ಆದರೆ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_ಗಾತ್ರ = ಗಾತ್ರ(ರಚನೆ ext2_dir_ನಮೂದು_ಬಾಲ);

	ಗಾಗಿ (i = 0; i < ಎಣಿಕೆ_2; i++) {
		de = (ರಚನೆ ext2_dir_ನಮೂದು *) ((ಅಕ್ಷರ *)from + map[i].off);
		rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(de));
		ಸ್ಮರಣೆ_ನಕಲು(ಗಮ್ಯ, de, rec_len);
		retval = ext2fs_ಗಣ_rec_len(fs, rec_len, ಗಮ್ಯ);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		ಗಮ್ಯ = (ಅಕ್ಷರ *)ಗಮ್ಯ + rec_len;
	}
	/*
	 * Update rec_len of the last dir entry to stretch to the end of block
	 */
	ಗಮ್ಯ = (ಅಕ್ಷರ *)ಗಮ್ಯ - rec_len;
	rec_len = fs->blocksize - ((ಅಕ್ಷರ *)ಗಮ್ಯ - (ಅಕ್ಷರ *)ತಳ) - csum_ಗಾತ್ರ;
	retval = ext2fs_ಗಣ_rec_len(fs, rec_len, ಗಮ್ಯ);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ಆದರೆ (csum_ಗಾತ್ರ)
		ext2fs_initialize_dirent_ಬಾಲ(fs,
				EXT2_DIRENT_ಬಾಲ(ತಳ, fs->blocksize));
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t dx_ಸೇರಿಸು_ನಮೂದು(ext2_filsys fs, ext2_ino_t dir,
				 ರಚನೆ dx_lookup_info *info, ಪೂರ್ಣಾಂಕ level,
				 __u32 hash, blk64_t lblk)
{
	ಪೂರ್ಣಾಂಕ pcount;
	ರಚನೆ ext2_dx_ನಮೂದು *ತುದಿ, *new;

	pcount = ext2fs_le16_ಗಮ್ಯ_cpu(info->ಚೌಕಟ್ಟುಗಳು[level].ತಲೆ->ಎಣಿಕೆ_2);
	ತುದಿ = info->ಚೌಕಟ್ಟುಗಳು[level].entries + pcount;
	new = info->ಚೌಕಟ್ಟುಗಳು[level].at + 1;
	memmove(new + 1, new, (ಅಕ್ಷರ *)ತುದಿ - (ಅಕ್ಷರ *)new);
	new->hash = ext2fs_cpu_ಗಮ್ಯ_le32(hash);
	new->ಖಂಡ = ext2fs_cpu_ಗಮ್ಯ_le32(lblk);
	info->ಚೌಕಟ್ಟುಗಳು[level].ತಲೆ->ಎಣಿಕೆ_2 = ext2fs_cpu_ಗಮ್ಯ_le16(pcount + 1);
	ಹಿಂತಿರುಗಿ ext2fs_write_dir_block4(fs, info->ಚೌಕಟ್ಟುಗಳು[level].pblock,
				       info->ಚೌಕಟ್ಟುಗಳು[level].buf, 0, dir);
}

ಸ್ಥಿರ errcode_t dx_ವಿಭಜಿಸು_ಎಲೆ(ext2_filsys fs, ext2_ino_t dir,
			       ರಚನೆ ext2_inode *diri,
			       ರಚನೆ dx_lookup_info *info, ಶೂನ್ಯ *buf,
			       blk64_t ಎಲೆ_pblk, blk64_t new_lblk,
			       blk64_t new_pblk)
{
	ಪೂರ್ಣಾಂಕ hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	ರಚನೆ ext2_dir_ನಮೂದು *de;
	ಶೂನ್ಯ *buf2;
	errcode_t retval = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ rec_len;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ, move_ಗಾತ್ರ;
	ಪೂರ್ಣಾಂಕ i, ಎಣಿಕೆ_2 = 0;
	ರಚನೆ dx_hash_map *map;
	ಪೂರ್ಣಾಂಕ continued;
	__u32 minor_hash;

	retval = ext2fs_get_mem(fs->blocksize, &buf2);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	retval = ext2fs_get_array(fs->blocksize / 12,
				  ಗಾತ್ರ(ರಚನೆ dx_hash_map), &map);
	ಆದರೆ (retval) {
		ext2fs_free_mem(&buf2);
		ಹಿಂತಿರುಗಿ retval;
	}
	ಗಾಗಿ (ಸ್ಥಾನಾಂತರ = 0; ಸ್ಥಾನಾಂತರ < fs->blocksize; ಸ್ಥಾನಾಂತರ += rec_len) {
		de = (ರಚನೆ ext2_dir_ನಮೂದು *) ((ಅಕ್ಷರ *)buf + ಸ್ಥಾನಾಂತರ);
		retval = ext2fs_get_rec_len(fs, de, &rec_len);
		ಆದರೆ (retval)
			ಹೋಗು ಹೊರಹೋಗು;
		ಆದರೆ (ext2fs_dirent_name_len(de) > 0 && de->inode) {
			map[ಎಣಿಕೆ_2].off = ಸ್ಥಾನಾಂತರ;
			map[ಎಣಿಕೆ_2].ಗಾತ್ರ_2 = rec_len;
			retval = ext2fs_dirhash2(info->hash_alg, de->name,
					ext2fs_dirent_name_len(de),
					fs->encoding, hash_flags,
					fs->super->s_hash_ಬೀಜ,
					&(map[ಎಣಿಕೆ_2].hash),
					&minor_hash);
			ಆದರೆ (retval)
				ಹೋಗು ಹೊರಹೋಗು;
			ಎಣಿಕೆ_2++;
		}
	}
	qsort(map, ಎಣಿಕೆ_2, ಗಾತ್ರ(ರಚನೆ dx_hash_map), dx_hash_map_cmp);
	move_ಗಾತ್ರ = 0;
	/* Find place to split block */
	ಗಾಗಿ (i = ಎಣಿಕೆ_2 - 1; i >= 0; i--) {
		ಆದರೆ (move_ಗಾತ್ರ + map[i].ಗಾತ್ರ_2 / 2 > fs->blocksize / 2)
			ನಿಲ್ಲಿಸು;
		move_ಗಾತ್ರ += map[i].ಗಾತ್ರ_2;
	}
	/* Let i be the first entry to move */
	i++;
	/* Move selected directory entries to new block */
	retval = dx_move_dirents(fs, map + i, ಎಣಿಕೆ_2 - i, buf, buf2);
	ಆದರೆ (retval)
		ಹೋಗು ಹೊರಹೋಗು;
	retval = ext2fs_write_dir_block4(fs, new_pblk, buf2, 0, dir);
	ಆದರೆ (retval)
		ಹೋಗು ಹೊರಹೋಗು;
	/* Repack remaining entries in the old block */
	retval = dx_move_dirents(fs, map, i, buf, buf2);
	ಆದರೆ (retval)
		ಹೋಗು ಹೊರಹೋಗು;
	retval = ext2fs_write_dir_block4(fs, ಎಲೆ_pblk, buf2, 0, dir);
	ಆದರೆ (retval)
		ಹೋಗು ಹೊರಹೋಗು;
	/* Update parent node */
	continued = map[i].hash == map[i-1].hash;
	retval = dx_ಸೇರಿಸು_ನಮೂದು(fs, dir, info, info->ಮಟ್ಟಗಳು - 1,
				 map[i].hash + continued, new_lblk);
ಹೊರಹೋಗು:
	ext2fs_free_mem(&buf2);
	ext2fs_free_mem(&map);
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ errcode_t dx_grow_ವೃಕ್ಷ(ext2_filsys fs, ext2_ino_t dir,
			      ರಚನೆ ext2_inode *diri,
			      ರಚನೆ dx_lookup_info *info, ಶೂನ್ಯ *buf,
			      blk64_t ಎಲೆ_pblk)
{
	ಪೂರ್ಣಾಂಕ i;
	errcode_t retval;
	ext2_off64_t ಗಾತ್ರ_2 = EXT2_I_ಗಾತ್ರ(diri);
	blk64_t lblk, pblk;
	ರಚನೆ ext2_dir_ನಮೂದು *de;
	ರಚನೆ ext2_dx_countlimit *ತಲೆ;
	ಪೂರ್ಣಾಂಕ csum_ಗಾತ್ರ = 0;
	ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2;

	ಆದರೆ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_ಗಾತ್ರ = ಗಾತ್ರ(ರಚನೆ ext2_dx_ಬಾಲ);

	/* Find level which can accommodate new child */
	ಗಾಗಿ (i = info->ಮಟ್ಟಗಳು - 1; i >= 0; i--)
		ಆದರೆ (ext2fs_le16_ಗಮ್ಯ_cpu(info->ಚೌಕಟ್ಟುಗಳು[i].ತಲೆ->ಎಣಿಕೆ_2) <
		    ext2fs_le16_ಗಮ್ಯ_cpu(info->ಚೌಕಟ್ಟುಗಳು[i].ತಲೆ->ಎಲ್ಲೆ))
			ನಿಲ್ಲಿಸು;
	/* Need to grow tree depth? */
	ಆದರೆ (i < 0 && info->ಮಟ್ಟಗಳು >= ext2_dir_htree_level(fs))
		ಹಿಂತಿರುಗಿ EXT2_ET_DIR_NO_SPACE;
	lblk = ಗಾತ್ರ_2 / fs->blocksize;
	ಗಾತ್ರ_2 += fs->blocksize;
	retval = ext2fs_inode_ಗಾತ್ರ_ಗಣ(fs, diri, ಗಾತ್ರ_2);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	retval = ext2fs_fallocate(fs,
			EXT2_FALLOCATE_FORCE_INIT | EXT2_FALLOCATE_ZERO_ಖಂಡಗಳು,
			dir, diri, 0, lblk, 1);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	retval = ext2fs_write_inode(fs, dir, diri);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	retval = ext2fs_bmap2(fs, dir, diri, ಖಾಲಿ, 0, lblk, ಖಾಲಿ, &pblk);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	/* Only leaf addition needed? */
	ಆದರೆ (i == (ಪೂರ್ಣಾಂಕ)info->ಮಟ್ಟಗಳು - 1)
		ಹಿಂತಿರುಗಿ dx_ವಿಭಜಿಸು_ಎಲೆ(fs, dir, diri, info, buf, ಎಲೆ_pblk,
				     lblk, pblk);

	de = buf;
	de->inode = 0;
	ext2fs_dirent_ಗಣ_name_len(de, 0);
	ext2fs_dirent_ಗಣ_file_type(de, 0);
	retval = ext2fs_ಗಣ_rec_len(fs, fs->blocksize, de);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ತಲೆ = (ರಚನೆ ext2_dx_countlimit *) ((ಅಕ್ಷರ *)buf + 8);
	ಎಣಿಕೆ_2 = ext2fs_le16_ಗಮ್ಯ_cpu(info->ಚೌಕಟ್ಟುಗಳು[i+1].ತಲೆ->ಎಣಿಕೆ_2);
	/* Growing tree depth? */
	ಆದರೆ (i < 0) {
		ರಚನೆ ext2_dx_ಬೇರು_info *ಬೇರು;

		ಸ್ಮರಣೆ_ನಕಲು(ತಲೆ, info->ಚೌಕಟ್ಟುಗಳು[0].entries,
		       ಎಣಿಕೆ_2 * ಗಾತ್ರ(ರಚನೆ ext2_dx_ನಮೂದು));
		ತಲೆ->ಎಲ್ಲೆ = ext2fs_cpu_ಗಮ್ಯ_le16(
				(fs->blocksize - (8 + csum_ಗಾತ್ರ)) /
				ಗಾತ್ರ(ರಚನೆ ext2_dx_ನಮೂದು));
		/* head->count gets set by memcpy above to correct value */

		/* Now update tree root */
		info->ಚೌಕಟ್ಟುಗಳು[0].ತಲೆ->ಎಣಿಕೆ_2 = ext2fs_cpu_ಗಮ್ಯ_le16(1);
		info->ಚೌಕಟ್ಟುಗಳು[0].entries[0].ಖಂಡ = ext2fs_cpu_ಗಮ್ಯ_le32(lblk);
		ಬೇರು = (ರಚನೆ ext2_dx_ಬೇರು_info *)
			((ಅಕ್ಷರ *)info->ಚೌಕಟ್ಟುಗಳು[0].buf + EXT2_DX_ಬೇರು_OFF);
		ಬೇರು->ಪರೋಕ್ಷ_ಮಟ್ಟಗಳು++;
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* Splitting internal node in two */
		ಪೂರ್ಣಾಂಕ count1 = ಎಣಿಕೆ_2 / 2;
		ಪೂರ್ಣಾಂಕ count2 = ಎಣಿಕೆ_2 - count1;
		__u32 ವಿಭಜಿಸು_hash = ext2fs_le32_ಗಮ್ಯ_cpu(info->ಚೌಕಟ್ಟುಗಳು[i+1].entries[count1].hash);

		ಸ್ಮರಣೆ_ನಕಲು(ತಲೆ, info->ಚೌಕಟ್ಟುಗಳು[i+1].entries + count1,
		       count2 * ಗಾತ್ರ(ರಚನೆ ext2_dx_ನಮೂದು));
		ತಲೆ->ಎಣಿಕೆ_2 = ext2fs_cpu_ಗಮ್ಯ_le16(count2);
		ತಲೆ->ಎಲ್ಲೆ = ext2fs_cpu_ಗಮ್ಯ_le16(
				(fs->blocksize - (8 + csum_ಗಾತ್ರ)) /
				ಗಾತ್ರ(ರಚನೆ ext2_dx_ನಮೂದು));
		info->ಚೌಕಟ್ಟುಗಳು[i+1].ತಲೆ->ಎಣಿಕೆ_2 = ext2fs_cpu_ಗಮ್ಯ_le16(count1);

		/* Update parent node */
		retval = dx_ಸೇರಿಸು_ನಮೂದು(fs, dir, info, i, ವಿಭಜಿಸು_hash, lblk);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

	}
	/* Writeout split block / updated root */
	retval = ext2fs_write_dir_block4(fs, info->ಚೌಕಟ್ಟುಗಳು[i+1].pblock,
					 info->ಚೌಕಟ್ಟುಗಳು[i+1].buf, 0, dir);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	/* Writeout new tree block */
	retval = ext2fs_write_dir_block4(fs, pblk, buf, 0, dir);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t dx_link(ext2_filsys fs, ext2_ino_t dir,
			 ರಚನೆ ext2_inode *diri, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
			 ext2_ino_t ino, ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ dx_lookup_info dx_info;
	errcode_t retval;
	ಶೂನ್ಯ *blockbuf;
	ಚಿಹ್ನೆರಹಿತ restart = 0;
	blk64_t ಎಲೆ_pblk;

	retval = ext2fs_get_mem(fs->blocksize, &blockbuf);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	dx_info.name = name;
	dx_info.namelen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name);
again:
	retval = dx_lookup(fs, dir, diri, &dx_info);
	ಆದರೆ (retval)
		ಹೋಗು free_buf;

	retval = ಕೂಡಿಸು_dirent_ಗಮ್ಯ_buf(fs,
		ext2fs_le32_ಗಮ್ಯ_cpu(dx_info.ಚೌಕಟ್ಟುಗಳು[dx_info.ಮಟ್ಟಗಳು-1].at->ಖಂಡ) & 0x0fffffff,
		blockbuf, dir, diri, name, ino, flags, &ಎಲೆ_pblk);
	/*
	 * Success or error other than ENOSPC...? We are done. We may need upto
	 * two tries to add entry. One to split htree node and another to add
	 * new leaf block.
	 */
	ಆದರೆ (restart >= dx_info.ಮಟ್ಟಗಳು || retval != EXT2_ET_DIR_NO_SPACE)
		ಹೋಗು free_ಚೌಕಟ್ಟುಗಳು;
	retval = dx_grow_ವೃಕ್ಷ(fs, dir, diri, &dx_info, blockbuf, ಎಲೆ_pblk);
	ಆದರೆ (retval)
		ಹೋಗು free_ಚೌಕಟ್ಟುಗಳು;
	/* Restart everything now that the tree is larger */
	restart++;
	dx_release(&dx_info);
	ಹೋಗು again;
free_ಚೌಕಟ್ಟುಗಳು:
	dx_release(&dx_info);
free_buf:
	ext2fs_free_mem(&blockbuf);
	ಹಿಂತಿರುಗಿ retval;
}

ರಚನೆ fake_dirent
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

ರಚನೆ dx_ಬೇರು
{
	ರಚನೆ fake_dirent dot;
	ಅಕ್ಷರ dot_name[4];
	ರಚನೆ fake_dirent dotdot;
	ಅಕ್ಷರ dotdot_name[4];
	ರಚನೆ ext2_dx_ಬೇರು_info info;
	ರಚನೆ ext2_dx_ನಮೂದು entries[];
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ check_dx_ಬೇರು(ext2_filsys fs, ext2_ino_t ino, ರಚನೆ dx_ಬೇರು *ಬೇರು)
{
	ರಚನೆ fake_dirent *fde;
	ಅಕ್ಷರ *error_ಸಂದೇಶ_2;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ rlen;
	ಅಕ್ಷರ *blockend = (ಅಕ್ಷರ *)ಬೇರು + fs->blocksize;

	fde = &ಬೇರು->dot;
	ಆದರೆ (fde->name_len != 1) {
		error_ಸಂದೇಶ_2 = "invalid name_len for '.'";
		ಹೋಗು corrupted;
	}
	ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(ಬೇರು->dot_name, ".", fde->name_len)) {
		error_ಸಂದೇಶ_2 = "invalid name for '.'";
		ಹೋಗು corrupted;
	}

	ext2fs_get_rec_len(fs, (ರಚನೆ ext2_dir_ನಮೂದು *)fde, &rlen);
	ಆದರೆ ((ಅಕ್ಷರ *)fde + rlen >= blockend) {
		error_ಸಂದೇಶ_2 = "invalid rec_len for '.'";
		ಹೋಗು corrupted;
	}

	fde = &ಬೇರು->dotdot;
	ಆದರೆ (fde->name_len != 2) {
		error_ಸಂದೇಶ_2 = "invalid name_len for '..'";
		ಹೋಗು corrupted;
	}
	ಆದರೆ (ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(ಬೇರು->dotdot_name, "..", fde->name_len)) {
		error_ಸಂದೇಶ_2 = "invalid name for '..'";
		ಹೋಗು corrupted;
	}
	ext2fs_get_rec_len(fs, (ರಚನೆ ext2_dir_ನಮೂದು *)fde, &rlen);
	ಆದರೆ ((ಅಕ್ಷರ *)fde + rlen >= blockend) {
		error_ಸಂದೇಶ_2 = "invalid rec_len for '..'";
		ಹೋಗು corrupted;
	}

	ಹಿಂತಿರುಗಿ 1;

corrupted:
	fprintf(stderr, "Corrupt dir %u: %s, running e2fsck is recommended\n",
			 ino, error_ಸಂದೇಶ_2);
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ ext2_dir_ನಮೂದು *
ext2fs_ಮುಂದಿನ_ನಮೂದು(ext2_filsys fs, ರಚನೆ ext2_dir_ನಮೂದು *p)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ rlen;

	ext2fs_get_rec_len(fs, p, &rlen);
	ಹಿಂತಿರುಗಿ (ರಚನೆ ext2_dir_ನಮೂದು *)((ಅಕ್ಷರ *)p + rlen);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ dx_ಗಣ_ಖಂಡ(ರಚನೆ ext2_dx_ನಮೂದು *ನಮೂದು, ಚಿಹ್ನೆರಹಿತ ಮೌಲ್ಯ)
{
	ನಮೂದು->ಖಂಡ = ext2fs_cpu_ಗಮ್ಯ_le32(ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ dx_ಗಣ_ಎಣಿಕೆ(ರಚನೆ ext2_dx_ನಮೂದು *entries, ಚಿಹ್ನೆರಹಿತ ಮೌಲ್ಯ)
{
	((ರಚನೆ ext2_dx_countlimit *) entries)->ಎಣಿಕೆ_2 = ext2fs_cpu_ಗಮ್ಯ_le16(ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ dx_ಗಣ_ಎಲ್ಲೆ(ರಚನೆ ext2_dx_ನಮೂದು *entries, ಚಿಹ್ನೆರಹಿತ ಮೌಲ್ಯ)
{
	((ರಚನೆ ext2_dx_countlimit *) entries)->ಎಲ್ಲೆ = ext2fs_cpu_ಗಮ್ಯ_le16(ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ dx_ಬೇರು_ಎಲ್ಲೆ(ext2_filsys fs, ಚಿಹ್ನೆರಹಿತ infosize)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ನಮೂದು_space = fs->blocksize -
			ext2fs_dir_rec_len(1, 0) -
			ext2fs_dir_rec_len(2, 0) - infosize;

	ಆದರೆ (ext2fs_has_feature_metadata_csum(fs->super))
		ನಮೂದು_space -= ಗಾತ್ರ(ರಚನೆ ext2_dx_ಬಾಲ);
	ಹಿಂತಿರುಗಿ ನಮೂದು_space / ಗಾತ್ರ(ರಚನೆ ext2_dx_ನಮೂದು);
}

/*
 * This converts a one block unindexed directory to a 3 block indexed
 * directory, and adds the dentry to the indexed directory.  Returns 0 if the
 * index was not created; EAGAIN if it was; or an errcode_t on error.
 */
ಸ್ಥಿರ errcode_t try_make_ಸೂಚ್ಯಂಕಿತ_dir(ext2_filsys fs, ext2_ino_t dir,
				      ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ext2_ino_t ino,
				      ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ ext2_inode inode;
	ಅಕ್ಷರ *buf0, *buf1, *ತುದಿ;
	ರಚನೆ dx_ಬೇರು *ಬೇರು;
	ರಚನೆ ext2_dx_ನಮೂದು *entries;
	ರಚನೆ fake_dirent *fde;
	ರಚನೆ ext2_dir_ನಮೂದು *de, *de2;
	blk64_t pblk0, pblk1;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ len;
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ blocksize = fs->blocksize;
	ಪೂರ್ಣಾಂಕ csum_ಗಾತ್ರ = 0;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಆದರೆ (inode.i_ಗಾತ್ರ > fs->blocksize || (inode.i_flags & EXT2_ಸೂಚ್ಯಂಕ_FL))
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_ಗಾತ್ರ = ಗಾತ್ರ(ರಚನೆ ext2_dir_ನಮೂದು_ಬಾಲ);

	retval = ext2fs_get_mem(blocksize, &buf0);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	retval = ext2fs_get_mem(blocksize, &buf1);
	ಆದರೆ (retval)
		ಹೋಗು ಹೊರಹೋಗು_buf0;

	retval = ext2fs_bmap2(fs, dir, &inode, ಖಾಲಿ, 0, 0, ಖಾಲಿ, &pblk0);
	ಆದರೆ (retval)
		ಹೋಗು ಹೊರಹೋಗು_buf1;

	retval = ext2fs_read_dir_block4(fs, pblk0, buf0, 0, dir);
	ಆದರೆ (retval)
		ಹೋಗು ಹೊರಹೋಗು_buf1;

	ಬೇರು = (ರಚನೆ dx_ಬೇರು *)buf0;
	ಆದರೆ (!check_dx_ಬೇರು(fs, dir, ಬೇರು)) {
		retval = EXT2_ET_DIR_CORRUPTED;
		ಹೋಗು ಹೊರಹೋಗು_buf1;
	}

	/* The 0th block becomes the root, move the dirents out */
	fde = &ಬೇರು->dotdot;
	de = ext2fs_ಮುಂದಿನ_ನಮೂದು(fs, (ರಚನೆ ext2_dir_ನಮೂದು *)fde);
	len = ((ಅಕ್ಷರ *) ಬೇರು) + (blocksize - csum_ಗಾತ್ರ) - (ಅಕ್ಷರ *) de;

	/* Allocate new block for the 0th block's dirents */
	retval = ext2fs_bmap2(fs, dir, &inode, ಖಾಲಿ, BMAP_ALLOC | BMAP_ZERO, 1,
			      ಖಾಲಿ, &pblk1);
	ಆದರೆ (retval)
		ಹೋಗು ಹೊರಹೋಗು_buf1;

	ಸ್ಮರಣೆ_ನಕಲು(buf1, de, len);
	memset(de, 0, len); /* wipe old data */

	de = (ರಚನೆ ext2_dir_ನಮೂದು *)buf1;
	ತುದಿ = buf1 + len;
	ಆಗ ((ಅಕ್ಷರ *)(de2 = ext2fs_ಮುಂದಿನ_ನಮೂದು(fs, de)) < ತುದಿ) {
#ಆದರೆ 0
		ಆದರೆ (ext4_check_dir_ನಮೂದು(dir, ಖಾಲಿ, de, bh2, buf1, len,
					(ಅಕ್ಷರ *)de - buf1)) {
			retval = EXT2_ET_DIR_CORRUPTED;
			ಹೋಗು ಹೊರಹೋಗು_buf1;
		}
#ಆದರೆ_ಅಂತ್ಯ
		de = de2;
	}
	retval = ext2fs_ಗಣ_rec_len(fs,
			buf1 + (blocksize - csum_ಗಾತ್ರ) - (ಅಕ್ಷರ *) de, de);
	ಆದರೆ (retval)
		ಹೋಗು ಹೊರಹೋಗು_buf1;

	ಆದರೆ (csum_ಗಾತ್ರ)
		ext2fs_initialize_dirent_ಬಾಲ(fs,
				EXT2_DIRENT_ಬಾಲ(buf1, fs->blocksize));

	/* Initialize the root; the dot dirents already exist */
	de = (ರಚನೆ ext2_dir_ನಮೂದು *) (&ಬೇರು->dotdot);
	retval = ext2fs_ಗಣ_rec_len(fs, blocksize - ext2fs_dir_rec_len(2, 0),
				    de);
	memset (&ಬೇರು->info, 0, ಗಾತ್ರ(ಬೇರು->info));
	ಬೇರು->info.info_ಉದ್ದಳತೆ = ಗಾತ್ರ(ಬೇರು->info);
	ಆದರೆ (ext4_hash_ಒಳ_dirent(&inode))
		ಬೇರು->info.hash_ಆವೃತ್ತಿ = EXT2_HASH_SIPHASH;
	ಇಲ್ಲದಿದ್ದರೆ
		ಬೇರು->info.hash_ಆವೃತ್ತಿ = fs->super->s_def_hash_ಆವೃತ್ತಿ;

	entries = ಬೇರು->entries;
	dx_ಗಣ_ಖಂಡ(entries, 1);
	dx_ಗಣ_ಎಣಿಕೆ(entries, 1);
	dx_ಗಣ_ಎಲ್ಲೆ(entries, dx_ಬೇರು_ಎಲ್ಲೆ(fs, ಗಾತ್ರ(ಬೇರು->info)));

	retval = ext2fs_write_dir_block4(fs, pblk1, buf1, 0, dir);
	ಆದರೆ (retval)
		ಹೋಗು ಹೊರಹೋಗು_buf1;

	retval = ext2fs_write_dir_block4(fs, pblk0, buf0, 0, dir);
	ಆದರೆ (retval)
		ಹೋಗು ಹೊರಹೋಗು_buf1;

	inode.i_flags |= EXT2_ಸೂಚ್ಯಂಕ_FL;
	inode.i_ಗಾತ್ರ += fs->blocksize;
	retval = ext2fs_write_inode(fs, dir, &inode);
	ಆದರೆ (retval)
		ಹೋಗು ಹೊರಹೋಗು_buf1;

	retval = EAGAIN;
ಹೊರಹೋಗು_buf1:
	ext2fs_free_mem(&buf1);
ಹೊರಹೋಗು_buf0:
	ext2fs_free_mem(&buf0);
	ಹಿಂತಿರುಗಿ retval;
}

/*
 * Note: the low 3 bits of the flags field are used as the directory
 * entry filetype.
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __TURBOC__
 #ಸೂಚನೆ argsused
#ಆದರೆ_ಅಂತ್ಯ
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name,
		      ext2_ino_t ino, ಪೂರ್ಣಾಂಕ flags)
{
	errcode_t		retval;
	ರಚನೆ link_struct	ls;
	ರಚನೆ ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ (!(fs->flags & EXT2_FLAG_RW))
		ಹಿಂತಿರುಗಿ EXT2_ET_RO_FILSYS;

retry:
	ಆದರೆ ((retval = ext2fs_read_inode(fs, dir, &inode)) != 0)
		ಹಿಂತಿರುಗಿ retval;

	ಆದರೆ (inode.i_flags & EXT2_ಸೂಚ್ಯಂಕ_FL)
		ಹಿಂತಿರುಗಿ dx_link(fs, dir, &inode, name, ino, flags);

	ls.fs = fs;
	ls.name = name;
	ls.namelen = name ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.ಮುಗಿದ = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	ಆದರೆ ((flags & EXT2FS_LINK_APPEND) &&
	    !(inode.i_flags & EXT4_INLINE_ದತ್ತಾಂಶ_FL)) {
		blk64_t		lblk, pblk = 0;
		ರಚನೆ dir_context ctx;

		lblk = (inode.i_ಗಾತ್ರ / fs->blocksize) - 1;
		retval = ext2fs_bmap2(fs, dir, &inode, ಖಾಲಿ, 0, lblk,
				      ಖಾಲಿ, &pblk);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

		ctx.dir = dir;
		ctx.flags = DIRENT_FLAG_INCLUDE_ಖಾಲಿಗಳು;
		ctx.func = link_proc;
		ctx.priv_ದತ್ತಾಂಶ = &ls;
		ctx.errcode = 0;
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

		ext2fs_process_dir_ಖಂಡ(fs, &pblk, lblk, 0, 0, &ctx);
		retval = ctx.errcode;
		ext2fs_free_mem(&ctx.buf);
	} ಇಲ್ಲದಿದ್ದರೆ {
		retval = ext2fs_dir_iterate2(fs, dir,
					     DIRENT_FLAG_INCLUDE_ಖಾಲಿಗಳು,
					     ಖಾಲಿ, link_proc, &ls);
	}
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ಆದರೆ (ls.err)
		ಹಿಂತಿರುಗಿ ls.err;

	ಆದರೆ (!ls.ಮುಗಿದ) {
		ಆದರೆ (!(flags & EXT2FS_LINK_EXPAND))
			ಹಿಂತಿರುಗಿ EXT2_ET_DIR_NO_SPACE;

		ಆದರೆ (ext2fs_has_feature_dir_ಸೂಚ್ಯಂಕ(fs->super)) {
			retval = try_make_ಸೂಚ್ಯಂಕಿತ_dir(fs, dir, name, ino,
						      flags);
			ಆದರೆ (retval == EAGAIN)
				ಹೋಗು retry;
			ಆದರೆ (retval)
				ಹಿಂತಿರುಗಿ retval;
		}

		retval = ext2fs_expand_dir(fs, dir);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		ಹೋಗು retry;
	}
	ಹಿಂತಿರುಗಿ 0;
}

/* Does this directory have an htree index? */
ಪೂರ್ಣಾಂಕ ext2fs_dir_is_dx(ext2_filsys fs, ಸ್ಥಿರಾಂಕ ರಚನೆ ext2_inode *inode)
{
	ಹಿಂತಿರುಗಿ ext2fs_has_feature_dir_ಸೂಚ್ಯಂಕ(fs->super) &&
		S_ISDIR(inode->i_ಬಹುಲಕ) && (inode->i_flags & EXT2_ಸೂಚ್ಯಂಕ_FL);
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
ಶೂನ್ಯ ext2fs_inc_nlink(ext2_filsys fs, ರಚನೆ ext2_inode *inode)
{
	inode->i_links_ಎಣಿಕೆ++;

	ಆದರೆ (ext2fs_dir_is_dx(fs, inode) &&
	    (inode->i_links_ಎಣಿಕೆ > EXT2_LINK_MAX || inode->i_links_ಎಣಿಕೆ == 2))
		inode->i_links_ಎಣಿಕೆ = 1;
}

/*
 * If a directory had nlink == 1, then we should let it be 1. This indicates
 * directory has >EXT2_LINK_MAX subdirs.
 */
ಶೂನ್ಯ ext2fs_dec_nlink(ರಚನೆ ext2_inode *inode)
{
	ಆದರೆ (!S_ISDIR(inode->i_ಬಹುಲಕ) || inode->i_links_ಎಣಿಕೆ > 2)
		inode->i_links_ಎಣಿಕೆ--;
}

ಪೂರ್ಣಾಂಕ ext2fs_dir_link_max(ext2_filsys fs, ರಚನೆ ext2_inode_large *inode)
{
	ಹಿಂತಿರುಗಿ inode->i_links_ಎಣಿಕೆ >= EXT2_LINK_MAX &&
	       !(ext2fs_dir_is_dx(fs, EXT2_INODE(inode)) &&
		 ext2fs_has_feature_dir_nlink(fs->super));
}

ಪೂರ್ಣಾಂಕ ext2fs_dir_link_ಖಾಲಿಗಳು(ರಚನೆ ext2_inode *inode)
{
	ಹಿಂತಿರುಗಿ inode->i_links_ಎಣಿಕೆ == 2 || inode->i_links_ಎಣಿಕೆ == 1;
}
