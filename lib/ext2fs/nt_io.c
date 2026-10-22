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
#அந்தர்பாவ "config.h"
#endif


//
// I need some warnings to disable...
//


#pragma warning(disable:4514) // unreferenced inline function has been removed
#pragma warning(தள்ளு,4)

#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union)
#pragma warning(disable:4214) // nonstandard extension used : bit field types other than int
#pragma warning(disable:4115) // named type definition in parentheses

#அந்தர்பாவ <ntddk.h>
#அந்தர்பாவ <ntdddisk.h>
#அந்தர்பாவ <ntstatus.h>

#pragma warning(எடு)


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
    IN PULONG சாவி_2 OPTIONAL
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
    IN PULONG சாவி_2 OPTIONAL
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
    IN PVOID உள்ளீடு_Buffer OPTIONAL,
    IN ULONG உள்ளீடு_Buffer_Length,
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
    IN PVOID உள்ளீடு_Buffer OPTIONAL,
    IN ULONG உள்ளீடு_Buffer_Length,
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

#அந்தர்பாவ <winerror.h>

//
// standard stuff
//

#அந்தர்பாவ <assert.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <malloc.h>

#அந்தர்பாவ <linux/types.h>
#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ <errno.h>

#அந்தர்பாவ "et/com_err.h"
#அந்தர்பாவ "ext2fs/ext2fs.h"
#அந்தர்பாவ "ext2fs/ext2_err.h"




//
// For checking structure magic numbers...
//


#define EXT2_CHECK_MAGIC(கட்டமைப்பு, code) \
	  எனில் ((கட்டமைப்பு)->magic != (code)) திரும்பு (code)

#define EXT2_ET_MAGIC_NT_IO_CHANNEL  0x10ed


//
// Private data block
//

வகைவரை கட்டமைப்பு _NT_PRIVATE_DATA {
	முழு	   magic;
	HANDLE Handle;
	முழு	   Flags;
	PCHAR  Buffer;
	__u32  Buffer_Block_எண்;
	ULONG  BufferSize;
	BOOLEAN OpenedReadonly;
	BOOLEAN Written;
}NT_PRIVATE_DATA, *PNT_PRIVATE_DATA;



//
// Standard interface prototypes
//

நிலையான errcode_t nt_open(மாறிலி எழுத்து *name, முழு flags, io_channel *channel);
நிலையான errcode_t nt_close(io_channel channel);
நிலையான errcode_t nt_set_blksize(io_channel channel, முழு blksize);
நிலையான errcode_t nt_read_blk(io_channel channel, unsigned long block,
			       முழு மொத்தம், வெறுமை *data);
நிலையான errcode_t nt_write_blk(io_channel channel, unsigned long block,
				முழு மொத்தம், மாறிலி வெறுமை *data);
நிலையான errcode_t nt_flush(io_channel channel);

நிலையான கட்டமைப்பு struct_io_manager struct_nt_manager = {
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
	திரும்பு &struct_nt_manager;
}





//
// This is a code to convert Win32 errors to unix errno
//

வகைவரை கட்டமைப்பு {
	ULONG WinError;
	முழு errnocode;
}ERROR_ENTRY;

நிலையான ERROR_ENTRY ErrorTable[] = {
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




நிலையான
unsigned
_MapDosError (
    IN ULONG WinError
   )
{
	முழு i;

	//
	// Lookup
	//

	க்கு (i = 0; i < (அளவு(ErrorTable)/அளவு(ErrorTable[0])); ++i)
	{
		எனில் (WinError == ErrorTable[i].WinError)
		{
			திரும்பு ErrorTable[i].errnocode;
		}
	}

	//
	// not in table. Check ranges
	//

	எனில் ((WinError >= ERROR_WRITE_PROTECT) &&
		(WinError <= ERROR_SHARING_BUFFER_EXCEEDED))
	{
		திரும்பு EACCES;
	}
	இல்லையெனில் எனில் ((WinError >= ERROR_INVALID_STARTING_CODESEG) &&
			 (WinError <= ERROR_INFLOOP_IN_RELOC_CHAIN))
	{
		திரும்பு ENOEXEC;
	}
	இல்லையெனில்
	{
		திரும்பு EINVAL;
	}
}







//
// Function to map NT status to dos error.
//

நிலையான
__inline
unsigned
_MapNtStatus(
    IN NTSTATUS Status
   )
{
	திரும்பு _MapDosError(RtlNtStatusToDosError(Status));
}





//
// Helper functions to make things easier
//

நிலையான
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
	UnicodeString.MaximumLength = அளவு(Buffer); // in bytes!!!

	RtlInitAnsiString(&AnsiString, Name);

	Status = RtlAnsiStringToUnicodeString(&UnicodeString, &AnsiString, FALSE);

	எனில்(!NT_SUCCESS(Status))
	{
		திரும்பு Status; // Unmappable character?
	}

	//
	// Initialize object
	//

	InitializeObjectAttributes(&ObjectAttributes,
							   &UnicodeString,
							   OBJ_CASE_INSENSITIVE,
							   வெற்று,
							   வெற்று );

	//
	// Try to open it in initial mode
	//

	எனில்(ARGUMENT_PRESENT(OpenedReadonly))
	{
		*OpenedReadonly = Readonly;
	}


	Status = NtOpenFile(Handle,
						SYNCHRONIZE | FILE_READ_DATA | (Readonly ? 0 : FILE_WRITE_DATA),
						&ObjectAttributes,
						&IoStatusBlock,
						FILE_SHARE_WRITE | FILE_SHARE_READ,
						FILE_SYNCHRONOUS_IO_NONALERT);

	எனில்(!NT_SUCCESS(Status))
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

		எனில்((STATUS_ACCESS_DENIED == Status) && !Readonly)
		{
			எனில்(ARGUMENT_PRESENT(OpenedReadonly))
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

	திரும்பு Status;
}


நிலையான
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

	திரும்பு _OpenNtName(Buffer, ReadOnly, Handle, OpenedReadonly);
}


//
// Flush device
//

நிலையான
__inline
NTSTATUS
_FlushDrive(
		IN HANDLE Handle
		)
{
	IO_STATUS_BLOCK IoStatusBlock;
	திரும்பு NtFlushBuffersFile(Handle, &IoStatusBlock);
}


//
// lock drive
//

நிலையான
__inline
NTSTATUS
_LockDrive(
		IN HANDLE Handle
		)
{
	IO_STATUS_BLOCK IoStatusBlock;
	திரும்பு NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_LOCK_VOLUME, 0, 0, 0, 0);
}


//
// unlock drive
//

நிலையான
__inline
NTSTATUS
_UnlockDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	திரும்பு NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0);
}

நிலையான
__inline
NTSTATUS
_DismountDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	திரும்பு NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0);
}


//
// is mounted
//

நிலையான
__inline
BOOLEAN
_IsMounted(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	NTSTATUS Status;
	Status = NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_IS_VOLUME_MOUNTED, 0, 0, 0, 0);
	திரும்பு (BOOLEAN)(STATUS_SUCCESS == Status);
}


நிலையான
__inline
NTSTATUS
_CloseDisk(
		IN HANDLE Handle
		)
{
	திரும்பு NtClose(Handle);
}




//
// Make NT name from any recognized name
//

நிலையான
PCSTR
_NormalizeDeviceName(
    IN PCSTR Device,
    IN PSTR NormalizedDeviceNameBuffer
   )
{
	முழு Partition_எண் = -1;
	UCHAR Disk_எண்;
	PSTR p;


	//
	// Do not try to parse NT name
	//

	எனில்('\\' == *Device)
		திரும்பு Device;



	//
	// Strip leading '/dev/' if any
	//

	எனில்(('/' == *(Device)) &&
		('d' == *(Device + 1)) &&
		('e' == *(Device + 2)) &&
		('v' == *(Device + 3)) &&
		('/' == *(Device + 4)))
	{
		Device += 5;
	}

	எனில்('\0' == *Device)
	{
		திரும்பு வெற்று;
	}


	//
	// forms: hda[n], fd[n]
	//

	எனில்('d' != *(Device + 1))
	{
		திரும்பு வெற்று;
	}

	எனில்('h' == *Device)
	{
		எனில்((*(Device + 2) < 'a') || (*(Device + 2) > ('a' + 9)) ||
		   ((*(Device + 3) != '\0') &&
			((*(Device + 4) != '\0') ||
			 ((*(Device + 3) < '0') || (*(Device + 3) > '9'))
			)
		   )
		  )
		{
			திரும்பு வெற்று;
		}

		Disk_எண் = (UCHAR)(*(Device + 2) - 'a');

		எனில்(*(Device + 3) != '\0')
		{
			Partition_எண் = (*(Device + 3) - '0');
		}

	}
	இல்லையெனில் எனில்('f' == *Device)
	{
		//
		// 3-d letter should be a digit.
		//

		எனில்((*(Device + 3) != '\0') ||
		   (*(Device + 2) < '0') || (*(Device + 2) > '9'))
		{
			திரும்பு வெற்று;
		}

		Disk_எண் = (UCHAR)(*(Device + 2) - '0');

	}
	இல்லையெனில்
	{
		//
		// invalid prefix
		//

		திரும்பு வெற்று;
	}



	//
	// Prefix
	//

	strcpy(NormalizedDeviceNameBuffer, "\\Device\\");

	//
	// Media name
	//

	தேர்வு(*Device)
	{

	நிலை 'f':
		strcat(NormalizedDeviceNameBuffer, "Floppy0");
		நிறுத்து;

	நிலை 'h':
		strcat(NormalizedDeviceNameBuffer, "Harddisk0");
		நிறுத்து;
	}


	p = NormalizedDeviceNameBuffer + strlen(NormalizedDeviceNameBuffer) - 1;
	*p = (CHAR)(*p + Disk_எண்);


	//
	// Partition nr.
	//

	எனில்(Partition_எண் >= 0)
	{
		strcat(NormalizedDeviceNameBuffer, "\\Partition0");

		p = NormalizedDeviceNameBuffer + strlen(NormalizedDeviceNameBuffer) - 1;
		*p = (CHAR)(*p + Partition_எண்);
	}


	திரும்பு NormalizedDeviceNameBuffer;
}




நிலையான
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

	RtlZeroMemory(&pi, அளவு(PARTITION_INFORMATION));

	Status = NtDeviceIoControlFile(
		h, வெற்று, வெற்று, வெற்று, &IoStatusBlock, IOCTL_DISK_GET_PARTITION_INFO,
		&pi, அளவு(PARTITION_INFORMATION),
		&pi, அளவு(PARTITION_INFORMATION));


	எனில்(NT_SUCCESS(Status))
	{
		*FsSize = pi.PartitionLength.QuadPart;
	}
	இல்லையெனில் எனில்(STATUS_INVALID_DEVICE_REQUEST == Status)
	{
		//
		// No partitions: get device info.
		//

		RtlZeroMemory(&gi, அளவு(DISK_GEOMETRY));

		Status = NtDeviceIoControlFile(
				h, வெற்று, வெற்று, வெற்று, &IoStatusBlock, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, அளவு(DISK_GEOMETRY),
				&gi, அளவு(DISK_GEOMETRY));


		எனில்(NT_SUCCESS(Status))
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

நிலையான
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

	எனில்(வெற்று == Name)
	{
		//
		// Set not found
		//

		எனில்(ARGUMENT_PRESENT(Errno))
			*Errno = ENOENT;

		திரும்பு FALSE;
	}


	எனில்((((*Name) | 0x20) >= 'a') && (((*Name) | 0x20) <= 'z') &&
		(':' == *(Name + 1)) && ('\0' == *(Name + 2)))
	{
		Status = _OpenDriveLetter(*Name, ReadOnly, Handle, OpenedReadonly);
	}
	இல்லையெனில்
	{
		//
		// Make name
		//

		Name = _NormalizeDeviceName(Name, NormalizedDeviceName);

		எனில்(வெற்று == Name)
		{
			//
			// Set not found
			//

			எனில்(ARGUMENT_PRESENT(Errno))
				*Errno = ENOENT;

			திரும்பு FALSE;
		}

		//
		// Try to open it
		//

		Status = _OpenNtName(Name, ReadOnly, Handle, OpenedReadonly);
	}


	எனில்(!NT_SUCCESS(Status))
	{
		எனில்(ARGUMENT_PRESENT(Errno))
			*Errno = _MapNtStatus(Status);

		திரும்பு FALSE;
	}

	திரும்பு TRUE;
}


//
// Raw block io. Sets dos errno
//

நிலையான
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
	ASSERT(0 == (Offset.கீழ்_Part % 512));


	//
	// perform io
	//

	எனில்(Read)
	{
		Status = NtReadFile(Handle, வெற்று, வெற்று, வெற்று,
			&IoStatusBlock, Buffer, Bytes, &Offset, வெற்று);
	}
	இல்லையெனில்
	{
		Status = NtWriteFile(Handle, வெற்று, வெற்று, வெற்று,
			&IoStatusBlock, Buffer, Bytes, &Offset, வெற்று);
	}


	//
	// translate error
	//

	எனில்(NT_SUCCESS(Status))
	{
		*Errno = 0;
		திரும்பு TRUE;
	}

	*Errno = _MapNtStatus(Status);

	திரும்பு FALSE;
}



__inline
BOOLEAN
_RawWrite(
    IN HANDLE Handle,
    IN LARGE_INTEGER Offset,
    IN ULONG Bytes,
    OUT மாறிலி CHAR* Buffer,
    OUT unsigned* Errno
   )
{
	திரும்பு _BlockIo(Handle, Offset, Bytes, (PCHAR)Buffer, FALSE, Errno);
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
	திரும்பு _BlockIo(Handle, Offset, Bytes, Buffer, TRUE, Errno);
}



__inline
BOOLEAN
_SetPartType(
    IN HANDLE Handle,
    IN UCHAR Type
   )
{
	IO_STATUS_BLOCK IoStatusBlock;
	திரும்பு STATUS_SUCCESS == NtDeviceIoControlFile(
												   Handle, வெற்று, வெற்று, வெற்று, &IoStatusBlock, IOCTL_DISK_SET_PARTITION_INFO,
												   &Type, அளவு(Type),
												   வெற்று, 0);
}



//--------------------- interface part

//
// Interface functions.
// Is_mounted is set to 1 if the device is mounted, 0 otherwise
//

errcode_t
ext2fs_check_if_mounted(மாறிலி எழுத்து *file, முழு *mount_flags)
{
	HANDLE h;
	BOOLEAN Readonly;

	*mount_flags = 0;

	எனில்(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, வெற்று))
	{
		திரும்பு 0;
	}


	__try{
		*mount_flags &= _IsMounted(h) ? EXT2_MF_MOUNTED : 0;
	}
	__finally{
		_CloseDisk(h);
	}

	திரும்பு 0;
}



//
// Returns the number of blocks in a partition
//

நிலையான __int64 FsSize = 0;
நிலையான எழுத்து knowndevice[1024] = "";


errcode_t
ext2fs_get_device_size(மாறிலி எழுத்து *file, முழு blocksize,
				 blk_t *retblocks)
{
	HANDLE h;
	BOOLEAN Readonly;

	எனில்((0 == FsSize) || (0 != strcmp(knowndevice, file)))
	{

		எனில்(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, வெற்று))
		{
			திரும்பு 0;
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
	திரும்பு 0;
}






//
// Table elements
//


நிலையான
errcode_t
nt_open(மாறிலி எழுத்து *name, முழு flags, io_channel *channel)
{
	io_channel      io = வெற்று;
	PNT_PRIVATE_DATA NtData = வெற்று;
	errcode_t Errno = 0;

	//
	// Check name
	//

	எனில் (வெற்று == name)
	{
		திரும்பு EXT2_ET_BAD_DEVICE_NAME;
	}

	__try{

		//
		// Allocate channel handle
		//

		io = (io_channel) malloc(அளவு(கட்டமைப்பு struct_io_channel));

		எனில் (வெற்று == io)
		{
			Errno = ENOMEM;
			__leave;
		}

		RtlZeroMemory(io, அளவு(கட்டமைப்பு struct_io_channel));
		io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

		NtData = (PNT_PRIVATE_DATA)malloc(அளவு(NT_PRIVATE_DATA));

		எனில் (வெற்று == NtData)
		{
			Errno = ENOMEM;
			__leave;
		}


		io->manager = nt_io_manager();
		io->name = malloc(strlen(name) + 1);
		எனில் (வெற்று == io->name)
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

		RtlZeroMemory(NtData, அளவு(NT_PRIVATE_DATA));

		NtData->magic = EXT2_ET_MAGIC_NT_IO_CHANNEL;
		NtData->Buffer_Block_எண் = 0xffffffff;
		NtData->BufferSize = 1024;
		NtData->Buffer = malloc(NtData->BufferSize);

		எனில் (வெற்று == NtData->Buffer)
		{
			Errno = ENOMEM;
			__leave;
		}

		//
		// Open it
		//

		எனில்(!_Ext2OpenDevice(name, (BOOLEAN)!BooleanFlagOn(flags, EXT2_FLAG_RW), &NtData->Handle, &NtData->OpenedReadonly, &Errno))
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

		எனில்(!NT_SUCCESS(_LockDrive(NtData->Handle)) /*|| !NT_SUCCESS(_DismountDrive(NtData->Handle))*/)
		{
			NtData->OpenedReadonly = TRUE;
		}

		//
		// Done
		//

		*channel = io;


	}
	__finally{

		எனில்(0 != Errno)
		{
			//
			// Cleanup
			//

			எனில் (வெற்று != io)
			{
				விடுவி(io->name);
				விடுவி(io);
			}

			எனில் (வெற்று != NtData)
			{
				எனில்(வெற்று != NtData->Handle)
				{
					_UnlockDrive(NtData->Handle);
					_CloseDisk(NtData->Handle);
				}

				விடுவி(NtData->Buffer);
				விடுவி(NtData);
			}
		}
	}

	திரும்பு Errno;
}


//
// Close api
//

நிலையான
errcode_t
nt_close(io_channel channel)
{
	PNT_PRIVATE_DATA NtData = வெற்று;

	எனில்(வெற்று == channel)
	{
		திரும்பு 0;
	}

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	எனில் (--channel->refcount > 0)
	{
		திரும்பு 0;
	}

	விடுவி(channel->name);
	விடுவி(channel);

	எனில் (வெற்று != NtData)
	{
		எனில்(வெற்று != NtData->Handle)
		{
			_DismountDrive(NtData->Handle);
			_UnlockDrive(NtData->Handle);
			_CloseDisk(NtData->Handle);
		}

		விடுவி(NtData->Buffer);
		விடுவி(NtData);
	}

	திரும்பு 0;
}



//
// set block size
//

நிலையான
errcode_t
nt_set_blksize(io_channel channel, முழு blksize)
{
	PNT_PRIVATE_DATA NtData = வெற்று;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	எனில் (channel->block_size != blksize)
	{
		channel->block_size = blksize;

		விடுவி(NtData->Buffer);
		NtData->Buffer_Block_எண் = 0xffffffff;
		NtData->BufferSize = channel->block_size;
		ASSERT(0 == (NtData->BufferSize % 512));

		NtData->Buffer = malloc(NtData->BufferSize);

		எனில் (வெற்று == NtData->Buffer)
		{
			திரும்பு ENOMEM;
		}

	}

	திரும்பு 0;
}


//
// read block
//

நிலையான
errcode_t
nt_read_blk(io_channel channel, unsigned long block,
			       முழு மொத்தம், வெறுமை *buf)
{
	PVOID BufferToRead;
	ULONG SizeToRead;
	ULONG Size;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_DATA NtData = வெற்று;
	unsigned Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	//
	// If it's in the cache, use it!
	//

	எனில் ((1 == மொத்தம்) &&
		(block == NtData->Buffer_Block_எண்) &&
		(NtData->Buffer_Block_எண் != 0xffffffff))
	{
		memcpy(buf, NtData->Buffer, channel->block_size);
		திரும்பு 0;
	}

	Size = (மொத்தம் < 0) ? (ULONG)(-மொத்தம்) : (ULONG)(மொத்தம் * channel->block_size);

	Offset.QuadPart = block * channel->block_size;

	//
	// If not fit to the block
	//

	எனில்(Size <= NtData->BufferSize)
	{
		//
		// Update the cache
		//

		NtData->Buffer_Block_எண் = block;
		BufferToRead = NtData->Buffer;
		SizeToRead = NtData->BufferSize;
	}
	இல்லையெனில்
	{
		SizeToRead = Size;
		BufferToRead = buf;
		ASSERT(0 == (SizeToRead % channel->block_size));
	}

	எனில்(!_RawRead(NtData->Handle, Offset, SizeToRead, BufferToRead, &Errno))
	{

		எனில் (channel->read_error)
		{
			திரும்பு (channel->read_error)(channel, block, மொத்தம், buf,
					       Size, 0, Errno);
		}
		இல்லையெனில்
		{
			திரும்பு Errno;
		}
	}


	எனில்(BufferToRead != buf)
	{
		ASSERT(Size <= SizeToRead);
		memcpy(buf, BufferToRead, Size);
	}

	திரும்பு 0;
}


//
// write block
//

நிலையான
errcode_t
nt_write_blk(io_channel channel, unsigned long block,
				முழு மொத்தம், மாறிலி வெறுமை *buf)
{
	ULONG SizeToWrite;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_DATA NtData = வெற்று;
	unsigned Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	எனில்(NtData->OpenedReadonly)
	{
		திரும்பு EACCES;
	}

	எனில் (மொத்தம் == 1)
	{
		SizeToWrite = channel->block_size;
	}
	இல்லையெனில்
	{
		NtData->Buffer_Block_எண் = 0xffffffff;

		எனில் (மொத்தம் < 0)
		{
			SizeToWrite = (ULONG)(-மொத்தம்);
		}
		இல்லையெனில்
		{
			SizeToWrite = (ULONG)(மொத்தம் * channel->block_size);
		}
	}


	ASSERT(0 == (SizeToWrite % 512));
	Offset.QuadPart = block * channel->block_size;

	எனில்(!_RawWrite(NtData->Handle, Offset, SizeToWrite, buf, &Errno))
	{
		எனில் (channel->write_error)
		{
			திரும்பு (channel->write_error)(channel, block, மொத்தம், buf,
						SizeToWrite, 0, Errno);
		}
		இல்லையெனில்
		{
			திரும்பு Errno;
		}
	}


	//
	// Stash a copy.
	//

	எனில்(SizeToWrite >= NtData->BufferSize)
	{
		NtData->Buffer_Block_எண் = block;
		memcpy(NtData->Buffer, buf, NtData->BufferSize);
	}

	NtData->Written = TRUE;

	திரும்பு 0;

}



//
// Flush data buffers to disk.  Since we are currently using a
// write-through cache, this is a no-op.
//

நிலையான
errcode_t
nt_flush(io_channel channel)
{
	PNT_PRIVATE_DATA NtData = வெற்று;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	எனில்(NtData->OpenedReadonly)
	{
		திரும்பு 0; // EACCESS;
	}


	//
	// Flush file buffers.
	//

	_FlushDrive(NtData->Handle);


	//
	// Test and correct partition type.
	//

	எனில்(NtData->Written)
	{
		_SetPartType(NtData->Handle, 0x83);
	}

	திரும்பு 0;
}


