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

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_STAND_ALONE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HAVE_MMAP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HAVE_STRDUP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HAVE_SYS_MMAN_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HAVE_UTIME_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HAVE_UTIME
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __FreeBSD__
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XOPEN_ਸਰੋਤ 600
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdarg.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYS_SELECT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/select.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_UTIME_H
#ਅਨ੍ਤਰ੍ਭਾਵ <utime.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/file.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYS_MMAN_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/mman.h>
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __GNUC__
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_ATTR(x) __attribute__(x)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_ATTR(x)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MAP_FILE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAP_FILE 0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MAP_FAILED
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAP_FAILED ((ਖਾਲੀ *)-1)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_STRDUP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ strdup rep_strdup
ਸਥਿਰ ਅੱਖਰ *rep_strdup(ਅਚਲ ਅੱਖਰ *s)
{
	ਅੱਖਰ *ret;
	ਪੂਰਨ ਲੰਮਾਈ;

	ਜੇ (!s)
		ਵਾਪਸ ਨਿਰਾ;
	ਲੰਮਾਈ = ਲੜੀ_ਲੰਬਾਈ(s);
	ret = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਲੰਮਾਈ + 1);
	ਜੇ (ret) {
		strncpy(ret, s, ਲੰਮਾਈ);
		ret[ਲੰਮਾਈ] = '\0';
	}
	ਵਾਪਸ ret;
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ PRINTF_ATTRIBUTE
#ਜੇ (__GNUC__ >= 3) && (__GNUC_MINOR__ >= 1 )
/** Use gcc attribute to check printf fns.  a1 is the 1-based index of
 * the parameter containing the format, and a2 the index of the first
 * argument. Note that some gcc 2.x versions don't handle this
 * properly **/
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRINTF_ATTRIBUTE(a1, a2) __attribute__ ((format (__printf__, a1, a2)))
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRINTF_ATTRIBUTE(a1, a2)
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ defined __STDC__ && defined __STDC_VERSION__ && __STDC_VERSION__ <= 201710L
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ bool;
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "tdb.h"

ਸਥਿਰ TDB_ਅੰਕੜੇ tdb_null;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ u32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ u32 ਬਿਨਾਨਿਸ਼ਾਨੀ
#ਜੇ_ਅੰਤ

ਕਿਸਮਪਰਿਭਾਸ਼ਾ u32 tdb_len_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ u32 tdb_off_t;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ offsetof
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ offsetof(t,f) ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)&((t *)0)->f)
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_MAGIC_FOOD "TDB file\n"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_ਸੰਸਕਰਣ (0x26011967 + 6)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_MAGIC (0x26011999U)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_FREE_MAGIC (~TDB_MAGIC)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_DEAD_MAGIC (0xFEE1DEAD)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_RECOVERY_MAGIC (0xf53bc0e7U)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_ALIGNMENT 4
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MIN_REC_ਮਾਪ (2*ਆਕਾਰ(ਬਣਤਰ list_struct) + TDB_ALIGNMENT)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFAULT_HASH_ਮਾਪ 131
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FREELIST_ਸਿਖਰ (ਆਕਾਰ(ਬਣਤਰ tdb_ਸਿਰਲੇਖ))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_ALIGN(x,a) (((x) + (a)-1) & ~((a)-1))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_BYTEREV(x) (((((x)&0xff)<<24)|((x)&0xFF00)<<8)|(((x)>>8)&0xFF00)|((x)>>24))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_DEAD(r) ((r)->magic == TDB_DEAD_MAGIC)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_BAD_MAGIC(r) ((r)->magic != TDB_MAGIC && !TDB_DEAD(r))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_HASH_ਸਿਖਰ(hash) (FREELIST_ਸਿਖਰ + (ਬਾਲਟੀ(hash)+1)*ਆਕਾਰ(tdb_off_t))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_HASHTABLE_ਮਾਪ(tdb) ((tdb->ਸਿਰਲੇਖ.hash_ਮਾਪ+1)*ਆਕਾਰ(tdb_off_t))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_ਅੰਕੜੇ_ਸ਼ੁਰੂ(hash_ਮਾਪ) TDB_HASH_ਸਿਖਰ(hash_ਮਾਪ-1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_RECOVERY_ਸਿਰ offsetof(ਬਣਤਰ tdb_ਸਿਰਲੇਖ, recovery_ਸ਼ੁਰੂ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_SEQNUM_OFS    offsetof(ਬਣਤਰ tdb_ਸਿਰਲੇਖ, sequence_ਸੰਖਿਆ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_PAD_BYTE 0x42
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_PAD_U32  0x42424242

/* NB assumes there is a local variable called "tdb" that is the
 * current context, also takes doubly-parenthesized print-style
 * argument. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_LOG(x) tdb->ਲਘੂਗਣਕ.log_fn x

/* lock offsets */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GLOBAL_ਤਾਲਾ      0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸਰਗਰਮ_ਤਾਲਾ      4
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TRANSACTION_ਤਾਲਾ 8

/* free memory if the pointer is valid and zero the pointer */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਸੁਰੱਖਿਅਤ_FREE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸੁਰੱਖਿਅਤ_FREE(x) ਕਰੋ { ਜੇ ((x) != ਨਿਰਾ) {ਮੁਕਤ(x); (x)=ਨਿਰਾ;} } ਜਦੋਂ(0)
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਬਾਲਟੀ(hash) ((hash) % tdb->ਸਿਰਲੇਖ.hash_ਮਾਪ)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DOCONV() (tdb->flags & TDB_CONVERT)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CONVERT(x) (DOCONV() ? tdb_convert(&x, ਆਕਾਰ(x)) : &x)


/* the body of the database is made of one list_struct for the free space
   plus a separate data list for each hash value */
ਬਣਤਰ list_struct {
	tdb_off_t ਅਗਲਾ; /* offset of the next record in the list */
	tdb_len_t rec_len; /* total byte length of record */
	tdb_len_t ਕੁੰਜੀ_len; /* byte length of key */
	tdb_len_t ਅੰਕੜੇ_len; /* byte length of data */
	u32 ਭਰਿਆ_hash; /* the full 32 bit hash of the key */
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
ਬਣਤਰ tdb_ਸਿਰਲੇਖ {
	ਅੱਖਰ magic_food[32]; /* for /etc/magic */
	u32 ਸੰਸਕਰਣ; /* version of the code */
	u32 hash_ਮਾਪ; /* number of hash entries */
	tdb_off_t rwlocks; /* obsolete - kept to detect old formats */
	tdb_off_t recovery_ਸ਼ੁਰੂ; /* offset of transaction recovery region */
	tdb_off_t sequence_ਸੰਖਿਆ; /* used when TDB_SEQNUM is set */
	tdb_off_t reserved[29];
};

ਬਣਤਰ tdb_ਤਾਲਾ_type {
	ਪੂਰਨ list;
	u32 ਗਿਣਤੀ_2;
	u32 ltype;
};

ਬਣਤਰ tdb_traverse_ਤਾਲਾ {
	ਬਣਤਰ tdb_traverse_ਤਾਲਾ *ਅਗਲਾ;
	u32 off;
	u32 hash;
	ਪੂਰਨ ਤਾਲਾ_rw;
};


ਬਣਤਰ tdb_methods {
	ਪੂਰਨ (*tdb_read)(ਬਣਤਰ tdb_context *, tdb_off_t , ਖਾਲੀ *, tdb_len_t , ਪੂਰਨ );
	ਪੂਰਨ (*tdb_write)(ਬਣਤਰ tdb_context *, tdb_off_t, ਅਚਲ ਖਾਲੀ *, tdb_len_t);
	ਖਾਲੀ (*ਅਗਲਾ_hash_chain)(ਬਣਤਰ tdb_context *, u32 *);
	ਪੂਰਨ (*tdb_oob)(ਬਣਤਰ tdb_context *, tdb_off_t , ਪੂਰਨ );
	ਪੂਰਨ (*tdb_expand_file)(ਬਣਤਰ tdb_context *, tdb_off_t , tdb_off_t );
	ਪੂਰਨ (*tdb_brlock)(ਬਣਤਰ tdb_context *, tdb_off_t , ਪੂਰਨ, ਪੂਰਨ, ਪੂਰਨ, size_t);
};

ਬਣਤਰ tdb_context {
	ਅੱਖਰ *name; /* the name of the database */
	ਖਾਲੀ *map_ptr; /* where it is currently mapped */
	ਪੂਰਨ fd; /* open file descriptor for the database */
	tdb_len_t map_ਮਾਪ; /* how much space has been mapped */
	ਪੂਰਨ read_only; /* opened read-only */
	ਪੂਰਨ traverse_read; /* read-only traversal */
	ਬਣਤਰ tdb_ਤਾਲਾ_type global_ਤਾਲਾ;
	ਪੂਰਨ ਅੰਕ_lockrecs;
	ਬਣਤਰ tdb_ਤਾਲਾ_type *lockrecs; /* only real locks, all with count>0 */
	ਗਿਣਤੀ TDB_ERROR ecode; /* error code for last tdb error */
	ਬਣਤਰ tdb_ਸਿਰਲੇਖ ਸਿਰਲੇਖ; /* a cached copy of the header */
	u32 flags; /* the flags passed to tdb_open */
	ਬਣਤਰ tdb_traverse_ਤਾਲਾ travlocks; /* current traversal locks */
	ਬਣਤਰ tdb_context *ਅਗਲਾ; /* all tdbs to avoid multiple opens */
	dev_t device;	/* uniquely identifies this tdb */
	ino_t inode;	/* uniquely identifies this tdb */
	ਬਣਤਰ tdb_logging_context ਲਘੂਗਣਕ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ (*hash_fn)(TDB_ਅੰਕੜੇ *ਕੁੰਜੀ);
	ਪੂਰਨ open_flags; /* flags used in the open - needed by reopen */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_locks; /* number of chain locks held */
	ਅਚਲ ਬਣਤਰ tdb_methods *methods;
	ਬਣਤਰ tdb_transaction *transaction;
	ਪੂਰਨ ਪੰਨਾ_ਮਾਪ;
	ਪੂਰਨ max_dead_ਰਿਕਾਰਡਾਂ;
	bool have_transaction_ਤਾਲਾ;
	tdb_len_t real_map_ਮਾਪ; /* how much space has been mapped */
};


/*
  internal prototypes
*/
ਸਥਿਰ ਪੂਰਨ tdb_munmap(ਬਣਤਰ tdb_context *tdb);
ਸਥਿਰ ਖਾਲੀ tdb_mmap(ਬਣਤਰ tdb_context *tdb);
ਸਥਿਰ ਪੂਰਨ tdb_ਤਾਲਾ(ਬਣਤਰ tdb_context *tdb, ਪੂਰਨ list, ਪੂਰਨ ltype);
ਪੂਰਨ tdb_ਤਾਲਾ_nonblock(ਬਣਤਰ tdb_context *tdb, ਪੂਰਨ list, ਪੂਰਨ ltype);
ਸਥਿਰ ਪੂਰਨ tdb_unlock(ਬਣਤਰ tdb_context *tdb, ਪੂਰਨ list, ਪੂਰਨ ltype);
ਸਥਿਰ ਪੂਰਨ tdb_brlock(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਵਿੱਥ, ਪੂਰਨ rw_type, ਪੂਰਨ lck_type, ਪੂਰਨ probe, size_t len);
ਸਥਿਰ ਪੂਰਨ tdb_transaction_ਤਾਲਾ(ਬਣਤਰ tdb_context *tdb, ਪੂਰਨ ltype);
ਸਥਿਰ ਪੂਰਨ tdb_transaction_unlock(ਬਣਤਰ tdb_context *tdb);
ਸਥਿਰ ਪੂਰਨ tdb_brlock_upgrade(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਵਿੱਥ, size_t len);
ਸਥਿਰ ਪੂਰਨ tdb_write_ਤਾਲਾ_ਰਿਕਾਰਡ(ਬਣਤਰ tdb_context *tdb, tdb_off_t off);
ਸਥਿਰ ਪੂਰਨ tdb_write_unlock_ਰਿਕਾਰਡ(ਬਣਤਰ tdb_context *tdb, tdb_off_t off);
ਸਥਿਰ ਪੂਰਨ tdb_ofs_read(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਵਿੱਥ, tdb_off_t *d);
ਸਥਿਰ ਪੂਰਨ tdb_ofs_write(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਵਿੱਥ, tdb_off_t *d);
ਸਥਿਰ ਖਾਲੀ *tdb_convert(ਖਾਲੀ *buf, u32 ਮਾਪ);
ਸਥਿਰ ਪੂਰਨ tdb_free(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਵਿੱਥ, ਬਣਤਰ list_struct *rec);
ਸਥਿਰ tdb_off_t tdb_allocate(ਬਣਤਰ tdb_context *tdb, tdb_len_t ਲੰਮਾਈ, ਬਣਤਰ list_struct *rec);
ਸਥਿਰ ਪੂਰਨ tdb_ofs_read(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਵਿੱਥ, tdb_off_t *d);
ਸਥਿਰ ਪੂਰਨ tdb_ofs_write(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਵਿੱਥ, tdb_off_t *d);
ਸਥਿਰ ਪੂਰਨ tdb_ਤਾਲਾ_ਰਿਕਾਰਡ(ਬਣਤਰ tdb_context *tdb, tdb_off_t off);
ਸਥਿਰ ਪੂਰਨ tdb_unlock_ਰਿਕਾਰਡ(ਬਣਤਰ tdb_context *tdb, tdb_off_t off);
ਸਥਿਰ ਪੂਰਨ tdb_rec_read(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਵਿੱਥ, ਬਣਤਰ list_struct *rec);
ਸਥਿਰ ਪੂਰਨ tdb_rec_write(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਵਿੱਥ, ਬਣਤਰ list_struct *rec);
ਸਥਿਰ ਪੂਰਨ tdb_do_delete(ਬਣਤਰ tdb_context *tdb, tdb_off_t rec_ptr, ਬਣਤਰ list_struct *rec);
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *tdb_alloc_read(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਵਿੱਥ, tdb_len_t len);
ਸਥਿਰ ਪੂਰਨ tdb_parse_ਅੰਕੜੇ(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ,
		   tdb_off_t ਵਿੱਥ, tdb_len_t len,
		   ਪੂਰਨ (*parser)(TDB_ਅੰਕੜੇ ਕੁੰਜੀ, TDB_ਅੰਕੜੇ ਅੰਕੜੇ,
				 ਖਾਲੀ *ਨਿੱਜੀ_ਅੰਕੜੇ),
		   ਖਾਲੀ *ਨਿੱਜੀ_ਅੰਕੜੇ);
ਸਥਿਰ tdb_off_t tdb_ਲੱਭੋ_ਤਾਲਾ_hash(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ, u32 hash, ਪੂਰਨ locktype,
			   ਬਣਤਰ list_struct *rec);
ਸਥਿਰ ਖਾਲੀ tdb_io_init(ਬਣਤਰ tdb_context *tdb);
ਸਥਿਰ ਪੂਰਨ tdb_expand(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਮਾਪ);
ਸਥਿਰ ਪੂਰਨ tdb_rec_free_read(ਬਣਤਰ tdb_context *tdb, tdb_off_t off,
		      ਬਣਤਰ list_struct *rec);


/* file: error.c */

ਗਿਣਤੀ TDB_ERROR tdb_error(ਬਣਤਰ tdb_context *tdb)
{
	ਵਾਪਸ tdb->ecode;
}

ਸਥਿਰ ਬਣਤਰ tdb_errname {
	ਗਿਣਤੀ TDB_ERROR ecode; ਅਚਲ ਅੱਖਰ *estring;
} emap[] = { {TDB_SUCCESS, "Success"},
	     {TDB_ERR_CORRUPT, "Corrupt database"},
	     {TDB_ERR_IO, "IO Error"},
	     {TDB_ERR_ਤਾਲਾ, "Locking error"},
	     {TDB_ERR_OOM, "Out of memory"},
	     {TDB_ERR_EXISTS, "Record exists"},
	     {TDB_ERR_NOLOCK, "Lock exists on other keys"},
	     {TDB_ERR_EINVAL, "Invalid parameter"},
	     {TDB_ERR_NOEXIST, "Record does not exist"},
	     {TDB_ERR_RDONLY, "write not permitted"} };

/* Error string for the last tdb error */
ਅਚਲ ਅੱਖਰ *tdb_errorstr(ਬਣਤਰ tdb_context *tdb)
{
	u32 i;
	ਲਈ (i = 0; i < ਆਕਾਰ(emap) / ਆਕਾਰ(ਬਣਤਰ tdb_errname); i++)
		ਜੇ (tdb->ecode == emap[i].ecode)
			ਵਾਪਸ emap[i].estring;
	ਵਾਪਸ "Invalid error code";
}

/* file: lock.c */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TDB_MARK_ਤਾਲਾ 0x80000000

/* a byte range locking function - return 0 on success
   this functions locks/unlocks 1 byte at the specified offset.

   On error, errno is also set so that errors are passed back properly
   through tdb_open().

   note that a len of zero means lock to end of file
*/
ਪੂਰਨ tdb_brlock(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਵਿੱਥ,
	       ਪੂਰਨ rw_type, ਪੂਰਨ lck_type, ਪੂਰਨ probe, size_t len)
{
	ਬਣਤਰ flock fl;
	ਪੂਰਨ ret;

	ਜੇ (tdb->flags & TDB_NOLOCK) {
		ਵਾਪਸ 0;
	}

	ਜੇ ((rw_type == F_WRLCK) && (tdb->read_only || tdb->traverse_read)) {
		tdb->ecode = TDB_ERR_RDONLY;
		ਵਾਪਸ -1;
	}

	fl.l_type = rw_type;
	fl.l_whence = SEEK_SET;
	fl.l_ਸ਼ੁਰੂ = ਵਿੱਥ;
	fl.l_len = len;
	fl.l_pid = 0;

	ਕਰੋ {
		ret = fcntl(tdb->fd,lck_type,&fl);
	} ਜਦੋਂ (ret == -1 && errno == EINTR);

	ਜੇ (ret == -1) {
		/* Generic lock error. errno set by fcntl.
		 * EAGAIN is an expected return from non-blocking
		 * locks. */
		ਜੇ (!probe && lck_type != F_SETLK) {
			/* Ensure error code is set for log fun to examine. */
			tdb->ecode = TDB_ERR_ਤਾਲਾ;
			TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock failed (fd=%d) at offset %d rw_type=%d lck_type=%d len=%d\n",
				 tdb->fd, ਵਿੱਥ, rw_type, lck_type, (ਪੂਰਨ)len));
		}
		ਵਾਪਸ TDB_ERRCODE(TDB_ERR_ਤਾਲਾ, -1);
	}
	ਵਾਪਸ 0;
}


/*
  upgrade a read lock to a write lock. This needs to be handled in a
  special way as some OSes (such as solaris) have too conservative
  deadlock detection and claim a deadlock when progress can be
  made. For those OSes we may loop for a while.
*/
ਪੂਰਨ tdb_brlock_upgrade(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਵਿੱਥ, size_t len)
{
	ਪੂਰਨ ਗਿਣਤੀ_2 = 1000;
	ਜਦੋਂ (ਗਿਣਤੀ_2--) {
		ਬਣਤਰ timeval tv;
		ਜੇ (tdb_brlock(tdb, ਵਿੱਥ, F_WRLCK, F_SETLKW, 1, len) == 0) {
			ਵਾਪਸ 0;
		}
		ਜੇ (errno != EDEADLK) {
			ਰੋਕੋ;
		}
		/* sleep for as short a time as we can - more portable than usleep() */
		tv.tv_sec = 0;
		tv.tv_usec = 1;
		select(0, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ, &tv);
	}
	TDB_LOG((tdb, TDB_DEBUG_TRACE,"tdb_brlock_upgrade failed at offset %d\n", ਵਿੱਥ));
	ਵਾਪਸ -1;
}


/* lock a list in the database. list -1 is the alloc list */
ਸਥਿਰ ਪੂਰਨ tdb_ਤਾਲਾ_2(ਬਣਤਰ tdb_context *tdb, ਪੂਰਨ list, ਪੂਰਨ ltype, ਪੂਰਨ op)
{
	ਬਣਤਰ tdb_ਤਾਲਾ_type *new_lck;
	ਪੂਰਨ i;
	bool mark_ਤਾਲਾ = ((ltype & TDB_MARK_ਤਾਲਾ) == TDB_MARK_ਤਾਲਾ);

	ltype &= ~TDB_MARK_ਤਾਲਾ;

	/* a global lock allows us to avoid per chain locks */
	ਜੇ (tdb->global_ਤਾਲਾ.ਗਿਣਤੀ_2 &&
	    ((u32)ltype == tdb->global_ਤਾਲਾ.ltype || ltype == F_RDLCK)) {
		ਵਾਪਸ 0;
	}

	ਜੇ (tdb->global_ਤਾਲਾ.ਗਿਣਤੀ_2) {
		ਵਾਪਸ TDB_ERRCODE(TDB_ERR_ਤਾਲਾ, -1);
	}

	ਜੇ (list < -1 || list >= (ਪੂਰਨ)tdb->ਸਿਰਲੇਖ.hash_ਮਾਪ) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_lock: invalid list %d for ltype=%d\n",
			   list, ltype));
		ਵਾਪਸ -1;
	}
	ਜੇ (tdb->flags & TDB_NOLOCK)
		ਵਾਪਸ 0;

	ਲਈ (i=0; i<tdb->ਅੰਕ_lockrecs; i++) {
		ਜੇ (tdb->lockrecs[i].list == list) {
			ਜੇ (tdb->lockrecs[i].ਗਿਣਤੀ_2 == 0) {
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
			tdb->lockrecs[i].ਗਿਣਤੀ_2++;
			ਵਾਪਸ 0;
		}
	}

	new_lck = (ਬਣਤਰ tdb_ਤਾਲਾ_type *)realloc(
		tdb->lockrecs,
		ਆਕਾਰ(*tdb->lockrecs) * (tdb->ਅੰਕ_lockrecs+1));
	ਜੇ (new_lck == ਨਿਰਾ) {
		errno = ENOMEM;
		ਵਾਪਸ -1;
	}
	tdb->lockrecs = new_lck;

	/* Since fcntl locks don't nest, we do a lock for the first one,
	   and simply bump the count for future ones */
	ਜੇ (!mark_ਤਾਲਾ &&
	    tdb->methods->tdb_brlock(tdb,FREELIST_ਸਿਖਰ+4*list, ltype, op,
				     0, 1)) {
		ਵਾਪਸ -1;
	}

	tdb->ਅੰਕ_locks++;

	tdb->lockrecs[tdb->ਅੰਕ_lockrecs].list = list;
	tdb->lockrecs[tdb->ਅੰਕ_lockrecs].ਗਿਣਤੀ_2 = 1;
	tdb->lockrecs[tdb->ਅੰਕ_lockrecs].ltype = ltype;
	tdb->ਅੰਕ_lockrecs += 1;

	ਵਾਪਸ 0;
}

/* lock a list in the database. list -1 is the alloc list */
ਪੂਰਨ tdb_ਤਾਲਾ(ਬਣਤਰ tdb_context *tdb, ਪੂਰਨ list, ਪੂਰਨ ltype)
{
	ਪੂਰਨ ret;
	ret = tdb_ਤਾਲਾ_2(tdb, list, ltype, F_SETLKW);
	ਜੇ (ret) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lock failed on list %d "
			 "ltype=%d (%s)\n",  list, ltype, strerror(errno)));
	}
	ਵਾਪਸ ret;
}

/* lock a list in the database. list -1 is the alloc list. non-blocking lock */
ਪੂਰਨ tdb_ਤਾਲਾ_nonblock(ਬਣਤਰ tdb_context *tdb, ਪੂਰਨ list, ਪੂਰਨ ltype)
{
	ਵਾਪਸ tdb_ਤਾਲਾ_2(tdb, list, ltype, F_SETLK);
}


/* unlock the database: returns void because it's too late for errors. */
	/* changed to return int it may be interesting to know there
	   has been an error  --simo */
ਪੂਰਨ tdb_unlock(ਬਣਤਰ tdb_context *tdb, ਪੂਰਨ list, ਪੂਰਨ ltype)
{
	ਪੂਰਨ ret = -1;
	ਪੂਰਨ i;
	ਬਣਤਰ tdb_ਤਾਲਾ_type *lck = ਨਿਰਾ;
	bool mark_ਤਾਲਾ = ((ltype & TDB_MARK_ਤਾਲਾ) == TDB_MARK_ਤਾਲਾ);

	ltype &= ~TDB_MARK_ਤਾਲਾ;

	/* a global lock allows us to avoid per chain locks */
	ਜੇ (tdb->global_ਤਾਲਾ.ਗਿਣਤੀ_2 &&
	    ((u32)ltype == tdb->global_ਤਾਲਾ.ltype || ltype == F_RDLCK)) {
		ਵਾਪਸ 0;
	}

	ਜੇ (tdb->global_ਤਾਲਾ.ਗਿਣਤੀ_2) {
		ਵਾਪਸ TDB_ERRCODE(TDB_ERR_ਤਾਲਾ, -1);
	}

	ਜੇ (tdb->flags & TDB_NOLOCK)
		ਵਾਪਸ 0;

	/* Sanity checks */
	ਜੇ (list < -1 || list >= (ਪੂਰਨ)tdb->ਸਿਰਲੇਖ.hash_ਮਾਪ) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: list %d invalid (%d)\n", list, tdb->ਸਿਰਲੇਖ.hash_ਮਾਪ));
		ਵਾਪਸ ret;
	}

	ਲਈ (i=0; i<tdb->ਅੰਕ_lockrecs; i++) {
		ਜੇ (tdb->lockrecs[i].list == list) {
			lck = &tdb->lockrecs[i];
			ਰੋਕੋ;
		}
	}

	ਜੇ ((lck == ਨਿਰਾ) || (lck->ਗਿਣਤੀ_2 == 0)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: count is 0\n"));
		ਵਾਪਸ -1;
	}

	ਜੇ (lck->ਗਿਣਤੀ_2 > 1) {
		lck->ਗਿਣਤੀ_2--;
		ਵਾਪਸ 0;
	}

	/*
	 * This lock has count==1 left, so we need to unlock it in the
	 * kernel. We don't bother with decrementing the in-memory array
	 * element, we're about to overwrite it with the last array element
	 * anyway.
	 */

	ਜੇ (mark_ਤਾਲਾ) {
		ret = 0;
	} ਨਹੀਂਤਾਂ {
		ret = tdb->methods->tdb_brlock(tdb, FREELIST_ਸਿਖਰ+4*list, F_UNLCK,
					       F_SETLKW, 0, 1);
	}
	tdb->ਅੰਕ_locks--;

	/*
	 * Shrink the array by overwriting the element just unlocked with the
	 * last array element.
	 */

	ਜੇ (tdb->ਅੰਕ_lockrecs > 1) {
		*lck = tdb->lockrecs[tdb->ਅੰਕ_lockrecs-1];
	}
	tdb->ਅੰਕ_lockrecs -= 1;

	/*
	 * We don't bother with realloc when the array shrinks, but if we have
	 * a completely idle tdb we should get rid of the locked array.
	 */

	ਜੇ (tdb->ਅੰਕ_lockrecs == 0) {
		ਸੁਰੱਖਿਅਤ_FREE(tdb->lockrecs);
	}

	ਜੇ (ret)
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlock: An error occurred unlocking!\n"));
	ਵਾਪਸ ret;
}

/*
  get the transaction lock
 */
ਪੂਰਨ tdb_transaction_ਤਾਲਾ(ਬਣਤਰ tdb_context *tdb, ਪੂਰਨ ltype)
{
	ਜੇ (tdb->have_transaction_ਤਾਲਾ || tdb->global_ਤਾਲਾ.ਗਿਣਤੀ_2) {
		ਵਾਪਸ 0;
	}
	ਜੇ (tdb->methods->tdb_brlock(tdb, TRANSACTION_ਤਾਲਾ, ltype,
				     F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_lock: failed to get transaction lock\n"));
		tdb->ecode = TDB_ERR_ਤਾਲਾ;
		ਵਾਪਸ -1;
	}
	tdb->have_transaction_ਤਾਲਾ = 1;
	ਵਾਪਸ 0;
}

/*
  release the transaction lock
 */
ਪੂਰਨ tdb_transaction_unlock(ਬਣਤਰ tdb_context *tdb)
{
	ਪੂਰਨ ret;
	ਜੇ (!tdb->have_transaction_ਤਾਲਾ) {
		ਵਾਪਸ 0;
	}
	ret = tdb->methods->tdb_brlock(tdb, TRANSACTION_ਤਾਲਾ, F_UNLCK, F_SETLKW, 0, 1);
	ਜੇ (ret == 0) {
		tdb->have_transaction_ਤਾਲਾ = 0;
	}
	ਵਾਪਸ ret;
}




/* lock/unlock entire database */
ਸਥਿਰ ਪੂਰਨ _tdb_lockall(ਬਣਤਰ tdb_context *tdb, ਪੂਰਨ ltype, ਪੂਰਨ op)
{
	bool mark_ਤਾਲਾ = ((ltype & TDB_MARK_ਤਾਲਾ) == TDB_MARK_ਤਾਲਾ);

	ltype &= ~TDB_MARK_ਤਾਲਾ;

	/* There are no locks on read-only dbs */
	ਜੇ (tdb->read_only || tdb->traverse_read)
		ਵਾਪਸ TDB_ERRCODE(TDB_ERR_ਤਾਲਾ, -1);

	ਜੇ (tdb->global_ਤਾਲਾ.ਗਿਣਤੀ_2 && tdb->global_ਤਾਲਾ.ltype == (u32)ltype) {
		tdb->global_ਤਾਲਾ.ਗਿਣਤੀ_2++;
		ਵਾਪਸ 0;
	}

	ਜੇ (tdb->global_ਤਾਲਾ.ਗਿਣਤੀ_2) {
		/* a global lock of a different type exists */
		ਵਾਪਸ TDB_ERRCODE(TDB_ERR_ਤਾਲਾ, -1);
	}

	ਜੇ (tdb->ਅੰਕ_locks != 0) {
		/* can't combine global and chain locks */
		ਵਾਪਸ TDB_ERRCODE(TDB_ERR_ਤਾਲਾ, -1);
	}

	ਜੇ (!mark_ਤਾਲਾ &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_ਸਿਖਰ, ltype, op,
				     0, 4*tdb->ਸਿਰਲੇਖ.hash_ਮਾਪ)) {
		ਜੇ (op == F_SETLKW) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_lockall failed (%s)\n", strerror(errno)));
		}
		ਵਾਪਸ -1;
	}

	tdb->global_ਤਾਲਾ.ਗਿਣਤੀ_2 = 1;
	tdb->global_ਤਾਲਾ.ltype = ltype;

	ਵਾਪਸ 0;
}



/* unlock entire db */
ਸਥਿਰ ਪੂਰਨ _tdb_unlockall(ਬਣਤਰ tdb_context *tdb, ਪੂਰਨ ltype)
{
	bool mark_ਤਾਲਾ = ((ltype & TDB_MARK_ਤਾਲਾ) == TDB_MARK_ਤਾਲਾ);

	ltype &= ~TDB_MARK_ਤਾਲਾ;

	/* There are no locks on read-only dbs */
	ਜੇ (tdb->read_only || tdb->traverse_read) {
		ਵਾਪਸ TDB_ERRCODE(TDB_ERR_ਤਾਲਾ, -1);
	}

	ਜੇ (tdb->global_ਤਾਲਾ.ltype != (u32)ltype ||
	    tdb->global_ਤਾਲਾ.ਗਿਣਤੀ_2 == 0) {
		ਵਾਪਸ TDB_ERRCODE(TDB_ERR_ਤਾਲਾ, -1);
	}

	ਜੇ (tdb->global_ਤਾਲਾ.ਗਿਣਤੀ_2 > 1) {
		tdb->global_ਤਾਲਾ.ਗਿਣਤੀ_2--;
		ਵਾਪਸ 0;
	}

	ਜੇ (!mark_ਤਾਲਾ &&
	    tdb->methods->tdb_brlock(tdb, FREELIST_ਸਿਖਰ, F_UNLCK, F_SETLKW,
				     0, 4*tdb->ਸਿਰਲੇਖ.hash_ਮਾਪ)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_unlockall failed (%s)\n", strerror(errno)));
		ਵਾਪਸ -1;
	}

	tdb->global_ਤਾਲਾ.ਗਿਣਤੀ_2 = 0;
	tdb->global_ਤਾਲਾ.ltype = 0;

	ਵਾਪਸ 0;
}

/* lock entire database with write lock */
ਪੂਰਨ tdb_lockall(ਬਣਤਰ tdb_context *tdb)
{
	ਵਾਪਸ _tdb_lockall(tdb, F_WRLCK, F_SETLKW);
}

/* lock entire database with write lock - mark only */
ਪੂਰਨ tdb_lockall_mark(ਬਣਤਰ tdb_context *tdb)
{
	ਵਾਪਸ _tdb_lockall(tdb, F_WRLCK | TDB_MARK_ਤਾਲਾ, F_SETLKW);
}

/* unlock entire database with write lock - unmark only */
ਪੂਰਨ tdb_lockall_unmark(ਬਣਤਰ tdb_context *tdb)
{
	ਵਾਪਸ _tdb_unlockall(tdb, F_WRLCK | TDB_MARK_ਤਾਲਾ);
}

/* lock entire database with write lock - nonblocking variant */
ਪੂਰਨ tdb_lockall_nonblock(ਬਣਤਰ tdb_context *tdb)
{
	ਵਾਪਸ _tdb_lockall(tdb, F_WRLCK, F_SETLK);
}

/* unlock entire database with write lock */
ਪੂਰਨ tdb_unlockall(ਬਣਤਰ tdb_context *tdb)
{
	ਵਾਪਸ _tdb_unlockall(tdb, F_WRLCK);
}

/* lock entire database with read lock */
ਪੂਰਨ tdb_lockall_read(ਬਣਤਰ tdb_context *tdb)
{
	ਵਾਪਸ _tdb_lockall(tdb, F_RDLCK, F_SETLKW);
}

/* lock entire database with read lock - nonblock variant */
ਪੂਰਨ tdb_lockall_read_nonblock(ਬਣਤਰ tdb_context *tdb)
{
	ਵਾਪਸ _tdb_lockall(tdb, F_RDLCK, F_SETLK);
}

/* unlock entire database with read lock */
ਪੂਰਨ tdb_unlockall_read(ਬਣਤਰ tdb_context *tdb)
{
	ਵਾਪਸ _tdb_unlockall(tdb, F_RDLCK);
}

/* lock/unlock one hash chain. This is meant to be used to reduce
   contention - it cannot guarantee how many records will be locked */
ਪੂਰਨ tdb_chainlock(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ)
{
	ਵਾਪਸ tdb_ਤਾਲਾ(tdb, ਬਾਲਟੀ(tdb->hash_fn(&ਕੁੰਜੀ)), F_WRLCK);
}

/* lock/unlock one hash chain, non-blocking. This is meant to be used
   to reduce contention - it cannot guarantee how many records will be
   locked */
ਪੂਰਨ tdb_chainlock_nonblock(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ)
{
	ਵਾਪਸ tdb_ਤਾਲਾ_nonblock(tdb, ਬਾਲਟੀ(tdb->hash_fn(&ਕੁੰਜੀ)), F_WRLCK);
}

/* mark a chain as locked without actually locking it. Warning! use with great caution! */
ਪੂਰਨ tdb_chainlock_mark(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ)
{
	ਵਾਪਸ tdb_ਤਾਲਾ(tdb, ਬਾਲਟੀ(tdb->hash_fn(&ਕੁੰਜੀ)), F_WRLCK | TDB_MARK_ਤਾਲਾ);
}

/* unmark a chain as locked without actually locking it. Warning! use with great caution! */
ਪੂਰਨ tdb_chainlock_unmark(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ)
{
	ਵਾਪਸ tdb_unlock(tdb, ਬਾਲਟੀ(tdb->hash_fn(&ਕੁੰਜੀ)), F_WRLCK | TDB_MARK_ਤਾਲਾ);
}

ਪੂਰਨ tdb_chainunlock(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ)
{
	ਵਾਪਸ tdb_unlock(tdb, ਬਾਲਟੀ(tdb->hash_fn(&ਕੁੰਜੀ)), F_WRLCK);
}

ਪੂਰਨ tdb_chainlock_read(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ)
{
	ਵਾਪਸ tdb_ਤਾਲਾ(tdb, ਬਾਲਟੀ(tdb->hash_fn(&ਕੁੰਜੀ)), F_RDLCK);
}

ਪੂਰਨ tdb_chainunlock_read(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ)
{
	ਵਾਪਸ tdb_unlock(tdb, ਬਾਲਟੀ(tdb->hash_fn(&ਕੁੰਜੀ)), F_RDLCK);
}



/* record lock stops delete underneath */
ਪੂਰਨ tdb_ਤਾਲਾ_ਰਿਕਾਰਡ(ਬਣਤਰ tdb_context *tdb, tdb_off_t off)
{
	ਵਾਪਸ off ? tdb->methods->tdb_brlock(tdb, off, F_RDLCK, F_SETLKW, 0, 1) : 0;
}

/*
  Write locks override our own fcntl readlocks, so check it here.
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
ਪੂਰਨ tdb_write_ਤਾਲਾ_ਰਿਕਾਰਡ(ਬਣਤਰ tdb_context *tdb, tdb_off_t off)
{
	ਬਣਤਰ tdb_traverse_ਤਾਲਾ *i;
	ਲਈ (i = &tdb->travlocks; i; i = i->ਅਗਲਾ)
		ਜੇ (i->off == off)
			ਵਾਪਸ -1;
	ਵਾਪਸ tdb->methods->tdb_brlock(tdb, off, F_WRLCK, F_SETLK, 1, 1);
}

/*
  Note this is meant to be F_SETLK, *not* F_SETLKW, as it's not
  an error to fail to get the lock here.
*/
ਪੂਰਨ tdb_write_unlock_ਰਿਕਾਰਡ(ਬਣਤਰ tdb_context *tdb, tdb_off_t off)
{
	ਵਾਪਸ tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLK, 0, 1);
}

/* fcntl locks don't stack: avoid unlocking someone else's */
ਪੂਰਨ tdb_unlock_ਰਿਕਾਰਡ(ਬਣਤਰ tdb_context *tdb, tdb_off_t off)
{
	ਬਣਤਰ tdb_traverse_ਤਾਲਾ *i;
	u32 ਗਿਣਤੀ_2 = 0;

	ਜੇ (off == 0)
		ਵਾਪਸ 0;
	ਲਈ (i = &tdb->travlocks; i; i = i->ਅਗਲਾ)
		ਜੇ (i->off == off)
			ਗਿਣਤੀ_2++;
	ਵਾਪਸ (ਗਿਣਤੀ_2 == 1 ? tdb->methods->tdb_brlock(tdb, off, F_UNLCK, F_SETLKW, 0, 1) : 0);
}

/* file: io.c */

/* check for an out of bounds access - if it is out of bounds then
   see if the database has been expanded by someone else and expand
   if necessary
   note that "len" is the minimum length needed for the db
*/
ਸਥਿਰ ਪੂਰਨ tdb_oob(ਬਣਤਰ tdb_context *tdb, tdb_off_t len, ਪੂਰਨ probe)
{
	ਬਣਤਰ stat st;
	ਜੇ (len <= tdb->map_ਮਾਪ)
		ਵਾਪਸ 0;
	ਜੇ (tdb->flags & TDB_ਅੰਦਰੂਨੀ) {
		ਜੇ (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond internal malloc size %d\n",
				 (ਪੂਰਨ)len, (ਪੂਰਨ)tdb->map_ਮਾਪ));
		}
		ਵਾਪਸ TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	ਜੇ (fstat(tdb->fd, &st) == -1) {
		ਵਾਪਸ TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	ਜੇ (st.st_ਮਾਪ < (off_t)len) {
		ਜੇ (!probe) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_oob len %d beyond eof at %d\n",
				 (ਪੂਰਨ)len, (ਪੂਰਨ)st.st_ਮਾਪ));
		}
		ਵਾਪਸ TDB_ERRCODE(TDB_ERR_IO, -1);
	}

	/* Unmap, update size, remap */
	ਜੇ (tdb_munmap(tdb) == -1)
		ਵਾਪਸ TDB_ERRCODE(TDB_ERR_IO, -1);
	tdb->map_ਮਾਪ = st.st_ਮਾਪ;
	tdb_mmap(tdb);
	ਵਾਪਸ 0;
}

/* write a lump of data at a specified offset */
ਸਥਿਰ ਪੂਰਨ tdb_write(ਬਣਤਰ tdb_context *tdb, tdb_off_t off,
		     ਅਚਲ ਖਾਲੀ *buf, tdb_len_t len)
{
	ਜੇ (len == 0) {
		ਵਾਪਸ 0;
	}

	ਜੇ (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		ਵਾਪਸ -1;
	}

	ਜੇ (tdb->methods->tdb_oob(tdb, off + len, 0) != 0)
		ਵਾਪਸ -1;

	ਜੇ (tdb->map_ptr) {
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(off + (ਅੱਖਰ *)tdb->map_ptr, buf, len);
	} ਨਹੀਂਤਾਂ ਜੇ (pwrite(tdb->fd, buf, len, off) != (ssize_t)len) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_write failed at %d len=%d (%s)\n",
			   off, len, strerror(errno)));
		ਵਾਪਸ TDB_ERRCODE(TDB_ERR_IO, -1);
	}
	ਵਾਪਸ 0;
}

/* Endian conversion: we only ever deal with 4 byte quantities */
ਖਾਲੀ *tdb_convert(ਖਾਲੀ *buf, u32 ਮਾਪ)
{
	u32 i, *p = (u32 *)buf;
	ਲਈ (i = 0; i < ਮਾਪ / 4; i++)
		p[i] = TDB_BYTEREV(p[i]);
	ਵਾਪਸ buf;
}


/* read a lump of data at a specified offset, maybe convert */
ਸਥਿਰ ਪੂਰਨ tdb_read(ਬਣਤਰ tdb_context *tdb, tdb_off_t off, ਖਾਲੀ *buf,
		    tdb_len_t len, ਪੂਰਨ cv)
{
	ਜੇ (tdb->methods->tdb_oob(tdb, off + len, 0) != 0) {
		ਵਾਪਸ -1;
	}

	ਜੇ (tdb->map_ptr) {
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf, off + (ਅੱਖਰ *)tdb->map_ptr, len);
	} ਨਹੀਂਤਾਂ {
		ssize_t ret = pread(tdb->fd, buf, len, off);
		ਜੇ (ret != (ssize_t)len) {
			/* Ensure ecode is set for log fn. */
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_read failed at %d "
				 "len=%d ret=%d (%s) map_size=%d\n",
				 (ਪੂਰਨ)off, (ਪੂਰਨ)len, (ਪੂਰਨ)ret, strerror(errno),
				 (ਪੂਰਨ)tdb->map_ਮਾਪ));
			ਵਾਪਸ TDB_ERRCODE(TDB_ERR_IO, -1);
		}
	}
	ਜੇ (cv) {
		tdb_convert(buf, len);
	}
	ਵਾਪਸ 0;
}



/*
  do an unlocked scan of the hash table heads to find the next non-zero head. The value
  will then be confirmed with the lock held
*/
ਸਥਿਰ ਖਾਲੀ tdb_ਅਗਲਾ_hash_chain(ਬਣਤਰ tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	ਜੇ (tdb->map_ptr) {
		ਲਈ (;h < tdb->ਸਿਰਲੇਖ.hash_ਮਾਪ;h++) {
			ਜੇ (0 != *(u32 *)(TDB_HASH_ਸਿਖਰ(h) + (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)tdb->map_ptr)) {
				ਰੋਕੋ;
			}
		}
	} ਨਹੀਂਤਾਂ {
		u32 off=0;
		ਲਈ (;h < tdb->ਸਿਰਲੇਖ.hash_ਮਾਪ;h++) {
			ਜੇ (tdb_ofs_read(tdb, TDB_HASH_ਸਿਖਰ(h), &off) != 0 || off != 0) {
				ਰੋਕੋ;
			}
		}
	}
	(*chain) = h;
}


ਪੂਰਨ tdb_munmap(ਬਣਤਰ tdb_context *tdb)
{
	ਜੇ (tdb->flags & TDB_ਅੰਦਰੂਨੀ)
		ਵਾਪਸ 0;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_MMAP
	ਜੇ (tdb->map_ptr) {
		ਪੂਰਨ ret = munmap(tdb->map_ptr, tdb->real_map_ਮਾਪ);
		ਜੇ (ret != 0)
			ਵਾਪਸ ret;
		tdb->real_map_ਮਾਪ = 0;
	}
#ਜੇ_ਅੰਤ
	tdb->map_ptr = ਨਿਰਾ;
	ਵਾਪਸ 0;
}

ਖਾਲੀ tdb_mmap(ਬਣਤਰ tdb_context *tdb)
{
	ਜੇ (tdb->flags & TDB_ਅੰਦਰੂਨੀ)
		ਵਾਪਸ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_MMAP
	ਜੇ (!(tdb->flags & TDB_NOMMAP)) {
		tdb->map_ptr = mmap(ਨਿਰਾ, tdb->map_ਮਾਪ,
				    PROT_READ|(tdb->read_only? 0:PROT_WRITE),
				    MAP_SHARED|MAP_FILE, tdb->fd, 0);

		/*
		 * NB. When mmap fails it returns MAP_FAILED *NOT* NULL !!!!
		 */

		ਜੇ (tdb->map_ptr == MAP_FAILED) {
			tdb->real_map_ਮਾਪ = 0;
			tdb->map_ptr = ਨਿਰਾ;
			TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_mmap failed for size %d (%s)\n",
				 tdb->map_ਮਾਪ, strerror(errno)));
		}
		tdb->real_map_ਮਾਪ = tdb->map_ਮਾਪ;
	} ਨਹੀਂਤਾਂ {
		tdb->map_ptr = ਨਿਰਾ;
	}
#ਨਹੀਂਤਾਂ
	tdb->map_ptr = ਨਿਰਾ;
#ਜੇ_ਅੰਤ
}

/* expand a file.  we prefer to use ftruncate, as that is what posix
  says to use for mmap expansion */
ਸਥਿਰ ਪੂਰਨ tdb_expand_file(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਮਾਪ, tdb_off_t addition)
{
	ਅੱਖਰ buf[1024];

	ਜੇ (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		ਵਾਪਸ -1;
	}

	ਜੇ (ftruncate(tdb->fd, ਮਾਪ+addition) == -1) {
		ਅੱਖਰ b = 0;
		ਜੇ (pwrite(tdb->fd,  &b, 1, (ਮਾਪ+addition) - 1) != 1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file to %d failed (%s)\n",
				 ਮਾਪ+addition, strerror(errno)));
			ਵਾਪਸ -1;
		}
	}

	/* now fill the file with something. This ensures that the
	   file isn't sparse, which would be very bad if we ran out of
	   disk. This must be done with write, not via mmap */
	memset(buf, TDB_PAD_BYTE, ਆਕਾਰ(buf));
	ਜਦੋਂ (addition) {
		ਪੂਰਨ n = addition>ਆਕਾਰ(buf)?ਆਕਾਰ(buf):addition;
		ਪੂਰਨ ret = pwrite(tdb->fd, buf, n, ਮਾਪ);
		ਜੇ (ret != n) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "expand_file write of %d failed (%s)\n",
				   n, strerror(errno)));
			ਵਾਪਸ -1;
		}
		addition -= n;
		ਮਾਪ += n;
	}
	ਵਾਪਸ 0;
}


/* expand the database at least size bytes by expanding the underlying
   file and doing the mmap again if necessary */
ਪੂਰਨ tdb_expand(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਮਾਪ)
{
	ਬਣਤਰ list_struct rec;
	tdb_off_t ਵਿੱਥ;

	ਜੇ (tdb_ਤਾਲਾ(tdb, -1, F_WRLCK) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "lock failed in tdb_expand\n"));
		ਵਾਪਸ -1;
	}

	/* must know about any previous expansions by another process */
	tdb->methods->tdb_oob(tdb, tdb->map_ਮਾਪ + 1, 1);

	/* always make room for at least 10 more records, and round
           the database up to a multiple of the page size */
	ਮਾਪ = TDB_ALIGN(tdb->map_ਮਾਪ + ਮਾਪ*10, tdb->ਪੰਨਾ_ਮਾਪ) - tdb->map_ਮਾਪ;

	ਜੇ (!(tdb->flags & TDB_ਅੰਦਰੂਨੀ))
		tdb_munmap(tdb);

	/*
	 * We must ensure the file is unmapped before doing this
	 * to ensure consistency with systems like OpenBSD where
	 * writes and mmaps are not consistent.
	 */

	/* expand the file itself */
	ਜੇ (!(tdb->flags & TDB_ਅੰਦਰੂਨੀ)) {
		ਜੇ (tdb->methods->tdb_expand_file(tdb, tdb->map_ਮਾਪ, ਮਾਪ) != 0)
			ਜਾਓ fail;
	}

	tdb->map_ਮਾਪ += ਮਾਪ;

	ਜੇ (tdb->flags & TDB_ਅੰਦਰੂਨੀ) {
		ਅੱਖਰ *new_map_ptr = (ਅੱਖਰ *)realloc(tdb->map_ptr,
						    tdb->map_ਮਾਪ);
		ਜੇ (!new_map_ptr) {
			tdb->map_ਮਾਪ -= ਮਾਪ;
			ਜਾਓ fail;
		}
		tdb->map_ptr = new_map_ptr;
	} ਨਹੀਂਤਾਂ {
		/*
		 * We must ensure the file is remapped before adding the space
		 * to ensure consistency with systems like OpenBSD where
		 * writes and mmaps are not consistent.
		 */

		/* We're ok if the mmap fails as we'll fallback to read/write */
		tdb_mmap(tdb);
	}

	/* form a new freelist record */
	memset(&rec,'\0',ਆਕਾਰ(rec));
	rec.rec_len = ਮਾਪ - ਆਕਾਰ(rec);

	/* link it into the free list */
	ਵਿੱਥ = tdb->map_ਮਾਪ - ਮਾਪ;
	ਜੇ (tdb_free(tdb, ਵਿੱਥ, &rec) == -1)
		ਜਾਓ fail;

	tdb_unlock(tdb, -1, F_WRLCK);
	ਵਾਪਸ 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ਵਾਪਸ -1;
}

/* read/write a tdb_off_t */
ਪੂਰਨ tdb_ofs_read(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਵਿੱਥ, tdb_off_t *d)
{
	ਵਾਪਸ tdb->methods->tdb_read(tdb, ਵਿੱਥ, (ਅੱਖਰ*)d, ਆਕਾਰ(*d), DOCONV());
}

ਪੂਰਨ tdb_ofs_write(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਵਿੱਥ, tdb_off_t *d)
{
	tdb_off_t off = *d;
	ਵਾਪਸ tdb->methods->tdb_write(tdb, ਵਿੱਥ, CONVERT(off), ਆਕਾਰ(*d));
}


/* read a lump of data, allocating the space for it */
ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *tdb_alloc_read(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਵਿੱਥ, tdb_len_t len)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *buf;

	/* some systems don't like zero length malloc */
	ਜੇ (len == 0) {
		len = 1;
	}

	ਜੇ (!(buf = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)ਸਮ੍ਰਿਤੀ_ਦਿਓ(len))) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_OOM;
		TDB_LOG((tdb, TDB_DEBUG_ERROR,"tdb_alloc_read malloc failed len=%d (%s)\n",
			   len, strerror(errno)));
		ਵਾਪਸ TDB_ERRCODE(TDB_ERR_OOM, buf);
	}
	ਜੇ (tdb->methods->tdb_read(tdb, ਵਿੱਥ, buf, len, 0) == -1) {
		ਸੁਰੱਖਿਅਤ_FREE(buf);
		ਵਾਪਸ ਨਿਰਾ;
	}
	ਵਾਪਸ buf;
}

/* Give a piece of tdb data to a parser */

ਪੂਰਨ tdb_parse_ਅੰਕੜੇ(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ,
		   tdb_off_t ਵਿੱਥ, tdb_len_t len,
		   ਪੂਰਨ (*parser)(TDB_ਅੰਕੜੇ ਕੁੰਜੀ, TDB_ਅੰਕੜੇ ਅੰਕੜੇ,
				 ਖਾਲੀ *ਨਿੱਜੀ_ਅੰਕੜੇ),
		   ਖਾਲੀ *ਨਿੱਜੀ_ਅੰਕੜੇ)
{
	TDB_ਅੰਕੜੇ ਅੰਕੜੇ;
	ਪੂਰਨ ਨਤੀਜਾ;

	ਅੰਕੜੇ.dsize = len;

	ਜੇ ((tdb->transaction == ਨਿਰਾ) && (tdb->map_ptr != ਨਿਰਾ)) {
		/*
		 * Optimize by avoiding the malloc/memcpy/free, point the
		 * parser directly at the mmap area.
		 */
		ਜੇ (tdb->methods->tdb_oob(tdb, ਵਿੱਥ+len, 0) != 0) {
			ਵਾਪਸ -1;
		}
		ਅੰਕੜੇ.dptr = ਵਿੱਥ + (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)tdb->map_ptr;
		ਵਾਪਸ parser(ਕੁੰਜੀ, ਅੰਕੜੇ, ਨਿੱਜੀ_ਅੰਕੜੇ);
	}

	ਜੇ (!(ਅੰਕੜੇ.dptr = tdb_alloc_read(tdb, ਵਿੱਥ, len))) {
		ਵਾਪਸ -1;
	}

	ਨਤੀਜਾ = parser(ਕੁੰਜੀ, ਅੰਕੜੇ, ਨਿੱਜੀ_ਅੰਕੜੇ);
	ਮੁਕਤ(ਅੰਕੜੇ.dptr);
	ਵਾਪਸ ਨਤੀਜਾ;
}

/* read/write a record */
ਪੂਰਨ tdb_rec_read(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਵਿੱਥ, ਬਣਤਰ list_struct *rec)
{
	ਜੇ (tdb->methods->tdb_read(tdb, ਵਿੱਥ, rec, ਆਕਾਰ(*rec),DOCONV()) == -1)
		ਵਾਪਸ -1;
	ਜੇ (TDB_BAD_MAGIC(rec)) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_FATAL,"tdb_rec_read bad magic 0x%x at offset=%d\n", rec->magic, ਵਿੱਥ));
		ਵਾਪਸ TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	ਵਾਪਸ tdb->methods->tdb_oob(tdb, rec->ਅਗਲਾ+ਆਕਾਰ(*rec), 0);
}

ਪੂਰਨ tdb_rec_write(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਵਿੱਥ, ਬਣਤਰ list_struct *rec)
{
	ਬਣਤਰ list_struct r = *rec;
	ਵਾਪਸ tdb->methods->tdb_write(tdb, ਵਿੱਥ, CONVERT(r), ਆਕਾਰ(r));
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ tdb_methods io_methods = {
	tdb_read,
	tdb_write,
	tdb_ਅਗਲਾ_hash_chain,
	tdb_oob,
	tdb_expand_file,
	tdb_brlock
};

/*
  initialise the default methods table
*/
ਖਾਲੀ tdb_io_init(ਬਣਤਰ tdb_context *tdb)
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

ਬਣਤਰ tdb_transaction_el {
	ਬਣਤਰ tdb_transaction_el *ਅਗਲਾ, *prev;
	tdb_off_t ਵਿੱਥ;
	tdb_len_t ਲੰਮਾਈ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਅੰਕੜੇ;
};

/*
  hold the context of any current transaction
*/
ਬਣਤਰ tdb_transaction {
	/* we keep a mirrored copy of the tdb hash heads here so
	   tdb_next_hash_chain() can operate efficiently */
	u32 *hash_heads;

	/* the original io methods - used to do IOs to the real db */
	ਅਚਲ ਬਣਤਰ tdb_methods *io_methods;

	/* the list of transaction elements. We use a doubly linked
	   list with a last pointer to allow us to keep the list
	   ordered, with first element at the front of the list. It
	   needs to be doubly linked as the read/write traversals need
	   to be backwards, while the commit needs to be forwards */
	ਬਣਤਰ tdb_transaction_el *elements, *elements_ਆਖਰੀ;

	/* non-zero when an internal transaction error has
	   occurred. All write operations will then fail until the
	   transaction is ended */
	ਪੂਰਨ transaction_error;

	/* when inside a transaction we need to keep track of any
	   nested tdb_transaction_start() calls, as these are allowed,
	   but don't create a new transaction */
	ਪੂਰਨ nesting;

	/* old file size before transaction */
	tdb_len_t old_map_ਮਾਪ;
};


/*
  read while in a transaction. We need to check first if the data is in our list
  of transaction elements, then if not do a real read
*/
ਸਥਿਰ ਪੂਰਨ transaction_read(ਬਣਤਰ tdb_context *tdb, tdb_off_t off, ਖਾਲੀ *buf,
			    tdb_len_t len, ਪੂਰਨ cv)
{
	ਬਣਤਰ tdb_transaction_el *el;

	/* we need to walk the list backwards to get the most recent data */
	ਲਈ (el=tdb->transaction->elements_ਆਖਰੀ;el;el=el->prev) {
		tdb_len_t partial;

		ਜੇ (off+len <= el->ਵਿੱਥ) {
			ਜਾਰੀ;
		}
		ਜੇ (off >= el->ਵਿੱਥ + el->ਲੰਮਾਈ) {
			ਜਾਰੀ;
		}

		/* an overlapping read - needs to be split into up to
		   2 reads and a memcpy */
		ਜੇ (off < el->ਵਿੱਥ) {
			partial = el->ਵਿੱਥ - off;
			ਜੇ (transaction_read(tdb, off, buf, partial, cv) != 0) {
				ਜਾਓ fail;
			}
			len -= partial;
			off += partial;
			buf = (ਖਾਲੀ *)(partial + (ਅੱਖਰ *)buf);
		}
		ਜੇ (off + len <= el->ਵਿੱਥ + el->ਲੰਮਾਈ) {
			partial = len;
		} ਨਹੀਂਤਾਂ {
			partial = el->ਵਿੱਥ + el->ਲੰਮਾਈ - off;
		}
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf, el->ਅੰਕੜੇ + (off - el->ਵਿੱਥ), partial);
		ਜੇ (cv) {
			tdb_convert(buf, len);
		}
		len -= partial;
		off += partial;
		buf = (ਖਾਲੀ *)(partial + (ਅੱਖਰ *)buf);

		ਜੇ (len != 0 && transaction_read(tdb, off, buf, len, cv) != 0) {
			ਜਾਓ fail;
		}

		ਵਾਪਸ 0;
	}

	/* its not in the transaction elements - do a real read */
	ਵਾਪਸ tdb->transaction->io_methods->tdb_read(tdb, off, buf, len, cv);

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_read: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	ਵਾਪਸ -1;
}


/*
  write while in a transaction
*/
ਸਥਿਰ ਪੂਰਨ transaction_write(ਬਣਤਰ tdb_context *tdb, tdb_off_t off,
			     ਅਚਲ ਖਾਲੀ *buf, tdb_len_t len)
{
	ਬਣਤਰ tdb_transaction_el *el, *ਉੱਤਮ_el=ਨਿਰਾ;

	ਜੇ (len == 0) {
		ਵਾਪਸ 0;
	}

	/* if the write is to a hash head, then update the transaction
	   hash heads */
	ਜੇ (len == ਆਕਾਰ(tdb_off_t) && off >= FREELIST_ਸਿਖਰ &&
	    off < FREELIST_ਸਿਖਰ+TDB_HASHTABLE_ਮਾਪ(tdb)) {
		u32 chain = (off-FREELIST_ਸਿਖਰ) / ਆਕਾਰ(tdb_off_t);
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(&tdb->transaction->hash_heads[chain], buf, len);
	}

	/* first see if we can replace an existing entry */
	ਲਈ (el=tdb->transaction->elements_ਆਖਰੀ;el;el=el->prev) {
		tdb_len_t partial;

		ਜੇ (ਉੱਤਮ_el == ਨਿਰਾ && off == el->ਵਿੱਥ+el->ਲੰਮਾਈ) {
			ਉੱਤਮ_el = el;
		}

		ਜੇ (off+len <= el->ਵਿੱਥ) {
			ਜਾਰੀ;
		}
		ਜੇ (off >= el->ਵਿੱਥ + el->ਲੰਮਾਈ) {
			ਜਾਰੀ;
		}

		/* an overlapping write - needs to be split into up to
		   2 writes and a memcpy */
		ਜੇ (off < el->ਵਿੱਥ) {
			partial = el->ਵਿੱਥ - off;
			ਜੇ (transaction_write(tdb, off, buf, partial) != 0) {
				ਜਾਓ fail;
			}
			len -= partial;
			off += partial;
			buf = (ਅਚਲ ਖਾਲੀ *)(partial + (ਅਚਲ ਅੱਖਰ *)buf);
		}
		ਜੇ (off + len <= el->ਵਿੱਥ + el->ਲੰਮਾਈ) {
			partial = len;
		} ਨਹੀਂਤਾਂ {
			partial = el->ਵਿੱਥ + el->ਲੰਮਾਈ - off;
		}
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(el->ਅੰਕੜੇ + (off - el->ਵਿੱਥ), buf, partial);
		len -= partial;
		off += partial;
		buf = (ਅਚਲ ਖਾਲੀ *)(partial + (ਅਚਲ ਅੱਖਰ *)buf);

		ਜੇ (len != 0 && transaction_write(tdb, off, buf, len) != 0) {
			ਜਾਓ fail;
		}

		ਵਾਪਸ 0;
	}

	/* see if we can append the new entry to an existing entry */
	ਜੇ (ਉੱਤਮ_el && ਉੱਤਮ_el->ਵਿੱਥ + ਉੱਤਮ_el->ਲੰਮਾਈ == off &&
	    (off+len < tdb->transaction->old_map_ਮਾਪ ||
	     off > tdb->transaction->old_map_ਮਾਪ)) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਅੰਕੜੇ = ਉੱਤਮ_el->ਅੰਕੜੇ;
		el = ਉੱਤਮ_el;
		el->ਅੰਕੜੇ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)realloc(el->ਅੰਕੜੇ,
						    el->ਲੰਮਾਈ + len);
		ਜੇ (el->ਅੰਕੜੇ == ਨਿਰਾ) {
			tdb->ecode = TDB_ERR_OOM;
			tdb->transaction->transaction_error = 1;
			el->ਅੰਕੜੇ = ਅੰਕੜੇ;
			ਵਾਪਸ -1;
		}
		ਜੇ (buf) {
			ਸਮ੍ਰਿਤੀ_ਨਕਲ(el->ਅੰਕੜੇ + el->ਲੰਮਾਈ, buf, len);
		} ਨਹੀਂਤਾਂ {
			memset(el->ਅੰਕੜੇ + el->ਲੰਮਾਈ, TDB_PAD_BYTE, len);
		}
		el->ਲੰਮਾਈ += len;
		ਵਾਪਸ 0;
	}

	/* add a new entry at the end of the list */
	el = (ਬਣਤਰ tdb_transaction_el *)ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(*el));
	ਜੇ (el == ਨਿਰਾ) {
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		ਵਾਪਸ -1;
	}
	el->ਅਗਲਾ = ਨਿਰਾ;
	el->prev = tdb->transaction->elements_ਆਖਰੀ;
	el->ਵਿੱਥ = off;
	el->ਲੰਮਾਈ = len;
	el->ਅੰਕੜੇ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)ਸਮ੍ਰਿਤੀ_ਦਿਓ(len);
	ਜੇ (el->ਅੰਕੜੇ == ਨਿਰਾ) {
		ਮੁਕਤ(el);
		tdb->ecode = TDB_ERR_OOM;
		tdb->transaction->transaction_error = 1;
		ਵਾਪਸ -1;
	}
	ਜੇ (buf) {
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(el->ਅੰਕੜੇ, buf, len);
	} ਨਹੀਂਤਾਂ {
		memset(el->ਅੰਕੜੇ, TDB_PAD_BYTE, len);
	}
	ਜੇ (el->prev) {
		el->prev->ਅਗਲਾ = el;
	} ਨਹੀਂਤਾਂ {
		tdb->transaction->elements = el;
	}
	tdb->transaction->elements_ਆਖਰੀ = el;
	ਵਾਪਸ 0;

fail:
	TDB_LOG((tdb, TDB_DEBUG_FATAL, "transaction_write: failed at off=%d len=%d\n", off, len));
	tdb->ecode = TDB_ERR_IO;
	tdb->transaction->transaction_error = 1;
	ਵਾਪਸ -1;
}

/*
  accelerated hash chain head search, using the cached hash heads
*/
ਸਥਿਰ ਖਾਲੀ transaction_ਅਗਲਾ_hash_chain(ਬਣਤਰ tdb_context *tdb, u32 *chain)
{
	u32 h = *chain;
	ਲਈ (;h < tdb->ਸਿਰਲੇਖ.hash_ਮਾਪ;h++) {
		/* the +1 takes account of the freelist */
		ਜੇ (0 != tdb->transaction->hash_heads[h+1]) {
			ਰੋਕੋ;
		}
	}
	(*chain) = h;
}

/*
  out of bounds check during a transaction
*/
ਸਥਿਰ ਪੂਰਨ transaction_oob(ਬਣਤਰ tdb_context *tdb, tdb_off_t len,
			   ਪੂਰਨ probe EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਜੇ (len <= tdb->map_ਮਾਪ) {
		ਵਾਪਸ 0;
	}
	ਵਾਪਸ TDB_ERRCODE(TDB_ERR_IO, -1);
}

/*
  transaction version of tdb_expand().
*/
ਸਥਿਰ ਪੂਰਨ transaction_expand_file(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਮਾਪ,
				   tdb_off_t addition)
{
	/* add a write to the transaction elements, so subsequent
	   reads see the zero data */
	ਜੇ (transaction_write(tdb, ਮਾਪ, ਨਿਰਾ, addition) != 0) {
		ਵਾਪਸ -1;
	}

	ਵਾਪਸ 0;
}

/*
  brlock during a transaction - ignore them
*/
ਸਥਿਰ ਪੂਰਨ transaction_brlock(ਬਣਤਰ tdb_context *tdb EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			      tdb_off_t ਵਿੱਥ EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			      ਪੂਰਨ rw_type EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			      ਪੂਰਨ lck_type EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			      ਪੂਰਨ probe EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			      size_t len EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਅਚਲ ਬਣਤਰ tdb_methods transaction_methods = {
	transaction_read,
	transaction_write,
	transaction_ਅਗਲਾ_hash_chain,
	transaction_oob,
	transaction_expand_file,
	transaction_brlock
};


/*
  start a tdb transaction. No token is returned, as only a single
  transaction is allowed to be pending per tdb_context
*/
ਪੂਰਨ tdb_transaction_ਸ਼ੁਰੂ(ਬਣਤਰ tdb_context *tdb)
{
	/* some sanity checks */
	ਜੇ (tdb->read_only || (tdb->flags & TDB_ਅੰਦਰੂਨੀ) || tdb->traverse_read) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction on a read-only or internal db\n"));
		tdb->ecode = TDB_ERR_EINVAL;
		ਵਾਪਸ -1;
	}

	/* cope with nested tdb_transaction_start() calls */
	ਜੇ (tdb->transaction != ਨਿਰਾ) {
		tdb->transaction->nesting++;
		TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_start: nesting %d\n",
			 tdb->transaction->nesting));
		ਵਾਪਸ 0;
	}

	ਜੇ (tdb->ਅੰਕ_locks != 0 || tdb->global_ਤਾਲਾ.ਗਿਣਤੀ_2) {
		/* the caller must not have any locks when starting a
		   transaction as otherwise we'll be screwed by lack
		   of nested locks in posix */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction with locks held\n"));
		tdb->ecode = TDB_ERR_ਤਾਲਾ;
		ਵਾਪਸ -1;
	}

	ਜੇ (tdb->travlocks.ਅਗਲਾ != ਨਿਰਾ) {
		/* you cannot use transactions inside a traverse (although you can use
		   traverse inside a transaction) as otherwise you can end up with
		   deadlock */
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: cannot start a transaction within a traverse\n"));
		tdb->ecode = TDB_ERR_ਤਾਲਾ;
		ਵਾਪਸ -1;
	}

	tdb->transaction = (ਬਣਤਰ tdb_transaction *)
		ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(ਬਣਤਰ tdb_transaction), 1);
	ਜੇ (tdb->transaction == ਨਿਰਾ) {
		tdb->ecode = TDB_ERR_OOM;
		ਵਾਪਸ -1;
	}

	/* get the transaction write lock. This is a blocking lock. As
	   discussed with Volker, there are a number of ways we could
	   make this async, which we will probably do in the future */
	ਜੇ (tdb_transaction_ਤਾਲਾ(tdb, F_WRLCK) == -1) {
		ਸੁਰੱਖਿਅਤ_FREE(tdb->transaction);
		ਵਾਪਸ -1;
	}

	/* get a read lock from the freelist to the end of file. This
	   is upgraded to a write lock during the commit */
	ਜੇ (tdb_brlock(tdb, FREELIST_ਸਿਖਰ, F_RDLCK, F_SETLKW, 0, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to get hash locks\n"));
		tdb->ecode = TDB_ERR_ਤਾਲਾ;
		ਜਾਓ fail;
	}

	/* setup a copy of the hash table heads so the hash scan in
	   traverse can be fast */
	tdb->transaction->hash_heads = (u32 *)
		ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(tdb->ਸਿਰਲੇਖ.hash_ਮਾਪ+1, ਆਕਾਰ(u32));
	ਜੇ (tdb->transaction->hash_heads == ਨਿਰਾ) {
		tdb->ecode = TDB_ERR_OOM;
		ਜਾਓ fail;
	}
	ਜੇ (tdb->methods->tdb_read(tdb, FREELIST_ਸਿਖਰ, tdb->transaction->hash_heads,
				   TDB_HASHTABLE_ਮਾਪ(tdb), 0) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to read hash heads\n"));
		tdb->ecode = TDB_ERR_IO;
		ਜਾਓ fail;
	}

	/* make sure we know about any file expansions already done by
	   anyone else */
	tdb->methods->tdb_oob(tdb, tdb->map_ਮਾਪ + 1, 1);
	tdb->transaction->old_map_ਮਾਪ = tdb->map_ਮਾਪ;

	/* finally hook the io methods, replacing them with
	   transaction specific methods */
	tdb->transaction->io_methods = tdb->methods;
	tdb->methods = &transaction_methods;

	/* by calling this transaction write here, we ensure that we don't grow the
	   transaction linked list due to hash table updates */
	ਜੇ (transaction_write(tdb, FREELIST_ਸਿਖਰ, tdb->transaction->hash_heads,
			      TDB_HASHTABLE_ਮਾਪ(tdb)) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_start: failed to prime hash table\n"));
		tdb->ecode = TDB_ERR_IO;
		tdb->methods = tdb->transaction->io_methods;
		ਜਾਓ fail;
	}

	ਵਾਪਸ 0;

fail:
	tdb_brlock(tdb, FREELIST_ਸਿਖਰ, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	ਸੁਰੱਖਿਅਤ_FREE(tdb->transaction->hash_heads);
	ਸੁਰੱਖਿਅਤ_FREE(tdb->transaction);
	ਵਾਪਸ -1;
}


/*
  cancel the current transaction
*/
ਪੂਰਨ tdb_transaction_cancel(ਬਣਤਰ tdb_context *tdb)
{
	ਜੇ (tdb->transaction == ਨਿਰਾ) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_cancel: no transaction\n"));
		ਵਾਪਸ -1;
	}

	ਜੇ (tdb->transaction->nesting != 0) {
		tdb->transaction->transaction_error = 1;
		tdb->transaction->nesting--;
		ਵਾਪਸ 0;
	}

	tdb->map_ਮਾਪ = tdb->transaction->old_map_ਮਾਪ;

	/* free all the transaction elements */
	ਜਦੋਂ (tdb->transaction->elements) {
		ਬਣਤਰ tdb_transaction_el *el = tdb->transaction->elements;
		tdb->transaction->elements = el->ਅਗਲਾ;
		ਮੁਕਤ(el->ਅੰਕੜੇ);
		ਮੁਕਤ(el);
	}

	/* remove any global lock created during the transaction */
	ਜੇ (tdb->global_ਤਾਲਾ.ਗਿਣਤੀ_2 != 0) {
		tdb_brlock(tdb, FREELIST_ਸਿਖਰ, F_UNLCK, F_SETLKW, 0, 4*tdb->ਸਿਰਲੇਖ.hash_ਮਾਪ);
		tdb->global_ਤਾਲਾ.ਗਿਣਤੀ_2 = 0;
	}

	/* remove any locks created during the transaction */
	ਜੇ (tdb->ਅੰਕ_locks != 0) {
		ਪੂਰਨ i;
		ਲਈ (i=0;i<tdb->ਅੰਕ_lockrecs;i++) {
			tdb_brlock(tdb,FREELIST_ਸਿਖਰ+4*tdb->lockrecs[i].list,
				   F_UNLCK,F_SETLKW, 0, 1);
		}
		tdb->ਅੰਕ_locks = 0;
		tdb->ਅੰਕ_lockrecs = 0;
		ਸੁਰੱਖਿਅਤ_FREE(tdb->lockrecs);
	}

	/* restore the normal io methods */
	tdb->methods = tdb->transaction->io_methods;

	tdb_brlock(tdb, FREELIST_ਸਿਖਰ, F_UNLCK, F_SETLKW, 0, 0);
	tdb_transaction_unlock(tdb);
	ਸੁਰੱਖਿਅਤ_FREE(tdb->transaction->hash_heads);
	ਸੁਰੱਖਿਅਤ_FREE(tdb->transaction);

	ਵਾਪਸ 0;
}

/*
  sync to disk
*/
ਸਥਿਰ ਪੂਰਨ transaction_sync(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਵਿੱਥ, tdb_len_t ਲੰਮਾਈ)
{
	ਜੇ (fsync(tdb->fd) != 0) {
		tdb->ecode = TDB_ERR_IO;
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: fsync failed\n"));
		ਵਾਪਸ -1;
	}
#ਜੇ defined(HAVE_MSYNC) && defined(MS_SYNC)
	ਜੇ (tdb->map_ptr) {
		tdb_off_t moffset = ਵਿੱਥ & ~(tdb->ਪੰਨਾ_ਮਾਪ-1);
		ਜੇ (msync(moffset + (ਅੱਖਰ *)tdb->map_ptr,
			  ਲੰਮਾਈ + (ਵਿੱਥ - moffset), MS_SYNC) != 0) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction: msync failed - %s\n",
				 strerror(errno)));
			ਵਾਪਸ -1;
		}
	}
#ਜੇ_ਅੰਤ
	ਵਾਪਸ 0;
}


/*
  work out how much space the linearised recovery data will consume
*/
ਸਥਿਰ tdb_len_t tdb_recovery_ਮਾਪ(ਬਣਤਰ tdb_context *tdb)
{
	ਬਣਤਰ tdb_transaction_el *el;
	tdb_len_t recovery_ਮਾਪ = 0;

	recovery_ਮਾਪ = ਆਕਾਰ(u32);
	ਲਈ (el=tdb->transaction->elements;el;el=el->ਅਗਲਾ) {
		ਜੇ (el->ਵਿੱਥ >= tdb->transaction->old_map_ਮਾਪ) {
			ਜਾਰੀ;
		}
		recovery_ਮਾਪ += 2*ਆਕਾਰ(tdb_off_t) + el->ਲੰਮਾਈ;
	}

	ਵਾਪਸ recovery_ਮਾਪ;
}

/*
  allocate the recovery area, or use an existing recovery area if it is
  large enough
*/
ਸਥਿਰ ਪੂਰਨ tdb_recovery_allocate(ਬਣਤਰ tdb_context *tdb,
				 tdb_len_t *recovery_ਮਾਪ,
				 tdb_off_t *recovery_ਵਿੱਥ,
				 tdb_len_t *recovery_max_ਮਾਪ)
{
	ਬਣਤਰ list_struct rec;
	ਅਚਲ ਬਣਤਰ tdb_methods *methods = tdb->transaction->io_methods;
	tdb_off_t recovery_ਸਿਰ;

	ਜੇ (tdb_ofs_read(tdb, TDB_RECOVERY_ਸਿਰ, &recovery_ਸਿਰ) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery head\n"));
		ਵਾਪਸ -1;
	}

	rec.rec_len = 0;

	ਜੇ (recovery_ਸਿਰ != 0 &&
	    methods->tdb_read(tdb, recovery_ਸਿਰ, &rec, ਆਕਾਰ(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to read recovery record\n"));
		ਵਾਪਸ -1;
	}

	*recovery_ਮਾਪ = tdb_recovery_ਮਾਪ(tdb);

	ਜੇ (recovery_ਸਿਰ != 0 && *recovery_ਮਾਪ <= rec.rec_len) {
		/* it fits in the existing area */
		*recovery_max_ਮਾਪ = rec.rec_len;
		*recovery_ਵਿੱਥ = recovery_ਸਿਰ;
		ਵਾਪਸ 0;
	}

	/* we need to free up the old recovery area, then allocate a
	   new one at the end of the file. Note that we cannot use
	   tdb_allocate() to allocate the new one as that might return
	   us an area that is being currently used (as of the start of
	   the transaction) */
	ਜੇ (recovery_ਸਿਰ != 0) {
		ਜੇ (tdb_free(tdb, recovery_ਸਿਰ, &rec) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to free previous recovery area\n"));
			ਵਾਪਸ -1;
		}
	}

	/* the tdb_free() call might have increased the recovery size */
	*recovery_ਮਾਪ = tdb_recovery_ਮਾਪ(tdb);

	/* round up to a multiple of page size */
	*recovery_max_ਮਾਪ = TDB_ALIGN(ਆਕਾਰ(rec) + *recovery_ਮਾਪ, tdb->ਪੰਨਾ_ਮਾਪ) - ਆਕਾਰ(rec);
	*recovery_ਵਿੱਥ = tdb->map_ਮਾਪ;
	recovery_ਸਿਰ = *recovery_ਵਿੱਥ;

	ਜੇ (methods->tdb_expand_file(tdb, tdb->transaction->old_map_ਮਾਪ,
				     (tdb->map_ਮਾਪ - tdb->transaction->old_map_ਮਾਪ) +
				     ਆਕਾਰ(rec) + *recovery_max_ਮਾਪ) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to create recovery area\n"));
		ਵਾਪਸ -1;
	}

	/* remap the file (if using mmap) */
	methods->tdb_oob(tdb, tdb->map_ਮਾਪ + 1, 1);

	/* we have to reset the old map size so that we don't try to expand the file
	   again in the transaction commit, which would destroy the recovery area */
	tdb->transaction->old_map_ਮਾਪ = tdb->map_ਮਾਪ;

	/* write the recovery header offset and sync - we can sync without a race here
	   as the magic ptr in the recovery record has not been set */
	CONVERT(recovery_ਸਿਰ);
	ਜੇ (methods->tdb_write(tdb, TDB_RECOVERY_ਸਿਰ,
			       &recovery_ਸਿਰ, ਆਕਾਰ(tdb_off_t)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_recovery_allocate: failed to write recovery head\n"));
		ਵਾਪਸ -1;
	}

	ਵਾਪਸ 0;
}


/*
  setup the recovery data that will be used on a crash during commit
*/
ਸਥਿਰ ਪੂਰਨ transaction_ਤਿਆਰੀ_recovery(ਬਣਤਰ tdb_context *tdb,
				      tdb_off_t *magic_ਵਿੱਥ)
{
	ਬਣਤਰ tdb_transaction_el *el;
	tdb_len_t recovery_ਮਾਪ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਅੰਕੜੇ, *p;
	ਅਚਲ ਬਣਤਰ tdb_methods *methods = tdb->transaction->io_methods;
	ਬਣਤਰ list_struct *rec;
	tdb_off_t recovery_ਵਿੱਥ, recovery_max_ਮਾਪ;
	tdb_off_t old_map_ਮਾਪ = tdb->transaction->old_map_ਮਾਪ;
	u32 magic, tailer;

	/*
	  check that the recovery area has enough space
	*/
	ਜੇ (tdb_recovery_allocate(tdb, &recovery_ਮਾਪ,
				  &recovery_ਵਿੱਥ, &recovery_max_ਮਾਪ) == -1) {
		ਵਾਪਸ -1;
	}

	ਅੰਕੜੇ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)ਸਮ੍ਰਿਤੀ_ਦਿਓ(recovery_ਮਾਪ + ਆਕਾਰ(*rec));
	ਜੇ (ਅੰਕੜੇ == ਨਿਰਾ) {
		tdb->ecode = TDB_ERR_OOM;
		ਵਾਪਸ -1;
	}

	rec = (ਬਣਤਰ list_struct *)ਅੰਕੜੇ;
	memset(rec, 0, ਆਕਾਰ(*rec));

	rec->magic    = 0;
	rec->ਅੰਕੜੇ_len = recovery_ਮਾਪ;
	rec->rec_len  = recovery_max_ਮਾਪ;
	rec->ਕੁੰਜੀ_len  = old_map_ਮਾਪ;
	CONVERT(rec);

	/* build the recovery data into a single blob to allow us to do a single
	   large write, which should be more efficient */
	p = ਅੰਕੜੇ + ਆਕਾਰ(*rec);
	ਲਈ (el=tdb->transaction->elements;el;el=el->ਅਗਲਾ) {
		ਜੇ (el->ਵਿੱਥ >= old_map_ਮਾਪ) {
			ਜਾਰੀ;
		}
		ਜੇ (el->ਵਿੱਥ + el->ਲੰਮਾਈ > tdb->transaction->old_map_ਮਾਪ) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: transaction data over new region boundary\n"));
			ਮੁਕਤ(ਅੰਕੜੇ);
			tdb->ecode = TDB_ERR_CORRUPT;
			ਵਾਪਸ -1;
		}
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(p, &el->ਵਿੱਥ, 4);
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(p+4, &el->ਲੰਮਾਈ, 4);
		ਜੇ (DOCONV()) {
			tdb_convert(p, 8);
		}
		/* the recovery area contains the old data, not the
		   new data, so we have to call the original tdb_read
		   method to get it */
		ਜੇ (methods->tdb_read(tdb, el->ਵਿੱਥ, p + 8, el->ਲੰਮਾਈ, 0) != 0) {
			ਮੁਕਤ(ਅੰਕੜੇ);
			tdb->ecode = TDB_ERR_IO;
			ਵਾਪਸ -1;
		}
		p += 8 + el->ਲੰਮਾਈ;
	}

	/* and the tailer */
	tailer = ਆਕਾਰ(*rec) + recovery_max_ਮਾਪ;
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(p, &tailer, 4);
	CONVERT(p);

	/* write the recovery data to the recovery area */
	ਜੇ (methods->tdb_write(tdb, recovery_ਵਿੱਥ, ਅੰਕੜੇ, ਆਕਾਰ(*rec) + recovery_ਮਾਪ) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery data\n"));
		ਮੁਕਤ(ਅੰਕੜੇ);
		tdb->ecode = TDB_ERR_IO;
		ਵਾਪਸ -1;
	}

	/* as we don't have ordered writes, we have to sync the recovery
	   data before we update the magic to indicate that the recovery
	   data is present */
	ਜੇ (transaction_sync(tdb, recovery_ਵਿੱਥ, ਆਕਾਰ(*rec) + recovery_ਮਾਪ) == -1) {
		ਮੁਕਤ(ਅੰਕੜੇ);
		ਵਾਪਸ -1;
	}

	ਮੁਕਤ(ਅੰਕੜੇ);

	magic = TDB_RECOVERY_MAGIC;
	CONVERT(magic);

	*magic_ਵਿੱਥ = recovery_ਵਿੱਥ + offsetof(ਬਣਤਰ list_struct, magic);

	ਜੇ (methods->tdb_write(tdb, *magic_ਵਿੱਥ, &magic, ਆਕਾਰ(magic)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_setup_recovery: failed to write recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		ਵਾਪਸ -1;
	}

	/* ensure the recovery magic marker is on disk */
	ਜੇ (transaction_sync(tdb, *magic_ਵਿੱਥ, ਆਕਾਰ(magic)) == -1) {
		ਵਾਪਸ -1;
	}

	ਵਾਪਸ 0;
}

/*
  commit the current transaction
*/
ਪੂਰਨ tdb_transaction_commit(ਬਣਤਰ tdb_context *tdb)
{
	ਅਚਲ ਬਣਤਰ tdb_methods *methods;
	tdb_off_t magic_ਵਿੱਥ = 0;
	u32 zero = 0;

	ਜੇ (tdb->transaction == ਨਿਰਾ) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: no transaction\n"));
		ਵਾਪਸ -1;
	}

	ਜੇ (tdb->transaction->transaction_error) {
		tdb->ecode = TDB_ERR_IO;
		tdb_transaction_cancel(tdb);
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: transaction error pending\n"));
		ਵਾਪਸ -1;
	}

	ਜੇ (tdb->transaction->nesting != 0) {
		tdb->transaction->nesting--;
		ਵਾਪਸ 0;
	}

	/* check for a null transaction */
	ਜੇ (tdb->transaction->elements == ਨਿਰਾ) {
		tdb_transaction_cancel(tdb);
		ਵਾਪਸ 0;
	}

	methods = tdb->transaction->io_methods;

	/* if there are any locks pending then the caller has not
	   nested their locks properly, so fail the transaction */
	ਜੇ (tdb->ਅੰਕ_locks || tdb->global_ਤਾਲਾ.ਗਿਣਤੀ_2) {
		tdb->ecode = TDB_ERR_ਤਾਲਾ;
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: locks pending on commit\n"));
		tdb_transaction_cancel(tdb);
		ਵਾਪਸ -1;
	}

	/* upgrade the main transaction lock region to a write lock */
	ਜੇ (tdb_brlock_upgrade(tdb, FREELIST_ਸਿਖਰ, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_start: failed to upgrade hash locks\n"));
		tdb->ecode = TDB_ERR_ਤਾਲਾ;
		tdb_transaction_cancel(tdb);
		ਵਾਪਸ -1;
	}

	/* get the global lock - this prevents new users attaching to the database
	   during the commit */
	ਜੇ (tdb_brlock(tdb, GLOBAL_ਤਾਲਾ, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_transaction_commit: failed to get global lock\n"));
		tdb->ecode = TDB_ERR_ਤਾਲਾ;
		tdb_transaction_cancel(tdb);
		ਵਾਪਸ -1;
	}

	ਜੇ (!(tdb->flags & TDB_NOSYNC)) {
		/* write the recovery data to the end of the file */
		ਜੇ (transaction_ਤਿਆਰੀ_recovery(tdb, &magic_ਵਿੱਥ) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to setup recovery data\n"));
			tdb_brlock(tdb, GLOBAL_ਤਾਲਾ, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			ਵਾਪਸ -1;
		}
	}

	/* expand the file to the new size if needed */
	ਜੇ (tdb->map_ਮਾਪ != tdb->transaction->old_map_ਮਾਪ) {
		ਜੇ (methods->tdb_expand_file(tdb, tdb->transaction->old_map_ਮਾਪ,
					     tdb->map_ਮਾਪ -
					     tdb->transaction->old_map_ਮਾਪ) == -1) {
			tdb->ecode = TDB_ERR_IO;
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: expansion failed\n"));
			tdb_brlock(tdb, GLOBAL_ਤਾਲਾ, F_UNLCK, F_SETLKW, 0, 1);
			tdb_transaction_cancel(tdb);
			ਵਾਪਸ -1;
		}
		tdb->map_ਮਾਪ = tdb->transaction->old_map_ਮਾਪ;
		methods->tdb_oob(tdb, tdb->map_ਮਾਪ + 1, 1);
	}

	/* perform all the writes */
	ਜਦੋਂ (tdb->transaction->elements) {
		ਬਣਤਰ tdb_transaction_el *el = tdb->transaction->elements;

		ਜੇ (methods->tdb_write(tdb, el->ਵਿੱਥ, el->ਅੰਕੜੇ, el->ਲੰਮਾਈ) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed during commit\n"));

			/* we've overwritten part of the data and
			   possibly expanded the file, so we need to
			   run the crash recovery code */
			tdb->methods = methods;
			tdb_transaction_recover(tdb);

			tdb_transaction_cancel(tdb);
			tdb_brlock(tdb, GLOBAL_ਤਾਲਾ, F_UNLCK, F_SETLKW, 0, 1);

			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: write failed\n"));
			ਵਾਪਸ -1;
		}
		tdb->transaction->elements = el->ਅਗਲਾ;
		ਮੁਕਤ(el->ਅੰਕੜੇ);
		ਮੁਕਤ(el);
	}

	ਜੇ (!(tdb->flags & TDB_NOSYNC)) {
		/* ensure the new data is on disk */
		ਜੇ (transaction_sync(tdb, 0, tdb->map_ਮਾਪ) == -1) {
			ਵਾਪਸ -1;
		}

		/* remove the recovery marker */
		ਜੇ (methods->tdb_write(tdb, magic_ਵਿੱਥ, &zero, 4) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_commit: failed to remove recovery magic\n"));
			ਵਾਪਸ -1;
		}

		/* ensure the recovery marker has been removed on disk */
		ਜੇ (transaction_sync(tdb, magic_ਵਿੱਥ, 4) == -1) {
			ਵਾਪਸ -1;
		}
	}

	tdb_brlock(tdb, GLOBAL_ਤਾਲਾ, F_UNLCK, F_SETLKW, 0, 1);

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
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_UTIME
	utime(tdb->name, ਨਿਰਾ);
#ਜੇ_ਅੰਤ

	/* use a transaction cancel to free memory and remove the
	   transaction locks */
	tdb_transaction_cancel(tdb);
	ਵਾਪਸ 0;
}


/*
  recover from an aborted transaction. Must be called with exclusive
  database write access already established (including the global
  lock to prevent new processes attaching)
*/
ਪੂਰਨ tdb_transaction_recover(ਬਣਤਰ tdb_context *tdb)
{
	tdb_off_t recovery_ਸਿਰ, recovery_eof;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਅੰਕੜੇ, *p;
	u32 zero = 0;
	ਬਣਤਰ list_struct rec;

	/* find the recovery area */
	ਜੇ (tdb_ofs_read(tdb, TDB_RECOVERY_ਸਿਰ, &recovery_ਸਿਰ) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery head\n"));
		tdb->ecode = TDB_ERR_IO;
		ਵਾਪਸ -1;
	}

	ਜੇ (recovery_ਸਿਰ == 0) {
		/* we have never allocated a recovery record */
		ਵਾਪਸ 0;
	}

	/* read the recovery record */
	ਜੇ (tdb->methods->tdb_read(tdb, recovery_ਸਿਰ, &rec,
				   ਆਕਾਰ(rec), DOCONV()) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery record\n"));
		tdb->ecode = TDB_ERR_IO;
		ਵਾਪਸ -1;
	}

	ਜੇ (rec.magic != TDB_RECOVERY_MAGIC) {
		/* there is no valid recovery data */
		ਵਾਪਸ 0;
	}

	ਜੇ (tdb->read_only) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: attempt to recover read only database\n"));
		tdb->ecode = TDB_ERR_CORRUPT;
		ਵਾਪਸ -1;
	}

	recovery_eof = rec.ਕੁੰਜੀ_len;

	ਅੰਕੜੇ = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)ਸਮ੍ਰਿਤੀ_ਦਿਓ(rec.ਅੰਕੜੇ_len);
	ਜੇ (ਅੰਕੜੇ == ਨਿਰਾ) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to allocate recovery data\n"));
		tdb->ecode = TDB_ERR_OOM;
		ਵਾਪਸ -1;
	}

	/* read the full recovery data */
	ਜੇ (tdb->methods->tdb_read(tdb, recovery_ਸਿਰ + ਆਕਾਰ(rec), ਅੰਕੜੇ,
				   rec.ਅੰਕੜੇ_len, 0) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to read recovery data\n"));
		tdb->ecode = TDB_ERR_IO;
		ਮੁਕਤ(ਅੰਕੜੇ);
		ਵਾਪਸ -1;
	}

	/* recover the file data */
	p = ਅੰਕੜੇ;
	ਜਦੋਂ (p+8 < ਅੰਕੜੇ + rec.ਅੰਕੜੇ_len) {
		u32 ofs, len;
		ਜੇ (DOCONV()) {
			tdb_convert(p, 8);
		}
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(&ofs, p, 4);
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(&len, p+4, 4);

		ਜੇ (tdb->methods->tdb_write(tdb, ofs, p+8, len) == -1) {
			ਮੁਕਤ(ਅੰਕੜੇ);
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to recover %d bytes at offset %d\n", len, ofs));
			tdb->ecode = TDB_ERR_IO;
			ਵਾਪਸ -1;
		}
		p += 8 + len;
	}

	ਮੁਕਤ(ਅੰਕੜੇ);

	ਜੇ (transaction_sync(tdb, 0, tdb->map_ਮਾਪ) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		ਵਾਪਸ -1;
	}

	/* if the recovery area is after the recovered eof then remove it */
	ਜੇ (recovery_eof <= recovery_ਸਿਰ) {
		ਜੇ (tdb_ofs_write(tdb, TDB_RECOVERY_ਸਿਰ, &zero) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery head\n"));
			tdb->ecode = TDB_ERR_IO;
			ਵਾਪਸ -1;
		}
	}

	/* remove the recovery magic */
	ਜੇ (tdb_ofs_write(tdb, recovery_ਸਿਰ + offsetof(ਬਣਤਰ list_struct, magic),
			  &zero) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to remove recovery magic\n"));
		tdb->ecode = TDB_ERR_IO;
		ਵਾਪਸ -1;
	}

	/* reduce the file size to the old size */
	tdb_munmap(tdb);
	ਜੇ (ftruncate(tdb->fd, recovery_eof) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to reduce to recovery size\n"));
		tdb->ecode = TDB_ERR_IO;
		ਵਾਪਸ -1;
	}
	tdb->map_ਮਾਪ = recovery_eof;
	tdb_mmap(tdb);

	ਜੇ (transaction_sync(tdb, 0, recovery_eof) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_transaction_recover: failed to sync2 recovery\n"));
		tdb->ecode = TDB_ERR_IO;
		ਵਾਪਸ -1;
	}

	TDB_LOG((tdb, TDB_DEBUG_TRACE, "tdb_transaction_recover: recovered %d byte database\n",
		 recovery_eof));

	/* all done */
	ਵਾਪਸ 0;
}

/* file: freelist.c */

/* read a freelist record and check for simple errors */
ਸਥਿਰ ਪੂਰਨ tdb_rec_free_read(ਬਣਤਰ tdb_context *tdb, tdb_off_t off, ਬਣਤਰ list_struct *rec)
{
	ਜੇ (tdb->methods->tdb_read(tdb, off, rec, ਆਕਾਰ(*rec),DOCONV()) == -1)
		ਵਾਪਸ -1;

	ਜੇ (rec->magic == TDB_MAGIC) {
		/* this happens when a app is showdown while deleting a record - we should
		   not completely fail when this happens */
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read non-free magic 0x%x at offset=%d - fixing\n",
			 rec->magic, off));
		rec->magic = TDB_FREE_MAGIC;
		ਜੇ (tdb->methods->tdb_write(tdb, off, rec, ਆਕਾਰ(*rec)) == -1)
			ਵਾਪਸ -1;
	}

	ਜੇ (rec->magic != TDB_FREE_MAGIC) {
		/* Ensure ecode is set for log fn. */
		tdb->ecode = TDB_ERR_CORRUPT;
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_rec_free_read bad magic 0x%x at offset=%d\n",
			   rec->magic, off));
		ਵਾਪਸ TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
	}
	ਜੇ (tdb->methods->tdb_oob(tdb, rec->ਅਗਲਾ+ਆਕਾਰ(*rec), 0) != 0)
		ਵਾਪਸ -1;
	ਵਾਪਸ 0;
}



/* Remove an element from the freelist.  Must have alloc lock. */
ਸਥਿਰ ਪੂਰਨ remove_from_freelist(ਬਣਤਰ tdb_context *tdb, tdb_off_t off, tdb_off_t ਅਗਲਾ)
{
	tdb_off_t ਆਖਰੀ_ptr, i;

	/* read in the freelist top */
	ਆਖਰੀ_ptr = FREELIST_ਸਿਖਰ;
	ਜਦੋਂ (tdb_ofs_read(tdb, ਆਖਰੀ_ptr, &i) != -1 && i != 0) {
		ਜੇ (i == off) {
			/* We've found it! */
			ਵਾਪਸ tdb_ofs_write(tdb, ਆਖਰੀ_ptr, &ਅਗਲਾ);
		}
		/* Follow chain (next offset is at start of record) */
		ਆਖਰੀ_ptr = i;
	}
	TDB_LOG((tdb, TDB_DEBUG_FATAL,"remove_from_freelist: not on list at off=%d\n", off));
	ਵਾਪਸ TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
}


/* update a record tailer (must hold allocation lock) */
ਸਥਿਰ ਪੂਰਨ update_tailer(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਵਿੱਥ,
			 ਅਚਲ ਬਣਤਰ list_struct *rec)
{
	tdb_off_t totalsize;

	/* Offset of tailer from record header */
	totalsize = ਆਕਾਰ(*rec) + rec->rec_len;
	ਵਾਪਸ tdb_ofs_write(tdb, ਵਿੱਥ + totalsize - ਆਕਾਰ(tdb_off_t),
			 &totalsize);
}

/* Add an element into the freelist. Merge adjacent records if
   necessary. */
ਪੂਰਨ tdb_free(ਬਣਤਰ tdb_context *tdb, tdb_off_t ਵਿੱਥ, ਬਣਤਰ list_struct *rec)
{
	tdb_off_t ਸੱਜਾ, ਖੱਬਾ;

	/* Allocation and tailer lock */
	ਜੇ (tdb_ਤਾਲਾ(tdb, -1, F_WRLCK) != 0)
		ਵਾਪਸ -1;

	/* set an initial tailer, so if we fail we don't leave a bogus record */
	ਜੇ (update_tailer(tdb, ਵਿੱਥ, rec) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed!\n"));
		ਜਾਓ fail;
	}

	/* Look right first (I'm an Australian, dammit) */
	ਸੱਜਾ = ਵਿੱਥ + ਆਕਾਰ(*rec) + rec->rec_len;
	ਜੇ (ਸੱਜਾ + ਆਕਾਰ(*rec) <= tdb->map_ਮਾਪ) {
		ਬਣਤਰ list_struct r;

		ਜੇ (tdb->methods->tdb_read(tdb, ਸੱਜਾ, &r, ਆਕਾਰ(r), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right read failed at %u\n", ਸੱਜਾ));
			ਜਾਓ ਖੱਬਾ;
		}

		/* If it's free, expand to include it. */
		ਜੇ (r.magic == TDB_FREE_MAGIC) {
			ਜੇ (remove_from_freelist(tdb, ਸੱਜਾ, r.ਅਗਲਾ) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: right free failed at %u\n", ਸੱਜਾ));
				ਜਾਓ ਖੱਬਾ;
			}
			rec->rec_len += ਆਕਾਰ(r) + r.rec_len;
		}
	}

ਖੱਬਾ:
	/* Look left */
	ਖੱਬਾ = ਵਿੱਥ - ਆਕਾਰ(tdb_off_t);
	ਜੇ (ਖੱਬਾ > TDB_ਅੰਕੜੇ_ਸ਼ੁਰੂ(tdb->ਸਿਰਲੇਖ.hash_ਮਾਪ)) {
		ਬਣਤਰ list_struct l;
		tdb_off_t leftsize;

		/* Read in tailer and jump back to header */
		ਜੇ (tdb_ofs_read(tdb, ਖੱਬਾ, &leftsize) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left offset read failed at %u\n", ਖੱਬਾ));
			ਜਾਓ update;
		}

		/* it could be uninitialised data */
		ਜੇ (leftsize == 0 || leftsize == TDB_PAD_U32) {
			ਜਾਓ update;
		}

		ਖੱਬਾ = ਵਿੱਥ - leftsize;

		/* Now read in record */
		ਜੇ (tdb->methods->tdb_read(tdb, ਖੱਬਾ, &l, ਆਕਾਰ(l), DOCONV()) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left read failed at %u (%u)\n", ਖੱਬਾ, leftsize));
			ਜਾਓ update;
		}

		/* If it's free, expand to include it. */
		ਜੇ (l.magic == TDB_FREE_MAGIC) {
			ਜੇ (remove_from_freelist(tdb, ਖੱਬਾ, l.ਅਗਲਾ) == -1) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: left free failed at %u\n", ਖੱਬਾ));
				ਜਾਓ update;
			} ਨਹੀਂਤਾਂ {
				ਵਿੱਥ = ਖੱਬਾ;
				rec->rec_len += leftsize;
			}
		}
	}

update:
	ਜੇ (update_tailer(tdb, ਵਿੱਥ, rec) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free: update_tailer failed at %u\n", ਵਿੱਥ));
		ਜਾਓ fail;
	}

	/* Now, prepend to free list */
	rec->magic = TDB_FREE_MAGIC;

	ਜੇ (tdb_ofs_read(tdb, FREELIST_ਸਿਖਰ, &rec->ਅਗਲਾ) == -1 ||
	    tdb_rec_write(tdb, ਵਿੱਥ, rec) == -1 ||
	    tdb_ofs_write(tdb, FREELIST_ਸਿਖਰ, &ਵਿੱਥ) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_free record write failed at offset=%d\n", ਵਿੱਥ));
		ਜਾਓ fail;
	}

	/* And we're done. */
	tdb_unlock(tdb, -1, F_WRLCK);
	ਵਾਪਸ 0;

 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ਵਾਪਸ -1;
}


/*
   the core of tdb_allocate - called when we have decided which
   free list entry to use
 */
ਸਥਿਰ tdb_off_t tdb_allocate_ofs(ਬਣਤਰ tdb_context *tdb, tdb_len_t ਲੰਮਾਈ, tdb_off_t rec_ptr,
				ਬਣਤਰ list_struct *rec, tdb_off_t ਆਖਰੀ_ptr)
{
	ਬਣਤਰ list_struct newrec;
	tdb_off_t newrec_ptr;

	memset(&newrec, '\0', ਆਕਾਰ(newrec));

	/* found it - now possibly split it up  */
	ਜੇ (rec->rec_len > ਲੰਮਾਈ + MIN_REC_ਮਾਪ) {
		/* Length of left piece */
		ਲੰਮਾਈ = TDB_ALIGN(ਲੰਮਾਈ, TDB_ALIGNMENT);

		/* Right piece to go on free list */
		newrec.rec_len = rec->rec_len - (ਆਕਾਰ(*rec) + ਲੰਮਾਈ);
		newrec_ptr = rec_ptr + ਆਕਾਰ(*rec) + ਲੰਮਾਈ;

		/* And left record is shortened */
		rec->rec_len = ਲੰਮਾਈ;
	} ਨਹੀਂਤਾਂ {
		newrec_ptr = 0;
	}

	/* Remove allocated record from the free list */
	ਜੇ (tdb_ofs_write(tdb, ਆਖਰੀ_ptr, &rec->ਅਗਲਾ) == -1) {
		ਵਾਪਸ 0;
	}

	/* Update header: do this before we drop alloc
	   lock, otherwise tdb_free() might try to
	   merge with us, thinking we're free.
	   (Thanks Jeremy Allison). */
	rec->magic = TDB_MAGIC;
	ਜੇ (tdb_rec_write(tdb, rec_ptr, rec) == -1) {
		ਵਾਪਸ 0;
	}

	/* Did we create new block? */
	ਜੇ (newrec_ptr) {
		/* Update allocated record tailer (we
		   shortened it). */
		ਜੇ (update_tailer(tdb, rec_ptr, rec) == -1) {
			ਵਾਪਸ 0;
		}

		/* Free new record */
		ਜੇ (tdb_free(tdb, newrec_ptr, &newrec) == -1) {
			ਵਾਪਸ 0;
		}
	}

	/* all done - return the new record offset */
	ਵਾਪਸ rec_ptr;
}

/* allocate some space from the free list. The offset returned points
   to a unconnected list_struct within the database with room for at
   least length bytes of total data

   0 is returned if the space could not be allocated
 */
tdb_off_t tdb_allocate(ਬਣਤਰ tdb_context *tdb, tdb_len_t ਲੰਮਾਈ, ਬਣਤਰ list_struct *rec)
{
	tdb_off_t rec_ptr, ਆਖਰੀ_ptr, newrec_ptr;
	ਬਣਤਰ {
		tdb_off_t rec_ptr, ਆਖਰੀ_ptr;
		tdb_len_t rec_len;
	} bestfit;

	ਜੇ (tdb_ਤਾਲਾ(tdb, -1, F_WRLCK) == -1)
		ਵਾਪਸ 0;

	/* Extra bytes required for tailer */
	ਲੰਮਾਈ += ਆਕਾਰ(tdb_off_t);

 again:
	ਆਖਰੀ_ptr = FREELIST_ਸਿਖਰ;

	/* read in the freelist top */
	ਜੇ (tdb_ofs_read(tdb, FREELIST_ਸਿਖਰ, &rec_ptr) == -1)
		ਜਾਓ fail;

	bestfit.rec_ptr = 0;
	bestfit.ਆਖਰੀ_ptr = 0;
	bestfit.rec_len = 0;

	/*
	   this is a best fit allocation strategy. Originally we used
	   a first fit strategy, but it suffered from massive fragmentation
	   issues when faced with a slowly increasing record size.
	 */
	ਜਦੋਂ (rec_ptr) {
		ਜੇ (tdb_rec_free_read(tdb, rec_ptr, rec) == -1) {
			ਜਾਓ fail;
		}

		ਜੇ (rec->rec_len >= ਲੰਮਾਈ) {
			ਜੇ (bestfit.rec_ptr == 0 ||
			    rec->rec_len < bestfit.rec_len) {
				bestfit.rec_len = rec->rec_len;
				bestfit.rec_ptr = rec_ptr;
				bestfit.ਆਖਰੀ_ptr = ਆਖਰੀ_ptr;
				/* consider a fit to be good enough if
				   we aren't wasting more than half
				   the space */
				ਜੇ (bestfit.rec_len < 2*ਲੰਮਾਈ) {
					ਰੋਕੋ;
				}
			}
		}

		/* move to the next record */
		ਆਖਰੀ_ptr = rec_ptr;
		rec_ptr = rec->ਅਗਲਾ;
	}

	ਜੇ (bestfit.rec_ptr != 0) {
		ਜੇ (tdb_rec_free_read(tdb, bestfit.rec_ptr, rec) == -1) {
			ਜਾਓ fail;
		}

		newrec_ptr = tdb_allocate_ofs(tdb, ਲੰਮਾਈ, bestfit.rec_ptr, rec, bestfit.ਆਖਰੀ_ptr);
		tdb_unlock(tdb, -1, F_WRLCK);
		ਵਾਪਸ newrec_ptr;
	}

	/* we didn't find enough space. See if we can expand the
	   database and if we can then try again */
	ਜੇ (tdb_expand(tdb, ਲੰਮਾਈ + ਆਕਾਰ(*rec)) == 0)
		ਜਾਓ again;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ਵਾਪਸ 0;
}

/* file: freelistcheck.c */

/* Check the freelist is good and contains no loops.
   Very memory intensive - only do this as a consistency
   checker. Heh heh - uses an in memory tdb as the storage
   for the "seen" record list. For some reason this strikes
   me as extremely clever as I don't have to write another tree
   data structure implementation :-).
 */

ਸਥਿਰ ਪੂਰਨ ਵੇਖਿਆ_ਪਾਓ(ਬਣਤਰ tdb_context *mem_tdb, tdb_off_t rec_ptr)
{
	TDB_ਅੰਕੜੇ ਕੁੰਜੀ, ਅੰਕੜੇ;

	memset(&ਅੰਕੜੇ, '\0', ਆਕਾਰ(ਅੰਕੜੇ));
	ਕੁੰਜੀ.dptr = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)&rec_ptr;
	ਕੁੰਜੀ.dsize = ਆਕਾਰ(rec_ptr);
	ਵਾਪਸ tdb_store(mem_tdb, ਕੁੰਜੀ, ਅੰਕੜੇ, TDB_ਪਾਓ);
}

ਪੂਰਨ tdb_validate_freelist(ਬਣਤਰ tdb_context *tdb, ਪੂਰਨ *pnum_entries)
{
	ਬਣਤਰ tdb_context *mem_tdb = ਨਿਰਾ;
	ਬਣਤਰ list_struct rec;
	tdb_off_t rec_ptr, ਆਖਰੀ_ptr;
	ਪੂਰਨ ret = -1;

	*pnum_entries = 0;

	mem_tdb = tdb_open("flval", tdb->ਸਿਰਲੇਖ.hash_ਮਾਪ,
				TDB_ਅੰਦਰੂਨੀ, O_RDWR, 0600);
	ਜੇ (!mem_tdb) {
		ਵਾਪਸ -1;
	}

	ਜੇ (tdb_ਤਾਲਾ(tdb, -1, F_WRLCK) == -1) {
		tdb_close(mem_tdb);
		ਵਾਪਸ 0;
	}

	ਆਖਰੀ_ptr = FREELIST_ਸਿਖਰ;

	/* Store the FREELIST_TOP record. */
	ਜੇ (ਵੇਖਿਆ_ਪਾਓ(mem_tdb, ਆਖਰੀ_ptr) == -1) {
		ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
		ਜਾਓ fail;
	}

	/* read in the freelist top */
	ਜੇ (tdb_ofs_read(tdb, FREELIST_ਸਿਖਰ, &rec_ptr) == -1) {
		ਜਾਓ fail;
	}

	ਜਦੋਂ (rec_ptr) {

		/* If we can't store this record (we've seen it
		   before) then the free list has a loop and must
		   be corrupt. */

		ਜੇ (ਵੇਖਿਆ_ਪਾਓ(mem_tdb, rec_ptr)) {
			ret = TDB_ERRCODE(TDB_ERR_CORRUPT, -1);
			ਜਾਓ fail;
		}

		ਜੇ (tdb_rec_free_read(tdb, rec_ptr, &rec) == -1) {
			ਜਾਓ fail;
		}

		/* move to the next record */
		ਆਖਰੀ_ptr = rec_ptr;
		rec_ptr = rec.ਅਗਲਾ;
		*pnum_entries += 1;
	}

	ret = 0;

  fail:

	tdb_close(mem_tdb);
	tdb_unlock(tdb, -1, F_WRLCK);
	ਵਾਪਸ ret;
}

/* file: traverse.c */

/* Uses traverse lock: 0 = finish, -1 = error, other = record offset */
ਸਥਿਰ ਪੂਰਨ tdb_ਅਗਲਾ_ਤਾਲਾ(ਬਣਤਰ tdb_context *tdb, ਬਣਤਰ tdb_traverse_ਤਾਲਾ *tlock,
			 ਬਣਤਰ list_struct *rec)
{
	ਪੂਰਨ want_ਅਗਲਾ = (tlock->off != 0);

	/* Lock each chain from the start one. */
	ਲਈ (; tlock->hash < tdb->ਸਿਰਲੇਖ.hash_ਮਾਪ; tlock->hash++) {
		ਜੇ (!tlock->off && tlock->hash != 0) {
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
			tdb->methods->ਅਗਲਾ_hash_chain(tdb, &tlock->hash);
			ਜੇ (tlock->hash == tdb->ਸਿਰਲੇਖ.hash_ਮਾਪ) {
				ਜਾਰੀ;
			}
		}

		ਜੇ (tdb_ਤਾਲਾ(tdb, tlock->hash, tlock->ਤਾਲਾ_rw) == -1)
			ਵਾਪਸ -1;

		/* No previous record?  Start at top of chain. */
		ਜੇ (!tlock->off) {
			ਜੇ (tdb_ofs_read(tdb, TDB_HASH_ਸਿਖਰ(tlock->hash),
				     &tlock->off) == -1)
				ਜਾਓ fail;
		} ਨਹੀਂਤਾਂ {
			/* Otherwise unlock the previous record. */
			ਜੇ (tdb_unlock_ਰਿਕਾਰਡ(tdb, tlock->off) != 0)
				ਜਾਓ fail;
		}

		ਜੇ (want_ਅਗਲਾ) {
			/* We have offset of old record: grab next */
			ਜੇ (tdb_rec_read(tdb, tlock->off, rec) == -1)
				ਜਾਓ fail;
			tlock->off = rec->ਅਗਲਾ;
		}

		/* Iterate through chain */
		ਜਦੋਂ( tlock->off) {
			tdb_off_t current;
			ਜੇ (tdb_rec_read(tdb, tlock->off, rec) == -1)
				ਜਾਓ fail;

			/* Detect infinite loops. From "Shlomi Yaakobovich" <Shlomi@exanet.com>. */
			ਜੇ (tlock->off == rec->ਅਗਲਾ) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: loop detected.\n"));
				ਜਾਓ fail;
			}

			ਜੇ (!TDB_DEAD(rec)) {
				/* Woohoo: we found one! */
				ਜੇ (tdb_ਤਾਲਾ_ਰਿਕਾਰਡ(tdb, tlock->off) != 0)
					ਜਾਓ fail;
				ਵਾਪਸ tlock->off;
			}

			/* Try to clean dead ones from old traverses */
			current = tlock->off;
			tlock->off = rec->ਅਗਲਾ;
			ਜੇ (!(tdb->read_only || tdb->traverse_read) &&
			    tdb_do_delete(tdb, current, rec) != 0)
				ਜਾਓ fail;
		}
		tdb_unlock(tdb, tlock->hash, tlock->ਤਾਲਾ_rw);
		want_ਅਗਲਾ = 0;
	}
	/* We finished iteration without finding anything */
	ਵਾਪਸ TDB_ERRCODE(TDB_SUCCESS, 0);

 fail:
	tlock->off = 0;
	ਜੇ (tdb_unlock(tdb, tlock->hash, tlock->ਤਾਲਾ_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_next_lock: On error unlock failed!\n"));
	ਵਾਪਸ -1;
}

/* traverse the entire database - calling fn(tdb, key, data) on each element.
   return -1 on error or the record count traversed
   if fn is NULL then it is not called
   a non-zero return value from fn() indicates that the traversal should stop
  */
ਸਥਿਰ ਪੂਰਨ tdb_traverse_ਅੰਦਰੂਨੀ(ਬਣਤਰ tdb_context *tdb,
				 tdb_traverse_func fn, ਖਾਲੀ *ਨਿੱਜੀ_ਅੰਕੜੇ,
				 ਬਣਤਰ tdb_traverse_ਤਾਲਾ *tl)
{
	TDB_ਅੰਕੜੇ ਕੁੰਜੀ, dbuf;
	ਬਣਤਰ list_struct rec;
	ਪੂਰਨ ret, ਗਿਣਤੀ_2 = 0;

	/* This was in the initialization, above, but the IRIX compiler
	 * did not like it.  crh
	 */
	tl->ਅਗਲਾ = tdb->travlocks.ਅਗਲਾ;

	/* fcntl locks don't stack: beware traverse inside traverse */
	tdb->travlocks.ਅਗਲਾ = tl;

	/* tdb_next_lock places locks on the record returned, and its chain */
	ਜਦੋਂ ((ret = tdb_ਅਗਲਾ_ਤਾਲਾ(tdb, tl, &rec)) > 0) {
		ਗਿਣਤੀ_2++;
		/* now read the full record */
		ਕੁੰਜੀ.dptr = tdb_alloc_read(tdb, tl->off + ਆਕਾਰ(rec),
					  rec.ਕੁੰਜੀ_len + rec.ਅੰਕੜੇ_len);
		ਜੇ (!ਕੁੰਜੀ.dptr) {
			ret = -1;
			ਜੇ (tdb_unlock(tdb, tl->hash, tl->ਤਾਲਾ_rw) != 0)
				ਜਾਓ ਬਾਹਰਲਾ;
			ਜੇ (tdb_unlock_ਰਿਕਾਰਡ(tdb, tl->off) != 0)
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: key.dptr == NULL and unlock_record failed!\n"));
			ਜਾਓ ਬਾਹਰਲਾ;
		}
		ਕੁੰਜੀ.dsize = rec.ਕੁੰਜੀ_len;
		dbuf.dptr = ਕੁੰਜੀ.dptr + rec.ਕੁੰਜੀ_len;
		dbuf.dsize = rec.ਅੰਕੜੇ_len;

		/* Drop chain lock, call out */
		ਜੇ (tdb_unlock(tdb, tl->hash, tl->ਤਾਲਾ_rw) != 0) {
			ret = -1;
			ਸੁਰੱਖਿਅਤ_FREE(ਕੁੰਜੀ.dptr);
			ਜਾਓ ਬਾਹਰਲਾ;
		}
		ਜੇ (fn && fn(tdb, ਕੁੰਜੀ, dbuf, ਨਿੱਜੀ_ਅੰਕੜੇ)) {
			/* They want us to terminate traversal */
			ret = ਗਿਣਤੀ_2;
			ਜੇ (tdb_unlock_ਰਿਕਾਰਡ(tdb, tl->off) != 0) {
				TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_traverse: unlock_record failed!\n"));;
				ret = -1;
			}
			ਸੁਰੱਖਿਅਤ_FREE(ਕੁੰਜੀ.dptr);
			ਜਾਓ ਬਾਹਰਲਾ;
		}
		ਸੁਰੱਖਿਅਤ_FREE(ਕੁੰਜੀ.dptr);
	}
ਬਾਹਰਲਾ:
	tdb->travlocks.ਅਗਲਾ = tl->ਅਗਲਾ;
	ਜੇ (ret < 0)
		ਵਾਪਸ -1;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ ਗਿਣਤੀ_2;
}


/*
  a write style traverse - temporarily marks the db read only
*/
ਪੂਰਨ tdb_traverse_read(ਬਣਤਰ tdb_context *tdb,
		      tdb_traverse_func fn, ਖਾਲੀ *ਨਿੱਜੀ_ਅੰਕੜੇ)
{
	ਬਣਤਰ tdb_traverse_ਤਾਲਾ tl = { ਨਿਰਾ, 0, 0, F_RDLCK };
	ਪੂਰਨ ret;

	/* we need to get a read lock on the transaction lock here to
	   cope with the lock ordering semantics of solaris10 */
	ਜੇ (tdb_transaction_ਤਾਲਾ(tdb, F_RDLCK)) {
		ਵਾਪਸ -1;
	}

	tdb->traverse_read++;
	ret = tdb_traverse_ਅੰਦਰੂਨੀ(tdb, fn, ਨਿੱਜੀ_ਅੰਕੜੇ, &tl);
	tdb->traverse_read--;

	tdb_transaction_unlock(tdb);

	ਵਾਪਸ ret;
}

/*
  a write style traverse - needs to get the transaction lock to
  prevent deadlocks
*/
ਪੂਰਨ tdb_traverse(ਬਣਤਰ tdb_context *tdb,
		 tdb_traverse_func fn, ਖਾਲੀ *ਨਿੱਜੀ_ਅੰਕੜੇ)
{
	ਬਣਤਰ tdb_traverse_ਤਾਲਾ tl = { ਨਿਰਾ, 0, 0, F_WRLCK };
	ਪੂਰਨ ret;

	ਜੇ (tdb->read_only || tdb->traverse_read) {
		ਵਾਪਸ tdb_traverse_read(tdb, fn, ਨਿੱਜੀ_ਅੰਕੜੇ);
	}

	ਜੇ (tdb_transaction_ਤਾਲਾ(tdb, F_WRLCK)) {
		ਵਾਪਸ -1;
	}

	ret = tdb_traverse_ਅੰਦਰੂਨੀ(tdb, fn, ਨਿੱਜੀ_ਅੰਕੜੇ, &tl);

	tdb_transaction_unlock(tdb);

	ਵਾਪਸ ret;
}


/* find the first entry in the database and return its key */
TDB_ਅੰਕੜੇ tdb_firstkey(ਬਣਤਰ tdb_context *tdb)
{
	TDB_ਅੰਕੜੇ ਕੁੰਜੀ;
	ਬਣਤਰ list_struct rec;

	/* release any old lock */
	ਜੇ (tdb_unlock_ਰਿਕਾਰਡ(tdb, tdb->travlocks.off) != 0)
		ਵਾਪਸ tdb_null;
	tdb->travlocks.off = tdb->travlocks.hash = 0;
	tdb->travlocks.ਤਾਲਾ_rw = F_RDLCK;

	/* Grab first record: locks chain and returned record. */
	ਜੇ (tdb_ਅਗਲਾ_ਤਾਲਾ(tdb, &tdb->travlocks, &rec) <= 0)
		ਵਾਪਸ tdb_null;
	/* now read the key */
	ਕੁੰਜੀ.dsize = rec.ਕੁੰਜੀ_len;
	ਕੁੰਜੀ.dptr =tdb_alloc_read(tdb,tdb->travlocks.off+ਆਕਾਰ(rec),ਕੁੰਜੀ.dsize);

	/* Unlock the hash chain of the record we just read. */
	ਜੇ (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.ਤਾਲਾ_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_firstkey: error occurred while tdb_unlocking!\n"));
	ਵਾਪਸ ਕੁੰਜੀ;
}

/* find the next entry in the database, returning its key */
TDB_ਅੰਕੜੇ tdb_nextkey(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ oldkey)
{
	u32 oldhash;
	TDB_ਅੰਕੜੇ ਕੁੰਜੀ = tdb_null;
	ਬਣਤਰ list_struct rec;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *k = ਨਿਰਾ;

	/* Is locked key the old key?  If so, traverse will be reliable. */
	ਜੇ (tdb->travlocks.off) {
		ਜੇ (tdb_ਤਾਲਾ(tdb,tdb->travlocks.hash,tdb->travlocks.ਤਾਲਾ_rw))
			ਵਾਪਸ tdb_null;
		ਜੇ (tdb_rec_read(tdb, tdb->travlocks.off, &rec) == -1
		    || !(k = tdb_alloc_read(tdb,tdb->travlocks.off+ਆਕਾਰ(rec),
					    rec.ਕੁੰਜੀ_len))
		    || memcmp(k, oldkey.dptr, oldkey.dsize) != 0) {
			/* No, it wasn't: unlock it and start from scratch */
			ਜੇ (tdb_unlock_ਰਿਕਾਰਡ(tdb, tdb->travlocks.off) != 0) {
				ਸੁਰੱਖਿਅਤ_FREE(k);
				ਵਾਪਸ tdb_null;
			}
			ਜੇ (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.ਤਾਲਾ_rw) != 0) {
				ਸੁਰੱਖਿਅਤ_FREE(k);
				ਵਾਪਸ tdb_null;
			}
			tdb->travlocks.off = 0;
		}

		ਸੁਰੱਖਿਅਤ_FREE(k);
	}

	ਜੇ (!tdb->travlocks.off) {
		/* No previous element: do normal find, and lock record */
		tdb->travlocks.off = tdb_ਲੱਭੋ_ਤਾਲਾ_hash(tdb, oldkey, tdb->hash_fn(&oldkey), tdb->travlocks.ਤਾਲਾ_rw, &rec);
		ਜੇ (!tdb->travlocks.off)
			ਵਾਪਸ tdb_null;
		tdb->travlocks.hash = ਬਾਲਟੀ(rec.ਭਰਿਆ_hash);
		ਜੇ (tdb_ਤਾਲਾ_ਰਿਕਾਰਡ(tdb, tdb->travlocks.off) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: lock_record failed (%s)!\n", strerror(errno)));
			ਵਾਪਸ tdb_null;
		}
	}
	oldhash = tdb->travlocks.hash;

	/* Grab next record: locks chain and returned record,
	   unlocks old record */
	ਜੇ (tdb_ਅਗਲਾ_ਤਾਲਾ(tdb, &tdb->travlocks, &rec) > 0) {
		ਕੁੰਜੀ.dsize = rec.ਕੁੰਜੀ_len;
		ਕੁੰਜੀ.dptr = tdb_alloc_read(tdb, tdb->travlocks.off+ਆਕਾਰ(rec),
					  ਕੁੰਜੀ.dsize);
		/* Unlock the chain of this new record */
		ਜੇ (tdb_unlock(tdb, tdb->travlocks.hash, tdb->travlocks.ਤਾਲਾ_rw) != 0)
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	}
	/* Unlock the chain of old record */
	ਜੇ (tdb_unlock(tdb, ਬਾਲਟੀ(oldhash), tdb->travlocks.ਤਾਲਾ_rw) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_nextkey: WARNING tdb_unlock failed!\n"));
	ਵਾਪਸ ਕੁੰਜੀ;
}

/* file: dump.c */

ਸਥਿਰ tdb_off_t tdb_dump_ਰਿਕਾਰਡ(ਬਣਤਰ tdb_context *tdb, ਪੂਰਨ hash,
				 tdb_off_t ਵਿੱਥ)
{
	ਬਣਤਰ list_struct rec;
	tdb_off_t tailer_ofs, tailer;

	ਜੇ (tdb->methods->tdb_read(tdb, ਵਿੱਥ, (ਅੱਖਰ *)&rec,
				   ਆਕਾਰ(rec), DOCONV()) == -1) {
		ਲਿਖੋ("ERROR: failed to read record at %u\n", ਵਿੱਥ);
		ਵਾਪਸ 0;
	}

	ਲਿਖੋ(" rec: hash=%d offset=0x%08x next=0x%08x rec_len=%d "
	       "key_len=%d data_len=%d full_hash=0x%x magic=0x%x\n",
	       hash, ਵਿੱਥ, rec.ਅਗਲਾ, rec.rec_len, rec.ਕੁੰਜੀ_len, rec.ਅੰਕੜੇ_len,
	       rec.ਭਰਿਆ_hash, rec.magic);

	tailer_ofs = ਵਿੱਥ + ਆਕਾਰ(rec) + rec.rec_len - ਆਕਾਰ(tdb_off_t);

	ਜੇ (tdb_ofs_read(tdb, tailer_ofs, &tailer) == -1) {
		ਲਿਖੋ("ERROR: failed to read tailer at %u\n", tailer_ofs);
		ਵਾਪਸ rec.ਅਗਲਾ;
	}

	ਜੇ (tailer != rec.rec_len + ਆਕਾਰ(rec)) {
		ਲਿਖੋ("ERROR: tailer does not match record! tailer=%u totalsize=%u\n",
				(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)tailer, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)(rec.rec_len + ਆਕਾਰ(rec)));
	}
	ਵਾਪਸ rec.ਅਗਲਾ;
}

ਸਥਿਰ ਪੂਰਨ tdb_dump_chain(ਬਣਤਰ tdb_context *tdb, ਪੂਰਨ i)
{
	tdb_off_t rec_ptr, ਸਿਖਰ;

	ਸਿਖਰ = TDB_HASH_ਸਿਖਰ(i);

	ਜੇ (tdb_ਤਾਲਾ(tdb, i, F_WRLCK) != 0)
		ਵਾਪਸ -1;

	ਜੇ (tdb_ofs_read(tdb, ਸਿਖਰ, &rec_ptr) == -1)
		ਵਾਪਸ tdb_unlock(tdb, i, F_WRLCK);

	ਜੇ (rec_ptr)
		ਲਿਖੋ("hash=%d\n", i);

	ਜਦੋਂ (rec_ptr) {
		rec_ptr = tdb_dump_ਰਿਕਾਰਡ(tdb, i, rec_ptr);
	}

	ਵਾਪਸ tdb_unlock(tdb, i, F_WRLCK);
}

ਖਾਲੀ tdb_dump_all(ਬਣਤਰ tdb_context *tdb)
{
	ਪੂਰਨ i;
	ਲਈ (i = 0; i < (ਪੂਰਨ)tdb->ਸਿਰਲੇਖ.hash_ਮਾਪ; i++) {
		tdb_dump_chain(tdb, i);
	}
	ਲਿਖੋ("freelist:\n");
	tdb_dump_chain(tdb, -1);
}

ਪੂਰਨ tdb_printfreelist(ਬਣਤਰ tdb_context *tdb)
{
	ਪੂਰਨ ret;
	ਲੰਮਾ ਕੁੱਲ_free = 0;
	tdb_off_t ਵਿੱਥ, rec_ptr;
	ਬਣਤਰ list_struct rec;

	ਜੇ ((ret = tdb_ਤਾਲਾ(tdb, -1, F_WRLCK)) != 0)
		ਵਾਪਸ ret;

	ਵਿੱਥ = FREELIST_ਸਿਖਰ;

	/* read in the freelist top */
	ਜੇ (tdb_ofs_read(tdb, ਵਿੱਥ, &rec_ptr) == -1) {
		tdb_unlock(tdb, -1, F_WRLCK);
		ਵਾਪਸ 0;
	}

	ਲਿਖੋ("freelist top=[0x%08x]\n", rec_ptr );
	ਜਦੋਂ (rec_ptr) {
		ਜੇ (tdb->methods->tdb_read(tdb, rec_ptr, (ਅੱਖਰ *)&rec,
					   ਆਕਾਰ(rec), DOCONV()) == -1) {
			tdb_unlock(tdb, -1, F_WRLCK);
			ਵਾਪਸ -1;
		}

		ਜੇ (rec.magic != TDB_FREE_MAGIC) {
			ਲਿਖੋ("bad magic 0x%08x in free list\n", rec.magic);
			tdb_unlock(tdb, -1, F_WRLCK);
			ਵਾਪਸ -1;
		}

		ਲਿਖੋ("entry offset=[0x%08x], rec.rec_len = [0x%08x (%d)] (end = 0x%08x)\n",
		       rec_ptr, rec.rec_len, rec.rec_len, rec_ptr + rec.rec_len);
		ਕੁੱਲ_free += rec.rec_len;

		/* move to the next record */
		rec_ptr = rec.ਅਗਲਾ;
	}
	ਲਿਖੋ("total rec_len = [0x%08x (%d)]\n", (ਪੂਰਨ)ਕੁੱਲ_free,
               (ਪੂਰਨ)ਕੁੱਲ_free);

	ਵਾਪਸ tdb_unlock(tdb, -1, F_WRLCK);
}

/* file: tdb.c */

/*
  non-blocking increment of the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
ਖਾਲੀ tdb_increment_seqnum_nonblock(ਬਣਤਰ tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	ਜੇ (!(tdb->flags & TDB_SEQNUM)) {
		ਵਾਪਸ;
	}

	/* we ignore errors from this, as we have no sane way of
	   dealing with them.
	*/
	ਜੇ (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		ਵਾਪਸ;
	seqnum++;
	(ਖਾਲੀ) tdb_ofs_write(tdb, TDB_SEQNUM_OFS, &seqnum);
}

/*
  increment the tdb sequence number if the tdb has been opened using
  the TDB_SEQNUM flag
*/
ਸਥਿਰ ਖਾਲੀ tdb_increment_seqnum(ਬਣਤਰ tdb_context *tdb)
{
	ਜੇ (!(tdb->flags & TDB_SEQNUM)) {
		ਵਾਪਸ;
	}

	ਜੇ (tdb_brlock(tdb, TDB_SEQNUM_OFS, F_WRLCK, F_SETLKW, 1, 1) != 0) {
		ਵਾਪਸ;
	}

	tdb_increment_seqnum_nonblock(tdb);

	tdb_brlock(tdb, TDB_SEQNUM_OFS, F_UNLCK, F_SETLKW, 1, 1);
}

ਸਥਿਰ ਪੂਰਨ tdb_ਕੁੰਜੀ_compare(TDB_ਅੰਕੜੇ ਕੁੰਜੀ, TDB_ਅੰਕੜੇ ਅੰਕੜੇ,
			   ਖਾਲੀ *ਨਿੱਜੀ_ਅੰਕੜੇ EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
	ਵਾਪਸ memcmp(ਅੰਕੜੇ.dptr, ਕੁੰਜੀ.dptr, ਅੰਕੜੇ.dsize);
}

/* Returns 0 on fail.  On success, return offset of record, and fills
   in rec */
ਸਥਿਰ tdb_off_t tdb_ਲੱਭੋ(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ, u32 hash,
			ਬਣਤਰ list_struct *r)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	ਜੇ (tdb_ofs_read(tdb, TDB_HASH_ਸਿਖਰ(hash), &rec_ptr) == -1)
		ਵਾਪਸ 0;

	/* keep looking until we find the right record */
	ਜਦੋਂ (rec_ptr) {
		ਜੇ (tdb_rec_read(tdb, rec_ptr, r) == -1)
			ਵਾਪਸ 0;

		ਜੇ (!TDB_DEAD(r) && hash==r->ਭਰਿਆ_hash
		    && ਕੁੰਜੀ.dsize==r->ਕੁੰਜੀ_len
		    && tdb_parse_ਅੰਕੜੇ(tdb, ਕੁੰਜੀ, rec_ptr + ਆਕਾਰ(*r),
				      r->ਕੁੰਜੀ_len, tdb_ਕੁੰਜੀ_compare,
				      ਨਿਰਾ) == 0) {
			ਵਾਪਸ rec_ptr;
		}
		rec_ptr = r->ਅਗਲਾ;
	}
	ਵਾਪਸ TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
}

/* As tdb_find, but if you succeed, keep the lock */
tdb_off_t tdb_ਲੱਭੋ_ਤਾਲਾ_hash(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ, u32 hash, ਪੂਰਨ locktype,
			   ਬਣਤਰ list_struct *rec)
{
	u32 rec_ptr;

	ਜੇ (tdb_ਤਾਲਾ(tdb, ਬਾਲਟੀ(hash), locktype) == -1)
		ਵਾਪਸ 0;
	ਜੇ (!(rec_ptr = tdb_ਲੱਭੋ(tdb, ਕੁੰਜੀ, hash, rec)))
		tdb_unlock(tdb, ਬਾਲਟੀ(hash), locktype);
	ਵਾਪਸ rec_ptr;
}


/* update an entry in place - this only works if the new data size
   is <= the old data size and the key exists.
   on failure return -1.
*/
ਸਥਿਰ ਪੂਰਨ tdb_update_hash(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ, u32 hash, TDB_ਅੰਕੜੇ dbuf)
{
	ਬਣਤਰ list_struct rec;
	tdb_off_t rec_ptr;

	/* find entry */
	ਜੇ (!(rec_ptr = tdb_ਲੱਭੋ(tdb, ਕੁੰਜੀ, hash, &rec)))
		ਵਾਪਸ -1;

	/* must be long enough key, data and tailer */
	ਜੇ (rec.rec_len < ਕੁੰਜੀ.dsize + dbuf.dsize + ਆਕਾਰ(tdb_off_t)) {
		tdb->ecode = TDB_SUCCESS; /* Not really an error */
		ਵਾਪਸ -1;
	}

	ਜੇ (tdb->methods->tdb_write(tdb, rec_ptr + ਆਕਾਰ(rec) + rec.ਕੁੰਜੀ_len,
		      dbuf.dptr, dbuf.dsize) == -1)
		ਵਾਪਸ -1;

	ਜੇ (dbuf.dsize != rec.ਅੰਕੜੇ_len) {
		/* update size */
		rec.ਅੰਕੜੇ_len = dbuf.dsize;
		ਵਾਪਸ tdb_rec_write(tdb, rec_ptr, &rec);
	}

	ਵਾਪਸ 0;
}

/* find an entry in the database given a key */
/* If an entry doesn't exist tdb_err will be set to
 * TDB_ERR_NOEXIST. If a key has no data attached
 * then the TDB_DATA will have zero length but
 * a non-zero pointer
 */
TDB_ਅੰਕੜੇ tdb_fetch(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ)
{
	tdb_off_t rec_ptr;
	ਬਣਤਰ list_struct rec;
	TDB_ਅੰਕੜੇ ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&ਕੁੰਜੀ);
	ਜੇ (!(rec_ptr = tdb_ਲੱਭੋ_ਤਾਲਾ_hash(tdb,ਕੁੰਜੀ,hash,F_RDLCK,&rec)))
		ਵਾਪਸ tdb_null;

	ret.dptr = tdb_alloc_read(tdb, rec_ptr + ਆਕਾਰ(rec) + rec.ਕੁੰਜੀ_len,
				  rec.ਅੰਕੜੇ_len);
	ret.dsize = rec.ਅੰਕੜੇ_len;
	tdb_unlock(tdb, ਬਾਲਟੀ(rec.ਭਰਿਆ_hash), F_RDLCK);
	ਵਾਪਸ ret;
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

ਪੂਰਨ tdb_parse_ਰਿਕਾਰਡ(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ,
		     ਪੂਰਨ (*parser)(TDB_ਅੰਕੜੇ ਕੁੰਜੀ, TDB_ਅੰਕੜੇ ਅੰਕੜੇ,
				   ਖਾਲੀ *ਨਿੱਜੀ_ਅੰਕੜੇ),
		     ਖਾਲੀ *ਨਿੱਜੀ_ਅੰਕੜੇ)
{
	tdb_off_t rec_ptr;
	ਬਣਤਰ list_struct rec;
	ਪੂਰਨ ret;
	u32 hash;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&ਕੁੰਜੀ);

	ਜੇ (!(rec_ptr = tdb_ਲੱਭੋ_ਤਾਲਾ_hash(tdb,ਕੁੰਜੀ,hash,F_RDLCK,&rec))) {
		ਵਾਪਸ TDB_ERRCODE(TDB_ERR_NOEXIST, 0);
	}

	ret = tdb_parse_ਅੰਕੜੇ(tdb, ਕੁੰਜੀ, rec_ptr + ਆਕਾਰ(rec) + rec.ਕੁੰਜੀ_len,
			     rec.ਅੰਕੜੇ_len, parser, ਨਿੱਜੀ_ਅੰਕੜੇ);

	tdb_unlock(tdb, ਬਾਲਟੀ(rec.ਭਰਿਆ_hash), F_RDLCK);

	ਵਾਪਸ ret;
}

/* check if an entry in the database exists

   note that 1 is returned if the key is found and 0 is returned if not found
   this doesn't match the conventions in the rest of this module, but is
   compatible with gdbm
*/
ਸਥਿਰ ਪੂਰਨ tdb_exists_hash(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ, u32 hash)
{
	ਬਣਤਰ list_struct rec;

	ਜੇ (tdb_ਲੱਭੋ_ਤਾਲਾ_hash(tdb, ਕੁੰਜੀ, hash, F_RDLCK, &rec) == 0)
		ਵਾਪਸ 0;
	tdb_unlock(tdb, ਬਾਲਟੀ(rec.ਭਰਿਆ_hash), F_RDLCK);
	ਵਾਪਸ 1;
}

ਪੂਰਨ tdb_exists(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ)
{
	u32 hash = tdb->hash_fn(&ਕੁੰਜੀ);
	ਵਾਪਸ tdb_exists_hash(tdb, ਕੁੰਜੀ, hash);
}

/* actually delete an entry in the database given the offset */
ਪੂਰਨ tdb_do_delete(ਬਣਤਰ tdb_context *tdb, tdb_off_t rec_ptr, ਬਣਤਰ list_struct*rec)
{
	tdb_off_t ਆਖਰੀ_ptr, i;
	ਬਣਤਰ list_struct lastrec;

	ਜੇ (tdb->read_only || tdb->traverse_read) ਵਾਪਸ -1;

	ਜੇ (tdb_write_ਤਾਲਾ_ਰਿਕਾਰਡ(tdb, rec_ptr) == -1) {
		/* Someone traversing here: mark it as dead */
		rec->magic = TDB_DEAD_MAGIC;
		ਵਾਪਸ tdb_rec_write(tdb, rec_ptr, rec);
	}
	ਜੇ (tdb_write_unlock_ਰਿਕਾਰਡ(tdb, rec_ptr) != 0)
		ਵਾਪਸ -1;

	/* find previous record in hash chain */
	ਜੇ (tdb_ofs_read(tdb, TDB_HASH_ਸਿਖਰ(rec->ਭਰਿਆ_hash), &i) == -1)
		ਵਾਪਸ -1;
	ਲਈ (ਆਖਰੀ_ptr = 0; i != rec_ptr; ਆਖਰੀ_ptr = i, i = lastrec.ਅਗਲਾ)
		ਜੇ (tdb_rec_read(tdb, i, &lastrec) == -1)
			ਵਾਪਸ -1;

	/* unlink it: next ptr is at start of record. */
	ਜੇ (ਆਖਰੀ_ptr == 0)
		ਆਖਰੀ_ptr = TDB_HASH_ਸਿਖਰ(rec->ਭਰਿਆ_hash);
	ਜੇ (tdb_ofs_write(tdb, ਆਖਰੀ_ptr, &rec->ਅਗਲਾ) == -1)
		ਵਾਪਸ -1;

	/* recover the space */
	ਜੇ (tdb_free(tdb, rec_ptr, rec) == -1)
		ਵਾਪਸ -1;
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ tdb_ਗਿਣਤੀ_dead(ਬਣਤਰ tdb_context *tdb, u32 hash)
{
	ਪੂਰਨ res = 0;
	tdb_off_t rec_ptr;
	ਬਣਤਰ list_struct rec;

	/* read in the hash top */
	ਜੇ (tdb_ofs_read(tdb, TDB_HASH_ਸਿਖਰ(hash), &rec_ptr) == -1)
		ਵਾਪਸ 0;

	ਜਦੋਂ (rec_ptr) {
		ਜੇ (tdb_rec_read(tdb, rec_ptr, &rec) == -1)
			ਵਾਪਸ 0;

		ਜੇ (rec.magic == TDB_DEAD_MAGIC) {
			res += 1;
		}
		rec_ptr = rec.ਅਗਲਾ;
	}
	ਵਾਪਸ res;
}

/*
 * Purge all DEAD records from a hash chain
 */
ਸਥਿਰ ਪੂਰਨ tdb_purge_dead(ਬਣਤਰ tdb_context *tdb, u32 hash)
{
	ਪੂਰਨ res = -1;
	ਬਣਤਰ list_struct rec;
	tdb_off_t rec_ptr;

	ਜੇ (tdb_ਤਾਲਾ(tdb, -1, F_WRLCK) == -1) {
		ਵਾਪਸ -1;
	}

	/* read in the hash top */
	ਜੇ (tdb_ofs_read(tdb, TDB_HASH_ਸਿਖਰ(hash), &rec_ptr) == -1)
		ਜਾਓ fail;

	ਜਦੋਂ (rec_ptr) {
		tdb_off_t ਅਗਲਾ;

		ਜੇ (tdb_rec_read(tdb, rec_ptr, &rec) == -1) {
			ਜਾਓ fail;
		}

		ਅਗਲਾ = rec.ਅਗਲਾ;

		ਜੇ (rec.magic == TDB_DEAD_MAGIC
		    && tdb_do_delete(tdb, rec_ptr, &rec) == -1) {
			ਜਾਓ fail;
		}
		rec_ptr = ਅਗਲਾ;
	}
	res = 0;
 fail:
	tdb_unlock(tdb, -1, F_WRLCK);
	ਵਾਪਸ res;
}

/* delete an entry in the database given a key */
ਸਥਿਰ ਪੂਰਨ tdb_delete_hash(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ, u32 hash)
{
	tdb_off_t rec_ptr;
	ਬਣਤਰ list_struct rec;
	ਪੂਰਨ ret;

	ਜੇ (tdb->max_dead_ਰਿਕਾਰਡਾਂ != 0) {

		/*
		 * Allow for some dead records per hash chain, mainly for
		 * tdb's with a very high create/delete rate like locking.tdb.
		 */

		ਜੇ (tdb_ਤਾਲਾ(tdb, ਬਾਲਟੀ(hash), F_WRLCK) == -1)
			ਵਾਪਸ -1;

		ਜੇ (tdb_ਗਿਣਤੀ_dead(tdb, hash) >= tdb->max_dead_ਰਿਕਾਰਡਾਂ) {
			/*
			 * Don't let the per-chain freelist grow too large,
			 * delete all existing dead records
			 */
			tdb_purge_dead(tdb, hash);
		}

		ਜੇ (!(rec_ptr = tdb_ਲੱਭੋ(tdb, ਕੁੰਜੀ, hash, &rec))) {
			tdb_unlock(tdb, ਬਾਲਟੀ(hash), F_WRLCK);
			ਵਾਪਸ -1;
		}

		/*
		 * Just mark the record as dead.
		 */
		rec.magic = TDB_DEAD_MAGIC;
		ret = tdb_rec_write(tdb, rec_ptr, &rec);
	}
	ਨਹੀਂਤਾਂ {
		ਜੇ (!(rec_ptr = tdb_ਲੱਭੋ_ਤਾਲਾ_hash(tdb, ਕੁੰਜੀ, hash, F_WRLCK,
						   &rec)))
			ਵਾਪਸ -1;

		ret = tdb_do_delete(tdb, rec_ptr, &rec);
	}

	ਜੇ (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	ਜੇ (tdb_unlock(tdb, ਬਾਲਟੀ(rec.ਭਰਿਆ_hash), F_WRLCK) != 0)
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_delete: WARNING tdb_unlock failed!\n"));
	ਵਾਪਸ ret;
}

ਪੂਰਨ tdb_delete(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ)
{
	u32 hash = tdb->hash_fn(&ਕੁੰਜੀ);
	ਵਾਪਸ tdb_delete_hash(tdb, ਕੁੰਜੀ, hash);
}

/*
 * See if we have a dead record around with enough space
 */
ਸਥਿਰ tdb_off_t tdb_ਲੱਭੋ_dead(ਬਣਤਰ tdb_context *tdb, u32 hash,
			       ਬਣਤਰ list_struct *r, tdb_len_t ਲੰਮਾਈ)
{
	tdb_off_t rec_ptr;

	/* read in the hash top */
	ਜੇ (tdb_ofs_read(tdb, TDB_HASH_ਸਿਖਰ(hash), &rec_ptr) == -1)
		ਵਾਪਸ 0;

	/* keep looking until we find the right record */
	ਜਦੋਂ (rec_ptr) {
		ਜੇ (tdb_rec_read(tdb, rec_ptr, r) == -1)
			ਵਾਪਸ 0;

		ਜੇ (TDB_DEAD(r) && r->rec_len >= ਲੰਮਾਈ) {
			/*
			 * First fit for simple coding, TODO: change to best
			 * fit
			 */
			ਵਾਪਸ rec_ptr;
		}
		rec_ptr = r->ਅਗਲਾ;
	}
	ਵਾਪਸ 0;
}

/* store an element in the database, replacing any existing element
   with the same key

   return 0 on success, -1 on failure
*/
ਪੂਰਨ tdb_store(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ, TDB_ਅੰਕੜੇ dbuf, ਪੂਰਨ flag)
{
	ਬਣਤਰ list_struct rec;
	u32 hash;
	tdb_off_t rec_ptr;
	ਅੱਖਰ *p = ਨਿਰਾ;
	ਪੂਰਨ ret = -1;

	ਜੇ (tdb->read_only || tdb->traverse_read) {
		tdb->ecode = TDB_ERR_RDONLY;
		ਵਾਪਸ -1;
	}

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&ਕੁੰਜੀ);
	ਜੇ (tdb_ਤਾਲਾ(tdb, ਬਾਲਟੀ(hash), F_WRLCK) == -1)
		ਵਾਪਸ -1;

	/* check for it existing, on insert. */
	ਜੇ (flag == TDB_ਪਾਓ) {
		ਜੇ (tdb_exists_hash(tdb, ਕੁੰਜੀ, hash)) {
			tdb->ecode = TDB_ERR_EXISTS;
			ਜਾਓ fail;
		}
	} ਨਹੀਂਤਾਂ {
		/* first try in-place update, on modify or replace. */
		ਜੇ (tdb_update_hash(tdb, ਕੁੰਜੀ, hash, dbuf) == 0) {
			ਜਾਓ ਪੂਰਾ;
		}
		ਜੇ (tdb->ecode == TDB_ERR_NOEXIST &&
		    flag == TDB_MODIFY) {
			/* if the record doesn't exist and we are in TDB_MODIFY mode then
			 we should fail the store */
			ਜਾਓ fail;
		}
	}
	/* reset the error code potentially set by the tdb_update() */
	tdb->ecode = TDB_SUCCESS;

	/* delete any existing record - if it doesn't exist we don't
           care.  Doing this first reduces fragmentation, and avoids
           coalescing with `allocated' block before it's updated. */
	ਜੇ (flag != TDB_ਪਾਓ)
		tdb_delete_hash(tdb, ਕੁੰਜੀ, hash);

	/* Copy key+value *before* allocating free space in case malloc
	   fails and we are left with a dead spot in the tdb. */

	ਜੇ (!(p = (ਅੱਖਰ *)ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਕੁੰਜੀ.dsize + dbuf.dsize))) {
		tdb->ecode = TDB_ERR_OOM;
		ਜਾਓ fail;
	}

	ਸਮ੍ਰਿਤੀ_ਨਕਲ(p, ਕੁੰਜੀ.dptr, ਕੁੰਜੀ.dsize);
	ਜੇ (dbuf.dsize)
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(p+ਕੁੰਜੀ.dsize, dbuf.dptr, dbuf.dsize);

	ਜੇ (tdb->max_dead_ਰਿਕਾਰਡਾਂ != 0) {
		/*
		 * Allow for some dead records per hash chain, look if we can
		 * find one that can hold the new record. We need enough space
		 * for key, data and tailer. If we find one, we don't have to
		 * consult the central freelist.
		 */
		rec_ptr = tdb_ਲੱਭੋ_dead(
			tdb, hash, &rec,
			ਕੁੰਜੀ.dsize + dbuf.dsize + ਆਕਾਰ(tdb_off_t));

		ਜੇ (rec_ptr != 0) {
			rec.ਕੁੰਜੀ_len = ਕੁੰਜੀ.dsize;
			rec.ਅੰਕੜੇ_len = dbuf.dsize;
			rec.ਭਰਿਆ_hash = hash;
			rec.magic = TDB_MAGIC;
			ਜੇ (tdb_rec_write(tdb, rec_ptr, &rec) == -1
			    || tdb->methods->tdb_write(
				    tdb, rec_ptr + ਆਕਾਰ(rec),
				    p, ਕੁੰਜੀ.dsize + dbuf.dsize) == -1) {
				ਜਾਓ fail;
			}
			ਜਾਓ ਪੂਰਾ;
		}
	}

	/*
	 * We have to allocate some space from the freelist, so this means we
	 * have to lock it. Use the chance to purge all the DEAD records from
	 * the hash chain under the freelist lock.
	 */

	ਜੇ (tdb_ਤਾਲਾ(tdb, -1, F_WRLCK) == -1) {
		ਜਾਓ fail;
	}

	ਜੇ ((tdb->max_dead_ਰਿਕਾਰਡਾਂ != 0)
	    && (tdb_purge_dead(tdb, hash) == -1)) {
		tdb_unlock(tdb, -1, F_WRLCK);
		ਜਾਓ fail;
	}

	/* we have to allocate some space */
	rec_ptr = tdb_allocate(tdb, ਕੁੰਜੀ.dsize + dbuf.dsize, &rec);

	tdb_unlock(tdb, -1, F_WRLCK);

	ਜੇ (rec_ptr == 0) {
		ਜਾਓ fail;
	}

	/* Read hash top into next ptr */
	ਜੇ (tdb_ofs_read(tdb, TDB_HASH_ਸਿਖਰ(hash), &rec.ਅਗਲਾ) == -1)
		ਜਾਓ fail;

	rec.ਕੁੰਜੀ_len = ਕੁੰਜੀ.dsize;
	rec.ਅੰਕੜੇ_len = dbuf.dsize;
	rec.ਭਰਿਆ_hash = hash;
	rec.magic = TDB_MAGIC;

	/* write out and point the top of the hash chain at it */
	ਜੇ (tdb_rec_write(tdb, rec_ptr, &rec) == -1
	    || tdb->methods->tdb_write(tdb, rec_ptr+ਆਕਾਰ(rec), p, ਕੁੰਜੀ.dsize+dbuf.dsize)==-1
	    || tdb_ofs_write(tdb, TDB_HASH_ਸਿਖਰ(hash), &rec_ptr) == -1) {
		/* Need to tdb_unallocate() here */
		ਜਾਓ fail;
	}

 ਪੂਰਾ:
	ret = 0;
 fail:
	ਜੇ (ret == 0) {
		tdb_increment_seqnum(tdb);
	}

	ਸੁਰੱਖਿਅਤ_FREE(p);
	tdb_unlock(tdb, ਬਾਲਟੀ(hash), F_WRLCK);
	ਵਾਪਸ ret;
}


/* Append to an entry. Create if not exist. */
ਪੂਰਨ tdb_append(ਬਣਤਰ tdb_context *tdb, TDB_ਅੰਕੜੇ ਕੁੰਜੀ, TDB_ਅੰਕੜੇ new_dbuf)
{
	u32 hash;
	TDB_ਅੰਕੜੇ dbuf;
	ਪੂਰਨ ret = -1;

	/* find which hash bucket it is in */
	hash = tdb->hash_fn(&ਕੁੰਜੀ);
	ਜੇ (tdb_ਤਾਲਾ(tdb, ਬਾਲਟੀ(hash), F_WRLCK) == -1)
		ਵਾਪਸ -1;

	dbuf = tdb_fetch(tdb, ਕੁੰਜੀ);

	ਜੇ (dbuf.dptr == ਨਿਰਾ) {
		dbuf.dptr = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)ਸਮ੍ਰਿਤੀ_ਦਿਓ(new_dbuf.dsize);
	} ਨਹੀਂਤਾਂ {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *new_dptr = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)realloc(dbuf.dptr,
						     dbuf.dsize + new_dbuf.dsize);
		ਜੇ (new_dptr == ਨਿਰਾ) {
			ਮੁਕਤ(dbuf.dptr);
		}
		dbuf.dptr = new_dptr;
	}

	ਜੇ (dbuf.dptr == ਨਿਰਾ) {
		tdb->ecode = TDB_ERR_OOM;
		ਜਾਓ failed;
	}

	ਸਮ੍ਰਿਤੀ_ਨਕਲ(dbuf.dptr + dbuf.dsize, new_dbuf.dptr, new_dbuf.dsize);
	dbuf.dsize += new_dbuf.dsize;

	ret = tdb_store(tdb, ਕੁੰਜੀ, dbuf, 0);

failed:
	tdb_unlock(tdb, ਬਾਲਟੀ(hash), F_WRLCK);
	ਸੁਰੱਖਿਅਤ_FREE(dbuf.dptr);
	ਵਾਪਸ ret;
}


/*
  return the name of the current tdb file
  useful for external logging functions
*/
ਅਚਲ ਅੱਖਰ *tdb_name(ਬਣਤਰ tdb_context *tdb)
{
	ਵਾਪਸ tdb->name;
}

/*
  return the underlying file descriptor being used by tdb, or -1
  useful for external routines that want to check the device/inode
  of the fd
*/
ਪੂਰਨ tdb_fd(ਬਣਤਰ tdb_context *tdb)
{
	ਵਾਪਸ tdb->fd;
}

/*
  return the current logging function
  useful for external tdb routines that wish to log tdb errors
*/
tdb_log_func tdb_log_fn(ਬਣਤਰ tdb_context *tdb)
{
	ਵਾਪਸ tdb->ਲਘੂਗਣਕ.log_fn;
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
ਪੂਰਨ tdb_get_seqnum(ਬਣਤਰ tdb_context *tdb)
{
	tdb_off_t seqnum=0;

	ਜੇ (tdb_ofs_read(tdb, TDB_SEQNUM_OFS, &seqnum) == -1)
		ਵਾਪਸ 0;
	ਵਾਪਸ seqnum;
}

ਪੂਰਨ tdb_hash_ਮਾਪ(ਬਣਤਰ tdb_context *tdb)
{
	ਵਾਪਸ tdb->ਸਿਰਲੇਖ.hash_ਮਾਪ;
}

size_t tdb_map_ਮਾਪ(ਬਣਤਰ tdb_context *tdb)
{
	ਵਾਪਸ tdb->map_ਮਾਪ;
}

ਪੂਰਨ tdb_get_flags(ਬਣਤਰ tdb_context *tdb)
{
	ਵਾਪਸ tdb->flags;
}


/*
  enable sequence number handling on an open tdb
*/
ਖਾਲੀ tdb_enable_seqnum(ਬਣਤਰ tdb_context *tdb)
{
	tdb->flags |= TDB_SEQNUM;
}

/* file: open.c */

/* all contexts, to ensure no double-opens (fcntl locks don't nest!) */
ਸਥਿਰ ਬਣਤਰ tdb_context *tdbs = ਨਿਰਾ;


/* This is from a hash algorithm suggested by Rogier Wolff */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ default_tdb_hash(TDB_ਅੰਕੜੇ *ਕੁੰਜੀ)
{
	u32 ਮੁੱਲ;	/* Used to compute the hash value.  */
	u32   i;	/* Used to cycle through random values. */

	/* Set the initial value from the key size. */
	ਲਈ (ਮੁੱਲ = 0, i=0; i < ਕੁੰਜੀ->dsize; i++)
		ਮੁੱਲ = ਮੁੱਲ * 256 + ਕੁੰਜੀ->dptr[i] + (ਮੁੱਲ >> 24) * 241;

	ਵਾਪਸ ਮੁੱਲ;
}


/* initialise a new database with a specified hash size */
ਸਥਿਰ ਪੂਰਨ tdb_new_database(ਬਣਤਰ tdb_context *tdb, ਪੂਰਨ hash_ਮਾਪ)
{
	ਬਣਤਰ tdb_ਸਿਰਲੇਖ *newdb;
	ਪੂਰਨ ਮਾਪ, ret = -1;

	/* We make it up in memory, then write it out if not internal */
	ਮਾਪ = ਆਕਾਰ(ਬਣਤਰ tdb_ਸਿਰਲੇਖ) + (hash_ਮਾਪ+1)*ਆਕਾਰ(tdb_off_t);
	ਜੇ (!(newdb = (ਬਣਤਰ tdb_ਸਿਰਲੇਖ *)ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਮਾਪ, 1)))
		ਵਾਪਸ TDB_ERRCODE(TDB_ERR_OOM, -1);

	/* Fill in the header */
	newdb->ਸੰਸਕਰਣ = TDB_ਸੰਸਕਰਣ;
	newdb->hash_ਮਾਪ = hash_ਮਾਪ;
	ਜੇ (tdb->flags & TDB_ਅੰਦਰੂਨੀ) {
		tdb->map_ਮਾਪ = ਮਾਪ;
		tdb->map_ptr = (ਅੱਖਰ *)newdb;
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(&tdb->ਸਿਰਲੇਖ, newdb, ਆਕਾਰ(tdb->ਸਿਰਲੇਖ));
		/* Convert the `ondisk' version if asked. */
		CONVERT(*newdb);
		ਵਾਪਸ 0;
	}
	ਜੇ (lseek(tdb->fd, 0, SEEK_SET) == -1)
		ਜਾਓ fail;

	ਜੇ (ftruncate(tdb->fd, 0) == -1)
		ਜਾਓ fail;

	/* This creates an endian-converted header, as if read from disk */
	CONVERT(*newdb);
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(&tdb->ਸਿਰਲੇਖ, newdb, ਆਕਾਰ(tdb->ਸਿਰਲੇਖ));
	/* Don't endian-convert the magic food! */
	ਸਮ੍ਰਿਤੀ_ਨਕਲ(newdb->magic_food, TDB_MAGIC_FOOD, ਲੜੀ_ਲੰਬਾਈ(TDB_MAGIC_FOOD)+1);
	ਜੇ (ਲਿਖ(tdb->fd, newdb, ਮਾਪ) != ਮਾਪ) {
		ret = -1;
	} ਨਹੀਂਤਾਂ {
		ret = 0;
	}

  fail:
	ਸੁਰੱਖਿਅਤ_FREE(newdb);
	ਵਾਪਸ ret;
}



ਸਥਿਰ ਪੂਰਨ tdb_already_open(dev_t device,
			    ino_t ino)
{
	ਬਣਤਰ tdb_context *i;

	ਲਈ (i = tdbs; i; i = i->ਅਗਲਾ) {
		ਜੇ (i->device == device && i->inode == ino) {
			ਵਾਪਸ 1;
		}
	}

	ਵਾਪਸ 0;
}

/* open the database, creating it if necessary

   The open_flags and mode are passed straight to the open call on the
   database file. A flags value of O_WRONLY is invalid. The hash size
   is advisory, use zero for a default value.

   Return is NULL on error, in which case errno is also set.  Don't
   try to call tdb_error or tdb_errname, just do strerror(errno).

   @param name may be NULL for internal databases. */
ਬਣਤਰ tdb_context *tdb_open(ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ hash_ਮਾਪ, ਪੂਰਨ tdb_flags,
		      ਪੂਰਨ open_flags, mode_t ਬਹੁਲਕ)
{
	ਵਾਪਸ tdb_open_ex(name, hash_ਮਾਪ, tdb_flags, open_flags, ਬਹੁਲਕ, ਨਿਰਾ, ਨਿਰਾ);
}

/* a default logging function */
ਸਥਿਰ ਖਾਲੀ null_log_fn(ਬਣਤਰ tdb_context *tdb, ਗਿਣਤੀ tdb_debug_level level, ਅਚਲ ਅੱਖਰ *fmt, ...) PRINTF_ATTRIBUTE(3, 4);
ਸਥਿਰ ਖਾਲੀ null_log_fn(ਬਣਤਰ tdb_context *tdb EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			ਗਿਣਤੀ tdb_debug_level level EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			ਅਚਲ ਅੱਖਰ *fmt EXT2FS_ATTR((ਅਣਵਰਤਿਆ)), ...)
{
}


ਬਣਤਰ tdb_context *tdb_open_ex(ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ hash_ਮਾਪ, ਪੂਰਨ tdb_flags,
				ਪੂਰਨ open_flags, mode_t ਬਹੁਲਕ,
				ਅਚਲ ਬਣਤਰ tdb_logging_context *log_ctx,
				tdb_hash_func hash_fn)
{
	ਬਣਤਰ tdb_context *tdb;
	ਬਣਤਰ stat st;
	ਪੂਰਨ rev = 0, locked = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *vp;
	u32 vertest;

	ਜੇ (!(tdb = (ਬਣਤਰ tdb_context *)ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ *tdb))) {
		/* Can't log this */
		errno = ENOMEM;
		ਜਾਓ fail;
	}
	tdb_io_init(tdb);
	tdb->fd = -1;
	tdb->name = ਨਿਰਾ;
	tdb->map_ptr = ਨਿਰਾ;
	tdb->flags = tdb_flags;
	tdb->open_flags = open_flags;
	ਜੇ (log_ctx) {
		tdb->ਲਘੂਗਣਕ = *log_ctx;
	} ਨਹੀਂਤਾਂ {
		tdb->ਲਘੂਗਣਕ.log_fn = null_log_fn;
		tdb->ਲਘੂਗਣਕ.log_ਨਿੱਜੀ = ਨਿਰਾ;
	}
	tdb->hash_fn = hash_fn ? hash_fn : default_tdb_hash;

	/* cache the page size */
	tdb->ਪੰਨਾ_ਮਾਪ = sysconf(_SC_PAGESIZE);
	ਜੇ (tdb->ਪੰਨਾ_ਮਾਪ <= 0) {
		tdb->ਪੰਨਾ_ਮਾਪ = 0x2000;
	}

	ਜੇ ((open_flags & O_ACCMODE) == O_WRONLY) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: can't open tdb %s write-only\n",
			 name));
		errno = EINVAL;
		ਜਾਓ fail;
	}

	ਜੇ (hash_ਮਾਪ == 0)
		hash_ਮਾਪ = DEFAULT_HASH_ਮਾਪ;
	ਜੇ ((open_flags & O_ACCMODE) == O_RDONLY) {
		tdb->read_only = 1;
		/* read only databases don't do locking or clear if first */
		tdb->flags |= TDB_NOLOCK;
		tdb->flags &= ~TDB_CLEAR_IF_ਪਹਿਲਾ;
	}

	/* internal databases don't mmap or lock, and start off cleared */
	ਜੇ (tdb->flags & TDB_ਅੰਦਰੂਨੀ) {
		tdb->flags |= (TDB_NOLOCK | TDB_NOMMAP);
		tdb->flags &= ~TDB_CLEAR_IF_ਪਹਿਲਾ;
		ਜੇ (tdb_new_database(tdb, hash_ਮਾਪ) != 0) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: tdb_new_database failed!"));
			ਜਾਓ fail;
		}
		ਜਾਓ ਅੰਦਰੂਨੀ;
	}

	ਜੇ ((tdb->fd = open(name, open_flags, ਬਹੁਲਕ)) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_WARNING, "tdb_open_ex: could not open file %s: %s\n",
			 name, strerror(errno)));
		ਜਾਓ fail;	/* errno set by open(2) */
	}

	/* ensure there is only one process initialising at once */
	ਜੇ (tdb->methods->tdb_brlock(tdb, GLOBAL_ਤਾਲਾ, F_WRLCK, F_SETLKW, 0, 1) == -1) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to get global lock on %s: %s\n",
			 name, strerror(errno)));
		ਜਾਓ fail;	/* errno set by tdb_brlock */
	}

	/* we need to zero database if we are the only one with it open */
	ਜੇ ((tdb_flags & TDB_CLEAR_IF_ਪਹਿਲਾ) &&
	    (locked = (tdb->methods->tdb_brlock(tdb, ਸਰਗਰਮ_ਤਾਲਾ, F_WRLCK, F_SETLK, 0, 1) == 0))) {
		open_flags |= O_CREAT;
		ਜੇ (ftruncate(tdb->fd, 0) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_open_ex: "
				 "failed to truncate %s: %s\n",
				 name, strerror(errno)));
			ਜਾਓ fail; /* errno set by ftruncate */
		}
	}

	ਜੇ (ਪਠ(tdb->fd, &tdb->ਸਿਰਲੇਖ, ਆਕਾਰ(tdb->ਸਿਰਲੇਖ)) != ਆਕਾਰ(tdb->ਸਿਰਲੇਖ)
	    || memcmp(tdb->ਸਿਰਲੇਖ.magic_food, TDB_MAGIC_FOOD,
		      ਆਕਾਰ(TDB_MAGIC_FOOD)) != 0
	    || (tdb->ਸਿਰਲੇਖ.ਸੰਸਕਰਣ != TDB_ਸੰਸਕਰਣ
		&& !(rev = (tdb->ਸਿਰਲੇਖ.ਸੰਸਕਰਣ==TDB_BYTEREV(TDB_ਸੰਸਕਰਣ))))) {
		/* its not a valid database - possibly initialise it */
		ਜੇ (!(open_flags & O_CREAT) || tdb_new_database(tdb, hash_ਮਾਪ) == -1) {
			errno = EIO; /* ie bad format or something */
			ਜਾਓ fail;
		}
		rev = (tdb->flags & TDB_CONVERT);
	}
	vp = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)&tdb->ਸਿਰਲੇਖ.ਸੰਸਕਰਣ;
	vertest = (((u32)vp[0]) << 24) | (((u32)vp[1]) << 16) |
		  (((u32)vp[2]) << 8) | (u32)vp[3];
	tdb->flags |= (vertest==TDB_ਸੰਸਕਰਣ) ? TDB_BIGENDIAN : 0;
	ਜੇ (!rev)
		tdb->flags &= ~TDB_CONVERT;
	ਨਹੀਂਤਾਂ {
		tdb->flags |= TDB_CONVERT;
		tdb_convert(&tdb->ਸਿਰਲੇਖ, ਆਕਾਰ(tdb->ਸਿਰਲੇਖ));
	}
	ਜੇ (fstat(tdb->fd, &st) == -1)
		ਜਾਓ fail;

	ਜੇ (tdb->ਸਿਰਲੇਖ.rwlocks != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: spinlocks no longer supported\n"));
		ਜਾਓ fail;
	}

	/* Is it already in the open list?  If so, fail. */
	ਜੇ (tdb_already_open(st.st_dev, st.st_ino)) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
			 "%s (%d,%d) is already open in this process\n",
			 name, (ਪੂਰਨ)st.st_dev, (ਪੂਰਨ)st.st_ino));
		errno = EBUSY;
		ਜਾਓ fail;
	}

	ਜੇ (!(tdb->name = (ਅੱਖਰ *)strdup(name))) {
		errno = ENOMEM;
		ਜਾਓ fail;
	}

	tdb->map_ਮਾਪ = st.st_ਮਾਪ;
	tdb->device = st.st_dev;
	tdb->inode = st.st_ino;
	tdb->max_dead_ਰਿਕਾਰਡਾਂ = 0;
	tdb_mmap(tdb);
	ਜੇ (locked) {
		ਜੇ (tdb->methods->tdb_brlock(tdb, ਸਰਗਰਮ_ਤਾਲਾ, F_UNLCK, F_SETLK, 0, 1) == -1) {
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: "
				 "failed to take ACTIVE_LOCK on %s: %s\n",
				 name, strerror(errno)));
			ਜਾਓ fail;
		}

	}

	/* We always need to do this if the CLEAR_IF_FIRST flag is set, even if
	   we didn't get the initial exclusive lock as we need to let all other
	   users know we're using it. */

	ਜੇ (tdb_flags & TDB_CLEAR_IF_ਪਹਿਲਾ) {
		/* leave this lock in place to indicate it's in use */
		ਜੇ (tdb->methods->tdb_brlock(tdb, ਸਰਗਰਮ_ਤਾਲਾ, F_RDLCK, F_SETLKW, 0, 1) == -1)
			ਜਾਓ fail;
	}

	/* if needed, run recovery */
	ਜੇ (tdb_transaction_recover(tdb) == -1) {
		ਜਾਓ fail;
	}

 ਅੰਦਰੂਨੀ:
	/* Internal (memory-only) databases skip all the code above to
	 * do with disk files, and resume here by releasing their
	 * global lock and hooking into the active list. */
	ਜੇ (tdb->methods->tdb_brlock(tdb, GLOBAL_ਤਾਲਾ, F_UNLCK, F_SETLKW, 0, 1) == -1)
		ਜਾਓ fail;
	tdb->ਅਗਲਾ = tdbs;
	tdbs = tdb;
	ਵਾਪਸ tdb;

 fail:
	{ ਪੂਰਨ save_errno = errno;

	ਜੇ (!tdb)
		ਵਾਪਸ ਨਿਰਾ;

	ਜੇ (tdb->map_ptr) {
		ਜੇ (tdb->flags & TDB_ਅੰਦਰੂਨੀ)
			ਸੁਰੱਖਿਅਤ_FREE(tdb->map_ptr);
		ਨਹੀਂਤਾਂ
			tdb_munmap(tdb);
	}
	ਸੁਰੱਖਿਅਤ_FREE(tdb->name);
	ਜੇ (tdb->fd != -1)
		ਜੇ (close(tdb->fd) != 0)
			TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_open_ex: failed to close tdb->fd on error!\n"));
	ਸੁਰੱਖਿਅਤ_FREE(tdb);
	errno = save_errno;
	ਵਾਪਸ ਨਿਰਾ;
	}
}

/*
 * Set the maximum number of dead records per hash chain
 */

ਖਾਲੀ tdb_ਸੈੱਟ_max_dead(ਬਣਤਰ tdb_context *tdb, ਪੂਰਨ max_dead)
{
	tdb->max_dead_ਰਿਕਾਰਡਾਂ = max_dead;
}

/**
 * Close a database.
 *
 * @returns -1 for error; 0 for success.
 **/
ਪੂਰਨ tdb_close(ਬਣਤਰ tdb_context *tdb)
{
	ਬਣਤਰ tdb_context **i;
	ਪੂਰਨ ret = 0;

	ਜੇ (tdb->transaction) {
		tdb_transaction_cancel(tdb);
	}

	ਜੇ (tdb->map_ptr) {
		ਜੇ (tdb->flags & TDB_ਅੰਦਰੂਨੀ)
			ਸੁਰੱਖਿਅਤ_FREE(tdb->map_ptr);
		ਨਹੀਂਤਾਂ
			tdb_munmap(tdb);
	}
	ਸੁਰੱਖਿਅਤ_FREE(tdb->name);
	ਜੇ (tdb->fd != -1)
		ret = close(tdb->fd);
	ਸੁਰੱਖਿਅਤ_FREE(tdb->lockrecs);

	/* Remove from contexts list */
	ਲਈ (i = &tdbs; *i; i = &(*i)->ਅਗਲਾ) {
		ਜੇ (*i == tdb) {
			*i = tdb->ਅਗਲਾ;
			ਰੋਕੋ;
		}
	}

	memset(tdb, 0, ਆਕਾਰ(*tdb));
	ਸੁਰੱਖਿਅਤ_FREE(tdb);

	ਵਾਪਸ ret;
}

/* register a logging function */
ਖਾਲੀ tdb_ਸੈੱਟ_logging_function(ਬਣਤਰ tdb_context *tdb,
                              ਅਚਲ ਬਣਤਰ tdb_logging_context *log_ctx)
{
        tdb->ਲਘੂਗਣਕ = *log_ctx;
}

ਖਾਲੀ *tdb_get_logging_ਨਿੱਜੀ(ਬਣਤਰ tdb_context *tdb)
{
	ਵਾਪਸ tdb->ਲਘੂਗਣਕ.log_ਨਿੱਜੀ;
}

/* reopen a tdb - this can be used after a fork to ensure that we have an independent
   seek pointer from our parent and to re-establish locks */
ਪੂਰਨ tdb_reopen(ਬਣਤਰ tdb_context *tdb)
{
	ਬਣਤਰ stat st;

	ਜੇ (tdb->flags & TDB_ਅੰਦਰੂਨੀ) {
		ਵਾਪਸ 0; /* Nothing to do. */
	}

	ਜੇ (tdb->ਅੰਕ_locks != 0 || tdb->global_ਤਾਲਾ.ਗਿਣਤੀ_2) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed with locks held\n"));
		ਜਾਓ fail;
	}

	ਜੇ (tdb->transaction != 0) {
		TDB_LOG((tdb, TDB_DEBUG_ERROR, "tdb_reopen: reopen not allowed inside a transaction\n"));
		ਜਾਓ fail;
	}

	ਜੇ (tdb_munmap(tdb) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: munmap failed (%s)\n", strerror(errno)));
		ਜਾਓ fail;
	}
	ਜੇ (close(tdb->fd) != 0)
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: WARNING closing tdb->fd failed!\n"));
	tdb->fd = open(tdb->name, tdb->open_flags & ~(O_CREAT|O_TRUNC), 0);
	ਜੇ (tdb->fd == -1) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: open failed (%s)\n", strerror(errno)));
		ਜਾਓ fail;
	}
	ਜੇ ((tdb->flags & TDB_CLEAR_IF_ਪਹਿਲਾ) &&
	    (tdb->methods->tdb_brlock(tdb, ਸਰਗਰਮ_ਤਾਲਾ, F_RDLCK, F_SETLKW, 0, 1) == -1)) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: failed to obtain active lock\n"));
		ਜਾਓ fail;
	}
	ਜੇ (fstat(tdb->fd, &st) != 0) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: fstat failed (%s)\n", strerror(errno)));
		ਜਾਓ fail;
	}
	ਜੇ (st.st_ino != tdb->inode || st.st_dev != tdb->device) {
		TDB_LOG((tdb, TDB_DEBUG_FATAL, "tdb_reopen: file dev/inode has changed!\n"));
		ਜਾਓ fail;
	}
	tdb_mmap(tdb);

	ਵਾਪਸ 0;

fail:
	tdb_close(tdb);
	ਵਾਪਸ -1;
}

/* reopen all tdb's */
ਪੂਰਨ tdb_reopen_all(ਪੂਰਨ ਮਾਪਾ_longlived)
{
	ਬਣਤਰ tdb_context *tdb;

	ਲਈ (tdb=tdbs; tdb; tdb = tdb->ਅਗਲਾ) {
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
		ਜੇ (ਮਾਪਾ_longlived) {
			/* Ensure no clear-if-first. */
			tdb->flags &= ~TDB_CLEAR_IF_ਪਹਿਲਾ;
		}

		ਜੇ (tdb_reopen(tdb) != 0)
			ਵਾਪਸ -1;
	}

	ਵਾਪਸ 0;
}

/**
 * Flush a database file from the page cache.
 **/
ਪੂਰਨ tdb_flush(ਬਣਤਰ tdb_context *tdb)
{
	ਜੇ (tdb->fd != -1)
		ਵਾਪਸ fsync(tdb->fd);
	ਵਾਪਸ 0;
}
