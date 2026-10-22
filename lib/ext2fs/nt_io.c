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
	  නම් ((sa_nracanaa)->magic != (code)) ආපසු (code)

#define EXT2_ET_MAGIC_NT_IO_CHANNEL  0x10ed


//
// Private data block
//

typedef sa_nracanaa _NT_PRIVATE_DATA {
	පූර්ණ_සංඛ්‍යා	   magic;
	HANDLE Handle;
	පූර්ණ_සංඛ්‍යා	   Flags;
	PCHAR  Buffer;
	__u32  BufferBlockNumber;
	ULONG  BufferSize;
	BOOLEAN OpenedReadonly;
	BOOLEAN Written;
}NT_PRIVATE_DATA, *PNT_PRIVATE_DATA;



//
// Standard interface prototypes
//

static errcode_t nt_open(const var_na *name, පූර්ණ_සංඛ්‍යා flags, io_channel *channel);
static errcode_t nt_close(io_channel channel);
static errcode_t nt_set_blksize(io_channel channel, පූර්ණ_සංඛ්‍යා blksize);
static errcode_t nt_read_blk(io_channel channel, unsigned long block,
			       පූර්ණ_සංඛ්‍යා count, void *data);
static errcode_t nt_write_blk(io_channel channel, unsigned long block,
				පූර්ණ_සංඛ්‍යා count, const void *data);
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
	ආපසු &struct_nt_manager;
}





//
// This is a code to convert Win32 errors to unix errno
//

typedef sa_nracanaa {
	ULONG WinError;
	පූර්ණ_සංඛ්‍යා errnocode;
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
	පූර්ණ_සංඛ්‍යා i;

	//
	// Lookup
	//

	සඳහා (i = 0; i < (parimaa_na(ErrorTable)/parimaa_na(ErrorTable[0])); ++i)
	{
		නම් (WinError == ErrorTable[i].WinError)
		{
			ආපසු ErrorTable[i].errnocode;
		}
	}

	//
	// not in table. Check ranges
	//

	නම් ((WinError >= ERROR_WRITE_PROTECT) &&
		(WinError <= ERROR_SHARING_BUFFER_EXCEEDED))
	{
		ආපසු EACCES;
	}
	නැතහොත් නම් ((WinError >= ERROR_INVALID_STARTING_CODESEG) &&
			 (WinError <= ERROR_INFLOOP_IN_RELOC_CHAIN))
	{
		ආපසු ENOEXEC;
	}
	නැතහොත්
	{
		ආපසු EINVAL;
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
	ආපසු _MapDosError(RtlNtStatusToDosError(Status));
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

	නම්(!NT_SUCCESS(Status))
	{
		ආපසු Status; // Unmappable character?
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

	නම්(ARGUMENT_PRESENT(OpenedReadonly))
	{
		*OpenedReadonly = Readonly;
	}


	Status = NtOpenFile(Handle,
						SYNCHRONIZE | FILE_READ_DATA | (Readonly ? 0 : FILE_WRITE_DATA),
						&ObjectAttributes,
						&IoStatusBlock,
						FILE_SHARE_WRITE | FILE_SHARE_READ,
						FILE_SYNCHRONOUS_IO_NONALERT);

	නම්(!NT_SUCCESS(Status))
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

		නම්((STATUS_ACCESS_DENIED == Status) && !Readonly)
		{
			නම්(ARGUMENT_PRESENT(OpenedReadonly))
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

	ආපසු Status;
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

	ආපසු _OpenNtName(Buffer, ReadOnly, Handle, OpenedReadonly);
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
	ආපසු NtFlushBuffersFile(Handle, &IoStatusBlock);
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
	ආපසු NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_LOCK_VOLUME, 0, 0, 0, 0);
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
	ආපසු NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0);
}

static
__inline
NTSTATUS
_DismountDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	ආපසු NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0);
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
	ආපසු (BOOLEAN)(STATUS_SUCCESS == Status);
}


static
__inline
NTSTATUS
_CloseDisk(
		IN HANDLE Handle
		)
{
	ආපසු NtClose(Handle);
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
	පූර්ණ_සංඛ්‍යා PartitionNumber = -1;
	UCHAR DiskNumber;
	PSTR p;


	//
	// Do not try to parse NT name
	//

	නම්('\\' == *Device)
		ආපසු Device;



	//
	// Strip leading '/dev/' if any
	//

	නම්(('/' == *(Device)) &&
		('d' == *(Device + 1)) &&
		('e' == *(Device + 2)) &&
		('v' == *(Device + 3)) &&
		('/' == *(Device + 4)))
	{
		Device += 5;
	}

	නම්('\0' == *Device)
	{
		ආපසු NULL;
	}


	//
	// forms: hda[n], fd[n]
	//

	නම්('d' != *(Device + 1))
	{
		ආපසු NULL;
	}

	නම්('h' == *Device)
	{
		නම්((*(Device + 2) < 'a') || (*(Device + 2) > ('a' + 9)) ||
		   ((*(Device + 3) != '\0') &&
			((*(Device + 4) != '\0') ||
			 ((*(Device + 3) < '0') || (*(Device + 3) > '9'))
			)
		   )
		  )
		{
			ආපසු NULL;
		}

		DiskNumber = (UCHAR)(*(Device + 2) - 'a');

		නම්(*(Device + 3) != '\0')
		{
			PartitionNumber = (*(Device + 3) - '0');
		}

	}
	නැතහොත් නම්('f' == *Device)
	{
		//
		// 3-d letter should be a digit.
		//

		නම්((*(Device + 3) != '\0') ||
		   (*(Device + 2) < '0') || (*(Device + 2) > '9'))
		{
			ආපසු NULL;
		}

		DiskNumber = (UCHAR)(*(Device + 2) - '0');

	}
	නැතහොත්
	{
		//
		// invalid prefix
		//

		ආපසු NULL;
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

	නම්(PartitionNumber >= 0)
	{
		strcat(NormalizedDeviceNameBuffer, "\\Partition0");

		p = NormalizedDeviceNameBuffer + strlen(NormalizedDeviceNameBuffer) - 1;
		*p = (CHAR)(*p + PartitionNumber);
	}


	ආපසු NormalizedDeviceNameBuffer;
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


	නම්(NT_SUCCESS(Status))
	{
		*FsSize = pi.PartitionLength.QuadPart;
	}
	නැතහොත් නම්(STATUS_INVALID_DEVICE_REQUEST == Status)
	{
		//
		// No partitions: get device info.
		//

		RtlZeroMemory(&gi, parimaa_na(DISK_GEOMETRY));

		Status = NtDeviceIoControlFile(
				h, NULL, NULL, NULL, &IoStatusBlock, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, parimaa_na(DISK_GEOMETRY),
				&gi, parimaa_na(DISK_GEOMETRY));


		නම්(NT_SUCCESS(Status))
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

	නම්(NULL == Name)
	{
		//
		// Set not found
		//

		නම්(ARGUMENT_PRESENT(Errno))
			*Errno = ENOENT;

		ආපසු FALSE;
	}


	නම්((((*Name) | 0x20) >= 'a') && (((*Name) | 0x20) <= 'z') &&
		(':' == *(Name + 1)) && ('\0' == *(Name + 2)))
	{
		Status = _OpenDriveLetter(*Name, ReadOnly, Handle, OpenedReadonly);
	}
	නැතහොත්
	{
		//
		// Make name
		//

		Name = _NormalizeDeviceName(Name, NormalizedDeviceName);

		නම්(NULL == Name)
		{
			//
			// Set not found
			//

			නම්(ARGUMENT_PRESENT(Errno))
				*Errno = ENOENT;

			ආපසු FALSE;
		}

		//
		// Try to open it
		//

		Status = _OpenNtName(Name, ReadOnly, Handle, OpenedReadonly);
	}


	නම්(!NT_SUCCESS(Status))
	{
		නම්(ARGUMENT_PRESENT(Errno))
			*Errno = _MapNtStatus(Status);

		ආපසු FALSE;
	}

	ආපසු TRUE;
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

	නම්(Read)
	{
		Status = NtReadFile(Handle, NULL, NULL, NULL,
			&IoStatusBlock, Buffer, Bytes, &Offset, NULL);
	}
	නැතහොත්
	{
		Status = NtWriteFile(Handle, NULL, NULL, NULL,
			&IoStatusBlock, Buffer, Bytes, &Offset, NULL);
	}


	//
	// translate error
	//

	නම්(NT_SUCCESS(Status))
	{
		*Errno = 0;
		ආපසු TRUE;
	}

	*Errno = _MapNtStatus(Status);

	ආපසු FALSE;
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
	ආපසු _BlockIo(Handle, Offset, Bytes, (PCHAR)Buffer, FALSE, Errno);
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
	ආපසු _BlockIo(Handle, Offset, Bytes, Buffer, TRUE, Errno);
}



__inline
BOOLEAN
_SetPartType(
    IN HANDLE Handle,
    IN UCHAR Type
   )
{
	IO_STATUS_BLOCK IoStatusBlock;
	ආපසු STATUS_SUCCESS == NtDeviceIoControlFile(
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
ext2fs_check_if_mounted(const var_na *file, පූර්ණ_සංඛ්‍යා *mount_flags)
{
	HANDLE h;
	BOOLEAN Readonly;

	*mount_flags = 0;

	නම්(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, NULL))
	{
		ආපසු 0;
	}


	__try{
		*mount_flags &= _IsMounted(h) ? EXT2_MF_MOUNTED : 0;
	}
	__finally{
		_CloseDisk(h);
	}

	ආපසු 0;
}



//
// Returns the number of blocks in a partition
//

static __int64 FsSize = 0;
static var_na knowndevice[1024] = "";


errcode_t
ext2fs_get_device_size(const var_na *file, පූර්ණ_සංඛ්‍යා blocksize,
				 blk_t *retblocks)
{
	HANDLE h;
	BOOLEAN Readonly;

	නම්((0 == FsSize) || (0 != strcmp(knowndevice, file)))
	{

		නම්(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, NULL))
		{
			ආපසු 0;
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
	ආපසු 0;
}






//
// Table elements
//


static
errcode_t
nt_open(const var_na *name, පූර්ණ_සංඛ්‍යා flags, io_channel *channel)
{
	io_channel      io = NULL;
	PNT_PRIVATE_DATA NtData = NULL;
	errcode_t Errno = 0;

	//
	// Check name
	//

	නම් (NULL == name)
	{
		ආපසු EXT2_ET_BAD_DEVICE_NAME;
	}

	__try{

		//
		// Allocate channel handle
		//

		io = (io_channel) malloc(parimaa_na(sa_nracanaa struct_io_channel));

		නම් (NULL == io)
		{
			Errno = ENOMEM;
			__leave;
		}

		RtlZeroMemory(io, parimaa_na(sa_nracanaa struct_io_channel));
		io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

		NtData = (PNT_PRIVATE_DATA)malloc(parimaa_na(NT_PRIVATE_DATA));

		නම් (NULL == NtData)
		{
			Errno = ENOMEM;
			__leave;
		}


		io->manager = nt_io_manager();
		io->name = malloc(strlen(name) + 1);
		නම් (NULL == io->name)
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

		නම් (NULL == NtData->Buffer)
		{
			Errno = ENOMEM;
			__leave;
		}

		//
		// Open it
		//

		නම්(!_Ext2OpenDevice(name, (BOOLEAN)!BooleanFlagOn(flags, EXT2_FLAG_RW), &NtData->Handle, &NtData->OpenedReadonly, &Errno))
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

		නම්(!NT_SUCCESS(_LockDrive(NtData->Handle)) /*|| !NT_SUCCESS(_DismountDrive(NtData->Handle))*/)
		{
			NtData->OpenedReadonly = TRUE;
		}

		//
		// Done
		//

		*channel = io;


	}
	__finally{

		නම්(0 != Errno)
		{
			//
			// Cleanup
			//

			නම් (NULL != io)
			{
				free(io->name);
				free(io);
			}

			නම් (NULL != NtData)
			{
				නම්(NULL != NtData->Handle)
				{
					_UnlockDrive(NtData->Handle);
					_CloseDisk(NtData->Handle);
				}

				free(NtData->Buffer);
				free(NtData);
			}
		}
	}

	ආපසු Errno;
}


//
// Close api
//

static
errcode_t
nt_close(io_channel channel)
{
	PNT_PRIVATE_DATA NtData = NULL;

	නම්(NULL == channel)
	{
		ආපසු 0;
	}

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	නම් (--channel->refcount > 0)
	{
		ආපසු 0;
	}

	free(channel->name);
	free(channel);

	නම් (NULL != NtData)
	{
		නම්(NULL != NtData->Handle)
		{
			_DismountDrive(NtData->Handle);
			_UnlockDrive(NtData->Handle);
			_CloseDisk(NtData->Handle);
		}

		free(NtData->Buffer);
		free(NtData);
	}

	ආපසු 0;
}



//
// set block size
//

static
errcode_t
nt_set_blksize(io_channel channel, පූර්ණ_සංඛ්‍යා blksize)
{
	PNT_PRIVATE_DATA NtData = NULL;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	නම් (channel->block_size != blksize)
	{
		channel->block_size = blksize;

		free(NtData->Buffer);
		NtData->BufferBlockNumber = 0xffffffff;
		NtData->BufferSize = channel->block_size;
		ASSERT(0 == (NtData->BufferSize % 512));

		NtData->Buffer = malloc(NtData->BufferSize);

		නම් (NULL == NtData->Buffer)
		{
			ආපසු ENOMEM;
		}

	}

	ආපසු 0;
}


//
// read block
//

static
errcode_t
nt_read_blk(io_channel channel, unsigned long block,
			       පූර්ණ_සංඛ්‍යා count, void *buf)
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

	නම් ((1 == count) &&
		(block == NtData->BufferBlockNumber) &&
		(NtData->BufferBlockNumber != 0xffffffff))
	{
		memcpy(buf, NtData->Buffer, channel->block_size);
		ආපසු 0;
	}

	Size = (count < 0) ? (ULONG)(-count) : (ULONG)(count * channel->block_size);

	Offset.QuadPart = block * channel->block_size;

	//
	// If not fit to the block
	//

	නම්(Size <= NtData->BufferSize)
	{
		//
		// Update the cache
		//

		NtData->BufferBlockNumber = block;
		BufferToRead = NtData->Buffer;
		SizeToRead = NtData->BufferSize;
	}
	නැතහොත්
	{
		SizeToRead = Size;
		BufferToRead = buf;
		ASSERT(0 == (SizeToRead % channel->block_size));
	}

	නම්(!_RawRead(NtData->Handle, Offset, SizeToRead, BufferToRead, &Errno))
	{

		නම් (channel->read_error)
		{
			ආපසු (channel->read_error)(channel, block, count, buf,
					       Size, 0, Errno);
		}
		නැතහොත්
		{
			ආපසු Errno;
		}
	}


	නම්(BufferToRead != buf)
	{
		ASSERT(Size <= SizeToRead);
		memcpy(buf, BufferToRead, Size);
	}

	ආපසු 0;
}


//
// write block
//

static
errcode_t
nt_write_blk(io_channel channel, unsigned long block,
				පූර්ණ_සංඛ්‍යා count, const void *buf)
{
	ULONG SizeToWrite;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_DATA NtData = NULL;
	unsigned Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	නම්(NtData->OpenedReadonly)
	{
		ආපසු EACCES;
	}

	නම් (count == 1)
	{
		SizeToWrite = channel->block_size;
	}
	නැතහොත්
	{
		NtData->BufferBlockNumber = 0xffffffff;

		නම් (count < 0)
		{
			SizeToWrite = (ULONG)(-count);
		}
		නැතහොත්
		{
			SizeToWrite = (ULONG)(count * channel->block_size);
		}
	}


	ASSERT(0 == (SizeToWrite % 512));
	Offset.QuadPart = block * channel->block_size;

	නම්(!_RawWrite(NtData->Handle, Offset, SizeToWrite, buf, &Errno))
	{
		නම් (channel->write_error)
		{
			ආපසු (channel->write_error)(channel, block, count, buf,
						SizeToWrite, 0, Errno);
		}
		නැතහොත්
		{
			ආපසු Errno;
		}
	}


	//
	// Stash a copy.
	//

	නම්(SizeToWrite >= NtData->BufferSize)
	{
		NtData->BufferBlockNumber = block;
		memcpy(NtData->Buffer, buf, NtData->BufferSize);
	}

	NtData->Written = TRUE;

	ආපසු 0;

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

	නම්(NtData->OpenedReadonly)
	{
		ආපසු 0; // EACCESS;
	}


	//
	// Flush file buffers.
	//

	_FlushDrive(NtData->Handle);


	//
	// Test and correct partition type.
	//

	නම්(NtData->Written)
	{
		_SetPartType(NtData->Handle, 0x83);
	}

	ආපසු 0;
}


