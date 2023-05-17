set( SRCS  transmission/scp_trans.c)

set( DIRS       "transmission")

idf_component_register(SRCS ${SRCS}
                    INCLUDE_DIRS ${DIRS} )