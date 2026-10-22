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

#شامل "config.h"
#شامل <stdio.h>
#شامل <bios.h>
#شامل <string.h>
#شامل <ctype.h>
#شامل <io.h>
#ifdef HAVE_ERRNO_H
#شامل <errno.h>
#endif

#شامل <ext2fs/ext2_types.h>
#شامل "utils.h"
#شامل "dosio.h"
#شامل "et/com_err.h"
#شامل "ext2_err.h"
#شامل "ext2fs/io.h"

/*
 * Some helper macros
 */
#define LINUX_EXT2FS       0x83
#define LINUX_SWAP         0x82
#define WRITE_ERR(پیام_2) اكتب(2, پیام_2, strlen(پیام_2))
#define WRITE_ERR_S(پیام_2) اكتب(2, پیام_2, اندازه(پیام_2))

/*
 * Exported variables
 */
unsigned long        _dio_error;
unsigned long        _dio_hw_error;

/*
 * Array of all opened partitions
 */
ایستا PARTITION        **partitions = پوچ;
ایستا unsigned short   npart = 0; /* Number of mapped partitions */
ایستا PARTITION        *active = پوچ;

/*
 * I/O Manager routine prototypes
 */
ایستا errcode_t dos_open(ثابت نویسه *dev, صحیح flags, io_channel *channel);
ایستا errcode_t dos_close(io_channel channel);
ایستا errcode_t dos_set_blksize(io_channel channel, صحیح blksize);
ایستا errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             صحیح شمار, تهی *buf);
ایستا errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               صحیح شمار, ثابت تهی *buf);
ایستا errcode_t dos_flush(io_channel channel);

ایستا ساختار struct_io_manager struct_dos_manager = {
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

#define EXT2_CHECK_MAGIC(ساختار, code) \
          اگر ((ساختار)->magic != (code)) بازگشت (code)

/*
 * Calculates a CHS address of a sector from its LBA
 * offset for the given partition.
 */
ایستا تهی lba2chs(unsigned long lba_addr, CHS *chs, PARTITION *part)
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
ایستا PTABLE_ENTRY *scan_partition_table(PTABLE_ENTRY *pentry,
                                          unsigned short phys,
                                          unsigned نویسه *pno)
{
  unsigned        i;

  اگر(*pno != 0xFF && *pno >= 5)
     بازگشت پوچ; /* We don't support extended partitions for now */

  اگر(*pno != 0xFF)
  {
    اگر(pentry[*pno].type == LINUX_EXT2FS)
      بازگشت &pentry[*pno];
    وگرنه
    {
      اگر(!pentry[*pno].type)
        *pno = 0xFE;
      وگرنه اگر(pentry[*pno].type == LINUX_SWAP)
        *pno = 0xFD;
      بازگشت پوچ;
    }
  }

  برای(i = 0; i < 4; i++)
    اگر(pentry[i].type == LINUX_EXT2FS)
    {
      *pno = i;
      بازگشت &pentry[i];
    }

  بازگشت پوچ;
}

/*
 * Allocate libext2fs structures associated with I/O manager
 */
ایستا io_channel alloc_io_channel(PARTITION *part)
{
  io_channel     ioch;

  ioch = (io_channel)malloc(اندازه(ساختار struct_io_channel));
  اگر (!ioch)
	  بازگشت پوچ;
  memset(ioch, 0, اندازه(ساختار struct_io_channel));
  ioch->magic = EXT2_ET_MAGIC_IO_CHANNEL;
  ioch->manager = dos_io_manager;
  ioch->name = (نویسه *)malloc(strlen(part->dev)+1);
  اگر (!ioch->name) {
	  آزاد(ioch);
	  بازگشت پوچ;
  }
  strcpy(ioch->name, part->dev);
  ioch->private_data = part;
  ioch->block_size = 1024; /* The smallest ext2fs block size */
  ioch->read_error = 0;
  ioch->write_error = 0;

  بازگشت ioch;
}

#ifdef __TURBOC__
#pragma argsused
#endif
/*
 * Open the 'name' partition, initialize all information structures
 * we need to keep and create libext2fs I/O manager.
 */
ایستا errcode_t dos_open(ثابت نویسه *dev, صحیح flags, io_channel *channel)
{
  unsigned نویسه  *tmp, sec[512];
  PARTITION      *part;
  PTABLE_ENTRY   *pent;
  PARTITION        **newparts;

  اگر(!dev)
  {
    _dio_error = ERR_BADDEV;
    بازگشت EXT2_ET_BAD_DEVICE_NAME;
  }

  /*
   * First check whether the dev name is OK
   */
  tmp = (unsigned نویسه*)strrchr(dev, '/');
  اگر(!tmp)
  {
    _dio_error = ERR_BADDEV;
    بازگشت EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp = 0;
  اگر(strcmp(dev, "/dev"))
  {
    _dio_error = ERR_BADDEV;
    بازگشت EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp++ = '/';

  /*
   * Check whether the partition data is already in cache
   */

  part = (PARTITION*)malloc(اندازه(PARTITION));
  اگر (!part)
	  بازگشت ENOMEM;
  {
    صحیح   i = 0;

    برای(;i < npart; i++)
      اگر(!strcmp(partitions[i]->dev, dev))
      {
        /* Found it! Make it the active one */
        active = partitions[i];
        *channel = alloc_io_channel(active);
	اگر (!*channel)
		بازگشت ENOMEM;
        بازگشت 0;
      }
  }

  /*
   * Drive number & optionally partn number
   */
  انتخاب(tmp[0])
  {
    حالت 'h':
    حالت 's':
      part->phys = 0x80;
      part->phys += toupper(tmp[2]) - 'A';
      /*
       * Do we have the partition number?
       */
      اگر(tmp[3])
        part->pno = isdigit((صحیح)tmp[3]) ? tmp[3] - '0' - 1: 0;
      وگرنه
        part->pno = 0xFF;
      بشکن;

    حالت 'f':
      اگر(tmp[2])
        part->phys = isdigit((صحیح)tmp[2]) ? tmp[2] - '0' : 0;
      وگرنه
        part->phys = 0x00; /* We'll assume /dev/fd0 */
      بشکن;

    پیشفرض:
      _dio_error = ERR_BADDEV;
      بازگشت ENODEV;
  }

  اگر(part->phys < 0x80)
  {
     /* We don't support floppies for now */
     _dio_error = ERR_NOTSUPP;
     بازگشت EINVAL;
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

  اگر(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    آزاد(part->dev);
    آزاد(part);
    بازگشت EFAULT;
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

  اگر(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    آزاد(part->dev);
    آزاد(part);
    بازگشت EFAULT;
  }

  pent = (PTABLE_ENTRY*)&sec[0x1BE];
  pent = scan_partition_table(pent, part->phys, &part->pno);

  اگر(!pent)
  {
    _dio_error = part->pno == 0xFE ? ERR_EMPTYPART :
                 part->pno == 0xFD ? ERR_LINUXSWAP : ERR_NOTEXT2FS;
    آزاد(part->dev);
    آزاد(part);
    بازگشت ENODEV;
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
  newparts = (PARTITION**)realloc(partitions, اندازه(PARTITION) * npart);
  اگر (!newparts) {
	  آزاد(part);
	  بازگشت ENOMEM;
  }
  partitions = newparts;
  partitions[npart++] = active = part;

  /*
   * Now alloc all libe2fs structures
   */
  *channel = alloc_io_channel(active);
  اگر (!*channel)
	  بازگشت ENOMEM;

  بازگشت 0;
}

ایستا errcode_t dos_close(io_channel channel)
{
	آزاد(channel->name);
	آزاد(channel);

	بازگشت 0;
}

ایستا errcode_t dos_set_blksize(io_channel channel, صحیح blksize)
{
  channel->block_size = blksize;

  بازگشت 0;
}

ایستا errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             صحیح شمار, تهی *buf)
{
  PARTITION     *part;
  size_t        size;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_data;

  size = (size_t)((شمار < 0) ? -شمار : شمار * channel->block_size);
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

  اگر(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    بازگشت EFAULT;
  }

  بازگشت 0;
}

ایستا errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               صحیح شمار, ثابت تهی *buf)
{
  PARTITION     *part;
  size_t        size;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_data;

  اگر(شمار == 1)
    size = (size_t)channel->block_size;
  وگرنه
  {
    اگر (شمار < 0)
      size = (size_t)-شمار;
    وگرنه
      size = (size_t)(شمار * channel->block_size);
  }

  loc = (ext2_loff_t)block * channel->block_size;
  lba2chs(loc, &chs, part);
  _dio_hw_error = biosdisk(DISK_WRITE,
                           part->phys,
                           chs.head,
                           chs.cyl,
                           chs.sector,
                           size < 512 ? 1 : size/512,
                           (تهی*)buf);

  اگر(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    بازگشت EFAULT;
  }

  بازگشت 0;
}

#ifdef __TURBOC__
#pragma argsused
#endif
ایستا errcode_t dos_flush(io_channel channel)
{
  /*
   * No buffers, no flush...
   */
  بازگشت 0;
}
