# meChat - 桌面即时通讯客户端

基于 Qt6 开发的跨平台桌面即时通讯软件，支持实时消息收发、好友管理、历史记录持久化。

## 功能特性

- **用户系统**：注册、登录、历史登录记忆
- **实时通讯**：TCP 长连接 + 心跳保活，消息实时收发
- **好友管理**：搜索添加好友、删除好友、好友列表展示
- **消息系统**：文本消息发送/接收、本地历史消息加载
- **界面交互**：页面切换动画、消息气泡淡入动画、右键菜单

## 技术栈

| 类别 | 技术 |
|------|------|
| 语言 | C++17 |
| 框架 | Qt6 (Widgets) |
| 网络 | QTcpSocket |
| 数据库 | SQLite |
| 构建 | CMake |
| 编译器 | MinGW |

## 项目结构

```
src/
├── main.cpp                    # 程序入口
├── core/                       # 核心界面
│   ├── main/mainwindow.*       #   主窗口（登录/聊天页面切换）
│   ├── login/                  #   登录注册模块
│   │   ├── loginwindow.*       #     登录窗口
│   │   └── registerwidget.*    #     注册组件
│   ├── chat/chatwindow.*       #   聊天主窗口
│   └── extra/messagebubble.*   #   消息气泡控件
├── thread/                     # 多线程模块
│   ├── network/networkmanager.*#   网络通信线程
│   └── dataLoad/dataloader.*   #   数据加载线程
├── modelView/                  # Model/View 层
│   ├── messagemodel.*          #   消息列表 Model
│   ├── messagedelegate.*       #   消息列表 Delegate
│   ├── contactmodel.*          #   联系人 Model
│   ├── contactdelegate.*       #   联系人 Delegate
│   ├── addcontactmodel.*       #   添加好友 Model
│   └── addcontactdelegate.*    #   添加好友 Delegate
├── sql/
│   └── database.*              # SQLite 数据库封装
└── custom/                     # 自定义组件
    ├── struct.h                #   数据结构定义
    ├── meplaintextedit.*       #   自定义输入框
    └── clickablelabel.*        #   可点击标签
```

## 架构设计

### 多线程架构

```
┌─────────────────────────────────────────────────┐
│                  UI 主线程                       │
│  MainWindow → LoginWindow → ChatWindow          │
│  (页面渲染 / 动画 / 用户交互)                     │
└───────────────┬────────────────┬────────────────┘
                │  信号槽         │  信号槽
                ▼                ▼
┌───────────────────────┐ ┌───────────────────────┐
│    网络线程            │ │    数据加载线程        │
│  NetworkManager       │ │  DataLoader           │
│  (TCP Socket / 心跳)   │ │  (SQLite 读写)        │
└───────────────────────┘ └───────────────────────┘
```

- 网络 I/O 与数据库操作分离到独立线程，通过 `QObject::moveToThread` 实现
- 线程间完全通过 Qt 信号槽通信，使用 `Qt::QueuedConnection` 保证线程安全
- `QMetaObject::invokeMethod` 跨线程调度网络发送操作

### 网络协议

基于 TCP Socket + JSON 的自定义应用层协议，支持以下消息类型：

| 消息类型 | 说明 |
|----------|------|
| `login` / `login_result` | 登录请求/响应 |
| `register` / `register_result` | 注册请求/响应 |
| `message` / `message_result` | 消息发送/确认 |
| `heartbeat` / `heartbeat_result` | 心跳保活 |
| `friend_list` / `friend_info` | 好友列表/信息 |
| `add_friend` / `add_friend_result` | 添加好友 |
| `delete_friend` / `delete_friend_result` | 删除好友 |
| `search_friend` | 搜索好友 |
| `user_info` | 用户信息 |
| `logout` | 退出登录 |

### Model/View/Delegate

采用 Qt Model/View 架构驱动列表渲染：

- **Model**：继承 `QAbstractListModel`，管理数据源，通过自定义 Role 传递多字段数据
- **Delegate**：继承 `QStyledItemDelegate`，重写 `paint` 实现头像圆角裁剪、多行文本渲染
- **缓存优化**：`QCache` / `QHash` 缓存已加载头像，避免重复磁盘 I/O

## 构建与运行

### 环境要求

- CMake >= 3.16
- Qt >= 6.5
- MinGW 编译器

### 编译

```bash
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

### 运行

```bash
./bin/meChatClient.exe
```

## 核心亮点

- **自研 TCP 应用层协议**：JSON 消息体 + `msgType` 路由分发，支持 10+ 消息类型
- **三线程异步架构**：UI / 网络 / 数据库完全分离，信号槽解耦，无竞态问题
- **自定义消息气泡**：继承 `QWidget` 自绘圆角气泡 + 三角箭头，`QPropertyAnimation` 淡入动画
- **Model/View/Delegate 分层渲染**：头像缓存、圆角裁剪、多行文本，列表流畅不卡顿
- **本地 SQLite 持久化**：三表设计 + 参数化查询，历史消息秒级加载

## 许可证

MIT
