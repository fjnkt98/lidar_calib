#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

const std::string FILEPATH = "/home/adamr2/rplidar_data.csv";

std::ofstream ofs_csv_file(FILEPATH);

int count = 0;

void scanCb(const sensor_msgs::LaserScan::ConstPtr msg) {
  unsigned long t = ros::Time::now().toNSec();

  double front_range_m = msg->ranges[0];
  int front_range_mm = 0;

  if (std::isfinite(front_range_m)) {
    front_range_mm = static_cast<int>(front_range_m * 1000);
    ofs_csv_file << count << "," << t << "," << front_range_mm << std::endl;
    ROS_INFO("index: %d, time: %lu, data: %d", count, t, front_range_mm);
  } else {
    front_range_mm = 6000.0;
    ofs_csv_file << count << "," << t << "," << front_range_mm << std::endl;
    ROS_INFO("index: %d, time: %lu, data: %d", count, t, front_range_mm);
  }

  /*if (std::isfinite(front_range)) {
    ofs_csv_file << std::fixed << std::setprecision(4) << front_range << std::endl;
  } else {
    ofs_csv_file << std::fixed << std::setprecision(4) << msg->range_max << std::endl;
  }*/
  
  count++;
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "rplidar_calib_node");
  ros::NodeHandle nh;
  ros::Subscriber scan_sub = nh.subscribe("scan", 100, scanCb);

  ofs_csv_file << "index,time,data" << std::endl;

  ros::spin();
}