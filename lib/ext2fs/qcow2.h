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

#समावेश <ext2fs/ext2_types.h>

/* Number of l2 tables in memory before writeback */
#घोषणा L2_CACHE_PREALLOC	512


#घोषणा QCOW_MAGIC (('Q' << 24) | ('F' << 16) | ('I' << 8) | 0xfb)
#घोषणा QCOW_VERSION		2
#घोषणा QCOW_OFLAG_COPIED	(1ULL << 63)
#घोषणा QCOW_OFLAG_COMPRESSED	(1ULL << 62)

#घोषणा QCOW_COMPRESSED		1
#घोषणा QCOW_ENCRYPTED		2
#घोषणा QCOW_CORRUPTED		3

काष्ठा ext2_qcow2_hdr {
	__u32	magic;
	__u32	version;

	__u64	backing_file_offset;
	__u32	backing_file_आकार;

	__u32	cluster_bits;
	__u64	आकार;
	__u32	crypt_method;

	__u32	l1_आकार;
	__u64	l1_table_offset;

	__u64	refcount_table_offset;
	__u32	refcount_table_clusters;

	__u32	nb_snapshots;
	__u64	snapshots_offset;
};

प्रकार काष्ठा ext2_qcow2_l2_table L2_CACHE_शीर्ष;

काष्ठा ext2_qcow2_l2_table {
	__u32		l1_सूचकांक;
	__u64		offset;
	__u64		*तथ्य;
	L2_CACHE_शीर्ष	*अग्र;
};

काष्ठा ext2_qcow2_l2_cache {
	L2_CACHE_शीर्ष	*used_शीर्ष;
	L2_CACHE_शीर्ष	*used_पुच्छ;
	L2_CACHE_शीर्ष	*free_शीर्ष;
	__u32		मुक्त;
	__u32		गिनती;
	__u64		अग्र_offset;
};

काष्ठा ext2_qcow2_refcount {
	__u64	*refcount_table;
	__u64	refcount_table_offset;
	__u64	refcount_block_offset;

	__u32	refcount_table_clusters;
	__u32	refcount_table_सूचकांक;
	__u32	refcount_block_सूचकांक;

	__u16	*refcount_block;
};

काष्ठा ext2_qcow2_image {
	पूर्णांक	fd;
	काष्ठा	ext2_qcow2_hdr		*hdr;
	काष्ठा	ext2_qcow2_l2_cache	*l2_cache;
	काष्ठा	ext2_qcow2_refcount	refcount;
	__u32	cluster_आकार;
	__u32	cluster_bits;
	__u32	l1_आकार;
	__u32	l2_आकार;

	__u64	*l1_table;
	__u64	l2_offset;
	__u64	l1_offset;
	__u64	image_आकार;
};

/* Function prototypes */

/* qcow2.c */

/* Functions for converting qcow2 image into raw image */
काष्ठा ext2_qcow2_hdr *qcow2_read_header(पूर्णांक);
पूर्णांक qcow2_write_raw_image(पूर्णांक, पूर्णांक, काष्ठा ext2_qcow2_hdr *);

