
require "yggr_lua_base_type"

local function test_s64_list_val_or_ref(param)
	print("test_list show", type(param), yggr.lua.typeid(param), param)

	print("\n")
	print("test_list construct", type(yggr.lua.s64_list_type()), yggr.lua.typeid(yggr.lua.s64_list_type()), yggr.lua.s64_list_type())
	print("test_list construct",
			type(yggr.lua.s64_list_type(yggr.lua.s64_allocator_type())),
			yggr.lua.typeid(yggr.lua.s64_list_type(yggr.lua.s64_allocator_type())),
			yggr.lua.s64_list_type(yggr.lua.s64_allocator_type()))

	print("test_list construct",
			type(yggr.lua.s64_list_type(5, 10)),
			yggr.lua.typeid(yggr.lua.s64_list_type(5, 10)),
			yggr.lua.s64_list_type(5, 10))
	print("test_list construct",
			type(yggr.lua.s64_list_type(yggr.lua.s64(5), 10)),
			yggr.lua.typeid(yggr.lua.s64_list_type(yggr.lua.s64(5), 10)),
			yggr.lua.s64_list_type(yggr.lua.s64(5), 10))
	print("test_list construct",
			type(yggr.lua.s64_list_type(yggr.lua.u64(5), 10)),
			yggr.lua.typeid(yggr.lua.s64_list_type(yggr.lua.u64(5), 10)),
			yggr.lua.s64_list_type(yggr.lua.u64(5), 10))

	print("test_list construct",
			type(yggr.lua.s64_list_type(5, 10, yggr.lua.s64_allocator_type())),
			yggr.lua.typeid(yggr.lua.s64_list_type(5, 10, yggr.lua.s64_allocator_type())),
			yggr.lua.s64_list_type(5, 10, yggr.lua.s64_allocator_type()))
	print("test_list construct",
			type(yggr.lua.s64_list_type(yggr.lua.s64(5), 10, yggr.lua.s64_allocator_type())),
			yggr.lua.typeid(yggr.lua.s64_list_type(yggr.lua.s64(5), 10, yggr.lua.s64_allocator_type())),
			yggr.lua.s64_list_type(yggr.lua.s64(5), 10, yggr.lua.s64_allocator_type()))
	print("test_list construct",
			type(yggr.lua.s64_list_type(yggr.lua.u64(5), 10, yggr.lua.s64_allocator_type())),
			yggr.lua.typeid(yggr.lua.s64_list_type(yggr.lua.u64(5), 10, yggr.lua.s64_allocator_type())),
			yggr.lua.s64_list_type(yggr.lua.u64(5), 10, yggr.lua.s64_allocator_type()))

	print("test_list construct",
			type(yggr.lua.s64_list_type(5)),
			yggr.lua.typeid(yggr.lua.s64_list_type(5)),
			yggr.lua.s64_list_type(5))
	print("test_list construct",
			type(yggr.lua.s64_list_type(yggr.lua.s64(5))),
			yggr.lua.typeid(yggr.lua.s64_list_type(yggr.lua.s64(5))),
			yggr.lua.s64_list_type(yggr.lua.s64(5)))
	print("test_list construct",
			type(yggr.lua.s64_list_type(yggr.lua.u64(5))),
			yggr.lua.typeid(yggr.lua.s64_list_type(yggr.lua.u64(5))),
			yggr.lua.s64_list_type(yggr.lua.u64(5)))

	print("test_list construct",
			type(yggr.lua.s64_list_type(5, yggr.lua.s64_allocator_type())),
			yggr.lua.typeid(yggr.lua.s64_list_type(5, yggr.lua.s64_allocator_type())),
			yggr.lua.s64_list_type(5, yggr.lua.s64_allocator_type()))
	print("test_list construct",
			type(yggr.lua.s64_list_type(yggr.lua.s64(5), yggr.lua.s64_allocator_type())),
			yggr.lua.typeid(yggr.lua.s64_list_type(yggr.lua.s64(5), yggr.lua.s64_allocator_type())),
			yggr.lua.s64_list_type(yggr.lua.s64(5), yggr.lua.s64_allocator_type()))
	print("test_list construct",
			type(yggr.lua.s64_list_type(yggr.lua.u64(5), yggr.lua.s64_allocator_type())),
			yggr.lua.typeid(yggr.lua.s64_list_type(yggr.lua.u64(5), yggr.lua.s64_allocator_type())),
			yggr.lua.s64_list_type(yggr.lua.u64(5), yggr.lua.s64_allocator_type()))

	print("test_list construct",
			type(yggr.lua.s64_list_type(param:begin_(), param:end_())),
			yggr.lua.typeid(yggr.lua.s64_list_type(param:begin_(), param:end_())),
			yggr.lua.s64_list_type(param:begin_(), param:end_()))
	print("test_list construct",
			type(yggr.lua.s64_list_type(param:cbegin_(), param:cend_())),
			yggr.lua.typeid(yggr.lua.s64_list_type(param:cbegin_(), param:cend_())),
			yggr.lua.s64_list_type(param:cbegin_(), param:cend_()))
	print("test_list construct",
			type(yggr.lua.s64_list_type(param:rbegin_(), param:rend_())),
			yggr.lua.typeid(yggr.lua.s64_list_type(param:rbegin_(), param:rend_())),
			yggr.lua.s64_list_type(param:rbegin_(), param:rend_()))
	print("test_list construct",
			type(yggr.lua.s64_list_type(param:crbegin_(), param:crend_())),
			yggr.lua.typeid(yggr.lua.s64_list_type(param:crbegin_(), param:crend_())),
			yggr.lua.s64_list_type(param:crbegin_(), param:crend_()))

	print("test_list construct",
			type(yggr.lua.s64_list_type(param:begin_(), param:end_(), yggr.lua.s64_allocator_type())),
			yggr.lua.typeid(yggr.lua.s64_list_type(param:begin_(), param:end_(), yggr.lua.s64_allocator_type())),
			yggr.lua.s64_list_type(param:begin_(), param:end_(), yggr.lua.s64_allocator_type()))
	print("test_list construct",
			type(yggr.lua.s64_list_type(param:cbegin_(), param:cend_(), yggr.lua.s64_allocator_type())),
			yggr.lua.typeid(yggr.lua.s64_list_type(param:cbegin_(), param:cend_(), yggr.lua.s64_allocator_type())),
			yggr.lua.s64_list_type(param:cbegin_(), param:cend_(), yggr.lua.s64_allocator_type()))
	print("test_list construct",
			type(yggr.lua.s64_list_type(param:rbegin_(), param:rend_(), yggr.lua.s64_allocator_type())),
			yggr.lua.typeid(yggr.lua.s64_list_type(param:rbegin_(), param:rend_(), yggr.lua.s64_allocator_type())),
			yggr.lua.s64_list_type(param:rbegin_(), param:rend_(), yggr.lua.s64_allocator_type()))
	print("test_list construct",
			type(yggr.lua.s64_list_type(param:crbegin_(), param:crend_(), yggr.lua.s64_allocator_type())),
			yggr.lua.typeid(yggr.lua.s64_list_type(param:crbegin_(), param:crend_(), yggr.lua.s64_allocator_type())),
			yggr.lua.s64_list_type(param:crbegin_(), param:crend_(), yggr.lua.s64_allocator_type()))

	print("test_list construct",
			type(yggr.lua.s64_list_type(param)),
			yggr.lua.typeid(yggr.lua.s64_list_type(param)),
			yggr.lua.s64_list_type(param))

	print("test_list construct",
			type(yggr.lua.s64_list_type(param, yggr.lua.s64_allocator_type())),
			yggr.lua.typeid(yggr.lua.s64_list_type(param, yggr.lua.s64_allocator_type())),
			yggr.lua.s64_list_type(param, yggr.lua.s64_allocator_type()))

	print("test_list construct_u64_lst",
			type(yggr.lua.u64_list_type(param:begin_(), param:end_())),
			yggr.lua.typeid(yggr.lua.u64_list_type(param:begin_(), param:end_())),
			yggr.lua.u64_list_type(param:begin_(), param:end_()))
	print("test_list construct_u64_lst",
			type(yggr.lua.u64_list_type(param:cbegin_(), param:cend_())),
			yggr.lua.typeid(yggr.lua.u64_list_type(param:cbegin_(), param:cend_())),
			yggr.lua.u64_list_type(param:cbegin_(), param:cend_()))
	print("test_list construct_u64_lst",
			type(yggr.lua.u64_list_type(param:rbegin_(), param:rend_())),
			yggr.lua.typeid(yggr.lua.u64_list_type(param:rbegin_(), param:rend_())),
			yggr.lua.u64_list_type(param:rbegin_(), param:rend_()))
	print("test_list construct_u64_lst",
			type(yggr.lua.u64_list_type(param:crbegin_(), param:crend_())),
			yggr.lua.typeid(yggr.lua.u64_list_type(param:crbegin_(), param:crend_())),
			yggr.lua.u64_list_type(param:crbegin_(), param:crend_()))

	print("test_list construct_u64_lst",
			type(yggr.lua.u64_list_type(param:begin_(), param:end_(), yggr.lua.u64_allocator_type())),
			yggr.lua.typeid(yggr.lua.u64_list_type(param:begin_(), param:end_(), yggr.lua.u64_allocator_type())),
			yggr.lua.u64_list_type(param:begin_(), param:end_(), yggr.lua.u64_allocator_type()))
	print("test_list construct_u64_lst",
			type(yggr.lua.u64_list_type(param:cbegin_(), param:cend_(), yggr.lua.u64_allocator_type())),
			yggr.lua.typeid(yggr.lua.u64_list_type(param:cbegin_(), param:cend_(), yggr.lua.u64_allocator_type())),
			yggr.lua.u64_list_type(param:cbegin_(), param:cend_(), yggr.lua.u64_allocator_type()))
	print("test_list construct_u64_lst",
			type(yggr.lua.u64_list_type(param:rbegin_(), param:rend_(), yggr.lua.u64_allocator_type())),
			yggr.lua.typeid(yggr.lua.u64_list_type(param:rbegin_(), param:rend_(), yggr.lua.u64_allocator_type())),
			yggr.lua.u64_list_type(param:rbegin_(), param:rend_(), yggr.lua.u64_allocator_type()))
	print("test_list construct_u64_lst",
			type(yggr.lua.u64_list_type(param:crbegin_(), param:crend_(), yggr.lua.u64_allocator_type())),
			yggr.lua.typeid(yggr.lua.u64_list_type(param:crbegin_(), param:crend_(), yggr.lua.u64_allocator_type())),
			yggr.lua.u64_list_type(param:crbegin_(), param:crend_(), yggr.lua.u64_allocator_type()))

	print("test_list construct_s64_vt",
			type(yggr.lua.s64_vector_type(param:begin_(), param:end_())),
			yggr.lua.typeid(yggr.lua.s64_vector_type(param:begin_(), param:end_())),
			yggr.lua.s64_vector_type(param:begin_(), param:end_()))
	print("test_list construct_s64_vt",
			type(yggr.lua.s64_vector_type(param:cbegin_(), param:cend_())),
			yggr.lua.typeid(yggr.lua.s64_vector_type(param:cbegin_(), param:cend_())),
			yggr.lua.s64_vector_type(param:cbegin_(), param:cend_()))
	print("test_list construct_s64_vt",
			type(yggr.lua.s64_vector_type(param:rbegin_(), param:rend_())),
			yggr.lua.typeid(yggr.lua.s64_vector_type(param:rbegin_(), param:rend_())),
			yggr.lua.s64_vector_type(param:rbegin_(), param:rend_()))
	print("test_list construct_s64_vt",
			type(yggr.lua.s64_vector_type(param:crbegin_(), param:crend_())),
			yggr.lua.typeid(yggr.lua.s64_vector_type(param:crbegin_(), param:crend_())),
			yggr.lua.s64_vector_type(param:crbegin_(), param:crend_()))

	print("test_list construct_s64_vt",
			type(yggr.lua.s64_vector_type(param:begin_(), param:end_(), yggr.lua.s64_allocator_type())),
			yggr.lua.typeid(yggr.lua.s64_vector_type(param:begin_(), param:end_(), yggr.lua.s64_allocator_type())),
			yggr.lua.s64_vector_type(param:begin_(), param:end_(), yggr.lua.s64_allocator_type()))
	print("test_list construct_s64_vt",
			type(yggr.lua.s64_vector_type(param:cbegin_(), param:cend_(), yggr.lua.s64_allocator_type())),
			yggr.lua.typeid(yggr.lua.s64_vector_type(param:cbegin_(), param:cend_(), yggr.lua.s64_allocator_type())),
			yggr.lua.s64_vector_type(param:cbegin_(), param:cend_(), yggr.lua.s64_allocator_type()))
	print("test_list construct_s64_vt",
			type(yggr.lua.s64_vector_type(param:rbegin_(), param:rend_(), yggr.lua.s64_allocator_type())),
			yggr.lua.typeid(yggr.lua.s64_vector_type(param:rbegin_(), param:rend_(), yggr.lua.s64_allocator_type())),
			yggr.lua.s64_vector_type(param:rbegin_(), param:rend_(), yggr.lua.s64_allocator_type()))
	print("test_list construct_s64_vt",
			type(yggr.lua.s64_vector_type(param:crbegin_(), param:crend_(), yggr.lua.s64_allocator_type())),
			yggr.lua.typeid(yggr.lua.s64_vector_type(param:crbegin_(), param:crend_(), yggr.lua.s64_allocator_type())),
			yggr.lua.s64_vector_type(param:crbegin_(), param:crend_(), yggr.lua.s64_allocator_type()))

	print("test_list construct_u64_vt",
			type(yggr.lua.u64_vector_type(param:begin_(), param:end_())),
			yggr.lua.typeid(yggr.lua.u64_vector_type(param:begin_(), param:end_())),
			yggr.lua.u64_vector_type(param:begin_(), param:end_()))
	print("test_list construct_u64_vt",
			type(yggr.lua.u64_vector_type(param:cbegin_(), param:cend_())),
			yggr.lua.typeid(yggr.lua.u64_vector_type(param:cbegin_(), param:cend_())),
			yggr.lua.u64_deque_type(param:cbegin_(), param:cend_()))
	print("test_list construct_u64_vt",
			type(yggr.lua.u64_vector_type(param:rbegin_(), param:rend_())),
			yggr.lua.typeid(yggr.lua.u64_vector_type(param:rbegin_(), param:rend_())),
			yggr.lua.u64_vector_type(param:rbegin_(), param:rend_()))
	print("test_list construct_u64_vt",
			type(yggr.lua.u64_vector_type(param:crbegin_(), param:crend_())),
			yggr.lua.typeid(yggr.lua.u64_vector_type(param:crbegin_(), param:crend_())),
			yggr.lua.u64_vector_type(param:crbegin_(), param:crend_()))

	print("test_list construct_u64_vt",
			type(yggr.lua.u64_vector_type(param:begin_(), param:end_(), yggr.lua.u64_allocator_type())),
			yggr.lua.typeid(yggr.lua.u64_vector_type(param:begin_(), param:end_(), yggr.lua.u64_allocator_type())),
			yggr.lua.u64_vector_type(param:begin_(), param:end_(), yggr.lua.u64_allocator_type()))
	print("test_list construct_u64_vt",
			type(yggr.lua.u64_vector_type(param:cbegin_(), param:cend_(), yggr.lua.u64_allocator_type())),
			yggr.lua.typeid(yggr.lua.u64_vector_type(param:cbegin_(), param:cend_(), yggr.lua.u64_allocator_type())),
			yggr.lua.u64_vector_type(param:cbegin_(), param:cend_(), yggr.lua.u64_allocator_type()))
	print("test_list construct_u64_vt",
			type(yggr.lua.u64_vector_type(param:rbegin_(), param:rend_(), yggr.lua.u64_allocator_type())),
			yggr.lua.typeid(yggr.lua.u64_vector_type(param:rbegin_(), param:rend_(), yggr.lua.u64_allocator_type())),
			yggr.lua.u64_vector_type(param:rbegin_(), param:rend_(), yggr.lua.u64_allocator_type()))
	print("test_list construct_u64_vt",
			type(yggr.lua.u64_vector_type(param:crbegin_(), param:crend_(), yggr.lua.u64_allocator_type())),
			yggr.lua.typeid(yggr.lua.u64_vector_type(param:crbegin_(), param:crend_(), yggr.lua.u64_allocator_type())),
			yggr.lua.u64_vector_type(param:crbegin_(), param:crend_(), yggr.lua.u64_allocator_type()))

	print("test_list construct_s64_dq",
			type(yggr.lua.s64_deque_type(param:begin_(), param:end_())),
			yggr.lua.typeid(yggr.lua.s64_deque_type(param:begin_(), param:end_())),
			yggr.lua.s64_deque_type(param:begin_(), param:end_()))
	print("test_list construct_s64_dq",
			type(yggr.lua.s64_deque_type(param:cbegin_(), param:cend_())),
			yggr.lua.typeid(yggr.lua.s64_deque_type(param:cbegin_(), param:cend_())),
			yggr.lua.s64_deque_type(param:cbegin_(), param:cend_()))
	print("test_list construct_s64_dq",
			type(yggr.lua.s64_deque_type(param:rbegin_(), param:rend_())),
			yggr.lua.typeid(yggr.lua.s64_deque_type(param:rbegin_(), param:rend_())),
			yggr.lua.s64_deque_type(param:rbegin_(), param:rend_()))
	print("test_list construct_s64_dq",
			type(yggr.lua.s64_deque_type(param:crbegin_(), param:crend_())),
			yggr.lua.typeid(yggr.lua.s64_deque_type(param:crbegin_(), param:crend_())),
			yggr.lua.s64_deque_type(param:crbegin_(), param:crend_()))

	print("test_list construct_s64_dq",
			type(yggr.lua.s64_deque_type(param:begin_(), param:end_(), yggr.lua.s64_allocator_type())),
			yggr.lua.typeid(yggr.lua.s64_deque_type(param:begin_(), param:end_(), yggr.lua.s64_allocator_type())),
			yggr.lua.s64_deque_type(param:begin_(), param:end_(), yggr.lua.s64_allocator_type()))
	print("test_list construct_s64_dq",
			type(yggr.lua.s64_deque_type(param:cbegin_(), param:cend_(), yggr.lua.s64_allocator_type())),
			yggr.lua.typeid(yggr.lua.s64_deque_type(param:cbegin_(), param:cend_(), yggr.lua.s64_allocator_type())),
			yggr.lua.s64_deque_type(param:cbegin_(), param:cend_(), yggr.lua.s64_allocator_type()))
	print("test_list construct_s64_dq",
			type(yggr.lua.s64_deque_type(param:rbegin_(), param:rend_(), yggr.lua.s64_allocator_type())),
			yggr.lua.typeid(yggr.lua.s64_deque_type(param:rbegin_(), param:rend_(), yggr.lua.s64_allocator_type())),
			yggr.lua.s64_deque_type(param:rbegin_(), param:rend_(), yggr.lua.s64_allocator_type()))
	print("test_list construct_s64_dq",
			type(yggr.lua.s64_deque_type(param:crbegin_(), param:crend_(), yggr.lua.s64_allocator_type())),
			yggr.lua.typeid(yggr.lua.s64_deque_type(param:crbegin_(), param:crend_(), yggr.lua.s64_allocator_type())),
			yggr.lua.s64_deque_type(param:crbegin_(), param:crend_(), yggr.lua.s64_allocator_type()))

	print("test_list construct_u64_dq",
			type(yggr.lua.u64_deque_type(param:begin_(), param:end_())),
			yggr.lua.typeid(yggr.lua.u64_deque_type(param:begin_(), param:end_())),
			yggr.lua.u64_deque_type(param:begin_(), param:end_()))
	print("test_list construct_u64_dq",
			type(yggr.lua.u64_deque_type(param:cbegin_(), param:cend_())),
			yggr.lua.typeid(yggr.lua.u64_deque_type(param:cbegin_(), param:cend_())),
			yggr.lua.u64_deque_type(param:cbegin_(), param:cend_()))
	print("test_list construct_u64_dq",
			type(yggr.lua.u64_deque_type(param:rbegin_(), param:rend_())),
			yggr.lua.typeid(yggr.lua.u64_deque_type(param:rbegin_(), param:rend_())),
			yggr.lua.u64_deque_type(param:rbegin_(), param:rend_()))
	print("test_list construct_u64_dq",
			type(yggr.lua.u64_deque_type(param:crbegin_(), param:crend_())),
			yggr.lua.typeid(yggr.lua.u64_deque_type(param:crbegin_(), param:crend_())),
			yggr.lua.u64_deque_type(param:crbegin_(), param:crend_()))

	print("test_list construct_u64_dq",
			type(yggr.lua.u64_deque_type(param:begin_(), param:end_(), yggr.lua.u64_allocator_type())),
			yggr.lua.typeid(yggr.lua.u64_deque_type(param:begin_(), param:end_(), yggr.lua.u64_allocator_type())),
			yggr.lua.u64_deque_type(param:begin_(), param:end_(), yggr.lua.u64_allocator_type()))
	print("test_list construct_u64_dq",
			type(yggr.lua.u64_deque_type(param:cbegin_(), param:cend_(), yggr.lua.u64_allocator_type())),
			yggr.lua.typeid(yggr.lua.u64_deque_type(param:cbegin_(), param:cend_(), yggr.lua.u64_allocator_type())),
			yggr.lua.u64_deque_type(param:cbegin_(), param:cend_(), yggr.lua.u64_allocator_type()))
	print("test_list construct_u64_dq",
			type(yggr.lua.u64_deque_type(param:rbegin_(), param:rend_(), yggr.lua.u64_allocator_type())),
			yggr.lua.typeid(yggr.lua.u64_deque_type(param:rbegin_(), param:rend_(), yggr.lua.u64_allocator_type())),
			yggr.lua.u64_deque_type(param:rbegin_(), param:rend_(), yggr.lua.u64_allocator_type()))
	print("test_list construct_u64_dq",
			type(yggr.lua.u64_deque_type(param:crbegin_(), param:crend_(), yggr.lua.u64_allocator_type())),
			yggr.lua.typeid(yggr.lua.u64_deque_type(param:crbegin_(), param:crend_(), yggr.lua.u64_allocator_type())),
			yggr.lua.u64_deque_type(param:crbegin_(), param:crend_(), yggr.lua.u64_allocator_type()))

	print("test_list construct_s32_vt",
			type(yggr.lua.s32_vector_type(param:begin_(), param:end_())),
			yggr.lua.typeid(yggr.lua.s32_vector_type(param:begin_(), param:end_())),
			yggr.lua.s32_vector_type(param:begin_(), param:end_()))
	print("test_list construct_s32_vt",
			type(yggr.lua.s32_vector_type(param:cbegin_(), param:cend_())),
			yggr.lua.typeid(yggr.lua.s32_vector_type(param:cbegin_(), param:cend_())),
			yggr.lua.s32_vector_type(param:cbegin_(), param:cend_()))
	print("test_list construct_s32_vt",
			type(yggr.lua.s32_vector_type(param:rbegin_(), param:rend_())),
			yggr.lua.typeid(yggr.lua.s32_vector_type(param:rbegin_(), param:rend_())),
			yggr.lua.s32_vector_type(param:rbegin_(), param:rend_()))
	print("test_list construct_s32_vt",
			type(yggr.lua.s32_vector_type(param:crbegin_(), param:crend_())),
			yggr.lua.typeid(yggr.lua.s32_vector_type(param:crbegin_(), param:crend_())),
			yggr.lua.s32_vector_type(param:crbegin_(), param:crend_()))


	print("test_list construct_s32_vt",
			type(yggr.lua.s32_vector_type(param:begin_(), param:end_(), yggr.lua.s32_allocator_type())),
			yggr.lua.typeid(yggr.lua.s32_vector_type(param:begin_(), param:end_(), yggr.lua.s32_allocator_type())),
			yggr.lua.s32_vector_type(param:begin_(), param:end_(), yggr.lua.s32_allocator_type()))
	print("test_list construct_s32_vt",
			type(yggr.lua.s32_vector_type(param:cbegin_(), param:cend_(), yggr.lua.s32_allocator_type())),
			yggr.lua.typeid(yggr.lua.s32_vector_type(param:cbegin_(), param:cend_(), yggr.lua.s32_allocator_type())),
			yggr.lua.s32_vector_type(param:cbegin_(), param:cend_(), yggr.lua.s32_allocator_type()))
	print("test_list construct_s32_vt",
			type(yggr.lua.s32_vector_type(param:rbegin_(), param:rend_(), yggr.lua.s32_allocator_type())),
			yggr.lua.typeid(yggr.lua.s32_vector_type(param:rbegin_(), param:rend_(), yggr.lua.s32_allocator_type())),
			yggr.lua.s32_vector_type(param:rbegin_(), param:rend_(), yggr.lua.s32_allocator_type()))
	print("test_list construct_s32_vt",
			type(yggr.lua.s32_vector_type(param:crbegin_(), param:crend_(), yggr.lua.s32_allocator_type())),
			yggr.lua.typeid(yggr.lua.s32_vector_type(param:crbegin_(), param:crend_(), yggr.lua.s32_allocator_type())),
			yggr.lua.s32_vector_type(param:crbegin_(), param:crend_(), yggr.lua.s32_allocator_type()))

	print("test_list construct_u32_vt",
			type(yggr.lua.u32_vector_type(param:begin_(), param:end_())),
			yggr.lua.typeid(yggr.lua.u32_vector_type(param:begin_(), param:end_())),
			yggr.lua.u32_vector_type(param:begin_(), param:end_()))
	print("test_list construct_u32_vt",
			type(yggr.lua.u32_vector_type(param:cbegin_(), param:cend_())),
			yggr.lua.typeid(yggr.lua.u32_vector_type(param:cbegin_(), param:cend_())),
			yggr.lua.u32_vector_type(param:cbegin_(), param:cend_()))
	print("test_list construct_u32_vt",
			type(yggr.lua.u32_vector_type(param:rbegin_(), param:rend_())),
			yggr.lua.typeid(yggr.lua.u32_vector_type(param:rbegin_(), param:rend_())),
			yggr.lua.u32_vector_type(param:rbegin_(), param:rend_()))
	print("test_list construct_u32_vt",
			type(yggr.lua.u32_vector_type(param:crbegin_(), param:crend_())),
			yggr.lua.typeid(yggr.lua.u32_vector_type(param:crbegin_(), param:crend_())),
			yggr.lua.u32_vector_type(param:crbegin_(), param:crend_()))


	print("test_list construct_u32_vt",
			type(yggr.lua.u32_vector_type(param:begin_(), param:end_(), yggr.lua.u32_allocator_type())),
			yggr.lua.typeid(yggr.lua.u32_vector_type(param:begin_(), param:end_(), yggr.lua.u32_allocator_type())),
			yggr.lua.u32_vector_type(param:begin_(), param:end_(), yggr.lua.u32_allocator_type()))
	print("test_list construct_u32_vt",
			type(yggr.lua.u32_vector_type(param:cbegin_(), param:cend_(), yggr.lua.u32_allocator_type())),
			yggr.lua.typeid(yggr.lua.u32_vector_type(param:cbegin_(), param:cend_(), yggr.lua.u32_allocator_type())),
			yggr.lua.u32_vector_type(param:cbegin_(), param:cend_(), yggr.lua.u32_allocator_type()))
	print("test_list construct_u32_vt",
			type(yggr.lua.u32_vector_type(param:rbegin_(), param:rend_(), yggr.lua.u32_allocator_type())),
			yggr.lua.typeid(yggr.lua.u32_vector_type(param:rbegin_(), param:rend_(), yggr.lua.u32_allocator_type())),
			yggr.lua.u32_vector_type(param:rbegin_(), param:rend_(), yggr.lua.u32_allocator_type()))
	print("test_list construct_u32_vt",
			type(yggr.lua.u32_vector_type(param:crbegin_(), param:crend_(), yggr.lua.u32_allocator_type())),
			yggr.lua.typeid(yggr.lua.u32_vector_type(param:crbegin_(), param:crend_(), yggr.lua.u32_allocator_type())),
			yggr.lua.u32_vector_type(param:crbegin_(), param:crend_(), yggr.lua.u32_allocator_type()))

	print("test_list construct_s32_dq",
			type(yggr.lua.s32_deque_type(param:begin_(), param:end_())),
			yggr.lua.typeid(yggr.lua.s32_deque_type(param:begin_(), param:end_())),
			yggr.lua.s32_deque_type(param:begin_(), param:end_()))
	print("test_list construct_s32_dq",
			type(yggr.lua.s32_deque_type(param:cbegin_(), param:cend_())),
			yggr.lua.typeid(yggr.lua.s32_deque_type(param:cbegin_(), param:cend_())),
			yggr.lua.s32_deque_type(param:cbegin_(), param:cend_()))
	print("test_list construct_s32_dq",
			type(yggr.lua.s32_deque_type(param:rbegin_(), param:rend_())),
			yggr.lua.typeid(yggr.lua.s32_deque_type(param:rbegin_(), param:rend_())),
			yggr.lua.s32_deque_type(param:rbegin_(), param:rend_()))
	print("test_list construct_s32_dq",
			type(yggr.lua.s32_deque_type(param:crbegin_(), param:crend_())),
			yggr.lua.typeid(yggr.lua.s32_deque_type(param:crbegin_(), param:crend_())),
			yggr.lua.s32_deque_type(param:crbegin_(), param:crend_()))

	print("test_list construct_s32_dq",
			type(yggr.lua.s32_deque_type(param:begin_(), param:end_(), yggr.lua.s32_allocator_type())),
			yggr.lua.typeid(yggr.lua.s32_deque_type(param:begin_(), param:end_(), yggr.lua.s32_allocator_type())),
			yggr.lua.s32_deque_type(param:begin_(), param:end_(), yggr.lua.s32_allocator_type()))
	print("test_list construct_s32_dq",
			type(yggr.lua.s32_deque_type(param:cbegin_(), param:cend_(), yggr.lua.s32_allocator_type())),
			yggr.lua.typeid(yggr.lua.s32_deque_type(param:cbegin_(), param:cend_(), yggr.lua.s32_allocator_type())),
			yggr.lua.s32_deque_type(param:cbegin_(), param:cend_(), yggr.lua.s32_allocator_type()))
	print("test_list construct_s32_dq",
			type(yggr.lua.s32_deque_type(param:rbegin_(), param:rend_(), yggr.lua.s32_allocator_type())),
			yggr.lua.typeid(yggr.lua.s32_deque_type(param:rbegin_(), param:rend_(), yggr.lua.s32_allocator_type())),
			yggr.lua.s32_deque_type(param:rbegin_(), param:rend_(), yggr.lua.s32_allocator_type()))
	print("test_list construct_s32_dq",
			type(yggr.lua.s32_deque_type(param:crbegin_(), param:crend_(), yggr.lua.s32_allocator_type())),
			yggr.lua.typeid(yggr.lua.s32_deque_type(param:crbegin_(), param:crend_(), yggr.lua.s32_allocator_type())),
			yggr.lua.s32_deque_type(param:crbegin_(), param:crend_(), yggr.lua.s32_allocator_type()))

	print("test_list construct_u32_dq",
			type(yggr.lua.u32_deque_type(param:begin_(), param:end_())),
			yggr.lua.typeid(yggr.lua.u32_deque_type(param:begin_(), param:end_())),
			yggr.lua.u32_deque_type(param:begin_(), param:end_()))
	print("test_list construct_u32_dq",
			type(yggr.lua.u32_deque_type(param:cbegin_(), param:cend_())),
			yggr.lua.typeid(yggr.lua.u32_deque_type(param:cbegin_(), param:cend_())),
			yggr.lua.u32_deque_type(param:cbegin_(), param:cend_()))
	print("test_list construct_u32_dq",
			type(yggr.lua.u32_deque_type(param:rbegin_(), param:rend_())),
			yggr.lua.typeid(yggr.lua.u32_deque_type(param:rbegin_(), param:rend_())),
			yggr.lua.u32_deque_type(param:rbegin_(), param:rend_()))
	print("test_list construct_u32_dq",
			type(yggr.lua.u32_deque_type(param:crbegin_(), param:crend_())),
			yggr.lua.typeid(yggr.lua.u32_deque_type(param:crbegin_(), param:crend_())),
			yggr.lua.u32_deque_type(param:crbegin_(), param:crend_()))

	print("test_list construct_u32_dq",
			type(yggr.lua.u32_deque_type(param:begin_(), param:end_(), yggr.lua.u32_allocator_type())),
			yggr.lua.typeid(yggr.lua.u32_deque_type(param:begin_(), param:end_(), yggr.lua.u32_allocator_type())),
			yggr.lua.u32_deque_type(param:begin_(), param:end_(), yggr.lua.u32_allocator_type()))
	print("test_list construct_u32_dq",
			type(yggr.lua.u32_deque_type(param:cbegin_(), param:cend_(), yggr.lua.u32_allocator_type())),
			yggr.lua.typeid(yggr.lua.u32_deque_type(param:cbegin_(), param:cend_(), yggr.lua.u32_allocator_type())),
			yggr.lua.u32_deque_type(param:cbegin_(), param:cend_(), yggr.lua.u32_allocator_type()))
	print("test_list construct_u32_dq",
			type(yggr.lua.u32_deque_type(param:rbegin_(), param:rend_(), yggr.lua.u32_allocator_type())),
			yggr.lua.typeid(yggr.lua.u32_deque_type(param:rbegin_(), param:rend_(), yggr.lua.u32_allocator_type())),
			yggr.lua.u32_deque_type(param:rbegin_(), param:rend_(), yggr.lua.u32_allocator_type()))
	print("test_list construct_u32_dq",
			type(yggr.lua.u32_deque_type(param:crbegin_(), param:crend_(), yggr.lua.u32_allocator_type())),
			yggr.lua.typeid(yggr.lua.u32_deque_type(param:crbegin_(), param:crend_(), yggr.lua.u32_allocator_type())),
			yggr.lua.u32_deque_type(param:crbegin_(), param:crend_(), yggr.lua.u32_allocator_type()))

	print("test_list construct_s32_lst",
			type(yggr.lua.s32_list_type(param:begin_(), param:end_())),
			yggr.lua.typeid(yggr.lua.s32_list_type(param:begin_(), param:end_())),
			yggr.lua.s32_list_type(param:begin_(), param:end_()))
	print("test_list construct_s32_lst",
			type(yggr.lua.s32_list_type(param:cbegin_(), param:cend_())),
			yggr.lua.typeid(yggr.lua.s32_list_type(param:cbegin_(), param:cend_())),
			yggr.lua.s32_list_type(param:cbegin_(), param:cend_()))
	print("test_list construct_s32_lst",
			type(yggr.lua.s32_list_type(param:rbegin_(), param:rend_())),
			yggr.lua.typeid(yggr.lua.s32_list_type(param:rbegin_(), param:rend_())),
			yggr.lua.s32_list_type(param:rbegin_(), param:rend_()))
	print("test_list construct_s32_lst",
			type(yggr.lua.s32_list_type(param:crbegin_(), param:crend_())),
			yggr.lua.typeid(yggr.lua.s32_list_type(param:crbegin_(), param:crend_())),
			yggr.lua.s32_list_type(param:crbegin_(), param:crend_()))

	print("test_list construct_s32_lst",
			type(yggr.lua.s32_list_type(param:begin_(), param:end_(), yggr.lua.s32_allocator_type())),
			yggr.lua.typeid(yggr.lua.s32_list_type(param:begin_(), param:end_(), yggr.lua.s32_allocator_type())),
			yggr.lua.s32_list_type(param:begin_(), param:end_(), yggr.lua.s32_allocator_type()))
	print("test_list construct_s32_lst",
			type(yggr.lua.s32_list_type(param:cbegin_(), param:cend_(), yggr.lua.s32_allocator_type())),
			yggr.lua.typeid(yggr.lua.s32_list_type(param:cbegin_(), param:cend_(), yggr.lua.s32_allocator_type())),
			yggr.lua.s32_list_type(param:cbegin_(), param:cend_(), yggr.lua.s32_allocator_type()))
	print("test_list construct_s32_lst",
			type(yggr.lua.s32_list_type(param:rbegin_(), param:rend_(), yggr.lua.s32_allocator_type())),
			yggr.lua.typeid(yggr.lua.s32_list_type(param:rbegin_(), param:rend_(), yggr.lua.s32_allocator_type())),
			yggr.lua.s32_list_type(param:rbegin_(), param:rend_(), yggr.lua.s32_allocator_type()))
	print("test_list construct_s32_lst",
			type(yggr.lua.s32_list_type(param:crbegin_(), param:crend_(), yggr.lua.s32_allocator_type())),
			yggr.lua.typeid(yggr.lua.s32_list_type(param:crbegin_(), param:crend_(), yggr.lua.s32_allocator_type())),
			yggr.lua.s32_list_type(param:crbegin_(), param:crend_(), yggr.lua.s32_allocator_type()))

	print("test_list construct_u32_lst",
			type(yggr.lua.u32_list_type(param:begin_(), param:end_())),
			yggr.lua.typeid(yggr.lua.u32_list_type(param:begin_(), param:end_())),
			yggr.lua.u32_list_type(param:begin_(), param:end_()))
	print("test_list construct_u32_lst",
			type(yggr.lua.u32_list_type(param:cbegin_(), param:cend_())),
			yggr.lua.typeid(yggr.lua.u32_list_type(param:cbegin_(), param:cend_())),
			yggr.lua.u32_list_type(param:cbegin_(), param:cend_()))
	print("test_list construct_u32_lst",
			type(yggr.lua.u32_list_type(param:rbegin_(), param:rend_())),
			yggr.lua.typeid(yggr.lua.u32_list_type(param:rbegin_(), param:rend_())),
			yggr.lua.u32_list_type(param:rbegin_(), param:rend_()))
	print("test_list construct_u32_lst",
			type(yggr.lua.u32_list_type(param:crbegin_(), param:crend_())),
			yggr.lua.typeid(yggr.lua.u32_list_type(param:crbegin_(), param:crend_())),
			yggr.lua.u32_list_type(param:crbegin_(), param:crend_()))

	print("test_list construct_u32_lst",
			type(yggr.lua.u32_list_type(param:begin_(), param:end_(), yggr.lua.u32_allocator_type())),
			yggr.lua.typeid(yggr.lua.u32_list_type(param:begin_(), param:end_(), yggr.lua.u32_allocator_type())),
			yggr.lua.u32_list_type(param:begin_(), param:end_(), yggr.lua.u32_allocator_type()))
	print("test_list construct_u32_lst",
			type(yggr.lua.u32_list_type(param:cbegin_(), param:cend_(), yggr.lua.u32_allocator_type())),
			yggr.lua.typeid(yggr.lua.u32_list_type(param:cbegin_(), param:cend_(), yggr.lua.u32_allocator_type())),
			yggr.lua.u32_list_type(param:cbegin_(), param:cend_(), yggr.lua.u32_allocator_type()))
	print("test_list construct_u32_lst",
			type(yggr.lua.u32_list_type(param:rbegin_(), param:rend_(), yggr.lua.u32_allocator_type())),
			yggr.lua.typeid(yggr.lua.u32_list_type(param:rbegin_(), param:rend_(), yggr.lua.u32_allocator_type())),
			yggr.lua.u32_list_type(param:rbegin_(), param:rend_(), yggr.lua.u32_allocator_type()))
	print("test_list construct_u32_lst",
			type(yggr.lua.u32_list_type(param:crbegin_(), param:crend_(), yggr.lua.u32_allocator_type())),
			yggr.lua.typeid(yggr.lua.u32_list_type(param:crbegin_(), param:crend_(), yggr.lua.u32_allocator_type())),
			yggr.lua.u32_list_type(param:crbegin_(), param:crend_(), yggr.lua.u32_allocator_type()))

	----[[

	print("\n")
	local tmp_swap_l = param:copy()
	local tmp_swap_r = yggr.lua.s64_list_type()
	print("test_list swap_l", type(tmp_swap_l), yggr.lua.typeid(tmp_swap_l), tmp_swap_l)
	print("test_list swap_r", type(tmp_swap_r), yggr.lua.typeid(tmp_swap_r), tmp_swap_r)
	tmp_swap_l:swap(tmp_swap_r)
	print("test_list swap_l", type(tmp_swap_l), yggr.lua.typeid(tmp_swap_l), tmp_swap_l)
	print("test_list swap_r", type(tmp_swap_r), yggr.lua.typeid(tmp_swap_r), tmp_swap_r)

	print("\n")
	print("test_list copy", type(param:copy()), yggr.lua.typeid(param:copy()), param:copy())

	print("\n")
	local tmp_assign = yggr.lua.s64_list_type()
	tmp_assign:assign(10, 1)
	print("test_list assign", type(tmp_assign), yggr.lua.typeid(tmp_assign), tmp_assign)
	tmp_assign:assign(yggr.lua.s64(10), 2)
	print("test_list assign", type(tmp_assign), yggr.lua.typeid(tmp_assign), tmp_assign)
	tmp_assign:assign(yggr.lua.u64(10), 3)
	print("test_list assign", type(tmp_assign), yggr.lua.typeid(tmp_assign), tmp_assign)

	tmp_assign:assign(param:begin_(), param:end_())
	print("test_list assign", type(tmp_assign), yggr.lua.typeid(tmp_assign), tmp_assign)
	tmp_assign:assign(param:cbegin_(), param:cend_())
	print("test_list assign", type(tmp_assign), yggr.lua.typeid(tmp_assign), tmp_assign)
	tmp_assign:assign(param:rbegin_(), param:rend_())
	print("test_list assign", type(tmp_assign), yggr.lua.typeid(tmp_assign), tmp_assign)
	tmp_assign:assign(param:crbegin_(), param:crend_())
	print("test_list assign", type(tmp_assign), yggr.lua.typeid(tmp_assign), tmp_assign)

	print("\n")
	print("test_list get_allocator", type(param:get_allocator()), yggr.lua.typeid(param:get_allocator()), param:get_allocator())

	print("\n")
	print("test_list front", type(param:front()), yggr.lua.typeid(param:front()), param:front())

	print("\n")
	print("test_list back", type(param:back()), yggr.lua.typeid(param:back()), param:back())

	print("\n")
	print("test_list begin_", type(param:begin_()), yggr.lua.typeid(param:begin_()), param:begin_())
	print("test_list cbegin_", type(param:cbegin_()), yggr.lua.typeid(param:cbegin_()), param:cbegin_())

	print("\n")
	print("test_list end_", type(param:end_()), yggr.lua.typeid(param:end_()), param:end_())
	print("test_list cend_", type(param:cend_()), yggr.lua.typeid(param:cend_()), param:cend_())

	print("\n")
	print("test_list rbegin_", type(param:rbegin_()), yggr.lua.typeid(param:rbegin_()), param:rbegin_())
	print("test_list crbegin_", type(param:crbegin_()), yggr.lua.typeid(param:crbegin_()), param:crbegin_())

	print("\n")
	print("test_list rend_", type(param:rend_()), yggr.lua.typeid(param:rend_()), param:rend_())
	print("test_list crend_", type(param:crend_()), yggr.lua.typeid(param:crend_()), param:crend_())

	print("\n")
	print("test_list g_begin_",
			type(yggr.lua.begin_(param)),
			yggr.lua.typeid(yggr.lua.begin_(param)),
			yggr.lua.begin_(param))
	print("test_list g_cbegin_",
			type(yggr.lua.cbegin_(param)),
			yggr.lua.typeid(yggr.lua.cbegin_(param)),
			yggr.lua.cbegin_(param))

	print("\n")
	print("test_list g_end_",
			type(yggr.lua.end_(param)),
			yggr.lua.typeid(yggr.lua.end_(param)),
			yggr.lua.end_(param))
	print("test_list g_cend_",
			type(yggr.lua.cend_(param)),
			yggr.lua.typeid(yggr.lua.cend_(param)),
			yggr.lua.cend_(param))

	print("\n")
	print("test_list g_rbegin_",
			type(yggr.lua.rbegin_(param)),
			yggr.lua.typeid(yggr.lua.rbegin_(param)),
			yggr.lua.rbegin_(param))
	print("test_list g_crbegin_",
			type(yggr.lua.crbegin_(param)),
			yggr.lua.typeid(yggr.lua.crbegin_(param)),
			yggr.lua.crbegin_(param))

	print("\n")
	print("test_list g_rend_",
			type(yggr.lua.rend_(param)),
			yggr.lua.typeid(yggr.lua.rend_(param)),
			yggr.lua.rend_(param))
	print("test_list g_crend_",
			type(yggr.lua.crend_(param)),
			yggr.lua.typeid(yggr.lua.crend_(param)),
			yggr.lua.crend_(param))

	print("\n")
	print("test_list empty", type(param:empty()), yggr.lua.typeid(param:empty()), param:empty())
	print("test_list empty",
			type(yggr.lua.s64_list_type():empty()),
			yggr.lua.typeid(yggr.lua.s64_list_type():empty()),
			yggr.lua.s64_list_type():empty())

	print("\n")
	print("test_list g_empty",
			type(yggr.lua.empty(param)),
			yggr.lua.typeid(yggr.lua.empty(param)),
			yggr.lua.empty(param))
	print("test_list g_empty",
			type(yggr.lua.empty(yggr.lua.s64_list_type())),
			yggr.lua.typeid(yggr.lua.empty(yggr.lua.s64_list_type())),
			yggr.lua.empty(yggr.lua.s64_list_type()))

	print("\n")
	print("test_list size", type(param:size()), yggr.lua.typeid(param:size()), param:size())
	print("test_list wraped_size", type(param:wraped_size()), yggr.lua.typeid(param:wraped_size()), param:wraped_size())
	print("test_list max_size", type(param:max_size()), yggr.lua.typeid(param:max_size()), param:max_size())
	print("test_list wraped_max_size", type(param:wraped_max_size()), yggr.lua.typeid(param:wraped_max_size()), param:wraped_max_size())

	print("\n")
	print("test_list g_size",
			type(yggr.lua.size(param)),
			yggr.lua.typeid(yggr.lua.size(param)),
			yggr.lua.size(param))
	print("test_list g_wraped_size",
			type(yggr.lua.wraped_size(param)),
			yggr.lua.typeid(yggr.lua.wraped_size(param)),
			yggr.lua.wraped_size(param))
	print("test_list g_ssize",
			type(yggr.lua.ssize(param)),
			yggr.lua.typeid(yggr.lua.ssize(param)),
			yggr.lua.ssize(param))
	print("test_list g_wraped_ssize",
			type(yggr.lua.wraped_ssize(param)),
			yggr.lua.typeid(yggr.lua.wraped_ssize(param)),
			yggr.lua.wraped_ssize(param))

	print("\n")
	local tmp_clear = param:copy()
	print("test_list clear", type(tmp_clear:size()), yggr.lua.typeid(tmp_clear:size()), tmp_clear:size())
	tmp_clear:clear()
	print("test_list clear", type(tmp_clear:size()), yggr.lua.typeid(tmp_clear:size()), tmp_clear:size())

	print("\n")
	tmp_clear = param:copy()
	print("test_list g_clear", type(tmp_clear:size()), yggr.lua.typeid(tmp_clear:size()), tmp_clear:size())
	yggr.lua.clear(tmp_clear)
	print("test_list g_clear", type(tmp_clear:size()), yggr.lua.typeid(tmp_clear:size()), tmp_clear:size())

	print("\n")
	print("test_list insert",
			type(param:insert(param:end_(), 10)),
			yggr.lua.typeid(param:insert(param:end_(), 10)),
			param:insert(param:end_(), 10),
			param)
	print("test_list insert",
			type(param:insert(param:cend_(), 20)),
			yggr.lua.typeid(param:insert(param:cend_(), 20)),
			param:insert(param:cend_(), 20),
			param)
	print("test_list insert",
			type(param:insert(param:end_(), 2, 30)),
			yggr.lua.typeid(param:insert(param:end_(), 2, 30)),
			param:insert(param:end_(), 2, 30),
			param)
	print("test_list insert",
			type(param:insert(param:cend_(), 2, 40)),
			yggr.lua.typeid(param:insert(param:cend_(), 2, 40)),
			param:insert(param:cend_(), 2, 40),
			param)
	print("test_list insert",
			type(param:insert(param:end_(), yggr.lua.s64(2), 50)),
			yggr.lua.typeid(param:insert(param:end_(), yggr.lua.s64(2), 50)),
			param:insert(param:end_(), yggr.lua.s64(2), 50),
			param)
	print("test_list insert",
			type(param:insert(param:cend_(), yggr.lua.u64(2), 60)),
			yggr.lua.typeid(param:insert(param:cend_(), yggr.lua.u64(2), 60)),
			param:insert(param:cend_(), yggr.lua.u64(2), 60),
			param)

	param:erase(yggr.lua.advance_copy(param:begin_(), 5), param:end_())
	local tmp_ins = param:copy()
	print("test_list insert",
			type(param:insert(param:end_(), yggr.lua.advance_copy(tmp_ins:begin_(), 0), yggr.lua.advance_copy(tmp_ins:begin_(), 2) )),
			yggr.lua.typeid(param:insert(param:end_(), yggr.lua.advance_copy(tmp_ins:begin_(), 0), yggr.lua.advance_copy(tmp_ins:begin_(), 2))),
			param:insert(param:end_(),  yggr.lua.advance_copy(tmp_ins:begin_(), 0), yggr.lua.advance_copy(tmp_ins:begin_(), 2)),
			param)
	print("test_list insert",
			type(param:insert(param:cend_(), yggr.lua.advance_copy(tmp_ins:begin_(), 0), yggr.lua.advance_copy(tmp_ins:begin_(), 2))),
			yggr.lua.typeid(param:insert(param:cend_(),  yggr.lua.advance_copy(tmp_ins:begin_(), 0), yggr.lua.advance_copy(tmp_ins:begin_(), 2))),
			param:insert(param:cend_(),  yggr.lua.advance_copy(tmp_ins:begin_(), 0), yggr.lua.advance_copy(tmp_ins:begin_(), 2)),
			param)
	print("test_list insert",
			type(param:insert(param:end_(), yggr.lua.advance_copy(tmp_ins:cbegin_(), 1), yggr.lua.advance_copy(tmp_ins:cbegin_(), 3))),
			yggr.lua.typeid(param:insert(param:end_(),  yggr.lua.advance_copy(tmp_ins:cbegin_(), 1), yggr.lua.advance_copy(tmp_ins:cbegin_(), 3))),
			param:insert(param:end_(),  yggr.lua.advance_copy(tmp_ins:cbegin_(), 1), yggr.lua.advance_copy(tmp_ins:cbegin_(), 3)),
			param)
	print("test_list insert",
			type(param:insert(param:cend_(), yggr.lua.advance_copy(tmp_ins:cbegin_(), 1), yggr.lua.advance_copy(tmp_ins:cbegin_(), 3))),
			yggr.lua.typeid(param:insert(param:cend_(),  yggr.lua.advance_copy(tmp_ins:cbegin_(), 1), yggr.lua.advance_copy(tmp_ins:cbegin_(), 3))),
			param:insert(param:cend_(),  yggr.lua.advance_copy(tmp_ins:cbegin_(), 1), yggr.lua.advance_copy(tmp_ins:cbegin_(), 3)),
			param)

	print("test_list insert",
			type(param:insert(param:end_(), yggr.lua.advance_copy(tmp_ins:rbegin_(), 0), yggr.lua.advance_copy(tmp_ins:rbegin_(), 2))),
			yggr.lua.typeid(param:insert(param:end_(),  yggr.lua.advance_copy(tmp_ins:rbegin_(), 0), yggr.lua.advance_copy(tmp_ins:rbegin_(), 2))),
			param:insert(param:end_(),  yggr.lua.advance_copy(tmp_ins:rbegin_(), 0), yggr.lua.advance_copy(tmp_ins:rbegin_(), 2)),
			param)
	print("test_list insert",
			type(param:insert(param:cend_(), yggr.lua.advance_copy(tmp_ins:rbegin_(), 0), yggr.lua.advance_copy(tmp_ins:rbegin_(), 2))),
			yggr.lua.typeid(param:insert(param:cend_(),  yggr.lua.advance_copy(tmp_ins:rbegin_(), 0), yggr.lua.advance_copy(tmp_ins:rbegin_(), 2))),
			param:insert(param:cend_(),  yggr.lua.advance_copy(tmp_ins:rbegin_(), 0), yggr.lua.advance_copy(tmp_ins:rbegin_(), 2)),
			param)
	print("test_list insert",
			type(param:insert(param:end_(), yggr.lua.advance_copy(tmp_ins:crbegin_(), 1), yggr.lua.advance_copy(tmp_ins:crbegin_(), 3))),
			yggr.lua.typeid(param:insert(param:end_(),  yggr.lua.advance_copy(tmp_ins:crbegin_(), 1), yggr.lua.advance_copy(tmp_ins:crbegin_(), 3))),
			param:insert(param:end_(),  yggr.lua.advance_copy(tmp_ins:crbegin_(), 1), yggr.lua.advance_copy(tmp_ins:crbegin_(), 3)),
			param)
	print("test_list insert",
			type(param:insert(param:cend_(), yggr.lua.advance_copy(tmp_ins:crbegin_(), 1), yggr.lua.advance_copy(tmp_ins:crbegin_(), 3))),
			yggr.lua.typeid(param:insert(param:cend_(),  yggr.lua.advance_copy(tmp_ins:crbegin_(), 1), yggr.lua.advance_copy(tmp_ins:crbegin_(), 3))),
			param:insert(param:cend_(),  yggr.lua.advance_copy(tmp_ins:crbegin_(), 1), yggr.lua.advance_copy(tmp_ins:crbegin_(), 3)),
			param)

	print("\n")
	print("test_list erase",
			type(param:erase(yggr.lua.advance_copy(param:begin_(), 5))),
			yggr.lua.typeid(param:erase(yggr.lua.advance_copy(param:begin_(), 5))),
			param:erase(yggr.lua.advance_copy(param:begin_(), 5)),
			param)
	print("test_list erase",
			type(param:erase(yggr.lua.advance_copy(param:cbegin_(), 5))),
			yggr.lua.typeid(param:erase(yggr.lua.advance_copy(param:cbegin_(), 5))),
			param:erase(yggr.lua.advance_copy(param:cbegin_(), 5)),
			param)

	print("test_list erase",
			type(param:erase(yggr.lua.advance_copy(param:begin_(), 5), yggr.lua.advance_copy(param:begin_(), 5 + 2))),
			yggr.lua.typeid(param:erase(yggr.lua.advance_copy(param:begin_(), 5), yggr.lua.advance_copy(param:begin_(), 5 + 2))),
			param:erase(yggr.lua.advance_copy(param:begin_(), 5), yggr.lua.advance_copy(param:begin_(), 5 + 2)),
			param)
	print("test_list erase",
			type(param:erase(yggr.lua.advance_copy(param:begin_(), 5), yggr.lua.advance_copy(param:cbegin_(), 5 + 2))),
			yggr.lua.typeid(param:erase(yggr.lua.advance_copy(param:begin_(), 5), yggr.lua.advance_copy(param:cbegin_(), 5 + 2))),
			param:erase(yggr.lua.advance_copy(param:begin_(), 5), yggr.lua.advance_copy(param:cbegin_(), 5 + 2)),
			param)
	print("test_list erase",
			type(param:erase(yggr.lua.advance_copy(param:cbegin_(), 5), yggr.lua.advance_copy(param:cbegin_(), 5 + 2))),
			yggr.lua.typeid(param:erase(yggr.lua.advance_copy(param:cbegin_(), 5), yggr.lua.advance_copy(param:cbegin_(), 5 + 2))),
			param:erase(yggr.lua.advance_copy(param:cbegin_(), 5), yggr.lua.advance_copy(param:cbegin_(), 5 + 2)),
			param)

	print("\n")
	param:erase(yggr.lua.advance_copy(param:begin_(), 5), param:end_())
	param:push_back(10)
	print("test_list push_back", type(param), yggr.lua.typeid(param), param)
	param:pop_back()
	print("test_list pop_back", type(param), yggr.lua.typeid(param), param)

	print("\n")
	param:push_front(20)
	print("test_list push_front", type(param), yggr.lua.typeid(param), param)
	param:pop_front()
	print("test_list push_front", type(param), yggr.lua.typeid(param), param)

	print("\n")
	param:resize(10)
	print("test_list resize", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	param:resize(yggr.lua.s64(20))
	print("test_list resize", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	param:resize(yggr.lua.u64(5))
	print("test_list resize", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)

	param:resize(10, 100)
	print("test_list resize", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	param:resize(yggr.lua.s64(20), 200)
	print("test_list resize", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	param:resize(yggr.lua.u64(5), 50)
	print("test_list resize", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)

	print("\n")
	yggr.lua.resize(param, 10)
	print("test_list g_resize", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	yggr.lua.resize(param, yggr.lua.s64(20))
	print("test_list g_resize", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	yggr.lua.resize(param, yggr.lua.u64(5))
	print("test_list g_resize", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)

	yggr.lua.resize(param, 10, 100)
	print("test_list g_resize", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	yggr.lua.resize(param, yggr.lua.s64(20), 200)
	print("test_list g_resize", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	yggr.lua.resize(param, yggr.lua.u64(5), 50)
	print("test_list g_resize", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)

	print("\n")
	param:erase(yggr.lua.advance_copy(param:begin_(), 5), param:end_())
	local merge_list = param:copy()
	param:merge(merge_list)
	print("test_list merge", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	print("test_list merge", type(merge_list:size()), yggr.lua.typeid(merge_list:size()), merge_list:size(), merge_list)

	merge_list = param:copy()
	param:merge_equal_to(merge_list)
	print("test_list merge_equal_to", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	print("test_list merge_equal_to", type(merge_list:size()), yggr.lua.typeid(merge_list:size()), merge_list:size(), merge_list)

	merge_list = param:copy()
	param:merge_not_equal_to(merge_list)
	print("test_list merge_not_equal_to", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	print("test_list merge_not_equal_to", type(merge_list:size()), yggr.lua.typeid(merge_list:size()), merge_list:size(), merge_list)

	merge_list = param:copy()
	param:merge_less(merge_list)
	print("test_list merge_less", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	print("test_list merge_less", type(merge_list:size()), yggr.lua.typeid(merge_list:size()), merge_list:size(), merge_list)

	merge_list = param:copy()
	param:merge_less_equal(merge_list)
	print("test_list merge_less_equal", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	print("test_list merge_less_equal", type(merge_list:size()), yggr.lua.typeid(merge_list:size()), merge_list:size(), merge_list)

	merge_list = param:copy()
	param:merge_greater(merge_list)
	print("test_list merge_greater", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	print("test_list merge_greater", type(merge_list:size()), yggr.lua.typeid(merge_list:size()), merge_list:size(), merge_list)

	merge_list = param:copy()
	param:merge_greater_equal(merge_list)
	print("test_list merge_greater_equal", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	print("test_list merge_greater_equal", type(merge_list:size()), yggr.lua.typeid(merge_list:size()), merge_list:size(), merge_list)

	print("\n")
	param:erase(yggr.lua.advance_copy(param:begin_(), 5), param:end_())

	local splice_list = param:copy()
	param:splice(param:end_(), splice_list)
	print("test_list splice", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	print("test_list splice", type(splice_list:size()), yggr.lua.typeid(splice_list:size()), splice_list:size(), splice_list)

	splice_list = param:copy()
	param:splice(param:cend_(), splice_list)
	print("test_list splice", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	print("test_list splice", type(splice_list:size()), yggr.lua.typeid(splice_list:size()), splice_list:size(), splice_list)

	splice_list = param:copy()
	param:splice(param:end_(), splice_list, splice_list:begin_())
	print("test_list splice", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	print("test_list splice", type(splice_list:size()), yggr.lua.typeid(splice_list:size()), splice_list:size(), splice_list)

	splice_list = param:copy()
	param:splice(param:end_(), splice_list, splice_list:cbegin_())
	print("test_list splice", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	print("test_list splice", type(splice_list:size()), yggr.lua.typeid(splice_list:size()), splice_list:size(), splice_list)

	splice_list = param:copy()
	param:splice(param:cend_(), splice_list, splice_list:begin_())
	print("test_list splice", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	print("test_list splice", type(splice_list:size()), yggr.lua.typeid(splice_list:size()), splice_list:size(), splice_list)

	splice_list = param:copy()
	param:splice(param:cend_(), splice_list, splice_list:cbegin_())
	print("test_list splice", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	print("test_list splice", type(splice_list:size()), yggr.lua.typeid(splice_list:size()), splice_list:size(), splice_list)

	splice_list = param:copy()
	param:splice(param:end_(), splice_list, splice_list:begin_(), splice_list:end_())
	print("test_list splice", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	print("test_list splice", type(splice_list:size()), yggr.lua.typeid(splice_list:size()), splice_list:size(), splice_list)

	splice_list = param:copy()
	param:splice(param:end_(), splice_list, splice_list:begin_(), splice_list:cend_())
	print("test_list splice", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	print("test_list splice", type(splice_list:size()), yggr.lua.typeid(splice_list:size()), splice_list:size(), splice_list)

	splice_list = param:copy()
	param:splice(param:end_(), splice_list, splice_list:cbegin_(), splice_list:end_())
	print("test_list splice", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	print("test_list splice", type(splice_list:size()), yggr.lua.typeid(splice_list:size()), splice_list:size(), splice_list)

	splice_list = param:copy()
	param:splice(param:end_(), splice_list, splice_list:cbegin_(), splice_list:cend_())
	print("test_list splice", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	print("test_list splice", type(splice_list:size()), yggr.lua.typeid(splice_list:size()), splice_list:size(), splice_list)

	splice_list = param:copy()
	param:splice(param:cend_(), splice_list, splice_list:begin_(), splice_list:end_())
	print("test_list splice", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	print("test_list splice", type(splice_list:size()), yggr.lua.typeid(splice_list:size()), splice_list:size(), splice_list)

	splice_list = param:copy()
	param:splice(param:cend_(), splice_list, splice_list:begin_(), splice_list:cend_())
	print("test_list splice", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	print("test_list splice", type(splice_list:size()), yggr.lua.typeid(splice_list:size()), splice_list:size(), splice_list)

	splice_list = param:copy()
	param:splice(param:cend_(), splice_list, splice_list:cbegin_(), splice_list:end_())
	print("test_list splice", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	print("test_list splice", type(splice_list:size()), yggr.lua.typeid(splice_list:size()), splice_list:size(), splice_list)

	splice_list = param:copy()
	param:splice(param:cend_(), splice_list, splice_list:cbegin_(), splice_list:cend_())
	print("test_list splice", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	print("test_list splice", type(splice_list:size()), yggr.lua.typeid(splice_list:size()), splice_list:size(), splice_list)

	print("\n")
	print("test_list remove", type(param:remove(1)), yggr.lua.typeid(param:remove(1)), param:remove(1), param)

	print("\n")
	for i = 1, 6 do
		param:push_back(i)
	end
	print("test_list wraped_remove", type(param:wraped_remove(6)), yggr.lua.typeid(param:wraped_remove(6)), param:wraped_remove(6), param)

	print("\n")
	param:reverse()
	print("test_list reverse", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)
	param:reverse()
	print("test_list reverse", type(param:size()), yggr.lua.typeid(param:size()), param:size(), param)

	print("\n")
	local unique_list = param:copy()
	unique_list:push_back(1)
	unique_list:push_back(1)
	print("test_list unique", unique_list:unique(), unique_list)

	unique_list = param:copy()
	unique_list:push_back(1)
	unique_list:push_back(1)
	print("test_list unique_equal_to", unique_list:unique_equal_to(), unique_list)

	unique_list = param:copy()
	print("test_list unique_not_equal_to", unique_list:unique_not_equal_to(), unique_list)

	unique_list = param:copy()
	print("test_list unique_less", unique_list:unique_less(), unique_list)

	unique_list = param:copy()
	print("test_list unique_less_equal", unique_list:unique_less_equal(), unique_list)

	unique_list = param:copy()
	print("test_list unique_greater", unique_list:unique_greater(), unique_list)

	unique_list = param:copy()
	print("test_list unique_greater_equal", unique_list:unique_greater_equal(), unique_list)

	print("\n")
	unique_list = param:copy()
	unique_list:push_back(1)
	unique_list:push_back(1)
	print("test_list wraped_unique", unique_list:wraped_unique(), unique_list)

	unique_list = param:copy()
	unique_list:push_back(1)
	unique_list:push_back(1)
	print("test_list wraped_unique_equal_to", unique_list:wraped_unique_equal_to(), unique_list)

	unique_list = param:copy()
	print("test_list wraped_unique_not_equal_to", unique_list:wraped_unique_not_equal_to(), unique_list)

	unique_list = param:copy()
	print("test_list wraped_unique_less", unique_list:wraped_unique_less(), unique_list)

	unique_list = param:copy()
	print("test_list wraped_unique_less_equal", unique_list:wraped_unique_less_equal(), unique_list)

	unique_list = param:copy()
	print("test_list wraped_unique_greater", unique_list:wraped_unique_greater(), unique_list)

	unique_list = param:copy()
	print("test_list wraped_unique_greater_equal", unique_list:wraped_unique_greater_equal(), unique_list)

	print("\n")
	param:reverse();
	param:sort();
	print("test_list sort", param)

	param:sort_aesc();
	print("test_list sort_aesc", param)

	param:sort_desc();
	print("test_list sort_desc", param)

	--]]

	return param
end


function calculate(param)
	print("\n")
	return test_s64_list_val_or_ref(param)
end


