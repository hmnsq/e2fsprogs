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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <bios.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <ctype.h>
#અન્તર્ભાવ <io.h>
#ifdef HAVE_ERRNO_H
#અન્તર્ભાવ <errno.h>
#endif

#અન્તર્ભાવ <ext2fs/ext2_types.h>
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "dosio.h"
#અન્તર્ભાવ "et/com_err.h"
#અન્તર્ભાવ "ext2_err.h"
#અન્તર્ભાવ "ext2fs/io.h"

/*
 * Some helper macros
 */
#define LINUX_EXT2FS       0x83
#define LINUX_SWAP         0x82
#define WRITE_ERR(સંદેશ_2) લિખ(2, સંદેશ_2, strlen(સંદેશ_2))
#define WRITE_ERR_S(સંદેશ_2) લિખ(2, સંદેશ_2, કદ(સંદેશ_2))

/*
 * Exported variables
 */
unsigned long        _dio_error;
unsigned long        _dio_hw_error;

/*
 * Array of all opened partitions
 */
સ્થિર PARTITION        **partitions = ખાલી;
સ્થિર unsigned short   npart = 0; /* Number of mapped partitions */
સ્થિર PARTITION        *active = ખાલી;

/*
 * I/O Manager routine prototypes
 */
સ્થિર errcode_t dos_open(અચલ અક્ષર *dev, પૂર્ણાંક flags, io_channel *channel);
સ્થિર errcode_t dos_close(io_channel channel);
સ્થિર errcode_t dos_set_blksize(io_channel channel, પૂર્ણાંક blksize);
સ્થિર errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             પૂર્ણાંક ગણતરી, શૂન્ય *buf);
સ્થિર errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               પૂર્ણાંક ગણતરી, અચલ શૂન્ય *buf);
સ્થિર errcode_t dos_flush(io_channel channel);

સ્થિર સંરચના struct_io_manager struct_dos_manager = {
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

#define EXT2_CHECK_MAGIC(સંરચના, code) \
          જો ((સંરચના)->magic != (code)) પાછા (code)

/*
 * Calculates a CHS address of a sector from its LBA
 * offset for the given partition.
 */
સ્થિર શૂન્ય lba2chs(unsigned long lba_addr, CHS *chs, PARTITION *part)
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
સ્થિર PTABLE_ENTRY *scan_partition_table(PTABLE_ENTRY *pentry,
                                          unsigned short phys,
                                          unsigned અક્ષર *pno)
{
  unsigned        i;

  જો(*pno != 0xFF && *pno >= 5)
     પાછા ખાલી; /* We don't support extended partitions for now */

  જો(*pno != 0xFF)
  {
    જો(pentry[*pno].type == LINUX_EXT2FS)
      પાછા &pentry[*pno];
    નહીંતર
    {
      જો(!pentry[*pno].type)
        *pno = 0xFE;
      નહીંતર જો(pentry[*pno].type == LINUX_SWAP)
        *pno = 0xFD;
      પાછા ખાલી;
    }
  }

  માટે(i = 0; i < 4; i++)
    જો(pentry[i].type == LINUX_EXT2FS)
    {
      *pno = i;
      પાછા &pentry[i];
    }

  પાછા ખાલી;
}

/*
 * Allocate libext2fs structures associated with I/O manager
 */
સ્થિર io_channel alloc_io_channel(PARTITION *part)
{
  io_channel     ioch;

  ioch = (io_channel)malloc(કદ(સંરચના struct_io_channel));
  જો (!ioch)
	  પાછા ખાલી;
  memset(ioch, 0, કદ(સંરચના struct_io_channel));
  ioch->magic = EXT2_ET_MAGIC_IO_CHANNEL;
  ioch->manager = dos_io_manager;
  ioch->name = (અક્ષર *)malloc(strlen(part->dev)+1);
  જો (!ioch->name) {
	  મુક્ત(ioch);
	  પાછા ખાલી;
  }
  strcpy(ioch->name, part->dev);
  ioch->private_data = part;
  ioch->block_size = 1024; /* The smallest ext2fs block size */
  ioch->read_error = 0;
  ioch->write_error = 0;

  પાછા ioch;
}

#ifdef __TURBOC__
#pragma argsused
#endif
/*
 * Open the 'name' partition, initialize all information structures
 * we need to keep and create libext2fs I/O manager.
 */
સ્થિર errcode_t dos_open(અચલ અક્ષર *dev, પૂર્ણાંક flags, io_channel *channel)
{
  unsigned અક્ષર  *tmp, sec[512];
  PARTITION      *part;
  PTABLE_ENTRY   *pent;
  PARTITION        **newparts;

  જો(!dev)
  {
    _dio_error = ERR_BADDEV;
    પાછા EXT2_ET_BAD_DEVICE_NAME;
  }

  /*
   * First check whether the dev name is OK
   */
  tmp = (unsigned અક્ષર*)strrchr(dev, '/');
  જો(!tmp)
  {
    _dio_error = ERR_BADDEV;
    પાછા EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp = 0;
  જો(strcmp(dev, "/dev"))
  {
    _dio_error = ERR_BADDEV;
    પાછા EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp++ = '/';

  /*
   * Check whether the partition data is already in cache
   */

  part = (PARTITION*)malloc(કદ(PARTITION));
  જો (!part)
	  પાછા ENOMEM;
  {
    પૂર્ણાંક   i = 0;

    માટે(;i < npart; i++)
      જો(!strcmp(partitions[i]->dev, dev))
      {
        /* Found it! Make it the active one */
        active = partitions[i];
        *channel = alloc_io_channel(active);
	જો (!*channel)
		પાછા ENOMEM;
        પાછા 0;
      }
  }

  /*
   * Drive number & optionally partn number
   */
  પસંદગી(tmp[0])
  {
    સ્થિતિ 'h':
    સ્થિતિ 's':
      part->phys = 0x80;
      part->phys += toupper(tmp[2]) - 'A';
      /*
       * Do we have the partition number?
       */
      જો(tmp[3])
        part->pno = isdigit((પૂર્ણાંક)tmp[3]) ? tmp[3] - '0' - 1: 0;
      નહીંતર
        part->pno = 0xFF;
      તોડો;

    સ્થિતિ 'f':
      જો(tmp[2])
        part->phys = isdigit((પૂર્ણાંક)tmp[2]) ? tmp[2] - '0' : 0;
      નહીંતર
        part->phys = 0x00; /* We'll assume /dev/fd0 */
      તોડો;

    મૂળભૂત:
      _dio_error = ERR_BADDEV;
      પાછા ENODEV;
  }

  જો(part->phys < 0x80)
  {
     /* We don't support floppies for now */
     _dio_error = ERR_NOTSUPP;
     પાછા EINVAL;
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

  જો(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    મુક્ત(part->dev);
    મુક્ત(part);
    પાછા EFAULT;
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

  જો(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    મુક્ત(part->dev);
    મુક્ત(part);
    પાછા EFAULT;
  }

  pent = (PTABLE_ENTRY*)&sec[0x1BE];
  pent = scan_partition_table(pent, part->phys, &part->pno);

  જો(!pent)
  {
    _dio_error = part->pno == 0xFE ? ERR_EMPTYPART :
                 part->pno == 0xFD ? ERR_LINUXSWAP : ERR_NOTEXT2FS;
    મુક્ત(part->dev);
    મુક્ત(part);
    પાછા ENODEV;
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
  newparts = (PARTITION**)realloc(partitions, કદ(PARTITION) * npart);
  જો (!newparts) {
	  મુક્ત(part);
	  પાછા ENOMEM;
  }
  partitions = newparts;
  partitions[npart++] = active = part;

  /*
   * Now alloc all libe2fs structures
   */
  *channel = alloc_io_channel(active);
  જો (!*channel)
	  પાછા ENOMEM;

  પાછા 0;
}

સ્થિર errcode_t dos_close(io_channel channel)
{
	મુક્ત(channel->name);
	મુક્ત(channel);

	પાછા 0;
}

સ્થિર errcode_t dos_set_blksize(io_channel channel, પૂર્ણાંક blksize)
{
  channel->block_size = blksize;

  પાછા 0;
}

સ્થિર errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             પૂર્ણાંક ગણતરી, શૂન્ય *buf)
{
  PARTITION     *part;
  size_t        size;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_data;

  size = (size_t)((ગણતરી < 0) ? -ગણતરી : ગણતરી * channel->block_size);
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

  જો(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    પાછા EFAULT;
  }

  પાછા 0;
}

સ્થિર errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               પૂર્ણાંક ગણતરી, અચલ શૂન્ય *buf)
{
  PARTITION     *part;
  size_t        size;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_data;

  જો(ગણતરી == 1)
    size = (size_t)channel->block_size;
  નહીંતર
  {
    જો (ગણતરી < 0)
      size = (size_t)-ગણતરી;
    નહીંતર
      size = (size_t)(ગણતરી * channel->block_size);
  }

  loc = (ext2_loff_t)block * channel->block_size;
  lba2chs(loc, &chs, part);
  _dio_hw_error = biosdisk(DISK_WRITE,
                           part->phys,
                           chs.head,
                           chs.cyl,
                           chs.sector,
                           size < 512 ? 1 : size/512,
                           (શૂન્ય*)buf);

  જો(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    પાછા EFAULT;
  }

  પાછા 0;
}

#ifdef __TURBOC__
#pragma argsused
#endif
સ્થિર errcode_t dos_flush(io_channel channel)
{
  /*
   * No buffers, no flush...
   */
  પાછા 0;
}
