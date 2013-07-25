// g++ `pkg-config --cflags opencv`   `pkg-config --libs opencv` face_detection_and_tracking.cpp -o face_detection_and_tracking

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

using namespace std;
using namespace cv;



struct face {
	int id;
	int x;
	int y;
	int r;
};


int detected = 0;

IplImage *image = 0, *hsv = 0, *hue = 0, *mask = 0, *backproject = 0, *histimg = 0;
CvHistogram *hist = 0;

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
int hdims = 16;
float hranges_arr[] = {0,180};
float* hranges = hranges_arr;
int vmin = 60, vmax = 256, smin = 50;
//-------------------

face* detectAndDraw( Mat& img,
					CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
					double scale);

String cascadeName = "haarcascade_frontalface_alt.xml";
String nestedCascadeName = "haarcascade_eye_tree_eyeglasses.xml";



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
    
	CvCapture* capture = 0;
    Mat frame, frameCopy;
	face *Faces;
	int i, bin_w;
	char string_faces[20];
	CvFont font;
	cvInitFont(&font,CV_FONT_HERSHEY_PLAIN|CV_FONT_ITALIC,1,1,1,1);


	
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
    
    if( inputName.empty() || (isdigit(inputName.c_str()[0]) && inputName.c_str()[1] == '\0') )
    {
        capture = cvCaptureFromCAM( inputName.empty() ? 0 : inputName.c_str()[0] - '0' );
        int c = inputName.empty() ? 0 : inputName.c_str()[0] - '0' ;
        if(!capture) cout << "Capture from CAM " <<  c << " didn't work" << endl;
    }
	
    cvNamedWindow( "Face Detection and Tracking", 1 );
	cvNamedWindow( "Histogram", 1 );
	
	
    if( capture )
    {
        cout << "In capture ..." << endl;
        for(;;)
        {
            IplImage* iplImg = cvQueryFrame( capture );
            frame = iplImg;
            if( frame.empty() )
                break;
            if( iplImg->origin == IPL_ORIGIN_TL )
                frame.copyTo( frameCopy );
            else
                flip( frame, frameCopy, 0 );
			
			
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
			}
			
			
			
			cvCopy( iplImg, image, 0 );
			cvCvtColor( image, hsv, CV_BGR2HSV );
			
			
			if (detected == 0) {
				Faces = detectAndDraw( frameCopy, cascade, nestedCascade, scale );
				printf("face_id=%d\tx=%d\ty=%d\tr=%d\n", Faces[0].id, Faces[0].x, Faces[0].y, Faces[0].r);
				
				selection = cvRect(Faces[0].x-Faces[0].r/10,Faces[0].y-Faces[0].r/10, Faces[0].x+Faces[0].r/5,Faces[0].y+Faces[0].r/5);

				printf("%d %d %d %d",selection.x,selection.y,selection.width,selection.height);
				track_object = -1;
				//-----------Mean-Shift-----------------------------
				printf("\n\nFace detected\n\n");
			}
			
			if (detected == 1) {
				printf("\n\nMean-Shift\n\n");
				
				
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
					
					for( i = 0; i < hdims; i++ ){
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
				
				cvEllipseBox( image, track_box, CV_RGB(255,0,0), 3, CV_AA, 0 );
				//cvCircle(image, track_box, 100 , CV_RGB(255,255,0), 1, CV_AA, 0 );
				snprintf(string_faces, sizeof(string_faces), "face %d", Faces[0].id );
				cvPutText(image,string_faces,cvPoint(track_window.x,track_window.y),&font, CV_RGB(0,128,255));
				//------------------------------------------------
				
				
				cvShowImage( "Face Detection and Tracking", image );
				cvShowImage( "Histogram", histimg );
				cvWaitKey(500); // o xronos gia kathe frame
				
				
				
			}
			
			
			
			
            
            if( waitKey( 10 ) >= 0 )
                goto _cleanup_;
        }
        waitKey(0);
	_cleanup_:
        cvReleaseCapture( &capture );
    }
    cvDestroyWindow("Face Detection and Tracking");
    return 0;
}





















face* detectAndDraw( Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, double scale){
	
	
	face Proswpa[2];
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
		
		
		
		Proswpa[0].id = 1;
		Proswpa[0].x = center.x;
		Proswpa[0].y = center.y;
		Proswpa[0].r = radius;
		printf("Entopistike ena proswpo.\n");
		detected = 1;
		printf("Faces = %d\n",i);
		cv::imshow( "Face Detection and Tracking", img );
		cvWaitKey(500);
		return Proswpa;
		
		
		
		/*
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
			
        }
		*/
    }
	printf("Faces = %d\n",i);
    cv::imshow( "Face Detection and Tracking", img );
	return Proswpa;
	
}
