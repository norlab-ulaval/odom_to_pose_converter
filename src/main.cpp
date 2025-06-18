#include <rclcpp/rclcpp.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <nav_msgs/msg/path.hpp>

class OdomToPathNode : public rclcpp::Node
{
public:
    OdomToPathNode() :
        Node("odom_to_path_node")
    {
        this->declare_parameter<int>("buffer_size", 100);
        this->get_parameter("buffer_size", bufferSize);

        odomSubscription = this->create_subscription<nav_msgs::msg::Odometry>("odom_in", 1000,
                                                                              std::bind(&OdomToPathNode::subscriptionCallback, this,
                                                                                        std::placeholders::_1));
        pathPublisher = this->create_publisher<nav_msgs::msg::Path>("path_out", 1000);
    }

private:
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odomSubscription;
    rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr pathPublisher;
    nav_msgs::msg::Path path;
    int bufferSize;

    void subscriptionCallback(const nav_msgs::msg::Odometry& odometryMsg)
    {
        geometry_msgs::msg::PoseStamped poseStamped;
        poseStamped.header.frame_id = odometryMsg.header.frame_id;
        poseStamped.header.stamp = odometryMsg.header.stamp;
        poseStamped.pose = odometryMsg.pose.pose;
        path.poses.push_back(poseStamped);
	path.header.frame_id = odometryMsg.header.frame_id;
	path.header.stamp = odometryMsg.header.stamp;
        if(path.poses.size() > bufferSize)
        {
            path.poses.erase(path.poses.begin());
        }
        pathPublisher->publish(path);
    }
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<OdomToPathNode>());
    rclcpp::shutdown();
    return 0;
}
