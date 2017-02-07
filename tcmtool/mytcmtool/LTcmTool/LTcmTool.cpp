#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "TCMTOOLDlg.h"

CTCMTOOLDlg	g_dlg;

int ShowHelp()
{
	printf("\r\nTcm Tool 3.0.0.2");
	printf("\r\n====================================");
	printf("\r\n/m    menu model");
	printf("\r\n/e    tcm enable");
	printf("\r\n/d    tcm disable");
	printf("\r\n/f    owner force clear");
	printf("\r\n/g    get tcm info");
	printf("\r\n/t    take owner ship");
	printf("\r\n/r    read pcr");
	printf("\r\n/s    startup and selftest") ;
	printf("\r\n====================================\r\n");	
	return 0;
}

	
int tcm_enable()
{
	int iret = 0;
	iret = g_dlg.OnButtonEnable();
	return iret;
}

int tcm_disable()
{
	int iret = 0;
	iret = g_dlg.OnButtonDisable();
	return iret;
}

int tcm_forceclear()
{
	int iret = 0;
	iret = g_dlg.OnButtonForceclear();
	return iret;
}

int tcm_gettcminfo()
{
	int iret = 0;
	iret = g_dlg.OnButtonGettcminfo();
	return iret;
}

int tcm_takeownership()
{
	int iret = 0;
	iret = g_dlg.OnTakeOwnerShip();
	return iret;
}

int tcm_readpcr()
{
	int iret = 0;
	iret = g_dlg.OnButtonPcrread();
	return iret;
}

int tcm_startupandselftest()
{
	int iret = 0;
	iret = g_dlg.OnButtonStartAndSelftest();
	return iret;
}

int MenuModel()
{
	int iret = 0;

	while(1)
	{
		char szInPut[128];
		memset( szInPut, 0, 128 );

		printf("\r\nTcm Tool 3.0.0.2");
		printf("\r\n====================================");
		printf("\r\n[1]    tcm enable");
		printf("\r\n[2]    tcm disable");
		printf("\r\n[3]    owner force clear");
		printf("\r\n[4]    get tcm info");
		printf("\r\n[5]    take owner ship");
		printf("\r\n[6]    read pcr");
		printf("\r\n[7]    startup and selftest") ;
		printf("\r\n[8]    exit") ;
		printf("\r\n====================================\r\n");	

		scanf("%s",szInPut);
		if( strlen(szInPut)!=1 )
		{
			continue;
		}

		switch(szInPut[0])
		{
		case '1':
			{
				iret = tcm_enable();
				break;
			}
		case '2':
			{
				iret = tcm_disable();
				break;
			}
		case '3':
			{
				iret = tcm_forceclear();
				break;
			}
		case '4':
			{
				iret = tcm_gettcminfo();
				break;
			}
		case '5':
			{
				iret = tcm_takeownership();
				break;
			}
		case '6':
			{
				iret = tcm_readpcr();
				break;
			}
		case '7':
			{
				iret = tcm_startupandselftest();
				break;
			}
		case '8':
			{
				goto EXIT;
			}
		}
	}
EXIT:
	return iret;
}

#include "tcm.h"
#include "dev.h"
#include "itcm.h"
#include "bsg.h"

int mydebug()
{
	WinDevice dev;
	int status=0;
	BYTE out[]={0x00,0xc4,0x00,0x00,0x00,0x7f,0x00,0x00,
				0x00,0x00,0x00,0x00,0x00,0x0b,0x00,0x06,
				0x00,0x01,0x00,0x00,0x00,0x04,0x00,0x00,
				0x01,0x00,0x00,0x00,0x00,0x41,0x04,0x08,
				0xc8,0x9b,0xa5,0xda,0x63,0x73,0x1b,0xbc,
				0xe0,0x69,0x5e,0xeb,0x87,0x46,0x6d,0x90,
				0x85,0x88,0xb1,0x3a,0xd4,0xbf,0x8f,0x44,
				0xb9,0xb9,0x31,0x6b,0x6a,0x73,0x2d,0x90,
				0x4f,0x4b,0xb1,0x2a,0x7f,0x30,0x98,0x5d,
				0x60,0xb6,0x5f,0xaa,0x94,0x8a,0xb9,0x0a,
				0x6b,0x81,0xfd,0x24,0xb3,0x64,0x02,0x56,
				0x30,0x12,0xf8,0x21,0x1b,0x42,0xbc,0xd7,
				0xa2,0x6f,0x9a,0xb4,0x74,0x89,0x78,0xbb,
				0xa3,0x83,0xc6,0xdf,0xc9,0x3c,0x0f,0xd3,
				0x67,0xe3,0x3e,0x1d,0x0b,0x13,0xbe,0xd3,
				0x06,0x61,0xef,0x8b,0x5a,0xc0,0x55};



	int outsize=127;
	TCM_PUBKEY pubkey;
	BYTE checksum[32];
	
	memset(checksum, 0x00, 32);
	memset(&pubkey, 0, sizeof(pubkey));
	
	status = dev.VerifySimple(
			out,
			outsize,
			TCM_ORD_ReadPubek,
			BSG_STRUCT_OUT(BSG::TCM_PUBKEY, &pubkey),
			BSG_DIGEST_OUT(checksum),
			0);
	return status;
}

int main( int argc, char** argv)
{
	int iret = -1;

	if( !g_dlg.OnInitDialog() )
	{
		return iret;
	}

	if( argc == 1 )
	{
		iret = ShowHelp();
	}
	else if( argc == 2 )
	{
		if( strcmp(argv[1],"/m") == 0 )	//menu model
		{
			iret = MenuModel();
		}
		else if( strcmp(argv[1],"/e") == 0 ) //enable
		{
			iret = tcm_enable();
		}
		else if( strcmp(argv[1],"/d") == 0 ) //disable
		{
			iret = tcm_disable();
		}
		else if( strcmp(argv[1],"/f") == 0 ) //force clear
		{
			iret = tcm_forceclear();
		}
		else if( strcmp(argv[1],"/g") == 0 ) //get tcm info
		{
			iret = tcm_gettcminfo();
		}
		else if( strcmp(argv[1],"/t") == 0 ) // take owner ship
		{
			iret = tcm_takeownership();
		}
		else if( strcmp(argv[1],"/r") == 0 ) //read pcr
		{
			iret = tcm_readpcr();
		}
		else if( strcmp(argv[1],"/s") == 0 ) //startup and selftest
		{
			iret = tcm_startupandselftest();
		}
		else if( strcmp(argv[1],"/debug") == 0 ) //my debug
		{
			iret = mydebug();
		}
	}
	else
	{
		printf("\r\nParam Is Error");
	}

	return iret;
}

