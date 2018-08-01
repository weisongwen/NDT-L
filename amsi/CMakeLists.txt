cmake_minimum_required(VERSION 2.8.3)
project(amsi)
execute_process(
  COMMAND rosversion -d
  OUTPUT_VARIABLE ROS_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

find_package(PCL REQUIRED)

IF(NOT (PCL_VERSION VERSION_LESS "1.7.2"))
SET(FAST_PCL_PACKAGES filters registration)
ENDIF(NOT (PCL_VERSION VERSION_LESS "1.7.2"))

find_package( OpenMP )
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()
if ("${ROS_VERSION}" MATCHES "(indigo|jade)")
find_package(catkin REQUIRED COMPONENTS
  roscpp
  pcl_ros
  sensor_msgs
  pcl_conversions
  velodyne_pointcloud
  message_generation
  ${FAST_PCL_PACKAGES}
)
elseif("${ROS_VERSION}" MATCHES "(kinetic)")
find_package(catkin REQUIRED COMPONENTS
  roscpp
  pcl_ros
  sensor_msgs
  pcl_conversions
  runtime_manager
  velodyne_pointcloud
  message_generation
  ${FAST_PCL_PACKAGES}
)
endif()
add_message_files(FILES ndt_stat.msg)

generate_messages(
  DEPENDENCIES
  std_msgs
)
###################################
## catkin specific configuration ##
###################################
if ("${ROS_VERSION}" MATCHES "(indigo|jade)")
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES ndt_pcl
  CATKIN_DEPENDS runtime_manager message_runtime std_msgs velodyne_pointcloud ${FAST_PCL_PACKAGES}
  DEPENDS ndt_tku
#  DEPENDS system_lib
)
elseif("${ROS_VERSION}" MATCHES "(kinetic)")
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES ndt_pcl
  CATKIN_DEPENDS runtime_manager message_runtime std_msgs velodyne_pointcloud ${FAST_PCL_PACKAGES}
#  DEPENDS ndt_tku
#  DEPENDS system_lib
)
endif()

###########
## Build ##
###########

include_directories(include ${catkin_INCLUDE_DIRS})

IF(PCL_VERSION VERSION_LESS "1.7.2")
SET(CMAKE_CXX_FLAGS "-std=c++11 -O2 -g -Wall ${CMAKE_CXX_FLAGS}")
ELSE(PCL_VERSION VERSION_LESS "1.7.2")
SET(CMAKE_CXX_FLAGS "-std=c++11 -O2 -g -Wall -DUSE_FAST_PCL ${CMAKE_CXX_FLAGS}")
ENDIF(PCL_VERSION VERSION_LESS "1.7.2")

# node: ndt matching
add_executable(amsi src/amsi.cpp)
target_link_libraries(amsi ${catkin_LIBRARIES})
add_dependencies(amsi amsi_generate_messages_cpp)

# node: map loader
add_executable(map_loaderNode src/map_loaderNode.cpp)
target_link_libraries(map_loaderNode ${catkin_LIBRARIES})
add_dependencies(map_loaderNode map_loaderNode_generate_messages_cpp)
