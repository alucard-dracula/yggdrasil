
require "yggr_lua_base_type"

local function test_u64(param)
	print("lua", param)
	return param + 100
end


function calculate(param)
	return test_u64(param)
end


