set( SRCS   "tcp_client.cpp" )

set( DIRS       ".")

idf_component_register(SRCS ${SRCS}
                    INCLUDE_DIRS ${DIRS}
                    REQUIRES esp_wifi)
