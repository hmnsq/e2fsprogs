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

#شامل <ext2fs/ext2_types.h>

/* Number of l2 tables in memory before writeback */
#تعریف_کرو L2_CACHE_PREALLOC	512


#تعریف_کرو QCOW_MAGIC (('Q' << 24) | ('F' << 16) | ('I' << 8) | 0xfb)
#تعریف_کرو QCOW_نسخہ		2
#تعریف_کرو QCOW_OFLAG_COPIED	(1ULL << 63)
#تعریف_کرو QCOW_OFLAG_COMPRESSED	(1ULL << 62)

#تعریف_کرو QCOW_COMPRESSED		1
#تعریف_کرو QCOW_ENCRYPTED		2
#تعریف_کرو QCOW_CORRUPTED		3

ساخت ext2_qcow2_hdr {
	__u32	magic;
	__u32	نسخہ;

	__u64	backing_file_انحراف;
	__u32	backing_file_سائز;

	__u32	cluster_بٹ;
	__u64	سائز;
	__u32	crypt_طریقہ;

	__u32	l1_سائز;
	__u64	l1_جدول_انحراف;

	__u64	refcount_جدول_انحراف;
	__u32	refcount_جدول_clusters;

	__u32	nb_snapshots;
	__u64	snapshots_انحراف;
};

قسمتعریف ساخت ext2_qcow2_l2_جدول L2_CACHE_سر;

ساخت ext2_qcow2_l2_جدول {
	__u32		l1_اشاریہ;
	__u64		انحراف;
	__u64		*ڈیٹا;
	L2_CACHE_سر	*اگلا;
};

ساخت ext2_qcow2_l2_cache {
	L2_CACHE_سر	*مستعمل_سر;
	L2_CACHE_سر	*مستعمل_دم;
	L2_CACHE_سر	*free_سر;
	__u32		آزاد;
	__u32		گنتی;
	__u64		اگلا_انحراف;
};

ساخت ext2_qcow2_refcount {
	__u64	*refcount_جدول;
	__u64	refcount_جدول_انحراف;
	__u64	refcount_بلاک_انحراف;

	__u32	refcount_جدول_clusters;
	__u32	refcount_جدول_اشاریہ;
	__u32	refcount_بلاک_اشاریہ;

	__u16	*refcount_بلاک;
};

ساخت ext2_qcow2_image {
	صحیح	fd;
	ساخت	ext2_qcow2_hdr		*hdr;
	ساخت	ext2_qcow2_l2_cache	*l2_cache;
	ساخت	ext2_qcow2_refcount	refcount;
	__u32	cluster_سائز;
	__u32	cluster_بٹ;
	__u32	l1_سائز;
	__u32	l2_سائز;

	__u64	*l1_جدول;
	__u64	l2_انحراف;
	__u64	l1_انحراف;
	__u64	image_سائز;
};

/* Function prototypes */

/* qcow2.c */

/* Functions for converting qcow2 image into raw image */
ساخت ext2_qcow2_hdr *qcow2_read_سرخی(صحیح);
صحیح qcow2_write_raw_image(صحیح, صحیح, ساخت ext2_qcow2_hdr *);

