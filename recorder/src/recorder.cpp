#include <stdio.h>
#include <time.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <cmath>
#include <ros/ros.h>
#include "std_msgs/String.h"
#include "std_msgs/Float64MultiArray.h"
#include <std_msgs/Float32.h>
#include <std_msgs/Header.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/LaserScan.h>
#include "sensor_msgs/Image.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cv_bridge/cv_bridge.h>
#include <cv_bridge/CvBridge.h>
#include <cvaux.h>
#include <cxcore.h>
#include <highgui.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>


#define NUM_READINGS 1128

using namespace std;
using namespace cv;
namespace enc = sensor_msgs::image_encodings;

time_t t;//img
long int timer ;
long int timer2;
float seconds ;

ros::Time rgb_cur_time, rgb_last_time;
ros::Time depth_cur_time, depth_last_time;

int REC = 0;

static const char WINDOW[] = "Image window";
 std::string path ="./rec/img_rgb/";
 std::string path2 ="./rec/img_di/";
 //std::string pathskel=".rec/skel.txt";
 std::string png = ".png";
 std::string path_ts;
 std::string path2_ts;
 std::string skel_ts;
 std::string hok_ts;
 std::stringstream sstream ;
 std::stringstream sstream2;




class recorder
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_rgb_; //image subscriber
  image_transport::Subscriber image_depth_;

  std_msgs::String msg;
  sensor_msgs::ImageConstPtr disparity_image_ptr_;
  sensor_msgs::CvBridge disparity_bridge_;


public:
 recorder()
    : it_(nh_)
  {

     image_rgb_ = it_.subscribe("/camera/rgb/image_color", 1, &recorder::imageCb_rgb, this);
     //image_depth_ = it_.subscribe("/camera/depth/image",1,&recorder::imageCb_di,this);
  
  }

  ~recorder()
  {
    cv::destroyWindow(WINDOW);
  }

 


  void imageCb_rgb(const sensor_msgs::ImageConstPtr& msg)
  {

     sensor_msgs::CvBridge bridge;//we need this object bridge for facilitating conversion from ros-img to opencv
     IplImage* img = bridge.imgMsgToCv(msg,"bgr8");  //image being converted from ros to opencv using cvbridge

     if (REC==1)
     {
         rgb_cur_time = ros::Time::now();
         cvShowImage( "RGB8 IMG",img);
         if(((rgb_cur_time.toSec())-(rgb_last_time.toSec()))>0.1)
         {
             time(&t);
             timer = (long)t;
             sstream << timer;
             path_ts = sstream.str();
             path_ts=path+path_ts+png;
             ros::Duration(0.01).sleep();
              printf("path %s\n",path_ts.c_str());
              const char* cstr = path_ts.c_str();
             // printf("lolol %s\n",cstr);
             //if(!(cvSaveImage(path_ts.c_str(),img)))
             if(!(cvSaveImage(cstr,img)))
             {
              printf("Can't Capture RGB Image\n");
              ros::Duration(0.01).sleep();
             }
             else
             {
                printf("RGB Image Captured\n");
                ros::Duration(0.01).sleep();
             }
             rgb_last_time=rgb_cur_time;
             rgb_cur_time=ros::Time::now();
             path_ts="";
             sstream.str(std::string());
             sstream.clear();
             //printf("path %s\n",path_ts.c_str());
          
         }

     }
      cvWaitKey(2);
    }

  void imageCb_di(const sensor_msgs::ImageConstPtr& msg)
    {
	
        sensor_msgs::CvBridge depth_bridge;
        IplImage* img_dd = depth_bridge.imgMsgToCv(msg,"mono16");
        cvShowImage( "pew",img_dd);
        if (REC==1)
             {
                depth_cur_time = ros::Time::now();
                if(((depth_cur_time.toSec())-(depth_last_time.toSec()))>0.1)
                {	
				
                    time(&t);
                    timer = (long)t;
                    sstream << timer;
                    path2_ts = sstream.str();
                    path2_ts=path2+path2_ts+png;
                    //printf("path 2 %s\n",path2_ts.c_str());
                    const char* cstr2 = path2_ts.c_str();
                    //ros::Duration(2).sleep();
                    if(!(cvSaveImage(cstr2,img_dd)))
                    {
                          printf("Can't Capture Depth Image\n");
                          //ros::Duration(2).sleep();
                    }
                    else
                    {
                         //printf("Depth Image Captured\n");
                          //ros::Duration(2).sleep();
                    }
                    path2_ts="";
                    sstream.str(std::string());
					sstream.clear();
                    //printf("path 2 cleared %s\n",path2_ts.c_str());
                    depth_last_time = depth_cur_time;
                    depth_cur_time=ros::Time::now();
                }
             }
        //cvShowImage( "pew",img_dd);
        cvWaitKey(2);
    
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
        rgb_cur_time = ros::Time::now();

        
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

    recorder ic;

    while (ros::ok())
    {
        ros::spin();
	}
    return 0;
}
