target_sources(${BUILD_NAME} PRIVATE
    src/main.cpp
    src/test_app.cpp
)

target_include_directories(${BUILD_NAME} PRIVATE
    include
)