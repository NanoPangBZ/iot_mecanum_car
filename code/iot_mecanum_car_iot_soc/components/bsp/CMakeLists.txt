set( BSP_SRCS   "bsp.cpp"
                "./led/SysLed.cpp"
                "./wifi/bsp_wifi.cpp"
                "./save/bsp_save.cpp")

set( BSP_DIRS   "."
                "led"
                "wifi"
                "save")

idf_component_register(SRCS ${BSP_SRCS}
                    INCLUDE_DIRS ${BSP_DIRS}
                    REQUIRES driver nvs_flash )
