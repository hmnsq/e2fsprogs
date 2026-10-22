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

#جے_بنا_تعریف __diskio_h
#تعریف_کرو __diskio_h
#جے_تعریف_شدہ __TURBOC__
#جے_بنا_تعریف __LARGE__
# error "ext2fs/DOS library requires LARGE model!"
#جے_انت
#جے_انت

#جے_تعریف_شدہ __TURBOC__
#ضمن "msdos.h"
#جے_انت

/*
 * A helper structure used in LBA => CHS conversion
 */
تعريفنوع بنية
{
  بنانشانی چھوٹا       cyl;     /* Cylinder (or track) */
  بنانشانی چھوٹا       سر;
  بنانشانی چھوٹا       sector;
  بنانشانی چھوٹا       وتھ;  /* Offset of byte within the sector */
} CHS;

/*
 * All partition data we need is here
 */
تعريفنوع بنية
{
  حرف                 *dev;  /* _Linux_ device name (like "/dev/hda1") */
  بنانشانی حرف        phys;  /* Physical DOS drive number */
  بنانشانی لما        شروع; /* LBA address of partition start */
  بنانشانی لما        len;   /* length of partition in sectors */
  بنانشانی حرف        pno;   /* Partition number (read from *dev) */

  /* This partition's drive geometry */
  بنانشانی چھوٹا       cyls;
  بنانشانی چھوٹا       heads;
  بنانشانی چھوٹا       sects;
} حصے_کرو;

/*
 * PC partition table entry format
 */
#جے_تعریف_شدہ __DJGPP__
#ہدایت pack(1)
#جے_انت
تعريفنوع بنية
{
  بنانشانی حرف        سرگرم;
  بنانشانی حرف        شروع_سر;
  بنانشانی حرف        شروع_sec;
  بنانشانی حرف        شروع_cyl;
  بنانشانی حرف        type;
  بنانشانی حرف        سرا_سر;
  بنانشانی حرف        سرا_sec;
  بنانشانی حرف        سرا_cyl;
  بنانشانی لما        پہلا_sec_rel;
  بنانشانی لما        ناپ;
} PTABLE_اندراج;
#جے_تعریف_شدہ __DJGPP__
#ہدایت pack()
#جے_انت

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
خارجي بنانشانی لما        _dio_error;

/*
 * This one contains last hardware error (if _dio_error == ERR_HARDWARE)
 */
خارجي بنانشانی لما        _dio_hw_error;

/*
 * Macros to check for disk hardware errors
 */
#تعریف_کرو HW_OK()             ((بنانشانی حرف)_dio_hw_error == 0x00)
#تعریف_کرو HW_BAD_CMD()        ((بنانشانی حرف)_dio_hw_error == 0x01)
#تعریف_کرو HW_NO_ADDR_MARK()   ((بنانشانی حرف)_dio_hw_error == 0x02)
#تعریف_کرو HW_WRITE_PROT()     ((بنانشانی حرف)_dio_hw_error == 0x03)
#تعریف_کرو HW_NO_SECTOR()      ((بنانشانی حرف)_dio_hw_error == 0x04)
#تعریف_کرو HW_RESET_FAIL()     ((بنانشانی حرف)_dio_hw_error == 0x05)
#تعریف_کرو HW_DISK_بدلیا()   ((بنانشانی حرف)_dio_hw_error == 0x06)
#تعریف_کرو HW_DRIVE_FAIL()     ((بنانشانی حرف)_dio_hw_error == 0x07)
#تعریف_کرو HW_DMA_OVERRUN()    ((بنانشانی حرف)_dio_hw_error == 0x08)
#تعریف_کرو HW_DMA_BOUNDARY()   ((بنانشانی حرف)_dio_hw_error == 0x09)
#تعریف_کرو HW_BAD_SECTOR()     ((بنانشانی حرف)_dio_hw_error == 0x0A)
#تعریف_کرو HW_BAD_TRACK()      ((بنانشانی حرف)_dio_hw_error == 0x0B)
#تعریف_کرو HW_UNSUPP_TRACK()   ((بنانشانی حرف)_dio_hw_error == 0x0C)
#تعریف_کرو HW_BAD_چکری_جانچ_ECC()    ((بنانشانی حرف)_dio_hw_error == 0x10)
#تعریف_کرو HW_چکری_جانچ_ECC_CORR()   ((بنانشانی حرف)_dio_hw_error == 0x11)
#تعریف_کرو HW_CONTR_FAIL()     ((بنانشانی حرف)_dio_hw_error == 0x20)
#تعریف_کرو HW_SEEK_FAIL()      ((بنانشانی حرف)_dio_hw_error == 0x40)
#تعریف_کرو HW_ATTACH_FAIL()    ((بنانشانی حرف)_dio_hw_error == 0x80)
#تعریف_کرو HW_DRIVE_NREADY()   ((بنانشانی حرف)_dio_hw_error == 0xAA)
#تعریف_کرو HW_UNDEF_ERROR()    ((بنانشانی حرف)_dio_hw_error == 0xBB)
#تعریف_کرو HW_WRITE_FAULT()    ((بنانشانی حرف)_dio_hw_error == 0xCC)
#تعریف_کرو HW_حالت_ERROR()   ((بنانشانی حرف)_dio_hw_error == 0xE0)
#تعریف_کرو HW_SENSE_FAIL()     ((بنانشانی حرف)_dio_hw_error == 0xFF)


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
صحيح open_حصے_کرو(حرف *dev);

#جے_انت /* __diskio_h */
