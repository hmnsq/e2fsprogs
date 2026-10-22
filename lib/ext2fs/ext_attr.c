/*
 * ext_attr.c --- extended attribute blocks
 *
 * Copyright (C) 2001 Andreas Gruenbacher, <a.gruenbacher@computer.org>
 *
 * Copyright (C) 2002 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <string.h>
#అన్తర్భావ <time.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2_ext_attr.h"
#అన్తర్భావ "ext4_acl.h"

#అన్తర్భావ "ext2fsP.h"

స్థిర errcode_t read_ea_inode_hash(ext2_filsys fs, ext2_ino_t ino, __u32 *hash)
{
	నిర్మాణం ext2_inode inode;
	errcode_t retval;

	retval = ext2fs_read_inode(fs, ino, &inode);
	ఒకవేళ (retval)
		తిరిగి retval;
	*hash = ext2fs_get_ea_inode_hash(&inode);
	తిరిగి 0;
}

#నిర్వచించు NAME_HASH_జరుపు 5
#నిర్వచించు విలువ_HASH_జరుపు 16

/*
 * ext2_xattr_hash_entry()
 *
 * Compute the hash of an extended attribute.
 */
__u32 ext2fs_ext_attr_hash_నమోదు(నిర్మాణం ext2_ext_attr_నమోదు *నమోదు_2, శూన్య *దత్తాంశం)
{
	__u32 hash = 0;
	గుర్తులేని అక్షరం *name = (((గుర్తులేని అక్షరం *) నమోదు_2) +
			       పరిమాణం(నిర్మాణం ext2_ext_attr_నమోదు));
	పూర్ణాంకం n;

	కొరకు (n = 0; n < నమోదు_2->e_name_len; n++) {
		hash = (hash << NAME_HASH_జరుపు) ^
		       (hash >> (8*పరిమాణం(hash) - NAME_HASH_జరుపు)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	ఒకవేళ (నమోదు_2->e_విలువ_inum == 0 && నమోదు_2->e_విలువ_కొలత != 0) {
		__u32 *విలువ = (__u32 *)దత్తాంశం;
		కొరకు (n = (నమోదు_2->e_విలువ_కొలత + EXT2_EXT_ATTR_విడత) >>
			 EXT2_EXT_ATTR_PAD_బిట్లు; n; n--) {
			hash = (hash << విలువ_HASH_జరుపు) ^
			       (hash >> (8*పరిమాణం(hash) - విలువ_HASH_జరుపు)) ^
			       ext2fs_le32_గమ్యం_cpu(*విలువ++);
		}
	}

	తిరిగి hash;
}

__u32 ext2fs_ext_attr_hash_నమోదు_signed(నిర్మాణం ext2_ext_attr_నమోదు *నమోదు_2,
					శూన్య *దత్తాంశం)
{
	__u32 hash = 0;
	గుర్తుగల అక్షరం *name = (((గుర్తుగల అక్షరం *) నమోదు_2) +
			     పరిమాణం(నిర్మాణం ext2_ext_attr_నమోదు));
	పూర్ణాంకం n;

	కొరకు (n = 0; n < నమోదు_2->e_name_len; n++) {
		hash = (hash << NAME_HASH_జరుపు) ^
		       (hash >> (8*పరిమాణం(hash) - NAME_HASH_జరుపు)) ^
		       *name++;
	}

	/* The hash needs to be calculated on the data in little-endian. */
	ఒకవేళ (నమోదు_2->e_విలువ_inum == 0 && నమోదు_2->e_విలువ_కొలత != 0) {
		__u32 *విలువ = (__u32 *)దత్తాంశం;
		కొరకు (n = (నమోదు_2->e_విలువ_కొలత + EXT2_EXT_ATTR_విడత) >>
			 EXT2_EXT_ATTR_PAD_బిట్లు; n; n--) {
			hash = (hash << విలువ_HASH_జరుపు) ^
			       (hash >> (8*పరిమాణం(hash) - విలువ_HASH_జరుపు)) ^
			       ext2fs_le32_గమ్యం_cpu(*విలువ++);
		}
	}

	తిరిగి hash;
}


/*
 * ext2fs_ext_attr_hash_entry3()
 *
 * Compute the hash of an extended attribute.  This version of the
 * function supports hashing entries that reference external inodes
 * (ea_inode feature) as well as calculating the old legacy signed
 * hash variant.
 */
errcode_t ext2fs_ext_attr_hash_entry3(ext2_filsys fs,
				      నిర్మాణం ext2_ext_attr_నమోదు *నమోదు_2,
				      శూన్య *దత్తాంశం, __u32 *hash,
				      __u32 *signed_hash)
{
	*hash = ext2fs_ext_attr_hash_నమోదు(నమోదు_2, దత్తాంశం);
	ఒకవేళ (signed_hash)
		*signed_hash = ext2fs_ext_attr_hash_నమోదు_signed(నమోదు_2, దత్తాంశం);

	ఒకవేళ (నమోదు_2->e_విలువ_inum) {
		__u32 ea_inode_hash;
		errcode_t retval;

		retval = read_ea_inode_hash(fs, నమోదు_2->e_విలువ_inum,
					    &ea_inode_hash);
		ఒకవేళ (retval)
			తిరిగి retval;

		*hash = (*hash << విలువ_HASH_జరుపు) ^
			(*hash >> (8*పరిమాణం(*hash) - విలువ_HASH_జరుపు)) ^
			ea_inode_hash;
		ఒకవేళ (signed_hash)
			*signed_hash = (*signed_hash << విలువ_HASH_జరుపు) ^
				(*signed_hash >> (8*పరిమాణం(*hash) -
						  విలువ_HASH_జరుపు)) ^
				ea_inode_hash;
	}
	తిరిగి 0;
}

/*
 * ext2fs_ext_attr_hash_entry2()
 *
 * Compute the hash of an extended attribute.
 * This version of the function supports hashing entries that reference
 * external inodes (ea_inode feature).
 */
errcode_t ext2fs_ext_attr_hash_entry2(ext2_filsys fs,
				      నిర్మాణం ext2_ext_attr_నమోదు *నమోదు_2,
				      శూన్య *దత్తాంశం, __u32 *hash)
{
	తిరిగి ext2fs_ext_attr_hash_entry3(fs, నమోదు_2, దత్తాంశం, hash, ఖాళీ);
}

#నిర్వచనం_రద్దు NAME_HASH_జరుపు
#నిర్వచనం_రద్దు విలువ_HASH_జరుపు

#నిర్వచించు ఖండం_HASH_జరుపు 16

/* Mirrors ext4_xattr_rehash() implementation in kernel. */
శూన్య ext2fs_ext_attr_ఖండం_rehash(నిర్మాణం ext2_ext_attr_శీర్షిక *శీర్షిక,
				  నిర్మాణం ext2_ext_attr_నమోదు *చివర)
{
	నిర్మాణం ext2_ext_attr_నమోదు *ఇక్కడ;
	__u32 hash = 0;

	ఇక్కడ = (నిర్మాణం ext2_ext_attr_నమోదు *)(శీర్షిక+1);
	అయితే (ఇక్కడ < చివర && !EXT2_EXT_IS_చివరి_నమోదు(ఇక్కడ)) {
		ఒకవేళ (!ఇక్కడ->e_hash) {
			/* Block is not shared if an entry's hash value == 0 */
			hash = 0;
			ఆపు;
		}
		hash = (hash << ఖండం_HASH_జరుపు) ^
		       (hash >> (8*పరిమాణం(hash) - ఖండం_HASH_జరుపు)) ^
		       ఇక్కడ->e_hash;
		ఇక్కడ = EXT2_EXT_ATTR_తదుపరి(ఇక్కడ);
	}
	శీర్షిక->h_hash = hash;
}

#నిర్వచనం_రద్దు ఖండం_HASH_జరుపు

__u32 ext2fs_get_ea_inode_hash(నిర్మాణం ext2_inode *inode)
{
	తిరిగి inode->i_atime;
}

శూన్య ext2fs_సమితి_ea_inode_hash(నిర్మాణం ext2_inode *inode, __u32 hash)
{
	inode->i_atime = hash;
}

__u64 ext2fs_get_ea_inode_ref(నిర్మాణం ext2_inode *inode)
{
	తిరిగి ((__u64)inode->i_ctime << 32) | inode->osd1.linux1.l_i_సంస్కరణ;
}

శూన్య ext2fs_సమితి_ea_inode_ref(నిర్మాణం ext2_inode *inode, __u64 ref_లెక్క)
{
	inode->i_ctime = (__u32)(ref_లెక్క >> 32);
	inode->osd1.linux1.l_i_సంస్కరణ = (__u32)ref_లెక్క;
}

స్థిర errcode_t check_ext_attr_శీర్షిక(నిర్మాణం ext2_ext_attr_శీర్షిక *శీర్షిక)
{
	ఒకవేళ ((శీర్షిక->h_magic != EXT2_EXT_ATTR_MAGIC_v1 &&
	     శీర్షిక->h_magic != EXT2_EXT_ATTR_MAGIC) ||
	    శీర్షిక->h_ఖండాలు != 1)
		తిరిగి EXT2_ET_BAD_EA_శీర్షిక;

	తిరిగి 0;
}

errcode_t ext2fs_read_ext_attr3(ext2_filsys fs, blk64_t ఖండం, శూన్య *buf,
				ext2_ino_t inum)
{
	పూర్ణాంకం		csum_failed = 0;
	errcode_t	retval;

	retval = io_channel_read_blk64(fs->io, ఖండం, 1, buf);
	ఒకవేళ (retval)
		తిరిగి retval;

	ఒకవేళ (!(fs->flags & EXT2_FLAG_IGNORE_CSUM_ERRORS) &&
	    !ext2fs_ext_attr_ఖండం_csum_verify(fs, inum, ఖండం, buf))
		csum_failed = 1;

#నిర్వచితమైతే పదాలు_BIGENDIAN
	ext2fs_మార్పిడి_ext_attr(buf, buf, fs->blocksize, 1);
#అయితే_ముగింపు

	retval = check_ext_attr_శీర్షిక(buf);
	ఒకవేళ (retval == 0 && csum_failed)
		retval = EXT2_ET_EXT_ATTR_CSUM_INVALID;

	తిరిగి retval;
}

errcode_t ext2fs_read_ext_attr2(ext2_filsys fs, blk64_t ఖండం, శూన్య *buf)
{
	తిరిగి ext2fs_read_ext_attr3(fs, ఖండం, buf, 0);
}

errcode_t ext2fs_read_ext_attr(ext2_filsys fs, blk_t ఖండం, శూన్య *buf)
{
	తిరిగి ext2fs_read_ext_attr2(fs, ఖండం, buf);
}

errcode_t ext2fs_write_ext_attr3(ext2_filsys fs, blk64_t ఖండం, శూన్య *inbuf,
				 ext2_ino_t inum)
{
	errcode_t	retval;
	అక్షరం		*write_buf;

#నిర్వచితమైతే పదాలు_BIGENDIAN
	retval = ext2fs_get_mem(fs->blocksize, &write_buf);
	ఒకవేళ (retval)
		తిరిగి retval;
	ext2fs_మార్పిడి_ext_attr(write_buf, inbuf, fs->blocksize, 1);
#లేకపోతే
	write_buf = (అక్షరం *) inbuf;
#అయితే_ముగింపు

	retval = ext2fs_ext_attr_ఖండం_csum_సమితి(fs, inum, ఖండం,
			(నిర్మాణం ext2_ext_attr_శీర్షిక *)write_buf);
	ఒకవేళ (retval)
		తిరిగి retval;

	retval = io_channel_write_blk64(fs->io, ఖండం, 1, write_buf);
#నిర్వచితమైతే పదాలు_BIGENDIAN
	ext2fs_free_mem(&write_buf);
#అయితే_ముగింపు
	ఒకవేళ (!retval)
		ext2fs_mark_మారింది(fs);
	తిరిగి retval;
}

errcode_t ext2fs_write_ext_attr2(ext2_filsys fs, blk64_t ఖండం, శూన్య *inbuf)
{
	తిరిగి ext2fs_write_ext_attr3(fs, ఖండం, inbuf, 0);
}

errcode_t ext2fs_write_ext_attr(ext2_filsys fs, blk_t ఖండం, శూన్య *inbuf)
{
	తిరిగి ext2fs_write_ext_attr2(fs, ఖండం, inbuf);
}

/*
 * This function adjusts the reference count of the EA block.
 */
errcode_t ext2fs_adjust_ea_refcount3(ext2_filsys fs, blk64_t blk,
				    అక్షరం *ఖండం_buf, పూర్ణాంకం adjust,
				    __u32 *newcount, ext2_ino_t inum)
{
	errcode_t	retval;
	నిర్మాణం ext2_ext_attr_శీర్షిక *శీర్షిక;
	అక్షరం	*buf = 0;

	ఒకవేళ ((blk >= ext2fs_ఖండాలు_లెక్క(fs->super)) ||
	    (blk < fs->super->s_మొదటి_దత్తాంశం_ఖండం))
		తిరిగి EXT2_ET_BAD_EA_ఖండం_అంకె;

	ఒకవేళ (!ఖండం_buf) {
		retval = ext2fs_get_mem(fs->blocksize, &buf);
		ఒకవేళ (retval)
			తిరిగి retval;
		ఖండం_buf = buf;
	}

	retval = ext2fs_read_ext_attr3(fs, blk, ఖండం_buf, inum);
	ఒకవేళ (retval)
		వెళ్ళు errout;

	శీర్షిక = (నిర్మాణం ext2_ext_attr_శీర్షిక *) ఖండం_buf;
	శీర్షిక->h_refcount += adjust;
	ఒకవేళ (newcount)
		*newcount = శీర్షిక->h_refcount;

	retval = ext2fs_write_ext_attr3(fs, blk, ఖండం_buf, inum);
	ఒకవేళ (retval)
		వెళ్ళు errout;

errout:
	ఒకవేళ (buf)
		ext2fs_free_mem(&buf);
	తిరిగి retval;
}

errcode_t ext2fs_adjust_ea_refcount2(ext2_filsys fs, blk64_t blk,
				    అక్షరం *ఖండం_buf, పూర్ణాంకం adjust,
				    __u32 *newcount)
{
	తిరిగి ext2fs_adjust_ea_refcount3(fs, blk, ఖండం_buf, adjust,
					  newcount, 0);
}

errcode_t ext2fs_adjust_ea_refcount(ext2_filsys fs, blk_t blk,
					అక్షరం *ఖండం_buf, పూర్ణాంకం adjust,
					__u32 *newcount)
{
	తిరిగి ext2fs_adjust_ea_refcount2(fs, blk, ఖండం_buf, adjust,
					  newcount);
}

/* Manipulate the contents of extended attribute regions */
నిర్మాణం ext2_xattr {
	పూర్ణాంకం name_సూచీ;
	అక్షరం *name;
	అక్షరం *short_name;
	శూన్య *విలువ;
	గుర్తులేని పూర్ణాంకం విలువ_len;
	ext2_ino_t ea_ino;
};

నిర్మాణం ext2_xattr_handle {
	errcode_t magic;
	ext2_filsys fs;
	నిర్మాణం ext2_xattr *attrs;
	పూర్ణాంకం సామర్థ్యం;
	పూర్ణాంకం లెక్క;
	పూర్ణాంకం ibody_లెక్క;
	ext2_ino_t ino;
	గుర్తులేని పూర్ణాంకం flags;
};

స్థిర errcode_t ext2fs_xattrs_expand(నిర్మాణం ext2_xattr_handle *h,
				      గుర్తులేని పూర్ణాంకం expandby)
{
	నిర్మాణం ext2_xattr *new_attrs;
	errcode_t err;

	err = ext2fs_get_arrayzero(h->సామర్థ్యం + expandby,
				   పరిమాణం(నిర్మాణం ext2_xattr), &new_attrs);
	ఒకవేళ (err)
		తిరిగి err;

	జ్ఞాపకం_నకలు(new_attrs, h->attrs, h->సామర్థ్యం * పరిమాణం(నిర్మాణం ext2_xattr));
	ext2fs_free_mem(&h->attrs);
	h->సామర్థ్యం += expandby;
	h->attrs = new_attrs;

	తిరిగి 0;
}

నిర్మాణం ea_name_సూచీ {
	పూర్ణాంకం సూచీ;
	స్థిరరాశి అక్షరం *name;
};

/* Keep these names sorted in order of decreasing specificity. */
స్థిర నిర్మాణం ea_name_సూచీ ea_పేర్లు[] = {
	{10, "gnu."},
	{3, "system.posix_acl_default"},
	{2, "system.posix_acl_access"},
	{8, "system.richacl"},
	{6, "security."},
	{4, "trusted."},
	{7, "system."},
	{1, "user."},
	{0, ఖాళీ},
};

స్థిర స్థిరరాశి అక్షరం *కనుగొను_ea_ఉపసర్గ(పూర్ణాంకం సూచీ)
{
	నిర్మాణం ea_name_సూచీ *e;

	కొరకు (e = ea_పేర్లు; e->name; e++)
		ఒకవేళ (e->సూచీ == సూచీ)
			తిరిగి e->name;

	తిరిగి ఖాళీ;
}

స్థిర పూర్ణాంకం కనుగొను_ea_సూచీ(స్థిరరాశి అక్షరం *fullname, స్థిరరాశి అక్షరం **name, పూర్ణాంకం *సూచీ)
{
	నిర్మాణం ea_name_సూచీ *e;

	కొరకు (e = ea_పేర్లు; e->name; e++) {
		ఒకవేళ (అక్షరశ్రేణి_పోలిక(fullname, e->name, అక్షరశ్రేణి_పొడవు(e->name)) == 0) {
			*name = fullname + అక్షరశ్రేణి_పొడవు(e->name);
			*సూచీ = e->సూచీ;
			తిరిగి 1;
		}
	}
	తిరిగి 0;
}

errcode_t ext2fs_free_ext_attr(ext2_filsys fs, ext2_ino_t ino,
			       నిర్మాణం ext2_inode_large *inode)
{
	నిర్మాణం ext2_ext_attr_శీర్షిక *శీర్షిక;
	శూన్య *ఖండం_buf = ఖాళీ;
	blk64_t blk;
	errcode_t err;
	నిర్మాణం ext2_inode_large i;

	/* Read inode? */
	ఒకవేళ (inode == ఖాళీ) {
		err = ext2fs_read_inode_నిండిన(fs, ino, (నిర్మాణం ext2_inode *)&i,
					     పరిమాణం(నిర్మాణం ext2_inode_large));
		ఒకవేళ (err)
			తిరిగి err;
		inode = &i;
	}

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_ఖండం(fs, (నిర్మాణం ext2_inode *)inode);
	ఒకవేళ (blk == 0)
		తిరిగి 0;

	/* Find block, zero it, write back */
	ఒకవేళ ((blk < fs->super->s_మొదటి_దత్తాంశం_ఖండం) ||
	    (blk >= ext2fs_ఖండాలు_లెక్క(fs->super))) {
		err = EXT2_ET_BAD_EA_ఖండం_అంకె;
		వెళ్ళు బహిర్గతం;
	}

	err = ext2fs_get_mem(fs->blocksize, &ఖండం_buf);
	ఒకవేళ (err)
		వెళ్ళు బహిర్గతం;

	err = ext2fs_read_ext_attr3(fs, blk, ఖండం_buf, ino);
	ఒకవేళ (err)
		వెళ్ళు out2;

	/* We only know how to deal with v2 EA blocks */
	శీర్షిక = (నిర్మాణం ext2_ext_attr_శీర్షిక *) ఖండం_buf;
	ఒకవేళ (శీర్షిక->h_magic != EXT2_EXT_ATTR_MAGIC) {
		err = EXT2_ET_BAD_EA_శీర్షిక;
		వెళ్ళు out2;
	}

	శీర్షిక->h_refcount--;
	err = ext2fs_write_ext_attr3(fs, blk, ఖండం_buf, ino);
	ఒకవేళ (err)
		వెళ్ళు out2;

	/* Erase link to block */
	ext2fs_file_acl_ఖండం_సమితి(fs, (నిర్మాణం ext2_inode *)inode, 0);
	ఒకవేళ (శీర్షిక->h_refcount == 0)
		ext2fs_ఖండం_alloc_stats2(fs, blk, -1);
	err = ext2fs_iblk_sub_ఖండాలు(fs, (నిర్మాణం ext2_inode *)inode, 1);
	ఒకవేళ (err)
		వెళ్ళు out2;

	/* Write inode? */
	ఒకవేళ (inode == &i) {
		err = ext2fs_write_inode_నిండిన(fs, ino, (నిర్మాణం ext2_inode *)&i,
					      పరిమాణం(నిర్మాణం ext2_inode_large));
		ఒకవేళ (err)
			వెళ్ళు out2;
	}

out2:
	ext2fs_free_mem(&ఖండం_buf);
బహిర్గతం:
	తిరిగి err;
}

స్థిర errcode_t prep_ea_ఖండం_for_write(ext2_filsys fs, ext2_ino_t ino,
					 నిర్మాణం ext2_inode_large *inode)
{
	నిర్మాణం ext2_ext_attr_శీర్షిక *శీర్షిక;
	శూన్య *ఖండం_buf = ఖాళీ;
	blk64_t blk, goal;
	errcode_t err;

	/* Do we already have an EA block? */
	blk = ext2fs_file_acl_ఖండం(fs, (నిర్మాణం ext2_inode *)inode);
	ఒకవేళ (blk != 0) {
		ఒకవేళ ((blk < fs->super->s_మొదటి_దత్తాంశం_ఖండం) ||
		    (blk >= ext2fs_ఖండాలు_లెక్క(fs->super))) {
			err = EXT2_ET_BAD_EA_ఖండం_అంకె;
			వెళ్ళు బహిర్గతం;
		}

		err = ext2fs_get_mem(fs->blocksize, &ఖండం_buf);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;

		err = ext2fs_read_ext_attr3(fs, blk, ఖండం_buf, ino);
		ఒకవేళ (err)
			వెళ్ళు out2;

		/* We only know how to deal with v2 EA blocks */
		శీర్షిక = (నిర్మాణం ext2_ext_attr_శీర్షిక *) ఖండం_buf;
		ఒకవేళ (శీర్షిక->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_శీర్షిక;
			వెళ్ళు out2;
		}

		/* Single-user block.  We're done here. */
		ఒకవేళ (శీర్షిక->h_refcount == 1)
			వెళ్ళు out2;

		/* We need to CoW the block. */
		శీర్షిక->h_refcount--;
		err = ext2fs_write_ext_attr3(fs, blk, ఖండం_buf, ino);
		ఒకవేళ (err)
			వెళ్ళు out2;
	} లేకపోతే {
		/* No block, we must increment i_blocks */
		err = ext2fs_iblk_కూడు_ఖండాలు(fs, (నిర్మాణం ext2_inode *)inode,
					     1);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;
	}

	/* Allocate a block */
	goal = ext2fs_కనుగొను_inode_goal(fs, ino, (నిర్మాణం ext2_inode *)inode, 0);
	err = ext2fs_alloc_block2(fs, goal, ఖాళీ, &blk);
	ఒకవేళ (err)
		వెళ్ళు out2;
	ext2fs_file_acl_ఖండం_సమితి(fs, (నిర్మాణం ext2_inode *)inode, blk);
out2:
	ఒకవేళ (ఖండం_buf)
		ext2fs_free_mem(&ఖండం_buf);
బహిర్గతం:
	తిరిగి err;
}


స్థిర అంతఃవరుస పూర్ణాంకం
posix_acl_xattr_లెక్క(size_t కొలత)
{
        ఒకవేళ (కొలత < పరిమాణం(posix_acl_xattr_శీర్షిక))
                తిరిగి -1;
        కొలత -= పరిమాణం(posix_acl_xattr_శీర్షిక);
        ఒకవేళ (కొలత % పరిమాణం(posix_acl_xattr_నమోదు))
                తిరిగి -1;
        తిరిగి కొలత / పరిమాణం(posix_acl_xattr_నమోదు);
}

/*
 * The lgetxattr function returns data formatted in the POSIX extended
 * attribute format.  The on-disk format uses a more compact encoding.
 * See the ext4_acl_to_disk in fs/ext4/acl.c.
 */
స్థిర errcode_t convert_posix_acl_గమ్యం_disk_బఫరు(స్థిరరాశి శూన్య *విలువ, size_t కొలత,
						  శూన్య *బహిర్గతం_buf, size_t *కొలత_బహిర్గతం)
{
	స్థిరరాశి posix_acl_xattr_శీర్షిక *శీర్షిక =
		(స్థిరరాశి posix_acl_xattr_శీర్షిక*) విలువ;
	స్థిరరాశి posix_acl_xattr_నమోదు *చివర, *నమోదు_2 =
		(స్థిరరాశి posix_acl_xattr_నమోదు *)(శీర్షిక+1);
	ext4_acl_శీర్షిక *ext_acl;
	size_t s;
	అక్షరం *e;

	పూర్ణాంకం లెక్క;

	ఒకవేళ (!విలువ)
		తిరిగి EINVAL;
	ఒకవేళ (కొలత < పరిమాణం(posix_acl_xattr_శీర్షిక))
		తిరిగి ENOMEM;
	ఒకవేళ (శీర్షిక->a_సంస్కరణ != ext2fs_cpu_గమ్యం_le32(POSIX_ACL_XATTR_సంస్కరణ))
		తిరిగి EINVAL;

	లెక్క = posix_acl_xattr_లెక్క(కొలత);
	ext_acl = బహిర్గతం_buf;
	ext_acl->a_సంస్కరణ = ext2fs_cpu_గమ్యం_le32(EXT4_ACL_సంస్కరణ);

	ఒకవేళ (లెక్క <= 0)
		తిరిగి EINVAL;

	e = (అక్షరం *) బహిర్గతం_buf + పరిమాణం(ext4_acl_శీర్షిక);
	s = పరిమాణం(ext4_acl_శీర్షిక);
	కొరకు (చివర = నమోదు_2 + లెక్క; నమోదు_2 != చివర;నమోదు_2++) {
		ext4_acl_నమోదు *disk_నమోదు = (ext4_acl_నమోదు*) e;
		disk_నమోదు->e_ట్యాగ్ = నమోదు_2->e_ట్యాగ్;
		disk_నమోదు->e_perm = నమోదు_2->e_perm;

		ఎంపిక(ext2fs_le16_గమ్యం_cpu(నమోదు_2->e_ట్యాగ్)) {
			స్థితి ACL_USER_OBJ:
			స్థితి ACL_సమూహం_OBJ:
			స్థితి ACL_ముసుగు:
			స్థితి ACL_OTHER:
				e += పరిమాణం(ext4_acl_నమోదు_short);
				s += పరిమాణం(ext4_acl_నమోదు_short);
				ఆపు;
			స్థితి ACL_USER:
			స్థితి ACL_సమూహం:
				disk_నమోదు->e_id = నమోదు_2->e_id;
				e += పరిమాణం(ext4_acl_నమోదు);
				s += పరిమాణం(ext4_acl_నమోదు);
				ఆపు;
			అప్రమేయ:
				తిరిగి EINVAL;
		}
	}
	*కొలత_బహిర్గతం = s;
	తిరిగి 0;
}

స్థిర errcode_t convert_disk_బఫరు_గమ్యం_posix_acl(స్థిరరాశి శూన్య *విలువ, size_t కొలత,
						  శూన్య **బహిర్గతం_buf, size_t *కొలత_బహిర్గతం)
{
	posix_acl_xattr_శీర్షిక *శీర్షిక;
	posix_acl_xattr_నమోదు *నమోదు_2;
	స్థిరరాశి ext4_acl_శీర్షిక *ext_acl = (స్థిరరాశి ext4_acl_శీర్షిక *) విలువ;
	errcode_t err;
	స్థిరరాశి అక్షరం *cp;
	అక్షరం *బహిర్గతం;

	ఒకవేళ ((!విలువ) ||
	    (కొలత < పరిమాణం(ext4_acl_శీర్షిక)) ||
	    (ext_acl->a_సంస్కరణ != ext2fs_cpu_గమ్యం_le32(EXT4_ACL_సంస్కరణ)))
		తిరిగి EINVAL;

	err = ext2fs_get_mem(కొలత * 2, &బహిర్గతం);
	ఒకవేళ (err)
		తిరిగి err;

	శీర్షిక = (posix_acl_xattr_శీర్షిక *) బహిర్గతం;
	శీర్షిక->a_సంస్కరణ = ext2fs_cpu_గమ్యం_le32(POSIX_ACL_XATTR_సంస్కరణ);
	నమోదు_2 = (posix_acl_xattr_నమోదు *) (బహిర్గతం + పరిమాణం(posix_acl_xattr_శీర్షిక));

	cp = (స్థిరరాశి అక్షరం *) విలువ + పరిమాణం(ext4_acl_శీర్షిక);
	కొలత -= పరిమాణం(ext4_acl_శీర్షిక);

	అయితే (కొలత > 0) {
		స్థిరరాశి ext4_acl_నమోదు *disk_నమోదు = (స్థిరరాశి ext4_acl_నమోదు *) cp;

		నమోదు_2->e_ట్యాగ్ = disk_నమోదు->e_ట్యాగ్;
		నమోదు_2->e_perm = disk_నమోదు->e_perm;

		ఎంపిక(ext2fs_le16_గమ్యం_cpu(నమోదు_2->e_ట్యాగ్)) {
			స్థితి ACL_USER_OBJ:
			స్థితి ACL_సమూహం_OBJ:
			స్థితి ACL_ముసుగు:
			స్థితి ACL_OTHER:
				నమోదు_2->e_id = 0;
				cp += పరిమాణం(ext4_acl_నమోదు_short);
				కొలత -= పరిమాణం(ext4_acl_నమోదు_short);
				ఆపు;
			స్థితి ACL_USER:
			స్థితి ACL_సమూహం:
				నమోదు_2->e_id = disk_నమోదు->e_id;
				cp += పరిమాణం(ext4_acl_నమోదు);
				కొలత -= పరిమాణం(ext4_acl_నమోదు);
				ఆపు;
			అప్రమేయ:
				ext2fs_free_mem(&బహిర్గతం);
				తిరిగి EINVAL;
		}
		నమోదు_2++;
	}
	*బహిర్గతం_buf = బహిర్గతం;
	*కొలత_బహిర్గతం = ((అక్షరం *) నమోదు_2 - బహిర్గతం);
	తిరిగి 0;
}

స్థిర errcode_t
write_xattrs_గమ్యం_బఫరు(ext2_filsys fs, నిర్మాణం ext2_xattr *attrs, పూర్ణాంకం లెక్క,
		       శూన్య *entries_ప్రారంభం, గుర్తులేని పూర్ణాంకం storage_కొలత,
		       గుర్తులేని పూర్ణాంకం విలువ_స్థానభ్రంశం_correction, పూర్ణాంకం write_hash)
{
	నిర్మాణం ext2_xattr *x;
	నిర్మాణం ext2_ext_attr_నమోదు *e = entries_ప్రారంభం;
	అక్షరం *చివర = (అక్షరం *) entries_ప్రారంభం + storage_కొలత;
	గుర్తులేని పూర్ణాంకం విలువ_కొలత;
	errcode_t err;

	memset(entries_ప్రారంభం, 0, storage_కొలత);
	కొరకు (x = attrs; x < attrs + లెక్క; x++) {
		విలువ_కొలత = ((x->విలువ_len + EXT2_EXT_ATTR_PAD - 1) /
			      EXT2_EXT_ATTR_PAD) * EXT2_EXT_ATTR_PAD;

		/* Fill out e appropriately */
		e->e_name_len = అక్షరశ్రేణి_పొడవు(x->short_name);
		e->e_name_సూచీ = x->name_సూచీ;

		e->e_విలువ_కొలత = x->విలువ_len;
		e->e_విలువ_inum = x->ea_ino;

		/* Store name */
		జ్ఞాపకం_నకలు((అక్షరం *)e + పరిమాణం(*e), x->short_name, e->e_name_len);
		ఒకవేళ (x->ea_ino) {
			e->e_విలువ_offs = 0;
		} లేకపోతే {
			చివర -= విలువ_కొలత;
			e->e_విలువ_offs = చివర - (అక్షరం *) entries_ప్రారంభం +
						విలువ_స్థానభ్రంశం_correction;
			జ్ఞాపకం_నకలు(చివర, x->విలువ, e->e_విలువ_కొలత);
		}

		ఒకవేళ (write_hash || x->ea_ino) {
			err = ext2fs_ext_attr_hash_entry2(fs, e,
							  x->ea_ino ? 0 : చివర,
							  &e->e_hash);
			ఒకవేళ (err)
				తిరిగి err;
		} లేకపోతే
			e->e_hash = 0;

		e = EXT2_EXT_ATTR_తదుపరి(e);
		*(__u32 *)e = 0;
	}
	తిరిగి 0;
}

errcode_t ext2fs_xattrs_write(నిర్మాణం ext2_xattr_handle *handle)
{
	ext2_filsys fs = handle->fs;
	స్థిరరాశి గుర్తులేని పూర్ణాంకం inode_కొలత = EXT2_INODE_కొలత(fs->super);
	నిర్మాణం ext2_inode_large *inode;
	అక్షరం *ప్రారంభం, *ఖండం_buf = ఖాళీ;
	నిర్మాణం ext2_ext_attr_శీర్షిక *శీర్షిక;
	__u32 ea_inode_magic;
	blk64_t blk;
	గుర్తులేని పూర్ణాంకం storage_కొలత;
	గుర్తులేని పూర్ణాంకం i;
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	i = inode_కొలత;
	ఒకవేళ (i < పరిమాణం(*inode))
		i = పరిమాణం(*inode);
	err = ext2fs_get_memzero(i, &inode);
	ఒకవేళ (err)
		తిరిగి err;

	err = ext2fs_read_inode_నిండిన(fs, handle->ino, EXT2_INODE(inode),
				     inode_కొలత);
	ఒకవేళ (err)
		వెళ్ళు బహిర్గతం;

	/* If extra_isize isn't set, we need to set it now */
	ఒకవేళ (inode->i_extra_isize == 0 &&
	    inode_కొలత > EXT2_GOOD_OLD_INODE_కొలత) {
		అక్షరం *p = (అక్షరం *)inode;
		size_t extra = fs->super->s_want_extra_isize;

		ఒకవేళ (extra == 0)
			extra = పరిమాణం(__u32);
		memset(p + EXT2_GOOD_OLD_INODE_కొలత, 0, extra);
		inode->i_extra_isize = extra;
	}
	ఒకవేళ (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		వెళ్ళు బహిర్గతం;
	}

	/* Does the inode have space for EA? */
	ఒకవేళ (inode->i_extra_isize < పరిమాణం(inode->i_extra_isize) ||
	    inode_కొలత <= EXT2_GOOD_OLD_INODE_కొలత + inode->i_extra_isize +
								పరిమాణం(__u32))
		వెళ్ళు write_ea_ఖండం;

	/* Write the inode EA */
	ea_inode_magic = EXT2_EXT_ATTR_MAGIC;
	జ్ఞాపకం_నకలు(((అక్షరం *) inode) + EXT2_GOOD_OLD_INODE_కొలత +
	       inode->i_extra_isize, &ea_inode_magic, పరిమాణం(__u32));
	storage_కొలత = inode_కొలత - EXT2_GOOD_OLD_INODE_కొలత -
				inode->i_extra_isize - పరిమాణం(__u32);
	ప్రారంభం = ((అక్షరం *) inode) + EXT2_GOOD_OLD_INODE_కొలత +
				inode->i_extra_isize + పరిమాణం(__u32);

	err = write_xattrs_గమ్యం_బఫరు(fs, handle->attrs, handle->ibody_లెక్క,
				     ప్రారంభం, storage_కొలత, 0, 0);
	ఒకవేళ (err)
		వెళ్ళు బహిర్గతం;
write_ea_ఖండం:
	/* Are we done? */
	ఒకవేళ (handle->ibody_లెక్క == handle->లెక్క &&
	    !ext2fs_file_acl_ఖండం(fs, EXT2_INODE(inode)))
		వెళ్ళు skip_ea_ఖండం;

	/* Write the EA block */
	err = ext2fs_get_memzero(fs->blocksize, &ఖండం_buf);
	ఒకవేళ (err)
		వెళ్ళు బహిర్గతం;

	storage_కొలత = fs->blocksize - పరిమాణం(నిర్మాణం ext2_ext_attr_శీర్షిక);
	ప్రారంభం = ఖండం_buf + పరిమాణం(నిర్మాణం ext2_ext_attr_శీర్షిక);

	err = write_xattrs_గమ్యం_బఫరు(fs, handle->attrs + handle->ibody_లెక్క,
				     handle->లెక్క - handle->ibody_లెక్క, ప్రారంభం,
				     storage_కొలత, ప్రారంభం - ఖండం_buf, 1);
	ఒకవేళ (err)
		వెళ్ళు out2;

	/* Write a header on the EA block */
	శీర్షిక = (నిర్మాణం ext2_ext_attr_శీర్షిక *) ఖండం_buf;
	శీర్షిక->h_magic = EXT2_EXT_ATTR_MAGIC;
	శీర్షిక->h_refcount = 1;
	శీర్షిక->h_ఖండాలు = 1;

	/* Get a new block for writing */
	err = prep_ea_ఖండం_for_write(fs, handle->ino, inode);
	ఒకవేళ (err)
		వెళ్ళు out2;

	/* Finally, write the new EA block */
	blk = ext2fs_file_acl_ఖండం(fs, EXT2_INODE(inode));
	err = ext2fs_write_ext_attr3(fs, blk, ఖండం_buf, handle->ino);
	ఒకవేళ (err)
		వెళ్ళు out2;

skip_ea_ఖండం:
	blk = ext2fs_file_acl_ఖండం(fs, (నిర్మాణం ext2_inode *)inode);
	ఒకవేళ (!ఖండం_buf && blk) {
		/* xattrs shrunk, free the block */
		err = ext2fs_free_ext_attr(fs, handle->ino, inode);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;
	}

	/* Write the inode */
	err = ext2fs_write_inode_నిండిన(fs, handle->ino, EXT2_INODE(inode),
				      inode_కొలత);
	ఒకవేళ (err)
		వెళ్ళు out2;

out2:
	ext2fs_free_mem(&ఖండం_buf);
బహిర్గతం:
	ext2fs_free_mem(&inode);
	తిరిగి err;
}

స్థిర errcode_t read_xattrs_from_బఫరు(నిర్మాణం ext2_xattr_handle *handle,
					 నిర్మాణం ext2_inode_large *inode,
					 నిర్మాణం ext2_ext_attr_నమోదు *entries,
					 గుర్తులేని పూర్ణాంకం storage_కొలత,
					 అక్షరం *విలువ_ప్రారంభం)
{
	నిర్మాణం ext2_xattr *x;
	నిర్మాణం ext2_ext_attr_నమోదు *నమోదు_2, *చివర;
	స్థిరరాశి అక్షరం *ఉపసర్గ;
	గుర్తులేని పూర్ణాంకం remain, ఉపసర్గ_len;
	errcode_t err;
	గుర్తులేని పూర్ణాంకం విలువలు_కొలత = storage_కొలత +
			((అక్షరం *)entries - విలువ_ప్రారంభం);

	/* find the end */
	చివర = entries;
	remain = storage_కొలత;
	అయితే (remain >= పరిమాణం(నిర్మాణం ext2_ext_attr_నమోదు) &&
	       !EXT2_EXT_IS_చివరి_నమోదు(చివర)) {

		/* header eats this space */
		remain -= పరిమాణం(నిర్మాణం ext2_ext_attr_నమోదు);

		/* is attribute name valid? */
		ఒకవేళ (EXT2_EXT_ATTR_కొలత(చివర->e_name_len) > remain)
			తిరిగి EXT2_ET_EA_BAD_NAME_LEN;

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_కొలత(చివర->e_name_len);
		చివర = EXT2_EXT_ATTR_తదుపరి(చివర);
	}

	నమోదు_2 = entries;
	remain = storage_కొలత;
	అయితే (remain >= పరిమాణం(నిర్మాణం ext2_ext_attr_నమోదు) &&
	       !EXT2_EXT_IS_చివరి_నమోదు(నమోదు_2)) {

		/* Allocate space for more attrs? */
		ఒకవేళ (handle->లెక్క == handle->సామర్థ్యం) {
			err = ext2fs_xattrs_expand(handle, 4);
			ఒకవేళ (err)
				తిరిగి err;
		}

		x = handle->attrs + handle->లెక్క;

		/* header eats this space */
		remain -= పరిమాణం(నిర్మాణం ext2_ext_attr_నమోదు);

		/* attribute len eats this space */
		remain -= EXT2_EXT_ATTR_కొలత(నమోదు_2->e_name_len);

		/* Extract name */
		ఉపసర్గ = కనుగొను_ea_ఉపసర్గ(నమోదు_2->e_name_సూచీ);
		ఉపసర్గ_len = (ఉపసర్గ ? అక్షరశ్రేణి_పొడవు(ఉపసర్గ) : 0);
		err = ext2fs_get_memzero(నమోదు_2->e_name_len + ఉపసర్గ_len + 1,
					 &x->name);
		ఒకవేళ (err)
			తిరిగి err;
		ఒకవేళ (ఉపసర్గ)
			జ్ఞాపకం_నకలు(x->name, ఉపసర్గ, ఉపసర్గ_len);
		ఒకవేళ (నమోదు_2->e_name_len)
			జ్ఞాపకం_నకలు(x->name + ఉపసర్గ_len,
			       (అక్షరం *)నమోదు_2 + పరిమాణం(*నమోదు_2),
			       నమోదు_2->e_name_len);
		x->short_name = x->name + ఉపసర్గ_len;
		x->name_సూచీ = నమోదు_2->e_name_సూచీ;

		/* Check & copy value */
		ఒకవేళ (!ext2fs_has_feature_ea_inode(handle->fs->super) &&
		    నమోదు_2->e_విలువ_inum != 0)
			తిరిగి EXT2_ET_BAD_EA_ఖండం_అంకె;

		ఒకవేళ (నమోదు_2->e_విలువ_inum == 0) {
			ఒకవేళ (నమోదు_2->e_విలువ_కొలత > remain)
				తిరిగి EXT2_ET_EA_BAD_విలువ_కొలత;

			ఒకవేళ (నమోదు_2->e_విలువ_offs + నమోదు_2->e_విలువ_కొలత > విలువలు_కొలత)
				తిరిగి EXT2_ET_EA_BAD_విలువ_స్థానభ్రంశం;

			ఒకవేళ (నమోదు_2->e_విలువ_కొలత > 0 &&
			    విలువ_ప్రారంభం + నమోదు_2->e_విలువ_offs <
			    (అక్షరం *)చివర + పరిమాణం(__u32))
				తిరిగి EXT2_ET_EA_BAD_విలువ_స్థానభ్రంశం;

			remain -= నమోదు_2->e_విలువ_కొలత;

			err = ext2fs_get_mem(నమోదు_2->e_విలువ_కొలత, &x->విలువ);
			ఒకవేళ (err)
				తిరిగి err;
			జ్ఞాపకం_నకలు(x->విలువ, విలువ_ప్రారంభం + నమోదు_2->e_విలువ_offs,
			       నమోదు_2->e_విలువ_కొలత);
		} లేకపోతే {
			నిర్మాణం ext2_inode *ea_inode;
			ext2_file_t ea_file;

			ఒకవేళ (నమోదు_2->e_విలువ_offs != 0)
				తిరిగి EXT2_ET_EA_BAD_విలువ_స్థానభ్రంశం;

			ఒకవేళ (నమోదు_2->e_విలువ_కొలత > (64 * 1024))
				తిరిగి EXT2_ET_EA_BAD_విలువ_కొలత;

			err = ext2fs_get_mem(నమోదు_2->e_విలువ_కొలత, &x->విలువ);
			ఒకవేళ (err)
				తిరిగి err;

			err = ext2fs_file_open(handle->fs, నమోదు_2->e_విలువ_inum,
					       0, &ea_file);
			ఒకవేళ (err)
				తిరిగి err;

			ea_inode = ext2fs_file_get_inode(ea_file);
			ఒకవేళ ((ea_inode->i_flags & EXT4_INLINE_దత్తాంశం_FL) ||
			    !(ea_inode->i_flags & EXT4_EA_INODE_FL) ||
			    ea_inode->i_links_లెక్క == 0)
				err = EXT2_ET_EA_INODE_CORRUPTED;
			లేకపోతే ఒకవేళ ((__u64) ext2fs_file_get_కొలత(ea_file) !=
				 నమోదు_2->e_విలువ_కొలత)
				err = EXT2_ET_EA_BAD_విలువ_కొలత;
			లేకపోతే
				err = ext2fs_file_read(ea_file, x->విలువ,
						       నమోదు_2->e_విలువ_కొలత, 0);
			ext2fs_file_close(ea_file);
			ఒకవేళ (err)
				తిరిగి err;
		}

		x->ea_ino = నమోదు_2->e_విలువ_inum;
		x->విలువ_len = నమోదు_2->e_విలువ_కొలత;

		/* e_hash may be 0 in older inode's ea */
		ఒకవేళ (నమోదు_2->e_hash != 0) {
			__u32 hash, signed_hash;

			శూన్య *దత్తాంశం = (నమోదు_2->e_విలువ_inum != 0) ?
					0 : విలువ_ప్రారంభం + నమోదు_2->e_విలువ_offs;

			err = ext2fs_ext_attr_hash_entry3(handle->fs, నమోదు_2,
							  దత్తాంశం, &hash,
							  &signed_hash);
			ఒకవేళ (err)
				తిరిగి err;
			ఒకవేళ ((నమోదు_2->e_hash != hash) &&
			    (నమోదు_2->e_hash != signed_hash)) {
				నిర్మాణం ext2_inode బిడ్డ;

				/* Check whether this is an old Lustre-style
				 * ea_inode reference.
				 */
				err = ext2fs_read_inode(handle->fs,
							నమోదు_2->e_విలువ_inum,
							&బిడ్డ);
				ఒకవేళ (err)
					తిరిగి err;
				ఒకవేళ (బిడ్డ.i_mtime != handle->ino ||
				    బిడ్డ.i_generation != inode->i_generation)
					తిరిగి EXT2_ET_BAD_EA_HASH;
			}
		}

		handle->లెక్క++;
		నమోదు_2 = EXT2_EXT_ATTR_తదుపరి(నమోదు_2);
	}

	తిరిగి 0;
}

స్థిర శూన్య xattrs_free_కీలకాలు(నిర్మాణం ext2_xattr_handle *h)
{
	నిర్మాణం ext2_xattr *a = h->attrs;
	పూర్ణాంకం i;

	కొరకు (i = 0; i < h->సామర్థ్యం; i++) {
		ఒకవేళ (a[i].name)
			ext2fs_free_mem(&a[i].name);
		ఒకవేళ (a[i].విలువ)
			ext2fs_free_mem(&a[i].విలువ);
	}
	h->లెక్క = 0;
	h->ibody_లెక్క = 0;
}

/* fetch xattrs from an already-loaded inode */
errcode_t ext2fs_xattrs_read_inode(నిర్మాణం ext2_xattr_handle *handle,
				   నిర్మాణం ext2_inode_large *inode)
{
	నిర్మాణం ext2_ext_attr_శీర్షిక *శీర్షిక;
	__u32 ea_inode_magic;
	గుర్తులేని పూర్ణాంకం storage_కొలత;
	అక్షరం *ప్రారంభం, *ఖండం_buf = ఖాళీ;
	blk64_t blk;
	errcode_t err = 0;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	xattrs_free_కీలకాలు(handle);

	/* Does the inode have space for EA? */
	ఒకవేళ (inode->i_extra_isize < పరిమాణం(inode->i_extra_isize) ||
	    EXT2_INODE_కొలత(handle->fs->super) <= EXT2_GOOD_OLD_INODE_కొలత +
						  inode->i_extra_isize +
						  పరిమాణం(__u32))
		వెళ్ళు read_ea_ఖండం;
	ఒకవేళ (inode->i_extra_isize & 3) {
		err = EXT2_ET_INODE_CORRUPTED;
		వెళ్ళు బహిర్గతం;
	}

	/* Look for EA in the inode */
	జ్ఞాపకం_నకలు(&ea_inode_magic, ((అక్షరం *) inode) + EXT2_GOOD_OLD_INODE_కొలత +
	       inode->i_extra_isize, పరిమాణం(__u32));
	ఒకవేళ (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		storage_కొలత = EXT2_INODE_కొలత(handle->fs->super) -
			EXT2_GOOD_OLD_INODE_కొలత - inode->i_extra_isize -
			పరిమాణం(__u32);
		ప్రారంభం = ((అక్షరం *) inode) + EXT2_GOOD_OLD_INODE_కొలత +
			inode->i_extra_isize + పరిమాణం(__u32);

		err = read_xattrs_from_బఫరు(handle, inode,
					(నిర్మాణం ext2_ext_attr_నమోదు *) ప్రారంభం,
					storage_కొలత, ప్రారంభం);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;

		handle->ibody_లెక్క = handle->లెక్క;
	}

read_ea_ఖండం:
	/* Look for EA in a separate EA block */
	blk = ext2fs_file_acl_ఖండం(handle->fs, EXT2_INODE(inode));
	ఒకవేళ (blk != 0) {
		ఒకవేళ ((blk < handle->fs->super->s_మొదటి_దత్తాంశం_ఖండం) ||
		    (blk >= ext2fs_ఖండాలు_లెక్క(handle->fs->super))) {
			err = EXT2_ET_BAD_EA_ఖండం_అంకె;
			వెళ్ళు బహిర్గతం;
		}

		err = ext2fs_get_mem(handle->fs->blocksize, &ఖండం_buf);
		ఒకవేళ (err)
			వెళ్ళు బహిర్గతం;

		err = ext2fs_read_ext_attr3(handle->fs, blk, ఖండం_buf,
					    handle->ino);
		ఒకవేళ (err)
			వెళ్ళు out3;

		/* We only know how to deal with v2 EA blocks */
		శీర్షిక = (నిర్మాణం ext2_ext_attr_శీర్షిక *) ఖండం_buf;
		ఒకవేళ (శీర్షిక->h_magic != EXT2_EXT_ATTR_MAGIC) {
			err = EXT2_ET_BAD_EA_శీర్షిక;
			వెళ్ళు out3;
		}

		/* Read EAs */
		storage_కొలత = handle->fs->blocksize -
			పరిమాణం(నిర్మాణం ext2_ext_attr_శీర్షిక);
		ప్రారంభం = ఖండం_buf + పరిమాణం(నిర్మాణం ext2_ext_attr_శీర్షిక);
		err = read_xattrs_from_బఫరు(handle, inode,
					(నిర్మాణం ext2_ext_attr_నమోదు *) ప్రారంభం,
					storage_కొలత, ఖండం_buf);
	}

out3:
	ఒకవేళ (ఖండం_buf)
		ext2fs_free_mem(&ఖండం_buf);
బహిర్గతం:
	తిరిగి err;
}

errcode_t ext2fs_xattrs_read(నిర్మాణం ext2_xattr_handle *handle)
{
	నిర్మాణం ext2_inode_large *inode;
	size_t inode_కొలత = EXT2_INODE_కొలత(handle->fs->super);
	errcode_t err;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);

	ఒకవేళ (inode_కొలత < పరిమాణం(*inode))
		inode_కొలత = పరిమాణం(*inode);
	err = ext2fs_get_memzero(inode_కొలత, &inode);
	ఒకవేళ (err)
		తిరిగి err;

	err = ext2fs_read_inode_నిండిన(handle->fs, handle->ino, EXT2_INODE(inode),
				     EXT2_INODE_కొలత(handle->fs->super));
	ఒకవేళ (err)
		వెళ్ళు బహిర్గతం;

	err = ext2fs_xattrs_read_inode(handle, inode);

బహిర్గతం:
	ext2fs_free_mem(&inode);

	తిరిగి err;
}

errcode_t ext2fs_xattrs_iterate(నిర్మాణం ext2_xattr_handle *h,
				పూర్ణాంకం (*func)(అక్షరం *name, అక్షరం *విలువ,
					    size_t విలువ_len, శూన్య *దత్తాంశం),
				శూన్య *దత్తాంశం)
{
	నిర్మాణం ext2_xattr *x;
	పూర్ణాంకం dirty = 0;
	పూర్ణాంకం ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	కొరకు (x = h->attrs; x < h->attrs + h->లెక్క; x++) {
		ret = func(x->name, x->విలువ, x->విలువ_len, దత్తాంశం);
		ఒకవేళ (ret & XATTR_మారింది)
			dirty = 1;
		ఒకవేళ (ret & XATTR_ABORT)
			ఆపు;
	}

	ఒకవేళ (dirty)
		తిరిగి ext2fs_xattrs_write(h);
	తిరిగి 0;
}

errcode_t ext2fs_xattr_get(నిర్మాణం ext2_xattr_handle *h, స్థిరరాశి అక్షరం *కీలకం,
			   శూన్య **విలువ, size_t *విలువ_len)
{
	నిర్మాణం ext2_xattr *x;
	అక్షరం *val;
	errcode_t err;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	కొరకు (x = h->attrs; x < h->attrs + h->లెక్క; x++) {
		ఒకవేళ (strcmp(x->name, కీలకం))
			కొనసాగు;

		ఒకవేళ (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
		    ((strcmp(కీలకం, "system.posix_acl_default") == 0) ||
		     (strcmp(కీలకం, "system.posix_acl_access") == 0))) {
			err = convert_disk_బఫరు_గమ్యం_posix_acl(x->విలువ, x->విలువ_len,
							       విలువ, విలువ_len);
			తిరిగి err;
		} లేకపోతే {
			err = ext2fs_get_mem(x->విలువ_len, &val);
			ఒకవేళ (err)
				తిరిగి err;
			జ్ఞాపకం_నకలు(val, x->విలువ, x->విలువ_len);
			*విలువ = val;
			*విలువ_len = x->విలువ_len;
			తిరిగి 0;
		}
	}

	తిరిగి EXT2_ET_EA_కీలకం_NOT_దొరికింది;
}

errcode_t ext2fs_xattr_inode_max_కొలత(ext2_filsys fs, ext2_ino_t ino,
				      size_t *కొలత)
{
	నిర్మాణం ext2_ext_attr_నమోదు *నమోదు_2;
	నిర్మాణం ext2_inode_large *inode;
	__u32 ea_inode_magic;
	గుర్తులేని పూర్ణాంకం minoff;
	అక్షరం *ప్రారంభం;
	size_t i;
	errcode_t err;

	i = EXT2_INODE_కొలత(fs->super);
	ఒకవేళ (i < పరిమాణం(*inode))
		i = పరిమాణం(*inode);
	err = ext2fs_get_memzero(i, &inode);
	ఒకవేళ (err)
		తిరిగి err;

	err = ext2fs_read_inode_నిండిన(fs, ino, (నిర్మాణం ext2_inode *)inode,
				     EXT2_INODE_కొలత(fs->super));
	ఒకవేళ (err)
		వెళ్ళు బహిర్గతం;

	/* Does the inode have size for EA? */
	ఒకవేళ (EXT2_INODE_కొలత(fs->super) <= EXT2_GOOD_OLD_INODE_కొలత +
						  inode->i_extra_isize +
						  పరిమాణం(__u32)) {
		err = EXT2_ET_INLINE_దత్తాంశం_NO_SPACE;
		వెళ్ళు బహిర్గతం;
	}

	minoff = EXT2_INODE_కొలత(fs->super) - పరిమాణం(*inode) - పరిమాణం(__u32);
	జ్ఞాపకం_నకలు(&ea_inode_magic, ((అక్షరం *) inode) + EXT2_GOOD_OLD_INODE_కొలత +
	       inode->i_extra_isize, పరిమాణం(__u32));
	ఒకవేళ (ea_inode_magic == EXT2_EXT_ATTR_MAGIC) {
		/* has xattrs.  calculate the size */
		ప్రారంభం= ((అక్షరం *) inode) + EXT2_GOOD_OLD_INODE_కొలత +
			inode->i_extra_isize + పరిమాణం(__u32);
		నమోదు_2 = (నిర్మాణం ext2_ext_attr_నమోదు *) ప్రారంభం;
		అయితే (!EXT2_EXT_IS_చివరి_నమోదు(నమోదు_2)) {
			ఒకవేళ (!నమోదు_2->e_విలువ_inum && నమోదు_2->e_విలువ_కొలత) {
				గుర్తులేని పూర్ణాంకం offs = నమోదు_2->e_విలువ_offs;
				ఒకవేళ (offs < minoff)
					minoff = offs;
			}
			నమోదు_2 = EXT2_EXT_ATTR_తదుపరి(నమోదు_2);
		}
		*కొలత = minoff - ((అక్షరం *)నమోదు_2 - (అక్షరం *)ప్రారంభం) - పరిమాణం(__u32);
	} లేకపోతే {
		/* no xattr.  return a maximum size */
		*కొలత = EXT2_EXT_ATTR_కొలత(minoff -
					   EXT2_EXT_ATTR_LEN(అక్షరశ్రేణి_పొడవు("data")) -
					   EXT2_EXT_ATTR_విడత - పరిమాణం(__u32));
	}

బహిర్గతం:
	ext2fs_free_mem(&inode);
	తిరిగి err;
}

స్థిర errcode_t xattr_create_ea_inode(ext2_filsys fs, స్థిరరాశి శూన్య *విలువ,
				       size_t విలువ_len, ext2_ino_t *ea_ino)
{
	నిర్మాణం ext2_inode inode;
	ext2_ino_t ino;
	ext2_file_t file;
	__u32 hash;
	errcode_t ret;

	ret = ext2fs_new_inode(fs, 0, 0, 0, &ino);
	ఒకవేళ (ret)
		తిరిగి ret;

	memset(&inode, 0, పరిమాణం(inode));
	inode.i_flags |= EXT4_EA_INODE_FL;
	ఒకవేళ (ext2fs_has_feature_extents(fs->super))
		inode.i_flags |= EXT4_EXTENTS_FL;
	inode.i_కొలత = 0;
	inode.i_బహుళకం = LINUX_S_IFREG | 0600;
	inode.i_links_లెక్క = 1;
	ret = ext2fs_write_new_inode(fs, ino, &inode);
	ఒకవేళ (ret)
		తిరిగి ret;
	/*
	 * ref_count and hash utilize inode's i_*time fields.
	 * ext2fs_write_new_inode() call above initializes these fields with
	 * current time. That's why ref count and hash updates are done
	 * separately below.
	 */
	ext2fs_సమితి_ea_inode_ref(&inode, 1);
	hash = ext2fs_crc32c_le(fs->csum_విత్తనం, విలువ, విలువ_len);
	ext2fs_సమితి_ea_inode_hash(&inode, hash);

	ret = ext2fs_write_inode(fs, ino, &inode);
	ఒకవేళ (ret)
		తిరిగి ret;

	ret = ext2fs_file_open(fs, ino, EXT2_FILE_WRITE, &file);
	ఒకవేళ (ret)
		తిరిగి ret;
	ret = ext2fs_file_write(file, విలువ, విలువ_len, ఖాళీ);
	ext2fs_file_close(file);
	ఒకవేళ (ret)
		తిరిగి ret;

	ext2fs_inode_alloc_stats2(fs, ino, 1 /* inuse */, 0 /* isdir */);

	*ea_ino = ino;
	తిరిగి 0;
}

స్థిర errcode_t xattr_inode_dec_ref(ext2_filsys fs, ext2_ino_t ino)
{
	నిర్మాణం ext2_inode_large inode;
	__u64 ref_లెక్క;
	errcode_t ret;

	ret = ext2fs_read_inode_నిండిన(fs, ino, (నిర్మాణం ext2_inode *)&inode,
				     పరిమాణం(inode));
	ఒకవేళ (ret)
		వెళ్ళు బహిర్గతం;

	ref_లెక్క = ext2fs_get_ea_inode_ref(EXT2_INODE(&inode));
	ref_లెక్క--;
	ext2fs_సమితి_ea_inode_ref(EXT2_INODE(&inode), ref_లెక్క);

	ఒకవేళ (ref_లెక్క)
		వెళ్ళు write_బహిర్గతం;

	inode.i_links_లెక్క = 0;
	ext2fs_సమితి_dtime(fs, EXT2_INODE(&inode));

	ret = ext2fs_free_ext_attr(fs, ino, &inode);
	ఒకవేళ (ret)
		వెళ్ళు write_బహిర్గతం;

	ఒకవేళ (ext2fs_inode_has_valid_blocks2(fs, (నిర్మాణం ext2_inode *)&inode)) {
		ret = ext2fs_punch(fs, ino, (నిర్మాణం ext2_inode *)&inode, ఖాళీ,
				   0, ~0ULL);
		ఒకవేళ (ret)
			వెళ్ళు బహిర్గతం;
	}

	inode.i_flags &= ~EXT4_EA_INODE_FL;
	ext2fs_inode_alloc_stats2(fs, ino, -1 /* inuse */, 0 /* is_dir */);

write_బహిర్గతం:
	ret = ext2fs_write_inode_నిండిన(fs, ino, (నిర్మాణం ext2_inode *)&inode,
				      పరిమాణం(inode));
బహిర్గతం:
	తిరిగి ret;
}

స్థిర errcode_t xattr_update_నమోదు(ext2_filsys fs, నిర్మాణం ext2_xattr *x,
				    స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *short_name,
				    పూర్ణాంకం సూచీ, స్థిరరాశి శూన్య *విలువ,
				    size_t విలువ_len, పూర్ణాంకం లోపల_inode)
{
	ext2_ino_t ea_ino = 0;
	శూన్య *new_విలువ = ఖాళీ;
	అక్షరం *new_name = ఖాళీ;
	పూర్ణాంకం name_len;
	errcode_t ret;

	ఒకవేళ (!x->name) {
		name_len = అక్షరశ్రేణి_పొడవు(name);
		ret = ext2fs_get_mem(name_len + 1, &new_name);
		ఒకవేళ (ret)
			వెళ్ళు fail;
		జ్ఞాపకం_నకలు(new_name, name, name_len + 1);
	}

	ret = ext2fs_get_mem(విలువ_len, &new_విలువ);
	ఒకవేళ (ret)
		వెళ్ళు fail;
	జ్ఞాపకం_నకలు(new_విలువ, విలువ, విలువ_len);

	ఒకవేళ (లోపల_inode) {
		ret = xattr_create_ea_inode(fs, విలువ, విలువ_len, &ea_ino);
		ఒకవేళ (ret)
			వెళ్ళు fail;
	}

	ఒకవేళ (x->ea_ino) {
		ret = xattr_inode_dec_ref(fs, x->ea_ino);
		ఒకవేళ (ret)
			వెళ్ళు fail;
	}

	ఒకవేళ (!x->name) {
		x->name = new_name;
		x->short_name = new_name + (short_name  - name);
	}
	x->name_సూచీ = సూచీ;

	ఒకవేళ (x->విలువ)
		ext2fs_free_mem(&x->విలువ);
	x->విలువ = new_విలువ;
	x->విలువ_len = విలువ_len;
	x->ea_ino = ea_ino;
	తిరిగి 0;
fail:
	ఒకవేళ (new_name)
		ext2fs_free_mem(&new_name);
	ఒకవేళ (new_విలువ)
		ext2fs_free_mem(&new_విలువ);
	ఒకవేళ (ea_ino)
		xattr_inode_dec_ref(fs, ea_ino);
	తిరిగి ret;
}

స్థిర పూర్ణాంకం xattr_కనుగొను_position(నిర్మాణం ext2_xattr *attrs, పూర్ణాంకం లెక్క,
			       స్థిరరాశి అక్షరం *shortname, పూర్ణాంకం name_idx)
{
	నిర్మాణం ext2_xattr *x;
	పూర్ణాంకం i;
	పూర్ణాంకం shortname_len, x_shortname_len;

	shortname_len = అక్షరశ్రేణి_పొడవు(shortname);

	కొరకు (i = 0, x = attrs; i < లెక్క; i++, x++) {
		ఒకవేళ (name_idx < x->name_సూచీ)
			ఆపు;
		ఒకవేళ (name_idx > x->name_సూచీ)
			కొనసాగు;

		x_shortname_len = అక్షరశ్రేణి_పొడవు(x->short_name);
		ఒకవేళ (shortname_len < x_shortname_len)
			ఆపు;
		ఒకవేళ (shortname_len > x_shortname_len)
			కొనసాగు;

		ఒకవేళ (memcmp(shortname, x->short_name, shortname_len) <= 0)
			ఆపు;
	}
	తిరిగి i;
}

స్థిర errcode_t xattr_array_update(నిర్మాణం ext2_xattr_handle *h,
				    స్థిరరాశి అక్షరం *name,
				    స్థిరరాశి శూన్య *విలువ, size_t విలువ_len,
				    పూర్ణాంకం ibody_free, పూర్ణాంకం ఖండం_free,
				    పూర్ణాంకం old_idx, పూర్ణాంకం లోపల_inode)
{
	నిర్మాణం ext2_xattr tmp;
	పూర్ణాంకం కూడు_గమ్యం_ibody;
	పూర్ణాంకం needed;
	పూర్ణాంకం name_len, name_idx = 0;
	స్థిరరాశి అక్షరం *shortname = name;
	పూర్ణాంకం new_idx;
	పూర్ణాంకం ret;

	కనుగొను_ea_సూచీ(name, &shortname, &name_idx);
	name_len = అక్షరశ్రేణి_పొడవు(shortname);

	needed = EXT2_EXT_ATTR_LEN(name_len);
	ఒకవేళ (!లోపల_inode)
		needed += EXT2_EXT_ATTR_కొలత(విలువ_len);

	ఒకవేళ (old_idx >= 0 && old_idx < h->ibody_లెక్క) {
		ibody_free += EXT2_EXT_ATTR_LEN(name_len);
		ఒకవేళ (!h->attrs[old_idx].ea_ino)
			ibody_free += EXT2_EXT_ATTR_కొలత(
						h->attrs[old_idx].విలువ_len);
	}

	ఒకవేళ (needed <= ibody_free) {
		ఒకవేళ (old_idx < 0) {
			new_idx = h->ibody_లెక్క;
			కూడు_గమ్యం_ibody = 1;
			వెళ్ళు కూడు_new;
		}

		/* Update the existing entry. */
		ret = xattr_update_నమోదు(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, విలువ,
					 విలువ_len, లోపల_inode);
		ఒకవేళ (ret)
			తిరిగి ret;
		ఒకవేళ (h->ibody_లెక్క <= old_idx) {
			/* Move entry from block to the end of ibody. */
			tmp = h->attrs[old_idx];
			memmove(h->attrs + h->ibody_లెక్క + 1,
				h->attrs + h->ibody_లెక్క,
				(old_idx - h->ibody_లెక్క) * పరిమాణం(*h->attrs));
			h->attrs[h->ibody_లెక్క] = tmp;
			h->ibody_లెక్క++;
		}
		తిరిగి 0;
	}

	ఒకవేళ (h->ibody_లెక్క <= old_idx) {
		ఖండం_free += EXT2_EXT_ATTR_LEN(name_len);
		ఒకవేళ (!h->attrs[old_idx].ea_ino)
			ఖండం_free +=
				EXT2_EXT_ATTR_కొలత(h->attrs[old_idx].విలువ_len);
	}

	ఒకవేళ (needed > ఖండం_free)
		తిరిగి EXT2_ET_EA_NO_SPACE;

	ఒకవేళ (old_idx >= 0) {
		/* Update the existing entry. */
		ret = xattr_update_నమోదు(h->fs, &h->attrs[old_idx], name,
					 shortname, name_idx, విలువ,
					 విలువ_len, లోపల_inode);
		ఒకవేళ (ret)
			తిరిగి ret;
		ఒకవేళ (old_idx < h->ibody_లెక్క) {
			/*
			 * Move entry from ibody to the block. Note that
			 * entries in the block are sorted.
			 */
			new_idx = xattr_కనుగొను_position(h->attrs + h->ibody_లెక్క,
						      h->లెక్క - h->ibody_లెక్క,
						      shortname, name_idx);
			new_idx += h->ibody_లెక్క - 1;
			tmp = h->attrs[old_idx];
			memmove(h->attrs + old_idx, h->attrs + old_idx + 1,
				(new_idx - old_idx) * పరిమాణం(*h->attrs));
			h->attrs[new_idx] = tmp;
			h->ibody_లెక్క--;
		}
		తిరిగి 0;
	}

	new_idx = xattr_కనుగొను_position(h->attrs + h->ibody_లెక్క,
				      h->లెక్క - h->ibody_లెక్క,
				      shortname, name_idx);
	new_idx += h->ibody_లెక్క;
	కూడు_గమ్యం_ibody = 0;

కూడు_new:
	ఒకవేళ (h->లెక్క == h->సామర్థ్యం) {
		ret = ext2fs_xattrs_expand(h, 4);
		ఒకవేళ (ret)
			తిరిగి ret;
	}

	ret = xattr_update_నమోదు(h->fs, &h->attrs[h->లెక్క], name, shortname,
				 name_idx, విలువ, విలువ_len, లోపల_inode);
	ఒకవేళ (ret)
		తిరిగి ret;

	tmp = h->attrs[h->లెక్క];
	memmove(h->attrs + new_idx + 1, h->attrs + new_idx,
		(h->లెక్క - new_idx)*పరిమాణం(*h->attrs));
	h->attrs[new_idx] = tmp;
	ఒకవేళ (కూడు_గమ్యం_ibody)
		h->ibody_లెక్క++;
	h->లెక్క++;
	తిరిగి 0;
}

స్థిర పూర్ణాంకం space_వాడిన(నిర్మాణం ext2_xattr *attrs, పూర్ణాంకం లెక్క)
{
	పూర్ణాంకం మొత్తం = 0;
	నిర్మాణం ext2_xattr *x;
	పూర్ణాంకం i, len;

	కొరకు (i = 0, x = attrs; i < లెక్క; i++, x++) {
		len = అక్షరశ్రేణి_పొడవు(x->short_name);
		మొత్తం += EXT2_EXT_ATTR_LEN(len);
		ఒకవేళ (!x->ea_ino)
			మొత్తం += EXT2_EXT_ATTR_కొలత(x->విలువ_len);
	}
	తిరిగి మొత్తం;
}

/*
 * The minimum size of EA value when you start storing it in an external inode
 * size of block - size of header - size of 1 entry - 4 null bytes
 */
#నిర్వచించు EXT4_XATTR_MIN_LARGE_EA_కొలత(b)	\
	((b) - EXT2_EXT_ATTR_LEN(3) - పరిమాణం(నిర్మాణం ext2_ext_attr_శీర్షిక) - 4)

errcode_t ext2fs_xattr_సమితి(నిర్మాణం ext2_xattr_handle *h,
			   స్థిరరాశి అక్షరం *name,
			   స్థిరరాశి శూన్య *విలువ,
			   size_t విలువ_len)
{
	ext2_filsys fs = h->fs;
	స్థిరరాశి పూర్ణాంకం inode_కొలత = EXT2_INODE_కొలత(fs->super);
	నిర్మాణం ext2_inode_large *inode = ఖాళీ;
	నిర్మాణం ext2_xattr *x;
	అక్షరం *new_విలువ;
	పూర్ణాంకం ibody_free, ఖండం_free;
	పూర్ణాంకం లోపల_inode = 0;
	పూర్ణాంకం old_idx = -1;
	పూర్ణాంకం extra_isize;
	errcode_t ret;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);

	ret = ext2fs_get_mem(విలువ_len, &new_విలువ);
	ఒకవేళ (ret)
		తిరిగి ret;
	ఒకవేళ (!(h->flags & XATTR_HANDLE_FLAG_RAW) &&
	    ((strcmp(name, "system.posix_acl_default") == 0) ||
	     (strcmp(name, "system.posix_acl_access") == 0))) {
		ret = convert_posix_acl_గమ్యం_disk_బఫరు(విలువ, విలువ_len,
						       new_విలువ, &విలువ_len);
		ఒకవేళ (ret)
			వెళ్ళు బహిర్గతం;
	} లేకపోతే ఒకవేళ (విలువ_len)
		జ్ఞాపకం_నకలు(new_విలువ, విలువ, విలువ_len);

	/* Imitate kernel behavior by skipping update if value is the same. */
	కొరకు (x = h->attrs; x < h->attrs + h->లెక్క; x++) {
		ఒకవేళ (!strcmp(x->name, name)) {
			ఒకవేళ (!x->ea_ino && x->విలువ_len == విలువ_len &&
			    (!విలువ_len ||
			     !memcmp(x->విలువ, new_విలువ, విలువ_len))) {
				ret = 0;
				వెళ్ళు బహిర్గతం;
			}
			old_idx = x - h->attrs;
			ఆపు;
		}
	}

	ret = ext2fs_get_memzero(inode_కొలత, &inode);
	ఒకవేళ (ret)
		వెళ్ళు బహిర్గతం;
	ret = ext2fs_read_inode_నిండిన(fs, h->ino,
				     (నిర్మాణం ext2_inode *)inode,
				     inode_కొలత);
	ఒకవేళ (ret)
		వెళ్ళు బహిర్గతం;
	ఒకవేళ (inode_కొలత > EXT2_GOOD_OLD_INODE_కొలత) {
		extra_isize = inode->i_extra_isize;
		ఒకవేళ (extra_isize == 0) {
			extra_isize = fs->super->s_want_extra_isize;
			ఒకవేళ (extra_isize == 0)
				extra_isize = పరిమాణం(__u32);
		}
		ibody_free = inode_కొలత - EXT2_GOOD_OLD_INODE_కొలత;
		ibody_free -= extra_isize;
		/* Extended attribute magic and final null entry. */
		ibody_free -= పరిమాణం(__u32) * 2;
		ibody_free -= space_వాడిన(h->attrs, h->ibody_లెక్క);
	} లేకపోతే
		ibody_free = 0;

	/* Inline data can only go to ibody. */
	ఒకవేళ (strcmp(name, "system.data") == 0) {
		ఒకవేళ (h->ibody_లెక్క <= old_idx) {
			ret = EXT2_ET_FILESYSTEM_CORRUPTED;
			వెళ్ళు బహిర్గతం;
		}
		ret = xattr_array_update(h, name, new_విలువ, విలువ_len,
					 ibody_free,
					 0 /* block_free */, old_idx,
					 0 /* in_inode */);
		ఒకవేళ (ret)
			వెళ్ళు బహిర్గతం;
		వెళ్ళు write_బహిర్గతం;
	}

	ఖండం_free = fs->blocksize;
	ఖండం_free -= పరిమాణం(నిర్మాణం ext2_ext_attr_శీర్షిక);
	/* Final null entry. */
	ఖండం_free -= పరిమాణం(__u32);
	ఖండం_free -= space_వాడిన(h->attrs + h->ibody_లెక్క,
				 h->లెక్క - h->ibody_లెక్క);

	ఒకవేళ (ext2fs_has_feature_ea_inode(fs->super) &&
	    విలువ_len > EXT4_XATTR_MIN_LARGE_EA_కొలత(fs->blocksize))
		లోపల_inode = 1;

	ret = xattr_array_update(h, name, new_విలువ, విలువ_len, ibody_free,
				 ఖండం_free, old_idx, లోపల_inode);
	ఒకవేళ (ret == EXT2_ET_EA_NO_SPACE && !లోపల_inode &&
	    ext2fs_has_feature_ea_inode(fs->super))
		ret = xattr_array_update(h, name, new_విలువ, విలువ_len,
			ibody_free, ఖండం_free, old_idx, 1 /* in_inode */);
	ఒకవేళ (ret)
		వెళ్ళు బహిర్గతం;

write_బహిర్గతం:
	ret = ext2fs_xattrs_write(h);
బహిర్గతం:
	ఒకవేళ (inode)
		ext2fs_free_mem(&inode);
	ext2fs_free_mem(&new_విలువ);
	తిరిగి ret;
}

errcode_t ext2fs_xattr_remove(నిర్మాణం ext2_xattr_handle *handle,
			      స్థిరరాశి అక్షరం *కీలకం)
{
	నిర్మాణం ext2_xattr *x;
	నిర్మాణం ext2_xattr *చివర = handle->attrs + handle->లెక్క;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	కొరకు (x = handle->attrs; x < చివర; x++) {
		ఒకవేళ (strcmp(x->name, కీలకం) == 0) {
			ext2fs_free_mem(&x->name);
			ext2fs_free_mem(&x->విలువ);
			ఒకవేళ (x->ea_ino)
				xattr_inode_dec_ref(handle->fs, x->ea_ino);
			memmove(x, x + 1, (చివర - x - 1)*పరిమాణం(*x));
			memset(చివర - 1, 0, పరిమాణం(*చివర));
			ఒకవేళ (x < handle->attrs + handle->ibody_లెక్క)
				handle->ibody_లెక్క--;
			handle->లెక్క--;
			తిరిగి ext2fs_xattrs_write(handle);
		}
	}

	/* no key found, success! */
	తిరిగి 0;
}

errcode_t ext2fs_xattr_remove_all(నిర్మాణం ext2_xattr_handle *handle)
{
	నిర్మాణం ext2_xattr *x;
	నిర్మాణం ext2_xattr *చివర = handle->attrs + handle->లెక్క;

	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	కొరకు (x = handle->attrs; x < చివర; x++) {
		ext2fs_free_mem(&x->name);
		ext2fs_free_mem(&x->విలువ);
		ఒకవేళ (x->ea_ino)
			xattr_inode_dec_ref(handle->fs, x->ea_ino);
	}

	handle->ibody_లెక్క = 0;
	handle->లెక్క = 0;
	తిరిగి ext2fs_xattrs_write(handle);
}

errcode_t ext2fs_xattrs_open(ext2_filsys fs, ext2_ino_t ino,
			     నిర్మాణం ext2_xattr_handle **handle)
{
	నిర్మాణం ext2_xattr_handle *h;
	errcode_t err;

	ఒకవేళ (!ext2fs_has_feature_xattr(fs->super) &&
	    !ext2fs_has_feature_inline_దత్తాంశం(fs->super))
		తిరిగి EXT2_ET_MISSING_EA_FEATURE;

	err = ext2fs_get_memzero(పరిమాణం(*h), &h);
	ఒకవేళ (err)
		తిరిగి err;

	h->magic = EXT2_ET_MAGIC_EA_HANDLE;
	h->సామర్థ్యం = 4;
	err = ext2fs_get_arrayzero(h->సామర్థ్యం, పరిమాణం(నిర్మాణం ext2_xattr),
				   &h->attrs);
	ఒకవేళ (err) {
		ext2fs_free_mem(&h);
		తిరిగి err;
	}
	h->లెక్క = 0;
	h->ino = ino;
	h->fs = fs;
	*handle = h;
	తిరిగి 0;
}

errcode_t ext2fs_xattrs_close(నిర్మాణం ext2_xattr_handle **handle)
{
	నిర్మాణం ext2_xattr_handle *h = *handle;

	EXT2_CHECK_MAGIC(h, EXT2_ET_MAGIC_EA_HANDLE);
	xattrs_free_కీలకాలు(h);
	ext2fs_free_mem(&h->attrs);
	ext2fs_free_mem(handle);
	తిరిగి 0;
}

errcode_t ext2fs_xattrs_లెక్క(నిర్మాణం ext2_xattr_handle *handle, size_t *లెక్క)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	*లెక్క = handle->లెక్క;
	తిరిగి 0;
}

errcode_t ext2fs_xattrs_flags(నిర్మాణం ext2_xattr_handle *handle,
			      గుర్తులేని పూర్ణాంకం *new_flags, గుర్తులేని పూర్ణాంకం *old_flags)
{
	EXT2_CHECK_MAGIC(handle, EXT2_ET_MAGIC_EA_HANDLE);
	ఒకవేళ (old_flags)
		*old_flags = handle->flags;
	ఒకవేళ (new_flags)
		handle->flags = *new_flags;
	తిరిగి 0;
}
