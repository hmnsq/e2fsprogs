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

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CONFIG_H
#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਜੇ_ਅੰਤ


//
// I need some warnings to disable...
//


#ਹਦਾਇਤ warning(disable:4514) // unreferenced inline function has been removed
#ਹਦਾਇਤ warning(ਧੱਕੋ,4)

#ਹਦਾਇਤ warning(disable:4201) // nonstandard extension used : nameless struct/union)
#ਹਦਾਇਤ warning(disable:4214) // nonstandard extension used : bit field types other than int
#ਹਦਾਇਤ warning(disable:4115) // named type definition in parentheses

#ਅਨ੍ਤਰ੍ਭਾਵ <ntddk.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ntdddisk.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ntstatus.h>

#ਹਦਾਇਤ warning(ਕੱਢੋ)


//
// Some native APIs.
//

NTSYSAPI
ULONG
NTAPI
Rtl_Nt_ਸਥਿਤੀ_ਮੰਜ਼ਿਲ_Dos_Error(
    ਵਿੱਚ_2 NTSTATUS ਸਥਿਤੀ
   );

NTSYSAPI
NTSTATUS
NTAPI
NtClose(
    ਵਿੱਚ_2 HANDLE Handle
   );


NTSYSAPI
NTSTATUS
NTAPI
NtOpenFile(
    ਬਾਹਰਲਾ_2 PHANDLE FileHandle,
    ਵਿੱਚ_2 ACCESS_ਨਕਾਬ DesiredAccess,
    ਵਿੱਚ_2 POBJECT_ਗੁਣ Object_ਗੁਣ,
    ਬਾਹਰਲਾ_2 PIO_ਸਥਿਤੀ_ਖੰਡ Io_ਸਥਿਤੀ_ਖੰਡ,
    ਵਿੱਚ_2 ULONG ShareAccess,
    ਵਿੱਚ_2 ULONG OpenOptions
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFlushBuffersFile(
    ਵਿੱਚ_2 HANDLE FileHandle,
    ਬਾਹਰਲਾ_2 PIO_ਸਥਿਤੀ_ਖੰਡ Io_ਸਥਿਤੀ_ਖੰਡ
   );


NTSYSAPI
NTSTATUS
NTAPI
NtReadFile(
    ਵਿੱਚ_2 HANDLE FileHandle,
    ਵਿੱਚ_2 HANDLE Event OPTIONAL,
    ਵਿੱਚ_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    ਵਿੱਚ_2 PVOID ApcContext OPTIONAL,
    ਬਾਹਰਲਾ_2 PIO_ਸਥਿਤੀ_ਖੰਡ Io_ਸਥਿਤੀ_ਖੰਡ,
    ਬਾਹਰਲਾ_2 PVOID ਬਫਰ_2,
    ਵਿੱਚ_2 ULONG ਲੰਮਾਈ_2,
    ਵਿੱਚ_2 PLARGE_INTEGER Byte_ਵਿੱਥ OPTIONAL,
    ਵਿੱਚ_2 PULONG ਕੁੰਜੀ_2 OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtWriteFile(
    ਵਿੱਚ_2 HANDLE FileHandle,
    ਵਿੱਚ_2 HANDLE Event OPTIONAL,
    ਵਿੱਚ_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    ਵਿੱਚ_2 PVOID ApcContext OPTIONAL,
    ਬਾਹਰਲਾ_2 PIO_ਸਥਿਤੀ_ਖੰਡ Io_ਸਥਿਤੀ_ਖੰਡ,
    ਵਿੱਚ_2 PVOID ਬਫਰ_2,
    ਵਿੱਚ_2 ULONG ਲੰਮਾਈ_2,
    ਵਿੱਚ_2 PLARGE_INTEGER Byte_ਵਿੱਥ OPTIONAL,
    ਵਿੱਚ_2 PULONG ਕੁੰਜੀ_2 OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtDeviceIoControlFile(
    ਵਿੱਚ_2 HANDLE FileHandle,
    ਵਿੱਚ_2 HANDLE Event OPTIONAL,
    ਵਿੱਚ_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    ਵਿੱਚ_2 PVOID ApcContext OPTIONAL,
    ਬਾਹਰਲਾ_2 PIO_ਸਥਿਤੀ_ਖੰਡ Io_ਸਥਿਤੀ_ਖੰਡ,
    ਵਿੱਚ_2 ULONG Io_Control_ਕੋਡ,
    ਵਿੱਚ_2 PVOID ਨਿਵੇਸ਼_ਬਫਰ OPTIONAL,
    ਵਿੱਚ_2 ULONG ਨਿਵੇਸ਼_ਬਫਰ_ਲੰਮਾਈ,
    ਬਾਹਰਲਾ_2 PVOID Output_ਬਫਰ OPTIONAL,
    ਵਿੱਚ_2 ULONG Output_ਬਫਰ_ਲੰਮਾਈ
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFsControlFile(
    ਵਿੱਚ_2 HANDLE FileHandle,
    ਵਿੱਚ_2 HANDLE Event OPTIONAL,
    ਵਿੱਚ_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    ਵਿੱਚ_2 PVOID ApcContext OPTIONAL,
    ਬਾਹਰਲਾ_2 PIO_ਸਥਿਤੀ_ਖੰਡ Io_ਸਥਿਤੀ_ਖੰਡ,
    ਵਿੱਚ_2 ULONG Io_Control_ਕੋਡ,
    ਵਿੱਚ_2 PVOID ਨਿਵੇਸ਼_ਬਫਰ OPTIONAL,
    ਵਿੱਚ_2 ULONG ਨਿਵੇਸ਼_ਬਫਰ_ਲੰਮਾਈ,
    ਬਾਹਰਲਾ_2 PVOID Output_ਬਫਰ OPTIONAL,
    ਵਿੱਚ_2 ULONG Output_ਬਫਰ_ਲੰਮਾਈ
    );


NTSYSAPI
NTSTATUS
NTAPI
NtDelayExecution(
    ਵਿੱਚ_2 BOOLEAN Alertable,
    ਵਿੱਚ_2 PLARGE_INTEGER Interval
    );


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FSCTL_ਤਾਲਾ_VOLUME               CTL_ਕੋਡ(FILE_DEVICE_FILE_SYSTEM, 6, ਵਿਧੀ_BUFFERED, FILE_ANY_ACCESS)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FSCTL_UNLOCK_VOLUME             CTL_ਕੋਡ(FILE_DEVICE_FILE_SYSTEM, 7, ਵਿਧੀ_BUFFERED, FILE_ANY_ACCESS)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FSCTL_DISMOUNT_VOLUME           CTL_ਕੋਡ(FILE_DEVICE_FILE_SYSTEM, 8, ਵਿਧੀ_BUFFERED, FILE_ANY_ACCESS)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FSCTL_IS_VOLUME_MOUNTED         CTL_ਕੋਡ(FILE_DEVICE_FILE_SYSTEM,10, ਵਿਧੀ_BUFFERED, FILE_ANY_ACCESS)


//
// useful macros
//

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BooleanFlagOn(Flags,SingleFlag) ((BOOLEAN)((((Flags) & (SingleFlag)) != 0)))


//
// Include Win32 error codes.
//

#ਅਨ੍ਤਰ੍ਭਾਵ <winerror.h>

//
// standard stuff
//

#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <malloc.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <linux/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "et/com_err.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs/ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs/ext2_err.h"




//
// For checking structure magic numbers...
//


#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_CHECK_MAGIC(ਬਣਤਰ, ਕੋਡ) \
	  ਜੇ ((ਬਣਤਰ)->magic != (ਕੋਡ)) ਵਾਪਸ (ਕੋਡ)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2_ET_MAGIC_NT_IO_CHANNEL  0x10ed


//
// Private data block
//

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ NT_ਨਿੱਜੀ_ਅੰਕੜੇ {
	ਪੂਰਨ	   magic;
	HANDLE Handle;
	ਪੂਰਨ	   Flags;
	PCHAR  ਬਫਰ_2;
	__u32  ਬਫਰ_ਖੰਡ_ਸੰਖਿਆ;
	ULONG  ਬਫਰ_ਮਾਪ;
	BOOLEAN OpenedReadonly;
	BOOLEAN Written;
}NT_ਨਿੱਜੀ_ਅੰਕੜੇ_2, *PNT_ਨਿੱਜੀ_ਅੰਕੜੇ;



//
// Standard interface prototypes
//

ਸਥਿਰ errcode_t nt_open(ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ flags, io_channel *channel);
ਸਥਿਰ errcode_t nt_close(io_channel channel);
ਸਥਿਰ errcode_t nt_ਸੈੱਟ_blksize(io_channel channel, ਪੂਰਨ blksize);
ਸਥਿਰ errcode_t nt_read_blk(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
			       ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *ਅੰਕੜੇ);
ਸਥਿਰ errcode_t nt_write_blk(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
				ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *ਅੰਕੜੇ);
ਸਥਿਰ errcode_t nt_flush(io_channel channel);

ਸਥਿਰ ਬਣਤਰ struct_io_manager struct_nt_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "NT I/O Manager",
	.open		= nt_open,
	.close		= nt_close,
	.ਸੈੱਟ_blksize	= nt_ਸੈੱਟ_blksize,
	.read_blk	= nt_read_blk,
	.write_blk	= nt_write_blk,
	.flush		= nt_flush
};

//
// function to get API
//

io_manager nt_io_manager()
{
	ਵਾਪਸ &struct_nt_manager;
}





//
// This is a code to convert Win32 errors to unix errno
//

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
	ULONG WinError;
	ਪੂਰਨ errnocode;
}ERROR_ਇੰਦਰਾਜ;

ਸਥਿਰ ERROR_ਇੰਦਰਾਜ Error_ਸਾਰਣੀ[] = {
        {  ERROR_INVALID_FUNCTION,       EINVAL    },
        {  ERROR_FILE_NOT_ਮਿਲਿਆ,         ENOENT    },
        {  ERROR_ਰਸਤਾ_NOT_ਮਿਲਿਆ,         ENOENT    },
        {  ERROR_TOO_MANY_OPEN_FILES,    EMFILE    },
        {  ERROR_ACCESS_DENIED,          EACCES    },
        {  ERROR_INVALID_HANDLE,         EBADF     },
        {  ERROR_ARENA_TRASHED,          ENOMEM    },
        {  ERROR_NOT_ENOUGH_ਸਮ੍ਰਿਤੀ,      ENOMEM    },
        {  ERROR_INVALID_ਖੰਡ,          ENOMEM    },
        {  ERROR_BAD_ENVIRONMENT,        E2BIG     },
        {  ERROR_BAD_FORMAT,             ENOEXEC   },
        {  ERROR_INVALID_ACCESS,         EINVAL    },
        {  ERROR_INVALID_ਅੰਕੜੇ,           EINVAL    },
        {  ERROR_INVALID_DRIVE,          ENOENT    },
        {  ERROR_CURRENT_DIRECTORY,      EACCES    },
        {  ERROR_NOT_SAME_DEVICE,        EXDEV     },
        {  ERROR_NO_ਹੋਰ_FILES,          ENOENT    },
        {  ERROR_ਤਾਲਾ_VIOLATION,         EACCES    },
        {  ERROR_BAD_NETPATH,            ENOENT    },
        {  ERROR_ਜਾਲ_ACCESS_DENIED,  EACCES    },
        {  ERROR_BAD_NET_NAME,           ENOENT    },
        {  ERROR_FILE_EXISTS,            EEXIST    },
        {  ERROR_CANNOT_MAKE,            EACCES    },
        {  ERROR_FAIL_I24,               EACCES    },
        {  ERROR_INVALID_PARAMETER,      EINVAL    },
        {  ERROR_NO_PROC_SLOTS,          EAGAIN    },
        {  ERROR_DRIVE_LOCKED,           EACCES    },
        {  ERROR_BROKEN_PIPE,            EPIPE     },
        {  ERROR_DISK_ਭਰਿਆ,              ENOSPC    },
        {  ERROR_INVALID_ਨਿਸ਼ਾਨਾ_HANDLE,  EBADF     },
        {  ERROR_INVALID_HANDLE,         EINVAL    },
        {  ERROR_WAIT_NO_CHILDREN,       ECHILD    },
        {  ERROR_ਬੱਚਾ_NOT_COMPLETE,     ECHILD    },
        {  ERROR_ਸਿੱਧਾ_ACCESS_HANDLE,   EBADF     },
        {  ERROR_NEGATIVE_SEEK,          EINVAL    },
        {  ERROR_SEEK_ON_DEVICE,         EACCES    },
        {  ERROR_DIR_NOT_ਖਾਲੀ_ਥਾਵਾਂ,          ENOTEMPTY },
        {  ERROR_NOT_LOCKED,             EACCES    },
        {  ERROR_BAD_PATHNAME,           ENOENT    },
        {  ERROR_MAX_THRDS_REACHED,      EAGAIN    },
        {  ERROR_ਤਾਲਾ_FAILED,            EACCES    },
        {  ERROR_ALREADY_EXISTS,         EEXIST    },
        {  ERROR_FILENAME_EXCED_RANGE,   ENOENT    },
        {  ERROR_NESTING_NOT_ALLOWED,    EAGAIN    },
        {  ERROR_NOT_ENOUGH_QUOTA,       ENOMEM    }
};




ਸਥਿਰ
ਬਿਨਾਨਿਸ਼ਾਨੀ
_MapDosError (
    ਵਿੱਚ_2 ULONG WinError
   )
{
	ਪੂਰਨ i;

	//
	// Lookup
	//

	ਲਈ (i = 0; i < (ਆਕਾਰ(Error_ਸਾਰਣੀ)/ਆਕਾਰ(Error_ਸਾਰਣੀ[0])); ++i)
	{
		ਜੇ (WinError == Error_ਸਾਰਣੀ[i].WinError)
		{
			ਵਾਪਸ Error_ਸਾਰਣੀ[i].errnocode;
		}
	}

	//
	// not in table. Check ranges
	//

	ਜੇ ((WinError >= ERROR_WRITE_PROTECT) &&
		(WinError <= ERROR_SHARING_ਬਫਰ_EXCEEDED))
	{
		ਵਾਪਸ EACCES;
	}
	ਨਹੀਂਤਾਂ ਜੇ ((WinError >= ERROR_INVALID_STARTING_CODESEG) &&
			 (WinError <= ERROR_INFLOOP_ਵਿੱਚ_RELOC_CHAIN))
	{
		ਵਾਪਸ ENOEXEC;
	}
	ਨਹੀਂਤਾਂ
	{
		ਵਾਪਸ EINVAL;
	}
}







//
// Function to map NT status to dos error.
//

ਸਥਿਰ
__inline
ਬਿਨਾਨਿਸ਼ਾਨੀ
Map_Nt_ਸਥਿਤੀ(
    ਵਿੱਚ_2 NTSTATUS ਸਥਿਤੀ
   )
{
	ਵਾਪਸ _MapDosError(Rtl_Nt_ਸਥਿਤੀ_ਮੰਜ਼ਿਲ_Dos_Error(ਸਥਿਤੀ));
}





//
// Helper functions to make things easier
//

ਸਥਿਰ
NTSTATUS
_OpenNtName(
    ਵਿੱਚ_2 PCSTR Name,
    ਵਿੱਚ_2 BOOLEAN Readonly,
    ਬਾਹਰਲਾ_2 PHANDLE Handle,
    ਬਾਹਰਲਾ_2 PBOOLEAN OpenedReadonly OPTIONAL
   )
{
	UNICODE_STRING UnicodeString;
	ANSI_STRING    AnsiString;
	WCHAR ਬਫਰ_2[512];
	NTSTATUS ਸਥਿਤੀ;
	OBJECT_ਗੁਣ Object_ਗੁਣ;
	IO_ਸਥਿਤੀ_ਖੰਡ Io_ਸਥਿਤੀ_ਖੰਡ;

	//
	// Make Unicode name from input string
	//

	UnicodeString.ਬਫਰ_2 = &ਬਫਰ_2[0];
	UnicodeString.ਲੰਮਾਈ_2 = 0;
	UnicodeString.ਵੱਧ_ਤੋਂ_ਵੱਧ_ਲੰਮਾਈ = ਆਕਾਰ(ਬਫਰ_2); // in bytes!!!

	RtlInitAnsiString(&AnsiString, Name);

	ਸਥਿਤੀ = Rtl_Ansi_String_ਮੰਜ਼ਿਲ_Unicode_String(&UnicodeString, &AnsiString, FALSE);

	ਜੇ(!NT_SUCCESS(ਸਥਿਤੀ))
	{
		ਵਾਪਸ ਸਥਿਤੀ; // Unmappable character?
	}

	//
	// Initialize object
	//

	Initialize_Object_ਗੁਣ(&Object_ਗੁਣ,
							   &UnicodeString,
							   OBJ_CASE_INSENSITIVE,
							   ਨਿਰਾ,
							   ਨਿਰਾ );

	//
	// Try to open it in initial mode
	//

	ਜੇ(ਦਲੀਲ_PRESENT(OpenedReadonly))
	{
		*OpenedReadonly = Readonly;
	}


	ਸਥਿਤੀ = NtOpenFile(Handle,
						SYNCHRONIZE | FILE_READ_ਅੰਕੜੇ | (Readonly ? 0 : FILE_WRITE_ਅੰਕੜੇ),
						&Object_ਗੁਣ,
						&Io_ਸਥਿਤੀ_ਖੰਡ,
						FILE_SHARE_WRITE | FILE_SHARE_READ,
						FILE_SYNCHRONOUS_IO_NONALERT);

	ਜੇ(!NT_SUCCESS(ਸਥਿਤੀ))
	{
		//
		// Maybe was just mounted? wait 0.5 sec and retry.
		//

		LARGE_INTEGER Interval;
		Interval.QuadPart = -5000000; // 0.5 sec. from now

		NtDelayExecution(FALSE, &Interval);

		ਸਥਿਤੀ = NtOpenFile(Handle,
							SYNCHRONIZE | FILE_READ_ਅੰਕੜੇ | (Readonly ? 0 : FILE_WRITE_ਅੰਕੜੇ),
							&Object_ਗੁਣ,
							&Io_ਸਥਿਤੀ_ਖੰਡ,
							FILE_SHARE_WRITE | FILE_SHARE_READ,
							FILE_SYNCHRONOUS_IO_NONALERT);

		//
		// Try to satisfy mode
		//

		ਜੇ((ਸਥਿਤੀ_ACCESS_DENIED == ਸਥਿਤੀ) && !Readonly)
		{
			ਜੇ(ਦਲੀਲ_PRESENT(OpenedReadonly))
			{
				*OpenedReadonly = TRUE;
			}

			ਸਥਿਤੀ = NtOpenFile(Handle,
							SYNCHRONIZE | FILE_READ_ਅੰਕੜੇ,
							&Object_ਗੁਣ,
							&Io_ਸਥਿਤੀ_ਖੰਡ,
							FILE_SHARE_WRITE | FILE_SHARE_READ,
							FILE_SYNCHRONOUS_IO_NONALERT);
		}
	}



	//
	//ਪੂਰਾ
	//

	ਵਾਪਸ ਸਥਿਤੀ;
}


ਸਥਿਰ
NTSTATUS
_OpenDriveLetter(
    ਵਿੱਚ_2 CHAR Letter,
    ਵਿੱਚ_2 BOOLEAN ReadOnly,
    ਬਾਹਰਲਾ_2 PHANDLE Handle,
    ਬਾਹਰਲਾ_2 PBOOLEAN OpenedReadonly OPTIONAL
   )
{
	CHAR ਬਫਰ_2[100];

	sprintf(ਬਫਰ_2, "\\DosDevices\\%c:", Letter);

	ਵਾਪਸ _OpenNtName(ਬਫਰ_2, ReadOnly, Handle, OpenedReadonly);
}


//
// Flush device
//

ਸਥਿਰ
__inline
NTSTATUS
_FlushDrive(
		ਵਿੱਚ_2 HANDLE Handle
		)
{
	IO_ਸਥਿਤੀ_ਖੰਡ Io_ਸਥਿਤੀ_ਖੰਡ;
	ਵਾਪਸ NtFlushBuffersFile(Handle, &Io_ਸਥਿਤੀ_ਖੰਡ);
}


//
// lock drive
//

ਸਥਿਰ
__inline
NTSTATUS
ਤਾਲਾ_Drive(
		ਵਿੱਚ_2 HANDLE Handle
		)
{
	IO_ਸਥਿਤੀ_ਖੰਡ Io_ਸਥਿਤੀ_ਖੰਡ;
	ਵਾਪਸ NtFsControlFile(Handle, 0, 0, 0, &Io_ਸਥਿਤੀ_ਖੰਡ, FSCTL_ਤਾਲਾ_VOLUME, 0, 0, 0, 0);
}


//
// unlock drive
//

ਸਥਿਰ
__inline
NTSTATUS
_UnlockDrive(
	ਵਿੱਚ_2 HANDLE Handle
	)
{
	IO_ਸਥਿਤੀ_ਖੰਡ Io_ਸਥਿਤੀ_ਖੰਡ;
	ਵਾਪਸ NtFsControlFile(Handle, 0, 0, 0, &Io_ਸਥਿਤੀ_ਖੰਡ, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0);
}

ਸਥਿਰ
__inline
NTSTATUS
_DismountDrive(
	ਵਿੱਚ_2 HANDLE Handle
	)
{
	IO_ਸਥਿਤੀ_ਖੰਡ Io_ਸਥਿਤੀ_ਖੰਡ;
	ਵਾਪਸ NtFsControlFile(Handle, 0, 0, 0, &Io_ਸਥਿਤੀ_ਖੰਡ, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0);
}


//
// is mounted
//

ਸਥਿਰ
__inline
BOOLEAN
_IsMounted(
	ਵਿੱਚ_2 HANDLE Handle
	)
{
	IO_ਸਥਿਤੀ_ਖੰਡ Io_ਸਥਿਤੀ_ਖੰਡ;
	NTSTATUS ਸਥਿਤੀ;
	ਸਥਿਤੀ = NtFsControlFile(Handle, 0, 0, 0, &Io_ਸਥਿਤੀ_ਖੰਡ, FSCTL_IS_VOLUME_MOUNTED, 0, 0, 0, 0);
	ਵਾਪਸ (BOOLEAN)(ਸਥਿਤੀ_SUCCESS == ਸਥਿਤੀ);
}


ਸਥਿਰ
__inline
NTSTATUS
_CloseDisk(
		ਵਿੱਚ_2 HANDLE Handle
		)
{
	ਵਾਪਸ NtClose(Handle);
}




//
// Make NT name from any recognized name
//

ਸਥਿਰ
PCSTR
_NormalizeDeviceName(
    ਵਿੱਚ_2 PCSTR Device,
    ਵਿੱਚ_2 PSTR Normalized_Device_Name_ਬਫਰ
   )
{
	ਪੂਰਨ ਹਿੱਸੇ_ਕਰੋ_ਸੰਖਿਆ = -1;
	UCHAR Disk_ਸੰਖਿਆ;
	PSTR p;


	//
	// Do not try to parse NT name
	//

	ਜੇ('\\' == *Device)
		ਵਾਪਸ Device;



	//
	// Strip leading '/dev/' if any
	//

	ਜੇ(('/' == *(Device)) &&
		('d' == *(Device + 1)) &&
		('e' == *(Device + 2)) &&
		('v' == *(Device + 3)) &&
		('/' == *(Device + 4)))
	{
		Device += 5;
	}

	ਜੇ('\0' == *Device)
	{
		ਵਾਪਸ ਨਿਰਾ;
	}


	//
	// forms: hda[n], fd[n]
	//

	ਜੇ('d' != *(Device + 1))
	{
		ਵਾਪਸ ਨਿਰਾ;
	}

	ਜੇ('h' == *Device)
	{
		ਜੇ((*(Device + 2) < 'a') || (*(Device + 2) > ('a' + 9)) ||
		   ((*(Device + 3) != '\0') &&
			((*(Device + 4) != '\0') ||
			 ((*(Device + 3) < '0') || (*(Device + 3) > '9'))
			)
		   )
		  )
		{
			ਵਾਪਸ ਨਿਰਾ;
		}

		Disk_ਸੰਖਿਆ = (UCHAR)(*(Device + 2) - 'a');

		ਜੇ(*(Device + 3) != '\0')
		{
			ਹਿੱਸੇ_ਕਰੋ_ਸੰਖਿਆ = (*(Device + 3) - '0');
		}

	}
	ਨਹੀਂਤਾਂ ਜੇ('f' == *Device)
	{
		//
		// 3-d letter should be a digit.
		//

		ਜੇ((*(Device + 3) != '\0') ||
		   (*(Device + 2) < '0') || (*(Device + 2) > '9'))
		{
			ਵਾਪਸ ਨਿਰਾ;
		}

		Disk_ਸੰਖਿਆ = (UCHAR)(*(Device + 2) - '0');

	}
	ਨਹੀਂਤਾਂ
	{
		//
		// invalid prefix
		//

		ਵਾਪਸ ਨਿਰਾ;
	}



	//
	//ਅਗੇਤਰ
	//

	strcpy(Normalized_Device_Name_ਬਫਰ, "\\Device\\");

	//
	// Media name
	//

	ਚੋਣ(*Device)
	{

	ਹਾਲਤ 'f':
		strcat(Normalized_Device_Name_ਬਫਰ, "Floppy0");
		ਰੋਕੋ;

	ਹਾਲਤ 'h':
		strcat(Normalized_Device_Name_ਬਫਰ, "Harddisk0");
		ਰੋਕੋ;
	}


	p = Normalized_Device_Name_ਬਫਰ + ਲੜੀ_ਲੰਬਾਈ(Normalized_Device_Name_ਬਫਰ) - 1;
	*p = (CHAR)(*p + Disk_ਸੰਖਿਆ);


	//
	// Partition nr.
	//

	ਜੇ(ਹਿੱਸੇ_ਕਰੋ_ਸੰਖਿਆ >= 0)
	{
		strcat(Normalized_Device_Name_ਬਫਰ, "\\Partition0");

		p = Normalized_Device_Name_ਬਫਰ + ਲੜੀ_ਲੰਬਾਈ(Normalized_Device_Name_ਬਫਰ) - 1;
		*p = (CHAR)(*p + ਹਿੱਸੇ_ਕਰੋ_ਸੰਖਿਆ);
	}


	ਵਾਪਸ Normalized_Device_Name_ਬਫਰ;
}




ਸਥਿਰ
VOID
Get_Device_ਮਾਪ(
    ਵਿੱਚ_2 HANDLE h,
    ਬਾਹਰਲਾ_2 ਬਿਨਾਨਿਸ਼ਾਨੀ __int64 *Fs_ਮਾਪ
   )
{
	ਹਿੱਸੇ_ਕਰੋ_INFORMATION pi;
	DISK_GEOMETRY gi;
	NTSTATUS ਸਥਿਤੀ;
	IO_ਸਥਿਤੀ_ਖੰਡ Io_ਸਥਿਤੀ_ਖੰਡ;

	//
	// Zero it
	//

	*Fs_ਮਾਪ = 0;

	//
	// Call driver
	//

	Rtl_Zero_ਸਮ੍ਰਿਤੀ(&pi, ਆਕਾਰ(ਹਿੱਸੇ_ਕਰੋ_INFORMATION));

	ਸਥਿਤੀ = NtDeviceIoControlFile(
		h, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ, &Io_ਸਥਿਤੀ_ਖੰਡ, IOCTL_DISK_GET_ਹਿੱਸੇ_ਕਰੋ_INFO,
		&pi, ਆਕਾਰ(ਹਿੱਸੇ_ਕਰੋ_INFORMATION),
		&pi, ਆਕਾਰ(ਹਿੱਸੇ_ਕਰੋ_INFORMATION));


	ਜੇ(NT_SUCCESS(ਸਥਿਤੀ))
	{
		*Fs_ਮਾਪ = pi.ਹਿੱਸੇ_ਕਰੋ_ਲੰਮਾਈ.QuadPart;
	}
	ਨਹੀਂਤਾਂ ਜੇ(ਸਥਿਤੀ_INVALID_DEVICE_REQUEST == ਸਥਿਤੀ)
	{
		//
		// No partitions: get device info.
		//

		Rtl_Zero_ਸਮ੍ਰਿਤੀ(&gi, ਆਕਾਰ(DISK_GEOMETRY));

		ਸਥਿਤੀ = NtDeviceIoControlFile(
				h, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ, &Io_ਸਥਿਤੀ_ਖੰਡ, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, ਆਕਾਰ(DISK_GEOMETRY),
				&gi, ਆਕਾਰ(DISK_GEOMETRY));


		ਜੇ(NT_SUCCESS(ਸਥਿਤੀ))
		{
			*Fs_ਮਾਪ =
				gi.ਬਾਈਟਾਂ_ਪ੍ਰਤੀ_Sector *
				gi.Sectors_ਪ੍ਰਤੀ_Track *
				gi.Tracks_ਪ੍ਰਤੀ_Cylinder *
				gi.Cylinders.QuadPart;
		}

	}
}



//
// Open device by name.
//

ਸਥਿਰ
BOOLEAN
_Ext2OpenDevice(
    ਵਿੱਚ_2 PCSTR Name,
    ਵਿੱਚ_2 BOOLEAN ReadOnly,
    ਬਾਹਰਲਾ_2 PHANDLE Handle,
    ਬਾਹਰਲਾ_2 PBOOLEAN OpenedReadonly OPTIONAL,
    ਬਾਹਰਲਾ_2 ਬਿਨਾਨਿਸ਼ਾਨੀ *Errno OPTIONAL
   )
{
	CHAR NormalizedDeviceName[512];
	NTSTATUS ਸਥਿਤੀ;

	ਜੇ(ਨਿਰਾ == Name)
	{
		//
		// Set not found
		//

		ਜੇ(ਦਲੀਲ_PRESENT(Errno))
			*Errno = ENOENT;

		ਵਾਪਸ FALSE;
	}


	ਜੇ((((*Name) | 0x20) >= 'a') && (((*Name) | 0x20) <= 'z') &&
		(':' == *(Name + 1)) && ('\0' == *(Name + 2)))
	{
		ਸਥਿਤੀ = _OpenDriveLetter(*Name, ReadOnly, Handle, OpenedReadonly);
	}
	ਨਹੀਂਤਾਂ
	{
		//
		// Make name
		//

		Name = _NormalizeDeviceName(Name, NormalizedDeviceName);

		ਜੇ(ਨਿਰਾ == Name)
		{
			//
			// Set not found
			//

			ਜੇ(ਦਲੀਲ_PRESENT(Errno))
				*Errno = ENOENT;

			ਵਾਪਸ FALSE;
		}

		//
		// Try to open it
		//

		ਸਥਿਤੀ = _OpenNtName(Name, ReadOnly, Handle, OpenedReadonly);
	}


	ਜੇ(!NT_SUCCESS(ਸਥਿਤੀ))
	{
		ਜੇ(ਦਲੀਲ_PRESENT(Errno))
			*Errno = Map_Nt_ਸਥਿਤੀ(ਸਥਿਤੀ);

		ਵਾਪਸ FALSE;
	}

	ਵਾਪਸ TRUE;
}


//
// Raw block io. Sets dos errno
//

ਸਥਿਰ
BOOLEAN
ਖੰਡ_Io(
    ਵਿੱਚ_2 HANDLE Handle,
    ਵਿੱਚ_2 LARGE_INTEGER ਵਿੱਥ_2,
    ਵਿੱਚ_2 ULONG ਬਾਈਟਾਂ_2,
    ਵਿੱਚ_2 ਬਾਹਰਲਾ_2 PCHAR ਬਫਰ_2,
    ਵਿੱਚ_2 BOOLEAN Read,
    ਬਾਹਰਲਾ_2 ਬਿਨਾਨਿਸ਼ਾਨੀ* Errno
   )
{
	IO_ਸਥਿਤੀ_ਖੰਡ Io_ਸਥਿਤੀ_ਖੰਡ;
	NTSTATUS ਸਥਿਤੀ;

	//
	// Should be aligned
	//

	ASSERT(0 == (ਬਾਈਟਾਂ_2 % 512));
	ASSERT(0 == (ਵਿੱਥ_2.ਨੀਵਾਂ_Part % 512));


	//
	// perform io
	//

	ਜੇ(Read)
	{
		ਸਥਿਤੀ = NtReadFile(Handle, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ,
			&Io_ਸਥਿਤੀ_ਖੰਡ, ਬਫਰ_2, ਬਾਈਟਾਂ_2, &ਵਿੱਥ_2, ਨਿਰਾ);
	}
	ਨਹੀਂਤਾਂ
	{
		ਸਥਿਤੀ = NtWriteFile(Handle, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ,
			&Io_ਸਥਿਤੀ_ਖੰਡ, ਬਫਰ_2, ਬਾਈਟਾਂ_2, &ਵਿੱਥ_2, ਨਿਰਾ);
	}


	//
	// translate error
	//

	ਜੇ(NT_SUCCESS(ਸਥਿਤੀ))
	{
		*Errno = 0;
		ਵਾਪਸ TRUE;
	}

	*Errno = Map_Nt_ਸਥਿਤੀ(ਸਥਿਤੀ);

	ਵਾਪਸ FALSE;
}



__inline
BOOLEAN
_RawWrite(
    ਵਿੱਚ_2 HANDLE Handle,
    ਵਿੱਚ_2 LARGE_INTEGER ਵਿੱਥ_2,
    ਵਿੱਚ_2 ULONG ਬਾਈਟਾਂ_2,
    ਬਾਹਰਲਾ_2 ਅਚਲ CHAR* ਬਫਰ_2,
    ਬਾਹਰਲਾ_2 ਬਿਨਾਨਿਸ਼ਾਨੀ* Errno
   )
{
	ਵਾਪਸ ਖੰਡ_Io(Handle, ਵਿੱਥ_2, ਬਾਈਟਾਂ_2, (PCHAR)ਬਫਰ_2, FALSE, Errno);
}

__inline
BOOLEAN
_RawRead(
    ਵਿੱਚ_2 HANDLE Handle,
    ਵਿੱਚ_2 LARGE_INTEGER ਵਿੱਥ_2,
    ਵਿੱਚ_2 ULONG ਬਾਈਟਾਂ_2,
    ਵਿੱਚ_2 PCHAR ਬਫਰ_2,
    ਬਾਹਰਲਾ_2 ਬਿਨਾਨਿਸ਼ਾਨੀ* Errno
   )
{
	ਵਾਪਸ ਖੰਡ_Io(Handle, ਵਿੱਥ_2, ਬਾਈਟਾਂ_2, ਬਫਰ_2, TRUE, Errno);
}



__inline
BOOLEAN
ਸੈੱਟ_Part_Type(
    ਵਿੱਚ_2 HANDLE Handle,
    ਵਿੱਚ_2 UCHAR Type
   )
{
	IO_ਸਥਿਤੀ_ਖੰਡ Io_ਸਥਿਤੀ_ਖੰਡ;
	ਵਾਪਸ ਸਥਿਤੀ_SUCCESS == NtDeviceIoControlFile(
												   Handle, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ, &Io_ਸਥਿਤੀ_ਖੰਡ, IOCTL_DISK_ਸੈੱਟ_ਹਿੱਸੇ_ਕਰੋ_INFO,
												   &Type, ਆਕਾਰ(Type),
												   ਨਿਰਾ, 0);
}



//--------------------- interface part

//
// Interface functions.
// Is_mounted is set to 1 if the device is mounted, 0 otherwise
//

errcode_t
ext2fs_check_if_mounted(ਅਚਲ ਅੱਖਰ *file, ਪੂਰਨ *mount_flags)
{
	HANDLE h;
	BOOLEAN Readonly;

	*mount_flags = 0;

	ਜੇ(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, ਨਿਰਾ))
	{
		ਵਾਪਸ 0;
	}


	__try{
		*mount_flags &= _IsMounted(h) ? EXT2_MF_MOUNTED : 0;
	}
	__finally{
		_CloseDisk(h);
	}

	ਵਾਪਸ 0;
}



//
// Returns the number of blocks in a partition
//

ਸਥਿਰ __int64 Fs_ਮਾਪ = 0;
ਸਥਿਰ ਅੱਖਰ knowndevice[1024] = "";


errcode_t
ext2fs_get_device_ਮਾਪ(ਅਚਲ ਅੱਖਰ *file, ਪੂਰਨ blocksize,
				 blk_t *retblocks)
{
	HANDLE h;
	BOOLEAN Readonly;

	ਜੇ((0 == Fs_ਮਾਪ) || (0 != strcmp(knowndevice, file)))
	{

		ਜੇ(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, ਨਿਰਾ))
		{
			ਵਾਪਸ 0;
		}


		__try{

			//
			// Get size
			//

			Get_Device_ਮਾਪ(h, &Fs_ਮਾਪ);
			strcpy(knowndevice, file);
		}
		__finally{
			_CloseDisk(h);
		}

	}

	*retblocks = (blk_t)(ਬਿਨਾਨਿਸ਼ਾਨੀ __int64)(Fs_ਮਾਪ / blocksize);
	UNREFERENCED_PARAMETER(file);
	ਵਾਪਸ 0;
}






//
// Table elements
//


ਸਥਿਰ
errcode_t
nt_open(ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ flags, io_channel *channel)
{
	io_channel      io = ਨਿਰਾ;
	PNT_ਨਿੱਜੀ_ਅੰਕੜੇ Nt_ਅੰਕੜੇ = ਨਿਰਾ;
	errcode_t Errno = 0;

	//
	// Check name
	//

	ਜੇ (ਨਿਰਾ == name)
	{
		ਵਾਪਸ EXT2_ET_BAD_DEVICE_NAME;
	}

	__try{

		//
		// Allocate channel handle
		//

		io = (io_channel) ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(ਬਣਤਰ struct_io_channel));

		ਜੇ (ਨਿਰਾ == io)
		{
			Errno = ENOMEM;
			__leave;
		}

		Rtl_Zero_ਸਮ੍ਰਿਤੀ(io, ਆਕਾਰ(ਬਣਤਰ struct_io_channel));
		io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

		Nt_ਅੰਕੜੇ = (PNT_ਨਿੱਜੀ_ਅੰਕੜੇ)ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(NT_ਨਿੱਜੀ_ਅੰਕੜੇ_2));

		ਜੇ (ਨਿਰਾ == Nt_ਅੰਕੜੇ)
		{
			Errno = ENOMEM;
			__leave;
		}


		io->manager = nt_io_manager();
		io->name = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਲੜੀ_ਲੰਬਾਈ(name) + 1);
		ਜੇ (ਨਿਰਾ == io->name)
		{
			Errno = ENOMEM;
			__leave;
		}

		strcpy(io->name, name);
		io->ਨਿੱਜੀ_ਅੰਕੜੇ = Nt_ਅੰਕੜੇ;
		io->ਖੰਡ_ਮਾਪ = 1024;
		io->read_error = 0;
		io->write_error = 0;
		io->refcount = 1;

		//
		// Initialize data
		//

		Rtl_Zero_ਸਮ੍ਰਿਤੀ(Nt_ਅੰਕੜੇ, ਆਕਾਰ(NT_ਨਿੱਜੀ_ਅੰਕੜੇ_2));

		Nt_ਅੰਕੜੇ->magic = EXT2_ET_MAGIC_NT_IO_CHANNEL;
		Nt_ਅੰਕੜੇ->ਬਫਰ_ਖੰਡ_ਸੰਖਿਆ = 0xffffffff;
		Nt_ਅੰਕੜੇ->ਬਫਰ_ਮਾਪ = 1024;
		Nt_ਅੰਕੜੇ->ਬਫਰ_2 = ਸਮ੍ਰਿਤੀ_ਦਿਓ(Nt_ਅੰਕੜੇ->ਬਫਰ_ਮਾਪ);

		ਜੇ (ਨਿਰਾ == Nt_ਅੰਕੜੇ->ਬਫਰ_2)
		{
			Errno = ENOMEM;
			__leave;
		}

		//
		// Open it
		//

		ਜੇ(!_Ext2OpenDevice(name, (BOOLEAN)!BooleanFlagOn(flags, EXT2_FLAG_RW), &Nt_ਅੰਕੜੇ->Handle, &Nt_ਅੰਕੜੇ->OpenedReadonly, &Errno))
		{
			__leave;
		}


		//
		// get size
		//

		Get_Device_ਮਾਪ(Nt_ਅੰਕੜੇ->Handle, &Fs_ਮਾਪ);
		strcpy(knowndevice, name);


		//
		// Lock/dismount
		//

		ਜੇ(!NT_SUCCESS(ਤਾਲਾ_Drive(Nt_ਅੰਕੜੇ->Handle)) /*|| !NT_SUCCESS(_DismountDrive(NtData->Handle))*/)
		{
			Nt_ਅੰਕੜੇ->OpenedReadonly = TRUE;
		}

		//
		// Done
		//

		*channel = io;


	}
	__finally{

		ਜੇ(0 != Errno)
		{
			//
			// Cleanup
			//

			ਜੇ (ਨਿਰਾ != io)
			{
				ਮੁਕਤ(io->name);
				ਮੁਕਤ(io);
			}

			ਜੇ (ਨਿਰਾ != Nt_ਅੰਕੜੇ)
			{
				ਜੇ(ਨਿਰਾ != Nt_ਅੰਕੜੇ->Handle)
				{
					_UnlockDrive(Nt_ਅੰਕੜੇ->Handle);
					_CloseDisk(Nt_ਅੰਕੜੇ->Handle);
				}

				ਮੁਕਤ(Nt_ਅੰਕੜੇ->ਬਫਰ_2);
				ਮੁਕਤ(Nt_ਅੰਕੜੇ);
			}
		}
	}

	ਵਾਪਸ Errno;
}


//
// Close api
//

ਸਥਿਰ
errcode_t
nt_close(io_channel channel)
{
	PNT_ਨਿੱਜੀ_ਅੰਕੜੇ Nt_ਅੰਕੜੇ = ਨਿਰਾ;

	ਜੇ(ਨਿਰਾ == channel)
	{
		ਵਾਪਸ 0;
	}

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_ਅੰਕੜੇ = (PNT_ਨਿੱਜੀ_ਅੰਕੜੇ) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(Nt_ਅੰਕੜੇ, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	ਜੇ (--channel->refcount > 0)
	{
		ਵਾਪਸ 0;
	}

	ਮੁਕਤ(channel->name);
	ਮੁਕਤ(channel);

	ਜੇ (ਨਿਰਾ != Nt_ਅੰਕੜੇ)
	{
		ਜੇ(ਨਿਰਾ != Nt_ਅੰਕੜੇ->Handle)
		{
			_DismountDrive(Nt_ਅੰਕੜੇ->Handle);
			_UnlockDrive(Nt_ਅੰਕੜੇ->Handle);
			_CloseDisk(Nt_ਅੰਕੜੇ->Handle);
		}

		ਮੁਕਤ(Nt_ਅੰਕੜੇ->ਬਫਰ_2);
		ਮੁਕਤ(Nt_ਅੰਕੜੇ);
	}

	ਵਾਪਸ 0;
}



//
// set block size
//

ਸਥਿਰ
errcode_t
nt_ਸੈੱਟ_blksize(io_channel channel, ਪੂਰਨ blksize)
{
	PNT_ਨਿੱਜੀ_ਅੰਕੜੇ Nt_ਅੰਕੜੇ = ਨਿਰਾ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_ਅੰਕੜੇ = (PNT_ਨਿੱਜੀ_ਅੰਕੜੇ) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(Nt_ਅੰਕੜੇ, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	ਜੇ (channel->ਖੰਡ_ਮਾਪ != blksize)
	{
		channel->ਖੰਡ_ਮਾਪ = blksize;

		ਮੁਕਤ(Nt_ਅੰਕੜੇ->ਬਫਰ_2);
		Nt_ਅੰਕੜੇ->ਬਫਰ_ਖੰਡ_ਸੰਖਿਆ = 0xffffffff;
		Nt_ਅੰਕੜੇ->ਬਫਰ_ਮਾਪ = channel->ਖੰਡ_ਮਾਪ;
		ASSERT(0 == (Nt_ਅੰਕੜੇ->ਬਫਰ_ਮਾਪ % 512));

		Nt_ਅੰਕੜੇ->ਬਫਰ_2 = ਸਮ੍ਰਿਤੀ_ਦਿਓ(Nt_ਅੰਕੜੇ->ਬਫਰ_ਮਾਪ);

		ਜੇ (ਨਿਰਾ == Nt_ਅੰਕੜੇ->ਬਫਰ_2)
		{
			ਵਾਪਸ ENOMEM;
		}

	}

	ਵਾਪਸ 0;
}


//
// read block
//

ਸਥਿਰ
errcode_t
nt_read_blk(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
			       ਪੂਰਨ ਗਿਣਤੀ_2, ਖਾਲੀ *buf)
{
	PVOID ਬਫਰ_ਮੰਜ਼ਿਲ_Read;
	ULONG ਮਾਪ_ਮੰਜ਼ਿਲ_Read;
	ULONG ਮਾਪ_2;
	LARGE_INTEGER ਵਿੱਥ_2;
	PNT_ਨਿੱਜੀ_ਅੰਕੜੇ Nt_ਅੰਕੜੇ = ਨਿਰਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_ਅੰਕੜੇ = (PNT_ਨਿੱਜੀ_ਅੰਕੜੇ) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(Nt_ਅੰਕੜੇ, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	//
	// If it's in the cache, use it!
	//

	ਜੇ ((1 == ਗਿਣਤੀ_2) &&
		(ਖੰਡ == Nt_ਅੰਕੜੇ->ਬਫਰ_ਖੰਡ_ਸੰਖਿਆ) &&
		(Nt_ਅੰਕੜੇ->ਬਫਰ_ਖੰਡ_ਸੰਖਿਆ != 0xffffffff))
	{
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf, Nt_ਅੰਕੜੇ->ਬਫਰ_2, channel->ਖੰਡ_ਮਾਪ);
		ਵਾਪਸ 0;
	}

	ਮਾਪ_2 = (ਗਿਣਤੀ_2 < 0) ? (ULONG)(-ਗਿਣਤੀ_2) : (ULONG)(ਗਿਣਤੀ_2 * channel->ਖੰਡ_ਮਾਪ);

	ਵਿੱਥ_2.QuadPart = ਖੰਡ * channel->ਖੰਡ_ਮਾਪ;

	//
	// If not fit to the block
	//

	ਜੇ(ਮਾਪ_2 <= Nt_ਅੰਕੜੇ->ਬਫਰ_ਮਾਪ)
	{
		//
		// Update the cache
		//

		Nt_ਅੰਕੜੇ->ਬਫਰ_ਖੰਡ_ਸੰਖਿਆ = ਖੰਡ;
		ਬਫਰ_ਮੰਜ਼ਿਲ_Read = Nt_ਅੰਕੜੇ->ਬਫਰ_2;
		ਮਾਪ_ਮੰਜ਼ਿਲ_Read = Nt_ਅੰਕੜੇ->ਬਫਰ_ਮਾਪ;
	}
	ਨਹੀਂਤਾਂ
	{
		ਮਾਪ_ਮੰਜ਼ਿਲ_Read = ਮਾਪ_2;
		ਬਫਰ_ਮੰਜ਼ਿਲ_Read = buf;
		ASSERT(0 == (ਮਾਪ_ਮੰਜ਼ਿਲ_Read % channel->ਖੰਡ_ਮਾਪ));
	}

	ਜੇ(!_RawRead(Nt_ਅੰਕੜੇ->Handle, ਵਿੱਥ_2, ਮਾਪ_ਮੰਜ਼ਿਲ_Read, ਬਫਰ_ਮੰਜ਼ਿਲ_Read, &Errno))
	{

		ਜੇ (channel->read_error)
		{
			ਵਾਪਸ (channel->read_error)(channel, ਖੰਡ, ਗਿਣਤੀ_2, buf,
					       ਮਾਪ_2, 0, Errno);
		}
		ਨਹੀਂਤਾਂ
		{
			ਵਾਪਸ Errno;
		}
	}


	ਜੇ(ਬਫਰ_ਮੰਜ਼ਿਲ_Read != buf)
	{
		ASSERT(ਮਾਪ_2 <= ਮਾਪ_ਮੰਜ਼ਿਲ_Read);
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(buf, ਬਫਰ_ਮੰਜ਼ਿਲ_Read, ਮਾਪ_2);
	}

	ਵਾਪਸ 0;
}


//
// write block
//

ਸਥਿਰ
errcode_t
nt_write_blk(io_channel channel, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਖੰਡ,
				ਪੂਰਨ ਗਿਣਤੀ_2, ਅਚਲ ਖਾਲੀ *buf)
{
	ULONG ਮਾਪ_ਮੰਜ਼ਿਲ_Write;
	LARGE_INTEGER ਵਿੱਥ_2;
	PNT_ਨਿੱਜੀ_ਅੰਕੜੇ Nt_ਅੰਕੜੇ = ਨਿਰਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_ਅੰਕੜੇ = (PNT_ਨਿੱਜੀ_ਅੰਕੜੇ) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(Nt_ਅੰਕੜੇ, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	ਜੇ(Nt_ਅੰਕੜੇ->OpenedReadonly)
	{
		ਵਾਪਸ EACCES;
	}

	ਜੇ (ਗਿਣਤੀ_2 == 1)
	{
		ਮਾਪ_ਮੰਜ਼ਿਲ_Write = channel->ਖੰਡ_ਮਾਪ;
	}
	ਨਹੀਂਤਾਂ
	{
		Nt_ਅੰਕੜੇ->ਬਫਰ_ਖੰਡ_ਸੰਖਿਆ = 0xffffffff;

		ਜੇ (ਗਿਣਤੀ_2 < 0)
		{
			ਮਾਪ_ਮੰਜ਼ਿਲ_Write = (ULONG)(-ਗਿਣਤੀ_2);
		}
		ਨਹੀਂਤਾਂ
		{
			ਮਾਪ_ਮੰਜ਼ਿਲ_Write = (ULONG)(ਗਿਣਤੀ_2 * channel->ਖੰਡ_ਮਾਪ);
		}
	}


	ASSERT(0 == (ਮਾਪ_ਮੰਜ਼ਿਲ_Write % 512));
	ਵਿੱਥ_2.QuadPart = ਖੰਡ * channel->ਖੰਡ_ਮਾਪ;

	ਜੇ(!_RawWrite(Nt_ਅੰਕੜੇ->Handle, ਵਿੱਥ_2, ਮਾਪ_ਮੰਜ਼ਿਲ_Write, buf, &Errno))
	{
		ਜੇ (channel->write_error)
		{
			ਵਾਪਸ (channel->write_error)(channel, ਖੰਡ, ਗਿਣਤੀ_2, buf,
						ਮਾਪ_ਮੰਜ਼ਿਲ_Write, 0, Errno);
		}
		ਨਹੀਂਤਾਂ
		{
			ਵਾਪਸ Errno;
		}
	}


	//
	// Stash a copy.
	//

	ਜੇ(ਮਾਪ_ਮੰਜ਼ਿਲ_Write >= Nt_ਅੰਕੜੇ->ਬਫਰ_ਮਾਪ)
	{
		Nt_ਅੰਕੜੇ->ਬਫਰ_ਖੰਡ_ਸੰਖਿਆ = ਖੰਡ;
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(Nt_ਅੰਕੜੇ->ਬਫਰ_2, buf, Nt_ਅੰਕੜੇ->ਬਫਰ_ਮਾਪ);
	}

	Nt_ਅੰਕੜੇ->Written = TRUE;

	ਵਾਪਸ 0;

}



//
// Flush data buffers to disk.  Since we are currently using a
// write-through cache, this is a no-op.
//

ਸਥਿਰ
errcode_t
nt_flush(io_channel channel)
{
	PNT_ਨਿੱਜੀ_ਅੰਕੜੇ Nt_ਅੰਕੜੇ = ਨਿਰਾ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_ਅੰਕੜੇ = (PNT_ਨਿੱਜੀ_ਅੰਕੜੇ) channel->ਨਿੱਜੀ_ਅੰਕੜੇ;
	EXT2_CHECK_MAGIC(Nt_ਅੰਕੜੇ, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	ਜੇ(Nt_ਅੰਕੜੇ->OpenedReadonly)
	{
		ਵਾਪਸ 0; // EACCESS;
	}


	//
	// Flush file buffers.
	//

	_FlushDrive(Nt_ਅੰਕੜੇ->Handle);


	//
	// Test and correct partition type.
	//

	ਜੇ(Nt_ਅੰਕੜੇ->Written)
	{
		ਸੈੱਟ_Part_Type(Nt_ਅੰਕੜੇ->Handle, 0x83);
	}

	ਵਾਪਸ 0;
}


