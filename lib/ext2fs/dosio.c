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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <bios.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <ctype.h>
#అన్తర్భావ <io.h>
#నిర్వచితమైతే HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు

#అన్తర్భావ <ext2fs/ext2_types.h>
#అన్తర్భావ "utils.h"
#అన్తర్భావ "dosio.h"
#అన్తర్భావ "et/com_err.h"
#అన్తర్భావ "ext2_err.h"
#అన్తర్భావ "ext2fs/io.h"

/*
 * Some helper macros
 */
#నిర్వచించు LINUX_EXT2FS       0x83
#నిర్వచించు LINUX_మార్పిడి         0x82
#నిర్వచించు WRITE_ERR(సందేశం_2) లిఖ(2, సందేశం_2, అక్షరశ్రేణి_పొడవు(సందేశం_2))
#నిర్వచించు WRITE_ERR_S(సందేశం_2) లిఖ(2, సందేశం_2, పరిమాణం(సందేశం_2))

/*
 * Exported variables
 */
గుర్తులేని దీర్ఘ        _dio_error;
గుర్తులేని దీర్ఘ        _dio_hw_error;

/*
 * Array of all opened partitions
 */
స్థిర విభజన        **partitions = ఖాళీ;
స్థిర గుర్తులేని పొట్టి   npart = 0; /* Number of mapped partitions */
స్థిర విభజన        *చురుకైన = ఖాళీ;

/*
 * I/O Manager routine prototypes
 */
స్థిర errcode_t dos_open(స్థిరరాశి అక్షరం *dev, పూర్ణాంకం flags, io_channel *channel);
స్థిర errcode_t dos_close(io_channel channel);
స్థిర errcode_t dos_సమితి_blksize(io_channel channel, పూర్ణాంకం blksize);
స్థిర errcode_t dos_read_blk(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
                                             పూర్ణాంకం లెక్క, శూన్య *buf);
స్థిర errcode_t dos_write_blk(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
                               పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *buf);
స్థిర errcode_t dos_flush(io_channel channel);

స్థిర నిర్మాణం struct_io_manager struct_dos_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "DOS I/O Manager",
	.open		= dos_open,
	.close		= dos_close,
	.సమితి_blksize	= dos_సమితి_blksize,
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

#నిర్వచించు EXT2_CHECK_MAGIC(నిర్మాణం, సంకేతం_2) \
          ఒకవేళ ((నిర్మాణం)->magic != (సంకేతం_2)) తిరిగి (సంకేతం_2)

/*
 * Calculates a CHS address of a sector from its LBA
 * offset for the given partition.
 */
స్థిర శూన్య lba2chs(గుర్తులేని దీర్ఘ lba_addr, CHS *chs, విభజన *part)
{
  గుర్తులేని దీర్ఘ      abss;

  chs->స్థానభ్రంశం = lba_addr & 0x000001FF;
  abss = (lba_addr >> 9) + part->ప్రారంభం;
  chs->cyl    = abss / (part->sects * part->heads);
  chs->తల   = (abss / part->sects) % part->heads;
  chs->sector = (abss % part->sects) + 1;
}

#నిర్వచితమైతే __TURBOC__
#సూచన argsused
#అయితే_ముగింపు
/*
 * Scans the passed partition table looking for *pno partition
 * that has LINUX_EXT2FS type.
 *
 * TODO:
 * For partition numbers >5 Linux uses DOS extended partitions -
 * dive into them an return an appropriate entry. Also dive into
 * extended partitions when scanning for a first Linux/ext2fs.
 */
స్థిర PTABLE_నమోదు *పరిశీలన_విభజన_పట్టిక(PTABLE_నమోదు *pentry,
                                          గుర్తులేని పొట్టి phys,
                                          గుర్తులేని అక్షరం *pno)
{
  గుర్తులేని        i;

  ఒకవేళ(*pno != 0xFF && *pno >= 5)
     తిరిగి ఖాళీ; /* We don't support extended partitions for now */

  ఒకవేళ(*pno != 0xFF)
  {
    ఒకవేళ(pentry[*pno].type == LINUX_EXT2FS)
      తిరిగి &pentry[*pno];
    లేకపోతే
    {
      ఒకవేళ(!pentry[*pno].type)
        *pno = 0xFE;
      లేకపోతే ఒకవేళ(pentry[*pno].type == LINUX_మార్పిడి)
        *pno = 0xFD;
      తిరిగి ఖాళీ;
    }
  }

  కొరకు(i = 0; i < 4; i++)
    ఒకవేళ(pentry[i].type == LINUX_EXT2FS)
    {
      *pno = i;
      తిరిగి &pentry[i];
    }

  తిరిగి ఖాళీ;
}

/*
 * Allocate libext2fs structures associated with I/O manager
 */
స్థిర io_channel alloc_io_channel(విభజన *part)
{
  io_channel     ioch;

  ioch = (io_channel)జ్ఞాపకం_ఇవ్వు(పరిమాణం(నిర్మాణం struct_io_channel));
  ఒకవేళ (!ioch)
	  తిరిగి ఖాళీ;
  memset(ioch, 0, పరిమాణం(నిర్మాణం struct_io_channel));
  ioch->magic = EXT2_ET_MAGIC_IO_CHANNEL;
  ioch->manager = dos_io_manager;
  ioch->name = (అక్షరం *)జ్ఞాపకం_ఇవ్వు(అక్షరశ్రేణి_పొడవు(part->dev)+1);
  ఒకవేళ (!ioch->name) {
	  విడుదల(ioch);
	  తిరిగి ఖాళీ;
  }
  strcpy(ioch->name, part->dev);
  ioch->వ్యక్తిగత_దత్తాంశం = part;
  ioch->ఖండం_కొలత = 1024; /* The smallest ext2fs block size */
  ioch->read_error = 0;
  ioch->write_error = 0;

  తిరిగి ioch;
}

#నిర్వచితమైతే __TURBOC__
#సూచన argsused
#అయితే_ముగింపు
/*
 * Open the 'name' partition, initialize all information structures
 * we need to keep and create libext2fs I/O manager.
 */
స్థిర errcode_t dos_open(స్థిరరాశి అక్షరం *dev, పూర్ణాంకం flags, io_channel *channel)
{
  గుర్తులేని అక్షరం  *tmp, sec[512];
  విభజన      *part;
  PTABLE_నమోదు   *pent;
  విభజన        **newparts;

  ఒకవేళ(!dev)
  {
    _dio_error = ERR_BADDEV;
    తిరిగి EXT2_ET_BAD_DEVICE_NAME;
  }

  /*
   * First check whether the dev name is OK
   */
  tmp = (గుర్తులేని అక్షరం*)strrchr(dev, '/');
  ఒకవేళ(!tmp)
  {
    _dio_error = ERR_BADDEV;
    తిరిగి EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp = 0;
  ఒకవేళ(strcmp(dev, "/dev"))
  {
    _dio_error = ERR_BADDEV;
    తిరిగి EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp++ = '/';

  /*
   * Check whether the partition data is already in cache
   */

  part = (విభజన*)జ్ఞాపకం_ఇవ్వు(పరిమాణం(విభజన));
  ఒకవేళ (!part)
	  తిరిగి ENOMEM;
  {
    పూర్ణాంకం   i = 0;

    కొరకు(;i < npart; i++)
      ఒకవేళ(!strcmp(partitions[i]->dev, dev))
      {
        /* Found it! Make it the active one */
        చురుకైన = partitions[i];
        *channel = alloc_io_channel(చురుకైన);
	ఒకవేళ (!*channel)
		తిరిగి ENOMEM;
        తిరిగి 0;
      }
  }

  /*
   * Drive number & optionally partn number
   */
  ఎంపిక(tmp[0])
  {
    స్థితి 'h':
    స్థితి 's':
      part->phys = 0x80;
      part->phys += పెద్ద_అక్షరం(tmp[2]) - 'A';
      /*
       * Do we have the partition number?
       */
      ఒకవేళ(tmp[3])
        part->pno = అంకెనా((పూర్ణాంకం)tmp[3]) ? tmp[3] - '0' - 1: 0;
      లేకపోతే
        part->pno = 0xFF;
      ఆపు;

    స్థితి 'f':
      ఒకవేళ(tmp[2])
        part->phys = అంకెనా((పూర్ణాంకం)tmp[2]) ? tmp[2] - '0' : 0;
      లేకపోతే
        part->phys = 0x00; /* We'll assume /dev/fd0 */
      ఆపు;

    అప్రమేయ:
      _dio_error = ERR_BADDEV;
      తిరిగి ENODEV;
  }

  ఒకవేళ(part->phys < 0x80)
  {
     /* We don't support floppies for now */
     _dio_error = ERR_NOTSUPP;
     తిరిగి EINVAL;
  }

  part->dev = strdup(dev);

  /*
   * Get drive's geometry
   */
  _dio_hw_error = biosdisk(DISK_GET_GEOMETRY,
                           part->phys,
                           0, /*తల*/
                           0, /* cylinder */
                           1, /* sector */
                           1, /* just one sector */
                           sec);

  ఒకవేళ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    విడుదల(part->dev);
    విడుదల(part);
    తిరిగి EFAULT;
  }

  /*
   * Calculate the geometry
   */
  part->cyls  = (గుర్తులేని పొట్టి)(((sec[0] >> 6) << 8) + sec[1] + 1);
  part->heads = sec[3] + 1;
  part->sects = sec[0] & 0x3F;

  /*
   * Now that we know all we need, let's look for the partition
   */
  _dio_hw_error = biosdisk(DISK_READ, part->phys, 0, 0, 1, 1, sec);

  ఒకవేళ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    విడుదల(part->dev);
    విడుదల(part);
    తిరిగి EFAULT;
  }

  pent = (PTABLE_నమోదు*)&sec[0x1BE];
  pent = పరిశీలన_విభజన_పట్టిక(pent, part->phys, &part->pno);

  ఒకవేళ(!pent)
  {
    _dio_error = part->pno == 0xFE ? ERR_EMPTYPART :
                 part->pno == 0xFD ? ERR_LINUXSWAP : ERR_NOTEXT2FS;
    విడుదల(part->dev);
    విడుదల(part);
    తిరిగి ENODEV;
  }

  /*
   * Calculate the remaining figures
   */
  {
    గుర్తులేని దీర్ఘ    fsec, fhead, fcyl;

    fsec = (గుర్తులేని దీర్ఘ)(pent->ప్రారంభం_sec & 0x3F);
    fhead = (గుర్తులేని దీర్ఘ)pent->ప్రారంభం_తల;
    fcyl = ((pent->ప్రారంభం_sec >> 6) << 8) + pent->ప్రారంభం_cyl;
    part->ప్రారంభం = fsec + fhead * part->sects + fcyl *
                  (part->heads * part->sects) - 1;
    part->len = pent->కొలత;
  }

  /*
   * Add the partition to the table
   */
  newparts = (విభజన**)realloc(partitions, పరిమాణం(విభజన) * npart);
  ఒకవేళ (!newparts) {
	  విడుదల(part);
	  తిరిగి ENOMEM;
  }
  partitions = newparts;
  partitions[npart++] = చురుకైన = part;

  /*
   * Now alloc all libe2fs structures
   */
  *channel = alloc_io_channel(చురుకైన);
  ఒకవేళ (!*channel)
	  తిరిగి ENOMEM;

  తిరిగి 0;
}

స్థిర errcode_t dos_close(io_channel channel)
{
	విడుదల(channel->name);
	విడుదల(channel);

	తిరిగి 0;
}

స్థిర errcode_t dos_సమితి_blksize(io_channel channel, పూర్ణాంకం blksize)
{
  channel->ఖండం_కొలత = blksize;

  తిరిగి 0;
}

స్థిర errcode_t dos_read_blk(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
                                             పూర్ణాంకం లెక్క, శూన్య *buf)
{
  విభజన     *part;
  size_t        కొలత;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (విభజన*)channel->వ్యక్తిగత_దత్తాంశం;

  కొలత = (size_t)((లెక్క < 0) ? -లెక్క : లెక్క * channel->ఖండం_కొలత);
  loc = (ext2_loff_t) ఖండం * channel->ఖండం_కొలత;

  lba2chs(loc, &chs, part);
  /*
   * Potential bug here:
   *   If DJGPP is used then reads of >18 sectors will fail!
   *   Have to rewrite biosdisk.
   */
  _dio_hw_error = biosdisk(DISK_READ,
                           part->phys,
                           chs.తల,
                           chs.cyl,
                           chs.sector,
                           కొలత < 512 ? 1 : కొలత/512,
                           buf);

  ఒకవేళ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    తిరిగి EFAULT;
  }

  తిరిగి 0;
}

స్థిర errcode_t dos_write_blk(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
                               పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *buf)
{
  విభజన     *part;
  size_t        కొలత;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (విభజన*)channel->వ్యక్తిగత_దత్తాంశం;

  ఒకవేళ(లెక్క == 1)
    కొలత = (size_t)channel->ఖండం_కొలత;
  లేకపోతే
  {
    ఒకవేళ (లెక్క < 0)
      కొలత = (size_t)-లెక్క;
    లేకపోతే
      కొలత = (size_t)(లెక్క * channel->ఖండం_కొలత);
  }

  loc = (ext2_loff_t)ఖండం * channel->ఖండం_కొలత;
  lba2chs(loc, &chs, part);
  _dio_hw_error = biosdisk(DISK_WRITE,
                           part->phys,
                           chs.తల,
                           chs.cyl,
                           chs.sector,
                           కొలత < 512 ? 1 : కొలత/512,
                           (శూన్య*)buf);

  ఒకవేళ(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    తిరిగి EFAULT;
  }

  తిరిగి 0;
}

#నిర్వచితమైతే __TURBOC__
#సూచన argsused
#అయితే_ముగింపు
స్థిర errcode_t dos_flush(io_channel channel)
{
  /*
   * No buffers, no flush...
   */
  తిరిగి 0;
}
