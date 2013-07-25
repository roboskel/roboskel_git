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

#include "cv.h"
#include "highgui.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "std_msgs/Float64MultiArray.h"
#include "face_rec/faceData.h"
#include "face_rec/faceDataArray.h"
#define CV_NO_BACKWARD_COMPATIBILITY

using namespace std;
using namespace cv;
namespace enc = sensor_msgs::image_encodings;

ros::Time rgb_cur_time, rgb_last_time;
float seconds ;

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
	
   pub = nh_.advertise<face_rec::faceDataArray>("faceDataArray", 100);

   image_sub_ = it_.subscribe("camera/rgb/image_color", 1, &FaceRec::imageCb, this);
}

  ~FaceRec()
  {
    
  }
  
  void imageCb(const sensor_msgs::ImageConstPtr & msg)
{	
	
				//printf("IN RGB LOOP\n");
                rgb_cur_time = ros::Time::now();
                seconds=(rgb_cur_time.toSec())-(rgb_last_time.toSec());
                if(seconds>0.1)
                {	
                    
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
					cv::imwrite("test.jpg",frame);

					rgb_last_time=rgb_cur_time;
					rgb_cur_time=ros::Time::now();

				}
	
		
			
			
			
}
  
};


int main(int argc, char** argv)
{
	ros::init(argc, argv, "recorder");
    ros::NodeHandle n;
    ros::Subscriber gp_in,face_lis;

   ///////////////////////
   //CLASSIFIER STUFF TELOS
   ////////////////////////
    FaceRec ic;
    while (ros::ok())
    {
        ros::spin();
	}
    cvDestroyWindow("Face Detection and Tracking");
return 0;
}
