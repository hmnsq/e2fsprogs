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

#اگر_تعریف_شدہ HAVE_CONFIG_H
#شامل "config.h"
#اگر_ختم


//
// I need some warnings to disable...
//


#ہدایت warning(disable:4514) // unreferenced inline function has been removed
#ہدایت warning(ڈالو,4)

#ہدایت warning(disable:4201) // nonstandard extension used : nameless struct/union)
#ہدایت warning(disable:4214) // nonstandard extension used : bit field types other than int
#ہدایت warning(disable:4115) // named type definition in parentheses

#شامل <ntddk.h>
#شامل <ntdddisk.h>
#شامل <ntstatus.h>

#ہدایت warning(نکالو)


//
// Some native APIs.
//

NTSYSAPI
ULONG
NTAPI
Rtl_Nt_حالت_مقصد_Dos_Error(
    اندر_2 NTSTATUS حالت_2
   );

NTSYSAPI
NTSTATUS
NTAPI
NtClose(
    اندر_2 HANDLE Handle
   );


NTSYSAPI
NTSTATUS
NTAPI
NtOpenFile(
    برآمد_2 PHANDLE FileHandle,
    اندر_2 ACCESS_نقاب DesiredAccess,
    اندر_2 POBJECT_اوصاف Object_اوصاف,
    برآمد_2 PIO_حالت_بلاک Io_حالت_بلاک,
    اندر_2 ULONG ShareAccess,
    اندر_2 ULONG OpenOptions
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFlushBuffersFile(
    اندر_2 HANDLE FileHandle,
    برآمد_2 PIO_حالت_بلاک Io_حالت_بلاک
   );


NTSYSAPI
NTSTATUS
NTAPI
NtReadFile(
    اندر_2 HANDLE FileHandle,
    اندر_2 HANDLE Event OPTIONAL,
    اندر_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    اندر_2 PVOID ApcContext OPTIONAL,
    برآمد_2 PIO_حالت_بلاک Io_حالت_بلاک,
    برآمد_2 PVOID بفر_2,
    اندر_2 ULONG لمبائی_2,
    اندر_2 PLARGE_INTEGER Byte_انحراف OPTIONAL,
    اندر_2 PULONG کلید_2 OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtWriteFile(
    اندر_2 HANDLE FileHandle,
    اندر_2 HANDLE Event OPTIONAL,
    اندر_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    اندر_2 PVOID ApcContext OPTIONAL,
    برآمد_2 PIO_حالت_بلاک Io_حالت_بلاک,
    اندر_2 PVOID بفر_2,
    اندر_2 ULONG لمبائی_2,
    اندر_2 PLARGE_INTEGER Byte_انحراف OPTIONAL,
    اندر_2 PULONG کلید_2 OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtDeviceIoControlFile(
    اندر_2 HANDLE FileHandle,
    اندر_2 HANDLE Event OPTIONAL,
    اندر_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    اندر_2 PVOID ApcContext OPTIONAL,
    برآمد_2 PIO_حالت_بلاک Io_حالت_بلاک,
    اندر_2 ULONG Io_Control_رمز,
    اندر_2 PVOID داخلہ_بفر OPTIONAL,
    اندر_2 ULONG داخلہ_بفر_لمبائی,
    برآمد_2 PVOID Output_بفر OPTIONAL,
    اندر_2 ULONG Output_بفر_لمبائی
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFsControlFile(
    اندر_2 HANDLE FileHandle,
    اندر_2 HANDLE Event OPTIONAL,
    اندر_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    اندر_2 PVOID ApcContext OPTIONAL,
    برآمد_2 PIO_حالت_بلاک Io_حالت_بلاک,
    اندر_2 ULONG Io_Control_رمز,
    اندر_2 PVOID داخلہ_بفر OPTIONAL,
    اندر_2 ULONG داخلہ_بفر_لمبائی,
    برآمد_2 PVOID Output_بفر OPTIONAL,
    اندر_2 ULONG Output_بفر_لمبائی
    );


NTSYSAPI
NTSTATUS
NTAPI
NtDelayExecution(
    اندر_2 BOOLEAN Alertable,
    اندر_2 PLARGE_INTEGER Interval
    );


#تعریف_کرو FSCTL_تالا_VOLUME               CTL_رمز(FILE_DEVICE_FILE_SYSTEM, 6, طریقہ_BUFFERED, FILE_ANY_ACCESS)
#تعریف_کرو FSCTL_UNLOCK_VOLUME             CTL_رمز(FILE_DEVICE_FILE_SYSTEM, 7, طریقہ_BUFFERED, FILE_ANY_ACCESS)
#تعریف_کرو FSCTL_DISMOUNT_VOLUME           CTL_رمز(FILE_DEVICE_FILE_SYSTEM, 8, طریقہ_BUFFERED, FILE_ANY_ACCESS)
#تعریف_کرو FSCTL_IS_VOLUME_MOUNTED         CTL_رمز(FILE_DEVICE_FILE_SYSTEM,10, طریقہ_BUFFERED, FILE_ANY_ACCESS)


//
// useful macros
//

#تعریف_کرو BooleanFlagOn(Flags,SingleFlag) ((BOOLEAN)((((Flags) & (SingleFlag)) != 0)))


//
// Include Win32 error codes.
//

#شامل <winerror.h>

//
// standard stuff
//

#شامل <assert.h>
#شامل <stdio.h>
#شامل <string.h>
#شامل <stdlib.h>
#شامل <malloc.h>

#شامل <linux/types.h>
#شامل "ext2_fs.h"
#شامل <errno.h>

#شامل "et/com_err.h"
#شامل "ext2fs/ext2fs.h"
#شامل "ext2fs/ext2_err.h"




//
// For checking structure magic numbers...
//


#تعریف_کرو EXT2_CHECK_MAGIC(ساخت, رمز) \
	  اگر ((ساخت)->magic != (رمز)) واپس (رمز)

#تعریف_کرو EXT2_ET_MAGIC_NT_IO_CHANNEL  0x10ed


//
// Private data block
//

قسمتعریف ساخت NT_نجی_ڈیٹا {
	صحیح	   magic;
	HANDLE Handle;
	صحیح	   Flags;
	PCHAR  بفر_2;
	__u32  بفر_بلاک_نمبر;
	ULONG  بفر_سائز;
	BOOLEAN OpenedReadonly;
	BOOLEAN Written;
}NT_نجی_ڈیٹا_2, *PNT_نجی_ڈیٹا;



//
// Standard interface prototypes
//

ساکن errcode_t nt_open(ثابت حرف *name, صحیح flags, io_channel *channel);
ساکن errcode_t nt_close(io_channel channel);
ساکن errcode_t nt_سیٹ_blksize(io_channel channel, صحیح blksize);
ساکن errcode_t nt_read_blk(io_channel channel, غیرعلامتی طویل بلاک,
			       صحیح گنتی, خالی *ڈیٹا);
ساکن errcode_t nt_write_blk(io_channel channel, غیرعلامتی طویل بلاک,
				صحیح گنتی, ثابت خالی *ڈیٹا);
ساکن errcode_t nt_flush(io_channel channel);

ساکن ساخت struct_io_manager struct_nt_manager = {
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
	واپس &struct_nt_manager;
}





//
// This is a code to convert Win32 errors to unix errno
//

قسمتعریف ساخت {
	ULONG WinError;
	صحیح errnocode;
}ERROR_اندراج;

ساکن ERROR_اندراج Error_جدول[] = {
        {  ERROR_INVALID_FUNCTION,       EINVAL    },
        {  ERROR_FILE_NOT_ملا,         ENOENT    },
        {  ERROR_راستہ_NOT_ملا,         ENOENT    },
        {  ERROR_TOO_MANY_OPEN_FILES,    EMFILE    },
        {  ERROR_ACCESS_DENIED,          EACCES    },
        {  ERROR_INVALID_HANDLE,         EBADF     },
        {  ERROR_ARENA_TRASHED,          ENOMEM    },
        {  ERROR_NOT_ENOUGH_حافظہ,      ENOMEM    },
        {  ERROR_INVALID_بلاک,          ENOMEM    },
        {  ERROR_BAD_ENVIRONMENT,        E2BIG     },
        {  ERROR_BAD_FORMAT,             ENOEXEC   },
        {  ERROR_INVALID_ACCESS,         EINVAL    },
        {  ERROR_INVALID_ڈیٹا,           EINVAL    },
        {  ERROR_INVALID_DRIVE,          ENOENT    },
        {  ERROR_CURRENT_DIRECTORY,      EACCES    },
        {  ERROR_NOT_SAME_DEVICE,        EXDEV     },
        {  ERROR_NO_مزید_FILES,          ENOENT    },
        {  ERROR_تالا_VIOLATION,         EACCES    },
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
        {  ERROR_DISK_بھرا,              ENOSPC    },
        {  ERROR_INVALID_ہدف_HANDLE,  EBADF     },
        {  ERROR_INVALID_HANDLE,         EINVAL    },
        {  ERROR_WAIT_NO_CHILDREN,       ECHILD    },
        {  ERROR_بچہ_NOT_COMPLETE,     ECHILD    },
        {  ERROR_براہ_راست_ACCESS_HANDLE,   EBADF     },
        {  ERROR_NEGATIVE_SEEK,          EINVAL    },
        {  ERROR_SEEK_ON_DEVICE,         EACCES    },
        {  ERROR_DIR_NOT_خالی_خانے,          ENOTEMPTY },
        {  ERROR_NOT_LOCKED,             EACCES    },
        {  ERROR_BAD_PATHNAME,           ENOENT    },
        {  ERROR_MAX_THRDS_REACHED,      EAGAIN    },
        {  ERROR_تالا_FAILED,            EACCES    },
        {  ERROR_ALREADY_EXISTS,         EEXIST    },
        {  ERROR_FILENAME_EXCED_RANGE,   ENOENT    },
        {  ERROR_NESTING_NOT_ALLOWED,    EAGAIN    },
        {  ERROR_NOT_ENOUGH_QUOTA,       ENOMEM    }
};




ساکن
غیرعلامتی
_MapDosError (
    اندر_2 ULONG WinError
   )
{
	صحیح i;

	//
	// Lookup
	//

	برائے (i = 0; i < (حجم(Error_جدول)/حجم(Error_جدول[0])); ++i)
	{
		اگر (WinError == Error_جدول[i].WinError)
		{
			واپس Error_جدول[i].errnocode;
		}
	}

	//
	// not in table. Check ranges
	//

	اگر ((WinError >= ERROR_WRITE_PROTECT) &&
		(WinError <= ERROR_SHARING_بفر_EXCEEDED))
	{
		واپس EACCES;
	}
	ورنہ اگر ((WinError >= ERROR_INVALID_STARTING_CODESEG) &&
			 (WinError <= ERROR_INFLOOP_اندر_RELOC_CHAIN))
	{
		واپس ENOEXEC;
	}
	ورنہ
	{
		واپس EINVAL;
	}
}







//
// Function to map NT status to dos error.
//

ساکن
__inline
غیرعلامتی
Map_Nt_حالت(
    اندر_2 NTSTATUS حالت_2
   )
{
	واپس _MapDosError(Rtl_Nt_حالت_مقصد_Dos_Error(حالت_2));
}





//
// Helper functions to make things easier
//

ساکن
NTSTATUS
_OpenNtName(
    اندر_2 PCSTR Name,
    اندر_2 BOOLEAN Readonly,
    برآمد_2 PHANDLE Handle,
    برآمد_2 PBOOLEAN OpenedReadonly OPTIONAL
   )
{
	UNICODE_STRING UnicodeString;
	ANSI_STRING    AnsiString;
	WCHAR بفر_2[512];
	NTSTATUS حالت_2;
	OBJECT_اوصاف Object_اوصاف;
	IO_حالت_بلاک Io_حالت_بلاک;

	//
	// Make Unicode name from input string
	//

	UnicodeString.بفر_2 = &بفر_2[0];
	UnicodeString.لمبائی_2 = 0;
	UnicodeString.زیادہ_سے_زیادہ_لمبائی = حجم(بفر_2); // in bytes!!!

	RtlInitAnsiString(&AnsiString, Name);

	حالت_2 = Rtl_Ansi_String_مقصد_Unicode_String(&UnicodeString, &AnsiString, FALSE);

	اگر(!NT_SUCCESS(حالت_2))
	{
		واپس حالت_2; // Unmappable character?
	}

	//
	// Initialize object
	//

	Initialize_Object_اوصاف(&Object_اوصاف,
							   &UnicodeString,
							   OBJ_CASE_INSENSITIVE,
							   تہی,
							   تہی );

	//
	// Try to open it in initial mode
	//

	اگر(دلیل_PRESENT(OpenedReadonly))
	{
		*OpenedReadonly = Readonly;
	}


	حالت_2 = NtOpenFile(Handle,
						SYNCHRONIZE | FILE_READ_ڈیٹا | (Readonly ? 0 : FILE_WRITE_ڈیٹا),
						&Object_اوصاف,
						&Io_حالت_بلاک,
						FILE_SHARE_WRITE | FILE_SHARE_READ,
						FILE_SYNCHRONOUS_IO_NONALERT);

	اگر(!NT_SUCCESS(حالت_2))
	{
		//
		// Maybe was just mounted? wait 0.5 sec and retry.
		//

		LARGE_INTEGER Interval;
		Interval.QuadPart = -5000000; // 0.5 sec. from now

		NtDelayExecution(FALSE, &Interval);

		حالت_2 = NtOpenFile(Handle,
							SYNCHRONIZE | FILE_READ_ڈیٹا | (Readonly ? 0 : FILE_WRITE_ڈیٹا),
							&Object_اوصاف,
							&Io_حالت_بلاک,
							FILE_SHARE_WRITE | FILE_SHARE_READ,
							FILE_SYNCHRONOUS_IO_NONALERT);

		//
		// Try to satisfy mode
		//

		اگر((حالت_ACCESS_DENIED == حالت_2) && !Readonly)
		{
			اگر(دلیل_PRESENT(OpenedReadonly))
			{
				*OpenedReadonly = TRUE;
			}

			حالت_2 = NtOpenFile(Handle,
							SYNCHRONIZE | FILE_READ_ڈیٹا,
							&Object_اوصاف,
							&Io_حالت_بلاک,
							FILE_SHARE_WRITE | FILE_SHARE_READ,
							FILE_SYNCHRONOUS_IO_NONALERT);
		}
	}



	//
	//مکمل
	//

	واپس حالت_2;
}


ساکن
NTSTATUS
_OpenDriveLetter(
    اندر_2 CHAR Letter,
    اندر_2 BOOLEAN ReadOnly,
    برآمد_2 PHANDLE Handle,
    برآمد_2 PBOOLEAN OpenedReadonly OPTIONAL
   )
{
	CHAR بفر_2[100];

	sprintf(بفر_2, "\\DosDevices\\%c:", Letter);

	واپس _OpenNtName(بفر_2, ReadOnly, Handle, OpenedReadonly);
}


//
// Flush device
//

ساکن
__inline
NTSTATUS
_FlushDrive(
		اندر_2 HANDLE Handle
		)
{
	IO_حالت_بلاک Io_حالت_بلاک;
	واپس NtFlushBuffersFile(Handle, &Io_حالت_بلاک);
}


//
// lock drive
//

ساکن
__inline
NTSTATUS
تالا_Drive(
		اندر_2 HANDLE Handle
		)
{
	IO_حالت_بلاک Io_حالت_بلاک;
	واپس NtFsControlFile(Handle, 0, 0, 0, &Io_حالت_بلاک, FSCTL_تالا_VOLUME, 0, 0, 0, 0);
}


//
// unlock drive
//

ساکن
__inline
NTSTATUS
_UnlockDrive(
	اندر_2 HANDLE Handle
	)
{
	IO_حالت_بلاک Io_حالت_بلاک;
	واپس NtFsControlFile(Handle, 0, 0, 0, &Io_حالت_بلاک, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0);
}

ساکن
__inline
NTSTATUS
_DismountDrive(
	اندر_2 HANDLE Handle
	)
{
	IO_حالت_بلاک Io_حالت_بلاک;
	واپس NtFsControlFile(Handle, 0, 0, 0, &Io_حالت_بلاک, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0);
}


//
// is mounted
//

ساکن
__inline
BOOLEAN
_IsMounted(
	اندر_2 HANDLE Handle
	)
{
	IO_حالت_بلاک Io_حالت_بلاک;
	NTSTATUS حالت_2;
	حالت_2 = NtFsControlFile(Handle, 0, 0, 0, &Io_حالت_بلاک, FSCTL_IS_VOLUME_MOUNTED, 0, 0, 0, 0);
	واپس (BOOLEAN)(حالت_SUCCESS == حالت_2);
}


ساکن
__inline
NTSTATUS
_CloseDisk(
		اندر_2 HANDLE Handle
		)
{
	واپس NtClose(Handle);
}




//
// Make NT name from any recognized name
//

ساکن
PCSTR
_NormalizeDeviceName(
    اندر_2 PCSTR Device,
    اندر_2 PSTR Normalized_Device_Name_بفر
   )
{
	صحیح تقسیم_نمبر = -1;
	UCHAR Disk_نمبر;
	PSTR p;


	//
	// Do not try to parse NT name
	//

	اگر('\\' == *Device)
		واپس Device;



	//
	// Strip leading '/dev/' if any
	//

	اگر(('/' == *(Device)) &&
		('d' == *(Device + 1)) &&
		('e' == *(Device + 2)) &&
		('v' == *(Device + 3)) &&
		('/' == *(Device + 4)))
	{
		Device += 5;
	}

	اگر('\0' == *Device)
	{
		واپس تہی;
	}


	//
	// forms: hda[n], fd[n]
	//

	اگر('d' != *(Device + 1))
	{
		واپس تہی;
	}

	اگر('h' == *Device)
	{
		اگر((*(Device + 2) < 'a') || (*(Device + 2) > ('a' + 9)) ||
		   ((*(Device + 3) != '\0') &&
			((*(Device + 4) != '\0') ||
			 ((*(Device + 3) < '0') || (*(Device + 3) > '9'))
			)
		   )
		  )
		{
			واپس تہی;
		}

		Disk_نمبر = (UCHAR)(*(Device + 2) - 'a');

		اگر(*(Device + 3) != '\0')
		{
			تقسیم_نمبر = (*(Device + 3) - '0');
		}

	}
	ورنہ اگر('f' == *Device)
	{
		//
		// 3-d letter should be a digit.
		//

		اگر((*(Device + 3) != '\0') ||
		   (*(Device + 2) < '0') || (*(Device + 2) > '9'))
		{
			واپس تہی;
		}

		Disk_نمبر = (UCHAR)(*(Device + 2) - '0');

	}
	ورنہ
	{
		//
		// invalid prefix
		//

		واپس تہی;
	}



	//
	//سابقہ
	//

	strcpy(Normalized_Device_Name_بفر, "\\Device\\");

	//
	// Media name
	//

	انتخاب(*Device)
	{

	حالت 'f':
		strcat(Normalized_Device_Name_بفر, "Floppy0");
		توڑو;

	حالت 'h':
		strcat(Normalized_Device_Name_بفر, "Harddisk0");
		توڑو;
	}


	p = Normalized_Device_Name_بفر + لڑی_لمبائی(Normalized_Device_Name_بفر) - 1;
	*p = (CHAR)(*p + Disk_نمبر);


	//
	// Partition nr.
	//

	اگر(تقسیم_نمبر >= 0)
	{
		strcat(Normalized_Device_Name_بفر, "\\Partition0");

		p = Normalized_Device_Name_بفر + لڑی_لمبائی(Normalized_Device_Name_بفر) - 1;
		*p = (CHAR)(*p + تقسیم_نمبر);
	}


	واپس Normalized_Device_Name_بفر;
}




ساکن
VOID
Get_Device_سائز(
    اندر_2 HANDLE h,
    برآمد_2 غیرعلامتی __int64 *Fs_سائز
   )
{
	تقسیم_INFORMATION pi;
	DISK_GEOMETRY gi;
	NTSTATUS حالت_2;
	IO_حالت_بلاک Io_حالت_بلاک;

	//
	// Zero it
	//

	*Fs_سائز = 0;

	//
	// Call driver
	//

	Rtl_Zero_حافظہ(&pi, حجم(تقسیم_INFORMATION));

	حالت_2 = NtDeviceIoControlFile(
		h, تہی, تہی, تہی, &Io_حالت_بلاک, IOCTL_DISK_GET_تقسیم_INFO,
		&pi, حجم(تقسیم_INFORMATION),
		&pi, حجم(تقسیم_INFORMATION));


	اگر(NT_SUCCESS(حالت_2))
	{
		*Fs_سائز = pi.تقسیم_لمبائی.QuadPart;
	}
	ورنہ اگر(حالت_INVALID_DEVICE_REQUEST == حالت_2)
	{
		//
		// No partitions: get device info.
		//

		Rtl_Zero_حافظہ(&gi, حجم(DISK_GEOMETRY));

		حالت_2 = NtDeviceIoControlFile(
				h, تہی, تہی, تہی, &Io_حالت_بلاک, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, حجم(DISK_GEOMETRY),
				&gi, حجم(DISK_GEOMETRY));


		اگر(NT_SUCCESS(حالت_2))
		{
			*Fs_سائز =
				gi.بائٹیں_فی_Sector *
				gi.Sectors_فی_Track *
				gi.Tracks_فی_Cylinder *
				gi.Cylinders.QuadPart;
		}

	}
}



//
// Open device by name.
//

ساکن
BOOLEAN
_Ext2OpenDevice(
    اندر_2 PCSTR Name,
    اندر_2 BOOLEAN ReadOnly,
    برآمد_2 PHANDLE Handle,
    برآمد_2 PBOOLEAN OpenedReadonly OPTIONAL,
    برآمد_2 غیرعلامتی *Errno OPTIONAL
   )
{
	CHAR NormalizedDeviceName[512];
	NTSTATUS حالت_2;

	اگر(تہی == Name)
	{
		//
		// Set not found
		//

		اگر(دلیل_PRESENT(Errno))
			*Errno = ENOENT;

		واپس FALSE;
	}


	اگر((((*Name) | 0x20) >= 'a') && (((*Name) | 0x20) <= 'z') &&
		(':' == *(Name + 1)) && ('\0' == *(Name + 2)))
	{
		حالت_2 = _OpenDriveLetter(*Name, ReadOnly, Handle, OpenedReadonly);
	}
	ورنہ
	{
		//
		// Make name
		//

		Name = _NormalizeDeviceName(Name, NormalizedDeviceName);

		اگر(تہی == Name)
		{
			//
			// Set not found
			//

			اگر(دلیل_PRESENT(Errno))
				*Errno = ENOENT;

			واپس FALSE;
		}

		//
		// Try to open it
		//

		حالت_2 = _OpenNtName(Name, ReadOnly, Handle, OpenedReadonly);
	}


	اگر(!NT_SUCCESS(حالت_2))
	{
		اگر(دلیل_PRESENT(Errno))
			*Errno = Map_Nt_حالت(حالت_2);

		واپس FALSE;
	}

	واپس TRUE;
}


//
// Raw block io. Sets dos errno
//

ساکن
BOOLEAN
بلاک_Io(
    اندر_2 HANDLE Handle,
    اندر_2 LARGE_INTEGER انحراف_2,
    اندر_2 ULONG بائٹیں_2,
    اندر_2 برآمد_2 PCHAR بفر_2,
    اندر_2 BOOLEAN Read,
    برآمد_2 غیرعلامتی* Errno
   )
{
	IO_حالت_بلاک Io_حالت_بلاک;
	NTSTATUS حالت_2;

	//
	// Should be aligned
	//

	ASSERT(0 == (بائٹیں_2 % 512));
	ASSERT(0 == (انحراف_2.کم_Part % 512));


	//
	// perform io
	//

	اگر(Read)
	{
		حالت_2 = NtReadFile(Handle, تہی, تہی, تہی,
			&Io_حالت_بلاک, بفر_2, بائٹیں_2, &انحراف_2, تہی);
	}
	ورنہ
	{
		حالت_2 = NtWriteFile(Handle, تہی, تہی, تہی,
			&Io_حالت_بلاک, بفر_2, بائٹیں_2, &انحراف_2, تہی);
	}


	//
	// translate error
	//

	اگر(NT_SUCCESS(حالت_2))
	{
		*Errno = 0;
		واپس TRUE;
	}

	*Errno = Map_Nt_حالت(حالت_2);

	واپس FALSE;
}



__inline
BOOLEAN
_RawWrite(
    اندر_2 HANDLE Handle,
    اندر_2 LARGE_INTEGER انحراف_2,
    اندر_2 ULONG بائٹیں_2,
    برآمد_2 ثابت CHAR* بفر_2,
    برآمد_2 غیرعلامتی* Errno
   )
{
	واپس بلاک_Io(Handle, انحراف_2, بائٹیں_2, (PCHAR)بفر_2, FALSE, Errno);
}

__inline
BOOLEAN
_RawRead(
    اندر_2 HANDLE Handle,
    اندر_2 LARGE_INTEGER انحراف_2,
    اندر_2 ULONG بائٹیں_2,
    اندر_2 PCHAR بفر_2,
    برآمد_2 غیرعلامتی* Errno
   )
{
	واپس بلاک_Io(Handle, انحراف_2, بائٹیں_2, بفر_2, TRUE, Errno);
}



__inline
BOOLEAN
سیٹ_Part_Type(
    اندر_2 HANDLE Handle,
    اندر_2 UCHAR Type
   )
{
	IO_حالت_بلاک Io_حالت_بلاک;
	واپس حالت_SUCCESS == NtDeviceIoControlFile(
												   Handle, تہی, تہی, تہی, &Io_حالت_بلاک, IOCTL_DISK_سیٹ_تقسیم_INFO,
												   &Type, حجم(Type),
												   تہی, 0);
}



//--------------------- interface part

//
// Interface functions.
// Is_mounted is set to 1 if the device is mounted, 0 otherwise
//

errcode_t
ext2fs_check_if_mounted(ثابت حرف *file, صحیح *mount_flags)
{
	HANDLE h;
	BOOLEAN Readonly;

	*mount_flags = 0;

	اگر(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, تہی))
	{
		واپس 0;
	}


	__try{
		*mount_flags &= _IsMounted(h) ? EXT2_MF_MOUNTED : 0;
	}
	__finally{
		_CloseDisk(h);
	}

	واپس 0;
}



//
// Returns the number of blocks in a partition
//

ساکن __int64 Fs_سائز = 0;
ساکن حرف knowndevice[1024] = "";


errcode_t
ext2fs_get_device_سائز(ثابت حرف *file, صحیح blocksize,
				 blk_t *retblocks)
{
	HANDLE h;
	BOOLEAN Readonly;

	اگر((0 == Fs_سائز) || (0 != strcmp(knowndevice, file)))
	{

		اگر(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, تہی))
		{
			واپس 0;
		}


		__try{

			//
			// Get size
			//

			Get_Device_سائز(h, &Fs_سائز);
			strcpy(knowndevice, file);
		}
		__finally{
			_CloseDisk(h);
		}

	}

	*retblocks = (blk_t)(غیرعلامتی __int64)(Fs_سائز / blocksize);
	UNREFERENCED_PARAMETER(file);
	واپس 0;
}






//
// Table elements
//


ساکن
errcode_t
nt_open(ثابت حرف *name, صحیح flags, io_channel *channel)
{
	io_channel      io = تہی;
	PNT_نجی_ڈیٹا Nt_ڈیٹا = تہی;
	errcode_t Errno = 0;

	//
	// Check name
	//

	اگر (تہی == name)
	{
		واپس EXT2_ET_BAD_DEVICE_NAME;
	}

	__try{

		//
		// Allocate channel handle
		//

		io = (io_channel) حافظہ_دو(حجم(ساخت struct_io_channel));

		اگر (تہی == io)
		{
			Errno = ENOMEM;
			__leave;
		}

		Rtl_Zero_حافظہ(io, حجم(ساخت struct_io_channel));
		io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

		Nt_ڈیٹا = (PNT_نجی_ڈیٹا)حافظہ_دو(حجم(NT_نجی_ڈیٹا_2));

		اگر (تہی == Nt_ڈیٹا)
		{
			Errno = ENOMEM;
			__leave;
		}


		io->manager = nt_io_manager();
		io->name = حافظہ_دو(لڑی_لمبائی(name) + 1);
		اگر (تہی == io->name)
		{
			Errno = ENOMEM;
			__leave;
		}

		strcpy(io->name, name);
		io->نجی_ڈیٹا = Nt_ڈیٹا;
		io->بلاک_سائز = 1024;
		io->read_error = 0;
		io->write_error = 0;
		io->refcount = 1;

		//
		// Initialize data
		//

		Rtl_Zero_حافظہ(Nt_ڈیٹا, حجم(NT_نجی_ڈیٹا_2));

		Nt_ڈیٹا->magic = EXT2_ET_MAGIC_NT_IO_CHANNEL;
		Nt_ڈیٹا->بفر_بلاک_نمبر = 0xffffffff;
		Nt_ڈیٹا->بفر_سائز = 1024;
		Nt_ڈیٹا->بفر_2 = حافظہ_دو(Nt_ڈیٹا->بفر_سائز);

		اگر (تہی == Nt_ڈیٹا->بفر_2)
		{
			Errno = ENOMEM;
			__leave;
		}

		//
		// Open it
		//

		اگر(!_Ext2OpenDevice(name, (BOOLEAN)!BooleanFlagOn(flags, EXT2_FLAG_RW), &Nt_ڈیٹا->Handle, &Nt_ڈیٹا->OpenedReadonly, &Errno))
		{
			__leave;
		}


		//
		// get size
		//

		Get_Device_سائز(Nt_ڈیٹا->Handle, &Fs_سائز);
		strcpy(knowndevice, name);


		//
		// Lock/dismount
		//

		اگر(!NT_SUCCESS(تالا_Drive(Nt_ڈیٹا->Handle)) /*|| !NT_SUCCESS(_DismountDrive(NtData->Handle))*/)
		{
			Nt_ڈیٹا->OpenedReadonly = TRUE;
		}

		//
		// Done
		//

		*channel = io;


	}
	__finally{

		اگر(0 != Errno)
		{
			//
			// Cleanup
			//

			اگر (تہی != io)
			{
				آزاد(io->name);
				آزاد(io);
			}

			اگر (تہی != Nt_ڈیٹا)
			{
				اگر(تہی != Nt_ڈیٹا->Handle)
				{
					_UnlockDrive(Nt_ڈیٹا->Handle);
					_CloseDisk(Nt_ڈیٹا->Handle);
				}

				آزاد(Nt_ڈیٹا->بفر_2);
				آزاد(Nt_ڈیٹا);
			}
		}
	}

	واپس Errno;
}


//
// Close api
//

ساکن
errcode_t
nt_close(io_channel channel)
{
	PNT_نجی_ڈیٹا Nt_ڈیٹا = تہی;

	اگر(تہی == channel)
	{
		واپس 0;
	}

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_ڈیٹا = (PNT_نجی_ڈیٹا) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(Nt_ڈیٹا, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	اگر (--channel->refcount > 0)
	{
		واپس 0;
	}

	آزاد(channel->name);
	آزاد(channel);

	اگر (تہی != Nt_ڈیٹا)
	{
		اگر(تہی != Nt_ڈیٹا->Handle)
		{
			_DismountDrive(Nt_ڈیٹا->Handle);
			_UnlockDrive(Nt_ڈیٹا->Handle);
			_CloseDisk(Nt_ڈیٹا->Handle);
		}

		آزاد(Nt_ڈیٹا->بفر_2);
		آزاد(Nt_ڈیٹا);
	}

	واپس 0;
}



//
// set block size
//

ساکن
errcode_t
nt_سیٹ_blksize(io_channel channel, صحیح blksize)
{
	PNT_نجی_ڈیٹا Nt_ڈیٹا = تہی;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_ڈیٹا = (PNT_نجی_ڈیٹا) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(Nt_ڈیٹا, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	اگر (channel->بلاک_سائز != blksize)
	{
		channel->بلاک_سائز = blksize;

		آزاد(Nt_ڈیٹا->بفر_2);
		Nt_ڈیٹا->بفر_بلاک_نمبر = 0xffffffff;
		Nt_ڈیٹا->بفر_سائز = channel->بلاک_سائز;
		ASSERT(0 == (Nt_ڈیٹا->بفر_سائز % 512));

		Nt_ڈیٹا->بفر_2 = حافظہ_دو(Nt_ڈیٹا->بفر_سائز);

		اگر (تہی == Nt_ڈیٹا->بفر_2)
		{
			واپس ENOMEM;
		}

	}

	واپس 0;
}


//
// read block
//

ساکن
errcode_t
nt_read_blk(io_channel channel, غیرعلامتی طویل بلاک,
			       صحیح گنتی, خالی *buf)
{
	PVOID بفر_مقصد_Read;
	ULONG سائز_مقصد_Read;
	ULONG سائز_2;
	LARGE_INTEGER انحراف_2;
	PNT_نجی_ڈیٹا Nt_ڈیٹا = تہی;
	غیرعلامتی Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_ڈیٹا = (PNT_نجی_ڈیٹا) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(Nt_ڈیٹا, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	//
	// If it's in the cache, use it!
	//

	اگر ((1 == گنتی) &&
		(بلاک == Nt_ڈیٹا->بفر_بلاک_نمبر) &&
		(Nt_ڈیٹا->بفر_بلاک_نمبر != 0xffffffff))
	{
		حافظہ_نقل(buf, Nt_ڈیٹا->بفر_2, channel->بلاک_سائز);
		واپس 0;
	}

	سائز_2 = (گنتی < 0) ? (ULONG)(-گنتی) : (ULONG)(گنتی * channel->بلاک_سائز);

	انحراف_2.QuadPart = بلاک * channel->بلاک_سائز;

	//
	// If not fit to the block
	//

	اگر(سائز_2 <= Nt_ڈیٹا->بفر_سائز)
	{
		//
		// Update the cache
		//

		Nt_ڈیٹا->بفر_بلاک_نمبر = بلاک;
		بفر_مقصد_Read = Nt_ڈیٹا->بفر_2;
		سائز_مقصد_Read = Nt_ڈیٹا->بفر_سائز;
	}
	ورنہ
	{
		سائز_مقصد_Read = سائز_2;
		بفر_مقصد_Read = buf;
		ASSERT(0 == (سائز_مقصد_Read % channel->بلاک_سائز));
	}

	اگر(!_RawRead(Nt_ڈیٹا->Handle, انحراف_2, سائز_مقصد_Read, بفر_مقصد_Read, &Errno))
	{

		اگر (channel->read_error)
		{
			واپس (channel->read_error)(channel, بلاک, گنتی, buf,
					       سائز_2, 0, Errno);
		}
		ورنہ
		{
			واپس Errno;
		}
	}


	اگر(بفر_مقصد_Read != buf)
	{
		ASSERT(سائز_2 <= سائز_مقصد_Read);
		حافظہ_نقل(buf, بفر_مقصد_Read, سائز_2);
	}

	واپس 0;
}


//
// write block
//

ساکن
errcode_t
nt_write_blk(io_channel channel, غیرعلامتی طویل بلاک,
				صحیح گنتی, ثابت خالی *buf)
{
	ULONG سائز_مقصد_Write;
	LARGE_INTEGER انحراف_2;
	PNT_نجی_ڈیٹا Nt_ڈیٹا = تہی;
	غیرعلامتی Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_ڈیٹا = (PNT_نجی_ڈیٹا) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(Nt_ڈیٹا, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	اگر(Nt_ڈیٹا->OpenedReadonly)
	{
		واپس EACCES;
	}

	اگر (گنتی == 1)
	{
		سائز_مقصد_Write = channel->بلاک_سائز;
	}
	ورنہ
	{
		Nt_ڈیٹا->بفر_بلاک_نمبر = 0xffffffff;

		اگر (گنتی < 0)
		{
			سائز_مقصد_Write = (ULONG)(-گنتی);
		}
		ورنہ
		{
			سائز_مقصد_Write = (ULONG)(گنتی * channel->بلاک_سائز);
		}
	}


	ASSERT(0 == (سائز_مقصد_Write % 512));
	انحراف_2.QuadPart = بلاک * channel->بلاک_سائز;

	اگر(!_RawWrite(Nt_ڈیٹا->Handle, انحراف_2, سائز_مقصد_Write, buf, &Errno))
	{
		اگر (channel->write_error)
		{
			واپس (channel->write_error)(channel, بلاک, گنتی, buf,
						سائز_مقصد_Write, 0, Errno);
		}
		ورنہ
		{
			واپس Errno;
		}
	}


	//
	// Stash a copy.
	//

	اگر(سائز_مقصد_Write >= Nt_ڈیٹا->بفر_سائز)
	{
		Nt_ڈیٹا->بفر_بلاک_نمبر = بلاک;
		حافظہ_نقل(Nt_ڈیٹا->بفر_2, buf, Nt_ڈیٹا->بفر_سائز);
	}

	Nt_ڈیٹا->Written = TRUE;

	واپس 0;

}



//
// Flush data buffers to disk.  Since we are currently using a
// write-through cache, this is a no-op.
//

ساکن
errcode_t
nt_flush(io_channel channel)
{
	PNT_نجی_ڈیٹا Nt_ڈیٹا = تہی;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_ڈیٹا = (PNT_نجی_ڈیٹا) channel->نجی_ڈیٹا;
	EXT2_CHECK_MAGIC(Nt_ڈیٹا, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	اگر(Nt_ڈیٹا->OpenedReadonly)
	{
		واپس 0; // EACCESS;
	}


	//
	// Flush file buffers.
	//

	_FlushDrive(Nt_ڈیٹا->Handle);


	//
	// Test and correct partition type.
	//

	اگر(Nt_ڈیٹا->Written)
	{
		سیٹ_Part_Type(Nt_ڈیٹا->Handle, 0x83);
	}

	واپس 0;
}


