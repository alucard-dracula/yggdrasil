# basic_proxy_mode.hpp — 中文文档

概览
- `basic_proxy_mode` 是一个抽象基类，用于表示代理模式（proxy mode）的基础行为。
- 它依赖于 `ModeDataStore` 模板参数提供的所有者信息容器和存储类型，并通过纯虚函数 `pro_get_owner_info_container` 由派生类实现具体的模式查询逻辑。

模板参数
- `ModeDataStore`：模式数据存储类型。该类型必须定义：
  - `owner_info_container_type`
  - `owner_info_type`
  - `owner_id_type`
  - `store_type`
  - `ex_args_type`

类型别名
- `mode_data_store_type`：对应模板参数 `ModeDataStore`。
- `owner_info_container_type`：`ModeDataStore` 中定义的所有者信息容器类型。
- `owner_info_type`：所有者信息类型。
- `owner_id_type`：所有者 ID 类型。
- `store_type`：底层存储类型。
- `ex_args_type`：扩展参数类型。

构造与析构
- 构造函数 `basic_proxy_mode(u32 mode)`：初始化当前模式 `_mode`。
- 虚析构函数 `~basic_proxy_mode()`：保证派生类能够正确析构。

主要接口
- `u32 now_mode() const`：返回当前模式值。

- `get_owner_info_container(owner_info_container_type& cont, const owner_info_type& owner_info, const store_type& store) const`：
  - 获取与给定 `owner_info` 和 `store` 相关的所有者信息集合。
  - 内部调用 `pro_get_owner_info_container(cont, owner_info, store, 0)`。

- `get_owner_info_container(owner_info_container_type& cont, const owner_info_type& owner_info, const store_type& store, const utility::basic_args_holder_nv& args_holder) const`：
  - 带扩展参数 `args_holder` 的版本，内部将其地址传给 `pro_get_owner_info_container`。

纯虚函数
- `pro_get_owner_info_container(owner_info_container_type& cont, const owner_info_type& owner_info, const store_type& store, const utility::basic_args_holder_nv* pargs_holder) const`：
  - 派生类必须实现此方法，用于根据当前模式和传入参数生成或过滤 `owner_info_container_type`。
  - `pargs_holder` 指向可选的额外参数；若为 `nullptr`，表示没有额外参数。

依赖与包含
- `yggr/mplex/static_assert.hpp`
- `yggr/utility/args_holder_nv.hpp`
- `yggr/proxy/proxy_mode/proxy_mode_def.hpp`
- `boost/type_traits/is_same.hpp`
- `boost/range/value_type.hpp`

注意事项
- `basic_proxy_mode` 仅包含模式编号和统一接口，具体的模式行为由派生类实现。
- `get_owner_info_container` 返回对传入容器 `cont` 的引用，调用方应确保容器在调用后继续有效。

示例用法（伪代码）

  class my_proxy_mode : public basic_proxy_mode<MyModeDataStore> {
  protected:
      owner_info_container_type& pro_get_owner_info_container(... ) const override {
          // 按模式实现查询/过滤逻辑
      }
  };

  my_proxy_mode mode_impl(1);
  mode_impl.get_owner_info_container(cont, owner_info, store);

参考文件
- 原始实现： [basic_proxy_mode.hpp](basic_proxy_mode.hpp)
