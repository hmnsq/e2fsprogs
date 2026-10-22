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

#నిర్వచితం_కాకపోతే __diskio_h
#నిర్వచించు __diskio_h
#నిర్వచితమైతే __TURBOC__
#నిర్వచితం_కాకపోతే __LARGE__
# error "ext2fs/DOS library requires LARGE model!"
#అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితమైతే __TURBOC__
#అన్తర్భావ "msdos.h"
#అయితే_ముగింపు

/*
 * A helper structure used in LBA => CHS conversion
 */
రకనిర్వచనం నిర్మాణం
{
  గుర్తులేని పొట్టి       cyl;     /* Cylinder (or track) */
  గుర్తులేని పొట్టి       తల;
  గుర్తులేని పొట్టి       sector;
  గుర్తులేని పొట్టి       స్థానభ్రంశం;  /* Offset of byte within the sector */
} CHS;

/*
 * All partition data we need is here
 */
రకనిర్వచనం నిర్మాణం
{
  అక్షరం                 *dev;  /* _Linux_ device name (like "/dev/hda1") */
  గుర్తులేని అక్షరం        phys;  /* Physical DOS drive number */
  గుర్తులేని దీర్ఘ        ప్రారంభం; /* LBA address of partition start */
  గుర్తులేని దీర్ఘ        len;   /* length of partition in sectors */
  గుర్తులేని అక్షరం        pno;   /* Partition number (read from *dev) */

  /* This partition's drive geometry */
  గుర్తులేని పొట్టి       cyls;
  గుర్తులేని పొట్టి       heads;
  గుర్తులేని పొట్టి       sects;
} విభజన;

/*
 * PC partition table entry format
 */
#నిర్వచితమైతే __DJGPP__
#సూచన pack(1)
#అయితే_ముగింపు
రకనిర్వచనం నిర్మాణం
{
  గుర్తులేని అక్షరం        చురుకైన;
  గుర్తులేని అక్షరం        ప్రారంభం_తల;
  గుర్తులేని అక్షరం        ప్రారంభం_sec;
  గుర్తులేని అక్షరం        ప్రారంభం_cyl;
  గుర్తులేని అక్షరం        type;
  గుర్తులేని అక్షరం        చివర_తల;
  గుర్తులేని అక్షరం        చివర_sec;
  గుర్తులేని అక్షరం        చివర_cyl;
  గుర్తులేని దీర్ఘ        మొదటి_sec_rel;
  గుర్తులేని దీర్ఘ        కొలత;
} PTABLE_నమోదు;
#నిర్వచితమైతే __DJGPP__
#సూచన pack()
#అయితే_ముగింపు

/*
 * INT 0x13 operation codes
 */
#నిర్వచించు DISK_READ          0x02
#నిర్వచించు DISK_WRITE         0x03
#నిర్వచించు DISK_GET_GEOMETRY  0x08
#నిర్వచించు DISK_READY         0x10

/*
 * Errors to put in _dio_error
 */
#నిర్వచించు ERR_BADDEV         0x00000001L
#నిర్వచించు ERR_HARDWARE       0x00000002L
#నిర్వచించు ERR_NOTSUPP        0x00000003L
#నిర్వచించు ERR_NOTEXT2FS      0x00000004L
#నిర్వచించు ERR_EMPTYPART      0x00000005L
#నిర్వచించు ERR_LINUXSWAP      0x00000006L

/*
 * Functions in diskio.c
 */

/*
 * Variable contains last module's error
 */
బాహ్య గుర్తులేని దీర్ఘ        _dio_error;

/*
 * This one contains last hardware error (if _dio_error == ERR_HARDWARE)
 */
బాహ్య గుర్తులేని దీర్ఘ        _dio_hw_error;

/*
 * Macros to check for disk hardware errors
 */
#నిర్వచించు HW_OK()             ((గుర్తులేని అక్షరం)_dio_hw_error == 0x00)
#నిర్వచించు HW_BAD_CMD()        ((గుర్తులేని అక్షరం)_dio_hw_error == 0x01)
#నిర్వచించు HW_NO_ADDR_MARK()   ((గుర్తులేని అక్షరం)_dio_hw_error == 0x02)
#నిర్వచించు HW_WRITE_PROT()     ((గుర్తులేని అక్షరం)_dio_hw_error == 0x03)
#నిర్వచించు HW_NO_SECTOR()      ((గుర్తులేని అక్షరం)_dio_hw_error == 0x04)
#నిర్వచించు HW_RESET_FAIL()     ((గుర్తులేని అక్షరం)_dio_hw_error == 0x05)
#నిర్వచించు HW_DISK_మారింది()   ((గుర్తులేని అక్షరం)_dio_hw_error == 0x06)
#నిర్వచించు HW_DRIVE_FAIL()     ((గుర్తులేని అక్షరం)_dio_hw_error == 0x07)
#నిర్వచించు HW_DMA_OVERRUN()    ((గుర్తులేని అక్షరం)_dio_hw_error == 0x08)
#నిర్వచించు HW_DMA_BOUNDARY()   ((గుర్తులేని అక్షరం)_dio_hw_error == 0x09)
#నిర్వచించు HW_BAD_SECTOR()     ((గుర్తులేని అక్షరం)_dio_hw_error == 0x0A)
#నిర్వచించు HW_BAD_TRACK()      ((గుర్తులేని అక్షరం)_dio_hw_error == 0x0B)
#నిర్వచించు HW_UNSUPP_TRACK()   ((గుర్తులేని అక్షరం)_dio_hw_error == 0x0C)
#నిర్వచించు HW_BAD_చక్రీయ_తనిఖీ_ECC()    ((గుర్తులేని అక్షరం)_dio_hw_error == 0x10)
#నిర్వచించు HW_చక్రీయ_తనిఖీ_ECC_CORR()   ((గుర్తులేని అక్షరం)_dio_hw_error == 0x11)
#నిర్వచించు HW_CONTR_FAIL()     ((గుర్తులేని అక్షరం)_dio_hw_error == 0x20)
#నిర్వచించు HW_SEEK_FAIL()      ((గుర్తులేని అక్షరం)_dio_hw_error == 0x40)
#నిర్వచించు HW_ATTACH_FAIL()    ((గుర్తులేని అక్షరం)_dio_hw_error == 0x80)
#నిర్వచించు HW_DRIVE_NREADY()   ((గుర్తులేని అక్షరం)_dio_hw_error == 0xAA)
#నిర్వచించు HW_UNDEF_ERROR()    ((గుర్తులేని అక్షరం)_dio_hw_error == 0xBB)
#నిర్వచించు HW_WRITE_FAULT()    ((గుర్తులేని అక్షరం)_dio_hw_error == 0xCC)
#నిర్వచించు HW_స్థితి_ERROR()   ((గుర్తులేని అక్షరం)_dio_hw_error == 0xE0)
#నిర్వచించు HW_SENSE_FAIL()     ((గుర్తులేని అక్షరం)_dio_hw_error == 0xFF)


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
పూర్ణాంకం open_విభజన(అక్షరం *dev);

#అయితే_ముగింపు /* __diskio_h */
