
#ifndef STRUCT_SMARTIOCTL_INCLUDED
#define STRUCT_SMARTIOCTL_INCLUDED

#include <pshpack1.h>

//---------------------------------------------------------------------
// The following structure defines the structure of a Drive Attribute
//---------------------------------------------------------------------
typedef	struct	_DRIVEATTRIBUTE {
	BYTE	bAttrID;		// Identifies which attribute
	WORD	wStatusFlags;	// see bit definitions below
	BYTE	bAttrValue;		// Current normalized value
	BYTE	bWorstValue;	// How bad has it ever been?
	BYTE	bRawValue[6];	// Un-normalized value
	BYTE	bReserved;		// ...
} DRIVEATTRIBUTE, *PDRIVEATTRIBUTE, *LPDRIVEATTRIBUTE;

//---------------------------------------------------------------------
// The following structure defines the structure of a Warranty Threshold
//---------------------------------------------------------------------
typedef	struct	_ATTRTHRESHOLD {
	BYTE	bAttrID;			// Identifies which attribute
	BYTE	bWarrantyThreshold;	// Triggering value
	BYTE	bReserved[10];		// ...
} ATTRTHRESHOLD, *PATTRTHRESHOLD, *LPATTRTHRESHOLD;

//---------------------------------------------------------------------
// The following struct defines the interesting part of the IDENTIFY
// buffer:
//---------------------------------------------------------------------
typedef struct _IDSECTOR
{
	USHORT	wGeneralConfiguration;		//0			//15bit:0=ATA�f�o�C�X�A7bit:1=�����[�o�u�����f�B�A�E�f�o�C�X
	USHORT	wObsolute1;					//1
	USHORT	wSpecificConfiguration;		//2
	USHORT	wObsolute2;					//3
	USHORT	wRetired1[2];				//4-5
	USHORT	wObsolute3;					//6
	ULONG	ulReservedForCompactFlash;	//7-8
	USHORT	wRetired2;					//9
	CHAR	sSerialNumber[20];		//10-19		//�V���A���i���o�[
	ULONG	ulRetired3;					//20-21
	USHORT	wObsolute4;					//22
	CHAR	sFirmwareRev[8];			//23-26		//�t�@�[���E�G�A�E�o�[�W����
	CHAR	sModelNumber[40];			//27-46		//���f����
	USHORT	wMaxNumPerInterupt;			//47
	USHORT	wReserved1;					//48
	USHORT	wCapabilities1;				//49		//8bit:1=DMA�Ή��@9bit:1=LBA�Ή��@10bit:1=IORDY�����@11bit:1=IORDY�Ή�/0=�s��
	USHORT	wCapabilities2;				//50
	ULONG	ulObsolute5;				//51-52
	USHORT	wField88and7063;			//53		//2bit:1=88���[�h�ڏ�񗘗p�\�@1bit:1=70:64���[�h�ڏ�񗘗p�\
	USHORT	wObsolute6[5];				//54-58
	USHORT	wMultSectorStuff;			//59
	ULONG	ulTotalAddressableSectors;	//60-61
	USHORT	wObsolute7;					//62
	USHORT	wMultiWordDMA;				//63		//0bit:1=mode0�Ή��@1bit:1=mode1�Ή��@2bit:1=mode2�Ή��@8bit:1=mode0�I���@9bit:mode1�I���@10bit:mode2�I��
	USHORT	wPIOMode;					//64
	USHORT	wMinMultiwordDMACycleTime;	//65
	USHORT	wRecommendedMultiwordDMACycleTime;	//66
	USHORT	wMinPIOCycleTimewoFlowCtrl;	//67
	USHORT	wMinPIOCycleTimeWithFlowCtrl;	//68
	USHORT	wReserved2[6];				//69-74
	USHORT	wQueueDepth;				//75
	USHORT	wReserved3[4];				//76-79
	USHORT	wMajorVersion;				//80		//4bit:1=ATA/ATAPI-4�Ή��@5bit:1=ATA/ATAPI-5�Ή��@6bit:1=ATA/ATAPI-6�Ή��@7bit:1=ATA/ATAPI-7�Ή�
	USHORT	wMinorVersion;				//81
	USHORT	wCommandSetSupported1;		//82		//0bit:1=SMART�Ή��@1bit:SecurityMode�Ή��@....
	USHORT	wCommandSetSupported2;		//83		//0bit:1=DOWNLOAD Microcode�Ή��@....
	USHORT	wCommandSetSupported3;		//84		//0bit:1=SMART error logging�Ή��@1bit:1=SMART self-test�Ή��@....
	USHORT	wCommandSetEnable1;			//85		//0bit:1=SMART���p�\�@....
	USHORT	wCommandSetEnable2;			//86		//0bit:1=DOWNLOAD Microcode���p�\�@....
	USHORT	wCommandSetDefault;			//87
	USHORT	wUltraDMAMode;				//88		//0bit:1=UltraDMAmode0�Ή��@1bit:1=UltraDMAmode1�Ή��@....
	USHORT	wTimeReqForSecurityErase;	//89
	USHORT	wTimeReqForEnhancedSecure;	//90
	USHORT	wCurrentPowerManagement;	//91
	USHORT	wMasterPasswordRevision;	//92
	USHORT	wHardwareResetResult;		//93
	USHORT	wAcoustricmanagement;		//94
	USHORT	wStreamMinRequestSize;		//95
	USHORT	wStreamingTimeDMA;			//96
	USHORT	wStreamingAccessLatency;	//97
	ULONG	ulStreamingPerformance;		//98-99
	USHORT	pwMaxUserLBA[4];			//100-103
	USHORT	wStremingTimePIO;			//104
	USHORT	wReserved4;					//105
	USHORT	wSectorSize;				//106
	USHORT	wInterSeekDelay;			//107
	USHORT	wIEEEOUI;					//108
	USHORT	wUniqueID3;					//109
	USHORT	wUniqueID2;					//110
	USHORT	wUniqueID1;					//111
	USHORT	wReserved5[4];				//112-115
	USHORT	wReserved6;					//116
	ULONG	ulWordsPerLogicalSector;	//117-118
	USHORT	wReserved7[8];				//119-126
	USHORT	wRemovableMediaStatus;		//127
	USHORT	wSecurityStatus;			//128
	USHORT	pwVendorSpecific[31];		//129-159
	USHORT	wCFAPowerMode1;				//160
	USHORT	wReserved8[15];				//161-175
	CHAR	pszCurrentMediaSerialNo[60];//176-205
	USHORT	wReserved9[49];				//206-254
	USHORT	wIntegrityWord;				//255		//0-7bit:Signature�@8-15bit:Checksum
} IDSECTOR, *PIDSECTOR;

//---------------------------------------------------------------------
// Status Flags Values
//---------------------------------------------------------------------
#define	PRE_FAILURE_WARRANTY		0x1
#define	ON_LINE_COLLECTION			0x2
#define	PERFORMANCE_ATTRIBUTE		0x4
#define	ERROR_RATE_ATTRIBUTE		0x8
#define	EVENT_COUNT_ATTRIBUTE		0x10
#define	SELF_PRESERVING_ATTRIBUTE	0x20

#define	NUM_ATTRIBUTE_STRUCTS		 30

#include <poppack.h>

#endif //STRUCT_SMARTIOCTL_INCLUDED

