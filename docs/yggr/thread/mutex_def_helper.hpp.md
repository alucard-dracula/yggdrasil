# `mutex_def_helper.hpp` 中文说明

## 用途

本头文件定义 `yggr::thread::mutex_def_helper<Mutex>`，用于统一获取互斥量类型及其锁类型。该辅助模板针对不同互斥量提供特化，常用于依赖锁策略的线程组件。

```cpp
#include <yggr/thread/mutex_def_helper.hpp>
```

`mutex_def_helper` 私有继承 `nonable::noncreateable`，只用于通过类型别名进行编译期配置，不需要也不能创建对象。

## 默认互斥量

对于普通互斥量类型：

```cpp
template<typename Mutex>
class mutex_def_helper;
```

公开类型别名为：

| 别名 | 定义 |
| --- | --- |
| `mutex_type` | 模板参数 `Mutex`。 |
| `lock_type` | `Mutex::scoped_lock`。 |
| `read_lock_type` | 与 `lock_type` 相同。 |
| `write_lock_type` | 与 `lock_type` 相同。 |

该形式适用于提供嵌套类型 `scoped_lock` 的互斥量。

## `void` 默认配置

```cpp
mutex_def_helper<void>
```

此特化将 `void` 解释为默认的 `boost::mutex`，并公开：

- `mutex_type = boost::mutex`
- `lock_type = boost::mutex::scoped_lock`
- `read_lock_type` 和 `write_lock_type` 均等于 `lock_type`

因此，以 `void` 作为互斥量模板参数时，相关组件可使用普通 Boost 互斥量。

## 共享互斥量

对于 `boost::shared_mutex`，特化提供不同的读写锁：

| 别名 | 定义 |
| --- | --- |
| `mutex_type` | `boost::shared_mutex` |
| `lock_type` | `boost::shared_lock<mutex_type>` |
| `read_lock_type` | 共享锁 `lock_type` |
| `write_lock_type` | `boost::unique_lock<mutex_type>` |

读锁允许共享读取，写锁以独占方式持有互斥量。

## 进程间互斥量

对于 `boost::interprocess::interprocess_mutex`，特化将 `lock_type` 定义为 `boost::interprocess::scoped_lock<mutex_type>`，并将其同时用作读锁和写锁。该互斥量不提供本文件中那种区分共享读锁与独占写锁的接口。

## Tuple 锁配置

头文件还包含针对 `yggr::tuple`、`boost::tuple` 以及特定模板模板参数形式的二元组和三元组特化。它们的实现意图是：

- 从第一个元素解析基础互斥量。
- 二元组以第二个元素作为读锁和写锁类型。
- 三元组分别以第二、第三个元素作为读锁和写锁类型。

**可见性注意：**这些 Tuple 特化中的类型别名在当前实现中没有放在 `public:` 区域，因此是私有成员。外部代码不能通过这些特化访问 `mutex_type`、`read_lock_type` 或 `write_lock_type`。如需将 Tuple 配置用于依赖这些别名的代码，应先确认项目中是否有其他适配层，或修正该可见性后再使用。

## 平台兼容

- 头文件要求首先包含 `yggr/base/yggrdef.h`，以便平台宏正确生效。
- Android 构建下会定义 `BOOST_INTERPROCESS_BUGGY_POSIX_PROCESS_SHARED`（若尚未定义），用于规避注释所述的 `PTHREAD_MUTEX_ROBUST` 声明兼容问题。
- MSVC 下会在包含和定义过程中临时禁用警告 4624，并在文件末尾恢复警告状态。

## 使用示例

```cpp
typedef yggr::thread::mutex_def_helper<void> default_mutex_def;
typedef default_mutex_def::mutex_type mutex_type;
typedef default_mutex_def::lock_type lock_type;

typedef yggr::thread::mutex_def_helper<boost::shared_mutex> shared_mutex_def;
typedef shared_mutex_def::read_lock_type read_lock_type;
typedef shared_mutex_def::write_lock_type write_lock_type;
```

使用示例中的互斥量类型前，应包含相应的 Boost.Thread 定义。本头文件提供的是类型选择辅助，不会创建或锁定互斥量。
