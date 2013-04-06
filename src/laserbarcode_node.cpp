/*
 * Copyright (c) 2013, University of Massachusetts Lowell.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of University of Massachusetts Lowell. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* Author: Mikhail Medvedev */

#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>

#include "player/laserbarcode.h"

class LaserBarcodeNode
{
public:
  LaserBarcodeNode();
  ~LaserBarcodeNode();
private:
  Player::LaserBarcode lb_;
  ros::NodeHandle nh_;
  ros::Subscriber sub_laser_;
  ros::Publisher pub_fiducials_;

  void LaserCallback(const sensor_msgs::LaserScanConstPtr & scan);
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "laserbarcode_node");
  LaserBarcodeNode lbn;
  ros::spin();
  return 0;
}

LaserBarcodeNode::LaserBarcodeNode() :
        sub_laser_(nh_.subscribe("scan", 5, &LaserBarcodeNode::LaserCallback, this))
{
  lb_.Setup();
}
LaserBarcodeNode::~LaserBarcodeNode()
{
  lb_.Shutdown();
}

void LaserBarcodeNode::LaserCallback(const sensor_msgs::LaserScanConstPtr & scan)
{
  Player::player_laser_data_t player_scan;
  player_scan.min_angle = scan->angle_min;
  player_scan.max_angle = scan->angle_max;
  player_scan.resolution = scan->angle_increment;
  player_scan.max_range = scan->range_max;
  player_scan.ranges_count = scan->ranges.size();
  player_scan.ranges = (float *)scan->ranges.data();
  player_scan.intensity_count = scan->intensities.size();

  std::vector<uint8_t> intensities;
  for (uint i = 0; i < scan->intensities.size(); i++)
  {
    intensities.push_back(scan->intensities[i]);
  }
  player_scan.intensity =(uint8_t *) &intensities;
  player_scan.id = scan->header.seq;

  lb_.ProcessMessage(&player_scan);
  ROS_INFO("Found %d fiducials.", lb_.data.fiducials_count);
}
