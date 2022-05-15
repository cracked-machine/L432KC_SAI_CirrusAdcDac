cmake_minimum_required(VERSION 3.13)

# recommended compiler: gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2
# https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads

SET(PROJECT_DIR L432KCU3_Blinky)

# common variables
set(CMAKE_C_STANDARD    17)
set(CMAKE_CXX_STANDARD  20)

set(ARM_TRIPLET     "arm-none-eabi")
set(X86_TRIPLET     "x86_64-linux-gnu")



# determine which kit was selected by user in VSCode CMake Tools extension
if(${CMAKE_C_COMPILER} MATCHES "(${ARM_TRIPLET})+") 
set(TARGET_TYPE ARM)
elseif(${CMAKE_C_COMPILER} MATCHES "(${X86_TRIPLET})+") 
set(TARGET_TYPE GNU)
else() 
message(FATAL_ERROR "No suitable kit found. Aborting.")
endif()

# load the appropriate toolchain settings (stm32.cmake or linux.cmake)
if(TARGET_TYPE STREQUAL ARM)
    add_compile_definitions(${TARGET} STM32L432xx)
    include(${CMAKE_SOURCE_DIR}/${PROJECT_DIR}/cmake/stm32.cmake)
elseif(TARGET_TYPE STREQUAL GNU)
    include(${CMAKE_SOURCE_DIR}/${PROJECT_DIR}/cmake/linux.cmake)    
endif()



# Declare the project 
project(${PROJECT_DIR} VERSION 0.1 DESCRIPTION ${PROJECT_DIR})

message("*** Selected ${PROJECT_DIR} project ***")

# set the executable target
add_executable(${BUILD_NAME} "")

# add common sources
add_subdirectory(${CMAKE_SOURCE_DIR}/${PROJECT_DIR}/Application)
add_subdirectory(${CMAKE_SOURCE_DIR}/${PROJECT_DIR}/Library)
add_subdirectory(${CMAKE_SOURCE_DIR}/${PROJECT_DIR}/LowLevel)

# add the stm32-specific sources for the embedded build
if(TARGET_TYPE STREQUAL ARM)
    if(CMAKE_BUILD_TYPE STREQUAL Debug)
    endif()
       
    # link in STM32CubeHAL static lib
    # add_subdirectory(extern/stm32_cube_f4)
    # target_link_libraries(${BUILD_NAME} PUBLIC STM32CubeLib)
    # target_include_directories(${BUILD_NAME} PUBLIC
    #     "${PROJECT_BINARY_DIR}"
    #     "${PROJECT_SOURCE_DIR}/stm32cube_workspace")

endif()

remove_definitions(USE_HAL_DRIVER)

# display size info
add_custom_target(size ALL ${CMAKE_SIZE} ${BUILD_NAME} DEPENDS ${BUILD_NAME})

# objcopy the elf file as a hex file, if using STM32
if(DEFINED ${HEX_NAME})
    add_custom_target(build.bin ALL DEPENDS ${BUILD_NAME} COMMAND ${CMAKE_OBJCOPY} -O ihex ${BUILD_NAME} ${HEX_NAME})
endif()
