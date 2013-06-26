#include <ros/ros.h>
#include "std_msgs/String.h"
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <signal.h>
#include <stdio.h>
#include <std_msgs/Float64.h>
#include <sstream>
#include "std_msgs/Float64MultiArray.h"
#include <cmath>
 
#define PI 3.14159265
int main(int argc, char** argv){
double w1;
double w2;
ros::init(argc, argv, "head_cor_publisher");
ros::NodeHandle node;
ros::Publisher cord_publ =
    node.advertise<geometry_msgs::Vector3>("head_cords", 1,false);

tf::TransformListener listener;
tf::Vector3 origin;
tf::StampedTransform transform;
geometry_msgs::Vector3 cord;
ros::Rate rate(10.0);

  while (node.ok()){
	ros::spinOnce();


    try{
      listener.lookupTransform("/openni_depth_frame", "head_1",
                               ros::Time(0), transform);
    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
    }

    origin =transform.getOrigin();
    cord.x=origin.x();
    cord.y=origin.y();
    cord.z=origin.z();
    w1=(atan2(cord.y,cord.x)* 180 / PI);
    w2=(atan2(cord.z,cord.x)* 180 / PI);

    printf("*********\n");
    printf("X : %f \n",cord.x);
	printf("Y : %f \n",cord.y);
	printf("Z:  %f \n",cord.z);
	printf("W1:  %f \n",w1);
	printf("W2:  %f \n",w2);
	printf("*********\n");
	cord_publ.publish(cord);
    ros::Duration(1).sleep();
 	
    //rate.sleep();
  }
  return 0;
};
