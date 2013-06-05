#ifndef CBuffer_H_
#define CBuffer_H_

class CBuffer
{ 
public:
	CBuffer( unsigned int nAllocBufferSize );
	CBuffer( const CBuffer& rhs );
	const CBuffer& operator=( const CBuffer& rhs );
	virtual ~CBuffer( );

private:
	// 할당된 버퍼의 시작 주소를 가리킨다.
	char *m_pBuffer;
//	char *m_pBufferTemp;

	// 데이터를 읽어올때의 시작 위치
	unsigned int m_nStartOfDataPosition;

	// 버퍼에 저장된 데이터의 실제  마지막 위치
	unsigned int m_nEndOfDataPosition;

	// 할당된 버퍼의 크기, 할당된 버퍼의 마지막 위치 = m_nAllocatedBufferSize - 1
	unsigned int m_nAllocatedBufferSize;
	//------------------------------

	char *m_p2Base;
	char *m_p4Base;
	char *m_p8Base;
private:
	// nSize 만큼 Write 가능한지 체크
	bool WriteAvailable( unsigned int nSize );
	// nSize 만큼 Read 가능한지 체크
	bool ReadAvailable( unsigned int nSize );
	//nSwap Byte 타입 사이즈 
	char* BufferForSwap( unsigned int nSwapBytes );

public:
	//m_nStartOfDataPosition 리턴
	unsigned int GetStartPosition( );

	// 버퍼링된 데이터 사이즈 중, 사용가능한 데이터 사이즈
	unsigned int GetBufferingSize( );

	// buffer로부터 nReadSize만큼 읽어오기, 읽은만큼 버퍼 삭제
	bool Read( char* pBuffer, unsigned int nReadSize );

	// nWriteSize만큼 Data 끝에 Write
	bool Write(char* pBuffer, unsigned int nWriteSize);

	// nWriteSize만큼 Buffer의 nPosition에 Write
	bool WriteAt( unsigned int nPosition, char* pBuffer, unsigned int nWriteSize);

	//할당된 버퍼 사이즈 Get 
	unsigned int GetAllocatedBufferSize();

	//버퍼 리셋 
	void Reset();

	//버퍼 종료 
	void Destroy();

	//버퍼를 초기화 후, nAllocBufferSize만큼 재할당 
	bool ReAllocBuffer(unsigned int nAllocBufferSize);

	//현재 위치의 버퍼의 주소를 리턴
	char* GetData();

	//버퍼의 사이즈를 nBufferSize만큼 추가
	bool ExtensionBuffer( unsigned int nBufferSize );

	//해당 버퍼를 nSwapBytes를 기준으로 Byte 배열 변경
	unsigned int SwapBuffer( unsigned int nSwapBytes );
	
	//해당 버퍼의 nSwapSize만큼  nSwapBytes를 기준으로 Byte 배열 변경
	unsigned int SwapBuffer( unsigned int nSwapBytes, unsigned int nSwapSize );

	//Wirte 위치 리턴
	char* GetWritePositon();
	
	//해당 버퍼의 남은 사이즈
	unsigned int RemainBuffer();

	//Socket Recv 외에는 사용을 주의해서 할 것
	//해당 버퍼의 Read 위치를 이동, m_nStartOfDataPosition 위치를 뒤로 이동
//	void MoveReadPosition( unsigned int nMoveOffset );

	//해당 버퍼의 Write 위치를 이동, m_nEndOfDataPosition 위치를 뒤로 이동
	void MoveWritePosition( unsigned int nMoveOffset );

	//void RemainFromRear(int nSize);
};

#endif /* CBuffer_H_ */

