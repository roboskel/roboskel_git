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
int detected = 0;
int flash = 0 ;
int displaying =0;
int occlusion = 1;
int count_occlusion = 0;

IplImage *image = 0, *hsv = 0, *hue = 0, *mask = 0, *backproject = 0, *histimg = 0;
CvHistogram *hist = 0;

IplImage *image2 = 0, *hsv2 = 0, *hue2 = 0, *mask2 = 0, *backproject2 = 0, *histimg2 = 0;
CvHistogram *hist2 = 0;

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
	show_hist(0),hdims(0),/*occlusion(1),count_occlusion(0),*/
	vmin(10),vmax(250),smin(30),id_(-1),fid(0),x_(0),y_(0),z_(0),
	hranges_arr({0.500}),max_val(0),head_stick(0),head_stick_before_occlusion(0){}/*head_stick({0,0}),head_stick_before_occlusion({0,0})*/
};


face* detectAndDraw( Mat& img,
					CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
					double scale);

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



int mainCallback(const std_msgs::String::ConstPtr& msg, struct face_ face_ac[])
{
	std::string data_adr=msg->data.c_str();
	if(data_adr.empty()){return 0;}
    int c,c2,c3;
	
	//FILE *file = fopen("sticks.data2","r");
	//ALEX
	//char number1[10], id1[2], x1[10], y1[10], z1[10];
	//char number2[10], id2[2], x2[10], y2[10], z2[10];
	char line[5000];
	//
	ROS_INFO("IN MAIN CALLBACK");
	//CvCapture* capture = 0;
    Mat frame, frameCopy;
	face *Faces;
	int i;//, bin_w, bin_w2;
	char string_faces[20];
	CvFont font;
	cvInitFont(&font,CV_FONT_HERSHEY_PLAIN|CV_FONT_ITALIC,1,2,2,2);
	
	char text[100];
	
	int result;
	//long int file_min = 1374935558156;
	long int file_min = 1375196305411;
	long int file_max = 1375196340719;
	
	
	char str[80];
	
	char hist_str[80];
	char hist_str2[80];
	
    const String scaleOpt = "--scale=";
    size_t scaleOptLen = scaleOpt.length();
    const String cascadeOpt = "--cascade=";
    size_t cascadeOptLen = cascadeOpt.length();
    const String nestedCascadeOpt = "--nested-cascade";
    size_t nestedCascadeOptLen = nestedCascadeOpt.length();
	
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
   
	//cvNamedWindow("Face Detection and Tracking",1);
	for (c=0;c<NO_FACES;c++)
	{
		sprintf(hist_str,"Histogram_%d",c+1);
		//text += boost::lexical_cast<std::string>(c+1);
		cvNamedWindow(hist_str,1);
		strcpy(hist_str,"Histogram");
		
	}
	
//MY_LOOP
const char *str_ = data_adr.c_str();
result=access(str_, F_OK);
if (result==0)
{
	IplImage* iplImg = cvLoadImage(str_,1);
			frame = iplImg;
			if( frame.empty() )
				return 0;
			if( iplImg->origin == IPL_ORIGIN_TL )
				frame.copyTo( frameCopy );
			else
				flip( frame, frameCopy, 0 );
	if(!image)
	{
		for(c=0;c<NO_FACES;c++)
		{
				face_ac[c].image=cvCreateImage( cvGetSize(iplImg), 8, 3 );
				face_ac[c].image->origin=iplImg->origin;
				face_ac[c].hsv=cvCreateImage( cvGetSize(iplImg), 8, 3 );
				face_ac[c].hue=cvCreateImage( cvGetSize(iplImg), 8, 1 );
				face_ac[c].mask=cvCreateImage( cvGetSize(iplImg), 8, 1 );
				face_ac[c].backproject=cvCreateImage( cvGetSize(iplImg), 8, 1);
				face_ac[c].hist=cvCreateHist(1 , &face_ac[c].hdims, CV_HIST_ARRAY, &face_ac[c].hranges, 1);//Creates a histogram.
				face_ac[c].histimg = cvCreateImage( cvSize(320,200), 8, 3 ); //Creates an image header and allocates the image data. //CvSize size, int depth, int channels)
				cvZero( face_ac[c].histimg );
				
				
		}
	}
	
	for (c=0;c<NO_FACES;c++)
	{
		cvCopy(iplImg, face_ac[c].image, 0);
		cvCvtColor(face_ac[c].image, face_ac[c].hsv, CV_BGR2HSV );
	}
	
	if (detected == 0)
	{
		Faces = detectAndDraw ( frameCopy, cascade, nestedCascade, scale/*,face_ac*/);
		for(c=0;c<NO_FACES;c++)
		{	face_ac[c].x_=Faces[c].x;
			face_ac[c].y_=Faces[c].y;
			//face_ac[c].r=Faces[c].r;
			face_ac[c].id_=Faces[c].fid;
			face_ac[c].track_object = -1;
			cvRectangle(face_ac[c].image, cvPoint(face_ac[c].selection.x, face_ac[c].selection.y), cvPoint(face_ac[c].selection.width, face_ac[c].selection.height),
			 CV_RGB(rgb_map_.rgb_ar[c][0],rgb_map_.rgb_ar[c][1],rgb_map_.rgb_ar[c][2]), 3, CV_AA, 0);
		}
		cvShowImage("Face Detection & Tracking",face_ac[0].image);
		for (c=0;c<NO_FACES;c++)
		{
			printf("Face #%d-------------\n%d %d %d %d\n\n",c,Faces[i].fid,Faces[i].x,Faces[i].y,Faces[i].r);
		}
	}
	
	if (detected==1)
	{
		int _vmin[NO_FACES];
		int _vmax[NO_FACES];
		int val;
		for (c=0;c<NO_FACES;c++)
		{
			_vmin[c]=face_ac[c].vmin;
			_vmax[c]=face_ac[c].vmax;
			cvInRangeS( face_ac[c].hsv, cvScalar(0,face_ac[c].smin,MIN(_vmin[c],_vmax[c]),0),cvScalar(180,256,MAX(_vmin[c],_vmax[c]),0), face_ac[c].mask );
			cvSplit( face_ac[c].hsv, face_ac[c].hue, 0, 0, 0 );
			if(face_ac[c].track_object<0)
			{
				face_ac[c].max_val = 0.f;
				cvSetImageROI( face_ac[c].hue, face_ac[c].selection );
				cvSetImageROI( face_ac[c].mask, face_ac[c].selection );
				cvCalcHist( &face_ac[c].hue, face_ac[c].hist, 0, face_ac[c].mask );
				cvGetMinMaxHistValue( face_ac[c].hist, 0, &face_ac[c].max_val, 0, 0 );
				cvConvertScale( face_ac[c].hist->bins, face_ac[c].hist->bins, face_ac[c].max_val ? 255. / face_ac[c].max_val : 0., 0 );
				cvResetImageROI( face_ac[c].hue );
				cvResetImageROI( face_ac[c].mask );
				face_ac[c].track_window = face_ac[c].selection;
				//printf("track_window_1-------------\n%d %d %d %d\n\n",track_window.x, track_window.y, track_window.width, track_window.height);
				//sleep(10);
				face_ac[c].track_object = 1;
				cvZero( face_ac[c].histimg );
				face_ac[c].bin_w = face_ac[c].histimg->width / face_ac[c].hdims;
				
				for( i = 0; i < face_ac[c].hdims; i++ )
				{
					val = cvRound( cvGetReal1D(hist->bins,i)*histimg->height/255 );
					CvScalar color = hsv2rgb(i*180.f/face_ac[c].hdims);
					cvRectangle( histimg, cvPoint(i*face_ac[c].bin_w,face_ac[c].histimg->height), cvPoint((i+1)*face_ac[c].bin_w,face_ac[c].histimg->height - val), color, -1, 8, 0 );
				}
				
				//printf("\n\nCamshift1\n\n");
				cvCalcBackProject( &face_ac[c].hue, face_ac[c].backproject, face_ac[c].hist ); //Calculates the back projection. --> ypologizei to kentro mazas me bash ta barh
				cvAnd( face_ac[c].backproject, face_ac[c].mask, face_ac[c].backproject, 0 );
				cvCamShift( face_ac[c].backproject, face_ac[c].track_window,cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ), &face_ac[c].track_comp, &face_ac[c].track_box );
				face_ac[c].track_window = face_ac[c].track_comp.rect;
				
				if( face_ac[c].backproject_mode ){
					cvCvtColor( face_ac[c].backproject, face_ac[c].image, CV_GRAY2BGR ); //Converts an image from one color space to another.
				}
				
				if( !face_ac[c].image->origin ){
					face_ac[c].track_box.angle = -face_ac[c].track_box.angle; // CvBox2D - track_box
				}
				
				cvEllipseBox( face_ac[c].image, face_ac[c].track_box, 
				CV_RGB(rgb_map_.rgb_ar[c][0],rgb_map_.rgb_ar[c][1],rgb_map_.rgb_ar[c][2]), 3, CV_AA, 0 );
				//cvCircle(image, track_box, 100 , CV_RGB(255,255,0), 1, CV_AA, 0 );
				
				//fgets(line, sizeof(line), file);
				
				//sscanf(line, "%s %s %s %s %s", &face_ac[c].number, &face_ac[c].id, &face_ac[c].x, &face_ac[c].y, &face_ac[c].z);
				printf("%s \t %s -->\t %s \t %s \t %s \n", face_ac[c].number, face_ac[c].id, face_ac[c].x, face_ac[c].y, face_ac[c].z);
				if(c==0)
				{
					//displaying=1;
					cvShowImage( "Face Detection and Tracking", face_ac[c].image );
					cvShowImage( "Histogram", face_ac[c].histimg );
				}
		
				//--------------- FACE OCCLUSION ---------------
				//int temp_id;
				if(NO_FACES>=2)
				{
					if (c==0)
					{
						if (occlusion == 1) {
							face_ac[c].head_stick=atof(face_ac[c].y);
							face_ac[c+1].head_stick=atof(face_ac[c+1].y);
							if (count_occlusion == 0) {
								face_ac[c].head_stick_before_occlusion=face_ac[c].head_stick;
								face_ac[c+1].head_stick_before_occlusion=face_ac[c+1].head_stick;
							}
							count_occlusion++;
							occlusion = 0;
							/*
							if (((face_ac[c].head_stick_before_occlusion>face_ac[c+1].head_stick_before_occlusion)&&
								(face_ac[c].head_stick>face_ac[c+1].head_stick))||
								((face_ac[c].head_stick_before_occlusion<face_ac[c+1].head_stick_before_occlusion)&&
								(face_ac[c].head_stick<face_ac[c+1].head_stick)))
							{
								face_ac[c].id=c+1;
								face_ac[c+1].id=
							}
							* */
							if (face_ac[c].head_stick_before_occlusion<0 && face_ac[c].head_stick>0) {
								face_ac[c].fid=c+1;
								face_ac[c+1].fid = c+2;
								face_ac[c].id_=c+1;
								face_ac[c+1].id_ = c+2;
							}
							
							if (face_ac[c].head_stick_before_occlusion>0 && face_ac[c].head_stick<0) {
								face_ac[c].id_=c;
								face_ac[c+1].id_= c+1;
							}
							
						}
					}
					
					else if (c==(NO_FACES-1))
					{
						if (occlusion == 1) 
						{
							face_ac[c].head_stick=atof(face_ac[c].y);
							face_ac[c-1].head_stick=atof(face_ac[c-1].y);
							if (count_occlusion == 0) 
							{
								face_ac[c].head_stick_before_occlusion=face_ac[c].head_stick;
								face_ac[c-1].head_stick_before_occlusion=face_ac[c-1].head_stick;
							}
							count_occlusion++;
							occlusion = 0;
							
							if (face_ac[c].head_stick_before_occlusion>0 && face_ac[c].head_stick<0) 
							{
								face_ac[c].id_=c+1;
								face_ac[c+1].id_ = c+2;
							}
							
							if (face_ac[c].head_stick_before_occlusion>0 && face_ac[c].head_stick<0) 
							{
								face_ac[c].id_=c;
								face_ac[c-1].id_= c+1;
							}
						}
					}
				}	
				}
				}	
			
				for(c=0;c<NO_FACES;c++)
				{
					printf("\nhead_stick %d = %g",c+1,face_ac[c].head_stick);
					printf("\n0_face_id=%d\n",face_ac[c].id_);
					sprintf(string_faces, "face_%d", face_ac[c].id_ );
					cvPutText(face_ac[c].image,string_faces,cvPoint(face_ac[c].track_window.x+50,face_ac[c].track_window.y+50),&font, CV_RGB(255,0,0));
					//--------- print coordinates of the Kinect stick models ----------
					sprintf(text, "face_%s = %s m", face_ac[c].id, face_ac[c].y);
					cvPutText(face_ac[c].image,text,cvPoint(10,430+c),&font, CV_RGB(255,255,100));
				}
				cvLine (face_ac[0].image,  (cvPoint(0,400)),(cvPoint(600,400)), CV_RGB(255,0,255), 3, 1, 0);
				
				cvLine (face_ac[0].image,  (cvPoint(290,390)),(cvPoint(290,410)), CV_RGB(255,0,255), 3, 1, 0);
				
				cvPutText(face_ac[0].image,"0",cvPoint(283,376),&font, CV_RGB(255,0,255));
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
						cvShowImage( "Face Detection and Tracking", face_ac[c].image );
						cvShowImage( hist_str2, face_ac[c].histimg );
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
							cvZero( face_ac[c].histimg );
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
//
    //cvDestroyWindow("Face Detection and Tracking");
    return 0;
}





















face* detectAndDraw( Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, 
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
		
		
		snprintf(string_faces, sizeof(string_faces), "face_%d", i+1);
		putText(img, string_faces, center, FONT_HERSHEY_COMPLEX_SMALL, 0.8, color, 1, CV_AA);
		
		//printf("i=%d\n",i);
		Proswpa[i].fid = i;
		Proswpa[i].x = center.x;
		Proswpa[i].y = center.y;
		Proswpa[i].r = radius;
		printf("-------------\n%d %d %d %d\n\n",Proswpa[i].fid,Proswpa[i].x,Proswpa[i].y,Proswpa[i].r);
		/*
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
			return Proswpa;
		}
		*/
		
		
		
		
    }
    cvWaitKey(100);
	printf("Faces = %d\n",i);
    cv::imshow( "Face Detection and Tracking", img );
    sleep(2);
	detected = 1;
	return Proswpa;
	
}

void no_faces_cb(const std_msgs::Int64::ConstPtr& msg)
{
	NO_FACES=msg->data;
	return;
}
void speaker_id_cb(const std_msgs::Int64::ConstPtr& msg)
{
	SPEAKER_ID=msg->data;
	return;
}
void skelCallback(const rn_face::head_cords_m_array::ConstPtr& msg,struct face_ face_sa[])
{	
	//printf("%s \t %s -->\t %s \t %s \t %s \n", face_ac[c].number, face_ac[c].id_, face_ac[c].x, face_ac[c].y, face_ac[c].z);
	FILE *write = fopen ("sticks2.data","w");

	for (int i=0; i<6; ++i)
    {
	  const rn_face::head_cords_m &data = msg->head_co[i];
      sprintf (face_sa[i].number, "%l", data.timestamp);
      sprintf (face_sa[i].id, "%d", data.id);
      sprintf (face_sa[i].x, "%f", data.x);
      sprintf (face_sa[i].y, "%f", data.y);
      sprintf (face_sa[i].z, "%f", data.z);
    }
	return ;
}

int main( int argc, char** argv )
{	
	/*rgb_map={	{220,20,60},
				{0,255,255},
				{255,200,100},
				{255,130,171},
				{138,43,226},
				{65,105,225},
				{162,181,205},
				{0,255,127},
				{61,145,64},
				{255,255,0}	}; */
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
	face_ * face_sa = new face_[SESSION_MAX];
	printf("Persons Tracked : %d\n",SESSION_MAX);
	ros::init(argc, argv, "recorder");
    ros::NodeHandle n;
	ros::Subscriber no_faces,im_adr,speaker_id,kinect_co;
	no_faces = n.subscribe<std_msgs::Int64>("no_faces",1,no_faces_cb);
	speaker_id = n.subscribe<std_msgs::Int64>("speaker_id",1,speaker_id_cb);
	kinect_co = n.subscribe<rn_face::head_cords_m_array>("head_cords",1,boost::bind(skelCallback, _1, face_sa));
	//im_adr = n.subscribe<std_msgs::String>("img_addr", 1, boost::bind(mainCallback, _1, face_sa));
	//im_adr = n.subscribe<std_msgs::String>("img_addr", 1, boost::bind(mcb, _1, face_sa));
	cvNamedWindow("Face Detection and Tracking",1);
	while (n.ok())
    {
        ros::spin();
	}
    cvDestroyWindow("Face Detection and Tracking");
	return 0;
}
