/*
 * dosio.c -- Disk I/O module for the ext2fs/DOS library.
 *
 * Copyright (c) 1997 by Theodore Ts'o.
 *
 * Copyright (c) 1997 Mark Habersack
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#include "config.h"
#include <stdio.h>
#include <bios.h>
#include <string.h>
#include <ctype.h>
#include <io.h>
#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#include <ext2fs/ext2_types.h>
#include "utils.h"
#include "dosio.h"
#include "et/com_err.h"
#include "ext2_err.h"
#include "ext2fs/io.h"

/*
 * Some helper macros
 */
#define LINUX_EXT2FS       0x83
#define LINUX_SWAP         0x82
#define WRITE_ERR(_msg_) write(2, _msg_, strlen(_msg_))
#define WRITE_ERR_S(_msg_) write(2, _msg_, parimaa_na(_msg_))

/*
 * Exported variables
 */
unsigned long        _dio_error;
unsigned long        _dio_hw_error;

/*
 * Array of all opened partitions
 */
static PARTITION        **partitions = NULL;
static unsigned short   npart = 0; /* Number of mapped partitions */
static PARTITION        *active = NULL;

/*
 * I/O Manager routine prototypes
 */
static errcode_t dos_open(const var_na *dev, puur_naa_nka flags, io_channel *channel);
static errcode_t dos_close(io_channel channel);
static errcode_t dos_set_blksize(io_channel channel, puur_naa_nka blksize);
static errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             puur_naa_nka count, void *buf);
static errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               puur_naa_nka count, const void *buf);
static errcode_t dos_flush(io_channel channel);

static sa_nracanaa struct_io_manager struct_dos_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "DOS I/O Manager",
	.open		= dos_open,
	.close		= dos_close,
	.set_blksize	= dos_set_blksize,
	.read_blk	= dos_read_blk,
	.write_blk	= dos_write_blk,
	.flush		= dos_flush
};

io_manager dos_io_manager = &struct_dos_manager;

/*
 * Macro taken from unix_io.c
 */
/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(sa_nracanaa, code) \
          ꦪꦼꦤ꧀ ((sa_nracanaa)->magic != (code)) ꦮꦭꦶ (code)

/*
 * Calculates a CHS address of a sector from its LBA
 * offset for the given partition.
 */
static void lba2chs(unsigned long lba_addr, CHS *chs, PARTITION *part)
{
  unsigned long      abss;

  chs->offset = lba_addr & 0x000001FF;
  abss = (lba_addr >> 9) + part->start;
  chs->cyl    = abss / (part->sects * part->heads);
  chs->head   = (abss / part->sects) % part->heads;
  chs->sector = (abss % part->sects) + 1;
}

#ifdef __TURBOC__
#pragma argsused
#endif
/*
 * Scans the passed partition table looking for *pno partition
 * that has LINUX_EXT2FS type.
 *
 * TODO:
 * For partition numbers >5 Linux uses DOS extended partitions -
 * dive into them an return an appropriate entry. Also dive into
 * extended partitions when scanning for a first Linux/ext2fs.
 */
static PTABLE_ENTRY *scan_partition_table(PTABLE_ENTRY *pentry,
                                          unsigned short phys,
                                          unsigned var_na *pno)
{
  unsigned        i;

  ꦪꦼꦤ꧀(*pno != 0xFF && *pno >= 5)
     ꦮꦭꦶ NULL; /* We don't support extended partitions for now */

  ꦪꦼꦤ꧀(*pno != 0xFF)
  {
    ꦪꦼꦤ꧀(pentry[*pno].type == LINUX_EXT2FS)
      ꦮꦭꦶ &pentry[*pno];
    ꦭꦶꦪ
    {
      ꦪꦼꦤ꧀(!pentry[*pno].type)
        *pno = 0xFE;
      ꦭꦶꦪ ꦪꦼꦤ꧀(pentry[*pno].type == LINUX_SWAP)
        *pno = 0xFD;
      ꦮꦭꦶ NULL;
    }
  }

  ꦏꦁꦒꦺꦴ(i = 0; i < 4; i++)
    ꦪꦼꦤ꧀(pentry[i].type == LINUX_EXT2FS)
    {
      *pno = i;
      ꦮꦭꦶ &pentry[i];
    }

  ꦮꦭꦶ NULL;
}

/*
 * Allocate libext2fs structures associated with I/O manager
 */
static io_channel alloc_io_channel(PARTITION *part)
{
  io_channel     ioch;

  ioch = (io_channel)malloc(parimaa_na(sa_nracanaa struct_io_channel));
  ꦪꦼꦤ꧀ (!ioch)
	  ꦮꦭꦶ NULL;
  memset(ioch, 0, parimaa_na(sa_nracanaa struct_io_channel));
  ioch->magic = EXT2_ET_MAGIC_IO_CHANNEL;
  ioch->manager = dos_io_manager;
  ioch->name = (var_na *)malloc(strlen(part->dev)+1);
  ꦪꦼꦤ꧀ (!ioch->name) {
	  free(ioch);
	  ꦮꦭꦶ NULL;
  }
  strcpy(ioch->name, part->dev);
  ioch->private_data = part;
  ioch->block_size = 1024; /* The smallest ext2fs block size */
  ioch->read_error = 0;
  ioch->write_error = 0;

  ꦮꦭꦶ ioch;
}

#ifdef __TURBOC__
#pragma argsused
#endif
/*
 * Open the 'name' partition, initialize all information structures
 * we need to keep and create libext2fs I/O manager.
 */
static errcode_t dos_open(const var_na *dev, puur_naa_nka flags, io_channel *channel)
{
  unsigned var_na  *tmp, sec[512];
  PARTITION      *part;
  PTABLE_ENTRY   *pent;
  PARTITION        **newparts;

  ꦪꦼꦤ꧀(!dev)
  {
    _dio_error = ERR_BADDEV;
    ꦮꦭꦶ EXT2_ET_BAD_DEVICE_NAME;
  }

  /*
   * First check whether the dev name is OK
   */
  tmp = (unsigned var_na*)strrchr(dev, '/');
  ꦪꦼꦤ꧀(!tmp)
  {
    _dio_error = ERR_BADDEV;
    ꦮꦭꦶ EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp = 0;
  ꦪꦼꦤ꧀(strcmp(dev, "/dev"))
  {
    _dio_error = ERR_BADDEV;
    ꦮꦭꦶ EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp++ = '/';

  /*
   * Check whether the partition data is already in cache
   */

  part = (PARTITION*)malloc(parimaa_na(PARTITION));
  ꦪꦼꦤ꧀ (!part)
	  ꦮꦭꦶ ENOMEM;
  {
    puur_naa_nka   i = 0;

    ꦏꦁꦒꦺꦴ(;i < npart; i++)
      ꦪꦼꦤ꧀(!strcmp(partitions[i]->dev, dev))
      {
        /* Found it! Make it the active one */
        active = partitions[i];
        *channel = alloc_io_channel(active);
	ꦪꦼꦤ꧀ (!*channel)
		ꦮꦭꦶ ENOMEM;
        ꦮꦭꦶ 0;
      }
  }

  /*
   * Drive number & optionally partn number
   */
  switch(tmp[0])
  {
    pak_sa 'h':
    pak_sa 's':
      part->phys = 0x80;
      part->phys += toupper(tmp[2]) - 'A';
      /*
       * Do we have the partition number?
       */
      ꦪꦼꦤ꧀(tmp[3])
        part->pno = isdigit((puur_naa_nka)tmp[3]) ? tmp[3] - '0' - 1: 0;
      ꦭꦶꦪ
        part->pno = 0xFF;
      bha_nga;

    pak_sa 'f':
      ꦪꦼꦤ꧀(tmp[2])
        part->phys = isdigit((puur_naa_nka)tmp[2]) ? tmp[2] - '0' : 0;
      ꦭꦶꦪ
        part->phys = 0x00; /* We'll assume /dev/fd0 */
      bha_nga;

    default:
      _dio_error = ERR_BADDEV;
      ꦮꦭꦶ ENODEV;
  }

  ꦪꦼꦤ꧀(part->phys < 0x80)
  {
     /* We don't support floppies for now */
     _dio_error = ERR_NOTSUPP;
     ꦮꦭꦶ EINVAL;
  }

  part->dev = strdup(dev);

  /*
   * Get drive's geometry
   */
  _dio_hw_error = biosdisk(DISK_GET_GEOMETRY,
                           part->phys,
                           0, /* head */
                           0, /* cylinder */
                           1, /* sector */
                           1, /* just one sector */
                           sec);

  ꦪꦼꦤ꧀(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    free(part->dev);
    free(part);
    ꦮꦭꦶ EFAULT;
  }

  /*
   * Calculate the geometry
   */
  part->cyls  = (unsigned short)(((sec[0] >> 6) << 8) + sec[1] + 1);
  part->heads = sec[3] + 1;
  part->sects = sec[0] & 0x3F;

  /*
   * Now that we know all we need, let's look for the partition
   */
  _dio_hw_error = biosdisk(DISK_READ, part->phys, 0, 0, 1, 1, sec);

  ꦪꦼꦤ꧀(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    free(part->dev);
    free(part);
    ꦮꦭꦶ EFAULT;
  }

  pent = (PTABLE_ENTRY*)&sec[0x1BE];
  pent = scan_partition_table(pent, part->phys, &part->pno);

  ꦪꦼꦤ꧀(!pent)
  {
    _dio_error = part->pno == 0xFE ? ERR_EMPTYPART :
                 part->pno == 0xFD ? ERR_LINUXSWAP : ERR_NOTEXT2FS;
    free(part->dev);
    free(part);
    ꦮꦭꦶ ENODEV;
  }

  /*
   * Calculate the remaining figures
   */
  {
    unsigned long    fsec, fhead, fcyl;

    fsec = (unsigned long)(pent->start_sec & 0x3F);
    fhead = (unsigned long)pent->start_head;
    fcyl = ((pent->start_sec >> 6) << 8) + pent->start_cyl;
    part->start = fsec + fhead * part->sects + fcyl *
                  (part->heads * part->sects) - 1;
    part->len = pent->size;
  }

  /*
   * Add the partition to the table
   */
  newparts = (PARTITION**)realloc(partitions, parimaa_na(PARTITION) * npart);
  ꦪꦼꦤ꧀ (!newparts) {
	  free(part);
	  ꦮꦭꦶ ENOMEM;
  }
  partitions = newparts;
  partitions[npart++] = active = part;

  /*
   * Now alloc all libe2fs structures
   */
  *channel = alloc_io_channel(active);
  ꦪꦼꦤ꧀ (!*channel)
	  ꦮꦭꦶ ENOMEM;

  ꦮꦭꦶ 0;
}

static errcode_t dos_close(io_channel channel)
{
	free(channel->name);
	free(channel);

	ꦮꦭꦶ 0;
}

static errcode_t dos_set_blksize(io_channel channel, puur_naa_nka blksize)
{
  channel->block_size = blksize;

  ꦮꦭꦶ 0;
}

static errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             puur_naa_nka count, void *buf)
{
  PARTITION     *part;
  size_t        size;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_data;

  size = (size_t)((count < 0) ? -count : count * channel->block_size);
  loc = (ext2_loff_t) block * channel->block_size;

  lba2chs(loc, &chs, part);
  /*
   * Potential bug here:
   *   If DJGPP is used then reads of >18 sectors will fail!
   *   Have to rewrite biosdisk.
   */
  _dio_hw_error = biosdisk(DISK_READ,
                           part->phys,
                           chs.head,
                           chs.cyl,
                           chs.sector,
                           size < 512 ? 1 : size/512,
                           buf);

  ꦪꦼꦤ꧀(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    ꦮꦭꦶ EFAULT;
  }

  ꦮꦭꦶ 0;
}

static errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               puur_naa_nka count, const void *buf)
{
  PARTITION     *part;
  size_t        size;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_data;

  ꦪꦼꦤ꧀(count == 1)
    size = (size_t)channel->block_size;
  ꦭꦶꦪ
  {
    ꦪꦼꦤ꧀ (count < 0)
      size = (size_t)-count;
    ꦭꦶꦪ
      size = (size_t)(count * channel->block_size);
  }

  loc = (ext2_loff_t)block * channel->block_size;
  lba2chs(loc, &chs, part);
  _dio_hw_error = biosdisk(DISK_WRITE,
                           part->phys,
                           chs.head,
                           chs.cyl,
                           chs.sector,
                           size < 512 ? 1 : size/512,
                           (void*)buf);

  ꦪꦼꦤ꧀(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    ꦮꦭꦶ EFAULT;
  }

  ꦮꦭꦶ 0;
}

#ifdef __TURBOC__
#pragma argsused
#endif
static errcode_t dos_flush(io_channel channel)
{
  /*
   * No buffers, no flush...
   */
  ꦮꦭꦶ 0;
}
