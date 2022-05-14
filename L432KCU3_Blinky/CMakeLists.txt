target_sources(${BUILD_NAME} PRIVATE
    Src/main.c
    Src/syscalls.c
    Src/sysmem.c
    Startup/startup_stm32l432kcux.s
)

target_include_directories(${BUILD_NAME} PRIVATE
    Inc
    ${CMAKE_SOURCE_DIR}/STM32CubeL4/Drivers/CMSIS/Device/ST/STM32L4xx/Include
    ${CMAKE_SOURCE_DIR}/STM32CubeL4/Drivers/CMSIS/Core/Include
)
