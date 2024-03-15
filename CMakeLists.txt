cmake_minimum_required(VERSION 3.0)
project(e-paper)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(DIR_GUI ./lib)
file(GLOB GUI_SOURCES lib/GUI/*.c)
file(GLOB FONTS_SOURCES lib/Config/*.c)
file(GLOB CONFIG_SOURCES lib/Fonts/*.c)
file(GLOB EPAPER_SOURCES lib/e-Paper/*.c)

add_definitions(-DRPI)

add_executable(epaper
  ${EPAPER_SOURCES}
  ${GUI_SOURCES}
  ${FONTS_SOURCES}
  ${CONFIG_SOURCES}
  main.cpp
)

set(USELIB_RPI USE_LGPIO_LIB)

set(LIB_RPI -Wl,--gc-sections)

if(USELIB_RPI STREQUAL USE_BCM2835_LIB)
    list(APPEND LIB_RPI -lbcm2835)
elseif(USELIB_RPI STREQUAL USE_WIRINGPI_LIB)
    list(APPEND LIB_RPI -lwiringPi)
elseif(USELIB_RPI STREQUAL USE_LGPIO_LIB)
    list(APPEND LIB_RPI -llgpio)
elseif(USELIB_RPI STREQUAL USE_DEV_LIB)
    list(APPEND LIB_RPI -lgpiod)
endif()

add_definitions(-D${USELIB_RPI} -DRPI)

target_include_directories(epaper PRIVATE
  lib/GUI
  lib/Config
  lib/Fonts
  lib/e-Paper
  lib/Examples
)

target_link_libraries(epaper
  gpiod
  ${LIB_RPI}
)

