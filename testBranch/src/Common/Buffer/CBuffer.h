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
	// �Ҵ�� ������ ���� �ּҸ� ����Ų��.
	char *m_pBuffer;
//	char *m_pBufferTemp;

	// �����͸� �о�ö��� ���� ��ġ
	unsigned int m_nStartOfDataPosition;

	// ���ۿ� ����� �������� ����  ������ ��ġ
	unsigned int m_nEndOfDataPosition;

	// �Ҵ�� ������ ũ��, �Ҵ�� ������ ������ ��ġ = m_nAllocatedBufferSize - 1
	unsigned int m_nAllocatedBufferSize;
	//------------------------------

	char *m_p2Base;
	char *m_p4Base;
	char *m_p8Base;
private:
	// nSize ��ŭ Write �������� üũ
	bool WriteAvailable( unsigned int nSize );
	// nSize ��ŭ Read �������� üũ
	bool ReadAvailable( unsigned int nSize );
	//nSwap Byte Ÿ�� ������ 
	char* BufferForSwap( unsigned int nSwapBytes );

public:
	//m_nStartOfDataPosition ����
	unsigned int GetStartPosition( );

	// ���۸��� ������ ������ ��, ��밡���� ������ ������
	unsigned int GetBufferingSize( );

	// buffer�κ��� nReadSize��ŭ �о����, ������ŭ ���� ����
	bool Read( char* pBuffer, unsigned int nReadSize );

	// nWriteSize��ŭ Data ���� Write
	bool Write(char* pBuffer, unsigned int nWriteSize);

	// nWriteSize��ŭ Buffer�� nPosition�� Write
	bool WriteAt( unsigned int nPosition, char* pBuffer, unsigned int nWriteSize);

	//�Ҵ�� ���� ������ Get 
	unsigned int GetAllocatedBufferSize();

	//���� ���� 
	void Reset();

	//���� ���� 
	void Destroy();

	//���۸� �ʱ�ȭ ��, nAllocBufferSize��ŭ ���Ҵ� 
	bool ReAllocBuffer(unsigned int nAllocBufferSize);

	//���� ��ġ�� ������ �ּҸ� ����
	char* GetData();

	//������ ����� nBufferSize��ŭ �߰�
	bool ExtensionBuffer( unsigned int nBufferSize );

	//�ش� ���۸� nSwapBytes�� �������� Byte �迭 ����
	unsigned int SwapBuffer( unsigned int nSwapBytes );
	
	//�ش� ������ nSwapSize��ŭ  nSwapBytes�� �������� Byte �迭 ����
	unsigned int SwapBuffer( unsigned int nSwapBytes, unsigned int nSwapSize );

	//Wirte ��ġ ����
	char* GetWritePositon();
	
	//�ش� ������ ���� ������
	unsigned int RemainBuffer();

	//Socket Recv �ܿ��� ����� �����ؼ� �� ��
	//�ش� ������ Read ��ġ�� �̵�, m_nStartOfDataPosition ��ġ�� �ڷ� �̵�
//	void MoveReadPosition( unsigned int nMoveOffset );

	//�ش� ������ Write ��ġ�� �̵�, m_nEndOfDataPosition ��ġ�� �ڷ� �̵�
	void MoveWritePosition( unsigned int nMoveOffset );

	//void RemainFromRear(int nSize);
};

#endif /* CBuffer_H_ */

