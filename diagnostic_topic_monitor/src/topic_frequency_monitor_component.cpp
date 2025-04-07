#include "diagnostic_topic_monitor/topic_frquency_monitor.hpp"

namespace diagnostic_topic_monitor
{

TopicFrequencyMonitor::TopicFrequencyMonitor(
  const std::string & node_name, rclcpp::NodeOptions options)
: GenericTopicMonitor(node_name, options)
{
  RCLCPP_INFO_STREAM(get_logger(), "TopicFrequencyMonitor constructor called");
}

TopicFrequencyMonitor::TopicFrequencyMonitor(rclcpp::NodeOptions options)
: GenericTopicMonitor("topic_frequency_monitor", options)
{
  RCLCPP_INFO_STREAM(get_logger(), "TopicFrequencyMonitor constructor called");
}

void TopicFrequencyMonitor::topic_cb(
  const std::string & topic_name, const std::shared_ptr<rclcpp::SerializedMessage> & msg)
{
  const auto diag = topic_diag_map_.find(topic_name);
  if (diag != topic_diag_map_.end()) {
    diag->second->tick();
  } else {
    const auto fallback_diag = fallback_topic_diag_map_.find(topic_name);
    if (fallback_diag != fallback_topic_diag_map_.end()) {
      fallback_diag->second->tick();
    } else {
      RCLCPP_WARN(get_logger(), "No diagnostic found for topic %s", topic_name.c_str());
    }
  }
}

}  // namespace diagnostic_topic_monitor

#include <rclcpp_components/register_node_macro.hpp>  // NOLINT: upstream
RCLCPP_COMPONENTS_REGISTER_NODE(diagnostic_topic_monitor::TopicFrequencyMonitor)
