# `nvp.hpp`

## 概述

该头文件为 Boost.Serialization 的名称-值对（NVP，Name-Value Pair）机制提供 yggdrasil 封装。

主要功能包括：

- 导出 `boost::serialization::nvp` 到 yggdrasil 命名空间；
- 提供兼容性 `fake_nvp` 和 `make_nvp()`；
- 通过宏自动生成字段名称；
- 支持显式字段名称、基类对象和归档操作符封装。

## 头文件

```cpp
#include <yggr/serialization/nvp.hpp>
```

## 命名空间

```cpp
yggr::serialization
```

## 类型别名

```cpp
using boost::serialization::nvp;
```

因此可以使用：

```cpp
yggr::serialization::nvp<T>
```

表示 Boost.Serialization 的名称-值对对象。

## `fake_nvp`

```cpp
template<typename T>
struct fake_nvp;
```

`fake_nvp<T>` 继承：

```cpp
std::pair<const char*, T*>
boost::serialization::wrapper_traits<const nvp<T> >
```

内部保存字段名称和被包装对象地址：

```cpp
typedef std::pair<const char*, T*> base_type;
```

### 构造函数

```cpp
explicit fake_nvp(const char* name_, T& t);
fake_nvp(const fake_nvp& rhs);
```

构造时保存名称指针和目标对象地址，不拥有目标对象。

### 转换操作

```cpp
operator nvp<T>(void) const;
```

将兼容包装对象转换为 `nvp<T>`。该实现用于适配不同编译器或 Boost 版本下的 NVP 布局和模板推导行为。

## `make_nvp`

```cpp
template<class T>
nvp<T> make_nvp(const char* name, T& t);
```

根据字段名和对象引用创建名称-值对。

在 Clang 下直接构造 `nvp<T>`；其他编译器下通过 `fake_nvp<T>` 转换，并使用静态断言确保两种对象布局大小一致。

## 序列化宏

### `YGGR_SERIALIZATION_NVP`

```cpp
YGGR_SERIALIZATION_NVP(__name__)
```

使用预处理器将表达式名称转换为字符串，并创建对应名称-值对：

```cpp
YGGR_SERIALIZATION_NVP(value)
```

等价于使用字段名 `"value"` 包装 `value`。

### `YGGR_SERIALIZATION_BASE_OBJECT_NVP`

```cpp
YGGR_SERIALIZATION_BASE_OBJECT_NVP(__name__)
```

以名称-值对形式包装基类对象：

```cpp
yggr::serialization::base_object<__name__>(*this)
```

适用于派生类序列化其基类部分。

### `YGGR_SERIALIZATION_NAME_NVP`

```cpp
YGGR_SERIALIZATION_NAME_NVP(__name__, __val__)
```

使用显式名称包装对象：

```cpp
YGGR_SERIALIZATION_NAME_NVP("field", value)
```

适用于名称不是 C++ 变量名，或需要动态生成字段名的场景。

### `YGGR_SERIALIZATION_NVP_AR`

```cpp
YGGR_SERIALIZATION_NVP_AR(__ar__, __val__)
```

使用 `&` 操作符将自动命名的 NVP 直接应用于归档：

```cpp
__ar__ & YGGR_SERIALIZATION_NVP(__val__)
```

### `YGGR_SERIALIZATION_NAME_NVP_AR`

```cpp
YGGR_SERIALIZATION_NAME_NVP_AR(__ar__, __name__, __val__)
```

使用显式字段名将对象直接应用于归档：

```cpp
__ar__ & YGGR_SERIALIZATION_NAME_NVP(__name__, __val__)
```

### `YGGR_SERIALIZATION_WRAP_NVP_AR`

```cpp
YGGR_SERIALIZATION_WRAP_NVP_AR(__ar__, __wrap__, __val__)
```

先用 `__wrap__` 包装 `__val__`，再使用 `__val__` 的字符串名称应用于归档。

宏内部创建临时对象 `wrap_obj_tmp`，其生命周期覆盖当前宏语句块。

### `YGGR_SERIALIZATION_NAME_WRAP_NVP_AR`

```cpp
YGGR_SERIALIZATION_NAME_WRAP_NVP_AR(
    __name__, __wrap__, __val__)
```

使用显式名称包装并归档临时包装对象。

## 使用示例

```cpp
#include <yggr/serialization/nvp.hpp>

struct record
{
    int id;
    std::string name;

    template<typename Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        archive & YGGR_SERIALIZATION_NVP(id);
        archive & YGGR_SERIALIZATION_NAME_NVP("name", name);
        (void)version;
    }
};
```

直接使用工厂函数：

```cpp
void save_value(my_archive& archive, int& value)
{
    archive & yggr::serialization::make_nvp("value", value);
}
```

## 兼容性处理

`make_nvp()` 根据编译器条件选择实现：

- Clang：直接返回 `nvp<T>`；
- 其他编译器：通过 `fake_nvp<T>` 转换为 `nvp<T>`。

非 Clang 分支使用：

```cpp
BOOST_STATIC_ASSERT_MSG(
    sizeof(ret_type) == sizeof(fake_type),
    "sizeof(ret_type) must equal_to sizeof(fake_type)");
```

确保兼容包装类型与 Boost NVP 类型具有相同大小。

## 注意事项

1. NVP 只包装对象和字段名称，不拥有被包装对象的生命周期。
2. 使用 `YGGR_SERIALIZATION_NVP` 时，传入表达式应适合预处理器字符串化。
3. 需要运行时或数字字段名时，应使用 `YGGR_SERIALIZATION_NAME_NVP`。
4. `YGGR_SERIALIZATION_BASE_OBJECT_NVP` 用于基类对象包装，依赖 Boost.Serialization 的 `base_object` 支持。
5. `YGGR_SERIALIZATION_WRAP_NVP_AR` 和 `YGGR_SERIALIZATION_NAME_WRAP_NVP_AR` 使用临时包装对象，只保证其在当前宏语句块内有效。
6. `make_nvp()` 返回的 NVP 保存对象引用或地址，调用期间目标对象必须保持有效。
7. 非 Clang 编译器依赖 `nvp` 与 `fake_nvp` 的对象布局大小一致。
8. 字段名称和归档格式的具体处理由目标归档类型决定，BSON 归档可能对字段名称有额外约束。

## 相关组件

- `yggr/serialization/export.hpp`
- `yggr/serialization/collections_save_imp.hpp`
- `yggr/serialization/collections_load_imp.hpp`
- `boost/serialization/nvp.hpp`
- `boost/serialization/wrapper.hpp`
