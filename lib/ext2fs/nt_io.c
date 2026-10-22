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

#अगर_घोषित HAVE_CONFIG_H
#समावेश "config.h"
#पूर्ण_अगर


//
// I need some warnings to disable...
//


#आशय warning(disable:4514) // unreferenced inline function has been removed
#आशय warning(धकेलो,4)

#आशय warning(disable:4201) // nonstandard extension used : nameless struct/union)
#आशय warning(disable:4214) // nonstandard extension used : bit field types other than int
#आशय warning(disable:4115) // named type definition in parentheses

#समावेश <ntddk.h>
#समावेश <ntdddisk.h>
#समावेश <ntstatus.h>

#आशय warning(निकालो)


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
    OUT PVOID बफर_2,
    IN ULONG लंबाई_2,
    IN PLARGE_INTEGER ByteOffset OPTIONAL,
    IN PULONG कुंजी_2 OPTIONAL
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
    IN PVOID बफर_2,
    IN ULONG लंबाई_2,
    IN PLARGE_INTEGER ByteOffset OPTIONAL,
    IN PULONG कुंजी_2 OPTIONAL
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
    IN PVOID निवेश_बफर OPTIONAL,
    IN ULONG निवेश_बफर_लंबाई,
    OUT PVOID Output_बफर OPTIONAL,
    IN ULONG Output_बफर_लंबाई
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
    IN PVOID निवेश_बफर OPTIONAL,
    IN ULONG निवेश_बफर_लंबाई,
    OUT PVOID Output_बफर OPTIONAL,
    IN ULONG Output_बफर_लंबाई
    );


NTSYSAPI
NTSTATUS
NTAPI
NtDelayExecution(
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER Interval
    );


#घोषणा FSCTL_LOCK_VOLUME               CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 6, METHOD_BUFFERED, FILE_ANY_ACCESS)
#घोषणा FSCTL_UNLOCK_VOLUME             CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 7, METHOD_BUFFERED, FILE_ANY_ACCESS)
#घोषणा FSCTL_DISMOUNT_VOLUME           CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 8, METHOD_BUFFERED, FILE_ANY_ACCESS)
#घोषणा FSCTL_IS_VOLUME_MOUNTED         CTL_CODE(FILE_DEVICE_FILE_SYSTEM,10, METHOD_BUFFERED, FILE_ANY_ACCESS)


//
// useful macros
//

#घोषणा BooleanFlagOn(Flags,SingleFlag) ((BOOLEAN)((((Flags) & (SingleFlag)) != 0)))


//
// Include Win32 error codes.
//

#समावेश <winerror.h>

//
// standard stuff
//

#समावेश <assert.h>
#समावेश <stdio.h>
#समावेश <string.h>
#समावेश <stdlib.h>
#समावेश <malloc.h>

#समावेश <linux/types.h>
#समावेश "ext2_fs.h"
#समावेश <errno.h>

#समावेश "et/com_err.h"
#समावेश "ext2fs/ext2fs.h"
#समावेश "ext2fs/ext2_err.h"




//
// For checking structure magic numbers...
//


#घोषणा EXT2_CHECK_MAGIC(काष्ठा, code) \
	  अगर ((काष्ठा)->magic != (code)) वापस (code)

#घोषणा EXT2_ET_MAGIC_NT_IO_CHANNEL  0x10ed


//
// Private data block
//

प्रकार काष्ठा NT_PRIVATE_तथ्य {
	पूर्णांक	   magic;
	HANDLE Handle;
	पूर्णांक	   Flags;
	PCHAR  बफर_2;
	__u32  बफर_Block_संख्या;
	ULONG  बफर_आकार;
	BOOLEAN OpenedReadonly;
	BOOLEAN Written;
}NT_PRIVATE_तथ्य_2, *PNT_PRIVATE_तथ्य;



//
// Standard interface prototypes
//

अटल errcode_t nt_open(स्थिर अक्षर *name, पूर्णांक flags, io_channel *channel);
अटल errcode_t nt_close(io_channel channel);
अटल errcode_t nt_set_blksize(io_channel channel, पूर्णांक blksize);
अटल errcode_t nt_read_blk(io_channel channel, अचिन्हित दीर्घ block,
			       पूर्णांक गिनती, व्योम *तथ्य);
अटल errcode_t nt_write_blk(io_channel channel, अचिन्हित दीर्घ block,
				पूर्णांक गिनती, स्थिर व्योम *तथ्य);
अटल errcode_t nt_flush(io_channel channel);

अटल काष्ठा struct_io_manager struct_nt_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "NT I/O Manager",
	.खोलो		= nt_open,
	.बंद		= nt_close,
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
	वापस &struct_nt_manager;
}





//
// This is a code to convert Win32 errors to unix errno
//

प्रकार काष्ठा {
	ULONG WinError;
	पूर्णांक errnocode;
}ERROR_ENTRY;

अटल ERROR_ENTRY ErrorTable[] = {
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
        {  ERROR_INVALID_तथ्य,           EINVAL    },
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




अटल
अचिन्हित
_MapDosError (
    IN ULONG WinError
   )
{
	पूर्णांक i;

	//
	// Lookup
	//

	क्रम (i = 0; i < (माप(ErrorTable)/माप(ErrorTable[0])); ++i)
	{
		अगर (WinError == ErrorTable[i].WinError)
		{
			वापस ErrorTable[i].errnocode;
		}
	}

	//
	// not in table. Check ranges
	//

	अगर ((WinError >= ERROR_WRITE_PROTECT) &&
		(WinError <= ERROR_SHARING_बफर_EXCEEDED))
	{
		वापस EACCES;
	}
	अन्यथा अगर ((WinError >= ERROR_INVALID_STARTING_CODESEG) &&
			 (WinError <= ERROR_INFLOOP_IN_RELOC_CHAIN))
	{
		वापस ENOEXEC;
	}
	अन्यथा
	{
		वापस EINVAL;
	}
}







//
// Function to map NT status to dos error.
//

अटल
__inline
अचिन्हित
_MapNtStatus(
    IN NTSTATUS Status
   )
{
	वापस _MapDosError(RtlNtStatusToDosError(Status));
}





//
// Helper functions to make things easier
//

अटल
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
	WCHAR बफर_2[512];
	NTSTATUS Status;
	OBJECT_ATTRIBUTES ObjectAttributes;
	IO_STATUS_BLOCK IoStatusBlock;

	//
	// Make Unicode name from input string
	//

	UnicodeString.बफर_2 = &बफर_2[0];
	UnicodeString.लंबाई_2 = 0;
	UnicodeString.Maximum_लंबाई = माप(बफर_2); // in bytes!!!

	RtlInitAnsiString(&AnsiString, Name);

	Status = RtlAnsiStringToUnicodeString(&UnicodeString, &AnsiString, FALSE);

	अगर(!NT_SUCCESS(Status))
	{
		वापस Status; // Unmappable character?
	}

	//
	// Initialize object
	//

	InitializeObjectAttributes(&ObjectAttributes,
							   &UnicodeString,
							   OBJ_CASE_INSENSITIVE,
							   शून्य,
							   शून्य );

	//
	// Try to open it in initial mode
	//

	अगर(ARGUMENT_PRESENT(OpenedReadonly))
	{
		*OpenedReadonly = Readonly;
	}


	Status = NtOpenFile(Handle,
						SYNCHRONIZE | FILE_READ_तथ्य | (Readonly ? 0 : FILE_WRITE_तथ्य),
						&ObjectAttributes,
						&IoStatusBlock,
						FILE_SHARE_WRITE | FILE_SHARE_READ,
						FILE_SYNCHRONOUS_IO_NONALERT);

	अगर(!NT_SUCCESS(Status))
	{
		//
		// Maybe was just mounted? wait 0.5 sec and retry.
		//

		LARGE_INTEGER Interval;
		Interval.QuadPart = -5000000; // 0.5 sec. from now

		NtDelayExecution(FALSE, &Interval);

		Status = NtOpenFile(Handle,
							SYNCHRONIZE | FILE_READ_तथ्य | (Readonly ? 0 : FILE_WRITE_तथ्य),
							&ObjectAttributes,
							&IoStatusBlock,
							FILE_SHARE_WRITE | FILE_SHARE_READ,
							FILE_SYNCHRONOUS_IO_NONALERT);

		//
		// Try to satisfy mode
		//

		अगर((STATUS_ACCESS_DENIED == Status) && !Readonly)
		{
			अगर(ARGUMENT_PRESENT(OpenedReadonly))
			{
				*OpenedReadonly = TRUE;
			}

			Status = NtOpenFile(Handle,
							SYNCHRONIZE | FILE_READ_तथ्य,
							&ObjectAttributes,
							&IoStatusBlock,
							FILE_SHARE_WRITE | FILE_SHARE_READ,
							FILE_SYNCHRONOUS_IO_NONALERT);
		}
	}



	//
	// done
	//

	वापस Status;
}


अटल
NTSTATUS
_OpenDriveLetter(
    IN CHAR Letter,
    IN BOOLEAN ReadOnly,
    OUT PHANDLE Handle,
    OUT PBOOLEAN OpenedReadonly OPTIONAL
   )
{
	CHAR बफर_2[100];

	प्र_लिखो(बफर_2, "\\DosDevices\\%c:", Letter);

	वापस _OpenNtName(बफर_2, ReadOnly, Handle, OpenedReadonly);
}


//
// Flush device
//

अटल
__inline
NTSTATUS
_FlushDrive(
		IN HANDLE Handle
		)
{
	IO_STATUS_BLOCK IoStatusBlock;
	वापस NtFlushBuffersFile(Handle, &IoStatusBlock);
}


//
// lock drive
//

अटल
__inline
NTSTATUS
_LockDrive(
		IN HANDLE Handle
		)
{
	IO_STATUS_BLOCK IoStatusBlock;
	वापस NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_LOCK_VOLUME, 0, 0, 0, 0);
}


//
// unlock drive
//

अटल
__inline
NTSTATUS
_UnlockDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	वापस NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0);
}

अटल
__inline
NTSTATUS
_DismountDrive(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	वापस NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0);
}


//
// is mounted
//

अटल
__inline
BOOLEAN
_IsMounted(
	IN HANDLE Handle
	)
{
	IO_STATUS_BLOCK IoStatusBlock;
	NTSTATUS Status;
	Status = NtFsControlFile(Handle, 0, 0, 0, &IoStatusBlock, FSCTL_IS_VOLUME_MOUNTED, 0, 0, 0, 0);
	वापस (BOOLEAN)(STATUS_SUCCESS == Status);
}


अटल
__inline
NTSTATUS
_CloseDisk(
		IN HANDLE Handle
		)
{
	वापस NtClose(Handle);
}




//
// Make NT name from any recognized name
//

अटल
PCSTR
_NormalizeDeviceName(
    IN PCSTR Device,
    IN PSTR Normalized_Device_Name_बफर
   )
{
	पूर्णांक Partition_संख्या = -1;
	UCHAR Disk_संख्या;
	PSTR p;


	//
	// Do not try to parse NT name
	//

	अगर('\\' == *Device)
		वापस Device;



	//
	// Strip leading '/dev/' if any
	//

	अगर(('/' == *(Device)) &&
		('d' == *(Device + 1)) &&
		('e' == *(Device + 2)) &&
		('v' == *(Device + 3)) &&
		('/' == *(Device + 4)))
	{
		Device += 5;
	}

	अगर('\0' == *Device)
	{
		वापस शून्य;
	}


	//
	// forms: hda[n], fd[n]
	//

	अगर('d' != *(Device + 1))
	{
		वापस शून्य;
	}

	अगर('h' == *Device)
	{
		अगर((*(Device + 2) < 'a') || (*(Device + 2) > ('a' + 9)) ||
		   ((*(Device + 3) != '\0') &&
			((*(Device + 4) != '\0') ||
			 ((*(Device + 3) < '0') || (*(Device + 3) > '9'))
			)
		   )
		  )
		{
			वापस शून्य;
		}

		Disk_संख्या = (UCHAR)(*(Device + 2) - 'a');

		अगर(*(Device + 3) != '\0')
		{
			Partition_संख्या = (*(Device + 3) - '0');
		}

	}
	अन्यथा अगर('f' == *Device)
	{
		//
		// 3-d letter should be a digit.
		//

		अगर((*(Device + 3) != '\0') ||
		   (*(Device + 2) < '0') || (*(Device + 2) > '9'))
		{
			वापस शून्य;
		}

		Disk_संख्या = (UCHAR)(*(Device + 2) - '0');

	}
	अन्यथा
	{
		//
		// invalid prefix
		//

		वापस शून्य;
	}



	//
	// Prefix
	//

	म_नकल(Normalized_Device_Name_बफर, "\\Device\\");

	//
	// Media name
	//

	चयन(*Device)
	{

	हाल 'f':
		म_जोड़ो(Normalized_Device_Name_बफर, "Floppy0");
		अवरोध;

	हाल 'h':
		म_जोड़ो(Normalized_Device_Name_बफर, "Harddisk0");
		अवरोध;
	}


	p = Normalized_Device_Name_बफर + म_माप(Normalized_Device_Name_बफर) - 1;
	*p = (CHAR)(*p + Disk_संख्या);


	//
	// Partition nr.
	//

	अगर(Partition_संख्या >= 0)
	{
		म_जोड़ो(Normalized_Device_Name_बफर, "\\Partition0");

		p = Normalized_Device_Name_बफर + म_माप(Normalized_Device_Name_बफर) - 1;
		*p = (CHAR)(*p + Partition_संख्या);
	}


	वापस Normalized_Device_Name_बफर;
}




अटल
VOID
Get_Device_आकार(
    IN HANDLE h,
    OUT अचिन्हित __int64 *Fs_आकार
   )
{
	PARTITION_INFORMATION pi;
	DISK_GEOMETRY gi;
	NTSTATUS Status;
	IO_STATUS_BLOCK IoStatusBlock;

	//
	// Zero it
	//

	*Fs_आकार = 0;

	//
	// Call driver
	//

	RtlZeroMemory(&pi, माप(PARTITION_INFORMATION));

	Status = NtDeviceIoControlFile(
		h, शून्य, शून्य, शून्य, &IoStatusBlock, IOCTL_DISK_GET_PARTITION_INFO,
		&pi, माप(PARTITION_INFORMATION),
		&pi, माप(PARTITION_INFORMATION));


	अगर(NT_SUCCESS(Status))
	{
		*Fs_आकार = pi.Partition_लंबाई.QuadPart;
	}
	अन्यथा अगर(STATUS_INVALID_DEVICE_REQUEST == Status)
	{
		//
		// No partitions: get device info.
		//

		RtlZeroMemory(&gi, माप(DISK_GEOMETRY));

		Status = NtDeviceIoControlFile(
				h, शून्य, शून्य, शून्य, &IoStatusBlock, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, माप(DISK_GEOMETRY),
				&gi, माप(DISK_GEOMETRY));


		अगर(NT_SUCCESS(Status))
		{
			*Fs_आकार =
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

अटल
BOOLEAN
_Ext2OpenDevice(
    IN PCSTR Name,
    IN BOOLEAN ReadOnly,
    OUT PHANDLE Handle,
    OUT PBOOLEAN OpenedReadonly OPTIONAL,
    OUT अचिन्हित *Errno OPTIONAL
   )
{
	CHAR NormalizedDeviceName[512];
	NTSTATUS Status;

	अगर(शून्य == Name)
	{
		//
		// Set not found
		//

		अगर(ARGUMENT_PRESENT(Errno))
			*Errno = ENOENT;

		वापस FALSE;
	}


	अगर((((*Name) | 0x20) >= 'a') && (((*Name) | 0x20) <= 'z') &&
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

		अगर(शून्य == Name)
		{
			//
			// Set not found
			//

			अगर(ARGUMENT_PRESENT(Errno))
				*Errno = ENOENT;

			वापस FALSE;
		}

		//
		// Try to open it
		//

		Status = _OpenNtName(Name, ReadOnly, Handle, OpenedReadonly);
	}


	अगर(!NT_SUCCESS(Status))
	{
		अगर(ARGUMENT_PRESENT(Errno))
			*Errno = _MapNtStatus(Status);

		वापस FALSE;
	}

	वापस TRUE;
}


//
// Raw block io. Sets dos errno
//

अटल
BOOLEAN
_BlockIo(
    IN HANDLE Handle,
    IN LARGE_INTEGER Offset,
    IN ULONG Bytes,
    IN OUT PCHAR बफर_2,
    IN BOOLEAN Read,
    OUT अचिन्हित* Errno
   )
{
	IO_STATUS_BLOCK IoStatusBlock;
	NTSTATUS Status;

	//
	// Should be aligned
	//

	ASSERT(0 == (Bytes % 512));
	ASSERT(0 == (Offset.निचला_Part % 512));


	//
	// perform io
	//

	अगर(Read)
	{
		Status = NtReadFile(Handle, शून्य, शून्य, शून्य,
			&IoStatusBlock, बफर_2, Bytes, &Offset, शून्य);
	}
	अन्यथा
	{
		Status = NtWriteFile(Handle, शून्य, शून्य, शून्य,
			&IoStatusBlock, बफर_2, Bytes, &Offset, शून्य);
	}


	//
	// translate error
	//

	अगर(NT_SUCCESS(Status))
	{
		*Errno = 0;
		वापस TRUE;
	}

	*Errno = _MapNtStatus(Status);

	वापस FALSE;
}



__inline
BOOLEAN
_RawWrite(
    IN HANDLE Handle,
    IN LARGE_INTEGER Offset,
    IN ULONG Bytes,
    OUT स्थिर CHAR* बफर_2,
    OUT अचिन्हित* Errno
   )
{
	वापस _BlockIo(Handle, Offset, Bytes, (PCHAR)बफर_2, FALSE, Errno);
}

__inline
BOOLEAN
_RawRead(
    IN HANDLE Handle,
    IN LARGE_INTEGER Offset,
    IN ULONG Bytes,
    IN PCHAR बफर_2,
    OUT अचिन्हित* Errno
   )
{
	वापस _BlockIo(Handle, Offset, Bytes, बफर_2, TRUE, Errno);
}



__inline
BOOLEAN
_SetPartType(
    IN HANDLE Handle,
    IN UCHAR Type
   )
{
	IO_STATUS_BLOCK IoStatusBlock;
	वापस STATUS_SUCCESS == NtDeviceIoControlFile(
												   Handle, शून्य, शून्य, शून्य, &IoStatusBlock, IOCTL_DISK_SET_PARTITION_INFO,
												   &Type, माप(Type),
												   शून्य, 0);
}



//--------------------- interface part

//
// Interface functions.
// Is_mounted is set to 1 if the device is mounted, 0 otherwise
//

errcode_t
ext2fs_check_if_mounted(स्थिर अक्षर *file, पूर्णांक *mount_flags)
{
	HANDLE h;
	BOOLEAN Readonly;

	*mount_flags = 0;

	अगर(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, शून्य))
	{
		वापस 0;
	}


	__try{
		*mount_flags &= _IsMounted(h) ? EXT2_MF_MOUNTED : 0;
	}
	__finally{
		_CloseDisk(h);
	}

	वापस 0;
}



//
// Returns the number of blocks in a partition
//

अटल __int64 Fs_आकार = 0;
अटल अक्षर knowndevice[1024] = "";


errcode_t
ext2fs_get_device_आकार(स्थिर अक्षर *file, पूर्णांक blocksize,
				 blk_t *retblocks)
{
	HANDLE h;
	BOOLEAN Readonly;

	अगर((0 == Fs_आकार) || (0 != म_भेद(knowndevice, file)))
	{

		अगर(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, शून्य))
		{
			वापस 0;
		}


		__try{

			//
			// Get size
			//

			Get_Device_आकार(h, &Fs_आकार);
			म_नकल(knowndevice, file);
		}
		__finally{
			_CloseDisk(h);
		}

	}

	*retblocks = (blk_t)(अचिन्हित __int64)(Fs_आकार / blocksize);
	UNREFERENCED_PARAMETER(file);
	वापस 0;
}






//
// Table elements
//


अटल
errcode_t
nt_open(स्थिर अक्षर *name, पूर्णांक flags, io_channel *channel)
{
	io_channel      io = शून्य;
	PNT_PRIVATE_तथ्य Nt_तथ्य = शून्य;
	errcode_t Errno = 0;

	//
	// Check name
	//

	अगर (शून्य == name)
	{
		वापस EXT2_ET_BAD_DEVICE_NAME;
	}

	__try{

		//
		// Allocate channel handle
		//

		io = (io_channel) दो_स्मृति(माप(काष्ठा struct_io_channel));

		अगर (शून्य == io)
		{
			Errno = ENOMEM;
			__leave;
		}

		RtlZeroMemory(io, माप(काष्ठा struct_io_channel));
		io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

		Nt_तथ्य = (PNT_PRIVATE_तथ्य)दो_स्मृति(माप(NT_PRIVATE_तथ्य_2));

		अगर (शून्य == Nt_तथ्य)
		{
			Errno = ENOMEM;
			__leave;
		}


		io->manager = nt_io_manager();
		io->name = दो_स्मृति(म_माप(name) + 1);
		अगर (शून्य == io->name)
		{
			Errno = ENOMEM;
			__leave;
		}

		म_नकल(io->name, name);
		io->private_तथ्य = Nt_तथ्य;
		io->block_आकार = 1024;
		io->read_error = 0;
		io->write_error = 0;
		io->refcount = 1;

		//
		// Initialize data
		//

		RtlZeroMemory(Nt_तथ्य, माप(NT_PRIVATE_तथ्य_2));

		Nt_तथ्य->magic = EXT2_ET_MAGIC_NT_IO_CHANNEL;
		Nt_तथ्य->बफर_Block_संख्या = 0xffffffff;
		Nt_तथ्य->बफर_आकार = 1024;
		Nt_तथ्य->बफर_2 = दो_स्मृति(Nt_तथ्य->बफर_आकार);

		अगर (शून्य == Nt_तथ्य->बफर_2)
		{
			Errno = ENOMEM;
			__leave;
		}

		//
		// Open it
		//

		अगर(!_Ext2OpenDevice(name, (BOOLEAN)!BooleanFlagOn(flags, EXT2_FLAG_RW), &Nt_तथ्य->Handle, &Nt_तथ्य->OpenedReadonly, &Errno))
		{
			__leave;
		}


		//
		// get size
		//

		Get_Device_आकार(Nt_तथ्य->Handle, &Fs_आकार);
		म_नकल(knowndevice, name);


		//
		// Lock/dismount
		//

		अगर(!NT_SUCCESS(_LockDrive(Nt_तथ्य->Handle)) /*|| !NT_SUCCESS(_DismountDrive(NtData->Handle))*/)
		{
			Nt_तथ्य->OpenedReadonly = TRUE;
		}

		//
		// Done
		//

		*channel = io;


	}
	__finally{

		अगर(0 != Errno)
		{
			//
			// Cleanup
			//

			अगर (शून्य != io)
			{
				मुक्त(io->name);
				मुक्त(io);
			}

			अगर (शून्य != Nt_तथ्य)
			{
				अगर(शून्य != Nt_तथ्य->Handle)
				{
					_UnlockDrive(Nt_तथ्य->Handle);
					_CloseDisk(Nt_तथ्य->Handle);
				}

				मुक्त(Nt_तथ्य->बफर_2);
				मुक्त(Nt_तथ्य);
			}
		}
	}

	वापस Errno;
}


//
// Close api
//

अटल
errcode_t
nt_close(io_channel channel)
{
	PNT_PRIVATE_तथ्य Nt_तथ्य = शून्य;

	अगर(शून्य == channel)
	{
		वापस 0;
	}

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_तथ्य = (PNT_PRIVATE_तथ्य) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(Nt_तथ्य, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	अगर (--channel->refcount > 0)
	{
		वापस 0;
	}

	मुक्त(channel->name);
	मुक्त(channel);

	अगर (शून्य != Nt_तथ्य)
	{
		अगर(शून्य != Nt_तथ्य->Handle)
		{
			_DismountDrive(Nt_तथ्य->Handle);
			_UnlockDrive(Nt_तथ्य->Handle);
			_CloseDisk(Nt_तथ्य->Handle);
		}

		मुक्त(Nt_तथ्य->बफर_2);
		मुक्त(Nt_तथ्य);
	}

	वापस 0;
}



//
// set block size
//

अटल
errcode_t
nt_set_blksize(io_channel channel, पूर्णांक blksize)
{
	PNT_PRIVATE_तथ्य Nt_तथ्य = शून्य;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_तथ्य = (PNT_PRIVATE_तथ्य) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(Nt_तथ्य, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	अगर (channel->block_आकार != blksize)
	{
		channel->block_आकार = blksize;

		मुक्त(Nt_तथ्य->बफर_2);
		Nt_तथ्य->बफर_Block_संख्या = 0xffffffff;
		Nt_तथ्य->बफर_आकार = channel->block_आकार;
		ASSERT(0 == (Nt_तथ्य->बफर_आकार % 512));

		Nt_तथ्य->बफर_2 = दो_स्मृति(Nt_तथ्य->बफर_आकार);

		अगर (शून्य == Nt_तथ्य->बफर_2)
		{
			वापस ENOMEM;
		}

	}

	वापस 0;
}


//
// read block
//

अटल
errcode_t
nt_read_blk(io_channel channel, अचिन्हित दीर्घ block,
			       पूर्णांक गिनती, व्योम *buf)
{
	PVOID बफर_To_Read;
	ULONG आकार_To_Read;
	ULONG आकार_2;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_तथ्य Nt_तथ्य = शून्य;
	अचिन्हित Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_तथ्य = (PNT_PRIVATE_तथ्य) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(Nt_तथ्य, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	//
	// If it's in the cache, use it!
	//

	अगर ((1 == गिनती) &&
		(block == Nt_तथ्य->बफर_Block_संख्या) &&
		(Nt_तथ्य->बफर_Block_संख्या != 0xffffffff))
	{
		स_नकल(buf, Nt_तथ्य->बफर_2, channel->block_आकार);
		वापस 0;
	}

	आकार_2 = (गिनती < 0) ? (ULONG)(-गिनती) : (ULONG)(गिनती * channel->block_आकार);

	Offset.QuadPart = block * channel->block_आकार;

	//
	// If not fit to the block
	//

	अगर(आकार_2 <= Nt_तथ्य->बफर_आकार)
	{
		//
		// Update the cache
		//

		Nt_तथ्य->बफर_Block_संख्या = block;
		बफर_To_Read = Nt_तथ्य->बफर_2;
		आकार_To_Read = Nt_तथ्य->बफर_आकार;
	}
	अन्यथा
	{
		आकार_To_Read = आकार_2;
		बफर_To_Read = buf;
		ASSERT(0 == (आकार_To_Read % channel->block_आकार));
	}

	अगर(!_RawRead(Nt_तथ्य->Handle, Offset, आकार_To_Read, बफर_To_Read, &Errno))
	{

		अगर (channel->read_error)
		{
			वापस (channel->read_error)(channel, block, गिनती, buf,
					       आकार_2, 0, Errno);
		}
		अन्यथा
		{
			वापस Errno;
		}
	}


	अगर(बफर_To_Read != buf)
	{
		ASSERT(आकार_2 <= आकार_To_Read);
		स_नकल(buf, बफर_To_Read, आकार_2);
	}

	वापस 0;
}


//
// write block
//

अटल
errcode_t
nt_write_blk(io_channel channel, अचिन्हित दीर्घ block,
				पूर्णांक गिनती, स्थिर व्योम *buf)
{
	ULONG आकार_To_Write;
	LARGE_INTEGER Offset;
	PNT_PRIVATE_तथ्य Nt_तथ्य = शून्य;
	अचिन्हित Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_तथ्य = (PNT_PRIVATE_तथ्य) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(Nt_तथ्य, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	अगर(Nt_तथ्य->OpenedReadonly)
	{
		वापस EACCES;
	}

	अगर (गिनती == 1)
	{
		आकार_To_Write = channel->block_आकार;
	}
	अन्यथा
	{
		Nt_तथ्य->बफर_Block_संख्या = 0xffffffff;

		अगर (गिनती < 0)
		{
			आकार_To_Write = (ULONG)(-गिनती);
		}
		अन्यथा
		{
			आकार_To_Write = (ULONG)(गिनती * channel->block_आकार);
		}
	}


	ASSERT(0 == (आकार_To_Write % 512));
	Offset.QuadPart = block * channel->block_आकार;

	अगर(!_RawWrite(Nt_तथ्य->Handle, Offset, आकार_To_Write, buf, &Errno))
	{
		अगर (channel->write_error)
		{
			वापस (channel->write_error)(channel, block, गिनती, buf,
						आकार_To_Write, 0, Errno);
		}
		अन्यथा
		{
			वापस Errno;
		}
	}


	//
	// Stash a copy.
	//

	अगर(आकार_To_Write >= Nt_तथ्य->बफर_आकार)
	{
		Nt_तथ्य->बफर_Block_संख्या = block;
		स_नकल(Nt_तथ्य->बफर_2, buf, Nt_तथ्य->बफर_आकार);
	}

	Nt_तथ्य->Written = TRUE;

	वापस 0;

}



//
// Flush data buffers to disk.  Since we are currently using a
// write-through cache, this is a no-op.
//

अटल
errcode_t
nt_flush(io_channel channel)
{
	PNT_PRIVATE_तथ्य Nt_तथ्य = शून्य;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_तथ्य = (PNT_PRIVATE_तथ्य) channel->private_तथ्य;
	EXT2_CHECK_MAGIC(Nt_तथ्य, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	अगर(Nt_तथ्य->OpenedReadonly)
	{
		वापस 0; // EACCESS;
	}


	//
	// Flush file buffers.
	//

	_FlushDrive(Nt_तथ्य->Handle);


	//
	// Test and correct partition type.
	//

	अगर(Nt_तथ्य->Written)
	{
		_SetPartType(Nt_तथ्य->Handle, 0x83);
	}

	वापस 0;
}


