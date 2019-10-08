#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseStamped.h>

ros::Publisher publisher;

void subscriberCallback(const nav_msgs::Odometry& odometryMsg)
{
	geometry_msgs::PoseStamped poseStamped;
	poseStamped.header = odometryMsg.header;
	poseStamped.pose = odometryMsg.pose.pose;
	publisher.publish(poseStamped);
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "odom_to_pose_converter_node");
	ros::NodeHandle nodeHandle;
	
	ros::Subscriber subscriber = nodeHandle.subscribe("odom_in", 1000, subscriberCallback);
	publisher = nodeHandle.advertise<geometry_msgs::PoseStamped>("pose", 1000);
	
	ros::spin();
	
	return 0;
}
