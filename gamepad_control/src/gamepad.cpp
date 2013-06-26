#include <signal.h>
#include <stdio.h>
#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <vector>
#include <sensor_msgs/Joy.h>
#include "std_msgs/Float64MultiArray.h"


#define FUN_NO 14

float functions[FUN_NO]={}; //left & right motor speed 
					//in functions[0], function[1] respectively
					//extra functions in the rest (12 buttons  currently available)
float r_s; //right stick (1=up,-1=down)
float l_s; //left stick  (1=left,-1=right)
float d_hor;//horizontal dpad   (1=up,-1=down)
float d_ver;//vertical dpad (1=left,-1=right)
float max_speed = 1; // edw ypoti8etai einai h megisth taxythta poy 8eloyme
				// gia to motori




void chatterCallback(const sensor_msgs::Joy::ConstPtr& msg)
{
	//eisodoi gia ta analogika velakia
	//exoyn proteraiothta gia na dwsoyn kinhsh dld
	//apenergopoioun ta sticks
	d_hor=msg->axes[5];
	d_ver=msg->axes[6];
	if ((d_hor==0.0)&&(d_ver==0.0))
	{
		//taxythta aristeroy moter
		functions[0]=(msg->axes[1])*max_speed;
		//taxythta de3ioy moter
		functions[1]=(msg->axes[3])*max_speed;
	}
	else if (d_ver!=0)//proteraiothta sthn kinhsh mpros/pisw an tyxei kai
						//path8oun parapanw apo ena plhktra toy dpad taftoxrona
	{
		//idia taxythta kai ta 2 moter
		functions[0]=max_speed*d_ver;
		functions[1]=max_speed*d_ver;
	}
	else if (d_hor>0)
	{
		//epi topoy strofh aristera
		functions[0]=-max_speed;
		functions[1]=max_speed;
	}
	else if (d_hor<0)
	{
		//epitopoy strofh de3ia
		functions[0]=max_speed;
		functions[1]=-max_speed;
	}
	//Use buttons for extra functions (emergency brake, etc)
	//when implemented
	for (int i=0;i<12;i++)
	{
		functions[i+2]=msg->buttons[i];
	}
	
}




int main(int argc, char* argv[])
{
	ros::init(argc, argv, "converter");
	ros::NodeHandle nh;

ros::Rate loop_rate(1000);
ros::Subscriber gp_in =nh.subscribe("joy", 1, chatterCallback);
ros::Publisher gp_out = nh.advertise<std_msgs::Float64MultiArray>("gp_functions", 1);
printf( "Advertising Gamepad Output");

while (ros::ok())
{	
	std_msgs::Float64MultiArray msg2;
	for (int i = 0; i < FUN_NO; i++)
		{
			msg2.data.push_back(functions[i]);
		}
	gp_out.publish(msg2);
	ros::spinOnce();
		 
		 
}
printf( "Closing...\n" );
    return 0;

  }
