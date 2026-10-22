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
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <ext2fs/ext2_types.h>
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "dosio.h"
#ಅನ್ತರ್ಭಾವ "et/com_err.h"
#ಅನ್ತರ್ಭಾವ "ext2_err.h"
#ಅನ್ತರ್ಭಾವ "ext2fs/io.h"

/*
 * Some helper macros
 */
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_EXT2FS       0x83
#ವ್ಯಾಖ್ಯಾನಿಸು LINUX_ಅದಲುಬದಲು         0x82
#ವ್ಯಾಖ್ಯಾನಿಸು WRITE_ERR(ಸಂದೇಶ_2) ಲಿಖ(2, ಸಂದೇಶ_2, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(ಸಂದೇಶ_2))
#ವ್ಯಾಖ್ಯಾನಿಸು WRITE_ERR_S(ಸಂದೇಶ_2) ಲಿಖ(2, ಸಂದೇಶ_2, ಗಾತ್ರ(ಸಂದೇಶ_2))

/*
 * Exported variables
 */
ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ        _dio_error;
ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ        _dio_hw_error;

/*
 * Array of all opened partitions
 */
ಸ್ಥಿರ ವಿಭಜನೆ        **partitions = ಖಾಲಿ;
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ   npart = 0; /* Number of mapped partitions */
ಸ್ಥಿರ ವಿಭಜನೆ        *ಸಕ್ರಿಯ = ಖಾಲಿ;

/*
 * I/O Manager routine prototypes
 */
ಸ್ಥಿರ errcode_t dos_open(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dev, ಪೂರ್ಣಾಂಕ flags, io_channel *channel);
ಸ್ಥಿರ errcode_t dos_close(io_channel channel);
ಸ್ಥಿರ errcode_t dos_ಗಣ_blksize(io_channel channel, ಪೂರ್ಣಾಂಕ blksize);
ಸ್ಥಿರ errcode_t dos_read_blk(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
                                             ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *buf);
ಸ್ಥಿರ errcode_t dos_write_blk(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
                               ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf);
ಸ್ಥಿರ errcode_t dos_flush(io_channel channel);

ಸ್ಥಿರ ರಚನೆ struct_io_manager struct_dos_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "DOS I/O Manager",
	.open		= dos_open,
	.close		= dos_close,
	.ಗಣ_blksize	= dos_ಗಣ_blksize,
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

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_CHECK_MAGIC(ರಚನೆ, ಸಂಕೇತ_2) \
          ಆದರೆ ((ರಚನೆ)->magic != (ಸಂಕೇತ_2)) ಹಿಂತಿರುಗಿ (ಸಂಕೇತ_2)

/*
 * Calculates a CHS address of a sector from its LBA
 * offset for the given partition.
 */
ಸ್ಥಿರ ಶೂನ್ಯ lba2chs(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ lba_addr, CHS *chs, ವಿಭಜನೆ *part)
{
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ      abss;

  chs->ಸ್ಥಾನಾಂತರ = lba_addr & 0x000001FF;
  abss = (lba_addr >> 9) + part->ಆರಂಭ;
  chs->cyl    = abss / (part->sects * part->heads);
  chs->ತಲೆ   = (abss / part->sects) % part->heads;
  chs->sector = (abss % part->sects) + 1;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __TURBOC__
#ಸೂಚನೆ argsused
#ಆದರೆ_ಅಂತ್ಯ
/*
 * Scans the passed partition table looking for *pno partition
 * that has LINUX_EXT2FS type.
 *
 * TODO:
 * For partition numbers >5 Linux uses DOS extended partitions -
 * dive into them an return an appropriate entry. Also dive into
 * extended partitions when scanning for a first Linux/ext2fs.
 */
ಸ್ಥಿರ PTABLE_ನಮೂದು *ಪರಿಶೀಲನೆ_ವಿಭಜನೆ_ಕೋಷ್ಟಕ(PTABLE_ನಮೂದು *pentry,
                                          ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ phys,
                                          ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *pno)
{
  ಚಿಹ್ನೆರಹಿತ        i;

  ಆದರೆ(*pno != 0xFF && *pno >= 5)
     ಹಿಂತಿರುಗಿ ಖಾಲಿ; /* We don't support extended partitions for now */

  ಆದರೆ(*pno != 0xFF)
  {
    ಆದರೆ(pentry[*pno].type == LINUX_EXT2FS)
      ಹಿಂತಿರುಗಿ &pentry[*pno];
    ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ(!pentry[*pno].type)
        *pno = 0xFE;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(pentry[*pno].type == LINUX_ಅದಲುಬದಲು)
        *pno = 0xFD;
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }
  }

  ಗಾಗಿ(i = 0; i < 4; i++)
    ಆದರೆ(pentry[i].type == LINUX_EXT2FS)
    {
      *pno = i;
      ಹಿಂತಿರುಗಿ &pentry[i];
    }

  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

/*
 * Allocate libext2fs structures associated with I/O manager
 */
ಸ್ಥಿರ io_channel alloc_io_channel(ವಿಭಜನೆ *part)
{
  io_channel     ioch;

  ioch = (io_channel)ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(ರಚನೆ struct_io_channel));
  ಆದರೆ (!ioch)
	  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
  memset(ioch, 0, ಗಾತ್ರ(ರಚನೆ struct_io_channel));
  ioch->magic = EXT2_ET_MAGIC_IO_CHANNEL;
  ioch->manager = dos_io_manager;
  ioch->name = (ಅಕ್ಷರ *)ಸ್ಮರಣೆ_ಕೊಡು(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(part->dev)+1);
  ಆದರೆ (!ioch->name) {
	  ಬಿಡುಗಡೆ(ioch);
	  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
  }
  strcpy(ioch->name, part->dev);
  ioch->ಖಾಸಗಿ_ದತ್ತಾಂಶ = part;
  ioch->ಖಂಡ_ಗಾತ್ರ = 1024; /* The smallest ext2fs block size */
  ioch->read_error = 0;
  ioch->write_error = 0;

  ಹಿಂತಿರುಗಿ ioch;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __TURBOC__
#ಸೂಚನೆ argsused
#ಆದರೆ_ಅಂತ್ಯ
/*
 * Open the 'name' partition, initialize all information structures
 * we need to keep and create libext2fs I/O manager.
 */
ಸ್ಥಿರ errcode_t dos_open(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *dev, ಪೂರ್ಣಾಂಕ flags, io_channel *channel)
{
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ  *tmp, sec[512];
  ವಿಭಜನೆ      *part;
  PTABLE_ನಮೂದು   *pent;
  ವಿಭಜನೆ        **newparts;

  ಆದರೆ(!dev)
  {
    _dio_error = ERR_BADDEV;
    ಹಿಂತಿರುಗಿ EXT2_ET_BAD_DEVICE_NAME;
  }

  /*
   * First check whether the dev name is OK
   */
  tmp = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ*)strrchr(dev, '/');
  ಆದರೆ(!tmp)
  {
    _dio_error = ERR_BADDEV;
    ಹಿಂತಿರುಗಿ EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp = 0;
  ಆದರೆ(strcmp(dev, "/dev"))
  {
    _dio_error = ERR_BADDEV;
    ಹಿಂತಿರುಗಿ EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp++ = '/';

  /*
   * Check whether the partition data is already in cache
   */

  part = (ವಿಭಜನೆ*)ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(ವಿಭಜನೆ));
  ಆದರೆ (!part)
	  ಹಿಂತಿರುಗಿ ENOMEM;
  {
    ಪೂರ್ಣಾಂಕ   i = 0;

    ಗಾಗಿ(;i < npart; i++)
      ಆದರೆ(!strcmp(partitions[i]->dev, dev))
      {
        /* Found it! Make it the active one */
        ಸಕ್ರಿಯ = partitions[i];
        *channel = alloc_io_channel(ಸಕ್ರಿಯ);
	ಆದರೆ (!*channel)
		ಹಿಂತಿರುಗಿ ENOMEM;
        ಹಿಂತಿರುಗಿ 0;
      }
  }

  /*
   * Drive number & optionally partn number
   */
  ಆಯ್ಕೆ(tmp[0])
  {
    ಸ್ಥಿತಿ 'h':
    ಸ್ಥಿತಿ 's':
      part->phys = 0x80;
      part->phys += ದೊಡ್ಡ_ಅಕ್ಷರ(tmp[2]) - 'A';
      /*
       * Do we have the partition number?
       */
      ಆದರೆ(tmp[3])
        part->pno = ಅಂಕಿಯೇ((ಪೂರ್ಣಾಂಕ)tmp[3]) ? tmp[3] - '0' - 1: 0;
      ಇಲ್ಲದಿದ್ದರೆ
        part->pno = 0xFF;
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'f':
      ಆದರೆ(tmp[2])
        part->phys = ಅಂಕಿಯೇ((ಪೂರ್ಣಾಂಕ)tmp[2]) ? tmp[2] - '0' : 0;
      ಇಲ್ಲದಿದ್ದರೆ
        part->phys = 0x00; /* We'll assume /dev/fd0 */
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
      _dio_error = ERR_BADDEV;
      ಹಿಂತಿರುಗಿ ENODEV;
  }

  ಆದರೆ(part->phys < 0x80)
  {
     /* We don't support floppies for now */
     _dio_error = ERR_NOTSUPP;
     ಹಿಂತಿರುಗಿ EINVAL;
  }

  part->dev = strdup(dev);

  /*
   * Get drive's geometry
   */
  _dio_hw_error = biosdisk(DISK_GET_GEOMETRY,
                           part->phys,
                           0, /*ತಲೆ*/
                           0, /* cylinder */
                           1, /* sector */
                           1, /* just one sector */
                           sec);

  ಆದರೆ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    ಬಿಡುಗಡೆ(part->dev);
    ಬಿಡುಗಡೆ(part);
    ಹಿಂತಿರುಗಿ EFAULT;
  }

  /*
   * Calculate the geometry
   */
  part->cyls  = (ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ)(((sec[0] >> 6) << 8) + sec[1] + 1);
  part->heads = sec[3] + 1;
  part->sects = sec[0] & 0x3F;

  /*
   * Now that we know all we need, let's look for the partition
   */
  _dio_hw_error = biosdisk(DISK_READ, part->phys, 0, 0, 1, 1, sec);

  ಆದರೆ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    ಬಿಡುಗಡೆ(part->dev);
    ಬಿಡುಗಡೆ(part);
    ಹಿಂತಿರುಗಿ EFAULT;
  }

  pent = (PTABLE_ನಮೂದು*)&sec[0x1BE];
  pent = ಪರಿಶೀಲನೆ_ವಿಭಜನೆ_ಕೋಷ್ಟಕ(pent, part->phys, &part->pno);

  ಆದರೆ(!pent)
  {
    _dio_error = part->pno == 0xFE ? ERR_EMPTYPART :
                 part->pno == 0xFD ? ERR_LINUXSWAP : ERR_NOTEXT2FS;
    ಬಿಡುಗಡೆ(part->dev);
    ಬಿಡುಗಡೆ(part);
    ಹಿಂತಿರುಗಿ ENODEV;
  }

  /*
   * Calculate the remaining figures
   */
  {
    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ    fsec, fhead, fcyl;

    fsec = (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)(pent->ಆರಂಭ_sec & 0x3F);
    fhead = (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)pent->ಆರಂಭ_ತಲೆ;
    fcyl = ((pent->ಆರಂಭ_sec >> 6) << 8) + pent->ಆರಂಭ_cyl;
    part->ಆರಂಭ = fsec + fhead * part->sects + fcyl *
                  (part->heads * part->sects) - 1;
    part->len = pent->ಗಾತ್ರ_2;
  }

  /*
   * Add the partition to the table
   */
  newparts = (ವಿಭಜನೆ**)realloc(partitions, ಗಾತ್ರ(ವಿಭಜನೆ) * npart);
  ಆದರೆ (!newparts) {
	  ಬಿಡುಗಡೆ(part);
	  ಹಿಂತಿರುಗಿ ENOMEM;
  }
  partitions = newparts;
  partitions[npart++] = ಸಕ್ರಿಯ = part;

  /*
   * Now alloc all libe2fs structures
   */
  *channel = alloc_io_channel(ಸಕ್ರಿಯ);
  ಆದರೆ (!*channel)
	  ಹಿಂತಿರುಗಿ ENOMEM;

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t dos_close(io_channel channel)
{
	ಬಿಡುಗಡೆ(channel->name);
	ಬಿಡುಗಡೆ(channel);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t dos_ಗಣ_blksize(io_channel channel, ಪೂರ್ಣಾಂಕ blksize)
{
  channel->ಖಂಡ_ಗಾತ್ರ = blksize;

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t dos_read_blk(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
                                             ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *buf)
{
  ವಿಭಜನೆ     *part;
  size_t        ಗಾತ್ರ_2;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (ವಿಭಜನೆ*)channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;

  ಗಾತ್ರ_2 = (size_t)((ಎಣಿಕೆ_2 < 0) ? -ಎಣಿಕೆ_2 : ಎಣಿಕೆ_2 * channel->ಖಂಡ_ಗಾತ್ರ);
  loc = (ext2_loff_t) ಖಂಡ * channel->ಖಂಡ_ಗಾತ್ರ;

  lba2chs(loc, &chs, part);
  /*
   * Potential bug here:
   *   If DJGPP is used then reads of >18 sectors will fail!
   *   Have to rewrite biosdisk.
   */
  _dio_hw_error = biosdisk(DISK_READ,
                           part->phys,
                           chs.ತಲೆ,
                           chs.cyl,
                           chs.sector,
                           ಗಾತ್ರ_2 < 512 ? 1 : ಗಾತ್ರ_2/512,
                           buf);

  ಆದರೆ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    ಹಿಂತಿರುಗಿ EFAULT;
  }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t dos_write_blk(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
                               ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf)
{
  ವಿಭಜನೆ     *part;
  size_t        ಗಾತ್ರ_2;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (ವಿಭಜನೆ*)channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;

  ಆದರೆ(ಎಣಿಕೆ_2 == 1)
    ಗಾತ್ರ_2 = (size_t)channel->ಖಂಡ_ಗಾತ್ರ;
  ಇಲ್ಲದಿದ್ದರೆ
  {
    ಆದರೆ (ಎಣಿಕೆ_2 < 0)
      ಗಾತ್ರ_2 = (size_t)-ಎಣಿಕೆ_2;
    ಇಲ್ಲದಿದ್ದರೆ
      ಗಾತ್ರ_2 = (size_t)(ಎಣಿಕೆ_2 * channel->ಖಂಡ_ಗಾತ್ರ);
  }

  loc = (ext2_loff_t)ಖಂಡ * channel->ಖಂಡ_ಗಾತ್ರ;
  lba2chs(loc, &chs, part);
  _dio_hw_error = biosdisk(DISK_WRITE,
                           part->phys,
                           chs.ತಲೆ,
                           chs.cyl,
                           chs.sector,
                           ಗಾತ್ರ_2 < 512 ? 1 : ಗಾತ್ರ_2/512,
                           (ಶೂನ್ಯ*)buf);

  ಆದರೆ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    ಹಿಂತಿರುಗಿ EFAULT;
  }

  ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __TURBOC__
#ಸೂಚನೆ argsused
#ಆದರೆ_ಅಂತ್ಯ
ಸ್ಥಿರ errcode_t dos_flush(io_channel channel)
{
  /*
   * No buffers, no flush...
   */
  ಹಿಂತಿರುಗಿ 0;
}
