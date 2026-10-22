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

#నిర్వచితమైతే HAVE_CONFIG_H
#అన్తర్భావ "config.h"
#అయితే_ముగింపు


//
// I need some warnings to disable...
//


#సూచన warning(disable:4514) // unreferenced inline function has been removed
#సూచన warning(నెట్టు,4)

#సూచన warning(disable:4201) // nonstandard extension used : nameless struct/union)
#సూచన warning(disable:4214) // nonstandard extension used : bit field types other than int
#సూచన warning(disable:4115) // named type definition in parentheses

#అన్తర్భావ <ntddk.h>
#అన్తర్భావ <ntdddisk.h>
#అన్తర్భావ <ntstatus.h>

#సూచన warning(తీయి)


//
// Some native APIs.
//

NTSYSAPI
ULONG
NTAPI
Rtl_Nt_స్థితి_గమ్యం_Dos_Error(
    లోపల_2 NTSTATUS స్థితి_2
   );

NTSYSAPI
NTSTATUS
NTAPI
NtClose(
    లోపల_2 HANDLE Handle
   );


NTSYSAPI
NTSTATUS
NTAPI
NtOpenFile(
    బహిర్గతం_2 PHANDLE FileHandle,
    లోపల_2 ACCESS_ముసుగు DesiredAccess,
    లోపల_2 POBJECT_లక్షణాలు Object_లక్షణాలు,
    బహిర్గతం_2 PIO_స్థితి_ఖండం Io_స్థితి_ఖండం,
    లోపల_2 ULONG ShareAccess,
    లోపల_2 ULONG OpenOptions
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFlushBuffersFile(
    లోపల_2 HANDLE FileHandle,
    బహిర్గతం_2 PIO_స్థితి_ఖండం Io_స్థితి_ఖండం
   );


NTSYSAPI
NTSTATUS
NTAPI
NtReadFile(
    లోపల_2 HANDLE FileHandle,
    లోపల_2 HANDLE Event OPTIONAL,
    లోపల_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    లోపల_2 PVOID ApcContext OPTIONAL,
    బహిర్గతం_2 PIO_స్థితి_ఖండం Io_స్థితి_ఖండం,
    బహిర్గతం_2 PVOID బఫరు_2,
    లోపల_2 ULONG నిడివి_2,
    లోపల_2 PLARGE_INTEGER Byte_స్థానభ్రంశం OPTIONAL,
    లోపల_2 PULONG కీలకం_2 OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtWriteFile(
    లోపల_2 HANDLE FileHandle,
    లోపల_2 HANDLE Event OPTIONAL,
    లోపల_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    లోపల_2 PVOID ApcContext OPTIONAL,
    బహిర్గతం_2 PIO_స్థితి_ఖండం Io_స్థితి_ఖండం,
    లోపల_2 PVOID బఫరు_2,
    లోపల_2 ULONG నిడివి_2,
    లోపల_2 PLARGE_INTEGER Byte_స్థానభ్రంశం OPTIONAL,
    లోపల_2 PULONG కీలకం_2 OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtDeviceIoControlFile(
    లోపల_2 HANDLE FileHandle,
    లోపల_2 HANDLE Event OPTIONAL,
    లోపల_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    లోపల_2 PVOID ApcContext OPTIONAL,
    బహిర్గతం_2 PIO_స్థితి_ఖండం Io_స్థితి_ఖండం,
    లోపల_2 ULONG Io_Control_సంకేతం,
    లోపల_2 PVOID ఆదానం_బఫరు OPTIONAL,
    లోపల_2 ULONG ఆదానం_బఫరు_నిడివి,
    బహిర్గతం_2 PVOID Output_బఫరు OPTIONAL,
    లోపల_2 ULONG Output_బఫరు_నిడివి
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFsControlFile(
    లోపల_2 HANDLE FileHandle,
    లోపల_2 HANDLE Event OPTIONAL,
    లోపల_2 PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    లోపల_2 PVOID ApcContext OPTIONAL,
    బహిర్గతం_2 PIO_స్థితి_ఖండం Io_స్థితి_ఖండం,
    లోపల_2 ULONG Io_Control_సంకేతం,
    లోపల_2 PVOID ఆదానం_బఫరు OPTIONAL,
    లోపల_2 ULONG ఆదానం_బఫరు_నిడివి,
    బహిర్గతం_2 PVOID Output_బఫరు OPTIONAL,
    లోపల_2 ULONG Output_బఫరు_నిడివి
    );


NTSYSAPI
NTSTATUS
NTAPI
NtDelayExecution(
    లోపల_2 BOOLEAN Alertable,
    లోపల_2 PLARGE_INTEGER Interval
    );


#నిర్వచించు FSCTL_తాళం_VOLUME               CTL_సంకేతం(FILE_DEVICE_FILE_SYSTEM, 6, పద్ధతి_BUFFERED, FILE_ANY_ACCESS)
#నిర్వచించు FSCTL_UNLOCK_VOLUME             CTL_సంకేతం(FILE_DEVICE_FILE_SYSTEM, 7, పద్ధతి_BUFFERED, FILE_ANY_ACCESS)
#నిర్వచించు FSCTL_DISMOUNT_VOLUME           CTL_సంకేతం(FILE_DEVICE_FILE_SYSTEM, 8, పద్ధతి_BUFFERED, FILE_ANY_ACCESS)
#నిర్వచించు FSCTL_IS_VOLUME_MOUNTED         CTL_సంకేతం(FILE_DEVICE_FILE_SYSTEM,10, పద్ధతి_BUFFERED, FILE_ANY_ACCESS)


//
// useful macros
//

#నిర్వచించు BooleanFlagOn(Flags,SingleFlag) ((BOOLEAN)((((Flags) & (SingleFlag)) != 0)))


//
// Include Win32 error codes.
//

#అన్తర్భావ <winerror.h>

//
// standard stuff
//

#అన్తర్భావ <assert.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <malloc.h>

#అన్తర్భావ <linux/types.h>
#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ <errno.h>

#అన్తర్భావ "et/com_err.h"
#అన్తర్భావ "ext2fs/ext2fs.h"
#అన్తర్భావ "ext2fs/ext2_err.h"




//
// For checking structure magic numbers...
//


#నిర్వచించు EXT2_CHECK_MAGIC(నిర్మాణం, సంకేతం_2) \
	  ఒకవేళ ((నిర్మాణం)->magic != (సంకేతం_2)) తిరిగి (సంకేతం_2)

#నిర్వచించు EXT2_ET_MAGIC_NT_IO_CHANNEL  0x10ed


//
// Private data block
//

రకనిర్వచనం నిర్మాణం NT_వ్యక్తిగత_దత్తాంశం {
	పూర్ణాంకం	   magic;
	HANDLE Handle;
	పూర్ణాంకం	   Flags;
	PCHAR  బఫరు_2;
	__u32  బఫరు_ఖండం_సంఖ్య;
	ULONG  బఫరు_కొలత;
	BOOLEAN OpenedReadonly;
	BOOLEAN Written;
}NT_వ్యక్తిగత_దత్తాంశం_2, *PNT_వ్యక్తిగత_దత్తాంశం;



//
// Standard interface prototypes
//

స్థిర errcode_t nt_open(స్థిరరాశి అక్షరం *name, పూర్ణాంకం flags, io_channel *channel);
స్థిర errcode_t nt_close(io_channel channel);
స్థిర errcode_t nt_సమితి_blksize(io_channel channel, పూర్ణాంకం blksize);
స్థిర errcode_t nt_read_blk(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
			       పూర్ణాంకం లెక్క, శూన్య *దత్తాంశం);
స్థిర errcode_t nt_write_blk(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
				పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *దత్తాంశం);
స్థిర errcode_t nt_flush(io_channel channel);

స్థిర నిర్మాణం struct_io_manager struct_nt_manager = {
	.magic		= EXT2_ET_MAGIC_IO_MANAGER,
	.name		= "NT I/O Manager",
	.open		= nt_open,
	.close		= nt_close,
	.సమితి_blksize	= nt_సమితి_blksize,
	.read_blk	= nt_read_blk,
	.write_blk	= nt_write_blk,
	.flush		= nt_flush
};

//
// function to get API
//

io_manager nt_io_manager()
{
	తిరిగి &struct_nt_manager;
}





//
// This is a code to convert Win32 errors to unix errno
//

రకనిర్వచనం నిర్మాణం {
	ULONG WinError;
	పూర్ణాంకం errnocode;
}ERROR_నమోదు;

స్థిర ERROR_నమోదు Error_పట్టిక[] = {
        {  ERROR_INVALID_FUNCTION,       EINVAL    },
        {  ERROR_FILE_NOT_దొరికింది,         ENOENT    },
        {  ERROR_మార్గం_NOT_దొరికింది,         ENOENT    },
        {  ERROR_TOO_MANY_OPEN_FILES,    EMFILE    },
        {  ERROR_ACCESS_DENIED,          EACCES    },
        {  ERROR_INVALID_HANDLE,         EBADF     },
        {  ERROR_ARENA_TRASHED,          ENOMEM    },
        {  ERROR_NOT_ENOUGH_జ్ఞాపకం,      ENOMEM    },
        {  ERROR_INVALID_ఖండం,          ENOMEM    },
        {  ERROR_BAD_ENVIRONMENT,        E2BIG     },
        {  ERROR_BAD_FORMAT,             ENOEXEC   },
        {  ERROR_INVALID_ACCESS,         EINVAL    },
        {  ERROR_INVALID_దత్తాంశం,           EINVAL    },
        {  ERROR_INVALID_DRIVE,          ENOENT    },
        {  ERROR_CURRENT_DIRECTORY,      EACCES    },
        {  ERROR_NOT_SAME_DEVICE,        EXDEV     },
        {  ERROR_NO_మరిన్ని_FILES,          ENOENT    },
        {  ERROR_తాళం_VIOLATION,         EACCES    },
        {  ERROR_BAD_NETPATH,            ENOENT    },
        {  ERROR_జాలం_ACCESS_DENIED,  EACCES    },
        {  ERROR_BAD_NET_NAME,           ENOENT    },
        {  ERROR_FILE_EXISTS,            EEXIST    },
        {  ERROR_CANNOT_MAKE,            EACCES    },
        {  ERROR_FAIL_I24,               EACCES    },
        {  ERROR_INVALID_PARAMETER,      EINVAL    },
        {  ERROR_NO_PROC_SLOTS,          EAGAIN    },
        {  ERROR_DRIVE_LOCKED,           EACCES    },
        {  ERROR_BROKEN_PIPE,            EPIPE     },
        {  ERROR_DISK_నిండిన,              ENOSPC    },
        {  ERROR_INVALID_లక్ష్యం_HANDLE,  EBADF     },
        {  ERROR_INVALID_HANDLE,         EINVAL    },
        {  ERROR_WAIT_NO_CHILDREN,       ECHILD    },
        {  ERROR_బిడ్డ_NOT_COMPLETE,     ECHILD    },
        {  ERROR_ప్రత్యక్ష_ACCESS_HANDLE,   EBADF     },
        {  ERROR_NEGATIVE_SEEK,          EINVAL    },
        {  ERROR_SEEK_ON_DEVICE,         EACCES    },
        {  ERROR_DIR_NOT_ఖాళీలు,          ENOTEMPTY },
        {  ERROR_NOT_LOCKED,             EACCES    },
        {  ERROR_BAD_PATHNAME,           ENOENT    },
        {  ERROR_MAX_THRDS_REACHED,      EAGAIN    },
        {  ERROR_తాళం_FAILED,            EACCES    },
        {  ERROR_ALREADY_EXISTS,         EEXIST    },
        {  ERROR_FILENAME_EXCED_RANGE,   ENOENT    },
        {  ERROR_NESTING_NOT_ALLOWED,    EAGAIN    },
        {  ERROR_NOT_ENOUGH_QUOTA,       ENOMEM    }
};




స్థిర
గుర్తులేని
_MapDosError (
    లోపల_2 ULONG WinError
   )
{
	పూర్ణాంకం i;

	//
	// Lookup
	//

	కొరకు (i = 0; i < (పరిమాణం(Error_పట్టిక)/పరిమాణం(Error_పట్టిక[0])); ++i)
	{
		ఒకవేళ (WinError == Error_పట్టిక[i].WinError)
		{
			తిరిగి Error_పట్టిక[i].errnocode;
		}
	}

	//
	// not in table. Check ranges
	//

	ఒకవేళ ((WinError >= ERROR_WRITE_PROTECT) &&
		(WinError <= ERROR_SHARING_బఫరు_EXCEEDED))
	{
		తిరిగి EACCES;
	}
	లేకపోతే ఒకవేళ ((WinError >= ERROR_INVALID_STARTING_CODESEG) &&
			 (WinError <= ERROR_INFLOOP_లోపల_RELOC_CHAIN))
	{
		తిరిగి ENOEXEC;
	}
	లేకపోతే
	{
		తిరిగి EINVAL;
	}
}







//
// Function to map NT status to dos error.
//

స్థిర
__inline
గుర్తులేని
Map_Nt_స్థితి(
    లోపల_2 NTSTATUS స్థితి_2
   )
{
	తిరిగి _MapDosError(Rtl_Nt_స్థితి_గమ్యం_Dos_Error(స్థితి_2));
}





//
// Helper functions to make things easier
//

స్థిర
NTSTATUS
_OpenNtName(
    లోపల_2 PCSTR Name,
    లోపల_2 BOOLEAN Readonly,
    బహిర్గతం_2 PHANDLE Handle,
    బహిర్గతం_2 PBOOLEAN OpenedReadonly OPTIONAL
   )
{
	UNICODE_STRING UnicodeString;
	ANSI_STRING    AnsiString;
	WCHAR బఫరు_2[512];
	NTSTATUS స్థితి_2;
	OBJECT_లక్షణాలు Object_లక్షణాలు;
	IO_స్థితి_ఖండం Io_స్థితి_ఖండం;

	//
	// Make Unicode name from input string
	//

	UnicodeString.బఫరు_2 = &బఫరు_2[0];
	UnicodeString.నిడివి_2 = 0;
	UnicodeString.గరిష్ఠ_నిడివి = పరిమాణం(బఫరు_2); // in bytes!!!

	RtlInitAnsiString(&AnsiString, Name);

	స్థితి_2 = Rtl_Ansi_String_గమ్యం_Unicode_String(&UnicodeString, &AnsiString, FALSE);

	ఒకవేళ(!NT_SUCCESS(స్థితి_2))
	{
		తిరిగి స్థితి_2; // Unmappable character?
	}

	//
	// Initialize object
	//

	Initialize_Object_లక్షణాలు(&Object_లక్షణాలు,
							   &UnicodeString,
							   OBJ_CASE_INSENSITIVE,
							   ఖాళీ,
							   ఖాళీ );

	//
	// Try to open it in initial mode
	//

	ఒకవేళ(వాదం_PRESENT(OpenedReadonly))
	{
		*OpenedReadonly = Readonly;
	}


	స్థితి_2 = NtOpenFile(Handle,
						SYNCHRONIZE | FILE_READ_దత్తాంశం | (Readonly ? 0 : FILE_WRITE_దత్తాంశం),
						&Object_లక్షణాలు,
						&Io_స్థితి_ఖండం,
						FILE_SHARE_WRITE | FILE_SHARE_READ,
						FILE_SYNCHRONOUS_IO_NONALERT);

	ఒకవేళ(!NT_SUCCESS(స్థితి_2))
	{
		//
		// Maybe was just mounted? wait 0.5 sec and retry.
		//

		LARGE_INTEGER Interval;
		Interval.QuadPart = -5000000; // 0.5 sec. from now

		NtDelayExecution(FALSE, &Interval);

		స్థితి_2 = NtOpenFile(Handle,
							SYNCHRONIZE | FILE_READ_దత్తాంశం | (Readonly ? 0 : FILE_WRITE_దత్తాంశం),
							&Object_లక్షణాలు,
							&Io_స్థితి_ఖండం,
							FILE_SHARE_WRITE | FILE_SHARE_READ,
							FILE_SYNCHRONOUS_IO_NONALERT);

		//
		// Try to satisfy mode
		//

		ఒకవేళ((స్థితి_ACCESS_DENIED == స్థితి_2) && !Readonly)
		{
			ఒకవేళ(వాదం_PRESENT(OpenedReadonly))
			{
				*OpenedReadonly = TRUE;
			}

			స్థితి_2 = NtOpenFile(Handle,
							SYNCHRONIZE | FILE_READ_దత్తాంశం,
							&Object_లక్షణాలు,
							&Io_స్థితి_ఖండం,
							FILE_SHARE_WRITE | FILE_SHARE_READ,
							FILE_SYNCHRONOUS_IO_NONALERT);
		}
	}



	//
	//పూర్తి
	//

	తిరిగి స్థితి_2;
}


స్థిర
NTSTATUS
_OpenDriveLetter(
    లోపల_2 CHAR Letter,
    లోపల_2 BOOLEAN ReadOnly,
    బహిర్గతం_2 PHANDLE Handle,
    బహిర్గతం_2 PBOOLEAN OpenedReadonly OPTIONAL
   )
{
	CHAR బఫరు_2[100];

	sprintf(బఫరు_2, "\\DosDevices\\%c:", Letter);

	తిరిగి _OpenNtName(బఫరు_2, ReadOnly, Handle, OpenedReadonly);
}


//
// Flush device
//

స్థిర
__inline
NTSTATUS
_FlushDrive(
		లోపల_2 HANDLE Handle
		)
{
	IO_స్థితి_ఖండం Io_స్థితి_ఖండం;
	తిరిగి NtFlushBuffersFile(Handle, &Io_స్థితి_ఖండం);
}


//
// lock drive
//

స్థిర
__inline
NTSTATUS
తాళం_Drive(
		లోపల_2 HANDLE Handle
		)
{
	IO_స్థితి_ఖండం Io_స్థితి_ఖండం;
	తిరిగి NtFsControlFile(Handle, 0, 0, 0, &Io_స్థితి_ఖండం, FSCTL_తాళం_VOLUME, 0, 0, 0, 0);
}


//
// unlock drive
//

స్థిర
__inline
NTSTATUS
_UnlockDrive(
	లోపల_2 HANDLE Handle
	)
{
	IO_స్థితి_ఖండం Io_స్థితి_ఖండం;
	తిరిగి NtFsControlFile(Handle, 0, 0, 0, &Io_స్థితి_ఖండం, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0);
}

స్థిర
__inline
NTSTATUS
_DismountDrive(
	లోపల_2 HANDLE Handle
	)
{
	IO_స్థితి_ఖండం Io_స్థితి_ఖండం;
	తిరిగి NtFsControlFile(Handle, 0, 0, 0, &Io_స్థితి_ఖండం, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0);
}


//
// is mounted
//

స్థిర
__inline
BOOLEAN
_IsMounted(
	లోపల_2 HANDLE Handle
	)
{
	IO_స్థితి_ఖండం Io_స్థితి_ఖండం;
	NTSTATUS స్థితి_2;
	స్థితి_2 = NtFsControlFile(Handle, 0, 0, 0, &Io_స్థితి_ఖండం, FSCTL_IS_VOLUME_MOUNTED, 0, 0, 0, 0);
	తిరిగి (BOOLEAN)(స్థితి_SUCCESS == స్థితి_2);
}


స్థిర
__inline
NTSTATUS
_CloseDisk(
		లోపల_2 HANDLE Handle
		)
{
	తిరిగి NtClose(Handle);
}




//
// Make NT name from any recognized name
//

స్థిర
PCSTR
_NormalizeDeviceName(
    లోపల_2 PCSTR Device,
    లోపల_2 PSTR Normalized_Device_Name_బఫరు
   )
{
	పూర్ణాంకం విభజన_సంఖ్య = -1;
	UCHAR Disk_సంఖ్య;
	PSTR p;


	//
	// Do not try to parse NT name
	//

	ఒకవేళ('\\' == *Device)
		తిరిగి Device;



	//
	// Strip leading '/dev/' if any
	//

	ఒకవేళ(('/' == *(Device)) &&
		('d' == *(Device + 1)) &&
		('e' == *(Device + 2)) &&
		('v' == *(Device + 3)) &&
		('/' == *(Device + 4)))
	{
		Device += 5;
	}

	ఒకవేళ('\0' == *Device)
	{
		తిరిగి ఖాళీ;
	}


	//
	// forms: hda[n], fd[n]
	//

	ఒకవేళ('d' != *(Device + 1))
	{
		తిరిగి ఖాళీ;
	}

	ఒకవేళ('h' == *Device)
	{
		ఒకవేళ((*(Device + 2) < 'a') || (*(Device + 2) > ('a' + 9)) ||
		   ((*(Device + 3) != '\0') &&
			((*(Device + 4) != '\0') ||
			 ((*(Device + 3) < '0') || (*(Device + 3) > '9'))
			)
		   )
		  )
		{
			తిరిగి ఖాళీ;
		}

		Disk_సంఖ్య = (UCHAR)(*(Device + 2) - 'a');

		ఒకవేళ(*(Device + 3) != '\0')
		{
			విభజన_సంఖ్య = (*(Device + 3) - '0');
		}

	}
	లేకపోతే ఒకవేళ('f' == *Device)
	{
		//
		// 3-d letter should be a digit.
		//

		ఒకవేళ((*(Device + 3) != '\0') ||
		   (*(Device + 2) < '0') || (*(Device + 2) > '9'))
		{
			తిరిగి ఖాళీ;
		}

		Disk_సంఖ్య = (UCHAR)(*(Device + 2) - '0');

	}
	లేకపోతే
	{
		//
		// invalid prefix
		//

		తిరిగి ఖాళీ;
	}



	//
	//ఉపసర్గ
	//

	strcpy(Normalized_Device_Name_బఫరు, "\\Device\\");

	//
	// Media name
	//

	ఎంపిక(*Device)
	{

	స్థితి 'f':
		strcat(Normalized_Device_Name_బఫరు, "Floppy0");
		ఆపు;

	స్థితి 'h':
		strcat(Normalized_Device_Name_బఫరు, "Harddisk0");
		ఆపు;
	}


	p = Normalized_Device_Name_బఫరు + అక్షరశ్రేణి_పొడవు(Normalized_Device_Name_బఫరు) - 1;
	*p = (CHAR)(*p + Disk_సంఖ్య);


	//
	// Partition nr.
	//

	ఒకవేళ(విభజన_సంఖ్య >= 0)
	{
		strcat(Normalized_Device_Name_బఫరు, "\\Partition0");

		p = Normalized_Device_Name_బఫరు + అక్షరశ్రేణి_పొడవు(Normalized_Device_Name_బఫరు) - 1;
		*p = (CHAR)(*p + విభజన_సంఖ్య);
	}


	తిరిగి Normalized_Device_Name_బఫరు;
}




స్థిర
VOID
Get_Device_కొలత(
    లోపల_2 HANDLE h,
    బహిర్గతం_2 గుర్తులేని __int64 *Fs_కొలత
   )
{
	విభజన_INFORMATION pi;
	DISK_GEOMETRY gi;
	NTSTATUS స్థితి_2;
	IO_స్థితి_ఖండం Io_స్థితి_ఖండం;

	//
	// Zero it
	//

	*Fs_కొలత = 0;

	//
	// Call driver
	//

	Rtl_Zero_జ్ఞాపకం(&pi, పరిమాణం(విభజన_INFORMATION));

	స్థితి_2 = NtDeviceIoControlFile(
		h, ఖాళీ, ఖాళీ, ఖాళీ, &Io_స్థితి_ఖండం, IOCTL_DISK_GET_విభజన_INFO,
		&pi, పరిమాణం(విభజన_INFORMATION),
		&pi, పరిమాణం(విభజన_INFORMATION));


	ఒకవేళ(NT_SUCCESS(స్థితి_2))
	{
		*Fs_కొలత = pi.విభజన_నిడివి.QuadPart;
	}
	లేకపోతే ఒకవేళ(స్థితి_INVALID_DEVICE_REQUEST == స్థితి_2)
	{
		//
		// No partitions: get device info.
		//

		Rtl_Zero_జ్ఞాపకం(&gi, పరిమాణం(DISK_GEOMETRY));

		స్థితి_2 = NtDeviceIoControlFile(
				h, ఖాళీ, ఖాళీ, ఖాళీ, &Io_స్థితి_ఖండం, IOCTL_DISK_GET_DRIVE_GEOMETRY,
				&gi, పరిమాణం(DISK_GEOMETRY),
				&gi, పరిమాణం(DISK_GEOMETRY));


		ఒకవేళ(NT_SUCCESS(స్థితి_2))
		{
			*Fs_కొలత =
				gi.బైట్లు_చొప్పున_Sector *
				gi.Sectors_చొప్పున_Track *
				gi.Tracks_చొప్పున_Cylinder *
				gi.Cylinders.QuadPart;
		}

	}
}



//
// Open device by name.
//

స్థిర
BOOLEAN
_Ext2OpenDevice(
    లోపల_2 PCSTR Name,
    లోపల_2 BOOLEAN ReadOnly,
    బహిర్గతం_2 PHANDLE Handle,
    బహిర్గతం_2 PBOOLEAN OpenedReadonly OPTIONAL,
    బహిర్గతం_2 గుర్తులేని *Errno OPTIONAL
   )
{
	CHAR NormalizedDeviceName[512];
	NTSTATUS స్థితి_2;

	ఒకవేళ(ఖాళీ == Name)
	{
		//
		// Set not found
		//

		ఒకవేళ(వాదం_PRESENT(Errno))
			*Errno = ENOENT;

		తిరిగి FALSE;
	}


	ఒకవేళ((((*Name) | 0x20) >= 'a') && (((*Name) | 0x20) <= 'z') &&
		(':' == *(Name + 1)) && ('\0' == *(Name + 2)))
	{
		స్థితి_2 = _OpenDriveLetter(*Name, ReadOnly, Handle, OpenedReadonly);
	}
	లేకపోతే
	{
		//
		// Make name
		//

		Name = _NormalizeDeviceName(Name, NormalizedDeviceName);

		ఒకవేళ(ఖాళీ == Name)
		{
			//
			// Set not found
			//

			ఒకవేళ(వాదం_PRESENT(Errno))
				*Errno = ENOENT;

			తిరిగి FALSE;
		}

		//
		// Try to open it
		//

		స్థితి_2 = _OpenNtName(Name, ReadOnly, Handle, OpenedReadonly);
	}


	ఒకవేళ(!NT_SUCCESS(స్థితి_2))
	{
		ఒకవేళ(వాదం_PRESENT(Errno))
			*Errno = Map_Nt_స్థితి(స్థితి_2);

		తిరిగి FALSE;
	}

	తిరిగి TRUE;
}


//
// Raw block io. Sets dos errno
//

స్థిర
BOOLEAN
ఖండం_Io(
    లోపల_2 HANDLE Handle,
    లోపల_2 LARGE_INTEGER స్థానభ్రంశం_2,
    లోపల_2 ULONG బైట్లు_2,
    లోపల_2 బహిర్గతం_2 PCHAR బఫరు_2,
    లోపల_2 BOOLEAN Read,
    బహిర్గతం_2 గుర్తులేని* Errno
   )
{
	IO_స్థితి_ఖండం Io_స్థితి_ఖండం;
	NTSTATUS స్థితి_2;

	//
	// Should be aligned
	//

	ASSERT(0 == (బైట్లు_2 % 512));
	ASSERT(0 == (స్థానభ్రంశం_2.దిగువ_Part % 512));


	//
	// perform io
	//

	ఒకవేళ(Read)
	{
		స్థితి_2 = NtReadFile(Handle, ఖాళీ, ఖాళీ, ఖాళీ,
			&Io_స్థితి_ఖండం, బఫరు_2, బైట్లు_2, &స్థానభ్రంశం_2, ఖాళీ);
	}
	లేకపోతే
	{
		స్థితి_2 = NtWriteFile(Handle, ఖాళీ, ఖాళీ, ఖాళీ,
			&Io_స్థితి_ఖండం, బఫరు_2, బైట్లు_2, &స్థానభ్రంశం_2, ఖాళీ);
	}


	//
	// translate error
	//

	ఒకవేళ(NT_SUCCESS(స్థితి_2))
	{
		*Errno = 0;
		తిరిగి TRUE;
	}

	*Errno = Map_Nt_స్థితి(స్థితి_2);

	తిరిగి FALSE;
}



__inline
BOOLEAN
_RawWrite(
    లోపల_2 HANDLE Handle,
    లోపల_2 LARGE_INTEGER స్థానభ్రంశం_2,
    లోపల_2 ULONG బైట్లు_2,
    బహిర్గతం_2 స్థిరరాశి CHAR* బఫరు_2,
    బహిర్గతం_2 గుర్తులేని* Errno
   )
{
	తిరిగి ఖండం_Io(Handle, స్థానభ్రంశం_2, బైట్లు_2, (PCHAR)బఫరు_2, FALSE, Errno);
}

__inline
BOOLEAN
_RawRead(
    లోపల_2 HANDLE Handle,
    లోపల_2 LARGE_INTEGER స్థానభ్రంశం_2,
    లోపల_2 ULONG బైట్లు_2,
    లోపల_2 PCHAR బఫరు_2,
    బహిర్గతం_2 గుర్తులేని* Errno
   )
{
	తిరిగి ఖండం_Io(Handle, స్థానభ్రంశం_2, బైట్లు_2, బఫరు_2, TRUE, Errno);
}



__inline
BOOLEAN
సమితి_Part_Type(
    లోపల_2 HANDLE Handle,
    లోపల_2 UCHAR Type
   )
{
	IO_స్థితి_ఖండం Io_స్థితి_ఖండం;
	తిరిగి స్థితి_SUCCESS == NtDeviceIoControlFile(
												   Handle, ఖాళీ, ఖాళీ, ఖాళీ, &Io_స్థితి_ఖండం, IOCTL_DISK_సమితి_విభజన_INFO,
												   &Type, పరిమాణం(Type),
												   ఖాళీ, 0);
}



//--------------------- interface part

//
// Interface functions.
// Is_mounted is set to 1 if the device is mounted, 0 otherwise
//

errcode_t
ext2fs_check_if_mounted(స్థిరరాశి అక్షరం *file, పూర్ణాంకం *mount_flags)
{
	HANDLE h;
	BOOLEAN Readonly;

	*mount_flags = 0;

	ఒకవేళ(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, ఖాళీ))
	{
		తిరిగి 0;
	}


	__try{
		*mount_flags &= _IsMounted(h) ? EXT2_MF_MOUNTED : 0;
	}
	__finally{
		_CloseDisk(h);
	}

	తిరిగి 0;
}



//
// Returns the number of blocks in a partition
//

స్థిర __int64 Fs_కొలత = 0;
స్థిర అక్షరం knowndevice[1024] = "";


errcode_t
ext2fs_get_device_కొలత(స్థిరరాశి అక్షరం *file, పూర్ణాంకం blocksize,
				 blk_t *retblocks)
{
	HANDLE h;
	BOOLEAN Readonly;

	ఒకవేళ((0 == Fs_కొలత) || (0 != strcmp(knowndevice, file)))
	{

		ఒకవేళ(!_Ext2OpenDevice(file, TRUE, &h, &Readonly, ఖాళీ))
		{
			తిరిగి 0;
		}


		__try{

			//
			// Get size
			//

			Get_Device_కొలత(h, &Fs_కొలత);
			strcpy(knowndevice, file);
		}
		__finally{
			_CloseDisk(h);
		}

	}

	*retblocks = (blk_t)(గుర్తులేని __int64)(Fs_కొలత / blocksize);
	UNREFERENCED_PARAMETER(file);
	తిరిగి 0;
}






//
// Table elements
//


స్థిర
errcode_t
nt_open(స్థిరరాశి అక్షరం *name, పూర్ణాంకం flags, io_channel *channel)
{
	io_channel      io = ఖాళీ;
	PNT_వ్యక్తిగత_దత్తాంశం Nt_దత్తాంశం = ఖాళీ;
	errcode_t Errno = 0;

	//
	// Check name
	//

	ఒకవేళ (ఖాళీ == name)
	{
		తిరిగి EXT2_ET_BAD_DEVICE_NAME;
	}

	__try{

		//
		// Allocate channel handle
		//

		io = (io_channel) జ్ఞాపకం_ఇవ్వు(పరిమాణం(నిర్మాణం struct_io_channel));

		ఒకవేళ (ఖాళీ == io)
		{
			Errno = ENOMEM;
			__leave;
		}

		Rtl_Zero_జ్ఞాపకం(io, పరిమాణం(నిర్మాణం struct_io_channel));
		io->magic = EXT2_ET_MAGIC_IO_CHANNEL;

		Nt_దత్తాంశం = (PNT_వ్యక్తిగత_దత్తాంశం)జ్ఞాపకం_ఇవ్వు(పరిమాణం(NT_వ్యక్తిగత_దత్తాంశం_2));

		ఒకవేళ (ఖాళీ == Nt_దత్తాంశం)
		{
			Errno = ENOMEM;
			__leave;
		}


		io->manager = nt_io_manager();
		io->name = జ్ఞాపకం_ఇవ్వు(అక్షరశ్రేణి_పొడవు(name) + 1);
		ఒకవేళ (ఖాళీ == io->name)
		{
			Errno = ENOMEM;
			__leave;
		}

		strcpy(io->name, name);
		io->వ్యక్తిగత_దత్తాంశం = Nt_దత్తాంశం;
		io->ఖండం_కొలత = 1024;
		io->read_error = 0;
		io->write_error = 0;
		io->refcount = 1;

		//
		// Initialize data
		//

		Rtl_Zero_జ్ఞాపకం(Nt_దత్తాంశం, పరిమాణం(NT_వ్యక్తిగత_దత్తాంశం_2));

		Nt_దత్తాంశం->magic = EXT2_ET_MAGIC_NT_IO_CHANNEL;
		Nt_దత్తాంశం->బఫరు_ఖండం_సంఖ్య = 0xffffffff;
		Nt_దత్తాంశం->బఫరు_కొలత = 1024;
		Nt_దత్తాంశం->బఫరు_2 = జ్ఞాపకం_ఇవ్వు(Nt_దత్తాంశం->బఫరు_కొలత);

		ఒకవేళ (ఖాళీ == Nt_దత్తాంశం->బఫరు_2)
		{
			Errno = ENOMEM;
			__leave;
		}

		//
		// Open it
		//

		ఒకవేళ(!_Ext2OpenDevice(name, (BOOLEAN)!BooleanFlagOn(flags, EXT2_FLAG_RW), &Nt_దత్తాంశం->Handle, &Nt_దత్తాంశం->OpenedReadonly, &Errno))
		{
			__leave;
		}


		//
		// get size
		//

		Get_Device_కొలత(Nt_దత్తాంశం->Handle, &Fs_కొలత);
		strcpy(knowndevice, name);


		//
		// Lock/dismount
		//

		ఒకవేళ(!NT_SUCCESS(తాళం_Drive(Nt_దత్తాంశం->Handle)) /*|| !NT_SUCCESS(_DismountDrive(NtData->Handle))*/)
		{
			Nt_దత్తాంశం->OpenedReadonly = TRUE;
		}

		//
		// Done
		//

		*channel = io;


	}
	__finally{

		ఒకవేళ(0 != Errno)
		{
			//
			// Cleanup
			//

			ఒకవేళ (ఖాళీ != io)
			{
				విడుదల(io->name);
				విడుదల(io);
			}

			ఒకవేళ (ఖాళీ != Nt_దత్తాంశం)
			{
				ఒకవేళ(ఖాళీ != Nt_దత్తాంశం->Handle)
				{
					_UnlockDrive(Nt_దత్తాంశం->Handle);
					_CloseDisk(Nt_దత్తాంశం->Handle);
				}

				విడుదల(Nt_దత్తాంశం->బఫరు_2);
				విడుదల(Nt_దత్తాంశం);
			}
		}
	}

	తిరిగి Errno;
}


//
// Close api
//

స్థిర
errcode_t
nt_close(io_channel channel)
{
	PNT_వ్యక్తిగత_దత్తాంశం Nt_దత్తాంశం = ఖాళీ;

	ఒకవేళ(ఖాళీ == channel)
	{
		తిరిగి 0;
	}

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_దత్తాంశం = (PNT_వ్యక్తిగత_దత్తాంశం) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(Nt_దత్తాంశం, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	ఒకవేళ (--channel->refcount > 0)
	{
		తిరిగి 0;
	}

	విడుదల(channel->name);
	విడుదల(channel);

	ఒకవేళ (ఖాళీ != Nt_దత్తాంశం)
	{
		ఒకవేళ(ఖాళీ != Nt_దత్తాంశం->Handle)
		{
			_DismountDrive(Nt_దత్తాంశం->Handle);
			_UnlockDrive(Nt_దత్తాంశం->Handle);
			_CloseDisk(Nt_దత్తాంశం->Handle);
		}

		విడుదల(Nt_దత్తాంశం->బఫరు_2);
		విడుదల(Nt_దత్తాంశం);
	}

	తిరిగి 0;
}



//
// set block size
//

స్థిర
errcode_t
nt_సమితి_blksize(io_channel channel, పూర్ణాంకం blksize)
{
	PNT_వ్యక్తిగత_దత్తాంశం Nt_దత్తాంశం = ఖాళీ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_దత్తాంశం = (PNT_వ్యక్తిగత_దత్తాంశం) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(Nt_దత్తాంశం, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	ఒకవేళ (channel->ఖండం_కొలత != blksize)
	{
		channel->ఖండం_కొలత = blksize;

		విడుదల(Nt_దత్తాంశం->బఫరు_2);
		Nt_దత్తాంశం->బఫరు_ఖండం_సంఖ్య = 0xffffffff;
		Nt_దత్తాంశం->బఫరు_కొలత = channel->ఖండం_కొలత;
		ASSERT(0 == (Nt_దత్తాంశం->బఫరు_కొలత % 512));

		Nt_దత్తాంశం->బఫరు_2 = జ్ఞాపకం_ఇవ్వు(Nt_దత్తాంశం->బఫరు_కొలత);

		ఒకవేళ (ఖాళీ == Nt_దత్తాంశం->బఫరు_2)
		{
			తిరిగి ENOMEM;
		}

	}

	తిరిగి 0;
}


//
// read block
//

స్థిర
errcode_t
nt_read_blk(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
			       పూర్ణాంకం లెక్క, శూన్య *buf)
{
	PVOID బఫరు_గమ్యం_Read;
	ULONG కొలత_గమ్యం_Read;
	ULONG కొలత_2;
	LARGE_INTEGER స్థానభ్రంశం_2;
	PNT_వ్యక్తిగత_దత్తాంశం Nt_దత్తాంశం = ఖాళీ;
	గుర్తులేని Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_దత్తాంశం = (PNT_వ్యక్తిగత_దత్తాంశం) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(Nt_దత్తాంశం, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	//
	// If it's in the cache, use it!
	//

	ఒకవేళ ((1 == లెక్క) &&
		(ఖండం == Nt_దత్తాంశం->బఫరు_ఖండం_సంఖ్య) &&
		(Nt_దత్తాంశం->బఫరు_ఖండం_సంఖ్య != 0xffffffff))
	{
		జ్ఞాపకం_నకలు(buf, Nt_దత్తాంశం->బఫరు_2, channel->ఖండం_కొలత);
		తిరిగి 0;
	}

	కొలత_2 = (లెక్క < 0) ? (ULONG)(-లెక్క) : (ULONG)(లెక్క * channel->ఖండం_కొలత);

	స్థానభ్రంశం_2.QuadPart = ఖండం * channel->ఖండం_కొలత;

	//
	// If not fit to the block
	//

	ఒకవేళ(కొలత_2 <= Nt_దత్తాంశం->బఫరు_కొలత)
	{
		//
		// Update the cache
		//

		Nt_దత్తాంశం->బఫరు_ఖండం_సంఖ్య = ఖండం;
		బఫరు_గమ్యం_Read = Nt_దత్తాంశం->బఫరు_2;
		కొలత_గమ్యం_Read = Nt_దత్తాంశం->బఫరు_కొలత;
	}
	లేకపోతే
	{
		కొలత_గమ్యం_Read = కొలత_2;
		బఫరు_గమ్యం_Read = buf;
		ASSERT(0 == (కొలత_గమ్యం_Read % channel->ఖండం_కొలత));
	}

	ఒకవేళ(!_RawRead(Nt_దత్తాంశం->Handle, స్థానభ్రంశం_2, కొలత_గమ్యం_Read, బఫరు_గమ్యం_Read, &Errno))
	{

		ఒకవేళ (channel->read_error)
		{
			తిరిగి (channel->read_error)(channel, ఖండం, లెక్క, buf,
					       కొలత_2, 0, Errno);
		}
		లేకపోతే
		{
			తిరిగి Errno;
		}
	}


	ఒకవేళ(బఫరు_గమ్యం_Read != buf)
	{
		ASSERT(కొలత_2 <= కొలత_గమ్యం_Read);
		జ్ఞాపకం_నకలు(buf, బఫరు_గమ్యం_Read, కొలత_2);
	}

	తిరిగి 0;
}


//
// write block
//

స్థిర
errcode_t
nt_write_blk(io_channel channel, గుర్తులేని దీర్ఘ ఖండం,
				పూర్ణాంకం లెక్క, స్థిరరాశి శూన్య *buf)
{
	ULONG కొలత_గమ్యం_Write;
	LARGE_INTEGER స్థానభ్రంశం_2;
	PNT_వ్యక్తిగత_దత్తాంశం Nt_దత్తాంశం = ఖాళీ;
	గుర్తులేని Errno = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_దత్తాంశం = (PNT_వ్యక్తిగత_దత్తాంశం) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(Nt_దత్తాంశం, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	ఒకవేళ(Nt_దత్తాంశం->OpenedReadonly)
	{
		తిరిగి EACCES;
	}

	ఒకవేళ (లెక్క == 1)
	{
		కొలత_గమ్యం_Write = channel->ఖండం_కొలత;
	}
	లేకపోతే
	{
		Nt_దత్తాంశం->బఫరు_ఖండం_సంఖ్య = 0xffffffff;

		ఒకవేళ (లెక్క < 0)
		{
			కొలత_గమ్యం_Write = (ULONG)(-లెక్క);
		}
		లేకపోతే
		{
			కొలత_గమ్యం_Write = (ULONG)(లెక్క * channel->ఖండం_కొలత);
		}
	}


	ASSERT(0 == (కొలత_గమ్యం_Write % 512));
	స్థానభ్రంశం_2.QuadPart = ఖండం * channel->ఖండం_కొలత;

	ఒకవేళ(!_RawWrite(Nt_దత్తాంశం->Handle, స్థానభ్రంశం_2, కొలత_గమ్యం_Write, buf, &Errno))
	{
		ఒకవేళ (channel->write_error)
		{
			తిరిగి (channel->write_error)(channel, ఖండం, లెక్క, buf,
						కొలత_గమ్యం_Write, 0, Errno);
		}
		లేకపోతే
		{
			తిరిగి Errno;
		}
	}


	//
	// Stash a copy.
	//

	ఒకవేళ(కొలత_గమ్యం_Write >= Nt_దత్తాంశం->బఫరు_కొలత)
	{
		Nt_దత్తాంశం->బఫరు_ఖండం_సంఖ్య = ఖండం;
		జ్ఞాపకం_నకలు(Nt_దత్తాంశం->బఫరు_2, buf, Nt_దత్తాంశం->బఫరు_కొలత);
	}

	Nt_దత్తాంశం->Written = TRUE;

	తిరిగి 0;

}



//
// Flush data buffers to disk.  Since we are currently using a
// write-through cache, this is a no-op.
//

స్థిర
errcode_t
nt_flush(io_channel channel)
{
	PNT_వ్యక్తిగత_దత్తాంశం Nt_దత్తాంశం = ఖాళీ;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	Nt_దత్తాంశం = (PNT_వ్యక్తిగత_దత్తాంశం) channel->వ్యక్తిగత_దత్తాంశం;
	EXT2_CHECK_MAGIC(Nt_దత్తాంశం, EXT2_ET_MAGIC_NT_IO_CHANNEL);

	ఒకవేళ(Nt_దత్తాంశం->OpenedReadonly)
	{
		తిరిగి 0; // EACCESS;
	}


	//
	// Flush file buffers.
	//

	_FlushDrive(Nt_దత్తాంశం->Handle);


	//
	// Test and correct partition type.
	//

	ఒకవేళ(Nt_దత్తాంశం->Written)
	{
		సమితి_Part_Type(Nt_దత్తాంశం->Handle, 0x83);
	}

	తిరిగి 0;
}


