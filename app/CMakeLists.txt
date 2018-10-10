add_executable(shell-app main.cpp)
TARGET_LINK_LIBRARIES( shell-app ${OpenCV_LIBS} )
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)
