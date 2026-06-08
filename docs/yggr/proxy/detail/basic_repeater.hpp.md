# basic_repeater.hpp — 中文文档

概览
- `basic_repeater` 是一个用于代理路由/广播场景的基础重复器（Repeater）辅助类。它封装了共享信息和路由器的安全访问（通过 `safe_wrap`），并提供注册/注销以及清理共享信息的基本操作。
- 该类为不可复制、不可移动（继承自 `nonable::noncopyable` 和 `nonable::nonmoveable`），意在被其他组件以组合/继承方式使用而非拷贝。

模板参数
- `Shared_Info_Ptr`：指向共享信息对象的智能指针类型，实际值类型通过 `mplex::pointer_to_value_t` 提取为 `shared_info_type`。
- `Router`：路由器类型（必须符合 `router_type` 的接口，包含 `proxy_val_type` 等定义）。

类型别名
- `shared_info_ptr_type`：模板参数 `Shared_Info_Ptr`。
- `router_type`：模板参数 `Router`。
- `shared_info_type`：`pointer_to_value_t<shared_info_ptr_type>::type`，共享信息的实际值类型。
- `shared_info_wrap_type`：`safe_container::safe_wrap<shared_info_type>`，用于对 `shared_info_type` 做线程安全访问包装。
- `shared_info_wrap_error_type`：`shared_info_wrap_type::error_type`，包装调用可能抛出的错误类型。
- `proxy_id_type`：`router_type::proxy_val_type`。
- `router_wrap_type`：`safe_container::safe_wrap<router_type>`，对路由器做线程安全包装。
- `router_ptr_type`：`router_wrap_type::value_ptr_type`，路由器内部存储的指针类型。
- `router_wrap_error_type`：`router_wrap_type::error_type`。

构造与析构
- `basic_repeater(const shared_info_ptr_type& psinfo, const router_ptr_type& proute)`：构造器，接收共享信息和路由器的指针并以 `safe_wrap` 包装保存。
- 析构函数为默认析构（私有或受保护），不会抛出异常。

主要成员函数
- `template<typename Repeater_Container> void register_repeater_data(Repeater_Container& cont)`
  - 获取 `_shared_info_wrap` 的共享指针；若为空则直接返回。
  - 调用 `shared_info_type::register_calculator(cont)` 将 `cont` 注册到共享信息中。

- `template<typename Repeater_Container> void unregister_repeater_data(Repeater_Container& cont)`
  - 类似 `register_repeater_data`，但调用 `shared_info_type::unregister_calculator(cont)` 注销容器。

- `void clear()`
  - 调用 `_shared_info_wrap.call_function(&shared_info_type::clear)` 清空共享信息；若包装调用抛出 `shared_info_wrap_error_type`，将被吞掉（捕获后空操作）。

- `shared_info_ptr_type get_shared_info() const`：返回 `_shared_info_wrap` 的共享指针。
- `router_ptr_type get_router() const`：返回 `_router_wrap` 的共享指针。

成员变量
- `_shared_info_wrap`：`shared_info_wrap_type`，线程安全包装的共享信息。
- `_router_wrap`：`router_wrap_type`，线程安全包装的路由器。

依赖与包含
- 使用了以下头文件：
  - `yggr/bind/bind.hpp`
  - `yggr/system_controller/ctrl_center.hpp`
  - `yggr/nonable/nonable.hpp`
  - `yggr/mplex/pointer_to_value_t.hpp`
  - `yggr/safe_container/safe_wrap.hpp`

注意事项
- `basic_repeater` 假定 `shared_info_type` 提供 `register_calculator`、`unregister_calculator` 和 `clear` 等接口；调用代码需确保这些成员存在并具有正确语义。
- `clear()` 捕获了包装调用可能抛出的错误，故调用该方法不会传播异常，但也无法得知是否成功。
- 该类通过 `safe_wrap` 包装成员，表明对共享资源的访问应当是线程安全的；但是具体并发语义依赖 `safe_wrap` 的实现。

示例用法（伪代码）

  // 假设 SharedInfoPtr 是 shared_ptr<SharedInfo>
  SharedInfoPtr psinfo = /* ... */;
  auto router_ptr = /* 路由器指针 */;
  basic_repeater<SharedInfoPtr, MyRouter> repeater(psinfo, router_ptr);

  // 注册/注销某个 repeater 容器
  repeater.register_repeater_data(my_cont);
  repeater.unregister_repeater_data(my_cont);

参考文件
- 原始实现： [basic_repeater.hpp](basic_repeater.hpp)
