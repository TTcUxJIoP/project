cmake_minimum_required(VERSION 2.8)
project( DisplayImage )
find_package( OpenCV 2.4.13.7 REQUIRED )
message("libs:${OpenCV_LIBS}")
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( DisplayImage fps.cpp )
target_link_libraries( DisplayImage ${OpenCV_LIBS} )


add_executable(mylib-unit-tests tests.cpp)
include_directories( ${OpenCV_INCLUDE_DIRS} )
target_link_libraries( mylib-unit-tests ${OpenCV_LIBS} )

add_custom_target(check ALL COMMAND mylib-unit-tests)
