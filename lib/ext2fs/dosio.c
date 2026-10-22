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
static errcode_t dos_open(const var_na *dev, ཧྲིལ flags, io_channel *channel);
static errcode_t dos_close(io_channel channel);
static errcode_t dos_set_blksize(io_channel channel, ཧྲིལ blksize);
static errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             ཧྲིལ count, void *buf);
static errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               ཧྲིལ count, const void *buf);
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
          གལ་ཏེ ((sa_nracanaa)->magic != (code)) ཕྱིར (code)

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

  གལ་ཏེ(*pno != 0xFF && *pno >= 5)
     ཕྱིར NULL; /* We don't support extended partitions for now */

  གལ་ཏེ(*pno != 0xFF)
  {
    གལ་ཏེ(pentry[*pno].type == LINUX_EXT2FS)
      ཕྱིར &pentry[*pno];
    དེ་མིན
    {
      གལ་ཏེ(!pentry[*pno].type)
        *pno = 0xFE;
      དེ་མིན གལ་ཏེ(pentry[*pno].type == LINUX_SWAP)
        *pno = 0xFD;
      ཕྱིར NULL;
    }
  }

  ཆེད་དུ(i = 0; i < 4; i++)
    གལ་ཏེ(pentry[i].type == LINUX_EXT2FS)
    {
      *pno = i;
      ཕྱིར &pentry[i];
    }

  ཕྱིར NULL;
}

/*
 * Allocate libext2fs structures associated with I/O manager
 */
static io_channel alloc_io_channel(PARTITION *part)
{
  io_channel     ioch;

  ioch = (io_channel)malloc(parimaa_na(sa_nracanaa struct_io_channel));
  གལ་ཏེ (!ioch)
	  ཕྱིར NULL;
  memset(ioch, 0, parimaa_na(sa_nracanaa struct_io_channel));
  ioch->magic = EXT2_ET_MAGIC_IO_CHANNEL;
  ioch->manager = dos_io_manager;
  ioch->name = (var_na *)malloc(strlen(part->dev)+1);
  གལ་ཏེ (!ioch->name) {
	  free(ioch);
	  ཕྱིར NULL;
  }
  strcpy(ioch->name, part->dev);
  ioch->private_data = part;
  ioch->block_size = 1024; /* The smallest ext2fs block size */
  ioch->read_error = 0;
  ioch->write_error = 0;

  ཕྱིར ioch;
}

#ifdef __TURBOC__
#pragma argsused
#endif
/*
 * Open the 'name' partition, initialize all information structures
 * we need to keep and create libext2fs I/O manager.
 */
static errcode_t dos_open(const var_na *dev, ཧྲིལ flags, io_channel *channel)
{
  unsigned var_na  *tmp, sec[512];
  PARTITION      *part;
  PTABLE_ENTRY   *pent;
  PARTITION        **newparts;

  གལ་ཏེ(!dev)
  {
    _dio_error = ERR_BADDEV;
    ཕྱིར EXT2_ET_BAD_DEVICE_NAME;
  }

  /*
   * First check whether the dev name is OK
   */
  tmp = (unsigned var_na*)strrchr(dev, '/');
  གལ་ཏེ(!tmp)
  {
    _dio_error = ERR_BADDEV;
    ཕྱིར EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp = 0;
  གལ་ཏེ(strcmp(dev, "/dev"))
  {
    _dio_error = ERR_BADDEV;
    ཕྱིར EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp++ = '/';

  /*
   * Check whether the partition data is already in cache
   */

  part = (PARTITION*)malloc(parimaa_na(PARTITION));
  གལ་ཏེ (!part)
	  ཕྱིར ENOMEM;
  {
    ཧྲིལ   i = 0;

    ཆེད་དུ(;i < npart; i++)
      གལ་ཏེ(!strcmp(partitions[i]->dev, dev))
      {
        /* Found it! Make it the active one */
        active = partitions[i];
        *channel = alloc_io_channel(active);
	གལ་ཏེ (!*channel)
		ཕྱིར ENOMEM;
        ཕྱིར 0;
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
      གལ་ཏེ(tmp[3])
        part->pno = isdigit((ཧྲིལ)tmp[3]) ? tmp[3] - '0' - 1: 0;
      དེ་མིན
        part->pno = 0xFF;
      bha_nga;

    pak_sa 'f':
      གལ་ཏེ(tmp[2])
        part->phys = isdigit((ཧྲིལ)tmp[2]) ? tmp[2] - '0' : 0;
      དེ་མིན
        part->phys = 0x00; /* We'll assume /dev/fd0 */
      bha_nga;

    default:
      _dio_error = ERR_BADDEV;
      ཕྱིར ENODEV;
  }

  གལ་ཏེ(part->phys < 0x80)
  {
     /* We don't support floppies for now */
     _dio_error = ERR_NOTSUPP;
     ཕྱིར EINVAL;
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

  གལ་ཏེ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    free(part->dev);
    free(part);
    ཕྱིར EFAULT;
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

  གལ་ཏེ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    free(part->dev);
    free(part);
    ཕྱིར EFAULT;
  }

  pent = (PTABLE_ENTRY*)&sec[0x1BE];
  pent = scan_partition_table(pent, part->phys, &part->pno);

  གལ་ཏེ(!pent)
  {
    _dio_error = part->pno == 0xFE ? ERR_EMPTYPART :
                 part->pno == 0xFD ? ERR_LINUXSWAP : ERR_NOTEXT2FS;
    free(part->dev);
    free(part);
    ཕྱིར ENODEV;
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
  གལ་ཏེ (!newparts) {
	  free(part);
	  ཕྱིར ENOMEM;
  }
  partitions = newparts;
  partitions[npart++] = active = part;

  /*
   * Now alloc all libe2fs structures
   */
  *channel = alloc_io_channel(active);
  གལ་ཏེ (!*channel)
	  ཕྱིར ENOMEM;

  ཕྱིར 0;
}

static errcode_t dos_close(io_channel channel)
{
	free(channel->name);
	free(channel);

	ཕྱིར 0;
}

static errcode_t dos_set_blksize(io_channel channel, ཧྲིལ blksize)
{
  channel->block_size = blksize;

  ཕྱིར 0;
}

static errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             ཧྲིལ count, void *buf)
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

  གལ་ཏེ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    ཕྱིར EFAULT;
  }

  ཕྱིར 0;
}

static errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               ཧྲིལ count, const void *buf)
{
  PARTITION     *part;
  size_t        size;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_data;

  གལ་ཏེ(count == 1)
    size = (size_t)channel->block_size;
  དེ་མིན
  {
    གལ་ཏེ (count < 0)
      size = (size_t)-count;
    དེ་མིན
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

  གལ་ཏེ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    ཕྱིར EFAULT;
  }

  ཕྱིར 0;
}

#ifdef __TURBOC__
#pragma argsused
#endif
static errcode_t dos_flush(io_channel channel)
{
  /*
   * No buffers, no flush...
   */
  ཕྱིར 0;
}
