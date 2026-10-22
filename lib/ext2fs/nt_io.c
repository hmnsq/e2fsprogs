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

#که_تعریف_شوی HAVE_CONFIG_H
#ضمن "config.h"
#د_که_پای


//
// I need some warnings to disable...
//


#لارښوونه warning(disable:4514) // unreferenced inline function has been removed
#لارښوونه warning(ټېل,4)

#لارښوونه warning(disable:4201) // nonstandard extension used : nameless struct/union)
#لارښوونه warning(disable:4214) // nonstandard extension used : bit field types other than int
#لارښوونه warning(disable:4115) // named type definition in parentheses

#ضمن <ntddk.h>
#ضمن <ntdddisk.h>
#ضمن <ntstatus.h>

#لارښوونه warning(راوباسه)


//
// Some native APIs.
//

NTSYSAPI
ULONG
NTAPI
Rtl_Nt_حالت_منزل_Dos_Error(
    دننه_2 NTSTATUS حالت
   );

NTSYSAPI
NTSTATUS
NTAPI
NtClose(
    دننه_2 HANDLE Handle
   );


NTSYSAPI
NTSTATUS
NTAPI
NtOpenFile(
    بهر_2 PHANDLE FileHandle,
    دننه_2 ACCESS_ماسک DesiredAccess,
    دننه_2 POBJECT_ځانګړتیاوې Object_ځانګړتیاوې,
    بهر_2 PIO_حالت_بلاک Io_حالت_بلاک,
    دننه_2 ULONG ShareAccess,
    دننه_2 ULONG OpenOptions
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFlushBuffersFile(
    دننه_2 HANDLE FileHandle,
    بهر_2 PIO_حالت_بلاک Io_حالت_بلاک
   );


NTSYSAPI
NTSTATUS
NTAPI
NtReadFile(
    دننه_2 HANDLE FileHandle,
    دننه_2 HANDLE Event OPTIONAL,
    دننه_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    دننه_2 PVOID ApcContext OPTIONAL,
    بهر_2 PIO_حالت_بلاک Io_حالت_بلاک,
    بهر_2 PVOID بفر_2,
    دننه_2 ULONG اوږدتیا_2,
    دننه_2 PLARGE_INTEGER Byte_دننه_واټن OPTIONAL,
    دننه_2 PULONG کلی_2 OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtWriteFile(
    دننه_2 HANDLE FileHandle,
    دننه_2 HANDLE Event OPTIONAL,
    دننه_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    دننه_2 PVOID ApcContext OPTIONAL,
    بهر_2 PIO_حالت_بلاک Io_حالت_بلاک,
    دننه_2 PVOID بفر_2,
    دننه_2 ULONG اوږدتیا_2,
    دننه_2 PLARGE_INTEGER Byte_دننه_واټن OPTIONAL,
    دننه_2 PULONG کلی_2 OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtDeviceIoControlFile(
    دننه_2 HANDLE FileHandle,
    دننه_2 HANDLE Event OPTIONAL,
    دننه_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    دننه_2 PVOID ApcContext OPTIONAL,
    بهر_2 PIO_حالت_بلاک Io_حالت_بلاک,
    دننه_2 ULONG Io_Control_کوډ,
    دننه_2 PVOID ننوت_بفر OPTIONAL,
    دننه_2 ULONG ننوت_بفر_اوږدتیا,
    بهر_2 PVOID Output_بفر OPTIONAL,
    دننه_2 ULONG Output_بفر_اوږدتیا
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFsControlFile(
    دننه_2 HANDLE FileHandle,
    دننه_2 HANDLE Event OPTIONAL,
    دننه_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    دننه_2 PVOID ApcContext OPTIONAL,
    بهر_2 PIO_حالت_بلاک Io_حالت_بلاک,
    دننه_2 ULONG Io_Control_کوډ,
    دننه_2 PVOID ننوت_بفر OPTIONAL,
    دننه_2 ULONG ننوت_بفر_اوږدتیا,
    بهر_2 PVOID Output_بفر OPTIONAL,
    دننه_2 ULONG Output_بفر_اوږدتیا
    );


NTSYSAPI
NTSTATUS
NTAPI
NtDelayExecution(
    دننه_2 BOOLEAN Alertable,
    دننه_2 PLARGE_INTEGER Interval
    );


#تعریف_کړه FSCTL_قلف_VOLUME               CTL_کوډ(FILE_DEVICE_FILE_SYSTEM, 6, لاره_BUFFERED, FILE_ANY_ACCESS)
#تعریف_کړه FSCTL_UNLOCK_VOLUME             CTL_کوډ(FILE_DEVICE_FILE_SYSTEM, 7, لاره_BUFFERED, FILE_ANY_ACCESS)
#تعریف_کړه FSCTL_DISMOUNT_VOLUME           CTL_کوډ(FILE_DEVICE_FILE_SYSTEM, 8, لاره_BUFFERED, FILE_ANY_ACCESS)
#تعریف_کړه FSCTL_IS_VOLUME_MOUNTED         CTL_کوډ(FILE_DEVICE_FILE_SYSTEM,10, لاره_BUFFERED, FILE_ANY_ACCESS)


//
// useful macros
//

#تعریف_کړه BooleanFlagOn(Flags,SingleFlag) ((BOOLEAN)((((Flags) & (SingleFlag)) != 0)))


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


#تعریف_کړه EXT2_CHECK_MAGIC(بنية, کوډ) \
	  إذا ((بنية)->magic != (کوډ)) أرجع (کوډ)

#تعریف_کړه EXT2_ET_MAGIC_NT_IO_CHANNEL  0x10ed


//
// Private data block
//

تعريفنوع بنية NT_شخصي_معلومات {
	صحيح	   magic;
	HANDLE Handle;
	صحيح	   Flags;
	PCHAR  بفر_2;
	__u32  بفر_بلاک_شمېره;
	ULONG  بفر_اندازه;
	BOOLEAN OpenedReadonly;
	BOOLEAN Written;
}NT_شخصي_معلومات_2, *PNT_شخصي_معلومات;



//
// Standard interface prototypes
//

ساكن errcode_t nt_open(ثابت حرف *name, صحيح flags, io_channel *channel);
ساكن errcode_t nt_close(io_channel channel);
ساكن errcode_t nt_سېټ_blksize(io_channel channel, صحيح blksize);
ساكن errcode_t nt_read_blk(io_channel channel, بینښه اوږد بلاک,
			       صحيح شمېر, فراغ *معلومات);
ساكن errcode_t nt_write_blk(io_channel channel, بینښه اوږد بلاک,
				صحيح شمېر, ثابت فراغ *معلومات);
ساكن errcode_t nt_flush(io_channel channel);

ساكن بنية struct_io_manager struct_nt_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "NT I/O Manager",
	.open		= nt_open,
	.close		= nt_close,
	.سېټ_blksize	= nt_سېټ_blksize,
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
}ERROR_ثبت;

ساكن ERROR_ثبت Error_جدول[] = {
        {  ERROR_INVALID_FUNCTION,       EINVAL    },
        {  ERROR_FILE_NOT_وموندل,         ENOENT    },
        {  ERROR_لار_NOT_وموندل,         ENOENT    },
        {  ERROR_TOO_MANY_OPEN_FILES,    EMFILE    },
        {  ERROR_ACCESS_DENIED,          EACCES    },
        {  ERROR_INVALID_HANDLE,         EBADF     },
        {  ERROR_ARENA_TRASHED,          ENOMEM    },
        {  ERROR_NOT_ENOUGH_حافظه,      ENOMEM    },
        {  ERROR_INVALID_بلاک,          ENOMEM    },
        {  ERROR_BAD_ENVIRONMENT,        E2BIG     },
        {  ERROR_BAD_FORMAT,             ENOEXEC   },
        {  ERROR_INVALID_ACCESS,         EINVAL    },
        {  ERROR_INVALID_معلومات,           EINVAL    },
        {  ERROR_INVALID_DRIVE,          ENOENT    },
        {  ERROR_CURRENT_DIRECTORY,      EACCES    },
        {  ERROR_NOT_SAME_DEVICE,        EXDEV     },
        {  ERROR_NO_نور_FILES,          ENOENT    },
        {  ERROR_قلف_VIOLATION,         EACCES    },
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
        {  ERROR_DISK_ډک,              ENOSPC    },
        {  ERROR_INVALID_موخه_HANDLE,  EBADF     },
        {  ERROR_INVALID_HANDLE,         EINVAL    },
        {  ERROR_WAIT_NO_CHILDREN,       ECHILD    },
        {  ERROR_ماشوم_NOT_COMPLETE,     ECHILD    },
        {  ERROR_مستقیم_ACCESS_HANDLE,   EBADF     },
        {  ERROR_NEGATIVE_SEEK,          EINVAL    },
        {  ERROR_SEEK_ON_DEVICE,         EACCES    },
        {  ERROR_DIR_NOT_تشه,          ENOTEMPTY },
        {  ERROR_NOT_LOCKED,             EACCES    },
        {  ERROR_BAD_PATHNAME,           ENOENT    },
        {  ERROR_MAX_THRDS_REACHED,      EAGAIN    },
        {  ERROR_قلف_FAILED,            EACCES    },
        {  ERROR_ALREADY_EXISTS,         EEXIST    },
        {  ERROR_FILENAME_EXCED_RANGE,   ENOENT    },
        {  ERROR_NESTING_NOT_ALLOWED,    EAGAIN    },
        {  ERROR_NOT_ENOUGH_QUOTA,       ENOMEM    }
};




ساكن
بینښه
_MapDosError (
    دننه_2 ULONG WinError
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
			 (WinError <= ERROR_INFLOOP_دننه_RELOC_CHAIN))
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
بینښه
Map_Nt_حالت(
    دننه_2 NTSTATUS حالت
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
    دننه_2 PCSTR Name,
    دننه_2 BOOLEAN Readonly,
    بهر_2 PHANDLE Handle,
    بهر_2 PBOOLEAN OpenedReadonly OPTIONAL
   )
{
	UNICODE_STRING UnicodeString;
	ANSI_STRING    AnsiString;
	WCHAR بفر_2[512];
	NTSTATUS حالت;
	OBJECT_ځانګړتیاوې Object_ځانګړتیاوې;
	IO_حالت_بلاک Io_حالت_بلاک;

	//
	// Make Unicode name from input string
	//

	UnicodeString.بفر_2 = &بفر_2[0];
	UnicodeString.اوږدتیا_2 = 0;
	UnicodeString.اعظمي_اوږدتیا = حجم(بفر_2); // in bytes!!!

	RtlInitAnsiString(&AnsiString, Name);

	حالت = Rtl_Ansi_String_منزل_Unicode_String(&UnicodeString, &AnsiString, FALSE);

	إذا(!NT_SUCCESS(حالت))
	{
		أرجع حالت; // Unmappable character?
	}

	//
	// Initialize object
	//

	Initialize_Object_ځانګړتیاوې(&Object_ځانګړتیاوې,
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
						SYNCHRONIZE | FILE_READ_معلومات | (Readonly ? 0 : FILE_WRITE_معلومات),
						&Object_ځانګړتیاوې,
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
							SYNCHRONIZE | FILE_READ_معلومات | (Readonly ? 0 : FILE_WRITE_معلومات),
							&Object_ځانګړتیاوې,
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
							SYNCHRONIZE | FILE_READ_معلومات,
							&Object_ځانګړتیاوې,
							&Io_حالت_بلاک,
							FILE_SHARE_WRITE | FILE_SHARE_READ,
							FILE_SYNCHRONOUS_IO_NONALERT);
		}
	}



	//
	//بشپړ
	//

	أرجع حالت;
}


ساكن
NTSTATUS
_OpenDriveLetter(
    دننه_2 CHAR Letter,
    دننه_2 BOOLEAN ReadOnly,
    بهر_2 PHANDLE Handle,
    بهر_2 PBOOLEAN OpenedReadonly OPTIONAL
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
		دننه_2 HANDLE Handle
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
قلف_Drive(
		دننه_2 HANDLE Handle
		)
{
	IO_حالت_بلاک Io_حالت_بلاک;
	أرجع NtFsControlFile(Handle, 0, 0, 0, &Io_حالت_بلاک, FSCTL_قلف_VOLUME, 0, 0, 0, 0);
}


//
// unlock drive
//

ساكن
__inline
NTSTATUS
_UnlockDrive(
	دننه_2 HANDLE Handle
	)
{
	IO_حالت_بلاک Io_حالت_بلاک;
	أرجع NtFsControlFile(Handle, 0, 0, 0, &Io_حالت_بلاک, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0);
}

ساكن
__inline
NTSTATUS
_DismountDrive(
	دننه_2 HANDLE Handle
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
	دننه_2 HANDLE Handle
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
		دننه_2 HANDLE Handle
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
    دننه_2 PCSTR Device,
    دننه_2 PSTR Normalized_Device_Name_بفر
   )
{
	صحيح برخه_کول_شمېره = -1;
	UCHAR Disk_شمېره;
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

		Disk_شمېره = (UCHAR)(*(Device + 2) - 'a');

		إذا(*(Device + 3) != '\0')
		{
			برخه_کول_شمېره = (*(Device + 3) - '0');
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

		Disk_شمېره = (UCHAR)(*(Device + 2) - '0');

	}
	وإلا
	{
		//
		// invalid prefix
		//

		أرجع عدم;
	}



	//
	//مختاړی
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


	p = Normalized_Device_Name_بفر + مزي_اوږدوالی(Normalized_Device_Name_بفر) - 1;
	*p = (CHAR)(*p + Disk_شمېره);


	//
	// Partition nr.
	//

	إذا(برخه_کول_شمېره >= 0)
	{
		strcat(Normalized_Device_Name_بفر, "\\Partition0");

		p = Normalized_Device_Name_بفر + مزي_اوږدوالی(Normalized_Device_Name_بفر) - 1;
		*p = (CHAR)(*p + برخه_کول_شمېره);
	}


	أرجع Normalized_Device_Name_بفر;
}




ساكن
VOID
Get_Device_اندازه(
    دننه_2 HANDLE h,
    بهر_2 بینښه __int64 *Fs_اندازه
   )
{
	برخه_کول_INFORMATION pi;
	DISK_GEOMETRY gi;
	NTSTATUS حالت;
	IO_حالت_بلاک Io_حالت_بلاک;

	//
	// Zero it
	//

	*Fs_اندازه = 0;

	//
	// Call driver
	//

	Rtl_Zero_حافظه(&pi, حجم(برخه_کول_INFORMATION));

	حالت = NtDeviceIoControlFile(
		h, عدم, عدم, عدم, &Io_حالت_بلاک, IOCTL_DISK_GET_برخه_کول_INFO,
		&pi, حجم(برخه_کول_INFORMATION),
		&pi, حجم(برخه_کول_INFORMATION));


	إذا(NT_SUCCESS(حالت))
	{
		*Fs_اندازه = pi.برخه_کول_اوږدتیا.QuadPart;
	}
	وإلا إذا(حالت_INVALID_DEVICE_REQUEST == حالت)
	{
		//
		// No partitions: get device info.
		//

		Rtl_Zero_حافظه(&gi, حجم(DISK_GEOMETRY));

		حالت = NtDeviceIoControlFile(
				h, عدم, عدم, عدم, &Io_حالت_بلاک, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, حجم(DISK_GEOMETRY),
				&gi, حجم(DISK_GEOMETRY));


		إذا(NT_SUCCESS(حالت))
		{
			*Fs_اندازه =
				gi.بایټونه_فی_Sector *
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
    دننه_2 PCSTR Name,
    دننه_2 BOOLEAN ReadOnly,
    بهر_2 PHANDLE Handle,
    بهر_2 PBOOLEAN OpenedReadonly OPTIONAL,
    بهر_2 بینښه *Errno OPTIONAL
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
    دننه_2 HANDLE Handle,
    دننه_2 LARGE_INTEGER دننه_واټن_2,
    دننه_2 ULONG بایټونه_2,
    دننه_2 بهر_2 PCHAR بفر_2,
    دننه_2 BOOLEAN Read,
    بهر_2 بینښه* Errno
   )
{
	IO_حالت_بلاک Io_حالت_بلاک;
	NTSTATUS حالت;

	//
	// Should be aligned
	//

	ASSERT(0 == (بایټونه_2 % 512));
	ASSERT(0 == (دننه_واټن_2.ټیټ_Part % 512));


	//
	// perform io
	//

	إذا(Read)
	{
		حالت = NtReadFile(Handle, عدم, عدم, عدم,
			&Io_حالت_بلاک, بفر_2, بایټونه_2, &دننه_واټن_2, عدم);
	}
	وإلا
	{
		حالت = NtWriteFile(Handle, عدم, عدم, عدم,
			&Io_حالت_بلاک, بفر_2, بایټونه_2, &دننه_واټن_2, عدم);
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
    دننه_2 HANDLE Handle,
    دننه_2 LARGE_INTEGER دننه_واټن_2,
    دننه_2 ULONG بایټونه_2,
    بهر_2 ثابت CHAR* بفر_2,
    بهر_2 بینښه* Errno
   )
{
	أرجع بلاک_Io(Handle, دننه_واټن_2, بایټونه_2, (PCHAR)بفر_2, FALSE, Errno);
}

__inline
BOOLEAN
_RawRead(
    دننه_2 HANDLE Handle,
    دننه_2 LARGE_INTEGER دننه_واټن_2,
    دننه_2 ULONG بایټونه_2,
    دننه_2 PCHAR بفر_2,
    بهر_2 بینښه* Errno
   )
{
	أرجع بلاک_Io(Handle, دننه_واټن_2, بایټونه_2, بفر_2, TRUE, Errno);
}



__inline
BOOLEAN
سېټ_Part_Type(
    دننه_2 HANDLE Handle,
    دننه_2 UCHAR Type
   )
{
	IO_حالت_بلاک Io_حالت_بلاک;
	أرجع حالت_SUCCESS == NtDeviceIoControlFile(
												   Handle, عدم, عدم, عدم, &Io_حالت_بلاک, IOCTL_DISK_سېټ_برخه_کول_INFO,
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

	*retblocks = (blk_t)(بینښه __int64)(Fs_اندازه / blocksize);
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
	PNT_شخصي_معلومات Nt_معلومات = عدم;
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

		io = (io_channel) حافظه_ورکړه(حجم(بنية struct_io_channel));

		إذا (عدم == io)
		{
			Errno = ENOMEM;
			__leave;
		}

		Rtl_Zero_حافظه(io, حجم(بنية struct_io_channel));
		io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

		Nt_معلومات = (PNT_شخصي_معلومات)حافظه_ورکړه(حجم(NT_شخصي_معلومات_2));

		إذا (عدم == Nt_معلومات)
		{
			Errno = ENOMEM;
			__leave;
		}


		io->manager = nt_io_manager();
		io->name = حافظه_ورکړه(مزي_اوږدوالی(name) + 1);
		إذا (عدم == io->name)
		{
			Errno = ENOMEM;
			__leave;
		}

		strcpy(io->name, name);
		io->شخصي_معلومات = Nt_معلومات;
		io->بلاک_اندازه = 1024;
		io->read_error = 0;
		io->write_error = 0;
		io->refcount = 1;

		//
		// Initialize data
		//

		Rtl_Zero_حافظه(Nt_معلومات, حجم(NT_شخصي_معلومات_2));

		Nt_معلومات->magic = EXT2_ET_MAGIC_NT_IO_CHANNEL;
		Nt_معلومات->بفر_بلاک_شمېره = 0xffffffff;
		Nt_معلومات->بفر_اندازه = 1024;
		Nt_معلومات->بفر_2 = حافظه_ورکړه(Nt_معلومات->بفر_اندازه);

		إذا (عدم == Nt_معلومات->بفر_2)
		{
			Errno = ENOMEM;
			__leave;
		}

		//
		// Open it
		//

		إذا(!_Ext2OpenDevice(name, (BOOLEAN)!BooleanFlagOn(flags, EXT2_FLAG_RW), &Nt_معلومات->Handle, &Nt_معلومات->OpenedReadonly, &Errno))
		{
			__leave;
		}


		//
		// get size
		//

		Get_Device_اندازه(Nt_معلومات->Handle, &Fs_اندازه);
		strcpy(knowndevice, name);


		//
		// Lock/dismount
		//

		إذا(!NT_SUCCESS(قلف_Drive(Nt_معلومات->Handle)) /*|| !NT_SUCCESS(_DismountDrive(NtData->Handle))*/)
		{
			Nt_معلومات->OpenedReadonly = TRUE;
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

			إذا (عدم != Nt_معلومات)
			{
				إذا(عدم != Nt_معلومات->Handle)
				{
					_UnlockDrive(Nt_معلومات->Handle);
					_CloseDisk(Nt_معلومات->Handle);
				}

				حرر(Nt_معلومات->بفر_2);
				حرر(Nt_معلومات);
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
	PNT_شخصي_معلومات Nt_معلومات = عدم;

	إذا(عدم == channel)
	{
		أرجع 0;
	}

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_معلومات = (PNT_شخصي_معلومات) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(Nt_معلومات, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	إذا (--channel->refcount > 0)
	{
		أرجع 0;
	}

	حرر(channel->name);
	حرر(channel);

	إذا (عدم != Nt_معلومات)
	{
		إذا(عدم != Nt_معلومات->Handle)
		{
			_DismountDrive(Nt_معلومات->Handle);
			_UnlockDrive(Nt_معلومات->Handle);
			_CloseDisk(Nt_معلومات->Handle);
		}

		حرر(Nt_معلومات->بفر_2);
		حرر(Nt_معلومات);
	}

	أرجع 0;
}



//
// set block size
//

ساكن
errcode_t
nt_سېټ_blksize(io_channel channel, صحيح blksize)
{
	PNT_شخصي_معلومات Nt_معلومات = عدم;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_معلومات = (PNT_شخصي_معلومات) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(Nt_معلومات, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	إذا (channel->بلاک_اندازه != blksize)
	{
		channel->بلاک_اندازه = blksize;

		حرر(Nt_معلومات->بفر_2);
		Nt_معلومات->بفر_بلاک_شمېره = 0xffffffff;
		Nt_معلومات->بفر_اندازه = channel->بلاک_اندازه;
		ASSERT(0 == (Nt_معلومات->بفر_اندازه % 512));

		Nt_معلومات->بفر_2 = حافظه_ورکړه(Nt_معلومات->بفر_اندازه);

		إذا (عدم == Nt_معلومات->بفر_2)
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
nt_read_blk(io_channel channel, بینښه اوږد بلاک,
			       صحيح شمېر, فراغ *buf)
{
	PVOID بفر_منزل_Read;
	ULONG اندازه_منزل_Read;
	ULONG اندازه_2;
	LARGE_INTEGER دننه_واټن_2;
	PNT_شخصي_معلومات Nt_معلومات = عدم;
	بینښه Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_معلومات = (PNT_شخصي_معلومات) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(Nt_معلومات, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	//
	// If it's in the cache, use it!
	//

	إذا ((1 == شمېر) &&
		(بلاک == Nt_معلومات->بفر_بلاک_شمېره) &&
		(Nt_معلومات->بفر_بلاک_شمېره != 0xffffffff))
	{
		حافظه_نقل(buf, Nt_معلومات->بفر_2, channel->بلاک_اندازه);
		أرجع 0;
	}

	اندازه_2 = (شمېر < 0) ? (ULONG)(-شمېر) : (ULONG)(شمېر * channel->بلاک_اندازه);

	دننه_واټن_2.QuadPart = بلاک * channel->بلاک_اندازه;

	//
	// If not fit to the block
	//

	إذا(اندازه_2 <= Nt_معلومات->بفر_اندازه)
	{
		//
		// Update the cache
		//

		Nt_معلومات->بفر_بلاک_شمېره = بلاک;
		بفر_منزل_Read = Nt_معلومات->بفر_2;
		اندازه_منزل_Read = Nt_معلومات->بفر_اندازه;
	}
	وإلا
	{
		اندازه_منزل_Read = اندازه_2;
		بفر_منزل_Read = buf;
		ASSERT(0 == (اندازه_منزل_Read % channel->بلاک_اندازه));
	}

	إذا(!_RawRead(Nt_معلومات->Handle, دننه_واټن_2, اندازه_منزل_Read, بفر_منزل_Read, &Errno))
	{

		إذا (channel->read_error)
		{
			أرجع (channel->read_error)(channel, بلاک, شمېر, buf,
					       اندازه_2, 0, Errno);
		}
		وإلا
		{
			أرجع Errno;
		}
	}


	إذا(بفر_منزل_Read != buf)
	{
		ASSERT(اندازه_2 <= اندازه_منزل_Read);
		حافظه_نقل(buf, بفر_منزل_Read, اندازه_2);
	}

	أرجع 0;
}


//
// write block
//

ساكن
errcode_t
nt_write_blk(io_channel channel, بینښه اوږد بلاک,
				صحيح شمېر, ثابت فراغ *buf)
{
	ULONG اندازه_منزل_Write;
	LARGE_INTEGER دننه_واټن_2;
	PNT_شخصي_معلومات Nt_معلومات = عدم;
	بینښه Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_معلومات = (PNT_شخصي_معلومات) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(Nt_معلومات, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	إذا(Nt_معلومات->OpenedReadonly)
	{
		أرجع EACCES;
	}

	إذا (شمېر == 1)
	{
		اندازه_منزل_Write = channel->بلاک_اندازه;
	}
	وإلا
	{
		Nt_معلومات->بفر_بلاک_شمېره = 0xffffffff;

		إذا (شمېر < 0)
		{
			اندازه_منزل_Write = (ULONG)(-شمېر);
		}
		وإلا
		{
			اندازه_منزل_Write = (ULONG)(شمېر * channel->بلاک_اندازه);
		}
	}


	ASSERT(0 == (اندازه_منزل_Write % 512));
	دننه_واټن_2.QuadPart = بلاک * channel->بلاک_اندازه;

	إذا(!_RawWrite(Nt_معلومات->Handle, دننه_واټن_2, اندازه_منزل_Write, buf, &Errno))
	{
		إذا (channel->write_error)
		{
			أرجع (channel->write_error)(channel, بلاک, شمېر, buf,
						اندازه_منزل_Write, 0, Errno);
		}
		وإلا
		{
			أرجع Errno;
		}
	}


	//
	// Stash a copy.
	//

	إذا(اندازه_منزل_Write >= Nt_معلومات->بفر_اندازه)
	{
		Nt_معلومات->بفر_بلاک_شمېره = بلاک;
		حافظه_نقل(Nt_معلومات->بفر_2, buf, Nt_معلومات->بفر_اندازه);
	}

	Nt_معلومات->Written = TRUE;

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
	PNT_شخصي_معلومات Nt_معلومات = عدم;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_معلومات = (PNT_شخصي_معلومات) channel->شخصي_معلومات;
	EXT2_CHECK_MAGIC(Nt_معلومات, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	إذا(Nt_معلومات->OpenedReadonly)
	{
		أرجع 0; // EACCESS;
	}


	//
	// Flush file buffers.
	//

	_FlushDrive(Nt_معلومات->Handle);


	//
	// Test and correct partition type.
	//

	إذا(Nt_معلومات->Written)
	{
		سېټ_Part_Type(Nt_معلومات->Handle, 0x83);
	}

	أرجع 0;
}


