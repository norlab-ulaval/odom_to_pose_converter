#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <chrono>
#include <fstream>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>

std::ofstream csv_file;

void subscriberCallback(const nav_msgs::Odometry& odometryMsg)
{
    std::chrono::time_point<std::chrono::steady_clock> stamp(std::chrono::nanoseconds(odometryMsg.header.stamp.toNSec()));
    double x = odometryMsg.pose.pose.position.x;
    double y = odometryMsg.pose.pose.position.y;
    double z = odometryMsg.pose.pose.position.z;
    double roll, pitch, yaw;
    geometry_msgs::Quaternion msgQuaternion = odometryMsg.pose.pose.orientation;
    tf2::Quaternion quat(msgQuaternion.x, msgQuaternion.y, msgQuaternion.z, msgQuaternion.w);
    tf2::Matrix3x3 rotationMatrix(quat);
    rotationMatrix.getRPY(roll, pitch, yaw);

    csv_file << stamp.time_since_epoch().count() << ", "
             << x << ", "
             << y << ", "
             << z << ", "
             << roll << ", "
             << pitch << ", "
             << yaw << std::endl;

}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "odom_to_pose_converter_node");
	ros::NodeHandle nodeHandle;
	ros::NodeHandle privateNodeHandle("~");
	std::string fileName = privateNodeHandle.param<std::string>("file_name", "file.csv");
	
	ros::Subscriber subscriber = nodeHandle.subscribe("odom_in", 1000, subscriberCallback);

	csv_file.open(fileName);

	csv_file << "time_stamp, x_position, y_position, z_position, roll_angle, pitch_angle, yaw_angle" << std::endl;

	ros::spin();

    csv_file.close();

	return 0;
}
