from launch import LaunchDescription
from launch_ros.actions import LifecycleNode
from launch_ros.actions import LifecycleTransition
from lifecycle_msgs.msg import Transition


def generate_launch_description():
    return LaunchDescription([
        LifecycleNode(
            package='lab1_nodes',
            executable='managed_node_exec',
            name='node1',
            namespace=''
        ),
        LifecycleNode(
            package='lab1_nodes',
            executable='managed_node_exec',
            name='node2',
            namespace=''
        ),
        LifecycleTransition(
            lifecycle_node_names=['node1', 'node2'],
            transitions_ids=[
                Transition.TRANSITION_CONFIGURE,
                Transition.TRANSITION_ACTIVATE
            ]
        )
    ])
