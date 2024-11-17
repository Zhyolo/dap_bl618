cmake_minimum_required(VERSION 3.15)

include(proj.conf)

find_package(bouffalo_sdk REQUIRED HINTS $ENV{BL_SDK_BASE})

target_sources(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_DAP/Firmware/Source/DAP_vendor.c
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_DAP/Firmware/Source/DAP.c
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_DAP/Firmware/Source/JTAG_DP.c
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_DAP/Firmware/Source/SW_DP.c
)


target_sources(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/CherryUSB/core/usbd_core.c
    ${CMAKE_CURRENT_SOURCE_DIR}/CherryUSB/port/bouffalolab/usb_dc_bl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/CherryUSB/class/cdc/usbd_cdc_acm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/CherryUSB/class/hid/usbd_hid.c
)
sdk_add_compile_definitions(-DCONFIG_USB_HS)
sdk_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/CherryUSB/common)
sdk_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/CherryUSB/core)
sdk_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/CherryUSB/class/cdc)
sdk_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/CherryUSB/class/msc)
sdk_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/CherryUSB/class/hid)
    
target_sources(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/letter_shell/src/shell.c
    ${CMAKE_CURRENT_SOURCE_DIR}/letter_shell/src/shell.h
    ${CMAKE_CURRENT_SOURCE_DIR}/letter_shell/src/shell_cmd_list.c
    ${CMAKE_CURRENT_SOURCE_DIR}/letter_shell/src/shell_companion.c
    ${CMAKE_CURRENT_SOURCE_DIR}/letter_shell/src/shell_ext.c
)
sdk_add_compile_definitions(-DSHELL_CFG_USER="shell_config_user.h")
sdk_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/letter_shell/src)

target_sources(app PRIVATE
    winusb_cdc.c
    ring_buffer.c
    shell_task.c
)
sdk_add_include_directories(.)

sdk_set_linker_script(bl616_flash.ld)

sdk_add_compile_definitions(-DCONFIG_BOARD_FLASH_INIT_SKIP)
sdk_set_main_file(main.c)

project(dap)
