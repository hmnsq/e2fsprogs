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
#ಅನ್ತರ್ಭಾವ "config.h"
#endif


//
// I need some warnings to disable...
//


#pragma warning(disable:4514) // unreferenced inline function has been removed
#pragma warning(ತಳ್ಳು,4)

#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union)
#pragma warning(disable:4214) // nonstandard extension used : bit field types other than int
#pragma warning(disable:4115) // named type definition in parentheses

#ಅನ್ತರ್ಭಾವ <ntddk.h>
#ಅನ್ತರ್ಭಾವ <ntdddisk.h>
#ಅನ್ತರ್ಭಾವ <ntstatus.h>

#pragma warning(ತೆಗೆ)


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
    IN PULONG ಕೀಲಿ_2 OPTIONAL
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
    IN PULONG ಕೀಲಿ_2 OPTIONAL
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
    IN PVOID ಒಳಹರಿವು_Buffer OPTIONAL,
    IN ULONG ಒಳಹರಿವು_Buffer_Length,
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
    IN PVOID ಒಳಹರಿವು_Buffer OPTIONAL,
    IN ULONG ಒಳಹರಿವು_Buffer_Length,
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

#ಅನ್ತರ್ಭಾವ <winerror.h>

//
// standard stuff
//

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <malloc.h>

#ಅನ್ತರ್ಭಾವ <linux/types.h>
#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ <errno.h>

#ಅನ್ತರ್ಭಾವ "et/com_err.h"
#ಅನ್ತರ್ಭಾವ "ext2fs/ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs/ext2_err.h"




//
// For checking structure magic numbers...
//


#define EXT2_CHECK_MAGIC(ಸಂರಚನಾ, code) \
	  ಯದಿ ((ಸಂರಚನಾ)->magic != (code)) ಪ್ರತ್ಯಾಗಮ (code)

#define EXT2_ET_MAGIC_NT_IO_CHANNEL  0x10ed


//
// Private data block
//

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ _NT_PRIVATE_DATA {
	ಪೂರ್ಣಾಙ್ಕ	   magic;
	HANDLE Handle;
	ಪೂರ್ಣಾಙ್ಕ	   Flags;
	PCHAR  Buffer;
	__u32  Buffer_Block_ಸಂಖ್ಯೆ;
	ULONG  BufferSize;
	BOOLEAN OpenedReadonly;
	BOOLEAN Written;
}NT_PRIVATE_DATA, *PNT_PRIVATE_DATA;



//
// Standard interface prototypes
//

ಸ್ಥಿರ errcode_t nt_open(ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ flags, io_channel *channel);
ಸ್ಥಿರ errcode_t nt_close(io_channel channel);
ಸ್ಥಿರ errcode_t nt_set_blksize(io_channel channel, ಪೂರ್ಣಾಙ್ಕ blksize);
ಸ್ಥಿರ errcode_t nt_read_blk(io_channel channel, unsigned long block,
			       ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಶೂನ್ಯ *data);
ಸ್ಥಿರ errcode_t nt_write_blk(io_channel channel, unsigned long block,
				ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಧ್ರುವ ಶೂನ್ಯ *data);
ಸ್ಥಿರ errcode_t nt_flush(io_channel channel);

ಸ್ಥಿರ ಸಂರಚನಾ struct_io_manager struct_nt_manager = {
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
	ಪ್ರತ್ಯಾಗಮ &struct_nt_manager;
}





//
// This is a code to convert Win32 errors to unix errno
//

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
	ULONG WinError;
	ಪೂರ್ಣಾಙ್ಕ errnocode;
}ERROR_ENTRY;

ಸ್ಥಿರ ERROR_ENTRY ErrorTable[] = {
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




ಸ್ಥಿರ
unsigned
_MapDosError (
    IN ULONG WinError
   )
{
	ಪೂರ್ಣಾಙ್ಕ i;

	//
	// Lookup
	//

	ಪ್ರತಿ (i = 0; i < (ಪರಿಮಾಣ(ErrorTable)/ಪರಿಮಾಣ(ErrorTable[0])); ++i)
	{
		ಯದಿ (WinError == ErrorTable[i].WinError)
		{
			ಪ್ರತ್ಯಾಗಮ ErrorTable[i].errnocode;
		}
	}

	//
	// not in table. Check ranges
	//

	ಯದಿ ((WinError >= ERROR_WRITE_PROTECT) &&
		(WinError <= ERROR_SHARING_BUFFER_EXCEEDED))
	{
		ಪ್ರತ್ಯಾಗಮ EACCES;
	}
	ಅನ್ಯಥಾ ಯದಿ ((WinError >= ERROR_INVALID_STARTING_CODESEG) &&
			 (WinError <= ERROR_INFLOOP_IN_RELOC_CHAIN))
	{
		ಪ್ರತ್ಯಾಗಮ ENOEXEC;
	}
	ಅನ್ಯಥಾ
	{
		ಪ್ರತ್ಯಾಗಮ EINVAL;
	}
}







//
// Function to map NT status to dos error.
//

ಸ್ಥಿರ
__inline
unsigned
_MapNtStatus(
    IN NTSTATUS Status
   )
{
	ಪ್ರತ್ಯಾಗಮ _MapDosError(RtlNtStatusToDosError(Status));
}





//
// Helper functions to make things easier
//

ಸ್ಥಿರ
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
	UnicodeString.MaximumLength = ಪರಿಮಾಣ(Buffer); // in bytes!!!

	RtlInitAnsiString(&AnsiString, Name);

	Status = RtlAnsiStringToUnicodeString(&UnicodeString, &AnsiString, FALSE);

	ಯದಿ(!NT_SUCCESS(Status))
	{
		ಪ್ರತ್ಯಾಗಮ Status; // Unmappable character?
	}

	//
	// Initialize object
	//

	InitializeObjectAttributes(&ObjectAttributes,
							   &UnicodeString,
							   OBJ_CASE_INSENSITIVE,
							   ರಿಕ್ತ,
							   ರಿಕ್ತ );

	//
	// Try to open it in initial mode
	//

	ಯದಿ(ARGUMENT_PRESENT(OpenedReadonly))
	{
		*OpenedReadonly = Readonly;
	}


	Status = NtOpenFile(Handle,
						SYNCHRONIZE | FILE_READ_DATA | (Readonly ? 0 : FILE_WRITE_DATA),
						&ObjectAttributes,
						&IoStatusBlock,
						FILE_SHARE_WRITE | FILE_SHARE_READ,
						FILE_SYNCHRONOUS_IO_NONALERT);

	ಯದಿ(!NT_SUCCESS(Status))
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

		ಯದಿ((STATUS_ACCESS_DENIED == Status) && !Readonly)
		{
			ಯದಿ(ARGUMENT_PRESENT(OpenedReadonly))
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

	ಪ್ರತ್ಯಾಗಮ Status;
}


ಸ್ಥಿರ
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

	ಪ್ರತ್ಯಾಗಮ _OpenNtName(Buffer, ReadOnly, Handle, OpenedReadonly);
}


//
// Flush device
//

ಸ್ಥಿರ
__inline
NTSTATUS
_FlushDrive(
		IN HANDLE Handle
		)
{
	IO_STATUS_BLOCK IoStatusBlock;
	ಪ್ರತ್ಯಾಗಮ NtFlushBuffersFile(Handle, &IoStatusBlock);
}


//
// lock drive
//

ಸ್ಥಿರ
__inline
NTSTATUS
_LockDrive(
		IN HANDLE Handle
		)
{
	IO_STATUS_BLOCK IoStatusBlock;
	ಪ್ರತ್ಯಾಗಮ NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_LOCK_VOLUME, 0, 0, 0, 0);
}


//
// unlock drive
//

ಸ್ಥಿರ
__inline
NTSTATUS
_UnlockDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	ಪ್ರತ್ಯಾಗಮ NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0);
}

ಸ್ಥಿರ
__inline
NTSTATUS
_DismountDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	ಪ್ರತ್ಯಾಗಮ NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0);
}


//
// is mounted
//

ಸ್ಥಿರ
__inline
BOOLEAN
_IsMounted(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	NTSTATUS Status;
	Status = NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_IS_VOLUME_MOUNTED, 0, 0, 0, 0);
	ಪ್ರತ್ಯಾಗಮ (BOOLEAN)(STATUS_SUCCESS == Status);
}


ಸ್ಥಿರ
__inline
NTSTATUS
_CloseDisk(
		IN HANDLE Handle
		)
{
	ಪ್ರತ್ಯಾಗಮ NtClose(Handle);
}




//
// Make NT name from any recognized name
//

ಸ್ಥಿರ
PCSTR
_NormalizeDeviceName(
    IN PCSTR Device,
    IN PSTR NormalizedDeviceNameBuffer
   )
{
	ಪೂರ್ಣಾಙ್ಕ Partition_ಸಂಖ್ಯೆ = -1;
	UCHAR Disk_ಸಂಖ್ಯೆ;
	PSTR p;


	//
	// Do not try to parse NT name
	//

	ಯದಿ('\\' == *Device)
		ಪ್ರತ್ಯಾಗಮ Device;



	//
	// Strip leading '/dev/' if any
	//

	ಯದಿ(('/' == *(Device)) &&
		('d' == *(Device + 1)) &&
		('e' == *(Device + 2)) &&
		('v' == *(Device + 3)) &&
		('/' == *(Device + 4)))
	{
		Device += 5;
	}

	ಯದಿ('\0' == *Device)
	{
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	}


	//
	// forms: hda[n], fd[n]
	//

	ಯದಿ('d' != *(Device + 1))
	{
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	}

	ಯದಿ('h' == *Device)
	{
		ಯದಿ((*(Device + 2) < 'a') || (*(Device + 2) > ('a' + 9)) ||
		   ((*(Device + 3) != '\0') &&
			((*(Device + 4) != '\0') ||
			 ((*(Device + 3) < '0') || (*(Device + 3) > '9'))
			)
		   )
		  )
		{
			ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
		}

		Disk_ಸಂಖ್ಯೆ = (UCHAR)(*(Device + 2) - 'a');

		ಯದಿ(*(Device + 3) != '\0')
		{
			Partition_ಸಂಖ್ಯೆ = (*(Device + 3) - '0');
		}

	}
	ಅನ್ಯಥಾ ಯದಿ('f' == *Device)
	{
		//
		// 3-d letter should be a digit.
		//

		ಯದಿ((*(Device + 3) != '\0') ||
		   (*(Device + 2) < '0') || (*(Device + 2) > '9'))
		{
			ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
		}

		Disk_ಸಂಖ್ಯೆ = (UCHAR)(*(Device + 2) - '0');

	}
	ಅನ್ಯಥಾ
	{
		//
		// invalid prefix
		//

		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	}



	//
	// Prefix
	//

	strcpy(NormalizedDeviceNameBuffer, "\\Device\\");

	//
	// Media name
	//

	ವಿಕಲ್ಪ(*Device)
	{

	ಪಕ್ಷ 'f':
		strcat(NormalizedDeviceNameBuffer, "Floppy0");
		ಭಙ್ಗ;

	ಪಕ್ಷ 'h':
		strcat(NormalizedDeviceNameBuffer, "Harddisk0");
		ಭಙ್ಗ;
	}


	p = NormalizedDeviceNameBuffer + strlen(NormalizedDeviceNameBuffer) - 1;
	*p = (CHAR)(*p + Disk_ಸಂಖ್ಯೆ);


	//
	// Partition nr.
	//

	ಯದಿ(Partition_ಸಂಖ್ಯೆ >= 0)
	{
		strcat(NormalizedDeviceNameBuffer, "\\Partition0");

		p = NormalizedDeviceNameBuffer + strlen(NormalizedDeviceNameBuffer) - 1;
		*p = (CHAR)(*p + Partition_ಸಂಖ್ಯೆ);
	}


	ಪ್ರತ್ಯಾಗಮ NormalizedDeviceNameBuffer;
}




ಸ್ಥಿರ
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

	RtlZeroMemory(&pi, ಪರಿಮಾಣ(PARTITION_INFORMATION));

	Status = NtDeviceIoControlFile(
		h, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, &IoStatusBlock, IOCTL_DISK_GET_PARTITION_INFO,
		&pi, ಪರಿಮಾಣ(PARTITION_INFORMATION),
		&pi, ಪರಿಮಾಣ(PARTITION_INFORMATION));


	ಯದಿ(NT_SUCCESS(Status))
	{
		*FsSize = pi.PartitionLength.QuadPart;
	}
	ಅನ್ಯಥಾ ಯದಿ(STATUS_INVALID_DEVICE_REQUEST == Status)
	{
		//
		// No partitions: get device info.
		//

		RtlZeroMemory(&gi, ಪರಿಮಾಣ(DISK_GEOMETRY));

		Status = NtDeviceIoControlFile(
				h, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, &IoStatusBlock, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, ಪರಿಮಾಣ(DISK_GEOMETRY),
				&gi, ಪರಿಮಾಣ(DISK_GEOMETRY));


		ಯದಿ(NT_SUCCESS(Status))
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

ಸ್ಥಿರ
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

	ಯದಿ(ರಿಕ್ತ == Name)
	{
		//
		// Set not found
		//

		ಯದಿ(ARGUMENT_PRESENT(Errno))
			*Errno = ENOENT;

		ಪ್ರತ್ಯಾಗಮ FALSE;
	}


	ಯದಿ((((*Name) | 0x20) >= 'a') && (((*Name) | 0x20) <= 'z') &&
		(':' == *(Name + 1)) && ('\0' == *(Name + 2)))
	{
		Status = _OpenDriveLetter(*Name, ReadOnly, Handle, OpenedReadonly);
	}
	ಅನ್ಯಥಾ
	{
		//
		// Make name
		//

		Name = _NormalizeDeviceName(Name, NormalizedDeviceName);

		ಯದಿ(ರಿಕ್ತ == Name)
		{
			//
			// Set not found
			//

			ಯದಿ(ARGUMENT_PRESENT(Errno))
				*Errno = ENOENT;

			ಪ್ರತ್ಯಾಗಮ FALSE;
		}

		//
		// Try to open it
		//

		Status = _OpenNtName(Name, ReadOnly, Handle, OpenedReadonly);
	}


	ಯದಿ(!NT_SUCCESS(Status))
	{
		ಯದಿ(ARGUMENT_PRESENT(Errno))
			*Errno = _MapNtStatus(Status);

		ಪ್ರತ್ಯಾಗಮ FALSE;
	}

	ಪ್ರತ್ಯಾಗಮ TRUE;
}


//
// Raw block io. Sets dos errno
//

ಸ್ಥಿರ
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
	ASSERT(0 == (Offset.ಕೆಳಗಿನ_Part % 512));


	//
	// perform io
	//

	ಯದಿ(Read)
	{
		Status = NtReadFile(Handle, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ,
			&IoStatusBlock, Buffer, Bytes, &Offset, ರಿಕ್ತ);
	}
	ಅನ್ಯಥಾ
	{
		Status = NtWriteFile(Handle, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ,
			&IoStatusBlock, Buffer, Bytes, &Offset, ರಿಕ್ತ);
	}


	//
	// translate error
	//

	ಯದಿ(NT_SUCCESS(Status))
	{
		*Errno = 0;
		ಪ್ರತ್ಯಾಗಮ TRUE;
	}

	*Errno = _MapNtStatus(Status);

	ಪ್ರತ್ಯಾಗಮ FALSE;
}



__inline
BOOLEAN
_RawWrite(
    IN HANDLE Handle,
    IN LARGE_INTEGER Offset,
    IN ULONG Bytes,
    OUT ಧ್ರುವ CHAR* Buffer,
    OUT unsigned* Errno
   )
{
	ಪ್ರತ್ಯಾಗಮ _BlockIo(Handle, Offset, Bytes, (PCHAR)Buffer, FALSE, Errno);
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
	ಪ್ರತ್ಯಾಗಮ _BlockIo(Handle, Offset, Bytes, Buffer, TRUE, Errno);
}



__inline
BOOLEAN
_SetPartType(
    IN HANDLE Handle,
    IN UCHAR Type
   )
{
	IO_STATUS_BLOCK IoStatusBlock;
	ಪ್ರತ್ಯಾಗಮ STATUS_SUCCESS == NtDeviceIoControlFile(
												   Handle, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ, &IoStatusBlock, IOCTL_DISK_SET_PARTITION_INFO,
												   &Type, ಪರಿಮಾಣ(Type),
												   ರಿಕ್ತ, 0);
}



//--------------------- interface part

//
// Interface functions.
// Is_mounted is set to 1 if the device is mounted, 0 otherwise
//

errcode_t
ext2fs_check_if_mounted(ಧ್ರುವ ವರ್ಣ *file, ಪೂರ್ಣಾಙ್ಕ *mount_flags)
{
	HANDLE h;
	BOOLEAN Readonly;

	*mount_flags = 0;

	ಯದಿ(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, ರಿಕ್ತ))
	{
		ಪ್ರತ್ಯಾಗಮ 0;
	}


	__try{
		*mount_flags &= _IsMounted(h) ? EXT2_MF_MOUNTED : 0;
	}
	__finally{
		_CloseDisk(h);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}



//
// Returns the number of blocks in a partition
//

ಸ್ಥಿರ __int64 FsSize = 0;
ಸ್ಥಿರ ವರ್ಣ knowndevice[1024] = "";


errcode_t
ext2fs_get_device_size(ಧ್ರುವ ವರ್ಣ *file, ಪೂರ್ಣಾಙ್ಕ blocksize,
				 blk_t *retblocks)
{
	HANDLE h;
	BOOLEAN Readonly;

	ಯದಿ((0 == FsSize) || (0 != strcmp(knowndevice, file)))
	{

		ಯದಿ(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, ರಿಕ್ತ))
		{
			ಪ್ರತ್ಯಾಗಮ 0;
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
	ಪ್ರತ್ಯಾಗಮ 0;
}






//
// Table elements
//


ಸ್ಥಿರ
errcode_t
nt_open(ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ flags, io_channel *channel)
{
	io_channel      io = ರಿಕ್ತ;
	PNT_PRIVATE_DATA NtData = ರಿಕ್ತ;
	errcode_t Errno = 0;

	//
	// Check name
	//

	ಯದಿ (ರಿಕ್ತ == name)
	{
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_BAD_DEVICE_NAME;
	}

	__try{

		//
		// Allocate channel handle
		//

		io = (io_channel) malloc(ಪರಿಮಾಣ(ಸಂರಚನಾ struct_io_channel));

		ಯದಿ (ರಿಕ್ತ == io)
		{
			Errno = ENOMEM;
			__leave;
		}

		RtlZeroMemory(io, ಪರಿಮಾಣ(ಸಂರಚನಾ struct_io_channel));
		io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

		NtData = (PNT_PRIVATE_DATA)malloc(ಪರಿಮಾಣ(NT_PRIVATE_DATA));

		ಯದಿ (ರಿಕ್ತ == NtData)
		{
			Errno = ENOMEM;
			__leave;
		}


		io->manager = nt_io_manager();
		io->name = malloc(strlen(name) + 1);
		ಯದಿ (ರಿಕ್ತ == io->name)
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

		RtlZeroMemory(NtData, ಪರಿಮಾಣ(NT_PRIVATE_DATA));

		NtData->magic = EXT2_ET_MAGIC_NT_IO_CHANNEL;
		NtData->Buffer_Block_ಸಂಖ್ಯೆ = 0xffffffff;
		NtData->BufferSize = 1024;
		NtData->Buffer = malloc(NtData->BufferSize);

		ಯದಿ (ರಿಕ್ತ == NtData->Buffer)
		{
			Errno = ENOMEM;
			__leave;
		}

		//
		// Open it
		//

		ಯದಿ(!_Ext2OpenDevice(name, (BOOLEAN)!BooleanFlagOn(flags, EXT2_FLAG_RW), &NtData->Handle, &NtData->OpenedReadonly, &Errno))
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

		ಯದಿ(!NT_SUCCESS(_LockDrive(NtData->Handle)) /*|| !NT_SUCCESS(_DismountDrive(NtData->Handle))*/)
		{
			NtData->OpenedReadonly = TRUE;
		}

		//
		// Done
		//

		*channel = io;


	}
	__finally{

		ಯದಿ(0 != Errno)
		{
			//
			// Cleanup
			//

			ಯದಿ (ರಿಕ್ತ != io)
			{
				ಮುಕ್ತ(io->name);
				ಮುಕ್ತ(io);
			}

			ಯದಿ (ರಿಕ್ತ != NtData)
			{
				ಯದಿ(ರಿಕ್ತ != NtData->Handle)
				{
					_UnlockDrive(NtData->Handle);
					_CloseDisk(NtData->Handle);
				}

				ಮುಕ್ತ(NtData->Buffer);
				ಮುಕ್ತ(NtData);
			}
		}
	}

	ಪ್ರತ್ಯಾಗಮ Errno;
}


//
// Close api
//

ಸ್ಥಿರ
errcode_t
nt_close(io_channel channel)
{
	PNT_PRIVATE_DATA NtData = ರಿಕ್ತ;

	ಯದಿ(ರಿಕ್ತ == channel)
	{
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	ಯದಿ (--channel->refcount > 0)
	{
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಮುಕ್ತ(channel->name);
	ಮುಕ್ತ(channel);

	ಯದಿ (ರಿಕ್ತ != NtData)
	{
		ಯದಿ(ರಿಕ್ತ != NtData->Handle)
		{
			_DismountDrive(NtData->Handle);
			_UnlockDrive(NtData->Handle);
			_CloseDisk(NtData->Handle);
		}

		ಮುಕ್ತ(NtData->Buffer);
		ಮುಕ್ತ(NtData);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}



//
// set block size
//

ಸ್ಥಿರ
errcode_t
nt_set_blksize(io_channel channel, ಪೂರ್ಣಾಙ್ಕ blksize)
{
	PNT_PRIVATE_DATA NtData = ರಿಕ್ತ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	ಯದಿ (channel->block_size != blksize)
	{
		channel->block_size = blksize;

		ಮುಕ್ತ(NtData->Buffer);
		NtData->Buffer_Block_ಸಂಖ್ಯೆ = 0xffffffff;
		NtData->BufferSize = channel->block_size;
		ASSERT(0 == (NtData->BufferSize % 512));

		NtData->Buffer = malloc(NtData->BufferSize);

		ಯದಿ (ರಿಕ್ತ == NtData->Buffer)
		{
			ಪ್ರತ್ಯಾಗಮ ENOMEM;
		}

	}

	ಪ್ರತ್ಯಾಗಮ 0;
}


//
// read block
//

ಸ್ಥಿರ
errcode_t
nt_read_blk(io_channel channel, unsigned long block,
			       ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಶೂನ್ಯ *buf)
{
	PVOID BufferToRead;
	ULONG SizeToRead;
	ULONG Size;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_DATA NtData = ರಿಕ್ತ;
	unsigned Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	//
	// If it's in the cache, use it!
	//

	ಯದಿ ((1 == ಎಣಿಕೆ) &&
		(block == NtData->Buffer_Block_ಸಂಖ್ಯೆ) &&
		(NtData->Buffer_Block_ಸಂಖ್ಯೆ != 0xffffffff))
	{
		memcpy(buf, NtData->Buffer, channel->block_size);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	Size = (ಎಣಿಕೆ < 0) ? (ULONG)(-ಎಣಿಕೆ) : (ULONG)(ಎಣಿಕೆ * channel->block_size);

	Offset.QuadPart = block * channel->block_size;

	//
	// If not fit to the block
	//

	ಯದಿ(Size <= NtData->BufferSize)
	{
		//
		// Update the cache
		//

		NtData->Buffer_Block_ಸಂಖ್ಯೆ = block;
		BufferToRead = NtData->Buffer;
		SizeToRead = NtData->BufferSize;
	}
	ಅನ್ಯಥಾ
	{
		SizeToRead = Size;
		BufferToRead = buf;
		ASSERT(0 == (SizeToRead % channel->block_size));
	}

	ಯದಿ(!_RawRead(NtData->Handle, Offset, SizeToRead, BufferToRead, &Errno))
	{

		ಯದಿ (channel->read_error)
		{
			ಪ್ರತ್ಯಾಗಮ (channel->read_error)(channel, block, ಎಣಿಕೆ, buf,
					       Size, 0, Errno);
		}
		ಅನ್ಯಥಾ
		{
			ಪ್ರತ್ಯಾಗಮ Errno;
		}
	}


	ಯದಿ(BufferToRead != buf)
	{
		ASSERT(Size <= SizeToRead);
		memcpy(buf, BufferToRead, Size);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}


//
// write block
//

ಸ್ಥಿರ
errcode_t
nt_write_blk(io_channel channel, unsigned long block,
				ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ಧ್ರುವ ಶೂನ್ಯ *buf)
{
	ULONG SizeToWrite;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_DATA NtData = ರಿಕ್ತ;
	unsigned Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	ಯದಿ(NtData->OpenedReadonly)
	{
		ಪ್ರತ್ಯಾಗಮ EACCES;
	}

	ಯದಿ (ಎಣಿಕೆ == 1)
	{
		SizeToWrite = channel->block_size;
	}
	ಅನ್ಯಥಾ
	{
		NtData->Buffer_Block_ಸಂಖ್ಯೆ = 0xffffffff;

		ಯದಿ (ಎಣಿಕೆ < 0)
		{
			SizeToWrite = (ULONG)(-ಎಣಿಕೆ);
		}
		ಅನ್ಯಥಾ
		{
			SizeToWrite = (ULONG)(ಎಣಿಕೆ * channel->block_size);
		}
	}


	ASSERT(0 == (SizeToWrite % 512));
	Offset.QuadPart = block * channel->block_size;

	ಯದಿ(!_RawWrite(NtData->Handle, Offset, SizeToWrite, buf, &Errno))
	{
		ಯದಿ (channel->write_error)
		{
			ಪ್ರತ್ಯಾಗಮ (channel->write_error)(channel, block, ಎಣಿಕೆ, buf,
						SizeToWrite, 0, Errno);
		}
		ಅನ್ಯಥಾ
		{
			ಪ್ರತ್ಯಾಗಮ Errno;
		}
	}


	//
	// Stash a copy.
	//

	ಯದಿ(SizeToWrite >= NtData->BufferSize)
	{
		NtData->Buffer_Block_ಸಂಖ್ಯೆ = block;
		memcpy(NtData->Buffer, buf, NtData->BufferSize);
	}

	NtData->Written = TRUE;

	ಪ್ರತ್ಯಾಗಮ 0;

}



//
// Flush data buffers to disk.  Since we are currently using a
// write-through cache, this is a no-op.
//

ಸ್ಥಿರ
errcode_t
nt_flush(io_channel channel)
{
	PNT_PRIVATE_DATA NtData = ರಿಕ್ತ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	ಯದಿ(NtData->OpenedReadonly)
	{
		ಪ್ರತ್ಯಾಗಮ 0; // EACCESS;
	}


	//
	// Flush file buffers.
	//

	_FlushDrive(NtData->Handle);


	//
	// Test and correct partition type.
	//

	ಯದಿ(NtData->Written)
	{
		_SetPartType(NtData->Handle, 0x83);
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}


