#include "bsp_wifi.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_mac.h"
#include <netdb.h>
#include "esp_log.h"
#include "nvs_flash.h"

#define TAG "bsp_wifi"

// #define STATIC_IP_ADDR          "192.168.1.202"     //静态IP地址
// #define STATIC_NETMASK_ADDR     "255.255.255.0"     //静态子网掩码
// #define STATIC_GW_ADDR          "192.168.1.1"       //静态网关IP地址
// #define MAIN_DNS_SERVER         STATIC_GW_ADDR      //DNS服务器
// #define BACKUP_DNS_SERVER       "0.0.0.0"           //DNS备份服务器

static EventGroupHandle_t s_wifi_event_group = NULL;   //wifi事件 同时用作bsp_wifi初始化标志
#define WIFI_CONNECTED_BIT          BIT0
#define WIFI_DISCONNECTED_BIT       BIT1
#define WIFI_AP_START_BIT           BIT2

static esp_netif_t* _sta_net_if = NULL;
static esp_netif_t* _ap_net_if = NULL;
static esp_event_handler_instance_t _instance_any_id = NULL;
static esp_event_handler_instance_t _instance_got_ip = NULL;

// static esp_err_t set_dns_server(esp_netif_t *netif, uint32_t addr, esp_netif_dns_type_t type)
// {
//     if (addr && (addr != IPADDR_NONE)) {
//         esp_netif_dns_info_t dns;
//         dns.ip.u_addr.ip4.addr = addr;
//         dns.ip.type = IPADDR_TYPE_V4;
//         ESP_ERROR_CHECK(esp_netif_set_dns_info(netif, type, &dns));
//     }
//     return ESP_OK;
// }

// static void set_static_ip(esp_netif_t *netif)
// {
//     if (esp_netif_dhcpc_stop(netif) != ESP_OK) {
//         ESP_LOGE(TAG, "Failed to stop dhcp client");
//         return;
//     }
//     esp_netif_ip_info_t ip;
//     memset(&ip, 0 , sizeof(esp_netif_ip_info_t));
//     ip.ip.addr = ipaddr_addr( STATIC_IP_ADDR );
//     ip.netmask.addr = ipaddr_addr( STATIC_NETMASK_ADDR );
//     ip.gw.addr = ipaddr_addr( STATIC_GW_ADDR );
//     if (esp_netif_set_ip_info(netif, &ip) != ESP_OK) {
//         ESP_LOGE(TAG, "Failed to set ip info");
//         return;
//     }
//     ESP_LOGI(TAG, "Success to set static ip: %s, netmask: %s, gw: %s", STATIC_IP_ADDR, STATIC_NETMASK_ADDR, STATIC_GW_ADDR);
//     ESP_ERROR_CHECK(set_dns_server(netif, ipaddr_addr(MAIN_DNS_SERVER), ESP_NETIF_DNS_MAIN));
//     ESP_ERROR_CHECK(set_dns_server(netif, ipaddr_addr(BACKUP_DNS_SERVER), ESP_NETIF_DNS_BACKUP));
// }

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT )
    {
        ESP_LOGW( TAG , "wifi event" );
        wifi_event_ap_staconnected_t* event;
        switch( event_id )
        {
            case WIFI_EVENT_STA_START:
                esp_wifi_connect();
            break;
            case WIFI_EVENT_STA_CONNECTED:
                ESP_LOGI( TAG , "wifi connected!" );
            break;
            case WIFI_EVENT_STA_DISCONNECTED:
                ESP_LOGW( TAG , "wifi disconnected,connect retry!" );
                xEventGroupSetBits(s_wifi_event_group, WIFI_DISCONNECTED_BIT);
            break;
            case WIFI_EVENT_STA_STOP:
                ESP_LOGW( TAG , "esp wifi stop!" );
                esp_wifi_disconnect();
            break;
            case WIFI_EVENT_AP_STACONNECTED:
                event = (wifi_event_ap_staconnected_t*) event_data;
                ESP_LOGI(TAG, "station "MACSTR" join, AID=%d",MAC2STR(event->mac), event->aid);
            break;
            case WIFI_EVENT_AP_STADISCONNECTED:
                event = (wifi_event_ap_stadisconnected_t*) event_data;
                ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d",MAC2STR(event->mac), event->aid);
            break;
        }
    }else if( event_base == IP_EVENT )
    {
        ESP_LOGW( TAG , "ip event" );
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

void bsp_wifi_init( void )
{
    if( s_wifi_event_group )
        return;

    s_wifi_event_group = xEventGroupCreate();

    esp_err_t ret = nvs_flash_init();
    if( ret != ESP_OK )
    {
        ESP_LOGE( TAG , "nvs init error!" );
    }

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    _sta_net_if = esp_netif_create_default_wifi_sta();
    _ap_net_if = esp_netif_create_default_wifi_ap();
    esp_netif_set_hostname( _sta_net_if , "iot_mecanum_car" );
    esp_netif_set_hostname( _ap_net_if , "iot_mecanum_car" );

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &_instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &_instance_got_ip));
}

int bsp_wifi_connect( char* wifi_name , char* passwd )
{
    if( !s_wifi_event_group )
    {
        ESP_LOGE( TAG , "bsp wifi is not int!" );
        return -1;
    }

    ESP_LOGI( TAG , "ready connect wifi:%s pwd:%s" , wifi_name , passwd );

    esp_wifi_stop();
    xEventGroupClearBits( s_wifi_event_group , WIFI_CONNECTED_BIT | WIFI_DISCONNECTED_BIT );

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

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
            WIFI_CONNECTED_BIT | WIFI_DISCONNECTED_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI( TAG , "connected wifi:%s" , wifi_name );
        return 0;    
    }else if( bits & WIFI_DISCONNECTED_BIT )
    {
        return -1;
    }

    return -1;
}

int bsp_wifi_ap( char* wifi_name , char* passwd )
{
    if( !s_wifi_event_group )
    {
        ESP_LOGE( TAG , "bsp wifi is not int!" );
        return -1;
    }

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config;
    memcpy( wifi_config.ap.ssid , wifi_name , strlen( wifi_name ) + 1 );
    wifi_config.ap.ssid_len = strlen( wifi_name );
    wifi_config.ap.channel = 1;
    memcpy( wifi_config.ap.password , passwd , strlen( passwd ) + 1 );
    wifi_config.ap.max_connection = 1;
    wifi_config.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
    wifi_config.ap.pmf_cfg.required = false;

    if (strlen(passwd) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
             wifi_name, passwd, 1);
    return 0;
}

void bsp_wifi_stop( void )
{
    esp_wifi_stop();
}

