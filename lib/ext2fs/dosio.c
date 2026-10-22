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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <bios.h>
#समावेश <string.h>
#समावेश <ctype.h>
#समावेश <io.h>
#अगर_घोषित HAVE_ERRNO_H
#समावेश <errno.h>
#पूर्ण_अगर

#समावेश <ext2fs/ext2_types.h>
#समावेश "utils.h"
#समावेश "dosio.h"
#समावेश "et/com_err.h"
#समावेश "ext2_err.h"
#समावेश "ext2fs/io.h"

/*
 * Some helper macros
 */
#घोषणा LINUX_EXT2FS       0x83
#घोषणा LINUX_SWAP         0x82
#घोषणा WRITE_ERR(संदेश_2) ग_लिखो(2, संदेश_2, म_माप(संदेश_2))
#घोषणा WRITE_ERR_S(संदेश_2) ग_लिखो(2, संदेश_2, माप(संदेश_2))

/*
 * Exported variables
 */
अचिन्हित दीर्घ        _dio_error;
अचिन्हित दीर्घ        _dio_hw_error;

/*
 * Array of all opened partitions
 */
अटल PARTITION        **partitions = शून्य;
अटल अचिन्हित लघु   npart = 0; /* Number of mapped partitions */
अटल PARTITION        *active = शून्य;

/*
 * I/O Manager routine prototypes
 */
अटल errcode_t dos_open(स्थिर अक्षर *dev, पूर्णांक flags, io_channel *channel);
अटल errcode_t dos_close(io_channel channel);
अटल errcode_t dos_set_blksize(io_channel channel, पूर्णांक blksize);
अटल errcode_t dos_read_blk(io_channel channel, अचिन्हित दीर्घ block,
                                             पूर्णांक गिनती, व्योम *buf);
अटल errcode_t dos_write_blk(io_channel channel, अचिन्हित दीर्घ block,
                               पूर्णांक गिनती, स्थिर व्योम *buf);
अटल errcode_t dos_flush(io_channel channel);

अटल काष्ठा struct_io_manager struct_dos_manager = {
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

#घोषणा EXT2_CHECK_MAGIC(काष्ठा, code) \
          अगर ((काष्ठा)->magic != (code)) वापस (code)

/*
 * Calculates a CHS address of a sector from its LBA
 * offset for the given partition.
 */
अटल व्योम lba2chs(अचिन्हित दीर्घ lba_addr, CHS *chs, PARTITION *part)
{
  अचिन्हित दीर्घ      abss;

  chs->offset = lba_addr & 0x000001FF;
  abss = (lba_addr >> 9) + part->start;
  chs->cyl    = abss / (part->sects * part->heads);
  chs->शीर्ष_2   = (abss / part->sects) % part->heads;
  chs->sector = (abss % part->sects) + 1;
}

#अगर_घोषित __TURBOC__
#आशय argsused
#पूर्ण_अगर
/*
 * Scans the passed partition table looking for *pno partition
 * that has LINUX_EXT2FS type.
 *
 * TODO:
 * For partition numbers >5 Linux uses DOS extended partitions -
 * dive into them an return an appropriate entry. Also dive into
 * extended partitions when scanning for a first Linux/ext2fs.
 */
अटल PTABLE_ENTRY *scan_partition_table(PTABLE_ENTRY *pentry,
                                          अचिन्हित लघु phys,
                                          अचिन्हित अक्षर *pno)
{
  अचिन्हित        i;

  अगर(*pno != 0xFF && *pno >= 5)
     वापस शून्य; /* We don't support extended partitions for now */

  अगर(*pno != 0xFF)
  {
    अगर(pentry[*pno].type == LINUX_EXT2FS)
      वापस &pentry[*pno];
    अन्यथा
    {
      अगर(!pentry[*pno].type)
        *pno = 0xFE;
      अन्यथा अगर(pentry[*pno].type == LINUX_SWAP)
        *pno = 0xFD;
      वापस शून्य;
    }
  }

  क्रम(i = 0; i < 4; i++)
    अगर(pentry[i].type == LINUX_EXT2FS)
    {
      *pno = i;
      वापस &pentry[i];
    }

  वापस शून्य;
}

/*
 * Allocate libext2fs structures associated with I/O manager
 */
अटल io_channel alloc_io_channel(PARTITION *part)
{
  io_channel     ioch;

  ioch = (io_channel)दो_स्मृति(माप(काष्ठा struct_io_channel));
  अगर (!ioch)
	  वापस शून्य;
  स_रखो(ioch, 0, माप(काष्ठा struct_io_channel));
  ioch->magic = EXT2_ET_MAGIC_IO_CHANNEL;
  ioch->manager = dos_io_manager;
  ioch->name = (अक्षर *)दो_स्मृति(म_माप(part->dev)+1);
  अगर (!ioch->name) {
	  मुक्त(ioch);
	  वापस शून्य;
  }
  म_नकल(ioch->name, part->dev);
  ioch->private_तथ्य = part;
  ioch->block_आकार = 1024; /* The smallest ext2fs block size */
  ioch->read_error = 0;
  ioch->write_error = 0;

  वापस ioch;
}

#अगर_घोषित __TURBOC__
#आशय argsused
#पूर्ण_अगर
/*
 * Open the 'name' partition, initialize all information structures
 * we need to keep and create libext2fs I/O manager.
 */
अटल errcode_t dos_open(स्थिर अक्षर *dev, पूर्णांक flags, io_channel *channel)
{
  अचिन्हित अक्षर  *tmp, sec[512];
  PARTITION      *part;
  PTABLE_ENTRY   *pent;
  PARTITION        **newparts;

  अगर(!dev)
  {
    _dio_error = ERR_BADDEV;
    वापस EXT2_ET_BAD_DEVICE_NAME;
  }

  /*
   * First check whether the dev name is OK
   */
  tmp = (अचिन्हित अक्षर*)म_खोजप(dev, '/');
  अगर(!tmp)
  {
    _dio_error = ERR_BADDEV;
    वापस EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp = 0;
  अगर(म_भेद(dev, "/dev"))
  {
    _dio_error = ERR_BADDEV;
    वापस EXT2_ET_BAD_DEVICE_NAME;
  }
  *tmp++ = '/';

  /*
   * Check whether the partition data is already in cache
   */

  part = (PARTITION*)दो_स्मृति(माप(PARTITION));
  अगर (!part)
	  वापस ENOMEM;
  {
    पूर्णांक   i = 0;

    क्रम(;i < npart; i++)
      अगर(!म_भेद(partitions[i]->dev, dev))
      {
        /* Found it! Make it the active one */
        active = partitions[i];
        *channel = alloc_io_channel(active);
	अगर (!*channel)
		वापस ENOMEM;
        वापस 0;
      }
  }

  /*
   * Drive number & optionally partn number
   */
  चयन(tmp[0])
  {
    हाल 'h':
    हाल 's':
      part->phys = 0x80;
      part->phys += बड़े(tmp[2]) - 'A';
      /*
       * Do we have the partition number?
       */
      अगर(tmp[3])
        part->pno = है_अंक((पूर्णांक)tmp[3]) ? tmp[3] - '0' - 1: 0;
      अन्यथा
        part->pno = 0xFF;
      अवरोध;

    हाल 'f':
      अगर(tmp[2])
        part->phys = है_अंक((पूर्णांक)tmp[2]) ? tmp[2] - '0' : 0;
      अन्यथा
        part->phys = 0x00; /* We'll assume /dev/fd0 */
      अवरोध;

    शेष:
      _dio_error = ERR_BADDEV;
      वापस ENODEV;
  }

  अगर(part->phys < 0x80)
  {
     /* We don't support floppies for now */
     _dio_error = ERR_NOTSUPP;
     वापस EINVAL;
  }

  part->dev = strdup(dev);

  /*
   * Get drive's geometry
   */
  _dio_hw_error = biosdisk(DISK_GET_GEOMETRY,
                           part->phys,
                           0, /*शीर्ष*/
                           0, /* cylinder */
                           1, /* sector */
                           1, /* just one sector */
                           sec);

  अगर(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    मुक्त(part->dev);
    मुक्त(part);
    वापस EFAULT;
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

  अगर(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    मुक्त(part->dev);
    मुक्त(part);
    वापस EFAULT;
  }

  pent = (PTABLE_ENTRY*)&sec[0x1BE];
  pent = scan_partition_table(pent, part->phys, &part->pno);

  अगर(!pent)
  {
    _dio_error = part->pno == 0xFE ? ERR_EMPTYPART :
                 part->pno == 0xFD ? ERR_LINUXSWAP : ERR_NOTEXT2FS;
    मुक्त(part->dev);
    मुक्त(part);
    वापस ENODEV;
  }

  /*
   * Calculate the remaining figures
   */
  {
    अचिन्हित दीर्घ    fsec, fhead, fcyl;

    fsec = (अचिन्हित दीर्घ)(pent->start_sec & 0x3F);
    fhead = (अचिन्हित दीर्घ)pent->start_शीर्ष;
    fcyl = ((pent->start_sec >> 6) << 8) + pent->start_cyl;
    part->start = fsec + fhead * part->sects + fcyl *
                  (part->heads * part->sects) - 1;
    part->len = pent->आकार;
  }

  /*
   * Add the partition to the table
   */
  newparts = (PARTITION**)पुनः_स्मृति(partitions, माप(PARTITION) * npart);
  अगर (!newparts) {
	  मुक्त(part);
	  वापस ENOMEM;
  }
  partitions = newparts;
  partitions[npart++] = active = part;

  /*
   * Now alloc all libe2fs structures
   */
  *channel = alloc_io_channel(active);
  अगर (!*channel)
	  वापस ENOMEM;

  वापस 0;
}

अटल errcode_t dos_close(io_channel channel)
{
	मुक्त(channel->name);
	मुक्त(channel);

	वापस 0;
}

अटल errcode_t dos_set_blksize(io_channel channel, पूर्णांक blksize)
{
  channel->block_आकार = blksize;

  वापस 0;
}

अटल errcode_t dos_read_blk(io_channel channel, अचिन्हित दीर्घ block,
                                             पूर्णांक गिनती, व्योम *buf)
{
  PARTITION     *part;
  माप_प्रकार        आकार;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_तथ्य;

  आकार = (माप_प्रकार)((गिनती < 0) ? -गिनती : गिनती * channel->block_आकार);
  loc = (ext2_loff_t) block * channel->block_आकार;

  lba2chs(loc, &chs, part);
  /*
   * Potential bug here:
   *   If DJGPP is used then reads of >18 sectors will fail!
   *   Have to rewrite biosdisk.
   */
  _dio_hw_error = biosdisk(DISK_READ,
                           part->phys,
                           chs.शीर्ष_2,
                           chs.cyl,
                           chs.sector,
                           आकार < 512 ? 1 : आकार/512,
                           buf);

  अगर(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    वापस EFAULT;
  }

  वापस 0;
}

अटल errcode_t dos_write_blk(io_channel channel, अचिन्हित दीर्घ block,
                               पूर्णांक गिनती, स्थिर व्योम *buf)
{
  PARTITION     *part;
  माप_प्रकार        आकार;
  ext2_loff_t   loc;
  CHS           chs;

  EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
  part = (PARTITION*)channel->private_तथ्य;

  अगर(गिनती == 1)
    आकार = (माप_प्रकार)channel->block_आकार;
  अन्यथा
  {
    अगर (गिनती < 0)
      आकार = (माप_प्रकार)-गिनती;
    अन्यथा
      आकार = (माप_प्रकार)(गिनती * channel->block_आकार);
  }

  loc = (ext2_loff_t)block * channel->block_आकार;
  lba2chs(loc, &chs, part);
  _dio_hw_error = biosdisk(DISK_WRITE,
                           part->phys,
                           chs.शीर्ष_2,
                           chs.cyl,
                           chs.sector,
                           आकार < 512 ? 1 : आकार/512,
                           (व्योम*)buf);

  अगर(!HW_OK())
  {
    _dio_error = ERR_HARDWARE;
    वापस EFAULT;
  }

  वापस 0;
}

#अगर_घोषित __TURBOC__
#आशय argsused
#पूर्ण_अगर
अटल errcode_t dos_flush(io_channel channel)
{
  /*
   * No buffers, no flush...
   */
  वापस 0;
}
