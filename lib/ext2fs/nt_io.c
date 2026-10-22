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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_CONFIG_H
#ಅನ್ತರ್ಭಾವ "config.h"
#ಆದರೆ_ಅಂತ್ಯ


//
// I need some warnings to disable...
//


#ಸೂಚನೆ warning(disable:4514) // unreferenced inline function has been removed
#ಸೂಚನೆ warning(ತಳ್ಳು,4)

#ಸೂಚನೆ warning(disable:4201) // nonstandard extension used : nameless struct/union)
#ಸೂಚನೆ warning(disable:4214) // nonstandard extension used : bit field types other than int
#ಸೂಚನೆ warning(disable:4115) // named type definition in parentheses

#ಅನ್ತರ್ಭಾವ <ntddk.h>
#ಅನ್ತರ್ಭಾವ <ntdddisk.h>
#ಅನ್ತರ್ಭಾವ <ntstatus.h>

#ಸೂಚನೆ warning(ತೆಗೆ)


//
// Some native APIs.
//

NTSYSAPI
ULONG
NTAPI
Rtl_Nt_ಸ್ಥಿತಿ_ಗಮ್ಯ_Dos_Error(
    ಒಳ_2 NTSTATUS ಸ್ಥಿತಿ_2
   );

NTSYSAPI
NTSTATUS
NTAPI
NtClose(
    ಒಳ_2 HANDLE Handle
   );


NTSYSAPI
NTSTATUS
NTAPI
NtOpenFile(
    ಹೊರಹೋಗು_2 PHANDLE FileHandle,
    ಒಳ_2 ACCESS_ಮುಖವಾಡ DesiredAccess,
    ಒಳ_2 POBJECT_ಗುಣಲಕ್ಷಣಗಳು Object_ಗುಣಲಕ್ಷಣಗಳು,
    ಹೊರಹೋಗು_2 PIO_ಸ್ಥಿತಿ_ಖಂಡ Io_ಸ್ಥಿತಿ_ಖಂಡ,
    ಒಳ_2 ULONG ShareAccess,
    ಒಳ_2 ULONG OpenOptions
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFlushBuffersFile(
    ಒಳ_2 HANDLE FileHandle,
    ಹೊರಹೋಗು_2 PIO_ಸ್ಥಿತಿ_ಖಂಡ Io_ಸ್ಥಿತಿ_ಖಂಡ
   );


NTSYSAPI
NTSTATUS
NTAPI
NtReadFile(
    ಒಳ_2 HANDLE FileHandle,
    ಒಳ_2 HANDLE Event OPTIONAL,
    ಒಳ_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    ಒಳ_2 PVOID ApcContext OPTIONAL,
    ಹೊರಹೋಗು_2 PIO_ಸ್ಥಿತಿ_ಖಂಡ Io_ಸ್ಥಿತಿ_ಖಂಡ,
    ಹೊರಹೋಗು_2 PVOID ಬಫರ್_2,
    ಒಳ_2 ULONG ಉದ್ದಳತೆ_2,
    ಒಳ_2 PLARGE_INTEGER Byte_ಸ್ಥಾನಾಂತರ OPTIONAL,
    ಒಳ_2 PULONG ಕೀಲಿ_2 OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtWriteFile(
    ಒಳ_2 HANDLE FileHandle,
    ಒಳ_2 HANDLE Event OPTIONAL,
    ಒಳ_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    ಒಳ_2 PVOID ApcContext OPTIONAL,
    ಹೊರಹೋಗು_2 PIO_ಸ್ಥಿತಿ_ಖಂಡ Io_ಸ್ಥಿತಿ_ಖಂಡ,
    ಒಳ_2 PVOID ಬಫರ್_2,
    ಒಳ_2 ULONG ಉದ್ದಳತೆ_2,
    ಒಳ_2 PLARGE_INTEGER Byte_ಸ್ಥಾನಾಂತರ OPTIONAL,
    ಒಳ_2 PULONG ಕೀಲಿ_2 OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtDeviceIoControlFile(
    ಒಳ_2 HANDLE FileHandle,
    ಒಳ_2 HANDLE Event OPTIONAL,
    ಒಳ_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    ಒಳ_2 PVOID ApcContext OPTIONAL,
    ಹೊರಹೋಗು_2 PIO_ಸ್ಥಿತಿ_ಖಂಡ Io_ಸ್ಥಿತಿ_ಖಂಡ,
    ಒಳ_2 ULONG Io_Control_ಸಂಕೇತ,
    ಒಳ_2 PVOID ಒಳಹರಿವು_ಬಫರ್ OPTIONAL,
    ಒಳ_2 ULONG ಒಳಹರಿವು_ಬಫರ್_ಉದ್ದಳತೆ,
    ಹೊರಹೋಗು_2 PVOID Output_ಬಫರ್ OPTIONAL,
    ಒಳ_2 ULONG Output_ಬಫರ್_ಉದ್ದಳತೆ
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFsControlFile(
    ಒಳ_2 HANDLE FileHandle,
    ಒಳ_2 HANDLE Event OPTIONAL,
    ಒಳ_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    ಒಳ_2 PVOID ApcContext OPTIONAL,
    ಹೊರಹೋಗು_2 PIO_ಸ್ಥಿತಿ_ಖಂಡ Io_ಸ್ಥಿತಿ_ಖಂಡ,
    ಒಳ_2 ULONG Io_Control_ಸಂಕೇತ,
    ಒಳ_2 PVOID ಒಳಹರಿವು_ಬಫರ್ OPTIONAL,
    ಒಳ_2 ULONG ಒಳಹರಿವು_ಬಫರ್_ಉದ್ದಳತೆ,
    ಹೊರಹೋಗು_2 PVOID Output_ಬಫರ್ OPTIONAL,
    ಒಳ_2 ULONG Output_ಬಫರ್_ಉದ್ದಳತೆ
    );


NTSYSAPI
NTSTATUS
NTAPI
NtDelayExecution(
    ಒಳ_2 BOOLEAN Alertable,
    ಒಳ_2 PLARGE_INTEGER Interval
    );


#ವ್ಯಾಖ್ಯಾನಿಸು FSCTL_ಬೀಗ_VOLUME               CTL_ಸಂಕೇತ(FILE_DEVICE_FILE_SYSTEM, 6, ವಿಧಾನ_BUFFERED, FILE_ANY_ACCESS)
#ವ್ಯಾಖ್ಯಾನಿಸು FSCTL_UNLOCK_VOLUME             CTL_ಸಂಕೇತ(FILE_DEVICE_FILE_SYSTEM, 7, ವಿಧಾನ_BUFFERED, FILE_ANY_ACCESS)
#ವ್ಯಾಖ್ಯಾನಿಸು FSCTL_DISMOUNT_VOLUME           CTL_ಸಂಕೇತ(FILE_DEVICE_FILE_SYSTEM, 8, ವಿಧಾನ_BUFFERED, FILE_ANY_ACCESS)
#ವ್ಯಾಖ್ಯಾನಿಸು FSCTL_IS_VOLUME_MOUNTED         CTL_ಸಂಕೇತ(FILE_DEVICE_FILE_SYSTEM,10, ವಿಧಾನ_BUFFERED, FILE_ANY_ACCESS)


//
// useful macros
//

#ವ್ಯಾಖ್ಯಾನಿಸು BooleanFlagOn(Flags,SingleFlag) ((BOOLEAN)((((Flags) & (SingleFlag)) != 0)))


//
// Include Win32 error codes.
//

#ಅನ್ತರ್ಭಾವ <winerror.h>

//
// standard stuff
//

#ಅನ್ತರ್ಭಾವ <assert.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <malloc.h>

#ಅನ್ತರ್ಭಾವ <linux/types.h>
#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ <errno.h>

#ಅನ್ತರ್ಭಾವ "et/com_err.h"
#ಅನ್ತರ್ಭಾವ "ext2fs/ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs/ext2_err.h"




//
// For checking structure magic numbers...
//


#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_CHECK_MAGIC(ರಚನೆ, ಸಂಕೇತ_2) \
	  ಆದರೆ ((ರಚನೆ)->magic != (ಸಂಕೇತ_2)) ಹಿಂತಿರುಗಿ (ಸಂಕೇತ_2)

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2_ET_MAGIC_NT_IO_CHANNEL  0x10ed


//
// Private data block
//

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ NT_ಖಾಸಗಿ_ದತ್ತಾಂಶ {
	ಪೂರ್ಣಾಂಕ	   magic;
	HANDLE Handle;
	ಪೂರ್ಣಾಂಕ	   Flags;
	PCHAR  ಬಫರ್_2;
	__u32  ಬಫರ್_ಖಂಡ_ಸಂಖ್ಯೆ;
	ULONG  ಬಫರ್_ಗಾತ್ರ;
	BOOLEAN OpenedReadonly;
	BOOLEAN Written;
}NT_ಖಾಸಗಿ_ದತ್ತಾಂಶ_2, *PNT_ಖಾಸಗಿ_ದತ್ತಾಂಶ;



//
// Standard interface prototypes
//

ಸ್ಥಿರ errcode_t nt_open(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ flags, io_channel *channel);
ಸ್ಥಿರ errcode_t nt_close(io_channel channel);
ಸ್ಥಿರ errcode_t nt_ಗಣ_blksize(io_channel channel, ಪೂರ್ಣಾಂಕ blksize);
ಸ್ಥಿರ errcode_t nt_read_blk(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
			       ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಸ್ಥಿರ errcode_t nt_write_blk(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
				ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಸ್ಥಿರ errcode_t nt_flush(io_channel channel);

ಸ್ಥಿರ ರಚನೆ struct_io_manager struct_nt_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "NT I/O Manager",
	.open		= nt_open,
	.close		= nt_close,
	.ಗಣ_blksize	= nt_ಗಣ_blksize,
	.read_blk	= nt_read_blk,
	.write_blk	= nt_write_blk,
	.flush		= nt_flush
};

//
// function to get API
//

io_manager nt_io_manager()
{
	ಹಿಂತಿರುಗಿ &struct_nt_manager;
}





//
// This is a code to convert Win32 errors to unix errno
//

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
	ULONG WinError;
	ಪೂರ್ಣಾಂಕ errnocode;
}ERROR_ನಮೂದು;

ಸ್ಥಿರ ERROR_ನಮೂದು Error_ಕೋಷ್ಟಕ[] = {
        {  ERROR_INVALID_FUNCTION,       EINVAL    },
        {  ERROR_FILE_NOT_ಸಿಕ್ಕಿತು,         ENOENT    },
        {  ERROR_ಮಾರ್ಗ_NOT_ಸಿಕ್ಕಿತು,         ENOENT    },
        {  ERROR_TOO_MANY_OPEN_FILES,    EMFILE    },
        {  ERROR_ACCESS_DENIED,          EACCES    },
        {  ERROR_INVALID_HANDLE,         EBADF     },
        {  ERROR_ARENA_TRASHED,          ENOMEM    },
        {  ERROR_NOT_ENOUGH_ಸ್ಮರಣೆ,      ENOMEM    },
        {  ERROR_INVALID_ಖಂಡ,          ENOMEM    },
        {  ERROR_BAD_ENVIRONMENT,        E2BIG     },
        {  ERROR_BAD_FORMAT,             ENOEXEC   },
        {  ERROR_INVALID_ACCESS,         EINVAL    },
        {  ERROR_INVALID_ದತ್ತಾಂಶ,           EINVAL    },
        {  ERROR_INVALID_DRIVE,          ENOENT    },
        {  ERROR_CURRENT_DIRECTORY,      EACCES    },
        {  ERROR_NOT_SAME_DEVICE,        EXDEV     },
        {  ERROR_NO_ಇನ್ನಷ್ಟು_FILES,          ENOENT    },
        {  ERROR_ಬೀಗ_VIOLATION,         EACCES    },
        {  ERROR_BAD_NETPATH,            ENOENT    },
        {  ERROR_ಜಾಲ_ACCESS_DENIED,  EACCES    },
        {  ERROR_BAD_NET_NAME,           ENOENT    },
        {  ERROR_FILE_EXISTS,            EEXIST    },
        {  ERROR_CANNOT_MAKE,            EACCES    },
        {  ERROR_FAIL_I24,               EACCES    },
        {  ERROR_INVALID_PARAMETER,      EINVAL    },
        {  ERROR_NO_PROC_SLOTS,          EAGAIN    },
        {  ERROR_DRIVE_LOCKED,           EACCES    },
        {  ERROR_BROKEN_PIPE,            EPIPE     },
        {  ERROR_DISK_ತುಂಬಿದ,              ENOSPC    },
        {  ERROR_INVALID_ಗುರಿ_HANDLE,  EBADF     },
        {  ERROR_INVALID_HANDLE,         EINVAL    },
        {  ERROR_WAIT_NO_CHILDREN,       ECHILD    },
        {  ERROR_ಮಗು_NOT_COMPLETE,     ECHILD    },
        {  ERROR_ನೇರ_ACCESS_HANDLE,   EBADF     },
        {  ERROR_NEGATIVE_SEEK,          EINVAL    },
        {  ERROR_SEEK_ON_DEVICE,         EACCES    },
        {  ERROR_DIR_NOT_ಖಾಲಿಗಳು,          ENOTEMPTY },
        {  ERROR_NOT_LOCKED,             EACCES    },
        {  ERROR_BAD_PATHNAME,           ENOENT    },
        {  ERROR_MAX_THRDS_REACHED,      EAGAIN    },
        {  ERROR_ಬೀಗ_FAILED,            EACCES    },
        {  ERROR_ALREADY_EXISTS,         EEXIST    },
        {  ERROR_FILENAME_EXCED_RANGE,   ENOENT    },
        {  ERROR_NESTING_NOT_ALLOWED,    EAGAIN    },
        {  ERROR_NOT_ENOUGH_QUOTA,       ENOMEM    }
};




ಸ್ಥಿರ
ಚಿಹ್ನೆರಹಿತ
_MapDosError (
    ಒಳ_2 ULONG WinError
   )
{
	ಪೂರ್ಣಾಂಕ i;

	//
	// Lookup
	//

	ಗಾಗಿ (i = 0; i < (ಗಾತ್ರ(Error_ಕೋಷ್ಟಕ)/ಗಾತ್ರ(Error_ಕೋಷ್ಟಕ[0])); ++i)
	{
		ಆದರೆ (WinError == Error_ಕೋಷ್ಟಕ[i].WinError)
		{
			ಹಿಂತಿರುಗಿ Error_ಕೋಷ್ಟಕ[i].errnocode;
		}
	}

	//
	// not in table. Check ranges
	//

	ಆದರೆ ((WinError >= ERROR_WRITE_PROTECT) &&
		(WinError <= ERROR_SHARING_ಬಫರ್_EXCEEDED))
	{
		ಹಿಂತಿರುಗಿ EACCES;
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((WinError >= ERROR_INVALID_STARTING_CODESEG) &&
			 (WinError <= ERROR_INFLOOP_ಒಳ_RELOC_CHAIN))
	{
		ಹಿಂತಿರುಗಿ ENOEXEC;
	}
	ಇಲ್ಲದಿದ್ದರೆ
	{
		ಹಿಂತಿರುಗಿ EINVAL;
	}
}







//
// Function to map NT status to dos error.
//

ಸ್ಥಿರ
__inline
ಚಿಹ್ನೆರಹಿತ
Map_Nt_ಸ್ಥಿತಿ(
    ಒಳ_2 NTSTATUS ಸ್ಥಿತಿ_2
   )
{
	ಹಿಂತಿರುಗಿ _MapDosError(Rtl_Nt_ಸ್ಥಿತಿ_ಗಮ್ಯ_Dos_Error(ಸ್ಥಿತಿ_2));
}





//
// Helper functions to make things easier
//

ಸ್ಥಿರ
NTSTATUS
_OpenNtName(
    ಒಳ_2 PCSTR Name,
    ಒಳ_2 BOOLEAN Readonly,
    ಹೊರಹೋಗು_2 PHANDLE Handle,
    ಹೊರಹೋಗು_2 PBOOLEAN OpenedReadonly OPTIONAL
   )
{
	UNICODE_STRING UnicodeString;
	ANSI_STRING    AnsiString;
	WCHAR ಬಫರ್_2[512];
	NTSTATUS ಸ್ಥಿತಿ_2;
	OBJECT_ಗುಣಲಕ್ಷಣಗಳು Object_ಗುಣಲಕ್ಷಣಗಳು;
	IO_ಸ್ಥಿತಿ_ಖಂಡ Io_ಸ್ಥಿತಿ_ಖಂಡ;

	//
	// Make Unicode name from input string
	//

	UnicodeString.ಬಫರ್_2 = &ಬಫರ್_2[0];
	UnicodeString.ಉದ್ದಳತೆ_2 = 0;
	UnicodeString.ಗರಿಷ್ಠ_ಉದ್ದಳತೆ = ಗಾತ್ರ(ಬಫರ್_2); // in bytes!!!

	RtlInitAnsiString(&AnsiString, Name);

	ಸ್ಥಿತಿ_2 = Rtl_Ansi_String_ಗಮ್ಯ_Unicode_String(&UnicodeString, &AnsiString, FALSE);

	ಆದರೆ(!NT_SUCCESS(ಸ್ಥಿತಿ_2))
	{
		ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2; // Unmappable character?
	}

	//
	// Initialize object
	//

	Initialize_Object_ಗುಣಲಕ್ಷಣಗಳು(&Object_ಗುಣಲಕ್ಷಣಗಳು,
							   &UnicodeString,
							   OBJ_CASE_INSENSITIVE,
							   ಖಾಲಿ,
							   ಖಾಲಿ );

	//
	// Try to open it in initial mode
	//

	ಆದರೆ(ವಾದ_PRESENT(OpenedReadonly))
	{
		*OpenedReadonly = Readonly;
	}


	ಸ್ಥಿತಿ_2 = NtOpenFile(Handle,
						SYNCHRONIZE | FILE_READ_ದತ್ತಾಂಶ | (Readonly ? 0 : FILE_WRITE_ದತ್ತಾಂಶ),
						&Object_ಗುಣಲಕ್ಷಣಗಳು,
						&Io_ಸ್ಥಿತಿ_ಖಂಡ,
						FILE_SHARE_WRITE | FILE_SHARE_READ,
						FILE_SYNCHRONOUS_IO_NONALERT);

	ಆದರೆ(!NT_SUCCESS(ಸ್ಥಿತಿ_2))
	{
		//
		// Maybe was just mounted? wait 0.5 sec and retry.
		//

		LARGE_INTEGER Interval;
		Interval.QuadPart = -5000000; // 0.5 sec. from now

		NtDelayExecution(FALSE, &Interval);

		ಸ್ಥಿತಿ_2 = NtOpenFile(Handle,
							SYNCHRONIZE | FILE_READ_ದತ್ತಾಂಶ | (Readonly ? 0 : FILE_WRITE_ದತ್ತಾಂಶ),
							&Object_ಗುಣಲಕ್ಷಣಗಳು,
							&Io_ಸ್ಥಿತಿ_ಖಂಡ,
							FILE_SHARE_WRITE | FILE_SHARE_READ,
							FILE_SYNCHRONOUS_IO_NONALERT);

		//
		// Try to satisfy mode
		//

		ಆದರೆ((ಸ್ಥಿತಿ_ACCESS_DENIED == ಸ್ಥಿತಿ_2) && !Readonly)
		{
			ಆದರೆ(ವಾದ_PRESENT(OpenedReadonly))
			{
				*OpenedReadonly = TRUE;
			}

			ಸ್ಥಿತಿ_2 = NtOpenFile(Handle,
							SYNCHRONIZE | FILE_READ_ದತ್ತಾಂಶ,
							&Object_ಗುಣಲಕ್ಷಣಗಳು,
							&Io_ಸ್ಥಿತಿ_ಖಂಡ,
							FILE_SHARE_WRITE | FILE_SHARE_READ,
							FILE_SYNCHRONOUS_IO_NONALERT);
		}
	}



	//
	//ಮುಗಿದ
	//

	ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
}


ಸ್ಥಿರ
NTSTATUS
_OpenDriveLetter(
    ಒಳ_2 CHAR Letter,
    ಒಳ_2 BOOLEAN ReadOnly,
    ಹೊರಹೋಗು_2 PHANDLE Handle,
    ಹೊರಹೋಗು_2 PBOOLEAN OpenedReadonly OPTIONAL
   )
{
	CHAR ಬಫರ್_2[100];

	sprintf(ಬಫರ್_2, "\\DosDevices\\%c:", Letter);

	ಹಿಂತಿರುಗಿ _OpenNtName(ಬಫರ್_2, ReadOnly, Handle, OpenedReadonly);
}


//
// Flush device
//

ಸ್ಥಿರ
__inline
NTSTATUS
_FlushDrive(
		ಒಳ_2 HANDLE Handle
		)
{
	IO_ಸ್ಥಿತಿ_ಖಂಡ Io_ಸ್ಥಿತಿ_ಖಂಡ;
	ಹಿಂತಿರುಗಿ NtFlushBuffersFile(Handle, &Io_ಸ್ಥಿತಿ_ಖಂಡ);
}


//
// lock drive
//

ಸ್ಥಿರ
__inline
NTSTATUS
ಬೀಗ_Drive(
		ಒಳ_2 HANDLE Handle
		)
{
	IO_ಸ್ಥಿತಿ_ಖಂಡ Io_ಸ್ಥಿತಿ_ಖಂಡ;
	ಹಿಂತಿರುಗಿ NtFsControlFile(Handle, 0, 0, 0, &Io_ಸ್ಥಿತಿ_ಖಂಡ, FSCTL_ಬೀಗ_VOLUME, 0, 0, 0, 0);
}


//
// unlock drive
//

ಸ್ಥಿರ
__inline
NTSTATUS
_UnlockDrive(
	ಒಳ_2 HANDLE Handle
	)
{
	IO_ಸ್ಥಿತಿ_ಖಂಡ Io_ಸ್ಥಿತಿ_ಖಂಡ;
	ಹಿಂತಿರುಗಿ NtFsControlFile(Handle, 0, 0, 0, &Io_ಸ್ಥಿತಿ_ಖಂಡ, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0);
}

ಸ್ಥಿರ
__inline
NTSTATUS
_DismountDrive(
	ಒಳ_2 HANDLE Handle
	)
{
	IO_ಸ್ಥಿತಿ_ಖಂಡ Io_ಸ್ಥಿತಿ_ಖಂಡ;
	ಹಿಂತಿರುಗಿ NtFsControlFile(Handle, 0, 0, 0, &Io_ಸ್ಥಿತಿ_ಖಂಡ, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0);
}


//
// is mounted
//

ಸ್ಥಿರ
__inline
BOOLEAN
_IsMounted(
	ಒಳ_2 HANDLE Handle
	)
{
	IO_ಸ್ಥಿತಿ_ಖಂಡ Io_ಸ್ಥಿತಿ_ಖಂಡ;
	NTSTATUS ಸ್ಥಿತಿ_2;
	ಸ್ಥಿತಿ_2 = NtFsControlFile(Handle, 0, 0, 0, &Io_ಸ್ಥಿತಿ_ಖಂಡ, FSCTL_IS_VOLUME_MOUNTED, 0, 0, 0, 0);
	ಹಿಂತಿರುಗಿ (BOOLEAN)(ಸ್ಥಿತಿ_SUCCESS == ಸ್ಥಿತಿ_2);
}


ಸ್ಥಿರ
__inline
NTSTATUS
_CloseDisk(
		ಒಳ_2 HANDLE Handle
		)
{
	ಹಿಂತಿರುಗಿ NtClose(Handle);
}




//
// Make NT name from any recognized name
//

ಸ್ಥಿರ
PCSTR
_NormalizeDeviceName(
    ಒಳ_2 PCSTR Device,
    ಒಳ_2 PSTR Normalized_Device_Name_ಬಫರ್
   )
{
	ಪೂರ್ಣಾಂಕ ವಿಭಜನೆ_ಸಂಖ್ಯೆ = -1;
	UCHAR Disk_ಸಂಖ್ಯೆ;
	PSTR p;


	//
	// Do not try to parse NT name
	//

	ಆದರೆ('\\' == *Device)
		ಹಿಂತಿರುಗಿ Device;



	//
	// Strip leading '/dev/' if any
	//

	ಆದರೆ(('/' == *(Device)) &&
		('d' == *(Device + 1)) &&
		('e' == *(Device + 2)) &&
		('v' == *(Device + 3)) &&
		('/' == *(Device + 4)))
	{
		Device += 5;
	}

	ಆದರೆ('\0' == *Device)
	{
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}


	//
	// forms: hda[n], fd[n]
	//

	ಆದರೆ('d' != *(Device + 1))
	{
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}

	ಆದರೆ('h' == *Device)
	{
		ಆದರೆ((*(Device + 2) < 'a') || (*(Device + 2) > ('a' + 9)) ||
		   ((*(Device + 3) != '\0') &&
			((*(Device + 4) != '\0') ||
			 ((*(Device + 3) < '0') || (*(Device + 3) > '9'))
			)
		   )
		  )
		{
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;
		}

		Disk_ಸಂಖ್ಯೆ = (UCHAR)(*(Device + 2) - 'a');

		ಆದರೆ(*(Device + 3) != '\0')
		{
			ವಿಭಜನೆ_ಸಂಖ್ಯೆ = (*(Device + 3) - '0');
		}

	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ('f' == *Device)
	{
		//
		// 3-d letter should be a digit.
		//

		ಆದರೆ((*(Device + 3) != '\0') ||
		   (*(Device + 2) < '0') || (*(Device + 2) > '9'))
		{
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;
		}

		Disk_ಸಂಖ್ಯೆ = (UCHAR)(*(Device + 2) - '0');

	}
	ಇಲ್ಲದಿದ್ದರೆ
	{
		//
		// invalid prefix
		//

		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}



	//
	//ಪೂರ್ವಪ್ರತ್ಯಯ
	//

	strcpy(Normalized_Device_Name_ಬಫರ್, "\\Device\\");

	//
	// Media name
	//

	ಆಯ್ಕೆ(*Device)
	{

	ಸ್ಥಿತಿ 'f':
		strcat(Normalized_Device_Name_ಬಫರ್, "Floppy0");
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ 'h':
		strcat(Normalized_Device_Name_ಬಫರ್, "Harddisk0");
		ನಿಲ್ಲಿಸು;
	}


	p = Normalized_Device_Name_ಬಫರ್ + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(Normalized_Device_Name_ಬಫರ್) - 1;
	*p = (CHAR)(*p + Disk_ಸಂಖ್ಯೆ);


	//
	// Partition nr.
	//

	ಆದರೆ(ವಿಭಜನೆ_ಸಂಖ್ಯೆ >= 0)
	{
		strcat(Normalized_Device_Name_ಬಫರ್, "\\Partition0");

		p = Normalized_Device_Name_ಬಫರ್ + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(Normalized_Device_Name_ಬಫರ್) - 1;
		*p = (CHAR)(*p + ವಿಭಜನೆ_ಸಂಖ್ಯೆ);
	}


	ಹಿಂತಿರುಗಿ Normalized_Device_Name_ಬಫರ್;
}




ಸ್ಥಿರ
VOID
Get_Device_ಗಾತ್ರ(
    ಒಳ_2 HANDLE h,
    ಹೊರಹೋಗು_2 ಚಿಹ್ನೆರಹಿತ __int64 *Fs_ಗಾತ್ರ
   )
{
	ವಿಭಜನೆ_INFORMATION pi;
	DISK_GEOMETRY gi;
	NTSTATUS ಸ್ಥಿತಿ_2;
	IO_ಸ್ಥಿತಿ_ಖಂಡ Io_ಸ್ಥಿತಿ_ಖಂಡ;

	//
	// Zero it
	//

	*Fs_ಗಾತ್ರ = 0;

	//
	// Call driver
	//

	Rtl_Zero_ಸ್ಮರಣೆ(&pi, ಗಾತ್ರ(ವಿಭಜನೆ_INFORMATION));

	ಸ್ಥಿತಿ_2 = NtDeviceIoControlFile(
		h, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, &Io_ಸ್ಥಿತಿ_ಖಂಡ, IOCTL_DISK_GET_ವಿಭಜನೆ_INFO,
		&pi, ಗಾತ್ರ(ವಿಭಜನೆ_INFORMATION),
		&pi, ಗಾತ್ರ(ವಿಭಜನೆ_INFORMATION));


	ಆದರೆ(NT_SUCCESS(ಸ್ಥಿತಿ_2))
	{
		*Fs_ಗಾತ್ರ = pi.ವಿಭಜನೆ_ಉದ್ದಳತೆ.QuadPart;
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸ್ಥಿತಿ_INVALID_DEVICE_REQUEST == ಸ್ಥಿತಿ_2)
	{
		//
		// No partitions: get device info.
		//

		Rtl_Zero_ಸ್ಮರಣೆ(&gi, ಗಾತ್ರ(DISK_GEOMETRY));

		ಸ್ಥಿತಿ_2 = NtDeviceIoControlFile(
				h, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, &Io_ಸ್ಥಿತಿ_ಖಂಡ, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, ಗಾತ್ರ(DISK_GEOMETRY),
				&gi, ಗಾತ್ರ(DISK_GEOMETRY));


		ಆದರೆ(NT_SUCCESS(ಸ್ಥಿತಿ_2))
		{
			*Fs_ಗಾತ್ರ =
				gi.ಬೈಟುಗಳು_ಪ್ರತಿ_Sector *
				gi.Sectors_ಪ್ರತಿ_Track *
				gi.Tracks_ಪ್ರತಿ_Cylinder *
				gi.Cylinders.QuadPart;
		}

	}
}



//
// Open device by name.
//

ಸ್ಥಿರ
BOOLEAN
_Ext2OpenDevice(
    ಒಳ_2 PCSTR Name,
    ಒಳ_2 BOOLEAN ReadOnly,
    ಹೊರಹೋಗು_2 PHANDLE Handle,
    ಹೊರಹೋಗು_2 PBOOLEAN OpenedReadonly OPTIONAL,
    ಹೊರಹೋಗು_2 ಚಿಹ್ನೆರಹಿತ *Errno OPTIONAL
   )
{
	CHAR NormalizedDeviceName[512];
	NTSTATUS ಸ್ಥಿತಿ_2;

	ಆದರೆ(ಖಾಲಿ == Name)
	{
		//
		// Set not found
		//

		ಆದರೆ(ವಾದ_PRESENT(Errno))
			*Errno = ENOENT;

		ಹಿಂತಿರುಗಿ FALSE;
	}


	ಆದರೆ((((*Name) | 0x20) >= 'a') && (((*Name) | 0x20) <= 'z') &&
		(':' == *(Name + 1)) && ('\0' == *(Name + 2)))
	{
		ಸ್ಥಿತಿ_2 = _OpenDriveLetter(*Name, ReadOnly, Handle, OpenedReadonly);
	}
	ಇಲ್ಲದಿದ್ದರೆ
	{
		//
		// Make name
		//

		Name = _NormalizeDeviceName(Name, NormalizedDeviceName);

		ಆದರೆ(ಖಾಲಿ == Name)
		{
			//
			// Set not found
			//

			ಆದರೆ(ವಾದ_PRESENT(Errno))
				*Errno = ENOENT;

			ಹಿಂತಿರುಗಿ FALSE;
		}

		//
		// Try to open it
		//

		ಸ್ಥಿತಿ_2 = _OpenNtName(Name, ReadOnly, Handle, OpenedReadonly);
	}


	ಆದರೆ(!NT_SUCCESS(ಸ್ಥಿತಿ_2))
	{
		ಆದರೆ(ವಾದ_PRESENT(Errno))
			*Errno = Map_Nt_ಸ್ಥಿತಿ(ಸ್ಥಿತಿ_2);

		ಹಿಂತಿರುಗಿ FALSE;
	}

	ಹಿಂತಿರುಗಿ TRUE;
}


//
// Raw block io. Sets dos errno
//

ಸ್ಥಿರ
BOOLEAN
ಖಂಡ_Io(
    ಒಳ_2 HANDLE Handle,
    ಒಳ_2 LARGE_INTEGER ಸ್ಥಾನಾಂತರ_2,
    ಒಳ_2 ULONG ಬೈಟುಗಳು_2,
    ಒಳ_2 ಹೊರಹೋಗು_2 PCHAR ಬಫರ್_2,
    ಒಳ_2 BOOLEAN Read,
    ಹೊರಹೋಗು_2 ಚಿಹ್ನೆರಹಿತ* Errno
   )
{
	IO_ಸ್ಥಿತಿ_ಖಂಡ Io_ಸ್ಥಿತಿ_ಖಂಡ;
	NTSTATUS ಸ್ಥಿತಿ_2;

	//
	// Should be aligned
	//

	ASSERT(0 == (ಬೈಟುಗಳು_2 % 512));
	ASSERT(0 == (ಸ್ಥಾನಾಂತರ_2.ಕೆಳಗಿನ_Part % 512));


	//
	// perform io
	//

	ಆದರೆ(Read)
	{
		ಸ್ಥಿತಿ_2 = NtReadFile(Handle, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ,
			&Io_ಸ್ಥಿತಿ_ಖಂಡ, ಬಫರ್_2, ಬೈಟುಗಳು_2, &ಸ್ಥಾನಾಂತರ_2, ಖಾಲಿ);
	}
	ಇಲ್ಲದಿದ್ದರೆ
	{
		ಸ್ಥಿತಿ_2 = NtWriteFile(Handle, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ,
			&Io_ಸ್ಥಿತಿ_ಖಂಡ, ಬಫರ್_2, ಬೈಟುಗಳು_2, &ಸ್ಥಾನಾಂತರ_2, ಖಾಲಿ);
	}


	//
	// translate error
	//

	ಆದರೆ(NT_SUCCESS(ಸ್ಥಿತಿ_2))
	{
		*Errno = 0;
		ಹಿಂತಿರುಗಿ TRUE;
	}

	*Errno = Map_Nt_ಸ್ಥಿತಿ(ಸ್ಥಿತಿ_2);

	ಹಿಂತಿರುಗಿ FALSE;
}



__inline
BOOLEAN
_RawWrite(
    ಒಳ_2 HANDLE Handle,
    ಒಳ_2 LARGE_INTEGER ಸ್ಥಾನಾಂತರ_2,
    ಒಳ_2 ULONG ಬೈಟುಗಳು_2,
    ಹೊರಹೋಗು_2 ಸ್ಥಿರಾಂಕ CHAR* ಬಫರ್_2,
    ಹೊರಹೋಗು_2 ಚಿಹ್ನೆರಹಿತ* Errno
   )
{
	ಹಿಂತಿರುಗಿ ಖಂಡ_Io(Handle, ಸ್ಥಾನಾಂತರ_2, ಬೈಟುಗಳು_2, (PCHAR)ಬಫರ್_2, FALSE, Errno);
}

__inline
BOOLEAN
_RawRead(
    ಒಳ_2 HANDLE Handle,
    ಒಳ_2 LARGE_INTEGER ಸ್ಥಾನಾಂತರ_2,
    ಒಳ_2 ULONG ಬೈಟುಗಳು_2,
    ಒಳ_2 PCHAR ಬಫರ್_2,
    ಹೊರಹೋಗು_2 ಚಿಹ್ನೆರಹಿತ* Errno
   )
{
	ಹಿಂತಿರುಗಿ ಖಂಡ_Io(Handle, ಸ್ಥಾನಾಂತರ_2, ಬೈಟುಗಳು_2, ಬಫರ್_2, TRUE, Errno);
}



__inline
BOOLEAN
ಗಣ_Part_Type(
    ಒಳ_2 HANDLE Handle,
    ಒಳ_2 UCHAR Type
   )
{
	IO_ಸ್ಥಿತಿ_ಖಂಡ Io_ಸ್ಥಿತಿ_ಖಂಡ;
	ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_SUCCESS == NtDeviceIoControlFile(
												   Handle, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ, &Io_ಸ್ಥಿತಿ_ಖಂಡ, IOCTL_DISK_ಗಣ_ವಿಭಜನೆ_INFO,
												   &Type, ಗಾತ್ರ(Type),
												   ಖಾಲಿ, 0);
}



//--------------------- interface part

//
// Interface functions.
// Is_mounted is set to 1 if the device is mounted, 0 otherwise
//

errcode_t
ext2fs_check_if_mounted(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, ಪೂರ್ಣಾಂಕ *mount_flags)
{
	HANDLE h;
	BOOLEAN Readonly;

	*mount_flags = 0;

	ಆದರೆ(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, ಖಾಲಿ))
	{
		ಹಿಂತಿರುಗಿ 0;
	}


	__try{
		*mount_flags &= _IsMounted(h) ? EXT2_MF_MOUNTED : 0;
	}
	__finally{
		_CloseDisk(h);
	}

	ಹಿಂತಿರುಗಿ 0;
}



//
// Returns the number of blocks in a partition
//

ಸ್ಥಿರ __int64 Fs_ಗಾತ್ರ = 0;
ಸ್ಥಿರ ಅಕ್ಷರ knowndevice[1024] = "";


errcode_t
ext2fs_get_device_ಗಾತ್ರ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, ಪೂರ್ಣಾಂಕ blocksize,
				 blk_t *retblocks)
{
	HANDLE h;
	BOOLEAN Readonly;

	ಆದರೆ((0 == Fs_ಗಾತ್ರ) || (0 != strcmp(knowndevice, file)))
	{

		ಆದರೆ(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, ಖಾಲಿ))
		{
			ಹಿಂತಿರುಗಿ 0;
		}


		__try{

			//
			// Get size
			//

			Get_Device_ಗಾತ್ರ(h, &Fs_ಗಾತ್ರ);
			strcpy(knowndevice, file);
		}
		__finally{
			_CloseDisk(h);
		}

	}

	*retblocks = (blk_t)(ಚಿಹ್ನೆರಹಿತ __int64)(Fs_ಗಾತ್ರ / blocksize);
	UNREFERENCED_PARAMETER(file);
	ಹಿಂತಿರುಗಿ 0;
}






//
// Table elements
//


ಸ್ಥಿರ
errcode_t
nt_open(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ flags, io_channel *channel)
{
	io_channel      io = ಖಾಲಿ;
	PNT_ಖಾಸಗಿ_ದತ್ತಾಂಶ Nt_ದತ್ತಾಂಶ = ಖಾಲಿ;
	errcode_t Errno = 0;

	//
	// Check name
	//

	ಆದರೆ (ಖಾಲಿ == name)
	{
		ಹಿಂತಿರುಗಿ EXT2_ET_BAD_DEVICE_NAME;
	}

	__try{

		//
		// Allocate channel handle
		//

		io = (io_channel) ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(ರಚನೆ struct_io_channel));

		ಆದರೆ (ಖಾಲಿ == io)
		{
			Errno = ENOMEM;
			__leave;
		}

		Rtl_Zero_ಸ್ಮರಣೆ(io, ಗಾತ್ರ(ರಚನೆ struct_io_channel));
		io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

		Nt_ದತ್ತಾಂಶ = (PNT_ಖಾಸಗಿ_ದತ್ತಾಂಶ)ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(NT_ಖಾಸಗಿ_ದತ್ತಾಂಶ_2));

		ಆದರೆ (ಖಾಲಿ == Nt_ದತ್ತಾಂಶ)
		{
			Errno = ENOMEM;
			__leave;
		}


		io->manager = nt_io_manager();
		io->name = ಸ್ಮರಣೆ_ಕೊಡು(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(name) + 1);
		ಆದರೆ (ಖಾಲಿ == io->name)
		{
			Errno = ENOMEM;
			__leave;
		}

		strcpy(io->name, name);
		io->ಖಾಸಗಿ_ದತ್ತಾಂಶ = Nt_ದತ್ತಾಂಶ;
		io->ಖಂಡ_ಗಾತ್ರ = 1024;
		io->read_error = 0;
		io->write_error = 0;
		io->refcount = 1;

		//
		// Initialize data
		//

		Rtl_Zero_ಸ್ಮರಣೆ(Nt_ದತ್ತಾಂಶ, ಗಾತ್ರ(NT_ಖಾಸಗಿ_ದತ್ತಾಂಶ_2));

		Nt_ದತ್ತಾಂಶ->magic = EXT2_ET_MAGIC_NT_IO_CHANNEL;
		Nt_ದತ್ತಾಂಶ->ಬಫರ್_ಖಂಡ_ಸಂಖ್ಯೆ = 0xffffffff;
		Nt_ದತ್ತಾಂಶ->ಬಫರ್_ಗಾತ್ರ = 1024;
		Nt_ದತ್ತಾಂಶ->ಬಫರ್_2 = ಸ್ಮರಣೆ_ಕೊಡು(Nt_ದತ್ತಾಂಶ->ಬಫರ್_ಗಾತ್ರ);

		ಆದರೆ (ಖಾಲಿ == Nt_ದತ್ತಾಂಶ->ಬಫರ್_2)
		{
			Errno = ENOMEM;
			__leave;
		}

		//
		// Open it
		//

		ಆದರೆ(!_Ext2OpenDevice(name, (BOOLEAN)!BooleanFlagOn(flags, EXT2_FLAG_RW), &Nt_ದತ್ತಾಂಶ->Handle, &Nt_ದತ್ತಾಂಶ->OpenedReadonly, &Errno))
		{
			__leave;
		}


		//
		// get size
		//

		Get_Device_ಗಾತ್ರ(Nt_ದತ್ತಾಂಶ->Handle, &Fs_ಗಾತ್ರ);
		strcpy(knowndevice, name);


		//
		// Lock/dismount
		//

		ಆದರೆ(!NT_SUCCESS(ಬೀಗ_Drive(Nt_ದತ್ತಾಂಶ->Handle)) /*|| !NT_SUCCESS(_DismountDrive(NtData->Handle))*/)
		{
			Nt_ದತ್ತಾಂಶ->OpenedReadonly = TRUE;
		}

		//
		// Done
		//

		*channel = io;


	}
	__finally{

		ಆದರೆ(0 != Errno)
		{
			//
			// Cleanup
			//

			ಆದರೆ (ಖಾಲಿ != io)
			{
				ಬಿಡುಗಡೆ(io->name);
				ಬಿಡುಗಡೆ(io);
			}

			ಆದರೆ (ಖಾಲಿ != Nt_ದತ್ತಾಂಶ)
			{
				ಆದರೆ(ಖಾಲಿ != Nt_ದತ್ತಾಂಶ->Handle)
				{
					_UnlockDrive(Nt_ದತ್ತಾಂಶ->Handle);
					_CloseDisk(Nt_ದತ್ತಾಂಶ->Handle);
				}

				ಬಿಡುಗಡೆ(Nt_ದತ್ತಾಂಶ->ಬಫರ್_2);
				ಬಿಡುಗಡೆ(Nt_ದತ್ತಾಂಶ);
			}
		}
	}

	ಹಿಂತಿರುಗಿ Errno;
}


//
// Close api
//

ಸ್ಥಿರ
errcode_t
nt_close(io_channel channel)
{
	PNT_ಖಾಸಗಿ_ದತ್ತಾಂಶ Nt_ದತ್ತಾಂಶ = ಖಾಲಿ;

	ಆದರೆ(ಖಾಲಿ == channel)
	{
		ಹಿಂತಿರುಗಿ 0;
	}

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_ದತ್ತಾಂಶ = (PNT_ಖಾಸಗಿ_ದತ್ತಾಂಶ) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(Nt_ದತ್ತಾಂಶ, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	ಆದರೆ (--channel->refcount > 0)
	{
		ಹಿಂತಿರುಗಿ 0;
	}

	ಬಿಡುಗಡೆ(channel->name);
	ಬಿಡುಗಡೆ(channel);

	ಆದರೆ (ಖಾಲಿ != Nt_ದತ್ತಾಂಶ)
	{
		ಆದರೆ(ಖಾಲಿ != Nt_ದತ್ತಾಂಶ->Handle)
		{
			_DismountDrive(Nt_ದತ್ತಾಂಶ->Handle);
			_UnlockDrive(Nt_ದತ್ತಾಂಶ->Handle);
			_CloseDisk(Nt_ದತ್ತಾಂಶ->Handle);
		}

		ಬಿಡುಗಡೆ(Nt_ದತ್ತಾಂಶ->ಬಫರ್_2);
		ಬಿಡುಗಡೆ(Nt_ದತ್ತಾಂಶ);
	}

	ಹಿಂತಿರುಗಿ 0;
}



//
// set block size
//

ಸ್ಥಿರ
errcode_t
nt_ಗಣ_blksize(io_channel channel, ಪೂರ್ಣಾಂಕ blksize)
{
	PNT_ಖಾಸಗಿ_ದತ್ತಾಂಶ Nt_ದತ್ತಾಂಶ = ಖಾಲಿ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_ದತ್ತಾಂಶ = (PNT_ಖಾಸಗಿ_ದತ್ತಾಂಶ) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(Nt_ದತ್ತಾಂಶ, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	ಆದರೆ (channel->ಖಂಡ_ಗಾತ್ರ != blksize)
	{
		channel->ಖಂಡ_ಗಾತ್ರ = blksize;

		ಬಿಡುಗಡೆ(Nt_ದತ್ತಾಂಶ->ಬಫರ್_2);
		Nt_ದತ್ತಾಂಶ->ಬಫರ್_ಖಂಡ_ಸಂಖ್ಯೆ = 0xffffffff;
		Nt_ದತ್ತಾಂಶ->ಬಫರ್_ಗಾತ್ರ = channel->ಖಂಡ_ಗಾತ್ರ;
		ASSERT(0 == (Nt_ದತ್ತಾಂಶ->ಬಫರ್_ಗಾತ್ರ % 512));

		Nt_ದತ್ತಾಂಶ->ಬಫರ್_2 = ಸ್ಮರಣೆ_ಕೊಡು(Nt_ದತ್ತಾಂಶ->ಬಫರ್_ಗಾತ್ರ);

		ಆದರೆ (ಖಾಲಿ == Nt_ದತ್ತಾಂಶ->ಬಫರ್_2)
		{
			ಹಿಂತಿರುಗಿ ENOMEM;
		}

	}

	ಹಿಂತಿರುಗಿ 0;
}


//
// read block
//

ಸ್ಥಿರ
errcode_t
nt_read_blk(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
			       ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಶೂನ್ಯ *buf)
{
	PVOID ಬಫರ್_ಗಮ್ಯ_Read;
	ULONG ಗಾತ್ರ_ಗಮ್ಯ_Read;
	ULONG ಗಾತ್ರ_3;
	LARGE_INTEGER ಸ್ಥಾನಾಂತರ_2;
	PNT_ಖಾಸಗಿ_ದತ್ತಾಂಶ Nt_ದತ್ತಾಂಶ = ಖಾಲಿ;
	ಚಿಹ್ನೆರಹಿತ Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_ದತ್ತಾಂಶ = (PNT_ಖಾಸಗಿ_ದತ್ತಾಂಶ) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(Nt_ದತ್ತಾಂಶ, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	//
	// If it's in the cache, use it!
	//

	ಆದರೆ ((1 == ಎಣಿಕೆ_2) &&
		(ಖಂಡ == Nt_ದತ್ತಾಂಶ->ಬಫರ್_ಖಂಡ_ಸಂಖ್ಯೆ) &&
		(Nt_ದತ್ತಾಂಶ->ಬಫರ್_ಖಂಡ_ಸಂಖ್ಯೆ != 0xffffffff))
	{
		ಸ್ಮರಣೆ_ನಕಲು(buf, Nt_ದತ್ತಾಂಶ->ಬಫರ್_2, channel->ಖಂಡ_ಗಾತ್ರ);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಗಾತ್ರ_3 = (ಎಣಿಕೆ_2 < 0) ? (ULONG)(-ಎಣಿಕೆ_2) : (ULONG)(ಎಣಿಕೆ_2 * channel->ಖಂಡ_ಗಾತ್ರ);

	ಸ್ಥಾನಾಂತರ_2.QuadPart = ಖಂಡ * channel->ಖಂಡ_ಗಾತ್ರ;

	//
	// If not fit to the block
	//

	ಆದರೆ(ಗಾತ್ರ_3 <= Nt_ದತ್ತಾಂಶ->ಬಫರ್_ಗಾತ್ರ)
	{
		//
		// Update the cache
		//

		Nt_ದತ್ತಾಂಶ->ಬಫರ್_ಖಂಡ_ಸಂಖ್ಯೆ = ಖಂಡ;
		ಬಫರ್_ಗಮ್ಯ_Read = Nt_ದತ್ತಾಂಶ->ಬಫರ್_2;
		ಗಾತ್ರ_ಗಮ್ಯ_Read = Nt_ದತ್ತಾಂಶ->ಬಫರ್_ಗಾತ್ರ;
	}
	ಇಲ್ಲದಿದ್ದರೆ
	{
		ಗಾತ್ರ_ಗಮ್ಯ_Read = ಗಾತ್ರ_3;
		ಬಫರ್_ಗಮ್ಯ_Read = buf;
		ASSERT(0 == (ಗಾತ್ರ_ಗಮ್ಯ_Read % channel->ಖಂಡ_ಗಾತ್ರ));
	}

	ಆದರೆ(!_RawRead(Nt_ದತ್ತಾಂಶ->Handle, ಸ್ಥಾನಾಂತರ_2, ಗಾತ್ರ_ಗಮ್ಯ_Read, ಬಫರ್_ಗಮ್ಯ_Read, &Errno))
	{

		ಆದರೆ (channel->read_error)
		{
			ಹಿಂತಿರುಗಿ (channel->read_error)(channel, ಖಂಡ, ಎಣಿಕೆ_2, buf,
					       ಗಾತ್ರ_3, 0, Errno);
		}
		ಇಲ್ಲದಿದ್ದರೆ
		{
			ಹಿಂತಿರುಗಿ Errno;
		}
	}


	ಆದರೆ(ಬಫರ್_ಗಮ್ಯ_Read != buf)
	{
		ASSERT(ಗಾತ್ರ_3 <= ಗಾತ್ರ_ಗಮ್ಯ_Read);
		ಸ್ಮರಣೆ_ನಕಲು(buf, ಬಫರ್_ಗಮ್ಯ_Read, ಗಾತ್ರ_3);
	}

	ಹಿಂತಿರುಗಿ 0;
}


//
// write block
//

ಸ್ಥಿರ
errcode_t
nt_write_blk(io_channel channel, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಂಡ,
				ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *buf)
{
	ULONG ಗಾತ್ರ_ಗಮ್ಯ_Write;
	LARGE_INTEGER ಸ್ಥಾನಾಂತರ_2;
	PNT_ಖಾಸಗಿ_ದತ್ತಾಂಶ Nt_ದತ್ತಾಂಶ = ಖಾಲಿ;
	ಚಿಹ್ನೆರಹಿತ Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_ದತ್ತಾಂಶ = (PNT_ಖಾಸಗಿ_ದತ್ತಾಂಶ) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(Nt_ದತ್ತಾಂಶ, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	ಆದರೆ(Nt_ದತ್ತಾಂಶ->OpenedReadonly)
	{
		ಹಿಂತಿರುಗಿ EACCES;
	}

	ಆದರೆ (ಎಣಿಕೆ_2 == 1)
	{
		ಗಾತ್ರ_ಗಮ್ಯ_Write = channel->ಖಂಡ_ಗಾತ್ರ;
	}
	ಇಲ್ಲದಿದ್ದರೆ
	{
		Nt_ದತ್ತಾಂಶ->ಬಫರ್_ಖಂಡ_ಸಂಖ್ಯೆ = 0xffffffff;

		ಆದರೆ (ಎಣಿಕೆ_2 < 0)
		{
			ಗಾತ್ರ_ಗಮ್ಯ_Write = (ULONG)(-ಎಣಿಕೆ_2);
		}
		ಇಲ್ಲದಿದ್ದರೆ
		{
			ಗಾತ್ರ_ಗಮ್ಯ_Write = (ULONG)(ಎಣಿಕೆ_2 * channel->ಖಂಡ_ಗಾತ್ರ);
		}
	}


	ASSERT(0 == (ಗಾತ್ರ_ಗಮ್ಯ_Write % 512));
	ಸ್ಥಾನಾಂತರ_2.QuadPart = ಖಂಡ * channel->ಖಂಡ_ಗಾತ್ರ;

	ಆದರೆ(!_RawWrite(Nt_ದತ್ತಾಂಶ->Handle, ಸ್ಥಾನಾಂತರ_2, ಗಾತ್ರ_ಗಮ್ಯ_Write, buf, &Errno))
	{
		ಆದರೆ (channel->write_error)
		{
			ಹಿಂತಿರುಗಿ (channel->write_error)(channel, ಖಂಡ, ಎಣಿಕೆ_2, buf,
						ಗಾತ್ರ_ಗಮ್ಯ_Write, 0, Errno);
		}
		ಇಲ್ಲದಿದ್ದರೆ
		{
			ಹಿಂತಿರುಗಿ Errno;
		}
	}


	//
	// Stash a copy.
	//

	ಆದರೆ(ಗಾತ್ರ_ಗಮ್ಯ_Write >= Nt_ದತ್ತಾಂಶ->ಬಫರ್_ಗಾತ್ರ)
	{
		Nt_ದತ್ತಾಂಶ->ಬಫರ್_ಖಂಡ_ಸಂಖ್ಯೆ = ಖಂಡ;
		ಸ್ಮರಣೆ_ನಕಲು(Nt_ದತ್ತಾಂಶ->ಬಫರ್_2, buf, Nt_ದತ್ತಾಂಶ->ಬಫರ್_ಗಾತ್ರ);
	}

	Nt_ದತ್ತಾಂಶ->Written = TRUE;

	ಹಿಂತಿರುಗಿ 0;

}



//
// Flush data buffers to disk.  Since we are currently using a
// write-through cache, this is a no-op.
//

ಸ್ಥಿರ
errcode_t
nt_flush(io_channel channel)
{
	PNT_ಖಾಸಗಿ_ದತ್ತಾಂಶ Nt_ದತ್ತಾಂಶ = ಖಾಲಿ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_ದತ್ತಾಂಶ = (PNT_ಖಾಸಗಿ_ದತ್ತಾಂಶ) channel->ಖಾಸಗಿ_ದತ್ತಾಂಶ;
	EXT2_CHECK_MAGIC(Nt_ದತ್ತಾಂಶ, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	ಆದರೆ(Nt_ದತ್ತಾಂಶ->OpenedReadonly)
	{
		ಹಿಂತಿರುಗಿ 0; // EACCESS;
	}


	//
	// Flush file buffers.
	//

	_FlushDrive(Nt_ದತ್ತಾಂಶ->Handle);


	//
	// Test and correct partition type.
	//

	ಆದರೆ(Nt_ದತ್ತಾಂಶ->Written)
	{
		ಗಣ_Part_Type(Nt_ದತ್ತಾಂಶ->Handle, 0x83);
	}

	ಹಿಂತಿರುಗಿ 0;
}


