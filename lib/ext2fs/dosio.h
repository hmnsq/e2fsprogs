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

#اگر_تعریف_نشده __diskio_h
#تعریف_کن __diskio_h
#اگر_تعریف_شده __TURBOC__
#اگر_تعریف_نشده __LARGE__
# error "ext2fs/DOS library requires LARGE model!"
#پایان_اگر
#پایان_اگر

#اگر_تعریف_شده __TURBOC__
#ضمن "msdos.h"
#پایان_اگر

/*
 * A helper structure used in LBA => CHS conversion
 */
تعريفنوع بنية
{
  بیعلامت کوتاه       cyl;     /* Cylinder (or track) */
  بیعلامت کوتاه       سر;
  بیعلامت کوتاه       sector;
  بیعلامت کوتاه       جابه_جایی;  /* Offset of byte within the sector */
} CHS;

/*
 * All partition data we need is here
 */
تعريفنوع بنية
{
  حرف                 *dev;  /* _Linux_ device name (like "/dev/hda1") */
  بیعلامت حرف        phys;  /* Physical DOS drive number */
  بیعلامت دراز        شروع; /* LBA address of partition start */
  بیعلامت دراز        len;   /* length of partition in sectors */
  بیعلامت حرف        pno;   /* Partition number (read from *dev) */

  /* This partition's drive geometry */
  بیعلامت کوتاه       cyls;
  بیعلامت کوتاه       heads;
  بیعلامت کوتاه       sects;
} افراز;

/*
 * PC partition table entry format
 */
#اگر_تعریف_شده __DJGPP__
#رهنمود pack(1)
#پایان_اگر
تعريفنوع بنية
{
  بیعلامت حرف        فعال;
  بیعلامت حرف        شروع_سر;
  بیعلامت حرف        شروع_sec;
  بیعلامت حرف        شروع_cyl;
  بیعلامت حرف        type;
  بیعلامت حرف        انتها_سر;
  بیعلامت حرف        انتها_sec;
  بیعلامت حرف        انتها_cyl;
  بیعلامت دراز        اولی_sec_rel;
  بیعلامت دراز        اندازه;
} PTABLE_مدخل;
#اگر_تعریف_شده __DJGPP__
#رهنمود pack()
#پایان_اگر

/*
 * INT 0x13 operation codes
 */
#تعریف_کن DISK_READ          0x02
#تعریف_کن DISK_WRITE         0x03
#تعریف_کن DISK_GET_GEOMETRY  0x08
#تعریف_کن DISK_READY         0x10

/*
 * Errors to put in _dio_error
 */
#تعریف_کن ERR_BADDEV         0x00000001L
#تعریف_کن ERR_HARDWARE       0x00000002L
#تعریف_کن ERR_NOTSUPP        0x00000003L
#تعریف_کن ERR_NOTEXT2FS      0x00000004L
#تعریف_کن ERR_EMPTYPART      0x00000005L
#تعریف_کن ERR_LINUXSWAP      0x00000006L

/*
 * Functions in diskio.c
 */

/*
 * Variable contains last module's error
 */
خارجي بیعلامت دراز        _dio_error;

/*
 * This one contains last hardware error (if _dio_error == ERR_HARDWARE)
 */
خارجي بیعلامت دراز        _dio_hw_error;

/*
 * Macros to check for disk hardware errors
 */
#تعریف_کن HW_OK()             ((بیعلامت حرف)_dio_hw_error == 0x00)
#تعریف_کن HW_BAD_CMD()        ((بیعلامت حرف)_dio_hw_error == 0x01)
#تعریف_کن HW_NO_ADDR_MARK()   ((بیعلامت حرف)_dio_hw_error == 0x02)
#تعریف_کن HW_WRITE_PROT()     ((بیعلامت حرف)_dio_hw_error == 0x03)
#تعریف_کن HW_NO_SECTOR()      ((بیعلامت حرف)_dio_hw_error == 0x04)
#تعریف_کن HW_RESET_FAIL()     ((بیعلامت حرف)_dio_hw_error == 0x05)
#تعریف_کن HW_DISK_تغییریافته()   ((بیعلامت حرف)_dio_hw_error == 0x06)
#تعریف_کن HW_DRIVE_FAIL()     ((بیعلامت حرف)_dio_hw_error == 0x07)
#تعریف_کن HW_DMA_OVERRUN()    ((بیعلامت حرف)_dio_hw_error == 0x08)
#تعریف_کن HW_DMA_BOUNDARY()   ((بیعلامت حرف)_dio_hw_error == 0x09)
#تعریف_کن HW_BAD_SECTOR()     ((بیعلامت حرف)_dio_hw_error == 0x0A)
#تعریف_کن HW_BAD_TRACK()      ((بیعلامت حرف)_dio_hw_error == 0x0B)
#تعریف_کن HW_UNSUPP_TRACK()   ((بیعلامت حرف)_dio_hw_error == 0x0C)
#تعریف_کن HW_BAD_بررسی_چرخهای_ECC()    ((بیعلامت حرف)_dio_hw_error == 0x10)
#تعریف_کن HW_بررسی_چرخهای_ECC_CORR()   ((بیعلامت حرف)_dio_hw_error == 0x11)
#تعریف_کن HW_CONTR_FAIL()     ((بیعلامت حرف)_dio_hw_error == 0x20)
#تعریف_کن HW_SEEK_FAIL()      ((بیعلامت حرف)_dio_hw_error == 0x40)
#تعریف_کن HW_ATTACH_FAIL()    ((بیعلامت حرف)_dio_hw_error == 0x80)
#تعریف_کن HW_DRIVE_NREADY()   ((بیعلامت حرف)_dio_hw_error == 0xAA)
#تعریف_کن HW_UNDEF_ERROR()    ((بیعلامت حرف)_dio_hw_error == 0xBB)
#تعریف_کن HW_WRITE_FAULT()    ((بیعلامت حرف)_dio_hw_error == 0xCC)
#تعریف_کن HW_وضعیت_ERROR()   ((بیعلامت حرف)_dio_hw_error == 0xE0)
#تعریف_کن HW_SENSE_FAIL()     ((بیعلامت حرف)_dio_hw_error == 0xFF)


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
صحيح open_افراز(حرف *dev);

#پایان_اگر /* __diskio_h */
