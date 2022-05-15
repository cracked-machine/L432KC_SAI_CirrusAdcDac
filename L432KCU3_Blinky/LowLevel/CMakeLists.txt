target_sources(${BUILD_NAME} PRIVATE
    src/low_level_handlers.c    
    src/syscalls.c
    src/sysmem.c
    Startup/startup_stm32l432kcux.s
    ${CMAKE_SOURCE_DIR}/STM32CubeL4/Drivers/CMSIS/Device/ST/STM32L4xx/Source/Templates/system_stm32l4xx.c
)

target_include_directories(${BUILD_NAME} PRIVATE
    include
    ${CMAKE_SOURCE_DIR}/STM32CubeL4/Drivers/CMSIS/Device/ST/STM32L4xx/Include
    ${CMAKE_SOURCE_DIR}/STM32CubeL4/Drivers/CMSIS/Core/Include
)