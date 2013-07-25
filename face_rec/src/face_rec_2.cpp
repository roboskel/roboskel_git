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
//-------------------
//Mean SHIFT vars
IplImage *image = 0, *hsv = 0, *hue = 0, *mask = 0, *backproject = 0, *histimg = 0;
CvHistogram *hist = 0;
int backproject_mode = 0;
int select_object = 0;
int track_object = 0;
int show_hist = 1;
CvPoint origin;
CvRect selection;
CvRect track_window;
CvBox2D track_box;
CvConnectedComp track_comp;
int hdims = 16;
float hranges_arr[] = {0,180};
float* hranges = hranges_arr;
int vmin = 60, vmax = 256, smin = 50;
//-----------------
int DETECTED = 0;

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
	int r;
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

CvFont font;

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
 
 
 
CvScalar hsv2rgb( float hue ){
    int rgb[3], p, sector;
    static const int sector_data[][3]={{0,2,1}, {1,2,0}, {1,0,2}, {2,0,1}, {2,1,0}, {0,1,2}};
    hue *= 0.033333333333333333333333333333333f;
    sector = cvFloor(hue);
    p = cvRound(255*(hue - sector));
    p ^= sector & 1 ? 255 : 0;
    rgb[sector_data[sector][0]] = 255;
    rgb[sector_data[sector][1]] = 0;
    rgb[sector_data[sector][2]] = p;
    return cvScalar(rgb[2], rgb[1], rgb[0],0);
}

 
 class FaceRec
{
ros::NodeHandle nh_; 
ros::Publisher pub ;;
image_transport::ImageTransport it_;
image_transport::Subscriber image_sub_;
image_transport::Subscriber depth_sub_;
image_transport::Publisher depth_pub_;

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
	if(REC==1){
				//printf("IN RGB LOOP\n");
                rgb_cur_time = ros::Time::now();
                seconds=(rgb_cur_time.toSec())-(rgb_last_time.toSec());
                if(seconds>0.1)
                {	
					time(&t);
                    timer = (long)t;
                    //printf("path 2 %s\n",path2_ts.c_str());
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
					frame = cv_ptr_rgb->image;  //mat=iplimg
					cv::imwrite("test.jpg",frame);
					//iplImg= cv_ptr_rgb->image;
					//iplImg=frame;
					//IplImage* cvGetImage( const CvArr* frame, IplImage* iplImg ); 
					iplImg = cvLoadImage("test.jpg",1);
					//IplImage* iplImg = new IplImage(frame); 
					//iplImg = cvCloneImage(&frame) ;
					if (!image) 
					{
					/* allocate all the buffers */
						image = cvCreateImage( cvGetSize(iplImg), 8, 3 );
						image->origin = iplImg->origin;
						hsv = cvCreateImage( cvGetSize(iplImg), 8, 3 );
						hue = cvCreateImage( cvGetSize(iplImg), 8, 1 );
						mask = cvCreateImage( cvGetSize(iplImg), 8, 1 );
						backproject = cvCreateImage( cvGetSize(iplImg), 8, 1 );
						hist = cvCreateHist( 1, &hdims, CV_HIST_ARRAY, &hranges, 1 ); //Creates a histogram.
						histimg = cvCreateImage( cvSize(320,200), 8, 3 ); //Creates an image header and allocates the image data. //CvSize size, int depth, int channels)
						cvZero( histimg );
					}
						cvCopy( iplImg, image, 0 );
					cvCvtColor( image, hsv, CV_BGR2HSV );
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
	//MESSAGE CREATION
	face_rec::faceData face_data;
	face_rec::faceDataArray face_array;

    int i = 0;

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
		if (i==0)
		{
			DETECTED=1;
		}
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



    }
    if (DETECTED==1)
    {
		pub.publish(face_array);
	}

  
   
    cv::imshow( "Face Detection and Tracking", img );
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

void faceCallback(const face_rec::faceDataArray::ConstPtr& msg)
{
	//int total = msg->total;
	int bin_w;
	if (DETECTED == 1) {
				printf("\n\nMean-Shift\n\n");
				selection = cvRect(msg->faces[0].x-msg->faces[0].radius/10,msg->faces[0].y-msg->faces[0].radius/10, msg->faces[0].x+msg->faces[0].radius/5,msg->faces[0].y+msg->faces[0].radius/5);
				printf("%d %d %d %d \n",selection.x,selection.y,selection.width,selection.height);
				int _vmin = vmin, _vmax = vmax;
				
				cvInRangeS( hsv, cvScalar(0,smin,MIN(_vmin,_vmax),0),cvScalar(180,256,MAX(_vmin,_vmax),0), mask );
				cvSplit( hsv, hue, 0, 0, 0 );
				
				if (track_object < 0) {
					float max_val = 0.f;
					cvSetImageROI( hue, selection );
					cvSetImageROI( mask, selection );
					cvCalcHist( &hue, hist, 0, mask );
					cvGetMinMaxHistValue( hist, 0, &max_val, 0, 0 );
					cvConvertScale( hist->bins, hist->bins, max_val ? 255. / max_val : 0., 0 );
					cvResetImageROI( hue );
					cvResetImageROI( mask );
					track_window = selection;
					track_object = 1;
					cvZero( histimg );
					bin_w = histimg->width / hdims;
					
					for(int i = 0; i < hdims; i++ ){
						int val = cvRound( cvGetReal1D(hist->bins,i)*histimg->height/255 );
						CvScalar color = hsv2rgb(i*180.f/hdims);
						cvRectangle( histimg, cvPoint(i*bin_w,histimg->height), cvPoint((i+1)*bin_w,histimg->height - val), color, -1, 8, 0 );
					}
				}
				cvCalcBackProject( &hue, backproject, hist ); //Calculates the back projection. --> ypologizei to kentro mazas me bash ta barh
				
				cvAnd( backproject, mask, backproject, 0 );
				
				cvCamShift( backproject, track_window,cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ), &track_comp, &track_box );
				
				track_window = track_comp.rect;
				
				
				if( backproject_mode ){
					cvCvtColor( backproject, image, CV_GRAY2BGR ); //Converts an image from one color space to another.
				}
				
				if( !image->origin ){
					track_box.angle = -track_box.angle; // CvBox2D - track_box
				}
								    printf("IMSHOW\n");

				cvEllipseBox( &image, track_box, CV_RGB(255,0,0), 3, CV_AA, 0 );
				//cvCircle(image, track_box, 100 , CV_RGB(255,255,0), 1, CV_AA, 0 );
				//snprintf(string_faces, sizeof(string_faces), "face %d", msg->faces[0].id );
				//cvPutText(image,string_faces,cvPoint(track_window.x,track_window.y),&font, CV_RGB(0,128,255));
				//------------------------------------------------
				    printf("IMSHOW\n");

				cvShowImage( "Face Detection and Tracking", image );
				cvWaitKey(500); // o xronos gia kathe frame
				
	}
	}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "recorder");
    ros::NodeHandle n;
    ros::Subscriber gp_in,face_lis;
	
	rgb_last_time = ros::Time::now();
    depth_last_time = ros::Time::now();

   cvInitFont(&font,CV_FONT_HERSHEY_PLAIN|CV_FONT_ITALIC,1,1,1,1);

   ///////////////////////
   //CLASSIFIER STUFF 
   ////////////////////////    

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
    cvNamedWindow("Face Detection and Tracking",1);
    cvNamedWindow( "Face Detection and Tracking", 1 );
	cvNamedWindow( "Histogram", 1 );
	printf("Waiting gamepad Input\n");
    gp_in =n.subscribe("gp_functions", 1, chatterCallback);
    face_lis=n.subscribe("faceDataArray",1, faceCallback);
    
   
	FaceRec ic;
    while (ros::ok())
    {
        ros::spin();
	}
    cvDestroyWindow("Face Detection and Tracking");
return 0;
}
