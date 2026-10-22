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

#অন্তর্ভাব <ext2fs/ext2_types.h>

/* Number of l2 tables in memory before writeback */
#define L2_CACHE_PREALLOC	512


#define QCOW_MAGIC (('Q' << 24) | ('F' << 16) | ('I' << 8) | 0xfb)
#define QCOW_VERSION		2
#define QCOW_OFLAG_COPIED	(1ULL << 63)
#define QCOW_OFLAG_COMPRESSED	(1ULL << 62)

#define QCOW_COMPRESSED		1
#define QCOW_ENCRYPTED		2
#define QCOW_CORRUPTED		3

কাঠামো ext2_qcow2_hdr {
	__u32	magic;
	__u32	version;

	__u64	backing_file_offset;
	__u32	backing_file_আকার;

	__u32	cluster_bits;
	__u64	আকার_2;
	__u32	crypt_method;

	__u32	l1_আকার;
	__u64	l1_table_offset;

	__u64	refcount_table_offset;
	__u32	refcount_table_clusters;

	__u32	nb_snapshots;
	__u64	snapshots_offset;
};

প্রকারলেখ কাঠামো ext2_qcow2_l2_table L2_CACHE_শীর্ষ;

কাঠামো ext2_qcow2_l2_table {
	__u32		l1_সূচক;
	__u64		offset;
	__u64		*তথ্য;
	L2_CACHE_শীর্ষ	*পরবর্তী;
};

কাঠামো ext2_qcow2_l2_cache {
	L2_CACHE_শীর্ষ	*used_শীর্ষ;
	L2_CACHE_শীর্ষ	*used_লেজ;
	L2_CACHE_শীর্ষ	*free_শীর্ষ;
	__u32		মুক্ত;
	__u32		গুনতি;
	__u64		পরবর্তী_offset;
};

কাঠামো ext2_qcow2_refcount {
	__u64	*refcount_table;
	__u64	refcount_table_offset;
	__u64	refcount_block_offset;

	__u32	refcount_table_clusters;
	__u32	refcount_table_সূচক;
	__u32	refcount_block_সূচক;

	__u16	*refcount_block;
};

কাঠামো ext2_qcow2_image {
	পূর্ণসংখ্যা	fd;
	কাঠামো	ext2_qcow2_hdr		*hdr;
	কাঠামো	ext2_qcow2_l2_cache	*l2_cache;
	কাঠামো	ext2_qcow2_refcount	refcount;
	__u32	cluster_আকার;
	__u32	cluster_bits;
	__u32	l1_আকার;
	__u32	l2_আকার;

	__u64	*l1_table;
	__u64	l2_offset;
	__u64	l1_offset;
	__u64	image_আকার;
};

/* Function prototypes */

/* qcow2.c */

/* Functions for converting qcow2 image into raw image */
কাঠামো ext2_qcow2_hdr *qcow2_read_header(পূর্ণসংখ্যা);
পূর্ণসংখ্যা qcow2_write_raw_image(পূর্ণসংখ্যা, পূর্ণসংখ্যা, কাঠামো ext2_qcow2_hdr *);

