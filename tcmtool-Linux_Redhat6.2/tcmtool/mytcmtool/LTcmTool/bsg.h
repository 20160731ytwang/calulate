
/*
 * File Name :  bsg.h
 * Author :  zhangyz@litsoft.com.cn         
 * Date : 2006/11/10 									
 * Description :   主要用于将TCM中的数据结构转化为字符串                     
 * Version : 0.1       		              
 * Function List : 
 */

#ifndef __BSG_H__
#define __BSG_H__

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
// #include "tsi.h"

/**
 * Byte stream generator
 */
class BSG
{
public:
	enum Type
	{
		TCM_RESULT = 1,
		TCM_COMMAND_CODE,
		TCM_AUTH_DATA_USAGE,
		TCM_ALGORITHM_ID,
		TCM_KEY_USAGE,
		TCM_ENC_SCHEME,
		TCM_SIG_SCHEME,
		TCM_MIGRATE_SCHEME,
		TCM_KEY_FLAGS,
		TCM_AUTHDATA,
		TCM_SECRET,
		TCM_ENCAUTH,
		TCM_PAYLOAD_TYPE,
		TCM_VERSION,
		TCM_DIGEST,
		TCM_COMPOSITE_HASH,
		TCM_CHOSENID_HASH,
		TCM_NONCE,
		TCM_KEY_HANDLE_LIST,
		TCM_KEY_PARMS,
		TCM_RSA_KEY_PARMS,
		TCM_ECC_KEY_PARMS,//add by zhanghuajun 2006-11-15
		TCM_STORE_PUBKEY,
		TCM_PUBKEY,
		TCM_KEY,
	//    TCM_KEY12,
		TCM_MIGRATIONKEYAUTH,
		TCM_AUDIT_EVENT,
		TCM_EVENT_CERT,
		TCM_PCR_SELECTION,
		TCM_PCR_COMPOSITE,
		TCM_PCR_INFO,
		TCM_STORED_DATA,
		TCM_SEALED_DATA,
		TCM_SYMMETRIC_KEY,
		TCM_STORE_PRIVKEY,
		TCM_STORE_ASYMKEY,
		TCM_MIGRATE_ASYMKEY,
		TCM_CERTIFY_INFO,		
		TCM_QUOTE_INFO,
		TCM_IDENTITY_CONTENTS,
		TCM_BOUND_DATA,
		TCM_HMAC,
		TCM_ASYM_CA_CONTENTS,
		TCM_CHANGEAUTH_VALIDATE,


		TCM_STRUCTURE_TAG,
		TCM_DELEGATE_LABEL,
		TCM_LOCALITY_SELECTION,
		TCM_MODIFIER_INDICATOR,
		TCM_FAMILY_ID,
		TCM_FAMILY_VERIFICATION,
		TCM_TRANSPORT_ATTRIBUTES,
		TCM_PLATFORM_SPECIFIC,
		TCM_MSA_COMPOSITE,
		TCM_CMK_AUTH,
		
		TCM_COUNTER_VALUE,

	//	TCM_PCR_INFO_SHORT,
	//	TCM_PCR_INFO_LONG,

		TCM_DELEGATIONS,
		TCM_DELEGATE_PUBLIC,
		TCM_DELEGATE_TABLE_ROW,
		TCM_DELEGATE_OWNER_BLOB,
		TCM_DELEGATE_KEY_BLOB,

		TCM_CURRENT_TICKS,

		TCM_NV_ATTRIBUTES,
		TCM_NV_DATA_PUBLIC,

		TCM_TRANSPORT_PUBLIC,
		TCM_TRANSPORT_LOG_IN,
		TCM_TRANSPORT_LOG_OUT,
		TCM_TRANSPORT_AUTH,
		
		TCM_GPIO_ATTRIBUTES,
		TCM_GPIO_BUS,
		TCM_GPIO_CHANNEL,
		TCM_GPIO_AUTHORIZE,

		TCM_RESOURCE_TYPE,
		TCM_CONTEXT_BLOB,

		TCM_CERTIFY_INFO2,

		TCM_FAMILY_LABEL,
		TCM_FAMILY_FLAGS,
		TCM_FAMILY_TABLE_ENTRY,
		TCM_STORED_DATA12,
		TCM_PERMANENT_FLAGS,
		TCM_STCLEAR_FLAGS,
		TCM_SELECT_SIZE,
		TCM_CAP_VERSION_INFO,
		TCM_EK_BLOB,
		TCM_EK_TYPE,
	

		TCM_SIGN_INFO
	};

	struct CmdInfo
	{
		int ord;
		int auth;
		int inhandle;
		int outhandle;
	};

	static const CmdInfo* Find(int ord);

	static int Pack(Type type, const void* src, BYTE* dst = NULL)
        {
		const BYTE* src2 = (const BYTE*) src;
		return Pack(type, &src2, dst);
	}
	static int Unpack(Type type, const BYTE* src, void* dst = NULL)
	{
		BYTE* dst2 = (BYTE*) dst;
		return Unpack(type, src, dst ? &dst2:NULL);
	}
	static int Copy(Type type, const void* src, void* dst);
	static void Destroy(Type type, void* src)
	{
		BYTE* src2 = (BYTE*) src;
		Destroy(type, &src2);
	}

	static void Pack16(BYTE* dst, int val);
	static int Unpack16(const BYTE* src);
	static void Pack32(BYTE* dst, int val);
	static int Unpack32(const BYTE* src);
	static void Pack64(BYTE* dst, TCM_UINT64 val);
	static TCM_UINT64 Unpack64(const BYTE* src);
	static void PackConst(unsigned long val, int size, BYTE* dst);
	static unsigned long UnpackConst(const BYTE* src, int size);

	static void Pack32R(BYTE* dst, int val);
	static int Unpack32R(const BYTE* src);

protected:
	struct Format
	{
		enum {MAX_FIELDS = 15};
		Type type;
		const char* name;
		unsigned long fields[MAX_FIELDS + 1];
	};

	static int Pack(Type type, const BYTE** src, BYTE* dst);
	static int Unpack(Type type, const BYTE* src, BYTE** dst);
	static void Destroy(Type type, BYTE** src);

	static Format s_fmt[];
	static CmdInfo s_cmd[];
};

// BuildRequest/VerifyRequest flags
#define __BSG_CONST  (1UL << 31)
#define __BSG_DATA   (1UL << 30)
#define __BSG_OUT    (1UL << 29)
#define __BSG_SIZE16 (1UL << 28)
#define __BSG_SIZE32 (1UL << 27)
#define __BSG_DIGEST (1UL << 26)
#define __BSG_MASK \
    ~(__BSG_CONST | __BSG_DATA | __BSG_OUT \
      | __BSG_SIZE16 | __BSG_SIZE32 | __BSG_DIGEST)

// constant (8/16/32-bits)
#define BSG_U8(value) (__BSG_CONST | __BSG_DIGEST | 1UL), (value)
#define BSG_U16(value) (__BSG_CONST | __BSG_DIGEST | 2UL), (value)
#define BSG_U32(value) (__BSG_CONST | __BSG_DIGEST | 4UL), (value)
#define BSG_U8_OUT(buf) \
    (__BSG_CONST | __BSG_OUT | __BSG_DIGEST | 1UL), (buf)
#define BSG_U16_OUT(buf) \
    (__BSG_CONST | __BSG_OUT | __BSG_DIGEST | 2UL), (buf)
#define BSG_U32_OUT(buf) \
    (__BSG_CONST | __BSG_OUT | __BSG_DIGEST | 4UL), (buf)

// other data
#define BSG_DATA(buf, size) \
    (__BSG_DATA \
     | __BSG_DIGEST \
     | ((unsigned long)(size) & __BSG_MASK)), \
     (buf)
#define BSG_SIZE16_DATA(buf, size) \
    (__BSG_DATA | __BSG_SIZE16 | __BSG_DIGEST | ((size) & __BSG_MASK)), (buf)
#define BSG_SIZE32_DATA(buf, size) \
    (__BSG_DATA | __BSG_SIZE32 | __BSG_DIGEST | ((size) & __BSG_MASK)), (buf)
#define BSG_DATA_OUT(buf, size) \
    (__BSG_DATA | __BSG_OUT | __BSG_DIGEST | ((size) & __BSG_MASK)), (buf)
#define BSG_SIZE16_DATA_OUT(buf, size) \
    (__BSG_DATA | __BSG_OUT | __BSG_SIZE16 | __BSG_DIGEST), (buf), (size)
#define BSG_SIZE32_DATA_OUT(buf, size) \
    (__BSG_DATA | __BSG_OUT | __BSG_SIZE32 | __BSG_DIGEST), (buf), (size)

// authentication/key handle (4-bytes)
#define BSG_HANDLE(handle) (__BSG_CONST | 4UL), (handle)
#define BSG_HANDLE_OUT(buf) \
    (__BSG_CONST | __BSG_OUT | __BSG_DIGEST | 4UL), (buf)

// digest (256-bits)
#define BSG_SIZE16(buf) (__BSG_DATA|__BSG_DIGEST | 16UL), (buf)
#define BSG_DIGEST(buf) (__BSG_DATA | __BSG_DIGEST | 32UL), (buf)
#define BSG_DIGEST_OUT(buf) \
    (__BSG_DATA | __BSG_OUT | __BSG_DIGEST | 32UL), (buf)
// digest (160-bits)
//#define BSG_DIGEST(buf) (__BSG_DATA | __BSG_DIGEST | 32UL), (buf)
//#define BSG_DIGEST_OUT(buf) \
 //   (__BSG_DATA | __BSG_OUT | __BSG_DIGEST | 32UL), (buf)


// TCM structure
#define BSG_STRUCT(type, buf) \
    (__BSG_DIGEST | (((int)(type)) & __BSG_MASK)), (buf)
#define BSG_STRUCT_OUT(type, buf) \
    (__BSG_DIGEST | __BSG_OUT | (((int)(type)) & __BSG_MASK)), (buf)
#define BSG_SIZE16_STRUCT(type, buf) \
    (__BSG_SIZE16 | __BSG_DIGEST | (((int)(type)) & __BSG_MASK)), (buf)
#define BSG_SIZE32_STRUCT(type, buf) \
    (__BSG_SIZE32 | __BSG_DIGEST | (((int)(type)) & __BSG_MASK)), (buf)
#define BSG_SIZE16_STRUCT_OUT(type, buf, size) \
    (__BSG_SIZE16 | __BSG_DIGEST | __BSG_OUT | (((int)(type)) & __BSG_MASK)), \
    (buf), \
    (size)
#define BSG_SIZE32_STRUCT_OUT(type, buf, size) \
    (__BSG_SIZE32 | __BSG_DIGEST | __BSG_OUT | (((int)(type)) & __BSG_MASK)), \
    (buf), \
    (size)

#endif /* __BSG_H__ */
