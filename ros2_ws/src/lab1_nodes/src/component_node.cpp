#include "rclcpp/rclcpp.hpp"

namespace lab1_nodes
{

class ComponentNode : public rclcpp::Node
{
public:
  explicit ComponentNode(const rclcpp::NodeOptions & options)
  : Node("best_node_name", options)
  {
    // Порожній конструктор відповідно до Завдання 1.2 (з інформаційним логом для верифікації)
    RCLCPP_INFO(this->get_logger(), "Вузол-компонент '%s' успішно ініціалізовано в процесі-виконавці.", this->get_name());
  }
};

}  // namespace lab1_nodes

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(lab1_nodes::ComponentNode)
