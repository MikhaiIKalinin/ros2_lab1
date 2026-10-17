#!/usr/bin/env bash
set -e

# Сорсинг базового системного середовища ROS 2
if [ -f "/opt/ros/${ROS_DISTRO}/setup.bash" ]; then
    source "/opt/ros/${ROS_DISTRO}/setup.bash"
fi

# Сорсинг локального робочого простору ros2_ws (якщо проєкт зібрано)
if [ -f "/root/ros2_ws/install/setup.bash" ]; then
    source "/root/ros2_ws/install/setup.bash"
fi

exec "$@"
