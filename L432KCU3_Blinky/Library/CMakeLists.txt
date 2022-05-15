target_sources(${BUILD_NAME} PRIVATE
    src/system_device.cpp
    src/gpio.cpp    
    src/basic_timer.cpp
    src/general_timer_16bit.cpp
    src/general_timer_32bit.cpp
    src/advanced_timer.cpp
)

target_include_directories(${BUILD_NAME} PRIVATE
    include
)