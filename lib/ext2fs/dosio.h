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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __diskio_h
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __diskio_h
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __TURBOC__
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __LARGE__
# error "ext2fs/DOS library requires LARGE model!"
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __TURBOC__
#ਅਨ੍ਤਰ੍ਭਾਵ "msdos.h"
#ਜੇ_ਅੰਤ

/*
 * A helper structure used in LBA => CHS conversion
 */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ       cyl;     /* Cylinder (or track) */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ       ਸਿਰ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ       sector;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ       ਵਿੱਥ;  /* Offset of byte within the sector */
} CHS;

/*
 * All partition data we need is here
 */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
  ਅੱਖਰ                 *dev;  /* _Linux_ device name (like "/dev/hda1") */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ        phys;  /* Physical DOS drive number */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ        ਸ਼ੁਰੂ; /* LBA address of partition start */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ        len;   /* length of partition in sectors */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ        pno;   /* Partition number (read from *dev) */

  /* This partition's drive geometry */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ       cyls;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ       heads;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ       sects;
} ਹਿੱਸੇ_ਕਰੋ;

/*
 * PC partition table entry format
 */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __DJGPP__
#ਹਦਾਇਤ pack(1)
#ਜੇ_ਅੰਤ
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ        ਸਰਗਰਮ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ        ਸ਼ੁਰੂ_ਸਿਰ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ        ਸ਼ੁਰੂ_sec;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ        ਸ਼ੁਰੂ_cyl;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ        type;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ        ਸਿਰਾ_ਸਿਰ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ        ਸਿਰਾ_sec;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ        ਸਿਰਾ_cyl;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ        ਪਹਿਲਾ_sec_rel;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ        ਮਾਪ;
} PTABLE_ਇੰਦਰਾਜ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __DJGPP__
#ਹਦਾਇਤ pack()
#ਜੇ_ਅੰਤ

/*
 * INT 0x13 operation codes
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISK_READ          0x02
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISK_WRITE         0x03
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISK_GET_GEOMETRY  0x08
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DISK_READY         0x10

/*
 * Errors to put in _dio_error
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ERR_BADDEV         0x00000001L
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ERR_HARDWARE       0x00000002L
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ERR_NOTSUPP        0x00000003L
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ERR_NOTEXT2FS      0x00000004L
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ERR_EMPTYPART      0x00000005L
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ERR_LINUXSWAP      0x00000006L

/*
 * Functions in diskio.c
 */

/*
 * Variable contains last module's error
 */
ਬਾਹਰੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ        _dio_error;

/*
 * This one contains last hardware error (if _dio_error == ERR_HARDWARE)
 */
ਬਾਹਰੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ        _dio_hw_error;

/*
 * Macros to check for disk hardware errors
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HW_OK()             ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)_dio_hw_error == 0x00)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HW_BAD_CMD()        ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)_dio_hw_error == 0x01)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HW_NO_ADDR_MARK()   ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)_dio_hw_error == 0x02)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HW_WRITE_PROT()     ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)_dio_hw_error == 0x03)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HW_NO_SECTOR()      ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)_dio_hw_error == 0x04)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HW_RESET_FAIL()     ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)_dio_hw_error == 0x05)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HW_DISK_ਬਦਲਿਆ()   ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)_dio_hw_error == 0x06)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HW_DRIVE_FAIL()     ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)_dio_hw_error == 0x07)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HW_DMA_OVERRUN()    ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)_dio_hw_error == 0x08)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HW_DMA_BOUNDARY()   ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)_dio_hw_error == 0x09)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HW_BAD_SECTOR()     ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)_dio_hw_error == 0x0A)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HW_BAD_TRACK()      ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)_dio_hw_error == 0x0B)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HW_UNSUPP_TRACK()   ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)_dio_hw_error == 0x0C)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HW_BAD_ਚੱਕਰੀ_ਜਾਂਚ_ECC()    ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)_dio_hw_error == 0x10)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HW_ਚੱਕਰੀ_ਜਾਂਚ_ECC_CORR()   ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)_dio_hw_error == 0x11)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HW_CONTR_FAIL()     ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)_dio_hw_error == 0x20)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HW_SEEK_FAIL()      ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)_dio_hw_error == 0x40)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HW_ATTACH_FAIL()    ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)_dio_hw_error == 0x80)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HW_DRIVE_NREADY()   ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)_dio_hw_error == 0xAA)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HW_UNDEF_ERROR()    ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)_dio_hw_error == 0xBB)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HW_WRITE_FAULT()    ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)_dio_hw_error == 0xCC)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HW_ਸਥਿਤੀ_ERROR()   ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)_dio_hw_error == 0xE0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HW_SENSE_FAIL()     ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ)_dio_hw_error == 0xFF)


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
ਪੂਰਨ open_ਹਿੱਸੇ_ਕਰੋ(ਅੱਖਰ *dev);

#ਜੇ_ਅੰਤ /* __diskio_h */
