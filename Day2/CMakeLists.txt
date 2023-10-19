cmake_minimum_required(VERSION 3.25)
project(Day2)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} src/main.cpp src/Image.cpp src/Ray.h src/Sphere.cpp src/Sphere.h src/Film.h src/Camera.h src/Camera.cpp src/Body.h src/Renderer.cpp src/Renderer.h src/Material.h src/Color.cpp)

find_package(Eigen3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Eigen3::Eigen)

find_package(OpenCV CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC opencv_ml opencv_dnn opencv_core opencv_flann opencv_imgcodecs opencv_highgui opencv_photo)

#for OpenMP
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    message("OpenMP found")
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()
