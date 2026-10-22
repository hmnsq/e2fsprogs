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
#include "config.h"
#endif


//
// I need some warnings to disable...
//


#pragma warning(disable:4514) // unreferenced inline function has been removed
#pragma warning(push,4)

#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union)
#pragma warning(disable:4214) // nonstandard extension used : bit field types other than int
#pragma warning(disable:4115) // named type definition in parentheses

#include <ntddk.h>
#include <ntdddisk.h>
#include <ntstatus.h>

#pragma warning(pop)


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
    IN PULONG Key OPTIONAL
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
    IN PULONG Key OPTIONAL
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
    IN PVOID InputBuffer OPTIONAL,
    IN ULONG InputBufferLength,
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
    IN PVOID InputBuffer OPTIONAL,
    IN ULONG InputBufferLength,
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

#include <winerror.h>

//
// standard stuff
//

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include <linux/types.h>
#include "ext2_fs.h"
#include <errno.h>

#include "et/com_err.h"
#include "ext2fs/ext2fs.h"
#include "ext2fs/ext2_err.h"




//
// For checking structure magic numbers...
//


#define EXT2_CHECK_MAGIC(sa_nracanaa, code) \
	  གལ་སྲིད ((sa_nracanaa)->magic != (code)) ལོག (code)

#define EXT2_ET_MAGIC_NT_IO_CHANNEL  0x10ed


//
// Private data block
//

typedef sa_nracanaa _NT_PRIVATE_DATA {
	ཧྲིལ	   magic;
	HANDLE Handle;
	ཧྲིལ	   Flags;
	PCHAR  Buffer;
	__u32  BufferBlockNumber;
	ULONG  BufferSize;
	BOOLEAN OpenedReadonly;
	BOOLEAN Written;
}NT_PRIVATE_DATA, *PNT_PRIVATE_DATA;



//
// Standard interface prototypes
//

static errcode_t nt_open(const var_na *name, ཧྲིལ flags, io_channel *channel);
static errcode_t nt_close(io_channel channel);
static errcode_t nt_set_blksize(io_channel channel, ཧྲིལ blksize);
static errcode_t nt_read_blk(io_channel channel, unsigned long block,
			       ཧྲིལ count, void *data);
static errcode_t nt_write_blk(io_channel channel, unsigned long block,
				ཧྲིལ count, const void *data);
static errcode_t nt_flush(io_channel channel);

static sa_nracanaa struct_io_manager struct_nt_manager = {
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
	ལོག &struct_nt_manager;
}





//
// This is a code to convert Win32 errors to unix errno
//

typedef sa_nracanaa {
	ULONG WinError;
	ཧྲིལ errnocode;
}ERROR_ENTRY;

static ERROR_ENTRY ErrorTable[] = {
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




static
unsigned
_MapDosError (
    IN ULONG WinError
   )
{
	ཧྲིལ i;

	//
	// Lookup
	//

	དོན་ལུ (i = 0; i < (parimaa_na(ErrorTable)/parimaa_na(ErrorTable[0])); ++i)
	{
		གལ་སྲིད (WinError == ErrorTable[i].WinError)
		{
			ལོག ErrorTable[i].errnocode;
		}
	}

	//
	// not in table. Check ranges
	//

	གལ་སྲིད ((WinError >= ERROR_WRITE_PROTECT) &&
		(WinError <= ERROR_SHARING_BUFFER_EXCEEDED))
	{
		ལོག EACCES;
	}
	དེ་མིན གལ་སྲིད ((WinError >= ERROR_INVALID_STARTING_CODESEG) &&
			 (WinError <= ERROR_INFLOOP_IN_RELOC_CHAIN))
	{
		ལོག ENOEXEC;
	}
	དེ་མིན
	{
		ལོག EINVAL;
	}
}







//
// Function to map NT status to dos error.
//

static
__inline
unsigned
_MapNtStatus(
    IN NTSTATUS Status
   )
{
	ལོག _MapDosError(RtlNtStatusToDosError(Status));
}





//
// Helper functions to make things easier
//

static
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
	UnicodeString.MaximumLength = parimaa_na(Buffer); // in bytes!!!

	RtlInitAnsiString(&AnsiString, Name);

	Status = RtlAnsiStringToUnicodeString(&UnicodeString, &AnsiString, FALSE);

	གལ་སྲིད(!NT_SUCCESS(Status))
	{
		ལོག Status; // Unmappable character?
	}

	//
	// Initialize object
	//

	InitializeObjectAttributes(&ObjectAttributes,
							   &UnicodeString,
							   OBJ_CASE_INSENSITIVE,
							   NULL,
							   NULL );

	//
	// Try to open it in initial mode
	//

	གལ་སྲིད(ARGUMENT_PRESENT(OpenedReadonly))
	{
		*OpenedReadonly = Readonly;
	}


	Status = NtOpenFile(Handle,
						SYNCHRONIZE | FILE_READ_DATA | (Readonly ? 0 : FILE_WRITE_DATA),
						&ObjectAttributes,
						&IoStatusBlock,
						FILE_SHARE_WRITE | FILE_SHARE_READ,
						FILE_SYNCHRONOUS_IO_NONALERT);

	གལ་སྲིད(!NT_SUCCESS(Status))
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

		གལ་སྲིད((STATUS_ACCESS_DENIED == Status) && !Readonly)
		{
			གལ་སྲིད(ARGUMENT_PRESENT(OpenedReadonly))
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

	ལོག Status;
}


static
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

	ལོག _OpenNtName(Buffer, ReadOnly, Handle, OpenedReadonly);
}


//
// Flush device
//

static
__inline
NTSTATUS
_FlushDrive(
		IN HANDLE Handle
		)
{
	IO_STATUS_BLOCK IoStatusBlock;
	ལོག NtFlushBuffersFile(Handle, &IoStatusBlock);
}


//
// lock drive
//

static
__inline
NTSTATUS
_LockDrive(
		IN HANDLE Handle
		)
{
	IO_STATUS_BLOCK IoStatusBlock;
	ལོག NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_LOCK_VOLUME, 0, 0, 0, 0);
}


//
// unlock drive
//

static
__inline
NTSTATUS
_UnlockDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	ལོག NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0);
}

static
__inline
NTSTATUS
_DismountDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	ལོག NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0);
}


//
// is mounted
//

static
__inline
BOOLEAN
_IsMounted(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	NTSTATUS Status;
	Status = NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_IS_VOLUME_MOUNTED, 0, 0, 0, 0);
	ལོག (BOOLEAN)(STATUS_SUCCESS == Status);
}


static
__inline
NTSTATUS
_CloseDisk(
		IN HANDLE Handle
		)
{
	ལོག NtClose(Handle);
}




//
// Make NT name from any recognized name
//

static
PCSTR
_NormalizeDeviceName(
    IN PCSTR Device,
    IN PSTR NormalizedDeviceNameBuffer
   )
{
	ཧྲིལ PartitionNumber = -1;
	UCHAR DiskNumber;
	PSTR p;


	//
	// Do not try to parse NT name
	//

	གལ་སྲིད('\\' == *Device)
		ལོག Device;



	//
	// Strip leading '/dev/' if any
	//

	གལ་སྲིད(('/' == *(Device)) &&
		('d' == *(Device + 1)) &&
		('e' == *(Device + 2)) &&
		('v' == *(Device + 3)) &&
		('/' == *(Device + 4)))
	{
		Device += 5;
	}

	གལ་སྲིད('\0' == *Device)
	{
		ལོག NULL;
	}


	//
	// forms: hda[n], fd[n]
	//

	གལ་སྲིད('d' != *(Device + 1))
	{
		ལོག NULL;
	}

	གལ་སྲིད('h' == *Device)
	{
		གལ་སྲིད((*(Device + 2) < 'a') || (*(Device + 2) > ('a' + 9)) ||
		   ((*(Device + 3) != '\0') &&
			((*(Device + 4) != '\0') ||
			 ((*(Device + 3) < '0') || (*(Device + 3) > '9'))
			)
		   )
		  )
		{
			ལོག NULL;
		}

		DiskNumber = (UCHAR)(*(Device + 2) - 'a');

		གལ་སྲིད(*(Device + 3) != '\0')
		{
			PartitionNumber = (*(Device + 3) - '0');
		}

	}
	དེ་མིན གལ་སྲིད('f' == *Device)
	{
		//
		// 3-d letter should be a digit.
		//

		གལ་སྲིད((*(Device + 3) != '\0') ||
		   (*(Device + 2) < '0') || (*(Device + 2) > '9'))
		{
			ལོག NULL;
		}

		DiskNumber = (UCHAR)(*(Device + 2) - '0');

	}
	དེ་མིན
	{
		//
		// invalid prefix
		//

		ལོག NULL;
	}



	//
	// Prefix
	//

	strcpy(NormalizedDeviceNameBuffer, "\\Device\\");

	//
	// Media name
	//

	switch(*Device)
	{

	pak_sa 'f':
		strcat(NormalizedDeviceNameBuffer, "Floppy0");
		bha_nga;

	pak_sa 'h':
		strcat(NormalizedDeviceNameBuffer, "Harddisk0");
		bha_nga;
	}


	p = NormalizedDeviceNameBuffer + strlen(NormalizedDeviceNameBuffer) - 1;
	*p = (CHAR)(*p + DiskNumber);


	//
	// Partition nr.
	//

	གལ་སྲིད(PartitionNumber >= 0)
	{
		strcat(NormalizedDeviceNameBuffer, "\\Partition0");

		p = NormalizedDeviceNameBuffer + strlen(NormalizedDeviceNameBuffer) - 1;
		*p = (CHAR)(*p + PartitionNumber);
	}


	ལོག NormalizedDeviceNameBuffer;
}




static
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

	RtlZeroMemory(&pi, parimaa_na(PARTITION_INFORMATION));

	Status = NtDeviceIoControlFile(
		h, NULL, NULL, NULL, &IoStatusBlock, IOCTL_DISK_GET_PARTITION_INFO,
		&pi, parimaa_na(PARTITION_INFORMATION),
		&pi, parimaa_na(PARTITION_INFORMATION));


	གལ་སྲིད(NT_SUCCESS(Status))
	{
		*FsSize = pi.PartitionLength.QuadPart;
	}
	དེ་མིན གལ་སྲིད(STATUS_INVALID_DEVICE_REQUEST == Status)
	{
		//
		// No partitions: get device info.
		//

		RtlZeroMemory(&gi, parimaa_na(DISK_GEOMETRY));

		Status = NtDeviceIoControlFile(
				h, NULL, NULL, NULL, &IoStatusBlock, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, parimaa_na(DISK_GEOMETRY),
				&gi, parimaa_na(DISK_GEOMETRY));


		གལ་སྲིད(NT_SUCCESS(Status))
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

static
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

	གལ་སྲིད(NULL == Name)
	{
		//
		// Set not found
		//

		གལ་སྲིད(ARGUMENT_PRESENT(Errno))
			*Errno = ENOENT;

		ལོག FALSE;
	}


	གལ་སྲིད((((*Name) | 0x20) >= 'a') && (((*Name) | 0x20) <= 'z') &&
		(':' == *(Name + 1)) && ('\0' == *(Name + 2)))
	{
		Status = _OpenDriveLetter(*Name, ReadOnly, Handle, OpenedReadonly);
	}
	དེ་མིན
	{
		//
		// Make name
		//

		Name = _NormalizeDeviceName(Name, NormalizedDeviceName);

		གལ་སྲིད(NULL == Name)
		{
			//
			// Set not found
			//

			གལ་སྲིད(ARGUMENT_PRESENT(Errno))
				*Errno = ENOENT;

			ལོག FALSE;
		}

		//
		// Try to open it
		//

		Status = _OpenNtName(Name, ReadOnly, Handle, OpenedReadonly);
	}


	གལ་སྲིད(!NT_SUCCESS(Status))
	{
		གལ་སྲིད(ARGUMENT_PRESENT(Errno))
			*Errno = _MapNtStatus(Status);

		ལོག FALSE;
	}

	ལོག TRUE;
}


//
// Raw block io. Sets dos errno
//

static
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
	ASSERT(0 == (Offset.LowPart % 512));


	//
	// perform io
	//

	གལ་སྲིད(Read)
	{
		Status = NtReadFile(Handle, NULL, NULL, NULL,
			&IoStatusBlock, Buffer, Bytes, &Offset, NULL);
	}
	དེ་མིན
	{
		Status = NtWriteFile(Handle, NULL, NULL, NULL,
			&IoStatusBlock, Buffer, Bytes, &Offset, NULL);
	}


	//
	// translate error
	//

	གལ་སྲིད(NT_SUCCESS(Status))
	{
		*Errno = 0;
		ལོག TRUE;
	}

	*Errno = _MapNtStatus(Status);

	ལོག FALSE;
}



__inline
BOOLEAN
_RawWrite(
    IN HANDLE Handle,
    IN LARGE_INTEGER Offset,
    IN ULONG Bytes,
    OUT const CHAR* Buffer,
    OUT unsigned* Errno
   )
{
	ལོག _BlockIo(Handle, Offset, Bytes, (PCHAR)Buffer, FALSE, Errno);
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
	ལོག _BlockIo(Handle, Offset, Bytes, Buffer, TRUE, Errno);
}



__inline
BOOLEAN
_SetPartType(
    IN HANDLE Handle,
    IN UCHAR Type
   )
{
	IO_STATUS_BLOCK IoStatusBlock;
	ལོག STATUS_SUCCESS == NtDeviceIoControlFile(
												   Handle, NULL, NULL, NULL, &IoStatusBlock, IOCTL_DISK_SET_PARTITION_INFO,
												   &Type, parimaa_na(Type),
												   NULL, 0);
}



//--------------------- interface part

//
// Interface functions.
// Is_mounted is set to 1 if the device is mounted, 0 otherwise
//

errcode_t
ext2fs_check_if_mounted(const var_na *file, ཧྲིལ *mount_flags)
{
	HANDLE h;
	BOOLEAN Readonly;

	*mount_flags = 0;

	གལ་སྲིད(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, NULL))
	{
		ལོག 0;
	}


	__try{
		*mount_flags &= _IsMounted(h) ? EXT2_MF_MOUNTED : 0;
	}
	__finally{
		_CloseDisk(h);
	}

	ལོག 0;
}



//
// Returns the number of blocks in a partition
//

static __int64 FsSize = 0;
static var_na knowndevice[1024] = "";


errcode_t
ext2fs_get_device_size(const var_na *file, ཧྲིལ blocksize,
				 blk_t *retblocks)
{
	HANDLE h;
	BOOLEAN Readonly;

	གལ་སྲིད((0 == FsSize) || (0 != strcmp(knowndevice, file)))
	{

		གལ་སྲིད(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, NULL))
		{
			ལོག 0;
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
	ལོག 0;
}






//
// Table elements
//


static
errcode_t
nt_open(const var_na *name, ཧྲིལ flags, io_channel *channel)
{
	io_channel      io = NULL;
	PNT_PRIVATE_DATA NtData = NULL;
	errcode_t Errno = 0;

	//
	// Check name
	//

	གལ་སྲིད (NULL == name)
	{
		ལོག EXT2_ET_BAD_DEVICE_NAME;
	}

	__try{

		//
		// Allocate channel handle
		//

		io = (io_channel) malloc(parimaa_na(sa_nracanaa struct_io_channel));

		གལ་སྲིད (NULL == io)
		{
			Errno = ENOMEM;
			__leave;
		}

		RtlZeroMemory(io, parimaa_na(sa_nracanaa struct_io_channel));
		io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

		NtData = (PNT_PRIVATE_DATA)malloc(parimaa_na(NT_PRIVATE_DATA));

		གལ་སྲིད (NULL == NtData)
		{
			Errno = ENOMEM;
			__leave;
		}


		io->manager = nt_io_manager();
		io->name = malloc(strlen(name) + 1);
		གལ་སྲིད (NULL == io->name)
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

		RtlZeroMemory(NtData, parimaa_na(NT_PRIVATE_DATA));

		NtData->magic = EXT2_ET_MAGIC_NT_IO_CHANNEL;
		NtData->BufferBlockNumber = 0xffffffff;
		NtData->BufferSize = 1024;
		NtData->Buffer = malloc(NtData->BufferSize);

		གལ་སྲིད (NULL == NtData->Buffer)
		{
			Errno = ENOMEM;
			__leave;
		}

		//
		// Open it
		//

		གལ་སྲིད(!_Ext2OpenDevice(name, (BOOLEAN)!BooleanFlagOn(flags, EXT2_FLAG_RW), &NtData->Handle, &NtData->OpenedReadonly, &Errno))
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

		གལ་སྲིད(!NT_SUCCESS(_LockDrive(NtData->Handle)) /*|| !NT_SUCCESS(_DismountDrive(NtData->Handle))*/)
		{
			NtData->OpenedReadonly = TRUE;
		}

		//
		// Done
		//

		*channel = io;


	}
	__finally{

		གལ་སྲིད(0 != Errno)
		{
			//
			// Cleanup
			//

			གལ་སྲིད (NULL != io)
			{
				free(io->name);
				free(io);
			}

			གལ་སྲིད (NULL != NtData)
			{
				གལ་སྲིད(NULL != NtData->Handle)
				{
					_UnlockDrive(NtData->Handle);
					_CloseDisk(NtData->Handle);
				}

				free(NtData->Buffer);
				free(NtData);
			}
		}
	}

	ལོག Errno;
}


//
// Close api
//

static
errcode_t
nt_close(io_channel channel)
{
	PNT_PRIVATE_DATA NtData = NULL;

	གལ་སྲིད(NULL == channel)
	{
		ལོག 0;
	}

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	གལ་སྲིད (--channel->refcount > 0)
	{
		ལོག 0;
	}

	free(channel->name);
	free(channel);

	གལ་སྲིད (NULL != NtData)
	{
		གལ་སྲིད(NULL != NtData->Handle)
		{
			_DismountDrive(NtData->Handle);
			_UnlockDrive(NtData->Handle);
			_CloseDisk(NtData->Handle);
		}

		free(NtData->Buffer);
		free(NtData);
	}

	ལོག 0;
}



//
// set block size
//

static
errcode_t
nt_set_blksize(io_channel channel, ཧྲིལ blksize)
{
	PNT_PRIVATE_DATA NtData = NULL;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	གལ་སྲིད (channel->block_size != blksize)
	{
		channel->block_size = blksize;

		free(NtData->Buffer);
		NtData->BufferBlockNumber = 0xffffffff;
		NtData->BufferSize = channel->block_size;
		ASSERT(0 == (NtData->BufferSize % 512));

		NtData->Buffer = malloc(NtData->BufferSize);

		གལ་སྲིད (NULL == NtData->Buffer)
		{
			ལོག ENOMEM;
		}

	}

	ལོག 0;
}


//
// read block
//

static
errcode_t
nt_read_blk(io_channel channel, unsigned long block,
			       ཧྲིལ count, void *buf)
{
	PVOID BufferToRead;
	ULONG SizeToRead;
	ULONG Size;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_DATA NtData = NULL;
	unsigned Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	//
	// If it's in the cache, use it!
	//

	གལ་སྲིད ((1 == count) &&
		(block == NtData->BufferBlockNumber) &&
		(NtData->BufferBlockNumber != 0xffffffff))
	{
		memcpy(buf, NtData->Buffer, channel->block_size);
		ལོག 0;
	}

	Size = (count < 0) ? (ULONG)(-count) : (ULONG)(count * channel->block_size);

	Offset.QuadPart = block * channel->block_size;

	//
	// If not fit to the block
	//

	གལ་སྲིད(Size <= NtData->BufferSize)
	{
		//
		// Update the cache
		//

		NtData->BufferBlockNumber = block;
		BufferToRead = NtData->Buffer;
		SizeToRead = NtData->BufferSize;
	}
	དེ་མིན
	{
		SizeToRead = Size;
		BufferToRead = buf;
		ASSERT(0 == (SizeToRead % channel->block_size));
	}

	གལ་སྲིད(!_RawRead(NtData->Handle, Offset, SizeToRead, BufferToRead, &Errno))
	{

		གལ་སྲིད (channel->read_error)
		{
			ལོག (channel->read_error)(channel, block, count, buf,
					       Size, 0, Errno);
		}
		དེ་མིན
		{
			ལོག Errno;
		}
	}


	གལ་སྲིད(BufferToRead != buf)
	{
		ASSERT(Size <= SizeToRead);
		memcpy(buf, BufferToRead, Size);
	}

	ལོག 0;
}


//
// write block
//

static
errcode_t
nt_write_blk(io_channel channel, unsigned long block,
				ཧྲིལ count, const void *buf)
{
	ULONG SizeToWrite;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_DATA NtData = NULL;
	unsigned Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	གལ་སྲིད(NtData->OpenedReadonly)
	{
		ལོག EACCES;
	}

	གལ་སྲིད (count == 1)
	{
		SizeToWrite = channel->block_size;
	}
	དེ་མིན
	{
		NtData->BufferBlockNumber = 0xffffffff;

		གལ་སྲིད (count < 0)
		{
			SizeToWrite = (ULONG)(-count);
		}
		དེ་མིན
		{
			SizeToWrite = (ULONG)(count * channel->block_size);
		}
	}


	ASSERT(0 == (SizeToWrite % 512));
	Offset.QuadPart = block * channel->block_size;

	གལ་སྲིད(!_RawWrite(NtData->Handle, Offset, SizeToWrite, buf, &Errno))
	{
		གལ་སྲིད (channel->write_error)
		{
			ལོག (channel->write_error)(channel, block, count, buf,
						SizeToWrite, 0, Errno);
		}
		དེ་མིན
		{
			ལོག Errno;
		}
	}


	//
	// Stash a copy.
	//

	གལ་སྲིད(SizeToWrite >= NtData->BufferSize)
	{
		NtData->BufferBlockNumber = block;
		memcpy(NtData->Buffer, buf, NtData->BufferSize);
	}

	NtData->Written = TRUE;

	ལོག 0;

}



//
// Flush data buffers to disk.  Since we are currently using a
// write-through cache, this is a no-op.
//

static
errcode_t
nt_flush(io_channel channel)
{
	PNT_PRIVATE_DATA NtData = NULL;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	གལ་སྲིད(NtData->OpenedReadonly)
	{
		ལོག 0; // EACCESS;
	}


	//
	// Flush file buffers.
	//

	_FlushDrive(NtData->Handle);


	//
	// Test and correct partition type.
	//

	གལ་སྲིད(NtData->Written)
	{
		_SetPartType(NtData->Handle, 0x83);
	}

	ལོག 0;
}


