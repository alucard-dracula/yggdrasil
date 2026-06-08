# proxy_special_data_filter.hpp — 中文文档

概览
- `proxy_special_data_filter` 提供对特殊数据项（由 `ProxyData::data_info_type` 标识）的线程安全过滤集合，用于控制哪些数据需要注册/处理。
- 类型为非拷贝非移动（不可复制、不可移动）的轻量封装，内部使用 `safe_set` 提供并发访问安全性。

模板参数
- `ProxyData`：代理数据类型，需定义 `data_info_type` 与 `data_info_parser_type`。
- `Mutex`：用于 `safe_set` 的互斥策略类型，默认 `void`（`safe_set` 的默认互斥策略）。

主要类型别名
- `proxy_data_type`：模板参数 `ProxyData`。
- `data_info_type`：`proxy_data_type::data_info_type`。
- `data_info_parser_type`：用于从类型或值解析 `data_info_type` 的解析器类型（来自 `ProxyData`）。
- `safe_data_info_set_type`：内部使用的线程安全集合类型 `safe_set<data_info_type, Mutex>`。

主要方法
- `template<typename T> bool append()`：根据类型 `T` 的静态 `data_info`（`data_info_parser_type::data_info<T>()`）插入过滤表。
- `bool append(const data_info_type& info)`：按 `data_info` 值插入过滤表。
- `template<typename T> bool need_filter() const`：检查类型 `T` 的 `data_info` 是否在过滤表中。
- `bool need_filter(const data_info_type& info) const`：按值检查是否需要过滤。
- `template<typename T> bool remove()`：按类型移除过滤项。
- `bool remove(const data_info_type& info)`：按值移除过滤项。
- `void clear()`：清空过滤表。

实现细节
- 所有操作基于 `_filter_table`（`safe_set`）完成，因此在并发环境下具有线程安全性。
- `append/remove` 返回布尔值，表示插入或删除是否成功（`remove` 使用 `!!` 将返回值转换为 `bool`）。
- `data_info_parser_type` 必须提供 `template data_info<T>()` 静态方法，用于从类型推导出对应的 `data_info_type`。

依赖
- `yggr/nonable/nonable.hpp`：用于使类不可复制不可移动。
- `yggr/safe_container/safe_set.hpp`：线程安全集合实现。

注意事项
- 该过滤器仅保存 `data_info_type`，不会保存与数据处理相关的处理函数或状态；它的作用是为上层组件（如 `proxy_repeater`）提供快速判断哪些数据属于“特殊”并需要单独注册或处理。
- 确保 `ProxyData` 的 `data_info_parser_type` 能正确解析类型到 `data_info_type`。

示例（伪代码）
- `filter.append<MyData>();`
- `if(filter.need_filter(some_data_info)) { /* 特殊处理 */ }`

参考
- 源文件： `proxy_special_data_filter.hpp`
