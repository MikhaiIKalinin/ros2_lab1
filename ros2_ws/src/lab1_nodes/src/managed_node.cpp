#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "lifecycle_msgs/msg/state.hpp"

namespace lab1_nodes
{

using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

class ManagedNode : public rclcpp_lifecycle::LifecycleNode
{
public:
  explicit ManagedNode(const rclcpp::NodeOptions & options)
  : rclcpp_lifecycle::LifecycleNode("managed_node", options)
  {
    RCLCPP_INFO(get_logger(), "[%s] Створено керований вузол (стан: Unconfigured).", get_name());
  }

  explicit ManagedNode(const std::string & node_name, const rclcpp::NodeOptions & options = rclcpp::NodeOptions())
  : rclcpp_lifecycle::LifecycleNode(node_name, options)
  {
    RCLCPP_INFO(get_logger(), "[%s] Створено керований вузол (стан: Unconfigured).", get_name());
  }

protected:
  CallbackReturn on_configure(const rclcpp_lifecycle::State & previous_state) override
  {
    RCLCPP_INFO(
      get_logger(),
      "[%s] Перехід on_configure() зі стану '%s' -> Ініціалізація ресурсів завершена (стан: Inactive).",
      get_name(), previous_state.label().c_str());
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_activate(const rclcpp_lifecycle::State & previous_state) override
  {
    RCLCPP_INFO(
      get_logger(),
      "[%s] Перехід on_activate() зі стану '%s' -> Вузол переведено у робочий режим (стан: Active).",
      get_name(), previous_state.label().c_str());
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_deactivate(const rclcpp_lifecycle::State & previous_state) override
  {
    RCLCPP_INFO(
      get_logger(),
      "[%s] Перехід on_deactivate() зі стану '%s' -> Роботу призупинено (стан: Inactive).",
      get_name(), previous_state.label().c_str());
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_cleanup(const rclcpp_lifecycle::State & previous_state) override
  {
    RCLCPP_INFO(
      get_logger(),
      "[%s] Перехід on_cleanup() зі стану '%s' -> Ресурси звільнено (стан: Unconfigured).",
      get_name(), previous_state.label().c_str());
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_shutdown(const rclcpp_lifecycle::State & previous_state) override
  {
    RCLCPP_INFO(
      get_logger(),
      "[%s] Перехід on_shutdown() зі стану '%s' -> Завершення роботи (стан: Finalized).",
      get_name(), previous_state.label().c_str());
    return CallbackReturn::SUCCESS;
  }
};

}  // namespace lab1_nodes

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(lab1_nodes::ManagedNode)
