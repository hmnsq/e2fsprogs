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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <bios.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <ctype.h>
#अन्तर्भाव <io.h>
#ifdef HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif

#अन्तर्भाव <ext2fs/ext2_types.h>
#अन्तर्भाव "utils.h"
#अन्तर्भाव "dosio.h"
#अन्तर्भाव "et/com_err.h"
#अन्तर्भाव "ext2_err.h"
#अन्तर्भाव "ext2fs/io.h"

/*
 * Some helper macros
 */
#define LINUX_EXT2FS       0x83
#define LINUX_SWAP         0x82
#define WRITE_ERR(सन्देशः_2) लिख(2, सन्देशः_2, strlen(सन्देशः_2))
#define WRITE_ERR_S(सन्देशः_2) लिख(2, सन्देशः_2, परिमाण(सन्देशः_2))

/*
 * Exported variables
 */
unsigned long        _dio_error;
unsigned long        _dio_hw_error;

/*
 * Array of all opened partitions
 */
स्थिर PARTITION        **partitions = रिक्त;
स्थिर unsigned short   npart = 0; /* Number of mapped partitions */
स्थिर PARTITION        *active = रिक्त;

/*
 * I/O Manager routine prototypes
 */
स्थिर errcode_t dos_open(ध्रुव वर्ण *dev, पूर्णाङ्क flags, io_channel *channel);
स्थिर errcode_t dos_close(io_channel channel);
स्थिर errcode_t dos_set_blksize(io_channel channel, पूर्णाङ्क blksize);
स्थिर errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             पूर्णाङ्क गणनम्, शून्य *buf);
स्थिर errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               पूर्णाङ्क गणनम्, ध्रुव शून्य *buf);
स्थिर errcode_t dos_flush(io_channel channel);

स्थिर संरचना struct_io_manager struct_dos_manager = {
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

#define EXT2_CHECK_MAGIC(संरचना, code) \
          सचे ((संरचना)->magic != (code)) पटिनिवत्त (code)

/*
 * Calculates a CHS address of a sector from its LBA
 * offset for the given partition.
 */
स्थिर शून्य lba2chs(unsigned long lba_addr, CHS *chs, PARTITION *part)
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
स्थिर PTABLE_ENTRY *scan_partition_table(PTABLE_ENTRY *pentry,
                                          unsigned short phys,
                                          unsigned वर्ण *pno)
{
  unsigned        i;

  सचे(*pno != 0xFF && *pno >= 5)
     पटिनिवत्त रिक्त; /* We don't support extended partitions for now */

  सचे(*pno != 0xFF)
  {
    सचे(pentry[*pno].type == LINUX_EXT2FS)
      पटिनिवत्त &pentry[*pno];
    अञ्ञथा
    {
      सचे(!pentry[*pno].type)
        *pno = 0xFE;
      अञ्ञथा सचे(pentry[*pno].type == LINUX_SWAP)
        *pno = 0xFD;
      पटिनिवत्त रिक्त;
    }
  }

  अत्थाय(i = 0; i < 4; i++)
    सचे(pentry[i].type == LINUX_EXT2FS)
    {
      *pno = i;
      पटिनिवत्त &pentry[i];
    }

  पटिनिवत्त रिक्त;
}

/*
 * Allocate libext2fs structures associated with I/O manager
 */
स्थिर io_channel alloc_io_channel(PARTITION *part)
{
  io_channel     ioch;

  ioch = (io_channel)malloc(परिमाण(संरचना struct_io_channel));
  सचे (!ioch)
	  पटिनिवत्त रिक्त;
  memset(ioch, 0, परिमाण(संरचना struct_io_channel));
  ioch->magic = EXT2_ET_MAGIC_IO_CHANNEL;
  ioch->manager = dos_io_manager;
  ioch->name = (वर्ण *)malloc(strlen(part->dev)+1);
  सचे (!ioch->name) {
	  मुक्त(ioch);
	  पटिनिवत्त रिक्त;
  }
  strcpy(ioch->name, part->dev);
  ioch->private_data = part;
  ioch->block_size = 1024; /* The smallest ext2fs block size */
  ioch->read_error = 0;
  ioch->write_error = 0;

  पटिनिवत्त ioch;
}

#ifdef __TURBOC__
#pragma argsused
#endif
/*
 * Open the 'name' partition, initialize all information structures
 * we need to keep and create libext2fs I/O manager.
 */
स्थिर errcode_t dos_open(ध्रुव वर्ण *dev, पूर्णाङ्क flags, io_channel *channel)
{
  unsigned वर्ण  *tmp, sec[512];
  PARTITION      *part;
  PTABLE_ENTRY   *pent;
  PARTITION        **newparts;

  सचे(!dev)
  {
    _dio_error = ERR_BADDEV;
    पटिनिवत्त EXT2_ET_BAD_DEVICE_NAME;
  }

  /*
   * First check whether the dev name is OK
   */
  tmp = (unsigned वर्ण*)strrchr(dev, '/');
  सचे(!tmp)
  {
    _dio_error = ERR_BADDEV;
    पटिनिवत्त EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp = 0;
  सचे(strcmp(dev, "/dev"))
  {
    _dio_error = ERR_BADDEV;
    पटिनिवत्त EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp++ = '/';

  /*
   * Check whether the partition data is already in cache
   */

  part = (PARTITION*)malloc(परिमाण(PARTITION));
  सचे (!part)
	  पटिनिवत्त ENOMEM;
  {
    पूर्णाङ्क   i = 0;

    अत्थाय(;i < npart; i++)
      सचे(!strcmp(partitions[i]->dev, dev))
      {
        /* Found it! Make it the active one */
        active = partitions[i];
        *channel = alloc_io_channel(active);
	सचे (!*channel)
		पटिनिवत्त ENOMEM;
        पटिनिवत्त 0;
      }
  }

  /*
   * Drive number & optionally partn number
   */
  विकल्प(tmp[0])
  {
    पक्ष 'h':
    पक्ष 's':
      part->phys = 0x80;
      part->phys += toupper(tmp[2]) - 'A';
      /*
       * Do we have the partition number?
       */
      सचे(tmp[3])
        part->pno = isdigit((पूर्णाङ्क)tmp[3]) ? tmp[3] - '0' - 1: 0;
      अञ्ञथा
        part->pno = 0xFF;
      भङ्ग;

    पक्ष 'f':
      सचे(tmp[2])
        part->phys = isdigit((पूर्णाङ्क)tmp[2]) ? tmp[2] - '0' : 0;
      अञ्ञथा
        part->phys = 0x00; /* We'll assume /dev/fd0 */
      भङ्ग;

    स्वभाव:
      _dio_error = ERR_BADDEV;
      पटिनिवत्त ENODEV;
  }

  सचे(part->phys < 0x80)
  {
     /* We don't support floppies for now */
     _dio_error = ERR_NOTSUPP;
     पटिनिवत्त EINVAL;
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

  सचे(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    मुक्त(part->dev);
    मुक्त(part);
    पटिनिवत्त EFAULT;
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

  सचे(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    मुक्त(part->dev);
    मुक्त(part);
    पटिनिवत्त EFAULT;
  }

  pent = (PTABLE_ENTRY*)&sec[0x1BE];
  pent = scan_partition_table(pent, part->phys, &part->pno);

  सचे(!pent)
  {
    _dio_error = part->pno == 0xFE ? ERR_EMPTYPART :
                 part->pno == 0xFD ? ERR_LINUXSWAP : ERR_NOTEXT2FS;
    मुक्त(part->dev);
    मुक्त(part);
    पटिनिवत्त ENODEV;
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
  newparts = (PARTITION**)realloc(partitions, परिमाण(PARTITION) * npart);
  सचे (!newparts) {
	  मुक्त(part);
	  पटिनिवत्त ENOMEM;
  }
  partitions = newparts;
  partitions[npart++] = active = part;

  /*
   * Now alloc all libe2fs structures
   */
  *channel = alloc_io_channel(active);
  सचे (!*channel)
	  पटिनिवत्त ENOMEM;

  पटिनिवत्त 0;
}

स्थिर errcode_t dos_close(io_channel channel)
{
	मुक्त(channel->name);
	मुक्त(channel);

	पटिनिवत्त 0;
}

स्थिर errcode_t dos_set_blksize(io_channel channel, पूर्णाङ्क blksize)
{
  channel->block_size = blksize;

  पटिनिवत्त 0;
}

स्थिर errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             पूर्णाङ्क गणनम्, शून्य *buf)
{
  PARTITION     *part;
  size_t        size;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_data;

  size = (size_t)((गणनम् < 0) ? -गणनम् : गणनम् * channel->block_size);
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

  सचे(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    पटिनिवत्त EFAULT;
  }

  पटिनिवत्त 0;
}

स्थिर errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
  PARTITION     *part;
  size_t        size;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_data;

  सचे(गणनम् == 1)
    size = (size_t)channel->block_size;
  अञ्ञथा
  {
    सचे (गणनम् < 0)
      size = (size_t)-गणनम्;
    अञ्ञथा
      size = (size_t)(गणनम् * channel->block_size);
  }

  loc = (ext2_loff_t)block * channel->block_size;
  lba2chs(loc, &chs, part);
  _dio_hw_error = biosdisk(DISK_WRITE,
                           part->phys,
                           chs.head,
                           chs.cyl,
                           chs.sector,
                           size < 512 ? 1 : size/512,
                           (शून्य*)buf);

  सचे(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    पटिनिवत्त EFAULT;
  }

  पटिनिवत्त 0;
}

#ifdef __TURBOC__
#pragma argsused
#endif
स्थिर errcode_t dos_flush(io_channel channel)
{
  /*
   * No buffers, no flush...
   */
  पटिनिवत्त 0;
}
