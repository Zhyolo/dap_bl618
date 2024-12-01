cmake_minimum_required(VERSION 3.15)

include(proj.conf)

find_package(bouffalo_sdk REQUIRED HINTS $ENV{BL_SDK_BASE})

# CMSIS_DAP
target_sources(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_DAP/Firmware/Source/DAP_vendor.c
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_DAP/Firmware/Source/DAP.c
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_DAP/Firmware/Source/JTAG_DP.c
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS_DAP/Firmware/Source/SW_DP.c
)

# CherryUSB
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

# letter_shell
target_sources(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/letter_shell/src/shell.c
    ${CMAKE_CURRENT_SOURCE_DIR}/letter_shell/src/shell_cmd_list.c
    ${CMAKE_CURRENT_SOURCE_DIR}/letter_shell/src/shell_companion.c
    ${CMAKE_CURRENT_SOURCE_DIR}/letter_shell/src/shell_ext.c
)
sdk_add_compile_definitions(-DSHELL_CFG_USER="shell_config_user.h")
sdk_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/letter_shell/src)

# lua
target_sources(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/lapi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/lauxlib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/lbaselib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/lcode.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/lcorolib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/lctype.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/ldblib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/ldebug.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/ldo.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/ldump.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/lfunc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/lgc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/linit.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/liolib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/llex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/lmathlib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/lmem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/loadlib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/lobject.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/lopcodes.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/loslib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/lparser.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/lstate.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/lstring.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/lstrlib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/ltable.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/ltablib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/ltm.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/lua.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/luac.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/lundump.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/lutf8lib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/lvm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lua/src/lzio.c
)
sdk_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lua/src)

target_sources(app PRIVATE
    cdc_trans.c
    lua_cmd.c
    fatfs_cmd.c
    sd_card.c
    winusb_cdc.c
    ring_buffer.c
    shell_task.c
)
sdk_add_include_directories(.)

sdk_set_linker_script(bl616_flash.ld)

sdk_add_compile_definitions(-DCONFIG_BOARD_FLASH_INIT_SKIP)
sdk_set_main_file(main.c)

project(dap)
