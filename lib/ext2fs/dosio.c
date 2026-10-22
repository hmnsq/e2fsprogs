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
#define WRITE_ERR(संदेश_2) लिख(2, संदेश_2, म_माप(संदेश_2))
#define WRITE_ERR_S(संदेश_2) लिख(2, संदेश_2, परिमाण(संदेश_2))

/*
 * Exported variables
 */
अचिन्हित दीर्घ        _dio_error;
अचिन्हित दीर्घ        _dio_hw_error;

/*
 * Array of all opened partitions
 */
स्थिर PARTITION        **partitions = रिक्त;
स्थिर अचिन्हित लघु   npart = 0; /* Number of mapped partitions */
स्थिर PARTITION        *active = रिक्त;

/*
 * I/O Manager routine prototypes
 */
स्थिर errcode_t dos_open(ध्रुव वर्ण *dev, पूर्णाङ्क flags, io_channel *channel);
स्थिर errcode_t dos_close(io_channel channel);
स्थिर errcode_t dos_set_blksize(io_channel channel, पूर्णाङ्क blksize);
स्थिर errcode_t dos_read_blk(io_channel channel, अचिन्हित दीर्घ block,
                                             पूर्णाङ्क गिनती, शून्य *buf);
स्थिर errcode_t dos_write_blk(io_channel channel, अचिन्हित दीर्घ block,
                               पूर्णाङ्क गिनती, ध्रुव शून्य *buf);
स्थिर errcode_t dos_flush(io_channel channel);

स्थिर संरचना struct_io_manager struct_dos_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "DOS I/O Manager",
	.खोलो		= dos_open,
	.बंद		= dos_close,
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
          यदि ((संरचना)->magic != (code)) प्रत्यागम (code)

/*
 * Calculates a CHS address of a sector from its LBA
 * offset for the given partition.
 */
स्थिर शून्य lba2chs(अचिन्हित दीर्घ lba_addr, CHS *chs, PARTITION *part)
{
  अचिन्हित दीर्घ      abss;

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
                                          अचिन्हित लघु phys,
                                          अचिन्हित वर्ण *pno)
{
  अचिन्हित        i;

  यदि(*pno != 0xFF && *pno >= 5)
     प्रत्यागम रिक्त; /* We don't support extended partitions for now */

  यदि(*pno != 0xFF)
  {
    यदि(pentry[*pno].type == LINUX_EXT2FS)
      प्रत्यागम &pentry[*pno];
    अन्यथा
    {
      यदि(!pentry[*pno].type)
        *pno = 0xFE;
      अन्यथा यदि(pentry[*pno].type == LINUX_SWAP)
        *pno = 0xFD;
      प्रत्यागम रिक्त;
    }
  }

  प्रति(i = 0; i < 4; i++)
    यदि(pentry[i].type == LINUX_EXT2FS)
    {
      *pno = i;
      प्रत्यागम &pentry[i];
    }

  प्रत्यागम रिक्त;
}

/*
 * Allocate libext2fs structures associated with I/O manager
 */
स्थिर io_channel alloc_io_channel(PARTITION *part)
{
  io_channel     ioch;

  ioch = (io_channel)दो_स्मृति(परिमाण(संरचना struct_io_channel));
  यदि (!ioch)
	  प्रत्यागम रिक्त;
  स_रखो(ioch, 0, परिमाण(संरचना struct_io_channel));
  ioch->magic = EXT2_ET_MAGIC_IO_CHANNEL;
  ioch->manager = dos_io_manager;
  ioch->name = (वर्ण *)दो_स्मृति(म_माप(part->dev)+1);
  यदि (!ioch->name) {
	  मुक्त(ioch);
	  प्रत्यागम रिक्त;
  }
  म_नकल(ioch->name, part->dev);
  ioch->private_data = part;
  ioch->block_size = 1024; /* The smallest ext2fs block size */
  ioch->read_error = 0;
  ioch->write_error = 0;

  प्रत्यागम ioch;
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
  अचिन्हित वर्ण  *tmp, sec[512];
  PARTITION      *part;
  PTABLE_ENTRY   *pent;
  PARTITION        **newparts;

  यदि(!dev)
  {
    _dio_error = ERR_BADDEV;
    प्रत्यागम EXT2_ET_BAD_DEVICE_NAME;
  }

  /*
   * First check whether the dev name is OK
   */
  tmp = (अचिन्हित वर्ण*)म_खोजप(dev, '/');
  यदि(!tmp)
  {
    _dio_error = ERR_BADDEV;
    प्रत्यागम EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp = 0;
  यदि(म_भेद(dev, "/dev"))
  {
    _dio_error = ERR_BADDEV;
    प्रत्यागम EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp++ = '/';

  /*
   * Check whether the partition data is already in cache
   */

  part = (PARTITION*)दो_स्मृति(परिमाण(PARTITION));
  यदि (!part)
	  प्रत्यागम ENOMEM;
  {
    पूर्णाङ्क   i = 0;

    प्रति(;i < npart; i++)
      यदि(!म_भेद(partitions[i]->dev, dev))
      {
        /* Found it! Make it the active one */
        active = partitions[i];
        *channel = alloc_io_channel(active);
	यदि (!*channel)
		प्रत्यागम ENOMEM;
        प्रत्यागम 0;
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
      part->phys += बड़े(tmp[2]) - 'A';
      /*
       * Do we have the partition number?
       */
      यदि(tmp[3])
        part->pno = है_अंक((पूर्णाङ्क)tmp[3]) ? tmp[3] - '0' - 1: 0;
      अन्यथा
        part->pno = 0xFF;
      भङ्ग;

    पक्ष 'f':
      यदि(tmp[2])
        part->phys = है_अंक((पूर्णाङ्क)tmp[2]) ? tmp[2] - '0' : 0;
      अन्यथा
        part->phys = 0x00; /* We'll assume /dev/fd0 */
      भङ्ग;

    स्वभाव:
      _dio_error = ERR_BADDEV;
      प्रत्यागम ENODEV;
  }

  यदि(part->phys < 0x80)
  {
     /* We don't support floppies for now */
     _dio_error = ERR_NOTSUPP;
     प्रत्यागम EINVAL;
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

  यदि(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    मुक्त(part->dev);
    मुक्त(part);
    प्रत्यागम EFAULT;
  }

  /*
   * Calculate the geometry
   */
  part->cyls  = (अचिन्हित लघु)(((sec[0] >> 6) << 8) + sec[1] + 1);
  part->heads = sec[3] + 1;
  part->sects = sec[0] & 0x3F;

  /*
   * Now that we know all we need, let's look for the partition
   */
  _dio_hw_error = biosdisk(DISK_READ, part->phys, 0, 0, 1, 1, sec);

  यदि(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    मुक्त(part->dev);
    मुक्त(part);
    प्रत्यागम EFAULT;
  }

  pent = (PTABLE_ENTRY*)&sec[0x1BE];
  pent = scan_partition_table(pent, part->phys, &part->pno);

  यदि(!pent)
  {
    _dio_error = part->pno == 0xFE ? ERR_EMPTYPART :
                 part->pno == 0xFD ? ERR_LINUXSWAP : ERR_NOTEXT2FS;
    मुक्त(part->dev);
    मुक्त(part);
    प्रत्यागम ENODEV;
  }

  /*
   * Calculate the remaining figures
   */
  {
    अचिन्हित दीर्घ    fsec, fhead, fcyl;

    fsec = (अचिन्हित दीर्घ)(pent->start_sec & 0x3F);
    fhead = (अचिन्हित दीर्घ)pent->start_head;
    fcyl = ((pent->start_sec >> 6) << 8) + pent->start_cyl;
    part->start = fsec + fhead * part->sects + fcyl *
                  (part->heads * part->sects) - 1;
    part->len = pent->size;
  }

  /*
   * Add the partition to the table
   */
  newparts = (PARTITION**)पुनः_स्मृति(partitions, परिमाण(PARTITION) * npart);
  यदि (!newparts) {
	  मुक्त(part);
	  प्रत्यागम ENOMEM;
  }
  partitions = newparts;
  partitions[npart++] = active = part;

  /*
   * Now alloc all libe2fs structures
   */
  *channel = alloc_io_channel(active);
  यदि (!*channel)
	  प्रत्यागम ENOMEM;

  प्रत्यागम 0;
}

स्थिर errcode_t dos_close(io_channel channel)
{
	मुक्त(channel->name);
	मुक्त(channel);

	प्रत्यागम 0;
}

स्थिर errcode_t dos_set_blksize(io_channel channel, पूर्णाङ्क blksize)
{
  channel->block_size = blksize;

  प्रत्यागम 0;
}

स्थिर errcode_t dos_read_blk(io_channel channel, अचिन्हित दीर्घ block,
                                             पूर्णाङ्क गिनती, शून्य *buf)
{
  PARTITION     *part;
  माप_प्रकार        size;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_data;

  size = (माप_प्रकार)((गिनती < 0) ? -गिनती : गिनती * channel->block_size);
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

  यदि(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    प्रत्यागम EFAULT;
  }

  प्रत्यागम 0;
}

स्थिर errcode_t dos_write_blk(io_channel channel, अचिन्हित दीर्घ block,
                               पूर्णाङ्क गिनती, ध्रुव शून्य *buf)
{
  PARTITION     *part;
  माप_प्रकार        size;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_data;

  यदि(गिनती == 1)
    size = (माप_प्रकार)channel->block_size;
  अन्यथा
  {
    यदि (गिनती < 0)
      size = (माप_प्रकार)-गिनती;
    अन्यथा
      size = (माप_प्रकार)(गिनती * channel->block_size);
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

  यदि(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    प्रत्यागम EFAULT;
  }

  प्रत्यागम 0;
}

#ifdef __TURBOC__
#pragma argsused
#endif
स्थिर errcode_t dos_flush(io_channel channel)
{
  /*
   * No buffers, no flush...
   */
  प्रत्यागम 0;
}
