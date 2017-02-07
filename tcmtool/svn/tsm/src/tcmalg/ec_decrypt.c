/******************************************
File	:ec_decrypt.c 
Author	:linyang
Date	:11/21/2006
comment :�����㷨
******************************************/

#include <string.h>
#include <malloc.h>
#include "openssl/bn.h"
#include "openssl/bnEx.h"
#include "openssl/crypto.h"
#include "openssl/ec_operations.h"
#include "tcm_bn.h"
#include "debug.h"
#include "tcm_hash.h"

/*
����EC�Ľ����㷨
���ܣ�ʵ��B���յ��ļ�����Ϣ{(x1, y1), cipher}����
�����������ܺ�����Ϣmsg
���ܺ�����Ϣ���ȸ������ĳ��ȶ�̬����
���룺group�����p, a, b, N
���룺B��˽Կkb
���룺������Ϣ{(x1, y1), cipher}
*/
int ECC_Decrypt(unsigned char *msg,const EC_GROUP *group,unsigned char *cipher, unsigned int cipherLen, const BIGNUM *kb)
{
	BIGNUM *x1,*y1,*one;
	BIGNUM *x2,*y2,*z2,*m,*c;
	BIGNUM *u;
	BIGNUM *h, *h1;
	BIGNUM *N;
	EC_POINT *C1,*S;

	//���ĳ���
	const int klen= (cipherLen - (1+2*g_uNumbits/8 + HASH_NUMBITS/8) );
	// pstr_h�ĳ��ȣ����涨��
	const int hLen = g_uNumbits/8 + klen + g_uNumbits/8;		// x2||M||y2
	int iret;

	//
	unsigned char* pstr_x1;
	unsigned char* pstr_y1;
	unsigned char* pstr_c;
	//
	unsigned char* pstr_xy;
	//
	unsigned char mac_in[HASH_NUMBITS/8];	//������hash
	unsigned char mac_u[HASH_NUMBITS/8];	//�������õ���
	//
	unsigned char* pstr_t=NULL;	//Ϊx2||y2�������ڴ�
	unsigned char* pstr_h=NULL;	//���ڼ���Hash(x2||M||y2)

	//
	int i;
	
	BN_CTX *ctx= BN_CTX_new();
	//

	/* ���������Ƿ�Ϊ�� */
	if (cipher == NULL)
	{
		return 1;
	}

	/* ����˽Կ�Ƿ�Ϊ�� */
	if (kb == NULL)
	{
		return 1;
	}

	x1=BN_new();
	y1=BN_new();
	x2=BN_new();
	y2=BN_new();
	z2=BN_new();
	one=BN_new();
	m=BN_new();
	c=BN_new();
	u=BN_new();
	//
	h=BN_new();
	h1=BN_new();
	//
	N=BN_new();
	//

	C1=EC_POINT_new();
	S=EC_POINT_new();
	//

	pstr_x1 = (unsigned char*)malloc(g_uNumbits/8);
	pstr_y1 = (unsigned char*)malloc(g_uNumbits/8);
	pstr_c = (unsigned char*)malloc(klen);
	//
	pstr_xy = (unsigned char*)malloc(2*(g_uNumbits/8));
	//
	//�����ռ䣬׼������
	pstr_t = (unsigned char *)malloc(klen);
	// x2||M||y2
	//�����ռ䣬׼������hash����
	pstr_h = (unsigned char *)malloc(hLen);

	//
	if ( ctx == NULL ||
		 x1 == NULL || y1 == NULL || x2 == NULL || y2 == NULL ||
		 z2 == NULL || one == NULL || m == NULL || c == NULL ||
		 C1 == NULL || S == NULL || u == NULL ||
		 h == NULL || h1 == NULL || N == NULL ||
		 pstr_x1 == NULL || pstr_y1 == NULL || pstr_c == NULL ||
		 pstr_t == NULL || pstr_h == NULL  )
	{
		return 1;
	}

	EC_GROUP_get_order(group,N);	/* �� */
//	BN_copy(p,&group->p); /* ������ */

	/* B1 */
	/* step1:��cipher����ȡx1,y1 */
	memcpy(pstr_x1,cipher+1,g_uNumbits/8);
	memcpy(pstr_y1,cipher+1+g_uNumbits/8,g_uNumbits/8);
	/* ��ȡ���� */
	memcpy(pstr_c,cipher+1+g_uNumbits/8+g_uNumbits/8,klen);
	/* ��ȡmac */
	memcpy(mac_in,cipher+1+g_uNumbits/8+g_uNumbits/8+klen,sizeof(mac_in));


	BN_bin2bn(pstr_x1,g_uNumbits/8,x1);
	BN_bin2bn(pstr_y1,g_uNumbits/8,y1);
	BN_bin2bn(pstr_c,g_uNumbits/8,c);

	BN_hex2bn(&one,"1");
	EC_POINT_set_point(C1,x1,y1,one);

	/* ����C1�Ƿ��������߷��� */
	if( EC_POINT_is_on_curve(group, C1) == FALSE )
	{
		iret = 1;
		goto end;
	}

	/* B2 */
	/*******************************/
	/* �õ������� */
	EC_GROUP_get_cofactor(group, h);

	/* �õ������ӵ��� */
	BN_mod_inverse(h1, h, N, ctx);
	BN_nnmod(h1,h1,N,ctx);

#ifdef TEST
{
	//���� h1�Ƿ���h����
	char *str;

	str = BN_bn2hex(h);
	printf("h: %s\n", str);
	OPENSSL_free(str);

	BN_mod_mul(z2,h,h1,N,ctx);

	str = BN_bn2hex(z2);
	printf("h1*h mod n: %s\n",str);
	OPENSSL_free(str);
}
#endif

	/* ���� [h]C1 */
	EC_POINT_mul(group,S,h,C1);

	/* ����[Db*h^(-1)] */
	/* ��ʱʹ��z2 */
	BN_mul(z2,kb,h1,ctx);

	/* ���� [Db*h^(-1)][h]C1 */
	EC_POINT_mul(group,S,z2,S);
	EC_POINT_affine2gem(group,S,S);
	/*******************************/
	/* �Ż��㷨������hΪ1������ */
	/* S=[Db*h^-1][h]C1=(x2,y2) */
	/* step2: */
//	EC_POINT_mul(group,S,kb,C1);
//	EC_POINT_affine2gem(group,S,S);
	/*******************************/
	/* ����������Զ�㣬����ʧ�� */
	if( EC_POINT_is_at_infinity(group, S) )
	{
		iret = 1;
		goto end;
	}
	EC_POINT_get_point(S,x2,y2,z2);
#ifdef TEST
	{
		char *str;
		str = BN_bn2hex(x2);
		printf("x2: %s\n",str);
		OPENSSL_free(str);

		str = BN_bn2hex(y2);
		printf("y2: %s\n",str);
		OPENSSL_free(str);
	}
#endif

	/* B3 */
	/* ���� t=KDF(x2||y2, klen) */	
	
	/* X2||Y2 */
	tcm_bn_bn2bin(x2, g_uNumbits/8, &pstr_xy[0]);
	tcm_bn_bn2bin(y2, g_uNumbits/8, &pstr_xy[(g_uNumbits/8)]);
	iret = tcm_kdf(pstr_t, klen, pstr_xy, 2*(g_uNumbits/8));
	//
#ifdef TEST_FIXED
{
	if( g_uNumbits == 256 )
	{
		const unsigned char tArray[] = { 0xCA, 0xD8, 0xBA, 0xB1, 0x11, 0x21, 0xB6, 
					0x1C, 0x4E, 0x98, 0x2C, 0xD7, 0xFC, 0x25, 0xC1,
					0x4F, 0x67, 0xEC, 0x79};

		ASSERT( klen == sizeof(tArray) ); 
		memcpy(pstr_t, tArray, sizeof(tArray));
	}
	else
	{
		const unsigned char tArray[] = { 0x6B, 0x8F, 0x54, 0xC0, 0x34, 0x69, 0x9C,
					0x61, 0x09, 0x7F, 0xA4, 0xEF, 0xBB, 0x53, 0x19, 
					0xE9, 0x5E, 0xD4, 0x60};
		ASSERT( klen == sizeof(tArray) ); 
		memcpy(pstr_t, tArray, sizeof(tArray));
	}
}
#endif

	BN_bin2bn(pstr_t, klen, z2);
	/* ����tΪȫ0���ش������󷵻� */
	if( BN_is_zero(z2) )
	{

#ifdef TEST
		printf("t is zeor\n");
#endif
		iret = 1;
		goto end;
	}
#ifdef TEST
	{
		char *str;
		str = BN_bn2hex(z2);
		printf("t: %s\n",str);
		OPENSSL_free(str);
	}
#endif

	/* B4 */
	/* ������������ M=C2^t */
	for(i=0;i<klen;i++)
	{
		pstr_c[i] ^= (unsigned char)pstr_t[i];
	}
	BN_bin2bn(pstr_c, klen, m);

#ifdef TEST
	{
		char *str;
		str = BN_bn2hex(m);
		printf("M: %s\n",str);
		OPENSSL_free(str);
	}
#endif
	

	/* B5 */
	/* ����u = Hash(x2||M||y2) */
	BN_copy(u,x2);
	BN_lshift(u,u, klen*8);
	//
	BN_add(u,u,m);
	BN_lshift(u,u, g_uNumbits);
	//
	BN_add(u,u,y2);

	//
	tcm_bn_bn2bin(u, hLen, pstr_h);

#ifdef TEST
	{
		char *str;
		str = BN_bn2hex(u);
		printf("x2||M||y2: %s\n",str);
		OPENSSL_free(str);
	}
#endif

	tcm_sch_hash(hLen, pstr_h, mac_u);
	//

#ifdef TEST_FIXED
{
	if( g_uNumbits == 256 )
	{
		BN_hex2bn(&u, "E1C8D1101EDE0D3430ACCDA0C9E45901BAA902BD44B03466930840210766195C");
		tcm_bn_bn2bin(u, mac_u);
	}
	else
	{
		//for Fp-192
		BN_hex2bn(&u, "8DCF8E4DC8C92FCBA5A2DDCE0A3BED07588A6A634AAA09216D098954FDBD6A51");
		tcm_bn_bn2bin(u, mac_u);
	}
}
#endif

	//�Ƚ�hash��������ͬ������ʧ��
	for( i=0; i<(int)(sizeof(mac_u));i++)
	{
		if( mac_in[i]!=mac_u[i] )
		{

			iret = 1;
#ifdef TEST
		printf("macУ��ʧ��\n");
#endif

			goto end;
		}
	}
		

	//��������
	memcpy(msg,pstr_c,klen);

	iret = 0;
end:
	BN_free(x1);
	BN_free(y1);
	BN_free(one);
	BN_free(x2);
	BN_free(y2);
	BN_free(z2);

	BN_free(m);
    BN_free(c);

    BN_free(u);

	//
	BN_free(h);
	BN_free(h1);
	BN_free(N);
	//


	EC_POINT_free(C1);
	EC_POINT_free(S);
	//
	free(pstr_x1);
	free(pstr_y1);
	free(pstr_c);
	//
	free(pstr_xy);
	free(pstr_t);

	return iret;
}