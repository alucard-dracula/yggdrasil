# proxy_mode_creator.hpp — 中文文档

概览
- `proxy_mode_creator` 用于动态创建代理模式对象（`basic_proxy_mode` 的派生类型）。
- 它通过模板参数 `ModeVector` 定义可支持的模式列表，并用工厂映射将模式 ID 映射到对应的创建函数。

模板参数
- `ModeVector`：Boost.MPL 序列，包含所有可用模式类型。
- `ModeDataStore`：模式数据存储类型，提供 `owner_info_container_type`、`owner_info_type`、`owner_id_type` 和 `store_type`。

类型别名
- `mode_vt_type`：`ModeVector`。
- `mode_data_store_type`：`ModeDataStore`。
- `owner_info_container_type`、`owner_info_type`、`owner_id_type`：来自 `ModeDataStore`。
- `store_type`：来自 `ModeDataStore`。
- `base_proxy_mode_type`：`basic_proxy_mode<mode_data_store_type>`，所有模式对象的公共基类。
- `interface_proxy_mode_type`：`shared_ptr<base_proxy_mode_type>`，工厂返回的接口指针类型。

内部实现
- `creator_type`：`boost::function1<interface_proxy_mode_type, const this_type&>`，对 `proxy_mode_creator` 的引用进行创建。
- `creator_map_type`：安全无序映射，键类型为 `u32`，值为 `creator_type`。
- `E_mode_length`：模式数量，等于 `boost::mpl::size<mode_vt_type>::value`。

初始化逻辑
- `init_creator<idx, idx_size>` 递归模板用于初始化 `_creator_map`：
  - 通过 `boost::mpl::at_c<mode_vt_type, idx>::type` 获取当前模式类型 `mode_type`。
  - 通过 `mode_type::E_mode` 作为键，将一个绑定到 `prv_s_create_mode<mode_type>` 的 creator 插入映射。
  - 如果未达到末尾，则递归构造下一个 `init_creator`；否则使用 `end_creator` 结束。
- 构造函数 `proxy_mode_creator()` 调用 `init_creator<0, E_mode_length> init; init(_creator_map);` 并断言映射大小与模式数量一致。

创建与调用
- `operator()(const key_type& key) const`：
  - 从 `_creator_map` 获取对应 `creator`。
  - 若找到，调用 `creator(*this)` 返回相应模式对象；否则返回空指针。

辅助函数
- `prv_s_create_mode<T>()`：静态工厂函数，使用 `construct_shared<base_proxy_mode_type>(yggr_nothrow_new now_proxy_mode_type())` 创建 `T` 类型对象，并以 `base_proxy_mode_type` 智能指针返回。

依赖与包含
- `yggr/base/static_constant.hpp`
- `yggr/bind/bind.hpp`
- `yggr/ppex/friend.hpp`
- `yggr/smart_ptr_ex/shared_ptr.hpp`
- `yggr/proxy/proxy_mode/basic_proxy_mode.hpp`
- `yggr/safe_container/safe_unordered_map.hpp`
- `boost/mpl/vector.hpp`
- `boost/mpl/size.hpp`
- `boost/mpl/at.hpp`
- `boost/mpl/push_back.hpp`
- `boost/function.hpp`
- `algorithm`

注意事项
- `ModeVector` 中每个模式类型必须定义静态常量 `E_mode`，并且 `E_mode` 唯一。
- 模式创建函数使用 `yggr_nothrow_new`，因此若内存分配失败会返回空指针。
- 生成的模式对象由 `shared_ptr<base_proxy_mode_type>` 管理。

示例用法（伪代码）

  typedef boost::mpl::vector<proxy_mode_blanace<MyDataStore>, ...> mode_list;
  proxy_mode_creator<mode_list, MyDataStore> creator;
  auto mode_obj = creator(proxy_mode_blanace<MyDataStore>::E_mode);
  if(mode_obj) {
      // 使用 mode_obj
  }

参考文件
- 原始实现： [proxy_mode_creator.hpp](proxy_mode_creator.hpp)
