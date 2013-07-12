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
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "std_msgs/Float64MultiArray.h"
#include "face_rec/faceData.h"
#include "face_rec/faceDataArray.h"

using namespace std;
using namespace cv;
namespace enc = sensor_msgs::image_encodings;

time_t t;
time_t t2;
long int timer ;
long int timer2;
float seconds ;
float seconds2;
ros::Time rgb_cur_time, rgb_last_time;
ros::Time depth_cur_time, depth_last_time;

struct face
{
	int id;
	int x;
	int y;
	float r;
};

int REC = 1;

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
 
String cascadeName = "./haarcascade_frontalface_alt.xml";
String nestedCascadeName ="./haarcascade_eye_tree_eyeglasses.xml";
 
	////////////////
   //CLASSIFIER STUFF
   ////////////////
    const String scaleOpt = "--scale=";
    size_t scaleOptLen = scaleOpt.length();
    const String cascadeOpt = "--cascade=";
    size_t cascadeOptLen = cascadeOpt.length();
    const String nestedCascadeOpt = "--nested-cascade";
    size_t nestedCascadeOptLen = nestedCascadeOpt.length();
    String inputName;

    CascadeClassifier cascade, nestedCascade;
    double scale = 1;
 
 	////////////////
   //CLASSIFIER STUFF
   ////////////////
 
 class FaceRec
{
ros::NodeHandle nh_; 
ros::Publisher pub ;;
image_transport::ImageTransport it_;
image_transport::Subscriber image_sub_;
image_transport::Subscriber depth_sub_;
image_transport::Publisher depth_pub_;

public:

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
	if(REC==1){
				//printf("IN RGB LOOP\n");
                rgb_cur_time = ros::Time::now();
                seconds=(rgb_cur_time.toSec())-(rgb_last_time.toSec());
                if(seconds>0.1)
                {	
					time(&t);
                    timer = (long)t;
                    //printf("path 2 %s\n",path2_ts.c_str());
                    const char* cstr = path_ts.c_str();
                   // printf("BEFORE BRIDGE\n");
                    
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
					//printf("OUTSIDE BRIDGE\n");
					frame = cv_ptr_rgb->image;
					detectAndDraw( frame, cascade, nestedCascade, scale );
					waitKey(1);
					//printf("BEFORE WRITE\n");
					//printf("path 2 %s\n",path_ts.c_str());
					//ros::Duration(1).sleep();
					rgb_last_time=rgb_cur_time;
					rgb_cur_time=ros::Time::now();
					sstream.str(std::string());
					sstream.clear();
				}
	
		}
			
			
			
}
void detectAndDraw( Mat& img,
                   CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
                   double scale)
{	
	face faces_str[50];
	//MESSAGE CREATION
	face_rec::faceData face_data;
	face_rec::faceDataArray face_array;

  

    int i = 0;
    int c1=0;
    int c2=0;
    double t = 0;
    vector<Rect> faces;
    const static Scalar colors[] =  { CV_RGB(0,0,255),
        CV_RGB(0,128,255),
        CV_RGB(0,255,255),
        CV_RGB(0,255,0),
        CV_RGB(255,128,0),
        CV_RGB(255,255,0),
        CV_RGB(255,0,0),
        CV_RGB(255,0,255)} ;
    Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );

    cvtColor( img, gray, CV_BGR2GRAY );
    resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
    equalizeHist( smallImg, smallImg );

    t = (double)cvGetTickCount();
    cascade.detectMultiScale( smallImg, faces,
        1.1, 2, 0
        //|CV_HAAR_FIND_BIGGEST_OBJECT
        //|CV_HAAR_DO_ROUGH_SEARCH
        |CV_HAAR_SCALE_IMAGE
        ,
        Size(30, 30) );
    t = (double)cvGetTickCount() - t;
    printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );
    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
    {
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = colors[i%8];
        int radius;
        center.x = cvRound((r->x + r->width*0.5)*scale);
        center.y = cvRound((r->y + r->height*0.5)*scale);
        radius = cvRound((r->width + r->height)*0.25*scale);
        circle( img, center, radius, color, 3, 8, 0 );
        face_data.id=i;
        face_data.x=center.x;
        face_data.y=center.y;
        face_data.radius=radius;

		face_array.faces.push_back(face_data);
		face_array.total++;

        if( nestedCascade.empty() )
            continue;
        smallImgROI = smallImg(*r);
        nestedCascade.detectMultiScale( smallImgROI, nestedObjects,
            1.1, 2, 0
            //|CV_HAAR_FIND_BIGGEST_OBJECT
            //|CV_HAAR_DO_ROUGH_SEARCH
            //|CV_HAAR_DO_CANNY_PRUNING
            |CV_HAAR_SCALE_IMAGE
            ,
            Size(30, 30) );
        for( vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++ )
        {
            center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
            center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
            radius = cvRound((nr->width + nr->height)*0.25*scale);
            circle( img, center, radius, color, 3, 8, 0 );
            c2++;
            face_data.id=c2++;
			face_data.x=center.x;
			face_data.y=center.y;
			face_data.radius=radius;
			face_array.faces.push_back(face_data);
			face_array.total++;

        }


    }
	pub.publish(face_array);

  
   
    
    cv::imshow( "Face_Detection", img );
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
    
    
    
    ///////////////////////
   //CLASSIFIER STUFF 
   ////////////////////////    
    /*
    for( int i = 1; i < argc; i++ )
    {
        cout << "Processing " << i << " " <<  argv[i] << endl;
        if( cascadeOpt.compare( 0, cascadeOptLen, argv[i], cascadeOptLen ) == 0 )
        {
            cascadeName.assign( argv[i] + cascadeOptLen );
            cout << "  from which we have cascadeName= " << cascadeName << endl;
        }
        else if( nestedCascadeOpt.compare( 0, nestedCascadeOptLen, argv[i], nestedCascadeOptLen ) == 0 )
        {
            if( argv[i][nestedCascadeOpt.length()] == '=' )
                nestedCascadeName.assign( argv[i] + nestedCascadeOpt.length() + 1 );
            if( !nestedCascade.load( nestedCascadeName ) )
                cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;
        }
        else if( scaleOpt.compare( 0, scaleOptLen, argv[i], scaleOptLen ) == 0 )
        {
            if( !sscanf( argv[i] + scaleOpt.length(), "%lf", &scale ) || scale < 1 )
                scale = 1;
            cout << " from which we read scale = " << scale << endl;
        }
        else if( argv[i][0] == '-' )
        {
            cerr << "WARNING: Unknown option %s" << argv[i] << endl;
        }
        else
            inputName.assign( argv[i] );
    }*/

    if( !cascade.load( cascadeName ) )
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        cerr << "Usage: facedetect [--cascade=<cascade_path>]\n"
            "   [--nested-cascade[=nested_cascade_path]]\n"
            "   [--scale[=<image scale>\n"
            "   [filename|camera_index]\n" << endl ;
        return -1;
    }
   ///////////////////////
   //CLASSIFIER STUFF TELOS
   ////////////////////////
    cvNamedWindow("Face_Detection",1);
	printf("Waiting gamepad Input\n");
    gp_in =n.subscribe("gp_functions", 1, chatterCallback);
    
   
	FaceRec ic;
    while (ros::ok())
    {
        ros::spin();
	}
    cvDestroyWindow("Face_Detection");
return 0;
}
