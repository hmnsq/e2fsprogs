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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <bios.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <ctype.h>
#ಅನ್ತರ್ಭಾವ <io.h>
#ifdef HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#endif

#ಅನ್ತರ್ಭಾವ <ext2fs/ext2_types.h>
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "dosio.h"
#ಅನ್ತರ್ಭಾವ "et/com_err.h"
#ಅನ್ತರ್ಭಾವ "ext2_err.h"
#ಅನ್ತರ್ಭಾವ "ext2fs/io.h"

/*
 * Some helper macros
 */
#define LINUX_EXT2FS       0x83
#define LINUX_SWAP         0x82
#define WRITE_ERR(ಸಂದೇಶ_2) ಲಿಖ(2, ಸಂದೇಶ_2, strlen(ಸಂದೇಶ_2))
#define WRITE_ERR_S(ಸಂದೇಶ_2) ಲಿಖ(2, ಸಂದೇಶ_2, ಪರಿಮಾಣ(ಸಂದೇಶ_2))

/*
 * Exported variables
 */
unsigned long        _dio_error;
unsigned long        _dio_hw_error;

/*
 * Array of all opened partitions
 */
ಸ್ಥಿರ PARTITION        **partitions = ರಿಕ್ತ;
ಸ್ಥಿರ unsigned short   npart = 0; /* Number of mapped partitions */
ಸ್ಥಿರ PARTITION        *active = ರಿಕ್ತ;

/*
 * I/O Manager routine prototypes
 */
ಸ್ಥಿರ errcode_t dos_open(ಧ್ರುವ ವರ್ಣ *dev, ಪೂರ್ಣಾಙ್ಕ flags, io_channel *channel);
ಸ್ಥಿರ errcode_t dos_close(io_channel channel);
ಸ್ಥಿರ errcode_t dos_set_blksize(io_channel channel, ಪೂರ್ಣಾಙ್ಕ blksize);
ಸ್ಥಿರ errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಶೂನ್ಯ *buf);
ಸ್ಥಿರ errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಧ್ರುವ ಶೂನ್ಯ *buf);
ಸ್ಥಿರ errcode_t dos_flush(io_channel channel);

ಸ್ಥಿರ ಸಂರಚನಾ struct_io_manager struct_dos_manager = {
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

#define EXT2_CHECK_MAGIC(ಸಂರಚನಾ, code) \
          ಯದಿ ((ಸಂರಚನಾ)->magic != (code)) ಪ್ರತ್ಯಾಗಮ (code)

/*
 * Calculates a CHS address of a sector from its LBA
 * offset for the given partition.
 */
ಸ್ಥಿರ ಶೂನ್ಯ lba2chs(unsigned long lba_addr, CHS *chs, PARTITION *part)
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
ಸ್ಥಿರ PTABLE_ENTRY *scan_partition_table(PTABLE_ENTRY *pentry,
                                          unsigned short phys,
                                          unsigned ವರ್ಣ *pno)
{
  unsigned        i;

  ಯದಿ(*pno != 0xFF && *pno >= 5)
     ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ; /* We don't support extended partitions for now */

  ಯದಿ(*pno != 0xFF)
  {
    ಯದಿ(pentry[*pno].type == LINUX_EXT2FS)
      ಪ್ರತ್ಯಾಗಮ &pentry[*pno];
    ಅನ್ಯಥಾ
    {
      ಯದಿ(!pentry[*pno].type)
        *pno = 0xFE;
      ಅನ್ಯಥಾ ಯದಿ(pentry[*pno].type == LINUX_SWAP)
        *pno = 0xFD;
      ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }
  }

  ಪ್ರತಿ(i = 0; i < 4; i++)
    ಯದಿ(pentry[i].type == LINUX_EXT2FS)
    {
      *pno = i;
      ಪ್ರತ್ಯಾಗಮ &pentry[i];
    }

  ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

/*
 * Allocate libext2fs structures associated with I/O manager
 */
ಸ್ಥಿರ io_channel alloc_io_channel(PARTITION *part)
{
  io_channel     ioch;

  ioch = (io_channel)malloc(ಪರಿಮಾಣ(ಸಂರಚನಾ struct_io_channel));
  ಯದಿ (!ioch)
	  ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
  memset(ioch, 0, ಪರಿಮಾಣ(ಸಂರಚನಾ struct_io_channel));
  ioch->magic = EXT2_ET_MAGIC_IO_CHANNEL;
  ioch->manager = dos_io_manager;
  ioch->name = (ವರ್ಣ *)malloc(strlen(part->dev)+1);
  ಯದಿ (!ioch->name) {
	  ಮುಕ್ತ(ioch);
	  ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
  }
  strcpy(ioch->name, part->dev);
  ioch->private_data = part;
  ioch->block_size = 1024; /* The smallest ext2fs block size */
  ioch->read_error = 0;
  ioch->write_error = 0;

  ಪ್ರತ್ಯಾಗಮ ioch;
}

#ifdef __TURBOC__
#pragma argsused
#endif
/*
 * Open the 'name' partition, initialize all information structures
 * we need to keep and create libext2fs I/O manager.
 */
ಸ್ಥಿರ errcode_t dos_open(ಧ್ರುವ ವರ್ಣ *dev, ಪೂರ್ಣಾಙ್ಕ flags, io_channel *channel)
{
  unsigned ವರ್ಣ  *tmp, sec[512];
  PARTITION      *part;
  PTABLE_ENTRY   *pent;
  PARTITION        **newparts;

  ಯದಿ(!dev)
  {
    _dio_error = ERR_BADDEV;
    ಪ್ರತ್ಯಾಗಮ EXT2_ET_BAD_DEVICE_NAME;
  }

  /*
   * First check whether the dev name is OK
   */
  tmp = (unsigned ವರ್ಣ*)strrchr(dev, '/');
  ಯದಿ(!tmp)
  {
    _dio_error = ERR_BADDEV;
    ಪ್ರತ್ಯಾಗಮ EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp = 0;
  ಯದಿ(strcmp(dev, "/dev"))
  {
    _dio_error = ERR_BADDEV;
    ಪ್ರತ್ಯಾಗಮ EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp++ = '/';

  /*
   * Check whether the partition data is already in cache
   */

  part = (PARTITION*)malloc(ಪರಿಮಾಣ(PARTITION));
  ಯದಿ (!part)
	  ಪ್ರತ್ಯಾಗಮ ENOMEM;
  {
    ಪೂರ್ಣಾಙ್ಕ   i = 0;

    ಪ್ರತಿ(;i < npart; i++)
      ಯದಿ(!strcmp(partitions[i]->dev, dev))
      {
        /* Found it! Make it the active one */
        active = partitions[i];
        *channel = alloc_io_channel(active);
	ಯದಿ (!*channel)
		ಪ್ರತ್ಯಾಗಮ ENOMEM;
        ಪ್ರತ್ಯಾಗಮ 0;
      }
  }

  /*
   * Drive number & optionally partn number
   */
  ವಿಕಲ್ಪ(tmp[0])
  {
    ಪಕ್ಷ 'h':
    ಪಕ್ಷ 's':
      part->phys = 0x80;
      part->phys += toupper(tmp[2]) - 'A';
      /*
       * Do we have the partition number?
       */
      ಯದಿ(tmp[3])
        part->pno = isdigit((ಪೂರ್ಣಾಙ್ಕ)tmp[3]) ? tmp[3] - '0' - 1: 0;
      ಅನ್ಯಥಾ
        part->pno = 0xFF;
      ಭಙ್ಗ;

    ಪಕ್ಷ 'f':
      ಯದಿ(tmp[2])
        part->phys = isdigit((ಪೂರ್ಣಾಙ್ಕ)tmp[2]) ? tmp[2] - '0' : 0;
      ಅನ್ಯಥಾ
        part->phys = 0x00; /* We'll assume /dev/fd0 */
      ಭಙ್ಗ;

    ಸ್ವಭಾವ:
      _dio_error = ERR_BADDEV;
      ಪ್ರತ್ಯಾಗಮ ENODEV;
  }

  ಯದಿ(part->phys < 0x80)
  {
     /* We don't support floppies for now */
     _dio_error = ERR_NOTSUPP;
     ಪ್ರತ್ಯಾಗಮ EINVAL;
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

  ಯದಿ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    ಮುಕ್ತ(part->dev);
    ಮುಕ್ತ(part);
    ಪ್ರತ್ಯಾಗಮ EFAULT;
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

  ಯದಿ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    ಮುಕ್ತ(part->dev);
    ಮುಕ್ತ(part);
    ಪ್ರತ್ಯಾಗಮ EFAULT;
  }

  pent = (PTABLE_ENTRY*)&sec[0x1BE];
  pent = scan_partition_table(pent, part->phys, &part->pno);

  ಯದಿ(!pent)
  {
    _dio_error = part->pno == 0xFE ? ERR_EMPTYPART :
                 part->pno == 0xFD ? ERR_LINUXSWAP : ERR_NOTEXT2FS;
    ಮುಕ್ತ(part->dev);
    ಮುಕ್ತ(part);
    ಪ್ರತ್ಯಾಗಮ ENODEV;
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
  newparts = (PARTITION**)realloc(partitions, ಪರಿಮಾಣ(PARTITION) * npart);
  ಯದಿ (!newparts) {
	  ಮುಕ್ತ(part);
	  ಪ್ರತ್ಯಾಗಮ ENOMEM;
  }
  partitions = newparts;
  partitions[npart++] = active = part;

  /*
   * Now alloc all libe2fs structures
   */
  *channel = alloc_io_channel(active);
  ಯದಿ (!*channel)
	  ಪ್ರತ್ಯಾಗಮ ENOMEM;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t dos_close(io_channel channel)
{
	ಮುಕ್ತ(channel->name);
	ಮುಕ್ತ(channel);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t dos_set_blksize(io_channel channel, ಪೂರ್ಣಾಙ್ಕ blksize)
{
  channel->block_size = blksize;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಶೂನ್ಯ *buf)
{
  PARTITION     *part;
  size_t        size;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_data;

  size = (size_t)((ಎಣಿಕೆ < 0) ? -ಎಣಿಕೆ : ಎಣಿಕೆ * channel->block_size);
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

  ಯದಿ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    ಪ್ರತ್ಯಾಗಮ EFAULT;
  }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಧ್ರುವ ಶೂನ್ಯ *buf)
{
  PARTITION     *part;
  size_t        size;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_data;

  ಯದಿ(ಎಣಿಕೆ == 1)
    size = (size_t)channel->block_size;
  ಅನ್ಯಥಾ
  {
    ಯದಿ (ಎಣಿಕೆ < 0)
      size = (size_t)-ಎಣಿಕೆ;
    ಅನ್ಯಥಾ
      size = (size_t)(ಎಣಿಕೆ * channel->block_size);
  }

  loc = (ext2_loff_t)block * channel->block_size;
  lba2chs(loc, &chs, part);
  _dio_hw_error = biosdisk(DISK_WRITE,
                           part->phys,
                           chs.head,
                           chs.cyl,
                           chs.sector,
                           size < 512 ? 1 : size/512,
                           (ಶೂನ್ಯ*)buf);

  ಯದಿ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    ಪ್ರತ್ಯಾಗಮ EFAULT;
  }

  ಪ್ರತ್ಯಾಗಮ 0;
}

#ifdef __TURBOC__
#pragma argsused
#endif
ಸ್ಥಿರ errcode_t dos_flush(io_channel channel)
{
  /*
   * No buffers, no flush...
   */
  ಪ್ರತ್ಯಾಗಮ 0;
}
