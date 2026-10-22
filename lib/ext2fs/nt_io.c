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

#جيڪڏهن_وصف_ٿيل HAVE_CONFIG_H
#ضمن "config.h"
#جيڪڏهن_پورو


//
// I need some warnings to disable...
//


#هدايت warning(disable:4514) // unreferenced inline function has been removed
#هدايت warning(ڌڪو,4)

#هدايت warning(disable:4201) // nonstandard extension used : nameless struct/union)
#هدايت warning(disable:4214) // nonstandard extension used : bit field types other than int
#هدايت warning(disable:4115) // named type definition in parentheses

#ضمن <ntddk.h>
#ضمن <ntdddisk.h>
#ضمن <ntstatus.h>

#هدايت warning(ڪڍو)


//
// Some native APIs.
//

NTSYSAPI
ULONG
NTAPI
Rtl_Nt_حالت_منزل_Dos_Error(
    اندريون_2 NTSTATUS حالت
   );

NTSYSAPI
NTSTATUS
NTAPI
NtClose(
    اندريون_2 HANDLE Handle
   );


NTSYSAPI
NTSTATUS
NTAPI
NtOpenFile(
    ٻاهريون_2 PHANDLE FileHandle,
    اندريون_2 ACCESS_نقاب DesiredAccess,
    اندريون_2 POBJECT_خاصيتون Object_خاصيتون,
    ٻاهريون_2 PIO_حالت_بلاڪ Io_حالت_بلاڪ,
    اندريون_2 ULONG ShareAccess,
    اندريون_2 ULONG OpenOptions
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFlushBuffersFile(
    اندريون_2 HANDLE FileHandle,
    ٻاهريون_2 PIO_حالت_بلاڪ Io_حالت_بلاڪ
   );


NTSYSAPI
NTSTATUS
NTAPI
NtReadFile(
    اندريون_2 HANDLE FileHandle,
    اندريون_2 HANDLE Event OPTIONAL,
    اندريون_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    اندريون_2 PVOID ApcContext OPTIONAL,
    ٻاهريون_2 PIO_حالت_بلاڪ Io_حالت_بلاڪ,
    ٻاهريون_2 PVOID بفر_2,
    اندريون_2 ULONG ڊيگهائي_2,
    اندريون_2 PLARGE_INTEGER Byte_وٿي OPTIONAL,
    اندريون_2 PULONG ڪنجي_2 OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtWriteFile(
    اندريون_2 HANDLE FileHandle,
    اندريون_2 HANDLE Event OPTIONAL,
    اندريون_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    اندريون_2 PVOID ApcContext OPTIONAL,
    ٻاهريون_2 PIO_حالت_بلاڪ Io_حالت_بلاڪ,
    اندريون_2 PVOID بفر_2,
    اندريون_2 ULONG ڊيگهائي_2,
    اندريون_2 PLARGE_INTEGER Byte_وٿي OPTIONAL,
    اندريون_2 PULONG ڪنجي_2 OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtDeviceIoControlFile(
    اندريون_2 HANDLE FileHandle,
    اندريون_2 HANDLE Event OPTIONAL,
    اندريون_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    اندريون_2 PVOID ApcContext OPTIONAL,
    ٻاهريون_2 PIO_حالت_بلاڪ Io_حالت_بلاڪ,
    اندريون_2 ULONG Io_Control_ڪوڊ,
    اندريون_2 PVOID داخلا_بفر OPTIONAL,
    اندريون_2 ULONG داخلا_بفر_ڊيگهائي,
    ٻاهريون_2 PVOID Output_بفر OPTIONAL,
    اندريون_2 ULONG Output_بفر_ڊيگهائي
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFsControlFile(
    اندريون_2 HANDLE FileHandle,
    اندريون_2 HANDLE Event OPTIONAL,
    اندريون_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    اندريون_2 PVOID ApcContext OPTIONAL,
    ٻاهريون_2 PIO_حالت_بلاڪ Io_حالت_بلاڪ,
    اندريون_2 ULONG Io_Control_ڪوڊ,
    اندريون_2 PVOID داخلا_بفر OPTIONAL,
    اندريون_2 ULONG داخلا_بفر_ڊيگهائي,
    ٻاهريون_2 PVOID Output_بفر OPTIONAL,
    اندريون_2 ULONG Output_بفر_ڊيگهائي
    );


NTSYSAPI
NTSTATUS
NTAPI
NtDelayExecution(
    اندريون_2 BOOLEAN Alertable,
    اندريون_2 PLARGE_INTEGER Interval
    );


#وصف_ڪريو FSCTL_تالو_VOLUME               CTL_ڪوڊ(FILE_DEVICE_FILE_SYSTEM, 6, طريقو_BUFFERED, FILE_ANY_ACCESS)
#وصف_ڪريو FSCTL_UNLOCK_VOLUME             CTL_ڪوڊ(FILE_DEVICE_FILE_SYSTEM, 7, طريقو_BUFFERED, FILE_ANY_ACCESS)
#وصف_ڪريو FSCTL_DISMOUNT_VOLUME           CTL_ڪوڊ(FILE_DEVICE_FILE_SYSTEM, 8, طريقو_BUFFERED, FILE_ANY_ACCESS)
#وصف_ڪريو FSCTL_IS_VOLUME_MOUNTED         CTL_ڪوڊ(FILE_DEVICE_FILE_SYSTEM,10, طريقو_BUFFERED, FILE_ANY_ACCESS)


//
// useful macros
//

#وصف_ڪريو BooleanFlagOn(Flags,SingleFlag) ((BOOLEAN)((((Flags) & (SingleFlag)) != 0)))


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


#وصف_ڪريو EXT2_CHECK_MAGIC(بنية, ڪوڊ) \
	  إذا ((بنية)->magic != (ڪوڊ)) أرجع (ڪوڊ)

#وصف_ڪريو EXT2_ET_MAGIC_NT_IO_CHANNEL  0x10ed


//
// Private data block
//

تعريفنوع بنية NT_ذاتي_معلومات {
	صحيح	   magic;
	HANDLE Handle;
	صحيح	   Flags;
	PCHAR  بفر_2;
	__u32  بفر_بلاڪ_عدد;
	ULONG  بفر_سائيز;
	BOOLEAN OpenedReadonly;
	BOOLEAN Written;
}NT_ذاتي_معلومات_2, *PNT_ذاتي_معلومات;



//
// Standard interface prototypes
//

ساكن errcode_t nt_open(ثابت حرف *name, صحيح flags, io_channel *channel);
ساكن errcode_t nt_close(io_channel channel);
ساكن errcode_t nt_سيٽ_blksize(io_channel channel, صحيح blksize);
ساكن errcode_t nt_read_blk(io_channel channel, بنانشان ڊگهو بلاڪ,
			       صحيح ڳڻپ, فراغ *معلومات);
ساكن errcode_t nt_write_blk(io_channel channel, بنانشان ڊگهو بلاڪ,
				صحيح ڳڻپ, ثابت فراغ *معلومات);
ساكن errcode_t nt_flush(io_channel channel);

ساكن بنية struct_io_manager struct_nt_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "NT I/O Manager",
	.open		= nt_open,
	.close		= nt_close,
	.سيٽ_blksize	= nt_سيٽ_blksize,
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
}ERROR_داخلا;

ساكن ERROR_داخلا Error_جدول[] = {
        {  ERROR_INVALID_FUNCTION,       EINVAL    },
        {  ERROR_FILE_NOT_مليو,         ENOENT    },
        {  ERROR_رستو_NOT_مليو,         ENOENT    },
        {  ERROR_TOO_MANY_OPEN_FILES,    EMFILE    },
        {  ERROR_ACCESS_DENIED,          EACCES    },
        {  ERROR_INVALID_HANDLE,         EBADF     },
        {  ERROR_ARENA_TRASHED,          ENOMEM    },
        {  ERROR_NOT_ENOUGH_يادگيري,      ENOMEM    },
        {  ERROR_INVALID_بلاڪ,          ENOMEM    },
        {  ERROR_BAD_ENVIRONMENT,        E2BIG     },
        {  ERROR_BAD_FORMAT,             ENOEXEC   },
        {  ERROR_INVALID_ACCESS,         EINVAL    },
        {  ERROR_INVALID_معلومات,           EINVAL    },
        {  ERROR_INVALID_DRIVE,          ENOENT    },
        {  ERROR_CURRENT_DIRECTORY,      EACCES    },
        {  ERROR_NOT_SAME_DEVICE,        EXDEV     },
        {  ERROR_NO_وڌيڪ_FILES,          ENOENT    },
        {  ERROR_تالو_VIOLATION,         EACCES    },
        {  ERROR_BAD_NETPATH,            ENOENT    },
        {  ERROR_ڄار_ACCESS_DENIED,  EACCES    },
        {  ERROR_BAD_NET_NAME,           ENOENT    },
        {  ERROR_FILE_EXISTS,            EEXIST    },
        {  ERROR_CANNOT_MAKE,            EACCES    },
        {  ERROR_FAIL_I24,               EACCES    },
        {  ERROR_INVALID_PARAMETER,      EINVAL    },
        {  ERROR_NO_PROC_SLOTS,          EAGAIN    },
        {  ERROR_DRIVE_LOCKED,           EACCES    },
        {  ERROR_BROKEN_PIPE,            EPIPE     },
        {  ERROR_DISK_ڀريل,              ENOSPC    },
        {  ERROR_INVALID_هدف_HANDLE,  EBADF     },
        {  ERROR_INVALID_HANDLE,         EINVAL    },
        {  ERROR_WAIT_NO_CHILDREN,       ECHILD    },
        {  ERROR_ٻار_NOT_COMPLETE,     ECHILD    },
        {  ERROR_سڌو_ACCESS_HANDLE,   EBADF     },
        {  ERROR_NEGATIVE_SEEK,          EINVAL    },
        {  ERROR_SEEK_ON_DEVICE,         EACCES    },
        {  ERROR_DIR_NOT_خالي_جايون,          ENOTEMPTY },
        {  ERROR_NOT_LOCKED,             EACCES    },
        {  ERROR_BAD_PATHNAME,           ENOENT    },
        {  ERROR_MAX_THRDS_REACHED,      EAGAIN    },
        {  ERROR_تالو_FAILED,            EACCES    },
        {  ERROR_ALREADY_EXISTS,         EEXIST    },
        {  ERROR_FILENAME_EXCED_RANGE,   ENOENT    },
        {  ERROR_NESTING_NOT_ALLOWED,    EAGAIN    },
        {  ERROR_NOT_ENOUGH_QUOTA,       ENOMEM    }
};




ساكن
بنانشان
_MapDosError (
    اندريون_2 ULONG WinError
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
			 (WinError <= ERROR_INFLOOP_اندريون_RELOC_CHAIN))
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
بنانشان
Map_Nt_حالت(
    اندريون_2 NTSTATUS حالت
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
    اندريون_2 PCSTR Name,
    اندريون_2 BOOLEAN Readonly,
    ٻاهريون_2 PHANDLE Handle,
    ٻاهريون_2 PBOOLEAN OpenedReadonly OPTIONAL
   )
{
	UNICODE_STRING UnicodeString;
	ANSI_STRING    AnsiString;
	WCHAR بفر_2[512];
	NTSTATUS حالت;
	OBJECT_خاصيتون Object_خاصيتون;
	IO_حالت_بلاڪ Io_حالت_بلاڪ;

	//
	// Make Unicode name from input string
	//

	UnicodeString.بفر_2 = &بفر_2[0];
	UnicodeString.ڊيگهائي_2 = 0;
	UnicodeString.وڌ_کان_وڌ_ڊيگهائي = حجم(بفر_2); // in bytes!!!

	RtlInitAnsiString(&AnsiString, Name);

	حالت = Rtl_Ansi_String_منزل_Unicode_String(&UnicodeString, &AnsiString, FALSE);

	إذا(!NT_SUCCESS(حالت))
	{
		أرجع حالت; // Unmappable character?
	}

	//
	// Initialize object
	//

	Initialize_Object_خاصيتون(&Object_خاصيتون,
							   &UnicodeString,
							   OBJ_CASE_INSENSITIVE,
							   عدم,
							   عدم );

	//
	// Try to open it in initial mode
	//

	إذا(دليل_PRESENT(OpenedReadonly))
	{
		*OpenedReadonly = Readonly;
	}


	حالت = NtOpenFile(Handle,
						SYNCHRONIZE | FILE_READ_معلومات | (Readonly ? 0 : FILE_WRITE_معلومات),
						&Object_خاصيتون,
						&Io_حالت_بلاڪ,
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
							&Object_خاصيتون,
							&Io_حالت_بلاڪ,
							FILE_SHARE_WRITE | FILE_SHARE_READ,
							FILE_SYNCHRONOUS_IO_NONALERT);

		//
		// Try to satisfy mode
		//

		إذا((حالت_ACCESS_DENIED == حالت) && !Readonly)
		{
			إذا(دليل_PRESENT(OpenedReadonly))
			{
				*OpenedReadonly = TRUE;
			}

			حالت = NtOpenFile(Handle,
							SYNCHRONIZE | FILE_READ_معلومات,
							&Object_خاصيتون,
							&Io_حالت_بلاڪ,
							FILE_SHARE_WRITE | FILE_SHARE_READ,
							FILE_SYNCHRONOUS_IO_NONALERT);
		}
	}



	//
	//پورو
	//

	أرجع حالت;
}


ساكن
NTSTATUS
_OpenDriveLetter(
    اندريون_2 CHAR Letter,
    اندريون_2 BOOLEAN ReadOnly,
    ٻاهريون_2 PHANDLE Handle,
    ٻاهريون_2 PBOOLEAN OpenedReadonly OPTIONAL
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
		اندريون_2 HANDLE Handle
		)
{
	IO_حالت_بلاڪ Io_حالت_بلاڪ;
	أرجع NtFlushBuffersFile(Handle, &Io_حالت_بلاڪ);
}


//
// lock drive
//

ساكن
__inline
NTSTATUS
تالو_Drive(
		اندريون_2 HANDLE Handle
		)
{
	IO_حالت_بلاڪ Io_حالت_بلاڪ;
	أرجع NtFsControlFile(Handle, 0, 0, 0, &Io_حالت_بلاڪ, FSCTL_تالو_VOLUME, 0, 0, 0, 0);
}


//
// unlock drive
//

ساكن
__inline
NTSTATUS
_UnlockDrive(
	اندريون_2 HANDLE Handle
	)
{
	IO_حالت_بلاڪ Io_حالت_بلاڪ;
	أرجع NtFsControlFile(Handle, 0, 0, 0, &Io_حالت_بلاڪ, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0);
}

ساكن
__inline
NTSTATUS
_DismountDrive(
	اندريون_2 HANDLE Handle
	)
{
	IO_حالت_بلاڪ Io_حالت_بلاڪ;
	أرجع NtFsControlFile(Handle, 0, 0, 0, &Io_حالت_بلاڪ, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0);
}


//
// is mounted
//

ساكن
__inline
BOOLEAN
_IsMounted(
	اندريون_2 HANDLE Handle
	)
{
	IO_حالت_بلاڪ Io_حالت_بلاڪ;
	NTSTATUS حالت;
	حالت = NtFsControlFile(Handle, 0, 0, 0, &Io_حالت_بلاڪ, FSCTL_IS_VOLUME_MOUNTED, 0, 0, 0, 0);
	أرجع (BOOLEAN)(حالت_SUCCESS == حالت);
}


ساكن
__inline
NTSTATUS
_CloseDisk(
		اندريون_2 HANDLE Handle
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
    اندريون_2 PCSTR Device,
    اندريون_2 PSTR Normalized_Device_Name_بفر
   )
{
	صحيح حصا_ڪريو_عدد = -1;
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
			حصا_ڪريو_عدد = (*(Device + 3) - '0');
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
	//اڳياڙي
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


	p = Normalized_Device_Name_بفر + لڙي_ڊيگهه(Normalized_Device_Name_بفر) - 1;
	*p = (CHAR)(*p + Disk_عدد);


	//
	// Partition nr.
	//

	إذا(حصا_ڪريو_عدد >= 0)
	{
		strcat(Normalized_Device_Name_بفر, "\\Partition0");

		p = Normalized_Device_Name_بفر + لڙي_ڊيگهه(Normalized_Device_Name_بفر) - 1;
		*p = (CHAR)(*p + حصا_ڪريو_عدد);
	}


	أرجع Normalized_Device_Name_بفر;
}




ساكن
VOID
Get_Device_سائيز(
    اندريون_2 HANDLE h,
    ٻاهريون_2 بنانشان __int64 *Fs_سائيز
   )
{
	حصا_ڪريو_INFORMATION pi;
	DISK_GEOMETRY gi;
	NTSTATUS حالت;
	IO_حالت_بلاڪ Io_حالت_بلاڪ;

	//
	// Zero it
	//

	*Fs_سائيز = 0;

	//
	// Call driver
	//

	Rtl_Zero_يادگيري(&pi, حجم(حصا_ڪريو_INFORMATION));

	حالت = NtDeviceIoControlFile(
		h, عدم, عدم, عدم, &Io_حالت_بلاڪ, IOCTL_DISK_GET_حصا_ڪريو_INFO,
		&pi, حجم(حصا_ڪريو_INFORMATION),
		&pi, حجم(حصا_ڪريو_INFORMATION));


	إذا(NT_SUCCESS(حالت))
	{
		*Fs_سائيز = pi.حصا_ڪريو_ڊيگهائي.QuadPart;
	}
	وإلا إذا(حالت_INVALID_DEVICE_REQUEST == حالت)
	{
		//
		// No partitions: get device info.
		//

		Rtl_Zero_يادگيري(&gi, حجم(DISK_GEOMETRY));

		حالت = NtDeviceIoControlFile(
				h, عدم, عدم, عدم, &Io_حالت_بلاڪ, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, حجم(DISK_GEOMETRY),
				&gi, حجم(DISK_GEOMETRY));


		إذا(NT_SUCCESS(حالت))
		{
			*Fs_سائيز =
				gi.بائيٽون_في_Sector *
				gi.Sectors_في_Track *
				gi.Tracks_في_Cylinder *
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
    اندريون_2 PCSTR Name,
    اندريون_2 BOOLEAN ReadOnly,
    ٻاهريون_2 PHANDLE Handle,
    ٻاهريون_2 PBOOLEAN OpenedReadonly OPTIONAL,
    ٻاهريون_2 بنانشان *Errno OPTIONAL
   )
{
	CHAR NormalizedDeviceName[512];
	NTSTATUS حالت;

	إذا(عدم == Name)
	{
		//
		// Set not found
		//

		إذا(دليل_PRESENT(Errno))
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

			إذا(دليل_PRESENT(Errno))
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
		إذا(دليل_PRESENT(Errno))
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
بلاڪ_Io(
    اندريون_2 HANDLE Handle,
    اندريون_2 LARGE_INTEGER وٿي_2,
    اندريون_2 ULONG بائيٽون_2,
    اندريون_2 ٻاهريون_2 PCHAR بفر_2,
    اندريون_2 BOOLEAN Read,
    ٻاهريون_2 بنانشان* Errno
   )
{
	IO_حالت_بلاڪ Io_حالت_بلاڪ;
	NTSTATUS حالت;

	//
	// Should be aligned
	//

	ASSERT(0 == (بائيٽون_2 % 512));
	ASSERT(0 == (وٿي_2.هيٺيون_Part % 512));


	//
	// perform io
	//

	إذا(Read)
	{
		حالت = NtReadFile(Handle, عدم, عدم, عدم,
			&Io_حالت_بلاڪ, بفر_2, بائيٽون_2, &وٿي_2, عدم);
	}
	وإلا
	{
		حالت = NtWriteFile(Handle, عدم, عدم, عدم,
			&Io_حالت_بلاڪ, بفر_2, بائيٽون_2, &وٿي_2, عدم);
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
    اندريون_2 HANDLE Handle,
    اندريون_2 LARGE_INTEGER وٿي_2,
    اندريون_2 ULONG بائيٽون_2,
    ٻاهريون_2 ثابت CHAR* بفر_2,
    ٻاهريون_2 بنانشان* Errno
   )
{
	أرجع بلاڪ_Io(Handle, وٿي_2, بائيٽون_2, (PCHAR)بفر_2, FALSE, Errno);
}

__inline
BOOLEAN
_RawRead(
    اندريون_2 HANDLE Handle,
    اندريون_2 LARGE_INTEGER وٿي_2,
    اندريون_2 ULONG بائيٽون_2,
    اندريون_2 PCHAR بفر_2,
    ٻاهريون_2 بنانشان* Errno
   )
{
	أرجع بلاڪ_Io(Handle, وٿي_2, بائيٽون_2, بفر_2, TRUE, Errno);
}



__inline
BOOLEAN
سيٽ_Part_Type(
    اندريون_2 HANDLE Handle,
    اندريون_2 UCHAR Type
   )
{
	IO_حالت_بلاڪ Io_حالت_بلاڪ;
	أرجع حالت_SUCCESS == NtDeviceIoControlFile(
												   Handle, عدم, عدم, عدم, &Io_حالت_بلاڪ, IOCTL_DISK_سيٽ_حصا_ڪريو_INFO,
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

ساكن __int64 Fs_سائيز = 0;
ساكن حرف knowndevice[1024] = "";


errcode_t
ext2fs_get_device_سائيز(ثابت حرف *file, صحيح blocksize,
				 blk_t *retblocks)
{
	HANDLE h;
	BOOLEAN Readonly;

	إذا((0 == Fs_سائيز) || (0 != strcmp(knowndevice, file)))
	{

		إذا(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, عدم))
		{
			أرجع 0;
		}


		__try{

			//
			// Get size
			//

			Get_Device_سائيز(h, &Fs_سائيز);
			strcpy(knowndevice, file);
		}
		__finally{
			_CloseDisk(h);
		}

	}

	*retblocks = (blk_t)(بنانشان __int64)(Fs_سائيز / blocksize);
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
	PNT_ذاتي_معلومات Nt_معلومات = عدم;
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

		io = (io_channel) يادگيري_ڏيو(حجم(بنية struct_io_channel));

		إذا (عدم == io)
		{
			Errno = ENOMEM;
			__leave;
		}

		Rtl_Zero_يادگيري(io, حجم(بنية struct_io_channel));
		io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

		Nt_معلومات = (PNT_ذاتي_معلومات)يادگيري_ڏيو(حجم(NT_ذاتي_معلومات_2));

		إذا (عدم == Nt_معلومات)
		{
			Errno = ENOMEM;
			__leave;
		}


		io->manager = nt_io_manager();
		io->name = يادگيري_ڏيو(لڙي_ڊيگهه(name) + 1);
		إذا (عدم == io->name)
		{
			Errno = ENOMEM;
			__leave;
		}

		strcpy(io->name, name);
		io->ذاتي_معلومات = Nt_معلومات;
		io->بلاڪ_سائيز = 1024;
		io->read_error = 0;
		io->write_error = 0;
		io->refcount = 1;

		//
		// Initialize data
		//

		Rtl_Zero_يادگيري(Nt_معلومات, حجم(NT_ذاتي_معلومات_2));

		Nt_معلومات->magic = EXT2_ET_MAGIC_NT_IO_CHANNEL;
		Nt_معلومات->بفر_بلاڪ_عدد = 0xffffffff;
		Nt_معلومات->بفر_سائيز = 1024;
		Nt_معلومات->بفر_2 = يادگيري_ڏيو(Nt_معلومات->بفر_سائيز);

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

		Get_Device_سائيز(Nt_معلومات->Handle, &Fs_سائيز);
		strcpy(knowndevice, name);


		//
		// Lock/dismount
		//

		إذا(!NT_SUCCESS(تالو_Drive(Nt_معلومات->Handle)) /*|| !NT_SUCCESS(_DismountDrive(NtData->Handle))*/)
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
	PNT_ذاتي_معلومات Nt_معلومات = عدم;

	إذا(عدم == channel)
	{
		أرجع 0;
	}

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_معلومات = (PNT_ذاتي_معلومات) channel->ذاتي_معلومات;
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
nt_سيٽ_blksize(io_channel channel, صحيح blksize)
{
	PNT_ذاتي_معلومات Nt_معلومات = عدم;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_معلومات = (PNT_ذاتي_معلومات) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(Nt_معلومات, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	إذا (channel->بلاڪ_سائيز != blksize)
	{
		channel->بلاڪ_سائيز = blksize;

		حرر(Nt_معلومات->بفر_2);
		Nt_معلومات->بفر_بلاڪ_عدد = 0xffffffff;
		Nt_معلومات->بفر_سائيز = channel->بلاڪ_سائيز;
		ASSERT(0 == (Nt_معلومات->بفر_سائيز % 512));

		Nt_معلومات->بفر_2 = يادگيري_ڏيو(Nt_معلومات->بفر_سائيز);

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
nt_read_blk(io_channel channel, بنانشان ڊگهو بلاڪ,
			       صحيح ڳڻپ, فراغ *buf)
{
	PVOID بفر_منزل_Read;
	ULONG سائيز_منزل_Read;
	ULONG سائيز_2;
	LARGE_INTEGER وٿي_2;
	PNT_ذاتي_معلومات Nt_معلومات = عدم;
	بنانشان Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_معلومات = (PNT_ذاتي_معلومات) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(Nt_معلومات, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	//
	// If it's in the cache, use it!
	//

	إذا ((1 == ڳڻپ) &&
		(بلاڪ == Nt_معلومات->بفر_بلاڪ_عدد) &&
		(Nt_معلومات->بفر_بلاڪ_عدد != 0xffffffff))
	{
		يادگيري_نقل(buf, Nt_معلومات->بفر_2, channel->بلاڪ_سائيز);
		أرجع 0;
	}

	سائيز_2 = (ڳڻپ < 0) ? (ULONG)(-ڳڻپ) : (ULONG)(ڳڻپ * channel->بلاڪ_سائيز);

	وٿي_2.QuadPart = بلاڪ * channel->بلاڪ_سائيز;

	//
	// If not fit to the block
	//

	إذا(سائيز_2 <= Nt_معلومات->بفر_سائيز)
	{
		//
		// Update the cache
		//

		Nt_معلومات->بفر_بلاڪ_عدد = بلاڪ;
		بفر_منزل_Read = Nt_معلومات->بفر_2;
		سائيز_منزل_Read = Nt_معلومات->بفر_سائيز;
	}
	وإلا
	{
		سائيز_منزل_Read = سائيز_2;
		بفر_منزل_Read = buf;
		ASSERT(0 == (سائيز_منزل_Read % channel->بلاڪ_سائيز));
	}

	إذا(!_RawRead(Nt_معلومات->Handle, وٿي_2, سائيز_منزل_Read, بفر_منزل_Read, &Errno))
	{

		إذا (channel->read_error)
		{
			أرجع (channel->read_error)(channel, بلاڪ, ڳڻپ, buf,
					       سائيز_2, 0, Errno);
		}
		وإلا
		{
			أرجع Errno;
		}
	}


	إذا(بفر_منزل_Read != buf)
	{
		ASSERT(سائيز_2 <= سائيز_منزل_Read);
		يادگيري_نقل(buf, بفر_منزل_Read, سائيز_2);
	}

	أرجع 0;
}


//
// write block
//

ساكن
errcode_t
nt_write_blk(io_channel channel, بنانشان ڊگهو بلاڪ,
				صحيح ڳڻپ, ثابت فراغ *buf)
{
	ULONG سائيز_منزل_Write;
	LARGE_INTEGER وٿي_2;
	PNT_ذاتي_معلومات Nt_معلومات = عدم;
	بنانشان Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_معلومات = (PNT_ذاتي_معلومات) channel->ذاتي_معلومات;
	EXT2_CHECK_MAGIC(Nt_معلومات, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	إذا(Nt_معلومات->OpenedReadonly)
	{
		أرجع EACCES;
	}

	إذا (ڳڻپ == 1)
	{
		سائيز_منزل_Write = channel->بلاڪ_سائيز;
	}
	وإلا
	{
		Nt_معلومات->بفر_بلاڪ_عدد = 0xffffffff;

		إذا (ڳڻپ < 0)
		{
			سائيز_منزل_Write = (ULONG)(-ڳڻپ);
		}
		وإلا
		{
			سائيز_منزل_Write = (ULONG)(ڳڻپ * channel->بلاڪ_سائيز);
		}
	}


	ASSERT(0 == (سائيز_منزل_Write % 512));
	وٿي_2.QuadPart = بلاڪ * channel->بلاڪ_سائيز;

	إذا(!_RawWrite(Nt_معلومات->Handle, وٿي_2, سائيز_منزل_Write, buf, &Errno))
	{
		إذا (channel->write_error)
		{
			أرجع (channel->write_error)(channel, بلاڪ, ڳڻپ, buf,
						سائيز_منزل_Write, 0, Errno);
		}
		وإلا
		{
			أرجع Errno;
		}
	}


	//
	// Stash a copy.
	//

	إذا(سائيز_منزل_Write >= Nt_معلومات->بفر_سائيز)
	{
		Nt_معلومات->بفر_بلاڪ_عدد = بلاڪ;
		يادگيري_نقل(Nt_معلومات->بفر_2, buf, Nt_معلومات->بفر_سائيز);
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
	PNT_ذاتي_معلومات Nt_معلومات = عدم;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_معلومات = (PNT_ذاتي_معلومات) channel->ذاتي_معلومات;
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
		سيٽ_Part_Type(Nt_معلومات->Handle, 0x83);
	}

	أرجع 0;
}


