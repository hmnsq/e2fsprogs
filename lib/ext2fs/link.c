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
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

#تعریف_کرو EXT2_DX_جڑ_OFF 24

ساخت dx_چوکھٹا {
	خالی *buf;
	blk64_t pblock;
	ساخت ext2_dx_countlimit *سر;
	ساخت ext2_dx_اندراج *entries;
	ساخت ext2_dx_اندراج *at;
};

ساخت dx_lookup_info {
	ثابت حرف *name;
	صحیح namelen;
	صحیح hash_alg;
	__u32 hash;
	غیرعلامتی سطحیں;
	ساخت dx_چوکھٹا چوکھٹے[EXT4_HTREE_LEVEL];
};

ساکن errcode_t alloc_dx_چوکھٹا(ext2_filsys fs, ساخت dx_چوکھٹا *چوکھٹا)
{
	واپس ext2fs_get_mem(fs->blocksize, &چوکھٹا->buf);
}

ساکن خالی dx_release(ساخت dx_lookup_info *info)
{
	غیرعلامتی level;

	برائے (level = 0; level < info->سطحیں; level++) {
		اگر (info->چوکھٹے[level].buf == تہی)
			توڑو;
		ext2fs_free_mem(&(info->چوکھٹے[level].buf));
	}
	info->سطحیں = 0;
}

ساکن خالی dx_تلاش_اندراج(ساخت dx_چوکھٹا *چوکھٹا, صحیح گنتی, __u32 hash)
{
	ساخت ext2_dx_اندراج *p, *q, *m;

	p = چوکھٹا->entries + 1;
	q = چوکھٹا->entries + گنتی - 1;
	جب (p <= q) {
		m = p + (q - p) / 2;
		اگر (ext2fs_le32_مقصد_cpu(m->hash) > hash)
			q = m - 1;
		ورنہ
			p = m + 1;
	}
	چوکھٹا->at = p - 1;
}

ساکن errcode_t بھرو_logical_dir_بلاک(ext2_filsys fs, ext2_ino_t dir,
					ساخت ext2_inode *diri, blk64_t بلاک,
					blk64_t *pblk, خالی *buf)
{
	errcode_t errcode;
	صحیح ret_flags;

	errcode = ext2fs_bmap2(fs, dir, diri, تہی, 0, بلاک, &ret_flags,
			       pblk);
	اگر (errcode)
		واپس errcode;
	اگر (ret_flags & BMAP_RET_UNINIT)
		واپس EXT2_ET_DIR_CORRUPTED;
	واپس ext2fs_read_dir_block4(fs, *pblk, buf, 0, dir);
}

ساکن errcode_t dx_lookup(ext2_filsys fs, ext2_ino_t dir,
			   ساخت ext2_inode *diri, ساخت dx_lookup_info *info)
{
	ساخت ext2_dx_جڑ_info *جڑ;
	errcode_t errcode;
	صحیح level = 0;
	صحیح گنتی, حد_بندی;
	صحیح hash_alg;
	صحیح hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	__u32 minor_hash;
	ساخت dx_چوکھٹا *چوکھٹا;

	errcode = alloc_dx_چوکھٹا(fs, &(info->چوکھٹے[0]));
	اگر (errcode)
		واپس errcode;
	info->سطحیں = 1;

	errcode = بھرو_logical_dir_بلاک(fs, dir, diri, 0,
					 &(info->چوکھٹے[0].pblock),
					 info->چوکھٹے[0].buf);
	اگر (errcode)
		جاؤ برآمد_err;
	جڑ = (ساخت ext2_dx_جڑ_info *) ((حرف *)info->چوکھٹے[0].buf +
					     EXT2_DX_جڑ_OFF);
	hash_alg = جڑ->hash_نسخہ;
	اگر (hash_alg != EXT2_HASH_TEA && hash_alg != EXT2_HASH_HALF_MD4 &&
	    hash_alg != EXT2_HASH_LEGACY) {
		errcode = EXT2_ET_DIRHASH_UNSUPP;
		جاؤ برآمد_err;
	}
	اگر (hash_alg <= EXT2_HASH_TEA &&
	    fs->super->s_flags & EXT2_FLAGS_UNSIGNED_HASH)
		hash_alg += 3;
	اگر (جڑ->بالواسطہ_سطحیں >= ext2_dir_htree_level(fs)) {
		errcode = EXT2_ET_DIR_CORRUPTED;
		جاؤ برآمد_err;
	}
	info->hash_alg = hash_alg;

	errcode = ext2fs_dirhash2(hash_alg, info->name, info->namelen,
				  fs->encoding, hash_flags,
				  fs->super->s_hash_بیج, &info->hash,
				  &minor_hash);
	اگر (errcode)
		جاؤ برآمد_err;

	برائے (level = 0; level <= جڑ->بالواسطہ_سطحیں; level++) {
		چوکھٹا = &(info->چوکھٹے[level]);
		اگر (level > 0) {
			errcode = alloc_dx_چوکھٹا(fs, چوکھٹا);
			اگر (errcode)
				جاؤ برآمد_err;
			info->سطحیں++;

			errcode = بھرو_logical_dir_بلاک(fs, dir, diri,
				ext2fs_le32_مقصد_cpu(info->چوکھٹے[level-1].at->بلاک) & 0x0fffffff,
				&(چوکھٹا->pblock), چوکھٹا->buf);
			اگر (errcode)
				جاؤ برآمد_err;
		}
		errcode = ext2fs_get_dx_countlimit(fs, چوکھٹا->buf,
						   &(چوکھٹا->سر), تہی);
		اگر (errcode)
			جاؤ برآمد_err;
		گنتی = ext2fs_le16_مقصد_cpu(چوکھٹا->سر->گنتی);
		حد_بندی = ext2fs_le16_مقصد_cpu(چوکھٹا->سر->حد_بندی);
		چوکھٹا->entries = (ساخت ext2_dx_اندراج *)(چوکھٹا->سر);
		اگر (!گنتی || گنتی > حد_بندی) {
			errcode = EXT2_ET_DIR_CORRUPTED;
			جاؤ برآمد_err;
		}

		dx_تلاش_اندراج(چوکھٹا, گنتی, info->hash);
	}
	واپس 0;
برآمد_err:
	dx_release(info);
	واپس errcode;
}

ساخت link_struct  {
	ext2_filsys	fs;
	ثابت حرف	*name;
	صحیح		namelen;
	ext2_ino_t	inode;
	صحیح		flags;
	صحیح		مکمل;
	غیرعلامتی صحیح	blocksize;
	errcode_t	err;
	ساخت ext2_super_بلاک *sb;
};

ساکن صحیح link_proc(ext2_ino_t dir EXT2FS_ATTR((غیر_مستعمل)),
		     صحیح entru EXT2FS_ATTR((غیر_مستعمل)),
		     ساخت ext2_dir_اندراج *dirent,
		     صحیح	انحراف,
		     صحیح	blocksize,
		     حرف	*buf,
		     خالی	*priv_ڈیٹا)
{
	ساخت link_struct *ls = (ساخت link_struct *) priv_ڈیٹا;
	ساخت ext2_dir_اندراج *اگلا;
	غیرعلامتی صحیح rec_len, min_rec_len, curr_rec_len;
	صحیح ret = 0;
	صحیح csum_سائز = 0;

	اگر (ls->مکمل)
		واپس DIRENT_ABORT;

	rec_len = EXT2_DIR_REC_LEN(ls->namelen);

	ls->err = ext2fs_get_rec_len(ls->fs, dirent, &curr_rec_len);
	اگر (ls->err)
		واپس DIRENT_ABORT;

	اگر (ext2fs_has_feature_metadata_csum(ls->fs->super))
		csum_سائز = حجم(ساخت ext2_dir_اندراج_دم);
	/*
	 * See if the following directory entry (if any) is unused;
	 * if so, absorb it into this one.
	 */
	اگلا = (ساخت ext2_dir_اندراج *) (buf + انحراف + curr_rec_len);
	اگر ((انحراف + (صحیح) curr_rec_len < blocksize - (8 + csum_سائز)) &&
	    (اگلا->inode == 0) &&
	    (انحراف + (صحیح) curr_rec_len + (صحیح) اگلا->rec_len <= blocksize)) {
		curr_rec_len += اگلا->rec_len;
		ls->err = ext2fs_سیٹ_rec_len(ls->fs, curr_rec_len, dirent);
		اگر (ls->err)
			واپس DIRENT_ABORT;
		ret = DIRENT_بدلا;
	}

	/*
	 * If the directory entry is used, see if we can split the
	 * directory entry to make room for the new name.  If so,
	 * truncate it and return.
	 */
	اگر (dirent->inode) {
		min_rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(dirent));
		اگر (curr_rec_len < (min_rec_len + rec_len))
			واپس ret;
		rec_len = curr_rec_len - min_rec_len;
		ls->err = ext2fs_سیٹ_rec_len(ls->fs, min_rec_len, dirent);
		اگر (ls->err)
			واپس DIRENT_ABORT;
		اگلا = (ساخت ext2_dir_اندراج *) (buf + انحراف +
						  dirent->rec_len);
		اگلا->inode = 0;
		ext2fs_dirent_سیٹ_name_len(اگلا, 0);
		ext2fs_dirent_سیٹ_file_type(اگلا, 0);
		ls->err = ext2fs_سیٹ_rec_len(ls->fs, rec_len, اگلا);
		اگر (ls->err)
			واپس DIRENT_ABORT;
		واپس DIRENT_بدلا;
	}

	/*
	 * If we get this far, then the directory entry is not used.
	 * See if we can fit the request entry in.  If so, do it.
	 */
	اگر (curr_rec_len < rec_len)
		واپس ret;
	dirent->inode = ls->inode;
	ext2fs_dirent_سیٹ_name_len(dirent, ls->namelen);
	strncpy(dirent->name, ls->name, ls->namelen);
	اگر (ext2fs_has_feature_filetype(ls->sb))
		ext2fs_dirent_سیٹ_file_type(dirent,
					    ls->flags & EXT2_FS_LINK_FT_نقاب);

	ls->مکمل++;
	واپس DIRENT_ABORT|DIRENT_بدلا;
}

ساکن errcode_t جوڑو_dirent_مقصد_buf(ext2_filsys fs, e2_blkcnt_t blockcnt,
				   حرف *buf, ext2_ino_t dir,
				   ساخت ext2_inode *diri, ثابت حرف *name,
				   ext2_ino_t ino, صحیح flags, blk64_t *pblkp)
{
	ساخت dir_context ctx;
	ساخت link_struct ls;
	errcode_t retval;

	retval = بھرو_logical_dir_بلاک(fs, dir, diri, blockcnt, pblkp, buf);
	اگر (retval)
		واپس retval;
	ctx.errcode = 0;
	ctx.func = link_proc;
	ctx.dir = dir;
	ctx.flags = DIRENT_FLAG_INCLUDE_خالی_خانے;
	ctx.buf = buf;
	ctx.priv_ڈیٹا = &ls;

	ls.fs = fs;
	ls.name = name;
	ls.namelen = لڑی_لمبائی(name);
	ls.inode = ino;
	ls.flags = flags;
	ls.مکمل = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	ext2fs_process_dir_بلاک(fs, pblkp, blockcnt, 0, 0, &ctx);
	اگر (ctx.errcode)
		واپس ctx.errcode;
	اگر (ls.err)
		واپس ls.err;
	اگر (!ls.مکمل)
		واپس EXT2_ET_DIR_NO_SPACE;
	واپس 0;
}

ساخت dx_hash_map {
	__u32 hash;
	صحیح سائز;
	صحیح off;
};

ساکن EXT2_QSORT_TYPE dx_hash_map_cmp(ثابت خالی *ap, ثابت خالی *bp)
{
	ثابت ساخت dx_hash_map *a = ap, *b = bp;

	اگر (a->hash < b->hash)
		واپس -1;
	اگر (a->hash > b->hash)
		واپس 1;
	واپس 0;
}

ساکن errcode_t dx_move_dirents(ext2_filsys fs, ساخت dx_hash_map *map,
				 صحیح گنتی, خالی *from, خالی *مقصد)
{
	ساخت ext2_dir_اندراج *de;
	صحیح i;
	صحیح rec_len = 0;
	errcode_t retval;
	صحیح csum_سائز = 0;
	خالی *بنیاد = مقصد;

	اگر (ext2fs_has_feature_metadata_csum(fs->super))
		csum_سائز = حجم(ساخت ext2_dir_اندراج_دم);

	برائے (i = 0; i < گنتی; i++) {
		de = (ساخت ext2_dir_اندراج *) ((حرف *)from + map[i].off);
		rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(de));
		حافظہ_نقل(مقصد, de, rec_len);
		retval = ext2fs_سیٹ_rec_len(fs, rec_len, مقصد);
		اگر (retval)
			واپس retval;
		مقصد = (حرف *)مقصد + rec_len;
	}
	/*
	 * Update rec_len of the last dir entry to stretch to the end of block
	 */
	مقصد = (حرف *)مقصد - rec_len;
	rec_len = fs->blocksize - ((حرف *)مقصد - (حرف *)بنیاد) - csum_سائز;
	retval = ext2fs_سیٹ_rec_len(fs, rec_len, مقصد);
	اگر (retval)
		واپس retval;
	اگر (csum_سائز)
		ext2fs_initialize_dirent_دم(fs,
				EXT2_DIRENT_دم(بنیاد, fs->blocksize));
	واپس 0;
}

ساکن errcode_t dx_داخل_کرو_اندراج(ext2_filsys fs, ext2_ino_t dir,
				 ساخت dx_lookup_info *info, صحیح level,
				 __u32 hash, blk64_t lblk)
{
	صحیح pcount;
	ساخت ext2_dx_اندراج *چوٹی, *new;

	pcount = ext2fs_le16_مقصد_cpu(info->چوکھٹے[level].سر->گنتی);
	چوٹی = info->چوکھٹے[level].entries + pcount;
	new = info->چوکھٹے[level].at + 1;
	memmove(new + 1, new, (حرف *)چوٹی - (حرف *)new);
	new->hash = ext2fs_cpu_مقصد_le32(hash);
	new->بلاک = ext2fs_cpu_مقصد_le32(lblk);
	info->چوکھٹے[level].سر->گنتی = ext2fs_cpu_مقصد_le16(pcount + 1);
	واپس ext2fs_write_dir_block4(fs, info->چوکھٹے[level].pblock,
				       info->چوکھٹے[level].buf, 0, dir);
}

ساکن errcode_t dx_بانٹو_برگ(ext2_filsys fs, ext2_ino_t dir,
			       ساخت ext2_inode *diri,
			       ساخت dx_lookup_info *info, خالی *buf,
			       blk64_t برگ_pblk, blk64_t new_lblk,
			       blk64_t new_pblk)
{
	صحیح hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	ساخت ext2_dir_اندراج *de;
	خالی *buf2;
	errcode_t retval = 0;
	غیرعلامتی صحیح rec_len;
	غیرعلامتی صحیح انحراف, move_سائز;
	صحیح i, گنتی = 0;
	ساخت dx_hash_map *map;
	صحیح continued;
	__u32 minor_hash;

	retval = ext2fs_get_mem(fs->blocksize, &buf2);
	اگر (retval)
		واپس retval;
	retval = ext2fs_get_array(fs->blocksize / 12,
				  حجم(ساخت dx_hash_map), &map);
	اگر (retval) {
		ext2fs_free_mem(&buf2);
		واپس retval;
	}
	برائے (انحراف = 0; انحراف < fs->blocksize; انحراف += rec_len) {
		de = (ساخت ext2_dir_اندراج *) ((حرف *)buf + انحراف);
		retval = ext2fs_get_rec_len(fs, de, &rec_len);
		اگر (retval)
			جاؤ برآمد;
		اگر (ext2fs_dirent_name_len(de) > 0 && de->inode) {
			map[گنتی].off = انحراف;
			map[گنتی].سائز = rec_len;
			retval = ext2fs_dirhash2(info->hash_alg, de->name,
					ext2fs_dirent_name_len(de),
					fs->encoding, hash_flags,
					fs->super->s_hash_بیج,
					&(map[گنتی].hash),
					&minor_hash);
			اگر (retval)
				جاؤ برآمد;
			گنتی++;
		}
	}
	qsort(map, گنتی, حجم(ساخت dx_hash_map), dx_hash_map_cmp);
	move_سائز = 0;
	/* Find place to split block */
	برائے (i = گنتی - 1; i >= 0; i--) {
		اگر (move_سائز + map[i].سائز / 2 > fs->blocksize / 2)
			توڑو;
		move_سائز += map[i].سائز;
	}
	/* Let i be the first entry to move */
	i++;
	/* Move selected directory entries to new block */
	retval = dx_move_dirents(fs, map + i, گنتی - i, buf, buf2);
	اگر (retval)
		جاؤ برآمد;
	retval = ext2fs_write_dir_block4(fs, new_pblk, buf2, 0, dir);
	اگر (retval)
		جاؤ برآمد;
	/* Repack remaining entries in the old block */
	retval = dx_move_dirents(fs, map, i, buf, buf2);
	اگر (retval)
		جاؤ برآمد;
	retval = ext2fs_write_dir_block4(fs, برگ_pblk, buf2, 0, dir);
	اگر (retval)
		جاؤ برآمد;
	/* Update parent node */
	continued = map[i].hash == map[i-1].hash;
	retval = dx_داخل_کرو_اندراج(fs, dir, info, info->سطحیں - 1,
				 map[i].hash + continued, new_lblk);
برآمد:
	ext2fs_free_mem(&buf2);
	ext2fs_free_mem(&map);
	واپس retval;
}

ساکن errcode_t dx_grow_درخت(ext2_filsys fs, ext2_ino_t dir,
			      ساخت ext2_inode *diri,
			      ساخت dx_lookup_info *info, خالی *buf,
			      blk64_t برگ_pblk)
{
	صحیح i;
	errcode_t retval;
	ext2_off64_t سائز = EXT2_I_سائز(diri);
	blk64_t lblk, pblk;
	ساخت ext2_dir_اندراج *de;
	ساخت ext2_dx_countlimit *سر;
	صحیح csum_سائز = 0;
	صحیح گنتی;

	اگر (ext2fs_has_feature_metadata_csum(fs->super))
		csum_سائز = حجم(ساخت ext2_dx_دم);

	/* Find level which can accommodate new child */
	برائے (i = info->سطحیں - 1; i >= 0; i--)
		اگر (ext2fs_le16_مقصد_cpu(info->چوکھٹے[i].سر->گنتی) <
		    ext2fs_le16_مقصد_cpu(info->چوکھٹے[i].سر->حد_بندی))
			توڑو;
	/* Need to grow tree depth? */
	اگر (i < 0 && info->سطحیں >= ext2_dir_htree_level(fs))
		واپس EXT2_ET_DIR_NO_SPACE;
	lblk = سائز / fs->blocksize;
	سائز += fs->blocksize;
	retval = ext2fs_inode_سائز_سیٹ(fs, diri, سائز);
	اگر (retval)
		واپس retval;
	retval = ext2fs_fallocate(fs,
			EXT2_FALLOCATE_FORCE_INIT | EXT2_FALLOCATE_ZERO_بلاکیں,
			dir, diri, 0, lblk, 1);
	اگر (retval)
		واپس retval;
	retval = ext2fs_write_inode(fs, dir, diri);
	اگر (retval)
		واپس retval;
	retval = ext2fs_bmap2(fs, dir, diri, تہی, 0, lblk, تہی, &pblk);
	اگر (retval)
		واپس retval;
	/* Only leaf addition needed? */
	اگر (i == (صحیح)info->سطحیں - 1)
		واپس dx_بانٹو_برگ(fs, dir, diri, info, buf, برگ_pblk,
				     lblk, pblk);

	de = buf;
	de->inode = 0;
	ext2fs_dirent_سیٹ_name_len(de, 0);
	ext2fs_dirent_سیٹ_file_type(de, 0);
	retval = ext2fs_سیٹ_rec_len(fs, fs->blocksize, de);
	اگر (retval)
		واپس retval;
	سر = (ساخت ext2_dx_countlimit *) ((حرف *)buf + 8);
	گنتی = ext2fs_le16_مقصد_cpu(info->چوکھٹے[i+1].سر->گنتی);
	/* Growing tree depth? */
	اگر (i < 0) {
		ساخت ext2_dx_جڑ_info *جڑ;

		حافظہ_نقل(سر, info->چوکھٹے[0].entries,
		       گنتی * حجم(ساخت ext2_dx_اندراج));
		سر->حد_بندی = ext2fs_cpu_مقصد_le16(
				(fs->blocksize - (8 + csum_سائز)) /
				حجم(ساخت ext2_dx_اندراج));
		/* head->count gets set by memcpy above to correct value */

		/* Now update tree root */
		info->چوکھٹے[0].سر->گنتی = ext2fs_cpu_مقصد_le16(1);
		info->چوکھٹے[0].entries[0].بلاک = ext2fs_cpu_مقصد_le32(lblk);
		جڑ = (ساخت ext2_dx_جڑ_info *)
			((حرف *)info->چوکھٹے[0].buf + EXT2_DX_جڑ_OFF);
		جڑ->بالواسطہ_سطحیں++;
	} ورنہ {
		/* Splitting internal node in two */
		صحیح count1 = گنتی / 2;
		صحیح count2 = گنتی - count1;
		__u32 بانٹو_hash = ext2fs_le32_مقصد_cpu(info->چوکھٹے[i+1].entries[count1].hash);

		حافظہ_نقل(سر, info->چوکھٹے[i+1].entries + count1,
		       count2 * حجم(ساخت ext2_dx_اندراج));
		سر->گنتی = ext2fs_cpu_مقصد_le16(count2);
		سر->حد_بندی = ext2fs_cpu_مقصد_le16(
				(fs->blocksize - (8 + csum_سائز)) /
				حجم(ساخت ext2_dx_اندراج));
		info->چوکھٹے[i+1].سر->گنتی = ext2fs_cpu_مقصد_le16(count1);

		/* Update parent node */
		retval = dx_داخل_کرو_اندراج(fs, dir, info, i, بانٹو_hash, lblk);
		اگر (retval)
			واپس retval;

	}
	/* Writeout split block / updated root */
	retval = ext2fs_write_dir_block4(fs, info->چوکھٹے[i+1].pblock,
					 info->چوکھٹے[i+1].buf, 0, dir);
	اگر (retval)
		واپس retval;
	/* Writeout new tree block */
	retval = ext2fs_write_dir_block4(fs, pblk, buf, 0, dir);
	اگر (retval)
		واپس retval;
	واپس 0;
}

ساکن errcode_t dx_link(ext2_filsys fs, ext2_ino_t dir,
			 ساخت ext2_inode *diri, ثابت حرف *name,
			 ext2_ino_t ino, صحیح flags)
{
	ساخت dx_lookup_info dx_info;
	errcode_t retval;
	خالی *blockbuf;
	غیرعلامتی restart = 0;
	blk64_t برگ_pblk;

	retval = ext2fs_get_mem(fs->blocksize, &blockbuf);
	اگر (retval)
		واپس retval;

	dx_info.name = name;
	dx_info.namelen = لڑی_لمبائی(name);
again:
	retval = dx_lookup(fs, dir, diri, &dx_info);
	اگر (retval)
		جاؤ free_buf;

	retval = جوڑو_dirent_مقصد_buf(fs,
		ext2fs_le32_مقصد_cpu(dx_info.چوکھٹے[dx_info.سطحیں-1].at->بلاک) & 0x0fffffff,
		blockbuf, dir, diri, name, ino, flags, &برگ_pblk);
	/*
	 * Success or error other than ENOSPC...? We are done. We may need upto
	 * two tries to add entry. One to split htree node and another to add
	 * new leaf block.
	 */
	اگر (restart >= dx_info.سطحیں || retval != EXT2_ET_DIR_NO_SPACE)
		جاؤ free_چوکھٹے;
	retval = dx_grow_درخت(fs, dir, diri, &dx_info, blockbuf, برگ_pblk);
	اگر (retval)
		جاؤ free_چوکھٹے;
	/* Restart everything now that the tree is larger */
	restart++;
	dx_release(&dx_info);
	جاؤ again;
free_چوکھٹے:
	dx_release(&dx_info);
free_buf:
	ext2fs_free_mem(&blockbuf);
	واپس retval;
}

ساخت fake_dirent
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

ساخت dx_جڑ
{
	ساخت fake_dirent dot;
	حرف dot_name[4];
	ساخت fake_dirent dotdot;
	حرف dotdot_name[4];
	ساخت ext2_dx_جڑ_info info;
	ساخت ext2_dx_اندراج entries[];
};

ساکن صحیح check_dx_جڑ(ext2_filsys fs, ext2_ino_t ino, ساخت dx_جڑ *جڑ)
{
	ساخت fake_dirent *fde;
	حرف *error_پیغام_2;
	غیرعلامتی صحیح rlen;
	حرف *blockend = (حرف *)جڑ + fs->blocksize;

	fde = &جڑ->dot;
	اگر (fde->name_len != 1) {
		error_پیغام_2 = "invalid name_len for '.'";
		جاؤ corrupted;
	}
	اگر (لڑی_موازنہ(جڑ->dot_name, ".", fde->name_len)) {
		error_پیغام_2 = "invalid name for '.'";
		جاؤ corrupted;
	}

	ext2fs_get_rec_len(fs, (ساخت ext2_dir_اندراج *)fde, &rlen);
	اگر ((حرف *)fde + rlen >= blockend) {
		error_پیغام_2 = "invalid rec_len for '.'";
		جاؤ corrupted;
	}

	fde = &جڑ->dotdot;
	اگر (fde->name_len != 2) {
		error_پیغام_2 = "invalid name_len for '..'";
		جاؤ corrupted;
	}
	اگر (لڑی_موازنہ(جڑ->dotdot_name, "..", fde->name_len)) {
		error_پیغام_2 = "invalid name for '..'";
		جاؤ corrupted;
	}
	ext2fs_get_rec_len(fs, (ساخت ext2_dir_اندراج *)fde, &rlen);
	اگر ((حرف *)fde + rlen >= blockend) {
		error_پیغام_2 = "invalid rec_len for '..'";
		جاؤ corrupted;
	}

	واپس 1;

corrupted:
	fprintf(stderr, "Corrupt dir %u: %s, running e2fsck is recommended\n",
			 ino, error_پیغام_2);
	واپس 0;
}

ساکن اندرونی ساخت ext2_dir_اندراج *
ext2fs_اگلا_اندراج(ext2_filsys fs, ساخت ext2_dir_اندراج *p)
{
	غیرعلامتی صحیح rlen;

	ext2fs_get_rec_len(fs, p, &rlen);
	واپس (ساخت ext2_dir_اندراج *)((حرف *)p + rlen);
}

ساکن اندرونی خالی dx_سیٹ_بلاک(ساخت ext2_dx_اندراج *اندراج_2, غیرعلامتی قدر)
{
	اندراج_2->بلاک = ext2fs_cpu_مقصد_le32(قدر);
}

ساکن اندرونی خالی dx_سیٹ_گنتی(ساخت ext2_dx_اندراج *entries, غیرعلامتی قدر)
{
	((ساخت ext2_dx_countlimit *) entries)->گنتی = ext2fs_cpu_مقصد_le16(قدر);
}

ساکن اندرونی خالی dx_سیٹ_حد_بندی(ساخت ext2_dx_اندراج *entries, غیرعلامتی قدر)
{
	((ساخت ext2_dx_countlimit *) entries)->حد_بندی = ext2fs_cpu_مقصد_le16(قدر);
}

ساکن اندرونی غیرعلامتی dx_جڑ_حد_بندی(ext2_filsys fs, غیرعلامتی infosize)
{
	غیرعلامتی صحیح اندراج_space = fs->blocksize -
			ext2fs_dir_rec_len(1, 0) -
			ext2fs_dir_rec_len(2, 0) - infosize;

	اگر (ext2fs_has_feature_metadata_csum(fs->super))
		اندراج_space -= حجم(ساخت ext2_dx_دم);
	واپس اندراج_space / حجم(ساخت ext2_dx_اندراج);
}

/*
 * This converts a one block unindexed directory to a 3 block indexed
 * directory, and adds the dentry to the indexed directory.  Returns 0 if the
 * index was not created; EAGAIN if it was; or an errcode_t on error.
 */
ساکن errcode_t try_make_اشاریہ_دار_dir(ext2_filsys fs, ext2_ino_t dir,
				      ثابت حرف *name, ext2_ino_t ino,
				      صحیح flags)
{
	ساخت ext2_inode inode;
	حرف *buf0, *buf1, *چوٹی;
	ساخت dx_جڑ *جڑ;
	ساخت ext2_dx_اندراج *entries;
	ساخت fake_dirent *fde;
	ساخت ext2_dir_اندراج *de, *de2;
	blk64_t pblk0, pblk1;
	غیرعلامتی صحیح len;
	ثابت غیرعلامتی صحیح blocksize = fs->blocksize;
	صحیح csum_سائز = 0;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	اگر (retval)
		واپس retval;

	اگر (inode.i_سائز > fs->blocksize || (inode.i_flags & EXT2_اشاریہ_FL))
		واپس 0;

	اگر (ext2fs_has_feature_metadata_csum(fs->super))
		csum_سائز = حجم(ساخت ext2_dir_اندراج_دم);

	retval = ext2fs_get_mem(blocksize, &buf0);
	اگر (retval)
		واپس retval;

	retval = ext2fs_get_mem(blocksize, &buf1);
	اگر (retval)
		جاؤ برآمد_buf0;

	retval = ext2fs_bmap2(fs, dir, &inode, تہی, 0, 0, تہی, &pblk0);
	اگر (retval)
		جاؤ برآمد_buf1;

	retval = ext2fs_read_dir_block4(fs, pblk0, buf0, 0, dir);
	اگر (retval)
		جاؤ برآمد_buf1;

	جڑ = (ساخت dx_جڑ *)buf0;
	اگر (!check_dx_جڑ(fs, dir, جڑ)) {
		retval = EXT2_ET_DIR_CORRUPTED;
		جاؤ برآمد_buf1;
	}

	/* The 0th block becomes the root, move the dirents out */
	fde = &جڑ->dotdot;
	de = ext2fs_اگلا_اندراج(fs, (ساخت ext2_dir_اندراج *)fde);
	len = ((حرف *) جڑ) + (blocksize - csum_سائز) - (حرف *) de;

	/* Allocate new block for the 0th block's dirents */
	retval = ext2fs_bmap2(fs, dir, &inode, تہی, BMAP_ALLOC | BMAP_ZERO, 1,
			      تہی, &pblk1);
	اگر (retval)
		جاؤ برآمد_buf1;

	حافظہ_نقل(buf1, de, len);
	memset(de, 0, len); /* wipe old data */

	de = (ساخت ext2_dir_اندراج *)buf1;
	چوٹی = buf1 + len;
	جب ((حرف *)(de2 = ext2fs_اگلا_اندراج(fs, de)) < چوٹی) {
#اگر 0
		اگر (ext4_check_dir_اندراج(dir, تہی, de, bh2, buf1, len,
					(حرف *)de - buf1)) {
			retval = EXT2_ET_DIR_CORRUPTED;
			جاؤ برآمد_buf1;
		}
#اگر_ختم
		de = de2;
	}
	retval = ext2fs_سیٹ_rec_len(fs,
			buf1 + (blocksize - csum_سائز) - (حرف *) de, de);
	اگر (retval)
		جاؤ برآمد_buf1;

	اگر (csum_سائز)
		ext2fs_initialize_dirent_دم(fs,
				EXT2_DIRENT_دم(buf1, fs->blocksize));

	/* Initialize the root; the dot dirents already exist */
	de = (ساخت ext2_dir_اندراج *) (&جڑ->dotdot);
	retval = ext2fs_سیٹ_rec_len(fs, blocksize - ext2fs_dir_rec_len(2, 0),
				    de);
	memset (&جڑ->info, 0, حجم(جڑ->info));
	جڑ->info.info_لمبائی = حجم(جڑ->info);
	اگر (ext4_hash_اندر_dirent(&inode))
		جڑ->info.hash_نسخہ = EXT2_HASH_SIPHASH;
	ورنہ
		جڑ->info.hash_نسخہ = fs->super->s_def_hash_نسخہ;

	entries = جڑ->entries;
	dx_سیٹ_بلاک(entries, 1);
	dx_سیٹ_گنتی(entries, 1);
	dx_سیٹ_حد_بندی(entries, dx_جڑ_حد_بندی(fs, حجم(جڑ->info)));

	retval = ext2fs_write_dir_block4(fs, pblk1, buf1, 0, dir);
	اگر (retval)
		جاؤ برآمد_buf1;

	retval = ext2fs_write_dir_block4(fs, pblk0, buf0, 0, dir);
	اگر (retval)
		جاؤ برآمد_buf1;

	inode.i_flags |= EXT2_اشاریہ_FL;
	inode.i_سائز += fs->blocksize;
	retval = ext2fs_write_inode(fs, dir, &inode);
	اگر (retval)
		جاؤ برآمد_buf1;

	retval = EAGAIN;
برآمد_buf1:
	ext2fs_free_mem(&buf1);
برآمد_buf0:
	ext2fs_free_mem(&buf0);
	واپس retval;
}

/*
 * Note: the low 3 bits of the flags field are used as the directory
 * entry filetype.
 */
#اگر_تعریف_شدہ __TURBOC__
 #ہدایت argsused
#اگر_ختم
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, ثابت حرف *name,
		      ext2_ino_t ino, صحیح flags)
{
	errcode_t		retval;
	ساخت link_struct	ls;
	ساخت ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (!(fs->flags & EXT2_FLAG_RW))
		واپس EXT2_ET_RO_FILSYS;

retry:
	اگر ((retval = ext2fs_read_inode(fs, dir, &inode)) != 0)
		واپس retval;

	اگر (inode.i_flags & EXT2_اشاریہ_FL)
		واپس dx_link(fs, dir, &inode, name, ino, flags);

	ls.fs = fs;
	ls.name = name;
	ls.namelen = name ? لڑی_لمبائی(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.مکمل = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	اگر ((flags & EXT2FS_LINK_APPEND) &&
	    !(inode.i_flags & EXT4_INLINE_ڈیٹا_FL)) {
		blk64_t		lblk, pblk = 0;
		ساخت dir_context ctx;

		lblk = (inode.i_سائز / fs->blocksize) - 1;
		retval = ext2fs_bmap2(fs, dir, &inode, تہی, 0, lblk,
				      تہی, &pblk);
		اگر (retval)
			واپس retval;

		ctx.dir = dir;
		ctx.flags = DIRENT_FLAG_INCLUDE_خالی_خانے;
		ctx.func = link_proc;
		ctx.priv_ڈیٹا = &ls;
		ctx.errcode = 0;
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		اگر (retval)
			واپس retval;

		ext2fs_process_dir_بلاک(fs, &pblk, lblk, 0, 0, &ctx);
		retval = ctx.errcode;
		ext2fs_free_mem(&ctx.buf);
	} ورنہ {
		retval = ext2fs_dir_iterate2(fs, dir,
					     DIRENT_FLAG_INCLUDE_خالی_خانے,
					     تہی, link_proc, &ls);
	}
	اگر (retval)
		واپس retval;
	اگر (ls.err)
		واپس ls.err;

	اگر (!ls.مکمل) {
		اگر (!(flags & EXT2FS_LINK_EXPAND))
			واپس EXT2_ET_DIR_NO_SPACE;

		اگر (ext2fs_has_feature_dir_اشاریہ(fs->super)) {
			retval = try_make_اشاریہ_دار_dir(fs, dir, name, ino,
						      flags);
			اگر (retval == EAGAIN)
				جاؤ retry;
			اگر (retval)
				واپس retval;
		}

		retval = ext2fs_expand_dir(fs, dir);
		اگر (retval)
			واپس retval;
		جاؤ retry;
	}
	واپس 0;
}

/* Does this directory have an htree index? */
صحیح ext2fs_dir_is_dx(ext2_filsys fs, ثابت ساخت ext2_inode *inode)
{
	واپس ext2fs_has_feature_dir_اشاریہ(fs->super) &&
		S_ISDIR(inode->i_منوال) && (inode->i_flags & EXT2_اشاریہ_FL);
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
خالی ext2fs_inc_nlink(ext2_filsys fs, ساخت ext2_inode *inode)
{
	inode->i_links_گنتی++;

	اگر (ext2fs_dir_is_dx(fs, inode) &&
	    (inode->i_links_گنتی > EXT2_LINK_MAX || inode->i_links_گنتی == 2))
		inode->i_links_گنتی = 1;
}

/*
 * If a directory had nlink == 1, then we should let it be 1. This indicates
 * directory has >EXT2_LINK_MAX subdirs.
 */
خالی ext2fs_dec_nlink(ساخت ext2_inode *inode)
{
	اگر (!S_ISDIR(inode->i_منوال) || inode->i_links_گنتی > 2)
		inode->i_links_گنتی--;
}

صحیح ext2fs_dir_link_max(ext2_filsys fs, ساخت ext2_inode_large *inode)
{
	واپس inode->i_links_گنتی >= EXT2_LINK_MAX &&
	       !(ext2fs_dir_is_dx(fs, EXT2_INODE(inode)) &&
		 ext2fs_has_feature_dir_nlink(fs->super));
}

صحیح ext2fs_dir_link_خالی_خانے(ساخت ext2_inode *inode)
{
	واپس inode->i_links_گنتی == 2 || inode->i_links_گنتی == 1;
}
