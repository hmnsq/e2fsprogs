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
#define WRITE_ERR_S(_msg_) write(2, _msg_, sizeof(_msg_))

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
static errcode_t dos_open(const char *dev, цео flags, io_channel *channel);
static errcode_t dos_close(io_channel channel);
static errcode_t dos_set_blksize(io_channel channel, цео blksize);
static errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             цео count, void *buf);
static errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               цео count, const void *buf);
static errcode_t dos_flush(io_channel channel);

static struct struct_io_manager struct_dos_manager = {
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

#define EXT2_CHECK_MAGIC(struct, code) \
          ако ((struct)->magic != (code)) врати (code)

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
                                          unsigned char *pno)
{
  unsigned        i;

  ако(*pno != 0xFF && *pno >= 5)
     врати NULL; /* We don't support extended partitions for now */

  ако(*pno != 0xFF)
  {
    ако(pentry[*pno].type == LINUX_EXT2FS)
      врати &pentry[*pno];
    инако
    {
      ако(!pentry[*pno].type)
        *pno = 0xFE;
      инако ако(pentry[*pno].type == LINUX_SWAP)
        *pno = 0xFD;
      врати NULL;
    }
  }

  за(i = 0; i < 4; i++)
    ако(pentry[i].type == LINUX_EXT2FS)
    {
      *pno = i;
      врати &pentry[i];
    }

  врати NULL;
}

/*
 * Allocate libext2fs structures associated with I/O manager
 */
static io_channel alloc_io_channel(PARTITION *part)
{
  io_channel     ioch;

  ioch = (io_channel)malloc(sizeof(struct struct_io_channel));
  ако (!ioch)
	  врати NULL;
  memset(ioch, 0, sizeof(struct struct_io_channel));
  ioch->magic = EXT2_ET_MAGIC_IO_CHANNEL;
  ioch->manager = dos_io_manager;
  ioch->name = (char *)malloc(strlen(part->dev)+1);
  ако (!ioch->name) {
	  free(ioch);
	  врати NULL;
  }
  strcpy(ioch->name, part->dev);
  ioch->private_data = part;
  ioch->block_size = 1024; /* The smallest ext2fs block size */
  ioch->read_error = 0;
  ioch->write_error = 0;

  врати ioch;
}

#ifdef __TURBOC__
#pragma argsused
#endif
/*
 * Open the 'name' partition, initialize all information structures
 * we need to keep and create libext2fs I/O manager.
 */
static errcode_t dos_open(const char *dev, цео flags, io_channel *channel)
{
  unsigned char  *tmp, sec[512];
  PARTITION      *part;
  PTABLE_ENTRY   *pent;
  PARTITION        **newparts;

  ако(!dev)
  {
    _dio_error = ERR_BADDEV;
    врати EXT2_ET_BAD_DEVICE_NAME;
  }

  /*
   * First check whether the dev name is OK
   */
  tmp = (unsigned char*)strrchr(dev, '/');
  ако(!tmp)
  {
    _dio_error = ERR_BADDEV;
    врати EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp = 0;
  ако(strcmp(dev, "/dev"))
  {
    _dio_error = ERR_BADDEV;
    врати EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp++ = '/';

  /*
   * Check whether the partition data is already in cache
   */

  part = (PARTITION*)malloc(sizeof(PARTITION));
  ако (!part)
	  врати ENOMEM;
  {
    цео   i = 0;

    за(;i < npart; i++)
      ако(!strcmp(partitions[i]->dev, dev))
      {
        /* Found it! Make it the active one */
        active = partitions[i];
        *channel = alloc_io_channel(active);
	ако (!*channel)
		врати ENOMEM;
        врати 0;
      }
  }

  /*
   * Drive number & optionally partn number
   */
  switch(tmp[0])
  {
    case 'h':
    case 's':
      part->phys = 0x80;
      part->phys += toupper(tmp[2]) - 'A';
      /*
       * Do we have the partition number?
       */
      ако(tmp[3])
        part->pno = isdigit((цео)tmp[3]) ? tmp[3] - '0' - 1: 0;
      инако
        part->pno = 0xFF;
      break;

    case 'f':
      ако(tmp[2])
        part->phys = isdigit((цео)tmp[2]) ? tmp[2] - '0' : 0;
      инако
        part->phys = 0x00; /* We'll assume /dev/fd0 */
      break;

    default:
      _dio_error = ERR_BADDEV;
      врати ENODEV;
  }

  ако(part->phys < 0x80)
  {
     /* We don't support floppies for now */
     _dio_error = ERR_NOTSUPP;
     врати EINVAL;
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

  ако(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    free(part->dev);
    free(part);
    врати EFAULT;
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

  ако(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    free(part->dev);
    free(part);
    врати EFAULT;
  }

  pent = (PTABLE_ENTRY*)&sec[0x1BE];
  pent = scan_partition_table(pent, part->phys, &part->pno);

  ако(!pent)
  {
    _dio_error = part->pno == 0xFE ? ERR_EMPTYPART :
                 part->pno == 0xFD ? ERR_LINUXSWAP : ERR_NOTEXT2FS;
    free(part->dev);
    free(part);
    врати ENODEV;
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
  newparts = (PARTITION**)realloc(partitions, sizeof(PARTITION) * npart);
  ако (!newparts) {
	  free(part);
	  врати ENOMEM;
  }
  partitions = newparts;
  partitions[npart++] = active = part;

  /*
   * Now alloc all libe2fs structures
   */
  *channel = alloc_io_channel(active);
  ако (!*channel)
	  врати ENOMEM;

  врати 0;
}

static errcode_t dos_close(io_channel channel)
{
	free(channel->name);
	free(channel);

	врати 0;
}

static errcode_t dos_set_blksize(io_channel channel, цео blksize)
{
  channel->block_size = blksize;

  врати 0;
}

static errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             цео count, void *buf)
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

  ако(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    врати EFAULT;
  }

  врати 0;
}

static errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               цео count, const void *buf)
{
  PARTITION     *part;
  size_t        size;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_data;

  ако(count == 1)
    size = (size_t)channel->block_size;
  инако
  {
    ако (count < 0)
      size = (size_t)-count;
    инако
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

  ако(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    врати EFAULT;
  }

  врати 0;
}

#ifdef __TURBOC__
#pragma argsused
#endif
static errcode_t dos_flush(io_channel channel)
{
  /*
   * No buffers, no flush...
   */
  врати 0;
}
