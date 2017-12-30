cmake_minimum_required (VERSION 2.6)
project (recognizeNum)

FIND_PACKAGE( OpenCV REQUIRED )
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

# add the executable
add_executable(recognizeNum recognizeNum.cpp)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OpenCV_LIBS})
