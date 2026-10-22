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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <bios.h>
#ضمن <string.h>
#ضمن <ctype.h>
#ضمن <io.h>
#جيڪڏهن_وصف_ٿيل HAVE_ERRNO_H
#ضمن <errno.h>
#جيڪڏهن_پورو

#ضمن <ext2fs/ext2_types.h>
#ضمن "utils.h"
#ضمن "dosio.h"
#ضمن "et/com_err.h"
#ضمن "ext2_err.h"
#ضمن "ext2fs/io.h"

/*
 * Some helper macros
 */
#وصف_ڪريو LINUX_EXT2FS       0x83
#وصف_ڪريو LINUX_مٽا_سٽا         0x82
#وصف_ڪريو WRITE_ERR(نياپو_2) اكتب(2, نياپو_2, لڙي_ڊيگهه(نياپو_2))
#وصف_ڪريو WRITE_ERR_S(نياپو_2) اكتب(2, نياپو_2, حجم(نياپو_2))

/*
 * Exported variables
 */
بنانشان ڊگهو        _dio_error;
بنانشان ڊگهو        _dio_hw_error;

/*
 * Array of all opened partitions
 */
ساكن حصا_ڪريو        **partitions = عدم;
ساكن بنانشان ننڍو   npart = 0; /* Number of mapped partitions */
ساكن حصا_ڪريو        *سرگرم = عدم;

/*
 * I/O Manager routine prototypes
 */
ساكن errcode_t dos_open(ثابت حرف *dev, صحيح flags, io_channel *channel);
ساكن errcode_t dos_close(io_channel channel);
ساكن errcode_t dos_سيٽ_blksize(io_channel channel, صحيح blksize);
ساكن errcode_t dos_read_blk(io_channel channel, بنانشان ڊگهو بلاڪ,
                                             صحيح ڳڻپ, فراغ *buf);
ساكن errcode_t dos_write_blk(io_channel channel, بنانشان ڊگهو بلاڪ,
                               صحيح ڳڻپ, ثابت فراغ *buf);
ساكن errcode_t dos_flush(io_channel channel);

ساكن بنية struct_io_manager struct_dos_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "DOS I/O Manager",
	.open		= dos_open,
	.close		= dos_close,
	.سيٽ_blksize	= dos_سيٽ_blksize,
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

#وصف_ڪريو EXT2_CHECK_MAGIC(بنية, ڪوڊ) \
          إذا ((بنية)->magic != (ڪوڊ)) أرجع (ڪوڊ)

/*
 * Calculates a CHS address of a sector from its LBA
 * offset for the given partition.
 */
ساكن فراغ lba2chs(بنانشان ڊگهو lba_addr, CHS *chs, حصا_ڪريو *part)
{
  بنانشان ڊگهو      abss;

  chs->وٿي = lba_addr & 0x000001FF;
  abss = (lba_addr >> 9) + part->شروعات;
  chs->cyl    = abss / (part->sects * part->heads);
  chs->سر   = (abss / part->sects) % part->heads;
  chs->sector = (abss % part->sects) + 1;
}

#جيڪڏهن_وصف_ٿيل __TURBOC__
#هدايت argsused
#جيڪڏهن_پورو
/*
 * Scans the passed partition table looking for *pno partition
 * that has LINUX_EXT2FS type.
 *
 * TODO:
 * For partition numbers >5 Linux uses DOS extended partitions -
 * dive into them an return an appropriate entry. Also dive into
 * extended partitions when scanning for a first Linux/ext2fs.
 */
ساكن PTABLE_داخلا *جاچ_حصا_ڪريو_جدول(PTABLE_داخلا *pentry,
                                          بنانشان ننڍو phys,
                                          بنانشان حرف *pno)
{
  بنانشان        i;

  إذا(*pno != 0xFF && *pno >= 5)
     أرجع عدم; /* We don't support extended partitions for now */

  إذا(*pno != 0xFF)
  {
    إذا(pentry[*pno].type == LINUX_EXT2FS)
      أرجع &pentry[*pno];
    وإلا
    {
      إذا(!pentry[*pno].type)
        *pno = 0xFE;
      وإلا إذا(pentry[*pno].type == LINUX_مٽا_سٽا)
        *pno = 0xFD;
      أرجع عدم;
    }
  }

  لكل(i = 0; i < 4; i++)
    إذا(pentry[i].type == LINUX_EXT2FS)
    {
      *pno = i;
      أرجع &pentry[i];
    }

  أرجع عدم;
}

/*
 * Allocate libext2fs structures associated with I/O manager
 */
ساكن io_channel alloc_io_channel(حصا_ڪريو *part)
{
  io_channel     ioch;

  ioch = (io_channel)يادگيري_ڏيو(حجم(بنية struct_io_channel));
  إذا (!ioch)
	  أرجع عدم;
  memset(ioch, 0, حجم(بنية struct_io_channel));
  ioch->magic = EXT2_ET_MAGIC_IO_CHANNEL;
  ioch->manager = dos_io_manager;
  ioch->name = (حرف *)يادگيري_ڏيو(لڙي_ڊيگهه(part->dev)+1);
  إذا (!ioch->name) {
	  حرر(ioch);
	  أرجع عدم;
  }
  strcpy(ioch->name, part->dev);
  ioch->ذاتي_معلومات = part;
  ioch->بلاڪ_سائيز = 1024; /* The smallest ext2fs block size */
  ioch->read_error = 0;
  ioch->write_error = 0;

  أرجع ioch;
}

#جيڪڏهن_وصف_ٿيل __TURBOC__
#هدايت argsused
#جيڪڏهن_پورو
/*
 * Open the 'name' partition, initialize all information structures
 * we need to keep and create libext2fs I/O manager.
 */
ساكن errcode_t dos_open(ثابت حرف *dev, صحيح flags, io_channel *channel)
{
  بنانشان حرف  *tmp, sec[512];
  حصا_ڪريو      *part;
  PTABLE_داخلا   *pent;
  حصا_ڪريو        **newparts;

  إذا(!dev)
  {
    _dio_error = ERR_BADDEV;
    أرجع EXT2_ET_BAD_DEVICE_NAME;
  }

  /*
   * First check whether the dev name is OK
   */
  tmp = (بنانشان حرف*)strrchr(dev, '/');
  إذا(!tmp)
  {
    _dio_error = ERR_BADDEV;
    أرجع EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp = 0;
  إذا(strcmp(dev, "/dev"))
  {
    _dio_error = ERR_BADDEV;
    أرجع EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp++ = '/';

  /*
   * Check whether the partition data is already in cache
   */

  part = (حصا_ڪريو*)يادگيري_ڏيو(حجم(حصا_ڪريو));
  إذا (!part)
	  أرجع ENOMEM;
  {
    صحيح   i = 0;

    لكل(;i < npart; i++)
      إذا(!strcmp(partitions[i]->dev, dev))
      {
        /* Found it! Make it the active one */
        سرگرم = partitions[i];
        *channel = alloc_io_channel(سرگرم);
	إذا (!*channel)
		أرجع ENOMEM;
        أرجع 0;
      }
  }

  /*
   * Drive number & optionally partn number
   */
  بدل(tmp[0])
  {
    حالة 'h':
    حالة 's':
      part->phys = 0x80;
      part->phys += وڏو_اکر(tmp[2]) - 'A';
      /*
       * Do we have the partition number?
       */
      إذا(tmp[3])
        part->pno = انگ_آهي((صحيح)tmp[3]) ? tmp[3] - '0' - 1: 0;
      وإلا
        part->pno = 0xFF;
      اكسر;

    حالة 'f':
      إذا(tmp[2])
        part->phys = انگ_آهي((صحيح)tmp[2]) ? tmp[2] - '0' : 0;
      وإلا
        part->phys = 0x00; /* We'll assume /dev/fd0 */
      اكسر;

    افتراضي:
      _dio_error = ERR_BADDEV;
      أرجع ENODEV;
  }

  إذا(part->phys < 0x80)
  {
     /* We don't support floppies for now */
     _dio_error = ERR_NOTSUPP;
     أرجع EINVAL;
  }

  part->dev = strdup(dev);

  /*
   * Get drive's geometry
   */
  _dio_hw_error = biosdisk(DISK_GET_GEOMETRY,
                           part->phys,
                           0, /*سر*/
                           0, /* cylinder */
                           1, /* sector */
                           1, /* just one sector */
                           sec);

  إذا(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    حرر(part->dev);
    حرر(part);
    أرجع EFAULT;
  }

  /*
   * Calculate the geometry
   */
  part->cyls  = (بنانشان ننڍو)(((sec[0] >> 6) << 8) + sec[1] + 1);
  part->heads = sec[3] + 1;
  part->sects = sec[0] & 0x3F;

  /*
   * Now that we know all we need, let's look for the partition
   */
  _dio_hw_error = biosdisk(DISK_READ, part->phys, 0, 0, 1, 1, sec);

  إذا(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    حرر(part->dev);
    حرر(part);
    أرجع EFAULT;
  }

  pent = (PTABLE_داخلا*)&sec[0x1BE];
  pent = جاچ_حصا_ڪريو_جدول(pent, part->phys, &part->pno);

  إذا(!pent)
  {
    _dio_error = part->pno == 0xFE ? ERR_EMPTYPART :
                 part->pno == 0xFD ? ERR_LINUXSWAP : ERR_NOTEXT2FS;
    حرر(part->dev);
    حرر(part);
    أرجع ENODEV;
  }

  /*
   * Calculate the remaining figures
   */
  {
    بنانشان ڊگهو    fsec, fhead, fcyl;

    fsec = (بنانشان ڊگهو)(pent->شروعات_sec & 0x3F);
    fhead = (بنانشان ڊگهو)pent->شروعات_سر;
    fcyl = ((pent->شروعات_sec >> 6) << 8) + pent->شروعات_cyl;
    part->شروعات = fsec + fhead * part->sects + fcyl *
                  (part->heads * part->sects) - 1;
    part->len = pent->سائيز;
  }

  /*
   * Add the partition to the table
   */
  newparts = (حصا_ڪريو**)realloc(partitions, حجم(حصا_ڪريو) * npart);
  إذا (!newparts) {
	  حرر(part);
	  أرجع ENOMEM;
  }
  partitions = newparts;
  partitions[npart++] = سرگرم = part;

  /*
   * Now alloc all libe2fs structures
   */
  *channel = alloc_io_channel(سرگرم);
  إذا (!*channel)
	  أرجع ENOMEM;

  أرجع 0;
}

ساكن errcode_t dos_close(io_channel channel)
{
	حرر(channel->name);
	حرر(channel);

	أرجع 0;
}

ساكن errcode_t dos_سيٽ_blksize(io_channel channel, صحيح blksize)
{
  channel->بلاڪ_سائيز = blksize;

  أرجع 0;
}

ساكن errcode_t dos_read_blk(io_channel channel, بنانشان ڊگهو بلاڪ,
                                             صحيح ڳڻپ, فراغ *buf)
{
  حصا_ڪريو     *part;
  size_t        سائيز;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (حصا_ڪريو*)channel->ذاتي_معلومات;

  سائيز = (size_t)((ڳڻپ < 0) ? -ڳڻپ : ڳڻپ * channel->بلاڪ_سائيز);
  loc = (ext2_loff_t) بلاڪ * channel->بلاڪ_سائيز;

  lba2chs(loc, &chs, part);
  /*
   * Potential bug here:
   *   If DJGPP is used then reads of >18 sectors will fail!
   *   Have to rewrite biosdisk.
   */
  _dio_hw_error = biosdisk(DISK_READ,
                           part->phys,
                           chs.سر,
                           chs.cyl,
                           chs.sector,
                           سائيز < 512 ? 1 : سائيز/512,
                           buf);

  إذا(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    أرجع EFAULT;
  }

  أرجع 0;
}

ساكن errcode_t dos_write_blk(io_channel channel, بنانشان ڊگهو بلاڪ,
                               صحيح ڳڻپ, ثابت فراغ *buf)
{
  حصا_ڪريو     *part;
  size_t        سائيز;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (حصا_ڪريو*)channel->ذاتي_معلومات;

  إذا(ڳڻپ == 1)
    سائيز = (size_t)channel->بلاڪ_سائيز;
  وإلا
  {
    إذا (ڳڻپ < 0)
      سائيز = (size_t)-ڳڻپ;
    وإلا
      سائيز = (size_t)(ڳڻپ * channel->بلاڪ_سائيز);
  }

  loc = (ext2_loff_t)بلاڪ * channel->بلاڪ_سائيز;
  lba2chs(loc, &chs, part);
  _dio_hw_error = biosdisk(DISK_WRITE,
                           part->phys,
                           chs.سر,
                           chs.cyl,
                           chs.sector,
                           سائيز < 512 ? 1 : سائيز/512,
                           (فراغ*)buf);

  إذا(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    أرجع EFAULT;
  }

  أرجع 0;
}

#جيڪڏهن_وصف_ٿيل __TURBOC__
#هدايت argsused
#جيڪڏهن_پورو
ساكن errcode_t dos_flush(io_channel channel)
{
  /*
   * No buffers, no flush...
   */
  أرجع 0;
}
