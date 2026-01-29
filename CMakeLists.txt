cmake_minimum_required(VERSION 3.13)

# IMPORTANT: set board before importing the SDK
set(PICO_BOARD pico_w CACHE STRING "Board type")

set(TARGET_NAME wokwi_project)

include(pico_sdk_import.cmake)

project(${TARGET_NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(${TARGET_NAME}
    src/main.c
    src/vote_manager.c
    src/wifi_driver.c
    src/button_driver.c
    src/lcd_driver.c
    src/thingspeak_driver.c
)

target_include_directories(${TARGET_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${CMAKE_CURRENT_LIST_DIR}
)

# Make output names match wokwi.toml
set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "wokwi_project")

pico_enable_stdio_usb(${TARGET_NAME} 1)
pico_enable_stdio_uart(${TARGET_NAME} 0)

target_link_libraries(${TARGET_NAME}
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    hardware_i2c
)

pico_add_extra_outputs(${TARGET_NAME})

# ----------------------------
# Export (copy) firmware files
# ----------------------------

# Respect OUTPUT_NAME if set
get_target_property(_outname ${TARGET_NAME} OUTPUT_NAME)
if(NOT _outname)
    set(_outname ${TARGET_NAME})
endif()

# Pico SDK writes these into the build dir
set(_uf2_in "${CMAKE_CURRENT_BINARY_DIR}/${_outname}.uf2")

# Where to copy it (project root here)
set(_export_dir "${CMAKE_CURRENT_LIST_DIR}")
set(_uf2_out   "${_export_dir}/firmware.uf2")

# Create an export rule that depends on the UF2 *file*
add_custom_command(
    OUTPUT "${_uf2_out}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_uf2_in}" "${_uf2_out}"
    DEPENDS "${_uf2_in}"
    COMMENT "Exporting UF2 to ${_uf2_out}"
    VERBATIM
)

add_custom_target(export_firmware ALL DEPENDS "${_uf2_out}")