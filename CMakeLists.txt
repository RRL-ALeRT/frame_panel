cmake_minimum_required(VERSION 3.5)
project(frame_panel)

find_package(ament_cmake REQUIRED)
find_package(rviz_common REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(pluginlib REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOMOC ON)

add_library(frame_panel SHARED
  src/frame_panel.cpp
  include/frame_panel/frame_panel.hpp          # <--- add header here
)

# make the include directory available when building and when installing
target_include_directories(frame_panel PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(frame_panel
  Qt5::Widgets)

ament_target_dependencies(frame_panel
  rviz_common
  rclcpp
  std_msgs
  pluginlib)

# make the library visible in the ament index
ament_export_libraries(frame_panel)

pluginlib_export_plugin_description_file(
  rviz_common plugin_description.xml)

install(TARGETS frame_panel
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib             # for Windows
)

install(DIRECTORY include/
  DESTINATION include)

install(FILES plugin_description.xml
  DESTINATION share/${PROJECT_NAME})

ament_package()