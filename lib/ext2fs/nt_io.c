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
#شامل "config.h"
#endif


//
// I need some warnings to disable...
//


#pragma warning(disable:4514) // unreferenced inline function has been removed
#pragma warning(افزودن,4)

#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union)
#pragma warning(disable:4214) // nonstandard extension used : bit field types other than int
#pragma warning(disable:4115) // named type definition in parentheses

#شامل <ntddk.h>
#شامل <ntdddisk.h>
#شامل <ntstatus.h>

#pragma warning(برداشت)


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
    IN PULONG کلید_2 OPTIONAL
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
    IN PULONG کلید_2 OPTIONAL
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
    IN PVOID درون_داد_Buffer OPTIONAL,
    IN ULONG درون_داد_Buffer_Length,
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
    IN PVOID درون_داد_Buffer OPTIONAL,
    IN ULONG درون_داد_Buffer_Length,
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

#شامل <winerror.h>

//
// standard stuff
//

#شامل <assert.h>
#شامل <stdio.h>
#شامل <string.h>
#شامل <stdlib.h>
#شامل <malloc.h>

#شامل <linux/types.h>
#شامل "ext2_fs.h"
#شامل <errno.h>

#شامل "et/com_err.h"
#شامل "ext2fs/ext2fs.h"
#شامل "ext2fs/ext2_err.h"




//
// For checking structure magic numbers...
//


#define EXT2_CHECK_MAGIC(ساختار, code) \
	  اگر ((ساختار)->magic != (code)) بازگشت (code)

#define EXT2_ET_MAGIC_NT_IO_CHANNEL  0x10ed


//
// Private data block
//

تعریفنوع ساختار _NT_PRIVATE_DATA {
	صحیح	   magic;
	HANDLE Handle;
	صحیح	   Flags;
	PCHAR  Buffer;
	__u32  Buffer_Block_عدد;
	ULONG  BufferSize;
	BOOLEAN OpenedReadonly;
	BOOLEAN Written;
}NT_PRIVATE_DATA, *PNT_PRIVATE_DATA;



//
// Standard interface prototypes
//

ایستا errcode_t nt_open(ثابت نویسه *name, صحیح flags, io_channel *channel);
ایستا errcode_t nt_close(io_channel channel);
ایستا errcode_t nt_set_blksize(io_channel channel, صحیح blksize);
ایستا errcode_t nt_read_blk(io_channel channel, unsigned long block,
			       صحیح شمار, تهی *data);
ایستا errcode_t nt_write_blk(io_channel channel, unsigned long block,
				صحیح شمار, ثابت تهی *data);
ایستا errcode_t nt_flush(io_channel channel);

ایستا ساختار struct_io_manager struct_nt_manager = {
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
	بازگشت &struct_nt_manager;
}





//
// This is a code to convert Win32 errors to unix errno
//

تعریفنوع ساختار {
	ULONG WinError;
	صحیح errnocode;
}ERROR_ENTRY;

ایستا ERROR_ENTRY ErrorTable[] = {
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




ایستا
unsigned
_MapDosError (
    IN ULONG WinError
   )
{
	صحیح i;

	//
	// Lookup
	//

	برای (i = 0; i < (اندازه(ErrorTable)/اندازه(ErrorTable[0])); ++i)
	{
		اگر (WinError == ErrorTable[i].WinError)
		{
			بازگشت ErrorTable[i].errnocode;
		}
	}

	//
	// not in table. Check ranges
	//

	اگر ((WinError >= ERROR_WRITE_PROTECT) &&
		(WinError <= ERROR_SHARING_BUFFER_EXCEEDED))
	{
		بازگشت EACCES;
	}
	وگرنه اگر ((WinError >= ERROR_INVALID_STARTING_CODESEG) &&
			 (WinError <= ERROR_INFLOOP_IN_RELOC_CHAIN))
	{
		بازگشت ENOEXEC;
	}
	وگرنه
	{
		بازگشت EINVAL;
	}
}







//
// Function to map NT status to dos error.
//

ایستا
__inline
unsigned
_MapNtStatus(
    IN NTSTATUS Status
   )
{
	بازگشت _MapDosError(RtlNtStatusToDosError(Status));
}





//
// Helper functions to make things easier
//

ایستا
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
	UnicodeString.MaximumLength = اندازه(Buffer); // in bytes!!!

	RtlInitAnsiString(&AnsiString, Name);

	Status = RtlAnsiStringToUnicodeString(&UnicodeString, &AnsiString, FALSE);

	اگر(!NT_SUCCESS(Status))
	{
		بازگشت Status; // Unmappable character?
	}

	//
	// Initialize object
	//

	InitializeObjectAttributes(&ObjectAttributes,
							   &UnicodeString,
							   OBJ_CASE_INSENSITIVE,
							   پوچ,
							   پوچ );

	//
	// Try to open it in initial mode
	//

	اگر(ARGUMENT_PRESENT(OpenedReadonly))
	{
		*OpenedReadonly = Readonly;
	}


	Status = NtOpenFile(Handle,
						SYNCHRONIZE | FILE_READ_DATA | (Readonly ? 0 : FILE_WRITE_DATA),
						&ObjectAttributes,
						&IoStatusBlock,
						FILE_SHARE_WRITE | FILE_SHARE_READ,
						FILE_SYNCHRONOUS_IO_NONALERT);

	اگر(!NT_SUCCESS(Status))
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

		اگر((STATUS_ACCESS_DENIED == Status) && !Readonly)
		{
			اگر(ARGUMENT_PRESENT(OpenedReadonly))
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

	بازگشت Status;
}


ایستا
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

	بازگشت _OpenNtName(Buffer, ReadOnly, Handle, OpenedReadonly);
}


//
// Flush device
//

ایستا
__inline
NTSTATUS
_FlushDrive(
		IN HANDLE Handle
		)
{
	IO_STATUS_BLOCK IoStatusBlock;
	بازگشت NtFlushBuffersFile(Handle, &IoStatusBlock);
}


//
// lock drive
//

ایستا
__inline
NTSTATUS
_LockDrive(
		IN HANDLE Handle
		)
{
	IO_STATUS_BLOCK IoStatusBlock;
	بازگشت NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_LOCK_VOLUME, 0, 0, 0, 0);
}


//
// unlock drive
//

ایستا
__inline
NTSTATUS
_UnlockDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	بازگشت NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0);
}

ایستا
__inline
NTSTATUS
_DismountDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	بازگشت NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0);
}


//
// is mounted
//

ایستا
__inline
BOOLEAN
_IsMounted(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	NTSTATUS Status;
	Status = NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_IS_VOLUME_MOUNTED, 0, 0, 0, 0);
	بازگشت (BOOLEAN)(STATUS_SUCCESS == Status);
}


ایستا
__inline
NTSTATUS
_CloseDisk(
		IN HANDLE Handle
		)
{
	بازگشت NtClose(Handle);
}




//
// Make NT name from any recognized name
//

ایستا
PCSTR
_NormalizeDeviceName(
    IN PCSTR Device,
    IN PSTR NormalizedDeviceNameBuffer
   )
{
	صحیح Partition_عدد = -1;
	UCHAR Disk_عدد;
	PSTR p;


	//
	// Do not try to parse NT name
	//

	اگر('\\' == *Device)
		بازگشت Device;



	//
	// Strip leading '/dev/' if any
	//

	اگر(('/' == *(Device)) &&
		('d' == *(Device + 1)) &&
		('e' == *(Device + 2)) &&
		('v' == *(Device + 3)) &&
		('/' == *(Device + 4)))
	{
		Device += 5;
	}

	اگر('\0' == *Device)
	{
		بازگشت پوچ;
	}


	//
	// forms: hda[n], fd[n]
	//

	اگر('d' != *(Device + 1))
	{
		بازگشت پوچ;
	}

	اگر('h' == *Device)
	{
		اگر((*(Device + 2) < 'a') || (*(Device + 2) > ('a' + 9)) ||
		   ((*(Device + 3) != '\0') &&
			((*(Device + 4) != '\0') ||
			 ((*(Device + 3) < '0') || (*(Device + 3) > '9'))
			)
		   )
		  )
		{
			بازگشت پوچ;
		}

		Disk_عدد = (UCHAR)(*(Device + 2) - 'a');

		اگر(*(Device + 3) != '\0')
		{
			Partition_عدد = (*(Device + 3) - '0');
		}

	}
	وگرنه اگر('f' == *Device)
	{
		//
		// 3-d letter should be a digit.
		//

		اگر((*(Device + 3) != '\0') ||
		   (*(Device + 2) < '0') || (*(Device + 2) > '9'))
		{
			بازگشت پوچ;
		}

		Disk_عدد = (UCHAR)(*(Device + 2) - '0');

	}
	وگرنه
	{
		//
		// invalid prefix
		//

		بازگشت پوچ;
	}



	//
	// Prefix
	//

	strcpy(NormalizedDeviceNameBuffer, "\\Device\\");

	//
	// Media name
	//

	انتخاب(*Device)
	{

	حالت 'f':
		strcat(NormalizedDeviceNameBuffer, "Floppy0");
		بشکن;

	حالت 'h':
		strcat(NormalizedDeviceNameBuffer, "Harddisk0");
		بشکن;
	}


	p = NormalizedDeviceNameBuffer + strlen(NormalizedDeviceNameBuffer) - 1;
	*p = (CHAR)(*p + Disk_عدد);


	//
	// Partition nr.
	//

	اگر(Partition_عدد >= 0)
	{
		strcat(NormalizedDeviceNameBuffer, "\\Partition0");

		p = NormalizedDeviceNameBuffer + strlen(NormalizedDeviceNameBuffer) - 1;
		*p = (CHAR)(*p + Partition_عدد);
	}


	بازگشت NormalizedDeviceNameBuffer;
}




ایستا
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

	RtlZeroMemory(&pi, اندازه(PARTITION_INFORMATION));

	Status = NtDeviceIoControlFile(
		h, پوچ, پوچ, پوچ, &IoStatusBlock, IOCTL_DISK_GET_PARTITION_INFO,
		&pi, اندازه(PARTITION_INFORMATION),
		&pi, اندازه(PARTITION_INFORMATION));


	اگر(NT_SUCCESS(Status))
	{
		*FsSize = pi.PartitionLength.QuadPart;
	}
	وگرنه اگر(STATUS_INVALID_DEVICE_REQUEST == Status)
	{
		//
		// No partitions: get device info.
		//

		RtlZeroMemory(&gi, اندازه(DISK_GEOMETRY));

		Status = NtDeviceIoControlFile(
				h, پوچ, پوچ, پوچ, &IoStatusBlock, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, اندازه(DISK_GEOMETRY),
				&gi, اندازه(DISK_GEOMETRY));


		اگر(NT_SUCCESS(Status))
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

ایستا
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

	اگر(پوچ == Name)
	{
		//
		// Set not found
		//

		اگر(ARGUMENT_PRESENT(Errno))
			*Errno = ENOENT;

		بازگشت FALSE;
	}


	اگر((((*Name) | 0x20) >= 'a') && (((*Name) | 0x20) <= 'z') &&
		(':' == *(Name + 1)) && ('\0' == *(Name + 2)))
	{
		Status = _OpenDriveLetter(*Name, ReadOnly, Handle, OpenedReadonly);
	}
	وگرنه
	{
		//
		// Make name
		//

		Name = _NormalizeDeviceName(Name, NormalizedDeviceName);

		اگر(پوچ == Name)
		{
			//
			// Set not found
			//

			اگر(ARGUMENT_PRESENT(Errno))
				*Errno = ENOENT;

			بازگشت FALSE;
		}

		//
		// Try to open it
		//

		Status = _OpenNtName(Name, ReadOnly, Handle, OpenedReadonly);
	}


	اگر(!NT_SUCCESS(Status))
	{
		اگر(ARGUMENT_PRESENT(Errno))
			*Errno = _MapNtStatus(Status);

		بازگشت FALSE;
	}

	بازگشت TRUE;
}


//
// Raw block io. Sets dos errno
//

ایستا
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
	ASSERT(0 == (Offset.کم_Part % 512));


	//
	// perform io
	//

	اگر(Read)
	{
		Status = NtReadFile(Handle, پوچ, پوچ, پوچ,
			&IoStatusBlock, Buffer, Bytes, &Offset, پوچ);
	}
	وگرنه
	{
		Status = NtWriteFile(Handle, پوچ, پوچ, پوچ,
			&IoStatusBlock, Buffer, Bytes, &Offset, پوچ);
	}


	//
	// translate error
	//

	اگر(NT_SUCCESS(Status))
	{
		*Errno = 0;
		بازگشت TRUE;
	}

	*Errno = _MapNtStatus(Status);

	بازگشت FALSE;
}



__inline
BOOLEAN
_RawWrite(
    IN HANDLE Handle,
    IN LARGE_INTEGER Offset,
    IN ULONG Bytes,
    OUT ثابت CHAR* Buffer,
    OUT unsigned* Errno
   )
{
	بازگشت _BlockIo(Handle, Offset, Bytes, (PCHAR)Buffer, FALSE, Errno);
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
	بازگشت _BlockIo(Handle, Offset, Bytes, Buffer, TRUE, Errno);
}



__inline
BOOLEAN
_SetPartType(
    IN HANDLE Handle,
    IN UCHAR Type
   )
{
	IO_STATUS_BLOCK IoStatusBlock;
	بازگشت STATUS_SUCCESS == NtDeviceIoControlFile(
												   Handle, پوچ, پوچ, پوچ, &IoStatusBlock, IOCTL_DISK_SET_PARTITION_INFO,
												   &Type, اندازه(Type),
												   پوچ, 0);
}



//--------------------- interface part

//
// Interface functions.
// Is_mounted is set to 1 if the device is mounted, 0 otherwise
//

errcode_t
ext2fs_check_if_mounted(ثابت نویسه *file, صحیح *mount_flags)
{
	HANDLE h;
	BOOLEAN Readonly;

	*mount_flags = 0;

	اگر(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, پوچ))
	{
		بازگشت 0;
	}


	__try{
		*mount_flags &= _IsMounted(h) ? EXT2_MF_MOUNTED : 0;
	}
	__finally{
		_CloseDisk(h);
	}

	بازگشت 0;
}



//
// Returns the number of blocks in a partition
//

ایستا __int64 FsSize = 0;
ایستا نویسه knowndevice[1024] = "";


errcode_t
ext2fs_get_device_size(ثابت نویسه *file, صحیح blocksize,
				 blk_t *retblocks)
{
	HANDLE h;
	BOOLEAN Readonly;

	اگر((0 == FsSize) || (0 != strcmp(knowndevice, file)))
	{

		اگر(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, پوچ))
		{
			بازگشت 0;
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
	بازگشت 0;
}






//
// Table elements
//


ایستا
errcode_t
nt_open(ثابت نویسه *name, صحیح flags, io_channel *channel)
{
	io_channel      io = پوچ;
	PNT_PRIVATE_DATA NtData = پوچ;
	errcode_t Errno = 0;

	//
	// Check name
	//

	اگر (پوچ == name)
	{
		بازگشت EXT2_ET_BAD_DEVICE_NAME;
	}

	__try{

		//
		// Allocate channel handle
		//

		io = (io_channel) malloc(اندازه(ساختار struct_io_channel));

		اگر (پوچ == io)
		{
			Errno = ENOMEM;
			__leave;
		}

		RtlZeroMemory(io, اندازه(ساختار struct_io_channel));
		io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

		NtData = (PNT_PRIVATE_DATA)malloc(اندازه(NT_PRIVATE_DATA));

		اگر (پوچ == NtData)
		{
			Errno = ENOMEM;
			__leave;
		}


		io->manager = nt_io_manager();
		io->name = malloc(strlen(name) + 1);
		اگر (پوچ == io->name)
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

		RtlZeroMemory(NtData, اندازه(NT_PRIVATE_DATA));

		NtData->magic = EXT2_ET_MAGIC_NT_IO_CHANNEL;
		NtData->Buffer_Block_عدد = 0xffffffff;
		NtData->BufferSize = 1024;
		NtData->Buffer = malloc(NtData->BufferSize);

		اگر (پوچ == NtData->Buffer)
		{
			Errno = ENOMEM;
			__leave;
		}

		//
		// Open it
		//

		اگر(!_Ext2OpenDevice(name, (BOOLEAN)!BooleanFlagOn(flags, EXT2_FLAG_RW), &NtData->Handle, &NtData->OpenedReadonly, &Errno))
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

		اگر(!NT_SUCCESS(_LockDrive(NtData->Handle)) /*|| !NT_SUCCESS(_DismountDrive(NtData->Handle))*/)
		{
			NtData->OpenedReadonly = TRUE;
		}

		//
		// Done
		//

		*channel = io;


	}
	__finally{

		اگر(0 != Errno)
		{
			//
			// Cleanup
			//

			اگر (پوچ != io)
			{
				آزاد(io->name);
				آزاد(io);
			}

			اگر (پوچ != NtData)
			{
				اگر(پوچ != NtData->Handle)
				{
					_UnlockDrive(NtData->Handle);
					_CloseDisk(NtData->Handle);
				}

				آزاد(NtData->Buffer);
				آزاد(NtData);
			}
		}
	}

	بازگشت Errno;
}


//
// Close api
//

ایستا
errcode_t
nt_close(io_channel channel)
{
	PNT_PRIVATE_DATA NtData = پوچ;

	اگر(پوچ == channel)
	{
		بازگشت 0;
	}

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	اگر (--channel->refcount > 0)
	{
		بازگشت 0;
	}

	آزاد(channel->name);
	آزاد(channel);

	اگر (پوچ != NtData)
	{
		اگر(پوچ != NtData->Handle)
		{
			_DismountDrive(NtData->Handle);
			_UnlockDrive(NtData->Handle);
			_CloseDisk(NtData->Handle);
		}

		آزاد(NtData->Buffer);
		آزاد(NtData);
	}

	بازگشت 0;
}



//
// set block size
//

ایستا
errcode_t
nt_set_blksize(io_channel channel, صحیح blksize)
{
	PNT_PRIVATE_DATA NtData = پوچ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	اگر (channel->block_size != blksize)
	{
		channel->block_size = blksize;

		آزاد(NtData->Buffer);
		NtData->Buffer_Block_عدد = 0xffffffff;
		NtData->BufferSize = channel->block_size;
		ASSERT(0 == (NtData->BufferSize % 512));

		NtData->Buffer = malloc(NtData->BufferSize);

		اگر (پوچ == NtData->Buffer)
		{
			بازگشت ENOMEM;
		}

	}

	بازگشت 0;
}


//
// read block
//

ایستا
errcode_t
nt_read_blk(io_channel channel, unsigned long block,
			       صحیح شمار, تهی *buf)
{
	PVOID BufferToRead;
	ULONG SizeToRead;
	ULONG Size;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_DATA NtData = پوچ;
	unsigned Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	//
	// If it's in the cache, use it!
	//

	اگر ((1 == شمار) &&
		(block == NtData->Buffer_Block_عدد) &&
		(NtData->Buffer_Block_عدد != 0xffffffff))
	{
		memcpy(buf, NtData->Buffer, channel->block_size);
		بازگشت 0;
	}

	Size = (شمار < 0) ? (ULONG)(-شمار) : (ULONG)(شمار * channel->block_size);

	Offset.QuadPart = block * channel->block_size;

	//
	// If not fit to the block
	//

	اگر(Size <= NtData->BufferSize)
	{
		//
		// Update the cache
		//

		NtData->Buffer_Block_عدد = block;
		BufferToRead = NtData->Buffer;
		SizeToRead = NtData->BufferSize;
	}
	وگرنه
	{
		SizeToRead = Size;
		BufferToRead = buf;
		ASSERT(0 == (SizeToRead % channel->block_size));
	}

	اگر(!_RawRead(NtData->Handle, Offset, SizeToRead, BufferToRead, &Errno))
	{

		اگر (channel->read_error)
		{
			بازگشت (channel->read_error)(channel, block, شمار, buf,
					       Size, 0, Errno);
		}
		وگرنه
		{
			بازگشت Errno;
		}
	}


	اگر(BufferToRead != buf)
	{
		ASSERT(Size <= SizeToRead);
		memcpy(buf, BufferToRead, Size);
	}

	بازگشت 0;
}


//
// write block
//

ایستا
errcode_t
nt_write_blk(io_channel channel, unsigned long block,
				صحیح شمار, ثابت تهی *buf)
{
	ULONG SizeToWrite;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_DATA NtData = پوچ;
	unsigned Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	اگر(NtData->OpenedReadonly)
	{
		بازگشت EACCES;
	}

	اگر (شمار == 1)
	{
		SizeToWrite = channel->block_size;
	}
	وگرنه
	{
		NtData->Buffer_Block_عدد = 0xffffffff;

		اگر (شمار < 0)
		{
			SizeToWrite = (ULONG)(-شمار);
		}
		وگرنه
		{
			SizeToWrite = (ULONG)(شمار * channel->block_size);
		}
	}


	ASSERT(0 == (SizeToWrite % 512));
	Offset.QuadPart = block * channel->block_size;

	اگر(!_RawWrite(NtData->Handle, Offset, SizeToWrite, buf, &Errno))
	{
		اگر (channel->write_error)
		{
			بازگشت (channel->write_error)(channel, block, شمار, buf,
						SizeToWrite, 0, Errno);
		}
		وگرنه
		{
			بازگشت Errno;
		}
	}


	//
	// Stash a copy.
	//

	اگر(SizeToWrite >= NtData->BufferSize)
	{
		NtData->Buffer_Block_عدد = block;
		memcpy(NtData->Buffer, buf, NtData->BufferSize);
	}

	NtData->Written = TRUE;

	بازگشت 0;

}



//
// Flush data buffers to disk.  Since we are currently using a
// write-through cache, this is a no-op.
//

ایستا
errcode_t
nt_flush(io_channel channel)
{
	PNT_PRIVATE_DATA NtData = پوچ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	اگر(NtData->OpenedReadonly)
	{
		بازگشت 0; // EACCESS;
	}


	//
	// Flush file buffers.
	//

	_FlushDrive(NtData->Handle);


	//
	// Test and correct partition type.
	//

	اگر(NtData->Written)
	{
		_SetPartType(NtData->Handle, 0x83);
	}

	بازگشت 0;
}


