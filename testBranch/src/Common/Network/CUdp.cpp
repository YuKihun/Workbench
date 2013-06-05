#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <memory>
#include "CUdp.h"

//using namespace ht_lib;
//
//pthread_mutex_t CUdp::m_mutexMsgProc = PTHREAD_MUTEX_INITIALIZER;
//unsigned int CUdp::nTest = 0;
//
//CUdp::CUdp( E_HTNetworkType eNetworkType )
//{
//    m_fpDoRecvCall = DoRecvCall;
//    m_fpDoProcCall = DoProcCall;
//    m_fpExternRecv = NULL;
//    m_nLocalPort = 0;
//    m_nSock = 0;
//    setClose();
//    m_nRecvThreadId = 0;
//    m_nProcThreadId = 0;
//    memset( &m_nLocalPort, 0, sizeof(m_nLocalPort) );
//    memset( &m_addrLocal, 0, sizeof(m_addrLocal) );
//    memset( m_strMulticastIp, 0, sizeof(m_strMulticastIp) );
//    memset( m_strLocalIp, 0, sizeof(m_strLocalIp) );
//    m_nTtl = 0;
//    m_eNetworkType = eNetworkType;
//
//    if( m_pRecvBuffer == NULL )
//    {
//        m_pRecvBuffer = new CHTBuffer( DEFAUL_UDP_RECV_BUFFER );
//    }
//
//    if( m_pProcBuffer == NULL )
//    {
//        m_pProcBuffer = new CHTBuffer( DEFAUL_UDP_RECV_BUFFER );
//    }
//
//    CreateRecvThread();
//    CreateProcThread();
//}
//
//CUdp::~CUdp()
//{
//    End();
//    pthread_cancel( m_nRecvThreadId );
//    pthread_cancel( m_nProcThreadId );
//
//    G_TRACE( "HTUdp.cpp", __LINE__, "Call Destroy" );
//    if( m_pRecvBuffer != NULL )
//    {
//        m_pRecvBuffer->Destroy();
//        delete m_pRecvBuffer;
//        m_pRecvBuffer = NULL;
//    }
//    if( m_pProcBuffer != NULL )
//    {
//        m_pProcBuffer->Destroy();
//        delete m_pProcBuffer;
//        m_pProcBuffer = NULL;
//    }
//}
//
//int CUdp::Set( const char* pLocalIp, int nMulticastPort, const char* pMulticastIp, RECV_CALL fpExternRecvCall, void* pRecvParam )
//{
//    if( strlen( pLocalIp ) >= sizeof(m_strLocalIp) )
//        return -1;
//
//    memcpy( m_strLocalIp, pLocalIp, strlen( pLocalIp ) );
//
//    if( strlen( pLocalIp ) >= sizeof(m_strMulticastIp) )
//        return -1;
//    memcpy( m_strMulticastIp, pMulticastIp, strlen( pMulticastIp ) );
//
//    return Set( nMulticastPort, fpExternRecvCall, pRecvParam );
//}
//
//int CUdp::Set( const char* pLocalIp, int nLocalPort, RECV_CALL fpExternRecvCall, void* pRecvParam )
//{
//    unsigned int nStrLen = strlen( pLocalIp );
//
//    if( nStrLen >= sizeof(m_strLocalIp) )
//        return -1;
//
//    memcpy( m_strLocalIp, pLocalIp, nStrLen );
//    return Set( nLocalPort, fpExternRecvCall, pRecvParam );
//}
//
//int CUdp::Set( int nLocalPort, RECV_CALL fpExternRecvCall, void* pRecvParam )
//{
//    //G_TRACE("HTUdp.cpp", __LINE__, "Begin socket..........____");
//    m_fpExternRecv = fpExternRecvCall;
//    m_nLocalPort = nLocalPort;
//    m_pRecvParam = pRecvParam;
//
//    return 1;
//}
//
//int CUdp::Begin()
//{
//    if( isOpen() )
//        return 0;
//
//    int nRet = 0;
//    m_nSock = (int)socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP );
//    nRet = m_nSock;
//    if( m_nSock < 0 )
//    {
//        //  GP_LOG_INFO( (char*)"HTUdp - Create Socket Fail", E_LOG_STATUS_LEVEL_2 );
//        perror( "HTUdp - Create Socket Fail : " );
//        return nRet;
//    }
//    //  GP_LOG_INFO( (char*)"HTUdp - Create Socket Success", E_LOG_STATUS_LEVEL_2 );
//
//    const int nUdpOptionOn = 1;
//    if( setsockopt( m_nSock, SOL_SOCKET, SO_REUSEADDR, (char *)&nUdpOptionOn, sizeof(nUdpOptionOn) ) != 0 )
//    {
//        //  GP_LOG_INFO( (char*)"HTUdp - Socket Option Error", E_LOG_STATUS_LEVEL_2 );
//        perror( "HTUdp - Socket Option Error : " );
//    }
//#ifndef _WINDOWS_
//    if( m_nSocketSendBufferSize > 0 )
//        setsockopt( m_nSock, SOL_SOCKET, SO_SNDBUF, (int*)&m_nSocketSendBufferSize, (int)sizeof(m_nSocketSendBufferSize) );
//
//    // 소켓 수신 버퍼 설정
//    if( m_nSocketRecvBufferSize > 0 )
//        setsockopt( m_nSock, SOL_SOCKET, SO_RCVBUF, (int*)&m_nSocketRecvBufferSize, (int)sizeof(m_nSocketRecvBufferSize) );
//    socklen_t nSize = sizeof(int);
//    getsockopt( m_nSock, SOL_SOCKET, SO_RCVBUF, (int*)&m_nSocketRecvBufferSize, &nSize );
//    getsockopt( m_nSock, SOL_SOCKET, SO_SNDBUF, (int*)&m_nSocketSendBufferSize, &nSize );
//
//#endif
//    memset( &m_addrLocal, 0, sizeof(m_addrLocal) );
//    m_addrLocal.sin_family = AF_INET;
//
//    switch( m_eNetworkType )
//    {
//        case E_NET_UNICAST :
//            m_addrLocal.sin_addr.s_addr = inet_addr( m_strLocalIp ); //자신의 주소...
//            break;
//        default :
//            m_addrLocal.sin_addr.s_addr = INADDR_ANY;
//            break;
//    }
//
//    m_addrLocal.sin_port = htons( m_nLocalPort );
//
//    nRet = bind( m_nSock, (sockaddr*)&m_addrLocal, sizeof(m_addrLocal) );
//    if( nRet < 0 )
//    {
//        //  GP_LOG_INFO( (char*)"HTUdp - Socket Bind Error", E_LOG_STATUS_LEVEL_2 );
//        perror( "HTUdp - Socket Bind Error : " );
//
//        shutdown( m_nSock, SHUT_RDWR );
//        close( m_nSock );
//        return nRet;
//    }
//
//    if( SetUdpOption() != 0 )
//        return -1;
//
//    setOpen();
//
//    return nRet;
//}
//
//int CUdp::SetUdpOption()
//{
//    int nResult = 0;
//
//    switch( m_eNetworkType )
//    {
//        case E_NET_UNICAST :
//            break;
//        case E_NET_MULTICAST :
////            if( (nResult = setMulticastInterface()) != 0 )
////                return nResult;
//
//            if( (nResult = setMulticastJoin()) != 0 )
//                return nResult;
//            break;
//
//        case E_NET_BROADCAST :
//            if( (nResult = setBroadcastSend()) != 0 )
//                return nResult;
//            break;
//        default :
//            return -1;
//    }
//
//    return 0;
//}
//
//int CUdp::setMulticastInterface()
//{
//    struct in_addr inNetIf;
//
//    inNetIf.s_addr = inet_addr( m_strLocalIp );
//
//    // Select a Default Interface for Outgoing Multicasts
//    if( setsockopt( m_nSock, IPPROTO_IP, IP_MULTICAST_IF, (char*)&inNetIf.s_addr, sizeof(struct in_addr) ) != 0 )
//    {
//        //  GP_LOG_INFO( (char*)"HTUdp - Mutlicast Interface Error", E_LOG_STATUS_LEVEL_2 );
//        perror( "HTUdp - Mutlicast Interface Error: " );
//        return -1;
//    }
//
//    return 0;
//}
//
//int CUdp::setMulticastJoin()
//{
//    struct ip_mreq ipmMcReq;
//    char cLoopBackOn;
//
//    //    cLoopBackOn = 0; //비활성
//    cLoopBackOn = 1; //활성화
//
//    if( m_nTtl > 0 )
//    {
//        if( setsockopt( m_nSock, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&m_nTtl, sizeof(m_nTtl) ) != 0 )
//        {
//            //  GP_LOG_INFO( (char*)"HTUdp - TLL is not set", E_LOG_STATUS_LEVEL_2 );
//            perror( "HTUdp - TLL is not set: " );
//        }
//    }
//
//    // option flag는 반드시 char 형으로 ..... int 는 에러 발생한다. //
//    if( setsockopt( m_nSock, IPPROTO_IP, IP_MULTICAST_LOOP, (char*)&cLoopBackOn, sizeof(cLoopBackOn) ) != 0 )
//    {
//        //  GP_LOG_INFO( (char*)"HTUdp - BLOCK LOOP BACK ERROR", E_LOG_STATUS_LEVEL_2 );
//        perror( "HTUdp - BLOCK LOOP BACK ERROR: " );
//        return -1;
//    }
//
//    ipmMcReq.imr_interface.s_addr = inet_addr( m_strLocalIp );
//    ipmMcReq.imr_multiaddr.s_addr = inet_addr( m_strMulticastIp );
//
//    if( setsockopt( m_nSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&ipmMcReq, sizeof(ipmMcReq) ) != 0 )
//    {
//        //  GP_LOG_INFO( (char*)"HTUdp - JOIN Multicast", E_LOG_STATUS_LEVEL_2 );
//        perror( "HTUdp - JOIN Multicast: " );
//        return -1;
//    }
//
//    return 0;
//}
//
//int CUdp::setBroadcastSend()
//{
//    const int nUdpBroadcast = 1;
//
//    if( setsockopt( m_nSock, SOL_SOCKET, SO_BROADCAST, (char *)&nUdpBroadcast, sizeof(nUdpBroadcast) ) != 0 )
//    {
//        perror( "HTUdp - Set Broadcast :" );
//        return -1;
//    }
//
//    return 0;
//}
//
//void CUdp::CreateRecvThread()
//{
//    // 4. 수신 전용 쓰레드 생성.
//    if( pthread_create( &m_nRecvThreadId, NULL, m_fpDoRecvCall, this ) != 0 )
//    {
//        //  GP_LOG_INFO( (char*)"HTUdp - RecvThread Create Fail", E_LOG_STATUS_LEVEL_2 );
//        perror( "HTUdp - RecvThread Create Fail: " );
//    }
//}
//
//void CUdp::CreateProcThread()
//{
//    // 4. 메시지 처리 전용 쓰레드 생성.
//    if( pthread_create( &m_nProcThreadId, NULL, m_fpDoProcCall, this ) != 0 )
//    {
//        //  GP_LOG_INFO( (char*)"HTUdp - ProcThread Create Fail", E_LOG_STATUS_LEVEL_2 );
//        perror( "HTUdp - ProcThread Create Fail: " );
//    }
//}
//
//void CUdp::DoRecv()
//{
//    sockaddr_in recv_src_addr;
//#ifndef _WINDOWS_
//    socklen_t nTarget_addr_size = sizeof(recv_src_addr);
//    //    int nTarget_addr_size = sizeof(recv_src_addr);
//    //    size_t nRecvSize = 0;
//    int nRecvSize = 0;
//#else
//    int nTarget_addr_size = sizeof(recv_src_addr);
//    int nRecvSize = 0;
//#endif
//
//    while( isOpen() )
//    {
//        if( m_pRecvBuffer != NULL && m_pProcBuffer != NULL )
//        {
//            nRecvSize = recvfrom( m_nSock, m_pRecvBuffer->GetWritePositon(), m_pRecvBuffer->RemainBuffer(), 0, (sockaddr*)&recv_src_addr, &nTarget_addr_size );
//
//            if( (int)nRecvSize > 0 )
//            {
//                m_pRecvBuffer->MoveWritePosition( nRecvSize );
//            }
//            else if( m_pRecvBuffer->GetBufferingSize() < 1 )
//            {
//                //m_pRecvBuffer->Reset();
//                continue;
//            }
//
//            if( GetProcSuccessed() )
//            {
//                pthread_mutex_lock( &m_mutexMsgProc );
//                SetProcSuccessed( false );
//
//                if( m_pProcBuffer->GetBufferingSize() == 0 )
//                {
//                    CHTBuffer* pTempBuffer = m_pRecvBuffer;
//                    m_pRecvBuffer = m_pProcBuffer;
//                    m_pProcBuffer = pTempBuffer;
//                }
//
//                pthread_kill( m_nProcThreadId, SIGUSR2 );
//
//                SetProcSuccessed( true );
//                pthread_mutex_unlock( &m_mutexMsgProc );
//            }
//        }
//    }
//}
//
//void sig_handler( int signo )
//{
//    //  GP_LOG_INFO((char*)"HTUdp - RecvBufferSwap",E_LOG_STATUS_LEVEL_2);
//}
//void CUdp::DoProc()
//{
//    //    return;
//    int lsigno;
//    bool bSwapEnable = false;
//    struct sigaction act;
//    sigset_t proc_mask;
//    sigemptyset( &proc_mask );
//    sigaddset( &proc_mask, SIGUSR2 );
//    act.sa_handler = sig_handler;
//    sigaction( SIGUSR2, &act, NULL );
//
//    while( true )
//    {
//        if( m_pRecvBuffer != NULL && m_pProcBuffer != NULL )
//        {
//            if( !bSwapEnable )
//                sigwait( &proc_mask, &lsigno );
//
//            bSwapEnable = false;
//            pthread_mutex_lock( &m_mutexMsgProc );
//
//            if( m_pProcBuffer->GetBufferingSize() > 0 )
//            {
//                m_fpExternRecv( *m_pProcBuffer, m_pRecvParam ); // void CHTAdaptorHost::RecvMain(CHTBuffer& buffer, void* pRecvParam)
//            }
//
//            if( m_pRecvBuffer->GetBufferingSize() > 0 )
//            {
//                CHTBuffer* pTempBuffer = m_pRecvBuffer;
//                m_pRecvBuffer = m_pProcBuffer;
//                m_pProcBuffer = pTempBuffer;
//                bSwapEnable = true;
//            }
//
//            SetProcSuccessed( true );
//            pthread_mutex_unlock( &m_mutexMsgProc );
//        }
//    }
//}
//
//int CUdp::SendData( char* pBuffer, int nBufferLength, const char* pTargetIp, int nTargetPort )
//{
//    if( isClose() )
//        return -1;
//
//    sockaddr_in addrTarget;
//    addrTarget.sin_family = AF_INET;
//    addrTarget.sin_addr.s_addr = inet_addr( pTargetIp );
//    addrTarget.sin_port = htons( nTargetPort );
//
//    if( nBufferLength > m_nSocketSendBufferSize )
//        return sendto( m_nSock, pBuffer, m_nSocketSendBufferSize, 0, (sockaddr*)&addrTarget, sizeof(addrTarget) );
//    else
//        return sendto( m_nSock, pBuffer, nBufferLength, 0, (sockaddr*)&addrTarget, sizeof(addrTarget) );
//}
//
//void CUdp::End( void )
//{
//    if( isClose() )
//        return;
//
//    shutdown( m_nSock, SHUT_RDWR );
//    close( m_nSock );
//    setClose();
//}
//
//void* CUdp::DoRecvCall( void *lParam )
//{
//    CUdp *pThis = (CUdp*)lParam;
//    //  pThis->DoRecv();
//
//    //    sleep( 1 );
//    usleep( 1 );
//    while( 1 )
//    {
//        if( pThis == NULL )
//        {
//            //  GP_LOG_INFO((char*)"HTUdp - DeRecvCall Error -> pThis is NULL",E_LOG_STATUS_LEVEL_2);
//        }
//        else
//        {
//            pThis->DoRecv();
//        }
//        //        sleep( 1 ); //110610
//        usleep( 1 );
//
//    }
//
//    return 0;
//}
//
//void* CUdp::DoProcCall( void *lParam )
//{
//    CUdp *pThis = (CUdp*)lParam;
//
//    //sleep( 1 ); //이거 왜 쉬어?
//    usleep( 1 );
//    while( 1 )
//    {
//        if( pThis == NULL )
//        {
//        }
//        else
//        {
//            //G_TRACE(__FILE__, __LINE__, "DoProcCall(void *lParam)");
//            pThis->DoProc();
//            //G_TRACE(__FILE__, __LINE__, "Sleep");
//        }
//        //        sleep( 1 ); //110610
//        usleep( 1 );
//    }
//
//    return 0;
//}
//
//void CUdp::SetTTL( int nTtl )
//{
//    m_nTtl = nTtl;
//}
