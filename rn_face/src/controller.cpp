#include <signal.h>
#include <stdio.h>
#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/Float64.h>
#include <vector>
#include "std_msgs/Float64MultiArray.h"
#include <cmath>



void chatterCallback(const sensor_msgs::Joy::ConstPtr& msg)
{
	ROS_INFO("Waiting For Input");
	if(msg->buttons[9]==1)
	{
		ROS_INFO("Giving Start Signal");
		system("/home/skel/start");
		return;
	}
	if(msg->buttons[8]==1)
	{
		ROS_INFO("Giving Stop Signal");
		system("/home/skel/stop");
		system("ps -ef | sed -n '/fdati/{/grep/!p;}' | awk '{print$2}' | xargs -i kill {}");
		system("ps -ef | sed -n '/fdati_crop/{/grep/!p;}' | awk '{print$2}' | xargs -i kill {}");
		system("ps -ef | sed -n '/openni_launch/{/grep/!p;}' | awk '{print$2}' | xargs -i kill {}");
	}
	else if(msg->buttons[0]==1)
	{
		ROS_INFO("Starting Face Recognition & Tracking");
		system("/home/skel/stop");
		system("roslaunch rn_face rec_launch.launch &");
		return;
	}
	else if(msg->buttons[1]==1)
	{
		ROS_INFO("Restarting Openni_launch");
		system("ps -ef | sed -n '/openni_launch/{/grep/!p;}' | awk '{print$2}' | xargs -i kill {}");
		system("killall XnSensorServer");
		system("sleep 5");
		system("roslaunch openni_launch openni.launch &");
		return;
	}
	else if(msg->buttons[2]==1)
	{
		
	}
	else if(msg->buttons[3]==1)
	{
		ROS_INFO("Shutting Down Openni_Launch");
		system("ps -ef | sed -n '/openni_launch/{/grep/!p;}' | awk '{print$2}' | xargs -i kill {}");
		return;
	}
	else if(msg->buttons[7]==1)
	{
		ROS_INFO("Clearing Images");
		system("rm /home/skel/.ros/image*");
		return;
	}
	return;
}

int main(int argc, char* argv[])
{
	ros::init(argc, argv, "controller");
	ros::NodeHandle nh;
	ros::Rate loop_rate(1000);
	ros::Subscriber gp_in =nh.subscribe("joy", 1, chatterCallback);
	//printf( "Transmitting Gamepad Output\n");
	//ros::Publisher teleop_pub = nh.advertise<teleop_skel::teleop_skel>("skel_teleop", 1);
	//ros::Publisher teleop_pub = nh.advertise<std_msgs::Float64MultiArray>("g", 1);
	while (ros::ok())
		{
			ros::spinOnce();
		}
	printf( "Closing...\n" );
	return 0;
}
