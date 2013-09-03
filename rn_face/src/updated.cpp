// Compile
// g++ `pkg-config --cflags opencv`   `pkg-config --libs opencv` face_detection_and_tracking_2_images_access_updated.cpp -o face_detection_and_tracking_2_images_access_updated -w

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define CV_NO_BACKWARD_COMPATIBILITY

#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <ctype.h>

#include <iostream>
#include <stdio.h>
#include <unistd.h>

using namespace std;
using namespace cv;

struct face {
	int fid;
	int x;
	int y;
	int r;
};

int detected = 0;

IplImage *image = 0, *hsv = 0, *hue = 0, *mask = 0, *backproject = 0, *histimg = 0;
CvHistogram *hist = 0;

IplImage *image2 = 0, *hsv2 = 0, *hue2 = 0, *mask2 = 0, *backproject2 = 0, *histimg2 = 0;
CvHistogram *hist2 = 0;

//-------------------
int backproject_mode = 0;
int select_object = 0;
int track_object = 0;
int show_hist = 1;
CvPoint origin;
CvRect selection;
CvRect track_window;
CvBox2D track_box;
CvConnectedComp track_comp;
int hdims = 42;
float hranges_arr[] = {0,500};
float* hranges = hranges_arr;
int vmin = 10, vmax = 250, smin = 30;
//-------------------
int backproject_mode2 = 0;
int select_object2 = 0;
int track_object2 = 0;
int show_hist2 = 1;
CvPoint origin2;
CvRect selection2;
CvRect track_window2;
CvBox2D track_box2;
CvConnectedComp track_comp2;
int hdims2 = 42;
float hranges_arr2[] = {0,500};
float* hranges2 = hranges_arr2;
int vmin2 = vmin, vmax2 = vmax, smin2 = smin;
int occlusion = 1;
int count_occlusion = 0;
//------------------------
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



int main( int argc, const char** argv )
{
    int c;
	FILE *file = fopen("sticks.data","r");
	char number1[10], id1[2], x1[10], y1[10], z1[10];
	char number2[10], id2[2], x2[10], y2[10], z2[10];
	char line[5000];
	int face_id_1=1;
	int face_id_2=2;
	double head_stick[2];
	double head_stick_before_occlusion[2];
	CvCapture* capture = 0;
    Mat frame, frameCopy;
	face *Faces;
	int i, bin_w, bin_w2;
	char string_faces[20];
	CvFont font;
	cvInitFont(&font,CV_FONT_HERSHEY_PLAIN|CV_FONT_ITALIC,1,2,2,2);
	char text[100];
	int result;
	//long int file_min = 1374935558156;
	long int file_min = 1375196305411;
	long int file_max = 1375196340719;
	char str[80];
	
	
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
	
    cvNamedWindow( "Face Detection and Tracking", 1 );
	cvNamedWindow( "Histogram", 1 );
	cvNamedWindow( "Histogram2", 1 );
	
	
	
	while (file_min<file_max+1) {
		
		sprintf (str, "Images/image_%ld.jpg",file_min);
		file_min = file_min + 1;
		result = access (str, F_OK); // F_OK tests existence also (R_OK,W_OK,X_OK).
		//            for readable, writeable, executable
		if ( result == 0 )
		{
			//printf("\n\nBRIKA: %s exists!!\n\n\n",str);
			
						
			IplImage* iplImg = cvLoadImage(str,1);
			frame = iplImg;
			if( frame.empty() )
				break;
			if( iplImg->origin == IPL_ORIGIN_TL )
				frame.copyTo( frameCopy );
			else
				flip( frame, frameCopy, 0 );
			
			
			//-----------Initialize the tracking parameters------------
			
			if (!image) {
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
				
				image2 = cvCreateImage( cvGetSize(iplImg), 8, 3 );
				image2->origin = iplImg->origin;
				hsv2 = cvCreateImage( cvGetSize(iplImg), 8, 3 );
				hue2 = cvCreateImage( cvGetSize(iplImg), 8, 1 );
				mask2 = cvCreateImage( cvGetSize(iplImg), 8, 1 );
				backproject2 = cvCreateImage( cvGetSize(iplImg), 8, 1 );
				hist2 = cvCreateHist( 1, &hdims2, CV_HIST_ARRAY, &hranges2, 1 ); //Creates a histogram.
				histimg2 = cvCreateImage( cvSize(320,200), 8, 3 ); //Creates an image header and allocates the image data. //CvSize size, int depth, int channels)
				cvZero( histimg2 );
			}
			
			//--------------------------------------------------------
			
			cvCopy( iplImg, image, 0 );
			cvCvtColor( image, hsv, CV_BGR2HSV );
			
			cvCopy( iplImg, image2, 0 );
			cvCvtColor( image2, hsv2, CV_BGR2HSV );
			
			
			if (detected == 0) {
				Faces = detectAndDraw( frameCopy, cascade, nestedCascade, scale );
				track_object = -1;
				track_object2 = -1;
				selection = cvRect(Faces[0].x-Faces[0].r,Faces[0].y-Faces[0].r, Faces[0].x+Faces[0].r,Faces[0].y+Faces[0].r);				
				selection2 = cvRect(Faces[1].x-Faces[1].r,Faces[1].y-Faces[1].r, Faces[1].x+Faces[1].r,Faces[1].y+Faces[1].r);
				// Check if faces are selected
				cvRectangle(image, cvPoint(selection.x, selection.y), cvPoint(selection.width, selection.height), CV_RGB(0,255,255), 3, CV_AA, 0);
				cvRectangle(image, cvPoint(selection2.x, selection2.y), cvPoint(selection2.width, selection2.height), CV_RGB(255,200,100), 3, CV_AA, 0);
				cvShowImage( "Face Detection and Tracking", image );
				for (i=0; i<2; i++) {
					printf("Face_%d-------------\n%d %d %d %d\n\n",i,Faces[i].fid,Faces[i].x,Faces[i].y,Faces[i].r);
				}
			}
			
			//-------- 2 faces have been detected-----------------
			
			if (detected == 1) {
				
				int _vmin = vmin, _vmax = vmax;
				cvInRangeS( hsv, cvScalar(0,smin,MIN(_vmin,_vmax),0),cvScalar(180,256,MAX(_vmin,_vmax),0), mask );
				cvSplit( hsv, hue, 0, 0, 0 );
				
				//----------------------track FACE 1 ------------------------
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
					//printf("track_window_1-------------\n%d %d %d %d\n\n",track_window.x, track_window.y, track_window.width, track_window.height);
					//sleep(10);
					track_object = 1;
					cvZero( histimg );
					bin_w = histimg->width / hdims;
					for( i = 0; i < hdims; i++ ){
						int val = cvRound( cvGetReal1D(hist->bins,i)*histimg->height/255 );
						CvScalar color = hsv2rgb(i*180.f/hdims);
						cvRectangle( histimg, cvPoint(i*bin_w,histimg->height), cvPoint((i+1)*bin_w,histimg->height - val), color, -1, 8, 0 );
					}
				}
				//printf("\n\nCamshift1\n\n");
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
				
				cvEllipseBox( image, track_box, CV_RGB(255,0,0), 3, CV_AA, 0 );
				//cvCircle(image, track_box, 100 , CV_RGB(255,255,0), 1, CV_AA, 0 );
				
				fgets(line, sizeof(line), file);
				sscanf(line, "%s %s %s %s %s", &number1, &id1, &x1, &y1, &z1);
				printf("%s \t %s -->\t %s \t %s \t %s \n", number1, id1, x1, y1, z1);

				cvShowImage( "Face Detection and Tracking", image );
				cvShowImage( "Histogram", histimg );
				
				int _vmin2 = vmin2, _vmax2 = vmax2;
				cvInRangeS( hsv2, cvScalar(0,smin2,MIN(_vmin2,_vmax2),0),cvScalar(180,256,MAX(_vmin2,_vmax2),0), mask2 );
				cvSplit( hsv2, hue2, 0, 0, 0 );
				//printf("lala\n\n");
				
				//----------------------track FACE 2 ------------------------
				
				if (track_object2 < 0) {
					float max_val2 = 0.f;
					cvSetImageROI( hue2, selection2 );
					cvSetImageROI( mask2, selection2 );
					cvCalcHist( &hue2, hist2, 0, mask2 );
					cvGetMinMaxHistValue( hist2, 0, &max_val2, 0, 0 );					
					cvConvertScale( hist2->bins, hist2->bins, max_val2 ? 255. / max_val2 : 0., 0 );
					cvResetImageROI( hue2 );
					cvResetImageROI( mask2 );
					track_window2 = selection2;
					track_object2 = 1;
					cvZero( histimg2 );
					bin_w2 = histimg2->width / hdims2;
					for( i = 0; i < hdims2; i++ ){
						int val2 = cvRound( cvGetReal1D(hist2->bins,i)*histimg2->height/255 );
						CvScalar color2 = hsv2rgb(i*180.f/hdims2);
						cvRectangle( histimg2, cvPoint(i*bin_w2,histimg2->height), cvPoint((i+1)*bin_w2,histimg2->height - val2), color2, -1, 8, 0 );
					}
				}
				cvCalcBackProject( &hue2, backproject2, hist2 ); //Calculates the back projection. --> ypologizei to kentro mazas me bash ta barh
				cvAnd( backproject2, mask2, backproject2, 0 );
				cvCamShift( backproject2, track_window2,cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ), &track_comp2, &track_box2 );
				track_window2 = track_comp2.rect;
				if( backproject_mode2 ){
					cvCvtColor( backproject2, image, CV_GRAY2BGR ); //Converts an image from one color space to another.
				}
				if( !image2->origin ){
					track_box2.angle = -track_box2.angle; // CvBox2D - track_box
				}
				cvEllipseBox( image, track_box2, CV_RGB(255,255,0), 3, CV_AA, 0 );
				//cvCircle(image, track_box, 100 , CV_RGB(255,255,0), 1, CV_AA, 0 );
	
				fgets(line, sizeof(line), file);
				sscanf(line, "%s %s %s %s %s", &number2, &id2, &x2, &y2, &z2);
				printf("%s \t %s -->\t %s \t %s \t %s \n", number2, id2, x2, y2, z2);
				
				
				//--------------- FACE OCCLUSION ---------------

				
				if (occlusion == 1) {
					head_stick[0]=atof(y1);
					head_stick[1]=atof(y2);
					if (count_occlusion == 0) {
						head_stick_before_occlusion[0]=head_stick[0];
						head_stick_before_occlusion[1]=head_stick[1];
					}
					count_occlusion++;
					occlusion = 0;
					
					if (head_stick_before_occlusion[0]<0 && head_stick[0]>0) {
						face_id_1=2;
						face_id_2 = 1;
					}
					
					if (head_stick_before_occlusion[0]>0 && head_stick[0]<0) {
						face_id_1=1;
						face_id_2 = 2;
					}
					
				}
				
				printf("\nhead_stick_1 = %g\n",head_stick[0]);
				printf("\nhead_stick_2 = %g\n",head_stick[1]);
				
				
				//int face_id_1 = 1;
				
				printf("\n0_face_id=%d\n",face_id_1);
				sprintf(string_faces, "face_%d", face_id_1 );
				cvPutText(image,string_faces,cvPoint(track_window.x+50,track_window.y+50),&font, CV_RGB(255,0,0));
				
				
				//int face_id_2 = 2;
				printf("\n0_face_id=%d\n",face_id_2);
				sprintf(string_faces, "face_%d", face_id_2 );
				cvPutText(image,string_faces,cvPoint(track_window2.x+50,track_window2.y+50),&font, CV_RGB(255,0,0));

				printf("\ntrack_box.center.y = %f\n",track_box.center.y);
				printf("\ntrack_box2.center.y = %f\n",track_box2.center.y);				
				if (track_box.center.y > track_box2.center.y - 5 && track_box.center.y < track_box2.center.y + 5) {
					printf("\n\nOcclusion\n\n");
				}
				
				//------------------------------------------------
				
				
				//--------- print coordinates of the Kinect stick models ----------
				
				sprintf(text, "face_%s = %s m", id1, y1);
				cvPutText(image,text,cvPoint(10,430),&font, CV_RGB(255,255,100));
				
				sprintf(text, "face_%s = %s m", id2,y2);
				cvPutText(image,text,cvPoint(10,450),&font, CV_RGB(255,255,100));
				
				cvLine (image,  (cvPoint(0,400)),(cvPoint(600,400)), CV_RGB(255,0,255), 3, 1, 0);
				
				cvLine (image,  (cvPoint(290,390)),(cvPoint(290,410)), CV_RGB(255,0,255), 3, 1, 0);
				
				cvPutText(image,"0",cvPoint(283,376),&font, CV_RGB(255,0,255));

				
				cvShowImage( "Face Detection and Tracking", image );
				cvShowImage( "Histogram2", histimg2 );
				cvWaitKey(10); // o xronos gia kathe frame
				
				
				//------------------------ press a key -----------------------
				
				c = cvWaitKey(100);
				if( (char) c == 27 ){
					break;
				}
				switch( (char) c ){
					case 'f':
						detected = 0;	
						cvZero( histimg );
						sleep(1);
					break;
					case 's':
						sleep(5);
						break;	
					default:
						;
				}
				
				cvReleaseImage(&iplImg);
				
			}
			
		}

	}
	
    cvDestroyWindow("Face Detection and Tracking");
    return 0;
}





face* detectAndDraw( Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, double scale){
	
	
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
		
		
		
		
    }
	printf("Faces = %d\n",i);
    cv::imshow( "Face Detection and Tracking", img );
	return Proswpa;
	
}
