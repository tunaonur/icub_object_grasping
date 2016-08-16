
// Author: Onur Tuna

#include <ros/ros.h>
#include <ar_track_alvar_msgs/AlvarMarker.h>
#include <ar_track_alvar_msgs/AlvarMarkers.h>
#include <tf/tf.h>
#include <tf/transform_datatypes.h>
#include "geometry_msgs/Pose.h"



ar_track_alvar_msgs::AlvarMarkers message;
geometry_msgs::Pose objectPose;
//tf::Pose marker_pose;
int messageReceived=0;
void printPose(const ar_track_alvar_msgs::AlvarMarkers msg)
{
  //  ROS_INFO("AR data [%f]", msg->data);

  /*  marker_pose.setOrigin(tf::Vector3(msg->pose.pose.position.x,
                                                 msg->pose.pose.position.y,
                                                 msg->pose.pose.position.z));

*/

  for(int i=0; i<2 ;i++){
    if (msg.markers[i].id!=0){
    objectPose.position.x = msg.markers[i].pose.pose.position.x + 0.49;
    objectPose.position.y =  msg.markers[i].pose.pose.position.y - 0.244659;
    objectPose.position.z =  msg.markers[i].pose.pose.position.z + 0.4;
    objectPose.orientation.x = msg.markers[i].pose.pose.orientation.x;
    objectPose.orientation.y =  msg.markers[i].pose.pose.orientation.y;
    objectPose.orientation.z =  msg.markers[i].pose.pose.orientation.z;
    objectPose.orientation.w =  msg.markers[i].pose.pose.orientation.w;

    messageReceived=1;
    ROS_INFO_STREAM("Message received from /ar_pose_marker!");
  }
}
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "pose_subcriber");

    ros::NodeHandle nh;

    ros::Subscriber* pose_sub=new ros::Subscriber(nh.subscribe("ar_pose_marker",1000,&printPose));

    ros::Publisher pose_pub = nh.advertise<geometry_msgs::Pose>("pose_subscriber", 1000);


    while(ros::ok()){

  if(messageReceived){


      messageReceived=0;



/*  ROS_INFO_STREAM("x_position: " << objectPose.position.x << "y_position: " << objectPose.position.y << "z_position: " << objectPose.position.z << "x_orientation: " <<
       objectPose.orientation.x << "y_orientation " << objectPose.orientation.y << "z_orientation " << objectPose.orientation.z << "w_orientation " << objectPose.orientation.w );*/

      pose_pub.publish(objectPose);
      ROS_INFO_STREAM("Object pose published!");
    }

      ros::spinOnce();

    }

    return 0;

}
