#include <signal.h>
#include <stdio.h>
#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include </usr/include/ncurses.h>
#include <vector>
#include <sensor_msgs/Joy.h>
#include "std_msgs/Float64MultiArray.h"
#include "/home/skel/roboskel_workspace/sandbox/robot_skel/teleop_skel/msg_gen/cpp/include/teleop_skel/teleop_skel.h"



#define FUN_NO 14

float functions[FUN_NO]={0}; //left & right motor speed 
					//in functions[0], function[1] respectively
					//extra functions in the rest (12 buttons  currently available)
float r_s; //right stick (1=up,-1=down)
float l_s; //left stick  (1=left,-1=right)
float d_hor;//horizontal dpad   (1=up,-1=down)
float d_ver;//vertical dpad (1=left,-1=right)
float max_speed = 100.0; // edw ypoti8etai einai h megisth taxythta poy 8eloyme
				// gia to motori
int ch;

int main(int argc, char* argv[])
{
	initscr();
	timeout(-1);
	//raw();
	keypad(stdscr, TRUE);
	//noecho();
	cbreak();
	
	ros::init(argc, argv, "converter");
	ros::NodeHandle nh;

	ros::Rate loop_rate(1000);
	//ros::Subscriber gp_in =nh.subscribe("joy", 1, chatterCallback);
	ros::Publisher gp_out = nh.advertise<std_msgs::Float64MultiArray>("gp_functions", 1);
	printf( "Advertising Gamepad Output");

	while (ros::ok())
		 {	
			
			while((ch = getch()) != KEY_UP)
			{	
				printf( "loopdyloop");
				if (ch=='w')
				{
					functions[0]=1;
					functions[1]=1;
				}
				else if (ch=='s')
				{
					functions[0]=-1;
					functions[1]=-1;
				}	
				else if ((ch!='s')&&(ch!='w'))
				{
					functions[0]=0;
					functions[1]=0;
				}
				ch='d';
				std_msgs::Float64MultiArray msg;
				//for (int i = 0; i < FUN_NO; i++)
				//{
				//	msg2.data.push_back(functions[i]);
				//}
				msg.data.push_back(functions[0]*200);
				msg.data.push_back(functions[1]*200);
				flushinp();
				gp_out.publish(msg);
				ros::Duration(2).sleep();
				ros::spinOnce();
			}
			nocbreak();
			endwin();
			timeout(0);
			return 0;
			 
			 
	}
	printf( "Closing...\n" );
	return 0;

  }
