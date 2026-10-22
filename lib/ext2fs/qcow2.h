/*
 * qcow2.h --- structures and function prototypes for qcow2.c to generate
 * qcow2 formatted disk images.  This format is used originally by QEMU
 * for virtual machines, and stores the filesystem data on disk in a
 * packed format to avoid creating sparse image files that need lots of
 * seeking to read and write.
 *
 * The qcow2 format supports zlib compression, but that is not yet
 * implemented.
 *
 * It is possible to directly mount a qcow2 image using qemu-nbd:
 *
 * [root]# modprobe nbd max_part=63
 * [root]# qemu-nbd -c /dev/nbd0 image.img
 * [root]# mount /dev/nbd0p1 /mnt/qemu
 *
 * Format details at http://people.gnome.org/~markmc/qcow-image-format.html
 *
 * Copyright (C) 2010 Red Hat, Inc., Lukas Czerner <lczerner@redhat.com>
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Public
 * License.
 * %End-Header%
 */

#ਅਨ੍ਤਰ੍ਭਾਵ <ext2fs/ext2_types.h>

/* Number of l2 tables in memory before writeback */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ L2_CACHE_PREALLOC	512


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ QCOW_MAGIC (('Q' << 24) | ('F' << 16) | ('I' << 8) | 0xfb)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ QCOW_ਸੰਸਕਰਣ		2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ QCOW_OFLAG_COPIED	(1ULL << 63)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ QCOW_OFLAG_COMPRESSED	(1ULL << 62)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ QCOW_COMPRESSED		1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ QCOW_ENCRYPTED		2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ QCOW_CORRUPTED		3

ਬਣਤਰ ext2_qcow2_hdr {
	__u32	magic;
	__u32	ਸੰਸਕਰਣ;

	__u64	backing_file_ਵਿੱਥ;
	__u32	backing_file_ਮਾਪ;

	__u32	cluster_ਬਿੱਟ;
	__u64	ਮਾਪ;
	__u32	crypt_ਵਿਧੀ;

	__u32	l1_ਮਾਪ;
	__u64	l1_ਸਾਰਣੀ_ਵਿੱਥ;

	__u64	refcount_ਸਾਰਣੀ_ਵਿੱਥ;
	__u32	refcount_ਸਾਰਣੀ_clusters;

	__u32	nb_snapshots;
	__u64	snapshots_ਵਿੱਥ;
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ext2_qcow2_l2_ਸਾਰਣੀ L2_CACHE_ਸਿਰ;

ਬਣਤਰ ext2_qcow2_l2_ਸਾਰਣੀ {
	__u32		l1_ਸੂਚਕਾਂਕ;
	__u64		ਵਿੱਥ;
	__u64		*ਅੰਕੜੇ;
	L2_CACHE_ਸਿਰ	*ਅਗਲਾ;
};

ਬਣਤਰ ext2_qcow2_l2_cache {
	L2_CACHE_ਸਿਰ	*ਵਰਤਿਆ_ਸਿਰ;
	L2_CACHE_ਸਿਰ	*ਵਰਤਿਆ_ਪੂਛ;
	L2_CACHE_ਸਿਰ	*free_ਸਿਰ;
	__u32		ਮੁਕਤ;
	__u32		ਗਿਣਤੀ_2;
	__u64		ਅਗਲਾ_ਵਿੱਥ;
};

ਬਣਤਰ ext2_qcow2_refcount {
	__u64	*refcount_ਸਾਰਣੀ;
	__u64	refcount_ਸਾਰਣੀ_ਵਿੱਥ;
	__u64	refcount_ਖੰਡ_ਵਿੱਥ;

	__u32	refcount_ਸਾਰਣੀ_clusters;
	__u32	refcount_ਸਾਰਣੀ_ਸੂਚਕਾਂਕ;
	__u32	refcount_ਖੰਡ_ਸੂਚਕਾਂਕ;

	__u16	*refcount_ਖੰਡ;
};

ਬਣਤਰ ext2_qcow2_image {
	ਪੂਰਨ	fd;
	ਬਣਤਰ	ext2_qcow2_hdr		*hdr;
	ਬਣਤਰ	ext2_qcow2_l2_cache	*l2_cache;
	ਬਣਤਰ	ext2_qcow2_refcount	refcount;
	__u32	cluster_ਮਾਪ;
	__u32	cluster_ਬਿੱਟ;
	__u32	l1_ਮਾਪ;
	__u32	l2_ਮਾਪ;

	__u64	*l1_ਸਾਰਣੀ;
	__u64	l2_ਵਿੱਥ;
	__u64	l1_ਵਿੱਥ;
	__u64	image_ਮਾਪ;
};

/* Function prototypes */

/* qcow2.c */

/* Functions for converting qcow2 image into raw image */
ਬਣਤਰ ext2_qcow2_hdr *qcow2_read_ਸਿਰਲੇਖ(ਪੂਰਨ);
ਪੂਰਨ qcow2_write_raw_image(ਪੂਰਨ, ਪੂਰਨ, ਬਣਤਰ ext2_qcow2_hdr *);

