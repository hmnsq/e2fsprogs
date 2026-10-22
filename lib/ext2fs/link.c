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
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

#تعریف_کن EXT2_DX_ریشه_OFF 24

بنية dx_قاب {
	فراغ *buf;
	blk64_t pblock;
	بنية ext2_dx_countlimit *سر;
	بنية ext2_dx_مدخل *entries;
	بنية ext2_dx_مدخل *at;
};

بنية dx_lookup_info {
	ثابت حرف *name;
	صحيح namelen;
	صحيح hash_alg;
	__u32 hash;
	بیعلامت سطوح;
	بنية dx_قاب قابها[EXT4_HTREE_LEVEL];
};

ساكن errcode_t alloc_dx_قاب(ext2_filsys fs, بنية dx_قاب *قاب)
{
	أرجع ext2fs_get_mem(fs->blocksize, &قاب->buf);
}

ساكن فراغ dx_release(بنية dx_lookup_info *info)
{
	بیعلامت level;

	لكل (level = 0; level < info->سطوح; level++) {
		إذا (info->قابها[level].buf == عدم)
			اكسر;
		ext2fs_free_mem(&(info->قابها[level].buf));
	}
	info->سطوح = 0;
}

ساكن فراغ dx_جستجو_مدخل(بنية dx_قاب *قاب, صحيح شمار, __u32 hash)
{
	بنية ext2_dx_مدخل *p, *q, *m;

	p = قاب->entries + 1;
	q = قاب->entries + شمار - 1;
	طالما (p <= q) {
		m = p + (q - p) / 2;
		إذا (ext2fs_le32_مقصد_cpu(m->hash) > hash)
			q = m - 1;
		وإلا
			p = m + 1;
	}
	قاب->at = p - 1;
}

ساكن errcode_t بارگیری_logical_dir_بلوک(ext2_filsys fs, ext2_ino_t dir,
					بنية ext2_inode *diri, blk64_t بلوک,
					blk64_t *pblk, فراغ *buf)
{
	errcode_t errcode;
	صحيح ret_flags;

	errcode = ext2fs_bmap2(fs, dir, diri, عدم, 0, بلوک, &ret_flags,
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
	بنية ext2_dx_ریشه_info *ریشه;
	errcode_t errcode;
	صحيح level = 0;
	صحيح شمار, حد;
	صحيح hash_alg;
	صحيح hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	__u32 minor_hash;
	بنية dx_قاب *قاب;

	errcode = alloc_dx_قاب(fs, &(info->قابها[0]));
	إذا (errcode)
		أرجع errcode;
	info->سطوح = 1;

	errcode = بارگیری_logical_dir_بلوک(fs, dir, diri, 0,
					 &(info->قابها[0].pblock),
					 info->قابها[0].buf);
	إذا (errcode)
		اذهب بیرون_err;
	ریشه = (بنية ext2_dx_ریشه_info *) ((حرف *)info->قابها[0].buf +
					     EXT2_DX_ریشه_OFF);
	hash_alg = ریشه->hash_نسخه;
	إذا (hash_alg != EXT2_HASH_TEA && hash_alg != EXT2_HASH_HALF_MD4 &&
	    hash_alg != EXT2_HASH_LEGACY) {
		errcode = EXT2_ET_DIRHASH_UNSUPP;
		اذهب بیرون_err;
	}
	إذا (hash_alg <= EXT2_HASH_TEA &&
	    fs->super->s_flags & EXT2_FLAGS_UNSIGNED_HASH)
		hash_alg += 3;
	إذا (ریشه->غیرمستقیم_سطوح >= ext2_dir_htree_level(fs)) {
		errcode = EXT2_ET_DIR_CORRUPTED;
		اذهب بیرون_err;
	}
	info->hash_alg = hash_alg;

	errcode = ext2fs_dirhash2(hash_alg, info->name, info->namelen,
				  fs->encoding, hash_flags,
				  fs->super->s_hash_دانه, &info->hash,
				  &minor_hash);
	إذا (errcode)
		اذهب بیرون_err;

	لكل (level = 0; level <= ریشه->غیرمستقیم_سطوح; level++) {
		قاب = &(info->قابها[level]);
		إذا (level > 0) {
			errcode = alloc_dx_قاب(fs, قاب);
			إذا (errcode)
				اذهب بیرون_err;
			info->سطوح++;

			errcode = بارگیری_logical_dir_بلوک(fs, dir, diri,
				ext2fs_le32_مقصد_cpu(info->قابها[level-1].at->بلوک) & 0x0fffffff,
				&(قاب->pblock), قاب->buf);
			إذا (errcode)
				اذهب بیرون_err;
		}
		errcode = ext2fs_get_dx_countlimit(fs, قاب->buf,
						   &(قاب->سر), عدم);
		إذا (errcode)
			اذهب بیرون_err;
		شمار = ext2fs_le16_مقصد_cpu(قاب->سر->شمار);
		حد = ext2fs_le16_مقصد_cpu(قاب->سر->حد);
		قاب->entries = (بنية ext2_dx_مدخل *)(قاب->سر);
		إذا (!شمار || شمار > حد) {
			errcode = EXT2_ET_DIR_CORRUPTED;
			اذهب بیرون_err;
		}

		dx_جستجو_مدخل(قاب, شمار, info->hash);
	}
	أرجع 0;
بیرون_err:
	dx_release(info);
	أرجع errcode;
}

بنية link_struct  {
	ext2_filsys	fs;
	ثابت حرف	*name;
	صحيح		namelen;
	ext2_ino_t	inode;
	صحيح		flags;
	صحيح		انجامشده;
	بیعلامت صحيح	blocksize;
	errcode_t	err;
	بنية ext2_super_بلوک *sb;
};

ساكن صحيح link_proc(ext2_ino_t dir EXT2FS_ATTR((استفادهنشده)),
		     صحيح entru EXT2FS_ATTR((استفادهنشده)),
		     بنية ext2_dir_مدخل *dirent,
		     صحيح	جابه_جایی,
		     صحيح	blocksize,
		     حرف	*buf,
		     فراغ	*priv_داده)
{
	بنية link_struct *ls = (بنية link_struct *) priv_داده;
	بنية ext2_dir_مدخل *بعدی;
	بیعلامت صحيح rec_len, min_rec_len, curr_rec_len;
	صحيح ret = 0;
	صحيح csum_اندازه = 0;

	إذا (ls->انجامشده)
		أرجع DIRENT_ABORT;

	rec_len = EXT2_DIR_REC_LEN(ls->namelen);

	ls->err = ext2fs_get_rec_len(ls->fs, dirent, &curr_rec_len);
	إذا (ls->err)
		أرجع DIRENT_ABORT;

	إذا (ext2fs_has_feature_metadata_csum(ls->fs->super))
		csum_اندازه = حجم(بنية ext2_dir_مدخل_دم);
	/*
	 * See if the following directory entry (if any) is unused;
	 * if so, absorb it into this one.
	 */
	بعدی = (بنية ext2_dir_مدخل *) (buf + جابه_جایی + curr_rec_len);
	إذا ((جابه_جایی + (صحيح) curr_rec_len < blocksize - (8 + csum_اندازه)) &&
	    (بعدی->inode == 0) &&
	    (جابه_جایی + (صحيح) curr_rec_len + (صحيح) بعدی->rec_len <= blocksize)) {
		curr_rec_len += بعدی->rec_len;
		ls->err = ext2fs_مجموعه_rec_len(ls->fs, curr_rec_len, dirent);
		إذا (ls->err)
			أرجع DIRENT_ABORT;
		ret = DIRENT_تغییریافته;
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
		ls->err = ext2fs_مجموعه_rec_len(ls->fs, min_rec_len, dirent);
		إذا (ls->err)
			أرجع DIRENT_ABORT;
		بعدی = (بنية ext2_dir_مدخل *) (buf + جابه_جایی +
						  dirent->rec_len);
		بعدی->inode = 0;
		ext2fs_dirent_مجموعه_name_len(بعدی, 0);
		ext2fs_dirent_مجموعه_file_type(بعدی, 0);
		ls->err = ext2fs_مجموعه_rec_len(ls->fs, rec_len, بعدی);
		إذا (ls->err)
			أرجع DIRENT_ABORT;
		أرجع DIRENT_تغییریافته;
	}

	/*
	 * If we get this far, then the directory entry is not used.
	 * See if we can fit the request entry in.  If so, do it.
	 */
	إذا (curr_rec_len < rec_len)
		أرجع ret;
	dirent->inode = ls->inode;
	ext2fs_dirent_مجموعه_name_len(dirent, ls->namelen);
	strncpy(dirent->name, ls->name, ls->namelen);
	إذا (ext2fs_has_feature_filetype(ls->sb))
		ext2fs_dirent_مجموعه_file_type(dirent,
					    ls->flags & EXT2_FS_LINK_FT_ماسک);

	ls->انجامشده++;
	أرجع DIRENT_ABORT|DIRENT_تغییریافته;
}

ساكن errcode_t جمع_کن_dirent_مقصد_buf(ext2_filsys fs, e2_blkcnt_t blockcnt,
				   حرف *buf, ext2_ino_t dir,
				   بنية ext2_inode *diri, ثابت حرف *name,
				   ext2_ino_t ino, صحيح flags, blk64_t *pblkp)
{
	بنية dir_context ctx;
	بنية link_struct ls;
	errcode_t retval;

	retval = بارگیری_logical_dir_بلوک(fs, dir, diri, blockcnt, pblkp, buf);
	إذا (retval)
		أرجع retval;
	ctx.errcode = 0;
	ctx.func = link_proc;
	ctx.dir = dir;
	ctx.flags = DIRENT_FLAG_INCLUDE_خالی;
	ctx.buf = buf;
	ctx.priv_داده = &ls;

	ls.fs = fs;
	ls.name = name;
	ls.namelen = طول_رشته(name);
	ls.inode = ino;
	ls.flags = flags;
	ls.انجامشده = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	ext2fs_process_dir_بلوک(fs, pblkp, blockcnt, 0, 0, &ctx);
	إذا (ctx.errcode)
		أرجع ctx.errcode;
	إذا (ls.err)
		أرجع ls.err;
	إذا (!ls.انجامشده)
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
				 صحيح شمار, فراغ *from, فراغ *مقصد)
{
	بنية ext2_dir_مدخل *de;
	صحيح i;
	صحيح rec_len = 0;
	errcode_t retval;
	صحيح csum_اندازه = 0;
	فراغ *پایه = مقصد;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_اندازه = حجم(بنية ext2_dir_مدخل_دم);

	لكل (i = 0; i < شمار; i++) {
		de = (بنية ext2_dir_مدخل *) ((حرف *)from + map[i].off);
		rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(de));
		کپی_حافظه(مقصد, de, rec_len);
		retval = ext2fs_مجموعه_rec_len(fs, rec_len, مقصد);
		إذا (retval)
			أرجع retval;
		مقصد = (حرف *)مقصد + rec_len;
	}
	/*
	 * Update rec_len of the last dir entry to stretch to the end of block
	 */
	مقصد = (حرف *)مقصد - rec_len;
	rec_len = fs->blocksize - ((حرف *)مقصد - (حرف *)پایه) - csum_اندازه;
	retval = ext2fs_مجموعه_rec_len(fs, rec_len, مقصد);
	إذا (retval)
		أرجع retval;
	إذا (csum_اندازه)
		ext2fs_initialize_dirent_دم(fs,
				EXT2_DIRENT_دم(پایه, fs->blocksize));
	أرجع 0;
}

ساكن errcode_t dx_درج_مدخل(ext2_filsys fs, ext2_ino_t dir,
				 بنية dx_lookup_info *info, صحيح level,
				 __u32 hash, blk64_t lblk)
{
	صحيح pcount;
	بنية ext2_dx_مدخل *بالاترین, *new;

	pcount = ext2fs_le16_مقصد_cpu(info->قابها[level].سر->شمار);
	بالاترین = info->قابها[level].entries + pcount;
	new = info->قابها[level].at + 1;
	memmove(new + 1, new, (حرف *)بالاترین - (حرف *)new);
	new->hash = ext2fs_cpu_مقصد_le32(hash);
	new->بلوک = ext2fs_cpu_مقصد_le32(lblk);
	info->قابها[level].سر->شمار = ext2fs_cpu_مقصد_le16(pcount + 1);
	أرجع ext2fs_write_dir_block4(fs, info->قابها[level].pblock,
				       info->قابها[level].buf, 0, dir);
}

ساكن errcode_t dx_تقسیم_برگ(ext2_filsys fs, ext2_ino_t dir,
			       بنية ext2_inode *diri,
			       بنية dx_lookup_info *info, فراغ *buf,
			       blk64_t برگ_pblk, blk64_t new_lblk,
			       blk64_t new_pblk)
{
	صحيح hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	بنية ext2_dir_مدخل *de;
	فراغ *buf2;
	errcode_t retval = 0;
	بیعلامت صحيح rec_len;
	بیعلامت صحيح جابه_جایی, move_اندازه;
	صحيح i, شمار = 0;
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
	لكل (جابه_جایی = 0; جابه_جایی < fs->blocksize; جابه_جایی += rec_len) {
		de = (بنية ext2_dir_مدخل *) ((حرف *)buf + جابه_جایی);
		retval = ext2fs_get_rec_len(fs, de, &rec_len);
		إذا (retval)
			اذهب بیرون;
		إذا (ext2fs_dirent_name_len(de) > 0 && de->inode) {
			map[شمار].off = جابه_جایی;
			map[شمار].اندازه = rec_len;
			retval = ext2fs_dirhash2(info->hash_alg, de->name,
					ext2fs_dirent_name_len(de),
					fs->encoding, hash_flags,
					fs->super->s_hash_دانه,
					&(map[شمار].hash),
					&minor_hash);
			إذا (retval)
				اذهب بیرون;
			شمار++;
		}
	}
	qsort(map, شمار, حجم(بنية dx_hash_map), dx_hash_map_cmp);
	move_اندازه = 0;
	/* Find place to split block */
	لكل (i = شمار - 1; i >= 0; i--) {
		إذا (move_اندازه + map[i].اندازه / 2 > fs->blocksize / 2)
			اكسر;
		move_اندازه += map[i].اندازه;
	}
	/* Let i be the first entry to move */
	i++;
	/* Move selected directory entries to new block */
	retval = dx_move_dirents(fs, map + i, شمار - i, buf, buf2);
	إذا (retval)
		اذهب بیرون;
	retval = ext2fs_write_dir_block4(fs, new_pblk, buf2, 0, dir);
	إذا (retval)
		اذهب بیرون;
	/* Repack remaining entries in the old block */
	retval = dx_move_dirents(fs, map, i, buf, buf2);
	إذا (retval)
		اذهب بیرون;
	retval = ext2fs_write_dir_block4(fs, برگ_pblk, buf2, 0, dir);
	إذا (retval)
		اذهب بیرون;
	/* Update parent node */
	continued = map[i].hash == map[i-1].hash;
	retval = dx_درج_مدخل(fs, dir, info, info->سطوح - 1,
				 map[i].hash + continued, new_lblk);
بیرون:
	ext2fs_free_mem(&buf2);
	ext2fs_free_mem(&map);
	أرجع retval;
}

ساكن errcode_t dx_grow_درخت(ext2_filsys fs, ext2_ino_t dir,
			      بنية ext2_inode *diri,
			      بنية dx_lookup_info *info, فراغ *buf,
			      blk64_t برگ_pblk)
{
	صحيح i;
	errcode_t retval;
	ext2_off64_t اندازه = EXT2_I_اندازه(diri);
	blk64_t lblk, pblk;
	بنية ext2_dir_مدخل *de;
	بنية ext2_dx_countlimit *سر;
	صحيح csum_اندازه = 0;
	صحيح شمار;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_اندازه = حجم(بنية ext2_dx_دم);

	/* Find level which can accommodate new child */
	لكل (i = info->سطوح - 1; i >= 0; i--)
		إذا (ext2fs_le16_مقصد_cpu(info->قابها[i].سر->شمار) <
		    ext2fs_le16_مقصد_cpu(info->قابها[i].سر->حد))
			اكسر;
	/* Need to grow tree depth? */
	إذا (i < 0 && info->سطوح >= ext2_dir_htree_level(fs))
		أرجع EXT2_ET_DIR_NO_SPACE;
	lblk = اندازه / fs->blocksize;
	اندازه += fs->blocksize;
	retval = ext2fs_inode_اندازه_مجموعه(fs, diri, اندازه);
	إذا (retval)
		أرجع retval;
	retval = ext2fs_fallocate(fs,
			EXT2_FALLOCATE_FORCE_INIT | EXT2_FALLOCATE_ZERO_بلوکها,
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
	إذا (i == (صحيح)info->سطوح - 1)
		أرجع dx_تقسیم_برگ(fs, dir, diri, info, buf, برگ_pblk,
				     lblk, pblk);

	de = buf;
	de->inode = 0;
	ext2fs_dirent_مجموعه_name_len(de, 0);
	ext2fs_dirent_مجموعه_file_type(de, 0);
	retval = ext2fs_مجموعه_rec_len(fs, fs->blocksize, de);
	إذا (retval)
		أرجع retval;
	سر = (بنية ext2_dx_countlimit *) ((حرف *)buf + 8);
	شمار = ext2fs_le16_مقصد_cpu(info->قابها[i+1].سر->شمار);
	/* Growing tree depth? */
	إذا (i < 0) {
		بنية ext2_dx_ریشه_info *ریشه;

		کپی_حافظه(سر, info->قابها[0].entries,
		       شمار * حجم(بنية ext2_dx_مدخل));
		سر->حد = ext2fs_cpu_مقصد_le16(
				(fs->blocksize - (8 + csum_اندازه)) /
				حجم(بنية ext2_dx_مدخل));
		/* head->count gets set by memcpy above to correct value */

		/* Now update tree root */
		info->قابها[0].سر->شمار = ext2fs_cpu_مقصد_le16(1);
		info->قابها[0].entries[0].بلوک = ext2fs_cpu_مقصد_le32(lblk);
		ریشه = (بنية ext2_dx_ریشه_info *)
			((حرف *)info->قابها[0].buf + EXT2_DX_ریشه_OFF);
		ریشه->غیرمستقیم_سطوح++;
	} وإلا {
		/* Splitting internal node in two */
		صحيح count1 = شمار / 2;
		صحيح count2 = شمار - count1;
		__u32 تقسیم_hash = ext2fs_le32_مقصد_cpu(info->قابها[i+1].entries[count1].hash);

		کپی_حافظه(سر, info->قابها[i+1].entries + count1,
		       count2 * حجم(بنية ext2_dx_مدخل));
		سر->شمار = ext2fs_cpu_مقصد_le16(count2);
		سر->حد = ext2fs_cpu_مقصد_le16(
				(fs->blocksize - (8 + csum_اندازه)) /
				حجم(بنية ext2_dx_مدخل));
		info->قابها[i+1].سر->شمار = ext2fs_cpu_مقصد_le16(count1);

		/* Update parent node */
		retval = dx_درج_مدخل(fs, dir, info, i, تقسیم_hash, lblk);
		إذا (retval)
			أرجع retval;

	}
	/* Writeout split block / updated root */
	retval = ext2fs_write_dir_block4(fs, info->قابها[i+1].pblock,
					 info->قابها[i+1].buf, 0, dir);
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
	بیعلامت restart = 0;
	blk64_t برگ_pblk;

	retval = ext2fs_get_mem(fs->blocksize, &blockbuf);
	إذا (retval)
		أرجع retval;

	dx_info.name = name;
	dx_info.namelen = طول_رشته(name);
again:
	retval = dx_lookup(fs, dir, diri, &dx_info);
	إذا (retval)
		اذهب free_buf;

	retval = جمع_کن_dirent_مقصد_buf(fs,
		ext2fs_le32_مقصد_cpu(dx_info.قابها[dx_info.سطوح-1].at->بلوک) & 0x0fffffff,
		blockbuf, dir, diri, name, ino, flags, &برگ_pblk);
	/*
	 * Success or error other than ENOSPC...? We are done. We may need upto
	 * two tries to add entry. One to split htree node and another to add
	 * new leaf block.
	 */
	إذا (restart >= dx_info.سطوح || retval != EXT2_ET_DIR_NO_SPACE)
		اذهب free_قابها;
	retval = dx_grow_درخت(fs, dir, diri, &dx_info, blockbuf, برگ_pblk);
	إذا (retval)
		اذهب free_قابها;
	/* Restart everything now that the tree is larger */
	restart++;
	dx_release(&dx_info);
	اذهب again;
free_قابها:
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

بنية dx_ریشه
{
	بنية fake_dirent dot;
	حرف dot_name[4];
	بنية fake_dirent dotdot;
	حرف dotdot_name[4];
	بنية ext2_dx_ریشه_info info;
	بنية ext2_dx_مدخل entries[];
};

ساكن صحيح check_dx_ریشه(ext2_filsys fs, ext2_ino_t ino, بنية dx_ریشه *ریشه)
{
	بنية fake_dirent *fde;
	حرف *error_پیام_2;
	بیعلامت صحيح rlen;
	حرف *blockend = (حرف *)ریشه + fs->blocksize;

	fde = &ریشه->dot;
	إذا (fde->name_len != 1) {
		error_پیام_2 = "invalid name_len for '.'";
		اذهب corrupted;
	}
	إذا (مقایسه_رشته(ریشه->dot_name, ".", fde->name_len)) {
		error_پیام_2 = "invalid name for '.'";
		اذهب corrupted;
	}

	ext2fs_get_rec_len(fs, (بنية ext2_dir_مدخل *)fde, &rlen);
	إذا ((حرف *)fde + rlen >= blockend) {
		error_پیام_2 = "invalid rec_len for '.'";
		اذهب corrupted;
	}

	fde = &ریشه->dotdot;
	إذا (fde->name_len != 2) {
		error_پیام_2 = "invalid name_len for '..'";
		اذهب corrupted;
	}
	إذا (مقایسه_رشته(ریشه->dotdot_name, "..", fde->name_len)) {
		error_پیام_2 = "invalid name for '..'";
		اذهب corrupted;
	}
	ext2fs_get_rec_len(fs, (بنية ext2_dir_مدخل *)fde, &rlen);
	إذا ((حرف *)fde + rlen >= blockend) {
		error_پیام_2 = "invalid rec_len for '..'";
		اذهب corrupted;
	}

	أرجع 1;

corrupted:
	fprintf(stderr, "Corrupt dir %u: %s, running e2fsck is recommended\n",
			 ino, error_پیام_2);
	أرجع 0;
}

ساكن مضمن بنية ext2_dir_مدخل *
ext2fs_بعدی_مدخل(ext2_filsys fs, بنية ext2_dir_مدخل *p)
{
	بیعلامت صحيح rlen;

	ext2fs_get_rec_len(fs, p, &rlen);
	أرجع (بنية ext2_dir_مدخل *)((حرف *)p + rlen);
}

ساكن مضمن فراغ dx_مجموعه_بلوک(بنية ext2_dx_مدخل *مدخل, بیعلامت مقدار)
{
	مدخل->بلوک = ext2fs_cpu_مقصد_le32(مقدار);
}

ساكن مضمن فراغ dx_مجموعه_شمار(بنية ext2_dx_مدخل *entries, بیعلامت مقدار)
{
	((بنية ext2_dx_countlimit *) entries)->شمار = ext2fs_cpu_مقصد_le16(مقدار);
}

ساكن مضمن فراغ dx_مجموعه_حد(بنية ext2_dx_مدخل *entries, بیعلامت مقدار)
{
	((بنية ext2_dx_countlimit *) entries)->حد = ext2fs_cpu_مقصد_le16(مقدار);
}

ساكن مضمن بیعلامت dx_ریشه_حد(ext2_filsys fs, بیعلامت infosize)
{
	بیعلامت صحيح مدخل_space = fs->blocksize -
			ext2fs_dir_rec_len(1, 0) -
			ext2fs_dir_rec_len(2, 0) - infosize;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		مدخل_space -= حجم(بنية ext2_dx_دم);
	أرجع مدخل_space / حجم(بنية ext2_dx_مدخل);
}

/*
 * This converts a one block unindexed directory to a 3 block indexed
 * directory, and adds the dentry to the indexed directory.  Returns 0 if the
 * index was not created; EAGAIN if it was; or an errcode_t on error.
 */
ساكن errcode_t try_make_نمایهای_dir(ext2_filsys fs, ext2_ino_t dir,
				      ثابت حرف *name, ext2_ino_t ino,
				      صحيح flags)
{
	بنية ext2_inode inode;
	حرف *buf0, *buf1, *بالاترین;
	بنية dx_ریشه *ریشه;
	بنية ext2_dx_مدخل *entries;
	بنية fake_dirent *fde;
	بنية ext2_dir_مدخل *de, *de2;
	blk64_t pblk0, pblk1;
	بیعلامت صحيح len;
	ثابت بیعلامت صحيح blocksize = fs->blocksize;
	صحيح csum_اندازه = 0;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	إذا (retval)
		أرجع retval;

	إذا (inode.i_اندازه > fs->blocksize || (inode.i_flags & EXT2_شاخص_FL))
		أرجع 0;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_اندازه = حجم(بنية ext2_dir_مدخل_دم);

	retval = ext2fs_get_mem(blocksize, &buf0);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_get_mem(blocksize, &buf1);
	إذا (retval)
		اذهب بیرون_buf0;

	retval = ext2fs_bmap2(fs, dir, &inode, عدم, 0, 0, عدم, &pblk0);
	إذا (retval)
		اذهب بیرون_buf1;

	retval = ext2fs_read_dir_block4(fs, pblk0, buf0, 0, dir);
	إذا (retval)
		اذهب بیرون_buf1;

	ریشه = (بنية dx_ریشه *)buf0;
	إذا (!check_dx_ریشه(fs, dir, ریشه)) {
		retval = EXT2_ET_DIR_CORRUPTED;
		اذهب بیرون_buf1;
	}

	/* The 0th block becomes the root, move the dirents out */
	fde = &ریشه->dotdot;
	de = ext2fs_بعدی_مدخل(fs, (بنية ext2_dir_مدخل *)fde);
	len = ((حرف *) ریشه) + (blocksize - csum_اندازه) - (حرف *) de;

	/* Allocate new block for the 0th block's dirents */
	retval = ext2fs_bmap2(fs, dir, &inode, عدم, BMAP_ALLOC | BMAP_ZERO, 1,
			      عدم, &pblk1);
	إذا (retval)
		اذهب بیرون_buf1;

	کپی_حافظه(buf1, de, len);
	memset(de, 0, len); /* wipe old data */

	de = (بنية ext2_dir_مدخل *)buf1;
	بالاترین = buf1 + len;
	طالما ((حرف *)(de2 = ext2fs_بعدی_مدخل(fs, de)) < بالاترین) {
#إذا 0
		إذا (ext4_check_dir_مدخل(dir, عدم, de, bh2, buf1, len,
					(حرف *)de - buf1)) {
			retval = EXT2_ET_DIR_CORRUPTED;
			اذهب بیرون_buf1;
		}
#پایان_اگر
		de = de2;
	}
	retval = ext2fs_مجموعه_rec_len(fs,
			buf1 + (blocksize - csum_اندازه) - (حرف *) de, de);
	إذا (retval)
		اذهب بیرون_buf1;

	إذا (csum_اندازه)
		ext2fs_initialize_dirent_دم(fs,
				EXT2_DIRENT_دم(buf1, fs->blocksize));

	/* Initialize the root; the dot dirents already exist */
	de = (بنية ext2_dir_مدخل *) (&ریشه->dotdot);
	retval = ext2fs_مجموعه_rec_len(fs, blocksize - ext2fs_dir_rec_len(2, 0),
				    de);
	memset (&ریشه->info, 0, حجم(ریشه->info));
	ریشه->info.info_درازی = حجم(ریشه->info);
	إذا (ext4_hash_درون_dirent(&inode))
		ریشه->info.hash_نسخه = EXT2_HASH_SIPHASH;
	وإلا
		ریشه->info.hash_نسخه = fs->super->s_def_hash_نسخه;

	entries = ریشه->entries;
	dx_مجموعه_بلوک(entries, 1);
	dx_مجموعه_شمار(entries, 1);
	dx_مجموعه_حد(entries, dx_ریشه_حد(fs, حجم(ریشه->info)));

	retval = ext2fs_write_dir_block4(fs, pblk1, buf1, 0, dir);
	إذا (retval)
		اذهب بیرون_buf1;

	retval = ext2fs_write_dir_block4(fs, pblk0, buf0, 0, dir);
	إذا (retval)
		اذهب بیرون_buf1;

	inode.i_flags |= EXT2_شاخص_FL;
	inode.i_اندازه += fs->blocksize;
	retval = ext2fs_write_inode(fs, dir, &inode);
	إذا (retval)
		اذهب بیرون_buf1;

	retval = EAGAIN;
بیرون_buf1:
	ext2fs_free_mem(&buf1);
بیرون_buf0:
	ext2fs_free_mem(&buf0);
	أرجع retval;
}

/*
 * Note: the low 3 bits of the flags field are used as the directory
 * entry filetype.
 */
#اگر_تعریف_شده __TURBOC__
 #رهنمود argsused
#پایان_اگر
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
	ls.namelen = name ? طول_رشته(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.انجامشده = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	إذا ((flags & EXT2FS_LINK_APPEND) &&
	    !(inode.i_flags & EXT4_INLINE_داده_FL)) {
		blk64_t		lblk, pblk = 0;
		بنية dir_context ctx;

		lblk = (inode.i_اندازه / fs->blocksize) - 1;
		retval = ext2fs_bmap2(fs, dir, &inode, عدم, 0, lblk,
				      عدم, &pblk);
		إذا (retval)
			أرجع retval;

		ctx.dir = dir;
		ctx.flags = DIRENT_FLAG_INCLUDE_خالی;
		ctx.func = link_proc;
		ctx.priv_داده = &ls;
		ctx.errcode = 0;
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		إذا (retval)
			أرجع retval;

		ext2fs_process_dir_بلوک(fs, &pblk, lblk, 0, 0, &ctx);
		retval = ctx.errcode;
		ext2fs_free_mem(&ctx.buf);
	} وإلا {
		retval = ext2fs_dir_iterate2(fs, dir,
					     DIRENT_FLAG_INCLUDE_خالی,
					     عدم, link_proc, &ls);
	}
	إذا (retval)
		أرجع retval;
	إذا (ls.err)
		أرجع ls.err;

	إذا (!ls.انجامشده) {
		إذا (!(flags & EXT2FS_LINK_EXPAND))
			أرجع EXT2_ET_DIR_NO_SPACE;

		إذا (ext2fs_has_feature_dir_شاخص(fs->super)) {
			retval = try_make_نمایهای_dir(fs, dir, name, ino,
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
		S_ISDIR(inode->i_نما) && (inode->i_flags & EXT2_شاخص_FL);
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
	inode->i_links_شمار++;

	إذا (ext2fs_dir_is_dx(fs, inode) &&
	    (inode->i_links_شمار > EXT2_LINK_MAX || inode->i_links_شمار == 2))
		inode->i_links_شمار = 1;
}

/*
 * If a directory had nlink == 1, then we should let it be 1. This indicates
 * directory has >EXT2_LINK_MAX subdirs.
 */
فراغ ext2fs_dec_nlink(بنية ext2_inode *inode)
{
	إذا (!S_ISDIR(inode->i_نما) || inode->i_links_شمار > 2)
		inode->i_links_شمار--;
}

صحيح ext2fs_dir_link_max(ext2_filsys fs, بنية ext2_inode_large *inode)
{
	أرجع inode->i_links_شمار >= EXT2_LINK_MAX &&
	       !(ext2fs_dir_is_dx(fs, EXT2_INODE(inode)) &&
		 ext2fs_has_feature_dir_nlink(fs->super));
}

صحيح ext2fs_dir_link_خالی(بنية ext2_inode *inode)
{
	أرجع inode->i_links_شمار == 2 || inode->i_links_شمار == 1;
}
