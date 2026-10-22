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

#ifndef __diskio_h
#define __diskio_h
#ifdef __TURBOC__
#ifndef __LARGE__
# error "ext2fs/DOS library requires LARGE model!"
#endif
#endif

#ifdef __TURBOC__
#अन्तर्भाव "msdos.h"
#endif

/*
 * A helper structure used in LBA => CHS conversion
 */
प्रकारनिर्देश संरचना
{
  अचिन्हित लघु       cyl;     /* Cylinder (or track) */
  अचिन्हित लघु       head;
  अचिन्हित लघु       sector;
  अचिन्हित लघु       offset;  /* Offset of byte within the sector */
} CHS;

/*
 * All partition data we need is here
 */
प्रकारनिर्देश संरचना
{
  वर्ण                 *dev;  /* _Linux_ device name (like "/dev/hda1") */
  अचिन्हित वर्ण        phys;  /* Physical DOS drive number */
  अचिन्हित दीर्घ        start; /* LBA address of partition start */
  अचिन्हित दीर्घ        len;   /* length of partition in sectors */
  अचिन्हित वर्ण        pno;   /* Partition number (read from *dev) */

  /* This partition's drive geometry */
  अचिन्हित लघु       cyls;
  अचिन्हित लघु       heads;
  अचिन्हित लघु       sects;
} PARTITION;

/*
 * PC partition table entry format
 */
#ifdef __DJGPP__
#pragma pack(1)
#endif
प्रकारनिर्देश संरचना
{
  अचिन्हित वर्ण        active;
  अचिन्हित वर्ण        start_head;
  अचिन्हित वर्ण        start_sec;
  अचिन्हित वर्ण        start_cyl;
  अचिन्हित वर्ण        type;
  अचिन्हित वर्ण        end_head;
  अचिन्हित वर्ण        end_sec;
  अचिन्हित वर्ण        end_cyl;
  अचिन्हित दीर्घ        first_sec_rel;
  अचिन्हित दीर्घ        size;
} PTABLE_ENTRY;
#ifdef __DJGPP__
#pragma pack()
#endif

/*
 * INT 0x13 operation codes
 */
#define DISK_READ          0x02
#define DISK_WRITE         0x03
#define DISK_GET_GEOMETRY  0x08
#define DISK_READY         0x10

/*
 * Errors to put in _dio_error
 */
#define ERR_BADDEV         0x00000001L
#define ERR_HARDWARE       0x00000002L
#define ERR_NOTSUPP        0x00000003L
#define ERR_NOTEXT2FS      0x00000004L
#define ERR_EMPTYPART      0x00000005L
#define ERR_LINUXSWAP      0x00000006L

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
#define HW_OK()             ((अचिन्हित वर्ण)_dio_hw_error == 0x00)
#define HW_BAD_CMD()        ((अचिन्हित वर्ण)_dio_hw_error == 0x01)
#define HW_NO_ADDR_MARK()   ((अचिन्हित वर्ण)_dio_hw_error == 0x02)
#define HW_WRITE_PROT()     ((अचिन्हित वर्ण)_dio_hw_error == 0x03)
#define HW_NO_SECTOR()      ((अचिन्हित वर्ण)_dio_hw_error == 0x04)
#define HW_RESET_FAIL()     ((अचिन्हित वर्ण)_dio_hw_error == 0x05)
#define HW_DISK_CHANGED()   ((अचिन्हित वर्ण)_dio_hw_error == 0x06)
#define HW_DRIVE_FAIL()     ((अचिन्हित वर्ण)_dio_hw_error == 0x07)
#define HW_DMA_OVERRUN()    ((अचिन्हित वर्ण)_dio_hw_error == 0x08)
#define HW_DMA_BOUNDARY()   ((अचिन्हित वर्ण)_dio_hw_error == 0x09)
#define HW_BAD_SECTOR()     ((अचिन्हित वर्ण)_dio_hw_error == 0x0A)
#define HW_BAD_TRACK()      ((अचिन्हित वर्ण)_dio_hw_error == 0x0B)
#define HW_UNSUPP_TRACK()   ((अचिन्हित वर्ण)_dio_hw_error == 0x0C)
#define HW_BAD_CRC_ECC()    ((अचिन्हित वर्ण)_dio_hw_error == 0x10)
#define HW_CRC_ECC_CORR()   ((अचिन्हित वर्ण)_dio_hw_error == 0x11)
#define HW_CONTR_FAIL()     ((अचिन्हित वर्ण)_dio_hw_error == 0x20)
#define HW_SEEK_FAIL()      ((अचिन्हित वर्ण)_dio_hw_error == 0x40)
#define HW_ATTACH_FAIL()    ((अचिन्हित वर्ण)_dio_hw_error == 0x80)
#define HW_DRIVE_NREADY()   ((अचिन्हित वर्ण)_dio_hw_error == 0xAA)
#define HW_UNDEF_ERROR()    ((अचिन्हित वर्ण)_dio_hw_error == 0xBB)
#define HW_WRITE_FAULT()    ((अचिन्हित वर्ण)_dio_hw_error == 0xCC)
#define HW_STATUS_ERROR()   ((अचिन्हित वर्ण)_dio_hw_error == 0xE0)
#define HW_SENSE_FAIL()     ((अचिन्हित वर्ण)_dio_hw_error == 0xFF)


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
पूर्णाङ्क open_partition(वर्ण *dev);

#endif /* __diskio_h */
