

local function test_int64(param)
	print("lua_int64", param)
	return param
end

local function test_uint64(param)
	print("lua_uint64", param)
	return param
end

function calculate_int64(param)
	return test_int64(param)
end

function calculate_uint64(param)
	return test_uint64(param)
end


