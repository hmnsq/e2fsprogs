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
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

#تعریف_کرو EXT2_DX_جڑ_OFF 24

بنية dx_چوکھٹا {
	فراغ *buf;
	blk64_t pblock;
	بنية ext2_dx_countlimit *سر;
	بنية ext2_dx_اندراج *entries;
	بنية ext2_dx_اندراج *at;
};

بنية dx_lookup_info {
	ثابت حرف *name;
	صحيح namelen;
	صحيح hash_alg;
	__u32 hash;
	بنانشانی پدھراں;
	بنية dx_چوکھٹا چوکھٹے[EXT4_HTREE_LEVEL];
};

ساكن errcode_t alloc_dx_چوکھٹا(ext2_filsys fs, بنية dx_چوکھٹا *چوکھٹا)
{
	أرجع ext2fs_get_mem(fs->blocksize, &چوکھٹا->buf);
}

ساكن فراغ dx_release(بنية dx_lookup_info *info)
{
	بنانشانی level;

	لكل (level = 0; level < info->پدھراں; level++) {
		إذا (info->چوکھٹے[level].buf == عدم)
			اكسر;
		ext2fs_free_mem(&(info->چوکھٹے[level].buf));
	}
	info->پدھراں = 0;
}

ساكن فراغ dx_تلاش_اندراج(بنية dx_چوکھٹا *چوکھٹا, صحيح گنتی, __u32 hash)
{
	بنية ext2_dx_اندراج *p, *q, *m;

	p = چوکھٹا->entries + 1;
	q = چوکھٹا->entries + گنتی - 1;
	طالما (p <= q) {
		m = p + (q - p) / 2;
		إذا (ext2fs_le32_منزل_cpu(m->hash) > hash)
			q = m - 1;
		وإلا
			p = m + 1;
	}
	چوکھٹا->at = p - 1;
}

ساكن errcode_t لدو_logical_dir_بلاک(ext2_filsys fs, ext2_ino_t dir,
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
	بنية ext2_dx_جڑ_info *جڑ;
	errcode_t errcode;
	صحيح level = 0;
	صحيح گنتی, حد_بندی;
	صحيح hash_alg;
	صحيح hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	__u32 minor_hash;
	بنية dx_چوکھٹا *چوکھٹا;

	errcode = alloc_dx_چوکھٹا(fs, &(info->چوکھٹے[0]));
	إذا (errcode)
		أرجع errcode;
	info->پدھراں = 1;

	errcode = لدو_logical_dir_بلاک(fs, dir, diri, 0,
					 &(info->چوکھٹے[0].pblock),
					 info->چوکھٹے[0].buf);
	إذا (errcode)
		اذهب باہرلا_err;
	جڑ = (بنية ext2_dx_جڑ_info *) ((حرف *)info->چوکھٹے[0].buf +
					     EXT2_DX_جڑ_OFF);
	hash_alg = جڑ->hash_نسخہ;
	إذا (hash_alg != EXT2_HASH_TEA && hash_alg != EXT2_HASH_HALF_MD4 &&
	    hash_alg != EXT2_HASH_LEGACY) {
		errcode = EXT2_ET_DIRHASH_UNSUPP;
		اذهب باہرلا_err;
	}
	إذا (hash_alg <= EXT2_HASH_TEA &&
	    fs->super->s_flags & EXT2_FLAGS_UNSIGNED_HASH)
		hash_alg += 3;
	إذا (جڑ->اسدھا_پدھراں >= ext2_dir_htree_level(fs)) {
		errcode = EXT2_ET_DIR_CORRUPTED;
		اذهب باہرلا_err;
	}
	info->hash_alg = hash_alg;

	errcode = ext2fs_dirhash2(hash_alg, info->name, info->namelen,
				  fs->encoding, hash_flags,
				  fs->super->s_hash_بیج, &info->hash,
				  &minor_hash);
	إذا (errcode)
		اذهب باہرلا_err;

	لكل (level = 0; level <= جڑ->اسدھا_پدھراں; level++) {
		چوکھٹا = &(info->چوکھٹے[level]);
		إذا (level > 0) {
			errcode = alloc_dx_چوکھٹا(fs, چوکھٹا);
			إذا (errcode)
				اذهب باہرلا_err;
			info->پدھراں++;

			errcode = لدو_logical_dir_بلاک(fs, dir, diri,
				ext2fs_le32_منزل_cpu(info->چوکھٹے[level-1].at->بلاک) & 0x0fffffff,
				&(چوکھٹا->pblock), چوکھٹا->buf);
			إذا (errcode)
				اذهب باہرلا_err;
		}
		errcode = ext2fs_get_dx_countlimit(fs, چوکھٹا->buf,
						   &(چوکھٹا->سر), عدم);
		إذا (errcode)
			اذهب باہرلا_err;
		گنتی = ext2fs_le16_منزل_cpu(چوکھٹا->سر->گنتی);
		حد_بندی = ext2fs_le16_منزل_cpu(چوکھٹا->سر->حد_بندی);
		چوکھٹا->entries = (بنية ext2_dx_اندراج *)(چوکھٹا->سر);
		إذا (!گنتی || گنتی > حد_بندی) {
			errcode = EXT2_ET_DIR_CORRUPTED;
			اذهب باہرلا_err;
		}

		dx_تلاش_اندراج(چوکھٹا, گنتی, info->hash);
	}
	أرجع 0;
باہرلا_err:
	dx_release(info);
	أرجع errcode;
}

بنية link_struct  {
	ext2_filsys	fs;
	ثابت حرف	*name;
	صحيح		namelen;
	ext2_ino_t	inode;
	صحيح		flags;
	صحيح		پورا;
	بنانشانی صحيح	blocksize;
	errcode_t	err;
	بنية ext2_super_بلاک *sb;
};

ساكن صحيح link_proc(ext2_ino_t dir EXT2FS_ATTR((بن_ورتیا)),
		     صحيح entru EXT2FS_ATTR((بن_ورتیا)),
		     بنية ext2_dir_اندراج *dirent,
		     صحيح	وتھ,
		     صحيح	blocksize,
		     حرف	*buf,
		     فراغ	*priv_انکڑے)
{
	بنية link_struct *ls = (بنية link_struct *) priv_انکڑے;
	بنية ext2_dir_اندراج *اگلا;
	بنانشانی صحيح rec_len, min_rec_len, curr_rec_len;
	صحيح ret = 0;
	صحيح csum_ناپ = 0;

	إذا (ls->پورا)
		أرجع DIRENT_ABORT;

	rec_len = EXT2_DIR_REC_LEN(ls->namelen);

	ls->err = ext2fs_get_rec_len(ls->fs, dirent, &curr_rec_len);
	إذا (ls->err)
		أرجع DIRENT_ABORT;

	إذا (ext2fs_has_feature_metadata_csum(ls->fs->super))
		csum_ناپ = حجم(بنية ext2_dir_اندراج_پوچھ);
	/*
	 * See if the following directory entry (if any) is unused;
	 * if so, absorb it into this one.
	 */
	اگلا = (بنية ext2_dir_اندراج *) (buf + وتھ + curr_rec_len);
	إذا ((وتھ + (صحيح) curr_rec_len < blocksize - (8 + csum_ناپ)) &&
	    (اگلا->inode == 0) &&
	    (وتھ + (صحيح) curr_rec_len + (صحيح) اگلا->rec_len <= blocksize)) {
		curr_rec_len += اگلا->rec_len;
		ls->err = ext2fs_سیٹ_rec_len(ls->fs, curr_rec_len, dirent);
		إذا (ls->err)
			أرجع DIRENT_ABORT;
		ret = DIRENT_بدلیا;
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
		ls->err = ext2fs_سیٹ_rec_len(ls->fs, min_rec_len, dirent);
		إذا (ls->err)
			أرجع DIRENT_ABORT;
		اگلا = (بنية ext2_dir_اندراج *) (buf + وتھ +
						  dirent->rec_len);
		اگلا->inode = 0;
		ext2fs_dirent_سیٹ_name_len(اگلا, 0);
		ext2fs_dirent_سیٹ_file_type(اگلا, 0);
		ls->err = ext2fs_سیٹ_rec_len(ls->fs, rec_len, اگلا);
		إذا (ls->err)
			أرجع DIRENT_ABORT;
		أرجع DIRENT_بدلیا;
	}

	/*
	 * If we get this far, then the directory entry is not used.
	 * See if we can fit the request entry in.  If so, do it.
	 */
	إذا (curr_rec_len < rec_len)
		أرجع ret;
	dirent->inode = ls->inode;
	ext2fs_dirent_سیٹ_name_len(dirent, ls->namelen);
	strncpy(dirent->name, ls->name, ls->namelen);
	إذا (ext2fs_has_feature_filetype(ls->sb))
		ext2fs_dirent_سیٹ_file_type(dirent,
					    ls->flags & EXT2_FS_LINK_FT_نقاب);

	ls->پورا++;
	أرجع DIRENT_ABORT|DIRENT_بدلیا;
}

ساكن errcode_t جوڑو_dirent_منزل_buf(ext2_filsys fs, e2_blkcnt_t blockcnt,
				   حرف *buf, ext2_ino_t dir,
				   بنية ext2_inode *diri, ثابت حرف *name,
				   ext2_ino_t ino, صحيح flags, blk64_t *pblkp)
{
	بنية dir_context ctx;
	بنية link_struct ls;
	errcode_t retval;

	retval = لدو_logical_dir_بلاک(fs, dir, diri, blockcnt, pblkp, buf);
	إذا (retval)
		أرجع retval;
	ctx.errcode = 0;
	ctx.func = link_proc;
	ctx.dir = dir;
	ctx.flags = DIRENT_FLAG_INCLUDE_خالی_تھاواں;
	ctx.buf = buf;
	ctx.priv_انکڑے = &ls;

	ls.fs = fs;
	ls.name = name;
	ls.namelen = لڑی_لمبائی(name);
	ls.inode = ino;
	ls.flags = flags;
	ls.پورا = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	ext2fs_process_dir_بلاک(fs, pblkp, blockcnt, 0, 0, &ctx);
	إذا (ctx.errcode)
		أرجع ctx.errcode;
	إذا (ls.err)
		أرجع ls.err;
	إذا (!ls.پورا)
		أرجع EXT2_ET_DIR_NO_SPACE;
	أرجع 0;
}

بنية dx_hash_map {
	__u32 hash;
	صحيح ناپ;
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
				 صحيح گنتی, فراغ *from, فراغ *منزل)
{
	بنية ext2_dir_اندراج *de;
	صحيح i;
	صحيح rec_len = 0;
	errcode_t retval;
	صحيح csum_ناپ = 0;
	فراغ *ادھار = منزل;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_ناپ = حجم(بنية ext2_dir_اندراج_پوچھ);

	لكل (i = 0; i < گنتی; i++) {
		de = (بنية ext2_dir_اندراج *) ((حرف *)from + map[i].off);
		rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(de));
		حافظہ_نقل(منزل, de, rec_len);
		retval = ext2fs_سیٹ_rec_len(fs, rec_len, منزل);
		إذا (retval)
			أرجع retval;
		منزل = (حرف *)منزل + rec_len;
	}
	/*
	 * Update rec_len of the last dir entry to stretch to the end of block
	 */
	منزل = (حرف *)منزل - rec_len;
	rec_len = fs->blocksize - ((حرف *)منزل - (حرف *)ادھار) - csum_ناپ;
	retval = ext2fs_سیٹ_rec_len(fs, rec_len, منزل);
	إذا (retval)
		أرجع retval;
	إذا (csum_ناپ)
		ext2fs_initialize_dirent_پوچھ(fs,
				EXT2_DIRENT_پوچھ(ادھار, fs->blocksize));
	أرجع 0;
}

ساكن errcode_t dx_داخل_کرو_اندراج(ext2_filsys fs, ext2_ino_t dir,
				 بنية dx_lookup_info *info, صحيح level,
				 __u32 hash, blk64_t lblk)
{
	صحيح pcount;
	بنية ext2_dx_اندراج *چوٹی, *new;

	pcount = ext2fs_le16_منزل_cpu(info->چوکھٹے[level].سر->گنتی);
	چوٹی = info->چوکھٹے[level].entries + pcount;
	new = info->چوکھٹے[level].at + 1;
	memmove(new + 1, new, (حرف *)چوٹی - (حرف *)new);
	new->hash = ext2fs_cpu_منزل_le32(hash);
	new->بلاک = ext2fs_cpu_منزل_le32(lblk);
	info->چوکھٹے[level].سر->گنتی = ext2fs_cpu_منزل_le16(pcount + 1);
	أرجع ext2fs_write_dir_block4(fs, info->چوکھٹے[level].pblock,
				       info->چوکھٹے[level].buf, 0, dir);
}

ساكن errcode_t dx_ونڈو_پتی(ext2_filsys fs, ext2_ino_t dir,
			       بنية ext2_inode *diri,
			       بنية dx_lookup_info *info, فراغ *buf,
			       blk64_t پتی_pblk, blk64_t new_lblk,
			       blk64_t new_pblk)
{
	صحيح hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	بنية ext2_dir_اندراج *de;
	فراغ *buf2;
	errcode_t retval = 0;
	بنانشانی صحيح rec_len;
	بنانشانی صحيح وتھ, move_ناپ;
	صحيح i, گنتی = 0;
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
	لكل (وتھ = 0; وتھ < fs->blocksize; وتھ += rec_len) {
		de = (بنية ext2_dir_اندراج *) ((حرف *)buf + وتھ);
		retval = ext2fs_get_rec_len(fs, de, &rec_len);
		إذا (retval)
			اذهب باہرلا;
		إذا (ext2fs_dirent_name_len(de) > 0 && de->inode) {
			map[گنتی].off = وتھ;
			map[گنتی].ناپ = rec_len;
			retval = ext2fs_dirhash2(info->hash_alg, de->name,
					ext2fs_dirent_name_len(de),
					fs->encoding, hash_flags,
					fs->super->s_hash_بیج,
					&(map[گنتی].hash),
					&minor_hash);
			إذا (retval)
				اذهب باہرلا;
			گنتی++;
		}
	}
	qsort(map, گنتی, حجم(بنية dx_hash_map), dx_hash_map_cmp);
	move_ناپ = 0;
	/* Find place to split block */
	لكل (i = گنتی - 1; i >= 0; i--) {
		إذا (move_ناپ + map[i].ناپ / 2 > fs->blocksize / 2)
			اكسر;
		move_ناپ += map[i].ناپ;
	}
	/* Let i be the first entry to move */
	i++;
	/* Move selected directory entries to new block */
	retval = dx_move_dirents(fs, map + i, گنتی - i, buf, buf2);
	إذا (retval)
		اذهب باہرلا;
	retval = ext2fs_write_dir_block4(fs, new_pblk, buf2, 0, dir);
	إذا (retval)
		اذهب باہرلا;
	/* Repack remaining entries in the old block */
	retval = dx_move_dirents(fs, map, i, buf, buf2);
	إذا (retval)
		اذهب باہرلا;
	retval = ext2fs_write_dir_block4(fs, پتی_pblk, buf2, 0, dir);
	إذا (retval)
		اذهب باہرلا;
	/* Update parent node */
	continued = map[i].hash == map[i-1].hash;
	retval = dx_داخل_کرو_اندراج(fs, dir, info, info->پدھراں - 1,
				 map[i].hash + continued, new_lblk);
باہرلا:
	ext2fs_free_mem(&buf2);
	ext2fs_free_mem(&map);
	أرجع retval;
}

ساكن errcode_t dx_grow_درخت(ext2_filsys fs, ext2_ino_t dir,
			      بنية ext2_inode *diri,
			      بنية dx_lookup_info *info, فراغ *buf,
			      blk64_t پتی_pblk)
{
	صحيح i;
	errcode_t retval;
	ext2_off64_t ناپ = EXT2_I_ناپ(diri);
	blk64_t lblk, pblk;
	بنية ext2_dir_اندراج *de;
	بنية ext2_dx_countlimit *سر;
	صحيح csum_ناپ = 0;
	صحيح گنتی;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_ناپ = حجم(بنية ext2_dx_پوچھ);

	/* Find level which can accommodate new child */
	لكل (i = info->پدھراں - 1; i >= 0; i--)
		إذا (ext2fs_le16_منزل_cpu(info->چوکھٹے[i].سر->گنتی) <
		    ext2fs_le16_منزل_cpu(info->چوکھٹے[i].سر->حد_بندی))
			اكسر;
	/* Need to grow tree depth? */
	إذا (i < 0 && info->پدھراں >= ext2_dir_htree_level(fs))
		أرجع EXT2_ET_DIR_NO_SPACE;
	lblk = ناپ / fs->blocksize;
	ناپ += fs->blocksize;
	retval = ext2fs_inode_ناپ_سیٹ(fs, diri, ناپ);
	إذا (retval)
		أرجع retval;
	retval = ext2fs_fallocate(fs,
			EXT2_FALLOCATE_FORCE_INIT | EXT2_FALLOCATE_ZERO_بلاکاں,
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
	إذا (i == (صحيح)info->پدھراں - 1)
		أرجع dx_ونڈو_پتی(fs, dir, diri, info, buf, پتی_pblk,
				     lblk, pblk);

	de = buf;
	de->inode = 0;
	ext2fs_dirent_سیٹ_name_len(de, 0);
	ext2fs_dirent_سیٹ_file_type(de, 0);
	retval = ext2fs_سیٹ_rec_len(fs, fs->blocksize, de);
	إذا (retval)
		أرجع retval;
	سر = (بنية ext2_dx_countlimit *) ((حرف *)buf + 8);
	گنتی = ext2fs_le16_منزل_cpu(info->چوکھٹے[i+1].سر->گنتی);
	/* Growing tree depth? */
	إذا (i < 0) {
		بنية ext2_dx_جڑ_info *جڑ;

		حافظہ_نقل(سر, info->چوکھٹے[0].entries,
		       گنتی * حجم(بنية ext2_dx_اندراج));
		سر->حد_بندی = ext2fs_cpu_منزل_le16(
				(fs->blocksize - (8 + csum_ناپ)) /
				حجم(بنية ext2_dx_اندراج));
		/* head->count gets set by memcpy above to correct value */

		/* Now update tree root */
		info->چوکھٹے[0].سر->گنتی = ext2fs_cpu_منزل_le16(1);
		info->چوکھٹے[0].entries[0].بلاک = ext2fs_cpu_منزل_le32(lblk);
		جڑ = (بنية ext2_dx_جڑ_info *)
			((حرف *)info->چوکھٹے[0].buf + EXT2_DX_جڑ_OFF);
		جڑ->اسدھا_پدھراں++;
	} وإلا {
		/* Splitting internal node in two */
		صحيح count1 = گنتی / 2;
		صحيح count2 = گنتی - count1;
		__u32 ونڈو_hash = ext2fs_le32_منزل_cpu(info->چوکھٹے[i+1].entries[count1].hash);

		حافظہ_نقل(سر, info->چوکھٹے[i+1].entries + count1,
		       count2 * حجم(بنية ext2_dx_اندراج));
		سر->گنتی = ext2fs_cpu_منزل_le16(count2);
		سر->حد_بندی = ext2fs_cpu_منزل_le16(
				(fs->blocksize - (8 + csum_ناپ)) /
				حجم(بنية ext2_dx_اندراج));
		info->چوکھٹے[i+1].سر->گنتی = ext2fs_cpu_منزل_le16(count1);

		/* Update parent node */
		retval = dx_داخل_کرو_اندراج(fs, dir, info, i, ونڈو_hash, lblk);
		إذا (retval)
			أرجع retval;

	}
	/* Writeout split block / updated root */
	retval = ext2fs_write_dir_block4(fs, info->چوکھٹے[i+1].pblock,
					 info->چوکھٹے[i+1].buf, 0, dir);
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
	بنانشانی restart = 0;
	blk64_t پتی_pblk;

	retval = ext2fs_get_mem(fs->blocksize, &blockbuf);
	إذا (retval)
		أرجع retval;

	dx_info.name = name;
	dx_info.namelen = لڑی_لمبائی(name);
again:
	retval = dx_lookup(fs, dir, diri, &dx_info);
	إذا (retval)
		اذهب free_buf;

	retval = جوڑو_dirent_منزل_buf(fs,
		ext2fs_le32_منزل_cpu(dx_info.چوکھٹے[dx_info.پدھراں-1].at->بلاک) & 0x0fffffff,
		blockbuf, dir, diri, name, ino, flags, &پتی_pblk);
	/*
	 * Success or error other than ENOSPC...? We are done. We may need upto
	 * two tries to add entry. One to split htree node and another to add
	 * new leaf block.
	 */
	إذا (restart >= dx_info.پدھراں || retval != EXT2_ET_DIR_NO_SPACE)
		اذهب free_چوکھٹے;
	retval = dx_grow_درخت(fs, dir, diri, &dx_info, blockbuf, پتی_pblk);
	إذا (retval)
		اذهب free_چوکھٹے;
	/* Restart everything now that the tree is larger */
	restart++;
	dx_release(&dx_info);
	اذهب again;
free_چوکھٹے:
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

بنية dx_جڑ
{
	بنية fake_dirent dot;
	حرف dot_name[4];
	بنية fake_dirent dotdot;
	حرف dotdot_name[4];
	بنية ext2_dx_جڑ_info info;
	بنية ext2_dx_اندراج entries[];
};

ساكن صحيح check_dx_جڑ(ext2_filsys fs, ext2_ino_t ino, بنية dx_جڑ *جڑ)
{
	بنية fake_dirent *fde;
	حرف *error_پیغام;
	بنانشانی صحيح rlen;
	حرف *blockend = (حرف *)جڑ + fs->blocksize;

	fde = &جڑ->dot;
	إذا (fde->name_len != 1) {
		error_پیغام = "invalid name_len for '.'";
		اذهب corrupted;
	}
	إذا (لڑی_مقابلہ(جڑ->dot_name, ".", fde->name_len)) {
		error_پیغام = "invalid name for '.'";
		اذهب corrupted;
	}

	ext2fs_get_rec_len(fs, (بنية ext2_dir_اندراج *)fde, &rlen);
	إذا ((حرف *)fde + rlen >= blockend) {
		error_پیغام = "invalid rec_len for '.'";
		اذهب corrupted;
	}

	fde = &جڑ->dotdot;
	إذا (fde->name_len != 2) {
		error_پیغام = "invalid name_len for '..'";
		اذهب corrupted;
	}
	إذا (لڑی_مقابلہ(جڑ->dotdot_name, "..", fde->name_len)) {
		error_پیغام = "invalid name for '..'";
		اذهب corrupted;
	}
	ext2fs_get_rec_len(fs, (بنية ext2_dir_اندراج *)fde, &rlen);
	إذا ((حرف *)fde + rlen >= blockend) {
		error_پیغام = "invalid rec_len for '..'";
		اذهب corrupted;
	}

	أرجع 1;

corrupted:
	fprintf(stderr, "Corrupt dir %u: %s, running e2fsck is recommended\n",
			 ino, error_پیغام);
	أرجع 0;
}

ساكن مضمن بنية ext2_dir_اندراج *
ext2fs_اگلا_اندراج(ext2_filsys fs, بنية ext2_dir_اندراج *p)
{
	بنانشانی صحيح rlen;

	ext2fs_get_rec_len(fs, p, &rlen);
	أرجع (بنية ext2_dir_اندراج *)((حرف *)p + rlen);
}

ساكن مضمن فراغ dx_سیٹ_بلاک(بنية ext2_dx_اندراج *اندراج, بنانشانی قدر)
{
	اندراج->بلاک = ext2fs_cpu_منزل_le32(قدر);
}

ساكن مضمن فراغ dx_سیٹ_گنتی(بنية ext2_dx_اندراج *entries, بنانشانی قدر)
{
	((بنية ext2_dx_countlimit *) entries)->گنتی = ext2fs_cpu_منزل_le16(قدر);
}

ساكن مضمن فراغ dx_سیٹ_حد_بندی(بنية ext2_dx_اندراج *entries, بنانشانی قدر)
{
	((بنية ext2_dx_countlimit *) entries)->حد_بندی = ext2fs_cpu_منزل_le16(قدر);
}

ساكن مضمن بنانشانی dx_جڑ_حد_بندی(ext2_filsys fs, بنانشانی infosize)
{
	بنانشانی صحيح اندراج_space = fs->blocksize -
			ext2fs_dir_rec_len(1, 0) -
			ext2fs_dir_rec_len(2, 0) - infosize;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		اندراج_space -= حجم(بنية ext2_dx_پوچھ);
	أرجع اندراج_space / حجم(بنية ext2_dx_اندراج);
}

/*
 * This converts a one block unindexed directory to a 3 block indexed
 * directory, and adds the dentry to the indexed directory.  Returns 0 if the
 * index was not created; EAGAIN if it was; or an errcode_t on error.
 */
ساكن errcode_t try_make_اشاریہ_دار_dir(ext2_filsys fs, ext2_ino_t dir,
				      ثابت حرف *name, ext2_ino_t ino,
				      صحيح flags)
{
	بنية ext2_inode inode;
	حرف *buf0, *buf1, *چوٹی;
	بنية dx_جڑ *جڑ;
	بنية ext2_dx_اندراج *entries;
	بنية fake_dirent *fde;
	بنية ext2_dir_اندراج *de, *de2;
	blk64_t pblk0, pblk1;
	بنانشانی صحيح len;
	ثابت بنانشانی صحيح blocksize = fs->blocksize;
	صحيح csum_ناپ = 0;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	إذا (retval)
		أرجع retval;

	إذا (inode.i_ناپ > fs->blocksize || (inode.i_flags & EXT2_اشاریہ_FL))
		أرجع 0;

	إذا (ext2fs_has_feature_metadata_csum(fs->super))
		csum_ناپ = حجم(بنية ext2_dir_اندراج_پوچھ);

	retval = ext2fs_get_mem(blocksize, &buf0);
	إذا (retval)
		أرجع retval;

	retval = ext2fs_get_mem(blocksize, &buf1);
	إذا (retval)
		اذهب باہرلا_buf0;

	retval = ext2fs_bmap2(fs, dir, &inode, عدم, 0, 0, عدم, &pblk0);
	إذا (retval)
		اذهب باہرلا_buf1;

	retval = ext2fs_read_dir_block4(fs, pblk0, buf0, 0, dir);
	إذا (retval)
		اذهب باہرلا_buf1;

	جڑ = (بنية dx_جڑ *)buf0;
	إذا (!check_dx_جڑ(fs, dir, جڑ)) {
		retval = EXT2_ET_DIR_CORRUPTED;
		اذهب باہرلا_buf1;
	}

	/* The 0th block becomes the root, move the dirents out */
	fde = &جڑ->dotdot;
	de = ext2fs_اگلا_اندراج(fs, (بنية ext2_dir_اندراج *)fde);
	len = ((حرف *) جڑ) + (blocksize - csum_ناپ) - (حرف *) de;

	/* Allocate new block for the 0th block's dirents */
	retval = ext2fs_bmap2(fs, dir, &inode, عدم, BMAP_ALLOC | BMAP_ZERO, 1,
			      عدم, &pblk1);
	إذا (retval)
		اذهب باہرلا_buf1;

	حافظہ_نقل(buf1, de, len);
	memset(de, 0, len); /* wipe old data */

	de = (بنية ext2_dir_اندراج *)buf1;
	چوٹی = buf1 + len;
	طالما ((حرف *)(de2 = ext2fs_اگلا_اندراج(fs, de)) < چوٹی) {
#إذا 0
		إذا (ext4_check_dir_اندراج(dir, عدم, de, bh2, buf1, len,
					(حرف *)de - buf1)) {
			retval = EXT2_ET_DIR_CORRUPTED;
			اذهب باہرلا_buf1;
		}
#جے_انت
		de = de2;
	}
	retval = ext2fs_سیٹ_rec_len(fs,
			buf1 + (blocksize - csum_ناپ) - (حرف *) de, de);
	إذا (retval)
		اذهب باہرلا_buf1;

	إذا (csum_ناپ)
		ext2fs_initialize_dirent_پوچھ(fs,
				EXT2_DIRENT_پوچھ(buf1, fs->blocksize));

	/* Initialize the root; the dot dirents already exist */
	de = (بنية ext2_dir_اندراج *) (&جڑ->dotdot);
	retval = ext2fs_سیٹ_rec_len(fs, blocksize - ext2fs_dir_rec_len(2, 0),
				    de);
	memset (&جڑ->info, 0, حجم(جڑ->info));
	جڑ->info.info_لمائی = حجم(جڑ->info);
	إذا (ext4_hash_وچ_dirent(&inode))
		جڑ->info.hash_نسخہ = EXT2_HASH_SIPHASH;
	وإلا
		جڑ->info.hash_نسخہ = fs->super->s_def_hash_نسخہ;

	entries = جڑ->entries;
	dx_سیٹ_بلاک(entries, 1);
	dx_سیٹ_گنتی(entries, 1);
	dx_سیٹ_حد_بندی(entries, dx_جڑ_حد_بندی(fs, حجم(جڑ->info)));

	retval = ext2fs_write_dir_block4(fs, pblk1, buf1, 0, dir);
	إذا (retval)
		اذهب باہرلا_buf1;

	retval = ext2fs_write_dir_block4(fs, pblk0, buf0, 0, dir);
	إذا (retval)
		اذهب باہرلا_buf1;

	inode.i_flags |= EXT2_اشاریہ_FL;
	inode.i_ناپ += fs->blocksize;
	retval = ext2fs_write_inode(fs, dir, &inode);
	إذا (retval)
		اذهب باہرلا_buf1;

	retval = EAGAIN;
باہرلا_buf1:
	ext2fs_free_mem(&buf1);
باہرلا_buf0:
	ext2fs_free_mem(&buf0);
	أرجع retval;
}

/*
 * Note: the low 3 bits of the flags field are used as the directory
 * entry filetype.
 */
#جے_تعریف_شدہ __TURBOC__
 #ہدایت argsused
#جے_انت
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

	إذا (inode.i_flags & EXT2_اشاریہ_FL)
		أرجع dx_link(fs, dir, &inode, name, ino, flags);

	ls.fs = fs;
	ls.name = name;
	ls.namelen = name ? لڑی_لمبائی(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.پورا = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	إذا ((flags & EXT2FS_LINK_APPEND) &&
	    !(inode.i_flags & EXT4_INLINE_انکڑے_FL)) {
		blk64_t		lblk, pblk = 0;
		بنية dir_context ctx;

		lblk = (inode.i_ناپ / fs->blocksize) - 1;
		retval = ext2fs_bmap2(fs, dir, &inode, عدم, 0, lblk,
				      عدم, &pblk);
		إذا (retval)
			أرجع retval;

		ctx.dir = dir;
		ctx.flags = DIRENT_FLAG_INCLUDE_خالی_تھاواں;
		ctx.func = link_proc;
		ctx.priv_انکڑے = &ls;
		ctx.errcode = 0;
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		إذا (retval)
			أرجع retval;

		ext2fs_process_dir_بلاک(fs, &pblk, lblk, 0, 0, &ctx);
		retval = ctx.errcode;
		ext2fs_free_mem(&ctx.buf);
	} وإلا {
		retval = ext2fs_dir_iterate2(fs, dir,
					     DIRENT_FLAG_INCLUDE_خالی_تھاواں,
					     عدم, link_proc, &ls);
	}
	إذا (retval)
		أرجع retval;
	إذا (ls.err)
		أرجع ls.err;

	إذا (!ls.پورا) {
		إذا (!(flags & EXT2FS_LINK_EXPAND))
			أرجع EXT2_ET_DIR_NO_SPACE;

		إذا (ext2fs_has_feature_dir_اشاریہ(fs->super)) {
			retval = try_make_اشاریہ_دار_dir(fs, dir, name, ino,
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
	أرجع ext2fs_has_feature_dir_اشاریہ(fs->super) &&
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
فراغ ext2fs_inc_nlink(ext2_filsys fs, بنية ext2_inode *inode)
{
	inode->i_links_گنتی++;

	إذا (ext2fs_dir_is_dx(fs, inode) &&
	    (inode->i_links_گنتی > EXT2_LINK_MAX || inode->i_links_گنتی == 2))
		inode->i_links_گنتی = 1;
}

/*
 * If a directory had nlink == 1, then we should let it be 1. This indicates
 * directory has >EXT2_LINK_MAX subdirs.
 */
فراغ ext2fs_dec_nlink(بنية ext2_inode *inode)
{
	إذا (!S_ISDIR(inode->i_منوال) || inode->i_links_گنتی > 2)
		inode->i_links_گنتی--;
}

صحيح ext2fs_dir_link_max(ext2_filsys fs, بنية ext2_inode_large *inode)
{
	أرجع inode->i_links_گنتی >= EXT2_LINK_MAX &&
	       !(ext2fs_dir_is_dx(fs, EXT2_INODE(inode)) &&
		 ext2fs_has_feature_dir_nlink(fs->super));
}

صحيح ext2fs_dir_link_خالی_تھاواں(بنية ext2_inode *inode)
{
	أرجع inode->i_links_گنتی == 2 || inode->i_links_گنتی == 1;
}
