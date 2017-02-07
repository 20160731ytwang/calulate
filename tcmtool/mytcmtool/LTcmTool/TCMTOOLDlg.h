// TCMTOOLDlg.h : header file
//

#ifndef ___TCMTOOLDLG_H___
#define ___TCMTOOLDLG_H___

#include "dev.h"
#include "TString.h"

#define TCM_RUNNING_INFO "INFO: TCMTOOL V3.0(Build20120217) Running \n"
#define TCM_EXIT_INO	 "INFO: TCMTOOL V3.0(Build20120217) Exit \n"

#define TCM_TOKEN_OUT "************************************************************"
#define TCM_TOKEN_SPACE " -------------------------------------------------------"
#define TCM_NEWLINE   "\r\n"

#define KEY_SIZE_256 32

#define CMD_STRATUP_STCLEAR					"StartUp(ST_Clear)                  "
#define CMD_PHYSICALENABLE					"PhysicalEnable                     "
#define CMD_PHYSICALDISABLE					"PhysicalDisable                    "
#define CMD_PHYSICALSETDEACTIVATED_FALSE	"PhysicalSetDeactivated(FALSE)      "
#define CMD_PHYSICALSETDEACTIVATED_TRUE		"PhysicalSetDeactivated(TRUE)       "
#define CMD_FORCECLEAR						"ForceClear                         "
#define CMD_SELFTEST						"SelfTest                           "
#define CMD_READPUBEK						"ReadPubEK                          "
#define CMD_CREATEEK					    "CreateEK                           "
#define CMD_PCEREAD							"PCR Read                           "
#define CMD_TESTTCMSTS						"TEST TCM Status                    "

#define CMD_GET_TCM_INFO					"Get TCMInfo                        "
#define CMD_TCM_DISABLE						"TCM disable                        "
#define CMD_TCM_ENABLE						"TCM enable                         "

//add by Kevin.Luo begin
#define CMD_TAKEOWNERSHIP					"TakeOwnerShip                      "
#define CMD_CALCMAC_AP						"CalcMac-ApCreate                   "
#define CMD_AP_CREATE						"ApCreate                           "
#define CMD_CALCMAC_OWNER					"CalcMac-Owner                      "
#define CMD_ENCDATA_AUTH					"EncData-Auth                       "
#define CMD_ENCDATA_SMK						"EncData-SMK                        "
#define CMD_CALCSCH							"CalcSCH                            "
//add by Kevin.Luo end


struct TCM_Status {
	BOOL enable;
	BOOL activate;
	BYTE version[4];
	BOOL EK_Installed;
	BOOL Owner_install;
} ;

/////////////////////////////////////////////////////////////////////////////
// CTCMTOOLDlg dialog

#ifndef TCM_TOOL_MAXPATH
#define TCM_TOOL_MAXPATH	256
#endif

#ifndef TCM_TOOL_MAXTIME
#define TCM_TOOL_MAXTIME	64
#endif

#ifndef TCM_TOOL_MAXMSG
#define TCM_TOOL_MAXMSG	512
#endif

#ifndef TCM_TOOL_ERR_SUCCESS
#define TCM_TOOL_ERR_SUCCESS	0
#endif

#ifndef TCM_TOOL_ERR_INIT
#define TCM_TOOL_ERR_INIT	-100001
#endif


class CTCMTOOLDlg
{
// Construction
public:
	CTCMTOOLDlg();
	virtual ~CTCMTOOLDlg();

public:

	BOOL OnInitDialog();
	void OnClose();

	int OnButtonEnable();
	int OnButtonStartAndSelftest();
	int OnButtonDisable();
	int OnButtonForceclear();
	int OnButtonPcrread();
	
	int OnButtonGettcminfo();
	int OnTakeOwnerShip();
	
	int OnButtonCreateek();

private:

	int GetVer( BYTE* outRsp ,int * len);
	
	INT32 PhysicalEnable();
	INT32 PhysicalDisable();
	INT32 PhysicalSetDeactivate(int deactivate);

	INT32 ForceClear();
	INT32 PCRRead();

	

	INT32 StartUp();
	INT32 SelfTest();

	INT32 CreateEK();

	INT32 PCRRead(int index);

	INT32 GetTCMStatus(TCM_Status *tcmstatus);

	INT32 GetTCM_EKExist(TCM_Status *tcmstatus,BYTE* pubKey=NULL);

	//INT32 ReadPubEK();

	INT32 ReadPubEK(BYTE* outPubKey=NULL);//modify by Kevin.Luo
	//add by Kevin.Luo begin
	//INT32 ReadPubEK(BYTE* outPubEK);
	INT32 CreateEndEK(TCM_Status *tcmstatus );
	INT32 EncData(BYTE* src,unsigned int srcSize,BYTE* pubKey,BYTE* dest,unsigned int* destSize );
	INT32 OwnerShip(BYTE* encOwnerData,int encOwnerDataSize,BYTE* encSMKData,int encSMKDataSize);
	INT32 CalcHMAC(BYTE*key,unsigned int keyLen,BYTE* s1Data, unsigned int s1Len,BYTE* s2Data,unsigned int s2Len,BYTE* outMac);
	INT32 TakeOwnership(BYTE* pubKey );//获取ownership
	INT32 CreateAPP(int entityType,int entityValue,BYTE* callNonce,BYTE* inMac,int* sessHandle,long*seq);
	INT32 CalcSCH(BYTE* inBuf,int inBufLen,BYTE* hash);

	const char* GetErrMsg(INT32 status );
	//add by Kevin.Luo end
	INT32 GetCapability(int cap, unsigned int sub, int* val, int* extra,BYTE* zteicID=NULL /*for get zteicID add by Kevin.Luo*/);

	void show_head(const char* s);
	void show_tail(int success);
	void show_operation(const char* s, UINT32 success);
	UINT32 show_operation_startup(const char* s, UINT32 success);
	void show_string(const char* s);
	void show_result();
	void show_output();
	void show_pcrvalue(BYTE * buf, int index);

public:

	WinDevice *m_TcmDev;

	BOOL m_isEKExist;

	BOOL m_Startup;

	CTString m_output_str;

	const char* m_ordinal;

private:
	void writeLogMsg(char* strMsg);
	BOOL m_bZteicFlag;
	CTString m_strOperLog;
	BOOL m_bFlag;//属于同一操作日志信息标志


};

#endif //___TCMTOOLDLG_H___
