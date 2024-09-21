cmake_minimum_required(VERSION 3.15)

include(proj.conf)

find_package(bouffalo_sdk REQUIRED HINTS $ENV{BL_SDK_BASE})

target_sources(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_DAP/Firmware/Source/DAP_vendor.c
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_DAP/Firmware/Source/DAP.c
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_DAP/Firmware/Source/JTAG_DP.c
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_DAP/Firmware/Source/SW_DP.c
    ${CMAKE_CURRENT_SOURCE_DIR}/CherryUSB/core/usbd_core.c
    ${CMAKE_CURRENT_SOURCE_DIR}/CherryUSB/port/bouffalolab/usb_dc_bl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/CherryUSB/class/cdc/usbd_cdc_acm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/CherryUSB/class/hid/usbd_hid.c
)

sdk_add_include_directories(.)
# sdk_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_DAP/Firmware/Include)

sdk_add_compile_definitions(-DCONFIG_USB_HS)
sdk_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/CherryUSB/common)
sdk_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/CherryUSB/core)
sdk_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/CherryUSB/class/cdc)
sdk_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/CherryUSB/class/msc)
sdk_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/CherryUSB/class/hid)

sdk_set_main_file(main.c)

project(dap)
