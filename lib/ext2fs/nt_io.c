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
#ଅନ୍ତର୍ଭାଵ "config.h"
#endif


//
// I need some warnings to disable...
//


#pragma warning(disable:4514) // unreferenced inline function has been removed
#pragma warning(ଠେଲ,4)

#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union)
#pragma warning(disable:4214) // nonstandard extension used : bit field types other than int
#pragma warning(disable:4115) // named type definition in parentheses

#ଅନ୍ତର୍ଭାଵ <ntddk.h>
#ଅନ୍ତର୍ଭାଵ <ntdddisk.h>
#ଅନ୍ତର୍ଭାଵ <ntstatus.h>

#pragma warning(କାଢ଼)


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
    IN PULONG ଚାବି_2 OPTIONAL
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
    IN PULONG ଚାବି_2 OPTIONAL
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
    IN PVOID ନିବେଶ_Buffer OPTIONAL,
    IN ULONG ନିବେଶ_Buffer_Length,
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
    IN PVOID ନିବେଶ_Buffer OPTIONAL,
    IN ULONG ନିବେଶ_Buffer_Length,
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

#ଅନ୍ତର୍ଭାଵ <winerror.h>

//
// standard stuff
//

#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <malloc.h>

#ଅନ୍ତର୍ଭାଵ <linux/types.h>
#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ <errno.h>

#ଅନ୍ତର୍ଭାଵ "et/com_err.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs/ext2fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs/ext2_err.h"




//
// For checking structure magic numbers...
//


#define EXT2_CHECK_MAGIC(ସଂରଚନା, code) \
	  ଯଦି ((ସଂରଚନା)->magic != (code)) ଫେରନ୍ତୁ (code)

#define EXT2_ET_MAGIC_NT_IO_CHANNEL  0x10ed


//
// Private data block
//

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା _NT_PRIVATE_DATA {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	   magic;
	HANDLE Handle;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	   Flags;
	PCHAR  Buffer;
	__u32  Buffer_Block_ସଂଖ୍ୟା;
	ULONG  BufferSize;
	BOOLEAN OpenedReadonly;
	BOOLEAN Written;
}NT_PRIVATE_DATA, *PNT_PRIVATE_DATA;



//
// Standard interface prototypes
//

ସ୍ଥିର errcode_t nt_open(ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, io_channel *channel);
ସ୍ଥିର errcode_t nt_close(io_channel channel);
ସ୍ଥିର errcode_t nt_set_blksize(io_channel channel, ପୂର୍ଣ୍ଣସଂଖ୍ୟା blksize);
ସ୍ଥିର errcode_t nt_read_blk(io_channel channel, unsigned long block,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଶୂନ୍ୟ *data);
ସ୍ଥିର errcode_t nt_write_blk(io_channel channel, unsigned long block,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *data);
ସ୍ଥିର errcode_t nt_flush(io_channel channel);

ସ୍ଥିର ସଂରଚନା struct_io_manager struct_nt_manager = {
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
	ଫେରନ୍ତୁ &struct_nt_manager;
}





//
// This is a code to convert Win32 errors to unix errno
//

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
	ULONG WinError;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା errnocode;
}ERROR_ENTRY;

ସ୍ଥିର ERROR_ENTRY ErrorTable[] = {
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




ସ୍ଥିର
unsigned
_MapDosError (
    IN ULONG WinError
   )
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	//
	// Lookup
	//

	ପାଇଁ (i = 0; i < (ଆକାର(ErrorTable)/ଆକାର(ErrorTable[0])); ++i)
	{
		ଯଦି (WinError == ErrorTable[i].WinError)
		{
			ଫେରନ୍ତୁ ErrorTable[i].errnocode;
		}
	}

	//
	// not in table. Check ranges
	//

	ଯଦି ((WinError >= ERROR_WRITE_PROTECT) &&
		(WinError <= ERROR_SHARING_BUFFER_EXCEEDED))
	{
		ଫେରନ୍ତୁ EACCES;
	}
	ଅନ୍ୟଥା ଯଦି ((WinError >= ERROR_INVALID_STARTING_CODESEG) &&
			 (WinError <= ERROR_INFLOOP_IN_RELOC_CHAIN))
	{
		ଫେରନ୍ତୁ ENOEXEC;
	}
	ଅନ୍ୟଥା
	{
		ଫେରନ୍ତୁ EINVAL;
	}
}







//
// Function to map NT status to dos error.
//

ସ୍ଥିର
__inline
unsigned
_MapNtStatus(
    IN NTSTATUS Status
   )
{
	ଫେରନ୍ତୁ _MapDosError(RtlNtStatusToDosError(Status));
}





//
// Helper functions to make things easier
//

ସ୍ଥିର
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
	UnicodeString.MaximumLength = ଆକାର(Buffer); // in bytes!!!

	RtlInitAnsiString(&AnsiString, Name);

	Status = RtlAnsiStringToUnicodeString(&UnicodeString, &AnsiString, FALSE);

	ଯଦି(!NT_SUCCESS(Status))
	{
		ଫେରନ୍ତୁ Status; // Unmappable character?
	}

	//
	// Initialize object
	//

	InitializeObjectAttributes(&ObjectAttributes,
							   &UnicodeString,
							   OBJ_CASE_INSENSITIVE,
							   ଖାଲି,
							   ଖାଲି );

	//
	// Try to open it in initial mode
	//

	ଯଦି(ARGUMENT_PRESENT(OpenedReadonly))
	{
		*OpenedReadonly = Readonly;
	}


	Status = NtOpenFile(Handle,
						SYNCHRONIZE | FILE_READ_DATA | (Readonly ? 0 : FILE_WRITE_DATA),
						&ObjectAttributes,
						&IoStatusBlock,
						FILE_SHARE_WRITE | FILE_SHARE_READ,
						FILE_SYNCHRONOUS_IO_NONALERT);

	ଯଦି(!NT_SUCCESS(Status))
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

		ଯଦି((STATUS_ACCESS_DENIED == Status) && !Readonly)
		{
			ଯଦି(ARGUMENT_PRESENT(OpenedReadonly))
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

	ଫେରନ୍ତୁ Status;
}


ସ୍ଥିର
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

	ଫେରନ୍ତୁ _OpenNtName(Buffer, ReadOnly, Handle, OpenedReadonly);
}


//
// Flush device
//

ସ୍ଥିର
__inline
NTSTATUS
_FlushDrive(
		IN HANDLE Handle
		)
{
	IO_STATUS_BLOCK IoStatusBlock;
	ଫେରନ୍ତୁ NtFlushBuffersFile(Handle, &IoStatusBlock);
}


//
// lock drive
//

ସ୍ଥିର
__inline
NTSTATUS
_LockDrive(
		IN HANDLE Handle
		)
{
	IO_STATUS_BLOCK IoStatusBlock;
	ଫେରନ୍ତୁ NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_LOCK_VOLUME, 0, 0, 0, 0);
}


//
// unlock drive
//

ସ୍ଥିର
__inline
NTSTATUS
_UnlockDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	ଫେରନ୍ତୁ NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0);
}

ସ୍ଥିର
__inline
NTSTATUS
_DismountDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	ଫେରନ୍ତୁ NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0);
}


//
// is mounted
//

ସ୍ଥିର
__inline
BOOLEAN
_IsMounted(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	NTSTATUS Status;
	Status = NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_IS_VOLUME_MOUNTED, 0, 0, 0, 0);
	ଫେରନ୍ତୁ (BOOLEAN)(STATUS_SUCCESS == Status);
}


ସ୍ଥିର
__inline
NTSTATUS
_CloseDisk(
		IN HANDLE Handle
		)
{
	ଫେରନ୍ତୁ NtClose(Handle);
}




//
// Make NT name from any recognized name
//

ସ୍ଥିର
PCSTR
_NormalizeDeviceName(
    IN PCSTR Device,
    IN PSTR NormalizedDeviceNameBuffer
   )
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା Partition_ସଂଖ୍ୟା = -1;
	UCHAR Disk_ସଂଖ୍ୟା;
	PSTR p;


	//
	// Do not try to parse NT name
	//

	ଯଦି('\\' == *Device)
		ଫେରନ୍ତୁ Device;



	//
	// Strip leading '/dev/' if any
	//

	ଯଦି(('/' == *(Device)) &&
		('d' == *(Device + 1)) &&
		('e' == *(Device + 2)) &&
		('v' == *(Device + 3)) &&
		('/' == *(Device + 4)))
	{
		Device += 5;
	}

	ଯଦି('\0' == *Device)
	{
		ଫେରନ୍ତୁ ଖାଲି;
	}


	//
	// forms: hda[n], fd[n]
	//

	ଯଦି('d' != *(Device + 1))
	{
		ଫେରନ୍ତୁ ଖାଲି;
	}

	ଯଦି('h' == *Device)
	{
		ଯଦି((*(Device + 2) < 'a') || (*(Device + 2) > ('a' + 9)) ||
		   ((*(Device + 3) != '\0') &&
			((*(Device + 4) != '\0') ||
			 ((*(Device + 3) < '0') || (*(Device + 3) > '9'))
			)
		   )
		  )
		{
			ଫେରନ୍ତୁ ଖାଲି;
		}

		Disk_ସଂଖ୍ୟା = (UCHAR)(*(Device + 2) - 'a');

		ଯଦି(*(Device + 3) != '\0')
		{
			Partition_ସଂଖ୍ୟା = (*(Device + 3) - '0');
		}

	}
	ଅନ୍ୟଥା ଯଦି('f' == *Device)
	{
		//
		// 3-d letter should be a digit.
		//

		ଯଦି((*(Device + 3) != '\0') ||
		   (*(Device + 2) < '0') || (*(Device + 2) > '9'))
		{
			ଫେରନ୍ତୁ ଖାଲି;
		}

		Disk_ସଂଖ୍ୟା = (UCHAR)(*(Device + 2) - '0');

	}
	ଅନ୍ୟଥା
	{
		//
		// invalid prefix
		//

		ଫେରନ୍ତୁ ଖାଲି;
	}



	//
	// Prefix
	//

	strcpy(NormalizedDeviceNameBuffer, "\\Device\\");

	//
	// Media name
	//

	ବାଛ(*Device)
	{

	ସ୍ଥିତି 'f':
		strcat(NormalizedDeviceNameBuffer, "Floppy0");
		ଭାଙ୍ଗ;

	ସ୍ଥିତି 'h':
		strcat(NormalizedDeviceNameBuffer, "Harddisk0");
		ଭାଙ୍ଗ;
	}


	p = NormalizedDeviceNameBuffer + strlen(NormalizedDeviceNameBuffer) - 1;
	*p = (CHAR)(*p + Disk_ସଂଖ୍ୟା);


	//
	// Partition nr.
	//

	ଯଦି(Partition_ସଂଖ୍ୟା >= 0)
	{
		strcat(NormalizedDeviceNameBuffer, "\\Partition0");

		p = NormalizedDeviceNameBuffer + strlen(NormalizedDeviceNameBuffer) - 1;
		*p = (CHAR)(*p + Partition_ସଂଖ୍ୟା);
	}


	ଫେରନ୍ତୁ NormalizedDeviceNameBuffer;
}




ସ୍ଥିର
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

	RtlZeroMemory(&pi, ଆକାର(PARTITION_INFORMATION));

	Status = NtDeviceIoControlFile(
		h, ଖାଲି, ଖାଲି, ଖାଲି, &IoStatusBlock, IOCTL_DISK_GET_PARTITION_INFO,
		&pi, ଆକାର(PARTITION_INFORMATION),
		&pi, ଆକାର(PARTITION_INFORMATION));


	ଯଦି(NT_SUCCESS(Status))
	{
		*FsSize = pi.PartitionLength.QuadPart;
	}
	ଅନ୍ୟଥା ଯଦି(STATUS_INVALID_DEVICE_REQUEST == Status)
	{
		//
		// No partitions: get device info.
		//

		RtlZeroMemory(&gi, ଆକାର(DISK_GEOMETRY));

		Status = NtDeviceIoControlFile(
				h, ଖାଲି, ଖାଲି, ଖାଲି, &IoStatusBlock, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, ଆକାର(DISK_GEOMETRY),
				&gi, ଆକାର(DISK_GEOMETRY));


		ଯଦି(NT_SUCCESS(Status))
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

ସ୍ଥିର
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

	ଯଦି(ଖାଲି == Name)
	{
		//
		// Set not found
		//

		ଯଦି(ARGUMENT_PRESENT(Errno))
			*Errno = ENOENT;

		ଫେରନ୍ତୁ FALSE;
	}


	ଯଦି((((*Name) | 0x20) >= 'a') && (((*Name) | 0x20) <= 'z') &&
		(':' == *(Name + 1)) && ('\0' == *(Name + 2)))
	{
		Status = _OpenDriveLetter(*Name, ReadOnly, Handle, OpenedReadonly);
	}
	ଅନ୍ୟଥା
	{
		//
		// Make name
		//

		Name = _NormalizeDeviceName(Name, NormalizedDeviceName);

		ଯଦି(ଖାଲି == Name)
		{
			//
			// Set not found
			//

			ଯଦି(ARGUMENT_PRESENT(Errno))
				*Errno = ENOENT;

			ଫେରନ୍ତୁ FALSE;
		}

		//
		// Try to open it
		//

		Status = _OpenNtName(Name, ReadOnly, Handle, OpenedReadonly);
	}


	ଯଦି(!NT_SUCCESS(Status))
	{
		ଯଦି(ARGUMENT_PRESENT(Errno))
			*Errno = _MapNtStatus(Status);

		ଫେରନ୍ତୁ FALSE;
	}

	ଫେରନ୍ତୁ TRUE;
}


//
// Raw block io. Sets dos errno
//

ସ୍ଥିର
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
	ASSERT(0 == (Offset.ତଳ_Part % 512));


	//
	// perform io
	//

	ଯଦି(Read)
	{
		Status = NtReadFile(Handle, ଖାଲି, ଖାଲି, ଖାଲି,
			&IoStatusBlock, Buffer, Bytes, &Offset, ଖାଲି);
	}
	ଅନ୍ୟଥା
	{
		Status = NtWriteFile(Handle, ଖାଲି, ଖାଲି, ଖାଲି,
			&IoStatusBlock, Buffer, Bytes, &Offset, ଖାଲି);
	}


	//
	// translate error
	//

	ଯଦି(NT_SUCCESS(Status))
	{
		*Errno = 0;
		ଫେରନ୍ତୁ TRUE;
	}

	*Errno = _MapNtStatus(Status);

	ଫେରନ୍ତୁ FALSE;
}



__inline
BOOLEAN
_RawWrite(
    IN HANDLE Handle,
    IN LARGE_INTEGER Offset,
    IN ULONG Bytes,
    OUT ଅଚଳ CHAR* Buffer,
    OUT unsigned* Errno
   )
{
	ଫେରନ୍ତୁ _BlockIo(Handle, Offset, Bytes, (PCHAR)Buffer, FALSE, Errno);
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
	ଫେରନ୍ତୁ _BlockIo(Handle, Offset, Bytes, Buffer, TRUE, Errno);
}



__inline
BOOLEAN
_SetPartType(
    IN HANDLE Handle,
    IN UCHAR Type
   )
{
	IO_STATUS_BLOCK IoStatusBlock;
	ଫେରନ୍ତୁ STATUS_SUCCESS == NtDeviceIoControlFile(
												   Handle, ଖାଲି, ଖାଲି, ଖାଲି, &IoStatusBlock, IOCTL_DISK_SET_PARTITION_INFO,
												   &Type, ଆକାର(Type),
												   ଖାଲି, 0);
}



//--------------------- interface part

//
// Interface functions.
// Is_mounted is set to 1 if the device is mounted, 0 otherwise
//

errcode_t
ext2fs_check_if_mounted(ଅଚଳ ଅକ୍ଷର *file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *mount_flags)
{
	HANDLE h;
	BOOLEAN Readonly;

	*mount_flags = 0;

	ଯଦି(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, ଖାଲି))
	{
		ଫେରନ୍ତୁ 0;
	}


	__try{
		*mount_flags &= _IsMounted(h) ? EXT2_MF_MOUNTED : 0;
	}
	__finally{
		_CloseDisk(h);
	}

	ଫେରନ୍ତୁ 0;
}



//
// Returns the number of blocks in a partition
//

ସ୍ଥିର __int64 FsSize = 0;
ସ୍ଥିର ଅକ୍ଷର knowndevice[1024] = "";


errcode_t
ext2fs_get_device_size(ଅଚଳ ଅକ୍ଷର *file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା blocksize,
				 blk_t *retblocks)
{
	HANDLE h;
	BOOLEAN Readonly;

	ଯଦି((0 == FsSize) || (0 != strcmp(knowndevice, file)))
	{

		ଯଦି(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, ଖାଲି))
		{
			ଫେରନ୍ତୁ 0;
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
	ଫେରନ୍ତୁ 0;
}






//
// Table elements
//


ସ୍ଥିର
errcode_t
nt_open(ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, io_channel *channel)
{
	io_channel      io = ଖାଲି;
	PNT_PRIVATE_DATA NtData = ଖାଲି;
	errcode_t Errno = 0;

	//
	// Check name
	//

	ଯଦି (ଖାଲି == name)
	{
		ଫେରନ୍ତୁ EXT2_ET_BAD_DEVICE_NAME;
	}

	__try{

		//
		// Allocate channel handle
		//

		io = (io_channel) malloc(ଆକାର(ସଂରଚନା struct_io_channel));

		ଯଦି (ଖାଲି == io)
		{
			Errno = ENOMEM;
			__leave;
		}

		RtlZeroMemory(io, ଆକାର(ସଂରଚନା struct_io_channel));
		io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

		NtData = (PNT_PRIVATE_DATA)malloc(ଆକାର(NT_PRIVATE_DATA));

		ଯଦି (ଖାଲି == NtData)
		{
			Errno = ENOMEM;
			__leave;
		}


		io->manager = nt_io_manager();
		io->name = malloc(strlen(name) + 1);
		ଯଦି (ଖାଲି == io->name)
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

		RtlZeroMemory(NtData, ଆକାର(NT_PRIVATE_DATA));

		NtData->magic = EXT2_ET_MAGIC_NT_IO_CHANNEL;
		NtData->Buffer_Block_ସଂଖ୍ୟା = 0xffffffff;
		NtData->BufferSize = 1024;
		NtData->Buffer = malloc(NtData->BufferSize);

		ଯଦି (ଖାଲି == NtData->Buffer)
		{
			Errno = ENOMEM;
			__leave;
		}

		//
		// Open it
		//

		ଯଦି(!_Ext2OpenDevice(name, (BOOLEAN)!BooleanFlagOn(flags, EXT2_FLAG_RW), &NtData->Handle, &NtData->OpenedReadonly, &Errno))
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

		ଯଦି(!NT_SUCCESS(_LockDrive(NtData->Handle)) /*|| !NT_SUCCESS(_DismountDrive(NtData->Handle))*/)
		{
			NtData->OpenedReadonly = TRUE;
		}

		//
		// Done
		//

		*channel = io;


	}
	__finally{

		ଯଦି(0 != Errno)
		{
			//
			// Cleanup
			//

			ଯଦି (ଖାଲି != io)
			{
				ମୁକ୍ତ(io->name);
				ମୁକ୍ତ(io);
			}

			ଯଦି (ଖାଲି != NtData)
			{
				ଯଦି(ଖାଲି != NtData->Handle)
				{
					_UnlockDrive(NtData->Handle);
					_CloseDisk(NtData->Handle);
				}

				ମୁକ୍ତ(NtData->Buffer);
				ମୁକ୍ତ(NtData);
			}
		}
	}

	ଫେରନ୍ତୁ Errno;
}


//
// Close api
//

ସ୍ଥିର
errcode_t
nt_close(io_channel channel)
{
	PNT_PRIVATE_DATA NtData = ଖାଲି;

	ଯଦି(ଖାଲି == channel)
	{
		ଫେରନ୍ତୁ 0;
	}

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	ଯଦି (--channel->refcount > 0)
	{
		ଫେରନ୍ତୁ 0;
	}

	ମୁକ୍ତ(channel->name);
	ମୁକ୍ତ(channel);

	ଯଦି (ଖାଲି != NtData)
	{
		ଯଦି(ଖାଲି != NtData->Handle)
		{
			_DismountDrive(NtData->Handle);
			_UnlockDrive(NtData->Handle);
			_CloseDisk(NtData->Handle);
		}

		ମୁକ୍ତ(NtData->Buffer);
		ମୁକ୍ତ(NtData);
	}

	ଫେରନ୍ତୁ 0;
}



//
// set block size
//

ସ୍ଥିର
errcode_t
nt_set_blksize(io_channel channel, ପୂର୍ଣ୍ଣସଂଖ୍ୟା blksize)
{
	PNT_PRIVATE_DATA NtData = ଖାଲି;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	ଯଦି (channel->block_size != blksize)
	{
		channel->block_size = blksize;

		ମୁକ୍ତ(NtData->Buffer);
		NtData->Buffer_Block_ସଂଖ୍ୟା = 0xffffffff;
		NtData->BufferSize = channel->block_size;
		ASSERT(0 == (NtData->BufferSize % 512));

		NtData->Buffer = malloc(NtData->BufferSize);

		ଯଦି (ଖାଲି == NtData->Buffer)
		{
			ଫେରନ୍ତୁ ENOMEM;
		}

	}

	ଫେରନ୍ତୁ 0;
}


//
// read block
//

ସ୍ଥିର
errcode_t
nt_read_blk(io_channel channel, unsigned long block,
			       ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଶୂନ୍ୟ *buf)
{
	PVOID BufferToRead;
	ULONG SizeToRead;
	ULONG Size;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_DATA NtData = ଖାଲି;
	unsigned Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	//
	// If it's in the cache, use it!
	//

	ଯଦି ((1 == ଗଣନା_2) &&
		(block == NtData->Buffer_Block_ସଂଖ୍ୟା) &&
		(NtData->Buffer_Block_ସଂଖ୍ୟା != 0xffffffff))
	{
		memcpy(buf, NtData->Buffer, channel->block_size);
		ଫେରନ୍ତୁ 0;
	}

	Size = (ଗଣନା_2 < 0) ? (ULONG)(-ଗଣନା_2) : (ULONG)(ଗଣନା_2 * channel->block_size);

	Offset.QuadPart = block * channel->block_size;

	//
	// If not fit to the block
	//

	ଯଦି(Size <= NtData->BufferSize)
	{
		//
		// Update the cache
		//

		NtData->Buffer_Block_ସଂଖ୍ୟା = block;
		BufferToRead = NtData->Buffer;
		SizeToRead = NtData->BufferSize;
	}
	ଅନ୍ୟଥା
	{
		SizeToRead = Size;
		BufferToRead = buf;
		ASSERT(0 == (SizeToRead % channel->block_size));
	}

	ଯଦି(!_RawRead(NtData->Handle, Offset, SizeToRead, BufferToRead, &Errno))
	{

		ଯଦି (channel->read_error)
		{
			ଫେରନ୍ତୁ (channel->read_error)(channel, block, ଗଣନା_2, buf,
					       Size, 0, Errno);
		}
		ଅନ୍ୟଥା
		{
			ଫେରନ୍ତୁ Errno;
		}
	}


	ଯଦି(BufferToRead != buf)
	{
		ASSERT(Size <= SizeToRead);
		memcpy(buf, BufferToRead, Size);
	}

	ଫେରନ୍ତୁ 0;
}


//
// write block
//

ସ୍ଥିର
errcode_t
nt_write_blk(io_channel channel, unsigned long block,
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ଅଚଳ ଶୂନ୍ୟ *buf)
{
	ULONG SizeToWrite;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_DATA NtData = ଖାଲି;
	unsigned Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	ଯଦି(NtData->OpenedReadonly)
	{
		ଫେରନ୍ତୁ EACCES;
	}

	ଯଦି (ଗଣନା_2 == 1)
	{
		SizeToWrite = channel->block_size;
	}
	ଅନ୍ୟଥା
	{
		NtData->Buffer_Block_ସଂଖ୍ୟା = 0xffffffff;

		ଯଦି (ଗଣନା_2 < 0)
		{
			SizeToWrite = (ULONG)(-ଗଣନା_2);
		}
		ଅନ୍ୟଥା
		{
			SizeToWrite = (ULONG)(ଗଣନା_2 * channel->block_size);
		}
	}


	ASSERT(0 == (SizeToWrite % 512));
	Offset.QuadPart = block * channel->block_size;

	ଯଦି(!_RawWrite(NtData->Handle, Offset, SizeToWrite, buf, &Errno))
	{
		ଯଦି (channel->write_error)
		{
			ଫେରନ୍ତୁ (channel->write_error)(channel, block, ଗଣନା_2, buf,
						SizeToWrite, 0, Errno);
		}
		ଅନ୍ୟଥା
		{
			ଫେରନ୍ତୁ Errno;
		}
	}


	//
	// Stash a copy.
	//

	ଯଦି(SizeToWrite >= NtData->BufferSize)
	{
		NtData->Buffer_Block_ସଂଖ୍ୟା = block;
		memcpy(NtData->Buffer, buf, NtData->BufferSize);
	}

	NtData->Written = TRUE;

	ଫେରନ୍ତୁ 0;

}



//
// Flush data buffers to disk.  Since we are currently using a
// write-through cache, this is a no-op.
//

ସ୍ଥିର
errcode_t
nt_flush(io_channel channel)
{
	PNT_PRIVATE_DATA NtData = ଖାଲି;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	ଯଦି(NtData->OpenedReadonly)
	{
		ଫେରନ୍ତୁ 0; // EACCESS;
	}


	//
	// Flush file buffers.
	//

	_FlushDrive(NtData->Handle);


	//
	// Test and correct partition type.
	//

	ଯଦି(NtData->Written)
	{
		_SetPartType(NtData->Handle, 0x83);
	}

	ଫେରନ୍ତୁ 0;
}


