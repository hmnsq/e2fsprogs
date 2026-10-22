/*
 * getsize.c --- get the size of a partition.
 *
 * Copyright (C) 1995, 1995 Theodore Ts'o.
 * Copyright (C) 2003 VMware, Inc.
 *
 * Windows version of ext2fs_get_device_size by Chris Li, VMware.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#अगर_अघोषित _LARGEFILE_SOURCE
#घोषणा _LARGEFILE_SOURCE
#पूर्ण_अगर
#अगर_अघोषित _LARGEFILE64_SOURCE
#घोषणा _LARGEFILE64_SOURCE
#पूर्ण_अगर

#समावेश "config.h"
#समावेश <stdio.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#अगर HAVE_ERRNO_H
#समावेश <errno.h>
#पूर्ण_अगर
#समावेश <fcntl.h>
#अगर_घोषित HAVE_SYS_IOCTL_H
#समावेश <sys/ioctl.h>
#पूर्ण_अगर
#अगर_घोषित HAVE_LINUX_FD_H
#समावेश <linux/fd.h>
#पूर्ण_अगर
#अगर_घोषित HAVE_SYS_DISKLABEL_H
#समावेश <sys/disklabel.h>
#पूर्ण_अगर
#अगर_घोषित HAVE_SYS_DISK_H
#समावेश <sys/disk.h>
#पूर्ण_अगर
#अगर_घोषित __linux__
#समावेश <sys/utsname.h>
#पूर्ण_अगर
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#समावेश <ctype.h>

#अगर defined(__linux__) && defined(_IO) && !defined(BLKGETSIZE)
#घोषणा BLKGETSIZE _IO(0x12,96)	/* return device size */
#पूर्ण_अगर

#अगर defined(__linux__) && defined(_IOR) && !defined(BLKGETSIZE64)
#घोषणा BLKGETSIZE64 _IOR(0x12,114,माप_प्रकार)	/* return device size in bytes (u64 *arg) */
#पूर्ण_अगर

#अगर_घोषित APPLE_DARWIN
#घोषणा BLKGETSIZE DKIOCGETBLOCKCOUNT32
#पूर्ण_अगर /* APPLE_DARWIN */

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

#अगर defined(__CYGWIN__) || defined (WIN32)
#समावेश "windows.h"
#समावेश "winioctl.h"

#अगर (_WIN32_WINNT >= 0x0500)
#घोषणा HAVE_GET_FILE_आकार_EX 1
#पूर्ण_अगर

errcode_t ext2fs_get_device_size2(स्थिर अक्षर *file, पूर्णांक blocksize,
				  blk64_t *retblocks)
{
	पूर्णांक fd;
	HANDLE h;
	PARTITION_INFORMATION pi;
	DISK_GEOMETRY gi;
	DWORD retbytes;
#अगर_घोषित HAVE_GET_FILE_आकार_EX
	LARGE_INTEGER filesize;
#अन्यथा
	DWORD filesize;
#पूर्ण_अगर /* HAVE_GET_FILE_SIZE_EX */

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	अगर (fd < 0)
		वापस त्रुटि_सं;
	h = (HANDLE)_get_osfhandle(fd);
	अगर (DeviceIoControl(h, IOCTL_DISK_GET_PARTITION_INFO,
			    &pi, माप(PARTITION_INFORMATION),
			    &pi, माप(PARTITION_INFORMATION),
			    &retbytes, शून्य)) {

		*retblocks = pi.Partition_लंबाई.QuadPart / blocksize;

	} अन्यथा अगर (DeviceIoControl(h, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, माप(DISK_GEOMETRY),
				&gi, माप(DISK_GEOMETRY),
				&retbytes, शून्य)) {

		*retblocks = gi.BytesPerSector *
			     gi.SectorsPerTrack *
			     gi.TracksPerCylinder *
			     gi.Cylinders.QuadPart / blocksize;

#अगर_घोषित HAVE_GET_FILE_आकार_EX
	} अन्यथा अगर (Get_File_आकार_Ex(h, &filesize)) {
		*retblocks = filesize.QuadPart / blocksize;
	}
#अन्यथा
	} अन्यथा {
		filesize = Get_File_आकार(h, शून्य);
		अगर (INVALID_FILE_आकार != filesize) {
			*retblocks = filesize / blocksize;
		}
	}
#पूर्ण_अगर /* HAVE_GET_FILE_SIZE_EX */

	बंद(fd);
	वापस 0;
}

#अन्यथा

अटल पूर्णांक valid_offset (पूर्णांक fd, ext2_loff_t offset)
{
	अक्षर ch;

	अगर (ext2fs_llseek (fd, offset, 0) < 0)
		वापस 0;
	अगर (पढ़ो (fd, &ch, 1) < 1)
		वापस 0;
	वापस 1;
}

/*
 * Returns the number of blocks in a partition
 */
errcode_t ext2fs_get_device_size2(स्थिर अक्षर *file, पूर्णांक blocksize,
				  blk64_t *retblocks)
{
	पूर्णांक	fd, rc = 0;
	अचिन्हित दीर्घ दीर्घ size64;
	ext2_loff_t ऊपरी, निचला;

	fd = ext2fs_open_file(file, O_RDONLY, 0);
	अगर (fd < 0)
		वापस त्रुटि_सं;

#अगर defined DKIOCGETBLOCKCOUNT && defined DKIOCGETBLOCKSIZE	/* For Apple Darwin */
	अचिन्हित पूर्णांक आकार;

	अगर (ioctl(fd, DKIOCGETBLOCKCOUNT, &size64) >= 0 &&
	    ioctl(fd, DKIOCGETBLOCKSIZE, &आकार) >= 0) {
		*retblocks = size64 * आकार / blocksize;
		जाओ out;
	}
#पूर्ण_अगर

#अगर_घोषित BLKGETSIZE64
	{
		पूर्णांक valid_blkgetsize64 = 1;
#अगर_घोषित __linux__
		काष्ठा utsname ut;

		अगर ((uname(&ut) == 0) &&
		    ((ut.release[0] == '2') && (ut.release[1] == '.') &&
		     (ut.release[2] < '6') && (ut.release[3] == '.')))
			valid_blkgetsize64 = 0;
#पूर्ण_अगर
		अगर (valid_blkgetsize64 &&
		    ioctl(fd, BLKGETSIZE64, &size64) >= 0) {
			*retblocks = size64 / blocksize;
			जाओ out;
		}
	}
#पूर्ण_अगर /* BLKGETSIZE64 */

#अगर_घोषित BLKGETSIZE
	{
		अचिन्हित दीर्घ	आकार;

		अगर (ioctl(fd, BLKGETSIZE, &आकार) >= 0) {
			*retblocks = आकार / (blocksize / 512);
			जाओ out;
		}
	}
#पूर्ण_अगर

#अगर_घोषित FDGETPRM
	{
		काष्ठा floppy_struct this_floppy;

		अगर (ioctl(fd, FDGETPRM, &this_floppy) >= 0) {
			*retblocks = this_floppy.आकार / (blocksize / 512);
			जाओ out;
		}
	}
#पूर्ण_अगर

#अगर_घोषित HAVE_SYS_DISKLABEL_H
	{
		पूर्णांक part;
		काष्ठा disklabel lab;
		काष्ठा partition *pp;
		अक्षर ch;

#अगर defined(DIOCGMEDIASIZE)
		{
			off_t ms;
			u_int bs;
			अगर (ioctl(fd, DIOCGMEDIASIZE, &ms) >= 0) {
				*retblocks = ms / blocksize;
				जाओ out;
			}
		}
#या_अगर defined(DIOCGDINFO)
		/* old disklabel interface */
		part = म_माप(file) - 1;
		अगर (part >= 0) {
			ch = file[part];
			अगर (है_अंक(ch))
				part = 0;
			अन्यथा अगर (ch >= 'a' && ch <= 'h')
				part = ch - 'a';
			अन्यथा
				part = -1;
		}
		अगर (part >= 0 && (ioctl(fd, DIOCGDINFO, (अक्षर *)&lab) >= 0)) {
			pp = &lab.d_partitions[part];
			अगर (pp->p_आकार) {
				*retblocks = pp->p_आकार / (blocksize / 512);
				जाओ out;
			}
		}
#पूर्ण_अगर /* defined(DIOCG*) */
	}
#पूर्ण_अगर /* HAVE_SYS_DISKLABEL_H */

	{
		ext2fs_struct_stat st;

		अगर (ext2fs_fstat(fd, &st) == 0)
			अगर (S_ISREG(st.st_mode)) {
				*retblocks = st.st_आकार / blocksize;
				जाओ out;
			}
	}

	/*
	 * OK, we couldn't figure it out by using a specialized ioctl,
	 * which is generally the best way.  So do binary search to
	 * find the size of the partition.
	 */
	निचला = 0;
	क्रम (ऊपरी = 1024; valid_offset(fd, ऊपरी); ऊपरी *= 2)
		निचला = ऊपरी;
	जबतक (निचला < ऊपरी - 1) {
		स्थिर ext2_loff_t mid = (निचला + ऊपरी) / 2;

		अगर (valid_offset (fd, mid))
			निचला = mid;
		अन्यथा
			ऊपरी = mid;
	}
	valid_offset(fd, 0);
	size64 = निचला + 1;
	*retblocks = size64 / blocksize;
out:
	बंद(fd);
	वापस rc;
}

#पूर्ण_अगर /* WIN32 */

errcode_t ext2fs_get_device_आकार(स्थिर अक्षर *file, पूर्णांक blocksize,
				 blk_t *retblocks)
{
	errcode_t retval;
	blk64_t	blocks;

	retval = ext2fs_get_device_size2(file, blocksize, &blocks);
	अगर (retval)
		वापस retval;
	अगर (blocks >= (1ULL << 32))
		वापस EFBIG;
	*retblocks = (blk_t) blocks;
	वापस 0;
}

#अगर_घोषित DEBUG
पूर्णांक मुख्य(पूर्णांक argc, अक्षर **argv)
{
	blk64_t	blocks;
	पूर्णांक	retval;

	अगर (argc < 2) {
		ख_लिखो(मानक_त्रुटि, "Usage: %s device\n", argv[0]);
		निकास(1);
	}

	retval = ext2fs_get_device_size2(argv[1], 1024, &blocks);
	अगर (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_size");
		निकास(1);
	}
	म_लिखो("Device %s has %llu 1k blocks.\n", argv[1],
	       (अचिन्हित दीर्घ दीर्घ) locks);
	निकास(0);
}
#पूर्ण_अगर
