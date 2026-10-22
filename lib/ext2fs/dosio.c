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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <bios.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <ctype.h>
#അന്തര്ഭാവ <io.h>
#ifdef HAVE_ERRNO_H
#അന്തര്ഭാവ <errno.h>
#endif

#അന്തര്ഭാവ <ext2fs/ext2_types.h>
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "dosio.h"
#അന്തര്ഭാവ "et/com_err.h"
#അന്തര്ഭാവ "ext2_err.h"
#അന്തര്ഭാവ "ext2fs/io.h"

/*
 * Some helper macros
 */
#define LINUX_EXT2FS       0x83
#define LINUX_SWAP         0x82
#define WRITE_ERR(സന്ദേശം_2) ലിഖ(2, സന്ദേശം_2, strlen(സന്ദേശം_2))
#define WRITE_ERR_S(സന്ദേശം_2) ലിഖ(2, സന്ദേശം_2, വലുപ്പം(സന്ദേശം_2))

/*
 * Exported variables
 */
unsigned long        _dio_error;
unsigned long        _dio_hw_error;

/*
 * Array of all opened partitions
 */
സ്ഥിതം PARTITION        **partitions = ശൂന്യ;
സ്ഥിതം unsigned short   npart = 0; /* Number of mapped partitions */
സ്ഥിതം PARTITION        *active = ശൂന്യ;

/*
 * I/O Manager routine prototypes
 */
സ്ഥിതം errcode_t dos_open(സ്ഥിരം അക്ഷരം *dev, പൂർണ്ണസംഖ്യ flags, io_channel *channel);
സ്ഥിതം errcode_t dos_close(io_channel channel);
സ്ഥിതം errcode_t dos_set_blksize(io_channel channel, പൂർണ്ണസംഖ്യ blksize);
സ്ഥിതം errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             പൂർണ്ണസംഖ്യ എണ്ണം, ശൂന്യം *buf);
സ്ഥിതം errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *buf);
സ്ഥിതം errcode_t dos_flush(io_channel channel);

സ്ഥിതം ഘടന struct_io_manager struct_dos_manager = {
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

#define EXT2_CHECK_MAGIC(ഘടന, code) \
          എങ്കിൽ ((ഘടന)->magic != (code)) തിരികെ (code)

/*
 * Calculates a CHS address of a sector from its LBA
 * offset for the given partition.
 */
സ്ഥിതം ശൂന്യം lba2chs(unsigned long lba_addr, CHS *chs, PARTITION *part)
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
സ്ഥിതം PTABLE_ENTRY *scan_partition_table(PTABLE_ENTRY *pentry,
                                          unsigned short phys,
                                          unsigned അക്ഷരം *pno)
{
  unsigned        i;

  എങ്കിൽ(*pno != 0xFF && *pno >= 5)
     തിരികെ ശൂന്യ; /* We don't support extended partitions for now */

  എങ്കിൽ(*pno != 0xFF)
  {
    എങ്കിൽ(pentry[*pno].type == LINUX_EXT2FS)
      തിരികെ &pentry[*pno];
    അല്ലെങ്കിൽ
    {
      എങ്കിൽ(!pentry[*pno].type)
        *pno = 0xFE;
      അല്ലെങ്കിൽ എങ്കിൽ(pentry[*pno].type == LINUX_SWAP)
        *pno = 0xFD;
      തിരികെ ശൂന്യ;
    }
  }

  വേണ്ടി(i = 0; i < 4; i++)
    എങ്കിൽ(pentry[i].type == LINUX_EXT2FS)
    {
      *pno = i;
      തിരികെ &pentry[i];
    }

  തിരികെ ശൂന്യ;
}

/*
 * Allocate libext2fs structures associated with I/O manager
 */
സ്ഥിതം io_channel alloc_io_channel(PARTITION *part)
{
  io_channel     ioch;

  ioch = (io_channel)malloc(വലുപ്പം(ഘടന struct_io_channel));
  എങ്കിൽ (!ioch)
	  തിരികെ ശൂന്യ;
  memset(ioch, 0, വലുപ്പം(ഘടന struct_io_channel));
  ioch->magic = EXT2_ET_MAGIC_IO_CHANNEL;
  ioch->manager = dos_io_manager;
  ioch->name = (അക്ഷരം *)malloc(strlen(part->dev)+1);
  എങ്കിൽ (!ioch->name) {
	  സ്വതന്ത്രം(ioch);
	  തിരികെ ശൂന്യ;
  }
  strcpy(ioch->name, part->dev);
  ioch->private_data = part;
  ioch->block_size = 1024; /* The smallest ext2fs block size */
  ioch->read_error = 0;
  ioch->write_error = 0;

  തിരികെ ioch;
}

#ifdef __TURBOC__
#pragma argsused
#endif
/*
 * Open the 'name' partition, initialize all information structures
 * we need to keep and create libext2fs I/O manager.
 */
സ്ഥിതം errcode_t dos_open(സ്ഥിരം അക്ഷരം *dev, പൂർണ്ണസംഖ്യ flags, io_channel *channel)
{
  unsigned അക്ഷരം  *tmp, sec[512];
  PARTITION      *part;
  PTABLE_ENTRY   *pent;
  PARTITION        **newparts;

  എങ്കിൽ(!dev)
  {
    _dio_error = ERR_BADDEV;
    തിരികെ EXT2_ET_BAD_DEVICE_NAME;
  }

  /*
   * First check whether the dev name is OK
   */
  tmp = (unsigned അക്ഷരം*)strrchr(dev, '/');
  എങ്കിൽ(!tmp)
  {
    _dio_error = ERR_BADDEV;
    തിരികെ EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp = 0;
  എങ്കിൽ(strcmp(dev, "/dev"))
  {
    _dio_error = ERR_BADDEV;
    തിരികെ EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp++ = '/';

  /*
   * Check whether the partition data is already in cache
   */

  part = (PARTITION*)malloc(വലുപ്പം(PARTITION));
  എങ്കിൽ (!part)
	  തിരികെ ENOMEM;
  {
    പൂർണ്ണസംഖ്യ   i = 0;

    വേണ്ടി(;i < npart; i++)
      എങ്കിൽ(!strcmp(partitions[i]->dev, dev))
      {
        /* Found it! Make it the active one */
        active = partitions[i];
        *channel = alloc_io_channel(active);
	എങ്കിൽ (!*channel)
		തിരികെ ENOMEM;
        തിരികെ 0;
      }
  }

  /*
   * Drive number & optionally partn number
   */
  തിരഞ്ഞെടുപ്പ്(tmp[0])
  {
    അവസ്ഥ 'h':
    അവസ്ഥ 's':
      part->phys = 0x80;
      part->phys += toupper(tmp[2]) - 'A';
      /*
       * Do we have the partition number?
       */
      എങ്കിൽ(tmp[3])
        part->pno = isdigit((പൂർണ്ണസംഖ്യ)tmp[3]) ? tmp[3] - '0' - 1: 0;
      അല്ലെങ്കിൽ
        part->pno = 0xFF;
      നിർത്തുക;

    അവസ്ഥ 'f':
      എങ്കിൽ(tmp[2])
        part->phys = isdigit((പൂർണ്ണസംഖ്യ)tmp[2]) ? tmp[2] - '0' : 0;
      അല്ലെങ്കിൽ
        part->phys = 0x00; /* We'll assume /dev/fd0 */
      നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
      _dio_error = ERR_BADDEV;
      തിരികെ ENODEV;
  }

  എങ്കിൽ(part->phys < 0x80)
  {
     /* We don't support floppies for now */
     _dio_error = ERR_NOTSUPP;
     തിരികെ EINVAL;
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

  എങ്കിൽ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    സ്വതന്ത്രം(part->dev);
    സ്വതന്ത്രം(part);
    തിരികെ EFAULT;
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

  എങ്കിൽ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    സ്വതന്ത്രം(part->dev);
    സ്വതന്ത്രം(part);
    തിരികെ EFAULT;
  }

  pent = (PTABLE_ENTRY*)&sec[0x1BE];
  pent = scan_partition_table(pent, part->phys, &part->pno);

  എങ്കിൽ(!pent)
  {
    _dio_error = part->pno == 0xFE ? ERR_EMPTYPART :
                 part->pno == 0xFD ? ERR_LINUXSWAP : ERR_NOTEXT2FS;
    സ്വതന്ത്രം(part->dev);
    സ്വതന്ത്രം(part);
    തിരികെ ENODEV;
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
  newparts = (PARTITION**)realloc(partitions, വലുപ്പം(PARTITION) * npart);
  എങ്കിൽ (!newparts) {
	  സ്വതന്ത്രം(part);
	  തിരികെ ENOMEM;
  }
  partitions = newparts;
  partitions[npart++] = active = part;

  /*
   * Now alloc all libe2fs structures
   */
  *channel = alloc_io_channel(active);
  എങ്കിൽ (!*channel)
	  തിരികെ ENOMEM;

  തിരികെ 0;
}

സ്ഥിതം errcode_t dos_close(io_channel channel)
{
	സ്വതന്ത്രം(channel->name);
	സ്വതന്ത്രം(channel);

	തിരികെ 0;
}

സ്ഥിതം errcode_t dos_set_blksize(io_channel channel, പൂർണ്ണസംഖ്യ blksize)
{
  channel->block_size = blksize;

  തിരികെ 0;
}

സ്ഥിതം errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             പൂർണ്ണസംഖ്യ എണ്ണം, ശൂന്യം *buf)
{
  PARTITION     *part;
  size_t        size;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_data;

  size = (size_t)((എണ്ണം < 0) ? -എണ്ണം : എണ്ണം * channel->block_size);
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

  എങ്കിൽ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    തിരികെ EFAULT;
  }

  തിരികെ 0;
}

സ്ഥിതം errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *buf)
{
  PARTITION     *part;
  size_t        size;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_data;

  എങ്കിൽ(എണ്ണം == 1)
    size = (size_t)channel->block_size;
  അല്ലെങ്കിൽ
  {
    എങ്കിൽ (എണ്ണം < 0)
      size = (size_t)-എണ്ണം;
    അല്ലെങ്കിൽ
      size = (size_t)(എണ്ണം * channel->block_size);
  }

  loc = (ext2_loff_t)block * channel->block_size;
  lba2chs(loc, &chs, part);
  _dio_hw_error = biosdisk(DISK_WRITE,
                           part->phys,
                           chs.head,
                           chs.cyl,
                           chs.sector,
                           size < 512 ? 1 : size/512,
                           (ശൂന്യം*)buf);

  എങ്കിൽ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    തിരികെ EFAULT;
  }

  തിരികെ 0;
}

#ifdef __TURBOC__
#pragma argsused
#endif
സ്ഥിതം errcode_t dos_flush(io_channel channel)
{
  /*
   * No buffers, no flush...
   */
  തിരികെ 0;
}
