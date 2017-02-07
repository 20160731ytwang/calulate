/******************************************
File	:calculate_hash.c 
Author	:linyang
Date	:11/21/2006
comment :�����㷨
******************************************/

#include <memory.h>
#include <malloc.h>
#include "openssl/bn.h"
#include "openssl/bnEx.h"
#include "openssl/crypto.h"
#include "tcm_hash.h"
#include "tcm_bn.h"
//#include <Winsock2.h>
#include <arpa/inet.h>


typedef unsigned char BYTE;

#ifndef uint8
#define uint8  unsigned char
#endif

#ifndef uint32
#define uint32 unsigned long int
#endif

/*
����Za������ʹ��256bits��HASH�㷨
�����û����ݣ������û���hash����
������hash���� Digest
���룺�û���ʶ��userid, �û���ʶ������userIDLen, 
���룺��ԿpPubkey_in, ��Կ����pubkeyLen_in,
*/
int tcm_get_usrinfo_value(
					   unsigned char *userID, unsigned short int userIDLen, 
					   unsigned char *pPubkey_in, unsigned int pubkeyLen_in,
					   unsigned char digest[HASH_NUMBITS/8])
{
	int iret;
	/* ECC�����ز�����ϵ��a��b����������xG��yG */
	BIGNUM *a, *b, *xG, *yG, *zG;
	BIGNUM *p;	/*��������*/
	//
	unsigned char	ENTL[2];
	unsigned char*	pstr_a=NULL;
	unsigned char*	pstr_b=NULL;
	unsigned char*	pstr_xG=NULL;
	unsigned char*	pstr_yG=NULL;
	//
	BYTE *pstr_Z=NULL;
	uint32 uZlen = 0;
	int iPos=0;

	/* ���鹫ԿpPubkey_in�Ƿ�Ϊ�� */
	if (pPubkey_in ==NULL)
	{
		return 1;
	}
	/* ���鹫ԿpubkeyLen_in���� */
	if (pubkeyLen_in != PUBKEY_LEN)
	{
		return 1;
	}

	//
	a = BN_new();
	if(a == NULL)
	{
	  return 1;
	}
	b = BN_new();
	if(b == NULL)
	{
	  BN_free(a);
	  return 1;
	}
	xG = BN_new();
	if(xG == NULL)
	{
	  BN_free(a);
	  BN_free(b);
	  return 1;
	}
	yG = BN_new();
	if(yG == NULL)
	{
	  BN_free(a);
	  BN_free(b);
	  BN_free(xG);
	  return 1;
	}
	zG = BN_new();
	if(zG == NULL)
	{
	  BN_free(a);
	  BN_free(b);
	  BN_free(xG);
	  BN_free(yG);
	  return 1;
	}
	//
	p = BN_new();
	if(p == NULL)
	{
	  BN_free(a);
	  BN_free(b);
	  BN_free(xG);
	  BN_free(yG);
	  BN_free(zG);
	  return 1;
	}
	//
	pstr_a = (unsigned char*)malloc(g_uNumbits/8);
	if(pstr_a == NULL)
	{
	  BN_free(a);
	  BN_free(b);
	  BN_free(xG);
	  BN_free(yG);
	  BN_free(zG);
	  BN_free(p);
	  return 1;
	}
	//
	pstr_b = (unsigned char*)malloc(g_uNumbits/8);
	if(pstr_b == NULL)
	{
	  BN_free(a);
	  BN_free(b);
	  BN_free(xG);
	  BN_free(yG);
	  BN_free(zG);
	  BN_free(p);
	  //
	  free(pstr_a);
	  return 1;
	}
	//
	pstr_xG = (unsigned char*)malloc(g_uNumbits/8);
	if(pstr_xG == NULL)
	{
	  BN_free(a);
	  BN_free(b);
	  BN_free(xG);
	  BN_free(yG);
	  BN_free(zG);
	  BN_free(p);
	  //
	  free(pstr_a);
	  free(pstr_b);
	  return 1;
	}
	//
	pstr_yG = (unsigned char*)malloc(g_uNumbits/8);
	if(pstr_yG == NULL)
	{
	  BN_free(a);
	  BN_free(b);
	  BN_free(xG);
	  BN_free(yG);
	  BN_free(zG);
	  BN_free(p);
	  //
	  free(pstr_a);
	  free(pstr_b);
	  free(pstr_xG);
	  return 1;
	}

	/* �õ���Բ����Ⱥ���ز���P */
	EC_GROUP_get_curve_GFp(group,p,a,b);
	EC_POINT_get_point(G, xG, yG, zG);


	//�õ�a,b���ַ���
	tcm_bn_bn2bin(a, g_uNumbits/8, pstr_a);
	tcm_bn_bn2bin(b, g_uNumbits/8, pstr_b);

	//�õ�G�����ַ���
	tcm_bn_bn2bin(xG, g_uNumbits/8, pstr_xG);
	tcm_bn_bn2bin(yG, g_uNumbits/8, pstr_yG);

	//
	{
		//const u_short userIDBitsLen = userIDLen*8;
		unsigned short userIDBitsLen = userIDLen*8;
		*((unsigned short*)ENTL) = htons(userIDBitsLen);
	}

	//����ZA������
	// ENTL||ID||a||b||xG||yG||xA||yA
	uZlen = 2	// sizeof(ENTL)
		 + userIDLen	// ID�ĳ���
		 + 6 * g_uNumbits/8;	// a||b||xG||yG||xA||yA

	pstr_Z = (BYTE*)malloc(uZlen);
	if( pstr_Z == NULL )
	{
		iret = 1;
		goto end;
	}
	iPos = 0;
	// 
	memcpy(pstr_Z, ENTL, 2);
	iPos += 2;
	//
	if( userIDLen != 0 )
	{
		memcpy(pstr_Z+iPos, userID, userIDLen);
		iPos += userIDLen;
	}
	//
	memcpy(pstr_Z+iPos, pstr_a, g_uNumbits/8);
	iPos += g_uNumbits/8;
	//
	memcpy(pstr_Z+iPos, pstr_b, g_uNumbits/8);
	iPos += g_uNumbits/8;
	//
	memcpy(pstr_Z+iPos, pstr_xG, g_uNumbits/8);
	iPos += g_uNumbits/8;
	//
	memcpy(pstr_Z+iPos, pstr_yG, g_uNumbits/8);
	iPos += g_uNumbits/8;
	//
	memcpy(pstr_Z+iPos, &pPubkey_in[1], 2*g_uNumbits/8);
	iPos += 2*g_uNumbits/8;
	//
#ifdef TEST
{
	char *str;
	BN_bin2bn(pstr_Z,uZlen,a);
	str = BN_bn2hex(a);
	printf("ENTL||ID||a||b||x||y||x||y: %s\n",str);
	OPENSSL_free(str);
}
#endif
	

	//����ZA
	iret = tcm_sch_256( uZlen, pstr_Z, digest);
	//
	free(pstr_Z);
	pstr_Z=NULL;

	//
	iret = 0;
end:
	BN_free(a);
	BN_free(b);
	BN_free(xG);
	BN_free(yG);
	BN_free(zG);
	BN_free(p);
	//
	free(pstr_a);
	free(pstr_b);
	free(pstr_xG);
	free(pstr_yG);
	//
	return iret;
}

/* ������Ϣ��hash */
int tcm_get_message_hash(unsigned char *msg, unsigned int msgLen,  
					   unsigned char *userID, unsigned short int userIDLen, 
					   unsigned char *pPubkey_in, unsigned int pubkeyLen_in,
					   unsigned char *pDigest,
					   unsigned int *puDigestLen)
{
	// ����ֵ
	int iret;
	//
	unsigned char schsum_Z[HASH_NUMBITS/8];	// ����Za������ʹ��256hash�㷨
	//
	BYTE *pstr_M=NULL;
	uint32 uMlen = 0;
	//
	iret = tcm_get_usrinfo_value(userID, userIDLen, 
					   pPubkey_in, pubkeyLen_in,
					   schsum_Z);
	if( iret == 1 )
	{
		return 1;
	}

	//
	//����M������
	// ZA||M
	uMlen = HASH_NUMBITS/8+msgLen;
	pstr_M = (BYTE*)malloc(uMlen);
	if( pstr_M == NULL )
	{
		return 1;
	}
	// 
	memcpy(pstr_M, schsum_Z, sizeof(schsum_Z));
	//
	if( msgLen != 0 )
	{
		memcpy(pstr_M+sizeof(schsum_Z), msg, msgLen);
	}
	//

	//����M
	if( g_uSCH_Numbits == 256 )
	{
		iret = tcm_sch_256( uMlen, pstr_M, pDigest);
		//
		free(pstr_M);
		pstr_M=NULL;
	}
	else if( g_uSCH_Numbits == 192 )
	{
		iret = tcm_sch_192( uMlen, pstr_M, pDigest);
		//
		free(pstr_M);
		pstr_M=NULL;
	}
	else
	{
		free(pstr_M);
		pstr_M=NULL;
		//
		return 1;
	}
	//
	*puDigestLen = g_uSCH_Numbits/8;
	//
	return 0;
}
