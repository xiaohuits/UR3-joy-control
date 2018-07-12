#include "ros/ros.h"
#include "geometry_msgs/TwistStamped.h"
#include "sensor_msgs/Joy.h"
#include "ros/console.h"
#include "ros/time.h"


class SubscriberAndPublish
{
public:
	void subscriberCB(const sensor_msgs::Joy::ConstPtr &joy)
	{
		geometry_msgs::TwistStamped vel_cmd;

		if(joy->buttons[5] == 1)
		{
			speedmult += 0.01;
			ROS_INFO("speed_mult = %f", speedmult);
		}

		if(joy->buttons[4] == 1)
		{
			speedmult -= 0.01;
			ROS_INFO("speed_mult = %f", speedmult);
		}

		vel_cmd.header.stamp = ros::Time::now();
		vel_cmd.twist.linear.x = -joy->axes[4]*speedmult;
		vel_cmd.twist.linear.y = joy->axes[5]*speedmult;
		vel_cmd.twist.linear.z = joy->axes[3]*speedmult;
		vel_cmd.twist.angular.x = joy->axes[1]*speedmult*2;
		vel_cmd.twist.angular.y = joy->axes[0]*speedmult*2;
		vel_cmd.twist.angular.z = joy->axes[2]*speedmult*2;

		twist_pub.publish(vel_cmd);

	}

private:
	ros::NodeHandle n;
	ros::Publisher twist_pub = n.advertise<geometry_msgs::TwistStamped>("cmd_vel",1);
	ros::Subscriber joy_sub = n.subscribe<sensor_msgs::Joy>("joy", 1, &SubscriberAndPublish::subscriberCB, this);
	double speedmult = 0.05;
};

int main(int argc, char** argv)
{
	ros::init(argc, argv, "joycommand_publisher");
	
	SubscriberAndPublish SAPObject;

	ros::spin();
	return 0;
}