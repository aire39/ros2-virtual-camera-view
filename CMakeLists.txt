cmake_minimum_required(VERSION 3.22)
project (ViewCamera VERSION 0.2.0)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

# ---- Find ROS 2 packages you need ----
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(gazebo_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME}
               ImageSubscriber.hpp
               ImageSubscriberWrapper.hpp
               main.cpp)

# ---- Link your node to ROS 2 libraries ----
ament_target_dependencies(${PROJECT_NAME}
  rclcpp
  sensor_msgs
  geometry_msgs
  gazebo_msgs
  cv_bridge
  OpenCV
)

add_compile_definitions(PROJECT_ROOT="${CMAKE_SOURCE_DIR}")
