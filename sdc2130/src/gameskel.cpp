#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
/*
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <AL/al.h>
#include <AL/alut.h>
*/
#include "sdc2130_skel/RoboteqDevice.h"
#include "sdc2130_skel/ErrorCodes.h"
#include "sdc2130_skel/Constants.h"
#include "std_msgs/Float64MultiArray.h"

#include "ros/ros.h"
#include "sdc2130_skel/teleop_skel.h"

using namespace std;
float functions[14]={0};
int LM = 0 ;
int RM = 0 ;
int HB = 0 ; //handbrake signal
int ESD = 0 ; //emergency shutdown signal
int MODE = 0 ;
double gp_funcs[14] = {0};
string response = "";
RoboteqDevice device;
int status = -1;
int i ;

//ALuint sndBuffer, sndSource;

void teleopCallback(const std_msgs::Float64MultiArray::ConstPtr& msg)
{	
	i=0;
	for(std::vector<double>::const_iterator it = msg->data.begin(); it != msg->data.end(); ++it)
	{
		
		functions[i]=msg->data[i];
		i++;
	}
	//////////////////////////////////////////////////////
	//PRESS SELECT TO ALTERNATE BETWEEN NAVIGATION METHODS
	//////////////////////////////////////////////////////
	if(functions[10]==1)
	{
		if (MODE==0)
		{
			MODE=1;
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
			//printf("Second IF\n");
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
			MODE=0;
		}
	}
	if(MODE==0)
	{
		////////////////////////////
		//PRESS START FOR SHUTDOWN 
		/////////////////////////////
		if(functions[11]==1)
		{
			printf("Initializing Emergency Shutdown Protocol pewpepwewpew\n");
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
				//printf("Second IF\n");
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
			printf("Dave, stop. Stop, will you? Stop, Dave. Will you stop Dave? Stop, Dave.\n\n");
			ros::Duration(3).sleep();
			ESD = 1 ;
			ros::shutdown();
			return ;
		}
		////////////////////////////////////////////
		//PRESS O BUTTON FOR 5 SECOND IMMOBILIZATION
		///////////////////////////////////////////
		else if (functions[3]==1)
		{	
				/*
				alSourcei (sndSource, AL_BUFFER, sndBuffer);
				alSourcePlay (sndSource);
				alutSleep (1);
				alutExit ();
				*/
			
			//ROS_INFO("Temporary Immobilization Protocol Engaged. Stand By.\n");
			printf("Temporary Immobilization Protocol Engaged. Stand By.\n\n");
			//printf("first IF\n");
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
			//printf("Second IF\n");
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
				//printf("first IF\n");
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
				//printf("Second IF\n");
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
		//////////////
		//MOVE MOTORS
		/////////////
		else
		{
		if(HB==0)
		{	
			
			LM=functions[0]*400;
			RM=functions[1]*400;
			if (LM>0)
				LM=(int)(LM+0.5);
			else
				LM=(int)(LM-0.5);
			if (RM>0)
				RM=(int)(RM+0.5);
			else
				RM=(int)(RM-0.5);
			/*
			if (functions[0] >= 0)
				LM = (int) (functions[0] + 0.5); 
			else 
				LM = (int) (functions[0] - 0.5);
			if (functions[1] >= 0)
				RM = (int) (functions[1] + 0.5); 
			else 
				RM = (int) (functions[1] - 0.5);
			*/
			 
			   
			//printf("first IF\n");
			if((status = device.SetCommand(_GO, 2,  LM) != RQ_SUCCESS))
			{
				//cout<<"failed --> \n"<<status<<endl;
				device.Disconnect();
				ros::shutdown();
				
			}	
			else 
			{
				//cout<<"succeeded.\n"<<endl;
			}
			//ros::Duration(0.01).sleep(); 
			//printf("Second IF\n");
			if((status = device.SetCommand(_GO, 1,- RM)) != RQ_SUCCESS)
			{
				//cout<<"failed --> \n"<<status<<endl;
				device.Disconnect();
				ros::shutdown();
			}	
			else
			{
				//cout<<"succeeded.\n"<<endl;
			}
			//ros::Duration(0.01).sleep();
		}
	}
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
	device.SetConfig(_RWD, 1, 0);
	device.SetConfig(_RWD, 2, 0);
	ros::init(argc, argv, "sdc2130_skel");

	ros::NodeHandle n;
	
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
    
    
			/*
			alutInit (&argc, argv);
			sndBuffer = alutCreateBufferFromFile("/home/skel/r2ack.wav");
			alGenSources (1, &sndSource);
			*/
    
    
while (ros::ok())
    {	
		if (ESD==1)
		{
			device.Disconnect();
			return 0;
		}
		else
		{
			ros::spin();
		}
		
	}
	device.Disconnect();
	return 0;
}

