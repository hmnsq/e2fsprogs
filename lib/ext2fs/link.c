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
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

#تعریف_کړه EXT2_DX_ریښه_OFF 24

بنية dx_چوکاټ {
	فراغ *buf;
	blk64_t pblock;
	بنية ext2_dx_countlimit *سر_2;
	بنية ext2_dx_ثبت *entries;
	بنية ext2_dx_ثبت *at;
};

بنية dx_lookup_info {
	ثابت حرف *name;
	صحيح namelen;
	صحيح hash_alg;
	__u32 hash;
	بینښه پوړونه;
	بنية dx_چوکاټ چوکاټونه[EXT4_HTREE_LEVEL];
};

ساكن errcode_t alloc_dx_چوکاټ(ext2_filsys fs, بنية dx_چوکاټ *چوکاټ)
{
	أرجع ext2fs_get_mem(fs->blocksize, &چوکاټ->buf);
}

ساكن فراغ dx_release(بنية dx_lookup_info *info)
{
	بینښه level;

	لكل (level = 0; level < info->پوړونه; level++) {
		إذا (info->چوکاټونه[level].buf == عدم)
			اكسر;
		ext2fs_free_mem(&(info->چوکاټونه[level].buf));
	}
	info->پوړونه = 0;
}

ساكن فراغ dx_پلټنه_ثبت(بنية dx_چوکاټ *چوکاټ, صحيح شمېر, __u32 hash)
{
	بنية ext2_dx_ثبت *p, *q, *m;

	p = چوکاټ->entries + 1;
	q = چوکاټ->entries + شمېر - 1;
	طالما (p <= q) {
		m = p + (q - p) / 2;
		إذا (ext2fs_le32_منزل_cpu(m->hash) > hash)
			q = m - 1;
		وإلا
			p = m + 1;
	}
	چوکاټ->at = p - 1;
}

ساكن errcode_t بارول_logical_dir_بلاک(ext2_filsys fs, ext2_ino_t dir,
					بنية ext2_inode *diri, blk64_t بلاک,
					blk64_t *pblk, فراغ *buf)
{
	errcode_t errcode;
	صحيح ret_flags;

	errcode = ext2fs_bmap2(fs, dir, diri, عدم, 0, بلاک, &ret_flags,
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
	بنية ext2_dx_ریښه_info *ریښه;
	errcode_t errcode;
	صحيح level = 0;
	صحيح شمېر, حد;
	صحيح hash_alg;
	صحيح hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	__u32 minor_hash;
	بنية dx_چوکاټ *چوکاټ;

	errcode = alloc_dx_چوکاټ(fs, &(info->چوکاټونه[0]));
	إذا (errcode)
		أرجع errcode;
	info->پوړونه = 1;

	errcode = بارول_logical_dir_بلاک(fs, dir, diri, 0,
					 &(info->چوکاټونه[0].pblock),
					 info->چوکاټونه[0].buf);
	إذا (errcode)
		اذهب بهر_err;
	ریښه = (بنية ext2_dx_ریښه_info *) ((حرف *)info->چوکاټونه[0].buf +
					     EXT2_DX_ریښه_OFF);
	hash_alg = ریښه->hash_نسخه;
	إذا (hash_alg != EXT2_HASH_TEA && hash_alg != EXT2_HASH_HALF_MD4 &&
	    hash_alg != EXT2_HASH_LEGACY) {
		errcode = EXT2_ET_DIRHASH_UNSUPP;
		اذهب بهر_err;
	}
	إذا (hash_alg <= EXT2_HASH_TEA &&
	    fs->super->s_flags & EXT2_FLAGS_UNSIGNED_HASH)
		hash_alg += 3;
	إذا (ریښه->غیرمستقیم_پوړونه >= ext2_dir_htree_level(fs)) {
		errcode = EXT2_ET_DIR_CORRUPTED;
		اذهب بهر_err;
	}
	info->hash_alg = hash_alg;

	errcode = ext2fs_dirhash2(hash_alg, info->name, info->namelen,
				  fs->encoding, hash_flags,
				  fs->super->s_hash_تخم, &info->hash,
				  &minor_hash);
	إذا (errcode)
		اذهب بهر_err;

	لكل (level = 0; level <= ریښه->غیرمستقیم_پوړونه; level++) {
		چوکاټ = &(info->چوکاټونه[level]);
		إذا (level > 0) {
			errcode = alloc_dx_چوکاټ(fs, چوکاټ);
			إذا (errcode)
				اذهب بهر_err;
			info->پوړونه++;

			errcode = بارول_logical_dir_بلاک(fs, dir, diri,
				ext2fs_le32_منزل_cpu(info->چوکاټونه[level-1].at->بلاک) & 0x0fffffff,
				&(چوکاټ->pblock), چوکاټ->buf);
			إذا (errcode)
				اذهب بهر_err;
		}
		errcode = ext2fs_get_dx_countlimit(fs, چوکاټ->buf,
						   &(چوکاټ->سر_2), عدم);
		إذا (errcode)
			اذهب بهر_err;
		شمېر = ext2fs_le16_منزل_cpu(چوکاټ->سر_2->شمېر);
		حد = ext2fs_le16_منزل_cpu(چوکاټ->سر_2->حد);
		چوکاټ->entries = (بنية ext2_dx_ثبت *)(چوکاټ->سر_2);
		إذا (!شمېر || شمېر > حد) {
			errcode = EXT2_ET_DIR_CORRUPTED;
			اذهب بهر_err;
		}

		dx_پلټنه_ثبت(چوکاټ, شمېر, info->hash);
	}
	أرجع 0;
بهر_err:
	dx_release(info);
	أرجع errcode;
}

بنية link_struct  {
	ext2_filsys	fs;
	ثابت حرف	*name;
	صحيح		namelen;
	ext2_ino_t	inode;
	صحيح		flags;
	صحيح		بشپړ;
	بینښه صحيح	blocksize;
	errcode_t	err;
	بنية ext2_super_بلاک *sb;
};

ساكن صحيح link_proc(ext2_ino_t dir EXT2FS_ATTR((ناکارول_شوی)),
		     صحيح entru EXT2FS_ATTR((ناکارول_شوی)),
		     بنية ext2_dir_ثبت *dirent,
		     صحيح	دننه_واټن,
		     صحيح	blocksize,
		     حرف	*buf,
		     فراغ	*priv_معلومات)
{
	بنية link_struct *ls = (بنية link_struct *) priv_معلومات;
	بنية ext2_dir_ثبت *بل;
	بینښه صحيح rec_len, min_rec_len, curr_rec_len;
	صحيح ret = 0;
	صحيح csum_اندازه = 0;

	إذا (ls->بشپړ)
		أرجع DIRENT_ABORT;

	rec_len = EXT2_DIR_REC_LEN(ls->namelen);

	ls->err = ext2fs_get_rec_len(ls->fs, dirent, &curr_rec_len);
	إذا (ls->err)
		أرجع DIRENT_ABORT;

	إذا (ext2fs_has_feature_metadata_csum(ls->fs->super))
		csum_اندازه = حجم(بنية ext2_dir_ثبت_لکۍ);
	/*
	 * See if the following directory entry (if any) is unused;
	 * if so, absorb it into this one.
	 */
	بل = (بنية ext2_dir_ثبت *) (buf + دننه_واټن + curr_rec_len);
	إذا ((دننه_واټن + (صحيح) curr_rec_len < blocksize - (8 + csum_اندازه)) &&
	    (بل->inode == 0) &&
	    (دننه_واټن + (صحيح) curr_rec_len + (صحيح) بل->rec_len <= blocksize)) {
		curr_rec_len += بل->rec_len;
		ls->err = ext2fs_سېټ_rec_len(ls->fs, curr_rec_len, dirent);
		إذا (ls->err)
			أرجع DIRENT_ABORT;
		ret = DIRENT_بدل_شوی;
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
		ls->err = ext2fs_سېټ_rec_len(ls->fs, min_rec_len, dirent);
		إذا (ls->err)
			أرجع DIRENT_ABORT;
		بل = (بنية ext2_dir_ثبت *) (buf + دننه_واټن +
						  dirent->rec_len);
		بل->inode = 0;
		ext2fs_dirent_سېټ_name_len(بل, 0);
		ext2fs_dirent_سېټ_file_type(بل, 0);
		ls->err = ext2fs_سېټ_rec_len(ls->fs, rec_len, بل);
		إذا (ls->err)
			أرجع DIRENT_ABORT;
		أرجع DIRENT_بدل_شوی;
	}

	/*
	 * If we get this far, then the directory entry is not used.
	 * See if we can fit the request entry in.  If so, do it.
	 */
	إذا (curr_rec_len < rec_len)
		أرجع ret;
	dirent->inode = ls->inode;
	ext2fs_dirent_سېټ_name_len(dirent, ls->namelen);
	strncpy(dirent->name, ls->name, ls->namelen);
	إذا (ext2fs_has_feature_filetype(ls->sb))
		ext2fs_dirent_سېټ_file_type(dirent,
					    ls->flags & EXT2_FS_LINK_FT_ماسک);

	ls->بشپړ++;
	أرجع DIRENT_ABORT|DIRENT_بدل_شوی;
}

ساكن errcode_t جمع_کړه_dirent_منزل_buf(ext2_filsys fs, e2_blkcnt_t blockcnt,
				   حرف *buf, ext2_ino_t dir,
				   بنية ext2_inode *diri, ثابت حرف *name,
				   ext2_ino_t ino, صحيح flags, blk64_t *pblkp)
{
	بنية dir_context ctx;
	بنية link_struct ls;
	errcode_t retval;

	retval = بارول_logical_dir_بلاک(fs, dir, diri, blockcnt, pblkp, buf);
	إذا (retval)
		أرجع retval;
	ctx.errcode = 0;
	ctx.func = link_proc;
	ctx.dir = dir;
	ctx.flags = DIRENT_FLAG_INCLUDE_تشه;
	ctx.buf = buf;
	ctx.priv_معلومات = &ls;

	ls.fs = fs;
	ls.name = name;
	ls.namelen = مزي_اوږدوالی(name);
	ls.inode = ino;
	ls.flags = flags;
	ls.بشپړ = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	ext2fs_process_dir_بلاک(fs, pblkp, blockcnt, 0, 0, &ctx);
	إذا (ctx.errcode)
		أرجع ctx.errcode;
	إذا (ls.err)
		أرجع ls.err;
	إذا (!ls.بشپړ)
		أرجع EXT2_ET_DIR_NO_SPACE;
	أرجع 0;
}

بنية dx_hash_map {
	__u32 hash;
	صحيح اندازه;
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
				 صحيح شمېر, فراغ *from, فراغ *منزل)
{
	بنية ext2_dir_ثبت *de;
	صحيح i;
	صحيح rec_len = 0;
	errcode_t retval;
	صحيح csum_اندازه = 0;
	فراغ *بنسټ = منزل;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_اندازه = حجم(بنية ext2_dir_ثبت_لکۍ);

	لكل (i = 0; i < شمېر; i++) {
		de = (بنية ext2_dir_ثبت *) ((حرف *)from + map[i].off);
		rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(de));
		حافظه_نقل(منزل, de, rec_len);
		retval = ext2fs_سېټ_rec_len(fs, rec_len, منزل);
		إذا (retval)
			أرجع retval;
		منزل = (حرف *)منزل + rec_len;
	}
	/*
	 * Update rec_len of the last dir entry to stretch to the end of block
	 */
	منزل = (حرف *)منزل - rec_len;
	rec_len = fs->blocksize - ((حرف *)منزل - (حرف *)بنسټ) - csum_اندازه;
	retval = ext2fs_سېټ_rec_len(fs, rec_len, منزل);
	إذا (retval)
		أرجع retval;
	إذا (csum_اندازه)
		ext2fs_initialize_dirent_لکۍ(fs,
				EXT2_DIRENT_لکۍ(بنسټ, fs->blocksize));
	أرجع 0;
}

ساكن errcode_t dx_ورننباسه_ثبت(ext2_filsys fs, ext2_ino_t dir,
				 بنية dx_lookup_info *info, صحيح level,
				 __u32 hash, blk64_t lblk)
{
	صحيح pcount;
	بنية ext2_dx_ثبت *سر, *new;

	pcount = ext2fs_le16_منزل_cpu(info->چوکاټونه[level].سر_2->شمېر);
	سر = info->چوکاټونه[level].entries + pcount;
	new = info->چوکاټونه[level].at + 1;
	memmove(new + 1, new, (حرف *)سر - (حرف *)new);
	new->hash = ext2fs_cpu_منزل_le32(hash);
	new->بلاک = ext2fs_cpu_منزل_le32(lblk);
	info->چوکاټونه[level].سر_2->شمېر = ext2fs_cpu_منزل_le16(pcount + 1);
	أرجع ext2fs_write_dir_block4(fs, info->چوکاټونه[level].pblock,
				       info->چوکاټونه[level].buf, 0, dir);
}

ساكن errcode_t dx_وېشل_ونې_پاڼه(ext2_filsys fs, ext2_ino_t dir,
			       بنية ext2_inode *diri,
			       بنية dx_lookup_info *info, فراغ *buf,
			       blk64_t ونې_پاڼه_pblk, blk64_t new_lblk,
			       blk64_t new_pblk)
{
	صحيح hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	بنية ext2_dir_ثبت *de;
	فراغ *buf2;
	errcode_t retval = 0;
	بینښه صحيح rec_len;
	بینښه صحيح دننه_واټن, move_اندازه;
	صحيح i, شمېر = 0;
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
	لكل (دننه_واټن = 0; دننه_واټن < fs->blocksize; دننه_واټن += rec_len) {
		de = (بنية ext2_dir_ثبت *) ((حرف *)buf + دننه_واټن);
		retval = ext2fs_get_rec_len(fs, de, &rec_len);
		إذا (retval)
			اذهب بهر;
		إذا (ext2fs_dirent_name_len(de) > 0 && de->inode) {
			map[شمېر].off = دننه_واټن;
			map[شمېر].اندازه = rec_len;
			retval = ext2fs_dirhash2(info->hash_alg, de->name,
					ext2fs_dirent_name_len(de),
					fs->encoding, hash_flags,
					fs->super->s_hash_تخم,
					&(map[شمېر].hash),
					&minor_hash);
			إذا (retval)
				اذهب بهر;
			شمېر++;
		}
	}
	qsort(map, شمېر, حجم(بنية dx_hash_map), dx_hash_map_cmp);
	move_اندازه = 0;
	/* Find place to split block */
	لكل (i = شمېر - 1; i >= 0; i--) {
		إذا (move_اندازه + map[i].اندازه / 2 > fs->blocksize / 2)
			اكسر;
		move_اندازه += map[i].اندازه;
	}
	/* Let i be the first entry to move */
	i++;
	/* Move selected directory entries to new block */
	retval = dx_move_dirents(fs, map + i, شمېر - i, buf, buf2);
	إذا (retval)
		اذهب بهر;
	retval = ext2fs_write_dir_block4(fs, new_pblk, buf2, 0, dir);
	إذا (retval)
		اذهب بهر;
	/* Repack remaining entries in the old block */
	retval = dx_move_dirents(fs, map, i, buf, buf2);
	إذا (retval)
		اذهب بهر;
	retval = ext2fs_write_dir_block4(fs, ونې_پاڼه_pblk, buf2, 0, dir);
	إذا (retval)
		اذهب بهر;
	/* Update parent node */
	continued = map[i].hash == map[i-1].hash;
	retval = dx_ورننباسه_ثبت(fs, dir, info, info->پوړونه - 1,
				 map[i].hash + continued, new_lblk);
بهر:
	ext2fs_free_mem(&buf2);
	ext2fs_free_mem(&map);
	أرجع retval;
}

ساكن errcode_t dx_grow_ونه(ext2_filsys fs, ext2_ino_t dir,
			      بنية ext2_inode *diri,
			      بنية dx_lookup_info *info, فراغ *buf,
			      blk64_t ونې_پاڼه_pblk)
{
	صحيح i;
	errcode_t retval;
	ext2_off64_t اندازه = EXT2_I_اندازه(diri);
	blk64_t lblk, pblk;
	بنية ext2_dir_ثبت *de;
	بنية ext2_dx_countlimit *سر_2;
	صحيح csum_اندازه = 0;
	صحيح شمېر;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_اندازه = حجم(بنية ext2_dx_لکۍ);

	/* Find level which can accommodate new child */
	لكل (i = info->پوړونه - 1; i >= 0; i--)
		إذا (ext2fs_le16_منزل_cpu(info->چوکاټونه[i].سر_2->شمېر) <
		    ext2fs_le16_منزل_cpu(info->چوکاټونه[i].سر_2->حد))
			اكسر;
	/* Need to grow tree depth? */
	إذا (i < 0 && info->پوړونه >= ext2_dir_htree_level(fs))
		أرجع EXT2_ET_DIR_NO_SPACE;
	lblk = اندازه / fs->blocksize;
	اندازه += fs->blocksize;
	retval = ext2fs_inode_اندازه_سېټ(fs, diri, اندازه);
	إذا (retval)
		أرجع retval;
	retval = ext2fs_fallocate(fs,
			EXT2_FALLOCATE_FORCE_INIT | EXT2_FALLOCATE_ZERO_بلاکونه,
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
	إذا (i == (صحيح)info->پوړونه - 1)
		أرجع dx_وېشل_ونې_پاڼه(fs, dir, diri, info, buf, ونې_پاڼه_pblk,
				     lblk, pblk);

	de = buf;
	de->inode = 0;
	ext2fs_dirent_سېټ_name_len(de, 0);
	ext2fs_dirent_سېټ_file_type(de, 0);
	retval = ext2fs_سېټ_rec_len(fs, fs->blocksize, de);
	إذا (retval)
		أرجع retval;
	سر_2 = (بنية ext2_dx_countlimit *) ((حرف *)buf + 8);
	شمېر = ext2fs_le16_منزل_cpu(info->چوکاټونه[i+1].سر_2->شمېر);
	/* Growing tree depth? */
	إذا (i < 0) {
		بنية ext2_dx_ریښه_info *ریښه;

		حافظه_نقل(سر_2, info->چوکاټونه[0].entries,
		       شمېر * حجم(بنية ext2_dx_ثبت));
		سر_2->حد = ext2fs_cpu_منزل_le16(
				(fs->blocksize - (8 + csum_اندازه)) /
				حجم(بنية ext2_dx_ثبت));
		/* head->count gets set by memcpy above to correct value */

		/* Now update tree root */
		info->چوکاټونه[0].سر_2->شمېر = ext2fs_cpu_منزل_le16(1);
		info->چوکاټونه[0].entries[0].بلاک = ext2fs_cpu_منزل_le32(lblk);
		ریښه = (بنية ext2_dx_ریښه_info *)
			((حرف *)info->چوکاټونه[0].buf + EXT2_DX_ریښه_OFF);
		ریښه->غیرمستقیم_پوړونه++;
	} وإلا {
		/* Splitting internal node in two */
		صحيح count1 = شمېر / 2;
		صحيح count2 = شمېر - count1;
		__u32 وېشل_hash = ext2fs_le32_منزل_cpu(info->چوکاټونه[i+1].entries[count1].hash);

		حافظه_نقل(سر_2, info->چوکاټونه[i+1].entries + count1,
		       count2 * حجم(بنية ext2_dx_ثبت));
		سر_2->شمېر = ext2fs_cpu_منزل_le16(count2);
		سر_2->حد = ext2fs_cpu_منزل_le16(
				(fs->blocksize - (8 + csum_اندازه)) /
				حجم(بنية ext2_dx_ثبت));
		info->چوکاټونه[i+1].سر_2->شمېر = ext2fs_cpu_منزل_le16(count1);

		/* Update parent node */
		retval = dx_ورننباسه_ثبت(fs, dir, info, i, وېشل_hash, lblk);
		إذا (retval)
			أرجع retval;

	}
	/* Writeout split block / updated root */
	retval = ext2fs_write_dir_block4(fs, info->چوکاټونه[i+1].pblock,
					 info->چوکاټونه[i+1].buf, 0, dir);
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
	بینښه restart = 0;
	blk64_t ونې_پاڼه_pblk;

	retval = ext2fs_get_mem(fs->blocksize, &blockbuf);
	إذا (retval)
		أرجع retval;

	dx_info.name = name;
	dx_info.namelen = مزي_اوږدوالی(name);
again:
	retval = dx_lookup(fs, dir, diri, &dx_info);
	إذا (retval)
		اذهب free_buf;

	retval = جمع_کړه_dirent_منزل_buf(fs,
		ext2fs_le32_منزل_cpu(dx_info.چوکاټونه[dx_info.پوړونه-1].at->بلاک) & 0x0fffffff,
		blockbuf, dir, diri, name, ino, flags, &ونې_پاڼه_pblk);
	/*
	 * Success or error other than ENOSPC...? We are done. We may need upto
	 * two tries to add entry. One to split htree node and another to add
	 * new leaf block.
	 */
	إذا (restart >= dx_info.پوړونه || retval != EXT2_ET_DIR_NO_SPACE)
		اذهب free_چوکاټونه;
	retval = dx_grow_ونه(fs, dir, diri, &dx_info, blockbuf, ونې_پاڼه_pblk);
	إذا (retval)
		اذهب free_چوکاټونه;
	/* Restart everything now that the tree is larger */
	restart++;
	dx_release(&dx_info);
	اذهب again;
free_چوکاټونه:
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

بنية dx_ریښه
{
	بنية fake_dirent dot;
	حرف dot_name[4];
	بنية fake_dirent dotdot;
	حرف dotdot_name[4];
	بنية ext2_dx_ریښه_info info;
	بنية ext2_dx_ثبت entries[];
};

ساكن صحيح check_dx_ریښه(ext2_filsys fs, ext2_ino_t ino, بنية dx_ریښه *ریښه)
{
	بنية fake_dirent *fde;
	حرف *error_پیغام_2;
	بینښه صحيح rlen;
	حرف *blockend = (حرف *)ریښه + fs->blocksize;

	fde = &ریښه->dot;
	إذا (fde->name_len != 1) {
		error_پیغام_2 = "invalid name_len for '.'";
		اذهب corrupted;
	}
	إذا (مزي_پرتله(ریښه->dot_name, ".", fde->name_len)) {
		error_پیغام_2 = "invalid name for '.'";
		اذهب corrupted;
	}

	ext2fs_get_rec_len(fs, (بنية ext2_dir_ثبت *)fde, &rlen);
	إذا ((حرف *)fde + rlen >= blockend) {
		error_پیغام_2 = "invalid rec_len for '.'";
		اذهب corrupted;
	}

	fde = &ریښه->dotdot;
	إذا (fde->name_len != 2) {
		error_پیغام_2 = "invalid name_len for '..'";
		اذهب corrupted;
	}
	إذا (مزي_پرتله(ریښه->dotdot_name, "..", fde->name_len)) {
		error_پیغام_2 = "invalid name for '..'";
		اذهب corrupted;
	}
	ext2fs_get_rec_len(fs, (بنية ext2_dir_ثبت *)fde, &rlen);
	إذا ((حرف *)fde + rlen >= blockend) {
		error_پیغام_2 = "invalid rec_len for '..'";
		اذهب corrupted;
	}

	أرجع 1;

corrupted:
	fprintf(stderr, "Corrupt dir %u: %s, running e2fsck is recommended\n",
			 ino, error_پیغام_2);
	أرجع 0;
}

ساكن مضمن بنية ext2_dir_ثبت *
ext2fs_بل_ثبت(ext2_filsys fs, بنية ext2_dir_ثبت *p)
{
	بینښه صحيح rlen;

	ext2fs_get_rec_len(fs, p, &rlen);
	أرجع (بنية ext2_dir_ثبت *)((حرف *)p + rlen);
}

ساكن مضمن فراغ dx_سېټ_بلاک(بنية ext2_dx_ثبت *ثبت, بینښه ارزښت)
{
	ثبت->بلاک = ext2fs_cpu_منزل_le32(ارزښت);
}

ساكن مضمن فراغ dx_سېټ_شمېر(بنية ext2_dx_ثبت *entries, بینښه ارزښت)
{
	((بنية ext2_dx_countlimit *) entries)->شمېر = ext2fs_cpu_منزل_le16(ارزښت);
}

ساكن مضمن فراغ dx_سېټ_حد(بنية ext2_dx_ثبت *entries, بینښه ارزښت)
{
	((بنية ext2_dx_countlimit *) entries)->حد = ext2fs_cpu_منزل_le16(ارزښت);
}

ساكن مضمن بینښه dx_ریښه_حد(ext2_filsys fs, بینښه infosize)
{
	بینښه صحيح ثبت_space = fs->blocksize -
			ext2fs_dir_rec_len(1, 0) -
			ext2fs_dir_rec_len(2, 0) - infosize;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		ثبت_space -= حجم(بنية ext2_dx_لکۍ);
	أرجع ثبت_space / حجم(بنية ext2_dx_ثبت);
}

/*
 * This converts a one block unindexed directory to a 3 block indexed
 * directory, and adds the dentry to the indexed directory.  Returns 0 if the
 * index was not created; EAGAIN if it was; or an errcode_t on error.
 */
ساكن errcode_t try_make_شاخصي_dir(ext2_filsys fs, ext2_ino_t dir,
				      ثابت حرف *name, ext2_ino_t ino,
				      صحيح flags)
{
	بنية ext2_inode inode;
	حرف *buf0, *buf1, *سر;
	بنية dx_ریښه *ریښه;
	بنية ext2_dx_ثبت *entries;
	بنية fake_dirent *fde;
	بنية ext2_dir_ثبت *de, *de2;
	blk64_t pblk0, pblk1;
	بینښه صحيح len;
	ثابت بینښه صحيح blocksize = fs->blocksize;
	صحيح csum_اندازه = 0;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	إذا (retval)
		أرجع retval;

	إذا (inode.i_اندازه > fs->blocksize || (inode.i_flags & EXT2_شاخص_FL))
		أرجع 0;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_اندازه = حجم(بنية ext2_dir_ثبت_لکۍ);

	retval = ext2fs_get_mem(blocksize, &buf0);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_get_mem(blocksize, &buf1);
	إذا (retval)
		اذهب بهر_buf0;

	retval = ext2fs_bmap2(fs, dir, &inode, عدم, 0, 0, عدم, &pblk0);
	إذا (retval)
		اذهب بهر_buf1;

	retval = ext2fs_read_dir_block4(fs, pblk0, buf0, 0, dir);
	إذا (retval)
		اذهب بهر_buf1;

	ریښه = (بنية dx_ریښه *)buf0;
	إذا (!check_dx_ریښه(fs, dir, ریښه)) {
		retval = EXT2_ET_DIR_CORRUPTED;
		اذهب بهر_buf1;
	}

	/* The 0th block becomes the root, move the dirents out */
	fde = &ریښه->dotdot;
	de = ext2fs_بل_ثبت(fs, (بنية ext2_dir_ثبت *)fde);
	len = ((حرف *) ریښه) + (blocksize - csum_اندازه) - (حرف *) de;

	/* Allocate new block for the 0th block's dirents */
	retval = ext2fs_bmap2(fs, dir, &inode, عدم, BMAP_ALLOC | BMAP_ZERO, 1,
			      عدم, &pblk1);
	إذا (retval)
		اذهب بهر_buf1;

	حافظه_نقل(buf1, de, len);
	memset(de, 0, len); /* wipe old data */

	de = (بنية ext2_dir_ثبت *)buf1;
	سر = buf1 + len;
	طالما ((حرف *)(de2 = ext2fs_بل_ثبت(fs, de)) < سر) {
#إذا 0
		إذا (ext4_check_dir_ثبت(dir, عدم, de, bh2, buf1, len,
					(حرف *)de - buf1)) {
			retval = EXT2_ET_DIR_CORRUPTED;
			اذهب بهر_buf1;
		}
#د_که_پای
		de = de2;
	}
	retval = ext2fs_سېټ_rec_len(fs,
			buf1 + (blocksize - csum_اندازه) - (حرف *) de, de);
	إذا (retval)
		اذهب بهر_buf1;

	إذا (csum_اندازه)
		ext2fs_initialize_dirent_لکۍ(fs,
				EXT2_DIRENT_لکۍ(buf1, fs->blocksize));

	/* Initialize the root; the dot dirents already exist */
	de = (بنية ext2_dir_ثبت *) (&ریښه->dotdot);
	retval = ext2fs_سېټ_rec_len(fs, blocksize - ext2fs_dir_rec_len(2, 0),
				    de);
	memset (&ریښه->info, 0, حجم(ریښه->info));
	ریښه->info.info_اوږدتیا = حجم(ریښه->info);
	إذا (ext4_hash_دننه_dirent(&inode))
		ریښه->info.hash_نسخه = EXT2_HASH_SIPHASH;
	وإلا
		ریښه->info.hash_نسخه = fs->super->s_def_hash_نسخه;

	entries = ریښه->entries;
	dx_سېټ_بلاک(entries, 1);
	dx_سېټ_شمېر(entries, 1);
	dx_سېټ_حد(entries, dx_ریښه_حد(fs, حجم(ریښه->info)));

	retval = ext2fs_write_dir_block4(fs, pblk1, buf1, 0, dir);
	إذا (retval)
		اذهب بهر_buf1;

	retval = ext2fs_write_dir_block4(fs, pblk0, buf0, 0, dir);
	إذا (retval)
		اذهب بهر_buf1;

	inode.i_flags |= EXT2_شاخص_FL;
	inode.i_اندازه += fs->blocksize;
	retval = ext2fs_write_inode(fs, dir, &inode);
	إذا (retval)
		اذهب بهر_buf1;

	retval = EAGAIN;
بهر_buf1:
	ext2fs_free_mem(&buf1);
بهر_buf0:
	ext2fs_free_mem(&buf0);
	أرجع retval;
}

/*
 * Note: the low 3 bits of the flags field are used as the directory
 * entry filetype.
 */
#که_تعریف_شوی __TURBOC__
 #لارښوونه argsused
#د_که_پای
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

	إذا (inode.i_flags & EXT2_شاخص_FL)
		أرجع dx_link(fs, dir, &inode, name, ino, flags);

	ls.fs = fs;
	ls.name = name;
	ls.namelen = name ? مزي_اوږدوالی(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.بشپړ = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	إذا ((flags & EXT2FS_LINK_APPEND) &&
	    !(inode.i_flags & EXT4_INLINE_معلومات_FL)) {
		blk64_t		lblk, pblk = 0;
		بنية dir_context ctx;

		lblk = (inode.i_اندازه / fs->blocksize) - 1;
		retval = ext2fs_bmap2(fs, dir, &inode, عدم, 0, lblk,
				      عدم, &pblk);
		إذا (retval)
			أرجع retval;

		ctx.dir = dir;
		ctx.flags = DIRENT_FLAG_INCLUDE_تشه;
		ctx.func = link_proc;
		ctx.priv_معلومات = &ls;
		ctx.errcode = 0;
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		إذا (retval)
			أرجع retval;

		ext2fs_process_dir_بلاک(fs, &pblk, lblk, 0, 0, &ctx);
		retval = ctx.errcode;
		ext2fs_free_mem(&ctx.buf);
	} وإلا {
		retval = ext2fs_dir_iterate2(fs, dir,
					     DIRENT_FLAG_INCLUDE_تشه,
					     عدم, link_proc, &ls);
	}
	إذا (retval)
		أرجع retval;
	إذا (ls.err)
		أرجع ls.err;

	إذا (!ls.بشپړ) {
		إذا (!(flags & EXT2FS_LINK_EXPAND))
			أرجع EXT2_ET_DIR_NO_SPACE;

		إذا (ext2fs_has_feature_dir_شاخص(fs->super)) {
			retval = try_make_شاخصي_dir(fs, dir, name, ino,
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
	أرجع ext2fs_has_feature_dir_شاخص(fs->super) &&
		S_ISDIR(inode->i_ډېرپېښ) && (inode->i_flags & EXT2_شاخص_FL);
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
	inode->i_links_شمېر++;

	إذا (ext2fs_dir_is_dx(fs, inode) &&
	    (inode->i_links_شمېر > EXT2_LINK_MAX || inode->i_links_شمېر == 2))
		inode->i_links_شمېر = 1;
}

/*
 * If a directory had nlink == 1, then we should let it be 1. This indicates
 * directory has >EXT2_LINK_MAX subdirs.
 */
فراغ ext2fs_dec_nlink(بنية ext2_inode *inode)
{
	إذا (!S_ISDIR(inode->i_ډېرپېښ) || inode->i_links_شمېر > 2)
		inode->i_links_شمېر--;
}

صحيح ext2fs_dir_link_max(ext2_filsys fs, بنية ext2_inode_large *inode)
{
	أرجع inode->i_links_شمېر >= EXT2_LINK_MAX &&
	       !(ext2fs_dir_is_dx(fs, EXT2_INODE(inode)) &&
		 ext2fs_has_feature_dir_nlink(fs->super));
}

صحيح ext2fs_dir_link_تشه(بنية ext2_inode *inode)
{
	أرجع inode->i_links_شمېر == 2 || inode->i_links_شمېر == 1;
}
