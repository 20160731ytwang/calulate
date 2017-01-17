
#ifndef ___DESTRING_H___
#define ___DESTRING_H___

#ifdef WIN32
#include "windows.h"
#else
#include "stdafx.h"
#endif

class CTString
{
public:
	CTString();
	virtual ~CTString();
	bool SetString( char * pszData );
	//处理不可见字符,用户尽量不调用此函数
	bool SetString( char * pszData, int nDataLen );

	char * GetString();
	int GetLength();

	CTString& operator =( CTString &pstrData );
	CTString& operator =( char *pszData );
	CTString& operator =( char cData );
	
	bool operator ==( char *pszData );
	bool operator ==( CTString &pstrData );
	CTString& operator +=( CTString &pstrData );
	CTString& operator +=( char * pszData );
	CTString& operator +=( char cData );

	operator char * (){ return m_pszData;};
	char operator [] (int iIndex);
	void Empty();

	void IntToString( int i );
	void FloatToString( float f );
	void DwordToString( DWORD dw );
	int FindSubString( int iBeginIndex, char * pszSubString );
	bool GetSubString( int iBeginIndex, int iLength, CTString& cSubString );
	bool GetSubString( int iBeginIndex, CTString& cSubString );
	char * GetBuffer(int iBuffLen);
	bool IsEmpty();

	//大写转小写
	void Lcase();
	//小写转大写 
	void Ucase(); 


protected:

	bool NewBuffer( int iLength );
	void FreeData();
	void LinkStr( char * pszData, int iLength );

protected:

	char * m_pszData;
	int m_iLength;
};

#endif


