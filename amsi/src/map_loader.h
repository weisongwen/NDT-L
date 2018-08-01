#include <mutex>
#include <memory>
#include <iostream>

#include <ros/ros.h>
#include <pcl_ros/point_cloud.h>
#include <tf_conversions/tf_eigen.h>
#include <tf/transform_broadcaster.h>

#include <nav_msgs/Odometry.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/PointCloud2.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>


#include <pcl/filters/voxel_grid.h>





class map_loader  {
public:
  using PointT = pcl::PointXYZI;

  map_loader(ros::NodeHandle nh_) {
    nh = nh_;

    // read globalmap from a pcd file
    std::string globalmap_pcd = nh.param<std::string>("globalmap_pcd", "/home/wenws/amsipolyu/src/amsi/data/map.pcd");
    globalmap.reset(new pcl::PointCloud<PointT>());
    pcl::io::loadPCDFile(globalmap_pcd, *globalmap);
    globalmap->header.frame_id = "map";

    // downsample globalmap
    double downsample_resolution = nh.param<double>("downsample_resolution", 0.1);
    boost::shared_ptr<pcl::VoxelGrid<PointT>> voxelgrid(new pcl::VoxelGrid<PointT>());
    voxelgrid->setLeafSize(downsample_resolution, downsample_resolution, downsample_resolution);
    voxelgrid->setInputCloud(globalmap);

    pcl::PointCloud<PointT>::Ptr filtered(new pcl::PointCloud<PointT>());
    voxelgrid->filter(*filtered);

    globalmap = filtered;

    // publish globalmap with "latched" publisher
    globalmap_pub = nh.advertise<sensor_msgs::PointCloud2>("/points_map", 5, true);
    globalmap_pub.publish(globalmap);
    std::cout<<"load map....";
  }
   ~map_loader() {
  }




private:
  // ROS
  ros::NodeHandle nh;
  ros::NodeHandle mt_nh;
  ros::NodeHandle private_nh;

  ros::Publisher globalmap_pub;

  pcl::PointCloud<PointT>::Ptr globalmap;
};

