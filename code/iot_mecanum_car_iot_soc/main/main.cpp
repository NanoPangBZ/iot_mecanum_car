/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "bsp.h"

#define TAG "main"

#include "esp_netif.h"
#include "driver/gpio.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
static void tcp_client_task(void *pvParameters)
{
    int sockfd = -1;    /* 定义一个socket描述符，用来接收打开的socket */
    int ret = -1;
    /* 打开一个socket套接字 */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (-1 == sockfd)
	{
		ESP_LOGE( TAG , "socket open failure !!! \n");
		return ;
	}
    else
    {
        struct sockaddr_in seraddr = {0};
        seraddr.sin_family = AF_INET;		                    /* 设置地址族为IPv4 */
	    seraddr.sin_port = htons(12341);	                    /* 设置地址的端口号信息 */
	    seraddr.sin_addr.s_addr = inet_addr("192.168.1.113");	/*　设置IP地址 */
	    ret = connect(sockfd, (const struct sockaddr *)&seraddr, sizeof(seraddr));
        if(ret < 0)
            printf("connect to server failure !!! ret = %d \n",ret);
        else
        {
            printf("connect success.\n");
            int cnt = 10;
            while(cnt--)
            {
                ret = send(sockfd, "I am ESP32.", strlen("I am ESP32."), 0);
                if(ret < 0)
                    printf("send err. \n");
                else
                    printf("send ok. \n");
                vTaskDelay(2000 / portTICK_PERIOD_MS);   /* 延时2000ms*/
            }      
        }          
        close(sockfd);
    }
    
    vTaskDelete(NULL);
}

extern "C" void app_main(void)
{
    ESP_LOGI( TAG , "App run." );
    bsp_init();
    xTaskCreate(tcp_client_task, "tcp_client", 4096, NULL, 5, NULL);
}
