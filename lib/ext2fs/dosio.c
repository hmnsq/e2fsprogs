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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <bios.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <ctype.h>
#অন্তর্ভাব <io.h>
#ifdef HAVE_ERRNO_H
#অন্তর্ভাব <errno.h>
#endif

#অন্তর্ভাব <ext2fs/ext2_types.h>
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "dosio.h"
#অন্তর্ভাব "et/com_err.h"
#অন্তর্ভাব "ext2_err.h"
#অন্তর্ভাব "ext2fs/io.h"

/*
 * Some helper macros
 */
#define LINUX_EXT2FS       0x83
#define LINUX_SWAP         0x82
#define WRITE_ERR(বার্তা_2) লিখ(2, বার্তা_2, strlen(বার্তা_2))
#define WRITE_ERR_S(বার্তা_2) লিখ(2, বার্তা_2, পরিমাণ(বার্তা_2))

/*
 * Exported variables
 */
unsigned long        _dio_error;
unsigned long        _dio_hw_error;

/*
 * Array of all opened partitions
 */
স্থির PARTITION        **partitions = রিক্ত;
স্থির unsigned short   npart = 0; /* Number of mapped partitions */
স্থির PARTITION        *active = রিক্ত;

/*
 * I/O Manager routine prototypes
 */
স্থির errcode_t dos_open(ধ্রুব বর্ণ *dev, পূর্ণাঙ্ক flags, io_channel *channel);
স্থির errcode_t dos_close(io_channel channel);
স্থির errcode_t dos_set_blksize(io_channel channel, পূর্ণাঙ্ক blksize);
স্থির errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             পূর্ণাঙ্ক গুনতি, শূন্য *buf);
স্থির errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               পূর্ণাঙ্ক গুনতি, ধ্রুব শূন্য *buf);
স্থির errcode_t dos_flush(io_channel channel);

স্থির সংরচনা struct_io_manager struct_dos_manager = {
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

#define EXT2_CHECK_MAGIC(সংরচনা, code) \
          যদি ((সংরচনা)->magic != (code)) প্রত্যাগম (code)

/*
 * Calculates a CHS address of a sector from its LBA
 * offset for the given partition.
 */
স্থির শূন্য lba2chs(unsigned long lba_addr, CHS *chs, PARTITION *part)
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
স্থির PTABLE_ENTRY *scan_partition_table(PTABLE_ENTRY *pentry,
                                          unsigned short phys,
                                          unsigned বর্ণ *pno)
{
  unsigned        i;

  যদি(*pno != 0xFF && *pno >= 5)
     প্রত্যাগম রিক্ত; /* We don't support extended partitions for now */

  যদি(*pno != 0xFF)
  {
    যদি(pentry[*pno].type == LINUX_EXT2FS)
      প্রত্যাগম &pentry[*pno];
    অন্যথা
    {
      যদি(!pentry[*pno].type)
        *pno = 0xFE;
      অন্যথা যদি(pentry[*pno].type == LINUX_SWAP)
        *pno = 0xFD;
      প্রত্যাগম রিক্ত;
    }
  }

  প্রতি(i = 0; i < 4; i++)
    যদি(pentry[i].type == LINUX_EXT2FS)
    {
      *pno = i;
      প্রত্যাগম &pentry[i];
    }

  প্রত্যাগম রিক্ত;
}

/*
 * Allocate libext2fs structures associated with I/O manager
 */
স্থির io_channel alloc_io_channel(PARTITION *part)
{
  io_channel     ioch;

  ioch = (io_channel)malloc(পরিমাণ(সংরচনা struct_io_channel));
  যদি (!ioch)
	  প্রত্যাগম রিক্ত;
  memset(ioch, 0, পরিমাণ(সংরচনা struct_io_channel));
  ioch->magic = EXT2_ET_MAGIC_IO_CHANNEL;
  ioch->manager = dos_io_manager;
  ioch->name = (বর্ণ *)malloc(strlen(part->dev)+1);
  যদি (!ioch->name) {
	  মুক্ত(ioch);
	  প্রত্যাগম রিক্ত;
  }
  strcpy(ioch->name, part->dev);
  ioch->private_data = part;
  ioch->block_size = 1024; /* The smallest ext2fs block size */
  ioch->read_error = 0;
  ioch->write_error = 0;

  প্রত্যাগম ioch;
}

#ifdef __TURBOC__
#pragma argsused
#endif
/*
 * Open the 'name' partition, initialize all information structures
 * we need to keep and create libext2fs I/O manager.
 */
স্থির errcode_t dos_open(ধ্রুব বর্ণ *dev, পূর্ণাঙ্ক flags, io_channel *channel)
{
  unsigned বর্ণ  *tmp, sec[512];
  PARTITION      *part;
  PTABLE_ENTRY   *pent;
  PARTITION        **newparts;

  যদি(!dev)
  {
    _dio_error = ERR_BADDEV;
    প্রত্যাগম EXT2_ET_BAD_DEVICE_NAME;
  }

  /*
   * First check whether the dev name is OK
   */
  tmp = (unsigned বর্ণ*)strrchr(dev, '/');
  যদি(!tmp)
  {
    _dio_error = ERR_BADDEV;
    প্রত্যাগম EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp = 0;
  যদি(strcmp(dev, "/dev"))
  {
    _dio_error = ERR_BADDEV;
    প্রত্যাগম EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp++ = '/';

  /*
   * Check whether the partition data is already in cache
   */

  part = (PARTITION*)malloc(পরিমাণ(PARTITION));
  যদি (!part)
	  প্রত্যাগম ENOMEM;
  {
    পূর্ণাঙ্ক   i = 0;

    প্রতি(;i < npart; i++)
      যদি(!strcmp(partitions[i]->dev, dev))
      {
        /* Found it! Make it the active one */
        active = partitions[i];
        *channel = alloc_io_channel(active);
	যদি (!*channel)
		প্রত্যাগম ENOMEM;
        প্রত্যাগম 0;
      }
  }

  /*
   * Drive number & optionally partn number
   */
  বিকল্প(tmp[0])
  {
    পক্ষ 'h':
    পক্ষ 's':
      part->phys = 0x80;
      part->phys += toupper(tmp[2]) - 'A';
      /*
       * Do we have the partition number?
       */
      যদি(tmp[3])
        part->pno = isdigit((পূর্ণাঙ্ক)tmp[3]) ? tmp[3] - '0' - 1: 0;
      অন্যথা
        part->pno = 0xFF;
      ভঙ্গ;

    পক্ষ 'f':
      যদি(tmp[2])
        part->phys = isdigit((পূর্ণাঙ্ক)tmp[2]) ? tmp[2] - '0' : 0;
      অন্যথা
        part->phys = 0x00; /* We'll assume /dev/fd0 */
      ভঙ্গ;

    স্বভাব:
      _dio_error = ERR_BADDEV;
      প্রত্যাগম ENODEV;
  }

  যদি(part->phys < 0x80)
  {
     /* We don't support floppies for now */
     _dio_error = ERR_NOTSUPP;
     প্রত্যাগম EINVAL;
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

  যদি(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    মুক্ত(part->dev);
    মুক্ত(part);
    প্রত্যাগম EFAULT;
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

  যদি(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    মুক্ত(part->dev);
    মুক্ত(part);
    প্রত্যাগম EFAULT;
  }

  pent = (PTABLE_ENTRY*)&sec[0x1BE];
  pent = scan_partition_table(pent, part->phys, &part->pno);

  যদি(!pent)
  {
    _dio_error = part->pno == 0xFE ? ERR_EMPTYPART :
                 part->pno == 0xFD ? ERR_LINUXSWAP : ERR_NOTEXT2FS;
    মুক্ত(part->dev);
    মুক্ত(part);
    প্রত্যাগম ENODEV;
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
  newparts = (PARTITION**)realloc(partitions, পরিমাণ(PARTITION) * npart);
  যদি (!newparts) {
	  মুক্ত(part);
	  প্রত্যাগম ENOMEM;
  }
  partitions = newparts;
  partitions[npart++] = active = part;

  /*
   * Now alloc all libe2fs structures
   */
  *channel = alloc_io_channel(active);
  যদি (!*channel)
	  প্রত্যাগম ENOMEM;

  প্রত্যাগম 0;
}

স্থির errcode_t dos_close(io_channel channel)
{
	মুক্ত(channel->name);
	মুক্ত(channel);

	প্রত্যাগম 0;
}

স্থির errcode_t dos_set_blksize(io_channel channel, পূর্ণাঙ্ক blksize)
{
  channel->block_size = blksize;

  প্রত্যাগম 0;
}

স্থির errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             পূর্ণাঙ্ক গুনতি, শূন্য *buf)
{
  PARTITION     *part;
  size_t        size;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_data;

  size = (size_t)((গুনতি < 0) ? -গুনতি : গুনতি * channel->block_size);
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

  যদি(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    প্রত্যাগম EFAULT;
  }

  প্রত্যাগম 0;
}

স্থির errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               পূর্ণাঙ্ক গুনতি, ধ্রুব শূন্য *buf)
{
  PARTITION     *part;
  size_t        size;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_data;

  যদি(গুনতি == 1)
    size = (size_t)channel->block_size;
  অন্যথা
  {
    যদি (গুনতি < 0)
      size = (size_t)-গুনতি;
    অন্যথা
      size = (size_t)(গুনতি * channel->block_size);
  }

  loc = (ext2_loff_t)block * channel->block_size;
  lba2chs(loc, &chs, part);
  _dio_hw_error = biosdisk(DISK_WRITE,
                           part->phys,
                           chs.head,
                           chs.cyl,
                           chs.sector,
                           size < 512 ? 1 : size/512,
                           (শূন্য*)buf);

  যদি(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    প্রত্যাগম EFAULT;
  }

  প্রত্যাগম 0;
}

#ifdef __TURBOC__
#pragma argsused
#endif
স্থির errcode_t dos_flush(io_channel channel)
{
  /*
   * No buffers, no flush...
   */
  প্রত্যাগম 0;
}
