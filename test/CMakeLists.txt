set(GTEST_SHUFFLE 1)

add_executable(
    cpp-test
    main.cpp
    test.cpp
    ../app/lanedetector.cpp
    ../app/preProcess.cpp
)

target_include_directories(cpp-test PUBLIC ../vendor/googletest/googletest/include 
											../vendor/googletest/googlemock/include
                                           ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(cpp-test PUBLIC gtest gmock ${OpenCV_LIBS})
