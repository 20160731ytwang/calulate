// TCMTOOLDlg.cpp : implementation file
//

#include "stdafx.h"

//#include <windows.h>
//#include <process.h>

#include "TCMTOOLDlg.h"

#include "tcm.h"
#include "dev.h"
#include "itcm.h"
#include "bsg.h"

#include "tcm_ecc.h" 
#include "tcm_hash.h"

CTCMTOOLDlg::CTCMTOOLDlg()
{
	m_bZteicFlag = TRUE;//不使用取ID判断 了
	m_bFlag = FALSE;
	m_strOperLog.Empty();
}

CTCMTOOLDlg::~CTCMTOOLDlg()
{
	OnClose();
	if(m_TcmDev)
	{
		delete m_TcmDev;
	}
}

void CTCMTOOLDlg::writeLogMsg(char* strMsg)
{
	FILE *fp = NULL;
	time_t t;
	struct tm *ptm = NULL;
	char * wrMsg = NULL;
	int iLen = 0;

	char strPath[TCM_TOOL_MAXPATH+1];
	char strTime[TCM_TOOL_MAXTIME+1];	

	memset( strPath, 0, TCM_TOOL_MAXPATH+1 );
	memset( strTime, 0, TCM_TOOL_MAXTIME+1 );

	strcpy( strPath, "./tcmlog.log" );

	fp = fopen( strPath,"a+");
	if( !fp )
	{
		return ;
	}
	
	time(&t);
	
	ptm = localtime( &t );  
	sprintf( strTime,"%04d-%02d-%02d %02d:%02d:%02d\n", 
		ptm->tm_year+1900, ptm->tm_mon+1, 
		ptm->tm_mday, ptm->tm_hour,
		ptm->tm_min, ptm->tm_sec);

	
	iLen = strlen(strTime)+64;
	if( strMsg != NULL )
	{
		iLen += strlen(strMsg);
	}
	wrMsg = new char[iLen];
	if( wrMsg == NULL )
	{
		fclose(fp);
		return;
	}
	memset( wrMsg, 0, iLen );
	if( strMsg == NULL )
	{
		sprintf( wrMsg, "\n%s\n", strTime );
	}
	else
	{
		sprintf( wrMsg, "\n%s%s\n", strTime, strMsg );
	}

	fwrite( wrMsg, strlen(wrMsg),1,fp);
	
	fflush(fp);
	fclose(fp);

	delete []wrMsg;
	return;
}

BOOL CTCMTOOLDlg::OnInitDialog()
{
	
	writeLogMsg(TCM_RUNNING_INFO);

	// TODO: Add extra initialization here
	m_TcmDev = new WinDevice;
	int nRst = m_TcmDev->Init(WinDevice::FLAG_TEST, NULL);
	
	m_isEKExist = FALSE;
	m_Startup = FALSE;
	
	
	//Add by Kevin.Luo begin
	if( nRst )
	{
		printf( "Load TCM Driver Failed!","Error");
		writeLogMsg("ERROR: Load TCM Driver Failed!\n");
		m_bZteicFlag = FALSE;

		return FALSE ;
		
	}

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

INT32 CTCMTOOLDlg::StartUp()
{
	INT32 status = 0;
	status = m_TcmDev->SubmitSimple(TCM_ORD_Startup, 2, TCM_ST_CLEAR);

	status =show_operation_startup(CMD_STRATUP_STCLEAR, status);
	m_Startup = TRUE;
	return status;
}

INT32 CTCMTOOLDlg::SelfTest()
{
	INT32 status = 0;
	status = m_TcmDev->SubmitSimple(TCM_ORD_SelfTestFull, 0, 0);
	if(status != 0)
	{
		return status;
	}

	return 0;
}

INT32 CTCMTOOLDlg::ForceClear()
{
	INT32 status = 0;
	status = m_TcmDev->SubmitSimple(TCM_ORD_ForceClear, 0 ,0);
	if(status != 0)
	{
		return status;
	} 
	return 0;
}


INT32 CTCMTOOLDlg::PhysicalEnable()
{
	INT32 status = 0;
	status = m_TcmDev->SubmitSimple(TCM_ORD_PhysicalEnable, 0 ,0);
	if(status != 0)
	{
		return status;
	}
	return 0;
	
}

INT32 CTCMTOOLDlg::PhysicalDisable()
{
	INT32 status = 0;
	status = m_TcmDev->SubmitSimple(TCM_ORD_PhysicalDisable, 0 ,0);
	if(status != 0)
	{
		return status;
	}
	return 0;
}

INT32 CTCMTOOLDlg::PhysicalSetDeactivate(int deactivate)
{
	INT32 status = 0;
	status = m_TcmDev->SubmitSimple(TCM_ORD_PhysicalSetDeactivated, 1, deactivate);
	if(status != 0)
	{	
		return status;
	}
	return 0;
}

INT32 CTCMTOOLDlg::ReadPubEK(BYTE* outPubKey)
{
	int status;
	int insize = 0;
	BYTE antireply[32];
	memset(antireply, 0x02, 32);
	
	BYTE* out = NULL;
	int outsize = 0;
	BYTE *indata = m_TcmDev->BuildSimple(
		&insize,
		TCM_ORD_ReadPubek,
		BSG_DIGEST(antireply),
		0);
	status = m_TcmDev->Submit(indata, insize, &out, &outsize);
	delete [] indata;
	if(status == 0)
	{
		TCM_PUBKEY pubkey;
		memset(&pubkey, 0, sizeof(pubkey));
		BYTE checksum[32];
		memset(checksum, 0x00, 32);
		status = m_TcmDev->VerifySimple(
			out,
			outsize,
			TCM_ORD_ReadPubek,
			BSG_STRUCT_OUT(BSG::TCM_PUBKEY, &pubkey),
			BSG_DIGEST_OUT(checksum),
			0);
		if( outPubKey )
			memcpy(outPubKey,pubkey.pubKey.key,pubkey.pubKey.keyLength);
		BSG::Destroy(BSG::TCM_PUBKEY, &pubkey);
		delete [] out;	
	}
	else if(status == TCM_NO_ENDORSEMENT)
	{
		m_isEKExist = FALSE;
		status = 0;
		//show_operation(CMD_READPUBEK, status);
		return status;
	}
	else
	{
		//	show_operation(CMD_READPUBEK, status);
		return -1;
	}
	
	
	if(status == 0)
	{
		//	show_operation(CMD_READPUBEK, status);
		m_isEKExist = TRUE;
	}
	else if(status == TCM_NO_ENDORSEMENT)
	{
		m_isEKExist = FALSE;
		status = 0;
		//	show_operation(CMD_READPUBEK, status);
	}
	else
	{
		//	show_operation(CMD_READPUBEK, status);
		return status;
	}
	
	return status;	
}

INT32 CTCMTOOLDlg::CreateEK()
{
	UINT32 status = 0;
	TCM_KEY_PARMS KeyParam = {0};
	KeyParam.algorithmID = TCM_ALG_ECC;
	KeyParam.encScheme = TCM_ES_ECC_GB;
	KeyParam.sigScheme = TCM_SS_NONE;
	KeyParam.parmSize = 4;
	KeyParam.parms = new BYTE[KeyParam.parmSize];
	BSG::Pack32(KeyParam.parms, KEY_SIZE_256*8);
	
	BYTE nonce[32];
	memset(nonce, 0x11, 32);
	
	BYTE revokeAuth[32] = {0x59,0xcd,0x9c,0x63,0xfd,0xd1,0x86,0xb3,0x41,0xd1,0x20,0x1b,0x92,0x0f,0xe1,0x34,0x4b,0xc1,0x1b,0x49,0x87,0xee,0x45,0xfc,0xbf,0x67,0x4c,0x71,0xb5,0x25,0x57,0xed};
	
	BOOL generateReset = FALSE;
	int insize = 0;
	BYTE *indata = m_TcmDev->BuildSimple(&insize,
		TCM_ORD_CreateRevocableEK,
		BSG_DIGEST(nonce),
		BSG_STRUCT(BSG::TCM_KEY_PARMS, &KeyParam),
		BSG_U8(generateReset),
		BSG_DIGEST(revokeAuth),
		0);
	delete [] KeyParam.parms;
	BYTE *outdata = NULL;
	int outsize = 0;
	status = m_TcmDev->Submit(indata, insize, &outdata, &outsize);
	delete [] indata;
	if(status == 0)
	{	
		TCM_PUBKEY pub;
		memset(&pub, 0, sizeof(pub));
		BYTE checksum[32];
		
		status = m_TcmDev->VerifySimple(
			outdata,
			outsize,
			TCM_ORD_CreateRevocableEK,
			BSG_STRUCT_OUT(BSG::TCM_PUBKEY, &pub),
			BSG_DIGEST_OUT(&checksum),
			BSG_DIGEST_OUT(revokeAuth), 0);
		BSG::Destroy(BSG::TCM_PUBKEY, &pub);
		delete [] outdata;
	}
	else
	{
		//	show_operation(CMD_CREATEEK, status);
		return -1;
	}
	if(status == 0)
	{
		//	show_operation(CMD_CREATEEK, status);
		m_isEKExist = TRUE;
	}
	else
	{
		//	show_operation(CMD_CREATEEK, status);
	}
	
	return 0;	
}


INT32 CTCMTOOLDlg::GetCapability(int cap, unsigned int sub, int* val, int* extra,BYTE* zteicID )
{
	int insize = 0;
	BYTE* in = NULL;
	int subsize = sizeof (sub);
	


	switch(cap) 
	{
	case TCM_CAP_FLAG:
		in = m_TcmDev->BuildSimple( &insize, TCM_ORD_GetCapability,
			BSG_U32(cap),BSG_U32(subsize),BSG_U32(sub),0);
		break;
	case TCM_CAP_VERSION:
		in = m_TcmDev->BuildSimple( &insize, TCM_ORD_GetCapability,
			BSG_U32(cap),BSG_U32(subsize),BSG_U32(sub),0);
		break;
	case TCM_CAP_PROPERTY:
		in = m_TcmDev->BuildSimple( &insize, TCM_ORD_GetCapability,
			BSG_U32(cap),BSG_U32(subsize),BSG_U32(sub),0);
		break;
	default:
		return -1;
	}
	

	BYTE* out = NULL;//此处的返回值由Submit内部负现分配空间，这里需要释放
	int outsize = 2048;
//	out = new BYTE[outsize]; //
	int status = m_TcmDev->Submit(in, insize, &out, &outsize);
	
	delete [] in;
	if (status)
	{
		delete [] out;
		out = NULL;
		return status;
	}
	int valsize = outsize - 14;
	switch(cap) 
	{
	case TCM_CAP_VERSION:
		status = m_TcmDev->VerifySimple(
			out,
			outsize,
			TCM_ORD_GetCapability,
			BSG_U32(valsize),
			BSG_U32_OUT(val),
			0);
		break;
	case TCM_CAP_FLAG:
		{
			int size = outsize - 14;
			BYTE buffer[22];
			memset(buffer, 0x00, 22);
			
			status = m_TcmDev->VerifySimple(out,outsize,TCM_ORD_GetCapability,
				BSG_SIZE32_DATA_OUT(buffer,&size),0);
			
			{
				int res = 0;
				int tem = 0;
				
				if(TCM_CAP_FLAG_VOLATILE != sub)
				{
					tem = m_TcmDev->UnpackArr(buffer+2, (size - 2), res);					
				}
				else
				{
					tem = m_TcmDev->UnpackArr(buffer+2, (size - 2), res);
				}
				memcpy(val, &tem, 4);
				
			}
			
		}
		break;
	case TCM_CAP_PROPERTY:
		{	
			//add by Kevin.Luo begin
			//purpose: Get ZTECI ID
			if(sub == TCM_CAP_PROP_MANUFACTURER)
			{
				
				status = m_TcmDev->VerifySimple(
					out,
					outsize,
					TCM_ORD_GetCapability,
					BSG_U32(valsize),
					BSG_DATA_OUT(zteicID,5),
					0);
			}//add by Kevin.Luo end
			else
			{
				status = m_TcmDev->VerifySimple(
					out,
					outsize,
					TCM_ORD_GetCapability,
					BSG_U32(valsize),
					BSG_U8_OUT(val),
					0);
			}
			break;
		}
		
	default:
		return -1;
	}
	
	delete [] out;
	return status;	
}

INT32 CTCMTOOLDlg::GetTCMStatus(TCM_Status *tcmstatus)
{
	UINT32 status = 0;
	int value;
	char buf[20];
	int *p = (int*)buf;
	BYTE temp;
	
	if(m_Startup == FALSE)
	{
		status = StartUp();
	}
	status = GetCapability(TCM_CAP_FLAG, TCM_CAP_FLAG_PERMANENT, &value, p);
	if(status == 0)
	{
		if((value&0x01) == 0x01)
			tcmstatus->enable = FALSE;
		else
			tcmstatus->enable = TRUE;
		
		if((value&0x04) == 0x04)
			tcmstatus->activate = FALSE;
		else
			tcmstatus->activate = TRUE;
	}
	else
	{
		//	show_operation(CMD_TESTTCMSTS, status);
		return status;
	}
	
	status = GetCapability(TCM_CAP_PROPERTY, TCM_CAP_PROP_OWNER, &value, p);
	if(status == 0)
	{
		if(value == 0)
			tcmstatus->Owner_install = FALSE;
		else
			tcmstatus->Owner_install = TRUE;
	}
	else
	{
		//	show_operation(CMD_TESTTCMSTS, status);
		return status;
	}
	
	status = GetCapability(TCM_CAP_VERSION, 0, &value, p);
	if(status == 0)
	{
		temp = (value>>24)&0xff;
		tcmstatus->version[0] = temp;
		temp = (value>>16)&0xff;
		tcmstatus->version[1] = temp;
		temp = (value>>8)&0xff;
		tcmstatus->version[2] = temp;
		temp = value&0xff;
		tcmstatus->version[3] = temp;
	}
	
	if(tcmstatus->Owner_install == TRUE)
	{
		tcmstatus->EK_Installed = TRUE;
		//	show_operation(CMD_TESTTCMSTS, 0);
		return 0;
	}
	
	return 0;
	
}
INT32 CTCMTOOLDlg::CreateEndEK(TCM_Status *tcmstatus )
{
	UINT32 status = 0;
	if(tcmstatus->Owner_install == TRUE)
	{
		tcmstatus->EK_Installed = TRUE;
		return 0;
	}
	
	if(tcmstatus->enable == FALSE || tcmstatus->activate == FALSE)
	{
		status = PhysicalEnable();
		if(status != 0)
		{
			return status;			
		}
		
		status = PhysicalSetDeactivate(0);
		if(status != 0)
		{
			return status;			
		}
		
		status = CreateEK();
		if(status == 0)
		{
			tcmstatus->EK_Installed = TRUE;			
		}
		else
		{
			return status;
		}
		
		status = PhysicalSetDeactivate(1);
		if(status != 0)
		{
			return status;			
		}
		
		status = PhysicalDisable();
		if(status != 0)
		{
			return status;			
		}
		
		
		return 0;
	}
	
	status = CreateEK();
	if(status == 0)
	{
		tcmstatus->EK_Installed = TRUE;
	}
	else
	{
		return status;
	}
	
	return 0;


}
INT32 CTCMTOOLDlg::GetTCM_EKExist(TCM_Status *tcmstatus,BYTE* pubKey)
{
	UINT32 status = 0;
	if(tcmstatus->Owner_install == TRUE)
	{
		tcmstatus->EK_Installed = TRUE;
		return 0;
	}
	
	if(tcmstatus->enable == FALSE || tcmstatus->activate == FALSE)
	{
		status = PhysicalEnable();
		if(status != 0)
		{
			return status;			
		}
		
		status = PhysicalSetDeactivate(0);
		if(status != 0)
		{
			return status;			
		}
		
		status = ReadPubEK(pubKey);
		if(status == 0)
		{
			tcmstatus->EK_Installed = m_isEKExist;			
		}
		else
		{
			return status;
		}
		
		status = PhysicalSetDeactivate(1);
		if(status != 0)
		{
			return status;			
		}
		
		status = PhysicalDisable();
		if(status != 0)
		{
			return status;			
		}
		
		
		return 0;
	}
	
	status = ReadPubEK(pubKey);
	if(status == 0)
	{
		tcmstatus->EK_Installed = m_isEKExist;
	}
	else
	{
		return status;
	}
	
	return 0;
}

INT32 CTCMTOOLDlg::PCRRead(int index)
{
	INT32 status = 0;
	int insize = 0;
	BYTE pcrvalue[32];
	BYTE *indata = m_TcmDev->BuildSimple(&insize,
		TCM_ORD_PcrRead,
		BSG_U32(index),0);
	
	BYTE *outdata = NULL;
	int outsize = 0;
	status = m_TcmDev->Submit(indata, insize, &outdata, &outsize);
	delete [] indata;
	
	if(status == 0)
	{
		status = m_TcmDev->VerifySimple(
			outdata,
			outsize,
			TCM_ORD_PcrRead,
			BSG_DIGEST_OUT(&pcrvalue),
			0);
			if( outdata )
			{
				delete [] outdata;
				outdata = NULL;
			}
		if(status != 0)
		{
			//	show_operation(CMD_PCEREAD, status);
			if( outdata )
			{
				delete [] outdata;
				outdata = NULL;
			}
			return status;
		}
	}
	else
	{
		//show_operation(CMD_PCEREAD, status);
		if( outdata )
		{
			delete [] outdata;
			outdata = NULL;
		}
		return status;
	}
	show_pcrvalue(pcrvalue, index);
	//	show_operation(CMD_PCEREAD, status);
	//show_result();
	
	
	
	return 0;
}

INT32 CTCMTOOLDlg::OnButtonStartAndSelftest() 
{
	if( !m_bZteicFlag )
		return TCM_TOOL_ERR_INIT;
	
	// TODO: Add your control notification handler code here
	INT32 status = 0;
	show_string("\n\n");
	m_bFlag = TRUE;
	show_head("Start And SelfTest");
	
	status = StartUp();
	
	if(status != 0)
	{	
		show_operation(CMD_SELFTEST ,status );
		show_tail(status);
		writeLogMsg( (char*)m_strOperLog );
		m_strOperLog.Empty();
		m_bFlag = FALSE; 	
		show_result();
		return status;
	}
	
	
	status = SelfTest();
	show_operation(CMD_SELFTEST ,status );
	show_tail(status);
	writeLogMsg( (char*)m_strOperLog );
	m_strOperLog.Empty();
	m_bFlag = FALSE; 	
	show_result();
	m_bFlag = FALSE;
	writeLogMsg( (char*)m_strOperLog );
	m_strOperLog.Empty();

	return status;
}

int CTCMTOOLDlg::OnButtonEnable() 
{
	// TODO: Add your control notification handler code here
	if( !m_bZteicFlag )
		return TCM_TOOL_ERR_INIT;
	INT32 status = 0;
	TCM_Status tcmsts;
	memset(&tcmsts,  0, sizeof(tcmsts));	
	show_string("\n\n");
	m_bFlag = TRUE; //表示本次操作的日志允许写到m_strOperLog内
	
	show_head("Enable TCM");
	
	status = GetTCMStatus(&tcmsts);

	if(status != 0)
	{
		show_operation( CMD_TCM_ENABLE,status);
		show_tail(status);	
		writeLogMsg( (char*)m_strOperLog );
		m_strOperLog.Empty();
		m_bFlag = FALSE; 	
		show_result();
		return status;
	}
	if(tcmsts.enable == TRUE && tcmsts.activate == TRUE)
	{		
		//TCM已经处于Enable状态
		show_string("TCM is already enabled!");
		show_tail(0);
		writeLogMsg( (char*)m_strOperLog );
		m_strOperLog.Empty();
		m_bFlag = FALSE; 	
		show_result();
		return TCM_TOOL_ERR_SUCCESS;
	}
	status = PhysicalEnable();
	if(status != 0)
	{
		show_tail(status);	
		writeLogMsg( (char*)m_strOperLog );
		m_strOperLog.Empty();
		m_bFlag = FALSE; 	
		show_result();
		return status;			
	}
	
	status = PhysicalSetDeactivate(0);
	if(status != 0)
	{
		show_operation( CMD_TCM_ENABLE,status);
		show_tail(status);	
		writeLogMsg( (char*)m_strOperLog );
		m_strOperLog.Empty();
		m_bFlag = FALSE; 	
		show_result();
		return status;			
	}
	show_operation( CMD_TCM_ENABLE,status);
	show_tail(status);
	writeLogMsg( (char*)m_strOperLog );
	m_strOperLog.Empty();
	m_bFlag = FALSE; 	
	show_result();
	
	return status;
}


int CTCMTOOLDlg::OnButtonDisable() 
{
	// TODO: Add your control notification handler code here
	if( !m_bZteicFlag )
		return TCM_TOOL_ERR_INIT;
	INT32 status = 0;
	
	TCM_Status tcmsts;
	memset(&tcmsts,  0, sizeof(tcmsts));
	show_string("\n\n");
	m_bFlag = TRUE;
	
	show_head("Disable TCM");
	
	status = GetTCMStatus(&tcmsts);
	if(status != 0)
	{
		show_operation( CMD_TCM_DISABLE,status);
		show_tail(status);	
		writeLogMsg( (char*)m_strOperLog );
		m_strOperLog.Empty();
		m_bFlag = FALSE; 	
		show_result();
		return status;
	}
	if(tcmsts.enable == FALSE && tcmsts.activate == FALSE)
	{
		show_string("TCM is already disabled!");
		show_tail(0);
		writeLogMsg( (char*)m_strOperLog );
		m_strOperLog.Empty();
		m_bFlag = FALSE; 	
		show_result();
		return status;
	}
	
	status = PhysicalSetDeactivate(1);
	if(status != 0)
	{
		show_operation( CMD_TCM_DISABLE,status);
		show_tail(status);
		writeLogMsg( (char*)m_strOperLog );
		m_strOperLog.Empty();
		m_bFlag = FALSE; 	
		show_result();
		return status;			
	}
	
	status = PhysicalDisable();
	if(status != 0)
	{
		show_operation( CMD_TCM_DISABLE,status);
		show_tail(status);
		writeLogMsg( (char*)m_strOperLog );
		m_strOperLog.Empty();
		m_bFlag = FALSE; 	
		show_result();
		return status;			
	}
	show_operation( CMD_TCM_DISABLE,status);
	show_tail(status);
	writeLogMsg( (char*)m_strOperLog );
	m_strOperLog.Empty();
	m_bFlag = FALSE; 	
	show_result();
	
	return status;
}

int CTCMTOOLDlg::OnButtonForceclear() 
{
	if( !m_bZteicFlag )
		return TCM_TOOL_ERR_INIT;

	UINT32 status = 0;
	
	show_string("\n\n");
	m_bFlag = TRUE;
	show_head("ForceClear ");
	status = ForceClear();
	show_operation( CMD_FORCECLEAR,status);
	show_tail(status);	
	writeLogMsg( (char*)m_strOperLog );
	m_strOperLog.Empty();
	m_bFlag = FALSE; 	
	show_result();
	
	return status;
}

int CTCMTOOLDlg::OnButtonPcrread() 
{
	if( !m_bZteicFlag )
		return TCM_TOOL_ERR_INIT;
	
	// TODO: Add your control notification handler code here
	UINT32 status = 0;
	UINT32 i=0;
	
	TCM_Status tcmsts;
	memset(&tcmsts,  0, sizeof(tcmsts));
	show_string("\n\n");
	m_bFlag = TRUE;
	show_head("Read Pcr ");
	status = GetTCMStatus(&tcmsts);
	if(status != 0)
	{
		show_operation(CMD_PCEREAD ,status );
		show_tail(status);	
		writeLogMsg( (char*)m_strOperLog );
		m_strOperLog.Empty();
		m_bFlag = FALSE; 	
		show_result();
		return status;
	}
	if(tcmsts.enable == FALSE && tcmsts.activate == FALSE)
	{
		status = PhysicalEnable();
		if(status != 0)
		{
			show_operation(CMD_PCEREAD ,status );
			show_tail(status);	
			writeLogMsg( (char*)m_strOperLog );
			m_strOperLog.Empty();
			m_bFlag = FALSE; 	
			show_result();
			return status;
		}
		
		status = PhysicalSetDeactivate(0);
		if(status != 0)
		{
			show_operation(CMD_PCEREAD ,status );
			show_tail(status);	
			writeLogMsg( (char*)m_strOperLog );
			m_strOperLog.Empty();
			m_bFlag = FALSE; 	
			show_result();
			return status;
		}
		
	}
	for(i=0; i<24; i++)
	{	
		show_operation(CMD_PCEREAD ,status );
		status = PCRRead(i);
		
		if(status != 0)
		{
			show_operation(CMD_PCEREAD ,status );
			show_tail(0);	
			writeLogMsg( (char*)m_strOperLog );
			m_strOperLog.Empty();
			m_bFlag = FALSE; 	
			show_result();
			return status;
		}
		
	}

	show_tail(status);	
	writeLogMsg( (char*)m_strOperLog );
	m_strOperLog.Empty();
	m_bFlag = FALSE; 	
	show_result();
	
	return status;
}

int CTCMTOOLDlg::OnButtonCreateek() 
{
	if( !m_bZteicFlag )
		return TCM_TOOL_ERR_INIT;
	// TODO: Add your control notification handler code here
	UINT32 status = 0;
	
	TCM_Status tcmsts;
	memset(&tcmsts,  0, sizeof(tcmsts));
	show_string("\n\n");
	show_head("Create EK ");
	status = GetTCMStatus(&tcmsts);
	if(status != 0)
	{
		show_tail(status);	
		show_result();
		return status;
	}
	
	status = GetTCM_EKExist(&tcmsts);
	if(status != 0)
	{
		show_tail(status);	
		show_result();
		return status;
	}
	//如果EK已经存在
	if(tcmsts.EK_Installed == TRUE)
	{
		show_string("TCM already has EK installed!");
		show_tail(0);	
		show_result();
		return status;
	}
	else
	{
		//判断目前TCM的状态
		if(tcmsts.enable == FALSE)
		{
			status = PhysicalEnable();
			if(status != 0)
			{
				show_tail(status);	
				show_result();
				return status;
			}
		}
		if(tcmsts.activate == FALSE)
		{
			status = PhysicalSetDeactivate(0);
			if(status != 0)
			{
				show_tail(status);	
				show_result();
				return status;
			}
			
		}
	}
	//如果没有EK，则创建EK
	status = CreateEK();
	show_tail(status);	
	show_result();
	
	return status;
}
int  CTCMTOOLDlg::GetVer( BYTE* outRsp ,int* len)
{
	//00C100000012 00 00 80 65 00 00 09 21 00 00 00 00
	BYTE inBytes[0x14] = {0};
	inBytes[0] = 0x00;
	inBytes[1] = 0xc1;
	inBytes[2] = 0x00;
	inBytes[3] = 0x00;
	inBytes[4] = 0x00;
	inBytes[5] = 0x12;
	inBytes[6] = 0x00;
	inBytes[7] = 0x00;
	inBytes[8] = 0x80;
	inBytes[9] = 0x65;
	inBytes[10] = 0x00;
	inBytes[11] = 0x00;
	inBytes[12] = 0x09;
	inBytes[13] = 0x21;
	inBytes[14] = 0x00;
	inBytes[15] = 0x00;
	inBytes[16] = 0x00;
	inBytes[17] = 0x00;
	BYTE* rstData = NULL;
	int rstSize = 0;
	int status = m_TcmDev->Submit(inBytes, 18, &rstData, &rstSize);
	if( status == 0 )
	{
	 memcpy(outRsp,rstData+ 10 ,rstSize - 10 );
	 *len = rstSize - 10;
	}

	if( rstData )
		delete rstData;

	return status;

}
int CTCMTOOLDlg::OnButtonGettcminfo() 
{
	if( !m_bZteicFlag )
		return TCM_TOOL_ERR_INIT;
	BYTE ver[0x33] = {0};
	int verLen = 0;
	UINT32 status = 0;

	// TODO: Add your control notification handler code here
	
	char message[TCM_TOOL_MAXMSG+1];
	TCM_Status tcmsts;
	memset(&tcmsts,  0, sizeof(tcmsts));
	memset(message,0,TCM_TOOL_MAXMSG+1);

	show_string("\n\n");
	m_bFlag = TRUE;
	show_head("Get TCM Infomation ");
	status = GetTCMStatus(&tcmsts);
	if(status != 0)
	{
		show_operation(CMD_GET_TCM_INFO ,status );
		show_tail(status);	
		writeLogMsg( (char*)m_strOperLog );
		m_strOperLog.Empty();
		m_bFlag = FALSE; 	
		show_result();
		return status;
	}
	
	status = GetTCM_EKExist(&tcmsts);
	if(status != 0)
	{
		show_operation(CMD_GET_TCM_INFO ,status );
		show_tail(status);	
		writeLogMsg( (char*)m_strOperLog );
		m_strOperLog.Empty();
		m_bFlag = FALSE; 	
		show_result();
		return status;
	}
	sprintf(message,"TCM Version                     [%x.%x.%x.%x]", 
		tcmsts.version[0], tcmsts.version[1],tcmsts.version[2],tcmsts.version[3]);
	
	show_string(message);
	if(tcmsts.enable == TRUE && tcmsts.activate == TRUE)
	{
		show_string("TCM status                      [Enable] [Activate]");
	}
	else
		show_string("TCM status                      [Disable] [Dectivated]");
	
	if(tcmsts.EK_Installed == TRUE)
	{
		show_string("EK                              [Installed]");
	}
	else
		show_string("EK                              [NOT Installed]");
	
	if(tcmsts.Owner_install == TRUE)
	{
		show_string("Owner                           [Installed]");
	}
	else
		show_string("Owner                           [NOT Installed]");
	
	show_tail(0);
	writeLogMsg( (char*)m_strOperLog );
	m_strOperLog.Empty();
	m_bFlag = FALSE; 	
	show_result();
	
	return status;
}
void CTCMTOOLDlg::show_head(const char* s)
{
	CTString message;
	message = TCM_TOKEN_OUT;
	m_output_str += message;
	m_output_str += TCM_NEWLINE;
	
	message="Operation: ";
	message+=(char*)s;
	
	m_output_str += message;
	m_output_str += TCM_NEWLINE;
	
	message = TCM_TOKEN_SPACE;
	m_output_str += message;
	m_output_str += TCM_NEWLINE;
	
	if( m_bFlag )
	{
		CTString strMsg;
		strMsg = TCM_TOKEN_OUT;
		m_strOperLog += strMsg;
		m_strOperLog += TCM_NEWLINE;		
		strMsg="Operation: ";
		strMsg+=(char *)s;

		m_strOperLog += strMsg;
		m_strOperLog += TCM_NEWLINE;		
		strMsg = TCM_TOKEN_SPACE;
		m_strOperLog += strMsg;
		m_strOperLog += TCM_NEWLINE;
		
	}
	else
		m_strOperLog.Empty();
	
}

void CTCMTOOLDlg::show_tail(int success)
{
	CTString message;
	
	message = TCM_TOKEN_SPACE;
	m_output_str += message;
	m_output_str += TCM_NEWLINE;
	
	if(success == 0)
	{
		message = "Operation result: Success";
	}
	else
	{
		message = "Operation result: Failed , error is ";
		message += (char*)GetErrMsg(success);	
	}
	m_output_str += message;
	m_output_str += TCM_NEWLINE;
	m_output_str += TCM_TOKEN_OUT;
	m_output_str += TCM_NEWLINE;
	m_output_str += TCM_NEWLINE;
	
	
	if( m_bFlag )
	{
		CTString strMsg;
		strMsg = TCM_TOKEN_SPACE;
		m_strOperLog += strMsg;
		m_strOperLog += TCM_NEWLINE;
		
		if(success == 0)
		{
			strMsg="Operation result: Success";
		}
		else
		{
			strMsg="Operation result: Failed , error is ";
			strMsg+=(char *)GetErrMsg(success);	
		}
		m_strOperLog += strMsg;
		m_strOperLog += TCM_NEWLINE;
		m_strOperLog += TCM_TOKEN_OUT;
		m_strOperLog += TCM_NEWLINE;
		m_strOperLog += TCM_NEWLINE;
		
		
	}
	else
		m_strOperLog.Empty();
	
}
//////////////////////////////////////////////////////
// 根据返回值得到对应的返回值对应的描述信息
// Add by Kevin.Luo 2008-11-21
/////////////////////////////////////////////////////
const char g_err_success[]="TCM_SUCCESS";
const char g_err_authfail[]="TCM_AUTHFAIL";
const char g_err_badindex[]="TCM_BADINDEX";
const char g_err_badparameter[]="TCM_BAD_PARAMETER";
const char g_err_auditfailure[]="TCM_AUDITFAILURE";
const char g_err_cleardisabled[]="TCM_CLEAR_DISABLED";
const char g_err_deactivated[]="TCM_DEACTIVATED";
const char g_err_disabled[]="TCM_DISABLED";
const char g_err_disabledcmd[]="TCM_DISABLED_CMD";
const char g_err_tcmfail[]="TCM_FAIL";
const char g_err_badordinal[]="TCM_BAD_ORDINAL";
const char g_err_installdisabled[]="TCM_INSTALL_DISABLED";
const char g_err_invalidkeyhandle[]="TCM_INVALID_KEYHANDLE";
const char g_err_tcmkeynotfound[]="TCM_KEYNOTFOUND";
const char g_err_tcminappropriateenc[]="TCM_INAPPROPRIATE_ENC";
const char g_err_tcmscherror[]="TCM_SCH_ERROR";
const char g_err_tcmfailedselftest[]="TCM_FAILEDSELFTEST";
const char g_err_tcmresources[]="TCM_RESOURCES";
const char g_err_tcmshortrandom[]="TCM_SHORTRANDOM";
const char g_err_tcmsize[]="TCM_SIZE";
const char g_err_tcmwrongpcrval[]="TCM_WRONGPCRVAL";
const char g_err_badparamsize[]="TCM_BAD_PARAM_SIZE";
const char g_err_tcmschthread[]="TCM_SCH_THREAD";
const char g_err_tcmmigratefall[]="TCM_MIGRATEFAIL";
const char g_err_tcminvalidpcrinfo[]="TCM_INVALID_PCR_INFO";
const char g_err_tcmnospace[]="TCM_NOSPACE";
const char g_err_tcmnosmk[]="TCM_NOSMK";
const char g_err_tcmnotsealedblob[]="TCM_NOTSEALED_BLOB";
const char g_err_tcmownerset[]="TCM_OWNER_SET";
const char g_err_tcmauth2fail[]="TCM_AUTH2FAIL";
const char g_err_tcmbadtag[]="TCM_BADTAG";
const char g_err_tcmioerror[]="TCM_IOERROR";
const char g_err_tcmencrypterror[]="TCM_ENCRYPT_ERROR";
const char g_err_tcmdecrypterror[]="TCM_DECRYPT_ERROR";
const char g_err_tcmdinvaledauthhandle[]="TCM_INVALID_AUTHHANDLE";
const char g_err_tcmdnoendorsement[]="TCM_NO_ENDORSEMENT";
const char g_err_tcminvalidkeyusage[]="TCM_INVALID_KEYUSAGE";
const char g_err_tcmwrongentitytype[]="TCM_WRONG_ENTITYTYPE";
const char g_err_tcminvalidpostinit[]="TCM_INVALID_POSTINIT";
const char g_err_tcminappropriatesig[]="TCM_INAPPROPRIATE_SIG";
const char g_err_tcmbadkeyproperty[]="TCM_BAD_KEY_PROPERTY";
const char g_err_tcmbadmigration[]="TCM_BAD_MIGRATION";
const char g_err_tcmbadscheme[]="TCM_BAD_SCHEME";
const char g_err_tcmbaddatasize[]="TCM_BAD_DATASIZE";
const char g_err_tcmbadmode[]="TCM_BAD_MODE";
const char g_err_tcmpresence[]="TCM_BAD_PRESENCE";
const char g_err_tcmbadversion[]="TCM_BAD_VERSION";
const char g_err_tcmretry[]="TCM_RETRY";
const char g_err_tcmdefendlockrunning[]="TCM_DEFEND_LOCK_RUNNING";
const char g_err_tcmnowraptransport[]="TCM_NO_WRAP_TRANSPORT";
const char g_err_tcmauditfailunsuccessful[]="TCM_AUDITFAIL_UNSUCCESSFUL";
const char g_err_tcmauditfailsuccessful[]="TCM_AUDITFAIL_SUCCESSFUL";
const char g_err_tcmnotresetable[]="TCM_NOTRESETABLE";
const char g_err_tcmnotlocal[]="TCM_NOTLOCAL";
const char g_err_tcmbadtype[]="TCM_BAD_TYPE";
const char g_err_tcminvalidresource[]="TCM_INVALID_RESOURCE";
const char g_err_tcmnotfips[]="TCM_NOTFIPS";
const char g_err_tcminvalidfamily[]="TCM_INVALID_FAMILY";
const char g_err_tcmnonvpermission[]="TCM_NO_NV_PERMISSION";
const char g_err_requiressign[]="TCM_REQUIRES_SIGN";
const char g_err_temkeynotsupported[]="TCM_KEY_NOTSUPPORTED";

const char g_err_TCM_AUTH_CONFLICT[]="TCM_AUTH_CONFLICT";
const char g_err_TCM_BAD_ATTRIBUTES[]="TCM_BAD_ATTRIBUTES";
const char g_err_TCM_AREA_LOCKED[]="TCM_AREA_LOCKED";
const char g_err_TCM_BAD_LOCALITY[]="TCM_BAD_LOCALITY";
const char g_err_TCM_READ_ONLY[]="TCM_READ_ONLY";
const char g_err_TCM_PER_NOWRITE[]="TCM_PER_NOWRITE";

const char g_err_TCM_FAMILYCOUNT[]="TCM_FAMILYCOUNT";
const char g_err_TCM_WRITE_LOCKED[]="TCM_WRITE_LOCKED";
const char g_err_TCM_INVALID_STRUCTURE[]="TCM_INVALID_STRUCTURE";
const char g_err_TCM_KEY_OWNER_CONTROL[]="TCM_KEY_OWNER_CONTROL";

const char g_err_TCM_BAD_COUNTER[]="TCM_BAD_COUNTER";
const char g_err_TCM_NOT_FULLWRITE[]="TCM_NOT_FULLWRITE";
const char g_err_TCM_CONTEXT_GAP[]="TCM_CONTEXT_GAP";
const char g_err_TCM_MAXNVWRITES[]="TCM_MAXNVWRITES";
const char g_err_TCM_NOOPERATOR[]="TCM_NOOPERATOR";

const char g_err_TCM_RESOURCEMISSING[]="TCM_RESOURCEMISSING";
const char g_err_TCM_DELEGATE_LOCK[]="TCM_DELEGATE_LOCK";
const char g_err_TCM_DELEGATE_FAMILY[]="TCM_DELEGATE_FAMILY";
const char g_err_TCM_DELEGATE_ADMIN[]="TCM_DELEGATE_ADMIN";
const char g_err_TCM_TRANS_EXCLUSIVE[]="TCM_TRANS_EXCLUSIVE";


const char g_err_TCM_OWNER_CONTROL[]="TCM_OWNER_CONTROL";
const char g_err_TCM_DAA_RESOURCES[]="TCM_DAA_RESOURCES";
const char g_err_TCM_DAA_INPUT_DATA0[]="TCM_DAA_INPUT_DATA0";
const char g_err_TCM_DAA_INPUT_DATA1[]="TCM_DAA_INPUT_DATA1";
const char g_err_TCM_DAA_ISSUER_SETTINGS[]="TCM_DAA_ISSUER_SETTINGS";
const char g_err_TCM_DAA_TCM_SETTINGS[]="TCM_DAA_TCM_SETTINGS";
const char g_err_TCM_DAA_STAGE[]="TCM_DAA_STAGE";

const char g_err_TCM_DAA_ISSUER_VALIDITY[]="TCM_DAA_ISSUER_VALIDITY";
const char g_err_TCM_DAA_WRONG_W[]="TCM_DAA_WRONG_W";
const char g_err_TCM_BAD_HANDLE[]="TCM_BAD_HANDLE";
const char g_err_TCM_BAD_DELEGATE[]="TCM_BAD_DELEGATE";
const char g_err_TCM_NOCONTEXTSPACE[]="TCM_NOCONTEXTSPACE";
const char g_err_TCM_BADCONTEXT[]="TCM_BADCONTEXT";

const char g_err_TCM_TOOMANYCONTEXTS[]="TCM_TOOMANYCONTEXTS";
const char g_err_TCM_MA_TICKET_SIGNATURE[]="TCM_MA_TICKET_SIGNATURE";
const char g_err_TCM_MA_DESTINATION[]="TCM_MA_DESTINATION";
const char g_err_TCM_MA_SOURCE[]="TCM_MA_SOURCE";
const char g_err_TCM_MA_AUTHORITY[]="TCM_MA_AUTHORITY";
const char g_err_TCM_PERMANENTEK[]="TCM_PERMANENTEK";
const char g_err_TCM_BAD_SIGNATURE[]="TCM_BAD_SIGNATURE";
const char g_err_TCM_BEGIN[]="TCM_BEGIN";

const char* CTCMTOOLDlg::GetErrMsg(INT32 status )
{
	if( status == 0x00000000)
		return g_err_success;
	else if (status == 0x00000001 )
		return g_err_authfail;
	else if (status == 0x00000002 )
		return g_err_badindex;
	else if (status == 0x00000003)
		return g_err_badparameter;
	else if (status ==0x00000004 )
		return g_err_auditfailure;
	else if (status ==0x00000005 )
		return g_err_cleardisabled;
	else if (status == 0x00000006)
		return g_err_deactivated;
	else if (status == 0x00000007)
		return g_err_disabled;
	else if (status ==0x00000008 )
		return g_err_disabledcmd;
	else if (status == 0x00000009)
		return g_err_tcmfail;
	else if (status == 0x0000000a)
		return g_err_badordinal;
	else if (status == 0x0000000b)
		return g_err_installdisabled;
	else if (status == 0x0000000c )
		return g_err_invalidkeyhandle;
	else if (status ==0x0000000d )
		return g_err_tcmkeynotfound;
	else if (status == 0x0000000e)
		return g_err_tcminappropriateenc;
	else if (status ==0x0000001b )
		return g_err_tcmscherror;
	else if (status == 0x0000001c)
		return g_err_tcmfailedselftest;
	else if (status ==0x00000015 )
		return g_err_tcmresources;
	else if (status == 0x00000016)
		return g_err_tcmshortrandom;
	else if (status == 0x00000017)
		return g_err_tcmsize;
	else if (status ==0x00000018)
		return g_err_tcmwrongpcrval;
	else if (status ==0x00000019 )
		return g_err_badparamsize;
	else if (status == 0x0000001a)
		return g_err_tcmschthread;
	else if (status == 0x0000000f)
		return g_err_tcmmigratefall;
	else if (status == 0x00000010)
		return g_err_tcminvalidpcrinfo;
	else if (status ==0x00000011 )
		return g_err_tcmnospace;
	else if (status ==0x00000012 )
		return g_err_tcmnosmk;
	else if (status ==0x00000013 )
		return g_err_tcmnotsealedblob;
	else if (status == 0x00000014)
		return g_err_tcmownerset;
	else if (status == 0x0000001d)
		return g_err_tcmauth2fail;
	else if (status == 0x0000001e)
		return g_err_tcmbadtag;
	else if (status == 0x0000001f)
		return g_err_tcmioerror;
	else if (status == 0x00000020)
		return g_err_tcmencrypterror;
	else if (status ==0x00000021 )
		return g_err_tcmdecrypterror;
	else if (status ==0x00000022 )
		return g_err_tcmdinvaledauthhandle;
	else if (status ==0x00000023 )
		return g_err_tcmdnoendorsement;
	else if (status == 0x00000024)
		return g_err_tcminvalidkeyusage;
	else if (status == 0x00000025)
		return g_err_tcmwrongentitytype;
	else if (status == 0x00000026)
		return g_err_tcminvalidpostinit;
	else if (status == 0x00000027)
		return g_err_tcminappropriatesig;
	else if (status == 0x00000028)
		return g_err_tcmbadkeyproperty;
	else if (status == 0x00000029)
		return g_err_tcmbadmigration;
	else if (status == 0x0000002a)
		return g_err_tcmbadscheme;
	else if (status == 0x0000002b)
		return g_err_tcmbaddatasize;
	else if (status == 0x0000002c)
		return g_err_tcmbadmode;
	else if (status == 0x0000002d)
		return g_err_tcmpresence;
	else if (status == 0x0000002e)
		return g_err_tcmbadversion;
	else if (status == 0x00000800)
		return g_err_tcmretry;
	else if (status == 0x00000803)
		return g_err_tcmdefendlockrunning;
	else if (status == 0x0000002f)
		return g_err_tcmnowraptransport;
	else if (status == 0x00000030)
		return g_err_tcmauditfailunsuccessful;
	else if (status == 0x00000031)
		return g_err_tcmauditfailsuccessful;
	else if (status ==0x00000032 )
		return g_err_tcmnotresetable;
	else if (status ==0x00000033 )
		return g_err_tcmnotlocal;
	else if (status == 0x00000034)
		return g_err_tcmbadtype;
	else if (status == 0x00000035)
		return g_err_tcminvalidresource;
	else if (status ==0x00000036 )
		return g_err_tcmnotfips;
	else if (status ==0x00000037 )
		return g_err_tcminvalidfamily;
	else if (status == 0x00000038)
		return g_err_tcmnonvpermission;
	else if (status == 0x00000039)
		return g_err_requiressign;
	else if (status == 0x0000003a)
		return g_err_temkeynotsupported;
	else if (status == 0x0000003b)
		return g_err_TCM_AUTH_CONFLICT;
	else if (status == 0x00000042)
		return g_err_TCM_BAD_ATTRIBUTES;
	else if (status == 0x0000003c)
		return g_err_TCM_AREA_LOCKED;
	else if (status == 0x0000003d)
		return g_err_TCM_BAD_LOCALITY;
	else if (status == 0x0000003e)
		return g_err_TCM_READ_ONLY;
	else if (status == 0x0000003f)
		return g_err_TCM_PER_NOWRITE;
	else if (status == 0x00000040)
		return g_err_TCM_FAMILYCOUNT;
	else if (status == 0x00000041)
		return g_err_TCM_WRITE_LOCKED;
	else if (status ==0x00000042 )
		return g_err_TCM_BAD_ATTRIBUTES;
	
	else if (status == 0x00000043)
		return g_err_TCM_INVALID_STRUCTURE;
	else if (status == 0x00000044)
		return g_err_TCM_KEY_OWNER_CONTROL;
	else if (status ==0x00000045)
		return g_err_TCM_BAD_COUNTER;
	else if (status == 0x00000046)
		return g_err_TCM_NOT_FULLWRITE;
	else if (status == 0x00000047)
		return g_err_TCM_CONTEXT_GAP;
	else if (status == 0x00000048)
		return g_err_TCM_MAXNVWRITES;
	else if (status == 0x00000049)
		return g_err_TCM_NOOPERATOR;
	else if (status == 0x0000004a)
		return g_err_TCM_RESOURCEMISSING;
	else if (status == 0x0000004b)
		return g_err_TCM_DELEGATE_LOCK;
	else if (status == 0x0000004c)
		return g_err_TCM_DELEGATE_FAMILY;
	else if (status == 0x0000004d)
		return g_err_TCM_DELEGATE_ADMIN;
	else if (status == 0x0000004e)
		return g_err_TCM_TRANS_EXCLUSIVE;
	
	else if (status == 0x0000004f)
		return g_err_TCM_OWNER_CONTROL;
	else if (status ==0x00000050 )
		return g_err_TCM_DAA_RESOURCES;
	else if (status == 0x00000051)
		return g_err_TCM_DAA_INPUT_DATA0;
	else if (status == 0x00000052)
		return g_err_TCM_DAA_INPUT_DATA1;
	else if (status == 0x00000053)
		return g_err_TCM_DAA_ISSUER_SETTINGS;
	else if (status == 0x00000054)
		return g_err_TCM_DAA_TCM_SETTINGS;
	else if (status == 0x00000055)
		return g_err_TCM_DAA_STAGE;
	else if (status ==0x00000056 )
		return g_err_TCM_DAA_ISSUER_VALIDITY;
	else if (status == 0x00000057)
		return g_err_TCM_DAA_WRONG_W;
	else if (status ==0x00000058 )
		return g_err_TCM_BAD_HANDLE;
	
	else if (status ==0x00000059 )
		return g_err_TCM_BAD_DELEGATE;
	else if (status == 0x00000059)
		return g_err_TCM_NOCONTEXTSPACE;
	else if (status == 0x0000005a)
		return g_err_TCM_BADCONTEXT;
	else if (status == 0x0000005b)
		return g_err_TCM_TOOMANYCONTEXTS;
	else if (status == 0x0000005c)
		return g_err_TCM_MA_TICKET_SIGNATURE;
	else if (status == 0x0000005d)
		return g_err_TCM_MA_DESTINATION;
	else if (status ==0x0000005e )
		return g_err_TCM_MA_SOURCE;
	else if (status == 0x0000005f)
		return g_err_TCM_MA_AUTHORITY;
	else if (status == 0x00000061)
		return g_err_TCM_PERMANENTEK;
	else if (status == 0x00000062)
		return g_err_TCM_BAD_SIGNATURE;
	else if (status == 0x00000063)
		return g_err_TCM_BEGIN;

	return NULL;
}

void CTCMTOOLDlg::show_operation(const char* s, UINT32 success)
{
	CTString message;
	char suc[128];

	memset(suc,0,128);
	if(success == 0)
	{
		sprintf(suc,"Success(0x%08x)",success);
	}
	else
	{
		sprintf(suc,"Failed (0x%08x)", success);
	}

	message=(char *)s;
	message+=suc;

	m_output_str += message;
	m_output_str += TCM_NEWLINE;
	if( m_bFlag )
	{
		CTString message;
		memset(suc,0,128);

		if(success == 0)
			sprintf(suc,"Success(0x%08x)", success);
		else
			sprintf(suc,"Failed (0x%08x)", success);
		message=(char *)s;
		message+=suc;

		m_strOperLog += message;
		m_strOperLog += TCM_NEWLINE;
	}
	else
		m_strOperLog.Empty();
}

UINT32 CTCMTOOLDlg::show_operation_startup(const char* s, UINT32 success)
{
	UINT32 ret = 0;
	CTString message;
	char strTmp[64];
	memset( strTmp, 0, 64 );

	if(success == 0)
	{
		ret = 0;
		sprintf(strTmp,"Success(0x%08x)", success);
	}
	else if(success == TCM_INVALID_POSTINIT)
	{
		ret = 0;
		sprintf(strTmp,"Success(0x%08x)", success);
	}
	else
	{
		ret = success;
		sprintf(strTmp,"Failed (0x%08x)", success);
	}

	message=(char*)s;
	message+=strTmp;

	m_output_str += message;
	m_output_str += TCM_NEWLINE;
	if( m_bFlag )
	{
		message.Empty();
		memset( strTmp, 0, 64 );

		if(success == 0)
			sprintf(strTmp,"Success(0x%08x)", success);
		else if(success == TCM_INVALID_POSTINIT)
			sprintf(strTmp,"Success(0x%08x)", success);
		else
			sprintf(strTmp,"Failed (0x%08x)", success);

		message=(char*)s;
		message+=strTmp;

		m_strOperLog += message;
		m_strOperLog += TCM_NEWLINE;
		
	}
	else
	{
		m_strOperLog.Empty();
	}

	return ret;
}

void CTCMTOOLDlg::show_string(const char* s)
{
	CTString message;
	message = (char*)s;
	m_output_str += message;
	m_output_str += TCM_NEWLINE;
	if( m_bFlag )
	{
		message.Empty();
		message = (char*)s;
		m_strOperLog += message;
		m_strOperLog += TCM_NEWLINE;
	}
	else 
		m_strOperLog.Empty();
}

void CTCMTOOLDlg::show_pcrvalue(BYTE * buf, int index)
{
	CTString message;
	int i = 0;
	CTString strI;

	strI.IntToString(index);

	message="PcrIndex: ";
	message+=strI;

	m_output_str += message;
	m_output_str += TCM_NEWLINE;
	for(i=0; i<32; i++)
	{
		char szTmp[64];
		memset( szTmp, 0, 64 );
		sprintf( szTmp,"%02x ", buf[i] );
		message=szTmp;
		m_output_str += message;
		if((i+1)%16 == 0)
		{
			m_output_str += TCM_NEWLINE;
		}
	}
	m_output_str += TCM_NEWLINE;
	if( m_bFlag )
	{
		CTString message;
		int i = 0;
		message="PcrIndex: ";
		message+=strI;

		m_strOperLog += message;
		m_strOperLog += TCM_NEWLINE;
		for(i=0; i<32; i++)
		{
			char szTmp[64];
			memset( szTmp, 0, 64 );
			sprintf( szTmp,"%02x ", buf[i] );
			message=szTmp;

			m_strOperLog += message;
			if((i+1)%16 == 0)
			{
				m_strOperLog += TCM_NEWLINE;
			}
		}
		m_strOperLog += TCM_NEWLINE;
		
	}
	else
		m_strOperLog.Empty();
}

void CTCMTOOLDlg::show_result()
{
	//UINT32 dwThreadID1;
	//_beginthreadex(NULL, 0, WorkThread, (PVOID)this, 0, &dwThreadID1);
	show_output();
}

void CTCMTOOLDlg::show_output()
{
	printf(m_output_str);
	//del by wangdong
	
	//GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowText(m_output_str);
	//CEdit *output = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT);
	//m_Output.LineScroll(m_Output.GetLineCount(),0);
}

///////////////////////////////////////////////////
// 计算MAC值，调用TCM_ORD_HMAC指令
// add by Kevin.Luo 2008-11-21
///////////////////////////////////////////////////
INT32 CTCMTOOLDlg::CalcHMAC(BYTE*text,unsigned int textLen,BYTE* s1Data,unsigned int s1Len,BYTE* s2Data,unsigned int s2Len,BYTE* outMac)
{
	int status = tcm_ecc_init();
	if( status )
		return status;

	unsigned int key_len = s1Len + s2Len;
	BYTE* key = new  BYTE[key_len];
	memcpy( key,s1Data,s1Len);
	memcpy( key+s1Len, s2Data,s2Len);
	status=  tcm_hmac( key, key_len,text, textLen, outMac);
	if( status )
	{
		if( key )
			delete[] key;
		return status;
	}

	status = tcm_ecc_release();
	if( status )
	{
		if( key )
			delete[] key;
		return status;
	}

	
	if( key )
			delete[] key;
	
	return status;

	
}

///////////////////////////////////////////////////
//对数据进行加密
//add by Kevin.Luo 2008-11-17
/////////////////////////////////////////////////
INT32 CTCMTOOLDlg::EncData(BYTE* src,unsigned int srcSize,BYTE* pubKey,BYTE* dest,unsigned int* destSize )
{
	
//	输入参数  
//		plaintext:		明文地址
//		uPlaintextLen:	明文长度
//		pubkey:			公钥地址
//		uPubkeyLen：	公钥长度
//		puCiphertextLen：输入的ciphertext缓冲区的大小
//输出参数 
//		ciphertext: 密文地址
//		puCiphertextLen : 密文长度地址指针，指明输出的密文长度
//返回值：加密成功返回0，否则返回1
	int status  =  tcm_ecc_init();
	if( status )
		return status;

	status =  tcm_ecc_encrypt(src, srcSize,pubKey,65,dest,destSize);
	if( status )
		return status;

	status = tcm_ecc_release();
	if( status )
		return status;

	return status;
	
}
////////////////////////////////////////////////////////
// ApCreate 调用TCM_ORD_APCreate
// add by Kevin.Luo 2008-11-21
////////////////////////////////////////////////////////
INT32 CTCMTOOLDlg::CreateAPP(int entityType,int entityValue,BYTE* callNonce,BYTE* inMac,int* sessHandle,long*seq)
{
	BYTE* rstData = NULL;
	int inSize = 0;
	int rstSize = 0;
	
	TCM_NONCE  Nonce;
	memset(Nonce.nonce,0,32);
	//组织 
	BYTE *inData = m_TcmDev->BuildSimple(
		&inSize,						//输入参数总长度
		TCM_ORD_APCreate,					//命令号
		BSG_U16(entityType),				
		BSG_U32(entityValue),				
		BSG_STRUCT(BSG::TCM_NONCE, &Nonce),			
		BSG_DATA(inMac,32),		
		0);
	int status = m_TcmDev->Submit(inData, inSize, &rstData, &rstSize);
	if( inData )
		delete [] inData;
	
	if(status == 0)  
	{
		int Handle = 0 ;
		long lSeq = 0;
		BYTE noce[32];
		status = m_TcmDev->VerifySimple(
			rstData,
			rstSize,
			TCM_ORD_ECC_E,
			BSG_U32_OUT(&Handle),
			BSG_DATA_OUT(noce ,32 ),
			BSG_U32_OUT(&lSeq),
			
			0);
		
		if( rstData)
			delete rstData;
		*sessHandle = Handle;
		*seq = lSeq;
		
	}
	
	return status;
}
////////////////////////////////////////////////////
// 计算SCH值
// add by Kevin.Luo 2008-11-21
////////////////////////////////////////////////////
INT32 CTCMTOOLDlg::CalcSCH(BYTE* inBuf,int inBufLen,BYTE* hash)
{
	return  tcm_sch_hash( inBufLen, inBuf, hash);
}
////////////////////////////////////////////////////
// add by Kevin.luo 2008-11-21
////////////////////////////////////////////////////
INT32 CTCMTOOLDlg::OwnerShip(BYTE* encOwnerData,int encOwnerDataSize,BYTE* encSMKData,int encSMKDataSize)
{
	int status;
	//组织参数开始 
	BYTE  SMSKeyParmsInfo[28];
	BSG::Pack32(SMSKeyParmsInfo, 16*8); //keyLength
	BSG::Pack32(SMSKeyParmsInfo+4, 128);  //blockSize
	BSG::Pack32(SMSKeyParmsInfo+8, 16);  //IVSize
	memset(SMSKeyParmsInfo+12, 0, 16);
	TCM_PROTOCOL_ID protocolID;
	TCM_KEY smkParams;
	protocolID = TCM_PID_OWNER;
	memset(&smkParams, 0, sizeof(smkParams));
	smkParams.tag=TCM_TAG_KEY,
		smkParams.fill=0,
		smkParams.keyUsage = TCM_SMS4KEY_STORAGE;
	smkParams.keyFlags = 0x04;
	smkParams.authDataUsage = TCM_AUTH_NEVER;
	smkParams.algorithmParms.parmSize = 28;
	smkParams.algorithmParms.parms = SMSKeyParmsInfo;
	smkParams.algorithmParms.algorithmID = TCM_ALG_SMS4 ;
	smkParams.algorithmParms.encScheme = TCM_ES_SMS4_CBC;
	smkParams.algorithmParms.sigScheme = TCM_SS_NONE ;
	smkParams.PCRInfo=NULL;
	smkParams.PCRInfoSize=0;
	
	
	TCM_AUTHDATA appMac;
	TCM_NONCE  callerNonce;
	memset(callerNonce.nonce,0,32);//伪随机
	
	
	BYTE key [32];
	memset(key,0,32);
	BYTE s1Data[4];
	BSG::Pack32(s1Data, TCM_ORD_APCreate);
	BYTE s2Data[32];
	memcpy(s2Data, &callerNonce.nonce, 32);
	status = CalcHMAC(key,32,s1Data,4,s2Data,32,appMac); ///计算用于创建ApCreate的mac
	if( status )
	{		
		return status;
	}
	
	
	
	int handle = 0;
	long seq=0;
	status = CreateAPP(0x12,0,callerNonce.nonce,appMac,&handle,&seq); //创建 Ap,得到handle,seq
	if( status )
	{		
		return status;
	}
	
	
	
	
	
	
	BYTE ownerDigest[32];
	int inBufSize = 0;
	BYTE* temp= m_TcmDev->BuildSimple(
		&inBufSize,						//输入参数总长度
		TCM_ORD_TakeOwnership,
		BSG_U16( protocolID ),			//
		BSG_U32( encOwnerDataSize),					//加密后的ownerauth的密文长度
		BSG_DATA( encOwnerData,encOwnerDataSize),	//ownerauth的密文
		BSG_U32 ( encSMKDataSize),					//加密后的SMK的密文长度
		BSG_DATA( encSMKData,encSMKDataSize),	//加密后的SMK的密文
		BSG_STRUCT( BSG::TCM_KEY, &smkParams),		//	
		0);
	inBufSize -=6;
	BYTE* inBuf = temp+6;
	
	
	status = CalcSCH(inBuf,inBufSize ,ownerDigest);
	if( temp )
		delete temp;
	if( status )
	{		
		return status;
	}
	
	
	
	TCM_AUTHDATA ownerMac;
	BYTE wonerkey [32];  
	memset( wonerkey,0 , 32 );
	memcpy(wonerkey,"ZTEIC",5);
	BYTE s1DataForOwner[4];
	BSG::Pack32(s1DataForOwner, seq);
	
	status = CalcHMAC(wonerkey,32,ownerDigest,32,s1DataForOwner,4,ownerMac); 
	if( status )
	{		
		return status;
	}
	
	
	BYTE* rstData = NULL;
	int inSize = 0,rstSize=0;
	//组织 用来调用TCM_ORD_ECC_E命令的参数
	BYTE *inData = m_TcmDev->BuildSimple(
		&inSize,						//输入参数总长度
		TCM_ORD_TakeOwnership,
		BSG_U16( protocolID ),			//
		BSG_U32( encOwnerDataSize),					//加密后的ownerauth的密文长度
		BSG_DATA( encOwnerData,encOwnerDataSize),	//ownerauth的密文
		BSG_U32 ( encSMKDataSize),					//加密后的SMK的密文长度
		BSG_DATA( encSMKData,encSMKDataSize),	//加密后的SMK的密文
		BSG_STRUCT( BSG::TCM_KEY, &smkParams),		//	
		BSG_U32(handle),
		BSG_DATA(ownerMac,32),
		0);
	
	
	
	
	*(inData+1)=0xc2;
	status = m_TcmDev->Submit(inData, inSize, &rstData, &rstSize);//调用TCM_ORD_TakeOwnership命令
	if( inData )
		delete [] inData;
	return status;
	
}
//add by Kevin.Luo 2008-11-17
INT32 CTCMTOOLDlg::TakeOwnership(BYTE* pubKey )//获取ownership
{
	
	//step1 取得公钥
	if( !pubKey )
		return -1;
	int status;	
	//step2 利用得到的公钥对onwner进行加密，得到密文 
	BYTE ownerAuthData [32];
	memset(ownerAuthData ,0 ,32);
	memcpy(ownerAuthData,"ZTEIC",5);  //OnwerAuth的明文
	
	BYTE  encAuthData[2048]; ///用于存放OnwerAuth的密文 
	unsigned int   encAuthDataSize= 2048;///用于存放OnwerAuth的密文长度
	status = EncData(ownerAuthData,32,pubKey, encAuthData,&encAuthDataSize);
	if( status )
	{		
		return status;
	}
	
	
	
	
	
	//step3 利用得到的公钥对SMK进行加密，得到密文 
	
	BYTE dataSMK[32];
	memset(dataSMK,0,32);  //SMKh的明文
	
	BYTE  encSMKData[2048]; ///用于存放SMK的密文 
	unsigned int   encSMKDataSize= 2048;///用于存放SMK的密文长度
	status = EncData(dataSMK,32,pubKey, encSMKData,&encSMKDataSize);
	if( status )
	{		
		
		return status;
	}
	
	
	//step4 获取 ownerShip
	status = OwnerShip(encAuthData,encAuthDataSize, encSMKData,encSMKDataSize);
	if( status )
	{
		return status;
	}
	
	return status;	
	
}
int CTCMTOOLDlg::OnTakeOwnerShip() 
{
	if( !m_bZteicFlag )
		return TCM_TOOL_ERR_INIT;
	
	// TODO: Add your control notification handler code here
	UINT32 status = 0;
	
	TCM_Status tcmsts;
	memset(&tcmsts,  0, sizeof(tcmsts));
	show_string("\n\n");
	m_bFlag = TRUE;
	show_head("TakeOwnerShip ");
	status = GetTCMStatus(&tcmsts);  //得到当前TCM的状态
	if(status != 0)
	{
		show_operation(CMD_TAKEOWNERSHIP ,status );
		show_tail(status);	
		writeLogMsg( (char*)m_strOperLog );
		m_strOperLog.Empty();
		m_bFlag = FALSE; 	
		show_result();
		return status;
	}
	if(tcmsts.Owner_install == TRUE)
	{		
		//owner已存在
		show_string("TCM Ownership is already installed!");
		show_tail(0);	
		writeLogMsg( (char*)m_strOperLog );
		m_strOperLog.Empty();
		m_bFlag = FALSE; 	
		show_result();
		return status;
	}

	BYTE pubKey[65] = {0};
	memset(pubKey,0,65);
	status = GetTCM_EKExist( &tcmsts ,pubKey);
	if( status)
	{		
		show_string("Read PubEk failed");
		show_tail(0);	
		writeLogMsg( (char*)m_strOperLog );
		m_strOperLog.Empty();
		m_bFlag = FALSE; 	
		show_result();
		return status;
	}
	if( tcmsts.EK_Installed == FALSE ) //未生成Pub EK 
	{
		
		show_string("Pub EK is not installed! Create it now...");
		status = CreateEndEK(&tcmsts);
		if(status != 0 )
		{
			show_string("Create PubEk failed");
			show_tail(0);	
			writeLogMsg( (char*)m_strOperLog );
			m_strOperLog.Empty();
			m_bFlag = FALSE; 	
			show_result();
			return status;
		}
		status = GetTCM_EKExist( &tcmsts ,pubKey);
		if(status != 0 )
		{
			show_string("Read PubEk failed");
			show_tail(0);	
			writeLogMsg( (char*)m_strOperLog );
			m_strOperLog.Empty();
			m_bFlag = FALSE; 	
			show_result();
			return status;
		}

	}	


	//判断目前TCM的状态
	if(tcmsts.enable == FALSE)
	{
		status = PhysicalEnable();
		if(status != 0)
		{
			show_operation(CMD_TAKEOWNERSHIP ,status );
			show_tail(status);
			writeLogMsg( (char*)m_strOperLog );
			m_strOperLog.Empty();
			m_bFlag = FALSE; 	
			show_result();
			return status;
		}
	}
	if(tcmsts.activate == FALSE)
	{
		status = PhysicalSetDeactivate(0);
		if(status != 0)
		{
			show_operation(CMD_TAKEOWNERSHIP ,status );
			show_tail(status);
			writeLogMsg( (char*)m_strOperLog );
			m_strOperLog.Empty();
			m_bFlag = FALSE; 	
			show_result();
			return status;
		}
		
	}
	
	status = TakeOwnership(pubKey);
	if(status != 0)
	{
		show_operation(CMD_TAKEOWNERSHIP ,status );
		show_tail(status);	
		writeLogMsg( (char*)m_strOperLog );
		m_strOperLog.Empty();
		m_bFlag = FALSE; 	
		show_result();
		return status;
	}
	show_operation(CMD_TAKEOWNERSHIP ,status );
	show_tail(status);	
	writeLogMsg( (char*)m_strOperLog );
	m_strOperLog.Empty();
	m_bFlag = FALSE; 	
	show_result();
	
	return status;
}

void CTCMTOOLDlg::OnClose() 
{
	writeLogMsg( TCM_EXIT_INO );
}
