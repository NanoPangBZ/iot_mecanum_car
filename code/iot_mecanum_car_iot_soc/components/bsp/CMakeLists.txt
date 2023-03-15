set( BSP_SRCS   "bsp.cpp"
                "./led/SysLed.cpp")

set( BSP_DIRS   "."
                "led")

idf_component_register(SRCS ${BSP_SRCS}
                    INCLUDE_DIRS ${BSP_DIRS})
