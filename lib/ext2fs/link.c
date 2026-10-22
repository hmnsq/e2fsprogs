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

#include "config.h"
#include <stdio.h>
#include <string.h>
#если HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

#define EXT2_DX_ROOT_OFF 24

struct dx_frame {
	void *buf;
	blk64_t pblock;
	struct ext2_dx_countlimit *head;
	struct ext2_dx_entry *entries;
	struct ext2_dx_entry *at;
};

struct dx_lookup_info {
	const char *name;
	целое namelen;
	целое hash_alg;
	__u32 hash;
	unsigned levels;
	struct dx_frame frames[EXT4_HTREE_LEVEL];
};

static errcode_t alloc_dx_frame(ext2_filsys fs, struct dx_frame *frame)
{
	вернуть ext2fs_get_mem(fs->blocksize, &frame->buf);
}

static void dx_release(struct dx_lookup_info *info)
{
	unsigned level;

	для (level = 0; level < info->levels; level++) {
		если (info->frames[level].buf == NULL)
			break;
		ext2fs_free_mem(&(info->frames[level].buf));
	}
	info->levels = 0;
}

static void dx_search_entry(struct dx_frame *frame, целое count, __u32 hash)
{
	struct ext2_dx_entry *p, *q, *m;

	p = frame->entries + 1;
	q = frame->entries + count - 1;
	пока (p <= q) {
		m = p + (q - p) / 2;
		если (ext2fs_le32_to_cpu(m->hash) > hash)
			q = m - 1;
		иначе
			p = m + 1;
	}
	frame->at = p - 1;
}

static errcode_t load_logical_dir_block(ext2_filsys fs, ext2_ino_t dir,
					struct ext2_inode *diri, blk64_t block,
					blk64_t *pblk, void *buf)
{
	errcode_t errcode;
	целое ret_flags;

	errcode = ext2fs_bmap2(fs, dir, diri, NULL, 0, block, &ret_flags,
			       pblk);
	если (errcode)
		вернуть errcode;
	если (ret_flags & BMAP_RET_UNINIT)
		вернуть EXT2_ET_DIR_CORRUPTED;
	вернуть ext2fs_read_dir_block4(fs, *pblk, buf, 0, dir);
}

static errcode_t dx_lookup(ext2_filsys fs, ext2_ino_t dir,
			   struct ext2_inode *diri, struct dx_lookup_info *info)
{
	struct ext2_dx_root_info *root;
	errcode_t errcode;
	целое level = 0;
	целое count, limit;
	целое hash_alg;
	целое hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	__u32 minor_hash;
	struct dx_frame *frame;

	errcode = alloc_dx_frame(fs, &(info->frames[0]));
	если (errcode)
		вернуть errcode;
	info->levels = 1;

	errcode = load_logical_dir_block(fs, dir, diri, 0,
					 &(info->frames[0].pblock),
					 info->frames[0].buf);
	если (errcode)
		goto out_err;
	root = (struct ext2_dx_root_info *) ((char *)info->frames[0].buf +
					     EXT2_DX_ROOT_OFF);
	hash_alg = root->hash_version;
	если (hash_alg != EXT2_HASH_TEA && hash_alg != EXT2_HASH_HALF_MD4 &&
	    hash_alg != EXT2_HASH_LEGACY) {
		errcode = EXT2_ET_DIRHASH_UNSUPP;
		goto out_err;
	}
	если (hash_alg <= EXT2_HASH_TEA &&
	    fs->super->s_flags & EXT2_FLAGS_UNSIGNED_HASH)
		hash_alg += 3;
	если (root->indirect_levels >= ext2_dir_htree_level(fs)) {
		errcode = EXT2_ET_DIR_CORRUPTED;
		goto out_err;
	}
	info->hash_alg = hash_alg;

	errcode = ext2fs_dirhash2(hash_alg, info->name, info->namelen,
				  fs->encoding, hash_flags,
				  fs->super->s_hash_seed, &info->hash,
				  &minor_hash);
	если (errcode)
		goto out_err;

	для (level = 0; level <= root->indirect_levels; level++) {
		frame = &(info->frames[level]);
		если (level > 0) {
			errcode = alloc_dx_frame(fs, frame);
			если (errcode)
				goto out_err;
			info->levels++;

			errcode = load_logical_dir_block(fs, dir, diri,
				ext2fs_le32_to_cpu(info->frames[level-1].at->block) & 0x0fffffff,
				&(frame->pblock), frame->buf);
			если (errcode)
				goto out_err;
		}
		errcode = ext2fs_get_dx_countlimit(fs, frame->buf,
						   &(frame->head), NULL);
		если (errcode)
			goto out_err;
		count = ext2fs_le16_to_cpu(frame->head->count);
		limit = ext2fs_le16_to_cpu(frame->head->limit);
		frame->entries = (struct ext2_dx_entry *)(frame->head);
		если (!count || count > limit) {
			errcode = EXT2_ET_DIR_CORRUPTED;
			goto out_err;
		}

		dx_search_entry(frame, count, info->hash);
	}
	вернуть 0;
out_err:
	dx_release(info);
	вернуть errcode;
}

struct link_struct  {
	ext2_filsys	fs;
	const char	*name;
	целое		namelen;
	ext2_ino_t	inode;
	целое		flags;
	целое		done;
	unsigned целое	blocksize;
	errcode_t	err;
	struct ext2_super_block *sb;
};

static целое link_proc(ext2_ino_t dir EXT2FS_ATTR((unused)),
		     целое entru EXT2FS_ATTR((unused)),
		     struct ext2_dir_entry *dirent,
		     целое	offset,
		     целое	blocksize,
		     char	*buf,
		     void	*priv_data)
{
	struct link_struct *ls = (struct link_struct *) priv_data;
	struct ext2_dir_entry *next;
	unsigned целое rec_len, min_rec_len, curr_rec_len;
	целое ret = 0;
	целое csum_size = 0;

	если (ls->done)
		вернуть DIRENT_ABORT;

	rec_len = EXT2_DIR_REC_LEN(ls->namelen);

	ls->err = ext2fs_get_rec_len(ls->fs, dirent, &curr_rec_len);
	если (ls->err)
		вернуть DIRENT_ABORT;

	если (ext2fs_has_feature_metadata_csum(ls->fs->super))
		csum_size = sizeof(struct ext2_dir_entry_tail);
	/*
	 * See if the following directory entry (if any) is unused;
	 * if so, absorb it into this one.
	 */
	next = (struct ext2_dir_entry *) (buf + offset + curr_rec_len);
	если ((offset + (целое) curr_rec_len < blocksize - (8 + csum_size)) &&
	    (next->inode == 0) &&
	    (offset + (целое) curr_rec_len + (целое) next->rec_len <= blocksize)) {
		curr_rec_len += next->rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, curr_rec_len, dirent);
		если (ls->err)
			вернуть DIRENT_ABORT;
		ret = DIRENT_CHANGED;
	}

	/*
	 * If the directory entry is used, see if we can split the
	 * directory entry to make room for the new name.  If so,
	 * truncate it and return.
	 */
	если (dirent->inode) {
		min_rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(dirent));
		если (curr_rec_len < (min_rec_len + rec_len))
			вернуть ret;
		rec_len = curr_rec_len - min_rec_len;
		ls->err = ext2fs_set_rec_len(ls->fs, min_rec_len, dirent);
		если (ls->err)
			вернуть DIRENT_ABORT;
		next = (struct ext2_dir_entry *) (buf + offset +
						  dirent->rec_len);
		next->inode = 0;
		ext2fs_dirent_set_name_len(next, 0);
		ext2fs_dirent_set_file_type(next, 0);
		ls->err = ext2fs_set_rec_len(ls->fs, rec_len, next);
		если (ls->err)
			вернуть DIRENT_ABORT;
		вернуть DIRENT_CHANGED;
	}

	/*
	 * If we get this far, then the directory entry is not used.
	 * See if we can fit the request entry in.  If so, do it.
	 */
	если (curr_rec_len < rec_len)
		вернуть ret;
	dirent->inode = ls->inode;
	ext2fs_dirent_set_name_len(dirent, ls->namelen);
	strncpy(dirent->name, ls->name, ls->namelen);
	если (ext2fs_has_feature_filetype(ls->sb))
		ext2fs_dirent_set_file_type(dirent,
					    ls->flags & EXT2FS_LINK_FT_MASK);

	ls->done++;
	вернуть DIRENT_ABORT|DIRENT_CHANGED;
}

static errcode_t add_dirent_to_buf(ext2_filsys fs, e2_blkcnt_t blockcnt,
				   char *buf, ext2_ino_t dir,
				   struct ext2_inode *diri, const char *name,
				   ext2_ino_t ino, целое flags, blk64_t *pblkp)
{
	struct dir_context ctx;
	struct link_struct ls;
	errcode_t retval;

	retval = load_logical_dir_block(fs, dir, diri, blockcnt, pblkp, buf);
	если (retval)
		вернуть retval;
	ctx.errcode = 0;
	ctx.func = link_proc;
	ctx.dir = dir;
	ctx.flags = DIRENT_FLAG_INCLUDE_EMPTY;
	ctx.buf = buf;
	ctx.priv_data = &ls;

	ls.fs = fs;
	ls.name = name;
	ls.namelen = strlen(name);
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	ext2fs_process_dir_block(fs, pblkp, blockcnt, 0, 0, &ctx);
	если (ctx.errcode)
		вернуть ctx.errcode;
	если (ls.err)
		вернуть ls.err;
	если (!ls.done)
		вернуть EXT2_ET_DIR_NO_SPACE;
	вернуть 0;
}

struct dx_hash_map {
	__u32 hash;
	целое size;
	целое off;
};

static EXT2_QSORT_TYPE dx_hash_map_cmp(const void *ap, const void *bp)
{
	const struct dx_hash_map *a = ap, *b = bp;

	если (a->hash < b->hash)
		вернуть -1;
	если (a->hash > b->hash)
		вернуть 1;
	вернуть 0;
}

static errcode_t dx_move_dirents(ext2_filsys fs, struct dx_hash_map *map,
				 целое count, void *from, void *to)
{
	struct ext2_dir_entry *de;
	целое i;
	целое rec_len = 0;
	errcode_t retval;
	целое csum_size = 0;
	void *base = to;

	если (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = sizeof(struct ext2_dir_entry_tail);

	для (i = 0; i < count; i++) {
		de = (struct ext2_dir_entry *) ((char *)from + map[i].off);
		rec_len = EXT2_DIR_REC_LEN(ext2fs_dirent_name_len(de));
		memcpy(to, de, rec_len);
		retval = ext2fs_set_rec_len(fs, rec_len, to);
		если (retval)
			вернуть retval;
		to = (char *)to + rec_len;
	}
	/*
	 * Update rec_len of the last dir entry to stretch to the end of block
	 */
	to = (char *)to - rec_len;
	rec_len = fs->blocksize - ((char *)to - (char *)base) - csum_size;
	retval = ext2fs_set_rec_len(fs, rec_len, to);
	если (retval)
		вернуть retval;
	если (csum_size)
		ext2fs_initialize_dirent_tail(fs,
				EXT2_DIRENT_TAIL(base, fs->blocksize));
	вернуть 0;
}

static errcode_t dx_insert_entry(ext2_filsys fs, ext2_ino_t dir,
				 struct dx_lookup_info *info, целое level,
				 __u32 hash, blk64_t lblk)
{
	целое pcount;
	struct ext2_dx_entry *top, *new;

	pcount = ext2fs_le16_to_cpu(info->frames[level].head->count);
	top = info->frames[level].entries + pcount;
	new = info->frames[level].at + 1;
	memmove(new + 1, new, (char *)top - (char *)new);
	new->hash = ext2fs_cpu_to_le32(hash);
	new->block = ext2fs_cpu_to_le32(lblk);
	info->frames[level].head->count = ext2fs_cpu_to_le16(pcount + 1);
	вернуть ext2fs_write_dir_block4(fs, info->frames[level].pblock,
				       info->frames[level].buf, 0, dir);
}

static errcode_t dx_split_leaf(ext2_filsys fs, ext2_ino_t dir,
			       struct ext2_inode *diri,
			       struct dx_lookup_info *info, void *buf,
			       blk64_t leaf_pblk, blk64_t new_lblk,
			       blk64_t new_pblk)
{
	целое hash_flags = diri->i_flags & EXT4_CASEFOLD_FL;
	struct ext2_dir_entry *de;
	void *buf2;
	errcode_t retval = 0;
	unsigned целое rec_len;
	unsigned целое offset, move_size;
	целое i, count = 0;
	struct dx_hash_map *map;
	целое continued;
	__u32 minor_hash;

	retval = ext2fs_get_mem(fs->blocksize, &buf2);
	если (retval)
		вернуть retval;
	retval = ext2fs_get_array(fs->blocksize / 12,
				  sizeof(struct dx_hash_map), &map);
	если (retval) {
		ext2fs_free_mem(&buf2);
		вернуть retval;
	}
	для (offset = 0; offset < fs->blocksize; offset += rec_len) {
		de = (struct ext2_dir_entry *) ((char *)buf + offset);
		retval = ext2fs_get_rec_len(fs, de, &rec_len);
		если (retval)
			goto out;
		если (ext2fs_dirent_name_len(de) > 0 && de->inode) {
			map[count].off = offset;
			map[count].size = rec_len;
			retval = ext2fs_dirhash2(info->hash_alg, de->name,
					ext2fs_dirent_name_len(de),
					fs->encoding, hash_flags,
					fs->super->s_hash_seed,
					&(map[count].hash),
					&minor_hash);
			если (retval)
				goto out;
			count++;
		}
	}
	qsort(map, count, sizeof(struct dx_hash_map), dx_hash_map_cmp);
	move_size = 0;
	/* Find place to split block */
	для (i = count - 1; i >= 0; i--) {
		если (move_size + map[i].size / 2 > fs->blocksize / 2)
			break;
		move_size += map[i].size;
	}
	/* Let i be the first entry to move */
	i++;
	/* Move selected directory entries to new block */
	retval = dx_move_dirents(fs, map + i, count - i, buf, buf2);
	если (retval)
		goto out;
	retval = ext2fs_write_dir_block4(fs, new_pblk, buf2, 0, dir);
	если (retval)
		goto out;
	/* Repack remaining entries in the old block */
	retval = dx_move_dirents(fs, map, i, buf, buf2);
	если (retval)
		goto out;
	retval = ext2fs_write_dir_block4(fs, leaf_pblk, buf2, 0, dir);
	если (retval)
		goto out;
	/* Update parent node */
	continued = map[i].hash == map[i-1].hash;
	retval = dx_insert_entry(fs, dir, info, info->levels - 1,
				 map[i].hash + continued, new_lblk);
out:
	ext2fs_free_mem(&buf2);
	ext2fs_free_mem(&map);
	вернуть retval;
}

static errcode_t dx_grow_tree(ext2_filsys fs, ext2_ino_t dir,
			      struct ext2_inode *diri,
			      struct dx_lookup_info *info, void *buf,
			      blk64_t leaf_pblk)
{
	целое i;
	errcode_t retval;
	ext2_off64_t size = EXT2_I_SIZE(diri);
	blk64_t lblk, pblk;
	struct ext2_dir_entry *de;
	struct ext2_dx_countlimit *head;
	целое csum_size = 0;
	целое count;

	если (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = sizeof(struct ext2_dx_tail);

	/* Find level which can accommodate new child */
	для (i = info->levels - 1; i >= 0; i--)
		если (ext2fs_le16_to_cpu(info->frames[i].head->count) <
		    ext2fs_le16_to_cpu(info->frames[i].head->limit))
			break;
	/* Need to grow tree depth? */
	если (i < 0 && info->levels >= ext2_dir_htree_level(fs))
		вернуть EXT2_ET_DIR_NO_SPACE;
	lblk = size / fs->blocksize;
	size += fs->blocksize;
	retval = ext2fs_inode_size_set(fs, diri, size);
	если (retval)
		вернуть retval;
	retval = ext2fs_fallocate(fs,
			EXT2_FALLOCATE_FORCE_INIT | EXT2_FALLOCATE_ZERO_BLOCKS,
			dir, diri, 0, lblk, 1);
	если (retval)
		вернуть retval;
	retval = ext2fs_write_inode(fs, dir, diri);
	если (retval)
		вернуть retval;
	retval = ext2fs_bmap2(fs, dir, diri, NULL, 0, lblk, NULL, &pblk);
	если (retval)
		вернуть retval;
	/* Only leaf addition needed? */
	если (i == (целое)info->levels - 1)
		вернуть dx_split_leaf(fs, dir, diri, info, buf, leaf_pblk,
				     lblk, pblk);

	de = buf;
	de->inode = 0;
	ext2fs_dirent_set_name_len(de, 0);
	ext2fs_dirent_set_file_type(de, 0);
	retval = ext2fs_set_rec_len(fs, fs->blocksize, de);
	если (retval)
		вернуть retval;
	head = (struct ext2_dx_countlimit *) ((char *)buf + 8);
	count = ext2fs_le16_to_cpu(info->frames[i+1].head->count);
	/* Growing tree depth? */
	если (i < 0) {
		struct ext2_dx_root_info *root;

		memcpy(head, info->frames[0].entries,
		       count * sizeof(struct ext2_dx_entry));
		head->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_size)) /
				sizeof(struct ext2_dx_entry));
		/* head->count gets set by memcpy above to correct value */

		/* Now update tree root */
		info->frames[0].head->count = ext2fs_cpu_to_le16(1);
		info->frames[0].entries[0].block = ext2fs_cpu_to_le32(lblk);
		root = (struct ext2_dx_root_info *)
			((char *)info->frames[0].buf + EXT2_DX_ROOT_OFF);
		root->indirect_levels++;
	} иначе {
		/* Splitting internal node in two */
		целое count1 = count / 2;
		целое count2 = count - count1;
		__u32 split_hash = ext2fs_le32_to_cpu(info->frames[i+1].entries[count1].hash);

		memcpy(head, info->frames[i+1].entries + count1,
		       count2 * sizeof(struct ext2_dx_entry));
		head->count = ext2fs_cpu_to_le16(count2);
		head->limit = ext2fs_cpu_to_le16(
				(fs->blocksize - (8 + csum_size)) /
				sizeof(struct ext2_dx_entry));
		info->frames[i+1].head->count = ext2fs_cpu_to_le16(count1);

		/* Update parent node */
		retval = dx_insert_entry(fs, dir, info, i, split_hash, lblk);
		если (retval)
			вернуть retval;

	}
	/* Writeout split block / updated root */
	retval = ext2fs_write_dir_block4(fs, info->frames[i+1].pblock,
					 info->frames[i+1].buf, 0, dir);
	если (retval)
		вернуть retval;
	/* Writeout new tree block */
	retval = ext2fs_write_dir_block4(fs, pblk, buf, 0, dir);
	если (retval)
		вернуть retval;
	вернуть 0;
}

static errcode_t dx_link(ext2_filsys fs, ext2_ino_t dir,
			 struct ext2_inode *diri, const char *name,
			 ext2_ino_t ino, целое flags)
{
	struct dx_lookup_info dx_info;
	errcode_t retval;
	void *blockbuf;
	unsigned restart = 0;
	blk64_t leaf_pblk;

	retval = ext2fs_get_mem(fs->blocksize, &blockbuf);
	если (retval)
		вернуть retval;

	dx_info.name = name;
	dx_info.namelen = strlen(name);
again:
	retval = dx_lookup(fs, dir, diri, &dx_info);
	если (retval)
		goto free_buf;

	retval = add_dirent_to_buf(fs,
		ext2fs_le32_to_cpu(dx_info.frames[dx_info.levels-1].at->block) & 0x0fffffff,
		blockbuf, dir, diri, name, ino, flags, &leaf_pblk);
	/*
	 * Success or error other than ENOSPC...? We are done. We may need upto
	 * two tries to add entry. One to split htree node and another to add
	 * new leaf block.
	 */
	если (restart >= dx_info.levels || retval != EXT2_ET_DIR_NO_SPACE)
		goto free_frames;
	retval = dx_grow_tree(fs, dir, diri, &dx_info, blockbuf, leaf_pblk);
	если (retval)
		goto free_frames;
	/* Restart everything now that the tree is larger */
	restart++;
	dx_release(&dx_info);
	goto again;
free_frames:
	dx_release(&dx_info);
free_buf:
	ext2fs_free_mem(&blockbuf);
	вернуть retval;
}

struct fake_dirent
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

struct dx_root
{
	struct fake_dirent dot;
	char dot_name[4];
	struct fake_dirent dotdot;
	char dotdot_name[4];
	struct ext2_dx_root_info info;
	struct ext2_dx_entry entries[];
};

static целое check_dx_root(ext2_filsys fs, ext2_ino_t ino, struct dx_root *root)
{
	struct fake_dirent *fde;
	char *error_msg;
	unsigned целое rlen;
	char *blockend = (char *)root + fs->blocksize;

	fde = &root->dot;
	если (fde->name_len != 1) {
		error_msg = "invalid name_len for '.'";
		goto corrupted;
	}
	если (strncmp(root->dot_name, ".", fde->name_len)) {
		error_msg = "invalid name for '.'";
		goto corrupted;
	}

	ext2fs_get_rec_len(fs, (struct ext2_dir_entry *)fde, &rlen);
	если ((char *)fde + rlen >= blockend) {
		error_msg = "invalid rec_len for '.'";
		goto corrupted;
	}

	fde = &root->dotdot;
	если (fde->name_len != 2) {
		error_msg = "invalid name_len for '..'";
		goto corrupted;
	}
	если (strncmp(root->dotdot_name, "..", fde->name_len)) {
		error_msg = "invalid name for '..'";
		goto corrupted;
	}
	ext2fs_get_rec_len(fs, (struct ext2_dir_entry *)fde, &rlen);
	если ((char *)fde + rlen >= blockend) {
		error_msg = "invalid rec_len for '..'";
		goto corrupted;
	}

	вернуть 1;

corrupted:
	fprintf(stderr, "Corrupt dir %u: %s, running e2fsck is recommended\n",
			 ino, error_msg);
	вернуть 0;
}

static inline struct ext2_dir_entry *
ext2fs_next_entry(ext2_filsys fs, struct ext2_dir_entry *p)
{
	unsigned целое rlen;

	ext2fs_get_rec_len(fs, p, &rlen);
	вернуть (struct ext2_dir_entry *)((char *)p + rlen);
}

static inline void dx_set_block(struct ext2_dx_entry *entry, unsigned value)
{
	entry->block = ext2fs_cpu_to_le32(value);
}

static inline void dx_set_count(struct ext2_dx_entry *entries, unsigned value)
{
	((struct ext2_dx_countlimit *) entries)->count = ext2fs_cpu_to_le16(value);
}

static inline void dx_set_limit(struct ext2_dx_entry *entries, unsigned value)
{
	((struct ext2_dx_countlimit *) entries)->limit = ext2fs_cpu_to_le16(value);
}

static inline unsigned dx_root_limit(ext2_filsys fs, unsigned infosize)
{
	unsigned целое entry_space = fs->blocksize -
			ext2fs_dir_rec_len(1, 0) -
			ext2fs_dir_rec_len(2, 0) - infosize;

	если (ext2fs_has_feature_metadata_csum(fs->super))
		entry_space -= sizeof(struct ext2_dx_tail);
	вернуть entry_space / sizeof(struct ext2_dx_entry);
}

/*
 * This converts a one block unindexed directory to a 3 block indexed
 * directory, and adds the dentry to the indexed directory.  Returns 0 if the
 * index was not created; EAGAIN if it was; or an errcode_t on error.
 */
static errcode_t try_make_indexed_dir(ext2_filsys fs, ext2_ino_t dir,
				      const char *name, ext2_ino_t ino,
				      целое flags)
{
	struct ext2_inode inode;
	char *buf0, *buf1, *top;
	struct dx_root *root;
	struct ext2_dx_entry *entries;
	struct fake_dirent *fde;
	struct ext2_dir_entry *de, *de2;
	blk64_t pblk0, pblk1;
	unsigned целое len;
	const unsigned целое blocksize = fs->blocksize;
	целое csum_size = 0;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	если (retval)
		вернуть retval;

	если (inode.i_size > fs->blocksize || (inode.i_flags & EXT2_INDEX_FL))
		вернуть 0;

	если (ext2fs_has_feature_metadata_csum(fs->super))
		csum_size = sizeof(struct ext2_dir_entry_tail);

	retval = ext2fs_get_mem(blocksize, &buf0);
	если (retval)
		вернуть retval;

	retval = ext2fs_get_mem(blocksize, &buf1);
	если (retval)
		goto out_buf0;

	retval = ext2fs_bmap2(fs, dir, &inode, NULL, 0, 0, NULL, &pblk0);
	если (retval)
		goto out_buf1;

	retval = ext2fs_read_dir_block4(fs, pblk0, buf0, 0, dir);
	если (retval)
		goto out_buf1;

	root = (struct dx_root *)buf0;
	если (!check_dx_root(fs, dir, root)) {
		retval = EXT2_ET_DIR_CORRUPTED;
		goto out_buf1;
	}

	/* The 0th block becomes the root, move the dirents out */
	fde = &root->dotdot;
	de = ext2fs_next_entry(fs, (struct ext2_dir_entry *)fde);
	len = ((char *) root) + (blocksize - csum_size) - (char *) de;

	/* Allocate new block for the 0th block's dirents */
	retval = ext2fs_bmap2(fs, dir, &inode, NULL, BMAP_ALLOC | BMAP_ZERO, 1,
			      NULL, &pblk1);
	если (retval)
		goto out_buf1;

	memcpy(buf1, de, len);
	memset(de, 0, len); /* wipe old data */

	de = (struct ext2_dir_entry *)buf1;
	top = buf1 + len;
	пока ((char *)(de2 = ext2fs_next_entry(fs, de)) < top) {
#если 0
		если (ext4_check_dir_entry(dir, NULL, de, bh2, buf1, len,
					(char *)de - buf1)) {
			retval = EXT2_ET_DIR_CORRUPTED;
			goto out_buf1;
		}
#endif
		de = de2;
	}
	retval = ext2fs_set_rec_len(fs,
			buf1 + (blocksize - csum_size) - (char *) de, de);
	если (retval)
		goto out_buf1;

	если (csum_size)
		ext2fs_initialize_dirent_tail(fs,
				EXT2_DIRENT_TAIL(buf1, fs->blocksize));

	/* Initialize the root; the dot dirents already exist */
	de = (struct ext2_dir_entry *) (&root->dotdot);
	retval = ext2fs_set_rec_len(fs, blocksize - ext2fs_dir_rec_len(2, 0),
				    de);
	memset (&root->info, 0, sizeof(root->info));
	root->info.info_length = sizeof(root->info);
	если (ext4_hash_in_dirent(&inode))
		root->info.hash_version = EXT2_HASH_SIPHASH;
	иначе
		root->info.hash_version = fs->super->s_def_hash_version;

	entries = root->entries;
	dx_set_block(entries, 1);
	dx_set_count(entries, 1);
	dx_set_limit(entries, dx_root_limit(fs, sizeof(root->info)));

	retval = ext2fs_write_dir_block4(fs, pblk1, buf1, 0, dir);
	если (retval)
		goto out_buf1;

	retval = ext2fs_write_dir_block4(fs, pblk0, buf0, 0, dir);
	если (retval)
		goto out_buf1;

	inode.i_flags |= EXT2_INDEX_FL;
	inode.i_size += fs->blocksize;
	retval = ext2fs_write_inode(fs, dir, &inode);
	если (retval)
		goto out_buf1;

	retval = EAGAIN;
out_buf1:
	ext2fs_free_mem(&buf1);
out_buf0:
	ext2fs_free_mem(&buf0);
	вернуть retval;
}

/*
 * Note: the low 3 bits of the flags field are used as the directory
 * entry filetype.
 */
#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_link(ext2_filsys fs, ext2_ino_t dir, const char *name,
		      ext2_ino_t ino, целое flags)
{
	errcode_t		retval;
	struct link_struct	ls;
	struct ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	если (!(fs->flags & EXT2_FLAG_RW))
		вернуть EXT2_ET_RO_FILSYS;

retry:
	если ((retval = ext2fs_read_inode(fs, dir, &inode)) != 0)
		вернуть retval;

	если (inode.i_flags & EXT2_INDEX_FL)
		вернуть dx_link(fs, dir, &inode, name, ino, flags);

	ls.fs = fs;
	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.sb = fs->super;
	ls.blocksize = fs->blocksize;
	ls.err = 0;

	если ((flags & EXT2FS_LINK_APPEND) &&
	    !(inode.i_flags & EXT4_INLINE_DATA_FL)) {
		blk64_t		lblk, pblk = 0;
		struct dir_context ctx;

		lblk = (inode.i_size / fs->blocksize) - 1;
		retval = ext2fs_bmap2(fs, dir, &inode, NULL, 0, lblk,
				      NULL, &pblk);
		если (retval)
			вернуть retval;

		ctx.dir = dir;
		ctx.flags = DIRENT_FLAG_INCLUDE_EMPTY;
		ctx.func = link_proc;
		ctx.priv_data = &ls;
		ctx.errcode = 0;
		retval = ext2fs_get_mem(fs->blocksize, &ctx.buf);
		если (retval)
			вернуть retval;

		ext2fs_process_dir_block(fs, &pblk, lblk, 0, 0, &ctx);
		retval = ctx.errcode;
		ext2fs_free_mem(&ctx.buf);
	} иначе {
		retval = ext2fs_dir_iterate2(fs, dir,
					     DIRENT_FLAG_INCLUDE_EMPTY,
					     NULL, link_proc, &ls);
	}
	если (retval)
		вернуть retval;
	если (ls.err)
		вернуть ls.err;

	если (!ls.done) {
		если (!(flags & EXT2FS_LINK_EXPAND))
			вернуть EXT2_ET_DIR_NO_SPACE;

		если (ext2fs_has_feature_dir_index(fs->super)) {
			retval = try_make_indexed_dir(fs, dir, name, ino,
						      flags);
			если (retval == EAGAIN)
				goto retry;
			если (retval)
				вернуть retval;
		}

		retval = ext2fs_expand_dir(fs, dir);
		если (retval)
			вернуть retval;
		goto retry;
	}
	вернуть 0;
}

/* Does this directory have an htree index? */
целое ext2fs_dir_is_dx(ext2_filsys fs, const struct ext2_inode *inode)
{
	вернуть ext2fs_has_feature_dir_index(fs->super) &&
		S_ISDIR(inode->i_mode) && (inode->i_flags & EXT2_INDEX_FL);
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
void ext2fs_inc_nlink(ext2_filsys fs, struct ext2_inode *inode)
{
	inode->i_links_count++;

	если (ext2fs_dir_is_dx(fs, inode) &&
	    (inode->i_links_count > EXT2_LINK_MAX || inode->i_links_count == 2))
		inode->i_links_count = 1;
}

/*
 * If a directory had nlink == 1, then we should let it be 1. This indicates
 * directory has >EXT2_LINK_MAX subdirs.
 */
void ext2fs_dec_nlink(struct ext2_inode *inode)
{
	если (!S_ISDIR(inode->i_mode) || inode->i_links_count > 2)
		inode->i_links_count--;
}

целое ext2fs_dir_link_max(ext2_filsys fs, struct ext2_inode_large *inode)
{
	вернуть inode->i_links_count >= EXT2_LINK_MAX &&
	       !(ext2fs_dir_is_dx(fs, EXT2_INODE(inode)) &&
		 ext2fs_has_feature_dir_nlink(fs->super));
}

целое ext2fs_dir_link_empty(struct ext2_inode *inode)
{
	вернуть inode->i_links_count == 2 || inode->i_links_count == 1;
}
