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
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

#وصف_ڪريو EXT2_DX_پاڙ_OFF 24

بنية dx_چوکٺو {
	فراغ *buf;
	blk64_t pblock;
	بنية ext2_dx_countlimit *سر;
	بنية ext2_dx_داخلا *entries;
	بنية ext2_dx_داخلا *at;
};

بنية dx_lookup_info {
	ثابت حرف *name;
	صحيح namelen;
	صحيح hash_alg;
	__u32 hash;
	بنانشان سطحون;
	بنية dx_چوکٺو چوکٺا[EXT4_HTREE_LEVEL];
};

ساكن errcode_t alloc_dx_چوکٺو(ext2_filsys fs, بنية dx_چوکٺو *چوکٺو)
{
	أرجع ext2fs_get_mem(fs->blocksize, &چوکٺو->buf);
}

ساكن فراغ dx_release(بنية dx_lookup_info *info)
{
	بنانشان level;

	لكل (level = 0; level < info->سطحون; level++) {
		إذا (info->چوکٺا[level].buf == عدم)
			اكسر;
		ext2fs_free_mem(&(info->چوکٺا[level].buf));
	}
	info->سطحون = 0;
}

ساكن فراغ dx_ڳولا_داخلا(بنية dx_چوکٺو *چوکٺو, صحيح ڳڻپ, __u32 hash)
{
	بنية ext2_dx_داخلا *p, *q, *m;

	p = چوکٺو->entries + 1;
	q = چوکٺو->entries + ڳڻپ - 1;
	طالما (p <= q) {
		m = p + (q - p) / 2;
		إذا (ext2fs_le32_منزل_cpu(m->hash) > hash)
			q = m - 1;
		وإلا
			p = m + 1;
	}
	چوکٺو->at = p - 1;
}

ساكن errcode_t ڀريو_logical_dir_بلاڪ(ext2_filsys fs, ext2_ino_t dir,
					بنية ext2_inode *diri, blk64_t بلاڪ,
					blk64_t *pblk, فراغ *buf)
{
	errcode_t errcode;
	صحيح ret_flags;

	errcode = ext2fs_bmap2(fs, dir, diri, عدم, 0, بلاڪ, &ret_flags,
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
	بنية ext2_dx_پاڙ_info *پاڙ;
	errcode_t errcode;
	صحيح level = 0;
	صحيح ڳڻپ, حد_بندي;
	صحيح hash_alg;
	صحيح hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	__u32 minor_hash;
	بنية dx_چوکٺو *چوکٺو;

	errcode = alloc_dx_چوکٺو(fs, &(info->چوکٺا[0]));
	إذا (errcode)
		أرجع errcode;
	info->سطحون = 1;

	errcode = ڀريو_logical_dir_بلاڪ(fs, dir, diri, 0,
					 &(info->چوکٺا[0].pblock),
					 info->چوکٺا[0].buf);
	إذا (errcode)
		اذهب ٻاهريون_err;
	پاڙ = (بنية ext2_dx_پاڙ_info *) ((حرف *)info->چوکٺا[0].buf +
					     EXT2_DX_پاڙ_OFF);
	hash_alg = پاڙ->hash_نسخو;
	إذا (hash_alg != EXT2_HASH_TEA && hash_alg != EXT2_HASH_HALF_MD4 &&
	    hash_alg != EXT2_HASH_LEGACY) {
		errcode = EXT2_ET_DIRHASH_UNSUPP;
		اذهب ٻاهريون_err;
	}
	إذا (hash_alg <= EXT2_HASH_TEA &&
	    fs->super->s_flags & EXT2_FLAGS_UNSIGNED_HASH)
		hash_alg += 3;
	إذا (پاڙ->اڻسڌو_سطحون >= ext2_dir_htree_level(fs)) {
		errcode = EXT2_ET_DIR_CORRUPTED;
		اذهب ٻاهريون_err;
	}
	info->hash_alg = hash_alg;

	errcode = ext2fs_dirhash2(hash_alg, info->name, info->namelen,
				  fs->encoding, hash_flags,
				  fs->super->s_hash_ٻج, &info->hash,
				  &minor_hash);
	إذا (errcode)
		اذهب ٻاهريون_err;

	لكل (level = 0; level <= پاڙ->اڻسڌو_سطحون; level++) {
		چوکٺو = &(info->چوکٺا[level]);
		إذا (level > 0) {
			errcode = alloc_dx_چوکٺو(fs, چوکٺو);
			إذا (errcode)
				اذهب ٻاهريون_err;
			info->سطحون++;

			errcode = ڀريو_logical_dir_بلاڪ(fs, dir, diri,
				ext2fs_le32_منزل_cpu(info->چوکٺا[level-1].at->بلاڪ) & 0x0fffffff,
				&(چوکٺو->pblock), چوکٺو->buf);
			إذا (errcode)
				اذهب ٻاهريون_err;
		}
		errcode = ext2fs_get_dx_countlimit(fs, چوکٺو->buf,
						   &(چوکٺو->سر), عدم);
		إذا (errcode)
			اذهب ٻاهريون_err;
		ڳڻپ = ext2fs_le16_منزل_cpu(چوکٺو->سر->ڳڻپ);
		حد_بندي = ext2fs_le16_منزل_cpu(چوکٺو->سر->حد_بندي);
		چوکٺو->entries = (بنية ext2_dx_داخلا *)(چوکٺو->سر);
		إذا (!ڳڻپ || ڳڻپ > حد_بندي) {
			errcode = EXT2_ET_DIR_CORRUPTED;
			اذهب ٻاهريون_err;
		}

		dx_ڳولا_داخلا(چوکٺو, ڳڻپ, info->hash);
	}
	أرجع 0;
ٻاهريون_err:
	dx_release(info);
	أرجع errcode;
}

بنية link_struct  {
	ext2_filsys	fs;
	ثابت حرف	*name;
	صحيح		namelen;
	ext2_ino_t	inode;
	صحيح		flags;
	صحيح		پورو;
	بنانشان صحيح	blocksize;
	errcode_t	err;
	بنية ext2_super_بلاڪ *sb;
};

ساكن صحيح link_proc(ext2_ino_t dir EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
		     صحيح entru EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
		     بنية ext2_dir_داخلا *dirent,
		     صحيح	وٿي,
		     صحيح	blocksize,
		     حرف	*buf,
		     فراغ	*priv_معلومات)
{
	بنية link_struct *ls = (بنية link_struct *) priv_معلومات;
	بنية ext2_dir_داخلا *ايندڙ;
	بنانشان صحيح rec_len, min_rec_len, curr_rec_len;
	صحيح ret = 0;
	صحيح csum_سائيز = 0;

	إذا (ls->پورو)
		أرجع DIRENT_ABORT;

	rec_len = EXT2_DIR_REC_LEN(ls->namelen);

	ls->err = ext2fs_get_rec_len(ls->fs, dirent, &curr_rec_len);
	إذا (ls->err)
		أرجع DIRENT_ABORT;

	إذا (ext2fs_has_feature_metadata_csum(ls->fs->super))
		csum_سائيز = حجم(بنية ext2_dir_داخلا_پڇ);
	/*
	 * See if the following directory entry (if any) is unused;
	 * if so, absorb it into this one.
	 */
	ايندڙ = (بنية ext2_dir_داخلا *) (buf + وٿي + curr_rec_len);
	إذا ((وٿي + (صحيح) curr_rec_len < blocksize - (8 + csum_سائيز)) &&
	    (ايندڙ->inode == 0) &&
	    (وٿي + (صحيح) curr_rec_len + (صحيح) ايندڙ->rec_len <= blocksize)) {
		curr_rec_len += ايندڙ->rec_len;
		ls->err = ext2fs_سيٽ_rec_len(ls->fs, curr_rec_len, dirent);
		إذا (ls->err)
			أرجع DIRENT_ABORT;
		ret = DIRENT_بدليو;
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
		ls->err = ext2fs_سيٽ_rec_len(ls->fs, min_rec_len, dirent);
		إذا (ls->err)
			أرجع DIRENT_ABORT;
		ايندڙ = (بنية ext2_dir_داخلا *) (buf + وٿي +
						  dirent->rec_len);
		ايندڙ->inode = 0;
		ext2fs_dirent_سيٽ_name_len(ايندڙ, 0);
		ext2fs_dirent_سيٽ_file_type(ايندڙ, 0);
		ls->err = ext2fs_سيٽ_rec_len(ls->fs, rec_len, ايندڙ);
		إذا (ls->err)
			أرجع DIRENT_ABORT;
		أرجع DIRENT_بدليو;
	}

	/*
	 * If we get this far, then the directory entry is not used.
	 * See if we can fit the request entry in.  If so, do it.
	 */
	إذا (curr_rec_len < rec_len)
		أرجع ret;
	dirent->inode = ls->inode;
	ext2fs_dirent_سيٽ_name_len(dirent, ls->namelen);
	strncpy(dirent->name, ls->name, ls->namelen);
	إذا (ext2fs_has_feature_filetype(ls->sb))
		ext2fs_dirent_سيٽ_file_type(dirent,
					    ls->flags & EXT2_FS_LINK_FT_نقاب);

	ls->پورو++;
	أرجع DIRENT_ABORT|DIRENT_بدليو;
}

ساكن errcode_t جوڙيو_dirent_منزل_buf(ext2_filsys fs, e2_blkcnt_t blockcnt,
				   حرف *buf, ext2_ino_t dir,
				   بنية ext2_inode *diri, ثابت حرف *name,
				   ext2_ino_t ino, صحيح flags, blk64_t *pblkp)
{
	بنية dir_context ctx;
	بنية link_struct ls;
	errcode_t retval;

	retval = ڀريو_logical_dir_بلاڪ(fs, dir, diri, blockcnt, pblkp, buf);
	إذا (retval)
		أرجع retval;
	ctx.errcode = 0;
	ctx.func = link_proc;
	ctx.dir = dir;
	ctx.flags = DIRENT_FLAG_INCLUDE_خالي_جايون;
	ctx.buf = buf;
	ctx.priv_معلومات = &ls;

	ls.fs = fs;
	ls.name = name;
	ls.namelen = لڙي_ڊيگهه(name);
	ls.inode = ino;
	ls.flags = flags;
	ls.پورو = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	ext2fs_process_dir_بلاڪ(fs, pblkp, blockcnt, 0, 0, &ctx);
	إذا (ctx.errcode)
		أرجع ctx.errcode;
	إذا (ls.err)
		أرجع ls.err;
	إذا (!ls.پورو)
		أرجع EXT2_ET_DIR_NO_SPACE;
	أرجع 0;
}

بنية dx_hash_map {
	__u32 hash;
	صحيح سائيز;
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
				 صحيح ڳڻپ, فراغ *from, فراغ *منزل)
{
	بنية ext2_dir_داخلا *de;
	صحيح i;
	صحيح rec_len = 0;
	errcode_t retval;
	صحيح csum_سائيز = 0;
	فراغ *بنياد = منزل;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_سائيز = حجم(بنية ext2_dir_داخلا_پڇ);

	لكل (i = 0; i < ڳڻپ; i++) {
		de = (بنية ext2_dir_داخلا *) ((حرف *)from + map[i].off);
		rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(de));
		يادگيري_نقل(منزل, de, rec_len);
		retval = ext2fs_سيٽ_rec_len(fs, rec_len, منزل);
		إذا (retval)
			أرجع retval;
		منزل = (حرف *)منزل + rec_len;
	}
	/*
	 * Update rec_len of the last dir entry to stretch to the end of block
	 */
	منزل = (حرف *)منزل - rec_len;
	rec_len = fs->blocksize - ((حرف *)منزل - (حرف *)بنياد) - csum_سائيز;
	retval = ext2fs_سيٽ_rec_len(fs, rec_len, منزل);
	إذا (retval)
		أرجع retval;
	إذا (csum_سائيز)
		ext2fs_initialize_dirent_پڇ(fs,
				EXT2_DIRENT_پڇ(بنياد, fs->blocksize));
	أرجع 0;
}

ساكن errcode_t dx_وجهو_داخلا(ext2_filsys fs, ext2_ino_t dir,
				 بنية dx_lookup_info *info, صحيح level,
				 __u32 hash, blk64_t lblk)
{
	صحيح pcount;
	بنية ext2_dx_داخلا *چوٽي, *new;

	pcount = ext2fs_le16_منزل_cpu(info->چوکٺا[level].سر->ڳڻپ);
	چوٽي = info->چوکٺا[level].entries + pcount;
	new = info->چوکٺا[level].at + 1;
	memmove(new + 1, new, (حرف *)چوٽي - (حرف *)new);
	new->hash = ext2fs_cpu_منزل_le32(hash);
	new->بلاڪ = ext2fs_cpu_منزل_le32(lblk);
	info->چوکٺا[level].سر->ڳڻپ = ext2fs_cpu_منزل_le16(pcount + 1);
	أرجع ext2fs_write_dir_block4(fs, info->چوکٺا[level].pblock,
				       info->چوکٺا[level].buf, 0, dir);
}

ساكن errcode_t dx_ورهايو_پن(ext2_filsys fs, ext2_ino_t dir,
			       بنية ext2_inode *diri,
			       بنية dx_lookup_info *info, فراغ *buf,
			       blk64_t پن_pblk, blk64_t new_lblk,
			       blk64_t new_pblk)
{
	صحيح hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	بنية ext2_dir_داخلا *de;
	فراغ *buf2;
	errcode_t retval = 0;
	بنانشان صحيح rec_len;
	بنانشان صحيح وٿي, move_سائيز;
	صحيح i, ڳڻپ = 0;
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
	لكل (وٿي = 0; وٿي < fs->blocksize; وٿي += rec_len) {
		de = (بنية ext2_dir_داخلا *) ((حرف *)buf + وٿي);
		retval = ext2fs_get_rec_len(fs, de, &rec_len);
		إذا (retval)
			اذهب ٻاهريون;
		إذا (ext2fs_dirent_name_len(de) > 0 && de->inode) {
			map[ڳڻپ].off = وٿي;
			map[ڳڻپ].سائيز = rec_len;
			retval = ext2fs_dirhash2(info->hash_alg, de->name,
					ext2fs_dirent_name_len(de),
					fs->encoding, hash_flags,
					fs->super->s_hash_ٻج,
					&(map[ڳڻپ].hash),
					&minor_hash);
			إذا (retval)
				اذهب ٻاهريون;
			ڳڻپ++;
		}
	}
	qsort(map, ڳڻپ, حجم(بنية dx_hash_map), dx_hash_map_cmp);
	move_سائيز = 0;
	/* Find place to split block */
	لكل (i = ڳڻپ - 1; i >= 0; i--) {
		إذا (move_سائيز + map[i].سائيز / 2 > fs->blocksize / 2)
			اكسر;
		move_سائيز += map[i].سائيز;
	}
	/* Let i be the first entry to move */
	i++;
	/* Move selected directory entries to new block */
	retval = dx_move_dirents(fs, map + i, ڳڻپ - i, buf, buf2);
	إذا (retval)
		اذهب ٻاهريون;
	retval = ext2fs_write_dir_block4(fs, new_pblk, buf2, 0, dir);
	إذا (retval)
		اذهب ٻاهريون;
	/* Repack remaining entries in the old block */
	retval = dx_move_dirents(fs, map, i, buf, buf2);
	إذا (retval)
		اذهب ٻاهريون;
	retval = ext2fs_write_dir_block4(fs, پن_pblk, buf2, 0, dir);
	إذا (retval)
		اذهب ٻاهريون;
	/* Update parent node */
	continued = map[i].hash == map[i-1].hash;
	retval = dx_وجهو_داخلا(fs, dir, info, info->سطحون - 1,
				 map[i].hash + continued, new_lblk);
ٻاهريون:
	ext2fs_free_mem(&buf2);
	ext2fs_free_mem(&map);
	أرجع retval;
}

ساكن errcode_t dx_grow_وڻ(ext2_filsys fs, ext2_ino_t dir,
			      بنية ext2_inode *diri,
			      بنية dx_lookup_info *info, فراغ *buf,
			      blk64_t پن_pblk)
{
	صحيح i;
	errcode_t retval;
	ext2_off64_t سائيز = EXT2_I_سائيز(diri);
	blk64_t lblk, pblk;
	بنية ext2_dir_داخلا *de;
	بنية ext2_dx_countlimit *سر;
	صحيح csum_سائيز = 0;
	صحيح ڳڻپ;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_سائيز = حجم(بنية ext2_dx_پڇ);

	/* Find level which can accommodate new child */
	لكل (i = info->سطحون - 1; i >= 0; i--)
		إذا (ext2fs_le16_منزل_cpu(info->چوکٺا[i].سر->ڳڻپ) <
		    ext2fs_le16_منزل_cpu(info->چوکٺا[i].سر->حد_بندي))
			اكسر;
	/* Need to grow tree depth? */
	إذا (i < 0 && info->سطحون >= ext2_dir_htree_level(fs))
		أرجع EXT2_ET_DIR_NO_SPACE;
	lblk = سائيز / fs->blocksize;
	سائيز += fs->blocksize;
	retval = ext2fs_inode_سائيز_سيٽ(fs, diri, سائيز);
	إذا (retval)
		أرجع retval;
	retval = ext2fs_fallocate(fs,
			EXT2_FALLOCATE_FORCE_INIT | EXT2_FALLOCATE_ZERO_بلاڪون,
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
	إذا (i == (صحيح)info->سطحون - 1)
		أرجع dx_ورهايو_پن(fs, dir, diri, info, buf, پن_pblk,
				     lblk, pblk);

	de = buf;
	de->inode = 0;
	ext2fs_dirent_سيٽ_name_len(de, 0);
	ext2fs_dirent_سيٽ_file_type(de, 0);
	retval = ext2fs_سيٽ_rec_len(fs, fs->blocksize, de);
	إذا (retval)
		أرجع retval;
	سر = (بنية ext2_dx_countlimit *) ((حرف *)buf + 8);
	ڳڻپ = ext2fs_le16_منزل_cpu(info->چوکٺا[i+1].سر->ڳڻپ);
	/* Growing tree depth? */
	إذا (i < 0) {
		بنية ext2_dx_پاڙ_info *پاڙ;

		يادگيري_نقل(سر, info->چوکٺا[0].entries,
		       ڳڻپ * حجم(بنية ext2_dx_داخلا));
		سر->حد_بندي = ext2fs_cpu_منزل_le16(
				(fs->blocksize - (8 + csum_سائيز)) /
				حجم(بنية ext2_dx_داخلا));
		/* head->count gets set by memcpy above to correct value */

		/* Now update tree root */
		info->چوکٺا[0].سر->ڳڻپ = ext2fs_cpu_منزل_le16(1);
		info->چوکٺا[0].entries[0].بلاڪ = ext2fs_cpu_منزل_le32(lblk);
		پاڙ = (بنية ext2_dx_پاڙ_info *)
			((حرف *)info->چوکٺا[0].buf + EXT2_DX_پاڙ_OFF);
		پاڙ->اڻسڌو_سطحون++;
	} وإلا {
		/* Splitting internal node in two */
		صحيح count1 = ڳڻپ / 2;
		صحيح count2 = ڳڻپ - count1;
		__u32 ورهايو_hash = ext2fs_le32_منزل_cpu(info->چوکٺا[i+1].entries[count1].hash);

		يادگيري_نقل(سر, info->چوکٺا[i+1].entries + count1,
		       count2 * حجم(بنية ext2_dx_داخلا));
		سر->ڳڻپ = ext2fs_cpu_منزل_le16(count2);
		سر->حد_بندي = ext2fs_cpu_منزل_le16(
				(fs->blocksize - (8 + csum_سائيز)) /
				حجم(بنية ext2_dx_داخلا));
		info->چوکٺا[i+1].سر->ڳڻپ = ext2fs_cpu_منزل_le16(count1);

		/* Update parent node */
		retval = dx_وجهو_داخلا(fs, dir, info, i, ورهايو_hash, lblk);
		إذا (retval)
			أرجع retval;

	}
	/* Writeout split block / updated root */
	retval = ext2fs_write_dir_block4(fs, info->چوکٺا[i+1].pblock,
					 info->چوکٺا[i+1].buf, 0, dir);
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
	بنانشان restart = 0;
	blk64_t پن_pblk;

	retval = ext2fs_get_mem(fs->blocksize, &blockbuf);
	إذا (retval)
		أرجع retval;

	dx_info.name = name;
	dx_info.namelen = لڙي_ڊيگهه(name);
again:
	retval = dx_lookup(fs, dir, diri, &dx_info);
	إذا (retval)
		اذهب free_buf;

	retval = جوڙيو_dirent_منزل_buf(fs,
		ext2fs_le32_منزل_cpu(dx_info.چوکٺا[dx_info.سطحون-1].at->بلاڪ) & 0x0fffffff,
		blockbuf, dir, diri, name, ino, flags, &پن_pblk);
	/*
	 * Success or error other than ENOSPC...? We are done. We may need upto
	 * two tries to add entry. One to split htree node and another to add
	 * new leaf block.
	 */
	إذا (restart >= dx_info.سطحون || retval != EXT2_ET_DIR_NO_SPACE)
		اذهب free_چوکٺا;
	retval = dx_grow_وڻ(fs, dir, diri, &dx_info, blockbuf, پن_pblk);
	إذا (retval)
		اذهب free_چوکٺا;
	/* Restart everything now that the tree is larger */
	restart++;
	dx_release(&dx_info);
	اذهب again;
free_چوکٺا:
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

بنية dx_پاڙ
{
	بنية fake_dirent dot;
	حرف dot_name[4];
	بنية fake_dirent dotdot;
	حرف dotdot_name[4];
	بنية ext2_dx_پاڙ_info info;
	بنية ext2_dx_داخلا entries[];
};

ساكن صحيح check_dx_پاڙ(ext2_filsys fs, ext2_ino_t ino, بنية dx_پاڙ *پاڙ)
{
	بنية fake_dirent *fde;
	حرف *error_نياپو;
	بنانشان صحيح rlen;
	حرف *blockend = (حرف *)پاڙ + fs->blocksize;

	fde = &پاڙ->dot;
	إذا (fde->name_len != 1) {
		error_نياپو = "invalid name_len for '.'";
		اذهب corrupted;
	}
	إذا (لڙي_ڀيٽ(پاڙ->dot_name, ".", fde->name_len)) {
		error_نياپو = "invalid name for '.'";
		اذهب corrupted;
	}

	ext2fs_get_rec_len(fs, (بنية ext2_dir_داخلا *)fde, &rlen);
	إذا ((حرف *)fde + rlen >= blockend) {
		error_نياپو = "invalid rec_len for '.'";
		اذهب corrupted;
	}

	fde = &پاڙ->dotdot;
	إذا (fde->name_len != 2) {
		error_نياپو = "invalid name_len for '..'";
		اذهب corrupted;
	}
	إذا (لڙي_ڀيٽ(پاڙ->dotdot_name, "..", fde->name_len)) {
		error_نياپو = "invalid name for '..'";
		اذهب corrupted;
	}
	ext2fs_get_rec_len(fs, (بنية ext2_dir_داخلا *)fde, &rlen);
	إذا ((حرف *)fde + rlen >= blockend) {
		error_نياپو = "invalid rec_len for '..'";
		اذهب corrupted;
	}

	أرجع 1;

corrupted:
	fprintf(stderr, "Corrupt dir %u: %s, running e2fsck is recommended\n",
			 ino, error_نياپو);
	أرجع 0;
}

ساكن مضمن بنية ext2_dir_داخلا *
ext2fs_ايندڙ_داخلا(ext2_filsys fs, بنية ext2_dir_داخلا *p)
{
	بنانشان صحيح rlen;

	ext2fs_get_rec_len(fs, p, &rlen);
	أرجع (بنية ext2_dir_داخلا *)((حرف *)p + rlen);
}

ساكن مضمن فراغ dx_سيٽ_بلاڪ(بنية ext2_dx_داخلا *داخلا, بنانشان قدر)
{
	داخلا->بلاڪ = ext2fs_cpu_منزل_le32(قدر);
}

ساكن مضمن فراغ dx_سيٽ_ڳڻپ(بنية ext2_dx_داخلا *entries, بنانشان قدر)
{
	((بنية ext2_dx_countlimit *) entries)->ڳڻپ = ext2fs_cpu_منزل_le16(قدر);
}

ساكن مضمن فراغ dx_سيٽ_حد_بندي(بنية ext2_dx_داخلا *entries, بنانشان قدر)
{
	((بنية ext2_dx_countlimit *) entries)->حد_بندي = ext2fs_cpu_منزل_le16(قدر);
}

ساكن مضمن بنانشان dx_پاڙ_حد_بندي(ext2_filsys fs, بنانشان infosize)
{
	بنانشان صحيح داخلا_space = fs->blocksize -
			ext2fs_dir_rec_len(1, 0) -
			ext2fs_dir_rec_len(2, 0) - infosize;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		داخلا_space -= حجم(بنية ext2_dx_پڇ);
	أرجع داخلا_space / حجم(بنية ext2_dx_داخلا);
}

/*
 * This converts a one block unindexed directory to a 3 block indexed
 * directory, and adds the dentry to the indexed directory.  Returns 0 if the
 * index was not created; EAGAIN if it was; or an errcode_t on error.
 */
ساكن errcode_t try_make_اشاريي_وارو_dir(ext2_filsys fs, ext2_ino_t dir,
				      ثابت حرف *name, ext2_ino_t ino,
				      صحيح flags)
{
	بنية ext2_inode inode;
	حرف *buf0, *buf1, *چوٽي;
	بنية dx_پاڙ *پاڙ;
	بنية ext2_dx_داخلا *entries;
	بنية fake_dirent *fde;
	بنية ext2_dir_داخلا *de, *de2;
	blk64_t pblk0, pblk1;
	بنانشان صحيح len;
	ثابت بنانشان صحيح blocksize = fs->blocksize;
	صحيح csum_سائيز = 0;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	إذا (retval)
		أرجع retval;

	إذا (inode.i_سائيز > fs->blocksize || (inode.i_flags & EXT2_اشاريو_FL))
		أرجع 0;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_سائيز = حجم(بنية ext2_dir_داخلا_پڇ);

	retval = ext2fs_get_mem(blocksize, &buf0);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_get_mem(blocksize, &buf1);
	إذا (retval)
		اذهب ٻاهريون_buf0;

	retval = ext2fs_bmap2(fs, dir, &inode, عدم, 0, 0, عدم, &pblk0);
	إذا (retval)
		اذهب ٻاهريون_buf1;

	retval = ext2fs_read_dir_block4(fs, pblk0, buf0, 0, dir);
	إذا (retval)
		اذهب ٻاهريون_buf1;

	پاڙ = (بنية dx_پاڙ *)buf0;
	إذا (!check_dx_پاڙ(fs, dir, پاڙ)) {
		retval = EXT2_ET_DIR_CORRUPTED;
		اذهب ٻاهريون_buf1;
	}

	/* The 0th block becomes the root, move the dirents out */
	fde = &پاڙ->dotdot;
	de = ext2fs_ايندڙ_داخلا(fs, (بنية ext2_dir_داخلا *)fde);
	len = ((حرف *) پاڙ) + (blocksize - csum_سائيز) - (حرف *) de;

	/* Allocate new block for the 0th block's dirents */
	retval = ext2fs_bmap2(fs, dir, &inode, عدم, BMAP_ALLOC | BMAP_ZERO, 1,
			      عدم, &pblk1);
	إذا (retval)
		اذهب ٻاهريون_buf1;

	يادگيري_نقل(buf1, de, len);
	memset(de, 0, len); /* wipe old data */

	de = (بنية ext2_dir_داخلا *)buf1;
	چوٽي = buf1 + len;
	طالما ((حرف *)(de2 = ext2fs_ايندڙ_داخلا(fs, de)) < چوٽي) {
#إذا 0
		إذا (ext4_check_dir_داخلا(dir, عدم, de, bh2, buf1, len,
					(حرف *)de - buf1)) {
			retval = EXT2_ET_DIR_CORRUPTED;
			اذهب ٻاهريون_buf1;
		}
#جيڪڏهن_پورو
		de = de2;
	}
	retval = ext2fs_سيٽ_rec_len(fs,
			buf1 + (blocksize - csum_سائيز) - (حرف *) de, de);
	إذا (retval)
		اذهب ٻاهريون_buf1;

	إذا (csum_سائيز)
		ext2fs_initialize_dirent_پڇ(fs,
				EXT2_DIRENT_پڇ(buf1, fs->blocksize));

	/* Initialize the root; the dot dirents already exist */
	de = (بنية ext2_dir_داخلا *) (&پاڙ->dotdot);
	retval = ext2fs_سيٽ_rec_len(fs, blocksize - ext2fs_dir_rec_len(2, 0),
				    de);
	memset (&پاڙ->info, 0, حجم(پاڙ->info));
	پاڙ->info.info_ڊيگهائي = حجم(پاڙ->info);
	إذا (ext4_hash_اندريون_dirent(&inode))
		پاڙ->info.hash_نسخو = EXT2_HASH_SIPHASH;
	وإلا
		پاڙ->info.hash_نسخو = fs->super->s_def_hash_نسخو;

	entries = پاڙ->entries;
	dx_سيٽ_بلاڪ(entries, 1);
	dx_سيٽ_ڳڻپ(entries, 1);
	dx_سيٽ_حد_بندي(entries, dx_پاڙ_حد_بندي(fs, حجم(پاڙ->info)));

	retval = ext2fs_write_dir_block4(fs, pblk1, buf1, 0, dir);
	إذا (retval)
		اذهب ٻاهريون_buf1;

	retval = ext2fs_write_dir_block4(fs, pblk0, buf0, 0, dir);
	إذا (retval)
		اذهب ٻاهريون_buf1;

	inode.i_flags |= EXT2_اشاريو_FL;
	inode.i_سائيز += fs->blocksize;
	retval = ext2fs_write_inode(fs, dir, &inode);
	إذا (retval)
		اذهب ٻاهريون_buf1;

	retval = EAGAIN;
ٻاهريون_buf1:
	ext2fs_free_mem(&buf1);
ٻاهريون_buf0:
	ext2fs_free_mem(&buf0);
	أرجع retval;
}

/*
 * Note: the low 3 bits of the flags field are used as the directory
 * entry filetype.
 */
#جيڪڏهن_وصف_ٿيل __TURBOC__
 #هدايت argsused
#جيڪڏهن_پورو
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

	إذا (inode.i_flags & EXT2_اشاريو_FL)
		أرجع dx_link(fs, dir, &inode, name, ino, flags);

	ls.fs = fs;
	ls.name = name;
	ls.namelen = name ? لڙي_ڊيگهه(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.پورو = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	إذا ((flags & EXT2FS_LINK_APPEND) &&
	    !(inode.i_flags & EXT4_INLINE_معلومات_FL)) {
		blk64_t		lblk, pblk = 0;
		بنية dir_context ctx;

		lblk = (inode.i_سائيز / fs->blocksize) - 1;
		retval = ext2fs_bmap2(fs, dir, &inode, عدم, 0, lblk,
				      عدم, &pblk);
		إذا (retval)
			أرجع retval;

		ctx.dir = dir;
		ctx.flags = DIRENT_FLAG_INCLUDE_خالي_جايون;
		ctx.func = link_proc;
		ctx.priv_معلومات = &ls;
		ctx.errcode = 0;
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		إذا (retval)
			أرجع retval;

		ext2fs_process_dir_بلاڪ(fs, &pblk, lblk, 0, 0, &ctx);
		retval = ctx.errcode;
		ext2fs_free_mem(&ctx.buf);
	} وإلا {
		retval = ext2fs_dir_iterate2(fs, dir,
					     DIRENT_FLAG_INCLUDE_خالي_جايون,
					     عدم, link_proc, &ls);
	}
	إذا (retval)
		أرجع retval;
	إذا (ls.err)
		أرجع ls.err;

	إذا (!ls.پورو) {
		إذا (!(flags & EXT2FS_LINK_EXPAND))
			أرجع EXT2_ET_DIR_NO_SPACE;

		إذا (ext2fs_has_feature_dir_اشاريو(fs->super)) {
			retval = try_make_اشاريي_وارو_dir(fs, dir, name, ino,
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
	أرجع ext2fs_has_feature_dir_اشاريو(fs->super) &&
		S_ISDIR(inode->i_گهڻائي) && (inode->i_flags & EXT2_اشاريو_FL);
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
	inode->i_links_ڳڻپ++;

	إذا (ext2fs_dir_is_dx(fs, inode) &&
	    (inode->i_links_ڳڻپ > EXT2_LINK_MAX || inode->i_links_ڳڻپ == 2))
		inode->i_links_ڳڻپ = 1;
}

/*
 * If a directory had nlink == 1, then we should let it be 1. This indicates
 * directory has >EXT2_LINK_MAX subdirs.
 */
فراغ ext2fs_dec_nlink(بنية ext2_inode *inode)
{
	إذا (!S_ISDIR(inode->i_گهڻائي) || inode->i_links_ڳڻپ > 2)
		inode->i_links_ڳڻپ--;
}

صحيح ext2fs_dir_link_max(ext2_filsys fs, بنية ext2_inode_large *inode)
{
	أرجع inode->i_links_ڳڻپ >= EXT2_LINK_MAX &&
	       !(ext2fs_dir_is_dx(fs, EXT2_INODE(inode)) &&
		 ext2fs_has_feature_dir_nlink(fs->super));
}

صحيح ext2fs_dir_link_خالي_جايون(بنية ext2_inode *inode)
{
	أرجع inode->i_links_ڳڻپ == 2 || inode->i_links_ڳڻپ == 1;
}
