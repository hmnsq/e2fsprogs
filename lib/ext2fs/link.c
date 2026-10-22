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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "ext2fsP.h"

#నిర్వచించు EXT2_DX_మూలం_OFF 24

నిర్మాణం dx_చట్రం {
	శూన్య *buf;
	blk64_t pblock;
	నిర్మాణం ext2_dx_countlimit *తల;
	నిర్మాణం ext2_dx_నమోదు *entries;
	నిర్మాణం ext2_dx_నమోదు *at;
};

నిర్మాణం dx_lookup_info {
	స్థిరరాశి అక్షరం *name;
	పూర్ణాంకం namelen;
	పూర్ణాంకం hash_alg;
	__u32 hash;
	గుర్తులేని స్థాయిలు;
	నిర్మాణం dx_చట్రం చట్రాలు[EXT4_HTREE_LEVEL];
};

స్థిర errcode_t alloc_dx_చట్రం(ext2_filsys fs, నిర్మాణం dx_చట్రం *చట్రం)
{
	తిరిగి ext2fs_get_mem(fs->blocksize, &చట్రం->buf);
}

స్థిర శూన్య dx_release(నిర్మాణం dx_lookup_info *info)
{
	గుర్తులేని level;

	కొరకు (level = 0; level < info->స్థాయిలు; level++) {
		ఒకవేళ (info->చట్రాలు[level].buf == ఖాళీ)
			ఆపు;
		ext2fs_free_mem(&(info->చట్రాలు[level].buf));
	}
	info->స్థాయిలు = 0;
}

స్థిర శూన్య dx_శోధన_నమోదు(నిర్మాణం dx_చట్రం *చట్రం, పూర్ణాంకం లెక్క, __u32 hash)
{
	నిర్మాణం ext2_dx_నమోదు *p, *q, *m;

	p = చట్రం->entries + 1;
	q = చట్రం->entries + లెక్క - 1;
	అయితే (p <= q) {
		m = p + (q - p) / 2;
		ఒకవేళ (ext2fs_le32_గమ్యం_cpu(m->hash) > hash)
			q = m - 1;
		లేకపోతే
			p = m + 1;
	}
	చట్రం->at = p - 1;
}

స్థిర errcode_t ఎక్కించు_logical_dir_ఖండం(ext2_filsys fs, ext2_ino_t dir,
					నిర్మాణం ext2_inode *diri, blk64_t ఖండం,
					blk64_t *pblk, శూన్య *buf)
{
	errcode_t errcode;
	పూర్ణాంకం ret_flags;

	errcode = ext2fs_bmap2(fs, dir, diri, ఖాళీ, 0, ఖండం, &ret_flags,
			       pblk);
	ఒకవేళ (errcode)
		తిరిగి errcode;
	ఒకవేళ (ret_flags & BMAP_RET_UNINIT)
		తిరిగి EXT2_ET_DIR_CORRUPTED;
	తిరిగి ext2fs_read_dir_block4(fs, *pblk, buf, 0, dir);
}

స్థిర errcode_t dx_lookup(ext2_filsys fs, ext2_ino_t dir,
			   నిర్మాణం ext2_inode *diri, నిర్మాణం dx_lookup_info *info)
{
	నిర్మాణం ext2_dx_మూలం_info *మూలం;
	errcode_t errcode;
	పూర్ణాంకం level = 0;
	పూర్ణాంకం లెక్క, హద్దు;
	పూర్ణాంకం hash_alg;
	పూర్ణాంకం hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	__u32 minor_hash;
	నిర్మాణం dx_చట్రం *చట్రం;

	errcode = alloc_dx_చట్రం(fs, &(info->చట్రాలు[0]));
	ఒకవేళ (errcode)
		తిరిగి errcode;
	info->స్థాయిలు = 1;

	errcode = ఎక్కించు_logical_dir_ఖండం(fs, dir, diri, 0,
					 &(info->చట్రాలు[0].pblock),
					 info->చట్రాలు[0].buf);
	ఒకవేళ (errcode)
		వెళ్ళు బహిర్గతం_err;
	మూలం = (నిర్మాణం ext2_dx_మూలం_info *) ((అక్షరం *)info->చట్రాలు[0].buf +
					     EXT2_DX_మూలం_OFF);
	hash_alg = మూలం->hash_సంస్కరణ;
	ఒకవేళ (hash_alg != EXT2_HASH_TEA && hash_alg != EXT2_HASH_HALF_MD4 &&
	    hash_alg != EXT2_HASH_LEGACY) {
		errcode = EXT2_ET_DIRHASH_UNSUPP;
		వెళ్ళు బహిర్గతం_err;
	}
	ఒకవేళ (hash_alg <= EXT2_HASH_TEA &&
	    fs->super->s_flags & EXT2_FLAGS_UNSIGNED_HASH)
		hash_alg += 3;
	ఒకవేళ (మూలం->పరోక్ష_స్థాయిలు >= ext2_dir_htree_level(fs)) {
		errcode = EXT2_ET_DIR_CORRUPTED;
		వెళ్ళు బహిర్గతం_err;
	}
	info->hash_alg = hash_alg;

	errcode = ext2fs_dirhash2(hash_alg, info->name, info->namelen,
				  fs->encoding, hash_flags,
				  fs->super->s_hash_విత్తనం, &info->hash,
				  &minor_hash);
	ఒకవేళ (errcode)
		వెళ్ళు బహిర్గతం_err;

	కొరకు (level = 0; level <= మూలం->పరోక్ష_స్థాయిలు; level++) {
		చట్రం = &(info->చట్రాలు[level]);
		ఒకవేళ (level > 0) {
			errcode = alloc_dx_చట్రం(fs, చట్రం);
			ఒకవేళ (errcode)
				వెళ్ళు బహిర్గతం_err;
			info->స్థాయిలు++;

			errcode = ఎక్కించు_logical_dir_ఖండం(fs, dir, diri,
				ext2fs_le32_గమ్యం_cpu(info->చట్రాలు[level-1].at->ఖండం) & 0x0fffffff,
				&(చట్రం->pblock), చట్రం->buf);
			ఒకవేళ (errcode)
				వెళ్ళు బహిర్గతం_err;
		}
		errcode = ext2fs_get_dx_countlimit(fs, చట్రం->buf,
						   &(చట్రం->తల), ఖాళీ);
		ఒకవేళ (errcode)
			వెళ్ళు బహిర్గతం_err;
		లెక్క = ext2fs_le16_గమ్యం_cpu(చట్రం->తల->లెక్క);
		హద్దు = ext2fs_le16_గమ్యం_cpu(చట్రం->తల->హద్దు);
		చట్రం->entries = (నిర్మాణం ext2_dx_నమోదు *)(చట్రం->తల);
		ఒకవేళ (!లెక్క || లెక్క > హద్దు) {
			errcode = EXT2_ET_DIR_CORRUPTED;
			వెళ్ళు బహిర్గతం_err;
		}

		dx_శోధన_నమోదు(చట్రం, లెక్క, info->hash);
	}
	తిరిగి 0;
బహిర్గతం_err:
	dx_release(info);
	తిరిగి errcode;
}

నిర్మాణం link_struct  {
	ext2_filsys	fs;
	స్థిరరాశి అక్షరం	*name;
	పూర్ణాంకం		namelen;
	ext2_ino_t	inode;
	పూర్ణాంకం		flags;
	పూర్ణాంకం		పూర్తి;
	గుర్తులేని పూర్ణాంకం	blocksize;
	errcode_t	err;
	నిర్మాణం ext2_super_ఖండం *sb;
};

స్థిర పూర్ణాంకం link_proc(ext2_ino_t dir EXT2FS_ATTR((వాడని)),
		     పూర్ణాంకం entru EXT2FS_ATTR((వాడని)),
		     నిర్మాణం ext2_dir_నమోదు *dirent,
		     పూర్ణాంకం	స్థానభ్రంశం,
		     పూర్ణాంకం	blocksize,
		     అక్షరం	*buf,
		     శూన్య	*priv_దత్తాంశం)
{
	నిర్మాణం link_struct *ls = (నిర్మాణం link_struct *) priv_దత్తాంశం;
	నిర్మాణం ext2_dir_నమోదు *తదుపరి;
	గుర్తులేని పూర్ణాంకం rec_len, min_rec_len, curr_rec_len;
	పూర్ణాంకం ret = 0;
	పూర్ణాంకం csum_కొలత = 0;

	ఒకవేళ (ls->పూర్తి)
		తిరిగి DIRENT_ABORT;

	rec_len = EXT2_DIR_REC_LEN(ls->namelen);

	ls->err = ext2fs_get_rec_len(ls->fs, dirent, &curr_rec_len);
	ఒకవేళ (ls->err)
		తిరిగి DIRENT_ABORT;

	ఒకవేళ (ext2fs_has_feature_metadata_csum(ls->fs->super))
		csum_కొలత = పరిమాణం(నిర్మాణం ext2_dir_నమోదు_తోక);
	/*
	 * See if the following directory entry (if any) is unused;
	 * if so, absorb it into this one.
	 */
	తదుపరి = (నిర్మాణం ext2_dir_నమోదు *) (buf + స్థానభ్రంశం + curr_rec_len);
	ఒకవేళ ((స్థానభ్రంశం + (పూర్ణాంకం) curr_rec_len < blocksize - (8 + csum_కొలత)) &&
	    (తదుపరి->inode == 0) &&
	    (స్థానభ్రంశం + (పూర్ణాంకం) curr_rec_len + (పూర్ణాంకం) తదుపరి->rec_len <= blocksize)) {
		curr_rec_len += తదుపరి->rec_len;
		ls->err = ext2fs_సమితి_rec_len(ls->fs, curr_rec_len, dirent);
		ఒకవేళ (ls->err)
			తిరిగి DIRENT_ABORT;
		ret = DIRENT_మారింది;
	}

	/*
	 * If the directory entry is used, see if we can split the
	 * directory entry to make room for the new name.  If so,
	 * truncate it and return.
	 */
	ఒకవేళ (dirent->inode) {
		min_rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(dirent));
		ఒకవేళ (curr_rec_len < (min_rec_len + rec_len))
			తిరిగి ret;
		rec_len = curr_rec_len - min_rec_len;
		ls->err = ext2fs_సమితి_rec_len(ls->fs, min_rec_len, dirent);
		ఒకవేళ (ls->err)
			తిరిగి DIRENT_ABORT;
		తదుపరి = (నిర్మాణం ext2_dir_నమోదు *) (buf + స్థానభ్రంశం +
						  dirent->rec_len);
		తదుపరి->inode = 0;
		ext2fs_dirent_సమితి_name_len(తదుపరి, 0);
		ext2fs_dirent_సమితి_file_type(తదుపరి, 0);
		ls->err = ext2fs_సమితి_rec_len(ls->fs, rec_len, తదుపరి);
		ఒకవేళ (ls->err)
			తిరిగి DIRENT_ABORT;
		తిరిగి DIRENT_మారింది;
	}

	/*
	 * If we get this far, then the directory entry is not used.
	 * See if we can fit the request entry in.  If so, do it.
	 */
	ఒకవేళ (curr_rec_len < rec_len)
		తిరిగి ret;
	dirent->inode = ls->inode;
	ext2fs_dirent_సమితి_name_len(dirent, ls->namelen);
	strncpy(dirent->name, ls->name, ls->namelen);
	ఒకవేళ (ext2fs_has_feature_filetype(ls->sb))
		ext2fs_dirent_సమితి_file_type(dirent,
					    ls->flags & EXT2_FS_LINK_FT_ముసుగు);

	ls->పూర్తి++;
	తిరిగి DIRENT_ABORT|DIRENT_మారింది;
}

స్థిర errcode_t కూడు_dirent_గమ్యం_buf(ext2_filsys fs, e2_blkcnt_t blockcnt,
				   అక్షరం *buf, ext2_ino_t dir,
				   నిర్మాణం ext2_inode *diri, స్థిరరాశి అక్షరం *name,
				   ext2_ino_t ino, పూర్ణాంకం flags, blk64_t *pblkp)
{
	నిర్మాణం dir_context ctx;
	నిర్మాణం link_struct ls;
	errcode_t retval;

	retval = ఎక్కించు_logical_dir_ఖండం(fs, dir, diri, blockcnt, pblkp, buf);
	ఒకవేళ (retval)
		తిరిగి retval;
	ctx.errcode = 0;
	ctx.func = link_proc;
	ctx.dir = dir;
	ctx.flags = DIRENT_FLAG_INCLUDE_ఖాళీలు;
	ctx.buf = buf;
	ctx.priv_దత్తాంశం = &ls;

	ls.fs = fs;
	ls.name = name;
	ls.namelen = అక్షరశ్రేణి_పొడవు(name);
	ls.inode = ino;
	ls.flags = flags;
	ls.పూర్తి = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	ext2fs_process_dir_ఖండం(fs, pblkp, blockcnt, 0, 0, &ctx);
	ఒకవేళ (ctx.errcode)
		తిరిగి ctx.errcode;
	ఒకవేళ (ls.err)
		తిరిగి ls.err;
	ఒకవేళ (!ls.పూర్తి)
		తిరిగి EXT2_ET_DIR_NO_SPACE;
	తిరిగి 0;
}

నిర్మాణం dx_hash_map {
	__u32 hash;
	పూర్ణాంకం కొలత;
	పూర్ణాంకం off;
};

స్థిర EXT2_QSORT_TYPE dx_hash_map_cmp(స్థిరరాశి శూన్య *ap, స్థిరరాశి శూన్య *bp)
{
	స్థిరరాశి నిర్మాణం dx_hash_map *a = ap, *b = bp;

	ఒకవేళ (a->hash < b->hash)
		తిరిగి -1;
	ఒకవేళ (a->hash > b->hash)
		తిరిగి 1;
	తిరిగి 0;
}

స్థిర errcode_t dx_move_dirents(ext2_filsys fs, నిర్మాణం dx_hash_map *map,
				 పూర్ణాంకం లెక్క, శూన్య *from, శూన్య *గమ్యం)
{
	నిర్మాణం ext2_dir_నమోదు *de;
	పూర్ణాంకం i;
	పూర్ణాంకం rec_len = 0;
	errcode_t retval;
	పూర్ణాంకం csum_కొలత = 0;
	శూన్య *పునాది = గమ్యం;

	ఒకవేళ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_కొలత = పరిమాణం(నిర్మాణం ext2_dir_నమోదు_తోక);

	కొరకు (i = 0; i < లెక్క; i++) {
		de = (నిర్మాణం ext2_dir_నమోదు *) ((అక్షరం *)from + map[i].off);
		rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(de));
		జ్ఞాపకం_నకలు(గమ్యం, de, rec_len);
		retval = ext2fs_సమితి_rec_len(fs, rec_len, గమ్యం);
		ఒకవేళ (retval)
			తిరిగి retval;
		గమ్యం = (అక్షరం *)గమ్యం + rec_len;
	}
	/*
	 * Update rec_len of the last dir entry to stretch to the end of block
	 */
	గమ్యం = (అక్షరం *)గమ్యం - rec_len;
	rec_len = fs->blocksize - ((అక్షరం *)గమ్యం - (అక్షరం *)పునాది) - csum_కొలత;
	retval = ext2fs_సమితి_rec_len(fs, rec_len, గమ్యం);
	ఒకవేళ (retval)
		తిరిగి retval;
	ఒకవేళ (csum_కొలత)
		ext2fs_initialize_dirent_తోక(fs,
				EXT2_DIRENT_తోక(పునాది, fs->blocksize));
	తిరిగి 0;
}

స్థిర errcode_t dx_చొప్పించు_నమోదు(ext2_filsys fs, ext2_ino_t dir,
				 నిర్మాణం dx_lookup_info *info, పూర్ణాంకం level,
				 __u32 hash, blk64_t lblk)
{
	పూర్ణాంకం pcount;
	నిర్మాణం ext2_dx_నమోదు *శిఖరం, *new;

	pcount = ext2fs_le16_గమ్యం_cpu(info->చట్రాలు[level].తల->లెక్క);
	శిఖరం = info->చట్రాలు[level].entries + pcount;
	new = info->చట్రాలు[level].at + 1;
	memmove(new + 1, new, (అక్షరం *)శిఖరం - (అక్షరం *)new);
	new->hash = ext2fs_cpu_గమ్యం_le32(hash);
	new->ఖండం = ext2fs_cpu_గమ్యం_le32(lblk);
	info->చట్రాలు[level].తల->లెక్క = ext2fs_cpu_గమ్యం_le16(pcount + 1);
	తిరిగి ext2fs_write_dir_block4(fs, info->చట్రాలు[level].pblock,
				       info->చట్రాలు[level].buf, 0, dir);
}

స్థిర errcode_t dx_విభజించు_ఆకు(ext2_filsys fs, ext2_ino_t dir,
			       నిర్మాణం ext2_inode *diri,
			       నిర్మాణం dx_lookup_info *info, శూన్య *buf,
			       blk64_t ఆకు_pblk, blk64_t new_lblk,
			       blk64_t new_pblk)
{
	పూర్ణాంకం hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	నిర్మాణం ext2_dir_నమోదు *de;
	శూన్య *buf2;
	errcode_t retval = 0;
	గుర్తులేని పూర్ణాంకం rec_len;
	గుర్తులేని పూర్ణాంకం స్థానభ్రంశం, move_కొలత;
	పూర్ణాంకం i, లెక్క = 0;
	నిర్మాణం dx_hash_map *map;
	పూర్ణాంకం continued;
	__u32 minor_hash;

	retval = ext2fs_get_mem(fs->blocksize, &buf2);
	ఒకవేళ (retval)
		తిరిగి retval;
	retval = ext2fs_get_array(fs->blocksize / 12,
				  పరిమాణం(నిర్మాణం dx_hash_map), &map);
	ఒకవేళ (retval) {
		ext2fs_free_mem(&buf2);
		తిరిగి retval;
	}
	కొరకు (స్థానభ్రంశం = 0; స్థానభ్రంశం < fs->blocksize; స్థానభ్రంశం += rec_len) {
		de = (నిర్మాణం ext2_dir_నమోదు *) ((అక్షరం *)buf + స్థానభ్రంశం);
		retval = ext2fs_get_rec_len(fs, de, &rec_len);
		ఒకవేళ (retval)
			వెళ్ళు బహిర్గతం;
		ఒకవేళ (ext2fs_dirent_name_len(de) > 0 && de->inode) {
			map[లెక్క].off = స్థానభ్రంశం;
			map[లెక్క].కొలత = rec_len;
			retval = ext2fs_dirhash2(info->hash_alg, de->name,
					ext2fs_dirent_name_len(de),
					fs->encoding, hash_flags,
					fs->super->s_hash_విత్తనం,
					&(map[లెక్క].hash),
					&minor_hash);
			ఒకవేళ (retval)
				వెళ్ళు బహిర్గతం;
			లెక్క++;
		}
	}
	qsort(map, లెక్క, పరిమాణం(నిర్మాణం dx_hash_map), dx_hash_map_cmp);
	move_కొలత = 0;
	/* Find place to split block */
	కొరకు (i = లెక్క - 1; i >= 0; i--) {
		ఒకవేళ (move_కొలత + map[i].కొలత / 2 > fs->blocksize / 2)
			ఆపు;
		move_కొలత += map[i].కొలత;
	}
	/* Let i be the first entry to move */
	i++;
	/* Move selected directory entries to new block */
	retval = dx_move_dirents(fs, map + i, లెక్క - i, buf, buf2);
	ఒకవేళ (retval)
		వెళ్ళు బహిర్గతం;
	retval = ext2fs_write_dir_block4(fs, new_pblk, buf2, 0, dir);
	ఒకవేళ (retval)
		వెళ్ళు బహిర్గతం;
	/* Repack remaining entries in the old block */
	retval = dx_move_dirents(fs, map, i, buf, buf2);
	ఒకవేళ (retval)
		వెళ్ళు బహిర్గతం;
	retval = ext2fs_write_dir_block4(fs, ఆకు_pblk, buf2, 0, dir);
	ఒకవేళ (retval)
		వెళ్ళు బహిర్గతం;
	/* Update parent node */
	continued = map[i].hash == map[i-1].hash;
	retval = dx_చొప్పించు_నమోదు(fs, dir, info, info->స్థాయిలు - 1,
				 map[i].hash + continued, new_lblk);
బహిర్గతం:
	ext2fs_free_mem(&buf2);
	ext2fs_free_mem(&map);
	తిరిగి retval;
}

స్థిర errcode_t dx_grow_వృక్షం(ext2_filsys fs, ext2_ino_t dir,
			      నిర్మాణం ext2_inode *diri,
			      నిర్మాణం dx_lookup_info *info, శూన్య *buf,
			      blk64_t ఆకు_pblk)
{
	పూర్ణాంకం i;
	errcode_t retval;
	ext2_off64_t కొలత = EXT2_I_కొలత(diri);
	blk64_t lblk, pblk;
	నిర్మాణం ext2_dir_నమోదు *de;
	నిర్మాణం ext2_dx_countlimit *తల;
	పూర్ణాంకం csum_కొలత = 0;
	పూర్ణాంకం లెక్క;

	ఒకవేళ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_కొలత = పరిమాణం(నిర్మాణం ext2_dx_తోక);

	/* Find level which can accommodate new child */
	కొరకు (i = info->స్థాయిలు - 1; i >= 0; i--)
		ఒకవేళ (ext2fs_le16_గమ్యం_cpu(info->చట్రాలు[i].తల->లెక్క) <
		    ext2fs_le16_గమ్యం_cpu(info->చట్రాలు[i].తల->హద్దు))
			ఆపు;
	/* Need to grow tree depth? */
	ఒకవేళ (i < 0 && info->స్థాయిలు >= ext2_dir_htree_level(fs))
		తిరిగి EXT2_ET_DIR_NO_SPACE;
	lblk = కొలత / fs->blocksize;
	కొలత += fs->blocksize;
	retval = ext2fs_inode_కొలత_సమితి(fs, diri, కొలత);
	ఒకవేళ (retval)
		తిరిగి retval;
	retval = ext2fs_fallocate(fs,
			EXT2_FALLOCATE_FORCE_INIT | EXT2_FALLOCATE_ZERO_ఖండాలు,
			dir, diri, 0, lblk, 1);
	ఒకవేళ (retval)
		తిరిగి retval;
	retval = ext2fs_write_inode(fs, dir, diri);
	ఒకవేళ (retval)
		తిరిగి retval;
	retval = ext2fs_bmap2(fs, dir, diri, ఖాళీ, 0, lblk, ఖాళీ, &pblk);
	ఒకవేళ (retval)
		తిరిగి retval;
	/* Only leaf addition needed? */
	ఒకవేళ (i == (పూర్ణాంకం)info->స్థాయిలు - 1)
		తిరిగి dx_విభజించు_ఆకు(fs, dir, diri, info, buf, ఆకు_pblk,
				     lblk, pblk);

	de = buf;
	de->inode = 0;
	ext2fs_dirent_సమితి_name_len(de, 0);
	ext2fs_dirent_సమితి_file_type(de, 0);
	retval = ext2fs_సమితి_rec_len(fs, fs->blocksize, de);
	ఒకవేళ (retval)
		తిరిగి retval;
	తల = (నిర్మాణం ext2_dx_countlimit *) ((అక్షరం *)buf + 8);
	లెక్క = ext2fs_le16_గమ్యం_cpu(info->చట్రాలు[i+1].తల->లెక్క);
	/* Growing tree depth? */
	ఒకవేళ (i < 0) {
		నిర్మాణం ext2_dx_మూలం_info *మూలం;

		జ్ఞాపకం_నకలు(తల, info->చట్రాలు[0].entries,
		       లెక్క * పరిమాణం(నిర్మాణం ext2_dx_నమోదు));
		తల->హద్దు = ext2fs_cpu_గమ్యం_le16(
				(fs->blocksize - (8 + csum_కొలత)) /
				పరిమాణం(నిర్మాణం ext2_dx_నమోదు));
		/* head->count gets set by memcpy above to correct value */

		/* Now update tree root */
		info->చట్రాలు[0].తల->లెక్క = ext2fs_cpu_గమ్యం_le16(1);
		info->చట్రాలు[0].entries[0].ఖండం = ext2fs_cpu_గమ్యం_le32(lblk);
		మూలం = (నిర్మాణం ext2_dx_మూలం_info *)
			((అక్షరం *)info->చట్రాలు[0].buf + EXT2_DX_మూలం_OFF);
		మూలం->పరోక్ష_స్థాయిలు++;
	} లేకపోతే {
		/* Splitting internal node in two */
		పూర్ణాంకం count1 = లెక్క / 2;
		పూర్ణాంకం count2 = లెక్క - count1;
		__u32 విభజించు_hash = ext2fs_le32_గమ్యం_cpu(info->చట్రాలు[i+1].entries[count1].hash);

		జ్ఞాపకం_నకలు(తల, info->చట్రాలు[i+1].entries + count1,
		       count2 * పరిమాణం(నిర్మాణం ext2_dx_నమోదు));
		తల->లెక్క = ext2fs_cpu_గమ్యం_le16(count2);
		తల->హద్దు = ext2fs_cpu_గమ్యం_le16(
				(fs->blocksize - (8 + csum_కొలత)) /
				పరిమాణం(నిర్మాణం ext2_dx_నమోదు));
		info->చట్రాలు[i+1].తల->లెక్క = ext2fs_cpu_గమ్యం_le16(count1);

		/* Update parent node */
		retval = dx_చొప్పించు_నమోదు(fs, dir, info, i, విభజించు_hash, lblk);
		ఒకవేళ (retval)
			తిరిగి retval;

	}
	/* Writeout split block / updated root */
	retval = ext2fs_write_dir_block4(fs, info->చట్రాలు[i+1].pblock,
					 info->చట్రాలు[i+1].buf, 0, dir);
	ఒకవేళ (retval)
		తిరిగి retval;
	/* Writeout new tree block */
	retval = ext2fs_write_dir_block4(fs, pblk, buf, 0, dir);
	ఒకవేళ (retval)
		తిరిగి retval;
	తిరిగి 0;
}

స్థిర errcode_t dx_link(ext2_filsys fs, ext2_ino_t dir,
			 నిర్మాణం ext2_inode *diri, స్థిరరాశి అక్షరం *name,
			 ext2_ino_t ino, పూర్ణాంకం flags)
{
	నిర్మాణం dx_lookup_info dx_info;
	errcode_t retval;
	శూన్య *blockbuf;
	గుర్తులేని restart = 0;
	blk64_t ఆకు_pblk;

	retval = ext2fs_get_mem(fs->blocksize, &blockbuf);
	ఒకవేళ (retval)
		తిరిగి retval;

	dx_info.name = name;
	dx_info.namelen = అక్షరశ్రేణి_పొడవు(name);
again:
	retval = dx_lookup(fs, dir, diri, &dx_info);
	ఒకవేళ (retval)
		వెళ్ళు free_buf;

	retval = కూడు_dirent_గమ్యం_buf(fs,
		ext2fs_le32_గమ్యం_cpu(dx_info.చట్రాలు[dx_info.స్థాయిలు-1].at->ఖండం) & 0x0fffffff,
		blockbuf, dir, diri, name, ino, flags, &ఆకు_pblk);
	/*
	 * Success or error other than ENOSPC...? We are done. We may need upto
	 * two tries to add entry. One to split htree node and another to add
	 * new leaf block.
	 */
	ఒకవేళ (restart >= dx_info.స్థాయిలు || retval != EXT2_ET_DIR_NO_SPACE)
		వెళ్ళు free_చట్రాలు;
	retval = dx_grow_వృక్షం(fs, dir, diri, &dx_info, blockbuf, ఆకు_pblk);
	ఒకవేళ (retval)
		వెళ్ళు free_చట్రాలు;
	/* Restart everything now that the tree is larger */
	restart++;
	dx_release(&dx_info);
	వెళ్ళు again;
free_చట్రాలు:
	dx_release(&dx_info);
free_buf:
	ext2fs_free_mem(&blockbuf);
	తిరిగి retval;
}

నిర్మాణం fake_dirent
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

నిర్మాణం dx_మూలం
{
	నిర్మాణం fake_dirent dot;
	అక్షరం dot_name[4];
	నిర్మాణం fake_dirent dotdot;
	అక్షరం dotdot_name[4];
	నిర్మాణం ext2_dx_మూలం_info info;
	నిర్మాణం ext2_dx_నమోదు entries[];
};

స్థిర పూర్ణాంకం check_dx_మూలం(ext2_filsys fs, ext2_ino_t ino, నిర్మాణం dx_మూలం *మూలం)
{
	నిర్మాణం fake_dirent *fde;
	అక్షరం *error_సందేశం_2;
	గుర్తులేని పూర్ణాంకం rlen;
	అక్షరం *blockend = (అక్షరం *)మూలం + fs->blocksize;

	fde = &మూలం->dot;
	ఒకవేళ (fde->name_len != 1) {
		error_సందేశం_2 = "invalid name_len for '.'";
		వెళ్ళు corrupted;
	}
	ఒకవేళ (అక్షరశ్రేణి_పోలిక(మూలం->dot_name, ".", fde->name_len)) {
		error_సందేశం_2 = "invalid name for '.'";
		వెళ్ళు corrupted;
	}

	ext2fs_get_rec_len(fs, (నిర్మాణం ext2_dir_నమోదు *)fde, &rlen);
	ఒకవేళ ((అక్షరం *)fde + rlen >= blockend) {
		error_సందేశం_2 = "invalid rec_len for '.'";
		వెళ్ళు corrupted;
	}

	fde = &మూలం->dotdot;
	ఒకవేళ (fde->name_len != 2) {
		error_సందేశం_2 = "invalid name_len for '..'";
		వెళ్ళు corrupted;
	}
	ఒకవేళ (అక్షరశ్రేణి_పోలిక(మూలం->dotdot_name, "..", fde->name_len)) {
		error_సందేశం_2 = "invalid name for '..'";
		వెళ్ళు corrupted;
	}
	ext2fs_get_rec_len(fs, (నిర్మాణం ext2_dir_నమోదు *)fde, &rlen);
	ఒకవేళ ((అక్షరం *)fde + rlen >= blockend) {
		error_సందేశం_2 = "invalid rec_len for '..'";
		వెళ్ళు corrupted;
	}

	తిరిగి 1;

corrupted:
	fprintf(stderr, "Corrupt dir %u: %s, running e2fsck is recommended\n",
			 ino, error_సందేశం_2);
	తిరిగి 0;
}

స్థిర అంతఃవరుస నిర్మాణం ext2_dir_నమోదు *
ext2fs_తదుపరి_నమోదు(ext2_filsys fs, నిర్మాణం ext2_dir_నమోదు *p)
{
	గుర్తులేని పూర్ణాంకం rlen;

	ext2fs_get_rec_len(fs, p, &rlen);
	తిరిగి (నిర్మాణం ext2_dir_నమోదు *)((అక్షరం *)p + rlen);
}

స్థిర అంతఃవరుస శూన్య dx_సమితి_ఖండం(నిర్మాణం ext2_dx_నమోదు *నమోదు_2, గుర్తులేని విలువ)
{
	నమోదు_2->ఖండం = ext2fs_cpu_గమ్యం_le32(విలువ);
}

స్థిర అంతఃవరుస శూన్య dx_సమితి_లెక్క(నిర్మాణం ext2_dx_నమోదు *entries, గుర్తులేని విలువ)
{
	((నిర్మాణం ext2_dx_countlimit *) entries)->లెక్క = ext2fs_cpu_గమ్యం_le16(విలువ);
}

స్థిర అంతఃవరుస శూన్య dx_సమితి_హద్దు(నిర్మాణం ext2_dx_నమోదు *entries, గుర్తులేని విలువ)
{
	((నిర్మాణం ext2_dx_countlimit *) entries)->హద్దు = ext2fs_cpu_గమ్యం_le16(విలువ);
}

స్థిర అంతఃవరుస గుర్తులేని dx_మూలం_హద్దు(ext2_filsys fs, గుర్తులేని infosize)
{
	గుర్తులేని పూర్ణాంకం నమోదు_space = fs->blocksize -
			ext2fs_dir_rec_len(1, 0) -
			ext2fs_dir_rec_len(2, 0) - infosize;

	ఒకవేళ (ext2fs_has_feature_metadata_csum(fs->super))
		నమోదు_space -= పరిమాణం(నిర్మాణం ext2_dx_తోక);
	తిరిగి నమోదు_space / పరిమాణం(నిర్మాణం ext2_dx_నమోదు);
}

/*
 * This converts a one block unindexed directory to a 3 block indexed
 * directory, and adds the dentry to the indexed directory.  Returns 0 if the
 * index was not created; EAGAIN if it was; or an errcode_t on error.
 */
స్థిర errcode_t try_make_సూచీకృత_dir(ext2_filsys fs, ext2_ino_t dir,
				      స్థిరరాశి అక్షరం *name, ext2_ino_t ino,
				      పూర్ణాంకం flags)
{
	నిర్మాణం ext2_inode inode;
	అక్షరం *buf0, *buf1, *శిఖరం;
	నిర్మాణం dx_మూలం *మూలం;
	నిర్మాణం ext2_dx_నమోదు *entries;
	నిర్మాణం fake_dirent *fde;
	నిర్మాణం ext2_dir_నమోదు *de, *de2;
	blk64_t pblk0, pblk1;
	గుర్తులేని పూర్ణాంకం len;
	స్థిరరాశి గుర్తులేని పూర్ణాంకం blocksize = fs->blocksize;
	పూర్ణాంకం csum_కొలత = 0;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	ఒకవేళ (retval)
		తిరిగి retval;

	ఒకవేళ (inode.i_కొలత > fs->blocksize || (inode.i_flags & EXT2_సూచీ_FL))
		తిరిగి 0;

	ఒకవేళ (ext2fs_has_feature_metadata_csum(fs->super))
		csum_కొలత = పరిమాణం(నిర్మాణం ext2_dir_నమోదు_తోక);

	retval = ext2fs_get_mem(blocksize, &buf0);
	ఒకవేళ (retval)
		తిరిగి retval;

	retval = ext2fs_get_mem(blocksize, &buf1);
	ఒకవేళ (retval)
		వెళ్ళు బహిర్గతం_buf0;

	retval = ext2fs_bmap2(fs, dir, &inode, ఖాళీ, 0, 0, ఖాళీ, &pblk0);
	ఒకవేళ (retval)
		వెళ్ళు బహిర్గతం_buf1;

	retval = ext2fs_read_dir_block4(fs, pblk0, buf0, 0, dir);
	ఒకవేళ (retval)
		వెళ్ళు బహిర్గతం_buf1;

	మూలం = (నిర్మాణం dx_మూలం *)buf0;
	ఒకవేళ (!check_dx_మూలం(fs, dir, మూలం)) {
		retval = EXT2_ET_DIR_CORRUPTED;
		వెళ్ళు బహిర్గతం_buf1;
	}

	/* The 0th block becomes the root, move the dirents out */
	fde = &మూలం->dotdot;
	de = ext2fs_తదుపరి_నమోదు(fs, (నిర్మాణం ext2_dir_నమోదు *)fde);
	len = ((అక్షరం *) మూలం) + (blocksize - csum_కొలత) - (అక్షరం *) de;

	/* Allocate new block for the 0th block's dirents */
	retval = ext2fs_bmap2(fs, dir, &inode, ఖాళీ, BMAP_ALLOC | BMAP_ZERO, 1,
			      ఖాళీ, &pblk1);
	ఒకవేళ (retval)
		వెళ్ళు బహిర్గతం_buf1;

	జ్ఞాపకం_నకలు(buf1, de, len);
	memset(de, 0, len); /* wipe old data */

	de = (నిర్మాణం ext2_dir_నమోదు *)buf1;
	శిఖరం = buf1 + len;
	అయితే ((అక్షరం *)(de2 = ext2fs_తదుపరి_నమోదు(fs, de)) < శిఖరం) {
#ఒకవేళ 0
		ఒకవేళ (ext4_check_dir_నమోదు(dir, ఖాళీ, de, bh2, buf1, len,
					(అక్షరం *)de - buf1)) {
			retval = EXT2_ET_DIR_CORRUPTED;
			వెళ్ళు బహిర్గతం_buf1;
		}
#అయితే_ముగింపు
		de = de2;
	}
	retval = ext2fs_సమితి_rec_len(fs,
			buf1 + (blocksize - csum_కొలత) - (అక్షరం *) de, de);
	ఒకవేళ (retval)
		వెళ్ళు బహిర్గతం_buf1;

	ఒకవేళ (csum_కొలత)
		ext2fs_initialize_dirent_తోక(fs,
				EXT2_DIRENT_తోక(buf1, fs->blocksize));

	/* Initialize the root; the dot dirents already exist */
	de = (నిర్మాణం ext2_dir_నమోదు *) (&మూలం->dotdot);
	retval = ext2fs_సమితి_rec_len(fs, blocksize - ext2fs_dir_rec_len(2, 0),
				    de);
	memset (&మూలం->info, 0, పరిమాణం(మూలం->info));
	మూలం->info.info_నిడివి = పరిమాణం(మూలం->info);
	ఒకవేళ (ext4_hash_లోపల_dirent(&inode))
		మూలం->info.hash_సంస్కరణ = EXT2_HASH_SIPHASH;
	లేకపోతే
		మూలం->info.hash_సంస్కరణ = fs->super->s_def_hash_సంస్కరణ;

	entries = మూలం->entries;
	dx_సమితి_ఖండం(entries, 1);
	dx_సమితి_లెక్క(entries, 1);
	dx_సమితి_హద్దు(entries, dx_మూలం_హద్దు(fs, పరిమాణం(మూలం->info)));

	retval = ext2fs_write_dir_block4(fs, pblk1, buf1, 0, dir);
	ఒకవేళ (retval)
		వెళ్ళు బహిర్గతం_buf1;

	retval = ext2fs_write_dir_block4(fs, pblk0, buf0, 0, dir);
	ఒకవేళ (retval)
		వెళ్ళు బహిర్గతం_buf1;

	inode.i_flags |= EXT2_సూచీ_FL;
	inode.i_కొలత += fs->blocksize;
	retval = ext2fs_write_inode(fs, dir, &inode);
	ఒకవేళ (retval)
		వెళ్ళు బహిర్గతం_buf1;

	retval = EAGAIN;
బహిర్గతం_buf1:
	ext2fs_free_mem(&buf1);
బహిర్గతం_buf0:
	ext2fs_free_mem(&buf0);
	తిరిగి retval;
}

/*
 * Note: the low 3 bits of the flags field are used as the directory
 * entry filetype.
 */
#నిర్వచితమైతే __TURBOC__
 #సూచన argsused
#అయితే_ముగింపు
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, స్థిరరాశి అక్షరం *name,
		      ext2_ino_t ino, పూర్ణాంకం flags)
{
	errcode_t		retval;
	నిర్మాణం link_struct	ls;
	నిర్మాణం ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ (!(fs->flags & EXT2_FLAG_RW))
		తిరిగి EXT2_ET_RO_FILSYS;

retry:
	ఒకవేళ ((retval = ext2fs_read_inode(fs, dir, &inode)) != 0)
		తిరిగి retval;

	ఒకవేళ (inode.i_flags & EXT2_సూచీ_FL)
		తిరిగి dx_link(fs, dir, &inode, name, ino, flags);

	ls.fs = fs;
	ls.name = name;
	ls.namelen = name ? అక్షరశ్రేణి_పొడవు(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.పూర్తి = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	ఒకవేళ ((flags & EXT2FS_LINK_APPEND) &&
	    !(inode.i_flags & EXT4_INLINE_దత్తాంశం_FL)) {
		blk64_t		lblk, pblk = 0;
		నిర్మాణం dir_context ctx;

		lblk = (inode.i_కొలత / fs->blocksize) - 1;
		retval = ext2fs_bmap2(fs, dir, &inode, ఖాళీ, 0, lblk,
				      ఖాళీ, &pblk);
		ఒకవేళ (retval)
			తిరిగి retval;

		ctx.dir = dir;
		ctx.flags = DIRENT_FLAG_INCLUDE_ఖాళీలు;
		ctx.func = link_proc;
		ctx.priv_దత్తాంశం = &ls;
		ctx.errcode = 0;
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		ఒకవేళ (retval)
			తిరిగి retval;

		ext2fs_process_dir_ఖండం(fs, &pblk, lblk, 0, 0, &ctx);
		retval = ctx.errcode;
		ext2fs_free_mem(&ctx.buf);
	} లేకపోతే {
		retval = ext2fs_dir_iterate2(fs, dir,
					     DIRENT_FLAG_INCLUDE_ఖాళీలు,
					     ఖాళీ, link_proc, &ls);
	}
	ఒకవేళ (retval)
		తిరిగి retval;
	ఒకవేళ (ls.err)
		తిరిగి ls.err;

	ఒకవేళ (!ls.పూర్తి) {
		ఒకవేళ (!(flags & EXT2FS_LINK_EXPAND))
			తిరిగి EXT2_ET_DIR_NO_SPACE;

		ఒకవేళ (ext2fs_has_feature_dir_సూచీ(fs->super)) {
			retval = try_make_సూచీకృత_dir(fs, dir, name, ino,
						      flags);
			ఒకవేళ (retval == EAGAIN)
				వెళ్ళు retry;
			ఒకవేళ (retval)
				తిరిగి retval;
		}

		retval = ext2fs_expand_dir(fs, dir);
		ఒకవేళ (retval)
			తిరిగి retval;
		వెళ్ళు retry;
	}
	తిరిగి 0;
}

/* Does this directory have an htree index? */
పూర్ణాంకం ext2fs_dir_is_dx(ext2_filsys fs, స్థిరరాశి నిర్మాణం ext2_inode *inode)
{
	తిరిగి ext2fs_has_feature_dir_సూచీ(fs->super) &&
		S_ISDIR(inode->i_బహుళకం) && (inode->i_flags & EXT2_సూచీ_FL);
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
శూన్య ext2fs_inc_nlink(ext2_filsys fs, నిర్మాణం ext2_inode *inode)
{
	inode->i_links_లెక్క++;

	ఒకవేళ (ext2fs_dir_is_dx(fs, inode) &&
	    (inode->i_links_లెక్క > EXT2_LINK_MAX || inode->i_links_లెక్క == 2))
		inode->i_links_లెక్క = 1;
}

/*
 * If a directory had nlink == 1, then we should let it be 1. This indicates
 * directory has >EXT2_LINK_MAX subdirs.
 */
శూన్య ext2fs_dec_nlink(నిర్మాణం ext2_inode *inode)
{
	ఒకవేళ (!S_ISDIR(inode->i_బహుళకం) || inode->i_links_లెక్క > 2)
		inode->i_links_లెక్క--;
}

పూర్ణాంకం ext2fs_dir_link_max(ext2_filsys fs, నిర్మాణం ext2_inode_large *inode)
{
	తిరిగి inode->i_links_లెక్క >= EXT2_LINK_MAX &&
	       !(ext2fs_dir_is_dx(fs, EXT2_INODE(inode)) &&
		 ext2fs_has_feature_dir_nlink(fs->super));
}

పూర్ణాంకం ext2fs_dir_link_ఖాళీలు(నిర్మాణం ext2_inode *inode)
{
	తిరిగి inode->i_links_లెక్క == 2 || inode->i_links_లెక్క == 1;
}
