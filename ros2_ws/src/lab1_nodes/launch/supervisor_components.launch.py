from launch import LaunchDescription
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode


def generate_launch_description():
    container = ComposableNodeContainer(
        name='lab1_component_container',
        namespace='',
        package='rclcpp_components',
        executable='component_container_mt',
        composable_node_descriptions=[
            ComposableNode(
                package='lab1_nodes',
                plugin='lab1_nodes::ComponentNode',
                name='best_node_name'
            ),
            ComposableNode(
                package='lab1_nodes',
                plugin='lab1_nodes::ManagedNode',
                name='node1'
            ),
            ComposableNode(
                package='lab1_nodes',
                plugin='lab1_nodes::ManagedNode',
                name='node2'
            ),
            ComposableNode(
                package='lab1_nodes',
                plugin='lab1_nodes::LifecycleSupervisor',
                name='lifecycle_supervisor',
                parameters=[{'managed_nodes': ['node1', 'node2']}]
            ),
        ],
        output='screen',
    )

    return LaunchDescription([container])
