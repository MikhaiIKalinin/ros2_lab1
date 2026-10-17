# Лабораторна робота №1: Вступ до ROS 2, налаштування середовища та розробка вузлів (Nodes)

Репозиторій містить повне виконання завдань Лабораторної роботи №1 з дисципліни **«Розробка програмного забезпечення систем реального часу»** (за посібником M. M. Bassa *«A Very Informal Journey Through ROS 2»*).

## Структура репозиторію

```text
ros2_lab1_repo/
├── Dockerfile                        # Конфігурація образу ROS 2 (Завдання 1.1)
├── docker-compose.yml                # Оркестрація контейнера з монтуванням ros2_ws/src
├── entrypoint.sh                     # Автоматичний сорсинг /opt/ros/$ROS_DISTRO/setup.bash
├── README.md                         # Інструкція з розгортання та запуску
└── ros2_ws/
    └── src/
        └── lab1_nodes/               # C++ пакет ROS 2 (ament_cmake)
            ├── CMakeLists.txt        # Правила збирання SHARED-бібліотек і реєстрації компонентів
            ├── package.xml           # Маніфест пакета та залежностей
            ├── launch/
            │   ├── lifecycle_launch.py             # Запуск і переходи станів через LifecycleTransition
            │   └── supervisor_components.launch.py # Завантаження компонентів у єдиний Executor
            └── src/
                ├── component_node.cpp              # Завдання 1.2: Вузол-компонент (ComponentNode)
                ├── managed_node.cpp                # Завдання 1.3: Керований вузол (ManagedNode)
                ├── lifecycle_supervisor.cpp        # Завдання 1.3: Супервізор життєвого циклу
                └── lifecycle_direct_main.cpp       # Завдання 1.3: Пряме керування Transition у Executor
```

## Швидкий старт

### 1. Розгортання через Docker (Завдання 1.1)
```bash
docker build -t ros2_lab1 .
docker run -it --rm ros2_lab1
```

### 2. Збирання пакета через colcon
```bash
cd /root/ros2_ws
colcon build --symlink-install
source install/setup.bash
```

### 3. Перевірка вузла-компонента (Завдання 1.2)
```bash
ros2 component types | grep lab1_nodes
# Запуск контейнера компонентів і завантаження lab1_nodes::ComponentNode:
ros2 run rclcpp_components component_container &
ros2 component load /ComponentManager lab1_nodes lab1_nodes::ComponentNode
```

### 4. Запуск керованих вузлів та супервізора (Завдання 1.3)
- **Варіант А (прямий запуск виконуваного файлу з Transition):**
  ```bash
  ros2 run lab1_nodes lifecycle_direct_demo
  ```
- **Варіант Б (через Launch-дію `LifecycleTransition`):**
  ```bash
  ros2 launch lab1_nodes lifecycle_launch.py
  ```
- **Варіант В (композиція в єдиному `MultiThreadedExecutor` + виклик сервісу супервізора):**
  ```bash
  ros2 launch lab1_nodes supervisor_components.launch.py
  # У іншому терміналі:
  ros2 service call /start_managed_nodes std_srvs/srv/Trigger {}
  ```
