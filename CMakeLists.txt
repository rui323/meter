cmake_minimum_required(VERSION 3.10)
project(meter)

if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

# ライブラリのリンク（TesseractとLeptonica）
find_package(PkgConfig REQUIRED)


set(HEIF_INCLUDE_DIR /usr/include/libheif)
set(HEIF_LIBRARY heif)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${HEIF_INCLUDE_DIR}
    ${EIGEN3_INCLUDE_DIR}
    include
    ${TESSERACT_INCLUDE_DIRS}
    ${LEPTONICA_INCLUDE_DIRS}
)

set(dependencies
    ${OpenCV_LIBS}
    ${HEIF_LIBRARIES}
    ${TESSERACT_LIBRARIES}
    ${LEPTONICA_LIBRARIES}
)


# 実行ファイルを作成
add_executable(tesseract2 src/tesseract.cpp)
target_link_libraries(tesseract2 ${dependencies})

add_executable(line_detect src/line_detect.cpp)
target_link_libraries(line_detect ${dependencies})

add_executable(video src/video_camera.cpp)
target_link_libraries(video ${dependencies})

add_executable(detect src/detect.cpp)
target_link_libraries(detect ${dependencies})