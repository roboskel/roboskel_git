#include <stdio.h>
#include <time.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <cmath>
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
using namespace std;
namespace enc = sensor_msgs::image_encodings;

time_t t;
time_t t2;
long int timer ;
long int timer2;
float seconds ;
float seconds2;
ros::Time rgb_cur_time, rgb_last_time;
ros::Time depth_cur_time, depth_last_time;

String cascadeName = "./haarcascade_frontalface_alt.xml";
String nestedCascadeName ="./haarcascade_eye_tree_eyeglasses.xml";

int REC = 0;

 std::string path ="rgb";
 std::string path2 ="dpt";
 //std::string path ="";
 //std::string path2 ="";
 std::string png = ".txt";
 std::string jpg = ".jpg";
 std::string path_ts;
 std::string path2_ts;
 std::string sec;
 std::string sec2;

 std::stringstream sstream ;
 std::stringstream sstream2;


class DepthInfo
{
ros::NodeHandle nh_;

image_transport::ImageTransport it_;
image_transport::Subscriber image_sub_;
image_transport::Subscriber depth_sub_;
image_transport::Publisher depth_pub_;

public:

cv::Mat rgb_frame;
cv::Mat rgb_frame_scaled;
cv::Mat depth_frame;
cv::Mat depth_frame_scaled;

DepthInfo()
: it_(nh_)
{
	depth_pub_ = it_.advertise("out", 1);
	depth_sub_ = it_.subscribe("/camera/depth/image", 1, &DepthInfo::depthInfoCb, this);
	//image_sub_ = it_.subscribe("camera/rgb/image_color", 1, &DepthInfo::imageCb, this);
}
void imageCb(const sensor_msgs::ImageConstPtr & msg)
{	
	if(REC==1){
				printf("IN RGB LOOP\n");
                rgb_cur_time = ros::Time::now();
                seconds=(rgb_cur_time.toSec())-(rgb_last_time.toSec());
                if(seconds>0.1)
                {	
				
                    time(&t);
                    timer = (long)t;
                    sstream << timer<<seconds;
                    path_ts = sstream.str();
                    path_ts=path+path_ts+png;
                    //printf("path 2 %s\n",path2_ts.c_str());
                    const char* cstr = path_ts.c_str();
                    printf("BEFORE BRIDGE\n");
                    
					cv_bridge::CvImagePtr cv_ptr_rgb;
					try
					{
						printf("IN BRIDGE\n");
						cv_ptr_rgb = cv_bridge::toCvCopy(msg, "bgr8");
					}
					catch (cv_bridge::Exception & e)
					{
						ROS_ERROR("cv_bridge exception: %s", e.what());
						return;
					}
					
					printf("OUTSIDE BRIDGE\n");
					rgb_frame = cv_ptr_rgb->image;
					
					printf("BEFORE WRITE\n");
					printf("path 2 %s\n",path_ts.c_str());
					//cv::imwrite(cstr,rgb_frame);
					//FILE *imageFile = fopen(cstr, "wb");
					//fwrite(depth_frame.data,  640*480, sizeof(u_int16_t), imageFile);
					printf("path 2 %s\n",path_ts.c_str());
					//ros::Duration(1).sleep();
					rgb_last_time=rgb_cur_time;
					rgb_cur_time=ros::Time::now();
					path_ts="";
					sstream.str(std::string());
					sstream.clear();
				}
			
			
			
	}
	//cv::waitKey(3);
}
void depthInfoCb(const sensor_msgs::ImageConstPtr & msg)
{
	
	if (REC==1)
	{
		printf("IN DEPTH LOOP\n");
		depth_cur_time = ros::Time::now();
		seconds2=depth_cur_time.toSec()-depth_last_time.toSec();
		if(seconds2>0.1)
		{	
					
			time(&t2);
			timer2 = (long)t2;
			sstream2 << timer2<<seconds2;
			path2_ts = sstream2.str();
			path2_ts=path2+path2_ts+png;
			//printf("path 2 %s\n",path2_ts.c_str());
			const char* cstr2 = path2_ts.c_str();
			printf("BEFORE DEPTH BRIDGE\n");
			
			cv_bridge::CvImagePtr cv_ptr_depth;
			try
			{
				cv_ptr_depth = cv_bridge::toCvCopy(msg, "mono16");
			}
			catch (cv_bridge::Exception & e)
			{
				ROS_ERROR("cv_bridge exception: %s", e.what());
				return;
			}
			VideoCapture capture( CV_CAP_OPENNI );
			depth_frame = cv_ptr_depth->image;
			capture.retrieve( depth_frame, CV_CAP_OPENNI_DEPTH_MAP );
			cout << "rows: " << depth_frame.rows << " cols: " << depth_frame.cols << endl;
			    	 cv::waitKey( 30 );
					
			//depth_frame.convertTo(depth_frame_scaled, CV_16UC1, 1024, 0); 
			//cv::imshow("depth_image", depth_frame_scaled);
			//depth_pub_.publish(cv_ptr_depth->toImageMsg());
			//printf("AFTER DEPTHBRIDGE \n");
			//printf("path 2 %s\n",path2_ts.c_str());
			//printf("path 2 %s\n",cstr2);
			//cv::imwrite(cstr2,depth_frame_scaled);
			//cv::imwrite(cstr2,depth_frame);
			//FILE *imageFile = fopen(cstr2, "wb");
			//fwrite(depth_frame_scaled.data,  640*480, sizeof(u_int16_t), imageFile);
			double min,max;
			
			
			cout << "M = "<< endl << " "  << depth_frame_scaled << endl << endl;
			ros::Duration(5).sleep();
			//fclose (imageFile);
			depth_last_time=rgb_cur_time;
			depth_cur_time=ros::Time::now();
			path_ts="";
			sstream2.str(std::string());
			sstream2.clear();
		}
	}
}
};


void chatterCallback(const std_msgs::Float64MultiArray::ConstPtr& msg)
{
    int start = msg->data[11]; //START gia na 3ekinhsei to record
    int cancel = msg->data[4]; //X gia na stamathsei
    //printf("Press Start to Start Recording \n");
    if (start==1)
    {	
		printf("Started Recording \n");
        REC = 1;
        ros::Duration(1).sleep();
	}
    else if ((REC==1)&&(cancel==1))
    {
        REC = 0;
        printf("Stopped Recording \n");
		ros::shutdown();
    }
}



int main(int argc, char** argv)
{
	ros::init(argc, argv, "recorder");
    ros::NodeHandle n;
    ros::Subscriber gp_in;
	
	rgb_last_time = ros::Time::now();
    depth_last_time = ros::Time::now();
        
	printf("Waiting gamepad Input\n");
    gp_in =n.subscribe("gp_functions", 1, chatterCallback);
	DepthInfo ic;
    while (ros::ok())
    {
        ros::spin();
	}
    return 0;
return 0;
}
