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
#અન્તર્ભાવ "config.h"
#endif


//
// I need some warnings to disable...
//


#pragma warning(disable:4514) // unreferenced inline function has been removed
#pragma warning(ધકેલો,4)

#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union)
#pragma warning(disable:4214) // nonstandard extension used : bit field types other than int
#pragma warning(disable:4115) // named type definition in parentheses

#અન્તર્ભાવ <ntddk.h>
#અન્તર્ભાવ <ntdddisk.h>
#અન્તર્ભાવ <ntstatus.h>

#pragma warning(કાઢો)


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
    IN PULONG ચાવી_2 OPTIONAL
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
    IN PULONG ચાવી_2 OPTIONAL
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
    IN PVOID આગત_Buffer OPTIONAL,
    IN ULONG આગત_Buffer_Length,
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
    IN PVOID આગત_Buffer OPTIONAL,
    IN ULONG આગત_Buffer_Length,
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

#અન્તર્ભાવ <winerror.h>

//
// standard stuff
//

#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <malloc.h>

#અન્તર્ભાવ <linux/types.h>
#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ <errno.h>

#અન્તર્ભાવ "et/com_err.h"
#અન્તર્ભાવ "ext2fs/ext2fs.h"
#અન્તર્ભાવ "ext2fs/ext2_err.h"




//
// For checking structure magic numbers...
//


#define EXT2_CHECK_MAGIC(સંરચના, code) \
	  જો ((સંરચના)->magic != (code)) પાછા (code)

#define EXT2_ET_MAGIC_NT_IO_CHANNEL  0x10ed


//
// Private data block
//

પ્રકારવ્યાખ્યા સંરચના _NT_PRIVATE_DATA {
	પૂર્ણાંક	   magic;
	HANDLE Handle;
	પૂર્ણાંક	   Flags;
	PCHAR  Buffer;
	__u32  Buffer_Block_સંખ્યા;
	ULONG  BufferSize;
	BOOLEAN OpenedReadonly;
	BOOLEAN Written;
}NT_PRIVATE_DATA, *PNT_PRIVATE_DATA;



//
// Standard interface prototypes
//

સ્થિર errcode_t nt_open(અચલ અક્ષર *name, પૂર્ણાંક flags, io_channel *channel);
સ્થિર errcode_t nt_close(io_channel channel);
સ્થિર errcode_t nt_set_blksize(io_channel channel, પૂર્ણાંક blksize);
સ્થિર errcode_t nt_read_blk(io_channel channel, unsigned long block,
			       પૂર્ણાંક ગણતરી, શૂન્ય *data);
સ્થિર errcode_t nt_write_blk(io_channel channel, unsigned long block,
				પૂર્ણાંક ગણતરી, અચલ શૂન્ય *data);
સ્થિર errcode_t nt_flush(io_channel channel);

સ્થિર સંરચના struct_io_manager struct_nt_manager = {
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
	પાછા &struct_nt_manager;
}





//
// This is a code to convert Win32 errors to unix errno
//

પ્રકારવ્યાખ્યા સંરચના {
	ULONG WinError;
	પૂર્ણાંક errnocode;
}ERROR_ENTRY;

સ્થિર ERROR_ENTRY ErrorTable[] = {
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




સ્થિર
unsigned
_MapDosError (
    IN ULONG WinError
   )
{
	પૂર્ણાંક i;

	//
	// Lookup
	//

	માટે (i = 0; i < (કદ(ErrorTable)/કદ(ErrorTable[0])); ++i)
	{
		જો (WinError == ErrorTable[i].WinError)
		{
			પાછા ErrorTable[i].errnocode;
		}
	}

	//
	// not in table. Check ranges
	//

	જો ((WinError >= ERROR_WRITE_PROTECT) &&
		(WinError <= ERROR_SHARING_BUFFER_EXCEEDED))
	{
		પાછા EACCES;
	}
	નહીંતર જો ((WinError >= ERROR_INVALID_STARTING_CODESEG) &&
			 (WinError <= ERROR_INFLOOP_IN_RELOC_CHAIN))
	{
		પાછા ENOEXEC;
	}
	નહીંતર
	{
		પાછા EINVAL;
	}
}







//
// Function to map NT status to dos error.
//

સ્થિર
__inline
unsigned
_MapNtStatus(
    IN NTSTATUS Status
   )
{
	પાછા _MapDosError(RtlNtStatusToDosError(Status));
}





//
// Helper functions to make things easier
//

સ્થિર
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
	UnicodeString.MaximumLength = કદ(Buffer); // in bytes!!!

	RtlInitAnsiString(&AnsiString, Name);

	Status = RtlAnsiStringToUnicodeString(&UnicodeString, &AnsiString, FALSE);

	જો(!NT_SUCCESS(Status))
	{
		પાછા Status; // Unmappable character?
	}

	//
	// Initialize object
	//

	InitializeObjectAttributes(&ObjectAttributes,
							   &UnicodeString,
							   OBJ_CASE_INSENSITIVE,
							   ખાલી,
							   ખાલી );

	//
	// Try to open it in initial mode
	//

	જો(ARGUMENT_PRESENT(OpenedReadonly))
	{
		*OpenedReadonly = Readonly;
	}


	Status = NtOpenFile(Handle,
						SYNCHRONIZE | FILE_READ_DATA | (Readonly ? 0 : FILE_WRITE_DATA),
						&ObjectAttributes,
						&IoStatusBlock,
						FILE_SHARE_WRITE | FILE_SHARE_READ,
						FILE_SYNCHRONOUS_IO_NONALERT);

	જો(!NT_SUCCESS(Status))
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

		જો((STATUS_ACCESS_DENIED == Status) && !Readonly)
		{
			જો(ARGUMENT_PRESENT(OpenedReadonly))
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

	પાછા Status;
}


સ્થિર
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

	પાછા _OpenNtName(Buffer, ReadOnly, Handle, OpenedReadonly);
}


//
// Flush device
//

સ્થિર
__inline
NTSTATUS
_FlushDrive(
		IN HANDLE Handle
		)
{
	IO_STATUS_BLOCK IoStatusBlock;
	પાછા NtFlushBuffersFile(Handle, &IoStatusBlock);
}


//
// lock drive
//

સ્થિર
__inline
NTSTATUS
_LockDrive(
		IN HANDLE Handle
		)
{
	IO_STATUS_BLOCK IoStatusBlock;
	પાછા NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_LOCK_VOLUME, 0, 0, 0, 0);
}


//
// unlock drive
//

સ્થિર
__inline
NTSTATUS
_UnlockDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	પાછા NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0);
}

સ્થિર
__inline
NTSTATUS
_DismountDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	પાછા NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0);
}


//
// is mounted
//

સ્થિર
__inline
BOOLEAN
_IsMounted(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	NTSTATUS Status;
	Status = NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_IS_VOLUME_MOUNTED, 0, 0, 0, 0);
	પાછા (BOOLEAN)(STATUS_SUCCESS == Status);
}


સ્થિર
__inline
NTSTATUS
_CloseDisk(
		IN HANDLE Handle
		)
{
	પાછા NtClose(Handle);
}




//
// Make NT name from any recognized name
//

સ્થિર
PCSTR
_NormalizeDeviceName(
    IN PCSTR Device,
    IN PSTR NormalizedDeviceNameBuffer
   )
{
	પૂર્ણાંક Partition_સંખ્યા = -1;
	UCHAR Disk_સંખ્યા;
	PSTR p;


	//
	// Do not try to parse NT name
	//

	જો('\\' == *Device)
		પાછા Device;



	//
	// Strip leading '/dev/' if any
	//

	જો(('/' == *(Device)) &&
		('d' == *(Device + 1)) &&
		('e' == *(Device + 2)) &&
		('v' == *(Device + 3)) &&
		('/' == *(Device + 4)))
	{
		Device += 5;
	}

	જો('\0' == *Device)
	{
		પાછા ખાલી;
	}


	//
	// forms: hda[n], fd[n]
	//

	જો('d' != *(Device + 1))
	{
		પાછા ખાલી;
	}

	જો('h' == *Device)
	{
		જો((*(Device + 2) < 'a') || (*(Device + 2) > ('a' + 9)) ||
		   ((*(Device + 3) != '\0') &&
			((*(Device + 4) != '\0') ||
			 ((*(Device + 3) < '0') || (*(Device + 3) > '9'))
			)
		   )
		  )
		{
			પાછા ખાલી;
		}

		Disk_સંખ્યા = (UCHAR)(*(Device + 2) - 'a');

		જો(*(Device + 3) != '\0')
		{
			Partition_સંખ્યા = (*(Device + 3) - '0');
		}

	}
	નહીંતર જો('f' == *Device)
	{
		//
		// 3-d letter should be a digit.
		//

		જો((*(Device + 3) != '\0') ||
		   (*(Device + 2) < '0') || (*(Device + 2) > '9'))
		{
			પાછા ખાલી;
		}

		Disk_સંખ્યા = (UCHAR)(*(Device + 2) - '0');

	}
	નહીંતર
	{
		//
		// invalid prefix
		//

		પાછા ખાલી;
	}



	//
	// Prefix
	//

	strcpy(NormalizedDeviceNameBuffer, "\\Device\\");

	//
	// Media name
	//

	પસંદગી(*Device)
	{

	સ્થિતિ 'f':
		strcat(NormalizedDeviceNameBuffer, "Floppy0");
		તોડો;

	સ્થિતિ 'h':
		strcat(NormalizedDeviceNameBuffer, "Harddisk0");
		તોડો;
	}


	p = NormalizedDeviceNameBuffer + strlen(NormalizedDeviceNameBuffer) - 1;
	*p = (CHAR)(*p + Disk_સંખ્યા);


	//
	// Partition nr.
	//

	જો(Partition_સંખ્યા >= 0)
	{
		strcat(NormalizedDeviceNameBuffer, "\\Partition0");

		p = NormalizedDeviceNameBuffer + strlen(NormalizedDeviceNameBuffer) - 1;
		*p = (CHAR)(*p + Partition_સંખ્યા);
	}


	પાછા NormalizedDeviceNameBuffer;
}




સ્થિર
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

	RtlZeroMemory(&pi, કદ(PARTITION_INFORMATION));

	Status = NtDeviceIoControlFile(
		h, ખાલી, ખાલી, ખાલી, &IoStatusBlock, IOCTL_DISK_GET_PARTITION_INFO,
		&pi, કદ(PARTITION_INFORMATION),
		&pi, કદ(PARTITION_INFORMATION));


	જો(NT_SUCCESS(Status))
	{
		*FsSize = pi.PartitionLength.QuadPart;
	}
	નહીંતર જો(STATUS_INVALID_DEVICE_REQUEST == Status)
	{
		//
		// No partitions: get device info.
		//

		RtlZeroMemory(&gi, કદ(DISK_GEOMETRY));

		Status = NtDeviceIoControlFile(
				h, ખાલી, ખાલી, ખાલી, &IoStatusBlock, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, કદ(DISK_GEOMETRY),
				&gi, કદ(DISK_GEOMETRY));


		જો(NT_SUCCESS(Status))
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

સ્થિર
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

	જો(ખાલી == Name)
	{
		//
		// Set not found
		//

		જો(ARGUMENT_PRESENT(Errno))
			*Errno = ENOENT;

		પાછા FALSE;
	}


	જો((((*Name) | 0x20) >= 'a') && (((*Name) | 0x20) <= 'z') &&
		(':' == *(Name + 1)) && ('\0' == *(Name + 2)))
	{
		Status = _OpenDriveLetter(*Name, ReadOnly, Handle, OpenedReadonly);
	}
	નહીંતર
	{
		//
		// Make name
		//

		Name = _NormalizeDeviceName(Name, NormalizedDeviceName);

		જો(ખાલી == Name)
		{
			//
			// Set not found
			//

			જો(ARGUMENT_PRESENT(Errno))
				*Errno = ENOENT;

			પાછા FALSE;
		}

		//
		// Try to open it
		//

		Status = _OpenNtName(Name, ReadOnly, Handle, OpenedReadonly);
	}


	જો(!NT_SUCCESS(Status))
	{
		જો(ARGUMENT_PRESENT(Errno))
			*Errno = _MapNtStatus(Status);

		પાછા FALSE;
	}

	પાછા TRUE;
}


//
// Raw block io. Sets dos errno
//

સ્થિર
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
	ASSERT(0 == (Offset.નીચલું_Part % 512));


	//
	// perform io
	//

	જો(Read)
	{
		Status = NtReadFile(Handle, ખાલી, ખાલી, ખાલી,
			&IoStatusBlock, Buffer, Bytes, &Offset, ખાલી);
	}
	નહીંતર
	{
		Status = NtWriteFile(Handle, ખાલી, ખાલી, ખાલી,
			&IoStatusBlock, Buffer, Bytes, &Offset, ખાલી);
	}


	//
	// translate error
	//

	જો(NT_SUCCESS(Status))
	{
		*Errno = 0;
		પાછા TRUE;
	}

	*Errno = _MapNtStatus(Status);

	પાછા FALSE;
}



__inline
BOOLEAN
_RawWrite(
    IN HANDLE Handle,
    IN LARGE_INTEGER Offset,
    IN ULONG Bytes,
    OUT અચલ CHAR* Buffer,
    OUT unsigned* Errno
   )
{
	પાછા _BlockIo(Handle, Offset, Bytes, (PCHAR)Buffer, FALSE, Errno);
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
	પાછા _BlockIo(Handle, Offset, Bytes, Buffer, TRUE, Errno);
}



__inline
BOOLEAN
_SetPartType(
    IN HANDLE Handle,
    IN UCHAR Type
   )
{
	IO_STATUS_BLOCK IoStatusBlock;
	પાછા STATUS_SUCCESS == NtDeviceIoControlFile(
												   Handle, ખાલી, ખાલી, ખાલી, &IoStatusBlock, IOCTL_DISK_SET_PARTITION_INFO,
												   &Type, કદ(Type),
												   ખાલી, 0);
}



//--------------------- interface part

//
// Interface functions.
// Is_mounted is set to 1 if the device is mounted, 0 otherwise
//

errcode_t
ext2fs_check_if_mounted(અચલ અક્ષર *file, પૂર્ણાંક *mount_flags)
{
	HANDLE h;
	BOOLEAN Readonly;

	*mount_flags = 0;

	જો(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, ખાલી))
	{
		પાછા 0;
	}


	__try{
		*mount_flags &= _IsMounted(h) ? EXT2_MF_MOUNTED : 0;
	}
	__finally{
		_CloseDisk(h);
	}

	પાછા 0;
}



//
// Returns the number of blocks in a partition
//

સ્થિર __int64 FsSize = 0;
સ્થિર અક્ષર knowndevice[1024] = "";


errcode_t
ext2fs_get_device_size(અચલ અક્ષર *file, પૂર્ણાંક blocksize,
				 blk_t *retblocks)
{
	HANDLE h;
	BOOLEAN Readonly;

	જો((0 == FsSize) || (0 != strcmp(knowndevice, file)))
	{

		જો(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, ખાલી))
		{
			પાછા 0;
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
	પાછા 0;
}






//
// Table elements
//


સ્થિર
errcode_t
nt_open(અચલ અક્ષર *name, પૂર્ણાંક flags, io_channel *channel)
{
	io_channel      io = ખાલી;
	PNT_PRIVATE_DATA NtData = ખાલી;
	errcode_t Errno = 0;

	//
	// Check name
	//

	જો (ખાલી == name)
	{
		પાછા EXT2_ET_BAD_DEVICE_NAME;
	}

	__try{

		//
		// Allocate channel handle
		//

		io = (io_channel) malloc(કદ(સંરચના struct_io_channel));

		જો (ખાલી == io)
		{
			Errno = ENOMEM;
			__leave;
		}

		RtlZeroMemory(io, કદ(સંરચના struct_io_channel));
		io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

		NtData = (PNT_PRIVATE_DATA)malloc(કદ(NT_PRIVATE_DATA));

		જો (ખાલી == NtData)
		{
			Errno = ENOMEM;
			__leave;
		}


		io->manager = nt_io_manager();
		io->name = malloc(strlen(name) + 1);
		જો (ખાલી == io->name)
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

		RtlZeroMemory(NtData, કદ(NT_PRIVATE_DATA));

		NtData->magic = EXT2_ET_MAGIC_NT_IO_CHANNEL;
		NtData->Buffer_Block_સંખ્યા = 0xffffffff;
		NtData->BufferSize = 1024;
		NtData->Buffer = malloc(NtData->BufferSize);

		જો (ખાલી == NtData->Buffer)
		{
			Errno = ENOMEM;
			__leave;
		}

		//
		// Open it
		//

		જો(!_Ext2OpenDevice(name, (BOOLEAN)!BooleanFlagOn(flags, EXT2_FLAG_RW), &NtData->Handle, &NtData->OpenedReadonly, &Errno))
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

		જો(!NT_SUCCESS(_LockDrive(NtData->Handle)) /*|| !NT_SUCCESS(_DismountDrive(NtData->Handle))*/)
		{
			NtData->OpenedReadonly = TRUE;
		}

		//
		// Done
		//

		*channel = io;


	}
	__finally{

		જો(0 != Errno)
		{
			//
			// Cleanup
			//

			જો (ખાલી != io)
			{
				મુક્ત(io->name);
				મુક્ત(io);
			}

			જો (ખાલી != NtData)
			{
				જો(ખાલી != NtData->Handle)
				{
					_UnlockDrive(NtData->Handle);
					_CloseDisk(NtData->Handle);
				}

				મુક્ત(NtData->Buffer);
				મુક્ત(NtData);
			}
		}
	}

	પાછા Errno;
}


//
// Close api
//

સ્થિર
errcode_t
nt_close(io_channel channel)
{
	PNT_PRIVATE_DATA NtData = ખાલી;

	જો(ખાલી == channel)
	{
		પાછા 0;
	}

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	જો (--channel->refcount > 0)
	{
		પાછા 0;
	}

	મુક્ત(channel->name);
	મુક્ત(channel);

	જો (ખાલી != NtData)
	{
		જો(ખાલી != NtData->Handle)
		{
			_DismountDrive(NtData->Handle);
			_UnlockDrive(NtData->Handle);
			_CloseDisk(NtData->Handle);
		}

		મુક્ત(NtData->Buffer);
		મુક્ત(NtData);
	}

	પાછા 0;
}



//
// set block size
//

સ્થિર
errcode_t
nt_set_blksize(io_channel channel, પૂર્ણાંક blksize)
{
	PNT_PRIVATE_DATA NtData = ખાલી;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	જો (channel->block_size != blksize)
	{
		channel->block_size = blksize;

		મુક્ત(NtData->Buffer);
		NtData->Buffer_Block_સંખ્યા = 0xffffffff;
		NtData->BufferSize = channel->block_size;
		ASSERT(0 == (NtData->BufferSize % 512));

		NtData->Buffer = malloc(NtData->BufferSize);

		જો (ખાલી == NtData->Buffer)
		{
			પાછા ENOMEM;
		}

	}

	પાછા 0;
}


//
// read block
//

સ્થિર
errcode_t
nt_read_blk(io_channel channel, unsigned long block,
			       પૂર્ણાંક ગણતરી, શૂન્ય *buf)
{
	PVOID BufferToRead;
	ULONG SizeToRead;
	ULONG Size;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_DATA NtData = ખાલી;
	unsigned Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	//
	// If it's in the cache, use it!
	//

	જો ((1 == ગણતરી) &&
		(block == NtData->Buffer_Block_સંખ્યા) &&
		(NtData->Buffer_Block_સંખ્યા != 0xffffffff))
	{
		memcpy(buf, NtData->Buffer, channel->block_size);
		પાછા 0;
	}

	Size = (ગણતરી < 0) ? (ULONG)(-ગણતરી) : (ULONG)(ગણતરી * channel->block_size);

	Offset.QuadPart = block * channel->block_size;

	//
	// If not fit to the block
	//

	જો(Size <= NtData->BufferSize)
	{
		//
		// Update the cache
		//

		NtData->Buffer_Block_સંખ્યા = block;
		BufferToRead = NtData->Buffer;
		SizeToRead = NtData->BufferSize;
	}
	નહીંતર
	{
		SizeToRead = Size;
		BufferToRead = buf;
		ASSERT(0 == (SizeToRead % channel->block_size));
	}

	જો(!_RawRead(NtData->Handle, Offset, SizeToRead, BufferToRead, &Errno))
	{

		જો (channel->read_error)
		{
			પાછા (channel->read_error)(channel, block, ગણતરી, buf,
					       Size, 0, Errno);
		}
		નહીંતર
		{
			પાછા Errno;
		}
	}


	જો(BufferToRead != buf)
	{
		ASSERT(Size <= SizeToRead);
		memcpy(buf, BufferToRead, Size);
	}

	પાછા 0;
}


//
// write block
//

સ્થિર
errcode_t
nt_write_blk(io_channel channel, unsigned long block,
				પૂર્ણાંક ગણતરી, અચલ શૂન્ય *buf)
{
	ULONG SizeToWrite;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_DATA NtData = ખાલી;
	unsigned Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	જો(NtData->OpenedReadonly)
	{
		પાછા EACCES;
	}

	જો (ગણતરી == 1)
	{
		SizeToWrite = channel->block_size;
	}
	નહીંતર
	{
		NtData->Buffer_Block_સંખ્યા = 0xffffffff;

		જો (ગણતરી < 0)
		{
			SizeToWrite = (ULONG)(-ગણતરી);
		}
		નહીંતર
		{
			SizeToWrite = (ULONG)(ગણતરી * channel->block_size);
		}
	}


	ASSERT(0 == (SizeToWrite % 512));
	Offset.QuadPart = block * channel->block_size;

	જો(!_RawWrite(NtData->Handle, Offset, SizeToWrite, buf, &Errno))
	{
		જો (channel->write_error)
		{
			પાછા (channel->write_error)(channel, block, ગણતરી, buf,
						SizeToWrite, 0, Errno);
		}
		નહીંતર
		{
			પાછા Errno;
		}
	}


	//
	// Stash a copy.
	//

	જો(SizeToWrite >= NtData->BufferSize)
	{
		NtData->Buffer_Block_સંખ્યા = block;
		memcpy(NtData->Buffer, buf, NtData->BufferSize);
	}

	NtData->Written = TRUE;

	પાછા 0;

}



//
// Flush data buffers to disk.  Since we are currently using a
// write-through cache, this is a no-op.
//

સ્થિર
errcode_t
nt_flush(io_channel channel)
{
	PNT_PRIVATE_DATA NtData = ખાલી;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	જો(NtData->OpenedReadonly)
	{
		પાછા 0; // EACCESS;
	}


	//
	// Flush file buffers.
	//

	_FlushDrive(NtData->Handle);


	//
	// Test and correct partition type.
	//

	જો(NtData->Written)
	{
		_SetPartType(NtData->Handle, 0x83);
	}

	પાછા 0;
}


