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

#اگر_غیر_تعریف_شدہ __diskio_h
#تعریف_کرو __diskio_h
#اگر_تعریف_شدہ __TURBOC__
#اگر_غیر_تعریف_شدہ __LARGE__
# error "ext2fs/DOS library requires LARGE model!"
#اگر_ختم
#اگر_ختم

#اگر_تعریف_شدہ __TURBOC__
#شامل "msdos.h"
#اگر_ختم

/*
 * A helper structure used in LBA => CHS conversion
 */
قسمتعریف ساخت
{
  غیرعلامتی مختصر       cyl;     /* Cylinder (or track) */
  غیرعلامتی مختصر       سر;
  غیرعلامتی مختصر       sector;
  غیرعلامتی مختصر       انحراف;  /* Offset of byte within the sector */
} CHS;

/*
 * All partition data we need is here
 */
قسمتعریف ساخت
{
  حرف                 *dev;  /* _Linux_ device name (like "/dev/hda1") */
  غیرعلامتی حرف        phys;  /* Physical DOS drive number */
  غیرعلامتی طویل        آغاز; /* LBA address of partition start */
  غیرعلامتی طویل        len;   /* length of partition in sectors */
  غیرعلامتی حرف        pno;   /* Partition number (read from *dev) */

  /* This partition's drive geometry */
  غیرعلامتی مختصر       cyls;
  غیرعلامتی مختصر       heads;
  غیرعلامتی مختصر       sects;
} تقسیم;

/*
 * PC partition table entry format
 */
#اگر_تعریف_شدہ __DJGPP__
#ہدایت pack(1)
#اگر_ختم
قسمتعریف ساخت
{
  غیرعلامتی حرف        فعال;
  غیرعلامتی حرف        آغاز_سر;
  غیرعلامتی حرف        آغاز_sec;
  غیرعلامتی حرف        آغاز_cyl;
  غیرعلامتی حرف        type;
  غیرعلامتی حرف        انتہا_سر;
  غیرعلامتی حرف        انتہا_sec;
  غیرعلامتی حرف        انتہا_cyl;
  غیرعلامتی طویل        پہلا_sec_rel;
  غیرعلامتی طویل        سائز;
} PTABLE_اندراج;
#اگر_تعریف_شدہ __DJGPP__
#ہدایت pack()
#اگر_ختم

/*
 * INT 0x13 operation codes
 */
#تعریف_کرو DISK_READ          0x02
#تعریف_کرو DISK_WRITE         0x03
#تعریف_کرو DISK_GET_GEOMETRY  0x08
#تعریف_کرو DISK_READY         0x10

/*
 * Errors to put in _dio_error
 */
#تعریف_کرو ERR_BADDEV         0x00000001L
#تعریف_کرو ERR_HARDWARE       0x00000002L
#تعریف_کرو ERR_NOTSUPP        0x00000003L
#تعریف_کرو ERR_NOTEXT2FS      0x00000004L
#تعریف_کرو ERR_EMPTYPART      0x00000005L
#تعریف_کرو ERR_LINUXSWAP      0x00000006L

/*
 * Functions in diskio.c
 */

/*
 * Variable contains last module's error
 */
خارجی غیرعلامتی طویل        _dio_error;

/*
 * This one contains last hardware error (if _dio_error == ERR_HARDWARE)
 */
خارجی غیرعلامتی طویل        _dio_hw_error;

/*
 * Macros to check for disk hardware errors
 */
#تعریف_کرو HW_OK()             ((غیرعلامتی حرف)_dio_hw_error == 0x00)
#تعریف_کرو HW_BAD_CMD()        ((غیرعلامتی حرف)_dio_hw_error == 0x01)
#تعریف_کرو HW_NO_ADDR_MARK()   ((غیرعلامتی حرف)_dio_hw_error == 0x02)
#تعریف_کرو HW_WRITE_PROT()     ((غیرعلامتی حرف)_dio_hw_error == 0x03)
#تعریف_کرو HW_NO_SECTOR()      ((غیرعلامتی حرف)_dio_hw_error == 0x04)
#تعریف_کرو HW_RESET_FAIL()     ((غیرعلامتی حرف)_dio_hw_error == 0x05)
#تعریف_کرو HW_DISK_بدلا()   ((غیرعلامتی حرف)_dio_hw_error == 0x06)
#تعریف_کرو HW_DRIVE_FAIL()     ((غیرعلامتی حرف)_dio_hw_error == 0x07)
#تعریف_کرو HW_DMA_OVERRUN()    ((غیرعلامتی حرف)_dio_hw_error == 0x08)
#تعریف_کرو HW_DMA_BOUNDARY()   ((غیرعلامتی حرف)_dio_hw_error == 0x09)
#تعریف_کرو HW_BAD_SECTOR()     ((غیرعلامتی حرف)_dio_hw_error == 0x0A)
#تعریف_کرو HW_BAD_TRACK()      ((غیرعلامتی حرف)_dio_hw_error == 0x0B)
#تعریف_کرو HW_UNSUPP_TRACK()   ((غیرعلامتی حرف)_dio_hw_error == 0x0C)
#تعریف_کرو HW_BAD_چکری_جانچ_ECC()    ((غیرعلامتی حرف)_dio_hw_error == 0x10)
#تعریف_کرو HW_چکری_جانچ_ECC_CORR()   ((غیرعلامتی حرف)_dio_hw_error == 0x11)
#تعریف_کرو HW_CONTR_FAIL()     ((غیرعلامتی حرف)_dio_hw_error == 0x20)
#تعریف_کرو HW_SEEK_FAIL()      ((غیرعلامتی حرف)_dio_hw_error == 0x40)
#تعریف_کرو HW_ATTACH_FAIL()    ((غیرعلامتی حرف)_dio_hw_error == 0x80)
#تعریف_کرو HW_DRIVE_NREADY()   ((غیرعلامتی حرف)_dio_hw_error == 0xAA)
#تعریف_کرو HW_UNDEF_ERROR()    ((غیرعلامتی حرف)_dio_hw_error == 0xBB)
#تعریف_کرو HW_WRITE_FAULT()    ((غیرعلامتی حرف)_dio_hw_error == 0xCC)
#تعریف_کرو HW_حالت_ERROR()   ((غیرعلامتی حرف)_dio_hw_error == 0xE0)
#تعریف_کرو HW_SENSE_FAIL()     ((غیرعلامتی حرف)_dio_hw_error == 0xFF)


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
صحیح open_تقسیم(حرف *dev);

#اگر_ختم /* __diskio_h */
