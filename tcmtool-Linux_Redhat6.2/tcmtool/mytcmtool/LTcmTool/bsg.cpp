
/*
 * File Name :  bsg.cpp
 * Copyright (c) 2006 : lenovo
 * 主要从lenovo的代码中继承而来
 * Author :  zhangyz@litsoft.com.cn         
 * Date : 2006/11/10 									
 * Description :    主要用于将TCM中的数据结构转化为字符串                       
 * Version : 0.1       		              
 * Function List : 
 */
#include "stdafx.h"
#include "tcm.h"
#include "bsg.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef unsigned short      UINT16;

// flags
#define __FMT_CONST (1UL << 31)
#define __FMT_DATA  (1UL << 30)
#define __FMT_SIZE  (1UL << 29)
#define __FMT_COUNT (1UL << 28)
#define __FMT_MASK \
  ~(__FMT_CONST | __FMT_DATA | __FMT_SIZE | __FMT_COUNT)

// constant (8/16/32-bits)
#define FMT_U8 (__FMT_CONST | 1UL)
#define FMT_U16 (__FMT_CONST | 2UL)
#define FMT_U32 (__FMT_CONST | 4UL)

// other data (size bytes)
#define FMT_DATA(size) (__FMT_DATA | ((unsigned long) size & __FMT_MASK))

// 16/32-bit size followed by N bytes of data
#define FMT_SIZE16_DATA (__FMT_SIZE | 2UL)
#define FMT_SIZE32_DATA (__FMT_SIZE | 4UL)

// 16/32-bit count followed by (N * dsize) bytes of data
#define FMT_COUNT16_DATA(dsize) (__FMT_COUNT | ((dsize) << 4) | 2UL)
#define FMT_COUNT32_DATA(dsize) (__FMT_COUNT | ((dsize) << 4) | 4UL)

/*
 * TCM structure data formats
 */
BSG::Format BSG::s_fmt[] =
{
	{TCM_COMMAND_CODE, "TCM_COMMAND_CODE", {FMT_U32, 0}},
	{TCM_AUTH_DATA_USAGE, "TCM_AUTH_DATA_USAGE", {FMT_U8, 0}},
	{TCM_ALGORITHM_ID, "TCM_ALGORITHM_ID", {FMT_U32, 0}},
	{TCM_KEY_USAGE, "TCM_KEY_USAGE", {FMT_U16, 0}},
	{TCM_ENC_SCHEME, "TCM_ENC_SCHEME", {FMT_U16, 0}},
	{TCM_SIG_SCHEME, "TCM_SIG_SCHEME", {FMT_U16, 0}},
	{TCM_MIGRATE_SCHEME, "TCM_MIGRATE_SCHEME", {FMT_U16, 0}},
	{TCM_KEY_FLAGS, "TCM_KEY_FLAGS", {FMT_U32, 0}},

	{TCM_AUTHDATA, "TCM_AUTHDATA", {FMT_DATA(TCM_DIGEST_SIZE), 0}},
	{TCM_SECRET, "TCM_SECRET", {TCM_AUTHDATA, 0}},
	{TCM_ENCAUTH, "TCM_ENCAUTH", {TCM_AUTHDATA, 0}},
	{TCM_PAYLOAD_TYPE, "TCM_PAYLOAD_TYPE", {FMT_U8, 0}},

	{TCM_VERSION, "TCM_VERSION", {FMT_DATA(4), 0}},
	{TCM_DIGEST, "TCM_DIGEST", {FMT_DATA(TCM_DIGEST_SIZE), 0}},
	{TCM_COMPOSITE_HASH, "TCM_COMPOSITE_HASH", {TCM_DIGEST, 0}},
	{TCM_CHOSENID_HASH, "TCM_CHOSENID_HASH", {TCM_DIGEST, 0}},

	{TCM_NONCE, "TCM_NONCE", {FMT_DATA(TCM_DIGEST_SIZE), 0}},
	{TCM_KEY_HANDLE_LIST, "TCM_KEY_HANDLE_LIST", {FMT_COUNT16_DATA(4), 0}},

	{TCM_CHANGEAUTH_VALIDATE, "TCM_CHANGEAUTH_VALIDATE", {
		TCM_SECRET,
		TCM_NONCE,
		0}},

	{TCM_KEY_PARMS, "TCM_KEY_PARMS", {
		TCM_ALGORITHM_ID,
		TCM_ENC_SCHEME,
		TCM_SIG_SCHEME,
		FMT_SIZE32_DATA,
		0}},
	//add by zhanghuajun 2006-11-15
    {TCM_ECC_KEY_PARMS, "TCM_ECC_KEY_PARMS", {
		FMT_U32, FMT_U32, FMT_SIZE32_DATA, 0}},
	//end by zhanghuajun 2006-11-15
	{TCM_RSA_KEY_PARMS, "TCM_RSA_KEY_PARMS", {
		FMT_U32, FMT_U32, FMT_SIZE32_DATA, 0}},
	{TCM_STORE_PUBKEY, "TCM_STORE_PUBKEY", {FMT_SIZE32_DATA, 0}},
	{TCM_PUBKEY, "TCM_PUBKEY", {TCM_KEY_PARMS, TCM_STORE_PUBKEY, 0}},
 	{TCM_KEY, "TCM_KEY", {	
		TCM_STRUCTURE_TAG,
		FMT_U16,			
		TCM_KEY_USAGE,
		TCM_KEY_FLAGS,
		TCM_AUTH_DATA_USAGE,
		TCM_KEY_PARMS,
		FMT_SIZE32_DATA,
		TCM_STORE_PUBKEY,
		FMT_SIZE32_DATA,
		0}},
	{TCM_MIGRATIONKEYAUTH, "TCM_MIGRATIONKEYAUTH", {
		TCM_PUBKEY, TCM_MIGRATE_SCHEME, TCM_DIGEST, 0}},
	{TCM_EVENT_CERT, "TCM_EVENT_CERT", {
		TCM_DIGEST,
		TCM_DIGEST,
		FMT_DATA(2),
		FMT_SIZE32_DATA,
		0}},
	{TCM_PCR_SELECTION, "TCM_PCR_SELECTION", {FMT_SIZE16_DATA, 0}},
	{TCM_PCR_COMPOSITE, "TCM_PCR_COMPOSITE", {
		TCM_PCR_SELECTION, FMT_SIZE32_DATA, 0}},
/*
	{TCM_STORED_DATA, "TCM_STORED_DATA", {
		TCM_VERSION,
		FMT_SIZE32_DATA,
		FMT_SIZE32_DATA,
		0}},*/
	{TCM_SEALED_DATA, "TCM_SEALED_DATA", {
		TCM_PAYLOAD_TYPE,
		TCM_SECRET,
		TCM_NONCE,
		TCM_DIGEST,
		FMT_SIZE32_DATA,
		0}},
	{TCM_SYMMETRIC_KEY, "TCM_SYMMETRIC_KEY", {
		TCM_ALGORITHM_ID,
		TCM_ENC_SCHEME,
		FMT_SIZE16_DATA,
		0}},
	{TCM_STORE_PRIVKEY, "TCM_STORE_PRIVKEY", {FMT_SIZE32_DATA, 0}},
	{TCM_STORE_ASYMKEY, "TCM_STORE_ASYMKEY", {
		TCM_PAYLOAD_TYPE,
		TCM_SECRET,
		TCM_SECRET,
		TCM_DIGEST,
		TCM_STORE_PRIVKEY,
		0}},
	{TCM_MIGRATE_ASYMKEY, "TCM_MIGRATE_ASYMKEY", {
		TCM_PAYLOAD_TYPE,
		TCM_SECRET,
		TCM_DIGEST,
		FMT_U32,
		TCM_STORE_PRIVKEY,
		0}},
	{TCM_CERTIFY_INFO, "TCM_CERTIFY_INFO", {
//		TCM_STRUCTURE_TAG,
//		FMT_U8,
//		TCM_PAYLOAD_TYPE,
		TCM_VERSION,
		TCM_KEY_USAGE,
		TCM_KEY_FLAGS,
		TCM_AUTH_DATA_USAGE,
		TCM_KEY_PARMS,
		TCM_DIGEST,
		TCM_NONCE,
		FMT_U8,
		FMT_SIZE32_DATA,
		0}},
	{TCM_QUOTE_INFO, "TCM_QUOTE_INFO", {
		TCM_STRUCTURE_TAG,
		FMT_DATA(4),
		TCM_NONCE,
        TCM_PCR_INFO,
		0}},
	{TCM_IDENTITY_CONTENTS, "TCM_IDENTITY_CONTENTS", {
		TCM_VERSION,
		FMT_U32,
		TCM_CHOSENID_HASH,
		TCM_PUBKEY,
		0}},
	{TCM_BOUND_DATA, "TCM_BOUND_DATA",
	 {TCM_VERSION, TCM_PAYLOAD_TYPE, 0}},
	{TCM_HMAC, "TCM_HMAC",
	 {TCM_DIGEST, 0}},
	 {TCM_ASYM_CA_CONTENTS, "TCM_ASYM_CA_CONTENTS",
	 {TCM_SYMMETRIC_KEY, TCM_DIGEST, 0}},



	{TCM_STRUCTURE_TAG, "TCM_STRUCTURE_TAG", {FMT_U16, 0}},
	{TCM_DELEGATE_LABEL, "TCM_DELEGATE_LABEL", {FMT_U8, 0}},
	{TCM_LOCALITY_SELECTION, "TCM_LOCALITY_SELECTION", {FMT_U8, 0}},
	{TCM_MODIFIER_INDICATOR, "TCM_MODIFIER_INDICATOR", {FMT_U32, 0}},
	{TCM_FAMILY_ID, "TCM_FAMILY_ID", {FMT_U32, 0}},
	{TCM_FAMILY_VERIFICATION, "TCM_FAMILY_VERIFICATION", {FMT_U32, 0}},
	{TCM_TRANSPORT_ATTRIBUTES, "TCM_TRANSPORT_ATTRIBUTES", {FMT_U32, 0}},
	{TCM_PLATFORM_SPECIFIC, "TCM_PLATFORM_SPECIFIC", {FMT_U16, 0}},

	{TCM_PCR_INFO, "TCM_PCR_INFO",
	 {TCM_STRUCTURE_TAG,
	  TCM_LOCALITY_SELECTION, TCM_LOCALITY_SELECTION,
	  TCM_PCR_SELECTION, TCM_PCR_SELECTION,
	  TCM_COMPOSITE_HASH, TCM_COMPOSITE_HASH, 0}},

	{TCM_DELEGATIONS, "TCM_DELEGATIONS",
	 {TCM_STRUCTURE_TAG, FMT_U32, FMT_U32, FMT_U32, 0}},
	{TCM_DELEGATE_PUBLIC, "TCM_DELEGATE_PUBLIC",
	 {TCM_STRUCTURE_TAG, TCM_DELEGATE_LABEL, TCM_PCR_INFO,
	  TCM_DELEGATIONS, TCM_FAMILY_ID, TCM_FAMILY_VERIFICATION, 0}},
	{TCM_DELEGATE_TABLE_ROW, "TCM_DELEGATE_TABLE_ROW",
	 {TCM_STRUCTURE_TAG, TCM_DELEGATE_PUBLIC, TCM_SECRET, 0}},
	{TCM_DELEGATE_OWNER_BLOB, "TCM_DELEGATE_OWNER_BLOB",
	 {TCM_STRUCTURE_TAG, TCM_DELEGATE_PUBLIC,
	  TCM_DIGEST, FMT_SIZE32_DATA, FMT_SIZE32_DATA, 0}},
	{TCM_DELEGATE_KEY_BLOB, "TCM_DELEGATE_KEY_BLOB",
	 {TCM_STRUCTURE_TAG, TCM_DELEGATE_PUBLIC,
	  TCM_DIGEST, TCM_DIGEST, FMT_SIZE32_DATA, FMT_SIZE32_DATA, 0}},

	{TCM_CURRENT_TICKS, "TCM_CURRENT_TICKS",
	 {TCM_STRUCTURE_TAG, FMT_U32, FMT_U32, FMT_U16, TCM_NONCE, 0}},

	{TCM_NV_ATTRIBUTES, "TCM_NV_ATTRIBUTES",
	 {TCM_STRUCTURE_TAG, FMT_U32, 0}},
	{TCM_NV_DATA_PUBLIC, "TCM_NV_DATA_PUBLIC",
	 {TCM_STRUCTURE_TAG,
	  FMT_U32,
	  TCM_PCR_INFO,
	  TCM_PCR_INFO,
	  TCM_NV_ATTRIBUTES,
	  FMT_U8,
	  FMT_U8,
	  FMT_U8,
	  FMT_U32,
	  0}},

	{TCM_TRANSPORT_PUBLIC, "TCM_TRANSPORT_PUBLIC",
	 {TCM_STRUCTURE_TAG, TCM_TRANSPORT_ATTRIBUTES,
	  TCM_ALGORITHM_ID, TCM_ENC_SCHEME, 0}},
	{TCM_TRANSPORT_LOG_IN, "TCM_TRANSPORT_LOG_IN",
	 {TCM_STRUCTURE_TAG, TCM_DIGEST, TCM_DIGEST, 0}},
	{TCM_TRANSPORT_LOG_OUT, "TCM_TRANSPORT_LOG_OUT",
	 {TCM_STRUCTURE_TAG, TCM_CURRENT_TICKS, TCM_DIGEST,
	  TCM_MODIFIER_INDICATOR, 0}},
	{TCM_TRANSPORT_AUTH, "TCM_TRANSPORT_AUTH",
	 {TCM_STRUCTURE_TAG, TCM_AUTHDATA, 0}},
	{TCM_MSA_COMPOSITE, "TCM_MSA_COMPOSITE", {FMT_COUNT32_DATA(20), 0}},
	{TCM_CMK_AUTH, "TCM_CMK_AUTH",
	 {TCM_DIGEST, TCM_DIGEST, TCM_DIGEST, 0}},

	{TCM_COUNTER_VALUE, "TCM_COUNTER_VALUE",
	 {TCM_STRUCTURE_TAG, FMT_U32, FMT_U32, 0}},

	{TCM_GPIO_ATTRIBUTES, "TCM_GPIO_ATTRIBUTES", {FMT_U32, 0}},
	{TCM_GPIO_BUS, "TCM_GPIO_BUS", {FMT_U32, 0}},
	{TCM_GPIO_CHANNEL, "TCM_GPIO_CHANNEL",
	 {TCM_STRUCTURE_TAG, 
	  TCM_PLATFORM_SPECIFIC,
	  FMT_U16,
	  TCM_GPIO_ATTRIBUTES,
	  TCM_GPIO_BUS,
	  FMT_SIZE32_DATA,
	  FMT_SIZE32_DATA,
	  FMT_U32,
	  TCM_PCR_INFO,
	  0}},
	{TCM_GPIO_AUTHORIZE, "TCM_GPIO_AUTHORIZE",
	 {TCM_STRUCTURE_TAG,
	  TCM_GPIO_CHANNEL,
	  TCM_DIGEST,
	  FMT_SIZE32_DATA,
	  FMT_SIZE32_DATA,
	  0}},

	{TCM_RESOURCE_TYPE, "TCM_RESOURCE_TYPE", {FMT_U32, 0}},
	{TCM_CONTEXT_BLOB, "TCM_CONTEXT_BLOB",
	 {TCM_STRUCTURE_TAG,
	  TCM_RESOURCE_TYPE,
	  FMT_U32,
	  FMT_DATA(16),
	  FMT_U32,
	  TCM_DIGEST,
	  FMT_SIZE32_DATA,
	  FMT_SIZE32_DATA,
	  0}},

	{TCM_CERTIFY_INFO2, "TCM_CERTIFY_INFO2", {
		TCM_STRUCTURE_TAG,
		FMT_U16,	
		TCM_KEY_USAGE,
		TCM_KEY_FLAGS,
		TCM_AUTH_DATA_USAGE,
		TCM_KEY_PARMS,
		TCM_DIGEST,
		TCM_NONCE,
		FMT_U8,
		FMT_SIZE32_DATA,
		FMT_SIZE32_DATA,
		0}},
	{TCM_SIGN_INFO, "TCM_SIGN_INFO",
	 {TCM_STRUCTURE_TAG,
	  FMT_DATA(4),
	  TCM_NONCE,
	  FMT_SIZE32_DATA,
	  0}},
	{TCM_FAMILY_LABEL, "TCM_FAMILY_LABEL", {FMT_U8,0}},
	{TCM_FAMILY_FLAGS, "TCM_FAMILY_FLAGS", {FMT_U32,0}},
	{TCM_FAMILY_TABLE_ENTRY, "TCM_FAMILY_TABLE_ENTRY",
		{TCM_STRUCTURE_TAG ,TCM_FAMILY_LABEL, TCM_FAMILY_ID,
		TCM_FAMILY_VERIFICATION, TCM_FAMILY_FLAGS,0}},
	{TCM_STORED_DATA, "TCM_STORED_DATA",
	 {TCM_STRUCTURE_TAG,
	  FMT_U16,
	  FMT_SIZE32_DATA,
	  FMT_SIZE32_DATA,
	  0}},
	{TCM_PERMANENT_FLAGS, "TCM_PERMANENT_FLAGS",
	 {TCM_STRUCTURE_TAG,
	  FMT_U16,
	  0}},
	{TCM_STCLEAR_FLAGS, "TCM_STCLEAR_FLAGS",
	 {TCM_STRUCTURE_TAG,
	  FMT_U8,
	  FMT_U8,
	  FMT_U8,
	  FMT_U8,
	  FMT_U8,
	  0}},

	{TCM_CAP_VERSION_INFO,"TCM_CAP_VERSION_INFO",
		{TCM_STRUCTURE_TAG,
		TCM_VERSION,
		FMT_U16,
		FMT_U8,
		FMT_U32,
		FMT_SIZE16_DATA,
		0}},

	{TCM_EK_BLOB,"TCM_EK_BLOB",
		{TCM_STRUCTURE_TAG,
		TCM_EK_TYPE,
		FMT_SIZE32_DATA,
		0}},
	{(Type) 0},
};

/*
 * tcm command information, 用于传输(Transport)
 */
BSG::CmdInfo BSG::s_cmd[] =
{
//	{TCM_ORD_OIAP, 0, 0, 0},
//	{TCM_ORD_OSAP, 0, 0, 0},
	{TCM_ORD_ChangeAuth, 2, 1, 0},
	{TCM_ORD_TakeOwnership, 1, 0, 0},
//	{TCM_ORD_ChangeAuthAsymStart, 1, 1, 0},
//	{TCM_ORD_ChangeAuthAsymFinish, 1, 2, 0},
	{TCM_ORD_ChangeAuthOwner, 1, 0, 0},
	{TCM_ORD_Extend, 0, 0, 0},
	{TCM_ORD_PcrRead, 0, 0, 0},
	{TCM_ORD_Quote, 1, 1, 0},
	{TCM_ORD_Seal, 1, 1, 0},
	{TCM_ORD_Unseal, 2, 1, 0},
//	{TCM_ORD_DirWriteAuth, 1, 0, 0},
//	{TCM_ORD_DirRead, 0, 0, 0},
//	{TCM_ORD_UnBind, 1, 1, 0},
	{TCM_ORD_CreateWrapKey, 1, 1, 0},
	{TCM_ORD_LoadKey, 1, 1, 1},
	{TCM_ORD_GetPubKey, 1, 1, 0},
//	{TCM_ORD_EvictKey, 0, 1, 0},
	{TCM_ORD_CreateMigrationBlob, 2, 1, 0},
	{TCM_ORD_ConvertMigrationBlob, 2, 2, 0},
	{TCM_ORD_AuthorizeMigrationKey, 1, 0, 0},
//	{TCM_ORD_CreateMaintenanceArchive, 1, 0, 0},
//	{TCM_ORD_LoadMaintenanceArchive, 1, 0, 0},
//	{TCM_ORD_KillMaintenanceFeature, 1, 0, 0},
//	{TCM_ORD_LoadManuMaintPub, 0, 0, 0},
//	{TCM_ORD_ReadManuMaintPub, 0, 0, 0},
	{TCM_ORD_CertifyKey, 2, 2, 0},
	{TCM_ORD_Sign, 1, 1, 0},

	{TCM_ORD_SetCapability, 1, 0, 0},            
	{TCM_ORD_ResetLockValue, 1, 0, 0},           
	
	{TCM_ORD_GetRandom, 0, 0, 0},
	{TCM_ORD_StirRandom, 0, 0, 0},
	{TCM_ORD_SelfTestFull, 0, 0, 0},
//	{TCM_ORD_CertifySelfTest, 1, 1, 0},
	{TCM_ORD_ContinueSelfTest, 0, 0, 0},
	{TCM_ORD_GetTestResult, 0, 0, 0},
//	{TCM_ORD_Reset, 0, 0, 0},
	{TCM_ORD_OwnerClear, 1, 0, 0},
	{TCM_ORD_DisableOwnerClear, 1, 0, 0},
	{TCM_ORD_ForceClear, 0, 0, 0},
	{TCM_ORD_DisableForceClear, 0, 0, 0},
//	{TCM_ORD_GetCapabilitySigned, 1, 1, 0},
	{TCM_ORD_GetCapability, 0, 0, 0},
//	{TCM_ORD_GetCapabilityOwner, 1, 0, 0},
	{TCM_ORD_OwnerSetDisable, 1, 0, 0},
	{TCM_ORD_PhysicalEnable, 0, 0, 0},
	{TCM_ORD_PhysicalDisable, 0, 0, 0},
	{TCM_ORD_SetOwnerInstall, 0, 0, 0},
	{TCM_ORD_PhysicalSetDeactivated, 0, 0, 0},
	{TCM_ORD_SetTempDeactivated, 0, 0, 0},
	{TCM_ORD_CreateEndorsementKeyPair, 0, 0, 0},
	{TCM_ORD_MakeIdentity, 2, 0, 0},
	{TCM_ORD_ActivateIdentity, 2, 1, 0},
	{TCM_ORD_ReadPubek, 0, 0, 0},
	{TCM_ORD_OwnerReadPubek, 1, 0, 0},
	{TCM_ORD_DisablePubekRead, 1, 0, 0},
//	{TCM_ORD_GetAuditEvent, 0, 0, 0},
//	{TCM_ORD_GetAuditEventSigned, 1, 1, 0},
//	{TCM_ORD_GetOrdinalAuditStatus, 0, 0, 0},
	{TCM_ORD_SetOrdinalAuditStatus, 1, 0, 0},
	{TCM_ORD_Terminate_Handle, 0, 1, 0},
//	{TCM_ORD_Init, 0, 0, 0},
	{TCM_ORD_SaveState, 0, 0, 0},
	{TCM_ORD_Startup, 0, 0, 0},
//	{TCM_ORD_SetRedirection, 1, 1, 0},
//	{TCM_ORD_SHA1Start, 0, 0, 0},
//	{TCM_ORD_SHA1Update, 0, 0, 0},
//	{TCM_ORD_SHA1Complete, 0, 0, 0},
//	{TCM_ORD_SHA1CompleteExtend, 0, 0, 0},
	{TCM_ORD_FieldUpgrade, 1, 0, 0},
//	{TCM_ORD_SaveKeyContext, 0, 1, 0},
//	{TCM_ORD_LoadKeyContext, 0, 0, 1},
//	{TCM_ORD_SaveAuthContext, 0, 1, 0},
//	{TCM_ORD_LoadAuthContext, 0, 0, 1},
	{TSC_ORD_PhysicalPresence, 0, 0, 0},
//	{TCM_ORD_DSAP, 0, 0, 1},
//	{TCM_ORD_CMK_CreateTicket, 1, 0, 0},
//	{TCM_ORD_CMK_CreateKey, 1, 1, 0},
//	{TCM_ORD_CMK_CreateBlob, 1, 1, 0},
//	{TCM_ORD_CMK_SetRestrictions, 1, 0, 0},
//	{TCM_ORD_KeyControlOwner, 1, 1, 0},
//	{TCM_ORD_CreateMaintenanceArchive, 1, 0, 0},
//	{TCM_ORD_LoadMaintenanceArchive, 1, 0, 0},
//	{TCM_ORD_KillMaintenanceFeature, 1, 0, 0},
//	{TCM_ORD_LoadManuMaintPub, 0, 0, 0},
//	{TCM_ORD_ReadManuMaintPub, 0, 0, 0},
	{TCM_ORD_CertifyKey2, 2, 2, 0},
	{TCM_ORD_SetOperatorAuth, 0, 0, 0},
//	{TCM_ORD_SetOwnerPointer, 0, 0, 0},
	{TCM_ORD_CreateRevocableEK, 0, 0, 0},
	{TCM_ORD_RevokeTrust, 0, 0, 0},
	{TCM_ORD_OwnerReadInternalPub, 1, 1, 0},
//	{TCM_ORD_GetAuditEvent, 0, 0, 0},
//	{TCM_ORD_GetAuditEventSigned, 1, 1, 0},
	{TCM_ORD_GetAuditDigest, 0, 0, 0},
	{TCM_ORD_GetAuditDigestSigned, 1, 1, 0},
	{TCM_ORD_SetOrdinalAuditStatus, 1, 0, 0},
//	{TCM_ORD_GetOrdinalAuditStatus, 0, 0, 0},
	{TCM_ORD_FlushSpecific, 0, 1, 0},
	{TCM_ORD_PCR_Reset, 0, 0, 0},
	{TCM_ORD_Delegate_UpdateVerification, 1, 0, 0},
	{TCM_ORD_Delegate_Manage, 1, 0, 0},
	{TCM_ORD_Delegate_CreateKeyDelegation, 1, 1, 0},
	{TCM_ORD_Delegate_CreateOwnerDelegation, 1, 0, 0},
	{TCM_ORD_Delegate_VerifyDelegation, 0, 0, 0},
	{TCM_ORD_Delegate_LoadOwnerDelegation, 1, 0, 0},
	{TCM_ORD_Delegate_ReadTable, 0, 0, 0},
	{TCM_ORD_NV_DefineSpace, 1, 0, 0},
	{TCM_ORD_NV_WriteValue, 1, 0, 0},
	{TCM_ORD_NV_WriteValueAuth, 1, 0, 0},
	{TCM_ORD_NV_ReadValue, 1, 0, 0},
	{TCM_ORD_NV_ReadValueAuth, 1, 0, 0},
	{TCM_ORD_CreateCounter, 1, 0, 0},
	{TCM_ORD_IncrementCounter, 1, 0, 0},
	{TCM_ORD_ReadCounter, 0, 0, 0},
	{TCM_ORD_ReleaseCounter, 1, 0, 0},
	{TCM_ORD_ReleaseCounterOwner, 1, 0, 0},
	{TCM_ORD_EstablishTransport, 1, 1, 1},
	{TCM_ORD_ExecuteTransport, 1, 0, 0},
//	{TCM_ORD_ReleaseTransport, 1, 1, 0},
	{TCM_ORD_GetTicks, 0, 0, 0},
	{TCM_ORD_TickStampBlob, 1, 1, 0},
	{TSC_ORD_ResetEstablishmentBit, 0, 0, 0},
	{TCM_ORD_SMS4Encrypt, 1, 1, 0},
	{TCM_ORD_SMS4Decrypt, 1, 1, 0},
	{TCM_ORD_ECCDecrypt, 1, 1, 0},
    {TCM_ORD_Sign, 1, 1, 0},
	{TCM_ORD_SCHStart, 0, 0, 0},
	{TCM_ORD_SCHUpdate, 0, 0, 0},
	{TCM_ORD_SCHComplete, 0, 0, 0},
	{TCM_ORD_SCHCompleteExtend, 0, 0, 0},
	{TCM_ORD_WrapKey, 1, 1, 0},
	{TCM_ORD_APCreate, 0, 0, 1},
	{TCM_ORD_APTerminate, 1, 0, 0},
	{TCM_ORD_ReadEKCert, 1, 0, 0},
	{TCM_ORD_FlushSpecific, 0, 1, 0},
	{TCM_ORD_CreateECCExchangeSession, 1, 0, 1},
	{TCM_ORD_GetKeyECCExchangeSession, 1, 2, 0},
	{TCM_ORD_ReleaseECCExchangeSession, 0, 1, 0},
	{0, 0, 0, 0}
};

/**
 * Find command information
 * @ord: Ordinal
 * Returns: Command information or NULL on error
 */
const BSG::CmdInfo*
BSG::Find(int ord)
{
	const CmdInfo* cmd = s_cmd;
	while (cmd->ord && cmd->ord != ord)
		cmd++;
	return (cmd->ord == ord) ? cmd:NULL;
}

/**
 * Flatten a TCM structure into a buffer in big-endian format
 * @type: TCM structure type
 * @src: (IN) TCM structure (OUT) end of TCM structure
 * @dst: (OUT) flattened data
 * Returns: Flattened size
 */
int
BSG::Pack(Type type, const BYTE** src, BYTE* dst)
{
	const BYTE* s = *src;
	BYTE* d = dst;

	unsigned int size = (type & __FMT_MASK);
	if (type & __FMT_CONST)
	{
		unsigned long val = 0;
		switch (size)
		{
		case 1: val = *s; break;
		case 2: val = *(unsigned short*) s; break;
		case 4: val = *(UINT32*) s; break;
		}
		s += size;
		if (dst)
			PackConst(val, size, d);
		d += size;
	}
	else if (type & __FMT_DATA)
	{
		if (dst)
			memcpy(d, s, size);
		s += size;
		d += size;
	}
	else if (type & __FMT_COUNT)
	{
		int dsize = size >> 4;
		size &= 0xf;
		int dcnt = 0;
		switch (size)
		{
		case 1: dcnt = (int) *s; break;
		case 2: dcnt = (int) *(UINT16*) s; break;
		case 4: dcnt = (int) *(UINT32*) s; break;
		}

		s += size;
		if (dst)
			PackConst(dcnt, size, d);
		d += size;
		BYTE* pdata = *(BYTE**) s;
		s += sizeof(BYTE*);

		if (pdata && dst && dsize && dcnt)
		{
			int x;
			for (x = 0; x < dcnt; x++)
			{
				switch (dsize)
				{
				case 1: *d = *pdata; break;
				case 2:	BSG::Pack16(d, *(UINT16*) pdata);
					break;
				case 4: BSG::Pack32(d, *(UINT32*)pdata); break;
				default: memcpy(d, pdata, dsize); break;
				}
				d += dsize;
				pdata += dsize;
			}
		}
		else
			d += dsize * dcnt;
			
	}
	else if (type & __FMT_SIZE)
	{
		unsigned long psize = 0;
		switch (size)
		{
		case 1: psize = *s; break;
		case 2: psize = *(unsigned short*) s; break;
		case 4: psize = *(UINT32*) s; break;
		}
		s += size;
		if (dst)
			PackConst(psize, size, d);
		d += size;

		BYTE* pdata = *(BYTE**) s;
		s += sizeof(BYTE*);
		if (dst && psize)
			memcpy(d, pdata, psize);
		d += psize;
	}
	else
	{
		Format* x = s_fmt;
		for (; x->type && x->type != type; x++) /* empty */ ;
		if (!x->type)
		{
			printf("BSG::Pack: cannot find %d\n", x->type);
			return -1;
		}
		unsigned long* f = x->fields;
		for (; *f; f++)
		{
			int fsize = Pack((Type) *f, &s, dst ? d:NULL);
			if (fsize <= 0)
				return fsize;
			d += fsize;
		}
	}

	*src = s;
	return (d - dst);
}

/**
 * Unflatten a TCM structure from a buffer in big-endian format
 * @type: TCM structure type
 * @src: flattened data
 * @dst: (IN) TCM structure (OUT) end of TCM structure
 * Returns: Flattened size
 * Note: Returns flattened size NOT the unpacked structure size
 */
int
BSG::Unpack(Type type, const BYTE* src, BYTE** dst)
{
	const BYTE* s = src;
	BYTE* d = dst ? *dst:NULL;
	if (dst && !d)
		dst = NULL;

	unsigned int size = type & __FMT_MASK;
	if (type & __FMT_CONST)
	{
		unsigned long val = UnpackConst(s, size);
		s += size;
		if (dst)
		{
			switch (size)
			{
			case 1: *d = (BYTE) val; break;
			case 2:
				*(unsigned short*) d = (unsigned short) val;
				break;
			case 4: *(UINT32*) d = (UINT32) val; break;
			}
		}
		d += size;
	}
	else if (type & __FMT_DATA)
	{
		if (dst)
			memcpy(d, s, size);
		d += size;
		s += size;
	}
	else if (type & __FMT_COUNT)
	{
		int dsize = size >> 4;
		size &= 0xf;
		int dcnt = UnpackConst(s, size);
		s += size;
		if (dst)
		{
			switch (size)
			{
			case 1: *d = (BYTE) dcnt; break;
			case 2:	*(UINT16*) d = (UINT16) dcnt; break;
			case 4: *(UINT32*) d = (UINT32) dcnt; break;
			}
		}
		d += size;

		BYTE* pdata = NULL;
		if (dst && dsize && dcnt)
		{
			pdata = new BYTE[dsize * dcnt];
			BYTE* pd = pdata;
			int x;
			for (x = 0; x < dcnt; x++)
			{
				switch (dsize)
				{
				case 1: *pd = *s; break;
				case 2: *(UINT16*) pd = BSG::Unpack16(s);
					break;
				case 4: *(UINT32*) pd = BSG::Unpack32(s);
					break;
				default: memcpy(pd, s, dsize); break;
				}
				pd += dsize;
				s += dsize;
			}
		}
		if (dst)
			*(void**) d = pdata;
		d += sizeof(void*);
	}
	else if (type & __FMT_SIZE)
	{
		unsigned long psize = UnpackConst(s, size);
		s += size;
		if (dst)
		{
			switch (size)
			{
			case 1: *d = (BYTE) psize; break;
			case 2:
				*(unsigned short*) d = (unsigned short) psize;
				break;
			case 4: *(UINT32*) d = (UINT32) psize; break;
			}
		}
		d += size;

		BYTE* pdata = NULL;
		if (psize)
		{
			if (dst)
			{
				pdata = new BYTE[psize];
				memcpy(pdata, s, psize);
			}
			s += psize;
		}
		if (dst)
			*(void**) d = pdata;
		d += sizeof(void*);
	}
	else
	{
		Format* x = s_fmt;
		int mc=0;
		for (; x->type && x->type != type; x++);

		if (!x->type)
		{
			printf("BSG::Unpack: cannot find %d\n", x->type);
			return -1;
		}
		unsigned long* f = x->fields;
		const BYTE* sstart = s;
		int imcount=0;
		for (; *f; f++)
		{
			imcount++;
			int fsize = Unpack((Type) *f, s, dst ? &d:NULL);
			if (fsize <= 0)
				return fsize;
			s += fsize;
		}
	}

	if (dst)
		*dst = d;
	return (s - src);
}

/**
 * Copy a TCM data structure
 * @type: Structure type
 * @src: Source
 * @dst: (OUT) Destination
 * Returns: Packed size
 */
int
BSG::Copy(Type type, const void* src, void* dst)
{
	// XXX - this could be done more efficiently
	int size = Pack(type, src);
	if (size <= 0)
		return size;
	BYTE* buffer = new BYTE[size];
	Pack(type, src, buffer);
	Unpack(type, buffer, dst);
	return size;
}

/**
 * Free memory associated with unpacked TCM structure
 * @type: TCM structure type
 * @src: (IN) TCM structure (OUT) end of TCM structure
 * Note: Destroy should be called on all structures created with Unpack
 *       to ensure that any allocated memory is freed
 */
void
BSG::Destroy(Type type, BYTE** src)
{
	BYTE* s = *src;

	unsigned int size = (type & __FMT_MASK);
	if (type & (__FMT_CONST | __FMT_DATA))
		s += size;
	else if (type & __FMT_COUNT)
	{
		int dsize = size >> 4;
		size &= 0xf;
		s += size;
		BYTE* ptr = *(BYTE**) s;
		if (ptr)
		{
			delete [] ptr;
			*(BYTE**) s = NULL;
		}
		s += sizeof(void*);
	}
	else if (type & __FMT_SIZE)
	{
		s += size;
		BYTE* ptr = *(BYTE**) s;
		if (ptr)
		{
			delete [] ptr;
			*(BYTE**) s = NULL;
		}
		s += sizeof(void*);
	}
	else
	{
		Format* x = s_fmt;
		for (; x->type && x->type != type; x++) ;
		if (!x->type)
			return;
		unsigned long* f = x->fields;
		for (; *f; f++)
			Destroy((Type) *f, &s);
	}

	*src = s;
}

/**
 * Pack a 16-bit constant into a buffer in big-endian format
 */
void
BSG::Pack16(BYTE* dst, int val)
{
	dst[0] = (BYTE)((val >> 8) & 0xff);
	dst[1] = (BYTE)(val & 0xff);
}

/**
 * Unpack a 16-bit constant from a buffer in big-endian format
 */
int
BSG::Unpack16(const BYTE* src)
{
	return (((UINT32) src[0]) << 8 | (UINT32) src[1]);
}

/**
 * Pack a 32-bit constant into a buffer in big-endian format
 */
void
BSG::Pack32(BYTE* dst, int val)
{
	dst[0] = (BYTE)((val >> 24) & 0xff);
	dst[1] = (BYTE)((val >> 16) & 0xff);
	dst[2] = (BYTE)((val >> 8) & 0xff);
	dst[3] = (BYTE)(val & 0xff);
}

/**
 * Unpack a 32-bit constant from a buffer in big-endian format
 */
int
BSG::Unpack32(const BYTE* src)
{
	return (((UINT32) src[0]) << 24
		| ((UINT32) src[1]) << 16
		| ((UINT32) src[2]) << 8
		| (UINT32) src[3]);
}

/**
 * Pack a 64-bit constant into a buffer in big-endian format
 */
void
BSG::Pack64(BYTE* dst, TCM_UINT64 val)
{
	Pack32(dst, (int)(val >> 32));
	Pack32(dst + 4, (int)(val & 0xffffffff));
}

/**
 * Unpack a 64-bit constant from a buffer in big-endian format
 */
TCM_UINT64
BSG::Unpack64(const BYTE* src)
{
	return ((((TCM_UINT64) Unpack32(src)) << 32)
		| (TCM_UINT64) Unpack32(src + 4));
}

/**
 * Pack a 8/16/32-bit constant into a buffer in big-endian format
 * Note: This works correctly on both big and little endian architectures
 */
void
BSG::PackConst(unsigned long val, int size, BYTE* dst)
{
	switch (size)
	{
	case 4:
		Pack32(dst, val);
		break;
	case 2:
		Pack16(dst, val);
		break;
	case 1:
		dst[0] = (BYTE)(val & 0xff);
		break;
	}
}

/**
 * Unpack a 8/16/32-bit constant from a buffer in big-endian format
 * Note: This works correctly on both big and little endian architectures
 */
unsigned long
BSG::UnpackConst(const BYTE* src, int size)
{
	unsigned long val = 0;
	switch (size)
	{
	case 4:
		val = (unsigned long) Unpack32(src);
		break;
	case 2:
		val = (unsigned long) Unpack16(src);
		break;
	case 1:
		val = (unsigned long) src[0];
		break;
	}		
	return val;
}

/**
 * Pack a 32-bit constant into a buffer in little-endian format
 */
void
BSG::Pack32R(BYTE* dst, int val)
{
	dst[0] = (BYTE)(val & 0xff);
	dst[1] = (BYTE)((val >> 8) & 0xff);
	dst[2] = (BYTE)((val >> 16) & 0xff);
	dst[3] = (BYTE)((val >> 24) & 0xff);
}

/**
 * Unpack a 32-bit constant from a buffer in little-endian format
 */
int
BSG::Unpack32R(const BYTE* src)
{
	return ((UINT32) src[0]
		| ((UINT32) src[1]) << 8
		| ((UINT32) src[2]) << 16
		| ((UINT32) src[3]) << 24);
}
