#include "bsp_save.h"
#include "nvs_flash.h"

#include "esp_log.h"

#define TAG "bsp_nvs"
#define NVS_NAMESPACE   "bsp_nvs"

nvs_handle_t bsp_nvs_handle = NULL;

static bool nvs_is_init(){
    if( bsp_nvs_handle == NULL ){
        ESP_LOGW(TAG , "nvs_handle is not init!");
        return false;
    }
    return true;
}

esp_err_t bsp_nvs_init(){
    esp_err_t ret = ESP_FAIL;
    if( nvs_is_init() ) return ESP_OK;
    if( nvs_flash_init() != ESP_OK ){
        ESP_LOGE(TAG,"can`t init nvs");
        return ESP_FAIL;
    }
    if( nvs_open( NVS_NAMESPACE , NVS_READWRITE , &bsp_nvs_handle ) != ESP_OK ){
        ESP_LOGE(TAG,"nvs namespace open error! , nvs namespace : \"%s\" " , NVS_NAMESPACE );
        bsp_nvs_handle = NULL;
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t bsp_nvs_deinit(){
    bsp_nvs_handle = NULL;
    nvs_close( bsp_nvs_handle );
    return nvs_flash_deinit();
}

esp_err_t bsp_nvs_clear(){
    return nvs_flash_erase();
}

esp_err_t bsp_nvs_get(char* key,uint8_t *buf,size_t* len){
    if( !nvs_is_init() ) return false ;
    return nvs_get_blob( bsp_nvs_handle , key , buf , len );
}

esp_err_t bsp_nvs_set(char* key,uint8_t *buf , size_t len){
    if( !nvs_is_init() ) return false ;
    return nvs_set_blob( bsp_nvs_handle , key , buf , len );
}

esp_err_t bsp_nvs_delete(char* key){
    if( !nvs_is_init() ) return false ;
    return nvs_erase_key( bsp_nvs_handle , key );
}

esp_err_t bsp_nvs_check(){
    char *key = "test";
    uint8_t test_value = 0xea;
    uint8_t test_out = 0x00;
    size_t len;
    bsp_nvs_set( key , &test_value , 1 );
    bsp_nvs_get( key , &test_out , &len );
    bsp_nvs_delete( key );
    if( test_value != test_out || len != 1){
        return ESP_FAIL;
    }else{
        return ESP_OK;
    }
}
