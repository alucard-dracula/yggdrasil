
require "yggr_lua_base_type"

local function test_typeinfo(param)
	print("lua_test_typeinfo:", type(true), yggr.lua.typeid(true), yggr.lua.typeid(true):name())
	print("lua_test_typeinfo:", type(100), yggr.lua.typeid(100), yggr.lua.typeid(100):name())
	print("lua_test_typeinfo:", type("100"), yggr.lua.typeid("100"), yggr.lua.typeid("100"):name())
	print("lua_test_typeinfo:", type(param), yggr.lua.typeid(param), yggr.lua.typeid(param):name())
end


function calculate(param)
	print("\n")
	test_typeinfo(param)
	return param + 100
end


