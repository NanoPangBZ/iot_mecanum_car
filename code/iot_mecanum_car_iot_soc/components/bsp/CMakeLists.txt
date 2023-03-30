set( BSP_SRCS   "bsp.cpp"
                "./led/SysLed.cpp"
                "./wifi/bsp_wifi.c")

set( BSP_DIRS   "."
                "led"
                "wifi")

idf_component_register(SRCS ${BSP_SRCS}
                    INCLUDE_DIRS ${BSP_DIRS}
                    REQUIRES driver nvs_flash esp_wifi)
