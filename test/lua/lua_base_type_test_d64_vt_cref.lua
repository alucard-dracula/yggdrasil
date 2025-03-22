
require "yggr_lua_base_type"

local function test_d64_vt_cref(param)
	print("test_vector show", type(param), yggr.lua.typeid(param), param)

	print("\n")
	print("test_vector copy", type(param:copy()), yggr.lua.typeid(param:copy()), param:copy())


	print("\n")
	print("test_vector get_allocator", type(param:get_allocator()), yggr.lua.typeid(param:get_allocator()), param:get_allocator())

	print("\n")
	print("test_vector get_value", type(param:get_value(1)), yggr.lua.typeid(param:get_value(1)), param:get_value(1))
	print("test_vector get_value",
			type(param:get_value(yggr.lua.s64(1))),
			yggr.lua.typeid(param:get_value(yggr.lua.s64(1))),
			param:get_value(yggr.lua.s64(1)))
	print("test_vector get_value",
			type(param:get_value(yggr.lua.u64(1))),
			yggr.lua.typeid(param:get_value(yggr.lua.u64(1))),
			param:get_value(yggr.lua.u64(1)))

	print("\n")
	print("test_vector front", type(param:front()), yggr.lua.typeid(param:front()), param:front())

	print("\n")
	print("test_vector back", type(param:back()), yggr.lua.typeid(param:back()), param:back())

	print("\n")
	print("test_vector begin_", type(param:begin_()), yggr.lua.typeid(param:begin_()), param:begin_())
	print("test_vector cbegin_", type(param:cbegin_()), yggr.lua.typeid(param:cbegin_()), param:cbegin_())

	print("\n")
	print("test_vector end_", type(param:end_()), yggr.lua.typeid(param:end_()), param:end_())
	print("test_vector cend_", type(param:cend_()), yggr.lua.typeid(param:cend_()), param:cend_())

	print("\n")
	print("test_vector rbegin_", type(param:rbegin_()), yggr.lua.typeid(param:rbegin_()), param:rbegin_())
	print("test_vector crbegin_", type(param:crbegin_()), yggr.lua.typeid(param:crbegin_()), param:crbegin_())

	print("\n")
	print("test_vector rend_", type(param:rend_()), yggr.lua.typeid(param:rend_()), param:rend_())
	print("test_vector crend_", type(param:crend_()), yggr.lua.typeid(param:crend_()), param:crend_())

	print("\n")
	print("test_vector g_begin_",
			type(yggr.lua.begin_(param)),
			yggr.lua.typeid(yggr.lua.begin_(param)),
			yggr.lua.begin_(param))
	print("test_vector g_cbegin_",
			type(yggr.lua.cbegin_(param)),
			yggr.lua.typeid(yggr.lua.cbegin_(param)),
			yggr.lua.cbegin_(param))

	print("\n")
	print("test_vector g_end_",
			type(yggr.lua.end_(param)),
			yggr.lua.typeid(yggr.lua.end_(param)),
			yggr.lua.end_(param))
	print("test_vector g_cend_",
			type(yggr.lua.cend_(param)),
			yggr.lua.typeid(yggr.lua.cend_(param)),
			yggr.lua.cend_(param))

	print("\n")
	print("test_vector g_rbegin_",
			type(yggr.lua.rbegin_(param)),
			yggr.lua.typeid(yggr.lua.rbegin_(param)),
			yggr.lua.rbegin_(param))
	print("test_vector g_crbegin_",
			type(yggr.lua.crbegin_(param)),
			yggr.lua.typeid(yggr.lua.crbegin_(param)),
			yggr.lua.crbegin_(param))

	print("\n")
	print("test_vector g_rend_",
			type(yggr.lua.rend_(param)),
			yggr.lua.typeid(yggr.lua.rend_(param)),
			yggr.lua.rend_(param))
	print("test_vector g_crend_",
			type(yggr.lua.crend_(param)),
			yggr.lua.typeid(yggr.lua.crend_(param)),
			yggr.lua.crend_(param))

	print("\n")
	print("test_vector empty", type(param:empty()), yggr.lua.typeid(param:empty()), param:empty())
	print("test_vector empty",
			type(yggr.lua.s64_vector_type():empty()),
			yggr.lua.typeid(yggr.lua.s64_vector_type():empty()),
			yggr.lua.s64_vector_type():empty())

	print("\n")
	print("test_vector g_empty",
			type(yggr.lua.empty(param)),
			yggr.lua.typeid(yggr.lua.empty(param)),
			yggr.lua.empty(param))

	print("\n")
	print("test_vector size", type(param:size()), yggr.lua.typeid(param:size()), param:size())
	print("test_vector wraped_size", type(param:wraped_size()), yggr.lua.typeid(param:wraped_size()), param:wraped_size())
	print("test_vector max_size", type(param:max_size()), yggr.lua.typeid(param:max_size()), param:max_size())
	print("test_vector wraped_max_size", type(param:wraped_max_size()), yggr.lua.typeid(param:wraped_max_size()), param:wraped_max_size())

	print("\n")
	print("test_vector g_size",
			type(yggr.lua.size(param)),
			yggr.lua.typeid(yggr.lua.size(param)),
			yggr.lua.size(param))
	print("test_vector g_wraped_size",
			type(yggr.lua.wraped_size(param)),
			yggr.lua.typeid(yggr.lua.wraped_size(param)),
			yggr.lua.wraped_size(param))
	print("test_vector g_ssize",
			type(yggr.lua.ssize(param)),
			yggr.lua.typeid(yggr.lua.ssize(param)),
			yggr.lua.ssize(param))
	print("test_vector g_wraped_ssize",
			type(yggr.lua.wraped_ssize(param)),
			yggr.lua.typeid(yggr.lua.wraped_ssize(param)),
			yggr.lua.wraped_ssize(param))

	print("\n")
	print("test_vector capacity", type(param:capacity()), yggr.lua.typeid(param:capacity()), param:capacity())
	print("test_vector wraped_capacity", type(param:wraped_capacity()), yggr.lua.typeid(param:wraped_capacity()), param:wraped_capacity())


	return param
end


function calculate(param)
	print("\n")
	return test_d64_vt_cref(param)
end


