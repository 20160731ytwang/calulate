/******************************************
File	:tcm_ecc.c 
Author	:linyang
Date	:11/21/2006
******************************************/

#include "openssl/bn.h"
#include "openssl/bnEx.h"
#include "openssl/ec_operations.h"
#include "openssl/crypto.h"
#include "tcm_ecc.h"
#include "tcm_bn.h"
#include <malloc.h>
#include <string.h>

/*
����EC�ļ����㷨
���룺����pPlaintext_in�����ĳ���plaintextLen_in
      �Է���ԿpPubkey_in, �Է���Կ����pubkeyLen_in
      sta��mac��ַpstamac
����������pCipher_out�����ĳ���pCipherLen_out
���أ����ܳɹ�����0�����򷵻�1
������
*/
int tcm_ecc_encrypt(unsigned char *pPlaintext_in, unsigned int plaintextLen_in, unsigned char *pPubkey_in, unsigned int pubkeyLen_in, unsigned char *pCipher_out, unsigned int *pCipherLen_out)
{
//���ص����ĳ���
#define CIPHER_LEN (1+2*g_uNumbits/8 + plaintextLen_in+ HASH_NUMBITS/8)

	unsigned int i;
	unsigned char*	pstr_r=NULL;
	unsigned char*	pstr_s=NULL;

	
	BIGNUM	*x, *y, *one;
	EC_POINT	*P;


	/* ���������Ƿ���Ч */
	if( (pPlaintext_in==NULL) || (plaintextLen_in <= 0) )
	{
		return 1;
	}

	/*�������Ŀռ��Ƿ���Ч*/
	if( pCipher_out==NULL )
	{
		return 1;
	}
	if( *pCipherLen_out < CIPHER_LEN )
	{
		return 1;
	}


	/* ���鹫ԿpubkeyLen_in���� */
	if (pubkeyLen_in != PUBKEY_LEN)
	{
		//uiPrintf("****pubkeyLen_in\n");
		return 1;
	}
	/* ���鹫ԿpPubkey_in�Ƿ�Ϊ�� */
	if (pPubkey_in ==NULL)
	{
		//uiPrintf("*****pPubkey_in ==NULL\n");
		return 1;
	}
	//�������Ƿ�ѹ����ʽ������ʧ��
	if( pPubkey_in[0] != 0x04 )
	{
		return 1;
	}

	//Ϊǩ�������ڴ�
	pstr_r=(unsigned char*)malloc(g_uNumbits/8);
	pstr_s=(unsigned char*)malloc(g_uNumbits/8);
	//
	x = BN_new();
	y = BN_new();
	one = BN_new();
	P = EC_POINT_new();

	if ( pstr_r == NULL || pstr_s == NULL ||
		x == NULL || y == NULL || one == NULL || P == NULL)
	{
		return 1;
	}

	/* ����Կ�ַ���(48�ֽ�)�ֳ������(24, 24)�ֽ� */
	for (i = 0; i < (g_uNumbits/8); i++) {
		pstr_r[i] = pPubkey_in[1+i];
		pstr_s[i] = pPubkey_in[1+g_uNumbits/8 + i];
	}

	/* �������(g_uNumbits/8, g_uNumbits/8)�ֽڴ�ת��Ϊ���� */
	BN_bin2bn(pstr_r, g_uNumbits/8, x);
	BN_bin2bn(pstr_s, g_uNumbits/8, y);

	BN_hex2bn(&one, "1");
	
	/* ���ɹ�Կ��P */
	EC_POINT_set_point(P, x, y, one);
	if (!(ECC_Encrypt(pCipher_out, group, G, P, pPlaintext_in, plaintextLen_in)))
	{
		/* ���ĳ��� */
		*pCipherLen_out = CIPHER_LEN;
#ifdef TEST_FIXED
	{
		//����Ϊ�̶�19���ֽ�
		*pCipherLen_out = (1+2*g_uNumbits/8 + 19+ HASH_NUMBITS/8);
	}
#endif


		//�ͷ��ڴ�
		free(pstr_r);
		free(pstr_s);
		//
		BN_free(x);
		BN_free(y);
		BN_free(one);
		EC_POINT_free(P);
		//		
		return 0;
	}
	else
	{
		//�ͷ��ڴ�
		free(pstr_r);
		free(pstr_s);
		//
		BN_free(x);
		BN_free(y);
		BN_free(one);
		EC_POINT_free(P);
		//		
		return 1;
	}
}

/*
����EC�Ľ����㷨
���룺����pCipher_in�����ĳ���cipherLen_in
      ˽ԿpPrikey_in , ˽Կ����prikeyLen_in
����������pPlaintext_out�����ĳ���pPlaintextLen_out
���أ����ܳɹ�����0�����򷵻�1
������ 
*/
int tcm_ecc_decrypt(unsigned char *pCipher_in, unsigned int cipherLen_in, unsigned char *pPrikey_in, unsigned int prikeyLen_in, unsigned char *pPlaintext_out, unsigned int *pPlaintextLen_out)
{

//���ص����ĳ���
#define PLAIN_LEN (cipherLen_in - (1+2*g_uNumbits/8 + HASH_NUMBITS/8) )

	BIGNUM *skey;
	//��������ָ���Ƿ���Ч
	if( pPlaintext_out == NULL )
	{
		return 1;
	}
	//�������ĳ����Ƿ��ϸ�
	if( *pPlaintextLen_out < PLAIN_LEN )
	{
		return 1;
	}
	//
	/* ���������Ƿ�Ϊ�� */
	if (pCipher_in == NULL)
	{
		return 1;
	}

	//����ֻ������ѹ����ʽ
	if (pCipher_in[0] != 04)
	{
		return 1;
	}


	/* ��������cipherLen_in���� */
	/* �����պõ���1+2*g_uNumbits/8 + HASH_NUMBITS/8���������� */
	if( cipherLen_in< (1+2*g_uNumbits/8 + HASH_NUMBITS/8) )
	{
		return 1;
	}


	/* ����˽Կ�Ƿ�Ϊ�� */
	if (pPrikey_in == NULL)
	{
		return 1;
	}
	/* ����˽ԿprikeyLen_in���� */
	if (prikeyLen_in != g_uNumbits/8)
	{
		return 1;
	}

	skey = BN_new();

	if (skey == NULL)
	{
      	return 1;
	}

	/* ת��˽Կ�ַ���Ϊ���� */
	BN_bin2bn(pPrikey_in, g_uNumbits/8, skey);
	if (!(ECC_Decrypt(pPlaintext_out, group, pCipher_in, cipherLen_in, skey)))
	{
		*pPlaintextLen_out = PLAIN_LEN;
		BN_free(skey);
		return 0;
	}
	else
	{
		*pPlaintextLen_out = 0;
		BN_free(skey);
		return 1;
	}
}

/*
����EC��Բ���ߵ�����ǩ���㷨
���룺��ǩ��ժҪֵpDigest����ǩ��ժҪֵ����uDigestLen
      ˽ԿpPrikey_in��˽Կ����prikeyLen_in
      sta��mac��ַpstamac
������������ǩ������sigData
      ǩ�������ݳ���puSigDataLen
���أ�ǩ���ɹ�����0�����򷵻�1
������
*/
int tcm_ecc_signature(	   unsigned char *pDigest, unsigned int uDigestLen,
						   unsigned char *pPrikey_in, unsigned int prikeyLen_in, 
						   /*out*/unsigned char *pSigData,
						   /*out*/unsigned int *puSigDataLen
						   )
{
	BIGNUM *skey;

	if( pSigData == NULL )
	{
		return 1;
	}
	//ǩ�����Ȳ���������ʧ��
	if( *puSigDataLen < 2*g_uSCH_Numbits/8 )
	{
		*puSigDataLen = 2*g_uSCH_Numbits/8;
		return 1;
	}

	//����������hashֵָ��ָ��NULL�����ش���
	if( pDigest==NULL )
	{
		return 1;

	}
	//������Ҫǩ����hashֵ���Ȳ�һ�£����ش�����
	if( uDigestLen != g_uSCH_Numbits/8 )
	{
		return 1;
	}

	/* ����˽Կ�Ƿ�Ϊ�� */
	if (pPrikey_in == NULL)
	{
		return 1;
	}

	/* ����˽ԿprikeyLen_in���� */
	if (prikeyLen_in != g_uNumbits/8)
	{
		return 1;
	}

	//

	skey = BN_new();

	if ( skey == NULL)
	{
		return 1;
	}

	/* ת��˽Կ�ַ���Ϊ���� */
	BN_bin2bn(pPrikey_in, g_uNumbits/8, skey);
	
	if (!(ECC_Signature(pSigData, group, G, skey, pDigest)))
	{
		*puSigDataLen = 2*g_uSCH_Numbits/8;
		BN_free(skey);
		return 0;
	}
	else
	{
		*puSigDataLen = 0;
		BN_free(skey);
		return 1;
	}
}

/*
����EC��Բ���ߵ�����ǩ����֤�㷨
���룺��������pData_in���������ݳ���dataLen_in
      ǩ������pSigndata_in, ǩ�����ݳ���signdataLen_in
      ��ԿpPubkey_in����Կ����pubkeyLen_in
�������� 
���أ���֤�㷨�ɹ�����0�����򷵻�1
������
*/
int tcm_ecc_verify(unsigned char *pDigest, unsigned int uDigestLen, unsigned char *pSigndata_in, unsigned int signdataLen_in, unsigned char *pPubkey_in, unsigned int pubkeyLen_in)
{
	unsigned int i;
	unsigned char*	pstr_r = NULL;
	unsigned char*	pstr_s = NULL;
	
	BIGNUM	*x, *y, *one;
	EC_POINT	*P;
	
	//����������hashֵָ��ָ��NULL�����ش���
	if( pDigest==NULL )
	{
		return 1;

	}
	//������Ҫ��֤��hashֵ���Ȳ�һ�£����ش�����
	if( uDigestLen != g_uSCH_Numbits/8 )
	{
		return 1;
	}
	//


	/* ����ǩ�������Ƿ�Ϊ�� */
	if (pSigndata_in == NULL)
	{
		 //uiPrintf("*****pSigndata_in == NULL\n");
		return 1;
	}

	/* ����ǩ������signdataLen_in�ĳ��� */
	if (signdataLen_in != 2 * (g_uNumbits/8))
	{
		 //uiPrintf("*****signdataLen_in:%d\n",signdataLen_in);
		return 1;
	}


	/* ���鹫ԿpubkeyLen_in���� */
	if (pubkeyLen_in != PUBKEY_LEN)
	{
		//uiPrintf("****pubkeyLen_in\n");
		return 1;
	}
	/* ���鹫ԿpPubkey_in�Ƿ�Ϊ�� */
	if (pPubkey_in ==NULL)
	{
		//uiPrintf("*****pPubkey_in ==NULL\n");
		return 1;
	}
	//�������Ƿ�ѹ����ʽ������ʧ��
	if( pPubkey_in[0] != 0x04 )
	{
		return 1;
	}


	x = BN_new();
	y = BN_new();
	one = BN_new();
	P = EC_POINT_new();
	//
	pstr_r = (unsigned char*)malloc(g_uNumbits/8);
	pstr_s = (unsigned char*)malloc(g_uNumbits/8);

	if ( x == NULL || y == NULL || one == NULL || P == NULL ||
		pstr_r == NULL || pstr_s == NULL  )

	{
		return 1;
	}
 

	/* ����Կ�ַ���(48�ֽ�)�ֳ������(g_uNumbits/8, g_uNumbits/8)�ֽ� */
	for (i = 0; i < (g_uNumbits/8); i++) 
	{
		pstr_r[i] = pPubkey_in[1+i];
		pstr_s[i] = pPubkey_in[1+g_uNumbits/8 + i];
	}

	/* �������(g_uNumbits/8, g_uNumbits/8)�ֽڴ�ת��Ϊ���� */
	BN_bin2bn(pstr_r, g_uNumbits/8, x);
	BN_bin2bn(pstr_s, g_uNumbits/8, y);

	BN_hex2bn(&one, "1");
 
   
	/* ���ɹ�Կ��P */
	EC_POINT_set_point(P, x, y, one);
	if (!(ECC_Verify(group, G, P, pDigest, pSigndata_in)))
	{
		BN_free(x);
		BN_free(y);
		BN_free(one);
		EC_POINT_free(P);
		//
		free(pstr_r);
		free(pstr_s);
		//
		return 0;
	}
	else
	{
		BN_free(x);
		BN_free(y);
		BN_free(one);
		EC_POINT_free(P);
		//
		free(pstr_r);
		free(pstr_s);
		//
		return 1;
	}
  
}


/*
�ж��Ƿ�����
���룺���ش������ֽڳ���
�������� 
���أ���������1��ż������0��
	�������󷵻�-1
������
*/
int tcm_ecc_string_is_odd(unsigned char *string,  unsigned int len)
{
	int iret;
	BIGNUM	*x;
	//
	x = BN_new();
	if( x == NULL )
	{
		return -1;
	}
	BN_bin2bn(string, len, x);
	iret = BN_is_odd(x);
	BN_free(x);
	return iret;
}

/*
����x���ش�����y���ش�
���룺x���ش���y���ش������ֽڳ��ȣ�ѹ��ģʽ
�������� 
���أ����أ��ɹ�����0�����򷵻�1
������
*/
int tcm_ecc_x_to_y(unsigned char *xstr, unsigned char *ystr, unsigned int len,
				   unsigned int form)
{

	// y^2=x^3+ax+b
	BIGNUM *x, *y, *z, *a, *b;
	BIGNUM *tmp, *left, *right;
	BN_CTX *ctx;
	const BIGNUM *p=&(group->p);
	int y_bit;
	int iret;

	//
	x=BN_new();
	y=BN_new();
	z=BN_new();
	a=BN_new();
	b=BN_new();
	//
	tmp=BN_new();
	left=BN_new();
	right=BN_new();
	//
	ctx = BN_CTX_new();

	//
	BN_copy(a,&(group->a));
	BN_copy(b,&(group->b));
	//

	
	if ( x == NULL || y == NULL || a == NULL || b == NULL || 
		tmp == NULL || left == NULL || right == NULL ||		
		ctx == NULL )
	{
		return 1;
	}

	BN_bin2bn(xstr, len, x);

	if( form == 02 || form == 06 )
	{
		y_bit = 0;
	}
	else if( form == 03 || form == 07 )
	{
		y_bit = 1;
	}
	else
	{
		iret = 1;
		goto end;
	}

//	BN_nnmod(x,x,p,ctx);
	// tmp := x^3 
	BN_mod_sqr(tmp, x, p, ctx);
	BN_mod_mul(tmp, tmp, x, p, ctx);

	
	//
	BN_copy(right, tmp);

	// tmp := ax
	BN_mod_mul(tmp,a,x,p,ctx);

	// x^3+ax+b
	BN_mod_add(right, right, tmp, p, ctx);
	BN_mod_add(right, right, b, p, ctx);
	// �������������ұ�


	//����y
	if( !BN_mod_sqrt(y, right, p, ctx) )
	{
		//û��ƽ���������ش���
		iret = 1;
		goto end;
	}


	if (y_bit != BN_is_odd(y))
	{
		if (BN_is_zero(y))
		{
			iret = 1;
			goto end;
		}
		if (!BN_usub(y, p, y))
		{
			iret = 1;
			goto end;
		}
		if (y_bit != BN_is_odd(y))
		{
			iret = 1;
			goto end;
		}
	}


	tcm_bn_bn2bin(y, len, ystr);

	iret = 0;
end:
	//
	BN_free(x);
	BN_free(y);
	BN_free(z);
	BN_free(a);
	BN_free(b);
	//
	BN_free(tmp);
	BN_free(left);
	BN_free(right);
	//
	BN_CTX_free(ctx);

	return iret;

}

/*
 ����EC��Բ���ߵ��жϱ��ش��Ƿ���ʾ�����ϵ�һ���㣬
  �����ǣ�����TRUE
  ���򷵻�FALSE
  �������ش������Ƿ�ѹ����ʽ������ʹ��tcm_ecc_point_to_uncompressed�����õ���ѹ����ʽ�Ĵ�
*/
BOOL tcm_ecc_is_point_valid(unsigned char *pPoint, unsigned int pointLen)
{
#define	UNCOMP_LEN		(1 + 2*g_uNumbits/8)

	unsigned char *pstr_x = NULL;
	unsigned char *pstr_y = NULL;

//nt iret;
	BOOL bret;

	if( pointLen != UNCOMP_LEN )
	{
		return 0;
	}

	//Ϊx,y���������ڴ�
	if( (pstr_x = (unsigned char*)malloc(g_uNumbits/8)) == NULL )
	{
		return FALSE;
	}
	if( (pstr_y = (unsigned char*)malloc(g_uNumbits/8)) == NULL )
	{
		free(pstr_x);
		return FALSE;
	}

	//�ж��Ƿ���ѹ����ʽ
	if( pPoint[0]!= 04 )
	{
		free(pstr_x);
		free(pstr_y);
		return FALSE;
	}

	//
	memcpy( pstr_x, &pPoint[1], g_uNumbits/8 );
	memcpy( pstr_y, &pPoint[1+g_uNumbits/8], g_uNumbits/8 );
	

	//�����жϵ��Ƿ���������
	{
		BIGNUM	*x, *y, *z;
		EC_POINT *P;

		x = BN_new();
		y = BN_new();
		z = BN_new();
		P = EC_POINT_new();
		//

		BN_bin2bn(pstr_x, g_uNumbits/8, x);
		BN_bin2bn(pstr_y, g_uNumbits/8, y);
		BN_hex2bn(&z, "1");

		//
		EC_POINT_set_point(P, x, y, z);
		/* ����C1�Ƿ��������߷��� */
		bret = EC_POINT_is_on_curve(group, P);
		//		
		BN_free(x);
		BN_free(y);
		BN_free(z);
		EC_POINT_free(P);
	}

	//�ͷŷ������ڴ�
	free(pstr_x);
	free(pstr_y);

	//
	return bret;
}


/*
����EC��Բ���ߵ��жϱ��ش��Ƿ���ʾ�����ϵ�һ����
���룺�����ֽڴ�pData_in���ֽڴ�����dataLen_in

�������� 
���أ��������ϵ�һ���㷵��0�����򷵻�1��
	�����������ϵĵ㣬���ص��ķ�ѹ����ʽ��
�������ɶ�0��������Ч����
*/
BOOL tcm_ecc_point_to_uncompressed(unsigned char *pPoint, unsigned int pubkeyLen_in,
								unsigned char *string, unsigned int *puStringLen)
{
#define ZERO_LEN		1
#define	COMP_LEN		(1 + g_uNumbits/8)
#define	UNCOMP_LEN		(1 + 2*g_uNumbits/8)

	unsigned char *pstr_x = NULL;
	unsigned char *pstr_y = NULL;

	BOOL bret;


	if( (pubkeyLen_in != ZERO_LEN) && (pubkeyLen_in != COMP_LEN) && (pubkeyLen_in != UNCOMP_LEN) )
	{
		return FALSE;
	}

	if( (string == NULL )  || *puStringLen < UNCOMP_LEN )
	{
		return FALSE;
	}

	//Ϊx,y���������ڴ�
	if( (pstr_x = (unsigned char*)malloc(g_uNumbits/8)) == NULL )
	{
		return FALSE;
	}
	if( (pstr_y = (unsigned char*)malloc(g_uNumbits/8)) == NULL )
	{
		free(pstr_x);
		return FALSE;
	}


	switch(pPoint[0])
	{
		case 00:
			{
				//0�㲻��Ҫ����x,y
				//�ͷŷ������ڴ�
				free(pstr_x);
				free(pstr_y);
				//
				if( pubkeyLen_in != ZERO_LEN )
				{
					return FALSE;
				}
				//0�㷵��0
				string[0] = 0;
				*puStringLen = 1;
				return TRUE;
			}


		case 02:
		case 03:
			{
				if( pubkeyLen_in != COMP_LEN )
				{
					//�ͷŷ������ڴ�
					free(pstr_x);
					free(pstr_y);
					//
					return FALSE;
				}
				memcpy(pstr_x, &pPoint[1], g_uNumbits/8 );
				if( tcm_ecc_x_to_y(pstr_x, pstr_y, g_uNumbits/8, pPoint[0]) == 1 )
				{
					//�ͷŷ������ڴ�
					free(pstr_x);
					free(pstr_y);
					//
					return FALSE;
				}
				break;
			}


		case 06:	//����ѹ����ʽ��Y����Ϊż��
		case 07:	//����ѹ����ʽ��Y����Ϊ����
			{
				//
				if( (pPoint[0] == 06) && (tcm_ecc_string_is_odd(pPoint, 1+2*g_uNumbits/8) != 0) )
				{
					//�ͷŷ������ڴ�
					free(pstr_x);
					free(pstr_y);
					//
					return FALSE;
				}
				//
				if( (pPoint[0] == 07) &&  (tcm_ecc_string_is_odd(pPoint, 1+2*g_uNumbits/8) != 1) )
				{
					//�ͷŷ������ڴ�
					free(pstr_x);
					free(pstr_y);
					//
					return FALSE;
				}
			}
		case 04:	//��ѹ��ģʽ
			{
				if( pubkeyLen_in != UNCOMP_LEN )
				{
					//�ͷŷ������ڴ�
					free(pstr_x);
					free(pstr_y);
					//
					return FALSE;
				}
				memcpy(pstr_x, &pPoint[1], g_uNumbits/8);
				memcpy(pstr_y, &pPoint[1+g_uNumbits/8], g_uNumbits/8);
				break;
			}
		default:	//��֧������ģʽ
			{
				//�ͷŷ������ڴ�
				free(pstr_x);
				free(pstr_y);
				//
				return FALSE;
			}
	}

	

	//�����жϵ��Ƿ���������
	{
		BIGNUM	*x, *y, *z;
		EC_POINT *P;

		x = BN_new();
		y = BN_new();
		z = BN_new();
		P = EC_POINT_new();
		//

		BN_bin2bn(pstr_x, g_uNumbits/8, x);
		BN_bin2bn(pstr_y, g_uNumbits/8, y);
		BN_hex2bn(&z, "1");

		//
		EC_POINT_set_point(P, x, y, z);
		/* ����C1�Ƿ��������߷��� */
		bret = EC_POINT_is_on_curve(group, P);
		if( bret == TRUE )
		{
			//����Ϊδѹ����ʽ
			string[0] = (unsigned char)04;
			//����x��y
			memcpy(&string[1], pstr_x, g_uNumbits/8);
			memcpy(&string[1+g_uNumbits/8], pstr_y, g_uNumbits/8);
		}
		BN_free(x);
		BN_free(y);
		BN_free(z);
		EC_POINT_free(P);
	}

	//�ͷŷ������ڴ�
	free(pstr_x);
	free(pstr_y);

	//
	return bret;
}
