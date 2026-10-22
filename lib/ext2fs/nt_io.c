/*
 * nt_io.c --- This is the Nt I/O interface to the I/O manager.
 *
 * Implements a one-block write-through cache.
 *
 * Copyright (C) 1993, 1994, 1995 Theodore Ts'o.
 * Copyright (C) 1998 Andrey Shedel (andreys@ns.cr.cyco.com)
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ifdef HAVE_CONFIG_H
#অন্তর্ভাব "config.h"
#endif


//
// I need some warnings to disable...
//


#pragma warning(disable:4514) // unreferenced inline function has been removed
#pragma warning(ঠেলো,4)

#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union)
#pragma warning(disable:4214) // nonstandard extension used : bit field types other than int
#pragma warning(disable:4115) // named type definition in parentheses

#অন্তর্ভাব <ntddk.h>
#অন্তর্ভাব <ntdddisk.h>
#অন্তর্ভাব <ntstatus.h>

#pragma warning(বের_করো)


//
// Some native APIs.
//

NTSYSAPI
ULONG
NTAPI
RtlNtStatusToDosError(
    IN NTSTATUS Status
   );

NTSYSAPI
NTSTATUS
NTAPI
NtClose(
    IN HANDLE Handle
   );


NTSYSAPI
NTSTATUS
NTAPI
NtOpenFile(
    OUT PHANDLE FileHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG ShareAccess,
    IN ULONG OpenOptions
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFlushBuffersFile(
    IN HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock
   );


NTSYSAPI
NTSTATUS
NTAPI
NtReadFile(
    IN HANDLE FileHandle,
    IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    OUT PVOID বাফার_2,
    IN ULONG দৈর্ঘ্য_2,
    IN PLARGE_INTEGER ByteOffset OPTIONAL,
    IN PULONG চাবি_2 OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtWriteFile(
    IN HANDLE FileHandle,
    IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PVOID বাফার_2,
    IN ULONG দৈর্ঘ্য_2,
    IN PLARGE_INTEGER ByteOffset OPTIONAL,
    IN PULONG চাবি_2 OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtDeviceIoControlFile(
    IN HANDLE FileHandle,
    IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG IoControlCode,
    IN PVOID প্রবেশ_বাফার OPTIONAL,
    IN ULONG প্রবেশ_বাফার_দৈর্ঘ্য,
    OUT PVOID Output_বাফার OPTIONAL,
    IN ULONG Output_বাফার_দৈর্ঘ্য
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFsControlFile(
    IN HANDLE FileHandle,
    IN HANDLE Event OPTIONAL,
    IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG IoControlCode,
    IN PVOID প্রবেশ_বাফার OPTIONAL,
    IN ULONG প্রবেশ_বাফার_দৈর্ঘ্য,
    OUT PVOID Output_বাফার OPTIONAL,
    IN ULONG Output_বাফার_দৈর্ঘ্য
    );


NTSYSAPI
NTSTATUS
NTAPI
NtDelayExecution(
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER Interval
    );


#define FSCTL_LOCK_VOLUME               CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 6, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_UNLOCK_VOLUME             CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 7, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_DISMOUNT_VOLUME           CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 8, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_IS_VOLUME_MOUNTED         CTL_CODE(FILE_DEVICE_FILE_SYSTEM,10, METHOD_BUFFERED, FILE_ANY_ACCESS)


//
// useful macros
//

#define BooleanFlagOn(Flags,SingleFlag) ((BOOLEAN)((((Flags) & (SingleFlag)) != 0)))


//
// Include Win32 error codes.
//

#অন্তর্ভাব <winerror.h>

//
// standard stuff
//

#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <malloc.h>

#অন্তর্ভাব <linux/types.h>
#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব <errno.h>

#অন্তর্ভাব "et/com_err.h"
#অন্তর্ভাব "ext2fs/ext2fs.h"
#অন্তর্ভাব "ext2fs/ext2_err.h"




//
// For checking structure magic numbers...
//


#define EXT2_CHECK_MAGIC(কাঠামো, code) \
	  যদি ((কাঠামো)->magic != (code)) ফেরত (code)

#define EXT2_ET_MAGIC_NT_IO_CHANNEL  0x10ed


//
// Private data block
//

প্রকারলেখ কাঠামো NT_PRIVATE_তথ্য {
	পূর্ণসংখ্যা	   magic;
	HANDLE Handle;
	পূর্ণসংখ্যা	   Flags;
	PCHAR  বাফার_2;
	__u32  বাফার_Block_সংখ্যা;
	ULONG  বাফার_আকার;
	BOOLEAN OpenedReadonly;
	BOOLEAN Written;
}NT_PRIVATE_তথ্য_2, *PNT_PRIVATE_তথ্য;



//
// Standard interface prototypes
//

স্থির errcode_t nt_open(ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা flags, io_channel *channel);
স্থির errcode_t nt_close(io_channel channel);
স্থির errcode_t nt_set_blksize(io_channel channel, পূর্ণসংখ্যা blksize);
স্থির errcode_t nt_read_blk(io_channel channel, unsigned long block,
			       পূর্ণসংখ্যা গুনতি, শূন্য *তথ্য);
স্থির errcode_t nt_write_blk(io_channel channel, unsigned long block,
				পূর্ণসংখ্যা গুনতি, ধ্রুবক শূন্য *তথ্য);
স্থির errcode_t nt_flush(io_channel channel);

স্থির কাঠামো struct_io_manager struct_nt_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "NT I/O Manager",
	.open		= nt_open,
	.close		= nt_close,
	.set_blksize	= nt_set_blksize,
	.read_blk	= nt_read_blk,
	.write_blk	= nt_write_blk,
	.flush		= nt_flush
};

//
// function to get API
//

io_manager nt_io_manager()
{
	ফেরত &struct_nt_manager;
}





//
// This is a code to convert Win32 errors to unix errno
//

প্রকারলেখ কাঠামো {
	ULONG WinError;
	পূর্ণসংখ্যা errnocode;
}ERROR_ENTRY;

স্থির ERROR_ENTRY ErrorTable[] = {
        {  ERROR_INVALID_FUNCTION,       EINVAL    },
        {  ERROR_FILE_NOT_FOUND,         ENOENT    },
        {  ERROR_PATH_NOT_FOUND,         ENOENT    },
        {  ERROR_TOO_MANY_OPEN_FILES,    EMFILE    },
        {  ERROR_ACCESS_DENIED,          EACCES    },
        {  ERROR_INVALID_HANDLE,         EBADF     },
        {  ERROR_ARENA_TRASHED,          ENOMEM    },
        {  ERROR_NOT_ENOUGH_MEMORY,      ENOMEM    },
        {  ERROR_INVALID_BLOCK,          ENOMEM    },
        {  ERROR_BAD_ENVIRONMENT,        E2BIG     },
        {  ERROR_BAD_FORMAT,             ENOEXEC   },
        {  ERROR_INVALID_ACCESS,         EINVAL    },
        {  ERROR_INVALID_তথ্য,           EINVAL    },
        {  ERROR_INVALID_DRIVE,          ENOENT    },
        {  ERROR_CURRENT_DIRECTORY,      EACCES    },
        {  ERROR_NOT_SAME_DEVICE,        EXDEV     },
        {  ERROR_NO_MORE_FILES,          ENOENT    },
        {  ERROR_LOCK_VIOLATION,         EACCES    },
        {  ERROR_BAD_NETPATH,            ENOENT    },
        {  ERROR_NETWORK_ACCESS_DENIED,  EACCES    },
        {  ERROR_BAD_NET_NAME,           ENOENT    },
        {  ERROR_FILE_EXISTS,            EEXIST    },
        {  ERROR_CANNOT_MAKE,            EACCES    },
        {  ERROR_FAIL_I24,               EACCES    },
        {  ERROR_INVALID_PARAMETER,      EINVAL    },
        {  ERROR_NO_PROC_SLOTS,          EAGAIN    },
        {  ERROR_DRIVE_LOCKED,           EACCES    },
        {  ERROR_BROKEN_PIPE,            EPIPE     },
        {  ERROR_DISK_FULL,              ENOSPC    },
        {  ERROR_INVALID_TARGET_HANDLE,  EBADF     },
        {  ERROR_INVALID_HANDLE,         EINVAL    },
        {  ERROR_WAIT_NO_CHILDREN,       ECHILD    },
        {  ERROR_CHILD_NOT_COMPLETE,     ECHILD    },
        {  ERROR_DIRECT_ACCESS_HANDLE,   EBADF     },
        {  ERROR_NEGATIVE_SEEK,          EINVAL    },
        {  ERROR_SEEK_ON_DEVICE,         EACCES    },
        {  ERROR_DIR_NOT_EMPTY,          ENOTEMPTY },
        {  ERROR_NOT_LOCKED,             EACCES    },
        {  ERROR_BAD_PATHNAME,           ENOENT    },
        {  ERROR_MAX_THRDS_REACHED,      EAGAIN    },
        {  ERROR_LOCK_FAILED,            EACCES    },
        {  ERROR_ALREADY_EXISTS,         EEXIST    },
        {  ERROR_FILENAME_EXCED_RANGE,   ENOENT    },
        {  ERROR_NESTING_NOT_ALLOWED,    EAGAIN    },
        {  ERROR_NOT_ENOUGH_QUOTA,       ENOMEM    }
};




স্থির
unsigned
_MapDosError (
    IN ULONG WinError
   )
{
	পূর্ণসংখ্যা i;

	//
	// Lookup
	//

	জন্য (i = 0; i < (আকার(ErrorTable)/আকার(ErrorTable[0])); ++i)
	{
		যদি (WinError == ErrorTable[i].WinError)
		{
			ফেরত ErrorTable[i].errnocode;
		}
	}

	//
	// not in table. Check ranges
	//

	যদি ((WinError >= ERROR_WRITE_PROTECT) &&
		(WinError <= ERROR_SHARING_বাফার_EXCEEDED))
	{
		ফেরত EACCES;
	}
	অন্যথা যদি ((WinError >= ERROR_INVALID_STARTING_CODESEG) &&
			 (WinError <= ERROR_INFLOOP_IN_RELOC_CHAIN))
	{
		ফেরত ENOEXEC;
	}
	অন্যথা
	{
		ফেরত EINVAL;
	}
}







//
// Function to map NT status to dos error.
//

স্থির
__inline
unsigned
_MapNtStatus(
    IN NTSTATUS Status
   )
{
	ফেরত _MapDosError(RtlNtStatusToDosError(Status));
}





//
// Helper functions to make things easier
//

স্থির
NTSTATUS
_OpenNtName(
    IN PCSTR Name,
    IN BOOLEAN Readonly,
    OUT PHANDLE Handle,
    OUT PBOOLEAN OpenedReadonly OPTIONAL
   )
{
	UNICODE_STRING UnicodeString;
	ANSI_STRING    AnsiString;
	WCHAR বাফার_2[512];
	NTSTATUS Status;
	OBJECT_ATTRIBUTES ObjectAttributes;
	IO_STATUS_BLOCK IoStatusBlock;

	//
	// Make Unicode name from input string
	//

	UnicodeString.বাফার_2 = &বাফার_2[0];
	UnicodeString.দৈর্ঘ্য_2 = 0;
	UnicodeString.Maximum_দৈর্ঘ্য = আকার(বাফার_2); // in bytes!!!

	RtlInitAnsiString(&AnsiString, Name);

	Status = RtlAnsiStringToUnicodeString(&UnicodeString, &AnsiString, FALSE);

	যদি(!NT_SUCCESS(Status))
	{
		ফেরত Status; // Unmappable character?
	}

	//
	// Initialize object
	//

	InitializeObjectAttributes(&ObjectAttributes,
							   &UnicodeString,
							   OBJ_CASE_INSENSITIVE,
							   ফাঁকা,
							   ফাঁকা );

	//
	// Try to open it in initial mode
	//

	যদি(ARGUMENT_PRESENT(OpenedReadonly))
	{
		*OpenedReadonly = Readonly;
	}


	Status = NtOpenFile(Handle,
						SYNCHRONIZE | FILE_READ_তথ্য | (Readonly ? 0 : FILE_WRITE_তথ্য),
						&ObjectAttributes,
						&IoStatusBlock,
						FILE_SHARE_WRITE | FILE_SHARE_READ,
						FILE_SYNCHRONOUS_IO_NONALERT);

	যদি(!NT_SUCCESS(Status))
	{
		//
		// Maybe was just mounted? wait 0.5 sec and retry.
		//

		LARGE_INTEGER Interval;
		Interval.QuadPart = -5000000; // 0.5 sec. from now

		NtDelayExecution(FALSE, &Interval);

		Status = NtOpenFile(Handle,
							SYNCHRONIZE | FILE_READ_তথ্য | (Readonly ? 0 : FILE_WRITE_তথ্য),
							&ObjectAttributes,
							&IoStatusBlock,
							FILE_SHARE_WRITE | FILE_SHARE_READ,
							FILE_SYNCHRONOUS_IO_NONALERT);

		//
		// Try to satisfy mode
		//

		যদি((STATUS_ACCESS_DENIED == Status) && !Readonly)
		{
			যদি(ARGUMENT_PRESENT(OpenedReadonly))
			{
				*OpenedReadonly = TRUE;
			}

			Status = NtOpenFile(Handle,
							SYNCHRONIZE | FILE_READ_তথ্য,
							&ObjectAttributes,
							&IoStatusBlock,
							FILE_SHARE_WRITE | FILE_SHARE_READ,
							FILE_SYNCHRONOUS_IO_NONALERT);
		}
	}



	//
	// done
	//

	ফেরত Status;
}


স্থির
NTSTATUS
_OpenDriveLetter(
    IN CHAR Letter,
    IN BOOLEAN ReadOnly,
    OUT PHANDLE Handle,
    OUT PBOOLEAN OpenedReadonly OPTIONAL
   )
{
	CHAR বাফার_2[100];

	sprintf(বাফার_2, "\\DosDevices\\%c:", Letter);

	ফেরত _OpenNtName(বাফার_2, ReadOnly, Handle, OpenedReadonly);
}


//
// Flush device
//

স্থির
__inline
NTSTATUS
_FlushDrive(
		IN HANDLE Handle
		)
{
	IO_STATUS_BLOCK IoStatusBlock;
	ফেরত NtFlushBuffersFile(Handle, &IoStatusBlock);
}


//
// lock drive
//

স্থির
__inline
NTSTATUS
_LockDrive(
		IN HANDLE Handle
		)
{
	IO_STATUS_BLOCK IoStatusBlock;
	ফেরত NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_LOCK_VOLUME, 0, 0, 0, 0);
}


//
// unlock drive
//

স্থির
__inline
NTSTATUS
_UnlockDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	ফেরত NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0);
}

স্থির
__inline
NTSTATUS
_DismountDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	ফেরত NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0);
}


//
// is mounted
//

স্থির
__inline
BOOLEAN
_IsMounted(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	NTSTATUS Status;
	Status = NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_IS_VOLUME_MOUNTED, 0, 0, 0, 0);
	ফেরত (BOOLEAN)(STATUS_SUCCESS == Status);
}


স্থির
__inline
NTSTATUS
_CloseDisk(
		IN HANDLE Handle
		)
{
	ফেরত NtClose(Handle);
}




//
// Make NT name from any recognized name
//

স্থির
PCSTR
_NormalizeDeviceName(
    IN PCSTR Device,
    IN PSTR Normalized_Device_Name_বাফার
   )
{
	পূর্ণসংখ্যা Partition_সংখ্যা = -1;
	UCHAR Disk_সংখ্যা;
	PSTR p;


	//
	// Do not try to parse NT name
	//

	যদি('\\' == *Device)
		ফেরত Device;



	//
	// Strip leading '/dev/' if any
	//

	যদি(('/' == *(Device)) &&
		('d' == *(Device + 1)) &&
		('e' == *(Device + 2)) &&
		('v' == *(Device + 3)) &&
		('/' == *(Device + 4)))
	{
		Device += 5;
	}

	যদি('\0' == *Device)
	{
		ফেরত ফাঁকা;
	}


	//
	// forms: hda[n], fd[n]
	//

	যদি('d' != *(Device + 1))
	{
		ফেরত ফাঁকা;
	}

	যদি('h' == *Device)
	{
		যদি((*(Device + 2) < 'a') || (*(Device + 2) > ('a' + 9)) ||
		   ((*(Device + 3) != '\0') &&
			((*(Device + 4) != '\0') ||
			 ((*(Device + 3) < '0') || (*(Device + 3) > '9'))
			)
		   )
		  )
		{
			ফেরত ফাঁকা;
		}

		Disk_সংখ্যা = (UCHAR)(*(Device + 2) - 'a');

		যদি(*(Device + 3) != '\0')
		{
			Partition_সংখ্যা = (*(Device + 3) - '0');
		}

	}
	অন্যথা যদি('f' == *Device)
	{
		//
		// 3-d letter should be a digit.
		//

		যদি((*(Device + 3) != '\0') ||
		   (*(Device + 2) < '0') || (*(Device + 2) > '9'))
		{
			ফেরত ফাঁকা;
		}

		Disk_সংখ্যা = (UCHAR)(*(Device + 2) - '0');

	}
	অন্যথা
	{
		//
		// invalid prefix
		//

		ফেরত ফাঁকা;
	}



	//
	// Prefix
	//

	strcpy(Normalized_Device_Name_বাফার, "\\Device\\");

	//
	// Media name
	//

	নির্বাচন(*Device)
	{

	ক্ষেত্র 'f':
		strcat(Normalized_Device_Name_বাফার, "Floppy0");
		বিরতি;

	ক্ষেত্র 'h':
		strcat(Normalized_Device_Name_বাফার, "Harddisk0");
		বিরতি;
	}


	p = Normalized_Device_Name_বাফার + strlen(Normalized_Device_Name_বাফার) - 1;
	*p = (CHAR)(*p + Disk_সংখ্যা);


	//
	// Partition nr.
	//

	যদি(Partition_সংখ্যা >= 0)
	{
		strcat(Normalized_Device_Name_বাফার, "\\Partition0");

		p = Normalized_Device_Name_বাফার + strlen(Normalized_Device_Name_বাফার) - 1;
		*p = (CHAR)(*p + Partition_সংখ্যা);
	}


	ফেরত Normalized_Device_Name_বাফার;
}




স্থির
VOID
Get_Device_আকার(
    IN HANDLE h,
    OUT unsigned __int64 *Fs_আকার
   )
{
	PARTITION_INFORMATION pi;
	DISK_GEOMETRY gi;
	NTSTATUS Status;
	IO_STATUS_BLOCK IoStatusBlock;

	//
	// Zero it
	//

	*Fs_আকার = 0;

	//
	// Call driver
	//

	RtlZeroMemory(&pi, আকার(PARTITION_INFORMATION));

	Status = NtDeviceIoControlFile(
		h, ফাঁকা, ফাঁকা, ফাঁকা, &IoStatusBlock, IOCTL_DISK_GET_PARTITION_INFO,
		&pi, আকার(PARTITION_INFORMATION),
		&pi, আকার(PARTITION_INFORMATION));


	যদি(NT_SUCCESS(Status))
	{
		*Fs_আকার = pi.Partition_দৈর্ঘ্য.QuadPart;
	}
	অন্যথা যদি(STATUS_INVALID_DEVICE_REQUEST == Status)
	{
		//
		// No partitions: get device info.
		//

		RtlZeroMemory(&gi, আকার(DISK_GEOMETRY));

		Status = NtDeviceIoControlFile(
				h, ফাঁকা, ফাঁকা, ফাঁকা, &IoStatusBlock, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, আকার(DISK_GEOMETRY),
				&gi, আকার(DISK_GEOMETRY));


		যদি(NT_SUCCESS(Status))
		{
			*Fs_আকার =
				gi.BytesPerSector *
				gi.SectorsPerTrack *
				gi.TracksPerCylinder *
				gi.Cylinders.QuadPart;
		}

	}
}



//
// Open device by name.
//

স্থির
BOOLEAN
_Ext2OpenDevice(
    IN PCSTR Name,
    IN BOOLEAN ReadOnly,
    OUT PHANDLE Handle,
    OUT PBOOLEAN OpenedReadonly OPTIONAL,
    OUT unsigned *Errno OPTIONAL
   )
{
	CHAR NormalizedDeviceName[512];
	NTSTATUS Status;

	যদি(ফাঁকা == Name)
	{
		//
		// Set not found
		//

		যদি(ARGUMENT_PRESENT(Errno))
			*Errno = ENOENT;

		ফেরত FALSE;
	}


	যদি((((*Name) | 0x20) >= 'a') && (((*Name) | 0x20) <= 'z') &&
		(':' == *(Name + 1)) && ('\0' == *(Name + 2)))
	{
		Status = _OpenDriveLetter(*Name, ReadOnly, Handle, OpenedReadonly);
	}
	অন্যথা
	{
		//
		// Make name
		//

		Name = _NormalizeDeviceName(Name, NormalizedDeviceName);

		যদি(ফাঁকা == Name)
		{
			//
			// Set not found
			//

			যদি(ARGUMENT_PRESENT(Errno))
				*Errno = ENOENT;

			ফেরত FALSE;
		}

		//
		// Try to open it
		//

		Status = _OpenNtName(Name, ReadOnly, Handle, OpenedReadonly);
	}


	যদি(!NT_SUCCESS(Status))
	{
		যদি(ARGUMENT_PRESENT(Errno))
			*Errno = _MapNtStatus(Status);

		ফেরত FALSE;
	}

	ফেরত TRUE;
}


//
// Raw block io. Sets dos errno
//

স্থির
BOOLEAN
_BlockIo(
    IN HANDLE Handle,
    IN LARGE_INTEGER Offset,
    IN ULONG Bytes,
    IN OUT PCHAR বাফার_2,
    IN BOOLEAN Read,
    OUT unsigned* Errno
   )
{
	IO_STATUS_BLOCK IoStatusBlock;
	NTSTATUS Status;

	//
	// Should be aligned
	//

	ASSERT(0 == (Bytes % 512));
	ASSERT(0 == (Offset.নিচের_Part % 512));


	//
	// perform io
	//

	যদি(Read)
	{
		Status = NtReadFile(Handle, ফাঁকা, ফাঁকা, ফাঁকা,
			&IoStatusBlock, বাফার_2, Bytes, &Offset, ফাঁকা);
	}
	অন্যথা
	{
		Status = NtWriteFile(Handle, ফাঁকা, ফাঁকা, ফাঁকা,
			&IoStatusBlock, বাফার_2, Bytes, &Offset, ফাঁকা);
	}


	//
	// translate error
	//

	যদি(NT_SUCCESS(Status))
	{
		*Errno = 0;
		ফেরত TRUE;
	}

	*Errno = _MapNtStatus(Status);

	ফেরত FALSE;
}



__inline
BOOLEAN
_RawWrite(
    IN HANDLE Handle,
    IN LARGE_INTEGER Offset,
    IN ULONG Bytes,
    OUT ধ্রুবক CHAR* বাফার_2,
    OUT unsigned* Errno
   )
{
	ফেরত _BlockIo(Handle, Offset, Bytes, (PCHAR)বাফার_2, FALSE, Errno);
}

__inline
BOOLEAN
_RawRead(
    IN HANDLE Handle,
    IN LARGE_INTEGER Offset,
    IN ULONG Bytes,
    IN PCHAR বাফার_2,
    OUT unsigned* Errno
   )
{
	ফেরত _BlockIo(Handle, Offset, Bytes, বাফার_2, TRUE, Errno);
}



__inline
BOOLEAN
_SetPartType(
    IN HANDLE Handle,
    IN UCHAR Type
   )
{
	IO_STATUS_BLOCK IoStatusBlock;
	ফেরত STATUS_SUCCESS == NtDeviceIoControlFile(
												   Handle, ফাঁকা, ফাঁকা, ফাঁকা, &IoStatusBlock, IOCTL_DISK_SET_PARTITION_INFO,
												   &Type, আকার(Type),
												   ফাঁকা, 0);
}



//--------------------- interface part

//
// Interface functions.
// Is_mounted is set to 1 if the device is mounted, 0 otherwise
//

errcode_t
ext2fs_check_if_mounted(ধ্রুবক অক্ষর *file, পূর্ণসংখ্যা *mount_flags)
{
	HANDLE h;
	BOOLEAN Readonly;

	*mount_flags = 0;

	যদি(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, ফাঁকা))
	{
		ফেরত 0;
	}


	__try{
		*mount_flags &= _IsMounted(h) ? EXT2_MF_MOUNTED : 0;
	}
	__finally{
		_CloseDisk(h);
	}

	ফেরত 0;
}



//
// Returns the number of blocks in a partition
//

স্থির __int64 Fs_আকার = 0;
স্থির অক্ষর knowndevice[1024] = "";


errcode_t
ext2fs_get_device_আকার(ধ্রুবক অক্ষর *file, পূর্ণসংখ্যা blocksize,
				 blk_t *retblocks)
{
	HANDLE h;
	BOOLEAN Readonly;

	যদি((0 == Fs_আকার) || (0 != strcmp(knowndevice, file)))
	{

		যদি(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, ফাঁকা))
		{
			ফেরত 0;
		}


		__try{

			//
			// Get size
			//

			Get_Device_আকার(h, &Fs_আকার);
			strcpy(knowndevice, file);
		}
		__finally{
			_CloseDisk(h);
		}

	}

	*retblocks = (blk_t)(unsigned __int64)(Fs_আকার / blocksize);
	UNREFERENCED_PARAMETER(file);
	ফেরত 0;
}






//
// Table elements
//


স্থির
errcode_t
nt_open(ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা flags, io_channel *channel)
{
	io_channel      io = ফাঁকা;
	PNT_PRIVATE_তথ্য Nt_তথ্য = ফাঁকা;
	errcode_t Errno = 0;

	//
	// Check name
	//

	যদি (ফাঁকা == name)
	{
		ফেরত EXT2_ET_BAD_DEVICE_NAME;
	}

	__try{

		//
		// Allocate channel handle
		//

		io = (io_channel) malloc(আকার(কাঠামো struct_io_channel));

		যদি (ফাঁকা == io)
		{
			Errno = ENOMEM;
			__leave;
		}

		RtlZeroMemory(io, আকার(কাঠামো struct_io_channel));
		io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

		Nt_তথ্য = (PNT_PRIVATE_তথ্য)malloc(আকার(NT_PRIVATE_তথ্য_2));

		যদি (ফাঁকা == Nt_তথ্য)
		{
			Errno = ENOMEM;
			__leave;
		}


		io->manager = nt_io_manager();
		io->name = malloc(strlen(name) + 1);
		যদি (ফাঁকা == io->name)
		{
			Errno = ENOMEM;
			__leave;
		}

		strcpy(io->name, name);
		io->private_তথ্য = Nt_তথ্য;
		io->block_আকার = 1024;
		io->read_error = 0;
		io->write_error = 0;
		io->refcount = 1;

		//
		// Initialize data
		//

		RtlZeroMemory(Nt_তথ্য, আকার(NT_PRIVATE_তথ্য_2));

		Nt_তথ্য->magic = EXT2_ET_MAGIC_NT_IO_CHANNEL;
		Nt_তথ্য->বাফার_Block_সংখ্যা = 0xffffffff;
		Nt_তথ্য->বাফার_আকার = 1024;
		Nt_তথ্য->বাফার_2 = malloc(Nt_তথ্য->বাফার_আকার);

		যদি (ফাঁকা == Nt_তথ্য->বাফার_2)
		{
			Errno = ENOMEM;
			__leave;
		}

		//
		// Open it
		//

		যদি(!_Ext2OpenDevice(name, (BOOLEAN)!BooleanFlagOn(flags, EXT2_FLAG_RW), &Nt_তথ্য->Handle, &Nt_তথ্য->OpenedReadonly, &Errno))
		{
			__leave;
		}


		//
		// get size
		//

		Get_Device_আকার(Nt_তথ্য->Handle, &Fs_আকার);
		strcpy(knowndevice, name);


		//
		// Lock/dismount
		//

		যদি(!NT_SUCCESS(_LockDrive(Nt_তথ্য->Handle)) /*|| !NT_SUCCESS(_DismountDrive(NtData->Handle))*/)
		{
			Nt_তথ্য->OpenedReadonly = TRUE;
		}

		//
		// Done
		//

		*channel = io;


	}
	__finally{

		যদি(0 != Errno)
		{
			//
			// Cleanup
			//

			যদি (ফাঁকা != io)
			{
				মুক্ত(io->name);
				মুক্ত(io);
			}

			যদি (ফাঁকা != Nt_তথ্য)
			{
				যদি(ফাঁকা != Nt_তথ্য->Handle)
				{
					_UnlockDrive(Nt_তথ্য->Handle);
					_CloseDisk(Nt_তথ্য->Handle);
				}

				মুক্ত(Nt_তথ্য->বাফার_2);
				মুক্ত(Nt_তথ্য);
			}
		}
	}

	ফেরত Errno;
}


//
// Close api
//

স্থির
errcode_t
nt_close(io_channel channel)
{
	PNT_PRIVATE_তথ্য Nt_তথ্য = ফাঁকা;

	যদি(ফাঁকা == channel)
	{
		ফেরত 0;
	}

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_তথ্য = (PNT_PRIVATE_তথ্য) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(Nt_তথ্য, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	যদি (--channel->refcount > 0)
	{
		ফেরত 0;
	}

	মুক্ত(channel->name);
	মুক্ত(channel);

	যদি (ফাঁকা != Nt_তথ্য)
	{
		যদি(ফাঁকা != Nt_তথ্য->Handle)
		{
			_DismountDrive(Nt_তথ্য->Handle);
			_UnlockDrive(Nt_তথ্য->Handle);
			_CloseDisk(Nt_তথ্য->Handle);
		}

		মুক্ত(Nt_তথ্য->বাফার_2);
		মুক্ত(Nt_তথ্য);
	}

	ফেরত 0;
}



//
// set block size
//

স্থির
errcode_t
nt_set_blksize(io_channel channel, পূর্ণসংখ্যা blksize)
{
	PNT_PRIVATE_তথ্য Nt_তথ্য = ফাঁকা;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_তথ্য = (PNT_PRIVATE_তথ্য) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(Nt_তথ্য, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	যদি (channel->block_আকার != blksize)
	{
		channel->block_আকার = blksize;

		মুক্ত(Nt_তথ্য->বাফার_2);
		Nt_তথ্য->বাফার_Block_সংখ্যা = 0xffffffff;
		Nt_তথ্য->বাফার_আকার = channel->block_আকার;
		ASSERT(0 == (Nt_তথ্য->বাফার_আকার % 512));

		Nt_তথ্য->বাফার_2 = malloc(Nt_তথ্য->বাফার_আকার);

		যদি (ফাঁকা == Nt_তথ্য->বাফার_2)
		{
			ফেরত ENOMEM;
		}

	}

	ফেরত 0;
}


//
// read block
//

স্থির
errcode_t
nt_read_blk(io_channel channel, unsigned long block,
			       পূর্ণসংখ্যা গুনতি, শূন্য *buf)
{
	PVOID বাফার_To_Read;
	ULONG আকার_To_Read;
	ULONG আকার_3;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_তথ্য Nt_তথ্য = ফাঁকা;
	unsigned Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_তথ্য = (PNT_PRIVATE_তথ্য) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(Nt_তথ্য, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	//
	// If it's in the cache, use it!
	//

	যদি ((1 == গুনতি) &&
		(block == Nt_তথ্য->বাফার_Block_সংখ্যা) &&
		(Nt_তথ্য->বাফার_Block_সংখ্যা != 0xffffffff))
	{
		memcpy(buf, Nt_তথ্য->বাফার_2, channel->block_আকার);
		ফেরত 0;
	}

	আকার_3 = (গুনতি < 0) ? (ULONG)(-গুনতি) : (ULONG)(গুনতি * channel->block_আকার);

	Offset.QuadPart = block * channel->block_আকার;

	//
	// If not fit to the block
	//

	যদি(আকার_3 <= Nt_তথ্য->বাফার_আকার)
	{
		//
		// Update the cache
		//

		Nt_তথ্য->বাফার_Block_সংখ্যা = block;
		বাফার_To_Read = Nt_তথ্য->বাফার_2;
		আকার_To_Read = Nt_তথ্য->বাফার_আকার;
	}
	অন্যথা
	{
		আকার_To_Read = আকার_3;
		বাফার_To_Read = buf;
		ASSERT(0 == (আকার_To_Read % channel->block_আকার));
	}

	যদি(!_RawRead(Nt_তথ্য->Handle, Offset, আকার_To_Read, বাফার_To_Read, &Errno))
	{

		যদি (channel->read_error)
		{
			ফেরত (channel->read_error)(channel, block, গুনতি, buf,
					       আকার_3, 0, Errno);
		}
		অন্যথা
		{
			ফেরত Errno;
		}
	}


	যদি(বাফার_To_Read != buf)
	{
		ASSERT(আকার_3 <= আকার_To_Read);
		memcpy(buf, বাফার_To_Read, আকার_3);
	}

	ফেরত 0;
}


//
// write block
//

স্থির
errcode_t
nt_write_blk(io_channel channel, unsigned long block,
				পূর্ণসংখ্যা গুনতি, ধ্রুবক শূন্য *buf)
{
	ULONG আকার_To_Write;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_তথ্য Nt_তথ্য = ফাঁকা;
	unsigned Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_তথ্য = (PNT_PRIVATE_তথ্য) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(Nt_তথ্য, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	যদি(Nt_তথ্য->OpenedReadonly)
	{
		ফেরত EACCES;
	}

	যদি (গুনতি == 1)
	{
		আকার_To_Write = channel->block_আকার;
	}
	অন্যথা
	{
		Nt_তথ্য->বাফার_Block_সংখ্যা = 0xffffffff;

		যদি (গুনতি < 0)
		{
			আকার_To_Write = (ULONG)(-গুনতি);
		}
		অন্যথা
		{
			আকার_To_Write = (ULONG)(গুনতি * channel->block_আকার);
		}
	}


	ASSERT(0 == (আকার_To_Write % 512));
	Offset.QuadPart = block * channel->block_আকার;

	যদি(!_RawWrite(Nt_তথ্য->Handle, Offset, আকার_To_Write, buf, &Errno))
	{
		যদি (channel->write_error)
		{
			ফেরত (channel->write_error)(channel, block, গুনতি, buf,
						আকার_To_Write, 0, Errno);
		}
		অন্যথা
		{
			ফেরত Errno;
		}
	}


	//
	// Stash a copy.
	//

	যদি(আকার_To_Write >= Nt_তথ্য->বাফার_আকার)
	{
		Nt_তথ্য->বাফার_Block_সংখ্যা = block;
		memcpy(Nt_তথ্য->বাফার_2, buf, Nt_তথ্য->বাফার_আকার);
	}

	Nt_তথ্য->Written = TRUE;

	ফেরত 0;

}



//
// Flush data buffers to disk.  Since we are currently using a
// write-through cache, this is a no-op.
//

স্থির
errcode_t
nt_flush(io_channel channel)
{
	PNT_PRIVATE_তথ্য Nt_তথ্য = ফাঁকা;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_তথ্য = (PNT_PRIVATE_তথ্য) channel->private_তথ্য;
	EXT2_CHECK_MAGIC(Nt_তথ্য, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	যদি(Nt_তথ্য->OpenedReadonly)
	{
		ফেরত 0; // EACCESS;
	}


	//
	// Flush file buffers.
	//

	_FlushDrive(Nt_তথ্য->Handle);


	//
	// Test and correct partition type.
	//

	যদি(Nt_তথ্য->Written)
	{
		_SetPartType(Nt_তথ্য->Handle, 0x83);
	}

	ফেরত 0;
}


