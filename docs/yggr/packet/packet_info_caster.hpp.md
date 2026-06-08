**概述**
- **描述：** `packet_info_caster.hpp` 定义了 `yggr::packet::packet_info_caster<T>` 模板，用于将任意符合模式的 tuple-like 类型转换为 `yggr::packet::packet_info` 类型。它封装了详细实现 `detail::detail_packet_info_caster`，支持 C++11 可变参数模板和旧版宏实现。
- **头文件：** [yggr/packet/packet_info_caster.hpp](yggr/packet/packet_info_caster.hpp)

**主要功能**
- 提供 `packet_info_caster<T>` 模板结构体，用于根据类型 `T` 推导对应的 `packet_info` 类型。
- 适用于将各种 tuple-like 类型（如 `boost::tuple`、`yggr::tuple`、以及其它模板类）转换为 `yggr::packet::packet_info<...>`。

**实现依赖**
- 该文件包含 `yggr/packet/detail/detail_packet_info_caster.hpp`，其中实现了对 `Src<Args...>` 模板类型的转换逻辑。

**详细实现概述**
- `detail::detail_packet_info_caster` 是一个模板元结构体，针对 `Src<Args...>` 模板类型特化，定义了 `type` 为对应的 `yggr::packet::packet_info<...>`。
- 当可用 C++11 可变参数模板时，特化形式为：
  - `template<typename ...Args, template<typename ..._Args> class Src>`
    - `typedef packet_info<typename reference_trans<Args>::type...> type;`
- 在不支持可变参数模板的环境下，使用预处理宏生成对不同参数数量的特化版本。
- `reference_trans<Args>::type` 用于将可能的引用类型转换为可存储于 `packet_info` 的实际类型。

**用途**
- `packet_info_caster` 常用于在模板元编程阶段自动推导出 `packet_info` 类型，便于将 tuple-like 数据结构转为统一的 packet 表示。
- 该模板不直接执行运行时转换，而是提供一个类型别名 `type`，用于编译期类型计算。

**典型用法**
```cpp
using source_type = boost::tuple<int, std::string>;
using dest_type = yggr::packet::packet_info_caster<source_type>::type;
// dest_type == yggr::packet::packet_info<int, std::string>
```

**相关文件**
- `yggr/packet/packet_info_caster.hpp`
- `yggr/packet/detail/detail_packet_info_caster.hpp`
- `yggr/packet/packet_info.hpp`

**作者 / 许可证**
- 源文件头部包含版权与 MIT 风格许可声明，详情请参见源文件顶部。
