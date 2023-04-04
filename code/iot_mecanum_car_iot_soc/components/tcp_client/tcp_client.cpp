#include "tcp_client.h"

#include "esp_log.h"
#define TAG "tcp_client"

static uint8_t buf[64];

tcp_client::tcp_client( uint32_t ip , uint16_t port )
{
    _ip = ip;
    _port = port;
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
}

tcp_client::tcp_client( char* ip , uint16_t port )
{
    _ip = inet_addr(ip);
    _port = htons(port);
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
}

tcp_client::~tcp_client()
{
    lwip_close( _sockfd );
}

int tcp_client::connect()
{
    int ret;
    if (-1 == _sockfd)
	{
		ESP_LOGE( TAG , "socket open failure !!! \n");
		return -1;
	}
    else
    {
        struct sockaddr_in seraddr = {0};
        seraddr.sin_family = AF_INET;		                    /* 设置地址族为IPv4 */
	    seraddr.sin_port = _port;	                    /* 设置地址的端口号信息 */
	    seraddr.sin_addr.s_addr = _ip;	/*　设置IP地址 */
	    ret = lwip_connect( _sockfd, (const struct sockaddr *)&seraddr, sizeof(seraddr));
        if(ret < 0)
        {
            ESP_LOGE( TAG , "connect to server failure !!! ret = %d \n",ret);
            return -1;
        }
        else
        {
            ESP_LOGI( TAG , "connect success.\n");
            return 0;
        }          
    }
    return -1;
}

int tcp_client::send( uint8_t* data , uint16_t len )
{
    int ret = lwip_send( _sockfd , data , len , 0);
    return ret<0? -1 : 0 ;
}

int tcp_client::revieve( uint8_t* data_buf , uint16_t buf_len )
{
    int num = lwip_read( _sockfd , data_buf , buf_len );

    if( num == -1 )
    {
        ESP_LOGE( TAG , "recieve error!" );
        ESP_LOGW( TAG , "tcp reconnect" );
        connect();
    }

    return num;
}
