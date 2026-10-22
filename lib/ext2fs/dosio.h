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

#यदि_अपरिभाषितम् __diskio_h
#परिभाषय __diskio_h
#यदि_परिभाषितम् __TURBOC__
#यदि_अपरिभाषितम् __LARGE__
# error "ext2fs/DOS library requires LARGE model!"
#यदि_अन्तः
#यदि_अन्तः

#यदि_परिभाषितम् __TURBOC__
#अन्तर्भाव "msdos.h"
#यदि_अन्तः

/*
 * A helper structure used in LBA => CHS conversion
 */
प्रकारनिर्देश संरचना
{
  अचिह्नित ह्रस्व       cyl;     /* Cylinder (or track) */
  अचिह्नित ह्रस्व       शीर्ष;
  अचिह्नित ह्रस्व       sector;
  अचिह्नित ह्रस्व       विस्थापनम्;  /* Offset of byte within the sector */
} CHS;

/*
 * All partition data we need is here
 */
प्रकारनिर्देश संरचना
{
  वर्ण                 *dev;  /* _Linux_ device name (like "/dev/hda1") */
  अचिह्नित वर्ण        phys;  /* Physical DOS drive number */
  अचिह्नित दीर्घ        start; /* LBA address of partition start */
  अचिह्नित दीर्घ        len;   /* length of partition in sectors */
  अचिह्नित वर्ण        pno;   /* Partition number (read from *dev) */

  /* This partition's drive geometry */
  अचिह्नित ह्रस्व       cyls;
  अचिह्नित ह्रस्व       heads;
  अचिह्नित ह्रस्व       sects;
} PARTITION;

/*
 * PC partition table entry format
 */
#यदि_परिभाषितम् __DJGPP__
#निर्देशः pack(1)
#यदि_अन्तः
प्रकारनिर्देश संरचना
{
  अचिह्नित वर्ण        सक्रिय;
  अचिह्नित वर्ण        start_शीर्ष;
  अचिह्नित वर्ण        start_sec;
  अचिह्नित वर्ण        start_cyl;
  अचिह्नित वर्ण        type;
  अचिह्नित वर्ण        अन्तः_शीर्ष;
  अचिह्नित वर्ण        अन्तः_sec;
  अचिह्नित वर्ण        अन्तः_cyl;
  अचिह्नित दीर्घ        first_sec_rel;
  अचिह्नित दीर्घ        आकार;
} PTABLE_प्रविष्टिः;
#यदि_परिभाषितम् __DJGPP__
#निर्देशः pack()
#यदि_अन्तः

/*
 * INT 0x13 operation codes
 */
#परिभाषय DISK_READ          0x02
#परिभाषय DISK_WRITE         0x03
#परिभाषय DISK_GET_GEOMETRY  0x08
#परिभाषय DISK_READY         0x10

/*
 * Errors to put in _dio_error
 */
#परिभाषय ERR_BADDEV         0x00000001L
#परिभाषय ERR_HARDWARE       0x00000002L
#परिभाषय ERR_NOTSUPP        0x00000003L
#परिभाषय ERR_NOTEXT2FS      0x00000004L
#परिभाषय ERR_EMPTYPART      0x00000005L
#परिभाषय ERR_LINUXSWAP      0x00000006L

/*
 * Functions in diskio.c
 */

/*
 * Variable contains last module's error
 */
बाह्य अचिह्नित दीर्घ        _dio_error;

/*
 * This one contains last hardware error (if _dio_error == ERR_HARDWARE)
 */
बाह्य अचिह्नित दीर्घ        _dio_hw_error;

/*
 * Macros to check for disk hardware errors
 */
#परिभाषय HW_OK()             ((अचिह्नित वर्ण)_dio_hw_error == 0x00)
#परिभाषय HW_BAD_CMD()        ((अचिह्नित वर्ण)_dio_hw_error == 0x01)
#परिभाषय HW_NO_ADDR_MARK()   ((अचिह्नित वर्ण)_dio_hw_error == 0x02)
#परिभाषय HW_WRITE_PROT()     ((अचिह्नित वर्ण)_dio_hw_error == 0x03)
#परिभाषय HW_NO_SECTOR()      ((अचिह्नित वर्ण)_dio_hw_error == 0x04)
#परिभाषय HW_RESET_FAIL()     ((अचिह्नित वर्ण)_dio_hw_error == 0x05)
#परिभाषय HW_DISK_परिवर्तितम्()   ((अचिह्नित वर्ण)_dio_hw_error == 0x06)
#परिभाषय HW_DRIVE_FAIL()     ((अचिह्नित वर्ण)_dio_hw_error == 0x07)
#परिभाषय HW_DMA_OVERRUN()    ((अचिह्नित वर्ण)_dio_hw_error == 0x08)
#परिभाषय HW_DMA_BOUNDARY()   ((अचिह्नित वर्ण)_dio_hw_error == 0x09)
#परिभाषय HW_BAD_SECTOR()     ((अचिह्नित वर्ण)_dio_hw_error == 0x0A)
#परिभाषय HW_BAD_TRACK()      ((अचिह्नित वर्ण)_dio_hw_error == 0x0B)
#परिभाषय HW_UNSUPP_TRACK()   ((अचिह्नित वर्ण)_dio_hw_error == 0x0C)
#परिभाषय HW_BAD_CRC_ECC()    ((अचिह्नित वर्ण)_dio_hw_error == 0x10)
#परिभाषय HW_CRC_ECC_CORR()   ((अचिह्नित वर्ण)_dio_hw_error == 0x11)
#परिभाषय HW_CONTR_FAIL()     ((अचिह्नित वर्ण)_dio_hw_error == 0x20)
#परिभाषय HW_SEEK_FAIL()      ((अचिह्नित वर्ण)_dio_hw_error == 0x40)
#परिभाषय HW_ATTACH_FAIL()    ((अचिह्नित वर्ण)_dio_hw_error == 0x80)
#परिभाषय HW_DRIVE_NREADY()   ((अचिह्नित वर्ण)_dio_hw_error == 0xAA)
#परिभाषय HW_UNDEF_ERROR()    ((अचिह्नित वर्ण)_dio_hw_error == 0xBB)
#परिभाषय HW_WRITE_FAULT()    ((अचिह्नित वर्ण)_dio_hw_error == 0xCC)
#परिभाषय HW_स्थितिः_ERROR()   ((अचिह्नित वर्ण)_dio_hw_error == 0xE0)
#परिभाषय HW_SENSE_FAIL()     ((अचिह्नित वर्ण)_dio_hw_error == 0xFF)


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

#यदि_अन्तः /* __diskio_h */
