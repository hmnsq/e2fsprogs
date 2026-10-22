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
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

#परिभाषय EXT2_DX_मूलम्_OFF 24

संरचना dx_frame {
	शून्य *buf;
	blk64_t pblock;
	संरचना ext2_dx_countlimit *शीर्ष;
	संरचना ext2_dx_प्रविष्टिः *entries;
	संरचना ext2_dx_प्रविष्टिः *at;
};

संरचना dx_lookup_info {
	ध्रुव वर्ण *name;
	पूर्णाङ्क namelen;
	पूर्णाङ्क hash_alg;
	__u32 hash;
	अचिह्नित स्तराः;
	संरचना dx_frame चौकटाः[EXT4_HTREE_LEVEL];
};

स्थिर errcode_t alloc_dx_frame(ext2_filsys fs, संरचना dx_frame *frame)
{
	प्रत्यागम ext2fs_get_mem(fs->blocksize, &frame->buf);
}

स्थिर शून्य dx_release(संरचना dx_lookup_info *info)
{
	अचिह्नित level;

	प्रति (level = 0; level < info->स्तराः; level++) {
		यदि (info->चौकटाः[level].buf == रिक्त)
			भङ्ग;
		ext2fs_free_mem(&(info->चौकटाः[level].buf));
	}
	info->स्तराः = 0;
}

स्थिर शून्य dx_अन्वेषणम्_प्रविष्टिः(संरचना dx_frame *frame, पूर्णाङ्क गणनम्, __u32 hash)
{
	संरचना ext2_dx_प्रविष्टिः *p, *q, *m;

	p = frame->entries + 1;
	q = frame->entries + गणनम् - 1;
	यावत् (p <= q) {
		m = p + (q - p) / 2;
		यदि (ext2fs_le32_गन्तव्यम्_cpu(m->hash) > hash)
			q = m - 1;
		अन्यथा
			p = m + 1;
	}
	frame->at = p - 1;
}

स्थिर errcode_t आरोपय_logical_dir_खण्डम्(ext2_filsys fs, ext2_ino_t dir,
					संरचना ext2_inode *diri, blk64_t खण्डम्,
					blk64_t *pblk, शून्य *buf)
{
	errcode_t errcode;
	पूर्णाङ्क ret_flags;

	errcode = ext2fs_bmap2(fs, dir, diri, रिक्त, 0, खण्डम्, &ret_flags,
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
	संरचना ext2_dx_मूलम्_info *मूलम्;
	errcode_t errcode;
	पूर्णाङ्क level = 0;
	पूर्णाङ्क गणनम्, सीमा;
	पूर्णाङ्क hash_alg;
	पूर्णाङ्क hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	__u32 minor_hash;
	संरचना dx_frame *frame;

	errcode = alloc_dx_frame(fs, &(info->चौकटाः[0]));
	यदि (errcode)
		प्रत्यागम errcode;
	info->स्तराः = 1;

	errcode = आरोपय_logical_dir_खण्डम्(fs, dir, diri, 0,
					 &(info->चौकटाः[0].pblock),
					 info->चौकटाः[0].buf);
	यदि (errcode)
		गच्छ बहिर्गमनम्_err;
	मूलम् = (संरचना ext2_dx_मूलम्_info *) ((वर्ण *)info->चौकटाः[0].buf +
					     EXT2_DX_मूलम्_OFF);
	hash_alg = मूलम्->hash_संस्करणम्;
	यदि (hash_alg != EXT2_HASH_TEA && hash_alg != EXT2_HASH_HALF_MD4 &&
	    hash_alg != EXT2_HASH_LEGACY) {
		errcode = EXT2_ET_DIRHASH_UNSUPP;
		गच्छ बहिर्गमनम्_err;
	}
	यदि (hash_alg <= EXT2_HASH_TEA &&
	    fs->super->s_flags & EXT2_FLAGS_UNSIGNED_HASH)
		hash_alg += 3;
	यदि (मूलम्->परोक्षम्_स्तराः >= ext2_dir_htree_level(fs)) {
		errcode = EXT2_ET_DIR_CORRUPTED;
		गच्छ बहिर्गमनम्_err;
	}
	info->hash_alg = hash_alg;

	errcode = ext2fs_dirhash2(hash_alg, info->name, info->namelen,
				  fs->encoding, hash_flags,
				  fs->super->s_hash_बीजम्, &info->hash,
				  &minor_hash);
	यदि (errcode)
		गच्छ बहिर्गमनम्_err;

	प्रति (level = 0; level <= मूलम्->परोक्षम्_स्तराः; level++) {
		frame = &(info->चौकटाः[level]);
		यदि (level > 0) {
			errcode = alloc_dx_frame(fs, frame);
			यदि (errcode)
				गच्छ बहिर्गमनम्_err;
			info->स्तराः++;

			errcode = आरोपय_logical_dir_खण्डम्(fs, dir, diri,
				ext2fs_le32_गन्तव्यम्_cpu(info->चौकटाः[level-1].at->खण्डम्) & 0x0fffffff,
				&(frame->pblock), frame->buf);
			यदि (errcode)
				गच्छ बहिर्गमनम्_err;
		}
		errcode = ext2fs_get_dx_countlimit(fs, frame->buf,
						   &(frame->शीर्ष), रिक्त);
		यदि (errcode)
			गच्छ बहिर्गमनम्_err;
		गणनम् = ext2fs_le16_गन्तव्यम्_cpu(frame->शीर्ष->गणनम्);
		सीमा = ext2fs_le16_गन्तव्यम्_cpu(frame->शीर्ष->सीमा);
		frame->entries = (संरचना ext2_dx_प्रविष्टिः *)(frame->शीर्ष);
		यदि (!गणनम् || गणनम् > सीमा) {
			errcode = EXT2_ET_DIR_CORRUPTED;
			गच्छ बहिर्गमनम्_err;
		}

		dx_अन्वेषणम्_प्रविष्टिः(frame, गणनम्, info->hash);
	}
	प्रत्यागम 0;
बहिर्गमनम्_err:
	dx_release(info);
	प्रत्यागम errcode;
}

संरचना link_struct  {
	ext2_filsys	fs;
	ध्रुव वर्ण	*name;
	पूर्णाङ्क		namelen;
	ext2_ino_t	inode;
	पूर्णाङ्क		flags;
	पूर्णाङ्क		समाप्तम्;
	अचिह्नित पूर्णाङ्क	blocksize;
	errcode_t	err;
	संरचना ext2_super_खण्डम् *sb;
};

स्थिर पूर्णाङ्क link_proc(ext2_ino_t dir EXT2FS_ATTR((अप्रयुक्तम्)),
		     पूर्णाङ्क entru EXT2FS_ATTR((अप्रयुक्तम्)),
		     संरचना ext2_dir_प्रविष्टिः *dirent,
		     पूर्णाङ्क	विस्थापनम्,
		     पूर्णाङ्क	blocksize,
		     वर्ण	*buf,
		     शून्य	*priv_दत्त)
{
	संरचना link_struct *ls = (संरचना link_struct *) priv_दत्त;
	संरचना ext2_dir_प्रविष्टिः *अग्र;
	अचिह्नित पूर्णाङ्क rec_len, min_rec_len, curr_rec_len;
	पूर्णाङ्क ret = 0;
	पूर्णाङ्क csum_आकार = 0;

	यदि (ls->समाप्तम्)
		प्रत्यागम DIRENT_ABORT;

	rec_len = EXT2_DIR_REC_LEN(ls->namelen);

	ls->err = ext2fs_get_rec_len(ls->fs, dirent, &curr_rec_len);
	यदि (ls->err)
		प्रत्यागम DIRENT_ABORT;

	यदि (ext2fs_has_feature_metadata_csum(ls->fs->super))
		csum_आकार = परिमाण(संरचना ext2_dir_प्रविष्टिः_पुच्छ);
	/*
	 * See if the following directory entry (if any) is unused;
	 * if so, absorb it into this one.
	 */
	अग्र = (संरचना ext2_dir_प्रविष्टिः *) (buf + विस्थापनम् + curr_rec_len);
	यदि ((विस्थापनम् + (पूर्णाङ्क) curr_rec_len < blocksize - (8 + csum_आकार)) &&
	    (अग्र->inode == 0) &&
	    (विस्थापनम् + (पूर्णाङ्क) curr_rec_len + (पूर्णाङ्क) अग्र->rec_len <= blocksize)) {
		curr_rec_len += अग्र->rec_len;
		ls->err = ext2fs_समुच्चयः_rec_len(ls->fs, curr_rec_len, dirent);
		यदि (ls->err)
			प्रत्यागम DIRENT_ABORT;
		ret = DIRENT_परिवर्तितम्;
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
		ls->err = ext2fs_समुच्चयः_rec_len(ls->fs, min_rec_len, dirent);
		यदि (ls->err)
			प्रत्यागम DIRENT_ABORT;
		अग्र = (संरचना ext2_dir_प्रविष्टिः *) (buf + विस्थापनम् +
						  dirent->rec_len);
		अग्र->inode = 0;
		ext2fs_dirent_समुच्चयः_name_len(अग्र, 0);
		ext2fs_dirent_समुच्चयः_file_type(अग्र, 0);
		ls->err = ext2fs_समुच्चयः_rec_len(ls->fs, rec_len, अग्र);
		यदि (ls->err)
			प्रत्यागम DIRENT_ABORT;
		प्रत्यागम DIRENT_परिवर्तितम्;
	}

	/*
	 * If we get this far, then the directory entry is not used.
	 * See if we can fit the request entry in.  If so, do it.
	 */
	यदि (curr_rec_len < rec_len)
		प्रत्यागम ret;
	dirent->inode = ls->inode;
	ext2fs_dirent_समुच्चयः_name_len(dirent, ls->namelen);
	strncpy(dirent->name, ls->name, ls->namelen);
	यदि (ext2fs_has_feature_filetype(ls->sb))
		ext2fs_dirent_समुच्चयः_file_type(dirent,
					    ls->flags & EXT2_FS_LINK_FT_आवरणम्);

	ls->समाप्तम्++;
	प्रत्यागम DIRENT_ABORT|DIRENT_परिवर्तितम्;
}

स्थिर errcode_t सङ्कलय_dirent_गन्तव्यम्_buf(ext2_filsys fs, e2_blkcnt_t blockcnt,
				   वर्ण *buf, ext2_ino_t dir,
				   संरचना ext2_inode *diri, ध्रुव वर्ण *name,
				   ext2_ino_t ino, पूर्णाङ्क flags, blk64_t *pblkp)
{
	संरचना dir_context ctx;
	संरचना link_struct ls;
	errcode_t retval;

	retval = आरोपय_logical_dir_खण्डम्(fs, dir, diri, blockcnt, pblkp, buf);
	यदि (retval)
		प्रत्यागम retval;
	ctx.errcode = 0;
	ctx.func = link_proc;
	ctx.dir = dir;
	ctx.flags = DIRENT_FLAG_INCLUDE_रिक्तानि;
	ctx.buf = buf;
	ctx.priv_दत्त = &ls;

	ls.fs = fs;
	ls.name = name;
	ls.namelen = वाक्यदैर्घ्यम्(name);
	ls.inode = ino;
	ls.flags = flags;
	ls.समाप्तम् = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	ext2fs_process_dir_खण्डम्(fs, pblkp, blockcnt, 0, 0, &ctx);
	यदि (ctx.errcode)
		प्रत्यागम ctx.errcode;
	यदि (ls.err)
		प्रत्यागम ls.err;
	यदि (!ls.समाप्तम्)
		प्रत्यागम EXT2_ET_DIR_NO_SPACE;
	प्रत्यागम 0;
}

संरचना dx_hash_map {
	__u32 hash;
	पूर्णाङ्क आकार;
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
				 पूर्णाङ्क गणनम्, शून्य *from, शून्य *गन्तव्यम्)
{
	संरचना ext2_dir_प्रविष्टिः *de;
	पूर्णाङ्क i;
	पूर्णाङ्क rec_len = 0;
	errcode_t retval;
	पूर्णाङ्क csum_आकार = 0;
	शून्य *आधारः = गन्तव्यम्;

	यदि (ext2fs_has_feature_metadata_csum(fs->super))
		csum_आकार = परिमाण(संरचना ext2_dir_प्रविष्टिः_पुच्छ);

	प्रति (i = 0; i < गणनम्; i++) {
		de = (संरचना ext2_dir_प्रविष्टिः *) ((वर्ण *)from + map[i].off);
		rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(de));
		स्मृतिं_प्रतिलिख(गन्तव्यम्, de, rec_len);
		retval = ext2fs_समुच्चयः_rec_len(fs, rec_len, गन्तव्यम्);
		यदि (retval)
			प्रत्यागम retval;
		गन्तव्यम् = (वर्ण *)गन्तव्यम् + rec_len;
	}
	/*
	 * Update rec_len of the last dir entry to stretch to the end of block
	 */
	गन्तव्यम् = (वर्ण *)गन्तव्यम् - rec_len;
	rec_len = fs->blocksize - ((वर्ण *)गन्तव्यम् - (वर्ण *)आधारः) - csum_आकार;
	retval = ext2fs_समुच्चयः_rec_len(fs, rec_len, गन्तव्यम्);
	यदि (retval)
		प्रत्यागम retval;
	यदि (csum_आकार)
		ext2fs_initialize_dirent_पुच्छ(fs,
				EXT2_DIRENT_पुच्छ(आधारः, fs->blocksize));
	प्रत्यागम 0;
}

स्थिर errcode_t dx_निवेशय_प्रविष्टिः(ext2_filsys fs, ext2_ino_t dir,
				 संरचना dx_lookup_info *info, पूर्णाङ्क level,
				 __u32 hash, blk64_t lblk)
{
	पूर्णाङ्क pcount;
	संरचना ext2_dx_प्रविष्टिः *शिखरम्, *new;

	pcount = ext2fs_le16_गन्तव्यम्_cpu(info->चौकटाः[level].शीर्ष->गणनम्);
	शिखरम् = info->चौकटाः[level].entries + pcount;
	new = info->चौकटाः[level].at + 1;
	memmove(new + 1, new, (वर्ण *)शिखरम् - (वर्ण *)new);
	new->hash = ext2fs_cpu_गन्तव्यम्_le32(hash);
	new->खण्डम् = ext2fs_cpu_गन्तव्यम्_le32(lblk);
	info->चौकटाः[level].शीर्ष->गणनम् = ext2fs_cpu_गन्तव्यम्_le16(pcount + 1);
	प्रत्यागम ext2fs_write_dir_block4(fs, info->चौकटाः[level].pblock,
				       info->चौकटाः[level].buf, 0, dir);
}

स्थिर errcode_t dx_विभज_पर्णम्(ext2_filsys fs, ext2_ino_t dir,
			       संरचना ext2_inode *diri,
			       संरचना dx_lookup_info *info, शून्य *buf,
			       blk64_t पर्णम्_pblk, blk64_t new_lblk,
			       blk64_t new_pblk)
{
	पूर्णाङ्क hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	संरचना ext2_dir_प्रविष्टिः *de;
	शून्य *buf2;
	errcode_t retval = 0;
	अचिह्नित पूर्णाङ्क rec_len;
	अचिह्नित पूर्णाङ्क विस्थापनम्, move_आकार;
	पूर्णाङ्क i, गणनम् = 0;
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
	प्रति (विस्थापनम् = 0; विस्थापनम् < fs->blocksize; विस्थापनम् += rec_len) {
		de = (संरचना ext2_dir_प्रविष्टिः *) ((वर्ण *)buf + विस्थापनम्);
		retval = ext2fs_get_rec_len(fs, de, &rec_len);
		यदि (retval)
			गच्छ बहिर्गमनम्;
		यदि (ext2fs_dirent_name_len(de) > 0 && de->inode) {
			map[गणनम्].off = विस्थापनम्;
			map[गणनम्].आकार = rec_len;
			retval = ext2fs_dirhash2(info->hash_alg, de->name,
					ext2fs_dirent_name_len(de),
					fs->encoding, hash_flags,
					fs->super->s_hash_बीजम्,
					&(map[गणनम्].hash),
					&minor_hash);
			यदि (retval)
				गच्छ बहिर्गमनम्;
			गणनम्++;
		}
	}
	शीघ्रं_क्रमय(map, गणनम्, परिमाण(संरचना dx_hash_map), dx_hash_map_cmp);
	move_आकार = 0;
	/* Find place to split block */
	प्रति (i = गणनम् - 1; i >= 0; i--) {
		यदि (move_आकार + map[i].आकार / 2 > fs->blocksize / 2)
			भङ्ग;
		move_आकार += map[i].आकार;
	}
	/* Let i be the first entry to move */
	i++;
	/* Move selected directory entries to new block */
	retval = dx_move_dirents(fs, map + i, गणनम् - i, buf, buf2);
	यदि (retval)
		गच्छ बहिर्गमनम्;
	retval = ext2fs_write_dir_block4(fs, new_pblk, buf2, 0, dir);
	यदि (retval)
		गच्छ बहिर्गमनम्;
	/* Repack remaining entries in the old block */
	retval = dx_move_dirents(fs, map, i, buf, buf2);
	यदि (retval)
		गच्छ बहिर्गमनम्;
	retval = ext2fs_write_dir_block4(fs, पर्णम्_pblk, buf2, 0, dir);
	यदि (retval)
		गच्छ बहिर्गमनम्;
	/* Update parent node */
	continued = map[i].hash == map[i-1].hash;
	retval = dx_निवेशय_प्रविष्टिः(fs, dir, info, info->स्तराः - 1,
				 map[i].hash + continued, new_lblk);
बहिर्गमनम्:
	ext2fs_free_mem(&buf2);
	ext2fs_free_mem(&map);
	प्रत्यागम retval;
}

स्थिर errcode_t dx_grow_वृक्षः(ext2_filsys fs, ext2_ino_t dir,
			      संरचना ext2_inode *diri,
			      संरचना dx_lookup_info *info, शून्य *buf,
			      blk64_t पर्णम्_pblk)
{
	पूर्णाङ्क i;
	errcode_t retval;
	ext2_off64_t आकार = EXT2_I_आकार(diri);
	blk64_t lblk, pblk;
	संरचना ext2_dir_प्रविष्टिः *de;
	संरचना ext2_dx_countlimit *शीर्ष;
	पूर्णाङ्क csum_आकार = 0;
	पूर्णाङ्क गणनम्;

	यदि (ext2fs_has_feature_metadata_csum(fs->super))
		csum_आकार = परिमाण(संरचना ext2_dx_पुच्छ);

	/* Find level which can accommodate new child */
	प्रति (i = info->स्तराः - 1; i >= 0; i--)
		यदि (ext2fs_le16_गन्तव्यम्_cpu(info->चौकटाः[i].शीर्ष->गणनम्) <
		    ext2fs_le16_गन्तव्यम्_cpu(info->चौकटाः[i].शीर्ष->सीमा))
			भङ्ग;
	/* Need to grow tree depth? */
	यदि (i < 0 && info->स्तराः >= ext2_dir_htree_level(fs))
		प्रत्यागम EXT2_ET_DIR_NO_SPACE;
	lblk = आकार / fs->blocksize;
	आकार += fs->blocksize;
	retval = ext2fs_inode_आकार_समुच्चयः(fs, diri, आकार);
	यदि (retval)
		प्रत्यागम retval;
	retval = ext2fs_fallocate(fs,
			EXT2_FALLOCATE_FORCE_INIT | EXT2_FALLOCATE_ZERO_खण्डानि,
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
	यदि (i == (पूर्णाङ्क)info->स्तराः - 1)
		प्रत्यागम dx_विभज_पर्णम्(fs, dir, diri, info, buf, पर्णम्_pblk,
				     lblk, pblk);

	de = buf;
	de->inode = 0;
	ext2fs_dirent_समुच्चयः_name_len(de, 0);
	ext2fs_dirent_समुच्चयः_file_type(de, 0);
	retval = ext2fs_समुच्चयः_rec_len(fs, fs->blocksize, de);
	यदि (retval)
		प्रत्यागम retval;
	शीर्ष = (संरचना ext2_dx_countlimit *) ((वर्ण *)buf + 8);
	गणनम् = ext2fs_le16_गन्तव्यम्_cpu(info->चौकटाः[i+1].शीर्ष->गणनम्);
	/* Growing tree depth? */
	यदि (i < 0) {
		संरचना ext2_dx_मूलम्_info *मूलम्;

		स्मृतिं_प्रतिलिख(शीर्ष, info->चौकटाः[0].entries,
		       गणनम् * परिमाण(संरचना ext2_dx_प्रविष्टिः));
		शीर्ष->सीमा = ext2fs_cpu_गन्तव्यम्_le16(
				(fs->blocksize - (8 + csum_आकार)) /
				परिमाण(संरचना ext2_dx_प्रविष्टिः));
		/* head->count gets set by memcpy above to correct value */

		/* Now update tree root */
		info->चौकटाः[0].शीर्ष->गणनम् = ext2fs_cpu_गन्तव्यम्_le16(1);
		info->चौकटाः[0].entries[0].खण्डम् = ext2fs_cpu_गन्तव्यम्_le32(lblk);
		मूलम् = (संरचना ext2_dx_मूलम्_info *)
			((वर्ण *)info->चौकटाः[0].buf + EXT2_DX_मूलम्_OFF);
		मूलम्->परोक्षम्_स्तराः++;
	} अन्यथा {
		/* Splitting internal node in two */
		पूर्णाङ्क count1 = गणनम् / 2;
		पूर्णाङ्क count2 = गणनम् - count1;
		__u32 विभज_hash = ext2fs_le32_गन्तव्यम्_cpu(info->चौकटाः[i+1].entries[count1].hash);

		स्मृतिं_प्रतिलिख(शीर्ष, info->चौकटाः[i+1].entries + count1,
		       count2 * परिमाण(संरचना ext2_dx_प्रविष्टिः));
		शीर्ष->गणनम् = ext2fs_cpu_गन्तव्यम्_le16(count2);
		शीर्ष->सीमा = ext2fs_cpu_गन्तव्यम्_le16(
				(fs->blocksize - (8 + csum_आकार)) /
				परिमाण(संरचना ext2_dx_प्रविष्टिः));
		info->चौकटाः[i+1].शीर्ष->गणनम् = ext2fs_cpu_गन्तव्यम्_le16(count1);

		/* Update parent node */
		retval = dx_निवेशय_प्रविष्टिः(fs, dir, info, i, विभज_hash, lblk);
		यदि (retval)
			प्रत्यागम retval;

	}
	/* Writeout split block / updated root */
	retval = ext2fs_write_dir_block4(fs, info->चौकटाः[i+1].pblock,
					 info->चौकटाः[i+1].buf, 0, dir);
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
	अचिह्नित restart = 0;
	blk64_t पर्णम्_pblk;

	retval = ext2fs_get_mem(fs->blocksize, &blockbuf);
	यदि (retval)
		प्रत्यागम retval;

	dx_info.name = name;
	dx_info.namelen = वाक्यदैर्घ्यम्(name);
again:
	retval = dx_lookup(fs, dir, diri, &dx_info);
	यदि (retval)
		गच्छ free_buf;

	retval = सङ्कलय_dirent_गन्तव्यम्_buf(fs,
		ext2fs_le32_गन्तव्यम्_cpu(dx_info.चौकटाः[dx_info.स्तराः-1].at->खण्डम्) & 0x0fffffff,
		blockbuf, dir, diri, name, ino, flags, &पर्णम्_pblk);
	/*
	 * Success or error other than ENOSPC...? We are done. We may need upto
	 * two tries to add entry. One to split htree node and another to add
	 * new leaf block.
	 */
	यदि (restart >= dx_info.स्तराः || retval != EXT2_ET_DIR_NO_SPACE)
		गच्छ free_चौकटाः;
	retval = dx_grow_वृक्षः(fs, dir, diri, &dx_info, blockbuf, पर्णम्_pblk);
	यदि (retval)
		गच्छ free_चौकटाः;
	/* Restart everything now that the tree is larger */
	restart++;
	dx_release(&dx_info);
	गच्छ again;
free_चौकटाः:
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

संरचना dx_मूलम्
{
	संरचना fake_dirent dot;
	वर्ण dot_name[4];
	संरचना fake_dirent dotdot;
	वर्ण dotdot_name[4];
	संरचना ext2_dx_मूलम्_info info;
	संरचना ext2_dx_प्रविष्टिः entries[];
};

स्थिर पूर्णाङ्क check_dx_मूलम्(ext2_filsys fs, ext2_ino_t ino, संरचना dx_मूलम् *मूलम्)
{
	संरचना fake_dirent *fde;
	वर्ण *error_सन्देशः_2;
	अचिह्नित पूर्णाङ्क rlen;
	वर्ण *blockend = (वर्ण *)मूलम् + fs->blocksize;

	fde = &मूलम्->dot;
	यदि (fde->name_len != 1) {
		error_सन्देशः_2 = "invalid name_len for '.'";
		गच्छ corrupted;
	}
	यदि (वाक्ये_तुलय(मूलम्->dot_name, ".", fde->name_len)) {
		error_सन्देशः_2 = "invalid name for '.'";
		गच्छ corrupted;
	}

	ext2fs_get_rec_len(fs, (संरचना ext2_dir_प्रविष्टिः *)fde, &rlen);
	यदि ((वर्ण *)fde + rlen >= blockend) {
		error_सन्देशः_2 = "invalid rec_len for '.'";
		गच्छ corrupted;
	}

	fde = &मूलम्->dotdot;
	यदि (fde->name_len != 2) {
		error_सन्देशः_2 = "invalid name_len for '..'";
		गच्छ corrupted;
	}
	यदि (वाक्ये_तुलय(मूलम्->dotdot_name, "..", fde->name_len)) {
		error_सन्देशः_2 = "invalid name for '..'";
		गच्छ corrupted;
	}
	ext2fs_get_rec_len(fs, (संरचना ext2_dir_प्रविष्टिः *)fde, &rlen);
	यदि ((वर्ण *)fde + rlen >= blockend) {
		error_सन्देशः_2 = "invalid rec_len for '..'";
		गच्छ corrupted;
	}

	प्रत्यागम 1;

corrupted:
	fprintf(stderr, "Corrupt dir %u: %s, running e2fsck is recommended\n",
			 ino, error_सन्देशः_2);
	प्रत्यागम 0;
}

स्थिर अन्तःस्थ संरचना ext2_dir_प्रविष्टिः *
ext2fs_अग्र_प्रविष्टिः(ext2_filsys fs, संरचना ext2_dir_प्रविष्टिः *p)
{
	अचिह्नित पूर्णाङ्क rlen;

	ext2fs_get_rec_len(fs, p, &rlen);
	प्रत्यागम (संरचना ext2_dir_प्रविष्टिः *)((वर्ण *)p + rlen);
}

स्थिर अन्तःस्थ शून्य dx_समुच्चयः_खण्डम्(संरचना ext2_dx_प्रविष्टिः *प्रविष्टिः, अचिह्नित मानम्)
{
	प्रविष्टिः->खण्डम् = ext2fs_cpu_गन्तव्यम्_le32(मानम्);
}

स्थिर अन्तःस्थ शून्य dx_समुच्चयः_गणनम्(संरचना ext2_dx_प्रविष्टिः *entries, अचिह्नित मानम्)
{
	((संरचना ext2_dx_countlimit *) entries)->गणनम् = ext2fs_cpu_गन्तव्यम्_le16(मानम्);
}

स्थिर अन्तःस्थ शून्य dx_समुच्चयः_सीमा(संरचना ext2_dx_प्रविष्टिः *entries, अचिह्नित मानम्)
{
	((संरचना ext2_dx_countlimit *) entries)->सीमा = ext2fs_cpu_गन्तव्यम्_le16(मानम्);
}

स्थिर अन्तःस्थ अचिह्नित dx_मूलम्_सीमा(ext2_filsys fs, अचिह्नित infosize)
{
	अचिह्नित पूर्णाङ्क प्रविष्टिः_space = fs->blocksize -
			ext2fs_dir_rec_len(1, 0) -
			ext2fs_dir_rec_len(2, 0) - infosize;

	यदि (ext2fs_has_feature_metadata_csum(fs->super))
		प्रविष्टिः_space -= परिमाण(संरचना ext2_dx_पुच्छ);
	प्रत्यागम प्रविष्टिः_space / परिमाण(संरचना ext2_dx_प्रविष्टिः);
}

/*
 * This converts a one block unindexed directory to a 3 block indexed
 * directory, and adds the dentry to the indexed directory.  Returns 0 if the
 * index was not created; EAGAIN if it was; or an errcode_t on error.
 */
स्थिर errcode_t try_make_अनुक्रमितम्_dir(ext2_filsys fs, ext2_ino_t dir,
				      ध्रुव वर्ण *name, ext2_ino_t ino,
				      पूर्णाङ्क flags)
{
	संरचना ext2_inode inode;
	वर्ण *buf0, *buf1, *शिखरम्;
	संरचना dx_मूलम् *मूलम्;
	संरचना ext2_dx_प्रविष्टिः *entries;
	संरचना fake_dirent *fde;
	संरचना ext2_dir_प्रविष्टिः *de, *de2;
	blk64_t pblk0, pblk1;
	अचिह्नित पूर्णाङ्क len;
	ध्रुव अचिह्नित पूर्णाङ्क blocksize = fs->blocksize;
	पूर्णाङ्क csum_आकार = 0;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	यदि (retval)
		प्रत्यागम retval;

	यदि (inode.i_आकार > fs->blocksize || (inode.i_flags & EXT2_सूचकांक_FL))
		प्रत्यागम 0;

	यदि (ext2fs_has_feature_metadata_csum(fs->super))
		csum_आकार = परिमाण(संरचना ext2_dir_प्रविष्टिः_पुच्छ);

	retval = ext2fs_get_mem(blocksize, &buf0);
	यदि (retval)
		प्रत्यागम retval;

	retval = ext2fs_get_mem(blocksize, &buf1);
	यदि (retval)
		गच्छ बहिर्गमनम्_buf0;

	retval = ext2fs_bmap2(fs, dir, &inode, रिक्त, 0, 0, रिक्त, &pblk0);
	यदि (retval)
		गच्छ बहिर्गमनम्_buf1;

	retval = ext2fs_read_dir_block4(fs, pblk0, buf0, 0, dir);
	यदि (retval)
		गच्छ बहिर्गमनम्_buf1;

	मूलम् = (संरचना dx_मूलम् *)buf0;
	यदि (!check_dx_मूलम्(fs, dir, मूलम्)) {
		retval = EXT2_ET_DIR_CORRUPTED;
		गच्छ बहिर्गमनम्_buf1;
	}

	/* The 0th block becomes the root, move the dirents out */
	fde = &मूलम्->dotdot;
	de = ext2fs_अग्र_प्रविष्टिः(fs, (संरचना ext2_dir_प्रविष्टिः *)fde);
	len = ((वर्ण *) मूलम्) + (blocksize - csum_आकार) - (वर्ण *) de;

	/* Allocate new block for the 0th block's dirents */
	retval = ext2fs_bmap2(fs, dir, &inode, रिक्त, BMAP_ALLOC | BMAP_ZERO, 1,
			      रिक्त, &pblk1);
	यदि (retval)
		गच्छ बहिर्गमनम्_buf1;

	स्मृतिं_प्रतिलिख(buf1, de, len);
	स्मृतिं_पूरय(de, 0, len); /* wipe old data */

	de = (संरचना ext2_dir_प्रविष्टिः *)buf1;
	शिखरम् = buf1 + len;
	यावत् ((वर्ण *)(de2 = ext2fs_अग्र_प्रविष्टिः(fs, de)) < शिखरम्) {
#यदि 0
		यदि (ext4_check_dir_प्रविष्टिः(dir, रिक्त, de, bh2, buf1, len,
					(वर्ण *)de - buf1)) {
			retval = EXT2_ET_DIR_CORRUPTED;
			गच्छ बहिर्गमनम्_buf1;
		}
#यदि_अन्तः
		de = de2;
	}
	retval = ext2fs_समुच्चयः_rec_len(fs,
			buf1 + (blocksize - csum_आकार) - (वर्ण *) de, de);
	यदि (retval)
		गच्छ बहिर्गमनम्_buf1;

	यदि (csum_आकार)
		ext2fs_initialize_dirent_पुच्छ(fs,
				EXT2_DIRENT_पुच्छ(buf1, fs->blocksize));

	/* Initialize the root; the dot dirents already exist */
	de = (संरचना ext2_dir_प्रविष्टिः *) (&मूलम्->dotdot);
	retval = ext2fs_समुच्चयः_rec_len(fs, blocksize - ext2fs_dir_rec_len(2, 0),
				    de);
	स्मृतिं_पूरय (&मूलम्->info, 0, परिमाण(मूलम्->info));
	मूलम्->info.info_दीर्घता = परिमाण(मूलम्->info);
	यदि (ext4_hash_अन्तर्_dirent(&inode))
		मूलम्->info.hash_संस्करणम् = EXT2_HASH_SIPHASH;
	अन्यथा
		मूलम्->info.hash_संस्करणम् = fs->super->s_def_hash_संस्करणम्;

	entries = मूलम्->entries;
	dx_समुच्चयः_खण्डम्(entries, 1);
	dx_समुच्चयः_गणनम्(entries, 1);
	dx_समुच्चयः_सीमा(entries, dx_मूलम्_सीमा(fs, परिमाण(मूलम्->info)));

	retval = ext2fs_write_dir_block4(fs, pblk1, buf1, 0, dir);
	यदि (retval)
		गच्छ बहिर्गमनम्_buf1;

	retval = ext2fs_write_dir_block4(fs, pblk0, buf0, 0, dir);
	यदि (retval)
		गच्छ बहिर्गमनम्_buf1;

	inode.i_flags |= EXT2_सूचकांक_FL;
	inode.i_आकार += fs->blocksize;
	retval = ext2fs_write_inode(fs, dir, &inode);
	यदि (retval)
		गच्छ बहिर्गमनम्_buf1;

	retval = EAGAIN;
बहिर्गमनम्_buf1:
	ext2fs_free_mem(&buf1);
बहिर्गमनम्_buf0:
	ext2fs_free_mem(&buf0);
	प्रत्यागम retval;
}

/*
 * Note: the low 3 bits of the flags field are used as the directory
 * entry filetype.
 */
#यदि_परिभाषितम् __TURBOC__
 #निर्देशः argsused
#यदि_अन्तः
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

	यदि (inode.i_flags & EXT2_सूचकांक_FL)
		प्रत्यागम dx_link(fs, dir, &inode, name, ino, flags);

	ls.fs = fs;
	ls.name = name;
	ls.namelen = name ? वाक्यदैर्घ्यम्(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.समाप्तम् = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	यदि ((flags & EXT2FS_LINK_APPEND) &&
	    !(inode.i_flags & EXT4_INLINE_दत्त_FL)) {
		blk64_t		lblk, pblk = 0;
		संरचना dir_context ctx;

		lblk = (inode.i_आकार / fs->blocksize) - 1;
		retval = ext2fs_bmap2(fs, dir, &inode, रिक्त, 0, lblk,
				      रिक्त, &pblk);
		यदि (retval)
			प्रत्यागम retval;

		ctx.dir = dir;
		ctx.flags = DIRENT_FLAG_INCLUDE_रिक्तानि;
		ctx.func = link_proc;
		ctx.priv_दत्त = &ls;
		ctx.errcode = 0;
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		यदि (retval)
			प्रत्यागम retval;

		ext2fs_process_dir_खण्डम्(fs, &pblk, lblk, 0, 0, &ctx);
		retval = ctx.errcode;
		ext2fs_free_mem(&ctx.buf);
	} अन्यथा {
		retval = ext2fs_dir_iterate2(fs, dir,
					     DIRENT_FLAG_INCLUDE_रिक्तानि,
					     रिक्त, link_proc, &ls);
	}
	यदि (retval)
		प्रत्यागम retval;
	यदि (ls.err)
		प्रत्यागम ls.err;

	यदि (!ls.समाप्तम्) {
		यदि (!(flags & EXT2FS_LINK_EXPAND))
			प्रत्यागम EXT2_ET_DIR_NO_SPACE;

		यदि (ext2fs_has_feature_dir_सूचकांक(fs->super)) {
			retval = try_make_अनुक्रमितम्_dir(fs, dir, name, ino,
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
	प्रत्यागम ext2fs_has_feature_dir_सूचकांक(fs->super) &&
		S_ISDIR(inode->i_बहुलकम्) && (inode->i_flags & EXT2_सूचकांक_FL);
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
	inode->i_links_गणनम्++;

	यदि (ext2fs_dir_is_dx(fs, inode) &&
	    (inode->i_links_गणनम् > EXT2_LINK_MAX || inode->i_links_गणनम् == 2))
		inode->i_links_गणनम् = 1;
}

/*
 * If a directory had nlink == 1, then we should let it be 1. This indicates
 * directory has >EXT2_LINK_MAX subdirs.
 */
शून्य ext2fs_dec_nlink(संरचना ext2_inode *inode)
{
	यदि (!S_ISDIR(inode->i_बहुलकम्) || inode->i_links_गणनम् > 2)
		inode->i_links_गणनम्--;
}

पूर्णाङ्क ext2fs_dir_link_max(ext2_filsys fs, संरचना ext2_inode_large *inode)
{
	प्रत्यागम inode->i_links_गणनम् >= EXT2_LINK_MAX &&
	       !(ext2fs_dir_is_dx(fs, EXT2_INODE(inode)) &&
		 ext2fs_has_feature_dir_nlink(fs->super));
}

पूर्णाङ्क ext2fs_dir_link_रिक्तानि(संरचना ext2_inode *inode)
{
	प्रत्यागम inode->i_links_गणनम् == 2 || inode->i_links_गणनम् == 1;
}
