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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __diskio_h
#ವ್ಯಾಖ್ಯಾನಿಸು __diskio_h
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __TURBOC__
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __LARGE__
# error "ext2fs/DOS library requires LARGE model!"
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __TURBOC__
#ಅನ್ತರ್ಭಾವ "msdos.h"
#ಆದರೆ_ಅಂತ್ಯ

/*
 * A helper structure used in LBA => CHS conversion
 */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
  ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ       cyl;     /* Cylinder (or track) */
  ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ       ತಲೆ;
  ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ       sector;
  ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ       ಸ್ಥಾನಾಂತರ;  /* Offset of byte within the sector */
} CHS;

/*
 * All partition data we need is here
 */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
  ಅಕ್ಷರ                 *dev;  /* _Linux_ device name (like "/dev/hda1") */
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ        phys;  /* Physical DOS drive number */
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ        ಆರಂಭ; /* LBA address of partition start */
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ        len;   /* length of partition in sectors */
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ        pno;   /* Partition number (read from *dev) */

  /* This partition's drive geometry */
  ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ       cyls;
  ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ       heads;
  ಚಿಹ್ನೆರಹಿತ ಗಿಡ್ಡ       sects;
} ವಿಭಜನೆ;

/*
 * PC partition table entry format
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __DJGPP__
#ಸೂಚನೆ pack(1)
#ಆದರೆ_ಅಂತ್ಯ
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ        ಸಕ್ರಿಯ;
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ        ಆರಂಭ_ತಲೆ;
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ        ಆರಂಭ_sec;
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ        ಆರಂಭ_cyl;
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ        type;
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ        ಕೊನೆ_ತಲೆ;
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ        ಕೊನೆ_sec;
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ        ಕೊನೆ_cyl;
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ        ಮೊದಲ_sec_rel;
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ        ಗಾತ್ರ_2;
} PTABLE_ನಮೂದು;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __DJGPP__
#ಸೂಚನೆ pack()
#ಆದರೆ_ಅಂತ್ಯ

/*
 * INT 0x13 operation codes
 */
#ವ್ಯಾಖ್ಯಾನಿಸು DISK_READ          0x02
#ವ್ಯಾಖ್ಯಾನಿಸು DISK_WRITE         0x03
#ವ್ಯಾಖ್ಯಾನಿಸು DISK_GET_GEOMETRY  0x08
#ವ್ಯಾಖ್ಯಾನಿಸು DISK_READY         0x10

/*
 * Errors to put in _dio_error
 */
#ವ್ಯಾಖ್ಯಾನಿಸು ERR_BADDEV         0x00000001L
#ವ್ಯಾಖ್ಯಾನಿಸು ERR_HARDWARE       0x00000002L
#ವ್ಯಾಖ್ಯಾನಿಸು ERR_NOTSUPP        0x00000003L
#ವ್ಯಾಖ್ಯಾನಿಸು ERR_NOTEXT2FS      0x00000004L
#ವ್ಯಾಖ್ಯಾನಿಸು ERR_EMPTYPART      0x00000005L
#ವ್ಯಾಖ್ಯಾನಿಸು ERR_LINUXSWAP      0x00000006L

/*
 * Functions in diskio.c
 */

/*
 * Variable contains last module's error
 */
ಬಾಹ್ಯ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ        _dio_error;

/*
 * This one contains last hardware error (if _dio_error == ERR_HARDWARE)
 */
ಬಾಹ್ಯ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ        _dio_hw_error;

/*
 * Macros to check for disk hardware errors
 */
#ವ್ಯಾಖ್ಯಾನಿಸು HW_OK()             ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)_dio_hw_error == 0x00)
#ವ್ಯಾಖ್ಯಾನಿಸು HW_BAD_CMD()        ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)_dio_hw_error == 0x01)
#ವ್ಯಾಖ್ಯಾನಿಸು HW_NO_ADDR_MARK()   ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)_dio_hw_error == 0x02)
#ವ್ಯಾಖ್ಯಾನಿಸು HW_WRITE_PROT()     ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)_dio_hw_error == 0x03)
#ವ್ಯಾಖ್ಯಾನಿಸು HW_NO_SECTOR()      ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)_dio_hw_error == 0x04)
#ವ್ಯಾಖ್ಯಾನಿಸು HW_RESET_FAIL()     ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)_dio_hw_error == 0x05)
#ವ್ಯಾಖ್ಯಾನಿಸು HW_DISK_ಬದಲಾಯಿತು()   ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)_dio_hw_error == 0x06)
#ವ್ಯಾಖ್ಯಾನಿಸು HW_DRIVE_FAIL()     ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)_dio_hw_error == 0x07)
#ವ್ಯಾಖ್ಯಾನಿಸು HW_DMA_OVERRUN()    ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)_dio_hw_error == 0x08)
#ವ್ಯಾಖ್ಯಾನಿಸು HW_DMA_BOUNDARY()   ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)_dio_hw_error == 0x09)
#ವ್ಯಾಖ್ಯಾನಿಸು HW_BAD_SECTOR()     ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)_dio_hw_error == 0x0A)
#ವ್ಯಾಖ್ಯಾನಿಸು HW_BAD_TRACK()      ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)_dio_hw_error == 0x0B)
#ವ್ಯಾಖ್ಯಾನಿಸು HW_UNSUPP_TRACK()   ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)_dio_hw_error == 0x0C)
#ವ್ಯಾಖ್ಯಾನಿಸು HW_BAD_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_ECC()    ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)_dio_hw_error == 0x10)
#ವ್ಯಾಖ್ಯಾನಿಸು HW_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_ECC_CORR()   ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)_dio_hw_error == 0x11)
#ವ್ಯಾಖ್ಯಾನಿಸು HW_CONTR_FAIL()     ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)_dio_hw_error == 0x20)
#ವ್ಯಾಖ್ಯಾನಿಸು HW_SEEK_FAIL()      ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)_dio_hw_error == 0x40)
#ವ್ಯಾಖ್ಯಾನಿಸು HW_ATTACH_FAIL()    ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)_dio_hw_error == 0x80)
#ವ್ಯಾಖ್ಯಾನಿಸು HW_DRIVE_NREADY()   ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)_dio_hw_error == 0xAA)
#ವ್ಯಾಖ್ಯಾನಿಸು HW_UNDEF_ERROR()    ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)_dio_hw_error == 0xBB)
#ವ್ಯಾಖ್ಯಾನಿಸು HW_WRITE_FAULT()    ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)_dio_hw_error == 0xCC)
#ವ್ಯಾಖ್ಯಾನಿಸು HW_ಸ್ಥಿತಿ_ERROR()   ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)_dio_hw_error == 0xE0)
#ವ್ಯಾಖ್ಯಾನಿಸು HW_SENSE_FAIL()     ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ)_dio_hw_error == 0xFF)


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
ಪೂರ್ಣಾಂಕ open_ವಿಭಜನೆ(ಅಕ್ಷರ *dev);

#ಆದರೆ_ಅಂತ್ಯ /* __diskio_h */
