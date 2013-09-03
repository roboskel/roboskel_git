// g++ `pkg-config --cflags opencv`   `pkg-config --libs opencv` face_detection_and_tracking.cpp -o face_detection_and_tracking
#include <stdlib.h>

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

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


#include "cv.h"
#include "highgui.h"
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <stdio.h>
#include <ctype.h>

#include <iostream>
#include <stdio.h>
#include <unistd.h>

#include "std_msgs/Float64MultiArray.h"
#include "std_msgs/String.h"
#include "std_msgs/Int64.h"
#include "rn_face/head_cords_m.h"
#include "rn_face/head_cords_m_array.h"

#include <sys/time.h>
#include <chrono>
#include <ctime>

#define CV_NO_BACKWARD_COMPATIBILITY

using namespace std;
using namespace cv;

struct face {
	int fid;
	int x;
	int y;
	int r;
};
//face *Faces;
//face Faces[10];
struct rgb_map
{
	int rgb_ar[10][3];
	
	rgb_map():rgb_ar({	{220,20,60},
				{0,255,255},
				{255,200,100},
				{255,130,171},
				{138,43,226},
				{65,105,225},
				{162,181,205},
				{0,255,127},
				{61,145,64},
				{255,255,0}	}){}
};

struct kinect_mesg
{
	double time;
	double id[6];
	double x[6];
	double y[0];
	double z[0];
};
rgb_map rgb_map_;
const int uid = 10;
const int rgbval = 3;
//int rgb_map[uid][rgbval];

int NO_FACES = 1;
int SPEAKER_ID = -1;
int MAX_PERSONS = 6;
int SESSION_MAX = 0;
int SKEL_SET = 0 ;
int ADDR_SET = 0 ;
int detected = 0;
int flash = 0 ;
int displaying =0;
int occlusion = 1;
int count_occlusion = 0;

std::string data_adr="";
struct face_{
//-------------------
//original toy alex
	IplImage *image,*hsv,*hue,*mask,*backproject,*histimg;
	CvHistogram *hist;

	int backproject_mode ;//= 0;
	int select_object ;//= 0;
	int track_object ;//= 0;
	int show_hist ;//= 1;
	int bin_w;
	int hdims ;//= 42;
	int vmin ;//= 10;
	int vmax ;//= 250; 
	int smin ;//= 30;
	int id_;
	int fid;
	int x_,y_,z_;
	//int occlusion ; //=0;
	//int count_occlusion; //=1;
	float hranges_arr[2];// = {0,500};
	float* hranges ;//= hranges_arr;
	float max_val;
	//double head_stick[2];
	//double head_stick_before_occlusion[2];
	double head_stick;
	double head_stick_before_occlusion;
//toy alex dhlwnontan sth main
//pleon melos tis domhs
	char number[10],id[2],x[10],y[10],z[10];
	
	CvPoint origin;
	CvRect selection;
	CvRect track_window;
	CvBox2D track_box;
	CvConnectedComp track_comp;
	
	face_():image(0),hsv(0),hue(0),mask(0),backproject(0),histimg(0),backproject_mode(0),select_object(0),track_object(0),
	show_hist(1),hdims(42),/*occlusion(1),count_occlusion(0),*/
	vmin(10),vmax(250),smin(30),id_(-1),fid(0),x_(0),y_(0),z_(0),
	hranges_arr({0.500}),hranges(hranges_arr),max_val(0),head_stick(0),head_stick_before_occlusion(0){}/*head_stick({0,0}),head_stick_before_occlusion({0,0})*/
};


face* detectAndDraw( Mat& img,
					CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
					double scale);

/*
void detectAndDraw( Mat& img,
					CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
					double scale);
*/
String cascadeName = "./haarcascades/haarcascade_frontalface_alt.xml";
String nestedCascadeName = "./haarcascades/haarcascade_eye_tree_eyeglasses.xml";



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



face* detectAndDraw( Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, 
//void detectAndDraw( Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, 
		double scale){
	
	
	face Proswpa[10];
	char string_faces[20];
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
	//|CV_HAAR_FIND_BIGGEST_OBJECT
	//|CV_HAAR_DO_ROUGH_SEARCH
    cascade.detectMultiScale( smallImg, faces,1.1, 2, 0|CV_HAAR_SCALE_IMAGE,Size(30, 30) );
	
    t = (double)cvGetTickCount() - t;
    printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );
	
    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++,i++/*, i++*/ )
    {
		if(i==NO_FACES){break;}
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = colors[i%8];
        int radius;
        center.x = cvRound((r->x + r->width*0.5)*scale);
        center.y = cvRound((r->y + r->height*0.5)*scale);
        radius = cvRound((r->width + r->height)*0.25*scale);
        circle( img, center, radius, color, 3, 8, 0 );
		
		//snprintf(string_faces, sizeof(string_faces), "face_%d", i);
		snprintf(string_faces, sizeof(string_faces), "face_%d", i+1);
		putText(img, string_faces, center, FONT_HERSHEY_COMPLEX_SMALL, 0.8, color, 1, CV_AA);
		
		if (i==0) {
			Proswpa[i].fid = 0;
			Proswpa[i].x = center.x;
			Proswpa[i].y = center.y;
			Proswpa[i].r = radius;
			//printf("i=%d\n",i);
			printf("-------------\n%d %d %d %d\n\n",Proswpa[i].fid,Proswpa[i].x,Proswpa[i].y,Proswpa[i].r);
		}
		
		if (i==1) {
			Proswpa[i].fid = 1;
			Proswpa[i].x = center.x;
			Proswpa[i].y = center.y;
			Proswpa[i].r = radius;
			//printf("i=%d\n",i);
			printf("-------------\n%d %d %d %d\n\n",Proswpa[i].fid,Proswpa[i].x,Proswpa[i].y,Proswpa[i].r);
			cvWaitKey(100);
			cv::imshow( "Face Detection and Tracking", img );
			sleep(2);
			detected = 1;
			occlusion = 1;
			return Proswpa;
		}
		
		//i++;
  }
	printf("Faces = %d\n",i-1);
    cv::imshow( "Face Detection and Tracking", img );
    
	return /*;*/Proswpa;
	
}

void no_faces_cb(const std_msgs::Int64::ConstPtr& msg)
{
	ROS_INFO("SET NO_FACES");
	NO_FACES=msg->data;
	return;
}
void speaker_id_cb(const std_msgs::Int64::ConstPtr& msg)
{
	ROS_INFO("SET SPEAKER_ID");
	SPEAKER_ID=msg->data;
	return;
}
void skelCallback(const rn_face::head_cords_m_array::ConstPtr& msg,struct face_ face_sa[])
{	
	//printf("%s \t %s -->\t %s \t %s \t %s \n", face_ac[c].number, face_ac[c].id_, face_ac[c].x, face_ac[c].y, face_ac[c].z);
	FILE *write = fopen ("sticks2.data","w");
	ROS_INFO("READING STICK DATA");
	for (int i=0; i<6; ++i)
    {
	  const rn_face::head_cords_m &data = msg->head_co[i];
      sprintf (face_sa[i].number, "%l", data.timestamp);
      sprintf (face_sa[i].id, "%d", data.id);
      sprintf (face_sa[i].x, "%f", data.x);
      sprintf (face_sa[i].y, "%f", data.y);
      sprintf (face_sa[i].z, "%f", data.z);
    }
    SKEL_SET=1;
    ROS_INFO("READ STICK DATA");
    ROS_INFO("%d",face_sa[0].id);
    ROS_INFO("%s",face_sa[0].x);
    ROS_INFO("%s",face_sa[0].y);
    ROS_INFO("%s",face_sa[0].z);

	return ;
}

void mcb (const std_msgs::String::ConstPtr & msg)
{
	ROS_INFO("SET IMG_ADDR");
	std::string pew = msg->data.c_str();
	if(!pew.empty())
			{data_adr=pew;
			ADDR_SET=1;}
	else {data_adr="";}
}
int main( int argc, char** argv )
{	

	if(argc>1)
    {
		SESSION_MAX=atoi(argv[1]);
	}
	else {SESSION_MAX=2;}
	//Hardcoded gia dokimes
	SESSION_MAX=2;
	NO_FACES=2;
	SPEAKER_ID=0;
	//
	face_  *face_sa = new face_[6];
	face *Faces = new face[10];
	int c,c2,c3;
	int i;
	int result;
	int _vmin[NO_FACES];
	int _vmax[NO_FACES];
	int val;
	//long int file_min = 1374935558156;
	long int file_min = 1375196305411;
	long int file_max = 1375196340719;
	char line[5000];
	char string_faces[20];
	char text[100];
	char str[80];
	char hist_str[80];
	char hist_str2[80];
	const char *str_ ;
	
	printf("Persons Tracked : %d\n",SESSION_MAX);
	
	//ROS
	ros::init(argc, argv, "recorder");
    ros::NodeHandle n;
	ros::Subscriber no_faces,im_adr,speaker_id,kinect_co;
	no_faces = n.subscribe<std_msgs::Int64>("no_faces",1,no_faces_cb);
	speaker_id = n.subscribe<std_msgs::Int64>("speaker_id",1,speaker_id_cb);
	kinect_co = n.subscribe<rn_face::head_cords_m_array>("head_cords",1,boost::bind(skelCallback, _1, face_sa));
	//im_adr = n.subscribe<std_msgs::String>("img_addr", 1, boost::bind(mainCallback, _1, face_sa));
	//
	im_adr = n.subscribe<std_msgs::String>("img_addr", 1, mcb);
	//ROS
	
	//OPENCV.Mat frame, frameCopy;
	Mat frame, frameCopy;
	CvFont font;
	cvNamedWindow("Face Detection and Tracking",1);
	cvInitFont(&font,CV_FONT_HERSHEY_PLAIN|CV_FONT_ITALIC,1,2,2,2);
	const String scaleOpt = "--scale=";
	size_t scaleOptLen = scaleOpt.length();
	const String cascadeOpt = "--cascade=";
	size_t cascadeOptLen = cascadeOpt.length();
	const String nestedCascadeOpt = "--nested-cascade";
	size_t nestedCascadeOptLen = nestedCascadeOpt.length();
	ROS_INFO("IN MAIN CALLBACK");
	String inputName;
	CascadeClassifier cascade, nestedCascade;
	double scale = 1;
	if( !cascade.load( cascadeName ) )
	{
			cerr << "ERROR: Could not load classifier cascade" << endl;
			cerr << "Usage: facedetect [--cascade=<cascade_path>]\n"
			"   [--nested-cascade[=nested_cascade_path]]\n"
			"   [--scale[=<image scale>\n"
			"   [filename|camera_index]\n" << endl ;
			return -1;
	}
	
	IplImage* iplImg;
	//OPENCV
	
	while (ros::ok())
    {	
		ROS_INFO("IN LOOP");
		ros::spinOnce();
		if ((SKEL_SET==1)&&(ADDR_SET==1))
		{
		ROS_INFO("CHECKING DATA_ADDR");
		//if(data_adr.empty()){continue;}
		ROS_INFO("CHECKED DATA_ADDR");
		//cvNamedWindow("Face Detection and Tracking",1);
		for (c=0;c<NO_FACES;c++)
		{
			ROS_INFO("ENTERED HIST LOOP");
			sprintf(hist_str,"Histogram_%d",c+1);
			//text += boost::lexical_cast<std::string>(c+1);
			cvNamedWindow(hist_str,1);
			strcpy(hist_str,"Histogram");
			ROS_INFO("LEFT HIST LOOP");
			
		}
		
	//MY_LOOP
	str_ = data_adr.c_str();
	result=access(str_, F_OK);
	if (result==0)
	{
		ROS_INFO("IN SECOND LOOP");
			iplImg = cvLoadImage(str_,1);
				frame = iplImg;
				if( frame.empty() )
					return 0;
				if( iplImg->origin == IPL_ORIGIN_TL )
					frame.copyTo( frameCopy );
				else
					flip( frame, frameCopy, 0 );
		ROS_INFO("CREATED IPL IMG");
		if(!face_sa[0].image)
		{	
			ROS_INFO("INITILIAZING HUES ETC");
			for(c=0;c<NO_FACES;c++)
			{		
					ROS_INFO("#1");
					face_sa[c].image=cvCreateImage( cvGetSize(iplImg), 8, 3 );
					ROS_INFO("#2");
					face_sa[c].image->origin=iplImg->origin;
					ROS_INFO("#3");
					face_sa[c].hsv=cvCreateImage( cvGetSize(iplImg), 8, 3 );
					ROS_INFO("#4");
					face_sa[c].hue=cvCreateImage( cvGetSize(iplImg), 8, 1 );
					ROS_INFO("#5");
					face_sa[c].mask=cvCreateImage( cvGetSize(iplImg), 8, 1 );
					ROS_INFO("#6");
					face_sa[c].backproject=cvCreateImage( cvGetSize(iplImg), 8, 1);
					ROS_INFO("#7");
					face_sa[c].hist=cvCreateHist(1 , &face_sa[c].hdims, CV_HIST_ARRAY, &face_sa[c].hranges, 1);//Creates a histogram.
					ROS_INFO("#8");
					face_sa[c].histimg = cvCreateImage( cvSize(320,200), 8, 3 ); //Creates an image header and allocates the image data. //CvSize size, int depth, int channels)
					ROS_INFO("#9");
					cvZero( face_sa[c].histimg );
					
					
			}
			ROS_INFO("INITILIAZED HUES ETC");

		}
		
		for (c=0;c<NO_FACES;c++)
		{
			cvCopy(iplImg, face_sa[c].image, 0);
			cvCvtColor(face_sa[c].image, face_sa[c].hsv, CV_BGR2HSV );
		}
		
		if (detected == 0)
		{	ROS_INFO("BEFORE DETECT AND DRAW");
			Faces = detectAndDraw ( frameCopy, cascade, nestedCascade, scale/*,face_sa*/);
			ROS_INFO("AFTER DETECT AND DRAW");
			for(c=0;c<NO_FACES;c++)
			{	ROS_INFO("AFTER D&D LOOP");
				//face_sa[c].x_=Faces[c].x;
				//face_sa[c].y_=Faces[c].y;
				//face_sa[c].r=Faces[c].r;
				//face_sa[c].id_=Faces[c].fid;
				ROS_INFO("%d   %d   %d   %d",Faces[c].fid,Faces[c].x,Faces[c].y,Faces[c].r);
				face_sa[c].track_object = -1;
				face_sa[c].selection = cvRect(Faces[c].x-Faces[c].r,Faces[c].y-Faces[c].r, Faces[c].x+Faces[c].r,Faces[c].y+Faces[c].r);				
				ROS_INFO("SX : %d, SY : %d",face_sa[c].selection.x, face_sa[c].selection.y);
				cvRectangle(face_sa[0].image, cvPoint(face_sa[c].selection.x, face_sa[c].selection.y), cvPoint(face_sa[c].selection.width, face_sa[c].selection.height),
				 CV_RGB(rgb_map_.rgb_ar[c][0],rgb_map_.rgb_ar[c][1],rgb_map_.rgb_ar[c][2]), 3, CV_AA, 0);
			}
			ROS_INFO("OUTSIDE THE LOOP");
			cvShowImage("Face Detection & Tracking",face_sa[0].image);
			for (c=0;c<NO_FACES;c++)
			{
				printf("->Face #%d-------------\n%d %d %d %d\n\n",c,Faces[c].fid,Faces[c].x,Faces[c].y,Faces[c].r);
			}
		}
		
		if (detected==1)
		{
			
			for (c=0;c<NO_FACES;c++)
			{
				_vmin[c]=face_sa[c].vmin;
				_vmax[c]=face_sa[c].vmax;
				ROS_INFO("BEFORE CV_INRANGES");
				//cvInRangeS( face_sa[c].hsv, cvScalar(0,face_sa[c].smin,MIN(_vmin[c],_vmax[c]),0),cvScalar(180,256,MAX(_vmin[c],_vmax[c]),0), face_sa[c].mask );
				cvInRangeS( face_sa[c].hsv, cvScalar(0,face_sa[c].smin,MIN(face_sa[c].vmin,face_sa[c].vmax),0),cvScalar(180,256,MAX(face_sa[c].vmin,face_sa[c].vmax),0), face_sa[c].mask );

				cvSplit( face_sa[c].hsv, face_sa[c].hue, 0, 0, 0 );
				ROS_INFO("AFTER CV_SPLIT");

				if(face_sa[c].track_object<0)
				{	
					ROS_INFO("IN TRACK LOOP");
					face_sa[c].max_val = 0.f;
					ROS_INFO("SET IMAGE ROI");
					cvSetImageROI( face_sa[c].hue, face_sa[c].selection );
					cvSetImageROI( face_sa[c].mask, face_sa[c].selection );
					ROS_INFO("CALC HIST");
					cvCalcHist( &face_sa[c].hue, face_sa[c].hist, 0, face_sa[c].mask );
					ROS_INFO("GETMINMAX");
					cvGetMinMaxHistValue( face_sa[c].hist, 0, &face_sa[c].max_val, 0, 0 );
					ROS_INFO("CONVERT SCALE");
					cvConvertScale( face_sa[c].hist->bins, face_sa[c].hist->bins, face_sa[c].max_val ? 255. / face_sa[c].max_val : 0., 0 );
					ROS_INFO("RESET IMAGE ROI");
					cvResetImageROI( face_sa[c].hue );
					cvResetImageROI( face_sa[c].mask );
					face_sa[c].track_window = face_sa[c].selection;
					//printf("track_window_1-------------\n%d %d %d %d\n\n",track_window.x, track_window.y, track_window.width, track_window.height);
					//sleep(10);
					face_sa[c].track_object = 1;
					cvZero( face_sa[c].histimg );
					face_sa[c].bin_w = face_sa[c].histimg->width / face_sa[c].hdims;
					ROS_INFO("BEFORE HDIMS LOOP");
					for( i = 0; i < face_sa[c].hdims; i++ )
					{	ROS_INFO("IN HDIMS LOOP");
						val = cvRound( cvGetReal1D(face_sa[c].hist->bins,i)*face_sa[c].histimg->height/255 );
						ROS_INFO("CVSCALAR");
						CvScalar color = hsv2rgb(i*180.f/face_sa[c].hdims);
						ROS_INFO("CVRECT");
						cvRectangle( face_sa[c].histimg, cvPoint(i*face_sa[c].bin_w,face_sa[c].histimg->height), cvPoint((i+1)*face_sa[c].bin_w,face_sa[c].histimg->height - val), color, -1, 8, 0 );
					}
					ROS_INFO("LEFT HDIMS LOOP");
					//printf("\n\nCamshift1\n\n");
					cvCalcBackProject( &face_sa[c].hue, face_sa[c].backproject, face_sa[c].hist ); //Calculates the back projection. --> ypologizei to kentro mazas me bash ta barh
					cvAnd( face_sa[c].backproject, face_sa[c].mask, face_sa[c].backproject, 0 );
					cvCamShift( face_sa[c].backproject, face_sa[c].track_window,cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ), &face_sa[c].track_comp, &face_sa[c].track_box );
					face_sa[c].track_window = face_sa[c].track_comp.rect;
					
					if( face_sa[c].backproject_mode ){
						cvCvtColor( face_sa[c].backproject, face_sa[c].image, CV_GRAY2BGR ); //Converts an image from one color space to another.
					}
					
					if( !face_sa[c].image->origin ){
						face_sa[c].track_box.angle = -face_sa[c].track_box.angle; // CvBox2D - track_box
					}
					
					cvEllipseBox( face_sa[c].image, face_sa[c].track_box, 
					CV_RGB(rgb_map_.rgb_ar[c][0],rgb_map_.rgb_ar[c][1],rgb_map_.rgb_ar[c][2]), 3, CV_AA, 0 );
					//cvCircle(image, track_box, 100 , CV_RGB(255,255,0), 1, CV_AA, 0 );
					
					//fgets(line, sizeof(line), file);
					
					//sscanf(line, "%s %s %s %s %s", &face_sa[c].number, &face_sa[c].id, &face_sa[c].x, &face_sa[c].y, &face_sa[c].z);
					printf("%s \t %s -->\t %s \t %s \t %s \n", face_sa[c].number, face_sa[c].id, face_sa[c].x, face_sa[c].y, face_sa[c].z);
					if(c==0)
					{
						//displaying=1;
						cvShowImage( "Face Detection and Tracking", face_sa[c].image );
						cvShowImage( "Histogram", face_sa[c].histimg );
					}
			
					//--------------- FACE OCCLUSION ---------------
					//int temp_id;
					if(NO_FACES>=2)
					{
						if (c==0)
						{
							if (occlusion == 1) {
								face_sa[c].head_stick=atof(face_sa[c].y);
								face_sa[c+1].head_stick=atof(face_sa[c+1].y);
								if (count_occlusion == 0) {
									face_sa[c].head_stick_before_occlusion=face_sa[c].head_stick;
									face_sa[c+1].head_stick_before_occlusion=face_sa[c+1].head_stick;
								}
								count_occlusion++;
								occlusion = 0;
								/*
								if (((face_sa[c].head_stick_before_occlusion>face_sa[c+1].head_stick_before_occlusion)&&
									(face_sa[c].head_stick>face_sa[c+1].head_stick))||
									((face_sa[c].head_stick_before_occlusion<face_sa[c+1].head_stick_before_occlusion)&&
									(face_sa[c].head_stick<face_sa[c+1].head_stick)))
								{
									face_sa[c].id=c+1;
									face_sa[c+1].id=
								}
								* */
								if (face_sa[c].head_stick_before_occlusion<0 && face_sa[c].head_stick>0) {
									face_sa[c].fid=c+1;
									face_sa[c+1].fid = c+2;
									face_sa[c].id_=c+1;
									face_sa[c+1].id_ = c+2;
								}
								
								if (face_sa[c].head_stick_before_occlusion>0 && face_sa[c].head_stick<0) {
									face_sa[c].id_=c;
									face_sa[c+1].id_= c+1;
								}
								
							}
						}
						
						else if (c==(NO_FACES-1))
						{
							if (occlusion == 1) 
							{
								face_sa[c].head_stick=atof(face_sa[c].y);
								face_sa[c-1].head_stick=atof(face_sa[c-1].y);
								if (count_occlusion == 0) 
								{
									face_sa[c].head_stick_before_occlusion=face_sa[c].head_stick;
									face_sa[c-1].head_stick_before_occlusion=face_sa[c-1].head_stick;
								}
								count_occlusion++;
								occlusion = 0;
								
								if (face_sa[c].head_stick_before_occlusion>0 && face_sa[c].head_stick<0) 
								{
									face_sa[c].id_=c+1;
									face_sa[c+1].id_ = c+2;
								}
								
								if (face_sa[c].head_stick_before_occlusion>0 && face_sa[c].head_stick<0) 
								{
									face_sa[c].id_=c;
									face_sa[c-1].id_= c+1;
								}
							}
						}
					}	
					}
					}	
				
					for(c=0;c<NO_FACES;c++)
					{
						printf("\nhead_stick %d = %g",c+1,face_sa[c].head_stick);
						printf("\n0_face_id=%d\n",face_sa[c].id_);
						sprintf(string_faces, "face_%d", face_sa[c].id_ );
						cvPutText(face_sa[c].image,string_faces,cvPoint(face_sa[c].track_window.x+50,face_sa[c].track_window.y+50),&font, CV_RGB(255,0,0));
						//--------- print coordinates of the Kinect stick models ----------
						sprintf(text, "face_%s = %s m", face_sa[c].id, face_sa[c].y);
						cvPutText(face_sa[c].image,text,cvPoint(10,430+c),&font, CV_RGB(255,255,100));
					}
					cvLine (face_sa[0].image,  (cvPoint(0,400)),(cvPoint(600,400)), CV_RGB(255,0,255), 3, 1, 0);
					
					cvLine (face_sa[0].image,  (cvPoint(290,390)),(cvPoint(290,410)), CV_RGB(255,0,255), 3, 1, 0);
					
					cvPutText(face_sa[0].image,"0",cvPoint(283,376),&font, CV_RGB(255,0,255));
					/*
					if (track_box.center.y > track_box2.center.y - 5 && track_box.center.y < track_box2.center.y + 5) {
						printf("\n\nOcclusion\n\n");
					}
					*/
					if(NO_FACES>1)
					{
						for(c=0;c<NO_FACES;c++)
						{	
							sprintf(hist_str2,"Histogram_%d",c+1);
							cvShowImage( "Face Detection and Tracking", face_sa[c].image );
							cvShowImage( hist_str2, face_sa[c].histimg );
							strcpy(hist_str2,"Histogram");
						}
					}
					
					cvWaitKey(10); // o xronos gia kathe frame
					
					
					//------------------------ press a key -----------------------
					
					c = cvWaitKey(100);
					if( (char) c == 27 ){
						return 0;
					}
					switch( (char) c ){
						case 'f':
							detected = 0;
							for(c=0;c<NO_FACES;c++)
							{	
								cvZero( face_sa[c].histimg );
							}
							sleep(1);
						return 0;
						case 's':
							sleep(5);
							return 0;	
						default:
							;
					}
					
					cvReleaseImage(&iplImg);
					
				}
					

		
	}
		}
	ROS_INFO("STILL INSIDE LOOP");
	}
	        
	
    cvDestroyWindow("Face Detection and Tracking");
	return 0;
}
