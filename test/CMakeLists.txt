set( CMAKE_CXX_FLAGS "-std=c++11" )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( a a.cpp )
target_link_libraries( a ${OpenCV_LIBS} )

add_executable( videotoframe videotoframe.cpp )
target_link_libraries( videotoframe ${OpenCV_LIBS} )

add_executable( rectificationtime rectificationtime.cpp )
target_link_libraries( rectificationtime ${OpenCV_LIBS} )

add_executable( testparams testparams.cpp )
target_link_libraries( testparams ${OpenCV_LIBS} )
