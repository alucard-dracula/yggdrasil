# proxy_io_converter.hpp — 中文文档

概览
- `proxy_io_converter` 是一个简单的模板适配器，用于在代理/适配器场景中，把任务类型与发送/接收的数据类型进行转换（双向）。
- 它假定发送端的数据类型与任务类型相同（由静态断言保证），并提供两个静态成员函数用于转换：`conv_to_send`（将数据转换并发送）和 `conv_from_recv`（从接收到的任务构造接收数据）。

模板参数
- `Send_Data`：发送数据类型（`send_data_type`）。
- `Task`：任务类型（`task_type`）。
- `Recv_Data`：接收数据类型（`recv_data_type`）。

类型别名
- `send_data_type`、`task_type`、`recv_data_type`：分别对应模板参数。
- `conv_to_send_result_type`：`bool`，`conv_to_send` 的返回类型。
- `conv_to_send_arg_data_type`：`send_data_type&`，传入 `conv_to_send` 的数据类型。
- `conv_from_recv_result_type`：`recv_data_type`，`conv_from_recv` 的返回类型。
- `conv_from_recv_arg_data_type`：`const task_type&`，传入 `conv_from_recv` 的参数类型。

设计约束
- 头文件中使用 `BOOST_MPL_ASSERT` 强制要求 `Send_Data` 与 `Task` 类型相同：

  BOOST_MPL_ASSERT((boost::is_same<Send_Data, Task>));

 这意味着发送的数据类型与任务类型在此适配器中必须完全一致。

成员函数
- `template<typename Real_Data_Type, typename Src> static conv_to_send_result_type conv_to_send(Src& src, conv_to_send_arg_data_type data)`
  - 要求 `Real_Data_Type` 等同于 `task_type`（由静态断言保证）。
  - 实现：调用 `src.template send<yggr::task_center::type_traits::mark_saver_condition>(data);` 将数据发送出去，并返回 `true`。

- `template<typename Real_Data_Type> static conv_from_recv_result_type conv_from_recv(conv_from_recv_arg_data_type tk)`
  - 要求 `Real_Data_Type` 等同于 `task_type`。
  - 实现：通过 `boost::move`（实际为 `yggr::move::move`）将 `task_type` 中的数据移动构造为 `recv_data_type` 并返回。

示例用法（伪代码）
- 发送场景：

  SrcImpl src;
  proxy_io_converter<MyTask, MyTask, MyRecv> ::conv_to_send<MyTask>(src, send_data);

  这里 `send_data` 的类型为 `MyTask`（等同于 `Send_Data` / `Task`）。

- 接收场景：

  const MyTask& tk = /* 从某处收到的任务 */;
  MyRecv recv = proxy_io_converter<MyTask, MyTask, MyRecv>::conv_from_recv<MyTask>(tk);

依赖与包含
- 头文件中使用了：
  - `yggr/move/move.hpp`（移动语义）
  - `yggr/mplex/static_assert.hpp`（静态断言封装，实际上使用 `BOOST_MPL_ASSERT`）
  - `yggr/task_center/type_traits/tags.hpp`（用于 `mark_saver_condition` 标记）
  - `boost/type_traits/is_same.hpp`

注意事项
- 本适配器使用 `const_cast` 在 `conv_from_recv` 中去掉 `const`，然后进行移动构造：

  task_type& ref_task = const_cast<task_type&>(tk);
  return recv_data_type(boost::move(ref_task));

  这要求调用方保证在传入 `conv_from_recv` 时，对象生命周期与访问安全性无问题；否则会引发未定义行为。谨慎在多线程或共享常量对象上使用。

- 由于模板中要求 `Send_Data` 与 `Task` 相同，该适配器只适用于两者一致的场景；若实际项目中两者不同，需要实现不同的转换器。

版权与许可
- 源文件头部包含 yggdrasil 项目的许可声明（MIT-like），生成的文档仅为代码说明，并保留源文件原始版权声明。

参考文件
- 原始实现： [proxy_io_converter.hpp](proxy_io_converter.hpp)
