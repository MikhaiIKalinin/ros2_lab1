#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "lifecycle_msgs/msg/transition.hpp"
#include "lifecycle_msgs/srv/change_state.hpp"
#include "std_srvs/srv/trigger.hpp"

using namespace std::chrono_literals;

namespace lab1_nodes
{

class LifecycleSupervisor : public rclcpp::Node
{
public:
  explicit LifecycleSupervisor(const rclcpp::NodeOptions & options)
  : Node("lifecycle_supervisor", options)
  {
    // Оголошення списку імен керованих вузлів (за замовчуванням node1 та node2)
    this->declare_parameter<std::vector<std::string>>("managed_nodes", {"node1", "node2"});
    managed_node_names_ = this->get_parameter("managed_nodes").as_string_array();

    // Callback-група Reentrant дозволяє виконувати сервісні запити клієнтів під час обробки вхідного сервісу
    cb_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);

    for (const auto & name : managed_node_names_) {
      std::string srv_name = "/" + name + "/change_state";
      clients_[name] = this->create_client<lifecycle_msgs::srv::ChangeState>(srv_name, rclcpp::ServicesQoS(), cb_group_);
    }

    // Сервіс для запуску послідовної ініціалізації та активації керованих вузлів (Homework Ch. 2 M. M. Bass)
    trigger_srv_ = this->create_service<std_srvs::srv::Trigger>(
      "start_managed_nodes",
      std::bind(&LifecycleSupervisor::handle_start_request, this, std::placeholders::_1, std::placeholders::_2),
      rclcpp::ServicesQoS(),
      cb_group_);

    RCLCPP_INFO(
      this->get_logger(),
      "Супервізор ініціалізовано. Очікування виклику сервісу '/start_managed_nodes' для конфігурації та активації вузлів.");
  }

private:
  bool change_node_state(const std::string & node_name, std::uint8_t transition_id)
  {
    auto client = clients_[node_name];
    if (!client->wait_for_service(5s)) {
      RCLCPP_ERROR(this->get_logger(), "Сервіс change_state для вузла '%s' недоступний!", node_name.c_str());
      return false;
    }

    auto request = std::make_shared<lifecycle_msgs::srv::ChangeState::Request>();
    request->transition.id = transition_id;

    auto future = client->async_send_request(request);
    if (future.wait_for(5s) != std::future_status::ready) {
      RCLCPP_ERROR(this->get_logger(), "Тайм-аут очікування переходу стану для вузла '%s'", node_name.c_str());
      return false;
    }

    return future.get()->success;
  }

  void handle_start_request(
    const std::shared_ptr<std_srvs::srv::Trigger::Request> /*request*/,
    std::shared_ptr<std_srvs::srv::Trigger::Response> response)
  {
    RCLCPP_INFO(this->get_logger(), "Отримано запит на послідовну ініціалізацію та активацію керованих вузлів...");

    for (const auto & name : managed_node_names_) {
      RCLCPP_INFO(this->get_logger(), "--> Конфігурування вузла '%s' (TRANSITION_CONFIGURE)...", name.c_str());
      if (!change_node_state(name, lifecycle_msgs::msg::Transition::TRANSITION_CONFIGURE)) {
        response->success = false;
        response->message = "Помилка конфігурування вузла " + name;
        return;
      }

      RCLCPP_INFO(this->get_logger(), "--> Активація вузла '%s' (TRANSITION_ACTIVATE)...", name.c_str());
      if (!change_node_state(name, lifecycle_msgs::msg::Transition::TRANSITION_ACTIVATE)) {
        response->success = false;
        response->message = "Помилка активації вузла " + name;
        return;
      }
    }

    response->success = true;
    response->message = "Усі керовані вузли успішно сконфігуровано та активовано!";
    RCLCPP_INFO(this->get_logger(), "%s", response->message.c_str());
  }

  std::vector<std::string> managed_node_names_;
  rclcpp::CallbackGroup::SharedPtr cb_group_;
  std::map<std::string, rclcpp::Client<lifecycle_msgs::srv::ChangeState>::SharedPtr> clients_;
  rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr trigger_srv_;
};

}  // namespace lab1_nodes

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(lab1_nodes::LifecycleSupervisor)
