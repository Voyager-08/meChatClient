# meChat 即时通讯客户端

## 项目概述

基于 Qt/C++ 开发的跨平台即时通讯应用，采用现代化的软件架构设计，实现了高内聚、低耦合的系统结构。

## 技术亮点

### 1. Composition Root 架构模式

- 在 `main.cpp` 中集中创建所有依赖对象（EventBus、NetworkManager、DatabaseManager）
- 通过依赖注入将服务层传递给 `MainWindow`，实现控制反转（IoC）
- 消除了全局单例的滥用，提升了代码的可测试性和可维护性

```cpp
// Composition Root - 依赖组装
EventBus eventBus;
MainWindow window(&eventBus, &networkManager, &databaseManager);
```

### 2. 纯信号中介模式（Pure Signal Mediator）

- `EventBus` 作为纯信号路由器，不包含任何业务逻辑
- 实现了 UI 层与服务层的完全解耦
- 所有信号连接使用 `Qt::QueuedConnection` 确保线程安全
- 清晰的信号分类：请求信号（UI → Service）和响应信号（Service → UI）

```cpp
// 信号转发链路
ChatWindow → EventBus → NetworkManager/DatabaseManager → EventBus → ChatWindow
```

### 3. 多线程架构

- `NetworkManager` 和 `DatabaseManager` 运行在独立子线程
- 通过信号槽机制实现跨线程通信，避免阻塞 UI 线程
- 使用 `QMetaObject::invokeMethod` 实现线程安全的初始化

```cpp
// 子线程管理
QThread networkThread;
networkManager.moveToThread(&networkThread);
QMetaObject::invokeMethod(&networkManager, &NetworkManager::initNetwork, Qt::QueuedConnection);
```

### 4. 严格的信号转发机制

- 底层 Widget 信号必须通过上层 Window 转发
- EventBus 只绑定 Window 级别的接口，不直接连接 Widget
- 实现了清晰的职责分离和信号流向

```cpp
// 信号转发示例
LoginWidget → LoginWindow → EventBus → NetworkManager
RegisterWidget → LoginWindow → EventBus → NetworkManager
```

### 5. 智能指针防御性编程

- 使用 `QPointer<T>` 管理动态创建/销毁的对象（如 ChatWindow）
- 外部传入的依赖使用 `QPointer` 防止悬空指针
- 自动置空机制避免访问已销毁对象导致的崩溃

```cpp
QPointer<ChatWindow> chatWindow;      // 动态创建/销毁
QPointer<EventBus> eventBus;          // 外部传入依赖
QPointer<NetworkManager> networkManager;
```

### 6. Model/View 架构

- 采用 Qt 的 Model/View 模式实现数据与界面分离
- 自定义 Model（ContactModel、MessageModel、AddContactModel）
- 自定义 Delegate（ContactDelegate、MessageDelegate）实现个性化渲染
- 支持大量数据的懒加载和高效显示

### 7. 模块化设计

```
src/
├── core/           # 核心业务逻辑
│   ├── chat/       # 聊天模块
│   ├── login/      # 登录注册模块
│   ├── main/       # 主窗口（Composition Root）
│   └── extra/      # 扩展功能（消息气泡等）
├── event/          # 事件总线（信号中介）
├── network/        # 网络通信层
├── sql/            # 数据库持久化层
├── modelView/      # 数据模型与视图委托
└── custom/         # 自定义控件与数据结构
```

### 8. 依赖注入与控制反转

- UI 层不直接依赖服务层，通过信号间接通信
- 服务层不依赖 UI 层，通过信号返回结果
- 所有依赖在 Composition Root 中显式注入
- 提升了代码的可测试性，便于 Mock 和单元测试

## 核心功能

- 用户注册与登录
- 实时消息收发
- 好友管理（添加、删除、搜索）
- 消息历史记录本地存储
- 心跳保活机制
- 用户在线状态同步
- 文件传输
- 朋友圈/收藏功能

## 技术栈

- **语言**: C++11/14
- **框架**: Qt 6.5.3
- **架构**: Composition Root + Pure Signal Mediator
- **并发**: Qt 多线程 + 信号槽机制
- **持久化**: SQLite（通过 QSqlDatabase）
- **网络**: TCP Socket 通信

## 设计原则

- **单一职责原则（SRP）**: 每个类只负责一个功能领域
- **开闭原则（OCP）**: 通过信号槽机制扩展功能，无需修改现有代码
- **依赖倒置原则（DIP）**: 高层模块不依赖低层模块，都依赖抽象（信号接口）
- **接口隔离原则（ISP）**: EventBus 提供细粒度的信号接口

## Windows 客户端部署

客户端运行在 Windows，使用 Qt 6、C++17 和 Protobuf。构建环境需要 CMake、对应编译器，以及 Qt 6 的 Core、Widgets、Sql、Network 模块和 Protobuf（含 `protoc`、C++ runtime）。CMake 使用系统/工具链中的包配置，不再依赖开发机上的固定盘符路径。

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:\Qt\6.5.3\mingw_64;C:\deps\protobuf"
cmake --build build --config Release
```

使用 Visual Studio 时，请将 `CMAKE_PREFIX_PATH` 指向匹配 MSVC 的 Qt 和 Protobuf 安装。发布时在 Qt 命令提示符中运行 `windeployqt --release --sql qsqlite .\build\bin\meChatClient.exe`，并确认 Qt 的 SQLite SQL driver（`sqldrivers\qsqlite.dll`）随包分发。

客户端连接地址通过环境变量配置；默认是本机 `127.0.0.1:6452`，部署到其他机器时必须将 host 设置为 Linux 服务器可访问的 IP/DNS：

```powershell
$env:MECHAT_SERVER_HOST = "192.0.2.10"
$env:MECHAT_SERVER_PORT = "6452"
.\build\bin\meChatClient.exe
```

## 与 Linux 服务端的协议

客户端通过 TCP 长连接通信，双方使用 `src/network/message.proto` 定义的 Protobuf `Packet`。每帧为 8 字节头部和 Protobuf body：Magic `0xABCD`（2 字节，大端）、body 长度（4 字节，大端）、CRC16/Modbus（2 字节，大端）和 body。服务端的协议定义位于 `../meChatServer/src/protocol/message.proto`；新增或变更消息时必须同步两份定义并重新构建两端。当前支持登录/注册、用户资料、好友列表与管理、聊天、心跳、离线消息和头像传输。

Linux 服务端构建、启动、数据库初始化和防火墙配置见 [`../meChatServer/README.md`](../meChatServer/README.md)。
