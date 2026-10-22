/*
 * unlink.c --- delete links in a ext2fs directory
 *
 * Copyright (C) 1993, 1994, 1997 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"

સંરચના link_struct  {
	અચલ અક્ષર	*name;
	પૂર્ણાંક		namelen;
	ext2_ino_t	inode;
	પૂર્ણાંક		flags;
	સંરચના ext2_dir_entry *prev;
	પૂર્ણાંક		done;
};

#ifdef __TURBOC__
 #pragma argsused
#endif
સ્થિર પૂર્ણાંક unlink_proc(સંરચના ext2_dir_entry *dirent,
		     પૂર્ણાંક	offset,
		     પૂર્ણાંક	blocksize EXT2FS_ATTR((unused)),
		     અક્ષર	*buf EXT2FS_ATTR((unused)),
		     શૂન્ય	*priv_data)
{
	સંરચના link_struct *ls = (સંરચના link_struct *) priv_data;
	સંરચના ext2_dir_entry *prev;

	prev = ls->prev;
	ls->prev = dirent;

	જો (ls->name) {
		જો (ext2fs_dirent_name_len(dirent) != ls->namelen)
			પાછા 0;
		જો (strncmp(ls->name, dirent->name, ext2fs_dirent_name_len(dirent)))
			પાછા 0;
	}
	જો (!(ls->flags & EXT2FS_UNLINK_FORCE) && ls->inode) {
		જો (dirent->inode != ls->inode)
			પાછા 0;
	} નહીંતર {
		જો (!dirent->inode)
			પાછા 0;
	}

	જો (offset)
		prev->rec_len += dirent->rec_len;
	નહીંતર
		dirent->inode = 0;
	ls->done++;
	પાછા DIRENT_ABORT|DIRENT_CHANGED;
}

#ifdef __TURBOC__
 #pragma argsused
#endif
errcode_t ext2fs_unlink(ext2_filsys fs, ext2_ino_t dir,
			અચલ અક્ષર *name, ext2_ino_t ino,
			પૂર્ણાંક flags)
{
	errcode_t	retval;
	સંરચના link_struct ls;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	જો (!name && !ino)
		પાછા EXT2_ET_INVALID_ARGUMENT;

	જો (!(fs->flags & EXT2_FLAG_RW))
		પાછા EXT2_ET_RO_FILSYS;

	ls.name = name;
	ls.namelen = name ? strlen(name) : 0;
	ls.inode = ino;
	ls.flags = flags;
	ls.done = 0;
	ls.prev = 0;

	retval = ext2fs_dir_iterate(fs, dir, DIRENT_FLAG_INCLUDE_EMPTY,
				    0, unlink_proc, &ls);
	જો (retval)
		પાછા retval;

	પાછા (ls.done) ? 0 : EXT2_ET_DIR_NO_SPACE;
}

