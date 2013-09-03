#include <fstream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <iterator>
#include <vector>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float64.h>

#include <std_msgs/Header.h>
#include <math.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <cmath>
#include <chrono>
#include <ctime>
#include "rn_face/head_cords_m.h"
#include "rn_face/head_cords_m_array.h"
using namespace std;
long int file_min = 1375196305411;
	long int file_max = 1375196340719;
	int result;
	

int main( int argc, char** argv )
{
	
	ros::init(argc, argv, "kati");
	ros::NodeHandle node;
	ros::Publisher gp_out,addr_out;
	gp_out = node.advertise<std_msgs::Float64>("coms",1);
	addr_out = node.advertise<std_msgs::String>("img_addr",10);
	int c;
	char str[80];
	while(ros::ok()){
		sprintf (str, "Images/image_%ld.jpg",file_min);
		//FILE *file = fopen("sticks.data","r");
		result = access (str, F_OK); // F_OK tests existence also (R_OK,W_OK,X_OK).
		//            for readable, writeable, executable
		file_min++;
		if ( result == 0 )
		{
			printf("\n\nBRIKA: %s exists!!\n\n\n",str);
			std_msgs::String msg;
			msg.data=str;
			//ROS_INFO("%c",str);
			std_msgs::Float64 msg2;
			msg2.data=1;
			gp_out.publish(msg2);
			addr_out.publish(msg);
			ros::Duration(0.1).sleep();
			ros::spinOnce();
		}
	}
	return 0;
}
