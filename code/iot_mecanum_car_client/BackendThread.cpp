#include "BackendThread.h"
#include <windows.h>
#include <QVariant>
#include <QQuickItem>
#include <math.h>

#include "TcpClient/TcpClient.h"
#include "scp_trans.h"

//唯一实例
BackendThread BackendThread::backendThread;

//UI对象
extern QObject* qmlObj;

//静态变量
static TcpClient* client = NULL;

//包装
static int send_port( uint8_t* data , uint16_t len )
{
    if( client == nullptr )
        return -1;
    return client->send( data , len ) ? len : -1;
}

static int scp_recieve_respond( scp_pack_t* pack )
{
    printf("pack printf.\n");
        printf( "control word: 0x%04X\n" , pack->control_word );
        printf( "cmd word: 0x%02x\n" , pack->cmd_word );
        printf( "param:\n" );
        int index = 0;
        while( index < pack->payload_len )
        {
            for( int line_c = 0 ; line_c < 16 ; line_c++ )
            {
                if( index < pack->payload_len )
                    printf( "0x%02X  " , pack->payload.buf[index] );
                index ++;
            }
            printf("\n");
        }
        printf("\n");
    return 0;
}

//后台线程
void* BackendThread::_main(void*param)
{
    sem_wait( &backendThread._rootUiCompleted );
    Sleep(100);

    backendThread.setCarRectangle( 100 , 140 );

    uint8_t send_buf[1024];
    uint8_t decoder_buf[1024];
    uint8_t tcp_buf[512];
    scp_pack_t send_pack = scp_trans_pack_create( send_buf , 1024 );
    scp_pack_t recieve_pack = scp_trans_pack_create( decoder_buf , 1024 );
    scp_trans_decoder_t decoder = scp_trans_decoder_create( &recieve_pack , scp_recieve_respond );

    while(1)
    {
        if( client != nullptr )
        {
            send_pack.cmd_word = 0xfe;
            send_pack.control_word = 0xffff;
            send_pack.payload_len = 72;
            for( uint8_t temp = 0; temp < 72 ; temp++ )
                send_pack.payload.buf[ temp ] = temp;
            scp_trans_send( &send_pack , send_port );
            qDebug("test send");
            scp_trans_decoder_input( &decoder , tcp_buf , client->readAll( tcp_buf ) );
        }
        Sleep(5000);
    }
    return param;
}

BackendThread::BackendThread() : QObject{NULL}
{
    sem_init( &_rootUiCompleted , 0 , 0 );
    pthread_create( &_thread , NULL , _main , this );
}

BackendThread::~BackendThread()
{
}

void BackendThread::qmlCompleted()
{
    sem_post( &_rootUiCompleted );
}

void BackendThread::targetPositionRefresh( float x , float y )
{
}

void BackendThread::connectCar( QString addr )
{
    if( client != NULL )
    {
        delete client;
        client = NULL;
    }

    client = new TcpClient;
    if( client->connect( addr.toLocal8Bit().data() , 144 ) )
    {
        qDebug("连接成功 %s" , addr.toLocal8Bit().data());
    }else
    {
        qDebug( "连接失败" );
        delete client;
        client = NULL;
    }
    Sleep( 2000 );
}

void BackendThread::carYawModeRefresh( int mode )
{

}

void BackendThread::updataCarPosition( float x , float y , float yaw )
{
    qmlObj->setProperty( "carPositionX" , QVariant(x) );
    qmlObj->setProperty( "carPositionY" , QVariant(y) );
    qmlObj->setProperty( "carPositionYaw" , QVariant(yaw) );
}

void BackendThread::setCarRectangle( float width , float height )
{
    qmlObj->setProperty( "carWidth" , QVariant(width) );
    qmlObj->setProperty( "carHeight" , QVariant(height) );
}

