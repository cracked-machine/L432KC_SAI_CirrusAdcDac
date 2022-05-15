target_sources(${BUILD_NAME} PRIVATE
    src/system_device.cpp
    src/gpio.cpp    
    src/basic_timer.cpp
)

target_include_directories(${BUILD_NAME} PRIVATE
    include
)