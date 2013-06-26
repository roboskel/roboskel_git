#include <signal.h>
#include <stdio.h>
#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <vector>
#include "std_msgs/Float64MultiArray.h"
#include <cmath>
//#include "sandbox/robot_skel/teleop_skel/teleop_skel.h"
#include "/home/skel/roboskel_workspace/sandbox/robot_skel/teleop_skel/msg_gen/cpp/include/teleop_skel/teleop_skel.h"

#define MAXSPEED 200
#define FUN_NO 14

double gp_funcs[14] = {0};
double prev_perc_LM = 0;
double prev_perc_RM = 0;
double new_perc_LM = 0;
double new_perc_RM = 0;
double tel_msg_left = 0;
double tel_msg_right = 0;
int i = 0;

void chatterCallback(const std_msgs::Float64MultiArray::ConstPtr& msg)
{
	i = 0;
	// print all the remaining numbers
	for(std::vector<double>::const_iterator it = msg->data.begin(); it != msg->data.end(); ++it)
	{
		//printf("gp_funcs[%d] : %f\n",i, msg->data[i]);
		gp_funcs[i]=msg->data[i];
		i++;
	}
	/*
	//ros::Duration(0.5).sleep();
	if (gp_funcs[0]==-0.000000)
	{
		gp_funcs[0]=0;
	}
	if (gp_funcs[1]==-0.000000)
	{
		gp_funcs[1]=0;
	}
	////////////
	//LEFT MOTOR
	////////////
	if (abs(gp_funcs[0]-prev_perc_LM)<0.025)
	{
		new_perc_LM = prev_perc_LM;
		prev_perc_LM = gp_funcs[0];
	}
	else
	{
		if (gp_funcs[0]>prev_perc_LM)
		{	
			if((prev_perc_LM + 0.025)<=1)
			{
				new_perc_LM = prev_perc_LM + 0.025 ;
			}
			else
			{
				new_perc_LM = 1;
			}
			prev_perc_LM = new_perc_LM;
		}
		else
		{	
			if((prev_perc_LM-0.025)>=-1)
			{
				new_perc_LM =  prev_perc_LM - 0.025 ;
			}
			else
			{
				new_perc_LM = -1;
			}
			prev_perc_LM = new_perc_LM  ;
		}
	}
	//////////////
	//RIGHT MOTOR
	//////////////
	if (abs(gp_funcs[1]-prev_perc_RM)<0.025)
	{
		new_perc_RM = prev_perc_RM;
		prev_perc_RM = gp_funcs[1];
	}
	else
	{
		if (gp_funcs[1]>prev_perc_RM)
		{	
			if((prev_perc_RM + 0.025)<=1)
			{
				new_perc_RM = prev_perc_RM + 0.025 ;
			}
			else
			{
				new_perc_RM = 1;
			}
			prev_perc_RM = new_perc_RM;
		}
		else
		{	
			if((prev_perc_RM-0.025)>=-1)
			{
				new_perc_RM =  prev_perc_RM - 0.025 ;
			}
			else
			{
				new_perc_RM = -1;
			}
			prev_perc_RM = new_perc_RM  ;
		}
	}
	*/
	/*
	printf("*************\n");
	printf("gp_rm %f\n",gp_funcs[1]);
	printf("perc_rm %f \n",new_perc_RM);
	printf("gp_lm %f\n",gp_funcs[0]);
	printf("perc_lm %f\n",new_perc_LM);
	printf("*************\n");
	*/
	//ros::Duration(0.5).sleep();

	

}


int main(int argc, char* argv[])
{
	ros::init(argc, argv, "gp_listener");
	ros::NodeHandle nh;

ros::Rate loop_rate(1000);
ros::Subscriber gp_in =nh.subscribe("gp_functions", 1, chatterCallback);
printf( "Transmitting Gamepad Output\n");
//ros::Publisher teleop_pub = nh.advertise<teleop_skel::teleop_skel>("skel_teleop", 1);
ros::Publisher teleop_pub = nh.advertise<std_msgs::Float64MultiArray>("gp_functions2", 1);

while (ros::ok())
     {
		ros::spinOnce();
		//printf("SPINNED\n");
		std_msgs::Float64MultiArray msg2;
		for (int i = 0; i < FUN_NO; i++)
		{
			msg2.data.push_back(gp_funcs[i]);
		}
		teleop_pub.publish(msg2);
		//teleop_skel::teleop_skel msg;
		//msg.left = MAXSPEED*new_perc_LM;
		//msg.right = MAXSPEED*new_perc_RM;
		//teleop_pub.publish(msg);
		//ros::Duration(2).sleep();
		//printf("SLEPT\n");
	}
printf( "Closing...\n" );
    return 0;

 }
