
#include "string.h"
#include "stdio.h"
#include "TString.h"

CTString::CTString()
{
	m_iLength = -1;
	m_pszData = NULL;
}

CTString::~CTString()
{
	FreeData();
}


bool CTString::NewBuffer( int iLength )
{
	FreeData();
	m_pszData = new char[iLength];
	if( m_pszData != NULL )
	{
		memset( m_pszData, 0, sizeof(char)*iLength );
		return true;
	}

	return false;
}

void CTString::FreeData()
{
	if( m_pszData != NULL )
	{
		delete []m_pszData;
		m_pszData = NULL;
	}
	m_iLength = -1;
}

void CTString::Empty()
{
	FreeData();
}

bool CTString::SetString( char * pszData )
{
	return SetString(pszData, -1);
}
bool CTString::SetString( char * pszData, int nDataLen )
{
	int iLen = 0;
	
	FreeData();
	if( pszData == NULL )
	{
		return true;
	}
	
	if (0 > nDataLen)
	{
		iLen=strlen(pszData);
	}
	else
	{
		iLen = nDataLen;
	}
	if( iLen == 0 )
	{
		return true;
	}
	
	if( !NewBuffer( iLen+1 ) )
	{
		return false;
	}
	if( m_pszData != NULL )
	{
		if(0 > nDataLen)
		{
			strcpy(m_pszData, pszData);
		}
		else
		{
			memcpy( m_pszData, pszData , iLen);
		}
	}
	m_iLength = iLen;
	return true;
}

char * CTString::GetString()
{
	return m_pszData;
}

int CTString::GetLength()
{
	if( m_pszData == NULL )
	{
		return 0;
	}
	if( m_iLength < 0 )
	{
		m_iLength = strlen( m_pszData );
	}
	return m_iLength;
}

CTString& CTString::operator =( CTString &pstrData )
{
	SetString( pstrData.m_pszData, pstrData.GetLength() );
	return *this;
}

CTString& CTString::operator =( char *pszData )
{
	SetString( pszData );
	return *this;
}

CTString& CTString::operator =( char cData )
{
	char	szDataTmp[2];

	szDataTmp[0] = cData;
	szDataTmp[1] = '\0'; 
	SetString( szDataTmp );
	return *this;
}

void CTString::LinkStr( char * pszData, int iLength )
{
	int iLenTmp = 0;
	int iSize = 0;
	char * pszTmp = NULL;

	if( pszData == NULL \
		|| iLength < 0 )
	{
		return;
	}

	iLenTmp = GetLength();
	iLenTmp += iLength;
	iSize = iLenTmp;

	if( iLenTmp > 0 )
	{
		pszTmp = new char[iLenTmp+1];
		if( pszTmp == NULL )
		{
			FreeData();
			return;
		}
		memset( pszTmp, 0, iLenTmp+1 );

		iLenTmp = GetLength();
		if( iLenTmp > 0 )
		{
			memcpy( pszTmp, GetString(), iLenTmp );
		}

		memcpy( pszTmp+iLenTmp, pszData, iLength );

		SetString( pszTmp, iSize );
		delete []pszTmp;
	}

	return;
}

CTString& CTString::operator +=( char * pszData )
{
	if( pszData != NULL && strlen( pszData ) > 0 )
	{
		LinkStr( pszData, strlen(pszData) );
	}
	return (*this);
}

CTString& CTString::operator +=( CTString &pstrData )
{
	LinkStr( pstrData.GetString(), pstrData.GetLength() );
	return (*this);
}

CTString& CTString::operator +=( char cData )
{
	char szDataTmp[2];
	szDataTmp[0] = cData;
	szDataTmp[1] = '\0';
	LinkStr( szDataTmp, strlen(szDataTmp) );
	return (*this);
}

char CTString::operator [] (int iIndex)
{
	if( m_pszData == NULL || iIndex < 0 || iIndex >= (int)strlen(m_pszData) )
	{
		return 0;
	}
	return m_pszData[iIndex];
}

bool CTString::operator ==( char *pszData )
{
	if( m_pszData == NULL && pszData == NULL  )
	{
		return true;
	}
	else if( m_pszData == NULL || pszData == NULL )
	{
		return false;
	}
	
	if( 0 == strcmp( m_pszData, pszData ) )
	{
		return true;
	}

	return false;
}

bool CTString::operator ==( CTString &pstrData )
{
	char *pszData = NULL;

	pszData = pstrData.m_pszData;

	if( m_pszData == NULL && pszData == NULL  )
	{
		return true;
	}
	else if( m_pszData == NULL || pszData == NULL )
	{
		return false;
	}
	
	if( this->GetLength() != pstrData.GetLength() )
	{
		return false;
	}
	
	if( 0 == memcmp( m_pszData, pszData, this->GetLength() ) )
	{
		return true;
	}

	return false;
}

void CTString::IntToString( int i )
{
	char szInt[20];
	sprintf(szInt, "%d", i);
	SetString( szInt );
}

void CTString::FloatToString( float f )
{
	char szFloat[128];
	sprintf(szFloat, "%f", f);
	SetString( szFloat );
}

void CTString::DwordToString( DWORD dw )
{
	char szDword[128];
	sprintf(szDword, "%ld", dw);
	SetString( szDword );
}

int CTString::FindSubString( int iBeginIndex, char * pszSubString )
{
	char * pszTmp = NULL;

	if( m_pszData == NULL || pszSubString == NULL 
		|| iBeginIndex < 0 || iBeginIndex >= GetLength() )
	{
		return -1; //没找到子串
	}

	pszTmp = m_pszData+iBeginIndex;
	pszTmp = strstr( pszTmp, pszSubString );
	if( pszTmp != NULL )
	{
		return (pszTmp - m_pszData );
	}

	return -1;
}

bool CTString::GetSubString( int iBeginIndex, CTString& cSubString )
{
	int iLen = GetLength();
	return GetSubString( iBeginIndex, iLen-iBeginIndex, cSubString );
}

bool CTString::GetSubString( int iBeginIndex, int iLength, CTString& cSubString )
{
	char * pszSubString = NULL;
	bool bRet = false;

	if( iBeginIndex < 0 || iLength <= 0
		|| iBeginIndex+iLength > GetLength() )
	{
		return false;
	}

	pszSubString = new char[iLength+1];
	if( pszSubString == NULL )
	{
		return false;
	}
	memset( pszSubString, 0, iLength+1 );
	memcpy( pszSubString, m_pszData + iBeginIndex, iLength );
	bRet = cSubString.SetString( pszSubString, iLength );
	
	delete []pszSubString;
	return bRet;
}

char * CTString::GetBuffer(int iBuffLen)
{
	if( NewBuffer( iBuffLen ) )
	{
		return m_pszData;
	}

	return NULL;
}

bool CTString::IsEmpty()
{
	int iLen = 0;
	iLen = GetLength();
	if( iLen <= 0 )
	{
		return true;
	}
	return false;
}

//大写转小写
void CTString::Lcase()
{
	int i = 0;

	for( i=0; i<m_iLength; i++ )
	{
		if( m_pszData[i] >= 'A' && m_pszData[i] <= 'Z')
		{
			m_pszData[i] += ('a'-'A');
		}
	}
}

//小写转大写 
void CTString::Ucase()
{
	int i = 0;

	for( i=0; i<m_iLength; i++ )
	{
		if( m_pszData[i] >= 'a' && m_pszData[i] <= 'z')
		{
			m_pszData[i] -= ('a'-'A');
		}
	}
} 

