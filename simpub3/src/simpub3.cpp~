#include <ros/ros.h>
#include "std_msgs/String.h"
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <signal.h>
#include <stdio.h>
#include <std_msgs/Float64.h>
#include <sstream>
#include <cmath>
#include "std_msgs/Float64MultiArray.h"
#include "/home/skel/roboskel_workspace/sandbox/servo_mast/msg_gen/cpp/include/servo_mast/mast_position.h"

#define PI 3.14159265
double x=0;
double y=0;
double z=0;
double w2=0; //gwnia gia tilt
double current_tilt_angle=0;//current tilt angle
double target_tilt_angle ;
double prev_tilt_angle=0;
int init = 0;

double w1; //gwnia gia servo
double current_mast_angle;//=100;//current mast angle (apolyth se sxesh me to fasma kinhshs)
double current_mast_position=100;//current mast position
double target_angle; //new mast angle
double target_pos;   //new mast position
double prev_angle=0;
double prev_pos=100;
/*
void mast_Callback (const double& w1)
{
	target_angle=0;
	//Find current mast angle
	if (current_mast_position >=98)
	{
	   current_mast_angle = (5.625*(current_mast_position-98));
	}
	else
	{
	    current_mast_angle = (5.29411764705882352941*(current_mast_position-98));
	}
	printf("******\n");
	printf("Current Mast Position : %f\n",current_mast_position);
	printf("Current Mast Angle : %f\n",current_mast_angle);


	//Find absolute angle for mast
	target_angle= current_mast_angle+w1 ;
	if (target_angle>90)
	{
	    target_angle=90;
	    target_pos=115;
	}
	else if (target_angle<-90)
	{
		target_angle=-90;
	    target_pos=82;
	}
	//Translate angle into mast position
	else if (target_angle>0)
	{
	    target_pos=98+target_angle*0.222222222;
	}
	else target_pos=98+target_angle*1.8888888889;

	if(abs(current_mast_position-prev_pos)<2)
	{
		target_pos=prev_pos;
		target_angle=prev_angle;
		prev_pos=current_mast_position;
		prev_angle=current_mast_angle;
	}
	else
	{
		if ((abs(target_pos-current_mast_position)>2)&&(target_pos<115)&&(target_pos>80))
		{
			if (target_pos>current_mast_position)
			{
				target_pos=current_mast_position+2;
			}
			else
			{
				target_pos=current_mast_position-2;
			}
			if (target_pos >=98)
			{
				target_angle = (5.625*(target_pos-98));
			}
				else
			{
				target_angle = (5.29411764705882352941*(target_pos-98));
			}

		}
		else if (target_pos>115)
		{
			target_pos = 115;
			target_angle = 90;
		}
		else if (target_pos<80)
		{
			target_pos = 80;
			target_angle = -90;
		}
	}
	current_mast_position = target_pos;
	current_mast_angle = target_angle; 
	printf("**********\n");
	printf("%f\n",target_pos);
	printf("%f\n",target_angle);
	printf("**********\n");
	
}
*/
void chatterCallback(const geometry_msgs::Vector3::ConstPtr& msg)
{
	x=msg->x;
	y=msg->y;
	z=msg->z;
 	w1=(atan2(y,x)* 180 / PI);
    w2=(atan2(z,x)* 180 / PI);
    printf("\n*****\nComputed w2 = %f\n",w2);
    printf("Computed w1 = %f\n*****\n",w1);
}



void tiltCallback(const std_msgs::Float64::ConstPtr& msg)
{
	
 	current_tilt_angle=msg->data;
	printf("******\n");
	printf("Current Tilt : %f\n",current_tilt_angle);
	printf("******\n");
	if ((abs(current_tilt_angle-w2)<4)&&(abs(current_tilt_angle-w2)>1.5))
	{  //X moires threshold gia na ginei to tilt
		if((current_tilt_angle-w2)<(-30))
		{
			target_tilt_angle=-29;
		}
		else if ((current_tilt_angle+w2)>30)
		{
			target_tilt_angle=29;
		}
		else 
		{
			target_tilt_angle=w2; 
		}
	}
	else if (abs(current_tilt_angle-w2)>4)
	{	
		if((current_tilt_angle-w2)<(-30))
		{
			target_tilt_angle=-29;
		}
		else if ((current_tilt_angle+w2)>30)
		{
			target_tilt_angle=29;
		}
		else if (w2>current_tilt_angle)
		{
			target_tilt_angle=current_tilt_angle+4;
		}
		else if (w2<current_tilt_angle)
		{
			target_tilt_angle=current_tilt_angle-4;
		}
	}
	else
	{
		target_tilt_angle=current_tilt_angle; //diathrhsh arxikhs gwnias
	}
}

	





int main(int argc, char* argv[])
{
	ros::init(argc, argv, "head_cords_listener");

	ros::NodeHandle nh;

	ros::Rate loop_rate(1000);
	ros::Subscriber cords_sub = nh.subscribe("head_cords", 1, chatterCallback);
	ros::Subscriber curr_tilt = nh.subscribe("cur_tilt_angle",1,tiltCallback);

	ros::Publisher servo_pos = nh.advertise<std_msgs::Float64>("mast_float", 1);
	printf( "Advertising Mast\n");
	ros::Publisher cmr_pos   = nh.advertise<std_msgs::Float64>("tilt_angle",1);
	printf( "Advertising Tilt Angle\n");

	 while (ros::ok())
	{
		ros::Duration(1).sleep();
		ros::spinOnce();
		printf("*****\n");
		printf("SPINNED\n");
		printf("*****\n");
			
		if (abs(target_tilt_angle)<=30)
		{	
			printf("**********\n");
			printf("X : %f\n",x);
			printf("Y : %f\n",y);
			printf("Z : %f\n",z);
			printf("gwnia = %f\n",w2 );
			printf("Tilt Angle : %f \n", target_tilt_angle);
			printf("**********\n");
			std_msgs::Float64 tilt_angle;
			tilt_angle.data=target_tilt_angle;
			cmr_pos.publish(tilt_angle);
			ros::Duration(0.5).sleep();
		}
		else
		{
			printf("Skipping Loop\n");
		}
	
		target_angle=0;
		//Find current mast angle
		if (current_mast_position >=98)
		{
		   current_mast_angle = (5.625*(current_mast_position-98));
		}
		else
		{
			current_mast_angle = (5.29411764705882352941*(current_mast_position-98));
		}
		printf("******\n");
		printf("Current Mast Position : %f\n",current_mast_position);
		printf("Current Mast Angle : %f\n",current_mast_angle);


		//Find absolute angle for mast
		target_angle= current_mast_angle+w1 ;
		if (target_angle>90)
		{
			target_angle=90;
			target_pos=115;
		}
		else if (target_angle<-90)
		{
			target_angle=-90;
			target_pos=82;
		}
		//Translate angle into mast position
		else if (target_angle>0)
		{
			target_pos=98+target_angle*0.222222222;
		}
		else target_pos=98+target_angle*1.8888888889;


			if ((abs(target_pos-current_mast_position)>2)&&(target_pos<115)&&(target_pos>80))
			{
				if (target_pos>current_mast_position)
				{
					target_pos=current_mast_position+2;
				}
				else
				{
					target_pos=current_mast_position-2;
				}
				if (target_pos >=98)
				{
					target_angle = (5.625*(target_pos-98));
				}
					else
				{
					target_angle = (5.29411764705882352941*(target_pos-98));
				}

			}
			else if (target_pos>115)
			{
				target_pos = 115;
				target_angle = 90;
			}
			else if (target_pos<80)
			{
				target_pos = 80;
				target_angle = -90;
			}
		
		
		printf("Target Mast Angle : %f\n",target_angle);
		printf("Target Mast Position : %f\n",target_pos);
		printf("******\n");

		std_msgs::Float64 serv_pos_msg;
		serv_pos_msg.data = target_pos;
		servo_pos.publish(serv_pos_msg);
		ros::Duration(0.5).sleep();

	}

	printf( "Closing...\n" );
	return 0;

}

