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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <bios.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ctype.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <io.h>
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_ERRNO_H
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <ext2fs/ext2_types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dosio.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "et/com_err.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_err.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs/io.h"

/*
 * Some helper macros
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_EXT2FS       0x83
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LINUX_ਅਦਲਾ_ਬਦਲੀ         0x82
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WRITE_ERR(ਸੁਨੇਹਾ_2) ਲਿਖ(2, ਸੁਨੇਹਾ_2, ਲੜੀ_ਲੰਬਾਈ(ਸੁਨੇਹਾ_2))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WRITE_ERR_S(ਸੁਨੇਹਾ_2) ਲਿਖ(2, ਸੁਨੇਹਾ_2, ਆਕਾਰ(ਸੁਨੇਹਾ_2))

/*
 * Exported variables
 */
ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ        _dio_error;
ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ        _dio_hw_error;

/*
 * Array of all opened partitions
 */
ਸਥਿਰ ਹਿੱਸੇ_ਕਰੋ        **partitions = ਨਿਰਾ;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ   npart = 0; /* Number of mapped partitions */
ਸਥਿਰ ਹਿੱਸੇ_ਕਰੋ        *ਸਰਗਰਮ = ਨਿਰਾ;

/*
 * I/O Manager routine prototypes
 */
ਸਥਿਰ errcode_t dos_open(ਅਚਲ ਅੱਖਰ *dev, ਪੂਰਨ flags, io_channel *channel);
ਸਥਿਰ errcode_t dos_close(io_channel channel);
ਸਥਿਰ errcode_t dos_ਸੈੱਟ_blksize(io_channel channel, ਪੂਰਨ blksize);
ਸਥਿਰ errcode_t dos_read_blk(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
                                             ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *buf);
ਸਥਿਰ errcode_t dos_write_blk(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
                               ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *buf);
ਸਥਿਰ errcode_t dos_flush(io_channel channel);

ਸਥਿਰ ਬਣਤਰ struct_io_manager struct_dos_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "DOS I/O Manager",
	.open		= dos_open,
	.close		= dos_close,
	.ਸੈੱਟ_blksize	= dos_ਸੈੱਟ_blksize,
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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_CHECK_MAGIC(ਬਣਤਰ, ਕੋਡ) \
          ਜੇ ((ਬਣਤਰ)->magic != (ਕੋਡ)) ਵਾਪਸ (ਕੋਡ)

/*
 * Calculates a CHS address of a sector from its LBA
 * offset for the given partition.
 */
ਸਥਿਰ ਖਾਲੀ lba2chs(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ lba_addr, CHS *chs, ਹਿੱਸੇ_ਕਰੋ *part)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ      abss;

  chs->ਵਿੱਥ = lba_addr & 0x000001FF;
  abss = (lba_addr >> 9) + part->ਸ਼ੁਰੂ;
  chs->cyl    = abss / (part->sects * part->heads);
  chs->ਸਿਰ   = (abss / part->sects) % part->heads;
  chs->sector = (abss % part->sects) + 1;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __TURBOC__
#ਹਦਾਇਤ argsused
#ਜੇ_ਅੰਤ
/*
 * Scans the passed partition table looking for *pno partition
 * that has LINUX_EXT2FS type.
 *
 * TODO:
 * For partition numbers >5 Linux uses DOS extended partitions -
 * dive into them an return an appropriate entry. Also dive into
 * extended partitions when scanning for a first Linux/ext2fs.
 */
ਸਥਿਰ PTABLE_ਇੰਦਰਾਜ *ਜਾਂਚ_ਹਿੱਸੇ_ਕਰੋ_ਸਾਰਣੀ(PTABLE_ਇੰਦਰਾਜ *pentry,
                                          ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ phys,
                                          ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *pno)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ        i;

  ਜੇ(*pno != 0xFF && *pno >= 5)
     ਵਾਪਸ ਨਿਰਾ; /* We don't support extended partitions for now */

  ਜੇ(*pno != 0xFF)
  {
    ਜੇ(pentry[*pno].type == LINUX_EXT2FS)
      ਵਾਪਸ &pentry[*pno];
    ਨਹੀਂਤਾਂ
    {
      ਜੇ(!pentry[*pno].type)
        *pno = 0xFE;
      ਨਹੀਂਤਾਂ ਜੇ(pentry[*pno].type == LINUX_ਅਦਲਾ_ਬਦਲੀ)
        *pno = 0xFD;
      ਵਾਪਸ ਨਿਰਾ;
    }
  }

  ਲਈ(i = 0; i < 4; i++)
    ਜੇ(pentry[i].type == LINUX_EXT2FS)
    {
      *pno = i;
      ਵਾਪਸ &pentry[i];
    }

  ਵਾਪਸ ਨਿਰਾ;
}

/*
 * Allocate libext2fs structures associated with I/O manager
 */
ਸਥਿਰ io_channel alloc_io_channel(ਹਿੱਸੇ_ਕਰੋ *part)
{
  io_channel     ioch;

  ioch = (io_channel)ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(ਬਣਤਰ struct_io_channel));
  ਜੇ (!ioch)
	  ਵਾਪਸ ਨਿਰਾ;
  memset(ioch, 0, ਆਕਾਰ(ਬਣਤਰ struct_io_channel));
  ioch->magic = EXT2_ET_MAGIC_IO_CHANNEL;
  ioch->manager = dos_io_manager;
  ioch->name = (ਅੱਖਰ *)ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਲੜੀ_ਲੰਬਾਈ(part->dev)+1);
  ਜੇ (!ioch->name) {
	  ਮੁਕਤ(ioch);
	  ਵਾਪਸ ਨਿਰਾ;
  }
  strcpy(ioch->name, part->dev);
  ioch->ਨਿੱਜੀ_ਅੰਕੜੇ = part;
  ioch->ਖੰਡ_ਮਾਪ = 1024; /* The smallest ext2fs block size */
  ioch->read_error = 0;
  ioch->write_error = 0;

  ਵਾਪਸ ioch;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __TURBOC__
#ਹਦਾਇਤ argsused
#ਜੇ_ਅੰਤ
/*
 * Open the 'name' partition, initialize all information structures
 * we need to keep and create libext2fs I/O manager.
 */
ਸਥਿਰ errcode_t dos_open(ਅਚਲ ਅੱਖਰ *dev, ਪੂਰਨ flags, io_channel *channel)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ  *tmp, sec[512];
  ਹਿੱਸੇ_ਕਰੋ      *part;
  PTABLE_ਇੰਦਰਾਜ   *pent;
  ਹਿੱਸੇ_ਕਰੋ        **newparts;

  ਜੇ(!dev)
  {
    _dio_error = ERR_BADDEV;
    ਵਾਪਸ EXT2_ET_BAD_DEVICE_NAME;
  }

  /*
   * First check whether the dev name is OK
   */
  tmp = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*)strrchr(dev, '/');
  ਜੇ(!tmp)
  {
    _dio_error = ERR_BADDEV;
    ਵਾਪਸ EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp = 0;
  ਜੇ(strcmp(dev, "/dev"))
  {
    _dio_error = ERR_BADDEV;
    ਵਾਪਸ EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp++ = '/';

  /*
   * Check whether the partition data is already in cache
   */

  part = (ਹਿੱਸੇ_ਕਰੋ*)ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(ਹਿੱਸੇ_ਕਰੋ));
  ਜੇ (!part)
	  ਵਾਪਸ ENOMEM;
  {
    ਪੂਰਨ   i = 0;

    ਲਈ(;i < npart; i++)
      ਜੇ(!strcmp(partitions[i]->dev, dev))
      {
        /* Found it! Make it the active one */
        ਸਰਗਰਮ = partitions[i];
        *channel = alloc_io_channel(ਸਰਗਰਮ);
	ਜੇ (!*channel)
		ਵਾਪਸ ENOMEM;
        ਵਾਪਸ 0;
      }
  }

  /*
   * Drive number & optionally partn number
   */
  ਚੋਣ(tmp[0])
  {
    ਹਾਲਤ 'h':
    ਹਾਲਤ 's':
      part->phys = 0x80;
      part->phys += ਵੱਡਾ_ਅੱਖਰ(tmp[2]) - 'A';
      /*
       * Do we have the partition number?
       */
      ਜੇ(tmp[3])
        part->pno = ਅੰਕ_ਹੈ((ਪੂਰਨ)tmp[3]) ? tmp[3] - '0' - 1: 0;
      ਨਹੀਂਤਾਂ
        part->pno = 0xFF;
      ਰੋਕੋ;

    ਹਾਲਤ 'f':
      ਜੇ(tmp[2])
        part->phys = ਅੰਕ_ਹੈ((ਪੂਰਨ)tmp[2]) ? tmp[2] - '0' : 0;
      ਨਹੀਂਤਾਂ
        part->phys = 0x00; /* We'll assume /dev/fd0 */
      ਰੋਕੋ;

    ਮੂਲ:
      _dio_error = ERR_BADDEV;
      ਵਾਪਸ ENODEV;
  }

  ਜੇ(part->phys < 0x80)
  {
     /* We don't support floppies for now */
     _dio_error = ERR_NOTSUPP;
     ਵਾਪਸ EINVAL;
  }

  part->dev = strdup(dev);

  /*
   * Get drive's geometry
   */
  _dio_hw_error = biosdisk(DISK_GET_GEOMETRY,
                           part->phys,
                           0, /*ਸਿਰ*/
                           0, /* cylinder */
                           1, /* sector */
                           1, /* just one sector */
                           sec);

  ਜੇ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    ਮੁਕਤ(part->dev);
    ਮੁਕਤ(part);
    ਵਾਪਸ EFAULT;
  }

  /*
   * Calculate the geometry
   */
  part->cyls  = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ)(((sec[0] >> 6) << 8) + sec[1] + 1);
  part->heads = sec[3] + 1;
  part->sects = sec[0] & 0x3F;

  /*
   * Now that we know all we need, let's look for the partition
   */
  _dio_hw_error = biosdisk(DISK_READ, part->phys, 0, 0, 1, 1, sec);

  ਜੇ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    ਮੁਕਤ(part->dev);
    ਮੁਕਤ(part);
    ਵਾਪਸ EFAULT;
  }

  pent = (PTABLE_ਇੰਦਰਾਜ*)&sec[0x1BE];
  pent = ਜਾਂਚ_ਹਿੱਸੇ_ਕਰੋ_ਸਾਰਣੀ(pent, part->phys, &part->pno);

  ਜੇ(!pent)
  {
    _dio_error = part->pno == 0xFE ? ERR_EMPTYPART :
                 part->pno == 0xFD ? ERR_LINUXSWAP : ERR_NOTEXT2FS;
    ਮੁਕਤ(part->dev);
    ਮੁਕਤ(part);
    ਵਾਪਸ ENODEV;
  }

  /*
   * Calculate the remaining figures
   */
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ    fsec, fhead, fcyl;

    fsec = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)(pent->ਸ਼ੁਰੂ_sec & 0x3F);
    fhead = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)pent->ਸ਼ੁਰੂ_ਸਿਰ;
    fcyl = ((pent->ਸ਼ੁਰੂ_sec >> 6) << 8) + pent->ਸ਼ੁਰੂ_cyl;
    part->ਸ਼ੁਰੂ = fsec + fhead * part->sects + fcyl *
                  (part->heads * part->sects) - 1;
    part->len = pent->ਮਾਪ;
  }

  /*
   * Add the partition to the table
   */
  newparts = (ਹਿੱਸੇ_ਕਰੋ**)realloc(partitions, ਆਕਾਰ(ਹਿੱਸੇ_ਕਰੋ) * npart);
  ਜੇ (!newparts) {
	  ਮੁਕਤ(part);
	  ਵਾਪਸ ENOMEM;
  }
  partitions = newparts;
  partitions[npart++] = ਸਰਗਰਮ = part;

  /*
   * Now alloc all libe2fs structures
   */
  *channel = alloc_io_channel(ਸਰਗਰਮ);
  ਜੇ (!*channel)
	  ਵਾਪਸ ENOMEM;

  ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t dos_close(io_channel channel)
{
	ਮੁਕਤ(channel->name);
	ਮੁਕਤ(channel);

	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t dos_ਸੈੱਟ_blksize(io_channel channel, ਪੂਰਨ blksize)
{
  channel->ਖੰਡ_ਮਾਪ = blksize;

  ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t dos_read_blk(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
                                             ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *buf)
{
  ਹਿੱਸੇ_ਕਰੋ     *part;
  size_t        ਮਾਪ;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (ਹਿੱਸੇ_ਕਰੋ*)channel->ਨਿੱਜੀ_ਅੰਕੜੇ;

  ਮਾਪ = (size_t)((ਗਿਣਤੀ_2 < 0) ? -ਗਿਣਤੀ_2 : ਗਿਣਤੀ_2 * channel->ਖੰਡ_ਮਾਪ);
  loc = (ext2_loff_t) ਖੰਡ * channel->ਖੰਡ_ਮਾਪ;

  lba2chs(loc, &chs, part);
  /*
   * Potential bug here:
   *   If DJGPP is used then reads of >18 sectors will fail!
   *   Have to rewrite biosdisk.
   */
  _dio_hw_error = biosdisk(DISK_READ,
                           part->phys,
                           chs.ਸਿਰ,
                           chs.cyl,
                           chs.sector,
                           ਮਾਪ < 512 ? 1 : ਮਾਪ/512,
                           buf);

  ਜੇ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    ਵਾਪਸ EFAULT;
  }

  ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t dos_write_blk(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
                               ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *buf)
{
  ਹਿੱਸੇ_ਕਰੋ     *part;
  size_t        ਮਾਪ;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (ਹਿੱਸੇ_ਕਰੋ*)channel->ਨਿੱਜੀ_ਅੰਕੜੇ;

  ਜੇ(ਗਿਣਤੀ_2 == 1)
    ਮਾਪ = (size_t)channel->ਖੰਡ_ਮਾਪ;
  ਨਹੀਂਤਾਂ
  {
    ਜੇ (ਗਿਣਤੀ_2 < 0)
      ਮਾਪ = (size_t)-ਗਿਣਤੀ_2;
    ਨਹੀਂਤਾਂ
      ਮਾਪ = (size_t)(ਗਿਣਤੀ_2 * channel->ਖੰਡ_ਮਾਪ);
  }

  loc = (ext2_loff_t)ਖੰਡ * channel->ਖੰਡ_ਮਾਪ;
  lba2chs(loc, &chs, part);
  _dio_hw_error = biosdisk(DISK_WRITE,
                           part->phys,
                           chs.ਸਿਰ,
                           chs.cyl,
                           chs.sector,
                           ਮਾਪ < 512 ? 1 : ਮਾਪ/512,
                           (ਖਾਲੀ*)buf);

  ਜੇ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    ਵਾਪਸ EFAULT;
  }

  ਵਾਪਸ 0;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __TURBOC__
#ਹਦਾਇਤ argsused
#ਜੇ_ਅੰਤ
ਸਥਿਰ errcode_t dos_flush(io_channel channel)
{
  /*
   * No buffers, no flush...
   */
  ਵਾਪਸ 0;
}
