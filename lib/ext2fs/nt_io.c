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

#यदि_परिभाषितम् HAVE_CONFIG_H
#अन्तर्भाव "config.h"
#यदि_अन्तः


//
// I need some warnings to disable...
//


#निर्देशः warning(disable:4514) // unreferenced inline function has been removed
#निर्देशः warning(प्रक्षिप,4)

#निर्देशः warning(disable:4201) // nonstandard extension used : nameless struct/union)
#निर्देशः warning(disable:4214) // nonstandard extension used : bit field types other than int
#निर्देशः warning(disable:4115) // named type definition in parentheses

#अन्तर्भाव <ntddk.h>
#अन्तर्भाव <ntdddisk.h>
#अन्तर्भाव <ntstatus.h>

#निर्देशः warning(निष्कासय)


//
// Some native APIs.
//

NTSYSAPI
ULONG
NTAPI
Rtl_Nt_स्थितिः_गन्तव्यम्_Dos_Error(
    अन्तर्_2 NTSTATUS स्थितिः
   );

NTSYSAPI
NTSTATUS
NTAPI
NtClose(
    अन्तर्_2 HANDLE Handle
   );


NTSYSAPI
NTSTATUS
NTAPI
NtOpenFile(
    बहिर्गमनम्_2 PHANDLE FileHandle,
    अन्तर्_2 ACCESS_आवरणम् DesiredAccess,
    अन्तर्_2 POBJECT_गुणाः Object_गुणाः,
    बहिर्गमनम्_2 PIO_स्थितिः_खण्डम् Io_स्थितिः_खण्डम्,
    अन्तर्_2 ULONG ShareAccess,
    अन्तर्_2 ULONG OpenOptions
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFlushBuffersFile(
    अन्तर्_2 HANDLE FileHandle,
    बहिर्गमनम्_2 PIO_स्थितिः_खण्डम् Io_स्थितिः_खण्डम्
   );


NTSYSAPI
NTSTATUS
NTAPI
NtReadFile(
    अन्तर्_2 HANDLE FileHandle,
    अन्तर्_2 HANDLE Event OPTIONAL,
    अन्तर्_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    अन्तर्_2 PVOID ApcContext OPTIONAL,
    बहिर्गमनम्_2 PIO_स्थितिः_खण्डम् Io_स्थितिः_खण्डम्,
    बहिर्गमनम्_2 PVOID बफर_2,
    अन्तर्_2 ULONG दीर्घता_2,
    अन्तर्_2 PLARGE_INTEGER Byte_विस्थापनम् OPTIONAL,
    अन्तर्_2 PULONG कुञ्चिका_2 OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtWriteFile(
    अन्तर्_2 HANDLE FileHandle,
    अन्तर्_2 HANDLE Event OPTIONAL,
    अन्तर्_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    अन्तर्_2 PVOID ApcContext OPTIONAL,
    बहिर्गमनम्_2 PIO_स्थितिः_खण्डम् Io_स्थितिः_खण्डम्,
    अन्तर्_2 PVOID बफर_2,
    अन्तर्_2 ULONG दीर्घता_2,
    अन्तर्_2 PLARGE_INTEGER Byte_विस्थापनम् OPTIONAL,
    अन्तर्_2 PULONG कुञ्चिका_2 OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtDeviceIoControlFile(
    अन्तर्_2 HANDLE FileHandle,
    अन्तर्_2 HANDLE Event OPTIONAL,
    अन्तर्_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    अन्तर्_2 PVOID ApcContext OPTIONAL,
    बहिर्गमनम्_2 PIO_स्थितिः_खण्डम् Io_स्थितिः_खण्डम्,
    अन्तर्_2 ULONG Io_Control_संहिता,
    अन्तर्_2 PVOID निवेशः_बफर OPTIONAL,
    अन्तर्_2 ULONG निवेशः_बफर_दीर्घता,
    बहिर्गमनम्_2 PVOID Output_बफर OPTIONAL,
    अन्तर्_2 ULONG Output_बफर_दीर्घता
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFsControlFile(
    अन्तर्_2 HANDLE FileHandle,
    अन्तर्_2 HANDLE Event OPTIONAL,
    अन्तर्_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    अन्तर्_2 PVOID ApcContext OPTIONAL,
    बहिर्गमनम्_2 PIO_स्थितिः_खण्डम् Io_स्थितिः_खण्डम्,
    अन्तर्_2 ULONG Io_Control_संहिता,
    अन्तर्_2 PVOID निवेशः_बफर OPTIONAL,
    अन्तर्_2 ULONG निवेशः_बफर_दीर्घता,
    बहिर्गमनम्_2 PVOID Output_बफर OPTIONAL,
    अन्तर्_2 ULONG Output_बफर_दीर्घता
    );


NTSYSAPI
NTSTATUS
NTAPI
NtDelayExecution(
    अन्तर्_2 BOOLEAN Alertable,
    अन्तर्_2 PLARGE_INTEGER Interval
    );


#परिभाषय FSCTL_तालकम्_VOLUME               CTL_संहिता(FILE_DEVICE_FILE_SYSTEM, 6, विधानम्_BUFFERED, FILE_ANY_ACCESS)
#परिभाषय FSCTL_UNLOCK_VOLUME             CTL_संहिता(FILE_DEVICE_FILE_SYSTEM, 7, विधानम्_BUFFERED, FILE_ANY_ACCESS)
#परिभाषय FSCTL_DISMOUNT_VOLUME           CTL_संहिता(FILE_DEVICE_FILE_SYSTEM, 8, विधानम्_BUFFERED, FILE_ANY_ACCESS)
#परिभाषय FSCTL_IS_VOLUME_MOUNTED         CTL_संहिता(FILE_DEVICE_FILE_SYSTEM,10, विधानम्_BUFFERED, FILE_ANY_ACCESS)


//
// useful macros
//

#परिभाषय BooleanFlagOn(Flags,SingleFlag) ((BOOLEAN)((((Flags) & (SingleFlag)) != 0)))


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


#परिभाषय EXT2_CHECK_MAGIC(संरचना, संहिता) \
	  यदि ((संरचना)->magic != (संहिता)) प्रत्यागम (संहिता)

#परिभाषय EXT2_ET_MAGIC_NT_IO_CHANNEL  0x10ed


//
// Private data block
//

प्रकारनिर्देश संरचना NT_वैयक्तिकम्_दत्त {
	पूर्णाङ्क	   magic;
	HANDLE Handle;
	पूर्णाङ्क	   Flags;
	PCHAR  बफर_2;
	__u32  बफर_खण्डम्_सङ्ख्या;
	ULONG  बफर_आकार;
	BOOLEAN OpenedReadonly;
	BOOLEAN Written;
}NT_वैयक्तिकम्_दत्त_2, *PNT_वैयक्तिकम्_दत्त;



//
// Standard interface prototypes
//

स्थिर errcode_t nt_open(ध्रुव वर्ण *name, पूर्णाङ्क flags, io_channel *channel);
स्थिर errcode_t nt_close(io_channel channel);
स्थिर errcode_t nt_समुच्चयः_blksize(io_channel channel, पूर्णाङ्क blksize);
स्थिर errcode_t nt_read_blk(io_channel channel, अचिह्नित दीर्घ खण्डम्,
			       पूर्णाङ्क गणनम्, शून्य *दत्त);
स्थिर errcode_t nt_write_blk(io_channel channel, अचिह्नित दीर्घ खण्डम्,
				पूर्णाङ्क गणनम्, ध्रुव शून्य *दत्त);
स्थिर errcode_t nt_flush(io_channel channel);

स्थिर संरचना struct_io_manager struct_nt_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "NT I/O Manager",
	.उद्घाटय		= nt_open,
	.close		= nt_close,
	.समुच्चयः_blksize	= nt_समुच्चयः_blksize,
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
}ERROR_प्रविष्टिः;

स्थिर ERROR_प्रविष्टिः Error_सारणी[] = {
        {  ERROR_INVALID_FUNCTION,       EINVAL    },
        {  ERROR_FILE_NOT_प्राप्तम्,         ENOENT    },
        {  ERROR_मार्गः_NOT_प्राप्तम्,         ENOENT    },
        {  ERROR_TOO_MANY_OPEN_FILES,    EMFILE    },
        {  ERROR_ACCESS_DENIED,          EACCES    },
        {  ERROR_INVALID_HANDLE,         EBADF     },
        {  ERROR_ARENA_TRASHED,          ENOMEM    },
        {  ERROR_NOT_ENOUGH_स्मृतिः,      ENOMEM    },
        {  ERROR_INVALID_खण्डम्,          ENOMEM    },
        {  ERROR_BAD_ENVIRONMENT,        E2BIG     },
        {  ERROR_BAD_FORMAT,             ENOEXEC   },
        {  ERROR_INVALID_ACCESS,         EINVAL    },
        {  ERROR_INVALID_दत्त,           EINVAL    },
        {  ERROR_INVALID_DRIVE,          ENOENT    },
        {  ERROR_CURRENT_DIRECTORY,      EACCES    },
        {  ERROR_NOT_SAME_DEVICE,        EXDEV     },
        {  ERROR_NO_अधिकम्_FILES,          ENOENT    },
        {  ERROR_तालकम्_VIOLATION,         EACCES    },
        {  ERROR_BAD_NETPATH,            ENOENT    },
        {  ERROR_जालम्_ACCESS_DENIED,  EACCES    },
        {  ERROR_BAD_NET_NAME,           ENOENT    },
        {  ERROR_FILE_EXISTS,            EEXIST    },
        {  ERROR_CANNOT_MAKE,            EACCES    },
        {  ERROR_FAIL_I24,               EACCES    },
        {  ERROR_INVALID_PARAMETER,      EINVAL    },
        {  ERROR_NO_PROC_SLOTS,          EAGAIN    },
        {  ERROR_DRIVE_LOCKED,           EACCES    },
        {  ERROR_BROKEN_PIPE,            EPIPE     },
        {  ERROR_DISK_पूर्णम्,              ENOSPC    },
        {  ERROR_INVALID_TARGET_HANDLE,  EBADF     },
        {  ERROR_INVALID_HANDLE,         EINVAL    },
        {  ERROR_WAIT_NO_CHILDREN,       ECHILD    },
        {  ERROR_CHILD_NOT_COMPLETE,     ECHILD    },
        {  ERROR_प्रत्यक्षम्_ACCESS_HANDLE,   EBADF     },
        {  ERROR_NEGATIVE_SEEK,          EINVAL    },
        {  ERROR_SEEK_ON_DEVICE,         EACCES    },
        {  ERROR_DIR_NOT_रिक्तानि,          ENOTEMPTY },
        {  ERROR_NOT_LOCKED,             EACCES    },
        {  ERROR_BAD_PATHNAME,           ENOENT    },
        {  ERROR_MAX_THRDS_REACHED,      EAGAIN    },
        {  ERROR_तालकम्_FAILED,            EACCES    },
        {  ERROR_ALREADY_EXISTS,         EEXIST    },
        {  ERROR_FILENAME_EXCED_RANGE,   ENOENT    },
        {  ERROR_NESTING_NOT_ALLOWED,    EAGAIN    },
        {  ERROR_NOT_ENOUGH_QUOTA,       ENOMEM    }
};




स्थिर
अचिह्नित
_MapDosError (
    अन्तर्_2 ULONG WinError
   )
{
	पूर्णाङ्क i;

	//
	// Lookup
	//

	प्रति (i = 0; i < (परिमाण(Error_सारणी)/परिमाण(Error_सारणी[0])); ++i)
	{
		यदि (WinError == Error_सारणी[i].WinError)
		{
			प्रत्यागम Error_सारणी[i].errnocode;
		}
	}

	//
	// not in table. Check ranges
	//

	यदि ((WinError >= ERROR_WRITE_PROTECT) &&
		(WinError <= ERROR_SHARING_बफर_EXCEEDED))
	{
		प्रत्यागम EACCES;
	}
	अन्यथा यदि ((WinError >= ERROR_INVALID_STARTING_CODESEG) &&
			 (WinError <= ERROR_INFLOOP_अन्तर्_RELOC_CHAIN))
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
अचिह्नित
Map_Nt_स्थितिः(
    अन्तर्_2 NTSTATUS स्थितिः
   )
{
	प्रत्यागम _MapDosError(Rtl_Nt_स्थितिः_गन्तव्यम्_Dos_Error(स्थितिः));
}





//
// Helper functions to make things easier
//

स्थिर
NTSTATUS
_OpenNtName(
    अन्तर्_2 PCSTR Name,
    अन्तर्_2 BOOLEAN Readonly,
    बहिर्गमनम्_2 PHANDLE Handle,
    बहिर्गमनम्_2 PBOOLEAN OpenedReadonly OPTIONAL
   )
{
	UNICODE_STRING UnicodeString;
	ANSI_STRING    AnsiString;
	WCHAR बफर_2[512];
	NTSTATUS स्थितिः;
	OBJECT_गुणाः Object_गुणाः;
	IO_स्थितिः_खण्डम् Io_स्थितिः_खण्डम्;

	//
	// Make Unicode name from input string
	//

	UnicodeString.बफर_2 = &बफर_2[0];
	UnicodeString.दीर्घता_2 = 0;
	UnicodeString.अधिकतमम्_दीर्घता = परिमाण(बफर_2); // in bytes!!!

	RtlInitAnsiString(&AnsiString, Name);

	स्थितिः = Rtl_Ansi_String_गन्तव्यम्_Unicode_String(&UnicodeString, &AnsiString, FALSE);

	यदि(!NT_SUCCESS(स्थितिः))
	{
		प्रत्यागम स्थितिः; // Unmappable character?
	}

	//
	// Initialize object
	//

	Initialize_Object_गुणाः(&Object_गुणाः,
							   &UnicodeString,
							   OBJ_CASE_INSENSITIVE,
							   रिक्त,
							   रिक्त );

	//
	// Try to open it in initial mode
	//

	यदि(तर्कः_PRESENT(OpenedReadonly))
	{
		*OpenedReadonly = Readonly;
	}


	स्थितिः = NtOpenFile(Handle,
						SYNCHRONIZE | FILE_READ_दत्त | (Readonly ? 0 : FILE_WRITE_दत्त),
						&Object_गुणाः,
						&Io_स्थितिः_खण्डम्,
						FILE_SHARE_WRITE | FILE_SHARE_READ,
						FILE_SYNCHRONOUS_IO_NONALERT);

	यदि(!NT_SUCCESS(स्थितिः))
	{
		//
		// Maybe was just mounted? wait 0.5 sec and retry.
		//

		LARGE_INTEGER Interval;
		Interval.QuadPart = -5000000; // 0.5 sec. from now

		NtDelayExecution(FALSE, &Interval);

		स्थितिः = NtOpenFile(Handle,
							SYNCHRONIZE | FILE_READ_दत्त | (Readonly ? 0 : FILE_WRITE_दत्त),
							&Object_गुणाः,
							&Io_स्थितिः_खण्डम्,
							FILE_SHARE_WRITE | FILE_SHARE_READ,
							FILE_SYNCHRONOUS_IO_NONALERT);

		//
		// Try to satisfy mode
		//

		यदि((स्थितिः_ACCESS_DENIED == स्थितिः) && !Readonly)
		{
			यदि(तर्कः_PRESENT(OpenedReadonly))
			{
				*OpenedReadonly = TRUE;
			}

			स्थितिः = NtOpenFile(Handle,
							SYNCHRONIZE | FILE_READ_दत्त,
							&Object_गुणाः,
							&Io_स्थितिः_खण्डम्,
							FILE_SHARE_WRITE | FILE_SHARE_READ,
							FILE_SYNCHRONOUS_IO_NONALERT);
		}
	}



	//
	//समाप्तम्
	//

	प्रत्यागम स्थितिः;
}


स्थिर
NTSTATUS
_OpenDriveLetter(
    अन्तर्_2 CHAR Letter,
    अन्तर्_2 BOOLEAN ReadOnly,
    बहिर्गमनम्_2 PHANDLE Handle,
    बहिर्गमनम्_2 PBOOLEAN OpenedReadonly OPTIONAL
   )
{
	CHAR बफर_2[100];

	sprintf(बफर_2, "\\DosDevices\\%c:", Letter);

	प्रत्यागम _OpenNtName(बफर_2, ReadOnly, Handle, OpenedReadonly);
}


//
// Flush device
//

स्थिर
__inline
NTSTATUS
_FlushDrive(
		अन्तर्_2 HANDLE Handle
		)
{
	IO_स्थितिः_खण्डम् Io_स्थितिः_खण्डम्;
	प्रत्यागम NtFlushBuffersFile(Handle, &Io_स्थितिः_खण्डम्);
}


//
// lock drive
//

स्थिर
__inline
NTSTATUS
तालकम्_Drive(
		अन्तर्_2 HANDLE Handle
		)
{
	IO_स्थितिः_खण्डम् Io_स्थितिः_खण्डम्;
	प्रत्यागम NtFsControlFile(Handle, 0, 0, 0, &Io_स्थितिः_खण्डम्, FSCTL_तालकम्_VOLUME, 0, 0, 0, 0);
}


//
// unlock drive
//

स्थिर
__inline
NTSTATUS
_UnlockDrive(
	अन्तर्_2 HANDLE Handle
	)
{
	IO_स्थितिः_खण्डम् Io_स्थितिः_खण्डम्;
	प्रत्यागम NtFsControlFile(Handle, 0, 0, 0, &Io_स्थितिः_खण्डम्, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0);
}

स्थिर
__inline
NTSTATUS
_DismountDrive(
	अन्तर्_2 HANDLE Handle
	)
{
	IO_स्थितिः_खण्डम् Io_स्थितिः_खण्डम्;
	प्रत्यागम NtFsControlFile(Handle, 0, 0, 0, &Io_स्थितिः_खण्डम्, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0);
}


//
// is mounted
//

स्थिर
__inline
BOOLEAN
_IsMounted(
	अन्तर्_2 HANDLE Handle
	)
{
	IO_स्थितिः_खण्डम् Io_स्थितिः_खण्डम्;
	NTSTATUS स्थितिः;
	स्थितिः = NtFsControlFile(Handle, 0, 0, 0, &Io_स्थितिः_खण्डम्, FSCTL_IS_VOLUME_MOUNTED, 0, 0, 0, 0);
	प्रत्यागम (BOOLEAN)(स्थितिः_SUCCESS == स्थितिः);
}


स्थिर
__inline
NTSTATUS
_CloseDisk(
		अन्तर्_2 HANDLE Handle
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
    अन्तर्_2 PCSTR Device,
    अन्तर्_2 PSTR Normalized_Device_Name_बफर
   )
{
	पूर्णाङ्क Partition_सङ्ख्या = -1;
	UCHAR Disk_सङ्ख्या;
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

		Disk_सङ्ख्या = (UCHAR)(*(Device + 2) - 'a');

		यदि(*(Device + 3) != '\0')
		{
			Partition_सङ्ख्या = (*(Device + 3) - '0');
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

		Disk_सङ्ख्या = (UCHAR)(*(Device + 2) - '0');

	}
	अन्यथा
	{
		//
		// invalid prefix
		//

		प्रत्यागम रिक्त;
	}



	//
	//उपसर्गः
	//

	वाक्यं_प्रतिलिख(Normalized_Device_Name_बफर, "\\Device\\");

	//
	// Media name
	//

	विकल्प(*Device)
	{

	पक्ष 'f':
		वाक्यं_योजय(Normalized_Device_Name_बफर, "Floppy0");
		भङ्ग;

	पक्ष 'h':
		वाक्यं_योजय(Normalized_Device_Name_बफर, "Harddisk0");
		भङ्ग;
	}


	p = Normalized_Device_Name_बफर + वाक्यदैर्घ्यम्(Normalized_Device_Name_बफर) - 1;
	*p = (CHAR)(*p + Disk_सङ्ख्या);


	//
	// Partition nr.
	//

	यदि(Partition_सङ्ख्या >= 0)
	{
		वाक्यं_योजय(Normalized_Device_Name_बफर, "\\Partition0");

		p = Normalized_Device_Name_बफर + वाक्यदैर्घ्यम्(Normalized_Device_Name_बफर) - 1;
		*p = (CHAR)(*p + Partition_सङ्ख्या);
	}


	प्रत्यागम Normalized_Device_Name_बफर;
}




स्थिर
VOID
Get_Device_आकार(
    अन्तर्_2 HANDLE h,
    बहिर्गमनम्_2 अचिह्नित __int64 *Fs_आकार
   )
{
	PARTITION_INFORMATION pi;
	DISK_GEOMETRY gi;
	NTSTATUS स्थितिः;
	IO_स्थितिः_खण्डम् Io_स्थितिः_खण्डम्;

	//
	// Zero it
	//

	*Fs_आकार = 0;

	//
	// Call driver
	//

	Rtl_Zero_स्मृतिः(&pi, परिमाण(PARTITION_INFORMATION));

	स्थितिः = NtDeviceIoControlFile(
		h, रिक्त, रिक्त, रिक्त, &Io_स्थितिः_खण्डम्, IOCTL_DISK_GET_PARTITION_INFO,
		&pi, परिमाण(PARTITION_INFORMATION),
		&pi, परिमाण(PARTITION_INFORMATION));


	यदि(NT_SUCCESS(स्थितिः))
	{
		*Fs_आकार = pi.Partition_दीर्घता.QuadPart;
	}
	अन्यथा यदि(स्थितिः_INVALID_DEVICE_REQUEST == स्थितिः)
	{
		//
		// No partitions: get device info.
		//

		Rtl_Zero_स्मृतिः(&gi, परिमाण(DISK_GEOMETRY));

		स्थितिः = NtDeviceIoControlFile(
				h, रिक्त, रिक्त, रिक्त, &Io_स्थितिः_खण्डम्, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, परिमाण(DISK_GEOMETRY),
				&gi, परिमाण(DISK_GEOMETRY));


		यदि(NT_SUCCESS(स्थितिः))
		{
			*Fs_आकार =
				gi.अष्टकानि_प्रत्येकम्_Sector *
				gi.Sectors_प्रत्येकम्_Track *
				gi.Tracks_प्रत्येकम्_Cylinder *
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
    अन्तर्_2 PCSTR Name,
    अन्तर्_2 BOOLEAN ReadOnly,
    बहिर्गमनम्_2 PHANDLE Handle,
    बहिर्गमनम्_2 PBOOLEAN OpenedReadonly OPTIONAL,
    बहिर्गमनम्_2 अचिह्नित *Errno OPTIONAL
   )
{
	CHAR NormalizedDeviceName[512];
	NTSTATUS स्थितिः;

	यदि(रिक्त == Name)
	{
		//
		// Set not found
		//

		यदि(तर्कः_PRESENT(Errno))
			*Errno = ENOENT;

		प्रत्यागम FALSE;
	}


	यदि((((*Name) | 0x20) >= 'a') && (((*Name) | 0x20) <= 'z') &&
		(':' == *(Name + 1)) && ('\0' == *(Name + 2)))
	{
		स्थितिः = _OpenDriveLetter(*Name, ReadOnly, Handle, OpenedReadonly);
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

			यदि(तर्कः_PRESENT(Errno))
				*Errno = ENOENT;

			प्रत्यागम FALSE;
		}

		//
		// Try to open it
		//

		स्थितिः = _OpenNtName(Name, ReadOnly, Handle, OpenedReadonly);
	}


	यदि(!NT_SUCCESS(स्थितिः))
	{
		यदि(तर्कः_PRESENT(Errno))
			*Errno = Map_Nt_स्थितिः(स्थितिः);

		प्रत्यागम FALSE;
	}

	प्रत्यागम TRUE;
}


//
// Raw block io. Sets dos errno
//

स्थिर
BOOLEAN
खण्डम्_Io(
    अन्तर्_2 HANDLE Handle,
    अन्तर्_2 LARGE_INTEGER विस्थापनम्_2,
    अन्तर्_2 ULONG अष्टकानि_2,
    अन्तर्_2 बहिर्गमनम्_2 PCHAR बफर_2,
    अन्तर्_2 BOOLEAN Read,
    बहिर्गमनम्_2 अचिह्नित* Errno
   )
{
	IO_स्थितिः_खण्डम् Io_स्थितिः_खण्डम्;
	NTSTATUS स्थितिः;

	//
	// Should be aligned
	//

	ASSERT(0 == (अष्टकानि_2 % 512));
	ASSERT(0 == (विस्थापनम्_2.नीचः_Part % 512));


	//
	// perform io
	//

	यदि(Read)
	{
		स्थितिः = NtReadFile(Handle, रिक्त, रिक्त, रिक्त,
			&Io_स्थितिः_खण्डम्, बफर_2, अष्टकानि_2, &विस्थापनम्_2, रिक्त);
	}
	अन्यथा
	{
		स्थितिः = NtWriteFile(Handle, रिक्त, रिक्त, रिक्त,
			&Io_स्थितिः_खण्डम्, बफर_2, अष्टकानि_2, &विस्थापनम्_2, रिक्त);
	}


	//
	// translate error
	//

	यदि(NT_SUCCESS(स्थितिः))
	{
		*Errno = 0;
		प्रत्यागम TRUE;
	}

	*Errno = Map_Nt_स्थितिः(स्थितिः);

	प्रत्यागम FALSE;
}



__inline
BOOLEAN
_RawWrite(
    अन्तर्_2 HANDLE Handle,
    अन्तर्_2 LARGE_INTEGER विस्थापनम्_2,
    अन्तर्_2 ULONG अष्टकानि_2,
    बहिर्गमनम्_2 ध्रुव CHAR* बफर_2,
    बहिर्गमनम्_2 अचिह्नित* Errno
   )
{
	प्रत्यागम खण्डम्_Io(Handle, विस्थापनम्_2, अष्टकानि_2, (PCHAR)बफर_2, FALSE, Errno);
}

__inline
BOOLEAN
_RawRead(
    अन्तर्_2 HANDLE Handle,
    अन्तर्_2 LARGE_INTEGER विस्थापनम्_2,
    अन्तर्_2 ULONG अष्टकानि_2,
    अन्तर्_2 PCHAR बफर_2,
    बहिर्गमनम्_2 अचिह्नित* Errno
   )
{
	प्रत्यागम खण्डम्_Io(Handle, विस्थापनम्_2, अष्टकानि_2, बफर_2, TRUE, Errno);
}



__inline
BOOLEAN
समुच्चयः_Part_Type(
    अन्तर्_2 HANDLE Handle,
    अन्तर्_2 UCHAR Type
   )
{
	IO_स्थितिः_खण्डम् Io_स्थितिः_खण्डम्;
	प्रत्यागम स्थितिः_SUCCESS == NtDeviceIoControlFile(
												   Handle, रिक्त, रिक्त, रिक्त, &Io_स्थितिः_खण्डम्, IOCTL_DISK_समुच्चयः_PARTITION_INFO,
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

स्थिर __int64 Fs_आकार = 0;
स्थिर वर्ण knowndevice[1024] = "";


errcode_t
ext2fs_get_device_आकार(ध्रुव वर्ण *file, पूर्णाङ्क blocksize,
				 blk_t *retblocks)
{
	HANDLE h;
	BOOLEAN Readonly;

	यदि((0 == Fs_आकार) || (0 != वाक्यं_तुलय(knowndevice, file)))
	{

		यदि(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, रिक्त))
		{
			प्रत्यागम 0;
		}


		__try{

			//
			// Get size
			//

			Get_Device_आकार(h, &Fs_आकार);
			वाक्यं_प्रतिलिख(knowndevice, file);
		}
		__finally{
			_CloseDisk(h);
		}

	}

	*retblocks = (blk_t)(अचिह्नित __int64)(Fs_आकार / blocksize);
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
	PNT_वैयक्तिकम्_दत्त Nt_दत्त = रिक्त;
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

		io = (io_channel) स्मृतिं_देहि(परिमाण(संरचना struct_io_channel));

		यदि (रिक्त == io)
		{
			Errno = ENOMEM;
			__leave;
		}

		Rtl_Zero_स्मृतिः(io, परिमाण(संरचना struct_io_channel));
		io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

		Nt_दत्त = (PNT_वैयक्तिकम्_दत्त)स्मृतिं_देहि(परिमाण(NT_वैयक्तिकम्_दत्त_2));

		यदि (रिक्त == Nt_दत्त)
		{
			Errno = ENOMEM;
			__leave;
		}


		io->manager = nt_io_manager();
		io->name = स्मृतिं_देहि(वाक्यदैर्घ्यम्(name) + 1);
		यदि (रिक्त == io->name)
		{
			Errno = ENOMEM;
			__leave;
		}

		वाक्यं_प्रतिलिख(io->name, name);
		io->वैयक्तिकम्_दत्त = Nt_दत्त;
		io->खण्डम्_आकार = 1024;
		io->read_error = 0;
		io->write_error = 0;
		io->refcount = 1;

		//
		// Initialize data
		//

		Rtl_Zero_स्मृतिः(Nt_दत्त, परिमाण(NT_वैयक्तिकम्_दत्त_2));

		Nt_दत्त->magic = EXT2_ET_MAGIC_NT_IO_CHANNEL;
		Nt_दत्त->बफर_खण्डम्_सङ्ख्या = 0xffffffff;
		Nt_दत्त->बफर_आकार = 1024;
		Nt_दत्त->बफर_2 = स्मृतिं_देहि(Nt_दत्त->बफर_आकार);

		यदि (रिक्त == Nt_दत्त->बफर_2)
		{
			Errno = ENOMEM;
			__leave;
		}

		//
		// Open it
		//

		यदि(!_Ext2OpenDevice(name, (BOOLEAN)!BooleanFlagOn(flags, EXT2_FLAG_RW), &Nt_दत्त->Handle, &Nt_दत्त->OpenedReadonly, &Errno))
		{
			__leave;
		}


		//
		// get size
		//

		Get_Device_आकार(Nt_दत्त->Handle, &Fs_आकार);
		वाक्यं_प्रतिलिख(knowndevice, name);


		//
		// Lock/dismount
		//

		यदि(!NT_SUCCESS(तालकम्_Drive(Nt_दत्त->Handle)) /*|| !NT_SUCCESS(_DismountDrive(NtData->Handle))*/)
		{
			Nt_दत्त->OpenedReadonly = TRUE;
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

			यदि (रिक्त != Nt_दत्त)
			{
				यदि(रिक्त != Nt_दत्त->Handle)
				{
					_UnlockDrive(Nt_दत्त->Handle);
					_CloseDisk(Nt_दत्त->Handle);
				}

				मुक्त(Nt_दत्त->बफर_2);
				मुक्त(Nt_दत्त);
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
	PNT_वैयक्तिकम्_दत्त Nt_दत्त = रिक्त;

	यदि(रिक्त == channel)
	{
		प्रत्यागम 0;
	}

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_दत्त = (PNT_वैयक्तिकम्_दत्त) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(Nt_दत्त, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	यदि (--channel->refcount > 0)
	{
		प्रत्यागम 0;
	}

	मुक्त(channel->name);
	मुक्त(channel);

	यदि (रिक्त != Nt_दत्त)
	{
		यदि(रिक्त != Nt_दत्त->Handle)
		{
			_DismountDrive(Nt_दत्त->Handle);
			_UnlockDrive(Nt_दत्त->Handle);
			_CloseDisk(Nt_दत्त->Handle);
		}

		मुक्त(Nt_दत्त->बफर_2);
		मुक्त(Nt_दत्त);
	}

	प्रत्यागम 0;
}



//
// set block size
//

स्थिर
errcode_t
nt_समुच्चयः_blksize(io_channel channel, पूर्णाङ्क blksize)
{
	PNT_वैयक्तिकम्_दत्त Nt_दत्त = रिक्त;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_दत्त = (PNT_वैयक्तिकम्_दत्त) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(Nt_दत्त, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	यदि (channel->खण्डम्_आकार != blksize)
	{
		channel->खण्डम्_आकार = blksize;

		मुक्त(Nt_दत्त->बफर_2);
		Nt_दत्त->बफर_खण्डम्_सङ्ख्या = 0xffffffff;
		Nt_दत्त->बफर_आकार = channel->खण्डम्_आकार;
		ASSERT(0 == (Nt_दत्त->बफर_आकार % 512));

		Nt_दत्त->बफर_2 = स्मृतिं_देहि(Nt_दत्त->बफर_आकार);

		यदि (रिक्त == Nt_दत्त->बफर_2)
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
nt_read_blk(io_channel channel, अचिह्नित दीर्घ खण्डम्,
			       पूर्णाङ्क गणनम्, शून्य *buf)
{
	PVOID बफर_गन्तव्यम्_Read;
	ULONG आकार_गन्तव्यम्_Read;
	ULONG आकार_2;
	LARGE_INTEGER विस्थापनम्_2;
	PNT_वैयक्तिकम्_दत्त Nt_दत्त = रिक्त;
	अचिह्नित Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_दत्त = (PNT_वैयक्तिकम्_दत्त) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(Nt_दत्त, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	//
	// If it's in the cache, use it!
	//

	यदि ((1 == गणनम्) &&
		(खण्डम् == Nt_दत्त->बफर_खण्डम्_सङ्ख्या) &&
		(Nt_दत्त->बफर_खण्डम्_सङ्ख्या != 0xffffffff))
	{
		स्मृतिं_प्रतिलिख(buf, Nt_दत्त->बफर_2, channel->खण्डम्_आकार);
		प्रत्यागम 0;
	}

	आकार_2 = (गणनम् < 0) ? (ULONG)(-गणनम्) : (ULONG)(गणनम् * channel->खण्डम्_आकार);

	विस्थापनम्_2.QuadPart = खण्डम् * channel->खण्डम्_आकार;

	//
	// If not fit to the block
	//

	यदि(आकार_2 <= Nt_दत्त->बफर_आकार)
	{
		//
		// Update the cache
		//

		Nt_दत्त->बफर_खण्डम्_सङ्ख्या = खण्डम्;
		बफर_गन्तव्यम्_Read = Nt_दत्त->बफर_2;
		आकार_गन्तव्यम्_Read = Nt_दत्त->बफर_आकार;
	}
	अन्यथा
	{
		आकार_गन्तव्यम्_Read = आकार_2;
		बफर_गन्तव्यम्_Read = buf;
		ASSERT(0 == (आकार_गन्तव्यम्_Read % channel->खण्डम्_आकार));
	}

	यदि(!_RawRead(Nt_दत्त->Handle, विस्थापनम्_2, आकार_गन्तव्यम्_Read, बफर_गन्तव्यम्_Read, &Errno))
	{

		यदि (channel->read_error)
		{
			प्रत्यागम (channel->read_error)(channel, खण्डम्, गणनम्, buf,
					       आकार_2, 0, Errno);
		}
		अन्यथा
		{
			प्रत्यागम Errno;
		}
	}


	यदि(बफर_गन्तव्यम्_Read != buf)
	{
		ASSERT(आकार_2 <= आकार_गन्तव्यम्_Read);
		स्मृतिं_प्रतिलिख(buf, बफर_गन्तव्यम्_Read, आकार_2);
	}

	प्रत्यागम 0;
}


//
// write block
//

स्थिर
errcode_t
nt_write_blk(io_channel channel, अचिह्नित दीर्घ खण्डम्,
				पूर्णाङ्क गणनम्, ध्रुव शून्य *buf)
{
	ULONG आकार_गन्तव्यम्_Write;
	LARGE_INTEGER विस्थापनम्_2;
	PNT_वैयक्तिकम्_दत्त Nt_दत्त = रिक्त;
	अचिह्नित Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_दत्त = (PNT_वैयक्तिकम्_दत्त) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(Nt_दत्त, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	यदि(Nt_दत्त->OpenedReadonly)
	{
		प्रत्यागम EACCES;
	}

	यदि (गणनम् == 1)
	{
		आकार_गन्तव्यम्_Write = channel->खण्डम्_आकार;
	}
	अन्यथा
	{
		Nt_दत्त->बफर_खण्डम्_सङ्ख्या = 0xffffffff;

		यदि (गणनम् < 0)
		{
			आकार_गन्तव्यम्_Write = (ULONG)(-गणनम्);
		}
		अन्यथा
		{
			आकार_गन्तव्यम्_Write = (ULONG)(गणनम् * channel->खण्डम्_आकार);
		}
	}


	ASSERT(0 == (आकार_गन्तव्यम्_Write % 512));
	विस्थापनम्_2.QuadPart = खण्डम् * channel->खण्डम्_आकार;

	यदि(!_RawWrite(Nt_दत्त->Handle, विस्थापनम्_2, आकार_गन्तव्यम्_Write, buf, &Errno))
	{
		यदि (channel->write_error)
		{
			प्रत्यागम (channel->write_error)(channel, खण्डम्, गणनम्, buf,
						आकार_गन्तव्यम्_Write, 0, Errno);
		}
		अन्यथा
		{
			प्रत्यागम Errno;
		}
	}


	//
	// Stash a copy.
	//

	यदि(आकार_गन्तव्यम्_Write >= Nt_दत्त->बफर_आकार)
	{
		Nt_दत्त->बफर_खण्डम्_सङ्ख्या = खण्डम्;
		स्मृतिं_प्रतिलिख(Nt_दत्त->बफर_2, buf, Nt_दत्त->बफर_आकार);
	}

	Nt_दत्त->Written = TRUE;

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
	PNT_वैयक्तिकम्_दत्त Nt_दत्त = रिक्त;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_दत्त = (PNT_वैयक्तिकम्_दत्त) channel->वैयक्तिकम्_दत्त;
	EXT2_CHECK_MAGIC(Nt_दत्त, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	यदि(Nt_दत्त->OpenedReadonly)
	{
		प्रत्यागम 0; // EACCESS;
	}


	//
	// Flush file buffers.
	//

	_FlushDrive(Nt_दत्त->Handle);


	//
	// Test and correct partition type.
	//

	यदि(Nt_दत्त->Written)
	{
		समुच्चयः_Part_Type(Nt_दत्त->Handle, 0x83);
	}

	प्रत्यागम 0;
}


