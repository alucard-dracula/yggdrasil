**概述**
- **描述：** `packet_info.hpp` 声明了 `yggr::packet::packet_info` 模板类，这是一个可变参数的包信息容器，用于存储和操作一组数据字段。它支持 tuple 转换、序列化、比较、交换、哈希以及对字段的按类型或按索引访问。
- **头文件：** [yggr/packet/packet_info.hpp](yggr/packet/packet_info.hpp)

**文件结构**
- `packet_info.hpp` 是主入口头文件，根据是否启用 C++11 可变参数模板选择包含不同实现：
  - 启用可变参数模板时包含 `yggr/packet/detail/packet_info_vtpl.hpp`。
  - 否则包含 `yggr/packet/detail/packet_info_vmacro.hpp`。
- 这两个实现文件提供相同的公共接口，但分别使用 variadic template 或 preprocessor macro 方式实现。

**主要类**
- `template<typename ...Val> class packet_info`
  - 继承自 `mplex::conv_to_yggr_tuple<...>::type`，用元编程方式将类型列表转换为 `yggr::tuple`。
  - 允许不定数量字段，并自动忽略尾部的 `yggr::tuples::null_type`。

**核心能力**
- `E_length`：静态常量，表示有效字段数。
- `params_type`：保存字段类型列表的 Boost MPL 类型。
- `value_type`：实际底层 `yggr::tuple` 类型。
- `arg<N>`：按索引访问字段类型。
- `t_arg<T>`：按字段类型访问字段索引。

**构造**
- 支持从可变参数列表初始化。
- 支持从 `boost::tuple<T...>`、`yggr::tuple<T...>`、以及其它 `packet_info<T...>` 对象拷贝或移动构造。
- 对构造参数个数进行静态检查，确保不超过 `E_length`。

**赋值与交换**
- 支持赋值自 `boost::tuple`、`yggr::tuple`、`packet_info` 以及自身类型。
- 使用 `copy_or_move_or_swap` 实现可移动或可拷贝的赋值行为。
- 提供多种 `swap` 重载，与 `boost::tuple`、`yggr::tuple` 和其它 `packet_info` 兼容。

**访问成员**
- `clear()`：清除字段内容。
- `static size_type element_size()`：返回字段数量 `E_length`。
- `get<N>()`：按索引获取字段引用。
- `get<T>()`：按字段类型获取字段引用。
- `arg_get<Arg>()` / `t_arg_get<T_Arg>()`：按 arg 类型访问字段。

**比较操作**
- 支持 `compare_eq` / `compare` 与 `boost::tuple`、`yggr::tuple`、以及其它 `packet_info` 的比较。
- 提供全局 `operator==` 和 `operator!=` 重载，支持不同 tuple 类型之间的比较。

**辅助工厂**
- `static this_type make_packet_info(Args...)`：静态工厂函数，根据传入参数创建 `packet_info`。
- 全局函数 `yggr::packet::make_packet_info(...)`：方便构造 `packet_info` 对象。

**类型转换**
- 支持隐式转换为 `boost::tuple<T...>`。
- `cast_to_packet_info<Src<T...>>`：将 tuple 类型转换为对应的 `packet_info<T...>`。

**输入输出**
- 提供 `operator<<` 和 `operator>>`，将 `packet_info` 与标准流进行序列化/反序列化。

**哈希支持**
- `hash_value(const packet_info<T...>&)`：基于底层 tuple 的 Boost Hash 计算哈希值。
- 若启用了 `YGGR_HAS_CXX11_STD_HASH`，提供 `std::hash<packet_info<T...>>` 特化。

**辅助类**
- `packet_info_op`：提供静态工具函数，用于比较、获取子字段、以及从 `packet_info` 中提取子信息。例如 `compare(...)`、`get_sub_infos(...)` 和 `get_sub_packet_info(...)`。

**实现细节**
- 可变参数版本（`packet_info_vtpl.hpp`）使用模板参数包与 Boost MPL 元编程构建类型列表。
- 宏版本（`packet_info_vmacro.hpp`）在禁用 variadic templates 时使用预处理器生成代码，保持相同接口。
- 两者均使用 `yggr::serialization` 的序列化支持，并通过 `serializion::access` 友元化。
- `packet_info` 支持与 `boost::tuple` 和 `yggr::tuple` 的无缝互操作，包括赋值、交换和比较。

**相关文件**
- `yggr/packet/packet_info.hpp`
- `yggr/packet/detail/packet_info_vtpl.hpp`
- `yggr/packet/detail/packet_info_vmacro.hpp`

**作者 / 许可证**
- 源文件头部声明了版权与 MIT 风格许可，详情请参见源文件顶部。
