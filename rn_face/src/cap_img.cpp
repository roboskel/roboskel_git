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
#include "std_msgs/String.h"
#include <sys/time.h>
#include <chrono>
#include <ctime>

#define CV_NO_BACKWARD_COMPATIBILITY

using namespace std;
using namespace cv;
namespace enc = sensor_msgs::image_encodings;


int REC = 0;
     
time_t t;
time_t t2;
double seconds=0 ;
std::chrono::time_point<std::chrono::system_clock> p1;
ros::Time rgb_cur_time, rgb_last_time;
ros::Time total_cur_time, total_time;
std::string path ="image_";
std::string png = ".png";
std::string path_ts;
std::stringstream sstream ;

class FaceRec
{
ros::NodeHandle nh_; 
ros::Publisher pub ;;
image_transport::ImageTransport it_;
image_transport::Subscriber image_sub_;


public:
IplImage* iplImg;

cv::Mat frame;
//cv::Mat frame_scaled;
//cv::Mat depth_frame;
//cv::Mat depth_frame_scaled;
FaceRec()
: it_(nh_)
{
	
   pub = nh_.advertise<std_msgs::String>("image_addr", 1);

   image_sub_ = it_.subscribe("camera/rgb/image_color", 1, &FaceRec::imageCb, this);
}

  ~FaceRec()
  {
    
  }
  
void imageCb(const sensor_msgs::ImageConstPtr & msg)
{	

	if(REC==1)
	{
	//TIME INTERVAL STUFF	
	total_cur_time=ros::Time::now();
	if(((total_cur_time.toSec())-(total_time.toSec()))>=10)
	{
			REC=0;
			ros::shutdown();
	}
		
				//printf("IN RGB LOOP\n");
                rgb_cur_time = ros::Time::now();
                seconds=(rgb_cur_time.toSec())-(rgb_last_time.toSec());
                if(seconds>0.1)
                {	
					p1 = std::chrono::system_clock::now();
					sstream << std::chrono::duration_cast<std::chrono::milliseconds>(
					p1.time_since_epoch()).count() ;
                    path_ts = path_ts+sstream.str();
                    path_ts=path+path_ts+png;
                    const char* cstr = path_ts.c_str();
                    //printf("BEFORE BRIDGE\n");
                    
					cv_bridge::CvImagePtr cv_ptr_rgb;
					try
					{
						//printf("IN BRIDGE\n");
						cv_ptr_rgb = cv_bridge::toCvCopy(msg, "bgr8");
					}
					catch (cv_bridge::Exception & e)
					{
						ROS_ERROR("cv_bridge exception: %s", e.what());
						return;
					}

					frame = cv_ptr_rgb->image;  //mat=iplimg
					cv::imwrite(cstr,frame);
					std_msgs::String msg;
					msg.data = path_ts;
					pub.publish(msg);
					//printf("path 2 %s\n",path_ts.c_str());
					rgb_last_time=rgb_cur_time;
					rgb_cur_time=ros::Time::now();
					path_ts="";
					sstream.str(std::string());
					sstream.clear();
				}
			
			
			
	}
	//cv::waitKey(3);
}
  
};

void callback(const std_msgs::Float64::ConstPtr& msg)
{
	        total_time = ros::Time::now();

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

int main(int argc, char** argv)
{
	ros::init(argc, argv, "recorder");
    ros::NodeHandle n;
    ros::Subscriber gp_in,face_lis;
	gp_in =n.subscribe("coms",5,callback);

   ///////////////////////
   //CLASSIFIER STUFF TELOS
   ////////////////////////
    FaceRec ic;
    while (ros::ok())
    {
        ros::spin();
	}
	ros::shutdown();
return 0;
}
