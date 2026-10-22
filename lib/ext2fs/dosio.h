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

#جيڪڏهن_وصف_نه_ٿيل __diskio_h
#وصف_ڪريو __diskio_h
#جيڪڏهن_وصف_ٿيل __TURBOC__
#جيڪڏهن_وصف_نه_ٿيل __LARGE__
# error "ext2fs/DOS library requires LARGE model!"
#جيڪڏهن_پورو
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل __TURBOC__
#ضمن "msdos.h"
#جيڪڏهن_پورو

/*
 * A helper structure used in LBA => CHS conversion
 */
تعريفنوع بنية
{
  بنانشان ننڍو       cyl;     /* Cylinder (or track) */
  بنانشان ننڍو       سر;
  بنانشان ننڍو       sector;
  بنانشان ننڍو       وٿي;  /* Offset of byte within the sector */
} CHS;

/*
 * All partition data we need is here
 */
تعريفنوع بنية
{
  حرف                 *dev;  /* _Linux_ device name (like "/dev/hda1") */
  بنانشان حرف        phys;  /* Physical DOS drive number */
  بنانشان ڊگهو        شروعات; /* LBA address of partition start */
  بنانشان ڊگهو        len;   /* length of partition in sectors */
  بنانشان حرف        pno;   /* Partition number (read from *dev) */

  /* This partition's drive geometry */
  بنانشان ننڍو       cyls;
  بنانشان ننڍو       heads;
  بنانشان ننڍو       sects;
} حصا_ڪريو;

/*
 * PC partition table entry format
 */
#جيڪڏهن_وصف_ٿيل __DJGPP__
#هدايت pack(1)
#جيڪڏهن_پورو
تعريفنوع بنية
{
  بنانشان حرف        سرگرم;
  بنانشان حرف        شروعات_سر;
  بنانشان حرف        شروعات_sec;
  بنانشان حرف        شروعات_cyl;
  بنانشان حرف        type;
  بنانشان حرف        پڇاڙي_سر;
  بنانشان حرف        پڇاڙي_sec;
  بنانشان حرف        پڇاڙي_cyl;
  بنانشان ڊگهو        پهريون_sec_rel;
  بنانشان ڊگهو        سائيز;
} PTABLE_داخلا;
#جيڪڏهن_وصف_ٿيل __DJGPP__
#هدايت pack()
#جيڪڏهن_پورو

/*
 * INT 0x13 operation codes
 */
#وصف_ڪريو DISK_READ          0x02
#وصف_ڪريو DISK_WRITE         0x03
#وصف_ڪريو DISK_GET_GEOMETRY  0x08
#وصف_ڪريو DISK_READY         0x10

/*
 * Errors to put in _dio_error
 */
#وصف_ڪريو ERR_BADDEV         0x00000001L
#وصف_ڪريو ERR_HARDWARE       0x00000002L
#وصف_ڪريو ERR_NOTSUPP        0x00000003L
#وصف_ڪريو ERR_NOTEXT2FS      0x00000004L
#وصف_ڪريو ERR_EMPTYPART      0x00000005L
#وصف_ڪريو ERR_LINUXSWAP      0x00000006L

/*
 * Functions in diskio.c
 */

/*
 * Variable contains last module's error
 */
خارجي بنانشان ڊگهو        _dio_error;

/*
 * This one contains last hardware error (if _dio_error == ERR_HARDWARE)
 */
خارجي بنانشان ڊگهو        _dio_hw_error;

/*
 * Macros to check for disk hardware errors
 */
#وصف_ڪريو HW_OK()             ((بنانشان حرف)_dio_hw_error == 0x00)
#وصف_ڪريو HW_BAD_CMD()        ((بنانشان حرف)_dio_hw_error == 0x01)
#وصف_ڪريو HW_NO_ADDR_MARK()   ((بنانشان حرف)_dio_hw_error == 0x02)
#وصف_ڪريو HW_WRITE_PROT()     ((بنانشان حرف)_dio_hw_error == 0x03)
#وصف_ڪريو HW_NO_SECTOR()      ((بنانشان حرف)_dio_hw_error == 0x04)
#وصف_ڪريو HW_RESET_FAIL()     ((بنانشان حرف)_dio_hw_error == 0x05)
#وصف_ڪريو HW_DISK_بدليو()   ((بنانشان حرف)_dio_hw_error == 0x06)
#وصف_ڪريو HW_DRIVE_FAIL()     ((بنانشان حرف)_dio_hw_error == 0x07)
#وصف_ڪريو HW_DMA_OVERRUN()    ((بنانشان حرف)_dio_hw_error == 0x08)
#وصف_ڪريو HW_DMA_BOUNDARY()   ((بنانشان حرف)_dio_hw_error == 0x09)
#وصف_ڪريو HW_BAD_SECTOR()     ((بنانشان حرف)_dio_hw_error == 0x0A)
#وصف_ڪريو HW_BAD_TRACK()      ((بنانشان حرف)_dio_hw_error == 0x0B)
#وصف_ڪريو HW_UNSUPP_TRACK()   ((بنانشان حرف)_dio_hw_error == 0x0C)
#وصف_ڪريو HW_BAD_چڪري_جاچ_ECC()    ((بنانشان حرف)_dio_hw_error == 0x10)
#وصف_ڪريو HW_چڪري_جاچ_ECC_CORR()   ((بنانشان حرف)_dio_hw_error == 0x11)
#وصف_ڪريو HW_CONTR_FAIL()     ((بنانشان حرف)_dio_hw_error == 0x20)
#وصف_ڪريو HW_SEEK_FAIL()      ((بنانشان حرف)_dio_hw_error == 0x40)
#وصف_ڪريو HW_ATTACH_FAIL()    ((بنانشان حرف)_dio_hw_error == 0x80)
#وصف_ڪريو HW_DRIVE_NREADY()   ((بنانشان حرف)_dio_hw_error == 0xAA)
#وصف_ڪريو HW_UNDEF_ERROR()    ((بنانشان حرف)_dio_hw_error == 0xBB)
#وصف_ڪريو HW_WRITE_FAULT()    ((بنانشان حرف)_dio_hw_error == 0xCC)
#وصف_ڪريو HW_حالت_ERROR()   ((بنانشان حرف)_dio_hw_error == 0xE0)
#وصف_ڪريو HW_SENSE_FAIL()     ((بنانشان حرف)_dio_hw_error == 0xFF)


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
صحيح open_حصا_ڪريو(حرف *dev);

#جيڪڏهن_پورو /* __diskio_h */
