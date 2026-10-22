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

#اگر_تعریف_شده HAVE_CONFIG_H
#ضمن "config.h"
#پایان_اگر


//
// I need some warnings to disable...
//


#رهنمود warning(disable:4514) // unreferenced inline function has been removed
#رهنمود warning(افزودن,4)

#رهنمود warning(disable:4201) // nonstandard extension used : nameless struct/union)
#رهنمود warning(disable:4214) // nonstandard extension used : bit field types other than int
#رهنمود warning(disable:4115) // named type definition in parentheses

#ضمن <ntddk.h>
#ضمن <ntdddisk.h>
#ضمن <ntstatus.h>

#رهنمود warning(برداشت)


//
// Some native APIs.
//

NTSYSAPI
ULONG
NTAPI
Rtl_Nt_وضعیت_مقصد_Dos_Error(
    درون_2 NTSTATUS وضعیت
   );

NTSYSAPI
NTSTATUS
NTAPI
NtClose(
    درون_2 HANDLE Handle
   );


NTSYSAPI
NTSTATUS
NTAPI
NtOpenFile(
    بیرون_2 PHANDLE FileHandle,
    درون_2 ACCESS_ماسک DesiredAccess,
    درون_2 POBJECT_صفات Object_صفات,
    بیرون_2 PIO_وضعیت_بلوک Io_وضعیت_بلوک,
    درون_2 ULONG ShareAccess,
    درون_2 ULONG OpenOptions
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFlushBuffersFile(
    درون_2 HANDLE FileHandle,
    بیرون_2 PIO_وضعیت_بلوک Io_وضعیت_بلوک
   );


NTSYSAPI
NTSTATUS
NTAPI
NtReadFile(
    درون_2 HANDLE FileHandle,
    درون_2 HANDLE Event OPTIONAL,
    درون_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    درون_2 PVOID ApcContext OPTIONAL,
    بیرون_2 PIO_وضعیت_بلوک Io_وضعیت_بلوک,
    بیرون_2 PVOID میانگیر_2,
    درون_2 ULONG درازی_2,
    درون_2 PLARGE_INTEGER Byte_جابه_جایی OPTIONAL,
    درون_2 PULONG کلید_2 OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtWriteFile(
    درون_2 HANDLE FileHandle,
    درون_2 HANDLE Event OPTIONAL,
    درون_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    درون_2 PVOID ApcContext OPTIONAL,
    بیرون_2 PIO_وضعیت_بلوک Io_وضعیت_بلوک,
    درون_2 PVOID میانگیر_2,
    درون_2 ULONG درازی_2,
    درون_2 PLARGE_INTEGER Byte_جابه_جایی OPTIONAL,
    درون_2 PULONG کلید_2 OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtDeviceIoControlFile(
    درون_2 HANDLE FileHandle,
    درون_2 HANDLE Event OPTIONAL,
    درون_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    درون_2 PVOID ApcContext OPTIONAL,
    بیرون_2 PIO_وضعیت_بلوک Io_وضعیت_بلوک,
    درون_2 ULONG Io_Control_کد,
    درون_2 PVOID درون_داد_میانگیر OPTIONAL,
    درون_2 ULONG درون_داد_میانگیر_درازی,
    بیرون_2 PVOID Output_میانگیر OPTIONAL,
    درون_2 ULONG Output_میانگیر_درازی
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFsControlFile(
    درون_2 HANDLE FileHandle,
    درون_2 HANDLE Event OPTIONAL,
    درون_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    درون_2 PVOID ApcContext OPTIONAL,
    بیرون_2 PIO_وضعیت_بلوک Io_وضعیت_بلوک,
    درون_2 ULONG Io_Control_کد,
    درون_2 PVOID درون_داد_میانگیر OPTIONAL,
    درون_2 ULONG درون_داد_میانگیر_درازی,
    بیرون_2 PVOID Output_میانگیر OPTIONAL,
    درون_2 ULONG Output_میانگیر_درازی
    );


NTSYSAPI
NTSTATUS
NTAPI
NtDelayExecution(
    درون_2 BOOLEAN Alertable,
    درون_2 PLARGE_INTEGER Interval
    );


#تعریف_کن FSCTL_قفل_VOLUME               CTL_کد(FILE_DEVICE_FILE_SYSTEM, 6, روش_BUFFERED, FILE_ANY_ACCESS)
#تعریف_کن FSCTL_UNLOCK_VOLUME             CTL_کد(FILE_DEVICE_FILE_SYSTEM, 7, روش_BUFFERED, FILE_ANY_ACCESS)
#تعریف_کن FSCTL_DISMOUNT_VOLUME           CTL_کد(FILE_DEVICE_FILE_SYSTEM, 8, روش_BUFFERED, FILE_ANY_ACCESS)
#تعریف_کن FSCTL_IS_VOLUME_MOUNTED         CTL_کد(FILE_DEVICE_FILE_SYSTEM,10, روش_BUFFERED, FILE_ANY_ACCESS)


//
// useful macros
//

#تعریف_کن BooleanFlagOn(Flags,SingleFlag) ((BOOLEAN)((((Flags) & (SingleFlag)) != 0)))


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


#تعریف_کن EXT2_CHECK_MAGIC(بنية, کد) \
	  إذا ((بنية)->magic != (کد)) أرجع (کد)

#تعریف_کن EXT2_ET_MAGIC_NT_IO_CHANNEL  0x10ed


//
// Private data block
//

تعريفنوع بنية NT_خصوصی_داده {
	صحيح	   magic;
	HANDLE Handle;
	صحيح	   Flags;
	PCHAR  میانگیر_2;
	__u32  میانگیر_بلوک_عدد;
	ULONG  میانگیر_اندازه;
	BOOLEAN OpenedReadonly;
	BOOLEAN Written;
}NT_خصوصی_داده_2, *PNT_خصوصی_داده;



//
// Standard interface prototypes
//

ساكن errcode_t nt_open(ثابت حرف *name, صحيح flags, io_channel *channel);
ساكن errcode_t nt_close(io_channel channel);
ساكن errcode_t nt_مجموعه_blksize(io_channel channel, صحيح blksize);
ساكن errcode_t nt_read_blk(io_channel channel, بیعلامت دراز بلوک,
			       صحيح شمار, فراغ *داده);
ساكن errcode_t nt_write_blk(io_channel channel, بیعلامت دراز بلوک,
				صحيح شمار, ثابت فراغ *داده);
ساكن errcode_t nt_flush(io_channel channel);

ساكن بنية struct_io_manager struct_nt_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "NT I/O Manager",
	.open		= nt_open,
	.close		= nt_close,
	.مجموعه_blksize	= nt_مجموعه_blksize,
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
}ERROR_مدخل;

ساكن ERROR_مدخل Error_جدول[] = {
        {  ERROR_INVALID_FUNCTION,       EINVAL    },
        {  ERROR_FILE_NOT_یافت,         ENOENT    },
        {  ERROR_مسیر_NOT_یافت,         ENOENT    },
        {  ERROR_TOO_MANY_OPEN_FILES,    EMFILE    },
        {  ERROR_ACCESS_DENIED,          EACCES    },
        {  ERROR_INVALID_HANDLE,         EBADF     },
        {  ERROR_ARENA_TRASHED,          ENOMEM    },
        {  ERROR_NOT_ENOUGH_حافظه,      ENOMEM    },
        {  ERROR_INVALID_بلوک,          ENOMEM    },
        {  ERROR_BAD_ENVIRONMENT,        E2BIG     },
        {  ERROR_BAD_FORMAT,             ENOEXEC   },
        {  ERROR_INVALID_ACCESS,         EINVAL    },
        {  ERROR_INVALID_داده,           EINVAL    },
        {  ERROR_INVALID_DRIVE,          ENOENT    },
        {  ERROR_CURRENT_DIRECTORY,      EACCES    },
        {  ERROR_NOT_SAME_DEVICE,        EXDEV     },
        {  ERROR_NO_بیشتر_FILES,          ENOENT    },
        {  ERROR_قفل_VIOLATION,         EACCES    },
        {  ERROR_BAD_NETPATH,            ENOENT    },
        {  ERROR_شبکه_ACCESS_DENIED,  EACCES    },
        {  ERROR_BAD_NET_NAME,           ENOENT    },
        {  ERROR_FILE_EXISTS,            EEXIST    },
        {  ERROR_CANNOT_MAKE,            EACCES    },
        {  ERROR_FAIL_I24,               EACCES    },
        {  ERROR_INVALID_PARAMETER,      EINVAL    },
        {  ERROR_NO_PROC_SLOTS,          EAGAIN    },
        {  ERROR_DRIVE_LOCKED,           EACCES    },
        {  ERROR_BROKEN_PIPE,            EPIPE     },
        {  ERROR_DISK_پر,              ENOSPC    },
        {  ERROR_INVALID_هدف_HANDLE,  EBADF     },
        {  ERROR_INVALID_HANDLE,         EINVAL    },
        {  ERROR_WAIT_NO_CHILDREN,       ECHILD    },
        {  ERROR_فرزند_NOT_COMPLETE,     ECHILD    },
        {  ERROR_مستقیم_ACCESS_HANDLE,   EBADF     },
        {  ERROR_NEGATIVE_SEEK,          EINVAL    },
        {  ERROR_SEEK_ON_DEVICE,         EACCES    },
        {  ERROR_DIR_NOT_خالی,          ENOTEMPTY },
        {  ERROR_NOT_LOCKED,             EACCES    },
        {  ERROR_BAD_PATHNAME,           ENOENT    },
        {  ERROR_MAX_THRDS_REACHED,      EAGAIN    },
        {  ERROR_قفل_FAILED,            EACCES    },
        {  ERROR_ALREADY_EXISTS,         EEXIST    },
        {  ERROR_FILENAME_EXCED_RANGE,   ENOENT    },
        {  ERROR_NESTING_NOT_ALLOWED,    EAGAIN    },
        {  ERROR_NOT_ENOUGH_QUOTA,       ENOMEM    }
};




ساكن
بیعلامت
_MapDosError (
    درون_2 ULONG WinError
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
		(WinError <= ERROR_SHARING_میانگیر_EXCEEDED))
	{
		أرجع EACCES;
	}
	وإلا إذا ((WinError >= ERROR_INVALID_STARTING_CODESEG) &&
			 (WinError <= ERROR_INFLOOP_درون_RELOC_CHAIN))
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
بیعلامت
Map_Nt_وضعیت(
    درون_2 NTSTATUS وضعیت
   )
{
	أرجع _MapDosError(Rtl_Nt_وضعیت_مقصد_Dos_Error(وضعیت));
}





//
// Helper functions to make things easier
//

ساكن
NTSTATUS
_OpenNtName(
    درون_2 PCSTR Name,
    درون_2 BOOLEAN Readonly,
    بیرون_2 PHANDLE Handle,
    بیرون_2 PBOOLEAN OpenedReadonly OPTIONAL
   )
{
	UNICODE_STRING UnicodeString;
	ANSI_STRING    AnsiString;
	WCHAR میانگیر_2[512];
	NTSTATUS وضعیت;
	OBJECT_صفات Object_صفات;
	IO_وضعیت_بلوک Io_وضعیت_بلوک;

	//
	// Make Unicode name from input string
	//

	UnicodeString.میانگیر_2 = &میانگیر_2[0];
	UnicodeString.درازی_2 = 0;
	UnicodeString.بیشینه_درازی = حجم(میانگیر_2); // in bytes!!!

	RtlInitAnsiString(&AnsiString, Name);

	وضعیت = Rtl_Ansi_String_مقصد_Unicode_String(&UnicodeString, &AnsiString, FALSE);

	إذا(!NT_SUCCESS(وضعیت))
	{
		أرجع وضعیت; // Unmappable character?
	}

	//
	// Initialize object
	//

	Initialize_Object_صفات(&Object_صفات,
							   &UnicodeString,
							   OBJ_CASE_INSENSITIVE,
							   عدم,
							   عدم );

	//
	// Try to open it in initial mode
	//

	إذا(آرگومان_PRESENT(OpenedReadonly))
	{
		*OpenedReadonly = Readonly;
	}


	وضعیت = NtOpenFile(Handle,
						SYNCHRONIZE | FILE_READ_داده | (Readonly ? 0 : FILE_WRITE_داده),
						&Object_صفات,
						&Io_وضعیت_بلوک,
						FILE_SHARE_WRITE | FILE_SHARE_READ,
						FILE_SYNCHRONOUS_IO_NONALERT);

	إذا(!NT_SUCCESS(وضعیت))
	{
		//
		// Maybe was just mounted? wait 0.5 sec and retry.
		//

		LARGE_INTEGER Interval;
		Interval.QuadPart = -5000000; // 0.5 sec. from now

		NtDelayExecution(FALSE, &Interval);

		وضعیت = NtOpenFile(Handle,
							SYNCHRONIZE | FILE_READ_داده | (Readonly ? 0 : FILE_WRITE_داده),
							&Object_صفات,
							&Io_وضعیت_بلوک,
							FILE_SHARE_WRITE | FILE_SHARE_READ,
							FILE_SYNCHRONOUS_IO_NONALERT);

		//
		// Try to satisfy mode
		//

		إذا((وضعیت_ACCESS_DENIED == وضعیت) && !Readonly)
		{
			إذا(آرگومان_PRESENT(OpenedReadonly))
			{
				*OpenedReadonly = TRUE;
			}

			وضعیت = NtOpenFile(Handle,
							SYNCHRONIZE | FILE_READ_داده,
							&Object_صفات,
							&Io_وضعیت_بلوک,
							FILE_SHARE_WRITE | FILE_SHARE_READ,
							FILE_SYNCHRONOUS_IO_NONALERT);
		}
	}



	//
	//انجامشده
	//

	أرجع وضعیت;
}


ساكن
NTSTATUS
_OpenDriveLetter(
    درون_2 CHAR Letter,
    درون_2 BOOLEAN ReadOnly,
    بیرون_2 PHANDLE Handle,
    بیرون_2 PBOOLEAN OpenedReadonly OPTIONAL
   )
{
	CHAR میانگیر_2[100];

	sprintf(میانگیر_2, "\\DosDevices\\%c:", Letter);

	أرجع _OpenNtName(میانگیر_2, ReadOnly, Handle, OpenedReadonly);
}


//
// Flush device
//

ساكن
__inline
NTSTATUS
_FlushDrive(
		درون_2 HANDLE Handle
		)
{
	IO_وضعیت_بلوک Io_وضعیت_بلوک;
	أرجع NtFlushBuffersFile(Handle, &Io_وضعیت_بلوک);
}


//
// lock drive
//

ساكن
__inline
NTSTATUS
قفل_Drive(
		درون_2 HANDLE Handle
		)
{
	IO_وضعیت_بلوک Io_وضعیت_بلوک;
	أرجع NtFsControlFile(Handle, 0, 0, 0, &Io_وضعیت_بلوک, FSCTL_قفل_VOLUME, 0, 0, 0, 0);
}


//
// unlock drive
//

ساكن
__inline
NTSTATUS
_UnlockDrive(
	درون_2 HANDLE Handle
	)
{
	IO_وضعیت_بلوک Io_وضعیت_بلوک;
	أرجع NtFsControlFile(Handle, 0, 0, 0, &Io_وضعیت_بلوک, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0);
}

ساكن
__inline
NTSTATUS
_DismountDrive(
	درون_2 HANDLE Handle
	)
{
	IO_وضعیت_بلوک Io_وضعیت_بلوک;
	أرجع NtFsControlFile(Handle, 0, 0, 0, &Io_وضعیت_بلوک, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0);
}


//
// is mounted
//

ساكن
__inline
BOOLEAN
_IsMounted(
	درون_2 HANDLE Handle
	)
{
	IO_وضعیت_بلوک Io_وضعیت_بلوک;
	NTSTATUS وضعیت;
	وضعیت = NtFsControlFile(Handle, 0, 0, 0, &Io_وضعیت_بلوک, FSCTL_IS_VOLUME_MOUNTED, 0, 0, 0, 0);
	أرجع (BOOLEAN)(وضعیت_SUCCESS == وضعیت);
}


ساكن
__inline
NTSTATUS
_CloseDisk(
		درون_2 HANDLE Handle
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
    درون_2 PCSTR Device,
    درون_2 PSTR Normalized_Device_Name_میانگیر
   )
{
	صحيح افراز_عدد = -1;
	UCHAR Disk_عدد;
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

		Disk_عدد = (UCHAR)(*(Device + 2) - 'a');

		إذا(*(Device + 3) != '\0')
		{
			افراز_عدد = (*(Device + 3) - '0');
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

		Disk_عدد = (UCHAR)(*(Device + 2) - '0');

	}
	وإلا
	{
		//
		// invalid prefix
		//

		أرجع عدم;
	}



	//
	//پیشوند
	//

	strcpy(Normalized_Device_Name_میانگیر, "\\Device\\");

	//
	// Media name
	//

	بدل(*Device)
	{

	حالة 'f':
		strcat(Normalized_Device_Name_میانگیر, "Floppy0");
		اكسر;

	حالة 'h':
		strcat(Normalized_Device_Name_میانگیر, "Harddisk0");
		اكسر;
	}


	p = Normalized_Device_Name_میانگیر + طول_رشته(Normalized_Device_Name_میانگیر) - 1;
	*p = (CHAR)(*p + Disk_عدد);


	//
	// Partition nr.
	//

	إذا(افراز_عدد >= 0)
	{
		strcat(Normalized_Device_Name_میانگیر, "\\Partition0");

		p = Normalized_Device_Name_میانگیر + طول_رشته(Normalized_Device_Name_میانگیر) - 1;
		*p = (CHAR)(*p + افراز_عدد);
	}


	أرجع Normalized_Device_Name_میانگیر;
}




ساكن
VOID
Get_Device_اندازه(
    درون_2 HANDLE h,
    بیرون_2 بیعلامت __int64 *Fs_اندازه
   )
{
	افراز_INFORMATION pi;
	DISK_GEOMETRY gi;
	NTSTATUS وضعیت;
	IO_وضعیت_بلوک Io_وضعیت_بلوک;

	//
	// Zero it
	//

	*Fs_اندازه = 0;

	//
	// Call driver
	//

	Rtl_Zero_حافظه(&pi, حجم(افراز_INFORMATION));

	وضعیت = NtDeviceIoControlFile(
		h, عدم, عدم, عدم, &Io_وضعیت_بلوک, IOCTL_DISK_GET_افراز_INFO,
		&pi, حجم(افراز_INFORMATION),
		&pi, حجم(افراز_INFORMATION));


	إذا(NT_SUCCESS(وضعیت))
	{
		*Fs_اندازه = pi.افراز_درازی.QuadPart;
	}
	وإلا إذا(وضعیت_INVALID_DEVICE_REQUEST == وضعیت)
	{
		//
		// No partitions: get device info.
		//

		Rtl_Zero_حافظه(&gi, حجم(DISK_GEOMETRY));

		وضعیت = NtDeviceIoControlFile(
				h, عدم, عدم, عدم, &Io_وضعیت_بلوک, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, حجم(DISK_GEOMETRY),
				&gi, حجم(DISK_GEOMETRY));


		إذا(NT_SUCCESS(وضعیت))
		{
			*Fs_اندازه =
				gi.بایتها_در_هر_Sector *
				gi.Sectors_در_هر_Track *
				gi.Tracks_در_هر_Cylinder *
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
    درون_2 PCSTR Name,
    درون_2 BOOLEAN ReadOnly,
    بیرون_2 PHANDLE Handle,
    بیرون_2 PBOOLEAN OpenedReadonly OPTIONAL,
    بیرون_2 بیعلامت *Errno OPTIONAL
   )
{
	CHAR NormalizedDeviceName[512];
	NTSTATUS وضعیت;

	إذا(عدم == Name)
	{
		//
		// Set not found
		//

		إذا(آرگومان_PRESENT(Errno))
			*Errno = ENOENT;

		أرجع FALSE;
	}


	إذا((((*Name) | 0x20) >= 'a') && (((*Name) | 0x20) <= 'z') &&
		(':' == *(Name + 1)) && ('\0' == *(Name + 2)))
	{
		وضعیت = _OpenDriveLetter(*Name, ReadOnly, Handle, OpenedReadonly);
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

			إذا(آرگومان_PRESENT(Errno))
				*Errno = ENOENT;

			أرجع FALSE;
		}

		//
		// Try to open it
		//

		وضعیت = _OpenNtName(Name, ReadOnly, Handle, OpenedReadonly);
	}


	إذا(!NT_SUCCESS(وضعیت))
	{
		إذا(آرگومان_PRESENT(Errno))
			*Errno = Map_Nt_وضعیت(وضعیت);

		أرجع FALSE;
	}

	أرجع TRUE;
}


//
// Raw block io. Sets dos errno
//

ساكن
BOOLEAN
بلوک_Io(
    درون_2 HANDLE Handle,
    درون_2 LARGE_INTEGER جابه_جایی_2,
    درون_2 ULONG بایتها_2,
    درون_2 بیرون_2 PCHAR میانگیر_2,
    درون_2 BOOLEAN Read,
    بیرون_2 بیعلامت* Errno
   )
{
	IO_وضعیت_بلوک Io_وضعیت_بلوک;
	NTSTATUS وضعیت;

	//
	// Should be aligned
	//

	ASSERT(0 == (بایتها_2 % 512));
	ASSERT(0 == (جابه_جایی_2.کم_Part % 512));


	//
	// perform io
	//

	إذا(Read)
	{
		وضعیت = NtReadFile(Handle, عدم, عدم, عدم,
			&Io_وضعیت_بلوک, میانگیر_2, بایتها_2, &جابه_جایی_2, عدم);
	}
	وإلا
	{
		وضعیت = NtWriteFile(Handle, عدم, عدم, عدم,
			&Io_وضعیت_بلوک, میانگیر_2, بایتها_2, &جابه_جایی_2, عدم);
	}


	//
	// translate error
	//

	إذا(NT_SUCCESS(وضعیت))
	{
		*Errno = 0;
		أرجع TRUE;
	}

	*Errno = Map_Nt_وضعیت(وضعیت);

	أرجع FALSE;
}



__inline
BOOLEAN
_RawWrite(
    درون_2 HANDLE Handle,
    درون_2 LARGE_INTEGER جابه_جایی_2,
    درون_2 ULONG بایتها_2,
    بیرون_2 ثابت CHAR* میانگیر_2,
    بیرون_2 بیعلامت* Errno
   )
{
	أرجع بلوک_Io(Handle, جابه_جایی_2, بایتها_2, (PCHAR)میانگیر_2, FALSE, Errno);
}

__inline
BOOLEAN
_RawRead(
    درون_2 HANDLE Handle,
    درون_2 LARGE_INTEGER جابه_جایی_2,
    درون_2 ULONG بایتها_2,
    درون_2 PCHAR میانگیر_2,
    بیرون_2 بیعلامت* Errno
   )
{
	أرجع بلوک_Io(Handle, جابه_جایی_2, بایتها_2, میانگیر_2, TRUE, Errno);
}



__inline
BOOLEAN
مجموعه_Part_Type(
    درون_2 HANDLE Handle,
    درون_2 UCHAR Type
   )
{
	IO_وضعیت_بلوک Io_وضعیت_بلوک;
	أرجع وضعیت_SUCCESS == NtDeviceIoControlFile(
												   Handle, عدم, عدم, عدم, &Io_وضعیت_بلوک, IOCTL_DISK_مجموعه_افراز_INFO,
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

ساكن __int64 Fs_اندازه = 0;
ساكن حرف knowndevice[1024] = "";


errcode_t
ext2fs_get_device_اندازه(ثابت حرف *file, صحيح blocksize,
				 blk_t *retblocks)
{
	HANDLE h;
	BOOLEAN Readonly;

	إذا((0 == Fs_اندازه) || (0 != strcmp(knowndevice, file)))
	{

		إذا(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, عدم))
		{
			أرجع 0;
		}


		__try{

			//
			// Get size
			//

			Get_Device_اندازه(h, &Fs_اندازه);
			strcpy(knowndevice, file);
		}
		__finally{
			_CloseDisk(h);
		}

	}

	*retblocks = (blk_t)(بیعلامت __int64)(Fs_اندازه / blocksize);
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
	PNT_خصوصی_داده Nt_داده = عدم;
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

		io = (io_channel) حافظه_بده(حجم(بنية struct_io_channel));

		إذا (عدم == io)
		{
			Errno = ENOMEM;
			__leave;
		}

		Rtl_Zero_حافظه(io, حجم(بنية struct_io_channel));
		io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

		Nt_داده = (PNT_خصوصی_داده)حافظه_بده(حجم(NT_خصوصی_داده_2));

		إذا (عدم == Nt_داده)
		{
			Errno = ENOMEM;
			__leave;
		}


		io->manager = nt_io_manager();
		io->name = حافظه_بده(طول_رشته(name) + 1);
		إذا (عدم == io->name)
		{
			Errno = ENOMEM;
			__leave;
		}

		strcpy(io->name, name);
		io->خصوصی_داده = Nt_داده;
		io->بلوک_اندازه = 1024;
		io->read_error = 0;
		io->write_error = 0;
		io->refcount = 1;

		//
		// Initialize data
		//

		Rtl_Zero_حافظه(Nt_داده, حجم(NT_خصوصی_داده_2));

		Nt_داده->magic = EXT2_ET_MAGIC_NT_IO_CHANNEL;
		Nt_داده->میانگیر_بلوک_عدد = 0xffffffff;
		Nt_داده->میانگیر_اندازه = 1024;
		Nt_داده->میانگیر_2 = حافظه_بده(Nt_داده->میانگیر_اندازه);

		إذا (عدم == Nt_داده->میانگیر_2)
		{
			Errno = ENOMEM;
			__leave;
		}

		//
		// Open it
		//

		إذا(!_Ext2OpenDevice(name, (BOOLEAN)!BooleanFlagOn(flags, EXT2_FLAG_RW), &Nt_داده->Handle, &Nt_داده->OpenedReadonly, &Errno))
		{
			__leave;
		}


		//
		// get size
		//

		Get_Device_اندازه(Nt_داده->Handle, &Fs_اندازه);
		strcpy(knowndevice, name);


		//
		// Lock/dismount
		//

		إذا(!NT_SUCCESS(قفل_Drive(Nt_داده->Handle)) /*|| !NT_SUCCESS(_DismountDrive(NtData->Handle))*/)
		{
			Nt_داده->OpenedReadonly = TRUE;
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

			إذا (عدم != Nt_داده)
			{
				إذا(عدم != Nt_داده->Handle)
				{
					_UnlockDrive(Nt_داده->Handle);
					_CloseDisk(Nt_داده->Handle);
				}

				حرر(Nt_داده->میانگیر_2);
				حرر(Nt_داده);
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
	PNT_خصوصی_داده Nt_داده = عدم;

	إذا(عدم == channel)
	{
		أرجع 0;
	}

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_داده = (PNT_خصوصی_داده) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(Nt_داده, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	إذا (--channel->refcount > 0)
	{
		أرجع 0;
	}

	حرر(channel->name);
	حرر(channel);

	إذا (عدم != Nt_داده)
	{
		إذا(عدم != Nt_داده->Handle)
		{
			_DismountDrive(Nt_داده->Handle);
			_UnlockDrive(Nt_داده->Handle);
			_CloseDisk(Nt_داده->Handle);
		}

		حرر(Nt_داده->میانگیر_2);
		حرر(Nt_داده);
	}

	أرجع 0;
}



//
// set block size
//

ساكن
errcode_t
nt_مجموعه_blksize(io_channel channel, صحيح blksize)
{
	PNT_خصوصی_داده Nt_داده = عدم;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_داده = (PNT_خصوصی_داده) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(Nt_داده, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	إذا (channel->بلوک_اندازه != blksize)
	{
		channel->بلوک_اندازه = blksize;

		حرر(Nt_داده->میانگیر_2);
		Nt_داده->میانگیر_بلوک_عدد = 0xffffffff;
		Nt_داده->میانگیر_اندازه = channel->بلوک_اندازه;
		ASSERT(0 == (Nt_داده->میانگیر_اندازه % 512));

		Nt_داده->میانگیر_2 = حافظه_بده(Nt_داده->میانگیر_اندازه);

		إذا (عدم == Nt_داده->میانگیر_2)
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
nt_read_blk(io_channel channel, بیعلامت دراز بلوک,
			       صحيح شمار, فراغ *buf)
{
	PVOID میانگیر_مقصد_Read;
	ULONG اندازه_مقصد_Read;
	ULONG اندازه_2;
	LARGE_INTEGER جابه_جایی_2;
	PNT_خصوصی_داده Nt_داده = عدم;
	بیعلامت Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_داده = (PNT_خصوصی_داده) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(Nt_داده, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	//
	// If it's in the cache, use it!
	//

	إذا ((1 == شمار) &&
		(بلوک == Nt_داده->میانگیر_بلوک_عدد) &&
		(Nt_داده->میانگیر_بلوک_عدد != 0xffffffff))
	{
		کپی_حافظه(buf, Nt_داده->میانگیر_2, channel->بلوک_اندازه);
		أرجع 0;
	}

	اندازه_2 = (شمار < 0) ? (ULONG)(-شمار) : (ULONG)(شمار * channel->بلوک_اندازه);

	جابه_جایی_2.QuadPart = بلوک * channel->بلوک_اندازه;

	//
	// If not fit to the block
	//

	إذا(اندازه_2 <= Nt_داده->میانگیر_اندازه)
	{
		//
		// Update the cache
		//

		Nt_داده->میانگیر_بلوک_عدد = بلوک;
		میانگیر_مقصد_Read = Nt_داده->میانگیر_2;
		اندازه_مقصد_Read = Nt_داده->میانگیر_اندازه;
	}
	وإلا
	{
		اندازه_مقصد_Read = اندازه_2;
		میانگیر_مقصد_Read = buf;
		ASSERT(0 == (اندازه_مقصد_Read % channel->بلوک_اندازه));
	}

	إذا(!_RawRead(Nt_داده->Handle, جابه_جایی_2, اندازه_مقصد_Read, میانگیر_مقصد_Read, &Errno))
	{

		إذا (channel->read_error)
		{
			أرجع (channel->read_error)(channel, بلوک, شمار, buf,
					       اندازه_2, 0, Errno);
		}
		وإلا
		{
			أرجع Errno;
		}
	}


	إذا(میانگیر_مقصد_Read != buf)
	{
		ASSERT(اندازه_2 <= اندازه_مقصد_Read);
		کپی_حافظه(buf, میانگیر_مقصد_Read, اندازه_2);
	}

	أرجع 0;
}


//
// write block
//

ساكن
errcode_t
nt_write_blk(io_channel channel, بیعلامت دراز بلوک,
				صحيح شمار, ثابت فراغ *buf)
{
	ULONG اندازه_مقصد_Write;
	LARGE_INTEGER جابه_جایی_2;
	PNT_خصوصی_داده Nt_داده = عدم;
	بیعلامت Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_داده = (PNT_خصوصی_داده) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(Nt_داده, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	إذا(Nt_داده->OpenedReadonly)
	{
		أرجع EACCES;
	}

	إذا (شمار == 1)
	{
		اندازه_مقصد_Write = channel->بلوک_اندازه;
	}
	وإلا
	{
		Nt_داده->میانگیر_بلوک_عدد = 0xffffffff;

		إذا (شمار < 0)
		{
			اندازه_مقصد_Write = (ULONG)(-شمار);
		}
		وإلا
		{
			اندازه_مقصد_Write = (ULONG)(شمار * channel->بلوک_اندازه);
		}
	}


	ASSERT(0 == (اندازه_مقصد_Write % 512));
	جابه_جایی_2.QuadPart = بلوک * channel->بلوک_اندازه;

	إذا(!_RawWrite(Nt_داده->Handle, جابه_جایی_2, اندازه_مقصد_Write, buf, &Errno))
	{
		إذا (channel->write_error)
		{
			أرجع (channel->write_error)(channel, بلوک, شمار, buf,
						اندازه_مقصد_Write, 0, Errno);
		}
		وإلا
		{
			أرجع Errno;
		}
	}


	//
	// Stash a copy.
	//

	إذا(اندازه_مقصد_Write >= Nt_داده->میانگیر_اندازه)
	{
		Nt_داده->میانگیر_بلوک_عدد = بلوک;
		کپی_حافظه(Nt_داده->میانگیر_2, buf, Nt_داده->میانگیر_اندازه);
	}

	Nt_داده->Written = TRUE;

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
	PNT_خصوصی_داده Nt_داده = عدم;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_داده = (PNT_خصوصی_داده) channel->خصوصی_داده;
	EXT2_CHECK_MAGIC(Nt_داده, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	إذا(Nt_داده->OpenedReadonly)
	{
		أرجع 0; // EACCESS;
	}


	//
	// Flush file buffers.
	//

	_FlushDrive(Nt_داده->Handle);


	//
	// Test and correct partition type.
	//

	إذا(Nt_داده->Written)
	{
		مجموعه_Part_Type(Nt_داده->Handle, 0x83);
	}

	أرجع 0;
}


