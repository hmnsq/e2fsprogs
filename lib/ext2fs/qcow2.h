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

#अन्तर्भाव <ext2fs/ext2_types.h>

/* Number of l2 tables in memory before writeback */
#परिभाषय L2_CACHE_PREALLOC	512


#परिभाषय QCOW_MAGIC (('Q' << 24) | ('F' << 16) | ('I' << 8) | 0xfb)
#परिभाषय QCOW_संस्करणम्		2
#परिभाषय QCOW_OFLAG_COPIED	(1ULL << 63)
#परिभाषय QCOW_OFLAG_COMPRESSED	(1ULL << 62)

#परिभाषय QCOW_COMPRESSED		1
#परिभाषय QCOW_ENCRYPTED		2
#परिभाषय QCOW_CORRUPTED		3

संरचना ext2_qcow2_hdr {
	__u32	magic;
	__u32	संस्करणम्;

	__u64	backing_file_विस्थापनम्;
	__u32	backing_file_आकार;

	__u32	cluster_द्वयङ्काः;
	__u64	आकार;
	__u32	crypt_विधानम्;

	__u32	l1_आकार;
	__u64	l1_सारणी_विस्थापनम्;

	__u64	refcount_सारणी_विस्थापनम्;
	__u32	refcount_सारणी_clusters;

	__u32	nb_snapshots;
	__u64	snapshots_विस्थापनम्;
};

प्रकारनिर्देश संरचना ext2_qcow2_l2_सारणी L2_CACHE_शीर्ष;

संरचना ext2_qcow2_l2_सारणी {
	__u32		l1_सूचकांक;
	__u64		विस्थापनम्;
	__u64		*दत्त;
	L2_CACHE_शीर्ष	*अग्र;
};

संरचना ext2_qcow2_l2_cache {
	L2_CACHE_शीर्ष	*प्रयुक्तम्_शीर्ष;
	L2_CACHE_शीर्ष	*प्रयुक्तम्_पुच्छ;
	L2_CACHE_शीर्ष	*free_शीर्ष;
	__u32		मुक्त;
	__u32		गणनम्;
	__u64		अग्र_विस्थापनम्;
};

संरचना ext2_qcow2_refcount {
	__u64	*refcount_सारणी;
	__u64	refcount_सारणी_विस्थापनम्;
	__u64	refcount_खण्डम्_विस्थापनम्;

	__u32	refcount_सारणी_clusters;
	__u32	refcount_सारणी_सूचकांक;
	__u32	refcount_खण्डम्_सूचकांक;

	__u16	*refcount_खण्डम्;
};

संरचना ext2_qcow2_image {
	पूर्णाङ्क	fd;
	संरचना	ext2_qcow2_hdr		*hdr;
	संरचना	ext2_qcow2_l2_cache	*l2_cache;
	संरचना	ext2_qcow2_refcount	refcount;
	__u32	cluster_आकार;
	__u32	cluster_द्वयङ्काः;
	__u32	l1_आकार;
	__u32	l2_आकार;

	__u64	*l1_सारणी;
	__u64	l2_विस्थापनम्;
	__u64	l1_विस्थापनम्;
	__u64	image_आकार;
};

/* Function prototypes */

/* qcow2.c */

/* Functions for converting qcow2 image into raw image */
संरचना ext2_qcow2_hdr *qcow2_read_शीर्षकम्(पूर्णाङ्क);
पूर्णाङ्क qcow2_write_raw_image(पूर्णाङ्क, पूर्णाङ्क, संरचना ext2_qcow2_hdr *);

