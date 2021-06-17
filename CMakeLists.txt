# cmake_minimum_required(VERSION 2.8) 
# PROJECT (name)
# find_package(OpenCV REQUIRED )
# set( NAME_SRC
#     fps.cpp    
#     qwe.cpp
#     )



# INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/include )
# link_directories( ${CMAKE_BINARY_DIR}/bin)
# set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
# add_executable( name ${NAME_SRC} ${NAME_HEADERS} )

# target_link_libraries( name ${OpenCV_LIBS} )

cmake_minimum_required(VERSION 2.8)
project( DisplayImage )
find_package( OpenCV 2.4.13.7 REQUIRED )
message("libs:${OpenCV_LIBS}")
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( DisplayImage fps.cpp )
target_link_libraries( DisplayImage ${OpenCV_LIBS} )