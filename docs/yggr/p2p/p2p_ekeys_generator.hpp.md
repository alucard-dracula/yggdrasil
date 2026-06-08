**概述**
- **描述：** `p2p_ekeys_generator` 是一个用于生成 P2P 加密密钥对的简单工具类模板。它使用 `yggr::ids::id_generator` 对同一密钥类型生成两个相同的密钥值，用于源端与目标端的加密对等信息。
- **头文件：** [yggr/p2p/p2p_ekeys_generator.hpp](yggr/p2p/p2p_ekeys_generator.hpp)

**模板参数**
- `EKeys`：加密密钥类型。

**类型别名**
- `ekeys_type`：等同于 `EKeys`。
- `gen_type`：基于 `yggr::ids::id_generator<ekeys_type>` 的生成器类型。
- `result_type`：`std::pair<ekeys_type, ekeys_type>`，表示生成的源密钥和目标密钥对。

**主要函数**
- `inline void operator()(ekeys_type& src, ekeys_type& dst) const`
  - 生成一对密钥，并将结果写入 `src` 与 `dst`。

- `inline result_type& operator()(result_type& rst) const`
  - 生成密钥对并写入传入的 `std::pair`，然后返回该引用。

- `inline result_type operator()(void) const`
  - 生成一个新的密钥对并返回该 `std::pair`。

**内部实现**
- `inline static void pro_s_gen(ekeys_type& src, ekeys_type& dst)`
  - 创建 `gen_type gen`。
  - 通过 `gen()` 生成一个密钥值，并分别赋值给 `src` 和 `dst`，因此源密钥与目标密钥默认相同。

**设计说明**
- 本类旨在为 P2P 引导信息提供默认的加密密钥生成机制。
- 如果需要更复杂的密钥生成策略，可以继承该模板或重写生成逻辑。

**示例用法**
```cpp
using EKeys = MyEncryptionKeyType;
using Generator = yggr::p2p::p2p_ekeys_generator<EKeys>;

Generator gen;
EKeys src, dst;
gen(src, dst);

std::pair<EKeys, EKeys> keys = gen();
```

**相关文件**
- 源文件： [yggr/p2p/p2p_ekeys_generator.hpp](yggr/p2p/p2p_ekeys_generator.hpp)
- 依赖：`yggr/ids/id_generator.hpp`

**作者 / 许可证**
- 源文件顶部包含版权与 MIT 风格许可声明，详情请参见源文件顶部。
