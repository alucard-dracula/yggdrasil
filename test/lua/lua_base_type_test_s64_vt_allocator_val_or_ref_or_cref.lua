
require "yggr_lua_base_type"

local function test_s64_vt_allocator_val_or_ref_or_cref(param)
	print("test_allocator show", type(param), yggr.lua.typeid(param), param)
	print("test_allocator get_allocator", type(param:get_allocator()), yggr.lua.typeid(param:get_allocator()), param:get_allocator())
	print("test_allocator copy", type(param:copy()), yggr.lua.typeid(param:copy()), param:copy())
	print("test_allocator max_size", type(param:max_size()), yggr.lua.typeid(param:max_size()), param:max_size())
	print("test_allocator wraped_max_size", type(param:wraped_max_size()), yggr.lua.typeid(param:wraped_max_size()), param:wraped_max_size())
	print("test_allocator ==",
			type(param == yggr.lua.s64_allocator_type()),
			yggr.lua.typeid(param == yggr.lua.s64_allocator_type()),
			param == yggr.lua.s64_allocator_type())

	print("test_allocator ~=",
			type(param ~= yggr.lua.s64_allocator_type()),
			yggr.lua.typeid(param ~= yggr.lua.s64_allocator_type()),
			param ~= yggr.lua.s64_allocator_type())

	return param:wraped_max_size()
end


function calculate(param)
	print("\n")
	return test_s64_vt_allocator_val_or_ref_or_cref(param)
end


