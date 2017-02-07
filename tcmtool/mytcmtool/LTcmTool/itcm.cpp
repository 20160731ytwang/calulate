/*
 * File Name :  dev.cpp
 * Author :  zhangyz@litsoft.com.cn         
 * Date : 2006/11/10 									
 * Description :                       
 * Version : 0.1       		              
 * Function List : 
 */
#include "stdafx.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "tcm.h"
#include "itcm.h"

void
itcm_pack_const(BYTE* dst, int val, int size)
{
	switch (size)
	{
	case 4:
		dst[0] = (BYTE)((val >> 24) & 0xff);
		dst[1] = (BYTE)((val >> 16) & 0xff);
		dst[2] = (BYTE)((val >> 8) & 0xff);
		dst[3] = (BYTE)(val & 0xff);
		break;
	case 2:
		dst[0] = (BYTE)((val >> 8) & 0xff);
		dst[1] = (BYTE)(val & 0xff);
		break;
	case 1:
		dst[0] = (BYTE)(val & 0xff);
		break;
	}
}

int
itcm_unpack_const(const BYTE* src, int size)
{
	int val = 0;
	switch (size)
	{
	case 4:
		val = (((UINT32) src[0]) << 24
		       | ((UINT32) src[1]) << 16
		       | ((UINT32) src[2]) << 8
		       | (UINT32) src[3]);
		break;
	case 2:
		val = (((UINT32) src[0]) << 8 | (UINT32) src[1]);
		break;
	case 1:
		val = (UINT32) src[0];
		break;
	}		
	return val;
}

int
itcm_pack_simple(
	int cmd,
	int argsize,
	int arg,
	BYTE* dst,
	int max)
{
	int size = 10 + argsize;
	if (dst && max >= size)
	{
		itcm_pack_const(dst, TCM_TAG_RQU_COMMAND, 2);
		itcm_pack_const(dst + 2, size, 4);
		itcm_pack_const(dst + 6, cmd, 4);
		if (argsize > 0)
			itcm_pack_const(dst + 10, arg, argsize);
	}
	return size;
}

int
itcm_unpack_simple(
	const BYTE* src,
	int size,
	int argsize,
	int* arg)
{
	int tag, status;
	tag = itcm_unpack_const(src, 2);
	if (tag != TCM_TAG_RSP_COMMAND
	    && tag != TCM_TAG_RSP_AUTH1_COMMAND
	    && tag != TCM_TAG_RSP_AUTH2_COMMAND)
		return -1;
	if (itcm_unpack_const(src + 2, 4) != size)
		return -1;
	status = itcm_unpack_const(src + 6, 4);
	if (status == 0 && argsize > 0 && arg)
		*arg = itcm_unpack_const(src + 10, argsize);
	return status;
}
	
const char*
itcm_ord_to_str(int ord)
{
	static char strbuf[256];
	const char* str = "Unknown ordinal";
	switch (ord)
	{
	case TCM_ORD_OIAP: str = "TCM_ORD_OIAP"; break;
	case TCM_ORD_OSAP: str = "TCM_ORD_OSAP"; break;
	case TCM_ORD_ChangeAuth: str = "TCM_ORD_ChangeAuth"; break;
	case TCM_ORD_TakeOwnership: str = "TCM_ORD_TakeOwnership"; break;
	case TCM_ORD_ChangeAuthAsymStart:
		str = "TCM_ORD_ChangeAuthAsymStart"; break;
	case TCM_ORD_ChangeAuthAsymFinish:
		str = "TCM_ORD_ChangeAuthAsymFinish"; break;
	case TCM_ORD_ChangeAuthOwner: str = "TCM_ORD_ChangeAuthOwner"; break;
	case TCM_ORD_Extend: str = "TCM_ORD_Extend"; break;
	case TCM_ORD_PcrRead: str = "TCM_ORD_PcrRead"; break;
	case TCM_ORD_Quote: str = "TCM_ORD_Quote"; break;
	case TCM_ORD_Seal: str = "TCM_ORD_Seal"; break;
	case TCM_ORD_Unseal: str = "TCM_ORD_Unseal"; break;
	case TCM_ORD_DirWriteAuth: str = "TCM_ORD_DirWriteAuth"; break;
	case TCM_ORD_DirRead: str = "TCM_ORD_DirRead"; break;
	case TCM_ORD_UnBind: str = "TCM_ORD_UnBind"; break;
	case TCM_ORD_CreateWrapKey: str = "TCM_ORD_CreateWrapKey"; break;
	case TCM_ORD_LoadKey: str = "TCM_ORD_LoadKey"; break;
	case TCM_ORD_GetPubKey: str = "TCM_ORD_GetPubKey"; break;
	case TCM_ORD_EvictKey: str = "TCM_ORD_EvictKey"; break;
	case TCM_ORD_CreateMigrationBlob:
		str = "TCM_ORD_CreateMigrationBlob"; break;
	// TCM_ORD_ReWrapKey removed
	case TCM_ORD_ConvertMigrationBlob:
		str = "TCM_ORD_ConvertMigrationBlob"; break;
	case TCM_ORD_AuthorizeMigrationKey: 
		str = "TCM_ORD_AuthorizeMigrationKey"; break;
	case TCM_ORD_CreateMaintenanceArchive:
		str = "TCM_ORD_CreateMaintenanceArchive"; break;
	case TCM_ORD_LoadMaintenanceArchive:
		str = "TCM_ORD_LoadMaintenanceArchive"; break;
	case TCM_ORD_KillMaintenanceFeature:
		str = "TCM_ORD_KillMaintenanceFeature"; break;
	case TCM_ORD_LoadManuMaintPub: str = "TCM_ORD_LoadManuMaintPub"; break;
	case TCM_ORD_ReadManuMaintPub: str = "TCM_ORD_ReadManuMaintPub"; break;
	case TCM_ORD_CertifyKey: str = "TCM_ORD_CertifyKey"; break;
	case TCM_ORD_Sign: str = "TCM_ORD_Sign"; break;
	case TCM_ORD_GetRandom: str = "TCM_ORD_GetRandom"; break;
	case TCM_ORD_StirRandom: str = "TCM_ORD_StirRandom"; break;
	case TCM_ORD_SelfTestFull: str = "TCM_ORD_SelfTestFull"; break;
	case TCM_ORD_SelfTestStartup: str = "TCM_ORD_SelfTestStartup"; break;
	case TCM_ORD_CertifySelfTest: str = "TCM_ORD_CertifySelfTest"; break;
	case TCM_ORD_ContinueSelfTest: str = "TCM_ORD_ContinueSelfTest"; break;
	case TCM_ORD_GetTestResult: str = "TCM_ORD_GetTestResult"; break;
	case TCM_ORD_Reset: str = "TCM_ORD_Reset"; break;
	case TCM_ORD_OwnerClear: str = "TCM_ORD_OwnerClear"; break;
	case TCM_ORD_DisableOwnerClear:
		str = "TCM_ORD_DisableOwnerClear"; break;
	case TCM_ORD_ForceClear: str = "TCM_ORD_ForceClear"; break;
	case TCM_ORD_DisableForceClear:
		str = "TCM_ORD_DisableForceClear"; break;
	case TCM_ORD_GetCapabilitySigned:
		str = "TCM_ORD_GetCapabilitySigned"; break;
	case TCM_ORD_GetCapability: str = "TCM_ORD_GetCapability"; break;
	case TCM_ORD_GetCapabilityOwner:
		str = "TCM_ORD_GetCapabilityOwner"; break;
//add by dongrm
	case TCM_ORD_SetCapability: str = "TCM_ORD_SetCapability"; break;
//end add by dongrm
	case TCM_ORD_OwnerSetDisable: str = "TCM_ORD_OwnerSetDisable"; break;
	case TCM_ORD_PhysicalEnable: str = "TCM_ORD_PhysicalEnable"; break;
	case TCM_ORD_PhysicalDisable: str = "TCM_ORD_PhysicalDisable"; break;
	case TCM_ORD_SetOwnerInstall: str = "TCM_ORD_SetOwnerInstall"; break;
	case TCM_ORD_PhysicalSetDeactivated:
		str = "TCM_ORD_PhysicalSetDeactivated"; break;
	case TCM_ORD_SetTempDeactivated:
		str = "TCM_ORD_SetTempDeactivated"; break;
	case TCM_ORD_CreateEndorsementKeyPair:
		str = "TCM_ORD_CreateEndorsementKeyPair"; break;
	case TCM_ORD_MakeIdentity: str = "TCM_ORD_MakeIdentity"; break;
	case TCM_ORD_ActivateIdentity: str = "TCM_ORD_ActivateIdentity"; break;
	case TCM_ORD_ReadPubek: str = "TCM_ORD_ReadPubek"; break;
	case TCM_ORD_OwnerReadPubek: str = "TCM_ORD_OwnerReadPubek"; break;
	case TCM_ORD_DisablePubekRead: str = "TCM_ORD_DisablePubekRead"; break;
	case TCM_ORD_GetAuditEvent: str = "TCM_ORD_GetAuditEvent"; break;
	case TCM_ORD_GetAuditEventSigned:
		str = "TCM_ORD_GetAuditEventSigned"; break;
	case TCM_ORD_GetOrdinalAuditStatus:
		str = "TCM_ORD_GetOrdinalAuditStatus"; break;
	case TCM_ORD_SetOrdinalAuditStatus: 
		str = "TCM_ORD_SetOrdinalAuditStatus"; break;
	case TCM_ORD_Terminate_Handle: str = "TCM_ORD_Terminate_Handle"; break;
	case TCM_ORD_Init: str = "TCM_ORD_Init"; break;
	case TCM_ORD_SaveState: str = "TCM_ORD_SaveState"; break;
	case TCM_ORD_Startup: str = "TCM_ORD_Startup"; break;
	case TCM_ORD_SetRedirection: str = "TCM_ORD_SetRedirection"; break;
	case TCM_ORD_SHA1Start: str = "TCM_ORD_SHA1Start"; break;
	case TCM_ORD_SHA1Update: str = "TCM_ORD_SHA1Update"; break;
	case TCM_ORD_SHA1Complete: str = "TCM_ORD_SHA1Complete"; break;
	case TCM_ORD_SHA1CompleteExtend:
		str = "TCM_ORD_SHA1CompleteExtend"; break;
	case TCM_ORD_FieldUpgrade: str = "TCM_ORD_FieldUpgrade"; break;
	case TCM_ORD_SaveKeyContext: str = "TCM_ORD_SaveKeyContext"; break;
	case TCM_ORD_LoadKeyContext: str = "TCM_ORD_LoadKeyContext"; break;
	case TCM_ORD_SaveAuthContext: str = "TCM_ORD_SaveAuthContext"; break;
	case TCM_ORD_LoadAuthContext: str = "TCM_ORD_LoadAuthContext"; break;
	case TSC_ORD_PhysicalPresence: str = "TSC_ORD_PhysicalPresence"; break;
		// TCM 1.2 additions
	case TCM_ORD_DSAP: str = "TCM_ORD_DSAP"; break;
	case TCM_ORD_CMK_CreateTicket: str = "TCM_ORD_CMK_CreateTicket"; break;
	case TCM_ORD_CMK_CreateKey: str = "TCM_ORD_CMK_CreateKey"; break;
	case TCM_ORD_CMK_CreateBlob: str = "TCM_ORD_CMK_CreateBlob"; break;
	case TCM_ORD_CMK_ConvertMigration: str = "TCM_ORD_CMK_ConvertMigration"; break;
	case TCM_ORD_CMK_SetRestrictions:
		str = "TCM_ORD_CMK_SetRestrictions"; break;
	case TCM_ORD_CMK_ApproveMA: str = "TCM_ORD_CMK_ApproveMA"; break;
	case TCM_ORD_KeyControlOwner: str = "TCM_ORD_KeyControlOwner"; break;
	case TCM_ORD_CertifyKey2: str = "TCM_ORD_CertifyKey2"; break;
	case TCM_ORD_SetOperatorAuth: str = "TCM_ORD_SetOperatorAuth"; break;
	case TCM_ORD_SetOwnerPointer: str = "TCM_ORD_SetOwnerPointer"; break;
	case TCM_ORD_CreateRevocableEK:
		str = "TCM_ORD_CreateRevocableEK"; break;
	case TCM_ORD_RevokeTrust: str = "TCM_ORD_RevokeTrust"; break;
	case TCM_ORD_OwnerReadInternalPub: 
		str = "TCM_ORD_OwnerReadInternalPub"; break;
	case TCM_ORD_GetAuditDigest: str = "TCM_ORD_GetAuditDigest"; break;
	case TCM_ORD_GetAuditDigestSigned: 
		str = "TCM_ORD_GetAuditDigestSigned"; break;
	case TCM_ORD_FlushSpecific: str = "TCM_ORD_FlushSpecific"; break;
	case TCM_ORD_PCR_Reset: str = "TCM_ORD_PCR_Reset"; break;
	case TCM_ORD_Delegate_UpdateVerification: 
		str = "TCM_ORD_Delegate_UpdateVerification"; break;
	case TCM_ORD_Delegate_Manage: str = "TCM_ORD_Delegate_Manage"; break;
	case TCM_ORD_Delegate_CreateKeyDelegation: 
		str = "TCM_ORD_Delegate_CreateKeyDelegation"; break;
	case TCM_ORD_Delegate_CreateOwnerDelegation:
		str = "TCM_ORD_Delegate_CreateOwnerDelegation"; break;
	case TCM_ORD_Delegate_VerifyDelegation: 
		str = "TCM_ORD_Delegate_VerifyDelegation"; break;
	case TCM_ORD_Delegate_LoadOwnerDelegation: 
		str = "TCM_ORD_Delegate_LoadOwnerDelegation"; break;
	case TCM_ORD_Delegate_ReadTable: 
		str = "TCM_ORD_Delegate_ReadTable"; break;
	case TCM_ORD_NV_DefineSpace: str = "TCM_ORD_NV_DefineSpace"; break;
	case TCM_ORD_NV_WriteValue: str = "TCM_ORD_NV_WriteValue"; break;
	case TCM_ORD_NV_WriteValueAuth: 
		str = "TCM_ORD_NV_WriteValueAuth"; break;
	case TCM_ORD_NV_ReadValue: str = "TCM_ORD_NV_ReadValue"; break;
	case TCM_ORD_NV_ReadValueAuth: str = "TCM_ORD_NV_ReadValueAuth"; break;
	case TCM_ORD_CreateCounter: str = "TCM_ORD_CreateCounter"; break;
	case TCM_ORD_IncrementCounter: str = "TCM_ORD_IncrementCounter"; break;
	case TCM_ORD_ReadCounter: str = "TCM_ORD_ReadCounter"; break;
	case TCM_ORD_ReleaseCounter: str = "TCM_ORD_ReleaseCounter"; break;
	case TCM_ORD_ReleaseCounterOwner:
		str = "TCM_ORD_ReleaseCounterOwner"; break;
	case TCM_ORD_EstablishTransport: 
		str = "TCM_ORD_EstablishTransport"; break;
	case TCM_ORD_ExecuteTransport: str = "TCM_ORD_ExecuteTransport"; break;
	case TCM_ORD_ReleaseTransport: 
		str = "TCM_ORD_ReleaseTransport"; break;
	case TCM_ORD_SetTickType: str = "TCM_ORD_SetTickType"; break;
	case TCM_ORD_GetTicks: str = "TCM_ORD_GetTicks"; break;
	case TCM_ORD_TickStampBlob: str = "TCM_ORD_TickStampBlob"; break;
	case TSC_ORD_ResetEstablishmentBit: 
		str = "TSC_ResetEstablishmentBit"; break;
	case TCM_ORD_SaveContext: str = "TCM_ORD_SaveContext"; break;
	case TCM_ORD_LoadContext: str = "TCM_ORD_LoadContext"; break;
	case TCM_ORD_DAA_Join: str = "TCM_ORD_DAA_Join"; break;
	case TCM_ORD_DAA_Sign: str = "TCM_ORD_DAA_Sign"; break;
	case TCM_ORD_GPIO_AuthChannel: str = "TCM_ORD_GPIO_AuthChannel"; break;
	case TCM_ORD_GPIO_ReadWrite: str = "TCM_ORD_GPIO_ReadWrite"; break;
	case TCM_ORD_MigrateKey: str = "TCM_ORD_MigrateKey"; break;

	default:
		sprintf(strbuf, "Unknown ordinal (0x%x)", ord);
		str = strbuf;
		break;
	}
	return str;
}

const char*
itcm_status_to_str(int status)
{
	static char strbuf[256];
	const char* str = "Unknown error";
	switch (status)
	{
	case TCM_SUCCESS: str = "Success"; break;
	case TCM_AUTHFAIL: str = "Authorization failed"; break;
	case TCM_BADINDEX: str = "Bad index"; break;
	case TCM_BAD_PARAMETER: str = "Bad parameter"; break;
	case TCM_AUDITFAILURE: str = "Audit failure"; break;
	case TCM_CLEAR_DISABLED: str = "Clear disabled"; break;
	case TCM_DEACTIVATED: str = "Deactivated"; break;
	case TCM_DISABLED: str = "Disabled"; break;
	case TCM_DISABLED_CMD: str = "Disabled command"; break;
	case TCM_FAIL: str = "Fail"; break;
	case TCM_BAD_ORDINAL: str = "Bad ordinal"; break;
	case TCM_INSTALL_DISABLED: str = "Install disabled"; break;
	case TCM_INVALID_KEYHANDLE: str = "Invalid key handle"; break;
	case TCM_KEYNOTFOUND: str = "Key not found"; break;
	case TCM_INAPPROPRIATE_ENC: str = "Inappropriate encoding"; break;
	case TCM_MIGRATEFAIL: str = "Migration failed"; break;
	case TCM_INVALID_PCR_INFO: str = "Invalid PCR info"; break;
	case TCM_NOSPACE: str = "No space"; break;
	case TCM_NOSMK: str = "No SMK"; break;
	case TCM_NOTSEALED_BLOB: str = "Not sealed blob"; break;
	case TCM_OWNER_SET: str = "Owner set"; break;
	case TCM_RESOURCES: str = "Resources"; break;
	case TCM_SHORTRANDOM: str = "Short random"; break;
	case TCM_SIZE: str = "Size"; break;
	case TCM_WRONGPCRVAL: str = "Wrong PCR value"; break;
	case TCM_BAD_PARAM_SIZE: str = "Bad parameter size"; break;
	case TCM_SCH_THREAD: str = "SCH thread"; break;
	case TCM_SCH_ERROR: str = "SCH error"; break;
	case TCM_FAILEDSELFTEST: str = "Failed self test"; break;
	case TCM_AUTH2FAIL: str = "Authorization #2 failed"; break;
	case TCM_BADTAG: str = "Bad tag"; break;
	case TCM_IOERROR: str = "I/O error"; break;
	case TCM_ENCRYPT_ERROR: str = "Encrypt error"; break;
	case TCM_DECRYPT_ERROR: str = "Decrypt error"; break;
	case TCM_INVALID_AUTHHANDLE:
		str = "Invalid authorization handle"; break;
	case TCM_NO_ENDORSEMENT: str = "No endorsement"; break;
	case TCM_INVALID_KEYUSAGE: str = "Invalid key usage"; break;
	case TCM_WRONG_ENTITYTYPE: str = "Wrong entity type"; break;
	case TCM_INVALID_POSTINIT: str = "Invalid POST initialization"; break;
	case TCM_INAPPROPRIATE_SIG: str = "Inappropriate signature"; break;
	case TCM_BAD_KEY_PROPERTY: str = "Bad key properties"; break;
	case TCM_BAD_MIGRATION: str = "Bad migration properties"; break;
	case TCM_BAD_SCHEME: str = "Bad signature/encryption scheme"; break;
	case TCM_BAD_DATASIZE: str = "Bad data/blob size"; break;
	case TCM_BAD_MODE: str = "Bad mode parameter"; break;
	case TCM_BAD_PRESENCE: str = "Bad presence state"; break;
	case TCM_BAD_VERSION: str = "Bad capability version"; break;
	case TCM_RETRY: str = "Retry"; break;
		// TCM 1.2 additions
	case TCM_NO_WRAP_TRANSPORT: str = "No wrap transport"; break;
	case TCM_AUDITFAIL_UNSUCCESSFUL:
		str = "Audit fail unsuccessful"; break;
	case TCM_AUDITFAIL_SUCCESSFUL: str = "Audit fail successful"; break;
	case TCM_NOTRESETABLE: str = "Not resetable"; break;
	case TCM_NOTLOCAL: str = "Not local"; break;
	case TCM_BAD_TYPE: str = "Bad type"; break;
	case TCM_INVALID_RESOURCE: str = "Invalid resource"; break;
	case TCM_NOTFIPS: str = "Not FIPS"; break;
	case TCM_INVALID_FAMILY: str = "Invalid family"; break;
	case TCM_NO_NV_PERMISSION: str = "No NV permission"; break;
	case TCM_REQUIRES_SIGN: str = "Requires signature"; break;
	case TCM_KEY_NOTSUPPORTED: str = "Key not supported"; break;
	case TCM_AUTH_CONFLICT: str = "Auth conflict"; break;
	case TCM_AREA_LOCKED: str = "Area locked"; break;
	case TCM_BAD_LOCALITY: str = "Bad locality"; break;
	case TCM_READ_ONLY: str = "Read-only"; break;
	case TCM_PER_NOWRITE: str = "PER no write"; break;
	case TCM_FAMILYCOUNT: str = "Bad family count"; break;
	case TCM_WRITE_LOCKED: str = "Write locked"; break;
	case TCM_BAD_ATTRIBUTES: str = "Bad attributes"; break;
	case TCM_INVALID_STRUCTURE: str = "Invalid structure"; break;
	case TCM_KEY_OWNER_CONTROL: str = "Key owner control"; break;
	case TCM_BAD_COUNTER: str = "Bad counter"; break;
	case TCM_NOT_FULLWRITE: str = "Not full write"; break;
	case TCM_CONTEXT_GAP: str = "Context gap"; break;
	case TCM_MAXNVWRITES: str = "Maximum NV writes"; break;
	case TCM_NOOPERATOR: str = "No operator"; break;
	case TCM_RESOURCEMISSING: str = "Resource missing"; break;
	case TCM_DELEGATE_LOCK: str = "Delegate locked"; break;
	case TCM_DELEGATE_FAMILY: str = "Delegate family invalid"; break;
	case TCM_DELEGATE_ADMIN: str = "Delegation admin disabled"; break;
	case TCM_TRANS_EXCLUSIVE: str = "Transport exclusive"; break;
	case TCM_OWNER_CONTROL: str = "Owner control"; break;
	case TCM_DAA_RESOURCES: str = "DAA resource"; break;
	case TCM_DAA_INPUT_DATA0: str = "DAA data#0 invalid"; break;
	case TCM_DAA_INPUT_DATA1: str = "DAA data#1 invalid"; break;
	case TCM_DAA_ISSUER_SETTINGS:
		str = "DAA issuer settings invalid"; break;
	case TCM_DAA_TCM_SETTINGS:
		str = "DAA TCM settings invalid"; break;
	case TCM_DAA_STAGE: str = "DAA stage invalid"; break;
	case TCM_DAA_ISSUER_VALIDITY: str = "DAA issuer invalid"; break;
	case TCM_DAA_WRONG_W: str = "DAA wrong W"; break;
	case TCM_BAD_HANDLE: str = "Bad Handle"; break;						
	case TCM_NOCONTEXTSPACE: str = "No Context Space"; break;			
	case TCM_BADCONTEXT: str = "Bad Context"; break;					
	case TCM_TOOMANYCONTEXTS: str = "Too Many Contexts"; break;			
	case TCM_MA_TICKET_SIGNATURE: str = "MA Ticket Signature"; break;	
	case TCM_MA_DESTINATION: str = "MA Destination"; break;				
	case TCM_MA_SOURCE: str = "MA Source"; break;
	default:
		sprintf(strbuf, "Unknown error (0x%x)", status);
		str = strbuf;
		break;
	}
	return str;
}

const char*
itcm_tag_to_str(int tag)
{
	const char* str = "Unknown tag";
	switch (tag)
	{
	case TCM_TAG_RQU_COMMAND:
		str = "TCM_TAG_RQU_COMMAND";
		break;
	case TCM_TAG_RQU_AUTH1_COMMAND:
		str = "TCM_TAG_RQU_AUTH1_COMMAND";
		break;
	case TCM_TAG_RQU_AUTH2_COMMAND:
		str = "TCM_TAG_RQU_AUTH2_COMMAND";
		break;
	case TCM_TAG_RSP_COMMAND:
		str = "TCM_TAG_RSP_COMMAND";
		break;
	case TCM_TAG_RSP_AUTH1_COMMAND:
		str = "TCM_TAG_RSP_AUTH1_COMMAND";
		break;
	case TCM_TAG_RSP_AUTH2_COMMAND:
		str = "TCM_TAG_RSP_AUTH2_COMMAND";
		break;
	}
	return str;
}

const char*
itcm_manu_to_str(int value)
{
	const char* str = "Unknown";
	int val2 = (int) itcm_unpack_const((BYTE*) &value, 4);
	if (memcmp(&val2, "ATML", 4) == 0)
		str = "Atmel";
	else if (memcmp(&val2, "IFX\0", 4) == 0)
		str = "Infineon";
	else if (memcmp(&val2, "NSM ", 4) == 0)
		str = "National";
	else if (memcmp(&val2, "AAT.", 4) == 0)
		str = "STMicro";
	return str;
}
