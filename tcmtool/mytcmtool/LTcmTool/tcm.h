/*
 * File Name :  tcm.h
 * Copyright (c) 2006 : lenovo
 * Author :  zhangyz@litsoft.com.cn         
 * Date : 2006/11/09 									
 * Description :                        
 * Version : 0.1       		              
 * Function List : 
 */

#ifndef __TCM_H__
#define __TCM_H__

#include "stdafx.h"

// Pack structures for BSG (MSVC note: Use -O1. With -O2, VC will pad!)
#ifdef _MSC_VER
#pragma pack(push, 1)
#define PACKED
#else /* _MSC_VER */
#define PACKED __attribute__ ((packed))
#endif /* _MSC_VER */


// Base types
typedef unsigned char BYTE;
typedef unsigned char TCM_BYTE;
typedef unsigned char TCM_BOOL;
typedef unsigned short TCM_UINT16;
typedef unsigned int TCM_UINT32;

#ifdef WIN32

typedef unsigned __int64 TCM_UINT64;

#else

typedef unsigned long long TCM_UINT64;

#endif

// TCM 1.1 types
typedef TCM_BYTE TCM_AUTH_DATA_USAGE;
typedef TCM_BYTE TCM_PAYLOAD_TYPE;
typedef TCM_UINT16 TCM_PROTOCOL_ID;
typedef TCM_UINT16 TCM_STARTUP_TYPE;
typedef TCM_UINT16 TCM_ENC_SCHEME;
typedef TCM_UINT16 TCM_SIG_SCHEME;
typedef TCM_UINT16 TCM_MIGRATE_SCHEME;
typedef TCM_UINT16 TCM_PHYSICAL_PRESENCE;
typedef TCM_UINT16 TCM_ENTITY_TYPE;
typedef TCM_UINT16 TCM_KEY_USAGE;
typedef TCM_UINT32 TCM_COMMAND_CODE;
typedef TCM_UINT32 TCM_CAPABILITY_AREA;
typedef TCM_UINT32 TCM_KEY_FLAGS;
typedef TCM_UINT32 TCM_ALGORITHM_ID;
typedef TCM_UINT32 TCM_AUTHHANDLE;
typedef TCM_UINT32 TCM_DIRINDEX;
typedef TCM_UINT32 TCM_KEYHANDLE;
typedef TCM_UINT32 TCM_PCRINDEX;
typedef TCM_UINT32 TCM_RESULT;
typedef TCM_UINT32 TCM_HANDLE;
typedef TCM_UINT32 TCM_SEQ;
typedef TCM_UINT16 TCM_TAG;
#define TCM_DIGEST_SIZE 32
typedef TCM_BYTE TCM_AUTHDATA[TCM_DIGEST_SIZE];
typedef TCM_AUTHDATA TCM_SECRET;
typedef TCM_AUTHDATA TCM_ENCAUTH;


typedef TCM_BYTE TCM_DELEGATE_LABEL;
typedef TCM_BYTE TCM_LOCALITY_MODIFIER;
typedef TCM_BYTE TCM_FAMILY_LABEL;
typedef TCM_BYTE TCM_LOCALITY_SELECTION;
typedef TCM_UINT16 TCM_EK_TYPE;
typedef TCM_UINT16 TCM_STRUCTURE_TAG;
typedef TCM_UINT16 TCM_PLATFORM_SPECIFIC;
typedef TCM_UINT32 TCM_MODIFIER_INDICATOR;
typedef TCM_UINT32 TCM_ACTUAL_COUNT;
typedef TCM_UINT32 TCM_TRANSPORT_ATTRIBUTES;
typedef TCM_UINT32 TCM_RESOURCE_TYPE;
typedef TCM_UINT32 TCM_KEY_CONTROL;
typedef TCM_UINT32 TCM_NV_INDEX;
typedef TCM_UINT32 TCM_FAMILY_ID;
typedef TCM_UINT32 TCM_FAMILY_VERIFICATION;
typedef TCM_UINT32 TCM_STARTUP_EFFECTS;
typedef TCM_UINT32 TCM_SYM_MODE;
typedef TCM_UINT32 TCM_FAMILY_FLAGS;
typedef TCM_UINT32 TCM_DELEGATE_INDEX;
typedef TCM_UINT32 TCM_CMK_RESTRICT_DELEGATE;
typedef TCM_UINT32 TCM_COUNT_ID;
typedef TCM_UINT32 TCM_REDIT_COMMAND;
typedef TCM_UINT32 TCM_TRANSHANDLE;
typedef TCM_UINT32 TCM_FAMILY_OPERATION;
typedef TCM_UINT32 TCM_GPIO_ATTRIBUTES;
typedef TCM_UINT32 TCM_GPIO_BUS;

// TCM_BOOL values
#define TCM_TRUE 0x01
#define TCM_FALSE 0x00

// TCM_KEY_USAGE values

#define TCM_KEY_SIGNING 0x0010
#define TCM_KEY_STORAGE 0x0011
#define TCM_KEY_IDENTITY 0x0012
#define TCM_KEY_AUTHCHANGE 0X0013
#define TCM_KEY_BIND 0x0014
#define TCM_KEY_LEGACY 0x0015
#define TCM_KEY_MIGRATE 0x0016


//TCM_KEY_USAGE values 国标相对于TCG新添加的定义
#define TCM_ECCKEY_SIGNING	0x0010
#define TCM_ECCKEY_STORAGE	0x0011
#define TCM_ECCKEY_IDENTITY	0x0012
#define TCM_ECCKEY_AUTHCHANGE	0X0013
#define TCM_ECCKEY_BIND		0x0014
#define TCM_ECCKEY_LEGACY	0x0015
#define TCM_ECCKEY_MIGRATE	0x0016
#define TCM_ECCKEY_PEK		0x0017
#define TCM_SMS4KEY_STORAGE	0x0018
#define TCM_SMS4KEY_BIND	0x0019
#define TCM_KEY_INVALID		0x0020


// TCM_ENC_SCHEME values
// add by Shi Ruiqiong 2006.11.20
// modified 2006.11.23
#define TCM_ES_NONE 0x0004               // 保留原来TPM的定义
#define TCM_ES_RSANONE 0x0001
#define TCM_ES_RSAESOAEP_SHA1_MGF1 0x0002 //保留原来TPM的定义,但原值为0x0003
#define TCM_ES_RSAESPKCSv15 0x0003   //保留原来TPM的定义,但原值为0x0002
#define TCM_ES_ECCNONE 0x0004
#define TCM_ES_ECCIES_MGF1 0x0005
#define TCM_ES_ECCIES 0x0006
#define TCM_ES_SMS4_ECB 0x0007
#define TCM_ES_SMS4_CBC 0x0008
#define TCM_ES_SMS4_OBF 0x0009

#define TCM_ES_SYM_CNT 0x0004
#define TCM_ES_SYM_OFB 0x0005

#define	TCM_ES_ECC_GB  0x0006


// TCM_SIG_SCHEME values

#define TCM_SS_NONE 0x0001//TCM_SS_NONE 0x0001 // 保留原来TPM的定义
//#define TCM_SS_RSANONE 0x0004 
//#define TCM_SS_RSASSAPKCS1v15_SHA1 0x0004
//#define TCM_SS_RSASSAPKCS1v15_DER 0x0003
//#define TCM_SS_RSASSAPKCS1v15_INFO 0x0004
#define TCM_SS_ECCSIGN_SCH 0x0005
//#define TCM_SS_ECCDSA_DER 0x0006
//#define TCM_SS_ECCDSA_INFO 0x0007
//#define TCM_SS_ECCNONE 0x0008

// end of add by Shi Ruiqiong 2006.11.20


// TCM_AUTH_DATA_USAGE values
#define TCM_AUTH_NEVER 0x00 
#define TCM_AUTH_ALWAYS 0x01
#define TCM_AUTH_PRIV_USE_ONLY 0x03

// TCM_KEY_FLAGS values
#define TCM_REDIRECTION 0x00000001
#define TCM_MIGRATABLE 0x00000002
#define TCM_VOLATILEKEY 0x00000004
#define TCM_PCRIGNOREDONREAD 0x00000008
#define TCM_MIGRATEAUTHORITY 0x00000010

// TCM_PAYLOAD_TYPE values
#define TCM_PT_ASYM 0x01
#define TCM_PT_BIND 0x02
#define TCM_PT_MIGRATE 0x03
#define TCM_PT_MAINT 0x04
#define TCM_PT_SEAL 0x05
#define TCM_PT_MIGRATE_RESTRICTED 0x06
#define TCM_PT_SYM 0x00
#define	TCM_PT_ASYM_MIGRATE      0x09
#define	TCM_PT_SYM_MIGRATE      0x08

// TCM_ENTITY_TYPE values
#define TCM_ET_KEYHANDLE 0x0001
#define TCM_ET_OWNER 0x0002
#define TCM_ET_DATA 0x0003
//#define TCM_ET_SRK 0x0004
#define TCM_ET_SMK 0x0004
#define TCM_ET_KEY 0x0005
#define TCM_ET_REVOKE 0x0006
#define TCM_ET_DEL_BLOB 0x0007

#define TCM_ET_DEL_OWNER_BLOB 0x0007 //add by zhanghuajun 2006/11/22
#define TCM_ET_DEL_KEY_BLOB 0x0009 //add by zhanghuajun 2006/11/22

#define TCM_ET_DEL_ROW 0x0008
#define TCM_ET_DEL_KEY 0x0009
#define TCM_ET_COUNTER 0x000a
#define TCM_ET_NV 0x000b
#define TCM_ET_RESERVED_HANDLE 0x0040

//国标增加
#define TCM_ET_KEYSMS4 0x0011
#define TCM_NONE	   0x0012 

// TCM_STARTUP_TYPE values
#define TCM_ST_CLEAR 0x0001
#define TCM_ST_STATE 0x0002
#define TCM_ST_DEACTIVATED 0x0003

// TCM_PROTOCOL_ID values
#define TCM_PID_OIAP 0x0001
#define TCM_PID_OSAP 0x0002
#define TCM_PID_ADIP 0x0003
#define TCM_PID_ADCP 0X0004
#define TCM_PID_OWNER 0X0005
#define TCM_PID_DSAP 0X0006
#define TCM_PID_TRANSPORT 0X0007
#define TCM_PID_AP 0x0008

// TCM_ALGORITHM_ID values
#define TCM_ALG_RSA 0x00000001
#define TCM_ALG_FIRST TCM_ALG_RSA
#define TCM_ALG_DES 0x00000002
#define TCM_ALG_3DES 0X00000003
#define TCM_ALG_SHA 0x00000004
#define TCM_ALG_HMAC2 0x00000005
#define TCM_ALG_AES128 0x00000006
#define TCM_ALG_KDF 0x00000007     //TCM_ALG_KDF
#define TCM_ALG_AES192 0x00000008
#define TCM_ALG_AES256 0x00000009
#define TCM_ALG_XOR 0x0000000A
#define TCM_ALG_ECC		0x0000000B
#define TCM_ALG_SMS4	0x0000000C
#define TCM_ALG_SCH		0x0000000D
#define TCM_ALG_HMAC	0x0000000E 
#define TCM_ALG_LAST TCM_ALG_HMAC2
#define TCM_ALG_MGF1 0x0000000F

// TCM_PHYSICAL_PRESENCE values
#define TCM_PHYSICAL_PRESENCE_LIFETIME_LOCK 0x0080
#define TCM_PHYSICAL_PRESENCE_HW_ENABLE 0x0040
#define TCM_PHYSICAL_PRESENCE_CMD_ENABLE 0x0020
#define TCM_PHYSICAL_PRESENCE_NOTPRESENT 0x0010
#define TCM_PHYSICAL_PRESENCE_PRESENT 0x0008
#define TCM_PHYSICAL_PRESENCE_LOCK 0x0004
#define TCM_PHYSICAL_PRESENCE_HW_DISABLE 0x0200
#define TCM_PHYSICAL_PRESENCE_CMD_DISABLE 0x0100
#define TCM_PHYSICAL_PRESENCE_FAIL 0x1111


// TCM_MIGRATE_SCHEME values
#define TCM_MS_MIGRATE 0x0001
#define TCM_MS_REWRAP 0x0002
#define TCM_MS_MAINT 0x0003
#define TCM_MS_RESTRICT_MIGRATE 0x0004
#define TCM_MS_RESTRICT_APPROVE_DOUBLE 0x0005

// TCM_PCR_EVENT values
#define TCM_EV_CODE_CERT 0
#define TCM_EV_CODE_NOCERT 1
#define TCM_EV_XML_CONFIG 2
#define TCM_EV_NO_ACTION 3
#define TCM_EV_SEPARATOR 4
#define TCM_EV_ACTION 5
#define TCM_EV_PLATFORM_SPECIFIC 6

// TCM_CAP_FLAG values
#define TCM_CAP_FLAG_PERMANENT 0x00000108
#define TCM_CAP_FLAG_VOLATILE 0x00000109

// TCM_CAPABILITY_AREA values
#define TCM_CAP_ORD 0x00000001
#define TCM_CAP_ALG 0x00000002
#define TCM_CAP_PID 0x00000003
#define TCM_CAP_FLAG 0x00000004
#define TCM_CAP_PROPERTY 0x00000005
#define TCM_CAP_VERSION 0x00000006
#define TCM_CAP_KEY_HANDLE 0x00000007
#define TCM_CAP_CHECK_LOADED 0x00000008


#define TCM_CAP_BIT_OWNER 0x00000009
#define TCM_CAP_BIT_LOCAL 0x0000000a
#define TCM_CAP_DELEGATIONS 0x0000000b
#define TCM_CAP_KEY_STATUS 0x0000000c
#define TCM_CAP_NV_LIST 0x0000000d
#define TCM_CAP_TABLE_ADMIN 0x0000000e
#define TCM_CAP_TABLE_ENABLE 0x0000000f
#define TCM_CAP_MFR 0x00000010
#define TCM_CAP_NV_INDEX 0x00000011
#define TCM_CAP_TRANS_ALG 0x00000012
#define TCM_CAP_GPIO_CHANNEL 0x00000013
#define TCM_CAP_HANDLE 0x00000014
#define TCM_CAP_TRANS_ES 0x00000015



#define TCM_CAP_SYM_MODE 0x00000009
#define TCM_CAP_KEY_STATUS 0x0000000c
#define TCM_CAP_NV_LIST 0x0000000d
#define TCM_CAP_TABLE_ADMIN 0x0000000e
#define TCM_CAP_TABLE_ENABLE 0x0000000f
#define TCM_CAP_MFR 0x00000010
#define TCM_CAP_NV_INDEX 0x00000011
#define TCM_CAP_TRANS_ALG 0x00000012
#define TCM_CAP_GPIO_CHANNEL 0x00000013
#define TCM_CAP_HANDLE 0x00000014
#define TCM_CAP_TRANS_ES 0x00000015
#define TCM_CAP_AUTH_ENCRYPT 0x00000017
#define TCM_CAP_SELECT_SIZE 0x00000018
#define TCM_CAP_VERSION_VAL 0x0000001a



#define TCM_SF_DEACTIVATED			0x00000001
#define TCM_SF_DISABLEFORCECLEAR	0x00000002
#define TCM_SF_PHYSICALPRESENCE		0x00000003
#define TCM_SF_PHYSICALPRESENCELOCK 0x00000004
#define TCM_SF_BGLOBALLOCK			0x00000005

// TCM sub capabilities
#define TCM_CAP_PROP_PCR 0x00000101
#define TCM_CAP_PROP_DIR 0x00000102
#define TCM_CAP_PROP_MANUFACTURER 0x00000103
#define TCM_CAP_PROP_KEYS 0x00000104
#define TCM_CAP_PROP_MIN_COUNTER 0x00000107
#define TCM_CAP_PROP_AUTHSESS 0x0000010a
#define TCM_CAP_PROP_TRANSESS 0x0000010b
#define TCM_CAP_PROP_COUNTERS 0x0000010c
#define TCM_CAP_PROP_MAX_AUTHSESS 0x0000010d
#define TCM_CAP_PROP_MAX_TRANSESS 0x0000010e
#define TCM_CAP_PROP_MAX_COUNTERS 0x0000010f
#define TCM_CAP_PROP_MAX_KEYS 0x00000110
#define TCM_CAP_PROP_OWNER 0x00000111
#define TCM_CAP_PROP_CONTEXT 0x00000112
#define TCM_CAP_PROP_MAX_CONTEXT 0x00000113
#define TCM_CAP_PROP_FAMILYROWS 0x00000114
#define TCM_CAP_PROP_TIS_TIMEOUT 0x00000115
#define TCM_CAP_PROP_STARTUP_EFFECT 0x00000116
#define TCM_CAP_PROP_DELEGATE_ROW 0x00000117
//#define TCM_CAP_PROP_NV_MAXBUF 0x00000118 open
#define TCM_CAP_PROP_DAA_MAX 0x00000119
#define TCM_CAP_PROP_SESSION_DAA 0x0000011a 
#define TCM_CAP_PROP_CONTEXT_DIST 0x0000011b
#define TCM_CAP_PROP_DAA_INTERRUPT 0x0000011c
#define TCM_CAP_PROP_SESSIONS 0x0000011d
#define TCM_CAP_PROP_MAX_SESSIONS 0x0000011e
#define TCM_CAP_PROP_CMK_RESTRICTION 0x0000011f
#define TCM_CAP_PROP_DURATION	0x00000120 
//#define TCM_CAP_PROP_NV_AVAILABLE 0x00000121 open
#define TCM_CAP_PROP_ACTIVE_COUNTER 0x00000122
#define TCM_CAP_PROP_MAX_NV_AVAILABLE 0x00000123
#define TCM_CAP_PROP_INPUT_BUFFER 0x00000124
#define TCM_SET_PERM_FLAGS 0x00000001
#define TCM_SET_PERM_DATA 0x00000002
#define TCM_SET_STCLEAR_FLAGS 0x00000003
#define TCM_SET_STCLEAR_DATA 0x00000004
#define TCM_SET_STANY_FLAGS 0x00000005
#define TCM_SET_STANY_DATA 0x00000006
#define TCM_SET_VENDOR 0x00000007 
#define TCM_EK_TYPE_ACTIVATE 0x0001
#define TCM_EK_TYPE_AUTH 0x0002

#define TCM_RT_KEY 0x00000001
#define TCM_RT_AUTH 0x00000002
#define TCM_RT_HASH 0x00000003
#define TCM_RT_TRANS 0x00000004
#define TCM_RT_CONTEXT 0x00000005
#define TCM_RT_COUNTER 0x00000006
#define TCM_RT_DELEGATE 0x00000007
#define TCM_RT_DAA_TCM 0x00000008
#define TCM_RT_DAA_V0 0x00000009
#define TCM_RT_DAA_V1 0x0000000a

// TCM_STRUCTURE_TAG values 
#define TCM_TAG_CONTEXTBLOB 0x0001
#define TCM_TAG_CONTEXT_SENSITIVE 0x0002
#define TCM_TAG_CONTEXTPOINTER 0x0003
#define TCM_TAG_CONTEXTLIST 0x0004
#define TCM_TAG_SIGN_INFO 0x0005
//#define TCM_TAG_PCR_INFO_LONG 0x0006
#define TCM_TAG_PCR_INFO_FINAL 0x0006
#define TCM_TAG_PERSISTENT_FLAGS 0x0007
#define TCM_TAG_VOLATILE_FLAGS 0x0008
#define TCM_TAG_PERSISTENT_DATA 0x0009
#define TCM_TAG_VOLATILE_DATA 0x000a
#define TCM_TAG_SV_DATA 0x000b
#define TCM_TAG_EK_BLOB 0x000c
#define TCM_TAG_EK_BLOB_AUTH 0x000d
#define TCM_TAG_COUNTER_VALUE 0x000e
#define TCM_TAG_TRANSPORT_INTERNAL 0x000f
#define TCM_TAG_TRANSPORT_LOG_IN 0x0010
#define TCM_TAG_TRANSPORT_LOG_OUT 0x0011
#define TCM_TAG_AUDIT_EVENT_IN 0x0012
#define TCM_TAG_AUDIT_EVENT_OUT 0x0013
#define TCM_TAG_CURRENT_TICKS 0x0014
#define TCM_TAG_KEY 0x0015
#define TCM_TAG_STORED_DATA12 0x0016
#define TCM_TAG_STORED_DATA 0x0016
#define TCM_TAG_NV_ATTRIBUTES 0x0017
#define TCM_TAG_NV_DATA_PUBLIC 0x0018
#define TCM_TAG_NV_DATA_SENSITIVE 0x0019
#define TCM_TAG_DELEGATIONS 0x001a
#define TCM_TAG_DELEGATE_PUBLIC 0x001b
#define TCM_TAG_DELEGATE_TABLE_ROW 0x001c
#define TCM_TAG_TRANSPORT_AUTH 0x001d
#define TCM_TAG_TRANSPORT_PUBLIC 0x001e
#define TCM_TAG_PERMANENT_FLAGS 0x001f
#define TCM_TAG_STCLEAR_FLAGS 0x0020
#define TCM_TAG_STANY_FLAGS 0x0021
#define TCM_TAG_PERMANENT_DATA 0x0022
#define TCM_TAG_STCLEAR_DATA 0x0023
#define TCM_TAG_STANY_DATA 0x0024
#define TCM_TAG_FAMILY_TABLE_ENTRY 0x0025
#define TCM_TAG_DELEGATE_SENSITIVE 0x0026
#define TCM_TAG_DELEGATE_KEY_BLOB 0x0027
#define TCM_TAG_KEY12 0x0028
#define TCM_TAG_CERTIFY_INFO 0x0029
#define TCM_TAG_CAP_VERSION_INFO  0x0030
#define TCM_TAG_DELEGATE_OWNER_BLOB 0x002a
#define TCM_TAG_EK_BLOB_ACTIVATE 0x002b
#define TCM_TAG_DAA_BLOB 0x002c
#define TCM_TAG_DAA_CONTEXT 0x002d
#define TCM_TAG_DAA_ENFORCE 0x002e
#define TCM_TAG_DAA_ISSUER 0x002f
#define TCM_TAG_DAA_ROOTINFO 0x0030
#define TCM_TAG_DAA_SENSITIVE 0x0031
#define TCM_TAG_DAA_TCM 0x0032
#define TCM_TAG_GPIO_AUTHORIZE 0x0033
#define TCM_TAG_GPIO_SENSITIVE 0x0034
#define TCM_TAG_GPIO_CHANNEL 0x0035
#define TCM_TAG_QUOTE_INFO  0x0036

// TCM_PERMANENT_FLAGS values 
#define TCM_FLAGS_PERM_DISABLE 0x0001
#define TCM_FLAGS_PERM_OWNERSHIP 0x0002
#define TCM_FLAGS_PERM_DEACTIVATED 0x0004
#define TCM_FLAGS_PERM_READPUBEK 0x0008
#define TCM_FLAGS_PERM_DISABLEOWNERCLEAR 0x0010
//#define TCM_FLAGS_PERM_ALLOWMAINTENANCE 0x0020
#define TCM_FLAGS_PERM_PHYSPRESLIFETIMELOCK 0x0020
#define TCM_FLAGS_PERM_PHYSPRESHWENABLE 0x0040
#define TCM_FLAGS_PERM_PHYSPRESCMDENABLE 0x0080
#define TCM_FLAGS_PERM_CEKPUSED 0x0100
#define TCM_FLAGS_PERM_TCMPOST 0x0200
#define TCM_FLAGS_PERM_TCMPOSTLOCK 0x0400
//#define TCM_FLAGS_PERM_FIPS  0x1000
#define TCM_FLAGS_PERM_OPERATOR 0x0800
#define TCM_FLAGS_PERM_ENABLE_REVOKEEK 0x1000
#define TCM_FLAGS_PERM_NVLOCKED 0x2000
#define TCM_FLAGS_PERM_TCMESTABLISHED 0x4000

// TCM_VOLATILE_FLAGS values 
#define TCM_FLAGS_VOLATILE_DEACTIVATED 0x00000001
#define TCM_FLAGS_VOLATILE_DISABLEFORCECLEAR 0x00000002
#define TCM_FLAGS_VOLATILE_PHYSPRES 0x00000004
#define TCM_FLAGS_VOLATILE_PHYSPRESLOCK 0x00000008
#define TCM_FLAGS_VOLATILE_POSTINIT 0x00000010

// TCM_STCLEAR_FLAGS 
#define TCM_FLAGS_STCLEAR_DEACTIVATED 0x0001
#define TCM_FLAGS_STCLEAR_DISABLEFORCECLEAR 0x0002
#define TCM_FLAGS_STCLEAR_PHYSPRES 0x0004
#define TCM_FLAGS_STCLEAR_PHYSPRESLOCK 0x0008
#define TCM_FLAGS_STCLEAR_TABLEADMIN_MASK 0x0ff0
#define TCM_FLAGS_STCLEAR_GLOBALLOCK 0x0010

//6.1	TCM_PERMANENT_FLAGS Flag Name ZHANGYZ
#define TCM_PF_BASE 0x00000000 
#define TCM_PF_DISABLE		TCM_PF_BASE + 1
#define TCM_PF_OWNERSHIP	TCM_PF_BASE + 2
#define TCM_PF_DEACTIVATED	TCM_PF_BASE + 3
#define TCM_PF_READPUBEK	TCM_PF_BASE + 4
#define TCM_PF_DISABLEOWNERCLEAR	TCM_PF_BASE + 5
#define TCM_PF_ALLOWMAINTENANCE		TCM_PF_BASE + 6
#define TCM_PF_PHYSICALPRESENCELIFETIMELOCK	TCM_PF_BASE + 7
#define TCM_PF_PHYSICALPRESENCEHWENABLE		TCM_PF_BASE + 8
#define TCM_PF_PHYSICALPRESENCECMDENABLE	TCM_PF_BASE + 9
#define TCM_PF_CEKPUSED				TCM_PF_BASE + 10
#define TCM_PF_TCMPOST				TCM_PF_BASE + 11
#define TCM_PF_TCMPOSTLOCK			TCM_PF_BASE + 12
#define TCM_PF_FIPS					TCM_PF_BASE + 13
#define TCM_PF_OPERATOR				TCM_PF_BASE + 14
#define TCM_PF_ENABLEREVOKEEK		TCM_PF_BASE + 15
#define TCM_PF_NV_LOCKED			TCM_PF_BASE + 16
#define TCM_PF_READSRKPUB			TCM_PF_BASE + 17
#define TCM_PF_RESETESTABLISHMENTBIT TCM_PF_BASE + 18
#define TCM_PF_MAINTENANCEDONE		TCM_PF_BASE + 19

// TCM_KH values 
#define TCM_KH_SRK 0x40000000
#define TCM_KH_SMK 0x40000000
#define TCM_KH_OWNER 0x40000001
#define TCM_KH_REVOKE 0x40000002
#define TCM_KH_TRANSPORT 0x40000003
#define TCM_KH_OPERATOR 0x40000004
#define TCM_KH_ADMIN 0x40000005
#define TCM_KH_EK 0x40000006

// TCM_PLATFORM_SPECIFIC values 
#define TCM_PS_PC_11 0x0001
#define TCM_PS_PC_12 0x0002
#define TCM_PS_PDA_12 0x0003
#define TCM_PS_SERVER_12 0x0004
#define TCM_PS_MOBILE_12 0x0005

// TCM_TRANSPORT_ATTRIBUTES 
#define TCM_TRANSPORT_ENCRYPT 0x00000001
#define TCM_TRANSPORT_LOG 0x00000002
#define TCM_TRANSPORT_EXCLUSIVE 0x00000004

// TCM_NV_INDEX values 
#define TCM_NV_INDEX_LOCK_HIGH 0xffffffff
#define TCM_NV_INDEX_LOCK_LOW 0xffffffff
#define TCM_NV_INDEX0 0x00000000
#define TCM_NV_INDEX_DIR 0x00000001
#define TCM_NV_INDEX_EKCERT 0x0000f000
#define TCM_NV_INDEX_TCM_CC 0x0000f001
#define TCM_NV_INDEX_PLATFORMCERT 0x0000f002
#define TCM_NV_INDEX_PLATFORM_CC 0x0000f003

// TCM_NV_ATTRIBUTES values 
#define TCM_NV_PER_READ_STCLEAR  (1UL << 31)
#define TCM_NV_PER_AUTH_READ	 (1UL << 18)
#define TCM_NV_PER_OWNER_READ    (1UL << 17)
#define TCM_NV_PER_PPREAD		 (1UL << 16)
#define TCM_NV_PER_GLOBALLOCK    (1UL << 15)
#define TCM_NV_PER_WRITE_STCLEAR (1UL << 14)
#define TCM_NV_PER_WRITEDEFINE   (1UL << 13)
#define TCM_NV_PER_WRITEALL      (1UL << 12)
#define TCM_NV_PER_AUTHWRITE     (1UL << 2) 
#define TCM_NV_PER_OWNERWRITE    (1UL << 1) 
#define TCM_NV_PER_PPWRITE       (1UL << 0) 
#define TCM_NV_INDEX_LOCK		 0xFFFFFFFF 

// TCM_CMK_DELEGATE values 	    
#define TCM_CMK_DELEGATE_SIGNING	(1UL << 31) 
#define TCM_CMK_DELEGATE_STORAGE	(1UL << 30) 
#define TCM_CMK_DELEGATE_BIND		(1UL << 29) 
#define TCM_CMK_DELEGATE_LEGACY		(1UL << 28) 

// TCM_FAMILY_OPERATION values 
#define TCM_FAMILY_CREATE 0x00000001
#define TCM_FAMILY_ENABLE 0x00000002
#define TCM_FAMILY_ADMIN 0x00000003
#define TCM_FAMILY_INVALIDATE 0x00000004

// TCM_GPIO_BUS values 
#define TCM_GPIO_SINGLE 0x00000001
#define TCM_GPIO_SMBUS 0x00000002
#define TCM_GPIO_SMBUS_ARP 0x00000003

// TCM_REDIR_COMMAND 
#define TCM_REDIR_GPIO 0x00000001

/*
// TCM_LOCALITY_SELECTION 
#define TCM_LOC_ZERO 0x00000001
#define TCM_LOC_ONE 0x00000002
#define TCM_LOC_TWO 0x00000004
#define TCM_LOC_THREE 0x00000008
#define TCM_LOC_FOUR 0x00000010
*/

// TCM_LOCALITY_SELECTION 
#define TCM_LOC_ZERO 0x01    //TCM_LOC_ZERO
#define TCM_LOC_ONE 0x02     //TCM_LOC_ONE
#define TCM_LOC_TWO 0x04     //TCM_LOC_TWO
#define TCM_LOC_THREE 0x08   //TCM_LOC_THREE
#define TCM_LOC_FOUR 0x10    //TCM_LOC_FOUR

// TCM_SYM_MODE 
#define TCM_SYM_MODE_ECB 0x00000001
#define TCM_SYM_MODE_CBC 0x00000002
#define TCM_SYM_MODE_CFB 0x00000003

// TCM_DELEGATE_TYPE values 
#define TCM_DEL_OWNER_BITS 0x00000001
#define TCM_DEL_KEY_BITS 0x00000002

// TCM_TRANSPORT_ATTRIBUTES
#define TCM_TRANSPORT_ENCRYPT 0x00000001
#define TCM_TRANSPORT_LOG 0x00000002
#define TCM_TRANSPORT_EXCLUSIVE 0x00000004

// Owner key permission bits 
#define TCM_OWNER_DELEGATE_CMD_CreateTicket (1UL << 26)
#define TCM_OWNER_DELEGATE_CMK_CreateKey (1UL << 25)
#define TCM_OWNER_DELEGATE_Delegate_LoadOwnerDelegation (1UL << 24)
#define TCM_OWNER_DELEGATE_DAA_Join (1UL << 23)
#define TCM_OWNER_DELEGATE_AuthorizeMigrationKey (1UL << 22)
#define TCM_OWNER_DELEGATE_CreateMaintenanceArchive (1UL << 21)
#define TCM_OWNER_DELEGATE_LoadMaintenanceArchive (1UL << 20)
#define TCM_OWNER_DELEGATE_KillMaintenanceFeature (1UL << 19)
#define TCM_OWNER_DELEGATE_Delegate_CreateKeyDelegation (1UL << 18)
#define TCM_OWNER_DELEGATE_LoadBlobOwner (1UL << 17)
#define TCM_OWNER_DELEGATE_OwnerClear (1UL << 16)
#define TCM_OWNER_DELEGATE_DisableOwnerClear (1UL << 15)
#define TCM_OWNER_DELEGATE_DisableForceClear (1UL << 14)
#define TCM_OWNER_DELEGATE_OwnerSetDisable (1UL << 13)
#define TCM_OWNER_DELEGATE_SetOwnerInstall (1UL << 12)
#define TCM_OWNER_DELEGATE_MakeIdentity (1UL << 11)
#define TCM_OWNER_DELEGATE_ActivateIdentity (1UL << 10)
#define TCM_OWNER_DELEGATE_OwnerReadPubek (1UL << 9)
#define TCM_OWNER_DELEGATE_DisablePubekRead (1UL << 8)
#define TCM_OWNER_DELEGATE_SetRedirection (1UL << 7)
#define TCM_OWNER_DELEGATE_FieldUpgrade (1UL << 6)
#define TCM_OWNER_DELEGATE_Delegation_UpdateVerification (1UL << 5)
#define TCM_OWNER_DELEGATE_CreateCounter (1UL << 4)
#define TCM_OWNER_DELEGATE_ReleaseCounterOwner (1UL << 3)
#define TCM_OWNER_DELEGATE_Delegate_Manage (1UL << 2)
#define TCM_OWNER_DELEGATE_Delegate_CreateOwnerDelegation (1UL << 1)
#define TCM_OWNER_DELEGATE_DAA_Sign (1UL << 0)

// Key permission bits 
#define TCM_DELEGATE_GetAuditDigestSigned (1UL << 15)
#define TCM_DELEGATE_Sign (1UL << 14)
#define TCM_DELEGATE_CertifyKey2 (1UL << 13)
#define TCM_DELEGATE_CertifyKey (1UL << 12)
#define TCM_DELEGATE_CreateWrapKey (1UL << 11)
#define TCM_DELEGATE_CMK_CreateBlob (1UL << 10)
#define TCM_DELEGATE_CreateMigrationBlob (1UL << 9)
#define TCM_DELEGATE_ConvertMigrationBlob (1UL << 8)
#define TCM_DELEGATE_Delegate_CreateKeyDelegation (1UL << 7)
#define TCM_DELEGATE_ChangeAuth (1UL << 6)
#define TCM_DELEGATE_GetPubKey (1UL << 5)
#define TCM_DELEGATE_Unbind (1UL << 4)
#define TCM_DELEGATE_Quote (1UL << 3)
#define TCM_DELEGATE_Unseal (1UL << 2)
#define TCM_DELEGATE_Seal (1UL << 1)
#define TCM_DELEGATE_LoadKey (1UL << 0)

#define TCM_DELEGATE_EstablishTransport (1UL << 21)//add by zhanghuajun 2006/11/20

// TCM_TAG values
#define TCM_TAG_RQU_COMMAND 0x00c1
#define TCM_TAG_RQU_AUTH1_COMMAND 0x00c2
#define TCM_TAG_RQU_AUTH2_COMMAND 0x00c3
#define TCM_TAG_RSP_COMMAND 0x00c4
#define TCM_TAG_RSP_AUTH1_COMMAND 0x00c5
#define TCM_TAG_RSP_AUTH2_COMMAND 0x00c6

// TCM ordinal codes
#define TCM_PROTECTED_COMMAND 0x00000000UL
#define TCM_UNPROTECTED_COMMAND 0x80000000UL
#define TCM_CONNECTION_COMMAND 0x40000000UL
#define TCM_VENDOR_COMMAND 0x20000000UL

// TCM_KEY_CONTROL values
#define TCM_KEY_CONTROL_OWNER_EVICT 0x00000001

// TCM vendor specific codes
#define TCM_Vendor_Specific32 0x00000400
#define TCM_Vendor_Specific8 0x80

typedef struct TCM_VERSION
{
	TCM_BYTE major;
	TCM_BYTE minor;
	TCM_BYTE revMajor;
	TCM_BYTE revMinor;
} PACKED TCM_VERSION;

typedef struct TCM_DIGEST
{
	TCM_BYTE digest[TCM_DIGEST_SIZE];
} PACKED TCM_DIGEST;

typedef TCM_DIGEST TCM_CHOSENID_HASH;
typedef TCM_DIGEST TCM_COMPOSITE_HASH;
typedef TCM_DIGEST TCM_DIRVALUE;
typedef TCM_DIGEST TCM_HMAC;
typedef TCM_DIGEST TCM_PCRVALUE;
typedef TCM_DIGEST TCM_AUDITDIGEST;
typedef TCM_DIGEST TCM_DAA_TCM_SEED;
typedef TCM_DIGEST TCM_DAA_CONTEXT_SEED;

typedef struct TCM_NONCE
{
	TCM_BYTE nonce[TCM_DIGEST_SIZE];
} PACKED TCM_NONCE;

typedef struct TCM_KEY_HANDLE_LIST
{
	TCM_UINT16 loaded;
	TCM_KEYHANDLE* handle;
} PACKED TCM_KEY_HANDLE_LIST;

typedef struct TCM_KEY_PARMS
{
	TCM_ALGORITHM_ID algorithmID;
	TCM_ENC_SCHEME encScheme;
	TCM_SIG_SCHEME sigScheme;
	TCM_UINT32 parmSize;
	TCM_BYTE* parms;
} PACKED TCM_KEY_PARMS;
/*
typedef struct TCM_RSA_KEY_PARMS
{  
	TCM_UINT32 keyLength; 
	TCM_UINT32 numPrimes; 
	TCM_UINT32 exponentSize;
	TCM_BYTE* exponent;
} PACKED TCM_RSA_KEY_PARMS;
*/
typedef struct TCM_ECC_KEY_PARMS //Ecc Key
{  
	TCM_UINT32 keyLength; 
} PACKED TCM_ECC_KEY_PARMS;

typedef struct TCM_STORE_PUBKEY
{
	TCM_UINT32 keyLength;
	TCM_BYTE* key;
} PACKED TCM_STORE_PUBKEY;

typedef struct TCM_PUBKEY
{
	TCM_KEY_PARMS algorithmParms;
	TCM_STORE_PUBKEY pubKey;
} PACKED TCM_PUBKEY;
/*
typedef struct TCM_KEY
{
	TCM_VERSION ver;
	TCM_KEY_USAGE keyUsage;
	TCM_KEY_FLAGS keyFlags;
	TCM_AUTH_DATA_USAGE authDataUsage;
	TCM_KEY_PARMS algorithmParms; 
	TCM_UINT32 PCRInfoSize;
	TCM_BYTE* PCRInfo;
	TCM_STORE_PUBKEY pubKey;
	TCM_UINT32 encSize;
	TCM_BYTE* encData;
} PACKED TCM_KEY;
*/

typedef struct TCM_MIGRATIONKEYAUTH
{
	TCM_PUBKEY migrationKey;
	TCM_MIGRATE_SCHEME migrationScheme;
	TCM_DIGEST digest;
} PACKED TCM_MIGRATIONKEYAUTH;

typedef struct TCM_AUDIT_EVENT
{
	TCM_COMMAND_CODE ordinal;
	TCM_RESULT returncode;
} PACKED TCM_AUDIT_EVENT;

typedef struct TCM_EVENT_CERT
{
	TCM_DIGEST certificateHash; 
	TCM_DIGEST entityDigest;
	TCM_BOOL digestChecked;
	TCM_BOOL digestVerified;
	TCM_UINT32 issuerSize;
	TCM_BYTE* issuer;
} PACKED TCM_EVENT_CERT;

typedef struct TCM_PCR_SELECTION
{ 
	TCM_UINT16 sizeOfSelect;
	TCM_BYTE* pcrSelect;
} PACKED TCM_PCR_SELECTION;

typedef struct TCM_PCR_COMPOSITE
{ 
	TCM_PCR_SELECTION select;
	TCM_UINT32 valueSize;
	TCM_BYTE* pcrValue;
} PACKED TCM_PCR_COMPOSITE;

/*
typedef struct TCM_PCR_INFO
{
	TCM_PCR_SELECTION pcrSelection;
	TCM_COMPOSITE_HASH digestAtRelease;
	TCM_COMPOSITE_HASH digestAtCreation;
} PACKED TCM_PCR_INFO;
*/
/*
typedef struct TCM_STORED_DATA
{ 
	TCM_VERSION ver;
	TCM_UINT32 sealInfoSize;
	TCM_BYTE* sealInfo;
	TCM_UINT32 encDataSize;
	TCM_BYTE* encData;
} PACKED TCM_STORED_DATA;
*/
typedef struct TCM_SEALED_DATA
{ 
	TCM_PAYLOAD_TYPE payload;
	TCM_SECRET authData;
	TCM_NONCE tcmProof;
	TCM_DIGEST storedDigest;
	TCM_UINT32 dataSize;
	TCM_BYTE* data;
} PACKED TCM_SEALED_DATA;

typedef struct TCM_STORE_PRIVKEY
{
	TCM_UINT32 keyLength;
	TCM_BYTE* key;
} PACKED TCM_STORE_PRIVKEY;

typedef struct TCM_STORE_ASYMKEY
{
	TCM_PAYLOAD_TYPE payload;
	TCM_SECRET usageAuth;
	TCM_SECRET migrationAuth;
	TCM_DIGEST pubDataDigest;
	TCM_STORE_PRIVKEY privKey;
} PACKED TCM_STORE_ASYMKEY;

typedef struct tdTCM_STORE_SYMKEY {
	TCM_PAYLOAD_TYPE payload;		  
	TCM_SECRET usageAuth;	
	TCM_SECRET migrationAuth;	
	TCM_UINT16 size;                         
	TCM_BYTE* data;       
} TCM_STORE_SYMKEY;                  


typedef struct TCM_MIGRATE_ASYMKEY
{
	TCM_PAYLOAD_TYPE payload;
	TCM_SECRET usageAuth;
	TCM_DIGEST pubDataDigest;
	TCM_UINT32 partPrivKeyLen;
	TCM_STORE_PRIVKEY partPrivKey;
} PACKED TCM_MIGRATE_ASYMKEY;

typedef struct TCM_CERTIFY_INFO
{
//	TCM_STRUCTURE_TAG tag;
//	TCM_BYTE fill;
//	TCM_PAYLOAD_TYPE payloadType;
	TCM_VERSION ver;
	TCM_KEY_USAGE keyUsage;
	TCM_KEY_FLAGS keyFlags;
	TCM_AUTH_DATA_USAGE authDataUsage;
	TCM_KEY_PARMS algorithmParms;
	TCM_DIGEST pubkeyDigest;
	TCM_NONCE data;
	TCM_BOOL parentPCRStatus;
	TCM_UINT32 PCRInfoSize;
	TCM_BYTE* PCRInfo;
} PACKED TCM_CERTIFY_INFO;

typedef struct TCM_PCR_INFO
{
	TCM_STRUCTURE_TAG tag;
	TCM_LOCALITY_SELECTION localityAtCreation;
	TCM_LOCALITY_SELECTION localityAtRelease;
	TCM_PCR_SELECTION creationPCRSelection;
	TCM_PCR_SELECTION releasePCRSelection;
	TCM_COMPOSITE_HASH digestAtCreation;
	TCM_COMPOSITE_HASH digestAtRelease;
} PACKED TCM_PCR_INFO;

typedef struct TCM_QUOTE_INFO
{
	TCM_STRUCTURE_TAG  tag;
	TCM_BYTE fixed[4]; // "QUOT"
	TCM_NONCE externalData;
	TCM_PCR_INFO infoShort;
} PACKED TCM_QUOTE_INFO;

typedef struct TCM_BOUND_DATA
{
	TCM_VERSION ver;
	TCM_PAYLOAD_TYPE payload;
	TCM_BYTE* payloadData;
} PACKED TCM_BOUND_DATA;

typedef struct TCM_CHANGEAUTH_VALIDATE
{
	TCM_SECRET newAuthSecret;
	TCM_NONCE n1;
}TCM_CHANGEAUTH_VALIDATE;

typedef struct TCM_SYMMETRIC_KEY
{
	TCM_ALGORITHM_ID algId;
	TCM_ENC_SCHEME encScheme;
	TCM_UINT16 size;
	TCM_BYTE* data;
} PACKED TCM_SYMMETRIC_KEY;

typedef struct TCM_ASYM_CA_CONTENTS
{
	TCM_SYMMETRIC_KEY sessionKey;
	TCM_DIGEST idDigest;
} PACKED TCM_ASYM_CA_CONTENTS;


typedef struct TCM_STRUCT_VER
{
	TCM_BYTE major;
	TCM_BYTE minor;
	TCM_BYTE revMajor;
	TCM_BYTE revMinor;
} PACKED TCM_STRUCT_VER;

typedef struct TCM_PERMANENT_FLAGS
{
	TCM_STRUCTURE_TAG tag;
	TCM_UINT16 flags;
} PACKED TCM_PERMANENT_FLAGS;

typedef struct TCM_STCLEAR_FLAGS
{
	TCM_STRUCTURE_TAG tag;
	//	TCM_UINT16 flags; //deleted by zhangyz, five item add by zhangyz 06-07-07
	TCM_BOOL deactivated;
	TCM_BOOL disableForceClear;
	TCM_BOOL physicalPresence;
	TCM_BOOL physicalPresenceLock;
	TCM_BOOL bGlobalLock;

} PACKED TCM_STCLEAR_FLAGS;

typedef struct TCM_STANY_FLAGS
{
	TCM_STRUCTURE_TAG tag;
	TCM_BOOL postInitialize;
	TCM_MODIFIER_INDICATOR localityModifier;
	TCM_BOOL transportExclusive;
} PACKED TCM_STANY_FLAGS;

typedef struct TCM_COUNTER_VALUE
{
	TCM_STRUCTURE_TAG tag;
	TCM_BYTE label[4];
	TCM_ACTUAL_COUNT counter;
} PACKED TCM_COUNTER_VALUE;

typedef struct TCM_SIGN_INFO
{
	TCM_STRUCTURE_TAG tag;
	TCM_BYTE fixed[4];
	TCM_NONCE replay;
	TCM_UINT32 dataLen;
	TCM_BYTE* data;
} PACKED TCM_SIGN_INFO;

typedef struct TCM_CMK_AUTH
{
	TCM_DIGEST migrationAuthorityDigest;
	TCM_DIGEST destinationKeyDigest;
	TCM_DIGEST sourceKeyDigest;
} PACKED TCM_CMK_AUTH;

typedef struct TCM_MSA_COMPOSITE
{
	TCM_UINT32 MSAList;
	TCM_DIGEST* migAuthDigest;
} PACKED TCM_MSA_COMPOSITE;

/*
typedef struct TCM_PCR_INFO_SHORT
{
	TCM_PCR_SELECTION pcrSelection;
	TCM_LOCALITY_SELECTION localityAtRelease;
	TCM_COMPOSITE_HASH digestAtRelease;
} PACKED TCM_PCR_INFO_SHORT;
*/
typedef struct TCM_STORED_DATA
{ 
	TCM_STRUCTURE_TAG tag;
	TCM_UINT16 fill;
	TCM_UINT32 sealInfoSize;
	TCM_BYTE* sealInfo;
	TCM_UINT32 encDataSize;
	TCM_BYTE* encData;
} PACKED TCM_STORED_DATA;

typedef struct TCM_SYMMETRIC_KEY_PARMS
{
	TCM_UINT32 keyLength;
	TCM_UINT32 blockSize;
	TCM_UINT32 ivSize;
	TCM_BYTE* iv;
} PACKED TCM_SYMMETRIC_KEY_PARMS;

typedef struct TCM_KEY//由TPM_KEY12改名得到
{
	TCM_STRUCTURE_TAG tag;
	TCM_UINT16 fill;
	TCM_KEY_USAGE keyUsage;
	TCM_KEY_FLAGS keyFlags;
	TCM_AUTH_DATA_USAGE authDataUsage;
	TCM_KEY_PARMS algorithmParms; 
	TCM_UINT32 PCRInfoSize;
	TCM_BYTE* PCRInfo;
	TCM_STORE_PUBKEY pubKey;
	TCM_UINT32 encSize;
	TCM_BYTE* encData;
} PACKED TCM_KEY12;

typedef struct TCM_CERTIFY_INFO2
{
	TCM_STRUCTURE_TAG tag;
	TCM_UINT16 fill;
	TCM_KEY_USAGE keyUsage;
	TCM_KEY_FLAGS keyFlags;
	TCM_AUTH_DATA_USAGE authDataUsage;
	TCM_KEY_PARMS algorithmParms;
	TCM_DIGEST pubkeyDigest;
	TCM_NONCE data;
	TCM_BOOL parentPCRStatus;
	TCM_UINT32 migrationAuthoritySize;
	TCM_BYTE* migrationAuthority;
	TCM_UINT32 PCRInfoSize;
	TCM_BYTE* PCRInfo;
} PACKED TCM_CERTIFY_INFO2;

typedef struct TCM_EK_BLOB
{
	TCM_STRUCTURE_TAG tag;
	TCM_EK_TYPE ekType;
	TCM_UINT32 blobSize;
	TCM_BYTE* blob;
} PACKED TCM_EK_BLOB;

typedef struct TCM_EK_BLOB_ACTIVATE
{
	TCM_STRUCTURE_TAG tag;
	TCM_SYMMETRIC_KEY sessionKey;
	TCM_DIGEST idDigest;
	TCM_PCR_INFO pcrInfo;
} PACKED TCM_EK_BLOB_ACTIVATE;

typedef struct TCM_EK_BLOB_AUTH
{
	TCM_STRUCTURE_TAG tag;
	TCM_SECRET authValue;
} PACKED TCM_EK_BLOB_AUTH;

typedef struct TCM_IDENTITY_CONTENTS
{
	TCM_STRUCT_VER ver;
	TCM_UINT32 ordinal;
	TCM_CHOSENID_HASH labelPrivCADigest;
	TCM_PUBKEY identityPubKey;
} PACKED TCM_IDENTITY_CONTENTS;

typedef struct TCM_CURRENT_TICKS
{
	TCM_STRUCTURE_TAG tag;
	TCM_UINT64 currentTicks;
	TCM_UINT16 tickRate;
	TCM_NONCE tickNonce;
} PACKED TCM_CURRENT_TICKS;

typedef struct TCM_TRANSPORT_PUBLIC
{
	TCM_STRUCTURE_TAG tag;
	TCM_TRANSPORT_ATTRIBUTES transAttributes;
	TCM_ALGORITHM_ID algID;
	TCM_ENC_SCHEME encScheme;
} PACKED TCM_TRANSPORT_PUBLIC;

typedef struct TCM_TRANSPORT_LOG_IN
{
	TCM_STRUCTURE_TAG tag;
	TCM_DIGEST parameters;
	TCM_DIGEST pubKeyHash;
} PACKED TCM_TRANSPORT_LOG_IN;

typedef struct TCM_TRANSPORT_LOG_OUT
{
	TCM_STRUCTURE_TAG tag;
	TCM_CURRENT_TICKS currentTicks;
	TCM_DIGEST parameters;
	TCM_MODIFIER_INDICATOR locality;
} PACKED TCM_TRANSPORT_LOG_OUT;

typedef struct TCM_TRANSPORT_AUTH
{
	TCM_STRUCTURE_TAG tag;
	TCM_AUTHDATA authData;
} PACKED TCM_TRANSPORT_AUTH;

typedef struct TCM_AUDIT_EVENT_IN
{
	TCM_STRUCTURE_TAG tag;
	TCM_DIGEST inputParms;
	TCM_COUNTER_VALUE auditCount;
} PACKED TCM_AUDIT_EVENT_IN;

typedef struct TCM_AUDIT_EVENT_OUT
{
	TCM_STRUCTURE_TAG tag;
//	TCM_COMMAND_CODE ordinal;
	TCM_DIGEST outputParms;
	TCM_COUNTER_VALUE auditCount;
//	TCM_RESULT returnCode;
} PACKED TCM_AUDIT_EVENT_OUT;

typedef struct TCM_FAMILY_TABLE_ENTRY
{
	TCM_STRUCTURE_TAG tag;
	TCM_FAMILY_LABEL familyLabel;
	TCM_FAMILY_ID familyId;
	TCM_FAMILY_VERIFICATION verificationCount;
	TCM_FAMILY_FLAGS flags;
} PACKED TCM_FAMILY_TABLE_ENTRY;

#define TCM_NUM_FAMILY_TABLE_ENTRY_MIN 8

typedef struct TCM_FAMILY_TABLE
{
	TCM_FAMILY_TABLE_ENTRY famTableRow[TCM_NUM_FAMILY_TABLE_ENTRY_MIN];
} PACKED TCM_FAMILY_TABLE;

typedef struct TCM_DELEGATIONS
{
	TCM_STRUCTURE_TAG tag;
	TCM_UINT32 delegateType;
	TCM_UINT32 per1;
	TCM_UINT32 per2;
} PACKED TCM_DELEGATIONS;

typedef struct TCM_DELEGATE_PUBLIC
{
	TCM_STRUCTURE_TAG tag;
	TCM_DELEGATE_LABEL rowLabel;
	TCM_PCR_INFO pcrInfo;
	TCM_DELEGATIONS permissions;
	TCM_FAMILY_ID familyId;
	TCM_FAMILY_VERIFICATION verificationCount;
} PACKED TCM_DELEGATE_PUBLIC;

typedef struct TCM_DELEGATE_TABLE_ROW
{
	TCM_STRUCTURE_TAG tag;
	TCM_DELEGATE_PUBLIC pub;
	TCM_SECRET authValue;
} PACKED TCM_DELEGATE_TABLE_ROW;

#define TCM_NUM_DELEGATE_TABLE_ENTRY_MIN 2

typedef struct TCM_DELEGATE_TABLE
{
	TCM_DELEGATE_TABLE_ROW delRow[TCM_NUM_DELEGATE_TABLE_ENTRY_MIN];
} PACKED TCM_DELEGATE_TABLE;

typedef struct TCM_DELEGATE_SENSITIVE
{
	TCM_STRUCTURE_TAG tag;
	TCM_SECRET authValue;
} PACKED TCM_DELEGATE_SENSITIVE;

typedef struct TCM_DELEGATE_OWNER_BLOB
{
	TCM_STRUCTURE_TAG tag;
	TCM_DELEGATE_PUBLIC pub;
	TCM_DIGEST integrityDigest;
	TCM_UINT32 additionalSize;
	TCM_BYTE* additionalArea;
	TCM_UINT32 sensitiveSize;
	TCM_BYTE* sensitiveArea;
} PACKED TCM_DELEGATE_OWNER_BLOB;

typedef struct TCM_DELEGATE_KEY_BLOB
{
	TCM_STRUCTURE_TAG tag;
	TCM_DELEGATE_PUBLIC pub;
	TCM_DIGEST integrityDigest;
	TCM_DIGEST pubKeyDigest;
	TCM_UINT32 additionalSize;
	TCM_BYTE* additionalArea;
	TCM_UINT32 sensitiveSize;
	TCM_BYTE* sensitiveArea;
} PACKED TCM_DELEGATE_KEY_BLOB;

// NV structures
typedef struct TCM_NV_ATTRIBUTES
{
	TCM_STRUCTURE_TAG tag;
	TCM_UINT32 attributes;
} PACKED TCM_NV_ATTRIBUTES;

typedef struct TCM_NV_DATA_PUBLIC 
{
	TCM_STRUCTURE_TAG tag;
	TCM_NV_INDEX nvIndex;
	TCM_PCR_INFO pcrInfoRead;
	TCM_PCR_INFO pcrInfoWrite;
	TCM_NV_ATTRIBUTES permission;
	TCM_BOOL bReadSTClear;
	TCM_BOOL bWriteSTClear;
	TCM_BOOL bWriteDefine;
	TCM_UINT32 dataSize;
} PACKED TCM_NV_DATA_PUBLIC;

typedef struct TCM_GPIO_CHANNEL
{
	TCM_STRUCTURE_TAG tag;
	TCM_PLATFORM_SPECIFIC ps;
	TCM_UINT16 channelNumber;
	TCM_GPIO_ATTRIBUTES attr;
	TCM_GPIO_BUS busInfo;
	TCM_UINT32 sizeOfAddress;
	TCM_BYTE* address;
	TCM_UINT32 sizeOfPubKey;
	TCM_DIGEST* pubKey;
	TCM_UINT32 sizeOfPcrInfo;
	TCM_BYTE* pcrInfo;
} PACKED TCM_GPIO_CHANNEL;

typedef struct TCM_GPIO_AUTHORIZE
{
	TCM_STRUCTURE_TAG tag;
	TCM_GPIO_CHANNEL channel;
	TCM_DIGEST blobIntegrity;
	TCM_UINT32 additionalSize;
	TCM_BYTE* additionalData;
	TCM_UINT32 sensitiveSize;
	TCM_BYTE* sensitiveData;
} PACKED TCM_GPIO_AUTHORIZE;

typedef struct TCM_CONTEXT_BLOB
{
	TCM_STRUCTURE_TAG tag;
	TCM_RESOURCE_TYPE resourceType;
	TCM_HANDLE handle;
	TCM_BYTE label[16];
	TCM_UINT32 contextCount;
	TCM_DIGEST blobIntegrity;
	TCM_UINT32 additionalSize;
	TCM_BYTE* additionalData;
	TCM_UINT32 sensitiveSize;
	TCM_BYTE* sensitiveData;
} PACKED TCM_CONTEXT_BLOB;

typedef struct TCM_SELECT_SIZE 
{
	TCM_BYTE major;
	TCM_BYTE minor;
	TCM_UINT16 reqSize;
}PACKED TCM_SELECT_SIZE;

typedef struct TCM_CAP_VERSION_INFO {
	TCM_STRUCTURE_TAG tag;
	TCM_VERSION version;
	TCM_UINT16 specLevel;
	TCM_BYTE errataRev;
	TCM_BYTE tcmVendorID[4];
	TCM_UINT16 vendorSpecificSize;
	TCM_BYTE* vendorSpecific;
} TCM_CAP_VERSION_INFO;

// TCM_COMMAND_CODE values
//
#define    TCM_MAIN                    0x00008000
#define    TCM_PC                      0x00010000
#define    TCM_PDA                     0x00020000
#define    TCM_CELL_PHONE              0x00030000
#define    TCM_SERVER                  0x00040000

#define    TCM_PROTECTED_ORDINAL                 (TCM_PROTECTED_COMMAND | TCM_MAIN)
#define    TCM_UNPROTECTED_ORDINAL               (TCM_UNPROTECTED_COMMAND | TCM_MAIN)
#define    TCM_CONNECTION_ORDINAL                (TCM_CONNECTION_COMMAND | TCM_MAIN)


#define    TCM_ORD_ActivateIdentity                        (TCM_PROTECTED_ORDINAL + 122)
#define    TCM_ORD_CertifyKey                              (TCM_PROTECTED_ORDINAL + 50)
#define    TCM_ORD_CertifyKey2                             (TCM_PROTECTED_ORDINAL + 51)
#define    TCM_ORD_ChangeAuth                              (TCM_PROTECTED_ORDINAL + 12)
#define    TCM_ORD_ChangeAuthOwner                         (TCM_PROTECTED_ORDINAL + 16)
#define    TCM_ORD_ContinueSelfTest                        (TCM_PROTECTED_ORDINAL + 83)
#define		TCM_ORD_ConvertMigrationBlob					(TCM_PROTECTED_ORDINAL + 194)
#define    TCM_ORD_CreateCounter                           (TCM_PROTECTED_ORDINAL + 220)
#define    TCM_ORD_CreateEndorsementKeyPair                (TCM_PROTECTED_ORDINAL + 120)
#define TCM_ORD_CreateMigrationBlob							(TCM_PROTECTED_ORDINAL + 193)
#define    TCM_ORD_CreateWrapKey                           (TCM_PROTECTED_ORDINAL + 31)
#define    TCM_ORD_CreateRevocableEK                       (TCM_PROTECTED_ORDINAL + 127)
#define    TCM_ORD_Delegate_CreateKeyDelegation            (TCM_PROTECTED_ORDINAL + 212)
#define    TCM_ORD_Delegate_CreateOwnerDelegation          (TCM_PROTECTED_ORDINAL + 213)
#define    TCM_ORD_Delegate_LoadOwnerDelegation            (TCM_PROTECTED_ORDINAL + 216)
#define    TCM_ORD_Delegate_Manage                         (TCM_PROTECTED_ORDINAL + 210)
#define    TCM_ORD_Delegate_ReadTable                      (TCM_PROTECTED_ORDINAL + 219)
#define    TCM_ORD_Delegate_UpdateVerification             (TCM_PROTECTED_ORDINAL + 209)
#define    TCM_ORD_Delegate_VerifyDelegation               (TCM_PROTECTED_ORDINAL + 214)
#define    TCM_ORD_DisableForceClear                       (TCM_PROTECTED_ORDINAL + 94)
#define    TCM_ORD_DisableOwnerClear                       (TCM_PROTECTED_ORDINAL + 92)
#define		TCM_ORD_DisablePubekRead						0x0000007e // 126
//#define    TCM_ORD_DSAP                                    (TCM_PROTECTED_ORDINAL + 17)
#define    TCM_ORD_EstablishTransport                      (TCM_PROTECTED_ORDINAL + 230)
#define    TCM_ORD_ExecuteTransport                        (TCM_PROTECTED_ORDINAL + 231)
#define    TCM_ORD_Extend                                  (TCM_PROTECTED_ORDINAL + 20)
#define    TCM_ORD_FieldUpgrade                            (TCM_PROTECTED_ORDINAL + 170)
#define    TCM_ORD_FlushSpecific                           (TCM_PROTECTED_ORDINAL + 186)
#define    TCM_ORD_ForceClear                              (TCM_PROTECTED_ORDINAL + 93)
#define    TCM_ORD_GetAuditDigest                          (TCM_PROTECTED_ORDINAL + 133)
#define    TCM_ORD_GetAuditDigestSigned                    (TCM_PROTECTED_ORDINAL + 134)
#define    TCM_ORD_GetCapability                           (TCM_PROTECTED_ORDINAL + 101)
#define    TCM_ORD_GetPubKey                               (TCM_PROTECTED_ORDINAL + 33)
#define    TCM_ORD_GetRandom                               (TCM_PROTECTED_ORDINAL + 70)
#define    TCM_ORD_GetTestResult                           (TCM_PROTECTED_ORDINAL + 84)
#define    TCM_ORD_GetTicks                                 (TCM_PROTECTED_ORDINAL + 241)
#define    TCM_ORD_IncrementCounter                        (TCM_PROTECTED_ORDINAL + 221)
//#define    TCM_ORD_Init                                    (TCM_PROTECTED_ORDINAL + 151)
#define    TCM_ORD_LoadContext                             (TCM_PROTECTED_ORDINAL + 185)
#define    TCM_ORD_MakeIdentity                            (TCM_PROTECTED_ORDINAL + 121)
#define    TCM_ORD_NV_DefineSpace                          (TCM_PROTECTED_ORDINAL + 204)
#define    TCM_ORD_NV_ReadValue                            (TCM_PROTECTED_ORDINAL + 207)
#define    TCM_ORD_NV_ReadValueAuth                        (TCM_PROTECTED_ORDINAL + 208)
#define    TCM_ORD_NV_WriteValue                           (TCM_PROTECTED_ORDINAL + 205)
#define    TCM_ORD_NV_WriteValueAuth                       (TCM_PROTECTED_ORDINAL + 206)
#define    TCM_ORD_OwnerClear                              (TCM_PROTECTED_ORDINAL + 91)
#define    TCM_ORD_OwnerReadInternalPub                    (TCM_PROTECTED_ORDINAL + 129)
#define		TCM_ORD_OwnerReadPubek           0x0000007d // 125
#define    TCM_ORD_OwnerSetDisable                         (TCM_PROTECTED_ORDINAL + 110)
#define    TCM_ORD_PCR_Reset                               (TCM_PROTECTED_ORDINAL + 200)
#define    TCM_ORD_PcrRead                                 (TCM_PROTECTED_ORDINAL + 21)
#define    TCM_ORD_PhysicalDisable                         (TCM_PROTECTED_ORDINAL + 112)
#define    TCM_ORD_PhysicalEnable                          (TCM_PROTECTED_ORDINAL + 111)
#define    TCM_ORD_PhysicalSetDeactivated                  (TCM_PROTECTED_ORDINAL + 114)
#define    TCM_ORD_Quote                                   (TCM_PROTECTED_ORDINAL + 22)
#define    TCM_ORD_Quote2                                   (TCM_PROTECTED_ORDINAL + 62)
#define    TCM_ORD_ReadCounter                             (TCM_PROTECTED_ORDINAL + 222)
#define    TCM_ORD_ReadPubek                               (TCM_PROTECTED_ORDINAL + 124)
#define    TCM_ORD_ReleaseCounter                          (TCM_PROTECTED_ORDINAL + 223)
#define    TCM_ORD_ReleaseCounterOwner                     (TCM_PROTECTED_ORDINAL + 224)
#define    TCM_ORD_ReleaseTransportSigned                  (TCM_PROTECTED_ORDINAL + 232)
#define    TCM_ORD_ResetLockValue                          (TCM_PROTECTED_ORDINAL + 64)
#define    TCM_ORD_RevokeTrust                             (TCM_PROTECTED_ORDINAL + 128)
#define    TCM_ORD_SaveContext                             (TCM_PROTECTED_ORDINAL + 184)
#define    TCM_ORD_SaveState                               (TCM_PROTECTED_ORDINAL + 152)
#define    TCM_ORD_Seal                                    (TCM_PROTECTED_ORDINAL + 23)
#define    TCM_ORD_Sealx                                    (TCM_PROTECTED_ORDINAL + 61)
#define    TCM_ORD_SelfTestFull                            (TCM_PROTECTED_ORDINAL + 80)
#define    TCM_ORD_SetCapability                           (TCM_PROTECTED_ORDINAL + 63)
#define    TCM_ORD_SetOperatorAuth                         (TCM_PROTECTED_ORDINAL + 116)
#define    TCM_ORD_SetOrdinalAuditStatus                   (TCM_PROTECTED_ORDINAL + 141)
#define    TCM_ORD_SetOwnerInstall                         (TCM_PROTECTED_ORDINAL + 113)
#define    TCM_ORD_SetTempDeactivated                      (TCM_PROTECTED_ORDINAL + 115)
#define    TCM_ORD_Sign                                    (TCM_PROTECTED_ORDINAL + 60)
#define    TCM_ORD_Startup                                 (TCM_PROTECTED_ORDINAL + 153)
#define    TCM_ORD_StirRandom                              (TCM_PROTECTED_ORDINAL + 71)
#define    TCM_ORD_TakeOwnership                           (TCM_PROTECTED_ORDINAL + 13)
#define TCM_ORD_Terminate_Handle         0x00000096 // 150
#define    TCM_ORD_TickStampBlob                           (TCM_PROTECTED_ORDINAL + 242)
//#define    TCM_ORD_UnBind                                  (TCM_PROTECTED_ORDINAL + 30)
#define    TCM_ORD_Unseal                                  (TCM_PROTECTED_ORDINAL + 24)

//#define   TCM_ORD_CertifyKeyA      			 (TCM_PROTECTED_ORDINAL + 190)
//#define   TCM_ORD_CertifyKeyB				 (TCM_PROTECTED_ORDINAL + 24)

#define    TCM_ORD_WrapKey  				(TCM_PROTECTED_ORDINAL + 189)
#define    TCM_ORD_WrapPEK					(TCM_PROTECTED_ORDINAL + 190)
#define    TCM_ORD_APCreate					(TCM_PROTECTED_ORDINAL + 191)
#define    TCM_ORD_APTerminate				(TCM_PROTECTED_ORDINAL + 192)
#define    TCM_ORD_CreateMigratedBlob		(TCM_PROTECTED_ORDINAL + 193)
#define    TCM_ORD_ConvertMigratedBlob		(TCM_PROTECTED_ORDINAL + 194)
#define    TCM_ORD_AuthorizeMigrationKey	(TCM_PROTECTED_ORDINAL + 195)
//#define    TCM_ORD_KeyNegotiation			(TCM_PROTECTED_ORDINAL + 196)
#define    TCM_ORD_SMS4Encrypt				(TCM_PROTECTED_ORDINAL + 197)
#define    TCM_ORD_SMS4Decrypt				(TCM_PROTECTED_ORDINAL + 198)
#define    TCM_ORD_ReadEKCert				(TCM_PROTECTED_ORDINAL + 199)
#define    TCM_ORD_ReleaseTransport         (TCM_PROTECTED_ORDINAL + 232)
#define    TCM_ORD_WriteEKCert				(TCM_PROTECTED_ORDINAL + 233)
#define    TCM_ORD_SCHStart					(TCM_PROTECTED_ORDINAL + 234)
#define    TCM_ORD_SCHUpdate				(TCM_PROTECTED_ORDINAL + 235)
#define    TCM_ORD_SCHComplete				(TCM_PROTECTED_ORDINAL + 236)
#define    TCM_ORD_SCHCompleteExtend		(TCM_PROTECTED_ORDINAL + 237)
#define    TCM_ORD_ECCDecrypt				(TCM_PROTECTED_ORDINAL + 238)
#define    TCM_ORD_LoadKey					(TCM_PROTECTED_ORDINAL + 239)

#define    TCM_ORD_ReleaseECCExchangeSession	(TCM_PROTECTED_ORDINAL + 174)
#define    TCM_ORD_CreateECCExchangeSession		(TCM_PROTECTED_ORDINAL + 175)
#define    TCM_ORD_GetKeyECCExchangeSession		(TCM_PROTECTED_ORDINAL + 176)


#define TCM_ORD_FIRST                    TCM_ORD_OIAP
#define TCM_ORD_LAST                    TCM_ORD_TickStampBlob
// TSC commands
#define    TSC_ORD_PhysicalPresence       		           (TCM_CONNECTION_ORDINAL + 10)
#define    TSC_ORD_ResetEstablishmentBit       			   (TCM_CONNECTION_ORDINAL + 11)

//add new PEK here
#define    TCM_ORD_ActivatePEKCert		 (TCM_CONNECTION_ORDINAL + 218)
#define    TCM_ORD_ActivatePEK			 (TCM_CONNECTION_ORDINAL + 217)
//
//已经没有的命令号，但为了编译通过而暂时留着，
#define TCM_ORD_OIAP                     0x0000000a // 10
#define TCM_ORD_FIRST                    TCM_ORD_OIAP
#define TCM_ORD_OSAP                     0x0000000b // 11


#define TCM_ORD_ChangeAuthAsymStart      0x0000000e // 14
#define TCM_ORD_ChangeAuthAsymFinish     0x0000000f // 15


#define TCM_ORD_DirWriteAuth             0x00000019 // 25
#define TCM_ORD_DirRead                  0x0000001a // 26
#define TCM_ORD_UnBind                   0x0000001e // 30
#define TCM_ORD_EvictKey                 0x00000022 // 34
//#define TCM_ORD_CreateMigrationBlob      0x00000028 // 40
#define TCM_ORD_ReWrapKey                0x00000029 // 41
//#define TCM_ORD_ConvertMigrationBlob     0x0000002a // 42
#define TCM_ORD_CreateMaintenanceArchive 0x0000002c // 44
#define TCM_ORD_LoadMaintenanceArchive   0x0000002d // 45
#define TCM_ORD_KillMaintenanceFeature   0x0000002e // 46
#define TCM_ORD_LoadManuMaintPub         0x0000002f // 47
#define TCM_ORD_ReadManuMaintPub         0x00000030 // 48   


//#define TCM_ORD_StirRandom               0x00000047 // 71
//#define TCM_ORD_SelfTestFull             0x00000050 // 80
#define TCM_ORD_SelfTestStartup          0x00000051 // 81
#define TCM_ORD_CertifySelfTest          0x00000052 // 82
//#define TCM_ORD_ContinueSelfTest         0x00000053 // 83
//#define TCM_ORD_GetTestResult            0x00000054 // 84
// #define TCM_ORD_Reset                    0x0000005a // 90

//#define TCM_ORD_DisableOwnerClear        0x0000005c // 92
//#define TCM_ORD_DisableForceClear        0x0000005e // 94
#define TCM_ORD_GetCapabilitySigned      0x00000064 // 100
#define TCM_ORD_GetCapabilityOwner       0x00000066 // 102
//#define TCM_ORD_OwnerSetDisable          0x0000006e // 110
//#define TCM_ORD_PhysicalEnable           0x0000006f // 111
//#define TCM_ORD_PhysicalDisable          0x00000070 // 112
//#define TCM_ORD_SetOwnerInstall          0x00000071 // 113
//#define TCM_ORD_PhysicalSetDeactivated   0x00000072 // 114
//#define TCM_ORD_SetTempDeactivated       0x00000073 // 115
//#define TCM_ORD_CreateEndorsementKeyPair 0x00000078 // 120
#define TCM_ORD_OwnerReadPubek           0x0000007d // 125
#define TCM_ORD_DisablePubekRead         0x0000007e // 126
#define TCM_ORD_GetAuditEvent            0x00000082 // 130
#define TCM_ORD_GetAuditEventSigned      0x00000083 // 131
#define TCM_ORD_GetOrdinalAuditStatus    0x0000008c // 140
//#define TCM_ORD_SetOrdinalAuditStatus    0x0000008d // 141
#define TCM_ORD_Terminate_Handle         0x00000096 // 150
#define TCM_ORD_Init                     0x00000097 // 151
//#define TCM_ORD_SaveState                0x00000098 // 152
//#define TCM_ORD_Startup                  0x00000099 // 153
#define TCM_ORD_SetRedirection           0x0000009a // 154
#define TCM_ORD_SHA1Start                0x000000a0 // 160
#define TCM_ORD_SHA1Update               0x000000a1 // 161
#define TCM_ORD_SHA1Complete             0x000000a2 // 162
#define TCM_ORD_SHA1CompleteExtend       0x000000a3 // 163
//#define TCM_ORD_FieldUpgrade             0x000000aa // 170
#define TCM_ORD_SaveKeyContext           0x000000b4 // 180
#define TCM_ORD_LoadKeyContext           0x000000b5 // 181
#define	TCM_ORD_SaveAuthContext          0x000000b6 // 182
#define	TCM_ORD_LoadAuthContext          0x000000b7 // 183



#define TCM_ORD_DSAP                           0x00000011 // 17
#define TCM_ORD_CMK_CreateTicket               0x00000012 // 18
#define TCM_ORD_CMK_CreateKey                  0x00000013 // 19
#define TCM_ORD_CMK_CreateBlob                 0x0000001b // 27
#define TCM_ORD_CMK_ConvertMigration           0x00000024 // 24
#define TCM_ORD_MigrateKey                     0x00000025 // 25
#define TCM_ORD_CMK_SetRestrictions            0x0000001c // 28
#define TCM_ORD_CMK_ApproveMA                  0x0000001d // 29	
#define TCM_ORD_KeyControlOwner                0x00000023 // 35
#define TCM_ORD_GPIO_AuthChannel               0x00000026 // 38
#define TCM_ORD_GPIO_ReadWrite                 0x00000027 // 39
#define TCM_ORD_DAA_Join                       0x00000029 // 41
#define TCM_ORD_DAA_Sign                       0x00000031 // 49
//#define TCM_ORD_CertifyKey2                    0x00000033 // 51
//#define TCM_ORD_SetOperatorAuth                0x00000074 // 116
#define TCM_ORD_SetOwnerPointer                0x00000075 // 117
//#define TCM_ORD_CreateRevocableEK              0x0000007f // 127
//#define TCM_ORD_RevokeTrust                    0x00000080 // 128
//#define TCM_ORD_OwnerReadInternalPub           0x00000081 // 129
//#define TCM_ORD_GetAuditDigest                 0x00000085 // 133
//#define TCM_ORD_GetAuditDigestSigned           0x00000086 // 134
//#define TCM_ORD_SaveContext                    0x000000b8 // 184
//#define TCM_ORD_LoadContext                    0x000000b9 // 185
//#define TCM_ORD_FlushSpecific                  0x000000ba // 186
//#define TCM_ORD_PCR_Reset                      0x000000c8 // 200
//#define TCM_ORD_Delegate_UpdateVerification    0x000000d1 // 209
//#define TCM_ORD_Delegate_Manage                0x000000d2 // 210
//#define TCM_ORD_Delegate_CreateKeyDelegation   0x000000d4 // 212
//#define TCM_ORD_Delegate_CreateOwnerDelegation 0x000000d5 // 213
//#define TCM_ORD_Delegate_VerifyDelegation      0x000000d6 // 214
//#define TCM_ORD_Delegate_LoadOwnerDelegation   0x000000d8 // 216
//#define TCM_ORD_Delegate_ReadTable             0x000000db // 219
//#define TCM_ORD_NV_DefineSpace                 0x000000cc // 204
//#define TCM_ORD_NV_WriteValue                  0x000000cd // 205
//#define TCM_ORD_NV_WriteValueAuth              0x000000ce // 206
//#define TCM_ORD_NV_ReadValue                   0x000000cf // 207
//#define TCM_ORD_NV_ReadValueAuth               0x000000d0 // 208
//#define TCM_ORD_CreateCounter                  0x000000dc // 220
//#define TCM_ORD_IncrementCounter               0x000000dd // 221
//#define TCM_ORD_ReadCounter                    0x000000de // 222
//#define TCM_ORD_ReleaseCounter                 0x000000df // 223
//#define TCM_ORD_ReleaseCounterOwner            0x000000e0 // 224
//#define TCM_ORD_EstablishTransport             0x000000e6 // 230
//#define TCM_ORD_ExecuteTransport               0x000000e7 // 231
//#define TCM_ORD_ReleaseTransportSigned         0x000000e8 // 232
#define TCM_ORD_SetTickType                    0x000000f0 // 240
//#define TCM_ORD_GetTicks                       0x000000f1 // 241
//#define TCM_ORD_TickStampBlob                  0x000000f2 // 242
//#define TCM_ORD_LAST TCM_ORD_TickStampBlob
#define TCM_ORD_Seal_Ex                        0x000000f3 //243 add by shiyong@lenovo
//#define TCM_ORD_LAST TCM_ORD_Seal_Ex



//密码学库测试用命令号
#define    TCM_CRYPTOTEST_BASE				(0x9000)

#define    TCM_ORD_SMS4_E					(TCM_CRYPTOTEST_BASE + 1)
#define    TCM_ORD_SMS4_D					(TCM_CRYPTOTEST_BASE + 2)
#define    TCM_ORD_SCH_Init					(TCM_CRYPTOTEST_BASE + 3)
#define    TCM_ORD_SCH_Update				(TCM_CRYPTOTEST_BASE + 4)
#define    TCM_ORD_SCH_Final				(TCM_CRYPTOTEST_BASE + 5)
#define    TCM_ORD_SCH_Hash					(TCM_CRYPTOTEST_BASE + 6)
#define    TCM_ORD_ECC_E					(TCM_CRYPTOTEST_BASE + 7)
#define    TCM_ORD_ECC_D					(TCM_CRYPTOTEST_BASE + 8)
#define    TCM_ORD_ECC_S					(TCM_CRYPTOTEST_BASE + 9)
#define    TCM_ORD_ECC_V					(TCM_CRYPTOTEST_BASE + 10)
#define    TCM_ORD_ECC_KDF					(TCM_CRYPTOTEST_BASE + 11)
#define    TCM_ORD_HMAC						(TCM_CRYPTOTEST_BASE + 12)
#define    TCM_ORD_CHECK_POINT				(TCM_CRYPTOTEST_BASE + 13)
#define    TCM_ORD_CREATE_KEY				(TCM_CRYPTOTEST_BASE + 14)
#define	   TCM_ORD_EXCHANGE_KEY			    (TCM_CRYPTOTEST_BASE + 15)
#define    TCM_ORD_Reset				    (TCM_CRYPTOTEST_BASE + 16)
//
// TCM_RESULT values
//
#define TCM_SUCCESS            0x00000000 // 0
#define TCM_AUTHFAIL           0x00000001 // 1
#define TCM_BADINDEX           0x00000002 // 2
#define TCM_BAD_PARAMETER      0x00000003 // 3
#define TCM_AUDITFAILURE       0x00000004 // 4
#define TCM_CLEAR_DISABLED     0x00000005 // 5
#define TCM_DEACTIVATED        0x00000006 // 6
#define TCM_DISABLED           0x00000007 // 7
#define TCM_DISABLED_CMD       0x00000008 // 8
#define TCM_FAIL               0x00000009 // 9
#define TCM_BAD_ORDINAL        0x0000000a // 10
#define TCM_INSTALL_DISABLED   0x0000000b // 11
#define TCM_INVALID_KEYHANDLE  0x0000000c // 12
#define TCM_KEYNOTFOUND        0x0000000d // 13
#define TCM_INAPPROPRIATE_ENC  0x0000000e // 14
#define TCM_MIGRATEFAIL        0x0000000f // 15
#define TCM_INVALID_PCR_INFO   0x00000010 // 16
#define TCM_NOSPACE            0x00000011 // 17
#define TCM_NOSMK              0x00000012 // 18
#define TCM_NOTSEALED_BLOB     0x00000013 // 19
#define TCM_OWNER_SET          0x00000014 // 20
#define TCM_RESOURCES          0x00000015 // 21
#define TCM_SHORTRANDOM        0x00000016 // 22
#define TCM_SIZE               0x00000017 // 23
#define TCM_WRONGPCRVAL        0x00000018 // 24
#define TCM_BAD_PARAM_SIZE     0x00000019 // 25
#define TCM_SCH_THREAD         0x0000001a // 26
#define TCM_SCH_ERROR          0x0000001b // 27
#define TCM_FAILEDSELFTEST     0x0000001c // 28
#define TCM_AUTH2FAIL          0x0000001d // 29
#define TCM_BADTAG             0x0000001e // 30
#define TCM_IOERROR            0x0000001f // 31
#define TCM_ENCRYPT_ERROR      0x00000020 // 32
#define TCM_DECRYPT_ERROR      0x00000021 // 33
#define TCM_INVALID_AUTHHANDLE 0x00000022 // 34
#define TCM_NO_ENDORSEMENT     0x00000023 // 35
#define TCM_INVALID_KEYUSAGE   0x00000024 // 36
#define TCM_WRONG_ENTITYTYPE   0x00000025 // 37
#define TCM_INVALID_POSTINIT   0x00000026 // 38
#define TCM_INAPPROPRIATE_SIG  0x00000027 // 39
#define TCM_BAD_KEY_PROPERTY   0x00000028 // 40
#define TCM_BAD_MIGRATION      0x00000029 // 41
#define TCM_BAD_SCHEME         0x0000002a // 42
#define TCM_BAD_DATASIZE       0x0000002b // 43
#define TCM_BAD_MODE           0x0000002c // 44
#define TCM_BAD_PRESENCE       0x0000002d // 45
#define TCM_BAD_VERSION        0x0000002e // 46
#define TCM_RETRY              0x00000800 // 2048
#define TCM_DEFEND_LOCK_RUNNING   0x00000803 // 2048

#define TCM_NO_WRAP_TRANSPORT      0x0000002f // 47
#define TCM_AUDITFAIL_UNSUCCESSFUL 0x00000030 // 48
#define TCM_AUDITFAIL_SUCCESSFUL   0x00000031 // 49
#define TCM_NOTRESETABLE           0x00000032 // 50
#define TCM_NOTLOCAL               0x00000033 // 51
#define TCM_BAD_TYPE               0x00000034 // 52
#define TCM_INVALID_RESOURCE       0x00000035 // 53
#define TCM_NOTFIPS                0x00000036 // 54
#define TCM_INVALID_FAMILY         0x00000037 // 55
#define TCM_NO_NV_PERMISSION       0x00000038 // 56
#define TCM_REQUIRES_SIGN          0x00000039 // 57
#define TCM_KEY_NOTSUPPORTED       0x0000003a // 58
#define TCM_AUTH_CONFLICT          0x0000003b // 59
#define TCM_AREA_LOCKED            0x0000003c // 60
#define TCM_BAD_LOCALITY           0x0000003d // 61
#define TCM_READ_ONLY              0x0000003e // 62
#define TCM_PER_NOWRITE            0x0000003f // 63
#define TCM_FAMILYCOUNT            0x00000040 // 64
#define TCM_WRITE_LOCKED           0x00000041 // 65
#define TCM_BAD_ATTRIBUTES         0x00000042 // 66
#define TCM_INVALID_STRUCTURE      0x00000043 // 67
#define TCM_KEY_OWNER_CONTROL      0x00000044 // 68
#define TCM_BAD_COUNTER            0x00000045 // 69
#define TCM_NOT_FULLWRITE          0x00000046 // 70
#define TCM_CONTEXT_GAP            0x00000047 // 71
#define TCM_MAXNVWRITES            0x00000048 // 72
#define TCM_NOOPERATOR             0x00000049 // 73
#define TCM_RESOURCEMISSING        0x0000004a // 74
#define TCM_DELEGATE_LOCK          0x0000004b // 75
#define TCM_DELEGATE_FAMILY        0x0000004c // 76
#define TCM_DELEGATE_ADMIN         0x0000004d // 77
#define TCM_TRANS_EXCLUSIVE        0x0000004e // 78
#define TCM_OWNER_CONTROL          0x0000004f // 79
#define TCM_DAA_RESOURCES          0x00000050 // 80
#define TCM_DAA_INPUT_DATA0        0x00000051 // 81
#define TCM_DAA_INPUT_DATA1        0x00000052 // 82
#define TCM_DAA_ISSUER_SETTINGS    0x00000053 // 83
#define TCM_DAA_TCM_SETTINGS       0x00000054 // 84
#define TCM_DAA_STAGE              0x00000055 // 85
#define TCM_DAA_ISSUER_VALIDITY    0x00000056 // 86
#define TCM_DAA_WRONG_W            0x00000057 // 87
#define TCM_BAD_HANDLE			   0x00000058 // 88	

#define TCM_BAD_DELEGATE		   0x00000059 // 89	//add by zhanghuajun 2006/11/22
#define TCM_NOCONTEXTSPACE		   0x00000059 // 89	
#define TCM_BADCONTEXT			   0x0000005a // 90	
#define TCM_TOOMANYCONTEXTS		   0x0000005b // 91	
#define TCM_MA_TICKET_SIGNATURE	   0x0000005c // 92	
#define TCM_MA_DESTINATION		   0x0000005d // 93	
#define TCM_MA_SOURCE			   0x0000005e // 94	
#define TCM_MA_AUTHORITY           0x0000005f // 95
#define TCM_PERMANENTEK            0x00000061 // 97
#define TCM_BAD_SIGNATURE          0x00000062 // 98
#define TCM_BEGIN                  0x00000063 // 99

// Turn off structure packing
#undef PACKED
#ifdef _MSC_VER
#pragma pack(pop)
#endif /* _MSC_VER */

#endif /* __TCM_H__ */
