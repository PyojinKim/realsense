/******************************************************************************
 INTEL CORPORATION PROPRIETARY INFORMATION
 This software is supplied under the terms of a license agreement or nondisclosure
 agreement with Intel Corporation and may not be copied or disclosed except in
 accordance with the terms of that agreement
 Copyright(c) 2011-2016 Intel Corporation. All Rights Reserved.
 *******************************************************************************/
#include "gtest/gtest.h"

#include "realsense_camera_test_node.h"
using namespace std;

void imageInfrared1Callback(const sensor_msgs::ImageConstPtr & msg, const sensor_msgs::CameraInfoConstPtr & info_msg)
{
  cv::Mat image = cv_bridge::toCvShare(msg, sensor_msgs::image_encodings::TYPE_8UC1)->image;

  uchar *infrared1_data = image.data;

  long infrared1_total = 0;
  int infrared1_count = 1;
  for (int i = 0; i < msg->height * msg->width; i++)
  {
    if (*infrared1_data > 0 && *infrared1_data < 255)
    {
      infrared1_total += *infrared1_data;
      infrared1_count++;
    }
    infrared1_data++;
  }
  infrared1_avg = infrared1_total / infrared1_count;

  infrared1_encoding_recv = msg->encoding;
  infrared1_width_recv = msg->width;
  infrared1_height_recv = msg->height;
  infrared1_step_recv = msg->step;

  inf1_caminfo_height_recv = info_msg->height;
  inf1_caminfo_width_recv = info_msg->width;
  inf1_dmodel_recv = info_msg->distortion_model;
  infrared1_recv = true;
}

void imageInfrared2Callback(const sensor_msgs::ImageConstPtr & msg, const sensor_msgs::CameraInfoConstPtr & info_msg)
{
  cv::Mat image = cv_bridge::toCvShare(msg, sensor_msgs::image_encodings::TYPE_8UC1)->image;

  uchar *infrared2_data = image.data;

  long infrared2_total = 0;
  int infrared2_count = 1;
  for (int i = 0; i < msg->height * msg->width; i++)
  {
    if (*infrared2_data > 0 && *infrared2_data < 255)
    {
      infrared2_total += *infrared2_data;
      infrared2_count++;
    }
    infrared2_data++;
  }
  infrared2_avg = infrared2_total / infrared2_count;

  infrared2_encoding_recv = msg->encoding;
  infrared2_width_recv = msg->width;
  infrared2_height_recv = msg->height;
  infrared2_step_recv = msg->step;

  inf2_caminfo_height_recv = info_msg->height;
  inf2_caminfo_width_recv = info_msg->width;
  inf2_dmodel_recv = info_msg->distortion_model;
  infrared2_recv = true;
}

void imageDepthCallback(const sensor_msgs::ImageConstPtr & msg, const sensor_msgs::CameraInfoConstPtr & info_msg)
{
  cv::Mat image = cv_bridge::toCvShare(msg, sensor_msgs::image_encodings::TYPE_16UC1)->image;
  uint16_t *image_data = (uint16_t *) image.data;

  long depth_total = 0;
  int depth_count = 0;
  for (int i = 0; i < msg->height * msg->width; ++i)
  {
    if (0 < *image_data <= R200_DEPTH_MAX)
    {
      depth_total += *image_data;
      depth_count++;
    }
    image_data++;
  }
  if (depth_count != 0)
  {
    depth_avg = depth_total / depth_count;
  }

  depth_encoding_recv = msg->encoding;
  depth_height_recv = msg->height;
  depth_width_recv = msg->width;
  depth_step_recv = msg->step;

  depth_caminfo_height_recv = info_msg->height;
  depth_caminfo_width_recv = info_msg->width;
  depth_dmodel_recv = info_msg->distortion_model;

  depth_recv = true;
}

/*
 void pcCallback (const sensor_msgs::PointCloud2ConstPtr pc)
 {
 pcl::PointCloud < pcl::PointXYZRGB > pointcloud;
 pcl::fromROSMsg (*pc, pointcloud);

 long pc_depth_total = 0;
 int pc_depth_count = 0;
 for (int i = 0; i < pointcloud.width * pointcloud.height; ++i)
 {
 pcl::PointXYZRGB point = pointcloud.points[i];
 float pc_depth = (float) std::ceil (point.z);
 if (0 < pc_depth <= 10)
 {
 pc_depth_total += pc_depth;
 pc_depth_count++;
 }
 }


 if (pc_depth_count != 0)
 {
 pc_depth_avg = pc_depth_total / pc_depth_count;
 }

 pc_recv = true;
 }
 */

void imageColorCallback(const sensor_msgs::ImageConstPtr & msg, const sensor_msgs::CameraInfoConstPtr & info_msg)
{
  cv::Mat image = cv_bridge::toCvShare(msg, "rgb8")->image;

  uchar *color_data = image.data;
  long color_total = 0;
  int color_count = 1;
  for (int i = 0; i < msg->height * msg->width * 3; i++)
  {
    if (*color_data > 0 && *color_data < 255)
    {
      color_total += *color_data;
      color_count++;
    }
    color_data++;
  }
  color_avg = color_total / color_count;

  color_encoding_recv = msg->encoding;
  color_height_recv = msg->height;
  color_width_recv = msg->width;
  color_step_recv = msg->step;

  color_caminfo_height_recv = info_msg->height;
  color_caminfo_width_recv = info_msg->width;
  color_dmodel_recv = info_msg->distortion_model;

  color_recv = true;
}

TEST (RealsenseTests, testColorStream)
{
  if (enable_color)
  {
    EXPECT_TRUE (color_avg > 0);
    EXPECT_TRUE (color_recv);

    if (!color_encoding_exp.empty ())
    {
      EXPECT_EQ (color_encoding_exp, color_encoding_recv);
    }
    if (color_step_exp > 0)
    {
      EXPECT_EQ (color_step_exp, color_step_recv);
    }
  }
  else
  {
    EXPECT_FALSE (color_recv);
  }
}

TEST (RealsenseTests, testColorResolution)
{
  if (enable_color)
  {
    if (color_height_exp > 0)
    {
      EXPECT_EQ (color_height_exp, color_height_recv);
    }
    if (color_width_exp > 0)
    {
      EXPECT_EQ (color_width_exp, color_width_recv);
    }
  }
}

TEST (RealsenseTests, testColorCameraInfo)
{
  if (enable_color)
  {
    EXPECT_EQ (color_width_recv, color_caminfo_width_recv);
    EXPECT_EQ (color_height_recv, color_caminfo_height_recv);
    EXPECT_STREQ (color_dmodel_recv.c_str (), "plumb_bob");
  }
}

TEST (RealsenseTests, testDepthStream)
{
  if (enable_depth)
  {
    ROS_INFO_STREAM ("RealSense Camera - depth_avg: " << depth_avg << " mm");
    EXPECT_TRUE (depth_avg > 0);
    EXPECT_TRUE (depth_recv);
    if (!depth_encoding_exp.empty ())
    {
      EXPECT_EQ (depth_encoding_exp, depth_encoding_recv);
    }
    if (depth_step_exp > 0)
    {
      EXPECT_EQ (depth_step_exp, depth_step_recv);
    }
  }
  else
  {
    EXPECT_FALSE (depth_recv);
  }
}

TEST (RealsenseTests, testDepthResolution)
{
  if (enable_depth)
  {
    if (depth_height_exp > 0)
    {
      EXPECT_EQ (depth_height_exp, depth_height_recv);
    }
    if (depth_width_exp > 0)
    {
      EXPECT_EQ (depth_width_exp, depth_width_recv);
    }
  }
}

TEST (RealsenseTests, testDepthCameraInfo)
{
  if (enable_depth)
  {
    EXPECT_EQ (depth_width_recv, depth_caminfo_width_recv);
    EXPECT_EQ (depth_height_recv, depth_caminfo_height_recv);
    EXPECT_STREQ (depth_dmodel_recv.c_str (), "plumb_bob");
  }
}

TEST (RealsenseTests, testInfrared1Stream)
{
  if (enable_depth)
  {
    EXPECT_TRUE (infrared1_avg > 0);
    EXPECT_TRUE (infrared1_recv);
    if (!infrared1_encoding_exp.empty ())
    {
      EXPECT_EQ (infrared1_encoding_exp, infrared1_encoding_recv);
    }
    if (infrared1_step_exp > 0)
    {
      EXPECT_EQ (infrared1_step_exp, infrared1_step_recv);
    }
  }
  else
  {
    EXPECT_FALSE (infrared1_recv);
  }
}

TEST (RealsenseTests, testInfrared1Resolution)
{
  if (enable_depth)
  {
    if (depth_width_exp > 0)
    {
      EXPECT_EQ (depth_width_exp, infrared1_width_recv);
    }
    if (depth_height_exp > 0)
    {
      EXPECT_EQ (depth_height_exp, infrared1_height_recv);
    }
  }
}

TEST (RealsenseTests, testInfrared1CameraInfo)
{
  if (enable_depth)
  {
    EXPECT_EQ (infrared1_width_recv, inf1_caminfo_width_recv);
    EXPECT_EQ (infrared1_height_recv, inf1_caminfo_height_recv);
    EXPECT_STREQ (inf1_dmodel_recv.c_str (), "plumb_bob");
  }
}

TEST (RealsenseTests, testInfrared2Stream)
{
  if (enable_depth)
  {
    EXPECT_TRUE (infrared2_avg > 0);
    EXPECT_TRUE (infrared2_recv);
  }
  else
  {
    EXPECT_FALSE (infrared2_recv);
  }
}

TEST (RealsenseTests, testInfrared2Resolution)
{
  if (enable_depth)
  {
    if (depth_width_exp > 0)
    {
      EXPECT_EQ (depth_width_exp, infrared2_width_recv);
    }
    if (depth_height_exp > 0)
    {
      EXPECT_EQ (depth_height_exp, infrared2_height_recv);
    }
  }
}

TEST (RealsenseTests, testInfrared2CameraInfo)
{
  if (enable_depth)
  {
    EXPECT_EQ (infrared2_width_recv, inf2_caminfo_width_recv);
    EXPECT_EQ (infrared2_height_recv, inf2_caminfo_height_recv);
    EXPECT_STREQ (inf2_dmodel_recv.c_str (), "plumb_bob");
  }
}

/*
 TEST (RealsenseTests, testPointCloud)
 {
 if (enable_depth)
 {
 ROS_INFO_STREAM ("RealSense Camera - pc_depth_avg: " << pc_depth_avg);
 EXPECT_TRUE (pc_depth_avg > 0);
 EXPECT_TRUE (pc_recv);
 }
 else
 {
 EXPECT_FALSE (pc_recv);
 }
 }
 */

TEST (RealsenseTests, testCameraSettings)
{
  stringstream ss (srv.response.configuration_str);
  string item;
  string option_name;

  while (getline (ss, item, ';'))
  {
    stringstream ss1 (item);
    getline (ss1, option_name, ':');
    if (config_args.find (option_name) != config_args.end ())
    {
      std::string last_element (item.substr (item.rfind (":") + 1));
      int expected_value = atoi (last_element.c_str ());
      int current_Value = atoi (config_args.at (option_name).c_str ());
      EXPECT_EQ (current_Value, expected_value);
    }
  }
}

void fillConfigMap(int argc, char **argv)
{
  std::vector < std::string > args;

  for (int i = 1; i < argc; ++i)
  {
    args.push_back(argv[i]);
  }
  while (args.size() > 1)
  {
    config_args[args[0]] = args[1];

    args.erase(args.begin());
    args.erase(args.begin());
  }

  if (argc > 1)
  {
    // Set depth arguments.
    if (config_args.find("enable_depth") != config_args.end())
    {
      ROS_INFO ("RealSense Camera - Setting %s to %s", "enable_depth", config_args.at("enable_depth").c_str());
      enable_depth = atoi(config_args.at("enable_depth").c_str());
    }
    if (config_args.find("depth_encoding") != config_args.end())
    {
      ROS_INFO ("RealSense Camera - Setting %s to %s", "depth_encoding", config_args.at("depth_encoding").c_str());
      depth_encoding_exp = config_args.at("depth_encoding").c_str();
    }
    if (config_args.find("depth_height") != config_args.end())
    {
      ROS_INFO ("RealSense Camera - Setting %s to %s", "depth_height", config_args.at("depth_height").c_str());
      depth_height_exp = atoi(config_args.at("depth_height").c_str());
    }
    if (config_args.find("depth_width") != config_args.end())
    {
      ROS_INFO ("RealSense Camera - Setting %s to %s", "depth_width", config_args.at("depth_width").c_str());
      depth_width_exp = atoi(config_args.at("depth_width").c_str());
    }
    if (config_args.find("depth_step") != config_args.end())
    {
      ROS_INFO ("RealSense Camera - Setting %s to %s", "depth_step", config_args.at("depth_step").c_str());
      depth_step_exp = atoi(config_args.at("depth_step").c_str());
    }

    // Set color arguments.
    if (config_args.find("enable_color") != config_args.end())
    {
      ROS_INFO ("RealSense Camera - Setting %s to %s", "enable_color", config_args.at("enable_color").c_str());
      enable_color = atoi(config_args.at("enable_color").c_str());
    }
    if (config_args.find("color_encoding") != config_args.end())
    {
      ROS_INFO ("RealSense Camera - Setting %s to %s", "color_encoding", config_args.at("color_encoding").c_str());
      color_encoding_exp = config_args.at("color_encoding").c_str();
    }
    if (config_args.find("color_height") != config_args.end())
    {
      ROS_INFO ("RealSense Camera - Setting %s to %s", "color_height", config_args.at("color_height").c_str());
      color_height_exp = atoi(config_args.at("color_height").c_str());
    }
    if (config_args.find("color_width") != config_args.end())
    {
      ROS_INFO ("RealSense Camera - Setting %s to %s", "color_width", config_args.at("color_width").c_str());
      color_width_exp = atoi(config_args.at("color_width").c_str());
    }
    if (config_args.find("color_step") != config_args.end())
    {
      ROS_INFO ("RealSense Camera - Setting %s to %s", "color_step", config_args.at("color_step").c_str());
      color_step_exp = atoi(config_args.at("color_step").c_str());
    }
  }
}

int main(int argc, char **argv)
{
  int return_value = 1;

  testing::InitGoogleTest(&argc, argv);
  ros::init(argc, argv, "utest");
  fillConfigMap(argc, argv);

  ROS_INFO_STREAM ("RealSense Camera - Starting Tests...");

  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  camera_subscriber[0] = it.subscribeCamera(DEPTH_TOPIC, 1, imageDepthCallback, 0);
  camera_subscriber[1] = it.subscribeCamera(COLOR_TOPIC, 1, imageColorCallback, 0);
  camera_subscriber[2] = it.subscribeCamera(IR1_TOPIC, 1, imageInfrared1Callback, 0);
  if (camera.find(R200) != std::string::npos)
  {
    camera_subscriber[3] = it.subscribeCamera(IR2_TOPIC, 1, imageInfrared2Callback, 0);
  }

  //m_sub_pc = nh.subscribe <sensor_msgs::PointCloud2> (PC_TOPIC, 1, pcCallback);
  ros::ServiceClient client = nh.serviceClient < realsense_camera::cameraConfiguration > (SETTINGS_SERVICE);
  client.call(srv);

  ros::Duration duration;
  duration.sec = 10;
  duration.sleep();
  ros::spinOnce();

  return RUN_ALL_TESTS();
}