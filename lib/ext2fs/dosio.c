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
static errcode_t dos_open(const var_na *dev, පූර්ණ_සංඛ්‍යා flags, io_channel *channel);
static errcode_t dos_close(io_channel channel);
static errcode_t dos_set_blksize(io_channel channel, පූර්ණ_සංඛ්‍යා blksize);
static errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             පූර්ණ_සංඛ්‍යා count, void *buf);
static errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               පූර්ණ_සංඛ්‍යා count, const void *buf);
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
          නම් ((sa_nracanaa)->magic != (code)) ආපසු (code)

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

  නම්(*pno != 0xFF && *pno >= 5)
     ආපසු NULL; /* We don't support extended partitions for now */

  නම්(*pno != 0xFF)
  {
    නම්(pentry[*pno].type == LINUX_EXT2FS)
      ආපසු &pentry[*pno];
    නැතහොත්
    {
      නම්(!pentry[*pno].type)
        *pno = 0xFE;
      නැතහොත් නම්(pentry[*pno].type == LINUX_SWAP)
        *pno = 0xFD;
      ආපසු NULL;
    }
  }

  සඳහා(i = 0; i < 4; i++)
    නම්(pentry[i].type == LINUX_EXT2FS)
    {
      *pno = i;
      ආපසු &pentry[i];
    }

  ආපසු NULL;
}

/*
 * Allocate libext2fs structures associated with I/O manager
 */
static io_channel alloc_io_channel(PARTITION *part)
{
  io_channel     ioch;

  ioch = (io_channel)malloc(parimaa_na(sa_nracanaa struct_io_channel));
  නම් (!ioch)
	  ආපසු NULL;
  memset(ioch, 0, parimaa_na(sa_nracanaa struct_io_channel));
  ioch->magic = EXT2_ET_MAGIC_IO_CHANNEL;
  ioch->manager = dos_io_manager;
  ioch->name = (var_na *)malloc(strlen(part->dev)+1);
  නම් (!ioch->name) {
	  free(ioch);
	  ආපසු NULL;
  }
  strcpy(ioch->name, part->dev);
  ioch->private_data = part;
  ioch->block_size = 1024; /* The smallest ext2fs block size */
  ioch->read_error = 0;
  ioch->write_error = 0;

  ආපසු ioch;
}

#ifdef __TURBOC__
#pragma argsused
#endif
/*
 * Open the 'name' partition, initialize all information structures
 * we need to keep and create libext2fs I/O manager.
 */
static errcode_t dos_open(const var_na *dev, පූර්ණ_සංඛ්‍යා flags, io_channel *channel)
{
  unsigned var_na  *tmp, sec[512];
  PARTITION      *part;
  PTABLE_ENTRY   *pent;
  PARTITION        **newparts;

  නම්(!dev)
  {
    _dio_error = ERR_BADDEV;
    ආපසු EXT2_ET_BAD_DEVICE_NAME;
  }

  /*
   * First check whether the dev name is OK
   */
  tmp = (unsigned var_na*)strrchr(dev, '/');
  නම්(!tmp)
  {
    _dio_error = ERR_BADDEV;
    ආපසු EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp = 0;
  නම්(strcmp(dev, "/dev"))
  {
    _dio_error = ERR_BADDEV;
    ආපසු EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp++ = '/';

  /*
   * Check whether the partition data is already in cache
   */

  part = (PARTITION*)malloc(parimaa_na(PARTITION));
  නම් (!part)
	  ආපසු ENOMEM;
  {
    පූර්ණ_සංඛ්‍යා   i = 0;

    සඳහා(;i < npart; i++)
      නම්(!strcmp(partitions[i]->dev, dev))
      {
        /* Found it! Make it the active one */
        active = partitions[i];
        *channel = alloc_io_channel(active);
	නම් (!*channel)
		ආපසු ENOMEM;
        ආපසු 0;
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
      නම්(tmp[3])
        part->pno = isdigit((පූර්ණ_සංඛ්‍යා)tmp[3]) ? tmp[3] - '0' - 1: 0;
      නැතහොත්
        part->pno = 0xFF;
      bha_nga;

    pak_sa 'f':
      නම්(tmp[2])
        part->phys = isdigit((පූර්ණ_සංඛ්‍යා)tmp[2]) ? tmp[2] - '0' : 0;
      නැතහොත්
        part->phys = 0x00; /* We'll assume /dev/fd0 */
      bha_nga;

    default:
      _dio_error = ERR_BADDEV;
      ආපසු ENODEV;
  }

  නම්(part->phys < 0x80)
  {
     /* We don't support floppies for now */
     _dio_error = ERR_NOTSUPP;
     ආපසු EINVAL;
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

  නම්(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    free(part->dev);
    free(part);
    ආපසු EFAULT;
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

  නම්(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    free(part->dev);
    free(part);
    ආපසු EFAULT;
  }

  pent = (PTABLE_ENTRY*)&sec[0x1BE];
  pent = scan_partition_table(pent, part->phys, &part->pno);

  නම්(!pent)
  {
    _dio_error = part->pno == 0xFE ? ERR_EMPTYPART :
                 part->pno == 0xFD ? ERR_LINUXSWAP : ERR_NOTEXT2FS;
    free(part->dev);
    free(part);
    ආපසු ENODEV;
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
  නම් (!newparts) {
	  free(part);
	  ආපසු ENOMEM;
  }
  partitions = newparts;
  partitions[npart++] = active = part;

  /*
   * Now alloc all libe2fs structures
   */
  *channel = alloc_io_channel(active);
  නම් (!*channel)
	  ආපසු ENOMEM;

  ආපසු 0;
}

static errcode_t dos_close(io_channel channel)
{
	free(channel->name);
	free(channel);

	ආපසු 0;
}

static errcode_t dos_set_blksize(io_channel channel, පූර්ණ_සංඛ්‍යා blksize)
{
  channel->block_size = blksize;

  ආපසු 0;
}

static errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             පූර්ණ_සංඛ්‍යා count, void *buf)
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

  නම්(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    ආපසු EFAULT;
  }

  ආපසු 0;
}

static errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               පූර්ණ_සංඛ්‍යා count, const void *buf)
{
  PARTITION     *part;
  size_t        size;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_data;

  නම්(count == 1)
    size = (size_t)channel->block_size;
  නැතහොත්
  {
    නම් (count < 0)
      size = (size_t)-count;
    නැතහොත්
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

  නම්(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    ආපසු EFAULT;
  }

  ආපසු 0;
}

#ifdef __TURBOC__
#pragma argsused
#endif
static errcode_t dos_flush(io_channel channel)
{
  /*
   * No buffers, no flush...
   */
  ආපසු 0;
}
