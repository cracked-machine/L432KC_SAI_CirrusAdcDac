target_sources(${BUILD_NAME} PRIVATE
    Src/main.cpp
    Src/system_device.cpp
    Src/low_level_handlers.c
    Src/syscalls.c
    Src/sysmem.c
    Src/system_stm32l4xx.c
    Startup/startup_stm32l432kcux.s
    
)

target_include_directories(${BUILD_NAME} PRIVATE
    Inc
    ${CMAKE_SOURCE_DIR}/STM32CubeL4/Drivers/CMSIS/Device/ST/STM32L4xx/Include
    ${CMAKE_SOURCE_DIR}/STM32CubeL4/Drivers/CMSIS/Core/Include
)
