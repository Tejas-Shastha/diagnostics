#include "diagnostic_topic_monitor/topic_age_monitor.hpp"

namespace diagnostic_topic_monitor
{

TopicAgeMonitor::TopicAgeMonitor(
  const std::string & node_name, rclcpp::NodeOptions options)
: GenericTopicMonitor(node_name, options)
{
  RCLCPP_INFO_STREAM(get_logger(), "TopicAgeMonitor constructor called");
}

TopicAgeMonitor::TopicAgeMonitor(rclcpp::NodeOptions options)
: GenericTopicMonitor("topic_frequency_monitor", options)
{
  RCLCPP_INFO_STREAM(get_logger(), "TopicAgeMonitor constructor called");
}

void TopicAgeMonitor::topic_cb(
  const std::string & topic_name, const std::shared_ptr<rclcpp::SerializedMessage> & msg)
{
  std_msgs::msg::Header header;
  const auto diag = topic_diag_map_.find(topic_name);
  _header_serializer.deserialize_message(msg.get(), &header);

  if (diag != topic_diag_map_.end()) {
    diag->second->tick(header.stamp);
  } else {
    RCLCPP_WARN(get_logger(), "No diagnostic found for topic %s", topic_name.c_str());
  }
}

}  // namespace diagnostic_topic_monitor

#include <rclcpp_components/register_node_macro.hpp>  // NOLINT: upstream
RCLCPP_COMPONENTS_REGISTER_NODE(diagnostic_topic_monitor::TopicAgeMonitor)
