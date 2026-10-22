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
#अन्तर्भाव "config.h"
#endif


//
// I need some warnings to disable...
//


#pragma warning(disable:4514) // unreferenced inline function has been removed
#pragma warning(ढकला,4)

#pragma warning(disable:4201) // nonstandard extension used : nameless struct/union)
#pragma warning(disable:4214) // nonstandard extension used : bit field types other than int
#pragma warning(disable:4115) // named type definition in parentheses

#अन्तर्भाव <ntddk.h>
#अन्तर्भाव <ntdddisk.h>
#अन्तर्भाव <ntstatus.h>

#pragma warning(काढा)


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
    IN PULONG किल्ली_2 OPTIONAL
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
    IN PULONG किल्ली_2 OPTIONAL
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
    IN PVOID आदान_Buffer OPTIONAL,
    IN ULONG आदान_Buffer_Length,
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
    IN PVOID आदान_Buffer OPTIONAL,
    IN ULONG आदान_Buffer_Length,
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

#अन्तर्भाव <winerror.h>

//
// standard stuff
//

#अन्तर्भाव <assert.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <malloc.h>

#अन्तर्भाव <linux/types.h>
#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव <errno.h>

#अन्तर्भाव "et/com_err.h"
#अन्तर्भाव "ext2fs/ext2fs.h"
#अन्तर्भाव "ext2fs/ext2_err.h"




//
// For checking structure magic numbers...
//


#define EXT2_CHECK_MAGIC(संरचना, code) \
	  यदि ((संरचना)->magic != (code)) प्रत्यागम (code)

#define EXT2_ET_MAGIC_NT_IO_CHANNEL  0x10ed


//
// Private data block
//

प्रकारनिर्देश संरचना _NT_PRIVATE_DATA {
	पूर्णाङ्क	   magic;
	HANDLE Handle;
	पूर्णाङ्क	   Flags;
	PCHAR  Buffer;
	__u32  Buffer_Block_संख्या;
	ULONG  BufferSize;
	BOOLEAN OpenedReadonly;
	BOOLEAN Written;
}NT_PRIVATE_DATA, *PNT_PRIVATE_DATA;



//
// Standard interface prototypes
//

स्थिर errcode_t nt_open(ध्रुव वर्ण *name, पूर्णाङ्क flags, io_channel *channel);
स्थिर errcode_t nt_close(io_channel channel);
स्थिर errcode_t nt_set_blksize(io_channel channel, पूर्णाङ्क blksize);
स्थिर errcode_t nt_read_blk(io_channel channel, unsigned long block,
			       पूर्णाङ्क मोजणी, शून्य *data);
स्थिर errcode_t nt_write_blk(io_channel channel, unsigned long block,
				पूर्णाङ्क मोजणी, ध्रुव शून्य *data);
स्थिर errcode_t nt_flush(io_channel channel);

स्थिर संरचना struct_io_manager struct_nt_manager = {
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
	प्रत्यागम &struct_nt_manager;
}





//
// This is a code to convert Win32 errors to unix errno
//

प्रकारनिर्देश संरचना {
	ULONG WinError;
	पूर्णाङ्क errnocode;
}ERROR_ENTRY;

स्थिर ERROR_ENTRY ErrorTable[] = {
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




स्थिर
unsigned
_MapDosError (
    IN ULONG WinError
   )
{
	पूर्णाङ्क i;

	//
	// Lookup
	//

	प्रति (i = 0; i < (परिमाण(ErrorTable)/परिमाण(ErrorTable[0])); ++i)
	{
		यदि (WinError == ErrorTable[i].WinError)
		{
			प्रत्यागम ErrorTable[i].errnocode;
		}
	}

	//
	// not in table. Check ranges
	//

	यदि ((WinError >= ERROR_WRITE_PROTECT) &&
		(WinError <= ERROR_SHARING_BUFFER_EXCEEDED))
	{
		प्रत्यागम EACCES;
	}
	अन्यथा यदि ((WinError >= ERROR_INVALID_STARTING_CODESEG) &&
			 (WinError <= ERROR_INFLOOP_IN_RELOC_CHAIN))
	{
		प्रत्यागम ENOEXEC;
	}
	अन्यथा
	{
		प्रत्यागम EINVAL;
	}
}







//
// Function to map NT status to dos error.
//

स्थिर
__inline
unsigned
_MapNtStatus(
    IN NTSTATUS Status
   )
{
	प्रत्यागम _MapDosError(RtlNtStatusToDosError(Status));
}





//
// Helper functions to make things easier
//

स्थिर
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
	UnicodeString.MaximumLength = परिमाण(Buffer); // in bytes!!!

	RtlInitAnsiString(&AnsiString, Name);

	Status = RtlAnsiStringToUnicodeString(&UnicodeString, &AnsiString, FALSE);

	यदि(!NT_SUCCESS(Status))
	{
		प्रत्यागम Status; // Unmappable character?
	}

	//
	// Initialize object
	//

	InitializeObjectAttributes(&ObjectAttributes,
							   &UnicodeString,
							   OBJ_CASE_INSENSITIVE,
							   रिक्त,
							   रिक्त );

	//
	// Try to open it in initial mode
	//

	यदि(ARGUMENT_PRESENT(OpenedReadonly))
	{
		*OpenedReadonly = Readonly;
	}


	Status = NtOpenFile(Handle,
						SYNCHRONIZE | FILE_READ_DATA | (Readonly ? 0 : FILE_WRITE_DATA),
						&ObjectAttributes,
						&IoStatusBlock,
						FILE_SHARE_WRITE | FILE_SHARE_READ,
						FILE_SYNCHRONOUS_IO_NONALERT);

	यदि(!NT_SUCCESS(Status))
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

		यदि((STATUS_ACCESS_DENIED == Status) && !Readonly)
		{
			यदि(ARGUMENT_PRESENT(OpenedReadonly))
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

	प्रत्यागम Status;
}


स्थिर
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

	प्रत्यागम _OpenNtName(Buffer, ReadOnly, Handle, OpenedReadonly);
}


//
// Flush device
//

स्थिर
__inline
NTSTATUS
_FlushDrive(
		IN HANDLE Handle
		)
{
	IO_STATUS_BLOCK IoStatusBlock;
	प्रत्यागम NtFlushBuffersFile(Handle, &IoStatusBlock);
}


//
// lock drive
//

स्थिर
__inline
NTSTATUS
_LockDrive(
		IN HANDLE Handle
		)
{
	IO_STATUS_BLOCK IoStatusBlock;
	प्रत्यागम NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_LOCK_VOLUME, 0, 0, 0, 0);
}


//
// unlock drive
//

स्थिर
__inline
NTSTATUS
_UnlockDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	प्रत्यागम NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0);
}

स्थिर
__inline
NTSTATUS
_DismountDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	प्रत्यागम NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0);
}


//
// is mounted
//

स्थिर
__inline
BOOLEAN
_IsMounted(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	NTSTATUS Status;
	Status = NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_IS_VOLUME_MOUNTED, 0, 0, 0, 0);
	प्रत्यागम (BOOLEAN)(STATUS_SUCCESS == Status);
}


स्थिर
__inline
NTSTATUS
_CloseDisk(
		IN HANDLE Handle
		)
{
	प्रत्यागम NtClose(Handle);
}




//
// Make NT name from any recognized name
//

स्थिर
PCSTR
_NormalizeDeviceName(
    IN PCSTR Device,
    IN PSTR NormalizedDeviceNameBuffer
   )
{
	पूर्णाङ्क Partition_संख्या = -1;
	UCHAR Disk_संख्या;
	PSTR p;


	//
	// Do not try to parse NT name
	//

	यदि('\\' == *Device)
		प्रत्यागम Device;



	//
	// Strip leading '/dev/' if any
	//

	यदि(('/' == *(Device)) &&
		('d' == *(Device + 1)) &&
		('e' == *(Device + 2)) &&
		('v' == *(Device + 3)) &&
		('/' == *(Device + 4)))
	{
		Device += 5;
	}

	यदि('\0' == *Device)
	{
		प्रत्यागम रिक्त;
	}


	//
	// forms: hda[n], fd[n]
	//

	यदि('d' != *(Device + 1))
	{
		प्रत्यागम रिक्त;
	}

	यदि('h' == *Device)
	{
		यदि((*(Device + 2) < 'a') || (*(Device + 2) > ('a' + 9)) ||
		   ((*(Device + 3) != '\0') &&
			((*(Device + 4) != '\0') ||
			 ((*(Device + 3) < '0') || (*(Device + 3) > '9'))
			)
		   )
		  )
		{
			प्रत्यागम रिक्त;
		}

		Disk_संख्या = (UCHAR)(*(Device + 2) - 'a');

		यदि(*(Device + 3) != '\0')
		{
			Partition_संख्या = (*(Device + 3) - '0');
		}

	}
	अन्यथा यदि('f' == *Device)
	{
		//
		// 3-d letter should be a digit.
		//

		यदि((*(Device + 3) != '\0') ||
		   (*(Device + 2) < '0') || (*(Device + 2) > '9'))
		{
			प्रत्यागम रिक्त;
		}

		Disk_संख्या = (UCHAR)(*(Device + 2) - '0');

	}
	अन्यथा
	{
		//
		// invalid prefix
		//

		प्रत्यागम रिक्त;
	}



	//
	// Prefix
	//

	strcpy(NormalizedDeviceNameBuffer, "\\Device\\");

	//
	// Media name
	//

	विकल्प(*Device)
	{

	पक्ष 'f':
		strcat(NormalizedDeviceNameBuffer, "Floppy0");
		भङ्ग;

	पक्ष 'h':
		strcat(NormalizedDeviceNameBuffer, "Harddisk0");
		भङ्ग;
	}


	p = NormalizedDeviceNameBuffer + strlen(NormalizedDeviceNameBuffer) - 1;
	*p = (CHAR)(*p + Disk_संख्या);


	//
	// Partition nr.
	//

	यदि(Partition_संख्या >= 0)
	{
		strcat(NormalizedDeviceNameBuffer, "\\Partition0");

		p = NormalizedDeviceNameBuffer + strlen(NormalizedDeviceNameBuffer) - 1;
		*p = (CHAR)(*p + Partition_संख्या);
	}


	प्रत्यागम NormalizedDeviceNameBuffer;
}




स्थिर
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

	RtlZeroMemory(&pi, परिमाण(PARTITION_INFORMATION));

	Status = NtDeviceIoControlFile(
		h, रिक्त, रिक्त, रिक्त, &IoStatusBlock, IOCTL_DISK_GET_PARTITION_INFO,
		&pi, परिमाण(PARTITION_INFORMATION),
		&pi, परिमाण(PARTITION_INFORMATION));


	यदि(NT_SUCCESS(Status))
	{
		*FsSize = pi.PartitionLength.QuadPart;
	}
	अन्यथा यदि(STATUS_INVALID_DEVICE_REQUEST == Status)
	{
		//
		// No partitions: get device info.
		//

		RtlZeroMemory(&gi, परिमाण(DISK_GEOMETRY));

		Status = NtDeviceIoControlFile(
				h, रिक्त, रिक्त, रिक्त, &IoStatusBlock, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, परिमाण(DISK_GEOMETRY),
				&gi, परिमाण(DISK_GEOMETRY));


		यदि(NT_SUCCESS(Status))
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

स्थिर
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

	यदि(रिक्त == Name)
	{
		//
		// Set not found
		//

		यदि(ARGUMENT_PRESENT(Errno))
			*Errno = ENOENT;

		प्रत्यागम FALSE;
	}


	यदि((((*Name) | 0x20) >= 'a') && (((*Name) | 0x20) <= 'z') &&
		(':' == *(Name + 1)) && ('\0' == *(Name + 2)))
	{
		Status = _OpenDriveLetter(*Name, ReadOnly, Handle, OpenedReadonly);
	}
	अन्यथा
	{
		//
		// Make name
		//

		Name = _NormalizeDeviceName(Name, NormalizedDeviceName);

		यदि(रिक्त == Name)
		{
			//
			// Set not found
			//

			यदि(ARGUMENT_PRESENT(Errno))
				*Errno = ENOENT;

			प्रत्यागम FALSE;
		}

		//
		// Try to open it
		//

		Status = _OpenNtName(Name, ReadOnly, Handle, OpenedReadonly);
	}


	यदि(!NT_SUCCESS(Status))
	{
		यदि(ARGUMENT_PRESENT(Errno))
			*Errno = _MapNtStatus(Status);

		प्रत्यागम FALSE;
	}

	प्रत्यागम TRUE;
}


//
// Raw block io. Sets dos errno
//

स्थिर
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
	ASSERT(0 == (Offset.खालचा_Part % 512));


	//
	// perform io
	//

	यदि(Read)
	{
		Status = NtReadFile(Handle, रिक्त, रिक्त, रिक्त,
			&IoStatusBlock, Buffer, Bytes, &Offset, रिक्त);
	}
	अन्यथा
	{
		Status = NtWriteFile(Handle, रिक्त, रिक्त, रिक्त,
			&IoStatusBlock, Buffer, Bytes, &Offset, रिक्त);
	}


	//
	// translate error
	//

	यदि(NT_SUCCESS(Status))
	{
		*Errno = 0;
		प्रत्यागम TRUE;
	}

	*Errno = _MapNtStatus(Status);

	प्रत्यागम FALSE;
}



__inline
BOOLEAN
_RawWrite(
    IN HANDLE Handle,
    IN LARGE_INTEGER Offset,
    IN ULONG Bytes,
    OUT ध्रुव CHAR* Buffer,
    OUT unsigned* Errno
   )
{
	प्रत्यागम _BlockIo(Handle, Offset, Bytes, (PCHAR)Buffer, FALSE, Errno);
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
	प्रत्यागम _BlockIo(Handle, Offset, Bytes, Buffer, TRUE, Errno);
}



__inline
BOOLEAN
_SetPartType(
    IN HANDLE Handle,
    IN UCHAR Type
   )
{
	IO_STATUS_BLOCK IoStatusBlock;
	प्रत्यागम STATUS_SUCCESS == NtDeviceIoControlFile(
												   Handle, रिक्त, रिक्त, रिक्त, &IoStatusBlock, IOCTL_DISK_SET_PARTITION_INFO,
												   &Type, परिमाण(Type),
												   रिक्त, 0);
}



//--------------------- interface part

//
// Interface functions.
// Is_mounted is set to 1 if the device is mounted, 0 otherwise
//

errcode_t
ext2fs_check_if_mounted(ध्रुव वर्ण *file, पूर्णाङ्क *mount_flags)
{
	HANDLE h;
	BOOLEAN Readonly;

	*mount_flags = 0;

	यदि(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, रिक्त))
	{
		प्रत्यागम 0;
	}


	__try{
		*mount_flags &= _IsMounted(h) ? EXT2_MF_MOUNTED : 0;
	}
	__finally{
		_CloseDisk(h);
	}

	प्रत्यागम 0;
}



//
// Returns the number of blocks in a partition
//

स्थिर __int64 FsSize = 0;
स्थिर वर्ण knowndevice[1024] = "";


errcode_t
ext2fs_get_device_size(ध्रुव वर्ण *file, पूर्णाङ्क blocksize,
				 blk_t *retblocks)
{
	HANDLE h;
	BOOLEAN Readonly;

	यदि((0 == FsSize) || (0 != strcmp(knowndevice, file)))
	{

		यदि(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, रिक्त))
		{
			प्रत्यागम 0;
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
	प्रत्यागम 0;
}






//
// Table elements
//


स्थिर
errcode_t
nt_open(ध्रुव वर्ण *name, पूर्णाङ्क flags, io_channel *channel)
{
	io_channel      io = रिक्त;
	PNT_PRIVATE_DATA NtData = रिक्त;
	errcode_t Errno = 0;

	//
	// Check name
	//

	यदि (रिक्त == name)
	{
		प्रत्यागम EXT2_ET_BAD_DEVICE_NAME;
	}

	__try{

		//
		// Allocate channel handle
		//

		io = (io_channel) malloc(परिमाण(संरचना struct_io_channel));

		यदि (रिक्त == io)
		{
			Errno = ENOMEM;
			__leave;
		}

		RtlZeroMemory(io, परिमाण(संरचना struct_io_channel));
		io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

		NtData = (PNT_PRIVATE_DATA)malloc(परिमाण(NT_PRIVATE_DATA));

		यदि (रिक्त == NtData)
		{
			Errno = ENOMEM;
			__leave;
		}


		io->manager = nt_io_manager();
		io->name = malloc(strlen(name) + 1);
		यदि (रिक्त == io->name)
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

		RtlZeroMemory(NtData, परिमाण(NT_PRIVATE_DATA));

		NtData->magic = EXT2_ET_MAGIC_NT_IO_CHANNEL;
		NtData->Buffer_Block_संख्या = 0xffffffff;
		NtData->BufferSize = 1024;
		NtData->Buffer = malloc(NtData->BufferSize);

		यदि (रिक्त == NtData->Buffer)
		{
			Errno = ENOMEM;
			__leave;
		}

		//
		// Open it
		//

		यदि(!_Ext2OpenDevice(name, (BOOLEAN)!BooleanFlagOn(flags, EXT2_FLAG_RW), &NtData->Handle, &NtData->OpenedReadonly, &Errno))
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

		यदि(!NT_SUCCESS(_LockDrive(NtData->Handle)) /*|| !NT_SUCCESS(_DismountDrive(NtData->Handle))*/)
		{
			NtData->OpenedReadonly = TRUE;
		}

		//
		// Done
		//

		*channel = io;


	}
	__finally{

		यदि(0 != Errno)
		{
			//
			// Cleanup
			//

			यदि (रिक्त != io)
			{
				मुक्त(io->name);
				मुक्त(io);
			}

			यदि (रिक्त != NtData)
			{
				यदि(रिक्त != NtData->Handle)
				{
					_UnlockDrive(NtData->Handle);
					_CloseDisk(NtData->Handle);
				}

				मुक्त(NtData->Buffer);
				मुक्त(NtData);
			}
		}
	}

	प्रत्यागम Errno;
}


//
// Close api
//

स्थिर
errcode_t
nt_close(io_channel channel)
{
	PNT_PRIVATE_DATA NtData = रिक्त;

	यदि(रिक्त == channel)
	{
		प्रत्यागम 0;
	}

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	यदि (--channel->refcount > 0)
	{
		प्रत्यागम 0;
	}

	मुक्त(channel->name);
	मुक्त(channel);

	यदि (रिक्त != NtData)
	{
		यदि(रिक्त != NtData->Handle)
		{
			_DismountDrive(NtData->Handle);
			_UnlockDrive(NtData->Handle);
			_CloseDisk(NtData->Handle);
		}

		मुक्त(NtData->Buffer);
		मुक्त(NtData);
	}

	प्रत्यागम 0;
}



//
// set block size
//

स्थिर
errcode_t
nt_set_blksize(io_channel channel, पूर्णाङ्क blksize)
{
	PNT_PRIVATE_DATA NtData = रिक्त;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	यदि (channel->block_size != blksize)
	{
		channel->block_size = blksize;

		मुक्त(NtData->Buffer);
		NtData->Buffer_Block_संख्या = 0xffffffff;
		NtData->BufferSize = channel->block_size;
		ASSERT(0 == (NtData->BufferSize % 512));

		NtData->Buffer = malloc(NtData->BufferSize);

		यदि (रिक्त == NtData->Buffer)
		{
			प्रत्यागम ENOMEM;
		}

	}

	प्रत्यागम 0;
}


//
// read block
//

स्थिर
errcode_t
nt_read_blk(io_channel channel, unsigned long block,
			       पूर्णाङ्क मोजणी, शून्य *buf)
{
	PVOID BufferToRead;
	ULONG SizeToRead;
	ULONG Size;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_DATA NtData = रिक्त;
	unsigned Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	//
	// If it's in the cache, use it!
	//

	यदि ((1 == मोजणी) &&
		(block == NtData->Buffer_Block_संख्या) &&
		(NtData->Buffer_Block_संख्या != 0xffffffff))
	{
		memcpy(buf, NtData->Buffer, channel->block_size);
		प्रत्यागम 0;
	}

	Size = (मोजणी < 0) ? (ULONG)(-मोजणी) : (ULONG)(मोजणी * channel->block_size);

	Offset.QuadPart = block * channel->block_size;

	//
	// If not fit to the block
	//

	यदि(Size <= NtData->BufferSize)
	{
		//
		// Update the cache
		//

		NtData->Buffer_Block_संख्या = block;
		BufferToRead = NtData->Buffer;
		SizeToRead = NtData->BufferSize;
	}
	अन्यथा
	{
		SizeToRead = Size;
		BufferToRead = buf;
		ASSERT(0 == (SizeToRead % channel->block_size));
	}

	यदि(!_RawRead(NtData->Handle, Offset, SizeToRead, BufferToRead, &Errno))
	{

		यदि (channel->read_error)
		{
			प्रत्यागम (channel->read_error)(channel, block, मोजणी, buf,
					       Size, 0, Errno);
		}
		अन्यथा
		{
			प्रत्यागम Errno;
		}
	}


	यदि(BufferToRead != buf)
	{
		ASSERT(Size <= SizeToRead);
		memcpy(buf, BufferToRead, Size);
	}

	प्रत्यागम 0;
}


//
// write block
//

स्थिर
errcode_t
nt_write_blk(io_channel channel, unsigned long block,
				पूर्णाङ्क मोजणी, ध्रुव शून्य *buf)
{
	ULONG SizeToWrite;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_DATA NtData = रिक्त;
	unsigned Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	यदि(NtData->OpenedReadonly)
	{
		प्रत्यागम EACCES;
	}

	यदि (मोजणी == 1)
	{
		SizeToWrite = channel->block_size;
	}
	अन्यथा
	{
		NtData->Buffer_Block_संख्या = 0xffffffff;

		यदि (मोजणी < 0)
		{
			SizeToWrite = (ULONG)(-मोजणी);
		}
		अन्यथा
		{
			SizeToWrite = (ULONG)(मोजणी * channel->block_size);
		}
	}


	ASSERT(0 == (SizeToWrite % 512));
	Offset.QuadPart = block * channel->block_size;

	यदि(!_RawWrite(NtData->Handle, Offset, SizeToWrite, buf, &Errno))
	{
		यदि (channel->write_error)
		{
			प्रत्यागम (channel->write_error)(channel, block, मोजणी, buf,
						SizeToWrite, 0, Errno);
		}
		अन्यथा
		{
			प्रत्यागम Errno;
		}
	}


	//
	// Stash a copy.
	//

	यदि(SizeToWrite >= NtData->BufferSize)
	{
		NtData->Buffer_Block_संख्या = block;
		memcpy(NtData->Buffer, buf, NtData->BufferSize);
	}

	NtData->Written = TRUE;

	प्रत्यागम 0;

}



//
// Flush data buffers to disk.  Since we are currently using a
// write-through cache, this is a no-op.
//

स्थिर
errcode_t
nt_flush(io_channel channel)
{
	PNT_PRIVATE_DATA NtData = रिक्त;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	NtData = (PNT_PRIVATE_DATA) channel->private_data;
	EXT2_CHECK_MAGIC(NtData, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	यदि(NtData->OpenedReadonly)
	{
		प्रत्यागम 0; // EACCESS;
	}


	//
	// Flush file buffers.
	//

	_FlushDrive(NtData->Handle);


	//
	// Test and correct partition type.
	//

	यदि(NtData->Written)
	{
		_SetPartType(NtData->Handle, 0x83);
	}

	प्रत्यागम 0;
}


