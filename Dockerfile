# Лабораторна робота №1: Середовище розробки ROS 2 (C++)
ARG ROS_DISTRO=jazzy
FROM osrf/ros:${ROS_DISTRO}-desktop

ENV DEBIAN_FRONTEND=noninteractive
ENV ROS_DISTRO=${ROS_DISTRO}

# Встановлення утиліт збирання colcon, компіляторів C++ та пакетів rclcpp_components / rclcpp_lifecycle
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3-colcon-common-extensions \
    python3-rosdep \
    ros-${ROS_DISTRO}-rclcpp \
    ros-${ROS_DISTRO}-rclcpp-components \
    ros-${ROS_DISTRO}-rclcpp-lifecycle \
    ros-${ROS_DISTRO}-lifecycle-msgs \
    ros-${ROS_DISTRO}-std-srvs \
    ros-${ROS_DISTRO}-launch-ros \
    && rm -rf /var/lib/apt/lists/*

# Створення робочого простору ros2_ws
WORKDIR /root/ros2_ws
COPY ros2_ws/src /root/ros2_ws/src

# Збирання пакетів із прапорцем --symlink-install
RUN /bin/bash -c "source /opt/ros/${ROS_DISTRO}/setup.bash && \
    colcon build --symlink-install"

# Автоматичний сорсинг у інтерактивних сесіях bash
RUN echo "source /opt/ros/${ROS_DISTRO}/setup.bash" >> /root/.bashrc && \
    echo "if [ -f /root/ros2_ws/install/setup.bash ]; then source /root/ros2_ws/install/setup.bash; fi" >> /root/.bashrc

# Налаштування entrypoint.sh
COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
CMD ["bash"]
