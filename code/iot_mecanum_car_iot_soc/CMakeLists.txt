# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/examples/common_components/iot_mecanum_car_iot_soc)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(iot_mecanum_car_iot_soc)
