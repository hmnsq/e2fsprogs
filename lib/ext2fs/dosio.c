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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <bios.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <ctype.h>
#அந்தர்பாவ <io.h>
#ifdef HAVE_ERRNO_H
#அந்தர்பாவ <errno.h>
#endif

#அந்தர்பாவ <ext2fs/ext2_types.h>
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "dosio.h"
#அந்தர்பாவ "et/com_err.h"
#அந்தர்பாவ "ext2_err.h"
#அந்தர்பாவ "ext2fs/io.h"

/*
 * Some helper macros
 */
#define LINUX_EXT2FS       0x83
#define LINUX_SWAP         0x82
#define WRITE_ERR(செய்தி_2) லிக(2, செய்தி_2, strlen(செய்தி_2))
#define WRITE_ERR_S(செய்தி_2) லிக(2, செய்தி_2, அளவு(செய்தி_2))

/*
 * Exported variables
 */
unsigned long        _dio_error;
unsigned long        _dio_hw_error;

/*
 * Array of all opened partitions
 */
நிலையான PARTITION        **partitions = வெற்று;
நிலையான unsigned short   npart = 0; /* Number of mapped partitions */
நிலையான PARTITION        *active = வெற்று;

/*
 * I/O Manager routine prototypes
 */
நிலையான errcode_t dos_open(மாறிலி எழுத்து *dev, முழு flags, io_channel *channel);
நிலையான errcode_t dos_close(io_channel channel);
நிலையான errcode_t dos_set_blksize(io_channel channel, முழு blksize);
நிலையான errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             முழு மொத்தம், வெறுமை *buf);
நிலையான errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               முழு மொத்தம், மாறிலி வெறுமை *buf);
நிலையான errcode_t dos_flush(io_channel channel);

நிலையான கட்டமைப்பு struct_io_manager struct_dos_manager = {
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

#define EXT2_CHECK_MAGIC(கட்டமைப்பு, code) \
          எனில் ((கட்டமைப்பு)->magic != (code)) திரும்பு (code)

/*
 * Calculates a CHS address of a sector from its LBA
 * offset for the given partition.
 */
நிலையான வெறுமை lba2chs(unsigned long lba_addr, CHS *chs, PARTITION *part)
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
நிலையான PTABLE_ENTRY *scan_partition_table(PTABLE_ENTRY *pentry,
                                          unsigned short phys,
                                          unsigned எழுத்து *pno)
{
  unsigned        i;

  எனில்(*pno != 0xFF && *pno >= 5)
     திரும்பு வெற்று; /* We don't support extended partitions for now */

  எனில்(*pno != 0xFF)
  {
    எனில்(pentry[*pno].type == LINUX_EXT2FS)
      திரும்பு &pentry[*pno];
    இல்லையெனில்
    {
      எனில்(!pentry[*pno].type)
        *pno = 0xFE;
      இல்லையெனில் எனில்(pentry[*pno].type == LINUX_SWAP)
        *pno = 0xFD;
      திரும்பு வெற்று;
    }
  }

  க்கு(i = 0; i < 4; i++)
    எனில்(pentry[i].type == LINUX_EXT2FS)
    {
      *pno = i;
      திரும்பு &pentry[i];
    }

  திரும்பு வெற்று;
}

/*
 * Allocate libext2fs structures associated with I/O manager
 */
நிலையான io_channel alloc_io_channel(PARTITION *part)
{
  io_channel     ioch;

  ioch = (io_channel)malloc(அளவு(கட்டமைப்பு struct_io_channel));
  எனில் (!ioch)
	  திரும்பு வெற்று;
  memset(ioch, 0, அளவு(கட்டமைப்பு struct_io_channel));
  ioch->magic = EXT2_ET_MAGIC_IO_CHANNEL;
  ioch->manager = dos_io_manager;
  ioch->name = (எழுத்து *)malloc(strlen(part->dev)+1);
  எனில் (!ioch->name) {
	  விடுவி(ioch);
	  திரும்பு வெற்று;
  }
  strcpy(ioch->name, part->dev);
  ioch->private_data = part;
  ioch->block_size = 1024; /* The smallest ext2fs block size */
  ioch->read_error = 0;
  ioch->write_error = 0;

  திரும்பு ioch;
}

#ifdef __TURBOC__
#pragma argsused
#endif
/*
 * Open the 'name' partition, initialize all information structures
 * we need to keep and create libext2fs I/O manager.
 */
நிலையான errcode_t dos_open(மாறிலி எழுத்து *dev, முழு flags, io_channel *channel)
{
  unsigned எழுத்து  *tmp, sec[512];
  PARTITION      *part;
  PTABLE_ENTRY   *pent;
  PARTITION        **newparts;

  எனில்(!dev)
  {
    _dio_error = ERR_BADDEV;
    திரும்பு EXT2_ET_BAD_DEVICE_NAME;
  }

  /*
   * First check whether the dev name is OK
   */
  tmp = (unsigned எழுத்து*)strrchr(dev, '/');
  எனில்(!tmp)
  {
    _dio_error = ERR_BADDEV;
    திரும்பு EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp = 0;
  எனில்(strcmp(dev, "/dev"))
  {
    _dio_error = ERR_BADDEV;
    திரும்பு EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp++ = '/';

  /*
   * Check whether the partition data is already in cache
   */

  part = (PARTITION*)malloc(அளவு(PARTITION));
  எனில் (!part)
	  திரும்பு ENOMEM;
  {
    முழு   i = 0;

    க்கு(;i < npart; i++)
      எனில்(!strcmp(partitions[i]->dev, dev))
      {
        /* Found it! Make it the active one */
        active = partitions[i];
        *channel = alloc_io_channel(active);
	எனில் (!*channel)
		திரும்பு ENOMEM;
        திரும்பு 0;
      }
  }

  /*
   * Drive number & optionally partn number
   */
  தேர்வு(tmp[0])
  {
    நிலை 'h':
    நிலை 's':
      part->phys = 0x80;
      part->phys += toupper(tmp[2]) - 'A';
      /*
       * Do we have the partition number?
       */
      எனில்(tmp[3])
        part->pno = isdigit((முழு)tmp[3]) ? tmp[3] - '0' - 1: 0;
      இல்லையெனில்
        part->pno = 0xFF;
      நிறுத்து;

    நிலை 'f':
      எனில்(tmp[2])
        part->phys = isdigit((முழு)tmp[2]) ? tmp[2] - '0' : 0;
      இல்லையெனில்
        part->phys = 0x00; /* We'll assume /dev/fd0 */
      நிறுத்து;

    இயல்பு:
      _dio_error = ERR_BADDEV;
      திரும்பு ENODEV;
  }

  எனில்(part->phys < 0x80)
  {
     /* We don't support floppies for now */
     _dio_error = ERR_NOTSUPP;
     திரும்பு EINVAL;
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

  எனில்(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    விடுவி(part->dev);
    விடுவி(part);
    திரும்பு EFAULT;
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

  எனில்(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    விடுவி(part->dev);
    விடுவி(part);
    திரும்பு EFAULT;
  }

  pent = (PTABLE_ENTRY*)&sec[0x1BE];
  pent = scan_partition_table(pent, part->phys, &part->pno);

  எனில்(!pent)
  {
    _dio_error = part->pno == 0xFE ? ERR_EMPTYPART :
                 part->pno == 0xFD ? ERR_LINUXSWAP : ERR_NOTEXT2FS;
    விடுவி(part->dev);
    விடுவி(part);
    திரும்பு ENODEV;
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
  newparts = (PARTITION**)realloc(partitions, அளவு(PARTITION) * npart);
  எனில் (!newparts) {
	  விடுவி(part);
	  திரும்பு ENOMEM;
  }
  partitions = newparts;
  partitions[npart++] = active = part;

  /*
   * Now alloc all libe2fs structures
   */
  *channel = alloc_io_channel(active);
  எனில் (!*channel)
	  திரும்பு ENOMEM;

  திரும்பு 0;
}

நிலையான errcode_t dos_close(io_channel channel)
{
	விடுவி(channel->name);
	விடுவி(channel);

	திரும்பு 0;
}

நிலையான errcode_t dos_set_blksize(io_channel channel, முழு blksize)
{
  channel->block_size = blksize;

  திரும்பு 0;
}

நிலையான errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             முழு மொத்தம், வெறுமை *buf)
{
  PARTITION     *part;
  size_t        size;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_data;

  size = (size_t)((மொத்தம் < 0) ? -மொத்தம் : மொத்தம் * channel->block_size);
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

  எனில்(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    திரும்பு EFAULT;
  }

  திரும்பு 0;
}

நிலையான errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               முழு மொத்தம், மாறிலி வெறுமை *buf)
{
  PARTITION     *part;
  size_t        size;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_data;

  எனில்(மொத்தம் == 1)
    size = (size_t)channel->block_size;
  இல்லையெனில்
  {
    எனில் (மொத்தம் < 0)
      size = (size_t)-மொத்தம்;
    இல்லையெனில்
      size = (size_t)(மொத்தம் * channel->block_size);
  }

  loc = (ext2_loff_t)block * channel->block_size;
  lba2chs(loc, &chs, part);
  _dio_hw_error = biosdisk(DISK_WRITE,
                           part->phys,
                           chs.head,
                           chs.cyl,
                           chs.sector,
                           size < 512 ? 1 : size/512,
                           (வெறுமை*)buf);

  எனில்(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    திரும்பு EFAULT;
  }

  திரும்பு 0;
}

#ifdef __TURBOC__
#pragma argsused
#endif
நிலையான errcode_t dos_flush(io_channel channel)
{
  /*
   * No buffers, no flush...
   */
  திரும்பு 0;
}
