#include <stdio.h>
#include <time.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <iterator>
#include <vector>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float64.h>

#include <std_msgs/Header.h>
#include <math.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <cmath>
#include <chrono>
#include <ctime>
#include "rn_face/head_cords_m.h"
#include "rn_face/head_cords_m_array.h"
#include "rn_face/hcm.h"
#include "rn_face/hcma.h"

#define NUM_READINGS 1128;
using namespace std;

std::chrono::time_point<std::chrono::system_clock> p1;
time_t t;
int result;

long int timer ;
double seconds ;
char number[10], id[2], x[10], y[10], z[10];
char* pEnd;
ros::Time skel_cur_time, skel_last_time;
//ofstream ofile;
//ifstream ifile;
//char filename[]="sticks.data";
char filename[] = "_skel.data";
FILE *ofile = fopen(filename, "r");
int rec = 1 ;
int REC = 0 ;
std::string pathskel=".rec/skel_data.txt";
std::string head ="head_";

std::string skel_ts;
std::stringstream sstream ;
std::stringstream sstream2 ;

void callback(const std_msgs::Float64::ConstPtr& msg, FILE* ofile)
{
    float start = msg->data; //START gia na 3ekinhsei to record
		//X gia na stamathsei
    //printf("Press Start to Start Recording \n");
    if (start==1)
    {	
		//printf("Started Recording Skeleton\n");
        REC = 1;
        ROS_INFO("STARTED");
       //ros::Duration(1).sleep();

    }
    else if ((REC==1)&&(start==0))
    {
        REC = 0;
        //printf("Stopped Recording Skeleton \n");
		ros::shutdown();
    }
    
    int counter = 0;
			ros::NodeHandle node_;
			ros::Publisher gp_out2 = node_.advertise<std_msgs::Float64>("coms",1);
			ros::Publisher skel_dat =node_.advertise<rn_face::head_cords_m_array>("head_cords",1);
			rn_face::head_cords_m_array head_ar;
			//rn_face::hcma head_ar;
			if(REC==1)
			{	
				char line[5000];
				while ((counter<6)&&(fgets(line, sizeof(line), ofile) != NULL)) 
				{	
					if(strlen(line)>40)
					{
						if (counter<6) 
						{
							rn_face::head_cords_m msg;
							//rn_face::hcm msg;
							ROS_INFO("Counter : %d ",counter);
							sscanf(line, "%s %s %s %s %s", number, id, x, y, z);
							ROS_INFO(" %s %s %s %s %s",number,id,x,y,z);
							
							msg.timestamp=stol(number);
							msg.id=stoi(id);
							msg.x=strtod(x,&pEnd);
							msg.y=strtod(y,&pEnd);
							msg.z=strtod(z,NULL);
							//msg.x=stod(x);
							//msg.y=stod(y);
							//msg.z=stod(z);
							/*
							msg.timestamp=stol(number);
							msg.id=stoi(id);
							msg.x=x;
							msg.y=y;
							msg.z=z;
							*/
							head_ar.head_co.push_back(msg);
							counter++;
							
						}
						else
						{	
							ROS_INFO("RESETING");
							counter=0;
							break;
							//skel_dat.publish(head_ar);
							ros::Duration(0.1).sleep();
							ROS_INFO("Counter : %d ",counter);
						}
					}
					else 
					{
						if (counter<6) 
						{	
							ROS_INFO("Counter : %d ",counter);
							ROS_INFO(" 0 0 0 0 0");
							rn_face::head_cords_m msg;
							msg.timestamp=0;
							msg.id=0;
							msg.x=0;
							msg.y=0;
							msg.z=0;
							head_ar.head_co.push_back(msg);
							counter++;
						}
						else
						{	
							ROS_INFO("RESETING");
							counter=0;
							break ;
							//skel_dat.publish(head_ar);
							ros::Duration(0.1).sleep();
							ROS_INFO("Counter : %d ",counter);
						}
					}
					
				//skel_dat.publish(head_ar);

				}
			skel_dat.publish(head_ar);

			}
			//skel_dat.publish(head_ar);
			std_msgs::Float64 msg2;
			msg2.data=1;
}

//void addr_cb(const std_msgs::Int32::ConstPtr& msg)
//{			
			
			
			
//}
int main (int argc, char** argv)
{	
	//float /*number,*/x,y,z;
	//long int number;
	//int id;
	ros::init(argc, argv, "test_transmitter");
	ros::NodeHandle node;
	ros::Publisher skel_dat,gp_out;
	ros::Subscriber gp_in;
	ros::Subscriber address_out;
	gp_in = node.subscribe<std_msgs::Float64>("coms",5,boost::bind(callback, _1,ofile));
	//address_out = node.subscribe("coms",5,boost::bind(addr_cb, _1,ofile));
	gp_out = node.advertise<std_msgs::Float64>("coms",1);
	std_msgs::Float64 msg2;
	msg2.data=1;
	
	int counter=0;
	skel_dat = node.advertise<rn_face::head_cords_m_array>("head_cords",1);
	//ofile.open ("./_skel.data", ios::out | ios::binary);
	//if (ofile != NULL) 
	//{   
		ROS_INFO("#1");
		while(ros::ok())
		{
			
				
					//fclose(ofile);
					//fclose(write);
			ros::spinOnce();		//ros::shutdown();
			}
			
		//}
	
	//else
	//{
	//	perror(filename);
	//}
	return 0;
}
