cmake_minimum_required(VERSION 3.8)
project(lab1_nodes)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Пошук необхідних пакетів ROS 2
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(lifecycle_msgs REQUIRED)
find_package(std_srvs REQUIRED)

# =============================================================================
# Завдання 1.2: Збирання спільної бібліотеки вузла-компонента (ComponentNode)
# =============================================================================
add_library(component_node SHARED src/component_node.cpp)
target_compile_definitions(component_node PRIVATE "COMPOSITION_BUILDING_DLL")
ament_target_dependencies(component_node
  "rclcpp"
  "rclcpp_components"
)
rclcpp_components_register_nodes(component_node "lab1_nodes::ComponentNode")

# =============================================================================
# Завдання 1.3: Збирання керованих вузлів (ManagedNode) та супервізора як компонентів
# =============================================================================
add_library(managed_nodes_components SHARED
  src/managed_node.cpp
  src/lifecycle_supervisor.cpp
)
target_compile_definitions(managed_nodes_components PRIVATE "COMPOSITION_BUILDING_DLL")
ament_target_dependencies(managed_nodes_components
  "rclcpp"
  "rclcpp_components"
  "rclcpp_lifecycle"
  "lifecycle_msgs"
  "std_srvs"
)
rclcpp_components_register_node(managed_nodes_components
  PLUGIN "lab1_nodes::ManagedNode"
  EXECUTABLE managed_node_exec
)
rclcpp_components_register_node(managed_nodes_components
  PLUGIN "lab1_nodes::LifecycleSupervisor"
  EXECUTABLE lifecycle_supervisor_exec
)

# Автономний виконуваний файл для прямої демонстрації переходів станів двох LifecycleNode
add_executable(lifecycle_direct_demo src/lifecycle_direct_main.cpp)
ament_target_dependencies(lifecycle_direct_demo
  "rclcpp"
  "rclcpp_lifecycle"
  "lifecycle_msgs"
)

# Встановлення скомпільованих бібліотек, виконуваних файлів та launch-скриптів
install(TARGETS
  component_node
  managed_nodes_components
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(TARGETS
  lifecycle_direct_demo
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}/
)

ament_package()
