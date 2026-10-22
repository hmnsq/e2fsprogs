/*
 * v1.0
 *
 * Disk I/O include file for the ext2fs/DOS library.
 *
 * Copyright (c) 1997 Mark Habersack
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#अगर_अघोषित __diskio_h
#घोषणा __diskio_h
#अगर_घोषित __TURBOC__
#अगर_अघोषित __LARGE__
# error "ext2fs/DOS library requires LARGE model!"
#पूर्ण_अगर
#पूर्ण_अगर

#अगर_घोषित __TURBOC__
#समावेश "msdos.h"
#पूर्ण_अगर

/*
 * A helper structure used in LBA => CHS conversion
 */
प्रकार काष्ठा
{
  अचिन्हित लघु       cyl;     /* Cylinder (or track) */
  अचिन्हित लघु       शीर्ष_2;
  अचिन्हित लघु       sector;
  अचिन्हित लघु       offset;  /* Offset of byte within the sector */
} CHS;

/*
 * All partition data we need is here
 */
प्रकार काष्ठा
{
  अक्षर                 *dev;  /* _Linux_ device name (like "/dev/hda1") */
  अचिन्हित अक्षर        phys;  /* Physical DOS drive number */
  अचिन्हित दीर्घ        start; /* LBA address of partition start */
  अचिन्हित दीर्घ        len;   /* length of partition in sectors */
  अचिन्हित अक्षर        pno;   /* Partition number (read from *dev) */

  /* This partition's drive geometry */
  अचिन्हित लघु       cyls;
  अचिन्हित लघु       heads;
  अचिन्हित लघु       sects;
} PARTITION;

/*
 * PC partition table entry format
 */
#अगर_घोषित __DJGPP__
#आशय pack(1)
#पूर्ण_अगर
प्रकार काष्ठा
{
  अचिन्हित अक्षर        active;
  अचिन्हित अक्षर        start_शीर्ष;
  अचिन्हित अक्षर        start_sec;
  अचिन्हित अक्षर        start_cyl;
  अचिन्हित अक्षर        type;
  अचिन्हित अक्षर        end_शीर्ष;
  अचिन्हित अक्षर        end_sec;
  अचिन्हित अक्षर        end_cyl;
  अचिन्हित दीर्घ        first_sec_rel;
  अचिन्हित दीर्घ        आकार;
} PTABLE_ENTRY;
#अगर_घोषित __DJGPP__
#आशय pack()
#पूर्ण_अगर

/*
 * INT 0x13 operation codes
 */
#घोषणा DISK_READ          0x02
#घोषणा DISK_WRITE         0x03
#घोषणा DISK_GET_GEOMETRY  0x08
#घोषणा DISK_READY         0x10

/*
 * Errors to put in _dio_error
 */
#घोषणा ERR_BADDEV         0x00000001L
#घोषणा ERR_HARDWARE       0x00000002L
#घोषणा ERR_NOTSUPP        0x00000003L
#घोषणा ERR_NOTEXT2FS      0x00000004L
#घोषणा ERR_EMPTYPART      0x00000005L
#घोषणा ERR_LINUXSWAP      0x00000006L

/*
 * Functions in diskio.c
 */

/*
 * Variable contains last module's error
 */
बाह्य अचिन्हित दीर्घ        _dio_error;

/*
 * This one contains last hardware error (if _dio_error == ERR_HARDWARE)
 */
बाह्य अचिन्हित दीर्घ        _dio_hw_error;

/*
 * Macros to check for disk hardware errors
 */
#घोषणा HW_OK()             ((अचिन्हित अक्षर)_dio_hw_error == 0x00)
#घोषणा HW_BAD_CMD()        ((अचिन्हित अक्षर)_dio_hw_error == 0x01)
#घोषणा HW_NO_ADDR_MARK()   ((अचिन्हित अक्षर)_dio_hw_error == 0x02)
#घोषणा HW_WRITE_PROT()     ((अचिन्हित अक्षर)_dio_hw_error == 0x03)
#घोषणा HW_NO_SECTOR()      ((अचिन्हित अक्षर)_dio_hw_error == 0x04)
#घोषणा HW_RESET_FAIL()     ((अचिन्हित अक्षर)_dio_hw_error == 0x05)
#घोषणा HW_DISK_CHANGED()   ((अचिन्हित अक्षर)_dio_hw_error == 0x06)
#घोषणा HW_DRIVE_FAIL()     ((अचिन्हित अक्षर)_dio_hw_error == 0x07)
#घोषणा HW_DMA_OVERRUN()    ((अचिन्हित अक्षर)_dio_hw_error == 0x08)
#घोषणा HW_DMA_BOUNDARY()   ((अचिन्हित अक्षर)_dio_hw_error == 0x09)
#घोषणा HW_BAD_SECTOR()     ((अचिन्हित अक्षर)_dio_hw_error == 0x0A)
#घोषणा HW_BAD_TRACK()      ((अचिन्हित अक्षर)_dio_hw_error == 0x0B)
#घोषणा HW_UNSUPP_TRACK()   ((अचिन्हित अक्षर)_dio_hw_error == 0x0C)
#घोषणा HW_BAD_CRC_ECC()    ((अचिन्हित अक्षर)_dio_hw_error == 0x10)
#घोषणा HW_CRC_ECC_CORR()   ((अचिन्हित अक्षर)_dio_hw_error == 0x11)
#घोषणा HW_CONTR_FAIL()     ((अचिन्हित अक्षर)_dio_hw_error == 0x20)
#घोषणा HW_SEEK_FAIL()      ((अचिन्हित अक्षर)_dio_hw_error == 0x40)
#घोषणा HW_ATTACH_FAIL()    ((अचिन्हित अक्षर)_dio_hw_error == 0x80)
#घोषणा HW_DRIVE_NREADY()   ((अचिन्हित अक्षर)_dio_hw_error == 0xAA)
#घोषणा HW_UNDEF_ERROR()    ((अचिन्हित अक्षर)_dio_hw_error == 0xBB)
#घोषणा HW_WRITE_FAULT()    ((अचिन्हित अक्षर)_dio_hw_error == 0xCC)
#घोषणा HW_STATUS_ERROR()   ((अचिन्हित अक्षर)_dio_hw_error == 0xE0)
#घोषणा HW_SENSE_FAIL()     ((अचिन्हित अक्षर)_dio_hw_error == 0xFF)


/*
 * Open the specified partition.
 * String 'dev' must have a format:
 *
 *  /dev/{sd|hd|fd}[X]
 *
 * where,
 *
 *  only one of the option in curly braces can be used and X is an optional
 *  partition number for the given device. If X is not specified, function
 *  scans the drive's partition table in search for the first Linux ext2fs
 *  partition (signature 0x83). Along the way it dives into every extended
 *  partition encountered.
 *  Scan ends if either (a) there are no more used partition entries, or
 *  (b) there is no Xth partition.
 *
 * Routine returns 0 on success and !=0 otherwise.
 */
पूर्णांक open_partition(अक्षर *dev);

#पूर्ण_अगर /* __diskio_h */
