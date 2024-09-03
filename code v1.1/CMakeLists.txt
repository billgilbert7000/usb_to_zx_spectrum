cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Pull in Raspberry Pi Pico SDK
include(pico_sdk_import.cmake)


if (PICO_SDK_VERSION_STRING VERSION_LESS "1.5.1")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.5.1 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()


# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

project(usb_to_zx_key C CXX ASM)



add_executable(
  usb_to_zx_key 
usb_to_zx_key.c 
scancode_zx.c 
ws2812.c 
)


add_compile_definitions(DEBUG=0)

pico_set_program_name(usb_to_zx_key "usb_to_zx_key")
pico_set_program_version(usb_to_zx_key "0.1")

pico_enable_stdio_uart(usb_to_zx_key 1)
pico_enable_stdio_usb(usb_to_zx_key 0)

target_include_directories(usb_to_zx_key PUBLIC ${CMAKE_CURRENT_LIST_DIR})
#target_link_libraries(usb_to_zx_key pico_stdlib hardware_pio tinyusb_host tinyusb_board)

target_link_libraries(usb_to_zx_key 


        pico_runtime
        pico_stdlib
        hardware_pio
        tinyusb_host 
        tinyusb_board
        pico_multicore
        hardware_flash
)



pico_add_extra_outputs(usb_to_zx_key)


