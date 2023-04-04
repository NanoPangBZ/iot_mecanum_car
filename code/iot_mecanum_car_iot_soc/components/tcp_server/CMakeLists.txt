set( SRCS  tcp_server.cpp)

set( DIRS       ".")

idf_component_register(SRCS ${SRCS}
                    INCLUDE_DIRS ${DIRS}
                    REQUIRES esp_wifi)