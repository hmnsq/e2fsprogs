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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <bios.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <ctype.h>
#ଅନ୍ତର୍ଭାଵ <io.h>
#ifdef HAVE_ERRNO_H
#ଅନ୍ତର୍ଭାଵ <errno.h>
#endif

#ଅନ୍ତର୍ଭାଵ <ext2fs/ext2_types.h>
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "dosio.h"
#ଅନ୍ତର୍ଭାଵ "et/com_err.h"
#ଅନ୍ତର୍ଭାଵ "ext2_err.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs/io.h"

/*
 * Some helper macros
 */
#define LINUX_EXT2FS       0x83
#define LINUX_SWAP         0x82
#define WRITE_ERR(ସନ୍ଦେଶ_2) ଲିଖ(2, ସନ୍ଦେଶ_2, strlen(ସନ୍ଦେଶ_2))
#define WRITE_ERR_S(ସନ୍ଦେଶ_2) ଲିଖ(2, ସନ୍ଦେଶ_2, ଆକାର(ସନ୍ଦେଶ_2))

/*
 * Exported variables
 */
unsigned long        _dio_error;
unsigned long        _dio_hw_error;

/*
 * Array of all opened partitions
 */
ସ୍ଥିର PARTITION        **partitions = ଖାଲି;
ସ୍ଥିର unsigned short   npart = 0; /* Number of mapped partitions */
ସ୍ଥିର PARTITION        *active = ଖାଲି;

/*
 * I/O Manager routine prototypes
 */
ସ୍ଥିର errcode_t dos_open(ଅଚଳ ଅକ୍ଷର *dev, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, io_channel *channel);
ସ୍ଥିର errcode_t dos_close(io_channel channel);
ସ୍ଥିର errcode_t dos_set_blksize(io_channel channel, ପୂର୍ଣ୍ଣସଂଖ୍ୟା blksize);
ସ୍ଥିର errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଶୂନ୍ୟ *buf);
ସ୍ଥିର errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *buf);
ସ୍ଥିର errcode_t dos_flush(io_channel channel);

ସ୍ଥିର ସଂରଚନା struct_io_manager struct_dos_manager = {
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

#define EXT2_CHECK_MAGIC(ସଂରଚନା, code) \
          ଯଦି ((ସଂରଚନା)->magic != (code)) ଫେରନ୍ତୁ (code)

/*
 * Calculates a CHS address of a sector from its LBA
 * offset for the given partition.
 */
ସ୍ଥିର ଶୂନ୍ୟ lba2chs(unsigned long lba_addr, CHS *chs, PARTITION *part)
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
ସ୍ଥିର PTABLE_ENTRY *scan_partition_table(PTABLE_ENTRY *pentry,
                                          unsigned short phys,
                                          unsigned ଅକ୍ଷର *pno)
{
  unsigned        i;

  ଯଦି(*pno != 0xFF && *pno >= 5)
     ଫେରନ୍ତୁ ଖାଲି; /* We don't support extended partitions for now */

  ଯଦି(*pno != 0xFF)
  {
    ଯଦି(pentry[*pno].type == LINUX_EXT2FS)
      ଫେରନ୍ତୁ &pentry[*pno];
    ଅନ୍ୟଥା
    {
      ଯଦି(!pentry[*pno].type)
        *pno = 0xFE;
      ଅନ୍ୟଥା ଯଦି(pentry[*pno].type == LINUX_SWAP)
        *pno = 0xFD;
      ଫେରନ୍ତୁ ଖାଲି;
    }
  }

  ପାଇଁ(i = 0; i < 4; i++)
    ଯଦି(pentry[i].type == LINUX_EXT2FS)
    {
      *pno = i;
      ଫେରନ୍ତୁ &pentry[i];
    }

  ଫେରନ୍ତୁ ଖାଲି;
}

/*
 * Allocate libext2fs structures associated with I/O manager
 */
ସ୍ଥିର io_channel alloc_io_channel(PARTITION *part)
{
  io_channel     ioch;

  ioch = (io_channel)malloc(ଆକାର(ସଂରଚନା struct_io_channel));
  ଯଦି (!ioch)
	  ଫେରନ୍ତୁ ଖାଲି;
  memset(ioch, 0, ଆକାର(ସଂରଚନା struct_io_channel));
  ioch->magic = EXT2_ET_MAGIC_IO_CHANNEL;
  ioch->manager = dos_io_manager;
  ioch->name = (ଅକ୍ଷର *)malloc(strlen(part->dev)+1);
  ଯଦି (!ioch->name) {
	  ମୁକ୍ତ(ioch);
	  ଫେରନ୍ତୁ ଖାଲି;
  }
  strcpy(ioch->name, part->dev);
  ioch->private_data = part;
  ioch->block_size = 1024; /* The smallest ext2fs block size */
  ioch->read_error = 0;
  ioch->write_error = 0;

  ଫେରନ୍ତୁ ioch;
}

#ifdef __TURBOC__
#pragma argsused
#endif
/*
 * Open the 'name' partition, initialize all information structures
 * we need to keep and create libext2fs I/O manager.
 */
ସ୍ଥିର errcode_t dos_open(ଅଚଳ ଅକ୍ଷର *dev, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, io_channel *channel)
{
  unsigned ଅକ୍ଷର  *tmp, sec[512];
  PARTITION      *part;
  PTABLE_ENTRY   *pent;
  PARTITION        **newparts;

  ଯଦି(!dev)
  {
    _dio_error = ERR_BADDEV;
    ଫେରନ୍ତୁ EXT2_ET_BAD_DEVICE_NAME;
  }

  /*
   * First check whether the dev name is OK
   */
  tmp = (unsigned ଅକ୍ଷର*)strrchr(dev, '/');
  ଯଦି(!tmp)
  {
    _dio_error = ERR_BADDEV;
    ଫେରନ୍ତୁ EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp = 0;
  ଯଦି(strcmp(dev, "/dev"))
  {
    _dio_error = ERR_BADDEV;
    ଫେରନ୍ତୁ EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp++ = '/';

  /*
   * Check whether the partition data is already in cache
   */

  part = (PARTITION*)malloc(ଆକାର(PARTITION));
  ଯଦି (!part)
	  ଫେରନ୍ତୁ ENOMEM;
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା   i = 0;

    ପାଇଁ(;i < npart; i++)
      ଯଦି(!strcmp(partitions[i]->dev, dev))
      {
        /* Found it! Make it the active one */
        active = partitions[i];
        *channel = alloc_io_channel(active);
	ଯଦି (!*channel)
		ଫେରନ୍ତୁ ENOMEM;
        ଫେରନ୍ତୁ 0;
      }
  }

  /*
   * Drive number & optionally partn number
   */
  ବାଛ(tmp[0])
  {
    ସ୍ଥିତି 'h':
    ସ୍ଥିତି 's':
      part->phys = 0x80;
      part->phys += toupper(tmp[2]) - 'A';
      /*
       * Do we have the partition number?
       */
      ଯଦି(tmp[3])
        part->pno = isdigit((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)tmp[3]) ? tmp[3] - '0' - 1: 0;
      ଅନ୍ୟଥା
        part->pno = 0xFF;
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 'f':
      ଯଦି(tmp[2])
        part->phys = isdigit((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)tmp[2]) ? tmp[2] - '0' : 0;
      ଅନ୍ୟଥା
        part->phys = 0x00; /* We'll assume /dev/fd0 */
      ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      _dio_error = ERR_BADDEV;
      ଫେରନ୍ତୁ ENODEV;
  }

  ଯଦି(part->phys < 0x80)
  {
     /* We don't support floppies for now */
     _dio_error = ERR_NOTSUPP;
     ଫେରନ୍ତୁ EINVAL;
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

  ଯଦି(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    ମୁକ୍ତ(part->dev);
    ମୁକ୍ତ(part);
    ଫେରନ୍ତୁ EFAULT;
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

  ଯଦି(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    ମୁକ୍ତ(part->dev);
    ମୁକ୍ତ(part);
    ଫେରନ୍ତୁ EFAULT;
  }

  pent = (PTABLE_ENTRY*)&sec[0x1BE];
  pent = scan_partition_table(pent, part->phys, &part->pno);

  ଯଦି(!pent)
  {
    _dio_error = part->pno == 0xFE ? ERR_EMPTYPART :
                 part->pno == 0xFD ? ERR_LINUXSWAP : ERR_NOTEXT2FS;
    ମୁକ୍ତ(part->dev);
    ମୁକ୍ତ(part);
    ଫେରନ୍ତୁ ENODEV;
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
  newparts = (PARTITION**)realloc(partitions, ଆକାର(PARTITION) * npart);
  ଯଦି (!newparts) {
	  ମୁକ୍ତ(part);
	  ଫେରନ୍ତୁ ENOMEM;
  }
  partitions = newparts;
  partitions[npart++] = active = part;

  /*
   * Now alloc all libe2fs structures
   */
  *channel = alloc_io_channel(active);
  ଯଦି (!*channel)
	  ଫେରନ୍ତୁ ENOMEM;

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t dos_close(io_channel channel)
{
	ମୁକ୍ତ(channel->name);
	ମୁକ୍ତ(channel);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t dos_set_blksize(io_channel channel, ପୂର୍ଣ୍ଣସଂଖ୍ୟା blksize)
{
  channel->block_size = blksize;

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t dos_read_blk(io_channel channel, unsigned long block,
                                             ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଶୂନ୍ୟ *buf)
{
  PARTITION     *part;
  size_t        size;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_data;

  size = (size_t)((ଗଣନା_2 < 0) ? -ଗଣନା_2 : ଗଣନା_2 * channel->block_size);
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

  ଯଦି(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    ଫେରନ୍ତୁ EFAULT;
  }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t dos_write_blk(io_channel channel, unsigned long block,
                               ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *buf)
{
  PARTITION     *part;
  size_t        size;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_data;

  ଯଦି(ଗଣନା_2 == 1)
    size = (size_t)channel->block_size;
  ଅନ୍ୟଥା
  {
    ଯଦି (ଗଣନା_2 < 0)
      size = (size_t)-ଗଣନା_2;
    ଅନ୍ୟଥା
      size = (size_t)(ଗଣନା_2 * channel->block_size);
  }

  loc = (ext2_loff_t)block * channel->block_size;
  lba2chs(loc, &chs, part);
  _dio_hw_error = biosdisk(DISK_WRITE,
                           part->phys,
                           chs.head,
                           chs.cyl,
                           chs.sector,
                           size < 512 ? 1 : size/512,
                           (ଶୂନ୍ୟ*)buf);

  ଯଦି(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    ଫେରନ୍ତୁ EFAULT;
  }

  ଫେରନ୍ତୁ 0;
}

#ifdef __TURBOC__
#pragma argsused
#endif
ସ୍ଥିର errcode_t dos_flush(io_channel channel)
{
  /*
   * No buffers, no flush...
   */
  ଫେରନ୍ତୁ 0;
}
