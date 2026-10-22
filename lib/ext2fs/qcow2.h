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

#ضمن <ext2fs/ext2_types.h>

/* Number of l2 tables in memory before writeback */
#وصف_ڪريو L2_CACHE_PREALLOC	512


#وصف_ڪريو QCOW_MAGIC (('Q' << 24) | ('F' << 16) | ('I' << 8) | 0xfb)
#وصف_ڪريو QCOW_نسخو		2
#وصف_ڪريو QCOW_OFLAG_COPIED	(1ULL << 63)
#وصف_ڪريو QCOW_OFLAG_COMPRESSED	(1ULL << 62)

#وصف_ڪريو QCOW_COMPRESSED		1
#وصف_ڪريو QCOW_ENCRYPTED		2
#وصف_ڪريو QCOW_CORRUPTED		3

بنية ext2_qcow2_hdr {
	__u32	magic;
	__u32	نسخو;

	__u64	backing_file_وٿي;
	__u32	backing_file_سائيز;

	__u32	cluster_بٽ;
	__u64	سائيز;
	__u32	crypt_طريقو;

	__u32	l1_سائيز;
	__u64	l1_جدول_وٿي;

	__u64	refcount_جدول_وٿي;
	__u32	refcount_جدول_clusters;

	__u32	nb_snapshots;
	__u64	snapshots_وٿي;
};

تعريفنوع بنية ext2_qcow2_l2_جدول L2_CACHE_سر;

بنية ext2_qcow2_l2_جدول {
	__u32		l1_اشاريو;
	__u64		وٿي;
	__u64		*معلومات;
	L2_CACHE_سر	*ايندڙ;
};

بنية ext2_qcow2_l2_cache {
	L2_CACHE_سر	*استعمال_ٿيل_سر;
	L2_CACHE_سر	*استعمال_ٿيل_پڇ;
	L2_CACHE_سر	*free_سر;
	__u32		حرر;
	__u32		ڳڻپ;
	__u64		ايندڙ_وٿي;
};

بنية ext2_qcow2_refcount {
	__u64	*refcount_جدول;
	__u64	refcount_جدول_وٿي;
	__u64	refcount_بلاڪ_وٿي;

	__u32	refcount_جدول_clusters;
	__u32	refcount_جدول_اشاريو;
	__u32	refcount_بلاڪ_اشاريو;

	__u16	*refcount_بلاڪ;
};

بنية ext2_qcow2_image {
	صحيح	fd;
	بنية	ext2_qcow2_hdr		*hdr;
	بنية	ext2_qcow2_l2_cache	*l2_cache;
	بنية	ext2_qcow2_refcount	refcount;
	__u32	cluster_سائيز;
	__u32	cluster_بٽ;
	__u32	l1_سائيز;
	__u32	l2_سائيز;

	__u64	*l1_جدول;
	__u64	l2_وٿي;
	__u64	l1_وٿي;
	__u64	image_سائيز;
};

/* Function prototypes */

/* qcow2.c */

/* Functions for converting qcow2 image into raw image */
بنية ext2_qcow2_hdr *qcow2_read_سرو(صحيح);
صحيح qcow2_write_raw_image(صحيح, صحيح, بنية ext2_qcow2_hdr *);

