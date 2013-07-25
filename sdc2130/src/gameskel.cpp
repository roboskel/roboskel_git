#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <time.h>
#include <math.h>

#include "sdc2130_skel/RoboteqDevice.h"
#include "sdc2130_skel/ErrorCodes.h"
#include "sdc2130_skel/Constants.h"
#include "std_msgs/Float64MultiArray.h"

#include "ros/ros.h"
#include "sdc2130_skel/teleop_skel.h"


using namespace std;

	#define PI 3.14159265358979323846264338
	#define DIAMETER 15.24
	#define RESOLUTION 2000
	#define WHEEL_BASE_WIDTH 40
	#define TWOPI 360
	#define RADS 57.2958
				
	double functions[14]={0};
	double seconds;
	int LM = 0 ;
	int RM = 0 ;
	int HB = 0 ; //handbrake signal
	int ESD = 0 ; //emergency shutdown signal
	int MODE = 0 ;
	int REC = 0 ;
	string response = "";
	RoboteqDevice device;
	int status = -1;
	int i ;
	clock_t t;
	
	int count_ = 0;
	double posx = 0.0;
	double posy = 0.0;
	double posth = 0.0;
	double mr = 0;
	double ml = 0;
	double mth = 0;
	double deg = 0 ;
	int dist = 0;
	int total_dist = 0;
	int battery = 0;
	ros::Time current_time, last_time;
	ros::Time write_time, cur_write_time, total_time, cur2;
	ofstream file;


void teleopCallback(const std_msgs::Float64MultiArray::ConstPtr& msg)
{	
	
	int lenc,renc;
	current_time = ros::Time::now();
	i=0;
	for(std::vector<double>::const_iterator it = msg->data.begin(); it != msg->data.end(); ++it)
	{
		
		functions[i]=msg->data[i];
		i++;
	}
	if(device.IsConnected())
	{
		if(MODE==0)
		{
			////////////////////////////
			//PRESS START FOR SHUTDOWN 
			/////////////////////////////
			if(functions[11]==1)
			{
				printf("Initializing Emergency Shutdown Protocol pewpepwewpew\n");			
				//device.SetCommand(_GO, 2,  0);
				if((status = device.SetCommand(_GO, 2,  0) != RQ_SUCCESS))
					{
						cout<<"failed --> \n"<<status<<endl;
						//device.Disconnect();
						//ros::shutdown();
						
					}	
				else 
					{
						//cout<<"succeeded.\n"<<endl;
						//rotL=device.GetValue(_S,2,rotL);
					}
				
				ros::Duration(0.01).sleep(); 
				//device.SetCommand(_GO, 1,- 0);
				if((status = device.SetCommand(_GO, 1,- 0)) != RQ_SUCCESS)
					{
						cout<<"failed --> \n"<<status<<endl;
						device.Disconnect();
						ros::shutdown();
					}	
				else
					{
						//cout<<"succeeded.\n"<<endl;
						//rotR=device.GetValue(_S,1,rotR);
					}
					
				printf("Dave, stop. Stop, will you? Stop, Dave. Will you stop Dave? Stop, Dave.\n\n");
				file<<"***********\n";
				file<<"seconds : "<<seconds<<"\n";
				file<<"mr : "<<mr<<"\n";
				file<<"ml : "<<ml<<"\n";
				file<<"mth : "<<mth<<"\n";
				file<<"deg : "<<deg<<"\n";
				file<<"X : "<<posx<<"\n";
				file<<"Y : "<<posy<<"\n";
				file<<"Total Distance : "<<total_dist<<"\n";
				file<<"Battery Voltage : "<<battery<<"\n";
				file<<"************\n\n";
				cur_write_time=ros::Time::now();
				file<<"Seconds Elapsed "<<((cur_write_time-total_time).toSec())<<endl;
				file.close();		
				ros::Duration(3).sleep();
				//std::cout << '\7';
				//std::cout << '\7';
				//std::cout << '\7';
				ESD = 1 ;
				ros::shutdown();
				return ;
			}
			///////////////////////////////////////////
			//PRESS X TO RECORD STATUS TO LOG
			///////////////////////////////////////////
			else if (functions[4]==1)
			{	
				REC=1;
				count_++;
				file<<"\n\nRECORDING AFTER REQUEST "<<count_<<"\n";
				file<<"***********\n";
				file<<"seconds : "<<seconds<<"\n";
				file<<"mr : "<<mr<<"\n";
				file<<"ml : "<<ml<<"\n";
				file<<"mth : "<<mth<<"\n";
				file<<"deg : "<<deg<<"\n";
				file<<"X : "<<posx<<"\n";
				file<<"Y : "<<posy<<"\n";
				file<<"Total Distance : "<<total_dist<<"\n";
				file<<"Battery Voltage : "<<battery<<"\n";
				cur2=ros::Time::now();
				file<<"Seconds Elapsed "<<((cur2-total_time).toSec())<<endl;
				file<<"************\n\n";
				//std::cout << '\7';
				ros::Duration(1).sleep();	
			}
			////////////////////////////////////////////
			//PRESS O BUTTON FOR 5 SECOND IMMOBILIZATION
			///////////////////////////////////////////
			else if (functions[3]==1)
			{	
				
				//ROS_INFO("Temporary Immobilization Protocol Engaged. Stand By.\n");
				printf("Temporary Immobilization Protocol Engaged. Stand By.\n\n");
				//device.SetCommand(_GO, 2,  0);
				if((status = device.SetCommand(_GO, 2,  0) != RQ_SUCCESS))
				{
					cout<<"failed --> \n"<<status<<endl;
					device.Disconnect();
					ros::shutdown();
					
				}	
				else 
				{
					//cout<<"succeeded.\n"<<endl;
				}
				
				ros::Duration(0.01).sleep(); 
				//device.SetCommand(_GO, 1,- 0);
				if((status = device.SetCommand(_GO, 1,- 0)) != RQ_SUCCESS)
				{
					cout<<"failed --> \n"<<status<<endl;
					device.Disconnect();
					ros::shutdown();
				}	
				else
				{
					//cout<<"succeeded.\n"<<endl;
				}
				
				ros::Duration(5).sleep();
				printf("Motion Resumed\n\n");
			}
			////////////////////////////////////////////////////
			//PRESS TRIANGLE FOR HANDBRAKE PRESS AGAIN TO RESUME
			////////////////////////////////////////////////////
			else if (functions[2]==1)
			{
				if(HB==0)
				{
					HB=1;
					ROS_INFO("Handbrake Engaged. Press Again to Resume Normal Function\n");
					printf("Handbrake Engaged. Press Again to Resume Normal Function\n");
					
					if((status = device.SetCommand(_GO, 2,  0) != RQ_SUCCESS))
					{
						cout<<"failed --> \n"<<status<<endl;
						device.Disconnect();
						ros::shutdown();
						
					}	
					else 
					{
						//cout<<"succeeded.\n"<<endl;
					}
					ros::Duration(0.01).sleep(); 
					if((status = device.SetCommand(_GO, 1,- 0)) != RQ_SUCCESS)
					{
						cout<<"failed --> \n"<<status<<endl;
						device.Disconnect();
						ros::shutdown();
					}	
					else
					{
						//cout<<"succeeded.\n"<<endl;
					}

				}
				else
				{
					HB=0;
					printf("Handbrake Disengaged. Motion Resumed. \n\n");
				}
				ros::Duration(1).sleep();
			}
		////////////////////////////////////////
		//TRY AND REINITIALIZE AFTER RF SHUTDOWN
		////////////////////////////////////////
		else if (functions[5]==1) 
		{
			while (!device.IsConnected())
			{	
				printf("*********************\n");
				printf("Attemptin to Reconnect SDC2130\n");
				printf("*********************\n");
				device.Connect("/dev/ttyACM1");
			}
		}
		//////////////
		//MOVE MOTORS
		/////////////
		else
		{
		if(HB==0)
		{	
			
			LM=functions[0]*800;
			RM=functions[1]*800;
			if (LM>0)
				LM=(int)(LM+0.5);
			else
				LM=(int)(LM-0.5);
			if (RM>0)
				RM=(int)(RM+0.5);
			else
				RM=(int)(RM-0.5);

			if((status = device.SetCommand(_GO, 2,  LM) != RQ_SUCCESS))
			{
				//cout<<"failed --> \n"<<status<<endl;
				//device.Disconnect();
				//ros::shutdown();
				
			}	
			else 
			{
				//if(device.GetValue(_C,2,rotL)!=RQ_SUCCESS)
				//ROS_INFO("Encoder data decoding failed");  
			}
			//ros::Duration(0.01).sleep(); 
			//printf("Second IF\n");
			if((status = device.SetCommand(_GO, 1,- RM)) != RQ_SUCCESS)
			{
				//cout<<"failed --> \n"<<status<<endl;
				//device.Disconnect();
				//ros::shutdown();
			}	
			else
			{
				//cout<<"succeeded.\n"<<endl;
				//ROS_INFO("Encoder data decoding failed");  
			}
			ros::Duration(0.05).sleep();

		}
	}
		if((device.GetValue(_S, 1, lenc)!=RQ_SUCCESS)||(device.GetValue(_S, 2, renc)!=RQ_SUCCESS))
		{
			//ROS_INFO("Encoder data decoding failed\n");
		}  
		//if(device.GetValue(_S, 2, renc)!=RQ_SUCCESS)
		//	{
		//		ROS_INFO("Encoder data decoding failed\n");
		//	}
		else
		{	
			if (REC==1)
			{
				ROS_INFO("left_%d Right_%d\n",lenc,-renc);
				//ros::Duration(0.5).sleep();
				seconds= (current_time - last_time).toSec();
				mr = seconds*(-renc/60.0)*(DIAMETER*PI);
				ml = seconds*(lenc/60.0)*(DIAMETER*PI);
				dist=(ml+mr)/2.0;
				total_dist +=abs(dist);
				mth += (ml-mr)/WHEEL_BASE_WIDTH;
				deg -=(float)((int)(mth/TWOPI))*TWOPI;
				posx += (dist*(cos(mth*RADS)));
				posy += (dist*(sin(mth*RADS)));
			}
		}
		if(device.GetValue(_V,battery)!=RQ_SUCCESS)
		{
			ROS_INFO("Failed to read battery voltage\n");
		}  
		/*
		printf("***********\n");
		printf("seconds : %f\n",seconds);;
		printf("mr : %f\n",mr);
		printf("ml : %f\n",ml);
		printf("mth : %f\n",mth);
		printf("X : %f\n",posx);
		printf("Y : %f\n",posy);
		printf("Total Distance : %d\n",total_dist);
		printf("Battery Voltage : %d\n",battery);
		printf("************\n\n");
		//ros::Duration(0.5).sleep();
		*/
		/*
		cur_write_time = ros::Time::now();
		if ((cur_write_time.toSec()-write_time.toSec())>0.6)
		{
			file<<"***********\n";
			file<<"seconds : "<<seconds<<"\n";
			file<<"mr : "<<mr<<"\n";
			file<<"ml : "<<ml<<"\n";
			file<<"mth : "<<mth<<"\n";
			file<<"X : "<<posx<<"\n";
			file<<"Y : "<<posy<<"\n";
			file<<"Total Distance : "<<total_dist<<"\n";
			file<<"Battery Voltage : "<<battery<<"\n";
			cur2=ros::Time::now();
			file<<"Seconds Elapsed "<<((cur2-total_time).toSec())<<endl;
			file<<"************\n\n";
			write_time=ros::Time::now();		
		}
		*/
		mr=0;
		ml=0;
		dist=0;
		last_time = current_time;
		}
	} 
	
	else if (functions[5]==1)
	{
		while (!device.IsConnected())
		{
			printf("***********************\n");
			printf("Attempting to reconnect\n");
			printf("***********************\n\n");
			device.Connect("/dev/ttyACM1");
			ros::Duration(3).sleep();
		}
		printf("********************\n");
		printf("Reconnect Successful\n");
		printf("********************\n\n");
		ros::Duration(3).sleep();
	}
}

int main(int argc, char *argv[])
{	
	int status = device.Connect("/dev/ttyACM1");
	if(status != RQ_SUCCESS)
	{
		cout<<"Error connecting to device: "<<status<<"."<<endl;
		return 1;
	}
	//device.SetConfig(_RWD, 1, 1000);
	//device.SetConfig(_RWD, 2, 1000);
	device.SetConfig(_RWD, 1, -1);
	device.SetConfig(_RWD, 2, -1);
	
	ros::init(argc, argv, "sdc2130_skel");

	ros::NodeHandle n;
	ros::Duration(0.1).sleep();
	ros::Subscriber sub = n.subscribe("gp_functions2", 1, teleopCallback);
	
	ROS_INFO("- SetConfig(_DINA, 1, 1)...");
    if((status = device.SetConfig(_DINA, 1, 1)) != RQ_SUCCESS)
      cout<<"failed --> "<<status<<endl;
    else
      ROS_INFO("succeeded.");
    ros::Duration(0.01).sleep(); //sleep for 10 ms

    int result;
    
    ROS_INFO("- GetConfig(_DINA, 1)...");
    if((status = device.GetConfig(_DINA, 1, result)) != RQ_SUCCESS)
      cout<<"failed --> "<<status<<endl;
    else
      cout<<"returned --> "<<result<<endl;
    ROS_INFO("Roboteq -- Successfull setup of the Roboteq SDC2130");
    printf ("Sek Operational\n\n");
    ros::Duration(0.01).sleep(); //sleep for 10 ms
    
	file.open ("roboskel_log.txt", ios::out | ios::binary);
	write_time = ros::Time::now();
	total_time =ros::Time::now();
	file<<write_time<<"\n";
	ros::Duration(3).sleep();
while (ros::ok())
    {	
		ros::spin();
	}
	
	device.Disconnect();
	return 0;
}

