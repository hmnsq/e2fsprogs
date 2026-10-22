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

#అన్తర్భావ <ext2fs/ext2_types.h>

/* Number of l2 tables in memory before writeback */
#నిర్వచించు L2_CACHE_PREALLOC	512


#నిర్వచించు QCOW_MAGIC (('Q' << 24) | ('F' << 16) | ('I' << 8) | 0xfb)
#నిర్వచించు QCOW_సంస్కరణ		2
#నిర్వచించు QCOW_OFLAG_COPIED	(1ULL << 63)
#నిర్వచించు QCOW_OFLAG_COMPRESSED	(1ULL << 62)

#నిర్వచించు QCOW_COMPRESSED		1
#నిర్వచించు QCOW_ENCRYPTED		2
#నిర్వచించు QCOW_CORRUPTED		3

నిర్మాణం ext2_qcow2_hdr {
	__u32	magic;
	__u32	సంస్కరణ;

	__u64	backing_file_స్థానభ్రంశం;
	__u32	backing_file_కొలత;

	__u32	cluster_బిట్లు;
	__u64	కొలత;
	__u32	crypt_పద్ధతి;

	__u32	l1_కొలత;
	__u64	l1_పట్టిక_స్థానభ్రంశం;

	__u64	refcount_పట్టిక_స్థానభ్రంశం;
	__u32	refcount_పట్టిక_clusters;

	__u32	nb_snapshots;
	__u64	snapshots_స్థానభ్రంశం;
};

రకనిర్వచనం నిర్మాణం ext2_qcow2_l2_పట్టిక L2_CACHE_తల;

నిర్మాణం ext2_qcow2_l2_పట్టిక {
	__u32		l1_సూచీ;
	__u64		స్థానభ్రంశం;
	__u64		*దత్తాంశం;
	L2_CACHE_తల	*తదుపరి;
};

నిర్మాణం ext2_qcow2_l2_cache {
	L2_CACHE_తల	*వాడిన_తల;
	L2_CACHE_తల	*వాడిన_తోక;
	L2_CACHE_తల	*free_తల;
	__u32		విడుదల;
	__u32		లెక్క;
	__u64		తదుపరి_స్థానభ్రంశం;
};

నిర్మాణం ext2_qcow2_refcount {
	__u64	*refcount_పట్టిక;
	__u64	refcount_పట్టిక_స్థానభ్రంశం;
	__u64	refcount_ఖండం_స్థానభ్రంశం;

	__u32	refcount_పట్టిక_clusters;
	__u32	refcount_పట్టిక_సూచీ;
	__u32	refcount_ఖండం_సూచీ;

	__u16	*refcount_ఖండం;
};

నిర్మాణం ext2_qcow2_image {
	పూర్ణాంకం	fd;
	నిర్మాణం	ext2_qcow2_hdr		*hdr;
	నిర్మాణం	ext2_qcow2_l2_cache	*l2_cache;
	నిర్మాణం	ext2_qcow2_refcount	refcount;
	__u32	cluster_కొలత;
	__u32	cluster_బిట్లు;
	__u32	l1_కొలత;
	__u32	l2_కొలత;

	__u64	*l1_పట్టిక;
	__u64	l2_స్థానభ్రంశం;
	__u64	l1_స్థానభ్రంశం;
	__u64	image_కొలత;
};

/* Function prototypes */

/* qcow2.c */

/* Functions for converting qcow2 image into raw image */
నిర్మాణం ext2_qcow2_hdr *qcow2_read_శీర్షిక(పూర్ణాంకం);
పూర్ణాంకం qcow2_write_raw_image(పూర్ణాంకం, పూర్ణాంకం, నిర్మాణం ext2_qcow2_hdr *);

