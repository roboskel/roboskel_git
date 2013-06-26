#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

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
int ESD = 0; //emergency shutdown signal
double gp_funcs[14] = {0};
string response = "";
RoboteqDevice device;
int status = -1;
int i ;

void teleopCallback(const std_msgs::Float64MultiArray::ConstPtr& msg)
{	
	i=0;
	for(std::vector<double>::const_iterator it = msg->data.begin(); it != msg->data.end(); ++it)
	{
		
		functions[i]=msg->data[i];
		i++;
	}
	
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
		printf("Dave, stop. Stop, will you? Stop, Dave. Will you stop Dave? Stop, Dave.\n");
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
		printf("Temporary Immobilization Protocol Engaged. Stand By.");
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
	}
	////////////////////////////////////////////////////
	//PRESS TRIANGLE FOR HANDBRAKE PRESS AGAIN TO RESUME
	////////////////////////////////////////////////////
	else if (functions[2]==1)
	{
		if(HB==0)
		{
			HB=1;
			printf("Handbrake Engaged. Press Again to Resume Normal Function");
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
		}
		ros::Duration(2).sleep();
	}
	//////////////
	//MOVE MOTORS
	/////////////
	else
	{
		if(HB==0)
		{
			if (functions[0] >= 0)
				LM = (int) (functions[0] + 0.5); 
			else 
				LM = (int) (functions[0] - 0.5);
			if (functions[1] >= 0)
				RM = (int) (functions[1] + 0.5); 
			else 
				RM = (int) (functions[1] - 0.5);
		
			LM=LM*200;
			RM=RM*200; 
			   
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
			ros::Duration(0.01).sleep(); 
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
			ros::Duration(0.01).sleep();
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
	device.SetConfig(_RWD, 1, 500);
	device.SetConfig(_RWD, 2, 500);
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
    printf ("Sek Operational");
    ros::Duration(0.01).sleep(); //sleep for 10 ms

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

