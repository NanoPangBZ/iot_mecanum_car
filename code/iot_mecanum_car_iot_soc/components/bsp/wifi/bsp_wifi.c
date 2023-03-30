#include "bsp_wifi.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define TAG "bsp_wifi"

static EventGroupHandle_t s_wifi_event_group = NULL;   //wifi事件
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static esp_event_handler_instance_t instance_any_id;
static esp_event_handler_instance_t instance_got_ip;

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT )
    {
        switch( event_id )
        {
            case WIFI_EVENT_STA_START:
                esp_wifi_connect();
            break;
        }
    }else if( event_base == IP_EVENT )
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        switch ( event_id )
        {
            //获得网关分配的IP地址
            case IP_EVENT_STA_GOT_IP:
                ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
                xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
            break;
            default:
            break;
        }
    }
}

int bsp_wifi_connect( char* wifi_name , char* passwd )
{
    esp_wifi_disconnect();

    esp_err_t ret = nvs_flash_init();
    if( ret != ESP_OK )
    {
        ESP_LOGE( TAG , "nvs init error!" );
    }

    if( !s_wifi_event_group )
        s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    wifi_config_t wifi_config;
    strcpy( (char*)wifi_config.sta.ssid , wifi_name );
    strcpy( (char*)wifi_config.sta.password , passwd );
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    wifi_config.sta.sae_pwe_h2e = WPA3_SAE_PWE_BOTH;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    //等待连接事件
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT)
    {
        return 0;    
    }else if( bits & WIFI_FAIL_BIT )
    {
        return -1;
    }

    return -1;
}
