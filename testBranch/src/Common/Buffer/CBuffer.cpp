#include "CBuffer.h"
#include <memory>

//ok
CBuffer::CBuffer(unsigned int nAllocBufferSize )
{
	m_nAllocatedBufferSize = nAllocBufferSize;

	m_pBuffer = new char[m_nAllocatedBufferSize];
//	m_pBufferTemp = new char[m_nAllocatedBufferSize];
	m_p2Base = new char[2];
	m_p4Base = new char[4];
	m_p8Base = new char[8];

	memset( m_pBuffer, 0, m_nAllocatedBufferSize );
//	memset( m_pBufferTemp, 0, m_nAllocatedBufferSize );
	memset( m_p2Base, 0, 2 );
	memset( m_p4Base, 0, 4 );
	memset( m_p8Base, 0, 8 );

	Reset( );
}

//ok
CBuffer::~CBuffer( )
{
	Destroy( );
}

CBuffer::CBuffer( const CBuffer& rhs )
{
	m_nStartOfDataPosition = rhs.m_nStartOfDataPosition;
	m_nEndOfDataPosition = rhs.m_nEndOfDataPosition;
	m_nAllocatedBufferSize = rhs.m_nAllocatedBufferSize;

	m_pBuffer = new char[m_nAllocatedBufferSize];
//	m_pBufferTemp = new char[m_nAllocatedBufferSize];
	memmove( m_pBuffer, rhs.m_pBuffer, m_nEndOfDataPosition //Start부터 복사해야되는거 아냐?
		- m_nStartOfDataPosition );

}

//ok
void CBuffer::Reset( )
{
	m_nStartOfDataPosition = 0;
	m_nEndOfDataPosition = 0;
}

//ok
const CBuffer& CBuffer::operator=( const CBuffer& rhs )
{
	Destroy( );
	m_nStartOfDataPosition = rhs.m_nStartOfDataPosition;
	m_nEndOfDataPosition = rhs.m_nEndOfDataPosition;
	m_nAllocatedBufferSize = rhs.m_nAllocatedBufferSize;

	m_pBuffer = new char[m_nAllocatedBufferSize];
//	m_pBufferTemp = new char[m_nAllocatedBufferSize];
	memmove( m_pBuffer, rhs.m_pBuffer, m_nEndOfDataPosition - m_nStartOfDataPosition );

	return *this;
}

//ok
void CBuffer::Destroy( )
{
	//Reset();
	if( m_pBuffer != NULL )
		delete[] m_pBuffer;
//	if( m_pBufferTemp != NULL )
//		delete[] m_pBufferTemp;
	if( m_p2Base != NULL )
		delete[] m_p2Base;
	if( m_p4Base != NULL )
		delete[] m_p4Base;
	if( m_p8Base != NULL )
		delete[] m_p8Base;

	m_pBuffer = NULL;
//	m_pBufferTemp = NULL;
	m_p2Base = NULL;
	m_p4Base = NULL;
	m_p8Base = NULL;
}

// 버퍼링된 데이터 사이즈 중, 사용가능한 데이터 사이즈
unsigned int CBuffer::GetBufferingSize( )
{
	//    return m_nEndOfDataPosition; //이렇게 하면 그냥 버퍼링된 데이터 사이즈야
	return m_nEndOfDataPosition - m_nStartOfDataPosition;
}

//ok
bool CBuffer::Read( char* pBuffer, unsigned int nReadSize )
{
	if( ReadAvailable( nReadSize ) == false || nReadSize == 0 )
		return false;

	memmove( pBuffer, m_pBuffer + m_nStartOfDataPosition, nReadSize );

	m_nStartOfDataPosition += nReadSize; //read 하면 밀어

	//Read에서는 Start로 무조건 정렬되
	memmove( m_pBuffer, m_pBuffer + m_nStartOfDataPosition, GetBufferingSize( ) ); //start가 0이라고 가정했는데
	m_nStartOfDataPosition -= nReadSize;
	m_nEndOfDataPosition -= nReadSize;
	//	m_nEndOfDataPosition -= m_nStartOfDataPosition;
	//	m_nStartOfDataPosition = 0;

	//if(GetBufferingSize()==0)
	//	Reset();
	return true;
}

unsigned int CBuffer::GetAllocatedBufferSize( )
{
	return m_nAllocatedBufferSize;
}

bool CBuffer::ReadAvailable( unsigned int nSize )
{
	if( GetBufferingSize( ) < nSize )
		return false;
	return true;
}

bool CBuffer::WriteAvailable( unsigned int nSize )
{
	unsigned int nRemainBuffer = m_nAllocatedBufferSize - m_nEndOfDataPosition;
	if( nRemainBuffer < nSize )
	{
		return false;
	}
	return true;
}

//ok
bool CBuffer::ReAllocBuffer( unsigned int nAllocBufferSize )
{
	Destroy( );
	m_nAllocatedBufferSize = nAllocBufferSize;
	m_pBuffer = new char[m_nAllocatedBufferSize];
//	m_pBufferTemp = new char[m_nAllocatedBufferSize];
	Reset( );

	return true;
}

char* CBuffer::GetData( )
{
	return m_pBuffer + m_nStartOfDataPosition;
}

//ok
bool CBuffer::ExtensionBuffer( unsigned int nBufferSize )
{
	unsigned int nGetBufferingSize = GetBufferingSize( );
	char* pExtensionBuffer = new char[nBufferSize + nGetBufferingSize];

//m_pBufferTemp는 m_pBuffer 따라가네
//	if( m_pBufferTemp != NULL )
//		delete[] m_pBufferTemp;
//	m_pBufferTemp = NULL;
//	m_pBufferTemp = new char[nBufferSize + nGetBufferingSize];
//
	
	memmove( pExtensionBuffer, GetData( ), nGetBufferingSize );

	if( m_pBuffer != NULL )
		delete[] m_pBuffer;

	m_pBuffer = pExtensionBuffer;

//필요없는거 같에
//	m_nEndOfDataPosition -= m_nStartOfDataPosition;
//	m_nStartOfDataPosition = 0;
	m_nAllocatedBufferSize = nBufferSize + nGetBufferingSize;

	return true;
}

char* CBuffer::BufferForSwap( unsigned int nSwapBytes )
{
	switch( nSwapBytes )
	{
	case 2 :
		return m_p2Base;
	case 4 :
		return m_p4Base;
	case 8 :
		return m_p8Base;
	default :
		return NULL;
	}
}

//nSwapBytes의 배수가 아니면 어떻게 되지?
unsigned int CBuffer::SwapBuffer( unsigned int nSwapBytes )
{
	char* lp_reverse = BufferForSwap( nSwapBytes );
	char* lp_origin = (char*)GetData( );
	memset( lp_reverse, 0, nSwapBytes );

	unsigned int nIndex_reverse = 0;
	unsigned int nIndex_origin = nSwapBytes;

	for( unsigned int i = 0 ; i < GetBufferingSize( ) ; i += nSwapBytes )
	{
		nIndex_origin = nSwapBytes;
		nIndex_reverse = 0;

		for( unsigned int j = 0 ; j < nSwapBytes ; j++ )
		{
			nIndex_origin--;
			lp_reverse[nIndex_reverse] = lp_origin[nIndex_origin];
			nIndex_reverse++;
		}

		memmove( lp_origin, lp_reverse, nSwapBytes );

		lp_origin += nSwapBytes;
	}

	return GetBufferingSize( ) % nSwapBytes;
}

unsigned int CBuffer::SwapBuffer( unsigned int nSwapBytes, unsigned int nSwapSize )
{
	char* lp_reverse = BufferForSwap( nSwapBytes );
	char* lp_origin = (char*)GetData( );
	memset( lp_reverse, 0, nSwapBytes );

	int nIndex_reverse = 0;
	int nIndex_origin = nSwapBytes;

	for( unsigned int i = 0 ; i < nSwapSize ; i += nSwapBytes )
	{
		nIndex_origin = nSwapBytes;
		nIndex_reverse = 0;

		for( unsigned int j = 0 ; j < nSwapBytes ; j++ )
		{
			nIndex_origin--;
			lp_reverse[nIndex_reverse] = lp_origin[nIndex_origin];
			nIndex_reverse++;
		}

		memmove( lp_origin, lp_reverse, nSwapBytes );

		lp_origin += nSwapBytes;
	}

	return GetBufferingSize( ) % nSwapBytes;
}

//void CBuffer::MoveReadPosition( unsigned int nMoveOffset )
//{
//	//	m_nStartOfDataPosition += nMoveOffset;
//	//
//	//	if (GetBufferingSize() < m_nStartOfDataPosition)
//	//	{
//	//		memmove(m_pBuffer, m_pBuffer+m_nStartOfDataPosition, GetBufferingSize());
//	//		m_nEndOfDataPosition -= m_nStartOfDataPosition;
//	//		m_nStartOfDataPosition = 0;
//	//	}
//	//110224 - 기훈 수정 //일단 버리게 해놨음.
//	if( nMoveOffset <= 0 || nMoveOffset > m_nAllocatedBufferSize )
//		return;
//
//	m_nStartOfDataPosition += nMoveOffset;
//
//	if( GetBufferingSize( ) < m_nStartOfDataPosition )
//	{
//		if( GetBufferingSize( ) < 0 )
//		{
//			Reset( );
//		}
//		else
//		{
//			memmove( m_pBuffer, m_pBuffer + m_nStartOfDataPosition, GetBufferingSize( ) );
//			m_nEndOfDataPosition -= m_nStartOfDataPosition;
//			m_nStartOfDataPosition = 0;
//		}
//	}
//
//}

void CBuffer::MoveWritePosition( unsigned int nMoveOffset )
{
	if( nMoveOffset < 0 )
		return;
	m_nEndOfDataPosition += nMoveOffset;
}

//ok
unsigned int CBuffer::RemainBuffer( )
{
	//버퍼 마지막 위치 = 사이즈  - 1
	return ( m_nAllocatedBufferSize - 1 ) - m_nEndOfDataPosition;
}

//ok
bool CBuffer::Write( char* pBuffer, unsigned int nWriteSize )
{
	if( nWriteSize == 0 )
		return false;
	
	if( WriteAvailable( nWriteSize ) == false )
	{
		ExtensionBuffer( nWriteSize );
	}

	memmove( m_pBuffer + m_nEndOfDataPosition, pBuffer, nWriteSize );
	m_nEndOfDataPosition += nWriteSize;
//temp도 똑같이 복사하네
//	memmove( m_pBufferTemp, m_pBuffer, m_nAllocatedBufferSize );
//	memmove( m_pBuffer, m_pBufferTemp + m_nStartOfDataPosition, GetBufferingSize( ) );
//	m_nEndOfDataPosition -= m_nStartOfDataPosition;
//
	return true;
}

//ok
bool CBuffer::WriteAt( unsigned int nPosition, char* pBuffer, unsigned int nWriteSize )
{
	if( nPosition > m_nAllocatedBufferSize - 1 || nWriteSize == 0 )
		return false;
	
	unsigned int nTemp = m_nEndOfDataPosition;
	m_nEndOfDataPosition = nPosition;

	if( WriteAvailable( nWriteSize ) == false )
	{
		m_nEndOfDataPosition = nTemp;
		ExtensionBuffer( ( nPosition + 1 ) + nWriteSize );
		return false;
	}

	m_nEndOfDataPosition = nTemp;
	memmove( m_pBuffer + nPosition, pBuffer, nWriteSize );

	if( m_nEndOfDataPosition < ( nPosition + nWriteSize ) )
	{
		m_nEndOfDataPosition = nPosition + nWriteSize;
	}

	return true;
}

//ok
char* CBuffer::GetWritePositon( )
{
	return m_pBuffer + m_nEndOfDataPosition;
}
/*
void CBuffer::RemainFromRear(int nSize)
{ 
int nReadSize = GetBufferingSize()-nSize;

m_nStartOfDataPosition += nReadSize;
memmove(m_pBuffer, m_pBuffer+m_nStartOfDataPosition, GetBufferingSize());
m_nStartOfDataPosition -= nReadSize;
m_nEndOfDataPosition -= nReadSize;

}*/

//ok
unsigned int CBuffer::GetStartPosition( )
{
	return m_nStartOfDataPosition;
}
