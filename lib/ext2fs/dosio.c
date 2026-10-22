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
#اگر_تعریف_شدہ HAVE_ERRNO_H
#شامل <errno.h>
#اگر_ختم

#شامل <ext2fs/ext2_types.h>
#شامل "utils.h"
#شامل "dosio.h"
#شامل "et/com_err.h"
#شامل "ext2_err.h"
#شامل "ext2fs/io.h"

/*
 * Some helper macros
 */
#تعریف_کرو LINUX_EXT2FS       0x83
#تعریف_کرو LINUX_تبادلہ         0x82
#تعریف_کرو WRITE_ERR(پیغام_2) اكتب(2, پیغام_2, لڑی_لمبائی(پیغام_2))
#تعریف_کرو WRITE_ERR_S(پیغام_2) اكتب(2, پیغام_2, حجم(پیغام_2))

/*
 * Exported variables
 */
غیرعلامتی طویل        _dio_error;
غیرعلامتی طویل        _dio_hw_error;

/*
 * Array of all opened partitions
 */
ساکن تقسیم        **partitions = تہی;
ساکن غیرعلامتی مختصر   npart = 0; /* Number of mapped partitions */
ساکن تقسیم        *فعال = تہی;

/*
 * I/O Manager routine prototypes
 */
ساکن errcode_t dos_open(ثابت حرف *dev, صحیح flags, io_channel *channel);
ساکن errcode_t dos_close(io_channel channel);
ساکن errcode_t dos_سیٹ_blksize(io_channel channel, صحیح blksize);
ساکن errcode_t dos_read_blk(io_channel channel, غیرعلامتی طویل بلاک,
                                             صحیح گنتی, خالی *buf);
ساکن errcode_t dos_write_blk(io_channel channel, غیرعلامتی طویل بلاک,
                               صحیح گنتی, ثابت خالی *buf);
ساکن errcode_t dos_flush(io_channel channel);

ساکن ساخت struct_io_manager struct_dos_manager = {
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

#تعریف_کرو EXT2_CHECK_MAGIC(ساخت, رمز) \
          اگر ((ساخت)->magic != (رمز)) واپس (رمز)

/*
 * Calculates a CHS address of a sector from its LBA
 * offset for the given partition.
 */
ساکن خالی lba2chs(غیرعلامتی طویل lba_addr, CHS *chs, تقسیم *part)
{
  غیرعلامتی طویل      abss;

  chs->انحراف = lba_addr & 0x000001FF;
  abss = (lba_addr >> 9) + part->آغاز;
  chs->cyl    = abss / (part->sects * part->heads);
  chs->سر   = (abss / part->sects) % part->heads;
  chs->sector = (abss % part->sects) + 1;
}

#اگر_تعریف_شدہ __TURBOC__
#ہدایت argsused
#اگر_ختم
/*
 * Scans the passed partition table looking for *pno partition
 * that has LINUX_EXT2FS type.
 *
 * TODO:
 * For partition numbers >5 Linux uses DOS extended partitions -
 * dive into them an return an appropriate entry. Also dive into
 * extended partitions when scanning for a first Linux/ext2fs.
 */
ساکن PTABLE_اندراج *جائزہ_تقسیم_جدول(PTABLE_اندراج *pentry,
                                          غیرعلامتی مختصر phys,
                                          غیرعلامتی حرف *pno)
{
  غیرعلامتی        i;

  اگر(*pno != 0xFF && *pno >= 5)
     واپس تہی; /* We don't support extended partitions for now */

  اگر(*pno != 0xFF)
  {
    اگر(pentry[*pno].type == LINUX_EXT2FS)
      واپس &pentry[*pno];
    ورنہ
    {
      اگر(!pentry[*pno].type)
        *pno = 0xFE;
      ورنہ اگر(pentry[*pno].type == LINUX_تبادلہ)
        *pno = 0xFD;
      واپس تہی;
    }
  }

  برائے(i = 0; i < 4; i++)
    اگر(pentry[i].type == LINUX_EXT2FS)
    {
      *pno = i;
      واپس &pentry[i];
    }

  واپس تہی;
}

/*
 * Allocate libext2fs structures associated with I/O manager
 */
ساکن io_channel alloc_io_channel(تقسیم *part)
{
  io_channel     ioch;

  ioch = (io_channel)حافظہ_دو(حجم(ساخت struct_io_channel));
  اگر (!ioch)
	  واپس تہی;
  memset(ioch, 0, حجم(ساخت struct_io_channel));
  ioch->magic = EXT2_ET_MAGIC_IO_CHANNEL;
  ioch->manager = dos_io_manager;
  ioch->name = (حرف *)حافظہ_دو(لڑی_لمبائی(part->dev)+1);
  اگر (!ioch->name) {
	  آزاد(ioch);
	  واپس تہی;
  }
  strcpy(ioch->name, part->dev);
  ioch->نجی_ڈیٹا = part;
  ioch->بلاک_سائز = 1024; /* The smallest ext2fs block size */
  ioch->read_error = 0;
  ioch->write_error = 0;

  واپس ioch;
}

#اگر_تعریف_شدہ __TURBOC__
#ہدایت argsused
#اگر_ختم
/*
 * Open the 'name' partition, initialize all information structures
 * we need to keep and create libext2fs I/O manager.
 */
ساکن errcode_t dos_open(ثابت حرف *dev, صحیح flags, io_channel *channel)
{
  غیرعلامتی حرف  *tmp, sec[512];
  تقسیم      *part;
  PTABLE_اندراج   *pent;
  تقسیم        **newparts;

  اگر(!dev)
  {
    _dio_error = ERR_BADDEV;
    واپس EXT2_ET_BAD_DEVICE_NAME;
  }

  /*
   * First check whether the dev name is OK
   */
  tmp = (غیرعلامتی حرف*)strrchr(dev, '/');
  اگر(!tmp)
  {
    _dio_error = ERR_BADDEV;
    واپس EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp = 0;
  اگر(strcmp(dev, "/dev"))
  {
    _dio_error = ERR_BADDEV;
    واپس EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp++ = '/';

  /*
   * Check whether the partition data is already in cache
   */

  part = (تقسیم*)حافظہ_دو(حجم(تقسیم));
  اگر (!part)
	  واپس ENOMEM;
  {
    صحیح   i = 0;

    برائے(;i < npart; i++)
      اگر(!strcmp(partitions[i]->dev, dev))
      {
        /* Found it! Make it the active one */
        فعال = partitions[i];
        *channel = alloc_io_channel(فعال);
	اگر (!*channel)
		واپس ENOMEM;
        واپس 0;
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
      part->phys += بڑا_حرف(tmp[2]) - 'A';
      /*
       * Do we have the partition number?
       */
      اگر(tmp[3])
        part->pno = ہندسہ_ہے((صحیح)tmp[3]) ? tmp[3] - '0' - 1: 0;
      ورنہ
        part->pno = 0xFF;
      توڑو;

    حالت 'f':
      اگر(tmp[2])
        part->phys = ہندسہ_ہے((صحیح)tmp[2]) ? tmp[2] - '0' : 0;
      ورنہ
        part->phys = 0x00; /* We'll assume /dev/fd0 */
      توڑو;

    طےشدہ:
      _dio_error = ERR_BADDEV;
      واپس ENODEV;
  }

  اگر(part->phys < 0x80)
  {
     /* We don't support floppies for now */
     _dio_error = ERR_NOTSUPP;
     واپس EINVAL;
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

  اگر(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    آزاد(part->dev);
    آزاد(part);
    واپس EFAULT;
  }

  /*
   * Calculate the geometry
   */
  part->cyls  = (غیرعلامتی مختصر)(((sec[0] >> 6) << 8) + sec[1] + 1);
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
    واپس EFAULT;
  }

  pent = (PTABLE_اندراج*)&sec[0x1BE];
  pent = جائزہ_تقسیم_جدول(pent, part->phys, &part->pno);

  اگر(!pent)
  {
    _dio_error = part->pno == 0xFE ? ERR_EMPTYPART :
                 part->pno == 0xFD ? ERR_LINUXSWAP : ERR_NOTEXT2FS;
    آزاد(part->dev);
    آزاد(part);
    واپس ENODEV;
  }

  /*
   * Calculate the remaining figures
   */
  {
    غیرعلامتی طویل    fsec, fhead, fcyl;

    fsec = (غیرعلامتی طویل)(pent->آغاز_sec & 0x3F);
    fhead = (غیرعلامتی طویل)pent->آغاز_سر;
    fcyl = ((pent->آغاز_sec >> 6) << 8) + pent->آغاز_cyl;
    part->آغاز = fsec + fhead * part->sects + fcyl *
                  (part->heads * part->sects) - 1;
    part->len = pent->سائز;
  }

  /*
   * Add the partition to the table
   */
  newparts = (تقسیم**)realloc(partitions, حجم(تقسیم) * npart);
  اگر (!newparts) {
	  آزاد(part);
	  واپس ENOMEM;
  }
  partitions = newparts;
  partitions[npart++] = فعال = part;

  /*
   * Now alloc all libe2fs structures
   */
  *channel = alloc_io_channel(فعال);
  اگر (!*channel)
	  واپس ENOMEM;

  واپس 0;
}

ساکن errcode_t dos_close(io_channel channel)
{
	آزاد(channel->name);
	آزاد(channel);

	واپس 0;
}

ساکن errcode_t dos_سیٹ_blksize(io_channel channel, صحیح blksize)
{
  channel->بلاک_سائز = blksize;

  واپس 0;
}

ساکن errcode_t dos_read_blk(io_channel channel, غیرعلامتی طویل بلاک,
                                             صحیح گنتی, خالی *buf)
{
  تقسیم     *part;
  size_t        سائز;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (تقسیم*)channel->نجی_ڈیٹا;

  سائز = (size_t)((گنتی < 0) ? -گنتی : گنتی * channel->بلاک_سائز);
  loc = (ext2_loff_t) بلاک * channel->بلاک_سائز;

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
                           سائز < 512 ? 1 : سائز/512,
                           buf);

  اگر(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    واپس EFAULT;
  }

  واپس 0;
}

ساکن errcode_t dos_write_blk(io_channel channel, غیرعلامتی طویل بلاک,
                               صحیح گنتی, ثابت خالی *buf)
{
  تقسیم     *part;
  size_t        سائز;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (تقسیم*)channel->نجی_ڈیٹا;

  اگر(گنتی == 1)
    سائز = (size_t)channel->بلاک_سائز;
  ورنہ
  {
    اگر (گنتی < 0)
      سائز = (size_t)-گنتی;
    ورنہ
      سائز = (size_t)(گنتی * channel->بلاک_سائز);
  }

  loc = (ext2_loff_t)بلاک * channel->بلاک_سائز;
  lba2chs(loc, &chs, part);
  _dio_hw_error = biosdisk(DISK_WRITE,
                           part->phys,
                           chs.سر,
                           chs.cyl,
                           chs.sector,
                           سائز < 512 ? 1 : سائز/512,
                           (خالی*)buf);

  اگر(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    واپس EFAULT;
  }

  واپس 0;
}

#اگر_تعریف_شدہ __TURBOC__
#ہدایت argsused
#اگر_ختم
ساکن errcode_t dos_flush(io_channel channel)
{
  /*
   * No buffers, no flush...
   */
  واپس 0;
}
