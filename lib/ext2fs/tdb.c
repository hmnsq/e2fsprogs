/*
URL: svn://svnanon.samba.org/samba/branches/SAMBA_4_0/source/lib/tdb/common
Rev: 23590
Last Changed Date: 2007-06-22 13:36:10 -0400 (Fri, 22 Jun 2007)
*/
 /*
   trivial database library - standalone version

   Copyright (C) Andrew Tridgell              1999-2005
   Copyright (C) Jeremy Allison               2000-2006
   Copyright (C) Paul `Rusty' Russell         2000

     ** NOTE! The following LGPL license applies to the tdb
     ** library. This does NOT imply that all of Samba is released
     ** under the LGPL

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_STAND_ALONE
#ವ್ಯಾಖ್ಯಾನಿಸು HAVE_MMAP
#ವ್ಯಾಖ್ಯಾನಿಸು HAVE_STRDUP
#ವ್ಯಾಖ್ಯಾನಿಸು HAVE_SYS_MMAN_H
#ವ್ಯಾಖ್ಯಾನಿಸು HAVE_UTIME_H
#ವ್ಯಾಖ್ಯಾನಿಸು HAVE_UTIME
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __FreeBSD__
#ವ್ಯಾಖ್ಯಾನಿಸು XOPEN_ಮೂಲ 600
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdarg.h>
#ಅನ್ತರ್ಭಾವ <stddef.h>
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYS_SELECT_H
#ಅನ್ತರ್ಭಾವ <sys/select.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <sys/time.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_UTIME_H
#ಅನ್ತರ್ಭಾವ <utime.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/file.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYS_MMAN_H
#ಅನ್ತರ್ಭಾವ <sys/mman.h>
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __GNUC__
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_ATTR(x) __attribute__(x)
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_ATTR(x)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MAP_FILE
#ವ್ಯಾಖ್ಯಾನಿಸು MAP_FILE 0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MAP_FAILED
#ವ್ಯಾಖ್ಯಾನಿಸು MAP_FAILED ((ಶೂನ್ಯ *)-1)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_STRDUP
#ವ್ಯಾಖ್ಯಾನಿಸು strdup rep_strdup
ಸ್ಥಿರ ಅಕ್ಷರ *rep_strdup(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *s)
{
	ಅಕ್ಷರ *ret;
	ಪೂರ್ಣಾಂಕ ಉದ್ದಳತೆ;

	ಆದರೆ (!s)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	ಉದ್ದಳತೆ = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(s);
	ret = ಸ್ಮರಣೆ_ಕೊಡು(ಉದ್ದಳತೆ + 1);
	ಆದರೆ (ret) {
		strncpy(ret, s, ಉದ್ದಳತೆ);
		ret[ಉದ್ದಳತೆ] = '\0';
	}
	ಹಿಂತಿರುಗಿ ret;
}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ PRINTF_ATTRIBUTE
#ಆದರೆ (__GNUC__ >= 3) && (__GNUC_MINOR__ >= 1 )
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#ವ್ಯಾಖ್ಯಾನಿಸು PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು PRINTF_ATTRIBUTE(a1, a2)
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined __STDC__ && defined __STDC_VERSION__ && __STDC_VERSION__ <= 201710L
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ bool;
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "tdb.h"

ಸ್ಥಿರ TDB_ದತ್ತಾಂಶ tdb_null;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ u32
#ವ್ಯಾಖ್ಯಾನಿಸು u32 ಚಿಹ್ನೆರಹಿತ
#ಆದರೆ_ಅಂತ್ಯ

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ u32 tdb_len_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ u32 tdb_off_t;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ offsetof
#ವ್ಯಾಖ್ಯಾನಿಸು offsetof(t,f) ((ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)&((t *)0)->f)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು TDB_MAGIC_FOOD "TDB file\n"
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_ಆವೃತ್ತಿ (0x26011967 + 6)
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_MAGIC (0x26011999U)
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_FREE_MAGIC (~TDB_MAGIC)
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_DEAD_MAGIC (0xFEE1DEAD)
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_RECOVERY_MAGIC (0xf53bc0e7U)
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_ALIGNMENT 4
#ವ್ಯಾಖ್ಯಾನಿಸು MIN_REC_ಗಾತ್ರ (2*ಗಾತ್ರ(ರಚನೆ list_struct) + TDB_ALIGNMENT)
#ವ್ಯಾಖ್ಯಾನಿಸು DEFAULT_HASH_ಗಾತ್ರ 131
#ವ್ಯಾಖ್ಯಾನಿಸು FREELIST_ತುದಿ (ಗಾತ್ರ(ರಚನೆ tdb_ಶೀರ್ಷಿಕೆ))
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_ALIGN(x,a) (((x) + (a)-1) & ~((a)-1))
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_BYTEREV(x) (((((x)&0xff)<<24)|((x)&0xFF00)<<8)|(((x)>>8)&0xFF00)|((x)>>24))
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_DEAD(r) ((r)->magic == TDB_DEAD_MAGIC)
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_BAD_MAGIC(r) ((r)->magic != TDB_MAGIC && !TDB_DEAD(r))
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_HASH_ತುದಿ(hash) (FREELIST_ತುದಿ + (ಪಾತ್ರೆ(hash)+1)*ಗಾತ್ರ(tdb_off_t))
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_HASHTABLE_ಗಾತ್ರ(tdb) ((tdb->ಶೀರ್ಷಿಕೆ.hash_ಗಾತ್ರ+1)*ಗಾತ್ರ(tdb_off_t))
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_ದತ್ತಾಂಶ_ಆರಂಭ(hash_ಗಾತ್ರ) TDB_HASH_ತುದಿ(hash_ಗಾತ್ರ-1)
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_RECOVERY_ತಲೆ offsetof(ರಚನೆ tdb_ಶೀರ್ಷಿಕೆ, recovery_ಆರಂಭ)
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_SEQNUM_OFS    offsetof(ರಚನೆ tdb_ಶೀರ್ಷಿಕೆ, sequence_ಸಂಖ್ಯೆ)
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_PAD_BYTE 0x42
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_PAD_U32  0x42424242

/* NB assumes there is a local variable called "tdb" that is the
 * current context, also takes doubly-parenthesized print-style
 * argument. */
#ವ್ಯಾಖ್ಯಾನಿಸು TDB_LOG(x) tdb->ಲಘುಗಣಕ.log_fn x

/* lock offsets */
#ವ್ಯಾಖ್ಯಾನಿಸು GLOBAL_ಬೀಗ      0
#ವ್ಯಾಖ್ಯಾನಿಸು ಸಕ್ರಿಯ_ಬೀಗ      4
#ವ್ಯಾಖ್ಯಾನಿಸು TRANSACTION_ಬೀಗ 8

/* free memory if the pointer is valid and zero the pointer */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಸುರಕ್ಷಿತ_FREE
#ವ್ಯಾಖ್ಯಾನಿಸು ಸುರಕ್ಷಿತ_FREE(x) ಮಾಡು { ಆದರೆ ((x) != ಖಾಲಿ) {ಬಿಡುಗಡೆ(x); (x)=ಖಾಲಿ;} } ಆಗ(0)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿಸು ಪಾತ್ರೆ(hash) ((hash) % tdb->ಶೀರ್ಷಿಕೆ.hash_ಗಾತ್ರ)

#ವ್ಯಾಖ್ಯಾನಿಸು DOCONV() (tdb->flags & TDB_CONVERT)
#ವ್ಯಾಖ್ಯಾನಿಸು CONVERT(x) (DOCONV() ? tdb_convert(&x, ಗಾತ್ರ(x)) : &x)


/* the body of the database is made of one list_struct for the free space
   plus a separate data list for each hash value */
ರಚನೆ list_struct {
	tdb_off_t ಮುಂದಿನ; /* offset of the next record in the list */
	tdb_len_t rec_len; /* total byte length of record */
	tdb_len_t ಕೀಲಿ_len; /* byte length of key */
	tdb_len_t ದತ್ತಾಂಶ_len; /* byte length of data */
	u32 ತುಂಬಿದ_hash; /* the full 32 bit hash of the key */
	u32 magic;   /* try to catch errors */
	/* the following union is implied:
		union {
			char record[rec_len];
			struct {
				char key[key_len];
				char data[data_len];
			}
			u32 totalsize; (tailer)
		}
	*/
};


/* this is stored at the front of every database */
ರಚನೆ tdb_ಶೀರ್ಷಿಕೆ {
	ಅಕ್ಷರ magic_food[32]; /* for /etc/magic */
	u32 ಆವೃತ್ತಿ; /* version of the code */
	u32 hash_ಗಾತ್ರ; /* number of hash entries */
	tdb_off_t rwlocks; /* obsolete - kept to detect old formats */
	tdb_off_t recovery_ಆರಂಭ; /* offset of transaction recovery region */
	tdb_off_t sequence_ಸಂಖ್ಯೆ; /* used when TDB_SEQNUM is set */
	tdb_off_t reserved[29];
};

ರಚನೆ tdb_ಬೀಗ_type {
	ಪೂರ್ಣಾಂಕ list;
	u32 ಎಣಿಕೆ_2;
	u32 ltype;
};

ರಚನೆ tdb_traverse_ಬೀಗ {
	ರಚನೆ tdb_traverse_ಬೀಗ *ಮುಂದಿನ;
	u32 off;
	u32 hash;
	ಪೂರ್ಣಾಂಕ ಬೀಗ_rw;
};


ರಚನೆ tdb_methods {
	ಪೂರ್ಣಾಂಕ (*tdb_read)(ರಚನೆ tdb_context *, tdb_off_t , ಶೂನ್ಯ *, tdb_len_t , ಪೂರ್ಣಾಂಕ );
	ಪೂರ್ಣಾಂಕ (*tdb_write)(ರಚನೆ tdb_context *, tdb_off_t, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *, tdb_len_t);
	ಶೂನ್ಯ (*ಮುಂದಿನ_hash_chain)(ರಚನೆ tdb_context *, u32 *);
	ಪೂರ್ಣಾಂಕ (*tdb_oob)(ರಚನೆ tdb_context *, tdb_off_t , ಪೂರ್ಣಾಂಕ );
	ಪೂರ್ಣಾಂಕ (*tdb_expand_file)(ರಚನೆ tdb_context *, tdb_off_t , tdb_off_t );
	ಪೂರ್ಣಾಂಕ (*tdb_brlock)(ರಚನೆ tdb_context *, tdb_off_t , ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, size_t);
};

ರಚನೆ tdb_context {
	ಅಕ್ಷರ *name; /* the name of the database */
	ಶೂನ್ಯ *map_ptr; /* where it is currently mapped */
	ಪೂರ್ಣಾಂಕ fd; /* open file descriptor for the database */
	tdb_len_t map_ಗಾತ್ರ; /* how much space has been mapped */
	ಪೂರ್ಣಾಂಕ read_only; /* opened read-only */
	ಪೂರ್ಣಾಂಕ traverse_read; /* read-only traversal */
	ರಚನೆ tdb_ಬೀಗ_type global_ಬೀಗ;
	ಪೂರ್ಣಾಂಕ ಅಂಕೆ_lockrecs;
	ರಚನೆ tdb_ಬೀಗ_type *lockrecs; /* only real locks, all with count>0 */
	ಎಣಿಕೆ TDB_ERROR ecode; /* error code for last tdb error */
	ರಚನೆ tdb_ಶೀರ್ಷಿಕೆ ಶೀರ್ಷಿಕೆ; /* a cached copy of the header */
	u32 flags; /* the flags passed to tdb_open */
	ರಚನೆ tdb_traverse_ಬೀಗ travlocks; /* current traversal locks */
	ರಚನೆ tdb_context *ಮುಂದಿನ; /* all tdbs to avoid multiple opens */
	dev_t device;	/* uniquely identifies this tdb */
	ino_t inode;	/* uniquely identifies this tdb */
	ರಚನೆ tdb_logging_context ಲಘುಗಣಕ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ (*hash_fn)(TDB_ದತ್ತಾಂಶ *ಕೀಲಿ);
	ಪೂರ್ಣಾಂಕ open_flags; /* flags used in the open - needed by reopen */
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ_locks; /* number of chain locks held */
	ಸ್ಥಿರಾಂಕ ರಚನೆ tdb_methods *methods;
	ರಚನೆ tdb_transaction *transaction;
	ಪೂರ್ಣಾಂಕ ಪುಟ_ಗಾತ್ರ;
	ಪೂರ್ಣಾಂಕ max_dead_ದಾಖಲೆಗಳು;
	bool have_transaction_ಬೀಗ;
	tdb_len_t real_map_ಗಾತ್ರ; /* how much space has been mapped */
};


/*
  internal prototypes
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_munmap(ರಚನೆ tdb_context *tdb);
ಸ್ಥಿರ ಶೂನ್ಯ tdb_mmap(ರಚನೆ tdb_context *tdb);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_ಬೀಗ(ರಚನೆ tdb_context *tdb, ಪೂರ್ಣಾಂಕ list, ಪೂರ್ಣಾಂಕ ltype);
ಪೂರ್ಣಾಂಕ tdb_ಬೀಗ_nonblock(ರಚನೆ tdb_context *tdb, ಪೂರ್ಣಾಂಕ list, ಪೂರ್ಣಾಂಕ ltype);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_unlock(ರಚನೆ tdb_context *tdb, ಪೂರ್ಣಾಂಕ list, ಪೂರ್ಣಾಂಕ ltype);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_brlock(ರಚನೆ tdb_context *tdb, tdb_off_t ಸ್ಥಾನಾಂತರ, ಪೂರ್ಣಾಂಕ rw_type, ಪೂರ್ಣಾಂಕ lck_type, ಪೂರ್ಣಾಂಕ probe, size_t len);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_transaction_ಬೀಗ(ರಚನೆ tdb_context *tdb, ಪೂರ್ಣಾಂಕ ltype);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_transaction_unlock(ರಚನೆ tdb_context *tdb);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_brlock_upgrade(ರಚನೆ tdb_context *tdb, tdb_off_t ಸ್ಥಾನಾಂತರ, size_t len);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_write_ಬೀಗ_ದಾಖಲೆ(ರಚನೆ tdb_context *tdb, tdb_off_t off);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_write_unlock_ದಾಖಲೆ(ರಚನೆ tdb_context *tdb, tdb_off_t off);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_ofs_read(ರಚನೆ tdb_context *tdb, tdb_off_t ಸ್ಥಾನಾಂತರ, tdb_off_t *d);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_ofs_write(ರಚನೆ tdb_context *tdb, tdb_off_t ಸ್ಥಾನಾಂತರ, tdb_off_t *d);
ಸ್ಥಿರ ಶೂನ್ಯ *tdb_convert(ಶೂನ್ಯ *buf, u32 ಗಾತ್ರ_2);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_free(ರಚನೆ tdb_context *tdb, tdb_off_t ಸ್ಥಾನಾಂತರ, ರಚನೆ list_struct *rec);
ಸ್ಥಿರ tdb_off_t tdb_allocate(ರಚನೆ tdb_context *tdb, tdb_len_t ಉದ್ದಳತೆ, ರಚನೆ list_struct *rec);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_ofs_read(ರಚನೆ tdb_context *tdb, tdb_off_t ಸ್ಥಾನಾಂತರ, tdb_off_t *d);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_ofs_write(ರಚನೆ tdb_context *tdb, tdb_off_t ಸ್ಥಾನಾಂತರ, tdb_off_t *d);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_ಬೀಗ_ದಾಖಲೆ(ರಚನೆ tdb_context *tdb, tdb_off_t off);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_unlock_ದಾಖಲೆ(ರಚನೆ tdb_context *tdb, tdb_off_t off);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_rec_read(ರಚನೆ tdb_context *tdb, tdb_off_t ಸ್ಥಾನಾಂತರ, ರಚನೆ list_struct *rec);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_rec_write(ರಚನೆ tdb_context *tdb, tdb_off_t ಸ್ಥಾನಾಂತರ, ರಚನೆ list_struct *rec);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_do_delete(ರಚನೆ tdb_context *tdb, tdb_off_t rec_ptr, ರಚನೆ list_struct *rec);
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *tdb_alloc_read(ರಚನೆ tdb_context *tdb, tdb_off_t ಸ್ಥಾನಾಂತರ, tdb_len_t len);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_parse_ದತ್ತಾಂಶ(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ,
		   tdb_off_t ಸ್ಥಾನಾಂತರ, tdb_len_t len,
		   ಪೂರ್ಣಾಂಕ (*parser)(TDB_ದತ್ತಾಂಶ ಕೀಲಿ, TDB_ದತ್ತಾಂಶ ದತ್ತಾಂಶ,
				 ಶೂನ್ಯ *ಖಾಸಗಿ_ದತ್ತಾಂಶ),
		   ಶೂನ್ಯ *ಖಾಸಗಿ_ದತ್ತಾಂಶ);
ಸ್ಥಿರ tdb_off_t tdb_ಕಂಡುಹಿಡಿ_ಬೀಗ_hash(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ, u32 hash, ಪೂರ್ಣಾಂಕ locktype,
			   ರಚನೆ list_struct *rec);
ಸ್ಥಿರ ಶೂನ್ಯ tdb_io_init(ರಚನೆ tdb_context *tdb);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_expand(ರಚನೆ tdb_context *tdb, tdb_off_t ಗಾತ್ರ_2);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_rec_free_read(ರಚನೆ tdb_context *tdb, tdb_off_t off,
		      ರಚನೆ list_struct *rec);


/* file: error.c */

ಎಣಿಕೆ TDB_ERROR tdb_error(ರಚನೆ tdb_context *tdb)
{
	ಹಿಂತಿರುಗಿ tdb->ecode;
}

ಸ್ಥಿರ ರಚನೆ tdb_errname {
	ಎಣಿಕೆ TDB_ERROR ecode; ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *estring;
} emap[] = { {TDB_SUCCESS, "Success"},
	     {TDB_ERR_CORRUPT, "Corrupt database"},
	     {TDB_ERR_IO, "IO Error"},
	     {TDB_ERR_ಬೀಗ, "Locking error"},
	     {TDB_ERR_OOM, "Out of memory"},
	     {TDB_ERR_EXISTS, "Record exists"},
	     {TDB_ERR_NOLOCK, "Lock exists on other keys"},
	     {TDB_ERR_EINVAL, "Invalid parameter"},
	     {TDB_ERR_NOEXIST, "Record does not exist"},
	     {TDB_ERR_RDONLY, "write not permitted"} };

/* Error string for the last tdb error */
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *tdb_errorstr(ರಚನೆ tdb_context *tdb)
{
	u32 i;
	ಗಾಗಿ (i = 0; i < ಗಾತ್ರ(emap) / ಗಾತ್ರ(ರಚನೆ tdb_errname); i++)
		ಆದರೆ (tdb->ecode == emap[i].ecode)
			ಹಿಂತಿರುಗಿ emap[i].estring;
	ಹಿಂತಿರುಗಿ "Invalid error code";
}

/* file: lock.c */

#ವ್ಯಾಖ್ಯಾನಿಸು TDB_MARK_ಬೀಗ 0x80000000

/* a byte range locking function - return 0 on success
   this functions locks/unlocks 1 byte at the specified offset.

   On error, errno is also set so that errors are passed back properly
   through tdb_open().

   note that a len of zero means lock to end of file
*/
ಪೂರ್ಣಾಂಕ tdb_brlock(ರಚನೆ tdb_context *tdb, tdb_off_t ಸ್ಥಾನಾಂತರ,
	       ಪೂರ್ಣಾಂಕ rw_type, ಪೂರ್ಣಾಂಕ lck_type, ಪೂರ್ಣಾಂಕ probe, size_t len)
{
	ರಚನೆ flock fl;
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (tdb->flags & TDB_NOLOCK) {
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ ((rw_type == F_WRLCK) && (tdb->read_only || tdb->traverse_read)) {
		tdb->ecode = TDB_ERR_RDONLY;
		ಹಿಂತಿರುಗಿ -1;
	}

	fl.l_type = rw_type;
	fl.l_whence = SEEK_SET;
	fl.l_ಆರಂಭ = ಸ್ಥಾನಾಂತರ;
	fl.l_len = len;
	fl.l_pid = 0;

	ಮಾಡು {
		ret = fcntl(tdb->fd,lck_type,&fl);
	} ಆಗ (ret == -1 && errno == EINTR);

	ಆದರೆ (ret == -1) {
		/* Generic lock error. errno set by fcntl.
		 * EAGAIN is an expected return from non-blocking
		 * locks. */
		ಆದರೆ (!probe && lck_type != F_SETLK) {
			/* Ensure error code is set for log fun to examine. */
			tdb->ecode = TDB_ERR_ಬೀಗ;
			TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock failed (fd=%d) at offset %d rw_type=%d lck_type=%d len=%d\n",
				 tdb->fd, ಸ್ಥಾನಾಂತರ, rw_type, lck_type, (ಪೂರ್ಣಾಂಕ)len));
		}
		ಹಿಂತಿರುಗಿ TDB_ERRCODE(TDB_ERR_ಬೀಗ, -1);
	}
	ಹಿಂತಿರುಗಿ 0;
}


/*
  upgrade a read lock to a write lock. This needs to be handled in a
  special way as some OSes (such as solaris) have too conservative
  deadlock detection and claim a deadlock when progress can be
  made. For those OSes we may loop for a while.
*/
ಪೂರ್ಣಾಂಕ tdb_brlock_upgrade(ರಚನೆ tdb_context *tdb, tdb_off_t ಸ್ಥಾನಾಂತರ, size_t len)
{
	ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 1000;
	ಆಗ (ಎಣಿಕೆ_2--) {
		ರಚನೆ timeval tv;
		ಆದರೆ (tdb_brlock(tdb, ಸ್ಥಾನಾಂತರ, F_WRLCK, F_SETLKW, 1, len) == 0) {
			ಹಿಂತಿರುಗಿ 0;
		}
		ಆದರೆ (errno != EDEADLK) {
			ನಿಲ್ಲಿಸು;
		}
		/* sleep for as short a time as we can - more portable than usleep() */
		tv.tv_sec = 0;
		tv.tv_usec = 1;
		select(0, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, &tv);
	}
	TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock_upgrade failed at offset %d\n", ಸ್ಥಾನಾಂತರ));
	ಹಿಂತಿರುಗಿ -1;
}


/* lock a list in the database. list -1 is the alloc list */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_ಬೀಗ_2(ರಚನೆ tdb_context *tdb, ಪೂರ್ಣಾಂಕ list, ಪೂರ್ಣಾಂಕ ltype, ಪೂರ್ಣಾಂಕ op)
{
	ರಚನೆ tdb_ಬೀಗ_type *new_lck;
	ಪೂರ್ಣಾಂಕ i;
	bool mark_ಬೀಗ = ((ltype & TDB_MARK_ಬೀಗ) == TDB_MARK_ಬೀಗ);

	ltype &= ~TDB_MARK_ಬೀಗ;

	/* a global lock allows us to avoid per chain locks */
	ಆದರೆ (tdb->global_ಬೀಗ.ಎಣಿಕೆ_2 &&
	    ((u32)ltype == tdb->global_ಬೀಗ.ltype || ltype == F_RDLCK)) {
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (tdb->global_ಬೀಗ.ಎಣಿಕೆ_2) {
		ಹಿಂತಿರುಗಿ TDB_ERRCODE(TDB_ERR_ಬೀಗ, -1);
	}

	ಆದರೆ (list < -1 || list >= (ಪೂರ್ಣಾಂಕ)tdb->ಶೀರ್ಷಿಕೆ.hash_ಗಾತ್ರ) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_lock: invalid list %d for ltype=%d\n",
			   list, ltype));
		ಹಿಂತಿರುಗಿ -1;
	}
	ಆದರೆ (tdb->flags & TDB_NOLOCK)
		ಹಿಂತಿರುಗಿ 0;

	ಗಾಗಿ (i=0; i<tdb->ಅಂಕೆ_lockrecs; i++) {
		ಆದರೆ (tdb->lockrecs[i].list == list) {
			ಆದರೆ (tdb->lockrecs[i].ಎಣಿಕೆ_2 == 0) {
				/*
				 * Can't happen, see tdb_unlock(). It should
				 * be an assert.
				 */
				TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lock: "
					 "lck->count == 0 for list %d", list));
			}
			/*
			 * Just increment the in-memory struct, posix locks
			 * don't stack.
			 */
			tdb->lockrecs[i].ಎಣಿಕೆ_2++;
			ಹಿಂತಿರುಗಿ 0;
		}
	}

	new_lck = (ರಚನೆ tdb_ಬೀಗ_type *)realloc(
		tdb->lockrecs,
		ಗಾತ್ರ(*tdb->lockrecs) * (tdb->ಅಂಕೆ_lockrecs+1));
	ಆದರೆ (new_lck == ಖಾಲಿ) {
		errno = ENOMEM;
		ಹಿಂತಿರುಗಿ -1;
	}
	tdb->lockrecs = new_lck;

	/* Since fcntl locks don't nest, we do a lock for the first one,
	   and simply bump the count for future ones */
	ಆದರೆ (!mark_ಬೀಗ &&
	    tdb->methods->tdb_brlock(tdb,FREELIST_ತುದಿ+4*list, ltype, op,
				     0, 1)) {
		ಹಿಂತಿರುಗಿ -1;
	}

	tdb->ಅಂಕೆ_locks++;

	tdb->lockrecs[tdb->ಅಂಕೆ_lockrecs].list = list;
	tdb->lockrecs[tdb->ಅಂಕೆ_lockrecs].ಎಣಿಕೆ_2 = 1;
	tdb->lockrecs[tdb->ಅಂಕೆ_lockrecs].ltype = ltype;
	tdb->ಅಂಕೆ_lockrecs += 1;

	ಹಿಂತಿರುಗಿ 0;
}

/* lock a list in the database. list -1 is the alloc list */
ಪೂರ್ಣಾಂಕ tdb_ಬೀಗ(ರಚನೆ tdb_context *tdb, ಪೂರ್ಣಾಂಕ list, ಪೂರ್ಣಾಂಕ ltype)
{
	ಪೂರ್ಣಾಂಕ ret;
	ret = tdb_ಬೀಗ_2(tdb, list, ltype, F_SETLKW);
	ಆದರೆ (ret) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lock failed on list %d "
			 "ltype=%d (%s)\n",  list, ltype, strerror(errno)));
	}
	ಹಿಂತಿರುಗಿ ret;
}

/* lock a list in the database. list -1 is the alloc list. non-blocking lock */
ಪೂರ್ಣಾಂಕ tdb_ಬೀಗ_nonblock(ರಚನೆ tdb_context *tdb, ಪೂರ್ಣಾಂಕ list, ಪೂರ್ಣಾಂಕ ltype)
{
	ಹಿಂತಿರುಗಿ tdb_ಬೀಗ_2(tdb, list, ltype, F_SETLK);
}


/* unlock the database: returns void because it's too late for errors. */
	/* changed to return int it may be interesting to know there
	   has been an error  --simo */
ಪೂರ್ಣಾಂಕ tdb_unlock(ರಚನೆ tdb_context *tdb, ಪೂರ್ಣಾಂಕ list, ಪೂರ್ಣಾಂಕ ltype)
{
	ಪೂರ್ಣಾಂಕ ret = -1;
	ಪೂರ್ಣಾಂಕ i;
	ರಚನೆ tdb_ಬೀಗ_type *lck = ಖಾಲಿ;
	bool mark_ಬೀಗ = ((ltype & TDB_MARK_ಬೀಗ) == TDB_MARK_ಬೀಗ);

	ltype &= ~TDB_MARK_ಬೀಗ;

	/* a global lock allows us to avoid per chain locks */
	ಆದರೆ (tdb->global_ಬೀಗ.ಎಣಿಕೆ_2 &&
	    ((u32)ltype == tdb->global_ಬೀಗ.ltype || ltype == F_RDLCK)) {
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (tdb->global_ಬೀಗ.ಎಣಿಕೆ_2) {
		ಹಿಂತಿರುಗಿ TDB_ERRCODE(TDB_ERR_ಬೀಗ, -1);
	}

	ಆದರೆ (tdb->flags & TDB_NOLOCK)
		ಹಿಂತಿರುಗಿ 0;

	/* Sanity checks */
	ಆದರೆ (list < -1 || list >= (ಪೂರ್ಣಾಂಕ)tdb->ಶೀರ್ಷಿಕೆ.hash_ಗಾತ್ರ) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: list %d invalid (%d)\n", list, tdb->ಶೀರ್ಷಿಕೆ.hash_ಗಾತ್ರ));
		ಹಿಂತಿರುಗಿ ret;
	}

	ಗಾಗಿ (i=0; i<tdb->ಅಂಕೆ_lockrecs; i++) {
		ಆದರೆ (tdb->lockrecs[i].list == list) {
			lck = &tdb->lockrecs[i];
			ನಿಲ್ಲಿಸು;
		}
	}

	ಆದರೆ ((lck == ಖಾಲಿ) || (lck->ಎಣಿಕೆ_2 == 0)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: count is 0\n"));
		ಹಿಂತಿರುಗಿ -1;
	}

	ಆದರೆ (lck->ಎಣಿಕೆ_2 > 1) {
		lck->ಎಣಿಕೆ_2--;
		ಹಿಂತಿರುಗಿ 0;
	}

	/*
	 * This lock has count==1 left, so we need to unlock it in the
	 * kernel. We don't bother with decrementing the in-memory array
	 * element, we're about to overwrite it with the last array element
	 * anyway.
	 */

	ಆದರೆ (mark_ಬೀಗ) {
		ret = 0;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ret = tdb->methods->tdb_brlock(tdb, FREELIST_ತುದಿ+4*list, F_UNLCK,
					       F_SETLKW, 0, 1);
	}
	tdb->ಅಂಕೆ_locks--;

	/*
	 * Shrink the array by overwriting the element just unlocked with the
	 * last array element.
	 */

	ಆದರೆ (tdb->ಅಂಕೆ_lockrecs > 1) {
		*lck = tdb->lockrecs[tdb->ಅಂಕೆ_lockrecs-1];
	}
	tdb->ಅಂಕೆ_lockrecs -= 1;

	/*
	 * We don't bother with realloc when the array shrinks, but if we have
	 * a completely idle tdb we should get rid of the locked array.
	 */

	ಆದರೆ (tdb->ಅಂಕೆ_lockrecs == 0) {
		ಸುರಕ್ಷಿತ_FREE(tdb->lockrecs);
	}

	ಆದರೆ (ret)
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: An error occurred unlocking!\n"));
	ಹಿಂತಿರುಗಿ ret;
}

/*
  get the transaction lock
 */
ಪೂರ್ಣಾಂಕ tdb_transaction_ಬೀಗ(ರಚನೆ tdb_context *tdb, ಪೂರ್ಣಾಂಕ ltype)
{
	ಆದರೆ (tdb->have_transaction_ಬೀಗ || tdb->global_ಬೀಗ.ಎಣಿಕೆ_2) {
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (tdb->methods->tdb_brlock(tdb, TRANSACTION_ಬೀಗ, ltype,
				     F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_lock: failed to get transaction lock\n"));
		tdb->ecode = TDB_ERR_ಬೀಗ;
		ಹಿಂತಿರುಗಿ -1;
	}
	tdb->have_transaction_ಬೀಗ = 1;
	ಹಿಂತಿರುಗಿ 0;
}

/*
  release the transaction lock
 */
ಪೂರ್ಣಾಂಕ tdb_transaction_unlock(ರಚನೆ tdb_context *tdb)
{
	ಪೂರ್ಣಾಂಕ ret;
	ಆದರೆ (!tdb->have_transaction_ಬೀಗ) {
		ಹಿಂತಿರುಗಿ 0;
	}
	ret = tdb->methods->tdb_brlock(tdb, TRANSACTION_ಬೀಗ, F_UNLCK, F_SETLKW, 0, 1);
	ಆದರೆ (ret == 0) {
		tdb->have_transaction_ಬೀಗ = 0;
	}
	ಹಿಂತಿರುಗಿ ret;
}




/* lock/unlock entire database */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ _tdb_lockall(ರಚನೆ tdb_context *tdb, ಪೂರ್ಣಾಂಕ ltype, ಪೂರ್ಣಾಂಕ op)
{
	bool mark_ಬೀಗ = ((ltype & TDB_MARK_ಬೀಗ) == TDB_MARK_ಬೀಗ);

	ltype &= ~TDB_MARK_ಬೀಗ;

	/* There are no locks on read-only dbs */
	ಆದರೆ (tdb->read_only || tdb->traverse_read)
		ಹಿಂತಿರುಗಿ TDB_ERRCODE(TDB_ERR_ಬೀಗ, -1);

	ಆದರೆ (tdb->global_ಬೀಗ.ಎಣಿಕೆ_2 && tdb->global_ಬೀಗ.ltype == (u32)ltype) {
		tdb->global_ಬೀಗ.ಎಣಿಕೆ_2++;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (tdb->global_ಬೀಗ.ಎಣಿಕೆ_2) {
		/* a global lock of a different type exists */
		ಹಿಂತಿರುಗಿ TDB_ERRCODE(TDB_ERR_ಬೀಗ, -1);
	}

	ಆದರೆ (tdb->ಅಂಕೆ_locks != 0) {
		/* can't combine global and chain locks */
		ಹಿಂತಿರುಗಿ TDB_ERRCODE(TDB_ERR_ಬೀಗ, -1);
	}

	ಆದರೆ (!mark_ಬೀಗ &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_ತುದಿ, ltype, op,
				     0, 4*tdb->ಶೀರ್ಷಿಕೆ.hash_ಗಾತ್ರ)) {
		ಆದರೆ (op == F_SETLKW) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lockall failed (%s)\n", strerror(errno)));
		}
		ಹಿಂತಿರುಗಿ -1;
	}

	tdb->global_ಬೀಗ.ಎಣಿಕೆ_2 = 1;
	tdb->global_ಬೀಗ.ltype = ltype;

	ಹಿಂತಿರುಗಿ 0;
}



/* unlock entire db */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ _tdb_unlockall(ರಚನೆ tdb_context *tdb, ಪೂರ್ಣಾಂಕ ltype)
{
	bool mark_ಬೀಗ = ((ltype & TDB_MARK_ಬೀಗ) == TDB_MARK_ಬೀಗ);

	ltype &= ~TDB_MARK_ಬೀಗ;

	/* There are no locks on read-only dbs */
	ಆದರೆ (tdb->read_only || tdb->traverse_read) {
		ಹಿಂತಿರುಗಿ TDB_ERRCODE(TDB_ERR_ಬೀಗ, -1);
	}

	ಆದರೆ (tdb->global_ಬೀಗ.ltype != (u32)ltype ||
	    tdb->global_ಬೀಗ.ಎಣಿಕೆ_2 == 0) {
		ಹಿಂತಿರುಗಿ TDB_ERRCODE(TDB_ERR_ಬೀಗ, -1);
	}

	ಆದರೆ (tdb->global_ಬೀಗ.ಎಣಿಕೆ_2 > 1) {
		tdb->global_ಬೀಗ.ಎಣಿಕೆ_2--;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (!mark_ಬೀಗ &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_ತುದಿ, F_UNLCK, F_SETLKW,
				     0, 4*tdb->ಶೀರ್ಷಿಕೆ.hash_ಗಾತ್ರ)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlockall failed (%s)\n", strerror(errno)));
		ಹಿಂತಿರುಗಿ -1;
	}

	tdb->global_ಬೀಗ.ಎಣಿಕೆ_2 = 0;
	tdb->global_ಬೀಗ.ltype = 0;

	ಹಿಂತಿರುಗಿ 0;
}

/* lock entire database with write lock */
ಪೂರ್ಣಾಂಕ tdb_lockall(ರಚನೆ tdb_context *tdb)
{
	ಹಿಂತಿರುಗಿ _tdb_lockall(tdb, F_WRLCK, F_SETLKW);
}

/* lock entire database with write lock - mark only */
ಪೂರ್ಣಾಂಕ tdb_lockall_mark(ರಚನೆ tdb_context *tdb)
{
	ಹಿಂತಿರುಗಿ _tdb_lockall(tdb, F_WRLCK | TDB_MARK_ಬೀಗ, F_SETLKW);
}

/* unlock entire database with write lock - unmark only */
ಪೂರ್ಣಾಂಕ tdb_lockall_unmark(ರಚನೆ tdb_context *tdb)
{
	ಹಿಂತಿರುಗಿ _tdb_unlockall(tdb, F_WRLCK | TDB_MARK_ಬೀಗ);
}

/* lock entire database with write lock - nonblocking variant */
ಪೂರ್ಣಾಂಕ tdb_lockall_nonblock(ರಚನೆ tdb_context *tdb)
{
	ಹಿಂತಿರುಗಿ _tdb_lockall(tdb, F_WRLCK, F_SETLK);
}

/* unlock entire database with write lock */
ಪೂರ್ಣಾಂಕ tdb_unlockall(ರಚನೆ tdb_context *tdb)
{
	ಹಿಂತಿರುಗಿ _tdb_unlockall(tdb, F_WRLCK);
}

/* lock entire database with read lock */
ಪೂರ್ಣಾಂಕ tdb_lockall_read(ರಚನೆ tdb_context *tdb)
{
	ಹಿಂತಿರುಗಿ _tdb_lockall(tdb, F_RDLCK, F_SETLKW);
}

/* lock entire database with read lock - nonblock variant */
ಪೂರ್ಣಾಂಕ tdb_lockall_read_nonblock(ರಚನೆ tdb_context *tdb)
{
	ಹಿಂತಿರುಗಿ _tdb_lockall(tdb, F_RDLCK, F_SETLK);
}

/* unlock entire database with read lock */
ಪೂರ್ಣಾಂಕ tdb_unlockall_read(ರಚನೆ tdb_context *tdb)
{
	ಹಿಂತಿರುಗಿ _tdb_unlockall(tdb, F_RDLCK);
}

/* lock/unlock one hash chain. This is meant to be used to reduce
   contention - it cannot guarantee how many records will be locked */
ಪೂರ್ಣಾಂಕ tdb_chainlock(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ)
{
	ಹಿಂತಿರುಗಿ tdb_ಬೀಗ(tdb, ಪಾತ್ರೆ(tdb->hash_fn(&ಕೀಲಿ)), F_WRLCK);
}

/* lock/unlock one hash chain, non-blocking. This is meant to be used
   to reduce contention - it cannot guarantee how many records will be
   locked */
ಪೂರ್ಣಾಂಕ tdb_chainlock_nonblock(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ)
{
	ಹಿಂತಿರುಗಿ tdb_ಬೀಗ_nonblock(tdb, ಪಾತ್ರೆ(tdb->hash_fn(&ಕೀಲಿ)), F_WRLCK);
}

/* mark a chain as locked without actually locking it. Warning! use with great caution! */
ಪೂರ್ಣಾಂಕ tdb_chainlock_mark(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ)
{
	ಹಿಂತಿರುಗಿ tdb_ಬೀಗ(tdb, ಪಾತ್ರೆ(tdb->hash_fn(&ಕೀಲಿ)), F_WRLCK | TDB_MARK_ಬೀಗ);
}

/* unmark a chain as locked without actually locking it. Warning! use with great caution! */
ಪೂರ್ಣಾಂಕ tdb_chainlock_unmark(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ)
{
	ಹಿಂತಿರುಗಿ tdb_unlock(tdb, ಪಾತ್ರೆ(tdb->hash_fn(&ಕೀಲಿ)), F_WRLCK | TDB_MARK_ಬೀಗ);
}

ಪೂರ್ಣಾಂಕ tdb_chainunlock(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ)
{
	ಹಿಂತಿರುಗಿ tdb_unlock(tdb, ಪಾತ್ರೆ(tdb->hash_fn(&ಕೀಲಿ)), F_WRLCK);
}

ಪೂರ್ಣಾಂಕ tdb_chainlock_read(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ)
{
	ಹಿಂತಿರುಗಿ tdb_ಬೀಗ(tdb, ಪಾತ್ರೆ(tdb->hash_fn(&ಕೀಲಿ)), F_RDLCK);
}

ಪೂರ್ಣಾಂಕ tdb_chainunlock_read(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ)
{
	ಹಿಂತಿರುಗಿ tdb_unlock(tdb, ಪಾತ್ರೆ(tdb->hash_fn(&ಕೀಲಿ)), F_RDLCK);
}



/* record lock stops delete underneath */
ಪೂರ್ಣಾಂಕ tdb_ಬೀಗ_ದಾಖಲೆ(ರಚನೆ tdb_context *tdb, tdb_off_t off)
{
	ಹಿಂತಿರುಗಿ off ? tdb->methods->tdb_brlock(tdb, off, F_RDLCK, F_SETLKW, 0, 1) : 0;
}

/*
  Write locks override our own fcntl readlocks, so check it here.
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
ಪೂರ್ಣಾಂಕ tdb_write_ಬೀಗ_ದಾಖಲೆ(ರಚನೆ tdb_context *tdb, tdb_off_t off)
{
	ರಚನೆ tdb_traverse_ಬೀಗ *i;
	ಗಾಗಿ (i = &tdb->travlocks; i; i = i->ಮುಂದಿನ)
		ಆದರೆ (i->off == off)
			ಹಿಂತಿರುಗಿ -1;
	ಹಿಂತಿರುಗಿ tdb->methods->tdb_brlock(tdb, off, F_WRLCK, F_SETLK, 1, 1);
}

/*
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
ಪೂರ್ಣಾಂಕ tdb_write_unlock_ದಾಖಲೆ(ರಚನೆ tdb_context *tdb, tdb_off_t off)
{
	ಹಿಂತಿರುಗಿ tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLK, 0, 1);
}

/* fcntl locks don't stack: avoid unlocking someone else's */
ಪೂರ್ಣಾಂಕ tdb_unlock_ದಾಖಲೆ(ರಚನೆ tdb_context *tdb, tdb_off_t off)
{
	ರಚನೆ tdb_traverse_ಬೀಗ *i;
	u32 ಎಣಿಕೆ_2 = 0;

	ಆದರೆ (off == 0)
		ಹಿಂತಿರುಗಿ 0;
	ಗಾಗಿ (i = &tdb->travlocks; i; i = i->ಮುಂದಿನ)
		ಆದರೆ (i->off == off)
			ಎಣಿಕೆ_2++;
	ಹಿಂತಿರುಗಿ (ಎಣಿಕೆ_2 == 1 ? tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLKW, 0, 1) : 0);
}

/* file: io.c */

/* check for an out of bounds access - if it is out of bounds then
   see if the database has been expanded by someone else and expand
   if necessary
   note that "len" is the minimum length needed for the db
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_oob(ರಚನೆ tdb_context *tdb, tdb_off_t len, ಪೂರ್ಣಾಂಕ probe)
{
	ರಚನೆ stat st;
	ಆದರೆ (len <= tdb->map_ಗಾತ್ರ)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (tdb->flags & TDB_ಆಂತರಿಕ) {
		ಆದರೆ (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond internal malloc size %d\n",
				 (ಪೂರ್ಣಾಂಕ)len, (ಪೂರ್ಣಾಂಕ)tdb->map_ಗಾತ್ರ));
		}
		ಹಿಂತಿರುಗಿ TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	ಆದರೆ (fstat(tdb->fd, &st) == -1) {
		ಹಿಂತಿರುಗಿ TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	ಆದರೆ (st.st_ಗಾತ್ರ < (off_t)len) {
		ಆದರೆ (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond eof at %d\n",
				 (ಪೂರ್ಣಾಂಕ)len, (ಪೂರ್ಣಾಂಕ)st.st_ಗಾತ್ರ));
		}
		ಹಿಂತಿರುಗಿ TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	/* Unmap, update size, remap */
	ಆದರೆ (tdb_munmap(tdb) == -1)
		ಹಿಂತಿರುಗಿ TDB_ERRCODE(TDB_ERR_IO, -1);
	tdb->map_ಗಾತ್ರ = st.st_ಗಾತ್ರ;
	tdb_mmap(tdb);
	ಹಿಂತಿರುಗಿ 0;
}

/* write a lump of data at a specified offset */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_write(ರಚನೆ tdb_context *tdb, tdb_off_t off,
		     ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf, tdb_len_t len)
{
	ಆದರೆ (len == 0) {
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		ಹಿಂತಿರುಗಿ -1;
	}

	ಆದರೆ (tdb->methods->tdb_oob(tdb, off + len, 0) != 0)
		ಹಿಂತಿರುಗಿ -1;

	ಆದರೆ (tdb->map_ptr) {
		ಸ್ಮರಣೆ_ನಕಲು(off + (ಅಕ್ಷರ *)tdb->map_ptr, buf, len);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (pwrite(tdb->fd, buf, len, off) != (ssize_t)len) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_write failed at %d len=%d (%s)\n",
			   off, len, strerror(errno)));
		ಹಿಂತಿರುಗಿ TDB_ERRCODE(TDB_ERR_IO, -1);
	}
	ಹಿಂತಿರುಗಿ 0;
}

/* Endian conversion: we only ever deal with 4 byte quantities */
ಶೂನ್ಯ *tdb_convert(ಶೂನ್ಯ *buf, u32 ಗಾತ್ರ_2)
{
	u32 i, *p = (u32 *)buf;
	ಗಾಗಿ (i = 0; i < ಗಾತ್ರ_2 / 4; i++)
		p[i] = TDB_BYTEREV(p[i]);
	ಹಿಂತಿರುಗಿ buf;
}


/* read a lump of data at a specified offset, maybe convert */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_read(ರಚನೆ tdb_context *tdb, tdb_off_t off, ಶೂನ್ಯ *buf,
		    tdb_len_t len, ಪೂರ್ಣಾಂಕ cv)
{
	ಆದರೆ (tdb->methods->tdb_oob(tdb, off + len, 0) != 0) {
		ಹಿಂತಿರುಗಿ -1;
	}

	ಆದರೆ (tdb->map_ptr) {
		ಸ್ಮರಣೆ_ನಕಲು(buf, off + (ಅಕ್ಷರ *)tdb->map_ptr, len);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ssize_t ret = pread(tdb->fd, buf, len, off);
		ಆದರೆ (ret != (ssize_t)len) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_read failed at %d "
				 "len=%d ret=%d (%s) map_size=%d\n",
				 (ಪೂರ್ಣಾಂಕ)off, (ಪೂರ್ಣಾಂಕ)len, (ಪೂರ್ಣಾಂಕ)ret, strerror(errno),
				 (ಪೂರ್ಣಾಂಕ)tdb->map_ಗಾತ್ರ));
			ಹಿಂತಿರುಗಿ TDB_ERRCODE(TDB_ERR_IO, -1);
		}
	}
	ಆದರೆ (cv) {
		tdb_convert(buf, len);
	}
	ಹಿಂತಿರುಗಿ 0;
}



/*
  do an unlocked scan of the hash table heads to find the next non-zero head. The value
  will then be confirmed with the lock held
*/
ಸ್ಥಿರ ಶೂನ್ಯ tdb_ಮುಂದಿನ_hash_chain(ರಚನೆ tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	ಆದರೆ (tdb->map_ptr) {
		ಗಾಗಿ (;h < tdb->ಶೀರ್ಷಿಕೆ.hash_ಗಾತ್ರ;h++) {
			ಆದರೆ (0 != *(u32 *)(TDB_HASH_ತುದಿ(h) + (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)tdb->map_ptr)) {
				ನಿಲ್ಲಿಸು;
			}
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		u32 off=0;
		ಗಾಗಿ (;h < tdb->ಶೀರ್ಷಿಕೆ.hash_ಗಾತ್ರ;h++) {
			ಆದರೆ (tdb_ofs_read(tdb, TDB_HASH_ತುದಿ(h), &off) != 0 || off != 0) {
				ನಿಲ್ಲಿಸು;
			}
		}
	}
	(*chain) = h;
}


ಪೂರ್ಣಾಂಕ tdb_munmap(ರಚನೆ tdb_context *tdb)
{
	ಆದರೆ (tdb->flags & TDB_ಆಂತರಿಕ)
		ಹಿಂತಿರುಗಿ 0;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_MMAP
	ಆದರೆ (tdb->map_ptr) {
		ಪೂರ್ಣಾಂಕ ret = munmap(tdb->map_ptr, tdb->real_map_ಗಾತ್ರ);
		ಆದರೆ (ret != 0)
			ಹಿಂತಿರುಗಿ ret;
		tdb->real_map_ಗಾತ್ರ = 0;
	}
#ಆದರೆ_ಅಂತ್ಯ
	tdb->map_ptr = ಖಾಲಿ;
	ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ tdb_mmap(ರಚನೆ tdb_context *tdb)
{
	ಆದರೆ (tdb->flags & TDB_ಆಂತರಿಕ)
		ಹಿಂತಿರುಗಿ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_MMAP
	ಆದರೆ (!(tdb->flags & TDB_NOMMAP)) {
		tdb->map_ptr = mmap(ಖಾಲಿ, tdb->map_ಗಾತ್ರ,
				    PROT_READ|(tdb->read_only? 0:PROT_WRITE),
				    MAP_SHARED|MAP_FILE, tdb->fd, 0);

		/*
		 * NB. When mmap fails it returns MAP_FAILED *NOT* NULL !!!!
		 */

		ಆದರೆ (tdb->map_ptr == MAP_FAILED) {
			tdb->real_map_ಗಾತ್ರ = 0;
			tdb->map_ptr = ಖಾಲಿ;
			TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_mmap failed for size %d (%s)\n",
				 tdb->map_ಗಾತ್ರ, strerror(errno)));
		}
		tdb->real_map_ಗಾತ್ರ = tdb->map_ಗಾತ್ರ;
	} ಇಲ್ಲದಿದ್ದರೆ {
		tdb->map_ptr = ಖಾಲಿ;
	}
#ಇಲ್ಲದಿದ್ದರೆ
	tdb->map_ptr = ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ
}

/* expand a file.  we prefer to use ftruncate, as that is what posix
  says to use for mmap expansion */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_expand_file(ರಚನೆ tdb_context *tdb, tdb_off_t ಗಾತ್ರ_2, tdb_off_t addition)
{
	ಅಕ್ಷರ buf[1024];

	ಆದರೆ (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		ಹಿಂತಿರುಗಿ -1;
	}

	ಆದರೆ (ftruncate(tdb->fd, ಗಾತ್ರ_2+addition) == -1) {
		ಅಕ್ಷರ b = 0;
		ಆದರೆ (pwrite(tdb->fd,  &b, 1, (ಗಾತ್ರ_2+addition) - 1) != 1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file to %d failed (%s)\n",
				 ಗಾತ್ರ_2+addition, strerror(errno)));
			ಹಿಂತಿರುಗಿ -1;
		}
	}

	/* now fill the file with something. This ensures that the
	   file isn't sparse, which would be very bad if we ran out of
	   disk. This must be done with write, not via mmap */
	memset(buf, TDB_PAD_BYTE, ಗಾತ್ರ(buf));
	ಆಗ (addition) {
		ಪೂರ್ಣಾಂಕ n = addition>ಗಾತ್ರ(buf)?ಗಾತ್ರ(buf):addition;
		ಪೂರ್ಣಾಂಕ ret = pwrite(tdb->fd, buf, n, ಗಾತ್ರ_2);
		ಆದರೆ (ret != n) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file write of %d failed (%s)\n",
				   n, strerror(errno)));
			ಹಿಂತಿರುಗಿ -1;
		}
		addition -= n;
		ಗಾತ್ರ_2 += n;
	}
	ಹಿಂತಿರುಗಿ 0;
}


/* expand the database at least size bytes by expanding the underlying
   file and doing the mmap again if necessary */
ಪೂರ್ಣಾಂಕ tdb_expand(ರಚನೆ tdb_context *tdb, tdb_off_t ಗಾತ್ರ_2)
{
	ರಚನೆ list_struct rec;
	tdb_off_t ಸ್ಥಾನಾಂತರ;

	ಆದರೆ (tdb_ಬೀಗ(tdb, -1, F_WRLCK) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "lock failed in tdb_expand\n"));
		ಹಿಂತಿರುಗಿ -1;
	}

	/* must know about any previous expansions by another process */
	tdb->methods->tdb_oob(tdb, tdb->map_ಗಾತ್ರ + 1, 1);

	/* always make room for at least 10 more records, and round
           the database up to a multiple of the page size */
	ಗಾತ್ರ_2 = TDB_ALIGN(tdb->map_ಗಾತ್ರ + ಗಾತ್ರ_2*10, tdb->ಪುಟ_ಗಾತ್ರ) - tdb->map_ಗಾತ್ರ;

	ಆದರೆ (!(tdb->flags & TDB_ಆಂತರಿಕ))
		tdb_munmap(tdb);

	/*
	 * We must ensure the file is unmapped before doing this
	 * to ensure consistency with systems like OpenBSD where
	 * writes and mmaps are not consistent.
	 */

	/* expand the file itself */
	ಆದರೆ (!(tdb->flags & TDB_ಆಂತರಿಕ)) {
		ಆದರೆ (tdb->methods->tdb_expand_file(tdb, tdb->map_ಗಾತ್ರ, ಗಾತ್ರ_2) != 0)
			ಹೋಗು fail;
	}

	tdb->map_ಗಾತ್ರ += ಗಾತ್ರ_2;

	ಆದರೆ (tdb->flags & TDB_ಆಂತರಿಕ) {
		ಅಕ್ಷರ *new_map_ptr = (ಅಕ್ಷರ *)realloc(tdb->map_ptr,
						    tdb->map_ಗಾತ್ರ);
		ಆದರೆ (!new_map_ptr) {
			tdb->map_ಗಾತ್ರ -= ಗಾತ್ರ_2;
			ಹೋಗು fail;
		}
		tdb->map_ptr = new_map_ptr;
	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		 * We must ensure the file is remapped before adding the space
		 * to ensure consistency with systems like OpenBSD where
		 * writes and mmaps are not consistent.
		 */

		/* We're ok if the mmap fails as we'll fallback to read/write */
		tdb_mmap(tdb);
	}

	/* form a new freelist record */
	memset(&rec,'\0',ಗಾತ್ರ(rec));
	rec.rec_len = ಗಾತ್ರ_2 - ಗಾತ್ರ(rec);

	/* link it into the free list */
	ಸ್ಥಾನಾಂತರ = tdb->map_ಗಾತ್ರ - ಗಾತ್ರ_2;
	ಆದರೆ (tdb_free(tdb, ಸ್ಥಾನಾಂತರ, &rec) == -1)
		ಹೋಗು fail;

	tdb_unlock(tdb, -1, F_WRLCK);
	ಹಿಂತಿರುಗಿ 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ಹಿಂತಿರುಗಿ -1;
}

/* read/write a tdb_off_t */
ಪೂರ್ಣಾಂಕ tdb_ofs_read(ರಚನೆ tdb_context *tdb, tdb_off_t ಸ್ಥಾನಾಂತರ, tdb_off_t *d)
{
	ಹಿಂತಿರುಗಿ tdb->methods->tdb_read(tdb, ಸ್ಥಾನಾಂತರ, (ಅಕ್ಷರ*)d, ಗಾತ್ರ(*d), DOCONV());
}

ಪೂರ್ಣಾಂಕ tdb_ofs_write(ರಚನೆ tdb_context *tdb, tdb_off_t ಸ್ಥಾನಾಂತರ, tdb_off_t *d)
{
	tdb_off_t off = *d;
	ಹಿಂತಿರುಗಿ tdb->methods->tdb_write(tdb, ಸ್ಥಾನಾಂತರ, CONVERT(off), ಗಾತ್ರ(*d));
}


/* read a lump of data, allocating the space for it */
ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *tdb_alloc_read(ರಚನೆ tdb_context *tdb, tdb_off_t ಸ್ಥಾನಾಂತರ, tdb_len_t len)
{
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *buf;

	/* some systems don't like zero length malloc */
	ಆದರೆ (len == 0) {
		len = 1;
	}

	ಆದರೆ (!(buf = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)ಸ್ಮರಣೆ_ಕೊಡು(len))) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_OOM;
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_alloc_read malloc failed len=%d (%s)\n",
			   len, strerror(errno)));
		ಹಿಂತಿರುಗಿ TDB_ERRCODE(TDB_ERR_OOM, buf);
	}
	ಆದರೆ (tdb->methods->tdb_read(tdb, ಸ್ಥಾನಾಂತರ, buf, len, 0) == -1) {
		ಸುರಕ್ಷಿತ_FREE(buf);
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}
	ಹಿಂತಿರುಗಿ buf;
}

/* Give a piece of tdb data to a parser */

ಪೂರ್ಣಾಂಕ tdb_parse_ದತ್ತಾಂಶ(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ,
		   tdb_off_t ಸ್ಥಾನಾಂತರ, tdb_len_t len,
		   ಪೂರ್ಣಾಂಕ (*parser)(TDB_ದತ್ತಾಂಶ ಕೀಲಿ, TDB_ದತ್ತಾಂಶ ದತ್ತಾಂಶ,
				 ಶೂನ್ಯ *ಖಾಸಗಿ_ದತ್ತಾಂಶ),
		   ಶೂನ್ಯ *ಖಾಸಗಿ_ದತ್ತಾಂಶ)
{
	TDB_ದತ್ತಾಂಶ ದತ್ತಾಂಶ;
	ಪೂರ್ಣಾಂಕ ಫಲಿತಾಂಶ;

	ದತ್ತಾಂಶ.dsize = len;

	ಆದರೆ ((tdb->transaction == ಖಾಲಿ) && (tdb->map_ptr != ಖಾಲಿ)) {
		/*
		 * Optimize by avoiding the malloc/memcpy/free, point the
		 * parser directly at the mmap area.
		 */
		ಆದರೆ (tdb->methods->tdb_oob(tdb, ಸ್ಥಾನಾಂತರ+len, 0) != 0) {
			ಹಿಂತಿರುಗಿ -1;
		}
		ದತ್ತಾಂಶ.dptr = ಸ್ಥಾನಾಂತರ + (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)tdb->map_ptr;
		ಹಿಂತಿರುಗಿ parser(ಕೀಲಿ, ದತ್ತಾಂಶ, ಖಾಸಗಿ_ದತ್ತಾಂಶ);
	}

	ಆದರೆ (!(ದತ್ತಾಂಶ.dptr = tdb_alloc_read(tdb, ಸ್ಥಾನಾಂತರ, len))) {
		ಹಿಂತಿರುಗಿ -1;
	}

	ಫಲಿತಾಂಶ = parser(ಕೀಲಿ, ದತ್ತಾಂಶ, ಖಾಸಗಿ_ದತ್ತಾಂಶ);
	ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ.dptr);
	ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

/* read/write a record */
ಪೂರ್ಣಾಂಕ tdb_rec_read(ರಚನೆ tdb_context *tdb, tdb_off_t ಸ್ಥಾನಾಂತರ, ರಚನೆ list_struct *rec)
{
	ಆದರೆ (tdb->methods->tdb_read(tdb, ಸ್ಥಾನಾಂತರ, rec, ಗಾತ್ರ(*rec),DOCONV()) == -1)
		ಹಿಂತಿರುಗಿ -1;
	ಆದರೆ (TDB_BAD_MAGIC(rec)) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_rec_read bad magic 0x%x at offset=%d\n", rec->magic, ಸ್ಥಾನಾಂತರ));
		ಹಿಂತಿರುಗಿ TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	ಹಿಂತಿರುಗಿ tdb->methods->tdb_oob(tdb, rec->ಮುಂದಿನ+ಗಾತ್ರ(*rec), 0);
}

ಪೂರ್ಣಾಂಕ tdb_rec_write(ರಚನೆ tdb_context *tdb, tdb_off_t ಸ್ಥಾನಾಂತರ, ರಚನೆ list_struct *rec)
{
	ರಚನೆ list_struct r = *rec;
	ಹಿಂತಿರುಗಿ tdb->methods->tdb_write(tdb, ಸ್ಥಾನಾಂತರ, CONVERT(r), ಗಾತ್ರ(r));
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ tdb_methods io_methods = {
	tdb_read,
	tdb_write,
	tdb_ಮುಂದಿನ_hash_chain,
	tdb_oob,
	tdb_expand_file,
	tdb_brlock
};

/*
  initialise the default methods table
*/
ಶೂನ್ಯ tdb_io_init(ರಚನೆ tdb_context *tdb)
{
	tdb->methods = &io_methods;
}

/* file: transaction.c */

/*
  transaction design:

  - only allow a single transaction at a time per database. This makes
    using the transaction API simpler, as otherwise the caller would
    have to cope with temporary failures in transactions that conflict
    with other current transactions

  - keep the transaction recovery information in the same file as the
    database, using a special 'transaction recovery' record pointed at
    by the header. This removes the need for extra journal files as
    used by some other databases

  - dynamically allocated the transaction recover record, re-using it
    for subsequent transactions. If a larger record is needed then
    tdb_free() the old record to place it on the normal tdb freelist
    before allocating the new record

  - during transactions, keep a linked list of writes all that have
    been performed by intercepting all tdb_write() calls. The hooked
    transaction versions of tdb_read() and tdb_write() check this
    linked list and try to use the elements of the list in preference
    to the real database.

  - don't allow any locks to be held when a transaction starts,
    otherwise we can end up with deadlock (plus lack of lock nesting
    in posix locks would mean the lock is lost)

  - if the caller gains a lock during the transaction but doesn't
    release it then fail the commit

  - allow for nested calls to tdb_transaction_start(), re-using the
    existing transaction record. If the inner transaction is cancelled
    then a subsequent commit will fail

  - keep a mirrored copy of the tdb hash chain heads to allow for the
    fast hash heads scan on traverse, updating the mirrored copy in
    the transaction version of tdb_write

  - allow callers to mix transaction and non-transaction use of tdb,
    although once a transaction is started then an exclusive lock is
    gained until the transaction is committed or cancelled

  - the commit strategy involves first saving away all modified data
    into a linearised buffer in the transaction recovery area, then
    marking the transaction recovery area with a magic value to
    indicate a valid recovery record. In total 4 fsync/msync calls are
    needed per commit to prevent race conditions. It might be possible
    to reduce this to 3 or even 2 with some more work.

  - check for a valid recovery record on open of the tdb, while the
    global lock is held. Automatically recover from the transaction
    recovery area if needed, then continue with the open as
    usual. This allows for smooth crash recovery with no administrator
    intervention.

  - if TDB_NOSYNC is passed to flags in tdb_open then transactions are
    still available, but no transaction recovery area is used and no
    fsync/msync calls are made.

*/

ರಚನೆ tdb_transaction_el {
	ರಚನೆ tdb_transaction_el *ಮುಂದಿನ, *prev;
	tdb_off_t ಸ್ಥಾನಾಂತರ;
	tdb_len_t ಉದ್ದಳತೆ;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ದತ್ತಾಂಶ;
};

/*
  hold the context of any current transaction
*/
ರಚನೆ tdb_transaction {
	/* we keep a mirrored copy of the tdb hash heads here so
	   tdb_next_hash_chain() can operate efficiently */
	u32 *hash_heads;

	/* the original io methods - used to do IOs to the real db */
	ಸ್ಥಿರಾಂಕ ರಚನೆ tdb_methods *io_methods;

	/* the list of transaction elements. We use a doubly linked
	   list with a last pointer to allow us to keep the list
	   ordered, with first element at the front of the list. It
	   needs to be doubly linked as the read/write traversals need
	   to be backwards, while the commit needs to be forwards */
	ರಚನೆ tdb_transaction_el *elements, *elements_ಕೊನೆಯ;

	/* non-zero when an internal transaction error has
	   occurred. All write operations will then fail until the
	   transaction is ended */
	ಪೂರ್ಣಾಂಕ transaction_error;

	/* when inside a transaction we need to keep track of any
	   nested tdb_transaction_start() calls, as these are allowed,
	   but don't create a new transaction */
	ಪೂರ್ಣಾಂಕ nesting;

	/* old file size before transaction */
	tdb_len_t old_map_ಗಾತ್ರ;
};


/*
  read while in a transaction. We need to check first if the data is in our list
  of transaction elements, then if not do a real read
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ transaction_read(ರಚನೆ tdb_context *tdb, tdb_off_t off, ಶೂನ್ಯ *buf,
			    tdb_len_t len, ಪೂರ್ಣಾಂಕ cv)
{
	ರಚನೆ tdb_transaction_el *el;

	/* we need to walk the list backwards to get the most recent data */
	ಗಾಗಿ (el=tdb->transaction->elements_ಕೊನೆಯ;el;el=el->prev) {
		tdb_len_t partial;

		ಆದರೆ (off+len <= el->ಸ್ಥಾನಾಂತರ) {
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (off >= el->ಸ್ಥಾನಾಂತರ + el->ಉದ್ದಳತೆ) {
			ಮುಂದುವರಿ;
		}

		/* an overlapping read - needs to be split into up to
		   2 reads and a memcpy */
		ಆದರೆ (off < el->ಸ್ಥಾನಾಂತರ) {
			partial = el->ಸ್ಥಾನಾಂತರ - off;
			ಆದರೆ (transaction_read(tdb, off, buf, partial, cv) != 0) {
				ಹೋಗು fail;
			}
			len -= partial;
			off += partial;
			buf = (ಶೂನ್ಯ *)(partial + (ಅಕ್ಷರ *)buf);
		}
		ಆದರೆ (off + len <= el->ಸ್ಥಾನಾಂತರ + el->ಉದ್ದಳತೆ) {
			partial = len;
		} ಇಲ್ಲದಿದ್ದರೆ {
			partial = el->ಸ್ಥಾನಾಂತರ + el->ಉದ್ದಳತೆ - off;
		}
		ಸ್ಮರಣೆ_ನಕಲು(buf, el->ದತ್ತಾಂಶ + (off - el->ಸ್ಥಾನಾಂತರ), partial);
		ಆದರೆ (cv) {
			tdb_convert(buf, len);
		}
		len -= partial;
		off += partial;
		buf = (ಶೂನ್ಯ *)(partial + (ಅಕ್ಷರ *)buf);

		ಆದರೆ (len != 0 && transaction_read(tdb, off, buf, len, cv) != 0) {
			ಹೋಗು fail;
		}

		ಹಿಂತಿರುಗಿ 0;
	}

	/* its not in the transaction elements - do a real read */
	ಹಿಂತಿರುಗಿ tdb->transaction->io_methods->tdb_read(tdb, off, buf, len, cv);

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_read: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	ಹಿಂತಿರುಗಿ -1;
}


/*
  write while in a transaction
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ transaction_write(ರಚನೆ tdb_context *tdb, tdb_off_t off,
			     ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf, tdb_len_t len)
{
	ರಚನೆ tdb_transaction_el *el, *ಉತ್ತಮ_el=ಖಾಲಿ;

	ಆದರೆ (len == 0) {
		ಹಿಂತಿರುಗಿ 0;
	}

	/* if the write is to a hash head, then update the transaction
	   hash heads */
	ಆದರೆ (len == ಗಾತ್ರ(tdb_off_t) && off >= FREELIST_ತುದಿ &&
	    off < FREELIST_ತುದಿ+TDB_HASHTABLE_ಗಾತ್ರ(tdb)) {
		u32 chain = (off-FREELIST_ತುದಿ) / ಗಾತ್ರ(tdb_off_t);
		ಸ್ಮರಣೆ_ನಕಲು(&tdb->transaction->hash_heads[chain], buf, len);
	}

	/* first see if we can replace an existing entry */
	ಗಾಗಿ (el=tdb->transaction->elements_ಕೊನೆಯ;el;el=el->prev) {
		tdb_len_t partial;

		ಆದರೆ (ಉತ್ತಮ_el == ಖಾಲಿ && off == el->ಸ್ಥಾನಾಂತರ+el->ಉದ್ದಳತೆ) {
			ಉತ್ತಮ_el = el;
		}

		ಆದರೆ (off+len <= el->ಸ್ಥಾನಾಂತರ) {
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (off >= el->ಸ್ಥಾನಾಂತರ + el->ಉದ್ದಳತೆ) {
			ಮುಂದುವರಿ;
		}

		/* an overlapping write - needs to be split into up to
		   2 writes and a memcpy */
		ಆದರೆ (off < el->ಸ್ಥಾನಾಂತರ) {
			partial = el->ಸ್ಥಾನಾಂತರ - off;
			ಆದರೆ (transaction_write(tdb, off, buf, partial) != 0) {
				ಹೋಗು fail;
			}
			len -= partial;
			off += partial;
			buf = (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *)(partial + (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *)buf);
		}
		ಆದರೆ (off + len <= el->ಸ್ಥಾನಾಂತರ + el->ಉದ್ದಳತೆ) {
			partial = len;
		} ಇಲ್ಲದಿದ್ದರೆ {
			partial = el->ಸ್ಥಾನಾಂತರ + el->ಉದ್ದಳತೆ - off;
		}
		ಸ್ಮರಣೆ_ನಕಲು(el->ದತ್ತಾಂಶ + (off - el->ಸ್ಥಾನಾಂತರ), buf, partial);
		len -= partial;
		off += partial;
		buf = (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *)(partial + (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *)buf);

		ಆದರೆ (len != 0 && transaction_write(tdb, off, buf, len) != 0) {
			ಹೋಗು fail;
		}

		ಹಿಂತಿರುಗಿ 0;
	}

	/* see if we can append the new entry to an existing entry */
	ಆದರೆ (ಉತ್ತಮ_el && ಉತ್ತಮ_el->ಸ್ಥಾನಾಂತರ + ಉತ್ತಮ_el->ಉದ್ದಳತೆ == off &&
	    (off+len < tdb->transaction->old_map_ಗಾತ್ರ ||
	     off > tdb->transaction->old_map_ಗಾತ್ರ)) {
		ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ದತ್ತಾಂಶ = ಉತ್ತಮ_el->ದತ್ತಾಂಶ;
		el = ಉತ್ತಮ_el;
		el->ದತ್ತಾಂಶ = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)realloc(el->ದತ್ತಾಂಶ,
						    el->ಉದ್ದಳತೆ + len);
		ಆದರೆ (el->ದತ್ತಾಂಶ == ಖಾಲಿ) {
			tdb->ecode = TDB_ERR_OOM;
			tdb->transaction->transaction_error = 1;
			el->ದತ್ತಾಂಶ = ದತ್ತಾಂಶ;
			ಹಿಂತಿರುಗಿ -1;
		}
		ಆದರೆ (buf) {
			ಸ್ಮರಣೆ_ನಕಲು(el->ದತ್ತಾಂಶ + el->ಉದ್ದಳತೆ, buf, len);
		} ಇಲ್ಲದಿದ್ದರೆ {
			memset(el->ದತ್ತಾಂಶ + el->ಉದ್ದಳತೆ, TDB_PAD_BYTE, len);
		}
		el->ಉದ್ದಳತೆ += len;
		ಹಿಂತಿರುಗಿ 0;
	}

	/* add a new entry at the end of the list */
	el = (ರಚನೆ tdb_transaction_el *)ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(*el));
	ಆದರೆ (el == ಖಾಲಿ) {
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		ಹಿಂತಿರುಗಿ -1;
	}
	el->ಮುಂದಿನ = ಖಾಲಿ;
	el->prev = tdb->transaction->elements_ಕೊನೆಯ;
	el->ಸ್ಥಾನಾಂತರ = off;
	el->ಉದ್ದಳತೆ = len;
	el->ದತ್ತಾಂಶ = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)ಸ್ಮರಣೆ_ಕೊಡು(len);
	ಆದರೆ (el->ದತ್ತಾಂಶ == ಖಾಲಿ) {
		ಬಿಡುಗಡೆ(el);
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		ಹಿಂತಿರುಗಿ -1;
	}
	ಆದರೆ (buf) {
		ಸ್ಮರಣೆ_ನಕಲು(el->ದತ್ತಾಂಶ, buf, len);
	} ಇಲ್ಲದಿದ್ದರೆ {
		memset(el->ದತ್ತಾಂಶ, TDB_PAD_BYTE, len);
	}
	ಆದರೆ (el->prev) {
		el->prev->ಮುಂದಿನ = el;
	} ಇಲ್ಲದಿದ್ದರೆ {
		tdb->transaction->elements = el;
	}
	tdb->transaction->elements_ಕೊನೆಯ = el;
	ಹಿಂತಿರುಗಿ 0;

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_write: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	ಹಿಂತಿರುಗಿ -1;
}

/*
  accelerated hash chain head search, using the cached hash heads
*/
ಸ್ಥಿರ ಶೂನ್ಯ transaction_ಮುಂದಿನ_hash_chain(ರಚನೆ tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	ಗಾಗಿ (;h < tdb->ಶೀರ್ಷಿಕೆ.hash_ಗಾತ್ರ;h++) {
		/* the +1 takes account of the freelist */
		ಆದರೆ (0 != tdb->transaction->hash_heads[h+1]) {
			ನಿಲ್ಲಿಸು;
		}
	}
	(*chain) = h;
}

/*
  out of bounds check during a transaction
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ transaction_oob(ರಚನೆ tdb_context *tdb, tdb_off_t len,
			   ಪೂರ್ಣಾಂಕ probe EXT2FS_ATTR((ಬಳಸದ)))
{
	ಆದರೆ (len <= tdb->map_ಗಾತ್ರ) {
		ಹಿಂತಿರುಗಿ 0;
	}
	ಹಿಂತಿರುಗಿ TDB_ERRCODE(TDB_ERR_IO, -1);
}

/*
  transaction version of tdb_expand().
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ transaction_expand_file(ರಚನೆ tdb_context *tdb, tdb_off_t ಗಾತ್ರ_2,
				   tdb_off_t addition)
{
	/* add a write to the transaction elements, so subsequent
	   reads see the zero data */
	ಆದರೆ (transaction_write(tdb, ಗಾತ್ರ_2, ಖಾಲಿ, addition) != 0) {
		ಹಿಂತಿರುಗಿ -1;
	}

	ಹಿಂತಿರುಗಿ 0;
}

/*
  brlock during a transaction - ignore them
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ transaction_brlock(ರಚನೆ tdb_context *tdb EXT2FS_ATTR((ಬಳಸದ)),
			      tdb_off_t ಸ್ಥಾನಾಂತರ EXT2FS_ATTR((ಬಳಸದ)),
			      ಪೂರ್ಣಾಂಕ rw_type EXT2FS_ATTR((ಬಳಸದ)),
			      ಪೂರ್ಣಾಂಕ lck_type EXT2FS_ATTR((ಬಳಸದ)),
			      ಪೂರ್ಣಾಂಕ probe EXT2FS_ATTR((ಬಳಸದ)),
			      size_t len EXT2FS_ATTR((ಬಳಸದ)))
{
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ tdb_methods transaction_methods = {
	transaction_read,
	transaction_write,
	transaction_ಮುಂದಿನ_hash_chain,
	transaction_oob,
	transaction_expand_file,
	transaction_brlock
};


/*
  start a tdb transaction. No token is returned, as only a single
  transaction is allowed to be pending per tdb_context
*/
ಪೂರ್ಣಾಂಕ tdb_transaction_ಆರಂಭ(ರಚನೆ tdb_context *tdb)
{
	/* some sanity checks */
	ಆದರೆ (tdb->read_only || (tdb->flags & TDB_ಆಂತರಿಕ) || tdb->traverse_read) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction on a read-only or internal db\n"));
		tdb->ecode = TDB_ERR_EINVAL;
		ಹಿಂತಿರುಗಿ -1;
	}

	/* cope with nested tdb_transaction_start() calls */
	ಆದರೆ (tdb->transaction != ಖಾಲಿ) {
		tdb->transaction->nesting++;
		TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_start: nesting %d\n",
			 tdb->transaction->nesting));
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (tdb->ಅಂಕೆ_locks != 0 || tdb->global_ಬೀಗ.ಎಣಿಕೆ_2) {
		/* the caller must not have any locks when starting a
		   transaction as otherwise we'll be screwed by lack
		   of nested locks in posix */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction with locks held\n"));
		tdb->ecode = TDB_ERR_ಬೀಗ;
		ಹಿಂತಿರುಗಿ -1;
	}

	ಆದರೆ (tdb->travlocks.ಮುಂದಿನ != ಖಾಲಿ) {
		/* you cannot use transactions inside a traverse (although you can use
		   traverse inside a transaction) as otherwise you can end up with
		   deadlock */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction within a traverse\n"));
		tdb->ecode = TDB_ERR_ಬೀಗ;
		ಹಿಂತಿರುಗಿ -1;
	}

	tdb->transaction = (ರಚನೆ tdb_transaction *)
		ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(ರಚನೆ tdb_transaction), 1);
	ಆದರೆ (tdb->transaction == ಖಾಲಿ) {
		tdb->ecode = TDB_ERR_OOM;
		ಹಿಂತಿರುಗಿ -1;
	}

	/* get the transaction write lock. This is a blocking lock. As
	   discussed with Volker, there are a number of ways we could
	   make this async, which we will probably do in the future */
	ಆದರೆ (tdb_transaction_ಬೀಗ(tdb, F_WRLCK) == -1) {
		ಸುರಕ್ಷಿತ_FREE(tdb->transaction);
		ಹಿಂತಿರುಗಿ -1;
	}

	/* get a read lock from the freelist to the end of file. This
	   is upgraded to a write lock during the commit */
	ಆದರೆ (tdb_brlock(tdb, FREELIST_ತುದಿ, F_RDLCK, F_SETLKW, 0, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to get hash locks\n"));
		tdb->ecode = TDB_ERR_ಬೀಗ;
		ಹೋಗು fail;
	}

	/* setup a copy of the hash table heads so the hash scan in
	   traverse can be fast */
	tdb->transaction->hash_heads = (u32 *)
		ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(tdb->ಶೀರ್ಷಿಕೆ.hash_ಗಾತ್ರ+1, ಗಾತ್ರ(u32));
	ಆದರೆ (tdb->transaction->hash_heads == ಖಾಲಿ) {
		tdb->ecode = TDB_ERR_OOM;
		ಹೋಗು fail;
	}
	ಆದರೆ (tdb->methods->tdb_read(tdb, FREELIST_ತುದಿ, tdb->transaction->hash_heads,
				   TDB_HASHTABLE_ಗಾತ್ರ(tdb), 0) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to read hash heads\n"));
		tdb->ecode = TDB_ERR_IO;
		ಹೋಗು fail;
	}

	/* make sure we know about any file expansions already done by
	   anyone else */
	tdb->methods->tdb_oob(tdb, tdb->map_ಗಾತ್ರ + 1, 1);
	tdb->transaction->old_map_ಗಾತ್ರ = tdb->map_ಗಾತ್ರ;

	/* finally hook the io methods, replacing them with
	   transaction specific methods */
	tdb->transaction->io_methods = tdb->methods;
	tdb->methods = &transaction_methods;

	/* by calling this transaction write here, we ensure that we don't grow the
	   transaction linked list due to hash table updates */
	ಆದರೆ (transaction_write(tdb, FREELIST_ತುದಿ, tdb->transaction->hash_heads,
			      TDB_HASHTABLE_ಗಾತ್ರ(tdb)) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to prime hash table\n"));
		tdb->ecode = TDB_ERR_IO;
		tdb->methods = tdb->transaction->io_methods;
		ಹೋಗು fail;
	}

	ಹಿಂತಿರುಗಿ 0;

fail:
	tdb_brlock(tdb, FREELIST_ತುದಿ, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	ಸುರಕ್ಷಿತ_FREE(tdb->transaction->hash_heads);
	ಸುರಕ್ಷಿತ_FREE(tdb->transaction);
	ಹಿಂತಿರುಗಿ -1;
}


/*
  cancel the current transaction
*/
ಪೂರ್ಣಾಂಕ tdb_transaction_cancel(ರಚನೆ tdb_context *tdb)
{
	ಆದರೆ (tdb->transaction == ಖಾಲಿ) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_cancel: no transaction\n"));
		ಹಿಂತಿರುಗಿ -1;
	}

	ಆದರೆ (tdb->transaction->nesting != 0) {
		tdb->transaction->transaction_error = 1;
		tdb->transaction->nesting--;
		ಹಿಂತಿರುಗಿ 0;
	}

	tdb->map_ಗಾತ್ರ = tdb->transaction->old_map_ಗಾತ್ರ;

	/* free all the transaction elements */
	ಆಗ (tdb->transaction->elements) {
		ರಚನೆ tdb_transaction_el *el = tdb->transaction->elements;
		tdb->transaction->elements = el->ಮುಂದಿನ;
		ಬಿಡುಗಡೆ(el->ದತ್ತಾಂಶ);
		ಬಿಡುಗಡೆ(el);
	}

	/* remove any global lock created during the transaction */
	ಆದರೆ (tdb->global_ಬೀಗ.ಎಣಿಕೆ_2 != 0) {
		tdb_brlock(tdb, FREELIST_ತುದಿ, F_UNLCK, F_SETLKW, 0, 4*tdb->ಶೀರ್ಷಿಕೆ.hash_ಗಾತ್ರ);
		tdb->global_ಬೀಗ.ಎಣಿಕೆ_2 = 0;
	}

	/* remove any locks created during the transaction */
	ಆದರೆ (tdb->ಅಂಕೆ_locks != 0) {
		ಪೂರ್ಣಾಂಕ i;
		ಗಾಗಿ (i=0;i<tdb->ಅಂಕೆ_lockrecs;i++) {
			tdb_brlock(tdb,FREELIST_ತುದಿ+4*tdb->lockrecs[i].list,
				   F_UNLCK,F_SETLKW, 0, 1);
		}
		tdb->ಅಂಕೆ_locks = 0;
		tdb->ಅಂಕೆ_lockrecs = 0;
		ಸುರಕ್ಷಿತ_FREE(tdb->lockrecs);
	}

	/* restore the normal io methods */
	tdb->methods = tdb->transaction->io_methods;

	tdb_brlock(tdb, FREELIST_ತುದಿ, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	ಸುರಕ್ಷಿತ_FREE(tdb->transaction->hash_heads);
	ಸುರಕ್ಷಿತ_FREE(tdb->transaction);

	ಹಿಂತಿರುಗಿ 0;
}

/*
  sync to disk
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ transaction_sync(ರಚನೆ tdb_context *tdb, tdb_off_t ಸ್ಥಾನಾಂತರ, tdb_len_t ಉದ್ದಳತೆ)
{
	ಆದರೆ (fsync(tdb->fd) != 0) {
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: fsync failed\n"));
		ಹಿಂತಿರುಗಿ -1;
	}
#ಆದರೆ defined(HAVE_MSYNC) && defined(MS_SYNC)
	ಆದರೆ (tdb->map_ptr) {
		tdb_off_t moffset = ಸ್ಥಾನಾಂತರ & ~(tdb->ಪುಟ_ಗಾತ್ರ-1);
		ಆದರೆ (msync(moffset + (ಅಕ್ಷರ *)tdb->map_ptr,
			  ಉದ್ದಳತೆ + (ಸ್ಥಾನಾಂತರ - moffset), MS_SYNC) != 0) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: msync failed - %s\n",
				 strerror(errno)));
			ಹಿಂತಿರುಗಿ -1;
		}
	}
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ 0;
}


/*
  work out how much space the linearised recovery data will consume
*/
ಸ್ಥಿರ tdb_len_t tdb_recovery_ಗಾತ್ರ(ರಚನೆ tdb_context *tdb)
{
	ರಚನೆ tdb_transaction_el *el;
	tdb_len_t recovery_ಗಾತ್ರ = 0;

	recovery_ಗಾತ್ರ = ಗಾತ್ರ(u32);
	ಗಾಗಿ (el=tdb->transaction->elements;el;el=el->ಮುಂದಿನ) {
		ಆದರೆ (el->ಸ್ಥಾನಾಂತರ >= tdb->transaction->old_map_ಗಾತ್ರ) {
			ಮುಂದುವರಿ;
		}
		recovery_ಗಾತ್ರ += 2*ಗಾತ್ರ(tdb_off_t) + el->ಉದ್ದಳತೆ;
	}

	ಹಿಂತಿರುಗಿ recovery_ಗಾತ್ರ;
}

/*
  allocate the recovery area, or use an existing recovery area if it is
  large enough
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_recovery_allocate(ರಚನೆ tdb_context *tdb,
				 tdb_len_t *recovery_ಗಾತ್ರ,
				 tdb_off_t *recovery_ಸ್ಥಾನಾಂತರ,
				 tdb_len_t *recovery_max_ಗಾತ್ರ)
{
	ರಚನೆ list_struct rec;
	ಸ್ಥಿರಾಂಕ ರಚನೆ tdb_methods *methods = tdb->transaction->io_methods;
	tdb_off_t recovery_ತಲೆ;

	ಆದರೆ (tdb_ofs_read(tdb, TDB_RECOVERY_ತಲೆ, &recovery_ತಲೆ) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery head\n"));
		ಹಿಂತಿರುಗಿ -1;
	}

	rec.rec_len = 0;

	ಆದರೆ (recovery_ತಲೆ != 0 &&
	    methods->tdb_read(tdb, recovery_ತಲೆ, &rec, ಗಾತ್ರ(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery record\n"));
		ಹಿಂತಿರುಗಿ -1;
	}

	*recovery_ಗಾತ್ರ = tdb_recovery_ಗಾತ್ರ(tdb);

	ಆದರೆ (recovery_ತಲೆ != 0 && *recovery_ಗಾತ್ರ <= rec.rec_len) {
		/* it fits in the existing area */
		*recovery_max_ಗಾತ್ರ = rec.rec_len;
		*recovery_ಸ್ಥಾನಾಂತರ = recovery_ತಲೆ;
		ಹಿಂತಿರುಗಿ 0;
	}

	/* we need to free up the old recovery area, then allocate a
	   new one at the end of the file. Note that we cannot use
	   tdb_allocate() to allocate the new one as that might return
	   us an area that is being currently used (as of the start of
	   the transaction) */
	ಆದರೆ (recovery_ತಲೆ != 0) {
		ಆದರೆ (tdb_free(tdb, recovery_ತಲೆ, &rec) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to free previous recovery area\n"));
			ಹಿಂತಿರುಗಿ -1;
		}
	}

	/* the tdb_free() call might have increased the recovery size */
	*recovery_ಗಾತ್ರ = tdb_recovery_ಗಾತ್ರ(tdb);

	/* round up to a multiple of page size */
	*recovery_max_ಗಾತ್ರ = TDB_ALIGN(ಗಾತ್ರ(rec) + *recovery_ಗಾತ್ರ, tdb->ಪುಟ_ಗಾತ್ರ) - ಗಾತ್ರ(rec);
	*recovery_ಸ್ಥಾನಾಂತರ = tdb->map_ಗಾತ್ರ;
	recovery_ತಲೆ = *recovery_ಸ್ಥಾನಾಂತರ;

	ಆದರೆ (methods->tdb_expand_file(tdb, tdb->transaction->old_map_ಗಾತ್ರ,
				     (tdb->map_ಗಾತ್ರ - tdb->transaction->old_map_ಗಾತ್ರ) +
				     ಗಾತ್ರ(rec) + *recovery_max_ಗಾತ್ರ) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to create recovery area\n"));
		ಹಿಂತಿರುಗಿ -1;
	}

	/* remap the file (if using mmap) */
	methods->tdb_oob(tdb, tdb->map_ಗಾತ್ರ + 1, 1);

	/* we have to reset the old map size so that we don't try to expand the file
	   again in the transaction commit, which would destroy the recovery area */
	tdb->transaction->old_map_ಗಾತ್ರ = tdb->map_ಗಾತ್ರ;

	/* write the recovery header offset and sync - we can sync without a race here
	   as the magic ptr in the recovery record has not been set */
	CONVERT(recovery_ತಲೆ);
	ಆದರೆ (methods->tdb_write(tdb, TDB_RECOVERY_ತಲೆ,
			       &recovery_ತಲೆ, ಗಾತ್ರ(tdb_off_t)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to write recovery head\n"));
		ಹಿಂತಿರುಗಿ -1;
	}

	ಹಿಂತಿರುಗಿ 0;
}


/*
  setup the recovery data that will be used on a crash during commit
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ transaction_ಸಿದ್ಧತೆ_recovery(ರಚನೆ tdb_context *tdb,
				      tdb_off_t *magic_ಸ್ಥಾನಾಂತರ)
{
	ರಚನೆ tdb_transaction_el *el;
	tdb_len_t recovery_ಗಾತ್ರ;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ದತ್ತಾಂಶ, *p;
	ಸ್ಥಿರಾಂಕ ರಚನೆ tdb_methods *methods = tdb->transaction->io_methods;
	ರಚನೆ list_struct *rec;
	tdb_off_t recovery_ಸ್ಥಾನಾಂತರ, recovery_max_ಗಾತ್ರ;
	tdb_off_t old_map_ಗಾತ್ರ = tdb->transaction->old_map_ಗಾತ್ರ;
	u32 magic, tailer;

	/*
	  check that the recovery area has enough space
	*/
	ಆದರೆ (tdb_recovery_allocate(tdb, &recovery_ಗಾತ್ರ,
				  &recovery_ಸ್ಥಾನಾಂತರ, &recovery_max_ಗಾತ್ರ) == -1) {
		ಹಿಂತಿರುಗಿ -1;
	}

	ದತ್ತಾಂಶ = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)ಸ್ಮರಣೆ_ಕೊಡು(recovery_ಗಾತ್ರ + ಗಾತ್ರ(*rec));
	ಆದರೆ (ದತ್ತಾಂಶ == ಖಾಲಿ) {
		tdb->ecode = TDB_ERR_OOM;
		ಹಿಂತಿರುಗಿ -1;
	}

	rec = (ರಚನೆ list_struct *)ದತ್ತಾಂಶ;
	memset(rec, 0, ಗಾತ್ರ(*rec));

	rec->magic    = 0;
	rec->ದತ್ತಾಂಶ_len = recovery_ಗಾತ್ರ;
	rec->rec_len  = recovery_max_ಗಾತ್ರ;
	rec->ಕೀಲಿ_len  = old_map_ಗಾತ್ರ;
	CONVERT(rec);

	/* build the recovery data into a single blob to allow us to do a single
	   large write, which should be more efficient */
	p = ದತ್ತಾಂಶ + ಗಾತ್ರ(*rec);
	ಗಾಗಿ (el=tdb->transaction->elements;el;el=el->ಮುಂದಿನ) {
		ಆದರೆ (el->ಸ್ಥಾನಾಂತರ >= old_map_ಗಾತ್ರ) {
			ಮುಂದುವರಿ;
		}
		ಆದರೆ (el->ಸ್ಥಾನಾಂತರ + el->ಉದ್ದಳತೆ > tdb->transaction->old_map_ಗಾತ್ರ) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: transaction data over new region boundary\n"));
			ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ);
			tdb->ecode = TDB_ERR_CORRUPT;
			ಹಿಂತಿರುಗಿ -1;
		}
		ಸ್ಮರಣೆ_ನಕಲು(p, &el->ಸ್ಥಾನಾಂತರ, 4);
		ಸ್ಮರಣೆ_ನಕಲು(p+4, &el->ಉದ್ದಳತೆ, 4);
		ಆದರೆ (DOCONV()) {
			tdb_convert(p, 8);
		}
		/* the recovery area contains the old data, not the
		   new data, so we have to call the original tdb_read
		   method to get it */
		ಆದರೆ (methods->tdb_read(tdb, el->ಸ್ಥಾನಾಂತರ, p + 8, el->ಉದ್ದಳತೆ, 0) != 0) {
			ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ);
			tdb->ecode = TDB_ERR_IO;
			ಹಿಂತಿರುಗಿ -1;
		}
		p += 8 + el->ಉದ್ದಳತೆ;
	}

	/* and the tailer */
	tailer = ಗಾತ್ರ(*rec) + recovery_max_ಗಾತ್ರ;
	ಸ್ಮರಣೆ_ನಕಲು(p, &tailer, 4);
	CONVERT(p);

	/* write the recovery data to the recovery area */
	ಆದರೆ (methods->tdb_write(tdb, recovery_ಸ್ಥಾನಾಂತರ, ದತ್ತಾಂಶ, ಗಾತ್ರ(*rec) + recovery_ಗಾತ್ರ) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery data\n"));
		ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ);
		tdb->ecode = TDB_ERR_IO;
		ಹಿಂತಿರುಗಿ -1;
	}

	/* as we don't have ordered writes, we have to sync the recovery
	   data before we update the magic to indicate that the recovery
	   data is present */
	ಆದರೆ (transaction_sync(tdb, recovery_ಸ್ಥಾನಾಂತರ, ಗಾತ್ರ(*rec) + recovery_ಗಾತ್ರ) == -1) {
		ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ);
		ಹಿಂತಿರುಗಿ -1;
	}

	ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ);

	magic = TDB_RECOVERY_MAGIC;
	CONVERT(magic);

	*magic_ಸ್ಥಾನಾಂತರ = recovery_ಸ್ಥಾನಾಂತರ + offsetof(ರಚನೆ list_struct, magic);

	ಆದರೆ (methods->tdb_write(tdb, *magic_ಸ್ಥಾನಾಂತರ, &magic, ಗಾತ್ರ(magic)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		ಹಿಂತಿರುಗಿ -1;
	}

	/* ensure the recovery magic marker is on disk */
	ಆದರೆ (transaction_sync(tdb, *magic_ಸ್ಥಾನಾಂತರ, ಗಾತ್ರ(magic)) == -1) {
		ಹಿಂತಿರುಗಿ -1;
	}

	ಹಿಂತಿರುಗಿ 0;
}

/*
  commit the current transaction
*/
ಪೂರ್ಣಾಂಕ tdb_transaction_commit(ರಚನೆ tdb_context *tdb)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ tdb_methods *methods;
	tdb_off_t magic_ಸ್ಥಾನಾಂತರ = 0;
	u32 zero = 0;

	ಆದರೆ (tdb->transaction == ಖಾಲಿ) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: no transaction\n"));
		ಹಿಂತಿರುಗಿ -1;
	}

	ಆದರೆ (tdb->transaction->transaction_error) {
		tdb->ecode = TDB_ERR_IO;
		tdb_transaction_cancel(tdb);
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: transaction error pending\n"));
		ಹಿಂತಿರುಗಿ -1;
	}

	ಆದರೆ (tdb->transaction->nesting != 0) {
		tdb->transaction->nesting--;
		ಹಿಂತಿರುಗಿ 0;
	}

	/* check for a null transaction */
	ಆದರೆ (tdb->transaction->elements == ಖಾಲಿ) {
		tdb_transaction_cancel(tdb);
		ಹಿಂತಿರುಗಿ 0;
	}

	methods = tdb->transaction->io_methods;

	/* if there are any locks pending then the caller has not
	   nested their locks properly, so fail the transaction */
	ಆದರೆ (tdb->ಅಂಕೆ_locks || tdb->global_ಬೀಗ.ಎಣಿಕೆ_2) {
		tdb->ecode = TDB_ERR_ಬೀಗ;
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: locks pending on commit\n"));
		tdb_transaction_cancel(tdb);
		ಹಿಂತಿರುಗಿ -1;
	}

	/* upgrade the main transaction lock region to a write lock */
	ಆದರೆ (tdb_brlock_upgrade(tdb, FREELIST_ತುದಿ, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to upgrade hash locks\n"));
		tdb->ecode = TDB_ERR_ಬೀಗ;
		tdb_transaction_cancel(tdb);
		ಹಿಂತಿರುಗಿ -1;
	}

	/* get the global lock - this prevents new users attaching to the database
	   during the commit */
	ಆದರೆ (tdb_brlock(tdb, GLOBAL_ಬೀಗ, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: failed to get global lock\n"));
		tdb->ecode = TDB_ERR_ಬೀಗ;
		tdb_transaction_cancel(tdb);
		ಹಿಂತಿರುಗಿ -1;
	}

	ಆದರೆ (!(tdb->flags & TDB_NOSYNC)) {
		/* write the recovery data to the end of the file */
		ಆದರೆ (transaction_ಸಿದ್ಧತೆ_recovery(tdb, &magic_ಸ್ಥಾನಾಂತರ) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to setup recovery data\n"));
			tdb_brlock(tdb, GLOBAL_ಬೀಗ, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			ಹಿಂತಿರುಗಿ -1;
		}
	}

	/* expand the file to the new size if needed */
	ಆದರೆ (tdb->map_ಗಾತ್ರ != tdb->transaction->old_map_ಗಾತ್ರ) {
		ಆದರೆ (methods->tdb_expand_file(tdb, tdb->transaction->old_map_ಗಾತ್ರ,
					     tdb->map_ಗಾತ್ರ -
					     tdb->transaction->old_map_ಗಾತ್ರ) == -1) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: expansion failed\n"));
			tdb_brlock(tdb, GLOBAL_ಬೀಗ, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			ಹಿಂತಿರುಗಿ -1;
		}
		tdb->map_ಗಾತ್ರ = tdb->transaction->old_map_ಗಾತ್ರ;
		methods->tdb_oob(tdb, tdb->map_ಗಾತ್ರ + 1, 1);
	}

	/* perform all the writes */
	ಆಗ (tdb->transaction->elements) {
		ರಚನೆ tdb_transaction_el *el = tdb->transaction->elements;

		ಆದರೆ (methods->tdb_write(tdb, el->ಸ್ಥಾನಾಂತರ, el->ದತ್ತಾಂಶ, el->ಉದ್ದಳತೆ) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed during commit\n"));

			/* we've overwritten part of the data and
			   possibly expanded the file, so we need to
			   run the crash recovery code */
			tdb->methods = methods;
			tdb_transaction_recover(tdb);

			tdb_transaction_cancel(tdb);
			tdb_brlock(tdb, GLOBAL_ಬೀಗ, F_UNLCK, F_SETLKW, 0, 1);

			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed\n"));
			ಹಿಂತಿರುಗಿ -1;
		}
		tdb->transaction->elements = el->ಮುಂದಿನ;
		ಬಿಡುಗಡೆ(el->ದತ್ತಾಂಶ);
		ಬಿಡುಗಡೆ(el);
	}

	ಆದರೆ (!(tdb->flags & TDB_NOSYNC)) {
		/* ensure the new data is on disk */
		ಆದರೆ (transaction_sync(tdb, 0, tdb->map_ಗಾತ್ರ) == -1) {
			ಹಿಂತಿರುಗಿ -1;
		}

		/* remove the recovery marker */
		ಆದರೆ (methods->tdb_write(tdb, magic_ಸ್ಥಾನಾಂತರ, &zero, 4) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to remove recovery magic\n"));
			ಹಿಂತಿರುಗಿ -1;
		}

		/* ensure the recovery marker has been removed on disk */
		ಆದರೆ (transaction_sync(tdb, magic_ಸ್ಥಾನಾಂತರ, 4) == -1) {
			ಹಿಂತಿರುಗಿ -1;
		}
	}

	tdb_brlock(tdb, GLOBAL_ಬೀಗ, F_UNLCK, F_SETLKW, 0, 1);

	/*
	  TODO: maybe write to some dummy hdr field, or write to magic
	  offset without mmap, before the last sync, instead of the
	  utime() call
	*/

	/* on some systems (like Linux 2.6.x) changes via mmap/msync
	   don't change the mtime of the file, this means the file may
	   not be backed up (as tdb rounding to block sizes means that
	   file size changes are quite rare too). The following forces
	   mtime changes when a transaction completes */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_UTIME
	utime(tdb->name, ಖಾಲಿ);
#ಆದರೆ_ಅಂತ್ಯ

	/* use a transaction cancel to free memory and remove the
	   transaction locks */
	tdb_transaction_cancel(tdb);
	ಹಿಂತಿರುಗಿ 0;
}


/*
  recover from an aborted transaction. Must be called with exclusive
  database write access already established (including the global
  lock to prevent new processes attaching)
*/
ಪೂರ್ಣಾಂಕ tdb_transaction_recover(ರಚನೆ tdb_context *tdb)
{
	tdb_off_t recovery_ತಲೆ, recovery_eof;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ದತ್ತಾಂಶ, *p;
	u32 zero = 0;
	ರಚನೆ list_struct rec;

	/* find the recovery area */
	ಆದರೆ (tdb_ofs_read(tdb, TDB_RECOVERY_ತಲೆ, &recovery_ತಲೆ) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery head\n"));
		tdb->ecode = TDB_ERR_IO;
		ಹಿಂತಿರುಗಿ -1;
	}

	ಆದರೆ (recovery_ತಲೆ == 0) {
		/* we have never allocated a recovery record */
		ಹಿಂತಿರುಗಿ 0;
	}

	/* read the recovery record */
	ಆದರೆ (tdb->methods->tdb_read(tdb, recovery_ತಲೆ, &rec,
				   ಗಾತ್ರ(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery record\n"));
		tdb->ecode = TDB_ERR_IO;
		ಹಿಂತಿರುಗಿ -1;
	}

	ಆದರೆ (rec.magic != TDB_RECOVERY_MAGIC) {
		/* there is no valid recovery data */
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (tdb->read_only) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: attempt to recover read only database\n"));
		tdb->ecode = TDB_ERR_CORRUPT;
		ಹಿಂತಿರುಗಿ -1;
	}

	recovery_eof = rec.ಕೀಲಿ_len;

	ದತ್ತಾಂಶ = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)ಸ್ಮರಣೆ_ಕೊಡು(rec.ದತ್ತಾಂಶ_len);
	ಆದರೆ (ದತ್ತಾಂಶ == ಖಾಲಿ) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to allocate recovery data\n"));
		tdb->ecode = TDB_ERR_OOM;
		ಹಿಂತಿರುಗಿ -1;
	}

	/* read the full recovery data */
	ಆದರೆ (tdb->methods->tdb_read(tdb, recovery_ತಲೆ + ಗಾತ್ರ(rec), ದತ್ತಾಂಶ,
				   rec.ದತ್ತಾಂಶ_len, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery data\n"));
		tdb->ecode = TDB_ERR_IO;
		ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ);
		ಹಿಂತಿರುಗಿ -1;
	}

	/* recover the file data */
	p = ದತ್ತಾಂಶ;
	ಆಗ (p+8 < ದತ್ತಾಂಶ + rec.ದತ್ತಾಂಶ_len) {
		u32 ofs, len;
		ಆದರೆ (DOCONV()) {
			tdb_convert(p, 8);
		}
		ಸ್ಮರಣೆ_ನಕಲು(&ofs, p, 4);
		ಸ್ಮರಣೆ_ನಕಲು(&len, p+4, 4);

		ಆದರೆ (tdb->methods->tdb_write(tdb, ofs, p+8, len) == -1) {
			ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ);
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to recover %d bytes at offset %d\n", len, ofs));
			tdb->ecode = TDB_ERR_IO;
			ಹಿಂತಿರುಗಿ -1;
		}
		p += 8 + len;
	}

	ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ);

	ಆದರೆ (transaction_sync(tdb, 0, tdb->map_ಗಾತ್ರ) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		ಹಿಂತಿರುಗಿ -1;
	}

	/* if the recovery area is after the recovered eof then remove it */
	ಆದರೆ (recovery_eof <= recovery_ತಲೆ) {
		ಆದರೆ (tdb_ofs_write(tdb, TDB_RECOVERY_ತಲೆ, &zero) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery head\n"));
			tdb->ecode = TDB_ERR_IO;
			ಹಿಂತಿರುಗಿ -1;
		}
	}

	/* remove the recovery magic */
	ಆದರೆ (tdb_ofs_write(tdb, recovery_ತಲೆ + offsetof(ರಚನೆ list_struct, magic),
			  &zero) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		ಹಿಂತಿರುಗಿ -1;
	}

	/* reduce the file size to the old size */
	tdb_munmap(tdb);
	ಆದರೆ (ftruncate(tdb->fd, recovery_eof) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to reduce to recovery size\n"));
		tdb->ecode = TDB_ERR_IO;
		ಹಿಂತಿರುಗಿ -1;
	}
	tdb->map_ಗಾತ್ರ = recovery_eof;
	tdb_mmap(tdb);

	ಆದರೆ (transaction_sync(tdb, 0, recovery_eof) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync2 recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		ಹಿಂತಿರುಗಿ -1;
	}

	TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_recover: recovered %d byte database\n",
		 recovery_eof));

	/* all done */
	ಹಿಂತಿರುಗಿ 0;
}

/* file: freelist.c */

/* read a freelist record and check for simple errors */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_rec_free_read(ರಚನೆ tdb_context *tdb, tdb_off_t off, ರಚನೆ list_struct *rec)
{
	ಆದರೆ (tdb->methods->tdb_read(tdb, off, rec, ಗಾತ್ರ(*rec),DOCONV()) == -1)
		ಹಿಂತಿರುಗಿ -1;

	ಆದರೆ (rec->magic == TDB_MAGIC) {
		/* this happens when a app is showdown while deleting a record - we should
		   not completely fail when this happens */
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read non-free magic 0x%x at offset=%d - fixing\n",
			 rec->magic, off));
		rec->magic = TDB_FREE_MAGIC;
		ಆದರೆ (tdb->methods->tdb_write(tdb, off, rec, ಗಾತ್ರ(*rec)) == -1)
			ಹಿಂತಿರುಗಿ -1;
	}

	ಆದರೆ (rec->magic != TDB_FREE_MAGIC) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read bad magic 0x%x at offset=%d\n",
			   rec->magic, off));
		ಹಿಂತಿರುಗಿ TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	ಆದರೆ (tdb->methods->tdb_oob(tdb, rec->ಮುಂದಿನ+ಗಾತ್ರ(*rec), 0) != 0)
		ಹಿಂತಿರುಗಿ -1;
	ಹಿಂತಿರುಗಿ 0;
}



/* Remove an element from the freelist.  Must have alloc lock. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ remove_from_freelist(ರಚನೆ tdb_context *tdb, tdb_off_t off, tdb_off_t ಮುಂದಿನ)
{
	tdb_off_t ಕೊನೆಯ_ptr, i;

	/* read in the freelist top */
	ಕೊನೆಯ_ptr = FREELIST_ತುದಿ;
	ಆಗ (tdb_ofs_read(tdb, ಕೊನೆಯ_ptr, &i) != -1 && i != 0) {
		ಆದರೆ (i == off) {
			/* We've found it! */
			ಹಿಂತಿರುಗಿ tdb_ofs_write(tdb, ಕೊನೆಯ_ptr, &ಮುಂದಿನ);
		}
		/* Follow chain (next offset is at start of record) */
		ಕೊನೆಯ_ptr = i;
	}
	TDB_LOG((tdb, TDB_DEBUG_FATAL,"remove_from_freelist: not on list at off=%d\n", off));
	ಹಿಂತಿರುಗಿ TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
}


/* update a record tailer (must hold allocation lock) */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ update_tailer(ರಚನೆ tdb_context *tdb, tdb_off_t ಸ್ಥಾನಾಂತರ,
			 ಸ್ಥಿರಾಂಕ ರಚನೆ list_struct *rec)
{
	tdb_off_t totalsize;

	/* Offset of tailer from record header */
	totalsize = ಗಾತ್ರ(*rec) + rec->rec_len;
	ಹಿಂತಿರುಗಿ tdb_ofs_write(tdb, ಸ್ಥಾನಾಂತರ + totalsize - ಗಾತ್ರ(tdb_off_t),
			 &totalsize);
}

/* Add an element into the freelist. Merge adjacent records if
   necessary. */
ಪೂರ್ಣಾಂಕ tdb_free(ರಚನೆ tdb_context *tdb, tdb_off_t ಸ್ಥಾನಾಂತರ, ರಚನೆ list_struct *rec)
{
	tdb_off_t ಬಲ, ಎಡ;

	/* Allocation and tailer lock */
	ಆದರೆ (tdb_ಬೀಗ(tdb, -1, F_WRLCK) != 0)
		ಹಿಂತಿರುಗಿ -1;

	/* set an initial tailer, so if we fail we don't leave a bogus record */
	ಆದರೆ (update_tailer(tdb, ಸ್ಥಾನಾಂತರ, rec) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed!\n"));
		ಹೋಗು fail;
	}

	/* Look right first (I'm an Australian, dammit) */
	ಬಲ = ಸ್ಥಾನಾಂತರ + ಗಾತ್ರ(*rec) + rec->rec_len;
	ಆದರೆ (ಬಲ + ಗಾತ್ರ(*rec) <= tdb->map_ಗಾತ್ರ) {
		ರಚನೆ list_struct r;

		ಆದರೆ (tdb->methods->tdb_read(tdb, ಬಲ, &r, ಗಾತ್ರ(r), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right read failed at %u\n", ಬಲ));
			ಹೋಗು ಎಡ;
		}

		/* If it's free, expand to include it. */
		ಆದರೆ (r.magic == TDB_FREE_MAGIC) {
			ಆದರೆ (remove_from_freelist(tdb, ಬಲ, r.ಮುಂದಿನ) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right free failed at %u\n", ಬಲ));
				ಹೋಗು ಎಡ;
			}
			rec->rec_len += ಗಾತ್ರ(r) + r.rec_len;
		}
	}

ಎಡ:
	/* Look left */
	ಎಡ = ಸ್ಥಾನಾಂತರ - ಗಾತ್ರ(tdb_off_t);
	ಆದರೆ (ಎಡ > TDB_ದತ್ತಾಂಶ_ಆರಂಭ(tdb->ಶೀರ್ಷಿಕೆ.hash_ಗಾತ್ರ)) {
		ರಚನೆ list_struct l;
		tdb_off_t leftsize;

		/* Read in tailer and jump back to header */
		ಆದರೆ (tdb_ofs_read(tdb, ಎಡ, &leftsize) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left offset read failed at %u\n", ಎಡ));
			ಹೋಗು update;
		}

		/* it could be uninitialised data */
		ಆದರೆ (leftsize == 0 || leftsize == TDB_PAD_U32) {
			ಹೋಗು update;
		}

		ಎಡ = ಸ್ಥಾನಾಂತರ - leftsize;

		/* Now read in record */
		ಆದರೆ (tdb->methods->tdb_read(tdb, ಎಡ, &l, ಗಾತ್ರ(l), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left read failed at %u (%u)\n", ಎಡ, leftsize));
			ಹೋಗು update;
		}

		/* If it's free, expand to include it. */
		ಆದರೆ (l.magic == TDB_FREE_MAGIC) {
			ಆದರೆ (remove_from_freelist(tdb, ಎಡ, l.ಮುಂದಿನ) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left free failed at %u\n", ಎಡ));
				ಹೋಗು update;
			} ಇಲ್ಲದಿದ್ದರೆ {
				ಸ್ಥಾನಾಂತರ = ಎಡ;
				rec->rec_len += leftsize;
			}
		}
	}

update:
	ಆದರೆ (update_tailer(tdb, ಸ್ಥಾನಾಂತರ, rec) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed at %u\n", ಸ್ಥಾನಾಂತರ));
		ಹೋಗು fail;
	}

	/* Now, prepend to free list */
	rec->magic = TDB_FREE_MAGIC;

	ಆದರೆ (tdb_ofs_read(tdb, FREELIST_ತುದಿ, &rec->ಮುಂದಿನ) == -1 ||
	    tdb_rec_write(tdb, ಸ್ಥಾನಾಂತರ, rec) == -1 ||
	    tdb_ofs_write(tdb, FREELIST_ತುದಿ, &ಸ್ಥಾನಾಂತರ) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free record write failed at offset=%d\n", ಸ್ಥಾನಾಂತರ));
		ಹೋಗು fail;
	}

	/* And we're done. */
	tdb_unlock(tdb, -1, F_WRLCK);
	ಹಿಂತಿರುಗಿ 0;

 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ಹಿಂತಿರುಗಿ -1;
}


/*
   the core of tdb_allocate - called when we have decided which
   free list entry to use
 */
ಸ್ಥಿರ tdb_off_t tdb_allocate_ofs(ರಚನೆ tdb_context *tdb, tdb_len_t ಉದ್ದಳತೆ, tdb_off_t rec_ptr,
				ರಚನೆ list_struct *rec, tdb_off_t ಕೊನೆಯ_ptr)
{
	ರಚನೆ list_struct newrec;
	tdb_off_t newrec_ptr;

	memset(&newrec, '\0', ಗಾತ್ರ(newrec));

	/* found it - now possibly split it up  */
	ಆದರೆ (rec->rec_len > ಉದ್ದಳತೆ + MIN_REC_ಗಾತ್ರ) {
		/* Length of left piece */
		ಉದ್ದಳತೆ = TDB_ALIGN(ಉದ್ದಳತೆ, TDB_ALIGNMENT);

		/* Right piece to go on free list */
		newrec.rec_len = rec->rec_len - (ಗಾತ್ರ(*rec) + ಉದ್ದಳತೆ);
		newrec_ptr = rec_ptr + ಗಾತ್ರ(*rec) + ಉದ್ದಳತೆ;

		/* And left record is shortened */
		rec->rec_len = ಉದ್ದಳತೆ;
	} ಇಲ್ಲದಿದ್ದರೆ {
		newrec_ptr = 0;
	}

	/* Remove allocated record from the free list */
	ಆದರೆ (tdb_ofs_write(tdb, ಕೊನೆಯ_ptr, &rec->ಮುಂದಿನ) == -1) {
		ಹಿಂತಿರುಗಿ 0;
	}

	/* Update header: do this before we drop alloc
	   lock, otherwise tdb_free() might try to
	   merge with us, thinking we're free.
	   (Thanks Jeremy Allison). */
	rec->magic = TDB_MAGIC;
	ಆದರೆ (tdb_rec_write(tdb, rec_ptr, rec) == -1) {
		ಹಿಂತಿರುಗಿ 0;
	}

	/* Did we create new block? */
	ಆದರೆ (newrec_ptr) {
		/* Update allocated record tailer (we
		   shortened it). */
		ಆದರೆ (update_tailer(tdb, rec_ptr, rec) == -1) {
			ಹಿಂತಿರುಗಿ 0;
		}

		/* Free new record */
		ಆದರೆ (tdb_free(tdb, newrec_ptr, &newrec) == -1) {
			ಹಿಂತಿರುಗಿ 0;
		}
	}

	/* all done - return the new record offset */
	ಹಿಂತಿರುಗಿ rec_ptr;
}

/* allocate some space from the free list. The offset returned points
   to a unconnected list_struct within the database with room for at
   least length bytes of total data

   0 is returned if the space could not be allocated
 */
tdb_off_t tdb_allocate(ರಚನೆ tdb_context *tdb, tdb_len_t ಉದ್ದಳತೆ, ರಚನೆ list_struct *rec)
{
	tdb_off_t rec_ptr, ಕೊನೆಯ_ptr, newrec_ptr;
	ರಚನೆ {
		tdb_off_t rec_ptr, ಕೊನೆಯ_ptr;
		tdb_len_t rec_len;
	} bestfit;

	ಆದರೆ (tdb_ಬೀಗ(tdb, -1, F_WRLCK) == -1)
		ಹಿಂತಿರುಗಿ 0;

	/* Extra bytes required for tailer */
	ಉದ್ದಳತೆ += ಗಾತ್ರ(tdb_off_t);

 again:
	ಕೊನೆಯ_ptr = FREELIST_ತುದಿ;

	/* read in the freelist top */
	ಆದರೆ (tdb_ofs_read(tdb, FREELIST_ತುದಿ, &rec_ptr) == -1)
		ಹೋಗು fail;

	bestfit.rec_ptr = 0;
	bestfit.ಕೊನೆಯ_ptr = 0;
	bestfit.rec_len = 0;

	/*
	   this is a best fit allocation strategy. Originally we used
	   a first fit strategy, but it suffered from massive fragmentation
	   issues when faced with a slowly increasing record size.
	 */
	ಆಗ (rec_ptr) {
		ಆದರೆ (tdb_rec_free_read(tdb, rec_ptr, rec) == -1) {
			ಹೋಗು fail;
		}

		ಆದರೆ (rec->rec_len >= ಉದ್ದಳತೆ) {
			ಆದರೆ (bestfit.rec_ptr == 0 ||
			    rec->rec_len < bestfit.rec_len) {
				bestfit.rec_len = rec->rec_len;
				bestfit.rec_ptr = rec_ptr;
				bestfit.ಕೊನೆಯ_ptr = ಕೊನೆಯ_ptr;
				/* consider a fit to be good enough if
				   we aren't wasting more than half
				   the space */
				ಆದರೆ (bestfit.rec_len < 2*ಉದ್ದಳತೆ) {
					ನಿಲ್ಲಿಸು;
				}
			}
		}

		/* move to the next record */
		ಕೊನೆಯ_ptr = rec_ptr;
		rec_ptr = rec->ಮುಂದಿನ;
	}

	ಆದರೆ (bestfit.rec_ptr != 0) {
		ಆದರೆ (tdb_rec_free_read(tdb, bestfit.rec_ptr, rec) == -1) {
			ಹೋಗು fail;
		}

		newrec_ptr = tdb_allocate_ofs(tdb, ಉದ್ದಳತೆ, bestfit.rec_ptr, rec, bestfit.ಕೊನೆಯ_ptr);
		tdb_unlock(tdb, -1, F_WRLCK);
		ಹಿಂತಿರುಗಿ newrec_ptr;
	}

	/* we didn't find enough space. See if we can expand the
	   database and if we can then try again */
	ಆದರೆ (tdb_expand(tdb, ಉದ್ದಳತೆ + ಗಾತ್ರ(*rec)) == 0)
		ಹೋಗು again;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ಹಿಂತಿರುಗಿ 0;
}

/* file: freelistcheck.c */

/* Check the freelist is good and contains no loops.
   Very memory intensive - only do this as a consistency
   checker. Heh heh - uses an in memory tdb as the storage
   for the "seen" record list. For some reason this strikes
   me as extremely clever as I don't have to write another tree
   data structure implementation :-).
 */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ನೋಡಿದ_ಸೇರಿಸು(ರಚನೆ tdb_context *mem_tdb, tdb_off_t rec_ptr)
{
	TDB_ದತ್ತಾಂಶ ಕೀಲಿ, ದತ್ತಾಂಶ;

	memset(&ದತ್ತಾಂಶ, '\0', ಗಾತ್ರ(ದತ್ತಾಂಶ));
	ಕೀಲಿ.dptr = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)&rec_ptr;
	ಕೀಲಿ.dsize = ಗಾತ್ರ(rec_ptr);
	ಹಿಂತಿರುಗಿ tdb_store(mem_tdb, ಕೀಲಿ, ದತ್ತಾಂಶ, TDB_ಸೇರಿಸು);
}

ಪೂರ್ಣಾಂಕ tdb_validate_freelist(ರಚನೆ tdb_context *tdb, ಪೂರ್ಣಾಂಕ *pnum_entries)
{
	ರಚನೆ tdb_context *mem_tdb = ಖಾಲಿ;
	ರಚನೆ list_struct rec;
	tdb_off_t rec_ptr, ಕೊನೆಯ_ptr;
	ಪೂರ್ಣಾಂಕ ret = -1;

	*pnum_entries = 0;

	mem_tdb = tdb_open("flval", tdb->ಶೀರ್ಷಿಕೆ.hash_ಗಾತ್ರ,
				TDB_ಆಂತರಿಕ, O_RDWR, 0600);
	ಆದರೆ (!mem_tdb) {
		ಹಿಂತಿರುಗಿ -1;
	}

	ಆದರೆ (tdb_ಬೀಗ(tdb, -1, F_WRLCK) == -1) {
		tdb_close(mem_tdb);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಕೊನೆಯ_ptr = FREELIST_ತುದಿ;

	/* Store the FREELIST_TOP record. */
	ಆದರೆ (ನೋಡಿದ_ಸೇರಿಸು(mem_tdb, ಕೊನೆಯ_ptr) == -1) {
		ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
		ಹೋಗು fail;
	}

	/* read in the freelist top */
	ಆದರೆ (tdb_ofs_read(tdb, FREELIST_ತುದಿ, &rec_ptr) == -1) {
		ಹೋಗು fail;
	}

	ಆಗ (rec_ptr) {

		/* If we can't store this record (we've seen it
		   before) then the free list has a loop and must
		   be corrupt. */

		ಆದರೆ (ನೋಡಿದ_ಸೇರಿಸು(mem_tdb, rec_ptr)) {
			ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
			ಹೋಗು fail;
		}

		ಆದರೆ (tdb_rec_free_read(tdb, rec_ptr, &rec) == -1) {
			ಹೋಗು fail;
		}

		/* move to the next record */
		ಕೊನೆಯ_ptr = rec_ptr;
		rec_ptr = rec.ಮುಂದಿನ;
		*pnum_entries += 1;
	}

	ret = 0;

  fail:

	tdb_close(mem_tdb);
	tdb_unlock(tdb, -1, F_WRLCK);
	ಹಿಂತಿರುಗಿ ret;
}

/* file: traverse.c */

/* Uses traverse lock: 0 = finish, -1 = error, other = record offset */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_ಮುಂದಿನ_ಬೀಗ(ರಚನೆ tdb_context *tdb, ರಚನೆ tdb_traverse_ಬೀಗ *tlock,
			 ರಚನೆ list_struct *rec)
{
	ಪೂರ್ಣಾಂಕ want_ಮುಂದಿನ = (tlock->off != 0);

	/* Lock each chain from the start one. */
	ಗಾಗಿ (; tlock->hash < tdb->ಶೀರ್ಷಿಕೆ.hash_ಗಾತ್ರ; tlock->hash++) {
		ಆದರೆ (!tlock->off && tlock->hash != 0) {
			/* this is an optimisation for the common case where
			   the hash chain is empty, which is particularly
			   common for the use of tdb with ldb, where large
			   hashes are used. In that case we spend most of our
			   time in tdb_brlock(), locking empty hash chains.

			   To avoid this, we do an unlocked pre-check to see
			   if the hash chain is empty before starting to look
			   inside it. If it is empty then we can avoid that
			   hash chain. If it isn't empty then we can't believe
			   the value we get back, as we read it without a
			   lock, so instead we get the lock and re-fetch the
			   value below.

			   Notice that not doing this optimisation on the
			   first hash chain is critical. We must guarantee
			   that we have done at least one fcntl lock at the
			   start of a search to guarantee that memory is
			   coherent on SMP systems. If records are added by
			   others during the search then that's OK, and we
			   could possibly miss those with this trick, but we
			   could miss them anyway without this trick, so the
			   semantics don't change.

			   With a non-indexed ldb search this trick gains us a
			   factor of around 80 in speed on a linux 2.6.x
			   system (testing using ldbtest).
			*/
			tdb->methods->ಮುಂದಿನ_hash_chain(tdb, &tlock->hash);
			ಆದರೆ (tlock->hash == tdb->ಶೀರ್ಷಿಕೆ.hash_ಗಾತ್ರ) {
				ಮುಂದುವರಿ;
			}
		}

		ಆದರೆ (tdb_ಬೀಗ(tdb, tlock->hash, tlock->ಬೀಗ_rw) == -1)
			ಹಿಂತಿರುಗಿ -1;

		/* No previous record?  Start at top of chain. */
		ಆದರೆ (!tlock->off) {
			ಆದರೆ (tdb_ofs_read(tdb, TDB_HASH_ತುದಿ(tlock->hash),
				     &tlock->off) == -1)
				ಹೋಗು fail;
		} ಇಲ್ಲದಿದ್ದರೆ {
			/* Otherwise unlock the previous record. */
			ಆದರೆ (tdb_unlock_ದಾಖಲೆ(tdb, tlock->off) != 0)
				ಹೋಗು fail;
		}

		ಆದರೆ (want_ಮುಂದಿನ) {
			/* We have offset of old record: grab next */
			ಆದರೆ (tdb_rec_read(tdb, tlock->off, rec) == -1)
				ಹೋಗು fail;
			tlock->off = rec->ಮುಂದಿನ;
		}

		/* Iterate through chain */
		ಆಗ( tlock->off) {
			tdb_off_t current;
			ಆದರೆ (tdb_rec_read(tdb, tlock->off, rec) == -1)
				ಹೋಗು fail;

			/* Detect infinite loops. From "Shlomi Yaakobovich" <Shlomi@exanet.com>. */
			ಆದರೆ (tlock->off == rec->ಮುಂದಿನ) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: loop detected.\n"));
				ಹೋಗು fail;
			}

			ಆದರೆ (!TDB_DEAD(rec)) {
				/* Woohoo: we found one! */
				ಆದರೆ (tdb_ಬೀಗ_ದಾಖಲೆ(tdb, tlock->off) != 0)
					ಹೋಗು fail;
				ಹಿಂತಿರುಗಿ tlock->off;
			}

			/* Try to clean dead ones from old traverses */
			current = tlock->off;
			tlock->off = rec->ಮುಂದಿನ;
			ಆದರೆ (!(tdb->read_only || tdb->traverse_read) &&
			    tdb_do_delete(tdb, current, rec) != 0)
				ಹೋಗು fail;
		}
		tdb_unlock(tdb, tlock->hash, tlock->ಬೀಗ_rw);
		want_ಮುಂದಿನ = 0;
	}
	/* We finished iteration without finding anything */
	ಹಿಂತಿರುಗಿ TDB_ERRCODE(TDB_SUCCESS, 0);

 fail:
	tlock->off = 0;
	ಆದರೆ (tdb_unlock(tdb, tlock->hash, tlock->ಬೀಗ_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: On error unlock failed!\n"));
	ಹಿಂತಿರುಗಿ -1;
}

/* traverse the entire database - calling fn(tdb, key, data) on each element.
   return -1 on error or the record count traversed
   if fn is NULL then it is not called
   a non-zero return value from fn() indicates that the traversal should stop
  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_traverse_ಆಂತರಿಕ(ರಚನೆ tdb_context *tdb,
				 tdb_traverse_func fn, ಶೂನ್ಯ *ಖಾಸಗಿ_ದತ್ತಾಂಶ,
				 ರಚನೆ tdb_traverse_ಬೀಗ *tl)
{
	TDB_ದತ್ತಾಂಶ ಕೀಲಿ, dbuf;
	ರಚನೆ list_struct rec;
	ಪೂರ್ಣಾಂಕ ret, ಎಣಿಕೆ_2 = 0;

	/* This was in the initialization, above, but the IRIX compiler
	 * did not like it.  crh
	 */
	tl->ಮುಂದಿನ = tdb->travlocks.ಮುಂದಿನ;

	/* fcntl locks don't stack: beware traverse inside traverse */
	tdb->travlocks.ಮುಂದಿನ = tl;

	/* tdb_next_lock places locks on the record returned, and its chain */
	ಆಗ ((ret = tdb_ಮುಂದಿನ_ಬೀಗ(tdb, tl, &rec)) > 0) {
		ಎಣಿಕೆ_2++;
		/* now read the full record */
		ಕೀಲಿ.dptr = tdb_alloc_read(tdb, tl->off + ಗಾತ್ರ(rec),
					  rec.ಕೀಲಿ_len + rec.ದತ್ತಾಂಶ_len);
		ಆದರೆ (!ಕೀಲಿ.dptr) {
			ret = -1;
			ಆದರೆ (tdb_unlock(tdb, tl->hash, tl->ಬೀಗ_rw) != 0)
				ಹೋಗು ಹೊರಹೋಗು;
			ಆದರೆ (tdb_unlock_ದಾಖಲೆ(tdb, tl->off) != 0)
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: key.dptr == NULL and unlock_record failed!\n"));
			ಹೋಗು ಹೊರಹೋಗು;
		}
		ಕೀಲಿ.dsize = rec.ಕೀಲಿ_len;
		dbuf.dptr = ಕೀಲಿ.dptr + rec.ಕೀಲಿ_len;
		dbuf.dsize = rec.ದತ್ತಾಂಶ_len;

		/* Drop chain lock, call out */
		ಆದರೆ (tdb_unlock(tdb, tl->hash, tl->ಬೀಗ_rw) != 0) {
			ret = -1;
			ಸುರಕ್ಷಿತ_FREE(ಕೀಲಿ.dptr);
			ಹೋಗು ಹೊರಹೋಗು;
		}
		ಆದರೆ (fn && fn(tdb, ಕೀಲಿ, dbuf, ಖಾಸಗಿ_ದತ್ತಾಂಶ)) {
			/* They want us to terminate traversal */
			ret = ಎಣಿಕೆ_2;
			ಆದರೆ (tdb_unlock_ದಾಖಲೆ(tdb, tl->off) != 0) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: unlock_record failed!\n"));;
				ret = -1;
			}
			ಸುರಕ್ಷಿತ_FREE(ಕೀಲಿ.dptr);
			ಹೋಗು ಹೊರಹೋಗು;
		}
		ಸುರಕ್ಷಿತ_FREE(ಕೀಲಿ.dptr);
	}
ಹೊರಹೋಗು:
	tdb->travlocks.ಮುಂದಿನ = tl->ಮುಂದಿನ;
	ಆದರೆ (ret < 0)
		ಹಿಂತಿರುಗಿ -1;
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ ಎಣಿಕೆ_2;
}


/*
  a write style traverse - temporarily marks the db read only
*/
ಪೂರ್ಣಾಂಕ tdb_traverse_read(ರಚನೆ tdb_context *tdb,
		      tdb_traverse_func fn, ಶೂನ್ಯ *ಖಾಸಗಿ_ದತ್ತಾಂಶ)
{
	ರಚನೆ tdb_traverse_ಬೀಗ tl = { ಖಾಲಿ, 0, 0, F_RDLCK };
	ಪೂರ್ಣಾಂಕ ret;

	/* we need to get a read lock on the transaction lock here to
	   cope with the lock ordering semantics of solaris10 */
	ಆದರೆ (tdb_transaction_ಬೀಗ(tdb, F_RDLCK)) {
		ಹಿಂತಿರುಗಿ -1;
	}

	tdb->traverse_read++;
	ret = tdb_traverse_ಆಂತರಿಕ(tdb, fn, ಖಾಸಗಿ_ದತ್ತಾಂಶ, &tl);
	tdb->traverse_read--;

	tdb_transaction_unlock(tdb);

	ಹಿಂತಿರುಗಿ ret;
}

/*
  a write style traverse - needs to get the transaction lock to
  prevent deadlocks
*/
ಪೂರ್ಣಾಂಕ tdb_traverse(ರಚನೆ tdb_context *tdb,
		 tdb_traverse_func fn, ಶೂನ್ಯ *ಖಾಸಗಿ_ದತ್ತಾಂಶ)
{
	ರಚನೆ tdb_traverse_ಬೀಗ tl = { ಖಾಲಿ, 0, 0, F_WRLCK };
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (tdb->read_only || tdb->traverse_read) {
		ಹಿಂತಿರುಗಿ tdb_traverse_read(tdb, fn, ಖಾಸಗಿ_ದತ್ತಾಂಶ);
	}

	ಆದರೆ (tdb_transaction_ಬೀಗ(tdb, F_WRLCK)) {
		ಹಿಂತಿರುಗಿ -1;
	}

	ret = tdb_traverse_ಆಂತರಿಕ(tdb, fn, ಖಾಸಗಿ_ದತ್ತಾಂಶ, &tl);

	tdb_transaction_unlock(tdb);

	ಹಿಂತಿರುಗಿ ret;
}


/* find the first entry in the database and return its key */
TDB_ದತ್ತಾಂಶ tdb_firstkey(ರಚನೆ tdb_context *tdb)
{
	TDB_ದತ್ತಾಂಶ ಕೀಲಿ;
	ರಚನೆ list_struct rec;

	/* release any old lock */
	ಆದರೆ (tdb_unlock_ದಾಖಲೆ(tdb, tdb->travlocks.off) != 0)
		ಹಿಂತಿರುಗಿ tdb_null;
	tdb->travlocks.off = tdb->travlocks.hash = 0;
	tdb->travlocks.ಬೀಗ_rw = F_RDLCK;

	/* Grab first record: locks chain and returned record. */
	ಆದರೆ (tdb_ಮುಂದಿನ_ಬೀಗ(tdb, &tdb->travlocks, &rec) <= 0)
		ಹಿಂತಿರುಗಿ tdb_null;
	/* now read the key */
	ಕೀಲಿ.dsize = rec.ಕೀಲಿ_len;
	ಕೀಲಿ.dptr =tdb_alloc_read(tdb,tdb->travlocks.off+ಗಾತ್ರ(rec),ಕೀಲಿ.dsize);

	/* Unlock the hash chain of the record we just read. */
	ಆದರೆ (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.ಬೀಗ_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_firstkey: error occurred while tdb_unlocking!\n"));
	ಹಿಂತಿರುಗಿ ಕೀಲಿ;
}

/* find the next entry in the database, returning its key */
TDB_ದತ್ತಾಂಶ tdb_nextkey(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ oldkey)
{
	u32 oldhash;
	TDB_ದತ್ತಾಂಶ ಕೀಲಿ = tdb_null;
	ರಚನೆ list_struct rec;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *k = ಖಾಲಿ;

	/* Is locked key the old key?  If so, traverse will be reliable. */
	ಆದರೆ (tdb->travlocks.off) {
		ಆದರೆ (tdb_ಬೀಗ(tdb,tdb->travlocks.hash,tdb->travlocks.ಬೀಗ_rw))
			ಹಿಂತಿರುಗಿ tdb_null;
		ಆದರೆ (tdb_rec_read(tdb, tdb->travlocks.off, &rec) == -1
		    || !(k = tdb_alloc_read(tdb,tdb->travlocks.off+ಗಾತ್ರ(rec),
					    rec.ಕೀಲಿ_len))
		    || memcmp(k, oldkey.dptr, oldkey.dsize) != 0) {
			/* No, it wasn't: unlock it and start from scratch */
			ಆದರೆ (tdb_unlock_ದಾಖಲೆ(tdb, tdb->travlocks.off) != 0) {
				ಸುರಕ್ಷಿತ_FREE(k);
				ಹಿಂತಿರುಗಿ tdb_null;
			}
			ಆದರೆ (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.ಬೀಗ_rw) != 0) {
				ಸುರಕ್ಷಿತ_FREE(k);
				ಹಿಂತಿರುಗಿ tdb_null;
			}
			tdb->travlocks.off = 0;
		}

		ಸುರಕ್ಷಿತ_FREE(k);
	}

	ಆದರೆ (!tdb->travlocks.off) {
		/* No previous element: do normal find, and lock record */
		tdb->travlocks.off = tdb_ಕಂಡುಹಿಡಿ_ಬೀಗ_hash(tdb, oldkey, tdb->hash_fn(&oldkey), tdb->travlocks.ಬೀಗ_rw, &rec);
		ಆದರೆ (!tdb->travlocks.off)
			ಹಿಂತಿರುಗಿ tdb_null;
		tdb->travlocks.hash = ಪಾತ್ರೆ(rec.ತುಂಬಿದ_hash);
		ಆದರೆ (tdb_ಬೀಗ_ದಾಖಲೆ(tdb, tdb->travlocks.off) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: lock_record failed (%s)!\n", strerror(errno)));
			ಹಿಂತಿರುಗಿ tdb_null;
		}
	}
	oldhash = tdb->travlocks.hash;

	/* Grab next record: locks chain and returned record,
	   unlocks old record */
	ಆದರೆ (tdb_ಮುಂದಿನ_ಬೀಗ(tdb, &tdb->travlocks, &rec) > 0) {
		ಕೀಲಿ.dsize = rec.ಕೀಲಿ_len;
		ಕೀಲಿ.dptr = tdb_alloc_read(tdb, tdb->travlocks.off+ಗಾತ್ರ(rec),
					  ಕೀಲಿ.dsize);
		/* Unlock the chain of this new record */
		ಆದರೆ (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.ಬೀಗ_rw) != 0)
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	}
	/* Unlock the chain of old record */
	ಆದರೆ (tdb_unlock(tdb, ಪಾತ್ರೆ(oldhash), tdb->travlocks.ಬೀಗ_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	ಹಿಂತಿರುಗಿ ಕೀಲಿ;
}

/* file: dump.c */

ಸ್ಥಿರ tdb_off_t tdb_dump_ದಾಖಲೆ(ರಚನೆ tdb_context *tdb, ಪೂರ್ಣಾಂಕ hash,
				 tdb_off_t ಸ್ಥಾನಾಂತರ)
{
	ರಚನೆ list_struct rec;
	tdb_off_t tailer_ofs, tailer;

	ಆದರೆ (tdb->methods->tdb_read(tdb, ಸ್ಥಾನಾಂತರ, (ಅಕ್ಷರ *)&rec,
				   ಗಾತ್ರ(rec), DOCONV()) == -1) {
		ಬರೆ("ERROR: failed to read record at %u\n", ಸ್ಥಾನಾಂತರ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಬರೆ(" rec: hash=%d offset=0x%08x next=0x%08x rec_len=%d "
	       "key_len=%d data_len=%d full_hash=0x%x magic=0x%x\n",
	       hash, ಸ್ಥಾನಾಂತರ, rec.ಮುಂದಿನ, rec.rec_len, rec.ಕೀಲಿ_len, rec.ದತ್ತಾಂಶ_len,
	       rec.ತುಂಬಿದ_hash, rec.magic);

	tailer_ofs = ಸ್ಥಾನಾಂತರ + ಗಾತ್ರ(rec) + rec.rec_len - ಗಾತ್ರ(tdb_off_t);

	ಆದರೆ (tdb_ofs_read(tdb, tailer_ofs, &tailer) == -1) {
		ಬರೆ("ERROR: failed to read tailer at %u\n", tailer_ofs);
		ಹಿಂತಿರುಗಿ rec.ಮುಂದಿನ;
	}

	ಆದರೆ (tailer != rec.rec_len + ಗಾತ್ರ(rec)) {
		ಬರೆ("ERROR: tailer does not match record! tailer=%u totalsize=%u\n",
				(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)tailer, (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)(rec.rec_len + ಗಾತ್ರ(rec)));
	}
	ಹಿಂತಿರುಗಿ rec.ಮುಂದಿನ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_dump_chain(ರಚನೆ tdb_context *tdb, ಪೂರ್ಣಾಂಕ i)
{
	tdb_off_t rec_ptr, ತುದಿ;

	ತುದಿ = TDB_HASH_ತುದಿ(i);

	ಆದರೆ (tdb_ಬೀಗ(tdb, i, F_WRLCK) != 0)
		ಹಿಂತಿರುಗಿ -1;

	ಆದರೆ (tdb_ofs_read(tdb, ತುದಿ, &rec_ptr) == -1)
		ಹಿಂತಿರುಗಿ tdb_unlock(tdb, i, F_WRLCK);

	ಆದರೆ (rec_ptr)
		ಬರೆ("hash=%d\n", i);

	ಆಗ (rec_ptr) {
		rec_ptr = tdb_dump_ದಾಖಲೆ(tdb, i, rec_ptr);
	}

	ಹಿಂತಿರುಗಿ tdb_unlock(tdb, i, F_WRLCK);
}

ಶೂನ್ಯ tdb_dump_all(ರಚನೆ tdb_context *tdb)
{
	ಪೂರ್ಣಾಂಕ i;
	ಗಾಗಿ (i = 0; i < (ಪೂರ್ಣಾಂಕ)tdb->ಶೀರ್ಷಿಕೆ.hash_ಗಾತ್ರ; i++) {
		tdb_dump_chain(tdb, i);
	}
	ಬರೆ("freelist:\n");
	tdb_dump_chain(tdb, -1);
}

ಪೂರ್ಣಾಂಕ tdb_printfreelist(ರಚನೆ tdb_context *tdb)
{
	ಪೂರ್ಣಾಂಕ ret;
	ದೀರ್ಘ ಒಟ್ಟು_free = 0;
	tdb_off_t ಸ್ಥಾನಾಂತರ, rec_ptr;
	ರಚನೆ list_struct rec;

	ಆದರೆ ((ret = tdb_ಬೀಗ(tdb, -1, F_WRLCK)) != 0)
		ಹಿಂತಿರುಗಿ ret;

	ಸ್ಥಾನಾಂತರ = FREELIST_ತುದಿ;

	/* read in the freelist top */
	ಆದರೆ (tdb_ofs_read(tdb, ಸ್ಥಾನಾಂತರ, &rec_ptr) == -1) {
		tdb_unlock(tdb, -1, F_WRLCK);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಬರೆ("freelist top=[0x%08x]\n", rec_ptr );
	ಆಗ (rec_ptr) {
		ಆದರೆ (tdb->methods->tdb_read(tdb, rec_ptr, (ಅಕ್ಷರ *)&rec,
					   ಗಾತ್ರ(rec), DOCONV()) == -1) {
			tdb_unlock(tdb, -1, F_WRLCK);
			ಹಿಂತಿರುಗಿ -1;
		}

		ಆದರೆ (rec.magic != TDB_FREE_MAGIC) {
			ಬರೆ("bad magic 0x%08x in free list\n", rec.magic);
			tdb_unlock(tdb, -1, F_WRLCK);
			ಹಿಂತಿರುಗಿ -1;
		}

		ಬರೆ("entry offset=[0x%08x], rec.rec_len = [0x%08x (%d)] (end = 0x%08x)\n",
		       rec_ptr, rec.rec_len, rec.rec_len, rec_ptr + rec.rec_len);
		ಒಟ್ಟು_free += rec.rec_len;

		/* move to the next record */
		rec_ptr = rec.ಮುಂದಿನ;
	}
	ಬರೆ("total rec_len = [0x%08x (%d)]\n", (ಪೂರ್ಣಾಂಕ)ಒಟ್ಟು_free,
               (ಪೂರ್ಣಾಂಕ)ಒಟ್ಟು_free);

	ಹಿಂತಿರುಗಿ tdb_unlock(tdb, -1, F_WRLCK);
}

/* file: tdb.c */

/*
  non-blocking increment of the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
ಶೂನ್ಯ tdb_increment_seqnum_nonblock(ರಚನೆ tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	ಆದರೆ (!(tdb->flags & TDB_SEQNUM)) {
		ಹಿಂತಿರುಗಿ;
	}

	/* we ignore errors from this, as we have no sane way of
	   dealing with them.
	*/
	ಆದರೆ (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		ಹಿಂತಿರುಗಿ;
	seqnum++;
	(ಶೂನ್ಯ) tdb_ofs_write(tdb, TDB_SEQNUM_OFS, &seqnum);
}

/*
  increment the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
ಸ್ಥಿರ ಶೂನ್ಯ tdb_increment_seqnum(ರಚನೆ tdb_context *tdb)
{
	ಆದರೆ (!(tdb->flags & TDB_SEQNUM)) {
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (tdb_brlock(tdb, TDB_SEQNUM_OFS, F_WRLCK, F_SETLKW, 1, 1) != 0) {
		ಹಿಂತಿರುಗಿ;
	}

	tdb_increment_seqnum_nonblock(tdb);

	tdb_brlock(tdb, TDB_SEQNUM_OFS, F_UNLCK, F_SETLKW, 1, 1);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_ಕೀಲಿ_compare(TDB_ದತ್ತಾಂಶ ಕೀಲಿ, TDB_ದತ್ತಾಂಶ ದತ್ತಾಂಶ,
			   ಶೂನ್ಯ *ಖಾಸಗಿ_ದತ್ತಾಂಶ EXT2FS_ATTR((ಬಳಸದ)))
{
	ಹಿಂತಿರುಗಿ memcmp(ದತ್ತಾಂಶ.dptr, ಕೀಲಿ.dptr, ದತ್ತಾಂಶ.dsize);
}

/* Returns 0 on fail.  On success, return offset of record, and fills
   in rec */
ಸ್ಥಿರ tdb_off_t tdb_ಕಂಡುಹಿಡಿ(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ, u32 hash,
			ರಚನೆ list_struct *r)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	ಆದರೆ (tdb_ofs_read(tdb, TDB_HASH_ತುದಿ(hash), &rec_ptr) == -1)
		ಹಿಂತಿರುಗಿ 0;

	/* keep looking until we find the right record */
	ಆಗ (rec_ptr) {
		ಆದರೆ (tdb_rec_read(tdb, rec_ptr, r) == -1)
			ಹಿಂತಿರುಗಿ 0;

		ಆದರೆ (!TDB_DEAD(r) && hash==r->ತುಂಬಿದ_hash
		    && ಕೀಲಿ.dsize==r->ಕೀಲಿ_len
		    && tdb_parse_ದತ್ತಾಂಶ(tdb, ಕೀಲಿ, rec_ptr + ಗಾತ್ರ(*r),
				      r->ಕೀಲಿ_len, tdb_ಕೀಲಿ_compare,
				      ಖಾಲಿ) == 0) {
			ಹಿಂತಿರುಗಿ rec_ptr;
		}
		rec_ptr = r->ಮುಂದಿನ;
	}
	ಹಿಂತಿರುಗಿ TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
}

/* As tdb_find, but if you succeed, keep the lock */
tdb_off_t tdb_ಕಂಡುಹಿಡಿ_ಬೀಗ_hash(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ, u32 hash, ಪೂರ್ಣಾಂಕ locktype,
			   ರಚನೆ list_struct *rec)
{
	u32 rec_ptr;

	ಆದರೆ (tdb_ಬೀಗ(tdb, ಪಾತ್ರೆ(hash), locktype) == -1)
		ಹಿಂತಿರುಗಿ 0;
	ಆದರೆ (!(rec_ptr = tdb_ಕಂಡುಹಿಡಿ(tdb, ಕೀಲಿ, hash, rec)))
		tdb_unlock(tdb, ಪಾತ್ರೆ(hash), locktype);
	ಹಿಂತಿರುಗಿ rec_ptr;
}


/* update an entry in place - this only works if the new data size
   is <= the old data size and the key exists.
   on failure return -1.
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_update_hash(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ, u32 hash, TDB_ದತ್ತಾಂಶ dbuf)
{
	ರಚನೆ list_struct rec;
	tdb_off_t rec_ptr;

	/* find entry */
	ಆದರೆ (!(rec_ptr = tdb_ಕಂಡುಹಿಡಿ(tdb, ಕೀಲಿ, hash, &rec)))
		ಹಿಂತಿರುಗಿ -1;

	/* must be long enough key, data and tailer */
	ಆದರೆ (rec.rec_len < ಕೀಲಿ.dsize + dbuf.dsize + ಗಾತ್ರ(tdb_off_t)) {
		tdb->ecode = TDB_SUCCESS; /* Not really an error */
		ಹಿಂತಿರುಗಿ -1;
	}

	ಆದರೆ (tdb->methods->tdb_write(tdb, rec_ptr + ಗಾತ್ರ(rec) + rec.ಕೀಲಿ_len,
		      dbuf.dptr, dbuf.dsize) == -1)
		ಹಿಂತಿರುಗಿ -1;

	ಆದರೆ (dbuf.dsize != rec.ದತ್ತಾಂಶ_len) {
		/* update size */
		rec.ದತ್ತಾಂಶ_len = dbuf.dsize;
		ಹಿಂತಿರುಗಿ tdb_rec_write(tdb, rec_ptr, &rec);
	}

	ಹಿಂತಿರುಗಿ 0;
}

/* find an entry in the database given a key */
/* If an entry doesn't exist tdb_err will be set to
 * TDB_ERR_NOEXIST. If a key has no data attached
 * then the TDB_DATA will have zero length but
 * a non-zero pointer
 */
TDB_ದತ್ತಾಂಶ tdb_fetch(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ)
{
	tdb_off_t rec_ptr;
	ರಚನೆ list_struct rec;
	TDB_ದತ್ತಾಂಶ ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&ಕೀಲಿ);
	ಆದರೆ (!(rec_ptr = tdb_ಕಂಡುಹಿಡಿ_ಬೀಗ_hash(tdb,ಕೀಲಿ,hash,F_RDLCK,&rec)))
		ಹಿಂತಿರುಗಿ tdb_null;

	ret.dptr = tdb_alloc_read(tdb, rec_ptr + ಗಾತ್ರ(rec) + rec.ಕೀಲಿ_len,
				  rec.ದತ್ತಾಂಶ_len);
	ret.dsize = rec.ದತ್ತಾಂಶ_len;
	tdb_unlock(tdb, ಪಾತ್ರೆ(rec.ತುಂಬಿದ_hash), F_RDLCK);
	ಹಿಂತಿರುಗಿ ret;
}

/*
 * Find an entry in the database and hand the record's data to a parsing
 * function. The parsing function is executed under the chain read lock, so it
 * should be fast and should not block on other syscalls.
 *
 * DONT CALL OTHER TDB CALLS FROM THE PARSER, THIS MIGHT LEAD TO SEGFAULTS.
 *
 * For mmapped tdb's that do not have a transaction open it points the parsing
 * function directly at the mmap area, it avoids the malloc/memcpy in this
 * case. If a transaction is open or no mmap is available, it has to do
 * malloc/read/parse/free.
 *
 * This is interesting for all readers of potentially large data structures in
 * the tdb records, ldb indexes being one example.
 */

ಪೂರ್ಣಾಂಕ tdb_parse_ದಾಖಲೆ(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ,
		     ಪೂರ್ಣಾಂಕ (*parser)(TDB_ದತ್ತಾಂಶ ಕೀಲಿ, TDB_ದತ್ತಾಂಶ ದತ್ತಾಂಶ,
				   ಶೂನ್ಯ *ಖಾಸಗಿ_ದತ್ತಾಂಶ),
		     ಶೂನ್ಯ *ಖಾಸಗಿ_ದತ್ತಾಂಶ)
{
	tdb_off_t rec_ptr;
	ರಚನೆ list_struct rec;
	ಪೂರ್ಣಾಂಕ ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&ಕೀಲಿ);

	ಆದರೆ (!(rec_ptr = tdb_ಕಂಡುಹಿಡಿ_ಬೀಗ_hash(tdb,ಕೀಲಿ,hash,F_RDLCK,&rec))) {
		ಹಿಂತಿರುಗಿ TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
	}

	ret = tdb_parse_ದತ್ತಾಂಶ(tdb, ಕೀಲಿ, rec_ptr + ಗಾತ್ರ(rec) + rec.ಕೀಲಿ_len,
			     rec.ದತ್ತಾಂಶ_len, parser, ಖಾಸಗಿ_ದತ್ತಾಂಶ);

	tdb_unlock(tdb, ಪಾತ್ರೆ(rec.ತುಂಬಿದ_hash), F_RDLCK);

	ಹಿಂತಿರುಗಿ ret;
}

/* check if an entry in the database exists

   note that 1 is returned if the key is found and 0 is returned if not found
   this doesn't match the conventions in the rest of this module, but is
   compatible with gdbm
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_exists_hash(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ, u32 hash)
{
	ರಚನೆ list_struct rec;

	ಆದರೆ (tdb_ಕಂಡುಹಿಡಿ_ಬೀಗ_hash(tdb, ಕೀಲಿ, hash, F_RDLCK, &rec) == 0)
		ಹಿಂತಿರುಗಿ 0;
	tdb_unlock(tdb, ಪಾತ್ರೆ(rec.ತುಂಬಿದ_hash), F_RDLCK);
	ಹಿಂತಿರುಗಿ 1;
}

ಪೂರ್ಣಾಂಕ tdb_exists(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ)
{
	u32 hash = tdb->hash_fn(&ಕೀಲಿ);
	ಹಿಂತಿರುಗಿ tdb_exists_hash(tdb, ಕೀಲಿ, hash);
}

/* actually delete an entry in the database given the offset */
ಪೂರ್ಣಾಂಕ tdb_do_delete(ರಚನೆ tdb_context *tdb, tdb_off_t rec_ptr, ರಚನೆ list_struct*rec)
{
	tdb_off_t ಕೊನೆಯ_ptr, i;
	ರಚನೆ list_struct lastrec;

	ಆದರೆ (tdb->read_only || tdb->traverse_read) ಹಿಂತಿರುಗಿ -1;

	ಆದರೆ (tdb_write_ಬೀಗ_ದಾಖಲೆ(tdb, rec_ptr) == -1) {
		/* Someone traversing here: mark it as dead */
		rec->magic = TDB_DEAD_MAGIC;
		ಹಿಂತಿರುಗಿ tdb_rec_write(tdb, rec_ptr, rec);
	}
	ಆದರೆ (tdb_write_unlock_ದಾಖಲೆ(tdb, rec_ptr) != 0)
		ಹಿಂತಿರುಗಿ -1;

	/* find previous record in hash chain */
	ಆದರೆ (tdb_ofs_read(tdb, TDB_HASH_ತುದಿ(rec->ತುಂಬಿದ_hash), &i) == -1)
		ಹಿಂತಿರುಗಿ -1;
	ಗಾಗಿ (ಕೊನೆಯ_ptr = 0; i != rec_ptr; ಕೊನೆಯ_ptr = i, i = lastrec.ಮುಂದಿನ)
		ಆದರೆ (tdb_rec_read(tdb, i, &lastrec) == -1)
			ಹಿಂತಿರುಗಿ -1;

	/* unlink it: next ptr is at start of record. */
	ಆದರೆ (ಕೊನೆಯ_ptr == 0)
		ಕೊನೆಯ_ptr = TDB_HASH_ತುದಿ(rec->ತುಂಬಿದ_hash);
	ಆದರೆ (tdb_ofs_write(tdb, ಕೊನೆಯ_ptr, &rec->ಮುಂದಿನ) == -1)
		ಹಿಂತಿರುಗಿ -1;

	/* recover the space */
	ಆದರೆ (tdb_free(tdb, rec_ptr, rec) == -1)
		ಹಿಂತಿರುಗಿ -1;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_ಎಣಿಕೆ_dead(ರಚನೆ tdb_context *tdb, u32 hash)
{
	ಪೂರ್ಣಾಂಕ res = 0;
	tdb_off_t rec_ptr;
	ರಚನೆ list_struct rec;

	/* read in the hash top */
	ಆದರೆ (tdb_ofs_read(tdb, TDB_HASH_ತುದಿ(hash), &rec_ptr) == -1)
		ಹಿಂತಿರುಗಿ 0;

	ಆಗ (rec_ptr) {
		ಆದರೆ (tdb_rec_read(tdb, rec_ptr, &rec) == -1)
			ಹಿಂತಿರುಗಿ 0;

		ಆದರೆ (rec.magic == TDB_DEAD_MAGIC) {
			res += 1;
		}
		rec_ptr = rec.ಮುಂದಿನ;
	}
	ಹಿಂತಿರುಗಿ res;
}

/*
 * Purge all DEAD records from a hash chain
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_purge_dead(ರಚನೆ tdb_context *tdb, u32 hash)
{
	ಪೂರ್ಣಾಂಕ res = -1;
	ರಚನೆ list_struct rec;
	tdb_off_t rec_ptr;

	ಆದರೆ (tdb_ಬೀಗ(tdb, -1, F_WRLCK) == -1) {
		ಹಿಂತಿರುಗಿ -1;
	}

	/* read in the hash top */
	ಆದರೆ (tdb_ofs_read(tdb, TDB_HASH_ತುದಿ(hash), &rec_ptr) == -1)
		ಹೋಗು fail;

	ಆಗ (rec_ptr) {
		tdb_off_t ಮುಂದಿನ;

		ಆದರೆ (tdb_rec_read(tdb, rec_ptr, &rec) == -1) {
			ಹೋಗು fail;
		}

		ಮುಂದಿನ = rec.ಮುಂದಿನ;

		ಆದರೆ (rec.magic == TDB_DEAD_MAGIC
		    && tdb_do_delete(tdb, rec_ptr, &rec) == -1) {
			ಹೋಗು fail;
		}
		rec_ptr = ಮುಂದಿನ;
	}
	res = 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ಹಿಂತಿರುಗಿ res;
}

/* delete an entry in the database given a key */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_delete_hash(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ, u32 hash)
{
	tdb_off_t rec_ptr;
	ರಚನೆ list_struct rec;
	ಪೂರ್ಣಾಂಕ ret;

	ಆದರೆ (tdb->max_dead_ದಾಖಲೆಗಳು != 0) {

		/*
		 * Allow for some dead records per hash chain, mainly for
		 * tdb's with a very high create/delete rate like locking.tdb.
		 */

		ಆದರೆ (tdb_ಬೀಗ(tdb, ಪಾತ್ರೆ(hash), F_WRLCK) == -1)
			ಹಿಂತಿರುಗಿ -1;

		ಆದರೆ (tdb_ಎಣಿಕೆ_dead(tdb, hash) >= tdb->max_dead_ದಾಖಲೆಗಳು) {
			/*
			 * Don't let the per-chain freelist grow too large,
			 * delete all existing dead records
			 */
			tdb_purge_dead(tdb, hash);
		}

		ಆದರೆ (!(rec_ptr = tdb_ಕಂಡುಹಿಡಿ(tdb, ಕೀಲಿ, hash, &rec))) {
			tdb_unlock(tdb, ಪಾತ್ರೆ(hash), F_WRLCK);
			ಹಿಂತಿರುಗಿ -1;
		}

		/*
		 * Just mark the record as dead.
		 */
		rec.magic = TDB_DEAD_MAGIC;
		ret = tdb_rec_write(tdb, rec_ptr, &rec);
	}
	ಇಲ್ಲದಿದ್ದರೆ {
		ಆದರೆ (!(rec_ptr = tdb_ಕಂಡುಹಿಡಿ_ಬೀಗ_hash(tdb, ಕೀಲಿ, hash, F_WRLCK,
						   &rec)))
			ಹಿಂತಿರುಗಿ -1;

		ret = tdb_do_delete(tdb, rec_ptr, &rec);
	}

	ಆದರೆ (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	ಆದರೆ (tdb_unlock(tdb, ಪಾತ್ರೆ(rec.ತುಂಬಿದ_hash), F_WRLCK) != 0)
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_delete: WARNING tdb_unlock failed!\n"));
	ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ tdb_delete(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ)
{
	u32 hash = tdb->hash_fn(&ಕೀಲಿ);
	ಹಿಂತಿರುಗಿ tdb_delete_hash(tdb, ಕೀಲಿ, hash);
}

/*
 * See if we have a dead record around with enough space
 */
ಸ್ಥಿರ tdb_off_t tdb_ಕಂಡುಹಿಡಿ_dead(ರಚನೆ tdb_context *tdb, u32 hash,
			       ರಚನೆ list_struct *r, tdb_len_t ಉದ್ದಳತೆ)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	ಆದರೆ (tdb_ofs_read(tdb, TDB_HASH_ತುದಿ(hash), &rec_ptr) == -1)
		ಹಿಂತಿರುಗಿ 0;

	/* keep looking until we find the right record */
	ಆಗ (rec_ptr) {
		ಆದರೆ (tdb_rec_read(tdb, rec_ptr, r) == -1)
			ಹಿಂತಿರುಗಿ 0;

		ಆದರೆ (TDB_DEAD(r) && r->rec_len >= ಉದ್ದಳತೆ) {
			/*
			 * First fit for simple coding, TODO: change to best
			 * fit
			 */
			ಹಿಂತಿರುಗಿ rec_ptr;
		}
		rec_ptr = r->ಮುಂದಿನ;
	}
	ಹಿಂತಿರುಗಿ 0;
}

/* store an element in the database, replacing any existing element
   with the same key

   return 0 on success, -1 on failure
*/
ಪೂರ್ಣಾಂಕ tdb_store(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ, TDB_ದತ್ತಾಂಶ dbuf, ಪೂರ್ಣಾಂಕ flag)
{
	ರಚನೆ list_struct rec;
	u32 hash;
	tdb_off_t rec_ptr;
	ಅಕ್ಷರ *p = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ ret = -1;

	ಆದರೆ (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		ಹಿಂತಿರುಗಿ -1;
	}

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&ಕೀಲಿ);
	ಆದರೆ (tdb_ಬೀಗ(tdb, ಪಾತ್ರೆ(hash), F_WRLCK) == -1)
		ಹಿಂತಿರುಗಿ -1;

	/* check for it existing, on insert. */
	ಆದರೆ (flag == TDB_ಸೇರಿಸು) {
		ಆದರೆ (tdb_exists_hash(tdb, ಕೀಲಿ, hash)) {
			tdb->ecode = TDB_ERR_EXISTS;
			ಹೋಗು fail;
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* first try in-place update, on modify or replace. */
		ಆದರೆ (tdb_update_hash(tdb, ಕೀಲಿ, hash, dbuf) == 0) {
			ಹೋಗು ಮುಗಿದ;
		}
		ಆದರೆ (tdb->ecode == TDB_ERR_NOEXIST &&
		    flag == TDB_MODIFY) {
			/* if the record doesn't exist and we are in TDB_MODIFY mode then
			 we should fail the store */
			ಹೋಗು fail;
		}
	}
	/* reset the error code potentially set by the tdb_update() */
	tdb->ecode = TDB_SUCCESS;

	/* delete any existing record - if it doesn't exist we don't
           care.  Doing this first reduces fragmentation, and avoids
           coalescing with `allocated' block before it's updated. */
	ಆದರೆ (flag != TDB_ಸೇರಿಸು)
		tdb_delete_hash(tdb, ಕೀಲಿ, hash);

	/* Copy key+value *before* allocating free space in case malloc
	   fails and we are left with a dead spot in the tdb. */

	ಆದರೆ (!(p = (ಅಕ್ಷರ *)ಸ್ಮರಣೆ_ಕೊಡು(ಕೀಲಿ.dsize + dbuf.dsize))) {
		tdb->ecode = TDB_ERR_OOM;
		ಹೋಗು fail;
	}

	ಸ್ಮರಣೆ_ನಕಲು(p, ಕೀಲಿ.dptr, ಕೀಲಿ.dsize);
	ಆದರೆ (dbuf.dsize)
		ಸ್ಮರಣೆ_ನಕಲು(p+ಕೀಲಿ.dsize, dbuf.dptr, dbuf.dsize);

	ಆದರೆ (tdb->max_dead_ದಾಖಲೆಗಳು != 0) {
		/*
		 * Allow for some dead records per hash chain, look if we can
		 * find one that can hold the new record. We need enough space
		 * for key, data and tailer. If we find one, we don't have to
		 * consult the central freelist.
		 */
		rec_ptr = tdb_ಕಂಡುಹಿಡಿ_dead(
			tdb, hash, &rec,
			ಕೀಲಿ.dsize + dbuf.dsize + ಗಾತ್ರ(tdb_off_t));

		ಆದರೆ (rec_ptr != 0) {
			rec.ಕೀಲಿ_len = ಕೀಲಿ.dsize;
			rec.ದತ್ತಾಂಶ_len = dbuf.dsize;
			rec.ತುಂಬಿದ_hash = hash;
			rec.magic = TDB_MAGIC;
			ಆದರೆ (tdb_rec_write(tdb, rec_ptr, &rec) == -1
			    || tdb->methods->tdb_write(
				    tdb, rec_ptr + ಗಾತ್ರ(rec),
				    p, ಕೀಲಿ.dsize + dbuf.dsize) == -1) {
				ಹೋಗು fail;
			}
			ಹೋಗು ಮುಗಿದ;
		}
	}

	/*
	 * We have to allocate some space from the freelist, so this means we
	 * have to lock it. Use the chance to purge all the DEAD records from
	 * the hash chain under the freelist lock.
	 */

	ಆದರೆ (tdb_ಬೀಗ(tdb, -1, F_WRLCK) == -1) {
		ಹೋಗು fail;
	}

	ಆದರೆ ((tdb->max_dead_ದಾಖಲೆಗಳು != 0)
	    && (tdb_purge_dead(tdb, hash) == -1)) {
		tdb_unlock(tdb, -1, F_WRLCK);
		ಹೋಗು fail;
	}

	/* we have to allocate some space */
	rec_ptr = tdb_allocate(tdb, ಕೀಲಿ.dsize + dbuf.dsize, &rec);

	tdb_unlock(tdb, -1, F_WRLCK);

	ಆದರೆ (rec_ptr == 0) {
		ಹೋಗು fail;
	}

	/* Read hash top into next ptr */
	ಆದರೆ (tdb_ofs_read(tdb, TDB_HASH_ತುದಿ(hash), &rec.ಮುಂದಿನ) == -1)
		ಹೋಗು fail;

	rec.ಕೀಲಿ_len = ಕೀಲಿ.dsize;
	rec.ದತ್ತಾಂಶ_len = dbuf.dsize;
	rec.ತುಂಬಿದ_hash = hash;
	rec.magic = TDB_MAGIC;

	/* write out and point the top of the hash chain at it */
	ಆದರೆ (tdb_rec_write(tdb, rec_ptr, &rec) == -1
	    || tdb->methods->tdb_write(tdb, rec_ptr+ಗಾತ್ರ(rec), p, ಕೀಲಿ.dsize+dbuf.dsize)==-1
	    || tdb_ofs_write(tdb, TDB_HASH_ತುದಿ(hash), &rec_ptr) == -1) {
		/* Need to tdb_unallocate() here */
		ಹೋಗು fail;
	}

 ಮುಗಿದ:
	ret = 0;
 fail:
	ಆದರೆ (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	ಸುರಕ್ಷಿತ_FREE(p);
	tdb_unlock(tdb, ಪಾತ್ರೆ(hash), F_WRLCK);
	ಹಿಂತಿರುಗಿ ret;
}


/* Append to an entry. Create if not exist. */
ಪೂರ್ಣಾಂಕ tdb_append(ರಚನೆ tdb_context *tdb, TDB_ದತ್ತಾಂಶ ಕೀಲಿ, TDB_ದತ್ತಾಂಶ new_dbuf)
{
	u32 hash;
	TDB_ದತ್ತಾಂಶ dbuf;
	ಪೂರ್ಣಾಂಕ ret = -1;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&ಕೀಲಿ);
	ಆದರೆ (tdb_ಬೀಗ(tdb, ಪಾತ್ರೆ(hash), F_WRLCK) == -1)
		ಹಿಂತಿರುಗಿ -1;

	dbuf = tdb_fetch(tdb, ಕೀಲಿ);

	ಆದರೆ (dbuf.dptr == ಖಾಲಿ) {
		dbuf.dptr = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)ಸ್ಮರಣೆ_ಕೊಡು(new_dbuf.dsize);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *new_dptr = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)realloc(dbuf.dptr,
						     dbuf.dsize + new_dbuf.dsize);
		ಆದರೆ (new_dptr == ಖಾಲಿ) {
			ಬಿಡುಗಡೆ(dbuf.dptr);
		}
		dbuf.dptr = new_dptr;
	}

	ಆದರೆ (dbuf.dptr == ಖಾಲಿ) {
		tdb->ecode = TDB_ERR_OOM;
		ಹೋಗು failed;
	}

	ಸ್ಮರಣೆ_ನಕಲು(dbuf.dptr + dbuf.dsize, new_dbuf.dptr, new_dbuf.dsize);
	dbuf.dsize += new_dbuf.dsize;

	ret = tdb_store(tdb, ಕೀಲಿ, dbuf, 0);

failed:
	tdb_unlock(tdb, ಪಾತ್ರೆ(hash), F_WRLCK);
	ಸುರಕ್ಷಿತ_FREE(dbuf.dptr);
	ಹಿಂತಿರುಗಿ ret;
}


/*
  return the name of the current tdb file
  useful for external logging functions
*/
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *tdb_name(ರಚನೆ tdb_context *tdb)
{
	ಹಿಂತಿರುಗಿ tdb->name;
}

/*
  return the underlying file descriptor being used by tdb, or -1
  useful for external routines that want to check the device/inode
  of the fd
*/
ಪೂರ್ಣಾಂಕ tdb_fd(ರಚನೆ tdb_context *tdb)
{
	ಹಿಂತಿರುಗಿ tdb->fd;
}

/*
  return the current logging function
  useful for external tdb routines that wish to log tdb errors
*/
tdb_log_func tdb_log_fn(ರಚನೆ tdb_context *tdb)
{
	ಹಿಂತಿರುಗಿ tdb->ಲಘುಗಣಕ.log_fn;
}


/*
  get the tdb sequence number. Only makes sense if the writers opened
  with TDB_SEQNUM set. Note that this sequence number will wrap quite
  quickly, so it should only be used for a 'has something changed'
  test, not for code that relies on the count of the number of changes
  made. If you want a counter then use a tdb record.

  The aim of this sequence number is to allow for a very lightweight
  test of a possible tdb change.
*/
ಪೂರ್ಣಾಂಕ tdb_get_seqnum(ರಚನೆ tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	ಆದರೆ (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ seqnum;
}

ಪೂರ್ಣಾಂಕ tdb_hash_ಗಾತ್ರ(ರಚನೆ tdb_context *tdb)
{
	ಹಿಂತಿರುಗಿ tdb->ಶೀರ್ಷಿಕೆ.hash_ಗಾತ್ರ;
}

size_t tdb_map_ಗಾತ್ರ(ರಚನೆ tdb_context *tdb)
{
	ಹಿಂತಿರುಗಿ tdb->map_ಗಾತ್ರ;
}

ಪೂರ್ಣಾಂಕ tdb_get_flags(ರಚನೆ tdb_context *tdb)
{
	ಹಿಂತಿರುಗಿ tdb->flags;
}


/*
  enable sequence number handling on an open tdb
*/
ಶೂನ್ಯ tdb_enable_seqnum(ರಚನೆ tdb_context *tdb)
{
	tdb->flags |= TDB_SEQNUM;
}

/* file: open.c */

/* all contexts, to ensure no double-opens (fcntl locks don't nest!) */
ಸ್ಥಿರ ರಚನೆ tdb_context *tdbs = ಖಾಲಿ;


/* This is from a hash algorithm suggested by Rogier Wolff */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ default_tdb_hash(TDB_ದತ್ತಾಂಶ *ಕೀಲಿ)
{
	u32 ಮೌಲ್ಯ;	/* Used to compute the hash value.  */
	u32   i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	ಗಾಗಿ (ಮೌಲ್ಯ = 0, i=0; i < ಕೀಲಿ->dsize; i++)
		ಮೌಲ್ಯ = ಮೌಲ್ಯ * 256 + ಕೀಲಿ->dptr[i] + (ಮೌಲ್ಯ >> 24) * 241;

	ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
}


/* initialise a new database with a specified hash size */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_new_database(ರಚನೆ tdb_context *tdb, ಪೂರ್ಣಾಂಕ hash_ಗಾತ್ರ)
{
	ರಚನೆ tdb_ಶೀರ್ಷಿಕೆ *newdb;
	ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ret = -1;

	/* We make it up in memory, then write it out if not internal */
	ಗಾತ್ರ_2 = ಗಾತ್ರ(ರಚನೆ tdb_ಶೀರ್ಷಿಕೆ) + (hash_ಗಾತ್ರ+1)*ಗಾತ್ರ(tdb_off_t);
	ಆದರೆ (!(newdb = (ರಚನೆ tdb_ಶೀರ್ಷಿಕೆ *)ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ_2, 1)))
		ಹಿಂತಿರುಗಿ TDB_ERRCODE(TDB_ERR_OOM, -1);

	/* Fill in the header */
	newdb->ಆವೃತ್ತಿ = TDB_ಆವೃತ್ತಿ;
	newdb->hash_ಗಾತ್ರ = hash_ಗಾತ್ರ;
	ಆದರೆ (tdb->flags & TDB_ಆಂತರಿಕ) {
		tdb->map_ಗಾತ್ರ = ಗಾತ್ರ_2;
		tdb->map_ptr = (ಅಕ್ಷರ *)newdb;
		ಸ್ಮರಣೆ_ನಕಲು(&tdb->ಶೀರ್ಷಿಕೆ, newdb, ಗಾತ್ರ(tdb->ಶೀರ್ಷಿಕೆ));
		/* Convert the `ondisk' version if asked. */
		CONVERT(*newdb);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (lseek(tdb->fd, 0, SEEK_SET) == -1)
		ಹೋಗು fail;

	ಆದರೆ (ftruncate(tdb->fd, 0) == -1)
		ಹೋಗು fail;

	/* This creates an endian-converted header, as if read from disk */
	CONVERT(*newdb);
	ಸ್ಮರಣೆ_ನಕಲು(&tdb->ಶೀರ್ಷಿಕೆ, newdb, ಗಾತ್ರ(tdb->ಶೀರ್ಷಿಕೆ));
	/* Don't endian-convert the magic food! */
	ಸ್ಮರಣೆ_ನಕಲು(newdb->magic_food, TDB_MAGIC_FOOD, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(TDB_MAGIC_FOOD)+1);
	ಆದರೆ (ಲಿಖ(tdb->fd, newdb, ಗಾತ್ರ_2) != ಗಾತ್ರ_2) {
		ret = -1;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ret = 0;
	}

  fail:
	ಸುರಕ್ಷಿತ_FREE(newdb);
	ಹಿಂತಿರುಗಿ ret;
}



ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tdb_already_open(dev_t device,
			    ino_t ino)
{
	ರಚನೆ tdb_context *i;

	ಗಾಗಿ (i = tdbs; i; i = i->ಮುಂದಿನ) {
		ಆದರೆ (i->device == device && i->inode == ino) {
			ಹಿಂತಿರುಗಿ 1;
		}
	}

	ಹಿಂತಿರುಗಿ 0;
}

/* open the database, creating it if necessary

   The open_flags and mode are passed straight to the open call on the
   database file. A flags value of O_WRONLY is invalid. The hash size
   is advisory, use zero for a default value.

   Return is NULL on error, in which case errno is also set.  Don't
   try to call tdb_error or tdb_errname, just do strerror(errno).

   @param name may be NULL for internal databases. */
ರಚನೆ tdb_context *tdb_open(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ hash_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ tdb_flags,
		      ಪೂರ್ಣಾಂಕ open_flags, mode_t ಬಹುಲಕ)
{
	ಹಿಂತಿರುಗಿ tdb_open_ex(name, hash_ಗಾತ್ರ, tdb_flags, open_flags, ಬಹುಲಕ, ಖಾಲಿ, ಖಾಲಿ);
}

/* a default logging function */
ಸ್ಥಿರ ಶೂನ್ಯ null_log_fn(ರಚನೆ tdb_context *tdb, ಎಣಿಕೆ tdb_debug_level level, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt, ...) PRINTF_ATTRIBUTE(3, 4);
ಸ್ಥಿರ ಶೂನ್ಯ null_log_fn(ರಚನೆ tdb_context *tdb EXT2FS_ATTR((ಬಳಸದ)),
			ಎಣಿಕೆ tdb_debug_level level EXT2FS_ATTR((ಬಳಸದ)),
			ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *fmt EXT2FS_ATTR((ಬಳಸದ)), ...)
{
}


ರಚನೆ tdb_context *tdb_open_ex(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ hash_ಗಾತ್ರ, ಪೂರ್ಣಾಂಕ tdb_flags,
				ಪೂರ್ಣಾಂಕ open_flags, mode_t ಬಹುಲಕ,
				ಸ್ಥಿರಾಂಕ ರಚನೆ tdb_logging_context *log_ctx,
				tdb_hash_func hash_fn)
{
	ರಚನೆ tdb_context *tdb;
	ರಚನೆ stat st;
	ಪೂರ್ಣಾಂಕ rev = 0, locked = 0;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *vp;
	u32 vertest;

	ಆದರೆ (!(tdb = (ರಚನೆ tdb_context *)ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ *tdb))) {
		/* Can't log this */
		errno = ENOMEM;
		ಹೋಗು fail;
	}
	tdb_io_init(tdb);
	tdb->fd = -1;
	tdb->name = ಖಾಲಿ;
	tdb->map_ptr = ಖಾಲಿ;
	tdb->flags = tdb_flags;
	tdb->open_flags = open_flags;
	ಆದರೆ (log_ctx) {
		tdb->ಲಘುಗಣಕ = *log_ctx;
	} ಇಲ್ಲದಿದ್ದರೆ {
		tdb->ಲಘುಗಣಕ.log_fn = null_log_fn;
		tdb->ಲಘುಗಣಕ.log_ಖಾಸಗಿ = ಖಾಲಿ;
	}
	tdb->hash_fn = hash_fn ? hash_fn : default_tdb_hash;

	/* cache the page size */
	tdb->ಪುಟ_ಗಾತ್ರ = sysconf(_SC_PAGESIZE);
	ಆದರೆ (tdb->ಪುಟ_ಗಾತ್ರ <= 0) {
		tdb->ಪುಟ_ಗಾತ್ರ = 0x2000;
	}

	ಆದರೆ ((open_flags & O_ACCMODE) == O_WRONLY) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: can't open tdb %s write-only\n",
			 name));
		errno = EINVAL;
		ಹೋಗು fail;
	}

	ಆದರೆ (hash_ಗಾತ್ರ == 0)
		hash_ಗಾತ್ರ = DEFAULT_HASH_ಗಾತ್ರ;
	ಆದರೆ ((open_flags & O_ACCMODE) == O_RDONLY) {
		tdb->read_only = 1;
		/* read only databases don't do locking or clear if first */
		tdb->flags |= TDB_NOLOCK;
		tdb->flags &= ~TDB_CLEAR_IF_ಮೊದಲ;
	}

	/* internal databases don't mmap or lock, and start off cleared */
	ಆದರೆ (tdb->flags & TDB_ಆಂತರಿಕ) {
		tdb->flags |= (TDB_NOLOCK | TDB_NOMMAP);
		tdb->flags &= ~TDB_CLEAR_IF_ಮೊದಲ;
		ಆದರೆ (tdb_new_database(tdb, hash_ಗಾತ್ರ) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: tdb_new_database failed!"));
			ಹೋಗು fail;
		}
		ಹೋಗು ಆಂತರಿಕ;
	}

	ಆದರೆ ((tdb->fd = open(name, open_flags, ಬಹುಲಕ)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_open_ex: could not open file %s: %s\n",
			 name, strerror(errno)));
		ಹೋಗು fail;	/* errno set by open(2) */
	}

	/* ensure there is only one process initialising at once */
	ಆದರೆ (tdb->methods->tdb_brlock(tdb, GLOBAL_ಬೀಗ, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to get global lock on %s: %s\n",
			 name, strerror(errno)));
		ಹೋಗು fail;	/* errno set by tdb_brlock */
	}

	/* we need to zero database if we are the only one with it open */
	ಆದರೆ ((tdb_flags & TDB_CLEAR_IF_ಮೊದಲ) &&
	    (locked = (tdb->methods->tdb_brlock(tdb, ಸಕ್ರಿಯ_ಬೀಗ, F_WRLCK, F_SETLK, 0, 1) == 0))) {
		open_flags |= O_CREAT;
		ಆದರೆ (ftruncate(tdb->fd, 0) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_open_ex: "
				 "failed to truncate %s: %s\n",
				 name, strerror(errno)));
			ಹೋಗು fail; /* errno set by ftruncate */
		}
	}

	ಆದರೆ (ಪಠ(tdb->fd, &tdb->ಶೀರ್ಷಿಕೆ, ಗಾತ್ರ(tdb->ಶೀರ್ಷಿಕೆ)) != ಗಾತ್ರ(tdb->ಶೀರ್ಷಿಕೆ)
	    || memcmp(tdb->ಶೀರ್ಷಿಕೆ.magic_food, TDB_MAGIC_FOOD,
		      ಗಾತ್ರ(TDB_MAGIC_FOOD)) != 0
	    || (tdb->ಶೀರ್ಷಿಕೆ.ಆವೃತ್ತಿ != TDB_ಆವೃತ್ತಿ
		&& !(rev = (tdb->ಶೀರ್ಷಿಕೆ.ಆವೃತ್ತಿ==TDB_BYTEREV(TDB_ಆವೃತ್ತಿ))))) {
		/* its not a valid database - possibly initialise it */
		ಆದರೆ (!(open_flags & O_CREAT) || tdb_new_database(tdb, hash_ಗಾತ್ರ) == -1) {
			errno = EIO; /* ie bad format or something */
			ಹೋಗು fail;
		}
		rev = (tdb->flags & TDB_CONVERT);
	}
	vp = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)&tdb->ಶೀರ್ಷಿಕೆ.ಆವೃತ್ತಿ;
	vertest = (((u32)vp[0]) << 24) | (((u32)vp[1]) << 16) |
		  (((u32)vp[2]) << 8) | (u32)vp[3];
	tdb->flags |= (vertest==TDB_ಆವೃತ್ತಿ) ? TDB_BIGENDIAN : 0;
	ಆದರೆ (!rev)
		tdb->flags &= ~TDB_CONVERT;
	ಇಲ್ಲದಿದ್ದರೆ {
		tdb->flags |= TDB_CONVERT;
		tdb_convert(&tdb->ಶೀರ್ಷಿಕೆ, ಗಾತ್ರ(tdb->ಶೀರ್ಷಿಕೆ));
	}
	ಆದರೆ (fstat(tdb->fd, &st) == -1)
		ಹೋಗು fail;

	ಆದರೆ (tdb->ಶೀರ್ಷಿಕೆ.rwlocks != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: spinlocks no longer supported\n"));
		ಹೋಗು fail;
	}

	/* Is it already in the open list?  If so, fail. */
	ಆದರೆ (tdb_already_open(st.st_dev, st.st_ino)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
			 "%s (%d,%d) is already open in this process\n",
			 name, (ಪೂರ್ಣಾಂಕ)st.st_dev, (ಪೂರ್ಣಾಂಕ)st.st_ino));
		errno = EBUSY;
		ಹೋಗು fail;
	}

	ಆದರೆ (!(tdb->name = (ಅಕ್ಷರ *)strdup(name))) {
		errno = ENOMEM;
		ಹೋಗು fail;
	}

	tdb->map_ಗಾತ್ರ = st.st_ಗಾತ್ರ;
	tdb->device = st.st_dev;
	tdb->inode = st.st_ino;
	tdb->max_dead_ದಾಖಲೆಗಳು = 0;
	tdb_mmap(tdb);
	ಆದರೆ (locked) {
		ಆದರೆ (tdb->methods->tdb_brlock(tdb, ಸಕ್ರಿಯ_ಬೀಗ, F_UNLCK, F_SETLK, 0, 1) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
				 "failed to take ACTIVE_LOCK on %s: %s\n",
				 name, strerror(errno)));
			ಹೋಗು fail;
		}

	}

	/* We always need to do this if the CLEAR_IF_FIRST flag is set, even if
	   we didn't get the initial exclusive lock as we need to let all other
	   users know we're using it. */

	ಆದರೆ (tdb_flags & TDB_CLEAR_IF_ಮೊದಲ) {
		/* leave this lock in place to indicate it's in use */
		ಆದರೆ (tdb->methods->tdb_brlock(tdb, ಸಕ್ರಿಯ_ಬೀಗ, F_RDLCK, F_SETLKW, 0, 1) == -1)
			ಹೋಗು fail;
	}

	/* if needed, run recovery */
	ಆದರೆ (tdb_transaction_recover(tdb) == -1) {
		ಹೋಗು fail;
	}

 ಆಂತರಿಕ:
	/* Internal (memory-only) databases skip all the code above to
	 * do with disk files, and resume here by releasing their
	 * global lock and hooking into the active list. */
	ಆದರೆ (tdb->methods->tdb_brlock(tdb, GLOBAL_ಬೀಗ, F_UNLCK, F_SETLKW, 0, 1) == -1)
		ಹೋಗು fail;
	tdb->ಮುಂದಿನ = tdbs;
	tdbs = tdb;
	ಹಿಂತಿರುಗಿ tdb;

 fail:
	{ ಪೂರ್ಣಾಂಕ save_errno = errno;

	ಆದರೆ (!tdb)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಆದರೆ (tdb->map_ptr) {
		ಆದರೆ (tdb->flags & TDB_ಆಂತರಿಕ)
			ಸುರಕ್ಷಿತ_FREE(tdb->map_ptr);
		ಇಲ್ಲದಿದ್ದರೆ
			tdb_munmap(tdb);
	}
	ಸುರಕ್ಷಿತ_FREE(tdb->name);
	ಆದರೆ (tdb->fd != -1)
		ಆದರೆ (close(tdb->fd) != 0)
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to close tdb->fd on error!\n"));
	ಸುರಕ್ಷಿತ_FREE(tdb);
	errno = save_errno;
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}
}

/*
 * Set the maximum number of dead records per hash chain
 */

ಶೂನ್ಯ tdb_ಗಣ_max_dead(ರಚನೆ tdb_context *tdb, ಪೂರ್ಣಾಂಕ max_dead)
{
	tdb->max_dead_ದಾಖಲೆಗಳು = max_dead;
}

/**
 * Close a database.
 *
 * @returns -1 for error; 0 for success.
 **/
ಪೂರ್ಣಾಂಕ tdb_close(ರಚನೆ tdb_context *tdb)
{
	ರಚನೆ tdb_context **i;
	ಪೂರ್ಣಾಂಕ ret = 0;

	ಆದರೆ (tdb->transaction) {
		tdb_transaction_cancel(tdb);
	}

	ಆದರೆ (tdb->map_ptr) {
		ಆದರೆ (tdb->flags & TDB_ಆಂತರಿಕ)
			ಸುರಕ್ಷಿತ_FREE(tdb->map_ptr);
		ಇಲ್ಲದಿದ್ದರೆ
			tdb_munmap(tdb);
	}
	ಸುರಕ್ಷಿತ_FREE(tdb->name);
	ಆದರೆ (tdb->fd != -1)
		ret = close(tdb->fd);
	ಸುರಕ್ಷಿತ_FREE(tdb->lockrecs);

	/* Remove from contexts list */
	ಗಾಗಿ (i = &tdbs; *i; i = &(*i)->ಮುಂದಿನ) {
		ಆದರೆ (*i == tdb) {
			*i = tdb->ಮುಂದಿನ;
			ನಿಲ್ಲಿಸು;
		}
	}

	memset(tdb, 0, ಗಾತ್ರ(*tdb));
	ಸುರಕ್ಷಿತ_FREE(tdb);

	ಹಿಂತಿರುಗಿ ret;
}

/* register a logging function */
ಶೂನ್ಯ tdb_ಗಣ_logging_function(ರಚನೆ tdb_context *tdb,
                              ಸ್ಥಿರಾಂಕ ರಚನೆ tdb_logging_context *log_ctx)
{
        tdb->ಲಘುಗಣಕ = *log_ctx;
}

ಶೂನ್ಯ *tdb_get_logging_ಖಾಸಗಿ(ರಚನೆ tdb_context *tdb)
{
	ಹಿಂತಿರುಗಿ tdb->ಲಘುಗಣಕ.log_ಖಾಸಗಿ;
}

/* reopen a tdb - this can be used after a fork to ensure that we have an independent
   seek pointer from our parent and to re-establish locks */
ಪೂರ್ಣಾಂಕ tdb_reopen(ರಚನೆ tdb_context *tdb)
{
	ರಚನೆ stat st;

	ಆದರೆ (tdb->flags & TDB_ಆಂತರಿಕ) {
		ಹಿಂತಿರುಗಿ 0; /* Nothing to do. */
	}

	ಆದರೆ (tdb->ಅಂಕೆ_locks != 0 || tdb->global_ಬೀಗ.ಎಣಿಕೆ_2) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed with locks held\n"));
		ಹೋಗು fail;
	}

	ಆದರೆ (tdb->transaction != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed inside a transaction\n"));
		ಹೋಗು fail;
	}

	ಆದರೆ (tdb_munmap(tdb) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: munmap failed (%s)\n", strerror(errno)));
		ಹೋಗು fail;
	}
	ಆದರೆ (close(tdb->fd) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: WARNING closing tdb->fd failed!\n"));
	tdb->fd = open(tdb->name, tdb->open_flags & ~(O_CREAT|O_TRUNC), 0);
	ಆದರೆ (tdb->fd == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: open failed (%s)\n", strerror(errno)));
		ಹೋಗು fail;
	}
	ಆದರೆ ((tdb->flags & TDB_CLEAR_IF_ಮೊದಲ) &&
	    (tdb->methods->tdb_brlock(tdb, ಸಕ್ರಿಯ_ಬೀಗ, F_RDLCK, F_SETLKW, 0, 1) == -1)) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: failed to obtain active lock\n"));
		ಹೋಗು fail;
	}
	ಆದರೆ (fstat(tdb->fd, &st) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: fstat failed (%s)\n", strerror(errno)));
		ಹೋಗು fail;
	}
	ಆದರೆ (st.st_ino != tdb->inode || st.st_dev != tdb->device) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: file dev/inode has changed!\n"));
		ಹೋಗು fail;
	}
	tdb_mmap(tdb);

	ಹಿಂತಿರುಗಿ 0;

fail:
	tdb_close(tdb);
	ಹಿಂತಿರುಗಿ -1;
}

/* reopen all tdb's */
ಪೂರ್ಣಾಂಕ tdb_reopen_all(ಪೂರ್ಣಾಂಕ ಪೋಷಕ_longlived)
{
	ರಚನೆ tdb_context *tdb;

	ಗಾಗಿ (tdb=tdbs; tdb; tdb = tdb->ಮುಂದಿನ) {
		/*
		 * If the parent is longlived (ie. a
		 * parent daemon architecture), we know
		 * it will keep it's active lock on a
		 * tdb opened with CLEAR_IF_FIRST. Thus
		 * for child processes we don't have to
		 * add an active lock. This is essential
		 * to improve performance on systems that
		 * keep POSIX locks as a non-scalable data
		 * structure in the kernel.
		 */
		ಆದರೆ (ಪೋಷಕ_longlived) {
			/* Ensure no clear-if-first. */
			tdb->flags &= ~TDB_CLEAR_IF_ಮೊದಲ;
		}

		ಆದರೆ (tdb_reopen(tdb) != 0)
			ಹಿಂತಿರುಗಿ -1;
	}

	ಹಿಂತಿರುಗಿ 0;
}

/**
 * Flush a database file from the page cache.
 **/
ಪೂರ್ಣಾಂಕ tdb_flush(ರಚನೆ tdb_context *tdb)
{
	ಆದರೆ (tdb->fd != -1)
		ಹಿಂತಿರುಗಿ fsync(tdb->fd);
	ಹಿಂತಿರುಗಿ 0;
}
