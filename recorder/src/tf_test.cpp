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
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "std_msgs/Float64MultiArray.h"
#include "std_msgs/Float64.h"
#include <sys/time.h>
#include <chrono>
#include <ctime>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#define PI 3.14159265

std::chrono::time_point<std::chrono::system_clock> p1;

using namespace std;

ofstream file;

std::string head ="head_";
std::string neck ="neck_";
std::string torso ="torso_";

std::string rshou ="right_shoulder_";
std::string relb ="right_elbow_";
std::string rhand="right_hand_";
std::string rhip="right_hip_";
std::string rknee="right_knee_";
std::string rfoot="right_foot_";

std::string lshou ="left_shoulder_";
std::string lelb ="left_elbow_";
std::string lhand="left_hand_";
std::string lhip="left_hip_";
std::string lknee="left_knee_";
std::string lfoot="left_foot_";

std::string skel_ts;

std::stringstream  sstream;
std::stringstream  sstream2;

ros::Time skel_cur_time, skel_last_time;

double seconds;

double w1;
double w2;
int rec=1;
double x_prev=1000;
double y_prev=1000;
double z_prev=1000;

double x;
double y;
double z;

int REC=0;

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

int main(int argc, char** argv){

ros::init(argc, argv, "skel_recorder");
ros::NodeHandle node;
ros::Subscriber gp_in;
gp_in =node.subscribe("coms",5,callback);
file.open ("./_skel.data", ios::out | ios::binary);

tf::TransformListener listener;
	tf::Vector3 origin;
	tf::StampedTransform transform;
geometry_msgs::Vector3 cord;
ros::Rate rate(10.0);


  while (node.ok())
  {
	ros::spinOnce();
	skel_cur_time=ros::Time::now();
	seconds=skel_cur_time.toSec()-skel_last_time.toSec();
	if ((REC==1)&&(seconds>0.1))
	{	
		
		p1 = std::chrono::system_clock::now();
		sstream2 << std::chrono::duration_cast<std::chrono::milliseconds>(p1.time_since_epoch()).count() ;
		sstream2>>skel_ts;
		for(int i=1;i<=6;i++)
		{
			sstream << i;
			head=head+sstream.str();
			neck=neck+sstream.str();
			torso=torso+sstream.str();
			
			rshou=rshou+sstream.str();
			relb=relb+sstream.str();
			rhand=rhand+sstream.str();
			rhip=rhip+sstream.str();
			rknee=rknee+sstream.str();
			rfoot=rfoot+sstream.str();
			
			lshou=lshou+sstream.str();
			lelb=lelb+sstream.str();
			lhand=lhand+sstream.str();
			lhip=lhip+sstream.str();
			lknee=lknee+sstream.str();
			lfoot=lfoot+sstream.str();
			
			try{
			  listener.lookupTransform("/openni_depth_frame", head,
									   ros::Time(0), transform);
				origin =transform.getOrigin();
				x=origin.x();
				y=origin.y();
				z=origin.z();
				
				
				file<<skel_ts<<" "<<i<<" ";
				
				if ((cord.x!=x_prev)&&(cord.y!=y_prev)&&(cord.z!=z_prev))
				{
					rec =1;
					x_prev = x;
					y_prev = y;
					z_prev = z;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";
				}
				else
				{
					//x=0;
					//y=0;
					//z=0;
					rec=0;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";

				}
			}
			catch (tf::TransformException ex)
			{
				rec=0;
			}
						try{
			  listener.lookupTransform("/openni_depth_frame", neck,
									   ros::Time(0), transform);
				origin =transform.getOrigin();
				x=origin.x();
				y=origin.y();
				z=origin.z();
				
				if ((cord.x!=x_prev)&&(cord.y!=y_prev)&&(cord.z!=z_prev))
				{
					rec =1;
					x_prev = x;
					y_prev = y;
					z_prev = z;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";
				}
				else
				{
					//x=0;
					//y=0;
					//z=0;
					rec=0;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";

				}
			}
			catch (tf::TransformException ex)
			{
				rec=0;
			}
			try{
			  listener.lookupTransform("/openni_depth_frame", torso,
									   ros::Time(0), transform);
				origin =transform.getOrigin();
				x=origin.x();
				y=origin.y();
				z=origin.z();
				
				if ((cord.x!=x_prev)&&(cord.y!=y_prev)&&(cord.z!=z_prev))
				{
					rec =1;
					x_prev = x;
					y_prev = y;
					z_prev = z;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";

				}
				else
				{
					//x=0;
					//y=0;
					//z=0;
					rec=0;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";


				}
			}
			catch (tf::TransformException ex)
			{
				rec=0;
				//ros::Duration(1).sleep();
			}
			try{
			  listener.lookupTransform("/openni_depth_frame", rshou,
									   ros::Time(0), transform);
				origin =transform.getOrigin();
				x=origin.x();
				y=origin.y();
				z=origin.z();
			if ((cord.x!=x_prev)&&(cord.y!=y_prev)&&(cord.z!=z_prev))
				{
					rec =1;
					x_prev = x;
					y_prev = y;
					z_prev = z;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";
				}
				else
				{
					//x=0;
					//y=0;
					//z=0;
					rec=0;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";


				}
			}
			catch (tf::TransformException ex)
			{
				rec=0;
				//ros::Duration(1).sleep();
			}
				
			try{
			  listener.lookupTransform("/openni_depth_frame", relb,
									   ros::Time(0), transform);
				origin =transform.getOrigin();
				x=origin.x();
				y=origin.y();
				z=origin.z();
				
				if ((cord.x!=x_prev)&&(cord.y!=y_prev)&&(cord.z!=z_prev))
				{
					rec =1;
					x_prev = x;
					y_prev = y;
					z_prev = z;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";

				}
				else
				{
					//x=0;
					//y=0;
					//z=0;
					rec=0;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";

				}
			}
			catch (tf::TransformException ex)
			{
				rec=0;
			}
			
			try{
			  listener.lookupTransform("/openni_depth_frame", rhand,
									   ros::Time(0), transform);
				origin =transform.getOrigin();
				x=origin.x();
				y=origin.y();
				z=origin.z();
				
				if ((cord.x!=x_prev)&&(cord.y!=y_prev)&&(cord.z!=z_prev))
				{
					rec =1;
					x_prev = x;
					y_prev = y;
					z_prev = z;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";

				}
				else
				{
					//x=0;
					//y=0;
					//z=0;
					rec=0;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";

				}
			}
			catch (tf::TransformException ex)
			{
				rec=0;
			}
			
			try{
			  listener.lookupTransform("/openni_depth_frame", rhip,
									   ros::Time(0), transform);
				origin =transform.getOrigin();
				x=origin.x();
				y=origin.y();
				z=origin.z();
				
				if ((cord.x!=x_prev)&&(cord.y!=y_prev)&&(cord.z!=z_prev))
				{
					rec =1;
					x_prev = x;
					y_prev = y;
					z_prev = z;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";

				}
				else
				{
					//x=0;
					//y=0;
					//z=0;
					rec=0;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";

				}
			}
			catch (tf::TransformException ex)
			{
				rec=0;
			}
			
			try{
			  listener.lookupTransform("/openni_depth_frame", rknee,
									   ros::Time(0), transform);
				origin =transform.getOrigin();
				x=origin.x();
				y=origin.y();
				z=origin.z();
				
				if ((cord.x!=x_prev)&&(cord.y!=y_prev)&&(cord.z!=z_prev))
				{
					rec =1;
					x_prev = x;
					y_prev = y;
					z_prev = z;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";

				}
				else
				{
					//x=0;
					//y=0;
					//z=0;
					rec=0;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";

				}
			}
			catch (tf::TransformException ex)
			{
				rec=0;
			}
			
			try{
			  listener.lookupTransform("/openni_depth_frame", rfoot,
									   ros::Time(0), transform);
				origin =transform.getOrigin();
				x=origin.x();
				y=origin.y();
				z=origin.z();
				
				if ((cord.x!=x_prev)&&(cord.y!=y_prev)&&(cord.z!=z_prev))
				{
					rec =1;
					x_prev = x;
					y_prev = y;
					z_prev = z;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";

				}
				else
				{
					//x=0;
					//y=0;
					//z=0;
					rec=0;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";

				}
			}
			catch (tf::TransformException ex)
			{
				rec=0;
			}
			
			try{
			  listener.lookupTransform("/openni_depth_frame", lshou,
									   ros::Time(0), transform);
				origin =transform.getOrigin();
				x=origin.x();
				y=origin.y();
				z=origin.z();
			if ((cord.x!=x_prev)&&(cord.y!=y_prev)&&(cord.z!=z_prev))
				{
					rec =1;
					x_prev = x;
					y_prev = y;
					z_prev = z;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";
				}
				else
				{
					//x=0;
					//y=0;
					//z=0;
					rec=0;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";


				}
			}
			catch (tf::TransformException ex)
			{
				rec=0;
				//ros::Duration(1).sleep();
			}
				
			try{
			  listener.lookupTransform("/openni_depth_frame", lelb,
									   ros::Time(0), transform);
				origin =transform.getOrigin();
				x=origin.x();
				y=origin.y();
				z=origin.z();
				
				if ((cord.x!=x_prev)&&(cord.y!=y_prev)&&(cord.z!=z_prev))
				{
					rec =1;
					x_prev = x;
					y_prev = y;
					z_prev = z;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";

				}
				else
				{
					//x=0;
					//y=0;
					//z=0;
					rec=0;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";

				}
			}
			catch (tf::TransformException ex)
			{
				rec=0;
			}
			
			try{
			  listener.lookupTransform("/openni_depth_frame", lhand,
									   ros::Time(0), transform);
				origin =transform.getOrigin();
				x=origin.x();
				y=origin.y();
				z=origin.z();
				
				if ((cord.x!=x_prev)&&(cord.y!=y_prev)&&(cord.z!=z_prev))
				{
					rec =1;
					x_prev = x;
					y_prev = y;
					z_prev = z;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";

				}
				else
				{
					//x=0;
					//y=0;
					//z=0;
					rec=0;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";

				}
			}
			catch (tf::TransformException ex)
			{
				rec=0;
			}
			
			try{
			  listener.lookupTransform("/openni_depth_frame", lhip,
									   ros::Time(0), transform);
				origin =transform.getOrigin();
				x=origin.x();
				y=origin.y();
				z=origin.z();
				
				if ((cord.x!=x_prev)&&(cord.y!=y_prev)&&(cord.z!=z_prev))
				{
					rec =1;
					x_prev = x;
					y_prev = y;
					z_prev = z;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";

				}
				else
				{
					//x=0;
					//y=0;
					//z=0;
					rec=0;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";

				}
			}
			catch (tf::TransformException ex)
			{
				rec=0;
			}
			
			try{
			  listener.lookupTransform("/openni_depth_frame", lknee,
									   ros::Time(0), transform);
				origin =transform.getOrigin();
				x=origin.x();
				y=origin.y();
				z=origin.z();
				
				if ((cord.x!=x_prev)&&(cord.y!=y_prev)&&(cord.z!=z_prev))
				{
					rec =1;
					x_prev = x;
					y_prev = y;
					z_prev = z;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";

				}
				else
				{
					//x=0;
					//y=0;
					//z=0;
					rec=0;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";

				}
			}
			catch (tf::TransformException ex)
			{
				rec=0;
			}
			
			try{
			  listener.lookupTransform("/openni_depth_frame", lfoot,
									   ros::Time(0), transform);
				origin =transform.getOrigin();
				x=origin.x();
				y=origin.y();
				z=origin.z();
				
				if ((cord.x!=x_prev)&&(cord.y!=y_prev)&&(cord.z!=z_prev))
				{
					rec =1;
					x_prev = x;
					y_prev = y;
					z_prev = z;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";

				}
				else
				{
					//x=0;
					//y=0;
					//z=0;
					rec=0;
					file<<x<<" "<<y<<" "<<" "<<z<<" ";

				}
			}
			catch (tf::TransformException ex)
			{
				rec=0;
			}
			file<<endl;
			head ="head_";
			neck ="neck_";
			torso ="torso_";

			rshou ="right_shoulder_";
			relb ="right_elbow_";
			rhand="right_hand_";
			rhip="right_hip_";
			rknee="right_knee_";
			rfoot="right_foot_";

			lshou ="left_shoulder_";
			lelb ="left_elbow_";
			lhand="left_hand_";
			lhip="left_hip_";
			lknee="left_knee_";
			lfoot="left_foot_";
			
		  sstream.str(std::string());
		  sstream.clear();
		
	  }//rate.sleep();
	  sstream2.str(std::string());
	  sstream2.clear();
	  
	  skel_last_time=skel_cur_time;
	  skel_cur_time=ros::Time::now();
  }
}
file.close();
  return 0;
};
