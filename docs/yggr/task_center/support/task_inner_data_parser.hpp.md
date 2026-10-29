# `task_inner_data_parser.hpp` 中文说明

## 用途

本头文件定义 `yggr::task_center::support::task_inner_data_parser<DataContainer>`，用于从任务数据容器中获取指定类型的实际数据。它是 `task<T>` 的默认数据解析策略，可通过模板参数为自定义容器提供解析逻辑。

```cpp
#include <yggr/task_center/support/task_inner_data_parser.hpp>
```

## 通用解析器

```cpp
template<typename T>
struct task_inner_data_parser;
```

通用实现将 `T` 作为 `data_container_type`，并调用容器自身的 `parse_data<Real_Data>()`：

```cpp
template<typename Real_Data>
static Real_Data* inner_data(data_container_type& cont);

template<typename Real_Data>
static const Real_Data* inner_data(const data_container_type& cont);
```

- 非常量容器重载返回可写的 `Real_Data*`。
- 常量容器重载返回只读的 `const Real_Data*`。
- 具体类型检查和解析行为由 `data_container_type::parse_data<Real_Data>()` 决定。

自定义容器需提供与这两个调用匹配的成员模板。

## `yggr::any` 特化

对 `yggr::any` 的特化先通过断言检查 `typeid(Real_Data) == cont.type()`，然后使用 `yggr::any_cast<Real_Data>()` 获取对象指针。

```cpp
task_inner_data_parser<yggr::any>::inner_data<Real_Data>(cont);
```

若容器中的实际类型与 `Real_Data` 不匹配，调试构建中的断言会触发；调用方应确保模板实参与存储的数据类型一致，并检查返回指针。

## `boost::any` 特化

对 `boost::any` 的特化采用相同的处理方式：断言运行时类型匹配，再通过 `boost::any` 对应的 `yggr::any_cast` 接口取得对象指针。该特化同样提供可写和只读容器重载。

## 与 `task` 的配合

`task` 的 `get<T>()` 将其内部 `data_container_type` 交给 `task_inner_data_parser<data_container_type>::inner_data<T>()`。默认容器为 `yggr::any`，因此默认使用相应特化；若为 `task` 指定其他 `DataContainer`，可提供同名解析器特化或自定义解析器模板。

```cpp
template<typename T>
T* get();

template<typename T>
const T* get() const;
```

## 使用注意

- 查询类型 `Real_Data` 必须与容器中保存的具体类型匹配。
- `inner_data()` 返回指针；使用前应确认指针非空。
- `yggr::any` 和 `boost::any` 特化通过 `assert` 校验类型，断言关闭时不能依赖该校验来处理错误类型。
- 对自定义容器，解析器需保留常量正确性：常量容器应返回 `const Real_Data*`。
