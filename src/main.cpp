#include <rclcpp/rclcpp.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>

class OdomToPoseNode : public rclcpp::Node
{
public:
    OdomToPoseNode() :
        Node("odom_to_pose_node")
    {
        odomSubscription = this->create_subscription<nav_msgs::msg::Odometry>("odom_in", 1000,
                                                                              std::bind(&OdomToPoseNode::subscriptionCallback, this,
                                                                                        std::placeholders::_1));
        posePublisher = this->create_publisher<geometry_msgs::msg::PoseStamped>("pose_out", 1000);
    }

private:
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odomSubscription;
    rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr posePublisher;

    void subscriptionCallback(const nav_msgs::msg::Odometry& odometryMsg)
    {
        geometry_msgs::msg::PoseStamped poseStamped;
        poseStamped.header.frame_id = odometryMsg.header.frame_id;
        poseStamped.header.stamp = odometryMsg.header.stamp;
        poseStamped.pose = odometryMsg.pose.pose;
        posePublisher->publish(poseStamped);
    }

};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<OdomToPoseNode>());
    rclcpp::shutdown();
    return 0;
}
