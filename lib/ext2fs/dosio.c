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
#که_تعریف_شوی HAVE_ERRNO_H
#ضمن <errno.h>
#د_که_پای

#ضمن <ext2fs/ext2_types.h>
#ضمن "utils.h"
#ضمن "dosio.h"
#ضمن "et/com_err.h"
#ضمن "ext2_err.h"
#ضمن "ext2fs/io.h"

/*
 * Some helper macros
 */
#تعریف_کړه LINUX_EXT2FS       0x83
#تعریف_کړه LINUX_بدلون         0x82
#تعریف_کړه WRITE_ERR(پیغام_2) اكتب(2, پیغام_2, مزي_اوږدوالی(پیغام_2))
#تعریف_کړه WRITE_ERR_S(پیغام_2) اكتب(2, پیغام_2, حجم(پیغام_2))

/*
 * Exported variables
 */
بینښه اوږد        _dio_error;
بینښه اوږد        _dio_hw_error;

/*
 * Array of all opened partitions
 */
ساكن برخه_کول        **partitions = عدم;
ساكن بینښه لنډ   npart = 0; /* Number of mapped partitions */
ساكن برخه_کول        *فعال = عدم;

/*
 * I/O Manager routine prototypes
 */
ساكن errcode_t dos_open(ثابت حرف *dev, صحيح flags, io_channel *channel);
ساكن errcode_t dos_close(io_channel channel);
ساكن errcode_t dos_سېټ_blksize(io_channel channel, صحيح blksize);
ساكن errcode_t dos_read_blk(io_channel channel, بینښه اوږد بلاک,
                                             صحيح شمېر, فراغ *buf);
ساكن errcode_t dos_write_blk(io_channel channel, بینښه اوږد بلاک,
                               صحيح شمېر, ثابت فراغ *buf);
ساكن errcode_t dos_flush(io_channel channel);

ساكن بنية struct_io_manager struct_dos_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "DOS I/O Manager",
	.open		= dos_open,
	.close		= dos_close,
	.سېټ_blksize	= dos_سېټ_blksize,
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

#تعریف_کړه EXT2_CHECK_MAGIC(بنية, کوډ) \
          إذا ((بنية)->magic != (کوډ)) أرجع (کوډ)

/*
 * Calculates a CHS address of a sector from its LBA
 * offset for the given partition.
 */
ساكن فراغ lba2chs(بینښه اوږد lba_addr, CHS *chs, برخه_کول *part)
{
  بینښه اوږد      abss;

  chs->دننه_واټن = lba_addr & 0x000001FF;
  abss = (lba_addr >> 9) + part->پیل;
  chs->cyl    = abss / (part->sects * part->heads);
  chs->سر_2   = (abss / part->sects) % part->heads;
  chs->sector = (abss % part->sects) + 1;
}

#که_تعریف_شوی __TURBOC__
#لارښوونه argsused
#د_که_پای
/*
 * Scans the passed partition table looking for *pno partition
 * that has LINUX_EXT2FS type.
 *
 * TODO:
 * For partition numbers >5 Linux uses DOS extended partitions -
 * dive into them an return an appropriate entry. Also dive into
 * extended partitions when scanning for a first Linux/ext2fs.
 */
ساكن PTABLE_ثبت *لټون_برخه_کول_جدول(PTABLE_ثبت *pentry,
                                          بینښه لنډ phys,
                                          بینښه حرف *pno)
{
  بینښه        i;

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
      وإلا إذا(pentry[*pno].type == LINUX_بدلون)
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
ساكن io_channel alloc_io_channel(برخه_کول *part)
{
  io_channel     ioch;

  ioch = (io_channel)حافظه_ورکړه(حجم(بنية struct_io_channel));
  إذا (!ioch)
	  أرجع عدم;
  memset(ioch, 0, حجم(بنية struct_io_channel));
  ioch->magic = EXT2_ET_MAGIC_IO_CHANNEL;
  ioch->manager = dos_io_manager;
  ioch->name = (حرف *)حافظه_ورکړه(مزي_اوږدوالی(part->dev)+1);
  إذا (!ioch->name) {
	  حرر(ioch);
	  أرجع عدم;
  }
  strcpy(ioch->name, part->dev);
  ioch->شخصي_معلومات = part;
  ioch->بلاک_اندازه = 1024; /* The smallest ext2fs block size */
  ioch->read_error = 0;
  ioch->write_error = 0;

  أرجع ioch;
}

#که_تعریف_شوی __TURBOC__
#لارښوونه argsused
#د_که_پای
/*
 * Open the 'name' partition, initialize all information structures
 * we need to keep and create libext2fs I/O manager.
 */
ساكن errcode_t dos_open(ثابت حرف *dev, صحيح flags, io_channel *channel)
{
  بینښه حرف  *tmp, sec[512];
  برخه_کول      *part;
  PTABLE_ثبت   *pent;
  برخه_کول        **newparts;

  إذا(!dev)
  {
    _dio_error = ERR_BADDEV;
    أرجع EXT2_ET_BAD_DEVICE_NAME;
  }

  /*
   * First check whether the dev name is OK
   */
  tmp = (بینښه حرف*)strrchr(dev, '/');
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

  part = (برخه_کول*)حافظه_ورکړه(حجم(برخه_کول));
  إذا (!part)
	  أرجع ENOMEM;
  {
    صحيح   i = 0;

    لكل(;i < npart; i++)
      إذا(!strcmp(partitions[i]->dev, dev))
      {
        /* Found it! Make it the active one */
        فعال = partitions[i];
        *channel = alloc_io_channel(فعال);
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
      part->phys += لوی_توری(tmp[2]) - 'A';
      /*
       * Do we have the partition number?
       */
      إذا(tmp[3])
        part->pno = که_عدد((صحيح)tmp[3]) ? tmp[3] - '0' - 1: 0;
      وإلا
        part->pno = 0xFF;
      اكسر;

    حالة 'f':
      إذا(tmp[2])
        part->phys = که_عدد((صحيح)tmp[2]) ? tmp[2] - '0' : 0;
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
  part->cyls  = (بینښه لنډ)(((sec[0] >> 6) << 8) + sec[1] + 1);
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

  pent = (PTABLE_ثبت*)&sec[0x1BE];
  pent = لټون_برخه_کول_جدول(pent, part->phys, &part->pno);

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
    بینښه اوږد    fsec, fhead, fcyl;

    fsec = (بینښه اوږد)(pent->پیل_sec & 0x3F);
    fhead = (بینښه اوږد)pent->پیل_سر;
    fcyl = ((pent->پیل_sec >> 6) << 8) + pent->پیل_cyl;
    part->پیل = fsec + fhead * part->sects + fcyl *
                  (part->heads * part->sects) - 1;
    part->len = pent->اندازه;
  }

  /*
   * Add the partition to the table
   */
  newparts = (برخه_کول**)realloc(partitions, حجم(برخه_کول) * npart);
  إذا (!newparts) {
	  حرر(part);
	  أرجع ENOMEM;
  }
  partitions = newparts;
  partitions[npart++] = فعال = part;

  /*
   * Now alloc all libe2fs structures
   */
  *channel = alloc_io_channel(فعال);
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

ساكن errcode_t dos_سېټ_blksize(io_channel channel, صحيح blksize)
{
  channel->بلاک_اندازه = blksize;

  أرجع 0;
}

ساكن errcode_t dos_read_blk(io_channel channel, بینښه اوږد بلاک,
                                             صحيح شمېر, فراغ *buf)
{
  برخه_کول     *part;
  size_t        اندازه;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (برخه_کول*)channel->شخصي_معلومات;

  اندازه = (size_t)((شمېر < 0) ? -شمېر : شمېر * channel->بلاک_اندازه);
  loc = (ext2_loff_t) بلاک * channel->بلاک_اندازه;

  lba2chs(loc, &chs, part);
  /*
   * Potential bug here:
   *   If DJGPP is used then reads of >18 sectors will fail!
   *   Have to rewrite biosdisk.
   */
  _dio_hw_error = biosdisk(DISK_READ,
                           part->phys,
                           chs.سر_2,
                           chs.cyl,
                           chs.sector,
                           اندازه < 512 ? 1 : اندازه/512,
                           buf);

  إذا(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    أرجع EFAULT;
  }

  أرجع 0;
}

ساكن errcode_t dos_write_blk(io_channel channel, بینښه اوږد بلاک,
                               صحيح شمېر, ثابت فراغ *buf)
{
  برخه_کول     *part;
  size_t        اندازه;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (برخه_کول*)channel->شخصي_معلومات;

  إذا(شمېر == 1)
    اندازه = (size_t)channel->بلاک_اندازه;
  وإلا
  {
    إذا (شمېر < 0)
      اندازه = (size_t)-شمېر;
    وإلا
      اندازه = (size_t)(شمېر * channel->بلاک_اندازه);
  }

  loc = (ext2_loff_t)بلاک * channel->بلاک_اندازه;
  lba2chs(loc, &chs, part);
  _dio_hw_error = biosdisk(DISK_WRITE,
                           part->phys,
                           chs.سر_2,
                           chs.cyl,
                           chs.sector,
                           اندازه < 512 ? 1 : اندازه/512,
                           (فراغ*)buf);

  إذا(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    أرجع EFAULT;
  }

  أرجع 0;
}

#که_تعریف_شوی __TURBOC__
#لارښوونه argsused
#د_که_پای
ساكن errcode_t dos_flush(io_channel channel)
{
  /*
   * No buffers, no flush...
   */
  أرجع 0;
}
