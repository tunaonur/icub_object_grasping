
// Author: Onur Tuna

#include <ros/ros.h>
#include <iostream>
#include <fstream>
#include <ar_track_alvar_msgs/AlvarMarker.h>
#include <ar_track_alvar_msgs/AlvarMarkers.h>
#include <tf/tf.h>
#include <tf/transform_listener.h>
#include <tf/transform_datatypes.h>
#include "geometry_msgs/Pose.h"
#include "visualization_msgs/Marker.h"
#include "visualization_msgs/MarkerArray.h"

using namespace std;

ar_track_alvar_msgs::AlvarMarkers message;
geometry_msgs::Pose objectPose;
//tf::Pose marker_pose;
//int messageReceived=0;
void printPose(const ar_track_alvar_msgs::AlvarMarkers msg)
{
  //  ROS_INFO("AR data [%f]", msg->data);

  /*  marker_pose.setOrigin(tf::Vector3(msg->pose.pose.position.x,
                                                 msg->pose.pose.position.y,
                                                 msg->pose.pose.position.z));

*/

// string target_frame = "/base_link"; // right hand
// string ref_frame = "/ar_marker_4"; // torso base
//
// tf::TransformListener listener;
//
// //std::stringstream ss;
// ss << count;
// tf::StampedTransform transform;
// try{
//   listener.lookupTransform(ref_frame, target_frame, ros::Time(0), transform);
// }
// catch (tf::TransformException &ex) {
//   ROS_ERROR("%s",ex.what());
//   ros::Duration(1.0).sleep();
//   continue;
// }

ROS_INFO_STREAM("I am at CallBack!");


}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "pose_subcriber");

    ros::NodeHandle nh;

  //  ros::Subscriber* pose_sub=new ros::Subscriber(nh.subscribe("ar_pose_marker",1000,&printPose));

    ros::Publisher pose_pub = nh.advertise<geometry_msgs::Pose>("pose_subscriber", 1000);

    ros::Duration sleeper(0.1);
    ros::Duration wait(3.0);


    while(ros::ok()){


      string target_frame = "/ar_marker_4"; // right hand
      string ref_frame = "/base_link"; // torso base

      tf::TransformListener listener;

      //std::stringstream ss;
      //ss << count;
      tf::StampedTransform transform;
      try{
        listener.waitForTransform(ref_frame, target_frame, ros::Time(0), wait);
        listener.lookupTransform(ref_frame, target_frame, ros::Time(0), transform);
      }
      catch (tf::TransformException &ex) {
        ROS_ERROR("%s",ex.what());
        ros::Duration(1.0).sleep();
        continue;
      }

//  if(messageReceived){

  //  for(int i=0; i<2 ;i++){
//      if (message.markers[i].id!=0){
      objectPose.position.x = transform.getOrigin().x();
      objectPose.position.y =  transform.getOrigin().y();
      objectPose.position.z =  transform.getOrigin().z();
      objectPose.orientation.x = transform.getRotation().x();
      objectPose.orientation.y =  transform.getRotation().y();
      objectPose.orientation.z =  transform.getRotation().z();
      objectPose.orientation.w =  transform.getRotation().w();

//      messageReceived=1;
//      ROS_INFO_STREAM("Message received from /ar_pose_marker!");
//    }
//  }

//      messageReceived=0;



/*  ROS_INFO_STREAM("x_position: " << objectPose.position.x << "y_position: " << objectPose.position.y << "z_position: " << objectPose.position.z << "x_orientation: " <<
       objectPose.orientation.x << "y_orientation " << objectPose.orientation.y << "z_orientation " << objectPose.orientation.z << "w_orientation " << objectPose.orientation.w );*/

      pose_pub.publish(objectPose);

      ROS_INFO_STREAM("Object pose published!");
      sleeper.sleep();
//    }

    //  ros::spinOnce();

    }

    return 0;

}
