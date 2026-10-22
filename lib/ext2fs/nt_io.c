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

#جے_تعریف_شدہ HAVE_CONFIG_H
#ضمن "config.h"
#جے_انت


//
// I need some warnings to disable...
//


#ہدایت warning(disable:4514) // unreferenced inline function has been removed
#ہدایت warning(ڈالو,4)

#ہدایت warning(disable:4201) // nonstandard extension used : nameless struct/union)
#ہدایت warning(disable:4214) // nonstandard extension used : bit field types other than int
#ہدایت warning(disable:4115) // named type definition in parentheses

#ضمن <ntddk.h>
#ضمن <ntdddisk.h>
#ضمن <ntstatus.h>

#ہدایت warning(نکالو)


//
// Some native APIs.
//

NTSYSAPI
ULONG
NTAPI
Rtl_Nt_حالت_منزل_Dos_Error(
    وچ_2 NTSTATUS حالت
   );

NTSYSAPI
NTSTATUS
NTAPI
NtClose(
    وچ_2 HANDLE Handle
   );


NTSYSAPI
NTSTATUS
NTAPI
NtOpenFile(
    باہرلا_2 PHANDLE FileHandle,
    وچ_2 ACCESS_نقاب DesiredAccess,
    وچ_2 POBJECT_گن Object_گن,
    باہرلا_2 PIO_حالت_بلاک Io_حالت_بلاک,
    وچ_2 ULONG ShareAccess,
    وچ_2 ULONG OpenOptions
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFlushBuffersFile(
    وچ_2 HANDLE FileHandle,
    باہرلا_2 PIO_حالت_بلاک Io_حالت_بلاک
   );


NTSYSAPI
NTSTATUS
NTAPI
NtReadFile(
    وچ_2 HANDLE FileHandle,
    وچ_2 HANDLE Event OPTIONAL,
    وچ_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    وچ_2 PVOID ApcContext OPTIONAL,
    باہرلا_2 PIO_حالت_بلاک Io_حالت_بلاک,
    باہرلا_2 PVOID بفر_2,
    وچ_2 ULONG لمائی_2,
    وچ_2 PLARGE_INTEGER Byte_وتھ OPTIONAL,
    وچ_2 PULONG کلید_2 OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtWriteFile(
    وچ_2 HANDLE FileHandle,
    وچ_2 HANDLE Event OPTIONAL,
    وچ_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    وچ_2 PVOID ApcContext OPTIONAL,
    باہرلا_2 PIO_حالت_بلاک Io_حالت_بلاک,
    وچ_2 PVOID بفر_2,
    وچ_2 ULONG لمائی_2,
    وچ_2 PLARGE_INTEGER Byte_وتھ OPTIONAL,
    وچ_2 PULONG کلید_2 OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtDeviceIoControlFile(
    وچ_2 HANDLE FileHandle,
    وچ_2 HANDLE Event OPTIONAL,
    وچ_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    وچ_2 PVOID ApcContext OPTIONAL,
    باہرلا_2 PIO_حالت_بلاک Io_حالت_بلاک,
    وچ_2 ULONG Io_Control_کوڈ,
    وچ_2 PVOID داخلہ_بفر OPTIONAL,
    وچ_2 ULONG داخلہ_بفر_لمائی,
    باہرلا_2 PVOID Output_بفر OPTIONAL,
    وچ_2 ULONG Output_بفر_لمائی
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFsControlFile(
    وچ_2 HANDLE FileHandle,
    وچ_2 HANDLE Event OPTIONAL,
    وچ_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    وچ_2 PVOID ApcContext OPTIONAL,
    باہرلا_2 PIO_حالت_بلاک Io_حالت_بلاک,
    وچ_2 ULONG Io_Control_کوڈ,
    وچ_2 PVOID داخلہ_بفر OPTIONAL,
    وچ_2 ULONG داخلہ_بفر_لمائی,
    باہرلا_2 PVOID Output_بفر OPTIONAL,
    وچ_2 ULONG Output_بفر_لمائی
    );


NTSYSAPI
NTSTATUS
NTAPI
NtDelayExecution(
    وچ_2 BOOLEAN Alertable,
    وچ_2 PLARGE_INTEGER Interval
    );


#تعریف_کرو FSCTL_جندرا_VOLUME               CTL_کوڈ(FILE_DEVICE_FILE_SYSTEM, 6, طریقہ_BUFFERED, FILE_ANY_ACCESS)
#تعریف_کرو FSCTL_UNLOCK_VOLUME             CTL_کوڈ(FILE_DEVICE_FILE_SYSTEM, 7, طریقہ_BUFFERED, FILE_ANY_ACCESS)
#تعریف_کرو FSCTL_DISMOUNT_VOLUME           CTL_کوڈ(FILE_DEVICE_FILE_SYSTEM, 8, طریقہ_BUFFERED, FILE_ANY_ACCESS)
#تعریف_کرو FSCTL_IS_VOLUME_MOUNTED         CTL_کوڈ(FILE_DEVICE_FILE_SYSTEM,10, طریقہ_BUFFERED, FILE_ANY_ACCESS)


//
// useful macros
//

#تعریف_کرو BooleanFlagOn(Flags,SingleFlag) ((BOOLEAN)((((Flags) & (SingleFlag)) != 0)))


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


#تعریف_کرو EXT2_CHECK_MAGIC(بنية, کوڈ) \
	  إذا ((بنية)->magic != (کوڈ)) أرجع (کوڈ)

#تعریف_کرو EXT2_ET_MAGIC_NT_IO_CHANNEL  0x10ed


//
// Private data block
//

تعريفنوع بنية NT_نجی_انکڑے {
	صحيح	   magic;
	HANDLE Handle;
	صحيح	   Flags;
	PCHAR  بفر_2;
	__u32  بفر_بلاک_نمبر;
	ULONG  بفر_ناپ;
	BOOLEAN OpenedReadonly;
	BOOLEAN Written;
}NT_نجی_انکڑے_2, *PNT_نجی_انکڑے;



//
// Standard interface prototypes
//

ساكن errcode_t nt_open(ثابت حرف *name, صحيح flags, io_channel *channel);
ساكن errcode_t nt_close(io_channel channel);
ساكن errcode_t nt_سیٹ_blksize(io_channel channel, صحيح blksize);
ساكن errcode_t nt_read_blk(io_channel channel, بنانشانی لما بلاک,
			       صحيح گنتی, فراغ *انکڑے);
ساكن errcode_t nt_write_blk(io_channel channel, بنانشانی لما بلاک,
				صحيح گنتی, ثابت فراغ *انکڑے);
ساكن errcode_t nt_flush(io_channel channel);

ساكن بنية struct_io_manager struct_nt_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "NT I/O Manager",
	.open		= nt_open,
	.close		= nt_close,
	.سیٹ_blksize	= nt_سیٹ_blksize,
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
}ERROR_اندراج;

ساكن ERROR_اندراج Error_جدول[] = {
        {  ERROR_INVALID_FUNCTION,       EINVAL    },
        {  ERROR_FILE_NOT_لبھیا,         ENOENT    },
        {  ERROR_رستہ_NOT_لبھیا,         ENOENT    },
        {  ERROR_TOO_MANY_OPEN_FILES,    EMFILE    },
        {  ERROR_ACCESS_DENIED,          EACCES    },
        {  ERROR_INVALID_HANDLE,         EBADF     },
        {  ERROR_ARENA_TRASHED,          ENOMEM    },
        {  ERROR_NOT_ENOUGH_حافظہ,      ENOMEM    },
        {  ERROR_INVALID_بلاک,          ENOMEM    },
        {  ERROR_BAD_ENVIRONMENT,        E2BIG     },
        {  ERROR_BAD_FORMAT,             ENOEXEC   },
        {  ERROR_INVALID_ACCESS,         EINVAL    },
        {  ERROR_INVALID_انکڑے,           EINVAL    },
        {  ERROR_INVALID_DRIVE,          ENOENT    },
        {  ERROR_CURRENT_DIRECTORY,      EACCES    },
        {  ERROR_NOT_SAME_DEVICE,        EXDEV     },
        {  ERROR_NO_ہور_FILES,          ENOENT    },
        {  ERROR_جندرا_VIOLATION,         EACCES    },
        {  ERROR_BAD_NETPATH,            ENOENT    },
        {  ERROR_جال_ACCESS_DENIED,  EACCES    },
        {  ERROR_BAD_NET_NAME,           ENOENT    },
        {  ERROR_FILE_EXISTS,            EEXIST    },
        {  ERROR_CANNOT_MAKE,            EACCES    },
        {  ERROR_FAIL_I24,               EACCES    },
        {  ERROR_INVALID_PARAMETER,      EINVAL    },
        {  ERROR_NO_PROC_SLOTS,          EAGAIN    },
        {  ERROR_DRIVE_LOCKED,           EACCES    },
        {  ERROR_BROKEN_PIPE,            EPIPE     },
        {  ERROR_DISK_بھریا,              ENOSPC    },
        {  ERROR_INVALID_نشانہ_HANDLE,  EBADF     },
        {  ERROR_INVALID_HANDLE,         EINVAL    },
        {  ERROR_WAIT_NO_CHILDREN,       ECHILD    },
        {  ERROR_بچہ_NOT_COMPLETE,     ECHILD    },
        {  ERROR_سدھا_ACCESS_HANDLE,   EBADF     },
        {  ERROR_NEGATIVE_SEEK,          EINVAL    },
        {  ERROR_SEEK_ON_DEVICE,         EACCES    },
        {  ERROR_DIR_NOT_خالی_تھاواں,          ENOTEMPTY },
        {  ERROR_NOT_LOCKED,             EACCES    },
        {  ERROR_BAD_PATHNAME,           ENOENT    },
        {  ERROR_MAX_THRDS_REACHED,      EAGAIN    },
        {  ERROR_جندرا_FAILED,            EACCES    },
        {  ERROR_ALREADY_EXISTS,         EEXIST    },
        {  ERROR_FILENAME_EXCED_RANGE,   ENOENT    },
        {  ERROR_NESTING_NOT_ALLOWED,    EAGAIN    },
        {  ERROR_NOT_ENOUGH_QUOTA,       ENOMEM    }
};




ساكن
بنانشانی
_MapDosError (
    وچ_2 ULONG WinError
   )
{
	صحيح i;

	//
	// Lookup
	//

	لكل (i = 0; i < (حجم(Error_جدول)/حجم(Error_جدول[0])); ++i)
	{
		إذا (WinError == Error_جدول[i].WinError)
		{
			أرجع Error_جدول[i].errnocode;
		}
	}

	//
	// not in table. Check ranges
	//

	إذا ((WinError >= ERROR_WRITE_PROTECT) &&
		(WinError <= ERROR_SHARING_بفر_EXCEEDED))
	{
		أرجع EACCES;
	}
	وإلا إذا ((WinError >= ERROR_INVALID_STARTING_CODESEG) &&
			 (WinError <= ERROR_INFLOOP_وچ_RELOC_CHAIN))
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
بنانشانی
Map_Nt_حالت(
    وچ_2 NTSTATUS حالت
   )
{
	أرجع _MapDosError(Rtl_Nt_حالت_منزل_Dos_Error(حالت));
}





//
// Helper functions to make things easier
//

ساكن
NTSTATUS
_OpenNtName(
    وچ_2 PCSTR Name,
    وچ_2 BOOLEAN Readonly,
    باہرلا_2 PHANDLE Handle,
    باہرلا_2 PBOOLEAN OpenedReadonly OPTIONAL
   )
{
	UNICODE_STRING UnicodeString;
	ANSI_STRING    AnsiString;
	WCHAR بفر_2[512];
	NTSTATUS حالت;
	OBJECT_گن Object_گن;
	IO_حالت_بلاک Io_حالت_بلاک;

	//
	// Make Unicode name from input string
	//

	UnicodeString.بفر_2 = &بفر_2[0];
	UnicodeString.لمائی_2 = 0;
	UnicodeString.ودھ_توں_ودھ_لمائی = حجم(بفر_2); // in bytes!!!

	RtlInitAnsiString(&AnsiString, Name);

	حالت = Rtl_Ansi_String_منزل_Unicode_String(&UnicodeString, &AnsiString, FALSE);

	إذا(!NT_SUCCESS(حالت))
	{
		أرجع حالت; // Unmappable character?
	}

	//
	// Initialize object
	//

	Initialize_Object_گن(&Object_گن,
							   &UnicodeString,
							   OBJ_CASE_INSENSITIVE,
							   عدم,
							   عدم );

	//
	// Try to open it in initial mode
	//

	إذا(دلیل_PRESENT(OpenedReadonly))
	{
		*OpenedReadonly = Readonly;
	}


	حالت = NtOpenFile(Handle,
						SYNCHRONIZE | FILE_READ_انکڑے | (Readonly ? 0 : FILE_WRITE_انکڑے),
						&Object_گن,
						&Io_حالت_بلاک,
						FILE_SHARE_WRITE | FILE_SHARE_READ,
						FILE_SYNCHRONOUS_IO_NONALERT);

	إذا(!NT_SUCCESS(حالت))
	{
		//
		// Maybe was just mounted? wait 0.5 sec and retry.
		//

		LARGE_INTEGER Interval;
		Interval.QuadPart = -5000000; // 0.5 sec. from now

		NtDelayExecution(FALSE, &Interval);

		حالت = NtOpenFile(Handle,
							SYNCHRONIZE | FILE_READ_انکڑے | (Readonly ? 0 : FILE_WRITE_انکڑے),
							&Object_گن,
							&Io_حالت_بلاک,
							FILE_SHARE_WRITE | FILE_SHARE_READ,
							FILE_SYNCHRONOUS_IO_NONALERT);

		//
		// Try to satisfy mode
		//

		إذا((حالت_ACCESS_DENIED == حالت) && !Readonly)
		{
			إذا(دلیل_PRESENT(OpenedReadonly))
			{
				*OpenedReadonly = TRUE;
			}

			حالت = NtOpenFile(Handle,
							SYNCHRONIZE | FILE_READ_انکڑے,
							&Object_گن,
							&Io_حالت_بلاک,
							FILE_SHARE_WRITE | FILE_SHARE_READ,
							FILE_SYNCHRONOUS_IO_NONALERT);
		}
	}



	//
	//پورا
	//

	أرجع حالت;
}


ساكن
NTSTATUS
_OpenDriveLetter(
    وچ_2 CHAR Letter,
    وچ_2 BOOLEAN ReadOnly,
    باہرلا_2 PHANDLE Handle,
    باہرلا_2 PBOOLEAN OpenedReadonly OPTIONAL
   )
{
	CHAR بفر_2[100];

	sprintf(بفر_2, "\\DosDevices\\%c:", Letter);

	أرجع _OpenNtName(بفر_2, ReadOnly, Handle, OpenedReadonly);
}


//
// Flush device
//

ساكن
__inline
NTSTATUS
_FlushDrive(
		وچ_2 HANDLE Handle
		)
{
	IO_حالت_بلاک Io_حالت_بلاک;
	أرجع NtFlushBuffersFile(Handle, &Io_حالت_بلاک);
}


//
// lock drive
//

ساكن
__inline
NTSTATUS
جندرا_Drive(
		وچ_2 HANDLE Handle
		)
{
	IO_حالت_بلاک Io_حالت_بلاک;
	أرجع NtFsControlFile(Handle, 0, 0, 0, &Io_حالت_بلاک, FSCTL_جندرا_VOLUME, 0, 0, 0, 0);
}


//
// unlock drive
//

ساكن
__inline
NTSTATUS
_UnlockDrive(
	وچ_2 HANDLE Handle
	)
{
	IO_حالت_بلاک Io_حالت_بلاک;
	أرجع NtFsControlFile(Handle, 0, 0, 0, &Io_حالت_بلاک, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0);
}

ساكن
__inline
NTSTATUS
_DismountDrive(
	وچ_2 HANDLE Handle
	)
{
	IO_حالت_بلاک Io_حالت_بلاک;
	أرجع NtFsControlFile(Handle, 0, 0, 0, &Io_حالت_بلاک, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0);
}


//
// is mounted
//

ساكن
__inline
BOOLEAN
_IsMounted(
	وچ_2 HANDLE Handle
	)
{
	IO_حالت_بلاک Io_حالت_بلاک;
	NTSTATUS حالت;
	حالت = NtFsControlFile(Handle, 0, 0, 0, &Io_حالت_بلاک, FSCTL_IS_VOLUME_MOUNTED, 0, 0, 0, 0);
	أرجع (BOOLEAN)(حالت_SUCCESS == حالت);
}


ساكن
__inline
NTSTATUS
_CloseDisk(
		وچ_2 HANDLE Handle
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
    وچ_2 PCSTR Device,
    وچ_2 PSTR Normalized_Device_Name_بفر
   )
{
	صحيح حصے_کرو_نمبر = -1;
	UCHAR Disk_نمبر;
	PSTR p;


	//
	// Do not try to parse NT name
	//

	إذا('\\' == *Device)
		أرجع Device;



	//
	// Strip leading '/dev/' if any
	//

	إذا(('/' == *(Device)) &&
		('d' == *(Device + 1)) &&
		('e' == *(Device + 2)) &&
		('v' == *(Device + 3)) &&
		('/' == *(Device + 4)))
	{
		Device += 5;
	}

	إذا('\0' == *Device)
	{
		أرجع عدم;
	}


	//
	// forms: hda[n], fd[n]
	//

	إذا('d' != *(Device + 1))
	{
		أرجع عدم;
	}

	إذا('h' == *Device)
	{
		إذا((*(Device + 2) < 'a') || (*(Device + 2) > ('a' + 9)) ||
		   ((*(Device + 3) != '\0') &&
			((*(Device + 4) != '\0') ||
			 ((*(Device + 3) < '0') || (*(Device + 3) > '9'))
			)
		   )
		  )
		{
			أرجع عدم;
		}

		Disk_نمبر = (UCHAR)(*(Device + 2) - 'a');

		إذا(*(Device + 3) != '\0')
		{
			حصے_کرو_نمبر = (*(Device + 3) - '0');
		}

	}
	وإلا إذا('f' == *Device)
	{
		//
		// 3-d letter should be a digit.
		//

		إذا((*(Device + 3) != '\0') ||
		   (*(Device + 2) < '0') || (*(Device + 2) > '9'))
		{
			أرجع عدم;
		}

		Disk_نمبر = (UCHAR)(*(Device + 2) - '0');

	}
	وإلا
	{
		//
		// invalid prefix
		//

		أرجع عدم;
	}



	//
	//اگیتر
	//

	strcpy(Normalized_Device_Name_بفر, "\\Device\\");

	//
	// Media name
	//

	بدل(*Device)
	{

	حالة 'f':
		strcat(Normalized_Device_Name_بفر, "Floppy0");
		اكسر;

	حالة 'h':
		strcat(Normalized_Device_Name_بفر, "Harddisk0");
		اكسر;
	}


	p = Normalized_Device_Name_بفر + لڑی_لمبائی(Normalized_Device_Name_بفر) - 1;
	*p = (CHAR)(*p + Disk_نمبر);


	//
	// Partition nr.
	//

	إذا(حصے_کرو_نمبر >= 0)
	{
		strcat(Normalized_Device_Name_بفر, "\\Partition0");

		p = Normalized_Device_Name_بفر + لڑی_لمبائی(Normalized_Device_Name_بفر) - 1;
		*p = (CHAR)(*p + حصے_کرو_نمبر);
	}


	أرجع Normalized_Device_Name_بفر;
}




ساكن
VOID
Get_Device_ناپ(
    وچ_2 HANDLE h,
    باہرلا_2 بنانشانی __int64 *Fs_ناپ
   )
{
	حصے_کرو_INFORMATION pi;
	DISK_GEOMETRY gi;
	NTSTATUS حالت;
	IO_حالت_بلاک Io_حالت_بلاک;

	//
	// Zero it
	//

	*Fs_ناپ = 0;

	//
	// Call driver
	//

	Rtl_Zero_حافظہ(&pi, حجم(حصے_کرو_INFORMATION));

	حالت = NtDeviceIoControlFile(
		h, عدم, عدم, عدم, &Io_حالت_بلاک, IOCTL_DISK_GET_حصے_کرو_INFO,
		&pi, حجم(حصے_کرو_INFORMATION),
		&pi, حجم(حصے_کرو_INFORMATION));


	إذا(NT_SUCCESS(حالت))
	{
		*Fs_ناپ = pi.حصے_کرو_لمائی.QuadPart;
	}
	وإلا إذا(حالت_INVALID_DEVICE_REQUEST == حالت)
	{
		//
		// No partitions: get device info.
		//

		Rtl_Zero_حافظہ(&gi, حجم(DISK_GEOMETRY));

		حالت = NtDeviceIoControlFile(
				h, عدم, عدم, عدم, &Io_حالت_بلاک, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, حجم(DISK_GEOMETRY),
				&gi, حجم(DISK_GEOMETRY));


		إذا(NT_SUCCESS(حالت))
		{
			*Fs_ناپ =
				gi.بائٹاں_فی_Sector *
				gi.Sectors_فی_Track *
				gi.Tracks_فی_Cylinder *
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
    وچ_2 PCSTR Name,
    وچ_2 BOOLEAN ReadOnly,
    باہرلا_2 PHANDLE Handle,
    باہرلا_2 PBOOLEAN OpenedReadonly OPTIONAL,
    باہرلا_2 بنانشانی *Errno OPTIONAL
   )
{
	CHAR NormalizedDeviceName[512];
	NTSTATUS حالت;

	إذا(عدم == Name)
	{
		//
		// Set not found
		//

		إذا(دلیل_PRESENT(Errno))
			*Errno = ENOENT;

		أرجع FALSE;
	}


	إذا((((*Name) | 0x20) >= 'a') && (((*Name) | 0x20) <= 'z') &&
		(':' == *(Name + 1)) && ('\0' == *(Name + 2)))
	{
		حالت = _OpenDriveLetter(*Name, ReadOnly, Handle, OpenedReadonly);
	}
	وإلا
	{
		//
		// Make name
		//

		Name = _NormalizeDeviceName(Name, NormalizedDeviceName);

		إذا(عدم == Name)
		{
			//
			// Set not found
			//

			إذا(دلیل_PRESENT(Errno))
				*Errno = ENOENT;

			أرجع FALSE;
		}

		//
		// Try to open it
		//

		حالت = _OpenNtName(Name, ReadOnly, Handle, OpenedReadonly);
	}


	إذا(!NT_SUCCESS(حالت))
	{
		إذا(دلیل_PRESENT(Errno))
			*Errno = Map_Nt_حالت(حالت);

		أرجع FALSE;
	}

	أرجع TRUE;
}


//
// Raw block io. Sets dos errno
//

ساكن
BOOLEAN
بلاک_Io(
    وچ_2 HANDLE Handle,
    وچ_2 LARGE_INTEGER وتھ_2,
    وچ_2 ULONG بائٹاں_2,
    وچ_2 باہرلا_2 PCHAR بفر_2,
    وچ_2 BOOLEAN Read,
    باہرلا_2 بنانشانی* Errno
   )
{
	IO_حالت_بلاک Io_حالت_بلاک;
	NTSTATUS حالت;

	//
	// Should be aligned
	//

	ASSERT(0 == (بائٹاں_2 % 512));
	ASSERT(0 == (وتھ_2.کم_Part % 512));


	//
	// perform io
	//

	إذا(Read)
	{
		حالت = NtReadFile(Handle, عدم, عدم, عدم,
			&Io_حالت_بلاک, بفر_2, بائٹاں_2, &وتھ_2, عدم);
	}
	وإلا
	{
		حالت = NtWriteFile(Handle, عدم, عدم, عدم,
			&Io_حالت_بلاک, بفر_2, بائٹاں_2, &وتھ_2, عدم);
	}


	//
	// translate error
	//

	إذا(NT_SUCCESS(حالت))
	{
		*Errno = 0;
		أرجع TRUE;
	}

	*Errno = Map_Nt_حالت(حالت);

	أرجع FALSE;
}



__inline
BOOLEAN
_RawWrite(
    وچ_2 HANDLE Handle,
    وچ_2 LARGE_INTEGER وتھ_2,
    وچ_2 ULONG بائٹاں_2,
    باہرلا_2 ثابت CHAR* بفر_2,
    باہرلا_2 بنانشانی* Errno
   )
{
	أرجع بلاک_Io(Handle, وتھ_2, بائٹاں_2, (PCHAR)بفر_2, FALSE, Errno);
}

__inline
BOOLEAN
_RawRead(
    وچ_2 HANDLE Handle,
    وچ_2 LARGE_INTEGER وتھ_2,
    وچ_2 ULONG بائٹاں_2,
    وچ_2 PCHAR بفر_2,
    باہرلا_2 بنانشانی* Errno
   )
{
	أرجع بلاک_Io(Handle, وتھ_2, بائٹاں_2, بفر_2, TRUE, Errno);
}



__inline
BOOLEAN
سیٹ_Part_Type(
    وچ_2 HANDLE Handle,
    وچ_2 UCHAR Type
   )
{
	IO_حالت_بلاک Io_حالت_بلاک;
	أرجع حالت_SUCCESS == NtDeviceIoControlFile(
												   Handle, عدم, عدم, عدم, &Io_حالت_بلاک, IOCTL_DISK_سیٹ_حصے_کرو_INFO,
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

	إذا(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, عدم))
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

ساكن __int64 Fs_ناپ = 0;
ساكن حرف knowndevice[1024] = "";


errcode_t
ext2fs_get_device_ناپ(ثابت حرف *file, صحيح blocksize,
				 blk_t *retblocks)
{
	HANDLE h;
	BOOLEAN Readonly;

	إذا((0 == Fs_ناپ) || (0 != strcmp(knowndevice, file)))
	{

		إذا(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, عدم))
		{
			أرجع 0;
		}


		__try{

			//
			// Get size
			//

			Get_Device_ناپ(h, &Fs_ناپ);
			strcpy(knowndevice, file);
		}
		__finally{
			_CloseDisk(h);
		}

	}

	*retblocks = (blk_t)(بنانشانی __int64)(Fs_ناپ / blocksize);
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
	PNT_نجی_انکڑے Nt_انکڑے = عدم;
	errcode_t Errno = 0;

	//
	// Check name
	//

	إذا (عدم == name)
	{
		أرجع EXT2_ET_BAD_DEVICE_NAME;
	}

	__try{

		//
		// Allocate channel handle
		//

		io = (io_channel) حافظہ_دیو(حجم(بنية struct_io_channel));

		إذا (عدم == io)
		{
			Errno = ENOMEM;
			__leave;
		}

		Rtl_Zero_حافظہ(io, حجم(بنية struct_io_channel));
		io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

		Nt_انکڑے = (PNT_نجی_انکڑے)حافظہ_دیو(حجم(NT_نجی_انکڑے_2));

		إذا (عدم == Nt_انکڑے)
		{
			Errno = ENOMEM;
			__leave;
		}


		io->manager = nt_io_manager();
		io->name = حافظہ_دیو(لڑی_لمبائی(name) + 1);
		إذا (عدم == io->name)
		{
			Errno = ENOMEM;
			__leave;
		}

		strcpy(io->name, name);
		io->نجی_انکڑے = Nt_انکڑے;
		io->بلاک_ناپ = 1024;
		io->read_error = 0;
		io->write_error = 0;
		io->refcount = 1;

		//
		// Initialize data
		//

		Rtl_Zero_حافظہ(Nt_انکڑے, حجم(NT_نجی_انکڑے_2));

		Nt_انکڑے->magic = EXT2_ET_MAGIC_NT_IO_CHANNEL;
		Nt_انکڑے->بفر_بلاک_نمبر = 0xffffffff;
		Nt_انکڑے->بفر_ناپ = 1024;
		Nt_انکڑے->بفر_2 = حافظہ_دیو(Nt_انکڑے->بفر_ناپ);

		إذا (عدم == Nt_انکڑے->بفر_2)
		{
			Errno = ENOMEM;
			__leave;
		}

		//
		// Open it
		//

		إذا(!_Ext2OpenDevice(name, (BOOLEAN)!BooleanFlagOn(flags, EXT2_FLAG_RW), &Nt_انکڑے->Handle, &Nt_انکڑے->OpenedReadonly, &Errno))
		{
			__leave;
		}


		//
		// get size
		//

		Get_Device_ناپ(Nt_انکڑے->Handle, &Fs_ناپ);
		strcpy(knowndevice, name);


		//
		// Lock/dismount
		//

		إذا(!NT_SUCCESS(جندرا_Drive(Nt_انکڑے->Handle)) /*|| !NT_SUCCESS(_DismountDrive(NtData->Handle))*/)
		{
			Nt_انکڑے->OpenedReadonly = TRUE;
		}

		//
		// Done
		//

		*channel = io;


	}
	__finally{

		إذا(0 != Errno)
		{
			//
			// Cleanup
			//

			إذا (عدم != io)
			{
				حرر(io->name);
				حرر(io);
			}

			إذا (عدم != Nt_انکڑے)
			{
				إذا(عدم != Nt_انکڑے->Handle)
				{
					_UnlockDrive(Nt_انکڑے->Handle);
					_CloseDisk(Nt_انکڑے->Handle);
				}

				حرر(Nt_انکڑے->بفر_2);
				حرر(Nt_انکڑے);
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
	PNT_نجی_انکڑے Nt_انکڑے = عدم;

	إذا(عدم == channel)
	{
		أرجع 0;
	}

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_انکڑے = (PNT_نجی_انکڑے) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(Nt_انکڑے, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	إذا (--channel->refcount > 0)
	{
		أرجع 0;
	}

	حرر(channel->name);
	حرر(channel);

	إذا (عدم != Nt_انکڑے)
	{
		إذا(عدم != Nt_انکڑے->Handle)
		{
			_DismountDrive(Nt_انکڑے->Handle);
			_UnlockDrive(Nt_انکڑے->Handle);
			_CloseDisk(Nt_انکڑے->Handle);
		}

		حرر(Nt_انکڑے->بفر_2);
		حرر(Nt_انکڑے);
	}

	أرجع 0;
}



//
// set block size
//

ساكن
errcode_t
nt_سیٹ_blksize(io_channel channel, صحيح blksize)
{
	PNT_نجی_انکڑے Nt_انکڑے = عدم;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_انکڑے = (PNT_نجی_انکڑے) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(Nt_انکڑے, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	إذا (channel->بلاک_ناپ != blksize)
	{
		channel->بلاک_ناپ = blksize;

		حرر(Nt_انکڑے->بفر_2);
		Nt_انکڑے->بفر_بلاک_نمبر = 0xffffffff;
		Nt_انکڑے->بفر_ناپ = channel->بلاک_ناپ;
		ASSERT(0 == (Nt_انکڑے->بفر_ناپ % 512));

		Nt_انکڑے->بفر_2 = حافظہ_دیو(Nt_انکڑے->بفر_ناپ);

		إذا (عدم == Nt_انکڑے->بفر_2)
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
nt_read_blk(io_channel channel, بنانشانی لما بلاک,
			       صحيح گنتی, فراغ *buf)
{
	PVOID بفر_منزل_Read;
	ULONG ناپ_منزل_Read;
	ULONG ناپ_2;
	LARGE_INTEGER وتھ_2;
	PNT_نجی_انکڑے Nt_انکڑے = عدم;
	بنانشانی Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_انکڑے = (PNT_نجی_انکڑے) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(Nt_انکڑے, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	//
	// If it's in the cache, use it!
	//

	إذا ((1 == گنتی) &&
		(بلاک == Nt_انکڑے->بفر_بلاک_نمبر) &&
		(Nt_انکڑے->بفر_بلاک_نمبر != 0xffffffff))
	{
		حافظہ_نقل(buf, Nt_انکڑے->بفر_2, channel->بلاک_ناپ);
		أرجع 0;
	}

	ناپ_2 = (گنتی < 0) ? (ULONG)(-گنتی) : (ULONG)(گنتی * channel->بلاک_ناپ);

	وتھ_2.QuadPart = بلاک * channel->بلاک_ناپ;

	//
	// If not fit to the block
	//

	إذا(ناپ_2 <= Nt_انکڑے->بفر_ناپ)
	{
		//
		// Update the cache
		//

		Nt_انکڑے->بفر_بلاک_نمبر = بلاک;
		بفر_منزل_Read = Nt_انکڑے->بفر_2;
		ناپ_منزل_Read = Nt_انکڑے->بفر_ناپ;
	}
	وإلا
	{
		ناپ_منزل_Read = ناپ_2;
		بفر_منزل_Read = buf;
		ASSERT(0 == (ناپ_منزل_Read % channel->بلاک_ناپ));
	}

	إذا(!_RawRead(Nt_انکڑے->Handle, وتھ_2, ناپ_منزل_Read, بفر_منزل_Read, &Errno))
	{

		إذا (channel->read_error)
		{
			أرجع (channel->read_error)(channel, بلاک, گنتی, buf,
					       ناپ_2, 0, Errno);
		}
		وإلا
		{
			أرجع Errno;
		}
	}


	إذا(بفر_منزل_Read != buf)
	{
		ASSERT(ناپ_2 <= ناپ_منزل_Read);
		حافظہ_نقل(buf, بفر_منزل_Read, ناپ_2);
	}

	أرجع 0;
}


//
// write block
//

ساكن
errcode_t
nt_write_blk(io_channel channel, بنانشانی لما بلاک,
				صحيح گنتی, ثابت فراغ *buf)
{
	ULONG ناپ_منزل_Write;
	LARGE_INTEGER وتھ_2;
	PNT_نجی_انکڑے Nt_انکڑے = عدم;
	بنانشانی Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_انکڑے = (PNT_نجی_انکڑے) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(Nt_انکڑے, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	إذا(Nt_انکڑے->OpenedReadonly)
	{
		أرجع EACCES;
	}

	إذا (گنتی == 1)
	{
		ناپ_منزل_Write = channel->بلاک_ناپ;
	}
	وإلا
	{
		Nt_انکڑے->بفر_بلاک_نمبر = 0xffffffff;

		إذا (گنتی < 0)
		{
			ناپ_منزل_Write = (ULONG)(-گنتی);
		}
		وإلا
		{
			ناپ_منزل_Write = (ULONG)(گنتی * channel->بلاک_ناپ);
		}
	}


	ASSERT(0 == (ناپ_منزل_Write % 512));
	وتھ_2.QuadPart = بلاک * channel->بلاک_ناپ;

	إذا(!_RawWrite(Nt_انکڑے->Handle, وتھ_2, ناپ_منزل_Write, buf, &Errno))
	{
		إذا (channel->write_error)
		{
			أرجع (channel->write_error)(channel, بلاک, گنتی, buf,
						ناپ_منزل_Write, 0, Errno);
		}
		وإلا
		{
			أرجع Errno;
		}
	}


	//
	// Stash a copy.
	//

	إذا(ناپ_منزل_Write >= Nt_انکڑے->بفر_ناپ)
	{
		Nt_انکڑے->بفر_بلاک_نمبر = بلاک;
		حافظہ_نقل(Nt_انکڑے->بفر_2, buf, Nt_انکڑے->بفر_ناپ);
	}

	Nt_انکڑے->Written = TRUE;

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
	PNT_نجی_انکڑے Nt_انکڑے = عدم;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_انکڑے = (PNT_نجی_انکڑے) channel->نجی_انکڑے;
	EXT2_CHECK_MAGIC(Nt_انکڑے, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	إذا(Nt_انکڑے->OpenedReadonly)
	{
		أرجع 0; // EACCESS;
	}


	//
	// Flush file buffers.
	//

	_FlushDrive(Nt_انکڑے->Handle);


	//
	// Test and correct partition type.
	//

	إذا(Nt_انکڑے->Written)
	{
		سیٹ_Part_Type(Nt_انکڑے->Handle, 0x83);
	}

	أرجع 0;
}


