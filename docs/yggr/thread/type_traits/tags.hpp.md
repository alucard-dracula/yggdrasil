# `tags.hpp` 中文说明

## 用途

本头文件在 `yggr::thread::type_traits` 命名空间中定义工作运行器类别的编译期标签，用于区分静态和动态工作运行器。

```cpp
#include <yggr/thread/type_traits/tags.hpp>
```

这些标签是空结构体，通常作为运行器的 `tag_type`，供模板和编译期断言识别运行器种类。

## 标签类型

| 标签 | 含义 |
| --- | --- |
| `tag_static_work_runner` | 标识静态工作运行器。 |
| `tag_dynamic_work_runner` | 标识动态工作运行器。 |

例如，`static_work_runner` 使用 `tag_static_work_runner`，`dynamic_work_runner` 使用 `tag_dynamic_work_runner`。适配器或泛型组件可根据该类型约束允许接入的 runner。

## 使用示例

```cpp
struct my_runner
{
    typedef yggr::thread::type_traits::tag_dynamic_work_runner tag_type;
};
```

一般无需创建标签对象；直接将标签类型放入组件的 `tag_type` 类型别名即可。
