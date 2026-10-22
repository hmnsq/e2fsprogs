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
#অন্তর্ভাৱ "config.h"
#endif


//
// I need some warnings to disable...
//


#pragma warning(disable:4514) // unreferenced inline function has been removed
#pragma warning(ঠেলো,4)

#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union)
#pragma warning(disable:4214) // nonstandard extension used : bit field types other than int
#pragma warning(disable:4115) // named type definition in parentheses

#অন্তর্ভাৱ <ntddk.h>
#অন্তর্ভাৱ <ntdddisk.h>
#অন্তর্ভাৱ <ntstatus.h>

#pragma warning(বেৰ_কৰো)


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
    OUT PVOID Buffer,
    IN ULONG Length,
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
    IN PVOID Buffer,
    IN ULONG Length,
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
    IN PVOID প্ৰবেশ_Buffer OPTIONAL,
    IN ULONG প্ৰবেশ_Buffer_Length,
    OUT PVOID OutputBuffer OPTIONAL,
    IN ULONG OutputBufferLength
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
    IN PVOID প্ৰবেশ_Buffer OPTIONAL,
    IN ULONG প্ৰবেশ_Buffer_Length,
    OUT PVOID OutputBuffer OPTIONAL,
    IN ULONG OutputBufferLength
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

#অন্তর্ভাৱ <winerror.h>

//
// standard stuff
//

#অন্তর্ভাৱ <assert.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <malloc.h>

#অন্তর্ভাৱ <linux/types.h>
#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ <errno.h>

#অন্তর্ভাৱ "et/com_err.h"
#অন্তর্ভাৱ "ext2fs/ext2fs.h"
#অন্তর্ভাৱ "ext2fs/ext2_err.h"




//
// For checking structure magic numbers...
//


#define EXT2_CHECK_MAGIC(সংরচনা, code) \
	  যদি ((সংরচনা)->magic != (code)) ঘূৰাই (code)

#define EXT2_ET_MAGIC_NT_IO_CHANNEL  0x10ed


//
// Private data block
//

প্রকারনির্দেশ সংরচনা _NT_PRIVATE_DATA {
	পূর্ণাঙ্ক	   magic;
	HANDLE Handle;
	পূর্ণাঙ্ক	   Flags;
	PCHAR  Buffer;
	__u32  Buffer_Block_সংখ্যা;
	ULONG  BufferSize;
	BOOLEAN OpenedReadonly;
	BOOLEAN Written;
}NT_PRIVATE_DATA, *PNT_PRIVATE_DATA;



//
// Standard interface prototypes
//

স্থির errcode_t nt_open(ধ্রুৱ ৱর্ণ *name, পূর্ণাঙ্ক flags, io_channel *channel);
স্থির errcode_t nt_close(io_channel channel);
স্থির errcode_t nt_set_blksize(io_channel channel, পূর্ণাঙ্ক blksize);
স্থির errcode_t nt_read_blk(io_channel channel, unsigned long block,
			       পূর্ণাঙ্ক গুনতি, শূন্য *data);
স্থির errcode_t nt_write_blk(io_channel channel, unsigned long block,
				পূর্ণাঙ্ক গুনতি, ধ্রুৱ শূন্য *data);
স্থির errcode_t nt_flush(io_channel channel);

স্থির সংরচনা struct_io_manager struct_nt_manager = {
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
	ঘূৰাই &struct_nt_manager;
}





//
// This is a code to convert Win32 errors to unix errno
//

প্রকারনির্দেশ সংরচনা {
	ULONG WinError;
	পূর্ণাঙ্ক errnocode;
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
        {  ERROR_INVALID_DATA,           EINVAL    },
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
	পূর্ণাঙ্ক i;

	//
	// Lookup
	//

	বাবে (i = 0; i < (পরিমাণ(ErrorTable)/পরিমাণ(ErrorTable[0])); ++i)
	{
		যদি (WinError == ErrorTable[i].WinError)
		{
			ঘূৰাই ErrorTable[i].errnocode;
		}
	}

	//
	// not in table. Check ranges
	//

	যদি ((WinError >= ERROR_WRITE_PROTECT) &&
		(WinError <= ERROR_SHARING_BUFFER_EXCEEDED))
	{
		ঘূৰাই EACCES;
	}
	অন্যথা যদি ((WinError >= ERROR_INVALID_STARTING_CODESEG) &&
			 (WinError <= ERROR_INFLOOP_IN_RELOC_CHAIN))
	{
		ঘূৰাই ENOEXEC;
	}
	অন্যথা
	{
		ঘূৰাই EINVAL;
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
	ঘূৰাই _MapDosError(RtlNtStatusToDosError(Status));
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
	WCHAR Buffer[512];
	NTSTATUS Status;
	OBJECT_ATTRIBUTES ObjectAttributes;
	IO_STATUS_BLOCK IoStatusBlock;

	//
	// Make Unicode name from input string
	//

	UnicodeString.Buffer = &Buffer[0];
	UnicodeString.Length = 0;
	UnicodeString.MaximumLength = পরিমাণ(Buffer); // in bytes!!!

	RtlInitAnsiString(&AnsiString, Name);

	Status = RtlAnsiStringToUnicodeString(&UnicodeString, &AnsiString, FALSE);

	যদি(!NT_SUCCESS(Status))
	{
		ঘূৰাই Status; // Unmappable character?
	}

	//
	// Initialize object
	//

	InitializeObjectAttributes(&ObjectAttributes,
							   &UnicodeString,
							   OBJ_CASE_INSENSITIVE,
							   রিক্ত,
							   রিক্ত );

	//
	// Try to open it in initial mode
	//

	যদি(ARGUMENT_PRESENT(OpenedReadonly))
	{
		*OpenedReadonly = Readonly;
	}


	Status = NtOpenFile(Handle,
						SYNCHRONIZE | FILE_READ_DATA | (Readonly ? 0 : FILE_WRITE_DATA),
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
							SYNCHRONIZE | FILE_READ_DATA | (Readonly ? 0 : FILE_WRITE_DATA),
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
							SYNCHRONIZE | FILE_READ_DATA,
							&ObjectAttributes,
							&IoStatusBlock,
							FILE_SHARE_WRITE | FILE_SHARE_READ,
							FILE_SYNCHRONOUS_IO_NONALERT);
		}
	}



	//
	// done
	//

	ঘূৰাই Status;
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
	CHAR Buffer[100];

	sprintf(Buffer, "\\DosDevices\\%c:", Letter);

	ঘূৰাই _OpenNtName(Buffer, ReadOnly, Handle, OpenedReadonly);
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
	ঘূৰাই NtFlushBuffersFile(Handle, &IoStatusBlock);
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
	ঘূৰাই NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_LOCK_VOLUME, 0, 0, 0, 0);
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
	ঘূৰাই NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0);
}

স্থির
__inline
NTSTATUS
_DismountDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	ঘূৰাই NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0);
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
	ঘূৰাই (BOOLEAN)(STATUS_SUCCESS == Status);
}


স্থির
__inline
NTSTATUS
_CloseDisk(
		IN HANDLE Handle
		)
{
	ঘূৰাই NtClose(Handle);
}




//
// Make NT name from any recognized name
//

স্থির
PCSTR
_NormalizeDeviceName(
    IN PCSTR Device,
    IN PSTR NormalizedDeviceNameBuffer
   )
{
	পূর্ণাঙ্ক Partition_সংখ্যা = -1;
	UCHAR Disk_সংখ্যা;
	PSTR p;


	//
	// Do not try to parse NT name
	//

	যদি('\\' == *Device)
		ঘূৰাই Device;



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
		ঘূৰাই রিক্ত;
	}


	//
	// forms: hda[n], fd[n]
	//

	যদি('d' != *(Device + 1))
	{
		ঘূৰাই রিক্ত;
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
			ঘূৰাই রিক্ত;
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
			ঘূৰাই রিক্ত;
		}

		Disk_সংখ্যা = (UCHAR)(*(Device + 2) - '0');

	}
	অন্যথা
	{
		//
		// invalid prefix
		//

		ঘূৰাই রিক্ত;
	}



	//
	// Prefix
	//

	strcpy(NormalizedDeviceNameBuffer, "\\Device\\");

	//
	// Media name
	//

	ৱিকল্প(*Device)
	{

	পক্ষ 'f':
		strcat(NormalizedDeviceNameBuffer, "Floppy0");
		ভঙ্গ;

	পক্ষ 'h':
		strcat(NormalizedDeviceNameBuffer, "Harddisk0");
		ভঙ্গ;
	}


	p = NormalizedDeviceNameBuffer + strlen(NormalizedDeviceNameBuffer) - 1;
	*p = (CHAR)(*p + Disk_সংখ্যা);


	//
	// Partition nr.
	//

	যদি(Partition_সংখ্যা >= 0)
	{
		strcat(NormalizedDeviceNameBuffer, "\\Partition0");

		p = NormalizedDeviceNameBuffer + strlen(NormalizedDeviceNameBuffer) - 1;
		*p = (CHAR)(*p + Partition_সংখ্যা);
	}


	ঘূৰাই NormalizedDeviceNameBuffer;
}




স্থির
VOID
_GetDeviceSize(
    IN HANDLE h,
    OUT unsigned __int64 *FsSize
   )
{
	PARTITION_INFORMATION pi;
	DISK_GEOMETRY gi;
	NTSTATUS Status;
	IO_STATUS_BLOCK IoStatusBlock;

	//
	// Zero it
	//

	*FsSize = 0;

	//
	// Call driver
	//

	RtlZeroMemory(&pi, পরিমাণ(PARTITION_INFORMATION));

	Status = NtDeviceIoControlFile(
		h, রিক্ত, রিক্ত, রিক্ত, &IoStatusBlock, IOCTL_DISK_GET_PARTITION_INFO,
		&pi, পরিমাণ(PARTITION_INFORMATION),
		&pi, পরিমাণ(PARTITION_INFORMATION));


	যদি(NT_SUCCESS(Status))
	{
		*FsSize = pi.PartitionLength.QuadPart;
	}
	অন্যথা যদি(STATUS_INVALID_DEVICE_REQUEST == Status)
	{
		//
		// No partitions: get device info.
		//

		RtlZeroMemory(&gi, পরিমাণ(DISK_GEOMETRY));

		Status = NtDeviceIoControlFile(
				h, রিক্ত, রিক্ত, রিক্ত, &IoStatusBlock, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, পরিমাণ(DISK_GEOMETRY),
				&gi, পরিমাণ(DISK_GEOMETRY));


		যদি(NT_SUCCESS(Status))
		{
			*FsSize =
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

	যদি(রিক্ত == Name)
	{
		//
		// Set not found
		//

		যদি(ARGUMENT_PRESENT(Errno))
			*Errno = ENOENT;

		ঘূৰাই FALSE;
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

		যদি(রিক্ত == Name)
		{
			//
			// Set not found
			//

			যদি(ARGUMENT_PRESENT(Errno))
				*Errno = ENOENT;

			ঘূৰাই FALSE;
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

		ঘূৰাই FALSE;
	}

	ঘূৰাই TRUE;
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
    IN OUT PCHAR Buffer,
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
	ASSERT(0 == (Offset.নিচেৰ_Part % 512));


	//
	// perform io
	//

	যদি(Read)
	{
		Status = NtReadFile(Handle, রিক্ত, রিক্ত, রিক্ত,
			&IoStatusBlock, Buffer, Bytes, &Offset, রিক্ত);
	}
	অন্যথা
	{
		Status = NtWriteFile(Handle, রিক্ত, রিক্ত, রিক্ত,
			&IoStatusBlock, Buffer, Bytes, &Offset, রিক্ত);
	}


	//
	// translate error
	//

	যদি(NT_SUCCESS(Status))
	{
		*Errno = 0;
		ঘূৰাই TRUE;
	}

	*Errno = _MapNtStatus(Status);

	ঘূৰাই FALSE;
}



__inline
BOOLEAN
_RawWrite(
    IN HANDLE Handle,
    IN LARGE_INTEGER Offset,
    IN ULONG Bytes,
    OUT ধ্রুৱ CHAR* Buffer,
    OUT unsigned* Errno
   )
{
	ঘূৰাই _BlockIo(Handle, Offset, Bytes, (PCHAR)Buffer, FALSE, Errno);
}

__inline
BOOLEAN
_RawRead(
    IN HANDLE Handle,
    IN LARGE_INTEGER Offset,
    IN ULONG Bytes,
    IN PCHAR Buffer,
    OUT unsigned* Errno
   )
{
	ঘূৰাই _BlockIo(Handle, Offset, Bytes, Buffer, TRUE, Errno);
}



__inline
BOOLEAN
_SetPartType(
    IN HANDLE Handle,
    IN UCHAR Type
   )
{
	IO_STATUS_BLOCK IoStatusBlock;
	ঘূৰাই STATUS_SUCCESS == NtDeviceIoControlFile(
												   Handle, রিক্ত, রিক্ত, রিক্ত, &IoStatusBlock, IOCTL_DISK_SET_PARTITION_INFO,
												   &Type, পরিমাণ(Type),
												   রিক্ত, 0);
}



//--------------------- interface part

//
// Interface functions.
// Is_mounted is set to 1 if the device is mounted, 0 otherwise
//

errcode_t
ext2fs_check_if_mounted(ধ্রুৱ ৱর্ণ *file, পূর্ণাঙ্ক *mount_flags)
{
	HANDLE h;
	BOOLEAN Readonly;

	*mount_flags = 0;

	যদি(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, রিক্ত))
	{
		ঘূৰাই 0;
	}


	__try{
		*mount_flags &= _IsMounted(h) ? EXT2_MF_MOUNTED : 0;
	}
	__finally{
		_CloseDisk(h);
	}

	ঘূৰাই 0;
}



//
// Returns the number of blocks in a partition
//

স্থির __int64 FsSize = 0;
স্থির ৱর্ণ knowndevice[1024] = "";


errcode_t
ext2fs_get_device_size(ধ্রুৱ ৱর্ণ *file, পূর্ণাঙ্ক blocksize,
				 blk_t *retblocks)
{
	HANDLE h;
	BOOLEAN Readonly;

	যদি((0 == FsSize) || (0 != strcmp(knowndevice, file)))
	{

		যদি(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, রিক্ত))
		{
			ঘূৰাই 0;
		}


		__try{

			//
			// Get size
			//

			_GetDeviceSize(h, &FsSize);
			strcpy(knowndevice, file);
		}
		__finally{
			_CloseDisk(h);
		}

	}

	*retblocks = (blk_t)(unsigned __int64)(FsSize / blocksize);
	UNREFERENCED_PARAMETER(file);
	ঘূৰাই 0;
}






//
// Table elements
//


স্থির
errcode_t
nt_open(ধ্রুৱ ৱর্ণ *name, পূর্ণাঙ্ক flags, io_channel *channel)
{
	io_channel      io = রিক্ত;
	PNT_PRIVATE_DATA NtData = রিক্ত;
	errcode_t Errno = 0;

	//
	// Check name
	//

	যদি (রিক্ত == name)
	{
		ঘূৰাই EXT2_ET_BAD_DEVICE_NAME;
	}

	__try{

		//
		// Allocate channel handle
		//

		io = (io_channel) malloc(পরিমাণ(সংরচনা struct_io_channel));

		যদি (রিক্ত == io)
		{
			Errno = ENOMEM;
			__leave;
		}

		RtlZeroMemory(io, পরিমাণ(সংরচনা struct_io_channel));
		io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

		NtData = (PNT_PRIVATE_DATA)malloc(পরিমাণ(NT_PRIVATE_DATA));

		যদি (রিক্ত == NtData)
		{
			Errno = ENOMEM;
			__leave;
		}


		io->manager = nt_io_manager();
		io->name = malloc(strlen(name) + 1);
		যদি (রিক্ত == io->name)
		{
			Errno = ENOMEM;
			__leave;
		}

		strcpy(io->name, name);
		io->private_data = NtData;
		io->block_size = 1024;
		io->read_error = 0;
		io->write_error = 0;
		io->refcount = 1;

		//
		// Initialize data
		//

		RtlZeroMemory(NtData, পরিমাণ(NT_PRIVATE_DATA));

		NtData->magic = EXT2_ET_MAGIC_NT_IO_CHANNEL;
		NtData->Buffer_Block_সংখ্যা = 0xffffffff;
		NtData->BufferSize = 1024;
		NtData->Buffer = malloc(NtData->BufferSize);

		যদি (রিক্ত == NtData->Buffer)
		{
			Errno = ENOMEM;
			__leave;
		}

		//
		// Open it
		//

		যদি(!_Ext2OpenDevice(name, (BOOLEAN)!BooleanFlagOn(flags, EXT2_FLAG_RW), &NtData->Handle, &NtData->OpenedReadonly, &Errno))
		{
			__leave;
		}


		//
		// get size
		//

		_GetDeviceSize(NtData->Handle, &FsSize);
		strcpy(knowndevice, name);


		//
		// Lock/dismount
		//

		যদি(!NT_SUCCESS(_LockDrive(NtData->Handle)) /*|| !NT_SUCCESS(_DismountDrive(NtData->Handle))*/)
		{
			NtData->OpenedReadonly = TRUE;
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

			যদি (রিক্ত != io)
			{
				মুক্ত(io->name);
				মুক্ত(io);
			}

			যদি (রিক্ত != NtData)
			{
				যদি(রিক্ত != NtData->Handle)
				{
					_UnlockDrive(NtData->Handle);
					_CloseDisk(NtData->Handle);
				}

				মুক্ত(NtData->Buffer);
				মুক্ত(NtData);
			}
		}
	}

	ঘূৰাই Errno;
}


//
// Close api
//

স্থির
errcode_t
nt_close(io_channel channel)
{
	PNT_PRIVATE_DATA NtData = রিক্ত;

	যদি(রিক্ত == channel)
	{
		ঘূৰাই 0;
	}

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	যদি (--channel->refcount > 0)
	{
		ঘূৰাই 0;
	}

	মুক্ত(channel->name);
	মুক্ত(channel);

	যদি (রিক্ত != NtData)
	{
		যদি(রিক্ত != NtData->Handle)
		{
			_DismountDrive(NtData->Handle);
			_UnlockDrive(NtData->Handle);
			_CloseDisk(NtData->Handle);
		}

		মুক্ত(NtData->Buffer);
		মুক্ত(NtData);
	}

	ঘূৰাই 0;
}



//
// set block size
//

স্থির
errcode_t
nt_set_blksize(io_channel channel, পূর্ণাঙ্ক blksize)
{
	PNT_PRIVATE_DATA NtData = রিক্ত;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	যদি (channel->block_size != blksize)
	{
		channel->block_size = blksize;

		মুক্ত(NtData->Buffer);
		NtData->Buffer_Block_সংখ্যা = 0xffffffff;
		NtData->BufferSize = channel->block_size;
		ASSERT(0 == (NtData->BufferSize % 512));

		NtData->Buffer = malloc(NtData->BufferSize);

		যদি (রিক্ত == NtData->Buffer)
		{
			ঘূৰাই ENOMEM;
		}

	}

	ঘূৰাই 0;
}


//
// read block
//

স্থির
errcode_t
nt_read_blk(io_channel channel, unsigned long block,
			       পূর্ণাঙ্ক গুনতি, শূন্য *buf)
{
	PVOID BufferToRead;
	ULONG SizeToRead;
	ULONG Size;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_DATA NtData = রিক্ত;
	unsigned Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	//
	// If it's in the cache, use it!
	//

	যদি ((1 == গুনতি) &&
		(block == NtData->Buffer_Block_সংখ্যা) &&
		(NtData->Buffer_Block_সংখ্যা != 0xffffffff))
	{
		memcpy(buf, NtData->Buffer, channel->block_size);
		ঘূৰাই 0;
	}

	Size = (গুনতি < 0) ? (ULONG)(-গুনতি) : (ULONG)(গুনতি * channel->block_size);

	Offset.QuadPart = block * channel->block_size;

	//
	// If not fit to the block
	//

	যদি(Size <= NtData->BufferSize)
	{
		//
		// Update the cache
		//

		NtData->Buffer_Block_সংখ্যা = block;
		BufferToRead = NtData->Buffer;
		SizeToRead = NtData->BufferSize;
	}
	অন্যথা
	{
		SizeToRead = Size;
		BufferToRead = buf;
		ASSERT(0 == (SizeToRead % channel->block_size));
	}

	যদি(!_RawRead(NtData->Handle, Offset, SizeToRead, BufferToRead, &Errno))
	{

		যদি (channel->read_error)
		{
			ঘূৰাই (channel->read_error)(channel, block, গুনতি, buf,
					       Size, 0, Errno);
		}
		অন্যথা
		{
			ঘূৰাই Errno;
		}
	}


	যদি(BufferToRead != buf)
	{
		ASSERT(Size <= SizeToRead);
		memcpy(buf, BufferToRead, Size);
	}

	ঘূৰাই 0;
}


//
// write block
//

স্থির
errcode_t
nt_write_blk(io_channel channel, unsigned long block,
				পূর্ণাঙ্ক গুনতি, ধ্রুৱ শূন্য *buf)
{
	ULONG SizeToWrite;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_DATA NtData = রিক্ত;
	unsigned Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	যদি(NtData->OpenedReadonly)
	{
		ঘূৰাই EACCES;
	}

	যদি (গুনতি == 1)
	{
		SizeToWrite = channel->block_size;
	}
	অন্যথা
	{
		NtData->Buffer_Block_সংখ্যা = 0xffffffff;

		যদি (গুনতি < 0)
		{
			SizeToWrite = (ULONG)(-গুনতি);
		}
		অন্যথা
		{
			SizeToWrite = (ULONG)(গুনতি * channel->block_size);
		}
	}


	ASSERT(0 == (SizeToWrite % 512));
	Offset.QuadPart = block * channel->block_size;

	যদি(!_RawWrite(NtData->Handle, Offset, SizeToWrite, buf, &Errno))
	{
		যদি (channel->write_error)
		{
			ঘূৰাই (channel->write_error)(channel, block, গুনতি, buf,
						SizeToWrite, 0, Errno);
		}
		অন্যথা
		{
			ঘূৰাই Errno;
		}
	}


	//
	// Stash a copy.
	//

	যদি(SizeToWrite >= NtData->BufferSize)
	{
		NtData->Buffer_Block_সংখ্যা = block;
		memcpy(NtData->Buffer, buf, NtData->BufferSize);
	}

	NtData->Written = TRUE;

	ঘূৰাই 0;

}



//
// Flush data buffers to disk.  Since we are currently using a
// write-through cache, this is a no-op.
//

স্থির
errcode_t
nt_flush(io_channel channel)
{
	PNT_PRIVATE_DATA NtData = রিক্ত;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	যদি(NtData->OpenedReadonly)
	{
		ঘূৰাই 0; // EACCESS;
	}


	//
	// Flush file buffers.
	//

	_FlushDrive(NtData->Handle);


	//
	// Test and correct partition type.
	//

	যদি(NtData->Written)
	{
		_SetPartType(NtData->Handle, 0x83);
	}

	ঘূৰাই 0;
}


