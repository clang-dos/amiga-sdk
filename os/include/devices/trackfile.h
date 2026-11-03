#ifndef DEVICES_TRACKFILE_H
#define DEVICES_TRACKFILE_H
/*
    trackfile.device: disk image files as trackdisk units (AmigaOS 3.2).
    Part of the clang-dos Amiga SDK; written for it.
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef EXEC_DEVICES_H
#include <exec/devices.h>
#endif
#ifndef UTILITY_TAGITEM_H
#include <utility/tagitem.h>
#endif
#ifndef DOS_DOS_H
#include <dos/dos.h>
#endif
#ifndef DEVICES_TRACKDISK_H
#include <devices/trackdisk.h>
#endif

/* Unit numbers */
#define TFUNIT_CONTROL          (-1)    /* control unit: no medium */
#define TFSU_NextAvailableUnit  (-1)    /* TFStartUnitTagList(): pick one */
#define TFGUD_AllUnits          (-1)    /* TFGetUnitData(): every unit */
#define TFEFS_Unsupported       (-1)    /* TFExamineFileSize(): not an image */

/* Tags */
#define TF_Dummy                (TAG_USER + 0x20200400)
#define TF_ImageFileName        (TF_Dummy + 1)  /* (STRPTR) image to insert */
#define TF_ImageFileHandle      (TF_Dummy + 2)  /* (BPTR) ... or an open file */
#define TF_WriteProtected       (TF_Dummy + 3)  /* (BOOL) */
#define TF_DriveType            (TF_Dummy + 4)  /* (ULONG) DRIVE3_5 etc */
#define TF_Timeout              (TF_Dummy + 5)  /* (ULONG) seconds before closing the file */
#define TF_EnableChecksums      (TF_Dummy + 6)  /* (BOOL) keep track checksums */
#define TF_TagItemFailed        (TF_Dummy + 7)  /* (struct TagItem **) the tag that failed */
#define TF_MaxCacheMemory       (TF_Dummy + 8)  /* (ULONG) bytes */
#define TF_EnableUnitCache      (TF_Dummy + 9)  /* (BOOL) */
#define TF_PrefillUnitCache     (TF_Dummy + 10) /* (BOOL) read the whole image at once */

#define TF_MINIMUM_CACHE_SIZE   46000

/* Error codes */
#define TFERROR_Base            (-202040)
#define TFERROR_UnitBusy        (TFERROR_Base - 1)
#define TFERROR_OutOfMemory     (TFERROR_Base - 2)
#define TFERROR_UnitNotFound    (TFERROR_Base - 3)
#define TFERROR_AlreadyInUse    (TFERROR_Base - 4)
#define TFERROR_UnitNotActive   (TFERROR_Base - 5)
#define TFERROR_InvalidFile     (TFERROR_Base - 6)
#define TFERROR_InvalidFileSize (TFERROR_Base - 7)
#define TFERROR_NoFileGiven     (TFERROR_Base - 8)
#define TFERROR_Aborted         (TFERROR_Base - 9)
#define TFERROR_InvalidDriveType (TFERROR_Base - 10)
#define TFERROR_ProcessFailed   (TFERROR_Base - 11)
#define TFERROR_NoMediumPresent (TFERROR_Base - 12)
#define TFERROR_ReadOnlyVolume  (TFERROR_Base - 13)
#define TFERROR_ReadOnlyFile    (TFERROR_Base - 14)
#define TFERROR_DuplicateDisk   (TFERROR_Base - 15)
#define TFERROR_DuplicateVolume (TFERROR_Base - 16)
#define TFERROR_Denied          (TFERROR_Base - 17)
#define TFERROR_NotSupported    (TFERROR_Base - 18)

/* 64-bit checksum of a disk image */
struct TrackFileChecksum
{
    ULONG tfc_high;
    ULONG tfc_low;
};

/* What TFGetUnitData() returns: one per unit, in a chain */
struct TrackFileUnitData
{
    struct TrackFileUnitData *tfud_Next;
    ULONG                     tfud_Size;            /* size of this structure */
    LONG                      tfud_UnitNumber;
    ULONG                     tfud_DriveType;
    STRPTR                    tfud_DeviceName;
    STRPTR                    tfud_FileName;        /* image file, if any */
    BOOL                      tfud_IsActive;
    BOOL                      tfud_IsWritable;
    BOOL                      tfud_MediumIsPresent;
    BOOL                      tfud_IsBusy;
    BOOL                      tfud_ChecksumsEnabled;
    struct TrackFileChecksum  tfud_Checksum;
    BOOL                      tfud_VolumeValid;     /* the next two are filled in */
    TEXT                      tfud_VolumeName[32];
    struct DateStamp          tfud_VolumeDate;
    ULONG                     tfud_FileSysSignature;
    ULONG                     tfud_BootBlockChecksum;
    BOOL                      tfud_CacheEnabled;
    ULONG                     tfud_CacheAccesses;
    ULONG                     tfud_CacheMisses;
};

#endif /* DEVICES_TRACKFILE_H */
