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
#ضمن "config.h"
#endif


//
// I need some warnings to disable...
//


#pragma warning(disable:4514) // unreferenced inline function has been removed
#pragma warning(push,4)

#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union)
#pragma warning(disable:4214) // nonstandard extension used : bit field types other than int
#pragma warning(disable:4115) // named type definition in parentheses

#ضمن <ntddk.h>
#ضمن <ntdddisk.h>
#ضمن <ntstatus.h>

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

#ضمن <winerror.h>

//
// standard stuff
//

#ضمن <assert.h>
#ضمن <stdio.h>
#ضمن <string.h>
#ضمن <stdlib.h>
#ضمن <malloc.h>

#ضمن <linux/types.h>
#ضمن "ext2_fs.h"
#ضمن <errno.h>

#ضمن "et/com_err.h"
#ضمن "ext2fs/ext2fs.h"
#ضمن "ext2fs/ext2_err.h"




//
// For checking structure magic numbers...
//


#define EXT2_CHECK_MAGIC(بنية, code) \
	  𐤀𐤌 ((بنية)->magic != (code)) أرجع (code)

#define EXT2_ET_MAGIC_NT_IO_CHANNEL  0x10ed


//
// Private data block
//

تعريفنوع بنية _NT_PRIVATE_DATA {
	صحيح	   magic;
	HANDLE Handle;
	صحيح	   Flags;
	PCHAR  Buffer;
	__u32  BufferBlockNumber;
	ULONG  BufferSize;
	BOOLEAN OpenedReadonly;
	BOOLEAN Written;
}NT_PRIVATE_DATA, *PNT_PRIVATE_DATA;



//
// Standard interface prototypes
//

ساكن errcode_t nt_open(ثابت حرف *name, صحيح flags, io_channel *channel);
ساكن errcode_t nt_close(io_channel channel);
ساكن errcode_t nt_set_blksize(io_channel channel, صحيح blksize);
ساكن errcode_t nt_read_blk(io_channel channel, unsigned long block,
			       صحيح count, فراغ *data);
ساكن errcode_t nt_write_blk(io_channel channel, unsigned long block,
				صحيح count, ثابت فراغ *data);
ساكن errcode_t nt_flush(io_channel channel);

ساكن بنية struct_io_manager struct_nt_manager = {
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
	أرجع &struct_nt_manager;
}





//
// This is a code to convert Win32 errors to unix errno
//

تعريفنوع بنية {
	ULONG WinError;
	صحيح errnocode;
}ERROR_ENTRY;

ساكن ERROR_ENTRY ErrorTable[] = {
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




ساكن
unsigned
_MapDosError (
    IN ULONG WinError
   )
{
	صحيح i;

	//
	// Lookup
	//

	لكل (i = 0; i < (حجم(ErrorTable)/حجم(ErrorTable[0])); ++i)
	{
		𐤀𐤌 (WinError == ErrorTable[i].WinError)
		{
			أرجع ErrorTable[i].errnocode;
		}
	}

	//
	// not in table. Check ranges
	//

	𐤀𐤌 ((WinError >= ERROR_WRITE_PROTECT) &&
		(WinError <= ERROR_SHARING_BUFFER_EXCEEDED))
	{
		أرجع EACCES;
	}
	وإلا 𐤀𐤌 ((WinError >= ERROR_INVALID_STARTING_CODESEG) &&
			 (WinError <= ERROR_INFLOOP_IN_RELOC_CHAIN))
	{
		أرجع ENOEXEC;
	}
	وإلا
	{
		أرجع EINVAL;
	}
}







//
// Function to map NT status to dos error.
//

ساكن
__inline
unsigned
_MapNtStatus(
    IN NTSTATUS Status
   )
{
	أرجع _MapDosError(RtlNtStatusToDosError(Status));
}





//
// Helper functions to make things easier
//

ساكن
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
	UnicodeString.MaximumLength = حجم(Buffer); // in bytes!!!

	RtlInitAnsiString(&AnsiString, Name);

	Status = RtlAnsiStringToUnicodeString(&UnicodeString, &AnsiString, FALSE);

	𐤀𐤌(!NT_SUCCESS(Status))
	{
		أرجع Status; // Unmappable character?
	}

	//
	// Initialize object
	//

	InitializeObjectAttributes(&ObjectAttributes,
							   &UnicodeString,
							   OBJ_CASE_INSENSITIVE,
							   عدم,
							   عدم );

	//
	// Try to open it in initial mode
	//

	𐤀𐤌(ARGUMENT_PRESENT(OpenedReadonly))
	{
		*OpenedReadonly = Readonly;
	}


	Status = NtOpenFile(Handle,
						SYNCHRONIZE | FILE_READ_DATA | (Readonly ? 0 : FILE_WRITE_DATA),
						&ObjectAttributes,
						&IoStatusBlock,
						FILE_SHARE_WRITE | FILE_SHARE_READ,
						FILE_SYNCHRONOUS_IO_NONALERT);

	𐤀𐤌(!NT_SUCCESS(Status))
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

		𐤀𐤌((STATUS_ACCESS_DENIED == Status) && !Readonly)
		{
			𐤀𐤌(ARGUMENT_PRESENT(OpenedReadonly))
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

	أرجع Status;
}


ساكن
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

	أرجع _OpenNtName(Buffer, ReadOnly, Handle, OpenedReadonly);
}


//
// Flush device
//

ساكن
__inline
NTSTATUS
_FlushDrive(
		IN HANDLE Handle
		)
{
	IO_STATUS_BLOCK IoStatusBlock;
	أرجع NtFlushBuffersFile(Handle, &IoStatusBlock);
}


//
// lock drive
//

ساكن
__inline
NTSTATUS
_LockDrive(
		IN HANDLE Handle
		)
{
	IO_STATUS_BLOCK IoStatusBlock;
	أرجع NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_LOCK_VOLUME, 0, 0, 0, 0);
}


//
// unlock drive
//

ساكن
__inline
NTSTATUS
_UnlockDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	أرجع NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0);
}

ساكن
__inline
NTSTATUS
_DismountDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	أرجع NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0);
}


//
// is mounted
//

ساكن
__inline
BOOLEAN
_IsMounted(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	NTSTATUS Status;
	Status = NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_IS_VOLUME_MOUNTED, 0, 0, 0, 0);
	أرجع (BOOLEAN)(STATUS_SUCCESS == Status);
}


ساكن
__inline
NTSTATUS
_CloseDisk(
		IN HANDLE Handle
		)
{
	أرجع NtClose(Handle);
}




//
// Make NT name from any recognized name
//

ساكن
PCSTR
_NormalizeDeviceName(
    IN PCSTR Device,
    IN PSTR NormalizedDeviceNameBuffer
   )
{
	صحيح PartitionNumber = -1;
	UCHAR DiskNumber;
	PSTR p;


	//
	// Do not try to parse NT name
	//

	𐤀𐤌('\\' == *Device)
		أرجع Device;



	//
	// Strip leading '/dev/' if any
	//

	𐤀𐤌(('/' == *(Device)) &&
		('d' == *(Device + 1)) &&
		('e' == *(Device + 2)) &&
		('v' == *(Device + 3)) &&
		('/' == *(Device + 4)))
	{
		Device += 5;
	}

	𐤀𐤌('\0' == *Device)
	{
		أرجع عدم;
	}


	//
	// forms: hda[n], fd[n]
	//

	𐤀𐤌('d' != *(Device + 1))
	{
		أرجع عدم;
	}

	𐤀𐤌('h' == *Device)
	{
		𐤀𐤌((*(Device + 2) < 'a') || (*(Device + 2) > ('a' + 9)) ||
		   ((*(Device + 3) != '\0') &&
			((*(Device + 4) != '\0') ||
			 ((*(Device + 3) < '0') || (*(Device + 3) > '9'))
			)
		   )
		  )
		{
			أرجع عدم;
		}

		DiskNumber = (UCHAR)(*(Device + 2) - 'a');

		𐤀𐤌(*(Device + 3) != '\0')
		{
			PartitionNumber = (*(Device + 3) - '0');
		}

	}
	وإلا 𐤀𐤌('f' == *Device)
	{
		//
		// 3-d letter should be a digit.
		//

		𐤀𐤌((*(Device + 3) != '\0') ||
		   (*(Device + 2) < '0') || (*(Device + 2) > '9'))
		{
			أرجع عدم;
		}

		DiskNumber = (UCHAR)(*(Device + 2) - '0');

	}
	وإلا
	{
		//
		// invalid prefix
		//

		أرجع عدم;
	}



	//
	// Prefix
	//

	strcpy(NormalizedDeviceNameBuffer, "\\Device\\");

	//
	// Media name
	//

	بدل(*Device)
	{

	حالة 'f':
		strcat(NormalizedDeviceNameBuffer, "Floppy0");
		اكسر;

	حالة 'h':
		strcat(NormalizedDeviceNameBuffer, "Harddisk0");
		اكسر;
	}


	p = NormalizedDeviceNameBuffer + strlen(NormalizedDeviceNameBuffer) - 1;
	*p = (CHAR)(*p + DiskNumber);


	//
	// Partition nr.
	//

	𐤀𐤌(PartitionNumber >= 0)
	{
		strcat(NormalizedDeviceNameBuffer, "\\Partition0");

		p = NormalizedDeviceNameBuffer + strlen(NormalizedDeviceNameBuffer) - 1;
		*p = (CHAR)(*p + PartitionNumber);
	}


	أرجع NormalizedDeviceNameBuffer;
}




ساكن
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

	RtlZeroMemory(&pi, حجم(PARTITION_INFORMATION));

	Status = NtDeviceIoControlFile(
		h, عدم, عدم, عدم, &IoStatusBlock, IOCTL_DISK_GET_PARTITION_INFO,
		&pi, حجم(PARTITION_INFORMATION),
		&pi, حجم(PARTITION_INFORMATION));


	𐤀𐤌(NT_SUCCESS(Status))
	{
		*FsSize = pi.PartitionLength.QuadPart;
	}
	وإلا 𐤀𐤌(STATUS_INVALID_DEVICE_REQUEST == Status)
	{
		//
		// No partitions: get device info.
		//

		RtlZeroMemory(&gi, حجم(DISK_GEOMETRY));

		Status = NtDeviceIoControlFile(
				h, عدم, عدم, عدم, &IoStatusBlock, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, حجم(DISK_GEOMETRY),
				&gi, حجم(DISK_GEOMETRY));


		𐤀𐤌(NT_SUCCESS(Status))
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

ساكن
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

	𐤀𐤌(عدم == Name)
	{
		//
		// Set not found
		//

		𐤀𐤌(ARGUMENT_PRESENT(Errno))
			*Errno = ENOENT;

		أرجع FALSE;
	}


	𐤀𐤌((((*Name) | 0x20) >= 'a') && (((*Name) | 0x20) <= 'z') &&
		(':' == *(Name + 1)) && ('\0' == *(Name + 2)))
	{
		Status = _OpenDriveLetter(*Name, ReadOnly, Handle, OpenedReadonly);
	}
	وإلا
	{
		//
		// Make name
		//

		Name = _NormalizeDeviceName(Name, NormalizedDeviceName);

		𐤀𐤌(عدم == Name)
		{
			//
			// Set not found
			//

			𐤀𐤌(ARGUMENT_PRESENT(Errno))
				*Errno = ENOENT;

			أرجع FALSE;
		}

		//
		// Try to open it
		//

		Status = _OpenNtName(Name, ReadOnly, Handle, OpenedReadonly);
	}


	𐤀𐤌(!NT_SUCCESS(Status))
	{
		𐤀𐤌(ARGUMENT_PRESENT(Errno))
			*Errno = _MapNtStatus(Status);

		أرجع FALSE;
	}

	أرجع TRUE;
}


//
// Raw block io. Sets dos errno
//

ساكن
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

	𐤀𐤌(Read)
	{
		Status = NtReadFile(Handle, عدم, عدم, عدم,
			&IoStatusBlock, Buffer, Bytes, &Offset, عدم);
	}
	وإلا
	{
		Status = NtWriteFile(Handle, عدم, عدم, عدم,
			&IoStatusBlock, Buffer, Bytes, &Offset, عدم);
	}


	//
	// translate error
	//

	𐤀𐤌(NT_SUCCESS(Status))
	{
		*Errno = 0;
		أرجع TRUE;
	}

	*Errno = _MapNtStatus(Status);

	أرجع FALSE;
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
	أرجع _BlockIo(Handle, Offset, Bytes, (PCHAR)Buffer, FALSE, Errno);
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
	أرجع _BlockIo(Handle, Offset, Bytes, Buffer, TRUE, Errno);
}



__inline
BOOLEAN
_SetPartType(
    IN HANDLE Handle,
    IN UCHAR Type
   )
{
	IO_STATUS_BLOCK IoStatusBlock;
	أرجع STATUS_SUCCESS == NtDeviceIoControlFile(
												   Handle, عدم, عدم, عدم, &IoStatusBlock, IOCTL_DISK_SET_PARTITION_INFO,
												   &Type, حجم(Type),
												   عدم, 0);
}



//--------------------- interface part

//
// Interface functions.
// Is_mounted is set to 1 if the device is mounted, 0 otherwise
//

errcode_t
ext2fs_check_if_mounted(ثابت حرف *file, صحيح *mount_flags)
{
	HANDLE h;
	BOOLEAN Readonly;

	*mount_flags = 0;

	𐤀𐤌(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, عدم))
	{
		أرجع 0;
	}


	__try{
		*mount_flags &= _IsMounted(h) ? EXT2_MF_MOUNTED : 0;
	}
	__finally{
		_CloseDisk(h);
	}

	أرجع 0;
}



//
// Returns the number of blocks in a partition
//

ساكن __int64 FsSize = 0;
ساكن حرف knowndevice[1024] = "";


errcode_t
ext2fs_get_device_size(ثابت حرف *file, صحيح blocksize,
				 blk_t *retblocks)
{
	HANDLE h;
	BOOLEAN Readonly;

	𐤀𐤌((0 == FsSize) || (0 != strcmp(knowndevice, file)))
	{

		𐤀𐤌(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, عدم))
		{
			أرجع 0;
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
	أرجع 0;
}






//
// Table elements
//


ساكن
errcode_t
nt_open(ثابت حرف *name, صحيح flags, io_channel *channel)
{
	io_channel      io = عدم;
	PNT_PRIVATE_DATA NtData = عدم;
	errcode_t Errno = 0;

	//
	// Check name
	//

	𐤀𐤌 (عدم == name)
	{
		أرجع EXT2_ET_BAD_DEVICE_NAME;
	}

	__try{

		//
		// Allocate channel handle
		//

		io = (io_channel) malloc(حجم(بنية struct_io_channel));

		𐤀𐤌 (عدم == io)
		{
			Errno = ENOMEM;
			__leave;
		}

		RtlZeroMemory(io, حجم(بنية struct_io_channel));
		io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

		NtData = (PNT_PRIVATE_DATA)malloc(حجم(NT_PRIVATE_DATA));

		𐤀𐤌 (عدم == NtData)
		{
			Errno = ENOMEM;
			__leave;
		}


		io->manager = nt_io_manager();
		io->name = malloc(strlen(name) + 1);
		𐤀𐤌 (عدم == io->name)
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

		RtlZeroMemory(NtData, حجم(NT_PRIVATE_DATA));

		NtData->magic = EXT2_ET_MAGIC_NT_IO_CHANNEL;
		NtData->BufferBlockNumber = 0xffffffff;
		NtData->BufferSize = 1024;
		NtData->Buffer = malloc(NtData->BufferSize);

		𐤀𐤌 (عدم == NtData->Buffer)
		{
			Errno = ENOMEM;
			__leave;
		}

		//
		// Open it
		//

		𐤀𐤌(!_Ext2OpenDevice(name, (BOOLEAN)!BooleanFlagOn(flags, EXT2_FLAG_RW), &NtData->Handle, &NtData->OpenedReadonly, &Errno))
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

		𐤀𐤌(!NT_SUCCESS(_LockDrive(NtData->Handle)) /*|| !NT_SUCCESS(_DismountDrive(NtData->Handle))*/)
		{
			NtData->OpenedReadonly = TRUE;
		}

		//
		// Done
		//

		*channel = io;


	}
	__finally{

		𐤀𐤌(0 != Errno)
		{
			//
			// Cleanup
			//

			𐤀𐤌 (عدم != io)
			{
				حرر(io->name);
				حرر(io);
			}

			𐤀𐤌 (عدم != NtData)
			{
				𐤀𐤌(عدم != NtData->Handle)
				{
					_UnlockDrive(NtData->Handle);
					_CloseDisk(NtData->Handle);
				}

				حرر(NtData->Buffer);
				حرر(NtData);
			}
		}
	}

	أرجع Errno;
}


//
// Close api
//

ساكن
errcode_t
nt_close(io_channel channel)
{
	PNT_PRIVATE_DATA NtData = عدم;

	𐤀𐤌(عدم == channel)
	{
		أرجع 0;
	}

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	𐤀𐤌 (--channel->refcount > 0)
	{
		أرجع 0;
	}

	حرر(channel->name);
	حرر(channel);

	𐤀𐤌 (عدم != NtData)
	{
		𐤀𐤌(عدم != NtData->Handle)
		{
			_DismountDrive(NtData->Handle);
			_UnlockDrive(NtData->Handle);
			_CloseDisk(NtData->Handle);
		}

		حرر(NtData->Buffer);
		حرر(NtData);
	}

	أرجع 0;
}



//
// set block size
//

ساكن
errcode_t
nt_set_blksize(io_channel channel, صحيح blksize)
{
	PNT_PRIVATE_DATA NtData = عدم;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	𐤀𐤌 (channel->block_size != blksize)
	{
		channel->block_size = blksize;

		حرر(NtData->Buffer);
		NtData->BufferBlockNumber = 0xffffffff;
		NtData->BufferSize = channel->block_size;
		ASSERT(0 == (NtData->BufferSize % 512));

		NtData->Buffer = malloc(NtData->BufferSize);

		𐤀𐤌 (عدم == NtData->Buffer)
		{
			أرجع ENOMEM;
		}

	}

	أرجع 0;
}


//
// read block
//

ساكن
errcode_t
nt_read_blk(io_channel channel, unsigned long block,
			       صحيح count, فراغ *buf)
{
	PVOID BufferToRead;
	ULONG SizeToRead;
	ULONG Size;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_DATA NtData = عدم;
	unsigned Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	//
	// If it's in the cache, use it!
	//

	𐤀𐤌 ((1 == count) &&
		(block == NtData->BufferBlockNumber) &&
		(NtData->BufferBlockNumber != 0xffffffff))
	{
		memcpy(buf, NtData->Buffer, channel->block_size);
		أرجع 0;
	}

	Size = (count < 0) ? (ULONG)(-count) : (ULONG)(count * channel->block_size);

	Offset.QuadPart = block * channel->block_size;

	//
	// If not fit to the block
	//

	𐤀𐤌(Size <= NtData->BufferSize)
	{
		//
		// Update the cache
		//

		NtData->BufferBlockNumber = block;
		BufferToRead = NtData->Buffer;
		SizeToRead = NtData->BufferSize;
	}
	وإلا
	{
		SizeToRead = Size;
		BufferToRead = buf;
		ASSERT(0 == (SizeToRead % channel->block_size));
	}

	𐤀𐤌(!_RawRead(NtData->Handle, Offset, SizeToRead, BufferToRead, &Errno))
	{

		𐤀𐤌 (channel->read_error)
		{
			أرجع (channel->read_error)(channel, block, count, buf,
					       Size, 0, Errno);
		}
		وإلا
		{
			أرجع Errno;
		}
	}


	𐤀𐤌(BufferToRead != buf)
	{
		ASSERT(Size <= SizeToRead);
		memcpy(buf, BufferToRead, Size);
	}

	أرجع 0;
}


//
// write block
//

ساكن
errcode_t
nt_write_blk(io_channel channel, unsigned long block,
				صحيح count, ثابت فراغ *buf)
{
	ULONG SizeToWrite;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_DATA NtData = عدم;
	unsigned Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	𐤀𐤌(NtData->OpenedReadonly)
	{
		أرجع EACCES;
	}

	𐤀𐤌 (count == 1)
	{
		SizeToWrite = channel->block_size;
	}
	وإلا
	{
		NtData->BufferBlockNumber = 0xffffffff;

		𐤀𐤌 (count < 0)
		{
			SizeToWrite = (ULONG)(-count);
		}
		وإلا
		{
			SizeToWrite = (ULONG)(count * channel->block_size);
		}
	}


	ASSERT(0 == (SizeToWrite % 512));
	Offset.QuadPart = block * channel->block_size;

	𐤀𐤌(!_RawWrite(NtData->Handle, Offset, SizeToWrite, buf, &Errno))
	{
		𐤀𐤌 (channel->write_error)
		{
			أرجع (channel->write_error)(channel, block, count, buf,
						SizeToWrite, 0, Errno);
		}
		وإلا
		{
			أرجع Errno;
		}
	}


	//
	// Stash a copy.
	//

	𐤀𐤌(SizeToWrite >= NtData->BufferSize)
	{
		NtData->BufferBlockNumber = block;
		memcpy(NtData->Buffer, buf, NtData->BufferSize);
	}

	NtData->Written = TRUE;

	أرجع 0;

}



//
// Flush data buffers to disk.  Since we are currently using a
// write-through cache, this is a no-op.
//

ساكن
errcode_t
nt_flush(io_channel channel)
{
	PNT_PRIVATE_DATA NtData = عدم;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	𐤀𐤌(NtData->OpenedReadonly)
	{
		أرجع 0; // EACCESS;
	}


	//
	// Flush file buffers.
	//

	_FlushDrive(NtData->Handle);


	//
	// Test and correct partition type.
	//

	𐤀𐤌(NtData->Written)
	{
		_SetPartType(NtData->Handle, 0x83);
	}

	أرجع 0;
}


