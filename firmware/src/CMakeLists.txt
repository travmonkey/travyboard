cmake_minimum_required(VERSION 3.13)

add_executable(master)

target_sources(master PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/master/master.cpp
    ${CMAKE_CURRENT_LIST_DIR}/rotary_encoder/encoder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/layers/keymap.cpp
    ${CMAKE_CURRENT_LIST_DIR}/oled/oled.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tusb/usb_descriptors.c
)

# Make sure TinyUSB can find tusb_config.h
target_include_directories(master PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/layers
    ${CMAKE_CURRENT_LIST_DIR}/oled
    ${CMAKE_CURRENT_LIST_DIR}/rotary_encoder
    ${CMAKE_CURRENT_LIST_DIR}/tusb
)

add_executable(slave
    ${CMAKE_CURRENT_LIST_DIR}/slave/slave.cpp
)

# target_sources(slave PUBLIC
#     ${CMAKE_CURRENT_LIST_DIR}/slave/slave.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/tusb/usb_descriptors.c
# )

target_include_directories(slave PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    # ${CMAKE_CURRENT_LIST_DIR}/layers
    # ${CMAKE_CURRENT_LIST_DIR}/oled
    # ${CMAKE_CURRENT_LIST_DIR}/rotary_encoder
    # ${CMAKE_CURRENT_LIST_DIR}/tusb
)

pico_enable_stdio_uart(master 1)
pico_enable_stdio_uart(slave 0)
pico_enable_stdio_usb(slave 1)

# example_auto_set_url(slave)

# In addition to pico_stdlib required for common PicoSDK functionality, add dependency on tinyusb_device
# for TinyUSB device support and tinyusb_board for the additional board support library used by the example
target_link_libraries(master PUBLIC pico_stdlib tinyusb_device tinyusb_board hardware_i2c)
target_link_libraries(slave pico_stdlib)

pico_add_extra_outputs(master)
pico_add_extra_outputs(slave)
