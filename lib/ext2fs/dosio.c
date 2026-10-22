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
#جے_تعریف_شدہ HAVE_ERRNO_H
#ضمن <errno.h>
#جے_انت

#ضمن <ext2fs/ext2_types.h>
#ضمن "utils.h"
#ضمن "dosio.h"
#ضمن "et/com_err.h"
#ضمن "ext2_err.h"
#ضمن "ext2fs/io.h"

/*
 * Some helper macros
 */
#تعریف_کرو LINUX_EXT2FS       0x83
#تعریف_کرو LINUX_وٹاندرا         0x82
#تعریف_کرو WRITE_ERR(پیغام_2) اكتب(2, پیغام_2, لڑی_لمبائی(پیغام_2))
#تعریف_کرو WRITE_ERR_S(پیغام_2) اكتب(2, پیغام_2, حجم(پیغام_2))

/*
 * Exported variables
 */
بنانشانی لما        _dio_error;
بنانشانی لما        _dio_hw_error;

/*
 * Array of all opened partitions
 */
ساكن حصے_کرو        **partitions = عدم;
ساكن بنانشانی چھوٹا   npart = 0; /* Number of mapped partitions */
ساكن حصے_کرو        *سرگرم = عدم;

/*
 * I/O Manager routine prototypes
 */
ساكن errcode_t dos_open(ثابت حرف *dev, صحيح flags, io_channel *channel);
ساكن errcode_t dos_close(io_channel channel);
ساكن errcode_t dos_سیٹ_blksize(io_channel channel, صحيح blksize);
ساكن errcode_t dos_read_blk(io_channel channel, بنانشانی لما بلاک,
                                             صحيح گنتی, فراغ *buf);
ساكن errcode_t dos_write_blk(io_channel channel, بنانشانی لما بلاک,
                               صحيح گنتی, ثابت فراغ *buf);
ساكن errcode_t dos_flush(io_channel channel);

ساكن بنية struct_io_manager struct_dos_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "DOS I/O Manager",
	.open		= dos_open,
	.close		= dos_close,
	.سیٹ_blksize	= dos_سیٹ_blksize,
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

#تعریف_کرو EXT2_CHECK_MAGIC(بنية, کوڈ) \
          إذا ((بنية)->magic != (کوڈ)) أرجع (کوڈ)

/*
 * Calculates a CHS address of a sector from its LBA
 * offset for the given partition.
 */
ساكن فراغ lba2chs(بنانشانی لما lba_addr, CHS *chs, حصے_کرو *part)
{
  بنانشانی لما      abss;

  chs->وتھ = lba_addr & 0x000001FF;
  abss = (lba_addr >> 9) + part->شروع;
  chs->cyl    = abss / (part->sects * part->heads);
  chs->سر   = (abss / part->sects) % part->heads;
  chs->sector = (abss % part->sects) + 1;
}

#جے_تعریف_شدہ __TURBOC__
#ہدایت argsused
#جے_انت
/*
 * Scans the passed partition table looking for *pno partition
 * that has LINUX_EXT2FS type.
 *
 * TODO:
 * For partition numbers >5 Linux uses DOS extended partitions -
 * dive into them an return an appropriate entry. Also dive into
 * extended partitions when scanning for a first Linux/ext2fs.
 */
ساكن PTABLE_اندراج *جانچ_حصے_کرو_جدول(PTABLE_اندراج *pentry,
                                          بنانشانی چھوٹا phys,
                                          بنانشانی حرف *pno)
{
  بنانشانی        i;

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
      وإلا إذا(pentry[*pno].type == LINUX_وٹاندرا)
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
ساكن io_channel alloc_io_channel(حصے_کرو *part)
{
  io_channel     ioch;

  ioch = (io_channel)حافظہ_دیو(حجم(بنية struct_io_channel));
  إذا (!ioch)
	  أرجع عدم;
  memset(ioch, 0, حجم(بنية struct_io_channel));
  ioch->magic = EXT2_ET_MAGIC_IO_CHANNEL;
  ioch->manager = dos_io_manager;
  ioch->name = (حرف *)حافظہ_دیو(لڑی_لمبائی(part->dev)+1);
  إذا (!ioch->name) {
	  حرر(ioch);
	  أرجع عدم;
  }
  strcpy(ioch->name, part->dev);
  ioch->نجی_انکڑے = part;
  ioch->بلاک_ناپ = 1024; /* The smallest ext2fs block size */
  ioch->read_error = 0;
  ioch->write_error = 0;

  أرجع ioch;
}

#جے_تعریف_شدہ __TURBOC__
#ہدایت argsused
#جے_انت
/*
 * Open the 'name' partition, initialize all information structures
 * we need to keep and create libext2fs I/O manager.
 */
ساكن errcode_t dos_open(ثابت حرف *dev, صحيح flags, io_channel *channel)
{
  بنانشانی حرف  *tmp, sec[512];
  حصے_کرو      *part;
  PTABLE_اندراج   *pent;
  حصے_کرو        **newparts;

  إذا(!dev)
  {
    _dio_error = ERR_BADDEV;
    أرجع EXT2_ET_BAD_DEVICE_NAME;
  }

  /*
   * First check whether the dev name is OK
   */
  tmp = (بنانشانی حرف*)strrchr(dev, '/');
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

  part = (حصے_کرو*)حافظہ_دیو(حجم(حصے_کرو));
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
      part->phys += وڈا_حرف(tmp[2]) - 'A';
      /*
       * Do we have the partition number?
       */
      إذا(tmp[3])
        part->pno = ہندسہ_اے((صحيح)tmp[3]) ? tmp[3] - '0' - 1: 0;
      وإلا
        part->pno = 0xFF;
      اكسر;

    حالة 'f':
      إذا(tmp[2])
        part->phys = ہندسہ_اے((صحيح)tmp[2]) ? tmp[2] - '0' : 0;
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
  part->cyls  = (بنانشانی چھوٹا)(((sec[0] >> 6) << 8) + sec[1] + 1);
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

  pent = (PTABLE_اندراج*)&sec[0x1BE];
  pent = جانچ_حصے_کرو_جدول(pent, part->phys, &part->pno);

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
    بنانشانی لما    fsec, fhead, fcyl;

    fsec = (بنانشانی لما)(pent->شروع_sec & 0x3F);
    fhead = (بنانشانی لما)pent->شروع_سر;
    fcyl = ((pent->شروع_sec >> 6) << 8) + pent->شروع_cyl;
    part->شروع = fsec + fhead * part->sects + fcyl *
                  (part->heads * part->sects) - 1;
    part->len = pent->ناپ;
  }

  /*
   * Add the partition to the table
   */
  newparts = (حصے_کرو**)realloc(partitions, حجم(حصے_کرو) * npart);
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

ساكن errcode_t dos_سیٹ_blksize(io_channel channel, صحيح blksize)
{
  channel->بلاک_ناپ = blksize;

  أرجع 0;
}

ساكن errcode_t dos_read_blk(io_channel channel, بنانشانی لما بلاک,
                                             صحيح گنتی, فراغ *buf)
{
  حصے_کرو     *part;
  size_t        ناپ;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (حصے_کرو*)channel->نجی_انکڑے;

  ناپ = (size_t)((گنتی < 0) ? -گنتی : گنتی * channel->بلاک_ناپ);
  loc = (ext2_loff_t) بلاک * channel->بلاک_ناپ;

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
                           ناپ < 512 ? 1 : ناپ/512,
                           buf);

  إذا(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    أرجع EFAULT;
  }

  أرجع 0;
}

ساكن errcode_t dos_write_blk(io_channel channel, بنانشانی لما بلاک,
                               صحيح گنتی, ثابت فراغ *buf)
{
  حصے_کرو     *part;
  size_t        ناپ;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (حصے_کرو*)channel->نجی_انکڑے;

  إذا(گنتی == 1)
    ناپ = (size_t)channel->بلاک_ناپ;
  وإلا
  {
    إذا (گنتی < 0)
      ناپ = (size_t)-گنتی;
    وإلا
      ناپ = (size_t)(گنتی * channel->بلاک_ناپ);
  }

  loc = (ext2_loff_t)بلاک * channel->بلاک_ناپ;
  lba2chs(loc, &chs, part);
  _dio_hw_error = biosdisk(DISK_WRITE,
                           part->phys,
                           chs.سر,
                           chs.cyl,
                           chs.sector,
                           ناپ < 512 ? 1 : ناپ/512,
                           (فراغ*)buf);

  إذا(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    أرجع EFAULT;
  }

  أرجع 0;
}

#جے_تعریف_شدہ __TURBOC__
#ہدایت argsused
#جے_انت
ساكن errcode_t dos_flush(io_channel channel)
{
  /*
   * No buffers, no flush...
   */
  أرجع 0;
}
