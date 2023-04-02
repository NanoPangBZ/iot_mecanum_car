set( SRCS   "tcp_server.c" )

set( DIRS       ".")

idf_component_register(SRCS ${SRCS}
                    INCLUDE_DIRS ${DIRS}
                    REQUIRES esp_wifi)