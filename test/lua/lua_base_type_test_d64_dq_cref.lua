
require "yggr_lua_base_type"

local function test_d64_dq_cref(param)
	print("test_deque show", type(param), yggr.lua.typeid(param), param)

	print("\n")
	print("test_deque copy", type(param:copy()), yggr.lua.typeid(param:copy()), param:copy())

	print("\n")
	print("test_deque get_allocator", type(param:get_allocator()), yggr.lua.typeid(param:get_allocator()), param:get_allocator())

	print("\n")
	print("test_deque get_value", type(param:get_value(1)), yggr.lua.typeid(param:get_value(1)), param:get_value(1))
	print("test_deque get_value",
			type(param:get_value(yggr.lua.s64(1))),
			yggr.lua.typeid(param:get_value(yggr.lua.s64(1))),
			param:get_value(yggr.lua.s64(1)))
	print("test_deque get_value",
			type(param:get_value(yggr.lua.u64(1))),
			yggr.lua.typeid(param:get_value(yggr.lua.u64(1))),
			param:get_value(yggr.lua.u64(1)))


	print("\n")
	print("test_deque front", type(param:front()), yggr.lua.typeid(param:front()), param:front())

	print("\n")
	print("test_deque back", type(param:back()), yggr.lua.typeid(param:back()), param:back())

	print("\n")
	print("test_deque begin_", type(param:begin_()), yggr.lua.typeid(param:begin_()), param:begin_())
	print("test_deque cbegin_", type(param:cbegin_()), yggr.lua.typeid(param:cbegin_()), param:cbegin_())

	print("\n")
	print("test_deque end_", type(param:end_()), yggr.lua.typeid(param:end_()), param:end_())
	print("test_deque cend_", type(param:cend_()), yggr.lua.typeid(param:cend_()), param:cend_())

	print("\n")
	print("test_deque rbegin_", type(param:rbegin_()), yggr.lua.typeid(param:rbegin_()), param:rbegin_())
	print("test_deque crbegin_", type(param:crbegin_()), yggr.lua.typeid(param:crbegin_()), param:crbegin_())

	print("\n")
	print("test_deque rend_", type(param:rend_()), yggr.lua.typeid(param:rend_()), param:rend_())
	print("test_deque crend_", type(param:crend_()), yggr.lua.typeid(param:crend_()), param:crend_())

	print("\n")
	print("test_deque g_begin_",
			type(yggr.lua.begin_(param)),
			yggr.lua.typeid(yggr.lua.begin_(param)),
			yggr.lua.begin_(param))
	print("test_deque g_cbegin_",
			type(yggr.lua.cbegin_(param)),
			yggr.lua.typeid(yggr.lua.cbegin_(param)),
			yggr.lua.cbegin_(param))

	print("\n")
	print("test_deque g_end_",
			type(yggr.lua.end_(param)),
			yggr.lua.typeid(yggr.lua.end_(param)),
			yggr.lua.end_(param))
	print("test_deque g_cend_",
			type(yggr.lua.cend_(param)),
			yggr.lua.typeid(yggr.lua.cend_(param)),
			yggr.lua.cend_(param))

	print("\n")
	print("test_deque g_rbegin_",
			type(yggr.lua.rbegin_(param)),
			yggr.lua.typeid(yggr.lua.rbegin_(param)),
			yggr.lua.rbegin_(param))
	print("test_deque g_crbegin_",
			type(yggr.lua.crbegin_(param)),
			yggr.lua.typeid(yggr.lua.crbegin_(param)),
			yggr.lua.crbegin_(param))

	print("\n")
	print("test_deque g_rend_",
			type(yggr.lua.rend_(param)),
			yggr.lua.typeid(yggr.lua.rend_(param)),
			yggr.lua.rend_(param))
	print("test_deque g_crend_",
			type(yggr.lua.crend_(param)),
			yggr.lua.typeid(yggr.lua.crend_(param)),
			yggr.lua.crend_(param))

	print("\n")
	print("test_deque empty", type(param:empty()), yggr.lua.typeid(param:empty()), param:empty())
	print("test_deque empty",
			type(yggr.lua.s64_deque_type():empty()),
			yggr.lua.typeid(yggr.lua.s64_deque_type():empty()),
			yggr.lua.s64_deque_type():empty())

	print("\n")
	print("test_deque g_empty",
			type(yggr.lua.empty(param)),
			yggr.lua.typeid(yggr.lua.empty(param)),
			yggr.lua.empty(param))

	print("\n")
	print("test_deque size", type(param:size()), yggr.lua.typeid(param:size()), param:size())
	print("test_deque wraped_size", type(param:wraped_size()), yggr.lua.typeid(param:wraped_size()), param:wraped_size())
	print("test_deque max_size", type(param:max_size()), yggr.lua.typeid(param:max_size()), param:max_size())
	print("test_deque wraped_max_size", type(param:wraped_max_size()), yggr.lua.typeid(param:wraped_max_size()), param:wraped_max_size())

	print("\n")
	print("test_deque g_size",
			type(yggr.lua.size(param)),
			yggr.lua.typeid(yggr.lua.size(param)),
			yggr.lua.size(param))
	print("test_deque g_wraped_size",
			type(yggr.lua.wraped_size(param)),
			yggr.lua.typeid(yggr.lua.wraped_size(param)),
			yggr.lua.wraped_size(param))
	print("test_deque g_ssize",
			type(yggr.lua.ssize(param)),
			yggr.lua.typeid(yggr.lua.ssize(param)),
			yggr.lua.ssize(param))
	print("test_deque g_wraped_ssize",
			type(yggr.lua.wraped_ssize(param)),
			yggr.lua.typeid(yggr.lua.wraped_ssize(param)),
			yggr.lua.wraped_ssize(param))

	return param
end


function calculate(param)
	print("\n")
	return test_d64_dq_cref(param)
end


