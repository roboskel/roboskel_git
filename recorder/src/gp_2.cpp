#include <signal.h>
#include <stdio.h>
#include <ros/ros.h>
#include <vector>
#include <std_msgs/Float64.h>
#include "std_msgs/Float64MultiArray.h"
#include <cmath>

float com = 2;
int i = 0;
double gp_funcs[14] = {0};

void chatterCallback(const std_msgs::Float64MultiArray::ConstPtr& msg)
{
	i = 0;
	// print all the remaining numbers
	for(std::vector<double>::const_iterator it = msg->data.begin(); it != msg->data.end(); ++it)
	{
		//printf("gp_funcs[%d] : %f\n",i, msg->data[i]);
		gp_funcs[i]=msg->data[i];
		i++;
	}
	if (gp_funcs[11]==1)
	{
		com=1;
	}
	else if (gp_funcs[5]==1)
	{
		com=0;
		ros::shutdown();
	}
}

int main(int argc, char* argv[])
{
	ros::init(argc, argv, "gp_listener");
	ros::NodeHandle nh;
	ros::Subscriber gp_in =nh.subscribe("gp_functions2", 1, chatterCallback);
	ros::Publisher float_pub = nh.advertise<std_msgs::Float64>("coms", 1);
	//ros::Publisher float_pub = nh.advertise<std_msgs::Float64>("gp_functions2", 1);

	while (ros::ok())
	{
		ros::spinOnce();
		std_msgs::Float64 msg2;
		for (int i = 0; i < FUN_NO; i++)
		{
			msg2.data=com;
		}
		float_pub.publish(msg2);
	}
	printf( "Closing...\n" );
	return 0;

}
