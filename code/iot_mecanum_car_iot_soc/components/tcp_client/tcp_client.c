#include "tcp_client.h"

#include "esp_log.h"
#define TAG "tcp_client"

static int sockfd = -1;     //套接字

int tcp_client_connect( uint32_t ip , uint16_t port )
{
    int ret = -1;
    /* 打开一个socket套接字 */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (-1 == sockfd)
	{
		ESP_LOGE( TAG , "socket open failure !!! \n");
		return -1;
	}
    else
    {
        struct sockaddr_in seraddr = {0};
        seraddr.sin_family = AF_INET;		                    /* 设置地址族为IPv4 */
	    seraddr.sin_port = port;	                    /* 设置地址的端口号信息 */
	    seraddr.sin_addr.s_addr = ip;	/*　设置IP地址 */
	    ret = connect(sockfd, (const struct sockaddr *)&seraddr, sizeof(seraddr));
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

int tcp_client_send( uint8_t* data , uint8_t len )
{
    if( sockfd == -1 )
    {
        ESP_LOGW( TAG , "socket is not init!" );
        return -1;
    }

    int ret = send( sockfd , data , len , 0);

    return ret<0? -1 : 0 ;
}

int tcp_client_recieve( uint8_t* buf , uint8_t buf_len )
{
    if( sockfd == -1 )
    {
        ESP_LOGW( TAG , "socket is not init!" );
        return -1;
    }

    int num = lwip_read( sockfd , buf , buf_len );

    if( num == -1 )
    {
        ESP_LOGE( TAG , " recieve error! " );
    }

    return num;
}
