#include <stdio.h>
#include <time.h>
#include <iostream>
#include <sstream>
#include <fstream>
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


#define NUM_READINGS 1128;

std::chrono::time_point<std::chrono::system_clock> p1;
using namespace std;
time_t t;

long int timer ;
double seconds ;

ros::Time skel_cur_time, skel_last_time;
ofstream file;
int rec = 1 ;
int REC = 0 ;
std::string pathskel=".rec/skel_data.txt";
std::string head ="head_";

std::string skel_ts;
std::stringstream sstream ;
std::stringstream sstream2 ;


struct head_cords 
{
	double x[6];
	double y[6];
	double z[6];
	double x_prev[6];
	double y_prev[6];
	double z_prev[6];
	head_cords(): x({0,0,0,0,0,0}),
				y({0,0,0,0,0,0}),
				z({0,0,0,0,0,0}),
				x_prev({0,0,0,0,0,0}),
				y_prev({0,0,0,0,0,0}),
				z_prev({0,0,0,0,0,0}){}
};

void callback(const std_msgs::Float64::ConstPtr& msg)
{
    float start = msg->data; //START gia na 3ekinhsei to record
		//X gia na stamathsei
    //printf("Press Start to Start Recording \n");
    if (start==1)
    {	
		//printf("Started Recording Skeleton\n");
        REC = 1;
       //ros::Duration(1).sleep();

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
	ros::init(argc, argv, "head_cords_recorder");
	ros::NodeHandle node;
	ros::Subscriber gp_in;
	ros::Publisher skel_dat;
	gp_in =node.subscribe("coms",5,callback);
	skel_dat = node.advertise<rn_face::head_cords_m>("head_cords",1);
	file.open ("./_skel2.data", ios::out | ios::binary);
	
	tf::TransformListener listener;
	tf::Vector3 origin;
	tf::StampedTransform transform;
	geometry_msgs::Vector3 cord;
	ros::Rate rate(10.0);
	
	while (node.ok())
  {
	head_cords heads ;
	ros::spinOnce();
	skel_cur_time=ros::Time::now();
	seconds=skel_cur_time.toSec()-skel_last_time.toSec();
	if ((REC==1)&&(seconds>0.1))
	{	
		rn_face::head_cords_m_array msg_ar;
		p1 = std::chrono::system_clock::now();
		sstream2 << std::chrono::duration_cast<std::chrono::milliseconds>(p1.time_since_epoch()).count() ;
		sstream2>>skel_ts;
		for(int i=1;i<=6;i++)
		{
			sstream << i;
			head=head+sstream.str();
			rn_face::head_cords_m msg;
			try{
			  listener.lookupTransform("/openni_depth_frame", head,
									   ros::Time(0), transform);
				origin =transform.getOrigin();
				heads.x[i-1]=origin.x();
				heads.y[i-1]=origin.y();
				heads.z[i-1]=origin.z();
				
				
				file<<skel_ts<<" "<<i<<" ";
				
				if ((cord.x!=heads.x_prev[i-1])&&(cord.y!=heads.y_prev[i-1])&&(cord.z!=heads.z_prev[i-1]))
				{
					rec =1;
					heads.x_prev[i-1] = heads.x[i-1];
					heads.y_prev[i-1] = heads.y[i-1];
					heads.z_prev[i-1] = heads.z[i-1];
					file<<heads.x[i-1]<<" "<<heads.y[i-1]<<" "<<" "<<heads.z[i-1]<<" ";
				}
				else
				{
					//x=0;
					//y=0;
					//z=0;
					rec=0;
					file<<heads.x[i-1]<<" "<<heads.y[i-1]<<" "<<heads.z[i-1]<<" ";

				}
			}
			catch (tf::TransformException ex)
			{
				rec=0;
			}
			file<<endl;
			msg.timestamp=stol(skel_ts);
			msg.id=i;
			msg.x=heads.x[i-1];
			msg.y=heads.y[i-1];
			msg.z=heads.z[i-1];
			msg_ar.head_co.push_back(msg);
			head ="head_";
			sstream.str(std::string());
			sstream.clear();
		
	  }//rate.sleep();
	  sstream2.str(std::string());
	  sstream2.clear();
	  skel_dat.publish(msg_ar);
	  skel_last_time=skel_cur_time;
	  skel_cur_time=ros::Time::now();
	}
}
file.close();
  return 0;
}
