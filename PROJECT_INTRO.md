# 工业级高性能即时通讯系统（C/S 架构）

> 简历项目介绍（C++ 上位机方向） · 含可展开技术点问答
> 技术栈：C++14 · Qt 6 · Linux · Arm · SQLite · TCP 自定义二进制协议 · Protobuf · 设计模式 · CMake

---

## 一、项目描述

基于 Qt 的跨平台即时通讯系统，服务端部署于 **Arm + Linux** 设备，覆盖完整 IM 业务闭环：用户注册登录、实时私聊、好友管理与在线状态同步；客户端支持离线消息拉取、文件传输、心跳保活与本地消息持久化；服务端在高并发场景下完成账号鉴权、消息路由转发与用户状态广播。采用**自定义二进制协议 + Protobuf** 替代传统 JSON 通信，并通过 Composition Root 与 EventBus 实现网络、数据库、UI 三模块彻底解耦。

---

## 二、核心亮点

- **模块解耦（EventBus + Composition Root）**：以 Composition Root 模式在 `main()` 中统一创建 `EventBus`、`NetworkManager`、`DatabaseManager`，通过构造函数依赖注入至 `MainWindow`。UI 层仅发射信号至 EventBus，由其路由到子线程中的 DB / Network 模块，实现三模块彻底解耦，主线程零 I/O 阻塞。
- **严格遵循 Qt 线程亲缘性**：`NetworkManager` 与 `DatabaseManager` 经 `moveToThread` 迁移至独立线程，`QTcpSocket`、`QSqlDatabase` 等对象通过 `QMetaObject::invokeMethod` 在子线程事件循环内**延迟实例化**，杜绝主线程误初始化；线程退出执行 `quit() + wait()`，保证事件排空后安全析构。
- **自定义 Header + Body 二进制协议替代 JSON**：Header 含 `MagicNum(0xABCD)` 快速识别合法包、`PayloadLen` 标识体长、`CRC16` 校验完整性；编解码器内部维护**累积缓冲区 + 状态机逐包解包**，彻底解决 TCP 粘包 / 半包问题。相比 JSON 体积减少约 60%、解析速度提升约 3 倍。
- **服务端内存优化**：服务端采用 `QMap<QString, QTcpSocket*>` 维护用户 ID 与 Socket 的**双向映射**，基于红黑树 O(log n) 查找，在中小规模场景下兼顾查询效率与内存开销，优于 `QVector`(O(n)) 与 `QHash`(额外哈希桶内存)。
- **多层面内存与并发安全**：数据库连接以 `std::unique_ptr` 实现 RAII 自动回收；动态对象及外部依赖统一使用 `QPointer` 防悬垂；加载状态用 `std::atomic<bool>` 无锁控制；跨线程信号全部走 `Qt::QueuedConnection`，依托事件循环天然隔离、消除数据竞争。
- **复杂 UI 绘制（Model/View 架构）**：`ContactModel` / `MessageModel` 继承 `QAbstractListModel` 抽象联系人列表与聊天消息，自定义 Delegate 在 `paint()` 中绘制头像、昵称、消息气泡与时间戳，避免大量 Widget 实例化开销，仅更新 Model 即可驱动 View 刷新。

---

## 三、整体架构

```
┌─────────────────────────── 上位机客户端 (Qt/C++) ───────────────────────────┐
│                                                                             │
│   UI 层 (登录/主界面/聊天)   ←→   EventBus (纯信号中介)   ←→   服务层         │
│   Model / View + Delegate              │                        │           │
│                                        │                ┌───────┴────────┐  │
│                                        │                │ NetworkManager │  │  ← 网络子线程
│                                        │                │  PacketCodec   │  │
│                                        │                └───────┬────────┘  │
│                                        │                ┌───────┴────────┐  │
│                                        │                │DatabaseManager │  │  ← 数据子线程
│                                        │                │  SQLite 本地缓存│  │
│                                        │                └────────────────┘  │
└────────────────────────────────────────┼────────────────────────────────────┘
                                         │ TCP 长连接（私有二进制协议）
                                         ▼
┌──────────────────────────── 通讯网关服务端 (QTcpServer) ─────────────────────┐
│  incomingConnection → socket↔userId 双向索引表 → 消息路由 / 状态广播          │
│  心跳保活  |  离线消息存储与补偿  |  SQLite 持久化  |  文件(头像)分发          │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## 四、可展开技术点 + 参考答案

### Q1：什么是 Composition Root？为什么用 EventBus 而不是直接互相调用？

**Composition Root（组合根）**：程序启动时**唯一且集中**组装对象依赖图的位置。本项目放在 `main()`：先构造 `EventBus`、`NetworkManager`、`DatabaseManager`，再注入 `MainWindow`。

- 好处：依赖关系集中可见、易于替换（Mock 测试）、消除全局单例、控制反转（IoC）。
- 传统写法 `NetworkManager` 内部 `new DatabaseManager` 会让模块硬耦合、无法复用与单测。

**为什么用 EventBus**：UI 不能直接持有子线程对象的内部状态；EventBus 作为**纯信号中介（不写业务逻辑）**，只做信号转发：

```
UI → EventBus(信号) → 队列连接 → NetworkManager/DatabaseManager 子线程
子线程 → EventBus(信号) → 队列连接 → UI
```

优点：UI 与服务层互不依赖，符合依赖倒置与单一职责。
代价：信号链路变长、调试时调用栈不直观——这是可接受的权衡。

---

### Q2：Qt 线程亲缘性是什么？为什么用 moveToThread 而不是继承 QThread？

**线程亲缘性（Thread Affinity）**：每个 `QObject` 属于创建它的线程，其**槽函数在该线程的事件循环中执行**。`QObject` 不能跨线程调用其方法，否则未定义行为。

**本项目做法**：

1. 在主线程 `new NetworkManager`（此时对象还未创建 socket）；
2. `networkManager.moveToThread(&networkThread)` 把对象归属迁到子线程；
3. 用 `QMetaObject::invokeMethod(&nm, &NetworkManager::initNetwork, Qt::QueuedConnection)` **让 `initNetwork` 在子线程事件循环里执行**，此时才 `new QTcpSocket(this)`——保证 socket 与所属对象在同一线程。

**为什么不用继承 QThread 重写 run()**：`QThread` 对象本身属于创建它的线程，只有 `run()` 在子线程执行；继承后容易把"线程对象"和"执行体"混淆，槽函数仍跑在主线程，易踩坑。`moveToThread` 更符合"用事件循环驱动"的 Qt 设计。

**退出顺序**：`quit()` 请求事件循环退出 → `wait()` 阻塞等待线程真正结束 → 事件队列排空后对象析构，避免对象析构时线程还在跑导致崩溃。

---

### Q3：TCP 为什么会有粘包 / 半包？你的协议如何解决？

**根本原因**：TCP 是**面向字节流**，只保证字节顺序，不保留应用层消息边界。一次 `readyRead` 可能收到：

- **半包**：一帧只到了一部分；
- **粘包**：多帧挤在一次读取里；
- 混合情况。

**协议设计**：

```
+----------+------------+---------+---------+
| MagicNum | PayloadLen |  CRC16  |  Body   |
|  2 字节  |   4 字节   |  2 字节 |  N 字节 |
+----------+------------+---------+---------+
 0xABCD      大端长度      校验      Protobuf
```

Header 定长 8 字节，Body 变长 → 用**长度前缀**界定边界。

**解包状态机流程**（`PacketCodec::unpack`）：

1. 新数据 `append` 进累积缓冲区 `m_buffer`；
2. `while (buffer.size() >= 8)`：读魔数；
3. 魔数不对 → `remove(0,1)` 逐字节**滑动重同步**（丢弃脏字节）；
4. 读 `PayloadLen`，超过 `MAX_PAYLOAD_SIZE(10MB)` → 视为异常包丢弃；
5. 若缓冲区不足整包 → **break 保留半包**，等下次 `readyRead`；
6. 足够则按 `8 + PayloadLen` 切出完整帧，校验 CRC16；
7. CRC 通过 → `ParseFromArray` 反序列化 Protobuf；否则丢帧继续。

这样半包、粘包、连续多帧都能正确切分。

**为什么用魔数**：快速判断数据流是否与本协议对齐，且断帧时能重新同步。
**为什么大端**：**网络字节序统一**，跨 x86 / Arm、跨编译器一致。
**为什么长度上限**：防止异常 / 恶意长度导致 `reserve` 巨量内存。

---

### Q4：为什么用 Protobuf 代替 JSON？「体积减少 60%、快 3 倍」怎么度量？

**Protobuf 优势**：

- **二进制紧凑**：字段用 tag（field number + wire type）编码，`varint` 压缩整数，不传字段名；JSON 每次都要传 `"user_id":` 这类键名。
- **解析快**：无需文本分词 / 字符串转义 / 浮点解析，直接按 tag 读字节；JSON 要词法 + 语法分析。
- **强类型 + 代码生成**：`.proto` 生成 C++ 类，字段访问是成员函数，天然类型安全。
- **可扩展**：新增字段用新 tag，旧端自动忽略未知字段；`oneof` 表达互斥负载。

**度量方法（面试要能说清）**：

- 体积：构造同样的业务消息，分别 `SerializeToString` 与 `QJsonDocument::toJson`，比较字节数（本项目测约减少 60%）；
- 速度：`QElapsedTimer` 循环序列化 / 反序列化 10 万次取平均（本项目测约 3 倍）。

**注意**：JSON 可读性好、调试方便、无编译依赖；Protobuf 需要 `.proto` 编译期生成代码。二者取舍取决于场景——高频大数据量选 Protobuf。

---

### Q5：QMap / QHash / QVector 如何选？为什么服务端用 QMap？

| 容器 | 底层 | 查找 | 特点 |
|---|---|---|---|
| `QVector`/`QList` | 连续数组 | O(n) | 内存最省、遍历快，但按 key 查慢 |
| `QHash` | 哈希表 | 均摊 O(1) | 查找最快，但有**哈希桶 / 负载因子**的额外内存，且无序 |
| `QMap` | **红黑树（有序）** | O(log n) | 有序、内存开销小于 QHash，增删稳定 |

**本项目场景**：在线用户数量为**中小规模**，映射表主要操作是"按 userId 找 socket 转发消息"。

- 用 `QMap` 兼顾查找效率（O(log n) 足够）与内存（避免 QHash 桶开销）；
- 需要 `userId → socket` 与 `socket → userId` **双向映射**，两张 QMap 配合使用；
- 若在线量级升到数十万，会考虑 `QHash` 或分片。

**红线**：绝不为了"看起来 O(1)"盲目选 QHash，中小规模下 QMap 更稳、更省。

---

### Q6：内存与并发安全是怎么做的？

1. **RAII**：`std::unique_ptr<Database> m_database` 管理数据库连接，对象析构自动关闭连接，异常 / 提前 return 也不会泄漏。
2. **QPointer 防悬垂**：`QPointer<T>` 对 `QObject` 做**弱引用**——当目标 `QObject` 被 `delete` 时自动置空（内部连接 `destroyed` 信号）。`MainWindow` 里的 `eventBus`、`networkManager`、动态创建的 `chatWindow` 都用 QPointer，访问前判空即可，避免 use-after-free。
3. **std::atomic<bool> 无锁标志**：数据加载状态用 `m_startLoading`，多线程读写无需加锁，避免锁竞争。
4. **QueuedConnection 消除数据竞争**：跨线程信号槽使用队列连接，信号发出时**参数被拷贝并放入目标线程事件队列**，槽函数在目标线程串行执行；两个线程不直接碰同一份数据，从模型上规避数据竞争。
5. **注意**：自定义类型跨线程传递必须 `qRegisterMetaType` 注册，否则队列连接运行时报错——项目启动时统一注册。

---

### Q7：Model/View 相比直接 new Widget 有什么优势？

- **对象数量**：1000 条消息若用 QLabel 就要 1000+ 个 Widget，每个都有布局 / 绘制 / 事件开销；Model/View 下 View 复用少量可视区域，只按需渲染。
- **数据驱动**：Model 负责数据，View 负责显示，Delegate 负责绘制。数据变化后 `emit dataChanged(...)` 或 `beginInsertRows/endInsertRows`，View 自动局部刷新。
- **自定义 Delegate**：重写 `paint(QPainter*, option, index)`，用 `QPainter` 直接画头像、昵称、消息气泡、时间戳，用 `sizeHint()` 控制行高；比组合多个 Widget 更灵活、更快。
- **解耦**：同一 Model 可被不同 View 使用，便于数据与界面分离。

**核心接口**：`rowCount()`、`data(index, role)`，列表用 `QAbstractListModel`。

---

### Q8：服务端如何支撑高并发？Arm / Linux 上如何考虑？

- **事件驱动模型**：`QTcpServer` + `QTcpSocket` 基于 epoll / select 的**非阻塞事件循环**，不为每连接开线程，单线程即可管理大量空闲长连接（长连接大部分时间在等数据，事件驱动天然合适）。
- **双向索引**：`userId ↔ socket` 两张表，消息转发 O(log n)；断连时通过 `sender()` 定位并清理，`deleteLater()` 延迟释放。
- **CPU 密集操作不要阻塞事件循环**：SQLite 读写、文件 IO 若量大，应放入工作线程或分批处理，避免拖慢所有连接。
- **横向扩展**：单机连接数到瓶颈时，可在前置层做连接接入、后端按用户分片，或多进程 + `SO_REUSEPORT` 负载均衡。
- **Arm 适配**：交叉编译工具链编译 Qt / Protobuf，注意**字节序**（Arm 有大小端，协议统一大端）、字节对齐、`-O2` 优化；嵌入式内存有限，故优先 `QMap` 和省内存设计。

---

### Q9：心跳保活、离线消息与文件传输怎么实现？

- **心跳**：客户端 `QTimer` 每 30s 发 `HEARTBEAT`，服务端回 `HEARTBEAT_RESPONSE`；若超时未响应判定断线，触发重连。作用：探测半开连接（对端其实已死但本端未收到 FIN）、维持 NAT 映射。
- **离线消息**：转发时目标 socket 不存在 → 写入 `offline_messages` 表（附 `message_uuid`）；用户上线拉取好友列表后按时间补发，成功即删除，实现"在线直发、离线暂存、上线补偿"。
- **文件传输**：走同一条二进制协议，用 Protobuf `bytes` 字段承载二进制体，受 `MAX_PAYLOAD_SIZE` 保护；大文件应做**分片 + 序号 + 断点续传**（可讲为后续优化方向）。

---

### Q10：这个项目还能怎么优化？（加分回答）

1. 大文件**分片传输 + 断点续传 + 进度回调**；
2. 协议加入**序列号 / ACK / 重传**，从"至多一次"升级为可靠投递；
3. 消息表分页 / 索引优化，历史消息**游标分页 + 懒加载**；
4. 引入 TLS 加密长连接，账号密码**加盐哈希**存储（当前是明文，需说明是待改进项）；
5. 服务端引入连接数限流、心跳超时回收；
6. 用 `QAbstractItemModel` 的批量插入 API 优化高频刷新。

---

## 五、面试高频追问速查

| 主题 | 一句话答案 |
|---|---|
| 线程亲缘性 | QObject 归属创建它的线程，槽在归属线程事件循环执行 |
| moveToThread vs 继承 QThread | 前者事件驱动、槽在线程内；后者易混淆对象与执行体 |
| quit + wait | 先停事件循环，再阻塞等线程结束，保证安全析构 |
| MagicNum | 0xABCD，识别协议对齐，断帧滑窗重同步 |
| PayloadLen | 4 字节大端，界定消息边界，解决粘包半包 |
| CRC16 | 0xA001（Modbus）多项式，校验 Body 完整性 |
| 大端序 | 网络字节序，跨 x86/Arm 一致 |
| Protobuf vs JSON | 二进制更小更快、强类型，JSON 可读性好 |
| QueuedConnection | 参数拷贝入队，目标线程串行执行，消除数据竞争 |
| QMap vs QHash | 中小规模 QMap 省内存且有序；QHash 均摊 O(1) 但有桶开销 |
| QPointer | 弱引用 QObject，对象销毁自动置空，防悬垂 |
| Model/View | 数据驱动、复用可视项、Delegate 自绘，避免大量 Widget |
