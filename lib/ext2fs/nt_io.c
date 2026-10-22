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
#അന്തര്ഭാവ "config.h"
#endif


//
// I need some warnings to disable...
//


#pragma warning(disable:4514) // unreferenced inline function has been removed
#pragma warning(തള്ളുക,4)

#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union)
#pragma warning(disable:4214) // nonstandard extension used : bit field types other than int
#pragma warning(disable:4115) // named type definition in parentheses

#അന്തര്ഭാവ <ntddk.h>
#അന്തര്ഭാവ <ntdddisk.h>
#അന്തര്ഭാവ <ntstatus.h>

#pragma warning(എടുക്കുക)


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
    IN PULONG താക്കോൽ_2 OPTIONAL
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
    IN PULONG താക്കോൽ_2 OPTIONAL
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
    IN PVOID നിവേശം_Buffer OPTIONAL,
    IN ULONG നിവേശം_Buffer_Length,
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
    IN PVOID നിവേശം_Buffer OPTIONAL,
    IN ULONG നിവേശം_Buffer_Length,
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

#അന്തര്ഭാവ <winerror.h>

//
// standard stuff
//

#അന്തര്ഭാവ <assert.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <malloc.h>

#അന്തര്ഭാവ <linux/types.h>
#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ <errno.h>

#അന്തര്ഭാവ "et/com_err.h"
#അന്തര്ഭാവ "ext2fs/ext2fs.h"
#അന്തര്ഭാവ "ext2fs/ext2_err.h"




//
// For checking structure magic numbers...
//


#define EXT2_CHECK_MAGIC(ഘടന, code) \
	  എങ്കിൽ ((ഘടന)->magic != (code)) തിരികെ (code)

#define EXT2_ET_MAGIC_NT_IO_CHANNEL  0x10ed


//
// Private data block
//

തരനിർവചനം ഘടന _NT_PRIVATE_DATA {
	പൂർണ്ണസംഖ്യ	   magic;
	HANDLE Handle;
	പൂർണ്ണസംഖ്യ	   Flags;
	PCHAR  Buffer;
	__u32  Buffer_Block_സംഖ്യ;
	ULONG  BufferSize;
	BOOLEAN OpenedReadonly;
	BOOLEAN Written;
}NT_PRIVATE_DATA, *PNT_PRIVATE_DATA;



//
// Standard interface prototypes
//

സ്ഥിതം errcode_t nt_open(സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ flags, io_channel *channel);
സ്ഥിതം errcode_t nt_close(io_channel channel);
സ്ഥിതം errcode_t nt_set_blksize(io_channel channel, പൂർണ്ണസംഖ്യ blksize);
സ്ഥിതം errcode_t nt_read_blk(io_channel channel, unsigned long block,
			       പൂർണ്ണസംഖ്യ എണ്ണം, ശൂന്യം *data);
സ്ഥിതം errcode_t nt_write_blk(io_channel channel, unsigned long block,
				പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *data);
സ്ഥിതം errcode_t nt_flush(io_channel channel);

സ്ഥിതം ഘടന struct_io_manager struct_nt_manager = {
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
	തിരികെ &struct_nt_manager;
}





//
// This is a code to convert Win32 errors to unix errno
//

തരനിർവചനം ഘടന {
	ULONG WinError;
	പൂർണ്ണസംഖ്യ errnocode;
}ERROR_ENTRY;

സ്ഥിതം ERROR_ENTRY ErrorTable[] = {
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




സ്ഥിതം
unsigned
_MapDosError (
    IN ULONG WinError
   )
{
	പൂർണ്ണസംഖ്യ i;

	//
	// Lookup
	//

	വേണ്ടി (i = 0; i < (വലുപ്പം(ErrorTable)/വലുപ്പം(ErrorTable[0])); ++i)
	{
		എങ്കിൽ (WinError == ErrorTable[i].WinError)
		{
			തിരികെ ErrorTable[i].errnocode;
		}
	}

	//
	// not in table. Check ranges
	//

	എങ്കിൽ ((WinError >= ERROR_WRITE_PROTECT) &&
		(WinError <= ERROR_SHARING_BUFFER_EXCEEDED))
	{
		തിരികെ EACCES;
	}
	അല്ലെങ്കിൽ എങ്കിൽ ((WinError >= ERROR_INVALID_STARTING_CODESEG) &&
			 (WinError <= ERROR_INFLOOP_IN_RELOC_CHAIN))
	{
		തിരികെ ENOEXEC;
	}
	അല്ലെങ്കിൽ
	{
		തിരികെ EINVAL;
	}
}







//
// Function to map NT status to dos error.
//

സ്ഥിതം
__inline
unsigned
_MapNtStatus(
    IN NTSTATUS Status
   )
{
	തിരികെ _MapDosError(RtlNtStatusToDosError(Status));
}





//
// Helper functions to make things easier
//

സ്ഥിതം
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
	UnicodeString.MaximumLength = വലുപ്പം(Buffer); // in bytes!!!

	RtlInitAnsiString(&AnsiString, Name);

	Status = RtlAnsiStringToUnicodeString(&UnicodeString, &AnsiString, FALSE);

	എങ്കിൽ(!NT_SUCCESS(Status))
	{
		തിരികെ Status; // Unmappable character?
	}

	//
	// Initialize object
	//

	InitializeObjectAttributes(&ObjectAttributes,
							   &UnicodeString,
							   OBJ_CASE_INSENSITIVE,
							   ശൂന്യ,
							   ശൂന്യ );

	//
	// Try to open it in initial mode
	//

	എങ്കിൽ(ARGUMENT_PRESENT(OpenedReadonly))
	{
		*OpenedReadonly = Readonly;
	}


	Status = NtOpenFile(Handle,
						SYNCHRONIZE | FILE_READ_DATA | (Readonly ? 0 : FILE_WRITE_DATA),
						&ObjectAttributes,
						&IoStatusBlock,
						FILE_SHARE_WRITE | FILE_SHARE_READ,
						FILE_SYNCHRONOUS_IO_NONALERT);

	എങ്കിൽ(!NT_SUCCESS(Status))
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

		എങ്കിൽ((STATUS_ACCESS_DENIED == Status) && !Readonly)
		{
			എങ്കിൽ(ARGUMENT_PRESENT(OpenedReadonly))
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

	തിരികെ Status;
}


സ്ഥിതം
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

	തിരികെ _OpenNtName(Buffer, ReadOnly, Handle, OpenedReadonly);
}


//
// Flush device
//

സ്ഥിതം
__inline
NTSTATUS
_FlushDrive(
		IN HANDLE Handle
		)
{
	IO_STATUS_BLOCK IoStatusBlock;
	തിരികെ NtFlushBuffersFile(Handle, &IoStatusBlock);
}


//
// lock drive
//

സ്ഥിതം
__inline
NTSTATUS
_LockDrive(
		IN HANDLE Handle
		)
{
	IO_STATUS_BLOCK IoStatusBlock;
	തിരികെ NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_LOCK_VOLUME, 0, 0, 0, 0);
}


//
// unlock drive
//

സ്ഥിതം
__inline
NTSTATUS
_UnlockDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	തിരികെ NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0);
}

സ്ഥിതം
__inline
NTSTATUS
_DismountDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	തിരികെ NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0);
}


//
// is mounted
//

സ്ഥിതം
__inline
BOOLEAN
_IsMounted(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	NTSTATUS Status;
	Status = NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_IS_VOLUME_MOUNTED, 0, 0, 0, 0);
	തിരികെ (BOOLEAN)(STATUS_SUCCESS == Status);
}


സ്ഥിതം
__inline
NTSTATUS
_CloseDisk(
		IN HANDLE Handle
		)
{
	തിരികെ NtClose(Handle);
}




//
// Make NT name from any recognized name
//

സ്ഥിതം
PCSTR
_NormalizeDeviceName(
    IN PCSTR Device,
    IN PSTR NormalizedDeviceNameBuffer
   )
{
	പൂർണ്ണസംഖ്യ Partition_സംഖ്യ = -1;
	UCHAR Disk_സംഖ്യ;
	PSTR p;


	//
	// Do not try to parse NT name
	//

	എങ്കിൽ('\\' == *Device)
		തിരികെ Device;



	//
	// Strip leading '/dev/' if any
	//

	എങ്കിൽ(('/' == *(Device)) &&
		('d' == *(Device + 1)) &&
		('e' == *(Device + 2)) &&
		('v' == *(Device + 3)) &&
		('/' == *(Device + 4)))
	{
		Device += 5;
	}

	എങ്കിൽ('\0' == *Device)
	{
		തിരികെ ശൂന്യ;
	}


	//
	// forms: hda[n], fd[n]
	//

	എങ്കിൽ('d' != *(Device + 1))
	{
		തിരികെ ശൂന്യ;
	}

	എങ്കിൽ('h' == *Device)
	{
		എങ്കിൽ((*(Device + 2) < 'a') || (*(Device + 2) > ('a' + 9)) ||
		   ((*(Device + 3) != '\0') &&
			((*(Device + 4) != '\0') ||
			 ((*(Device + 3) < '0') || (*(Device + 3) > '9'))
			)
		   )
		  )
		{
			തിരികെ ശൂന്യ;
		}

		Disk_സംഖ്യ = (UCHAR)(*(Device + 2) - 'a');

		എങ്കിൽ(*(Device + 3) != '\0')
		{
			Partition_സംഖ്യ = (*(Device + 3) - '0');
		}

	}
	അല്ലെങ്കിൽ എങ്കിൽ('f' == *Device)
	{
		//
		// 3-d letter should be a digit.
		//

		എങ്കിൽ((*(Device + 3) != '\0') ||
		   (*(Device + 2) < '0') || (*(Device + 2) > '9'))
		{
			തിരികെ ശൂന്യ;
		}

		Disk_സംഖ്യ = (UCHAR)(*(Device + 2) - '0');

	}
	അല്ലെങ്കിൽ
	{
		//
		// invalid prefix
		//

		തിരികെ ശൂന്യ;
	}



	//
	// Prefix
	//

	strcpy(NormalizedDeviceNameBuffer, "\\Device\\");

	//
	// Media name
	//

	തിരഞ്ഞെടുപ്പ്(*Device)
	{

	അവസ്ഥ 'f':
		strcat(NormalizedDeviceNameBuffer, "Floppy0");
		നിർത്തുക;

	അവസ്ഥ 'h':
		strcat(NormalizedDeviceNameBuffer, "Harddisk0");
		നിർത്തുക;
	}


	p = NormalizedDeviceNameBuffer + strlen(NormalizedDeviceNameBuffer) - 1;
	*p = (CHAR)(*p + Disk_സംഖ്യ);


	//
	// Partition nr.
	//

	എങ്കിൽ(Partition_സംഖ്യ >= 0)
	{
		strcat(NormalizedDeviceNameBuffer, "\\Partition0");

		p = NormalizedDeviceNameBuffer + strlen(NormalizedDeviceNameBuffer) - 1;
		*p = (CHAR)(*p + Partition_സംഖ്യ);
	}


	തിരികെ NormalizedDeviceNameBuffer;
}




സ്ഥിതം
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

	RtlZeroMemory(&pi, വലുപ്പം(PARTITION_INFORMATION));

	Status = NtDeviceIoControlFile(
		h, ശൂന്യ, ശൂന്യ, ശൂന്യ, &IoStatusBlock, IOCTL_DISK_GET_PARTITION_INFO,
		&pi, വലുപ്പം(PARTITION_INFORMATION),
		&pi, വലുപ്പം(PARTITION_INFORMATION));


	എങ്കിൽ(NT_SUCCESS(Status))
	{
		*FsSize = pi.PartitionLength.QuadPart;
	}
	അല്ലെങ്കിൽ എങ്കിൽ(STATUS_INVALID_DEVICE_REQUEST == Status)
	{
		//
		// No partitions: get device info.
		//

		RtlZeroMemory(&gi, വലുപ്പം(DISK_GEOMETRY));

		Status = NtDeviceIoControlFile(
				h, ശൂന്യ, ശൂന്യ, ശൂന്യ, &IoStatusBlock, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, വലുപ്പം(DISK_GEOMETRY),
				&gi, വലുപ്പം(DISK_GEOMETRY));


		എങ്കിൽ(NT_SUCCESS(Status))
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

സ്ഥിതം
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

	എങ്കിൽ(ശൂന്യ == Name)
	{
		//
		// Set not found
		//

		എങ്കിൽ(ARGUMENT_PRESENT(Errno))
			*Errno = ENOENT;

		തിരികെ FALSE;
	}


	എങ്കിൽ((((*Name) | 0x20) >= 'a') && (((*Name) | 0x20) <= 'z') &&
		(':' == *(Name + 1)) && ('\0' == *(Name + 2)))
	{
		Status = _OpenDriveLetter(*Name, ReadOnly, Handle, OpenedReadonly);
	}
	അല്ലെങ്കിൽ
	{
		//
		// Make name
		//

		Name = _NormalizeDeviceName(Name, NormalizedDeviceName);

		എങ്കിൽ(ശൂന്യ == Name)
		{
			//
			// Set not found
			//

			എങ്കിൽ(ARGUMENT_PRESENT(Errno))
				*Errno = ENOENT;

			തിരികെ FALSE;
		}

		//
		// Try to open it
		//

		Status = _OpenNtName(Name, ReadOnly, Handle, OpenedReadonly);
	}


	എങ്കിൽ(!NT_SUCCESS(Status))
	{
		എങ്കിൽ(ARGUMENT_PRESENT(Errno))
			*Errno = _MapNtStatus(Status);

		തിരികെ FALSE;
	}

	തിരികെ TRUE;
}


//
// Raw block io. Sets dos errno
//

സ്ഥിതം
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
	ASSERT(0 == (Offset.താഴെയുള്ള_Part % 512));


	//
	// perform io
	//

	എങ്കിൽ(Read)
	{
		Status = NtReadFile(Handle, ശൂന്യ, ശൂന്യ, ശൂന്യ,
			&IoStatusBlock, Buffer, Bytes, &Offset, ശൂന്യ);
	}
	അല്ലെങ്കിൽ
	{
		Status = NtWriteFile(Handle, ശൂന്യ, ശൂന്യ, ശൂന്യ,
			&IoStatusBlock, Buffer, Bytes, &Offset, ശൂന്യ);
	}


	//
	// translate error
	//

	എങ്കിൽ(NT_SUCCESS(Status))
	{
		*Errno = 0;
		തിരികെ TRUE;
	}

	*Errno = _MapNtStatus(Status);

	തിരികെ FALSE;
}



__inline
BOOLEAN
_RawWrite(
    IN HANDLE Handle,
    IN LARGE_INTEGER Offset,
    IN ULONG Bytes,
    OUT സ്ഥിരം CHAR* Buffer,
    OUT unsigned* Errno
   )
{
	തിരികെ _BlockIo(Handle, Offset, Bytes, (PCHAR)Buffer, FALSE, Errno);
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
	തിരികെ _BlockIo(Handle, Offset, Bytes, Buffer, TRUE, Errno);
}



__inline
BOOLEAN
_SetPartType(
    IN HANDLE Handle,
    IN UCHAR Type
   )
{
	IO_STATUS_BLOCK IoStatusBlock;
	തിരികെ STATUS_SUCCESS == NtDeviceIoControlFile(
												   Handle, ശൂന്യ, ശൂന്യ, ശൂന്യ, &IoStatusBlock, IOCTL_DISK_SET_PARTITION_INFO,
												   &Type, വലുപ്പം(Type),
												   ശൂന്യ, 0);
}



//--------------------- interface part

//
// Interface functions.
// Is_mounted is set to 1 if the device is mounted, 0 otherwise
//

errcode_t
ext2fs_check_if_mounted(സ്ഥിരം അക്ഷരം *file, പൂർണ്ണസംഖ്യ *mount_flags)
{
	HANDLE h;
	BOOLEAN Readonly;

	*mount_flags = 0;

	എങ്കിൽ(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, ശൂന്യ))
	{
		തിരികെ 0;
	}


	__try{
		*mount_flags &= _IsMounted(h) ? EXT2_MF_MOUNTED : 0;
	}
	__finally{
		_CloseDisk(h);
	}

	തിരികെ 0;
}



//
// Returns the number of blocks in a partition
//

സ്ഥിതം __int64 FsSize = 0;
സ്ഥിതം അക്ഷരം knowndevice[1024] = "";


errcode_t
ext2fs_get_device_size(സ്ഥിരം അക്ഷരം *file, പൂർണ്ണസംഖ്യ blocksize,
				 blk_t *retblocks)
{
	HANDLE h;
	BOOLEAN Readonly;

	എങ്കിൽ((0 == FsSize) || (0 != strcmp(knowndevice, file)))
	{

		എങ്കിൽ(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, ശൂന്യ))
		{
			തിരികെ 0;
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
	തിരികെ 0;
}






//
// Table elements
//


സ്ഥിതം
errcode_t
nt_open(സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ flags, io_channel *channel)
{
	io_channel      io = ശൂന്യ;
	PNT_PRIVATE_DATA NtData = ശൂന്യ;
	errcode_t Errno = 0;

	//
	// Check name
	//

	എങ്കിൽ (ശൂന്യ == name)
	{
		തിരികെ EXT2_ET_BAD_DEVICE_NAME;
	}

	__try{

		//
		// Allocate channel handle
		//

		io = (io_channel) malloc(വലുപ്പം(ഘടന struct_io_channel));

		എങ്കിൽ (ശൂന്യ == io)
		{
			Errno = ENOMEM;
			__leave;
		}

		RtlZeroMemory(io, വലുപ്പം(ഘടന struct_io_channel));
		io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

		NtData = (PNT_PRIVATE_DATA)malloc(വലുപ്പം(NT_PRIVATE_DATA));

		എങ്കിൽ (ശൂന്യ == NtData)
		{
			Errno = ENOMEM;
			__leave;
		}


		io->manager = nt_io_manager();
		io->name = malloc(strlen(name) + 1);
		എങ്കിൽ (ശൂന്യ == io->name)
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

		RtlZeroMemory(NtData, വലുപ്പം(NT_PRIVATE_DATA));

		NtData->magic = EXT2_ET_MAGIC_NT_IO_CHANNEL;
		NtData->Buffer_Block_സംഖ്യ = 0xffffffff;
		NtData->BufferSize = 1024;
		NtData->Buffer = malloc(NtData->BufferSize);

		എങ്കിൽ (ശൂന്യ == NtData->Buffer)
		{
			Errno = ENOMEM;
			__leave;
		}

		//
		// Open it
		//

		എങ്കിൽ(!_Ext2OpenDevice(name, (BOOLEAN)!BooleanFlagOn(flags, EXT2_FLAG_RW), &NtData->Handle, &NtData->OpenedReadonly, &Errno))
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

		എങ്കിൽ(!NT_SUCCESS(_LockDrive(NtData->Handle)) /*|| !NT_SUCCESS(_DismountDrive(NtData->Handle))*/)
		{
			NtData->OpenedReadonly = TRUE;
		}

		//
		// Done
		//

		*channel = io;


	}
	__finally{

		എങ്കിൽ(0 != Errno)
		{
			//
			// Cleanup
			//

			എങ്കിൽ (ശൂന്യ != io)
			{
				സ്വതന്ത്രം(io->name);
				സ്വതന്ത്രം(io);
			}

			എങ്കിൽ (ശൂന്യ != NtData)
			{
				എങ്കിൽ(ശൂന്യ != NtData->Handle)
				{
					_UnlockDrive(NtData->Handle);
					_CloseDisk(NtData->Handle);
				}

				സ്വതന്ത്രം(NtData->Buffer);
				സ്വതന്ത്രം(NtData);
			}
		}
	}

	തിരികെ Errno;
}


//
// Close api
//

സ്ഥിതം
errcode_t
nt_close(io_channel channel)
{
	PNT_PRIVATE_DATA NtData = ശൂന്യ;

	എങ്കിൽ(ശൂന്യ == channel)
	{
		തിരികെ 0;
	}

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	എങ്കിൽ (--channel->refcount > 0)
	{
		തിരികെ 0;
	}

	സ്വതന്ത്രം(channel->name);
	സ്വതന്ത്രം(channel);

	എങ്കിൽ (ശൂന്യ != NtData)
	{
		എങ്കിൽ(ശൂന്യ != NtData->Handle)
		{
			_DismountDrive(NtData->Handle);
			_UnlockDrive(NtData->Handle);
			_CloseDisk(NtData->Handle);
		}

		സ്വതന്ത്രം(NtData->Buffer);
		സ്വതന്ത്രം(NtData);
	}

	തിരികെ 0;
}



//
// set block size
//

സ്ഥിതം
errcode_t
nt_set_blksize(io_channel channel, പൂർണ്ണസംഖ്യ blksize)
{
	PNT_PRIVATE_DATA NtData = ശൂന്യ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	എങ്കിൽ (channel->block_size != blksize)
	{
		channel->block_size = blksize;

		സ്വതന്ത്രം(NtData->Buffer);
		NtData->Buffer_Block_സംഖ്യ = 0xffffffff;
		NtData->BufferSize = channel->block_size;
		ASSERT(0 == (NtData->BufferSize % 512));

		NtData->Buffer = malloc(NtData->BufferSize);

		എങ്കിൽ (ശൂന്യ == NtData->Buffer)
		{
			തിരികെ ENOMEM;
		}

	}

	തിരികെ 0;
}


//
// read block
//

സ്ഥിതം
errcode_t
nt_read_blk(io_channel channel, unsigned long block,
			       പൂർണ്ണസംഖ്യ എണ്ണം, ശൂന്യം *buf)
{
	PVOID BufferToRead;
	ULONG SizeToRead;
	ULONG Size;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_DATA NtData = ശൂന്യ;
	unsigned Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	//
	// If it's in the cache, use it!
	//

	എങ്കിൽ ((1 == എണ്ണം) &&
		(block == NtData->Buffer_Block_സംഖ്യ) &&
		(NtData->Buffer_Block_സംഖ്യ != 0xffffffff))
	{
		memcpy(buf, NtData->Buffer, channel->block_size);
		തിരികെ 0;
	}

	Size = (എണ്ണം < 0) ? (ULONG)(-എണ്ണം) : (ULONG)(എണ്ണം * channel->block_size);

	Offset.QuadPart = block * channel->block_size;

	//
	// If not fit to the block
	//

	എങ്കിൽ(Size <= NtData->BufferSize)
	{
		//
		// Update the cache
		//

		NtData->Buffer_Block_സംഖ്യ = block;
		BufferToRead = NtData->Buffer;
		SizeToRead = NtData->BufferSize;
	}
	അല്ലെങ്കിൽ
	{
		SizeToRead = Size;
		BufferToRead = buf;
		ASSERT(0 == (SizeToRead % channel->block_size));
	}

	എങ്കിൽ(!_RawRead(NtData->Handle, Offset, SizeToRead, BufferToRead, &Errno))
	{

		എങ്കിൽ (channel->read_error)
		{
			തിരികെ (channel->read_error)(channel, block, എണ്ണം, buf,
					       Size, 0, Errno);
		}
		അല്ലെങ്കിൽ
		{
			തിരികെ Errno;
		}
	}


	എങ്കിൽ(BufferToRead != buf)
	{
		ASSERT(Size <= SizeToRead);
		memcpy(buf, BufferToRead, Size);
	}

	തിരികെ 0;
}


//
// write block
//

സ്ഥിതം
errcode_t
nt_write_blk(io_channel channel, unsigned long block,
				പൂർണ്ണസംഖ്യ എണ്ണം, സ്ഥിരം ശൂന്യം *buf)
{
	ULONG SizeToWrite;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_DATA NtData = ശൂന്യ;
	unsigned Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	എങ്കിൽ(NtData->OpenedReadonly)
	{
		തിരികെ EACCES;
	}

	എങ്കിൽ (എണ്ണം == 1)
	{
		SizeToWrite = channel->block_size;
	}
	അല്ലെങ്കിൽ
	{
		NtData->Buffer_Block_സംഖ്യ = 0xffffffff;

		എങ്കിൽ (എണ്ണം < 0)
		{
			SizeToWrite = (ULONG)(-എണ്ണം);
		}
		അല്ലെങ്കിൽ
		{
			SizeToWrite = (ULONG)(എണ്ണം * channel->block_size);
		}
	}


	ASSERT(0 == (SizeToWrite % 512));
	Offset.QuadPart = block * channel->block_size;

	എങ്കിൽ(!_RawWrite(NtData->Handle, Offset, SizeToWrite, buf, &Errno))
	{
		എങ്കിൽ (channel->write_error)
		{
			തിരികെ (channel->write_error)(channel, block, എണ്ണം, buf,
						SizeToWrite, 0, Errno);
		}
		അല്ലെങ്കിൽ
		{
			തിരികെ Errno;
		}
	}


	//
	// Stash a copy.
	//

	എങ്കിൽ(SizeToWrite >= NtData->BufferSize)
	{
		NtData->Buffer_Block_സംഖ്യ = block;
		memcpy(NtData->Buffer, buf, NtData->BufferSize);
	}

	NtData->Written = TRUE;

	തിരികെ 0;

}



//
// Flush data buffers to disk.  Since we are currently using a
// write-through cache, this is a no-op.
//

സ്ഥിതം
errcode_t
nt_flush(io_channel channel)
{
	PNT_PRIVATE_DATA NtData = ശൂന്യ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	എങ്കിൽ(NtData->OpenedReadonly)
	{
		തിരികെ 0; // EACCESS;
	}


	//
	// Flush file buffers.
	//

	_FlushDrive(NtData->Handle);


	//
	// Test and correct partition type.
	//

	എങ്കിൽ(NtData->Written)
	{
		_SetPartType(NtData->Handle, 0x83);
	}

	തിരികെ 0;
}


