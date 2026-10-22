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

#که_نه_تعریف_شوی __diskio_h
#تعریف_کړه __diskio_h
#که_تعریف_شوی __TURBOC__
#که_نه_تعریف_شوی __LARGE__
# error "ext2fs/DOS library requires LARGE model!"
#د_که_پای
#د_که_پای

#که_تعریف_شوی __TURBOC__
#ضمن "msdos.h"
#د_که_پای

/*
 * A helper structure used in LBA => CHS conversion
 */
تعريفنوع بنية
{
  بینښه لنډ       cyl;     /* Cylinder (or track) */
  بینښه لنډ       سر_2;
  بینښه لنډ       sector;
  بینښه لنډ       دننه_واټن;  /* Offset of byte within the sector */
} CHS;

/*
 * All partition data we need is here
 */
تعريفنوع بنية
{
  حرف                 *dev;  /* _Linux_ device name (like "/dev/hda1") */
  بینښه حرف        phys;  /* Physical DOS drive number */
  بینښه اوږد        پیل; /* LBA address of partition start */
  بینښه اوږد        len;   /* length of partition in sectors */
  بینښه حرف        pno;   /* Partition number (read from *dev) */

  /* This partition's drive geometry */
  بینښه لنډ       cyls;
  بینښه لنډ       heads;
  بینښه لنډ       sects;
} برخه_کول;

/*
 * PC partition table entry format
 */
#که_تعریف_شوی __DJGPP__
#لارښوونه pack(1)
#د_که_پای
تعريفنوع بنية
{
  بینښه حرف        فعال;
  بینښه حرف        پیل_سر;
  بینښه حرف        پیل_sec;
  بینښه حرف        پیل_cyl;
  بینښه حرف        type;
  بینښه حرف        پای_ټکی_سر;
  بینښه حرف        پای_ټکی_sec;
  بینښه حرف        پای_ټکی_cyl;
  بینښه اوږد        لومړی_sec_rel;
  بینښه اوږد        اندازه;
} PTABLE_ثبت;
#که_تعریف_شوی __DJGPP__
#لارښوونه pack()
#د_که_پای

/*
 * INT 0x13 operation codes
 */
#تعریف_کړه DISK_READ          0x02
#تعریف_کړه DISK_WRITE         0x03
#تعریف_کړه DISK_GET_GEOMETRY  0x08
#تعریف_کړه DISK_READY         0x10

/*
 * Errors to put in _dio_error
 */
#تعریف_کړه ERR_BADDEV         0x00000001L
#تعریف_کړه ERR_HARDWARE       0x00000002L
#تعریف_کړه ERR_NOTSUPP        0x00000003L
#تعریف_کړه ERR_NOTEXT2FS      0x00000004L
#تعریف_کړه ERR_EMPTYPART      0x00000005L
#تعریف_کړه ERR_LINUXSWAP      0x00000006L

/*
 * Functions in diskio.c
 */

/*
 * Variable contains last module's error
 */
خارجي بینښه اوږد        _dio_error;

/*
 * This one contains last hardware error (if _dio_error == ERR_HARDWARE)
 */
خارجي بینښه اوږد        _dio_hw_error;

/*
 * Macros to check for disk hardware errors
 */
#تعریف_کړه HW_OK()             ((بینښه حرف)_dio_hw_error == 0x00)
#تعریف_کړه HW_BAD_CMD()        ((بینښه حرف)_dio_hw_error == 0x01)
#تعریف_کړه HW_NO_ADDR_MARK()   ((بینښه حرف)_dio_hw_error == 0x02)
#تعریف_کړه HW_WRITE_PROT()     ((بینښه حرف)_dio_hw_error == 0x03)
#تعریف_کړه HW_NO_SECTOR()      ((بینښه حرف)_dio_hw_error == 0x04)
#تعریف_کړه HW_RESET_FAIL()     ((بینښه حرف)_dio_hw_error == 0x05)
#تعریف_کړه HW_DISK_بدل_شوی()   ((بینښه حرف)_dio_hw_error == 0x06)
#تعریف_کړه HW_DRIVE_FAIL()     ((بینښه حرف)_dio_hw_error == 0x07)
#تعریف_کړه HW_DMA_OVERRUN()    ((بینښه حرف)_dio_hw_error == 0x08)
#تعریف_کړه HW_DMA_BOUNDARY()   ((بینښه حرف)_dio_hw_error == 0x09)
#تعریف_کړه HW_BAD_SECTOR()     ((بینښه حرف)_dio_hw_error == 0x0A)
#تعریف_کړه HW_BAD_TRACK()      ((بینښه حرف)_dio_hw_error == 0x0B)
#تعریف_کړه HW_UNSUPP_TRACK()   ((بینښه حرف)_dio_hw_error == 0x0C)
#تعریف_کړه HW_BAD_دوراني_ازموینه_ECC()    ((بینښه حرف)_dio_hw_error == 0x10)
#تعریف_کړه HW_دوراني_ازموینه_ECC_CORR()   ((بینښه حرف)_dio_hw_error == 0x11)
#تعریف_کړه HW_CONTR_FAIL()     ((بینښه حرف)_dio_hw_error == 0x20)
#تعریف_کړه HW_SEEK_FAIL()      ((بینښه حرف)_dio_hw_error == 0x40)
#تعریف_کړه HW_ATTACH_FAIL()    ((بینښه حرف)_dio_hw_error == 0x80)
#تعریف_کړه HW_DRIVE_NREADY()   ((بینښه حرف)_dio_hw_error == 0xAA)
#تعریف_کړه HW_UNDEF_ERROR()    ((بینښه حرف)_dio_hw_error == 0xBB)
#تعریف_کړه HW_WRITE_FAULT()    ((بینښه حرف)_dio_hw_error == 0xCC)
#تعریف_کړه HW_حالت_ERROR()   ((بینښه حرف)_dio_hw_error == 0xE0)
#تعریف_کړه HW_SENSE_FAIL()     ((بینښه حرف)_dio_hw_error == 0xFF)


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
صحيح open_برخه_کول(حرف *dev);

#د_که_پای /* __diskio_h */
