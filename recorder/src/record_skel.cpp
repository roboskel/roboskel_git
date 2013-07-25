#include <stdio.h>
#include <time.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <cmath>
#include <ros/ros.h>
#include "std_msgs/String.h"
#include "std_msgs/Float64MultiArray.h"
#include <std_msgs/Float32.h>
#include "std_msgs/Float64.h"
#include <std_msgs/Header.h>
#include <math.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <cmath>
#include <chrono>
#include <ctime>
#define NUM_READINGS 1128;

using namespace std;

std::chrono::time_point<std::chrono::system_clock> p1;

double head_x,head_y,head_z;
double neck_x,neck_y,neck_z;
double torso_x,torso_y,torso_z;

double left_shoulder_x,left_shoulder_y,left_shoulder_z;
double left_elbow_x,left_elbow_y,left_elbow_z;
double left_hand_x,left_hand_y,left_hand_z;
double left_hip_x,left_hip_y,left_hip_z;
double left_knee_x,left_knee_y,left_knee_z;
double left_foot_x,left_foot_y,left_foot_z;

double right_shoulder_x,right_shoulder_y,right_shoulder_z;
double right_elbow_x,right_elbow_y,right_elbow_z;
double right_hand_x,right_hand_y,right_hand_z;
double right_hip_x,right_hip_y,right_hip_z;
double right_knee_x,right_knee_y,right_knee_z;
double right_foot_x,right_foot_y,right_foot_z;


time_t t;

long int timer ;
double seconds ;

ros::Time skel_cur_time, skel_last_time;
ofstream file;

int REC = 0;
std::string pathskel=".rec/skel.txt";
std::string skel_ts;
std::stringstream sstream ;

void chatterCallback(const std_msgs::Float64MultiArray::ConstPtr& msg)
{
    int start = msg->data[11]; //START gia na 3ekinhsei to record
    int cancel = msg->data[4]; //X gia na stamathsei
    //printf("Press Start to Start Recording \n");
    if (start==1)
    {	
		printf("Started Recording Skeleton\n");
        REC = 1;
        ros::Duration(1).sleep();

    }
    else if ((REC==1)&&(cancel==1))
    {
        REC = 0;
        printf("Stopped Recording Skeleton \n");
		ros::shutdown();
    }
}


void callback(const std_msgs::Float64::ConstPtr& msg)
{
    float start = msg->data; //START gia na 3ekinhsei to record
		//X gia na stamathsei
    //printf("Press Start to Start Recording \n");
    if (start==1)
    {	
		//printf("Started Recording Skeleton\n");
        REC = 1;
        ros::Duration(1).sleep();

    }
    else if ((REC==1)&&(start==0))
    {
        REC = 0;
        //printf("Stopped Recording Skeleton \n");
		ros::shutdown();
    }
}

int main (int argc, char** argv)
{	
	//int counter=0;
	file.open ("./skel.txt", ios::out | ios::binary);
	ros::init(argc, argv, "recorder");
	ros::NodeHandle node;
    ros::Subscriber gp_in;
    //gp_in =node.subscribe("gp_functions2", 1, chatterCallback);
    gp_in =node.subscribe("coms",1,callback);
	tf::TransformListener listener;
	tf::Vector3 origin;
	tf::StampedTransform transform;
	skel_last_time = ros::Time::now();
	while(node.ok())
	{
		ros::spinOnce();
		//printf("%d",counter);
		//counter++;
		skel_cur_time=ros::Time::now();
		int i = 1;
		seconds=skel_cur_time.toSec()-skel_last_time.toSec();
		if ((REC==1)&&(seconds>0.1))
		{	
			listener.waitForTransform("/openni_depth_frame", "head_1",ros::Time(0),ros::Duration(30.0));
			printf("STARTED");
			try{
			listener.lookupTransform("/openni_depth_frame", "head_1",
									   ros::Time(0), transform);
			}
			catch (tf::TransformException ex){
			  ROS_ERROR("%s",ex.what());
			}
			origin =transform.getOrigin();
			head_x=origin.x();
			head_y=origin.y();
			head_z=origin.z();

			try{
			listener.lookupTransform("/openni_depth_frame", "neck_1",
									   ros::Time(0), transform);
			}
			catch (tf::TransformException ex){
			  ROS_ERROR("%s",ex.what());
			}
			origin =transform.getOrigin();
			neck_x=origin.x();
			neck_y=origin.y();
			neck_z=origin.z();

			try
			{
			listener.lookupTransform("/openni_depth_frame", "torso_1",
									   ros::Time(0), transform);
			}
			catch (tf::TransformException ex){
			  ROS_ERROR("%s",ex.what());
			}
			origin =transform.getOrigin();
			torso_x=origin.x();
			torso_y=origin.y();
			torso_z=origin.z();

			try
			{
			listener.lookupTransform("/openni_depth_frame", "left_shoulder_1",
									   ros::Time(0), transform);
			}
			catch (tf::TransformException ex){
			  ROS_ERROR("%s",ex.what());
			}
			origin =transform.getOrigin();
			left_shoulder_x=origin.x();
			left_shoulder_y=origin.y();
			left_shoulder_z=origin.z();

			try
			{
			listener.lookupTransform("/openni_depth_frame", "left_elbow_1",
									   ros::Time(0), transform);
			}
			catch (tf::TransformException ex){
			  ROS_ERROR("%s",ex.what());
			}
			origin =transform.getOrigin();
			left_elbow_x=origin.x();
			left_elbow_y=origin.y();
			left_elbow_z=origin.z();

			try
			{
			listener.lookupTransform("/openni_depth_frame", "left_hand_1",
									   ros::Time(0), transform);
			}
			catch (tf::TransformException ex){
			  ROS_ERROR("%s",ex.what());
			}
			origin =transform.getOrigin();
			left_hand_x=origin.x();
			left_hand_y=origin.y();
			left_hand_z=origin.z();

			try
			{
			listener.lookupTransform("/openni_depth_frame", "left_hip_1",
									   ros::Time(0), transform);
			}
			catch (tf::TransformException ex){
			  ROS_ERROR("%s",ex.what());
			}
			origin =transform.getOrigin();
			left_hip_x=origin.x();
			left_hip_y=origin.y();
			left_hip_z=origin.z();

			try
			{
			listener.lookupTransform("/openni_depth_frame", "left_knee_1",
									   ros::Time(0), transform);
			}
			catch (tf::TransformException ex){
			  ROS_ERROR("%s",ex.what());
			}
			origin =transform.getOrigin();
			left_knee_x=origin.x();
			left_knee_y=origin.y();
			left_knee_z=origin.z();

			try
			{
			listener.lookupTransform("/openni_depth_frame", "left_foot_1",
									   ros::Time(0), transform);
			}
			catch (tf::TransformException ex){
			  ROS_ERROR("%s",ex.what());
			}
			origin =transform.getOrigin();
			left_foot_x=origin.x();
			left_foot_y=origin.y();
			left_foot_z=origin.z();


			try
			{
			listener.lookupTransform("/openni_depth_frame", "right_shoulder_1",
									   ros::Time(0), transform);
			}
			catch (tf::TransformException ex){
			  ROS_ERROR("%s",ex.what());
			}
			origin =transform.getOrigin();
			right_shoulder_x=origin.x();
			right_shoulder_y=origin.y();
			right_shoulder_z=origin.z();

			try
			{
			listener.lookupTransform("/openni_depth_frame", "right_elbow_1",
									   ros::Time(0), transform);
			}
			catch (tf::TransformException ex){
			  ROS_ERROR("%s",ex.what());
			}
			origin =transform.getOrigin();
			right_elbow_x=origin.x();
			right_elbow_y=origin.y();
			right_elbow_z=origin.z();

			try
			{
			listener.lookupTransform("/openni_depth_frame", "right_hand_1",
									   ros::Time(0), transform);
			}
			catch (tf::TransformException ex){
			  ROS_ERROR("%s",ex.what());
			}
			origin =transform.getOrigin();
			right_hand_x=origin.x();
			right_hand_y=origin.y();
			right_hand_z=origin.z();

			try
			{
			listener.lookupTransform("/openni_depth_frame", "right_hip_1",
									   ros::Time(0), transform);
			}
			catch (tf::TransformException ex){
			  ROS_ERROR("%s",ex.what());
			}
			origin =transform.getOrigin();
			right_hip_x=origin.x();
			right_hip_y=origin.y();
			right_hip_z=origin.z();

			try
			{
			listener.lookupTransform("/openni_depth_frame", "right_knee_1",
									   ros::Time(0), transform);
			}
			catch (tf::TransformException ex){
			  ROS_ERROR("%s",ex.what());
			}
			origin =transform.getOrigin();
			right_knee_x=origin.x();
			right_knee_y=origin.y();
			right_knee_z=origin.z();

			try
			{
			listener.lookupTransform("/openni_depth_frame", "right_foot_1",
									   ros::Time(0), transform);
			}
			catch (tf::TransformException ex){
			  ROS_ERROR("%s",ex.what());
			}
			origin =transform.getOrigin();
			right_foot_x=origin.x();
			right_foot_y=origin.y();
			right_foot_z=origin.z();
			
			p1 = std::chrono::system_clock::now();
			sstream << std::chrono::duration_cast<std::chrono::milliseconds>(
			p1.time_since_epoch()).count() ;
			sstream>>skel_ts;
			file<<skel_ts<<" ";
			file<<head_x<<" "<<head_y<<" "<<head_z<<" ";
			file<<neck_x<<" "<<neck_y<<" "<<neck_z<<" ";
			file<<torso_x<<" "<<torso_y<<" "<<torso_z<<" ";
			file<<left_shoulder_x<<" "<<left_shoulder_y<<" "<<left_shoulder_z<<" ";
			file<<left_elbow_x<<" "<<left_elbow_y<<" "<<left_elbow_z<<" ";
			file<<left_hand_x<<" "<<left_hand_y<<" "<<left_hand_z<<" ";
			file<<left_hip_x<<" "<<left_hip_y<<" "<<left_hip_z<<" ";
			file<<left_knee_x<<" "<<left_knee_y<<" "<<left_knee_z<<" ";
			file<<left_foot_x<<" "<<left_foot_y<<" "<<left_foot_z<<" ";
			file<<right_shoulder_x<<" "<<right_shoulder_y<<" "<<right_shoulder_z<<" ";
			file<<right_elbow_x<<" "<<right_elbow_y<<" "<<right_elbow_z<<" ";
			file<<right_hand_x<<" "<<right_hand_y<<" "<<right_hand_z<<" ";
			file<<right_hip_x<<" "<<right_hip_y<<" "<<right_hip_z<<" ";
			file<<right_knee_x<<" "<<right_knee_y<<" "<<right_knee_z<<" ";
			file<<right_foot_x<<" "<<right_foot_y<<" "<<right_foot_z<<endl;
			skel_ts=("");
			sstream.str(std::string());
			sstream.clear();
			skel_last_time=skel_cur_time;
			skel_cur_time=ros::Time::now();
		}
		
	}
	file.close();
	return 1;	
		
}
