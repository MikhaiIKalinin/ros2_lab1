#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "lifecycle_msgs/msg/transition.hpp"
#include "lifecycle_msgs/msg/state.hpp"

namespace lab1_nodes
{
using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

class SimpleManagedNode : public rclcpp_lifecycle::LifecycleNode
{
public:
  explicit SimpleManagedNode(const std::string & node_name)
  : rclcpp_lifecycle::LifecycleNode(node_name)
  {
    RCLCPP_INFO(get_logger(), "Ініціалізовано конструктор LifecycleNode '%s' (стан: Unconfigured)", get_name());
  }

  CallbackReturn on_configure(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "[%s] Виконано перехід TRANSITION_CONFIGURE -> стан Inactive", get_name());
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_activate(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "[%s] Виконано перехід TRANSITION_ACTIVATE -> стан Active", get_name());
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_deactivate(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "[%s] Виконано перехід TRANSITION_DEACTIVATE -> стан Inactive", get_name());
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_cleanup(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "[%s] Виконано перехід TRANSITION_CLEANUP -> стан Unconfigured", get_name());
    return CallbackReturn::SUCCESS;
  }
};
}  // namespace lab1_nodes

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  // Створення двох керованих вузлів (Managed Nodes)
  auto node1 = std::make_shared<lab1_nodes::SimpleManagedNode>("managed_node_1");
  auto node2 = std::make_shared<lab1_nodes::SimpleManagedNode>("managed_node_2");

  // Додавання обох вузлів до єдиного процесу-виконавця (Executor)
  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(node1->get_node_base_interface());
  executor.add_node(node2->get_node_base_interface());

  // Послідовне конфігурування та активація командами Transition
  node1->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE);
  node1->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);

  node2->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE);
  node2->trigger_transition(lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE);

  RCLCPP_INFO(
    rclcpp::get_logger("supervisor_main"),
    "Обидва вузли ('%s' [стан: %s] та '%s' [стан: %s]) успішно переведено в активний стан.",
    node1->get_name(), node1->get_current_state().label().c_str(),
    node2->get_name(), node2->get_current_state().label().c_str());

  executor.spin_some();
  rclcpp::shutdown();
  return 0;
}
